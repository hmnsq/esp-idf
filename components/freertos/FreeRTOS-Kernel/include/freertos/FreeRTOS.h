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

#अगर_अघोषित INC_FREERTOS_H
#घोषणा INC_FREERTOS_H

/*
 * Include the generic headers required for the FreeRTOS port being used.
 */
#समावेश <stddef.h>

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
#समावेश <stdint.h> /* READ COMMENT ABOVE. */

/* *INDENT-OFF* */
#अगर_घोषित __cplusplus
    बाह्य "C" {
#पूर्ण_अगर
/* *INDENT-ON* */

/* Application specific configuration options. */
#समावेश "FreeRTOSConfig.h"

/* Basic FreeRTOS definitions. */
#समावेश "projdefs.h"

/* Definitions specific to the port being used. */
#समावेश "portable.h"

/* Must be defaulted before configUSE_NEWLIB_REENTRANT is used below. */
#अगर_अघोषित configUSE_NEWLIB_REENTRANT
    #घोषणा configUSE_NEWLIB_REENTRANT    0
#पूर्ण_अगर

/* Required if struct _reent is used. */
#अगर ( configUSE_NEWLIB_REENTRANT == 1 )

/* Note Newlib support has been included by popular demand, but is not
 * used by the FreeRTOS maintainers themselves.  FreeRTOS is not
 * responsible for resulting newlib operation.  User must be familiar with
 * newlib and must provide system-wide implementations of the necessary
 * stubs. Be warned that (at the time of writing) the current newlib design
 * implements a system-wide malloc() that must be provided with locks.
 *
 * See the third party link http://www.nadler.com/embedded/newlibAndFreeRTOS.html
 * for additional information. */
    #समावेश <reent.h>

    #घोषणा configUSE_C_RUNTIME_TLS_SUPPORT    1

    #अगर_अघोषित configTLS_BLOCK_TYPE
        #घोषणा configTLS_BLOCK_TYPE           काष्ठा _reent
    #पूर्ण_अगर

    #अगर_अघोषित configINIT_TLS_BLOCK
        #घोषणा configINIT_TLS_BLOCK( xTLSBlock )    _REENT_INIT_PTR( &( xTLSBlock ) )
    #पूर्ण_अगर

    #अगर_अघोषित configSET_TLS_BLOCK
        #घोषणा configSET_TLS_BLOCK( xTLSBlock )    _impure_ptr = &( xTLSBlock )
    #पूर्ण_अगर

    #अगर_अघोषित configDEINIT_TLS_BLOCK
        #घोषणा configDEINIT_TLS_BLOCK( xTLSBlock )    _reclaim_reent( &( xTLSBlock ) )
    #पूर्ण_अगर
#पूर्ण_अगर /* if ( configUSE_NEWLIB_REENTRANT == 1 ) */

#अगर_अघोषित configUSE_C_RUNTIME_TLS_SUPPORT
    #घोषणा configUSE_C_RUNTIME_TLS_SUPPORT    0
#पूर्ण_अगर

#अगर ( ( configUSE_NEWLIB_REENTRANT == 0 ) && ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 ) )

    #अगर_अघोषित configTLS_BLOCK_TYPE
        #त्रुटि Missing definition:  configTLS_BLOCK_TYPE must be defined in FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is set to 1.
    #पूर्ण_अगर

    #अगर_अघोषित configINIT_TLS_BLOCK
        #त्रुटि Missing definition:  configINIT_TLS_BLOCK must be defined in FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is set to 1.
    #पूर्ण_अगर

    #अगर_अघोषित configSET_TLS_BLOCK
        #त्रुटि Missing definition:  configSET_TLS_BLOCK must be defined in FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is set to 1.
    #पूर्ण_अगर

    #अगर_अघोषित configDEINIT_TLS_BLOCK
        #त्रुटि Missing definition:  configDEINIT_TLS_BLOCK must be defined in FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is set to 1.
    #पूर्ण_अगर
#पूर्ण_अगर /* if ( ( configUSE_NEWLIB_REENTRANT == 0 ) && ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 ) ) */

/*
 * Check all the required application specific macros have been defined.
 * These macros are application specific and (as downloaded) are defined
 * within FreeRTOSConfig.h.
 */

#अगर_अघोषित config_संख्या_OF_CORES
    #त्रुटि Missing definition:  config_संख्या_OF_CORES must be defined in FreeRTOSConfig.h
#पूर्ण_अगर

#अगर ( config_संख्या_OF_CORES > 1 )
    #अगर_अघोषित portGET_CORE_ID
        #त्रुटि "Missing definition: portGET_CORE_ID() must be defined if in portmacro.h if configNUMBER_OF_CORES > 1"
    #पूर्ण_अगर
    #अगर_अघोषित portYIELD_CORE
        #त्रुटि "Missing definition: portYIELD_CORE() must be defined if in portmacro.h if configNUMBER_OF_CORES > 1"
    #पूर्ण_अगर
#या_अगर ( config_संख्या_OF_CORES == 1 )
    #अघोषित portGET_CORE_ID
    #घोषणा portGET_CORE_ID()    0
#अन्यथा
    #त्रुटि config_संख्या_OF_CORES must be defined to either 1 or > 1.
#पूर्ण_अगर /* if ( configNUMBER_OF_CORES > 1 ) */

#अगर_अघोषित config_MINIMAL_ढेर_आकार
    #त्रुटि Missing definition:  config_MINIMAL_ढेर_आकार must be defined in FreeRTOSConfig.h.  config_MINIMAL_ढेर_आकार defines the आकार (in शब्दगण) of the ढेर allocated to the idle task.  Refer to the demo project provided क्रम your port क्रम a suitable मूल्य.
#पूर्ण_अगर

#अगर_अघोषित configMAX_PRIORITIES
    #त्रुटि Missing definition:  configMAX_PRIORITIES must be defined in FreeRTOSConfig.h.  See the Configuration section of the FreeRTOS API documentation क्रम details.
#पूर्ण_अगर

#अगर configMAX_PRIORITIES < 1
    #त्रुटि configMAX_PRIORITIES must be defined to be greater than or equal to 1.
#पूर्ण_अगर

#अगर_अघोषित configUSE_PREEMPTION
    #त्रुटि Missing definition:  configUSE_PREEMPTION must be defined in FreeRTOSConfig.h as either 1 or 0.  See the Configuration section of the FreeRTOS API documentation क्रम details.
#पूर्ण_अगर

#अगर_अघोषित configUSE_IDLE_HOOK
    #त्रुटि Missing definition:  configUSE_IDLE_HOOK must be defined in FreeRTOSConfig.h as either 1 or 0.  See the Configuration section of the FreeRTOS API documentation क्रम details.
#पूर्ण_अगर

#अगर_अघोषित configUSE_TICK_HOOK
    #त्रुटि Missing definition:  configUSE_TICK_HOOK must be defined in FreeRTOSConfig.h as either 1 or 0.  See the Configuration section of the FreeRTOS API documentation क्रम details.
#पूर्ण_अगर

#अगर_अघोषित configUSE_16_BIT_TICKS
    #त्रुटि Missing definition:  configUSE_16_BIT_TICKS must be defined in FreeRTOSConfig.h as either 1 or 0.  See the Configuration section of the FreeRTOS API documentation क्रम details.
#पूर्ण_अगर

#अगर_अघोषित configUSE_CO_ROUTINES
    #घोषणा configUSE_CO_ROUTINES    0
#पूर्ण_अगर

#अगर_अघोषित INCLUDE_vTaskPrioritySet
    #घोषणा INCLUDE_vTaskPrioritySet    0
#पूर्ण_अगर

#अगर_अघोषित INCLUDE_uxTaskPriorityGet
    #घोषणा INCLUDE_uxTaskPriorityGet    0
#पूर्ण_अगर

#अगर_अघोषित INCLUDE_vTaskDelete
    #घोषणा INCLUDE_vTaskDelete    0
#पूर्ण_अगर

#अगर_अघोषित INCLUDE_vTaskSuspend
    #घोषणा INCLUDE_vTaskSuspend    0
#पूर्ण_अगर

#अगर_घोषित INCLUDE_xTaskDelayUntil
    #अगर_घोषित INCLUDE_vTaskDelayUntil

/* INCLUDE_vTaskDelayUntil was replaced by INCLUDE_xTaskDelayUntil.  Backward
 * compatibility is maintained if only one or the other is defined, but
 * there is a conflict if both are defined. */
        #त्रुटि INCLUDE_vTaskDelayUntil and INCLUDE_xTaskDelayUntil are both defined.  INCLUDE_vTaskDelayUntil is no longer required and should be removed
    #पूर्ण_अगर
#पूर्ण_अगर

#अगर_अघोषित INCLUDE_xTaskDelayUntil
    #अगर_घोषित INCLUDE_vTaskDelayUntil

/* If INCLUDE_vTaskDelayUntil is set but INCLUDE_xTaskDelayUntil is not then
 * the project's FreeRTOSConfig.h probably pre-dates the introduction of
 * xTaskDelayUntil and setting INCLUDE_xTaskDelayUntil to whatever
 * INCLUDE_vTaskDelayUntil is set to will ensure backward compatibility.
 */
        #घोषणा INCLUDE_xTaskDelayUntil    INCLUDE_vTaskDelayUntil
    #पूर्ण_अगर
#पूर्ण_अगर

#अगर_अघोषित INCLUDE_xTaskDelayUntil
    #घोषणा INCLUDE_xTaskDelayUntil    0
#पूर्ण_अगर

#अगर_अघोषित INCLUDE_vTaskDelay
    #घोषणा INCLUDE_vTaskDelay    0
#पूर्ण_अगर

#अगर_अघोषित INCLUDE_xTaskGetIdleTaskHandle
    #घोषणा INCLUDE_xTaskGetIdleTaskHandle    0
#पूर्ण_अगर

#अगर_अघोषित INCLUDE_xTaskAbortDelay
    #घोषणा INCLUDE_xTaskAbortDelay    0
#पूर्ण_अगर

#अगर_अघोषित INCLUDE_xQueueGetMutexHolder
    #घोषणा INCLUDE_xQueueGetMutexHolder    0
#पूर्ण_अगर

#अगर_अघोषित INCLUDE_xSemaphoreGetMutexHolder
    #घोषणा INCLUDE_xSemaphoreGetMutexHolder    INCLUDE_xQueueGetMutexHolder
#पूर्ण_अगर

#अगर_अघोषित INCLUDE_xTaskGetHandle
    #घोषणा INCLUDE_xTaskGetHandle    0
#पूर्ण_अगर

#अगर_अघोषित INCLUDE_ux_Task_Get_ढेर_ऊपरी_Water_Mark
    #घोषणा INCLUDE_ux_Task_Get_ढेर_ऊपरी_Water_Mark    0
#पूर्ण_अगर

#अगर_अघोषित INCLUDE_ux_Task_Get_ढेर_ऊपरी_Water_Mark2
    #घोषणा INCLUDE_ux_Task_Get_ढेर_ऊपरी_Water_Mark2    0
#पूर्ण_अगर

#अगर_अघोषित INCLUDE_eTaskGetState
    #घोषणा INCLUDE_eTaskGetState    0
#पूर्ण_अगर

#अगर_अघोषित INCLUDE_xTaskResumeFromISR
    #घोषणा INCLUDE_xTaskResumeFromISR    1
#पूर्ण_अगर

#अगर_अघोषित INCLUDE_xTimerPendFunctionCall
    #घोषणा INCLUDE_xTimerPendFunctionCall    0
#पूर्ण_अगर

#अगर_अघोषित INCLUDE_xTaskGetSchedulerState
    #घोषणा INCLUDE_xTaskGetSchedulerState    0
#पूर्ण_अगर

#अगर_अघोषित INCLUDE_xTaskGetCurrentTaskHandle
    #घोषणा INCLUDE_xTaskGetCurrentTaskHandle    1
#पूर्ण_अगर

#अगर configUSE_CO_ROUTINES != 0
    #अगर_अघोषित configMAX_CO_ROUTINE_PRIORITIES
        #त्रुटि configMAX_CO_ROUTINE_PRIORITIES must be greater than or equal to 1.
    #पूर्ण_अगर
#पूर्ण_अगर

#अगर_अघोषित configUSE_DAEMON_TASK_STARTUP_HOOK
    #घोषणा configUSE_DAEMON_TASK_STARTUP_HOOK    0
#पूर्ण_अगर

#अगर_अघोषित configUSE_APPLICATION_TASK_TAG
    #घोषणा configUSE_APPLICATION_TASK_TAG    0
#पूर्ण_अगर

#अगर_अघोषित config_अंक_THREAD_LOCAL_STORAGE_POINTERS
    #घोषणा config_अंक_THREAD_LOCAL_STORAGE_POINTERS    0
#पूर्ण_अगर

#अगर_अघोषित configUSE_RECURSIVE_MUTEXES
    #घोषणा configUSE_RECURSIVE_MUTEXES    0
#पूर्ण_अगर

#अगर_अघोषित configUSE_MUTEXES
    #घोषणा configUSE_MUTEXES    0
#पूर्ण_अगर

#अगर_अघोषित configUSE_TIMERS
    #घोषणा configUSE_TIMERS    0
#पूर्ण_अगर

#अगर_अघोषित configUSE_COUNTING_SEMAPHORES
    #घोषणा configUSE_COUNTING_SEMAPHORES    0
#पूर्ण_अगर

#अगर_अघोषित configUSE_ALTERNATIVE_API
    #घोषणा configUSE_ALTERNATIVE_API    0
#पूर्ण_अगर

#अगर_अघोषित portCRITICAL_NESTING_IN_TCB
    #घोषणा portCRITICAL_NESTING_IN_TCB    0
#पूर्ण_अगर

#अगर_अघोषित configMAX_TASK_NAME_LEN
    #घोषणा configMAX_TASK_NAME_LEN    16
#पूर्ण_अगर

#अगर_अघोषित configIDLE_SHOULD_YIELD
    #घोषणा configIDLE_SHOULD_YIELD    1
#पूर्ण_अगर

#अगर configMAX_TASK_NAME_LEN < 1
    #त्रुटि configMAX_TASK_NAME_LEN must be set to a minimum of 1 in FreeRTOSConfig.h
#पूर्ण_अगर

#अगर_अघोषित configASSERT
    #घोषणा configASSERT( x )
    #घोषणा configASSERT_DEFINED    0
#अन्यथा
    #घोषणा configASSERT_DEFINED    1
#पूर्ण_अगर

/* configPRECONDITION should be defined as configASSERT.
 * The CBMC proofs need a way to track assumptions and assertions.
 * A configPRECONDITION statement should express an implicit invariant or
 * assumption made.  A configASSERT statement should express an invariant that must
 * hold explicit before calling the code. */
#अगर_अघोषित configPRECONDITION
    #घोषणा configPRECONDITION( X )    configASSERT( X )
    #घोषणा configPRECONDITION_DEFINED    0
#अन्यथा
    #घोषणा configPRECONDITION_DEFINED    1
#पूर्ण_अगर

#अगर_अघोषित portMEMORY_BARRIER
    #घोषणा portMEMORY_BARRIER()
#पूर्ण_अगर

#अगर_अघोषित portSOFTWARE_BARRIER
    #घोषणा portSOFTWARE_BARRIER()
#पूर्ण_अगर

/* The timers module relies on xTaskGetSchedulerState(). */
#अगर configUSE_TIMERS == 1

    #अगर_अघोषित configTIMER_TASK_PRIORITY
        #त्रुटि If configUSE_TIMERS is set to 1 then configTIMER_TASK_PRIORITY must also be defined.
    #पूर्ण_अगर /* configTIMER_TASK_PRIORITY */

    #अगर_अघोषित config_TIMER_QUEUE_लंबाई
        #त्रुटि If configUSE_TIMERS is set to 1 then config_TIMER_QUEUE_लंबाई must also be defined.
    #पूर्ण_अगर /* configTIMER_QUEUE_LENGTH */

    #अगर_अघोषित config_TIMER_TASK_ढेर_DEPTH
        #त्रुटि If configUSE_TIMERS is set to 1 then config_TIMER_TASK_ढेर_DEPTH must also be defined.
    #पूर्ण_अगर /* configTIMER_TASK_STACK_DEPTH */

#पूर्ण_अगर /* configUSE_TIMERS */

#अगर_अघोषित portSET_INTERRUPT_MASK_FROM_ISR
    #घोषणा portSET_INTERRUPT_MASK_FROM_ISR()    0
#पूर्ण_अगर

#अगर_अघोषित portCLEAR_INTERRUPT_MASK_FROM_ISR
    #घोषणा portCLEAR_INTERRUPT_MASK_FROM_ISR( ux_Saved_Status_मूल्य )    ( व्योम ) ( ux_Saved_Status_मूल्य )
#पूर्ण_अगर

#अगर_अघोषित portCLEAN_UP_TCB
    #घोषणा portCLEAN_UP_TCB( pxTCB )    ( व्योम ) ( pxTCB )
#पूर्ण_अगर

#अगर_अघोषित portPRE_TASK_DELETE_HOOK
    #घोषणा portPRE_TASK_DELETE_HOOK( pvTaskToDelete, pxYieldPending )
#पूर्ण_अगर

#अगर_अघोषित portSETUP_TCB
    #घोषणा portSETUP_TCB( pxTCB )    ( व्योम ) ( pxTCB )
#पूर्ण_अगर

#अगर_अघोषित config_QUEUE_REGISTRY_आकार
    #घोषणा config_QUEUE_REGISTRY_आकार    0U
#पूर्ण_अगर

#अगर ( config_QUEUE_REGISTRY_आकार < 1 )
    #घोषणा vQueueAddToRegistry( xQueue, pcName )
    #घोषणा vQueueUnregisterQueue( xQueue )
    #घोषणा pcQueueGetName( xQueue )
#पूर्ण_अगर

#अगर_अघोषित configUSE_MINI_LIST_ITEM
    #घोषणा configUSE_MINI_LIST_ITEM    1
#पूर्ण_अगर

#अगर_अघोषित port_POINTER_आकार_TYPE
    #घोषणा port_POINTER_आकार_TYPE    uint32_t
#पूर्ण_अगर

/* Remove any unused trace macros. */
#अगर_अघोषित traceSTART

/* Used to perform any necessary initialisation - for example, open a file
 * into which trace is to be written. */
    #घोषणा traceSTART()
#पूर्ण_अगर

#अगर_अघोषित traceEND

/* Use to close a trace, for example close a file into which trace has been
 * written. */
    #घोषणा traceEND()
#पूर्ण_अगर

#अगर_अघोषित traceTASK_SWITCHED_IN

/* Called after a task has been selected to run.  pxCurrentTCB holds a pointer
 * to the task control block of the selected task. */
    #घोषणा traceTASK_SWITCHED_IN()
#पूर्ण_अगर

#अगर_अघोषित trace_INCREASE_TICK_गिनती

/* Called before stepping the tick count after waking from tickless idle
 * sleep. */
    #घोषणा trace_INCREASE_TICK_गिनती( x )
#पूर्ण_अगर

#अगर_अघोषित trace_निचला_POWER_IDLE_BEGIN
    /* Called immediately before entering tickless idle. */
    #घोषणा trace_निचला_POWER_IDLE_BEGIN()
#पूर्ण_अगर

#अगर_अघोषित trace_निचला_POWER_IDLE_END
    /* Called when returning to the Idle task after a tickless idle. */
    #घोषणा trace_निचला_POWER_IDLE_END()
#पूर्ण_अगर

#अगर_अघोषित traceTASK_SWITCHED_OUT

/* Called before a task has been selected to run.  pxCurrentTCB holds a pointer
 * to the task control block of the task being switched out. */
    #घोषणा traceTASK_SWITCHED_OUT()
#पूर्ण_अगर

#अगर_अघोषित traceTASK_PRIORITY_INHERIT

/* Called when a task attempts to take a mutex that is already held by a
 * lower priority task.  pxTCBOfMutexHolder is a pointer to the TCB of the task
 * that holds the mutex.  uxInheritedPriority is the priority the mutex holder
 * will inherit (the priority of the task that is attempting to obtain the
 * muted. */
    #घोषणा traceTASK_PRIORITY_INHERIT( pxTCBOfMutexHolder, uxInheritedPriority )
#पूर्ण_अगर

#अगर_अघोषित traceTASK_PRIORITY_DISINHERIT

/* Called when a task releases a mutex, the holding of which had resulted in
 * the task inheriting the priority of a higher priority task.
 * pxTCBOfMutexHolder is a pointer to the TCB of the task that is releasing the
 * mutex.  uxOriginalPriority is the task's configured (base) priority. */
    #घोषणा traceTASK_PRIORITY_DISINHERIT( pxTCBOfMutexHolder, uxOriginalPriority )
#पूर्ण_अगर

#अगर_अघोषित traceBLOCKING_ON_QUEUE_RECEIVE

/* Task is about to block because it cannot read from a
 * queue/mutex/semaphore.  pxQueue is a pointer to the queue/mutex/semaphore
 * upon which the read was attempted.  pxCurrentTCB points to the TCB of the
 * task that attempted the read. */
    #घोषणा traceBLOCKING_ON_QUEUE_RECEIVE( pxQueue )
#पूर्ण_अगर

#अगर_अघोषित traceBLOCKING_ON_QUEUE_PEEK

/* Task is about to block because it cannot read from a
 * queue/mutex/semaphore.  pxQueue is a pointer to the queue/mutex/semaphore
 * upon which the read was attempted.  pxCurrentTCB points to the TCB of the
 * task that attempted the read. */
    #घोषणा traceBLOCKING_ON_QUEUE_PEEK( pxQueue )
#पूर्ण_अगर

#अगर_अघोषित traceBLOCKING_ON_QUEUE_SEND

/* Task is about to block because it cannot write to a
 * queue/mutex/semaphore.  pxQueue is a pointer to the queue/mutex/semaphore
 * upon which the write was attempted.  pxCurrentTCB points to the TCB of the
 * task that attempted the write. */
    #घोषणा traceBLOCKING_ON_QUEUE_SEND( pxQueue )
#पूर्ण_अगर

#अगर_अघोषित config_CHECK_FOR_ढेर_OVERFLOW
    #घोषणा config_CHECK_FOR_ढेर_OVERFLOW    0
#पूर्ण_अगर

#अगर_अघोषित config_RECORD_ढेर_ऊपरी_ADDRESS
    #घोषणा config_RECORD_ढेर_ऊपरी_ADDRESS    0
#पूर्ण_अगर

#अगर_अघोषित configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H
    #घोषणा configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H    0
#पूर्ण_अगर

/* The following event macros are embedded in the kernel API calls. */

#अगर_अघोषित traceMOVED_TASK_TO_READY_STATE
    #घोषणा traceMOVED_TASK_TO_READY_STATE( pxTCB )
#पूर्ण_अगर

#अगर_अघोषित tracePOST_MOVED_TASK_TO_READY_STATE
    #घोषणा tracePOST_MOVED_TASK_TO_READY_STATE( pxTCB )
#पूर्ण_अगर

#अगर_अघोषित traceQUEUE_CREATE
    #घोषणा traceQUEUE_CREATE( pxNewQueue )
#पूर्ण_अगर

#अगर_अघोषित traceQUEUE_CREATE_FAILED
    #घोषणा traceQUEUE_CREATE_FAILED( ucQueueType )
#पूर्ण_अगर

#अगर_अघोषित traceCREATE_MUTEX
    #घोषणा traceCREATE_MUTEX( pxNewQueue )
#पूर्ण_अगर

#अगर_अघोषित traceCREATE_MUTEX_FAILED
    #घोषणा traceCREATE_MUTEX_FAILED()
#पूर्ण_अगर

#अगर_अघोषित traceGIVE_MUTEX_RECURSIVE
    #घोषणा traceGIVE_MUTEX_RECURSIVE( pxMutex )
#पूर्ण_अगर

#अगर_अघोषित traceGIVE_MUTEX_RECURSIVE_FAILED
    #घोषणा traceGIVE_MUTEX_RECURSIVE_FAILED( pxMutex )
#पूर्ण_अगर

#अगर_अघोषित traceTAKE_MUTEX_RECURSIVE
    #घोषणा traceTAKE_MUTEX_RECURSIVE( pxMutex )
#पूर्ण_अगर

#अगर_अघोषित traceTAKE_MUTEX_RECURSIVE_FAILED
    #घोषणा traceTAKE_MUTEX_RECURSIVE_FAILED( pxMutex )
#पूर्ण_अगर

#अगर_अघोषित traceCREATE_COUNTING_SEMAPHORE
    #घोषणा traceCREATE_COUNTING_SEMAPHORE()
#पूर्ण_अगर

#अगर_अघोषित traceCREATE_COUNTING_SEMAPHORE_FAILED
    #घोषणा traceCREATE_COUNTING_SEMAPHORE_FAILED()
#पूर्ण_अगर

#अगर_अघोषित traceQUEUE_SET_SEND
    #घोषणा traceQUEUE_SET_SEND    traceQUEUE_SEND
#पूर्ण_अगर

#अगर_अघोषित traceQUEUE_SEND
    #घोषणा traceQUEUE_SEND( pxQueue )
#पूर्ण_अगर

#अगर_अघोषित traceQUEUE_SEND_FAILED
    #घोषणा traceQUEUE_SEND_FAILED( pxQueue )
#पूर्ण_अगर

#अगर_अघोषित traceQUEUE_RECEIVE
    #घोषणा traceQUEUE_RECEIVE( pxQueue )
#पूर्ण_अगर

#अगर_अघोषित traceQUEUE_PEEK
    #घोषणा traceQUEUE_PEEK( pxQueue )
#पूर्ण_अगर

#अगर_अघोषित traceQUEUE_PEEK_FAILED
    #घोषणा traceQUEUE_PEEK_FAILED( pxQueue )
#पूर्ण_अगर

#अगर_अघोषित traceQUEUE_PEEK_FROM_ISR
    #घोषणा traceQUEUE_PEEK_FROM_ISR( pxQueue )
#पूर्ण_अगर

#अगर_अघोषित traceQUEUE_RECEIVE_FAILED
    #घोषणा traceQUEUE_RECEIVE_FAILED( pxQueue )
#पूर्ण_अगर

#अगर_अघोषित traceQUEUE_SEND_FROM_ISR
    #घोषणा traceQUEUE_SEND_FROM_ISR( pxQueue )
#पूर्ण_अगर

#अगर_अघोषित traceQUEUE_SEND_FROM_ISR_FAILED
    #घोषणा traceQUEUE_SEND_FROM_ISR_FAILED( pxQueue )
#पूर्ण_अगर

#अगर_अघोषित traceQUEUE_RECEIVE_FROM_ISR
    #घोषणा traceQUEUE_RECEIVE_FROM_ISR( pxQueue )
#पूर्ण_अगर

#अगर_अघोषित traceQUEUE_RECEIVE_FROM_ISR_FAILED
    #घोषणा traceQUEUE_RECEIVE_FROM_ISR_FAILED( pxQueue )
#पूर्ण_अगर

#अगर_अघोषित traceQUEUE_PEEK_FROM_ISR_FAILED
    #घोषणा traceQUEUE_PEEK_FROM_ISR_FAILED( pxQueue )
#पूर्ण_अगर

#अगर_अघोषित traceQUEUE_DELETE
    #घोषणा traceQUEUE_DELETE( pxQueue )
#पूर्ण_अगर

#अगर_अघोषित traceTASK_CREATE
    #घोषणा traceTASK_CREATE( pxNewTCB )
#पूर्ण_अगर

#अगर_अघोषित traceTASK_CREATE_FAILED
    #घोषणा traceTASK_CREATE_FAILED()
#पूर्ण_अगर

#अगर_अघोषित traceTASK_DELETE
    #घोषणा traceTASK_DELETE( pxTaskToDelete )
#पूर्ण_अगर

#अगर_अघोषित traceTASK_DELAY_UNTIL
    #घोषणा traceTASK_DELAY_UNTIL( x )
#पूर्ण_अगर

#अगर_अघोषित traceTASK_DELAY
    #घोषणा traceTASK_DELAY()
#पूर्ण_अगर

#अगर_अघोषित traceTASK_PRIORITY_SET
    #घोषणा traceTASK_PRIORITY_SET( pxTask, uxNewPriority )
#पूर्ण_अगर

#अगर_अघोषित traceTASK_SUSPEND
    #घोषणा traceTASK_SUSPEND( pxTaskToSuspend )
#पूर्ण_अगर

#अगर_अघोषित traceTASK_RESUME
    #घोषणा traceTASK_RESUME( pxTaskToResume )
#पूर्ण_अगर

#अगर_अघोषित traceTASK_RESUME_FROM_ISR
    #घोषणा traceTASK_RESUME_FROM_ISR( pxTaskToResume )
#पूर्ण_अगर

#अगर_अघोषित traceTASK_INCREMENT_TICK
    #घोषणा traceTASK_INCREMENT_TICK( x_Tick_गिनती )
#पूर्ण_अगर

#अगर_अघोषित traceTIMER_CREATE
    #घोषणा traceTIMER_CREATE( pxNewTimer )
#पूर्ण_अगर

#अगर_अघोषित traceTIMER_CREATE_FAILED
    #घोषणा traceTIMER_CREATE_FAILED()
#पूर्ण_अगर

#अगर_अघोषित traceTIMER_COMMAND_SEND
    #घोषणा traceTIMER_COMMAND_SEND( xTimer, xMessageID, x_Message_मूल्य_मूल्य, xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceTIMER_EXPIRED
    #घोषणा traceTIMER_EXPIRED( pxTimer )
#पूर्ण_अगर

#अगर_अघोषित traceTIMER_COMMAND_RECEIVED
    #घोषणा traceTIMER_COMMAND_RECEIVED( pxTimer, xMessageID, x_Message_मूल्य )
#पूर्ण_अगर

#अगर_अघोषित traceMALLOC
    #घोषणा traceMALLOC( pvAddress, ui_आकार )
#पूर्ण_अगर

#अगर_अघोषित traceFREE
    #घोषणा traceFREE( pvAddress, ui_आकार )
#पूर्ण_अगर

#अगर_अघोषित traceEVENT_GROUP_CREATE
    #घोषणा traceEVENT_GROUP_CREATE( xEventGroup )
#पूर्ण_अगर

#अगर_अघोषित traceEVENT_GROUP_CREATE_FAILED
    #घोषणा traceEVENT_GROUP_CREATE_FAILED()
#पूर्ण_अगर

#अगर_अघोषित traceEVENT_GROUP_SYNC_BLOCK
    #घोषणा traceEVENT_GROUP_SYNC_BLOCK( xEventGroup, uxBitsToSet, uxBitsToWaitFor )
#पूर्ण_अगर

#अगर_अघोषित traceEVENT_GROUP_SYNC_END
    #घोषणा traceEVENT_GROUP_SYNC_END( xEventGroup, uxBitsToSet, uxBitsToWaitFor, xTimeoutOccurred )    ( व्योम ) ( xTimeoutOccurred )
#पूर्ण_अगर

#अगर_अघोषित traceEVENT_GROUP_WAIT_BITS_BLOCK
    #घोषणा traceEVENT_GROUP_WAIT_BITS_BLOCK( xEventGroup, uxBitsToWaitFor )
#पूर्ण_अगर

#अगर_अघोषित traceEVENT_GROUP_WAIT_BITS_END
    #घोषणा traceEVENT_GROUP_WAIT_BITS_END( xEventGroup, uxBitsToWaitFor, xTimeoutOccurred )    ( व्योम ) ( xTimeoutOccurred )
#पूर्ण_अगर

#अगर_अघोषित traceEVENT_GROUP_CLEAR_BITS
    #घोषणा traceEVENT_GROUP_CLEAR_BITS( xEventGroup, uxBitsToClear )
#पूर्ण_अगर

#अगर_अघोषित traceEVENT_GROUP_CLEAR_BITS_FROM_ISR
    #घोषणा traceEVENT_GROUP_CLEAR_BITS_FROM_ISR( xEventGroup, uxBitsToClear )
#पूर्ण_अगर

#अगर_अघोषित traceEVENT_GROUP_SET_BITS
    #घोषणा traceEVENT_GROUP_SET_BITS( xEventGroup, uxBitsToSet )
#पूर्ण_अगर

#अगर_अघोषित traceEVENT_GROUP_SET_BITS_FROM_ISR
    #घोषणा traceEVENT_GROUP_SET_BITS_FROM_ISR( xEventGroup, uxBitsToSet )
#पूर्ण_अगर

#अगर_अघोषित traceEVENT_GROUP_DELETE
    #घोषणा traceEVENT_GROUP_DELETE( xEventGroup )
#पूर्ण_अगर

#अगर_अघोषित tracePEND_FUNC_CALL
    #घोषणा tracePEND_FUNC_CALL( xFunctionToPend, pvParameter1, ulParameter2, ret )
#पूर्ण_अगर

#अगर_अघोषित tracePEND_FUNC_CALL_FROM_ISR
    #घोषणा tracePEND_FUNC_CALL_FROM_ISR( xFunctionToPend, pvParameter1, ulParameter2, ret )
#पूर्ण_अगर

#अगर_अघोषित traceQUEUE_REGISTRY_ADD
    #घोषणा traceQUEUE_REGISTRY_ADD( xQueue, pcQueueName )
#पूर्ण_अगर

#अगर_अघोषित traceTASK_NOTIFY_TAKE_BLOCK
    #घोषणा traceTASK_NOTIFY_TAKE_BLOCK( ux_सूचकांक_To_Wait )
#पूर्ण_अगर

#अगर_अघोषित traceTASK_NOTIFY_TAKE
    #घोषणा traceTASK_NOTIFY_TAKE( ux_सूचकांक_To_Wait )
#पूर्ण_अगर

#अगर_अघोषित traceTASK_NOTIFY_WAIT_BLOCK
    #घोषणा traceTASK_NOTIFY_WAIT_BLOCK( ux_सूचकांक_To_Wait )
#पूर्ण_अगर

#अगर_अघोषित traceTASK_NOTIFY_WAIT
    #घोषणा traceTASK_NOTIFY_WAIT( ux_सूचकांक_To_Wait )
#पूर्ण_अगर

#अगर_अघोषित traceTASK_NOTIFY
    #घोषणा traceTASK_NOTIFY( ux_सूचकांक_To_Notify )
#पूर्ण_अगर

#अगर_अघोषित traceTASK_NOTIFY_FROM_ISR
    #घोषणा traceTASK_NOTIFY_FROM_ISR( ux_सूचकांक_To_Notify )
#पूर्ण_अगर

#अगर_अघोषित traceTASK_NOTIFY_GIVE_FROM_ISR
    #घोषणा traceTASK_NOTIFY_GIVE_FROM_ISR( ux_सूचकांक_To_Notify )
#पूर्ण_अगर

#अगर_अघोषित trace_STREAM_बफर_CREATE_FAILED
    #घोषणा trace_STREAM_बफर_CREATE_FAILED( x_Is_Message_बफर )
#पूर्ण_अगर

#अगर_अघोषित trace_STREAM_बफर_CREATE_STATIC_FAILED
    #घोषणा trace_STREAM_बफर_CREATE_STATIC_FAILED( xReturn, x_Is_Message_बफर )
#पूर्ण_अगर

#अगर_अघोषित trace_STREAM_बफर_CREATE
    #घोषणा trace_STREAM_बफर_CREATE( px_Stream_बफर, x_Is_Message_बफर )
#पूर्ण_अगर

#अगर_अघोषित trace_STREAM_बफर_DELETE
    #घोषणा trace_STREAM_बफर_DELETE( x_Stream_बफर )
#पूर्ण_अगर

#अगर_अघोषित trace_STREAM_बफर_RESET
    #घोषणा trace_STREAM_बफर_RESET( x_Stream_बफर )
#पूर्ण_अगर

#अगर_अघोषित trace_BLOCKING_ON_STREAM_बफर_SEND
    #घोषणा trace_BLOCKING_ON_STREAM_बफर_SEND( x_Stream_बफर )
#पूर्ण_अगर

#अगर_अघोषित trace_STREAM_बफर_SEND
    #घोषणा trace_STREAM_बफर_SEND( x_Stream_बफर, xBytesSent )
#पूर्ण_अगर

#अगर_अघोषित trace_STREAM_बफर_SEND_FAILED
    #घोषणा trace_STREAM_बफर_SEND_FAILED( x_Stream_बफर )
#पूर्ण_अगर

#अगर_अघोषित trace_STREAM_बफर_SEND_FROM_ISR
    #घोषणा trace_STREAM_बफर_SEND_FROM_ISR( x_Stream_बफर, xBytesSent )
#पूर्ण_अगर

#अगर_अघोषित trace_BLOCKING_ON_STREAM_बफर_RECEIVE
    #घोषणा trace_BLOCKING_ON_STREAM_बफर_RECEIVE( x_Stream_बफर )
#पूर्ण_अगर

#अगर_अघोषित trace_STREAM_बफर_RECEIVE
    #घोषणा trace_STREAM_बफर_RECEIVE( x_Stream_बफर, x_Received_लंबाई )
#पूर्ण_अगर

#अगर_अघोषित trace_STREAM_बफर_RECEIVE_FAILED
    #घोषणा trace_STREAM_बफर_RECEIVE_FAILED( x_Stream_बफर )
#पूर्ण_अगर

#अगर_अघोषित trace_STREAM_बफर_RECEIVE_FROM_ISR
    #घोषणा trace_STREAM_बफर_RECEIVE_FROM_ISR( x_Stream_बफर, x_Received_लंबाई )
#पूर्ण_अगर

#अगर_अघोषित configGENERATE_RUN_TIME_STATS
    #घोषणा configGENERATE_RUN_TIME_STATS    0
#पूर्ण_अगर

#अगर ( configGENERATE_RUN_TIME_STATS == 1 )

    #अगर_अघोषित portCONFIGURE_TIMER_FOR_RUN_TIME_STATS
        #त्रुटि If configGENERATE_RUN_TIME_STATS is defined then portCONFIGURE_TIMER_FOR_RUN_TIME_STATS must also be defined.  portCONFIGURE_TIMER_FOR_RUN_TIME_STATS should call a port layer function to setup a peripheral timer/गणक that can then be used as the run समय गणक समय base.
    #पूर्ण_अगर /* portCONFIGURE_TIMER_FOR_RUN_TIME_STATS */

    #अगर_अघोषित port_GET_RUN_TIME_गणक_मूल्य
        #अगर_अघोषित port_ALT_GET_RUN_TIME_गणक_मूल्य
            #त्रुटि If configGENERATE_RUN_TIME_STATS is defined then either port_GET_RUN_TIME_गणक_मूल्य or port_ALT_GET_RUN_TIME_गणक_मूल्य must also be defined.  See the examples provided and the FreeRTOS web site क्रम more information.
        #पूर्ण_अगर /* portALT_GET_RUN_TIME_COUNTER_VALUE */
    #पूर्ण_अगर /* portGET_RUN_TIME_COUNTER_VALUE */

#पूर्ण_अगर /* configGENERATE_RUN_TIME_STATS */

#अगर_अघोषित portCONFIGURE_TIMER_FOR_RUN_TIME_STATS
    #घोषणा portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()
#पूर्ण_अगर

#अगर_अघोषित configUSE_MALLOC_FAILED_HOOK
    #घोषणा configUSE_MALLOC_FAILED_HOOK    0
#पूर्ण_अगर

#अगर_अघोषित portPRIVILEGE_BIT
    #घोषणा portPRIVILEGE_BIT    ( ( UBaseType_t ) 0x00 )
#पूर्ण_अगर

#अगर_अघोषित portYIELD_WITHIN_API
    #घोषणा portYIELD_WITHIN_API    portYIELD
#पूर्ण_अगर

#अगर_अघोषित portSUPPRESS_TICKS_AND_SLEEP
    #घोषणा portSUPPRESS_TICKS_AND_SLEEP( xExpectedIdleTime )
#पूर्ण_अगर

#अगर_अघोषित configEXPECTED_IDLE_TIME_BEFORE_SLEEP
    #घोषणा configEXPECTED_IDLE_TIME_BEFORE_SLEEP    2
#पूर्ण_अगर

#अगर configEXPECTED_IDLE_TIME_BEFORE_SLEEP < 2
    #त्रुटि configEXPECTED_IDLE_TIME_BEFORE_SLEEP must not be less than 2
#पूर्ण_अगर

#अगर_अघोषित configUSE_TICKLESS_IDLE
    #घोषणा configUSE_TICKLESS_IDLE    0
#पूर्ण_अगर

#अगर_अघोषित configPRE_SUPPRESS_TICKS_AND_SLEEP_PROCESSING
    #घोषणा configPRE_SUPPRESS_TICKS_AND_SLEEP_PROCESSING( x )
#पूर्ण_अगर

#अगर_अघोषित configPRE_SLEEP_PROCESSING
    #घोषणा configPRE_SLEEP_PROCESSING( x )
#पूर्ण_अगर

#अगर_अघोषित configPOST_SLEEP_PROCESSING
    #घोषणा configPOST_SLEEP_PROCESSING( x )
#पूर्ण_अगर

#अगर_अघोषित configUSE_QUEUE_SETS
    #घोषणा configUSE_QUEUE_SETS    0
#पूर्ण_अगर

#अगर_अघोषित portTASK_USES_FLOATING_POINT
    #घोषणा portTASK_USES_FLOATING_POINT()
#पूर्ण_अगर

#अगर_अघोषित portALLOCATE_SECURE_CONTEXT
    #घोषणा portALLOCATE_SECURE_CONTEXT( ul_Secure_ढेर_आकार )
#पूर्ण_अगर

#अगर_अघोषित portDONT_DISCARD
    #घोषणा portDONT_DISCARD
#पूर्ण_अगर

#अगर_अघोषित configUSE_TIME_SLICING
    #घोषणा configUSE_TIME_SLICING    1
#पूर्ण_अगर

#अगर_अघोषित configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS
    #घोषणा configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS    0
#पूर्ण_अगर

#अगर_अघोषित configUSE_STATS_FORMATTING_FUNCTIONS
    #घोषणा configUSE_STATS_FORMATTING_FUNCTIONS    0
#पूर्ण_अगर

#अगर_अघोषित portASSERT_IF_INTERRUPT_PRIORITY_INVALID
    #घोषणा portASSERT_IF_INTERRUPT_PRIORITY_INVALID()
#पूर्ण_अगर

#अगर_अघोषित configUSE_TRACE_FACILITY
    #घोषणा configUSE_TRACE_FACILITY    0
#पूर्ण_अगर

#अगर_अघोषित mtCOVERAGE_TEST_MARKER
    #घोषणा mtCOVERAGE_TEST_MARKER()
#पूर्ण_अगर

#अगर_अघोषित mtCOVERAGE_TEST_DELAY
    #घोषणा mtCOVERAGE_TEST_DELAY()
#पूर्ण_अगर

#अगर_अघोषित portASSERT_IF_IN_ISR
    #घोषणा portASSERT_IF_IN_ISR()
#पूर्ण_अगर

#अगर_अघोषित configUSE_PORT_OPTIMISED_TASK_SELECTION
    #घोषणा configUSE_PORT_OPTIMISED_TASK_SELECTION    0
#पूर्ण_अगर

#अगर_अघोषित configAPPLICATION_ALLOCATED_HEAP
    #घोषणा configAPPLICATION_ALLOCATED_HEAP    0
#पूर्ण_अगर

#अगर_अघोषित configUSE_TASK_NOTIFICATIONS
    #घोषणा configUSE_TASK_NOTIFICATIONS    1
#पूर्ण_अगर

#अगर_अघोषित configTASK_NOTIFICATION_ARRAY_ENTRIES
    #घोषणा configTASK_NOTIFICATION_ARRAY_ENTRIES    1
#पूर्ण_अगर

#अगर configTASK_NOTIFICATION_ARRAY_ENTRIES < 1
    #त्रुटि configTASK_NOTIFICATION_ARRAY_ENTRIES must be at least 1
#पूर्ण_अगर

#अगर_अघोषित configUSE_POSIX_ERRNO
    #घोषणा configUSE_POSIX_ERRNO    0
#पूर्ण_अगर

#अगर_अघोषित configUSE_SB_COMPLETED_CALLBACK

/* By default per-instance callbacks are not enabled for stream buffer or message buffer. */
    #घोषणा configUSE_SB_COMPLETED_CALLBACK    0
#पूर्ण_अगर

#अगर_अघोषित portTICK_TYPE_IS_ATOMIC
    #घोषणा portTICK_TYPE_IS_ATOMIC    0
#पूर्ण_अगर

#अगर_अघोषित configSUPPORT_STATIC_ALLOCATION
    /* Defaults to 0 for backward compatibility. */
    #घोषणा configSUPPORT_STATIC_ALLOCATION    0
#पूर्ण_अगर

#अगर_अघोषित configSUPPORT_DYNAMIC_ALLOCATION
    /* Defaults to 1 for backward compatibility. */
    #घोषणा configSUPPORT_DYNAMIC_ALLOCATION    1
#पूर्ण_अगर

#अगर ( ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 ) && ( configSUPPORT_DYNAMIC_ALLOCATION != 1 ) )
    #त्रुटि configUSE_STATS_FORMATTING_FUNCTIONS cannot be used without dynamic allocation, but configSUPPORT_DYNAMIC_ALLOCATION is not set to 1.
#पूर्ण_अगर

#अगर ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 )
    #अगर ( ( configUSE_TRACE_FACILITY != 1 ) && ( configGENERATE_RUN_TIME_STATS != 1 ) )
        #त्रुटि configUSE_STATS_FORMATTING_FUNCTIONS is 1 but the functions it enables are not used because neither configUSE_TRACE_FACILITY or configGENERATE_RUN_TIME_STATS are 1.  Set configUSE_STATS_FORMATTING_FUNCTIONS to 0 in FreeRTOSConfig.h.
    #पूर्ण_अगर
#पूर्ण_अगर

#अगर_अघोषित config_ढेर_DEPTH_TYPE

/* Defaults to uint16_t for backward compatibility, but can be overridden
 * in FreeRTOSConfig.h if uint16_t is too restrictive. */
    #घोषणा config_ढेर_DEPTH_TYPE    uint16_t
#पूर्ण_अगर

#अगर_अघोषित config_RUN_TIME_गणक_TYPE

/* Defaults to uint32_t for backward compatibility, but can be overridden in
 * FreeRTOSConfig.h if uint32_t is too restrictive. */

    #घोषणा config_RUN_TIME_गणक_TYPE    uint32_t
#पूर्ण_अगर

#अगर_अघोषित config_MESSAGE_बफर_लंबाई_TYPE

/* Defaults to size_t for backward compatibility, but can be overridden
 * in FreeRTOSConfig.h if lengths will always be less than the number of bytes
 * in a size_t. */
    #घोषणा config_MESSAGE_बफर_लंबाई_TYPE    माप_प्रकार
#पूर्ण_अगर

/* Sanity check the configuration. */
#अगर ( ( configSUPPORT_STATIC_ALLOCATION == 0 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 0 ) )
    #त्रुटि configSUPPORT_STATIC_ALLOCATION and configSUPPORT_DYNAMIC_ALLOCATION cannot both be 0, but can both be 1.
#पूर्ण_अगर

#अगर ( ( configUSE_RECURSIVE_MUTEXES == 1 ) && ( configUSE_MUTEXES != 1 ) )
    #त्रुटि configUSE_MUTEXES must be set to 1 to use recursive mutexes
#पूर्ण_अगर

#अगर ( ( config_संख्या_OF_CORES > 1 ) && ( configUSE_PORT_OPTIMISED_TASK_SELECTION != 0 ) )
    #त्रुटि configUSE_PORT_OPTIMISED_TASK_SELECTION is not supported in SMP
#पूर्ण_अगर

#अगर_अघोषित config_INITIAL_TICK_गिनती
    #घोषणा config_INITIAL_TICK_गिनती    0
#पूर्ण_अगर

#अगर ( portTICK_TYPE_IS_ATOMIC == 0 )

/* Either variables of tick type cannot be read atomically, or
 * portTICK_TYPE_IS_ATOMIC was not set - map the critical sections used when
 * the tick count is returned to the standard critical section macros. */
    #घोषणा portTICK_TYPE_ENTER_CRITICAL()                      portENTER_CRITICAL()
    #घोषणा portTICK_TYPE_EXIT_CRITICAL()                       portEXIT_CRITICAL()
    #घोषणा portTICK_TYPE_SET_INTERRUPT_MASK_FROM_ISR()         portSET_INTERRUPT_MASK_FROM_ISR()
    #घोषणा portTICK_TYPE_CLEAR_INTERRUPT_MASK_FROM_ISR( x )    portCLEAR_INTERRUPT_MASK_FROM_ISR( ( x ) )
#अन्यथा

/* The tick type can be read atomically, so critical sections used when the
 * tick count is returned can be defined away. */
    #घोषणा portTICK_TYPE_ENTER_CRITICAL()
    #घोषणा portTICK_TYPE_EXIT_CRITICAL()
    #घोषणा portTICK_TYPE_SET_INTERRUPT_MASK_FROM_ISR()         0
    #घोषणा portTICK_TYPE_CLEAR_INTERRUPT_MASK_FROM_ISR( x )    ( व्योम ) ( x )
#पूर्ण_अगर /* if ( portTICK_TYPE_IS_ATOMIC == 0 ) */

/* Definitions to allow backward compatibility with FreeRTOS versions prior to
 * V8 if desired. */
#अगर_अघोषित configENABLE_BACKWARD_COMPATIBILITY
    #घोषणा configENABLE_BACKWARD_COMPATIBILITY    1
#पूर्ण_अगर

#अगर_अघोषित configPRINTF

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
    #घोषणा configPRINTF( X )
#पूर्ण_अगर

#अगर_अघोषित configMAX

/* The application writer has not provided their own MAX macro, so define
 * the following generic implementation. */
    #घोषणा configMAX( a, b )    ( ( ( a ) > ( b ) ) ? ( a ) : ( b ) )
#पूर्ण_अगर

#अगर_अघोषित configMIN

/* The application writer has not provided their own MIN macro, so define
 * the following generic implementation. */
    #घोषणा configMIN( a, b )    ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )
#पूर्ण_अगर

#अगर configENABLE_BACKWARD_COMPATIBILITY == 1
    #घोषणा eTaskStateGet                 eTaskGetState
    #घोषणा portTickType                  TickType_t
    #घोषणा xTaskHandle                   TaskHandle_t
    #घोषणा xQueueHandle                  QueueHandle_t
    #घोषणा xSemaphoreHandle              SemaphoreHandle_t
    #घोषणा xQueueSetHandle               QueueSetHandle_t
    #घोषणा xQueueSetMemberHandle         QueueSetMemberHandle_t
    #घोषणा xTimeOutType                  TimeOut_t
    #घोषणा xMemoryRegion                 MemoryRegion_t
    #घोषणा xTaskParameters               TaskParameters_t
    #घोषणा xTaskStatusType               TaskStatus_t
    #घोषणा xTimerHandle                  TimerHandle_t
    #घोषणा xCoRoutineHandle              CoRoutineHandle_t
    #घोषणा pdTASK_HOOK_CODE              TaskHookFunction_t
    #घोषणा portTICK_RATE_MS              portTICK_PERIOD_MS
    #घोषणा pcTaskGetTaskName             pcTaskGetName
    #घोषणा pcTimerGetTimerName           pcTimerGetName
    #घोषणा pcQueueGetQueueName           pcQueueGetName
    #घोषणा vTaskGetTaskInfo              vTaskGetInfo
    #घोषणा x_Task_Get_Idle_Run_Time_गणक    ul_Task_Get_Idle_Run_Time_गणक

/* Backward compatibility within the scheduler code only - these definitions
 * are not really required but are included for completeness. */
    #घोषणा tmrTIMER_CALLBACK             TimerCallbackFunction_t
    #घोषणा pdTASK_CODE                   TaskFunction_t
    #घोषणा xListItem                     ListItem_t
    #घोषणा xList                         List_t

/* For libraries that break the list data hiding, and access list structure
 * members directly (which is not supposed to be done). */
    #घोषणा pxContainer                   pvContainer
#पूर्ण_अगर /* configENABLE_BACKWARD_COMPATIBILITY */

#अगर ( configUSE_ALTERNATIVE_API != 0 )
    #त्रुटि The alternative API was deprecated some समय ago, and was removed in FreeRTOS V9.0 0
#पूर्ण_अगर

/* Set configUSE_TASK_FPU_SUPPORT to 0 to omit floating point support even
 * if floating point hardware is otherwise supported by the FreeRTOS port in use.
 * This constant is not supported by all FreeRTOS ports that include floating
 * point support. */
#अगर_अघोषित configUSE_TASK_FPU_SUPPORT
    #घोषणा configUSE_TASK_FPU_SUPPORT    1
#पूर्ण_अगर

/* Set configENABLE_MPU to 1 to enable MPU support and 0 to disable it. This is
 * currently used in ARMv8M ports. */
#अगर_अघोषित configENABLE_MPU
    #घोषणा configENABLE_MPU    0
#पूर्ण_अगर

/* Set configENABLE_FPU to 1 to enable FPU support and 0 to disable it. This is
 * currently used in ARMv8M ports. */
#अगर_अघोषित configENABLE_FPU
    #घोषणा configENABLE_FPU    1
#पूर्ण_अगर

/* Set configENABLE_MVE to 1 to enable MVE support and 0 to disable it. This is
 * currently used in ARMv8M ports. */
#अगर_अघोषित configENABLE_MVE
    #घोषणा configENABLE_MVE    0
#पूर्ण_अगर

/* Set configENABLE_TRUSTZONE to 1 enable TrustZone support and 0 to disable it.
 * This is currently used in ARMv8M ports. */
#अगर_अघोषित configENABLE_TRUSTZONE
    #घोषणा configENABLE_TRUSTZONE    1
#पूर्ण_अगर

/* Set configRUN_FREERTOS_SECURE_ONLY to 1 to run the FreeRTOS ARMv8M port on
 * the Secure Side only. */
#अगर_अघोषित configRUN_FREERTOS_SECURE_ONLY
    #घोषणा configRUN_FREERTOS_SECURE_ONLY    0
#पूर्ण_अगर

#अगर_अघोषित configRUN_ADDITIONAL_TESTS
    #घोषणा configRUN_ADDITIONAL_TESTS    0
#पूर्ण_अगर


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
#घोषणा tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE                                                                                     \
    ( ( ( portUSING_MPU_WRAPPERS == 0 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) && ( configSUPPORT_STATIC_ALLOCATION == 1 ) ) || \
      ( ( portUSING_MPU_WRAPPERS == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) ) )

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
काष्ठा xSTATIC_LIST_ITEM
{
    #अगर ( config_USE_LIST_तथ्य_INTEGRITY_CHECK_BYTES == 1 )
        TickType_t xDummy1;
    #पूर्ण_अगर
    TickType_t xDummy2;
    व्योम * pvDummy3[ 4 ];
    #अगर ( config_USE_LIST_तथ्य_INTEGRITY_CHECK_BYTES == 1 )
        TickType_t xDummy4;
    #पूर्ण_अगर
};
प्रकार काष्ठा xSTATIC_LIST_ITEM StaticListItem_t;

#अगर ( configUSE_MINI_LIST_ITEM == 1 )
    /* See the comments above the struct xSTATIC_LIST_ITEM definition. */
    काष्ठा xSTATIC_MINI_LIST_ITEM
    {
        #अगर ( config_USE_LIST_तथ्य_INTEGRITY_CHECK_BYTES == 1 )
            TickType_t xDummy1;
        #पूर्ण_अगर
        TickType_t xDummy2;
        व्योम * pvDummy3[ 2 ];
    };
    प्रकार काष्ठा xSTATIC_MINI_LIST_ITEM StaticMiniListItem_t;
#अन्यथा /* if ( configUSE_MINI_LIST_ITEM == 1 ) */
    प्रकार काष्ठा xSTATIC_LIST_ITEM      StaticMiniListItem_t;
#पूर्ण_अगर /* if ( configUSE_MINI_LIST_ITEM == 1 ) */

/* See the comments above the struct xSTATIC_LIST_ITEM definition. */
प्रकार काष्ठा xSTATIC_LIST
{
    #अगर ( config_USE_LIST_तथ्य_INTEGRITY_CHECK_BYTES == 1 )
        TickType_t xDummy1;
    #पूर्ण_अगर
    UBaseType_t uxDummy2;
    व्योम * pvDummy3;
    StaticMiniListItem_t xDummy4;
    #अगर ( config_USE_LIST_तथ्य_INTEGRITY_CHECK_BYTES == 1 )
        TickType_t xDummy5;
    #पूर्ण_अगर
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
प्रकार काष्ठा xSTATIC_TCB
{
    व्योम * pxDummy1;
    #अगर ( portUSING_MPU_WRAPPERS == 1 )
        xMPU_SETTINGS xDummy2;
    #पूर्ण_अगर
    StaticListItem_t xDummy3[ 2 ];
    UBaseType_t uxDummy5;
    व्योम * pxDummy6;
    uint8_t ucDummy7[ configMAX_TASK_NAME_LEN ];
    #अगर ( config_संख्या_OF_CORES > 1 )
        BaseType_t xDummyCoreID;
    #पूर्ण_अगर /* configNUMBER_OF_CORES > 1 */
    #अगर ( ( port_ढेर_GROWTH > 0 ) || ( config_RECORD_ढेर_ऊपरी_ADDRESS == 1 ) )
        व्योम * pxDummy8;
    #पूर्ण_अगर
    #अगर ( portCRITICAL_NESTING_IN_TCB == 1 )
        UBaseType_t uxDummy9;
    #पूर्ण_अगर
    #अगर ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy10[ 2 ];
    #पूर्ण_अगर
    #अगर ( configUSE_MUTEXES == 1 )
        UBaseType_t uxDummy12[ 2 ];
    #पूर्ण_अगर
    #अगर ( configUSE_APPLICATION_TASK_TAG == 1 )
        व्योम * pxDummy14;
    #पूर्ण_अगर
    #अगर ( config_अंक_THREAD_LOCAL_STORAGE_POINTERS > 0 )
        व्योम * pvDummy15[ config_अंक_THREAD_LOCAL_STORAGE_POINTERS ];
    #पूर्ण_अगर
    #अगर ( configGENERATE_RUN_TIME_STATS == 1 )
        config_RUN_TIME_गणक_TYPE ulDummy16;
    #पूर्ण_अगर
    #अगर ( ( configUSE_NEWLIB_REENTRANT == 1 ) || ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 ) )
        configTLS_BLOCK_TYPE xDummy17;
    #पूर्ण_अगर
    #अगर ( configUSE_TASK_NOTIFICATIONS == 1 )
        uint32_t ulDummy18[ configTASK_NOTIFICATION_ARRAY_ENTRIES ];
        uint8_t ucDummy19[ configTASK_NOTIFICATION_ARRAY_ENTRIES ];
    #पूर्ण_अगर
    #अगर ( tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE != 0 )
        uint8_t uxDummy20;
    #पूर्ण_अगर

    #अगर ( INCLUDE_xTaskAbortDelay == 1 )
        uint8_t ucDummy21;
    #पूर्ण_अगर
    #अगर ( configUSE_POSIX_ERRNO == 1 )
        पूर्णांक iDummy22;
    #पूर्ण_अगर
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
प्रकार काष्ठा xSTATIC_QUEUE
{
    व्योम * pvDummy1[ 3 ];

    जोड़
    {
        व्योम * pvDummy2;
        UBaseType_t uxDummy2;
    } u;

    StaticList_t xDummy3[ 2 ];
    UBaseType_t uxDummy4[ 3 ];
    uint8_t ucDummy5[ 2 ];

    #अगर ( ( configSUPPORT_STATIC_ALLOCATION == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) )
        uint8_t ucDummy6;
    #पूर्ण_अगर

    #अगर ( configUSE_QUEUE_SETS == 1 )
        व्योम * pvDummy7;
    #पूर्ण_अगर

    #अगर ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy8;
        uint8_t ucDummy9;
    #पूर्ण_अगर
    portMUX_TYPE xDummyQueueLock;
} StaticQueue_t;
प्रकार StaticQueue_t StaticSemaphore_t;

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
प्रकार काष्ठा xSTATIC_EVENT_GROUP
{
    TickType_t xDummy1;
    StaticList_t xDummy2;

    #अगर ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy3;
    #पूर्ण_अगर

    #अगर ( ( configSUPPORT_STATIC_ALLOCATION == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) )
        uint8_t ucDummy4;
    #पूर्ण_अगर
    portMUX_TYPE xDummyEventGroupLock;
} StaticEventGroup_t;

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
प्रकार काष्ठा xSTATIC_TIMER
{
    व्योम * pvDummy1;
    StaticListItem_t xDummy2;
    TickType_t xDummy3;
    व्योम * pvDummy5;
    TaskFunction_t pvDummy6;
    #अगर ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy7;
    #पूर्ण_अगर
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
प्रकार काष्ठा x_STATIC_STREAM_बफर
{
    माप_प्रकार uxDummy1[ 4 ];
    व्योम * pvDummy2[ 3 ];
    uint8_t ucDummy3;
    #अगर ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy4;
    #पूर्ण_अगर
    #अगर ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        व्योम * pvDummy5[ 2 ];
    #पूर्ण_अगर
    portMUX_TYPE x_Dummy_Stream_बफर_Lock;
} Static_Stream_बफर_t;

/* Message buffers are built on stream buffers. */
प्रकार Static_Stream_बफर_t Static_Message_बफर_t;

/* *INDENT-OFF* */
#अगर_घोषित __cplusplus
    }
#पूर्ण_अगर
/* *INDENT-ON* */

/*-----------------------------------------------------------
* IDF Compatibility
*----------------------------------------------------------*/

#अगर_घोषित ESP_PLATFORM

/* *INDENT-OFF* */
#अगर_घोषित __cplusplus
    बाह्य "C" {
#पूर्ण_अगर
/* *INDENT-ON* */

/*
 * Default values for trace macros added by ESP-IDF and are not part of Vanilla FreeRTOS
 */

    #अगर_अघोषित traceISR_EXIT_TO_SCHEDULER
        #घोषणा traceISR_EXIT_TO_SCHEDULER()
    #पूर्ण_अगर

    #अगर_अघोषित traceISR_EXIT
        #घोषणा traceISR_EXIT()
    #पूर्ण_अगर

    #अगर_अघोषित traceISR_ENTER
        #घोषणा traceISR_ENTER( _n_ )
    #पूर्ण_अगर

    #अगर_अघोषित traceQUEUE_SEMAPHORE_RECEIVE
        #घोषणा traceQUEUE_SEMAPHORE_RECEIVE( pxQueue )
    #पूर्ण_अगर

    #अगर_अघोषित traceQUEUE_GIVE_FROM_ISR
        #घोषणा traceQUEUE_GIVE_FROM_ISR( pxQueue )
    #पूर्ण_अगर

    #अगर_अघोषित traceQUEUE_GIVE_FROM_ISR_FAILED
        #घोषणा traceQUEUE_GIVE_FROM_ISR_FAILED( pxQueue )
    #पूर्ण_अगर

/* *INDENT-OFF* */
#अगर_घोषित __cplusplus
    }
#पूर्ण_अगर
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
    #समावेश "freertos/idf_additions.h"

#पूर्ण_अगर /* ESP_PLATFORM */

#पूर्ण_अगर /* INC_FREERTOS_H */
