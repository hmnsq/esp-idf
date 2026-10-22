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

#اگر_غیر_تعریف_شدہ INC_FREERTOS_H
#تعریف_کرو INC_FREERTOS_H

/*
 * Include the generic headers required for the FreeRTOS port being used.
 */
#شامل <stddef.h>

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
#شامل <stdint.h> /* READ COMMENT ABOVE. */

/* *INDENT-OFF* */
#اگر_تعریف_شدہ __cplusplus
    خارجی "C" {
#اگر_ختم
/* *INDENT-ON* */

/* Application specific configuration options. */
#شامل "FreeRTOSConfig.h"

/* Basic FreeRTOS definitions. */
#شامل "projdefs.h"

/* Definitions specific to the port being used. */
#شامل "portable.h"

/* Must be defaulted before configUSE_NEWLIB_REENTRANT is used below. */
#اگر_غیر_تعریف_شدہ configUSE_NEWLIB_REENTRANT
    #تعریف_کرو configUSE_NEWLIB_REENTRANT    0
#اگر_ختم

/* Required if struct _reent is used. */
#اگر ( configUSE_NEWLIB_REENTRANT == 1 )

/* Note Newlib support has been included by popular demand, but is not
 * used by the FreeRTOS maintainers themselves.  FreeRTOS is not
 * responsible for resulting newlib operation.  User must be familiar with
 * newlib and must provide system-wide implementations of the necessary
 * stubs. Be warned that (at the time of writing) the current newlib design
 * implements a system-wide malloc() that must be provided with locks.
 *
 * See the third party link http://www.nadler.com/embedded/newlibAndFreeRTOS.html
 * for additional information. */
    #شامل <reent.h>

    #تعریف_کرو configUSE_C_RUNTIME_TLS_SUPPORT    1

    #اگر_غیر_تعریف_شدہ config_TLS_بلاک_TYPE
        #تعریف_کرو config_TLS_بلاک_TYPE           ساخت _reent
    #اگر_ختم

    #اگر_غیر_تعریف_شدہ config_INIT_TLS_بلاک
        #تعریف_کرو config_INIT_TLS_بلاک( xTLSBlock )    _REENT_INIT_PTR( &( xTLSBlock ) )
    #اگر_ختم

    #اگر_غیر_تعریف_شدہ config_سیٹ_TLS_بلاک
        #تعریف_کرو config_سیٹ_TLS_بلاک( xTLSBlock )    _impure_ptr = &( xTLSBlock )
    #اگر_ختم

    #اگر_غیر_تعریف_شدہ config_DEINIT_TLS_بلاک
        #تعریف_کرو config_DEINIT_TLS_بلاک( xTLSBlock )    _reclaim_reent( &( xTLSBlock ) )
    #اگر_ختم
#اگر_ختم /* if ( configUSE_NEWLIB_REENTRANT == 1 ) */

#اگر_غیر_تعریف_شدہ configUSE_C_RUNTIME_TLS_SUPPORT
    #تعریف_کرو configUSE_C_RUNTIME_TLS_SUPPORT    0
#اگر_ختم

#اگر ( ( configUSE_NEWLIB_REENTRANT == 0 ) && ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 ) )

    #اگر_غیر_تعریف_شدہ config_TLS_بلاک_TYPE
        #غلطی Missing definition:  config_TLS_بلاک_TYPE must be defined اندر FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is سیٹ مقصد 1.
    #اگر_ختم

    #اگر_غیر_تعریف_شدہ config_INIT_TLS_بلاک
        #غلطی Missing definition:  config_INIT_TLS_بلاک must be defined اندر FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is سیٹ مقصد 1.
    #اگر_ختم

    #اگر_غیر_تعریف_شدہ config_سیٹ_TLS_بلاک
        #غلطی Missing definition:  config_سیٹ_TLS_بلاک must be defined اندر FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is سیٹ مقصد 1.
    #اگر_ختم

    #اگر_غیر_تعریف_شدہ config_DEINIT_TLS_بلاک
        #غلطی Missing definition:  config_DEINIT_TLS_بلاک must be defined اندر FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is سیٹ مقصد 1.
    #اگر_ختم
#اگر_ختم /* if ( ( configUSE_NEWLIB_REENTRANT == 0 ) && ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 ) ) */

/*
 * Check all the required application specific macros have been defined.
 * These macros are application specific and (as downloaded) are defined
 * within FreeRTOSConfig.h.
 */

#اگر_غیر_تعریف_شدہ config_نمبر_کا_CORES
    #غلطی Missing definition:  config_نمبر_کا_CORES must be defined اندر FreeRTOSConfig.h
#اگر_ختم

#اگر ( config_نمبر_کا_CORES > 1 )
    #اگر_غیر_تعریف_شدہ پورٹ_GET_CORE_ID
        #غلطی "Missing definition: portGET_CORE_ID() must be defined if in portmacro.h if configNUMBER_OF_CORES > 1"
    #اگر_ختم
    #اگر_غیر_تعریف_شدہ پورٹ_YIELD_CORE
        #غلطی "Missing definition: portYIELD_CORE() must be defined if in portmacro.h if configNUMBER_OF_CORES > 1"
    #اگر_ختم
#وگرنہ_اگر ( config_نمبر_کا_CORES == 1 )
    #تعریف_منسوخ پورٹ_GET_CORE_ID
    #تعریف_کرو پورٹ_GET_CORE_ID()    0
#ورنہ
    #غلطی config_نمبر_کا_CORES must be defined مقصد either 1 or > 1.
#اگر_ختم /* if ( configNUMBER_OF_CORES > 1 ) */

#اگر_غیر_تعریف_شدہ config_اقل_ڈھیر_سائز
    #غلطی Missing definition:  config_اقل_ڈھیر_سائز must be defined اندر FreeRTOSConfig.h.  config_اقل_ڈھیر_سائز متعین_کرتا the سائز (اندر الفاظ) کا the ڈھیر allocated مقصد the idle task.  Refer مقصد the demo project provided برائے your پورٹ برائے a suitable قدر.
#اگر_ختم

#اگر_غیر_تعریف_شدہ configMAX_PRIORITIES
    #غلطی Missing definition:  configMAX_PRIORITIES must be defined اندر FreeRTOSConfig.h.  See the Configuration section کا the FreeRTOS API documentation برائے details.
#اگر_ختم

#اگر configMAX_PRIORITIES < 1
    #غلطی configMAX_PRIORITIES must be defined مقصد be greater than or equal مقصد 1.
#اگر_ختم

#اگر_غیر_تعریف_شدہ configUSE_PREEMPTION
    #غلطی Missing definition:  configUSE_PREEMPTION must be defined اندر FreeRTOSConfig.h as either 1 or 0.  See the Configuration section کا the FreeRTOS API documentation برائے details.
#اگر_ختم

#اگر_غیر_تعریف_شدہ configUSE_IDLE_HOOK
    #غلطی Missing definition:  configUSE_IDLE_HOOK must be defined اندر FreeRTOSConfig.h as either 1 or 0.  See the Configuration section کا the FreeRTOS API documentation برائے details.
#اگر_ختم

#اگر_غیر_تعریف_شدہ configUSE_TICK_HOOK
    #غلطی Missing definition:  configUSE_TICK_HOOK must be defined اندر FreeRTOSConfig.h as either 1 or 0.  See the Configuration section کا the FreeRTOS API documentation برائے details.
#اگر_ختم

#اگر_غیر_تعریف_شدہ config_USE_16_ثنائی_ہندسہ_TICKS
    #غلطی Missing definition:  config_USE_16_ثنائی_ہندسہ_TICKS must be defined اندر FreeRTOSConfig.h as either 1 or 0.  See the Configuration section کا the FreeRTOS API documentation برائے details.
#اگر_ختم

#اگر_غیر_تعریف_شدہ configUSE_CO_ROUTINES
    #تعریف_کرو configUSE_CO_ROUTINES    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ INCLUDE_v_Task_ترجیح_سیٹ
    #تعریف_کرو INCLUDE_v_Task_ترجیح_سیٹ    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ INCLUDE_ux_Task_ترجیح_Get
    #تعریف_کرو INCLUDE_ux_Task_ترجیح_Get    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ INCLUDE_vTaskDelete
    #تعریف_کرو INCLUDE_vTaskDelete    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ INCLUDE_vTaskSuspend
    #تعریف_کرو INCLUDE_vTaskSuspend    0
#اگر_ختم

#اگر_تعریف_شدہ INCLUDE_xTaskDelayUntil
    #اگر_تعریف_شدہ INCLUDE_vTaskDelayUntil

/* INCLUDE_vTaskDelayUntil was replaced by INCLUDE_xTaskDelayUntil.  Backward
 * compatibility is maintained if only one or the other is defined, but
 * there is a conflict if both are defined. */
        #غلطی INCLUDE_vTaskDelayUntil and INCLUDE_xTaskDelayUntil are both defined.  INCLUDE_vTaskDelayUntil is no longer required and should be removed
    #اگر_ختم
#اگر_ختم

#اگر_غیر_تعریف_شدہ INCLUDE_xTaskDelayUntil
    #اگر_تعریف_شدہ INCLUDE_vTaskDelayUntil

/* If INCLUDE_vTaskDelayUntil is set but INCLUDE_xTaskDelayUntil is not then
 * the project's FreeRTOSConfig.h probably pre-dates the introduction of
 * xTaskDelayUntil and setting INCLUDE_xTaskDelayUntil to whatever
 * INCLUDE_vTaskDelayUntil is set to will ensure backward compatibility.
 */
        #تعریف_کرو INCLUDE_xTaskDelayUntil    INCLUDE_vTaskDelayUntil
    #اگر_ختم
#اگر_ختم

#اگر_غیر_تعریف_شدہ INCLUDE_xTaskDelayUntil
    #تعریف_کرو INCLUDE_xTaskDelayUntil    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ INCLUDE_vTaskDelay
    #تعریف_کرو INCLUDE_vTaskDelay    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ INCLUDE_xTaskGetIdleTaskHandle
    #تعریف_کرو INCLUDE_xTaskGetIdleTaskHandle    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ INCLUDE_xTaskAbortDelay
    #تعریف_کرو INCLUDE_xTaskAbortDelay    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ INCLUDE_x_قطار_Get_Mutex_Holder
    #تعریف_کرو INCLUDE_x_قطار_Get_Mutex_Holder    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ INCLUDE_xSemaphoreGetMutexHolder
    #تعریف_کرو INCLUDE_xSemaphoreGetMutexHolder    INCLUDE_x_قطار_Get_Mutex_Holder
#اگر_ختم

#اگر_غیر_تعریف_شدہ INCLUDE_xTaskGetHandle
    #تعریف_کرو INCLUDE_xTaskGetHandle    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ INCLUDE_ux_Task_Get_ڈھیر_بلند_Water_Mark
    #تعریف_کرو INCLUDE_ux_Task_Get_ڈھیر_بلند_Water_Mark    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ INCLUDE_ux_Task_Get_ڈھیر_بلند_Water_Mark2
    #تعریف_کرو INCLUDE_ux_Task_Get_ڈھیر_بلند_Water_Mark2    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ INCLUDE_eTaskGetState
    #تعریف_کرو INCLUDE_eTaskGetState    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ INCLUDE_xTaskResumeFromISR
    #تعریف_کرو INCLUDE_xTaskResumeFromISR    1
#اگر_ختم

#اگر_غیر_تعریف_شدہ INCLUDE_xTimerPendFunctionCall
    #تعریف_کرو INCLUDE_xTimerPendFunctionCall    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ INCLUDE_xTaskGetSchedulerState
    #تعریف_کرو INCLUDE_xTaskGetSchedulerState    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ INCLUDE_xTaskGetCurrentTaskHandle
    #تعریف_کرو INCLUDE_xTaskGetCurrentTaskHandle    1
#اگر_ختم

#اگر configUSE_CO_ROUTINES != 0
    #اگر_غیر_تعریف_شدہ configMAX_CO_ROUTINE_PRIORITIES
        #غلطی configMAX_CO_ROUTINE_PRIORITIES must be greater than or equal مقصد 1.
    #اگر_ختم
#اگر_ختم

#اگر_غیر_تعریف_شدہ configUSE_DAEMON_TASK_STARTUP_HOOK
    #تعریف_کرو configUSE_DAEMON_TASK_STARTUP_HOOK    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ config_USE_APPLICATION_TASK_لیبل
    #تعریف_کرو config_USE_APPLICATION_TASK_لیبل    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ config_عدد_THREAD_LOCAL_STORAGE_اشارے
    #تعریف_کرو config_عدد_THREAD_LOCAL_STORAGE_اشارے    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ configUSE_RECURSIVE_MUTEXES
    #تعریف_کرو configUSE_RECURSIVE_MUTEXES    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ configUSE_MUTEXES
    #تعریف_کرو configUSE_MUTEXES    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ configUSE_TIMERS
    #تعریف_کرو configUSE_TIMERS    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ configUSE_COUNTING_SEMAPHORES
    #تعریف_کرو configUSE_COUNTING_SEMAPHORES    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ configUSE_ALTERNATIVE_API
    #تعریف_کرو configUSE_ALTERNATIVE_API    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ پورٹ_CRITICAL_NESTING_اندر_TCB
    #تعریف_کرو پورٹ_CRITICAL_NESTING_اندر_TCB    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ configMAX_TASK_NAME_LEN
    #تعریف_کرو configMAX_TASK_NAME_LEN    16
#اگر_ختم

#اگر_غیر_تعریف_شدہ configIDLE_SHOULD_YIELD
    #تعریف_کرو configIDLE_SHOULD_YIELD    1
#اگر_ختم

#اگر configMAX_TASK_NAME_LEN < 1
    #غلطی configMAX_TASK_NAME_LEN must be سیٹ مقصد a minimum کا 1 اندر FreeRTOSConfig.h
#اگر_ختم

#اگر_غیر_تعریف_شدہ configASSERT
    #تعریف_کرو configASSERT( x )
    #تعریف_کرو configASSERT_DEFINED    0
#ورنہ
    #تعریف_کرو configASSERT_DEFINED    1
#اگر_ختم

/* configPRECONDITION should be defined as configASSERT.
 * The CBMC proofs need a way to track assumptions and assertions.
 * A configPRECONDITION statement should express an implicit invariant or
 * assumption made.  A configASSERT statement should express an invariant that must
 * hold explicit before calling the code. */
#اگر_غیر_تعریف_شدہ configPRECONDITION
    #تعریف_کرو configPRECONDITION( X )    configASSERT( X )
    #تعریف_کرو configPRECONDITION_DEFINED    0
#ورنہ
    #تعریف_کرو configPRECONDITION_DEFINED    1
#اگر_ختم

#اگر_غیر_تعریف_شدہ پورٹ_حافظہ_BARRIER
    #تعریف_کرو پورٹ_حافظہ_BARRIER()
#اگر_ختم

#اگر_غیر_تعریف_شدہ پورٹ_SOFTWARE_BARRIER
    #تعریف_کرو پورٹ_SOFTWARE_BARRIER()
#اگر_ختم

/* The timers module relies on xTaskGetSchedulerState(). */
#اگر configUSE_TIMERS == 1

    #اگر_غیر_تعریف_شدہ config_TIMER_TASK_ترجیح
        #غلطی If configUSE_TIMERS is سیٹ مقصد 1 then config_TIMER_TASK_ترجیح must also be defined.
    #اگر_ختم /* configTIMER_TASK_PRIORITY */

    #اگر_غیر_تعریف_شدہ config_TIMER_قطار_لمبائی
        #غلطی If configUSE_TIMERS is سیٹ مقصد 1 then config_TIMER_قطار_لمبائی must also be defined.
    #اگر_ختم /* configTIMER_QUEUE_LENGTH */

    #اگر_غیر_تعریف_شدہ config_TIMER_TASK_ڈھیر_گہرائی
        #غلطی If configUSE_TIMERS is سیٹ مقصد 1 then config_TIMER_TASK_ڈھیر_گہرائی must also be defined.
    #اگر_ختم /* configTIMER_TASK_STACK_DEPTH */

#اگر_ختم /* configUSE_TIMERS */

#اگر_غیر_تعریف_شدہ پورٹ_سیٹ_مداخلت_نقاب_FROM_ISR
    #تعریف_کرو پورٹ_سیٹ_مداخلت_نقاب_FROM_ISR()    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ پورٹ_CLEAR_مداخلت_نقاب_FROM_ISR
    #تعریف_کرو پورٹ_CLEAR_مداخلت_نقاب_FROM_ISR( ux_Saved_حالت_قدر )    ( خالی ) ( ux_Saved_حالت_قدر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ پورٹ_CLEAN_اوپر_TCB
    #تعریف_کرو پورٹ_CLEAN_اوپر_TCB( pxTCB )    ( خالی ) ( pxTCB )
#اگر_ختم

#اگر_غیر_تعریف_شدہ پورٹ_PRE_TASK_DELETE_HOOK
    #تعریف_کرو پورٹ_PRE_TASK_DELETE_HOOK( pv_Task_مقصد_Delete, pxYieldPending )
#اگر_ختم

#اگر_غیر_تعریف_شدہ پورٹ_تیاری_TCB
    #تعریف_کرو پورٹ_تیاری_TCB( pxTCB )    ( خالی ) ( pxTCB )
#اگر_ختم

#اگر_غیر_تعریف_شدہ config_قطار_REGISTRY_سائز
    #تعریف_کرو config_قطار_REGISTRY_سائز    0U
#اگر_ختم

#اگر ( config_قطار_REGISTRY_سائز < 1 )
    #تعریف_کرو v_قطار_جوڑو_مقصد_Registry( x_قطار, pcName )
    #تعریف_کرو v_قطار_Unregister_قطار( x_قطار )
    #تعریف_کرو pc_قطار_Get_Name( x_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ config_USE_MINI_LIST_شے
    #تعریف_کرو config_USE_MINI_LIST_شے    1
#اگر_ختم

#اگر_غیر_تعریف_شدہ پورٹ_اشارہ_گر_سائز_TYPE
    #تعریف_کرو پورٹ_اشارہ_گر_سائز_TYPE    uint32_t
#اگر_ختم

/* Remove any unused trace macros. */
#اگر_غیر_تعریف_شدہ trace_آغاز

/* Used to perform any necessary initialisation - for example, open a file
 * into which trace is to be written. */
    #تعریف_کرو trace_آغاز()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_انتہا

/* Use to close a trace, for example close a file into which trace has been
 * written. */
    #تعریف_کرو trace_انتہا()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_TASK_SWITCHED_اندر

/* Called after a task has been selected to run.  pxCurrentTCB holds a pointer
 * to the task control block of the selected task. */
    #تعریف_کرو trace_TASK_SWITCHED_اندر()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_INCREASE_TICK_گنتی

/* Called before stepping the tick count after waking from tickless idle
 * sleep. */
    #تعریف_کرو trace_INCREASE_TICK_گنتی( x )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_کم_POWER_IDLE_BEGIN
    /* Called immediately before entering tickless idle. */
    #تعریف_کرو trace_کم_POWER_IDLE_BEGIN()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_کم_POWER_IDLE_انتہا
    /* Called when returning to the Idle task after a tickless idle. */
    #تعریف_کرو trace_کم_POWER_IDLE_انتہا()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_TASK_SWITCHED_برآمد

/* Called before a task has been selected to run.  pxCurrentTCB holds a pointer
 * to the task control block of the task being switched out. */
    #تعریف_کرو trace_TASK_SWITCHED_برآمد()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_TASK_ترجیح_INHERIT

/* Called when a task attempts to take a mutex that is already held by a
 * lower priority task.  pxTCBOfMutexHolder is a pointer to the TCB of the task
 * that holds the mutex.  uxInheritedPriority is the priority the mutex holder
 * will inherit (the priority of the task that is attempting to obtain the
 * muted. */
    #تعریف_کرو trace_TASK_ترجیح_INHERIT( pxTCBOfMutexHolder, ux_Inherited_ترجیح )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_TASK_ترجیح_DISINHERIT

/* Called when a task releases a mutex, the holding of which had resulted in
 * the task inheriting the priority of a higher priority task.
 * pxTCBOfMutexHolder is a pointer to the TCB of the task that is releasing the
 * mutex.  uxOriginalPriority is the task's configured (base) priority. */
    #تعریف_کرو trace_TASK_ترجیح_DISINHERIT( pxTCBOfMutexHolder, ux_Original_ترجیح )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_BLOCKING_ON_قطار_RECEIVE

/* Task is about to block because it cannot read from a
 * queue/mutex/semaphore.  pxQueue is a pointer to the queue/mutex/semaphore
 * upon which the read was attempted.  pxCurrentTCB points to the TCB of the
 * task that attempted the read. */
    #تعریف_کرو trace_BLOCKING_ON_قطار_RECEIVE( px_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_BLOCKING_ON_قطار_PEEK

/* Task is about to block because it cannot read from a
 * queue/mutex/semaphore.  pxQueue is a pointer to the queue/mutex/semaphore
 * upon which the read was attempted.  pxCurrentTCB points to the TCB of the
 * task that attempted the read. */
    #تعریف_کرو trace_BLOCKING_ON_قطار_PEEK( px_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_BLOCKING_ON_قطار_SEND

/* Task is about to block because it cannot write to a
 * queue/mutex/semaphore.  pxQueue is a pointer to the queue/mutex/semaphore
 * upon which the write was attempted.  pxCurrentTCB points to the TCB of the
 * task that attempted the write. */
    #تعریف_کرو trace_BLOCKING_ON_قطار_SEND( px_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ config_CHECK_FOR_ڈھیر_حد_سے_تجاوز
    #تعریف_کرو config_CHECK_FOR_ڈھیر_حد_سے_تجاوز    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ config_ریکارڈ_ڈھیر_بلند_پتہ
    #تعریف_کرو config_ریکارڈ_ڈھیر_بلند_پتہ    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H
    #تعریف_کرو configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H    0
#اگر_ختم

/* The following event macros are embedded in the kernel API calls. */

#اگر_غیر_تعریف_شدہ trace_MOVED_TASK_مقصد_READY_STATE
    #تعریف_کرو trace_MOVED_TASK_مقصد_READY_STATE( pxTCB )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_POST_MOVED_TASK_مقصد_READY_STATE
    #تعریف_کرو trace_POST_MOVED_TASK_مقصد_READY_STATE( pxTCB )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_قطار_CREATE
    #تعریف_کرو trace_قطار_CREATE( px_New_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_قطار_CREATE_FAILED
    #تعریف_کرو trace_قطار_CREATE_FAILED( uc_قطار_Type )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceCREATE_MUTEX
    #تعریف_کرو traceCREATE_MUTEX( px_New_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceCREATE_MUTEX_FAILED
    #تعریف_کرو traceCREATE_MUTEX_FAILED()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceGIVE_MUTEX_RECURSIVE
    #تعریف_کرو traceGIVE_MUTEX_RECURSIVE( pxMutex )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceGIVE_MUTEX_RECURSIVE_FAILED
    #تعریف_کرو traceGIVE_MUTEX_RECURSIVE_FAILED( pxMutex )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceTAKE_MUTEX_RECURSIVE
    #تعریف_کرو traceTAKE_MUTEX_RECURSIVE( pxMutex )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceTAKE_MUTEX_RECURSIVE_FAILED
    #تعریف_کرو traceTAKE_MUTEX_RECURSIVE_FAILED( pxMutex )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceCREATE_COUNTING_SEMAPHORE
    #تعریف_کرو traceCREATE_COUNTING_SEMAPHORE()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceCREATE_COUNTING_SEMAPHORE_FAILED
    #تعریف_کرو traceCREATE_COUNTING_SEMAPHORE_FAILED()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_قطار_سیٹ_SEND
    #تعریف_کرو trace_قطار_سیٹ_SEND    trace_قطار_SEND
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_قطار_SEND
    #تعریف_کرو trace_قطار_SEND( px_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_قطار_SEND_FAILED
    #تعریف_کرو trace_قطار_SEND_FAILED( px_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_قطار_RECEIVE
    #تعریف_کرو trace_قطار_RECEIVE( px_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_قطار_PEEK
    #تعریف_کرو trace_قطار_PEEK( px_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_قطار_PEEK_FAILED
    #تعریف_کرو trace_قطار_PEEK_FAILED( px_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_قطار_PEEK_FROM_ISR
    #تعریف_کرو trace_قطار_PEEK_FROM_ISR( px_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_قطار_RECEIVE_FAILED
    #تعریف_کرو trace_قطار_RECEIVE_FAILED( px_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_قطار_SEND_FROM_ISR
    #تعریف_کرو trace_قطار_SEND_FROM_ISR( px_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_قطار_SEND_FROM_ISR_FAILED
    #تعریف_کرو trace_قطار_SEND_FROM_ISR_FAILED( px_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_قطار_RECEIVE_FROM_ISR
    #تعریف_کرو trace_قطار_RECEIVE_FROM_ISR( px_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_قطار_RECEIVE_FROM_ISR_FAILED
    #تعریف_کرو trace_قطار_RECEIVE_FROM_ISR_FAILED( px_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_قطار_PEEK_FROM_ISR_FAILED
    #تعریف_کرو trace_قطار_PEEK_FROM_ISR_FAILED( px_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_قطار_DELETE
    #تعریف_کرو trace_قطار_DELETE( px_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceTASK_CREATE
    #تعریف_کرو traceTASK_CREATE( pxNewTCB )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceTASK_CREATE_FAILED
    #تعریف_کرو traceTASK_CREATE_FAILED()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceTASK_DELETE
    #تعریف_کرو traceTASK_DELETE( px_Task_مقصد_Delete )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceTASK_DELAY_UNTIL
    #تعریف_کرو traceTASK_DELAY_UNTIL( x )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceTASK_DELAY
    #تعریف_کرو traceTASK_DELAY()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_TASK_ترجیح_سیٹ
    #تعریف_کرو trace_TASK_ترجیح_سیٹ( pxTask, ux_New_ترجیح )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceTASK_SUSPEND
    #تعریف_کرو traceTASK_SUSPEND( px_Task_مقصد_Suspend )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceTASK_RESUME
    #تعریف_کرو traceTASK_RESUME( px_Task_مقصد_Resume )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceTASK_RESUME_FROM_ISR
    #تعریف_کرو traceTASK_RESUME_FROM_ISR( px_Task_مقصد_Resume )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceTASK_INCREMENT_TICK
    #تعریف_کرو traceTASK_INCREMENT_TICK( x_Tick_گنتی )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceTIMER_CREATE
    #تعریف_کرو traceTIMER_CREATE( pxNewTimer )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceTIMER_CREATE_FAILED
    #تعریف_کرو traceTIMER_CREATE_FAILED()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceTIMER_COMMAND_SEND
    #تعریف_کرو traceTIMER_COMMAND_SEND( xTimer, x_پیغام_ID, x_پیغام_قدر_قدر, xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceTIMER_EXPIRED
    #تعریف_کرو traceTIMER_EXPIRED( pxTimer )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceTIMER_COMMAND_RECEIVED
    #تعریف_کرو traceTIMER_COMMAND_RECEIVED( pxTimer, x_پیغام_ID, x_پیغام_قدر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceMALLOC
    #تعریف_کرو traceMALLOC( pv_پتہ, ui_سائز )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceFREE
    #تعریف_کرو traceFREE( pv_پتہ, ui_سائز )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_EVENT_گروہ_CREATE
    #تعریف_کرو trace_EVENT_گروہ_CREATE( x_Event_گروہ )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_EVENT_گروہ_CREATE_FAILED
    #تعریف_کرو trace_EVENT_گروہ_CREATE_FAILED()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_EVENT_گروہ_SYNC_بلاک
    #تعریف_کرو trace_EVENT_گروہ_SYNC_بلاک( x_Event_گروہ, ux_بٹ_مقصد_سیٹ, ux_بٹ_مقصد_Wait_For )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_EVENT_گروہ_SYNC_انتہا
    #تعریف_کرو trace_EVENT_گروہ_SYNC_انتہا( x_Event_گروہ, ux_بٹ_مقصد_سیٹ, ux_بٹ_مقصد_Wait_For, xTimeoutOccurred )    ( خالی ) ( xTimeoutOccurred )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_EVENT_گروہ_WAIT_بٹ_بلاک
    #تعریف_کرو trace_EVENT_گروہ_WAIT_بٹ_بلاک( x_Event_گروہ, ux_بٹ_مقصد_Wait_For )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_EVENT_گروہ_WAIT_بٹ_انتہا
    #تعریف_کرو trace_EVENT_گروہ_WAIT_بٹ_انتہا( x_Event_گروہ, ux_بٹ_مقصد_Wait_For, xTimeoutOccurred )    ( خالی ) ( xTimeoutOccurred )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_EVENT_گروہ_CLEAR_بٹ
    #تعریف_کرو trace_EVENT_گروہ_CLEAR_بٹ( x_Event_گروہ, ux_بٹ_مقصد_Clear )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_EVENT_گروہ_CLEAR_بٹ_FROM_ISR
    #تعریف_کرو trace_EVENT_گروہ_CLEAR_بٹ_FROM_ISR( x_Event_گروہ, ux_بٹ_مقصد_Clear )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_EVENT_گروہ_سیٹ_بٹ
    #تعریف_کرو trace_EVENT_گروہ_سیٹ_بٹ( x_Event_گروہ, ux_بٹ_مقصد_سیٹ )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_EVENT_گروہ_سیٹ_بٹ_FROM_ISR
    #تعریف_کرو trace_EVENT_گروہ_سیٹ_بٹ_FROM_ISR( x_Event_گروہ, ux_بٹ_مقصد_سیٹ )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_EVENT_گروہ_DELETE
    #تعریف_کرو trace_EVENT_گروہ_DELETE( x_Event_گروہ )
#اگر_ختم

#اگر_غیر_تعریف_شدہ tracePEND_FUNC_CALL
    #تعریف_کرو tracePEND_FUNC_CALL( x_Function_مقصد_Pend, pvParameter1, ulParameter2, ret )
#اگر_ختم

#اگر_غیر_تعریف_شدہ tracePEND_FUNC_CALL_FROM_ISR
    #تعریف_کرو tracePEND_FUNC_CALL_FROM_ISR( x_Function_مقصد_Pend, pvParameter1, ulParameter2, ret )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_قطار_REGISTRY_جوڑو
    #تعریف_کرو trace_قطار_REGISTRY_جوڑو( x_قطار, pc_قطار_Name )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_TASK_NOTIFY_TAKE_بلاک
    #تعریف_کرو trace_TASK_NOTIFY_TAKE_بلاک( ux_اشاریہ_مقصد_Wait )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceTASK_NOTIFY_TAKE
    #تعریف_کرو traceTASK_NOTIFY_TAKE( ux_اشاریہ_مقصد_Wait )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_TASK_NOTIFY_WAIT_بلاک
    #تعریف_کرو trace_TASK_NOTIFY_WAIT_بلاک( ux_اشاریہ_مقصد_Wait )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceTASK_NOTIFY_WAIT
    #تعریف_کرو traceTASK_NOTIFY_WAIT( ux_اشاریہ_مقصد_Wait )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceTASK_NOTIFY
    #تعریف_کرو traceTASK_NOTIFY( ux_اشاریہ_مقصد_Notify )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceTASK_NOTIFY_FROM_ISR
    #تعریف_کرو traceTASK_NOTIFY_FROM_ISR( ux_اشاریہ_مقصد_Notify )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceTASK_NOTIFY_GIVE_FROM_ISR
    #تعریف_کرو traceTASK_NOTIFY_GIVE_FROM_ISR( ux_اشاریہ_مقصد_Notify )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_دھارا_بفر_CREATE_FAILED
    #تعریف_کرو trace_دھارا_بفر_CREATE_FAILED( x_Is_پیغام_بفر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_دھارا_بفر_CREATE_STATIC_FAILED
    #تعریف_کرو trace_دھارا_بفر_CREATE_STATIC_FAILED( xReturn, x_Is_پیغام_بفر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_دھارا_بفر_CREATE
    #تعریف_کرو trace_دھارا_بفر_CREATE( px_دھارا_بفر, x_Is_پیغام_بفر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_دھارا_بفر_DELETE
    #تعریف_کرو trace_دھارا_بفر_DELETE( x_دھارا_بفر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_دھارا_بفر_RESET
    #تعریف_کرو trace_دھارا_بفر_RESET( x_دھارا_بفر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_BLOCKING_ON_دھارا_بفر_SEND
    #تعریف_کرو trace_BLOCKING_ON_دھارا_بفر_SEND( x_دھارا_بفر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_دھارا_بفر_SEND
    #تعریف_کرو trace_دھارا_بفر_SEND( x_دھارا_بفر, x_بائٹیں_بھیجا )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_دھارا_بفر_SEND_FAILED
    #تعریف_کرو trace_دھارا_بفر_SEND_FAILED( x_دھارا_بفر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_دھارا_بفر_SEND_FROM_ISR
    #تعریف_کرو trace_دھارا_بفر_SEND_FROM_ISR( x_دھارا_بفر, x_بائٹیں_بھیجا )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_BLOCKING_ON_دھارا_بفر_RECEIVE
    #تعریف_کرو trace_BLOCKING_ON_دھارا_بفر_RECEIVE( x_دھارا_بفر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_دھارا_بفر_RECEIVE
    #تعریف_کرو trace_دھارا_بفر_RECEIVE( x_دھارا_بفر, x_Received_لمبائی )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_دھارا_بفر_RECEIVE_FAILED
    #تعریف_کرو trace_دھارا_بفر_RECEIVE_FAILED( x_دھارا_بفر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_دھارا_بفر_RECEIVE_FROM_ISR
    #تعریف_کرو trace_دھارا_بفر_RECEIVE_FROM_ISR( x_دھارا_بفر, x_Received_لمبائی )
#اگر_ختم

#اگر_غیر_تعریف_شدہ config_GENERATE_تسلسل_وقت_STATS
    #تعریف_کرو config_GENERATE_تسلسل_وقت_STATS    0
#اگر_ختم

#اگر ( config_GENERATE_تسلسل_وقت_STATS == 1 )

    #اگر_غیر_تعریف_شدہ پورٹ_CONFIGURE_TIMER_FOR_تسلسل_وقت_STATS
        #غلطی If config_GENERATE_تسلسل_وقت_STATS is defined then پورٹ_CONFIGURE_TIMER_FOR_تسلسل_وقت_STATS must also be defined.  پورٹ_CONFIGURE_TIMER_FOR_تسلسل_وقت_STATS should call a پورٹ layer function مقصد تیاری a peripheral timer/شمار_2 that can then be مستعمل as the تسلسل time شمار_2 time بنیاد.
    #اگر_ختم /* portCONFIGURE_TIMER_FOR_RUN_TIME_STATS */

    #اگر_غیر_تعریف_شدہ پورٹ_GET_تسلسل_وقت_شمار_قدر
        #اگر_غیر_تعریف_شدہ پورٹ_ALT_GET_تسلسل_وقت_شمار_قدر
            #غلطی If config_GENERATE_تسلسل_وقت_STATS is defined then either پورٹ_GET_تسلسل_وقت_شمار_قدر or پورٹ_ALT_GET_تسلسل_وقت_شمار_قدر must also be defined.  See the examples provided and the FreeRTOS web site برائے مزید information.
        #اگر_ختم /* portALT_GET_RUN_TIME_COUNTER_VALUE */
    #اگر_ختم /* portGET_RUN_TIME_COUNTER_VALUE */

#اگر_ختم /* configGENERATE_RUN_TIME_STATS */

#اگر_غیر_تعریف_شدہ پورٹ_CONFIGURE_TIMER_FOR_تسلسل_وقت_STATS
    #تعریف_کرو پورٹ_CONFIGURE_TIMER_FOR_تسلسل_وقت_STATS()
#اگر_ختم

#اگر_غیر_تعریف_شدہ configUSE_MALLOC_FAILED_HOOK
    #تعریف_کرو configUSE_MALLOC_FAILED_HOOK    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ پورٹ_PRIVILEGE_ثنائی_ہندسہ
    #تعریف_کرو پورٹ_PRIVILEGE_ثنائی_ہندسہ    ( ( UBaseType_t ) 0x00 )
#اگر_ختم

#اگر_غیر_تعریف_شدہ پورٹ_YIELD_WITHIN_API
    #تعریف_کرو پورٹ_YIELD_WITHIN_API    پورٹ_YIELD
#اگر_ختم

#اگر_غیر_تعریف_شدہ پورٹ_SUPPRESS_TICKS_AND_SLEEP
    #تعریف_کرو پورٹ_SUPPRESS_TICKS_AND_SLEEP( x_Expected_Idle_وقت )
#اگر_ختم

#اگر_غیر_تعریف_شدہ config_EXPECTED_IDLE_وقت_BEFORE_SLEEP
    #تعریف_کرو config_EXPECTED_IDLE_وقت_BEFORE_SLEEP    2
#اگر_ختم

#اگر config_EXPECTED_IDLE_وقت_BEFORE_SLEEP < 2
    #غلطی config_EXPECTED_IDLE_وقت_BEFORE_SLEEP must not be less than 2
#اگر_ختم

#اگر_غیر_تعریف_شدہ configUSE_TICKLESS_IDLE
    #تعریف_کرو configUSE_TICKLESS_IDLE    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ configPRE_SUPPRESS_TICKS_AND_SLEEP_PROCESSING
    #تعریف_کرو configPRE_SUPPRESS_TICKS_AND_SLEEP_PROCESSING( x )
#اگر_ختم

#اگر_غیر_تعریف_شدہ configPRE_SLEEP_PROCESSING
    #تعریف_کرو configPRE_SLEEP_PROCESSING( x )
#اگر_ختم

#اگر_غیر_تعریف_شدہ configPOST_SLEEP_PROCESSING
    #تعریف_کرو configPOST_SLEEP_PROCESSING( x )
#اگر_ختم

#اگر_غیر_تعریف_شدہ config_USE_قطار_SETS
    #تعریف_کرو config_USE_قطار_SETS    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ پورٹ_TASK_استعمال_کرتا_FLOATING_نقطہ
    #تعریف_کرو پورٹ_TASK_استعمال_کرتا_FLOATING_نقطہ()
#اگر_ختم

#اگر_غیر_تعریف_شدہ پورٹ_ALLOCATE_SECURE_CONTEXT
    #تعریف_کرو پورٹ_ALLOCATE_SECURE_CONTEXT( ul_Secure_ڈھیر_سائز )
#اگر_ختم

#اگر_غیر_تعریف_شدہ پورٹ_DONT_DISCARD
    #تعریف_کرو پورٹ_DONT_DISCARD
#اگر_ختم

#اگر_غیر_تعریف_شدہ config_USE_وقت_SLICING
    #تعریف_کرو config_USE_وقت_SLICING    1
#اگر_ختم

#اگر_غیر_تعریف_شدہ configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS
    #تعریف_کرو configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ configUSE_STATS_FORMATTING_FUNCTIONS
    #تعریف_کرو configUSE_STATS_FORMATTING_FUNCTIONS    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ پورٹ_ASSERT_IF_مداخلت_ترجیح_INVALID
    #تعریف_کرو پورٹ_ASSERT_IF_مداخلت_ترجیح_INVALID()
#اگر_ختم

#اگر_غیر_تعریف_شدہ configUSE_TRACE_FACILITY
    #تعریف_کرو configUSE_TRACE_FACILITY    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ mtCOVERAGE_TEST_MARKER
    #تعریف_کرو mtCOVERAGE_TEST_MARKER()
#اگر_ختم

#اگر_غیر_تعریف_شدہ mtCOVERAGE_TEST_DELAY
    #تعریف_کرو mtCOVERAGE_TEST_DELAY()
#اگر_ختم

#اگر_غیر_تعریف_شدہ پورٹ_ASSERT_IF_اندر_ISR
    #تعریف_کرو پورٹ_ASSERT_IF_اندر_ISR()
#اگر_ختم

#اگر_غیر_تعریف_شدہ config_USE_پورٹ_OPTIMISED_TASK_SELECTION
    #تعریف_کرو config_USE_پورٹ_OPTIMISED_TASK_SELECTION    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ config_APPLICATION_ALLOCATED_ڈھیر
    #تعریف_کرو config_APPLICATION_ALLOCATED_ڈھیر    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ configUSE_TASK_NOTIFICATIONS
    #تعریف_کرو configUSE_TASK_NOTIFICATIONS    1
#اگر_ختم

#اگر_غیر_تعریف_شدہ configTASK_NOTIFICATION_ARRAY_ENTRIES
    #تعریف_کرو configTASK_NOTIFICATION_ARRAY_ENTRIES    1
#اگر_ختم

#اگر configTASK_NOTIFICATION_ARRAY_ENTRIES < 1
    #غلطی configTASK_NOTIFICATION_ARRAY_ENTRIES must be at کم_سے_کم 1
#اگر_ختم

#اگر_غیر_تعریف_شدہ configUSE_POSIX_ERRNO
    #تعریف_کرو configUSE_POSIX_ERRNO    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ configUSE_SB_COMPLETED_CALLBACK

/* By default per-instance callbacks are not enabled for stream buffer or message buffer. */
    #تعریف_کرو configUSE_SB_COMPLETED_CALLBACK    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ پورٹ_TICK_TYPE_IS_ATOMIC
    #تعریف_کرو پورٹ_TICK_TYPE_IS_ATOMIC    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ config_SUPPORT_STATIC_تخصیص
    /* Defaults to 0 for backward compatibility. */
    #تعریف_کرو config_SUPPORT_STATIC_تخصیص    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ config_SUPPORT_DYNAMIC_تخصیص
    /* Defaults to 1 for backward compatibility. */
    #تعریف_کرو config_SUPPORT_DYNAMIC_تخصیص    1
#اگر_ختم

#اگر ( ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 ) && ( config_SUPPORT_DYNAMIC_تخصیص != 1 ) )
    #غلطی configUSE_STATS_FORMATTING_FUNCTIONS cannot be مستعمل without dynamic تخصیص, but config_SUPPORT_DYNAMIC_تخصیص is not سیٹ مقصد 1.
#اگر_ختم

#اگر ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 )
    #اگر ( ( configUSE_TRACE_FACILITY != 1 ) && ( config_GENERATE_تسلسل_وقت_STATS != 1 ) )
        #غلطی configUSE_STATS_FORMATTING_FUNCTIONS is 1 but the functions it enables are not مستعمل because neither configUSE_TRACE_FACILITY or config_GENERATE_تسلسل_وقت_STATS are 1.  سیٹ_2 configUSE_STATS_FORMATTING_FUNCTIONS مقصد 0 اندر FreeRTOSConfig.h.
    #اگر_ختم
#اگر_ختم

#اگر_غیر_تعریف_شدہ config_ڈھیر_گہرائی_TYPE

/* Defaults to uint16_t for backward compatibility, but can be overridden
 * in FreeRTOSConfig.h if uint16_t is too restrictive. */
    #تعریف_کرو config_ڈھیر_گہرائی_TYPE    uint16_t
#اگر_ختم

#اگر_غیر_تعریف_شدہ config_تسلسل_وقت_شمار_TYPE

/* Defaults to uint32_t for backward compatibility, but can be overridden in
 * FreeRTOSConfig.h if uint32_t is too restrictive. */

    #تعریف_کرو config_تسلسل_وقت_شمار_TYPE    uint32_t
#اگر_ختم

#اگر_غیر_تعریف_شدہ config_پیغام_بفر_لمبائی_TYPE

/* Defaults to size_t for backward compatibility, but can be overridden
 * in FreeRTOSConfig.h if lengths will always be less than the number of bytes
 * in a size_t. */
    #تعریف_کرو config_پیغام_بفر_لمبائی_TYPE    size_t
#اگر_ختم

/* Sanity check the configuration. */
#اگر ( ( config_SUPPORT_STATIC_تخصیص == 0 ) && ( config_SUPPORT_DYNAMIC_تخصیص == 0 ) )
    #غلطی config_SUPPORT_STATIC_تخصیص and config_SUPPORT_DYNAMIC_تخصیص cannot both be 0, but can both be 1.
#اگر_ختم

#اگر ( ( configUSE_RECURSIVE_MUTEXES == 1 ) && ( configUSE_MUTEXES != 1 ) )
    #غلطی configUSE_MUTEXES must be سیٹ مقصد 1 مقصد use recursive mutexes
#اگر_ختم

#اگر ( ( config_نمبر_کا_CORES > 1 ) && ( config_USE_پورٹ_OPTIMISED_TASK_SELECTION != 0 ) )
    #غلطی config_USE_پورٹ_OPTIMISED_TASK_SELECTION is not supported اندر SMP
#اگر_ختم

#اگر_غیر_تعریف_شدہ config_INITIAL_TICK_گنتی
    #تعریف_کرو config_INITIAL_TICK_گنتی    0
#اگر_ختم

#اگر ( پورٹ_TICK_TYPE_IS_ATOMIC == 0 )

/* Either variables of tick type cannot be read atomically, or
 * portTICK_TYPE_IS_ATOMIC was not set - map the critical sections used when
 * the tick count is returned to the standard critical section macros. */
    #تعریف_کرو پورٹ_TICK_TYPE_ENTER_CRITICAL()                      پورٹ_ENTER_CRITICAL()
    #تعریف_کرو پورٹ_TICK_TYPE_EXIT_CRITICAL()                       پورٹ_EXIT_CRITICAL()
    #تعریف_کرو پورٹ_TICK_TYPE_سیٹ_مداخلت_نقاب_FROM_ISR()         پورٹ_سیٹ_مداخلت_نقاب_FROM_ISR()
    #تعریف_کرو پورٹ_TICK_TYPE_CLEAR_مداخلت_نقاب_FROM_ISR( x )    پورٹ_CLEAR_مداخلت_نقاب_FROM_ISR( ( x ) )
#ورنہ

/* The tick type can be read atomically, so critical sections used when the
 * tick count is returned can be defined away. */
    #تعریف_کرو پورٹ_TICK_TYPE_ENTER_CRITICAL()
    #تعریف_کرو پورٹ_TICK_TYPE_EXIT_CRITICAL()
    #تعریف_کرو پورٹ_TICK_TYPE_سیٹ_مداخلت_نقاب_FROM_ISR()         0
    #تعریف_کرو پورٹ_TICK_TYPE_CLEAR_مداخلت_نقاب_FROM_ISR( x )    ( خالی ) ( x )
#اگر_ختم /* if ( portTICK_TYPE_IS_ATOMIC == 0 ) */

/* Definitions to allow backward compatibility with FreeRTOS versions prior to
 * V8 if desired. */
#اگر_غیر_تعریف_شدہ configENABLE_BACKWARD_COMPATIBILITY
    #تعریف_کرو configENABLE_BACKWARD_COMPATIBILITY    1
#اگر_ختم

#اگر_غیر_تعریف_شدہ configPRINTF

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
    #تعریف_کرو configPRINTF( X )
#اگر_ختم

#اگر_غیر_تعریف_شدہ configMAX

/* The application writer has not provided their own MAX macro, so define
 * the following generic implementation. */
    #تعریف_کرو configMAX( a, b )    ( ( ( a ) > ( b ) ) ? ( a ) : ( b ) )
#اگر_ختم

#اگر_غیر_تعریف_شدہ configMIN

/* The application writer has not provided their own MIN macro, so define
 * the following generic implementation. */
    #تعریف_کرو configMIN( a, b )    ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )
#اگر_ختم

#اگر configENABLE_BACKWARD_COMPATIBILITY == 1
    #تعریف_کرو eTaskStateGet                 eTaskGetState
    #تعریف_کرو پورٹ_Tick_Type                  TickType_t
    #تعریف_کرو xTaskHandle                   TaskHandle_t
    #تعریف_کرو x_قطار_Handle                  قطار_Handle_t
    #تعریف_کرو xSemaphoreHandle              SemaphoreHandle_t
    #تعریف_کرو x_قطار_سیٹ_Handle               قطار_سیٹ_Handle_t
    #تعریف_کرو x_قطار_سیٹ_Member_Handle         قطار_سیٹ_Member_Handle_t
    #تعریف_کرو x_وقت_برآمد_Type                  وقت_برآمد_t
    #تعریف_کرو x_حافظہ_Region                 حافظہ_Region_t
    #تعریف_کرو xTaskParameters               TaskParameters_t
    #تعریف_کرو x_Task_حالت_Type               Task_حالت_t
    #تعریف_کرو xTimerHandle                  TimerHandle_t
    #تعریف_کرو xCoRoutineHandle              CoRoutineHandle_t
    #تعریف_کرو pd_TASK_HOOK_رمز              TaskHookFunction_t
    #تعریف_کرو پورٹ_TICK_شرح_MS              پورٹ_TICK_PERIOD_MS
    #تعریف_کرو pcTaskGetTaskName             pcTaskGetName
    #تعریف_کرو pcTimerGetTimerName           pcTimerGetName
    #تعریف_کرو pc_قطار_Get_قطار_Name           pc_قطار_Get_Name
    #تعریف_کرو vTaskGetTaskInfo              vTaskGetInfo
    #تعریف_کرو x_Task_Get_Idle_تسلسل_وقت_شمار    ul_Task_Get_Idle_تسلسل_وقت_شمار

/* Backward compatibility within the scheduler code only - these definitions
 * are not really required but are included for completeness. */
    #تعریف_کرو tmrTIMER_CALLBACK             TimerCallbackFunction_t
    #تعریف_کرو pd_TASK_رمز                   TaskFunction_t
    #تعریف_کرو x_List_شے                     List_شے_t
    #تعریف_کرو xList                         List_t

/* For libraries that break the list data hiding, and access list structure
 * members directly (which is not supposed to be done). */
    #تعریف_کرو pxContainer                   pvContainer
#اگر_ختم /* configENABLE_BACKWARD_COMPATIBILITY */

#اگر ( configUSE_ALTERNATIVE_API != 0 )
    #غلطی The alternative API was deprecated some time ago, and was removed اندر FreeRTOS V9.0 0
#اگر_ختم

/* Set configUSE_TASK_FPU_SUPPORT to 0 to omit floating point support even
 * if floating point hardware is otherwise supported by the FreeRTOS port in use.
 * This constant is not supported by all FreeRTOS ports that include floating
 * point support. */
#اگر_غیر_تعریف_شدہ configUSE_TASK_FPU_SUPPORT
    #تعریف_کرو configUSE_TASK_FPU_SUPPORT    1
#اگر_ختم

/* Set configENABLE_MPU to 1 to enable MPU support and 0 to disable it. This is
 * currently used in ARMv8M ports. */
#اگر_غیر_تعریف_شدہ configENABLE_MPU
    #تعریف_کرو configENABLE_MPU    0
#اگر_ختم

/* Set configENABLE_FPU to 1 to enable FPU support and 0 to disable it. This is
 * currently used in ARMv8M ports. */
#اگر_غیر_تعریف_شدہ configENABLE_FPU
    #تعریف_کرو configENABLE_FPU    1
#اگر_ختم

/* Set configENABLE_MVE to 1 to enable MVE support and 0 to disable it. This is
 * currently used in ARMv8M ports. */
#اگر_غیر_تعریف_شدہ configENABLE_MVE
    #تعریف_کرو configENABLE_MVE    0
#اگر_ختم

/* Set configENABLE_TRUSTZONE to 1 enable TrustZone support and 0 to disable it.
 * This is currently used in ARMv8M ports. */
#اگر_غیر_تعریف_شدہ configENABLE_TRUSTZONE
    #تعریف_کرو configENABLE_TRUSTZONE    1
#اگر_ختم

/* Set configRUN_FREERTOS_SECURE_ONLY to 1 to run the FreeRTOS ARMv8M port on
 * the Secure Side only. */
#اگر_غیر_تعریف_شدہ config_تسلسل_FREERTOS_SECURE_ONLY
    #تعریف_کرو config_تسلسل_FREERTOS_SECURE_ONLY    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ config_تسلسل_ADDITIONAL_TESTS
    #تعریف_کرو config_تسلسل_ADDITIONAL_TESTS    0
#اگر_ختم


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
#تعریف_کرو tsk_STATIC_AND_DYNAMIC_تخصیص_POSSIBLE                                                                                     \
    ( ( ( پورٹ_USING_MPU_WRAPPERS == 0 ) && ( config_SUPPORT_DYNAMIC_تخصیص == 1 ) && ( config_SUPPORT_STATIC_تخصیص == 1 ) ) || \
      ( ( پورٹ_USING_MPU_WRAPPERS == 1 ) && ( config_SUPPORT_DYNAMIC_تخصیص == 1 ) ) )

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
ساخت x_STATIC_LIST_شے
{
    #اگر ( config_USE_LIST_ڈیٹا_INTEGRITY_CHECK_بائٹیں == 1 )
        TickType_t xDummy1;
    #اگر_ختم
    TickType_t xDummy2;
    خالی * pvDummy3[ 4 ];
    #اگر ( config_USE_LIST_ڈیٹا_INTEGRITY_CHECK_بائٹیں == 1 )
        TickType_t xDummy4;
    #اگر_ختم
};
قسمتعریف ساخت x_STATIC_LIST_شے Static_List_شے_t;

#اگر ( config_USE_MINI_LIST_شے == 1 )
    /* See the comments above the struct xSTATIC_LIST_ITEM definition. */
    ساخت x_STATIC_MINI_LIST_شے
    {
        #اگر ( config_USE_LIST_ڈیٹا_INTEGRITY_CHECK_بائٹیں == 1 )
            TickType_t xDummy1;
        #اگر_ختم
        TickType_t xDummy2;
        خالی * pvDummy3[ 2 ];
    };
    قسمتعریف ساخت x_STATIC_MINI_LIST_شے Static_Mini_List_شے_t;
#ورنہ /* if ( configUSE_MINI_LIST_ITEM == 1 ) */
    قسمتعریف ساخت x_STATIC_LIST_شے      Static_Mini_List_شے_t;
#اگر_ختم /* if ( configUSE_MINI_LIST_ITEM == 1 ) */

/* See the comments above the struct xSTATIC_LIST_ITEM definition. */
قسمتعریف ساخت xSTATIC_LIST
{
    #اگر ( config_USE_LIST_ڈیٹا_INTEGRITY_CHECK_بائٹیں == 1 )
        TickType_t xDummy1;
    #اگر_ختم
    UBaseType_t uxDummy2;
    خالی * pvDummy3;
    Static_Mini_List_شے_t xDummy4;
    #اگر ( config_USE_LIST_ڈیٹا_INTEGRITY_CHECK_بائٹیں == 1 )
        TickType_t xDummy5;
    #اگر_ختم
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
قسمتعریف ساخت xSTATIC_TCB
{
    خالی * pxDummy1;
    #اگر ( پورٹ_USING_MPU_WRAPPERS == 1 )
        xMPU_SETTINGS xDummy2;
    #اگر_ختم
    Static_List_شے_t xDummy3[ 2 ];
    UBaseType_t uxDummy5;
    خالی * pxDummy6;
    uint8_t ucDummy7[ configMAX_TASK_NAME_LEN ];
    #اگر ( config_نمبر_کا_CORES > 1 )
        بنیاد_Type_t xDummyCoreID;
    #اگر_ختم /* configNUMBER_OF_CORES > 1 */
    #اگر ( ( پورٹ_ڈھیر_GROWTH > 0 ) || ( config_ریکارڈ_ڈھیر_بلند_پتہ == 1 ) )
        خالی * pxDummy8;
    #اگر_ختم
    #اگر ( پورٹ_CRITICAL_NESTING_اندر_TCB == 1 )
        UBaseType_t uxDummy9;
    #اگر_ختم
    #اگر ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy10[ 2 ];
    #اگر_ختم
    #اگر ( configUSE_MUTEXES == 1 )
        UBaseType_t uxDummy12[ 2 ];
    #اگر_ختم
    #اگر ( config_USE_APPLICATION_TASK_لیبل == 1 )
        خالی * pxDummy14;
    #اگر_ختم
    #اگر ( config_عدد_THREAD_LOCAL_STORAGE_اشارے > 0 )
        خالی * pvDummy15[ config_عدد_THREAD_LOCAL_STORAGE_اشارے ];
    #اگر_ختم
    #اگر ( config_GENERATE_تسلسل_وقت_STATS == 1 )
        config_تسلسل_وقت_شمار_TYPE ulDummy16;
    #اگر_ختم
    #اگر ( ( configUSE_NEWLIB_REENTRANT == 1 ) || ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 ) )
        config_TLS_بلاک_TYPE xDummy17;
    #اگر_ختم
    #اگر ( configUSE_TASK_NOTIFICATIONS == 1 )
        uint32_t ulDummy18[ configTASK_NOTIFICATION_ARRAY_ENTRIES ];
        uint8_t ucDummy19[ configTASK_NOTIFICATION_ARRAY_ENTRIES ];
    #اگر_ختم
    #اگر ( tsk_STATIC_AND_DYNAMIC_تخصیص_POSSIBLE != 0 )
        uint8_t uxDummy20;
    #اگر_ختم

    #اگر ( INCLUDE_xTaskAbortDelay == 1 )
        uint8_t ucDummy21;
    #اگر_ختم
    #اگر ( configUSE_POSIX_ERRNO == 1 )
        صحیح iDummy22;
    #اگر_ختم
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
قسمتعریف ساخت x_STATIC_قطار
{
    خالی * pvDummy1[ 3 ];

    اتحاد
    {
        خالی * pvDummy2;
        UBaseType_t uxDummy2;
    } u;

    StaticList_t xDummy3[ 2 ];
    UBaseType_t uxDummy4[ 3 ];
    uint8_t ucDummy5[ 2 ];

    #اگر ( ( config_SUPPORT_STATIC_تخصیص == 1 ) && ( config_SUPPORT_DYNAMIC_تخصیص == 1 ) )
        uint8_t ucDummy6;
    #اگر_ختم

    #اگر ( config_USE_قطار_SETS == 1 )
        خالی * pvDummy7;
    #اگر_ختم

    #اگر ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy8;
        uint8_t ucDummy9;
    #اگر_ختم
    پورٹ_MUX_TYPE x_Dummy_قطار_تالا;
} Static_قطار_t;
قسمتعریف Static_قطار_t StaticSemaphore_t;

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
قسمتعریف ساخت x_STATIC_EVENT_گروہ
{
    TickType_t xDummy1;
    StaticList_t xDummy2;

    #اگر ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy3;
    #اگر_ختم

    #اگر ( ( config_SUPPORT_STATIC_تخصیص == 1 ) && ( config_SUPPORT_DYNAMIC_تخصیص == 1 ) )
        uint8_t ucDummy4;
    #اگر_ختم
    پورٹ_MUX_TYPE x_Dummy_Event_گروہ_تالا;
} Static_Event_گروہ_t;

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
قسمتعریف ساخت xSTATIC_TIMER
{
    خالی * pvDummy1;
    Static_List_شے_t xDummy2;
    TickType_t xDummy3;
    خالی * pvDummy5;
    TaskFunction_t pvDummy6;
    #اگر ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy7;
    #اگر_ختم
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
قسمتعریف ساخت x_STATIC_دھارا_بفر
{
    size_t uxDummy1[ 4 ];
    خالی * pvDummy2[ 3 ];
    uint8_t ucDummy3;
    #اگر ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy4;
    #اگر_ختم
    #اگر ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        خالی * pvDummy5[ 2 ];
    #اگر_ختم
    پورٹ_MUX_TYPE x_Dummy_دھارا_بفر_تالا;
} Static_دھارا_بفر_t;

/* Message buffers are built on stream buffers. */
قسمتعریف Static_دھارا_بفر_t Static_پیغام_بفر_t;

/* *INDENT-OFF* */
#اگر_تعریف_شدہ __cplusplus
    }
#اگر_ختم
/* *INDENT-ON* */

/*-----------------------------------------------------------
* IDF Compatibility
*----------------------------------------------------------*/

#اگر_تعریف_شدہ ESP_PLATFORM

/* *INDENT-OFF* */
#اگر_تعریف_شدہ __cplusplus
    خارجی "C" {
#اگر_ختم
/* *INDENT-ON* */

/*
 * Default values for trace macros added by ESP-IDF and are not part of Vanilla FreeRTOS
 */

    #اگر_غیر_تعریف_شدہ trace_ISR_EXIT_مقصد_SCHEDULER
        #تعریف_کرو trace_ISR_EXIT_مقصد_SCHEDULER()
    #اگر_ختم

    #اگر_غیر_تعریف_شدہ traceISR_EXIT
        #تعریف_کرو traceISR_EXIT()
    #اگر_ختم

    #اگر_غیر_تعریف_شدہ traceISR_ENTER
        #تعریف_کرو traceISR_ENTER( _n_ )
    #اگر_ختم

    #اگر_غیر_تعریف_شدہ trace_قطار_SEMAPHORE_RECEIVE
        #تعریف_کرو trace_قطار_SEMAPHORE_RECEIVE( px_قطار )
    #اگر_ختم

    #اگر_غیر_تعریف_شدہ trace_قطار_GIVE_FROM_ISR
        #تعریف_کرو trace_قطار_GIVE_FROM_ISR( px_قطار )
    #اگر_ختم

    #اگر_غیر_تعریف_شدہ trace_قطار_GIVE_FROM_ISR_FAILED
        #تعریف_کرو trace_قطار_GIVE_FROM_ISR_FAILED( px_قطار )
    #اگر_ختم

/* *INDENT-OFF* */
#اگر_تعریف_شدہ __cplusplus
    }
#اگر_ختم
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
    #شامل "freertos/idf_additions.h"

#اگر_ختم /* ESP_PLATFORM */

#اگر_ختم /* INC_FREERTOS_H */
