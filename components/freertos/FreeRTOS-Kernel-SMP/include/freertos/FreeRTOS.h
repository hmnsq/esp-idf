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

#جيڪڏهن_وصف_نه_ٿيل INC_FREERTOS_H
#وصف_ڪريو INC_FREERTOS_H

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
#جيڪڏهن_وصف_ٿيل __cplusplus
    خارجي "C" {
#جيڪڏهن_پورو
/* *INDENT-ON* */

/* Acceptable values for configTICK_TYPE_WIDTH_IN_BITS. */
#وصف_ڪريو TICK_TYPE_ويڪر_16_بٽ    0
#وصف_ڪريو TICK_TYPE_ويڪر_32_بٽ    1
#وصف_ڪريو TICK_TYPE_ويڪر_64_بٽ    2

/* Application specific configuration options. */
#ضمن "FreeRTOSConfig.h"

#إذا !defined( config_USE_16_ٻٽو_انگ_TICKS ) && !defined( config_TICK_TYPE_ويڪر_اندريون_بٽ )
    #غلطي Missing definition:  One جو config_USE_16_ٻٽو_انگ_TICKS and config_TICK_TYPE_ويڪر_اندريون_بٽ must be defined اندريون FreeRTOSConfig.h.  See the Configuration section جو the FreeRTOS API documentation لكل details.
#جيڪڏهن_پورو

#إذا defined( config_USE_16_ٻٽو_انگ_TICKS ) && defined( config_TICK_TYPE_ويڪر_اندريون_بٽ )
    #غلطي Only one جو config_USE_16_ٻٽو_انگ_TICKS and config_TICK_TYPE_ويڪر_اندريون_بٽ must be defined اندريون FreeRTOSConfig.h.  See the Configuration section جو the FreeRTOS API documentation لكل details.
#جيڪڏهن_پورو

/* Define configTICK_TYPE_WIDTH_IN_BITS according to the
 * value of configUSE_16_BIT_TICKS for backward compatibility. */
#جيڪڏهن_وصف_نه_ٿيل config_TICK_TYPE_ويڪر_اندريون_بٽ
    #إذا ( config_USE_16_ٻٽو_انگ_TICKS == 1 )
        #وصف_ڪريو config_TICK_TYPE_ويڪر_اندريون_بٽ    TICK_TYPE_ويڪر_16_بٽ
    #وإلا
        #وصف_ڪريو config_TICK_TYPE_ويڪر_اندريون_بٽ    TICK_TYPE_ويڪر_32_بٽ
    #جيڪڏهن_پورو
#جيڪڏهن_پورو

/* Set configUSE_MPU_WRAPPERS_V1 to 1 to use MPU wrappers v1. */
#جيڪڏهن_وصف_نه_ٿيل configUSE_MPU_WRAPPERS_V1
    #وصف_ڪريو configUSE_MPU_WRAPPERS_V1    0
#جيڪڏهن_پورو

/* Set configENABLE_ACCESS_CONTROL_LIST to 1 to enable access control list support. */
#جيڪڏهن_وصف_نه_ٿيل configENABLE_ACCESS_CONTROL_LIST
    #وصف_ڪريو configENABLE_ACCESS_CONTROL_LIST    0
#جيڪڏهن_پورو

/* Set default value of configNUMBER_OF_CORES to 1 to use single core FreeRTOS. */
#جيڪڏهن_وصف_نه_ٿيل config_عدد_جو_CORES
    #وصف_ڪريو config_عدد_جو_CORES    1
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل configUSE_MALLOC_FAILED_HOOK
    #وصف_ڪريو configUSE_MALLOC_FAILED_HOOK    0
#جيڪڏهن_پورو

/* Basic FreeRTOS definitions. */
#ضمن "projdefs.h"

/* Definitions specific to the port being used. */
#ضمن "portable.h"

/* Must be defaulted before configUSE_NEWLIB_REENTRANT is used below. */
#جيڪڏهن_وصف_نه_ٿيل configUSE_NEWLIB_REENTRANT
    #وصف_ڪريو configUSE_NEWLIB_REENTRANT    0
#جيڪڏهن_پورو

/* Required if struct _reent is used. */
#إذا ( configUSE_NEWLIB_REENTRANT == 1 )

    #ضمن "newlib-freertos.h"

#جيڪڏهن_پورو /* if ( configUSE_NEWLIB_REENTRANT == 1 ) */

/* Must be defaulted before configUSE_PICOLIBC_TLS is used below. */
#جيڪڏهن_وصف_نه_ٿيل configUSE_PICOLIBC_TLS
    #وصف_ڪريو configUSE_PICOLIBC_TLS    0
#جيڪڏهن_پورو

#إذا ( configUSE_PICOLIBC_TLS == 1 )

    #ضمن "picolibc-freertos.h"

#جيڪڏهن_پورو /* if ( configUSE_PICOLIBC_TLS == 1 ) */

#جيڪڏهن_وصف_نه_ٿيل configUSE_C_RUNTIME_TLS_SUPPORT
    #وصف_ڪريو configUSE_C_RUNTIME_TLS_SUPPORT    0
#جيڪڏهن_پورو

#إذا ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 )

    #جيڪڏهن_وصف_نه_ٿيل config_TLS_بلاڪ_TYPE
        #غلطي Missing definition:  config_TLS_بلاڪ_TYPE must be defined اندريون FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is سيٽ منزل 1.
    #جيڪڏهن_پورو

    #جيڪڏهن_وصف_نه_ٿيل config_INIT_TLS_بلاڪ
        #غلطي Missing definition:  config_INIT_TLS_بلاڪ must be defined اندريون FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is سيٽ منزل 1.
    #جيڪڏهن_پورو

    #جيڪڏهن_وصف_نه_ٿيل config_سيٽ_TLS_بلاڪ
        #غلطي Missing definition:  config_سيٽ_TLS_بلاڪ must be defined اندريون FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is سيٽ منزل 1.
    #جيڪڏهن_پورو

    #جيڪڏهن_وصف_نه_ٿيل config_DEINIT_TLS_بلاڪ
        #غلطي Missing definition:  config_DEINIT_TLS_بلاڪ must be defined اندريون FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is سيٽ منزل 1.
    #جيڪڏهن_پورو
#جيڪڏهن_پورو /* if ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 ) */

/*
 * Check all the required application specific macros have been defined.
 * These macros are application specific and (as downloaded) are defined
 * within FreeRTOSConfig.h.
 */

#جيڪڏهن_وصف_نه_ٿيل config_گهٽتر_ڍير_سائيز
    #غلطي Missing definition:  config_گهٽتر_ڍير_سائيز must be defined اندريون FreeRTOSConfig.h.  config_گهٽتر_ڍير_سائيز مقرر_ڪري the سائيز (اندريون لفظ) جو the ڍير allocated منزل the idle task.  Refer منزل the demo project provided لكل your پورٽ لكل a suitable قدر.
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل configMAX_PRIORITIES
    #غلطي Missing definition:  configMAX_PRIORITIES must be defined اندريون FreeRTOSConfig.h.  See the Configuration section جو the FreeRTOS API documentation لكل details.
#جيڪڏهن_پورو

#إذا configMAX_PRIORITIES < 1
    #غلطي configMAX_PRIORITIES must be defined منزل be greater than or equal منزل 1.
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل configUSE_PREEMPTION
    #غلطي Missing definition:  configUSE_PREEMPTION must be defined اندريون FreeRTOSConfig.h as either 1 or 0.  See the Configuration section جو the FreeRTOS API documentation لكل details.
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل configUSE_IDLE_HOOK
    #غلطي Missing definition:  configUSE_IDLE_HOOK must be defined اندريون FreeRTOSConfig.h as either 1 or 0.  See the Configuration section جو the FreeRTOS API documentation لكل details.
#جيڪڏهن_پورو

#إذا ( config_عدد_جو_CORES > 1 )
    #جيڪڏهن_وصف_نه_ٿيل configUSE_PASSIVE_IDLE_HOOK
        #غلطي Missing definition:  configUSE_PASSIVE_IDLE_HOOK must be defined اندريون FreeRTOSConfig.h as either 1 or 0.  See the Configuration section جو the FreeRTOS API documentation لكل details.
    #جيڪڏهن_پورو
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل configUSE_TICK_HOOK
    #غلطي Missing definition:  configUSE_TICK_HOOK must be defined اندريون FreeRTOSConfig.h as either 1 or 0.  See the Configuration section جو the FreeRTOS API documentation لكل details.
#جيڪڏهن_پورو

#إذا ( ( config_TICK_TYPE_ويڪر_اندريون_بٽ != TICK_TYPE_ويڪر_16_بٽ ) && \
    ( config_TICK_TYPE_ويڪر_اندريون_بٽ != TICK_TYPE_ويڪر_32_بٽ ) &&   \
    ( config_TICK_TYPE_ويڪر_اندريون_بٽ != TICK_TYPE_ويڪر_64_بٽ ) )
    #غلطي Macro config_TICK_TYPE_ويڪر_اندريون_بٽ is defined منزل incorrect قدر.  See the Configuration section جو the FreeRTOS API documentation لكل details.
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل configUSE_CO_ROUTINES
    #وصف_ڪريو configUSE_CO_ROUTINES    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل INCLUDE_v_Task_ترجيح_سيٽ
    #وصف_ڪريو INCLUDE_v_Task_ترجيح_سيٽ    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل INCLUDE_ux_Task_ترجيح_Get
    #وصف_ڪريو INCLUDE_ux_Task_ترجيح_Get    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل INCLUDE_vTaskDelete
    #وصف_ڪريو INCLUDE_vTaskDelete    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل INCLUDE_vTaskSuspend
    #وصف_ڪريو INCLUDE_vTaskSuspend    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل INCLUDE_xTaskDelayUntil
    #جيڪڏهن_وصف_ٿيل INCLUDE_vTaskDelayUntil

/* INCLUDE_vTaskDelayUntil was replaced by INCLUDE_xTaskDelayUntil.  Backward
 * compatibility is maintained if only one or the other is defined, but
 * there is a conflict if both are defined. */
        #غلطي INCLUDE_vTaskDelayUntil and INCLUDE_xTaskDelayUntil are both defined.  INCLUDE_vTaskDelayUntil is no longer required and should be removed
    #جيڪڏهن_پورو
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل INCLUDE_xTaskDelayUntil
    #جيڪڏهن_وصف_ٿيل INCLUDE_vTaskDelayUntil

/* If INCLUDE_vTaskDelayUntil is set but INCLUDE_xTaskDelayUntil is not then
 * the project's FreeRTOSConfig.h probably pre-dates the introduction of
 * xTaskDelayUntil and setting INCLUDE_xTaskDelayUntil to whatever
 * INCLUDE_vTaskDelayUntil is set to will ensure backward compatibility.
 */
        #وصف_ڪريو INCLUDE_xTaskDelayUntil    INCLUDE_vTaskDelayUntil
    #جيڪڏهن_پورو
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل INCLUDE_xTaskDelayUntil
    #وصف_ڪريو INCLUDE_xTaskDelayUntil    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل INCLUDE_vTaskDelay
    #وصف_ڪريو INCLUDE_vTaskDelay    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل INCLUDE_xTaskGetIdleTaskHandle
    #وصف_ڪريو INCLUDE_xTaskGetIdleTaskHandle    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل INCLUDE_xTaskAbortDelay
    #وصف_ڪريو INCLUDE_xTaskAbortDelay    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل INCLUDE_x_قطار_Get_Mutex_Holder
    #وصف_ڪريو INCLUDE_x_قطار_Get_Mutex_Holder    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل INCLUDE_xSemaphoreGetMutexHolder
    #وصف_ڪريو INCLUDE_xSemaphoreGetMutexHolder    INCLUDE_x_قطار_Get_Mutex_Holder
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل INCLUDE_xTaskGetHandle
    #وصف_ڪريو INCLUDE_xTaskGetHandle    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل INCLUDE_ux_Task_Get_ڍير_مٿاهون_Water_Mark
    #وصف_ڪريو INCLUDE_ux_Task_Get_ڍير_مٿاهون_Water_Mark    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل INCLUDE_ux_Task_Get_ڍير_مٿاهون_Water_Mark2
    #وصف_ڪريو INCLUDE_ux_Task_Get_ڍير_مٿاهون_Water_Mark2    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل INCLUDE_eTaskGetState
    #وصف_ڪريو INCLUDE_eTaskGetState    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل INCLUDE_xTaskResumeFromISR
    #وصف_ڪريو INCLUDE_xTaskResumeFromISR    1
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل INCLUDE_xTimerPendFunctionCall
    #وصف_ڪريو INCLUDE_xTimerPendFunctionCall    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل INCLUDE_xTaskGetSchedulerState
    #وصف_ڪريو INCLUDE_xTaskGetSchedulerState    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل INCLUDE_xTaskGetCurrentTaskHandle
    #وصف_ڪريو INCLUDE_xTaskGetCurrentTaskHandle    1
#جيڪڏهن_پورو

#إذا configUSE_CO_ROUTINES != 0
    #جيڪڏهن_وصف_نه_ٿيل configMAX_CO_ROUTINE_PRIORITIES
        #غلطي configMAX_CO_ROUTINE_PRIORITIES must be greater than or equal منزل 1.
    #جيڪڏهن_پورو
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل config_USE_APPLICATION_TASK_ٽيگ
    #وصف_ڪريو config_USE_APPLICATION_TASK_ٽيگ    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل config_انگ_THREAD_LOCAL_STORAGE_اشارا
    #وصف_ڪريو config_انگ_THREAD_LOCAL_STORAGE_اشارا    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل configUSE_RECURSIVE_MUTEXES
    #وصف_ڪريو configUSE_RECURSIVE_MUTEXES    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل configUSE_MUTEXES
    #وصف_ڪريو configUSE_MUTEXES    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل configUSE_TIMERS
    #وصف_ڪريو configUSE_TIMERS    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل configUSE_EVENT_GROUPS
    #وصف_ڪريو configUSE_EVENT_GROUPS    1
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل config_USE_وهڪرو_BUFFERS
    #وصف_ڪريو config_USE_وهڪرو_BUFFERS    1
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل configUSE_DAEMON_TASK_STARTUP_HOOK
    #وصف_ڪريو configUSE_DAEMON_TASK_STARTUP_HOOK    0
#جيڪڏهن_پورو

#إذا ( configUSE_DAEMON_TASK_STARTUP_HOOK != 0 )
    #إذا ( configUSE_TIMERS == 0 )
        #غلطي configUSE_DAEMON_TASK_STARTUP_HOOK is سيٽ, but the daemon task is not created because configUSE_TIMERS is 0.
    #جيڪڏهن_پورو
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل configUSE_COUNTING_SEMAPHORES
    #وصف_ڪريو configUSE_COUNTING_SEMAPHORES    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل configUSE_TASK_PREEMPTION_DISABLE
    #وصف_ڪريو configUSE_TASK_PREEMPTION_DISABLE    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل configUSE_ALTERNATIVE_API
    #وصف_ڪريو configUSE_ALTERNATIVE_API    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل پورٽ_CRITICAL_NESTING_اندريون_TCB
    #وصف_ڪريو پورٽ_CRITICAL_NESTING_اندريون_TCB    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل configMAX_TASK_NAME_LEN
    #وصف_ڪريو configMAX_TASK_NAME_LEN    16
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل configIDLE_SHOULD_YIELD
    #وصف_ڪريو configIDLE_SHOULD_YIELD    1
#جيڪڏهن_پورو

#إذا configMAX_TASK_NAME_LEN < 1
    #غلطي configMAX_TASK_NAME_LEN must be سيٽ منزل a minimum جو 1 اندريون FreeRTOSConfig.h
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل configASSERT
    #وصف_ڪريو configASSERT( x )
    #وصف_ڪريو configASSERT_DEFINED    0
#وإلا
    #وصف_ڪريو configASSERT_DEFINED    1
#جيڪڏهن_پورو

/* configPRECONDITION should be defined as configASSERT.
 * The CBMC proofs need a way to track assumptions and assertions.
 * A configPRECONDITION statement should express an implicit invariant or
 * assumption made.  A configASSERT statement should express an invariant that must
 * hold explicit before calling the code. */
#جيڪڏهن_وصف_نه_ٿيل configPRECONDITION
    #وصف_ڪريو configPRECONDITION( X )    configASSERT( X )
    #وصف_ڪريو configPRECONDITION_DEFINED    0
#وإلا
    #وصف_ڪريو configPRECONDITION_DEFINED    1
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل configCHECK_HANDLER_INSTALLATION
    #وصف_ڪريو configCHECK_HANDLER_INSTALLATION    1
#وإلا

/* The application has explicitly defined configCHECK_HANDLER_INSTALLATION
 * to 1. The checks requires configASSERT() to be defined. */
    #إذا ( ( configCHECK_HANDLER_INSTALLATION == 1 ) && ( configASSERT_DEFINED == 0 ) )
        #غلطي You must define configASSERT() when configCHECK_HANDLER_INSTALLATION is 1.
    #جيڪڏهن_پورو
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل پورٽ_يادگيري_BARRIER
    #وصف_ڪريو پورٽ_يادگيري_BARRIER()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل پورٽ_SOFTWARE_BARRIER
    #وصف_ڪريو پورٽ_SOFTWARE_BARRIER()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل config_لاڳيتو_گڻو_PRIORITIES
    #وصف_ڪريو config_لاڳيتو_گڻو_PRIORITIES    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل پورٽ_GET_CORE_ID

    #إذا ( config_عدد_جو_CORES == 1 )
        #وصف_ڪريو پورٽ_GET_CORE_ID()    0
    #وإلا
        #غلطي config_عدد_جو_CORES is سيٽ منزل وڌيڪ than 1 then پورٽ_GET_CORE_ID must also be defined.
    #جيڪڏهن_پورو /* configNUMBER_OF_CORES */

#جيڪڏهن_پورو /* portGET_CORE_ID */

#جيڪڏهن_وصف_نه_ٿيل پورٽ_YIELD_CORE

    #إذا ( config_عدد_جو_CORES == 1 )
        #وصف_ڪريو پورٽ_YIELD_CORE( x )    پورٽ_YIELD()
    #وإلا
        #غلطي config_عدد_جو_CORES is سيٽ منزل وڌيڪ than 1 then پورٽ_YIELD_CORE must also be defined.
    #جيڪڏهن_پورو /* configNUMBER_OF_CORES */

#جيڪڏهن_پورو /* portYIELD_CORE */

#جيڪڏهن_وصف_نه_ٿيل پورٽ_سيٽ_مداخلت_نقاب

    #إذا ( config_عدد_جو_CORES > 1 )
        #غلطي پورٽ_سيٽ_مداخلت_نقاب is required اندريون SMP
    #جيڪڏهن_پورو

#جيڪڏهن_پورو /* portSET_INTERRUPT_MASK */

#جيڪڏهن_وصف_نه_ٿيل پورٽ_CLEAR_مداخلت_نقاب

    #إذا ( config_عدد_جو_CORES > 1 )
        #غلطي پورٽ_CLEAR_مداخلت_نقاب is required اندريون SMP
    #جيڪڏهن_پورو

#جيڪڏهن_پورو /* portCLEAR_INTERRUPT_MASK */

#جيڪڏهن_وصف_نه_ٿيل پورٽ_RELEASE_TASK_تالو

    #إذا ( config_عدد_جو_CORES == 1 )
        #وصف_ڪريو پورٽ_RELEASE_TASK_تالو()
    #وإلا
        #غلطي پورٽ_RELEASE_TASK_تالو is required اندريون SMP
    #جيڪڏهن_پورو

#جيڪڏهن_پورو /* portRELEASE_TASK_LOCK */

#جيڪڏهن_وصف_نه_ٿيل پورٽ_GET_TASK_تالو

    #إذا ( config_عدد_جو_CORES == 1 )
        #وصف_ڪريو پورٽ_GET_TASK_تالو()
    #وإلا
        #غلطي پورٽ_GET_TASK_تالو is required اندريون SMP
    #جيڪڏهن_پورو

#جيڪڏهن_پورو /* portGET_TASK_LOCK */

#جيڪڏهن_وصف_نه_ٿيل پورٽ_RELEASE_ISR_تالو

    #إذا ( config_عدد_جو_CORES == 1 )
        #وصف_ڪريو پورٽ_RELEASE_ISR_تالو()
    #وإلا
        #غلطي پورٽ_RELEASE_ISR_تالو is required اندريون SMP
    #جيڪڏهن_پورو

#جيڪڏهن_پورو /* portRELEASE_ISR_LOCK */

#جيڪڏهن_وصف_نه_ٿيل پورٽ_GET_ISR_تالو

    #إذا ( config_عدد_جو_CORES == 1 )
        #وصف_ڪريو پورٽ_GET_ISR_تالو()
    #وإلا
        #غلطي پورٽ_GET_ISR_تالو is required اندريون SMP
    #جيڪڏهن_پورو

#جيڪڏهن_پورو /* portGET_ISR_LOCK */

#جيڪڏهن_وصف_نه_ٿيل پورٽ_ENTER_CRITICAL_FROM_ISR

    #إذا ( config_عدد_جو_CORES > 1 )
        #غلطي پورٽ_ENTER_CRITICAL_FROM_ISR is required اندريون SMP
    #جيڪڏهن_پورو

#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل پورٽ_EXIT_CRITICAL_FROM_ISR

    #إذا ( config_عدد_جو_CORES > 1 )
        #غلطي پورٽ_EXIT_CRITICAL_FROM_ISR is required اندريون SMP
    #جيڪڏهن_پورو

#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل configUSE_CORE_AFFINITY
    #وصف_ڪريو configUSE_CORE_AFFINITY    0
#جيڪڏهن_پورو /* configUSE_CORE_AFFINITY */

#إذا ( ( config_عدد_جو_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
    #جيڪڏهن_وصف_نه_ٿيل configTASK_DEFAULT_CORE_AFFINITY
        #وصف_ڪريو configTASK_DEFAULT_CORE_AFFINITY    tskNO_AFFINITY
    #جيڪڏهن_پورو
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل configUSE_PASSIVE_IDLE_HOOK
    #وصف_ڪريو configUSE_PASSIVE_IDLE_HOOK    0
#جيڪڏهن_پورو /* configUSE_PASSIVE_IDLE_HOOK */

/* The timers module relies on xTaskGetSchedulerState(). */
#إذا configUSE_TIMERS == 1

    #جيڪڏهن_وصف_نه_ٿيل config_TIMER_TASK_ترجيح
        #غلطي If configUSE_TIMERS is سيٽ منزل 1 then config_TIMER_TASK_ترجيح must also be defined.
    #جيڪڏهن_پورو /* configTIMER_TASK_PRIORITY */

    #جيڪڏهن_وصف_نه_ٿيل config_TIMER_قطار_ڊيگهائي
        #غلطي If configUSE_TIMERS is سيٽ منزل 1 then config_TIMER_قطار_ڊيگهائي must also be defined.
    #جيڪڏهن_پورو /* configTIMER_QUEUE_LENGTH */

    #جيڪڏهن_وصف_نه_ٿيل config_TIMER_TASK_ڍير_اونهائي
        #غلطي If configUSE_TIMERS is سيٽ منزل 1 then config_TIMER_TASK_ڍير_اونهائي must also be defined.
    #جيڪڏهن_پورو /* configTIMER_TASK_STACK_DEPTH */

    #جيڪڏهن_وصف_نه_ٿيل پورٽ_TIMER_CALLBACK_ATTRIBUTE
        #وصف_ڪريو پورٽ_TIMER_CALLBACK_ATTRIBUTE
    #جيڪڏهن_پورو /* portTIMER_CALLBACK_ATTRIBUTE */

#جيڪڏهن_پورو /* configUSE_TIMERS */

#جيڪڏهن_وصف_نه_ٿيل پورٽ_HAS_NESTED_INTERRUPTS
    #إذا defined( پورٽ_سيٽ_مداخلت_نقاب_FROM_ISR ) && defined( پورٽ_CLEAR_مداخلت_نقاب_FROM_ISR )
        #وصف_ڪريو پورٽ_HAS_NESTED_INTERRUPTS    1
    #وإلا
        #وصف_ڪريو پورٽ_HAS_NESTED_INTERRUPTS    0
    #جيڪڏهن_پورو
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل پورٽ_سيٽ_مداخلت_نقاب_FROM_ISR
    #إذا ( پورٽ_HAS_NESTED_INTERRUPTS == 1 )
        #غلطي پورٽ_سيٽ_مداخلت_نقاب_FROM_ISR must be defined لكل ports that support nested interrupts (i.e. پورٽ_HAS_NESTED_INTERRUPTS is سيٽ منزل 1)
    #وإلا
        #وصف_ڪريو پورٽ_سيٽ_مداخلت_نقاب_FROM_ISR()    0
    #جيڪڏهن_پورو
#وإلا
    #إذا ( پورٽ_HAS_NESTED_INTERRUPTS == 0 )
        #غلطي پورٽ_سيٽ_مداخلت_نقاب_FROM_ISR must not be defined لكل ports that افعل not support nested interrupts (i.e. پورٽ_HAS_NESTED_INTERRUPTS is سيٽ منزل 0)
    #جيڪڏهن_پورو
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل پورٽ_CLEAR_مداخلت_نقاب_FROM_ISR
    #إذا ( پورٽ_HAS_NESTED_INTERRUPTS == 1 )
        #غلطي پورٽ_CLEAR_مداخلت_نقاب_FROM_ISR must be defined لكل ports that support nested interrupts  (i.e. پورٽ_HAS_NESTED_INTERRUPTS is سيٽ منزل 1)
    #وإلا
        #وصف_ڪريو پورٽ_CLEAR_مداخلت_نقاب_FROM_ISR( ux_Saved_حالت_قدر )    ( فراغ ) ( ux_Saved_حالت_قدر )
    #جيڪڏهن_پورو
#وإلا
    #إذا ( پورٽ_HAS_NESTED_INTERRUPTS == 0 )
        #غلطي پورٽ_CLEAR_مداخلت_نقاب_FROM_ISR must not be defined لكل ports that افعل not support nested interrupts (i.e. پورٽ_HAS_NESTED_INTERRUPTS is سيٽ منزل 0)
    #جيڪڏهن_پورو
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل پورٽ_CLEAN_مٿي_TCB
    #وصف_ڪريو پورٽ_CLEAN_مٿي_TCB( pxTCB )    ( فراغ ) ( pxTCB )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل پورٽ_PRE_TASK_DELETE_HOOK
    #وصف_ڪريو پورٽ_PRE_TASK_DELETE_HOOK( pv_Task_منزل_Delete, pxYieldPending )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل پورٽ_تياري_TCB
    #وصف_ڪريو پورٽ_تياري_TCB( pxTCB )    ( فراغ ) ( pxTCB )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل پورٽ_TASK_SWITCH_HOOK
    #وصف_ڪريو پورٽ_TASK_SWITCH_HOOK( pxTCB )    ( فراغ ) ( pxTCB )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل config_قطار_REGISTRY_سائيز
    #وصف_ڪريو config_قطار_REGISTRY_سائيز    0U
#جيڪڏهن_پورو

#إذا ( config_قطار_REGISTRY_سائيز < 1 )
    #وصف_ڪريو v_قطار_جوڙيو_منزل_Registry( x_قطار, pcName )
    #وصف_ڪريو v_قطار_Unregister_قطار( x_قطار )
    #وصف_ڪريو pc_قطار_Get_Name( x_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل config_USE_MINI_LIST_شي
    #وصف_ڪريو config_USE_MINI_LIST_شي    1
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل پورٽ_اشارو_سائيز_TYPE
    #وصف_ڪريو پورٽ_اشارو_سائيز_TYPE    uint32_t
#جيڪڏهن_پورو

/* Remove any unused trace macros. */
#جيڪڏهن_وصف_نه_ٿيل trace_شروعات

/* Used to perform any necessary initialisation - for example, open a file
 * into which trace is to be written. */
    #وصف_ڪريو trace_شروعات()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_پڇاڙي

/* Use to close a trace, for example close a file into which trace has been
 * written. */
    #وصف_ڪريو trace_پڇاڙي()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_TASK_SWITCHED_اندريون

/* Called after a task has been selected to run.  pxCurrentTCB holds a pointer
 * to the task control block of the selected task. */
    #وصف_ڪريو trace_TASK_SWITCHED_اندريون()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_INCREASE_TICK_ڳڻپ

/* Called before stepping the tick count after waking from tickless idle
 * sleep. */
    #وصف_ڪريو trace_INCREASE_TICK_ڳڻپ( x )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_هيٺيون_POWER_IDLE_BEGIN
    /* Called immediately before entering tickless idle. */
    #وصف_ڪريو trace_هيٺيون_POWER_IDLE_BEGIN()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_هيٺيون_POWER_IDLE_پڇاڙي
    /* Called when returning to the Idle task after a tickless idle. */
    #وصف_ڪريو trace_هيٺيون_POWER_IDLE_پڇاڙي()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_TASK_SWITCHED_ٻاهريون

/* Called before a task has been selected to run.  pxCurrentTCB holds a pointer
 * to the task control block of the task being switched out. */
    #وصف_ڪريو trace_TASK_SWITCHED_ٻاهريون()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_TASK_ترجيح_INHERIT

/* Called when a task attempts to take a mutex that is already held by a
 * lower priority task.  pxTCBOfMutexHolder is a pointer to the TCB of the task
 * that holds the mutex.  uxInheritedPriority is the priority the mutex holder
 * will inherit (the priority of the task that is attempting to obtain the
 * muted. */
    #وصف_ڪريو trace_TASK_ترجيح_INHERIT( pxTCBOfMutexHolder, ux_Inherited_ترجيح )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_TASK_ترجيح_DISINHERIT

/* Called when a task releases a mutex, the holding of which had resulted in
 * the task inheriting the priority of a higher priority task.
 * pxTCBOfMutexHolder is a pointer to the TCB of the task that is releasing the
 * mutex.  uxOriginalPriority is the task's configured (base) priority. */
    #وصف_ڪريو trace_TASK_ترجيح_DISINHERIT( pxTCBOfMutexHolder, ux_Original_ترجيح )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_BLOCKING_ON_قطار_RECEIVE

/* Task is about to block because it cannot read from a
 * queue/mutex/semaphore.  pxQueue is a pointer to the queue/mutex/semaphore
 * upon which the read was attempted.  pxCurrentTCB points to the TCB of the
 * task that attempted the read. */
    #وصف_ڪريو trace_BLOCKING_ON_قطار_RECEIVE( px_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_BLOCKING_ON_قطار_PEEK

/* Task is about to block because it cannot read from a
 * queue/mutex/semaphore.  pxQueue is a pointer to the queue/mutex/semaphore
 * upon which the read was attempted.  pxCurrentTCB points to the TCB of the
 * task that attempted the read. */
    #وصف_ڪريو trace_BLOCKING_ON_قطار_PEEK( px_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_BLOCKING_ON_قطار_SEND

/* Task is about to block because it cannot write to a
 * queue/mutex/semaphore.  pxQueue is a pointer to the queue/mutex/semaphore
 * upon which the write was attempted.  pxCurrentTCB points to the TCB of the
 * task that attempted the write. */
    #وصف_ڪريو trace_BLOCKING_ON_قطار_SEND( px_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل config_CHECK_FOR_ڍير_حد_کان_مٿي
    #وصف_ڪريو config_CHECK_FOR_ڍير_حد_کان_مٿي    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل config_رڪارڊ_ڍير_مٿاهون_پتو
    #وصف_ڪريو config_رڪارڊ_ڍير_مٿاهون_پتو    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H
    #وصف_ڪريو configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H    0
#جيڪڏهن_پورو

/* The following event macros are embedded in the kernel API calls. */

#جيڪڏهن_وصف_نه_ٿيل trace_MOVED_TASK_منزل_READY_STATE
    #وصف_ڪريو trace_MOVED_TASK_منزل_READY_STATE( pxTCB )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_POST_MOVED_TASK_منزل_READY_STATE
    #وصف_ڪريو trace_POST_MOVED_TASK_منزل_READY_STATE( pxTCB )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_MOVED_TASK_منزل_DELAYED_LIST
    #وصف_ڪريو trace_MOVED_TASK_منزل_DELAYED_LIST()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_MOVED_TASK_منزل_حد_کان_مٿي_DELAYED_LIST
    #وصف_ڪريو trace_MOVED_TASK_منزل_حد_کان_مٿي_DELAYED_LIST()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_قطار_CREATE
    #وصف_ڪريو trace_قطار_CREATE( px_New_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_قطار_CREATE_FAILED
    #وصف_ڪريو trace_قطار_CREATE_FAILED( uc_قطار_Type )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceCREATE_MUTEX
    #وصف_ڪريو traceCREATE_MUTEX( px_New_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceCREATE_MUTEX_FAILED
    #وصف_ڪريو traceCREATE_MUTEX_FAILED()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceGIVE_MUTEX_RECURSIVE
    #وصف_ڪريو traceGIVE_MUTEX_RECURSIVE( pxMutex )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceGIVE_MUTEX_RECURSIVE_FAILED
    #وصف_ڪريو traceGIVE_MUTEX_RECURSIVE_FAILED( pxMutex )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceTAKE_MUTEX_RECURSIVE
    #وصف_ڪريو traceTAKE_MUTEX_RECURSIVE( pxMutex )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceTAKE_MUTEX_RECURSIVE_FAILED
    #وصف_ڪريو traceTAKE_MUTEX_RECURSIVE_FAILED( pxMutex )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceCREATE_COUNTING_SEMAPHORE
    #وصف_ڪريو traceCREATE_COUNTING_SEMAPHORE()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceCREATE_COUNTING_SEMAPHORE_FAILED
    #وصف_ڪريو traceCREATE_COUNTING_SEMAPHORE_FAILED()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_قطار_سيٽ_SEND
    #وصف_ڪريو trace_قطار_سيٽ_SEND    trace_قطار_SEND
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_قطار_SEND
    #وصف_ڪريو trace_قطار_SEND( px_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_قطار_SEND_FAILED
    #وصف_ڪريو trace_قطار_SEND_FAILED( px_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_قطار_RECEIVE
    #وصف_ڪريو trace_قطار_RECEIVE( px_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_قطار_PEEK
    #وصف_ڪريو trace_قطار_PEEK( px_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_قطار_PEEK_FAILED
    #وصف_ڪريو trace_قطار_PEEK_FAILED( px_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_قطار_PEEK_FROM_ISR
    #وصف_ڪريو trace_قطار_PEEK_FROM_ISR( px_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_قطار_RECEIVE_FAILED
    #وصف_ڪريو trace_قطار_RECEIVE_FAILED( px_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_قطار_SEND_FROM_ISR
    #وصف_ڪريو trace_قطار_SEND_FROM_ISR( px_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_قطار_SEND_FROM_ISR_FAILED
    #وصف_ڪريو trace_قطار_SEND_FROM_ISR_FAILED( px_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_قطار_RECEIVE_FROM_ISR
    #وصف_ڪريو trace_قطار_RECEIVE_FROM_ISR( px_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_قطار_RECEIVE_FROM_ISR_FAILED
    #وصف_ڪريو trace_قطار_RECEIVE_FROM_ISR_FAILED( px_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_قطار_PEEK_FROM_ISR_FAILED
    #وصف_ڪريو trace_قطار_PEEK_FROM_ISR_FAILED( px_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_قطار_DELETE
    #وصف_ڪريو trace_قطار_DELETE( px_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceTASK_CREATE
    #وصف_ڪريو traceTASK_CREATE( pxNewTCB )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceTASK_CREATE_FAILED
    #وصف_ڪريو traceTASK_CREATE_FAILED()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceTASK_DELETE
    #وصف_ڪريو traceTASK_DELETE( px_Task_منزل_Delete )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceTASK_DELAY_UNTIL
    #وصف_ڪريو traceTASK_DELAY_UNTIL( x )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceTASK_DELAY
    #وصف_ڪريو traceTASK_DELAY()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_TASK_ترجيح_سيٽ
    #وصف_ڪريو trace_TASK_ترجيح_سيٽ( pxTask, ux_New_ترجيح )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceTASK_SUSPEND
    #وصف_ڪريو traceTASK_SUSPEND( px_Task_منزل_Suspend )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceTASK_RESUME
    #وصف_ڪريو traceTASK_RESUME( px_Task_منزل_Resume )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceTASK_RESUME_FROM_ISR
    #وصف_ڪريو traceTASK_RESUME_FROM_ISR( px_Task_منزل_Resume )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceTASK_INCREMENT_TICK
    #وصف_ڪريو traceTASK_INCREMENT_TICK( x_Tick_ڳڻپ )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceTIMER_CREATE
    #وصف_ڪريو traceTIMER_CREATE( pxNewTimer )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceTIMER_CREATE_FAILED
    #وصف_ڪريو traceTIMER_CREATE_FAILED()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceTIMER_COMMAND_SEND
    #وصف_ڪريو traceTIMER_COMMAND_SEND( xTimer, x_پيغام_ID, x_پيغام_قدر_قدر, xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceTIMER_EXPIRED
    #وصف_ڪريو traceTIMER_EXPIRED( pxTimer )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceTIMER_COMMAND_RECEIVED
    #وصف_ڪريو traceTIMER_COMMAND_RECEIVED( pxTimer, x_پيغام_ID, x_پيغام_قدر )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceMALLOC
    #وصف_ڪريو traceMALLOC( pv_پتو, ui_سائيز )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceFREE
    #وصف_ڪريو traceFREE( pv_پتو, ui_سائيز )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_EVENT_گروهه_CREATE
    #وصف_ڪريو trace_EVENT_گروهه_CREATE( x_Event_گروهه )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_EVENT_گروهه_CREATE_FAILED
    #وصف_ڪريو trace_EVENT_گروهه_CREATE_FAILED()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_EVENT_گروهه_SYNC_بلاڪ
    #وصف_ڪريو trace_EVENT_گروهه_SYNC_بلاڪ( x_Event_گروهه, ux_بٽ_منزل_سيٽ, ux_بٽ_منزل_Wait_For )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_EVENT_گروهه_SYNC_پڇاڙي
    #وصف_ڪريو trace_EVENT_گروهه_SYNC_پڇاڙي( x_Event_گروهه, ux_بٽ_منزل_سيٽ, ux_بٽ_منزل_Wait_For, xTimeoutOccurred )    ( فراغ ) ( xTimeoutOccurred )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_EVENT_گروهه_WAIT_بٽ_بلاڪ
    #وصف_ڪريو trace_EVENT_گروهه_WAIT_بٽ_بلاڪ( x_Event_گروهه, ux_بٽ_منزل_Wait_For )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_EVENT_گروهه_WAIT_بٽ_پڇاڙي
    #وصف_ڪريو trace_EVENT_گروهه_WAIT_بٽ_پڇاڙي( x_Event_گروهه, ux_بٽ_منزل_Wait_For, xTimeoutOccurred )    ( فراغ ) ( xTimeoutOccurred )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_EVENT_گروهه_CLEAR_بٽ
    #وصف_ڪريو trace_EVENT_گروهه_CLEAR_بٽ( x_Event_گروهه, ux_بٽ_منزل_Clear )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_EVENT_گروهه_CLEAR_بٽ_FROM_ISR
    #وصف_ڪريو trace_EVENT_گروهه_CLEAR_بٽ_FROM_ISR( x_Event_گروهه, ux_بٽ_منزل_Clear )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_EVENT_گروهه_سيٽ_بٽ
    #وصف_ڪريو trace_EVENT_گروهه_سيٽ_بٽ( x_Event_گروهه, ux_بٽ_منزل_سيٽ )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_EVENT_گروهه_سيٽ_بٽ_FROM_ISR
    #وصف_ڪريو trace_EVENT_گروهه_سيٽ_بٽ_FROM_ISR( x_Event_گروهه, ux_بٽ_منزل_سيٽ )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_EVENT_گروهه_DELETE
    #وصف_ڪريو trace_EVENT_گروهه_DELETE( x_Event_گروهه )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل tracePEND_FUNC_CALL
    #وصف_ڪريو tracePEND_FUNC_CALL( x_Function_منزل_Pend, pvParameter1, ulParameter2, ret )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل tracePEND_FUNC_CALL_FROM_ISR
    #وصف_ڪريو tracePEND_FUNC_CALL_FROM_ISR( x_Function_منزل_Pend, pvParameter1, ulParameter2, ret )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_قطار_REGISTRY_جوڙيو
    #وصف_ڪريو trace_قطار_REGISTRY_جوڙيو( x_قطار, pc_قطار_Name )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_TASK_NOTIFY_TAKE_بلاڪ
    #وصف_ڪريو trace_TASK_NOTIFY_TAKE_بلاڪ( ux_اشاريو_منزل_Wait )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceTASK_NOTIFY_TAKE
    #وصف_ڪريو traceTASK_NOTIFY_TAKE( ux_اشاريو_منزل_Wait )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_TASK_NOTIFY_WAIT_بلاڪ
    #وصف_ڪريو trace_TASK_NOTIFY_WAIT_بلاڪ( ux_اشاريو_منزل_Wait )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceTASK_NOTIFY_WAIT
    #وصف_ڪريو traceTASK_NOTIFY_WAIT( ux_اشاريو_منزل_Wait )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceTASK_NOTIFY
    #وصف_ڪريو traceTASK_NOTIFY( ux_اشاريو_منزل_Notify )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceTASK_NOTIFY_FROM_ISR
    #وصف_ڪريو traceTASK_NOTIFY_FROM_ISR( ux_اشاريو_منزل_Notify )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceTASK_NOTIFY_GIVE_FROM_ISR
    #وصف_ڪريو traceTASK_NOTIFY_GIVE_FROM_ISR( ux_اشاريو_منزل_Notify )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ISR_EXIT_منزل_SCHEDULER
    #وصف_ڪريو trace_ISR_EXIT_منزل_SCHEDULER()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceISR_EXIT
    #وصف_ڪريو traceISR_EXIT()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceISR_ENTER
    #وصف_ڪريو traceISR_ENTER()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_وهڪرو_بفر_CREATE_FAILED
    #وصف_ڪريو trace_وهڪرو_بفر_CREATE_FAILED( x_وهڪرو_بفر_Type )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_وهڪرو_بفر_CREATE_STATIC_FAILED
    #وصف_ڪريو trace_وهڪرو_بفر_CREATE_STATIC_FAILED( xReturn, x_وهڪرو_بفر_Type )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_وهڪرو_بفر_CREATE
    #وصف_ڪريو trace_وهڪرو_بفر_CREATE( px_وهڪرو_بفر, x_وهڪرو_بفر_Type )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_وهڪرو_بفر_DELETE
    #وصف_ڪريو trace_وهڪرو_بفر_DELETE( x_وهڪرو_بفر )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_وهڪرو_بفر_RESET
    #وصف_ڪريو trace_وهڪرو_بفر_RESET( x_وهڪرو_بفر )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_وهڪرو_بفر_RESET_FROM_ISR
    #وصف_ڪريو trace_وهڪرو_بفر_RESET_FROM_ISR( x_وهڪرو_بفر )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_BLOCKING_ON_وهڪرو_بفر_SEND
    #وصف_ڪريو trace_BLOCKING_ON_وهڪرو_بفر_SEND( x_وهڪرو_بفر )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_وهڪرو_بفر_SEND
    #وصف_ڪريو trace_وهڪرو_بفر_SEND( x_وهڪرو_بفر, x_بائيٽون_موڪليل )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_وهڪرو_بفر_SEND_FAILED
    #وصف_ڪريو trace_وهڪرو_بفر_SEND_FAILED( x_وهڪرو_بفر )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_وهڪرو_بفر_SEND_FROM_ISR
    #وصف_ڪريو trace_وهڪرو_بفر_SEND_FROM_ISR( x_وهڪرو_بفر, x_بائيٽون_موڪليل )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_BLOCKING_ON_وهڪرو_بفر_RECEIVE
    #وصف_ڪريو trace_BLOCKING_ON_وهڪرو_بفر_RECEIVE( x_وهڪرو_بفر )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_وهڪرو_بفر_RECEIVE
    #وصف_ڪريو trace_وهڪرو_بفر_RECEIVE( x_وهڪرو_بفر, x_Received_ڊيگهائي )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_وهڪرو_بفر_RECEIVE_FAILED
    #وصف_ڪريو trace_وهڪرو_بفر_RECEIVE_FAILED( x_وهڪرو_بفر )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_وهڪرو_بفر_RECEIVE_FROM_ISR
    #وصف_ڪريو trace_وهڪرو_بفر_RECEIVE_FROM_ISR( x_وهڪرو_بفر, x_Received_ڊيگهائي )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_Event_گروهه_Create_Static
    #وصف_ڪريو trace_ENTER_x_Event_گروهه_Create_Static( px_Event_گروهه_بفر )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_Event_گروهه_Create_Static
    #وصف_ڪريو trace_RETURN_x_Event_گروهه_Create_Static( px_Event_بٽ )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_Event_گروهه_Create
    #وصف_ڪريو trace_ENTER_x_Event_گروهه_Create()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_Event_گروهه_Create
    #وصف_ڪريو trace_RETURN_x_Event_گروهه_Create( px_Event_بٽ )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_Event_گروهه_Sync
    #وصف_ڪريو trace_ENTER_x_Event_گروهه_Sync( x_Event_گروهه, ux_بٽ_منزل_سيٽ, ux_بٽ_منزل_Wait_For, x_Ticks_منزل_Wait )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_Event_گروهه_Sync
    #وصف_ڪريو trace_RETURN_x_Event_گروهه_Sync( uxReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_Event_گروهه_Wait_بٽ
    #وصف_ڪريو trace_ENTER_x_Event_گروهه_Wait_بٽ( x_Event_گروهه, ux_بٽ_منزل_Wait_For, xClearOnExit, x_Wait_For_All_بٽ, x_Ticks_منزل_Wait )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_Event_گروهه_Wait_بٽ
    #وصف_ڪريو trace_RETURN_x_Event_گروهه_Wait_بٽ( uxReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_Event_گروهه_Clear_بٽ
    #وصف_ڪريو trace_ENTER_x_Event_گروهه_Clear_بٽ( x_Event_گروهه, ux_بٽ_منزل_Clear )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_Event_گروهه_Clear_بٽ
    #وصف_ڪريو trace_RETURN_x_Event_گروهه_Clear_بٽ( uxReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_Event_گروهه_Clear_بٽ_From_ISR
    #وصف_ڪريو trace_ENTER_x_Event_گروهه_Clear_بٽ_From_ISR( x_Event_گروهه, ux_بٽ_منزل_Clear )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_Event_گروهه_Clear_بٽ_From_ISR
    #وصف_ڪريو trace_RETURN_x_Event_گروهه_Clear_بٽ_From_ISR( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_Event_گروهه_Get_بٽ_From_ISR
    #وصف_ڪريو trace_ENTER_x_Event_گروهه_Get_بٽ_From_ISR( x_Event_گروهه )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_Event_گروهه_Get_بٽ_From_ISR
    #وصف_ڪريو trace_RETURN_x_Event_گروهه_Get_بٽ_From_ISR( uxReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_Event_گروهه_سيٽ_بٽ
    #وصف_ڪريو trace_ENTER_x_Event_گروهه_سيٽ_بٽ( x_Event_گروهه, ux_بٽ_منزل_سيٽ )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_Event_گروهه_سيٽ_بٽ
    #وصف_ڪريو trace_RETURN_x_Event_گروهه_سيٽ_بٽ( ux_Event_بٽ )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_v_Event_گروهه_Delete
    #وصف_ڪريو trace_ENTER_v_Event_گروهه_Delete( x_Event_گروهه )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_v_Event_گروهه_Delete
    #وصف_ڪريو trace_RETURN_v_Event_گروهه_Delete()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_Event_گروهه_Get_Static_بفر
    #وصف_ڪريو trace_ENTER_x_Event_گروهه_Get_Static_بفر( x_Event_گروهه, ppx_Event_گروهه_بفر )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_Event_گروهه_Get_Static_بفر
    #وصف_ڪريو trace_RETURN_x_Event_گروهه_Get_Static_بفر( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_v_Event_گروهه_سيٽ_بٽ_Callback
    #وصف_ڪريو trace_ENTER_v_Event_گروهه_سيٽ_بٽ_Callback( pv_Event_گروهه, ul_بٽ_منزل_سيٽ )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_v_Event_گروهه_سيٽ_بٽ_Callback
    #وصف_ڪريو trace_RETURN_v_Event_گروهه_سيٽ_بٽ_Callback()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_v_Event_گروهه_Clear_بٽ_Callback
    #وصف_ڪريو trace_ENTER_v_Event_گروهه_Clear_بٽ_Callback( pv_Event_گروهه, ul_بٽ_منزل_Clear )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_v_Event_گروهه_Clear_بٽ_Callback
    #وصف_ڪريو trace_RETURN_v_Event_گروهه_Clear_بٽ_Callback()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_Event_گروهه_سيٽ_بٽ_From_ISR
    #وصف_ڪريو trace_ENTER_x_Event_گروهه_سيٽ_بٽ_From_ISR( x_Event_گروهه, ux_بٽ_منزل_سيٽ, px_Higher_ترجيح_Task_Woken )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_Event_گروهه_سيٽ_بٽ_From_ISR
    #وصف_ڪريو trace_RETURN_x_Event_گروهه_سيٽ_بٽ_From_ISR( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_ux_Event_گروهه_Get_عدد
    #وصف_ڪريو trace_ENTER_ux_Event_گروهه_Get_عدد( x_Event_گروهه )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_ux_Event_گروهه_Get_عدد
    #وصف_ڪريو trace_RETURN_ux_Event_گروهه_Get_عدد( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_v_Event_گروهه_سيٽ_عدد
    #وصف_ڪريو trace_ENTER_v_Event_گروهه_سيٽ_عدد( x_Event_گروهه, ux_Event_گروهه_عدد )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_v_Event_گروهه_سيٽ_عدد
    #وصف_ڪريو trace_RETURN_v_Event_گروهه_سيٽ_عدد()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_قطار_Generic_Reset
    #وصف_ڪريو trace_ENTER_x_قطار_Generic_Reset( x_قطار, x_New_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_قطار_Generic_Reset
    #وصف_ڪريو trace_RETURN_x_قطار_Generic_Reset( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_قطار_Generic_Create_Static
    #وصف_ڪريو trace_ENTER_x_قطار_Generic_Create_Static( ux_قطار_ڊيگهائي, ux_شي_سائيز, puc_قطار_Storage, px_Static_قطار, uc_قطار_Type )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_قطار_Generic_Create_Static
    #وصف_ڪريو trace_RETURN_x_قطار_Generic_Create_Static( px_New_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_قطار_Generic_Get_Static_Buffers
    #وصف_ڪريو trace_ENTER_x_قطار_Generic_Get_Static_Buffers( x_قطار, ppuc_قطار_Storage, ppx_Static_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_قطار_Generic_Get_Static_Buffers
    #وصف_ڪريو trace_RETURN_x_قطار_Generic_Get_Static_Buffers( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_قطار_Generic_Create
    #وصف_ڪريو trace_ENTER_x_قطار_Generic_Create( ux_قطار_ڊيگهائي, ux_شي_سائيز, uc_قطار_Type )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_قطار_Generic_Create
    #وصف_ڪريو trace_RETURN_x_قطار_Generic_Create( px_New_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_قطار_Create_Mutex
    #وصف_ڪريو trace_ENTER_x_قطار_Create_Mutex( uc_قطار_Type )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_قطار_Create_Mutex
    #وصف_ڪريو trace_RETURN_x_قطار_Create_Mutex( x_New_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_قطار_Create_Mutex_Static
    #وصف_ڪريو trace_ENTER_x_قطار_Create_Mutex_Static( uc_قطار_Type, px_Static_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_قطار_Create_Mutex_Static
    #وصف_ڪريو trace_RETURN_x_قطار_Create_Mutex_Static( x_New_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_قطار_Get_Mutex_Holder
    #وصف_ڪريو trace_ENTER_x_قطار_Get_Mutex_Holder( xSemaphore )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_قطار_Get_Mutex_Holder
    #وصف_ڪريو trace_RETURN_x_قطار_Get_Mutex_Holder( pxReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_قطار_Get_Mutex_Holder_From_ISR
    #وصف_ڪريو trace_ENTER_x_قطار_Get_Mutex_Holder_From_ISR( xSemaphore )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_قطار_Get_Mutex_Holder_From_ISR
    #وصف_ڪريو trace_RETURN_x_قطار_Get_Mutex_Holder_From_ISR( pxReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_قطار_Give_Mutex_Recursive
    #وصف_ڪريو trace_ENTER_x_قطار_Give_Mutex_Recursive( xMutex )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_قطار_Give_Mutex_Recursive
    #وصف_ڪريو trace_RETURN_x_قطار_Give_Mutex_Recursive( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_قطار_Take_Mutex_Recursive
    #وصف_ڪريو trace_ENTER_x_قطار_Take_Mutex_Recursive( xMutex, x_Ticks_منزل_Wait )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_قطار_Take_Mutex_Recursive
    #وصف_ڪريو trace_RETURN_x_قطار_Take_Mutex_Recursive( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_قطار_Create_Counting_Semaphore_Static
    #وصف_ڪريو trace_ENTER_x_قطار_Create_Counting_Semaphore_Static( ux_Max_ڳڻپ, ux_Initial_ڳڻپ, px_Static_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_قطار_Create_Counting_Semaphore_Static
    #وصف_ڪريو trace_RETURN_x_قطار_Create_Counting_Semaphore_Static( xHandle )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_قطار_Create_Counting_Semaphore
    #وصف_ڪريو trace_ENTER_x_قطار_Create_Counting_Semaphore( ux_Max_ڳڻپ, ux_Initial_ڳڻپ )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_قطار_Create_Counting_Semaphore
    #وصف_ڪريو trace_RETURN_x_قطار_Create_Counting_Semaphore( xHandle )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_قطار_Generic_Send
    #وصف_ڪريو trace_ENTER_x_قطار_Generic_Send( x_قطار, pv_شي_منزل_قطار, x_Ticks_منزل_Wait, xCopyPosition )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_قطار_Generic_Send
    #وصف_ڪريو trace_RETURN_x_قطار_Generic_Send( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_قطار_Generic_Send_From_ISR
    #وصف_ڪريو trace_ENTER_x_قطار_Generic_Send_From_ISR( x_قطار, pv_شي_منزل_قطار, px_Higher_ترجيح_Task_Woken, xCopyPosition )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_قطار_Generic_Send_From_ISR
    #وصف_ڪريو trace_RETURN_x_قطار_Generic_Send_From_ISR( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_قطار_Give_From_ISR
    #وصف_ڪريو trace_ENTER_x_قطار_Give_From_ISR( x_قطار, px_Higher_ترجيح_Task_Woken )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_قطار_Give_From_ISR
    #وصف_ڪريو trace_RETURN_x_قطار_Give_From_ISR( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_قطار_Receive
    #وصف_ڪريو trace_ENTER_x_قطار_Receive( x_قطار, pv_بفر, x_Ticks_منزل_Wait )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_قطار_Receive
    #وصف_ڪريو trace_RETURN_x_قطار_Receive( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_قطار_Semaphore_Take
    #وصف_ڪريو trace_ENTER_x_قطار_Semaphore_Take( x_قطار, x_Ticks_منزل_Wait )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_قطار_Semaphore_Take
    #وصف_ڪريو trace_RETURN_x_قطار_Semaphore_Take( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_قطار_Peek
    #وصف_ڪريو trace_ENTER_x_قطار_Peek( x_قطار, pv_بفر, x_Ticks_منزل_Wait )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_قطار_Peek
    #وصف_ڪريو trace_RETURN_x_قطار_Peek( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_قطار_Receive_From_ISR
    #وصف_ڪريو trace_ENTER_x_قطار_Receive_From_ISR( x_قطار, pv_بفر, px_Higher_ترجيح_Task_Woken )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_قطار_Receive_From_ISR
    #وصف_ڪريو trace_RETURN_x_قطار_Receive_From_ISR( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_قطار_Peek_From_ISR
    #وصف_ڪريو trace_ENTER_x_قطار_Peek_From_ISR( x_قطار, pv_بفر )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_قطار_Peek_From_ISR
    #وصف_ڪريو trace_RETURN_x_قطار_Peek_From_ISR( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_ux_قطار_Messages_انتظار
    #وصف_ڪريو trace_ENTER_ux_قطار_Messages_انتظار( x_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_ux_قطار_Messages_انتظار
    #وصف_ڪريو trace_RETURN_ux_قطار_Messages_انتظار( uxReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_ux_قطار_Spaces_موجود
    #وصف_ڪريو trace_ENTER_ux_قطار_Spaces_موجود( x_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_ux_قطار_Spaces_موجود
    #وصف_ڪريو trace_RETURN_ux_قطار_Spaces_موجود( uxReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_ux_قطار_Messages_انتظار_From_ISR
    #وصف_ڪريو trace_ENTER_ux_قطار_Messages_انتظار_From_ISR( x_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_ux_قطار_Messages_انتظار_From_ISR
    #وصف_ڪريو trace_RETURN_ux_قطار_Messages_انتظار_From_ISR( uxReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_v_قطار_Delete
    #وصف_ڪريو trace_ENTER_v_قطار_Delete( x_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_v_قطار_Delete
    #وصف_ڪريو trace_RETURN_v_قطار_Delete()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_ux_قطار_Get_قطار_عدد
    #وصف_ڪريو trace_ENTER_ux_قطار_Get_قطار_عدد( x_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_ux_قطار_Get_قطار_عدد
    #وصف_ڪريو trace_RETURN_ux_قطار_Get_قطار_عدد( ux_قطار_عدد )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_v_قطار_سيٽ_قطار_عدد
    #وصف_ڪريو trace_ENTER_v_قطار_سيٽ_قطار_عدد( x_قطار, ux_قطار_عدد )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_v_قطار_سيٽ_قطار_عدد
    #وصف_ڪريو trace_RETURN_v_قطار_سيٽ_قطار_عدد()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_uc_قطار_Get_قطار_Type
    #وصف_ڪريو trace_ENTER_uc_قطار_Get_قطار_Type( x_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_uc_قطار_Get_قطار_Type
    #وصف_ڪريو trace_RETURN_uc_قطار_Get_قطار_Type( uc_قطار_Type )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_ux_قطار_Get_قطار_شي_سائيز
    #وصف_ڪريو trace_ENTER_ux_قطار_Get_قطار_شي_سائيز( x_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_ux_قطار_Get_قطار_شي_سائيز
    #وصف_ڪريو trace_RETURN_ux_قطار_Get_قطار_شي_سائيز( ux_شي_سائيز )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_ux_قطار_Get_قطار_ڊيگهائي
    #وصف_ڪريو trace_ENTER_ux_قطار_Get_قطار_ڊيگهائي( x_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_ux_قطار_Get_قطار_ڊيگهائي
    #وصف_ڪريو trace_RETURN_ux_قطار_Get_قطار_ڊيگهائي( ux_ڊيگهائي )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_قطار_Is_قطار_خالي_جايون_From_ISR
    #وصف_ڪريو trace_ENTER_x_قطار_Is_قطار_خالي_جايون_From_ISR( x_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_قطار_Is_قطار_خالي_جايون_From_ISR
    #وصف_ڪريو trace_RETURN_x_قطار_Is_قطار_خالي_جايون_From_ISR( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_قطار_Is_قطار_ڀريل_From_ISR
    #وصف_ڪريو trace_ENTER_x_قطار_Is_قطار_ڀريل_From_ISR( x_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_قطار_Is_قطار_ڀريل_From_ISR
    #وصف_ڪريو trace_RETURN_x_قطار_Is_قطار_ڀريل_From_ISR( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_قطار_CRSend
    #وصف_ڪريو trace_ENTER_x_قطار_CRSend( x_قطار, pv_شي_منزل_قطار, x_Ticks_منزل_Wait )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_قطار_CRSend
    #وصف_ڪريو trace_RETURN_x_قطار_CRSend( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_قطار_CRReceive
    #وصف_ڪريو trace_ENTER_x_قطار_CRReceive( x_قطار, pv_بفر, x_Ticks_منزل_Wait )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_قطار_CRReceive
    #وصف_ڪريو trace_RETURN_x_قطار_CRReceive( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_قطار_CRSend_From_ISR
    #وصف_ڪريو trace_ENTER_x_قطار_CRSend_From_ISR( x_قطار, pv_شي_منزل_قطار, xCoRoutinePreviouslyWoken )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_قطار_CRSend_From_ISR
    #وصف_ڪريو trace_RETURN_x_قطار_CRSend_From_ISR( xCoRoutinePreviouslyWoken )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_قطار_CRReceive_From_ISR
    #وصف_ڪريو trace_ENTER_x_قطار_CRReceive_From_ISR( x_قطار, pv_بفر, pxCoRoutineWoken )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_قطار_CRReceive_From_ISR
    #وصف_ڪريو trace_RETURN_x_قطار_CRReceive_From_ISR( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_v_قطار_جوڙيو_منزل_Registry
    #وصف_ڪريو trace_ENTER_v_قطار_جوڙيو_منزل_Registry( x_قطار, pc_قطار_Name )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_v_قطار_جوڙيو_منزل_Registry
    #وصف_ڪريو trace_RETURN_v_قطار_جوڙيو_منزل_Registry()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_pc_قطار_Get_Name
    #وصف_ڪريو trace_ENTER_pc_قطار_Get_Name( x_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_pc_قطار_Get_Name
    #وصف_ڪريو trace_RETURN_pc_قطار_Get_Name( pcReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_v_قطار_Unregister_قطار
    #وصف_ڪريو trace_ENTER_v_قطار_Unregister_قطار( x_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_v_قطار_Unregister_قطار
    #وصف_ڪريو trace_RETURN_v_قطار_Unregister_قطار()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_v_قطار_Wait_For_پيغام_Restricted
    #وصف_ڪريو trace_ENTER_v_قطار_Wait_For_پيغام_Restricted( x_قطار, x_Ticks_منزل_Wait, xWaitIndefinitely )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_v_قطار_Wait_For_پيغام_Restricted
    #وصف_ڪريو trace_RETURN_v_قطار_Wait_For_پيغام_Restricted()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_قطار_Create_سيٽ
    #وصف_ڪريو trace_ENTER_x_قطار_Create_سيٽ( ux_Event_قطار_ڊيگهائي )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_قطار_Create_سيٽ
    #وصف_ڪريو trace_RETURN_x_قطار_Create_سيٽ( px_قطار )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_قطار_جوڙيو_منزل_سيٽ
    #وصف_ڪريو trace_ENTER_x_قطار_جوڙيو_منزل_سيٽ( x_قطار_Or_Semaphore, x_قطار_سيٽ )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_قطار_جوڙيو_منزل_سيٽ
    #وصف_ڪريو trace_RETURN_x_قطار_جوڙيو_منزل_سيٽ( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_قطار_Remove_From_سيٽ
    #وصف_ڪريو trace_ENTER_x_قطار_Remove_From_سيٽ( x_قطار_Or_Semaphore, x_قطار_سيٽ )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_قطار_Remove_From_سيٽ
    #وصف_ڪريو trace_RETURN_x_قطار_Remove_From_سيٽ( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_قطار_Select_From_سيٽ
    #وصف_ڪريو trace_ENTER_x_قطار_Select_From_سيٽ( x_قطار_سيٽ, x_Ticks_منزل_Wait )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_قطار_Select_From_سيٽ
    #وصف_ڪريو trace_RETURN_x_قطار_Select_From_سيٽ( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_قطار_Select_From_سيٽ_From_ISR
    #وصف_ڪريو trace_ENTER_x_قطار_Select_From_سيٽ_From_ISR( x_قطار_سيٽ )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_قطار_Select_From_سيٽ_From_ISR
    #وصف_ڪريو trace_RETURN_x_قطار_Select_From_سيٽ_From_ISR( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_xTimerCreateTimerTask
    #وصف_ڪريو traceENTER_xTimerCreateTimerTask()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_xTimerCreateTimerTask
    #وصف_ڪريو traceRETURN_xTimerCreateTimerTask( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_xTimerCreate
    #وصف_ڪريو traceENTER_xTimerCreate( pcTimerName, x_Timer_Period_اندريون_Ticks, xAutoReload, pvTimerID, pxCallbackFunction )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_xTimerCreate
    #وصف_ڪريو traceRETURN_xTimerCreate( pxNewTimer )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_xTimerCreateStatic
    #وصف_ڪريو traceENTER_xTimerCreateStatic( pcTimerName, x_Timer_Period_اندريون_Ticks, xAutoReload, pvTimerID, pxCallbackFunction, px_Timer_بفر )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_xTimerCreateStatic
    #وصف_ڪريو traceRETURN_xTimerCreateStatic( pxNewTimer )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_xTimerGenericCommandFromTask
    #وصف_ڪريو traceENTER_xTimerGenericCommandFromTask( xTimer, xCommandID, x_Optional_قدر, px_Higher_ترجيح_Task_Woken, x_Ticks_منزل_Wait )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_xTimerGenericCommandFromTask
    #وصف_ڪريو traceRETURN_xTimerGenericCommandFromTask( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_xTimerGenericCommandFromISR
    #وصف_ڪريو traceENTER_xTimerGenericCommandFromISR( xTimer, xCommandID, x_Optional_قدر, px_Higher_ترجيح_Task_Woken, x_Ticks_منزل_Wait )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_xTimerGenericCommandFromISR
    #وصف_ڪريو traceRETURN_xTimerGenericCommandFromISR( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_xTimerGetTimerDaemonTaskHandle
    #وصف_ڪريو traceENTER_xTimerGetTimerDaemonTaskHandle()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_xTimerGetTimerDaemonTaskHandle
    #وصف_ڪريو traceRETURN_xTimerGetTimerDaemonTaskHandle( xTimerTaskHandle )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_xTimerGetPeriod
    #وصف_ڪريو traceENTER_xTimerGetPeriod( xTimer )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_xTimerGetPeriod
    #وصف_ڪريو traceRETURN_xTimerGetPeriod( x_Timer_Period_اندريون_Ticks )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_v_Timer_سيٽ_Reload_گهڻائي
    #وصف_ڪريو trace_ENTER_v_Timer_سيٽ_Reload_گهڻائي( xTimer, xAutoReload )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_v_Timer_سيٽ_Reload_گهڻائي
    #وصف_ڪريو trace_RETURN_v_Timer_سيٽ_Reload_گهڻائي()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_Timer_Get_Reload_گهڻائي
    #وصف_ڪريو trace_ENTER_x_Timer_Get_Reload_گهڻائي( xTimer )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_Timer_Get_Reload_گهڻائي
    #وصف_ڪريو trace_RETURN_x_Timer_Get_Reload_گهڻائي( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_ux_Timer_Get_Reload_گهڻائي
    #وصف_ڪريو trace_ENTER_ux_Timer_Get_Reload_گهڻائي( xTimer )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_ux_Timer_Get_Reload_گهڻائي
    #وصف_ڪريو trace_RETURN_ux_Timer_Get_Reload_گهڻائي( uxReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_Timer_Get_Expiry_وقت
    #وصف_ڪريو trace_ENTER_x_Timer_Get_Expiry_وقت( xTimer )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_Timer_Get_Expiry_وقت
    #وصف_ڪريو trace_RETURN_x_Timer_Get_Expiry_وقت( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_Timer_Get_Static_بفر
    #وصف_ڪريو trace_ENTER_x_Timer_Get_Static_بفر( xTimer, ppx_Timer_بفر )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_Timer_Get_Static_بفر
    #وصف_ڪريو trace_RETURN_x_Timer_Get_Static_بفر( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_pcTimerGetName
    #وصف_ڪريو traceENTER_pcTimerGetName( xTimer )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_pcTimerGetName
    #وصف_ڪريو traceRETURN_pcTimerGetName( pcTimerName )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_Timer_Is_Timer_سرگرم
    #وصف_ڪريو trace_ENTER_x_Timer_Is_Timer_سرگرم( xTimer )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_Timer_Is_Timer_سرگرم
    #وصف_ڪريو trace_RETURN_x_Timer_Is_Timer_سرگرم( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_pvTimerGetTimerID
    #وصف_ڪريو traceENTER_pvTimerGetTimerID( xTimer )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_pvTimerGetTimerID
    #وصف_ڪريو traceRETURN_pvTimerGetTimerID( pvReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_v_Timer_سيٽ_Timer_ID
    #وصف_ڪريو trace_ENTER_v_Timer_سيٽ_Timer_ID( xTimer, pvNewID )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_v_Timer_سيٽ_Timer_ID
    #وصف_ڪريو trace_RETURN_v_Timer_سيٽ_Timer_ID()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_xTimerPendFunctionCallFromISR
    #وصف_ڪريو traceENTER_xTimerPendFunctionCallFromISR( x_Function_منزل_Pend, pvParameter1, ulParameter2, px_Higher_ترجيح_Task_Woken )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_xTimerPendFunctionCallFromISR
    #وصف_ڪريو traceRETURN_xTimerPendFunctionCallFromISR( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_xTimerPendFunctionCall
    #وصف_ڪريو traceENTER_xTimerPendFunctionCall( x_Function_منزل_Pend, pvParameter1, ulParameter2, x_Ticks_منزل_Wait )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_xTimerPendFunctionCall
    #وصف_ڪريو traceRETURN_xTimerPendFunctionCall( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_ux_Timer_Get_Timer_عدد
    #وصف_ڪريو trace_ENTER_ux_Timer_Get_Timer_عدد( xTimer )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_ux_Timer_Get_Timer_عدد
    #وصف_ڪريو trace_RETURN_ux_Timer_Get_Timer_عدد( ux_Timer_عدد )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_v_Timer_سيٽ_Timer_عدد
    #وصف_ڪريو trace_ENTER_v_Timer_سيٽ_Timer_عدد( xTimer, ux_Timer_عدد )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_v_Timer_سيٽ_Timer_عدد
    #وصف_ڪريو trace_RETURN_v_Timer_سيٽ_Timer_عدد()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_xTaskCreateStatic
    #وصف_ڪريو traceENTER_xTaskCreateStatic( px_Task_ڪوڊ, pcName, ux_ڍير_اونهائي, pvParameters, ux_ترجيح, pux_ڍير_بفر, px_Task_بفر )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_xTaskCreateStatic
    #وصف_ڪريو traceRETURN_xTaskCreateStatic( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_Task_Create_Static_Affinity_سيٽ
    #وصف_ڪريو trace_ENTER_x_Task_Create_Static_Affinity_سيٽ( px_Task_ڪوڊ, pcName, ux_ڍير_اونهائي, pvParameters, ux_ترجيح, pux_ڍير_بفر, px_Task_بفر, ux_Core_Affinity_نقاب )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_Task_Create_Static_Affinity_سيٽ
    #وصف_ڪريو trace_RETURN_x_Task_Create_Static_Affinity_سيٽ( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_xTaskCreateRestrictedStatic
    #وصف_ڪريو traceENTER_xTaskCreateRestrictedStatic( pxTaskDefinition, pxCreatedTask )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_xTaskCreateRestrictedStatic
    #وصف_ڪريو traceRETURN_xTaskCreateRestrictedStatic( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_Task_Create_Restricted_Static_Affinity_سيٽ
    #وصف_ڪريو trace_ENTER_x_Task_Create_Restricted_Static_Affinity_سيٽ( pxTaskDefinition, ux_Core_Affinity_نقاب, pxCreatedTask )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_Task_Create_Restricted_Static_Affinity_سيٽ
    #وصف_ڪريو trace_RETURN_x_Task_Create_Restricted_Static_Affinity_سيٽ( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_xTaskCreateRestricted
    #وصف_ڪريو traceENTER_xTaskCreateRestricted( pxTaskDefinition, pxCreatedTask )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_xTaskCreateRestricted
    #وصف_ڪريو traceRETURN_xTaskCreateRestricted( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_Task_Create_Restricted_Affinity_سيٽ
    #وصف_ڪريو trace_ENTER_x_Task_Create_Restricted_Affinity_سيٽ( pxTaskDefinition, ux_Core_Affinity_نقاب, pxCreatedTask )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_Task_Create_Restricted_Affinity_سيٽ
    #وصف_ڪريو trace_RETURN_x_Task_Create_Restricted_Affinity_سيٽ( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_xTaskCreate
    #وصف_ڪريو traceENTER_xTaskCreate( px_Task_ڪوڊ, pcName, ux_ڍير_اونهائي, pvParameters, ux_ترجيح, pxCreatedTask )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_xTaskCreate
    #وصف_ڪريو traceRETURN_xTaskCreate( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_Task_Create_Affinity_سيٽ
    #وصف_ڪريو trace_ENTER_x_Task_Create_Affinity_سيٽ( px_Task_ڪوڊ, pcName, ux_ڍير_اونهائي, pvParameters, ux_ترجيح, ux_Core_Affinity_نقاب, pxCreatedTask )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_Task_Create_Affinity_سيٽ
    #وصف_ڪريو trace_RETURN_x_Task_Create_Affinity_سيٽ( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_vTaskDelete
    #وصف_ڪريو traceENTER_vTaskDelete( x_Task_منزل_Delete )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_vTaskDelete
    #وصف_ڪريو traceRETURN_vTaskDelete()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_xTaskDelayUntil
    #وصف_ڪريو traceENTER_xTaskDelayUntil( px_اڳيون_Wake_وقت, x_وقت_Increment )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_xTaskDelayUntil
    #وصف_ڪريو traceRETURN_xTaskDelayUntil( xShouldDelay )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_vTaskDelay
    #وصف_ڪريو traceENTER_vTaskDelay( x_Ticks_منزل_Delay )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_vTaskDelay
    #وصف_ڪريو traceRETURN_vTaskDelay()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_eTaskGetState
    #وصف_ڪريو traceENTER_eTaskGetState( xTask )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_eTaskGetState
    #وصف_ڪريو traceRETURN_eTaskGetState( eReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_ux_Task_ترجيح_Get
    #وصف_ڪريو trace_ENTER_ux_Task_ترجيح_Get( xTask )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_ux_Task_ترجيح_Get
    #وصف_ڪريو trace_RETURN_ux_Task_ترجيح_Get( uxReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_ux_Task_ترجيح_Get_From_ISR
    #وصف_ڪريو trace_ENTER_ux_Task_ترجيح_Get_From_ISR( xTask )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_ux_Task_ترجيح_Get_From_ISR
    #وصف_ڪريو trace_RETURN_ux_Task_ترجيح_Get_From_ISR( uxReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_ux_Task_بنياد_ترجيح_Get
    #وصف_ڪريو trace_ENTER_ux_Task_بنياد_ترجيح_Get( xTask )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_ux_Task_بنياد_ترجيح_Get
    #وصف_ڪريو trace_RETURN_ux_Task_بنياد_ترجيح_Get( uxReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_ux_Task_بنياد_ترجيح_Get_From_ISR
    #وصف_ڪريو trace_ENTER_ux_Task_بنياد_ترجيح_Get_From_ISR( xTask )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_ux_Task_بنياد_ترجيح_Get_From_ISR
    #وصف_ڪريو trace_RETURN_ux_Task_بنياد_ترجيح_Get_From_ISR( uxReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_v_Task_ترجيح_سيٽ
    #وصف_ڪريو trace_ENTER_v_Task_ترجيح_سيٽ( xTask, ux_New_ترجيح )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_v_Task_ترجيح_سيٽ
    #وصف_ڪريو trace_RETURN_v_Task_ترجيح_سيٽ()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_v_Task_Core_Affinity_سيٽ
    #وصف_ڪريو trace_ENTER_v_Task_Core_Affinity_سيٽ( xTask, ux_Core_Affinity_نقاب )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_v_Task_Core_Affinity_سيٽ
    #وصف_ڪريو trace_RETURN_v_Task_Core_Affinity_سيٽ()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_vTaskCoreAffinityGet
    #وصف_ڪريو traceENTER_vTaskCoreAffinityGet( xTask )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_vTaskCoreAffinityGet
    #وصف_ڪريو traceRETURN_vTaskCoreAffinityGet( ux_Core_Affinity_نقاب )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_vTaskPreemptionDisable
    #وصف_ڪريو traceENTER_vTaskPreemptionDisable( xTask )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_vTaskPreemptionDisable
    #وصف_ڪريو traceRETURN_vTaskPreemptionDisable()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_vTaskPreemptionEnable
    #وصف_ڪريو traceENTER_vTaskPreemptionEnable( xTask )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_vTaskPreemptionEnable
    #وصف_ڪريو traceRETURN_vTaskPreemptionEnable()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_vTaskSuspend
    #وصف_ڪريو traceENTER_vTaskSuspend( x_Task_منزل_Suspend )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_vTaskSuspend
    #وصف_ڪريو traceRETURN_vTaskSuspend()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_vTaskResume
    #وصف_ڪريو traceENTER_vTaskResume( x_Task_منزل_Resume )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_vTaskResume
    #وصف_ڪريو traceRETURN_vTaskResume()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_xTaskResumeFromISR
    #وصف_ڪريو traceENTER_xTaskResumeFromISR( x_Task_منزل_Resume )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_xTaskResumeFromISR
    #وصف_ڪريو traceRETURN_xTaskResumeFromISR( xYieldRequired )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_v_Task_شروعات_Scheduler
    #وصف_ڪريو trace_ENTER_v_Task_شروعات_Scheduler()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_v_Task_شروعات_Scheduler
    #وصف_ڪريو trace_RETURN_v_Task_شروعات_Scheduler()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_v_Task_پڇاڙي_Scheduler
    #وصف_ڪريو trace_ENTER_v_Task_پڇاڙي_Scheduler()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_v_Task_پڇاڙي_Scheduler
    #وصف_ڪريو trace_RETURN_v_Task_پڇاڙي_Scheduler()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_vTaskSuspendAll
    #وصف_ڪريو traceENTER_vTaskSuspendAll()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_vTaskSuspendAll
    #وصف_ڪريو traceRETURN_vTaskSuspendAll()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_xTaskResumeAll
    #وصف_ڪريو traceENTER_xTaskResumeAll()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_xTaskResumeAll
    #وصف_ڪريو traceRETURN_xTaskResumeAll( xAlreadyYielded )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_Task_Get_Tick_ڳڻپ
    #وصف_ڪريو trace_ENTER_x_Task_Get_Tick_ڳڻپ()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_Task_Get_Tick_ڳڻپ
    #وصف_ڪريو trace_RETURN_x_Task_Get_Tick_ڳڻپ( xTicks )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_Task_Get_Tick_ڳڻپ_From_ISR
    #وصف_ڪريو trace_ENTER_x_Task_Get_Tick_ڳڻپ_From_ISR()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_Task_Get_Tick_ڳڻپ_From_ISR
    #وصف_ڪريو trace_RETURN_x_Task_Get_Tick_ڳڻپ_From_ISR( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_ux_Task_Get_عدد_جو_Tasks
    #وصف_ڪريو trace_ENTER_ux_Task_Get_عدد_جو_Tasks()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_ux_Task_Get_عدد_جو_Tasks
    #وصف_ڪريو trace_RETURN_ux_Task_Get_عدد_جو_Tasks( ux_Current_عدد_جو_Tasks )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_pcTaskGetName
    #وصف_ڪريو traceENTER_pcTaskGetName( x_Task_منزل_Query )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_pcTaskGetName
    #وصف_ڪريو traceRETURN_pcTaskGetName( pcTaskName )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_xTaskGetHandle
    #وصف_ڪريو traceENTER_xTaskGetHandle( pc_Name_منزل_Query )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_xTaskGetHandle
    #وصف_ڪريو traceRETURN_xTaskGetHandle( pxTCB )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_xTaskGetStaticBuffers
    #وصف_ڪريو traceENTER_xTaskGetStaticBuffers( xTask, ppux_ڍير_بفر, ppx_Task_بفر )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_xTaskGetStaticBuffers
    #وصف_ڪريو traceRETURN_xTaskGetStaticBuffers( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_uxTaskGetSystemState
    #وصف_ڪريو traceENTER_uxTaskGetSystemState( px_Task_حالت_Array, ux_Array_سائيز, pul_ڪل_لاڳيتو_وقت )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_uxTaskGetSystemState
    #وصف_ڪريو traceRETURN_uxTaskGetSystemState( uxTask )
#جيڪڏهن_پورو

#إذا ( config_عدد_جو_CORES == 1 )
    #جيڪڏهن_وصف_نه_ٿيل traceENTER_xTaskGetIdleTaskHandle
        #وصف_ڪريو traceENTER_xTaskGetIdleTaskHandle()
    #جيڪڏهن_پورو
#جيڪڏهن_پورو

#إذا ( config_عدد_جو_CORES == 1 )
    #جيڪڏهن_وصف_نه_ٿيل traceRETURN_xTaskGetIdleTaskHandle
        #وصف_ڪريو traceRETURN_xTaskGetIdleTaskHandle( xIdleTaskHandle )
    #جيڪڏهن_پورو
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_xTaskGetIdleTaskHandleForCore
    #وصف_ڪريو traceENTER_xTaskGetIdleTaskHandleForCore( xCoreID )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_xTaskGetIdleTaskHandleForCore
    #وصف_ڪريو traceRETURN_xTaskGetIdleTaskHandleForCore( xIdleTaskHandle )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_v_Task_قدم_Tick
    #وصف_ڪريو trace_ENTER_v_Task_قدم_Tick( x_Ticks_منزل_Jump )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_v_Task_قدم_Tick
    #وصف_ڪريو trace_RETURN_v_Task_قدم_Tick()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_Task_Catch_مٿي_Ticks
    #وصف_ڪريو trace_ENTER_x_Task_Catch_مٿي_Ticks( x_Ticks_منزل_Catch_مٿي )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_Task_Catch_مٿي_Ticks
    #وصف_ڪريو trace_RETURN_x_Task_Catch_مٿي_Ticks( xYieldOccurred )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_xTaskAbortDelay
    #وصف_ڪريو traceENTER_xTaskAbortDelay( xTask )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_xTaskAbortDelay
    #وصف_ڪريو traceRETURN_xTaskAbortDelay( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_xTaskIncrementTick
    #وصف_ڪريو traceENTER_xTaskIncrementTick()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_xTaskIncrementTick
    #وصف_ڪريو traceRETURN_xTaskIncrementTick( xSwitchRequired )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_v_Task_سيٽ_Application_Task_ٽيگ
    #وصف_ڪريو trace_ENTER_v_Task_سيٽ_Application_Task_ٽيگ( xTask, pxHookFunction )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_v_Task_سيٽ_Application_Task_ٽيگ
    #وصف_ڪريو trace_RETURN_v_Task_سيٽ_Application_Task_ٽيگ()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_Task_Get_Application_Task_ٽيگ
    #وصف_ڪريو trace_ENTER_x_Task_Get_Application_Task_ٽيگ( xTask )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_Task_Get_Application_Task_ٽيگ
    #وصف_ڪريو trace_RETURN_x_Task_Get_Application_Task_ٽيگ( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_Task_Get_Application_Task_ٽيگ_From_ISR
    #وصف_ڪريو trace_ENTER_x_Task_Get_Application_Task_ٽيگ_From_ISR( xTask )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_Task_Get_Application_Task_ٽيگ_From_ISR
    #وصف_ڪريو trace_RETURN_x_Task_Get_Application_Task_ٽيگ_From_ISR( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_xTaskCallApplicationTaskHook
    #وصف_ڪريو traceENTER_xTaskCallApplicationTaskHook( xTask, pvParameter )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_xTaskCallApplicationTaskHook
    #وصف_ڪريو traceRETURN_xTaskCallApplicationTaskHook( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_vTaskSwitchContext
    #وصف_ڪريو traceENTER_vTaskSwitchContext()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_vTaskSwitchContext
    #وصف_ڪريو traceRETURN_vTaskSwitchContext()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_v_Task_رکو_On_Event_List
    #وصف_ڪريو trace_ENTER_v_Task_رکو_On_Event_List( pxEventList, x_Ticks_منزل_Wait )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_v_Task_رکو_On_Event_List
    #وصف_ڪريو trace_RETURN_v_Task_رکو_On_Event_List()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_v_Task_رکو_On_Unordered_Event_List
    #وصف_ڪريو trace_ENTER_v_Task_رکو_On_Unordered_Event_List( pxEventList, x_شي_قدر, x_Ticks_منزل_Wait )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_v_Task_رکو_On_Unordered_Event_List
    #وصف_ڪريو trace_RETURN_v_Task_رکو_On_Unordered_Event_List()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_v_Task_رکو_On_Event_List_Restricted
    #وصف_ڪريو trace_ENTER_v_Task_رکو_On_Event_List_Restricted( pxEventList, x_Ticks_منزل_Wait, xWaitIndefinitely )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_v_Task_رکو_On_Event_List_Restricted
    #وصف_ڪريو trace_RETURN_v_Task_رکو_On_Event_List_Restricted()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_xTaskRemoveFromEventList
    #وصف_ڪريو traceENTER_xTaskRemoveFromEventList( pxEventList )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_xTaskRemoveFromEventList
    #وصف_ڪريو traceRETURN_xTaskRemoveFromEventList( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_vTaskRemoveFromUnorderedEventList
    #وصف_ڪريو traceENTER_vTaskRemoveFromUnorderedEventList( px_Event_List_شي, x_شي_قدر )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_vTaskRemoveFromUnorderedEventList
    #وصف_ڪريو traceRETURN_vTaskRemoveFromUnorderedEventList()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_v_Task_سيٽ_وقت_ٻاهريون_State
    #وصف_ڪريو trace_ENTER_v_Task_سيٽ_وقت_ٻاهريون_State( px_وقت_ٻاهريون )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_v_Task_سيٽ_وقت_ٻاهريون_State
    #وصف_ڪريو trace_RETURN_v_Task_سيٽ_وقت_ٻاهريون_State()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_v_Task_اندروني_سيٽ_وقت_ٻاهريون_State
    #وصف_ڪريو trace_ENTER_v_Task_اندروني_سيٽ_وقت_ٻاهريون_State( px_وقت_ٻاهريون )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_v_Task_اندروني_سيٽ_وقت_ٻاهريون_State
    #وصف_ڪريو trace_RETURN_v_Task_اندروني_سيٽ_وقت_ٻاهريون_State()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_Task_Check_For_وقت_ٻاهريون
    #وصف_ڪريو trace_ENTER_x_Task_Check_For_وقت_ٻاهريون( px_وقت_ٻاهريون, px_Ticks_منزل_Wait )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_Task_Check_For_وقت_ٻاهريون
    #وصف_ڪريو trace_RETURN_x_Task_Check_For_وقت_ٻاهريون( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_vTaskMissedYield
    #وصف_ڪريو traceENTER_vTaskMissedYield()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_vTaskMissedYield
    #وصف_ڪريو traceRETURN_vTaskMissedYield()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_ux_Task_Get_Task_عدد
    #وصف_ڪريو trace_ENTER_ux_Task_Get_Task_عدد( xTask )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_ux_Task_Get_Task_عدد
    #وصف_ڪريو trace_RETURN_ux_Task_Get_Task_عدد( uxReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_v_Task_سيٽ_Task_عدد
    #وصف_ڪريو trace_ENTER_v_Task_سيٽ_Task_عدد( xTask, uxHandle )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_v_Task_سيٽ_Task_عدد
    #وصف_ڪريو trace_RETURN_v_Task_سيٽ_Task_عدد()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_e_Task_Confirm_Sleep_گهڻائي_حالت
    #وصف_ڪريو trace_ENTER_e_Task_Confirm_Sleep_گهڻائي_حالت()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_e_Task_Confirm_Sleep_گهڻائي_حالت
    #وصف_ڪريو trace_RETURN_e_Task_Confirm_Sleep_گهڻائي_حالت( eReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_v_Task_سيٽ_Thread_Local_Storage_اشارو
    #وصف_ڪريو trace_ENTER_v_Task_سيٽ_Thread_Local_Storage_اشارو( x_Task_منزل_سيٽ, x_اشاريو, pv_قدر )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_v_Task_سيٽ_Thread_Local_Storage_اشارو
    #وصف_ڪريو trace_RETURN_v_Task_سيٽ_Thread_Local_Storage_اشارو()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_pv_Task_Get_Thread_Local_Storage_اشارو
    #وصف_ڪريو trace_ENTER_pv_Task_Get_Thread_Local_Storage_اشارو( x_Task_منزل_Query, x_اشاريو )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_pv_Task_Get_Thread_Local_Storage_اشارو
    #وصف_ڪريو trace_RETURN_pv_Task_Get_Thread_Local_Storage_اشارو( pvReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_vTaskAllocateMPURegions
    #وصف_ڪريو traceENTER_vTaskAllocateMPURegions( x_Task_منزل_Modify, pxRegions )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_vTaskAllocateMPURegions
    #وصف_ڪريو traceRETURN_vTaskAllocateMPURegions()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_vTaskGetInfo
    #وصف_ڪريو traceENTER_vTaskGetInfo( xTask, px_Task_حالت, x_Get_Free_ڍير_Space, eState )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_vTaskGetInfo
    #وصف_ڪريو traceRETURN_vTaskGetInfo()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_ux_Task_Get_ڍير_مٿاهون_Water_Mark2
    #وصف_ڪريو trace_ENTER_ux_Task_Get_ڍير_مٿاهون_Water_Mark2( xTask )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_ux_Task_Get_ڍير_مٿاهون_Water_Mark2
    #وصف_ڪريو trace_RETURN_ux_Task_Get_ڍير_مٿاهون_Water_Mark2( uxReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_ux_Task_Get_ڍير_مٿاهون_Water_Mark
    #وصف_ڪريو trace_ENTER_ux_Task_Get_ڍير_مٿاهون_Water_Mark( xTask )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_ux_Task_Get_ڍير_مٿاهون_Water_Mark
    #وصف_ڪريو trace_RETURN_ux_Task_Get_ڍير_مٿاهون_Water_Mark( uxReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_xTaskGetCurrentTaskHandle
    #وصف_ڪريو traceENTER_xTaskGetCurrentTaskHandle()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_xTaskGetCurrentTaskHandle
    #وصف_ڪريو traceRETURN_xTaskGetCurrentTaskHandle( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_xTaskGetCurrentTaskHandleForCore
    #وصف_ڪريو traceENTER_xTaskGetCurrentTaskHandleForCore( xCoreID )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_xTaskGetCurrentTaskHandleForCore
    #وصف_ڪريو traceRETURN_xTaskGetCurrentTaskHandleForCore( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_xTaskGetSchedulerState
    #وصف_ڪريو traceENTER_xTaskGetSchedulerState()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_xTaskGetSchedulerState
    #وصف_ڪريو traceRETURN_xTaskGetSchedulerState( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_Task_ترجيح_Inherit
    #وصف_ڪريو trace_ENTER_x_Task_ترجيح_Inherit( pxMutexHolder )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_Task_ترجيح_Inherit
    #وصف_ڪريو trace_RETURN_x_Task_ترجيح_Inherit( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_Task_ترجيح_Disinherit
    #وصف_ڪريو trace_ENTER_x_Task_ترجيح_Disinherit( pxMutexHolder )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_Task_ترجيح_Disinherit
    #وصف_ڪريو trace_RETURN_x_Task_ترجيح_Disinherit( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_v_Task_ترجيح_Disinherit_After_Timeout
    #وصف_ڪريو trace_ENTER_v_Task_ترجيح_Disinherit_After_Timeout( pxMutexHolder, ux_Highest_ترجيح_انتظار_Task )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_v_Task_ترجيح_Disinherit_After_Timeout
    #وصف_ڪريو trace_RETURN_v_Task_ترجيح_Disinherit_After_Timeout()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_vTaskYieldWithinAPI
    #وصف_ڪريو traceENTER_vTaskYieldWithinAPI()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_vTaskYieldWithinAPI
    #وصف_ڪريو traceRETURN_vTaskYieldWithinAPI()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_vTaskEnterCritical
    #وصف_ڪريو traceENTER_vTaskEnterCritical()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_vTaskEnterCritical
    #وصف_ڪريو traceRETURN_vTaskEnterCritical()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_vTaskEnterCriticalFromISR
    #وصف_ڪريو traceENTER_vTaskEnterCriticalFromISR()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_vTaskEnterCriticalFromISR
    #وصف_ڪريو traceRETURN_vTaskEnterCriticalFromISR( ux_Saved_مداخلت_حالت )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_vTaskExitCritical
    #وصف_ڪريو traceENTER_vTaskExitCritical()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_vTaskExitCritical
    #وصف_ڪريو traceRETURN_vTaskExitCritical()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_vTaskExitCriticalFromISR
    #وصف_ڪريو traceENTER_vTaskExitCriticalFromISR( ux_Saved_مداخلت_حالت )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_vTaskExitCriticalFromISR
    #وصف_ڪريو traceRETURN_vTaskExitCriticalFromISR()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_vTaskListTasks
    #وصف_ڪريو traceENTER_vTaskListTasks( pc_Write_بفر, ux_بفر_ڊيگهائي )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_vTaskListTasks
    #وصف_ڪريو traceRETURN_vTaskListTasks()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_v_Task_Get_لاڳيتو_وقت_Statistics
    #وصف_ڪريو trace_ENTER_v_Task_Get_لاڳيتو_وقت_Statistics( pc_Write_بفر, ux_بفر_ڊيگهائي )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_v_Task_Get_لاڳيتو_وقت_Statistics
    #وصف_ڪريو trace_RETURN_v_Task_Get_لاڳيتو_وقت_Statistics()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_ux_Task_Reset_Event_شي_قدر
    #وصف_ڪريو trace_ENTER_ux_Task_Reset_Event_شي_قدر()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_ux_Task_Reset_Event_شي_قدر
    #وصف_ڪريو trace_RETURN_ux_Task_Reset_Event_شي_قدر( uxReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_pv_Task_Increment_Mutex_Held_ڳڻپ
    #وصف_ڪريو trace_ENTER_pv_Task_Increment_Mutex_Held_ڳڻپ()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_pv_Task_Increment_Mutex_Held_ڳڻپ
    #وصف_ڪريو trace_RETURN_pv_Task_Increment_Mutex_Held_ڳڻپ( pxTCB )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_ulTaskGenericNotifyTake
    #وصف_ڪريو traceENTER_ulTaskGenericNotifyTake( ux_اشاريو_منزل_Wait_On, x_Clear_ڳڻپ_On_Exit, x_Ticks_منزل_Wait )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_ulTaskGenericNotifyTake
    #وصف_ڪريو traceRETURN_ulTaskGenericNotifyTake( ulReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_xTaskGenericNotifyWait
    #وصف_ڪريو traceENTER_xTaskGenericNotifyWait( ux_اشاريو_منزل_Wait_On, ul_بٽ_منزل_Clear_On_داخلا, ul_بٽ_منزل_Clear_On_Exit, pul_Notification_قدر, x_Ticks_منزل_Wait )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_xTaskGenericNotifyWait
    #وصف_ڪريو traceRETURN_xTaskGenericNotifyWait( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_xTaskGenericNotify
    #وصف_ڪريو traceENTER_xTaskGenericNotify( x_Task_منزل_Notify, ux_اشاريو_منزل_Notify, ul_قدر, eAction, pul_اڳيون_Notification_قدر )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_xTaskGenericNotify
    #وصف_ڪريو traceRETURN_xTaskGenericNotify( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_xTaskGenericNotifyFromISR
    #وصف_ڪريو traceENTER_xTaskGenericNotifyFromISR( x_Task_منزل_Notify, ux_اشاريو_منزل_Notify, ul_قدر, eAction, pul_اڳيون_Notification_قدر, px_Higher_ترجيح_Task_Woken )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_xTaskGenericNotifyFromISR
    #وصف_ڪريو traceRETURN_xTaskGenericNotifyFromISR( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_vTaskGenericNotifyGiveFromISR
    #وصف_ڪريو traceENTER_vTaskGenericNotifyGiveFromISR( x_Task_منزل_Notify, ux_اشاريو_منزل_Notify, px_Higher_ترجيح_Task_Woken )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_vTaskGenericNotifyGiveFromISR
    #وصف_ڪريو traceRETURN_vTaskGenericNotifyGiveFromISR()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_xTaskGenericNotifyStateClear
    #وصف_ڪريو traceENTER_xTaskGenericNotifyStateClear( xTask, ux_اشاريو_منزل_Clear )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_xTaskGenericNotifyStateClear
    #وصف_ڪريو traceRETURN_xTaskGenericNotifyStateClear( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_ul_Task_Generic_Notify_قدر_Clear
    #وصف_ڪريو trace_ENTER_ul_Task_Generic_Notify_قدر_Clear( xTask, ux_اشاريو_منزل_Clear, ul_بٽ_منزل_Clear )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_ul_Task_Generic_Notify_قدر_Clear
    #وصف_ڪريو trace_RETURN_ul_Task_Generic_Notify_قدر_Clear( ulReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_ul_Task_Get_لاڳيتو_وقت_Counter
    #وصف_ڪريو trace_ENTER_ul_Task_Get_لاڳيتو_وقت_Counter( xTask )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_ul_Task_Get_لاڳيتو_وقت_Counter
    #وصف_ڪريو trace_RETURN_ul_Task_Get_لاڳيتو_وقت_Counter( ul_لاڳيتو_وقت_Counter )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_ul_Task_Get_لاڳيتو_وقت_Percent
    #وصف_ڪريو trace_ENTER_ul_Task_Get_لاڳيتو_وقت_Percent( xTask )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_ul_Task_Get_لاڳيتو_وقت_Percent
    #وصف_ڪريو trace_RETURN_ul_Task_Get_لاڳيتو_وقت_Percent( ulReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_ul_Task_Get_Idle_لاڳيتو_وقت_Counter
    #وصف_ڪريو trace_ENTER_ul_Task_Get_Idle_لاڳيتو_وقت_Counter()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_ul_Task_Get_Idle_لاڳيتو_وقت_Counter
    #وصف_ڪريو trace_RETURN_ul_Task_Get_Idle_لاڳيتو_وقت_Counter( ulReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_ul_Task_Get_Idle_لاڳيتو_وقت_Percent
    #وصف_ڪريو trace_ENTER_ul_Task_Get_Idle_لاڳيتو_وقت_Percent()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_ul_Task_Get_Idle_لاڳيتو_وقت_Percent
    #وصف_ڪريو trace_RETURN_ul_Task_Get_Idle_لاڳيتو_وقت_Percent( ulReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_xTaskGetMPUSettings
    #وصف_ڪريو traceENTER_xTaskGetMPUSettings( xTask )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_xTaskGetMPUSettings
    #وصف_ڪريو traceRETURN_xTaskGetMPUSettings( xMPUSettings )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_وهڪرو_بفر_Generic_Create
    #وصف_ڪريو trace_ENTER_x_وهڪرو_بفر_Generic_Create( x_بفر_سائيز_بائيٽون, x_Trigger_Level_بائيٽون, x_وهڪرو_بفر_Type, pxSendCompletedCallback, pxReceiveCompletedCallback )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_وهڪرو_بفر_Generic_Create
    #وصف_ڪريو trace_RETURN_x_وهڪرو_بفر_Generic_Create( pv_Allocated_يادگيري )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_وهڪرو_بفر_Generic_Create_Static
    #وصف_ڪريو trace_ENTER_x_وهڪرو_بفر_Generic_Create_Static( x_بفر_سائيز_بائيٽون, x_Trigger_Level_بائيٽون, x_وهڪرو_بفر_Type, puc_وهڪرو_بفر_Storage_Area, px_Static_وهڪرو_بفر, pxSendCompletedCallback, pxReceiveCompletedCallback )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_وهڪرو_بفر_Generic_Create_Static
    #وصف_ڪريو trace_RETURN_x_وهڪرو_بفر_Generic_Create_Static( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_وهڪرو_بفر_Get_Static_Buffers
    #وصف_ڪريو trace_ENTER_x_وهڪرو_بفر_Get_Static_Buffers( x_وهڪرو_بفر, ppuc_وهڪرو_بفر_Storage_Area, ppx_Static_وهڪرو_بفر )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_وهڪرو_بفر_Get_Static_Buffers
    #وصف_ڪريو trace_RETURN_x_وهڪرو_بفر_Get_Static_Buffers( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_v_وهڪرو_بفر_Delete
    #وصف_ڪريو trace_ENTER_v_وهڪرو_بفر_Delete( x_وهڪرو_بفر )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_v_وهڪرو_بفر_Delete
    #وصف_ڪريو trace_RETURN_v_وهڪرو_بفر_Delete()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_وهڪرو_بفر_Reset
    #وصف_ڪريو trace_ENTER_x_وهڪرو_بفر_Reset( x_وهڪرو_بفر )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_وهڪرو_بفر_Reset
    #وصف_ڪريو trace_RETURN_x_وهڪرو_بفر_Reset( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_وهڪرو_بفر_Reset_From_ISR
    #وصف_ڪريو trace_ENTER_x_وهڪرو_بفر_Reset_From_ISR( x_وهڪرو_بفر )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_وهڪرو_بفر_Reset_From_ISR
    #وصف_ڪريو trace_RETURN_x_وهڪرو_بفر_Reset_From_ISR( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_وهڪرو_بفر_سيٽ_Trigger_Level
    #وصف_ڪريو trace_ENTER_x_وهڪرو_بفر_سيٽ_Trigger_Level( x_وهڪرو_بفر, xTriggerLevel )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_وهڪرو_بفر_سيٽ_Trigger_Level
    #وصف_ڪريو trace_RETURN_x_وهڪرو_بفر_سيٽ_Trigger_Level( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_وهڪرو_بفر_Spaces_موجود
    #وصف_ڪريو trace_ENTER_x_وهڪرو_بفر_Spaces_موجود( x_وهڪرو_بفر )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_وهڪرو_بفر_Spaces_موجود
    #وصف_ڪريو trace_RETURN_x_وهڪرو_بفر_Spaces_موجود( xSpace )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_وهڪرو_بفر_بائيٽون_موجود
    #وصف_ڪريو trace_ENTER_x_وهڪرو_بفر_بائيٽون_موجود( x_وهڪرو_بفر )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_وهڪرو_بفر_بائيٽون_موجود
    #وصف_ڪريو trace_RETURN_x_وهڪرو_بفر_بائيٽون_موجود( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_وهڪرو_بفر_Send
    #وصف_ڪريو trace_ENTER_x_وهڪرو_بفر_Send( x_وهڪرو_بفر, pv_Tx_معلومات, x_معلومات_ڊيگهائي_بائيٽون, x_Ticks_منزل_Wait )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_وهڪرو_بفر_Send
    #وصف_ڪريو trace_RETURN_x_وهڪرو_بفر_Send( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_وهڪرو_بفر_Send_From_ISR
    #وصف_ڪريو trace_ENTER_x_وهڪرو_بفر_Send_From_ISR( x_وهڪرو_بفر, pv_Tx_معلومات, x_معلومات_ڊيگهائي_بائيٽون, px_Higher_ترجيح_Task_Woken )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_وهڪرو_بفر_Send_From_ISR
    #وصف_ڪريو trace_RETURN_x_وهڪرو_بفر_Send_From_ISR( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_وهڪرو_بفر_Receive
    #وصف_ڪريو trace_ENTER_x_وهڪرو_بفر_Receive( x_وهڪرو_بفر, pv_Rx_معلومات, x_بفر_ڊيگهائي_بائيٽون, x_Ticks_منزل_Wait )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_وهڪرو_بفر_Receive
    #وصف_ڪريو trace_RETURN_x_وهڪرو_بفر_Receive( x_Received_ڊيگهائي )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_وهڪرو_بفر_ايندڙ_پيغام_ڊيگهائي_بائيٽون
    #وصف_ڪريو trace_ENTER_x_وهڪرو_بفر_ايندڙ_پيغام_ڊيگهائي_بائيٽون( x_وهڪرو_بفر )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_وهڪرو_بفر_ايندڙ_پيغام_ڊيگهائي_بائيٽون
    #وصف_ڪريو trace_RETURN_x_وهڪرو_بفر_ايندڙ_پيغام_ڊيگهائي_بائيٽون( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_وهڪرو_بفر_Receive_From_ISR
    #وصف_ڪريو trace_ENTER_x_وهڪرو_بفر_Receive_From_ISR( x_وهڪرو_بفر, pv_Rx_معلومات, x_بفر_ڊيگهائي_بائيٽون, px_Higher_ترجيح_Task_Woken )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_وهڪرو_بفر_Receive_From_ISR
    #وصف_ڪريو trace_RETURN_x_وهڪرو_بفر_Receive_From_ISR( x_Received_ڊيگهائي )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_وهڪرو_بفر_Is_خالي_جايون
    #وصف_ڪريو trace_ENTER_x_وهڪرو_بفر_Is_خالي_جايون( x_وهڪرو_بفر )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_وهڪرو_بفر_Is_خالي_جايون
    #وصف_ڪريو trace_RETURN_x_وهڪرو_بفر_Is_خالي_جايون( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_وهڪرو_بفر_Is_ڀريل
    #وصف_ڪريو trace_ENTER_x_وهڪرو_بفر_Is_ڀريل( x_وهڪرو_بفر )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_وهڪرو_بفر_Is_ڀريل
    #وصف_ڪريو trace_RETURN_x_وهڪرو_بفر_Is_ڀريل( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_وهڪرو_بفر_Send_Completed_From_ISR
    #وصف_ڪريو trace_ENTER_x_وهڪرو_بفر_Send_Completed_From_ISR( x_وهڪرو_بفر, px_Higher_ترجيح_Task_Woken )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_وهڪرو_بفر_Send_Completed_From_ISR
    #وصف_ڪريو trace_RETURN_x_وهڪرو_بفر_Send_Completed_From_ISR( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_x_وهڪرو_بفر_Receive_Completed_From_ISR
    #وصف_ڪريو trace_ENTER_x_وهڪرو_بفر_Receive_Completed_From_ISR( x_وهڪرو_بفر, px_Higher_ترجيح_Task_Woken )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_x_وهڪرو_بفر_Receive_Completed_From_ISR
    #وصف_ڪريو trace_RETURN_x_وهڪرو_بفر_Receive_Completed_From_ISR( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_ux_وهڪرو_بفر_Get_وهڪرو_بفر_Notification_اشاريو
    #وصف_ڪريو trace_ENTER_ux_وهڪرو_بفر_Get_وهڪرو_بفر_Notification_اشاريو( x_وهڪرو_بفر )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_ux_وهڪرو_بفر_Get_وهڪرو_بفر_Notification_اشاريو
    #وصف_ڪريو trace_RETURN_ux_وهڪرو_بفر_Get_وهڪرو_بفر_Notification_اشاريو( ux_Notification_اشاريو )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_v_وهڪرو_بفر_سيٽ_وهڪرو_بفر_Notification_اشاريو
    #وصف_ڪريو trace_ENTER_v_وهڪرو_بفر_سيٽ_وهڪرو_بفر_Notification_اشاريو( x_وهڪرو_بفر, ux_Notification_اشاريو )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_v_وهڪرو_بفر_سيٽ_وهڪرو_بفر_Notification_اشاريو
    #وصف_ڪريو trace_RETURN_v_وهڪرو_بفر_سيٽ_وهڪرو_بفر_Notification_اشاريو()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_ux_وهڪرو_بفر_Get_وهڪرو_بفر_عدد
    #وصف_ڪريو trace_ENTER_ux_وهڪرو_بفر_Get_وهڪرو_بفر_عدد( x_وهڪرو_بفر )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_ux_وهڪرو_بفر_Get_وهڪرو_بفر_عدد
    #وصف_ڪريو trace_RETURN_ux_وهڪرو_بفر_Get_وهڪرو_بفر_عدد( ux_وهڪرو_بفر_عدد )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_v_وهڪرو_بفر_سيٽ_وهڪرو_بفر_عدد
    #وصف_ڪريو trace_ENTER_v_وهڪرو_بفر_سيٽ_وهڪرو_بفر_عدد( x_وهڪرو_بفر, ux_وهڪرو_بفر_عدد )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_v_وهڪرو_بفر_سيٽ_وهڪرو_بفر_عدد
    #وصف_ڪريو trace_RETURN_v_وهڪرو_بفر_سيٽ_وهڪرو_بفر_عدد()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_uc_وهڪرو_بفر_Get_وهڪرو_بفر_Type
    #وصف_ڪريو trace_ENTER_uc_وهڪرو_بفر_Get_وهڪرو_بفر_Type( x_وهڪرو_بفر )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_uc_وهڪرو_بفر_Get_وهڪرو_بفر_Type
    #وصف_ڪريو trace_RETURN_uc_وهڪرو_بفر_Get_وهڪرو_بفر_Type( uc_وهڪرو_بفر_Type )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_vListInitialise
    #وصف_ڪريو traceENTER_vListInitialise( pxList )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_vListInitialise
    #وصف_ڪريو traceRETURN_vListInitialise()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_v_List_Initialise_شي
    #وصف_ڪريو trace_ENTER_v_List_Initialise_شي( px_شي )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_v_List_Initialise_شي
    #وصف_ڪريو trace_RETURN_v_List_Initialise_شي()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_v_List_وجهو_پڇاڙي
    #وصف_ڪريو trace_ENTER_v_List_وجهو_پڇاڙي( pxList, px_New_List_شي )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_v_List_وجهو_پڇاڙي
    #وصف_ڪريو trace_RETURN_v_List_وجهو_پڇاڙي()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_v_List_وجهو
    #وصف_ڪريو trace_ENTER_v_List_وجهو( pxList, px_New_List_شي )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_v_List_وجهو
    #وصف_ڪريو trace_RETURN_v_List_وجهو()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_uxListRemove
    #وصف_ڪريو traceENTER_uxListRemove( px_شي_منزل_Remove )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_uxListRemove
    #وصف_ڪريو traceRETURN_uxListRemove( ux_عدد_جو_شيون )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_xCoRoutineCreate
    #وصف_ڪريو traceENTER_xCoRoutineCreate( px_Co_Routine_ڪوڊ, ux_ترجيح, ux_اشاريو )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_xCoRoutineCreate
    #وصف_ڪريو traceRETURN_xCoRoutineCreate( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_v_Co_Routine_جوڙيو_منزل_Delayed_List
    #وصف_ڪريو trace_ENTER_v_Co_Routine_جوڙيو_منزل_Delayed_List( x_Ticks_منزل_Delay, pxEventList )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_v_Co_Routine_جوڙيو_منزل_Delayed_List
    #وصف_ڪريو trace_RETURN_v_Co_Routine_جوڙيو_منزل_Delayed_List()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_ENTER_v_Co_Routine_وقت_جدول
    #وصف_ڪريو trace_ENTER_v_Co_Routine_وقت_جدول()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل trace_RETURN_v_Co_Routine_وقت_جدول
    #وصف_ڪريو trace_RETURN_v_Co_Routine_وقت_جدول()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceENTER_xCoRoutineRemoveFromEventList
    #وصف_ڪريو traceENTER_xCoRoutineRemoveFromEventList( pxEventList )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل traceRETURN_xCoRoutineRemoveFromEventList
    #وصف_ڪريو traceRETURN_xCoRoutineRemoveFromEventList( xReturn )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل config_GENERATE_لاڳيتو_وقت_STATS
    #وصف_ڪريو config_GENERATE_لاڳيتو_وقت_STATS    0
#جيڪڏهن_پورو

#إذا ( config_GENERATE_لاڳيتو_وقت_STATS == 1 )

    #جيڪڏهن_وصف_نه_ٿيل پورٽ_CONFIGURE_TIMER_FOR_لاڳيتو_وقت_STATS
        #غلطي If config_GENERATE_لاڳيتو_وقت_STATS is defined then پورٽ_CONFIGURE_TIMER_FOR_لاڳيتو_وقت_STATS must also be defined.  پورٽ_CONFIGURE_TIMER_FOR_لاڳيتو_وقت_STATS should call a پورٽ layer function منزل تياري a peripheral timer/counter that can then be استعمال_ٿيل as the لاڳيتو time counter time بنياد.
    #جيڪڏهن_پورو /* portCONFIGURE_TIMER_FOR_RUN_TIME_STATS */

    #جيڪڏهن_وصف_نه_ٿيل پورٽ_GET_لاڳيتو_وقت_COUNTER_قدر
        #جيڪڏهن_وصف_نه_ٿيل پورٽ_ALT_GET_لاڳيتو_وقت_COUNTER_قدر
            #غلطي If config_GENERATE_لاڳيتو_وقت_STATS is defined then either پورٽ_GET_لاڳيتو_وقت_COUNTER_قدر or پورٽ_ALT_GET_لاڳيتو_وقت_COUNTER_قدر must also be defined.  See the examples provided and the FreeRTOS web site لكل وڌيڪ information.
        #جيڪڏهن_پورو /* portALT_GET_RUN_TIME_COUNTER_VALUE */
    #جيڪڏهن_پورو /* portGET_RUN_TIME_COUNTER_VALUE */

#جيڪڏهن_پورو /* configGENERATE_RUN_TIME_STATS */

#جيڪڏهن_وصف_نه_ٿيل پورٽ_CONFIGURE_TIMER_FOR_لاڳيتو_وقت_STATS
    #وصف_ڪريو پورٽ_CONFIGURE_TIMER_FOR_لاڳيتو_وقت_STATS()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل پورٽ_PRIVILEGE_ٻٽو_انگ
    #وصف_ڪريو پورٽ_PRIVILEGE_ٻٽو_انگ    ( ( UBaseType_t ) 0x00 )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل پورٽ_YIELD_WITHIN_API
    #وصف_ڪريو پورٽ_YIELD_WITHIN_API    پورٽ_YIELD
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل پورٽ_SUPPRESS_TICKS_AND_SLEEP
    #وصف_ڪريو پورٽ_SUPPRESS_TICKS_AND_SLEEP( x_Expected_Idle_وقت )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل config_EXPECTED_IDLE_وقت_BEFORE_SLEEP
    #وصف_ڪريو config_EXPECTED_IDLE_وقت_BEFORE_SLEEP    2
#جيڪڏهن_پورو

#إذا config_EXPECTED_IDLE_وقت_BEFORE_SLEEP < 2
    #غلطي config_EXPECTED_IDLE_وقت_BEFORE_SLEEP must not be less than 2
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل configUSE_TICKLESS_IDLE
    #وصف_ڪريو configUSE_TICKLESS_IDLE    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل configPRE_SUPPRESS_TICKS_AND_SLEEP_PROCESSING
    #وصف_ڪريو configPRE_SUPPRESS_TICKS_AND_SLEEP_PROCESSING( x )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل configPRE_SLEEP_PROCESSING
    #وصف_ڪريو configPRE_SLEEP_PROCESSING( x )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل configPOST_SLEEP_PROCESSING
    #وصف_ڪريو configPOST_SLEEP_PROCESSING( x )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل config_USE_قطار_SETS
    #وصف_ڪريو config_USE_قطار_SETS    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل پورٽ_TASK_استعمال_ڪري_FLOATING_نقطو
    #وصف_ڪريو پورٽ_TASK_استعمال_ڪري_FLOATING_نقطو()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل پورٽ_ALLOCATE_SECURE_CONTEXT
    #وصف_ڪريو پورٽ_ALLOCATE_SECURE_CONTEXT( ul_Secure_ڍير_سائيز )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل پورٽ_DONT_DISCARD
    #وصف_ڪريو پورٽ_DONT_DISCARD
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل config_USE_وقت_SLICING
    #وصف_ڪريو config_USE_وقت_SLICING    1
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS
    #وصف_ڪريو configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل configUSE_STATS_FORMATTING_FUNCTIONS
    #وصف_ڪريو configUSE_STATS_FORMATTING_FUNCTIONS    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل پورٽ_ASSERT_IF_مداخلت_ترجيح_INVALID
    #وصف_ڪريو پورٽ_ASSERT_IF_مداخلت_ترجيح_INVALID()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل configUSE_TRACE_FACILITY
    #وصف_ڪريو configUSE_TRACE_FACILITY    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل mtCOVERAGE_TEST_MARKER
    #وصف_ڪريو mtCOVERAGE_TEST_MARKER()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل mtCOVERAGE_TEST_DELAY
    #وصف_ڪريو mtCOVERAGE_TEST_DELAY()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل پورٽ_ASSERT_IF_اندريون_ISR
    #وصف_ڪريو پورٽ_ASSERT_IF_اندريون_ISR()
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل config_USE_پورٽ_OPTIMISED_TASK_SELECTION
    #وصف_ڪريو config_USE_پورٽ_OPTIMISED_TASK_SELECTION    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل config_APPLICATION_ALLOCATED_ڍير
    #وصف_ڪريو config_APPLICATION_ALLOCATED_ڍير    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل config_ENABLE_ڍير_PROTECTOR
    #وصف_ڪريو config_ENABLE_ڍير_PROTECTOR    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل configUSE_TASK_NOTIFICATIONS
    #وصف_ڪريو configUSE_TASK_NOTIFICATIONS    1
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل configTASK_NOTIFICATION_ARRAY_ENTRIES
    #وصف_ڪريو configTASK_NOTIFICATION_ARRAY_ENTRIES    1
#جيڪڏهن_پورو

#إذا configTASK_NOTIFICATION_ARRAY_ENTRIES < 1
    #غلطي configTASK_NOTIFICATION_ARRAY_ENTRIES must be at گهٽ_کان_گهٽ 1
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل configUSE_POSIX_ERRNO
    #وصف_ڪريو configUSE_POSIX_ERRNO    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل configUSE_SB_COMPLETED_CALLBACK

/* By default per-instance callbacks are not enabled for stream buffer or message buffer. */
    #وصف_ڪريو configUSE_SB_COMPLETED_CALLBACK    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل پورٽ_TICK_TYPE_IS_ATOMIC
    #وصف_ڪريو پورٽ_TICK_TYPE_IS_ATOMIC    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل config_SUPPORT_STATIC_ورهاست
    /* Defaults to 0 for backward compatibility. */
    #وصف_ڪريو config_SUPPORT_STATIC_ورهاست    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل config_KERNEL_PROVIDED_STATIC_يادگيري
    #وصف_ڪريو config_KERNEL_PROVIDED_STATIC_يادگيري    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل config_SUPPORT_DYNAMIC_ورهاست
    /* Defaults to 1 for backward compatibility. */
    #وصف_ڪريو config_SUPPORT_DYNAMIC_ورهاست    1
#جيڪڏهن_پورو

#إذا ( ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 ) && ( config_SUPPORT_DYNAMIC_ورهاست != 1 ) )
    #غلطي configUSE_STATS_FORMATTING_FUNCTIONS cannot be استعمال_ٿيل without dynamic ورهاست, but config_SUPPORT_DYNAMIC_ورهاست is not سيٽ منزل 1.
#جيڪڏهن_پورو

#إذا ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 )
    #إذا ( ( configUSE_TRACE_FACILITY != 1 ) && ( config_GENERATE_لاڳيتو_وقت_STATS != 1 ) )
        #غلطي configUSE_STATS_FORMATTING_FUNCTIONS is 1 but the functions it enables are not استعمال_ٿيل because neither configUSE_TRACE_FACILITY or config_GENERATE_لاڳيتو_وقت_STATS are 1.  سيٽ_2 configUSE_STATS_FORMATTING_FUNCTIONS منزل 0 اندريون FreeRTOSConfig.h.
    #جيڪڏهن_پورو
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل config_STATS_بفر_MAX_ڊيگهائي
    #وصف_ڪريو config_STATS_بفر_MAX_ڊيگهائي    0xFFFF
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل config_ڍير_اونهائي_TYPE

/* Defaults to StackType_t for backward compatibility, but can be overridden
 * in FreeRTOSConfig.h if StackType_t is too restrictive. */
    #وصف_ڪريو config_ڍير_اونهائي_TYPE    ڍير_Type_t
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل config_لاڳيتو_وقت_COUNTER_TYPE

/* Defaults to uint32_t for backward compatibility, but can be overridden in
 * FreeRTOSConfig.h if uint32_t is too restrictive. */

    #وصف_ڪريو config_لاڳيتو_وقت_COUNTER_TYPE    uint32_t
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل config_پيغام_بفر_ڊيگهائي_TYPE

/* Defaults to size_t for backward compatibility, but can be overridden
 * in FreeRTOSConfig.h if lengths will always be less than the number of bytes
 * in a size_t. */
    #وصف_ڪريو config_پيغام_بفر_ڊيگهائي_TYPE    size_t
#جيڪڏهن_پورو

/* Sanity check the configuration. */
#إذا ( ( config_SUPPORT_STATIC_ورهاست == 0 ) && ( config_SUPPORT_DYNAMIC_ورهاست == 0 ) )
    #غلطي config_SUPPORT_STATIC_ورهاست and config_SUPPORT_DYNAMIC_ورهاست cannot both be 0, but can both be 1.
#جيڪڏهن_پورو

#إذا ( ( configUSE_RECURSIVE_MUTEXES == 1 ) && ( configUSE_MUTEXES != 1 ) )
    #غلطي configUSE_MUTEXES must be سيٽ منزل 1 منزل use recursive mutexes
#جيڪڏهن_پورو

#إذا ( ( config_لاڳيتو_گڻو_PRIORITIES == 0 ) && ( configUSE_TASK_PREEMPTION_DISABLE != 0 ) )
    #غلطي config_لاڳيتو_گڻو_PRIORITIES must be سيٽ منزل 1 منزل use task preemption disable
#جيڪڏهن_پورو

#إذا ( ( configUSE_PREEMPTION == 0 ) && ( configUSE_TASK_PREEMPTION_DISABLE != 0 ) )
    #غلطي configUSE_PREEMPTION must be سيٽ منزل 1 منزل use task preemption disable
#جيڪڏهن_پورو

#إذا ( ( config_عدد_جو_CORES == 1 ) && ( configUSE_TASK_PREEMPTION_DISABLE != 0 ) )
    #غلطي configUSE_TASK_PREEMPTION_DISABLE is not supported اندريون single core FreeRTOS
#جيڪڏهن_پورو

#إذا ( ( config_عدد_جو_CORES == 1 ) && ( configUSE_CORE_AFFINITY != 0 ) )
    #غلطي configUSE_CORE_AFFINITY is not supported اندريون single core FreeRTOS
#جيڪڏهن_پورو

#إذا ( ( config_عدد_جو_CORES > 1 ) && ( config_USE_پورٽ_OPTIMISED_TASK_SELECTION != 0 ) )
    #غلطي config_USE_پورٽ_OPTIMISED_TASK_SELECTION is not supported اندريون SMP FreeRTOS
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل config_INITIAL_TICK_ڳڻپ
    #وصف_ڪريو config_INITIAL_TICK_ڳڻپ    0
#جيڪڏهن_پورو

#إذا ( پورٽ_TICK_TYPE_IS_ATOMIC == 0 )

/* Either variables of tick type cannot be read atomically, or
 * portTICK_TYPE_IS_ATOMIC was not set - map the critical sections used when
 * the tick count is returned to the standard critical section macros. */
    #وصف_ڪريو پورٽ_TICK_TYPE_ENTER_CRITICAL()                      پورٽ_ENTER_CRITICAL()
    #وصف_ڪريو پورٽ_TICK_TYPE_EXIT_CRITICAL()                       پورٽ_EXIT_CRITICAL()
    #وصف_ڪريو پورٽ_TICK_TYPE_سيٽ_مداخلت_نقاب_FROM_ISR()         پورٽ_سيٽ_مداخلت_نقاب_FROM_ISR()
    #وصف_ڪريو پورٽ_TICK_TYPE_CLEAR_مداخلت_نقاب_FROM_ISR( x )    پورٽ_CLEAR_مداخلت_نقاب_FROM_ISR( ( x ) )
#وإلا

/* The tick type can be read atomically, so critical sections used when the
 * tick count is returned can be defined away. */
    #وصف_ڪريو پورٽ_TICK_TYPE_ENTER_CRITICAL()
    #وصف_ڪريو پورٽ_TICK_TYPE_EXIT_CRITICAL()
    #وصف_ڪريو پورٽ_TICK_TYPE_سيٽ_مداخلت_نقاب_FROM_ISR()         0
    #وصف_ڪريو پورٽ_TICK_TYPE_CLEAR_مداخلت_نقاب_FROM_ISR( x )    ( فراغ ) ( x )
#جيڪڏهن_پورو /* if ( portTICK_TYPE_IS_ATOMIC == 0 ) */

/* Definitions to allow backward compatibility with FreeRTOS versions prior to
 * V8 if desired. */
#جيڪڏهن_وصف_نه_ٿيل configENABLE_BACKWARD_COMPATIBILITY
    #وصف_ڪريو configENABLE_BACKWARD_COMPATIBILITY    1
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل configPRINTF

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
    #وصف_ڪريو configPRINTF( X )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل configMAX

/* The application writer has not provided their own MAX macro, so define
 * the following generic implementation. */
    #وصف_ڪريو configMAX( a, b )    ( ( ( a ) > ( b ) ) ? ( a ) : ( b ) )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل configMIN

/* The application writer has not provided their own MIN macro, so define
 * the following generic implementation. */
    #وصف_ڪريو configMIN( a, b )    ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )
#جيڪڏهن_پورو

#إذا configENABLE_BACKWARD_COMPATIBILITY == 1
    #وصف_ڪريو eTaskStateGet                 eTaskGetState
    #وصف_ڪريو پورٽ_Tick_Type                  TickType_t
    #وصف_ڪريو xTaskHandle                   TaskHandle_t
    #وصف_ڪريو x_قطار_Handle                  قطار_Handle_t
    #وصف_ڪريو xSemaphoreHandle              SemaphoreHandle_t
    #وصف_ڪريو x_قطار_سيٽ_Handle               قطار_سيٽ_Handle_t
    #وصف_ڪريو x_قطار_سيٽ_Member_Handle         قطار_سيٽ_Member_Handle_t
    #وصف_ڪريو x_وقت_ٻاهريون_Type                  وقت_ٻاهريون_t
    #وصف_ڪريو x_يادگيري_Region                 يادگيري_Region_t
    #وصف_ڪريو xTaskParameters               TaskParameters_t
    #وصف_ڪريو x_Task_حالت_Type               Task_حالت_t
    #وصف_ڪريو xTimerHandle                  TimerHandle_t
    #وصف_ڪريو xCoRoutineHandle              CoRoutineHandle_t
    #وصف_ڪريو pd_TASK_HOOK_ڪوڊ              TaskHookFunction_t
    #وصف_ڪريو پورٽ_TICK_شرح_MS              پورٽ_TICK_PERIOD_MS
    #وصف_ڪريو pcTaskGetTaskName             pcTaskGetName
    #وصف_ڪريو pcTimerGetTimerName           pcTimerGetName
    #وصف_ڪريو pc_قطار_Get_قطار_Name           pc_قطار_Get_Name
    #وصف_ڪريو vTaskGetTaskInfo              vTaskGetInfo
    #وصف_ڪريو x_Task_Get_Idle_لاڳيتو_وقت_Counter    ul_Task_Get_Idle_لاڳيتو_وقت_Counter

/* Backward compatibility within the scheduler code only - these definitions
 * are not really required but are included for completeness. */
    #وصف_ڪريو tmrTIMER_CALLBACK             TimerCallbackFunction_t
    #وصف_ڪريو pd_TASK_ڪوڊ                   TaskFunction_t
    #وصف_ڪريو x_List_شي                     List_شي_t
    #وصف_ڪريو xList                         List_t

/* For libraries that break the list data hiding, and access list structure
 * members directly (which is not supposed to be done). */
    #وصف_ڪريو pxContainer                   pvContainer
#جيڪڏهن_پورو /* configENABLE_BACKWARD_COMPATIBILITY */

#إذا ( configUSE_ALTERNATIVE_API != 0 )
    #غلطي The alternative API was deprecated some time ago, and was removed اندريون FreeRTOS V9.0 0
#جيڪڏهن_پورو

/* Set configUSE_TASK_FPU_SUPPORT to 0 to omit floating point support even
 * if floating point hardware is otherwise supported by the FreeRTOS port in use.
 * This constant is not supported by all FreeRTOS ports that include floating
 * point support. */
#جيڪڏهن_وصف_نه_ٿيل configUSE_TASK_FPU_SUPPORT
    #وصف_ڪريو configUSE_TASK_FPU_SUPPORT    1
#جيڪڏهن_پورو

/* Set configENABLE_MPU to 1 to enable MPU support and 0 to disable it. This is
 * currently used in ARMv8M ports. */
#جيڪڏهن_وصف_نه_ٿيل configENABLE_MPU
    #وصف_ڪريو configENABLE_MPU    0
#جيڪڏهن_پورو

/* Set configENABLE_FPU to 1 to enable FPU support and 0 to disable it. This is
 * currently used in ARMv8M ports. */
#جيڪڏهن_وصف_نه_ٿيل configENABLE_FPU
    #وصف_ڪريو configENABLE_FPU    1
#جيڪڏهن_پورو

/* Set configENABLE_MVE to 1 to enable MVE support and 0 to disable it. This is
 * currently used in ARMv8M ports. */
#جيڪڏهن_وصف_نه_ٿيل configENABLE_MVE
    #وصف_ڪريو configENABLE_MVE    0
#جيڪڏهن_پورو

/* Set configENABLE_TRUSTZONE to 1 enable TrustZone support and 0 to disable it.
 * This is currently used in ARMv8M ports. */
#جيڪڏهن_وصف_نه_ٿيل configENABLE_TRUSTZONE
    #وصف_ڪريو configENABLE_TRUSTZONE    1
#جيڪڏهن_پورو

/* Set configRUN_FREERTOS_SECURE_ONLY to 1 to run the FreeRTOS ARMv8M port on
 * the Secure Side only. */
#جيڪڏهن_وصف_نه_ٿيل config_لاڳيتو_FREERTOS_SECURE_ONLY
    #وصف_ڪريو config_لاڳيتو_FREERTOS_SECURE_ONLY    0
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل config_لاڳيتو_ADDITIONAL_TESTS
    #وصف_ڪريو config_لاڳيتو_ADDITIONAL_TESTS    0
#جيڪڏهن_پورو

/* The following config allows infinite loop control. For example, control the
 * infinite loop in idle task function when performing unit tests. */
#جيڪڏهن_وصف_نه_ٿيل configCONTROL_INFINITE_LOOP
    #وصف_ڪريو configCONTROL_INFINITE_LOOP()
#جيڪڏهن_پورو

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
#وصف_ڪريو tsk_STATIC_AND_DYNAMIC_ورهاست_POSSIBLE                                                                                     \
    ( ( ( پورٽ_USING_MPU_WRAPPERS == 0 ) && ( config_SUPPORT_DYNAMIC_ورهاست == 1 ) && ( config_SUPPORT_STATIC_ورهاست == 1 ) ) || \
      ( ( پورٽ_USING_MPU_WRAPPERS == 1 ) && ( config_SUPPORT_DYNAMIC_ورهاست == 1 ) ) )

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
بنية x_STATIC_LIST_شي
{
    #إذا ( config_USE_LIST_معلومات_INTEGRITY_CHECK_بائيٽون == 1 )
        TickType_t xDummy1;
    #جيڪڏهن_پورو
    TickType_t xDummy2;
    فراغ * pvDummy3[ 4 ];
    #إذا ( config_USE_LIST_معلومات_INTEGRITY_CHECK_بائيٽون == 1 )
        TickType_t xDummy4;
    #جيڪڏهن_پورو
};
تعريفنوع بنية x_STATIC_LIST_شي Static_List_شي_t;

#إذا ( config_USE_MINI_LIST_شي == 1 )
    /* See the comments above the struct xSTATIC_LIST_ITEM definition. */
    بنية x_STATIC_MINI_LIST_شي
    {
        #إذا ( config_USE_LIST_معلومات_INTEGRITY_CHECK_بائيٽون == 1 )
            TickType_t xDummy1;
        #جيڪڏهن_پورو
        TickType_t xDummy2;
        فراغ * pvDummy3[ 2 ];
    };
    تعريفنوع بنية x_STATIC_MINI_LIST_شي Static_Mini_List_شي_t;
#وإلا /* if ( configUSE_MINI_LIST_ITEM == 1 ) */
    تعريفنوع بنية x_STATIC_LIST_شي      Static_Mini_List_شي_t;
#جيڪڏهن_پورو /* if ( configUSE_MINI_LIST_ITEM == 1 ) */

/* See the comments above the struct xSTATIC_LIST_ITEM definition. */
تعريفنوع بنية xSTATIC_LIST
{
    #إذا ( config_USE_LIST_معلومات_INTEGRITY_CHECK_بائيٽون == 1 )
        TickType_t xDummy1;
    #جيڪڏهن_پورو
    UBaseType_t uxDummy2;
    فراغ * pvDummy3;
    Static_Mini_List_شي_t xDummy4;
    #إذا ( config_USE_LIST_معلومات_INTEGRITY_CHECK_بائيٽون == 1 )
        TickType_t xDummy5;
    #جيڪڏهن_پورو
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
    #إذا ( پورٽ_USING_MPU_WRAPPERS == 1 )
        xMPU_SETTINGS xDummy2;
    #جيڪڏهن_پورو
    #إذا ( configUSE_CORE_AFFINITY == 1 ) && ( config_عدد_جو_CORES > 1 )
        UBaseType_t uxDummy26;
    #جيڪڏهن_پورو
    Static_List_شي_t xDummy3[ 2 ];
    UBaseType_t uxDummy5;
    فراغ * pxDummy6;
    #إذا ( config_عدد_جو_CORES > 1 )
        بنياد_Type_t xDummy23;
        UBaseType_t uxDummy24;
    #جيڪڏهن_پورو
    uint8_t ucDummy7[ configMAX_TASK_NAME_LEN ];
    #إذا ( configUSE_TASK_PREEMPTION_DISABLE == 1 )
        بنياد_Type_t xDummy25;
    #جيڪڏهن_پورو
    #إذا ( ( پورٽ_ڍير_GROWTH > 0 ) || ( config_رڪارڊ_ڍير_مٿاهون_پتو == 1 ) )
        فراغ * pxDummy8;
    #جيڪڏهن_پورو
    #إذا ( پورٽ_CRITICAL_NESTING_اندريون_TCB == 1 )
        UBaseType_t uxDummy9;
    #جيڪڏهن_پورو
    #إذا ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy10[ 2 ];
    #جيڪڏهن_پورو
    #إذا ( configUSE_MUTEXES == 1 )
        UBaseType_t uxDummy12[ 2 ];
    #جيڪڏهن_پورو
    #إذا ( config_USE_APPLICATION_TASK_ٽيگ == 1 )
        فراغ * pxDummy14;
    #جيڪڏهن_پورو
    #إذا ( config_انگ_THREAD_LOCAL_STORAGE_اشارا > 0 )
        فراغ * pvDummy15[ config_انگ_THREAD_LOCAL_STORAGE_اشارا ];
    #جيڪڏهن_پورو
    #إذا ( config_GENERATE_لاڳيتو_وقت_STATS == 1 )
        config_لاڳيتو_وقت_COUNTER_TYPE ulDummy16;
    #جيڪڏهن_پورو
    #إذا ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 )
        config_TLS_بلاڪ_TYPE xDummy17;
    #جيڪڏهن_پورو
    #إذا ( configUSE_TASK_NOTIFICATIONS == 1 )
        uint32_t ulDummy18[ configTASK_NOTIFICATION_ARRAY_ENTRIES ];
        uint8_t ucDummy19[ configTASK_NOTIFICATION_ARRAY_ENTRIES ];
    #جيڪڏهن_پورو
    #إذا ( tsk_STATIC_AND_DYNAMIC_ورهاست_POSSIBLE != 0 )
        uint8_t uxDummy20;
    #جيڪڏهن_پورو

    #إذا ( INCLUDE_xTaskAbortDelay == 1 )
        uint8_t ucDummy21;
    #جيڪڏهن_پورو
    #إذا ( configUSE_POSIX_ERRNO == 1 )
        صحيح iDummy22;
    #جيڪڏهن_پورو
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
تعريفنوع بنية x_STATIC_قطار
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

    #إذا ( ( config_SUPPORT_STATIC_ورهاست == 1 ) && ( config_SUPPORT_DYNAMIC_ورهاست == 1 ) )
        uint8_t ucDummy6;
    #جيڪڏهن_پورو

    #إذا ( config_USE_قطار_SETS == 1 )
        فراغ * pvDummy7;
    #جيڪڏهن_پورو

    #إذا ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy8;
        uint8_t ucDummy9;
    #جيڪڏهن_پورو
} Static_قطار_t;
تعريفنوع Static_قطار_t StaticSemaphore_t;

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
تعريفنوع بنية x_STATIC_EVENT_گروهه
{
    TickType_t xDummy1;
    StaticList_t xDummy2;

    #إذا ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy3;
    #جيڪڏهن_پورو

    #إذا ( ( config_SUPPORT_STATIC_ورهاست == 1 ) && ( config_SUPPORT_DYNAMIC_ورهاست == 1 ) )
        uint8_t ucDummy4;
    #جيڪڏهن_پورو
} Static_Event_گروهه_t;

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
    Static_List_شي_t xDummy2;
    TickType_t xDummy3;
    فراغ * pvDummy5;
    TaskFunction_t pvDummy6;
    #إذا ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy7;
    #جيڪڏهن_پورو
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
تعريفنوع بنية x_STATIC_وهڪرو_بفر
{
    size_t uxDummy1[ 4 ];
    فراغ * pvDummy2[ 3 ];
    uint8_t ucDummy3;
    #إذا ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy4;
    #جيڪڏهن_پورو
    #إذا ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        فراغ * pvDummy5[ 2 ];
    #جيڪڏهن_پورو
    UBaseType_t uxDummy6;
} Static_وهڪرو_بفر_t;

/* Message buffers are built on stream buffers. */
تعريفنوع Static_وهڪرو_بفر_t Static_پيغام_بفر_t;

/* *INDENT-OFF* */
#جيڪڏهن_وصف_ٿيل __cplusplus
    }
#جيڪڏهن_پورو
/* *INDENT-ON* */

/*-----------------------------------------------------------
* IDF Compatibility
*----------------------------------------------------------*/

#جيڪڏهن_وصف_ٿيل ESP_PLATFORM

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

#جيڪڏهن_پورو /* ESP_PLATFORM */

#جيڪڏهن_پورو /* INC_FREERTOS_H */
