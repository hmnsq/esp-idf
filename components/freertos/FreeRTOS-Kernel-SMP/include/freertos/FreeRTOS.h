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

#నిర్వచితం_కాకపోతే INC_FREERTOS_H
#నిర్వచించు INC_FREERTOS_H

/*
 * Include the generic headers required for the FreeRTOS port being used.
 */
#అన్తర్భావ <stddef.h>

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
#అన్తర్భావ <stdint.h> /* READ COMMENT ABOVE. */

/* *INDENT-OFF* */
#నిర్వచితమైతే __cplusplus
    బాహ్య "C" {
#అయితే_ముగింపు
/* *INDENT-ON* */

/* Acceptable values for configTICK_TYPE_WIDTH_IN_BITS. */
#నిర్వచించు TICK_TYPE_వెడల్పు_16_బిట్లు    0
#నిర్వచించు TICK_TYPE_వెడల్పు_32_బిట్లు    1
#నిర్వచించు TICK_TYPE_వెడల్పు_64_బిట్లు    2

/* Application specific configuration options. */
#అన్తర్భావ "FreeRTOSConfig.h"

#ఒకవేళ !defined( config_USE_16_బిట్టు_TICKS ) && !defined( config_TICK_TYPE_వెడల్పు_లోపల_బిట్లు )
    #దోషం Missing definition:  One యొక్క config_USE_16_బిట్టు_TICKS and config_TICK_TYPE_వెడల్పు_లోపల_బిట్లు must be defined లోపల FreeRTOSConfig.h.  See the Configuration section యొక్క the FreeRTOS API documentation కొరకు details.
#అయితే_ముగింపు

#ఒకవేళ defined( config_USE_16_బిట్టు_TICKS ) && defined( config_TICK_TYPE_వెడల్పు_లోపల_బిట్లు )
    #దోషం Only one యొక్క config_USE_16_బిట్టు_TICKS and config_TICK_TYPE_వెడల్పు_లోపల_బిట్లు must be defined లోపల FreeRTOSConfig.h.  See the Configuration section యొక్క the FreeRTOS API documentation కొరకు details.
#అయితే_ముగింపు

/* Define configTICK_TYPE_WIDTH_IN_BITS according to the
 * value of configUSE_16_BIT_TICKS for backward compatibility. */
#నిర్వచితం_కాకపోతే config_TICK_TYPE_వెడల్పు_లోపల_బిట్లు
    #ఒకవేళ ( config_USE_16_బిట్టు_TICKS == 1 )
        #నిర్వచించు config_TICK_TYPE_వెడల్పు_లోపల_బిట్లు    TICK_TYPE_వెడల్పు_16_బిట్లు
    #లేకపోతే
        #నిర్వచించు config_TICK_TYPE_వెడల్పు_లోపల_బిట్లు    TICK_TYPE_వెడల్పు_32_బిట్లు
    #అయితే_ముగింపు
#అయితే_ముగింపు

/* Set configUSE_MPU_WRAPPERS_V1 to 1 to use MPU wrappers v1. */
#నిర్వచితం_కాకపోతే configUSE_MPU_WRAPPERS_V1
    #నిర్వచించు configUSE_MPU_WRAPPERS_V1    0
#అయితే_ముగింపు

/* Set configENABLE_ACCESS_CONTROL_LIST to 1 to enable access control list support. */
#నిర్వచితం_కాకపోతే configENABLE_ACCESS_CONTROL_LIST
    #నిర్వచించు configENABLE_ACCESS_CONTROL_LIST    0
#అయితే_ముగింపు

/* Set default value of configNUMBER_OF_CORES to 1 to use single core FreeRTOS. */
#నిర్వచితం_కాకపోతే config_సంఖ్య_యొక్క_CORES
    #నిర్వచించు config_సంఖ్య_యొక్క_CORES    1
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే configUSE_MALLOC_FAILED_HOOK
    #నిర్వచించు configUSE_MALLOC_FAILED_HOOK    0
#అయితే_ముగింపు

/* Basic FreeRTOS definitions. */
#అన్తర్భావ "projdefs.h"

/* Definitions specific to the port being used. */
#అన్తర్భావ "portable.h"

/* Must be defaulted before configUSE_NEWLIB_REENTRANT is used below. */
#నిర్వచితం_కాకపోతే configUSE_NEWLIB_REENTRANT
    #నిర్వచించు configUSE_NEWLIB_REENTRANT    0
#అయితే_ముగింపు

/* Required if struct _reent is used. */
#ఒకవేళ ( configUSE_NEWLIB_REENTRANT == 1 )

    #అన్తర్భావ "newlib-freertos.h"

#అయితే_ముగింపు /* if ( configUSE_NEWLIB_REENTRANT == 1 ) */

/* Must be defaulted before configUSE_PICOLIBC_TLS is used below. */
#నిర్వచితం_కాకపోతే configUSE_PICOLIBC_TLS
    #నిర్వచించు configUSE_PICOLIBC_TLS    0
#అయితే_ముగింపు

#ఒకవేళ ( configUSE_PICOLIBC_TLS == 1 )

    #అన్తర్భావ "picolibc-freertos.h"

#అయితే_ముగింపు /* if ( configUSE_PICOLIBC_TLS == 1 ) */

#నిర్వచితం_కాకపోతే configUSE_C_RUNTIME_TLS_SUPPORT
    #నిర్వచించు configUSE_C_RUNTIME_TLS_SUPPORT    0
#అయితే_ముగింపు

#ఒకవేళ ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 )

    #నిర్వచితం_కాకపోతే config_TLS_ఖండం_TYPE
        #దోషం Missing definition:  config_TLS_ఖండం_TYPE must be defined లోపల FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is సమితి గమ్యం 1.
    #అయితే_ముగింపు

    #నిర్వచితం_కాకపోతే config_INIT_TLS_ఖండం
        #దోషం Missing definition:  config_INIT_TLS_ఖండం must be defined లోపల FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is సమితి గమ్యం 1.
    #అయితే_ముగింపు

    #నిర్వచితం_కాకపోతే config_సమితి_TLS_ఖండం
        #దోషం Missing definition:  config_సమితి_TLS_ఖండం must be defined లోపల FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is సమితి గమ్యం 1.
    #అయితే_ముగింపు

    #నిర్వచితం_కాకపోతే config_DEINIT_TLS_ఖండం
        #దోషం Missing definition:  config_DEINIT_TLS_ఖండం must be defined లోపల FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is సమితి గమ్యం 1.
    #అయితే_ముగింపు
#అయితే_ముగింపు /* if ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 ) */

/*
 * Check all the required application specific macros have been defined.
 * These macros are application specific and (as downloaded) are defined
 * within FreeRTOSConfig.h.
 */

#నిర్వచితం_కాకపోతే config_కనీస_దొంతర_కొలత
    #దోషం Missing definition:  config_కనీస_దొంతర_కొలత must be defined లోపల FreeRTOSConfig.h.  config_కనీస_దొంతర_కొలత నిర్వచిస్తుంది the కొలత (లోపల పదాలు) యొక్క the దొంతర allocated గమ్యం the idle task.  Refer గమ్యం the demo project provided కొరకు your పోర్టు కొరకు a suitable విలువ.
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే configMAX_PRIORITIES
    #దోషం Missing definition:  configMAX_PRIORITIES must be defined లోపల FreeRTOSConfig.h.  See the Configuration section యొక్క the FreeRTOS API documentation కొరకు details.
#అయితే_ముగింపు

#ఒకవేళ configMAX_PRIORITIES < 1
    #దోషం configMAX_PRIORITIES must be defined గమ్యం be greater than or equal గమ్యం 1.
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే configUSE_PREEMPTION
    #దోషం Missing definition:  configUSE_PREEMPTION must be defined లోపల FreeRTOSConfig.h as either 1 or 0.  See the Configuration section యొక్క the FreeRTOS API documentation కొరకు details.
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే configUSE_IDLE_HOOK
    #దోషం Missing definition:  configUSE_IDLE_HOOK must be defined లోపల FreeRTOSConfig.h as either 1 or 0.  See the Configuration section యొక్క the FreeRTOS API documentation కొరకు details.
#అయితే_ముగింపు

#ఒకవేళ ( config_సంఖ్య_యొక్క_CORES > 1 )
    #నిర్వచితం_కాకపోతే configUSE_PASSIVE_IDLE_HOOK
        #దోషం Missing definition:  configUSE_PASSIVE_IDLE_HOOK must be defined లోపల FreeRTOSConfig.h as either 1 or 0.  See the Configuration section యొక్క the FreeRTOS API documentation కొరకు details.
    #అయితే_ముగింపు
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే configUSE_TICK_HOOK
    #దోషం Missing definition:  configUSE_TICK_HOOK must be defined లోపల FreeRTOSConfig.h as either 1 or 0.  See the Configuration section యొక్క the FreeRTOS API documentation కొరకు details.
#అయితే_ముగింపు

#ఒకవేళ ( ( config_TICK_TYPE_వెడల్పు_లోపల_బిట్లు != TICK_TYPE_వెడల్పు_16_బిట్లు ) && \
    ( config_TICK_TYPE_వెడల్పు_లోపల_బిట్లు != TICK_TYPE_వెడల్పు_32_బిట్లు ) &&   \
    ( config_TICK_TYPE_వెడల్పు_లోపల_బిట్లు != TICK_TYPE_వెడల్పు_64_బిట్లు ) )
    #దోషం Macro config_TICK_TYPE_వెడల్పు_లోపల_బిట్లు is defined గమ్యం incorrect విలువ.  See the Configuration section యొక్క the FreeRTOS API documentation కొరకు details.
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే configUSE_CO_ROUTINES
    #నిర్వచించు configUSE_CO_ROUTINES    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే INCLUDE_v_Task_ప్రాధాన్యత_సమితి
    #నిర్వచించు INCLUDE_v_Task_ప్రాధాన్యత_సమితి    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే INCLUDE_ux_Task_ప్రాధాన్యత_Get
    #నిర్వచించు INCLUDE_ux_Task_ప్రాధాన్యత_Get    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే INCLUDE_vTaskDelete
    #నిర్వచించు INCLUDE_vTaskDelete    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే INCLUDE_vTaskSuspend
    #నిర్వచించు INCLUDE_vTaskSuspend    0
#అయితే_ముగింపు

#నిర్వచితమైతే INCLUDE_xTaskDelayUntil
    #నిర్వచితమైతే INCLUDE_vTaskDelayUntil

/* INCLUDE_vTaskDelayUntil was replaced by INCLUDE_xTaskDelayUntil.  Backward
 * compatibility is maintained if only one or the other is defined, but
 * there is a conflict if both are defined. */
        #దోషం INCLUDE_vTaskDelayUntil and INCLUDE_xTaskDelayUntil are both defined.  INCLUDE_vTaskDelayUntil is no longer required and should be removed
    #అయితే_ముగింపు
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే INCLUDE_xTaskDelayUntil
    #నిర్వచితమైతే INCLUDE_vTaskDelayUntil

/* If INCLUDE_vTaskDelayUntil is set but INCLUDE_xTaskDelayUntil is not then
 * the project's FreeRTOSConfig.h probably pre-dates the introduction of
 * xTaskDelayUntil and setting INCLUDE_xTaskDelayUntil to whatever
 * INCLUDE_vTaskDelayUntil is set to will ensure backward compatibility.
 */
        #నిర్వచించు INCLUDE_xTaskDelayUntil    INCLUDE_vTaskDelayUntil
    #అయితే_ముగింపు
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే INCLUDE_xTaskDelayUntil
    #నిర్వచించు INCLUDE_xTaskDelayUntil    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే INCLUDE_vTaskDelay
    #నిర్వచించు INCLUDE_vTaskDelay    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే INCLUDE_xTaskGetIdleTaskHandle
    #నిర్వచించు INCLUDE_xTaskGetIdleTaskHandle    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే INCLUDE_xTaskAbortDelay
    #నిర్వచించు INCLUDE_xTaskAbortDelay    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే INCLUDE_x_వరుస_Get_Mutex_Holder
    #నిర్వచించు INCLUDE_x_వరుస_Get_Mutex_Holder    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే INCLUDE_xSemaphoreGetMutexHolder
    #నిర్వచించు INCLUDE_xSemaphoreGetMutexHolder    INCLUDE_x_వరుస_Get_Mutex_Holder
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే INCLUDE_xTaskGetHandle
    #నిర్వచించు INCLUDE_xTaskGetHandle    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే INCLUDE_ux_Task_Get_దొంతర_ఎగువ_Water_Mark
    #నిర్వచించు INCLUDE_ux_Task_Get_దొంతర_ఎగువ_Water_Mark    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే INCLUDE_ux_Task_Get_దొంతర_ఎగువ_Water_Mark2
    #నిర్వచించు INCLUDE_ux_Task_Get_దొంతర_ఎగువ_Water_Mark2    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే INCLUDE_eTaskGetState
    #నిర్వచించు INCLUDE_eTaskGetState    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే INCLUDE_xTaskResumeFromISR
    #నిర్వచించు INCLUDE_xTaskResumeFromISR    1
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే INCLUDE_xTimerPendFunctionCall
    #నిర్వచించు INCLUDE_xTimerPendFunctionCall    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే INCLUDE_xTaskGetSchedulerState
    #నిర్వచించు INCLUDE_xTaskGetSchedulerState    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే INCLUDE_xTaskGetCurrentTaskHandle
    #నిర్వచించు INCLUDE_xTaskGetCurrentTaskHandle    1
#అయితే_ముగింపు

#ఒకవేళ configUSE_CO_ROUTINES != 0
    #నిర్వచితం_కాకపోతే configMAX_CO_ROUTINE_PRIORITIES
        #దోషం configMAX_CO_ROUTINE_PRIORITIES must be greater than or equal గమ్యం 1.
    #అయితే_ముగింపు
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే config_USE_APPLICATION_TASK_ట్యాగ్
    #నిర్వచించు config_USE_APPLICATION_TASK_ట్యాగ్    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే config_అంకె_THREAD_LOCAL_STORAGE_సూచికలు
    #నిర్వచించు config_అంకె_THREAD_LOCAL_STORAGE_సూచికలు    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే configUSE_RECURSIVE_MUTEXES
    #నిర్వచించు configUSE_RECURSIVE_MUTEXES    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే configUSE_MUTEXES
    #నిర్వచించు configUSE_MUTEXES    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే configUSE_TIMERS
    #నిర్వచించు configUSE_TIMERS    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే configUSE_EVENT_GROUPS
    #నిర్వచించు configUSE_EVENT_GROUPS    1
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే config_USE_ప్రవాహం_BUFFERS
    #నిర్వచించు config_USE_ప్రవాహం_BUFFERS    1
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే configUSE_DAEMON_TASK_STARTUP_HOOK
    #నిర్వచించు configUSE_DAEMON_TASK_STARTUP_HOOK    0
#అయితే_ముగింపు

#ఒకవేళ ( configUSE_DAEMON_TASK_STARTUP_HOOK != 0 )
    #ఒకవేళ ( configUSE_TIMERS == 0 )
        #దోషం configUSE_DAEMON_TASK_STARTUP_HOOK is సమితి, but the daemon task is not created because configUSE_TIMERS is 0.
    #అయితే_ముగింపు
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే configUSE_COUNTING_SEMAPHORES
    #నిర్వచించు configUSE_COUNTING_SEMAPHORES    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే configUSE_TASK_PREEMPTION_DISABLE
    #నిర్వచించు configUSE_TASK_PREEMPTION_DISABLE    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే configUSE_ALTERNATIVE_API
    #నిర్వచించు configUSE_ALTERNATIVE_API    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే పోర్టు_CRITICAL_NESTING_లోపల_TCB
    #నిర్వచించు పోర్టు_CRITICAL_NESTING_లోపల_TCB    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే configMAX_TASK_NAME_LEN
    #నిర్వచించు configMAX_TASK_NAME_LEN    16
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే configIDLE_SHOULD_YIELD
    #నిర్వచించు configIDLE_SHOULD_YIELD    1
#అయితే_ముగింపు

#ఒకవేళ configMAX_TASK_NAME_LEN < 1
    #దోషం configMAX_TASK_NAME_LEN must be సమితి గమ్యం a minimum యొక్క 1 లోపల FreeRTOSConfig.h
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే configASSERT
    #నిర్వచించు configASSERT( x )
    #నిర్వచించు configASSERT_DEFINED    0
#లేకపోతే
    #నిర్వచించు configASSERT_DEFINED    1
#అయితే_ముగింపు

/* configPRECONDITION should be defined as configASSERT.
 * The CBMC proofs need a way to track assumptions and assertions.
 * A configPRECONDITION statement should express an implicit invariant or
 * assumption made.  A configASSERT statement should express an invariant that must
 * hold explicit before calling the code. */
#నిర్వచితం_కాకపోతే configPRECONDITION
    #నిర్వచించు configPRECONDITION( X )    configASSERT( X )
    #నిర్వచించు configPRECONDITION_DEFINED    0
#లేకపోతే
    #నిర్వచించు configPRECONDITION_DEFINED    1
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే configCHECK_HANDLER_INSTALLATION
    #నిర్వచించు configCHECK_HANDLER_INSTALLATION    1
#లేకపోతే

/* The application has explicitly defined configCHECK_HANDLER_INSTALLATION
 * to 1. The checks requires configASSERT() to be defined. */
    #ఒకవేళ ( ( configCHECK_HANDLER_INSTALLATION == 1 ) && ( configASSERT_DEFINED == 0 ) )
        #దోషం You must define configASSERT() when configCHECK_HANDLER_INSTALLATION is 1.
    #అయితే_ముగింపు
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే పోర్టు_జ్ఞాపకం_BARRIER
    #నిర్వచించు పోర్టు_జ్ఞాపకం_BARRIER()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే పోర్టు_SOFTWARE_BARRIER
    #నిర్వచించు పోర్టు_SOFTWARE_BARRIER()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే config_నిరంతరం_గుణిజం_PRIORITIES
    #నిర్వచించు config_నిరంతరం_గుణిజం_PRIORITIES    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే పోర్టు_GET_CORE_ID

    #ఒకవేళ ( config_సంఖ్య_యొక్క_CORES == 1 )
        #నిర్వచించు పోర్టు_GET_CORE_ID()    0
    #లేకపోతే
        #దోషం config_సంఖ్య_యొక్క_CORES is సమితి గమ్యం మరిన్ని than 1 then పోర్టు_GET_CORE_ID must also be defined.
    #అయితే_ముగింపు /* configNUMBER_OF_CORES */

#అయితే_ముగింపు /* portGET_CORE_ID */

#నిర్వచితం_కాకపోతే పోర్టు_YIELD_CORE

    #ఒకవేళ ( config_సంఖ్య_యొక్క_CORES == 1 )
        #నిర్వచించు పోర్టు_YIELD_CORE( x )    పోర్టు_YIELD()
    #లేకపోతే
        #దోషం config_సంఖ్య_యొక్క_CORES is సమితి గమ్యం మరిన్ని than 1 then పోర్టు_YIELD_CORE must also be defined.
    #అయితే_ముగింపు /* configNUMBER_OF_CORES */

#అయితే_ముగింపు /* portYIELD_CORE */

#నిర్వచితం_కాకపోతే పోర్టు_సమితి_అంతరాయం_ముసుగు

    #ఒకవేళ ( config_సంఖ్య_యొక్క_CORES > 1 )
        #దోషం పోర్టు_సమితి_అంతరాయం_ముసుగు is required లోపల SMP
    #అయితే_ముగింపు

#అయితే_ముగింపు /* portSET_INTERRUPT_MASK */

#నిర్వచితం_కాకపోతే పోర్టు_CLEAR_అంతరాయం_ముసుగు

    #ఒకవేళ ( config_సంఖ్య_యొక్క_CORES > 1 )
        #దోషం పోర్టు_CLEAR_అంతరాయం_ముసుగు is required లోపల SMP
    #అయితే_ముగింపు

#అయితే_ముగింపు /* portCLEAR_INTERRUPT_MASK */

#నిర్వచితం_కాకపోతే పోర్టు_RELEASE_TASK_తాళం

    #ఒకవేళ ( config_సంఖ్య_యొక్క_CORES == 1 )
        #నిర్వచించు పోర్టు_RELEASE_TASK_తాళం()
    #లేకపోతే
        #దోషం పోర్టు_RELEASE_TASK_తాళం is required లోపల SMP
    #అయితే_ముగింపు

#అయితే_ముగింపు /* portRELEASE_TASK_LOCK */

#నిర్వచితం_కాకపోతే పోర్టు_GET_TASK_తాళం

    #ఒకవేళ ( config_సంఖ్య_యొక్క_CORES == 1 )
        #నిర్వచించు పోర్టు_GET_TASK_తాళం()
    #లేకపోతే
        #దోషం పోర్టు_GET_TASK_తాళం is required లోపల SMP
    #అయితే_ముగింపు

#అయితే_ముగింపు /* portGET_TASK_LOCK */

#నిర్వచితం_కాకపోతే పోర్టు_RELEASE_ISR_తాళం

    #ఒకవేళ ( config_సంఖ్య_యొక్క_CORES == 1 )
        #నిర్వచించు పోర్టు_RELEASE_ISR_తాళం()
    #లేకపోతే
        #దోషం పోర్టు_RELEASE_ISR_తాళం is required లోపల SMP
    #అయితే_ముగింపు

#అయితే_ముగింపు /* portRELEASE_ISR_LOCK */

#నిర్వచితం_కాకపోతే పోర్టు_GET_ISR_తాళం

    #ఒకవేళ ( config_సంఖ్య_యొక్క_CORES == 1 )
        #నిర్వచించు పోర్టు_GET_ISR_తాళం()
    #లేకపోతే
        #దోషం పోర్టు_GET_ISR_తాళం is required లోపల SMP
    #అయితే_ముగింపు

#అయితే_ముగింపు /* portGET_ISR_LOCK */

#నిర్వచితం_కాకపోతే పోర్టు_ENTER_CRITICAL_FROM_ISR

    #ఒకవేళ ( config_సంఖ్య_యొక్క_CORES > 1 )
        #దోషం పోర్టు_ENTER_CRITICAL_FROM_ISR is required లోపల SMP
    #అయితే_ముగింపు

#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే పోర్టు_EXIT_CRITICAL_FROM_ISR

    #ఒకవేళ ( config_సంఖ్య_యొక్క_CORES > 1 )
        #దోషం పోర్టు_EXIT_CRITICAL_FROM_ISR is required లోపల SMP
    #అయితే_ముగింపు

#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే configUSE_CORE_AFFINITY
    #నిర్వచించు configUSE_CORE_AFFINITY    0
#అయితే_ముగింపు /* configUSE_CORE_AFFINITY */

#ఒకవేళ ( ( config_సంఖ్య_యొక్క_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
    #నిర్వచితం_కాకపోతే configTASK_DEFAULT_CORE_AFFINITY
        #నిర్వచించు configTASK_DEFAULT_CORE_AFFINITY    tskNO_AFFINITY
    #అయితే_ముగింపు
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే configUSE_PASSIVE_IDLE_HOOK
    #నిర్వచించు configUSE_PASSIVE_IDLE_HOOK    0
#అయితే_ముగింపు /* configUSE_PASSIVE_IDLE_HOOK */

/* The timers module relies on xTaskGetSchedulerState(). */
#ఒకవేళ configUSE_TIMERS == 1

    #నిర్వచితం_కాకపోతే config_TIMER_TASK_ప్రాధాన్యత
        #దోషం If configUSE_TIMERS is సమితి గమ్యం 1 then config_TIMER_TASK_ప్రాధాన్యత must also be defined.
    #అయితే_ముగింపు /* configTIMER_TASK_PRIORITY */

    #నిర్వచితం_కాకపోతే config_TIMER_వరుస_నిడివి
        #దోషం If configUSE_TIMERS is సమితి గమ్యం 1 then config_TIMER_వరుస_నిడివి must also be defined.
    #అయితే_ముగింపు /* configTIMER_QUEUE_LENGTH */

    #నిర్వచితం_కాకపోతే config_TIMER_TASK_దొంతర_లోతు
        #దోషం If configUSE_TIMERS is సమితి గమ్యం 1 then config_TIMER_TASK_దొంతర_లోతు must also be defined.
    #అయితే_ముగింపు /* configTIMER_TASK_STACK_DEPTH */

    #నిర్వచితం_కాకపోతే పోర్టు_TIMER_CALLBACK_ATTRIBUTE
        #నిర్వచించు పోర్టు_TIMER_CALLBACK_ATTRIBUTE
    #అయితే_ముగింపు /* portTIMER_CALLBACK_ATTRIBUTE */

#అయితే_ముగింపు /* configUSE_TIMERS */

#నిర్వచితం_కాకపోతే పోర్టు_HAS_NESTED_INTERRUPTS
    #ఒకవేళ defined( పోర్టు_సమితి_అంతరాయం_ముసుగు_FROM_ISR ) && defined( పోర్టు_CLEAR_అంతరాయం_ముసుగు_FROM_ISR )
        #నిర్వచించు పోర్టు_HAS_NESTED_INTERRUPTS    1
    #లేకపోతే
        #నిర్వచించు పోర్టు_HAS_NESTED_INTERRUPTS    0
    #అయితే_ముగింపు
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే పోర్టు_సమితి_అంతరాయం_ముసుగు_FROM_ISR
    #ఒకవేళ ( పోర్టు_HAS_NESTED_INTERRUPTS == 1 )
        #దోషం పోర్టు_సమితి_అంతరాయం_ముసుగు_FROM_ISR must be defined కొరకు ports that support nested interrupts (i.e. పోర్టు_HAS_NESTED_INTERRUPTS is సమితి గమ్యం 1)
    #లేకపోతే
        #నిర్వచించు పోర్టు_సమితి_అంతరాయం_ముసుగు_FROM_ISR()    0
    #అయితే_ముగింపు
#లేకపోతే
    #ఒకవేళ ( పోర్టు_HAS_NESTED_INTERRUPTS == 0 )
        #దోషం పోర్టు_సమితి_అంతరాయం_ముసుగు_FROM_ISR must not be defined కొరకు ports that చేయి not support nested interrupts (i.e. పోర్టు_HAS_NESTED_INTERRUPTS is సమితి గమ్యం 0)
    #అయితే_ముగింపు
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే పోర్టు_CLEAR_అంతరాయం_ముసుగు_FROM_ISR
    #ఒకవేళ ( పోర్టు_HAS_NESTED_INTERRUPTS == 1 )
        #దోషం పోర్టు_CLEAR_అంతరాయం_ముసుగు_FROM_ISR must be defined కొరకు ports that support nested interrupts  (i.e. పోర్టు_HAS_NESTED_INTERRUPTS is సమితి గమ్యం 1)
    #లేకపోతే
        #నిర్వచించు పోర్టు_CLEAR_అంతరాయం_ముసుగు_FROM_ISR( ux_Saved_స్థితి_విలువ )    ( శూన్య ) ( ux_Saved_స్థితి_విలువ )
    #అయితే_ముగింపు
#లేకపోతే
    #ఒకవేళ ( పోర్టు_HAS_NESTED_INTERRUPTS == 0 )
        #దోషం పోర్టు_CLEAR_అంతరాయం_ముసుగు_FROM_ISR must not be defined కొరకు ports that చేయి not support nested interrupts (i.e. పోర్టు_HAS_NESTED_INTERRUPTS is సమితి గమ్యం 0)
    #అయితే_ముగింపు
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే పోర్టు_CLEAN_పైకి_TCB
    #నిర్వచించు పోర్టు_CLEAN_పైకి_TCB( pxTCB )    ( శూన్య ) ( pxTCB )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే పోర్టు_PRE_TASK_DELETE_HOOK
    #నిర్వచించు పోర్టు_PRE_TASK_DELETE_HOOK( pv_Task_గమ్యం_Delete, pxYieldPending )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే పోర్టు_అమరిక_TCB
    #నిర్వచించు పోర్టు_అమరిక_TCB( pxTCB )    ( శూన్య ) ( pxTCB )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే పోర్టు_TASK_SWITCH_HOOK
    #నిర్వచించు పోర్టు_TASK_SWITCH_HOOK( pxTCB )    ( శూన్య ) ( pxTCB )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే config_వరుస_REGISTRY_కొలత
    #నిర్వచించు config_వరుస_REGISTRY_కొలత    0U
#అయితే_ముగింపు

#ఒకవేళ ( config_వరుస_REGISTRY_కొలత < 1 )
    #నిర్వచించు v_వరుస_కూడు_గమ్యం_Registry( x_వరుస, pcName )
    #నిర్వచించు v_వరుస_Unregister_వరుస( x_వరుస )
    #నిర్వచించు pc_వరుస_Get_Name( x_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే config_USE_MINI_LIST_అంశం
    #నిర్వచించు config_USE_MINI_LIST_అంశం    1
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే పోర్టు_సూచిక_కొలత_TYPE
    #నిర్వచించు పోర్టు_సూచిక_కొలత_TYPE    uint32_t
#అయితే_ముగింపు

/* Remove any unused trace macros. */
#నిర్వచితం_కాకపోతే trace_ప్రారంభం

/* Used to perform any necessary initialisation - for example, open a file
 * into which trace is to be written. */
    #నిర్వచించు trace_ప్రారంభం()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_చివర

/* Use to close a trace, for example close a file into which trace has been
 * written. */
    #నిర్వచించు trace_చివర()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_TASK_SWITCHED_లోపల

/* Called after a task has been selected to run.  pxCurrentTCB holds a pointer
 * to the task control block of the selected task. */
    #నిర్వచించు trace_TASK_SWITCHED_లోపల()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_INCREASE_TICK_లెక్క

/* Called before stepping the tick count after waking from tickless idle
 * sleep. */
    #నిర్వచించు trace_INCREASE_TICK_లెక్క( x )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_దిగువ_POWER_IDLE_BEGIN
    /* Called immediately before entering tickless idle. */
    #నిర్వచించు trace_దిగువ_POWER_IDLE_BEGIN()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_దిగువ_POWER_IDLE_చివర
    /* Called when returning to the Idle task after a tickless idle. */
    #నిర్వచించు trace_దిగువ_POWER_IDLE_చివర()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_TASK_SWITCHED_బహిర్గతం

/* Called before a task has been selected to run.  pxCurrentTCB holds a pointer
 * to the task control block of the task being switched out. */
    #నిర్వచించు trace_TASK_SWITCHED_బహిర్గతం()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_TASK_ప్రాధాన్యత_INHERIT

/* Called when a task attempts to take a mutex that is already held by a
 * lower priority task.  pxTCBOfMutexHolder is a pointer to the TCB of the task
 * that holds the mutex.  uxInheritedPriority is the priority the mutex holder
 * will inherit (the priority of the task that is attempting to obtain the
 * muted. */
    #నిర్వచించు trace_TASK_ప్రాధాన్యత_INHERIT( pxTCBOfMutexHolder, ux_Inherited_ప్రాధాన్యత )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_TASK_ప్రాధాన్యత_DISINHERIT

/* Called when a task releases a mutex, the holding of which had resulted in
 * the task inheriting the priority of a higher priority task.
 * pxTCBOfMutexHolder is a pointer to the TCB of the task that is releasing the
 * mutex.  uxOriginalPriority is the task's configured (base) priority. */
    #నిర్వచించు trace_TASK_ప్రాధాన్యత_DISINHERIT( pxTCBOfMutexHolder, ux_Original_ప్రాధాన్యత )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_BLOCKING_ON_వరుస_RECEIVE

/* Task is about to block because it cannot read from a
 * queue/mutex/semaphore.  pxQueue is a pointer to the queue/mutex/semaphore
 * upon which the read was attempted.  pxCurrentTCB points to the TCB of the
 * task that attempted the read. */
    #నిర్వచించు trace_BLOCKING_ON_వరుస_RECEIVE( px_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_BLOCKING_ON_వరుస_PEEK

/* Task is about to block because it cannot read from a
 * queue/mutex/semaphore.  pxQueue is a pointer to the queue/mutex/semaphore
 * upon which the read was attempted.  pxCurrentTCB points to the TCB of the
 * task that attempted the read. */
    #నిర్వచించు trace_BLOCKING_ON_వరుస_PEEK( px_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_BLOCKING_ON_వరుస_SEND

/* Task is about to block because it cannot write to a
 * queue/mutex/semaphore.  pxQueue is a pointer to the queue/mutex/semaphore
 * upon which the write was attempted.  pxCurrentTCB points to the TCB of the
 * task that attempted the write. */
    #నిర్వచించు trace_BLOCKING_ON_వరుస_SEND( px_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే config_CHECK_FOR_దొంతర_అధికప్రవాహం
    #నిర్వచించు config_CHECK_FOR_దొంతర_అధికప్రవాహం    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే config_రికార్డు_దొంతర_ఎగువ_చిరునామా
    #నిర్వచించు config_రికార్డు_దొంతర_ఎగువ_చిరునామా    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H
    #నిర్వచించు configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H    0
#అయితే_ముగింపు

/* The following event macros are embedded in the kernel API calls. */

#నిర్వచితం_కాకపోతే trace_MOVED_TASK_గమ్యం_READY_STATE
    #నిర్వచించు trace_MOVED_TASK_గమ్యం_READY_STATE( pxTCB )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_POST_MOVED_TASK_గమ్యం_READY_STATE
    #నిర్వచించు trace_POST_MOVED_TASK_గమ్యం_READY_STATE( pxTCB )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_MOVED_TASK_గమ్యం_DELAYED_LIST
    #నిర్వచించు trace_MOVED_TASK_గమ్యం_DELAYED_LIST()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_MOVED_TASK_గమ్యం_అధికప్రవాహం_DELAYED_LIST
    #నిర్వచించు trace_MOVED_TASK_గమ్యం_అధికప్రవాహం_DELAYED_LIST()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_వరుస_CREATE
    #నిర్వచించు trace_వరుస_CREATE( px_New_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_వరుస_CREATE_FAILED
    #నిర్వచించు trace_వరుస_CREATE_FAILED( uc_వరుస_Type )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceCREATE_MUTEX
    #నిర్వచించు traceCREATE_MUTEX( px_New_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceCREATE_MUTEX_FAILED
    #నిర్వచించు traceCREATE_MUTEX_FAILED()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceGIVE_MUTEX_RECURSIVE
    #నిర్వచించు traceGIVE_MUTEX_RECURSIVE( pxMutex )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceGIVE_MUTEX_RECURSIVE_FAILED
    #నిర్వచించు traceGIVE_MUTEX_RECURSIVE_FAILED( pxMutex )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceTAKE_MUTEX_RECURSIVE
    #నిర్వచించు traceTAKE_MUTEX_RECURSIVE( pxMutex )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceTAKE_MUTEX_RECURSIVE_FAILED
    #నిర్వచించు traceTAKE_MUTEX_RECURSIVE_FAILED( pxMutex )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceCREATE_COUNTING_SEMAPHORE
    #నిర్వచించు traceCREATE_COUNTING_SEMAPHORE()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceCREATE_COUNTING_SEMAPHORE_FAILED
    #నిర్వచించు traceCREATE_COUNTING_SEMAPHORE_FAILED()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_వరుస_సమితి_SEND
    #నిర్వచించు trace_వరుస_సమితి_SEND    trace_వరుస_SEND
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_వరుస_SEND
    #నిర్వచించు trace_వరుస_SEND( px_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_వరుస_SEND_FAILED
    #నిర్వచించు trace_వరుస_SEND_FAILED( px_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_వరుస_RECEIVE
    #నిర్వచించు trace_వరుస_RECEIVE( px_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_వరుస_PEEK
    #నిర్వచించు trace_వరుస_PEEK( px_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_వరుస_PEEK_FAILED
    #నిర్వచించు trace_వరుస_PEEK_FAILED( px_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_వరుస_PEEK_FROM_ISR
    #నిర్వచించు trace_వరుస_PEEK_FROM_ISR( px_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_వరుస_RECEIVE_FAILED
    #నిర్వచించు trace_వరుస_RECEIVE_FAILED( px_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_వరుస_SEND_FROM_ISR
    #నిర్వచించు trace_వరుస_SEND_FROM_ISR( px_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_వరుస_SEND_FROM_ISR_FAILED
    #నిర్వచించు trace_వరుస_SEND_FROM_ISR_FAILED( px_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_వరుస_RECEIVE_FROM_ISR
    #నిర్వచించు trace_వరుస_RECEIVE_FROM_ISR( px_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_వరుస_RECEIVE_FROM_ISR_FAILED
    #నిర్వచించు trace_వరుస_RECEIVE_FROM_ISR_FAILED( px_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_వరుస_PEEK_FROM_ISR_FAILED
    #నిర్వచించు trace_వరుస_PEEK_FROM_ISR_FAILED( px_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_వరుస_DELETE
    #నిర్వచించు trace_వరుస_DELETE( px_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceTASK_CREATE
    #నిర్వచించు traceTASK_CREATE( pxNewTCB )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceTASK_CREATE_FAILED
    #నిర్వచించు traceTASK_CREATE_FAILED()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceTASK_DELETE
    #నిర్వచించు traceTASK_DELETE( px_Task_గమ్యం_Delete )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceTASK_DELAY_UNTIL
    #నిర్వచించు traceTASK_DELAY_UNTIL( x )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceTASK_DELAY
    #నిర్వచించు traceTASK_DELAY()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_TASK_ప్రాధాన్యత_సమితి
    #నిర్వచించు trace_TASK_ప్రాధాన్యత_సమితి( pxTask, ux_New_ప్రాధాన్యత )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceTASK_SUSPEND
    #నిర్వచించు traceTASK_SUSPEND( px_Task_గమ్యం_Suspend )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceTASK_RESUME
    #నిర్వచించు traceTASK_RESUME( px_Task_గమ్యం_Resume )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceTASK_RESUME_FROM_ISR
    #నిర్వచించు traceTASK_RESUME_FROM_ISR( px_Task_గమ్యం_Resume )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceTASK_INCREMENT_TICK
    #నిర్వచించు traceTASK_INCREMENT_TICK( x_Tick_లెక్క )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceTIMER_CREATE
    #నిర్వచించు traceTIMER_CREATE( pxNewTimer )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceTIMER_CREATE_FAILED
    #నిర్వచించు traceTIMER_CREATE_FAILED()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceTIMER_COMMAND_SEND
    #నిర్వచించు traceTIMER_COMMAND_SEND( xTimer, x_సందేశం_ID, x_సందేశం_విలువ_విలువ, xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceTIMER_EXPIRED
    #నిర్వచించు traceTIMER_EXPIRED( pxTimer )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceTIMER_COMMAND_RECEIVED
    #నిర్వచించు traceTIMER_COMMAND_RECEIVED( pxTimer, x_సందేశం_ID, x_సందేశం_విలువ )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceMALLOC
    #నిర్వచించు traceMALLOC( pv_చిరునామా, ui_కొలత )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceFREE
    #నిర్వచించు traceFREE( pv_చిరునామా, ui_కొలత )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_EVENT_సమూహం_CREATE
    #నిర్వచించు trace_EVENT_సమూహం_CREATE( x_Event_సమూహం )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_EVENT_సమూహం_CREATE_FAILED
    #నిర్వచించు trace_EVENT_సమూహం_CREATE_FAILED()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_EVENT_సమూహం_SYNC_ఖండం
    #నిర్వచించు trace_EVENT_సమూహం_SYNC_ఖండం( x_Event_సమూహం, ux_బిట్లు_గమ్యం_సమితి, ux_బిట్లు_గమ్యం_Wait_For )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_EVENT_సమూహం_SYNC_చివర
    #నిర్వచించు trace_EVENT_సమూహం_SYNC_చివర( x_Event_సమూహం, ux_బిట్లు_గమ్యం_సమితి, ux_బిట్లు_గమ్యం_Wait_For, xTimeoutOccurred )    ( శూన్య ) ( xTimeoutOccurred )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_EVENT_సమూహం_WAIT_బిట్లు_ఖండం
    #నిర్వచించు trace_EVENT_సమూహం_WAIT_బిట్లు_ఖండం( x_Event_సమూహం, ux_బిట్లు_గమ్యం_Wait_For )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_EVENT_సమూహం_WAIT_బిట్లు_చివర
    #నిర్వచించు trace_EVENT_సమూహం_WAIT_బిట్లు_చివర( x_Event_సమూహం, ux_బిట్లు_గమ్యం_Wait_For, xTimeoutOccurred )    ( శూన్య ) ( xTimeoutOccurred )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_EVENT_సమూహం_CLEAR_బిట్లు
    #నిర్వచించు trace_EVENT_సమూహం_CLEAR_బిట్లు( x_Event_సమూహం, ux_బిట్లు_గమ్యం_Clear )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_EVENT_సమూహం_CLEAR_బిట్లు_FROM_ISR
    #నిర్వచించు trace_EVENT_సమూహం_CLEAR_బిట్లు_FROM_ISR( x_Event_సమూహం, ux_బిట్లు_గమ్యం_Clear )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_EVENT_సమూహం_సమితి_బిట్లు
    #నిర్వచించు trace_EVENT_సమూహం_సమితి_బిట్లు( x_Event_సమూహం, ux_బిట్లు_గమ్యం_సమితి )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_EVENT_సమూహం_సమితి_బిట్లు_FROM_ISR
    #నిర్వచించు trace_EVENT_సమూహం_సమితి_బిట్లు_FROM_ISR( x_Event_సమూహం, ux_బిట్లు_గమ్యం_సమితి )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_EVENT_సమూహం_DELETE
    #నిర్వచించు trace_EVENT_సమూహం_DELETE( x_Event_సమూహం )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే tracePEND_FUNC_CALL
    #నిర్వచించు tracePEND_FUNC_CALL( x_Function_గమ్యం_Pend, pvParameter1, ulParameter2, ret )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే tracePEND_FUNC_CALL_FROM_ISR
    #నిర్వచించు tracePEND_FUNC_CALL_FROM_ISR( x_Function_గమ్యం_Pend, pvParameter1, ulParameter2, ret )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_వరుస_REGISTRY_కూడు
    #నిర్వచించు trace_వరుస_REGISTRY_కూడు( x_వరుస, pc_వరుస_Name )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_TASK_NOTIFY_TAKE_ఖండం
    #నిర్వచించు trace_TASK_NOTIFY_TAKE_ఖండం( ux_సూచీ_గమ్యం_Wait )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceTASK_NOTIFY_TAKE
    #నిర్వచించు traceTASK_NOTIFY_TAKE( ux_సూచీ_గమ్యం_Wait )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_TASK_NOTIFY_WAIT_ఖండం
    #నిర్వచించు trace_TASK_NOTIFY_WAIT_ఖండం( ux_సూచీ_గమ్యం_Wait )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceTASK_NOTIFY_WAIT
    #నిర్వచించు traceTASK_NOTIFY_WAIT( ux_సూచీ_గమ్యం_Wait )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceTASK_NOTIFY
    #నిర్వచించు traceTASK_NOTIFY( ux_సూచీ_గమ్యం_Notify )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceTASK_NOTIFY_FROM_ISR
    #నిర్వచించు traceTASK_NOTIFY_FROM_ISR( ux_సూచీ_గమ్యం_Notify )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceTASK_NOTIFY_GIVE_FROM_ISR
    #నిర్వచించు traceTASK_NOTIFY_GIVE_FROM_ISR( ux_సూచీ_గమ్యం_Notify )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ISR_EXIT_గమ్యం_SCHEDULER
    #నిర్వచించు trace_ISR_EXIT_గమ్యం_SCHEDULER()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceISR_EXIT
    #నిర్వచించు traceISR_EXIT()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceISR_ENTER
    #నిర్వచించు traceISR_ENTER()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ప్రవాహం_బఫరు_CREATE_FAILED
    #నిర్వచించు trace_ప్రవాహం_బఫరు_CREATE_FAILED( x_ప్రవాహం_బఫరు_Type )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ప్రవాహం_బఫరు_CREATE_STATIC_FAILED
    #నిర్వచించు trace_ప్రవాహం_బఫరు_CREATE_STATIC_FAILED( xReturn, x_ప్రవాహం_బఫరు_Type )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ప్రవాహం_బఫరు_CREATE
    #నిర్వచించు trace_ప్రవాహం_బఫరు_CREATE( px_ప్రవాహం_బఫరు, x_ప్రవాహం_బఫరు_Type )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ప్రవాహం_బఫరు_DELETE
    #నిర్వచించు trace_ప్రవాహం_బఫరు_DELETE( x_ప్రవాహం_బఫరు )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ప్రవాహం_బఫరు_RESET
    #నిర్వచించు trace_ప్రవాహం_బఫరు_RESET( x_ప్రవాహం_బఫరు )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ప్రవాహం_బఫరు_RESET_FROM_ISR
    #నిర్వచించు trace_ప్రవాహం_బఫరు_RESET_FROM_ISR( x_ప్రవాహం_బఫరు )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_BLOCKING_ON_ప్రవాహం_బఫరు_SEND
    #నిర్వచించు trace_BLOCKING_ON_ప్రవాహం_బఫరు_SEND( x_ప్రవాహం_బఫరు )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ప్రవాహం_బఫరు_SEND
    #నిర్వచించు trace_ప్రవాహం_బఫరు_SEND( x_ప్రవాహం_బఫరు, x_బైట్లు_పంపిన )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ప్రవాహం_బఫరు_SEND_FAILED
    #నిర్వచించు trace_ప్రవాహం_బఫరు_SEND_FAILED( x_ప్రవాహం_బఫరు )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ప్రవాహం_బఫరు_SEND_FROM_ISR
    #నిర్వచించు trace_ప్రవాహం_బఫరు_SEND_FROM_ISR( x_ప్రవాహం_బఫరు, x_బైట్లు_పంపిన )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_BLOCKING_ON_ప్రవాహం_బఫరు_RECEIVE
    #నిర్వచించు trace_BLOCKING_ON_ప్రవాహం_బఫరు_RECEIVE( x_ప్రవాహం_బఫరు )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ప్రవాహం_బఫరు_RECEIVE
    #నిర్వచించు trace_ప్రవాహం_బఫరు_RECEIVE( x_ప్రవాహం_బఫరు, x_Received_నిడివి )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ప్రవాహం_బఫరు_RECEIVE_FAILED
    #నిర్వచించు trace_ప్రవాహం_బఫరు_RECEIVE_FAILED( x_ప్రవాహం_బఫరు )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ప్రవాహం_బఫరు_RECEIVE_FROM_ISR
    #నిర్వచించు trace_ప్రవాహం_బఫరు_RECEIVE_FROM_ISR( x_ప్రవాహం_బఫరు, x_Received_నిడివి )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_Event_సమూహం_Create_Static
    #నిర్వచించు trace_ENTER_x_Event_సమూహం_Create_Static( px_Event_సమూహం_బఫరు )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_Event_సమూహం_Create_Static
    #నిర్వచించు trace_RETURN_x_Event_సమూహం_Create_Static( px_Event_బిట్లు )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_Event_సమూహం_Create
    #నిర్వచించు trace_ENTER_x_Event_సమూహం_Create()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_Event_సమూహం_Create
    #నిర్వచించు trace_RETURN_x_Event_సమూహం_Create( px_Event_బిట్లు )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_Event_సమూహం_Sync
    #నిర్వచించు trace_ENTER_x_Event_సమూహం_Sync( x_Event_సమూహం, ux_బిట్లు_గమ్యం_సమితి, ux_బిట్లు_గమ్యం_Wait_For, x_Ticks_గమ్యం_Wait )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_Event_సమూహం_Sync
    #నిర్వచించు trace_RETURN_x_Event_సమూహం_Sync( uxReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_Event_సమూహం_Wait_బిట్లు
    #నిర్వచించు trace_ENTER_x_Event_సమూహం_Wait_బిట్లు( x_Event_సమూహం, ux_బిట్లు_గమ్యం_Wait_For, xClearOnExit, x_Wait_For_All_బిట్లు, x_Ticks_గమ్యం_Wait )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_Event_సమూహం_Wait_బిట్లు
    #నిర్వచించు trace_RETURN_x_Event_సమూహం_Wait_బిట్లు( uxReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_Event_సమూహం_Clear_బిట్లు
    #నిర్వచించు trace_ENTER_x_Event_సమూహం_Clear_బిట్లు( x_Event_సమూహం, ux_బిట్లు_గమ్యం_Clear )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_Event_సమూహం_Clear_బిట్లు
    #నిర్వచించు trace_RETURN_x_Event_సమూహం_Clear_బిట్లు( uxReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_Event_సమూహం_Clear_బిట్లు_From_ISR
    #నిర్వచించు trace_ENTER_x_Event_సమూహం_Clear_బిట్లు_From_ISR( x_Event_సమూహం, ux_బిట్లు_గమ్యం_Clear )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_Event_సమూహం_Clear_బిట్లు_From_ISR
    #నిర్వచించు trace_RETURN_x_Event_సమూహం_Clear_బిట్లు_From_ISR( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_Event_సమూహం_Get_బిట్లు_From_ISR
    #నిర్వచించు trace_ENTER_x_Event_సమూహం_Get_బిట్లు_From_ISR( x_Event_సమూహం )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_Event_సమూహం_Get_బిట్లు_From_ISR
    #నిర్వచించు trace_RETURN_x_Event_సమూహం_Get_బిట్లు_From_ISR( uxReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_Event_సమూహం_సమితి_బిట్లు
    #నిర్వచించు trace_ENTER_x_Event_సమూహం_సమితి_బిట్లు( x_Event_సమూహం, ux_బిట్లు_గమ్యం_సమితి )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_Event_సమూహం_సమితి_బిట్లు
    #నిర్వచించు trace_RETURN_x_Event_సమూహం_సమితి_బిట్లు( ux_Event_బిట్లు )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_v_Event_సమూహం_Delete
    #నిర్వచించు trace_ENTER_v_Event_సమూహం_Delete( x_Event_సమూహం )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_v_Event_సమూహం_Delete
    #నిర్వచించు trace_RETURN_v_Event_సమూహం_Delete()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_Event_సమూహం_Get_Static_బఫరు
    #నిర్వచించు trace_ENTER_x_Event_సమూహం_Get_Static_బఫరు( x_Event_సమూహం, ppx_Event_సమూహం_బఫరు )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_Event_సమూహం_Get_Static_బఫరు
    #నిర్వచించు trace_RETURN_x_Event_సమూహం_Get_Static_బఫరు( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_v_Event_సమూహం_సమితి_బిట్లు_Callback
    #నిర్వచించు trace_ENTER_v_Event_సమూహం_సమితి_బిట్లు_Callback( pv_Event_సమూహం, ul_బిట్లు_గమ్యం_సమితి )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_v_Event_సమూహం_సమితి_బిట్లు_Callback
    #నిర్వచించు trace_RETURN_v_Event_సమూహం_సమితి_బిట్లు_Callback()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_v_Event_సమూహం_Clear_బిట్లు_Callback
    #నిర్వచించు trace_ENTER_v_Event_సమూహం_Clear_బిట్లు_Callback( pv_Event_సమూహం, ul_బిట్లు_గమ్యం_Clear )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_v_Event_సమూహం_Clear_బిట్లు_Callback
    #నిర్వచించు trace_RETURN_v_Event_సమూహం_Clear_బిట్లు_Callback()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_Event_సమూహం_సమితి_బిట్లు_From_ISR
    #నిర్వచించు trace_ENTER_x_Event_సమూహం_సమితి_బిట్లు_From_ISR( x_Event_సమూహం, ux_బిట్లు_గమ్యం_సమితి, px_Higher_ప్రాధాన్యత_Task_Woken )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_Event_సమూహం_సమితి_బిట్లు_From_ISR
    #నిర్వచించు trace_RETURN_x_Event_సమూహం_సమితి_బిట్లు_From_ISR( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_ux_Event_సమూహం_Get_సంఖ్య
    #నిర్వచించు trace_ENTER_ux_Event_సమూహం_Get_సంఖ్య( x_Event_సమూహం )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_ux_Event_సమూహం_Get_సంఖ్య
    #నిర్వచించు trace_RETURN_ux_Event_సమూహం_Get_సంఖ్య( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_v_Event_సమూహం_సమితి_సంఖ్య
    #నిర్వచించు trace_ENTER_v_Event_సమూహం_సమితి_సంఖ్య( x_Event_సమూహం, ux_Event_సమూహం_సంఖ్య )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_v_Event_సమూహం_సమితి_సంఖ్య
    #నిర్వచించు trace_RETURN_v_Event_సమూహం_సమితి_సంఖ్య()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_వరుస_Generic_Reset
    #నిర్వచించు trace_ENTER_x_వరుస_Generic_Reset( x_వరుస, x_New_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_వరుస_Generic_Reset
    #నిర్వచించు trace_RETURN_x_వరుస_Generic_Reset( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_వరుస_Generic_Create_Static
    #నిర్వచించు trace_ENTER_x_వరుస_Generic_Create_Static( ux_వరుస_నిడివి, ux_అంశం_కొలత, puc_వరుస_Storage, px_Static_వరుస, uc_వరుస_Type )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_వరుస_Generic_Create_Static
    #నిర్వచించు trace_RETURN_x_వరుస_Generic_Create_Static( px_New_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_వరుస_Generic_Get_Static_Buffers
    #నిర్వచించు trace_ENTER_x_వరుస_Generic_Get_Static_Buffers( x_వరుస, ppuc_వరుస_Storage, ppx_Static_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_వరుస_Generic_Get_Static_Buffers
    #నిర్వచించు trace_RETURN_x_వరుస_Generic_Get_Static_Buffers( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_వరుస_Generic_Create
    #నిర్వచించు trace_ENTER_x_వరుస_Generic_Create( ux_వరుస_నిడివి, ux_అంశం_కొలత, uc_వరుస_Type )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_వరుస_Generic_Create
    #నిర్వచించు trace_RETURN_x_వరుస_Generic_Create( px_New_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_వరుస_Create_Mutex
    #నిర్వచించు trace_ENTER_x_వరుస_Create_Mutex( uc_వరుస_Type )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_వరుస_Create_Mutex
    #నిర్వచించు trace_RETURN_x_వరుస_Create_Mutex( x_New_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_వరుస_Create_Mutex_Static
    #నిర్వచించు trace_ENTER_x_వరుస_Create_Mutex_Static( uc_వరుస_Type, px_Static_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_వరుస_Create_Mutex_Static
    #నిర్వచించు trace_RETURN_x_వరుస_Create_Mutex_Static( x_New_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_వరుస_Get_Mutex_Holder
    #నిర్వచించు trace_ENTER_x_వరుస_Get_Mutex_Holder( xSemaphore )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_వరుస_Get_Mutex_Holder
    #నిర్వచించు trace_RETURN_x_వరుస_Get_Mutex_Holder( pxReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_వరుస_Get_Mutex_Holder_From_ISR
    #నిర్వచించు trace_ENTER_x_వరుస_Get_Mutex_Holder_From_ISR( xSemaphore )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_వరుస_Get_Mutex_Holder_From_ISR
    #నిర్వచించు trace_RETURN_x_వరుస_Get_Mutex_Holder_From_ISR( pxReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_వరుస_Give_Mutex_Recursive
    #నిర్వచించు trace_ENTER_x_వరుస_Give_Mutex_Recursive( xMutex )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_వరుస_Give_Mutex_Recursive
    #నిర్వచించు trace_RETURN_x_వరుస_Give_Mutex_Recursive( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_వరుస_Take_Mutex_Recursive
    #నిర్వచించు trace_ENTER_x_వరుస_Take_Mutex_Recursive( xMutex, x_Ticks_గమ్యం_Wait )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_వరుస_Take_Mutex_Recursive
    #నిర్వచించు trace_RETURN_x_వరుస_Take_Mutex_Recursive( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_వరుస_Create_Counting_Semaphore_Static
    #నిర్వచించు trace_ENTER_x_వరుస_Create_Counting_Semaphore_Static( ux_Max_లెక్క, ux_Initial_లెక్క, px_Static_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_వరుస_Create_Counting_Semaphore_Static
    #నిర్వచించు trace_RETURN_x_వరుస_Create_Counting_Semaphore_Static( xHandle )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_వరుస_Create_Counting_Semaphore
    #నిర్వచించు trace_ENTER_x_వరుస_Create_Counting_Semaphore( ux_Max_లెక్క, ux_Initial_లెక్క )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_వరుస_Create_Counting_Semaphore
    #నిర్వచించు trace_RETURN_x_వరుస_Create_Counting_Semaphore( xHandle )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_వరుస_Generic_Send
    #నిర్వచించు trace_ENTER_x_వరుస_Generic_Send( x_వరుస, pv_అంశం_గమ్యం_వరుస, x_Ticks_గమ్యం_Wait, xCopyPosition )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_వరుస_Generic_Send
    #నిర్వచించు trace_RETURN_x_వరుస_Generic_Send( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_వరుస_Generic_Send_From_ISR
    #నిర్వచించు trace_ENTER_x_వరుస_Generic_Send_From_ISR( x_వరుస, pv_అంశం_గమ్యం_వరుస, px_Higher_ప్రాధాన్యత_Task_Woken, xCopyPosition )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_వరుస_Generic_Send_From_ISR
    #నిర్వచించు trace_RETURN_x_వరుస_Generic_Send_From_ISR( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_వరుస_Give_From_ISR
    #నిర్వచించు trace_ENTER_x_వరుస_Give_From_ISR( x_వరుస, px_Higher_ప్రాధాన్యత_Task_Woken )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_వరుస_Give_From_ISR
    #నిర్వచించు trace_RETURN_x_వరుస_Give_From_ISR( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_వరుస_Receive
    #నిర్వచించు trace_ENTER_x_వరుస_Receive( x_వరుస, pv_బఫరు, x_Ticks_గమ్యం_Wait )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_వరుస_Receive
    #నిర్వచించు trace_RETURN_x_వరుస_Receive( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_వరుస_Semaphore_Take
    #నిర్వచించు trace_ENTER_x_వరుస_Semaphore_Take( x_వరుస, x_Ticks_గమ్యం_Wait )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_వరుస_Semaphore_Take
    #నిర్వచించు trace_RETURN_x_వరుస_Semaphore_Take( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_వరుస_Peek
    #నిర్వచించు trace_ENTER_x_వరుస_Peek( x_వరుస, pv_బఫరు, x_Ticks_గమ్యం_Wait )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_వరుస_Peek
    #నిర్వచించు trace_RETURN_x_వరుస_Peek( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_వరుస_Receive_From_ISR
    #నిర్వచించు trace_ENTER_x_వరుస_Receive_From_ISR( x_వరుస, pv_బఫరు, px_Higher_ప్రాధాన్యత_Task_Woken )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_వరుస_Receive_From_ISR
    #నిర్వచించు trace_RETURN_x_వరుస_Receive_From_ISR( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_వరుస_Peek_From_ISR
    #నిర్వచించు trace_ENTER_x_వరుస_Peek_From_ISR( x_వరుస, pv_బఫరు )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_వరుస_Peek_From_ISR
    #నిర్వచించు trace_RETURN_x_వరుస_Peek_From_ISR( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_ux_వరుస_Messages_నిరీక్షణ
    #నిర్వచించు trace_ENTER_ux_వరుస_Messages_నిరీక్షణ( x_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_ux_వరుస_Messages_నిరీక్షణ
    #నిర్వచించు trace_RETURN_ux_వరుస_Messages_నిరీక్షణ( uxReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_ux_వరుస_Spaces_అందుబాటు
    #నిర్వచించు trace_ENTER_ux_వరుస_Spaces_అందుబాటు( x_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_ux_వరుస_Spaces_అందుబాటు
    #నిర్వచించు trace_RETURN_ux_వరుస_Spaces_అందుబాటు( uxReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_ux_వరుస_Messages_నిరీక్షణ_From_ISR
    #నిర్వచించు trace_ENTER_ux_వరుస_Messages_నిరీక్షణ_From_ISR( x_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_ux_వరుస_Messages_నిరీక్షణ_From_ISR
    #నిర్వచించు trace_RETURN_ux_వరుస_Messages_నిరీక్షణ_From_ISR( uxReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_v_వరుస_Delete
    #నిర్వచించు trace_ENTER_v_వరుస_Delete( x_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_v_వరుస_Delete
    #నిర్వచించు trace_RETURN_v_వరుస_Delete()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_ux_వరుస_Get_వరుస_సంఖ్య
    #నిర్వచించు trace_ENTER_ux_వరుస_Get_వరుస_సంఖ్య( x_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_ux_వరుస_Get_వరుస_సంఖ్య
    #నిర్వచించు trace_RETURN_ux_వరుస_Get_వరుస_సంఖ్య( ux_వరుస_సంఖ్య )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_v_వరుస_సమితి_వరుస_సంఖ్య
    #నిర్వచించు trace_ENTER_v_వరుస_సమితి_వరుస_సంఖ్య( x_వరుస, ux_వరుస_సంఖ్య )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_v_వరుస_సమితి_వరుస_సంఖ్య
    #నిర్వచించు trace_RETURN_v_వరుస_సమితి_వరుస_సంఖ్య()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_uc_వరుస_Get_వరుస_Type
    #నిర్వచించు trace_ENTER_uc_వరుస_Get_వరుస_Type( x_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_uc_వరుస_Get_వరుస_Type
    #నిర్వచించు trace_RETURN_uc_వరుస_Get_వరుస_Type( uc_వరుస_Type )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_ux_వరుస_Get_వరుస_అంశం_కొలత
    #నిర్వచించు trace_ENTER_ux_వరుస_Get_వరుస_అంశం_కొలత( x_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_ux_వరుస_Get_వరుస_అంశం_కొలత
    #నిర్వచించు trace_RETURN_ux_వరుస_Get_వరుస_అంశం_కొలత( ux_అంశం_కొలత )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_ux_వరుస_Get_వరుస_నిడివి
    #నిర్వచించు trace_ENTER_ux_వరుస_Get_వరుస_నిడివి( x_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_ux_వరుస_Get_వరుస_నిడివి
    #నిర్వచించు trace_RETURN_ux_వరుస_Get_వరుస_నిడివి( ux_నిడివి )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_వరుస_Is_వరుస_ఖాళీలు_From_ISR
    #నిర్వచించు trace_ENTER_x_వరుస_Is_వరుస_ఖాళీలు_From_ISR( x_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_వరుస_Is_వరుస_ఖాళీలు_From_ISR
    #నిర్వచించు trace_RETURN_x_వరుస_Is_వరుస_ఖాళీలు_From_ISR( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_వరుస_Is_వరుస_నిండిన_From_ISR
    #నిర్వచించు trace_ENTER_x_వరుస_Is_వరుస_నిండిన_From_ISR( x_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_వరుస_Is_వరుస_నిండిన_From_ISR
    #నిర్వచించు trace_RETURN_x_వరుస_Is_వరుస_నిండిన_From_ISR( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_వరుస_CRSend
    #నిర్వచించు trace_ENTER_x_వరుస_CRSend( x_వరుస, pv_అంశం_గమ్యం_వరుస, x_Ticks_గమ్యం_Wait )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_వరుస_CRSend
    #నిర్వచించు trace_RETURN_x_వరుస_CRSend( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_వరుస_CRReceive
    #నిర్వచించు trace_ENTER_x_వరుస_CRReceive( x_వరుస, pv_బఫరు, x_Ticks_గమ్యం_Wait )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_వరుస_CRReceive
    #నిర్వచించు trace_RETURN_x_వరుస_CRReceive( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_వరుస_CRSend_From_ISR
    #నిర్వచించు trace_ENTER_x_వరుస_CRSend_From_ISR( x_వరుస, pv_అంశం_గమ్యం_వరుస, xCoRoutinePreviouslyWoken )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_వరుస_CRSend_From_ISR
    #నిర్వచించు trace_RETURN_x_వరుస_CRSend_From_ISR( xCoRoutinePreviouslyWoken )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_వరుస_CRReceive_From_ISR
    #నిర్వచించు trace_ENTER_x_వరుస_CRReceive_From_ISR( x_వరుస, pv_బఫరు, pxCoRoutineWoken )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_వరుస_CRReceive_From_ISR
    #నిర్వచించు trace_RETURN_x_వరుస_CRReceive_From_ISR( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_v_వరుస_కూడు_గమ్యం_Registry
    #నిర్వచించు trace_ENTER_v_వరుస_కూడు_గమ్యం_Registry( x_వరుస, pc_వరుస_Name )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_v_వరుస_కూడు_గమ్యం_Registry
    #నిర్వచించు trace_RETURN_v_వరుస_కూడు_గమ్యం_Registry()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_pc_వరుస_Get_Name
    #నిర్వచించు trace_ENTER_pc_వరుస_Get_Name( x_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_pc_వరుస_Get_Name
    #నిర్వచించు trace_RETURN_pc_వరుస_Get_Name( pcReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_v_వరుస_Unregister_వరుస
    #నిర్వచించు trace_ENTER_v_వరుస_Unregister_వరుస( x_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_v_వరుస_Unregister_వరుస
    #నిర్వచించు trace_RETURN_v_వరుస_Unregister_వరుస()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_v_వరుస_Wait_For_సందేశం_Restricted
    #నిర్వచించు trace_ENTER_v_వరుస_Wait_For_సందేశం_Restricted( x_వరుస, x_Ticks_గమ్యం_Wait, xWaitIndefinitely )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_v_వరుస_Wait_For_సందేశం_Restricted
    #నిర్వచించు trace_RETURN_v_వరుస_Wait_For_సందేశం_Restricted()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_వరుస_Create_సమితి
    #నిర్వచించు trace_ENTER_x_వరుస_Create_సమితి( ux_Event_వరుస_నిడివి )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_వరుస_Create_సమితి
    #నిర్వచించు trace_RETURN_x_వరుస_Create_సమితి( px_వరుస )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_వరుస_కూడు_గమ్యం_సమితి
    #నిర్వచించు trace_ENTER_x_వరుస_కూడు_గమ్యం_సమితి( x_వరుస_Or_Semaphore, x_వరుస_సమితి )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_వరుస_కూడు_గమ్యం_సమితి
    #నిర్వచించు trace_RETURN_x_వరుస_కూడు_గమ్యం_సమితి( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_వరుస_Remove_From_సమితి
    #నిర్వచించు trace_ENTER_x_వరుస_Remove_From_సమితి( x_వరుస_Or_Semaphore, x_వరుస_సమితి )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_వరుస_Remove_From_సమితి
    #నిర్వచించు trace_RETURN_x_వరుస_Remove_From_సమితి( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_వరుస_Select_From_సమితి
    #నిర్వచించు trace_ENTER_x_వరుస_Select_From_సమితి( x_వరుస_సమితి, x_Ticks_గమ్యం_Wait )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_వరుస_Select_From_సమితి
    #నిర్వచించు trace_RETURN_x_వరుస_Select_From_సమితి( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_వరుస_Select_From_సమితి_From_ISR
    #నిర్వచించు trace_ENTER_x_వరుస_Select_From_సమితి_From_ISR( x_వరుస_సమితి )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_వరుస_Select_From_సమితి_From_ISR
    #నిర్వచించు trace_RETURN_x_వరుస_Select_From_సమితి_From_ISR( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_xTimerCreateTimerTask
    #నిర్వచించు traceENTER_xTimerCreateTimerTask()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_xTimerCreateTimerTask
    #నిర్వచించు traceRETURN_xTimerCreateTimerTask( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_xTimerCreate
    #నిర్వచించు traceENTER_xTimerCreate( pcTimerName, x_Timer_Period_లోపల_Ticks, xAutoReload, pvTimerID, pxCallbackFunction )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_xTimerCreate
    #నిర్వచించు traceRETURN_xTimerCreate( pxNewTimer )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_xTimerCreateStatic
    #నిర్వచించు traceENTER_xTimerCreateStatic( pcTimerName, x_Timer_Period_లోపల_Ticks, xAutoReload, pvTimerID, pxCallbackFunction, px_Timer_బఫరు )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_xTimerCreateStatic
    #నిర్వచించు traceRETURN_xTimerCreateStatic( pxNewTimer )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_xTimerGenericCommandFromTask
    #నిర్వచించు traceENTER_xTimerGenericCommandFromTask( xTimer, xCommandID, x_Optional_విలువ, px_Higher_ప్రాధాన్యత_Task_Woken, x_Ticks_గమ్యం_Wait )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_xTimerGenericCommandFromTask
    #నిర్వచించు traceRETURN_xTimerGenericCommandFromTask( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_xTimerGenericCommandFromISR
    #నిర్వచించు traceENTER_xTimerGenericCommandFromISR( xTimer, xCommandID, x_Optional_విలువ, px_Higher_ప్రాధాన్యత_Task_Woken, x_Ticks_గమ్యం_Wait )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_xTimerGenericCommandFromISR
    #నిర్వచించు traceRETURN_xTimerGenericCommandFromISR( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_xTimerGetTimerDaemonTaskHandle
    #నిర్వచించు traceENTER_xTimerGetTimerDaemonTaskHandle()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_xTimerGetTimerDaemonTaskHandle
    #నిర్వచించు traceRETURN_xTimerGetTimerDaemonTaskHandle( xTimerTaskHandle )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_xTimerGetPeriod
    #నిర్వచించు traceENTER_xTimerGetPeriod( xTimer )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_xTimerGetPeriod
    #నిర్వచించు traceRETURN_xTimerGetPeriod( x_Timer_Period_లోపల_Ticks )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_v_Timer_సమితి_Reload_బహుళకం
    #నిర్వచించు trace_ENTER_v_Timer_సమితి_Reload_బహుళకం( xTimer, xAutoReload )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_v_Timer_సమితి_Reload_బహుళకం
    #నిర్వచించు trace_RETURN_v_Timer_సమితి_Reload_బహుళకం()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_Timer_Get_Reload_బహుళకం
    #నిర్వచించు trace_ENTER_x_Timer_Get_Reload_బహుళకం( xTimer )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_Timer_Get_Reload_బహుళకం
    #నిర్వచించు trace_RETURN_x_Timer_Get_Reload_బహుళకం( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_ux_Timer_Get_Reload_బహుళకం
    #నిర్వచించు trace_ENTER_ux_Timer_Get_Reload_బహుళకం( xTimer )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_ux_Timer_Get_Reload_బహుళకం
    #నిర్వచించు trace_RETURN_ux_Timer_Get_Reload_బహుళకం( uxReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_Timer_Get_Expiry_సమయం
    #నిర్వచించు trace_ENTER_x_Timer_Get_Expiry_సమయం( xTimer )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_Timer_Get_Expiry_సమయం
    #నిర్వచించు trace_RETURN_x_Timer_Get_Expiry_సమయం( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_Timer_Get_Static_బఫరు
    #నిర్వచించు trace_ENTER_x_Timer_Get_Static_బఫరు( xTimer, ppx_Timer_బఫరు )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_Timer_Get_Static_బఫరు
    #నిర్వచించు trace_RETURN_x_Timer_Get_Static_బఫరు( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_pcTimerGetName
    #నిర్వచించు traceENTER_pcTimerGetName( xTimer )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_pcTimerGetName
    #నిర్వచించు traceRETURN_pcTimerGetName( pcTimerName )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_Timer_Is_Timer_చురుకైన
    #నిర్వచించు trace_ENTER_x_Timer_Is_Timer_చురుకైన( xTimer )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_Timer_Is_Timer_చురుకైన
    #నిర్వచించు trace_RETURN_x_Timer_Is_Timer_చురుకైన( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_pvTimerGetTimerID
    #నిర్వచించు traceENTER_pvTimerGetTimerID( xTimer )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_pvTimerGetTimerID
    #నిర్వచించు traceRETURN_pvTimerGetTimerID( pvReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_v_Timer_సమితి_Timer_ID
    #నిర్వచించు trace_ENTER_v_Timer_సమితి_Timer_ID( xTimer, pvNewID )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_v_Timer_సమితి_Timer_ID
    #నిర్వచించు trace_RETURN_v_Timer_సమితి_Timer_ID()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_xTimerPendFunctionCallFromISR
    #నిర్వచించు traceENTER_xTimerPendFunctionCallFromISR( x_Function_గమ్యం_Pend, pvParameter1, ulParameter2, px_Higher_ప్రాధాన్యత_Task_Woken )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_xTimerPendFunctionCallFromISR
    #నిర్వచించు traceRETURN_xTimerPendFunctionCallFromISR( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_xTimerPendFunctionCall
    #నిర్వచించు traceENTER_xTimerPendFunctionCall( x_Function_గమ్యం_Pend, pvParameter1, ulParameter2, x_Ticks_గమ్యం_Wait )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_xTimerPendFunctionCall
    #నిర్వచించు traceRETURN_xTimerPendFunctionCall( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_ux_Timer_Get_Timer_సంఖ్య
    #నిర్వచించు trace_ENTER_ux_Timer_Get_Timer_సంఖ్య( xTimer )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_ux_Timer_Get_Timer_సంఖ్య
    #నిర్వచించు trace_RETURN_ux_Timer_Get_Timer_సంఖ్య( ux_Timer_సంఖ్య )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_v_Timer_సమితి_Timer_సంఖ్య
    #నిర్వచించు trace_ENTER_v_Timer_సమితి_Timer_సంఖ్య( xTimer, ux_Timer_సంఖ్య )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_v_Timer_సమితి_Timer_సంఖ్య
    #నిర్వచించు trace_RETURN_v_Timer_సమితి_Timer_సంఖ్య()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_xTaskCreateStatic
    #నిర్వచించు traceENTER_xTaskCreateStatic( px_Task_సంకేతం, pcName, ux_దొంతర_లోతు, pvParameters, ux_ప్రాధాన్యత, pux_దొంతర_బఫరు, px_Task_బఫరు )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_xTaskCreateStatic
    #నిర్వచించు traceRETURN_xTaskCreateStatic( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_Task_Create_Static_Affinity_సమితి
    #నిర్వచించు trace_ENTER_x_Task_Create_Static_Affinity_సమితి( px_Task_సంకేతం, pcName, ux_దొంతర_లోతు, pvParameters, ux_ప్రాధాన్యత, pux_దొంతర_బఫరు, px_Task_బఫరు, ux_Core_Affinity_ముసుగు )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_Task_Create_Static_Affinity_సమితి
    #నిర్వచించు trace_RETURN_x_Task_Create_Static_Affinity_సమితి( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_xTaskCreateRestrictedStatic
    #నిర్వచించు traceENTER_xTaskCreateRestrictedStatic( pxTaskDefinition, pxCreatedTask )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_xTaskCreateRestrictedStatic
    #నిర్వచించు traceRETURN_xTaskCreateRestrictedStatic( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_Task_Create_Restricted_Static_Affinity_సమితి
    #నిర్వచించు trace_ENTER_x_Task_Create_Restricted_Static_Affinity_సమితి( pxTaskDefinition, ux_Core_Affinity_ముసుగు, pxCreatedTask )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_Task_Create_Restricted_Static_Affinity_సమితి
    #నిర్వచించు trace_RETURN_x_Task_Create_Restricted_Static_Affinity_సమితి( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_xTaskCreateRestricted
    #నిర్వచించు traceENTER_xTaskCreateRestricted( pxTaskDefinition, pxCreatedTask )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_xTaskCreateRestricted
    #నిర్వచించు traceRETURN_xTaskCreateRestricted( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_Task_Create_Restricted_Affinity_సమితి
    #నిర్వచించు trace_ENTER_x_Task_Create_Restricted_Affinity_సమితి( pxTaskDefinition, ux_Core_Affinity_ముసుగు, pxCreatedTask )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_Task_Create_Restricted_Affinity_సమితి
    #నిర్వచించు trace_RETURN_x_Task_Create_Restricted_Affinity_సమితి( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_xTaskCreate
    #నిర్వచించు traceENTER_xTaskCreate( px_Task_సంకేతం, pcName, ux_దొంతర_లోతు, pvParameters, ux_ప్రాధాన్యత, pxCreatedTask )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_xTaskCreate
    #నిర్వచించు traceRETURN_xTaskCreate( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_Task_Create_Affinity_సమితి
    #నిర్వచించు trace_ENTER_x_Task_Create_Affinity_సమితి( px_Task_సంకేతం, pcName, ux_దొంతర_లోతు, pvParameters, ux_ప్రాధాన్యత, ux_Core_Affinity_ముసుగు, pxCreatedTask )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_Task_Create_Affinity_సమితి
    #నిర్వచించు trace_RETURN_x_Task_Create_Affinity_సమితి( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_vTaskDelete
    #నిర్వచించు traceENTER_vTaskDelete( x_Task_గమ్యం_Delete )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_vTaskDelete
    #నిర్వచించు traceRETURN_vTaskDelete()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_xTaskDelayUntil
    #నిర్వచించు traceENTER_xTaskDelayUntil( px_మునుపటి_Wake_సమయం, x_సమయం_Increment )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_xTaskDelayUntil
    #నిర్వచించు traceRETURN_xTaskDelayUntil( xShouldDelay )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_vTaskDelay
    #నిర్వచించు traceENTER_vTaskDelay( x_Ticks_గమ్యం_Delay )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_vTaskDelay
    #నిర్వచించు traceRETURN_vTaskDelay()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_eTaskGetState
    #నిర్వచించు traceENTER_eTaskGetState( xTask )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_eTaskGetState
    #నిర్వచించు traceRETURN_eTaskGetState( eReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_ux_Task_ప్రాధాన్యత_Get
    #నిర్వచించు trace_ENTER_ux_Task_ప్రాధాన్యత_Get( xTask )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_ux_Task_ప్రాధాన్యత_Get
    #నిర్వచించు trace_RETURN_ux_Task_ప్రాధాన్యత_Get( uxReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_ux_Task_ప్రాధాన్యత_Get_From_ISR
    #నిర్వచించు trace_ENTER_ux_Task_ప్రాధాన్యత_Get_From_ISR( xTask )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_ux_Task_ప్రాధాన్యత_Get_From_ISR
    #నిర్వచించు trace_RETURN_ux_Task_ప్రాధాన్యత_Get_From_ISR( uxReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_ux_Task_పునాది_ప్రాధాన్యత_Get
    #నిర్వచించు trace_ENTER_ux_Task_పునాది_ప్రాధాన్యత_Get( xTask )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_ux_Task_పునాది_ప్రాధాన్యత_Get
    #నిర్వచించు trace_RETURN_ux_Task_పునాది_ప్రాధాన్యత_Get( uxReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_ux_Task_పునాది_ప్రాధాన్యత_Get_From_ISR
    #నిర్వచించు trace_ENTER_ux_Task_పునాది_ప్రాధాన్యత_Get_From_ISR( xTask )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_ux_Task_పునాది_ప్రాధాన్యత_Get_From_ISR
    #నిర్వచించు trace_RETURN_ux_Task_పునాది_ప్రాధాన్యత_Get_From_ISR( uxReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_v_Task_ప్రాధాన్యత_సమితి
    #నిర్వచించు trace_ENTER_v_Task_ప్రాధాన్యత_సమితి( xTask, ux_New_ప్రాధాన్యత )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_v_Task_ప్రాధాన్యత_సమితి
    #నిర్వచించు trace_RETURN_v_Task_ప్రాధాన్యత_సమితి()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_v_Task_Core_Affinity_సమితి
    #నిర్వచించు trace_ENTER_v_Task_Core_Affinity_సమితి( xTask, ux_Core_Affinity_ముసుగు )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_v_Task_Core_Affinity_సమితి
    #నిర్వచించు trace_RETURN_v_Task_Core_Affinity_సమితి()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_vTaskCoreAffinityGet
    #నిర్వచించు traceENTER_vTaskCoreAffinityGet( xTask )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_vTaskCoreAffinityGet
    #నిర్వచించు traceRETURN_vTaskCoreAffinityGet( ux_Core_Affinity_ముసుగు )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_vTaskPreemptionDisable
    #నిర్వచించు traceENTER_vTaskPreemptionDisable( xTask )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_vTaskPreemptionDisable
    #నిర్వచించు traceRETURN_vTaskPreemptionDisable()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_vTaskPreemptionEnable
    #నిర్వచించు traceENTER_vTaskPreemptionEnable( xTask )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_vTaskPreemptionEnable
    #నిర్వచించు traceRETURN_vTaskPreemptionEnable()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_vTaskSuspend
    #నిర్వచించు traceENTER_vTaskSuspend( x_Task_గమ్యం_Suspend )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_vTaskSuspend
    #నిర్వచించు traceRETURN_vTaskSuspend()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_vTaskResume
    #నిర్వచించు traceENTER_vTaskResume( x_Task_గమ్యం_Resume )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_vTaskResume
    #నిర్వచించు traceRETURN_vTaskResume()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_xTaskResumeFromISR
    #నిర్వచించు traceENTER_xTaskResumeFromISR( x_Task_గమ్యం_Resume )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_xTaskResumeFromISR
    #నిర్వచించు traceRETURN_xTaskResumeFromISR( xYieldRequired )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_v_Task_ప్రారంభం_Scheduler
    #నిర్వచించు trace_ENTER_v_Task_ప్రారంభం_Scheduler()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_v_Task_ప్రారంభం_Scheduler
    #నిర్వచించు trace_RETURN_v_Task_ప్రారంభం_Scheduler()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_v_Task_చివర_Scheduler
    #నిర్వచించు trace_ENTER_v_Task_చివర_Scheduler()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_v_Task_చివర_Scheduler
    #నిర్వచించు trace_RETURN_v_Task_చివర_Scheduler()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_vTaskSuspendAll
    #నిర్వచించు traceENTER_vTaskSuspendAll()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_vTaskSuspendAll
    #నిర్వచించు traceRETURN_vTaskSuspendAll()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_xTaskResumeAll
    #నిర్వచించు traceENTER_xTaskResumeAll()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_xTaskResumeAll
    #నిర్వచించు traceRETURN_xTaskResumeAll( xAlreadyYielded )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_Task_Get_Tick_లెక్క
    #నిర్వచించు trace_ENTER_x_Task_Get_Tick_లెక్క()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_Task_Get_Tick_లెక్క
    #నిర్వచించు trace_RETURN_x_Task_Get_Tick_లెక్క( xTicks )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_Task_Get_Tick_లెక్క_From_ISR
    #నిర్వచించు trace_ENTER_x_Task_Get_Tick_లెక్క_From_ISR()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_Task_Get_Tick_లెక్క_From_ISR
    #నిర్వచించు trace_RETURN_x_Task_Get_Tick_లెక్క_From_ISR( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_ux_Task_Get_సంఖ్య_యొక్క_Tasks
    #నిర్వచించు trace_ENTER_ux_Task_Get_సంఖ్య_యొక్క_Tasks()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_ux_Task_Get_సంఖ్య_యొక్క_Tasks
    #నిర్వచించు trace_RETURN_ux_Task_Get_సంఖ్య_యొక్క_Tasks( ux_Current_సంఖ్య_యొక్క_Tasks )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_pcTaskGetName
    #నిర్వచించు traceENTER_pcTaskGetName( x_Task_గమ్యం_Query )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_pcTaskGetName
    #నిర్వచించు traceRETURN_pcTaskGetName( pcTaskName )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_xTaskGetHandle
    #నిర్వచించు traceENTER_xTaskGetHandle( pc_Name_గమ్యం_Query )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_xTaskGetHandle
    #నిర్వచించు traceRETURN_xTaskGetHandle( pxTCB )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_xTaskGetStaticBuffers
    #నిర్వచించు traceENTER_xTaskGetStaticBuffers( xTask, ppux_దొంతర_బఫరు, ppx_Task_బఫరు )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_xTaskGetStaticBuffers
    #నిర్వచించు traceRETURN_xTaskGetStaticBuffers( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_uxTaskGetSystemState
    #నిర్వచించు traceENTER_uxTaskGetSystemState( px_Task_స్థితి_Array, ux_Array_కొలత, pul_మొత్తం_నిరంతరం_సమయం )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_uxTaskGetSystemState
    #నిర్వచించు traceRETURN_uxTaskGetSystemState( uxTask )
#అయితే_ముగింపు

#ఒకవేళ ( config_సంఖ్య_యొక్క_CORES == 1 )
    #నిర్వచితం_కాకపోతే traceENTER_xTaskGetIdleTaskHandle
        #నిర్వచించు traceENTER_xTaskGetIdleTaskHandle()
    #అయితే_ముగింపు
#అయితే_ముగింపు

#ఒకవేళ ( config_సంఖ్య_యొక్క_CORES == 1 )
    #నిర్వచితం_కాకపోతే traceRETURN_xTaskGetIdleTaskHandle
        #నిర్వచించు traceRETURN_xTaskGetIdleTaskHandle( xIdleTaskHandle )
    #అయితే_ముగింపు
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_xTaskGetIdleTaskHandleForCore
    #నిర్వచించు traceENTER_xTaskGetIdleTaskHandleForCore( xCoreID )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_xTaskGetIdleTaskHandleForCore
    #నిర్వచించు traceRETURN_xTaskGetIdleTaskHandleForCore( xIdleTaskHandle )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_v_Task_అడుగు_Tick
    #నిర్వచించు trace_ENTER_v_Task_అడుగు_Tick( x_Ticks_గమ్యం_Jump )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_v_Task_అడుగు_Tick
    #నిర్వచించు trace_RETURN_v_Task_అడుగు_Tick()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_Task_Catch_పైకి_Ticks
    #నిర్వచించు trace_ENTER_x_Task_Catch_పైకి_Ticks( x_Ticks_గమ్యం_Catch_పైకి )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_Task_Catch_పైకి_Ticks
    #నిర్వచించు trace_RETURN_x_Task_Catch_పైకి_Ticks( xYieldOccurred )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_xTaskAbortDelay
    #నిర్వచించు traceENTER_xTaskAbortDelay( xTask )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_xTaskAbortDelay
    #నిర్వచించు traceRETURN_xTaskAbortDelay( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_xTaskIncrementTick
    #నిర్వచించు traceENTER_xTaskIncrementTick()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_xTaskIncrementTick
    #నిర్వచించు traceRETURN_xTaskIncrementTick( xSwitchRequired )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_v_Task_సమితి_Application_Task_ట్యాగ్
    #నిర్వచించు trace_ENTER_v_Task_సమితి_Application_Task_ట్యాగ్( xTask, pxHookFunction )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_v_Task_సమితి_Application_Task_ట్యాగ్
    #నిర్వచించు trace_RETURN_v_Task_సమితి_Application_Task_ట్యాగ్()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_Task_Get_Application_Task_ట్యాగ్
    #నిర్వచించు trace_ENTER_x_Task_Get_Application_Task_ట్యాగ్( xTask )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_Task_Get_Application_Task_ట్యాగ్
    #నిర్వచించు trace_RETURN_x_Task_Get_Application_Task_ట్యాగ్( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_Task_Get_Application_Task_ట్యాగ్_From_ISR
    #నిర్వచించు trace_ENTER_x_Task_Get_Application_Task_ట్యాగ్_From_ISR( xTask )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_Task_Get_Application_Task_ట్యాగ్_From_ISR
    #నిర్వచించు trace_RETURN_x_Task_Get_Application_Task_ట్యాగ్_From_ISR( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_xTaskCallApplicationTaskHook
    #నిర్వచించు traceENTER_xTaskCallApplicationTaskHook( xTask, pvParameter )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_xTaskCallApplicationTaskHook
    #నిర్వచించు traceRETURN_xTaskCallApplicationTaskHook( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_vTaskSwitchContext
    #నిర్వచించు traceENTER_vTaskSwitchContext()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_vTaskSwitchContext
    #నిర్వచించు traceRETURN_vTaskSwitchContext()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_v_Task_ఉంచు_On_Event_List
    #నిర్వచించు trace_ENTER_v_Task_ఉంచు_On_Event_List( pxEventList, x_Ticks_గమ్యం_Wait )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_v_Task_ఉంచు_On_Event_List
    #నిర్వచించు trace_RETURN_v_Task_ఉంచు_On_Event_List()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_v_Task_ఉంచు_On_Unordered_Event_List
    #నిర్వచించు trace_ENTER_v_Task_ఉంచు_On_Unordered_Event_List( pxEventList, x_అంశం_విలువ, x_Ticks_గమ్యం_Wait )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_v_Task_ఉంచు_On_Unordered_Event_List
    #నిర్వచించు trace_RETURN_v_Task_ఉంచు_On_Unordered_Event_List()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_v_Task_ఉంచు_On_Event_List_Restricted
    #నిర్వచించు trace_ENTER_v_Task_ఉంచు_On_Event_List_Restricted( pxEventList, x_Ticks_గమ్యం_Wait, xWaitIndefinitely )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_v_Task_ఉంచు_On_Event_List_Restricted
    #నిర్వచించు trace_RETURN_v_Task_ఉంచు_On_Event_List_Restricted()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_xTaskRemoveFromEventList
    #నిర్వచించు traceENTER_xTaskRemoveFromEventList( pxEventList )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_xTaskRemoveFromEventList
    #నిర్వచించు traceRETURN_xTaskRemoveFromEventList( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_vTaskRemoveFromUnorderedEventList
    #నిర్వచించు traceENTER_vTaskRemoveFromUnorderedEventList( px_Event_List_అంశం, x_అంశం_విలువ )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_vTaskRemoveFromUnorderedEventList
    #నిర్వచించు traceRETURN_vTaskRemoveFromUnorderedEventList()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_v_Task_సమితి_సమయం_బహిర్గతం_State
    #నిర్వచించు trace_ENTER_v_Task_సమితి_సమయం_బహిర్గతం_State( px_సమయం_బహిర్గతం )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_v_Task_సమితి_సమయం_బహిర్గతం_State
    #నిర్వచించు trace_RETURN_v_Task_సమితి_సమయం_బహిర్గతం_State()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_v_Task_అంతర్గత_సమితి_సమయం_బహిర్గతం_State
    #నిర్వచించు trace_ENTER_v_Task_అంతర్గత_సమితి_సమయం_బహిర్గతం_State( px_సమయం_బహిర్గతం )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_v_Task_అంతర్గత_సమితి_సమయం_బహిర్గతం_State
    #నిర్వచించు trace_RETURN_v_Task_అంతర్గత_సమితి_సమయం_బహిర్గతం_State()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_Task_Check_For_సమయం_బహిర్గతం
    #నిర్వచించు trace_ENTER_x_Task_Check_For_సమయం_బహిర్గతం( px_సమయం_బహిర్గతం, px_Ticks_గమ్యం_Wait )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_Task_Check_For_సమయం_బహిర్గతం
    #నిర్వచించు trace_RETURN_x_Task_Check_For_సమయం_బహిర్గతం( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_vTaskMissedYield
    #నిర్వచించు traceENTER_vTaskMissedYield()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_vTaskMissedYield
    #నిర్వచించు traceRETURN_vTaskMissedYield()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_ux_Task_Get_Task_సంఖ్య
    #నిర్వచించు trace_ENTER_ux_Task_Get_Task_సంఖ్య( xTask )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_ux_Task_Get_Task_సంఖ్య
    #నిర్వచించు trace_RETURN_ux_Task_Get_Task_సంఖ్య( uxReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_v_Task_సమితి_Task_సంఖ్య
    #నిర్వచించు trace_ENTER_v_Task_సమితి_Task_సంఖ్య( xTask, uxHandle )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_v_Task_సమితి_Task_సంఖ్య
    #నిర్వచించు trace_RETURN_v_Task_సమితి_Task_సంఖ్య()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_e_Task_Confirm_Sleep_బహుళకం_స్థితి
    #నిర్వచించు trace_ENTER_e_Task_Confirm_Sleep_బహుళకం_స్థితి()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_e_Task_Confirm_Sleep_బహుళకం_స్థితి
    #నిర్వచించు trace_RETURN_e_Task_Confirm_Sleep_బహుళకం_స్థితి( eReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_v_Task_సమితి_Thread_Local_Storage_సూచిక
    #నిర్వచించు trace_ENTER_v_Task_సమితి_Thread_Local_Storage_సూచిక( x_Task_గమ్యం_సమితి, x_సూచీ, pv_విలువ )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_v_Task_సమితి_Thread_Local_Storage_సూచిక
    #నిర్వచించు trace_RETURN_v_Task_సమితి_Thread_Local_Storage_సూచిక()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_pv_Task_Get_Thread_Local_Storage_సూచిక
    #నిర్వచించు trace_ENTER_pv_Task_Get_Thread_Local_Storage_సూచిక( x_Task_గమ్యం_Query, x_సూచీ )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_pv_Task_Get_Thread_Local_Storage_సూచిక
    #నిర్వచించు trace_RETURN_pv_Task_Get_Thread_Local_Storage_సూచిక( pvReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_vTaskAllocateMPURegions
    #నిర్వచించు traceENTER_vTaskAllocateMPURegions( x_Task_గమ్యం_Modify, pxRegions )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_vTaskAllocateMPURegions
    #నిర్వచించు traceRETURN_vTaskAllocateMPURegions()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_vTaskGetInfo
    #నిర్వచించు traceENTER_vTaskGetInfo( xTask, px_Task_స్థితి, x_Get_Free_దొంతర_Space, eState )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_vTaskGetInfo
    #నిర్వచించు traceRETURN_vTaskGetInfo()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_ux_Task_Get_దొంతర_ఎగువ_Water_Mark2
    #నిర్వచించు trace_ENTER_ux_Task_Get_దొంతర_ఎగువ_Water_Mark2( xTask )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_ux_Task_Get_దొంతర_ఎగువ_Water_Mark2
    #నిర్వచించు trace_RETURN_ux_Task_Get_దొంతర_ఎగువ_Water_Mark2( uxReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_ux_Task_Get_దొంతర_ఎగువ_Water_Mark
    #నిర్వచించు trace_ENTER_ux_Task_Get_దొంతర_ఎగువ_Water_Mark( xTask )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_ux_Task_Get_దొంతర_ఎగువ_Water_Mark
    #నిర్వచించు trace_RETURN_ux_Task_Get_దొంతర_ఎగువ_Water_Mark( uxReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_xTaskGetCurrentTaskHandle
    #నిర్వచించు traceENTER_xTaskGetCurrentTaskHandle()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_xTaskGetCurrentTaskHandle
    #నిర్వచించు traceRETURN_xTaskGetCurrentTaskHandle( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_xTaskGetCurrentTaskHandleForCore
    #నిర్వచించు traceENTER_xTaskGetCurrentTaskHandleForCore( xCoreID )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_xTaskGetCurrentTaskHandleForCore
    #నిర్వచించు traceRETURN_xTaskGetCurrentTaskHandleForCore( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_xTaskGetSchedulerState
    #నిర్వచించు traceENTER_xTaskGetSchedulerState()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_xTaskGetSchedulerState
    #నిర్వచించు traceRETURN_xTaskGetSchedulerState( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_Task_ప్రాధాన్యత_Inherit
    #నిర్వచించు trace_ENTER_x_Task_ప్రాధాన్యత_Inherit( pxMutexHolder )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_Task_ప్రాధాన్యత_Inherit
    #నిర్వచించు trace_RETURN_x_Task_ప్రాధాన్యత_Inherit( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_Task_ప్రాధాన్యత_Disinherit
    #నిర్వచించు trace_ENTER_x_Task_ప్రాధాన్యత_Disinherit( pxMutexHolder )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_Task_ప్రాధాన్యత_Disinherit
    #నిర్వచించు trace_RETURN_x_Task_ప్రాధాన్యత_Disinherit( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_v_Task_ప్రాధాన్యత_Disinherit_After_Timeout
    #నిర్వచించు trace_ENTER_v_Task_ప్రాధాన్యత_Disinherit_After_Timeout( pxMutexHolder, ux_Highest_ప్రాధాన్యత_నిరీక్షణ_Task )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_v_Task_ప్రాధాన్యత_Disinherit_After_Timeout
    #నిర్వచించు trace_RETURN_v_Task_ప్రాధాన్యత_Disinherit_After_Timeout()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_vTaskYieldWithinAPI
    #నిర్వచించు traceENTER_vTaskYieldWithinAPI()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_vTaskYieldWithinAPI
    #నిర్వచించు traceRETURN_vTaskYieldWithinAPI()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_vTaskEnterCritical
    #నిర్వచించు traceENTER_vTaskEnterCritical()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_vTaskEnterCritical
    #నిర్వచించు traceRETURN_vTaskEnterCritical()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_vTaskEnterCriticalFromISR
    #నిర్వచించు traceENTER_vTaskEnterCriticalFromISR()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_vTaskEnterCriticalFromISR
    #నిర్వచించు traceRETURN_vTaskEnterCriticalFromISR( ux_Saved_అంతరాయం_స్థితి )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_vTaskExitCritical
    #నిర్వచించు traceENTER_vTaskExitCritical()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_vTaskExitCritical
    #నిర్వచించు traceRETURN_vTaskExitCritical()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_vTaskExitCriticalFromISR
    #నిర్వచించు traceENTER_vTaskExitCriticalFromISR( ux_Saved_అంతరాయం_స్థితి )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_vTaskExitCriticalFromISR
    #నిర్వచించు traceRETURN_vTaskExitCriticalFromISR()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_vTaskListTasks
    #నిర్వచించు traceENTER_vTaskListTasks( pc_Write_బఫరు, ux_బఫరు_నిడివి )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_vTaskListTasks
    #నిర్వచించు traceRETURN_vTaskListTasks()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_v_Task_Get_నిరంతరం_సమయం_Statistics
    #నిర్వచించు trace_ENTER_v_Task_Get_నిరంతరం_సమయం_Statistics( pc_Write_బఫరు, ux_బఫరు_నిడివి )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_v_Task_Get_నిరంతరం_సమయం_Statistics
    #నిర్వచించు trace_RETURN_v_Task_Get_నిరంతరం_సమయం_Statistics()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_ux_Task_Reset_Event_అంశం_విలువ
    #నిర్వచించు trace_ENTER_ux_Task_Reset_Event_అంశం_విలువ()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_ux_Task_Reset_Event_అంశం_విలువ
    #నిర్వచించు trace_RETURN_ux_Task_Reset_Event_అంశం_విలువ( uxReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_pv_Task_Increment_Mutex_Held_లెక్క
    #నిర్వచించు trace_ENTER_pv_Task_Increment_Mutex_Held_లెక్క()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_pv_Task_Increment_Mutex_Held_లెక్క
    #నిర్వచించు trace_RETURN_pv_Task_Increment_Mutex_Held_లెక్క( pxTCB )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_ulTaskGenericNotifyTake
    #నిర్వచించు traceENTER_ulTaskGenericNotifyTake( ux_సూచీ_గమ్యం_Wait_On, x_Clear_లెక్క_On_Exit, x_Ticks_గమ్యం_Wait )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_ulTaskGenericNotifyTake
    #నిర్వచించు traceRETURN_ulTaskGenericNotifyTake( ulReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_xTaskGenericNotifyWait
    #నిర్వచించు traceENTER_xTaskGenericNotifyWait( ux_సూచీ_గమ్యం_Wait_On, ul_బిట్లు_గమ్యం_Clear_On_నమోదు, ul_బిట్లు_గమ్యం_Clear_On_Exit, pul_Notification_విలువ, x_Ticks_గమ్యం_Wait )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_xTaskGenericNotifyWait
    #నిర్వచించు traceRETURN_xTaskGenericNotifyWait( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_xTaskGenericNotify
    #నిర్వచించు traceENTER_xTaskGenericNotify( x_Task_గమ్యం_Notify, ux_సూచీ_గమ్యం_Notify, ul_విలువ, eAction, pul_మునుపటి_Notification_విలువ )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_xTaskGenericNotify
    #నిర్వచించు traceRETURN_xTaskGenericNotify( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_xTaskGenericNotifyFromISR
    #నిర్వచించు traceENTER_xTaskGenericNotifyFromISR( x_Task_గమ్యం_Notify, ux_సూచీ_గమ్యం_Notify, ul_విలువ, eAction, pul_మునుపటి_Notification_విలువ, px_Higher_ప్రాధాన్యత_Task_Woken )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_xTaskGenericNotifyFromISR
    #నిర్వచించు traceRETURN_xTaskGenericNotifyFromISR( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_vTaskGenericNotifyGiveFromISR
    #నిర్వచించు traceENTER_vTaskGenericNotifyGiveFromISR( x_Task_గమ్యం_Notify, ux_సూచీ_గమ్యం_Notify, px_Higher_ప్రాధాన్యత_Task_Woken )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_vTaskGenericNotifyGiveFromISR
    #నిర్వచించు traceRETURN_vTaskGenericNotifyGiveFromISR()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_xTaskGenericNotifyStateClear
    #నిర్వచించు traceENTER_xTaskGenericNotifyStateClear( xTask, ux_సూచీ_గమ్యం_Clear )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_xTaskGenericNotifyStateClear
    #నిర్వచించు traceRETURN_xTaskGenericNotifyStateClear( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_ul_Task_Generic_Notify_విలువ_Clear
    #నిర్వచించు trace_ENTER_ul_Task_Generic_Notify_విలువ_Clear( xTask, ux_సూచీ_గమ్యం_Clear, ul_బిట్లు_గమ్యం_Clear )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_ul_Task_Generic_Notify_విలువ_Clear
    #నిర్వచించు trace_RETURN_ul_Task_Generic_Notify_విలువ_Clear( ulReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_ul_Task_Get_నిరంతరం_సమయం_Counter
    #నిర్వచించు trace_ENTER_ul_Task_Get_నిరంతరం_సమయం_Counter( xTask )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_ul_Task_Get_నిరంతరం_సమయం_Counter
    #నిర్వచించు trace_RETURN_ul_Task_Get_నిరంతరం_సమయం_Counter( ul_నిరంతరం_సమయం_Counter )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_ul_Task_Get_నిరంతరం_సమయం_Percent
    #నిర్వచించు trace_ENTER_ul_Task_Get_నిరంతరం_సమయం_Percent( xTask )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_ul_Task_Get_నిరంతరం_సమయం_Percent
    #నిర్వచించు trace_RETURN_ul_Task_Get_నిరంతరం_సమయం_Percent( ulReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_ul_Task_Get_Idle_నిరంతరం_సమయం_Counter
    #నిర్వచించు trace_ENTER_ul_Task_Get_Idle_నిరంతరం_సమయం_Counter()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_ul_Task_Get_Idle_నిరంతరం_సమయం_Counter
    #నిర్వచించు trace_RETURN_ul_Task_Get_Idle_నిరంతరం_సమయం_Counter( ulReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_ul_Task_Get_Idle_నిరంతరం_సమయం_Percent
    #నిర్వచించు trace_ENTER_ul_Task_Get_Idle_నిరంతరం_సమయం_Percent()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_ul_Task_Get_Idle_నిరంతరం_సమయం_Percent
    #నిర్వచించు trace_RETURN_ul_Task_Get_Idle_నిరంతరం_సమయం_Percent( ulReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_xTaskGetMPUSettings
    #నిర్వచించు traceENTER_xTaskGetMPUSettings( xTask )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_xTaskGetMPUSettings
    #నిర్వచించు traceRETURN_xTaskGetMPUSettings( xMPUSettings )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_ప్రవాహం_బఫరు_Generic_Create
    #నిర్వచించు trace_ENTER_x_ప్రవాహం_బఫరు_Generic_Create( x_బఫరు_కొలత_బైట్లు, x_Trigger_Level_బైట్లు, x_ప్రవాహం_బఫరు_Type, pxSendCompletedCallback, pxReceiveCompletedCallback )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_ప్రవాహం_బఫరు_Generic_Create
    #నిర్వచించు trace_RETURN_x_ప్రవాహం_బఫరు_Generic_Create( pv_Allocated_జ్ఞాపకం )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_ప్రవాహం_బఫరు_Generic_Create_Static
    #నిర్వచించు trace_ENTER_x_ప్రవాహం_బఫరు_Generic_Create_Static( x_బఫరు_కొలత_బైట్లు, x_Trigger_Level_బైట్లు, x_ప్రవాహం_బఫరు_Type, puc_ప్రవాహం_బఫరు_Storage_Area, px_Static_ప్రవాహం_బఫరు, pxSendCompletedCallback, pxReceiveCompletedCallback )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_ప్రవాహం_బఫరు_Generic_Create_Static
    #నిర్వచించు trace_RETURN_x_ప్రవాహం_బఫరు_Generic_Create_Static( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_ప్రవాహం_బఫరు_Get_Static_Buffers
    #నిర్వచించు trace_ENTER_x_ప్రవాహం_బఫరు_Get_Static_Buffers( x_ప్రవాహం_బఫరు, ppuc_ప్రవాహం_బఫరు_Storage_Area, ppx_Static_ప్రవాహం_బఫరు )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_ప్రవాహం_బఫరు_Get_Static_Buffers
    #నిర్వచించు trace_RETURN_x_ప్రవాహం_బఫరు_Get_Static_Buffers( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_v_ప్రవాహం_బఫరు_Delete
    #నిర్వచించు trace_ENTER_v_ప్రవాహం_బఫరు_Delete( x_ప్రవాహం_బఫరు )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_v_ప్రవాహం_బఫరు_Delete
    #నిర్వచించు trace_RETURN_v_ప్రవాహం_బఫరు_Delete()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_ప్రవాహం_బఫరు_Reset
    #నిర్వచించు trace_ENTER_x_ప్రవాహం_బఫరు_Reset( x_ప్రవాహం_బఫరు )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_ప్రవాహం_బఫరు_Reset
    #నిర్వచించు trace_RETURN_x_ప్రవాహం_బఫరు_Reset( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_ప్రవాహం_బఫరు_Reset_From_ISR
    #నిర్వచించు trace_ENTER_x_ప్రవాహం_బఫరు_Reset_From_ISR( x_ప్రవాహం_బఫరు )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_ప్రవాహం_బఫరు_Reset_From_ISR
    #నిర్వచించు trace_RETURN_x_ప్రవాహం_బఫరు_Reset_From_ISR( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_ప్రవాహం_బఫరు_సమితి_Trigger_Level
    #నిర్వచించు trace_ENTER_x_ప్రవాహం_బఫరు_సమితి_Trigger_Level( x_ప్రవాహం_బఫరు, xTriggerLevel )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_ప్రవాహం_బఫరు_సమితి_Trigger_Level
    #నిర్వచించు trace_RETURN_x_ప్రవాహం_బఫరు_సమితి_Trigger_Level( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_ప్రవాహం_బఫరు_Spaces_అందుబాటు
    #నిర్వచించు trace_ENTER_x_ప్రవాహం_బఫరు_Spaces_అందుబాటు( x_ప్రవాహం_బఫరు )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_ప్రవాహం_బఫరు_Spaces_అందుబాటు
    #నిర్వచించు trace_RETURN_x_ప్రవాహం_బఫరు_Spaces_అందుబాటు( xSpace )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_ప్రవాహం_బఫరు_బైట్లు_అందుబాటు
    #నిర్వచించు trace_ENTER_x_ప్రవాహం_బఫరు_బైట్లు_అందుబాటు( x_ప్రవాహం_బఫరు )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_ప్రవాహం_బఫరు_బైట్లు_అందుబాటు
    #నిర్వచించు trace_RETURN_x_ప్రవాహం_బఫరు_బైట్లు_అందుబాటు( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_ప్రవాహం_బఫరు_Send
    #నిర్వచించు trace_ENTER_x_ప్రవాహం_బఫరు_Send( x_ప్రవాహం_బఫరు, pv_Tx_దత్తాంశం, x_దత్తాంశం_నిడివి_బైట్లు, x_Ticks_గమ్యం_Wait )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_ప్రవాహం_బఫరు_Send
    #నిర్వచించు trace_RETURN_x_ప్రవాహం_బఫరు_Send( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_ప్రవాహం_బఫరు_Send_From_ISR
    #నిర్వచించు trace_ENTER_x_ప్రవాహం_బఫరు_Send_From_ISR( x_ప్రవాహం_బఫరు, pv_Tx_దత్తాంశం, x_దత్తాంశం_నిడివి_బైట్లు, px_Higher_ప్రాధాన్యత_Task_Woken )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_ప్రవాహం_బఫరు_Send_From_ISR
    #నిర్వచించు trace_RETURN_x_ప్రవాహం_బఫరు_Send_From_ISR( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_ప్రవాహం_బఫరు_Receive
    #నిర్వచించు trace_ENTER_x_ప్రవాహం_బఫరు_Receive( x_ప్రవాహం_బఫరు, pv_Rx_దత్తాంశం, x_బఫరు_నిడివి_బైట్లు, x_Ticks_గమ్యం_Wait )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_ప్రవాహం_బఫరు_Receive
    #నిర్వచించు trace_RETURN_x_ప్రవాహం_బఫరు_Receive( x_Received_నిడివి )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_ప్రవాహం_బఫరు_తదుపరి_సందేశం_నిడివి_బైట్లు
    #నిర్వచించు trace_ENTER_x_ప్రవాహం_బఫరు_తదుపరి_సందేశం_నిడివి_బైట్లు( x_ప్రవాహం_బఫరు )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_ప్రవాహం_బఫరు_తదుపరి_సందేశం_నిడివి_బైట్లు
    #నిర్వచించు trace_RETURN_x_ప్రవాహం_బఫరు_తదుపరి_సందేశం_నిడివి_బైట్లు( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_ప్రవాహం_బఫరు_Receive_From_ISR
    #నిర్వచించు trace_ENTER_x_ప్రవాహం_బఫరు_Receive_From_ISR( x_ప్రవాహం_బఫరు, pv_Rx_దత్తాంశం, x_బఫరు_నిడివి_బైట్లు, px_Higher_ప్రాధాన్యత_Task_Woken )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_ప్రవాహం_బఫరు_Receive_From_ISR
    #నిర్వచించు trace_RETURN_x_ప్రవాహం_బఫరు_Receive_From_ISR( x_Received_నిడివి )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_ప్రవాహం_బఫరు_Is_ఖాళీలు
    #నిర్వచించు trace_ENTER_x_ప్రవాహం_బఫరు_Is_ఖాళీలు( x_ప్రవాహం_బఫరు )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_ప్రవాహం_బఫరు_Is_ఖాళీలు
    #నిర్వచించు trace_RETURN_x_ప్రవాహం_బఫరు_Is_ఖాళీలు( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_ప్రవాహం_బఫరు_Is_నిండిన
    #నిర్వచించు trace_ENTER_x_ప్రవాహం_బఫరు_Is_నిండిన( x_ప్రవాహం_బఫరు )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_ప్రవాహం_బఫరు_Is_నిండిన
    #నిర్వచించు trace_RETURN_x_ప్రవాహం_బఫరు_Is_నిండిన( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_ప్రవాహం_బఫరు_Send_Completed_From_ISR
    #నిర్వచించు trace_ENTER_x_ప్రవాహం_బఫరు_Send_Completed_From_ISR( x_ప్రవాహం_బఫరు, px_Higher_ప్రాధాన్యత_Task_Woken )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_ప్రవాహం_బఫరు_Send_Completed_From_ISR
    #నిర్వచించు trace_RETURN_x_ప్రవాహం_బఫరు_Send_Completed_From_ISR( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_x_ప్రవాహం_బఫరు_Receive_Completed_From_ISR
    #నిర్వచించు trace_ENTER_x_ప్రవాహం_బఫరు_Receive_Completed_From_ISR( x_ప్రవాహం_బఫరు, px_Higher_ప్రాధాన్యత_Task_Woken )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_x_ప్రవాహం_బఫరు_Receive_Completed_From_ISR
    #నిర్వచించు trace_RETURN_x_ప్రవాహం_బఫరు_Receive_Completed_From_ISR( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_ux_ప్రవాహం_బఫరు_Get_ప్రవాహం_బఫరు_Notification_సూచీ
    #నిర్వచించు trace_ENTER_ux_ప్రవాహం_బఫరు_Get_ప్రవాహం_బఫరు_Notification_సూచీ( x_ప్రవాహం_బఫరు )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_ux_ప్రవాహం_బఫరు_Get_ప్రవాహం_బఫరు_Notification_సూచీ
    #నిర్వచించు trace_RETURN_ux_ప్రవాహం_బఫరు_Get_ప్రవాహం_బఫరు_Notification_సూచీ( ux_Notification_సూచీ )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_v_ప్రవాహం_బఫరు_సమితి_ప్రవాహం_బఫరు_Notification_సూచీ
    #నిర్వచించు trace_ENTER_v_ప్రవాహం_బఫరు_సమితి_ప్రవాహం_బఫరు_Notification_సూచీ( x_ప్రవాహం_బఫరు, ux_Notification_సూచీ )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_v_ప్రవాహం_బఫరు_సమితి_ప్రవాహం_బఫరు_Notification_సూచీ
    #నిర్వచించు trace_RETURN_v_ప్రవాహం_బఫరు_సమితి_ప్రవాహం_బఫరు_Notification_సూచీ()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_ux_ప్రవాహం_బఫరు_Get_ప్రవాహం_బఫరు_సంఖ్య
    #నిర్వచించు trace_ENTER_ux_ప్రవాహం_బఫరు_Get_ప్రవాహం_బఫరు_సంఖ్య( x_ప్రవాహం_బఫరు )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_ux_ప్రవాహం_బఫరు_Get_ప్రవాహం_బఫరు_సంఖ్య
    #నిర్వచించు trace_RETURN_ux_ప్రవాహం_బఫరు_Get_ప్రవాహం_బఫరు_సంఖ్య( ux_ప్రవాహం_బఫరు_సంఖ్య )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_v_ప్రవాహం_బఫరు_సమితి_ప్రవాహం_బఫరు_సంఖ్య
    #నిర్వచించు trace_ENTER_v_ప్రవాహం_బఫరు_సమితి_ప్రవాహం_బఫరు_సంఖ్య( x_ప్రవాహం_బఫరు, ux_ప్రవాహం_బఫరు_సంఖ్య )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_v_ప్రవాహం_బఫరు_సమితి_ప్రవాహం_బఫరు_సంఖ్య
    #నిర్వచించు trace_RETURN_v_ప్రవాహం_బఫరు_సమితి_ప్రవాహం_బఫరు_సంఖ్య()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_uc_ప్రవాహం_బఫరు_Get_ప్రవాహం_బఫరు_Type
    #నిర్వచించు trace_ENTER_uc_ప్రవాహం_బఫరు_Get_ప్రవాహం_బఫరు_Type( x_ప్రవాహం_బఫరు )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_uc_ప్రవాహం_బఫరు_Get_ప్రవాహం_బఫరు_Type
    #నిర్వచించు trace_RETURN_uc_ప్రవాహం_బఫరు_Get_ప్రవాహం_బఫరు_Type( uc_ప్రవాహం_బఫరు_Type )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_vListInitialise
    #నిర్వచించు traceENTER_vListInitialise( pxList )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_vListInitialise
    #నిర్వచించు traceRETURN_vListInitialise()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_v_List_Initialise_అంశం
    #నిర్వచించు trace_ENTER_v_List_Initialise_అంశం( px_అంశం )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_v_List_Initialise_అంశం
    #నిర్వచించు trace_RETURN_v_List_Initialise_అంశం()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_v_List_చొప్పించు_చివర
    #నిర్వచించు trace_ENTER_v_List_చొప్పించు_చివర( pxList, px_New_List_అంశం )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_v_List_చొప్పించు_చివర
    #నిర్వచించు trace_RETURN_v_List_చొప్పించు_చివర()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_v_List_చొప్పించు
    #నిర్వచించు trace_ENTER_v_List_చొప్పించు( pxList, px_New_List_అంశం )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_v_List_చొప్పించు
    #నిర్వచించు trace_RETURN_v_List_చొప్పించు()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_uxListRemove
    #నిర్వచించు traceENTER_uxListRemove( px_అంశం_గమ్యం_Remove )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_uxListRemove
    #నిర్వచించు traceRETURN_uxListRemove( ux_సంఖ్య_యొక్క_వస్తువులు )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_xCoRoutineCreate
    #నిర్వచించు traceENTER_xCoRoutineCreate( px_Co_Routine_సంకేతం, ux_ప్రాధాన్యత, ux_సూచీ )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_xCoRoutineCreate
    #నిర్వచించు traceRETURN_xCoRoutineCreate( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_v_Co_Routine_కూడు_గమ్యం_Delayed_List
    #నిర్వచించు trace_ENTER_v_Co_Routine_కూడు_గమ్యం_Delayed_List( x_Ticks_గమ్యం_Delay, pxEventList )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_v_Co_Routine_కూడు_గమ్యం_Delayed_List
    #నిర్వచించు trace_RETURN_v_Co_Routine_కూడు_గమ్యం_Delayed_List()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_ENTER_v_Co_Routine_కాలపట్టిక
    #నిర్వచించు trace_ENTER_v_Co_Routine_కాలపట్టిక()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే trace_RETURN_v_Co_Routine_కాలపట్టిక
    #నిర్వచించు trace_RETURN_v_Co_Routine_కాలపట్టిక()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceENTER_xCoRoutineRemoveFromEventList
    #నిర్వచించు traceENTER_xCoRoutineRemoveFromEventList( pxEventList )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే traceRETURN_xCoRoutineRemoveFromEventList
    #నిర్వచించు traceRETURN_xCoRoutineRemoveFromEventList( xReturn )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే config_GENERATE_నిరంతరం_సమయం_STATS
    #నిర్వచించు config_GENERATE_నిరంతరం_సమయం_STATS    0
#అయితే_ముగింపు

#ఒకవేళ ( config_GENERATE_నిరంతరం_సమయం_STATS == 1 )

    #నిర్వచితం_కాకపోతే పోర్టు_CONFIGURE_TIMER_FOR_నిరంతరం_సమయం_STATS
        #దోషం If config_GENERATE_నిరంతరం_సమయం_STATS is defined then పోర్టు_CONFIGURE_TIMER_FOR_నిరంతరం_సమయం_STATS must also be defined.  పోర్టు_CONFIGURE_TIMER_FOR_నిరంతరం_సమయం_STATS should call a పోర్టు layer function గమ్యం అమరిక a peripheral timer/counter that can then be వాడిన as the నిరంతరం time counter time పునాది.
    #అయితే_ముగింపు /* portCONFIGURE_TIMER_FOR_RUN_TIME_STATS */

    #నిర్వచితం_కాకపోతే పోర్టు_GET_నిరంతరం_సమయం_COUNTER_విలువ
        #నిర్వచితం_కాకపోతే పోర్టు_ALT_GET_నిరంతరం_సమయం_COUNTER_విలువ
            #దోషం If config_GENERATE_నిరంతరం_సమయం_STATS is defined then either పోర్టు_GET_నిరంతరం_సమయం_COUNTER_విలువ or పోర్టు_ALT_GET_నిరంతరం_సమయం_COUNTER_విలువ must also be defined.  See the examples provided and the FreeRTOS web site కొరకు మరిన్ని information.
        #అయితే_ముగింపు /* portALT_GET_RUN_TIME_COUNTER_VALUE */
    #అయితే_ముగింపు /* portGET_RUN_TIME_COUNTER_VALUE */

#అయితే_ముగింపు /* configGENERATE_RUN_TIME_STATS */

#నిర్వచితం_కాకపోతే పోర్టు_CONFIGURE_TIMER_FOR_నిరంతరం_సమయం_STATS
    #నిర్వచించు పోర్టు_CONFIGURE_TIMER_FOR_నిరంతరం_సమయం_STATS()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే పోర్టు_PRIVILEGE_బిట్టు
    #నిర్వచించు పోర్టు_PRIVILEGE_బిట్టు    ( ( UBaseType_t ) 0x00 )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే పోర్టు_YIELD_WITHIN_API
    #నిర్వచించు పోర్టు_YIELD_WITHIN_API    పోర్టు_YIELD
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే పోర్టు_SUPPRESS_TICKS_AND_SLEEP
    #నిర్వచించు పోర్టు_SUPPRESS_TICKS_AND_SLEEP( x_Expected_Idle_సమయం )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే config_EXPECTED_IDLE_సమయం_BEFORE_SLEEP
    #నిర్వచించు config_EXPECTED_IDLE_సమయం_BEFORE_SLEEP    2
#అయితే_ముగింపు

#ఒకవేళ config_EXPECTED_IDLE_సమయం_BEFORE_SLEEP < 2
    #దోషం config_EXPECTED_IDLE_సమయం_BEFORE_SLEEP must not be less than 2
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే configUSE_TICKLESS_IDLE
    #నిర్వచించు configUSE_TICKLESS_IDLE    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే configPRE_SUPPRESS_TICKS_AND_SLEEP_PROCESSING
    #నిర్వచించు configPRE_SUPPRESS_TICKS_AND_SLEEP_PROCESSING( x )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే configPRE_SLEEP_PROCESSING
    #నిర్వచించు configPRE_SLEEP_PROCESSING( x )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే configPOST_SLEEP_PROCESSING
    #నిర్వచించు configPOST_SLEEP_PROCESSING( x )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే config_USE_వరుస_SETS
    #నిర్వచించు config_USE_వరుస_SETS    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే పోర్టు_TASK_ఉపయోగిస్తుంది_FLOATING_బిందువు
    #నిర్వచించు పోర్టు_TASK_ఉపయోగిస్తుంది_FLOATING_బిందువు()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే పోర్టు_ALLOCATE_SECURE_CONTEXT
    #నిర్వచించు పోర్టు_ALLOCATE_SECURE_CONTEXT( ul_Secure_దొంతర_కొలత )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే పోర్టు_DONT_DISCARD
    #నిర్వచించు పోర్టు_DONT_DISCARD
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే config_USE_సమయం_SLICING
    #నిర్వచించు config_USE_సమయం_SLICING    1
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS
    #నిర్వచించు configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే configUSE_STATS_FORMATTING_FUNCTIONS
    #నిర్వచించు configUSE_STATS_FORMATTING_FUNCTIONS    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే పోర్టు_ASSERT_IF_అంతరాయం_ప్రాధాన్యత_INVALID
    #నిర్వచించు పోర్టు_ASSERT_IF_అంతరాయం_ప్రాధాన్యత_INVALID()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే configUSE_TRACE_FACILITY
    #నిర్వచించు configUSE_TRACE_FACILITY    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే mtCOVERAGE_TEST_MARKER
    #నిర్వచించు mtCOVERAGE_TEST_MARKER()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే mtCOVERAGE_TEST_DELAY
    #నిర్వచించు mtCOVERAGE_TEST_DELAY()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే పోర్టు_ASSERT_IF_లోపల_ISR
    #నిర్వచించు పోర్టు_ASSERT_IF_లోపల_ISR()
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే config_USE_పోర్టు_OPTIMISED_TASK_SELECTION
    #నిర్వచించు config_USE_పోర్టు_OPTIMISED_TASK_SELECTION    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే config_APPLICATION_ALLOCATED_కుప్ప
    #నిర్వచించు config_APPLICATION_ALLOCATED_కుప్ప    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే config_ENABLE_కుప్ప_PROTECTOR
    #నిర్వచించు config_ENABLE_కుప్ప_PROTECTOR    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే configUSE_TASK_NOTIFICATIONS
    #నిర్వచించు configUSE_TASK_NOTIFICATIONS    1
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే configTASK_NOTIFICATION_ARRAY_ENTRIES
    #నిర్వచించు configTASK_NOTIFICATION_ARRAY_ENTRIES    1
#అయితే_ముగింపు

#ఒకవేళ configTASK_NOTIFICATION_ARRAY_ENTRIES < 1
    #దోషం configTASK_NOTIFICATION_ARRAY_ENTRIES must be at కనిష్ఠ 1
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే configUSE_POSIX_ERRNO
    #నిర్వచించు configUSE_POSIX_ERRNO    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే configUSE_SB_COMPLETED_CALLBACK

/* By default per-instance callbacks are not enabled for stream buffer or message buffer. */
    #నిర్వచించు configUSE_SB_COMPLETED_CALLBACK    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే పోర్టు_TICK_TYPE_IS_ATOMIC
    #నిర్వచించు పోర్టు_TICK_TYPE_IS_ATOMIC    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే config_SUPPORT_STATIC_కేటాయింపు
    /* Defaults to 0 for backward compatibility. */
    #నిర్వచించు config_SUPPORT_STATIC_కేటాయింపు    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే config_KERNEL_PROVIDED_STATIC_జ్ఞాపకం
    #నిర్వచించు config_KERNEL_PROVIDED_STATIC_జ్ఞాపకం    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే config_SUPPORT_DYNAMIC_కేటాయింపు
    /* Defaults to 1 for backward compatibility. */
    #నిర్వచించు config_SUPPORT_DYNAMIC_కేటాయింపు    1
#అయితే_ముగింపు

#ఒకవేళ ( ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 ) && ( config_SUPPORT_DYNAMIC_కేటాయింపు != 1 ) )
    #దోషం configUSE_STATS_FORMATTING_FUNCTIONS cannot be వాడిన without dynamic కేటాయింపు, but config_SUPPORT_DYNAMIC_కేటాయింపు is not సమితి గమ్యం 1.
#అయితే_ముగింపు

#ఒకవేళ ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 )
    #ఒకవేళ ( ( configUSE_TRACE_FACILITY != 1 ) && ( config_GENERATE_నిరంతరం_సమయం_STATS != 1 ) )
        #దోషం configUSE_STATS_FORMATTING_FUNCTIONS is 1 but the functions it enables are not వాడిన because neither configUSE_TRACE_FACILITY or config_GENERATE_నిరంతరం_సమయం_STATS are 1.  సమితి_2 configUSE_STATS_FORMATTING_FUNCTIONS గమ్యం 0 లోపల FreeRTOSConfig.h.
    #అయితే_ముగింపు
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే config_STATS_బఫరు_MAX_నిడివి
    #నిర్వచించు config_STATS_బఫరు_MAX_నిడివి    0xFFFF
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే config_దొంతర_లోతు_TYPE

/* Defaults to StackType_t for backward compatibility, but can be overridden
 * in FreeRTOSConfig.h if StackType_t is too restrictive. */
    #నిర్వచించు config_దొంతర_లోతు_TYPE    దొంతర_Type_t
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే config_నిరంతరం_సమయం_COUNTER_TYPE

/* Defaults to uint32_t for backward compatibility, but can be overridden in
 * FreeRTOSConfig.h if uint32_t is too restrictive. */

    #నిర్వచించు config_నిరంతరం_సమయం_COUNTER_TYPE    uint32_t
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే config_సందేశం_బఫరు_నిడివి_TYPE

/* Defaults to size_t for backward compatibility, but can be overridden
 * in FreeRTOSConfig.h if lengths will always be less than the number of bytes
 * in a size_t. */
    #నిర్వచించు config_సందేశం_బఫరు_నిడివి_TYPE    size_t
#అయితే_ముగింపు

/* Sanity check the configuration. */
#ఒకవేళ ( ( config_SUPPORT_STATIC_కేటాయింపు == 0 ) && ( config_SUPPORT_DYNAMIC_కేటాయింపు == 0 ) )
    #దోషం config_SUPPORT_STATIC_కేటాయింపు and config_SUPPORT_DYNAMIC_కేటాయింపు cannot both be 0, but can both be 1.
#అయితే_ముగింపు

#ఒకవేళ ( ( configUSE_RECURSIVE_MUTEXES == 1 ) && ( configUSE_MUTEXES != 1 ) )
    #దోషం configUSE_MUTEXES must be సమితి గమ్యం 1 గమ్యం use recursive mutexes
#అయితే_ముగింపు

#ఒకవేళ ( ( config_నిరంతరం_గుణిజం_PRIORITIES == 0 ) && ( configUSE_TASK_PREEMPTION_DISABLE != 0 ) )
    #దోషం config_నిరంతరం_గుణిజం_PRIORITIES must be సమితి గమ్యం 1 గమ్యం use task preemption disable
#అయితే_ముగింపు

#ఒకవేళ ( ( configUSE_PREEMPTION == 0 ) && ( configUSE_TASK_PREEMPTION_DISABLE != 0 ) )
    #దోషం configUSE_PREEMPTION must be సమితి గమ్యం 1 గమ్యం use task preemption disable
#అయితే_ముగింపు

#ఒకవేళ ( ( config_సంఖ్య_యొక్క_CORES == 1 ) && ( configUSE_TASK_PREEMPTION_DISABLE != 0 ) )
    #దోషం configUSE_TASK_PREEMPTION_DISABLE is not supported లోపల single core FreeRTOS
#అయితే_ముగింపు

#ఒకవేళ ( ( config_సంఖ్య_యొక్క_CORES == 1 ) && ( configUSE_CORE_AFFINITY != 0 ) )
    #దోషం configUSE_CORE_AFFINITY is not supported లోపల single core FreeRTOS
#అయితే_ముగింపు

#ఒకవేళ ( ( config_సంఖ్య_యొక్క_CORES > 1 ) && ( config_USE_పోర్టు_OPTIMISED_TASK_SELECTION != 0 ) )
    #దోషం config_USE_పోర్టు_OPTIMISED_TASK_SELECTION is not supported లోపల SMP FreeRTOS
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే config_INITIAL_TICK_లెక్క
    #నిర్వచించు config_INITIAL_TICK_లెక్క    0
#అయితే_ముగింపు

#ఒకవేళ ( పోర్టు_TICK_TYPE_IS_ATOMIC == 0 )

/* Either variables of tick type cannot be read atomically, or
 * portTICK_TYPE_IS_ATOMIC was not set - map the critical sections used when
 * the tick count is returned to the standard critical section macros. */
    #నిర్వచించు పోర్టు_TICK_TYPE_ENTER_CRITICAL()                      పోర్టు_ENTER_CRITICAL()
    #నిర్వచించు పోర్టు_TICK_TYPE_EXIT_CRITICAL()                       పోర్టు_EXIT_CRITICAL()
    #నిర్వచించు పోర్టు_TICK_TYPE_సమితి_అంతరాయం_ముసుగు_FROM_ISR()         పోర్టు_సమితి_అంతరాయం_ముసుగు_FROM_ISR()
    #నిర్వచించు పోర్టు_TICK_TYPE_CLEAR_అంతరాయం_ముసుగు_FROM_ISR( x )    పోర్టు_CLEAR_అంతరాయం_ముసుగు_FROM_ISR( ( x ) )
#లేకపోతే

/* The tick type can be read atomically, so critical sections used when the
 * tick count is returned can be defined away. */
    #నిర్వచించు పోర్టు_TICK_TYPE_ENTER_CRITICAL()
    #నిర్వచించు పోర్టు_TICK_TYPE_EXIT_CRITICAL()
    #నిర్వచించు పోర్టు_TICK_TYPE_సమితి_అంతరాయం_ముసుగు_FROM_ISR()         0
    #నిర్వచించు పోర్టు_TICK_TYPE_CLEAR_అంతరాయం_ముసుగు_FROM_ISR( x )    ( శూన్య ) ( x )
#అయితే_ముగింపు /* if ( portTICK_TYPE_IS_ATOMIC == 0 ) */

/* Definitions to allow backward compatibility with FreeRTOS versions prior to
 * V8 if desired. */
#నిర్వచితం_కాకపోతే configENABLE_BACKWARD_COMPATIBILITY
    #నిర్వచించు configENABLE_BACKWARD_COMPATIBILITY    1
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే configPRINTF

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
    #నిర్వచించు configPRINTF( X )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే configMAX

/* The application writer has not provided their own MAX macro, so define
 * the following generic implementation. */
    #నిర్వచించు configMAX( a, b )    ( ( ( a ) > ( b ) ) ? ( a ) : ( b ) )
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే configMIN

/* The application writer has not provided their own MIN macro, so define
 * the following generic implementation. */
    #నిర్వచించు configMIN( a, b )    ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )
#అయితే_ముగింపు

#ఒకవేళ configENABLE_BACKWARD_COMPATIBILITY == 1
    #నిర్వచించు eTaskStateGet                 eTaskGetState
    #నిర్వచించు పోర్టు_Tick_Type                  TickType_t
    #నిర్వచించు xTaskHandle                   TaskHandle_t
    #నిర్వచించు x_వరుస_Handle                  వరుస_Handle_t
    #నిర్వచించు xSemaphoreHandle              SemaphoreHandle_t
    #నిర్వచించు x_వరుస_సమితి_Handle               వరుస_సమితి_Handle_t
    #నిర్వచించు x_వరుస_సమితి_Member_Handle         వరుస_సమితి_Member_Handle_t
    #నిర్వచించు x_సమయం_బహిర్గతం_Type                  సమయం_బహిర్గతం_t
    #నిర్వచించు x_జ్ఞాపకం_Region                 జ్ఞాపకం_Region_t
    #నిర్వచించు xTaskParameters               TaskParameters_t
    #నిర్వచించు x_Task_స్థితి_Type               Task_స్థితి_t
    #నిర్వచించు xTimerHandle                  TimerHandle_t
    #నిర్వచించు xCoRoutineHandle              CoRoutineHandle_t
    #నిర్వచించు pd_TASK_HOOK_సంకేతం              TaskHookFunction_t
    #నిర్వచించు పోర్టు_TICK_రేటు_MS              పోర్టు_TICK_PERIOD_MS
    #నిర్వచించు pcTaskGetTaskName             pcTaskGetName
    #నిర్వచించు pcTimerGetTimerName           pcTimerGetName
    #నిర్వచించు pc_వరుస_Get_వరుస_Name           pc_వరుస_Get_Name
    #నిర్వచించు vTaskGetTaskInfo              vTaskGetInfo
    #నిర్వచించు x_Task_Get_Idle_నిరంతరం_సమయం_Counter    ul_Task_Get_Idle_నిరంతరం_సమయం_Counter

/* Backward compatibility within the scheduler code only - these definitions
 * are not really required but are included for completeness. */
    #నిర్వచించు tmrTIMER_CALLBACK             TimerCallbackFunction_t
    #నిర్వచించు pd_TASK_సంకేతం                   TaskFunction_t
    #నిర్వచించు x_List_అంశం                     List_అంశం_t
    #నిర్వచించు xList                         List_t

/* For libraries that break the list data hiding, and access list structure
 * members directly (which is not supposed to be done). */
    #నిర్వచించు pxContainer                   pvContainer
#అయితే_ముగింపు /* configENABLE_BACKWARD_COMPATIBILITY */

#ఒకవేళ ( configUSE_ALTERNATIVE_API != 0 )
    #దోషం The alternative API was deprecated some time ago, and was removed లోపల FreeRTOS V9.0 0
#అయితే_ముగింపు

/* Set configUSE_TASK_FPU_SUPPORT to 0 to omit floating point support even
 * if floating point hardware is otherwise supported by the FreeRTOS port in use.
 * This constant is not supported by all FreeRTOS ports that include floating
 * point support. */
#నిర్వచితం_కాకపోతే configUSE_TASK_FPU_SUPPORT
    #నిర్వచించు configUSE_TASK_FPU_SUPPORT    1
#అయితే_ముగింపు

/* Set configENABLE_MPU to 1 to enable MPU support and 0 to disable it. This is
 * currently used in ARMv8M ports. */
#నిర్వచితం_కాకపోతే configENABLE_MPU
    #నిర్వచించు configENABLE_MPU    0
#అయితే_ముగింపు

/* Set configENABLE_FPU to 1 to enable FPU support and 0 to disable it. This is
 * currently used in ARMv8M ports. */
#నిర్వచితం_కాకపోతే configENABLE_FPU
    #నిర్వచించు configENABLE_FPU    1
#అయితే_ముగింపు

/* Set configENABLE_MVE to 1 to enable MVE support and 0 to disable it. This is
 * currently used in ARMv8M ports. */
#నిర్వచితం_కాకపోతే configENABLE_MVE
    #నిర్వచించు configENABLE_MVE    0
#అయితే_ముగింపు

/* Set configENABLE_TRUSTZONE to 1 enable TrustZone support and 0 to disable it.
 * This is currently used in ARMv8M ports. */
#నిర్వచితం_కాకపోతే configENABLE_TRUSTZONE
    #నిర్వచించు configENABLE_TRUSTZONE    1
#అయితే_ముగింపు

/* Set configRUN_FREERTOS_SECURE_ONLY to 1 to run the FreeRTOS ARMv8M port on
 * the Secure Side only. */
#నిర్వచితం_కాకపోతే config_నిరంతరం_FREERTOS_SECURE_ONLY
    #నిర్వచించు config_నిరంతరం_FREERTOS_SECURE_ONLY    0
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే config_నిరంతరం_ADDITIONAL_TESTS
    #నిర్వచించు config_నిరంతరం_ADDITIONAL_TESTS    0
#అయితే_ముగింపు

/* The following config allows infinite loop control. For example, control the
 * infinite loop in idle task function when performing unit tests. */
#నిర్వచితం_కాకపోతే configCONTROL_INFINITE_LOOP
    #నిర్వచించు configCONTROL_INFINITE_LOOP()
#అయితే_ముగింపు

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
#నిర్వచించు tsk_STATIC_AND_DYNAMIC_కేటాయింపు_POSSIBLE                                                                                     \
    ( ( ( పోర్టు_USING_MPU_WRAPPERS == 0 ) && ( config_SUPPORT_DYNAMIC_కేటాయింపు == 1 ) && ( config_SUPPORT_STATIC_కేటాయింపు == 1 ) ) || \
      ( ( పోర్టు_USING_MPU_WRAPPERS == 1 ) && ( config_SUPPORT_DYNAMIC_కేటాయింపు == 1 ) ) )

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
నిర్మాణం x_STATIC_LIST_అంశం
{
    #ఒకవేళ ( config_USE_LIST_దత్తాంశం_INTEGRITY_CHECK_బైట్లు == 1 )
        TickType_t xDummy1;
    #అయితే_ముగింపు
    TickType_t xDummy2;
    శూన్య * pvDummy3[ 4 ];
    #ఒకవేళ ( config_USE_LIST_దత్తాంశం_INTEGRITY_CHECK_బైట్లు == 1 )
        TickType_t xDummy4;
    #అయితే_ముగింపు
};
రకనిర్వచనం నిర్మాణం x_STATIC_LIST_అంశం Static_List_అంశం_t;

#ఒకవేళ ( config_USE_MINI_LIST_అంశం == 1 )
    /* See the comments above the struct xSTATIC_LIST_ITEM definition. */
    నిర్మాణం x_STATIC_MINI_LIST_అంశం
    {
        #ఒకవేళ ( config_USE_LIST_దత్తాంశం_INTEGRITY_CHECK_బైట్లు == 1 )
            TickType_t xDummy1;
        #అయితే_ముగింపు
        TickType_t xDummy2;
        శూన్య * pvDummy3[ 2 ];
    };
    రకనిర్వచనం నిర్మాణం x_STATIC_MINI_LIST_అంశం Static_Mini_List_అంశం_t;
#లేకపోతే /* if ( configUSE_MINI_LIST_ITEM == 1 ) */
    రకనిర్వచనం నిర్మాణం x_STATIC_LIST_అంశం      Static_Mini_List_అంశం_t;
#అయితే_ముగింపు /* if ( configUSE_MINI_LIST_ITEM == 1 ) */

/* See the comments above the struct xSTATIC_LIST_ITEM definition. */
రకనిర్వచనం నిర్మాణం xSTATIC_LIST
{
    #ఒకవేళ ( config_USE_LIST_దత్తాంశం_INTEGRITY_CHECK_బైట్లు == 1 )
        TickType_t xDummy1;
    #అయితే_ముగింపు
    UBaseType_t uxDummy2;
    శూన్య * pvDummy3;
    Static_Mini_List_అంశం_t xDummy4;
    #ఒకవేళ ( config_USE_LIST_దత్తాంశం_INTEGRITY_CHECK_బైట్లు == 1 )
        TickType_t xDummy5;
    #అయితే_ముగింపు
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
రకనిర్వచనం నిర్మాణం xSTATIC_TCB
{
    శూన్య * pxDummy1;
    #ఒకవేళ ( పోర్టు_USING_MPU_WRAPPERS == 1 )
        xMPU_SETTINGS xDummy2;
    #అయితే_ముగింపు
    #ఒకవేళ ( configUSE_CORE_AFFINITY == 1 ) && ( config_సంఖ్య_యొక్క_CORES > 1 )
        UBaseType_t uxDummy26;
    #అయితే_ముగింపు
    Static_List_అంశం_t xDummy3[ 2 ];
    UBaseType_t uxDummy5;
    శూన్య * pxDummy6;
    #ఒకవేళ ( config_సంఖ్య_యొక్క_CORES > 1 )
        పునాది_Type_t xDummy23;
        UBaseType_t uxDummy24;
    #అయితే_ముగింపు
    uint8_t ucDummy7[ configMAX_TASK_NAME_LEN ];
    #ఒకవేళ ( configUSE_TASK_PREEMPTION_DISABLE == 1 )
        పునాది_Type_t xDummy25;
    #అయితే_ముగింపు
    #ఒకవేళ ( ( పోర్టు_దొంతర_GROWTH > 0 ) || ( config_రికార్డు_దొంతర_ఎగువ_చిరునామా == 1 ) )
        శూన్య * pxDummy8;
    #అయితే_ముగింపు
    #ఒకవేళ ( పోర్టు_CRITICAL_NESTING_లోపల_TCB == 1 )
        UBaseType_t uxDummy9;
    #అయితే_ముగింపు
    #ఒకవేళ ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy10[ 2 ];
    #అయితే_ముగింపు
    #ఒకవేళ ( configUSE_MUTEXES == 1 )
        UBaseType_t uxDummy12[ 2 ];
    #అయితే_ముగింపు
    #ఒకవేళ ( config_USE_APPLICATION_TASK_ట్యాగ్ == 1 )
        శూన్య * pxDummy14;
    #అయితే_ముగింపు
    #ఒకవేళ ( config_అంకె_THREAD_LOCAL_STORAGE_సూచికలు > 0 )
        శూన్య * pvDummy15[ config_అంకె_THREAD_LOCAL_STORAGE_సూచికలు ];
    #అయితే_ముగింపు
    #ఒకవేళ ( config_GENERATE_నిరంతరం_సమయం_STATS == 1 )
        config_నిరంతరం_సమయం_COUNTER_TYPE ulDummy16;
    #అయితే_ముగింపు
    #ఒకవేళ ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 )
        config_TLS_ఖండం_TYPE xDummy17;
    #అయితే_ముగింపు
    #ఒకవేళ ( configUSE_TASK_NOTIFICATIONS == 1 )
        uint32_t ulDummy18[ configTASK_NOTIFICATION_ARRAY_ENTRIES ];
        uint8_t ucDummy19[ configTASK_NOTIFICATION_ARRAY_ENTRIES ];
    #అయితే_ముగింపు
    #ఒకవేళ ( tsk_STATIC_AND_DYNAMIC_కేటాయింపు_POSSIBLE != 0 )
        uint8_t uxDummy20;
    #అయితే_ముగింపు

    #ఒకవేళ ( INCLUDE_xTaskAbortDelay == 1 )
        uint8_t ucDummy21;
    #అయితే_ముగింపు
    #ఒకవేళ ( configUSE_POSIX_ERRNO == 1 )
        పూర్ణాంకం iDummy22;
    #అయితే_ముగింపు
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
రకనిర్వచనం నిర్మాణం x_STATIC_వరుస
{
    శూన్య * pvDummy1[ 3 ];

    సంఘం
    {
        శూన్య * pvDummy2;
        UBaseType_t uxDummy2;
    } u;

    StaticList_t xDummy3[ 2 ];
    UBaseType_t uxDummy4[ 3 ];
    uint8_t ucDummy5[ 2 ];

    #ఒకవేళ ( ( config_SUPPORT_STATIC_కేటాయింపు == 1 ) && ( config_SUPPORT_DYNAMIC_కేటాయింపు == 1 ) )
        uint8_t ucDummy6;
    #అయితే_ముగింపు

    #ఒకవేళ ( config_USE_వరుస_SETS == 1 )
        శూన్య * pvDummy7;
    #అయితే_ముగింపు

    #ఒకవేళ ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy8;
        uint8_t ucDummy9;
    #అయితే_ముగింపు
} Static_వరుస_t;
రకనిర్వచనం Static_వరుస_t StaticSemaphore_t;

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
రకనిర్వచనం నిర్మాణం x_STATIC_EVENT_సమూహం
{
    TickType_t xDummy1;
    StaticList_t xDummy2;

    #ఒకవేళ ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy3;
    #అయితే_ముగింపు

    #ఒకవేళ ( ( config_SUPPORT_STATIC_కేటాయింపు == 1 ) && ( config_SUPPORT_DYNAMIC_కేటాయింపు == 1 ) )
        uint8_t ucDummy4;
    #అయితే_ముగింపు
} Static_Event_సమూహం_t;

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
రకనిర్వచనం నిర్మాణం xSTATIC_TIMER
{
    శూన్య * pvDummy1;
    Static_List_అంశం_t xDummy2;
    TickType_t xDummy3;
    శూన్య * pvDummy5;
    TaskFunction_t pvDummy6;
    #ఒకవేళ ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy7;
    #అయితే_ముగింపు
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
రకనిర్వచనం నిర్మాణం x_STATIC_ప్రవాహం_బఫరు
{
    size_t uxDummy1[ 4 ];
    శూన్య * pvDummy2[ 3 ];
    uint8_t ucDummy3;
    #ఒకవేళ ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy4;
    #అయితే_ముగింపు
    #ఒకవేళ ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        శూన్య * pvDummy5[ 2 ];
    #అయితే_ముగింపు
    UBaseType_t uxDummy6;
} Static_ప్రవాహం_బఫరు_t;

/* Message buffers are built on stream buffers. */
రకనిర్వచనం Static_ప్రవాహం_బఫరు_t Static_సందేశం_బఫరు_t;

/* *INDENT-OFF* */
#నిర్వచితమైతే __cplusplus
    }
#అయితే_ముగింపు
/* *INDENT-ON* */

/*-----------------------------------------------------------
* IDF Compatibility
*----------------------------------------------------------*/

#నిర్వచితమైతే ESP_PLATFORM

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
    #అన్తర్భావ "freertos/idf_additions.h"

#అయితే_ముగింపు /* ESP_PLATFORM */

#అయితే_ముగింపు /* INC_FREERTOS_H */
