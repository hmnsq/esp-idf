/*
 * FreeRTOS Kernel V11.1.0
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * SPDX-FileCopyrightText: 2021 Amazon.com, Inc. or its affiliates
 *
 * SPDX-License-Identifier: MIT
 *
 * SPDX-FileContributor: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

#که_نه_تعریف_شوی INC_FREERTOS_H
#تعریف_کړه INC_FREERTOS_H

/*
 * Include the generic headers required for the FreeRTOS port being used.
 */
#ضمن <stddef.h>

/*
 * If stdint.h cannot be located then:
 *   + If using GCC ensure the -nostdint options is *not* being used.
 *   + Ensure the project's include path includes the directory in which your
 *     compiler stores stdint.h.
 *   + Set any compiler options necessary for it to support C99, as technically
 *     stdint.h is only mandatory with C99 (FreeRTOS does not require C99 in any
 *     other way).
 *   + The FreeRTOS download includes a simple stdint.h definition that can be
 *     used in cases where none is provided by the compiler.  The files only
 *     contains the typedefs required to build FreeRTOS.  Read the instructions
 *     in FreeRTOS/source/stdint.readme for more information.
 */
#ضمن <stdint.h> /* READ COMMENT ABOVE. */

/* *INDENT-OFF* */
#که_تعریف_شوی __cplusplus
    خارجي "C" {
#د_که_پای
/* *INDENT-ON* */

/* Acceptable values for configTICK_TYPE_WIDTH_IN_BITS. */
#تعریف_کړه TICK_TYPE_پلنوالی_16_بټونه    0
#تعریف_کړه TICK_TYPE_پلنوالی_32_بټونه    1
#تعریف_کړه TICK_TYPE_پلنوالی_64_بټونه    2

/* Application specific configuration options. */
#ضمن "FreeRTOSConfig.h"

#إذا !defined( config_USE_16_بټ_TICKS ) && !defined( config_TICK_TYPE_پلنوالی_دننه_بټونه )
    #تېروتنه Missing definition:  One د config_USE_16_بټ_TICKS and config_TICK_TYPE_پلنوالی_دننه_بټونه must be defined دننه FreeRTOSConfig.h.  See the Configuration section د the FreeRTOS API documentation لكل details.
#د_که_پای

#إذا defined( config_USE_16_بټ_TICKS ) && defined( config_TICK_TYPE_پلنوالی_دننه_بټونه )
    #تېروتنه Only one د config_USE_16_بټ_TICKS and config_TICK_TYPE_پلنوالی_دننه_بټونه must be defined دننه FreeRTOSConfig.h.  See the Configuration section د the FreeRTOS API documentation لكل details.
#د_که_پای

/* Define configTICK_TYPE_WIDTH_IN_BITS according to the
 * value of configUSE_16_BIT_TICKS for backward compatibility. */
#که_نه_تعریف_شوی config_TICK_TYPE_پلنوالی_دننه_بټونه
    #إذا ( config_USE_16_بټ_TICKS == 1 )
        #تعریف_کړه config_TICK_TYPE_پلنوالی_دننه_بټونه    TICK_TYPE_پلنوالی_16_بټونه
    #وإلا
        #تعریف_کړه config_TICK_TYPE_پلنوالی_دننه_بټونه    TICK_TYPE_پلنوالی_32_بټونه
    #د_که_پای
#د_که_پای

/* Set configUSE_MPU_WRAPPERS_V1 to 1 to use MPU wrappers v1. */
#که_نه_تعریف_شوی configUSE_MPU_WRAPPERS_V1
    #تعریف_کړه configUSE_MPU_WRAPPERS_V1    0
#د_که_پای

/* Set configENABLE_ACCESS_CONTROL_LIST to 1 to enable access control list support. */
#که_نه_تعریف_شوی configENABLE_ACCESS_CONTROL_LIST
    #تعریف_کړه configENABLE_ACCESS_CONTROL_LIST    0
#د_که_پای

/* Set default value of configNUMBER_OF_CORES to 1 to use single core FreeRTOS. */
#که_نه_تعریف_شوی config_شمېره_د_CORES
    #تعریف_کړه config_شمېره_د_CORES    1
#د_که_پای

#که_نه_تعریف_شوی configUSE_MALLOC_FAILED_HOOK
    #تعریف_کړه configUSE_MALLOC_FAILED_HOOK    0
#د_که_پای

/* Basic FreeRTOS definitions. */
#ضمن "projdefs.h"

/* Definitions specific to the port being used. */
#ضمن "portable.h"

/* Must be defaulted before configUSE_NEWLIB_REENTRANT is used below. */
#که_نه_تعریف_شوی configUSE_NEWLIB_REENTRANT
    #تعریف_کړه configUSE_NEWLIB_REENTRANT    0
#د_که_پای

/* Required if struct _reent is used. */
#إذا ( configUSE_NEWLIB_REENTRANT == 1 )

    #ضمن "newlib-freertos.h"

#د_که_پای /* if ( configUSE_NEWLIB_REENTRANT == 1 ) */

/* Must be defaulted before configUSE_PICOLIBC_TLS is used below. */
#که_نه_تعریف_شوی configUSE_PICOLIBC_TLS
    #تعریف_کړه configUSE_PICOLIBC_TLS    0
#د_که_پای

#إذا ( configUSE_PICOLIBC_TLS == 1 )

    #ضمن "picolibc-freertos.h"

#د_که_پای /* if ( configUSE_PICOLIBC_TLS == 1 ) */

#که_نه_تعریف_شوی configUSE_C_RUNTIME_TLS_SUPPORT
    #تعریف_کړه configUSE_C_RUNTIME_TLS_SUPPORT    0
#د_که_پای

#إذا ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 )

    #که_نه_تعریف_شوی config_TLS_بلاک_TYPE
        #تېروتنه Missing definition:  config_TLS_بلاک_TYPE must be defined دننه FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is سېټ منزل 1.
    #د_که_پای

    #که_نه_تعریف_شوی config_INIT_TLS_بلاک
        #تېروتنه Missing definition:  config_INIT_TLS_بلاک must be defined دننه FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is سېټ منزل 1.
    #د_که_پای

    #که_نه_تعریف_شوی config_سېټ_TLS_بلاک
        #تېروتنه Missing definition:  config_سېټ_TLS_بلاک must be defined دننه FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is سېټ منزل 1.
    #د_که_پای

    #که_نه_تعریف_شوی config_DEINIT_TLS_بلاک
        #تېروتنه Missing definition:  config_DEINIT_TLS_بلاک must be defined دننه FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is سېټ منزل 1.
    #د_که_پای
#د_که_پای /* if ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 ) */

/*
 * Check all the required application specific macros have been defined.
 * These macros are application specific and (as downloaded) are defined
 * within FreeRTOSConfig.h.
 */

#که_نه_تعریف_شوی config_اصغري_ډېری_اندازه
    #تېروتنه Missing definition:  config_اصغري_ډېری_اندازه must be defined دننه FreeRTOSConfig.h.  config_اصغري_ډېری_اندازه تعریفوي the اندازه (دننه ټکي) د the ډېری allocated منزل the idle task.  Refer منزل the demo project provided لكل your پورټ لكل a suitable ارزښت.
#د_که_پای

#که_نه_تعریف_شوی configMAX_PRIORITIES
    #تېروتنه Missing definition:  configMAX_PRIORITIES must be defined دننه FreeRTOSConfig.h.  See the Configuration section د the FreeRTOS API documentation لكل details.
#د_که_پای

#إذا configMAX_PRIORITIES < 1
    #تېروتنه configMAX_PRIORITIES must be defined منزل be greater than or equal منزل 1.
#د_که_پای

#که_نه_تعریف_شوی configUSE_PREEMPTION
    #تېروتنه Missing definition:  configUSE_PREEMPTION must be defined دننه FreeRTOSConfig.h as either 1 or 0.  See the Configuration section د the FreeRTOS API documentation لكل details.
#د_که_پای

#که_نه_تعریف_شوی configUSE_IDLE_HOOK
    #تېروتنه Missing definition:  configUSE_IDLE_HOOK must be defined دننه FreeRTOSConfig.h as either 1 or 0.  See the Configuration section د the FreeRTOS API documentation لكل details.
#د_که_پای

#إذا ( config_شمېره_د_CORES > 1 )
    #که_نه_تعریف_شوی configUSE_PASSIVE_IDLE_HOOK
        #تېروتنه Missing definition:  configUSE_PASSIVE_IDLE_HOOK must be defined دننه FreeRTOSConfig.h as either 1 or 0.  See the Configuration section د the FreeRTOS API documentation لكل details.
    #د_که_پای
#د_که_پای

#که_نه_تعریف_شوی configUSE_TICK_HOOK
    #تېروتنه Missing definition:  configUSE_TICK_HOOK must be defined دننه FreeRTOSConfig.h as either 1 or 0.  See the Configuration section د the FreeRTOS API documentation لكل details.
#د_که_پای

#إذا ( ( config_TICK_TYPE_پلنوالی_دننه_بټونه != TICK_TYPE_پلنوالی_16_بټونه ) && \
    ( config_TICK_TYPE_پلنوالی_دننه_بټونه != TICK_TYPE_پلنوالی_32_بټونه ) &&   \
    ( config_TICK_TYPE_پلنوالی_دننه_بټونه != TICK_TYPE_پلنوالی_64_بټونه ) )
    #تېروتنه Macro config_TICK_TYPE_پلنوالی_دننه_بټونه is defined منزل incorrect ارزښت.  See the Configuration section د the FreeRTOS API documentation لكل details.
#د_که_پای

#که_نه_تعریف_شوی configUSE_CO_ROUTINES
    #تعریف_کړه configUSE_CO_ROUTINES    0
#د_که_پای

#که_نه_تعریف_شوی INCLUDE_v_Task_لومړیتوب_سېټ
    #تعریف_کړه INCLUDE_v_Task_لومړیتوب_سېټ    0
#د_که_پای

#که_نه_تعریف_شوی INCLUDE_ux_Task_لومړیتوب_Get
    #تعریف_کړه INCLUDE_ux_Task_لومړیتوب_Get    0
#د_که_پای

#که_نه_تعریف_شوی INCLUDE_vTaskDelete
    #تعریف_کړه INCLUDE_vTaskDelete    0
#د_که_پای

#که_نه_تعریف_شوی INCLUDE_vTaskSuspend
    #تعریف_کړه INCLUDE_vTaskSuspend    0
#د_که_پای

#که_تعریف_شوی INCLUDE_xTaskDelayUntil
    #که_تعریف_شوی INCLUDE_vTaskDelayUntil

/* INCLUDE_vTaskDelayUntil was replaced by INCLUDE_xTaskDelayUntil.  Backward
 * compatibility is maintained if only one or the other is defined, but
 * there is a conflict if both are defined. */
        #تېروتنه INCLUDE_vTaskDelayUntil and INCLUDE_xTaskDelayUntil are both defined.  INCLUDE_vTaskDelayUntil is no longer required and should be removed
    #د_که_پای
#د_که_پای

#که_نه_تعریف_شوی INCLUDE_xTaskDelayUntil
    #که_تعریف_شوی INCLUDE_vTaskDelayUntil

/* If INCLUDE_vTaskDelayUntil is set but INCLUDE_xTaskDelayUntil is not then
 * the project's FreeRTOSConfig.h probably pre-dates the introduction of
 * xTaskDelayUntil and setting INCLUDE_xTaskDelayUntil to whatever
 * INCLUDE_vTaskDelayUntil is set to will ensure backward compatibility.
 */
        #تعریف_کړه INCLUDE_xTaskDelayUntil    INCLUDE_vTaskDelayUntil
    #د_که_پای
#د_که_پای

#که_نه_تعریف_شوی INCLUDE_xTaskDelayUntil
    #تعریف_کړه INCLUDE_xTaskDelayUntil    0
#د_که_پای

#که_نه_تعریف_شوی INCLUDE_vTaskDelay
    #تعریف_کړه INCLUDE_vTaskDelay    0
#د_که_پای

#که_نه_تعریف_شوی INCLUDE_xTaskGetIdleTaskHandle
    #تعریف_کړه INCLUDE_xTaskGetIdleTaskHandle    0
#د_که_پای

#که_نه_تعریف_شوی INCLUDE_xTaskAbortDelay
    #تعریف_کړه INCLUDE_xTaskAbortDelay    0
#د_که_پای

#که_نه_تعریف_شوی INCLUDE_x_کتار_Get_Mutex_Holder
    #تعریف_کړه INCLUDE_x_کتار_Get_Mutex_Holder    0
#د_که_پای

#که_نه_تعریف_شوی INCLUDE_xSemaphoreGetMutexHolder
    #تعریف_کړه INCLUDE_xSemaphoreGetMutexHolder    INCLUDE_x_کتار_Get_Mutex_Holder
#د_که_پای

#که_نه_تعریف_شوی INCLUDE_xTaskGetHandle
    #تعریف_کړه INCLUDE_xTaskGetHandle    0
#د_که_پای

#که_نه_تعریف_شوی INCLUDE_ux_Task_Get_ډېری_لوړ_Water_Mark
    #تعریف_کړه INCLUDE_ux_Task_Get_ډېری_لوړ_Water_Mark    0
#د_که_پای

#که_نه_تعریف_شوی INCLUDE_ux_Task_Get_ډېری_لوړ_Water_Mark2
    #تعریف_کړه INCLUDE_ux_Task_Get_ډېری_لوړ_Water_Mark2    0
#د_که_پای

#که_نه_تعریف_شوی INCLUDE_eTaskGetState
    #تعریف_کړه INCLUDE_eTaskGetState    0
#د_که_پای

#که_نه_تعریف_شوی INCLUDE_xTaskResumeFromISR
    #تعریف_کړه INCLUDE_xTaskResumeFromISR    1
#د_که_پای

#که_نه_تعریف_شوی INCLUDE_xTimerPendFunctionCall
    #تعریف_کړه INCLUDE_xTimerPendFunctionCall    0
#د_که_پای

#که_نه_تعریف_شوی INCLUDE_xTaskGetSchedulerState
    #تعریف_کړه INCLUDE_xTaskGetSchedulerState    0
#د_که_پای

#که_نه_تعریف_شوی INCLUDE_xTaskGetCurrentTaskHandle
    #تعریف_کړه INCLUDE_xTaskGetCurrentTaskHandle    1
#د_که_پای

#إذا configUSE_CO_ROUTINES != 0
    #که_نه_تعریف_شوی configMAX_CO_ROUTINE_PRIORITIES
        #تېروتنه configMAX_CO_ROUTINE_PRIORITIES must be greater than or equal منزل 1.
    #د_که_پای
#د_که_پای

#که_نه_تعریف_شوی config_USE_APPLICATION_TASK_نښان
    #تعریف_کړه config_USE_APPLICATION_TASK_نښان    0
#د_که_پای

#که_نه_تعریف_شوی config_عدد_THREAD_LOCAL_STORAGE_اشارې
    #تعریف_کړه config_عدد_THREAD_LOCAL_STORAGE_اشارې    0
#د_که_پای

#که_نه_تعریف_شوی configUSE_RECURSIVE_MUTEXES
    #تعریف_کړه configUSE_RECURSIVE_MUTEXES    0
#د_که_پای

#که_نه_تعریف_شوی configUSE_MUTEXES
    #تعریف_کړه configUSE_MUTEXES    0
#د_که_پای

#که_نه_تعریف_شوی configUSE_TIMERS
    #تعریف_کړه configUSE_TIMERS    0
#د_که_پای

#که_نه_تعریف_شوی configUSE_EVENT_GROUPS
    #تعریف_کړه configUSE_EVENT_GROUPS    1
#د_که_پای

#که_نه_تعریف_شوی config_USE_بهیر_BUFFERS
    #تعریف_کړه config_USE_بهیر_BUFFERS    1
#د_که_پای

#که_نه_تعریف_شوی configUSE_DAEMON_TASK_STARTUP_HOOK
    #تعریف_کړه configUSE_DAEMON_TASK_STARTUP_HOOK    0
#د_که_پای

#إذا ( configUSE_DAEMON_TASK_STARTUP_HOOK != 0 )
    #إذا ( configUSE_TIMERS == 0 )
        #تېروتنه configUSE_DAEMON_TASK_STARTUP_HOOK is سېټ, but the daemon task is not created because configUSE_TIMERS is 0.
    #د_که_پای
#د_که_پای

#که_نه_تعریف_شوی configUSE_COUNTING_SEMAPHORES
    #تعریف_کړه configUSE_COUNTING_SEMAPHORES    0
#د_که_پای

#که_نه_تعریف_شوی configUSE_TASK_PREEMPTION_DISABLE
    #تعریف_کړه configUSE_TASK_PREEMPTION_DISABLE    0
#د_که_پای

#که_نه_تعریف_شوی configUSE_ALTERNATIVE_API
    #تعریف_کړه configUSE_ALTERNATIVE_API    0
#د_که_پای

#که_نه_تعریف_شوی پورټ_CRITICAL_NESTING_دننه_TCB
    #تعریف_کړه پورټ_CRITICAL_NESTING_دننه_TCB    0
#د_که_پای

#که_نه_تعریف_شوی configMAX_TASK_NAME_LEN
    #تعریف_کړه configMAX_TASK_NAME_LEN    16
#د_که_پای

#که_نه_تعریف_شوی configIDLE_SHOULD_YIELD
    #تعریف_کړه configIDLE_SHOULD_YIELD    1
#د_که_پای

#إذا configMAX_TASK_NAME_LEN < 1
    #تېروتنه configMAX_TASK_NAME_LEN must be سېټ منزل a minimum د 1 دننه FreeRTOSConfig.h
#د_که_پای

#که_نه_تعریف_شوی configASSERT
    #تعریف_کړه configASSERT( x )
    #تعریف_کړه configASSERT_DEFINED    0
#وإلا
    #تعریف_کړه configASSERT_DEFINED    1
#د_که_پای

/* configPRECONDITION should be defined as configASSERT.
 * The CBMC proofs need a way to track assumptions and assertions.
 * A configPRECONDITION statement should express an implicit invariant or
 * assumption made.  A configASSERT statement should express an invariant that must
 * hold explicit before calling the code. */
#که_نه_تعریف_شوی configPRECONDITION
    #تعریف_کړه configPRECONDITION( X )    configASSERT( X )
    #تعریف_کړه configPRECONDITION_DEFINED    0
#وإلا
    #تعریف_کړه configPRECONDITION_DEFINED    1
#د_که_پای

#که_نه_تعریف_شوی configCHECK_HANDLER_INSTALLATION
    #تعریف_کړه configCHECK_HANDLER_INSTALLATION    1
#وإلا

/* The application has explicitly defined configCHECK_HANDLER_INSTALLATION
 * to 1. The checks requires configASSERT() to be defined. */
    #إذا ( ( configCHECK_HANDLER_INSTALLATION == 1 ) && ( configASSERT_DEFINED == 0 ) )
        #تېروتنه You must define configASSERT() when configCHECK_HANDLER_INSTALLATION is 1.
    #د_که_پای
#د_که_پای

#که_نه_تعریف_شوی پورټ_حافظه_BARRIER
    #تعریف_کړه پورټ_حافظه_BARRIER()
#د_که_پای

#که_نه_تعریف_شوی پورټ_SOFTWARE_BARRIER
    #تعریف_کړه پورټ_SOFTWARE_BARRIER()
#د_که_پای

#که_نه_تعریف_شوی config_پرلپسې_مضرب_PRIORITIES
    #تعریف_کړه config_پرلپسې_مضرب_PRIORITIES    0
#د_که_پای

#که_نه_تعریف_شوی پورټ_GET_CORE_ID

    #إذا ( config_شمېره_د_CORES == 1 )
        #تعریف_کړه پورټ_GET_CORE_ID()    0
    #وإلا
        #تېروتنه config_شمېره_د_CORES is سېټ منزل نور than 1 then پورټ_GET_CORE_ID must also be defined.
    #د_که_پای /* configNUMBER_OF_CORES */

#د_که_پای /* portGET_CORE_ID */

#که_نه_تعریف_شوی پورټ_YIELD_CORE

    #إذا ( config_شمېره_د_CORES == 1 )
        #تعریف_کړه پورټ_YIELD_CORE( x )    پورټ_YIELD()
    #وإلا
        #تېروتنه config_شمېره_د_CORES is سېټ منزل نور than 1 then پورټ_YIELD_CORE must also be defined.
    #د_که_پای /* configNUMBER_OF_CORES */

#د_که_پای /* portYIELD_CORE */

#که_نه_تعریف_شوی پورټ_سېټ_مداخله_ماسک

    #إذا ( config_شمېره_د_CORES > 1 )
        #تېروتنه پورټ_سېټ_مداخله_ماسک is required دننه SMP
    #د_که_پای

#د_که_پای /* portSET_INTERRUPT_MASK */

#که_نه_تعریف_شوی پورټ_CLEAR_مداخله_ماسک

    #إذا ( config_شمېره_د_CORES > 1 )
        #تېروتنه پورټ_CLEAR_مداخله_ماسک is required دننه SMP
    #د_که_پای

#د_که_پای /* portCLEAR_INTERRUPT_MASK */

#که_نه_تعریف_شوی پورټ_RELEASE_TASK_قلف

    #إذا ( config_شمېره_د_CORES == 1 )
        #تعریف_کړه پورټ_RELEASE_TASK_قلف()
    #وإلا
        #تېروتنه پورټ_RELEASE_TASK_قلف is required دننه SMP
    #د_که_پای

#د_که_پای /* portRELEASE_TASK_LOCK */

#که_نه_تعریف_شوی پورټ_GET_TASK_قلف

    #إذا ( config_شمېره_د_CORES == 1 )
        #تعریف_کړه پورټ_GET_TASK_قلف()
    #وإلا
        #تېروتنه پورټ_GET_TASK_قلف is required دننه SMP
    #د_که_پای

#د_که_پای /* portGET_TASK_LOCK */

#که_نه_تعریف_شوی پورټ_RELEASE_ISR_قلف

    #إذا ( config_شمېره_د_CORES == 1 )
        #تعریف_کړه پورټ_RELEASE_ISR_قلف()
    #وإلا
        #تېروتنه پورټ_RELEASE_ISR_قلف is required دننه SMP
    #د_که_پای

#د_که_پای /* portRELEASE_ISR_LOCK */

#که_نه_تعریف_شوی پورټ_GET_ISR_قلف

    #إذا ( config_شمېره_د_CORES == 1 )
        #تعریف_کړه پورټ_GET_ISR_قلف()
    #وإلا
        #تېروتنه پورټ_GET_ISR_قلف is required دننه SMP
    #د_که_پای

#د_که_پای /* portGET_ISR_LOCK */

#که_نه_تعریف_شوی پورټ_ENTER_CRITICAL_FROM_ISR

    #إذا ( config_شمېره_د_CORES > 1 )
        #تېروتنه پورټ_ENTER_CRITICAL_FROM_ISR is required دننه SMP
    #د_که_پای

#د_که_پای

#که_نه_تعریف_شوی پورټ_EXIT_CRITICAL_FROM_ISR

    #إذا ( config_شمېره_د_CORES > 1 )
        #تېروتنه پورټ_EXIT_CRITICAL_FROM_ISR is required دننه SMP
    #د_که_پای

#د_که_پای

#که_نه_تعریف_شوی configUSE_CORE_AFFINITY
    #تعریف_کړه configUSE_CORE_AFFINITY    0
#د_که_پای /* configUSE_CORE_AFFINITY */

#إذا ( ( config_شمېره_د_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
    #که_نه_تعریف_شوی configTASK_DEFAULT_CORE_AFFINITY
        #تعریف_کړه configTASK_DEFAULT_CORE_AFFINITY    tskNO_AFFINITY
    #د_که_پای
#د_که_پای

#که_نه_تعریف_شوی configUSE_PASSIVE_IDLE_HOOK
    #تعریف_کړه configUSE_PASSIVE_IDLE_HOOK    0
#د_که_پای /* configUSE_PASSIVE_IDLE_HOOK */

/* The timers module relies on xTaskGetSchedulerState(). */
#إذا configUSE_TIMERS == 1

    #که_نه_تعریف_شوی config_TIMER_TASK_لومړیتوب
        #تېروتنه If configUSE_TIMERS is سېټ منزل 1 then config_TIMER_TASK_لومړیتوب must also be defined.
    #د_که_پای /* configTIMER_TASK_PRIORITY */

    #که_نه_تعریف_شوی config_TIMER_کتار_اوږدتیا
        #تېروتنه If configUSE_TIMERS is سېټ منزل 1 then config_TIMER_کتار_اوږدتیا must also be defined.
    #د_که_پای /* configTIMER_QUEUE_LENGTH */

    #که_نه_تعریف_شوی config_TIMER_TASK_ډېری_ژوروالی
        #تېروتنه If configUSE_TIMERS is سېټ منزل 1 then config_TIMER_TASK_ډېری_ژوروالی must also be defined.
    #د_که_پای /* configTIMER_TASK_STACK_DEPTH */

    #که_نه_تعریف_شوی پورټ_TIMER_CALLBACK_ATTRIBUTE
        #تعریف_کړه پورټ_TIMER_CALLBACK_ATTRIBUTE
    #د_که_پای /* portTIMER_CALLBACK_ATTRIBUTE */

#د_که_پای /* configUSE_TIMERS */

#که_نه_تعریف_شوی پورټ_HAS_NESTED_INTERRUPTS
    #إذا defined( پورټ_سېټ_مداخله_ماسک_FROM_ISR ) && defined( پورټ_CLEAR_مداخله_ماسک_FROM_ISR )
        #تعریف_کړه پورټ_HAS_NESTED_INTERRUPTS    1
    #وإلا
        #تعریف_کړه پورټ_HAS_NESTED_INTERRUPTS    0
    #د_که_پای
#د_که_پای

#که_نه_تعریف_شوی پورټ_سېټ_مداخله_ماسک_FROM_ISR
    #إذا ( پورټ_HAS_NESTED_INTERRUPTS == 1 )
        #تېروتنه پورټ_سېټ_مداخله_ماسک_FROM_ISR must be defined لكل ports that support nested interrupts (i.e. پورټ_HAS_NESTED_INTERRUPTS is سېټ منزل 1)
    #وإلا
        #تعریف_کړه پورټ_سېټ_مداخله_ماسک_FROM_ISR()    0
    #د_که_پای
#وإلا
    #إذا ( پورټ_HAS_NESTED_INTERRUPTS == 0 )
        #تېروتنه پورټ_سېټ_مداخله_ماسک_FROM_ISR must not be defined لكل ports that افعل not support nested interrupts (i.e. پورټ_HAS_NESTED_INTERRUPTS is سېټ منزل 0)
    #د_که_پای
#د_که_پای

#که_نه_تعریف_شوی پورټ_CLEAR_مداخله_ماسک_FROM_ISR
    #إذا ( پورټ_HAS_NESTED_INTERRUPTS == 1 )
        #تېروتنه پورټ_CLEAR_مداخله_ماسک_FROM_ISR must be defined لكل ports that support nested interrupts  (i.e. پورټ_HAS_NESTED_INTERRUPTS is سېټ منزل 1)
    #وإلا
        #تعریف_کړه پورټ_CLEAR_مداخله_ماسک_FROM_ISR( ux_Saved_حالت_ارزښت )    ( فراغ ) ( ux_Saved_حالت_ارزښت )
    #د_که_پای
#وإلا
    #إذا ( پورټ_HAS_NESTED_INTERRUPTS == 0 )
        #تېروتنه پورټ_CLEAR_مداخله_ماسک_FROM_ISR must not be defined لكل ports that افعل not support nested interrupts (i.e. پورټ_HAS_NESTED_INTERRUPTS is سېټ منزل 0)
    #د_که_پای
#د_که_پای

#که_نه_تعریف_شوی پورټ_CLEAN_پورته_TCB
    #تعریف_کړه پورټ_CLEAN_پورته_TCB( pxTCB )    ( فراغ ) ( pxTCB )
#د_که_پای

#که_نه_تعریف_شوی پورټ_PRE_TASK_DELETE_HOOK
    #تعریف_کړه پورټ_PRE_TASK_DELETE_HOOK( pv_Task_منزل_Delete, pxYieldPending )
#د_که_پای

#که_نه_تعریف_شوی پورټ_تیاری_TCB
    #تعریف_کړه پورټ_تیاری_TCB( pxTCB )    ( فراغ ) ( pxTCB )
#د_که_پای

#که_نه_تعریف_شوی پورټ_TASK_SWITCH_HOOK
    #تعریف_کړه پورټ_TASK_SWITCH_HOOK( pxTCB )    ( فراغ ) ( pxTCB )
#د_که_پای

#که_نه_تعریف_شوی config_کتار_REGISTRY_اندازه
    #تعریف_کړه config_کتار_REGISTRY_اندازه    0U
#د_که_پای

#إذا ( config_کتار_REGISTRY_اندازه < 1 )
    #تعریف_کړه v_کتار_جمع_کړه_منزل_Registry( x_کتار, pcName )
    #تعریف_کړه v_کتار_Unregister_کتار( x_کتار )
    #تعریف_کړه pc_کتار_Get_Name( x_کتار )
#د_که_پای

#که_نه_تعریف_شوی config_USE_MINI_LIST_توکی
    #تعریف_کړه config_USE_MINI_LIST_توکی    1
#د_که_پای

#که_نه_تعریف_شوی پورټ_اشاره_ګر_اندازه_TYPE
    #تعریف_کړه پورټ_اشاره_ګر_اندازه_TYPE    uint32_t
#د_که_پای

/* Remove any unused trace macros. */
#که_نه_تعریف_شوی trace_پیل

/* Used to perform any necessary initialisation - for example, open a file
 * into which trace is to be written. */
    #تعریف_کړه trace_پیل()
#د_که_پای

#که_نه_تعریف_شوی trace_پای_ټکی

/* Use to close a trace, for example close a file into which trace has been
 * written. */
    #تعریف_کړه trace_پای_ټکی()
#د_که_پای

#که_نه_تعریف_شوی trace_TASK_SWITCHED_دننه

/* Called after a task has been selected to run.  pxCurrentTCB holds a pointer
 * to the task control block of the selected task. */
    #تعریف_کړه trace_TASK_SWITCHED_دننه()
#د_که_پای

#که_نه_تعریف_شوی trace_INCREASE_TICK_شمېر

/* Called before stepping the tick count after waking from tickless idle
 * sleep. */
    #تعریف_کړه trace_INCREASE_TICK_شمېر( x )
#د_که_پای

#که_نه_تعریف_شوی trace_ټیټ_POWER_IDLE_BEGIN
    /* Called immediately before entering tickless idle. */
    #تعریف_کړه trace_ټیټ_POWER_IDLE_BEGIN()
#د_که_پای

#که_نه_تعریف_شوی trace_ټیټ_POWER_IDLE_پای_ټکی
    /* Called when returning to the Idle task after a tickless idle. */
    #تعریف_کړه trace_ټیټ_POWER_IDLE_پای_ټکی()
#د_که_پای

#که_نه_تعریف_شوی trace_TASK_SWITCHED_بهر

/* Called before a task has been selected to run.  pxCurrentTCB holds a pointer
 * to the task control block of the task being switched out. */
    #تعریف_کړه trace_TASK_SWITCHED_بهر()
#د_که_پای

#که_نه_تعریف_شوی trace_TASK_لومړیتوب_INHERIT

/* Called when a task attempts to take a mutex that is already held by a
 * lower priority task.  pxTCBOfMutexHolder is a pointer to the TCB of the task
 * that holds the mutex.  uxInheritedPriority is the priority the mutex holder
 * will inherit (the priority of the task that is attempting to obtain the
 * muted. */
    #تعریف_کړه trace_TASK_لومړیتوب_INHERIT( pxTCBOfMutexHolder, ux_Inherited_لومړیتوب )
#د_که_پای

#که_نه_تعریف_شوی trace_TASK_لومړیتوب_DISINHERIT

/* Called when a task releases a mutex, the holding of which had resulted in
 * the task inheriting the priority of a higher priority task.
 * pxTCBOfMutexHolder is a pointer to the TCB of the task that is releasing the
 * mutex.  uxOriginalPriority is the task's configured (base) priority. */
    #تعریف_کړه trace_TASK_لومړیتوب_DISINHERIT( pxTCBOfMutexHolder, ux_Original_لومړیتوب )
#د_که_پای

#که_نه_تعریف_شوی trace_BLOCKING_ON_کتار_RECEIVE

/* Task is about to block because it cannot read from a
 * queue/mutex/semaphore.  pxQueue is a pointer to the queue/mutex/semaphore
 * upon which the read was attempted.  pxCurrentTCB points to the TCB of the
 * task that attempted the read. */
    #تعریف_کړه trace_BLOCKING_ON_کتار_RECEIVE( px_کتار )
#د_که_پای

#که_نه_تعریف_شوی trace_BLOCKING_ON_کتار_PEEK

/* Task is about to block because it cannot read from a
 * queue/mutex/semaphore.  pxQueue is a pointer to the queue/mutex/semaphore
 * upon which the read was attempted.  pxCurrentTCB points to the TCB of the
 * task that attempted the read. */
    #تعریف_کړه trace_BLOCKING_ON_کتار_PEEK( px_کتار )
#د_که_پای

#که_نه_تعریف_شوی trace_BLOCKING_ON_کتار_SEND

/* Task is about to block because it cannot write to a
 * queue/mutex/semaphore.  pxQueue is a pointer to the queue/mutex/semaphore
 * upon which the write was attempted.  pxCurrentTCB points to the TCB of the
 * task that attempted the write. */
    #تعریف_کړه trace_BLOCKING_ON_کتار_SEND( px_کتار )
#د_که_پای

#که_نه_تعریف_شوی config_CHECK_FOR_ډېری_ډکېدنه
    #تعریف_کړه config_CHECK_FOR_ډېری_ډکېدنه    0
#د_که_پای

#که_نه_تعریف_شوی config_ریکارډ_ډېری_لوړ_پته
    #تعریف_کړه config_ریکارډ_ډېری_لوړ_پته    0
#د_که_پای

#که_نه_تعریف_شوی configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H
    #تعریف_کړه configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H    0
#د_که_پای

/* The following event macros are embedded in the kernel API calls. */

#که_نه_تعریف_شوی trace_MOVED_TASK_منزل_READY_STATE
    #تعریف_کړه trace_MOVED_TASK_منزل_READY_STATE( pxTCB )
#د_که_پای

#که_نه_تعریف_شوی trace_POST_MOVED_TASK_منزل_READY_STATE
    #تعریف_کړه trace_POST_MOVED_TASK_منزل_READY_STATE( pxTCB )
#د_که_پای

#که_نه_تعریف_شوی trace_MOVED_TASK_منزل_DELAYED_LIST
    #تعریف_کړه trace_MOVED_TASK_منزل_DELAYED_LIST()
#د_که_پای

#که_نه_تعریف_شوی trace_MOVED_TASK_منزل_ډکېدنه_DELAYED_LIST
    #تعریف_کړه trace_MOVED_TASK_منزل_ډکېدنه_DELAYED_LIST()
#د_که_پای

#که_نه_تعریف_شوی trace_کتار_CREATE
    #تعریف_کړه trace_کتار_CREATE( px_New_کتار )
#د_که_پای

#که_نه_تعریف_شوی trace_کتار_CREATE_FAILED
    #تعریف_کړه trace_کتار_CREATE_FAILED( uc_کتار_Type )
#د_که_پای

#که_نه_تعریف_شوی traceCREATE_MUTEX
    #تعریف_کړه traceCREATE_MUTEX( px_New_کتار )
#د_که_پای

#که_نه_تعریف_شوی traceCREATE_MUTEX_FAILED
    #تعریف_کړه traceCREATE_MUTEX_FAILED()
#د_که_پای

#که_نه_تعریف_شوی traceGIVE_MUTEX_RECURSIVE
    #تعریف_کړه traceGIVE_MUTEX_RECURSIVE( pxMutex )
#د_که_پای

#که_نه_تعریف_شوی traceGIVE_MUTEX_RECURSIVE_FAILED
    #تعریف_کړه traceGIVE_MUTEX_RECURSIVE_FAILED( pxMutex )
#د_که_پای

#که_نه_تعریف_شوی traceTAKE_MUTEX_RECURSIVE
    #تعریف_کړه traceTAKE_MUTEX_RECURSIVE( pxMutex )
#د_که_پای

#که_نه_تعریف_شوی traceTAKE_MUTEX_RECURSIVE_FAILED
    #تعریف_کړه traceTAKE_MUTEX_RECURSIVE_FAILED( pxMutex )
#د_که_پای

#که_نه_تعریف_شوی traceCREATE_COUNTING_SEMAPHORE
    #تعریف_کړه traceCREATE_COUNTING_SEMAPHORE()
#د_که_پای

#که_نه_تعریف_شوی traceCREATE_COUNTING_SEMAPHORE_FAILED
    #تعریف_کړه traceCREATE_COUNTING_SEMAPHORE_FAILED()
#د_که_پای

#که_نه_تعریف_شوی trace_کتار_سېټ_SEND
    #تعریف_کړه trace_کتار_سېټ_SEND    trace_کتار_SEND
#د_که_پای

#که_نه_تعریف_شوی trace_کتار_SEND
    #تعریف_کړه trace_کتار_SEND( px_کتار )
#د_که_پای

#که_نه_تعریف_شوی trace_کتار_SEND_FAILED
    #تعریف_کړه trace_کتار_SEND_FAILED( px_کتار )
#د_که_پای

#که_نه_تعریف_شوی trace_کتار_RECEIVE
    #تعریف_کړه trace_کتار_RECEIVE( px_کتار )
#د_که_پای

#که_نه_تعریف_شوی trace_کتار_PEEK
    #تعریف_کړه trace_کتار_PEEK( px_کتار )
#د_که_پای

#که_نه_تعریف_شوی trace_کتار_PEEK_FAILED
    #تعریف_کړه trace_کتار_PEEK_FAILED( px_کتار )
#د_که_پای

#که_نه_تعریف_شوی trace_کتار_PEEK_FROM_ISR
    #تعریف_کړه trace_کتار_PEEK_FROM_ISR( px_کتار )
#د_که_پای

#که_نه_تعریف_شوی trace_کتار_RECEIVE_FAILED
    #تعریف_کړه trace_کتار_RECEIVE_FAILED( px_کتار )
#د_که_پای

#که_نه_تعریف_شوی trace_کتار_SEND_FROM_ISR
    #تعریف_کړه trace_کتار_SEND_FROM_ISR( px_کتار )
#د_که_پای

#که_نه_تعریف_شوی trace_کتار_SEND_FROM_ISR_FAILED
    #تعریف_کړه trace_کتار_SEND_FROM_ISR_FAILED( px_کتار )
#د_که_پای

#که_نه_تعریف_شوی trace_کتار_RECEIVE_FROM_ISR
    #تعریف_کړه trace_کتار_RECEIVE_FROM_ISR( px_کتار )
#د_که_پای

#که_نه_تعریف_شوی trace_کتار_RECEIVE_FROM_ISR_FAILED
    #تعریف_کړه trace_کتار_RECEIVE_FROM_ISR_FAILED( px_کتار )
#د_که_پای

#که_نه_تعریف_شوی trace_کتار_PEEK_FROM_ISR_FAILED
    #تعریف_کړه trace_کتار_PEEK_FROM_ISR_FAILED( px_کتار )
#د_که_پای

#که_نه_تعریف_شوی trace_کتار_DELETE
    #تعریف_کړه trace_کتار_DELETE( px_کتار )
#د_که_پای

#که_نه_تعریف_شوی traceTASK_CREATE
    #تعریف_کړه traceTASK_CREATE( pxNewTCB )
#د_که_پای

#که_نه_تعریف_شوی traceTASK_CREATE_FAILED
    #تعریف_کړه traceTASK_CREATE_FAILED()
#د_که_پای

#که_نه_تعریف_شوی traceTASK_DELETE
    #تعریف_کړه traceTASK_DELETE( px_Task_منزل_Delete )
#د_که_پای

#که_نه_تعریف_شوی traceTASK_DELAY_UNTIL
    #تعریف_کړه traceTASK_DELAY_UNTIL( x )
#د_که_پای

#که_نه_تعریف_شوی traceTASK_DELAY
    #تعریف_کړه traceTASK_DELAY()
#د_که_پای

#که_نه_تعریف_شوی trace_TASK_لومړیتوب_سېټ
    #تعریف_کړه trace_TASK_لومړیتوب_سېټ( pxTask, ux_New_لومړیتوب )
#د_که_پای

#که_نه_تعریف_شوی traceTASK_SUSPEND
    #تعریف_کړه traceTASK_SUSPEND( px_Task_منزل_Suspend )
#د_که_پای

#که_نه_تعریف_شوی traceTASK_RESUME
    #تعریف_کړه traceTASK_RESUME( px_Task_منزل_Resume )
#د_که_پای

#که_نه_تعریف_شوی traceTASK_RESUME_FROM_ISR
    #تعریف_کړه traceTASK_RESUME_FROM_ISR( px_Task_منزل_Resume )
#د_که_پای

#که_نه_تعریف_شوی traceTASK_INCREMENT_TICK
    #تعریف_کړه traceTASK_INCREMENT_TICK( x_Tick_شمېر )
#د_که_پای

#که_نه_تعریف_شوی traceTIMER_CREATE
    #تعریف_کړه traceTIMER_CREATE( pxNewTimer )
#د_که_پای

#که_نه_تعریف_شوی traceTIMER_CREATE_FAILED
    #تعریف_کړه traceTIMER_CREATE_FAILED()
#د_که_پای

#که_نه_تعریف_شوی traceTIMER_COMMAND_SEND
    #تعریف_کړه traceTIMER_COMMAND_SEND( xTimer, x_پیغام_ID, x_پیغام_ارزښت_ارزښت, xReturn )
#د_که_پای

#که_نه_تعریف_شوی traceTIMER_EXPIRED
    #تعریف_کړه traceTIMER_EXPIRED( pxTimer )
#د_که_پای

#که_نه_تعریف_شوی traceTIMER_COMMAND_RECEIVED
    #تعریف_کړه traceTIMER_COMMAND_RECEIVED( pxTimer, x_پیغام_ID, x_پیغام_ارزښت )
#د_که_پای

#که_نه_تعریف_شوی traceMALLOC
    #تعریف_کړه traceMALLOC( pv_پته, ui_اندازه )
#د_که_پای

#که_نه_تعریف_شوی traceFREE
    #تعریف_کړه traceFREE( pv_پته, ui_اندازه )
#د_که_پای

#که_نه_تعریف_شوی trace_EVENT_ډله_CREATE
    #تعریف_کړه trace_EVENT_ډله_CREATE( x_Event_ډله )
#د_که_پای

#که_نه_تعریف_شوی trace_EVENT_ډله_CREATE_FAILED
    #تعریف_کړه trace_EVENT_ډله_CREATE_FAILED()
#د_که_پای

#که_نه_تعریف_شوی trace_EVENT_ډله_SYNC_بلاک
    #تعریف_کړه trace_EVENT_ډله_SYNC_بلاک( x_Event_ډله, ux_بټونه_منزل_سېټ, ux_بټونه_منزل_Wait_For )
#د_که_پای

#که_نه_تعریف_شوی trace_EVENT_ډله_SYNC_پای_ټکی
    #تعریف_کړه trace_EVENT_ډله_SYNC_پای_ټکی( x_Event_ډله, ux_بټونه_منزل_سېټ, ux_بټونه_منزل_Wait_For, xTimeoutOccurred )    ( فراغ ) ( xTimeoutOccurred )
#د_که_پای

#که_نه_تعریف_شوی trace_EVENT_ډله_WAIT_بټونه_بلاک
    #تعریف_کړه trace_EVENT_ډله_WAIT_بټونه_بلاک( x_Event_ډله, ux_بټونه_منزل_Wait_For )
#د_که_پای

#که_نه_تعریف_شوی trace_EVENT_ډله_WAIT_بټونه_پای_ټکی
    #تعریف_کړه trace_EVENT_ډله_WAIT_بټونه_پای_ټکی( x_Event_ډله, ux_بټونه_منزل_Wait_For, xTimeoutOccurred )    ( فراغ ) ( xTimeoutOccurred )
#د_که_پای

#که_نه_تعریف_شوی trace_EVENT_ډله_CLEAR_بټونه
    #تعریف_کړه trace_EVENT_ډله_CLEAR_بټونه( x_Event_ډله, ux_بټونه_منزل_Clear )
#د_که_پای

#که_نه_تعریف_شوی trace_EVENT_ډله_CLEAR_بټونه_FROM_ISR
    #تعریف_کړه trace_EVENT_ډله_CLEAR_بټونه_FROM_ISR( x_Event_ډله, ux_بټونه_منزل_Clear )
#د_که_پای

#که_نه_تعریف_شوی trace_EVENT_ډله_سېټ_بټونه
    #تعریف_کړه trace_EVENT_ډله_سېټ_بټونه( x_Event_ډله, ux_بټونه_منزل_سېټ )
#د_که_پای

#که_نه_تعریف_شوی trace_EVENT_ډله_سېټ_بټونه_FROM_ISR
    #تعریف_کړه trace_EVENT_ډله_سېټ_بټونه_FROM_ISR( x_Event_ډله, ux_بټونه_منزل_سېټ )
#د_که_پای

#که_نه_تعریف_شوی trace_EVENT_ډله_DELETE
    #تعریف_کړه trace_EVENT_ډله_DELETE( x_Event_ډله )
#د_که_پای

#که_نه_تعریف_شوی tracePEND_FUNC_CALL
    #تعریف_کړه tracePEND_FUNC_CALL( x_Function_منزل_Pend, pvParameter1, ulParameter2, ret )
#د_که_پای

#که_نه_تعریف_شوی tracePEND_FUNC_CALL_FROM_ISR
    #تعریف_کړه tracePEND_FUNC_CALL_FROM_ISR( x_Function_منزل_Pend, pvParameter1, ulParameter2, ret )
#د_که_پای

#که_نه_تعریف_شوی trace_کتار_REGISTRY_جمع_کړه
    #تعریف_کړه trace_کتار_REGISTRY_جمع_کړه( x_کتار, pc_کتار_Name )
#د_که_پای

#که_نه_تعریف_شوی trace_TASK_NOTIFY_TAKE_بلاک
    #تعریف_کړه trace_TASK_NOTIFY_TAKE_بلاک( ux_شاخص_منزل_Wait )
#د_که_پای

#که_نه_تعریف_شوی traceTASK_NOTIFY_TAKE
    #تعریف_کړه traceTASK_NOTIFY_TAKE( ux_شاخص_منزل_Wait )
#د_که_پای

#که_نه_تعریف_شوی trace_TASK_NOTIFY_WAIT_بلاک
    #تعریف_کړه trace_TASK_NOTIFY_WAIT_بلاک( ux_شاخص_منزل_Wait )
#د_که_پای

#که_نه_تعریف_شوی traceTASK_NOTIFY_WAIT
    #تعریف_کړه traceTASK_NOTIFY_WAIT( ux_شاخص_منزل_Wait )
#د_که_پای

#که_نه_تعریف_شوی traceTASK_NOTIFY
    #تعریف_کړه traceTASK_NOTIFY( ux_شاخص_منزل_Notify )
#د_که_پای

#که_نه_تعریف_شوی traceTASK_NOTIFY_FROM_ISR
    #تعریف_کړه traceTASK_NOTIFY_FROM_ISR( ux_شاخص_منزل_Notify )
#د_که_پای

#که_نه_تعریف_شوی traceTASK_NOTIFY_GIVE_FROM_ISR
    #تعریف_کړه traceTASK_NOTIFY_GIVE_FROM_ISR( ux_شاخص_منزل_Notify )
#د_که_پای

#که_نه_تعریف_شوی trace_ISR_EXIT_منزل_SCHEDULER
    #تعریف_کړه trace_ISR_EXIT_منزل_SCHEDULER()
#د_که_پای

#که_نه_تعریف_شوی traceISR_EXIT
    #تعریف_کړه traceISR_EXIT()
#د_که_پای

#که_نه_تعریف_شوی traceISR_ENTER
    #تعریف_کړه traceISR_ENTER()
#د_که_پای

#که_نه_تعریف_شوی trace_بهیر_بفر_CREATE_FAILED
    #تعریف_کړه trace_بهیر_بفر_CREATE_FAILED( x_بهیر_بفر_Type )
#د_که_پای

#که_نه_تعریف_شوی trace_بهیر_بفر_CREATE_STATIC_FAILED
    #تعریف_کړه trace_بهیر_بفر_CREATE_STATIC_FAILED( xReturn, x_بهیر_بفر_Type )
#د_که_پای

#که_نه_تعریف_شوی trace_بهیر_بفر_CREATE
    #تعریف_کړه trace_بهیر_بفر_CREATE( px_بهیر_بفر, x_بهیر_بفر_Type )
#د_که_پای

#که_نه_تعریف_شوی trace_بهیر_بفر_DELETE
    #تعریف_کړه trace_بهیر_بفر_DELETE( x_بهیر_بفر )
#د_که_پای

#که_نه_تعریف_شوی trace_بهیر_بفر_RESET
    #تعریف_کړه trace_بهیر_بفر_RESET( x_بهیر_بفر )
#د_که_پای

#که_نه_تعریف_شوی trace_بهیر_بفر_RESET_FROM_ISR
    #تعریف_کړه trace_بهیر_بفر_RESET_FROM_ISR( x_بهیر_بفر )
#د_که_پای

#که_نه_تعریف_شوی trace_BLOCKING_ON_بهیر_بفر_SEND
    #تعریف_کړه trace_BLOCKING_ON_بهیر_بفر_SEND( x_بهیر_بفر )
#د_که_پای

#که_نه_تعریف_شوی trace_بهیر_بفر_SEND
    #تعریف_کړه trace_بهیر_بفر_SEND( x_بهیر_بفر, x_بایټونه_لېږل_شوی )
#د_که_پای

#که_نه_تعریف_شوی trace_بهیر_بفر_SEND_FAILED
    #تعریف_کړه trace_بهیر_بفر_SEND_FAILED( x_بهیر_بفر )
#د_که_پای

#که_نه_تعریف_شوی trace_بهیر_بفر_SEND_FROM_ISR
    #تعریف_کړه trace_بهیر_بفر_SEND_FROM_ISR( x_بهیر_بفر, x_بایټونه_لېږل_شوی )
#د_که_پای

#که_نه_تعریف_شوی trace_BLOCKING_ON_بهیر_بفر_RECEIVE
    #تعریف_کړه trace_BLOCKING_ON_بهیر_بفر_RECEIVE( x_بهیر_بفر )
#د_که_پای

#که_نه_تعریف_شوی trace_بهیر_بفر_RECEIVE
    #تعریف_کړه trace_بهیر_بفر_RECEIVE( x_بهیر_بفر, x_Received_اوږدتیا )
#د_که_پای

#که_نه_تعریف_شوی trace_بهیر_بفر_RECEIVE_FAILED
    #تعریف_کړه trace_بهیر_بفر_RECEIVE_FAILED( x_بهیر_بفر )
#د_که_پای

#که_نه_تعریف_شوی trace_بهیر_بفر_RECEIVE_FROM_ISR
    #تعریف_کړه trace_بهیر_بفر_RECEIVE_FROM_ISR( x_بهیر_بفر, x_Received_اوږدتیا )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_Event_ډله_Create_Static
    #تعریف_کړه trace_ENTER_x_Event_ډله_Create_Static( px_Event_ډله_بفر )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_Event_ډله_Create_Static
    #تعریف_کړه trace_RETURN_x_Event_ډله_Create_Static( px_Event_بټونه )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_Event_ډله_Create
    #تعریف_کړه trace_ENTER_x_Event_ډله_Create()
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_Event_ډله_Create
    #تعریف_کړه trace_RETURN_x_Event_ډله_Create( px_Event_بټونه )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_Event_ډله_Sync
    #تعریف_کړه trace_ENTER_x_Event_ډله_Sync( x_Event_ډله, ux_بټونه_منزل_سېټ, ux_بټونه_منزل_Wait_For, x_Ticks_منزل_Wait )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_Event_ډله_Sync
    #تعریف_کړه trace_RETURN_x_Event_ډله_Sync( uxReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_Event_ډله_Wait_بټونه
    #تعریف_کړه trace_ENTER_x_Event_ډله_Wait_بټونه( x_Event_ډله, ux_بټونه_منزل_Wait_For, xClearOnExit, x_Wait_For_All_بټونه, x_Ticks_منزل_Wait )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_Event_ډله_Wait_بټونه
    #تعریف_کړه trace_RETURN_x_Event_ډله_Wait_بټونه( uxReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_Event_ډله_Clear_بټونه
    #تعریف_کړه trace_ENTER_x_Event_ډله_Clear_بټونه( x_Event_ډله, ux_بټونه_منزل_Clear )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_Event_ډله_Clear_بټونه
    #تعریف_کړه trace_RETURN_x_Event_ډله_Clear_بټونه( uxReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_Event_ډله_Clear_بټونه_From_ISR
    #تعریف_کړه trace_ENTER_x_Event_ډله_Clear_بټونه_From_ISR( x_Event_ډله, ux_بټونه_منزل_Clear )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_Event_ډله_Clear_بټونه_From_ISR
    #تعریف_کړه trace_RETURN_x_Event_ډله_Clear_بټونه_From_ISR( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_Event_ډله_Get_بټونه_From_ISR
    #تعریف_کړه trace_ENTER_x_Event_ډله_Get_بټونه_From_ISR( x_Event_ډله )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_Event_ډله_Get_بټونه_From_ISR
    #تعریف_کړه trace_RETURN_x_Event_ډله_Get_بټونه_From_ISR( uxReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_Event_ډله_سېټ_بټونه
    #تعریف_کړه trace_ENTER_x_Event_ډله_سېټ_بټونه( x_Event_ډله, ux_بټونه_منزل_سېټ )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_Event_ډله_سېټ_بټونه
    #تعریف_کړه trace_RETURN_x_Event_ډله_سېټ_بټونه( ux_Event_بټونه )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_v_Event_ډله_Delete
    #تعریف_کړه trace_ENTER_v_Event_ډله_Delete( x_Event_ډله )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_v_Event_ډله_Delete
    #تعریف_کړه trace_RETURN_v_Event_ډله_Delete()
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_Event_ډله_Get_Static_بفر
    #تعریف_کړه trace_ENTER_x_Event_ډله_Get_Static_بفر( x_Event_ډله, ppx_Event_ډله_بفر )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_Event_ډله_Get_Static_بفر
    #تعریف_کړه trace_RETURN_x_Event_ډله_Get_Static_بفر( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_v_Event_ډله_سېټ_بټونه_Callback
    #تعریف_کړه trace_ENTER_v_Event_ډله_سېټ_بټونه_Callback( pv_Event_ډله, ul_بټونه_منزل_سېټ )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_v_Event_ډله_سېټ_بټونه_Callback
    #تعریف_کړه trace_RETURN_v_Event_ډله_سېټ_بټونه_Callback()
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_v_Event_ډله_Clear_بټونه_Callback
    #تعریف_کړه trace_ENTER_v_Event_ډله_Clear_بټونه_Callback( pv_Event_ډله, ul_بټونه_منزل_Clear )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_v_Event_ډله_Clear_بټونه_Callback
    #تعریف_کړه trace_RETURN_v_Event_ډله_Clear_بټونه_Callback()
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_Event_ډله_سېټ_بټونه_From_ISR
    #تعریف_کړه trace_ENTER_x_Event_ډله_سېټ_بټونه_From_ISR( x_Event_ډله, ux_بټونه_منزل_سېټ, px_Higher_لومړیتوب_Task_Woken )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_Event_ډله_سېټ_بټونه_From_ISR
    #تعریف_کړه trace_RETURN_x_Event_ډله_سېټ_بټونه_From_ISR( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_ux_Event_ډله_Get_شمېره
    #تعریف_کړه trace_ENTER_ux_Event_ډله_Get_شمېره( x_Event_ډله )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_ux_Event_ډله_Get_شمېره
    #تعریف_کړه trace_RETURN_ux_Event_ډله_Get_شمېره( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_v_Event_ډله_سېټ_شمېره
    #تعریف_کړه trace_ENTER_v_Event_ډله_سېټ_شمېره( x_Event_ډله, ux_Event_ډله_شمېره )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_v_Event_ډله_سېټ_شمېره
    #تعریف_کړه trace_RETURN_v_Event_ډله_سېټ_شمېره()
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_کتار_Generic_Reset
    #تعریف_کړه trace_ENTER_x_کتار_Generic_Reset( x_کتار, x_New_کتار )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_کتار_Generic_Reset
    #تعریف_کړه trace_RETURN_x_کتار_Generic_Reset( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_کتار_Generic_Create_Static
    #تعریف_کړه trace_ENTER_x_کتار_Generic_Create_Static( ux_کتار_اوږدتیا, ux_توکی_اندازه, puc_کتار_Storage, px_Static_کتار, uc_کتار_Type )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_کتار_Generic_Create_Static
    #تعریف_کړه trace_RETURN_x_کتار_Generic_Create_Static( px_New_کتار )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_کتار_Generic_Get_Static_Buffers
    #تعریف_کړه trace_ENTER_x_کتار_Generic_Get_Static_Buffers( x_کتار, ppuc_کتار_Storage, ppx_Static_کتار )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_کتار_Generic_Get_Static_Buffers
    #تعریف_کړه trace_RETURN_x_کتار_Generic_Get_Static_Buffers( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_کتار_Generic_Create
    #تعریف_کړه trace_ENTER_x_کتار_Generic_Create( ux_کتار_اوږدتیا, ux_توکی_اندازه, uc_کتار_Type )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_کتار_Generic_Create
    #تعریف_کړه trace_RETURN_x_کتار_Generic_Create( px_New_کتار )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_کتار_Create_Mutex
    #تعریف_کړه trace_ENTER_x_کتار_Create_Mutex( uc_کتار_Type )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_کتار_Create_Mutex
    #تعریف_کړه trace_RETURN_x_کتار_Create_Mutex( x_New_کتار )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_کتار_Create_Mutex_Static
    #تعریف_کړه trace_ENTER_x_کتار_Create_Mutex_Static( uc_کتار_Type, px_Static_کتار )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_کتار_Create_Mutex_Static
    #تعریف_کړه trace_RETURN_x_کتار_Create_Mutex_Static( x_New_کتار )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_کتار_Get_Mutex_Holder
    #تعریف_کړه trace_ENTER_x_کتار_Get_Mutex_Holder( xSemaphore )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_کتار_Get_Mutex_Holder
    #تعریف_کړه trace_RETURN_x_کتار_Get_Mutex_Holder( pxReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_کتار_Get_Mutex_Holder_From_ISR
    #تعریف_کړه trace_ENTER_x_کتار_Get_Mutex_Holder_From_ISR( xSemaphore )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_کتار_Get_Mutex_Holder_From_ISR
    #تعریف_کړه trace_RETURN_x_کتار_Get_Mutex_Holder_From_ISR( pxReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_کتار_Give_Mutex_Recursive
    #تعریف_کړه trace_ENTER_x_کتار_Give_Mutex_Recursive( xMutex )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_کتار_Give_Mutex_Recursive
    #تعریف_کړه trace_RETURN_x_کتار_Give_Mutex_Recursive( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_کتار_Take_Mutex_Recursive
    #تعریف_کړه trace_ENTER_x_کتار_Take_Mutex_Recursive( xMutex, x_Ticks_منزل_Wait )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_کتار_Take_Mutex_Recursive
    #تعریف_کړه trace_RETURN_x_کتار_Take_Mutex_Recursive( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_کتار_Create_Counting_Semaphore_Static
    #تعریف_کړه trace_ENTER_x_کتار_Create_Counting_Semaphore_Static( ux_Max_شمېر, ux_Initial_شمېر, px_Static_کتار )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_کتار_Create_Counting_Semaphore_Static
    #تعریف_کړه trace_RETURN_x_کتار_Create_Counting_Semaphore_Static( xHandle )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_کتار_Create_Counting_Semaphore
    #تعریف_کړه trace_ENTER_x_کتار_Create_Counting_Semaphore( ux_Max_شمېر, ux_Initial_شمېر )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_کتار_Create_Counting_Semaphore
    #تعریف_کړه trace_RETURN_x_کتار_Create_Counting_Semaphore( xHandle )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_کتار_Generic_Send
    #تعریف_کړه trace_ENTER_x_کتار_Generic_Send( x_کتار, pv_توکی_منزل_کتار, x_Ticks_منزل_Wait, xCopyPosition )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_کتار_Generic_Send
    #تعریف_کړه trace_RETURN_x_کتار_Generic_Send( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_کتار_Generic_Send_From_ISR
    #تعریف_کړه trace_ENTER_x_کتار_Generic_Send_From_ISR( x_کتار, pv_توکی_منزل_کتار, px_Higher_لومړیتوب_Task_Woken, xCopyPosition )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_کتار_Generic_Send_From_ISR
    #تعریف_کړه trace_RETURN_x_کتار_Generic_Send_From_ISR( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_کتار_Give_From_ISR
    #تعریف_کړه trace_ENTER_x_کتار_Give_From_ISR( x_کتار, px_Higher_لومړیتوب_Task_Woken )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_کتار_Give_From_ISR
    #تعریف_کړه trace_RETURN_x_کتار_Give_From_ISR( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_کتار_Receive
    #تعریف_کړه trace_ENTER_x_کتار_Receive( x_کتار, pv_بفر, x_Ticks_منزل_Wait )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_کتار_Receive
    #تعریف_کړه trace_RETURN_x_کتار_Receive( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_کتار_Semaphore_Take
    #تعریف_کړه trace_ENTER_x_کتار_Semaphore_Take( x_کتار, x_Ticks_منزل_Wait )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_کتار_Semaphore_Take
    #تعریف_کړه trace_RETURN_x_کتار_Semaphore_Take( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_کتار_Peek
    #تعریف_کړه trace_ENTER_x_کتار_Peek( x_کتار, pv_بفر, x_Ticks_منزل_Wait )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_کتار_Peek
    #تعریف_کړه trace_RETURN_x_کتار_Peek( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_کتار_Receive_From_ISR
    #تعریف_کړه trace_ENTER_x_کتار_Receive_From_ISR( x_کتار, pv_بفر, px_Higher_لومړیتوب_Task_Woken )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_کتار_Receive_From_ISR
    #تعریف_کړه trace_RETURN_x_کتار_Receive_From_ISR( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_کتار_Peek_From_ISR
    #تعریف_کړه trace_ENTER_x_کتار_Peek_From_ISR( x_کتار, pv_بفر )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_کتار_Peek_From_ISR
    #تعریف_کړه trace_RETURN_x_کتار_Peek_From_ISR( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_ux_کتار_Messages_انتظار
    #تعریف_کړه trace_ENTER_ux_کتار_Messages_انتظار( x_کتار )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_ux_کتار_Messages_انتظار
    #تعریف_کړه trace_RETURN_ux_کتار_Messages_انتظار( uxReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_ux_کتار_Spaces_شته
    #تعریف_کړه trace_ENTER_ux_کتار_Spaces_شته( x_کتار )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_ux_کتار_Spaces_شته
    #تعریف_کړه trace_RETURN_ux_کتار_Spaces_شته( uxReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_ux_کتار_Messages_انتظار_From_ISR
    #تعریف_کړه trace_ENTER_ux_کتار_Messages_انتظار_From_ISR( x_کتار )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_ux_کتار_Messages_انتظار_From_ISR
    #تعریف_کړه trace_RETURN_ux_کتار_Messages_انتظار_From_ISR( uxReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_v_کتار_Delete
    #تعریف_کړه trace_ENTER_v_کتار_Delete( x_کتار )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_v_کتار_Delete
    #تعریف_کړه trace_RETURN_v_کتار_Delete()
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_ux_کتار_Get_کتار_شمېره
    #تعریف_کړه trace_ENTER_ux_کتار_Get_کتار_شمېره( x_کتار )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_ux_کتار_Get_کتار_شمېره
    #تعریف_کړه trace_RETURN_ux_کتار_Get_کتار_شمېره( ux_کتار_شمېره )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_v_کتار_سېټ_کتار_شمېره
    #تعریف_کړه trace_ENTER_v_کتار_سېټ_کتار_شمېره( x_کتار, ux_کتار_شمېره )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_v_کتار_سېټ_کتار_شمېره
    #تعریف_کړه trace_RETURN_v_کتار_سېټ_کتار_شمېره()
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_uc_کتار_Get_کتار_Type
    #تعریف_کړه trace_ENTER_uc_کتار_Get_کتار_Type( x_کتار )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_uc_کتار_Get_کتار_Type
    #تعریف_کړه trace_RETURN_uc_کتار_Get_کتار_Type( uc_کتار_Type )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_ux_کتار_Get_کتار_توکی_اندازه
    #تعریف_کړه trace_ENTER_ux_کتار_Get_کتار_توکی_اندازه( x_کتار )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_ux_کتار_Get_کتار_توکی_اندازه
    #تعریف_کړه trace_RETURN_ux_کتار_Get_کتار_توکی_اندازه( ux_توکی_اندازه )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_ux_کتار_Get_کتار_اوږدتیا
    #تعریف_کړه trace_ENTER_ux_کتار_Get_کتار_اوږدتیا( x_کتار )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_ux_کتار_Get_کتار_اوږدتیا
    #تعریف_کړه trace_RETURN_ux_کتار_Get_کتار_اوږدتیا( ux_اوږدتیا )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_کتار_Is_کتار_تشه_From_ISR
    #تعریف_کړه trace_ENTER_x_کتار_Is_کتار_تشه_From_ISR( x_کتار )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_کتار_Is_کتار_تشه_From_ISR
    #تعریف_کړه trace_RETURN_x_کتار_Is_کتار_تشه_From_ISR( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_کتار_Is_کتار_ډک_From_ISR
    #تعریف_کړه trace_ENTER_x_کتار_Is_کتار_ډک_From_ISR( x_کتار )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_کتار_Is_کتار_ډک_From_ISR
    #تعریف_کړه trace_RETURN_x_کتار_Is_کتار_ډک_From_ISR( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_کتار_CRSend
    #تعریف_کړه trace_ENTER_x_کتار_CRSend( x_کتار, pv_توکی_منزل_کتار, x_Ticks_منزل_Wait )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_کتار_CRSend
    #تعریف_کړه trace_RETURN_x_کتار_CRSend( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_کتار_CRReceive
    #تعریف_کړه trace_ENTER_x_کتار_CRReceive( x_کتار, pv_بفر, x_Ticks_منزل_Wait )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_کتار_CRReceive
    #تعریف_کړه trace_RETURN_x_کتار_CRReceive( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_کتار_CRSend_From_ISR
    #تعریف_کړه trace_ENTER_x_کتار_CRSend_From_ISR( x_کتار, pv_توکی_منزل_کتار, xCoRoutinePreviouslyWoken )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_کتار_CRSend_From_ISR
    #تعریف_کړه trace_RETURN_x_کتار_CRSend_From_ISR( xCoRoutinePreviouslyWoken )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_کتار_CRReceive_From_ISR
    #تعریف_کړه trace_ENTER_x_کتار_CRReceive_From_ISR( x_کتار, pv_بفر, pxCoRoutineWoken )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_کتار_CRReceive_From_ISR
    #تعریف_کړه trace_RETURN_x_کتار_CRReceive_From_ISR( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_v_کتار_جمع_کړه_منزل_Registry
    #تعریف_کړه trace_ENTER_v_کتار_جمع_کړه_منزل_Registry( x_کتار, pc_کتار_Name )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_v_کتار_جمع_کړه_منزل_Registry
    #تعریف_کړه trace_RETURN_v_کتار_جمع_کړه_منزل_Registry()
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_pc_کتار_Get_Name
    #تعریف_کړه trace_ENTER_pc_کتار_Get_Name( x_کتار )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_pc_کتار_Get_Name
    #تعریف_کړه trace_RETURN_pc_کتار_Get_Name( pcReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_v_کتار_Unregister_کتار
    #تعریف_کړه trace_ENTER_v_کتار_Unregister_کتار( x_کتار )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_v_کتار_Unregister_کتار
    #تعریف_کړه trace_RETURN_v_کتار_Unregister_کتار()
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_v_کتار_Wait_For_پیغام_Restricted
    #تعریف_کړه trace_ENTER_v_کتار_Wait_For_پیغام_Restricted( x_کتار, x_Ticks_منزل_Wait, xWaitIndefinitely )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_v_کتار_Wait_For_پیغام_Restricted
    #تعریف_کړه trace_RETURN_v_کتار_Wait_For_پیغام_Restricted()
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_کتار_Create_سېټ
    #تعریف_کړه trace_ENTER_x_کتار_Create_سېټ( ux_Event_کتار_اوږدتیا )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_کتار_Create_سېټ
    #تعریف_کړه trace_RETURN_x_کتار_Create_سېټ( px_کتار )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_کتار_جمع_کړه_منزل_سېټ
    #تعریف_کړه trace_ENTER_x_کتار_جمع_کړه_منزل_سېټ( x_کتار_Or_Semaphore, x_کتار_سېټ )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_کتار_جمع_کړه_منزل_سېټ
    #تعریف_کړه trace_RETURN_x_کتار_جمع_کړه_منزل_سېټ( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_کتار_Remove_From_سېټ
    #تعریف_کړه trace_ENTER_x_کتار_Remove_From_سېټ( x_کتار_Or_Semaphore, x_کتار_سېټ )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_کتار_Remove_From_سېټ
    #تعریف_کړه trace_RETURN_x_کتار_Remove_From_سېټ( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_کتار_Select_From_سېټ
    #تعریف_کړه trace_ENTER_x_کتار_Select_From_سېټ( x_کتار_سېټ, x_Ticks_منزل_Wait )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_کتار_Select_From_سېټ
    #تعریف_کړه trace_RETURN_x_کتار_Select_From_سېټ( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_کتار_Select_From_سېټ_From_ISR
    #تعریف_کړه trace_ENTER_x_کتار_Select_From_سېټ_From_ISR( x_کتار_سېټ )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_کتار_Select_From_سېټ_From_ISR
    #تعریف_کړه trace_RETURN_x_کتار_Select_From_سېټ_From_ISR( xReturn )
#د_که_پای

#که_نه_تعریف_شوی traceENTER_xTimerCreateTimerTask
    #تعریف_کړه traceENTER_xTimerCreateTimerTask()
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_xTimerCreateTimerTask
    #تعریف_کړه traceRETURN_xTimerCreateTimerTask( xReturn )
#د_که_پای

#که_نه_تعریف_شوی traceENTER_xTimerCreate
    #تعریف_کړه traceENTER_xTimerCreate( pcTimerName, x_Timer_Period_دننه_Ticks, xAutoReload, pvTimerID, pxCallbackFunction )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_xTimerCreate
    #تعریف_کړه traceRETURN_xTimerCreate( pxNewTimer )
#د_که_پای

#که_نه_تعریف_شوی traceENTER_xTimerCreateStatic
    #تعریف_کړه traceENTER_xTimerCreateStatic( pcTimerName, x_Timer_Period_دننه_Ticks, xAutoReload, pvTimerID, pxCallbackFunction, px_Timer_بفر )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_xTimerCreateStatic
    #تعریف_کړه traceRETURN_xTimerCreateStatic( pxNewTimer )
#د_که_پای

#که_نه_تعریف_شوی traceENTER_xTimerGenericCommandFromTask
    #تعریف_کړه traceENTER_xTimerGenericCommandFromTask( xTimer, xCommandID, x_Optional_ارزښت, px_Higher_لومړیتوب_Task_Woken, x_Ticks_منزل_Wait )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_xTimerGenericCommandFromTask
    #تعریف_کړه traceRETURN_xTimerGenericCommandFromTask( xReturn )
#د_که_پای

#که_نه_تعریف_شوی traceENTER_xTimerGenericCommandFromISR
    #تعریف_کړه traceENTER_xTimerGenericCommandFromISR( xTimer, xCommandID, x_Optional_ارزښت, px_Higher_لومړیتوب_Task_Woken, x_Ticks_منزل_Wait )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_xTimerGenericCommandFromISR
    #تعریف_کړه traceRETURN_xTimerGenericCommandFromISR( xReturn )
#د_که_پای

#که_نه_تعریف_شوی traceENTER_xTimerGetTimerDaemonTaskHandle
    #تعریف_کړه traceENTER_xTimerGetTimerDaemonTaskHandle()
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_xTimerGetTimerDaemonTaskHandle
    #تعریف_کړه traceRETURN_xTimerGetTimerDaemonTaskHandle( xTimerTaskHandle )
#د_که_پای

#که_نه_تعریف_شوی traceENTER_xTimerGetPeriod
    #تعریف_کړه traceENTER_xTimerGetPeriod( xTimer )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_xTimerGetPeriod
    #تعریف_کړه traceRETURN_xTimerGetPeriod( x_Timer_Period_دننه_Ticks )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_v_Timer_سېټ_Reload_ډېرپېښ
    #تعریف_کړه trace_ENTER_v_Timer_سېټ_Reload_ډېرپېښ( xTimer, xAutoReload )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_v_Timer_سېټ_Reload_ډېرپېښ
    #تعریف_کړه trace_RETURN_v_Timer_سېټ_Reload_ډېرپېښ()
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_Timer_Get_Reload_ډېرپېښ
    #تعریف_کړه trace_ENTER_x_Timer_Get_Reload_ډېرپېښ( xTimer )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_Timer_Get_Reload_ډېرپېښ
    #تعریف_کړه trace_RETURN_x_Timer_Get_Reload_ډېرپېښ( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_ux_Timer_Get_Reload_ډېرپېښ
    #تعریف_کړه trace_ENTER_ux_Timer_Get_Reload_ډېرپېښ( xTimer )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_ux_Timer_Get_Reload_ډېرپېښ
    #تعریف_کړه trace_RETURN_ux_Timer_Get_Reload_ډېرپېښ( uxReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_Timer_Get_Expiry_وخت
    #تعریف_کړه trace_ENTER_x_Timer_Get_Expiry_وخت( xTimer )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_Timer_Get_Expiry_وخت
    #تعریف_کړه trace_RETURN_x_Timer_Get_Expiry_وخت( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_Timer_Get_Static_بفر
    #تعریف_کړه trace_ENTER_x_Timer_Get_Static_بفر( xTimer, ppx_Timer_بفر )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_Timer_Get_Static_بفر
    #تعریف_کړه trace_RETURN_x_Timer_Get_Static_بفر( xReturn )
#د_که_پای

#که_نه_تعریف_شوی traceENTER_pcTimerGetName
    #تعریف_کړه traceENTER_pcTimerGetName( xTimer )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_pcTimerGetName
    #تعریف_کړه traceRETURN_pcTimerGetName( pcTimerName )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_Timer_Is_Timer_فعال
    #تعریف_کړه trace_ENTER_x_Timer_Is_Timer_فعال( xTimer )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_Timer_Is_Timer_فعال
    #تعریف_کړه trace_RETURN_x_Timer_Is_Timer_فعال( xReturn )
#د_که_پای

#که_نه_تعریف_شوی traceENTER_pvTimerGetTimerID
    #تعریف_کړه traceENTER_pvTimerGetTimerID( xTimer )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_pvTimerGetTimerID
    #تعریف_کړه traceRETURN_pvTimerGetTimerID( pvReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_v_Timer_سېټ_Timer_ID
    #تعریف_کړه trace_ENTER_v_Timer_سېټ_Timer_ID( xTimer, pvNewID )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_v_Timer_سېټ_Timer_ID
    #تعریف_کړه trace_RETURN_v_Timer_سېټ_Timer_ID()
#د_که_پای

#که_نه_تعریف_شوی traceENTER_xTimerPendFunctionCallFromISR
    #تعریف_کړه traceENTER_xTimerPendFunctionCallFromISR( x_Function_منزل_Pend, pvParameter1, ulParameter2, px_Higher_لومړیتوب_Task_Woken )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_xTimerPendFunctionCallFromISR
    #تعریف_کړه traceRETURN_xTimerPendFunctionCallFromISR( xReturn )
#د_که_پای

#که_نه_تعریف_شوی traceENTER_xTimerPendFunctionCall
    #تعریف_کړه traceENTER_xTimerPendFunctionCall( x_Function_منزل_Pend, pvParameter1, ulParameter2, x_Ticks_منزل_Wait )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_xTimerPendFunctionCall
    #تعریف_کړه traceRETURN_xTimerPendFunctionCall( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_ux_Timer_Get_Timer_شمېره
    #تعریف_کړه trace_ENTER_ux_Timer_Get_Timer_شمېره( xTimer )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_ux_Timer_Get_Timer_شمېره
    #تعریف_کړه trace_RETURN_ux_Timer_Get_Timer_شمېره( ux_Timer_شمېره )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_v_Timer_سېټ_Timer_شمېره
    #تعریف_کړه trace_ENTER_v_Timer_سېټ_Timer_شمېره( xTimer, ux_Timer_شمېره )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_v_Timer_سېټ_Timer_شمېره
    #تعریف_کړه trace_RETURN_v_Timer_سېټ_Timer_شمېره()
#د_که_پای

#که_نه_تعریف_شوی traceENTER_xTaskCreateStatic
    #تعریف_کړه traceENTER_xTaskCreateStatic( px_Task_کوډ, pcName, ux_ډېری_ژوروالی, pvParameters, ux_لومړیتوب, pux_ډېری_بفر, px_Task_بفر )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_xTaskCreateStatic
    #تعریف_کړه traceRETURN_xTaskCreateStatic( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_Task_Create_Static_Affinity_سېټ
    #تعریف_کړه trace_ENTER_x_Task_Create_Static_Affinity_سېټ( px_Task_کوډ, pcName, ux_ډېری_ژوروالی, pvParameters, ux_لومړیتوب, pux_ډېری_بفر, px_Task_بفر, ux_Core_Affinity_ماسک )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_Task_Create_Static_Affinity_سېټ
    #تعریف_کړه trace_RETURN_x_Task_Create_Static_Affinity_سېټ( xReturn )
#د_که_پای

#که_نه_تعریف_شوی traceENTER_xTaskCreateRestrictedStatic
    #تعریف_کړه traceENTER_xTaskCreateRestrictedStatic( pxTaskDefinition, pxCreatedTask )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_xTaskCreateRestrictedStatic
    #تعریف_کړه traceRETURN_xTaskCreateRestrictedStatic( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_Task_Create_Restricted_Static_Affinity_سېټ
    #تعریف_کړه trace_ENTER_x_Task_Create_Restricted_Static_Affinity_سېټ( pxTaskDefinition, ux_Core_Affinity_ماسک, pxCreatedTask )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_Task_Create_Restricted_Static_Affinity_سېټ
    #تعریف_کړه trace_RETURN_x_Task_Create_Restricted_Static_Affinity_سېټ( xReturn )
#د_که_پای

#که_نه_تعریف_شوی traceENTER_xTaskCreateRestricted
    #تعریف_کړه traceENTER_xTaskCreateRestricted( pxTaskDefinition, pxCreatedTask )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_xTaskCreateRestricted
    #تعریف_کړه traceRETURN_xTaskCreateRestricted( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_Task_Create_Restricted_Affinity_سېټ
    #تعریف_کړه trace_ENTER_x_Task_Create_Restricted_Affinity_سېټ( pxTaskDefinition, ux_Core_Affinity_ماسک, pxCreatedTask )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_Task_Create_Restricted_Affinity_سېټ
    #تعریف_کړه trace_RETURN_x_Task_Create_Restricted_Affinity_سېټ( xReturn )
#د_که_پای

#که_نه_تعریف_شوی traceENTER_xTaskCreate
    #تعریف_کړه traceENTER_xTaskCreate( px_Task_کوډ, pcName, ux_ډېری_ژوروالی, pvParameters, ux_لومړیتوب, pxCreatedTask )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_xTaskCreate
    #تعریف_کړه traceRETURN_xTaskCreate( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_Task_Create_Affinity_سېټ
    #تعریف_کړه trace_ENTER_x_Task_Create_Affinity_سېټ( px_Task_کوډ, pcName, ux_ډېری_ژوروالی, pvParameters, ux_لومړیتوب, ux_Core_Affinity_ماسک, pxCreatedTask )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_Task_Create_Affinity_سېټ
    #تعریف_کړه trace_RETURN_x_Task_Create_Affinity_سېټ( xReturn )
#د_که_پای

#که_نه_تعریف_شوی traceENTER_vTaskDelete
    #تعریف_کړه traceENTER_vTaskDelete( x_Task_منزل_Delete )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_vTaskDelete
    #تعریف_کړه traceRETURN_vTaskDelete()
#د_که_پای

#که_نه_تعریف_شوی traceENTER_xTaskDelayUntil
    #تعریف_کړه traceENTER_xTaskDelayUntil( px_پخوانی_Wake_وخت, x_وخت_Increment )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_xTaskDelayUntil
    #تعریف_کړه traceRETURN_xTaskDelayUntil( xShouldDelay )
#د_که_پای

#که_نه_تعریف_شوی traceENTER_vTaskDelay
    #تعریف_کړه traceENTER_vTaskDelay( x_Ticks_منزل_Delay )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_vTaskDelay
    #تعریف_کړه traceRETURN_vTaskDelay()
#د_که_پای

#که_نه_تعریف_شوی traceENTER_eTaskGetState
    #تعریف_کړه traceENTER_eTaskGetState( xTask )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_eTaskGetState
    #تعریف_کړه traceRETURN_eTaskGetState( eReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_ux_Task_لومړیتوب_Get
    #تعریف_کړه trace_ENTER_ux_Task_لومړیتوب_Get( xTask )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_ux_Task_لومړیتوب_Get
    #تعریف_کړه trace_RETURN_ux_Task_لومړیتوب_Get( uxReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_ux_Task_لومړیتوب_Get_From_ISR
    #تعریف_کړه trace_ENTER_ux_Task_لومړیتوب_Get_From_ISR( xTask )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_ux_Task_لومړیتوب_Get_From_ISR
    #تعریف_کړه trace_RETURN_ux_Task_لومړیتوب_Get_From_ISR( uxReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_ux_Task_بنسټ_لومړیتوب_Get
    #تعریف_کړه trace_ENTER_ux_Task_بنسټ_لومړیتوب_Get( xTask )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_ux_Task_بنسټ_لومړیتوب_Get
    #تعریف_کړه trace_RETURN_ux_Task_بنسټ_لومړیتوب_Get( uxReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_ux_Task_بنسټ_لومړیتوب_Get_From_ISR
    #تعریف_کړه trace_ENTER_ux_Task_بنسټ_لومړیتوب_Get_From_ISR( xTask )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_ux_Task_بنسټ_لومړیتوب_Get_From_ISR
    #تعریف_کړه trace_RETURN_ux_Task_بنسټ_لومړیتوب_Get_From_ISR( uxReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_v_Task_لومړیتوب_سېټ
    #تعریف_کړه trace_ENTER_v_Task_لومړیتوب_سېټ( xTask, ux_New_لومړیتوب )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_v_Task_لومړیتوب_سېټ
    #تعریف_کړه trace_RETURN_v_Task_لومړیتوب_سېټ()
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_v_Task_Core_Affinity_سېټ
    #تعریف_کړه trace_ENTER_v_Task_Core_Affinity_سېټ( xTask, ux_Core_Affinity_ماسک )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_v_Task_Core_Affinity_سېټ
    #تعریف_کړه trace_RETURN_v_Task_Core_Affinity_سېټ()
#د_که_پای

#که_نه_تعریف_شوی traceENTER_vTaskCoreAffinityGet
    #تعریف_کړه traceENTER_vTaskCoreAffinityGet( xTask )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_vTaskCoreAffinityGet
    #تعریف_کړه traceRETURN_vTaskCoreAffinityGet( ux_Core_Affinity_ماسک )
#د_که_پای

#که_نه_تعریف_شوی traceENTER_vTaskPreemptionDisable
    #تعریف_کړه traceENTER_vTaskPreemptionDisable( xTask )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_vTaskPreemptionDisable
    #تعریف_کړه traceRETURN_vTaskPreemptionDisable()
#د_که_پای

#که_نه_تعریف_شوی traceENTER_vTaskPreemptionEnable
    #تعریف_کړه traceENTER_vTaskPreemptionEnable( xTask )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_vTaskPreemptionEnable
    #تعریف_کړه traceRETURN_vTaskPreemptionEnable()
#د_که_پای

#که_نه_تعریف_شوی traceENTER_vTaskSuspend
    #تعریف_کړه traceENTER_vTaskSuspend( x_Task_منزل_Suspend )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_vTaskSuspend
    #تعریف_کړه traceRETURN_vTaskSuspend()
#د_که_پای

#که_نه_تعریف_شوی traceENTER_vTaskResume
    #تعریف_کړه traceENTER_vTaskResume( x_Task_منزل_Resume )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_vTaskResume
    #تعریف_کړه traceRETURN_vTaskResume()
#د_که_پای

#که_نه_تعریف_شوی traceENTER_xTaskResumeFromISR
    #تعریف_کړه traceENTER_xTaskResumeFromISR( x_Task_منزل_Resume )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_xTaskResumeFromISR
    #تعریف_کړه traceRETURN_xTaskResumeFromISR( xYieldRequired )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_v_Task_پیل_Scheduler
    #تعریف_کړه trace_ENTER_v_Task_پیل_Scheduler()
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_v_Task_پیل_Scheduler
    #تعریف_کړه trace_RETURN_v_Task_پیل_Scheduler()
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_v_Task_پای_ټکی_Scheduler
    #تعریف_کړه trace_ENTER_v_Task_پای_ټکی_Scheduler()
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_v_Task_پای_ټکی_Scheduler
    #تعریف_کړه trace_RETURN_v_Task_پای_ټکی_Scheduler()
#د_که_پای

#که_نه_تعریف_شوی traceENTER_vTaskSuspendAll
    #تعریف_کړه traceENTER_vTaskSuspendAll()
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_vTaskSuspendAll
    #تعریف_کړه traceRETURN_vTaskSuspendAll()
#د_که_پای

#که_نه_تعریف_شوی traceENTER_xTaskResumeAll
    #تعریف_کړه traceENTER_xTaskResumeAll()
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_xTaskResumeAll
    #تعریف_کړه traceRETURN_xTaskResumeAll( xAlreadyYielded )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_Task_Get_Tick_شمېر
    #تعریف_کړه trace_ENTER_x_Task_Get_Tick_شمېر()
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_Task_Get_Tick_شمېر
    #تعریف_کړه trace_RETURN_x_Task_Get_Tick_شمېر( xTicks )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_Task_Get_Tick_شمېر_From_ISR
    #تعریف_کړه trace_ENTER_x_Task_Get_Tick_شمېر_From_ISR()
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_Task_Get_Tick_شمېر_From_ISR
    #تعریف_کړه trace_RETURN_x_Task_Get_Tick_شمېر_From_ISR( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_ux_Task_Get_شمېره_د_Tasks
    #تعریف_کړه trace_ENTER_ux_Task_Get_شمېره_د_Tasks()
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_ux_Task_Get_شمېره_د_Tasks
    #تعریف_کړه trace_RETURN_ux_Task_Get_شمېره_د_Tasks( ux_Current_شمېره_د_Tasks )
#د_که_پای

#که_نه_تعریف_شوی traceENTER_pcTaskGetName
    #تعریف_کړه traceENTER_pcTaskGetName( x_Task_منزل_Query )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_pcTaskGetName
    #تعریف_کړه traceRETURN_pcTaskGetName( pcTaskName )
#د_که_پای

#که_نه_تعریف_شوی traceENTER_xTaskGetHandle
    #تعریف_کړه traceENTER_xTaskGetHandle( pc_Name_منزل_Query )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_xTaskGetHandle
    #تعریف_کړه traceRETURN_xTaskGetHandle( pxTCB )
#د_که_پای

#که_نه_تعریف_شوی traceENTER_xTaskGetStaticBuffers
    #تعریف_کړه traceENTER_xTaskGetStaticBuffers( xTask, ppux_ډېری_بفر, ppx_Task_بفر )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_xTaskGetStaticBuffers
    #تعریف_کړه traceRETURN_xTaskGetStaticBuffers( xReturn )
#د_که_پای

#که_نه_تعریف_شوی traceENTER_uxTaskGetSystemState
    #تعریف_کړه traceENTER_uxTaskGetSystemState( px_Task_حالت_Array, ux_Array_اندازه, pul_ټول_پرلپسې_وخت )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_uxTaskGetSystemState
    #تعریف_کړه traceRETURN_uxTaskGetSystemState( uxTask )
#د_که_پای

#إذا ( config_شمېره_د_CORES == 1 )
    #که_نه_تعریف_شوی traceENTER_xTaskGetIdleTaskHandle
        #تعریف_کړه traceENTER_xTaskGetIdleTaskHandle()
    #د_که_پای
#د_که_پای

#إذا ( config_شمېره_د_CORES == 1 )
    #که_نه_تعریف_شوی traceRETURN_xTaskGetIdleTaskHandle
        #تعریف_کړه traceRETURN_xTaskGetIdleTaskHandle( xIdleTaskHandle )
    #د_که_پای
#د_که_پای

#که_نه_تعریف_شوی traceENTER_xTaskGetIdleTaskHandleForCore
    #تعریف_کړه traceENTER_xTaskGetIdleTaskHandleForCore( xCoreID )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_xTaskGetIdleTaskHandleForCore
    #تعریف_کړه traceRETURN_xTaskGetIdleTaskHandleForCore( xIdleTaskHandle )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_v_Task_ګام_Tick
    #تعریف_کړه trace_ENTER_v_Task_ګام_Tick( x_Ticks_منزل_Jump )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_v_Task_ګام_Tick
    #تعریف_کړه trace_RETURN_v_Task_ګام_Tick()
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_Task_Catch_پورته_Ticks
    #تعریف_کړه trace_ENTER_x_Task_Catch_پورته_Ticks( x_Ticks_منزل_Catch_پورته )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_Task_Catch_پورته_Ticks
    #تعریف_کړه trace_RETURN_x_Task_Catch_پورته_Ticks( xYieldOccurred )
#د_که_پای

#که_نه_تعریف_شوی traceENTER_xTaskAbortDelay
    #تعریف_کړه traceENTER_xTaskAbortDelay( xTask )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_xTaskAbortDelay
    #تعریف_کړه traceRETURN_xTaskAbortDelay( xReturn )
#د_که_پای

#که_نه_تعریف_شوی traceENTER_xTaskIncrementTick
    #تعریف_کړه traceENTER_xTaskIncrementTick()
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_xTaskIncrementTick
    #تعریف_کړه traceRETURN_xTaskIncrementTick( xSwitchRequired )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_v_Task_سېټ_Application_Task_نښان
    #تعریف_کړه trace_ENTER_v_Task_سېټ_Application_Task_نښان( xTask, pxHookFunction )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_v_Task_سېټ_Application_Task_نښان
    #تعریف_کړه trace_RETURN_v_Task_سېټ_Application_Task_نښان()
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_Task_Get_Application_Task_نښان
    #تعریف_کړه trace_ENTER_x_Task_Get_Application_Task_نښان( xTask )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_Task_Get_Application_Task_نښان
    #تعریف_کړه trace_RETURN_x_Task_Get_Application_Task_نښان( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_Task_Get_Application_Task_نښان_From_ISR
    #تعریف_کړه trace_ENTER_x_Task_Get_Application_Task_نښان_From_ISR( xTask )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_Task_Get_Application_Task_نښان_From_ISR
    #تعریف_کړه trace_RETURN_x_Task_Get_Application_Task_نښان_From_ISR( xReturn )
#د_که_پای

#که_نه_تعریف_شوی traceENTER_xTaskCallApplicationTaskHook
    #تعریف_کړه traceENTER_xTaskCallApplicationTaskHook( xTask, pvParameter )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_xTaskCallApplicationTaskHook
    #تعریف_کړه traceRETURN_xTaskCallApplicationTaskHook( xReturn )
#د_که_پای

#که_نه_تعریف_شوی traceENTER_vTaskSwitchContext
    #تعریف_کړه traceENTER_vTaskSwitchContext()
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_vTaskSwitchContext
    #تعریف_کړه traceRETURN_vTaskSwitchContext()
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_v_Task_ځای_پرځای_کړه_On_Event_List
    #تعریف_کړه trace_ENTER_v_Task_ځای_پرځای_کړه_On_Event_List( pxEventList, x_Ticks_منزل_Wait )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_v_Task_ځای_پرځای_کړه_On_Event_List
    #تعریف_کړه trace_RETURN_v_Task_ځای_پرځای_کړه_On_Event_List()
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_v_Task_ځای_پرځای_کړه_On_Unordered_Event_List
    #تعریف_کړه trace_ENTER_v_Task_ځای_پرځای_کړه_On_Unordered_Event_List( pxEventList, x_توکی_ارزښت, x_Ticks_منزل_Wait )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_v_Task_ځای_پرځای_کړه_On_Unordered_Event_List
    #تعریف_کړه trace_RETURN_v_Task_ځای_پرځای_کړه_On_Unordered_Event_List()
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_v_Task_ځای_پرځای_کړه_On_Event_List_Restricted
    #تعریف_کړه trace_ENTER_v_Task_ځای_پرځای_کړه_On_Event_List_Restricted( pxEventList, x_Ticks_منزل_Wait, xWaitIndefinitely )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_v_Task_ځای_پرځای_کړه_On_Event_List_Restricted
    #تعریف_کړه trace_RETURN_v_Task_ځای_پرځای_کړه_On_Event_List_Restricted()
#د_که_پای

#که_نه_تعریف_شوی traceENTER_xTaskRemoveFromEventList
    #تعریف_کړه traceENTER_xTaskRemoveFromEventList( pxEventList )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_xTaskRemoveFromEventList
    #تعریف_کړه traceRETURN_xTaskRemoveFromEventList( xReturn )
#د_که_پای

#که_نه_تعریف_شوی traceENTER_vTaskRemoveFromUnorderedEventList
    #تعریف_کړه traceENTER_vTaskRemoveFromUnorderedEventList( px_Event_List_توکی, x_توکی_ارزښت )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_vTaskRemoveFromUnorderedEventList
    #تعریف_کړه traceRETURN_vTaskRemoveFromUnorderedEventList()
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_v_Task_سېټ_وخت_بهر_State
    #تعریف_کړه trace_ENTER_v_Task_سېټ_وخت_بهر_State( px_وخت_بهر )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_v_Task_سېټ_وخت_بهر_State
    #تعریف_کړه trace_RETURN_v_Task_سېټ_وخت_بهر_State()
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_v_Task_دننی_سېټ_وخت_بهر_State
    #تعریف_کړه trace_ENTER_v_Task_دننی_سېټ_وخت_بهر_State( px_وخت_بهر )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_v_Task_دننی_سېټ_وخت_بهر_State
    #تعریف_کړه trace_RETURN_v_Task_دننی_سېټ_وخت_بهر_State()
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_Task_Check_For_وخت_بهر
    #تعریف_کړه trace_ENTER_x_Task_Check_For_وخت_بهر( px_وخت_بهر, px_Ticks_منزل_Wait )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_Task_Check_For_وخت_بهر
    #تعریف_کړه trace_RETURN_x_Task_Check_For_وخت_بهر( xReturn )
#د_که_پای

#که_نه_تعریف_شوی traceENTER_vTaskMissedYield
    #تعریف_کړه traceENTER_vTaskMissedYield()
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_vTaskMissedYield
    #تعریف_کړه traceRETURN_vTaskMissedYield()
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_ux_Task_Get_Task_شمېره
    #تعریف_کړه trace_ENTER_ux_Task_Get_Task_شمېره( xTask )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_ux_Task_Get_Task_شمېره
    #تعریف_کړه trace_RETURN_ux_Task_Get_Task_شمېره( uxReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_v_Task_سېټ_Task_شمېره
    #تعریف_کړه trace_ENTER_v_Task_سېټ_Task_شمېره( xTask, uxHandle )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_v_Task_سېټ_Task_شمېره
    #تعریف_کړه trace_RETURN_v_Task_سېټ_Task_شمېره()
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_e_Task_Confirm_Sleep_ډېرپېښ_حالت
    #تعریف_کړه trace_ENTER_e_Task_Confirm_Sleep_ډېرپېښ_حالت()
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_e_Task_Confirm_Sleep_ډېرپېښ_حالت
    #تعریف_کړه trace_RETURN_e_Task_Confirm_Sleep_ډېرپېښ_حالت( eReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_v_Task_سېټ_Thread_Local_Storage_اشاره_ګر
    #تعریف_کړه trace_ENTER_v_Task_سېټ_Thread_Local_Storage_اشاره_ګر( x_Task_منزل_سېټ, x_شاخص, pv_ارزښت )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_v_Task_سېټ_Thread_Local_Storage_اشاره_ګر
    #تعریف_کړه trace_RETURN_v_Task_سېټ_Thread_Local_Storage_اشاره_ګر()
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_pv_Task_Get_Thread_Local_Storage_اشاره_ګر
    #تعریف_کړه trace_ENTER_pv_Task_Get_Thread_Local_Storage_اشاره_ګر( x_Task_منزل_Query, x_شاخص )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_pv_Task_Get_Thread_Local_Storage_اشاره_ګر
    #تعریف_کړه trace_RETURN_pv_Task_Get_Thread_Local_Storage_اشاره_ګر( pvReturn )
#د_که_پای

#که_نه_تعریف_شوی traceENTER_vTaskAllocateMPURegions
    #تعریف_کړه traceENTER_vTaskAllocateMPURegions( x_Task_منزل_Modify, pxRegions )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_vTaskAllocateMPURegions
    #تعریف_کړه traceRETURN_vTaskAllocateMPURegions()
#د_که_پای

#که_نه_تعریف_شوی traceENTER_vTaskGetInfo
    #تعریف_کړه traceENTER_vTaskGetInfo( xTask, px_Task_حالت, x_Get_Free_ډېری_Space, eState )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_vTaskGetInfo
    #تعریف_کړه traceRETURN_vTaskGetInfo()
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_ux_Task_Get_ډېری_لوړ_Water_Mark2
    #تعریف_کړه trace_ENTER_ux_Task_Get_ډېری_لوړ_Water_Mark2( xTask )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_ux_Task_Get_ډېری_لوړ_Water_Mark2
    #تعریف_کړه trace_RETURN_ux_Task_Get_ډېری_لوړ_Water_Mark2( uxReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_ux_Task_Get_ډېری_لوړ_Water_Mark
    #تعریف_کړه trace_ENTER_ux_Task_Get_ډېری_لوړ_Water_Mark( xTask )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_ux_Task_Get_ډېری_لوړ_Water_Mark
    #تعریف_کړه trace_RETURN_ux_Task_Get_ډېری_لوړ_Water_Mark( uxReturn )
#د_که_پای

#که_نه_تعریف_شوی traceENTER_xTaskGetCurrentTaskHandle
    #تعریف_کړه traceENTER_xTaskGetCurrentTaskHandle()
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_xTaskGetCurrentTaskHandle
    #تعریف_کړه traceRETURN_xTaskGetCurrentTaskHandle( xReturn )
#د_که_پای

#که_نه_تعریف_شوی traceENTER_xTaskGetCurrentTaskHandleForCore
    #تعریف_کړه traceENTER_xTaskGetCurrentTaskHandleForCore( xCoreID )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_xTaskGetCurrentTaskHandleForCore
    #تعریف_کړه traceRETURN_xTaskGetCurrentTaskHandleForCore( xReturn )
#د_که_پای

#که_نه_تعریف_شوی traceENTER_xTaskGetSchedulerState
    #تعریف_کړه traceENTER_xTaskGetSchedulerState()
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_xTaskGetSchedulerState
    #تعریف_کړه traceRETURN_xTaskGetSchedulerState( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_Task_لومړیتوب_Inherit
    #تعریف_کړه trace_ENTER_x_Task_لومړیتوب_Inherit( pxMutexHolder )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_Task_لومړیتوب_Inherit
    #تعریف_کړه trace_RETURN_x_Task_لومړیتوب_Inherit( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_Task_لومړیتوب_Disinherit
    #تعریف_کړه trace_ENTER_x_Task_لومړیتوب_Disinherit( pxMutexHolder )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_Task_لومړیتوب_Disinherit
    #تعریف_کړه trace_RETURN_x_Task_لومړیتوب_Disinherit( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_v_Task_لومړیتوب_Disinherit_After_Timeout
    #تعریف_کړه trace_ENTER_v_Task_لومړیتوب_Disinherit_After_Timeout( pxMutexHolder, ux_Highest_لومړیتوب_انتظار_Task )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_v_Task_لومړیتوب_Disinherit_After_Timeout
    #تعریف_کړه trace_RETURN_v_Task_لومړیتوب_Disinherit_After_Timeout()
#د_که_پای

#که_نه_تعریف_شوی traceENTER_vTaskYieldWithinAPI
    #تعریف_کړه traceENTER_vTaskYieldWithinAPI()
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_vTaskYieldWithinAPI
    #تعریف_کړه traceRETURN_vTaskYieldWithinAPI()
#د_که_پای

#که_نه_تعریف_شوی traceENTER_vTaskEnterCritical
    #تعریف_کړه traceENTER_vTaskEnterCritical()
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_vTaskEnterCritical
    #تعریف_کړه traceRETURN_vTaskEnterCritical()
#د_که_پای

#که_نه_تعریف_شوی traceENTER_vTaskEnterCriticalFromISR
    #تعریف_کړه traceENTER_vTaskEnterCriticalFromISR()
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_vTaskEnterCriticalFromISR
    #تعریف_کړه traceRETURN_vTaskEnterCriticalFromISR( ux_Saved_مداخله_حالت )
#د_که_پای

#که_نه_تعریف_شوی traceENTER_vTaskExitCritical
    #تعریف_کړه traceENTER_vTaskExitCritical()
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_vTaskExitCritical
    #تعریف_کړه traceRETURN_vTaskExitCritical()
#د_که_پای

#که_نه_تعریف_شوی traceENTER_vTaskExitCriticalFromISR
    #تعریف_کړه traceENTER_vTaskExitCriticalFromISR( ux_Saved_مداخله_حالت )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_vTaskExitCriticalFromISR
    #تعریف_کړه traceRETURN_vTaskExitCriticalFromISR()
#د_که_پای

#که_نه_تعریف_شوی traceENTER_vTaskListTasks
    #تعریف_کړه traceENTER_vTaskListTasks( pc_Write_بفر, ux_بفر_اوږدتیا )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_vTaskListTasks
    #تعریف_کړه traceRETURN_vTaskListTasks()
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_v_Task_Get_پرلپسې_وخت_Statistics
    #تعریف_کړه trace_ENTER_v_Task_Get_پرلپسې_وخت_Statistics( pc_Write_بفر, ux_بفر_اوږدتیا )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_v_Task_Get_پرلپسې_وخت_Statistics
    #تعریف_کړه trace_RETURN_v_Task_Get_پرلپسې_وخت_Statistics()
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_ux_Task_Reset_Event_توکی_ارزښت
    #تعریف_کړه trace_ENTER_ux_Task_Reset_Event_توکی_ارزښت()
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_ux_Task_Reset_Event_توکی_ارزښت
    #تعریف_کړه trace_RETURN_ux_Task_Reset_Event_توکی_ارزښت( uxReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_pv_Task_Increment_Mutex_Held_شمېر
    #تعریف_کړه trace_ENTER_pv_Task_Increment_Mutex_Held_شمېر()
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_pv_Task_Increment_Mutex_Held_شمېر
    #تعریف_کړه trace_RETURN_pv_Task_Increment_Mutex_Held_شمېر( pxTCB )
#د_که_پای

#که_نه_تعریف_شوی traceENTER_ulTaskGenericNotifyTake
    #تعریف_کړه traceENTER_ulTaskGenericNotifyTake( ux_شاخص_منزل_Wait_On, x_Clear_شمېر_On_Exit, x_Ticks_منزل_Wait )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_ulTaskGenericNotifyTake
    #تعریف_کړه traceRETURN_ulTaskGenericNotifyTake( ulReturn )
#د_که_پای

#که_نه_تعریف_شوی traceENTER_xTaskGenericNotifyWait
    #تعریف_کړه traceENTER_xTaskGenericNotifyWait( ux_شاخص_منزل_Wait_On, ul_بټونه_منزل_Clear_On_ثبت, ul_بټونه_منزل_Clear_On_Exit, pul_Notification_ارزښت, x_Ticks_منزل_Wait )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_xTaskGenericNotifyWait
    #تعریف_کړه traceRETURN_xTaskGenericNotifyWait( xReturn )
#د_که_پای

#که_نه_تعریف_شوی traceENTER_xTaskGenericNotify
    #تعریف_کړه traceENTER_xTaskGenericNotify( x_Task_منزل_Notify, ux_شاخص_منزل_Notify, ul_ارزښت, eAction, pul_پخوانی_Notification_ارزښت )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_xTaskGenericNotify
    #تعریف_کړه traceRETURN_xTaskGenericNotify( xReturn )
#د_که_پای

#که_نه_تعریف_شوی traceENTER_xTaskGenericNotifyFromISR
    #تعریف_کړه traceENTER_xTaskGenericNotifyFromISR( x_Task_منزل_Notify, ux_شاخص_منزل_Notify, ul_ارزښت, eAction, pul_پخوانی_Notification_ارزښت, px_Higher_لومړیتوب_Task_Woken )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_xTaskGenericNotifyFromISR
    #تعریف_کړه traceRETURN_xTaskGenericNotifyFromISR( xReturn )
#د_که_پای

#که_نه_تعریف_شوی traceENTER_vTaskGenericNotifyGiveFromISR
    #تعریف_کړه traceENTER_vTaskGenericNotifyGiveFromISR( x_Task_منزل_Notify, ux_شاخص_منزل_Notify, px_Higher_لومړیتوب_Task_Woken )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_vTaskGenericNotifyGiveFromISR
    #تعریف_کړه traceRETURN_vTaskGenericNotifyGiveFromISR()
#د_که_پای

#که_نه_تعریف_شوی traceENTER_xTaskGenericNotifyStateClear
    #تعریف_کړه traceENTER_xTaskGenericNotifyStateClear( xTask, ux_شاخص_منزل_Clear )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_xTaskGenericNotifyStateClear
    #تعریف_کړه traceRETURN_xTaskGenericNotifyStateClear( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_ul_Task_Generic_Notify_ارزښت_Clear
    #تعریف_کړه trace_ENTER_ul_Task_Generic_Notify_ارزښت_Clear( xTask, ux_شاخص_منزل_Clear, ul_بټونه_منزل_Clear )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_ul_Task_Generic_Notify_ارزښت_Clear
    #تعریف_کړه trace_RETURN_ul_Task_Generic_Notify_ارزښت_Clear( ulReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_ul_Task_Get_پرلپسې_وخت_Counter
    #تعریف_کړه trace_ENTER_ul_Task_Get_پرلپسې_وخت_Counter( xTask )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_ul_Task_Get_پرلپسې_وخت_Counter
    #تعریف_کړه trace_RETURN_ul_Task_Get_پرلپسې_وخت_Counter( ul_پرلپسې_وخت_Counter )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_ul_Task_Get_پرلپسې_وخت_Percent
    #تعریف_کړه trace_ENTER_ul_Task_Get_پرلپسې_وخت_Percent( xTask )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_ul_Task_Get_پرلپسې_وخت_Percent
    #تعریف_کړه trace_RETURN_ul_Task_Get_پرلپسې_وخت_Percent( ulReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_ul_Task_Get_Idle_پرلپسې_وخت_Counter
    #تعریف_کړه trace_ENTER_ul_Task_Get_Idle_پرلپسې_وخت_Counter()
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_ul_Task_Get_Idle_پرلپسې_وخت_Counter
    #تعریف_کړه trace_RETURN_ul_Task_Get_Idle_پرلپسې_وخت_Counter( ulReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_ul_Task_Get_Idle_پرلپسې_وخت_Percent
    #تعریف_کړه trace_ENTER_ul_Task_Get_Idle_پرلپسې_وخت_Percent()
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_ul_Task_Get_Idle_پرلپسې_وخت_Percent
    #تعریف_کړه trace_RETURN_ul_Task_Get_Idle_پرلپسې_وخت_Percent( ulReturn )
#د_که_پای

#که_نه_تعریف_شوی traceENTER_xTaskGetMPUSettings
    #تعریف_کړه traceENTER_xTaskGetMPUSettings( xTask )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_xTaskGetMPUSettings
    #تعریف_کړه traceRETURN_xTaskGetMPUSettings( xMPUSettings )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_بهیر_بفر_Generic_Create
    #تعریف_کړه trace_ENTER_x_بهیر_بفر_Generic_Create( x_بفر_اندازه_بایټونه, x_Trigger_Level_بایټونه, x_بهیر_بفر_Type, pxSendCompletedCallback, pxReceiveCompletedCallback )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_بهیر_بفر_Generic_Create
    #تعریف_کړه trace_RETURN_x_بهیر_بفر_Generic_Create( pv_Allocated_حافظه )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_بهیر_بفر_Generic_Create_Static
    #تعریف_کړه trace_ENTER_x_بهیر_بفر_Generic_Create_Static( x_بفر_اندازه_بایټونه, x_Trigger_Level_بایټونه, x_بهیر_بفر_Type, puc_بهیر_بفر_Storage_Area, px_Static_بهیر_بفر, pxSendCompletedCallback, pxReceiveCompletedCallback )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_بهیر_بفر_Generic_Create_Static
    #تعریف_کړه trace_RETURN_x_بهیر_بفر_Generic_Create_Static( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_بهیر_بفر_Get_Static_Buffers
    #تعریف_کړه trace_ENTER_x_بهیر_بفر_Get_Static_Buffers( x_بهیر_بفر, ppuc_بهیر_بفر_Storage_Area, ppx_Static_بهیر_بفر )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_بهیر_بفر_Get_Static_Buffers
    #تعریف_کړه trace_RETURN_x_بهیر_بفر_Get_Static_Buffers( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_v_بهیر_بفر_Delete
    #تعریف_کړه trace_ENTER_v_بهیر_بفر_Delete( x_بهیر_بفر )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_v_بهیر_بفر_Delete
    #تعریف_کړه trace_RETURN_v_بهیر_بفر_Delete()
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_بهیر_بفر_Reset
    #تعریف_کړه trace_ENTER_x_بهیر_بفر_Reset( x_بهیر_بفر )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_بهیر_بفر_Reset
    #تعریف_کړه trace_RETURN_x_بهیر_بفر_Reset( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_بهیر_بفر_Reset_From_ISR
    #تعریف_کړه trace_ENTER_x_بهیر_بفر_Reset_From_ISR( x_بهیر_بفر )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_بهیر_بفر_Reset_From_ISR
    #تعریف_کړه trace_RETURN_x_بهیر_بفر_Reset_From_ISR( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_بهیر_بفر_سېټ_Trigger_Level
    #تعریف_کړه trace_ENTER_x_بهیر_بفر_سېټ_Trigger_Level( x_بهیر_بفر, xTriggerLevel )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_بهیر_بفر_سېټ_Trigger_Level
    #تعریف_کړه trace_RETURN_x_بهیر_بفر_سېټ_Trigger_Level( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_بهیر_بفر_Spaces_شته
    #تعریف_کړه trace_ENTER_x_بهیر_بفر_Spaces_شته( x_بهیر_بفر )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_بهیر_بفر_Spaces_شته
    #تعریف_کړه trace_RETURN_x_بهیر_بفر_Spaces_شته( xSpace )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_بهیر_بفر_بایټونه_شته
    #تعریف_کړه trace_ENTER_x_بهیر_بفر_بایټونه_شته( x_بهیر_بفر )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_بهیر_بفر_بایټونه_شته
    #تعریف_کړه trace_RETURN_x_بهیر_بفر_بایټونه_شته( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_بهیر_بفر_Send
    #تعریف_کړه trace_ENTER_x_بهیر_بفر_Send( x_بهیر_بفر, pv_Tx_معلومات, x_معلومات_اوږدتیا_بایټونه, x_Ticks_منزل_Wait )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_بهیر_بفر_Send
    #تعریف_کړه trace_RETURN_x_بهیر_بفر_Send( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_بهیر_بفر_Send_From_ISR
    #تعریف_کړه trace_ENTER_x_بهیر_بفر_Send_From_ISR( x_بهیر_بفر, pv_Tx_معلومات, x_معلومات_اوږدتیا_بایټونه, px_Higher_لومړیتوب_Task_Woken )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_بهیر_بفر_Send_From_ISR
    #تعریف_کړه trace_RETURN_x_بهیر_بفر_Send_From_ISR( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_بهیر_بفر_Receive
    #تعریف_کړه trace_ENTER_x_بهیر_بفر_Receive( x_بهیر_بفر, pv_Rx_معلومات, x_بفر_اوږدتیا_بایټونه, x_Ticks_منزل_Wait )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_بهیر_بفر_Receive
    #تعریف_کړه trace_RETURN_x_بهیر_بفر_Receive( x_Received_اوږدتیا )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_بهیر_بفر_بل_پیغام_اوږدتیا_بایټونه
    #تعریف_کړه trace_ENTER_x_بهیر_بفر_بل_پیغام_اوږدتیا_بایټونه( x_بهیر_بفر )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_بهیر_بفر_بل_پیغام_اوږدتیا_بایټونه
    #تعریف_کړه trace_RETURN_x_بهیر_بفر_بل_پیغام_اوږدتیا_بایټونه( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_بهیر_بفر_Receive_From_ISR
    #تعریف_کړه trace_ENTER_x_بهیر_بفر_Receive_From_ISR( x_بهیر_بفر, pv_Rx_معلومات, x_بفر_اوږدتیا_بایټونه, px_Higher_لومړیتوب_Task_Woken )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_بهیر_بفر_Receive_From_ISR
    #تعریف_کړه trace_RETURN_x_بهیر_بفر_Receive_From_ISR( x_Received_اوږدتیا )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_بهیر_بفر_Is_تشه
    #تعریف_کړه trace_ENTER_x_بهیر_بفر_Is_تشه( x_بهیر_بفر )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_بهیر_بفر_Is_تشه
    #تعریف_کړه trace_RETURN_x_بهیر_بفر_Is_تشه( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_بهیر_بفر_Is_ډک
    #تعریف_کړه trace_ENTER_x_بهیر_بفر_Is_ډک( x_بهیر_بفر )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_بهیر_بفر_Is_ډک
    #تعریف_کړه trace_RETURN_x_بهیر_بفر_Is_ډک( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_بهیر_بفر_Send_Completed_From_ISR
    #تعریف_کړه trace_ENTER_x_بهیر_بفر_Send_Completed_From_ISR( x_بهیر_بفر, px_Higher_لومړیتوب_Task_Woken )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_بهیر_بفر_Send_Completed_From_ISR
    #تعریف_کړه trace_RETURN_x_بهیر_بفر_Send_Completed_From_ISR( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_x_بهیر_بفر_Receive_Completed_From_ISR
    #تعریف_کړه trace_ENTER_x_بهیر_بفر_Receive_Completed_From_ISR( x_بهیر_بفر, px_Higher_لومړیتوب_Task_Woken )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_x_بهیر_بفر_Receive_Completed_From_ISR
    #تعریف_کړه trace_RETURN_x_بهیر_بفر_Receive_Completed_From_ISR( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_ux_بهیر_بفر_Get_بهیر_بفر_Notification_شاخص
    #تعریف_کړه trace_ENTER_ux_بهیر_بفر_Get_بهیر_بفر_Notification_شاخص( x_بهیر_بفر )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_ux_بهیر_بفر_Get_بهیر_بفر_Notification_شاخص
    #تعریف_کړه trace_RETURN_ux_بهیر_بفر_Get_بهیر_بفر_Notification_شاخص( ux_Notification_شاخص )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_v_بهیر_بفر_سېټ_بهیر_بفر_Notification_شاخص
    #تعریف_کړه trace_ENTER_v_بهیر_بفر_سېټ_بهیر_بفر_Notification_شاخص( x_بهیر_بفر, ux_Notification_شاخص )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_v_بهیر_بفر_سېټ_بهیر_بفر_Notification_شاخص
    #تعریف_کړه trace_RETURN_v_بهیر_بفر_سېټ_بهیر_بفر_Notification_شاخص()
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_ux_بهیر_بفر_Get_بهیر_بفر_شمېره
    #تعریف_کړه trace_ENTER_ux_بهیر_بفر_Get_بهیر_بفر_شمېره( x_بهیر_بفر )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_ux_بهیر_بفر_Get_بهیر_بفر_شمېره
    #تعریف_کړه trace_RETURN_ux_بهیر_بفر_Get_بهیر_بفر_شمېره( ux_بهیر_بفر_شمېره )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_v_بهیر_بفر_سېټ_بهیر_بفر_شمېره
    #تعریف_کړه trace_ENTER_v_بهیر_بفر_سېټ_بهیر_بفر_شمېره( x_بهیر_بفر, ux_بهیر_بفر_شمېره )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_v_بهیر_بفر_سېټ_بهیر_بفر_شمېره
    #تعریف_کړه trace_RETURN_v_بهیر_بفر_سېټ_بهیر_بفر_شمېره()
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_uc_بهیر_بفر_Get_بهیر_بفر_Type
    #تعریف_کړه trace_ENTER_uc_بهیر_بفر_Get_بهیر_بفر_Type( x_بهیر_بفر )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_uc_بهیر_بفر_Get_بهیر_بفر_Type
    #تعریف_کړه trace_RETURN_uc_بهیر_بفر_Get_بهیر_بفر_Type( uc_بهیر_بفر_Type )
#د_که_پای

#که_نه_تعریف_شوی traceENTER_vListInitialise
    #تعریف_کړه traceENTER_vListInitialise( pxList )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_vListInitialise
    #تعریف_کړه traceRETURN_vListInitialise()
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_v_List_Initialise_توکی
    #تعریف_کړه trace_ENTER_v_List_Initialise_توکی( px_توکی )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_v_List_Initialise_توکی
    #تعریف_کړه trace_RETURN_v_List_Initialise_توکی()
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_v_List_ورننباسه_پای_ټکی
    #تعریف_کړه trace_ENTER_v_List_ورننباسه_پای_ټکی( pxList, px_New_List_توکی )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_v_List_ورننباسه_پای_ټکی
    #تعریف_کړه trace_RETURN_v_List_ورننباسه_پای_ټکی()
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_v_List_ورننباسه
    #تعریف_کړه trace_ENTER_v_List_ورننباسه( pxList, px_New_List_توکی )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_v_List_ورننباسه
    #تعریف_کړه trace_RETURN_v_List_ورننباسه()
#د_که_پای

#که_نه_تعریف_شوی traceENTER_uxListRemove
    #تعریف_کړه traceENTER_uxListRemove( px_توکی_منزل_Remove )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_uxListRemove
    #تعریف_کړه traceRETURN_uxListRemove( ux_شمېره_د_توکي )
#د_که_پای

#که_نه_تعریف_شوی traceENTER_xCoRoutineCreate
    #تعریف_کړه traceENTER_xCoRoutineCreate( px_Co_Routine_کوډ, ux_لومړیتوب, ux_شاخص )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_xCoRoutineCreate
    #تعریف_کړه traceRETURN_xCoRoutineCreate( xReturn )
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_v_Co_Routine_جمع_کړه_منزل_Delayed_List
    #تعریف_کړه trace_ENTER_v_Co_Routine_جمع_کړه_منزل_Delayed_List( x_Ticks_منزل_Delay, pxEventList )
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_v_Co_Routine_جمع_کړه_منزل_Delayed_List
    #تعریف_کړه trace_RETURN_v_Co_Routine_جمع_کړه_منزل_Delayed_List()
#د_که_پای

#که_نه_تعریف_شوی trace_ENTER_v_Co_Routine_مهالوېش
    #تعریف_کړه trace_ENTER_v_Co_Routine_مهالوېش()
#د_که_پای

#که_نه_تعریف_شوی trace_RETURN_v_Co_Routine_مهالوېش
    #تعریف_کړه trace_RETURN_v_Co_Routine_مهالوېش()
#د_که_پای

#که_نه_تعریف_شوی traceENTER_xCoRoutineRemoveFromEventList
    #تعریف_کړه traceENTER_xCoRoutineRemoveFromEventList( pxEventList )
#د_که_پای

#که_نه_تعریف_شوی traceRETURN_xCoRoutineRemoveFromEventList
    #تعریف_کړه traceRETURN_xCoRoutineRemoveFromEventList( xReturn )
#د_که_پای

#که_نه_تعریف_شوی config_GENERATE_پرلپسې_وخت_STATS
    #تعریف_کړه config_GENERATE_پرلپسې_وخت_STATS    0
#د_که_پای

#إذا ( config_GENERATE_پرلپسې_وخت_STATS == 1 )

    #که_نه_تعریف_شوی پورټ_CONFIGURE_TIMER_FOR_پرلپسې_وخت_STATS
        #تېروتنه If config_GENERATE_پرلپسې_وخت_STATS is defined then پورټ_CONFIGURE_TIMER_FOR_پرلپسې_وخت_STATS must also be defined.  پورټ_CONFIGURE_TIMER_FOR_پرلپسې_وخت_STATS should call a پورټ layer function منزل تیاری a peripheral timer/counter that can then be کارول_شوی as the پرلپسې time counter time بنسټ.
    #د_که_پای /* portCONFIGURE_TIMER_FOR_RUN_TIME_STATS */

    #که_نه_تعریف_شوی پورټ_GET_پرلپسې_وخت_COUNTER_ارزښت
        #که_نه_تعریف_شوی پورټ_ALT_GET_پرلپسې_وخت_COUNTER_ارزښت
            #تېروتنه If config_GENERATE_پرلپسې_وخت_STATS is defined then either پورټ_GET_پرلپسې_وخت_COUNTER_ارزښت or پورټ_ALT_GET_پرلپسې_وخت_COUNTER_ارزښت must also be defined.  See the examples provided and the FreeRTOS web site لكل نور information.
        #د_که_پای /* portALT_GET_RUN_TIME_COUNTER_VALUE */
    #د_که_پای /* portGET_RUN_TIME_COUNTER_VALUE */

#د_که_پای /* configGENERATE_RUN_TIME_STATS */

#که_نه_تعریف_شوی پورټ_CONFIGURE_TIMER_FOR_پرلپسې_وخت_STATS
    #تعریف_کړه پورټ_CONFIGURE_TIMER_FOR_پرلپسې_وخت_STATS()
#د_که_پای

#که_نه_تعریف_شوی پورټ_PRIVILEGE_بټ
    #تعریف_کړه پورټ_PRIVILEGE_بټ    ( ( UBaseType_t ) 0x00 )
#د_که_پای

#که_نه_تعریف_شوی پورټ_YIELD_WITHIN_API
    #تعریف_کړه پورټ_YIELD_WITHIN_API    پورټ_YIELD
#د_که_پای

#که_نه_تعریف_شوی پورټ_SUPPRESS_TICKS_AND_SLEEP
    #تعریف_کړه پورټ_SUPPRESS_TICKS_AND_SLEEP( x_Expected_Idle_وخت )
#د_که_پای

#که_نه_تعریف_شوی config_EXPECTED_IDLE_وخت_BEFORE_SLEEP
    #تعریف_کړه config_EXPECTED_IDLE_وخت_BEFORE_SLEEP    2
#د_که_پای

#إذا config_EXPECTED_IDLE_وخت_BEFORE_SLEEP < 2
    #تېروتنه config_EXPECTED_IDLE_وخت_BEFORE_SLEEP must not be less than 2
#د_که_پای

#که_نه_تعریف_شوی configUSE_TICKLESS_IDLE
    #تعریف_کړه configUSE_TICKLESS_IDLE    0
#د_که_پای

#که_نه_تعریف_شوی configPRE_SUPPRESS_TICKS_AND_SLEEP_PROCESSING
    #تعریف_کړه configPRE_SUPPRESS_TICKS_AND_SLEEP_PROCESSING( x )
#د_که_پای

#که_نه_تعریف_شوی configPRE_SLEEP_PROCESSING
    #تعریف_کړه configPRE_SLEEP_PROCESSING( x )
#د_که_پای

#که_نه_تعریف_شوی configPOST_SLEEP_PROCESSING
    #تعریف_کړه configPOST_SLEEP_PROCESSING( x )
#د_که_پای

#که_نه_تعریف_شوی config_USE_کتار_SETS
    #تعریف_کړه config_USE_کتار_SETS    0
#د_که_پای

#که_نه_تعریف_شوی پورټ_TASK_کاروي_FLOATING_ټکی
    #تعریف_کړه پورټ_TASK_کاروي_FLOATING_ټکی()
#د_که_پای

#که_نه_تعریف_شوی پورټ_ALLOCATE_SECURE_CONTEXT
    #تعریف_کړه پورټ_ALLOCATE_SECURE_CONTEXT( ul_Secure_ډېری_اندازه )
#د_که_پای

#که_نه_تعریف_شوی پورټ_DONT_DISCARD
    #تعریف_کړه پورټ_DONT_DISCARD
#د_که_پای

#که_نه_تعریف_شوی config_USE_وخت_SLICING
    #تعریف_کړه config_USE_وخت_SLICING    1
#د_که_پای

#که_نه_تعریف_شوی configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS
    #تعریف_کړه configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS    0
#د_که_پای

#که_نه_تعریف_شوی configUSE_STATS_FORMATTING_FUNCTIONS
    #تعریف_کړه configUSE_STATS_FORMATTING_FUNCTIONS    0
#د_که_پای

#که_نه_تعریف_شوی پورټ_ASSERT_IF_مداخله_لومړیتوب_INVALID
    #تعریف_کړه پورټ_ASSERT_IF_مداخله_لومړیتوب_INVALID()
#د_که_پای

#که_نه_تعریف_شوی configUSE_TRACE_FACILITY
    #تعریف_کړه configUSE_TRACE_FACILITY    0
#د_که_پای

#که_نه_تعریف_شوی mtCOVERAGE_TEST_MARKER
    #تعریف_کړه mtCOVERAGE_TEST_MARKER()
#د_که_پای

#که_نه_تعریف_شوی mtCOVERAGE_TEST_DELAY
    #تعریف_کړه mtCOVERAGE_TEST_DELAY()
#د_که_پای

#که_نه_تعریف_شوی پورټ_ASSERT_IF_دننه_ISR
    #تعریف_کړه پورټ_ASSERT_IF_دننه_ISR()
#د_که_پای

#که_نه_تعریف_شوی config_USE_پورټ_OPTIMISED_TASK_SELECTION
    #تعریف_کړه config_USE_پورټ_OPTIMISED_TASK_SELECTION    0
#د_که_پای

#که_نه_تعریف_شوی config_APPLICATION_ALLOCATED_ډېرۍ
    #تعریف_کړه config_APPLICATION_ALLOCATED_ډېرۍ    0
#د_که_پای

#که_نه_تعریف_شوی config_ENABLE_ډېرۍ_PROTECTOR
    #تعریف_کړه config_ENABLE_ډېرۍ_PROTECTOR    0
#د_که_پای

#که_نه_تعریف_شوی configUSE_TASK_NOTIFICATIONS
    #تعریف_کړه configUSE_TASK_NOTIFICATIONS    1
#د_که_پای

#که_نه_تعریف_شوی configTASK_NOTIFICATION_ARRAY_ENTRIES
    #تعریف_کړه configTASK_NOTIFICATION_ARRAY_ENTRIES    1
#د_که_پای

#إذا configTASK_NOTIFICATION_ARRAY_ENTRIES < 1
    #تېروتنه configTASK_NOTIFICATION_ARRAY_ENTRIES must be at لږترلږه 1
#د_که_پای

#که_نه_تعریف_شوی configUSE_POSIX_ERRNO
    #تعریف_کړه configUSE_POSIX_ERRNO    0
#د_که_پای

#که_نه_تعریف_شوی configUSE_SB_COMPLETED_CALLBACK

/* By default per-instance callbacks are not enabled for stream buffer or message buffer. */
    #تعریف_کړه configUSE_SB_COMPLETED_CALLBACK    0
#د_که_پای

#که_نه_تعریف_شوی پورټ_TICK_TYPE_IS_ATOMIC
    #تعریف_کړه پورټ_TICK_TYPE_IS_ATOMIC    0
#د_که_پای

#که_نه_تعریف_شوی config_SUPPORT_STATIC_ویش
    /* Defaults to 0 for backward compatibility. */
    #تعریف_کړه config_SUPPORT_STATIC_ویش    0
#د_که_پای

#که_نه_تعریف_شوی config_KERNEL_PROVIDED_STATIC_حافظه
    #تعریف_کړه config_KERNEL_PROVIDED_STATIC_حافظه    0
#د_که_پای

#که_نه_تعریف_شوی config_SUPPORT_DYNAMIC_ویش
    /* Defaults to 1 for backward compatibility. */
    #تعریف_کړه config_SUPPORT_DYNAMIC_ویش    1
#د_که_پای

#إذا ( ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 ) && ( config_SUPPORT_DYNAMIC_ویش != 1 ) )
    #تېروتنه configUSE_STATS_FORMATTING_FUNCTIONS cannot be کارول_شوی without dynamic ویش, but config_SUPPORT_DYNAMIC_ویش is not سېټ منزل 1.
#د_که_پای

#إذا ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 )
    #إذا ( ( configUSE_TRACE_FACILITY != 1 ) && ( config_GENERATE_پرلپسې_وخت_STATS != 1 ) )
        #تېروتنه configUSE_STATS_FORMATTING_FUNCTIONS is 1 but the functions it enables are not کارول_شوی because neither configUSE_TRACE_FACILITY or config_GENERATE_پرلپسې_وخت_STATS are 1.  سېټ_2 configUSE_STATS_FORMATTING_FUNCTIONS منزل 0 دننه FreeRTOSConfig.h.
    #د_که_پای
#د_که_پای

#که_نه_تعریف_شوی config_STATS_بفر_MAX_اوږدتیا
    #تعریف_کړه config_STATS_بفر_MAX_اوږدتیا    0xFFFF
#د_که_پای

#که_نه_تعریف_شوی config_ډېری_ژوروالی_TYPE

/* Defaults to StackType_t for backward compatibility, but can be overridden
 * in FreeRTOSConfig.h if StackType_t is too restrictive. */
    #تعریف_کړه config_ډېری_ژوروالی_TYPE    ډېری_Type_t
#د_که_پای

#که_نه_تعریف_شوی config_پرلپسې_وخت_COUNTER_TYPE

/* Defaults to uint32_t for backward compatibility, but can be overridden in
 * FreeRTOSConfig.h if uint32_t is too restrictive. */

    #تعریف_کړه config_پرلپسې_وخت_COUNTER_TYPE    uint32_t
#د_که_پای

#که_نه_تعریف_شوی config_پیغام_بفر_اوږدتیا_TYPE

/* Defaults to size_t for backward compatibility, but can be overridden
 * in FreeRTOSConfig.h if lengths will always be less than the number of bytes
 * in a size_t. */
    #تعریف_کړه config_پیغام_بفر_اوږدتیا_TYPE    size_t
#د_که_پای

/* Sanity check the configuration. */
#إذا ( ( config_SUPPORT_STATIC_ویش == 0 ) && ( config_SUPPORT_DYNAMIC_ویش == 0 ) )
    #تېروتنه config_SUPPORT_STATIC_ویش and config_SUPPORT_DYNAMIC_ویش cannot both be 0, but can both be 1.
#د_که_پای

#إذا ( ( configUSE_RECURSIVE_MUTEXES == 1 ) && ( configUSE_MUTEXES != 1 ) )
    #تېروتنه configUSE_MUTEXES must be سېټ منزل 1 منزل use recursive mutexes
#د_که_پای

#إذا ( ( config_پرلپسې_مضرب_PRIORITIES == 0 ) && ( configUSE_TASK_PREEMPTION_DISABLE != 0 ) )
    #تېروتنه config_پرلپسې_مضرب_PRIORITIES must be سېټ منزل 1 منزل use task preemption disable
#د_که_پای

#إذا ( ( configUSE_PREEMPTION == 0 ) && ( configUSE_TASK_PREEMPTION_DISABLE != 0 ) )
    #تېروتنه configUSE_PREEMPTION must be سېټ منزل 1 منزل use task preemption disable
#د_که_پای

#إذا ( ( config_شمېره_د_CORES == 1 ) && ( configUSE_TASK_PREEMPTION_DISABLE != 0 ) )
    #تېروتنه configUSE_TASK_PREEMPTION_DISABLE is not supported دننه single core FreeRTOS
#د_که_پای

#إذا ( ( config_شمېره_د_CORES == 1 ) && ( configUSE_CORE_AFFINITY != 0 ) )
    #تېروتنه configUSE_CORE_AFFINITY is not supported دننه single core FreeRTOS
#د_که_پای

#إذا ( ( config_شمېره_د_CORES > 1 ) && ( config_USE_پورټ_OPTIMISED_TASK_SELECTION != 0 ) )
    #تېروتنه config_USE_پورټ_OPTIMISED_TASK_SELECTION is not supported دننه SMP FreeRTOS
#د_که_پای

#که_نه_تعریف_شوی config_INITIAL_TICK_شمېر
    #تعریف_کړه config_INITIAL_TICK_شمېر    0
#د_که_پای

#إذا ( پورټ_TICK_TYPE_IS_ATOMIC == 0 )

/* Either variables of tick type cannot be read atomically, or
 * portTICK_TYPE_IS_ATOMIC was not set - map the critical sections used when
 * the tick count is returned to the standard critical section macros. */
    #تعریف_کړه پورټ_TICK_TYPE_ENTER_CRITICAL()                      پورټ_ENTER_CRITICAL()
    #تعریف_کړه پورټ_TICK_TYPE_EXIT_CRITICAL()                       پورټ_EXIT_CRITICAL()
    #تعریف_کړه پورټ_TICK_TYPE_سېټ_مداخله_ماسک_FROM_ISR()         پورټ_سېټ_مداخله_ماسک_FROM_ISR()
    #تعریف_کړه پورټ_TICK_TYPE_CLEAR_مداخله_ماسک_FROM_ISR( x )    پورټ_CLEAR_مداخله_ماسک_FROM_ISR( ( x ) )
#وإلا

/* The tick type can be read atomically, so critical sections used when the
 * tick count is returned can be defined away. */
    #تعریف_کړه پورټ_TICK_TYPE_ENTER_CRITICAL()
    #تعریف_کړه پورټ_TICK_TYPE_EXIT_CRITICAL()
    #تعریف_کړه پورټ_TICK_TYPE_سېټ_مداخله_ماسک_FROM_ISR()         0
    #تعریف_کړه پورټ_TICK_TYPE_CLEAR_مداخله_ماسک_FROM_ISR( x )    ( فراغ ) ( x )
#د_که_پای /* if ( portTICK_TYPE_IS_ATOMIC == 0 ) */

/* Definitions to allow backward compatibility with FreeRTOS versions prior to
 * V8 if desired. */
#که_نه_تعریف_شوی configENABLE_BACKWARD_COMPATIBILITY
    #تعریف_کړه configENABLE_BACKWARD_COMPATIBILITY    1
#د_که_پای

#که_نه_تعریف_شوی configPRINTF

/* configPRINTF() was not defined, so define it away to nothing.  To use
 * configPRINTF() then define it as follows (where MyPrintFunction() is
 * provided by the application writer):
 *
 * void MyPrintFunction(const char *pcFormat, ... );
 #define configPRINTF( X )   MyPrintFunction X
 *
 * Then call like a standard printf() function, but placing brackets around
 * all parameters so they are passed as a single parameter.  For example:
 * configPRINTF( ("Value = %d", MyVariable) ); */
    #تعریف_کړه configPRINTF( X )
#د_که_پای

#که_نه_تعریف_شوی configMAX

/* The application writer has not provided their own MAX macro, so define
 * the following generic implementation. */
    #تعریف_کړه configMAX( a, b )    ( ( ( a ) > ( b ) ) ? ( a ) : ( b ) )
#د_که_پای

#که_نه_تعریف_شوی configMIN

/* The application writer has not provided their own MIN macro, so define
 * the following generic implementation. */
    #تعریف_کړه configMIN( a, b )    ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )
#د_که_پای

#إذا configENABLE_BACKWARD_COMPATIBILITY == 1
    #تعریف_کړه eTaskStateGet                 eTaskGetState
    #تعریف_کړه پورټ_Tick_Type                  TickType_t
    #تعریف_کړه xTaskHandle                   TaskHandle_t
    #تعریف_کړه x_کتار_Handle                  کتار_Handle_t
    #تعریف_کړه xSemaphoreHandle              SemaphoreHandle_t
    #تعریف_کړه x_کتار_سېټ_Handle               کتار_سېټ_Handle_t
    #تعریف_کړه x_کتار_سېټ_Member_Handle         کتار_سېټ_Member_Handle_t
    #تعریف_کړه x_وخت_بهر_Type                  وخت_بهر_t
    #تعریف_کړه x_حافظه_Region                 حافظه_Region_t
    #تعریف_کړه xTaskParameters               TaskParameters_t
    #تعریف_کړه x_Task_حالت_Type               Task_حالت_t
    #تعریف_کړه xTimerHandle                  TimerHandle_t
    #تعریف_کړه xCoRoutineHandle              CoRoutineHandle_t
    #تعریف_کړه pd_TASK_HOOK_کوډ              TaskHookFunction_t
    #تعریف_کړه پورټ_TICK_شرح_MS              پورټ_TICK_PERIOD_MS
    #تعریف_کړه pcTaskGetTaskName             pcTaskGetName
    #تعریف_کړه pcTimerGetTimerName           pcTimerGetName
    #تعریف_کړه pc_کتار_Get_کتار_Name           pc_کتار_Get_Name
    #تعریف_کړه vTaskGetTaskInfo              vTaskGetInfo
    #تعریف_کړه x_Task_Get_Idle_پرلپسې_وخت_Counter    ul_Task_Get_Idle_پرلپسې_وخت_Counter

/* Backward compatibility within the scheduler code only - these definitions
 * are not really required but are included for completeness. */
    #تعریف_کړه tmrTIMER_CALLBACK             TimerCallbackFunction_t
    #تعریف_کړه pd_TASK_کوډ                   TaskFunction_t
    #تعریف_کړه x_List_توکی                     List_توکی_t
    #تعریف_کړه xList                         List_t

/* For libraries that break the list data hiding, and access list structure
 * members directly (which is not supposed to be done). */
    #تعریف_کړه pxContainer                   pvContainer
#د_که_پای /* configENABLE_BACKWARD_COMPATIBILITY */

#إذا ( configUSE_ALTERNATIVE_API != 0 )
    #تېروتنه The alternative API was deprecated some time ago, and was removed دننه FreeRTOS V9.0 0
#د_که_پای

/* Set configUSE_TASK_FPU_SUPPORT to 0 to omit floating point support even
 * if floating point hardware is otherwise supported by the FreeRTOS port in use.
 * This constant is not supported by all FreeRTOS ports that include floating
 * point support. */
#که_نه_تعریف_شوی configUSE_TASK_FPU_SUPPORT
    #تعریف_کړه configUSE_TASK_FPU_SUPPORT    1
#د_که_پای

/* Set configENABLE_MPU to 1 to enable MPU support and 0 to disable it. This is
 * currently used in ARMv8M ports. */
#که_نه_تعریف_شوی configENABLE_MPU
    #تعریف_کړه configENABLE_MPU    0
#د_که_پای

/* Set configENABLE_FPU to 1 to enable FPU support and 0 to disable it. This is
 * currently used in ARMv8M ports. */
#که_نه_تعریف_شوی configENABLE_FPU
    #تعریف_کړه configENABLE_FPU    1
#د_که_پای

/* Set configENABLE_MVE to 1 to enable MVE support and 0 to disable it. This is
 * currently used in ARMv8M ports. */
#که_نه_تعریف_شوی configENABLE_MVE
    #تعریف_کړه configENABLE_MVE    0
#د_که_پای

/* Set configENABLE_TRUSTZONE to 1 enable TrustZone support and 0 to disable it.
 * This is currently used in ARMv8M ports. */
#که_نه_تعریف_شوی configENABLE_TRUSTZONE
    #تعریف_کړه configENABLE_TRUSTZONE    1
#د_که_پای

/* Set configRUN_FREERTOS_SECURE_ONLY to 1 to run the FreeRTOS ARMv8M port on
 * the Secure Side only. */
#که_نه_تعریف_شوی config_پرلپسې_FREERTOS_SECURE_ONLY
    #تعریف_کړه config_پرلپسې_FREERTOS_SECURE_ONLY    0
#د_که_پای

#که_نه_تعریف_شوی config_پرلپسې_ADDITIONAL_TESTS
    #تعریف_کړه config_پرلپسې_ADDITIONAL_TESTS    0
#د_که_پای

/* The following config allows infinite loop control. For example, control the
 * infinite loop in idle task function when performing unit tests. */
#که_نه_تعریف_شوی configCONTROL_INFINITE_LOOP
    #تعریف_کړه configCONTROL_INFINITE_LOOP()
#د_که_پای

/* Sometimes the FreeRTOSConfig.h settings only allow a task to be created using
 * dynamically allocated RAM, in which case when any task is deleted it is known
 * that both the task's stack and TCB need to be freed.  Sometimes the
 * FreeRTOSConfig.h settings only allow a task to be created using statically
 * allocated RAM, in which case when any task is deleted it is known that neither
 * the task's stack or TCB should be freed.  Sometimes the FreeRTOSConfig.h
 * settings allow a task to be created using either statically or dynamically
 * allocated RAM, in which case a member of the TCB is used to record whether the
 * stack and/or TCB were allocated statically or dynamically, so when a task is
 * deleted the RAM that was allocated dynamically is freed again and no attempt is
 * made to free the RAM that was allocated statically.
 * tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE is only true if it is possible for a
 * task to be created using either statically or dynamically allocated RAM.  Note
 * that if portUSING_MPU_WRAPPERS is 1 then a protected task can be created with
 * a statically allocated stack and a dynamically allocated TCB.
 *
 * The following table lists various combinations of portUSING_MPU_WRAPPERS,
 * configSUPPORT_DYNAMIC_ALLOCATION and configSUPPORT_STATIC_ALLOCATION and
 * when it is possible to have both static and dynamic allocation:
 *  +-----+---------+--------+-----------------------------+-----------------------------------+------------------+-----------+
 * | MPU | Dynamic | Static |     Available Functions     |       Possible Allocations        | Both Dynamic and | Need Free |
 * |     |         |        |                             |                                   | Static Possible  |           |
 * +-----+---------+--------+-----------------------------+-----------------------------------+------------------+-----------+
 * | 0   | 0       | 1      | xTaskCreateStatic           | TCB - Static, Stack - Static      | No               | No        |
 * +-----|---------|--------|-----------------------------|-----------------------------------|------------------|-----------|
 * | 0   | 1       | 0      | xTaskCreate                 | TCB - Dynamic, Stack - Dynamic    | No               | Yes       |
 * +-----|---------|--------|-----------------------------|-----------------------------------|------------------|-----------|
 * | 0   | 1       | 1      | xTaskCreate,                | 1. TCB - Dynamic, Stack - Dynamic | Yes              | Yes       |
 * |     |         |        | xTaskCreateStatic           | 2. TCB - Static, Stack - Static   |                  |           |
 * +-----|---------|--------|-----------------------------|-----------------------------------|------------------|-----------|
 * | 1   | 0       | 1      | xTaskCreateStatic,          | TCB - Static, Stack - Static      | No               | No        |
 * |     |         |        | xTaskCreateRestrictedStatic |                                   |                  |           |
 * +-----|---------|--------|-----------------------------|-----------------------------------|------------------|-----------|
 * | 1   | 1       | 0      | xTaskCreate,                | 1. TCB - Dynamic, Stack - Dynamic | Yes              | Yes       |
 * |     |         |        | xTaskCreateRestricted       | 2. TCB - Dynamic, Stack - Static  |                  |           |
 * +-----|---------|--------|-----------------------------|-----------------------------------|------------------|-----------|
 * | 1   | 1       | 1      | xTaskCreate,                | 1. TCB - Dynamic, Stack - Dynamic | Yes              | Yes       |
 * |     |         |        | xTaskCreateStatic,          | 2. TCB - Dynamic, Stack - Static  |                  |           |
 * |     |         |        | xTaskCreateRestricted,      | 3. TCB - Static, Stack - Static   |                  |           |
 * |     |         |        | xTaskCreateRestrictedStatic |                                   |                  |           |
 * +-----+---------+--------+-----------------------------+-----------------------------------+------------------+-----------+
 */
#تعریف_کړه tsk_STATIC_AND_DYNAMIC_ویش_POSSIBLE                                                                                     \
    ( ( ( پورټ_USING_MPU_WRAPPERS == 0 ) && ( config_SUPPORT_DYNAMIC_ویش == 1 ) && ( config_SUPPORT_STATIC_ویش == 1 ) ) || \
      ( ( پورټ_USING_MPU_WRAPPERS == 1 ) && ( config_SUPPORT_DYNAMIC_ویش == 1 ) ) )

/*
 * In line with software engineering best practice, FreeRTOS implements a strict
 * data hiding policy, so the real structures used by FreeRTOS to maintain the
 * state of tasks, queues, semaphores, etc. are not accessible to the application
 * code.  However, if the application writer wants to statically allocate such
 * an object then the size of the object needs to be known.  Dummy structures
 * that are guaranteed to have the same size and alignment requirements of the
 * real objects are used for this purpose.  The dummy list and list item
 * structures below are used for inclusion in such a dummy structure.
 */
بنية x_STATIC_LIST_توکی
{
    #إذا ( config_USE_LIST_معلومات_INTEGRITY_CHECK_بایټونه == 1 )
        TickType_t xDummy1;
    #د_که_پای
    TickType_t xDummy2;
    فراغ * pvDummy3[ 4 ];
    #إذا ( config_USE_LIST_معلومات_INTEGRITY_CHECK_بایټونه == 1 )
        TickType_t xDummy4;
    #د_که_پای
};
تعريفنوع بنية x_STATIC_LIST_توکی Static_List_توکی_t;

#إذا ( config_USE_MINI_LIST_توکی == 1 )
    /* See the comments above the struct xSTATIC_LIST_ITEM definition. */
    بنية x_STATIC_MINI_LIST_توکی
    {
        #إذا ( config_USE_LIST_معلومات_INTEGRITY_CHECK_بایټونه == 1 )
            TickType_t xDummy1;
        #د_که_پای
        TickType_t xDummy2;
        فراغ * pvDummy3[ 2 ];
    };
    تعريفنوع بنية x_STATIC_MINI_LIST_توکی Static_Mini_List_توکی_t;
#وإلا /* if ( configUSE_MINI_LIST_ITEM == 1 ) */
    تعريفنوع بنية x_STATIC_LIST_توکی      Static_Mini_List_توکی_t;
#د_که_پای /* if ( configUSE_MINI_LIST_ITEM == 1 ) */

/* See the comments above the struct xSTATIC_LIST_ITEM definition. */
تعريفنوع بنية xSTATIC_LIST
{
    #إذا ( config_USE_LIST_معلومات_INTEGRITY_CHECK_بایټونه == 1 )
        TickType_t xDummy1;
    #د_که_پای
    UBaseType_t uxDummy2;
    فراغ * pvDummy3;
    Static_Mini_List_توکی_t xDummy4;
    #إذا ( config_USE_LIST_معلومات_INTEGRITY_CHECK_بایټونه == 1 )
        TickType_t xDummy5;
    #د_که_پای
} StaticList_t;

/*
 * In line with software engineering best practice, especially when supplying a
 * library that is likely to change in future versions, FreeRTOS implements a
 * strict data hiding policy.  This means the Task structure used internally by
 * FreeRTOS is not accessible to application code.  However, if the application
 * writer wants to statically allocate the memory required to create a task then
 * the size of the task object needs to be known.  The StaticTask_t structure
 * below is provided for this purpose.  Its sizes and alignment requirements are
 * guaranteed to match those of the genuine structure, no matter which
 * architecture is being used, and no matter how the values in FreeRTOSConfig.h
 * are set.  Its contents are somewhat obfuscated in the hope users will
 * recognise that it would be unwise to make direct use of the structure members.
 */
تعريفنوع بنية xSTATIC_TCB
{
    فراغ * pxDummy1;
    #إذا ( پورټ_USING_MPU_WRAPPERS == 1 )
        xMPU_SETTINGS xDummy2;
    #د_که_پای
    #إذا ( configUSE_CORE_AFFINITY == 1 ) && ( config_شمېره_د_CORES > 1 )
        UBaseType_t uxDummy26;
    #د_که_پای
    Static_List_توکی_t xDummy3[ 2 ];
    UBaseType_t uxDummy5;
    فراغ * pxDummy6;
    #إذا ( config_شمېره_د_CORES > 1 )
        بنسټ_Type_t xDummy23;
        UBaseType_t uxDummy24;
    #د_که_پای
    uint8_t ucDummy7[ configMAX_TASK_NAME_LEN ];
    #إذا ( configUSE_TASK_PREEMPTION_DISABLE == 1 )
        بنسټ_Type_t xDummy25;
    #د_که_پای
    #إذا ( ( پورټ_ډېری_GROWTH > 0 ) || ( config_ریکارډ_ډېری_لوړ_پته == 1 ) )
        فراغ * pxDummy8;
    #د_که_پای
    #إذا ( پورټ_CRITICAL_NESTING_دننه_TCB == 1 )
        UBaseType_t uxDummy9;
    #د_که_پای
    #إذا ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy10[ 2 ];
    #د_که_پای
    #إذا ( configUSE_MUTEXES == 1 )
        UBaseType_t uxDummy12[ 2 ];
    #د_که_پای
    #إذا ( config_USE_APPLICATION_TASK_نښان == 1 )
        فراغ * pxDummy14;
    #د_که_پای
    #إذا ( config_عدد_THREAD_LOCAL_STORAGE_اشارې > 0 )
        فراغ * pvDummy15[ config_عدد_THREAD_LOCAL_STORAGE_اشارې ];
    #د_که_پای
    #إذا ( config_GENERATE_پرلپسې_وخت_STATS == 1 )
        config_پرلپسې_وخت_COUNTER_TYPE ulDummy16;
    #د_که_پای
    #إذا ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 )
        config_TLS_بلاک_TYPE xDummy17;
    #د_که_پای
    #إذا ( configUSE_TASK_NOTIFICATIONS == 1 )
        uint32_t ulDummy18[ configTASK_NOTIFICATION_ARRAY_ENTRIES ];
        uint8_t ucDummy19[ configTASK_NOTIFICATION_ARRAY_ENTRIES ];
    #د_که_پای
    #إذا ( tsk_STATIC_AND_DYNAMIC_ویش_POSSIBLE != 0 )
        uint8_t uxDummy20;
    #د_که_پای

    #إذا ( INCLUDE_xTaskAbortDelay == 1 )
        uint8_t ucDummy21;
    #د_که_پای
    #إذا ( configUSE_POSIX_ERRNO == 1 )
        صحيح iDummy22;
    #د_که_پای
} StaticTask_t;

/*
 * In line with software engineering best practice, especially when supplying a
 * library that is likely to change in future versions, FreeRTOS implements a
 * strict data hiding policy.  This means the Queue structure used internally by
 * FreeRTOS is not accessible to application code.  However, if the application
 * writer wants to statically allocate the memory required to create a queue
 * then the size of the queue object needs to be known.  The StaticQueue_t
 * structure below is provided for this purpose.  Its sizes and alignment
 * requirements are guaranteed to match those of the genuine structure, no
 * matter which architecture is being used, and no matter how the values in
 * FreeRTOSConfig.h are set.  Its contents are somewhat obfuscated in the hope
 * users will recognise that it would be unwise to make direct use of the
 * structure members.
 */
تعريفنوع بنية x_STATIC_کتار
{
    فراغ * pvDummy1[ 3 ];

    اتحاد
    {
        فراغ * pvDummy2;
        UBaseType_t uxDummy2;
    } u;

    StaticList_t xDummy3[ 2 ];
    UBaseType_t uxDummy4[ 3 ];
    uint8_t ucDummy5[ 2 ];

    #إذا ( ( config_SUPPORT_STATIC_ویش == 1 ) && ( config_SUPPORT_DYNAMIC_ویش == 1 ) )
        uint8_t ucDummy6;
    #د_که_پای

    #إذا ( config_USE_کتار_SETS == 1 )
        فراغ * pvDummy7;
    #د_که_پای

    #إذا ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy8;
        uint8_t ucDummy9;
    #د_که_پای
} Static_کتار_t;
تعريفنوع Static_کتار_t StaticSemaphore_t;

/*
 * In line with software engineering best practice, especially when supplying a
 * library that is likely to change in future versions, FreeRTOS implements a
 * strict data hiding policy.  This means the event group structure used
 * internally by FreeRTOS is not accessible to application code.  However, if
 * the application writer wants to statically allocate the memory required to
 * create an event group then the size of the event group object needs to be
 * know.  The StaticEventGroup_t structure below is provided for this purpose.
 * Its sizes and alignment requirements are guaranteed to match those of the
 * genuine structure, no matter which architecture is being used, and no matter
 * how the values in FreeRTOSConfig.h are set.  Its contents are somewhat
 * obfuscated in the hope users will recognise that it would be unwise to make
 * direct use of the structure members.
 */
تعريفنوع بنية x_STATIC_EVENT_ډله
{
    TickType_t xDummy1;
    StaticList_t xDummy2;

    #إذا ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy3;
    #د_که_پای

    #إذا ( ( config_SUPPORT_STATIC_ویش == 1 ) && ( config_SUPPORT_DYNAMIC_ویش == 1 ) )
        uint8_t ucDummy4;
    #د_که_پای
} Static_Event_ډله_t;

/*
 * In line with software engineering best practice, especially when supplying a
 * library that is likely to change in future versions, FreeRTOS implements a
 * strict data hiding policy.  This means the software timer structure used
 * internally by FreeRTOS is not accessible to application code.  However, if
 * the application writer wants to statically allocate the memory required to
 * create a software timer then the size of the queue object needs to be known.
 * The StaticTimer_t structure below is provided for this purpose.  Its sizes
 * and alignment requirements are guaranteed to match those of the genuine
 * structure, no matter which architecture is being used, and no matter how the
 * values in FreeRTOSConfig.h are set.  Its contents are somewhat obfuscated in
 * the hope users will recognise that it would be unwise to make direct use of
 * the structure members.
 */
تعريفنوع بنية xSTATIC_TIMER
{
    فراغ * pvDummy1;
    Static_List_توکی_t xDummy2;
    TickType_t xDummy3;
    فراغ * pvDummy5;
    TaskFunction_t pvDummy6;
    #إذا ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy7;
    #د_که_پای
    uint8_t ucDummy8;
} StaticTimer_t;

/*
 * In line with software engineering best practice, especially when supplying a
 * library that is likely to change in future versions, FreeRTOS implements a
 * strict data hiding policy.  This means the stream buffer structure used
 * internally by FreeRTOS is not accessible to application code.  However, if
 * the application writer wants to statically allocate the memory required to
 * create a stream buffer then the size of the stream buffer object needs to be
 * known.  The StaticStreamBuffer_t structure below is provided for this
 * purpose.  Its size and alignment requirements are guaranteed to match those
 * of the genuine structure, no matter which architecture is being used, and
 * no matter how the values in FreeRTOSConfig.h are set.  Its contents are
 * somewhat obfuscated in the hope users will recognise that it would be unwise
 * to make direct use of the structure members.
 */
تعريفنوع بنية x_STATIC_بهیر_بفر
{
    size_t uxDummy1[ 4 ];
    فراغ * pvDummy2[ 3 ];
    uint8_t ucDummy3;
    #إذا ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy4;
    #د_که_پای
    #إذا ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        فراغ * pvDummy5[ 2 ];
    #د_که_پای
    UBaseType_t uxDummy6;
} Static_بهیر_بفر_t;

/* Message buffers are built on stream buffers. */
تعريفنوع Static_بهیر_بفر_t Static_پیغام_بفر_t;

/* *INDENT-OFF* */
#که_تعریف_شوی __cplusplus
    }
#د_که_پای
/* *INDENT-ON* */

/*-----------------------------------------------------------
* IDF Compatibility
*----------------------------------------------------------*/

#که_تعریف_شوی ESP_PLATFORM

/*
 * Include ESP-IDF API additions implicitly for compatibility reasons.
 *
 * ESP-IDF API additions were previously added directly to FreeRTOS headers
 * (e.g., task.h, queue.h). These APIs have now been moved to
 * idf_additions.h.
 *
 * To ensure there are no breaking changes, we include idf_additions.h
 * implicitly here so that those API additions are still accessible. Given
 * that FreeRTOS.h must be included first before calling any FreeRTOS API,
 * any existing source code can continue using these relocated APIs without
 * any additional header inclusions via this implicit inclusion.
 */
    #ضمن "freertos/idf_additions.h"

#د_که_پای /* ESP_PLATFORM */

#د_که_پای /* INC_FREERTOS_H */
