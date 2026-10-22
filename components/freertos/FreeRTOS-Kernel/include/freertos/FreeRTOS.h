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

#यदि_अपरिभाषितम् INC_FREERTOS_H
#परिभाषय INC_FREERTOS_H

/*
 * Include the generic headers required for the FreeRTOS port being used.
 */
#अन्तर्भाव <stddef.h>

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
#अन्तर्भाव <stdint.h> /* READ COMMENT ABOVE. */

/* *INDENT-OFF* */
#यदि_परिभाषितम् __cplusplus
    बाह्य "C" {
#यदि_अन्तः
/* *INDENT-ON* */

/* Application specific configuration options. */
#अन्तर्भाव "FreeRTOSConfig.h"

/* Basic FreeRTOS definitions. */
#अन्तर्भाव "projdefs.h"

/* Definitions specific to the port being used. */
#अन्तर्भाव "portable.h"

/* Must be defaulted before configUSE_NEWLIB_REENTRANT is used below. */
#यदि_अपरिभाषितम् configUSE_NEWLIB_REENTRANT
    #परिभाषय configUSE_NEWLIB_REENTRANT    0
#यदि_अन्तः

/* Required if struct _reent is used. */
#यदि ( configUSE_NEWLIB_REENTRANT == 1 )

/* Note Newlib support has been included by popular demand, but is not
 * used by the FreeRTOS maintainers themselves.  FreeRTOS is not
 * responsible for resulting newlib operation.  User must be familiar with
 * newlib and must provide system-wide implementations of the necessary
 * stubs. Be warned that (at the time of writing) the current newlib design
 * implements a system-wide malloc() that must be provided with locks.
 *
 * See the third party link http://www.nadler.com/embedded/newlibAndFreeRTOS.html
 * for additional information. */
    #अन्तर्भाव <reent.h>

    #परिभाषय configUSE_C_RUNTIME_TLS_SUPPORT    1

    #यदि_अपरिभाषितम् config_TLS_खण्डम्_TYPE
        #परिभाषय config_TLS_खण्डम्_TYPE           संरचना _reent
    #यदि_अन्तः

    #यदि_अपरिभाषितम् config_INIT_TLS_खण्डम्
        #परिभाषय config_INIT_TLS_खण्डम्( xTLSBlock )    _REENT_INIT_PTR( &( xTLSBlock ) )
    #यदि_अन्तः

    #यदि_अपरिभाषितम् config_समुच्चयः_TLS_खण्डम्
        #परिभाषय config_समुच्चयः_TLS_खण्डम्( xTLSBlock )    _impure_ptr = &( xTLSBlock )
    #यदि_अन्तः

    #यदि_अपरिभाषितम् config_DEINIT_TLS_खण्डम्
        #परिभाषय config_DEINIT_TLS_खण्डम्( xTLSBlock )    _reclaim_reent( &( xTLSBlock ) )
    #यदि_अन्तः
#यदि_अन्तः /* if ( configUSE_NEWLIB_REENTRANT == 1 ) */

#यदि_अपरिभाषितम् configUSE_C_RUNTIME_TLS_SUPPORT
    #परिभाषय configUSE_C_RUNTIME_TLS_SUPPORT    0
#यदि_अन्तः

#यदि ( ( configUSE_NEWLIB_REENTRANT == 0 ) && ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 ) )

    #यदि_अपरिभाषितम् config_TLS_खण्डम्_TYPE
        #दोषः Missing definition:  config_TLS_खण्डम्_TYPE must be defined अन्तर् FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is समुच्चयः गन्तव्यम् 1.
    #यदि_अन्तः

    #यदि_अपरिभाषितम् config_INIT_TLS_खण्डम्
        #दोषः Missing definition:  config_INIT_TLS_खण्डम् must be defined अन्तर् FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is समुच्चयः गन्तव्यम् 1.
    #यदि_अन्तः

    #यदि_अपरिभाषितम् config_समुच्चयः_TLS_खण्डम्
        #दोषः Missing definition:  config_समुच्चयः_TLS_खण्डम् must be defined अन्तर् FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is समुच्चयः गन्तव्यम् 1.
    #यदि_अन्तः

    #यदि_अपरिभाषितम् config_DEINIT_TLS_खण्डम्
        #दोषः Missing definition:  config_DEINIT_TLS_खण्डम् must be defined अन्तर् FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is समुच्चयः गन्तव्यम् 1.
    #यदि_अन्तः
#यदि_अन्तः /* if ( ( configUSE_NEWLIB_REENTRANT == 0 ) && ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 ) ) */

/*
 * Check all the required application specific macros have been defined.
 * These macros are application specific and (as downloaded) are defined
 * within FreeRTOSConfig.h.
 */

#यदि_अपरिभाषितम् config_सङ्ख्या_सम्बन्धि_CORES
    #दोषः Missing definition:  config_सङ्ख्या_सम्बन्धि_CORES must be defined अन्तर् FreeRTOSConfig.h
#यदि_अन्तः

#यदि ( config_सङ्ख्या_सम्बन्धि_CORES > 1 )
    #यदि_अपरिभाषितम् पत्तनम्_GET_CORE_ID
        #दोषः "Missing definition: portGET_CORE_ID() must be defined if in portmacro.h if configNUMBER_OF_CORES > 1"
    #यदि_अन्तः
    #यदि_अपरिभाषितम् पत्तनम्_YIELD_CORE
        #दोषः "Missing definition: portYIELD_CORE() must be defined if in portmacro.h if configNUMBER_OF_CORES > 1"
    #यदि_अन्तः
#नोचेद्यदि ( config_सङ्ख्या_सम्बन्धि_CORES == 1 )
    #अपरिभाषय पत्तनम्_GET_CORE_ID
    #परिभाषय पत्तनम्_GET_CORE_ID()    0
#अन्यथा
    #दोषः config_सङ्ख्या_सम्बन्धि_CORES must be defined गन्तव्यम् either 1 or > 1.
#यदि_अन्तः /* if ( configNUMBER_OF_CORES > 1 ) */

#यदि_अपरिभाषितम् config_अल्पिष्ठम्_राशिः_आकार
    #दोषः Missing definition:  config_अल्पिष्ठम्_राशिः_आकार must be defined अन्तर् FreeRTOSConfig.h.  config_अल्पिष्ठम्_राशिः_आकार परिभाषते the आकार (अन्तर् शब्दाः) सम्बन्धि the राशिः allocated गन्तव्यम् the idle task.  Refer गन्तव्यम् the demo project provided प्रति your पत्तनम् प्रति a suitable मानम्.
#यदि_अन्तः

#यदि_अपरिभाषितम् configMAX_PRIORITIES
    #दोषः Missing definition:  configMAX_PRIORITIES must be defined अन्तर् FreeRTOSConfig.h.  See the Configuration section सम्बन्धि the FreeRTOS API documentation प्रति details.
#यदि_अन्तः

#यदि configMAX_PRIORITIES < 1
    #दोषः configMAX_PRIORITIES must be defined गन्तव्यम् be greater than or equal गन्तव्यम् 1.
#यदि_अन्तः

#यदि_अपरिभाषितम् configUSE_PREEMPTION
    #दोषः Missing definition:  configUSE_PREEMPTION must be defined अन्तर् FreeRTOSConfig.h as either 1 or 0.  See the Configuration section सम्बन्धि the FreeRTOS API documentation प्रति details.
#यदि_अन्तः

#यदि_अपरिभाषितम् configUSE_IDLE_HOOK
    #दोषः Missing definition:  configUSE_IDLE_HOOK must be defined अन्तर् FreeRTOSConfig.h as either 1 or 0.  See the Configuration section सम्बन्धि the FreeRTOS API documentation प्रति details.
#यदि_अन्तः

#यदि_अपरिभाषितम् configUSE_TICK_HOOK
    #दोषः Missing definition:  configUSE_TICK_HOOK must be defined अन्तर् FreeRTOSConfig.h as either 1 or 0.  See the Configuration section सम्बन्धि the FreeRTOS API documentation प्रति details.
#यदि_अन्तः

#यदि_अपरिभाषितम् configUSE_16_BIT_TICKS
    #दोषः Missing definition:  configUSE_16_BIT_TICKS must be defined अन्तर् FreeRTOSConfig.h as either 1 or 0.  See the Configuration section सम्बन्धि the FreeRTOS API documentation प्रति details.
#यदि_अन्तः

#यदि_अपरिभाषितम् configUSE_CO_ROUTINES
    #परिभाषय configUSE_CO_ROUTINES    0
#यदि_अन्तः

#यदि_अपरिभाषितम् INCLUDE_v_Task_प्राथमिकता_समुच्चयः
    #परिभाषय INCLUDE_v_Task_प्राथमिकता_समुच्चयः    0
#यदि_अन्तः

#यदि_अपरिभाषितम् INCLUDE_ux_Task_प्राथमिकता_Get
    #परिभाषय INCLUDE_ux_Task_प्राथमिकता_Get    0
#यदि_अन्तः

#यदि_अपरिभाषितम् INCLUDE_vTaskDelete
    #परिभाषय INCLUDE_vTaskDelete    0
#यदि_अन्तः

#यदि_अपरिभाषितम् INCLUDE_vTaskSuspend
    #परिभाषय INCLUDE_vTaskSuspend    0
#यदि_अन्तः

#यदि_परिभाषितम् INCLUDE_xTaskDelayUntil
    #यदि_परिभाषितम् INCLUDE_vTaskDelayUntil

/* INCLUDE_vTaskDelayUntil was replaced by INCLUDE_xTaskDelayUntil.  Backward
 * compatibility is maintained if only one or the other is defined, but
 * there is a conflict if both are defined. */
        #दोषः INCLUDE_vTaskDelayUntil and INCLUDE_xTaskDelayUntil are both defined.  INCLUDE_vTaskDelayUntil is no longer required and should be removed
    #यदि_अन्तः
#यदि_अन्तः

#यदि_अपरिभाषितम् INCLUDE_xTaskDelayUntil
    #यदि_परिभाषितम् INCLUDE_vTaskDelayUntil

/* If INCLUDE_vTaskDelayUntil is set but INCLUDE_xTaskDelayUntil is not then
 * the project's FreeRTOSConfig.h probably pre-dates the introduction of
 * xTaskDelayUntil and setting INCLUDE_xTaskDelayUntil to whatever
 * INCLUDE_vTaskDelayUntil is set to will ensure backward compatibility.
 */
        #परिभाषय INCLUDE_xTaskDelayUntil    INCLUDE_vTaskDelayUntil
    #यदि_अन्तः
#यदि_अन्तः

#यदि_अपरिभाषितम् INCLUDE_xTaskDelayUntil
    #परिभाषय INCLUDE_xTaskDelayUntil    0
#यदि_अन्तः

#यदि_अपरिभाषितम् INCLUDE_vTaskDelay
    #परिभाषय INCLUDE_vTaskDelay    0
#यदि_अन्तः

#यदि_अपरिभाषितम् INCLUDE_xTaskGetIdleTaskHandle
    #परिभाषय INCLUDE_xTaskGetIdleTaskHandle    0
#यदि_अन्तः

#यदि_अपरिभाषितम् INCLUDE_xTaskAbortDelay
    #परिभाषय INCLUDE_xTaskAbortDelay    0
#यदि_अन्तः

#यदि_अपरिभाषितम् INCLUDE_xQueueGetMutexHolder
    #परिभाषय INCLUDE_xQueueGetMutexHolder    0
#यदि_अन्तः

#यदि_अपरिभाषितम् INCLUDE_xSemaphoreGetMutexHolder
    #परिभाषय INCLUDE_xSemaphoreGetMutexHolder    INCLUDE_xQueueGetMutexHolder
#यदि_अन्तः

#यदि_अपरिभाषितम् INCLUDE_xTaskGetHandle
    #परिभाषय INCLUDE_xTaskGetHandle    0
#यदि_अन्तः

#यदि_अपरिभाषितम् INCLUDE_ux_Task_Get_राशिः_उच्चः_Water_Mark
    #परिभाषय INCLUDE_ux_Task_Get_राशिः_उच्चः_Water_Mark    0
#यदि_अन्तः

#यदि_अपरिभाषितम् INCLUDE_ux_Task_Get_राशिः_उच्चः_Water_Mark2
    #परिभाषय INCLUDE_ux_Task_Get_राशिः_उच्चः_Water_Mark2    0
#यदि_अन्तः

#यदि_अपरिभाषितम् INCLUDE_eTaskGetState
    #परिभाषय INCLUDE_eTaskGetState    0
#यदि_अन्तः

#यदि_अपरिभाषितम् INCLUDE_xTaskResumeFromISR
    #परिभाषय INCLUDE_xTaskResumeFromISR    1
#यदि_अन्तः

#यदि_अपरिभाषितम् INCLUDE_xTimerPendFunctionCall
    #परिभाषय INCLUDE_xTimerPendFunctionCall    0
#यदि_अन्तः

#यदि_अपरिभाषितम् INCLUDE_xTaskGetSchedulerState
    #परिभाषय INCLUDE_xTaskGetSchedulerState    0
#यदि_अन्तः

#यदि_अपरिभाषितम् INCLUDE_xTaskGetCurrentTaskHandle
    #परिभाषय INCLUDE_xTaskGetCurrentTaskHandle    1
#यदि_अन्तः

#यदि configUSE_CO_ROUTINES != 0
    #यदि_अपरिभाषितम् configMAX_CO_ROUTINE_PRIORITIES
        #दोषः configMAX_CO_ROUTINE_PRIORITIES must be greater than or equal गन्तव्यम् 1.
    #यदि_अन्तः
#यदि_अन्तः

#यदि_अपरिभाषितम् configUSE_DAEMON_TASK_STARTUP_HOOK
    #परिभाषय configUSE_DAEMON_TASK_STARTUP_HOOK    0
#यदि_अन्तः

#यदि_अपरिभाषितम् config_USE_APPLICATION_TASK_चिह्नकम्
    #परिभाषय config_USE_APPLICATION_TASK_चिह्नकम्    0
#यदि_अन्तः

#यदि_अपरिभाषितम् config_अङ्कः_THREAD_LOCAL_STORAGE_सूचकाः
    #परिभाषय config_अङ्कः_THREAD_LOCAL_STORAGE_सूचकाः    0
#यदि_अन्तः

#यदि_अपरिभाषितम् configUSE_RECURSIVE_MUTEXES
    #परिभाषय configUSE_RECURSIVE_MUTEXES    0
#यदि_अन्तः

#यदि_अपरिभाषितम् configUSE_MUTEXES
    #परिभाषय configUSE_MUTEXES    0
#यदि_अन्तः

#यदि_अपरिभाषितम् configUSE_TIMERS
    #परिभाषय configUSE_TIMERS    0
#यदि_अन्तः

#यदि_अपरिभाषितम् configUSE_COUNTING_SEMAPHORES
    #परिभाषय configUSE_COUNTING_SEMAPHORES    0
#यदि_अन्तः

#यदि_अपरिभाषितम् configUSE_ALTERNATIVE_API
    #परिभाषय configUSE_ALTERNATIVE_API    0
#यदि_अन्तः

#यदि_अपरिभाषितम् पत्तनम्_CRITICAL_NESTING_अन्तर्_TCB
    #परिभाषय पत्तनम्_CRITICAL_NESTING_अन्तर्_TCB    0
#यदि_अन्तः

#यदि_अपरिभाषितम् configMAX_TASK_NAME_LEN
    #परिभाषय configMAX_TASK_NAME_LEN    16
#यदि_अन्तः

#यदि_अपरिभाषितम् configIDLE_SHOULD_YIELD
    #परिभाषय configIDLE_SHOULD_YIELD    1
#यदि_अन्तः

#यदि configMAX_TASK_NAME_LEN < 1
    #दोषः configMAX_TASK_NAME_LEN must be समुच्चयः गन्तव्यम् a minimum सम्बन्धि 1 अन्तर् FreeRTOSConfig.h
#यदि_अन्तः

#यदि_अपरिभाषितम् configASSERT
    #परिभाषय configASSERT( x )
    #परिभाषय configASSERT_DEFINED    0
#अन्यथा
    #परिभाषय configASSERT_DEFINED    1
#यदि_अन्तः

/* configPRECONDITION should be defined as configASSERT.
 * The CBMC proofs need a way to track assumptions and assertions.
 * A configPRECONDITION statement should express an implicit invariant or
 * assumption made.  A configASSERT statement should express an invariant that must
 * hold explicit before calling the code. */
#यदि_अपरिभाषितम् configPRECONDITION
    #परिभाषय configPRECONDITION( X )    configASSERT( X )
    #परिभाषय configPRECONDITION_DEFINED    0
#अन्यथा
    #परिभाषय configPRECONDITION_DEFINED    1
#यदि_अन्तः

#यदि_अपरिभाषितम् पत्तनम्_स्मृतिः_BARRIER
    #परिभाषय पत्तनम्_स्मृतिः_BARRIER()
#यदि_अन्तः

#यदि_अपरिभाषितम् पत्तनम्_SOFTWARE_BARRIER
    #परिभाषय पत्तनम्_SOFTWARE_BARRIER()
#यदि_अन्तः

/* The timers module relies on xTaskGetSchedulerState(). */
#यदि configUSE_TIMERS == 1

    #यदि_अपरिभाषितम् config_TIMER_TASK_प्राथमिकता
        #दोषः If configUSE_TIMERS is समुच्चयः गन्तव्यम् 1 then config_TIMER_TASK_प्राथमिकता must also be defined.
    #यदि_अन्तः /* configTIMER_TASK_PRIORITY */

    #यदि_अपरिभाषितम् config_TIMER_QUEUE_दीर्घता
        #दोषः If configUSE_TIMERS is समुच्चयः गन्तव्यम् 1 then config_TIMER_QUEUE_दीर्घता must also be defined.
    #यदि_अन्तः /* configTIMER_QUEUE_LENGTH */

    #यदि_अपरिभाषितम् config_TIMER_TASK_राशिः_गभीरता
        #दोषः If configUSE_TIMERS is समुच्चयः गन्तव्यम् 1 then config_TIMER_TASK_राशिः_गभीरता must also be defined.
    #यदि_अन्तः /* configTIMER_TASK_STACK_DEPTH */

#यदि_अन्तः /* configUSE_TIMERS */

#यदि_अपरिभाषितम् पत्तनम्_समुच्चयः_व्यवधानम्_आवरणम्_FROM_ISR
    #परिभाषय पत्तनम्_समुच्चयः_व्यवधानम्_आवरणम्_FROM_ISR()    0
#यदि_अन्तः

#यदि_अपरिभाषितम् पत्तनम्_CLEAR_व्यवधानम्_आवरणम्_FROM_ISR
    #परिभाषय पत्तनम्_CLEAR_व्यवधानम्_आवरणम्_FROM_ISR( ux_Saved_स्थितिः_मानम् )    ( शून्य ) ( ux_Saved_स्थितिः_मानम् )
#यदि_अन्तः

#यदि_अपरिभाषितम् पत्तनम्_CLEAN_ऊर्ध्वम्_TCB
    #परिभाषय पत्तनम्_CLEAN_ऊर्ध्वम्_TCB( pxTCB )    ( शून्य ) ( pxTCB )
#यदि_अन्तः

#यदि_अपरिभाषितम् पत्तनम्_PRE_TASK_DELETE_HOOK
    #परिभाषय पत्तनम्_PRE_TASK_DELETE_HOOK( pv_Task_गन्तव्यम्_Delete, pxYieldPending )
#यदि_अन्तः

#यदि_अपरिभाषितम् पत्तनम्_सज्जता_TCB
    #परिभाषय पत्तनम्_सज्जता_TCB( pxTCB )    ( शून्य ) ( pxTCB )
#यदि_अन्तः

#यदि_अपरिभाषितम् config_QUEUE_REGISTRY_आकार
    #परिभाषय config_QUEUE_REGISTRY_आकार    0U
#यदि_अन्तः

#यदि ( config_QUEUE_REGISTRY_आकार < 1 )
    #परिभाषय v_Queue_सङ्कलय_गन्तव्यम्_Registry( xQueue, pcName )
    #परिभाषय vQueueUnregisterQueue( xQueue )
    #परिभाषय pcQueueGetName( xQueue )
#यदि_अन्तः

#यदि_अपरिभाषितम् configUSE_MINI_LIST_ITEM
    #परिभाषय configUSE_MINI_LIST_ITEM    1
#यदि_अन्तः

#यदि_अपरिभाषितम् पत्तनम्_POINTER_आकार_TYPE
    #परिभाषय पत्तनम्_POINTER_आकार_TYPE    uint32_t
#यदि_अन्तः

/* Remove any unused trace macros. */
#यदि_अपरिभाषितम् traceSTART

/* Used to perform any necessary initialisation - for example, open a file
 * into which trace is to be written. */
    #परिभाषय traceSTART()
#यदि_अन्तः

#यदि_अपरिभाषितम् trace_अन्तः

/* Use to close a trace, for example close a file into which trace has been
 * written. */
    #परिभाषय trace_अन्तः()
#यदि_अन्तः

#यदि_अपरिभाषितम् trace_TASK_SWITCHED_अन्तर्

/* Called after a task has been selected to run.  pxCurrentTCB holds a pointer
 * to the task control block of the selected task. */
    #परिभाषय trace_TASK_SWITCHED_अन्तर्()
#यदि_अन्तः

#यदि_अपरिभाषितम् trace_INCREASE_TICK_गणनम्

/* Called before stepping the tick count after waking from tickless idle
 * sleep. */
    #परिभाषय trace_INCREASE_TICK_गणनम्( x )
#यदि_अन्तः

#यदि_अपरिभाषितम् trace_नीचः_POWER_IDLE_BEGIN
    /* Called immediately before entering tickless idle. */
    #परिभाषय trace_नीचः_POWER_IDLE_BEGIN()
#यदि_अन्तः

#यदि_अपरिभाषितम् trace_नीचः_POWER_IDLE_अन्तः
    /* Called when returning to the Idle task after a tickless idle. */
    #परिभाषय trace_नीचः_POWER_IDLE_अन्तः()
#यदि_अन्तः

#यदि_अपरिभाषितम् trace_TASK_SWITCHED_बहिर्गमनम्

/* Called before a task has been selected to run.  pxCurrentTCB holds a pointer
 * to the task control block of the task being switched out. */
    #परिभाषय trace_TASK_SWITCHED_बहिर्गमनम्()
#यदि_अन्तः

#यदि_अपरिभाषितम् trace_TASK_प्राथमिकता_INHERIT

/* Called when a task attempts to take a mutex that is already held by a
 * lower priority task.  pxTCBOfMutexHolder is a pointer to the TCB of the task
 * that holds the mutex.  uxInheritedPriority is the priority the mutex holder
 * will inherit (the priority of the task that is attempting to obtain the
 * muted. */
    #परिभाषय trace_TASK_प्राथमिकता_INHERIT( pxTCBOfMutexHolder, ux_Inherited_प्राथमिकता )
#यदि_अन्तः

#यदि_अपरिभाषितम् trace_TASK_प्राथमिकता_DISINHERIT

/* Called when a task releases a mutex, the holding of which had resulted in
 * the task inheriting the priority of a higher priority task.
 * pxTCBOfMutexHolder is a pointer to the TCB of the task that is releasing the
 * mutex.  uxOriginalPriority is the task's configured (base) priority. */
    #परिभाषय trace_TASK_प्राथमिकता_DISINHERIT( pxTCBOfMutexHolder, ux_Original_प्राथमिकता )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceBLOCKING_ON_QUEUE_RECEIVE

/* Task is about to block because it cannot read from a
 * queue/mutex/semaphore.  pxQueue is a pointer to the queue/mutex/semaphore
 * upon which the read was attempted.  pxCurrentTCB points to the TCB of the
 * task that attempted the read. */
    #परिभाषय traceBLOCKING_ON_QUEUE_RECEIVE( pxQueue )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceBLOCKING_ON_QUEUE_PEEK

/* Task is about to block because it cannot read from a
 * queue/mutex/semaphore.  pxQueue is a pointer to the queue/mutex/semaphore
 * upon which the read was attempted.  pxCurrentTCB points to the TCB of the
 * task that attempted the read. */
    #परिभाषय traceBLOCKING_ON_QUEUE_PEEK( pxQueue )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceBLOCKING_ON_QUEUE_SEND

/* Task is about to block because it cannot write to a
 * queue/mutex/semaphore.  pxQueue is a pointer to the queue/mutex/semaphore
 * upon which the write was attempted.  pxCurrentTCB points to the TCB of the
 * task that attempted the write. */
    #परिभाषय traceBLOCKING_ON_QUEUE_SEND( pxQueue )
#यदि_अन्तः

#यदि_अपरिभाषितम् config_CHECK_FOR_राशिः_अतिप्रवाहः
    #परिभाषय config_CHECK_FOR_राशिः_अतिप्रवाहः    0
#यदि_अन्तः

#यदि_अपरिभाषितम् config_लेखः_राशिः_उच्चः_स्थानाङ्कः
    #परिभाषय config_लेखः_राशिः_उच्चः_स्थानाङ्कः    0
#यदि_अन्तः

#यदि_अपरिभाषितम् configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H
    #परिभाषय configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H    0
#यदि_अन्तः

/* The following event macros are embedded in the kernel API calls. */

#यदि_अपरिभाषितम् trace_MOVED_TASK_गन्तव्यम्_READY_STATE
    #परिभाषय trace_MOVED_TASK_गन्तव्यम्_READY_STATE( pxTCB )
#यदि_अन्तः

#यदि_अपरिभाषितम् trace_POST_MOVED_TASK_गन्तव्यम्_READY_STATE
    #परिभाषय trace_POST_MOVED_TASK_गन्तव्यम्_READY_STATE( pxTCB )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceQUEUE_CREATE
    #परिभाषय traceQUEUE_CREATE( pxNewQueue )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceQUEUE_CREATE_FAILED
    #परिभाषय traceQUEUE_CREATE_FAILED( ucQueueType )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceCREATE_MUTEX
    #परिभाषय traceCREATE_MUTEX( pxNewQueue )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceCREATE_MUTEX_FAILED
    #परिभाषय traceCREATE_MUTEX_FAILED()
#यदि_अन्तः

#यदि_अपरिभाषितम् traceGIVE_MUTEX_RECURSIVE
    #परिभाषय traceGIVE_MUTEX_RECURSIVE( pxMutex )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceGIVE_MUTEX_RECURSIVE_FAILED
    #परिभाषय traceGIVE_MUTEX_RECURSIVE_FAILED( pxMutex )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceTAKE_MUTEX_RECURSIVE
    #परिभाषय traceTAKE_MUTEX_RECURSIVE( pxMutex )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceTAKE_MUTEX_RECURSIVE_FAILED
    #परिभाषय traceTAKE_MUTEX_RECURSIVE_FAILED( pxMutex )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceCREATE_COUNTING_SEMAPHORE
    #परिभाषय traceCREATE_COUNTING_SEMAPHORE()
#यदि_अन्तः

#यदि_अपरिभाषितम् traceCREATE_COUNTING_SEMAPHORE_FAILED
    #परिभाषय traceCREATE_COUNTING_SEMAPHORE_FAILED()
#यदि_अन्तः

#यदि_अपरिभाषितम् trace_QUEUE_समुच्चयः_SEND
    #परिभाषय trace_QUEUE_समुच्चयः_SEND    traceQUEUE_SEND
#यदि_अन्तः

#यदि_अपरिभाषितम् traceQUEUE_SEND
    #परिभाषय traceQUEUE_SEND( pxQueue )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceQUEUE_SEND_FAILED
    #परिभाषय traceQUEUE_SEND_FAILED( pxQueue )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceQUEUE_RECEIVE
    #परिभाषय traceQUEUE_RECEIVE( pxQueue )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceQUEUE_PEEK
    #परिभाषय traceQUEUE_PEEK( pxQueue )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceQUEUE_PEEK_FAILED
    #परिभाषय traceQUEUE_PEEK_FAILED( pxQueue )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceQUEUE_PEEK_FROM_ISR
    #परिभाषय traceQUEUE_PEEK_FROM_ISR( pxQueue )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceQUEUE_RECEIVE_FAILED
    #परिभाषय traceQUEUE_RECEIVE_FAILED( pxQueue )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceQUEUE_SEND_FROM_ISR
    #परिभाषय traceQUEUE_SEND_FROM_ISR( pxQueue )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceQUEUE_SEND_FROM_ISR_FAILED
    #परिभाषय traceQUEUE_SEND_FROM_ISR_FAILED( pxQueue )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceQUEUE_RECEIVE_FROM_ISR
    #परिभाषय traceQUEUE_RECEIVE_FROM_ISR( pxQueue )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceQUEUE_RECEIVE_FROM_ISR_FAILED
    #परिभाषय traceQUEUE_RECEIVE_FROM_ISR_FAILED( pxQueue )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceQUEUE_PEEK_FROM_ISR_FAILED
    #परिभाषय traceQUEUE_PEEK_FROM_ISR_FAILED( pxQueue )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceQUEUE_DELETE
    #परिभाषय traceQUEUE_DELETE( pxQueue )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceTASK_CREATE
    #परिभाषय traceTASK_CREATE( pxNewTCB )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceTASK_CREATE_FAILED
    #परिभाषय traceTASK_CREATE_FAILED()
#यदि_अन्तः

#यदि_अपरिभाषितम् traceTASK_DELETE
    #परिभाषय traceTASK_DELETE( px_Task_गन्तव्यम्_Delete )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceTASK_DELAY_UNTIL
    #परिभाषय traceTASK_DELAY_UNTIL( x )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceTASK_DELAY
    #परिभाषय traceTASK_DELAY()
#यदि_अन्तः

#यदि_अपरिभाषितम् trace_TASK_प्राथमिकता_समुच्चयः
    #परिभाषय trace_TASK_प्राथमिकता_समुच्चयः( pxTask, ux_New_प्राथमिकता )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceTASK_SUSPEND
    #परिभाषय traceTASK_SUSPEND( px_Task_गन्तव्यम्_Suspend )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceTASK_RESUME
    #परिभाषय traceTASK_RESUME( px_Task_गन्तव्यम्_Resume )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceTASK_RESUME_FROM_ISR
    #परिभाषय traceTASK_RESUME_FROM_ISR( px_Task_गन्तव्यम्_Resume )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceTASK_INCREMENT_TICK
    #परिभाषय traceTASK_INCREMENT_TICK( x_Tick_गणनम् )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceTIMER_CREATE
    #परिभाषय traceTIMER_CREATE( pxNewTimer )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceTIMER_CREATE_FAILED
    #परिभाषय traceTIMER_CREATE_FAILED()
#यदि_अन्तः

#यदि_अपरिभाषितम् traceTIMER_COMMAND_SEND
    #परिभाषय traceTIMER_COMMAND_SEND( xTimer, x_सन्देशः_ID, x_सन्देशः_मानम्_मानम्, xReturn )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceTIMER_EXPIRED
    #परिभाषय traceTIMER_EXPIRED( pxTimer )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceTIMER_COMMAND_RECEIVED
    #परिभाषय traceTIMER_COMMAND_RECEIVED( pxTimer, x_सन्देशः_ID, x_सन्देशः_मानम् )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceMALLOC
    #परिभाषय traceMALLOC( pv_स्थानाङ्कः, ui_आकार )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceFREE
    #परिभाषय traceFREE( pv_स्थानाङ्कः, ui_आकार )
#यदि_अन्तः

#यदि_अपरिभाषितम् trace_EVENT_गणः_CREATE
    #परिभाषय trace_EVENT_गणः_CREATE( x_Event_गणः )
#यदि_अन्तः

#यदि_अपरिभाषितम् trace_EVENT_गणः_CREATE_FAILED
    #परिभाषय trace_EVENT_गणः_CREATE_FAILED()
#यदि_अन्तः

#यदि_अपरिभाषितम् trace_EVENT_गणः_SYNC_खण्डम्
    #परिभाषय trace_EVENT_गणः_SYNC_खण्डम्( x_Event_गणः, ux_द्वयङ्काः_गन्तव्यम्_समुच्चयः, ux_द्वयङ्काः_गन्तव्यम्_Wait_For )
#यदि_अन्तः

#यदि_अपरिभाषितम् trace_EVENT_गणः_SYNC_अन्तः
    #परिभाषय trace_EVENT_गणः_SYNC_अन्तः( x_Event_गणः, ux_द्वयङ्काः_गन्तव्यम्_समुच्चयः, ux_द्वयङ्काः_गन्तव्यम्_Wait_For, xTimeoutOccurred )    ( शून्य ) ( xTimeoutOccurred )
#यदि_अन्तः

#यदि_अपरिभाषितम् trace_EVENT_गणः_WAIT_द्वयङ्काः_खण्डम्
    #परिभाषय trace_EVENT_गणः_WAIT_द्वयङ्काः_खण्डम्( x_Event_गणः, ux_द्वयङ्काः_गन्तव्यम्_Wait_For )
#यदि_अन्तः

#यदि_अपरिभाषितम् trace_EVENT_गणः_WAIT_द्वयङ्काः_अन्तः
    #परिभाषय trace_EVENT_गणः_WAIT_द्वयङ्काः_अन्तः( x_Event_गणः, ux_द्वयङ्काः_गन्तव्यम्_Wait_For, xTimeoutOccurred )    ( शून्य ) ( xTimeoutOccurred )
#यदि_अन्तः

#यदि_अपरिभाषितम् trace_EVENT_गणः_CLEAR_द्वयङ्काः
    #परिभाषय trace_EVENT_गणः_CLEAR_द्वयङ्काः( x_Event_गणः, ux_द्वयङ्काः_गन्तव्यम्_Clear )
#यदि_अन्तः

#यदि_अपरिभाषितम् trace_EVENT_गणः_CLEAR_द्वयङ्काः_FROM_ISR
    #परिभाषय trace_EVENT_गणः_CLEAR_द्वयङ्काः_FROM_ISR( x_Event_गणः, ux_द्वयङ्काः_गन्तव्यम्_Clear )
#यदि_अन्तः

#यदि_अपरिभाषितम् trace_EVENT_गणः_समुच्चयः_द्वयङ्काः
    #परिभाषय trace_EVENT_गणः_समुच्चयः_द्वयङ्काः( x_Event_गणः, ux_द्वयङ्काः_गन्तव्यम्_समुच्चयः )
#यदि_अन्तः

#यदि_अपरिभाषितम् trace_EVENT_गणः_समुच्चयः_द्वयङ्काः_FROM_ISR
    #परिभाषय trace_EVENT_गणः_समुच्चयः_द्वयङ्काः_FROM_ISR( x_Event_गणः, ux_द्वयङ्काः_गन्तव्यम्_समुच्चयः )
#यदि_अन्तः

#यदि_अपरिभाषितम् trace_EVENT_गणः_DELETE
    #परिभाषय trace_EVENT_गणः_DELETE( x_Event_गणः )
#यदि_अन्तः

#यदि_अपरिभाषितम् tracePEND_FUNC_CALL
    #परिभाषय tracePEND_FUNC_CALL( x_Function_गन्तव्यम्_Pend, pvParameter1, ulParameter2, ret )
#यदि_अन्तः

#यदि_अपरिभाषितम् tracePEND_FUNC_CALL_FROM_ISR
    #परिभाषय tracePEND_FUNC_CALL_FROM_ISR( x_Function_गन्तव्यम्_Pend, pvParameter1, ulParameter2, ret )
#यदि_अन्तः

#यदि_अपरिभाषितम् trace_QUEUE_REGISTRY_सङ्कलय
    #परिभाषय trace_QUEUE_REGISTRY_सङ्कलय( xQueue, pcQueueName )
#यदि_अन्तः

#यदि_अपरिभाषितम् trace_TASK_NOTIFY_TAKE_खण्डम्
    #परिभाषय trace_TASK_NOTIFY_TAKE_खण्डम्( ux_सूचकांक_गन्तव्यम्_Wait )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceTASK_NOTIFY_TAKE
    #परिभाषय traceTASK_NOTIFY_TAKE( ux_सूचकांक_गन्तव्यम्_Wait )
#यदि_अन्तः

#यदि_अपरिभाषितम् trace_TASK_NOTIFY_WAIT_खण्डम्
    #परिभाषय trace_TASK_NOTIFY_WAIT_खण्डम्( ux_सूचकांक_गन्तव्यम्_Wait )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceTASK_NOTIFY_WAIT
    #परिभाषय traceTASK_NOTIFY_WAIT( ux_सूचकांक_गन्तव्यम्_Wait )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceTASK_NOTIFY
    #परिभाषय traceTASK_NOTIFY( ux_सूचकांक_गन्तव्यम्_Notify )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceTASK_NOTIFY_FROM_ISR
    #परिभाषय traceTASK_NOTIFY_FROM_ISR( ux_सूचकांक_गन्तव्यम्_Notify )
#यदि_अन्तः

#यदि_अपरिभाषितम् traceTASK_NOTIFY_GIVE_FROM_ISR
    #परिभाषय traceTASK_NOTIFY_GIVE_FROM_ISR( ux_सूचकांक_गन्तव्यम्_Notify )
#यदि_अन्तः

#यदि_अपरिभाषितम् trace_प्रवाहः_बफर_CREATE_FAILED
    #परिभाषय trace_प्रवाहः_बफर_CREATE_FAILED( x_Is_सन्देशः_बफर )
#यदि_अन्तः

#यदि_अपरिभाषितम् trace_प्रवाहः_बफर_CREATE_STATIC_FAILED
    #परिभाषय trace_प्रवाहः_बफर_CREATE_STATIC_FAILED( xReturn, x_Is_सन्देशः_बफर )
#यदि_अन्तः

#यदि_अपरिभाषितम् trace_प्रवाहः_बफर_CREATE
    #परिभाषय trace_प्रवाहः_बफर_CREATE( px_प्रवाहः_बफर, x_Is_सन्देशः_बफर )
#यदि_अन्तः

#यदि_अपरिभाषितम् trace_प्रवाहः_बफर_DELETE
    #परिभाषय trace_प्रवाहः_बफर_DELETE( x_प्रवाहः_बफर )
#यदि_अन्तः

#यदि_अपरिभाषितम् trace_प्रवाहः_बफर_RESET
    #परिभाषय trace_प्रवाहः_बफर_RESET( x_प्रवाहः_बफर )
#यदि_अन्तः

#यदि_अपरिभाषितम् trace_BLOCKING_ON_प्रवाहः_बफर_SEND
    #परिभाषय trace_BLOCKING_ON_प्रवाहः_बफर_SEND( x_प्रवाहः_बफर )
#यदि_अन्तः

#यदि_अपरिभाषितम् trace_प्रवाहः_बफर_SEND
    #परिभाषय trace_प्रवाहः_बफर_SEND( x_प्रवाहः_बफर, x_अष्टकानि_प्रेषितम् )
#यदि_अन्तः

#यदि_अपरिभाषितम् trace_प्रवाहः_बफर_SEND_FAILED
    #परिभाषय trace_प्रवाहः_बफर_SEND_FAILED( x_प्रवाहः_बफर )
#यदि_अन्तः

#यदि_अपरिभाषितम् trace_प्रवाहः_बफर_SEND_FROM_ISR
    #परिभाषय trace_प्रवाहः_बफर_SEND_FROM_ISR( x_प्रवाहः_बफर, x_अष्टकानि_प्रेषितम् )
#यदि_अन्तः

#यदि_अपरिभाषितम् trace_BLOCKING_ON_प्रवाहः_बफर_RECEIVE
    #परिभाषय trace_BLOCKING_ON_प्रवाहः_बफर_RECEIVE( x_प्रवाहः_बफर )
#यदि_अन्तः

#यदि_अपरिभाषितम् trace_प्रवाहः_बफर_RECEIVE
    #परिभाषय trace_प्रवाहः_बफर_RECEIVE( x_प्रवाहः_बफर, x_Received_दीर्घता )
#यदि_अन्तः

#यदि_अपरिभाषितम् trace_प्रवाहः_बफर_RECEIVE_FAILED
    #परिभाषय trace_प्रवाहः_बफर_RECEIVE_FAILED( x_प्रवाहः_बफर )
#यदि_अन्तः

#यदि_अपरिभाषितम् trace_प्रवाहः_बफर_RECEIVE_FROM_ISR
    #परिभाषय trace_प्रवाहः_बफर_RECEIVE_FROM_ISR( x_प्रवाहः_बफर, x_Received_दीर्घता )
#यदि_अन्तः

#यदि_अपरिभाषितम् config_GENERATE_सततम्_TIME_STATS
    #परिभाषय config_GENERATE_सततम्_TIME_STATS    0
#यदि_अन्तः

#यदि ( config_GENERATE_सततम्_TIME_STATS == 1 )

    #यदि_अपरिभाषितम् पत्तनम्_CONFIGURE_TIMER_FOR_सततम्_TIME_STATS
        #दोषः If config_GENERATE_सततम्_TIME_STATS is defined then पत्तनम्_CONFIGURE_TIMER_FOR_सततम्_TIME_STATS must also be defined.  पत्तनम्_CONFIGURE_TIMER_FOR_सततम्_TIME_STATS should call a पत्तनम् layer function गन्तव्यम् सज्जता a peripheral timer/गणक that can then be प्रयुक्तम् as the सततम् time गणक time आधारः.
    #यदि_अन्तः /* portCONFIGURE_TIMER_FOR_RUN_TIME_STATS */

    #यदि_अपरिभाषितम् पत्तनम्_GET_सततम्_TIME_गणक_मानम्
        #यदि_अपरिभाषितम् पत्तनम्_ALT_GET_सततम्_TIME_गणक_मानम्
            #दोषः If config_GENERATE_सततम्_TIME_STATS is defined then either पत्तनम्_GET_सततम्_TIME_गणक_मानम् or पत्तनम्_ALT_GET_सततम्_TIME_गणक_मानम् must also be defined.  See the examples provided and the FreeRTOS web site प्रति अधिकम् information.
        #यदि_अन्तः /* portALT_GET_RUN_TIME_COUNTER_VALUE */
    #यदि_अन्तः /* portGET_RUN_TIME_COUNTER_VALUE */

#यदि_अन्तः /* configGENERATE_RUN_TIME_STATS */

#यदि_अपरिभाषितम् पत्तनम्_CONFIGURE_TIMER_FOR_सततम्_TIME_STATS
    #परिभाषय पत्तनम्_CONFIGURE_TIMER_FOR_सततम्_TIME_STATS()
#यदि_अन्तः

#यदि_अपरिभाषितम् configUSE_MALLOC_FAILED_HOOK
    #परिभाषय configUSE_MALLOC_FAILED_HOOK    0
#यदि_अन्तः

#यदि_अपरिभाषितम् पत्तनम्_PRIVILEGE_BIT
    #परिभाषय पत्तनम्_PRIVILEGE_BIT    ( ( UBaseType_t ) 0x00 )
#यदि_अन्तः

#यदि_अपरिभाषितम् पत्तनम्_YIELD_WITHIN_API
    #परिभाषय पत्तनम्_YIELD_WITHIN_API    पत्तनम्_YIELD
#यदि_अन्तः

#यदि_अपरिभाषितम् पत्तनम्_SUPPRESS_TICKS_AND_SLEEP
    #परिभाषय पत्तनम्_SUPPRESS_TICKS_AND_SLEEP( xExpectedIdleTime )
#यदि_अन्तः

#यदि_अपरिभाषितम् configEXPECTED_IDLE_TIME_BEFORE_SLEEP
    #परिभाषय configEXPECTED_IDLE_TIME_BEFORE_SLEEP    2
#यदि_अन्तः

#यदि configEXPECTED_IDLE_TIME_BEFORE_SLEEP < 2
    #दोषः configEXPECTED_IDLE_TIME_BEFORE_SLEEP must not be less than 2
#यदि_अन्तः

#यदि_अपरिभाषितम् configUSE_TICKLESS_IDLE
    #परिभाषय configUSE_TICKLESS_IDLE    0
#यदि_अन्तः

#यदि_अपरिभाषितम् configPRE_SUPPRESS_TICKS_AND_SLEEP_PROCESSING
    #परिभाषय configPRE_SUPPRESS_TICKS_AND_SLEEP_PROCESSING( x )
#यदि_अन्तः

#यदि_अपरिभाषितम् configPRE_SLEEP_PROCESSING
    #परिभाषय configPRE_SLEEP_PROCESSING( x )
#यदि_अन्तः

#यदि_अपरिभाषितम् configPOST_SLEEP_PROCESSING
    #परिभाषय configPOST_SLEEP_PROCESSING( x )
#यदि_अन्तः

#यदि_अपरिभाषितम् configUSE_QUEUE_SETS
    #परिभाषय configUSE_QUEUE_SETS    0
#यदि_अन्तः

#यदि_अपरिभाषितम् पत्तनम्_TASK_उपयुङ्क्ते_FLOATING_POINT
    #परिभाषय पत्तनम्_TASK_उपयुङ्क्ते_FLOATING_POINT()
#यदि_अन्तः

#यदि_अपरिभाषितम् पत्तनम्_ALLOCATE_SECURE_CONTEXT
    #परिभाषय पत्तनम्_ALLOCATE_SECURE_CONTEXT( ul_Secure_राशिः_आकार )
#यदि_अन्तः

#यदि_अपरिभाषितम् पत्तनम्_DONT_DISCARD
    #परिभाषय पत्तनम्_DONT_DISCARD
#यदि_अन्तः

#यदि_अपरिभाषितम् configUSE_TIME_SLICING
    #परिभाषय configUSE_TIME_SLICING    1
#यदि_अन्तः

#यदि_अपरिभाषितम् configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS
    #परिभाषय configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS    0
#यदि_अन्तः

#यदि_अपरिभाषितम् configUSE_STATS_FORMATTING_FUNCTIONS
    #परिभाषय configUSE_STATS_FORMATTING_FUNCTIONS    0
#यदि_अन्तः

#यदि_अपरिभाषितम् पत्तनम्_ASSERT_IF_व्यवधानम्_प्राथमिकता_INVALID
    #परिभाषय पत्तनम्_ASSERT_IF_व्यवधानम्_प्राथमिकता_INVALID()
#यदि_अन्तः

#यदि_अपरिभाषितम् configUSE_TRACE_FACILITY
    #परिभाषय configUSE_TRACE_FACILITY    0
#यदि_अन्तः

#यदि_अपरिभाषितम् mtCOVERAGE_TEST_MARKER
    #परिभाषय mtCOVERAGE_TEST_MARKER()
#यदि_अन्तः

#यदि_अपरिभाषितम् mtCOVERAGE_TEST_DELAY
    #परिभाषय mtCOVERAGE_TEST_DELAY()
#यदि_अन्तः

#यदि_अपरिभाषितम् पत्तनम्_ASSERT_IF_अन्तर्_ISR
    #परिभाषय पत्तनम्_ASSERT_IF_अन्तर्_ISR()
#यदि_अन्तः

#यदि_अपरिभाषितम् config_USE_पत्तनम्_OPTIMISED_TASK_SELECTION
    #परिभाषय config_USE_पत्तनम्_OPTIMISED_TASK_SELECTION    0
#यदि_अन्तः

#यदि_अपरिभाषितम् configAPPLICATION_ALLOCATED_HEAP
    #परिभाषय configAPPLICATION_ALLOCATED_HEAP    0
#यदि_अन्तः

#यदि_अपरिभाषितम् configUSE_TASK_NOTIFICATIONS
    #परिभाषय configUSE_TASK_NOTIFICATIONS    1
#यदि_अन्तः

#यदि_अपरिभाषितम् configTASK_NOTIFICATION_ARRAY_ENTRIES
    #परिभाषय configTASK_NOTIFICATION_ARRAY_ENTRIES    1
#यदि_अन्तः

#यदि configTASK_NOTIFICATION_ARRAY_ENTRIES < 1
    #दोषः configTASK_NOTIFICATION_ARRAY_ENTRIES must be at न्यूनतमम् 1
#यदि_अन्तः

#यदि_अपरिभाषितम् configUSE_POSIX_ERRNO
    #परिभाषय configUSE_POSIX_ERRNO    0
#यदि_अन्तः

#यदि_अपरिभाषितम् configUSE_SB_COMPLETED_CALLBACK

/* By default per-instance callbacks are not enabled for stream buffer or message buffer. */
    #परिभाषय configUSE_SB_COMPLETED_CALLBACK    0
#यदि_अन्तः

#यदि_अपरिभाषितम् पत्तनम्_TICK_TYPE_IS_ATOMIC
    #परिभाषय पत्तनम्_TICK_TYPE_IS_ATOMIC    0
#यदि_अन्तः

#यदि_अपरिभाषितम् config_SUPPORT_STATIC_आवण्टनम्
    /* Defaults to 0 for backward compatibility. */
    #परिभाषय config_SUPPORT_STATIC_आवण्टनम्    0
#यदि_अन्तः

#यदि_अपरिभाषितम् config_SUPPORT_DYNAMIC_आवण्टनम्
    /* Defaults to 1 for backward compatibility. */
    #परिभाषय config_SUPPORT_DYNAMIC_आवण्टनम्    1
#यदि_अन्तः

#यदि ( ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 ) && ( config_SUPPORT_DYNAMIC_आवण्टनम् != 1 ) )
    #दोषः configUSE_STATS_FORMATTING_FUNCTIONS cannot be प्रयुक्तम् without dynamic आवण्टनम्, but config_SUPPORT_DYNAMIC_आवण्टनम् is not समुच्चयः गन्तव्यम् 1.
#यदि_अन्तः

#यदि ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 )
    #यदि ( ( configUSE_TRACE_FACILITY != 1 ) && ( config_GENERATE_सततम्_TIME_STATS != 1 ) )
        #दोषः configUSE_STATS_FORMATTING_FUNCTIONS is 1 but the functions it enables are not प्रयुक्तम् because neither configUSE_TRACE_FACILITY or config_GENERATE_सततम्_TIME_STATS are 1.  समुच्चयः_2 configUSE_STATS_FORMATTING_FUNCTIONS गन्तव्यम् 0 अन्तर् FreeRTOSConfig.h.
    #यदि_अन्तः
#यदि_अन्तः

#यदि_अपरिभाषितम् config_राशिः_गभीरता_TYPE

/* Defaults to uint16_t for backward compatibility, but can be overridden
 * in FreeRTOSConfig.h if uint16_t is too restrictive. */
    #परिभाषय config_राशिः_गभीरता_TYPE    uint16_t
#यदि_अन्तः

#यदि_अपरिभाषितम् config_सततम्_TIME_गणक_TYPE

/* Defaults to uint32_t for backward compatibility, but can be overridden in
 * FreeRTOSConfig.h if uint32_t is too restrictive. */

    #परिभाषय config_सततम्_TIME_गणक_TYPE    uint32_t
#यदि_अन्तः

#यदि_अपरिभाषितम् config_सन्देशः_बफर_दीर्घता_TYPE

/* Defaults to size_t for backward compatibility, but can be overridden
 * in FreeRTOSConfig.h if lengths will always be less than the number of bytes
 * in a size_t. */
    #परिभाषय config_सन्देशः_बफर_दीर्घता_TYPE    size_t
#यदि_अन्तः

/* Sanity check the configuration. */
#यदि ( ( config_SUPPORT_STATIC_आवण्टनम् == 0 ) && ( config_SUPPORT_DYNAMIC_आवण्टनम् == 0 ) )
    #दोषः config_SUPPORT_STATIC_आवण्टनम् and config_SUPPORT_DYNAMIC_आवण्टनम् cannot both be 0, but can both be 1.
#यदि_अन्तः

#यदि ( ( configUSE_RECURSIVE_MUTEXES == 1 ) && ( configUSE_MUTEXES != 1 ) )
    #दोषः configUSE_MUTEXES must be समुच्चयः गन्तव्यम् 1 गन्तव्यम् use recursive mutexes
#यदि_अन्तः

#यदि ( ( config_सङ्ख्या_सम्बन्धि_CORES > 1 ) && ( config_USE_पत्तनम्_OPTIMISED_TASK_SELECTION != 0 ) )
    #दोषः config_USE_पत्तनम्_OPTIMISED_TASK_SELECTION is not supported अन्तर् SMP
#यदि_अन्तः

#यदि_अपरिभाषितम् config_INITIAL_TICK_गणनम्
    #परिभाषय config_INITIAL_TICK_गणनम्    0
#यदि_अन्तः

#यदि ( पत्तनम्_TICK_TYPE_IS_ATOMIC == 0 )

/* Either variables of tick type cannot be read atomically, or
 * portTICK_TYPE_IS_ATOMIC was not set - map the critical sections used when
 * the tick count is returned to the standard critical section macros. */
    #परिभाषय पत्तनम्_TICK_TYPE_ENTER_CRITICAL()                      पत्तनम्_ENTER_CRITICAL()
    #परिभाषय पत्तनम्_TICK_TYPE_EXIT_CRITICAL()                       पत्तनम्_EXIT_CRITICAL()
    #परिभाषय पत्तनम्_TICK_TYPE_समुच्चयः_व्यवधानम्_आवरणम्_FROM_ISR()         पत्तनम्_समुच्चयः_व्यवधानम्_आवरणम्_FROM_ISR()
    #परिभाषय पत्तनम्_TICK_TYPE_CLEAR_व्यवधानम्_आवरणम्_FROM_ISR( x )    पत्तनम्_CLEAR_व्यवधानम्_आवरणम्_FROM_ISR( ( x ) )
#अन्यथा

/* The tick type can be read atomically, so critical sections used when the
 * tick count is returned can be defined away. */
    #परिभाषय पत्तनम्_TICK_TYPE_ENTER_CRITICAL()
    #परिभाषय पत्तनम्_TICK_TYPE_EXIT_CRITICAL()
    #परिभाषय पत्तनम्_TICK_TYPE_समुच्चयः_व्यवधानम्_आवरणम्_FROM_ISR()         0
    #परिभाषय पत्तनम्_TICK_TYPE_CLEAR_व्यवधानम्_आवरणम्_FROM_ISR( x )    ( शून्य ) ( x )
#यदि_अन्तः /* if ( portTICK_TYPE_IS_ATOMIC == 0 ) */

/* Definitions to allow backward compatibility with FreeRTOS versions prior to
 * V8 if desired. */
#यदि_अपरिभाषितम् configENABLE_BACKWARD_COMPATIBILITY
    #परिभाषय configENABLE_BACKWARD_COMPATIBILITY    1
#यदि_अन्तः

#यदि_अपरिभाषितम् configPRINTF

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
    #परिभाषय configPRINTF( X )
#यदि_अन्तः

#यदि_अपरिभाषितम् configMAX

/* The application writer has not provided their own MAX macro, so define
 * the following generic implementation. */
    #परिभाषय configMAX( a, b )    ( ( ( a ) > ( b ) ) ? ( a ) : ( b ) )
#यदि_अन्तः

#यदि_अपरिभाषितम् configMIN

/* The application writer has not provided their own MIN macro, so define
 * the following generic implementation. */
    #परिभाषय configMIN( a, b )    ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )
#यदि_अन्तः

#यदि configENABLE_BACKWARD_COMPATIBILITY == 1
    #परिभाषय eTaskStateGet                 eTaskGetState
    #परिभाषय पत्तनम्_Tick_Type                  TickType_t
    #परिभाषय xTaskHandle                   TaskHandle_t
    #परिभाषय xQueueHandle                  QueueHandle_t
    #परिभाषय xSemaphoreHandle              SemaphoreHandle_t
    #परिभाषय x_Queue_समुच्चयः_Handle               Queue_समुच्चयः_Handle_t
    #परिभाषय x_Queue_समुच्चयः_Member_Handle         Queue_समुच्चयः_Member_Handle_t
    #परिभाषय x_Time_बहिर्गमनम्_Type                  Time_बहिर्गमनम्_t
    #परिभाषय x_स्मृतिः_Region                 स्मृतिः_Region_t
    #परिभाषय xTaskParameters               TaskParameters_t
    #परिभाषय x_Task_स्थितिः_Type               Task_स्थितिः_t
    #परिभाषय xTimerHandle                  TimerHandle_t
    #परिभाषय xCoRoutineHandle              CoRoutineHandle_t
    #परिभाषय pd_TASK_HOOK_संहिता              TaskHookFunction_t
    #परिभाषय पत्तनम्_TICK_दरः_MS              पत्तनम्_TICK_PERIOD_MS
    #परिभाषय pcTaskGetTaskName             pcTaskGetName
    #परिभाषय pcTimerGetTimerName           pcTimerGetName
    #परिभाषय pcQueueGetQueueName           pcQueueGetName
    #परिभाषय vTaskGetTaskInfo              vTaskGetInfo
    #परिभाषय x_Task_Get_Idle_सततम्_Time_गणक    ul_Task_Get_Idle_सततम्_Time_गणक

/* Backward compatibility within the scheduler code only - these definitions
 * are not really required but are included for completeness. */
    #परिभाषय tmrTIMER_CALLBACK             TimerCallbackFunction_t
    #परिभाषय pd_TASK_संहिता                   TaskFunction_t
    #परिभाषय xListItem                     ListItem_t
    #परिभाषय xList                         List_t

/* For libraries that break the list data hiding, and access list structure
 * members directly (which is not supposed to be done). */
    #परिभाषय pxContainer                   pvContainer
#यदि_अन्तः /* configENABLE_BACKWARD_COMPATIBILITY */

#यदि ( configUSE_ALTERNATIVE_API != 0 )
    #दोषः The alternative API was deprecated some time ago, and was removed अन्तर् FreeRTOS V9.0 0
#यदि_अन्तः

/* Set configUSE_TASK_FPU_SUPPORT to 0 to omit floating point support even
 * if floating point hardware is otherwise supported by the FreeRTOS port in use.
 * This constant is not supported by all FreeRTOS ports that include floating
 * point support. */
#यदि_अपरिभाषितम् configUSE_TASK_FPU_SUPPORT
    #परिभाषय configUSE_TASK_FPU_SUPPORT    1
#यदि_अन्तः

/* Set configENABLE_MPU to 1 to enable MPU support and 0 to disable it. This is
 * currently used in ARMv8M ports. */
#यदि_अपरिभाषितम् configENABLE_MPU
    #परिभाषय configENABLE_MPU    0
#यदि_अन्तः

/* Set configENABLE_FPU to 1 to enable FPU support and 0 to disable it. This is
 * currently used in ARMv8M ports. */
#यदि_अपरिभाषितम् configENABLE_FPU
    #परिभाषय configENABLE_FPU    1
#यदि_अन्तः

/* Set configENABLE_MVE to 1 to enable MVE support and 0 to disable it. This is
 * currently used in ARMv8M ports. */
#यदि_अपरिभाषितम् configENABLE_MVE
    #परिभाषय configENABLE_MVE    0
#यदि_अन्तः

/* Set configENABLE_TRUSTZONE to 1 enable TrustZone support and 0 to disable it.
 * This is currently used in ARMv8M ports. */
#यदि_अपरिभाषितम् configENABLE_TRUSTZONE
    #परिभाषय configENABLE_TRUSTZONE    1
#यदि_अन्तः

/* Set configRUN_FREERTOS_SECURE_ONLY to 1 to run the FreeRTOS ARMv8M port on
 * the Secure Side only. */
#यदि_अपरिभाषितम् config_सततम्_FREERTOS_SECURE_ONLY
    #परिभाषय config_सततम्_FREERTOS_SECURE_ONLY    0
#यदि_अन्तः

#यदि_अपरिभाषितम् config_सततम्_ADDITIONAL_TESTS
    #परिभाषय config_सततम्_ADDITIONAL_TESTS    0
#यदि_अन्तः


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
#परिभाषय tsk_STATIC_AND_DYNAMIC_आवण्टनम्_POSSIBLE                                                                                     \
    ( ( ( पत्तनम्_USING_MPU_WRAPPERS == 0 ) && ( config_SUPPORT_DYNAMIC_आवण्टनम् == 1 ) && ( config_SUPPORT_STATIC_आवण्टनम् == 1 ) ) || \
      ( ( पत्तनम्_USING_MPU_WRAPPERS == 1 ) && ( config_SUPPORT_DYNAMIC_आवण्टनम् == 1 ) ) )

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
संरचना xSTATIC_LIST_ITEM
{
    #यदि ( config_USE_LIST_दत्त_INTEGRITY_CHECK_अष्टकानि == 1 )
        TickType_t xDummy1;
    #यदि_अन्तः
    TickType_t xDummy2;
    शून्य * pvDummy3[ 4 ];
    #यदि ( config_USE_LIST_दत्त_INTEGRITY_CHECK_अष्टकानि == 1 )
        TickType_t xDummy4;
    #यदि_अन्तः
};
प्रकारनिर्देश संरचना xSTATIC_LIST_ITEM StaticListItem_t;

#यदि ( configUSE_MINI_LIST_ITEM == 1 )
    /* See the comments above the struct xSTATIC_LIST_ITEM definition. */
    संरचना xSTATIC_MINI_LIST_ITEM
    {
        #यदि ( config_USE_LIST_दत्त_INTEGRITY_CHECK_अष्टकानि == 1 )
            TickType_t xDummy1;
        #यदि_अन्तः
        TickType_t xDummy2;
        शून्य * pvDummy3[ 2 ];
    };
    प्रकारनिर्देश संरचना xSTATIC_MINI_LIST_ITEM StaticMiniListItem_t;
#अन्यथा /* if ( configUSE_MINI_LIST_ITEM == 1 ) */
    प्रकारनिर्देश संरचना xSTATIC_LIST_ITEM      StaticMiniListItem_t;
#यदि_अन्तः /* if ( configUSE_MINI_LIST_ITEM == 1 ) */

/* See the comments above the struct xSTATIC_LIST_ITEM definition. */
प्रकारनिर्देश संरचना xSTATIC_LIST
{
    #यदि ( config_USE_LIST_दत्त_INTEGRITY_CHECK_अष्टकानि == 1 )
        TickType_t xDummy1;
    #यदि_अन्तः
    UBaseType_t uxDummy2;
    शून्य * pvDummy3;
    StaticMiniListItem_t xDummy4;
    #यदि ( config_USE_LIST_दत्त_INTEGRITY_CHECK_अष्टकानि == 1 )
        TickType_t xDummy5;
    #यदि_अन्तः
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
प्रकारनिर्देश संरचना xSTATIC_TCB
{
    शून्य * pxDummy1;
    #यदि ( पत्तनम्_USING_MPU_WRAPPERS == 1 )
        xMPU_SETTINGS xDummy2;
    #यदि_अन्तः
    StaticListItem_t xDummy3[ 2 ];
    UBaseType_t uxDummy5;
    शून्य * pxDummy6;
    uint8_t ucDummy7[ configMAX_TASK_NAME_LEN ];
    #यदि ( config_सङ्ख्या_सम्बन्धि_CORES > 1 )
        आधारः_Type_t xDummyCoreID;
    #यदि_अन्तः /* configNUMBER_OF_CORES > 1 */
    #यदि ( ( पत्तनम्_राशिः_GROWTH > 0 ) || ( config_लेखः_राशिः_उच्चः_स्थानाङ्कः == 1 ) )
        शून्य * pxDummy8;
    #यदि_अन्तः
    #यदि ( पत्तनम्_CRITICAL_NESTING_अन्तर्_TCB == 1 )
        UBaseType_t uxDummy9;
    #यदि_अन्तः
    #यदि ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy10[ 2 ];
    #यदि_अन्तः
    #यदि ( configUSE_MUTEXES == 1 )
        UBaseType_t uxDummy12[ 2 ];
    #यदि_अन्तः
    #यदि ( config_USE_APPLICATION_TASK_चिह्नकम् == 1 )
        शून्य * pxDummy14;
    #यदि_अन्तः
    #यदि ( config_अङ्कः_THREAD_LOCAL_STORAGE_सूचकाः > 0 )
        शून्य * pvDummy15[ config_अङ्कः_THREAD_LOCAL_STORAGE_सूचकाः ];
    #यदि_अन्तः
    #यदि ( config_GENERATE_सततम्_TIME_STATS == 1 )
        config_सततम्_TIME_गणक_TYPE ulDummy16;
    #यदि_अन्तः
    #यदि ( ( configUSE_NEWLIB_REENTRANT == 1 ) || ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 ) )
        config_TLS_खण्डम्_TYPE xDummy17;
    #यदि_अन्तः
    #यदि ( configUSE_TASK_NOTIFICATIONS == 1 )
        uint32_t ulDummy18[ configTASK_NOTIFICATION_ARRAY_ENTRIES ];
        uint8_t ucDummy19[ configTASK_NOTIFICATION_ARRAY_ENTRIES ];
    #यदि_अन्तः
    #यदि ( tsk_STATIC_AND_DYNAMIC_आवण्टनम्_POSSIBLE != 0 )
        uint8_t uxDummy20;
    #यदि_अन्तः

    #यदि ( INCLUDE_xTaskAbortDelay == 1 )
        uint8_t ucDummy21;
    #यदि_अन्तः
    #यदि ( configUSE_POSIX_ERRNO == 1 )
        पूर्णाङ्क iDummy22;
    #यदि_अन्तः
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
प्रकारनिर्देश संरचना xSTATIC_QUEUE
{
    शून्य * pvDummy1[ 3 ];

    सङ्घ
    {
        शून्य * pvDummy2;
        UBaseType_t uxDummy2;
    } u;

    StaticList_t xDummy3[ 2 ];
    UBaseType_t uxDummy4[ 3 ];
    uint8_t ucDummy5[ 2 ];

    #यदि ( ( config_SUPPORT_STATIC_आवण्टनम् == 1 ) && ( config_SUPPORT_DYNAMIC_आवण्टनम् == 1 ) )
        uint8_t ucDummy6;
    #यदि_अन्तः

    #यदि ( configUSE_QUEUE_SETS == 1 )
        शून्य * pvDummy7;
    #यदि_अन्तः

    #यदि ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy8;
        uint8_t ucDummy9;
    #यदि_अन्तः
    पत्तनम्_MUX_TYPE x_Dummy_Queue_तालकम्;
} StaticQueue_t;
प्रकारनिर्देश StaticQueue_t StaticSemaphore_t;

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
प्रकारनिर्देश संरचना x_STATIC_EVENT_गणः
{
    TickType_t xDummy1;
    StaticList_t xDummy2;

    #यदि ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy3;
    #यदि_अन्तः

    #यदि ( ( config_SUPPORT_STATIC_आवण्टनम् == 1 ) && ( config_SUPPORT_DYNAMIC_आवण्टनम् == 1 ) )
        uint8_t ucDummy4;
    #यदि_अन्तः
    पत्तनम्_MUX_TYPE x_Dummy_Event_गणः_तालकम्;
} Static_Event_गणः_t;

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
प्रकारनिर्देश संरचना xSTATIC_TIMER
{
    शून्य * pvDummy1;
    StaticListItem_t xDummy2;
    TickType_t xDummy3;
    शून्य * pvDummy5;
    TaskFunction_t pvDummy6;
    #यदि ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy7;
    #यदि_अन्तः
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
प्रकारनिर्देश संरचना x_STATIC_प्रवाहः_बफर
{
    size_t uxDummy1[ 4 ];
    शून्य * pvDummy2[ 3 ];
    uint8_t ucDummy3;
    #यदि ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy4;
    #यदि_अन्तः
    #यदि ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        शून्य * pvDummy5[ 2 ];
    #यदि_अन्तः
    पत्तनम्_MUX_TYPE x_Dummy_प्रवाहः_बफर_तालकम्;
} Static_प्रवाहः_बफर_t;

/* Message buffers are built on stream buffers. */
प्रकारनिर्देश Static_प्रवाहः_बफर_t Static_सन्देशः_बफर_t;

/* *INDENT-OFF* */
#यदि_परिभाषितम् __cplusplus
    }
#यदि_अन्तः
/* *INDENT-ON* */

/*-----------------------------------------------------------
* IDF Compatibility
*----------------------------------------------------------*/

#यदि_परिभाषितम् ESP_PLATFORM

/* *INDENT-OFF* */
#यदि_परिभाषितम् __cplusplus
    बाह्य "C" {
#यदि_अन्तः
/* *INDENT-ON* */

/*
 * Default values for trace macros added by ESP-IDF and are not part of Vanilla FreeRTOS
 */

    #यदि_अपरिभाषितम् trace_ISR_EXIT_गन्तव्यम्_SCHEDULER
        #परिभाषय trace_ISR_EXIT_गन्तव्यम्_SCHEDULER()
    #यदि_अन्तः

    #यदि_अपरिभाषितम् traceISR_EXIT
        #परिभाषय traceISR_EXIT()
    #यदि_अन्तः

    #यदि_अपरिभाषितम् traceISR_ENTER
        #परिभाषय traceISR_ENTER( _n_ )
    #यदि_अन्तः

    #यदि_अपरिभाषितम् traceQUEUE_SEMAPHORE_RECEIVE
        #परिभाषय traceQUEUE_SEMAPHORE_RECEIVE( pxQueue )
    #यदि_अन्तः

    #यदि_अपरिभाषितम् traceQUEUE_GIVE_FROM_ISR
        #परिभाषय traceQUEUE_GIVE_FROM_ISR( pxQueue )
    #यदि_अन्तः

    #यदि_अपरिभाषितम् traceQUEUE_GIVE_FROM_ISR_FAILED
        #परिभाषय traceQUEUE_GIVE_FROM_ISR_FAILED( pxQueue )
    #यदि_अन्तः

/* *INDENT-OFF* */
#यदि_परिभाषितम् __cplusplus
    }
#यदि_अन्तः
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
    #अन्तर्भाव "freertos/idf_additions.h"

#यदि_अन्तः /* ESP_PLATFORM */

#यदि_अन्तः /* INC_FREERTOS_H */
