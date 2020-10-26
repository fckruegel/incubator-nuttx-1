/*****************************************************************************
 * arch/arm/src/stm32l4/stm32l4_adc.h
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 *****************************************************************************/

#ifndef __ARCH_ARM_SRC_STM32L4_STM32L4_ADC_H
#define __ARCH_ARM_SRC_STM32L4_STM32L4_ADC_H

/*****************************************************************************
 * Included Files
 *****************************************************************************/

#include <nuttx/config.h>
#include <nuttx/analog/adc.h>
#include "chip.h"
#include "hardware/stm32l4_adc.h"

/*****************************************************************************
 * Pre-processor Definitions
 *****************************************************************************/

/* Configuration *************************************************************/

/* Timer devices may be used for different purposes.  One special purpose is
 * to control periodic ADC sampling.  If CONFIG_STM32L4_TIMn is defined then
 * CONFIG_STM32L4_TIMn_ADC must also be defined to indicate that timer "n" is
 * intended to be used for that purpose. Timers 1,2,3,6 and 15 may be used on
 * STM32L4X3, while STM32L4X6 adds support for timers 4 and 8 as well.
 */

#ifndef CONFIG_STM32L4_TIM1
#  undef CONFIG_STM32L4_TIM1_ADC
#  undef CONFIG_STM32L4_TIM1_ADC1
#  undef CONFIG_STM32L4_TIM1_ADC2
#  undef CONFIG_STM32L4_TIM1_ADC3
#endif
#ifndef CONFIG_STM32L4_TIM2
#  undef CONFIG_STM32L4_TIM2_ADC
#  undef CONFIG_STM32L4_TIM2_ADC1
#  undef CONFIG_STM32L4_TIM2_ADC2
#  undef CONFIG_STM32L4_TIM2_ADC3
#endif
#ifndef CONFIG_STM32L4_TIM3
#  undef CONFIG_STM32L4_TIM3_ADC
#  undef CONFIG_STM32L4_TIM3_ADC1
#  undef CONFIG_STM32L4_TIM3_ADC2
#  undef CONFIG_STM32L4_TIM3_ADC3
#endif
#ifndef CONFIG_STM32L4_TIM4
#  undef CONFIG_STM32L4_TIM4_ADC
#  undef CONFIG_STM32L4_TIM4_ADC1
#  undef CONFIG_STM32L4_TIM4_ADC2
#  undef CONFIG_STM32L4_TIM4_ADC3
#endif
#ifndef CONFIG_STM32L4_TIM6
#  undef CONFIG_STM32L4_TIM6_ADC
#  undef CONFIG_STM32L4_TIM6_ADC1
#  undef CONFIG_STM32L4_TIM6_ADC2
#  undef CONFIG_STM32L4_TIM6_ADC3
#endif
#ifndef CONFIG_STM32L4_TIM8
#  undef CONFIG_STM32L4_TIM8_ADC
#  undef CONFIG_STM32L4_TIM8_ADC1
#  undef CONFIG_STM32L4_TIM8_ADC2
#  undef CONFIG_STM32L4_TIM8_ADC3
#endif
#ifndef CONFIG_STM32L4_TIM15
#  undef CONFIG_STM32L4_TIM15_ADC
#  undef CONFIG_STM32L4_TIM15_ADC1
#  undef CONFIG_STM32L4_TIM15_ADC2
#  undef CONFIG_STM32L4_TIM15_ADC3
#endif

/* Up to 3 ADC interfaces are supported */

#if STM32L4_NADC < 3
#  undef CONFIG_STM32L4_ADC3
#endif

#if STM32L4_NADC < 2
#  undef CONFIG_STM32L4_ADC2
#endif

#if STM32L4_NADC < 1
#  undef CONFIG_STM32L4_ADC1
#endif

#if defined(CONFIG_STM32L4_ADC1) || defined(CONFIG_STM32L4_ADC2) || \
    defined(CONFIG_STM32L4_ADC3)

/* ADC output to DFSDM support. Note that DFSDM and DMA are
 * mutually exclusive.
 */

#undef ADC_HAVE_DFSDM
#if defined(CONFIG_STM32L4_ADC1_OUTPUT_DFSDM) || \
    defined(CONFIG_STM32L4_ADC2_OUTPUT_DFSDM) || \
    defined(CONFIG_STM32L4_ADC3_OUTPUT_DFSDM)
#  define ADC_HAVE_DFSDM
#endif

#if defined(CONFIG_STM32L4_ADC1_OUTPUT_DFSDM)
#  define ADC1_HAVE_DFSDM 1
#  undef  CONFIG_STM32L4_ADC1_DMA
#else
#  undef  ADC1_HAVE_DFSDM
#endif

#if defined(CONFIG_STM32L4_ADC2_OUTPUT_DFSDM)
#  define ADC2_HAVE_DFSDM 1
#  undef  CONFIG_STM32L4_ADC2_DMA
#else
#  undef  ADC2_HAVE_DFSDM
#endif

#if defined(CONFIG_STM32L4_ADC3_OUTPUT_DFSDM)
#  define ADC3_HAVE_DFSDM 1
#  undef  CONFIG_STM32L4_ADC3_DMA
#else
#  undef  ADC3_HAVE_DFSDM
#endif

/* DMA support */

#undef ADC_HAVE_DMA
#if defined(CONFIG_STM32L4_ADC1_DMA) || defined(CONFIG_STM32L4_ADC2_DMA) || \
    defined(CONFIG_STM32L4_ADC3_DMA)
#  define ADC_HAVE_DMA  1
#endif

#ifdef CONFIG_STM32L4_ADC1_DMA
#  define ADC1_HAVE_DMA 1
#else
#  undef  ADC1_HAVE_DMA
#endif

#ifdef CONFIG_STM32L4_ADC2_DMA
#  define ADC2_HAVE_DMA 1
#else
#  undef  ADC2_HAVE_DMA
#endif

#ifdef CONFIG_STM32L4_ADC3_DMA
#  define ADC3_HAVE_DMA 1
#else
#  undef  ADC3_HAVE_DMA
#endif

/* Timer configuration:  If a timer trigger is specified, then get
 * information about the timer.
 */

#if defined(CONFIG_STM32L4_TIM1_ADC1)
#    define ADC1_HAVE_TIMER           1
#    define ADC1_TIMER_BASE           STM32L4_TIM1_BASE
#    define ADC1_TIMER_PCLK_FREQUENCY STM32L4_APB2_TIM1_CLKIN
#elif defined(CONFIG_STM32L4_TIM2_ADC1)
#    define ADC1_HAVE_TIMER           1
#    define ADC1_TIMER_BASE           STM32L4_TIM2_BASE
#    define ADC1_TIMER_PCLK_FREQUENCY STM32L4_APB1_TIM2_CLKIN
#elif defined(CONFIG_STM32L4_TIM3_ADC1)
#    define ADC1_HAVE_TIMER           1
#    define ADC1_TIMER_BASE           STM32L4_TIM3_BASE
#    define ADC1_TIMER_PCLK_FREQUENCY STM32L4_APB1_TIM3_CLKIN
#elif defined(CONFIG_STM32L4_TIM4_ADC1)
#    define ADC1_HAVE_TIMER           1
#    define ADC1_TIMER_BASE           STM32L4_TIM4_BASE
#    define ADC1_TIMER_PCLK_FREQUENCY STM32L4_APB1_TIM4_CLKIN
#elif defined(CONFIG_STM32L4_TIM6_ADC1)
#    define ADC1_HAVE_TIMER           1
#    define ADC1_TIMER_BASE           STM32L4_TIM6_BASE
#    define ADC1_TIMER_PCLK_FREQUENCY STM32L4_APB1_TIM6_CLKIN
#elif defined(CONFIG_STM32L4_TIM8_ADC1)
#    define ADC1_HAVE_TIMER           1
#    define ADC1_TIMER_BASE           STM32L4_TIM8_BASE
#    define ADC1_TIMER_PCLK_FREQUENCY STM32L4_APB2_TIM8_CLKIN
#elif defined(CONFIG_STM32L4_TIM15_ADC1)
#    define ADC1_HAVE_TIMER           1
#    define ADC1_TIMER_BASE           STM32L4_TIM15_BASE
#    define ADC1_TIMER_PCLK_FREQUENCY STM32L4_APB2_TIM15_CLKIN
#else
#    undef  ADC1_HAVE_TIMER
#endif

#ifdef ADC1_HAVE_TIMER
#  ifndef CONFIG_STM32L4_ADC1_SAMPLE_FREQUENCY
#    error "CONFIG_STM32L4_ADC1_SAMPLE_FREQUENCY not defined"
#  endif
#  ifndef CONFIG_STM32L4_ADC1_TIMTRIG
#    error "CONFIG_STM32L4_ADC1_TIMTRIG not defined"
#    warning "Values 0:CC1 1:CC2 2:CC3 3:CC4 4:TRGO"
#  endif
#endif

#if defined(CONFIG_STM32L4_TIM1_ADC2)
#    define ADC2_HAVE_TIMER           1
#    define ADC2_TIMER_BASE           STM32L4_TIM1_BASE
#    define ADC2_TIMER_PCLK_FREQUENCY STM32L4_APB2_TIM1_CLKIN
#elif defined(CONFIG_STM32L4_TIM2_ADC2)
#    define ADC2_HAVE_TIMER           1
#    define ADC2_TIMER_BASE           STM32L4_TIM2_BASE
#    define ADC2_TIMER_PCLK_FREQUENCY STM32L4_APB1_TIM2_CLKIN
#elif defined(CONFIG_STM32L4_TIM3_ADC2)
#    define ADC2_HAVE_TIMER           1
#    define ADC2_TIMER_BASE           STM32L4_TIM3_BASE
#    define ADC2_TIMER_PCLK_FREQUENCY STM32L4_APB1_TIM3_CLKIN
#elif defined(CONFIG_STM32L4_TIM4_ADC2)
#    define ADC2_HAVE_TIMER           1
#    define ADC2_TIMER_BASE           STM32L4_TIM4_BASE
#    define ADC2_TIMER_PCLK_FREQUENCY STM32L4_APB1_TIM4_CLKIN
#elif defined(CONFIG_STM32L4_TIM6_ADC2)
#    define ADC2_HAVE_TIMER           1
#    define ADC2_TIMER_BASE           STM32L4_TIM6_BASE
#    define ADC2_TIMER_PCLK_FREQUENCY STM32L4_APB1_TIM6_CLKIN
#elif defined(CONFIG_STM32L4_TIM8_ADC2)
#    define ADC2_HAVE_TIMER           1
#    define ADC2_TIMER_BASE           STM32L4_TIM8_BASE
#    define ADC2_TIMER_PCLK_FREQUENCY STM32L4_APB2_TIM8_CLKIN
#elif defined(CONFIG_STM32L4_TIM15_ADC2)
#    define ADC2_HAVE_TIMER           1
#    define ADC2_TIMER_BASE           STM32L4_TIM15_BASE
#    define ADC2_TIMER_PCLK_FREQUENCY STM32L4_APB2_TIM15_CLKIN
#else
#    undef  ADC2_HAVE_TIMER
#endif

#ifdef ADC2_HAVE_TIMER
#  ifndef CONFIG_STM32L4_ADC2_SAMPLE_FREQUENCY
#    error "CONFIG_STM32L4_ADC2_SAMPLE_FREQUENCY not defined"
#  endif
#  ifndef CONFIG_STM32L4_ADC2_TIMTRIG
#    error "CONFIG_STM32L4_ADC2_TIMTRIG not defined"
#    warning "Values 0:CC1 1:CC2 2:CC3 3:CC4 4:TRGO"
#  endif
#endif

#if defined(CONFIG_STM32L4_TIM1_ADC3)
#    define ADC3_HAVE_TIMER           1
#    define ADC3_TIMER_BASE           STM32L4_TIM1_BASE
#    define ADC3_TIMER_PCLK_FREQUENCY STM32L4_APB2_TIM1_CLKIN
#elif defined(CONFIG_STM32L4_TIM2_ADC3)
#    define ADC3_HAVE_TIMER           1
#    define ADC3_TIMER_BASE           STM32L4_TIM2_BASE
#    define ADC3_TIMER_PCLK_FREQUENCY STM32L4_APB1_TIM2_CLKIN
#elif defined(CONFIG_STM32L4_TIM3_ADC3)
#    define ADC3_HAVE_TIMER           1
#    define ADC3_TIMER_BASE           STM32L4_TIM3_BASE
#    define ADC3_TIMER_PCLK_FREQUENCY STM32L4_APB1_TIM3_CLKIN
#elif defined(CONFIG_STM32L4_TIM4_ADC3)
#    define ADC3_HAVE_TIMER           1
#    define ADC3_TIMER_BASE           STM32L4_TIM4_BASE
#    define ADC3_TIMER_PCLK_FREQUENCY STM32L4_APB1_TIM4_CLKIN
#elif defined(CONFIG_STM32L4_TIM6_ADC3)
#    define ADC3_HAVE_TIMER           1
#    define ADC3_TIMER_BASE           STM32L4_TIM6_BASE
#    define ADC3_TIMER_PCLK_FREQUENCY STM32L4_APB1_TIM6_CLKIN
#elif defined(CONFIG_STM32L4_TIM8_ADC3)
#    define ADC3_HAVE_TIMER           1
#    define ADC3_TIMER_BASE           STM32L4_TIM8_BASE
#    define ADC3_TIMER_PCLK_FREQUENCY STM32L4_APB2_TIM8_CLKIN
#elif defined(CONFIG_STM32L4_TIM15_ADC3)
#    define ADC3_HAVE_TIMER           1
#    define ADC3_TIMER_BASE           STM32L4_TIM15_BASE
#    define ADC3_TIMER_PCLK_FREQUENCY STM32L4_APB2_TIM15_CLKIN
#else
#    undef  ADC3_HAVE_TIMER
#endif

#ifdef ADC3_HAVE_TIMER
#  ifndef CONFIG_STM32L4_ADC3_SAMPLE_FREQUENCY
#    error "CONFIG_STM32L4_ADC3_SAMPLE_FREQUENCY not defined"
#  endif
#  ifndef CONFIG_STM32L4_ADC3_TIMTRIG
#    error "CONFIG_STM32L4_ADC3_TIMTRIG not defined"
#    warning "Values 0:CC1 1:CC2 2:CC3 3:CC4 4:TRGO"
#  endif
#endif

#if defined(ADC1_HAVE_TIMER) || defined(ADC2_HAVE_TIMER) || \
    defined(ADC3_HAVE_TIMER)
#  define ADC_HAVE_TIMER 1
#else
#  undef ADC_HAVE_TIMER
#endif

/* NOTE:  The following assumes that all possible combinations of timers and
 * values are support EXTSEL.  That is not so and it varies from one STM32
 * to another.  But this (wrong) assumptions keeps the logic as simple as
 * possible.  If unsupported combination is used, an error will show up
 * later during compilation although it may be difficult to track it back
 * to this simplification.
 */

#define ADC1_EXTSEL_T1CC1      ADC_CFGR_EXTSEL_T1CC1
#define ADC1_EXTSEL_T1CC2      ADC_CFGR_EXTSEL_T1CC2
#define ADC1_EXTSEL_T1CC3      ADC_CFGR_EXTSEL_T1CC3
#define ADC1_EXTSEL_T1CC4      ADC_CFGR_EXTSEL_T1CC4
#define ADC1_EXTSEL_T1TRGO     ADC_CFGR_EXTSEL_T1TRGO
#define ADC1_EXTSEL_T1TRGO2    ADC_CFGR_EXTSEL_T1TRGO2
#define ADC2_EXTSEL_T1CC1      ADC_CFGR_EXTSEL_T1CC1
#define ADC2_EXTSEL_T1CC2      ADC_CFGR_EXTSEL_T1CC2
#define ADC2_EXTSEL_T1CC3      ADC_CFGR_EXTSEL_T1CC3
#define ADC2_EXTSEL_T1CC4      ADC_CFGR_EXTSEL_T1CC4
#define ADC2_EXTSEL_T1TRGO     ADC_CFGR_EXTSEL_T1TRGO
#define ADC2_EXTSEL_T1TRGO2    ADC_CFGR_EXTSEL_T1TRGO2
#define ADC3_EXTSEL_T1CC1      ADC_CFGR_EXTSEL_T1CC1
#define ADC3_EXTSEL_T1CC2      ADC_CFGR_EXTSEL_T1CC2
#define ADC3_EXTSEL_T1CC3      ADC_CFGR_EXTSEL_T1CC3
#define ADC3_EXTSEL_T1CC4      ADC_CFGR_EXTSEL_T1CC4
#define ADC3_EXTSEL_T1TRGO     ADC_CFGR_EXTSEL_T1TRGO
#define ADC3_EXTSEL_T1TRGO2    ADC_CFGR_EXTSEL_T1TRGO2

#define ADC1_EXTSEL_T2CC1      ADC_CFGR_EXTSEL_T2CC1
#define ADC1_EXTSEL_T2CC2      ADC_CFGR_EXTSEL_T2CC2
#define ADC1_EXTSEL_T2CC3      ADC_CFGR_EXTSEL_T2CC3
#define ADC1_EXTSEL_T2CC4      ADC_CFGR_EXTSEL_T2CC4
#define ADC1_EXTSEL_T2TRGO     ADC_CFGR_EXTSEL_T2TRGO
#define ADC2_EXTSEL_T2CC1      ADC_CFGR_EXTSEL_T2CC1
#define ADC2_EXTSEL_T2CC2      ADC_CFGR_EXTSEL_T2CC2
#define ADC2_EXTSEL_T2CC3      ADC_CFGR_EXTSEL_T2CC3
#define ADC2_EXTSEL_T2CC4      ADC_CFGR_EXTSEL_T2CC4
#define ADC2_EXTSEL_T2TRGO     ADC_CFGR_EXTSEL_T2TRGO
#define ADC3_EXTSEL_T2CC1      ADC_CFGR_EXTSEL_T2CC1
#define ADC3_EXTSEL_T2CC2      ADC_CFGR_EXTSEL_T2CC2
#define ADC3_EXTSEL_T2CC3      ADC_CFGR_EXTSEL_T2CC3
#define ADC3_EXTSEL_T2CC4      ADC_CFGR_EXTSEL_T2CC4
#define ADC3_EXTSEL_T2TRGO     ADC_CFGR_EXTSEL_T2TRGO

#define ADC1_EXTSEL_T3CC1      ADC_CFGR_EXTSEL_T3CC1
#define ADC1_EXTSEL_T3CC2      ADC_CFGR_EXTSEL_T3CC2
#define ADC1_EXTSEL_T3CC3      ADC_CFGR_EXTSEL_T3CC3
#define ADC1_EXTSEL_T3CC4      ADC_CFGR_EXTSEL_T3CC4
#define ADC1_EXTSEL_T3TRGO     ADC_CFGR_EXTSEL_T3TRGO
#define ADC2_EXTSEL_T3CC1      ADC_CFGR_EXTSEL_T3CC1
#define ADC2_EXTSEL_T3CC2      ADC_CFGR_EXTSEL_T3CC2
#define ADC2_EXTSEL_T3CC3      ADC_CFGR_EXTSEL_T3CC3
#define ADC2_EXTSEL_T3CC4      ADC_CFGR_EXTSEL_T3CC4
#define ADC2_EXTSEL_T3TRGO     ADC_CFGR_EXTSEL_T3TRGO
#define ADC3_EXTSEL_T3CC1      ADC_CFGR_EXTSEL_T3CC1
#define ADC3_EXTSEL_T3CC2      ADC_CFGR_EXTSEL_T3CC2
#define ADC3_EXTSEL_T3CC3      ADC_CFGR_EXTSEL_T3CC3
#define ADC3_EXTSEL_T3CC4      ADC_CFGR_EXTSEL_T3CC4
#define ADC3_EXTSEL_T3TRGO     ADC_CFGR_EXTSEL_T3TRGO

#define ADC1_EXTSEL_T4CC1      ADC_CFGR_EXTSEL_T4CC1
#define ADC1_EXTSEL_T4CC2      ADC_CFGR_EXTSEL_T4CC2
#define ADC1_EXTSEL_T4CC3      ADC_CFGR_EXTSEL_T4CC3
#define ADC1_EXTSEL_T4CC4      ADC_CFGR_EXTSEL_T4CC4
#define ADC1_EXTSEL_T4TRGO     ADC_CFGR_EXTSEL_T4TRGO
#define ADC2_EXTSEL_T4CC1      ADC_CFGR_EXTSEL_T4CC1
#define ADC2_EXTSEL_T4CC2      ADC_CFGR_EXTSEL_T4CC2
#define ADC2_EXTSEL_T4CC3      ADC_CFGR_EXTSEL_T4CC3
#define ADC2_EXTSEL_T4CC4      ADC_CFGR_EXTSEL_T4CC4
#define ADC2_EXTSEL_T4TRGO     ADC_CFGR_EXTSEL_T4TRGO
#define ADC3_EXTSEL_T4CC1      ADC_CFGR_EXTSEL_T4CC1
#define ADC3_EXTSEL_T4CC2      ADC_CFGR_EXTSEL_T4CC2
#define ADC3_EXTSEL_T4CC3      ADC_CFGR_EXTSEL_T4CC3
#define ADC3_EXTSEL_T4CC4      ADC_CFGR_EXTSEL_T4CC4
#define ADC3_EXTSEL_T4TRGO     ADC_CFGR_EXTSEL_T4TRGO

#define ADC1_EXTSEL_T6CC1      ADC_CFGR_EXTSEL_T6CC1
#define ADC1_EXTSEL_T6CC2      ADC_CFGR_EXTSEL_T6CC2
#define ADC1_EXTSEL_T6CC3      ADC_CFGR_EXTSEL_T6CC3
#define ADC1_EXTSEL_T6CC4      ADC_CFGR_EXTSEL_T6CC4
#define ADC1_EXTSEL_T6TRGO     ADC_CFGR_EXTSEL_T6TRGO
#define ADC2_EXTSEL_T6CC1      ADC_CFGR_EXTSEL_T6CC1
#define ADC2_EXTSEL_T6CC2      ADC_CFGR_EXTSEL_T6CC2
#define ADC2_EXTSEL_T6CC3      ADC_CFGR_EXTSEL_T6CC3
#define ADC2_EXTSEL_T6CC4      ADC_CFGR_EXTSEL_T6CC4
#define ADC2_EXTSEL_T6TRGO     ADC_CFGR_EXTSEL_T6TRGO
#define ADC3_EXTSEL_T6CC1      ADC_CFGR_EXTSEL_T6CC1
#define ADC3_EXTSEL_T6CC2      ADC_CFGR_EXTSEL_T6CC2
#define ADC3_EXTSEL_T6CC3      ADC_CFGR_EXTSEL_T6CC3
#define ADC3_EXTSEL_T6CC4      ADC_CFGR_EXTSEL_T6CC4
#define ADC3_EXTSEL_T6TRGO     ADC_CFGR_EXTSEL_T6TRGO

#define ADC1_EXTSEL_T8CC1      ADC_CFGR_EXTSEL_T8CC1
#define ADC1_EXTSEL_T8CC2      ADC_CFGR_EXTSEL_T8CC2
#define ADC1_EXTSEL_T8CC3      ADC_CFGR_EXTSEL_T8CC3
#define ADC1_EXTSEL_T8CC4      ADC_CFGR_EXTSEL_T8CC4
#define ADC1_EXTSEL_T8TRGO     ADC_CFGR_EXTSEL_T8TRGO
#define ADC1_EXTSEL_T8TRGO2    ADC_CFGR_EXTSEL_T8TRGO2
#define ADC2_EXTSEL_T8CC1      ADC_CFGR_EXTSEL_T8CC1
#define ADC2_EXTSEL_T8CC2      ADC_CFGR_EXTSEL_T8CC2
#define ADC2_EXTSEL_T8CC3      ADC_CFGR_EXTSEL_T8CC3
#define ADC2_EXTSEL_T8CC4      ADC_CFGR_EXTSEL_T8CC4
#define ADC2_EXTSEL_T8TRGO     ADC_CFGR_EXTSEL_T8TRGO
#define ADC2_EXTSEL_T8TRGO2    ADC_CFGR_EXTSEL_T8TRGO2
#define ADC3_EXTSEL_T8CC1      ADC_CFGR_EXTSEL_T8CC1
#define ADC3_EXTSEL_T8CC2      ADC_CFGR_EXTSEL_T8CC2
#define ADC3_EXTSEL_T8CC3      ADC_CFGR_EXTSEL_T8CC3
#define ADC3_EXTSEL_T8CC4      ADC_CFGR_EXTSEL_T8CC4
#define ADC3_EXTSEL_T8TRGO     ADC_CFGR_EXTSEL_T8TRGO
#define ADC3_EXTSEL_T8TRGO2    ADC_CFGR_EXTSEL_T8TRGO2

#define ADC1_EXTSEL_T15CC1     ADC_CFGR_EXTSEL_T15CC1
#define ADC1_EXTSEL_T15CC2     ADC_CFGR_EXTSEL_T15CC2
#define ADC1_EXTSEL_T15CC3     ADC_CFGR_EXTSEL_T15CC3
#define ADC1_EXTSEL_T15CC4     ADC_CFGR_EXTSEL_T15CC4
#define ADC1_EXTSEL_T15TRGO    ADC_CFGR_EXTSEL_T15TRGO
#define ADC2_EXTSEL_T15CC1     ADC_CFGR_EXTSEL_T15CC1
#define ADC2_EXTSEL_T15CC2     ADC_CFGR_EXTSEL_T15CC2
#define ADC2_EXTSEL_T15CC3     ADC_CFGR_EXTSEL_T15CC3
#define ADC2_EXTSEL_T15CC4     ADC_CFGR_EXTSEL_T15CC4
#define ADC2_EXTSEL_T15TRGO    ADC_CFGR_EXTSEL_T15TRGO
#define ADC3_EXTSEL_T15CC1     ADC_CFGR_EXTSEL_T15CC1
#define ADC3_EXTSEL_T15CC2     ADC_CFGR_EXTSEL_T15CC2
#define ADC3_EXTSEL_T15CC3     ADC_CFGR_EXTSEL_T15CC3
#define ADC3_EXTSEL_T15CC4     ADC_CFGR_EXTSEL_T15CC4
#define ADC3_EXTSEL_T15TRGO    ADC_CFGR_EXTSEL_T15TRGO

/* EXTSEL configuration ******************************************************/

/* Configure external event for regular group */

#if defined(CONFIG_STM32L4_TIM1_ADC1)
#  if CONFIG_STM32L4_ADC1_TIMTRIG == 0
#    define ADC1_EXTSEL_VALUE ADC1_EXTSEL_T1CC1
#  elif CONFIG_STM32L4_ADC1_TIMTRIG == 1
#    define ADC1_EXTSEL_VALUE ADC1_EXTSEL_T1CC2
#  elif CONFIG_STM32L4_ADC1_TIMTRIG == 2
#    define ADC1_EXTSEL_VALUE ADC1_EXTSEL_T1CC3
#  elif CONFIG_STM32L4_ADC1_TIMTRIG == 3
#    define ADC1_EXTSEL_VALUE ADC1_EXTSEL_T1CC4
#  elif CONFIG_STM32L4_ADC1_TIMTRIG == 4
#    define ADC1_EXTSEL_VALUE ADC1_EXTSEL_T1TRGO
#  elif CONFIG_STM32L4_ADC1_TIMTRIG == 5
#    define ADC1_EXTSEL_VALUE ADC1_EXTSEL_T1TRGO2
#  else
#    error "CONFIG_STM32L4_ADC1_TIMTRIG is out of range"
#  endif
#elif defined(CONFIG_STM32L4_TIM2_ADC1)
#  if CONFIG_STM32L4_ADC1_TIMTRIG == 0
#    define ADC1_EXTSEL_VALUE ADC1_EXTSEL_T2CC1
#  elif CONFIG_STM32L4_ADC1_TIMTRIG == 1
#    define ADC1_EXTSEL_VALUE ADC1_EXTSEL_T2CC2
#  elif CONFIG_STM32L4_ADC1_TIMTRIG == 2
#    define ADC1_EXTSEL_VALUE ADC1_EXTSEL_T2CC3
#  elif CONFIG_STM32L4_ADC1_TIMTRIG == 3
#    define ADC1_EXTSEL_VALUE ADC1_EXTSEL_T2CC4
#  elif CONFIG_STM32L4_ADC1_TIMTRIG == 4
#    define ADC1_EXTSEL_VALUE ADC1_EXTSEL_T2TRGO
#  else
#    error "CONFIG_STM32L4_ADC1_TIMTRIG is out of range"
#  endif
#elif defined(CONFIG_STM32L4_TIM3_ADC1)
#  if CONFIG_STM32L4_ADC1_TIMTRIG == 0
#    define ADC1_EXTSEL_VALUE ADC1_EXTSEL_T3CC1
#  elif CONFIG_STM32L4_ADC1_TIMTRIG == 1
#    define ADC1_EXTSEL_VALUE ADC1_EXTSEL_T3CC2
#  elif CONFIG_STM32L4_ADC1_TIMTRIG == 2
#    define ADC1_EXTSEL_VALUE ADC1_EXTSEL_T3CC3
#  elif CONFIG_STM32L4_ADC1_TIMTRIG == 3
#    define ADC1_EXTSEL_VALUE ADC1_EXTSEL_T3CC4
#  elif CONFIG_STM32L4_ADC1_TIMTRIG == 4
#    define ADC1_EXTSEL_VALUE ADC1_EXTSEL_T3TRGO
#  else
#    error "CONFIG_STM32L4_ADC1_TIMTRIG is out of range"
#  endif
#elif defined(CONFIG_STM32L4_TIM4_ADC1)
#  if CONFIG_STM32L4_ADC1_TIMTRIG == 0
#    define ADC1_EXTSEL_VALUE ADC1_EXTSEL_T4CC1
#  elif CONFIG_STM32L4_ADC1_TIMTRIG == 1
#    define ADC1_EXTSEL_VALUE ADC1_EXTSEL_T4CC2
#  elif CONFIG_STM32L4_ADC1_TIMTRIG == 2
#    define ADC1_EXTSEL_VALUE ADC1_EXTSEL_T4CC3
#  elif CONFIG_STM32L4_ADC1_TIMTRIG == 3
#    define ADC1_EXTSEL_VALUE ADC1_EXTSEL_T4CC4
#  elif CONFIG_STM32L4_ADC1_TIMTRIG == 4
#    define ADC1_EXTSEL_VALUE ADC1_EXTSEL_T4TRGO
#  else
#    error "CONFIG_STM32L4_ADC1_TIMTRIG is out of range"
#  endif
#elif defined(CONFIG_STM32L4_TIM6_ADC1)
#  if CONFIG_STM32L4_ADC1_TIMTRIG == 0
#    define ADC1_EXTSEL_VALUE ADC1_EXTSEL_T6CC1
#  elif CONFIG_STM32L4_ADC1_TIMTRIG == 1
#    define ADC1_EXTSEL_VALUE ADC1_EXTSEL_T6CC2
#  elif CONFIG_STM32L4_ADC1_TIMTRIG == 2
#    define ADC1_EXTSEL_VALUE ADC1_EXTSEL_T6CC3
#  elif CONFIG_STM32L4_ADC1_TIMTRIG == 3
#    define ADC1_EXTSEL_VALUE ADC1_EXTSEL_T6CC4
#  elif CONFIG_STM32L4_ADC1_TIMTRIG == 4
#    define ADC1_EXTSEL_VALUE ADC1_EXTSEL_T6TRGO
#  else
#    error "CONFIG_STM32L4_ADC1_TIMTRIG is out of range"
#  endif
#elif defined(CONFIG_STM32L4_TIM8_ADC1)
#  if CONFIG_STM32L4_ADC1_TIMTRIG == 0
#    define ADC1_EXTSEL_VALUE ADC1_EXTSEL_T8CC1
#  elif CONFIG_STM32L4_ADC1_TIMTRIG == 1
#    define ADC1_EXTSEL_VALUE ADC1_EXTSEL_T8CC2
#  elif CONFIG_STM32L4_ADC1_TIMTRIG == 2
#    define ADC1_EXTSEL_VALUE ADC1_EXTSEL_T8CC3
#  elif CONFIG_STM32L4_ADC1_TIMTRIG == 3
#    define ADC1_EXTSEL_VALUE ADC1_EXTSEL_T8CC4
#  elif CONFIG_STM32L4_ADC1_TIMTRIG == 4
#    define ADC1_EXTSEL_VALUE ADC1_EXTSEL_T8TRGO
#  elif CONFIG_STM32L4_ADC1_TIMTRIG == 5
#    define ADC1_EXTSEL_VALUE ADC1_EXTSEL_T8TRGO2
#  else
#    error "CONFIG_STM32L4_ADC1_TIMTRIG is out of range"
#  endif
#elif defined(CONFIG_STM32L4_TIM15_ADC1)
#  if CONFIG_STM32L4_ADC1_TIMTRIG == 0
#    define ADC1_EXTSEL_VALUE ADC1_EXTSEL_T15CC1
#  elif CONFIG_STM32L4_ADC1_TIMTRIG == 1
#    define ADC1_EXTSEL_VALUE ADC1_EXTSEL_T15CC2
#  elif CONFIG_STM32L4_ADC1_TIMTRIG == 2
#    define ADC1_EXTSEL_VALUE ADC1_EXTSEL_T15CC3
#  elif CONFIG_STM32L4_ADC1_TIMTRIG == 3
#    define ADC1_EXTSEL_VALUE ADC1_EXTSEL_T15CC4
#  elif CONFIG_STM32L4_ADC1_TIMTRIG == 4
#    define ADC1_EXTSEL_VALUE ADC1_EXTSEL_T15TRGO
#  else
#    error "CONFIG_STM32L4_ADC1_TIMTRIG is out of range"
#  endif
#endif

#if defined(CONFIG_STM32L4_TIM1_ADC2)
#  if CONFIG_STM32L4_ADC2_TIMTRIG == 0
#    define ADC2_EXTSEL_VALUE ADC2_EXTSEL_T1CC1
#  elif CONFIG_STM32L4_ADC2_TIMTRIG == 1
#    define ADC2_EXTSEL_VALUE ADC2_EXTSEL_T1CC2
#  elif CONFIG_STM32L4_ADC2_TIMTRIG == 2
#    define ADC2_EXTSEL_VALUE ADC2_EXTSEL_T1CC3
#  elif CONFIG_STM32L4_ADC2_TIMTRIG == 3
#    define ADC2_EXTSEL_VALUE ADC2_EXTSEL_T1CC4
#  elif CONFIG_STM32L4_ADC2_TIMTRIG == 4
#    define ADC2_EXTSEL_VALUE ADC2_EXTSEL_T1TRGO
#  elif CONFIG_STM32L4_ADC2_TIMTRIG == 5
#    define ADC2_EXTSEL_VALUE ADC2_EXTSEL_T1TRGO2
#  else
#    error "CONFIG_STM32L4_ADC2_TIMTRIG is out of range"
#  endif
#elif defined(CONFIG_STM32L4_TIM2_ADC2)
#  if CONFIG_STM32L4_ADC2_TIMTRIG == 0
#    define ADC2_EXTSEL_VALUE ADC2_EXTSEL_T2CC1
#  elif CONFIG_STM32L4_ADC2_TIMTRIG == 1
#    define ADC2_EXTSEL_VALUE ADC2_EXTSEL_T2CC2
#  elif CONFIG_STM32L4_ADC2_TIMTRIG == 2
#    define ADC2_EXTSEL_VALUE ADC2_EXTSEL_T2CC3
#  elif CONFIG_STM32L4_ADC2_TIMTRIG == 3
#    define ADC2_EXTSEL_VALUE ADC2_EXTSEL_T2CC4
#  elif CONFIG_STM32L4_ADC2_TIMTRIG == 4
#    define ADC2_EXTSEL_VALUE ADC2_EXTSEL_T2TRGO
#  else
#    error "CONFIG_STM32L4_ADC2_TIMTRIG is out of range"
#  endif
#elif defined(CONFIG_STM32L4_TIM3_ADC2)
#  if CONFIG_STM32L4_ADC2_TIMTRIG == 0
#    define ADC2_EXTSEL_VALUE ADC2_EXTSEL_T3CC1
#  elif CONFIG_STM32L4_ADC2_TIMTRIG == 1
#    define ADC2_EXTSEL_VALUE ADC2_EXTSEL_T3CC2
#  elif CONFIG_STM32L4_ADC2_TIMTRIG == 2
#    define ADC2_EXTSEL_VALUE ADC2_EXTSEL_T3CC3
#  elif CONFIG_STM32L4_ADC2_TIMTRIG == 3
#    define ADC2_EXTSEL_VALUE ADC2_EXTSEL_T3CC4
#  elif CONFIG_STM32L4_ADC2_TIMTRIG == 4
#    define ADC2_EXTSEL_VALUE ADC2_EXTSEL_T3TRGO
#  else
#    error "CONFIG_STM32L4_ADC2_TIMTRIG is out of range"
#  endif
#elif defined(CONFIG_STM32L4_TIM4_ADC2)
#  if CONFIG_STM32L4_ADC2_TIMTRIG == 0
#    define ADC2_EXTSEL_VALUE ADC2_EXTSEL_T4CC1
#  elif CONFIG_STM32L4_ADC2_TIMTRIG == 1
#    define ADC2_EXTSEL_VALUE ADC2_EXTSEL_T4CC2
#  elif CONFIG_STM32L4_ADC2_TIMTRIG == 2
#    define ADC2_EXTSEL_VALUE ADC2_EXTSEL_T4CC3
#  elif CONFIG_STM32L4_ADC2_TIMTRIG == 3
#    define ADC2_EXTSEL_VALUE ADC2_EXTSEL_T4CC4
#  elif CONFIG_STM32L4_ADC2_TIMTRIG == 4
#    define ADC2_EXTSEL_VALUE ADC2_EXTSEL_T4TRGO
#  else
#    error "CONFIG_STM32L4_ADC2_TIMTRIG is out of range"
#  endif
#elif defined(CONFIG_STM32L4_TIM6_ADC2)
#  if CONFIG_STM32L4_ADC2_TIMTRIG == 0
#    define ADC2_EXTSEL_VALUE ADC2_EXTSEL_T6CC1
#  elif CONFIG_STM32L4_ADC2_TIMTRIG == 1
#    define ADC2_EXTSEL_VALUE ADC2_EXTSEL_T6CC2
#  elif CONFIG_STM32L4_ADC2_TIMTRIG == 2
#    define ADC2_EXTSEL_VALUE ADC2_EXTSEL_T6CC3
#  elif CONFIG_STM32L4_ADC2_TIMTRIG == 3
#    define ADC2_EXTSEL_VALUE ADC2_EXTSEL_T6CC4
#  elif CONFIG_STM32L4_ADC2_TIMTRIG == 4
#    define ADC2_EXTSEL_VALUE ADC2_EXTSEL_T6TRGO
#  else
#    error "CONFIG_STM32L4_ADC2_TIMTRIG is out of range"
#  endif
#elif defined(CONFIG_STM32L4_TIM8_ADC2)
#  if CONFIG_STM32L4_ADC2_TIMTRIG == 0
#    define ADC2_EXTSEL_VALUE ADC2_EXTSEL_T8CC1
#  elif CONFIG_STM32L4_ADC2_TIMTRIG == 1
#    define ADC2_EXTSEL_VALUE ADC2_EXTSEL_T8CC2
#  elif CONFIG_STM32L4_ADC2_TIMTRIG == 2
#    define ADC2_EXTSEL_VALUE ADC2_EXTSEL_T8CC3
#  elif CONFIG_STM32L4_ADC2_TIMTRIG == 3
#    define ADC2_EXTSEL_VALUE ADC2_EXTSEL_T8CC4
#  elif CONFIG_STM32L4_ADC2_TIMTRIG == 4
#    define ADC2_EXTSEL_VALUE ADC2_EXTSEL_T8TRGO
#  elif CONFIG_STM32L4_ADC2_TIMTRIG == 5
#    define ADC2_EXTSEL_VALUE ADC2_EXTSEL_T8TRGO2
#  else
#    error "CONFIG_STM32L4_ADC2_TIMTRIG is out of range"
#  endif
#elif defined(CONFIG_STM32L4_TIM15_ADC2)
#  if CONFIG_STM32L4_ADC2_TIMTRIG == 0
#    define ADC2_EXTSEL_VALUE ADC2_EXTSEL_T15CC1
#  elif CONFIG_STM32L4_ADC2_TIMTRIG == 1
#    define ADC2_EXTSEL_VALUE ADC2_EXTSEL_T15CC2
#  elif CONFIG_STM32L4_ADC2_TIMTRIG == 2
#    define ADC2_EXTSEL_VALUE ADC2_EXTSEL_T15CC3
#  elif CONFIG_STM32L4_ADC2_TIMTRIG == 3
#    define ADC2_EXTSEL_VALUE ADC2_EXTSEL_T15CC4
#  elif CONFIG_STM32L4_ADC2_TIMTRIG == 4
#    define ADC2_EXTSEL_VALUE ADC2_EXTSEL_T15TRGO
#  else
#    error "CONFIG_STM32L4_ADC2_TIMTRIG is out of range"
#  endif
#endif

#if defined(CONFIG_STM32L4_TIM1_ADC3)
#  if CONFIG_STM32L4_ADC3_TIMTRIG == 0
#    define ADC3_EXTSEL_VALUE ADC3_EXTSEL_T1CC1
#  elif CONFIG_STM32L4_ADC3_TIMTRIG == 1
#    define ADC3_EXTSEL_VALUE ADC3_EXTSEL_T1CC2
#  elif CONFIG_STM32L4_ADC3_TIMTRIG == 2
#    define ADC3_EXTSEL_VALUE ADC3_EXTSEL_T1CC3
#  elif CONFIG_STM32L4_ADC3_TIMTRIG == 3
#    define ADC3_EXTSEL_VALUE ADC3_EXTSEL_T1CC4
#  elif CONFIG_STM32L4_ADC3_TIMTRIG == 4
#    define ADC3_EXTSEL_VALUE ADC3_EXTSEL_T1TRGO
#  elif CONFIG_STM32L4_ADC3_TIMTRIG == 5
#    define ADC3_EXTSEL_VALUE ADC3_EXTSEL_T1TRGO2
#  else
#    error "CONFIG_STM32L4_ADC3_TIMTRIG is out of range"
#  endif
#elif defined(CONFIG_STM32L4_TIM2_ADC3)
#  if CONFIG_STM32L4_ADC3_TIMTRIG == 0
#    define ADC3_EXTSEL_VALUE ADC3_EXTSEL_T2CC1
#  elif CONFIG_STM32L4_ADC3_TIMTRIG == 1
#    define ADC3_EXTSEL_VALUE ADC3_EXTSEL_T2CC2
#  elif CONFIG_STM32L4_ADC3_TIMTRIG == 2
#    define ADC3_EXTSEL_VALUE ADC3_EXTSEL_T2CC3
#  elif CONFIG_STM32L4_ADC3_TIMTRIG == 3
#    define ADC3_EXTSEL_VALUE ADC3_EXTSEL_T2CC4
#  elif CONFIG_STM32L4_ADC3_TIMTRIG == 4
#    define ADC3_EXTSEL_VALUE ADC3_EXTSEL_T2TRGO
#  else
#    error "CONFIG_STM32L4_ADC3_TIMTRIG is out of range"
#  endif
#elif defined(CONFIG_STM32L4_TIM3_ADC3)
#  if CONFIG_STM32L4_ADC3_TIMTRIG == 0
#    define ADC3_EXTSEL_VALUE ADC3_EXTSEL_T3CC1
#  elif CONFIG_STM32L4_ADC3_TIMTRIG == 1
#    define ADC3_EXTSEL_VALUE ADC3_EXTSEL_T3CC2
#  elif CONFIG_STM32L4_ADC3_TIMTRIG == 2
#    define ADC3_EXTSEL_VALUE ADC3_EXTSEL_T3CC3
#  elif CONFIG_STM32L4_ADC3_TIMTRIG == 3
#    define ADC3_EXTSEL_VALUE ADC3_EXTSEL_T3CC4
#  elif CONFIG_STM32L4_ADC3_TIMTRIG == 4
#    define ADC3_EXTSEL_VALUE ADC3_EXTSEL_T3TRGO
#  else
#    error "CONFIG_STM32L4_ADC3_TIMTRIG is out of range"
#  endif
#elif defined(CONFIG_STM32L4_TIM4_ADC3)
#  if CONFIG_STM32L4_ADC3_TIMTRIG == 0
#    define ADC3_EXTSEL_VALUE ADC3_EXTSEL_T4CC1
#  elif CONFIG_STM32L4_ADC3_TIMTRIG == 1
#    define ADC3_EXTSEL_VALUE ADC3_EXTSEL_T4CC2
#  elif CONFIG_STM32L4_ADC3_TIMTRIG == 2
#    define ADC3_EXTSEL_VALUE ADC3_EXTSEL_T4CC3
#  elif CONFIG_STM32L4_ADC3_TIMTRIG == 3
#    define ADC3_EXTSEL_VALUE ADC3_EXTSEL_T4CC4
#  elif CONFIG_STM32L4_ADC3_TIMTRIG == 4
#    define ADC3_EXTSEL_VALUE ADC3_EXTSEL_T4TRGO
#  else
#    error "CONFIG_STM32L4_ADC3_TIMTRIG is out of range"
#  endif
#elif defined(CONFIG_STM32L4_TIM6_ADC3)
#  if CONFIG_STM32L4_ADC3_TIMTRIG == 0
#    define ADC3_EXTSEL_VALUE ADC3_EXTSEL_T6CC1
#  elif CONFIG_STM32L4_ADC3_TIMTRIG == 1
#    define ADC3_EXTSEL_VALUE ADC3_EXTSEL_T6CC2
#  elif CONFIG_STM32L4_ADC3_TIMTRIG == 2
#    define ADC3_EXTSEL_VALUE ADC3_EXTSEL_T6CC3
#  elif CONFIG_STM32L4_ADC3_TIMTRIG == 3
#    define ADC3_EXTSEL_VALUE ADC3_EXTSEL_T6CC4
#  elif CONFIG_STM32L4_ADC3_TIMTRIG == 4
#    define ADC3_EXTSEL_VALUE ADC3_EXTSEL_T6TRGO
#  else
#    error "CONFIG_STM32L4_ADC3_TIMTRIG is out of range"
#  endif
#elif defined(CONFIG_STM32L4_TIM8_ADC3)
#  if CONFIG_STM32L4_ADC3_TIMTRIG == 0
#    define ADC3_EXTSEL_VALUE ADC3_EXTSEL_T8CC1
#  elif CONFIG_STM32L4_ADC3_TIMTRIG == 1
#    define ADC3_EXTSEL_VALUE ADC3_EXTSEL_T8CC2
#  elif CONFIG_STM32L4_ADC3_TIMTRIG == 2
#    define ADC3_EXTSEL_VALUE ADC3_EXTSEL_T8CC3
#  elif CONFIG_STM32L4_ADC3_TIMTRIG == 3
#    define ADC3_EXTSEL_VALUE ADC3_EXTSEL_T8CC4
#  elif CONFIG_STM32L4_ADC3_TIMTRIG == 4
#    define ADC3_EXTSEL_VALUE ADC3_EXTSEL_T8TRGO
#  elif CONFIG_STM32L4_ADC3_TIMTRIG == 5
#    define ADC3_EXTSEL_VALUE ADC3_EXTSEL_T8TRGO2
#  else
#    error "CONFIG_STM32L4_ADC3_TIMTRIG is out of range"
#  endif
#elif defined(CONFIG_STM32L4_TIM15_ADC3)
#  if CONFIG_STM32L4_ADC3_TIMTRIG == 0
#    define ADC3_EXTSEL_VALUE ADC3_EXTSEL_T15CC1
#  elif CONFIG_STM32L4_ADC3_TIMTRIG == 1
#    define ADC3_EXTSEL_VALUE ADC3_EXTSEL_T15CC2
#  elif CONFIG_STM32L4_ADC3_TIMTRIG == 2
#    define ADC3_EXTSEL_VALUE ADC3_EXTSEL_T15CC3
#  elif CONFIG_STM32L4_ADC3_TIMTRIG == 3
#    define ADC3_EXTSEL_VALUE ADC3_EXTSEL_T15CC4
#  elif CONFIG_STM32L4_ADC3_TIMTRIG == 4
#    define ADC3_EXTSEL_VALUE ADC3_EXTSEL_T15TRGO
#  else
#    error "CONFIG_STM32L4_ADC3_TIMTRIG is out of range"
#  endif
#endif

/* ADC interrupts ************************************************************/

#define ADC_ISR_EOC                  ADC_INT_EOC
#define ADC_IER_EOC                  ADC_INT_EOC
#define ADC_ISR_AWD                  ADC_INT_AWD1
#define ADC_IER_AWD                  ADC_INT_AWD1
#define ADC_ISR_JEOC                 ADC_INT_JEOC
#define ADC_IER_JEOC                 ADC_INT_JEOC
#define ADC_ISR_OVR                  ADC_INT_OVR
#define ADC_IER_OVR                  ADC_INT_OVR
#define ADC_ISR_JEOS                 ADC_INT_JEOS
#define ADC_IER_JEOS                 ADC_INT_JEOS

#define ADC_ISR_ALLINTS (ADC_ISR_EOC | ADC_ISR_AWD | ADC_ISR_JEOC | \
                         ADC_ISR_JEOS | ADC_ISR_OVR)
#define ADC_IER_ALLINTS (ADC_IER_EOC | ADC_IER_AWD | ADC_IER_JEOC | \
                         ADC_IER_JEOS | ADC_IER_OVR)

/* Low-level ops helpers *****************************************************/

#define ADC_INT_ACK(adc, source)                     \
        (adc)->llops->int_ack(adc, source)
#define ADC_INT_GET(adc)                             \
        (adc)->llops->int_get(adc)
#define ADC_INT_ENABLE(adc, source)                  \
        (adc)->llops->int_en(adc, source)
#define ADC_INT_DISABLE(adc, source)                 \
        (adc)->llops->int_dis(adc, source)
#define ADC_REGDATA_GET(adc)                         \
        (adc)->llops->val_get(adc)
#define ADC_REGBUF_REGISTER(adc, buffer, len)        \
        (adc)->llops->regbuf_reg(adc, buffer, len)
#define ADC_REG_STARTCONV(adc, state)                \
        (adc)->llops->reg_startconv(adc, state)
#define ADC_OFFSET_SET(adc, ch, i, o)                \
        (adc)->llops->offset_set(adc, ch, i, o)
#define ADC_DUMP_REGS(adc)                           \
        (adc)->llops->dump_regs(adc)

/*****************************************************************************
 * Public Types
 *****************************************************************************/

#ifdef CONFIG_STM32L4_ADC_LL_OPS

/* This structure provides the publicly visable representation of the
 * "lower-half" ADC driver structure.
 */

struct stm32_adc_dev_s
{
  /* Publicly visible portion of the "lower-half" ADC driver structure */

  FAR const struct stm32_adc_ops_s *llops;

  /* Require cast-compatibility with private "lower-half" ADC strucutre */
};

/* Low-level operations for ADC */

struct stm32_adc_ops_s
{
  /* Acknowledge interrupts */

  void (*int_ack)(FAR struct stm32_adc_dev_s *dev, uint32_t source);

  /* Get pending interrupts */

  uint32_t (*int_get)(FAR struct stm32_adc_dev_s *dev);

  /* Enable interrupts */

  void (*int_en)(FAR struct stm32_adc_dev_s *dev, uint32_t source);

  /* Disable interrupts */

  void (*int_dis)(FAR struct stm32_adc_dev_s *dev, uint32_t source);

  /* Get current ADC data register */

  uint32_t (*val_get)(FAR struct stm32_adc_dev_s *dev);

  /* Register buffer for ADC DMA transfer */

  int (*regbuf_reg)(FAR struct stm32_adc_dev_s *dev, uint16_t *buffer,
                    uint8_t len);

  /* Start/stop regular conversion */

  void (*reg_startconv)(FAR struct stm32_adc_dev_s *dev, bool state);

  /* Set offset for channel */

  int (*offset_set)(FAR struct stm32_adc_dev_s *dev, uint8_t ch, uint8_t i,
                    uint16_t offset);

  void (*dump_regs)(FAR struct stm32_adc_dev_s *dev);
};

#endif  /* CONFIG_STM32L4_ADC_LL_OPS */

/*****************************************************************************
 * Public Function Prototypes
 *****************************************************************************/

#ifndef __ASSEMBLY__
#ifdef __cplusplus
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/*****************************************************************************
 * Name: stm32l4_adc_initialize
 *
 * Description:
 *   Initialize the ADC.
 *
 * Input Parameters:
 *   intf      - Could be {1,2,3} for ADC1, ADC2, or ADC3
 *   chanlist  - The list of channels
 *   nchannels - Number of channels
 *
 * Returned Value:
 *   Valid ADC device structure reference on success; a NULL on failure
 *
 *****************************************************************************/

struct adc_dev_s;
struct adc_dev_s *stm32l4_adc_initialize(int intf,
                                         FAR const uint8_t *chanlist,
                                         int nchannels);
#undef EXTERN
#ifdef __cplusplus
}
#endif
#endif /* __ASSEMBLY__ */

#endif /* CONFIG_STM32L4_ADC1 || CONFIG_STM32L4_ADC2 || CONFIG_STM32L4_ADC3 */
#endif /* __ARCH_ARM_SRC_STM32L4_STM32L4_ADC_H */