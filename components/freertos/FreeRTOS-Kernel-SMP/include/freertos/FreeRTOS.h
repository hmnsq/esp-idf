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

/* Acceptable values for configTICK_TYPE_WIDTH_IN_BITS. */
#تعریف_کرو TICK_TYPE_چوڑائی_16_بٹ    0
#تعریف_کرو TICK_TYPE_چوڑائی_32_بٹ    1
#تعریف_کرو TICK_TYPE_چوڑائی_64_بٹ    2

/* Application specific configuration options. */
#شامل "FreeRTOSConfig.h"

#اگر !defined( config_USE_16_ثنائی_ہندسہ_TICKS ) && !defined( config_TICK_TYPE_چوڑائی_اندر_بٹ )
    #غلطی Missing definition:  One کا config_USE_16_ثنائی_ہندسہ_TICKS and config_TICK_TYPE_چوڑائی_اندر_بٹ must be defined اندر FreeRTOSConfig.h.  See the Configuration section کا the FreeRTOS API documentation برائے details.
#اگر_ختم

#اگر defined( config_USE_16_ثنائی_ہندسہ_TICKS ) && defined( config_TICK_TYPE_چوڑائی_اندر_بٹ )
    #غلطی Only one کا config_USE_16_ثنائی_ہندسہ_TICKS and config_TICK_TYPE_چوڑائی_اندر_بٹ must be defined اندر FreeRTOSConfig.h.  See the Configuration section کا the FreeRTOS API documentation برائے details.
#اگر_ختم

/* Define configTICK_TYPE_WIDTH_IN_BITS according to the
 * value of configUSE_16_BIT_TICKS for backward compatibility. */
#اگر_غیر_تعریف_شدہ config_TICK_TYPE_چوڑائی_اندر_بٹ
    #اگر ( config_USE_16_ثنائی_ہندسہ_TICKS == 1 )
        #تعریف_کرو config_TICK_TYPE_چوڑائی_اندر_بٹ    TICK_TYPE_چوڑائی_16_بٹ
    #ورنہ
        #تعریف_کرو config_TICK_TYPE_چوڑائی_اندر_بٹ    TICK_TYPE_چوڑائی_32_بٹ
    #اگر_ختم
#اگر_ختم

/* Set configUSE_MPU_WRAPPERS_V1 to 1 to use MPU wrappers v1. */
#اگر_غیر_تعریف_شدہ configUSE_MPU_WRAPPERS_V1
    #تعریف_کرو configUSE_MPU_WRAPPERS_V1    0
#اگر_ختم

/* Set configENABLE_ACCESS_CONTROL_LIST to 1 to enable access control list support. */
#اگر_غیر_تعریف_شدہ configENABLE_ACCESS_CONTROL_LIST
    #تعریف_کرو configENABLE_ACCESS_CONTROL_LIST    0
#اگر_ختم

/* Set default value of configNUMBER_OF_CORES to 1 to use single core FreeRTOS. */
#اگر_غیر_تعریف_شدہ config_نمبر_کا_CORES
    #تعریف_کرو config_نمبر_کا_CORES    1
#اگر_ختم

#اگر_غیر_تعریف_شدہ configUSE_MALLOC_FAILED_HOOK
    #تعریف_کرو configUSE_MALLOC_FAILED_HOOK    0
#اگر_ختم

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

    #شامل "newlib-freertos.h"

#اگر_ختم /* if ( configUSE_NEWLIB_REENTRANT == 1 ) */

/* Must be defaulted before configUSE_PICOLIBC_TLS is used below. */
#اگر_غیر_تعریف_شدہ configUSE_PICOLIBC_TLS
    #تعریف_کرو configUSE_PICOLIBC_TLS    0
#اگر_ختم

#اگر ( configUSE_PICOLIBC_TLS == 1 )

    #شامل "picolibc-freertos.h"

#اگر_ختم /* if ( configUSE_PICOLIBC_TLS == 1 ) */

#اگر_غیر_تعریف_شدہ configUSE_C_RUNTIME_TLS_SUPPORT
    #تعریف_کرو configUSE_C_RUNTIME_TLS_SUPPORT    0
#اگر_ختم

#اگر ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 )

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
#اگر_ختم /* if ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 ) */

/*
 * Check all the required application specific macros have been defined.
 * These macros are application specific and (as downloaded) are defined
 * within FreeRTOSConfig.h.
 */

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

#اگر ( config_نمبر_کا_CORES > 1 )
    #اگر_غیر_تعریف_شدہ configUSE_PASSIVE_IDLE_HOOK
        #غلطی Missing definition:  configUSE_PASSIVE_IDLE_HOOK must be defined اندر FreeRTOSConfig.h as either 1 or 0.  See the Configuration section کا the FreeRTOS API documentation برائے details.
    #اگر_ختم
#اگر_ختم

#اگر_غیر_تعریف_شدہ configUSE_TICK_HOOK
    #غلطی Missing definition:  configUSE_TICK_HOOK must be defined اندر FreeRTOSConfig.h as either 1 or 0.  See the Configuration section کا the FreeRTOS API documentation برائے details.
#اگر_ختم

#اگر ( ( config_TICK_TYPE_چوڑائی_اندر_بٹ != TICK_TYPE_چوڑائی_16_بٹ ) && \
    ( config_TICK_TYPE_چوڑائی_اندر_بٹ != TICK_TYPE_چوڑائی_32_بٹ ) &&   \
    ( config_TICK_TYPE_چوڑائی_اندر_بٹ != TICK_TYPE_چوڑائی_64_بٹ ) )
    #غلطی Macro config_TICK_TYPE_چوڑائی_اندر_بٹ is defined مقصد incorrect قدر.  See the Configuration section کا the FreeRTOS API documentation برائے details.
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

#اگر_غیر_تعریف_شدہ configUSE_EVENT_GROUPS
    #تعریف_کرو configUSE_EVENT_GROUPS    1
#اگر_ختم

#اگر_غیر_تعریف_شدہ config_USE_دھارا_BUFFERS
    #تعریف_کرو config_USE_دھارا_BUFFERS    1
#اگر_ختم

#اگر_غیر_تعریف_شدہ configUSE_DAEMON_TASK_STARTUP_HOOK
    #تعریف_کرو configUSE_DAEMON_TASK_STARTUP_HOOK    0
#اگر_ختم

#اگر ( configUSE_DAEMON_TASK_STARTUP_HOOK != 0 )
    #اگر ( configUSE_TIMERS == 0 )
        #غلطی configUSE_DAEMON_TASK_STARTUP_HOOK is سیٹ, but the daemon task is not created because configUSE_TIMERS is 0.
    #اگر_ختم
#اگر_ختم

#اگر_غیر_تعریف_شدہ configUSE_COUNTING_SEMAPHORES
    #تعریف_کرو configUSE_COUNTING_SEMAPHORES    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ configUSE_TASK_PREEMPTION_DISABLE
    #تعریف_کرو configUSE_TASK_PREEMPTION_DISABLE    0
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

#اگر_غیر_تعریف_شدہ configCHECK_HANDLER_INSTALLATION
    #تعریف_کرو configCHECK_HANDLER_INSTALLATION    1
#ورنہ

/* The application has explicitly defined configCHECK_HANDLER_INSTALLATION
 * to 1. The checks requires configASSERT() to be defined. */
    #اگر ( ( configCHECK_HANDLER_INSTALLATION == 1 ) && ( configASSERT_DEFINED == 0 ) )
        #غلطی You must define configASSERT() when configCHECK_HANDLER_INSTALLATION is 1.
    #اگر_ختم
#اگر_ختم

#اگر_غیر_تعریف_شدہ پورٹ_حافظہ_BARRIER
    #تعریف_کرو پورٹ_حافظہ_BARRIER()
#اگر_ختم

#اگر_غیر_تعریف_شدہ پورٹ_SOFTWARE_BARRIER
    #تعریف_کرو پورٹ_SOFTWARE_BARRIER()
#اگر_ختم

#اگر_غیر_تعریف_شدہ config_تسلسل_مضرب_PRIORITIES
    #تعریف_کرو config_تسلسل_مضرب_PRIORITIES    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ پورٹ_GET_CORE_ID

    #اگر ( config_نمبر_کا_CORES == 1 )
        #تعریف_کرو پورٹ_GET_CORE_ID()    0
    #ورنہ
        #غلطی config_نمبر_کا_CORES is سیٹ مقصد مزید than 1 then پورٹ_GET_CORE_ID must also be defined.
    #اگر_ختم /* configNUMBER_OF_CORES */

#اگر_ختم /* portGET_CORE_ID */

#اگر_غیر_تعریف_شدہ پورٹ_YIELD_CORE

    #اگر ( config_نمبر_کا_CORES == 1 )
        #تعریف_کرو پورٹ_YIELD_CORE( x )    پورٹ_YIELD()
    #ورنہ
        #غلطی config_نمبر_کا_CORES is سیٹ مقصد مزید than 1 then پورٹ_YIELD_CORE must also be defined.
    #اگر_ختم /* configNUMBER_OF_CORES */

#اگر_ختم /* portYIELD_CORE */

#اگر_غیر_تعریف_شدہ پورٹ_سیٹ_مداخلت_نقاب

    #اگر ( config_نمبر_کا_CORES > 1 )
        #غلطی پورٹ_سیٹ_مداخلت_نقاب is required اندر SMP
    #اگر_ختم

#اگر_ختم /* portSET_INTERRUPT_MASK */

#اگر_غیر_تعریف_شدہ پورٹ_CLEAR_مداخلت_نقاب

    #اگر ( config_نمبر_کا_CORES > 1 )
        #غلطی پورٹ_CLEAR_مداخلت_نقاب is required اندر SMP
    #اگر_ختم

#اگر_ختم /* portCLEAR_INTERRUPT_MASK */

#اگر_غیر_تعریف_شدہ پورٹ_RELEASE_TASK_تالا

    #اگر ( config_نمبر_کا_CORES == 1 )
        #تعریف_کرو پورٹ_RELEASE_TASK_تالا()
    #ورنہ
        #غلطی پورٹ_RELEASE_TASK_تالا is required اندر SMP
    #اگر_ختم

#اگر_ختم /* portRELEASE_TASK_LOCK */

#اگر_غیر_تعریف_شدہ پورٹ_GET_TASK_تالا

    #اگر ( config_نمبر_کا_CORES == 1 )
        #تعریف_کرو پورٹ_GET_TASK_تالا()
    #ورنہ
        #غلطی پورٹ_GET_TASK_تالا is required اندر SMP
    #اگر_ختم

#اگر_ختم /* portGET_TASK_LOCK */

#اگر_غیر_تعریف_شدہ پورٹ_RELEASE_ISR_تالا

    #اگر ( config_نمبر_کا_CORES == 1 )
        #تعریف_کرو پورٹ_RELEASE_ISR_تالا()
    #ورنہ
        #غلطی پورٹ_RELEASE_ISR_تالا is required اندر SMP
    #اگر_ختم

#اگر_ختم /* portRELEASE_ISR_LOCK */

#اگر_غیر_تعریف_شدہ پورٹ_GET_ISR_تالا

    #اگر ( config_نمبر_کا_CORES == 1 )
        #تعریف_کرو پورٹ_GET_ISR_تالا()
    #ورنہ
        #غلطی پورٹ_GET_ISR_تالا is required اندر SMP
    #اگر_ختم

#اگر_ختم /* portGET_ISR_LOCK */

#اگر_غیر_تعریف_شدہ پورٹ_ENTER_CRITICAL_FROM_ISR

    #اگر ( config_نمبر_کا_CORES > 1 )
        #غلطی پورٹ_ENTER_CRITICAL_FROM_ISR is required اندر SMP
    #اگر_ختم

#اگر_ختم

#اگر_غیر_تعریف_شدہ پورٹ_EXIT_CRITICAL_FROM_ISR

    #اگر ( config_نمبر_کا_CORES > 1 )
        #غلطی پورٹ_EXIT_CRITICAL_FROM_ISR is required اندر SMP
    #اگر_ختم

#اگر_ختم

#اگر_غیر_تعریف_شدہ configUSE_CORE_AFFINITY
    #تعریف_کرو configUSE_CORE_AFFINITY    0
#اگر_ختم /* configUSE_CORE_AFFINITY */

#اگر ( ( config_نمبر_کا_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
    #اگر_غیر_تعریف_شدہ configTASK_DEFAULT_CORE_AFFINITY
        #تعریف_کرو configTASK_DEFAULT_CORE_AFFINITY    tskNO_AFFINITY
    #اگر_ختم
#اگر_ختم

#اگر_غیر_تعریف_شدہ configUSE_PASSIVE_IDLE_HOOK
    #تعریف_کرو configUSE_PASSIVE_IDLE_HOOK    0
#اگر_ختم /* configUSE_PASSIVE_IDLE_HOOK */

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

    #اگر_غیر_تعریف_شدہ پورٹ_TIMER_CALLBACK_ATTRIBUTE
        #تعریف_کرو پورٹ_TIMER_CALLBACK_ATTRIBUTE
    #اگر_ختم /* portTIMER_CALLBACK_ATTRIBUTE */

#اگر_ختم /* configUSE_TIMERS */

#اگر_غیر_تعریف_شدہ پورٹ_HAS_NESTED_INTERRUPTS
    #اگر defined( پورٹ_سیٹ_مداخلت_نقاب_FROM_ISR ) && defined( پورٹ_CLEAR_مداخلت_نقاب_FROM_ISR )
        #تعریف_کرو پورٹ_HAS_NESTED_INTERRUPTS    1
    #ورنہ
        #تعریف_کرو پورٹ_HAS_NESTED_INTERRUPTS    0
    #اگر_ختم
#اگر_ختم

#اگر_غیر_تعریف_شدہ پورٹ_سیٹ_مداخلت_نقاب_FROM_ISR
    #اگر ( پورٹ_HAS_NESTED_INTERRUPTS == 1 )
        #غلطی پورٹ_سیٹ_مداخلت_نقاب_FROM_ISR must be defined برائے ports that support nested interrupts (i.e. پورٹ_HAS_NESTED_INTERRUPTS is سیٹ مقصد 1)
    #ورنہ
        #تعریف_کرو پورٹ_سیٹ_مداخلت_نقاب_FROM_ISR()    0
    #اگر_ختم
#ورنہ
    #اگر ( پورٹ_HAS_NESTED_INTERRUPTS == 0 )
        #غلطی پورٹ_سیٹ_مداخلت_نقاب_FROM_ISR must not be defined برائے ports that کرو not support nested interrupts (i.e. پورٹ_HAS_NESTED_INTERRUPTS is سیٹ مقصد 0)
    #اگر_ختم
#اگر_ختم

#اگر_غیر_تعریف_شدہ پورٹ_CLEAR_مداخلت_نقاب_FROM_ISR
    #اگر ( پورٹ_HAS_NESTED_INTERRUPTS == 1 )
        #غلطی پورٹ_CLEAR_مداخلت_نقاب_FROM_ISR must be defined برائے ports that support nested interrupts  (i.e. پورٹ_HAS_NESTED_INTERRUPTS is سیٹ مقصد 1)
    #ورنہ
        #تعریف_کرو پورٹ_CLEAR_مداخلت_نقاب_FROM_ISR( ux_Saved_حالت_قدر )    ( خالی ) ( ux_Saved_حالت_قدر )
    #اگر_ختم
#ورنہ
    #اگر ( پورٹ_HAS_NESTED_INTERRUPTS == 0 )
        #غلطی پورٹ_CLEAR_مداخلت_نقاب_FROM_ISR must not be defined برائے ports that کرو not support nested interrupts (i.e. پورٹ_HAS_NESTED_INTERRUPTS is سیٹ مقصد 0)
    #اگر_ختم
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

#اگر_غیر_تعریف_شدہ پورٹ_TASK_SWITCH_HOOK
    #تعریف_کرو پورٹ_TASK_SWITCH_HOOK( pxTCB )    ( خالی ) ( pxTCB )
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

#اگر_غیر_تعریف_شدہ trace_MOVED_TASK_مقصد_DELAYED_LIST
    #تعریف_کرو trace_MOVED_TASK_مقصد_DELAYED_LIST()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_MOVED_TASK_مقصد_حد_سے_تجاوز_DELAYED_LIST
    #تعریف_کرو trace_MOVED_TASK_مقصد_حد_سے_تجاوز_DELAYED_LIST()
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

#اگر_غیر_تعریف_شدہ trace_ISR_EXIT_مقصد_SCHEDULER
    #تعریف_کرو trace_ISR_EXIT_مقصد_SCHEDULER()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceISR_EXIT
    #تعریف_کرو traceISR_EXIT()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceISR_ENTER
    #تعریف_کرو traceISR_ENTER()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_دھارا_بفر_CREATE_FAILED
    #تعریف_کرو trace_دھارا_بفر_CREATE_FAILED( x_دھارا_بفر_Type )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_دھارا_بفر_CREATE_STATIC_FAILED
    #تعریف_کرو trace_دھارا_بفر_CREATE_STATIC_FAILED( xReturn, x_دھارا_بفر_Type )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_دھارا_بفر_CREATE
    #تعریف_کرو trace_دھارا_بفر_CREATE( px_دھارا_بفر, x_دھارا_بفر_Type )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_دھارا_بفر_DELETE
    #تعریف_کرو trace_دھارا_بفر_DELETE( x_دھارا_بفر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_دھارا_بفر_RESET
    #تعریف_کرو trace_دھارا_بفر_RESET( x_دھارا_بفر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_دھارا_بفر_RESET_FROM_ISR
    #تعریف_کرو trace_دھارا_بفر_RESET_FROM_ISR( x_دھارا_بفر )
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

#اگر_غیر_تعریف_شدہ trace_ENTER_x_Event_گروہ_Create_Static
    #تعریف_کرو trace_ENTER_x_Event_گروہ_Create_Static( px_Event_گروہ_بفر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_Event_گروہ_Create_Static
    #تعریف_کرو trace_RETURN_x_Event_گروہ_Create_Static( px_Event_بٹ )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_Event_گروہ_Create
    #تعریف_کرو trace_ENTER_x_Event_گروہ_Create()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_Event_گروہ_Create
    #تعریف_کرو trace_RETURN_x_Event_گروہ_Create( px_Event_بٹ )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_Event_گروہ_Sync
    #تعریف_کرو trace_ENTER_x_Event_گروہ_Sync( x_Event_گروہ, ux_بٹ_مقصد_سیٹ, ux_بٹ_مقصد_Wait_For, x_Ticks_مقصد_Wait )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_Event_گروہ_Sync
    #تعریف_کرو trace_RETURN_x_Event_گروہ_Sync( uxReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_Event_گروہ_Wait_بٹ
    #تعریف_کرو trace_ENTER_x_Event_گروہ_Wait_بٹ( x_Event_گروہ, ux_بٹ_مقصد_Wait_For, xClearOnExit, x_Wait_For_All_بٹ, x_Ticks_مقصد_Wait )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_Event_گروہ_Wait_بٹ
    #تعریف_کرو trace_RETURN_x_Event_گروہ_Wait_بٹ( uxReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_Event_گروہ_Clear_بٹ
    #تعریف_کرو trace_ENTER_x_Event_گروہ_Clear_بٹ( x_Event_گروہ, ux_بٹ_مقصد_Clear )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_Event_گروہ_Clear_بٹ
    #تعریف_کرو trace_RETURN_x_Event_گروہ_Clear_بٹ( uxReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_Event_گروہ_Clear_بٹ_From_ISR
    #تعریف_کرو trace_ENTER_x_Event_گروہ_Clear_بٹ_From_ISR( x_Event_گروہ, ux_بٹ_مقصد_Clear )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_Event_گروہ_Clear_بٹ_From_ISR
    #تعریف_کرو trace_RETURN_x_Event_گروہ_Clear_بٹ_From_ISR( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_Event_گروہ_Get_بٹ_From_ISR
    #تعریف_کرو trace_ENTER_x_Event_گروہ_Get_بٹ_From_ISR( x_Event_گروہ )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_Event_گروہ_Get_بٹ_From_ISR
    #تعریف_کرو trace_RETURN_x_Event_گروہ_Get_بٹ_From_ISR( uxReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_Event_گروہ_سیٹ_بٹ
    #تعریف_کرو trace_ENTER_x_Event_گروہ_سیٹ_بٹ( x_Event_گروہ, ux_بٹ_مقصد_سیٹ )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_Event_گروہ_سیٹ_بٹ
    #تعریف_کرو trace_RETURN_x_Event_گروہ_سیٹ_بٹ( ux_Event_بٹ )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_v_Event_گروہ_Delete
    #تعریف_کرو trace_ENTER_v_Event_گروہ_Delete( x_Event_گروہ )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_v_Event_گروہ_Delete
    #تعریف_کرو trace_RETURN_v_Event_گروہ_Delete()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_Event_گروہ_Get_Static_بفر
    #تعریف_کرو trace_ENTER_x_Event_گروہ_Get_Static_بفر( x_Event_گروہ, ppx_Event_گروہ_بفر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_Event_گروہ_Get_Static_بفر
    #تعریف_کرو trace_RETURN_x_Event_گروہ_Get_Static_بفر( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_v_Event_گروہ_سیٹ_بٹ_Callback
    #تعریف_کرو trace_ENTER_v_Event_گروہ_سیٹ_بٹ_Callback( pv_Event_گروہ, ul_بٹ_مقصد_سیٹ )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_v_Event_گروہ_سیٹ_بٹ_Callback
    #تعریف_کرو trace_RETURN_v_Event_گروہ_سیٹ_بٹ_Callback()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_v_Event_گروہ_Clear_بٹ_Callback
    #تعریف_کرو trace_ENTER_v_Event_گروہ_Clear_بٹ_Callback( pv_Event_گروہ, ul_بٹ_مقصد_Clear )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_v_Event_گروہ_Clear_بٹ_Callback
    #تعریف_کرو trace_RETURN_v_Event_گروہ_Clear_بٹ_Callback()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_Event_گروہ_سیٹ_بٹ_From_ISR
    #تعریف_کرو trace_ENTER_x_Event_گروہ_سیٹ_بٹ_From_ISR( x_Event_گروہ, ux_بٹ_مقصد_سیٹ, px_Higher_ترجیح_Task_Woken )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_Event_گروہ_سیٹ_بٹ_From_ISR
    #تعریف_کرو trace_RETURN_x_Event_گروہ_سیٹ_بٹ_From_ISR( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_ux_Event_گروہ_Get_نمبر
    #تعریف_کرو trace_ENTER_ux_Event_گروہ_Get_نمبر( x_Event_گروہ )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_ux_Event_گروہ_Get_نمبر
    #تعریف_کرو trace_RETURN_ux_Event_گروہ_Get_نمبر( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_v_Event_گروہ_سیٹ_نمبر
    #تعریف_کرو trace_ENTER_v_Event_گروہ_سیٹ_نمبر( x_Event_گروہ, ux_Event_گروہ_نمبر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_v_Event_گروہ_سیٹ_نمبر
    #تعریف_کرو trace_RETURN_v_Event_گروہ_سیٹ_نمبر()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_قطار_Generic_Reset
    #تعریف_کرو trace_ENTER_x_قطار_Generic_Reset( x_قطار, x_New_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_قطار_Generic_Reset
    #تعریف_کرو trace_RETURN_x_قطار_Generic_Reset( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_قطار_Generic_Create_Static
    #تعریف_کرو trace_ENTER_x_قطار_Generic_Create_Static( ux_قطار_لمبائی, ux_شے_سائز, puc_قطار_Storage, px_Static_قطار, uc_قطار_Type )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_قطار_Generic_Create_Static
    #تعریف_کرو trace_RETURN_x_قطار_Generic_Create_Static( px_New_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_قطار_Generic_Get_Static_Buffers
    #تعریف_کرو trace_ENTER_x_قطار_Generic_Get_Static_Buffers( x_قطار, ppuc_قطار_Storage, ppx_Static_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_قطار_Generic_Get_Static_Buffers
    #تعریف_کرو trace_RETURN_x_قطار_Generic_Get_Static_Buffers( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_قطار_Generic_Create
    #تعریف_کرو trace_ENTER_x_قطار_Generic_Create( ux_قطار_لمبائی, ux_شے_سائز, uc_قطار_Type )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_قطار_Generic_Create
    #تعریف_کرو trace_RETURN_x_قطار_Generic_Create( px_New_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_قطار_Create_Mutex
    #تعریف_کرو trace_ENTER_x_قطار_Create_Mutex( uc_قطار_Type )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_قطار_Create_Mutex
    #تعریف_کرو trace_RETURN_x_قطار_Create_Mutex( x_New_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_قطار_Create_Mutex_Static
    #تعریف_کرو trace_ENTER_x_قطار_Create_Mutex_Static( uc_قطار_Type, px_Static_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_قطار_Create_Mutex_Static
    #تعریف_کرو trace_RETURN_x_قطار_Create_Mutex_Static( x_New_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_قطار_Get_Mutex_Holder
    #تعریف_کرو trace_ENTER_x_قطار_Get_Mutex_Holder( xSemaphore )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_قطار_Get_Mutex_Holder
    #تعریف_کرو trace_RETURN_x_قطار_Get_Mutex_Holder( pxReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_قطار_Get_Mutex_Holder_From_ISR
    #تعریف_کرو trace_ENTER_x_قطار_Get_Mutex_Holder_From_ISR( xSemaphore )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_قطار_Get_Mutex_Holder_From_ISR
    #تعریف_کرو trace_RETURN_x_قطار_Get_Mutex_Holder_From_ISR( pxReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_قطار_Give_Mutex_Recursive
    #تعریف_کرو trace_ENTER_x_قطار_Give_Mutex_Recursive( xMutex )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_قطار_Give_Mutex_Recursive
    #تعریف_کرو trace_RETURN_x_قطار_Give_Mutex_Recursive( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_قطار_Take_Mutex_Recursive
    #تعریف_کرو trace_ENTER_x_قطار_Take_Mutex_Recursive( xMutex, x_Ticks_مقصد_Wait )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_قطار_Take_Mutex_Recursive
    #تعریف_کرو trace_RETURN_x_قطار_Take_Mutex_Recursive( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_قطار_Create_Counting_Semaphore_Static
    #تعریف_کرو trace_ENTER_x_قطار_Create_Counting_Semaphore_Static( ux_Max_گنتی, ux_Initial_گنتی, px_Static_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_قطار_Create_Counting_Semaphore_Static
    #تعریف_کرو trace_RETURN_x_قطار_Create_Counting_Semaphore_Static( xHandle )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_قطار_Create_Counting_Semaphore
    #تعریف_کرو trace_ENTER_x_قطار_Create_Counting_Semaphore( ux_Max_گنتی, ux_Initial_گنتی )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_قطار_Create_Counting_Semaphore
    #تعریف_کرو trace_RETURN_x_قطار_Create_Counting_Semaphore( xHandle )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_قطار_Generic_Send
    #تعریف_کرو trace_ENTER_x_قطار_Generic_Send( x_قطار, pv_شے_مقصد_قطار, x_Ticks_مقصد_Wait, xCopyPosition )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_قطار_Generic_Send
    #تعریف_کرو trace_RETURN_x_قطار_Generic_Send( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_قطار_Generic_Send_From_ISR
    #تعریف_کرو trace_ENTER_x_قطار_Generic_Send_From_ISR( x_قطار, pv_شے_مقصد_قطار, px_Higher_ترجیح_Task_Woken, xCopyPosition )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_قطار_Generic_Send_From_ISR
    #تعریف_کرو trace_RETURN_x_قطار_Generic_Send_From_ISR( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_قطار_Give_From_ISR
    #تعریف_کرو trace_ENTER_x_قطار_Give_From_ISR( x_قطار, px_Higher_ترجیح_Task_Woken )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_قطار_Give_From_ISR
    #تعریف_کرو trace_RETURN_x_قطار_Give_From_ISR( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_قطار_Receive
    #تعریف_کرو trace_ENTER_x_قطار_Receive( x_قطار, pv_بفر, x_Ticks_مقصد_Wait )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_قطار_Receive
    #تعریف_کرو trace_RETURN_x_قطار_Receive( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_قطار_Semaphore_Take
    #تعریف_کرو trace_ENTER_x_قطار_Semaphore_Take( x_قطار, x_Ticks_مقصد_Wait )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_قطار_Semaphore_Take
    #تعریف_کرو trace_RETURN_x_قطار_Semaphore_Take( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_قطار_Peek
    #تعریف_کرو trace_ENTER_x_قطار_Peek( x_قطار, pv_بفر, x_Ticks_مقصد_Wait )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_قطار_Peek
    #تعریف_کرو trace_RETURN_x_قطار_Peek( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_قطار_Receive_From_ISR
    #تعریف_کرو trace_ENTER_x_قطار_Receive_From_ISR( x_قطار, pv_بفر, px_Higher_ترجیح_Task_Woken )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_قطار_Receive_From_ISR
    #تعریف_کرو trace_RETURN_x_قطار_Receive_From_ISR( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_قطار_Peek_From_ISR
    #تعریف_کرو trace_ENTER_x_قطار_Peek_From_ISR( x_قطار, pv_بفر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_قطار_Peek_From_ISR
    #تعریف_کرو trace_RETURN_x_قطار_Peek_From_ISR( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_ux_قطار_Messages_انتظار
    #تعریف_کرو trace_ENTER_ux_قطار_Messages_انتظار( x_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_ux_قطار_Messages_انتظار
    #تعریف_کرو trace_RETURN_ux_قطار_Messages_انتظار( uxReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_ux_قطار_Spaces_دستیاب
    #تعریف_کرو trace_ENTER_ux_قطار_Spaces_دستیاب( x_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_ux_قطار_Spaces_دستیاب
    #تعریف_کرو trace_RETURN_ux_قطار_Spaces_دستیاب( uxReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_ux_قطار_Messages_انتظار_From_ISR
    #تعریف_کرو trace_ENTER_ux_قطار_Messages_انتظار_From_ISR( x_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_ux_قطار_Messages_انتظار_From_ISR
    #تعریف_کرو trace_RETURN_ux_قطار_Messages_انتظار_From_ISR( uxReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_v_قطار_Delete
    #تعریف_کرو trace_ENTER_v_قطار_Delete( x_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_v_قطار_Delete
    #تعریف_کرو trace_RETURN_v_قطار_Delete()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_ux_قطار_Get_قطار_نمبر
    #تعریف_کرو trace_ENTER_ux_قطار_Get_قطار_نمبر( x_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_ux_قطار_Get_قطار_نمبر
    #تعریف_کرو trace_RETURN_ux_قطار_Get_قطار_نمبر( ux_قطار_نمبر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_v_قطار_سیٹ_قطار_نمبر
    #تعریف_کرو trace_ENTER_v_قطار_سیٹ_قطار_نمبر( x_قطار, ux_قطار_نمبر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_v_قطار_سیٹ_قطار_نمبر
    #تعریف_کرو trace_RETURN_v_قطار_سیٹ_قطار_نمبر()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_uc_قطار_Get_قطار_Type
    #تعریف_کرو trace_ENTER_uc_قطار_Get_قطار_Type( x_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_uc_قطار_Get_قطار_Type
    #تعریف_کرو trace_RETURN_uc_قطار_Get_قطار_Type( uc_قطار_Type )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_ux_قطار_Get_قطار_شے_سائز
    #تعریف_کرو trace_ENTER_ux_قطار_Get_قطار_شے_سائز( x_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_ux_قطار_Get_قطار_شے_سائز
    #تعریف_کرو trace_RETURN_ux_قطار_Get_قطار_شے_سائز( ux_شے_سائز )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_ux_قطار_Get_قطار_لمبائی
    #تعریف_کرو trace_ENTER_ux_قطار_Get_قطار_لمبائی( x_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_ux_قطار_Get_قطار_لمبائی
    #تعریف_کرو trace_RETURN_ux_قطار_Get_قطار_لمبائی( ux_لمبائی )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_قطار_Is_قطار_خالی_خانے_From_ISR
    #تعریف_کرو trace_ENTER_x_قطار_Is_قطار_خالی_خانے_From_ISR( x_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_قطار_Is_قطار_خالی_خانے_From_ISR
    #تعریف_کرو trace_RETURN_x_قطار_Is_قطار_خالی_خانے_From_ISR( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_قطار_Is_قطار_بھرا_From_ISR
    #تعریف_کرو trace_ENTER_x_قطار_Is_قطار_بھرا_From_ISR( x_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_قطار_Is_قطار_بھرا_From_ISR
    #تعریف_کرو trace_RETURN_x_قطار_Is_قطار_بھرا_From_ISR( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_قطار_CRSend
    #تعریف_کرو trace_ENTER_x_قطار_CRSend( x_قطار, pv_شے_مقصد_قطار, x_Ticks_مقصد_Wait )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_قطار_CRSend
    #تعریف_کرو trace_RETURN_x_قطار_CRSend( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_قطار_CRReceive
    #تعریف_کرو trace_ENTER_x_قطار_CRReceive( x_قطار, pv_بفر, x_Ticks_مقصد_Wait )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_قطار_CRReceive
    #تعریف_کرو trace_RETURN_x_قطار_CRReceive( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_قطار_CRSend_From_ISR
    #تعریف_کرو trace_ENTER_x_قطار_CRSend_From_ISR( x_قطار, pv_شے_مقصد_قطار, xCoRoutinePreviouslyWoken )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_قطار_CRSend_From_ISR
    #تعریف_کرو trace_RETURN_x_قطار_CRSend_From_ISR( xCoRoutinePreviouslyWoken )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_قطار_CRReceive_From_ISR
    #تعریف_کرو trace_ENTER_x_قطار_CRReceive_From_ISR( x_قطار, pv_بفر, pxCoRoutineWoken )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_قطار_CRReceive_From_ISR
    #تعریف_کرو trace_RETURN_x_قطار_CRReceive_From_ISR( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_v_قطار_جوڑو_مقصد_Registry
    #تعریف_کرو trace_ENTER_v_قطار_جوڑو_مقصد_Registry( x_قطار, pc_قطار_Name )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_v_قطار_جوڑو_مقصد_Registry
    #تعریف_کرو trace_RETURN_v_قطار_جوڑو_مقصد_Registry()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_pc_قطار_Get_Name
    #تعریف_کرو trace_ENTER_pc_قطار_Get_Name( x_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_pc_قطار_Get_Name
    #تعریف_کرو trace_RETURN_pc_قطار_Get_Name( pcReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_v_قطار_Unregister_قطار
    #تعریف_کرو trace_ENTER_v_قطار_Unregister_قطار( x_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_v_قطار_Unregister_قطار
    #تعریف_کرو trace_RETURN_v_قطار_Unregister_قطار()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_v_قطار_Wait_For_پیغام_Restricted
    #تعریف_کرو trace_ENTER_v_قطار_Wait_For_پیغام_Restricted( x_قطار, x_Ticks_مقصد_Wait, xWaitIndefinitely )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_v_قطار_Wait_For_پیغام_Restricted
    #تعریف_کرو trace_RETURN_v_قطار_Wait_For_پیغام_Restricted()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_قطار_Create_سیٹ
    #تعریف_کرو trace_ENTER_x_قطار_Create_سیٹ( ux_Event_قطار_لمبائی )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_قطار_Create_سیٹ
    #تعریف_کرو trace_RETURN_x_قطار_Create_سیٹ( px_قطار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_قطار_جوڑو_مقصد_سیٹ
    #تعریف_کرو trace_ENTER_x_قطار_جوڑو_مقصد_سیٹ( x_قطار_Or_Semaphore, x_قطار_سیٹ )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_قطار_جوڑو_مقصد_سیٹ
    #تعریف_کرو trace_RETURN_x_قطار_جوڑو_مقصد_سیٹ( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_قطار_Remove_From_سیٹ
    #تعریف_کرو trace_ENTER_x_قطار_Remove_From_سیٹ( x_قطار_Or_Semaphore, x_قطار_سیٹ )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_قطار_Remove_From_سیٹ
    #تعریف_کرو trace_RETURN_x_قطار_Remove_From_سیٹ( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_قطار_Select_From_سیٹ
    #تعریف_کرو trace_ENTER_x_قطار_Select_From_سیٹ( x_قطار_سیٹ, x_Ticks_مقصد_Wait )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_قطار_Select_From_سیٹ
    #تعریف_کرو trace_RETURN_x_قطار_Select_From_سیٹ( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_قطار_Select_From_سیٹ_From_ISR
    #تعریف_کرو trace_ENTER_x_قطار_Select_From_سیٹ_From_ISR( x_قطار_سیٹ )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_قطار_Select_From_سیٹ_From_ISR
    #تعریف_کرو trace_RETURN_x_قطار_Select_From_سیٹ_From_ISR( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_xTimerCreateTimerTask
    #تعریف_کرو traceENTER_xTimerCreateTimerTask()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_xTimerCreateTimerTask
    #تعریف_کرو traceRETURN_xTimerCreateTimerTask( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_xTimerCreate
    #تعریف_کرو traceENTER_xTimerCreate( pcTimerName, x_Timer_Period_اندر_Ticks, xAutoReload, pvTimerID, pxCallbackFunction )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_xTimerCreate
    #تعریف_کرو traceRETURN_xTimerCreate( pxNewTimer )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_xTimerCreateStatic
    #تعریف_کرو traceENTER_xTimerCreateStatic( pcTimerName, x_Timer_Period_اندر_Ticks, xAutoReload, pvTimerID, pxCallbackFunction, px_Timer_بفر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_xTimerCreateStatic
    #تعریف_کرو traceRETURN_xTimerCreateStatic( pxNewTimer )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_xTimerGenericCommandFromTask
    #تعریف_کرو traceENTER_xTimerGenericCommandFromTask( xTimer, xCommandID, x_Optional_قدر, px_Higher_ترجیح_Task_Woken, x_Ticks_مقصد_Wait )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_xTimerGenericCommandFromTask
    #تعریف_کرو traceRETURN_xTimerGenericCommandFromTask( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_xTimerGenericCommandFromISR
    #تعریف_کرو traceENTER_xTimerGenericCommandFromISR( xTimer, xCommandID, x_Optional_قدر, px_Higher_ترجیح_Task_Woken, x_Ticks_مقصد_Wait )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_xTimerGenericCommandFromISR
    #تعریف_کرو traceRETURN_xTimerGenericCommandFromISR( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_xTimerGetTimerDaemonTaskHandle
    #تعریف_کرو traceENTER_xTimerGetTimerDaemonTaskHandle()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_xTimerGetTimerDaemonTaskHandle
    #تعریف_کرو traceRETURN_xTimerGetTimerDaemonTaskHandle( xTimerTaskHandle )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_xTimerGetPeriod
    #تعریف_کرو traceENTER_xTimerGetPeriod( xTimer )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_xTimerGetPeriod
    #تعریف_کرو traceRETURN_xTimerGetPeriod( x_Timer_Period_اندر_Ticks )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_v_Timer_سیٹ_Reload_منوال
    #تعریف_کرو trace_ENTER_v_Timer_سیٹ_Reload_منوال( xTimer, xAutoReload )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_v_Timer_سیٹ_Reload_منوال
    #تعریف_کرو trace_RETURN_v_Timer_سیٹ_Reload_منوال()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_Timer_Get_Reload_منوال
    #تعریف_کرو trace_ENTER_x_Timer_Get_Reload_منوال( xTimer )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_Timer_Get_Reload_منوال
    #تعریف_کرو trace_RETURN_x_Timer_Get_Reload_منوال( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_ux_Timer_Get_Reload_منوال
    #تعریف_کرو trace_ENTER_ux_Timer_Get_Reload_منوال( xTimer )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_ux_Timer_Get_Reload_منوال
    #تعریف_کرو trace_RETURN_ux_Timer_Get_Reload_منوال( uxReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_Timer_Get_Expiry_وقت
    #تعریف_کرو trace_ENTER_x_Timer_Get_Expiry_وقت( xTimer )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_Timer_Get_Expiry_وقت
    #تعریف_کرو trace_RETURN_x_Timer_Get_Expiry_وقت( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_Timer_Get_Static_بفر
    #تعریف_کرو trace_ENTER_x_Timer_Get_Static_بفر( xTimer, ppx_Timer_بفر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_Timer_Get_Static_بفر
    #تعریف_کرو trace_RETURN_x_Timer_Get_Static_بفر( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_pcTimerGetName
    #تعریف_کرو traceENTER_pcTimerGetName( xTimer )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_pcTimerGetName
    #تعریف_کرو traceRETURN_pcTimerGetName( pcTimerName )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_Timer_Is_Timer_فعال
    #تعریف_کرو trace_ENTER_x_Timer_Is_Timer_فعال( xTimer )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_Timer_Is_Timer_فعال
    #تعریف_کرو trace_RETURN_x_Timer_Is_Timer_فعال( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_pvTimerGetTimerID
    #تعریف_کرو traceENTER_pvTimerGetTimerID( xTimer )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_pvTimerGetTimerID
    #تعریف_کرو traceRETURN_pvTimerGetTimerID( pvReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_v_Timer_سیٹ_Timer_ID
    #تعریف_کرو trace_ENTER_v_Timer_سیٹ_Timer_ID( xTimer, pvNewID )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_v_Timer_سیٹ_Timer_ID
    #تعریف_کرو trace_RETURN_v_Timer_سیٹ_Timer_ID()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_xTimerPendFunctionCallFromISR
    #تعریف_کرو traceENTER_xTimerPendFunctionCallFromISR( x_Function_مقصد_Pend, pvParameter1, ulParameter2, px_Higher_ترجیح_Task_Woken )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_xTimerPendFunctionCallFromISR
    #تعریف_کرو traceRETURN_xTimerPendFunctionCallFromISR( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_xTimerPendFunctionCall
    #تعریف_کرو traceENTER_xTimerPendFunctionCall( x_Function_مقصد_Pend, pvParameter1, ulParameter2, x_Ticks_مقصد_Wait )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_xTimerPendFunctionCall
    #تعریف_کرو traceRETURN_xTimerPendFunctionCall( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_ux_Timer_Get_Timer_نمبر
    #تعریف_کرو trace_ENTER_ux_Timer_Get_Timer_نمبر( xTimer )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_ux_Timer_Get_Timer_نمبر
    #تعریف_کرو trace_RETURN_ux_Timer_Get_Timer_نمبر( ux_Timer_نمبر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_v_Timer_سیٹ_Timer_نمبر
    #تعریف_کرو trace_ENTER_v_Timer_سیٹ_Timer_نمبر( xTimer, ux_Timer_نمبر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_v_Timer_سیٹ_Timer_نمبر
    #تعریف_کرو trace_RETURN_v_Timer_سیٹ_Timer_نمبر()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_xTaskCreateStatic
    #تعریف_کرو traceENTER_xTaskCreateStatic( px_Task_رمز, pcName, ux_ڈھیر_گہرائی, pvParameters, ux_ترجیح, pux_ڈھیر_بفر, px_Task_بفر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_xTaskCreateStatic
    #تعریف_کرو traceRETURN_xTaskCreateStatic( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_Task_Create_Static_Affinity_سیٹ
    #تعریف_کرو trace_ENTER_x_Task_Create_Static_Affinity_سیٹ( px_Task_رمز, pcName, ux_ڈھیر_گہرائی, pvParameters, ux_ترجیح, pux_ڈھیر_بفر, px_Task_بفر, ux_Core_Affinity_نقاب )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_Task_Create_Static_Affinity_سیٹ
    #تعریف_کرو trace_RETURN_x_Task_Create_Static_Affinity_سیٹ( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_xTaskCreateRestrictedStatic
    #تعریف_کرو traceENTER_xTaskCreateRestrictedStatic( pxTaskDefinition, pxCreatedTask )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_xTaskCreateRestrictedStatic
    #تعریف_کرو traceRETURN_xTaskCreateRestrictedStatic( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_Task_Create_Restricted_Static_Affinity_سیٹ
    #تعریف_کرو trace_ENTER_x_Task_Create_Restricted_Static_Affinity_سیٹ( pxTaskDefinition, ux_Core_Affinity_نقاب, pxCreatedTask )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_Task_Create_Restricted_Static_Affinity_سیٹ
    #تعریف_کرو trace_RETURN_x_Task_Create_Restricted_Static_Affinity_سیٹ( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_xTaskCreateRestricted
    #تعریف_کرو traceENTER_xTaskCreateRestricted( pxTaskDefinition, pxCreatedTask )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_xTaskCreateRestricted
    #تعریف_کرو traceRETURN_xTaskCreateRestricted( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_Task_Create_Restricted_Affinity_سیٹ
    #تعریف_کرو trace_ENTER_x_Task_Create_Restricted_Affinity_سیٹ( pxTaskDefinition, ux_Core_Affinity_نقاب, pxCreatedTask )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_Task_Create_Restricted_Affinity_سیٹ
    #تعریف_کرو trace_RETURN_x_Task_Create_Restricted_Affinity_سیٹ( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_xTaskCreate
    #تعریف_کرو traceENTER_xTaskCreate( px_Task_رمز, pcName, ux_ڈھیر_گہرائی, pvParameters, ux_ترجیح, pxCreatedTask )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_xTaskCreate
    #تعریف_کرو traceRETURN_xTaskCreate( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_Task_Create_Affinity_سیٹ
    #تعریف_کرو trace_ENTER_x_Task_Create_Affinity_سیٹ( px_Task_رمز, pcName, ux_ڈھیر_گہرائی, pvParameters, ux_ترجیح, ux_Core_Affinity_نقاب, pxCreatedTask )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_Task_Create_Affinity_سیٹ
    #تعریف_کرو trace_RETURN_x_Task_Create_Affinity_سیٹ( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_vTaskDelete
    #تعریف_کرو traceENTER_vTaskDelete( x_Task_مقصد_Delete )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_vTaskDelete
    #تعریف_کرو traceRETURN_vTaskDelete()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_xTaskDelayUntil
    #تعریف_کرو traceENTER_xTaskDelayUntil( px_پچھلا_Wake_وقت, x_وقت_Increment )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_xTaskDelayUntil
    #تعریف_کرو traceRETURN_xTaskDelayUntil( xShouldDelay )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_vTaskDelay
    #تعریف_کرو traceENTER_vTaskDelay( x_Ticks_مقصد_Delay )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_vTaskDelay
    #تعریف_کرو traceRETURN_vTaskDelay()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_eTaskGetState
    #تعریف_کرو traceENTER_eTaskGetState( xTask )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_eTaskGetState
    #تعریف_کرو traceRETURN_eTaskGetState( eReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_ux_Task_ترجیح_Get
    #تعریف_کرو trace_ENTER_ux_Task_ترجیح_Get( xTask )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_ux_Task_ترجیح_Get
    #تعریف_کرو trace_RETURN_ux_Task_ترجیح_Get( uxReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_ux_Task_ترجیح_Get_From_ISR
    #تعریف_کرو trace_ENTER_ux_Task_ترجیح_Get_From_ISR( xTask )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_ux_Task_ترجیح_Get_From_ISR
    #تعریف_کرو trace_RETURN_ux_Task_ترجیح_Get_From_ISR( uxReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_ux_Task_بنیاد_ترجیح_Get
    #تعریف_کرو trace_ENTER_ux_Task_بنیاد_ترجیح_Get( xTask )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_ux_Task_بنیاد_ترجیح_Get
    #تعریف_کرو trace_RETURN_ux_Task_بنیاد_ترجیح_Get( uxReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_ux_Task_بنیاد_ترجیح_Get_From_ISR
    #تعریف_کرو trace_ENTER_ux_Task_بنیاد_ترجیح_Get_From_ISR( xTask )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_ux_Task_بنیاد_ترجیح_Get_From_ISR
    #تعریف_کرو trace_RETURN_ux_Task_بنیاد_ترجیح_Get_From_ISR( uxReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_v_Task_ترجیح_سیٹ
    #تعریف_کرو trace_ENTER_v_Task_ترجیح_سیٹ( xTask, ux_New_ترجیح )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_v_Task_ترجیح_سیٹ
    #تعریف_کرو trace_RETURN_v_Task_ترجیح_سیٹ()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_v_Task_Core_Affinity_سیٹ
    #تعریف_کرو trace_ENTER_v_Task_Core_Affinity_سیٹ( xTask, ux_Core_Affinity_نقاب )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_v_Task_Core_Affinity_سیٹ
    #تعریف_کرو trace_RETURN_v_Task_Core_Affinity_سیٹ()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_vTaskCoreAffinityGet
    #تعریف_کرو traceENTER_vTaskCoreAffinityGet( xTask )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_vTaskCoreAffinityGet
    #تعریف_کرو traceRETURN_vTaskCoreAffinityGet( ux_Core_Affinity_نقاب )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_vTaskPreemptionDisable
    #تعریف_کرو traceENTER_vTaskPreemptionDisable( xTask )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_vTaskPreemptionDisable
    #تعریف_کرو traceRETURN_vTaskPreemptionDisable()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_vTaskPreemptionEnable
    #تعریف_کرو traceENTER_vTaskPreemptionEnable( xTask )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_vTaskPreemptionEnable
    #تعریف_کرو traceRETURN_vTaskPreemptionEnable()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_vTaskSuspend
    #تعریف_کرو traceENTER_vTaskSuspend( x_Task_مقصد_Suspend )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_vTaskSuspend
    #تعریف_کرو traceRETURN_vTaskSuspend()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_vTaskResume
    #تعریف_کرو traceENTER_vTaskResume( x_Task_مقصد_Resume )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_vTaskResume
    #تعریف_کرو traceRETURN_vTaskResume()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_xTaskResumeFromISR
    #تعریف_کرو traceENTER_xTaskResumeFromISR( x_Task_مقصد_Resume )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_xTaskResumeFromISR
    #تعریف_کرو traceRETURN_xTaskResumeFromISR( xYieldRequired )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_v_Task_آغاز_Scheduler
    #تعریف_کرو trace_ENTER_v_Task_آغاز_Scheduler()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_v_Task_آغاز_Scheduler
    #تعریف_کرو trace_RETURN_v_Task_آغاز_Scheduler()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_v_Task_انتہا_Scheduler
    #تعریف_کرو trace_ENTER_v_Task_انتہا_Scheduler()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_v_Task_انتہا_Scheduler
    #تعریف_کرو trace_RETURN_v_Task_انتہا_Scheduler()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_vTaskSuspendAll
    #تعریف_کرو traceENTER_vTaskSuspendAll()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_vTaskSuspendAll
    #تعریف_کرو traceRETURN_vTaskSuspendAll()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_xTaskResumeAll
    #تعریف_کرو traceENTER_xTaskResumeAll()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_xTaskResumeAll
    #تعریف_کرو traceRETURN_xTaskResumeAll( xAlreadyYielded )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_Task_Get_Tick_گنتی
    #تعریف_کرو trace_ENTER_x_Task_Get_Tick_گنتی()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_Task_Get_Tick_گنتی
    #تعریف_کرو trace_RETURN_x_Task_Get_Tick_گنتی( xTicks )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_Task_Get_Tick_گنتی_From_ISR
    #تعریف_کرو trace_ENTER_x_Task_Get_Tick_گنتی_From_ISR()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_Task_Get_Tick_گنتی_From_ISR
    #تعریف_کرو trace_RETURN_x_Task_Get_Tick_گنتی_From_ISR( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_ux_Task_Get_نمبر_کا_Tasks
    #تعریف_کرو trace_ENTER_ux_Task_Get_نمبر_کا_Tasks()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_ux_Task_Get_نمبر_کا_Tasks
    #تعریف_کرو trace_RETURN_ux_Task_Get_نمبر_کا_Tasks( ux_Current_نمبر_کا_Tasks )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_pcTaskGetName
    #تعریف_کرو traceENTER_pcTaskGetName( x_Task_مقصد_Query )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_pcTaskGetName
    #تعریف_کرو traceRETURN_pcTaskGetName( pcTaskName )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_xTaskGetHandle
    #تعریف_کرو traceENTER_xTaskGetHandle( pc_Name_مقصد_Query )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_xTaskGetHandle
    #تعریف_کرو traceRETURN_xTaskGetHandle( pxTCB )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_xTaskGetStaticBuffers
    #تعریف_کرو traceENTER_xTaskGetStaticBuffers( xTask, ppux_ڈھیر_بفر, ppx_Task_بفر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_xTaskGetStaticBuffers
    #تعریف_کرو traceRETURN_xTaskGetStaticBuffers( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_uxTaskGetSystemState
    #تعریف_کرو traceENTER_uxTaskGetSystemState( px_Task_حالت_Array, ux_Array_سائز, pul_کل_تسلسل_وقت )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_uxTaskGetSystemState
    #تعریف_کرو traceRETURN_uxTaskGetSystemState( uxTask )
#اگر_ختم

#اگر ( config_نمبر_کا_CORES == 1 )
    #اگر_غیر_تعریف_شدہ traceENTER_xTaskGetIdleTaskHandle
        #تعریف_کرو traceENTER_xTaskGetIdleTaskHandle()
    #اگر_ختم
#اگر_ختم

#اگر ( config_نمبر_کا_CORES == 1 )
    #اگر_غیر_تعریف_شدہ traceRETURN_xTaskGetIdleTaskHandle
        #تعریف_کرو traceRETURN_xTaskGetIdleTaskHandle( xIdleTaskHandle )
    #اگر_ختم
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_xTaskGetIdleTaskHandleForCore
    #تعریف_کرو traceENTER_xTaskGetIdleTaskHandleForCore( xCoreID )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_xTaskGetIdleTaskHandleForCore
    #تعریف_کرو traceRETURN_xTaskGetIdleTaskHandleForCore( xIdleTaskHandle )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_v_Task_قدم_Tick
    #تعریف_کرو trace_ENTER_v_Task_قدم_Tick( x_Ticks_مقصد_Jump )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_v_Task_قدم_Tick
    #تعریف_کرو trace_RETURN_v_Task_قدم_Tick()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_Task_Catch_اوپر_Ticks
    #تعریف_کرو trace_ENTER_x_Task_Catch_اوپر_Ticks( x_Ticks_مقصد_Catch_اوپر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_Task_Catch_اوپر_Ticks
    #تعریف_کرو trace_RETURN_x_Task_Catch_اوپر_Ticks( xYieldOccurred )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_xTaskAbortDelay
    #تعریف_کرو traceENTER_xTaskAbortDelay( xTask )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_xTaskAbortDelay
    #تعریف_کرو traceRETURN_xTaskAbortDelay( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_xTaskIncrementTick
    #تعریف_کرو traceENTER_xTaskIncrementTick()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_xTaskIncrementTick
    #تعریف_کرو traceRETURN_xTaskIncrementTick( xSwitchRequired )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_v_Task_سیٹ_Application_Task_لیبل
    #تعریف_کرو trace_ENTER_v_Task_سیٹ_Application_Task_لیبل( xTask, pxHookFunction )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_v_Task_سیٹ_Application_Task_لیبل
    #تعریف_کرو trace_RETURN_v_Task_سیٹ_Application_Task_لیبل()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_Task_Get_Application_Task_لیبل
    #تعریف_کرو trace_ENTER_x_Task_Get_Application_Task_لیبل( xTask )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_Task_Get_Application_Task_لیبل
    #تعریف_کرو trace_RETURN_x_Task_Get_Application_Task_لیبل( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_Task_Get_Application_Task_لیبل_From_ISR
    #تعریف_کرو trace_ENTER_x_Task_Get_Application_Task_لیبل_From_ISR( xTask )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_Task_Get_Application_Task_لیبل_From_ISR
    #تعریف_کرو trace_RETURN_x_Task_Get_Application_Task_لیبل_From_ISR( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_xTaskCallApplicationTaskHook
    #تعریف_کرو traceENTER_xTaskCallApplicationTaskHook( xTask, pvParameter )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_xTaskCallApplicationTaskHook
    #تعریف_کرو traceRETURN_xTaskCallApplicationTaskHook( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_vTaskSwitchContext
    #تعریف_کرو traceENTER_vTaskSwitchContext()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_vTaskSwitchContext
    #تعریف_کرو traceRETURN_vTaskSwitchContext()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_v_Task_رکھو_On_Event_List
    #تعریف_کرو trace_ENTER_v_Task_رکھو_On_Event_List( pxEventList, x_Ticks_مقصد_Wait )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_v_Task_رکھو_On_Event_List
    #تعریف_کرو trace_RETURN_v_Task_رکھو_On_Event_List()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_v_Task_رکھو_On_Unordered_Event_List
    #تعریف_کرو trace_ENTER_v_Task_رکھو_On_Unordered_Event_List( pxEventList, x_شے_قدر, x_Ticks_مقصد_Wait )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_v_Task_رکھو_On_Unordered_Event_List
    #تعریف_کرو trace_RETURN_v_Task_رکھو_On_Unordered_Event_List()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_v_Task_رکھو_On_Event_List_Restricted
    #تعریف_کرو trace_ENTER_v_Task_رکھو_On_Event_List_Restricted( pxEventList, x_Ticks_مقصد_Wait, xWaitIndefinitely )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_v_Task_رکھو_On_Event_List_Restricted
    #تعریف_کرو trace_RETURN_v_Task_رکھو_On_Event_List_Restricted()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_xTaskRemoveFromEventList
    #تعریف_کرو traceENTER_xTaskRemoveFromEventList( pxEventList )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_xTaskRemoveFromEventList
    #تعریف_کرو traceRETURN_xTaskRemoveFromEventList( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_vTaskRemoveFromUnorderedEventList
    #تعریف_کرو traceENTER_vTaskRemoveFromUnorderedEventList( px_Event_List_شے, x_شے_قدر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_vTaskRemoveFromUnorderedEventList
    #تعریف_کرو traceRETURN_vTaskRemoveFromUnorderedEventList()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_v_Task_سیٹ_وقت_برآمد_State
    #تعریف_کرو trace_ENTER_v_Task_سیٹ_وقت_برآمد_State( px_وقت_برآمد )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_v_Task_سیٹ_وقت_برآمد_State
    #تعریف_کرو trace_RETURN_v_Task_سیٹ_وقت_برآمد_State()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_v_Task_اندرونی_سیٹ_وقت_برآمد_State
    #تعریف_کرو trace_ENTER_v_Task_اندرونی_سیٹ_وقت_برآمد_State( px_وقت_برآمد )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_v_Task_اندرونی_سیٹ_وقت_برآمد_State
    #تعریف_کرو trace_RETURN_v_Task_اندرونی_سیٹ_وقت_برآمد_State()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_Task_Check_For_وقت_برآمد
    #تعریف_کرو trace_ENTER_x_Task_Check_For_وقت_برآمد( px_وقت_برآمد, px_Ticks_مقصد_Wait )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_Task_Check_For_وقت_برآمد
    #تعریف_کرو trace_RETURN_x_Task_Check_For_وقت_برآمد( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_vTaskMissedYield
    #تعریف_کرو traceENTER_vTaskMissedYield()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_vTaskMissedYield
    #تعریف_کرو traceRETURN_vTaskMissedYield()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_ux_Task_Get_Task_نمبر
    #تعریف_کرو trace_ENTER_ux_Task_Get_Task_نمبر( xTask )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_ux_Task_Get_Task_نمبر
    #تعریف_کرو trace_RETURN_ux_Task_Get_Task_نمبر( uxReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_v_Task_سیٹ_Task_نمبر
    #تعریف_کرو trace_ENTER_v_Task_سیٹ_Task_نمبر( xTask, uxHandle )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_v_Task_سیٹ_Task_نمبر
    #تعریف_کرو trace_RETURN_v_Task_سیٹ_Task_نمبر()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_e_Task_Confirm_Sleep_منوال_حالت
    #تعریف_کرو trace_ENTER_e_Task_Confirm_Sleep_منوال_حالت()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_e_Task_Confirm_Sleep_منوال_حالت
    #تعریف_کرو trace_RETURN_e_Task_Confirm_Sleep_منوال_حالت( eReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_v_Task_سیٹ_Thread_Local_Storage_اشارہ_گر
    #تعریف_کرو trace_ENTER_v_Task_سیٹ_Thread_Local_Storage_اشارہ_گر( x_Task_مقصد_سیٹ, x_اشاریہ, pv_قدر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_v_Task_سیٹ_Thread_Local_Storage_اشارہ_گر
    #تعریف_کرو trace_RETURN_v_Task_سیٹ_Thread_Local_Storage_اشارہ_گر()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_pv_Task_Get_Thread_Local_Storage_اشارہ_گر
    #تعریف_کرو trace_ENTER_pv_Task_Get_Thread_Local_Storage_اشارہ_گر( x_Task_مقصد_Query, x_اشاریہ )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_pv_Task_Get_Thread_Local_Storage_اشارہ_گر
    #تعریف_کرو trace_RETURN_pv_Task_Get_Thread_Local_Storage_اشارہ_گر( pvReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_vTaskAllocateMPURegions
    #تعریف_کرو traceENTER_vTaskAllocateMPURegions( x_Task_مقصد_Modify, pxRegions )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_vTaskAllocateMPURegions
    #تعریف_کرو traceRETURN_vTaskAllocateMPURegions()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_vTaskGetInfo
    #تعریف_کرو traceENTER_vTaskGetInfo( xTask, px_Task_حالت, x_Get_Free_ڈھیر_Space, eState )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_vTaskGetInfo
    #تعریف_کرو traceRETURN_vTaskGetInfo()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_ux_Task_Get_ڈھیر_بلند_Water_Mark2
    #تعریف_کرو trace_ENTER_ux_Task_Get_ڈھیر_بلند_Water_Mark2( xTask )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_ux_Task_Get_ڈھیر_بلند_Water_Mark2
    #تعریف_کرو trace_RETURN_ux_Task_Get_ڈھیر_بلند_Water_Mark2( uxReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_ux_Task_Get_ڈھیر_بلند_Water_Mark
    #تعریف_کرو trace_ENTER_ux_Task_Get_ڈھیر_بلند_Water_Mark( xTask )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_ux_Task_Get_ڈھیر_بلند_Water_Mark
    #تعریف_کرو trace_RETURN_ux_Task_Get_ڈھیر_بلند_Water_Mark( uxReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_xTaskGetCurrentTaskHandle
    #تعریف_کرو traceENTER_xTaskGetCurrentTaskHandle()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_xTaskGetCurrentTaskHandle
    #تعریف_کرو traceRETURN_xTaskGetCurrentTaskHandle( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_xTaskGetCurrentTaskHandleForCore
    #تعریف_کرو traceENTER_xTaskGetCurrentTaskHandleForCore( xCoreID )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_xTaskGetCurrentTaskHandleForCore
    #تعریف_کرو traceRETURN_xTaskGetCurrentTaskHandleForCore( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_xTaskGetSchedulerState
    #تعریف_کرو traceENTER_xTaskGetSchedulerState()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_xTaskGetSchedulerState
    #تعریف_کرو traceRETURN_xTaskGetSchedulerState( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_Task_ترجیح_Inherit
    #تعریف_کرو trace_ENTER_x_Task_ترجیح_Inherit( pxMutexHolder )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_Task_ترجیح_Inherit
    #تعریف_کرو trace_RETURN_x_Task_ترجیح_Inherit( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_Task_ترجیح_Disinherit
    #تعریف_کرو trace_ENTER_x_Task_ترجیح_Disinherit( pxMutexHolder )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_Task_ترجیح_Disinherit
    #تعریف_کرو trace_RETURN_x_Task_ترجیح_Disinherit( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_v_Task_ترجیح_Disinherit_After_Timeout
    #تعریف_کرو trace_ENTER_v_Task_ترجیح_Disinherit_After_Timeout( pxMutexHolder, ux_Highest_ترجیح_انتظار_Task )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_v_Task_ترجیح_Disinherit_After_Timeout
    #تعریف_کرو trace_RETURN_v_Task_ترجیح_Disinherit_After_Timeout()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_vTaskYieldWithinAPI
    #تعریف_کرو traceENTER_vTaskYieldWithinAPI()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_vTaskYieldWithinAPI
    #تعریف_کرو traceRETURN_vTaskYieldWithinAPI()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_vTaskEnterCritical
    #تعریف_کرو traceENTER_vTaskEnterCritical()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_vTaskEnterCritical
    #تعریف_کرو traceRETURN_vTaskEnterCritical()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_vTaskEnterCriticalFromISR
    #تعریف_کرو traceENTER_vTaskEnterCriticalFromISR()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_vTaskEnterCriticalFromISR
    #تعریف_کرو traceRETURN_vTaskEnterCriticalFromISR( ux_Saved_مداخلت_حالت )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_vTaskExitCritical
    #تعریف_کرو traceENTER_vTaskExitCritical()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_vTaskExitCritical
    #تعریف_کرو traceRETURN_vTaskExitCritical()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_vTaskExitCriticalFromISR
    #تعریف_کرو traceENTER_vTaskExitCriticalFromISR( ux_Saved_مداخلت_حالت )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_vTaskExitCriticalFromISR
    #تعریف_کرو traceRETURN_vTaskExitCriticalFromISR()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_vTaskListTasks
    #تعریف_کرو traceENTER_vTaskListTasks( pc_Write_بفر, ux_بفر_لمبائی )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_vTaskListTasks
    #تعریف_کرو traceRETURN_vTaskListTasks()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_v_Task_Get_تسلسل_وقت_Statistics
    #تعریف_کرو trace_ENTER_v_Task_Get_تسلسل_وقت_Statistics( pc_Write_بفر, ux_بفر_لمبائی )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_v_Task_Get_تسلسل_وقت_Statistics
    #تعریف_کرو trace_RETURN_v_Task_Get_تسلسل_وقت_Statistics()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_ux_Task_Reset_Event_شے_قدر
    #تعریف_کرو trace_ENTER_ux_Task_Reset_Event_شے_قدر()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_ux_Task_Reset_Event_شے_قدر
    #تعریف_کرو trace_RETURN_ux_Task_Reset_Event_شے_قدر( uxReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_pv_Task_Increment_Mutex_Held_گنتی
    #تعریف_کرو trace_ENTER_pv_Task_Increment_Mutex_Held_گنتی()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_pv_Task_Increment_Mutex_Held_گنتی
    #تعریف_کرو trace_RETURN_pv_Task_Increment_Mutex_Held_گنتی( pxTCB )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_ulTaskGenericNotifyTake
    #تعریف_کرو traceENTER_ulTaskGenericNotifyTake( ux_اشاریہ_مقصد_Wait_On, x_Clear_گنتی_On_Exit, x_Ticks_مقصد_Wait )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_ulTaskGenericNotifyTake
    #تعریف_کرو traceRETURN_ulTaskGenericNotifyTake( ulReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_xTaskGenericNotifyWait
    #تعریف_کرو traceENTER_xTaskGenericNotifyWait( ux_اشاریہ_مقصد_Wait_On, ul_بٹ_مقصد_Clear_On_اندراج, ul_بٹ_مقصد_Clear_On_Exit, pul_Notification_قدر, x_Ticks_مقصد_Wait )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_xTaskGenericNotifyWait
    #تعریف_کرو traceRETURN_xTaskGenericNotifyWait( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_xTaskGenericNotify
    #تعریف_کرو traceENTER_xTaskGenericNotify( x_Task_مقصد_Notify, ux_اشاریہ_مقصد_Notify, ul_قدر, eAction, pul_پچھلا_Notification_قدر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_xTaskGenericNotify
    #تعریف_کرو traceRETURN_xTaskGenericNotify( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_xTaskGenericNotifyFromISR
    #تعریف_کرو traceENTER_xTaskGenericNotifyFromISR( x_Task_مقصد_Notify, ux_اشاریہ_مقصد_Notify, ul_قدر, eAction, pul_پچھلا_Notification_قدر, px_Higher_ترجیح_Task_Woken )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_xTaskGenericNotifyFromISR
    #تعریف_کرو traceRETURN_xTaskGenericNotifyFromISR( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_vTaskGenericNotifyGiveFromISR
    #تعریف_کرو traceENTER_vTaskGenericNotifyGiveFromISR( x_Task_مقصد_Notify, ux_اشاریہ_مقصد_Notify, px_Higher_ترجیح_Task_Woken )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_vTaskGenericNotifyGiveFromISR
    #تعریف_کرو traceRETURN_vTaskGenericNotifyGiveFromISR()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_xTaskGenericNotifyStateClear
    #تعریف_کرو traceENTER_xTaskGenericNotifyStateClear( xTask, ux_اشاریہ_مقصد_Clear )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_xTaskGenericNotifyStateClear
    #تعریف_کرو traceRETURN_xTaskGenericNotifyStateClear( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_ul_Task_Generic_Notify_قدر_Clear
    #تعریف_کرو trace_ENTER_ul_Task_Generic_Notify_قدر_Clear( xTask, ux_اشاریہ_مقصد_Clear, ul_بٹ_مقصد_Clear )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_ul_Task_Generic_Notify_قدر_Clear
    #تعریف_کرو trace_RETURN_ul_Task_Generic_Notify_قدر_Clear( ulReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_ul_Task_Get_تسلسل_وقت_شمار
    #تعریف_کرو trace_ENTER_ul_Task_Get_تسلسل_وقت_شمار( xTask )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_ul_Task_Get_تسلسل_وقت_شمار
    #تعریف_کرو trace_RETURN_ul_Task_Get_تسلسل_وقت_شمار( ul_تسلسل_وقت_شمار )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_ul_Task_Get_تسلسل_وقت_Percent
    #تعریف_کرو trace_ENTER_ul_Task_Get_تسلسل_وقت_Percent( xTask )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_ul_Task_Get_تسلسل_وقت_Percent
    #تعریف_کرو trace_RETURN_ul_Task_Get_تسلسل_وقت_Percent( ulReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_ul_Task_Get_Idle_تسلسل_وقت_شمار
    #تعریف_کرو trace_ENTER_ul_Task_Get_Idle_تسلسل_وقت_شمار()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_ul_Task_Get_Idle_تسلسل_وقت_شمار
    #تعریف_کرو trace_RETURN_ul_Task_Get_Idle_تسلسل_وقت_شمار( ulReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_ul_Task_Get_Idle_تسلسل_وقت_Percent
    #تعریف_کرو trace_ENTER_ul_Task_Get_Idle_تسلسل_وقت_Percent()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_ul_Task_Get_Idle_تسلسل_وقت_Percent
    #تعریف_کرو trace_RETURN_ul_Task_Get_Idle_تسلسل_وقت_Percent( ulReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_xTaskGetMPUSettings
    #تعریف_کرو traceENTER_xTaskGetMPUSettings( xTask )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_xTaskGetMPUSettings
    #تعریف_کرو traceRETURN_xTaskGetMPUSettings( xMPUSettings )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_دھارا_بفر_Generic_Create
    #تعریف_کرو trace_ENTER_x_دھارا_بفر_Generic_Create( x_بفر_سائز_بائٹیں, x_Trigger_Level_بائٹیں, x_دھارا_بفر_Type, pxSendCompletedCallback, pxReceiveCompletedCallback )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_دھارا_بفر_Generic_Create
    #تعریف_کرو trace_RETURN_x_دھارا_بفر_Generic_Create( pv_Allocated_حافظہ )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_دھارا_بفر_Generic_Create_Static
    #تعریف_کرو trace_ENTER_x_دھارا_بفر_Generic_Create_Static( x_بفر_سائز_بائٹیں, x_Trigger_Level_بائٹیں, x_دھارا_بفر_Type, puc_دھارا_بفر_Storage_Area, px_Static_دھارا_بفر, pxSendCompletedCallback, pxReceiveCompletedCallback )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_دھارا_بفر_Generic_Create_Static
    #تعریف_کرو trace_RETURN_x_دھارا_بفر_Generic_Create_Static( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_دھارا_بفر_Get_Static_Buffers
    #تعریف_کرو trace_ENTER_x_دھارا_بفر_Get_Static_Buffers( x_دھارا_بفر, ppuc_دھارا_بفر_Storage_Area, ppx_Static_دھارا_بفر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_دھارا_بفر_Get_Static_Buffers
    #تعریف_کرو trace_RETURN_x_دھارا_بفر_Get_Static_Buffers( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_v_دھارا_بفر_Delete
    #تعریف_کرو trace_ENTER_v_دھارا_بفر_Delete( x_دھارا_بفر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_v_دھارا_بفر_Delete
    #تعریف_کرو trace_RETURN_v_دھارا_بفر_Delete()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_دھارا_بفر_Reset
    #تعریف_کرو trace_ENTER_x_دھارا_بفر_Reset( x_دھارا_بفر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_دھارا_بفر_Reset
    #تعریف_کرو trace_RETURN_x_دھارا_بفر_Reset( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_دھارا_بفر_Reset_From_ISR
    #تعریف_کرو trace_ENTER_x_دھارا_بفر_Reset_From_ISR( x_دھارا_بفر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_دھارا_بفر_Reset_From_ISR
    #تعریف_کرو trace_RETURN_x_دھارا_بفر_Reset_From_ISR( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_دھارا_بفر_سیٹ_Trigger_Level
    #تعریف_کرو trace_ENTER_x_دھارا_بفر_سیٹ_Trigger_Level( x_دھارا_بفر, xTriggerLevel )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_دھارا_بفر_سیٹ_Trigger_Level
    #تعریف_کرو trace_RETURN_x_دھارا_بفر_سیٹ_Trigger_Level( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_دھارا_بفر_Spaces_دستیاب
    #تعریف_کرو trace_ENTER_x_دھارا_بفر_Spaces_دستیاب( x_دھارا_بفر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_دھارا_بفر_Spaces_دستیاب
    #تعریف_کرو trace_RETURN_x_دھارا_بفر_Spaces_دستیاب( xSpace )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_دھارا_بفر_بائٹیں_دستیاب
    #تعریف_کرو trace_ENTER_x_دھارا_بفر_بائٹیں_دستیاب( x_دھارا_بفر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_دھارا_بفر_بائٹیں_دستیاب
    #تعریف_کرو trace_RETURN_x_دھارا_بفر_بائٹیں_دستیاب( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_دھارا_بفر_Send
    #تعریف_کرو trace_ENTER_x_دھارا_بفر_Send( x_دھارا_بفر, pv_Tx_ڈیٹا, x_ڈیٹا_لمبائی_بائٹیں, x_Ticks_مقصد_Wait )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_دھارا_بفر_Send
    #تعریف_کرو trace_RETURN_x_دھارا_بفر_Send( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_دھارا_بفر_Send_From_ISR
    #تعریف_کرو trace_ENTER_x_دھارا_بفر_Send_From_ISR( x_دھارا_بفر, pv_Tx_ڈیٹا, x_ڈیٹا_لمبائی_بائٹیں, px_Higher_ترجیح_Task_Woken )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_دھارا_بفر_Send_From_ISR
    #تعریف_کرو trace_RETURN_x_دھارا_بفر_Send_From_ISR( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_دھارا_بفر_Receive
    #تعریف_کرو trace_ENTER_x_دھارا_بفر_Receive( x_دھارا_بفر, pv_Rx_ڈیٹا, x_بفر_لمبائی_بائٹیں, x_Ticks_مقصد_Wait )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_دھارا_بفر_Receive
    #تعریف_کرو trace_RETURN_x_دھارا_بفر_Receive( x_Received_لمبائی )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_دھارا_بفر_اگلا_پیغام_لمبائی_بائٹیں
    #تعریف_کرو trace_ENTER_x_دھارا_بفر_اگلا_پیغام_لمبائی_بائٹیں( x_دھارا_بفر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_دھارا_بفر_اگلا_پیغام_لمبائی_بائٹیں
    #تعریف_کرو trace_RETURN_x_دھارا_بفر_اگلا_پیغام_لمبائی_بائٹیں( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_دھارا_بفر_Receive_From_ISR
    #تعریف_کرو trace_ENTER_x_دھارا_بفر_Receive_From_ISR( x_دھارا_بفر, pv_Rx_ڈیٹا, x_بفر_لمبائی_بائٹیں, px_Higher_ترجیح_Task_Woken )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_دھارا_بفر_Receive_From_ISR
    #تعریف_کرو trace_RETURN_x_دھارا_بفر_Receive_From_ISR( x_Received_لمبائی )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_دھارا_بفر_Is_خالی_خانے
    #تعریف_کرو trace_ENTER_x_دھارا_بفر_Is_خالی_خانے( x_دھارا_بفر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_دھارا_بفر_Is_خالی_خانے
    #تعریف_کرو trace_RETURN_x_دھارا_بفر_Is_خالی_خانے( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_دھارا_بفر_Is_بھرا
    #تعریف_کرو trace_ENTER_x_دھارا_بفر_Is_بھرا( x_دھارا_بفر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_دھارا_بفر_Is_بھرا
    #تعریف_کرو trace_RETURN_x_دھارا_بفر_Is_بھرا( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_دھارا_بفر_Send_Completed_From_ISR
    #تعریف_کرو trace_ENTER_x_دھارا_بفر_Send_Completed_From_ISR( x_دھارا_بفر, px_Higher_ترجیح_Task_Woken )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_دھارا_بفر_Send_Completed_From_ISR
    #تعریف_کرو trace_RETURN_x_دھارا_بفر_Send_Completed_From_ISR( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_x_دھارا_بفر_Receive_Completed_From_ISR
    #تعریف_کرو trace_ENTER_x_دھارا_بفر_Receive_Completed_From_ISR( x_دھارا_بفر, px_Higher_ترجیح_Task_Woken )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_x_دھارا_بفر_Receive_Completed_From_ISR
    #تعریف_کرو trace_RETURN_x_دھارا_بفر_Receive_Completed_From_ISR( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_ux_دھارا_بفر_Get_دھارا_بفر_Notification_اشاریہ
    #تعریف_کرو trace_ENTER_ux_دھارا_بفر_Get_دھارا_بفر_Notification_اشاریہ( x_دھارا_بفر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_ux_دھارا_بفر_Get_دھارا_بفر_Notification_اشاریہ
    #تعریف_کرو trace_RETURN_ux_دھارا_بفر_Get_دھارا_بفر_Notification_اشاریہ( ux_Notification_اشاریہ )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_v_دھارا_بفر_سیٹ_دھارا_بفر_Notification_اشاریہ
    #تعریف_کرو trace_ENTER_v_دھارا_بفر_سیٹ_دھارا_بفر_Notification_اشاریہ( x_دھارا_بفر, ux_Notification_اشاریہ )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_v_دھارا_بفر_سیٹ_دھارا_بفر_Notification_اشاریہ
    #تعریف_کرو trace_RETURN_v_دھارا_بفر_سیٹ_دھارا_بفر_Notification_اشاریہ()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_ux_دھارا_بفر_Get_دھارا_بفر_نمبر
    #تعریف_کرو trace_ENTER_ux_دھارا_بفر_Get_دھارا_بفر_نمبر( x_دھارا_بفر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_ux_دھارا_بفر_Get_دھارا_بفر_نمبر
    #تعریف_کرو trace_RETURN_ux_دھارا_بفر_Get_دھارا_بفر_نمبر( ux_دھارا_بفر_نمبر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_v_دھارا_بفر_سیٹ_دھارا_بفر_نمبر
    #تعریف_کرو trace_ENTER_v_دھارا_بفر_سیٹ_دھارا_بفر_نمبر( x_دھارا_بفر, ux_دھارا_بفر_نمبر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_v_دھارا_بفر_سیٹ_دھارا_بفر_نمبر
    #تعریف_کرو trace_RETURN_v_دھارا_بفر_سیٹ_دھارا_بفر_نمبر()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_uc_دھارا_بفر_Get_دھارا_بفر_Type
    #تعریف_کرو trace_ENTER_uc_دھارا_بفر_Get_دھارا_بفر_Type( x_دھارا_بفر )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_uc_دھارا_بفر_Get_دھارا_بفر_Type
    #تعریف_کرو trace_RETURN_uc_دھارا_بفر_Get_دھارا_بفر_Type( uc_دھارا_بفر_Type )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_vListInitialise
    #تعریف_کرو traceENTER_vListInitialise( pxList )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_vListInitialise
    #تعریف_کرو traceRETURN_vListInitialise()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_v_List_Initialise_شے
    #تعریف_کرو trace_ENTER_v_List_Initialise_شے( px_شے )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_v_List_Initialise_شے
    #تعریف_کرو trace_RETURN_v_List_Initialise_شے()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_v_List_داخل_کرو_انتہا
    #تعریف_کرو trace_ENTER_v_List_داخل_کرو_انتہا( pxList, px_New_List_شے )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_v_List_داخل_کرو_انتہا
    #تعریف_کرو trace_RETURN_v_List_داخل_کرو_انتہا()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_v_List_داخل_کرو
    #تعریف_کرو trace_ENTER_v_List_داخل_کرو( pxList, px_New_List_شے )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_v_List_داخل_کرو
    #تعریف_کرو trace_RETURN_v_List_داخل_کرو()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_uxListRemove
    #تعریف_کرو traceENTER_uxListRemove( px_شے_مقصد_Remove )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_uxListRemove
    #تعریف_کرو traceRETURN_uxListRemove( ux_نمبر_کا_اشیاء )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_xCoRoutineCreate
    #تعریف_کرو traceENTER_xCoRoutineCreate( px_Co_Routine_رمز, ux_ترجیح, ux_اشاریہ )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_xCoRoutineCreate
    #تعریف_کرو traceRETURN_xCoRoutineCreate( xReturn )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_v_Co_Routine_جوڑو_مقصد_Delayed_List
    #تعریف_کرو trace_ENTER_v_Co_Routine_جوڑو_مقصد_Delayed_List( x_Ticks_مقصد_Delay, pxEventList )
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_v_Co_Routine_جوڑو_مقصد_Delayed_List
    #تعریف_کرو trace_RETURN_v_Co_Routine_جوڑو_مقصد_Delayed_List()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_ENTER_v_Co_Routine_جدول_وقت
    #تعریف_کرو trace_ENTER_v_Co_Routine_جدول_وقت()
#اگر_ختم

#اگر_غیر_تعریف_شدہ trace_RETURN_v_Co_Routine_جدول_وقت
    #تعریف_کرو trace_RETURN_v_Co_Routine_جدول_وقت()
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceENTER_xCoRoutineRemoveFromEventList
    #تعریف_کرو traceENTER_xCoRoutineRemoveFromEventList( pxEventList )
#اگر_ختم

#اگر_غیر_تعریف_شدہ traceRETURN_xCoRoutineRemoveFromEventList
    #تعریف_کرو traceRETURN_xCoRoutineRemoveFromEventList( xReturn )
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

#اگر_غیر_تعریف_شدہ config_ENABLE_ڈھیر_PROTECTOR
    #تعریف_کرو config_ENABLE_ڈھیر_PROTECTOR    0
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

#اگر_غیر_تعریف_شدہ config_KERNEL_PROVIDED_STATIC_حافظہ
    #تعریف_کرو config_KERNEL_PROVIDED_STATIC_حافظہ    0
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

#اگر_غیر_تعریف_شدہ config_STATS_بفر_MAX_لمبائی
    #تعریف_کرو config_STATS_بفر_MAX_لمبائی    0xFFFF
#اگر_ختم

#اگر_غیر_تعریف_شدہ config_ڈھیر_گہرائی_TYPE

/* Defaults to StackType_t for backward compatibility, but can be overridden
 * in FreeRTOSConfig.h if StackType_t is too restrictive. */
    #تعریف_کرو config_ڈھیر_گہرائی_TYPE    ڈھیر_Type_t
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

#اگر ( ( config_تسلسل_مضرب_PRIORITIES == 0 ) && ( configUSE_TASK_PREEMPTION_DISABLE != 0 ) )
    #غلطی config_تسلسل_مضرب_PRIORITIES must be سیٹ مقصد 1 مقصد use task preemption disable
#اگر_ختم

#اگر ( ( configUSE_PREEMPTION == 0 ) && ( configUSE_TASK_PREEMPTION_DISABLE != 0 ) )
    #غلطی configUSE_PREEMPTION must be سیٹ مقصد 1 مقصد use task preemption disable
#اگر_ختم

#اگر ( ( config_نمبر_کا_CORES == 1 ) && ( configUSE_TASK_PREEMPTION_DISABLE != 0 ) )
    #غلطی configUSE_TASK_PREEMPTION_DISABLE is not supported اندر single core FreeRTOS
#اگر_ختم

#اگر ( ( config_نمبر_کا_CORES == 1 ) && ( configUSE_CORE_AFFINITY != 0 ) )
    #غلطی configUSE_CORE_AFFINITY is not supported اندر single core FreeRTOS
#اگر_ختم

#اگر ( ( config_نمبر_کا_CORES > 1 ) && ( config_USE_پورٹ_OPTIMISED_TASK_SELECTION != 0 ) )
    #غلطی config_USE_پورٹ_OPTIMISED_TASK_SELECTION is not supported اندر SMP FreeRTOS
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

/* The following config allows infinite loop control. For example, control the
 * infinite loop in idle task function when performing unit tests. */
#اگر_غیر_تعریف_شدہ configCONTROL_INFINITE_LOOP
    #تعریف_کرو configCONTROL_INFINITE_LOOP()
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
    #اگر ( configUSE_CORE_AFFINITY == 1 ) && ( config_نمبر_کا_CORES > 1 )
        UBaseType_t uxDummy26;
    #اگر_ختم
    Static_List_شے_t xDummy3[ 2 ];
    UBaseType_t uxDummy5;
    خالی * pxDummy6;
    #اگر ( config_نمبر_کا_CORES > 1 )
        بنیاد_Type_t xDummy23;
        UBaseType_t uxDummy24;
    #اگر_ختم
    uint8_t ucDummy7[ configMAX_TASK_NAME_LEN ];
    #اگر ( configUSE_TASK_PREEMPTION_DISABLE == 1 )
        بنیاد_Type_t xDummy25;
    #اگر_ختم
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
    #اگر ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 )
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
    UBaseType_t uxDummy6;
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
