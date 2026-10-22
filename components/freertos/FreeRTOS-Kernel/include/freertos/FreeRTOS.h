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

#جے_بنا_تعریف INC_FREERTOS_H
#تعریف_کرو INC_FREERTOS_H

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
#جے_تعریف_شدہ __cplusplus
    خارجي "C" {
#جے_انت
/* *INDENT-ON* */

/* Application specific configuration options. */
#ضمن "FreeRTOSConfig.h"

/* Basic FreeRTOS definitions. */
#ضمن "projdefs.h"

/* Definitions specific to the port being used. */
#ضمن "portable.h"

/* Must be defaulted before configUSE_NEWLIB_REENTRANT is used below. */
#جے_بنا_تعریف configUSE_NEWLIB_REENTRANT
    #تعریف_کرو configUSE_NEWLIB_REENTRANT    0
#جے_انت

/* Required if struct _reent is used. */
#إذا ( configUSE_NEWLIB_REENTRANT == 1 )

/* Note Newlib support has been included by popular demand, but is not
 * used by the FreeRTOS maintainers themselves.  FreeRTOS is not
 * responsible for resulting newlib operation.  User must be familiar with
 * newlib and must provide system-wide implementations of the necessary
 * stubs. Be warned that (at the time of writing) the current newlib design
 * implements a system-wide malloc() that must be provided with locks.
 *
 * See the third party link http://www.nadler.com/embedded/newlibAndFreeRTOS.html
 * for additional information. */
    #ضمن <reent.h>

    #تعریف_کرو configUSE_C_RUNTIME_TLS_SUPPORT    1

    #جے_بنا_تعریف config_TLS_بلاک_TYPE
        #تعریف_کرو config_TLS_بلاک_TYPE           بنية _reent
    #جے_انت

    #جے_بنا_تعریف config_INIT_TLS_بلاک
        #تعریف_کرو config_INIT_TLS_بلاک( xTLSBlock )    _REENT_INIT_PTR( &( xTLSBlock ) )
    #جے_انت

    #جے_بنا_تعریف config_سیٹ_TLS_بلاک
        #تعریف_کرو config_سیٹ_TLS_بلاک( xTLSBlock )    _impure_ptr = &( xTLSBlock )
    #جے_انت

    #جے_بنا_تعریف config_DEINIT_TLS_بلاک
        #تعریف_کرو config_DEINIT_TLS_بلاک( xTLSBlock )    _reclaim_reent( &( xTLSBlock ) )
    #جے_انت
#جے_انت /* if ( configUSE_NEWLIB_REENTRANT == 1 ) */

#جے_بنا_تعریف configUSE_C_RUNTIME_TLS_SUPPORT
    #تعریف_کرو configUSE_C_RUNTIME_TLS_SUPPORT    0
#جے_انت

#إذا ( ( configUSE_NEWLIB_REENTRANT == 0 ) && ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 ) )

    #جے_بنا_تعریف config_TLS_بلاک_TYPE
        #غلطی Missing definition:  config_TLS_بلاک_TYPE must be defined وچ FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is سیٹ منزل 1.
    #جے_انت

    #جے_بنا_تعریف config_INIT_TLS_بلاک
        #غلطی Missing definition:  config_INIT_TLS_بلاک must be defined وچ FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is سیٹ منزل 1.
    #جے_انت

    #جے_بنا_تعریف config_سیٹ_TLS_بلاک
        #غلطی Missing definition:  config_سیٹ_TLS_بلاک must be defined وچ FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is سیٹ منزل 1.
    #جے_انت

    #جے_بنا_تعریف config_DEINIT_TLS_بلاک
        #غلطی Missing definition:  config_DEINIT_TLS_بلاک must be defined وچ FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is سیٹ منزل 1.
    #جے_انت
#جے_انت /* if ( ( configUSE_NEWLIB_REENTRANT == 0 ) && ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 ) ) */

/*
 * Check all the required application specific macros have been defined.
 * These macros are application specific and (as downloaded) are defined
 * within FreeRTOSConfig.h.
 */

#جے_بنا_تعریف config_نمبر_دا_CORES
    #غلطی Missing definition:  config_نمبر_دا_CORES must be defined وچ FreeRTOSConfig.h
#جے_انت

#إذا ( config_نمبر_دا_CORES > 1 )
    #جے_بنا_تعریف پورٹ_GET_CORE_ID
        #غلطی "Missing definition: portGET_CORE_ID() must be defined if in portmacro.h if configNUMBER_OF_CORES > 1"
    #جے_انت
    #جے_بنا_تعریف پورٹ_YIELD_CORE
        #غلطی "Missing definition: portYIELD_CORE() must be defined if in portmacro.h if configNUMBER_OF_CORES > 1"
    #جے_انت
#وإلاإذا ( config_نمبر_دا_CORES == 1 )
    #تعریف_رد پورٹ_GET_CORE_ID
    #تعریف_کرو پورٹ_GET_CORE_ID()    0
#وإلا
    #غلطی config_نمبر_دا_CORES must be defined منزل either 1 or > 1.
#جے_انت /* if ( configNUMBER_OF_CORES > 1 ) */

#جے_بنا_تعریف config_اقل_ڈھیر_ناپ
    #غلطی Missing definition:  config_اقل_ڈھیر_ناپ must be defined وچ FreeRTOSConfig.h.  config_اقل_ڈھیر_ناپ مقرر_کردا the ناپ (وچ الفاظ) دا the ڈھیر allocated منزل the idle task.  Refer منزل the demo project provided لكل your پورٹ لكل a suitable قدر.
#جے_انت

#جے_بنا_تعریف configMAX_PRIORITIES
    #غلطی Missing definition:  configMAX_PRIORITIES must be defined وچ FreeRTOSConfig.h.  See the Configuration section دا the FreeRTOS API documentation لكل details.
#جے_انت

#إذا configMAX_PRIORITIES < 1
    #غلطی configMAX_PRIORITIES must be defined منزل be greater than or equal منزل 1.
#جے_انت

#جے_بنا_تعریف configUSE_PREEMPTION
    #غلطی Missing definition:  configUSE_PREEMPTION must be defined وچ FreeRTOSConfig.h as either 1 or 0.  See the Configuration section دا the FreeRTOS API documentation لكل details.
#جے_انت

#جے_بنا_تعریف configUSE_IDLE_HOOK
    #غلطی Missing definition:  configUSE_IDLE_HOOK must be defined وچ FreeRTOSConfig.h as either 1 or 0.  See the Configuration section دا the FreeRTOS API documentation لكل details.
#جے_انت

#جے_بنا_تعریف configUSE_TICK_HOOK
    #غلطی Missing definition:  configUSE_TICK_HOOK must be defined وچ FreeRTOSConfig.h as either 1 or 0.  See the Configuration section دا the FreeRTOS API documentation لكل details.
#جے_انت

#جے_بنا_تعریف config_USE_16_ثنائی_ہندسہ_TICKS
    #غلطی Missing definition:  config_USE_16_ثنائی_ہندسہ_TICKS must be defined وچ FreeRTOSConfig.h as either 1 or 0.  See the Configuration section دا the FreeRTOS API documentation لكل details.
#جے_انت

#جے_بنا_تعریف configUSE_CO_ROUTINES
    #تعریف_کرو configUSE_CO_ROUTINES    0
#جے_انت

#جے_بنا_تعریف INCLUDE_v_Task_ترجیح_سیٹ
    #تعریف_کرو INCLUDE_v_Task_ترجیح_سیٹ    0
#جے_انت

#جے_بنا_تعریف INCLUDE_ux_Task_ترجیح_Get
    #تعریف_کرو INCLUDE_ux_Task_ترجیح_Get    0
#جے_انت

#جے_بنا_تعریف INCLUDE_vTaskDelete
    #تعریف_کرو INCLUDE_vTaskDelete    0
#جے_انت

#جے_بنا_تعریف INCLUDE_vTaskSuspend
    #تعریف_کرو INCLUDE_vTaskSuspend    0
#جے_انت

#جے_تعریف_شدہ INCLUDE_xTaskDelayUntil
    #جے_تعریف_شدہ INCLUDE_vTaskDelayUntil

/* INCLUDE_vTaskDelayUntil was replaced by INCLUDE_xTaskDelayUntil.  Backward
 * compatibility is maintained if only one or the other is defined, but
 * there is a conflict if both are defined. */
        #غلطی INCLUDE_vTaskDelayUntil and INCLUDE_xTaskDelayUntil are both defined.  INCLUDE_vTaskDelayUntil is no longer required and should be removed
    #جے_انت
#جے_انت

#جے_بنا_تعریف INCLUDE_xTaskDelayUntil
    #جے_تعریف_شدہ INCLUDE_vTaskDelayUntil

/* If INCLUDE_vTaskDelayUntil is set but INCLUDE_xTaskDelayUntil is not then
 * the project's FreeRTOSConfig.h probably pre-dates the introduction of
 * xTaskDelayUntil and setting INCLUDE_xTaskDelayUntil to whatever
 * INCLUDE_vTaskDelayUntil is set to will ensure backward compatibility.
 */
        #تعریف_کرو INCLUDE_xTaskDelayUntil    INCLUDE_vTaskDelayUntil
    #جے_انت
#جے_انت

#جے_بنا_تعریف INCLUDE_xTaskDelayUntil
    #تعریف_کرو INCLUDE_xTaskDelayUntil    0
#جے_انت

#جے_بنا_تعریف INCLUDE_vTaskDelay
    #تعریف_کرو INCLUDE_vTaskDelay    0
#جے_انت

#جے_بنا_تعریف INCLUDE_xTaskGetIdleTaskHandle
    #تعریف_کرو INCLUDE_xTaskGetIdleTaskHandle    0
#جے_انت

#جے_بنا_تعریف INCLUDE_xTaskAbortDelay
    #تعریف_کرو INCLUDE_xTaskAbortDelay    0
#جے_انت

#جے_بنا_تعریف INCLUDE_x_قطار_Get_Mutex_Holder
    #تعریف_کرو INCLUDE_x_قطار_Get_Mutex_Holder    0
#جے_انت

#جے_بنا_تعریف INCLUDE_xSemaphoreGetMutexHolder
    #تعریف_کرو INCLUDE_xSemaphoreGetMutexHolder    INCLUDE_x_قطار_Get_Mutex_Holder
#جے_انت

#جے_بنا_تعریف INCLUDE_xTaskGetHandle
    #تعریف_کرو INCLUDE_xTaskGetHandle    0
#جے_انت

#جے_بنا_تعریف INCLUDE_ux_Task_Get_ڈھیر_بلند_Water_Mark
    #تعریف_کرو INCLUDE_ux_Task_Get_ڈھیر_بلند_Water_Mark    0
#جے_انت

#جے_بنا_تعریف INCLUDE_ux_Task_Get_ڈھیر_بلند_Water_Mark2
    #تعریف_کرو INCLUDE_ux_Task_Get_ڈھیر_بلند_Water_Mark2    0
#جے_انت

#جے_بنا_تعریف INCLUDE_eTaskGetState
    #تعریف_کرو INCLUDE_eTaskGetState    0
#جے_انت

#جے_بنا_تعریف INCLUDE_xTaskResumeFromISR
    #تعریف_کرو INCLUDE_xTaskResumeFromISR    1
#جے_انت

#جے_بنا_تعریف INCLUDE_xTimerPendFunctionCall
    #تعریف_کرو INCLUDE_xTimerPendFunctionCall    0
#جے_انت

#جے_بنا_تعریف INCLUDE_xTaskGetSchedulerState
    #تعریف_کرو INCLUDE_xTaskGetSchedulerState    0
#جے_انت

#جے_بنا_تعریف INCLUDE_xTaskGetCurrentTaskHandle
    #تعریف_کرو INCLUDE_xTaskGetCurrentTaskHandle    1
#جے_انت

#إذا configUSE_CO_ROUTINES != 0
    #جے_بنا_تعریف configMAX_CO_ROUTINE_PRIORITIES
        #غلطی configMAX_CO_ROUTINE_PRIORITIES must be greater than or equal منزل 1.
    #جے_انت
#جے_انت

#جے_بنا_تعریف configUSE_DAEMON_TASK_STARTUP_HOOK
    #تعریف_کرو configUSE_DAEMON_TASK_STARTUP_HOOK    0
#جے_انت

#جے_بنا_تعریف config_USE_APPLICATION_TASK_ٹیگ
    #تعریف_کرو config_USE_APPLICATION_TASK_ٹیگ    0
#جے_انت

#جے_بنا_تعریف config_عدد_THREAD_LOCAL_STORAGE_سوچکاں
    #تعریف_کرو config_عدد_THREAD_LOCAL_STORAGE_سوچکاں    0
#جے_انت

#جے_بنا_تعریف configUSE_RECURSIVE_MUTEXES
    #تعریف_کرو configUSE_RECURSIVE_MUTEXES    0
#جے_انت

#جے_بنا_تعریف configUSE_MUTEXES
    #تعریف_کرو configUSE_MUTEXES    0
#جے_انت

#جے_بنا_تعریف configUSE_TIMERS
    #تعریف_کرو configUSE_TIMERS    0
#جے_انت

#جے_بنا_تعریف configUSE_COUNTING_SEMAPHORES
    #تعریف_کرو configUSE_COUNTING_SEMAPHORES    0
#جے_انت

#جے_بنا_تعریف configUSE_ALTERNATIVE_API
    #تعریف_کرو configUSE_ALTERNATIVE_API    0
#جے_انت

#جے_بنا_تعریف پورٹ_CRITICAL_NESTING_وچ_TCB
    #تعریف_کرو پورٹ_CRITICAL_NESTING_وچ_TCB    0
#جے_انت

#جے_بنا_تعریف configMAX_TASK_NAME_LEN
    #تعریف_کرو configMAX_TASK_NAME_LEN    16
#جے_انت

#جے_بنا_تعریف configIDLE_SHOULD_YIELD
    #تعریف_کرو configIDLE_SHOULD_YIELD    1
#جے_انت

#إذا configMAX_TASK_NAME_LEN < 1
    #غلطی configMAX_TASK_NAME_LEN must be سیٹ منزل a minimum دا 1 وچ FreeRTOSConfig.h
#جے_انت

#جے_بنا_تعریف configASSERT
    #تعریف_کرو configASSERT( x )
    #تعریف_کرو configASSERT_DEFINED    0
#وإلا
    #تعریف_کرو configASSERT_DEFINED    1
#جے_انت

/* configPRECONDITION should be defined as configASSERT.
 * The CBMC proofs need a way to track assumptions and assertions.
 * A configPRECONDITION statement should express an implicit invariant or
 * assumption made.  A configASSERT statement should express an invariant that must
 * hold explicit before calling the code. */
#جے_بنا_تعریف configPRECONDITION
    #تعریف_کرو configPRECONDITION( X )    configASSERT( X )
    #تعریف_کرو configPRECONDITION_DEFINED    0
#وإلا
    #تعریف_کرو configPRECONDITION_DEFINED    1
#جے_انت

#جے_بنا_تعریف پورٹ_حافظہ_BARRIER
    #تعریف_کرو پورٹ_حافظہ_BARRIER()
#جے_انت

#جے_بنا_تعریف پورٹ_SOFTWARE_BARRIER
    #تعریف_کرو پورٹ_SOFTWARE_BARRIER()
#جے_انت

/* The timers module relies on xTaskGetSchedulerState(). */
#إذا configUSE_TIMERS == 1

    #جے_بنا_تعریف config_TIMER_TASK_ترجیح
        #غلطی If configUSE_TIMERS is سیٹ منزل 1 then config_TIMER_TASK_ترجیح must also be defined.
    #جے_انت /* configTIMER_TASK_PRIORITY */

    #جے_بنا_تعریف config_TIMER_قطار_لمائی
        #غلطی If configUSE_TIMERS is سیٹ منزل 1 then config_TIMER_قطار_لمائی must also be defined.
    #جے_انت /* configTIMER_QUEUE_LENGTH */

    #جے_بنا_تعریف config_TIMER_TASK_ڈھیر_ڈونگھائی
        #غلطی If configUSE_TIMERS is سیٹ منزل 1 then config_TIMER_TASK_ڈھیر_ڈونگھائی must also be defined.
    #جے_انت /* configTIMER_TASK_STACK_DEPTH */

#جے_انت /* configUSE_TIMERS */

#جے_بنا_تعریف پورٹ_سیٹ_رکاوٹ_نقاب_FROM_ISR
    #تعریف_کرو پورٹ_سیٹ_رکاوٹ_نقاب_FROM_ISR()    0
#جے_انت

#جے_بنا_تعریف پورٹ_CLEAR_رکاوٹ_نقاب_FROM_ISR
    #تعریف_کرو پورٹ_CLEAR_رکاوٹ_نقاب_FROM_ISR( ux_Saved_حالت_قدر )    ( فراغ ) ( ux_Saved_حالت_قدر )
#جے_انت

#جے_بنا_تعریف پورٹ_CLEAN_اتے_TCB
    #تعریف_کرو پورٹ_CLEAN_اتے_TCB( pxTCB )    ( فراغ ) ( pxTCB )
#جے_انت

#جے_بنا_تعریف پورٹ_PRE_TASK_DELETE_HOOK
    #تعریف_کرو پورٹ_PRE_TASK_DELETE_HOOK( pv_Task_منزل_Delete, pxYieldPending )
#جے_انت

#جے_بنا_تعریف پورٹ_تیاری_TCB
    #تعریف_کرو پورٹ_تیاری_TCB( pxTCB )    ( فراغ ) ( pxTCB )
#جے_انت

#جے_بنا_تعریف config_قطار_REGISTRY_ناپ
    #تعریف_کرو config_قطار_REGISTRY_ناپ    0U
#جے_انت

#إذا ( config_قطار_REGISTRY_ناپ < 1 )
    #تعریف_کرو v_قطار_جوڑو_منزل_Registry( x_قطار, pcName )
    #تعریف_کرو v_قطار_Unregister_قطار( x_قطار )
    #تعریف_کرو pc_قطار_Get_Name( x_قطار )
#جے_انت

#جے_بنا_تعریف config_USE_MINI_LIST_شے
    #تعریف_کرو config_USE_MINI_LIST_شے    1
#جے_انت

#جے_بنا_تعریف پورٹ_اشارہ_گر_ناپ_TYPE
    #تعریف_کرو پورٹ_اشارہ_گر_ناپ_TYPE    uint32_t
#جے_انت

/* Remove any unused trace macros. */
#جے_بنا_تعریف trace_شروع

/* Used to perform any necessary initialisation - for example, open a file
 * into which trace is to be written. */
    #تعریف_کرو trace_شروع()
#جے_انت

#جے_بنا_تعریف trace_سرا

/* Use to close a trace, for example close a file into which trace has been
 * written. */
    #تعریف_کرو trace_سرا()
#جے_انت

#جے_بنا_تعریف trace_TASK_SWITCHED_وچ

/* Called after a task has been selected to run.  pxCurrentTCB holds a pointer
 * to the task control block of the selected task. */
    #تعریف_کرو trace_TASK_SWITCHED_وچ()
#جے_انت

#جے_بنا_تعریف trace_INCREASE_TICK_گنتی

/* Called before stepping the tick count after waking from tickless idle
 * sleep. */
    #تعریف_کرو trace_INCREASE_TICK_گنتی( x )
#جے_انت

#جے_بنا_تعریف trace_کم_POWER_IDLE_BEGIN
    /* Called immediately before entering tickless idle. */
    #تعریف_کرو trace_کم_POWER_IDLE_BEGIN()
#جے_انت

#جے_بنا_تعریف trace_کم_POWER_IDLE_سرا
    /* Called when returning to the Idle task after a tickless idle. */
    #تعریف_کرو trace_کم_POWER_IDLE_سرا()
#جے_انت

#جے_بنا_تعریف trace_TASK_SWITCHED_باہرلا

/* Called before a task has been selected to run.  pxCurrentTCB holds a pointer
 * to the task control block of the task being switched out. */
    #تعریف_کرو trace_TASK_SWITCHED_باہرلا()
#جے_انت

#جے_بنا_تعریف trace_TASK_ترجیح_INHERIT

/* Called when a task attempts to take a mutex that is already held by a
 * lower priority task.  pxTCBOfMutexHolder is a pointer to the TCB of the task
 * that holds the mutex.  uxInheritedPriority is the priority the mutex holder
 * will inherit (the priority of the task that is attempting to obtain the
 * muted. */
    #تعریف_کرو trace_TASK_ترجیح_INHERIT( pxTCBOfMutexHolder, ux_Inherited_ترجیح )
#جے_انت

#جے_بنا_تعریف trace_TASK_ترجیح_DISINHERIT

/* Called when a task releases a mutex, the holding of which had resulted in
 * the task inheriting the priority of a higher priority task.
 * pxTCBOfMutexHolder is a pointer to the TCB of the task that is releasing the
 * mutex.  uxOriginalPriority is the task's configured (base) priority. */
    #تعریف_کرو trace_TASK_ترجیح_DISINHERIT( pxTCBOfMutexHolder, ux_Original_ترجیح )
#جے_انت

#جے_بنا_تعریف trace_BLOCKING_ON_قطار_RECEIVE

/* Task is about to block because it cannot read from a
 * queue/mutex/semaphore.  pxQueue is a pointer to the queue/mutex/semaphore
 * upon which the read was attempted.  pxCurrentTCB points to the TCB of the
 * task that attempted the read. */
    #تعریف_کرو trace_BLOCKING_ON_قطار_RECEIVE( px_قطار )
#جے_انت

#جے_بنا_تعریف trace_BLOCKING_ON_قطار_PEEK

/* Task is about to block because it cannot read from a
 * queue/mutex/semaphore.  pxQueue is a pointer to the queue/mutex/semaphore
 * upon which the read was attempted.  pxCurrentTCB points to the TCB of the
 * task that attempted the read. */
    #تعریف_کرو trace_BLOCKING_ON_قطار_PEEK( px_قطار )
#جے_انت

#جے_بنا_تعریف trace_BLOCKING_ON_قطار_SEND

/* Task is about to block because it cannot write to a
 * queue/mutex/semaphore.  pxQueue is a pointer to the queue/mutex/semaphore
 * upon which the write was attempted.  pxCurrentTCB points to the TCB of the
 * task that attempted the write. */
    #تعریف_کرو trace_BLOCKING_ON_قطار_SEND( px_قطار )
#جے_انت

#جے_بنا_تعریف config_CHECK_FOR_ڈھیر_حد_توں_ودھ
    #تعریف_کرو config_CHECK_FOR_ڈھیر_حد_توں_ودھ    0
#جے_انت

#جے_بنا_تعریف config_ریکارڈ_ڈھیر_بلند_پتہ
    #تعریف_کرو config_ریکارڈ_ڈھیر_بلند_پتہ    0
#جے_انت

#جے_بنا_تعریف configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H
    #تعریف_کرو configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H    0
#جے_انت

/* The following event macros are embedded in the kernel API calls. */

#جے_بنا_تعریف trace_MOVED_TASK_منزل_READY_STATE
    #تعریف_کرو trace_MOVED_TASK_منزل_READY_STATE( pxTCB )
#جے_انت

#جے_بنا_تعریف trace_POST_MOVED_TASK_منزل_READY_STATE
    #تعریف_کرو trace_POST_MOVED_TASK_منزل_READY_STATE( pxTCB )
#جے_انت

#جے_بنا_تعریف trace_قطار_CREATE
    #تعریف_کرو trace_قطار_CREATE( px_New_قطار )
#جے_انت

#جے_بنا_تعریف trace_قطار_CREATE_FAILED
    #تعریف_کرو trace_قطار_CREATE_FAILED( uc_قطار_Type )
#جے_انت

#جے_بنا_تعریف traceCREATE_MUTEX
    #تعریف_کرو traceCREATE_MUTEX( px_New_قطار )
#جے_انت

#جے_بنا_تعریف traceCREATE_MUTEX_FAILED
    #تعریف_کرو traceCREATE_MUTEX_FAILED()
#جے_انت

#جے_بنا_تعریف traceGIVE_MUTEX_RECURSIVE
    #تعریف_کرو traceGIVE_MUTEX_RECURSIVE( pxMutex )
#جے_انت

#جے_بنا_تعریف traceGIVE_MUTEX_RECURSIVE_FAILED
    #تعریف_کرو traceGIVE_MUTEX_RECURSIVE_FAILED( pxMutex )
#جے_انت

#جے_بنا_تعریف traceTAKE_MUTEX_RECURSIVE
    #تعریف_کرو traceTAKE_MUTEX_RECURSIVE( pxMutex )
#جے_انت

#جے_بنا_تعریف traceTAKE_MUTEX_RECURSIVE_FAILED
    #تعریف_کرو traceTAKE_MUTEX_RECURSIVE_FAILED( pxMutex )
#جے_انت

#جے_بنا_تعریف traceCREATE_COUNTING_SEMAPHORE
    #تعریف_کرو traceCREATE_COUNTING_SEMAPHORE()
#جے_انت

#جے_بنا_تعریف traceCREATE_COUNTING_SEMAPHORE_FAILED
    #تعریف_کرو traceCREATE_COUNTING_SEMAPHORE_FAILED()
#جے_انت

#جے_بنا_تعریف trace_قطار_سیٹ_SEND
    #تعریف_کرو trace_قطار_سیٹ_SEND    trace_قطار_SEND
#جے_انت

#جے_بنا_تعریف trace_قطار_SEND
    #تعریف_کرو trace_قطار_SEND( px_قطار )
#جے_انت

#جے_بنا_تعریف trace_قطار_SEND_FAILED
    #تعریف_کرو trace_قطار_SEND_FAILED( px_قطار )
#جے_انت

#جے_بنا_تعریف trace_قطار_RECEIVE
    #تعریف_کرو trace_قطار_RECEIVE( px_قطار )
#جے_انت

#جے_بنا_تعریف trace_قطار_PEEK
    #تعریف_کرو trace_قطار_PEEK( px_قطار )
#جے_انت

#جے_بنا_تعریف trace_قطار_PEEK_FAILED
    #تعریف_کرو trace_قطار_PEEK_FAILED( px_قطار )
#جے_انت

#جے_بنا_تعریف trace_قطار_PEEK_FROM_ISR
    #تعریف_کرو trace_قطار_PEEK_FROM_ISR( px_قطار )
#جے_انت

#جے_بنا_تعریف trace_قطار_RECEIVE_FAILED
    #تعریف_کرو trace_قطار_RECEIVE_FAILED( px_قطار )
#جے_انت

#جے_بنا_تعریف trace_قطار_SEND_FROM_ISR
    #تعریف_کرو trace_قطار_SEND_FROM_ISR( px_قطار )
#جے_انت

#جے_بنا_تعریف trace_قطار_SEND_FROM_ISR_FAILED
    #تعریف_کرو trace_قطار_SEND_FROM_ISR_FAILED( px_قطار )
#جے_انت

#جے_بنا_تعریف trace_قطار_RECEIVE_FROM_ISR
    #تعریف_کرو trace_قطار_RECEIVE_FROM_ISR( px_قطار )
#جے_انت

#جے_بنا_تعریف trace_قطار_RECEIVE_FROM_ISR_FAILED
    #تعریف_کرو trace_قطار_RECEIVE_FROM_ISR_FAILED( px_قطار )
#جے_انت

#جے_بنا_تعریف trace_قطار_PEEK_FROM_ISR_FAILED
    #تعریف_کرو trace_قطار_PEEK_FROM_ISR_FAILED( px_قطار )
#جے_انت

#جے_بنا_تعریف trace_قطار_DELETE
    #تعریف_کرو trace_قطار_DELETE( px_قطار )
#جے_انت

#جے_بنا_تعریف traceTASK_CREATE
    #تعریف_کرو traceTASK_CREATE( pxNewTCB )
#جے_انت

#جے_بنا_تعریف traceTASK_CREATE_FAILED
    #تعریف_کرو traceTASK_CREATE_FAILED()
#جے_انت

#جے_بنا_تعریف traceTASK_DELETE
    #تعریف_کرو traceTASK_DELETE( px_Task_منزل_Delete )
#جے_انت

#جے_بنا_تعریف traceTASK_DELAY_UNTIL
    #تعریف_کرو traceTASK_DELAY_UNTIL( x )
#جے_انت

#جے_بنا_تعریف traceTASK_DELAY
    #تعریف_کرو traceTASK_DELAY()
#جے_انت

#جے_بنا_تعریف trace_TASK_ترجیح_سیٹ
    #تعریف_کرو trace_TASK_ترجیح_سیٹ( pxTask, ux_New_ترجیح )
#جے_انت

#جے_بنا_تعریف traceTASK_SUSPEND
    #تعریف_کرو traceTASK_SUSPEND( px_Task_منزل_Suspend )
#جے_انت

#جے_بنا_تعریف traceTASK_RESUME
    #تعریف_کرو traceTASK_RESUME( px_Task_منزل_Resume )
#جے_انت

#جے_بنا_تعریف traceTASK_RESUME_FROM_ISR
    #تعریف_کرو traceTASK_RESUME_FROM_ISR( px_Task_منزل_Resume )
#جے_انت

#جے_بنا_تعریف traceTASK_INCREMENT_TICK
    #تعریف_کرو traceTASK_INCREMENT_TICK( x_Tick_گنتی )
#جے_انت

#جے_بنا_تعریف traceTIMER_CREATE
    #تعریف_کرو traceTIMER_CREATE( pxNewTimer )
#جے_انت

#جے_بنا_تعریف traceTIMER_CREATE_FAILED
    #تعریف_کرو traceTIMER_CREATE_FAILED()
#جے_انت

#جے_بنا_تعریف traceTIMER_COMMAND_SEND
    #تعریف_کرو traceTIMER_COMMAND_SEND( xTimer, x_سنیہا_ID, x_سنیہا_قدر_قدر, xReturn )
#جے_انت

#جے_بنا_تعریف traceTIMER_EXPIRED
    #تعریف_کرو traceTIMER_EXPIRED( pxTimer )
#جے_انت

#جے_بنا_تعریف traceTIMER_COMMAND_RECEIVED
    #تعریف_کرو traceTIMER_COMMAND_RECEIVED( pxTimer, x_سنیہا_ID, x_سنیہا_قدر )
#جے_انت

#جے_بنا_تعریف traceMALLOC
    #تعریف_کرو traceMALLOC( pv_پتہ, ui_ناپ )
#جے_انت

#جے_بنا_تعریف traceFREE
    #تعریف_کرو traceFREE( pv_پتہ, ui_ناپ )
#جے_انت

#جے_بنا_تعریف trace_EVENT_گروہ_CREATE
    #تعریف_کرو trace_EVENT_گروہ_CREATE( x_Event_گروہ )
#جے_انت

#جے_بنا_تعریف trace_EVENT_گروہ_CREATE_FAILED
    #تعریف_کرو trace_EVENT_گروہ_CREATE_FAILED()
#جے_انت

#جے_بنا_تعریف trace_EVENT_گروہ_SYNC_بلاک
    #تعریف_کرو trace_EVENT_گروہ_SYNC_بلاک( x_Event_گروہ, ux_بٹ_منزل_سیٹ, ux_بٹ_منزل_Wait_For )
#جے_انت

#جے_بنا_تعریف trace_EVENT_گروہ_SYNC_سرا
    #تعریف_کرو trace_EVENT_گروہ_SYNC_سرا( x_Event_گروہ, ux_بٹ_منزل_سیٹ, ux_بٹ_منزل_Wait_For, xTimeoutOccurred )    ( فراغ ) ( xTimeoutOccurred )
#جے_انت

#جے_بنا_تعریف trace_EVENT_گروہ_WAIT_بٹ_بلاک
    #تعریف_کرو trace_EVENT_گروہ_WAIT_بٹ_بلاک( x_Event_گروہ, ux_بٹ_منزل_Wait_For )
#جے_انت

#جے_بنا_تعریف trace_EVENT_گروہ_WAIT_بٹ_سرا
    #تعریف_کرو trace_EVENT_گروہ_WAIT_بٹ_سرا( x_Event_گروہ, ux_بٹ_منزل_Wait_For, xTimeoutOccurred )    ( فراغ ) ( xTimeoutOccurred )
#جے_انت

#جے_بنا_تعریف trace_EVENT_گروہ_CLEAR_بٹ
    #تعریف_کرو trace_EVENT_گروہ_CLEAR_بٹ( x_Event_گروہ, ux_بٹ_منزل_Clear )
#جے_انت

#جے_بنا_تعریف trace_EVENT_گروہ_CLEAR_بٹ_FROM_ISR
    #تعریف_کرو trace_EVENT_گروہ_CLEAR_بٹ_FROM_ISR( x_Event_گروہ, ux_بٹ_منزل_Clear )
#جے_انت

#جے_بنا_تعریف trace_EVENT_گروہ_سیٹ_بٹ
    #تعریف_کرو trace_EVENT_گروہ_سیٹ_بٹ( x_Event_گروہ, ux_بٹ_منزل_سیٹ )
#جے_انت

#جے_بنا_تعریف trace_EVENT_گروہ_سیٹ_بٹ_FROM_ISR
    #تعریف_کرو trace_EVENT_گروہ_سیٹ_بٹ_FROM_ISR( x_Event_گروہ, ux_بٹ_منزل_سیٹ )
#جے_انت

#جے_بنا_تعریف trace_EVENT_گروہ_DELETE
    #تعریف_کرو trace_EVENT_گروہ_DELETE( x_Event_گروہ )
#جے_انت

#جے_بنا_تعریف tracePEND_FUNC_CALL
    #تعریف_کرو tracePEND_FUNC_CALL( x_Function_منزل_Pend, pvParameter1, ulParameter2, ret )
#جے_انت

#جے_بنا_تعریف tracePEND_FUNC_CALL_FROM_ISR
    #تعریف_کرو tracePEND_FUNC_CALL_FROM_ISR( x_Function_منزل_Pend, pvParameter1, ulParameter2, ret )
#جے_انت

#جے_بنا_تعریف trace_قطار_REGISTRY_جوڑو
    #تعریف_کرو trace_قطار_REGISTRY_جوڑو( x_قطار, pc_قطار_Name )
#جے_انت

#جے_بنا_تعریف trace_TASK_NOTIFY_TAKE_بلاک
    #تعریف_کرو trace_TASK_NOTIFY_TAKE_بلاک( ux_اشاریہ_منزل_Wait )
#جے_انت

#جے_بنا_تعریف traceTASK_NOTIFY_TAKE
    #تعریف_کرو traceTASK_NOTIFY_TAKE( ux_اشاریہ_منزل_Wait )
#جے_انت

#جے_بنا_تعریف trace_TASK_NOTIFY_WAIT_بلاک
    #تعریف_کرو trace_TASK_NOTIFY_WAIT_بلاک( ux_اشاریہ_منزل_Wait )
#جے_انت

#جے_بنا_تعریف traceTASK_NOTIFY_WAIT
    #تعریف_کرو traceTASK_NOTIFY_WAIT( ux_اشاریہ_منزل_Wait )
#جے_انت

#جے_بنا_تعریف traceTASK_NOTIFY
    #تعریف_کرو traceTASK_NOTIFY( ux_اشاریہ_منزل_Notify )
#جے_انت

#جے_بنا_تعریف traceTASK_NOTIFY_FROM_ISR
    #تعریف_کرو traceTASK_NOTIFY_FROM_ISR( ux_اشاریہ_منزل_Notify )
#جے_انت

#جے_بنا_تعریف traceTASK_NOTIFY_GIVE_FROM_ISR
    #تعریف_کرو traceTASK_NOTIFY_GIVE_FROM_ISR( ux_اشاریہ_منزل_Notify )
#جے_انت

#جے_بنا_تعریف trace_دھارا_بفر_CREATE_FAILED
    #تعریف_کرو trace_دھارا_بفر_CREATE_FAILED( x_Is_سنیہا_بفر )
#جے_انت

#جے_بنا_تعریف trace_دھارا_بفر_CREATE_STATIC_FAILED
    #تعریف_کرو trace_دھارا_بفر_CREATE_STATIC_FAILED( xReturn, x_Is_سنیہا_بفر )
#جے_انت

#جے_بنا_تعریف trace_دھارا_بفر_CREATE
    #تعریف_کرو trace_دھارا_بفر_CREATE( px_دھارا_بفر, x_Is_سنیہا_بفر )
#جے_انت

#جے_بنا_تعریف trace_دھارا_بفر_DELETE
    #تعریف_کرو trace_دھارا_بفر_DELETE( x_دھارا_بفر )
#جے_انت

#جے_بنا_تعریف trace_دھارا_بفر_RESET
    #تعریف_کرو trace_دھارا_بفر_RESET( x_دھارا_بفر )
#جے_انت

#جے_بنا_تعریف trace_BLOCKING_ON_دھارا_بفر_SEND
    #تعریف_کرو trace_BLOCKING_ON_دھارا_بفر_SEND( x_دھارا_بفر )
#جے_انت

#جے_بنا_تعریف trace_دھارا_بفر_SEND
    #تعریف_کرو trace_دھارا_بفر_SEND( x_دھارا_بفر, x_بائٹاں_گھلیا )
#جے_انت

#جے_بنا_تعریف trace_دھارا_بفر_SEND_FAILED
    #تعریف_کرو trace_دھارا_بفر_SEND_FAILED( x_دھارا_بفر )
#جے_انت

#جے_بنا_تعریف trace_دھارا_بفر_SEND_FROM_ISR
    #تعریف_کرو trace_دھارا_بفر_SEND_FROM_ISR( x_دھارا_بفر, x_بائٹاں_گھلیا )
#جے_انت

#جے_بنا_تعریف trace_BLOCKING_ON_دھارا_بفر_RECEIVE
    #تعریف_کرو trace_BLOCKING_ON_دھارا_بفر_RECEIVE( x_دھارا_بفر )
#جے_انت

#جے_بنا_تعریف trace_دھارا_بفر_RECEIVE
    #تعریف_کرو trace_دھارا_بفر_RECEIVE( x_دھارا_بفر, x_Received_لمائی )
#جے_انت

#جے_بنا_تعریف trace_دھارا_بفر_RECEIVE_FAILED
    #تعریف_کرو trace_دھارا_بفر_RECEIVE_FAILED( x_دھارا_بفر )
#جے_انت

#جے_بنا_تعریف trace_دھارا_بفر_RECEIVE_FROM_ISR
    #تعریف_کرو trace_دھارا_بفر_RECEIVE_FROM_ISR( x_دھارا_بفر, x_Received_لمائی )
#جے_انت

#جے_بنا_تعریف config_GENERATE_لگاتار_ویلا_STATS
    #تعریف_کرو config_GENERATE_لگاتار_ویلا_STATS    0
#جے_انت

#إذا ( config_GENERATE_لگاتار_ویلا_STATS == 1 )

    #جے_بنا_تعریف پورٹ_CONFIGURE_TIMER_FOR_لگاتار_ویلا_STATS
        #غلطی If config_GENERATE_لگاتار_ویلا_STATS is defined then پورٹ_CONFIGURE_TIMER_FOR_لگاتار_ویلا_STATS must also be defined.  پورٹ_CONFIGURE_TIMER_FOR_لگاتار_ویلا_STATS should call a پورٹ layer function منزل تیاری a peripheral timer/counter that can then be ورتیا as the لگاتار time counter time ادھار.
    #جے_انت /* portCONFIGURE_TIMER_FOR_RUN_TIME_STATS */

    #جے_بنا_تعریف پورٹ_GET_لگاتار_ویلا_COUNTER_قدر
        #جے_بنا_تعریف پورٹ_ALT_GET_لگاتار_ویلا_COUNTER_قدر
            #غلطی If config_GENERATE_لگاتار_ویلا_STATS is defined then either پورٹ_GET_لگاتار_ویلا_COUNTER_قدر or پورٹ_ALT_GET_لگاتار_ویلا_COUNTER_قدر must also be defined.  See the examples provided and the FreeRTOS web site لكل ہور information.
        #جے_انت /* portALT_GET_RUN_TIME_COUNTER_VALUE */
    #جے_انت /* portGET_RUN_TIME_COUNTER_VALUE */

#جے_انت /* configGENERATE_RUN_TIME_STATS */

#جے_بنا_تعریف پورٹ_CONFIGURE_TIMER_FOR_لگاتار_ویلا_STATS
    #تعریف_کرو پورٹ_CONFIGURE_TIMER_FOR_لگاتار_ویلا_STATS()
#جے_انت

#جے_بنا_تعریف configUSE_MALLOC_FAILED_HOOK
    #تعریف_کرو configUSE_MALLOC_FAILED_HOOK    0
#جے_انت

#جے_بنا_تعریف پورٹ_PRIVILEGE_ثنائی_ہندسہ
    #تعریف_کرو پورٹ_PRIVILEGE_ثنائی_ہندسہ    ( ( UBaseType_t ) 0x00 )
#جے_انت

#جے_بنا_تعریف پورٹ_YIELD_WITHIN_API
    #تعریف_کرو پورٹ_YIELD_WITHIN_API    پورٹ_YIELD
#جے_انت

#جے_بنا_تعریف پورٹ_SUPPRESS_TICKS_AND_SLEEP
    #تعریف_کرو پورٹ_SUPPRESS_TICKS_AND_SLEEP( x_Expected_Idle_ویلا )
#جے_انت

#جے_بنا_تعریف config_EXPECTED_IDLE_ویلا_BEFORE_SLEEP
    #تعریف_کرو config_EXPECTED_IDLE_ویلا_BEFORE_SLEEP    2
#جے_انت

#إذا config_EXPECTED_IDLE_ویلا_BEFORE_SLEEP < 2
    #غلطی config_EXPECTED_IDLE_ویلا_BEFORE_SLEEP must not be less than 2
#جے_انت

#جے_بنا_تعریف configUSE_TICKLESS_IDLE
    #تعریف_کرو configUSE_TICKLESS_IDLE    0
#جے_انت

#جے_بنا_تعریف configPRE_SUPPRESS_TICKS_AND_SLEEP_PROCESSING
    #تعریف_کرو configPRE_SUPPRESS_TICKS_AND_SLEEP_PROCESSING( x )
#جے_انت

#جے_بنا_تعریف configPRE_SLEEP_PROCESSING
    #تعریف_کرو configPRE_SLEEP_PROCESSING( x )
#جے_انت

#جے_بنا_تعریف configPOST_SLEEP_PROCESSING
    #تعریف_کرو configPOST_SLEEP_PROCESSING( x )
#جے_انت

#جے_بنا_تعریف config_USE_قطار_SETS
    #تعریف_کرو config_USE_قطار_SETS    0
#جے_انت

#جے_بنا_تعریف پورٹ_TASK_ورتدا_FLOATING_نقطہ
    #تعریف_کرو پورٹ_TASK_ورتدا_FLOATING_نقطہ()
#جے_انت

#جے_بنا_تعریف پورٹ_ALLOCATE_SECURE_CONTEXT
    #تعریف_کرو پورٹ_ALLOCATE_SECURE_CONTEXT( ul_Secure_ڈھیر_ناپ )
#جے_انت

#جے_بنا_تعریف پورٹ_DONT_DISCARD
    #تعریف_کرو پورٹ_DONT_DISCARD
#جے_انت

#جے_بنا_تعریف config_USE_ویلا_SLICING
    #تعریف_کرو config_USE_ویلا_SLICING    1
#جے_انت

#جے_بنا_تعریف configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS
    #تعریف_کرو configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS    0
#جے_انت

#جے_بنا_تعریف configUSE_STATS_FORMATTING_FUNCTIONS
    #تعریف_کرو configUSE_STATS_FORMATTING_FUNCTIONS    0
#جے_انت

#جے_بنا_تعریف پورٹ_ASSERT_IF_رکاوٹ_ترجیح_INVALID
    #تعریف_کرو پورٹ_ASSERT_IF_رکاوٹ_ترجیح_INVALID()
#جے_انت

#جے_بنا_تعریف configUSE_TRACE_FACILITY
    #تعریف_کرو configUSE_TRACE_FACILITY    0
#جے_انت

#جے_بنا_تعریف mtCOVERAGE_TEST_MARKER
    #تعریف_کرو mtCOVERAGE_TEST_MARKER()
#جے_انت

#جے_بنا_تعریف mtCOVERAGE_TEST_DELAY
    #تعریف_کرو mtCOVERAGE_TEST_DELAY()
#جے_انت

#جے_بنا_تعریف پورٹ_ASSERT_IF_وچ_ISR
    #تعریف_کرو پورٹ_ASSERT_IF_وچ_ISR()
#جے_انت

#جے_بنا_تعریف config_USE_پورٹ_OPTIMISED_TASK_SELECTION
    #تعریف_کرو config_USE_پورٹ_OPTIMISED_TASK_SELECTION    0
#جے_انت

#جے_بنا_تعریف config_APPLICATION_ALLOCATED_ڈھیر
    #تعریف_کرو config_APPLICATION_ALLOCATED_ڈھیر    0
#جے_انت

#جے_بنا_تعریف configUSE_TASK_NOTIFICATIONS
    #تعریف_کرو configUSE_TASK_NOTIFICATIONS    1
#جے_انت

#جے_بنا_تعریف configTASK_NOTIFICATION_ARRAY_ENTRIES
    #تعریف_کرو configTASK_NOTIFICATION_ARRAY_ENTRIES    1
#جے_انت

#إذا configTASK_NOTIFICATION_ARRAY_ENTRIES < 1
    #غلطی configTASK_NOTIFICATION_ARRAY_ENTRIES must be at گھٹ_توں_گھٹ 1
#جے_انت

#جے_بنا_تعریف configUSE_POSIX_ERRNO
    #تعریف_کرو configUSE_POSIX_ERRNO    0
#جے_انت

#جے_بنا_تعریف configUSE_SB_COMPLETED_CALLBACK

/* By default per-instance callbacks are not enabled for stream buffer or message buffer. */
    #تعریف_کرو configUSE_SB_COMPLETED_CALLBACK    0
#جے_انت

#جے_بنا_تعریف پورٹ_TICK_TYPE_IS_ATOMIC
    #تعریف_کرو پورٹ_TICK_TYPE_IS_ATOMIC    0
#جے_انت

#جے_بنا_تعریف config_SUPPORT_STATIC_ونڈ
    /* Defaults to 0 for backward compatibility. */
    #تعریف_کرو config_SUPPORT_STATIC_ونڈ    0
#جے_انت

#جے_بنا_تعریف config_SUPPORT_DYNAMIC_ونڈ
    /* Defaults to 1 for backward compatibility. */
    #تعریف_کرو config_SUPPORT_DYNAMIC_ونڈ    1
#جے_انت

#إذا ( ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 ) && ( config_SUPPORT_DYNAMIC_ونڈ != 1 ) )
    #غلطی configUSE_STATS_FORMATTING_FUNCTIONS cannot be ورتیا without dynamic ونڈ, but config_SUPPORT_DYNAMIC_ونڈ is not سیٹ منزل 1.
#جے_انت

#إذا ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 )
    #إذا ( ( configUSE_TRACE_FACILITY != 1 ) && ( config_GENERATE_لگاتار_ویلا_STATS != 1 ) )
        #غلطی configUSE_STATS_FORMATTING_FUNCTIONS is 1 but the functions it enables are not ورتیا because neither configUSE_TRACE_FACILITY or config_GENERATE_لگاتار_ویلا_STATS are 1.  سیٹ_2 configUSE_STATS_FORMATTING_FUNCTIONS منزل 0 وچ FreeRTOSConfig.h.
    #جے_انت
#جے_انت

#جے_بنا_تعریف config_ڈھیر_ڈونگھائی_TYPE

/* Defaults to uint16_t for backward compatibility, but can be overridden
 * in FreeRTOSConfig.h if uint16_t is too restrictive. */
    #تعریف_کرو config_ڈھیر_ڈونگھائی_TYPE    uint16_t
#جے_انت

#جے_بنا_تعریف config_لگاتار_ویلا_COUNTER_TYPE

/* Defaults to uint32_t for backward compatibility, but can be overridden in
 * FreeRTOSConfig.h if uint32_t is too restrictive. */

    #تعریف_کرو config_لگاتار_ویلا_COUNTER_TYPE    uint32_t
#جے_انت

#جے_بنا_تعریف config_سنیہا_بفر_لمائی_TYPE

/* Defaults to size_t for backward compatibility, but can be overridden
 * in FreeRTOSConfig.h if lengths will always be less than the number of bytes
 * in a size_t. */
    #تعریف_کرو config_سنیہا_بفر_لمائی_TYPE    size_t
#جے_انت

/* Sanity check the configuration. */
#إذا ( ( config_SUPPORT_STATIC_ونڈ == 0 ) && ( config_SUPPORT_DYNAMIC_ونڈ == 0 ) )
    #غلطی config_SUPPORT_STATIC_ونڈ and config_SUPPORT_DYNAMIC_ونڈ cannot both be 0, but can both be 1.
#جے_انت

#إذا ( ( configUSE_RECURSIVE_MUTEXES == 1 ) && ( configUSE_MUTEXES != 1 ) )
    #غلطی configUSE_MUTEXES must be سیٹ منزل 1 منزل use recursive mutexes
#جے_انت

#إذا ( ( config_نمبر_دا_CORES > 1 ) && ( config_USE_پورٹ_OPTIMISED_TASK_SELECTION != 0 ) )
    #غلطی config_USE_پورٹ_OPTIMISED_TASK_SELECTION is not supported وچ SMP
#جے_انت

#جے_بنا_تعریف config_INITIAL_TICK_گنتی
    #تعریف_کرو config_INITIAL_TICK_گنتی    0
#جے_انت

#إذا ( پورٹ_TICK_TYPE_IS_ATOMIC == 0 )

/* Either variables of tick type cannot be read atomically, or
 * portTICK_TYPE_IS_ATOMIC was not set - map the critical sections used when
 * the tick count is returned to the standard critical section macros. */
    #تعریف_کرو پورٹ_TICK_TYPE_ENTER_CRITICAL()                      پورٹ_ENTER_CRITICAL()
    #تعریف_کرو پورٹ_TICK_TYPE_EXIT_CRITICAL()                       پورٹ_EXIT_CRITICAL()
    #تعریف_کرو پورٹ_TICK_TYPE_سیٹ_رکاوٹ_نقاب_FROM_ISR()         پورٹ_سیٹ_رکاوٹ_نقاب_FROM_ISR()
    #تعریف_کرو پورٹ_TICK_TYPE_CLEAR_رکاوٹ_نقاب_FROM_ISR( x )    پورٹ_CLEAR_رکاوٹ_نقاب_FROM_ISR( ( x ) )
#وإلا

/* The tick type can be read atomically, so critical sections used when the
 * tick count is returned can be defined away. */
    #تعریف_کرو پورٹ_TICK_TYPE_ENTER_CRITICAL()
    #تعریف_کرو پورٹ_TICK_TYPE_EXIT_CRITICAL()
    #تعریف_کرو پورٹ_TICK_TYPE_سیٹ_رکاوٹ_نقاب_FROM_ISR()         0
    #تعریف_کرو پورٹ_TICK_TYPE_CLEAR_رکاوٹ_نقاب_FROM_ISR( x )    ( فراغ ) ( x )
#جے_انت /* if ( portTICK_TYPE_IS_ATOMIC == 0 ) */

/* Definitions to allow backward compatibility with FreeRTOS versions prior to
 * V8 if desired. */
#جے_بنا_تعریف configENABLE_BACKWARD_COMPATIBILITY
    #تعریف_کرو configENABLE_BACKWARD_COMPATIBILITY    1
#جے_انت

#جے_بنا_تعریف configPRINTF

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
#جے_انت

#جے_بنا_تعریف configMAX

/* The application writer has not provided their own MAX macro, so define
 * the following generic implementation. */
    #تعریف_کرو configMAX( a, b )    ( ( ( a ) > ( b ) ) ? ( a ) : ( b ) )
#جے_انت

#جے_بنا_تعریف configMIN

/* The application writer has not provided their own MIN macro, so define
 * the following generic implementation. */
    #تعریف_کرو configMIN( a, b )    ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )
#جے_انت

#إذا configENABLE_BACKWARD_COMPATIBILITY == 1
    #تعریف_کرو eTaskStateGet                 eTaskGetState
    #تعریف_کرو پورٹ_Tick_Type                  TickType_t
    #تعریف_کرو xTaskHandle                   TaskHandle_t
    #تعریف_کرو x_قطار_Handle                  قطار_Handle_t
    #تعریف_کرو xSemaphoreHandle              SemaphoreHandle_t
    #تعریف_کرو x_قطار_سیٹ_Handle               قطار_سیٹ_Handle_t
    #تعریف_کرو x_قطار_سیٹ_Member_Handle         قطار_سیٹ_Member_Handle_t
    #تعریف_کرو x_ویلا_باہرلا_Type                  ویلا_باہرلا_t
    #تعریف_کرو x_حافظہ_Region                 حافظہ_Region_t
    #تعریف_کرو xTaskParameters               TaskParameters_t
    #تعریف_کرو x_Task_حالت_Type               Task_حالت_t
    #تعریف_کرو xTimerHandle                  TimerHandle_t
    #تعریف_کرو xCoRoutineHandle              CoRoutineHandle_t
    #تعریف_کرو pd_TASK_HOOK_کوڈ              TaskHookFunction_t
    #تعریف_کرو پورٹ_TICK_شرح_MS              پورٹ_TICK_PERIOD_MS
    #تعریف_کرو pcTaskGetTaskName             pcTaskGetName
    #تعریف_کرو pcTimerGetTimerName           pcTimerGetName
    #تعریف_کرو pc_قطار_Get_قطار_Name           pc_قطار_Get_Name
    #تعریف_کرو vTaskGetTaskInfo              vTaskGetInfo
    #تعریف_کرو x_Task_Get_Idle_لگاتار_ویلا_Counter    ul_Task_Get_Idle_لگاتار_ویلا_Counter

/* Backward compatibility within the scheduler code only - these definitions
 * are not really required but are included for completeness. */
    #تعریف_کرو tmrTIMER_CALLBACK             TimerCallbackFunction_t
    #تعریف_کرو pd_TASK_کوڈ                   TaskFunction_t
    #تعریف_کرو x_List_شے                     List_شے_t
    #تعریف_کرو xList                         List_t

/* For libraries that break the list data hiding, and access list structure
 * members directly (which is not supposed to be done). */
    #تعریف_کرو pxContainer                   pvContainer
#جے_انت /* configENABLE_BACKWARD_COMPATIBILITY */

#إذا ( configUSE_ALTERNATIVE_API != 0 )
    #غلطی The alternative API was deprecated some time ago, and was removed وچ FreeRTOS V9.0 0
#جے_انت

/* Set configUSE_TASK_FPU_SUPPORT to 0 to omit floating point support even
 * if floating point hardware is otherwise supported by the FreeRTOS port in use.
 * This constant is not supported by all FreeRTOS ports that include floating
 * point support. */
#جے_بنا_تعریف configUSE_TASK_FPU_SUPPORT
    #تعریف_کرو configUSE_TASK_FPU_SUPPORT    1
#جے_انت

/* Set configENABLE_MPU to 1 to enable MPU support and 0 to disable it. This is
 * currently used in ARMv8M ports. */
#جے_بنا_تعریف configENABLE_MPU
    #تعریف_کرو configENABLE_MPU    0
#جے_انت

/* Set configENABLE_FPU to 1 to enable FPU support and 0 to disable it. This is
 * currently used in ARMv8M ports. */
#جے_بنا_تعریف configENABLE_FPU
    #تعریف_کرو configENABLE_FPU    1
#جے_انت

/* Set configENABLE_MVE to 1 to enable MVE support and 0 to disable it. This is
 * currently used in ARMv8M ports. */
#جے_بنا_تعریف configENABLE_MVE
    #تعریف_کرو configENABLE_MVE    0
#جے_انت

/* Set configENABLE_TRUSTZONE to 1 enable TrustZone support and 0 to disable it.
 * This is currently used in ARMv8M ports. */
#جے_بنا_تعریف configENABLE_TRUSTZONE
    #تعریف_کرو configENABLE_TRUSTZONE    1
#جے_انت

/* Set configRUN_FREERTOS_SECURE_ONLY to 1 to run the FreeRTOS ARMv8M port on
 * the Secure Side only. */
#جے_بنا_تعریف config_لگاتار_FREERTOS_SECURE_ONLY
    #تعریف_کرو config_لگاتار_FREERTOS_SECURE_ONLY    0
#جے_انت

#جے_بنا_تعریف config_لگاتار_ADDITIONAL_TESTS
    #تعریف_کرو config_لگاتار_ADDITIONAL_TESTS    0
#جے_انت


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
#تعریف_کرو tsk_STATIC_AND_DYNAMIC_ونڈ_POSSIBLE                                                                                     \
    ( ( ( پورٹ_USING_MPU_WRAPPERS == 0 ) && ( config_SUPPORT_DYNAMIC_ونڈ == 1 ) && ( config_SUPPORT_STATIC_ونڈ == 1 ) ) || \
      ( ( پورٹ_USING_MPU_WRAPPERS == 1 ) && ( config_SUPPORT_DYNAMIC_ونڈ == 1 ) ) )

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
بنية x_STATIC_LIST_شے
{
    #إذا ( config_USE_LIST_انکڑے_INTEGRITY_CHECK_بائٹاں == 1 )
        TickType_t xDummy1;
    #جے_انت
    TickType_t xDummy2;
    فراغ * pvDummy3[ 4 ];
    #إذا ( config_USE_LIST_انکڑے_INTEGRITY_CHECK_بائٹاں == 1 )
        TickType_t xDummy4;
    #جے_انت
};
تعريفنوع بنية x_STATIC_LIST_شے Static_List_شے_t;

#إذا ( config_USE_MINI_LIST_شے == 1 )
    /* See the comments above the struct xSTATIC_LIST_ITEM definition. */
    بنية x_STATIC_MINI_LIST_شے
    {
        #إذا ( config_USE_LIST_انکڑے_INTEGRITY_CHECK_بائٹاں == 1 )
            TickType_t xDummy1;
        #جے_انت
        TickType_t xDummy2;
        فراغ * pvDummy3[ 2 ];
    };
    تعريفنوع بنية x_STATIC_MINI_LIST_شے Static_Mini_List_شے_t;
#وإلا /* if ( configUSE_MINI_LIST_ITEM == 1 ) */
    تعريفنوع بنية x_STATIC_LIST_شے      Static_Mini_List_شے_t;
#جے_انت /* if ( configUSE_MINI_LIST_ITEM == 1 ) */

/* See the comments above the struct xSTATIC_LIST_ITEM definition. */
تعريفنوع بنية xSTATIC_LIST
{
    #إذا ( config_USE_LIST_انکڑے_INTEGRITY_CHECK_بائٹاں == 1 )
        TickType_t xDummy1;
    #جے_انت
    UBaseType_t uxDummy2;
    فراغ * pvDummy3;
    Static_Mini_List_شے_t xDummy4;
    #إذا ( config_USE_LIST_انکڑے_INTEGRITY_CHECK_بائٹاں == 1 )
        TickType_t xDummy5;
    #جے_انت
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
    #إذا ( پورٹ_USING_MPU_WRAPPERS == 1 )
        xMPU_SETTINGS xDummy2;
    #جے_انت
    Static_List_شے_t xDummy3[ 2 ];
    UBaseType_t uxDummy5;
    فراغ * pxDummy6;
    uint8_t ucDummy7[ configMAX_TASK_NAME_LEN ];
    #إذا ( config_نمبر_دا_CORES > 1 )
        ادھار_Type_t xDummyCoreID;
    #جے_انت /* configNUMBER_OF_CORES > 1 */
    #إذا ( ( پورٹ_ڈھیر_GROWTH > 0 ) || ( config_ریکارڈ_ڈھیر_بلند_پتہ == 1 ) )
        فراغ * pxDummy8;
    #جے_انت
    #إذا ( پورٹ_CRITICAL_NESTING_وچ_TCB == 1 )
        UBaseType_t uxDummy9;
    #جے_انت
    #إذا ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy10[ 2 ];
    #جے_انت
    #إذا ( configUSE_MUTEXES == 1 )
        UBaseType_t uxDummy12[ 2 ];
    #جے_انت
    #إذا ( config_USE_APPLICATION_TASK_ٹیگ == 1 )
        فراغ * pxDummy14;
    #جے_انت
    #إذا ( config_عدد_THREAD_LOCAL_STORAGE_سوچکاں > 0 )
        فراغ * pvDummy15[ config_عدد_THREAD_LOCAL_STORAGE_سوچکاں ];
    #جے_انت
    #إذا ( config_GENERATE_لگاتار_ویلا_STATS == 1 )
        config_لگاتار_ویلا_COUNTER_TYPE ulDummy16;
    #جے_انت
    #إذا ( ( configUSE_NEWLIB_REENTRANT == 1 ) || ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 ) )
        config_TLS_بلاک_TYPE xDummy17;
    #جے_انت
    #إذا ( configUSE_TASK_NOTIFICATIONS == 1 )
        uint32_t ulDummy18[ configTASK_NOTIFICATION_ARRAY_ENTRIES ];
        uint8_t ucDummy19[ configTASK_NOTIFICATION_ARRAY_ENTRIES ];
    #جے_انت
    #إذا ( tsk_STATIC_AND_DYNAMIC_ونڈ_POSSIBLE != 0 )
        uint8_t uxDummy20;
    #جے_انت

    #إذا ( INCLUDE_xTaskAbortDelay == 1 )
        uint8_t ucDummy21;
    #جے_انت
    #إذا ( configUSE_POSIX_ERRNO == 1 )
        صحيح iDummy22;
    #جے_انت
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

    #إذا ( ( config_SUPPORT_STATIC_ونڈ == 1 ) && ( config_SUPPORT_DYNAMIC_ونڈ == 1 ) )
        uint8_t ucDummy6;
    #جے_انت

    #إذا ( config_USE_قطار_SETS == 1 )
        فراغ * pvDummy7;
    #جے_انت

    #إذا ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy8;
        uint8_t ucDummy9;
    #جے_انت
    پورٹ_MUX_TYPE x_Dummy_قطار_جندرا;
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
تعريفنوع بنية x_STATIC_EVENT_گروہ
{
    TickType_t xDummy1;
    StaticList_t xDummy2;

    #إذا ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy3;
    #جے_انت

    #إذا ( ( config_SUPPORT_STATIC_ونڈ == 1 ) && ( config_SUPPORT_DYNAMIC_ونڈ == 1 ) )
        uint8_t ucDummy4;
    #جے_انت
    پورٹ_MUX_TYPE x_Dummy_Event_گروہ_جندرا;
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
تعريفنوع بنية xSTATIC_TIMER
{
    فراغ * pvDummy1;
    Static_List_شے_t xDummy2;
    TickType_t xDummy3;
    فراغ * pvDummy5;
    TaskFunction_t pvDummy6;
    #إذا ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy7;
    #جے_انت
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
تعريفنوع بنية x_STATIC_دھارا_بفر
{
    size_t uxDummy1[ 4 ];
    فراغ * pvDummy2[ 3 ];
    uint8_t ucDummy3;
    #إذا ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy4;
    #جے_انت
    #إذا ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        فراغ * pvDummy5[ 2 ];
    #جے_انت
    پورٹ_MUX_TYPE x_Dummy_دھارا_بفر_جندرا;
} Static_دھارا_بفر_t;

/* Message buffers are built on stream buffers. */
تعريفنوع Static_دھارا_بفر_t Static_سنیہا_بفر_t;

/* *INDENT-OFF* */
#جے_تعریف_شدہ __cplusplus
    }
#جے_انت
/* *INDENT-ON* */

/*-----------------------------------------------------------
* IDF Compatibility
*----------------------------------------------------------*/

#جے_تعریف_شدہ ESP_PLATFORM

/* *INDENT-OFF* */
#جے_تعریف_شدہ __cplusplus
    خارجي "C" {
#جے_انت
/* *INDENT-ON* */

/*
 * Default values for trace macros added by ESP-IDF and are not part of Vanilla FreeRTOS
 */

    #جے_بنا_تعریف trace_ISR_EXIT_منزل_SCHEDULER
        #تعریف_کرو trace_ISR_EXIT_منزل_SCHEDULER()
    #جے_انت

    #جے_بنا_تعریف traceISR_EXIT
        #تعریف_کرو traceISR_EXIT()
    #جے_انت

    #جے_بنا_تعریف traceISR_ENTER
        #تعریف_کرو traceISR_ENTER( _n_ )
    #جے_انت

    #جے_بنا_تعریف trace_قطار_SEMAPHORE_RECEIVE
        #تعریف_کرو trace_قطار_SEMAPHORE_RECEIVE( px_قطار )
    #جے_انت

    #جے_بنا_تعریف trace_قطار_GIVE_FROM_ISR
        #تعریف_کرو trace_قطار_GIVE_FROM_ISR( px_قطار )
    #جے_انت

    #جے_بنا_تعریف trace_قطار_GIVE_FROM_ISR_FAILED
        #تعریف_کرو trace_قطار_GIVE_FROM_ISR_FAILED( px_قطار )
    #جے_انت

/* *INDENT-OFF* */
#جے_تعریف_شدہ __cplusplus
    }
#جے_انت
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
    #ضمن "freertos/idf_additions.h"

#جے_انت /* ESP_PLATFORM */

#جے_انت /* INC_FREERTOS_H */
