/****************************************************************************
 * sched/sched/sched_mergepending.c
 *
 *   Copyright (C) 2007, 2009, 2012, 2016 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <stdbool.h>
#include <sched.h>
#include <queue.h>
#include <assert.h>

#ifdef CONFIG_SMP
#  include <nuttx/spinlock.h>
#endif

#include "sched/sched.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define ALL_CPUS ((cpu_set_t)-1)

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: sched_mergepending
 *
 * Description:
 *   This function merges the prioritized g_pendingtasks list into the
 *   prioritized ready-to-run task list.
 *
 * Inputs:
 *   None
 *
 * Return Value:
 *   true if the head of the ready-to-run task list has changed indicating
 *     a context switch is needed.
 *
 * Assumptions:
 * - The caller has established a critical section before
 *   calling this function (calling sched_lock() first is NOT
 *   a good idea -- use enter_critical_section()).
 * - The caller handles the condition that occurs if the
 *   the head of the sched_mergTSTATE_TASK_PENDINGs is changed.
 *
 ****************************************************************************/

#ifndef CONFIG_SMP
bool sched_mergepending(void)
{
  FAR struct tcb_s *ptcb;
  FAR struct tcb_s *pnext;
  FAR struct tcb_s *rtcb;
  FAR struct tcb_s *rprev;
  bool ret = false;

  /* Initialize the inner search loop */

  rtcb = this_task();

  /* Process every TCB in the g_pendingtasks list */

  for (ptcb = (FAR struct tcb_s *)g_pendingtasks.head;
       ptcb;
       ptcb = pnext)
    {
      pnext = ptcb->flink;

      /* REVISIT:  Why don't we just remove the ptcb from pending task list
       * and call sched_addreadytorun?
       */

      /* Search the ready-to-run list to find the location to insert the
       * new ptcb. Each is list is maintained in ascending sched_priority
       * order.
       */

      for (;
           (rtcb && ptcb->sched_priority <= rtcb->sched_priority);
           rtcb = rtcb->flink);

      /* Add the ptcb to the spot found in the list.  Check if the
       * ptcb goes at the ends of the ready-to-run list. This would be
       * error condition since the idle test must always be at the end of
       * the ready-to-run list!
       */

      ASSERT(rtcb);

      /* The ptcb goes just before rtcb */

      rprev = rtcb->blink;
      if (!rprev)
        {
          /* Special case: Inserting ptcb at the head of the list */

          ptcb->flink       = rtcb;
          ptcb->blink       = NULL;
          rtcb->blink       = ptcb;
          g_readytorun.head = (FAR dq_entry_t *)ptcb;
          rtcb->task_state  = TSTATE_TASK_READYTORUN;
          ptcb->task_state  = TSTATE_TASK_RUNNING;
          ret               = true;
        }
      else
        {
          /* Insert in the middle of the list */

          ptcb->flink       = rtcb;
          ptcb->blink       = rprev;
          rprev->flink      = ptcb;
          rtcb->blink       = ptcb;
          ptcb->task_state  = TSTATE_TASK_READYTORUN;
        }

      /* Set up for the next time through */

      rtcb = ptcb;
    }

  /* Mark the input list empty */

  g_pendingtasks.head = NULL;
  g_pendingtasks.tail = NULL;

  return ret;
}
#endif /* !CONFIG_SMP */

/****************************************************************************
 * Name: sched_mergepending
 *
 * Description:
 *   This function merges the prioritized g_pendingtasks list into the
 *   prioritized ready-to-run task list.
 *
 * Inputs:
 *   None
 *
 * Return Value:
 *   true if the head of the ready-to-run task list has changed indicating
 *     a context switch is needed.
 *
 * Assumptions:
 * - The caller has established a critical section before
 *   calling this function (calling sched_lock() first is NOT
 *   a good idea -- use enter_critical_section()).
 * - The caller handles the condition that occurs if the
 *   the head of the sched_mergTSTATE_TASK_PENDINGs is changed.
 *
 ****************************************************************************/

#ifdef CONFIG_SMP
bool sched_mergepending(void)
{
  FAR struct tcb_s *rtcb;
  FAR struct tcb_s *ptcb;
  FAR struct tcb_s *tcb;
  bool ret = false;
  int cpu;

  /* Remove and process every TCB in the g_pendingtasks list.
   *
   * This function is only called in the context where locking is known to
   * disabled on one CPU.  However, we must do nothing if pre-emption is
   * still locked because of actions of other CPUs.
   */

  if (!spin_islocked(&g_cpu_schedlock))
    {
      /* Find the CPU that is executing the lowest priority task */

      ptcb = (FAR struct tcb_s *)dq_peek((FAR dq_queue_t *)&g_pendingtasks);
      if (ptcb == NULL)
        {
          /* The pending task list is empty */

          return ret;
        }

      cpu  = sched_cpu_select(ALL_CPUS /* ptcb->affinity */);
      rtcb = current_task(cpu);

      /* Loop while there is a higher priority task in the pending task list
       * than in the lowest executing task.
       *
       * Normally, this loop should execute no more than CONFIG_SMP_NCPUS
       * times.  That number could be larger, however, if the CPU affinity
       * sets do not include all CPUs. In that case, the excess TCBs will
       * end up in the g_readytorun list.
       */

      while (ptcb->sched_priority > rtcb->sched_priority)
        {
          /* Remove the task from the pending task list */

          tcb = (FAR struct tcb_s *)dq_remfirst((FAR dq_queue_t *)&g_pendingtasks);
          DEBUGASSERT(tcb == ptcb);

          /* Add the pending task to the correct ready-to-run list. */

          ret |= sched_addreadytorun(tcb);

          /* This operation could cause the scheduler to become locked.
           * Check if that happened.
           */

          if (spin_islocked(&g_cpu_schedlock))
            {
              /* Yes.. then we may have incorrectly placed some TCBs in the
               * g_readytorun list (unlikely, but possible).  We will have to
               * move them back to the pending task list.
               */

              sched_mergeprioritized((FAR dq_queue_t *)&g_readytorun,
                                     (FAR dq_queue_t *)&g_pendingtasks);

              /* And return with the schedule locked and tasks in the
               * pending task list.
               */

              return ret;
            }

          /* Set up for the next time through the loop */

          ptcb = (FAR struct tcb_s *)dq_peek((FAR dq_queue_t *)&g_pendingtasks);
          if (ptcb == NULL)
            {
              /* The pending task list is empty */

              return ret;
            }

          cpu = sched_cpu_select(ALL_CPUS /* ptcb->affinity */);
          rtcb = current_task(cpu);
        }

      /* No more pending tasks can be made running.  Move any reamaining
       * tasks in the pending task list to the ready-to-run task list.
       */

      sched_mergeprioritized((FAR dq_queue_t *)&g_pendingtasks,
                             (FAR dq_queue_t *)&g_readytorun);
    }

  return ret;
}
#endif /* CONFIG_SMP */
