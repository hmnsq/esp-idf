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

/* Acceptable values for configTICK_TYPE_WIDTH_IN_BITS. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TICK_TYPE_ਚੌੜਾਈ_16_ਬਿੱਟ    0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TICK_TYPE_ਚੌੜਾਈ_32_ਬਿੱਟ    1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TICK_TYPE_ਚੌੜਾਈ_64_ਬਿੱਟ    2

/* Application specific configuration options. */
#ਅਨ੍ਤਰ੍ਭਾਵ "FreeRTOSConfig.h"

#ਜੇ !defined( config_USE_16_ਦੋਆਧਾਰੀ_ਅੰਕ_TICKS ) && !defined( config_TICK_TYPE_ਚੌੜਾਈ_ਵਿੱਚ_ਬਿੱਟ )
    #ਗਲਤੀ Missing definition:  One ਦਾ config_USE_16_ਦੋਆਧਾਰੀ_ਅੰਕ_TICKS and config_TICK_TYPE_ਚੌੜਾਈ_ਵਿੱਚ_ਬਿੱਟ must be defined ਵਿੱਚ FreeRTOSConfig.h.  See the Configuration section ਦਾ the FreeRTOS API documentation ਲਈ details.
#ਜੇ_ਅੰਤ

#ਜੇ defined( config_USE_16_ਦੋਆਧਾਰੀ_ਅੰਕ_TICKS ) && defined( config_TICK_TYPE_ਚੌੜਾਈ_ਵਿੱਚ_ਬਿੱਟ )
    #ਗਲਤੀ Only one ਦਾ config_USE_16_ਦੋਆਧਾਰੀ_ਅੰਕ_TICKS and config_TICK_TYPE_ਚੌੜਾਈ_ਵਿੱਚ_ਬਿੱਟ must be defined ਵਿੱਚ FreeRTOSConfig.h.  See the Configuration section ਦਾ the FreeRTOS API documentation ਲਈ details.
#ਜੇ_ਅੰਤ

/* Define configTICK_TYPE_WIDTH_IN_BITS according to the
 * value of configUSE_16_BIT_TICKS for backward compatibility. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_TICK_TYPE_ਚੌੜਾਈ_ਵਿੱਚ_ਬਿੱਟ
    #ਜੇ ( config_USE_16_ਦੋਆਧਾਰੀ_ਅੰਕ_TICKS == 1 )
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_TICK_TYPE_ਚੌੜਾਈ_ਵਿੱਚ_ਬਿੱਟ    TICK_TYPE_ਚੌੜਾਈ_16_ਬਿੱਟ
    #ਨਹੀਂਤਾਂ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_TICK_TYPE_ਚੌੜਾਈ_ਵਿੱਚ_ਬਿੱਟ    TICK_TYPE_ਚੌੜਾਈ_32_ਬਿੱਟ
    #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

/* Set configUSE_MPU_WRAPPERS_V1 to 1 to use MPU wrappers v1. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configUSE_MPU_WRAPPERS_V1
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_MPU_WRAPPERS_V1    0
#ਜੇ_ਅੰਤ

/* Set configENABLE_ACCESS_CONTROL_LIST to 1 to enable access control list support. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configENABLE_ACCESS_CONTROL_LIST
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configENABLE_ACCESS_CONTROL_LIST    0
#ਜੇ_ਅੰਤ

/* Set default value of configNUMBER_OF_CORES to 1 to use single core FreeRTOS. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_ਸੰਖਿਆ_ਦਾ_CORES
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_ਸੰਖਿਆ_ਦਾ_CORES    1
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configUSE_MALLOC_FAILED_HOOK
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_MALLOC_FAILED_HOOK    0
#ਜੇ_ਅੰਤ

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

    #ਅਨ੍ਤਰ੍ਭਾਵ "newlib-freertos.h"

#ਜੇ_ਅੰਤ /* if ( configUSE_NEWLIB_REENTRANT == 1 ) */

/* Must be defaulted before configUSE_PICOLIBC_TLS is used below. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configUSE_PICOLIBC_TLS
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_PICOLIBC_TLS    0
#ਜੇ_ਅੰਤ

#ਜੇ ( configUSE_PICOLIBC_TLS == 1 )

    #ਅਨ੍ਤਰ੍ਭਾਵ "picolibc-freertos.h"

#ਜੇ_ਅੰਤ /* if ( configUSE_PICOLIBC_TLS == 1 ) */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configUSE_C_RUNTIME_TLS_SUPPORT
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_C_RUNTIME_TLS_SUPPORT    0
#ਜੇ_ਅੰਤ

#ਜੇ ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 )

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
#ਜੇ_ਅੰਤ /* if ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 ) */

/*
 * Check all the required application specific macros have been defined.
 * These macros are application specific and (as downloaded) are defined
 * within FreeRTOSConfig.h.
 */

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

#ਜੇ ( config_ਸੰਖਿਆ_ਦਾ_CORES > 1 )
    #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configUSE_PASSIVE_IDLE_HOOK
        #ਗਲਤੀ Missing definition:  configUSE_PASSIVE_IDLE_HOOK must be defined ਵਿੱਚ FreeRTOSConfig.h as either 1 or 0.  See the Configuration section ਦਾ the FreeRTOS API documentation ਲਈ details.
    #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configUSE_TICK_HOOK
    #ਗਲਤੀ Missing definition:  configUSE_TICK_HOOK must be defined ਵਿੱਚ FreeRTOSConfig.h as either 1 or 0.  See the Configuration section ਦਾ the FreeRTOS API documentation ਲਈ details.
#ਜੇ_ਅੰਤ

#ਜੇ ( ( config_TICK_TYPE_ਚੌੜਾਈ_ਵਿੱਚ_ਬਿੱਟ != TICK_TYPE_ਚੌੜਾਈ_16_ਬਿੱਟ ) && \
    ( config_TICK_TYPE_ਚੌੜਾਈ_ਵਿੱਚ_ਬਿੱਟ != TICK_TYPE_ਚੌੜਾਈ_32_ਬਿੱਟ ) &&   \
    ( config_TICK_TYPE_ਚੌੜਾਈ_ਵਿੱਚ_ਬਿੱਟ != TICK_TYPE_ਚੌੜਾਈ_64_ਬਿੱਟ ) )
    #ਗਲਤੀ Macro config_TICK_TYPE_ਚੌੜਾਈ_ਵਿੱਚ_ਬਿੱਟ is defined ਮੰਜ਼ਿਲ incorrect ਮੁੱਲ.  See the Configuration section ਦਾ the FreeRTOS API documentation ਲਈ details.
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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configUSE_EVENT_GROUPS
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_EVENT_GROUPS    1
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_USE_ਧਾਰਾ_BUFFERS
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_USE_ਧਾਰਾ_BUFFERS    1
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configUSE_DAEMON_TASK_STARTUP_HOOK
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_DAEMON_TASK_STARTUP_HOOK    0
#ਜੇ_ਅੰਤ

#ਜੇ ( configUSE_DAEMON_TASK_STARTUP_HOOK != 0 )
    #ਜੇ ( configUSE_TIMERS == 0 )
        #ਗਲਤੀ configUSE_DAEMON_TASK_STARTUP_HOOK is ਸੈੱਟ, but the daemon task is not created because configUSE_TIMERS is 0.
    #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configUSE_COUNTING_SEMAPHORES
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_COUNTING_SEMAPHORES    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configUSE_TASK_PREEMPTION_DISABLE
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_TASK_PREEMPTION_DISABLE    0
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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configCHECK_HANDLER_INSTALLATION
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configCHECK_HANDLER_INSTALLATION    1
#ਨਹੀਂਤਾਂ

/* The application has explicitly defined configCHECK_HANDLER_INSTALLATION
 * to 1. The checks requires configASSERT() to be defined. */
    #ਜੇ ( ( configCHECK_HANDLER_INSTALLATION == 1 ) && ( configASSERT_DEFINED == 0 ) )
        #ਗਲਤੀ You must define configASSERT() when configCHECK_HANDLER_INSTALLATION is 1.
    #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_ਸਮ੍ਰਿਤੀ_BARRIER
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਸਮ੍ਰਿਤੀ_BARRIER()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_SOFTWARE_BARRIER
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_SOFTWARE_BARRIER()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_ਲੜੀਵਾਰ_ਗੁਣਜ_PRIORITIES
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_ਲੜੀਵਾਰ_ਗੁਣਜ_PRIORITIES    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_GET_CORE_ID

    #ਜੇ ( config_ਸੰਖਿਆ_ਦਾ_CORES == 1 )
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_GET_CORE_ID()    0
    #ਨਹੀਂਤਾਂ
        #ਗਲਤੀ config_ਸੰਖਿਆ_ਦਾ_CORES is ਸੈੱਟ ਮੰਜ਼ਿਲ ਹੋਰ than 1 then ਪੋਰਟ_GET_CORE_ID must also be defined.
    #ਜੇ_ਅੰਤ /* configNUMBER_OF_CORES */

#ਜੇ_ਅੰਤ /* portGET_CORE_ID */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_YIELD_CORE

    #ਜੇ ( config_ਸੰਖਿਆ_ਦਾ_CORES == 1 )
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_YIELD_CORE( x )    ਪੋਰਟ_YIELD()
    #ਨਹੀਂਤਾਂ
        #ਗਲਤੀ config_ਸੰਖਿਆ_ਦਾ_CORES is ਸੈੱਟ ਮੰਜ਼ਿਲ ਹੋਰ than 1 then ਪੋਰਟ_YIELD_CORE must also be defined.
    #ਜੇ_ਅੰਤ /* configNUMBER_OF_CORES */

#ਜੇ_ਅੰਤ /* portYIELD_CORE */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ

    #ਜੇ ( config_ਸੰਖਿਆ_ਦਾ_CORES > 1 )
        #ਗਲਤੀ ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ is required ਵਿੱਚ SMP
    #ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ /* portSET_INTERRUPT_MASK */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_CLEAR_ਰੁਕਾਵਟ_ਨਕਾਬ

    #ਜੇ ( config_ਸੰਖਿਆ_ਦਾ_CORES > 1 )
        #ਗਲਤੀ ਪੋਰਟ_CLEAR_ਰੁਕਾਵਟ_ਨਕਾਬ is required ਵਿੱਚ SMP
    #ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ /* portCLEAR_INTERRUPT_MASK */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_RELEASE_TASK_ਤਾਲਾ

    #ਜੇ ( config_ਸੰਖਿਆ_ਦਾ_CORES == 1 )
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_RELEASE_TASK_ਤਾਲਾ()
    #ਨਹੀਂਤਾਂ
        #ਗਲਤੀ ਪੋਰਟ_RELEASE_TASK_ਤਾਲਾ is required ਵਿੱਚ SMP
    #ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ /* portRELEASE_TASK_LOCK */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_GET_TASK_ਤਾਲਾ

    #ਜੇ ( config_ਸੰਖਿਆ_ਦਾ_CORES == 1 )
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_GET_TASK_ਤਾਲਾ()
    #ਨਹੀਂਤਾਂ
        #ਗਲਤੀ ਪੋਰਟ_GET_TASK_ਤਾਲਾ is required ਵਿੱਚ SMP
    #ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ /* portGET_TASK_LOCK */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_RELEASE_ISR_ਤਾਲਾ

    #ਜੇ ( config_ਸੰਖਿਆ_ਦਾ_CORES == 1 )
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_RELEASE_ISR_ਤਾਲਾ()
    #ਨਹੀਂਤਾਂ
        #ਗਲਤੀ ਪੋਰਟ_RELEASE_ISR_ਤਾਲਾ is required ਵਿੱਚ SMP
    #ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ /* portRELEASE_ISR_LOCK */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_GET_ISR_ਤਾਲਾ

    #ਜੇ ( config_ਸੰਖਿਆ_ਦਾ_CORES == 1 )
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_GET_ISR_ਤਾਲਾ()
    #ਨਹੀਂਤਾਂ
        #ਗਲਤੀ ਪੋਰਟ_GET_ISR_ਤਾਲਾ is required ਵਿੱਚ SMP
    #ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ /* portGET_ISR_LOCK */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_ENTER_CRITICAL_FROM_ISR

    #ਜੇ ( config_ਸੰਖਿਆ_ਦਾ_CORES > 1 )
        #ਗਲਤੀ ਪੋਰਟ_ENTER_CRITICAL_FROM_ISR is required ਵਿੱਚ SMP
    #ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_EXIT_CRITICAL_FROM_ISR

    #ਜੇ ( config_ਸੰਖਿਆ_ਦਾ_CORES > 1 )
        #ਗਲਤੀ ਪੋਰਟ_EXIT_CRITICAL_FROM_ISR is required ਵਿੱਚ SMP
    #ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configUSE_CORE_AFFINITY
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_CORE_AFFINITY    0
#ਜੇ_ਅੰਤ /* configUSE_CORE_AFFINITY */

#ਜੇ ( ( config_ਸੰਖਿਆ_ਦਾ_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
    #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configTASK_DEFAULT_CORE_AFFINITY
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configTASK_DEFAULT_CORE_AFFINITY    tskNO_AFFINITY
    #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configUSE_PASSIVE_IDLE_HOOK
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_PASSIVE_IDLE_HOOK    0
#ਜੇ_ਅੰਤ /* configUSE_PASSIVE_IDLE_HOOK */

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

    #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_TIMER_CALLBACK_ATTRIBUTE
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TIMER_CALLBACK_ATTRIBUTE
    #ਜੇ_ਅੰਤ /* portTIMER_CALLBACK_ATTRIBUTE */

#ਜੇ_ਅੰਤ /* configUSE_TIMERS */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_HAS_NESTED_INTERRUPTS
    #ਜੇ defined( ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR ) && defined( ਪੋਰਟ_CLEAR_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR )
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_HAS_NESTED_INTERRUPTS    1
    #ਨਹੀਂਤਾਂ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_HAS_NESTED_INTERRUPTS    0
    #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR
    #ਜੇ ( ਪੋਰਟ_HAS_NESTED_INTERRUPTS == 1 )
        #ਗਲਤੀ ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR must be defined ਲਈ ports that support nested interrupts (i.e. ਪੋਰਟ_HAS_NESTED_INTERRUPTS is ਸੈੱਟ ਮੰਜ਼ਿਲ 1)
    #ਨਹੀਂਤਾਂ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR()    0
    #ਜੇ_ਅੰਤ
#ਨਹੀਂਤਾਂ
    #ਜੇ ( ਪੋਰਟ_HAS_NESTED_INTERRUPTS == 0 )
        #ਗਲਤੀ ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR must not be defined ਲਈ ports that ਕਰੋ not support nested interrupts (i.e. ਪੋਰਟ_HAS_NESTED_INTERRUPTS is ਸੈੱਟ ਮੰਜ਼ਿਲ 0)
    #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_CLEAR_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR
    #ਜੇ ( ਪੋਰਟ_HAS_NESTED_INTERRUPTS == 1 )
        #ਗਲਤੀ ਪੋਰਟ_CLEAR_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR must be defined ਲਈ ports that support nested interrupts  (i.e. ਪੋਰਟ_HAS_NESTED_INTERRUPTS is ਸੈੱਟ ਮੰਜ਼ਿਲ 1)
    #ਨਹੀਂਤਾਂ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_CLEAR_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR( ux_Saved_ਸਥਿਤੀ_ਮੁੱਲ )    ( ਖਾਲੀ ) ( ux_Saved_ਸਥਿਤੀ_ਮੁੱਲ )
    #ਜੇ_ਅੰਤ
#ਨਹੀਂਤਾਂ
    #ਜੇ ( ਪੋਰਟ_HAS_NESTED_INTERRUPTS == 0 )
        #ਗਲਤੀ ਪੋਰਟ_CLEAR_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR must not be defined ਲਈ ports that ਕਰੋ not support nested interrupts (i.e. ਪੋਰਟ_HAS_NESTED_INTERRUPTS is ਸੈੱਟ ਮੰਜ਼ਿਲ 0)
    #ਜੇ_ਅੰਤ
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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_TASK_SWITCH_HOOK
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TASK_SWITCH_HOOK( pxTCB )    ( ਖਾਲੀ ) ( pxTCB )
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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_MOVED_TASK_ਮੰਜ਼ਿਲ_DELAYED_LIST
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_MOVED_TASK_ਮੰਜ਼ਿਲ_DELAYED_LIST()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_MOVED_TASK_ਮੰਜ਼ਿਲ_ਉਛਾਲ_DELAYED_LIST
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_MOVED_TASK_ਮੰਜ਼ਿਲ_ਉਛਾਲ_DELAYED_LIST()
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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ISR_EXIT_ਮੰਜ਼ਿਲ_SCHEDULER
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ISR_EXIT_ਮੰਜ਼ਿਲ_SCHEDULER()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceISR_EXIT
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceISR_EXIT()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceISR_ENTER
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceISR_ENTER()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਧਾਰਾ_ਬਫਰ_CREATE_FAILED
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਧਾਰਾ_ਬਫਰ_CREATE_FAILED( x_ਧਾਰਾ_ਬਫਰ_Type )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਧਾਰਾ_ਬਫਰ_CREATE_STATIC_FAILED
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਧਾਰਾ_ਬਫਰ_CREATE_STATIC_FAILED( xReturn, x_ਧਾਰਾ_ਬਫਰ_Type )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਧਾਰਾ_ਬਫਰ_CREATE
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਧਾਰਾ_ਬਫਰ_CREATE( px_ਧਾਰਾ_ਬਫਰ, x_ਧਾਰਾ_ਬਫਰ_Type )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਧਾਰਾ_ਬਫਰ_DELETE
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਧਾਰਾ_ਬਫਰ_DELETE( x_ਧਾਰਾ_ਬਫਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਧਾਰਾ_ਬਫਰ_RESET
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਧਾਰਾ_ਬਫਰ_RESET( x_ਧਾਰਾ_ਬਫਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ਧਾਰਾ_ਬਫਰ_RESET_FROM_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ਧਾਰਾ_ਬਫਰ_RESET_FROM_ISR( x_ਧਾਰਾ_ਬਫਰ )
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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_Event_ਸਮੂਹ_Create_Static
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_Event_ਸਮੂਹ_Create_Static( px_Event_ਸਮੂਹ_ਬਫਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_Event_ਸਮੂਹ_Create_Static
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_Event_ਸਮੂਹ_Create_Static( px_Event_ਬਿੱਟ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_Event_ਸਮੂਹ_Create
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_Event_ਸਮੂਹ_Create()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_Event_ਸਮੂਹ_Create
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_Event_ਸਮੂਹ_Create( px_Event_ਬਿੱਟ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_Event_ਸਮੂਹ_Sync
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_Event_ਸਮੂਹ_Sync( x_Event_ਸਮੂਹ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For, x_Ticks_ਮੰਜ਼ਿਲ_Wait )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_Event_ਸਮੂਹ_Sync
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_Event_ਸਮੂਹ_Sync( uxReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_Event_ਸਮੂਹ_Wait_ਬਿੱਟ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_Event_ਸਮੂਹ_Wait_ਬਿੱਟ( x_Event_ਸਮੂਹ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For, xClearOnExit, x_Wait_For_All_ਬਿੱਟ, x_Ticks_ਮੰਜ਼ਿਲ_Wait )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_Event_ਸਮੂਹ_Wait_ਬਿੱਟ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_Event_ਸਮੂਹ_Wait_ਬਿੱਟ( uxReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_Event_ਸਮੂਹ_Clear_ਬਿੱਟ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_Event_ਸਮੂਹ_Clear_ਬਿੱਟ( x_Event_ਸਮੂਹ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_Event_ਸਮੂਹ_Clear_ਬਿੱਟ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_Event_ਸਮੂਹ_Clear_ਬਿੱਟ( uxReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_Event_ਸਮੂਹ_Clear_ਬਿੱਟ_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_Event_ਸਮੂਹ_Clear_ਬਿੱਟ_From_ISR( x_Event_ਸਮੂਹ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_Event_ਸਮੂਹ_Clear_ਬਿੱਟ_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_Event_ਸਮੂਹ_Clear_ਬਿੱਟ_From_ISR( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_Event_ਸਮੂਹ_Get_ਬਿੱਟ_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_Event_ਸਮੂਹ_Get_ਬਿੱਟ_From_ISR( x_Event_ਸਮੂਹ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_Event_ਸਮੂਹ_Get_ਬਿੱਟ_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_Event_ਸਮੂਹ_Get_ਬਿੱਟ_From_ISR( uxReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ( x_Event_ਸਮੂਹ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ( ux_Event_ਬਿੱਟ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_v_Event_ਸਮੂਹ_Delete
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_v_Event_ਸਮੂਹ_Delete( x_Event_ਸਮੂਹ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_v_Event_ਸਮੂਹ_Delete
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_v_Event_ਸਮੂਹ_Delete()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_Event_ਸਮੂਹ_Get_Static_ਬਫਰ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_Event_ਸਮੂਹ_Get_Static_ਬਫਰ( x_Event_ਸਮੂਹ, ppx_Event_ਸਮੂਹ_ਬਫਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_Event_ਸਮੂਹ_Get_Static_ਬਫਰ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_Event_ਸਮੂਹ_Get_Static_ਬਫਰ( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_v_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ_Callback
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_v_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ_Callback( pv_Event_ਸਮੂਹ, ul_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_v_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ_Callback
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_v_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ_Callback()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_v_Event_ਸਮੂਹ_Clear_ਬਿੱਟ_Callback
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_v_Event_ਸਮੂਹ_Clear_ਬਿੱਟ_Callback( pv_Event_ਸਮੂਹ, ul_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_v_Event_ਸਮੂਹ_Clear_ਬਿੱਟ_Callback
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_v_Event_ਸਮੂਹ_Clear_ਬਿੱਟ_Callback()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ_From_ISR( x_Event_ਸਮੂਹ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ, px_Higher_ਤਰਜੀਹ_Task_Woken )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ_From_ISR( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_ux_Event_ਸਮੂਹ_Get_ਸੰਖਿਆ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_ux_Event_ਸਮੂਹ_Get_ਸੰਖਿਆ( x_Event_ਸਮੂਹ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_ux_Event_ਸਮੂਹ_Get_ਸੰਖਿਆ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_ux_Event_ਸਮੂਹ_Get_ਸੰਖਿਆ( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_v_Event_ਸਮੂਹ_ਸੈੱਟ_ਸੰਖਿਆ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_v_Event_ਸਮੂਹ_ਸੈੱਟ_ਸੰਖਿਆ( x_Event_ਸਮੂਹ, ux_Event_ਸਮੂਹ_ਸੰਖਿਆ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_v_Event_ਸਮੂਹ_ਸੈੱਟ_ਸੰਖਿਆ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_v_Event_ਸਮੂਹ_ਸੈੱਟ_ਸੰਖਿਆ()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਕਤਾਰ_Generic_Reset
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਕਤਾਰ_Generic_Reset( x_ਕਤਾਰ, x_New_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਕਤਾਰ_Generic_Reset
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਕਤਾਰ_Generic_Reset( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਕਤਾਰ_Generic_Create_Static
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਕਤਾਰ_Generic_Create_Static( ux_ਕਤਾਰ_ਲੰਮਾਈ, ux_ਵਸਤੂ_ਮਾਪ, puc_ਕਤਾਰ_Storage, px_Static_ਕਤਾਰ, uc_ਕਤਾਰ_Type )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਕਤਾਰ_Generic_Create_Static
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਕਤਾਰ_Generic_Create_Static( px_New_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਕਤਾਰ_Generic_Get_Static_Buffers
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਕਤਾਰ_Generic_Get_Static_Buffers( x_ਕਤਾਰ, ppuc_ਕਤਾਰ_Storage, ppx_Static_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਕਤਾਰ_Generic_Get_Static_Buffers
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਕਤਾਰ_Generic_Get_Static_Buffers( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਕਤਾਰ_Generic_Create
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਕਤਾਰ_Generic_Create( ux_ਕਤਾਰ_ਲੰਮਾਈ, ux_ਵਸਤੂ_ਮਾਪ, uc_ਕਤਾਰ_Type )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਕਤਾਰ_Generic_Create
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਕਤਾਰ_Generic_Create( px_New_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਕਤਾਰ_Create_Mutex
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਕਤਾਰ_Create_Mutex( uc_ਕਤਾਰ_Type )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਕਤਾਰ_Create_Mutex
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਕਤਾਰ_Create_Mutex( x_New_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਕਤਾਰ_Create_Mutex_Static
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਕਤਾਰ_Create_Mutex_Static( uc_ਕਤਾਰ_Type, px_Static_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਕਤਾਰ_Create_Mutex_Static
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਕਤਾਰ_Create_Mutex_Static( x_New_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਕਤਾਰ_Get_Mutex_Holder
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਕਤਾਰ_Get_Mutex_Holder( xSemaphore )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਕਤਾਰ_Get_Mutex_Holder
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਕਤਾਰ_Get_Mutex_Holder( pxReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਕਤਾਰ_Get_Mutex_Holder_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਕਤਾਰ_Get_Mutex_Holder_From_ISR( xSemaphore )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਕਤਾਰ_Get_Mutex_Holder_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਕਤਾਰ_Get_Mutex_Holder_From_ISR( pxReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਕਤਾਰ_Give_Mutex_Recursive
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਕਤਾਰ_Give_Mutex_Recursive( xMutex )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਕਤਾਰ_Give_Mutex_Recursive
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਕਤਾਰ_Give_Mutex_Recursive( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਕਤਾਰ_Take_Mutex_Recursive
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਕਤਾਰ_Take_Mutex_Recursive( xMutex, x_Ticks_ਮੰਜ਼ਿਲ_Wait )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਕਤਾਰ_Take_Mutex_Recursive
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਕਤਾਰ_Take_Mutex_Recursive( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਕਤਾਰ_Create_Counting_Semaphore_Static
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਕਤਾਰ_Create_Counting_Semaphore_Static( ux_Max_ਗਿਣਤੀ, ux_Initial_ਗਿਣਤੀ, px_Static_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਕਤਾਰ_Create_Counting_Semaphore_Static
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਕਤਾਰ_Create_Counting_Semaphore_Static( xHandle )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਕਤਾਰ_Create_Counting_Semaphore
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਕਤਾਰ_Create_Counting_Semaphore( ux_Max_ਗਿਣਤੀ, ux_Initial_ਗਿਣਤੀ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਕਤਾਰ_Create_Counting_Semaphore
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਕਤਾਰ_Create_Counting_Semaphore( xHandle )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਕਤਾਰ_Generic_Send
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਕਤਾਰ_Generic_Send( x_ਕਤਾਰ, pv_ਵਸਤੂ_ਮੰਜ਼ਿਲ_ਕਤਾਰ, x_Ticks_ਮੰਜ਼ਿਲ_Wait, xCopyPosition )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਕਤਾਰ_Generic_Send
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਕਤਾਰ_Generic_Send( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਕਤਾਰ_Generic_Send_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਕਤਾਰ_Generic_Send_From_ISR( x_ਕਤਾਰ, pv_ਵਸਤੂ_ਮੰਜ਼ਿਲ_ਕਤਾਰ, px_Higher_ਤਰਜੀਹ_Task_Woken, xCopyPosition )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਕਤਾਰ_Generic_Send_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਕਤਾਰ_Generic_Send_From_ISR( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਕਤਾਰ_Give_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਕਤਾਰ_Give_From_ISR( x_ਕਤਾਰ, px_Higher_ਤਰਜੀਹ_Task_Woken )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਕਤਾਰ_Give_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਕਤਾਰ_Give_From_ISR( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਕਤਾਰ_Receive
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਕਤਾਰ_Receive( x_ਕਤਾਰ, pv_ਬਫਰ, x_Ticks_ਮੰਜ਼ਿਲ_Wait )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਕਤਾਰ_Receive
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਕਤਾਰ_Receive( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਕਤਾਰ_Semaphore_Take
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਕਤਾਰ_Semaphore_Take( x_ਕਤਾਰ, x_Ticks_ਮੰਜ਼ਿਲ_Wait )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਕਤਾਰ_Semaphore_Take
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਕਤਾਰ_Semaphore_Take( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਕਤਾਰ_Peek
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਕਤਾਰ_Peek( x_ਕਤਾਰ, pv_ਬਫਰ, x_Ticks_ਮੰਜ਼ਿਲ_Wait )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਕਤਾਰ_Peek
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਕਤਾਰ_Peek( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਕਤਾਰ_Receive_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਕਤਾਰ_Receive_From_ISR( x_ਕਤਾਰ, pv_ਬਫਰ, px_Higher_ਤਰਜੀਹ_Task_Woken )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਕਤਾਰ_Receive_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਕਤਾਰ_Receive_From_ISR( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਕਤਾਰ_Peek_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਕਤਾਰ_Peek_From_ISR( x_ਕਤਾਰ, pv_ਬਫਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਕਤਾਰ_Peek_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਕਤਾਰ_Peek_From_ISR( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_ux_ਕਤਾਰ_Messages_ਉਡੀਕ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_ux_ਕਤਾਰ_Messages_ਉਡੀਕ( x_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_ux_ਕਤਾਰ_Messages_ਉਡੀਕ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_ux_ਕਤਾਰ_Messages_ਉਡੀਕ( uxReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_ux_ਕਤਾਰ_Spaces_ਉਪਲਬਧ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_ux_ਕਤਾਰ_Spaces_ਉਪਲਬਧ( x_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_ux_ਕਤਾਰ_Spaces_ਉਪਲਬਧ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_ux_ਕਤਾਰ_Spaces_ਉਪਲਬਧ( uxReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_ux_ਕਤਾਰ_Messages_ਉਡੀਕ_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_ux_ਕਤਾਰ_Messages_ਉਡੀਕ_From_ISR( x_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_ux_ਕਤਾਰ_Messages_ਉਡੀਕ_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_ux_ਕਤਾਰ_Messages_ਉਡੀਕ_From_ISR( uxReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_v_ਕਤਾਰ_Delete
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_v_ਕਤਾਰ_Delete( x_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_v_ਕਤਾਰ_Delete
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_v_ਕਤਾਰ_Delete()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_ux_ਕਤਾਰ_Get_ਕਤਾਰ_ਸੰਖਿਆ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_ux_ਕਤਾਰ_Get_ਕਤਾਰ_ਸੰਖਿਆ( x_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_ux_ਕਤਾਰ_Get_ਕਤਾਰ_ਸੰਖਿਆ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_ux_ਕਤਾਰ_Get_ਕਤਾਰ_ਸੰਖਿਆ( ux_ਕਤਾਰ_ਸੰਖਿਆ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_v_ਕਤਾਰ_ਸੈੱਟ_ਕਤਾਰ_ਸੰਖਿਆ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_v_ਕਤਾਰ_ਸੈੱਟ_ਕਤਾਰ_ਸੰਖਿਆ( x_ਕਤਾਰ, ux_ਕਤਾਰ_ਸੰਖਿਆ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_v_ਕਤਾਰ_ਸੈੱਟ_ਕਤਾਰ_ਸੰਖਿਆ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_v_ਕਤਾਰ_ਸੈੱਟ_ਕਤਾਰ_ਸੰਖਿਆ()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_uc_ਕਤਾਰ_Get_ਕਤਾਰ_Type
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_uc_ਕਤਾਰ_Get_ਕਤਾਰ_Type( x_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_uc_ਕਤਾਰ_Get_ਕਤਾਰ_Type
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_uc_ਕਤਾਰ_Get_ਕਤਾਰ_Type( uc_ਕਤਾਰ_Type )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_ux_ਕਤਾਰ_Get_ਕਤਾਰ_ਵਸਤੂ_ਮਾਪ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_ux_ਕਤਾਰ_Get_ਕਤਾਰ_ਵਸਤੂ_ਮਾਪ( x_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_ux_ਕਤਾਰ_Get_ਕਤਾਰ_ਵਸਤੂ_ਮਾਪ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_ux_ਕਤਾਰ_Get_ਕਤਾਰ_ਵਸਤੂ_ਮਾਪ( ux_ਵਸਤੂ_ਮਾਪ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_ux_ਕਤਾਰ_Get_ਕਤਾਰ_ਲੰਮਾਈ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_ux_ਕਤਾਰ_Get_ਕਤਾਰ_ਲੰਮਾਈ( x_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_ux_ਕਤਾਰ_Get_ਕਤਾਰ_ਲੰਮਾਈ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_ux_ਕਤਾਰ_Get_ਕਤਾਰ_ਲੰਮਾਈ( ux_ਲੰਮਾਈ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਕਤਾਰ_Is_ਕਤਾਰ_ਖਾਲੀ_ਥਾਵਾਂ_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਕਤਾਰ_Is_ਕਤਾਰ_ਖਾਲੀ_ਥਾਵਾਂ_From_ISR( x_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਕਤਾਰ_Is_ਕਤਾਰ_ਖਾਲੀ_ਥਾਵਾਂ_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਕਤਾਰ_Is_ਕਤਾਰ_ਖਾਲੀ_ਥਾਵਾਂ_From_ISR( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਕਤਾਰ_Is_ਕਤਾਰ_ਭਰਿਆ_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਕਤਾਰ_Is_ਕਤਾਰ_ਭਰਿਆ_From_ISR( x_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਕਤਾਰ_Is_ਕਤਾਰ_ਭਰਿਆ_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਕਤਾਰ_Is_ਕਤਾਰ_ਭਰਿਆ_From_ISR( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਕਤਾਰ_CRSend
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਕਤਾਰ_CRSend( x_ਕਤਾਰ, pv_ਵਸਤੂ_ਮੰਜ਼ਿਲ_ਕਤਾਰ, x_Ticks_ਮੰਜ਼ਿਲ_Wait )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਕਤਾਰ_CRSend
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਕਤਾਰ_CRSend( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਕਤਾਰ_CRReceive
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਕਤਾਰ_CRReceive( x_ਕਤਾਰ, pv_ਬਫਰ, x_Ticks_ਮੰਜ਼ਿਲ_Wait )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਕਤਾਰ_CRReceive
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਕਤਾਰ_CRReceive( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਕਤਾਰ_CRSend_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਕਤਾਰ_CRSend_From_ISR( x_ਕਤਾਰ, pv_ਵਸਤੂ_ਮੰਜ਼ਿਲ_ਕਤਾਰ, xCoRoutinePreviouslyWoken )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਕਤਾਰ_CRSend_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਕਤਾਰ_CRSend_From_ISR( xCoRoutinePreviouslyWoken )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਕਤਾਰ_CRReceive_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਕਤਾਰ_CRReceive_From_ISR( x_ਕਤਾਰ, pv_ਬਫਰ, pxCoRoutineWoken )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਕਤਾਰ_CRReceive_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਕਤਾਰ_CRReceive_From_ISR( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_v_ਕਤਾਰ_ਜੋੜੋ_ਮੰਜ਼ਿਲ_Registry
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_v_ਕਤਾਰ_ਜੋੜੋ_ਮੰਜ਼ਿਲ_Registry( x_ਕਤਾਰ, pc_ਕਤਾਰ_Name )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_v_ਕਤਾਰ_ਜੋੜੋ_ਮੰਜ਼ਿਲ_Registry
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_v_ਕਤਾਰ_ਜੋੜੋ_ਮੰਜ਼ਿਲ_Registry()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_pc_ਕਤਾਰ_Get_Name
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_pc_ਕਤਾਰ_Get_Name( x_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_pc_ਕਤਾਰ_Get_Name
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_pc_ਕਤਾਰ_Get_Name( pcReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_v_ਕਤਾਰ_Unregister_ਕਤਾਰ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_v_ਕਤਾਰ_Unregister_ਕਤਾਰ( x_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_v_ਕਤਾਰ_Unregister_ਕਤਾਰ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_v_ਕਤਾਰ_Unregister_ਕਤਾਰ()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_v_ਕਤਾਰ_Wait_For_ਸੁਨੇਹਾ_Restricted
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_v_ਕਤਾਰ_Wait_For_ਸੁਨੇਹਾ_Restricted( x_ਕਤਾਰ, x_Ticks_ਮੰਜ਼ਿਲ_Wait, xWaitIndefinitely )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_v_ਕਤਾਰ_Wait_For_ਸੁਨੇਹਾ_Restricted
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_v_ਕਤਾਰ_Wait_For_ਸੁਨੇਹਾ_Restricted()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਕਤਾਰ_Create_ਸੈੱਟ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਕਤਾਰ_Create_ਸੈੱਟ( ux_Event_ਕਤਾਰ_ਲੰਮਾਈ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਕਤਾਰ_Create_ਸੈੱਟ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਕਤਾਰ_Create_ਸੈੱਟ( px_ਕਤਾਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਕਤਾਰ_ਜੋੜੋ_ਮੰਜ਼ਿਲ_ਸੈੱਟ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਕਤਾਰ_ਜੋੜੋ_ਮੰਜ਼ਿਲ_ਸੈੱਟ( x_ਕਤਾਰ_Or_Semaphore, x_ਕਤਾਰ_ਸੈੱਟ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਕਤਾਰ_ਜੋੜੋ_ਮੰਜ਼ਿਲ_ਸੈੱਟ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਕਤਾਰ_ਜੋੜੋ_ਮੰਜ਼ਿਲ_ਸੈੱਟ( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਕਤਾਰ_Remove_From_ਸੈੱਟ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਕਤਾਰ_Remove_From_ਸੈੱਟ( x_ਕਤਾਰ_Or_Semaphore, x_ਕਤਾਰ_ਸੈੱਟ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਕਤਾਰ_Remove_From_ਸੈੱਟ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਕਤਾਰ_Remove_From_ਸੈੱਟ( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਕਤਾਰ_Select_From_ਸੈੱਟ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਕਤਾਰ_Select_From_ਸੈੱਟ( x_ਕਤਾਰ_ਸੈੱਟ, x_Ticks_ਮੰਜ਼ਿਲ_Wait )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਕਤਾਰ_Select_From_ਸੈੱਟ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਕਤਾਰ_Select_From_ਸੈੱਟ( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਕਤਾਰ_Select_From_ਸੈੱਟ_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਕਤਾਰ_Select_From_ਸੈੱਟ_From_ISR( x_ਕਤਾਰ_ਸੈੱਟ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਕਤਾਰ_Select_From_ਸੈੱਟ_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਕਤਾਰ_Select_From_ਸੈੱਟ_From_ISR( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_xTimerCreateTimerTask
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_xTimerCreateTimerTask()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_xTimerCreateTimerTask
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_xTimerCreateTimerTask( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_xTimerCreate
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_xTimerCreate( pcTimerName, x_Timer_Period_ਵਿੱਚ_Ticks, xAutoReload, pvTimerID, pxCallbackFunction )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_xTimerCreate
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_xTimerCreate( pxNewTimer )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_xTimerCreateStatic
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_xTimerCreateStatic( pcTimerName, x_Timer_Period_ਵਿੱਚ_Ticks, xAutoReload, pvTimerID, pxCallbackFunction, px_Timer_ਬਫਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_xTimerCreateStatic
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_xTimerCreateStatic( pxNewTimer )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_xTimerGenericCommandFromTask
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_xTimerGenericCommandFromTask( xTimer, xCommandID, x_Optional_ਮੁੱਲ, px_Higher_ਤਰਜੀਹ_Task_Woken, x_Ticks_ਮੰਜ਼ਿਲ_Wait )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_xTimerGenericCommandFromTask
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_xTimerGenericCommandFromTask( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_xTimerGenericCommandFromISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_xTimerGenericCommandFromISR( xTimer, xCommandID, x_Optional_ਮੁੱਲ, px_Higher_ਤਰਜੀਹ_Task_Woken, x_Ticks_ਮੰਜ਼ਿਲ_Wait )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_xTimerGenericCommandFromISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_xTimerGenericCommandFromISR( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_xTimerGetTimerDaemonTaskHandle
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_xTimerGetTimerDaemonTaskHandle()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_xTimerGetTimerDaemonTaskHandle
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_xTimerGetTimerDaemonTaskHandle( xTimerTaskHandle )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_xTimerGetPeriod
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_xTimerGetPeriod( xTimer )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_xTimerGetPeriod
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_xTimerGetPeriod( x_Timer_Period_ਵਿੱਚ_Ticks )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_v_Timer_ਸੈੱਟ_Reload_ਬਹੁਲਕ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_v_Timer_ਸੈੱਟ_Reload_ਬਹੁਲਕ( xTimer, xAutoReload )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_v_Timer_ਸੈੱਟ_Reload_ਬਹੁਲਕ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_v_Timer_ਸੈੱਟ_Reload_ਬਹੁਲਕ()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_Timer_Get_Reload_ਬਹੁਲਕ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_Timer_Get_Reload_ਬਹੁਲਕ( xTimer )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_Timer_Get_Reload_ਬਹੁਲਕ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_Timer_Get_Reload_ਬਹੁਲਕ( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_ux_Timer_Get_Reload_ਬਹੁਲਕ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_ux_Timer_Get_Reload_ਬਹੁਲਕ( xTimer )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_ux_Timer_Get_Reload_ਬਹੁਲਕ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_ux_Timer_Get_Reload_ਬਹੁਲਕ( uxReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_Timer_Get_Expiry_ਸਮਾਂ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_Timer_Get_Expiry_ਸਮਾਂ( xTimer )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_Timer_Get_Expiry_ਸਮਾਂ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_Timer_Get_Expiry_ਸਮਾਂ( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_Timer_Get_Static_ਬਫਰ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_Timer_Get_Static_ਬਫਰ( xTimer, ppx_Timer_ਬਫਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_Timer_Get_Static_ਬਫਰ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_Timer_Get_Static_ਬਫਰ( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_pcTimerGetName
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_pcTimerGetName( xTimer )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_pcTimerGetName
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_pcTimerGetName( pcTimerName )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_Timer_Is_Timer_ਸਰਗਰਮ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_Timer_Is_Timer_ਸਰਗਰਮ( xTimer )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_Timer_Is_Timer_ਸਰਗਰਮ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_Timer_Is_Timer_ਸਰਗਰਮ( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_pvTimerGetTimerID
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_pvTimerGetTimerID( xTimer )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_pvTimerGetTimerID
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_pvTimerGetTimerID( pvReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_v_Timer_ਸੈੱਟ_Timer_ID
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_v_Timer_ਸੈੱਟ_Timer_ID( xTimer, pvNewID )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_v_Timer_ਸੈੱਟ_Timer_ID
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_v_Timer_ਸੈੱਟ_Timer_ID()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_xTimerPendFunctionCallFromISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_xTimerPendFunctionCallFromISR( x_Function_ਮੰਜ਼ਿਲ_Pend, pvParameter1, ulParameter2, px_Higher_ਤਰਜੀਹ_Task_Woken )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_xTimerPendFunctionCallFromISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_xTimerPendFunctionCallFromISR( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_xTimerPendFunctionCall
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_xTimerPendFunctionCall( x_Function_ਮੰਜ਼ਿਲ_Pend, pvParameter1, ulParameter2, x_Ticks_ਮੰਜ਼ਿਲ_Wait )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_xTimerPendFunctionCall
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_xTimerPendFunctionCall( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_ux_Timer_Get_Timer_ਸੰਖਿਆ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_ux_Timer_Get_Timer_ਸੰਖਿਆ( xTimer )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_ux_Timer_Get_Timer_ਸੰਖਿਆ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_ux_Timer_Get_Timer_ਸੰਖਿਆ( ux_Timer_ਸੰਖਿਆ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_v_Timer_ਸੈੱਟ_Timer_ਸੰਖਿਆ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_v_Timer_ਸੈੱਟ_Timer_ਸੰਖਿਆ( xTimer, ux_Timer_ਸੰਖਿਆ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_v_Timer_ਸੈੱਟ_Timer_ਸੰਖਿਆ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_v_Timer_ਸੈੱਟ_Timer_ਸੰਖਿਆ()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_xTaskCreateStatic
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_xTaskCreateStatic( px_Task_ਕੋਡ, pcName, ux_ਢੇਰ_ਡੂੰਘਾਈ, pvParameters, ux_ਤਰਜੀਹ, pux_ਢੇਰ_ਬਫਰ, px_Task_ਬਫਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_xTaskCreateStatic
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_xTaskCreateStatic( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_Task_Create_Static_Affinity_ਸੈੱਟ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_Task_Create_Static_Affinity_ਸੈੱਟ( px_Task_ਕੋਡ, pcName, ux_ਢੇਰ_ਡੂੰਘਾਈ, pvParameters, ux_ਤਰਜੀਹ, pux_ਢੇਰ_ਬਫਰ, px_Task_ਬਫਰ, ux_Core_Affinity_ਨਕਾਬ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_Task_Create_Static_Affinity_ਸੈੱਟ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_Task_Create_Static_Affinity_ਸੈੱਟ( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_xTaskCreateRestrictedStatic
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_xTaskCreateRestrictedStatic( pxTaskDefinition, pxCreatedTask )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_xTaskCreateRestrictedStatic
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_xTaskCreateRestrictedStatic( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_Task_Create_Restricted_Static_Affinity_ਸੈੱਟ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_Task_Create_Restricted_Static_Affinity_ਸੈੱਟ( pxTaskDefinition, ux_Core_Affinity_ਨਕਾਬ, pxCreatedTask )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_Task_Create_Restricted_Static_Affinity_ਸੈੱਟ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_Task_Create_Restricted_Static_Affinity_ਸੈੱਟ( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_xTaskCreateRestricted
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_xTaskCreateRestricted( pxTaskDefinition, pxCreatedTask )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_xTaskCreateRestricted
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_xTaskCreateRestricted( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_Task_Create_Restricted_Affinity_ਸੈੱਟ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_Task_Create_Restricted_Affinity_ਸੈੱਟ( pxTaskDefinition, ux_Core_Affinity_ਨਕਾਬ, pxCreatedTask )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_Task_Create_Restricted_Affinity_ਸੈੱਟ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_Task_Create_Restricted_Affinity_ਸੈੱਟ( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_xTaskCreate
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_xTaskCreate( px_Task_ਕੋਡ, pcName, ux_ਢੇਰ_ਡੂੰਘਾਈ, pvParameters, ux_ਤਰਜੀਹ, pxCreatedTask )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_xTaskCreate
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_xTaskCreate( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_Task_Create_Affinity_ਸੈੱਟ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_Task_Create_Affinity_ਸੈੱਟ( px_Task_ਕੋਡ, pcName, ux_ਢੇਰ_ਡੂੰਘਾਈ, pvParameters, ux_ਤਰਜੀਹ, ux_Core_Affinity_ਨਕਾਬ, pxCreatedTask )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_Task_Create_Affinity_ਸੈੱਟ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_Task_Create_Affinity_ਸੈੱਟ( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_vTaskDelete
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_vTaskDelete( x_Task_ਮੰਜ਼ਿਲ_Delete )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_vTaskDelete
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_vTaskDelete()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_xTaskDelayUntil
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_xTaskDelayUntil( px_ਪਿਛਲਾ_Wake_ਸਮਾਂ, x_ਸਮਾਂ_Increment )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_xTaskDelayUntil
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_xTaskDelayUntil( xShouldDelay )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_vTaskDelay
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_vTaskDelay( x_Ticks_ਮੰਜ਼ਿਲ_Delay )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_vTaskDelay
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_vTaskDelay()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_eTaskGetState
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_eTaskGetState( xTask )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_eTaskGetState
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_eTaskGetState( eReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_ux_Task_ਤਰਜੀਹ_Get
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_ux_Task_ਤਰਜੀਹ_Get( xTask )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_ux_Task_ਤਰਜੀਹ_Get
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_ux_Task_ਤਰਜੀਹ_Get( uxReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_ux_Task_ਤਰਜੀਹ_Get_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_ux_Task_ਤਰਜੀਹ_Get_From_ISR( xTask )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_ux_Task_ਤਰਜੀਹ_Get_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_ux_Task_ਤਰਜੀਹ_Get_From_ISR( uxReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_ux_Task_ਅਧਾਰ_ਤਰਜੀਹ_Get
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_ux_Task_ਅਧਾਰ_ਤਰਜੀਹ_Get( xTask )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_ux_Task_ਅਧਾਰ_ਤਰਜੀਹ_Get
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_ux_Task_ਅਧਾਰ_ਤਰਜੀਹ_Get( uxReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_ux_Task_ਅਧਾਰ_ਤਰਜੀਹ_Get_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_ux_Task_ਅਧਾਰ_ਤਰਜੀਹ_Get_From_ISR( xTask )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_ux_Task_ਅਧਾਰ_ਤਰਜੀਹ_Get_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_ux_Task_ਅਧਾਰ_ਤਰਜੀਹ_Get_From_ISR( uxReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_v_Task_ਤਰਜੀਹ_ਸੈੱਟ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_v_Task_ਤਰਜੀਹ_ਸੈੱਟ( xTask, ux_New_ਤਰਜੀਹ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_v_Task_ਤਰਜੀਹ_ਸੈੱਟ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_v_Task_ਤਰਜੀਹ_ਸੈੱਟ()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_v_Task_Core_Affinity_ਸੈੱਟ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_v_Task_Core_Affinity_ਸੈੱਟ( xTask, ux_Core_Affinity_ਨਕਾਬ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_v_Task_Core_Affinity_ਸੈੱਟ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_v_Task_Core_Affinity_ਸੈੱਟ()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_vTaskCoreAffinityGet
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_vTaskCoreAffinityGet( xTask )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_vTaskCoreAffinityGet
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_vTaskCoreAffinityGet( ux_Core_Affinity_ਨਕਾਬ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_vTaskPreemptionDisable
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_vTaskPreemptionDisable( xTask )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_vTaskPreemptionDisable
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_vTaskPreemptionDisable()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_vTaskPreemptionEnable
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_vTaskPreemptionEnable( xTask )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_vTaskPreemptionEnable
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_vTaskPreemptionEnable()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_vTaskSuspend
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_vTaskSuspend( x_Task_ਮੰਜ਼ਿਲ_Suspend )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_vTaskSuspend
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_vTaskSuspend()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_vTaskResume
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_vTaskResume( x_Task_ਮੰਜ਼ਿਲ_Resume )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_vTaskResume
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_vTaskResume()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_xTaskResumeFromISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_xTaskResumeFromISR( x_Task_ਮੰਜ਼ਿਲ_Resume )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_xTaskResumeFromISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_xTaskResumeFromISR( xYieldRequired )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_v_Task_ਸ਼ੁਰੂ_Scheduler
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_v_Task_ਸ਼ੁਰੂ_Scheduler()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_v_Task_ਸ਼ੁਰੂ_Scheduler
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_v_Task_ਸ਼ੁਰੂ_Scheduler()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_v_Task_ਸਿਰਾ_Scheduler
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_v_Task_ਸਿਰਾ_Scheduler()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_v_Task_ਸਿਰਾ_Scheduler
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_v_Task_ਸਿਰਾ_Scheduler()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_vTaskSuspendAll
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_vTaskSuspendAll()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_vTaskSuspendAll
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_vTaskSuspendAll()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_xTaskResumeAll
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_xTaskResumeAll()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_xTaskResumeAll
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_xTaskResumeAll( xAlreadyYielded )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_Task_Get_Tick_ਗਿਣਤੀ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_Task_Get_Tick_ਗਿਣਤੀ()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_Task_Get_Tick_ਗਿਣਤੀ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_Task_Get_Tick_ਗਿਣਤੀ( xTicks )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_Task_Get_Tick_ਗਿਣਤੀ_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_Task_Get_Tick_ਗਿਣਤੀ_From_ISR()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_Task_Get_Tick_ਗਿਣਤੀ_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_Task_Get_Tick_ਗਿਣਤੀ_From_ISR( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_ux_Task_Get_ਸੰਖਿਆ_ਦਾ_Tasks
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_ux_Task_Get_ਸੰਖਿਆ_ਦਾ_Tasks()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_ux_Task_Get_ਸੰਖਿਆ_ਦਾ_Tasks
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_ux_Task_Get_ਸੰਖਿਆ_ਦਾ_Tasks( ux_Current_ਸੰਖਿਆ_ਦਾ_Tasks )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_pcTaskGetName
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_pcTaskGetName( x_Task_ਮੰਜ਼ਿਲ_Query )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_pcTaskGetName
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_pcTaskGetName( pcTaskName )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_xTaskGetHandle
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_xTaskGetHandle( pc_Name_ਮੰਜ਼ਿਲ_Query )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_xTaskGetHandle
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_xTaskGetHandle( pxTCB )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_xTaskGetStaticBuffers
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_xTaskGetStaticBuffers( xTask, ppux_ਢੇਰ_ਬਫਰ, ppx_Task_ਬਫਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_xTaskGetStaticBuffers
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_xTaskGetStaticBuffers( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_uxTaskGetSystemState
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_uxTaskGetSystemState( px_Task_ਸਥਿਤੀ_Array, ux_Array_ਮਾਪ, pul_ਕੁੱਲ_ਲੜੀਵਾਰ_ਸਮਾਂ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_uxTaskGetSystemState
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_uxTaskGetSystemState( uxTask )
#ਜੇ_ਅੰਤ

#ਜੇ ( config_ਸੰਖਿਆ_ਦਾ_CORES == 1 )
    #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_xTaskGetIdleTaskHandle
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_xTaskGetIdleTaskHandle()
    #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਜੇ ( config_ਸੰਖਿਆ_ਦਾ_CORES == 1 )
    #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_xTaskGetIdleTaskHandle
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_xTaskGetIdleTaskHandle( xIdleTaskHandle )
    #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_xTaskGetIdleTaskHandleForCore
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_xTaskGetIdleTaskHandleForCore( xCoreID )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_xTaskGetIdleTaskHandleForCore
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_xTaskGetIdleTaskHandleForCore( xIdleTaskHandle )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_v_Task_ਕਦਮ_Tick
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_v_Task_ਕਦਮ_Tick( x_Ticks_ਮੰਜ਼ਿਲ_Jump )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_v_Task_ਕਦਮ_Tick
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_v_Task_ਕਦਮ_Tick()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_Task_Catch_ਉੱਪਰ_Ticks
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_Task_Catch_ਉੱਪਰ_Ticks( x_Ticks_ਮੰਜ਼ਿਲ_Catch_ਉੱਪਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_Task_Catch_ਉੱਪਰ_Ticks
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_Task_Catch_ਉੱਪਰ_Ticks( xYieldOccurred )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_xTaskAbortDelay
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_xTaskAbortDelay( xTask )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_xTaskAbortDelay
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_xTaskAbortDelay( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_xTaskIncrementTick
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_xTaskIncrementTick()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_xTaskIncrementTick
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_xTaskIncrementTick( xSwitchRequired )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_v_Task_ਸੈੱਟ_Application_Task_ਟੈਗ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_v_Task_ਸੈੱਟ_Application_Task_ਟੈਗ( xTask, pxHookFunction )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_v_Task_ਸੈੱਟ_Application_Task_ਟੈਗ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_v_Task_ਸੈੱਟ_Application_Task_ਟੈਗ()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_Task_Get_Application_Task_ਟੈਗ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_Task_Get_Application_Task_ਟੈਗ( xTask )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_Task_Get_Application_Task_ਟੈਗ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_Task_Get_Application_Task_ਟੈਗ( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_Task_Get_Application_Task_ਟੈਗ_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_Task_Get_Application_Task_ਟੈਗ_From_ISR( xTask )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_Task_Get_Application_Task_ਟੈਗ_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_Task_Get_Application_Task_ਟੈਗ_From_ISR( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_xTaskCallApplicationTaskHook
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_xTaskCallApplicationTaskHook( xTask, pvParameter )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_xTaskCallApplicationTaskHook
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_xTaskCallApplicationTaskHook( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_vTaskSwitchContext
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_vTaskSwitchContext()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_vTaskSwitchContext
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_vTaskSwitchContext()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_v_Task_ਰੱਖੋ_On_Event_List
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_v_Task_ਰੱਖੋ_On_Event_List( pxEventList, x_Ticks_ਮੰਜ਼ਿਲ_Wait )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_v_Task_ਰੱਖੋ_On_Event_List
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_v_Task_ਰੱਖੋ_On_Event_List()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_v_Task_ਰੱਖੋ_On_Unordered_Event_List
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_v_Task_ਰੱਖੋ_On_Unordered_Event_List( pxEventList, x_ਵਸਤੂ_ਮੁੱਲ, x_Ticks_ਮੰਜ਼ਿਲ_Wait )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_v_Task_ਰੱਖੋ_On_Unordered_Event_List
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_v_Task_ਰੱਖੋ_On_Unordered_Event_List()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_v_Task_ਰੱਖੋ_On_Event_List_Restricted
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_v_Task_ਰੱਖੋ_On_Event_List_Restricted( pxEventList, x_Ticks_ਮੰਜ਼ਿਲ_Wait, xWaitIndefinitely )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_v_Task_ਰੱਖੋ_On_Event_List_Restricted
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_v_Task_ਰੱਖੋ_On_Event_List_Restricted()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_xTaskRemoveFromEventList
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_xTaskRemoveFromEventList( pxEventList )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_xTaskRemoveFromEventList
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_xTaskRemoveFromEventList( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_vTaskRemoveFromUnorderedEventList
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_vTaskRemoveFromUnorderedEventList( px_Event_List_ਵਸਤੂ, x_ਵਸਤੂ_ਮੁੱਲ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_vTaskRemoveFromUnorderedEventList
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_vTaskRemoveFromUnorderedEventList()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_v_Task_ਸੈੱਟ_ਸਮਾਂ_ਬਾਹਰਲਾ_State
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_v_Task_ਸੈੱਟ_ਸਮਾਂ_ਬਾਹਰਲਾ_State( px_ਸਮਾਂ_ਬਾਹਰਲਾ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_v_Task_ਸੈੱਟ_ਸਮਾਂ_ਬਾਹਰਲਾ_State
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_v_Task_ਸੈੱਟ_ਸਮਾਂ_ਬਾਹਰਲਾ_State()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_v_Task_ਅੰਦਰੂਨੀ_ਸੈੱਟ_ਸਮਾਂ_ਬਾਹਰਲਾ_State
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_v_Task_ਅੰਦਰੂਨੀ_ਸੈੱਟ_ਸਮਾਂ_ਬਾਹਰਲਾ_State( px_ਸਮਾਂ_ਬਾਹਰਲਾ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_v_Task_ਅੰਦਰੂਨੀ_ਸੈੱਟ_ਸਮਾਂ_ਬਾਹਰਲਾ_State
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_v_Task_ਅੰਦਰੂਨੀ_ਸੈੱਟ_ਸਮਾਂ_ਬਾਹਰਲਾ_State()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_Task_Check_For_ਸਮਾਂ_ਬਾਹਰਲਾ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_Task_Check_For_ਸਮਾਂ_ਬਾਹਰਲਾ( px_ਸਮਾਂ_ਬਾਹਰਲਾ, px_Ticks_ਮੰਜ਼ਿਲ_Wait )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_Task_Check_For_ਸਮਾਂ_ਬਾਹਰਲਾ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_Task_Check_For_ਸਮਾਂ_ਬਾਹਰਲਾ( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_vTaskMissedYield
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_vTaskMissedYield()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_vTaskMissedYield
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_vTaskMissedYield()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_ux_Task_Get_Task_ਸੰਖਿਆ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_ux_Task_Get_Task_ਸੰਖਿਆ( xTask )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_ux_Task_Get_Task_ਸੰਖਿਆ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_ux_Task_Get_Task_ਸੰਖਿਆ( uxReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_v_Task_ਸੈੱਟ_Task_ਸੰਖਿਆ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_v_Task_ਸੈੱਟ_Task_ਸੰਖਿਆ( xTask, uxHandle )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_v_Task_ਸੈੱਟ_Task_ਸੰਖਿਆ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_v_Task_ਸੈੱਟ_Task_ਸੰਖਿਆ()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_e_Task_Confirm_Sleep_ਬਹੁਲਕ_ਸਥਿਤੀ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_e_Task_Confirm_Sleep_ਬਹੁਲਕ_ਸਥਿਤੀ()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_e_Task_Confirm_Sleep_ਬਹੁਲਕ_ਸਥਿਤੀ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_e_Task_Confirm_Sleep_ਬਹੁਲਕ_ਸਥਿਤੀ( eReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_v_Task_ਸੈੱਟ_Thread_Local_Storage_ਸੂਚਕ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_v_Task_ਸੈੱਟ_Thread_Local_Storage_ਸੂਚਕ( x_Task_ਮੰਜ਼ਿਲ_ਸੈੱਟ, x_ਸੂਚਕਾਂਕ, pv_ਮੁੱਲ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_v_Task_ਸੈੱਟ_Thread_Local_Storage_ਸੂਚਕ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_v_Task_ਸੈੱਟ_Thread_Local_Storage_ਸੂਚਕ()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_pv_Task_Get_Thread_Local_Storage_ਸੂਚਕ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_pv_Task_Get_Thread_Local_Storage_ਸੂਚਕ( x_Task_ਮੰਜ਼ਿਲ_Query, x_ਸੂਚਕਾਂਕ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_pv_Task_Get_Thread_Local_Storage_ਸੂਚਕ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_pv_Task_Get_Thread_Local_Storage_ਸੂਚਕ( pvReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_vTaskAllocateMPURegions
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_vTaskAllocateMPURegions( x_Task_ਮੰਜ਼ਿਲ_Modify, pxRegions )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_vTaskAllocateMPURegions
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_vTaskAllocateMPURegions()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_vTaskGetInfo
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_vTaskGetInfo( xTask, px_Task_ਸਥਿਤੀ, x_Get_Free_ਢੇਰ_Space, eState )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_vTaskGetInfo
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_vTaskGetInfo()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_ux_Task_Get_ਢੇਰ_ਉੱਚਾ_Water_Mark2
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_ux_Task_Get_ਢੇਰ_ਉੱਚਾ_Water_Mark2( xTask )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_ux_Task_Get_ਢੇਰ_ਉੱਚਾ_Water_Mark2
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_ux_Task_Get_ਢੇਰ_ਉੱਚਾ_Water_Mark2( uxReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_ux_Task_Get_ਢੇਰ_ਉੱਚਾ_Water_Mark
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_ux_Task_Get_ਢੇਰ_ਉੱਚਾ_Water_Mark( xTask )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_ux_Task_Get_ਢੇਰ_ਉੱਚਾ_Water_Mark
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_ux_Task_Get_ਢੇਰ_ਉੱਚਾ_Water_Mark( uxReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_xTaskGetCurrentTaskHandle
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_xTaskGetCurrentTaskHandle()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_xTaskGetCurrentTaskHandle
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_xTaskGetCurrentTaskHandle( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_xTaskGetCurrentTaskHandleForCore
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_xTaskGetCurrentTaskHandleForCore( xCoreID )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_xTaskGetCurrentTaskHandleForCore
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_xTaskGetCurrentTaskHandleForCore( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_xTaskGetSchedulerState
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_xTaskGetSchedulerState()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_xTaskGetSchedulerState
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_xTaskGetSchedulerState( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_Task_ਤਰਜੀਹ_Inherit
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_Task_ਤਰਜੀਹ_Inherit( pxMutexHolder )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_Task_ਤਰਜੀਹ_Inherit
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_Task_ਤਰਜੀਹ_Inherit( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_Task_ਤਰਜੀਹ_Disinherit
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_Task_ਤਰਜੀਹ_Disinherit( pxMutexHolder )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_Task_ਤਰਜੀਹ_Disinherit
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_Task_ਤਰਜੀਹ_Disinherit( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_v_Task_ਤਰਜੀਹ_Disinherit_After_Timeout
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_v_Task_ਤਰਜੀਹ_Disinherit_After_Timeout( pxMutexHolder, ux_Highest_ਤਰਜੀਹ_ਉਡੀਕ_Task )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_v_Task_ਤਰਜੀਹ_Disinherit_After_Timeout
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_v_Task_ਤਰਜੀਹ_Disinherit_After_Timeout()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_vTaskYieldWithinAPI
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_vTaskYieldWithinAPI()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_vTaskYieldWithinAPI
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_vTaskYieldWithinAPI()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_vTaskEnterCritical
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_vTaskEnterCritical()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_vTaskEnterCritical
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_vTaskEnterCritical()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_vTaskEnterCriticalFromISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_vTaskEnterCriticalFromISR()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_vTaskEnterCriticalFromISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_vTaskEnterCriticalFromISR( ux_Saved_ਰੁਕਾਵਟ_ਸਥਿਤੀ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_vTaskExitCritical
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_vTaskExitCritical()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_vTaskExitCritical
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_vTaskExitCritical()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_vTaskExitCriticalFromISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_vTaskExitCriticalFromISR( ux_Saved_ਰੁਕਾਵਟ_ਸਥਿਤੀ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_vTaskExitCriticalFromISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_vTaskExitCriticalFromISR()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_vTaskListTasks
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_vTaskListTasks( pc_Write_ਬਫਰ, ux_ਬਫਰ_ਲੰਮਾਈ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_vTaskListTasks
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_vTaskListTasks()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_v_Task_Get_ਲੜੀਵਾਰ_ਸਮਾਂ_Statistics
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_v_Task_Get_ਲੜੀਵਾਰ_ਸਮਾਂ_Statistics( pc_Write_ਬਫਰ, ux_ਬਫਰ_ਲੰਮਾਈ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_v_Task_Get_ਲੜੀਵਾਰ_ਸਮਾਂ_Statistics
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_v_Task_Get_ਲੜੀਵਾਰ_ਸਮਾਂ_Statistics()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_ux_Task_Reset_Event_ਵਸਤੂ_ਮੁੱਲ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_ux_Task_Reset_Event_ਵਸਤੂ_ਮੁੱਲ()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_ux_Task_Reset_Event_ਵਸਤੂ_ਮੁੱਲ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_ux_Task_Reset_Event_ਵਸਤੂ_ਮੁੱਲ( uxReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_pv_Task_Increment_Mutex_Held_ਗਿਣਤੀ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_pv_Task_Increment_Mutex_Held_ਗਿਣਤੀ()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_pv_Task_Increment_Mutex_Held_ਗਿਣਤੀ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_pv_Task_Increment_Mutex_Held_ਗਿਣਤੀ( pxTCB )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_ulTaskGenericNotifyTake
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_ulTaskGenericNotifyTake( ux_ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_Wait_On, x_Clear_ਗਿਣਤੀ_On_Exit, x_Ticks_ਮੰਜ਼ਿਲ_Wait )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_ulTaskGenericNotifyTake
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_ulTaskGenericNotifyTake( ulReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_xTaskGenericNotifyWait
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_xTaskGenericNotifyWait( ux_ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_Wait_On, ul_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear_On_ਇੰਦਰਾਜ, ul_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear_On_Exit, pul_Notification_ਮੁੱਲ, x_Ticks_ਮੰਜ਼ਿਲ_Wait )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_xTaskGenericNotifyWait
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_xTaskGenericNotifyWait( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_xTaskGenericNotify
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_xTaskGenericNotify( x_Task_ਮੰਜ਼ਿਲ_Notify, ux_ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_Notify, ul_ਮੁੱਲ, eAction, pul_ਪਿਛਲਾ_Notification_ਮੁੱਲ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_xTaskGenericNotify
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_xTaskGenericNotify( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_xTaskGenericNotifyFromISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_xTaskGenericNotifyFromISR( x_Task_ਮੰਜ਼ਿਲ_Notify, ux_ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_Notify, ul_ਮੁੱਲ, eAction, pul_ਪਿਛਲਾ_Notification_ਮੁੱਲ, px_Higher_ਤਰਜੀਹ_Task_Woken )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_xTaskGenericNotifyFromISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_xTaskGenericNotifyFromISR( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_vTaskGenericNotifyGiveFromISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_vTaskGenericNotifyGiveFromISR( x_Task_ਮੰਜ਼ਿਲ_Notify, ux_ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_Notify, px_Higher_ਤਰਜੀਹ_Task_Woken )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_vTaskGenericNotifyGiveFromISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_vTaskGenericNotifyGiveFromISR()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_xTaskGenericNotifyStateClear
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_xTaskGenericNotifyStateClear( xTask, ux_ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_Clear )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_xTaskGenericNotifyStateClear
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_xTaskGenericNotifyStateClear( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_ul_Task_Generic_Notify_ਮੁੱਲ_Clear
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_ul_Task_Generic_Notify_ਮੁੱਲ_Clear( xTask, ux_ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_Clear, ul_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_ul_Task_Generic_Notify_ਮੁੱਲ_Clear
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_ul_Task_Generic_Notify_ਮੁੱਲ_Clear( ulReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_ul_Task_Get_ਲੜੀਵਾਰ_ਸਮਾਂ_Counter
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_ul_Task_Get_ਲੜੀਵਾਰ_ਸਮਾਂ_Counter( xTask )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_ul_Task_Get_ਲੜੀਵਾਰ_ਸਮਾਂ_Counter
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_ul_Task_Get_ਲੜੀਵਾਰ_ਸਮਾਂ_Counter( ul_ਲੜੀਵਾਰ_ਸਮਾਂ_Counter )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_ul_Task_Get_ਲੜੀਵਾਰ_ਸਮਾਂ_Percent
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_ul_Task_Get_ਲੜੀਵਾਰ_ਸਮਾਂ_Percent( xTask )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_ul_Task_Get_ਲੜੀਵਾਰ_ਸਮਾਂ_Percent
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_ul_Task_Get_ਲੜੀਵਾਰ_ਸਮਾਂ_Percent( ulReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_ul_Task_Get_Idle_ਲੜੀਵਾਰ_ਸਮਾਂ_Counter
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_ul_Task_Get_Idle_ਲੜੀਵਾਰ_ਸਮਾਂ_Counter()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_ul_Task_Get_Idle_ਲੜੀਵਾਰ_ਸਮਾਂ_Counter
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_ul_Task_Get_Idle_ਲੜੀਵਾਰ_ਸਮਾਂ_Counter( ulReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_ul_Task_Get_Idle_ਲੜੀਵਾਰ_ਸਮਾਂ_Percent
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_ul_Task_Get_Idle_ਲੜੀਵਾਰ_ਸਮਾਂ_Percent()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_ul_Task_Get_Idle_ਲੜੀਵਾਰ_ਸਮਾਂ_Percent
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_ul_Task_Get_Idle_ਲੜੀਵਾਰ_ਸਮਾਂ_Percent( ulReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_xTaskGetMPUSettings
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_xTaskGetMPUSettings( xTask )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_xTaskGetMPUSettings
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_xTaskGetMPUSettings( xMPUSettings )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਧਾਰਾ_ਬਫਰ_Generic_Create
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਧਾਰਾ_ਬਫਰ_Generic_Create( x_ਬਫਰ_ਮਾਪ_ਬਾਈਟਾਂ, x_Trigger_Level_ਬਾਈਟਾਂ, x_ਧਾਰਾ_ਬਫਰ_Type, pxSendCompletedCallback, pxReceiveCompletedCallback )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਧਾਰਾ_ਬਫਰ_Generic_Create
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਧਾਰਾ_ਬਫਰ_Generic_Create( pv_Allocated_ਸਮ੍ਰਿਤੀ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਧਾਰਾ_ਬਫਰ_Generic_Create_Static
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਧਾਰਾ_ਬਫਰ_Generic_Create_Static( x_ਬਫਰ_ਮਾਪ_ਬਾਈਟਾਂ, x_Trigger_Level_ਬਾਈਟਾਂ, x_ਧਾਰਾ_ਬਫਰ_Type, puc_ਧਾਰਾ_ਬਫਰ_Storage_Area, px_Static_ਧਾਰਾ_ਬਫਰ, pxSendCompletedCallback, pxReceiveCompletedCallback )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਧਾਰਾ_ਬਫਰ_Generic_Create_Static
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਧਾਰਾ_ਬਫਰ_Generic_Create_Static( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਧਾਰਾ_ਬਫਰ_Get_Static_Buffers
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਧਾਰਾ_ਬਫਰ_Get_Static_Buffers( x_ਧਾਰਾ_ਬਫਰ, ppuc_ਧਾਰਾ_ਬਫਰ_Storage_Area, ppx_Static_ਧਾਰਾ_ਬਫਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਧਾਰਾ_ਬਫਰ_Get_Static_Buffers
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਧਾਰਾ_ਬਫਰ_Get_Static_Buffers( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_v_ਧਾਰਾ_ਬਫਰ_Delete
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_v_ਧਾਰਾ_ਬਫਰ_Delete( x_ਧਾਰਾ_ਬਫਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_v_ਧਾਰਾ_ਬਫਰ_Delete
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_v_ਧਾਰਾ_ਬਫਰ_Delete()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਧਾਰਾ_ਬਫਰ_Reset
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਧਾਰਾ_ਬਫਰ_Reset( x_ਧਾਰਾ_ਬਫਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਧਾਰਾ_ਬਫਰ_Reset
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਧਾਰਾ_ਬਫਰ_Reset( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਧਾਰਾ_ਬਫਰ_Reset_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਧਾਰਾ_ਬਫਰ_Reset_From_ISR( x_ਧਾਰਾ_ਬਫਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਧਾਰਾ_ਬਫਰ_Reset_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਧਾਰਾ_ਬਫਰ_Reset_From_ISR( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਧਾਰਾ_ਬਫਰ_ਸੈੱਟ_Trigger_Level
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਧਾਰਾ_ਬਫਰ_ਸੈੱਟ_Trigger_Level( x_ਧਾਰਾ_ਬਫਰ, xTriggerLevel )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਧਾਰਾ_ਬਫਰ_ਸੈੱਟ_Trigger_Level
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਧਾਰਾ_ਬਫਰ_ਸੈੱਟ_Trigger_Level( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਧਾਰਾ_ਬਫਰ_Spaces_ਉਪਲਬਧ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਧਾਰਾ_ਬਫਰ_Spaces_ਉਪਲਬਧ( x_ਧਾਰਾ_ਬਫਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਧਾਰਾ_ਬਫਰ_Spaces_ਉਪਲਬਧ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਧਾਰਾ_ਬਫਰ_Spaces_ਉਪਲਬਧ( xSpace )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਧਾਰਾ_ਬਫਰ_ਬਾਈਟਾਂ_ਉਪਲਬਧ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਧਾਰਾ_ਬਫਰ_ਬਾਈਟਾਂ_ਉਪਲਬਧ( x_ਧਾਰਾ_ਬਫਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਧਾਰਾ_ਬਫਰ_ਬਾਈਟਾਂ_ਉਪਲਬਧ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਧਾਰਾ_ਬਫਰ_ਬਾਈਟਾਂ_ਉਪਲਬਧ( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਧਾਰਾ_ਬਫਰ_Send
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਧਾਰਾ_ਬਫਰ_Send( x_ਧਾਰਾ_ਬਫਰ, pv_Tx_ਅੰਕੜੇ, x_ਅੰਕੜੇ_ਲੰਮਾਈ_ਬਾਈਟਾਂ, x_Ticks_ਮੰਜ਼ਿਲ_Wait )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਧਾਰਾ_ਬਫਰ_Send
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਧਾਰਾ_ਬਫਰ_Send( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਧਾਰਾ_ਬਫਰ_Send_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਧਾਰਾ_ਬਫਰ_Send_From_ISR( x_ਧਾਰਾ_ਬਫਰ, pv_Tx_ਅੰਕੜੇ, x_ਅੰਕੜੇ_ਲੰਮਾਈ_ਬਾਈਟਾਂ, px_Higher_ਤਰਜੀਹ_Task_Woken )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਧਾਰਾ_ਬਫਰ_Send_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਧਾਰਾ_ਬਫਰ_Send_From_ISR( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਧਾਰਾ_ਬਫਰ_Receive
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਧਾਰਾ_ਬਫਰ_Receive( x_ਧਾਰਾ_ਬਫਰ, pv_Rx_ਅੰਕੜੇ, x_ਬਫਰ_ਲੰਮਾਈ_ਬਾਈਟਾਂ, x_Ticks_ਮੰਜ਼ਿਲ_Wait )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਧਾਰਾ_ਬਫਰ_Receive
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਧਾਰਾ_ਬਫਰ_Receive( x_Received_ਲੰਮਾਈ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਧਾਰਾ_ਬਫਰ_ਅਗਲਾ_ਸੁਨੇਹਾ_ਲੰਮਾਈ_ਬਾਈਟਾਂ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਧਾਰਾ_ਬਫਰ_ਅਗਲਾ_ਸੁਨੇਹਾ_ਲੰਮਾਈ_ਬਾਈਟਾਂ( x_ਧਾਰਾ_ਬਫਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਧਾਰਾ_ਬਫਰ_ਅਗਲਾ_ਸੁਨੇਹਾ_ਲੰਮਾਈ_ਬਾਈਟਾਂ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਧਾਰਾ_ਬਫਰ_ਅਗਲਾ_ਸੁਨੇਹਾ_ਲੰਮਾਈ_ਬਾਈਟਾਂ( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਧਾਰਾ_ਬਫਰ_Receive_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਧਾਰਾ_ਬਫਰ_Receive_From_ISR( x_ਧਾਰਾ_ਬਫਰ, pv_Rx_ਅੰਕੜੇ, x_ਬਫਰ_ਲੰਮਾਈ_ਬਾਈਟਾਂ, px_Higher_ਤਰਜੀਹ_Task_Woken )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਧਾਰਾ_ਬਫਰ_Receive_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਧਾਰਾ_ਬਫਰ_Receive_From_ISR( x_Received_ਲੰਮਾਈ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਧਾਰਾ_ਬਫਰ_Is_ਖਾਲੀ_ਥਾਵਾਂ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਧਾਰਾ_ਬਫਰ_Is_ਖਾਲੀ_ਥਾਵਾਂ( x_ਧਾਰਾ_ਬਫਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਧਾਰਾ_ਬਫਰ_Is_ਖਾਲੀ_ਥਾਵਾਂ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਧਾਰਾ_ਬਫਰ_Is_ਖਾਲੀ_ਥਾਵਾਂ( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਧਾਰਾ_ਬਫਰ_Is_ਭਰਿਆ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਧਾਰਾ_ਬਫਰ_Is_ਭਰਿਆ( x_ਧਾਰਾ_ਬਫਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਧਾਰਾ_ਬਫਰ_Is_ਭਰਿਆ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਧਾਰਾ_ਬਫਰ_Is_ਭਰਿਆ( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਧਾਰਾ_ਬਫਰ_Send_Completed_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਧਾਰਾ_ਬਫਰ_Send_Completed_From_ISR( x_ਧਾਰਾ_ਬਫਰ, px_Higher_ਤਰਜੀਹ_Task_Woken )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਧਾਰਾ_ਬਫਰ_Send_Completed_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਧਾਰਾ_ਬਫਰ_Send_Completed_From_ISR( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_x_ਧਾਰਾ_ਬਫਰ_Receive_Completed_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_x_ਧਾਰਾ_ਬਫਰ_Receive_Completed_From_ISR( x_ਧਾਰਾ_ਬਫਰ, px_Higher_ਤਰਜੀਹ_Task_Woken )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_x_ਧਾਰਾ_ਬਫਰ_Receive_Completed_From_ISR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_x_ਧਾਰਾ_ਬਫਰ_Receive_Completed_From_ISR( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_ux_ਧਾਰਾ_ਬਫਰ_Get_ਧਾਰਾ_ਬਫਰ_Notification_ਸੂਚਕਾਂਕ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_ux_ਧਾਰਾ_ਬਫਰ_Get_ਧਾਰਾ_ਬਫਰ_Notification_ਸੂਚਕਾਂਕ( x_ਧਾਰਾ_ਬਫਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_ux_ਧਾਰਾ_ਬਫਰ_Get_ਧਾਰਾ_ਬਫਰ_Notification_ਸੂਚਕਾਂਕ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_ux_ਧਾਰਾ_ਬਫਰ_Get_ਧਾਰਾ_ਬਫਰ_Notification_ਸੂਚਕਾਂਕ( ux_Notification_ਸੂਚਕਾਂਕ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_v_ਧਾਰਾ_ਬਫਰ_ਸੈੱਟ_ਧਾਰਾ_ਬਫਰ_Notification_ਸੂਚਕਾਂਕ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_v_ਧਾਰਾ_ਬਫਰ_ਸੈੱਟ_ਧਾਰਾ_ਬਫਰ_Notification_ਸੂਚਕਾਂਕ( x_ਧਾਰਾ_ਬਫਰ, ux_Notification_ਸੂਚਕਾਂਕ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_v_ਧਾਰਾ_ਬਫਰ_ਸੈੱਟ_ਧਾਰਾ_ਬਫਰ_Notification_ਸੂਚਕਾਂਕ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_v_ਧਾਰਾ_ਬਫਰ_ਸੈੱਟ_ਧਾਰਾ_ਬਫਰ_Notification_ਸੂਚਕਾਂਕ()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_ux_ਧਾਰਾ_ਬਫਰ_Get_ਧਾਰਾ_ਬਫਰ_ਸੰਖਿਆ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_ux_ਧਾਰਾ_ਬਫਰ_Get_ਧਾਰਾ_ਬਫਰ_ਸੰਖਿਆ( x_ਧਾਰਾ_ਬਫਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_ux_ਧਾਰਾ_ਬਫਰ_Get_ਧਾਰਾ_ਬਫਰ_ਸੰਖਿਆ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_ux_ਧਾਰਾ_ਬਫਰ_Get_ਧਾਰਾ_ਬਫਰ_ਸੰਖਿਆ( ux_ਧਾਰਾ_ਬਫਰ_ਸੰਖਿਆ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_v_ਧਾਰਾ_ਬਫਰ_ਸੈੱਟ_ਧਾਰਾ_ਬਫਰ_ਸੰਖਿਆ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_v_ਧਾਰਾ_ਬਫਰ_ਸੈੱਟ_ਧਾਰਾ_ਬਫਰ_ਸੰਖਿਆ( x_ਧਾਰਾ_ਬਫਰ, ux_ਧਾਰਾ_ਬਫਰ_ਸੰਖਿਆ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_v_ਧਾਰਾ_ਬਫਰ_ਸੈੱਟ_ਧਾਰਾ_ਬਫਰ_ਸੰਖਿਆ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_v_ਧਾਰਾ_ਬਫਰ_ਸੈੱਟ_ਧਾਰਾ_ਬਫਰ_ਸੰਖਿਆ()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_uc_ਧਾਰਾ_ਬਫਰ_Get_ਧਾਰਾ_ਬਫਰ_Type
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_uc_ਧਾਰਾ_ਬਫਰ_Get_ਧਾਰਾ_ਬਫਰ_Type( x_ਧਾਰਾ_ਬਫਰ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_uc_ਧਾਰਾ_ਬਫਰ_Get_ਧਾਰਾ_ਬਫਰ_Type
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_uc_ਧਾਰਾ_ਬਫਰ_Get_ਧਾਰਾ_ਬਫਰ_Type( uc_ਧਾਰਾ_ਬਫਰ_Type )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_vListInitialise
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_vListInitialise( pxList )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_vListInitialise
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_vListInitialise()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_v_List_Initialise_ਵਸਤੂ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_v_List_Initialise_ਵਸਤੂ( px_ਵਸਤੂ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_v_List_Initialise_ਵਸਤੂ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_v_List_Initialise_ਵਸਤੂ()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_v_List_ਪਾਓ_ਸਿਰਾ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_v_List_ਪਾਓ_ਸਿਰਾ( pxList, px_New_List_ਵਸਤੂ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_v_List_ਪਾਓ_ਸਿਰਾ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_v_List_ਪਾਓ_ਸਿਰਾ()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_v_List_ਪਾਓ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_v_List_ਪਾਓ( pxList, px_New_List_ਵਸਤੂ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_v_List_ਪਾਓ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_v_List_ਪਾਓ()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_uxListRemove
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_uxListRemove( px_ਵਸਤੂ_ਮੰਜ਼ਿਲ_Remove )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_uxListRemove
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_uxListRemove( ux_ਸੰਖਿਆ_ਦਾ_ਚੀਜ਼ਾਂ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_xCoRoutineCreate
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_xCoRoutineCreate( px_Co_Routine_ਕੋਡ, ux_ਤਰਜੀਹ, ux_ਸੂਚਕਾਂਕ )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_xCoRoutineCreate
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_xCoRoutineCreate( xReturn )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_v_Co_Routine_ਜੋੜੋ_ਮੰਜ਼ਿਲ_Delayed_List
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_v_Co_Routine_ਜੋੜੋ_ਮੰਜ਼ਿਲ_Delayed_List( x_Ticks_ਮੰਜ਼ਿਲ_Delay, pxEventList )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_v_Co_Routine_ਜੋੜੋ_ਮੰਜ਼ਿਲ_Delayed_List
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_v_Co_Routine_ਜੋੜੋ_ਮੰਜ਼ਿਲ_Delayed_List()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_ENTER_v_Co_Routine_ਸਮਾਂ_ਸਾਰਣੀ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_ENTER_v_Co_Routine_ਸਮਾਂ_ਸਾਰਣੀ()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ trace_RETURN_v_Co_Routine_ਸਮਾਂ_ਸਾਰਣੀ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ trace_RETURN_v_Co_Routine_ਸਮਾਂ_ਸਾਰਣੀ()
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceENTER_xCoRoutineRemoveFromEventList
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceENTER_xCoRoutineRemoveFromEventList( pxEventList )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ traceRETURN_xCoRoutineRemoveFromEventList
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ traceRETURN_xCoRoutineRemoveFromEventList( xReturn )
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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_ENABLE_ਢੇਰ_PROTECTOR
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_ENABLE_ਢੇਰ_PROTECTOR    0
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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_KERNEL_PROVIDED_STATIC_ਸਮ੍ਰਿਤੀ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_KERNEL_PROVIDED_STATIC_ਸਮ੍ਰਿਤੀ    0
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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_STATS_ਬਫਰ_MAX_ਲੰਮਾਈ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_STATS_ਬਫਰ_MAX_ਲੰਮਾਈ    0xFFFF
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_ਢੇਰ_ਡੂੰਘਾਈ_TYPE

/* Defaults to StackType_t for backward compatibility, but can be overridden
 * in FreeRTOSConfig.h if StackType_t is too restrictive. */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_ਢੇਰ_ਡੂੰਘਾਈ_TYPE    ਢੇਰ_Type_t
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

#ਜੇ ( ( config_ਲੜੀਵਾਰ_ਗੁਣਜ_PRIORITIES == 0 ) && ( configUSE_TASK_PREEMPTION_DISABLE != 0 ) )
    #ਗਲਤੀ config_ਲੜੀਵਾਰ_ਗੁਣਜ_PRIORITIES must be ਸੈੱਟ ਮੰਜ਼ਿਲ 1 ਮੰਜ਼ਿਲ use task preemption disable
#ਜੇ_ਅੰਤ

#ਜੇ ( ( configUSE_PREEMPTION == 0 ) && ( configUSE_TASK_PREEMPTION_DISABLE != 0 ) )
    #ਗਲਤੀ configUSE_PREEMPTION must be ਸੈੱਟ ਮੰਜ਼ਿਲ 1 ਮੰਜ਼ਿਲ use task preemption disable
#ਜੇ_ਅੰਤ

#ਜੇ ( ( config_ਸੰਖਿਆ_ਦਾ_CORES == 1 ) && ( configUSE_TASK_PREEMPTION_DISABLE != 0 ) )
    #ਗਲਤੀ configUSE_TASK_PREEMPTION_DISABLE is not supported ਵਿੱਚ single core FreeRTOS
#ਜੇ_ਅੰਤ

#ਜੇ ( ( config_ਸੰਖਿਆ_ਦਾ_CORES == 1 ) && ( configUSE_CORE_AFFINITY != 0 ) )
    #ਗਲਤੀ configUSE_CORE_AFFINITY is not supported ਵਿੱਚ single core FreeRTOS
#ਜੇ_ਅੰਤ

#ਜੇ ( ( config_ਸੰਖਿਆ_ਦਾ_CORES > 1 ) && ( config_USE_ਪੋਰਟ_OPTIMISED_TASK_SELECTION != 0 ) )
    #ਗਲਤੀ config_USE_ਪੋਰਟ_OPTIMISED_TASK_SELECTION is not supported ਵਿੱਚ SMP FreeRTOS
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

/* The following config allows infinite loop control. For example, control the
 * infinite loop in idle task function when performing unit tests. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configCONTROL_INFINITE_LOOP
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configCONTROL_INFINITE_LOOP()
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
    #ਜੇ ( configUSE_CORE_AFFINITY == 1 ) && ( config_ਸੰਖਿਆ_ਦਾ_CORES > 1 )
        UBaseType_t uxDummy26;
    #ਜੇ_ਅੰਤ
    Static_List_ਵਸਤੂ_t xDummy3[ 2 ];
    UBaseType_t uxDummy5;
    ਖਾਲੀ * pxDummy6;
    #ਜੇ ( config_ਸੰਖਿਆ_ਦਾ_CORES > 1 )
        ਅਧਾਰ_Type_t xDummy23;
        UBaseType_t uxDummy24;
    #ਜੇ_ਅੰਤ
    uint8_t ucDummy7[ configMAX_TASK_NAME_LEN ];
    #ਜੇ ( configUSE_TASK_PREEMPTION_DISABLE == 1 )
        ਅਧਾਰ_Type_t xDummy25;
    #ਜੇ_ਅੰਤ
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
    #ਜੇ ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 )
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
    UBaseType_t uxDummy6;
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
