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

/* Acceptable values for configTICK_TYPE_WIDTH_IN_BITS. */
#घोषणा TICK_TYPE_WIDTH_16_BITS    0
#घोषणा TICK_TYPE_WIDTH_32_BITS    1
#घोषणा TICK_TYPE_WIDTH_64_BITS    2

/* Application specific configuration options. */
#समावेश "FreeRTOSConfig.h"

#अगर !defined( configUSE_16_BIT_TICKS ) && !defined( configTICK_TYPE_WIDTH_IN_BITS )
    #त्रुटि Missing definition:  One of configUSE_16_BIT_TICKS and configTICK_TYPE_WIDTH_IN_BITS must be defined in FreeRTOSConfig.h.  See the Configuration section of the FreeRTOS API documentation क्रम details.
#पूर्ण_अगर

#अगर defined( configUSE_16_BIT_TICKS ) && defined( configTICK_TYPE_WIDTH_IN_BITS )
    #त्रुटि Only one of configUSE_16_BIT_TICKS and configTICK_TYPE_WIDTH_IN_BITS must be defined in FreeRTOSConfig.h.  See the Configuration section of the FreeRTOS API documentation क्रम details.
#पूर्ण_अगर

/* Define configTICK_TYPE_WIDTH_IN_BITS according to the
 * value of configUSE_16_BIT_TICKS for backward compatibility. */
#अगर_अघोषित configTICK_TYPE_WIDTH_IN_BITS
    #अगर ( configUSE_16_BIT_TICKS == 1 )
        #घोषणा configTICK_TYPE_WIDTH_IN_BITS    TICK_TYPE_WIDTH_16_BITS
    #अन्यथा
        #घोषणा configTICK_TYPE_WIDTH_IN_BITS    TICK_TYPE_WIDTH_32_BITS
    #पूर्ण_अगर
#पूर्ण_अगर

/* Set configUSE_MPU_WRAPPERS_V1 to 1 to use MPU wrappers v1. */
#अगर_अघोषित configUSE_MPU_WRAPPERS_V1
    #घोषणा configUSE_MPU_WRAPPERS_V1    0
#पूर्ण_अगर

/* Set configENABLE_ACCESS_CONTROL_LIST to 1 to enable access control list support. */
#अगर_अघोषित configENABLE_ACCESS_CONTROL_LIST
    #घोषणा configENABLE_ACCESS_CONTROL_LIST    0
#पूर्ण_अगर

/* Set default value of configNUMBER_OF_CORES to 1 to use single core FreeRTOS. */
#अगर_अघोषित config_संख्या_OF_CORES
    #घोषणा config_संख्या_OF_CORES    1
#पूर्ण_अगर

#अगर_अघोषित configUSE_MALLOC_FAILED_HOOK
    #घोषणा configUSE_MALLOC_FAILED_HOOK    0
#पूर्ण_अगर

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

    #समावेश "newlib-freertos.h"

#पूर्ण_अगर /* if ( configUSE_NEWLIB_REENTRANT == 1 ) */

/* Must be defaulted before configUSE_PICOLIBC_TLS is used below. */
#अगर_अघोषित configUSE_PICOLIBC_TLS
    #घोषणा configUSE_PICOLIBC_TLS    0
#पूर्ण_अगर

#अगर ( configUSE_PICOLIBC_TLS == 1 )

    #समावेश "picolibc-freertos.h"

#पूर्ण_अगर /* if ( configUSE_PICOLIBC_TLS == 1 ) */

#अगर_अघोषित configUSE_C_RUNTIME_TLS_SUPPORT
    #घोषणा configUSE_C_RUNTIME_TLS_SUPPORT    0
#पूर्ण_अगर

#अगर ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 )

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
#पूर्ण_अगर /* if ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 ) */

/*
 * Check all the required application specific macros have been defined.
 * These macros are application specific and (as downloaded) are defined
 * within FreeRTOSConfig.h.
 */

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

#अगर ( config_संख्या_OF_CORES > 1 )
    #अगर_अघोषित configUSE_PASSIVE_IDLE_HOOK
        #त्रुटि Missing definition:  configUSE_PASSIVE_IDLE_HOOK must be defined in FreeRTOSConfig.h as either 1 or 0.  See the Configuration section of the FreeRTOS API documentation क्रम details.
    #पूर्ण_अगर
#पूर्ण_अगर

#अगर_अघोषित configUSE_TICK_HOOK
    #त्रुटि Missing definition:  configUSE_TICK_HOOK must be defined in FreeRTOSConfig.h as either 1 or 0.  See the Configuration section of the FreeRTOS API documentation क्रम details.
#पूर्ण_अगर

#अगर ( ( configTICK_TYPE_WIDTH_IN_BITS != TICK_TYPE_WIDTH_16_BITS ) && \
    ( configTICK_TYPE_WIDTH_IN_BITS != TICK_TYPE_WIDTH_32_BITS ) &&   \
    ( configTICK_TYPE_WIDTH_IN_BITS != TICK_TYPE_WIDTH_64_BITS ) )
    #त्रुटि Macro configTICK_TYPE_WIDTH_IN_BITS is defined to incorrect मूल्य.  See the Configuration section of the FreeRTOS API documentation क्रम details.
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

#अगर_अघोषित configUSE_EVENT_GROUPS
    #घोषणा configUSE_EVENT_GROUPS    1
#पूर्ण_अगर

#अगर_अघोषित configUSE_STREAM_BUFFERS
    #घोषणा configUSE_STREAM_BUFFERS    1
#पूर्ण_अगर

#अगर_अघोषित configUSE_DAEMON_TASK_STARTUP_HOOK
    #घोषणा configUSE_DAEMON_TASK_STARTUP_HOOK    0
#पूर्ण_अगर

#अगर ( configUSE_DAEMON_TASK_STARTUP_HOOK != 0 )
    #अगर ( configUSE_TIMERS == 0 )
        #त्रुटि configUSE_DAEMON_TASK_STARTUP_HOOK is set, but the daemon task is not created because configUSE_TIMERS is 0.
    #पूर्ण_अगर
#पूर्ण_अगर

#अगर_अघोषित configUSE_COUNTING_SEMAPHORES
    #घोषणा configUSE_COUNTING_SEMAPHORES    0
#पूर्ण_अगर

#अगर_अघोषित configUSE_TASK_PREEMPTION_DISABLE
    #घोषणा configUSE_TASK_PREEMPTION_DISABLE    0
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

#अगर_अघोषित configCHECK_HANDLER_INSTALLATION
    #घोषणा configCHECK_HANDLER_INSTALLATION    1
#अन्यथा

/* The application has explicitly defined configCHECK_HANDLER_INSTALLATION
 * to 1. The checks requires configASSERT() to be defined. */
    #अगर ( ( configCHECK_HANDLER_INSTALLATION == 1 ) && ( configASSERT_DEFINED == 0 ) )
        #त्रुटि You must define configASSERT() when configCHECK_HANDLER_INSTALLATION is 1.
    #पूर्ण_अगर
#पूर्ण_अगर

#अगर_अघोषित portMEMORY_BARRIER
    #घोषणा portMEMORY_BARRIER()
#पूर्ण_अगर

#अगर_अघोषित portSOFTWARE_BARRIER
    #घोषणा portSOFTWARE_BARRIER()
#पूर्ण_अगर

#अगर_अघोषित configRUN_MULTIPLE_PRIORITIES
    #घोषणा configRUN_MULTIPLE_PRIORITIES    0
#पूर्ण_अगर

#अगर_अघोषित portGET_CORE_ID

    #अगर ( config_संख्या_OF_CORES == 1 )
        #घोषणा portGET_CORE_ID()    0
    #अन्यथा
        #त्रुटि config_संख्या_OF_CORES is set to more than 1 then portGET_CORE_ID must also be defined.
    #पूर्ण_अगर /* configNUMBER_OF_CORES */

#पूर्ण_अगर /* portGET_CORE_ID */

#अगर_अघोषित portYIELD_CORE

    #अगर ( config_संख्या_OF_CORES == 1 )
        #घोषणा portYIELD_CORE( x )    portYIELD()
    #अन्यथा
        #त्रुटि config_संख्या_OF_CORES is set to more than 1 then portYIELD_CORE must also be defined.
    #पूर्ण_अगर /* configNUMBER_OF_CORES */

#पूर्ण_अगर /* portYIELD_CORE */

#अगर_अघोषित portSET_INTERRUPT_MASK

    #अगर ( config_संख्या_OF_CORES > 1 )
        #त्रुटि portSET_INTERRUPT_MASK is required in SMP
    #पूर्ण_अगर

#पूर्ण_अगर /* portSET_INTERRUPT_MASK */

#अगर_अघोषित portCLEAR_INTERRUPT_MASK

    #अगर ( config_संख्या_OF_CORES > 1 )
        #त्रुटि portCLEAR_INTERRUPT_MASK is required in SMP
    #पूर्ण_अगर

#पूर्ण_अगर /* portCLEAR_INTERRUPT_MASK */

#अगर_अघोषित portRELEASE_TASK_LOCK

    #अगर ( config_संख्या_OF_CORES == 1 )
        #घोषणा portRELEASE_TASK_LOCK()
    #अन्यथा
        #त्रुटि portRELEASE_TASK_LOCK is required in SMP
    #पूर्ण_अगर

#पूर्ण_अगर /* portRELEASE_TASK_LOCK */

#अगर_अघोषित portGET_TASK_LOCK

    #अगर ( config_संख्या_OF_CORES == 1 )
        #घोषणा portGET_TASK_LOCK()
    #अन्यथा
        #त्रुटि portGET_TASK_LOCK is required in SMP
    #पूर्ण_अगर

#पूर्ण_अगर /* portGET_TASK_LOCK */

#अगर_अघोषित portRELEASE_ISR_LOCK

    #अगर ( config_संख्या_OF_CORES == 1 )
        #घोषणा portRELEASE_ISR_LOCK()
    #अन्यथा
        #त्रुटि portRELEASE_ISR_LOCK is required in SMP
    #पूर्ण_अगर

#पूर्ण_अगर /* portRELEASE_ISR_LOCK */

#अगर_अघोषित portGET_ISR_LOCK

    #अगर ( config_संख्या_OF_CORES == 1 )
        #घोषणा portGET_ISR_LOCK()
    #अन्यथा
        #त्रुटि portGET_ISR_LOCK is required in SMP
    #पूर्ण_अगर

#पूर्ण_अगर /* portGET_ISR_LOCK */

#अगर_अघोषित portENTER_CRITICAL_FROM_ISR

    #अगर ( config_संख्या_OF_CORES > 1 )
        #त्रुटि portENTER_CRITICAL_FROM_ISR is required in SMP
    #पूर्ण_अगर

#पूर्ण_अगर

#अगर_अघोषित portEXIT_CRITICAL_FROM_ISR

    #अगर ( config_संख्या_OF_CORES > 1 )
        #त्रुटि portEXIT_CRITICAL_FROM_ISR is required in SMP
    #पूर्ण_अगर

#पूर्ण_अगर

#अगर_अघोषित configUSE_CORE_AFFINITY
    #घोषणा configUSE_CORE_AFFINITY    0
#पूर्ण_अगर /* configUSE_CORE_AFFINITY */

#अगर ( ( config_संख्या_OF_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
    #अगर_अघोषित configTASK_DEFAULT_CORE_AFFINITY
        #घोषणा configTASK_DEFAULT_CORE_AFFINITY    tskNO_AFFINITY
    #पूर्ण_अगर
#पूर्ण_अगर

#अगर_अघोषित configUSE_PASSIVE_IDLE_HOOK
    #घोषणा configUSE_PASSIVE_IDLE_HOOK    0
#पूर्ण_अगर /* configUSE_PASSIVE_IDLE_HOOK */

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

    #अगर_अघोषित portTIMER_CALLBACK_ATTRIBUTE
        #घोषणा portTIMER_CALLBACK_ATTRIBUTE
    #पूर्ण_अगर /* portTIMER_CALLBACK_ATTRIBUTE */

#पूर्ण_अगर /* configUSE_TIMERS */

#अगर_अघोषित portHAS_NESTED_INTERRUPTS
    #अगर defined( portSET_INTERRUPT_MASK_FROM_ISR ) && defined( portCLEAR_INTERRUPT_MASK_FROM_ISR )
        #घोषणा portHAS_NESTED_INTERRUPTS    1
    #अन्यथा
        #घोषणा portHAS_NESTED_INTERRUPTS    0
    #पूर्ण_अगर
#पूर्ण_अगर

#अगर_अघोषित portSET_INTERRUPT_MASK_FROM_ISR
    #अगर ( portHAS_NESTED_INTERRUPTS == 1 )
        #त्रुटि portSET_INTERRUPT_MASK_FROM_ISR must be defined क्रम ports that support nested interrupts (i.e. portHAS_NESTED_INTERRUPTS is set to 1)
    #अन्यथा
        #घोषणा portSET_INTERRUPT_MASK_FROM_ISR()    0
    #पूर्ण_अगर
#अन्यथा
    #अगर ( portHAS_NESTED_INTERRUPTS == 0 )
        #त्रुटि portSET_INTERRUPT_MASK_FROM_ISR must not be defined क्रम ports that करो not support nested interrupts (i.e. portHAS_NESTED_INTERRUPTS is set to 0)
    #पूर्ण_अगर
#पूर्ण_अगर

#अगर_अघोषित portCLEAR_INTERRUPT_MASK_FROM_ISR
    #अगर ( portHAS_NESTED_INTERRUPTS == 1 )
        #त्रुटि portCLEAR_INTERRUPT_MASK_FROM_ISR must be defined क्रम ports that support nested interrupts  (i.e. portHAS_NESTED_INTERRUPTS is set to 1)
    #अन्यथा
        #घोषणा portCLEAR_INTERRUPT_MASK_FROM_ISR( ux_Saved_Status_मूल्य )    ( व्योम ) ( ux_Saved_Status_मूल्य )
    #पूर्ण_अगर
#अन्यथा
    #अगर ( portHAS_NESTED_INTERRUPTS == 0 )
        #त्रुटि portCLEAR_INTERRUPT_MASK_FROM_ISR must not be defined क्रम ports that करो not support nested interrupts (i.e. portHAS_NESTED_INTERRUPTS is set to 0)
    #पूर्ण_अगर
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

#अगर_अघोषित portTASK_SWITCH_HOOK
    #घोषणा portTASK_SWITCH_HOOK( pxTCB )    ( व्योम ) ( pxTCB )
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

#अगर_अघोषित traceMOVED_TASK_TO_DELAYED_LIST
    #घोषणा traceMOVED_TASK_TO_DELAYED_LIST()
#पूर्ण_अगर

#अगर_अघोषित traceMOVED_TASK_TO_OVERFLOW_DELAYED_LIST
    #घोषणा traceMOVED_TASK_TO_OVERFLOW_DELAYED_LIST()
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

#अगर_अघोषित traceISR_EXIT_TO_SCHEDULER
    #घोषणा traceISR_EXIT_TO_SCHEDULER()
#पूर्ण_अगर

#अगर_अघोषित traceISR_EXIT
    #घोषणा traceISR_EXIT()
#पूर्ण_अगर

#अगर_अघोषित traceISR_ENTER
    #घोषणा traceISR_ENTER()
#पूर्ण_अगर

#अगर_अघोषित trace_STREAM_बफर_CREATE_FAILED
    #घोषणा trace_STREAM_बफर_CREATE_FAILED( x_Stream_बफर_Type )
#पूर्ण_अगर

#अगर_अघोषित trace_STREAM_बफर_CREATE_STATIC_FAILED
    #घोषणा trace_STREAM_बफर_CREATE_STATIC_FAILED( xReturn, x_Stream_बफर_Type )
#पूर्ण_अगर

#अगर_अघोषित trace_STREAM_बफर_CREATE
    #घोषणा trace_STREAM_बफर_CREATE( px_Stream_बफर, x_Stream_बफर_Type )
#पूर्ण_अगर

#अगर_अघोषित trace_STREAM_बफर_DELETE
    #घोषणा trace_STREAM_बफर_DELETE( x_Stream_बफर )
#पूर्ण_अगर

#अगर_अघोषित trace_STREAM_बफर_RESET
    #घोषणा trace_STREAM_बफर_RESET( x_Stream_बफर )
#पूर्ण_अगर

#अगर_अघोषित trace_STREAM_बफर_RESET_FROM_ISR
    #घोषणा trace_STREAM_बफर_RESET_FROM_ISR( x_Stream_बफर )
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

#अगर_अघोषित traceENTER_xEventGroupCreateStatic
    #घोषणा traceENTER_xEventGroupCreateStatic( px_Event_Group_बफर )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xEventGroupCreateStatic
    #घोषणा traceRETURN_xEventGroupCreateStatic( pxEventBits )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xEventGroupCreate
    #घोषणा traceENTER_xEventGroupCreate()
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xEventGroupCreate
    #घोषणा traceRETURN_xEventGroupCreate( pxEventBits )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xEventGroupSync
    #घोषणा traceENTER_xEventGroupSync( xEventGroup, uxBitsToSet, uxBitsToWaitFor, xTicksToWait )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xEventGroupSync
    #घोषणा traceRETURN_xEventGroupSync( uxReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xEventGroupWaitBits
    #घोषणा traceENTER_xEventGroupWaitBits( xEventGroup, uxBitsToWaitFor, xClearOnExit, xWaitForAllBits, xTicksToWait )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xEventGroupWaitBits
    #घोषणा traceRETURN_xEventGroupWaitBits( uxReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xEventGroupClearBits
    #घोषणा traceENTER_xEventGroupClearBits( xEventGroup, uxBitsToClear )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xEventGroupClearBits
    #घोषणा traceRETURN_xEventGroupClearBits( uxReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xEventGroupClearBitsFromISR
    #घोषणा traceENTER_xEventGroupClearBitsFromISR( xEventGroup, uxBitsToClear )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xEventGroupClearBitsFromISR
    #घोषणा traceRETURN_xEventGroupClearBitsFromISR( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xEventGroupGetBitsFromISR
    #घोषणा traceENTER_xEventGroupGetBitsFromISR( xEventGroup )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xEventGroupGetBitsFromISR
    #घोषणा traceRETURN_xEventGroupGetBitsFromISR( uxReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xEventGroupSetBits
    #घोषणा traceENTER_xEventGroupSetBits( xEventGroup, uxBitsToSet )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xEventGroupSetBits
    #घोषणा traceRETURN_xEventGroupSetBits( uxEventBits )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vEventGroupDelete
    #घोषणा traceENTER_vEventGroupDelete( xEventGroup )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vEventGroupDelete
    #घोषणा traceRETURN_vEventGroupDelete()
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_x_Event_Group_Get_Static_बफर
    #घोषणा trace_ENTER_x_Event_Group_Get_Static_बफर( xEventGroup, ppx_Event_Group_बफर )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_x_Event_Group_Get_Static_बफर
    #घोषणा trace_RETURN_x_Event_Group_Get_Static_बफर( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vEventGroupSetBitsCallback
    #घोषणा traceENTER_vEventGroupSetBitsCallback( pvEventGroup, ulBitsToSet )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vEventGroupSetBitsCallback
    #घोषणा traceRETURN_vEventGroupSetBitsCallback()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vEventGroupClearBitsCallback
    #घोषणा traceENTER_vEventGroupClearBitsCallback( pvEventGroup, ulBitsToClear )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vEventGroupClearBitsCallback
    #घोषणा traceRETURN_vEventGroupClearBitsCallback()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xEventGroupSetBitsFromISR
    #घोषणा traceENTER_xEventGroupSetBitsFromISR( xEventGroup, uxBitsToSet, pxHigherPriorityTaskWoken )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xEventGroupSetBitsFromISR
    #घोषणा traceRETURN_xEventGroupSetBitsFromISR( xReturn )
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_ux_Event_Group_Get_संख्या
    #घोषणा trace_ENTER_ux_Event_Group_Get_संख्या( xEventGroup )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_ux_Event_Group_Get_संख्या
    #घोषणा trace_RETURN_ux_Event_Group_Get_संख्या( xReturn )
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_v_Event_Group_Set_संख्या
    #घोषणा trace_ENTER_v_Event_Group_Set_संख्या( xEventGroup, ux_Event_Group_संख्या )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_v_Event_Group_Set_संख्या
    #घोषणा trace_RETURN_v_Event_Group_Set_संख्या()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xQueueGenericReset
    #घोषणा traceENTER_xQueueGenericReset( xQueue, xNewQueue )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xQueueGenericReset
    #घोषणा traceRETURN_xQueueGenericReset( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xQueueGenericCreateStatic
    #घोषणा traceENTER_xQueueGenericCreateStatic( ux_Queue_लंबाई, ux_Item_आकार, pucQueueStorage, pxStaticQueue, ucQueueType )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xQueueGenericCreateStatic
    #घोषणा traceRETURN_xQueueGenericCreateStatic( pxNewQueue )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xQueueGenericGetStaticBuffers
    #घोषणा traceENTER_xQueueGenericGetStaticBuffers( xQueue, ppucQueueStorage, ppxStaticQueue )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xQueueGenericGetStaticBuffers
    #घोषणा traceRETURN_xQueueGenericGetStaticBuffers( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xQueueGenericCreate
    #घोषणा traceENTER_xQueueGenericCreate( ux_Queue_लंबाई, ux_Item_आकार, ucQueueType )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xQueueGenericCreate
    #घोषणा traceRETURN_xQueueGenericCreate( pxNewQueue )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xQueueCreateMutex
    #घोषणा traceENTER_xQueueCreateMutex( ucQueueType )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xQueueCreateMutex
    #घोषणा traceRETURN_xQueueCreateMutex( xNewQueue )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xQueueCreateMutexStatic
    #घोषणा traceENTER_xQueueCreateMutexStatic( ucQueueType, pxStaticQueue )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xQueueCreateMutexStatic
    #घोषणा traceRETURN_xQueueCreateMutexStatic( xNewQueue )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xQueueGetMutexHolder
    #घोषणा traceENTER_xQueueGetMutexHolder( xSemaphore )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xQueueGetMutexHolder
    #घोषणा traceRETURN_xQueueGetMutexHolder( pxReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xQueueGetMutexHolderFromISR
    #घोषणा traceENTER_xQueueGetMutexHolderFromISR( xSemaphore )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xQueueGetMutexHolderFromISR
    #घोषणा traceRETURN_xQueueGetMutexHolderFromISR( pxReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xQueueGiveMutexRecursive
    #घोषणा traceENTER_xQueueGiveMutexRecursive( xMutex )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xQueueGiveMutexRecursive
    #घोषणा traceRETURN_xQueueGiveMutexRecursive( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xQueueTakeMutexRecursive
    #घोषणा traceENTER_xQueueTakeMutexRecursive( xMutex, xTicksToWait )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xQueueTakeMutexRecursive
    #घोषणा traceRETURN_xQueueTakeMutexRecursive( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xQueueCreateCountingSemaphoreStatic
    #घोषणा traceENTER_xQueueCreateCountingSemaphoreStatic( ux_Max_गिनती, ux_Initial_गिनती, pxStaticQueue )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xQueueCreateCountingSemaphoreStatic
    #घोषणा traceRETURN_xQueueCreateCountingSemaphoreStatic( xHandle )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xQueueCreateCountingSemaphore
    #घोषणा traceENTER_xQueueCreateCountingSemaphore( ux_Max_गिनती, ux_Initial_गिनती )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xQueueCreateCountingSemaphore
    #घोषणा traceRETURN_xQueueCreateCountingSemaphore( xHandle )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xQueueGenericSend
    #घोषणा traceENTER_xQueueGenericSend( xQueue, pvItemToQueue, xTicksToWait, xCopyPosition )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xQueueGenericSend
    #घोषणा traceRETURN_xQueueGenericSend( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xQueueGenericSendFromISR
    #घोषणा traceENTER_xQueueGenericSendFromISR( xQueue, pvItemToQueue, pxHigherPriorityTaskWoken, xCopyPosition )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xQueueGenericSendFromISR
    #घोषणा traceRETURN_xQueueGenericSendFromISR( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xQueueGiveFromISR
    #घोषणा traceENTER_xQueueGiveFromISR( xQueue, pxHigherPriorityTaskWoken )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xQueueGiveFromISR
    #घोषणा traceRETURN_xQueueGiveFromISR( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xQueueReceive
    #घोषणा traceENTER_xQueueReceive( xQueue, pv_बफर, xTicksToWait )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xQueueReceive
    #घोषणा traceRETURN_xQueueReceive( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xQueueSemaphoreTake
    #घोषणा traceENTER_xQueueSemaphoreTake( xQueue, xTicksToWait )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xQueueSemaphoreTake
    #घोषणा traceRETURN_xQueueSemaphoreTake( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xQueuePeek
    #घोषणा traceENTER_xQueuePeek( xQueue, pv_बफर, xTicksToWait )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xQueuePeek
    #घोषणा traceRETURN_xQueuePeek( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xQueueReceiveFromISR
    #घोषणा traceENTER_xQueueReceiveFromISR( xQueue, pv_बफर, pxHigherPriorityTaskWoken )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xQueueReceiveFromISR
    #घोषणा traceRETURN_xQueueReceiveFromISR( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xQueuePeekFromISR
    #घोषणा traceENTER_xQueuePeekFromISR( xQueue, pv_बफर )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xQueuePeekFromISR
    #घोषणा traceRETURN_xQueuePeekFromISR( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_uxQueueMessagesWaiting
    #घोषणा traceENTER_uxQueueMessagesWaiting( xQueue )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_uxQueueMessagesWaiting
    #घोषणा traceRETURN_uxQueueMessagesWaiting( uxReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_uxQueueSpacesAvailable
    #घोषणा traceENTER_uxQueueSpacesAvailable( xQueue )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_uxQueueSpacesAvailable
    #घोषणा traceRETURN_uxQueueSpacesAvailable( uxReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_uxQueueMessagesWaitingFromISR
    #घोषणा traceENTER_uxQueueMessagesWaitingFromISR( xQueue )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_uxQueueMessagesWaitingFromISR
    #घोषणा traceRETURN_uxQueueMessagesWaitingFromISR( uxReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vQueueDelete
    #घोषणा traceENTER_vQueueDelete( xQueue )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vQueueDelete
    #घोषणा traceRETURN_vQueueDelete()
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_ux_Queue_Get_Queue_संख्या
    #घोषणा trace_ENTER_ux_Queue_Get_Queue_संख्या( xQueue )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_ux_Queue_Get_Queue_संख्या
    #घोषणा trace_RETURN_ux_Queue_Get_Queue_संख्या( ux_Queue_संख्या )
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_v_Queue_Set_Queue_संख्या
    #घोषणा trace_ENTER_v_Queue_Set_Queue_संख्या( xQueue, ux_Queue_संख्या )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_v_Queue_Set_Queue_संख्या
    #घोषणा trace_RETURN_v_Queue_Set_Queue_संख्या()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_ucQueueGetQueueType
    #घोषणा traceENTER_ucQueueGetQueueType( xQueue )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_ucQueueGetQueueType
    #घोषणा traceRETURN_ucQueueGetQueueType( ucQueueType )
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_ux_Queue_Get_Queue_Item_आकार
    #घोषणा trace_ENTER_ux_Queue_Get_Queue_Item_आकार( xQueue )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_ux_Queue_Get_Queue_Item_आकार
    #घोषणा trace_RETURN_ux_Queue_Get_Queue_Item_आकार( ux_Item_आकार )
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_ux_Queue_Get_Queue_लंबाई
    #घोषणा trace_ENTER_ux_Queue_Get_Queue_लंबाई( xQueue )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_ux_Queue_Get_Queue_लंबाई
    #घोषणा trace_RETURN_ux_Queue_Get_Queue_लंबाई( ux_लंबाई )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xQueueIsQueueEmptyFromISR
    #घोषणा traceENTER_xQueueIsQueueEmptyFromISR( xQueue )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xQueueIsQueueEmptyFromISR
    #घोषणा traceRETURN_xQueueIsQueueEmptyFromISR( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xQueueIsQueueFullFromISR
    #घोषणा traceENTER_xQueueIsQueueFullFromISR( xQueue )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xQueueIsQueueFullFromISR
    #घोषणा traceRETURN_xQueueIsQueueFullFromISR( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xQueueCRSend
    #घोषणा traceENTER_xQueueCRSend( xQueue, pvItemToQueue, xTicksToWait )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xQueueCRSend
    #घोषणा traceRETURN_xQueueCRSend( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xQueueCRReceive
    #घोषणा traceENTER_xQueueCRReceive( xQueue, pv_बफर, xTicksToWait )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xQueueCRReceive
    #घोषणा traceRETURN_xQueueCRReceive( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xQueueCRSendFromISR
    #घोषणा traceENTER_xQueueCRSendFromISR( xQueue, pvItemToQueue, xCoRoutinePreviouslyWoken )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xQueueCRSendFromISR
    #घोषणा traceRETURN_xQueueCRSendFromISR( xCoRoutinePreviouslyWoken )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xQueueCRReceiveFromISR
    #घोषणा traceENTER_xQueueCRReceiveFromISR( xQueue, pv_बफर, pxCoRoutineWoken )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xQueueCRReceiveFromISR
    #घोषणा traceRETURN_xQueueCRReceiveFromISR( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vQueueAddToRegistry
    #घोषणा traceENTER_vQueueAddToRegistry( xQueue, pcQueueName )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vQueueAddToRegistry
    #घोषणा traceRETURN_vQueueAddToRegistry()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_pcQueueGetName
    #घोषणा traceENTER_pcQueueGetName( xQueue )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_pcQueueGetName
    #घोषणा traceRETURN_pcQueueGetName( pcReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vQueueUnregisterQueue
    #घोषणा traceENTER_vQueueUnregisterQueue( xQueue )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vQueueUnregisterQueue
    #घोषणा traceRETURN_vQueueUnregisterQueue()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vQueueWaitForMessageRestricted
    #घोषणा traceENTER_vQueueWaitForMessageRestricted( xQueue, xTicksToWait, xWaitIndefinitely )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vQueueWaitForMessageRestricted
    #घोषणा traceRETURN_vQueueWaitForMessageRestricted()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xQueueCreateSet
    #घोषणा traceENTER_xQueueCreateSet( ux_Event_Queue_लंबाई )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xQueueCreateSet
    #घोषणा traceRETURN_xQueueCreateSet( pxQueue )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xQueueAddToSet
    #घोषणा traceENTER_xQueueAddToSet( xQueueOrSemaphore, xQueueSet )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xQueueAddToSet
    #घोषणा traceRETURN_xQueueAddToSet( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xQueueRemoveFromSet
    #घोषणा traceENTER_xQueueRemoveFromSet( xQueueOrSemaphore, xQueueSet )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xQueueRemoveFromSet
    #घोषणा traceRETURN_xQueueRemoveFromSet( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xQueueSelectFromSet
    #घोषणा traceENTER_xQueueSelectFromSet( xQueueSet, xTicksToWait )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xQueueSelectFromSet
    #घोषणा traceRETURN_xQueueSelectFromSet( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xQueueSelectFromSetFromISR
    #घोषणा traceENTER_xQueueSelectFromSetFromISR( xQueueSet )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xQueueSelectFromSetFromISR
    #घोषणा traceRETURN_xQueueSelectFromSetFromISR( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTimerCreateTimerTask
    #घोषणा traceENTER_xTimerCreateTimerTask()
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTimerCreateTimerTask
    #घोषणा traceRETURN_xTimerCreateTimerTask( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTimerCreate
    #घोषणा traceENTER_xTimerCreate( pcTimerName, xTimerPeriodInTicks, xAutoReload, pvTimerID, pxCallbackFunction )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTimerCreate
    #घोषणा traceRETURN_xTimerCreate( pxNewTimer )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTimerCreateStatic
    #घोषणा traceENTER_xTimerCreateStatic( pcTimerName, xTimerPeriodInTicks, xAutoReload, pvTimerID, pxCallbackFunction, px_Timer_बफर )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTimerCreateStatic
    #घोषणा traceRETURN_xTimerCreateStatic( pxNewTimer )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTimerGenericCommandFromTask
    #घोषणा traceENTER_xTimerGenericCommandFromTask( xTimer, xCommandID, x_Optional_मूल्य, pxHigherPriorityTaskWoken, xTicksToWait )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTimerGenericCommandFromTask
    #घोषणा traceRETURN_xTimerGenericCommandFromTask( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTimerGenericCommandFromISR
    #घोषणा traceENTER_xTimerGenericCommandFromISR( xTimer, xCommandID, x_Optional_मूल्य, pxHigherPriorityTaskWoken, xTicksToWait )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTimerGenericCommandFromISR
    #घोषणा traceRETURN_xTimerGenericCommandFromISR( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTimerGetTimerDaemonTaskHandle
    #घोषणा traceENTER_xTimerGetTimerDaemonTaskHandle()
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTimerGetTimerDaemonTaskHandle
    #घोषणा traceRETURN_xTimerGetTimerDaemonTaskHandle( xTimerTaskHandle )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTimerGetPeriod
    #घोषणा traceENTER_xTimerGetPeriod( xTimer )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTimerGetPeriod
    #घोषणा traceRETURN_xTimerGetPeriod( xTimerPeriodInTicks )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vTimerSetReloadMode
    #घोषणा traceENTER_vTimerSetReloadMode( xTimer, xAutoReload )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vTimerSetReloadMode
    #घोषणा traceRETURN_vTimerSetReloadMode()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTimerGetReloadMode
    #घोषणा traceENTER_xTimerGetReloadMode( xTimer )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTimerGetReloadMode
    #घोषणा traceRETURN_xTimerGetReloadMode( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_uxTimerGetReloadMode
    #घोषणा traceENTER_uxTimerGetReloadMode( xTimer )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_uxTimerGetReloadMode
    #घोषणा traceRETURN_uxTimerGetReloadMode( uxReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTimerGetExpiryTime
    #घोषणा traceENTER_xTimerGetExpiryTime( xTimer )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTimerGetExpiryTime
    #घोषणा traceRETURN_xTimerGetExpiryTime( xReturn )
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_x_Timer_Get_Static_बफर
    #घोषणा trace_ENTER_x_Timer_Get_Static_बफर( xTimer, ppx_Timer_बफर )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_x_Timer_Get_Static_बफर
    #घोषणा trace_RETURN_x_Timer_Get_Static_बफर( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_pcTimerGetName
    #घोषणा traceENTER_pcTimerGetName( xTimer )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_pcTimerGetName
    #घोषणा traceRETURN_pcTimerGetName( pcTimerName )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTimerIsTimerActive
    #घोषणा traceENTER_xTimerIsTimerActive( xTimer )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTimerIsTimerActive
    #घोषणा traceRETURN_xTimerIsTimerActive( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_pvTimerGetTimerID
    #घोषणा traceENTER_pvTimerGetTimerID( xTimer )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_pvTimerGetTimerID
    #घोषणा traceRETURN_pvTimerGetTimerID( pvReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vTimerSetTimerID
    #घोषणा traceENTER_vTimerSetTimerID( xTimer, pvNewID )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vTimerSetTimerID
    #घोषणा traceRETURN_vTimerSetTimerID()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTimerPendFunctionCallFromISR
    #घोषणा traceENTER_xTimerPendFunctionCallFromISR( xFunctionToPend, pvParameter1, ulParameter2, pxHigherPriorityTaskWoken )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTimerPendFunctionCallFromISR
    #घोषणा traceRETURN_xTimerPendFunctionCallFromISR( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTimerPendFunctionCall
    #घोषणा traceENTER_xTimerPendFunctionCall( xFunctionToPend, pvParameter1, ulParameter2, xTicksToWait )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTimerPendFunctionCall
    #घोषणा traceRETURN_xTimerPendFunctionCall( xReturn )
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_ux_Timer_Get_Timer_संख्या
    #घोषणा trace_ENTER_ux_Timer_Get_Timer_संख्या( xTimer )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_ux_Timer_Get_Timer_संख्या
    #घोषणा trace_RETURN_ux_Timer_Get_Timer_संख्या( ux_Timer_संख्या )
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_v_Timer_Set_Timer_संख्या
    #घोषणा trace_ENTER_v_Timer_Set_Timer_संख्या( xTimer, ux_Timer_संख्या )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_v_Timer_Set_Timer_संख्या
    #घोषणा trace_RETURN_v_Timer_Set_Timer_संख्या()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTaskCreateStatic
    #घोषणा traceENTER_xTaskCreateStatic( pxTaskCode, pcName, ux_ढेर_Depth, pvParameters, uxPriority, pux_ढेर_बफर, px_Task_बफर )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTaskCreateStatic
    #घोषणा traceRETURN_xTaskCreateStatic( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTaskCreateStaticAffinitySet
    #घोषणा traceENTER_xTaskCreateStaticAffinitySet( pxTaskCode, pcName, ux_ढेर_Depth, pvParameters, uxPriority, pux_ढेर_बफर, px_Task_बफर, uxCoreAffinityMask )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTaskCreateStaticAffinitySet
    #घोषणा traceRETURN_xTaskCreateStaticAffinitySet( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTaskCreateRestrictedStatic
    #घोषणा traceENTER_xTaskCreateRestrictedStatic( pxTaskDefinition, pxCreatedTask )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTaskCreateRestrictedStatic
    #घोषणा traceRETURN_xTaskCreateRestrictedStatic( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTaskCreateRestrictedStaticAffinitySet
    #घोषणा traceENTER_xTaskCreateRestrictedStaticAffinitySet( pxTaskDefinition, uxCoreAffinityMask, pxCreatedTask )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTaskCreateRestrictedStaticAffinitySet
    #घोषणा traceRETURN_xTaskCreateRestrictedStaticAffinitySet( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTaskCreateRestricted
    #घोषणा traceENTER_xTaskCreateRestricted( pxTaskDefinition, pxCreatedTask )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTaskCreateRestricted
    #घोषणा traceRETURN_xTaskCreateRestricted( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTaskCreateRestrictedAffinitySet
    #घोषणा traceENTER_xTaskCreateRestrictedAffinitySet( pxTaskDefinition, uxCoreAffinityMask, pxCreatedTask )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTaskCreateRestrictedAffinitySet
    #घोषणा traceRETURN_xTaskCreateRestrictedAffinitySet( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTaskCreate
    #घोषणा traceENTER_xTaskCreate( pxTaskCode, pcName, ux_ढेर_Depth, pvParameters, uxPriority, pxCreatedTask )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTaskCreate
    #घोषणा traceRETURN_xTaskCreate( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTaskCreateAffinitySet
    #घोषणा traceENTER_xTaskCreateAffinitySet( pxTaskCode, pcName, ux_ढेर_Depth, pvParameters, uxPriority, uxCoreAffinityMask, pxCreatedTask )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTaskCreateAffinitySet
    #घोषणा traceRETURN_xTaskCreateAffinitySet( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vTaskDelete
    #घोषणा traceENTER_vTaskDelete( xTaskToDelete )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vTaskDelete
    #घोषणा traceRETURN_vTaskDelete()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTaskDelayUntil
    #घोषणा traceENTER_xTaskDelayUntil( pxPreviousWakeTime, xTimeIncrement )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTaskDelayUntil
    #घोषणा traceRETURN_xTaskDelayUntil( xShouldDelay )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vTaskDelay
    #घोषणा traceENTER_vTaskDelay( xTicksToDelay )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vTaskDelay
    #घोषणा traceRETURN_vTaskDelay()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_eTaskGetState
    #घोषणा traceENTER_eTaskGetState( xTask )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_eTaskGetState
    #घोषणा traceRETURN_eTaskGetState( eReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_uxTaskPriorityGet
    #घोषणा traceENTER_uxTaskPriorityGet( xTask )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_uxTaskPriorityGet
    #घोषणा traceRETURN_uxTaskPriorityGet( uxReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_uxTaskPriorityGetFromISR
    #घोषणा traceENTER_uxTaskPriorityGetFromISR( xTask )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_uxTaskPriorityGetFromISR
    #घोषणा traceRETURN_uxTaskPriorityGetFromISR( uxReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_uxTaskBasePriorityGet
    #घोषणा traceENTER_uxTaskBasePriorityGet( xTask )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_uxTaskBasePriorityGet
    #घोषणा traceRETURN_uxTaskBasePriorityGet( uxReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_uxTaskBasePriorityGetFromISR
    #घोषणा traceENTER_uxTaskBasePriorityGetFromISR( xTask )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_uxTaskBasePriorityGetFromISR
    #घोषणा traceRETURN_uxTaskBasePriorityGetFromISR( uxReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vTaskPrioritySet
    #घोषणा traceENTER_vTaskPrioritySet( xTask, uxNewPriority )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vTaskPrioritySet
    #घोषणा traceRETURN_vTaskPrioritySet()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vTaskCoreAffinitySet
    #घोषणा traceENTER_vTaskCoreAffinitySet( xTask, uxCoreAffinityMask )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vTaskCoreAffinitySet
    #घोषणा traceRETURN_vTaskCoreAffinitySet()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vTaskCoreAffinityGet
    #घोषणा traceENTER_vTaskCoreAffinityGet( xTask )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vTaskCoreAffinityGet
    #घोषणा traceRETURN_vTaskCoreAffinityGet( uxCoreAffinityMask )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vTaskPreemptionDisable
    #घोषणा traceENTER_vTaskPreemptionDisable( xTask )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vTaskPreemptionDisable
    #घोषणा traceRETURN_vTaskPreemptionDisable()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vTaskPreemptionEnable
    #घोषणा traceENTER_vTaskPreemptionEnable( xTask )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vTaskPreemptionEnable
    #घोषणा traceRETURN_vTaskPreemptionEnable()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vTaskSuspend
    #घोषणा traceENTER_vTaskSuspend( xTaskToSuspend )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vTaskSuspend
    #घोषणा traceRETURN_vTaskSuspend()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vTaskResume
    #घोषणा traceENTER_vTaskResume( xTaskToResume )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vTaskResume
    #घोषणा traceRETURN_vTaskResume()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTaskResumeFromISR
    #घोषणा traceENTER_xTaskResumeFromISR( xTaskToResume )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTaskResumeFromISR
    #घोषणा traceRETURN_xTaskResumeFromISR( xYieldRequired )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vTaskStartScheduler
    #घोषणा traceENTER_vTaskStartScheduler()
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vTaskStartScheduler
    #घोषणा traceRETURN_vTaskStartScheduler()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vTaskEndScheduler
    #घोषणा traceENTER_vTaskEndScheduler()
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vTaskEndScheduler
    #घोषणा traceRETURN_vTaskEndScheduler()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vTaskSuspendAll
    #घोषणा traceENTER_vTaskSuspendAll()
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vTaskSuspendAll
    #घोषणा traceRETURN_vTaskSuspendAll()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTaskResumeAll
    #घोषणा traceENTER_xTaskResumeAll()
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTaskResumeAll
    #घोषणा traceRETURN_xTaskResumeAll( xAlreadyYielded )
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_x_Task_Get_Tick_गिनती
    #घोषणा trace_ENTER_x_Task_Get_Tick_गिनती()
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_x_Task_Get_Tick_गिनती
    #घोषणा trace_RETURN_x_Task_Get_Tick_गिनती( xTicks )
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_x_Task_Get_Tick_गिनती_From_ISR
    #घोषणा trace_ENTER_x_Task_Get_Tick_गिनती_From_ISR()
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_x_Task_Get_Tick_गिनती_From_ISR
    #घोषणा trace_RETURN_x_Task_Get_Tick_गिनती_From_ISR( xReturn )
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_ux_Task_Get_संख्या_Of_Tasks
    #घोषणा trace_ENTER_ux_Task_Get_संख्या_Of_Tasks()
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_ux_Task_Get_संख्या_Of_Tasks
    #घोषणा trace_RETURN_ux_Task_Get_संख्या_Of_Tasks( ux_Current_संख्या_Of_Tasks )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_pcTaskGetName
    #घोषणा traceENTER_pcTaskGetName( xTaskToQuery )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_pcTaskGetName
    #घोषणा traceRETURN_pcTaskGetName( pcTaskName )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTaskGetHandle
    #घोषणा traceENTER_xTaskGetHandle( pcNameToQuery )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTaskGetHandle
    #घोषणा traceRETURN_xTaskGetHandle( pxTCB )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTaskGetStaticBuffers
    #घोषणा traceENTER_xTaskGetStaticBuffers( xTask, ppux_ढेर_बफर, ppx_Task_बफर )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTaskGetStaticBuffers
    #घोषणा traceRETURN_xTaskGetStaticBuffers( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_uxTaskGetSystemState
    #घोषणा traceENTER_uxTaskGetSystemState( pxTaskStatusArray, ux_Array_आकार, pul_योग_Run_Time )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_uxTaskGetSystemState
    #घोषणा traceRETURN_uxTaskGetSystemState( uxTask )
#पूर्ण_अगर

#अगर ( config_संख्या_OF_CORES == 1 )
    #अगर_अघोषित traceENTER_xTaskGetIdleTaskHandle
        #घोषणा traceENTER_xTaskGetIdleTaskHandle()
    #पूर्ण_अगर
#पूर्ण_अगर

#अगर ( config_संख्या_OF_CORES == 1 )
    #अगर_अघोषित traceRETURN_xTaskGetIdleTaskHandle
        #घोषणा traceRETURN_xTaskGetIdleTaskHandle( xIdleTaskHandle )
    #पूर्ण_अगर
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTaskGetIdleTaskHandleForCore
    #घोषणा traceENTER_xTaskGetIdleTaskHandleForCore( xCoreID )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTaskGetIdleTaskHandleForCore
    #घोषणा traceRETURN_xTaskGetIdleTaskHandleForCore( xIdleTaskHandle )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vTaskStepTick
    #घोषणा traceENTER_vTaskStepTick( xTicksToJump )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vTaskStepTick
    #घोषणा traceRETURN_vTaskStepTick()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTaskCatchUpTicks
    #घोषणा traceENTER_xTaskCatchUpTicks( xTicksToCatchUp )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTaskCatchUpTicks
    #घोषणा traceRETURN_xTaskCatchUpTicks( xYieldOccurred )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTaskAbortDelay
    #घोषणा traceENTER_xTaskAbortDelay( xTask )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTaskAbortDelay
    #घोषणा traceRETURN_xTaskAbortDelay( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTaskIncrementTick
    #घोषणा traceENTER_xTaskIncrementTick()
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTaskIncrementTick
    #घोषणा traceRETURN_xTaskIncrementTick( xSwitchRequired )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vTaskSetApplicationTaskTag
    #घोषणा traceENTER_vTaskSetApplicationTaskTag( xTask, pxHookFunction )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vTaskSetApplicationTaskTag
    #घोषणा traceRETURN_vTaskSetApplicationTaskTag()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTaskGetApplicationTaskTag
    #घोषणा traceENTER_xTaskGetApplicationTaskTag( xTask )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTaskGetApplicationTaskTag
    #घोषणा traceRETURN_xTaskGetApplicationTaskTag( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTaskGetApplicationTaskTagFromISR
    #घोषणा traceENTER_xTaskGetApplicationTaskTagFromISR( xTask )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTaskGetApplicationTaskTagFromISR
    #घोषणा traceRETURN_xTaskGetApplicationTaskTagFromISR( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTaskCallApplicationTaskHook
    #घोषणा traceENTER_xTaskCallApplicationTaskHook( xTask, pvParameter )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTaskCallApplicationTaskHook
    #घोषणा traceRETURN_xTaskCallApplicationTaskHook( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vTaskSwitchContext
    #घोषणा traceENTER_vTaskSwitchContext()
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vTaskSwitchContext
    #घोषणा traceRETURN_vTaskSwitchContext()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vTaskPlaceOnEventList
    #घोषणा traceENTER_vTaskPlaceOnEventList( pxEventList, xTicksToWait )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vTaskPlaceOnEventList
    #घोषणा traceRETURN_vTaskPlaceOnEventList()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vTaskPlaceOnUnorderedEventList
    #घोषणा traceENTER_vTaskPlaceOnUnorderedEventList( pxEventList, x_Item_मूल्य, xTicksToWait )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vTaskPlaceOnUnorderedEventList
    #घोषणा traceRETURN_vTaskPlaceOnUnorderedEventList()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vTaskPlaceOnEventListRestricted
    #घोषणा traceENTER_vTaskPlaceOnEventListRestricted( pxEventList, xTicksToWait, xWaitIndefinitely )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vTaskPlaceOnEventListRestricted
    #घोषणा traceRETURN_vTaskPlaceOnEventListRestricted()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTaskRemoveFromEventList
    #घोषणा traceENTER_xTaskRemoveFromEventList( pxEventList )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTaskRemoveFromEventList
    #घोषणा traceRETURN_xTaskRemoveFromEventList( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vTaskRemoveFromUnorderedEventList
    #घोषणा traceENTER_vTaskRemoveFromUnorderedEventList( pxEventListItem, x_Item_मूल्य )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vTaskRemoveFromUnorderedEventList
    #घोषणा traceRETURN_vTaskRemoveFromUnorderedEventList()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vTaskSetTimeOutState
    #घोषणा traceENTER_vTaskSetTimeOutState( pxTimeOut )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vTaskSetTimeOutState
    #घोषणा traceRETURN_vTaskSetTimeOutState()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vTaskInternalSetTimeOutState
    #घोषणा traceENTER_vTaskInternalSetTimeOutState( pxTimeOut )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vTaskInternalSetTimeOutState
    #घोषणा traceRETURN_vTaskInternalSetTimeOutState()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTaskCheckForTimeOut
    #घोषणा traceENTER_xTaskCheckForTimeOut( pxTimeOut, pxTicksToWait )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTaskCheckForTimeOut
    #घोषणा traceRETURN_xTaskCheckForTimeOut( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vTaskMissedYield
    #घोषणा traceENTER_vTaskMissedYield()
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vTaskMissedYield
    #घोषणा traceRETURN_vTaskMissedYield()
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_ux_Task_Get_Task_संख्या
    #घोषणा trace_ENTER_ux_Task_Get_Task_संख्या( xTask )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_ux_Task_Get_Task_संख्या
    #घोषणा trace_RETURN_ux_Task_Get_Task_संख्या( uxReturn )
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_v_Task_Set_Task_संख्या
    #घोषणा trace_ENTER_v_Task_Set_Task_संख्या( xTask, uxHandle )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_v_Task_Set_Task_संख्या
    #घोषणा trace_RETURN_v_Task_Set_Task_संख्या()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_eTaskConfirmSleepModeStatus
    #घोषणा traceENTER_eTaskConfirmSleepModeStatus()
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_eTaskConfirmSleepModeStatus
    #घोषणा traceRETURN_eTaskConfirmSleepModeStatus( eReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vTaskSetThreadLocalStoragePointer
    #घोषणा traceENTER_vTaskSetThreadLocalStoragePointer( xTaskToSet, x_सूचकांक, pv_मूल्य )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vTaskSetThreadLocalStoragePointer
    #घोषणा traceRETURN_vTaskSetThreadLocalStoragePointer()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_pvTaskGetThreadLocalStoragePointer
    #घोषणा traceENTER_pvTaskGetThreadLocalStoragePointer( xTaskToQuery, x_सूचकांक )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_pvTaskGetThreadLocalStoragePointer
    #घोषणा traceRETURN_pvTaskGetThreadLocalStoragePointer( pvReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vTaskAllocateMPURegions
    #घोषणा traceENTER_vTaskAllocateMPURegions( xTaskToModify, pxRegions )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vTaskAllocateMPURegions
    #घोषणा traceRETURN_vTaskAllocateMPURegions()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vTaskGetInfo
    #घोषणा traceENTER_vTaskGetInfo( xTask, pxTaskStatus, x_Get_Free_ढेर_Space, eState )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vTaskGetInfo
    #घोषणा traceRETURN_vTaskGetInfo()
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_ux_Task_Get_ढेर_ऊपरी_Water_Mark2
    #घोषणा trace_ENTER_ux_Task_Get_ढेर_ऊपरी_Water_Mark2( xTask )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_ux_Task_Get_ढेर_ऊपरी_Water_Mark2
    #घोषणा trace_RETURN_ux_Task_Get_ढेर_ऊपरी_Water_Mark2( uxReturn )
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_ux_Task_Get_ढेर_ऊपरी_Water_Mark
    #घोषणा trace_ENTER_ux_Task_Get_ढेर_ऊपरी_Water_Mark( xTask )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_ux_Task_Get_ढेर_ऊपरी_Water_Mark
    #घोषणा trace_RETURN_ux_Task_Get_ढेर_ऊपरी_Water_Mark( uxReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTaskGetCurrentTaskHandle
    #घोषणा traceENTER_xTaskGetCurrentTaskHandle()
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTaskGetCurrentTaskHandle
    #घोषणा traceRETURN_xTaskGetCurrentTaskHandle( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTaskGetCurrentTaskHandleForCore
    #घोषणा traceENTER_xTaskGetCurrentTaskHandleForCore( xCoreID )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTaskGetCurrentTaskHandleForCore
    #घोषणा traceRETURN_xTaskGetCurrentTaskHandleForCore( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTaskGetSchedulerState
    #घोषणा traceENTER_xTaskGetSchedulerState()
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTaskGetSchedulerState
    #घोषणा traceRETURN_xTaskGetSchedulerState( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTaskPriorityInherit
    #घोषणा traceENTER_xTaskPriorityInherit( pxMutexHolder )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTaskPriorityInherit
    #घोषणा traceRETURN_xTaskPriorityInherit( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTaskPriorityDisinherit
    #घोषणा traceENTER_xTaskPriorityDisinherit( pxMutexHolder )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTaskPriorityDisinherit
    #घोषणा traceRETURN_xTaskPriorityDisinherit( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vTaskPriorityDisinheritAfterTimeout
    #घोषणा traceENTER_vTaskPriorityDisinheritAfterTimeout( pxMutexHolder, uxHighestPriorityWaitingTask )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vTaskPriorityDisinheritAfterTimeout
    #घोषणा traceRETURN_vTaskPriorityDisinheritAfterTimeout()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vTaskYieldWithinAPI
    #घोषणा traceENTER_vTaskYieldWithinAPI()
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vTaskYieldWithinAPI
    #घोषणा traceRETURN_vTaskYieldWithinAPI()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vTaskEnterCritical
    #घोषणा traceENTER_vTaskEnterCritical()
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vTaskEnterCritical
    #घोषणा traceRETURN_vTaskEnterCritical()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vTaskEnterCriticalFromISR
    #घोषणा traceENTER_vTaskEnterCriticalFromISR()
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vTaskEnterCriticalFromISR
    #घोषणा traceRETURN_vTaskEnterCriticalFromISR( uxSavedInterruptStatus )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vTaskExitCritical
    #घोषणा traceENTER_vTaskExitCritical()
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vTaskExitCritical
    #घोषणा traceRETURN_vTaskExitCritical()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vTaskExitCriticalFromISR
    #घोषणा traceENTER_vTaskExitCriticalFromISR( uxSavedInterruptStatus )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vTaskExitCriticalFromISR
    #घोषणा traceRETURN_vTaskExitCriticalFromISR()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vTaskListTasks
    #घोषणा traceENTER_vTaskListTasks( pc_Write_बफर, ux_बफर_लंबाई )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vTaskListTasks
    #घोषणा traceRETURN_vTaskListTasks()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vTaskGetRunTimeStatistics
    #घोषणा traceENTER_vTaskGetRunTimeStatistics( pc_Write_बफर, ux_बफर_लंबाई )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vTaskGetRunTimeStatistics
    #घोषणा traceRETURN_vTaskGetRunTimeStatistics()
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_ux_Task_Reset_Event_Item_मूल्य
    #घोषणा trace_ENTER_ux_Task_Reset_Event_Item_मूल्य()
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_ux_Task_Reset_Event_Item_मूल्य
    #घोषणा trace_RETURN_ux_Task_Reset_Event_Item_मूल्य( uxReturn )
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_pv_Task_Increment_Mutex_Held_गिनती
    #घोषणा trace_ENTER_pv_Task_Increment_Mutex_Held_गिनती()
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_pv_Task_Increment_Mutex_Held_गिनती
    #घोषणा trace_RETURN_pv_Task_Increment_Mutex_Held_गिनती( pxTCB )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_ulTaskGenericNotifyTake
    #घोषणा traceENTER_ulTaskGenericNotifyTake( ux_सूचकांक_To_Wait_On, x_Clear_गिनती_On_Exit, xTicksToWait )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_ulTaskGenericNotifyTake
    #घोषणा traceRETURN_ulTaskGenericNotifyTake( ulReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTaskGenericNotifyWait
    #घोषणा traceENTER_xTaskGenericNotifyWait( ux_सूचकांक_To_Wait_On, ulBitsToClearOnEntry, ulBitsToClearOnExit, pul_Notification_मूल्य, xTicksToWait )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTaskGenericNotifyWait
    #घोषणा traceRETURN_xTaskGenericNotifyWait( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTaskGenericNotify
    #घोषणा traceENTER_xTaskGenericNotify( xTaskToNotify, ux_सूचकांक_To_Notify, ul_मूल्य, eAction, pul_Previous_Notification_मूल्य )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTaskGenericNotify
    #घोषणा traceRETURN_xTaskGenericNotify( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTaskGenericNotifyFromISR
    #घोषणा traceENTER_xTaskGenericNotifyFromISR( xTaskToNotify, ux_सूचकांक_To_Notify, ul_मूल्य, eAction, pul_Previous_Notification_मूल्य, pxHigherPriorityTaskWoken )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTaskGenericNotifyFromISR
    #घोषणा traceRETURN_xTaskGenericNotifyFromISR( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vTaskGenericNotifyGiveFromISR
    #घोषणा traceENTER_vTaskGenericNotifyGiveFromISR( xTaskToNotify, ux_सूचकांक_To_Notify, pxHigherPriorityTaskWoken )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vTaskGenericNotifyGiveFromISR
    #घोषणा traceRETURN_vTaskGenericNotifyGiveFromISR()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTaskGenericNotifyStateClear
    #घोषणा traceENTER_xTaskGenericNotifyStateClear( xTask, ux_सूचकांक_To_Clear )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTaskGenericNotifyStateClear
    #घोषणा traceRETURN_xTaskGenericNotifyStateClear( xReturn )
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_ul_Task_Generic_Notify_मूल्य_Clear
    #घोषणा trace_ENTER_ul_Task_Generic_Notify_मूल्य_Clear( xTask, ux_सूचकांक_To_Clear, ulBitsToClear )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_ul_Task_Generic_Notify_मूल्य_Clear
    #घोषणा trace_RETURN_ul_Task_Generic_Notify_मूल्य_Clear( ulReturn )
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_ul_Task_Get_Run_Time_गणक
    #घोषणा trace_ENTER_ul_Task_Get_Run_Time_गणक( xTask )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_ul_Task_Get_Run_Time_गणक
    #घोषणा trace_RETURN_ul_Task_Get_Run_Time_गणक( ul_Run_Time_गणक )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_ulTaskGetRunTimePercent
    #घोषणा traceENTER_ulTaskGetRunTimePercent( xTask )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_ulTaskGetRunTimePercent
    #घोषणा traceRETURN_ulTaskGetRunTimePercent( ulReturn )
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_ul_Task_Get_Idle_Run_Time_गणक
    #घोषणा trace_ENTER_ul_Task_Get_Idle_Run_Time_गणक()
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_ul_Task_Get_Idle_Run_Time_गणक
    #घोषणा trace_RETURN_ul_Task_Get_Idle_Run_Time_गणक( ulReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_ulTaskGetIdleRunTimePercent
    #घोषणा traceENTER_ulTaskGetIdleRunTimePercent()
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_ulTaskGetIdleRunTimePercent
    #घोषणा traceRETURN_ulTaskGetIdleRunTimePercent( ulReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xTaskGetMPUSettings
    #घोषणा traceENTER_xTaskGetMPUSettings( xTask )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xTaskGetMPUSettings
    #घोषणा traceRETURN_xTaskGetMPUSettings( xMPUSettings )
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_x_Stream_बफर_Generic_Create
    #घोषणा trace_ENTER_x_Stream_बफर_Generic_Create( x_बफर_आकार_Bytes, xTriggerLevelBytes, x_Stream_बफर_Type, pxSendCompletedCallback, pxReceiveCompletedCallback )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_x_Stream_बफर_Generic_Create
    #घोषणा trace_RETURN_x_Stream_बफर_Generic_Create( pvAllocatedMemory )
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_x_Stream_बफर_Generic_Create_Static
    #घोषणा trace_ENTER_x_Stream_बफर_Generic_Create_Static( x_बफर_आकार_Bytes, xTriggerLevelBytes, x_Stream_बफर_Type, puc_Stream_बफर_Storage_Area, px_Static_Stream_बफर, pxSendCompletedCallback, pxReceiveCompletedCallback )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_x_Stream_बफर_Generic_Create_Static
    #घोषणा trace_RETURN_x_Stream_बफर_Generic_Create_Static( xReturn )
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_x_Stream_बफर_Get_Static_Buffers
    #घोषणा trace_ENTER_x_Stream_बफर_Get_Static_Buffers( x_Stream_बफर, ppuc_Stream_बफर_Storage_Area, ppx_Static_Stream_बफर )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_x_Stream_बफर_Get_Static_Buffers
    #घोषणा trace_RETURN_x_Stream_बफर_Get_Static_Buffers( xReturn )
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_v_Stream_बफर_Delete
    #घोषणा trace_ENTER_v_Stream_बफर_Delete( x_Stream_बफर )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_v_Stream_बफर_Delete
    #घोषणा trace_RETURN_v_Stream_बफर_Delete()
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_x_Stream_बफर_Reset
    #घोषणा trace_ENTER_x_Stream_बफर_Reset( x_Stream_बफर )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_x_Stream_बफर_Reset
    #घोषणा trace_RETURN_x_Stream_बफर_Reset( xReturn )
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_x_Stream_बफर_Reset_From_ISR
    #घोषणा trace_ENTER_x_Stream_बफर_Reset_From_ISR( x_Stream_बफर )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_x_Stream_बफर_Reset_From_ISR
    #घोषणा trace_RETURN_x_Stream_बफर_Reset_From_ISR( xReturn )
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_x_Stream_बफर_Set_Trigger_Level
    #घोषणा trace_ENTER_x_Stream_बफर_Set_Trigger_Level( x_Stream_बफर, xTriggerLevel )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_x_Stream_बफर_Set_Trigger_Level
    #घोषणा trace_RETURN_x_Stream_बफर_Set_Trigger_Level( xReturn )
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_x_Stream_बफर_Spaces_Available
    #घोषणा trace_ENTER_x_Stream_बफर_Spaces_Available( x_Stream_बफर )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_x_Stream_बफर_Spaces_Available
    #घोषणा trace_RETURN_x_Stream_बफर_Spaces_Available( xSpace )
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_x_Stream_बफर_Bytes_Available
    #घोषणा trace_ENTER_x_Stream_बफर_Bytes_Available( x_Stream_बफर )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_x_Stream_बफर_Bytes_Available
    #घोषणा trace_RETURN_x_Stream_बफर_Bytes_Available( xReturn )
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_x_Stream_बफर_Send
    #घोषणा trace_ENTER_x_Stream_बफर_Send( x_Stream_बफर, pv_Tx_तथ्य, x_तथ्य_लंबाई_Bytes, xTicksToWait )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_x_Stream_बफर_Send
    #घोषणा trace_RETURN_x_Stream_बफर_Send( xReturn )
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_x_Stream_बफर_Send_From_ISR
    #घोषणा trace_ENTER_x_Stream_बफर_Send_From_ISR( x_Stream_बफर, pv_Tx_तथ्य, x_तथ्य_लंबाई_Bytes, pxHigherPriorityTaskWoken )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_x_Stream_बफर_Send_From_ISR
    #घोषणा trace_RETURN_x_Stream_बफर_Send_From_ISR( xReturn )
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_x_Stream_बफर_Receive
    #घोषणा trace_ENTER_x_Stream_बफर_Receive( x_Stream_बफर, pv_Rx_तथ्य, x_बफर_लंबाई_Bytes, xTicksToWait )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_x_Stream_बफर_Receive
    #घोषणा trace_RETURN_x_Stream_बफर_Receive( x_Received_लंबाई )
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_x_Stream_बफर_अग्र_Message_लंबाई_Bytes
    #घोषणा trace_ENTER_x_Stream_बफर_अग्र_Message_लंबाई_Bytes( x_Stream_बफर )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_x_Stream_बफर_अग्र_Message_लंबाई_Bytes
    #घोषणा trace_RETURN_x_Stream_बफर_अग्र_Message_लंबाई_Bytes( xReturn )
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_x_Stream_बफर_Receive_From_ISR
    #घोषणा trace_ENTER_x_Stream_बफर_Receive_From_ISR( x_Stream_बफर, pv_Rx_तथ्य, x_बफर_लंबाई_Bytes, pxHigherPriorityTaskWoken )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_x_Stream_बफर_Receive_From_ISR
    #घोषणा trace_RETURN_x_Stream_बफर_Receive_From_ISR( x_Received_लंबाई )
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_x_Stream_बफर_Is_Empty
    #घोषणा trace_ENTER_x_Stream_बफर_Is_Empty( x_Stream_बफर )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_x_Stream_बफर_Is_Empty
    #घोषणा trace_RETURN_x_Stream_बफर_Is_Empty( xReturn )
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_x_Stream_बफर_Is_Full
    #घोषणा trace_ENTER_x_Stream_बफर_Is_Full( x_Stream_बफर )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_x_Stream_बफर_Is_Full
    #घोषणा trace_RETURN_x_Stream_बफर_Is_Full( xReturn )
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_x_Stream_बफर_Send_Completed_From_ISR
    #घोषणा trace_ENTER_x_Stream_बफर_Send_Completed_From_ISR( x_Stream_बफर, pxHigherPriorityTaskWoken )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_x_Stream_बफर_Send_Completed_From_ISR
    #घोषणा trace_RETURN_x_Stream_बफर_Send_Completed_From_ISR( xReturn )
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_x_Stream_बफर_Receive_Completed_From_ISR
    #घोषणा trace_ENTER_x_Stream_बफर_Receive_Completed_From_ISR( x_Stream_बफर, pxHigherPriorityTaskWoken )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_x_Stream_बफर_Receive_Completed_From_ISR
    #घोषणा trace_RETURN_x_Stream_बफर_Receive_Completed_From_ISR( xReturn )
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_ux_Stream_बफर_Get_Stream_बफर_Notification_सूचकांक
    #घोषणा trace_ENTER_ux_Stream_बफर_Get_Stream_बफर_Notification_सूचकांक( x_Stream_बफर )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_ux_Stream_बफर_Get_Stream_बफर_Notification_सूचकांक
    #घोषणा trace_RETURN_ux_Stream_बफर_Get_Stream_बफर_Notification_सूचकांक( ux_Notification_सूचकांक )
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_v_Stream_बफर_Set_Stream_बफर_Notification_सूचकांक
    #घोषणा trace_ENTER_v_Stream_बफर_Set_Stream_बफर_Notification_सूचकांक( x_Stream_बफर, ux_Notification_सूचकांक )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_v_Stream_बफर_Set_Stream_बफर_Notification_सूचकांक
    #घोषणा trace_RETURN_v_Stream_बफर_Set_Stream_बफर_Notification_सूचकांक()
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_ux_Stream_बफर_Get_Stream_बफर_संख्या
    #घोषणा trace_ENTER_ux_Stream_बफर_Get_Stream_बफर_संख्या( x_Stream_बफर )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_ux_Stream_बफर_Get_Stream_बफर_संख्या
    #घोषणा trace_RETURN_ux_Stream_बफर_Get_Stream_बफर_संख्या( ux_Stream_बफर_संख्या )
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_v_Stream_बफर_Set_Stream_बफर_संख्या
    #घोषणा trace_ENTER_v_Stream_बफर_Set_Stream_बफर_संख्या( x_Stream_बफर, ux_Stream_बफर_संख्या )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_v_Stream_बफर_Set_Stream_बफर_संख्या
    #घोषणा trace_RETURN_v_Stream_बफर_Set_Stream_बफर_संख्या()
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_uc_Stream_बफर_Get_Stream_बफर_Type
    #घोषणा trace_ENTER_uc_Stream_बफर_Get_Stream_बफर_Type( x_Stream_बफर )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_uc_Stream_बफर_Get_Stream_बफर_Type
    #घोषणा trace_RETURN_uc_Stream_बफर_Get_Stream_बफर_Type( uc_Stream_बफर_Type )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vListInitialise
    #घोषणा traceENTER_vListInitialise( pxList )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vListInitialise
    #घोषणा traceRETURN_vListInitialise()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vListInitialiseItem
    #घोषणा traceENTER_vListInitialiseItem( pxItem )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vListInitialiseItem
    #घोषणा traceRETURN_vListInitialiseItem()
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_v_List_जोड़ो_End
    #घोषणा trace_ENTER_v_List_जोड़ो_End( pxList, pxNewListItem )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_v_List_जोड़ो_End
    #घोषणा trace_RETURN_v_List_जोड़ो_End()
#पूर्ण_अगर

#अगर_अघोषित trace_ENTER_v_List_जोड़ो
    #घोषणा trace_ENTER_v_List_जोड़ो( pxList, pxNewListItem )
#पूर्ण_अगर

#अगर_अघोषित trace_RETURN_v_List_जोड़ो
    #घोषणा trace_RETURN_v_List_जोड़ो()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_uxListRemove
    #घोषणा traceENTER_uxListRemove( pxItemToRemove )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_uxListRemove
    #घोषणा traceRETURN_uxListRemove( ux_संख्या_Of_वस्तुएँ )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xCoRoutineCreate
    #घोषणा traceENTER_xCoRoutineCreate( pxCoRoutineCode, uxPriority, ux_सूचकांक )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xCoRoutineCreate
    #घोषणा traceRETURN_xCoRoutineCreate( xReturn )
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vCoRoutineAddToDelayedList
    #घोषणा traceENTER_vCoRoutineAddToDelayedList( xTicksToDelay, pxEventList )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vCoRoutineAddToDelayedList
    #घोषणा traceRETURN_vCoRoutineAddToDelayedList()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_vCoRoutineSchedule
    #घोषणा traceENTER_vCoRoutineSchedule()
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_vCoRoutineSchedule
    #घोषणा traceRETURN_vCoRoutineSchedule()
#पूर्ण_अगर

#अगर_अघोषित traceENTER_xCoRoutineRemoveFromEventList
    #घोषणा traceENTER_xCoRoutineRemoveFromEventList( pxEventList )
#पूर्ण_अगर

#अगर_अघोषित traceRETURN_xCoRoutineRemoveFromEventList
    #घोषणा traceRETURN_xCoRoutineRemoveFromEventList( xReturn )
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

#अगर_अघोषित configENABLE_HEAP_PROTECTOR
    #घोषणा configENABLE_HEAP_PROTECTOR    0
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

#अगर_अघोषित configKERNEL_PROVIDED_STATIC_MEMORY
    #घोषणा configKERNEL_PROVIDED_STATIC_MEMORY    0
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

#अगर_अघोषित config_STATS_बफर_MAX_लंबाई
    #घोषणा config_STATS_बफर_MAX_लंबाई    0xFFFF
#पूर्ण_अगर

#अगर_अघोषित config_ढेर_DEPTH_TYPE

/* Defaults to StackType_t for backward compatibility, but can be overridden
 * in FreeRTOSConfig.h if StackType_t is too restrictive. */
    #घोषणा config_ढेर_DEPTH_TYPE    ढेर_Type_t
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

#अगर ( ( configRUN_MULTIPLE_PRIORITIES == 0 ) && ( configUSE_TASK_PREEMPTION_DISABLE != 0 ) )
    #त्रुटि configRUN_MULTIPLE_PRIORITIES must be set to 1 to use task preemption disable
#पूर्ण_अगर

#अगर ( ( configUSE_PREEMPTION == 0 ) && ( configUSE_TASK_PREEMPTION_DISABLE != 0 ) )
    #त्रुटि configUSE_PREEMPTION must be set to 1 to use task preemption disable
#पूर्ण_अगर

#अगर ( ( config_संख्या_OF_CORES == 1 ) && ( configUSE_TASK_PREEMPTION_DISABLE != 0 ) )
    #त्रुटि configUSE_TASK_PREEMPTION_DISABLE is not supported in single core FreeRTOS
#पूर्ण_अगर

#अगर ( ( config_संख्या_OF_CORES == 1 ) && ( configUSE_CORE_AFFINITY != 0 ) )
    #त्रुटि configUSE_CORE_AFFINITY is not supported in single core FreeRTOS
#पूर्ण_अगर

#अगर ( ( config_संख्या_OF_CORES > 1 ) && ( configUSE_PORT_OPTIMISED_TASK_SELECTION != 0 ) )
    #त्रुटि configUSE_PORT_OPTIMISED_TASK_SELECTION is not supported in SMP FreeRTOS
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

/* The following config allows infinite loop control. For example, control the
 * infinite loop in idle task function when performing unit tests. */
#अगर_अघोषित configCONTROL_INFINITE_LOOP
    #घोषणा configCONTROL_INFINITE_LOOP()
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
    #अगर ( configUSE_CORE_AFFINITY == 1 ) && ( config_संख्या_OF_CORES > 1 )
        UBaseType_t uxDummy26;
    #पूर्ण_अगर
    StaticListItem_t xDummy3[ 2 ];
    UBaseType_t uxDummy5;
    व्योम * pxDummy6;
    #अगर ( config_संख्या_OF_CORES > 1 )
        BaseType_t xDummy23;
        UBaseType_t uxDummy24;
    #पूर्ण_अगर
    uint8_t ucDummy7[ configMAX_TASK_NAME_LEN ];
    #अगर ( configUSE_TASK_PREEMPTION_DISABLE == 1 )
        BaseType_t xDummy25;
    #पूर्ण_अगर
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
    #अगर ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 )
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
    UBaseType_t uxDummy6;
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
