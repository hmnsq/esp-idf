/*
 * FreeRTOS Kernel V10.5.1 (ESP-IDF SMP modified)
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-FileCopyrightText: 2021 Amazon.com, Inc. or its affiliates
 *
 * SPDX-License-Identifier: MIT
 *
 * SPDX-FileContributor: 2023 Espressif Systems (Shanghai) CO LTD
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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ INC_FREERTOS_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INC_FREERTOS_H

/*
 * Include the generic headers required for the FreeRTOS port being used.
 */
#ਅਨ੍ਤਰ੍ਭਾਵ <stddef.h>

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
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h> /* READ COMMENT ABOVE. */

/* *INDENT-OFF* */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
    ਬਾਹਰੀ "C" {
#ਜੇ_ਅੰਤ
/* *INDENT-ON* */

/* Application specific configuration options. */
#ਅਨ੍ਤਰ੍ਭਾਵ "FreeRTOSConfig.h"

/* Basic FreeRTOS definitions. */
#ਅਨ੍ਤਰ੍ਭਾਵ "projdefs.h"

/* Definitions specific to the port being used. */
#ਅਨ੍ਤਰ੍ਭਾਵ "portable.h"

/* Must be defaulted before configUSE_NEWLIB_REENTRANT is used below. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configUSE_NEWLIB_REENTRANT
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_NEWLIB_REENTRANT    0
#ਜੇ_ਅੰਤ

/* Required if struct _reent is used. */
#ਜੇ ( configUSE_NEWLIB_REENTRANT == 1 )

/* Note Newlib support has been included by popular demand, but is not
 * used by the FreeRTOS maintainers themselves.  FreeRTOS is not
 * responsible for resulting newlib operation.  User must be familiar with
 * newlib and must provide system-wide implementations of the necessary
 * stubs. Be warned that (at the time of writing) the current newlib design
 * implements a system-wide malloc() that must be provided with locks.
 *
 * See the third party link http://www.nadler.com/embedded/newlibAndFreeRTOS.html
 * for additional information. */
    #ਅਨ੍ਤਰ੍ਭਾਵ <reent.h>

    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_C_RUNTIME_TLS_SUPPORT    1

    #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_TLS_ਖੰਡ_TYPE
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_TLS_ਖੰਡ_TYPE           ਬਣਤਰ _reent
    #ਜੇ_ਅੰਤ

    #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_INIT_TLS_ਖੰਡ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_INIT_TLS_ਖੰਡ( xTLSBlock )    _REENT_INIT_PTR( &( xTLSBlock ) )
    #ਜੇ_ਅੰਤ

    #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_ਸੈੱਟ_TLS_ਖੰਡ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_ਸੈੱਟ_TLS_ਖੰਡ( xTLSBlock )    _impure_ptr = &( xTLSBlock )
    #ਜੇ_ਅੰਤ

    #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_DEINIT_TLS_ਖੰਡ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_DEINIT_TLS_ਖੰਡ( xTLSBlock )    _reclaim_reent( &( xTLSBlock ) )
    #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ /* if ( configUSE_NEWLIB_REENTRANT == 1 ) */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configUSE_C_RUNTIME_TLS_SUPPORT
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_C_RUNTIME_TLS_SUPPORT    0
#ਜੇ_ਅੰਤ

#ਜੇ ( ( configUSE_NEWLIB_REENTRANT == 0 ) && ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 ) )

    #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_TLS_ਖੰਡ_TYPE
        #ਗਲਤੀ Missing definition:  config_TLS_ਖੰਡ_TYPE must be defined ਵਿੱਚ FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is ਸੈੱਟ ਮੰਜ਼ਿਲ 1.
    #ਜੇ_ਅੰਤ

    #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_INIT_TLS_ਖੰਡ
        #ਗਲਤੀ Missing definition:  config_INIT_TLS_ਖੰਡ must be defined ਵਿੱਚ FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is ਸੈੱਟ ਮੰਜ਼ਿਲ 1.
    #ਜੇ_ਅੰਤ

    #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_ਸੈੱਟ_TLS_ਖੰਡ
        #ਗਲਤੀ Missing definition:  config_ਸੈੱਟ_TLS_ਖੰਡ must be defined ਵਿੱਚ FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is ਸੈੱਟ ਮੰਜ਼ਿਲ 1.
    #ਜੇ_ਅੰਤ

    #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_DEINIT_TLS_ਖੰਡ
        #ਗਲਤੀ Missing definition:  config_DEINIT_TLS_ਖੰਡ must be defined ਵਿੱਚ FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is ਸੈੱਟ ਮੰਜ਼ਿਲ 1.
    #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ /* if ( ( configUSE_NEWLIB_REENTRANT == 0 ) && ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 ) ) */

/*
 * Check all the required application specific macros have been defined.
 * These macros are application specific and (as downloaded) are defined
 * within FreeRTOSConfig.h.
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_ਸੰਖਿਆ_ਦਾ_CORES
    #ਗਲਤੀ Missing definition:  config_ਸੰਖਿਆ_ਦਾ_CORES must be defined ਵਿੱਚ FreeRTOSConfig.h
#ਜੇ_ਅੰਤ

#ਜੇ ( config_ਸੰਖਿਆ_ਦਾ_CORES > 1 )
    #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_GET_CORE_ID
        #ਗਲਤੀ "Missing definition: portGET_CORE_ID() must be defined if in portmacro.h if configNUMBER_OF_CORES > 1"
    #ਜੇ_ਅੰਤ
    #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_YIELD_CORE
        #ਗਲਤੀ "Missing definition: portYIELD_CORE() must be defined if in portmacro.h if configNUMBER_OF_CORES > 1"
    #ਜੇ_ਅੰਤ
#ਜਾਂਜੇ ( config_ਸੰਖਿਆ_ਦਾ_CORES == 1 )
    #ਪਰਿਭਾਸ਼ਾ_ਰੱਦ ਪੋਰਟ_GET_CORE_ID
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_GET_CORE_ID()    0
#ਨਹੀਂਤਾਂ
    #ਗਲਤੀ config_ਸੰਖਿਆ_ਦਾ_CORES must be defined ਮੰਜ਼ਿਲ either 1 or > 1.
#ਜੇ_ਅੰਤ /* if ( configNUMBER_OF_CORES > 1 ) */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_ਨਿਊਨਤਮ_ਢੇਰ_ਮਾਪ
    #ਗਲਤੀ Missing definition:  config_ਨਿਊਨਤਮ_ਢੇਰ_ਮਾਪ must be defined ਵਿੱਚ FreeRTOSConfig.h.  config_ਨਿਊਨਤਮ_ਢੇਰ_ਮਾਪ ਨਿਰਧਾਰਤ_ਕਰਦਾ the ਮਾਪ (ਵਿੱਚ ਸ਼ਬਦ) ਦਾ the ਢੇਰ allocated ਮੰਜ਼ਿਲ the idle task.  Refer ਮੰਜ਼ਿਲ the demo project provided ਲਈ your ਪੋਰਟ ਲਈ a suitable ਮੁੱਲ.
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configMAX_PRIORITIES
    #ਗਲਤੀ Missing definition:  configMAX_PRIORITIES must be defined ਵਿੱਚ FreeRTOSConfig.h.  See the Configuration section ਦਾ the FreeRTOS API documentation ਲਈ details.
#ਜੇ_ਅੰਤ

#ਜੇ configMAX_PRIORITIES < 1
    #ਗਲਤੀ configMAX_PRIORITIES must be defined ਮੰਜ਼ਿਲ be greater than or equal ਮੰਜ਼ਿਲ 1.
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configUSE_PREEMPTION
    #ਗਲਤੀ Missing definition:  configUSE_PREEMPTION must be defined ਵਿੱਚ FreeRTOSConfig.h as either 1 or 0.  See the Configuration section ਦਾ the FreeRTOS API documentation ਲਈ details.
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configUSE_IDLE_HOOK
    #ਗਲਤੀ Missing definition:  configUSE_IDLE_HOOK must be defined ਵਿੱਚ FreeRTOSConfig.h as either 1 or 0.  See the Configuration section ਦਾ the FreeRTOS API documentation ਲਈ details.
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configUSE_TICK_HOOK
    #ਗਲਤੀ Missing definition:  configUSE_TICK_HOOK must be defined ਵਿੱਚ FreeRTOSConfig.h as either 1 or 0.  See the Configuration section ਦਾ the FreeRTOS API documentation ਲਈ details.
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_USE_16_ਦੋਆਧਾਰੀ_ਅੰਕ_TICKS
    #ਗਲਤੀ Missing definition:  config_USE_16_ਦੋਆਧਾਰੀ_ਅੰਕ_TICKS must be defined ਵਿੱਚ FreeRTOSConfig.h as either 1 or 0.  See the Configuration section ਦਾ the FreeRTOS API documentation ਲਈ details.
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configUSE_CO_ROUTINES
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_CO_ROUTINES    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ INCLUDE_v_Task_ਤਰਜੀਹ_ਸੈੱਟ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_v_Task_ਤਰਜੀਹ_ਸੈੱਟ    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ INCLUDE_ux_Task_ਤਰਜੀਹ_Get
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_ux_Task_ਤਰਜੀਹ_Get    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ INCLUDE_vTaskDelete
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_vTaskDelete    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ INCLUDE_vTaskSuspend
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_vTaskSuspend    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ INCLUDE_xTaskDelayUntil
    #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ INCLUDE_vTaskDelayUntil

/* INCLUDE_vTaskDelayUntil was replaced by INCLUDE_xTaskDelayUntil.  Backward
 * compatibility is maintained if only one or the other is defined, but
 * there is a conflict if both are defined. */
        #ਗਲਤੀ INCLUDE_vTaskDelayUntil and INCLUDE_xTaskDelayUntil are both defined.  INCLUDE_vTaskDelayUntil is no longer required and should be removed
    #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ INCLUDE_xTaskDelayUntil
    #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ INCLUDE_vTaskDelayUntil

/* If INCLUDE_vTaskDelayUntil is set but INCLUDE_xTaskDelayUntil is not then
 * the project's FreeRTOSConfig.h probably pre-dates the introduction of
 * xTaskDelayUntil and setting INCLUDE_xTaskDelayUntil to whatever
 * INCLUDE_vTaskDelayUntil is set to will ensure backward compatibility.
 */
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_xTaskDelayUntil    INCLUDE_vTaskDelayUntil
    #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ INCLUDE_xTaskDelayUntil
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_xTaskDelayUntil    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ INCLUDE_vTaskDelay
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_vTaskDelay    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ INCLUDE_xTaskGetIdleTaskHandle
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_xTaskGetIdleTaskHandle    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ INCLUDE_xTaskAbortDelay
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_xTaskAbortDelay    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ INCLUDE_x_ਕਤਾਰ_Get_Mutex_Holder
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_x_ਕਤਾਰ_Get_Mutex_Holder    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ INCLUDE_xSemaphoreGetMutexHolder
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_xSemaphoreGetMutexHolder    INCLUDE_x_ਕਤਾਰ_Get_Mutex_Holder
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ INCLUDE_xTaskGetHandle
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_xTaskGetHandle    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ INCLUDE_ux_Task_Get_ਢੇਰ_ਉੱਚਾ_Water_Mark
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_ux_Task_Get_ਢੇਰ_ਉੱਚਾ_Water_Mark    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ INCLUDE_ux_Task_Get_ਢੇਰ_ਉੱਚਾ_Water_Mark2
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_ux_Task_Get_ਢੇਰ_ਉੱਚਾ_Water_Mark2    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ INCLUDE_eTaskGetState
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_eTaskGetState    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ INCLUDE_xTaskResumeFromISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_xTaskResumeFromISR    1
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ INCLUDE_xTimerPendFunctionCall
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_xTimerPendFunctionCall    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ INCLUDE_xTaskGetSchedulerState
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_xTaskGetSchedulerState    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ INCLUDE_xTaskGetCurrentTaskHandle
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_xTaskGetCurrentTaskHandle    1
#ਜੇ_ਅੰਤ

#ਜੇ configUSE_CO_ROUTINES != 0
    #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configMAX_CO_ROUTINE_PRIORITIES
        #ਗਲਤੀ configMAX_CO_ROUTINE_PRIORITIES must be greater than or equal ਮੰਜ਼ਿਲ 1.
    #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configUSE_DAEMON_TASK_STARTUP_HOOK
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_DAEMON_TASK_STARTUP_HOOK    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_USE_APPLICATION_TASK_ਟੈਗ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_USE_APPLICATION_TASK_ਟੈਗ    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_ਅੰਕ_THREAD_LOCAL_STORAGE_ਸੂਚਕ_ਸਾਰੇ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_ਅੰਕ_THREAD_LOCAL_STORAGE_ਸੂਚਕ_ਸਾਰੇ    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configUSE_RECURSIVE_MUTEXES
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_RECURSIVE_MUTEXES    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configUSE_MUTEXES
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_MUTEXES    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configUSE_TIMERS
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_TIMERS    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configUSE_COUNTING_SEMAPHORES
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_COUNTING_SEMAPHORES    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configUSE_ALTERNATIVE_API
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_ALTERNATIVE_API    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_CRITICAL_NESTING_ਵਿੱਚ_TCB
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_CRITICAL_NESTING_ਵਿੱਚ_TCB    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configMAX_TASK_NAME_LEN
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configMAX_TASK_NAME_LEN    16
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configIDLE_SHOULD_YIELD
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configIDLE_SHOULD_YIELD    1
#ਜੇ_ਅੰਤ

#ਜੇ configMAX_TASK_NAME_LEN < 1
    #ਗਲਤੀ configMAX_TASK_NAME_LEN must be ਸੈੱਟ ਮੰਜ਼ਿਲ a minimum ਦਾ 1 ਵਿੱਚ FreeRTOSConfig.h
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configASSERT
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configASSERT( x )
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configASSERT_DEFINED    0
#ਨਹੀਂਤਾਂ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configASSERT_DEFINED    1
#ਜੇ_ਅੰਤ

/* configPRECONDITION should be defined as configASSERT.
 * The CBMC proofs need a way to track assumptions and assertions.
 * A configPRECONDITION statement should express an implicit invariant or
 * assumption made.  A configASSERT statement should express an invariant that must
 * hold explicit before calling the code. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configPRECONDITION
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configPRECONDITION( X )    configASSERT( X )
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configPRECONDITION_DEFINED    0
#ਨਹੀਂਤਾਂ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configPRECONDITION_DEFINED    1
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_ਸਮ੍ਰਿਤੀ_BARRIER
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਸਮ੍ਰਿਤੀ_BARRIER()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_SOFTWARE_BARRIER
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_SOFTWARE_BARRIER()
#ਜੇ_ਅੰਤ

/* The timers module relies on xTaskGetSchedulerState(). */
#ਜੇ configUSE_TIMERS == 1

    #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_TIMER_TASK_ਤਰਜੀਹ
        #ਗਲਤੀ If configUSE_TIMERS is ਸੈੱਟ ਮੰਜ਼ਿਲ 1 then config_TIMER_TASK_ਤਰਜੀਹ must also be defined.
    #ਜੇ_ਅੰਤ /* configTIMER_TASK_PRIORITY */

    #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_TIMER_ਕਤਾਰ_ਲੰਮਾਈ
        #ਗਲਤੀ If configUSE_TIMERS is ਸੈੱਟ ਮੰਜ਼ਿਲ 1 then config_TIMER_ਕਤਾਰ_ਲੰਮਾਈ must also be defined.
    #ਜੇ_ਅੰਤ /* configTIMER_QUEUE_LENGTH */

    #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_TIMER_TASK_ਢੇਰ_ਡੂੰਘਾਈ
        #ਗਲਤੀ If configUSE_TIMERS is ਸੈੱਟ ਮੰਜ਼ਿਲ 1 then config_TIMER_TASK_ਢੇਰ_ਡੂੰਘਾਈ must also be defined.
    #ਜੇ_ਅੰਤ /* configTIMER_TASK_STACK_DEPTH */

#ਜੇ_ਅੰਤ /* configUSE_TIMERS */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR()    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_CLEAR_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_CLEAR_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR( ux_Saved_ਸਥਿਤੀ_ਮੁੱਲ )    ( ਖਾਲੀ ) ( ux_Saved_ਸਥਿਤੀ_ਮੁੱਲ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_CLEAN_ਉੱਪਰ_TCB
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_CLEAN_ਉੱਪਰ_TCB( pxTCB )    ( ਖਾਲੀ ) ( pxTCB )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_PRE_TASK_DELETE_HOOK
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_PRE_TASK_DELETE_HOOK( pv_Task_ਮੰਜ਼ਿਲ_Delete, pxYieldPending )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_ਤਿਆਰੀ_TCB
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਤਿਆਰੀ_TCB( pxTCB )    ( ਖਾਲੀ ) ( pxTCB )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_ਕਤਾਰ_REGISTRY_ਮਾਪ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_ਕਤਾਰ_REGISTRY_ਮਾਪ    0U
#ਜੇ_ਅੰਤ

#ਜੇ ( config_ਕਤਾਰ_REGISTRY_ਮਾਪ < 1 )
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_ਕਤਾਰ_ਜੋੜੋ_ਮੰਜ਼ਿਲ_Registry( x_ਕਤਾਰ, pcName )
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_ਕਤਾਰ_Unregister_ਕਤਾਰ( x_ਕਤਾਰ )
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pc_ਕਤਾਰ_Get_Name( x_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_USE_MINI_LIST_ਵਸਤੂ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_USE_MINI_LIST_ਵਸਤੂ    1
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_ਸੂਚਕ_ਮਾਪ_TYPE
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਸੂਚਕ_ਮਾਪ_TYPE    uint32_t
#ਜੇ_ਅੰਤ

/* Remove any unused trace macros. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਸ਼ੁਰੂ

/* Used to perform any necessary initialisation - for example, open a file
 * into which trace is to be written. */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਸ਼ੁਰੂ()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਸਿਰਾ

/* Use to close a trace, for example close a file into which trace has been
 * written. */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਸਿਰਾ()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_TASK_SWITCHED_ਵਿੱਚ

/* Called after a task has been selected to run.  pxCurrentTCB holds a pointer
 * to the task control block of the selected task. */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_TASK_SWITCHED_ਵਿੱਚ()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_INCREASE_TICK_ਗਿਣਤੀ

/* Called before stepping the tick count after waking from tickless idle
 * sleep. */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_INCREASE_TICK_ਗਿਣਤੀ( x )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਨੀਵਾਂ_POWER_IDLE_BEGIN
    /* Called immediately before entering tickless idle. */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਨੀਵਾਂ_POWER_IDLE_BEGIN()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਨੀਵਾਂ_POWER_IDLE_ਸਿਰਾ
    /* Called when returning to the Idle task after a tickless idle. */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਨੀਵਾਂ_POWER_IDLE_ਸਿਰਾ()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_TASK_SWITCHED_ਬਾਹਰਲਾ

/* Called before a task has been selected to run.  pxCurrentTCB holds a pointer
 * to the task control block of the task being switched out. */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_TASK_SWITCHED_ਬਾਹਰਲਾ()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_TASK_ਤਰਜੀਹ_INHERIT

/* Called when a task attempts to take a mutex that is already held by a
 * lower priority task.  pxTCBOfMutexHolder is a pointer to the TCB of the task
 * that holds the mutex.  uxInheritedPriority is the priority the mutex holder
 * will inherit (the priority of the task that is attempting to obtain the
 * muted. */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_TASK_ਤਰਜੀਹ_INHERIT( pxTCBOfMutexHolder, ux_Inherited_ਤਰਜੀਹ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_TASK_ਤਰਜੀਹ_DISINHERIT

/* Called when a task releases a mutex, the holding of which had resulted in
 * the task inheriting the priority of a higher priority task.
 * pxTCBOfMutexHolder is a pointer to the TCB of the task that is releasing the
 * mutex.  uxOriginalPriority is the task's configured (base) priority. */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_TASK_ਤਰਜੀਹ_DISINHERIT( pxTCBOfMutexHolder, ux_Original_ਤਰਜੀਹ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_BLOCKING_ON_ਕਤਾਰ_RECEIVE

/* Task is about to block because it cannot read from a
 * queue/mutex/semaphore.  pxQueue is a pointer to the queue/mutex/semaphore
 * upon which the read was attempted.  pxCurrentTCB points to the TCB of the
 * task that attempted the read. */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_BLOCKING_ON_ਕਤਾਰ_RECEIVE( px_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_BLOCKING_ON_ਕਤਾਰ_PEEK

/* Task is about to block because it cannot read from a
 * queue/mutex/semaphore.  pxQueue is a pointer to the queue/mutex/semaphore
 * upon which the read was attempted.  pxCurrentTCB points to the TCB of the
 * task that attempted the read. */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_BLOCKING_ON_ਕਤਾਰ_PEEK( px_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_BLOCKING_ON_ਕਤਾਰ_SEND

/* Task is about to block because it cannot write to a
 * queue/mutex/semaphore.  pxQueue is a pointer to the queue/mutex/semaphore
 * upon which the write was attempted.  pxCurrentTCB points to the TCB of the
 * task that attempted the write. */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_BLOCKING_ON_ਕਤਾਰ_SEND( px_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_CHECK_FOR_ਢੇਰ_ਉਛਾਲ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_CHECK_FOR_ਢੇਰ_ਉਛਾਲ    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_ਰਿਕਾਰਡ_ਢੇਰ_ਉੱਚਾ_ਪਤਾ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_ਰਿਕਾਰਡ_ਢੇਰ_ਉੱਚਾ_ਪਤਾ    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H    0
#ਜੇ_ਅੰਤ

/* The following event macros are embedded in the kernel API calls. */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_MOVED_TASK_ਮੰਜ਼ਿਲ_READY_STATE
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_MOVED_TASK_ਮੰਜ਼ਿਲ_READY_STATE( pxTCB )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_POST_MOVED_TASK_ਮੰਜ਼ਿਲ_READY_STATE
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_POST_MOVED_TASK_ਮੰਜ਼ਿਲ_READY_STATE( pxTCB )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਕਤਾਰ_CREATE
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਕਤਾਰ_CREATE( px_New_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਕਤਾਰ_CREATE_FAILED
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਕਤਾਰ_CREATE_FAILED( uc_ਕਤਾਰ_Type )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceCREATE_MUTEX
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceCREATE_MUTEX( px_New_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceCREATE_MUTEX_FAILED
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceCREATE_MUTEX_FAILED()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceGIVE_MUTEX_RECURSIVE
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceGIVE_MUTEX_RECURSIVE( pxMutex )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceGIVE_MUTEX_RECURSIVE_FAILED
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceGIVE_MUTEX_RECURSIVE_FAILED( pxMutex )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceTAKE_MUTEX_RECURSIVE
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceTAKE_MUTEX_RECURSIVE( pxMutex )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceTAKE_MUTEX_RECURSIVE_FAILED
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceTAKE_MUTEX_RECURSIVE_FAILED( pxMutex )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceCREATE_COUNTING_SEMAPHORE
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceCREATE_COUNTING_SEMAPHORE()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceCREATE_COUNTING_SEMAPHORE_FAILED
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceCREATE_COUNTING_SEMAPHORE_FAILED()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਕਤਾਰ_ਸੈੱਟ_SEND
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਕਤਾਰ_ਸੈੱਟ_SEND    trace_ਕਤਾਰ_SEND
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਕਤਾਰ_SEND
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਕਤਾਰ_SEND( px_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਕਤਾਰ_SEND_FAILED
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਕਤਾਰ_SEND_FAILED( px_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਕਤਾਰ_RECEIVE
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਕਤਾਰ_RECEIVE( px_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਕਤਾਰ_PEEK
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਕਤਾਰ_PEEK( px_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਕਤਾਰ_PEEK_FAILED
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਕਤਾਰ_PEEK_FAILED( px_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਕਤਾਰ_PEEK_FROM_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਕਤਾਰ_PEEK_FROM_ISR( px_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਕਤਾਰ_RECEIVE_FAILED
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਕਤਾਰ_RECEIVE_FAILED( px_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਕਤਾਰ_SEND_FROM_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਕਤਾਰ_SEND_FROM_ISR( px_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਕਤਾਰ_SEND_FROM_ISR_FAILED
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਕਤਾਰ_SEND_FROM_ISR_FAILED( px_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਕਤਾਰ_RECEIVE_FROM_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਕਤਾਰ_RECEIVE_FROM_ISR( px_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਕਤਾਰ_RECEIVE_FROM_ISR_FAILED
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਕਤਾਰ_RECEIVE_FROM_ISR_FAILED( px_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਕਤਾਰ_PEEK_FROM_ISR_FAILED
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਕਤਾਰ_PEEK_FROM_ISR_FAILED( px_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਕਤਾਰ_DELETE
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਕਤਾਰ_DELETE( px_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceTASK_CREATE
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceTASK_CREATE( pxNewTCB )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceTASK_CREATE_FAILED
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceTASK_CREATE_FAILED()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceTASK_DELETE
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceTASK_DELETE( px_Task_ਮੰਜ਼ਿਲ_Delete )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceTASK_DELAY_UNTIL
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceTASK_DELAY_UNTIL( x )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceTASK_DELAY
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceTASK_DELAY()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_TASK_ਤਰਜੀਹ_ਸੈੱਟ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_TASK_ਤਰਜੀਹ_ਸੈੱਟ( pxTask, ux_New_ਤਰਜੀਹ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceTASK_SUSPEND
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceTASK_SUSPEND( px_Task_ਮੰਜ਼ਿਲ_Suspend )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceTASK_RESUME
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceTASK_RESUME( px_Task_ਮੰਜ਼ਿਲ_Resume )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceTASK_RESUME_FROM_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceTASK_RESUME_FROM_ISR( px_Task_ਮੰਜ਼ਿਲ_Resume )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceTASK_INCREMENT_TICK
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceTASK_INCREMENT_TICK( x_Tick_ਗਿਣਤੀ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceTIMER_CREATE
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceTIMER_CREATE( pxNewTimer )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceTIMER_CREATE_FAILED
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceTIMER_CREATE_FAILED()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceTIMER_COMMAND_SEND
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceTIMER_COMMAND_SEND( xTimer, x_ਸੁਨੇਹਾ_ID, x_ਸੁਨੇਹਾ_ਮੁੱਲ_ਮੁੱਲ, xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceTIMER_EXPIRED
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceTIMER_EXPIRED( pxTimer )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceTIMER_COMMAND_RECEIVED
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceTIMER_COMMAND_RECEIVED( pxTimer, x_ਸੁਨੇਹਾ_ID, x_ਸੁਨੇਹਾ_ਮੁੱਲ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceMALLOC
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceMALLOC( pv_ਪਤਾ, ui_ਮਾਪ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceFREE
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceFREE( pv_ਪਤਾ, ui_ਮਾਪ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_EVENT_ਸਮੂਹ_CREATE
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_EVENT_ਸਮੂਹ_CREATE( x_Event_ਸਮੂਹ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_EVENT_ਸਮੂਹ_CREATE_FAILED
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_EVENT_ਸਮੂਹ_CREATE_FAILED()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_EVENT_ਸਮੂਹ_SYNC_ਖੰਡ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_EVENT_ਸਮੂਹ_SYNC_ਖੰਡ( x_Event_ਸਮੂਹ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_EVENT_ਸਮੂਹ_SYNC_ਸਿਰਾ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_EVENT_ਸਮੂਹ_SYNC_ਸਿਰਾ( x_Event_ਸਮੂਹ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For, xTimeoutOccurred )    ( ਖਾਲੀ ) ( xTimeoutOccurred )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_EVENT_ਸਮੂਹ_WAIT_ਬਿੱਟ_ਖੰਡ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_EVENT_ਸਮੂਹ_WAIT_ਬਿੱਟ_ਖੰਡ( x_Event_ਸਮੂਹ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_EVENT_ਸਮੂਹ_WAIT_ਬਿੱਟ_ਸਿਰਾ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_EVENT_ਸਮੂਹ_WAIT_ਬਿੱਟ_ਸਿਰਾ( x_Event_ਸਮੂਹ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For, xTimeoutOccurred )    ( ਖਾਲੀ ) ( xTimeoutOccurred )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_EVENT_ਸਮੂਹ_CLEAR_ਬਿੱਟ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_EVENT_ਸਮੂਹ_CLEAR_ਬਿੱਟ( x_Event_ਸਮੂਹ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_EVENT_ਸਮੂਹ_CLEAR_ਬਿੱਟ_FROM_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_EVENT_ਸਮੂਹ_CLEAR_ਬਿੱਟ_FROM_ISR( x_Event_ਸਮੂਹ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_EVENT_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_EVENT_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ( x_Event_ਸਮੂਹ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_EVENT_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ_FROM_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_EVENT_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ_FROM_ISR( x_Event_ਸਮੂਹ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_EVENT_ਸਮੂਹ_DELETE
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_EVENT_ਸਮੂਹ_DELETE( x_Event_ਸਮੂਹ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ tracePEND_FUNC_CALL
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tracePEND_FUNC_CALL( x_Function_ਮੰਜ਼ਿਲ_Pend, pvParameter1, ulParameter2, ret )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ tracePEND_FUNC_CALL_FROM_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tracePEND_FUNC_CALL_FROM_ISR( x_Function_ਮੰਜ਼ਿਲ_Pend, pvParameter1, ulParameter2, ret )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਕਤਾਰ_REGISTRY_ਜੋੜੋ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਕਤਾਰ_REGISTRY_ਜੋੜੋ( x_ਕਤਾਰ, pc_ਕਤਾਰ_Name )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_TASK_NOTIFY_TAKE_ਖੰਡ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_TASK_NOTIFY_TAKE_ਖੰਡ( ux_ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_Wait )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceTASK_NOTIFY_TAKE
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceTASK_NOTIFY_TAKE( ux_ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_Wait )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_TASK_NOTIFY_WAIT_ਖੰਡ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_TASK_NOTIFY_WAIT_ਖੰਡ( ux_ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_Wait )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceTASK_NOTIFY_WAIT
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceTASK_NOTIFY_WAIT( ux_ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_Wait )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceTASK_NOTIFY
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceTASK_NOTIFY( ux_ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_Notify )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceTASK_NOTIFY_FROM_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceTASK_NOTIFY_FROM_ISR( ux_ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_Notify )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceTASK_NOTIFY_GIVE_FROM_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceTASK_NOTIFY_GIVE_FROM_ISR( ux_ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_Notify )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਧਾਰਾ_ਬਫਰ_CREATE_FAILED
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਧਾਰਾ_ਬਫਰ_CREATE_FAILED( x_Is_ਸੁਨੇਹਾ_ਬਫਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਧਾਰਾ_ਬਫਰ_CREATE_STATIC_FAILED
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਧਾਰਾ_ਬਫਰ_CREATE_STATIC_FAILED( xReturn, x_Is_ਸੁਨੇਹਾ_ਬਫਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਧਾਰਾ_ਬਫਰ_CREATE
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਧਾਰਾ_ਬਫਰ_CREATE( px_ਧਾਰਾ_ਬਫਰ, x_Is_ਸੁਨੇਹਾ_ਬਫਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਧਾਰਾ_ਬਫਰ_DELETE
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਧਾਰਾ_ਬਫਰ_DELETE( x_ਧਾਰਾ_ਬਫਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਧਾਰਾ_ਬਫਰ_RESET
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਧਾਰਾ_ਬਫਰ_RESET( x_ਧਾਰਾ_ਬਫਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_BLOCKING_ON_ਧਾਰਾ_ਬਫਰ_SEND
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_BLOCKING_ON_ਧਾਰਾ_ਬਫਰ_SEND( x_ਧਾਰਾ_ਬਫਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਧਾਰਾ_ਬਫਰ_SEND
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਧਾਰਾ_ਬਫਰ_SEND( x_ਧਾਰਾ_ਬਫਰ, x_ਬਾਈਟਾਂ_ਭੇਜਿਆ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਧਾਰਾ_ਬਫਰ_SEND_FAILED
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਧਾਰਾ_ਬਫਰ_SEND_FAILED( x_ਧਾਰਾ_ਬਫਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਧਾਰਾ_ਬਫਰ_SEND_FROM_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਧਾਰਾ_ਬਫਰ_SEND_FROM_ISR( x_ਧਾਰਾ_ਬਫਰ, x_ਬਾਈਟਾਂ_ਭੇਜਿਆ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_BLOCKING_ON_ਧਾਰਾ_ਬਫਰ_RECEIVE
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_BLOCKING_ON_ਧਾਰਾ_ਬਫਰ_RECEIVE( x_ਧਾਰਾ_ਬਫਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਧਾਰਾ_ਬਫਰ_RECEIVE
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਧਾਰਾ_ਬਫਰ_RECEIVE( x_ਧਾਰਾ_ਬਫਰ, x_Received_ਲੰਮਾਈ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਧਾਰਾ_ਬਫਰ_RECEIVE_FAILED
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਧਾਰਾ_ਬਫਰ_RECEIVE_FAILED( x_ਧਾਰਾ_ਬਫਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਧਾਰਾ_ਬਫਰ_RECEIVE_FROM_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਧਾਰਾ_ਬਫਰ_RECEIVE_FROM_ISR( x_ਧਾਰਾ_ਬਫਰ, x_Received_ਲੰਮਾਈ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_GENERATE_ਲੜੀਵਾਰ_ਸਮਾਂ_STATS
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_GENERATE_ਲੜੀਵਾਰ_ਸਮਾਂ_STATS    0
#ਜੇ_ਅੰਤ

#ਜੇ ( config_GENERATE_ਲੜੀਵਾਰ_ਸਮਾਂ_STATS == 1 )

    #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_CONFIGURE_TIMER_FOR_ਲੜੀਵਾਰ_ਸਮਾਂ_STATS
        #ਗਲਤੀ If config_GENERATE_ਲੜੀਵਾਰ_ਸਮਾਂ_STATS is defined then ਪੋਰਟ_CONFIGURE_TIMER_FOR_ਲੜੀਵਾਰ_ਸਮਾਂ_STATS must also be defined.  ਪੋਰਟ_CONFIGURE_TIMER_FOR_ਲੜੀਵਾਰ_ਸਮਾਂ_STATS should call a ਪੋਰਟ layer function ਮੰਜ਼ਿਲ ਤਿਆਰੀ a peripheral timer/counter that can then be ਵਰਤਿਆ as the ਲੜੀਵਾਰ time counter time ਅਧਾਰ.
    #ਜੇ_ਅੰਤ /* portCONFIGURE_TIMER_FOR_RUN_TIME_STATS */

    #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_GET_ਲੜੀਵਾਰ_ਸਮਾਂ_COUNTER_ਮੁੱਲ
        #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_ALT_GET_ਲੜੀਵਾਰ_ਸਮਾਂ_COUNTER_ਮੁੱਲ
            #ਗਲਤੀ If config_GENERATE_ਲੜੀਵਾਰ_ਸਮਾਂ_STATS is defined then either ਪੋਰਟ_GET_ਲੜੀਵਾਰ_ਸਮਾਂ_COUNTER_ਮੁੱਲ or ਪੋਰਟ_ALT_GET_ਲੜੀਵਾਰ_ਸਮਾਂ_COUNTER_ਮੁੱਲ must also be defined.  See the examples provided and the FreeRTOS web site ਲਈ ਹੋਰ information.
        #ਜੇ_ਅੰਤ /* portALT_GET_RUN_TIME_COUNTER_VALUE */
    #ਜੇ_ਅੰਤ /* portGET_RUN_TIME_COUNTER_VALUE */

#ਜੇ_ਅੰਤ /* configGENERATE_RUN_TIME_STATS */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_CONFIGURE_TIMER_FOR_ਲੜੀਵਾਰ_ਸਮਾਂ_STATS
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_CONFIGURE_TIMER_FOR_ਲੜੀਵਾਰ_ਸਮਾਂ_STATS()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configUSE_MALLOC_FAILED_HOOK
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_MALLOC_FAILED_HOOK    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_PRIVILEGE_ਦੋਆਧਾਰੀ_ਅੰਕ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_PRIVILEGE_ਦੋਆਧਾਰੀ_ਅੰਕ    ( ( UBaseType_t ) 0x00 )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_YIELD_WITHIN_API
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_YIELD_WITHIN_API    ਪੋਰਟ_YIELD
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_SUPPRESS_TICKS_AND_SLEEP
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_SUPPRESS_TICKS_AND_SLEEP( x_Expected_Idle_ਸਮਾਂ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_EXPECTED_IDLE_ਸਮਾਂ_BEFORE_SLEEP
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_EXPECTED_IDLE_ਸਮਾਂ_BEFORE_SLEEP    2
#ਜੇ_ਅੰਤ

#ਜੇ config_EXPECTED_IDLE_ਸਮਾਂ_BEFORE_SLEEP < 2
    #ਗਲਤੀ config_EXPECTED_IDLE_ਸਮਾਂ_BEFORE_SLEEP must not be less than 2
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configUSE_TICKLESS_IDLE
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_TICKLESS_IDLE    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configPRE_SUPPRESS_TICKS_AND_SLEEP_PROCESSING
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configPRE_SUPPRESS_TICKS_AND_SLEEP_PROCESSING( x )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configPRE_SLEEP_PROCESSING
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configPRE_SLEEP_PROCESSING( x )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configPOST_SLEEP_PROCESSING
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configPOST_SLEEP_PROCESSING( x )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_USE_ਕਤਾਰ_SETS
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_USE_ਕਤਾਰ_SETS    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_TASK_ਵਰਤਦਾ_FLOATING_ਬਿੰਦੂ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TASK_ਵਰਤਦਾ_FLOATING_ਬਿੰਦੂ()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_ALLOCATE_SECURE_CONTEXT
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ALLOCATE_SECURE_CONTEXT( ul_Secure_ਢੇਰ_ਮਾਪ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_DONT_DISCARD
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_DONT_DISCARD
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_USE_ਸਮਾਂ_SLICING
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_USE_ਸਮਾਂ_SLICING    1
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configUSE_STATS_FORMATTING_FUNCTIONS
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_STATS_FORMATTING_FUNCTIONS    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_ASSERT_IF_ਰੁਕਾਵਟ_ਤਰਜੀਹ_INVALID
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ASSERT_IF_ਰੁਕਾਵਟ_ਤਰਜੀਹ_INVALID()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configUSE_TRACE_FACILITY
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_TRACE_FACILITY    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ mtCOVERAGE_TEST_MARKER
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ mtCOVERAGE_TEST_MARKER()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ mtCOVERAGE_TEST_DELAY
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ mtCOVERAGE_TEST_DELAY()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_ASSERT_IF_ਵਿੱਚ_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ASSERT_IF_ਵਿੱਚ_ISR()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_USE_ਪੋਰਟ_OPTIMISED_TASK_SELECTION
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_USE_ਪੋਰਟ_OPTIMISED_TASK_SELECTION    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_APPLICATION_ALLOCATED_ਢੇਰ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_APPLICATION_ALLOCATED_ਢੇਰ    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configUSE_TASK_NOTIFICATIONS
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_TASK_NOTIFICATIONS    1
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configTASK_NOTIFICATION_ARRAY_ENTRIES
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configTASK_NOTIFICATION_ARRAY_ENTRIES    1
#ਜੇ_ਅੰਤ

#ਜੇ configTASK_NOTIFICATION_ARRAY_ENTRIES < 1
    #ਗਲਤੀ configTASK_NOTIFICATION_ARRAY_ENTRIES must be at ਘੱਟ_ਤੋਂ_ਘੱਟ 1
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configUSE_POSIX_ERRNO
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_POSIX_ERRNO    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configUSE_SB_COMPLETED_CALLBACK

/* By default per-instance callbacks are not enabled for stream buffer or message buffer. */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_SB_COMPLETED_CALLBACK    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_TICK_TYPE_IS_ATOMIC
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TICK_TYPE_IS_ATOMIC    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_SUPPORT_STATIC_ਵੰਡ
    /* Defaults to 0 for backward compatibility. */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_SUPPORT_STATIC_ਵੰਡ    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_SUPPORT_DYNAMIC_ਵੰਡ
    /* Defaults to 1 for backward compatibility. */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_SUPPORT_DYNAMIC_ਵੰਡ    1
#ਜੇ_ਅੰਤ

#ਜੇ ( ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 ) && ( config_SUPPORT_DYNAMIC_ਵੰਡ != 1 ) )
    #ਗਲਤੀ configUSE_STATS_FORMATTING_FUNCTIONS cannot be ਵਰਤਿਆ without dynamic ਵੰਡ, but config_SUPPORT_DYNAMIC_ਵੰਡ is not ਸੈੱਟ ਮੰਜ਼ਿਲ 1.
#ਜੇ_ਅੰਤ

#ਜੇ ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 )
    #ਜੇ ( ( configUSE_TRACE_FACILITY != 1 ) && ( config_GENERATE_ਲੜੀਵਾਰ_ਸਮਾਂ_STATS != 1 ) )
        #ਗਲਤੀ configUSE_STATS_FORMATTING_FUNCTIONS is 1 but the functions it enables are not ਵਰਤਿਆ because neither configUSE_TRACE_FACILITY or config_GENERATE_ਲੜੀਵਾਰ_ਸਮਾਂ_STATS are 1.  ਸੈੱਟ_2 configUSE_STATS_FORMATTING_FUNCTIONS ਮੰਜ਼ਿਲ 0 ਵਿੱਚ FreeRTOSConfig.h.
    #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_ਢੇਰ_ਡੂੰਘਾਈ_TYPE

/* Defaults to uint16_t for backward compatibility, but can be overridden
 * in FreeRTOSConfig.h if uint16_t is too restrictive. */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_ਢੇਰ_ਡੂੰਘਾਈ_TYPE    uint16_t
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_ਲੜੀਵਾਰ_ਸਮਾਂ_COUNTER_TYPE

/* Defaults to uint32_t for backward compatibility, but can be overridden in
 * FreeRTOSConfig.h if uint32_t is too restrictive. */

    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_ਲੜੀਵਾਰ_ਸਮਾਂ_COUNTER_TYPE    uint32_t
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_ਸੁਨੇਹਾ_ਬਫਰ_ਲੰਮਾਈ_TYPE

/* Defaults to size_t for backward compatibility, but can be overridden
 * in FreeRTOSConfig.h if lengths will always be less than the number of bytes
 * in a size_t. */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_ਸੁਨੇਹਾ_ਬਫਰ_ਲੰਮਾਈ_TYPE    size_t
#ਜੇ_ਅੰਤ

/* Sanity check the configuration. */
#ਜੇ ( ( config_SUPPORT_STATIC_ਵੰਡ == 0 ) && ( config_SUPPORT_DYNAMIC_ਵੰਡ == 0 ) )
    #ਗਲਤੀ config_SUPPORT_STATIC_ਵੰਡ and config_SUPPORT_DYNAMIC_ਵੰਡ cannot both be 0, but can both be 1.
#ਜੇ_ਅੰਤ

#ਜੇ ( ( configUSE_RECURSIVE_MUTEXES == 1 ) && ( configUSE_MUTEXES != 1 ) )
    #ਗਲਤੀ configUSE_MUTEXES must be ਸੈੱਟ ਮੰਜ਼ਿਲ 1 ਮੰਜ਼ਿਲ use recursive mutexes
#ਜੇ_ਅੰਤ

#ਜੇ ( ( config_ਸੰਖਿਆ_ਦਾ_CORES > 1 ) && ( config_USE_ਪੋਰਟ_OPTIMISED_TASK_SELECTION != 0 ) )
    #ਗਲਤੀ config_USE_ਪੋਰਟ_OPTIMISED_TASK_SELECTION is not supported ਵਿੱਚ SMP
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_INITIAL_TICK_ਗਿਣਤੀ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_INITIAL_TICK_ਗਿਣਤੀ    0
#ਜੇ_ਅੰਤ

#ਜੇ ( ਪੋਰਟ_TICK_TYPE_IS_ATOMIC == 0 )

/* Either variables of tick type cannot be read atomically, or
 * portTICK_TYPE_IS_ATOMIC was not set - map the critical sections used when
 * the tick count is returned to the standard critical section macros. */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TICK_TYPE_ENTER_CRITICAL()                      ਪੋਰਟ_ENTER_CRITICAL()
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TICK_TYPE_EXIT_CRITICAL()                       ਪੋਰਟ_EXIT_CRITICAL()
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TICK_TYPE_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR()         ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR()
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TICK_TYPE_CLEAR_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR( x )    ਪੋਰਟ_CLEAR_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR( ( x ) )
#ਨਹੀਂਤਾਂ

/* The tick type can be read atomically, so critical sections used when the
 * tick count is returned can be defined away. */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TICK_TYPE_ENTER_CRITICAL()
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TICK_TYPE_EXIT_CRITICAL()
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TICK_TYPE_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR()         0
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TICK_TYPE_CLEAR_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR( x )    ( ਖਾਲੀ ) ( x )
#ਜੇ_ਅੰਤ /* if ( portTICK_TYPE_IS_ATOMIC == 0 ) */

/* Definitions to allow backward compatibility with FreeRTOS versions prior to
 * V8 if desired. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configENABLE_BACKWARD_COMPATIBILITY
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configENABLE_BACKWARD_COMPATIBILITY    1
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configPRINTF

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
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configPRINTF( X )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configMAX

/* The application writer has not provided their own MAX macro, so define
 * the following generic implementation. */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configMAX( a, b )    ( ( ( a ) > ( b ) ) ? ( a ) : ( b ) )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configMIN

/* The application writer has not provided their own MIN macro, so define
 * the following generic implementation. */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configMIN( a, b )    ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )
#ਜੇ_ਅੰਤ

#ਜੇ configENABLE_BACKWARD_COMPATIBILITY == 1
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ eTaskStateGet                 eTaskGetState
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_Tick_Type                  TickType_t
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTaskHandle                   TaskHandle_t
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Handle                  ਕਤਾਰ_Handle_t
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xSemaphoreHandle              SemaphoreHandle_t
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_ਸੈੱਟ_Handle               ਕਤਾਰ_ਸੈੱਟ_Handle_t
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_ਸੈੱਟ_Member_Handle         ਕਤਾਰ_ਸੈੱਟ_Member_Handle_t
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਸਮਾਂ_ਬਾਹਰਲਾ_Type                  ਸਮਾਂ_ਬਾਹਰਲਾ_t
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਸਮ੍ਰਿਤੀ_Region                 ਸਮ੍ਰਿਤੀ_Region_t
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTaskParameters               TaskParameters_t
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_Task_ਸਥਿਤੀ_Type               Task_ਸਥਿਤੀ_t
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTimerHandle                  TimerHandle_t
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xCoRoutineHandle              CoRoutineHandle_t
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pd_TASK_HOOK_ਕੋਡ              TaskHookFunction_t
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TICK_ਦਰ_MS              ਪੋਰਟ_TICK_PERIOD_MS
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pcTaskGetTaskName             pcTaskGetName
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pcTimerGetTimerName           pcTimerGetName
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pc_ਕਤਾਰ_Get_ਕਤਾਰ_Name           pc_ਕਤਾਰ_Get_Name
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ vTaskGetTaskInfo              vTaskGetInfo
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_Task_Get_Idle_ਲੜੀਵਾਰ_ਸਮਾਂ_Counter    ul_Task_Get_Idle_ਲੜੀਵਾਰ_ਸਮਾਂ_Counter

/* Backward compatibility within the scheduler code only - these definitions
 * are not really required but are included for completeness. */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tmrTIMER_CALLBACK             TimerCallbackFunction_t
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pd_TASK_ਕੋਡ                   TaskFunction_t
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_List_ਵਸਤੂ                     List_ਵਸਤੂ_t
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xList                         List_t

/* For libraries that break the list data hiding, and access list structure
 * members directly (which is not supposed to be done). */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pxContainer                   pvContainer
#ਜੇ_ਅੰਤ /* configENABLE_BACKWARD_COMPATIBILITY */

#ਜੇ ( configUSE_ALTERNATIVE_API != 0 )
    #ਗਲਤੀ The alternative API was deprecated some time ago, and was removed ਵਿੱਚ FreeRTOS V9.0 0
#ਜੇ_ਅੰਤ

/* Set configUSE_TASK_FPU_SUPPORT to 0 to omit floating point support even
 * if floating point hardware is otherwise supported by the FreeRTOS port in use.
 * This constant is not supported by all FreeRTOS ports that include floating
 * point support. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configUSE_TASK_FPU_SUPPORT
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_TASK_FPU_SUPPORT    1
#ਜੇ_ਅੰਤ

/* Set configENABLE_MPU to 1 to enable MPU support and 0 to disable it. This is
 * currently used in ARMv8M ports. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configENABLE_MPU
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configENABLE_MPU    0
#ਜੇ_ਅੰਤ

/* Set configENABLE_FPU to 1 to enable FPU support and 0 to disable it. This is
 * currently used in ARMv8M ports. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configENABLE_FPU
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configENABLE_FPU    1
#ਜੇ_ਅੰਤ

/* Set configENABLE_MVE to 1 to enable MVE support and 0 to disable it. This is
 * currently used in ARMv8M ports. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configENABLE_MVE
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configENABLE_MVE    0
#ਜੇ_ਅੰਤ

/* Set configENABLE_TRUSTZONE to 1 enable TrustZone support and 0 to disable it.
 * This is currently used in ARMv8M ports. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configENABLE_TRUSTZONE
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configENABLE_TRUSTZONE    1
#ਜੇ_ਅੰਤ

/* Set configRUN_FREERTOS_SECURE_ONLY to 1 to run the FreeRTOS ARMv8M port on
 * the Secure Side only. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_ਲੜੀਵਾਰ_FREERTOS_SECURE_ONLY
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_ਲੜੀਵਾਰ_FREERTOS_SECURE_ONLY    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_ਲੜੀਵਾਰ_ADDITIONAL_TESTS
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_ਲੜੀਵਾਰ_ADDITIONAL_TESTS    0
#ਜੇ_ਅੰਤ


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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tsk_STATIC_AND_DYNAMIC_ਵੰਡ_POSSIBLE                                                                                     \
    ( ( ( ਪੋਰਟ_USING_MPU_WRAPPERS == 0 ) && ( config_SUPPORT_DYNAMIC_ਵੰਡ == 1 ) && ( config_SUPPORT_STATIC_ਵੰਡ == 1 ) ) || \
      ( ( ਪੋਰਟ_USING_MPU_WRAPPERS == 1 ) && ( config_SUPPORT_DYNAMIC_ਵੰਡ == 1 ) ) )

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
ਬਣਤਰ x_STATIC_LIST_ਵਸਤੂ
{
    #ਜੇ ( config_USE_LIST_ਅੰਕੜੇ_INTEGRITY_CHECK_ਬਾਈਟਾਂ == 1 )
        TickType_t xDummy1;
    #ਜੇ_ਅੰਤ
    TickType_t xDummy2;
    ਖਾਲੀ * pvDummy3[ 4 ];
    #ਜੇ ( config_USE_LIST_ਅੰਕੜੇ_INTEGRITY_CHECK_ਬਾਈਟਾਂ == 1 )
        TickType_t xDummy4;
    #ਜੇ_ਅੰਤ
};
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ x_STATIC_LIST_ਵਸਤੂ Static_List_ਵਸਤੂ_t;

#ਜੇ ( config_USE_MINI_LIST_ਵਸਤੂ == 1 )
    /* See the comments above the struct xSTATIC_LIST_ITEM definition. */
    ਬਣਤਰ x_STATIC_MINI_LIST_ਵਸਤੂ
    {
        #ਜੇ ( config_USE_LIST_ਅੰਕੜੇ_INTEGRITY_CHECK_ਬਾਈਟਾਂ == 1 )
            TickType_t xDummy1;
        #ਜੇ_ਅੰਤ
        TickType_t xDummy2;
        ਖਾਲੀ * pvDummy3[ 2 ];
    };
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ x_STATIC_MINI_LIST_ਵਸਤੂ Static_Mini_List_ਵਸਤੂ_t;
#ਨਹੀਂਤਾਂ /* if ( configUSE_MINI_LIST_ITEM == 1 ) */
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ x_STATIC_LIST_ਵਸਤੂ      Static_Mini_List_ਵਸਤੂ_t;
#ਜੇ_ਅੰਤ /* if ( configUSE_MINI_LIST_ITEM == 1 ) */

/* See the comments above the struct xSTATIC_LIST_ITEM definition. */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ xSTATIC_LIST
{
    #ਜੇ ( config_USE_LIST_ਅੰਕੜੇ_INTEGRITY_CHECK_ਬਾਈਟਾਂ == 1 )
        TickType_t xDummy1;
    #ਜੇ_ਅੰਤ
    UBaseType_t uxDummy2;
    ਖਾਲੀ * pvDummy3;
    Static_Mini_List_ਵਸਤੂ_t xDummy4;
    #ਜੇ ( config_USE_LIST_ਅੰਕੜੇ_INTEGRITY_CHECK_ਬਾਈਟਾਂ == 1 )
        TickType_t xDummy5;
    #ਜੇ_ਅੰਤ
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
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ xSTATIC_TCB
{
    ਖਾਲੀ * pxDummy1;
    #ਜੇ ( ਪੋਰਟ_USING_MPU_WRAPPERS == 1 )
        xMPU_SETTINGS xDummy2;
    #ਜੇ_ਅੰਤ
    Static_List_ਵਸਤੂ_t xDummy3[ 2 ];
    UBaseType_t uxDummy5;
    ਖਾਲੀ * pxDummy6;
    uint8_t ucDummy7[ configMAX_TASK_NAME_LEN ];
    #ਜੇ ( config_ਸੰਖਿਆ_ਦਾ_CORES > 1 )
        ਅਧਾਰ_Type_t xDummyCoreID;
    #ਜੇ_ਅੰਤ /* configNUMBER_OF_CORES > 1 */
    #ਜੇ ( ( ਪੋਰਟ_ਢੇਰ_GROWTH > 0 ) || ( config_ਰਿਕਾਰਡ_ਢੇਰ_ਉੱਚਾ_ਪਤਾ == 1 ) )
        ਖਾਲੀ * pxDummy8;
    #ਜੇ_ਅੰਤ
    #ਜੇ ( ਪੋਰਟ_CRITICAL_NESTING_ਵਿੱਚ_TCB == 1 )
        UBaseType_t uxDummy9;
    #ਜੇ_ਅੰਤ
    #ਜੇ ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy10[ 2 ];
    #ਜੇ_ਅੰਤ
    #ਜੇ ( configUSE_MUTEXES == 1 )
        UBaseType_t uxDummy12[ 2 ];
    #ਜੇ_ਅੰਤ
    #ਜੇ ( config_USE_APPLICATION_TASK_ਟੈਗ == 1 )
        ਖਾਲੀ * pxDummy14;
    #ਜੇ_ਅੰਤ
    #ਜੇ ( config_ਅੰਕ_THREAD_LOCAL_STORAGE_ਸੂਚਕ_ਸਾਰੇ > 0 )
        ਖਾਲੀ * pvDummy15[ config_ਅੰਕ_THREAD_LOCAL_STORAGE_ਸੂਚਕ_ਸਾਰੇ ];
    #ਜੇ_ਅੰਤ
    #ਜੇ ( config_GENERATE_ਲੜੀਵਾਰ_ਸਮਾਂ_STATS == 1 )
        config_ਲੜੀਵਾਰ_ਸਮਾਂ_COUNTER_TYPE ulDummy16;
    #ਜੇ_ਅੰਤ
    #ਜੇ ( ( configUSE_NEWLIB_REENTRANT == 1 ) || ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 ) )
        config_TLS_ਖੰਡ_TYPE xDummy17;
    #ਜੇ_ਅੰਤ
    #ਜੇ ( configUSE_TASK_NOTIFICATIONS == 1 )
        uint32_t ulDummy18[ configTASK_NOTIFICATION_ARRAY_ENTRIES ];
        uint8_t ucDummy19[ configTASK_NOTIFICATION_ARRAY_ENTRIES ];
    #ਜੇ_ਅੰਤ
    #ਜੇ ( tsk_STATIC_AND_DYNAMIC_ਵੰਡ_POSSIBLE != 0 )
        uint8_t uxDummy20;
    #ਜੇ_ਅੰਤ

    #ਜੇ ( INCLUDE_xTaskAbortDelay == 1 )
        uint8_t ucDummy21;
    #ਜੇ_ਅੰਤ
    #ਜੇ ( configUSE_POSIX_ERRNO == 1 )
        ਪੂਰਨ iDummy22;
    #ਜੇ_ਅੰਤ
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
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ x_STATIC_ਕਤਾਰ
{
    ਖਾਲੀ * pvDummy1[ 3 ];

    ਸੰਘ
    {
        ਖਾਲੀ * pvDummy2;
        UBaseType_t uxDummy2;
    } u;

    StaticList_t xDummy3[ 2 ];
    UBaseType_t uxDummy4[ 3 ];
    uint8_t ucDummy5[ 2 ];

    #ਜੇ ( ( config_SUPPORT_STATIC_ਵੰਡ == 1 ) && ( config_SUPPORT_DYNAMIC_ਵੰਡ == 1 ) )
        uint8_t ucDummy6;
    #ਜੇ_ਅੰਤ

    #ਜੇ ( config_USE_ਕਤਾਰ_SETS == 1 )
        ਖਾਲੀ * pvDummy7;
    #ਜੇ_ਅੰਤ

    #ਜੇ ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy8;
        uint8_t ucDummy9;
    #ਜੇ_ਅੰਤ
    ਪੋਰਟ_MUX_TYPE x_Dummy_ਕਤਾਰ_ਤਾਲਾ;
} Static_ਕਤਾਰ_t;
ਕਿਸਮਪਰਿਭਾਸ਼ਾ Static_ਕਤਾਰ_t StaticSemaphore_t;

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
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ x_STATIC_EVENT_ਸਮੂਹ
{
    TickType_t xDummy1;
    StaticList_t xDummy2;

    #ਜੇ ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy3;
    #ਜੇ_ਅੰਤ

    #ਜੇ ( ( config_SUPPORT_STATIC_ਵੰਡ == 1 ) && ( config_SUPPORT_DYNAMIC_ਵੰਡ == 1 ) )
        uint8_t ucDummy4;
    #ਜੇ_ਅੰਤ
    ਪੋਰਟ_MUX_TYPE x_Dummy_Event_ਸਮੂਹ_ਤਾਲਾ;
} Static_Event_ਸਮੂਹ_t;

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
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ xSTATIC_TIMER
{
    ਖਾਲੀ * pvDummy1;
    Static_List_ਵਸਤੂ_t xDummy2;
    TickType_t xDummy3;
    ਖਾਲੀ * pvDummy5;
    TaskFunction_t pvDummy6;
    #ਜੇ ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy7;
    #ਜੇ_ਅੰਤ
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
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ x_STATIC_ਧਾਰਾ_ਬਫਰ
{
    size_t uxDummy1[ 4 ];
    ਖਾਲੀ * pvDummy2[ 3 ];
    uint8_t ucDummy3;
    #ਜੇ ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy4;
    #ਜੇ_ਅੰਤ
    #ਜੇ ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        ਖਾਲੀ * pvDummy5[ 2 ];
    #ਜੇ_ਅੰਤ
    ਪੋਰਟ_MUX_TYPE x_Dummy_ਧਾਰਾ_ਬਫਰ_ਤਾਲਾ;
} Static_ਧਾਰਾ_ਬਫਰ_t;

/* Message buffers are built on stream buffers. */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ Static_ਧਾਰਾ_ਬਫਰ_t Static_ਸੁਨੇਹਾ_ਬਫਰ_t;

/* *INDENT-OFF* */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
    }
#ਜੇ_ਅੰਤ
/* *INDENT-ON* */

/*-----------------------------------------------------------
* IDF Compatibility
*----------------------------------------------------------*/

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ESP_PLATFORM

/* *INDENT-OFF* */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
    ਬਾਹਰੀ "C" {
#ਜੇ_ਅੰਤ
/* *INDENT-ON* */

/*
 * Default values for trace macros added by ESP-IDF and are not part of Vanilla FreeRTOS
 */

    #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ISR_EXIT_ਮੰਜ਼ਿਲ_SCHEDULER
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ISR_EXIT_ਮੰਜ਼ਿਲ_SCHEDULER()
    #ਜੇ_ਅੰਤ

    #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceISR_EXIT
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceISR_EXIT()
    #ਜੇ_ਅੰਤ

    #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceISR_ENTER
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceISR_ENTER( _n_ )
    #ਜੇ_ਅੰਤ

    #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਕਤਾਰ_SEMAPHORE_RECEIVE
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਕਤਾਰ_SEMAPHORE_RECEIVE( px_ਕਤਾਰ )
    #ਜੇ_ਅੰਤ

    #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਕਤਾਰ_GIVE_FROM_ISR
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਕਤਾਰ_GIVE_FROM_ISR( px_ਕਤਾਰ )
    #ਜੇ_ਅੰਤ

    #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਕਤਾਰ_GIVE_FROM_ISR_FAILED
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਕਤਾਰ_GIVE_FROM_ISR_FAILED( px_ਕਤਾਰ )
    #ਜੇ_ਅੰਤ

/* *INDENT-OFF* */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
    }
#ਜੇ_ਅੰਤ
/* *INDENT-ON* */

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
    #ਅਨ੍ਤਰ੍ਭਾਵ "freertos/idf_additions.h"

#ਜੇ_ਅੰਤ /* ESP_PLATFORM */

#ਜੇ_ਅੰਤ /* INC_FREERTOS_H */
