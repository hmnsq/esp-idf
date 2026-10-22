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

#اگر_تعریف_نشده INC_FREERTOS_H
#تعریف_کن INC_FREERTOS_H

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
#اگر_تعریف_شده __cplusplus
    خارجي "C" {
#پایان_اگر
/* *INDENT-ON* */

/* Acceptable values for configTICK_TYPE_WIDTH_IN_BITS. */
#تعریف_کن TICK_TYPE_عرض_16_بیتها    0
#تعریف_کن TICK_TYPE_عرض_32_بیتها    1
#تعریف_کن TICK_TYPE_عرض_64_بیتها    2

/* Application specific configuration options. */
#ضمن "FreeRTOSConfig.h"

#إذا !defined( config_USE_16_بیت_TICKS ) && !defined( config_TICK_TYPE_عرض_درون_بیتها )
    #خطا Missing definition:  One از config_USE_16_بیت_TICKS and config_TICK_TYPE_عرض_درون_بیتها must be defined درون FreeRTOSConfig.h.  See the Configuration section از the FreeRTOS API documentation لكل details.
#پایان_اگر

#إذا defined( config_USE_16_بیت_TICKS ) && defined( config_TICK_TYPE_عرض_درون_بیتها )
    #خطا Only one از config_USE_16_بیت_TICKS and config_TICK_TYPE_عرض_درون_بیتها must be defined درون FreeRTOSConfig.h.  See the Configuration section از the FreeRTOS API documentation لكل details.
#پایان_اگر

/* Define configTICK_TYPE_WIDTH_IN_BITS according to the
 * value of configUSE_16_BIT_TICKS for backward compatibility. */
#اگر_تعریف_نشده config_TICK_TYPE_عرض_درون_بیتها
    #إذا ( config_USE_16_بیت_TICKS == 1 )
        #تعریف_کن config_TICK_TYPE_عرض_درون_بیتها    TICK_TYPE_عرض_16_بیتها
    #وإلا
        #تعریف_کن config_TICK_TYPE_عرض_درون_بیتها    TICK_TYPE_عرض_32_بیتها
    #پایان_اگر
#پایان_اگر

/* Set configUSE_MPU_WRAPPERS_V1 to 1 to use MPU wrappers v1. */
#اگر_تعریف_نشده configUSE_MPU_WRAPPERS_V1
    #تعریف_کن configUSE_MPU_WRAPPERS_V1    0
#پایان_اگر

/* Set configENABLE_ACCESS_CONTROL_LIST to 1 to enable access control list support. */
#اگر_تعریف_نشده configENABLE_ACCESS_CONTROL_LIST
    #تعریف_کن configENABLE_ACCESS_CONTROL_LIST    0
#پایان_اگر

/* Set default value of configNUMBER_OF_CORES to 1 to use single core FreeRTOS. */
#اگر_تعریف_نشده config_عدد_از_CORES
    #تعریف_کن config_عدد_از_CORES    1
#پایان_اگر

#اگر_تعریف_نشده configUSE_MALLOC_FAILED_HOOK
    #تعریف_کن configUSE_MALLOC_FAILED_HOOK    0
#پایان_اگر

/* Basic FreeRTOS definitions. */
#ضمن "projdefs.h"

/* Definitions specific to the port being used. */
#ضمن "portable.h"

/* Must be defaulted before configUSE_NEWLIB_REENTRANT is used below. */
#اگر_تعریف_نشده configUSE_NEWLIB_REENTRANT
    #تعریف_کن configUSE_NEWLIB_REENTRANT    0
#پایان_اگر

/* Required if struct _reent is used. */
#إذا ( configUSE_NEWLIB_REENTRANT == 1 )

    #ضمن "newlib-freertos.h"

#پایان_اگر /* if ( configUSE_NEWLIB_REENTRANT == 1 ) */

/* Must be defaulted before configUSE_PICOLIBC_TLS is used below. */
#اگر_تعریف_نشده configUSE_PICOLIBC_TLS
    #تعریف_کن configUSE_PICOLIBC_TLS    0
#پایان_اگر

#إذا ( configUSE_PICOLIBC_TLS == 1 )

    #ضمن "picolibc-freertos.h"

#پایان_اگر /* if ( configUSE_PICOLIBC_TLS == 1 ) */

#اگر_تعریف_نشده configUSE_C_RUNTIME_TLS_SUPPORT
    #تعریف_کن configUSE_C_RUNTIME_TLS_SUPPORT    0
#پایان_اگر

#إذا ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 )

    #اگر_تعریف_نشده config_TLS_بلوک_TYPE
        #خطا Missing definition:  config_TLS_بلوک_TYPE must be defined درون FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is مجموعه مقصد 1.
    #پایان_اگر

    #اگر_تعریف_نشده config_INIT_TLS_بلوک
        #خطا Missing definition:  config_INIT_TLS_بلوک must be defined درون FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is مجموعه مقصد 1.
    #پایان_اگر

    #اگر_تعریف_نشده config_مجموعه_TLS_بلوک
        #خطا Missing definition:  config_مجموعه_TLS_بلوک must be defined درون FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is مجموعه مقصد 1.
    #پایان_اگر

    #اگر_تعریف_نشده config_DEINIT_TLS_بلوک
        #خطا Missing definition:  config_DEINIT_TLS_بلوک must be defined درون FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is مجموعه مقصد 1.
    #پایان_اگر
#پایان_اگر /* if ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 ) */

/*
 * Check all the required application specific macros have been defined.
 * These macros are application specific and (as downloaded) are defined
 * within FreeRTOSConfig.h.
 */

#اگر_تعریف_نشده config_کمینه_پشته_اندازه
    #خطا Missing definition:  config_کمینه_پشته_اندازه must be defined درون FreeRTOSConfig.h.  config_کمینه_پشته_اندازه تعریف_میکند the اندازه (درون واژگان) از the پشته allocated مقصد the idle task.  Refer مقصد the demo project provided لكل your پورت لكل a suitable مقدار.
#پایان_اگر

#اگر_تعریف_نشده configMAX_PRIORITIES
    #خطا Missing definition:  configMAX_PRIORITIES must be defined درون FreeRTOSConfig.h.  See the Configuration section از the FreeRTOS API documentation لكل details.
#پایان_اگر

#إذا configMAX_PRIORITIES < 1
    #خطا configMAX_PRIORITIES must be defined مقصد be greater than or equal مقصد 1.
#پایان_اگر

#اگر_تعریف_نشده configUSE_PREEMPTION
    #خطا Missing definition:  configUSE_PREEMPTION must be defined درون FreeRTOSConfig.h as either 1 or 0.  See the Configuration section از the FreeRTOS API documentation لكل details.
#پایان_اگر

#اگر_تعریف_نشده configUSE_IDLE_HOOK
    #خطا Missing definition:  configUSE_IDLE_HOOK must be defined درون FreeRTOSConfig.h as either 1 or 0.  See the Configuration section از the FreeRTOS API documentation لكل details.
#پایان_اگر

#إذا ( config_عدد_از_CORES > 1 )
    #اگر_تعریف_نشده configUSE_PASSIVE_IDLE_HOOK
        #خطا Missing definition:  configUSE_PASSIVE_IDLE_HOOK must be defined درون FreeRTOSConfig.h as either 1 or 0.  See the Configuration section از the FreeRTOS API documentation لكل details.
    #پایان_اگر
#پایان_اگر

#اگر_تعریف_نشده configUSE_TICK_HOOK
    #خطا Missing definition:  configUSE_TICK_HOOK must be defined درون FreeRTOSConfig.h as either 1 or 0.  See the Configuration section از the FreeRTOS API documentation لكل details.
#پایان_اگر

#إذا ( ( config_TICK_TYPE_عرض_درون_بیتها != TICK_TYPE_عرض_16_بیتها ) && \
    ( config_TICK_TYPE_عرض_درون_بیتها != TICK_TYPE_عرض_32_بیتها ) &&   \
    ( config_TICK_TYPE_عرض_درون_بیتها != TICK_TYPE_عرض_64_بیتها ) )
    #خطا Macro config_TICK_TYPE_عرض_درون_بیتها is defined مقصد incorrect مقدار.  See the Configuration section از the FreeRTOS API documentation لكل details.
#پایان_اگر

#اگر_تعریف_نشده configUSE_CO_ROUTINES
    #تعریف_کن configUSE_CO_ROUTINES    0
#پایان_اگر

#اگر_تعریف_نشده INCLUDE_v_Task_اولویت_مجموعه
    #تعریف_کن INCLUDE_v_Task_اولویت_مجموعه    0
#پایان_اگر

#اگر_تعریف_نشده INCLUDE_ux_Task_اولویت_Get
    #تعریف_کن INCLUDE_ux_Task_اولویت_Get    0
#پایان_اگر

#اگر_تعریف_نشده INCLUDE_vTaskDelete
    #تعریف_کن INCLUDE_vTaskDelete    0
#پایان_اگر

#اگر_تعریف_نشده INCLUDE_vTaskSuspend
    #تعریف_کن INCLUDE_vTaskSuspend    0
#پایان_اگر

#اگر_تعریف_شده INCLUDE_xTaskDelayUntil
    #اگر_تعریف_شده INCLUDE_vTaskDelayUntil

/* INCLUDE_vTaskDelayUntil was replaced by INCLUDE_xTaskDelayUntil.  Backward
 * compatibility is maintained if only one or the other is defined, but
 * there is a conflict if both are defined. */
        #خطا INCLUDE_vTaskDelayUntil and INCLUDE_xTaskDelayUntil are both defined.  INCLUDE_vTaskDelayUntil is no longer required and should be removed
    #پایان_اگر
#پایان_اگر

#اگر_تعریف_نشده INCLUDE_xTaskDelayUntil
    #اگر_تعریف_شده INCLUDE_vTaskDelayUntil

/* If INCLUDE_vTaskDelayUntil is set but INCLUDE_xTaskDelayUntil is not then
 * the project's FreeRTOSConfig.h probably pre-dates the introduction of
 * xTaskDelayUntil and setting INCLUDE_xTaskDelayUntil to whatever
 * INCLUDE_vTaskDelayUntil is set to will ensure backward compatibility.
 */
        #تعریف_کن INCLUDE_xTaskDelayUntil    INCLUDE_vTaskDelayUntil
    #پایان_اگر
#پایان_اگر

#اگر_تعریف_نشده INCLUDE_xTaskDelayUntil
    #تعریف_کن INCLUDE_xTaskDelayUntil    0
#پایان_اگر

#اگر_تعریف_نشده INCLUDE_vTaskDelay
    #تعریف_کن INCLUDE_vTaskDelay    0
#پایان_اگر

#اگر_تعریف_نشده INCLUDE_xTaskGetIdleTaskHandle
    #تعریف_کن INCLUDE_xTaskGetIdleTaskHandle    0
#پایان_اگر

#اگر_تعریف_نشده INCLUDE_xTaskAbortDelay
    #تعریف_کن INCLUDE_xTaskAbortDelay    0
#پایان_اگر

#اگر_تعریف_نشده INCLUDE_x_صف_Get_Mutex_Holder
    #تعریف_کن INCLUDE_x_صف_Get_Mutex_Holder    0
#پایان_اگر

#اگر_تعریف_نشده INCLUDE_xSemaphoreGetMutexHolder
    #تعریف_کن INCLUDE_xSemaphoreGetMutexHolder    INCLUDE_x_صف_Get_Mutex_Holder
#پایان_اگر

#اگر_تعریف_نشده INCLUDE_xTaskGetHandle
    #تعریف_کن INCLUDE_xTaskGetHandle    0
#پایان_اگر

#اگر_تعریف_نشده INCLUDE_ux_Task_Get_پشته_بالا_Water_Mark
    #تعریف_کن INCLUDE_ux_Task_Get_پشته_بالا_Water_Mark    0
#پایان_اگر

#اگر_تعریف_نشده INCLUDE_ux_Task_Get_پشته_بالا_Water_Mark2
    #تعریف_کن INCLUDE_ux_Task_Get_پشته_بالا_Water_Mark2    0
#پایان_اگر

#اگر_تعریف_نشده INCLUDE_eTaskGetState
    #تعریف_کن INCLUDE_eTaskGetState    0
#پایان_اگر

#اگر_تعریف_نشده INCLUDE_xTaskResumeFromISR
    #تعریف_کن INCLUDE_xTaskResumeFromISR    1
#پایان_اگر

#اگر_تعریف_نشده INCLUDE_xTimerPendFunctionCall
    #تعریف_کن INCLUDE_xTimerPendFunctionCall    0
#پایان_اگر

#اگر_تعریف_نشده INCLUDE_xTaskGetSchedulerState
    #تعریف_کن INCLUDE_xTaskGetSchedulerState    0
#پایان_اگر

#اگر_تعریف_نشده INCLUDE_xTaskGetCurrentTaskHandle
    #تعریف_کن INCLUDE_xTaskGetCurrentTaskHandle    1
#پایان_اگر

#إذا configUSE_CO_ROUTINES != 0
    #اگر_تعریف_نشده configMAX_CO_ROUTINE_PRIORITIES
        #خطا configMAX_CO_ROUTINE_PRIORITIES must be greater than or equal مقصد 1.
    #پایان_اگر
#پایان_اگر

#اگر_تعریف_نشده config_USE_APPLICATION_TASK_برچسب
    #تعریف_کن config_USE_APPLICATION_TASK_برچسب    0
#پایان_اگر

#اگر_تعریف_نشده config_رقم_THREAD_LOCAL_STORAGE_اشارهگرها
    #تعریف_کن config_رقم_THREAD_LOCAL_STORAGE_اشارهگرها    0
#پایان_اگر

#اگر_تعریف_نشده configUSE_RECURSIVE_MUTEXES
    #تعریف_کن configUSE_RECURSIVE_MUTEXES    0
#پایان_اگر

#اگر_تعریف_نشده configUSE_MUTEXES
    #تعریف_کن configUSE_MUTEXES    0
#پایان_اگر

#اگر_تعریف_نشده configUSE_TIMERS
    #تعریف_کن configUSE_TIMERS    0
#پایان_اگر

#اگر_تعریف_نشده configUSE_EVENT_GROUPS
    #تعریف_کن configUSE_EVENT_GROUPS    1
#پایان_اگر

#اگر_تعریف_نشده config_USE_جریان_BUFFERS
    #تعریف_کن config_USE_جریان_BUFFERS    1
#پایان_اگر

#اگر_تعریف_نشده configUSE_DAEMON_TASK_STARTUP_HOOK
    #تعریف_کن configUSE_DAEMON_TASK_STARTUP_HOOK    0
#پایان_اگر

#إذا ( configUSE_DAEMON_TASK_STARTUP_HOOK != 0 )
    #إذا ( configUSE_TIMERS == 0 )
        #خطا configUSE_DAEMON_TASK_STARTUP_HOOK is مجموعه, but the daemon task is not created because configUSE_TIMERS is 0.
    #پایان_اگر
#پایان_اگر

#اگر_تعریف_نشده configUSE_COUNTING_SEMAPHORES
    #تعریف_کن configUSE_COUNTING_SEMAPHORES    0
#پایان_اگر

#اگر_تعریف_نشده configUSE_TASK_PREEMPTION_DISABLE
    #تعریف_کن configUSE_TASK_PREEMPTION_DISABLE    0
#پایان_اگر

#اگر_تعریف_نشده configUSE_ALTERNATIVE_API
    #تعریف_کن configUSE_ALTERNATIVE_API    0
#پایان_اگر

#اگر_تعریف_نشده پورت_CRITICAL_NESTING_درون_TCB
    #تعریف_کن پورت_CRITICAL_NESTING_درون_TCB    0
#پایان_اگر

#اگر_تعریف_نشده configMAX_TASK_NAME_LEN
    #تعریف_کن configMAX_TASK_NAME_LEN    16
#پایان_اگر

#اگر_تعریف_نشده configIDLE_SHOULD_YIELD
    #تعریف_کن configIDLE_SHOULD_YIELD    1
#پایان_اگر

#إذا configMAX_TASK_NAME_LEN < 1
    #خطا configMAX_TASK_NAME_LEN must be مجموعه مقصد a minimum از 1 درون FreeRTOSConfig.h
#پایان_اگر

#اگر_تعریف_نشده configASSERT
    #تعریف_کن configASSERT( x )
    #تعریف_کن configASSERT_DEFINED    0
#وإلا
    #تعریف_کن configASSERT_DEFINED    1
#پایان_اگر

/* configPRECONDITION should be defined as configASSERT.
 * The CBMC proofs need a way to track assumptions and assertions.
 * A configPRECONDITION statement should express an implicit invariant or
 * assumption made.  A configASSERT statement should express an invariant that must
 * hold explicit before calling the code. */
#اگر_تعریف_نشده configPRECONDITION
    #تعریف_کن configPRECONDITION( X )    configASSERT( X )
    #تعریف_کن configPRECONDITION_DEFINED    0
#وإلا
    #تعریف_کن configPRECONDITION_DEFINED    1
#پایان_اگر

#اگر_تعریف_نشده configCHECK_HANDLER_INSTALLATION
    #تعریف_کن configCHECK_HANDLER_INSTALLATION    1
#وإلا

/* The application has explicitly defined configCHECK_HANDLER_INSTALLATION
 * to 1. The checks requires configASSERT() to be defined. */
    #إذا ( ( configCHECK_HANDLER_INSTALLATION == 1 ) && ( configASSERT_DEFINED == 0 ) )
        #خطا You must define configASSERT() when configCHECK_HANDLER_INSTALLATION is 1.
    #پایان_اگر
#پایان_اگر

#اگر_تعریف_نشده پورت_حافظه_BARRIER
    #تعریف_کن پورت_حافظه_BARRIER()
#پایان_اگر

#اگر_تعریف_نشده پورت_SOFTWARE_BARRIER
    #تعریف_کن پورت_SOFTWARE_BARRIER()
#پایان_اگر

#اگر_تعریف_نشده config_پیاپی_مضرب_PRIORITIES
    #تعریف_کن config_پیاپی_مضرب_PRIORITIES    0
#پایان_اگر

#اگر_تعریف_نشده پورت_GET_CORE_ID

    #إذا ( config_عدد_از_CORES == 1 )
        #تعریف_کن پورت_GET_CORE_ID()    0
    #وإلا
        #خطا config_عدد_از_CORES is مجموعه مقصد بیشتر than 1 then پورت_GET_CORE_ID must also be defined.
    #پایان_اگر /* configNUMBER_OF_CORES */

#پایان_اگر /* portGET_CORE_ID */

#اگر_تعریف_نشده پورت_YIELD_CORE

    #إذا ( config_عدد_از_CORES == 1 )
        #تعریف_کن پورت_YIELD_CORE( x )    پورت_YIELD()
    #وإلا
        #خطا config_عدد_از_CORES is مجموعه مقصد بیشتر than 1 then پورت_YIELD_CORE must also be defined.
    #پایان_اگر /* configNUMBER_OF_CORES */

#پایان_اگر /* portYIELD_CORE */

#اگر_تعریف_نشده پورت_مجموعه_وقفه_ماسک

    #إذا ( config_عدد_از_CORES > 1 )
        #خطا پورت_مجموعه_وقفه_ماسک is required درون SMP
    #پایان_اگر

#پایان_اگر /* portSET_INTERRUPT_MASK */

#اگر_تعریف_نشده پورت_CLEAR_وقفه_ماسک

    #إذا ( config_عدد_از_CORES > 1 )
        #خطا پورت_CLEAR_وقفه_ماسک is required درون SMP
    #پایان_اگر

#پایان_اگر /* portCLEAR_INTERRUPT_MASK */

#اگر_تعریف_نشده پورت_RELEASE_TASK_قفل

    #إذا ( config_عدد_از_CORES == 1 )
        #تعریف_کن پورت_RELEASE_TASK_قفل()
    #وإلا
        #خطا پورت_RELEASE_TASK_قفل is required درون SMP
    #پایان_اگر

#پایان_اگر /* portRELEASE_TASK_LOCK */

#اگر_تعریف_نشده پورت_GET_TASK_قفل

    #إذا ( config_عدد_از_CORES == 1 )
        #تعریف_کن پورت_GET_TASK_قفل()
    #وإلا
        #خطا پورت_GET_TASK_قفل is required درون SMP
    #پایان_اگر

#پایان_اگر /* portGET_TASK_LOCK */

#اگر_تعریف_نشده پورت_RELEASE_ISR_قفل

    #إذا ( config_عدد_از_CORES == 1 )
        #تعریف_کن پورت_RELEASE_ISR_قفل()
    #وإلا
        #خطا پورت_RELEASE_ISR_قفل is required درون SMP
    #پایان_اگر

#پایان_اگر /* portRELEASE_ISR_LOCK */

#اگر_تعریف_نشده پورت_GET_ISR_قفل

    #إذا ( config_عدد_از_CORES == 1 )
        #تعریف_کن پورت_GET_ISR_قفل()
    #وإلا
        #خطا پورت_GET_ISR_قفل is required درون SMP
    #پایان_اگر

#پایان_اگر /* portGET_ISR_LOCK */

#اگر_تعریف_نشده پورت_ENTER_CRITICAL_FROM_ISR

    #إذا ( config_عدد_از_CORES > 1 )
        #خطا پورت_ENTER_CRITICAL_FROM_ISR is required درون SMP
    #پایان_اگر

#پایان_اگر

#اگر_تعریف_نشده پورت_EXIT_CRITICAL_FROM_ISR

    #إذا ( config_عدد_از_CORES > 1 )
        #خطا پورت_EXIT_CRITICAL_FROM_ISR is required درون SMP
    #پایان_اگر

#پایان_اگر

#اگر_تعریف_نشده configUSE_CORE_AFFINITY
    #تعریف_کن configUSE_CORE_AFFINITY    0
#پایان_اگر /* configUSE_CORE_AFFINITY */

#إذا ( ( config_عدد_از_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
    #اگر_تعریف_نشده configTASK_DEFAULT_CORE_AFFINITY
        #تعریف_کن configTASK_DEFAULT_CORE_AFFINITY    tskNO_AFFINITY
    #پایان_اگر
#پایان_اگر

#اگر_تعریف_نشده configUSE_PASSIVE_IDLE_HOOK
    #تعریف_کن configUSE_PASSIVE_IDLE_HOOK    0
#پایان_اگر /* configUSE_PASSIVE_IDLE_HOOK */

/* The timers module relies on xTaskGetSchedulerState(). */
#إذا configUSE_TIMERS == 1

    #اگر_تعریف_نشده config_TIMER_TASK_اولویت
        #خطا If configUSE_TIMERS is مجموعه مقصد 1 then config_TIMER_TASK_اولویت must also be defined.
    #پایان_اگر /* configTIMER_TASK_PRIORITY */

    #اگر_تعریف_نشده config_TIMER_صف_درازی
        #خطا If configUSE_TIMERS is مجموعه مقصد 1 then config_TIMER_صف_درازی must also be defined.
    #پایان_اگر /* configTIMER_QUEUE_LENGTH */

    #اگر_تعریف_نشده config_TIMER_TASK_پشته_عمق
        #خطا If configUSE_TIMERS is مجموعه مقصد 1 then config_TIMER_TASK_پشته_عمق must also be defined.
    #پایان_اگر /* configTIMER_TASK_STACK_DEPTH */

    #اگر_تعریف_نشده پورت_TIMER_CALLBACK_ATTRIBUTE
        #تعریف_کن پورت_TIMER_CALLBACK_ATTRIBUTE
    #پایان_اگر /* portTIMER_CALLBACK_ATTRIBUTE */

#پایان_اگر /* configUSE_TIMERS */

#اگر_تعریف_نشده پورت_HAS_NESTED_INTERRUPTS
    #إذا defined( پورت_مجموعه_وقفه_ماسک_FROM_ISR ) && defined( پورت_CLEAR_وقفه_ماسک_FROM_ISR )
        #تعریف_کن پورت_HAS_NESTED_INTERRUPTS    1
    #وإلا
        #تعریف_کن پورت_HAS_NESTED_INTERRUPTS    0
    #پایان_اگر
#پایان_اگر

#اگر_تعریف_نشده پورت_مجموعه_وقفه_ماسک_FROM_ISR
    #إذا ( پورت_HAS_NESTED_INTERRUPTS == 1 )
        #خطا پورت_مجموعه_وقفه_ماسک_FROM_ISR must be defined لكل ports that support nested interrupts (i.e. پورت_HAS_NESTED_INTERRUPTS is مجموعه مقصد 1)
    #وإلا
        #تعریف_کن پورت_مجموعه_وقفه_ماسک_FROM_ISR()    0
    #پایان_اگر
#وإلا
    #إذا ( پورت_HAS_NESTED_INTERRUPTS == 0 )
        #خطا پورت_مجموعه_وقفه_ماسک_FROM_ISR must not be defined لكل ports that افعل not support nested interrupts (i.e. پورت_HAS_NESTED_INTERRUPTS is مجموعه مقصد 0)
    #پایان_اگر
#پایان_اگر

#اگر_تعریف_نشده پورت_CLEAR_وقفه_ماسک_FROM_ISR
    #إذا ( پورت_HAS_NESTED_INTERRUPTS == 1 )
        #خطا پورت_CLEAR_وقفه_ماسک_FROM_ISR must be defined لكل ports that support nested interrupts  (i.e. پورت_HAS_NESTED_INTERRUPTS is مجموعه مقصد 1)
    #وإلا
        #تعریف_کن پورت_CLEAR_وقفه_ماسک_FROM_ISR( ux_Saved_وضعیت_مقدار )    ( فراغ ) ( ux_Saved_وضعیت_مقدار )
    #پایان_اگر
#وإلا
    #إذا ( پورت_HAS_NESTED_INTERRUPTS == 0 )
        #خطا پورت_CLEAR_وقفه_ماسک_FROM_ISR must not be defined لكل ports that افعل not support nested interrupts (i.e. پورت_HAS_NESTED_INTERRUPTS is مجموعه مقصد 0)
    #پایان_اگر
#پایان_اگر

#اگر_تعریف_نشده پورت_CLEAN_به_بالا_TCB
    #تعریف_کن پورت_CLEAN_به_بالا_TCB( pxTCB )    ( فراغ ) ( pxTCB )
#پایان_اگر

#اگر_تعریف_نشده پورت_PRE_TASK_DELETE_HOOK
    #تعریف_کن پورت_PRE_TASK_DELETE_HOOK( pv_Task_مقصد_Delete, pxYieldPending )
#پایان_اگر

#اگر_تعریف_نشده پورت_راهاندازی_TCB
    #تعریف_کن پورت_راهاندازی_TCB( pxTCB )    ( فراغ ) ( pxTCB )
#پایان_اگر

#اگر_تعریف_نشده پورت_TASK_SWITCH_HOOK
    #تعریف_کن پورت_TASK_SWITCH_HOOK( pxTCB )    ( فراغ ) ( pxTCB )
#پایان_اگر

#اگر_تعریف_نشده config_صف_REGISTRY_اندازه
    #تعریف_کن config_صف_REGISTRY_اندازه    0U
#پایان_اگر

#إذا ( config_صف_REGISTRY_اندازه < 1 )
    #تعریف_کن v_صف_جمع_کن_مقصد_Registry( x_صف, pcName )
    #تعریف_کن v_صف_Unregister_صف( x_صف )
    #تعریف_کن pc_صف_Get_Name( x_صف )
#پایان_اگر

#اگر_تعریف_نشده config_USE_MINI_LIST_قلم
    #تعریف_کن config_USE_MINI_LIST_قلم    1
#پایان_اگر

#اگر_تعریف_نشده پورت_اشاره_گر_اندازه_TYPE
    #تعریف_کن پورت_اشاره_گر_اندازه_TYPE    uint32_t
#پایان_اگر

/* Remove any unused trace macros. */
#اگر_تعریف_نشده trace_شروع

/* Used to perform any necessary initialisation - for example, open a file
 * into which trace is to be written. */
    #تعریف_کن trace_شروع()
#پایان_اگر

#اگر_تعریف_نشده trace_انتها

/* Use to close a trace, for example close a file into which trace has been
 * written. */
    #تعریف_کن trace_انتها()
#پایان_اگر

#اگر_تعریف_نشده trace_TASK_SWITCHED_درون

/* Called after a task has been selected to run.  pxCurrentTCB holds a pointer
 * to the task control block of the selected task. */
    #تعریف_کن trace_TASK_SWITCHED_درون()
#پایان_اگر

#اگر_تعریف_نشده trace_INCREASE_TICK_شمار

/* Called before stepping the tick count after waking from tickless idle
 * sleep. */
    #تعریف_کن trace_INCREASE_TICK_شمار( x )
#پایان_اگر

#اگر_تعریف_نشده trace_کم_POWER_IDLE_BEGIN
    /* Called immediately before entering tickless idle. */
    #تعریف_کن trace_کم_POWER_IDLE_BEGIN()
#پایان_اگر

#اگر_تعریف_نشده trace_کم_POWER_IDLE_انتها
    /* Called when returning to the Idle task after a tickless idle. */
    #تعریف_کن trace_کم_POWER_IDLE_انتها()
#پایان_اگر

#اگر_تعریف_نشده trace_TASK_SWITCHED_بیرون

/* Called before a task has been selected to run.  pxCurrentTCB holds a pointer
 * to the task control block of the task being switched out. */
    #تعریف_کن trace_TASK_SWITCHED_بیرون()
#پایان_اگر

#اگر_تعریف_نشده trace_TASK_اولویت_INHERIT

/* Called when a task attempts to take a mutex that is already held by a
 * lower priority task.  pxTCBOfMutexHolder is a pointer to the TCB of the task
 * that holds the mutex.  uxInheritedPriority is the priority the mutex holder
 * will inherit (the priority of the task that is attempting to obtain the
 * muted. */
    #تعریف_کن trace_TASK_اولویت_INHERIT( pxTCBOfMutexHolder, ux_Inherited_اولویت )
#پایان_اگر

#اگر_تعریف_نشده trace_TASK_اولویت_DISINHERIT

/* Called when a task releases a mutex, the holding of which had resulted in
 * the task inheriting the priority of a higher priority task.
 * pxTCBOfMutexHolder is a pointer to the TCB of the task that is releasing the
 * mutex.  uxOriginalPriority is the task's configured (base) priority. */
    #تعریف_کن trace_TASK_اولویت_DISINHERIT( pxTCBOfMutexHolder, ux_Original_اولویت )
#پایان_اگر

#اگر_تعریف_نشده trace_BLOCKING_ON_صف_RECEIVE

/* Task is about to block because it cannot read from a
 * queue/mutex/semaphore.  pxQueue is a pointer to the queue/mutex/semaphore
 * upon which the read was attempted.  pxCurrentTCB points to the TCB of the
 * task that attempted the read. */
    #تعریف_کن trace_BLOCKING_ON_صف_RECEIVE( px_صف )
#پایان_اگر

#اگر_تعریف_نشده trace_BLOCKING_ON_صف_PEEK

/* Task is about to block because it cannot read from a
 * queue/mutex/semaphore.  pxQueue is a pointer to the queue/mutex/semaphore
 * upon which the read was attempted.  pxCurrentTCB points to the TCB of the
 * task that attempted the read. */
    #تعریف_کن trace_BLOCKING_ON_صف_PEEK( px_صف )
#پایان_اگر

#اگر_تعریف_نشده trace_BLOCKING_ON_صف_SEND

/* Task is about to block because it cannot write to a
 * queue/mutex/semaphore.  pxQueue is a pointer to the queue/mutex/semaphore
 * upon which the write was attempted.  pxCurrentTCB points to the TCB of the
 * task that attempted the write. */
    #تعریف_کن trace_BLOCKING_ON_صف_SEND( px_صف )
#پایان_اگر

#اگر_تعریف_نشده config_CHECK_FOR_پشته_سرریز
    #تعریف_کن config_CHECK_FOR_پشته_سرریز    0
#پایان_اگر

#اگر_تعریف_نشده config_رکورد_پشته_بالا_آدرس
    #تعریف_کن config_رکورد_پشته_بالا_آدرس    0
#پایان_اگر

#اگر_تعریف_نشده configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H
    #تعریف_کن configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H    0
#پایان_اگر

/* The following event macros are embedded in the kernel API calls. */

#اگر_تعریف_نشده trace_MOVED_TASK_مقصد_READY_STATE
    #تعریف_کن trace_MOVED_TASK_مقصد_READY_STATE( pxTCB )
#پایان_اگر

#اگر_تعریف_نشده trace_POST_MOVED_TASK_مقصد_READY_STATE
    #تعریف_کن trace_POST_MOVED_TASK_مقصد_READY_STATE( pxTCB )
#پایان_اگر

#اگر_تعریف_نشده trace_MOVED_TASK_مقصد_DELAYED_LIST
    #تعریف_کن trace_MOVED_TASK_مقصد_DELAYED_LIST()
#پایان_اگر

#اگر_تعریف_نشده trace_MOVED_TASK_مقصد_سرریز_DELAYED_LIST
    #تعریف_کن trace_MOVED_TASK_مقصد_سرریز_DELAYED_LIST()
#پایان_اگر

#اگر_تعریف_نشده trace_صف_CREATE
    #تعریف_کن trace_صف_CREATE( px_New_صف )
#پایان_اگر

#اگر_تعریف_نشده trace_صف_CREATE_FAILED
    #تعریف_کن trace_صف_CREATE_FAILED( uc_صف_Type )
#پایان_اگر

#اگر_تعریف_نشده traceCREATE_MUTEX
    #تعریف_کن traceCREATE_MUTEX( px_New_صف )
#پایان_اگر

#اگر_تعریف_نشده traceCREATE_MUTEX_FAILED
    #تعریف_کن traceCREATE_MUTEX_FAILED()
#پایان_اگر

#اگر_تعریف_نشده traceGIVE_MUTEX_RECURSIVE
    #تعریف_کن traceGIVE_MUTEX_RECURSIVE( pxMutex )
#پایان_اگر

#اگر_تعریف_نشده traceGIVE_MUTEX_RECURSIVE_FAILED
    #تعریف_کن traceGIVE_MUTEX_RECURSIVE_FAILED( pxMutex )
#پایان_اگر

#اگر_تعریف_نشده traceTAKE_MUTEX_RECURSIVE
    #تعریف_کن traceTAKE_MUTEX_RECURSIVE( pxMutex )
#پایان_اگر

#اگر_تعریف_نشده traceTAKE_MUTEX_RECURSIVE_FAILED
    #تعریف_کن traceTAKE_MUTEX_RECURSIVE_FAILED( pxMutex )
#پایان_اگر

#اگر_تعریف_نشده traceCREATE_COUNTING_SEMAPHORE
    #تعریف_کن traceCREATE_COUNTING_SEMAPHORE()
#پایان_اگر

#اگر_تعریف_نشده traceCREATE_COUNTING_SEMAPHORE_FAILED
    #تعریف_کن traceCREATE_COUNTING_SEMAPHORE_FAILED()
#پایان_اگر

#اگر_تعریف_نشده trace_صف_مجموعه_SEND
    #تعریف_کن trace_صف_مجموعه_SEND    trace_صف_SEND
#پایان_اگر

#اگر_تعریف_نشده trace_صف_SEND
    #تعریف_کن trace_صف_SEND( px_صف )
#پایان_اگر

#اگر_تعریف_نشده trace_صف_SEND_FAILED
    #تعریف_کن trace_صف_SEND_FAILED( px_صف )
#پایان_اگر

#اگر_تعریف_نشده trace_صف_RECEIVE
    #تعریف_کن trace_صف_RECEIVE( px_صف )
#پایان_اگر

#اگر_تعریف_نشده trace_صف_PEEK
    #تعریف_کن trace_صف_PEEK( px_صف )
#پایان_اگر

#اگر_تعریف_نشده trace_صف_PEEK_FAILED
    #تعریف_کن trace_صف_PEEK_FAILED( px_صف )
#پایان_اگر

#اگر_تعریف_نشده trace_صف_PEEK_FROM_ISR
    #تعریف_کن trace_صف_PEEK_FROM_ISR( px_صف )
#پایان_اگر

#اگر_تعریف_نشده trace_صف_RECEIVE_FAILED
    #تعریف_کن trace_صف_RECEIVE_FAILED( px_صف )
#پایان_اگر

#اگر_تعریف_نشده trace_صف_SEND_FROM_ISR
    #تعریف_کن trace_صف_SEND_FROM_ISR( px_صف )
#پایان_اگر

#اگر_تعریف_نشده trace_صف_SEND_FROM_ISR_FAILED
    #تعریف_کن trace_صف_SEND_FROM_ISR_FAILED( px_صف )
#پایان_اگر

#اگر_تعریف_نشده trace_صف_RECEIVE_FROM_ISR
    #تعریف_کن trace_صف_RECEIVE_FROM_ISR( px_صف )
#پایان_اگر

#اگر_تعریف_نشده trace_صف_RECEIVE_FROM_ISR_FAILED
    #تعریف_کن trace_صف_RECEIVE_FROM_ISR_FAILED( px_صف )
#پایان_اگر

#اگر_تعریف_نشده trace_صف_PEEK_FROM_ISR_FAILED
    #تعریف_کن trace_صف_PEEK_FROM_ISR_FAILED( px_صف )
#پایان_اگر

#اگر_تعریف_نشده trace_صف_DELETE
    #تعریف_کن trace_صف_DELETE( px_صف )
#پایان_اگر

#اگر_تعریف_نشده traceTASK_CREATE
    #تعریف_کن traceTASK_CREATE( pxNewTCB )
#پایان_اگر

#اگر_تعریف_نشده traceTASK_CREATE_FAILED
    #تعریف_کن traceTASK_CREATE_FAILED()
#پایان_اگر

#اگر_تعریف_نشده traceTASK_DELETE
    #تعریف_کن traceTASK_DELETE( px_Task_مقصد_Delete )
#پایان_اگر

#اگر_تعریف_نشده traceTASK_DELAY_UNTIL
    #تعریف_کن traceTASK_DELAY_UNTIL( x )
#پایان_اگر

#اگر_تعریف_نشده traceTASK_DELAY
    #تعریف_کن traceTASK_DELAY()
#پایان_اگر

#اگر_تعریف_نشده trace_TASK_اولویت_مجموعه
    #تعریف_کن trace_TASK_اولویت_مجموعه( pxTask, ux_New_اولویت )
#پایان_اگر

#اگر_تعریف_نشده traceTASK_SUSPEND
    #تعریف_کن traceTASK_SUSPEND( px_Task_مقصد_Suspend )
#پایان_اگر

#اگر_تعریف_نشده traceTASK_RESUME
    #تعریف_کن traceTASK_RESUME( px_Task_مقصد_Resume )
#پایان_اگر

#اگر_تعریف_نشده traceTASK_RESUME_FROM_ISR
    #تعریف_کن traceTASK_RESUME_FROM_ISR( px_Task_مقصد_Resume )
#پایان_اگر

#اگر_تعریف_نشده traceTASK_INCREMENT_TICK
    #تعریف_کن traceTASK_INCREMENT_TICK( x_Tick_شمار )
#پایان_اگر

#اگر_تعریف_نشده traceTIMER_CREATE
    #تعریف_کن traceTIMER_CREATE( pxNewTimer )
#پایان_اگر

#اگر_تعریف_نشده traceTIMER_CREATE_FAILED
    #تعریف_کن traceTIMER_CREATE_FAILED()
#پایان_اگر

#اگر_تعریف_نشده traceTIMER_COMMAND_SEND
    #تعریف_کن traceTIMER_COMMAND_SEND( xTimer, x_پیام_ID, x_پیام_مقدار_مقدار, xReturn )
#پایان_اگر

#اگر_تعریف_نشده traceTIMER_EXPIRED
    #تعریف_کن traceTIMER_EXPIRED( pxTimer )
#پایان_اگر

#اگر_تعریف_نشده traceTIMER_COMMAND_RECEIVED
    #تعریف_کن traceTIMER_COMMAND_RECEIVED( pxTimer, x_پیام_ID, x_پیام_مقدار )
#پایان_اگر

#اگر_تعریف_نشده traceMALLOC
    #تعریف_کن traceMALLOC( pv_آدرس, ui_اندازه )
#پایان_اگر

#اگر_تعریف_نشده traceFREE
    #تعریف_کن traceFREE( pv_آدرس, ui_اندازه )
#پایان_اگر

#اگر_تعریف_نشده trace_EVENT_گروه_CREATE
    #تعریف_کن trace_EVENT_گروه_CREATE( x_Event_گروه )
#پایان_اگر

#اگر_تعریف_نشده trace_EVENT_گروه_CREATE_FAILED
    #تعریف_کن trace_EVENT_گروه_CREATE_FAILED()
#پایان_اگر

#اگر_تعریف_نشده trace_EVENT_گروه_SYNC_بلوک
    #تعریف_کن trace_EVENT_گروه_SYNC_بلوک( x_Event_گروه, ux_بیتها_مقصد_مجموعه, ux_بیتها_مقصد_Wait_For )
#پایان_اگر

#اگر_تعریف_نشده trace_EVENT_گروه_SYNC_انتها
    #تعریف_کن trace_EVENT_گروه_SYNC_انتها( x_Event_گروه, ux_بیتها_مقصد_مجموعه, ux_بیتها_مقصد_Wait_For, xTimeoutOccurred )    ( فراغ ) ( xTimeoutOccurred )
#پایان_اگر

#اگر_تعریف_نشده trace_EVENT_گروه_WAIT_بیتها_بلوک
    #تعریف_کن trace_EVENT_گروه_WAIT_بیتها_بلوک( x_Event_گروه, ux_بیتها_مقصد_Wait_For )
#پایان_اگر

#اگر_تعریف_نشده trace_EVENT_گروه_WAIT_بیتها_انتها
    #تعریف_کن trace_EVENT_گروه_WAIT_بیتها_انتها( x_Event_گروه, ux_بیتها_مقصد_Wait_For, xTimeoutOccurred )    ( فراغ ) ( xTimeoutOccurred )
#پایان_اگر

#اگر_تعریف_نشده trace_EVENT_گروه_CLEAR_بیتها
    #تعریف_کن trace_EVENT_گروه_CLEAR_بیتها( x_Event_گروه, ux_بیتها_مقصد_Clear )
#پایان_اگر

#اگر_تعریف_نشده trace_EVENT_گروه_CLEAR_بیتها_FROM_ISR
    #تعریف_کن trace_EVENT_گروه_CLEAR_بیتها_FROM_ISR( x_Event_گروه, ux_بیتها_مقصد_Clear )
#پایان_اگر

#اگر_تعریف_نشده trace_EVENT_گروه_مجموعه_بیتها
    #تعریف_کن trace_EVENT_گروه_مجموعه_بیتها( x_Event_گروه, ux_بیتها_مقصد_مجموعه )
#پایان_اگر

#اگر_تعریف_نشده trace_EVENT_گروه_مجموعه_بیتها_FROM_ISR
    #تعریف_کن trace_EVENT_گروه_مجموعه_بیتها_FROM_ISR( x_Event_گروه, ux_بیتها_مقصد_مجموعه )
#پایان_اگر

#اگر_تعریف_نشده trace_EVENT_گروه_DELETE
    #تعریف_کن trace_EVENT_گروه_DELETE( x_Event_گروه )
#پایان_اگر

#اگر_تعریف_نشده tracePEND_FUNC_CALL
    #تعریف_کن tracePEND_FUNC_CALL( x_Function_مقصد_Pend, pvParameter1, ulParameter2, ret )
#پایان_اگر

#اگر_تعریف_نشده tracePEND_FUNC_CALL_FROM_ISR
    #تعریف_کن tracePEND_FUNC_CALL_FROM_ISR( x_Function_مقصد_Pend, pvParameter1, ulParameter2, ret )
#پایان_اگر

#اگر_تعریف_نشده trace_صف_REGISTRY_جمع_کن
    #تعریف_کن trace_صف_REGISTRY_جمع_کن( x_صف, pc_صف_Name )
#پایان_اگر

#اگر_تعریف_نشده trace_TASK_NOTIFY_TAKE_بلوک
    #تعریف_کن trace_TASK_NOTIFY_TAKE_بلوک( ux_شاخص_مقصد_Wait )
#پایان_اگر

#اگر_تعریف_نشده traceTASK_NOTIFY_TAKE
    #تعریف_کن traceTASK_NOTIFY_TAKE( ux_شاخص_مقصد_Wait )
#پایان_اگر

#اگر_تعریف_نشده trace_TASK_NOTIFY_WAIT_بلوک
    #تعریف_کن trace_TASK_NOTIFY_WAIT_بلوک( ux_شاخص_مقصد_Wait )
#پایان_اگر

#اگر_تعریف_نشده traceTASK_NOTIFY_WAIT
    #تعریف_کن traceTASK_NOTIFY_WAIT( ux_شاخص_مقصد_Wait )
#پایان_اگر

#اگر_تعریف_نشده traceTASK_NOTIFY
    #تعریف_کن traceTASK_NOTIFY( ux_شاخص_مقصد_Notify )
#پایان_اگر

#اگر_تعریف_نشده traceTASK_NOTIFY_FROM_ISR
    #تعریف_کن traceTASK_NOTIFY_FROM_ISR( ux_شاخص_مقصد_Notify )
#پایان_اگر

#اگر_تعریف_نشده traceTASK_NOTIFY_GIVE_FROM_ISR
    #تعریف_کن traceTASK_NOTIFY_GIVE_FROM_ISR( ux_شاخص_مقصد_Notify )
#پایان_اگر

#اگر_تعریف_نشده trace_ISR_EXIT_مقصد_SCHEDULER
    #تعریف_کن trace_ISR_EXIT_مقصد_SCHEDULER()
#پایان_اگر

#اگر_تعریف_نشده traceISR_EXIT
    #تعریف_کن traceISR_EXIT()
#پایان_اگر

#اگر_تعریف_نشده traceISR_ENTER
    #تعریف_کن traceISR_ENTER()
#پایان_اگر

#اگر_تعریف_نشده trace_جریان_میانگیر_CREATE_FAILED
    #تعریف_کن trace_جریان_میانگیر_CREATE_FAILED( x_جریان_میانگیر_Type )
#پایان_اگر

#اگر_تعریف_نشده trace_جریان_میانگیر_CREATE_STATIC_FAILED
    #تعریف_کن trace_جریان_میانگیر_CREATE_STATIC_FAILED( xReturn, x_جریان_میانگیر_Type )
#پایان_اگر

#اگر_تعریف_نشده trace_جریان_میانگیر_CREATE
    #تعریف_کن trace_جریان_میانگیر_CREATE( px_جریان_میانگیر, x_جریان_میانگیر_Type )
#پایان_اگر

#اگر_تعریف_نشده trace_جریان_میانگیر_DELETE
    #تعریف_کن trace_جریان_میانگیر_DELETE( x_جریان_میانگیر )
#پایان_اگر

#اگر_تعریف_نشده trace_جریان_میانگیر_RESET
    #تعریف_کن trace_جریان_میانگیر_RESET( x_جریان_میانگیر )
#پایان_اگر

#اگر_تعریف_نشده trace_جریان_میانگیر_RESET_FROM_ISR
    #تعریف_کن trace_جریان_میانگیر_RESET_FROM_ISR( x_جریان_میانگیر )
#پایان_اگر

#اگر_تعریف_نشده trace_BLOCKING_ON_جریان_میانگیر_SEND
    #تعریف_کن trace_BLOCKING_ON_جریان_میانگیر_SEND( x_جریان_میانگیر )
#پایان_اگر

#اگر_تعریف_نشده trace_جریان_میانگیر_SEND
    #تعریف_کن trace_جریان_میانگیر_SEND( x_جریان_میانگیر, x_بایتها_فرستاده )
#پایان_اگر

#اگر_تعریف_نشده trace_جریان_میانگیر_SEND_FAILED
    #تعریف_کن trace_جریان_میانگیر_SEND_FAILED( x_جریان_میانگیر )
#پایان_اگر

#اگر_تعریف_نشده trace_جریان_میانگیر_SEND_FROM_ISR
    #تعریف_کن trace_جریان_میانگیر_SEND_FROM_ISR( x_جریان_میانگیر, x_بایتها_فرستاده )
#پایان_اگر

#اگر_تعریف_نشده trace_BLOCKING_ON_جریان_میانگیر_RECEIVE
    #تعریف_کن trace_BLOCKING_ON_جریان_میانگیر_RECEIVE( x_جریان_میانگیر )
#پایان_اگر

#اگر_تعریف_نشده trace_جریان_میانگیر_RECEIVE
    #تعریف_کن trace_جریان_میانگیر_RECEIVE( x_جریان_میانگیر, x_Received_درازی )
#پایان_اگر

#اگر_تعریف_نشده trace_جریان_میانگیر_RECEIVE_FAILED
    #تعریف_کن trace_جریان_میانگیر_RECEIVE_FAILED( x_جریان_میانگیر )
#پایان_اگر

#اگر_تعریف_نشده trace_جریان_میانگیر_RECEIVE_FROM_ISR
    #تعریف_کن trace_جریان_میانگیر_RECEIVE_FROM_ISR( x_جریان_میانگیر, x_Received_درازی )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_Event_گروه_Create_Static
    #تعریف_کن trace_ENTER_x_Event_گروه_Create_Static( px_Event_گروه_میانگیر )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_Event_گروه_Create_Static
    #تعریف_کن trace_RETURN_x_Event_گروه_Create_Static( px_Event_بیتها )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_Event_گروه_Create
    #تعریف_کن trace_ENTER_x_Event_گروه_Create()
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_Event_گروه_Create
    #تعریف_کن trace_RETURN_x_Event_گروه_Create( px_Event_بیتها )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_Event_گروه_Sync
    #تعریف_کن trace_ENTER_x_Event_گروه_Sync( x_Event_گروه, ux_بیتها_مقصد_مجموعه, ux_بیتها_مقصد_Wait_For, x_Ticks_مقصد_Wait )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_Event_گروه_Sync
    #تعریف_کن trace_RETURN_x_Event_گروه_Sync( uxReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_Event_گروه_Wait_بیتها
    #تعریف_کن trace_ENTER_x_Event_گروه_Wait_بیتها( x_Event_گروه, ux_بیتها_مقصد_Wait_For, xClearOnExit, x_Wait_For_All_بیتها, x_Ticks_مقصد_Wait )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_Event_گروه_Wait_بیتها
    #تعریف_کن trace_RETURN_x_Event_گروه_Wait_بیتها( uxReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_Event_گروه_Clear_بیتها
    #تعریف_کن trace_ENTER_x_Event_گروه_Clear_بیتها( x_Event_گروه, ux_بیتها_مقصد_Clear )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_Event_گروه_Clear_بیتها
    #تعریف_کن trace_RETURN_x_Event_گروه_Clear_بیتها( uxReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_Event_گروه_Clear_بیتها_From_ISR
    #تعریف_کن trace_ENTER_x_Event_گروه_Clear_بیتها_From_ISR( x_Event_گروه, ux_بیتها_مقصد_Clear )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_Event_گروه_Clear_بیتها_From_ISR
    #تعریف_کن trace_RETURN_x_Event_گروه_Clear_بیتها_From_ISR( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_Event_گروه_Get_بیتها_From_ISR
    #تعریف_کن trace_ENTER_x_Event_گروه_Get_بیتها_From_ISR( x_Event_گروه )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_Event_گروه_Get_بیتها_From_ISR
    #تعریف_کن trace_RETURN_x_Event_گروه_Get_بیتها_From_ISR( uxReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_Event_گروه_مجموعه_بیتها
    #تعریف_کن trace_ENTER_x_Event_گروه_مجموعه_بیتها( x_Event_گروه, ux_بیتها_مقصد_مجموعه )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_Event_گروه_مجموعه_بیتها
    #تعریف_کن trace_RETURN_x_Event_گروه_مجموعه_بیتها( ux_Event_بیتها )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_v_Event_گروه_Delete
    #تعریف_کن trace_ENTER_v_Event_گروه_Delete( x_Event_گروه )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_v_Event_گروه_Delete
    #تعریف_کن trace_RETURN_v_Event_گروه_Delete()
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_Event_گروه_Get_Static_میانگیر
    #تعریف_کن trace_ENTER_x_Event_گروه_Get_Static_میانگیر( x_Event_گروه, ppx_Event_گروه_میانگیر )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_Event_گروه_Get_Static_میانگیر
    #تعریف_کن trace_RETURN_x_Event_گروه_Get_Static_میانگیر( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_v_Event_گروه_مجموعه_بیتها_Callback
    #تعریف_کن trace_ENTER_v_Event_گروه_مجموعه_بیتها_Callback( pv_Event_گروه, ul_بیتها_مقصد_مجموعه )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_v_Event_گروه_مجموعه_بیتها_Callback
    #تعریف_کن trace_RETURN_v_Event_گروه_مجموعه_بیتها_Callback()
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_v_Event_گروه_Clear_بیتها_Callback
    #تعریف_کن trace_ENTER_v_Event_گروه_Clear_بیتها_Callback( pv_Event_گروه, ul_بیتها_مقصد_Clear )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_v_Event_گروه_Clear_بیتها_Callback
    #تعریف_کن trace_RETURN_v_Event_گروه_Clear_بیتها_Callback()
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_Event_گروه_مجموعه_بیتها_From_ISR
    #تعریف_کن trace_ENTER_x_Event_گروه_مجموعه_بیتها_From_ISR( x_Event_گروه, ux_بیتها_مقصد_مجموعه, px_Higher_اولویت_Task_Woken )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_Event_گروه_مجموعه_بیتها_From_ISR
    #تعریف_کن trace_RETURN_x_Event_گروه_مجموعه_بیتها_From_ISR( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_ux_Event_گروه_Get_عدد
    #تعریف_کن trace_ENTER_ux_Event_گروه_Get_عدد( x_Event_گروه )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_ux_Event_گروه_Get_عدد
    #تعریف_کن trace_RETURN_ux_Event_گروه_Get_عدد( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_v_Event_گروه_مجموعه_عدد
    #تعریف_کن trace_ENTER_v_Event_گروه_مجموعه_عدد( x_Event_گروه, ux_Event_گروه_عدد )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_v_Event_گروه_مجموعه_عدد
    #تعریف_کن trace_RETURN_v_Event_گروه_مجموعه_عدد()
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_صف_Generic_Reset
    #تعریف_کن trace_ENTER_x_صف_Generic_Reset( x_صف, x_New_صف )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_صف_Generic_Reset
    #تعریف_کن trace_RETURN_x_صف_Generic_Reset( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_صف_Generic_Create_Static
    #تعریف_کن trace_ENTER_x_صف_Generic_Create_Static( ux_صف_درازی, ux_قلم_اندازه, puc_صف_Storage, px_Static_صف, uc_صف_Type )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_صف_Generic_Create_Static
    #تعریف_کن trace_RETURN_x_صف_Generic_Create_Static( px_New_صف )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_صف_Generic_Get_Static_Buffers
    #تعریف_کن trace_ENTER_x_صف_Generic_Get_Static_Buffers( x_صف, ppuc_صف_Storage, ppx_Static_صف )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_صف_Generic_Get_Static_Buffers
    #تعریف_کن trace_RETURN_x_صف_Generic_Get_Static_Buffers( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_صف_Generic_Create
    #تعریف_کن trace_ENTER_x_صف_Generic_Create( ux_صف_درازی, ux_قلم_اندازه, uc_صف_Type )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_صف_Generic_Create
    #تعریف_کن trace_RETURN_x_صف_Generic_Create( px_New_صف )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_صف_Create_Mutex
    #تعریف_کن trace_ENTER_x_صف_Create_Mutex( uc_صف_Type )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_صف_Create_Mutex
    #تعریف_کن trace_RETURN_x_صف_Create_Mutex( x_New_صف )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_صف_Create_Mutex_Static
    #تعریف_کن trace_ENTER_x_صف_Create_Mutex_Static( uc_صف_Type, px_Static_صف )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_صف_Create_Mutex_Static
    #تعریف_کن trace_RETURN_x_صف_Create_Mutex_Static( x_New_صف )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_صف_Get_Mutex_Holder
    #تعریف_کن trace_ENTER_x_صف_Get_Mutex_Holder( xSemaphore )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_صف_Get_Mutex_Holder
    #تعریف_کن trace_RETURN_x_صف_Get_Mutex_Holder( pxReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_صف_Get_Mutex_Holder_From_ISR
    #تعریف_کن trace_ENTER_x_صف_Get_Mutex_Holder_From_ISR( xSemaphore )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_صف_Get_Mutex_Holder_From_ISR
    #تعریف_کن trace_RETURN_x_صف_Get_Mutex_Holder_From_ISR( pxReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_صف_Give_Mutex_Recursive
    #تعریف_کن trace_ENTER_x_صف_Give_Mutex_Recursive( xMutex )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_صف_Give_Mutex_Recursive
    #تعریف_کن trace_RETURN_x_صف_Give_Mutex_Recursive( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_صف_Take_Mutex_Recursive
    #تعریف_کن trace_ENTER_x_صف_Take_Mutex_Recursive( xMutex, x_Ticks_مقصد_Wait )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_صف_Take_Mutex_Recursive
    #تعریف_کن trace_RETURN_x_صف_Take_Mutex_Recursive( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_صف_Create_Counting_Semaphore_Static
    #تعریف_کن trace_ENTER_x_صف_Create_Counting_Semaphore_Static( ux_Max_شمار, ux_Initial_شمار, px_Static_صف )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_صف_Create_Counting_Semaphore_Static
    #تعریف_کن trace_RETURN_x_صف_Create_Counting_Semaphore_Static( xHandle )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_صف_Create_Counting_Semaphore
    #تعریف_کن trace_ENTER_x_صف_Create_Counting_Semaphore( ux_Max_شمار, ux_Initial_شمار )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_صف_Create_Counting_Semaphore
    #تعریف_کن trace_RETURN_x_صف_Create_Counting_Semaphore( xHandle )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_صف_Generic_Send
    #تعریف_کن trace_ENTER_x_صف_Generic_Send( x_صف, pv_قلم_مقصد_صف, x_Ticks_مقصد_Wait, xCopyPosition )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_صف_Generic_Send
    #تعریف_کن trace_RETURN_x_صف_Generic_Send( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_صف_Generic_Send_From_ISR
    #تعریف_کن trace_ENTER_x_صف_Generic_Send_From_ISR( x_صف, pv_قلم_مقصد_صف, px_Higher_اولویت_Task_Woken, xCopyPosition )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_صف_Generic_Send_From_ISR
    #تعریف_کن trace_RETURN_x_صف_Generic_Send_From_ISR( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_صف_Give_From_ISR
    #تعریف_کن trace_ENTER_x_صف_Give_From_ISR( x_صف, px_Higher_اولویت_Task_Woken )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_صف_Give_From_ISR
    #تعریف_کن trace_RETURN_x_صف_Give_From_ISR( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_صف_Receive
    #تعریف_کن trace_ENTER_x_صف_Receive( x_صف, pv_میانگیر, x_Ticks_مقصد_Wait )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_صف_Receive
    #تعریف_کن trace_RETURN_x_صف_Receive( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_صف_Semaphore_Take
    #تعریف_کن trace_ENTER_x_صف_Semaphore_Take( x_صف, x_Ticks_مقصد_Wait )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_صف_Semaphore_Take
    #تعریف_کن trace_RETURN_x_صف_Semaphore_Take( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_صف_Peek
    #تعریف_کن trace_ENTER_x_صف_Peek( x_صف, pv_میانگیر, x_Ticks_مقصد_Wait )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_صف_Peek
    #تعریف_کن trace_RETURN_x_صف_Peek( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_صف_Receive_From_ISR
    #تعریف_کن trace_ENTER_x_صف_Receive_From_ISR( x_صف, pv_میانگیر, px_Higher_اولویت_Task_Woken )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_صف_Receive_From_ISR
    #تعریف_کن trace_RETURN_x_صف_Receive_From_ISR( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_صف_Peek_From_ISR
    #تعریف_کن trace_ENTER_x_صف_Peek_From_ISR( x_صف, pv_میانگیر )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_صف_Peek_From_ISR
    #تعریف_کن trace_RETURN_x_صف_Peek_From_ISR( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_ux_صف_Messages_انتظار
    #تعریف_کن trace_ENTER_ux_صف_Messages_انتظار( x_صف )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_ux_صف_Messages_انتظار
    #تعریف_کن trace_RETURN_ux_صف_Messages_انتظار( uxReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_ux_صف_Spaces_موجود
    #تعریف_کن trace_ENTER_ux_صف_Spaces_موجود( x_صف )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_ux_صف_Spaces_موجود
    #تعریف_کن trace_RETURN_ux_صف_Spaces_موجود( uxReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_ux_صف_Messages_انتظار_From_ISR
    #تعریف_کن trace_ENTER_ux_صف_Messages_انتظار_From_ISR( x_صف )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_ux_صف_Messages_انتظار_From_ISR
    #تعریف_کن trace_RETURN_ux_صف_Messages_انتظار_From_ISR( uxReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_v_صف_Delete
    #تعریف_کن trace_ENTER_v_صف_Delete( x_صف )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_v_صف_Delete
    #تعریف_کن trace_RETURN_v_صف_Delete()
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_ux_صف_Get_صف_عدد
    #تعریف_کن trace_ENTER_ux_صف_Get_صف_عدد( x_صف )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_ux_صف_Get_صف_عدد
    #تعریف_کن trace_RETURN_ux_صف_Get_صف_عدد( ux_صف_عدد )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_v_صف_مجموعه_صف_عدد
    #تعریف_کن trace_ENTER_v_صف_مجموعه_صف_عدد( x_صف, ux_صف_عدد )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_v_صف_مجموعه_صف_عدد
    #تعریف_کن trace_RETURN_v_صف_مجموعه_صف_عدد()
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_uc_صف_Get_صف_Type
    #تعریف_کن trace_ENTER_uc_صف_Get_صف_Type( x_صف )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_uc_صف_Get_صف_Type
    #تعریف_کن trace_RETURN_uc_صف_Get_صف_Type( uc_صف_Type )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_ux_صف_Get_صف_قلم_اندازه
    #تعریف_کن trace_ENTER_ux_صف_Get_صف_قلم_اندازه( x_صف )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_ux_صف_Get_صف_قلم_اندازه
    #تعریف_کن trace_RETURN_ux_صف_Get_صف_قلم_اندازه( ux_قلم_اندازه )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_ux_صف_Get_صف_درازی
    #تعریف_کن trace_ENTER_ux_صف_Get_صف_درازی( x_صف )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_ux_صف_Get_صف_درازی
    #تعریف_کن trace_RETURN_ux_صف_Get_صف_درازی( ux_درازی )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_صف_Is_صف_خالی_From_ISR
    #تعریف_کن trace_ENTER_x_صف_Is_صف_خالی_From_ISR( x_صف )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_صف_Is_صف_خالی_From_ISR
    #تعریف_کن trace_RETURN_x_صف_Is_صف_خالی_From_ISR( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_صف_Is_صف_پر_From_ISR
    #تعریف_کن trace_ENTER_x_صف_Is_صف_پر_From_ISR( x_صف )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_صف_Is_صف_پر_From_ISR
    #تعریف_کن trace_RETURN_x_صف_Is_صف_پر_From_ISR( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_صف_CRSend
    #تعریف_کن trace_ENTER_x_صف_CRSend( x_صف, pv_قلم_مقصد_صف, x_Ticks_مقصد_Wait )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_صف_CRSend
    #تعریف_کن trace_RETURN_x_صف_CRSend( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_صف_CRReceive
    #تعریف_کن trace_ENTER_x_صف_CRReceive( x_صف, pv_میانگیر, x_Ticks_مقصد_Wait )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_صف_CRReceive
    #تعریف_کن trace_RETURN_x_صف_CRReceive( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_صف_CRSend_From_ISR
    #تعریف_کن trace_ENTER_x_صف_CRSend_From_ISR( x_صف, pv_قلم_مقصد_صف, xCoRoutinePreviouslyWoken )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_صف_CRSend_From_ISR
    #تعریف_کن trace_RETURN_x_صف_CRSend_From_ISR( xCoRoutinePreviouslyWoken )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_صف_CRReceive_From_ISR
    #تعریف_کن trace_ENTER_x_صف_CRReceive_From_ISR( x_صف, pv_میانگیر, pxCoRoutineWoken )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_صف_CRReceive_From_ISR
    #تعریف_کن trace_RETURN_x_صف_CRReceive_From_ISR( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_v_صف_جمع_کن_مقصد_Registry
    #تعریف_کن trace_ENTER_v_صف_جمع_کن_مقصد_Registry( x_صف, pc_صف_Name )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_v_صف_جمع_کن_مقصد_Registry
    #تعریف_کن trace_RETURN_v_صف_جمع_کن_مقصد_Registry()
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_pc_صف_Get_Name
    #تعریف_کن trace_ENTER_pc_صف_Get_Name( x_صف )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_pc_صف_Get_Name
    #تعریف_کن trace_RETURN_pc_صف_Get_Name( pcReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_v_صف_Unregister_صف
    #تعریف_کن trace_ENTER_v_صف_Unregister_صف( x_صف )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_v_صف_Unregister_صف
    #تعریف_کن trace_RETURN_v_صف_Unregister_صف()
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_v_صف_Wait_For_پیام_Restricted
    #تعریف_کن trace_ENTER_v_صف_Wait_For_پیام_Restricted( x_صف, x_Ticks_مقصد_Wait, xWaitIndefinitely )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_v_صف_Wait_For_پیام_Restricted
    #تعریف_کن trace_RETURN_v_صف_Wait_For_پیام_Restricted()
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_صف_Create_مجموعه
    #تعریف_کن trace_ENTER_x_صف_Create_مجموعه( ux_Event_صف_درازی )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_صف_Create_مجموعه
    #تعریف_کن trace_RETURN_x_صف_Create_مجموعه( px_صف )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_صف_جمع_کن_مقصد_مجموعه
    #تعریف_کن trace_ENTER_x_صف_جمع_کن_مقصد_مجموعه( x_صف_Or_Semaphore, x_صف_مجموعه )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_صف_جمع_کن_مقصد_مجموعه
    #تعریف_کن trace_RETURN_x_صف_جمع_کن_مقصد_مجموعه( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_صف_Remove_From_مجموعه
    #تعریف_کن trace_ENTER_x_صف_Remove_From_مجموعه( x_صف_Or_Semaphore, x_صف_مجموعه )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_صف_Remove_From_مجموعه
    #تعریف_کن trace_RETURN_x_صف_Remove_From_مجموعه( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_صف_Select_From_مجموعه
    #تعریف_کن trace_ENTER_x_صف_Select_From_مجموعه( x_صف_مجموعه, x_Ticks_مقصد_Wait )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_صف_Select_From_مجموعه
    #تعریف_کن trace_RETURN_x_صف_Select_From_مجموعه( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_صف_Select_From_مجموعه_From_ISR
    #تعریف_کن trace_ENTER_x_صف_Select_From_مجموعه_From_ISR( x_صف_مجموعه )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_صف_Select_From_مجموعه_From_ISR
    #تعریف_کن trace_RETURN_x_صف_Select_From_مجموعه_From_ISR( xReturn )
#پایان_اگر

#اگر_تعریف_نشده traceENTER_xTimerCreateTimerTask
    #تعریف_کن traceENTER_xTimerCreateTimerTask()
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_xTimerCreateTimerTask
    #تعریف_کن traceRETURN_xTimerCreateTimerTask( xReturn )
#پایان_اگر

#اگر_تعریف_نشده traceENTER_xTimerCreate
    #تعریف_کن traceENTER_xTimerCreate( pcTimerName, x_Timer_Period_درون_Ticks, xAutoReload, pvTimerID, pxCallbackFunction )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_xTimerCreate
    #تعریف_کن traceRETURN_xTimerCreate( pxNewTimer )
#پایان_اگر

#اگر_تعریف_نشده traceENTER_xTimerCreateStatic
    #تعریف_کن traceENTER_xTimerCreateStatic( pcTimerName, x_Timer_Period_درون_Ticks, xAutoReload, pvTimerID, pxCallbackFunction, px_Timer_میانگیر )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_xTimerCreateStatic
    #تعریف_کن traceRETURN_xTimerCreateStatic( pxNewTimer )
#پایان_اگر

#اگر_تعریف_نشده traceENTER_xTimerGenericCommandFromTask
    #تعریف_کن traceENTER_xTimerGenericCommandFromTask( xTimer, xCommandID, x_Optional_مقدار, px_Higher_اولویت_Task_Woken, x_Ticks_مقصد_Wait )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_xTimerGenericCommandFromTask
    #تعریف_کن traceRETURN_xTimerGenericCommandFromTask( xReturn )
#پایان_اگر

#اگر_تعریف_نشده traceENTER_xTimerGenericCommandFromISR
    #تعریف_کن traceENTER_xTimerGenericCommandFromISR( xTimer, xCommandID, x_Optional_مقدار, px_Higher_اولویت_Task_Woken, x_Ticks_مقصد_Wait )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_xTimerGenericCommandFromISR
    #تعریف_کن traceRETURN_xTimerGenericCommandFromISR( xReturn )
#پایان_اگر

#اگر_تعریف_نشده traceENTER_xTimerGetTimerDaemonTaskHandle
    #تعریف_کن traceENTER_xTimerGetTimerDaemonTaskHandle()
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_xTimerGetTimerDaemonTaskHandle
    #تعریف_کن traceRETURN_xTimerGetTimerDaemonTaskHandle( xTimerTaskHandle )
#پایان_اگر

#اگر_تعریف_نشده traceENTER_xTimerGetPeriod
    #تعریف_کن traceENTER_xTimerGetPeriod( xTimer )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_xTimerGetPeriod
    #تعریف_کن traceRETURN_xTimerGetPeriod( x_Timer_Period_درون_Ticks )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_v_Timer_مجموعه_Reload_نما
    #تعریف_کن trace_ENTER_v_Timer_مجموعه_Reload_نما( xTimer, xAutoReload )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_v_Timer_مجموعه_Reload_نما
    #تعریف_کن trace_RETURN_v_Timer_مجموعه_Reload_نما()
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_Timer_Get_Reload_نما
    #تعریف_کن trace_ENTER_x_Timer_Get_Reload_نما( xTimer )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_Timer_Get_Reload_نما
    #تعریف_کن trace_RETURN_x_Timer_Get_Reload_نما( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_ux_Timer_Get_Reload_نما
    #تعریف_کن trace_ENTER_ux_Timer_Get_Reload_نما( xTimer )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_ux_Timer_Get_Reload_نما
    #تعریف_کن trace_RETURN_ux_Timer_Get_Reload_نما( uxReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_Timer_Get_Expiry_زمان
    #تعریف_کن trace_ENTER_x_Timer_Get_Expiry_زمان( xTimer )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_Timer_Get_Expiry_زمان
    #تعریف_کن trace_RETURN_x_Timer_Get_Expiry_زمان( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_Timer_Get_Static_میانگیر
    #تعریف_کن trace_ENTER_x_Timer_Get_Static_میانگیر( xTimer, ppx_Timer_میانگیر )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_Timer_Get_Static_میانگیر
    #تعریف_کن trace_RETURN_x_Timer_Get_Static_میانگیر( xReturn )
#پایان_اگر

#اگر_تعریف_نشده traceENTER_pcTimerGetName
    #تعریف_کن traceENTER_pcTimerGetName( xTimer )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_pcTimerGetName
    #تعریف_کن traceRETURN_pcTimerGetName( pcTimerName )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_Timer_Is_Timer_فعال
    #تعریف_کن trace_ENTER_x_Timer_Is_Timer_فعال( xTimer )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_Timer_Is_Timer_فعال
    #تعریف_کن trace_RETURN_x_Timer_Is_Timer_فعال( xReturn )
#پایان_اگر

#اگر_تعریف_نشده traceENTER_pvTimerGetTimerID
    #تعریف_کن traceENTER_pvTimerGetTimerID( xTimer )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_pvTimerGetTimerID
    #تعریف_کن traceRETURN_pvTimerGetTimerID( pvReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_v_Timer_مجموعه_Timer_ID
    #تعریف_کن trace_ENTER_v_Timer_مجموعه_Timer_ID( xTimer, pvNewID )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_v_Timer_مجموعه_Timer_ID
    #تعریف_کن trace_RETURN_v_Timer_مجموعه_Timer_ID()
#پایان_اگر

#اگر_تعریف_نشده traceENTER_xTimerPendFunctionCallFromISR
    #تعریف_کن traceENTER_xTimerPendFunctionCallFromISR( x_Function_مقصد_Pend, pvParameter1, ulParameter2, px_Higher_اولویت_Task_Woken )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_xTimerPendFunctionCallFromISR
    #تعریف_کن traceRETURN_xTimerPendFunctionCallFromISR( xReturn )
#پایان_اگر

#اگر_تعریف_نشده traceENTER_xTimerPendFunctionCall
    #تعریف_کن traceENTER_xTimerPendFunctionCall( x_Function_مقصد_Pend, pvParameter1, ulParameter2, x_Ticks_مقصد_Wait )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_xTimerPendFunctionCall
    #تعریف_کن traceRETURN_xTimerPendFunctionCall( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_ux_Timer_Get_Timer_عدد
    #تعریف_کن trace_ENTER_ux_Timer_Get_Timer_عدد( xTimer )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_ux_Timer_Get_Timer_عدد
    #تعریف_کن trace_RETURN_ux_Timer_Get_Timer_عدد( ux_Timer_عدد )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_v_Timer_مجموعه_Timer_عدد
    #تعریف_کن trace_ENTER_v_Timer_مجموعه_Timer_عدد( xTimer, ux_Timer_عدد )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_v_Timer_مجموعه_Timer_عدد
    #تعریف_کن trace_RETURN_v_Timer_مجموعه_Timer_عدد()
#پایان_اگر

#اگر_تعریف_نشده traceENTER_xTaskCreateStatic
    #تعریف_کن traceENTER_xTaskCreateStatic( px_Task_کد, pcName, ux_پشته_عمق, pvParameters, ux_اولویت, pux_پشته_میانگیر, px_Task_میانگیر )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_xTaskCreateStatic
    #تعریف_کن traceRETURN_xTaskCreateStatic( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_Task_Create_Static_Affinity_مجموعه
    #تعریف_کن trace_ENTER_x_Task_Create_Static_Affinity_مجموعه( px_Task_کد, pcName, ux_پشته_عمق, pvParameters, ux_اولویت, pux_پشته_میانگیر, px_Task_میانگیر, ux_Core_Affinity_ماسک )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_Task_Create_Static_Affinity_مجموعه
    #تعریف_کن trace_RETURN_x_Task_Create_Static_Affinity_مجموعه( xReturn )
#پایان_اگر

#اگر_تعریف_نشده traceENTER_xTaskCreateRestrictedStatic
    #تعریف_کن traceENTER_xTaskCreateRestrictedStatic( pxTaskDefinition, pxCreatedTask )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_xTaskCreateRestrictedStatic
    #تعریف_کن traceRETURN_xTaskCreateRestrictedStatic( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_Task_Create_Restricted_Static_Affinity_مجموعه
    #تعریف_کن trace_ENTER_x_Task_Create_Restricted_Static_Affinity_مجموعه( pxTaskDefinition, ux_Core_Affinity_ماسک, pxCreatedTask )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_Task_Create_Restricted_Static_Affinity_مجموعه
    #تعریف_کن trace_RETURN_x_Task_Create_Restricted_Static_Affinity_مجموعه( xReturn )
#پایان_اگر

#اگر_تعریف_نشده traceENTER_xTaskCreateRestricted
    #تعریف_کن traceENTER_xTaskCreateRestricted( pxTaskDefinition, pxCreatedTask )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_xTaskCreateRestricted
    #تعریف_کن traceRETURN_xTaskCreateRestricted( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_Task_Create_Restricted_Affinity_مجموعه
    #تعریف_کن trace_ENTER_x_Task_Create_Restricted_Affinity_مجموعه( pxTaskDefinition, ux_Core_Affinity_ماسک, pxCreatedTask )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_Task_Create_Restricted_Affinity_مجموعه
    #تعریف_کن trace_RETURN_x_Task_Create_Restricted_Affinity_مجموعه( xReturn )
#پایان_اگر

#اگر_تعریف_نشده traceENTER_xTaskCreate
    #تعریف_کن traceENTER_xTaskCreate( px_Task_کد, pcName, ux_پشته_عمق, pvParameters, ux_اولویت, pxCreatedTask )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_xTaskCreate
    #تعریف_کن traceRETURN_xTaskCreate( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_Task_Create_Affinity_مجموعه
    #تعریف_کن trace_ENTER_x_Task_Create_Affinity_مجموعه( px_Task_کد, pcName, ux_پشته_عمق, pvParameters, ux_اولویت, ux_Core_Affinity_ماسک, pxCreatedTask )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_Task_Create_Affinity_مجموعه
    #تعریف_کن trace_RETURN_x_Task_Create_Affinity_مجموعه( xReturn )
#پایان_اگر

#اگر_تعریف_نشده traceENTER_vTaskDelete
    #تعریف_کن traceENTER_vTaskDelete( x_Task_مقصد_Delete )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_vTaskDelete
    #تعریف_کن traceRETURN_vTaskDelete()
#پایان_اگر

#اگر_تعریف_نشده traceENTER_xTaskDelayUntil
    #تعریف_کن traceENTER_xTaskDelayUntil( px_قبلی_Wake_زمان, x_زمان_Increment )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_xTaskDelayUntil
    #تعریف_کن traceRETURN_xTaskDelayUntil( xShouldDelay )
#پایان_اگر

#اگر_تعریف_نشده traceENTER_vTaskDelay
    #تعریف_کن traceENTER_vTaskDelay( x_Ticks_مقصد_Delay )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_vTaskDelay
    #تعریف_کن traceRETURN_vTaskDelay()
#پایان_اگر

#اگر_تعریف_نشده traceENTER_eTaskGetState
    #تعریف_کن traceENTER_eTaskGetState( xTask )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_eTaskGetState
    #تعریف_کن traceRETURN_eTaskGetState( eReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_ux_Task_اولویت_Get
    #تعریف_کن trace_ENTER_ux_Task_اولویت_Get( xTask )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_ux_Task_اولویت_Get
    #تعریف_کن trace_RETURN_ux_Task_اولویت_Get( uxReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_ux_Task_اولویت_Get_From_ISR
    #تعریف_کن trace_ENTER_ux_Task_اولویت_Get_From_ISR( xTask )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_ux_Task_اولویت_Get_From_ISR
    #تعریف_کن trace_RETURN_ux_Task_اولویت_Get_From_ISR( uxReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_ux_Task_پایه_اولویت_Get
    #تعریف_کن trace_ENTER_ux_Task_پایه_اولویت_Get( xTask )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_ux_Task_پایه_اولویت_Get
    #تعریف_کن trace_RETURN_ux_Task_پایه_اولویت_Get( uxReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_ux_Task_پایه_اولویت_Get_From_ISR
    #تعریف_کن trace_ENTER_ux_Task_پایه_اولویت_Get_From_ISR( xTask )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_ux_Task_پایه_اولویت_Get_From_ISR
    #تعریف_کن trace_RETURN_ux_Task_پایه_اولویت_Get_From_ISR( uxReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_v_Task_اولویت_مجموعه
    #تعریف_کن trace_ENTER_v_Task_اولویت_مجموعه( xTask, ux_New_اولویت )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_v_Task_اولویت_مجموعه
    #تعریف_کن trace_RETURN_v_Task_اولویت_مجموعه()
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_v_Task_Core_Affinity_مجموعه
    #تعریف_کن trace_ENTER_v_Task_Core_Affinity_مجموعه( xTask, ux_Core_Affinity_ماسک )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_v_Task_Core_Affinity_مجموعه
    #تعریف_کن trace_RETURN_v_Task_Core_Affinity_مجموعه()
#پایان_اگر

#اگر_تعریف_نشده traceENTER_vTaskCoreAffinityGet
    #تعریف_کن traceENTER_vTaskCoreAffinityGet( xTask )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_vTaskCoreAffinityGet
    #تعریف_کن traceRETURN_vTaskCoreAffinityGet( ux_Core_Affinity_ماسک )
#پایان_اگر

#اگر_تعریف_نشده traceENTER_vTaskPreemptionDisable
    #تعریف_کن traceENTER_vTaskPreemptionDisable( xTask )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_vTaskPreemptionDisable
    #تعریف_کن traceRETURN_vTaskPreemptionDisable()
#پایان_اگر

#اگر_تعریف_نشده traceENTER_vTaskPreemptionEnable
    #تعریف_کن traceENTER_vTaskPreemptionEnable( xTask )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_vTaskPreemptionEnable
    #تعریف_کن traceRETURN_vTaskPreemptionEnable()
#پایان_اگر

#اگر_تعریف_نشده traceENTER_vTaskSuspend
    #تعریف_کن traceENTER_vTaskSuspend( x_Task_مقصد_Suspend )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_vTaskSuspend
    #تعریف_کن traceRETURN_vTaskSuspend()
#پایان_اگر

#اگر_تعریف_نشده traceENTER_vTaskResume
    #تعریف_کن traceENTER_vTaskResume( x_Task_مقصد_Resume )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_vTaskResume
    #تعریف_کن traceRETURN_vTaskResume()
#پایان_اگر

#اگر_تعریف_نشده traceENTER_xTaskResumeFromISR
    #تعریف_کن traceENTER_xTaskResumeFromISR( x_Task_مقصد_Resume )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_xTaskResumeFromISR
    #تعریف_کن traceRETURN_xTaskResumeFromISR( xYieldRequired )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_v_Task_شروع_Scheduler
    #تعریف_کن trace_ENTER_v_Task_شروع_Scheduler()
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_v_Task_شروع_Scheduler
    #تعریف_کن trace_RETURN_v_Task_شروع_Scheduler()
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_v_Task_انتها_Scheduler
    #تعریف_کن trace_ENTER_v_Task_انتها_Scheduler()
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_v_Task_انتها_Scheduler
    #تعریف_کن trace_RETURN_v_Task_انتها_Scheduler()
#پایان_اگر

#اگر_تعریف_نشده traceENTER_vTaskSuspendAll
    #تعریف_کن traceENTER_vTaskSuspendAll()
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_vTaskSuspendAll
    #تعریف_کن traceRETURN_vTaskSuspendAll()
#پایان_اگر

#اگر_تعریف_نشده traceENTER_xTaskResumeAll
    #تعریف_کن traceENTER_xTaskResumeAll()
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_xTaskResumeAll
    #تعریف_کن traceRETURN_xTaskResumeAll( xAlreadyYielded )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_Task_Get_Tick_شمار
    #تعریف_کن trace_ENTER_x_Task_Get_Tick_شمار()
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_Task_Get_Tick_شمار
    #تعریف_کن trace_RETURN_x_Task_Get_Tick_شمار( xTicks )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_Task_Get_Tick_شمار_From_ISR
    #تعریف_کن trace_ENTER_x_Task_Get_Tick_شمار_From_ISR()
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_Task_Get_Tick_شمار_From_ISR
    #تعریف_کن trace_RETURN_x_Task_Get_Tick_شمار_From_ISR( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_ux_Task_Get_عدد_از_Tasks
    #تعریف_کن trace_ENTER_ux_Task_Get_عدد_از_Tasks()
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_ux_Task_Get_عدد_از_Tasks
    #تعریف_کن trace_RETURN_ux_Task_Get_عدد_از_Tasks( ux_Current_عدد_از_Tasks )
#پایان_اگر

#اگر_تعریف_نشده traceENTER_pcTaskGetName
    #تعریف_کن traceENTER_pcTaskGetName( x_Task_مقصد_Query )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_pcTaskGetName
    #تعریف_کن traceRETURN_pcTaskGetName( pcTaskName )
#پایان_اگر

#اگر_تعریف_نشده traceENTER_xTaskGetHandle
    #تعریف_کن traceENTER_xTaskGetHandle( pc_Name_مقصد_Query )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_xTaskGetHandle
    #تعریف_کن traceRETURN_xTaskGetHandle( pxTCB )
#پایان_اگر

#اگر_تعریف_نشده traceENTER_xTaskGetStaticBuffers
    #تعریف_کن traceENTER_xTaskGetStaticBuffers( xTask, ppux_پشته_میانگیر, ppx_Task_میانگیر )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_xTaskGetStaticBuffers
    #تعریف_کن traceRETURN_xTaskGetStaticBuffers( xReturn )
#پایان_اگر

#اگر_تعریف_نشده traceENTER_uxTaskGetSystemState
    #تعریف_کن traceENTER_uxTaskGetSystemState( px_Task_وضعیت_Array, ux_Array_اندازه, pul_مجموع_پیاپی_زمان )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_uxTaskGetSystemState
    #تعریف_کن traceRETURN_uxTaskGetSystemState( uxTask )
#پایان_اگر

#إذا ( config_عدد_از_CORES == 1 )
    #اگر_تعریف_نشده traceENTER_xTaskGetIdleTaskHandle
        #تعریف_کن traceENTER_xTaskGetIdleTaskHandle()
    #پایان_اگر
#پایان_اگر

#إذا ( config_عدد_از_CORES == 1 )
    #اگر_تعریف_نشده traceRETURN_xTaskGetIdleTaskHandle
        #تعریف_کن traceRETURN_xTaskGetIdleTaskHandle( xIdleTaskHandle )
    #پایان_اگر
#پایان_اگر

#اگر_تعریف_نشده traceENTER_xTaskGetIdleTaskHandleForCore
    #تعریف_کن traceENTER_xTaskGetIdleTaskHandleForCore( xCoreID )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_xTaskGetIdleTaskHandleForCore
    #تعریف_کن traceRETURN_xTaskGetIdleTaskHandleForCore( xIdleTaskHandle )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_v_Task_گام_Tick
    #تعریف_کن trace_ENTER_v_Task_گام_Tick( x_Ticks_مقصد_Jump )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_v_Task_گام_Tick
    #تعریف_کن trace_RETURN_v_Task_گام_Tick()
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_Task_Catch_به_بالا_Ticks
    #تعریف_کن trace_ENTER_x_Task_Catch_به_بالا_Ticks( x_Ticks_مقصد_Catch_به_بالا )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_Task_Catch_به_بالا_Ticks
    #تعریف_کن trace_RETURN_x_Task_Catch_به_بالا_Ticks( xYieldOccurred )
#پایان_اگر

#اگر_تعریف_نشده traceENTER_xTaskAbortDelay
    #تعریف_کن traceENTER_xTaskAbortDelay( xTask )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_xTaskAbortDelay
    #تعریف_کن traceRETURN_xTaskAbortDelay( xReturn )
#پایان_اگر

#اگر_تعریف_نشده traceENTER_xTaskIncrementTick
    #تعریف_کن traceENTER_xTaskIncrementTick()
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_xTaskIncrementTick
    #تعریف_کن traceRETURN_xTaskIncrementTick( xSwitchRequired )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_v_Task_مجموعه_Application_Task_برچسب
    #تعریف_کن trace_ENTER_v_Task_مجموعه_Application_Task_برچسب( xTask, pxHookFunction )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_v_Task_مجموعه_Application_Task_برچسب
    #تعریف_کن trace_RETURN_v_Task_مجموعه_Application_Task_برچسب()
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_Task_Get_Application_Task_برچسب
    #تعریف_کن trace_ENTER_x_Task_Get_Application_Task_برچسب( xTask )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_Task_Get_Application_Task_برچسب
    #تعریف_کن trace_RETURN_x_Task_Get_Application_Task_برچسب( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_Task_Get_Application_Task_برچسب_From_ISR
    #تعریف_کن trace_ENTER_x_Task_Get_Application_Task_برچسب_From_ISR( xTask )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_Task_Get_Application_Task_برچسب_From_ISR
    #تعریف_کن trace_RETURN_x_Task_Get_Application_Task_برچسب_From_ISR( xReturn )
#پایان_اگر

#اگر_تعریف_نشده traceENTER_xTaskCallApplicationTaskHook
    #تعریف_کن traceENTER_xTaskCallApplicationTaskHook( xTask, pvParameter )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_xTaskCallApplicationTaskHook
    #تعریف_کن traceRETURN_xTaskCallApplicationTaskHook( xReturn )
#پایان_اگر

#اگر_تعریف_نشده traceENTER_vTaskSwitchContext
    #تعریف_کن traceENTER_vTaskSwitchContext()
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_vTaskSwitchContext
    #تعریف_کن traceRETURN_vTaskSwitchContext()
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_v_Task_قراربده_On_Event_List
    #تعریف_کن trace_ENTER_v_Task_قراربده_On_Event_List( pxEventList, x_Ticks_مقصد_Wait )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_v_Task_قراربده_On_Event_List
    #تعریف_کن trace_RETURN_v_Task_قراربده_On_Event_List()
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_v_Task_قراربده_On_Unordered_Event_List
    #تعریف_کن trace_ENTER_v_Task_قراربده_On_Unordered_Event_List( pxEventList, x_قلم_مقدار, x_Ticks_مقصد_Wait )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_v_Task_قراربده_On_Unordered_Event_List
    #تعریف_کن trace_RETURN_v_Task_قراربده_On_Unordered_Event_List()
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_v_Task_قراربده_On_Event_List_Restricted
    #تعریف_کن trace_ENTER_v_Task_قراربده_On_Event_List_Restricted( pxEventList, x_Ticks_مقصد_Wait, xWaitIndefinitely )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_v_Task_قراربده_On_Event_List_Restricted
    #تعریف_کن trace_RETURN_v_Task_قراربده_On_Event_List_Restricted()
#پایان_اگر

#اگر_تعریف_نشده traceENTER_xTaskRemoveFromEventList
    #تعریف_کن traceENTER_xTaskRemoveFromEventList( pxEventList )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_xTaskRemoveFromEventList
    #تعریف_کن traceRETURN_xTaskRemoveFromEventList( xReturn )
#پایان_اگر

#اگر_تعریف_نشده traceENTER_vTaskRemoveFromUnorderedEventList
    #تعریف_کن traceENTER_vTaskRemoveFromUnorderedEventList( px_Event_List_قلم, x_قلم_مقدار )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_vTaskRemoveFromUnorderedEventList
    #تعریف_کن traceRETURN_vTaskRemoveFromUnorderedEventList()
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_v_Task_مجموعه_زمان_بیرون_State
    #تعریف_کن trace_ENTER_v_Task_مجموعه_زمان_بیرون_State( px_زمان_بیرون )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_v_Task_مجموعه_زمان_بیرون_State
    #تعریف_کن trace_RETURN_v_Task_مجموعه_زمان_بیرون_State()
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_v_Task_داخلی_مجموعه_زمان_بیرون_State
    #تعریف_کن trace_ENTER_v_Task_داخلی_مجموعه_زمان_بیرون_State( px_زمان_بیرون )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_v_Task_داخلی_مجموعه_زمان_بیرون_State
    #تعریف_کن trace_RETURN_v_Task_داخلی_مجموعه_زمان_بیرون_State()
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_Task_Check_For_زمان_بیرون
    #تعریف_کن trace_ENTER_x_Task_Check_For_زمان_بیرون( px_زمان_بیرون, px_Ticks_مقصد_Wait )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_Task_Check_For_زمان_بیرون
    #تعریف_کن trace_RETURN_x_Task_Check_For_زمان_بیرون( xReturn )
#پایان_اگر

#اگر_تعریف_نشده traceENTER_vTaskMissedYield
    #تعریف_کن traceENTER_vTaskMissedYield()
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_vTaskMissedYield
    #تعریف_کن traceRETURN_vTaskMissedYield()
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_ux_Task_Get_Task_عدد
    #تعریف_کن trace_ENTER_ux_Task_Get_Task_عدد( xTask )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_ux_Task_Get_Task_عدد
    #تعریف_کن trace_RETURN_ux_Task_Get_Task_عدد( uxReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_v_Task_مجموعه_Task_عدد
    #تعریف_کن trace_ENTER_v_Task_مجموعه_Task_عدد( xTask, uxHandle )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_v_Task_مجموعه_Task_عدد
    #تعریف_کن trace_RETURN_v_Task_مجموعه_Task_عدد()
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_e_Task_Confirm_Sleep_نما_وضعیت
    #تعریف_کن trace_ENTER_e_Task_Confirm_Sleep_نما_وضعیت()
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_e_Task_Confirm_Sleep_نما_وضعیت
    #تعریف_کن trace_RETURN_e_Task_Confirm_Sleep_نما_وضعیت( eReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_v_Task_مجموعه_Thread_Local_Storage_اشاره_گر
    #تعریف_کن trace_ENTER_v_Task_مجموعه_Thread_Local_Storage_اشاره_گر( x_Task_مقصد_مجموعه, x_شاخص, pv_مقدار )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_v_Task_مجموعه_Thread_Local_Storage_اشاره_گر
    #تعریف_کن trace_RETURN_v_Task_مجموعه_Thread_Local_Storage_اشاره_گر()
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_pv_Task_Get_Thread_Local_Storage_اشاره_گر
    #تعریف_کن trace_ENTER_pv_Task_Get_Thread_Local_Storage_اشاره_گر( x_Task_مقصد_Query, x_شاخص )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_pv_Task_Get_Thread_Local_Storage_اشاره_گر
    #تعریف_کن trace_RETURN_pv_Task_Get_Thread_Local_Storage_اشاره_گر( pvReturn )
#پایان_اگر

#اگر_تعریف_نشده traceENTER_vTaskAllocateMPURegions
    #تعریف_کن traceENTER_vTaskAllocateMPURegions( x_Task_مقصد_Modify, pxRegions )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_vTaskAllocateMPURegions
    #تعریف_کن traceRETURN_vTaskAllocateMPURegions()
#پایان_اگر

#اگر_تعریف_نشده traceENTER_vTaskGetInfo
    #تعریف_کن traceENTER_vTaskGetInfo( xTask, px_Task_وضعیت, x_Get_Free_پشته_Space, eState )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_vTaskGetInfo
    #تعریف_کن traceRETURN_vTaskGetInfo()
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_ux_Task_Get_پشته_بالا_Water_Mark2
    #تعریف_کن trace_ENTER_ux_Task_Get_پشته_بالا_Water_Mark2( xTask )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_ux_Task_Get_پشته_بالا_Water_Mark2
    #تعریف_کن trace_RETURN_ux_Task_Get_پشته_بالا_Water_Mark2( uxReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_ux_Task_Get_پشته_بالا_Water_Mark
    #تعریف_کن trace_ENTER_ux_Task_Get_پشته_بالا_Water_Mark( xTask )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_ux_Task_Get_پشته_بالا_Water_Mark
    #تعریف_کن trace_RETURN_ux_Task_Get_پشته_بالا_Water_Mark( uxReturn )
#پایان_اگر

#اگر_تعریف_نشده traceENTER_xTaskGetCurrentTaskHandle
    #تعریف_کن traceENTER_xTaskGetCurrentTaskHandle()
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_xTaskGetCurrentTaskHandle
    #تعریف_کن traceRETURN_xTaskGetCurrentTaskHandle( xReturn )
#پایان_اگر

#اگر_تعریف_نشده traceENTER_xTaskGetCurrentTaskHandleForCore
    #تعریف_کن traceENTER_xTaskGetCurrentTaskHandleForCore( xCoreID )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_xTaskGetCurrentTaskHandleForCore
    #تعریف_کن traceRETURN_xTaskGetCurrentTaskHandleForCore( xReturn )
#پایان_اگر

#اگر_تعریف_نشده traceENTER_xTaskGetSchedulerState
    #تعریف_کن traceENTER_xTaskGetSchedulerState()
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_xTaskGetSchedulerState
    #تعریف_کن traceRETURN_xTaskGetSchedulerState( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_Task_اولویت_Inherit
    #تعریف_کن trace_ENTER_x_Task_اولویت_Inherit( pxMutexHolder )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_Task_اولویت_Inherit
    #تعریف_کن trace_RETURN_x_Task_اولویت_Inherit( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_Task_اولویت_Disinherit
    #تعریف_کن trace_ENTER_x_Task_اولویت_Disinherit( pxMutexHolder )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_Task_اولویت_Disinherit
    #تعریف_کن trace_RETURN_x_Task_اولویت_Disinherit( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_v_Task_اولویت_Disinherit_After_Timeout
    #تعریف_کن trace_ENTER_v_Task_اولویت_Disinherit_After_Timeout( pxMutexHolder, ux_Highest_اولویت_انتظار_Task )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_v_Task_اولویت_Disinherit_After_Timeout
    #تعریف_کن trace_RETURN_v_Task_اولویت_Disinherit_After_Timeout()
#پایان_اگر

#اگر_تعریف_نشده traceENTER_vTaskYieldWithinAPI
    #تعریف_کن traceENTER_vTaskYieldWithinAPI()
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_vTaskYieldWithinAPI
    #تعریف_کن traceRETURN_vTaskYieldWithinAPI()
#پایان_اگر

#اگر_تعریف_نشده traceENTER_vTaskEnterCritical
    #تعریف_کن traceENTER_vTaskEnterCritical()
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_vTaskEnterCritical
    #تعریف_کن traceRETURN_vTaskEnterCritical()
#پایان_اگر

#اگر_تعریف_نشده traceENTER_vTaskEnterCriticalFromISR
    #تعریف_کن traceENTER_vTaskEnterCriticalFromISR()
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_vTaskEnterCriticalFromISR
    #تعریف_کن traceRETURN_vTaskEnterCriticalFromISR( ux_Saved_وقفه_وضعیت )
#پایان_اگر

#اگر_تعریف_نشده traceENTER_vTaskExitCritical
    #تعریف_کن traceENTER_vTaskExitCritical()
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_vTaskExitCritical
    #تعریف_کن traceRETURN_vTaskExitCritical()
#پایان_اگر

#اگر_تعریف_نشده traceENTER_vTaskExitCriticalFromISR
    #تعریف_کن traceENTER_vTaskExitCriticalFromISR( ux_Saved_وقفه_وضعیت )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_vTaskExitCriticalFromISR
    #تعریف_کن traceRETURN_vTaskExitCriticalFromISR()
#پایان_اگر

#اگر_تعریف_نشده traceENTER_vTaskListTasks
    #تعریف_کن traceENTER_vTaskListTasks( pc_Write_میانگیر, ux_میانگیر_درازی )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_vTaskListTasks
    #تعریف_کن traceRETURN_vTaskListTasks()
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_v_Task_Get_پیاپی_زمان_Statistics
    #تعریف_کن trace_ENTER_v_Task_Get_پیاپی_زمان_Statistics( pc_Write_میانگیر, ux_میانگیر_درازی )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_v_Task_Get_پیاپی_زمان_Statistics
    #تعریف_کن trace_RETURN_v_Task_Get_پیاپی_زمان_Statistics()
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_ux_Task_Reset_Event_قلم_مقدار
    #تعریف_کن trace_ENTER_ux_Task_Reset_Event_قلم_مقدار()
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_ux_Task_Reset_Event_قلم_مقدار
    #تعریف_کن trace_RETURN_ux_Task_Reset_Event_قلم_مقدار( uxReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_pv_Task_Increment_Mutex_Held_شمار
    #تعریف_کن trace_ENTER_pv_Task_Increment_Mutex_Held_شمار()
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_pv_Task_Increment_Mutex_Held_شمار
    #تعریف_کن trace_RETURN_pv_Task_Increment_Mutex_Held_شمار( pxTCB )
#پایان_اگر

#اگر_تعریف_نشده traceENTER_ulTaskGenericNotifyTake
    #تعریف_کن traceENTER_ulTaskGenericNotifyTake( ux_شاخص_مقصد_Wait_On, x_Clear_شمار_On_Exit, x_Ticks_مقصد_Wait )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_ulTaskGenericNotifyTake
    #تعریف_کن traceRETURN_ulTaskGenericNotifyTake( ulReturn )
#پایان_اگر

#اگر_تعریف_نشده traceENTER_xTaskGenericNotifyWait
    #تعریف_کن traceENTER_xTaskGenericNotifyWait( ux_شاخص_مقصد_Wait_On, ul_بیتها_مقصد_Clear_On_مدخل, ul_بیتها_مقصد_Clear_On_Exit, pul_Notification_مقدار, x_Ticks_مقصد_Wait )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_xTaskGenericNotifyWait
    #تعریف_کن traceRETURN_xTaskGenericNotifyWait( xReturn )
#پایان_اگر

#اگر_تعریف_نشده traceENTER_xTaskGenericNotify
    #تعریف_کن traceENTER_xTaskGenericNotify( x_Task_مقصد_Notify, ux_شاخص_مقصد_Notify, ul_مقدار, eAction, pul_قبلی_Notification_مقدار )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_xTaskGenericNotify
    #تعریف_کن traceRETURN_xTaskGenericNotify( xReturn )
#پایان_اگر

#اگر_تعریف_نشده traceENTER_xTaskGenericNotifyFromISR
    #تعریف_کن traceENTER_xTaskGenericNotifyFromISR( x_Task_مقصد_Notify, ux_شاخص_مقصد_Notify, ul_مقدار, eAction, pul_قبلی_Notification_مقدار, px_Higher_اولویت_Task_Woken )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_xTaskGenericNotifyFromISR
    #تعریف_کن traceRETURN_xTaskGenericNotifyFromISR( xReturn )
#پایان_اگر

#اگر_تعریف_نشده traceENTER_vTaskGenericNotifyGiveFromISR
    #تعریف_کن traceENTER_vTaskGenericNotifyGiveFromISR( x_Task_مقصد_Notify, ux_شاخص_مقصد_Notify, px_Higher_اولویت_Task_Woken )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_vTaskGenericNotifyGiveFromISR
    #تعریف_کن traceRETURN_vTaskGenericNotifyGiveFromISR()
#پایان_اگر

#اگر_تعریف_نشده traceENTER_xTaskGenericNotifyStateClear
    #تعریف_کن traceENTER_xTaskGenericNotifyStateClear( xTask, ux_شاخص_مقصد_Clear )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_xTaskGenericNotifyStateClear
    #تعریف_کن traceRETURN_xTaskGenericNotifyStateClear( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_ul_Task_Generic_Notify_مقدار_Clear
    #تعریف_کن trace_ENTER_ul_Task_Generic_Notify_مقدار_Clear( xTask, ux_شاخص_مقصد_Clear, ul_بیتها_مقصد_Clear )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_ul_Task_Generic_Notify_مقدار_Clear
    #تعریف_کن trace_RETURN_ul_Task_Generic_Notify_مقدار_Clear( ulReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_ul_Task_Get_پیاپی_زمان_Counter
    #تعریف_کن trace_ENTER_ul_Task_Get_پیاپی_زمان_Counter( xTask )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_ul_Task_Get_پیاپی_زمان_Counter
    #تعریف_کن trace_RETURN_ul_Task_Get_پیاپی_زمان_Counter( ul_پیاپی_زمان_Counter )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_ul_Task_Get_پیاپی_زمان_Percent
    #تعریف_کن trace_ENTER_ul_Task_Get_پیاپی_زمان_Percent( xTask )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_ul_Task_Get_پیاپی_زمان_Percent
    #تعریف_کن trace_RETURN_ul_Task_Get_پیاپی_زمان_Percent( ulReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_ul_Task_Get_Idle_پیاپی_زمان_Counter
    #تعریف_کن trace_ENTER_ul_Task_Get_Idle_پیاپی_زمان_Counter()
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_ul_Task_Get_Idle_پیاپی_زمان_Counter
    #تعریف_کن trace_RETURN_ul_Task_Get_Idle_پیاپی_زمان_Counter( ulReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_ul_Task_Get_Idle_پیاپی_زمان_Percent
    #تعریف_کن trace_ENTER_ul_Task_Get_Idle_پیاپی_زمان_Percent()
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_ul_Task_Get_Idle_پیاپی_زمان_Percent
    #تعریف_کن trace_RETURN_ul_Task_Get_Idle_پیاپی_زمان_Percent( ulReturn )
#پایان_اگر

#اگر_تعریف_نشده traceENTER_xTaskGetMPUSettings
    #تعریف_کن traceENTER_xTaskGetMPUSettings( xTask )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_xTaskGetMPUSettings
    #تعریف_کن traceRETURN_xTaskGetMPUSettings( xMPUSettings )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_جریان_میانگیر_Generic_Create
    #تعریف_کن trace_ENTER_x_جریان_میانگیر_Generic_Create( x_میانگیر_اندازه_بایتها, x_Trigger_Level_بایتها, x_جریان_میانگیر_Type, pxSendCompletedCallback, pxReceiveCompletedCallback )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_جریان_میانگیر_Generic_Create
    #تعریف_کن trace_RETURN_x_جریان_میانگیر_Generic_Create( pv_Allocated_حافظه )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_جریان_میانگیر_Generic_Create_Static
    #تعریف_کن trace_ENTER_x_جریان_میانگیر_Generic_Create_Static( x_میانگیر_اندازه_بایتها, x_Trigger_Level_بایتها, x_جریان_میانگیر_Type, puc_جریان_میانگیر_Storage_Area, px_Static_جریان_میانگیر, pxSendCompletedCallback, pxReceiveCompletedCallback )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_جریان_میانگیر_Generic_Create_Static
    #تعریف_کن trace_RETURN_x_جریان_میانگیر_Generic_Create_Static( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_جریان_میانگیر_Get_Static_Buffers
    #تعریف_کن trace_ENTER_x_جریان_میانگیر_Get_Static_Buffers( x_جریان_میانگیر, ppuc_جریان_میانگیر_Storage_Area, ppx_Static_جریان_میانگیر )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_جریان_میانگیر_Get_Static_Buffers
    #تعریف_کن trace_RETURN_x_جریان_میانگیر_Get_Static_Buffers( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_v_جریان_میانگیر_Delete
    #تعریف_کن trace_ENTER_v_جریان_میانگیر_Delete( x_جریان_میانگیر )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_v_جریان_میانگیر_Delete
    #تعریف_کن trace_RETURN_v_جریان_میانگیر_Delete()
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_جریان_میانگیر_Reset
    #تعریف_کن trace_ENTER_x_جریان_میانگیر_Reset( x_جریان_میانگیر )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_جریان_میانگیر_Reset
    #تعریف_کن trace_RETURN_x_جریان_میانگیر_Reset( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_جریان_میانگیر_Reset_From_ISR
    #تعریف_کن trace_ENTER_x_جریان_میانگیر_Reset_From_ISR( x_جریان_میانگیر )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_جریان_میانگیر_Reset_From_ISR
    #تعریف_کن trace_RETURN_x_جریان_میانگیر_Reset_From_ISR( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_جریان_میانگیر_مجموعه_Trigger_Level
    #تعریف_کن trace_ENTER_x_جریان_میانگیر_مجموعه_Trigger_Level( x_جریان_میانگیر, xTriggerLevel )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_جریان_میانگیر_مجموعه_Trigger_Level
    #تعریف_کن trace_RETURN_x_جریان_میانگیر_مجموعه_Trigger_Level( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_جریان_میانگیر_Spaces_موجود
    #تعریف_کن trace_ENTER_x_جریان_میانگیر_Spaces_موجود( x_جریان_میانگیر )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_جریان_میانگیر_Spaces_موجود
    #تعریف_کن trace_RETURN_x_جریان_میانگیر_Spaces_موجود( xSpace )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_جریان_میانگیر_بایتها_موجود
    #تعریف_کن trace_ENTER_x_جریان_میانگیر_بایتها_موجود( x_جریان_میانگیر )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_جریان_میانگیر_بایتها_موجود
    #تعریف_کن trace_RETURN_x_جریان_میانگیر_بایتها_موجود( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_جریان_میانگیر_Send
    #تعریف_کن trace_ENTER_x_جریان_میانگیر_Send( x_جریان_میانگیر, pv_Tx_داده, x_داده_درازی_بایتها, x_Ticks_مقصد_Wait )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_جریان_میانگیر_Send
    #تعریف_کن trace_RETURN_x_جریان_میانگیر_Send( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_جریان_میانگیر_Send_From_ISR
    #تعریف_کن trace_ENTER_x_جریان_میانگیر_Send_From_ISR( x_جریان_میانگیر, pv_Tx_داده, x_داده_درازی_بایتها, px_Higher_اولویت_Task_Woken )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_جریان_میانگیر_Send_From_ISR
    #تعریف_کن trace_RETURN_x_جریان_میانگیر_Send_From_ISR( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_جریان_میانگیر_Receive
    #تعریف_کن trace_ENTER_x_جریان_میانگیر_Receive( x_جریان_میانگیر, pv_Rx_داده, x_میانگیر_درازی_بایتها, x_Ticks_مقصد_Wait )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_جریان_میانگیر_Receive
    #تعریف_کن trace_RETURN_x_جریان_میانگیر_Receive( x_Received_درازی )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_جریان_میانگیر_بعدی_پیام_درازی_بایتها
    #تعریف_کن trace_ENTER_x_جریان_میانگیر_بعدی_پیام_درازی_بایتها( x_جریان_میانگیر )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_جریان_میانگیر_بعدی_پیام_درازی_بایتها
    #تعریف_کن trace_RETURN_x_جریان_میانگیر_بعدی_پیام_درازی_بایتها( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_جریان_میانگیر_Receive_From_ISR
    #تعریف_کن trace_ENTER_x_جریان_میانگیر_Receive_From_ISR( x_جریان_میانگیر, pv_Rx_داده, x_میانگیر_درازی_بایتها, px_Higher_اولویت_Task_Woken )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_جریان_میانگیر_Receive_From_ISR
    #تعریف_کن trace_RETURN_x_جریان_میانگیر_Receive_From_ISR( x_Received_درازی )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_جریان_میانگیر_Is_خالی
    #تعریف_کن trace_ENTER_x_جریان_میانگیر_Is_خالی( x_جریان_میانگیر )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_جریان_میانگیر_Is_خالی
    #تعریف_کن trace_RETURN_x_جریان_میانگیر_Is_خالی( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_جریان_میانگیر_Is_پر
    #تعریف_کن trace_ENTER_x_جریان_میانگیر_Is_پر( x_جریان_میانگیر )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_جریان_میانگیر_Is_پر
    #تعریف_کن trace_RETURN_x_جریان_میانگیر_Is_پر( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_جریان_میانگیر_Send_Completed_From_ISR
    #تعریف_کن trace_ENTER_x_جریان_میانگیر_Send_Completed_From_ISR( x_جریان_میانگیر, px_Higher_اولویت_Task_Woken )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_جریان_میانگیر_Send_Completed_From_ISR
    #تعریف_کن trace_RETURN_x_جریان_میانگیر_Send_Completed_From_ISR( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_x_جریان_میانگیر_Receive_Completed_From_ISR
    #تعریف_کن trace_ENTER_x_جریان_میانگیر_Receive_Completed_From_ISR( x_جریان_میانگیر, px_Higher_اولویت_Task_Woken )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_x_جریان_میانگیر_Receive_Completed_From_ISR
    #تعریف_کن trace_RETURN_x_جریان_میانگیر_Receive_Completed_From_ISR( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_ux_جریان_میانگیر_Get_جریان_میانگیر_Notification_شاخص
    #تعریف_کن trace_ENTER_ux_جریان_میانگیر_Get_جریان_میانگیر_Notification_شاخص( x_جریان_میانگیر )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_ux_جریان_میانگیر_Get_جریان_میانگیر_Notification_شاخص
    #تعریف_کن trace_RETURN_ux_جریان_میانگیر_Get_جریان_میانگیر_Notification_شاخص( ux_Notification_شاخص )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_v_جریان_میانگیر_مجموعه_جریان_میانگیر_Notification_شاخص
    #تعریف_کن trace_ENTER_v_جریان_میانگیر_مجموعه_جریان_میانگیر_Notification_شاخص( x_جریان_میانگیر, ux_Notification_شاخص )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_v_جریان_میانگیر_مجموعه_جریان_میانگیر_Notification_شاخص
    #تعریف_کن trace_RETURN_v_جریان_میانگیر_مجموعه_جریان_میانگیر_Notification_شاخص()
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_ux_جریان_میانگیر_Get_جریان_میانگیر_عدد
    #تعریف_کن trace_ENTER_ux_جریان_میانگیر_Get_جریان_میانگیر_عدد( x_جریان_میانگیر )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_ux_جریان_میانگیر_Get_جریان_میانگیر_عدد
    #تعریف_کن trace_RETURN_ux_جریان_میانگیر_Get_جریان_میانگیر_عدد( ux_جریان_میانگیر_عدد )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_v_جریان_میانگیر_مجموعه_جریان_میانگیر_عدد
    #تعریف_کن trace_ENTER_v_جریان_میانگیر_مجموعه_جریان_میانگیر_عدد( x_جریان_میانگیر, ux_جریان_میانگیر_عدد )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_v_جریان_میانگیر_مجموعه_جریان_میانگیر_عدد
    #تعریف_کن trace_RETURN_v_جریان_میانگیر_مجموعه_جریان_میانگیر_عدد()
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_uc_جریان_میانگیر_Get_جریان_میانگیر_Type
    #تعریف_کن trace_ENTER_uc_جریان_میانگیر_Get_جریان_میانگیر_Type( x_جریان_میانگیر )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_uc_جریان_میانگیر_Get_جریان_میانگیر_Type
    #تعریف_کن trace_RETURN_uc_جریان_میانگیر_Get_جریان_میانگیر_Type( uc_جریان_میانگیر_Type )
#پایان_اگر

#اگر_تعریف_نشده traceENTER_vListInitialise
    #تعریف_کن traceENTER_vListInitialise( pxList )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_vListInitialise
    #تعریف_کن traceRETURN_vListInitialise()
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_v_List_Initialise_قلم
    #تعریف_کن trace_ENTER_v_List_Initialise_قلم( px_قلم )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_v_List_Initialise_قلم
    #تعریف_کن trace_RETURN_v_List_Initialise_قلم()
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_v_List_درج_انتها
    #تعریف_کن trace_ENTER_v_List_درج_انتها( pxList, px_New_List_قلم )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_v_List_درج_انتها
    #تعریف_کن trace_RETURN_v_List_درج_انتها()
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_v_List_درج
    #تعریف_کن trace_ENTER_v_List_درج( pxList, px_New_List_قلم )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_v_List_درج
    #تعریف_کن trace_RETURN_v_List_درج()
#پایان_اگر

#اگر_تعریف_نشده traceENTER_uxListRemove
    #تعریف_کن traceENTER_uxListRemove( px_قلم_مقصد_Remove )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_uxListRemove
    #تعریف_کن traceRETURN_uxListRemove( ux_عدد_از_اقلام )
#پایان_اگر

#اگر_تعریف_نشده traceENTER_xCoRoutineCreate
    #تعریف_کن traceENTER_xCoRoutineCreate( px_Co_Routine_کد, ux_اولویت, ux_شاخص )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_xCoRoutineCreate
    #تعریف_کن traceRETURN_xCoRoutineCreate( xReturn )
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_v_Co_Routine_جمع_کن_مقصد_Delayed_List
    #تعریف_کن trace_ENTER_v_Co_Routine_جمع_کن_مقصد_Delayed_List( x_Ticks_مقصد_Delay, pxEventList )
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_v_Co_Routine_جمع_کن_مقصد_Delayed_List
    #تعریف_کن trace_RETURN_v_Co_Routine_جمع_کن_مقصد_Delayed_List()
#پایان_اگر

#اگر_تعریف_نشده trace_ENTER_v_Co_Routine_زمانبندی
    #تعریف_کن trace_ENTER_v_Co_Routine_زمانبندی()
#پایان_اگر

#اگر_تعریف_نشده trace_RETURN_v_Co_Routine_زمانبندی
    #تعریف_کن trace_RETURN_v_Co_Routine_زمانبندی()
#پایان_اگر

#اگر_تعریف_نشده traceENTER_xCoRoutineRemoveFromEventList
    #تعریف_کن traceENTER_xCoRoutineRemoveFromEventList( pxEventList )
#پایان_اگر

#اگر_تعریف_نشده traceRETURN_xCoRoutineRemoveFromEventList
    #تعریف_کن traceRETURN_xCoRoutineRemoveFromEventList( xReturn )
#پایان_اگر

#اگر_تعریف_نشده config_GENERATE_پیاپی_زمان_STATS
    #تعریف_کن config_GENERATE_پیاپی_زمان_STATS    0
#پایان_اگر

#إذا ( config_GENERATE_پیاپی_زمان_STATS == 1 )

    #اگر_تعریف_نشده پورت_CONFIGURE_TIMER_FOR_پیاپی_زمان_STATS
        #خطا If config_GENERATE_پیاپی_زمان_STATS is defined then پورت_CONFIGURE_TIMER_FOR_پیاپی_زمان_STATS must also be defined.  پورت_CONFIGURE_TIMER_FOR_پیاپی_زمان_STATS should call a پورت layer function مقصد راهاندازی a peripheral timer/counter that can then be استفاده_شده as the پیاپی time counter time پایه.
    #پایان_اگر /* portCONFIGURE_TIMER_FOR_RUN_TIME_STATS */

    #اگر_تعریف_نشده پورت_GET_پیاپی_زمان_COUNTER_مقدار
        #اگر_تعریف_نشده پورت_ALT_GET_پیاپی_زمان_COUNTER_مقدار
            #خطا If config_GENERATE_پیاپی_زمان_STATS is defined then either پورت_GET_پیاپی_زمان_COUNTER_مقدار or پورت_ALT_GET_پیاپی_زمان_COUNTER_مقدار must also be defined.  See the examples provided and the FreeRTOS web site لكل بیشتر information.
        #پایان_اگر /* portALT_GET_RUN_TIME_COUNTER_VALUE */
    #پایان_اگر /* portGET_RUN_TIME_COUNTER_VALUE */

#پایان_اگر /* configGENERATE_RUN_TIME_STATS */

#اگر_تعریف_نشده پورت_CONFIGURE_TIMER_FOR_پیاپی_زمان_STATS
    #تعریف_کن پورت_CONFIGURE_TIMER_FOR_پیاپی_زمان_STATS()
#پایان_اگر

#اگر_تعریف_نشده پورت_PRIVILEGE_بیت
    #تعریف_کن پورت_PRIVILEGE_بیت    ( ( UBaseType_t ) 0x00 )
#پایان_اگر

#اگر_تعریف_نشده پورت_YIELD_WITHIN_API
    #تعریف_کن پورت_YIELD_WITHIN_API    پورت_YIELD
#پایان_اگر

#اگر_تعریف_نشده پورت_SUPPRESS_TICKS_AND_SLEEP
    #تعریف_کن پورت_SUPPRESS_TICKS_AND_SLEEP( x_Expected_Idle_زمان )
#پایان_اگر

#اگر_تعریف_نشده config_EXPECTED_IDLE_زمان_BEFORE_SLEEP
    #تعریف_کن config_EXPECTED_IDLE_زمان_BEFORE_SLEEP    2
#پایان_اگر

#إذا config_EXPECTED_IDLE_زمان_BEFORE_SLEEP < 2
    #خطا config_EXPECTED_IDLE_زمان_BEFORE_SLEEP must not be less than 2
#پایان_اگر

#اگر_تعریف_نشده configUSE_TICKLESS_IDLE
    #تعریف_کن configUSE_TICKLESS_IDLE    0
#پایان_اگر

#اگر_تعریف_نشده configPRE_SUPPRESS_TICKS_AND_SLEEP_PROCESSING
    #تعریف_کن configPRE_SUPPRESS_TICKS_AND_SLEEP_PROCESSING( x )
#پایان_اگر

#اگر_تعریف_نشده configPRE_SLEEP_PROCESSING
    #تعریف_کن configPRE_SLEEP_PROCESSING( x )
#پایان_اگر

#اگر_تعریف_نشده configPOST_SLEEP_PROCESSING
    #تعریف_کن configPOST_SLEEP_PROCESSING( x )
#پایان_اگر

#اگر_تعریف_نشده config_USE_صف_SETS
    #تعریف_کن config_USE_صف_SETS    0
#پایان_اگر

#اگر_تعریف_نشده پورت_TASK_استفاده_میکند_FLOATING_نقطه
    #تعریف_کن پورت_TASK_استفاده_میکند_FLOATING_نقطه()
#پایان_اگر

#اگر_تعریف_نشده پورت_ALLOCATE_SECURE_CONTEXT
    #تعریف_کن پورت_ALLOCATE_SECURE_CONTEXT( ul_Secure_پشته_اندازه )
#پایان_اگر

#اگر_تعریف_نشده پورت_DONT_DISCARD
    #تعریف_کن پورت_DONT_DISCARD
#پایان_اگر

#اگر_تعریف_نشده config_USE_زمان_SLICING
    #تعریف_کن config_USE_زمان_SLICING    1
#پایان_اگر

#اگر_تعریف_نشده configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS
    #تعریف_کن configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS    0
#پایان_اگر

#اگر_تعریف_نشده configUSE_STATS_FORMATTING_FUNCTIONS
    #تعریف_کن configUSE_STATS_FORMATTING_FUNCTIONS    0
#پایان_اگر

#اگر_تعریف_نشده پورت_ASSERT_IF_وقفه_اولویت_INVALID
    #تعریف_کن پورت_ASSERT_IF_وقفه_اولویت_INVALID()
#پایان_اگر

#اگر_تعریف_نشده configUSE_TRACE_FACILITY
    #تعریف_کن configUSE_TRACE_FACILITY    0
#پایان_اگر

#اگر_تعریف_نشده mtCOVERAGE_TEST_MARKER
    #تعریف_کن mtCOVERAGE_TEST_MARKER()
#پایان_اگر

#اگر_تعریف_نشده mtCOVERAGE_TEST_DELAY
    #تعریف_کن mtCOVERAGE_TEST_DELAY()
#پایان_اگر

#اگر_تعریف_نشده پورت_ASSERT_IF_درون_ISR
    #تعریف_کن پورت_ASSERT_IF_درون_ISR()
#پایان_اگر

#اگر_تعریف_نشده config_USE_پورت_OPTIMISED_TASK_SELECTION
    #تعریف_کن config_USE_پورت_OPTIMISED_TASK_SELECTION    0
#پایان_اگر

#اگر_تعریف_نشده config_APPLICATION_ALLOCATED_انباشته
    #تعریف_کن config_APPLICATION_ALLOCATED_انباشته    0
#پایان_اگر

#اگر_تعریف_نشده config_ENABLE_انباشته_PROTECTOR
    #تعریف_کن config_ENABLE_انباشته_PROTECTOR    0
#پایان_اگر

#اگر_تعریف_نشده configUSE_TASK_NOTIFICATIONS
    #تعریف_کن configUSE_TASK_NOTIFICATIONS    1
#پایان_اگر

#اگر_تعریف_نشده configTASK_NOTIFICATION_ARRAY_ENTRIES
    #تعریف_کن configTASK_NOTIFICATION_ARRAY_ENTRIES    1
#پایان_اگر

#إذا configTASK_NOTIFICATION_ARRAY_ENTRIES < 1
    #خطا configTASK_NOTIFICATION_ARRAY_ENTRIES must be at کمترین 1
#پایان_اگر

#اگر_تعریف_نشده configUSE_POSIX_ERRNO
    #تعریف_کن configUSE_POSIX_ERRNO    0
#پایان_اگر

#اگر_تعریف_نشده configUSE_SB_COMPLETED_CALLBACK

/* By default per-instance callbacks are not enabled for stream buffer or message buffer. */
    #تعریف_کن configUSE_SB_COMPLETED_CALLBACK    0
#پایان_اگر

#اگر_تعریف_نشده پورت_TICK_TYPE_IS_ATOMIC
    #تعریف_کن پورت_TICK_TYPE_IS_ATOMIC    0
#پایان_اگر

#اگر_تعریف_نشده config_SUPPORT_STATIC_تخصیص
    /* Defaults to 0 for backward compatibility. */
    #تعریف_کن config_SUPPORT_STATIC_تخصیص    0
#پایان_اگر

#اگر_تعریف_نشده config_KERNEL_PROVIDED_STATIC_حافظه
    #تعریف_کن config_KERNEL_PROVIDED_STATIC_حافظه    0
#پایان_اگر

#اگر_تعریف_نشده config_SUPPORT_DYNAMIC_تخصیص
    /* Defaults to 1 for backward compatibility. */
    #تعریف_کن config_SUPPORT_DYNAMIC_تخصیص    1
#پایان_اگر

#إذا ( ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 ) && ( config_SUPPORT_DYNAMIC_تخصیص != 1 ) )
    #خطا configUSE_STATS_FORMATTING_FUNCTIONS cannot be استفاده_شده without dynamic تخصیص, but config_SUPPORT_DYNAMIC_تخصیص is not مجموعه مقصد 1.
#پایان_اگر

#إذا ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 )
    #إذا ( ( configUSE_TRACE_FACILITY != 1 ) && ( config_GENERATE_پیاپی_زمان_STATS != 1 ) )
        #خطا configUSE_STATS_FORMATTING_FUNCTIONS is 1 but the functions it enables are not استفاده_شده because neither configUSE_TRACE_FACILITY or config_GENERATE_پیاپی_زمان_STATS are 1.  مجموعه_2 configUSE_STATS_FORMATTING_FUNCTIONS مقصد 0 درون FreeRTOSConfig.h.
    #پایان_اگر
#پایان_اگر

#اگر_تعریف_نشده config_STATS_میانگیر_MAX_درازی
    #تعریف_کن config_STATS_میانگیر_MAX_درازی    0xFFFF
#پایان_اگر

#اگر_تعریف_نشده config_پشته_عمق_TYPE

/* Defaults to StackType_t for backward compatibility, but can be overridden
 * in FreeRTOSConfig.h if StackType_t is too restrictive. */
    #تعریف_کن config_پشته_عمق_TYPE    پشته_Type_t
#پایان_اگر

#اگر_تعریف_نشده config_پیاپی_زمان_COUNTER_TYPE

/* Defaults to uint32_t for backward compatibility, but can be overridden in
 * FreeRTOSConfig.h if uint32_t is too restrictive. */

    #تعریف_کن config_پیاپی_زمان_COUNTER_TYPE    uint32_t
#پایان_اگر

#اگر_تعریف_نشده config_پیام_میانگیر_درازی_TYPE

/* Defaults to size_t for backward compatibility, but can be overridden
 * in FreeRTOSConfig.h if lengths will always be less than the number of bytes
 * in a size_t. */
    #تعریف_کن config_پیام_میانگیر_درازی_TYPE    size_t
#پایان_اگر

/* Sanity check the configuration. */
#إذا ( ( config_SUPPORT_STATIC_تخصیص == 0 ) && ( config_SUPPORT_DYNAMIC_تخصیص == 0 ) )
    #خطا config_SUPPORT_STATIC_تخصیص and config_SUPPORT_DYNAMIC_تخصیص cannot both be 0, but can both be 1.
#پایان_اگر

#إذا ( ( configUSE_RECURSIVE_MUTEXES == 1 ) && ( configUSE_MUTEXES != 1 ) )
    #خطا configUSE_MUTEXES must be مجموعه مقصد 1 مقصد use recursive mutexes
#پایان_اگر

#إذا ( ( config_پیاپی_مضرب_PRIORITIES == 0 ) && ( configUSE_TASK_PREEMPTION_DISABLE != 0 ) )
    #خطا config_پیاپی_مضرب_PRIORITIES must be مجموعه مقصد 1 مقصد use task preemption disable
#پایان_اگر

#إذا ( ( configUSE_PREEMPTION == 0 ) && ( configUSE_TASK_PREEMPTION_DISABLE != 0 ) )
    #خطا configUSE_PREEMPTION must be مجموعه مقصد 1 مقصد use task preemption disable
#پایان_اگر

#إذا ( ( config_عدد_از_CORES == 1 ) && ( configUSE_TASK_PREEMPTION_DISABLE != 0 ) )
    #خطا configUSE_TASK_PREEMPTION_DISABLE is not supported درون single core FreeRTOS
#پایان_اگر

#إذا ( ( config_عدد_از_CORES == 1 ) && ( configUSE_CORE_AFFINITY != 0 ) )
    #خطا configUSE_CORE_AFFINITY is not supported درون single core FreeRTOS
#پایان_اگر

#إذا ( ( config_عدد_از_CORES > 1 ) && ( config_USE_پورت_OPTIMISED_TASK_SELECTION != 0 ) )
    #خطا config_USE_پورت_OPTIMISED_TASK_SELECTION is not supported درون SMP FreeRTOS
#پایان_اگر

#اگر_تعریف_نشده config_INITIAL_TICK_شمار
    #تعریف_کن config_INITIAL_TICK_شمار    0
#پایان_اگر

#إذا ( پورت_TICK_TYPE_IS_ATOMIC == 0 )

/* Either variables of tick type cannot be read atomically, or
 * portTICK_TYPE_IS_ATOMIC was not set - map the critical sections used when
 * the tick count is returned to the standard critical section macros. */
    #تعریف_کن پورت_TICK_TYPE_ENTER_CRITICAL()                      پورت_ENTER_CRITICAL()
    #تعریف_کن پورت_TICK_TYPE_EXIT_CRITICAL()                       پورت_EXIT_CRITICAL()
    #تعریف_کن پورت_TICK_TYPE_مجموعه_وقفه_ماسک_FROM_ISR()         پورت_مجموعه_وقفه_ماسک_FROM_ISR()
    #تعریف_کن پورت_TICK_TYPE_CLEAR_وقفه_ماسک_FROM_ISR( x )    پورت_CLEAR_وقفه_ماسک_FROM_ISR( ( x ) )
#وإلا

/* The tick type can be read atomically, so critical sections used when the
 * tick count is returned can be defined away. */
    #تعریف_کن پورت_TICK_TYPE_ENTER_CRITICAL()
    #تعریف_کن پورت_TICK_TYPE_EXIT_CRITICAL()
    #تعریف_کن پورت_TICK_TYPE_مجموعه_وقفه_ماسک_FROM_ISR()         0
    #تعریف_کن پورت_TICK_TYPE_CLEAR_وقفه_ماسک_FROM_ISR( x )    ( فراغ ) ( x )
#پایان_اگر /* if ( portTICK_TYPE_IS_ATOMIC == 0 ) */

/* Definitions to allow backward compatibility with FreeRTOS versions prior to
 * V8 if desired. */
#اگر_تعریف_نشده configENABLE_BACKWARD_COMPATIBILITY
    #تعریف_کن configENABLE_BACKWARD_COMPATIBILITY    1
#پایان_اگر

#اگر_تعریف_نشده configPRINTF

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
    #تعریف_کن configPRINTF( X )
#پایان_اگر

#اگر_تعریف_نشده configMAX

/* The application writer has not provided their own MAX macro, so define
 * the following generic implementation. */
    #تعریف_کن configMAX( a, b )    ( ( ( a ) > ( b ) ) ? ( a ) : ( b ) )
#پایان_اگر

#اگر_تعریف_نشده configMIN

/* The application writer has not provided their own MIN macro, so define
 * the following generic implementation. */
    #تعریف_کن configMIN( a, b )    ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )
#پایان_اگر

#إذا configENABLE_BACKWARD_COMPATIBILITY == 1
    #تعریف_کن eTaskStateGet                 eTaskGetState
    #تعریف_کن پورت_Tick_Type                  TickType_t
    #تعریف_کن xTaskHandle                   TaskHandle_t
    #تعریف_کن x_صف_Handle                  صف_Handle_t
    #تعریف_کن xSemaphoreHandle              SemaphoreHandle_t
    #تعریف_کن x_صف_مجموعه_Handle               صف_مجموعه_Handle_t
    #تعریف_کن x_صف_مجموعه_Member_Handle         صف_مجموعه_Member_Handle_t
    #تعریف_کن x_زمان_بیرون_Type                  زمان_بیرون_t
    #تعریف_کن x_حافظه_Region                 حافظه_Region_t
    #تعریف_کن xTaskParameters               TaskParameters_t
    #تعریف_کن x_Task_وضعیت_Type               Task_وضعیت_t
    #تعریف_کن xTimerHandle                  TimerHandle_t
    #تعریف_کن xCoRoutineHandle              CoRoutineHandle_t
    #تعریف_کن pd_TASK_HOOK_کد              TaskHookFunction_t
    #تعریف_کن پورت_TICK_نرخ_MS              پورت_TICK_PERIOD_MS
    #تعریف_کن pcTaskGetTaskName             pcTaskGetName
    #تعریف_کن pcTimerGetTimerName           pcTimerGetName
    #تعریف_کن pc_صف_Get_صف_Name           pc_صف_Get_Name
    #تعریف_کن vTaskGetTaskInfo              vTaskGetInfo
    #تعریف_کن x_Task_Get_Idle_پیاپی_زمان_Counter    ul_Task_Get_Idle_پیاپی_زمان_Counter

/* Backward compatibility within the scheduler code only - these definitions
 * are not really required but are included for completeness. */
    #تعریف_کن tmrTIMER_CALLBACK             TimerCallbackFunction_t
    #تعریف_کن pd_TASK_کد                   TaskFunction_t
    #تعریف_کن x_List_قلم                     List_قلم_t
    #تعریف_کن xList                         List_t

/* For libraries that break the list data hiding, and access list structure
 * members directly (which is not supposed to be done). */
    #تعریف_کن pxContainer                   pvContainer
#پایان_اگر /* configENABLE_BACKWARD_COMPATIBILITY */

#إذا ( configUSE_ALTERNATIVE_API != 0 )
    #خطا The alternative API was deprecated some time ago, and was removed درون FreeRTOS V9.0 0
#پایان_اگر

/* Set configUSE_TASK_FPU_SUPPORT to 0 to omit floating point support even
 * if floating point hardware is otherwise supported by the FreeRTOS port in use.
 * This constant is not supported by all FreeRTOS ports that include floating
 * point support. */
#اگر_تعریف_نشده configUSE_TASK_FPU_SUPPORT
    #تعریف_کن configUSE_TASK_FPU_SUPPORT    1
#پایان_اگر

/* Set configENABLE_MPU to 1 to enable MPU support and 0 to disable it. This is
 * currently used in ARMv8M ports. */
#اگر_تعریف_نشده configENABLE_MPU
    #تعریف_کن configENABLE_MPU    0
#پایان_اگر

/* Set configENABLE_FPU to 1 to enable FPU support and 0 to disable it. This is
 * currently used in ARMv8M ports. */
#اگر_تعریف_نشده configENABLE_FPU
    #تعریف_کن configENABLE_FPU    1
#پایان_اگر

/* Set configENABLE_MVE to 1 to enable MVE support and 0 to disable it. This is
 * currently used in ARMv8M ports. */
#اگر_تعریف_نشده configENABLE_MVE
    #تعریف_کن configENABLE_MVE    0
#پایان_اگر

/* Set configENABLE_TRUSTZONE to 1 enable TrustZone support and 0 to disable it.
 * This is currently used in ARMv8M ports. */
#اگر_تعریف_نشده configENABLE_TRUSTZONE
    #تعریف_کن configENABLE_TRUSTZONE    1
#پایان_اگر

/* Set configRUN_FREERTOS_SECURE_ONLY to 1 to run the FreeRTOS ARMv8M port on
 * the Secure Side only. */
#اگر_تعریف_نشده config_پیاپی_FREERTOS_SECURE_ONLY
    #تعریف_کن config_پیاپی_FREERTOS_SECURE_ONLY    0
#پایان_اگر

#اگر_تعریف_نشده config_پیاپی_ADDITIONAL_TESTS
    #تعریف_کن config_پیاپی_ADDITIONAL_TESTS    0
#پایان_اگر

/* The following config allows infinite loop control. For example, control the
 * infinite loop in idle task function when performing unit tests. */
#اگر_تعریف_نشده configCONTROL_INFINITE_LOOP
    #تعریف_کن configCONTROL_INFINITE_LOOP()
#پایان_اگر

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
#تعریف_کن tsk_STATIC_AND_DYNAMIC_تخصیص_POSSIBLE                                                                                     \
    ( ( ( پورت_USING_MPU_WRAPPERS == 0 ) && ( config_SUPPORT_DYNAMIC_تخصیص == 1 ) && ( config_SUPPORT_STATIC_تخصیص == 1 ) ) || \
      ( ( پورت_USING_MPU_WRAPPERS == 1 ) && ( config_SUPPORT_DYNAMIC_تخصیص == 1 ) ) )

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
بنية x_STATIC_LIST_قلم
{
    #إذا ( config_USE_LIST_داده_INTEGRITY_CHECK_بایتها == 1 )
        TickType_t xDummy1;
    #پایان_اگر
    TickType_t xDummy2;
    فراغ * pvDummy3[ 4 ];
    #إذا ( config_USE_LIST_داده_INTEGRITY_CHECK_بایتها == 1 )
        TickType_t xDummy4;
    #پایان_اگر
};
تعريفنوع بنية x_STATIC_LIST_قلم Static_List_قلم_t;

#إذا ( config_USE_MINI_LIST_قلم == 1 )
    /* See the comments above the struct xSTATIC_LIST_ITEM definition. */
    بنية x_STATIC_MINI_LIST_قلم
    {
        #إذا ( config_USE_LIST_داده_INTEGRITY_CHECK_بایتها == 1 )
            TickType_t xDummy1;
        #پایان_اگر
        TickType_t xDummy2;
        فراغ * pvDummy3[ 2 ];
    };
    تعريفنوع بنية x_STATIC_MINI_LIST_قلم Static_Mini_List_قلم_t;
#وإلا /* if ( configUSE_MINI_LIST_ITEM == 1 ) */
    تعريفنوع بنية x_STATIC_LIST_قلم      Static_Mini_List_قلم_t;
#پایان_اگر /* if ( configUSE_MINI_LIST_ITEM == 1 ) */

/* See the comments above the struct xSTATIC_LIST_ITEM definition. */
تعريفنوع بنية xSTATIC_LIST
{
    #إذا ( config_USE_LIST_داده_INTEGRITY_CHECK_بایتها == 1 )
        TickType_t xDummy1;
    #پایان_اگر
    UBaseType_t uxDummy2;
    فراغ * pvDummy3;
    Static_Mini_List_قلم_t xDummy4;
    #إذا ( config_USE_LIST_داده_INTEGRITY_CHECK_بایتها == 1 )
        TickType_t xDummy5;
    #پایان_اگر
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
    #إذا ( پورت_USING_MPU_WRAPPERS == 1 )
        xMPU_SETTINGS xDummy2;
    #پایان_اگر
    #إذا ( configUSE_CORE_AFFINITY == 1 ) && ( config_عدد_از_CORES > 1 )
        UBaseType_t uxDummy26;
    #پایان_اگر
    Static_List_قلم_t xDummy3[ 2 ];
    UBaseType_t uxDummy5;
    فراغ * pxDummy6;
    #إذا ( config_عدد_از_CORES > 1 )
        پایه_Type_t xDummy23;
        UBaseType_t uxDummy24;
    #پایان_اگر
    uint8_t ucDummy7[ configMAX_TASK_NAME_LEN ];
    #إذا ( configUSE_TASK_PREEMPTION_DISABLE == 1 )
        پایه_Type_t xDummy25;
    #پایان_اگر
    #إذا ( ( پورت_پشته_GROWTH > 0 ) || ( config_رکورد_پشته_بالا_آدرس == 1 ) )
        فراغ * pxDummy8;
    #پایان_اگر
    #إذا ( پورت_CRITICAL_NESTING_درون_TCB == 1 )
        UBaseType_t uxDummy9;
    #پایان_اگر
    #إذا ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy10[ 2 ];
    #پایان_اگر
    #إذا ( configUSE_MUTEXES == 1 )
        UBaseType_t uxDummy12[ 2 ];
    #پایان_اگر
    #إذا ( config_USE_APPLICATION_TASK_برچسب == 1 )
        فراغ * pxDummy14;
    #پایان_اگر
    #إذا ( config_رقم_THREAD_LOCAL_STORAGE_اشارهگرها > 0 )
        فراغ * pvDummy15[ config_رقم_THREAD_LOCAL_STORAGE_اشارهگرها ];
    #پایان_اگر
    #إذا ( config_GENERATE_پیاپی_زمان_STATS == 1 )
        config_پیاپی_زمان_COUNTER_TYPE ulDummy16;
    #پایان_اگر
    #إذا ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 )
        config_TLS_بلوک_TYPE xDummy17;
    #پایان_اگر
    #إذا ( configUSE_TASK_NOTIFICATIONS == 1 )
        uint32_t ulDummy18[ configTASK_NOTIFICATION_ARRAY_ENTRIES ];
        uint8_t ucDummy19[ configTASK_NOTIFICATION_ARRAY_ENTRIES ];
    #پایان_اگر
    #إذا ( tsk_STATIC_AND_DYNAMIC_تخصیص_POSSIBLE != 0 )
        uint8_t uxDummy20;
    #پایان_اگر

    #إذا ( INCLUDE_xTaskAbortDelay == 1 )
        uint8_t ucDummy21;
    #پایان_اگر
    #إذا ( configUSE_POSIX_ERRNO == 1 )
        صحيح iDummy22;
    #پایان_اگر
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
تعريفنوع بنية x_STATIC_صف
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

    #إذا ( ( config_SUPPORT_STATIC_تخصیص == 1 ) && ( config_SUPPORT_DYNAMIC_تخصیص == 1 ) )
        uint8_t ucDummy6;
    #پایان_اگر

    #إذا ( config_USE_صف_SETS == 1 )
        فراغ * pvDummy7;
    #پایان_اگر

    #إذا ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy8;
        uint8_t ucDummy9;
    #پایان_اگر
} Static_صف_t;
تعريفنوع Static_صف_t StaticSemaphore_t;

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
تعريفنوع بنية x_STATIC_EVENT_گروه
{
    TickType_t xDummy1;
    StaticList_t xDummy2;

    #إذا ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy3;
    #پایان_اگر

    #إذا ( ( config_SUPPORT_STATIC_تخصیص == 1 ) && ( config_SUPPORT_DYNAMIC_تخصیص == 1 ) )
        uint8_t ucDummy4;
    #پایان_اگر
} Static_Event_گروه_t;

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
    Static_List_قلم_t xDummy2;
    TickType_t xDummy3;
    فراغ * pvDummy5;
    TaskFunction_t pvDummy6;
    #إذا ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy7;
    #پایان_اگر
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
تعريفنوع بنية x_STATIC_جریان_میانگیر
{
    size_t uxDummy1[ 4 ];
    فراغ * pvDummy2[ 3 ];
    uint8_t ucDummy3;
    #إذا ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy4;
    #پایان_اگر
    #إذا ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        فراغ * pvDummy5[ 2 ];
    #پایان_اگر
    UBaseType_t uxDummy6;
} Static_جریان_میانگیر_t;

/* Message buffers are built on stream buffers. */
تعريفنوع Static_جریان_میانگیر_t Static_پیام_میانگیر_t;

/* *INDENT-OFF* */
#اگر_تعریف_شده __cplusplus
    }
#پایان_اگر
/* *INDENT-ON* */

/*-----------------------------------------------------------
* IDF Compatibility
*----------------------------------------------------------*/

#اگر_تعریف_شده ESP_PLATFORM

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

#پایان_اگر /* ESP_PLATFORM */

#پایان_اگر /* INC_FREERTOS_H */
