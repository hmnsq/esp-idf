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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ INC_FREERTOS_H
#ವ್ಯಾಖ್ಯಾನಿಸು INC_FREERTOS_H

/*
 * Include the generic headers required for the FreeRTOS port being used.
 */
#ಅನ್ತರ್ಭಾವ <stddef.h>

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
#ಅನ್ತರ್ಭಾವ <stdint.h> /* READ COMMENT ABOVE. */

/* *INDENT-OFF* */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
    ಬಾಹ್ಯ "C" {
#ಆದರೆ_ಅಂತ್ಯ
/* *INDENT-ON* */

/* Application specific configuration options. */
#ಅನ್ತರ್ಭಾವ "FreeRTOSConfig.h"

/* Basic FreeRTOS definitions. */
#ಅನ್ತರ್ಭಾವ "projdefs.h"

/* Definitions specific to the port being used. */
#ಅನ್ತರ್ಭಾವ "portable.h"

/* Must be defaulted before configUSE_NEWLIB_REENTRANT is used below. */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configUSE_NEWLIB_REENTRANT
    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_NEWLIB_REENTRANT    0
#ಆದರೆ_ಅಂತ್ಯ

/* Required if struct _reent is used. */
#ಆದರೆ ( configUSE_NEWLIB_REENTRANT == 1 )

/* Note Newlib support has been included by popular demand, but is not
 * used by the FreeRTOS maintainers themselves.  FreeRTOS is not
 * responsible for resulting newlib operation.  User must be familiar with
 * newlib and must provide system-wide implementations of the necessary
 * stubs. Be warned that (at the time of writing) the current newlib design
 * implements a system-wide malloc() that must be provided with locks.
 *
 * See the third party link http://www.nadler.com/embedded/newlibAndFreeRTOS.html
 * for additional information. */
    #ಅನ್ತರ್ಭಾವ <reent.h>

    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_C_RUNTIME_TLS_SUPPORT    1

    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_TLS_ಖಂಡ_TYPE
        #ವ್ಯಾಖ್ಯಾನಿಸು config_TLS_ಖಂಡ_TYPE           ರಚನೆ _reent
    #ಆದರೆ_ಅಂತ್ಯ

    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_INIT_TLS_ಖಂಡ
        #ವ್ಯಾಖ್ಯಾನಿಸು config_INIT_TLS_ಖಂಡ( xTLSBlock )    _REENT_INIT_PTR( &( xTLSBlock ) )
    #ಆದರೆ_ಅಂತ್ಯ

    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_ಗಣ_TLS_ಖಂಡ
        #ವ್ಯಾಖ್ಯಾನಿಸು config_ಗಣ_TLS_ಖಂಡ( xTLSBlock )    _impure_ptr = &( xTLSBlock )
    #ಆದರೆ_ಅಂತ್ಯ

    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_DEINIT_TLS_ಖಂಡ
        #ವ್ಯಾಖ್ಯಾನಿಸು config_DEINIT_TLS_ಖಂಡ( xTLSBlock )    _reclaim_reent( &( xTLSBlock ) )
    #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ /* if ( configUSE_NEWLIB_REENTRANT == 1 ) */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configUSE_C_RUNTIME_TLS_SUPPORT
    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_C_RUNTIME_TLS_SUPPORT    0
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ ( ( configUSE_NEWLIB_REENTRANT == 0 ) && ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 ) )

    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_TLS_ಖಂಡ_TYPE
        #ದೋಷ Missing definition:  config_TLS_ಖಂಡ_TYPE must be defined ಒಳ FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is ಗಣ ಗಮ್ಯ 1.
    #ಆದರೆ_ಅಂತ್ಯ

    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_INIT_TLS_ಖಂಡ
        #ದೋಷ Missing definition:  config_INIT_TLS_ಖಂಡ must be defined ಒಳ FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is ಗಣ ಗಮ್ಯ 1.
    #ಆದರೆ_ಅಂತ್ಯ

    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_ಗಣ_TLS_ಖಂಡ
        #ದೋಷ Missing definition:  config_ಗಣ_TLS_ಖಂಡ must be defined ಒಳ FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is ಗಣ ಗಮ್ಯ 1.
    #ಆದರೆ_ಅಂತ್ಯ

    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_DEINIT_TLS_ಖಂಡ
        #ದೋಷ Missing definition:  config_DEINIT_TLS_ಖಂಡ must be defined ಒಳ FreeRTOSConfig.h when configUSE_C_RUNTIME_TLS_SUPPORT is ಗಣ ಗಮ್ಯ 1.
    #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ /* if ( ( configUSE_NEWLIB_REENTRANT == 0 ) && ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 ) ) */

/*
 * Check all the required application specific macros have been defined.
 * These macros are application specific and (as downloaded) are defined
 * within FreeRTOSConfig.h.
 */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_ಸಂಖ್ಯೆ_ದ_CORES
    #ದೋಷ Missing definition:  config_ಸಂಖ್ಯೆ_ದ_CORES must be defined ಒಳ FreeRTOSConfig.h
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ ( config_ಸಂಖ್ಯೆ_ದ_CORES > 1 )
    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_GET_CORE_ID
        #ದೋಷ "Missing definition: portGET_CORE_ID() must be defined if in portmacro.h if configNUMBER_OF_CORES > 1"
    #ಆದರೆ_ಅಂತ್ಯ
    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_YIELD_CORE
        #ದೋಷ "Missing definition: portYIELD_CORE() must be defined if in portmacro.h if configNUMBER_OF_CORES > 1"
    #ಆದರೆ_ಅಂತ್ಯ
#ಅಥವಾಆದರೆ ( config_ಸಂಖ್ಯೆ_ದ_CORES == 1 )
    #ವ್ಯಾಖ್ಯಾನ_ರದ್ದು ಪೋರ್ಟ್_GET_CORE_ID
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_GET_CORE_ID()    0
#ಇಲ್ಲದಿದ್ದರೆ
    #ದೋಷ config_ಸಂಖ್ಯೆ_ದ_CORES must be defined ಗಮ್ಯ either 1 or > 1.
#ಆದರೆ_ಅಂತ್ಯ /* if ( configNUMBER_OF_CORES > 1 ) */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_ಕನಿಷ್ಠತಮ_ರಾಶಿ_ಗಾತ್ರ
    #ದೋಷ Missing definition:  config_ಕನಿಷ್ಠತಮ_ರಾಶಿ_ಗಾತ್ರ must be defined ಒಳ FreeRTOSConfig.h.  config_ಕನಿಷ್ಠತಮ_ರಾಶಿ_ಗಾತ್ರ ನಿರೂಪಿಸುತ್ತದೆ the ಗಾತ್ರ_2 (ಒಳ ಪದಗಳು) ದ the ರಾಶಿ allocated ಗಮ್ಯ the idle task.  Refer ಗಮ್ಯ the demo project provided ಗಾಗಿ your ಪೋರ್ಟ್ ಗಾಗಿ a suitable ಮೌಲ್ಯ.
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configMAX_PRIORITIES
    #ದೋಷ Missing definition:  configMAX_PRIORITIES must be defined ಒಳ FreeRTOSConfig.h.  See the Configuration section ದ the FreeRTOS API documentation ಗಾಗಿ details.
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ configMAX_PRIORITIES < 1
    #ದೋಷ configMAX_PRIORITIES must be defined ಗಮ್ಯ be greater than or equal ಗಮ್ಯ 1.
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configUSE_PREEMPTION
    #ದೋಷ Missing definition:  configUSE_PREEMPTION must be defined ಒಳ FreeRTOSConfig.h as either 1 or 0.  See the Configuration section ದ the FreeRTOS API documentation ಗಾಗಿ details.
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configUSE_IDLE_HOOK
    #ದೋಷ Missing definition:  configUSE_IDLE_HOOK must be defined ಒಳ FreeRTOSConfig.h as either 1 or 0.  See the Configuration section ದ the FreeRTOS API documentation ಗಾಗಿ details.
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configUSE_TICK_HOOK
    #ದೋಷ Missing definition:  configUSE_TICK_HOOK must be defined ಒಳ FreeRTOSConfig.h as either 1 or 0.  See the Configuration section ದ the FreeRTOS API documentation ಗಾಗಿ details.
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_USE_16_ಬಿಟ್_TICKS
    #ದೋಷ Missing definition:  config_USE_16_ಬಿಟ್_TICKS must be defined ಒಳ FreeRTOSConfig.h as either 1 or 0.  See the Configuration section ದ the FreeRTOS API documentation ಗಾಗಿ details.
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configUSE_CO_ROUTINES
    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_CO_ROUTINES    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ INCLUDE_v_Task_ಆದ್ಯತೆ_ಗಣ
    #ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_v_Task_ಆದ್ಯತೆ_ಗಣ    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ INCLUDE_ux_Task_ಆದ್ಯತೆ_Get
    #ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_ux_Task_ಆದ್ಯತೆ_Get    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ INCLUDE_vTaskDelete
    #ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_vTaskDelete    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ INCLUDE_vTaskSuspend
    #ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_vTaskSuspend    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ INCLUDE_xTaskDelayUntil
    #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ INCLUDE_vTaskDelayUntil

/* INCLUDE_vTaskDelayUntil was replaced by INCLUDE_xTaskDelayUntil.  Backward
 * compatibility is maintained if only one or the other is defined, but
 * there is a conflict if both are defined. */
        #ದೋಷ INCLUDE_vTaskDelayUntil and INCLUDE_xTaskDelayUntil are both defined.  INCLUDE_vTaskDelayUntil is no longer required and should be removed
    #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ INCLUDE_xTaskDelayUntil
    #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ INCLUDE_vTaskDelayUntil

/* If INCLUDE_vTaskDelayUntil is set but INCLUDE_xTaskDelayUntil is not then
 * the project's FreeRTOSConfig.h probably pre-dates the introduction of
 * xTaskDelayUntil and setting INCLUDE_xTaskDelayUntil to whatever
 * INCLUDE_vTaskDelayUntil is set to will ensure backward compatibility.
 */
        #ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_xTaskDelayUntil    INCLUDE_vTaskDelayUntil
    #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ INCLUDE_xTaskDelayUntil
    #ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_xTaskDelayUntil    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ INCLUDE_vTaskDelay
    #ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_vTaskDelay    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ INCLUDE_xTaskGetIdleTaskHandle
    #ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_xTaskGetIdleTaskHandle    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ INCLUDE_xTaskAbortDelay
    #ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_xTaskAbortDelay    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ INCLUDE_x_ಸರತಿ_Get_Mutex_Holder
    #ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_x_ಸರತಿ_Get_Mutex_Holder    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ INCLUDE_xSemaphoreGetMutexHolder
    #ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_xSemaphoreGetMutexHolder    INCLUDE_x_ಸರತಿ_Get_Mutex_Holder
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ INCLUDE_xTaskGetHandle
    #ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_xTaskGetHandle    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ INCLUDE_ux_Task_Get_ರಾಶಿ_ಮೇಲಿನ_Water_Mark
    #ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_ux_Task_Get_ರಾಶಿ_ಮೇಲಿನ_Water_Mark    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ INCLUDE_ux_Task_Get_ರಾಶಿ_ಮೇಲಿನ_Water_Mark2
    #ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_ux_Task_Get_ರಾಶಿ_ಮೇಲಿನ_Water_Mark2    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ INCLUDE_eTaskGetState
    #ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_eTaskGetState    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ INCLUDE_xTaskResumeFromISR
    #ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_xTaskResumeFromISR    1
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ INCLUDE_xTimerPendFunctionCall
    #ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_xTimerPendFunctionCall    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ INCLUDE_xTaskGetSchedulerState
    #ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_xTaskGetSchedulerState    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ INCLUDE_xTaskGetCurrentTaskHandle
    #ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_xTaskGetCurrentTaskHandle    1
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ configUSE_CO_ROUTINES != 0
    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configMAX_CO_ROUTINE_PRIORITIES
        #ದೋಷ configMAX_CO_ROUTINE_PRIORITIES must be greater than or equal ಗಮ್ಯ 1.
    #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configUSE_DAEMON_TASK_STARTUP_HOOK
    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_DAEMON_TASK_STARTUP_HOOK    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_USE_APPLICATION_TASK_ಟ್ಯಾಗ್
    #ವ್ಯಾಖ್ಯಾನಿಸು config_USE_APPLICATION_TASK_ಟ್ಯಾಗ್    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_ಅಂಕೆ_THREAD_LOCAL_STORAGE_ಸೂಚಕಗಳು
    #ವ್ಯಾಖ್ಯಾನಿಸು config_ಅಂಕೆ_THREAD_LOCAL_STORAGE_ಸೂಚಕಗಳು    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configUSE_RECURSIVE_MUTEXES
    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_RECURSIVE_MUTEXES    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configUSE_MUTEXES
    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_MUTEXES    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configUSE_TIMERS
    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_TIMERS    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configUSE_COUNTING_SEMAPHORES
    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_COUNTING_SEMAPHORES    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configUSE_ALTERNATIVE_API
    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_ALTERNATIVE_API    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_CRITICAL_NESTING_ಒಳ_TCB
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_CRITICAL_NESTING_ಒಳ_TCB    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configMAX_TASK_NAME_LEN
    #ವ್ಯಾಖ್ಯಾನಿಸು configMAX_TASK_NAME_LEN    16
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configIDLE_SHOULD_YIELD
    #ವ್ಯಾಖ್ಯಾನಿಸು configIDLE_SHOULD_YIELD    1
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ configMAX_TASK_NAME_LEN < 1
    #ದೋಷ configMAX_TASK_NAME_LEN must be ಗಣ ಗಮ್ಯ a minimum ದ 1 ಒಳ FreeRTOSConfig.h
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configASSERT
    #ವ್ಯಾಖ್ಯಾನಿಸು configASSERT( x )
    #ವ್ಯಾಖ್ಯಾನಿಸು configASSERT_DEFINED    0
#ಇಲ್ಲದಿದ್ದರೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು configASSERT_DEFINED    1
#ಆದರೆ_ಅಂತ್ಯ

/* configPRECONDITION should be defined as configASSERT.
 * The CBMC proofs need a way to track assumptions and assertions.
 * A configPRECONDITION statement should express an implicit invariant or
 * assumption made.  A configASSERT statement should express an invariant that must
 * hold explicit before calling the code. */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configPRECONDITION
    #ವ್ಯಾಖ್ಯಾನಿಸು configPRECONDITION( X )    configASSERT( X )
    #ವ್ಯಾಖ್ಯಾನಿಸು configPRECONDITION_DEFINED    0
#ಇಲ್ಲದಿದ್ದರೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು configPRECONDITION_DEFINED    1
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_ಸ್ಮರಣೆ_BARRIER
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ಸ್ಮರಣೆ_BARRIER()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_SOFTWARE_BARRIER
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_SOFTWARE_BARRIER()
#ಆದರೆ_ಅಂತ್ಯ

/* The timers module relies on xTaskGetSchedulerState(). */
#ಆದರೆ configUSE_TIMERS == 1

    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_TIMER_TASK_ಆದ್ಯತೆ
        #ದೋಷ If configUSE_TIMERS is ಗಣ ಗಮ್ಯ 1 then config_TIMER_TASK_ಆದ್ಯತೆ must also be defined.
    #ಆದರೆ_ಅಂತ್ಯ /* configTIMER_TASK_PRIORITY */

    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_TIMER_ಸರತಿ_ಉದ್ದಳತೆ
        #ದೋಷ If configUSE_TIMERS is ಗಣ ಗಮ್ಯ 1 then config_TIMER_ಸರತಿ_ಉದ್ದಳತೆ must also be defined.
    #ಆದರೆ_ಅಂತ್ಯ /* configTIMER_QUEUE_LENGTH */

    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_TIMER_TASK_ರಾಶಿ_ಆಳ
        #ದೋಷ If configUSE_TIMERS is ಗಣ ಗಮ್ಯ 1 then config_TIMER_TASK_ರಾಶಿ_ಆಳ must also be defined.
    #ಆದರೆ_ಅಂತ್ಯ /* configTIMER_TASK_STACK_DEPTH */

#ಆದರೆ_ಅಂತ್ಯ /* configUSE_TIMERS */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR()    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_CLEAR_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_CLEAR_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR( ux_Saved_ಸ್ಥಿತಿ_ಮೌಲ್ಯ )    ( ಶೂನ್ಯ ) ( ux_Saved_ಸ್ಥಿತಿ_ಮೌಲ್ಯ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_CLEAN_ಮೇಲೆ_TCB
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_CLEAN_ಮೇಲೆ_TCB( pxTCB )    ( ಶೂನ್ಯ ) ( pxTCB )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_PRE_TASK_DELETE_HOOK
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_PRE_TASK_DELETE_HOOK( pv_Task_ಗಮ್ಯ_Delete, pxYieldPending )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_ಸಿದ್ಧತೆ_TCB
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ಸಿದ್ಧತೆ_TCB( pxTCB )    ( ಶೂನ್ಯ ) ( pxTCB )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_ಸರತಿ_REGISTRY_ಗಾತ್ರ
    #ವ್ಯಾಖ್ಯಾನಿಸು config_ಸರತಿ_REGISTRY_ಗಾತ್ರ    0U
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ ( config_ಸರತಿ_REGISTRY_ಗಾತ್ರ < 1 )
    #ವ್ಯಾಖ್ಯಾನಿಸು v_ಸರತಿ_ಕೂಡಿಸು_ಗಮ್ಯ_Registry( x_ಸರತಿ, pcName )
    #ವ್ಯಾಖ್ಯಾನಿಸು v_ಸರತಿ_Unregister_ಸರತಿ( x_ಸರತಿ )
    #ವ್ಯಾಖ್ಯಾನಿಸು pc_ಸರತಿ_Get_Name( x_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_USE_MINI_LIST_ಅಂಶ
    #ವ್ಯಾಖ್ಯಾನಿಸು config_USE_MINI_LIST_ಅಂಶ    1
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_ಸೂಚಕ_ಗಾತ್ರ_TYPE
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ಸೂಚಕ_ಗಾತ್ರ_TYPE    uint32_t
#ಆದರೆ_ಅಂತ್ಯ

/* Remove any unused trace macros. */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಆರಂಭ

/* Used to perform any necessary initialisation - for example, open a file
 * into which trace is to be written. */
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಆರಂಭ()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಕೊನೆ

/* Use to close a trace, for example close a file into which trace has been
 * written. */
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಕೊನೆ()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_TASK_SWITCHED_ಒಳ

/* Called after a task has been selected to run.  pxCurrentTCB holds a pointer
 * to the task control block of the selected task. */
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_TASK_SWITCHED_ಒಳ()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_INCREASE_TICK_ಎಣಿಕೆ

/* Called before stepping the tick count after waking from tickless idle
 * sleep. */
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_INCREASE_TICK_ಎಣಿಕೆ( x )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಕೆಳಗಿನ_POWER_IDLE_BEGIN
    /* Called immediately before entering tickless idle. */
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಕೆಳಗಿನ_POWER_IDLE_BEGIN()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಕೆಳಗಿನ_POWER_IDLE_ಕೊನೆ
    /* Called when returning to the Idle task after a tickless idle. */
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಕೆಳಗಿನ_POWER_IDLE_ಕೊನೆ()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_TASK_SWITCHED_ಹೊರಹೋಗು

/* Called before a task has been selected to run.  pxCurrentTCB holds a pointer
 * to the task control block of the task being switched out. */
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_TASK_SWITCHED_ಹೊರಹೋಗು()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_TASK_ಆದ್ಯತೆ_INHERIT

/* Called when a task attempts to take a mutex that is already held by a
 * lower priority task.  pxTCBOfMutexHolder is a pointer to the TCB of the task
 * that holds the mutex.  uxInheritedPriority is the priority the mutex holder
 * will inherit (the priority of the task that is attempting to obtain the
 * muted. */
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_TASK_ಆದ್ಯತೆ_INHERIT( pxTCBOfMutexHolder, ux_Inherited_ಆದ್ಯತೆ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_TASK_ಆದ್ಯತೆ_DISINHERIT

/* Called when a task releases a mutex, the holding of which had resulted in
 * the task inheriting the priority of a higher priority task.
 * pxTCBOfMutexHolder is a pointer to the TCB of the task that is releasing the
 * mutex.  uxOriginalPriority is the task's configured (base) priority. */
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_TASK_ಆದ್ಯತೆ_DISINHERIT( pxTCBOfMutexHolder, ux_Original_ಆದ್ಯತೆ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_BLOCKING_ON_ಸರತಿ_RECEIVE

/* Task is about to block because it cannot read from a
 * queue/mutex/semaphore.  pxQueue is a pointer to the queue/mutex/semaphore
 * upon which the read was attempted.  pxCurrentTCB points to the TCB of the
 * task that attempted the read. */
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_BLOCKING_ON_ಸರತಿ_RECEIVE( px_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_BLOCKING_ON_ಸರತಿ_PEEK

/* Task is about to block because it cannot read from a
 * queue/mutex/semaphore.  pxQueue is a pointer to the queue/mutex/semaphore
 * upon which the read was attempted.  pxCurrentTCB points to the TCB of the
 * task that attempted the read. */
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_BLOCKING_ON_ಸರತಿ_PEEK( px_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_BLOCKING_ON_ಸರತಿ_SEND

/* Task is about to block because it cannot write to a
 * queue/mutex/semaphore.  pxQueue is a pointer to the queue/mutex/semaphore
 * upon which the write was attempted.  pxCurrentTCB points to the TCB of the
 * task that attempted the write. */
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_BLOCKING_ON_ಸರತಿ_SEND( px_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_CHECK_FOR_ರಾಶಿ_ಅತಿಪ್ರವಾಹ
    #ವ್ಯಾಖ್ಯಾನಿಸು config_CHECK_FOR_ರಾಶಿ_ಅತಿಪ್ರವಾಹ    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_ದಾಖಲೆ_ರಾಶಿ_ಮೇಲಿನ_ವಿಳಾಸ
    #ವ್ಯಾಖ್ಯಾನಿಸು config_ದಾಖಲೆ_ರಾಶಿ_ಮೇಲಿನ_ವಿಳಾಸ    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H
    #ವ್ಯಾಖ್ಯಾನಿಸು configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H    0
#ಆದರೆ_ಅಂತ್ಯ

/* The following event macros are embedded in the kernel API calls. */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_MOVED_TASK_ಗಮ್ಯ_READY_STATE
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_MOVED_TASK_ಗಮ್ಯ_READY_STATE( pxTCB )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_POST_MOVED_TASK_ಗಮ್ಯ_READY_STATE
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_POST_MOVED_TASK_ಗಮ್ಯ_READY_STATE( pxTCB )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಸರತಿ_CREATE
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಸರತಿ_CREATE( px_New_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಸರತಿ_CREATE_FAILED
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಸರತಿ_CREATE_FAILED( uc_ಸರತಿ_Type )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceCREATE_MUTEX
    #ವ್ಯಾಖ್ಯಾನಿಸು traceCREATE_MUTEX( px_New_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceCREATE_MUTEX_FAILED
    #ವ್ಯಾಖ್ಯಾನಿಸು traceCREATE_MUTEX_FAILED()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceGIVE_MUTEX_RECURSIVE
    #ವ್ಯಾಖ್ಯಾನಿಸು traceGIVE_MUTEX_RECURSIVE( pxMutex )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceGIVE_MUTEX_RECURSIVE_FAILED
    #ವ್ಯಾಖ್ಯಾನಿಸು traceGIVE_MUTEX_RECURSIVE_FAILED( pxMutex )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceTAKE_MUTEX_RECURSIVE
    #ವ್ಯಾಖ್ಯಾನಿಸು traceTAKE_MUTEX_RECURSIVE( pxMutex )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceTAKE_MUTEX_RECURSIVE_FAILED
    #ವ್ಯಾಖ್ಯಾನಿಸು traceTAKE_MUTEX_RECURSIVE_FAILED( pxMutex )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceCREATE_COUNTING_SEMAPHORE
    #ವ್ಯಾಖ್ಯಾನಿಸು traceCREATE_COUNTING_SEMAPHORE()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceCREATE_COUNTING_SEMAPHORE_FAILED
    #ವ್ಯಾಖ್ಯಾನಿಸು traceCREATE_COUNTING_SEMAPHORE_FAILED()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಸರತಿ_ಗಣ_SEND
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಸರತಿ_ಗಣ_SEND    trace_ಸರತಿ_SEND
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಸರತಿ_SEND
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಸರತಿ_SEND( px_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಸರತಿ_SEND_FAILED
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಸರತಿ_SEND_FAILED( px_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಸರತಿ_RECEIVE
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಸರತಿ_RECEIVE( px_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಸರತಿ_PEEK
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಸರತಿ_PEEK( px_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಸರತಿ_PEEK_FAILED
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಸರತಿ_PEEK_FAILED( px_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಸರತಿ_PEEK_FROM_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಸರತಿ_PEEK_FROM_ISR( px_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಸರತಿ_RECEIVE_FAILED
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಸರತಿ_RECEIVE_FAILED( px_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಸರತಿ_SEND_FROM_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಸರತಿ_SEND_FROM_ISR( px_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಸರತಿ_SEND_FROM_ISR_FAILED
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಸರತಿ_SEND_FROM_ISR_FAILED( px_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಸರತಿ_RECEIVE_FROM_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಸರತಿ_RECEIVE_FROM_ISR( px_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಸರತಿ_RECEIVE_FROM_ISR_FAILED
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಸರತಿ_RECEIVE_FROM_ISR_FAILED( px_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಸರತಿ_PEEK_FROM_ISR_FAILED
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಸರತಿ_PEEK_FROM_ISR_FAILED( px_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಸರತಿ_DELETE
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಸರತಿ_DELETE( px_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceTASK_CREATE
    #ವ್ಯಾಖ್ಯಾನಿಸು traceTASK_CREATE( pxNewTCB )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceTASK_CREATE_FAILED
    #ವ್ಯಾಖ್ಯಾನಿಸು traceTASK_CREATE_FAILED()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceTASK_DELETE
    #ವ್ಯಾಖ್ಯಾನಿಸು traceTASK_DELETE( px_Task_ಗಮ್ಯ_Delete )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceTASK_DELAY_UNTIL
    #ವ್ಯಾಖ್ಯಾನಿಸು traceTASK_DELAY_UNTIL( x )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceTASK_DELAY
    #ವ್ಯಾಖ್ಯಾನಿಸು traceTASK_DELAY()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_TASK_ಆದ್ಯತೆ_ಗಣ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_TASK_ಆದ್ಯತೆ_ಗಣ( pxTask, ux_New_ಆದ್ಯತೆ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceTASK_SUSPEND
    #ವ್ಯಾಖ್ಯಾನಿಸು traceTASK_SUSPEND( px_Task_ಗಮ್ಯ_Suspend )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceTASK_RESUME
    #ವ್ಯಾಖ್ಯಾನಿಸು traceTASK_RESUME( px_Task_ಗಮ್ಯ_Resume )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceTASK_RESUME_FROM_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು traceTASK_RESUME_FROM_ISR( px_Task_ಗಮ್ಯ_Resume )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceTASK_INCREMENT_TICK
    #ವ್ಯಾಖ್ಯಾನಿಸು traceTASK_INCREMENT_TICK( x_Tick_ಎಣಿಕೆ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceTIMER_CREATE
    #ವ್ಯಾಖ್ಯಾನಿಸು traceTIMER_CREATE( pxNewTimer )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceTIMER_CREATE_FAILED
    #ವ್ಯಾಖ್ಯಾನಿಸು traceTIMER_CREATE_FAILED()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceTIMER_COMMAND_SEND
    #ವ್ಯಾಖ್ಯಾನಿಸು traceTIMER_COMMAND_SEND( xTimer, x_ಸಂದೇಶ_ID, x_ಸಂದೇಶ_ಮೌಲ್ಯ_ಮೌಲ್ಯ, xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceTIMER_EXPIRED
    #ವ್ಯಾಖ್ಯಾನಿಸು traceTIMER_EXPIRED( pxTimer )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceTIMER_COMMAND_RECEIVED
    #ವ್ಯಾಖ್ಯಾನಿಸು traceTIMER_COMMAND_RECEIVED( pxTimer, x_ಸಂದೇಶ_ID, x_ಸಂದೇಶ_ಮೌಲ್ಯ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceMALLOC
    #ವ್ಯಾಖ್ಯಾನಿಸು traceMALLOC( pv_ವಿಳಾಸ, ui_ಗಾತ್ರ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceFREE
    #ವ್ಯಾಖ್ಯಾನಿಸು traceFREE( pv_ವಿಳಾಸ, ui_ಗಾತ್ರ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_EVENT_ಗುಂಪು_CREATE
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_EVENT_ಗುಂಪು_CREATE( x_Event_ಗುಂಪು )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_EVENT_ಗುಂಪು_CREATE_FAILED
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_EVENT_ಗುಂಪು_CREATE_FAILED()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_EVENT_ಗುಂಪು_SYNC_ಖಂಡ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_EVENT_ಗುಂಪು_SYNC_ಖಂಡ( x_Event_ಗುಂಪು, ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_ಗಣ, ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Wait_For )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_EVENT_ಗುಂಪು_SYNC_ಕೊನೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_EVENT_ಗುಂಪು_SYNC_ಕೊನೆ( x_Event_ಗುಂಪು, ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_ಗಣ, ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Wait_For, xTimeoutOccurred )    ( ಶೂನ್ಯ ) ( xTimeoutOccurred )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_EVENT_ಗುಂಪು_WAIT_ಬಿಟ್ಟುಗಳು_ಖಂಡ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_EVENT_ಗುಂಪು_WAIT_ಬಿಟ್ಟುಗಳು_ಖಂಡ( x_Event_ಗುಂಪು, ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Wait_For )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_EVENT_ಗುಂಪು_WAIT_ಬಿಟ್ಟುಗಳು_ಕೊನೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_EVENT_ಗುಂಪು_WAIT_ಬಿಟ್ಟುಗಳು_ಕೊನೆ( x_Event_ಗುಂಪು, ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Wait_For, xTimeoutOccurred )    ( ಶೂನ್ಯ ) ( xTimeoutOccurred )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_EVENT_ಗುಂಪು_CLEAR_ಬಿಟ್ಟುಗಳು
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_EVENT_ಗುಂಪು_CLEAR_ಬಿಟ್ಟುಗಳು( x_Event_ಗುಂಪು, ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Clear )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_EVENT_ಗುಂಪು_CLEAR_ಬಿಟ್ಟುಗಳು_FROM_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_EVENT_ಗುಂಪು_CLEAR_ಬಿಟ್ಟುಗಳು_FROM_ISR( x_Event_ಗುಂಪು, ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Clear )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_EVENT_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_EVENT_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು( x_Event_ಗುಂಪು, ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_ಗಣ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_EVENT_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು_FROM_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_EVENT_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು_FROM_ISR( x_Event_ಗುಂಪು, ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_ಗಣ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_EVENT_ಗುಂಪು_DELETE
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_EVENT_ಗುಂಪು_DELETE( x_Event_ಗುಂಪು )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ tracePEND_FUNC_CALL
    #ವ್ಯಾಖ್ಯಾನಿಸು tracePEND_FUNC_CALL( x_Function_ಗಮ್ಯ_Pend, pvParameter1, ulParameter2, ret )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ tracePEND_FUNC_CALL_FROM_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು tracePEND_FUNC_CALL_FROM_ISR( x_Function_ಗಮ್ಯ_Pend, pvParameter1, ulParameter2, ret )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಸರತಿ_REGISTRY_ಕೂಡಿಸು
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಸರತಿ_REGISTRY_ಕೂಡಿಸು( x_ಸರತಿ, pc_ಸರತಿ_Name )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_TASK_NOTIFY_TAKE_ಖಂಡ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_TASK_NOTIFY_TAKE_ಖಂಡ( ux_ಸೂಚ್ಯಂಕ_ಗಮ್ಯ_Wait )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceTASK_NOTIFY_TAKE
    #ವ್ಯಾಖ್ಯಾನಿಸು traceTASK_NOTIFY_TAKE( ux_ಸೂಚ್ಯಂಕ_ಗಮ್ಯ_Wait )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_TASK_NOTIFY_WAIT_ಖಂಡ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_TASK_NOTIFY_WAIT_ಖಂಡ( ux_ಸೂಚ್ಯಂಕ_ಗಮ್ಯ_Wait )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceTASK_NOTIFY_WAIT
    #ವ್ಯಾಖ್ಯಾನಿಸು traceTASK_NOTIFY_WAIT( ux_ಸೂಚ್ಯಂಕ_ಗಮ್ಯ_Wait )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceTASK_NOTIFY
    #ವ್ಯಾಖ್ಯಾನಿಸು traceTASK_NOTIFY( ux_ಸೂಚ್ಯಂಕ_ಗಮ್ಯ_Notify )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceTASK_NOTIFY_FROM_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು traceTASK_NOTIFY_FROM_ISR( ux_ಸೂಚ್ಯಂಕ_ಗಮ್ಯ_Notify )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceTASK_NOTIFY_GIVE_FROM_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು traceTASK_NOTIFY_GIVE_FROM_ISR( ux_ಸೂಚ್ಯಂಕ_ಗಮ್ಯ_Notify )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಹರಿವು_ಬಫರ್_CREATE_FAILED
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಹರಿವು_ಬಫರ್_CREATE_FAILED( x_Is_ಸಂದೇಶ_ಬಫರ್ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಹರಿವು_ಬಫರ್_CREATE_STATIC_FAILED
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಹರಿವು_ಬಫರ್_CREATE_STATIC_FAILED( xReturn, x_Is_ಸಂದೇಶ_ಬಫರ್ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಹರಿವು_ಬಫರ್_CREATE
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಹರಿವು_ಬಫರ್_CREATE( px_ಹರಿವು_ಬಫರ್, x_Is_ಸಂದೇಶ_ಬಫರ್ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಹರಿವು_ಬಫರ್_DELETE
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಹರಿವು_ಬಫರ್_DELETE( x_ಹರಿವು_ಬಫರ್ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಹರಿವು_ಬಫರ್_RESET
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಹರಿವು_ಬಫರ್_RESET( x_ಹರಿವು_ಬಫರ್ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_BLOCKING_ON_ಹರಿವು_ಬಫರ್_SEND
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_BLOCKING_ON_ಹರಿವು_ಬಫರ್_SEND( x_ಹರಿವು_ಬಫರ್ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಹರಿವು_ಬಫರ್_SEND
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಹರಿವು_ಬಫರ್_SEND( x_ಹರಿವು_ಬಫರ್, x_ಬೈಟುಗಳು_ಕಳುಹಿಸಿದ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಹರಿವು_ಬಫರ್_SEND_FAILED
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಹರಿವು_ಬಫರ್_SEND_FAILED( x_ಹರಿವು_ಬಫರ್ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಹರಿವು_ಬಫರ್_SEND_FROM_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಹರಿವು_ಬಫರ್_SEND_FROM_ISR( x_ಹರಿವು_ಬಫರ್, x_ಬೈಟುಗಳು_ಕಳುಹಿಸಿದ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_BLOCKING_ON_ಹರಿವು_ಬಫರ್_RECEIVE
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_BLOCKING_ON_ಹರಿವು_ಬಫರ್_RECEIVE( x_ಹರಿವು_ಬಫರ್ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಹರಿವು_ಬಫರ್_RECEIVE
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಹರಿವು_ಬಫರ್_RECEIVE( x_ಹರಿವು_ಬಫರ್, x_Received_ಉದ್ದಳತೆ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಹರಿವು_ಬಫರ್_RECEIVE_FAILED
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಹರಿವು_ಬಫರ್_RECEIVE_FAILED( x_ಹರಿವು_ಬಫರ್ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಹರಿವು_ಬಫರ್_RECEIVE_FROM_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಹರಿವು_ಬಫರ್_RECEIVE_FROM_ISR( x_ಹರಿವು_ಬಫರ್, x_Received_ಉದ್ದಳತೆ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_GENERATE_ಸತತ_ಸಮಯ_STATS
    #ವ್ಯಾಖ್ಯಾನಿಸು config_GENERATE_ಸತತ_ಸಮಯ_STATS    0
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ ( config_GENERATE_ಸತತ_ಸಮಯ_STATS == 1 )

    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_CONFIGURE_TIMER_FOR_ಸತತ_ಸಮಯ_STATS
        #ದೋಷ If config_GENERATE_ಸತತ_ಸಮಯ_STATS is defined then ಪೋರ್ಟ್_CONFIGURE_TIMER_FOR_ಸತತ_ಸಮಯ_STATS must also be defined.  ಪೋರ್ಟ್_CONFIGURE_TIMER_FOR_ಸತತ_ಸಮಯ_STATS should call a ಪೋರ್ಟ್ layer function ಗಮ್ಯ ಸಿದ್ಧತೆ a peripheral timer/counter that can then be ಬಳಸಿದ as the ಸತತ time counter time ತಳ.
    #ಆದರೆ_ಅಂತ್ಯ /* portCONFIGURE_TIMER_FOR_RUN_TIME_STATS */

    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_GET_ಸತತ_ಸಮಯ_COUNTER_ಮೌಲ್ಯ
        #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_ALT_GET_ಸತತ_ಸಮಯ_COUNTER_ಮೌಲ್ಯ
            #ದೋಷ If config_GENERATE_ಸತತ_ಸಮಯ_STATS is defined then either ಪೋರ್ಟ್_GET_ಸತತ_ಸಮಯ_COUNTER_ಮೌಲ್ಯ or ಪೋರ್ಟ್_ALT_GET_ಸತತ_ಸಮಯ_COUNTER_ಮೌಲ್ಯ must also be defined.  See the examples provided and the FreeRTOS web site ಗಾಗಿ ಇನ್ನಷ್ಟು information.
        #ಆದರೆ_ಅಂತ್ಯ /* portALT_GET_RUN_TIME_COUNTER_VALUE */
    #ಆದರೆ_ಅಂತ್ಯ /* portGET_RUN_TIME_COUNTER_VALUE */

#ಆದರೆ_ಅಂತ್ಯ /* configGENERATE_RUN_TIME_STATS */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_CONFIGURE_TIMER_FOR_ಸತತ_ಸಮಯ_STATS
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_CONFIGURE_TIMER_FOR_ಸತತ_ಸಮಯ_STATS()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configUSE_MALLOC_FAILED_HOOK
    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_MALLOC_FAILED_HOOK    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_PRIVILEGE_ಬಿಟ್
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_PRIVILEGE_ಬಿಟ್    ( ( UBaseType_t ) 0x00 )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_YIELD_WITHIN_API
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_YIELD_WITHIN_API    ಪೋರ್ಟ್_YIELD
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_SUPPRESS_TICKS_AND_SLEEP
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_SUPPRESS_TICKS_AND_SLEEP( x_Expected_Idle_ಸಮಯ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_EXPECTED_IDLE_ಸಮಯ_BEFORE_SLEEP
    #ವ್ಯಾಖ್ಯಾನಿಸು config_EXPECTED_IDLE_ಸಮಯ_BEFORE_SLEEP    2
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ config_EXPECTED_IDLE_ಸಮಯ_BEFORE_SLEEP < 2
    #ದೋಷ config_EXPECTED_IDLE_ಸಮಯ_BEFORE_SLEEP must not be less than 2
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configUSE_TICKLESS_IDLE
    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_TICKLESS_IDLE    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configPRE_SUPPRESS_TICKS_AND_SLEEP_PROCESSING
    #ವ್ಯಾಖ್ಯಾನಿಸು configPRE_SUPPRESS_TICKS_AND_SLEEP_PROCESSING( x )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configPRE_SLEEP_PROCESSING
    #ವ್ಯಾಖ್ಯಾನಿಸು configPRE_SLEEP_PROCESSING( x )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configPOST_SLEEP_PROCESSING
    #ವ್ಯಾಖ್ಯಾನಿಸು configPOST_SLEEP_PROCESSING( x )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_USE_ಸರತಿ_SETS
    #ವ್ಯಾಖ್ಯಾನಿಸು config_USE_ಸರತಿ_SETS    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_TASK_ಬಳಸುತ್ತದೆ_FLOATING_ಬಿಂದು
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TASK_ಬಳಸುತ್ತದೆ_FLOATING_ಬಿಂದು()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_ALLOCATE_SECURE_CONTEXT
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ALLOCATE_SECURE_CONTEXT( ul_Secure_ರಾಶಿ_ಗಾತ್ರ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_DONT_DISCARD
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_DONT_DISCARD
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_USE_ಸಮಯ_SLICING
    #ವ್ಯಾಖ್ಯಾನಿಸು config_USE_ಸಮಯ_SLICING    1
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS
    #ವ್ಯಾಖ್ಯಾನಿಸು configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configUSE_STATS_FORMATTING_FUNCTIONS
    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_STATS_FORMATTING_FUNCTIONS    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_ASSERT_IF_ಅಡಚಣೆ_ಆದ್ಯತೆ_INVALID
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ASSERT_IF_ಅಡಚಣೆ_ಆದ್ಯತೆ_INVALID()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configUSE_TRACE_FACILITY
    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_TRACE_FACILITY    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ mtCOVERAGE_TEST_MARKER
    #ವ್ಯಾಖ್ಯಾನಿಸು mtCOVERAGE_TEST_MARKER()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ mtCOVERAGE_TEST_DELAY
    #ವ್ಯಾಖ್ಯಾನಿಸು mtCOVERAGE_TEST_DELAY()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_ASSERT_IF_ಒಳ_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ASSERT_IF_ಒಳ_ISR()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_USE_ಪೋರ್ಟ್_OPTIMISED_TASK_SELECTION
    #ವ್ಯಾಖ್ಯಾನಿಸು config_USE_ಪೋರ್ಟ್_OPTIMISED_TASK_SELECTION    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_APPLICATION_ALLOCATED_ರಾಶಿ
    #ವ್ಯಾಖ್ಯಾನಿಸು config_APPLICATION_ALLOCATED_ರಾಶಿ    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configUSE_TASK_NOTIFICATIONS
    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_TASK_NOTIFICATIONS    1
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configTASK_NOTIFICATION_ARRAY_ENTRIES
    #ವ್ಯಾಖ್ಯಾನಿಸು configTASK_NOTIFICATION_ARRAY_ENTRIES    1
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ configTASK_NOTIFICATION_ARRAY_ENTRIES < 1
    #ದೋಷ configTASK_NOTIFICATION_ARRAY_ENTRIES must be at ಕನಿಷ್ಠ 1
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configUSE_POSIX_ERRNO
    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_POSIX_ERRNO    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configUSE_SB_COMPLETED_CALLBACK

/* By default per-instance callbacks are not enabled for stream buffer or message buffer. */
    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_SB_COMPLETED_CALLBACK    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_TICK_TYPE_IS_ATOMIC
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TICK_TYPE_IS_ATOMIC    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_SUPPORT_STATIC_ಹಂಚಿಕೆ
    /* Defaults to 0 for backward compatibility. */
    #ವ್ಯಾಖ್ಯಾನಿಸು config_SUPPORT_STATIC_ಹಂಚಿಕೆ    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_SUPPORT_DYNAMIC_ಹಂಚಿಕೆ
    /* Defaults to 1 for backward compatibility. */
    #ವ್ಯಾಖ್ಯಾನಿಸು config_SUPPORT_DYNAMIC_ಹಂಚಿಕೆ    1
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ ( ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 ) && ( config_SUPPORT_DYNAMIC_ಹಂಚಿಕೆ != 1 ) )
    #ದೋಷ configUSE_STATS_FORMATTING_FUNCTIONS cannot be ಬಳಸಿದ without dynamic ಹಂಚಿಕೆ, but config_SUPPORT_DYNAMIC_ಹಂಚಿಕೆ is not ಗಣ ಗಮ್ಯ 1.
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ ( configUSE_STATS_FORMATTING_FUNCTIONS > 0 )
    #ಆದರೆ ( ( configUSE_TRACE_FACILITY != 1 ) && ( config_GENERATE_ಸತತ_ಸಮಯ_STATS != 1 ) )
        #ದೋಷ configUSE_STATS_FORMATTING_FUNCTIONS is 1 but the functions it enables are not ಬಳಸಿದ because neither configUSE_TRACE_FACILITY or config_GENERATE_ಸತತ_ಸಮಯ_STATS are 1.  ಗಣ_2 configUSE_STATS_FORMATTING_FUNCTIONS ಗಮ್ಯ 0 ಒಳ FreeRTOSConfig.h.
    #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_ರಾಶಿ_ಆಳ_TYPE

/* Defaults to uint16_t for backward compatibility, but can be overridden
 * in FreeRTOSConfig.h if uint16_t is too restrictive. */
    #ವ್ಯಾಖ್ಯಾನಿಸು config_ರಾಶಿ_ಆಳ_TYPE    uint16_t
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_ಸತತ_ಸಮಯ_COUNTER_TYPE

/* Defaults to uint32_t for backward compatibility, but can be overridden in
 * FreeRTOSConfig.h if uint32_t is too restrictive. */

    #ವ್ಯಾಖ್ಯಾನಿಸು config_ಸತತ_ಸಮಯ_COUNTER_TYPE    uint32_t
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_ಸಂದೇಶ_ಬಫರ್_ಉದ್ದಳತೆ_TYPE

/* Defaults to size_t for backward compatibility, but can be overridden
 * in FreeRTOSConfig.h if lengths will always be less than the number of bytes
 * in a size_t. */
    #ವ್ಯಾಖ್ಯಾನಿಸು config_ಸಂದೇಶ_ಬಫರ್_ಉದ್ದಳತೆ_TYPE    size_t
#ಆದರೆ_ಅಂತ್ಯ

/* Sanity check the configuration. */
#ಆದರೆ ( ( config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 0 ) && ( config_SUPPORT_DYNAMIC_ಹಂಚಿಕೆ == 0 ) )
    #ದೋಷ config_SUPPORT_STATIC_ಹಂಚಿಕೆ and config_SUPPORT_DYNAMIC_ಹಂಚಿಕೆ cannot both be 0, but can both be 1.
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ ( ( configUSE_RECURSIVE_MUTEXES == 1 ) && ( configUSE_MUTEXES != 1 ) )
    #ದೋಷ configUSE_MUTEXES must be ಗಣ ಗಮ್ಯ 1 ಗಮ್ಯ use recursive mutexes
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ ( ( config_ಸಂಖ್ಯೆ_ದ_CORES > 1 ) && ( config_USE_ಪೋರ್ಟ್_OPTIMISED_TASK_SELECTION != 0 ) )
    #ದೋಷ config_USE_ಪೋರ್ಟ್_OPTIMISED_TASK_SELECTION is not supported ಒಳ SMP
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_INITIAL_TICK_ಎಣಿಕೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು config_INITIAL_TICK_ಎಣಿಕೆ    0
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ ( ಪೋರ್ಟ್_TICK_TYPE_IS_ATOMIC == 0 )

/* Either variables of tick type cannot be read atomically, or
 * portTICK_TYPE_IS_ATOMIC was not set - map the critical sections used when
 * the tick count is returned to the standard critical section macros. */
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TICK_TYPE_ENTER_CRITICAL()                      ಪೋರ್ಟ್_ENTER_CRITICAL()
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TICK_TYPE_EXIT_CRITICAL()                       ಪೋರ್ಟ್_EXIT_CRITICAL()
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TICK_TYPE_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR()         ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR()
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TICK_TYPE_CLEAR_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR( x )    ಪೋರ್ಟ್_CLEAR_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR( ( x ) )
#ಇಲ್ಲದಿದ್ದರೆ

/* The tick type can be read atomically, so critical sections used when the
 * tick count is returned can be defined away. */
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TICK_TYPE_ENTER_CRITICAL()
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TICK_TYPE_EXIT_CRITICAL()
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TICK_TYPE_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR()         0
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TICK_TYPE_CLEAR_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR( x )    ( ಶೂನ್ಯ ) ( x )
#ಆದರೆ_ಅಂತ್ಯ /* if ( portTICK_TYPE_IS_ATOMIC == 0 ) */

/* Definitions to allow backward compatibility with FreeRTOS versions prior to
 * V8 if desired. */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configENABLE_BACKWARD_COMPATIBILITY
    #ವ್ಯಾಖ್ಯಾನಿಸು configENABLE_BACKWARD_COMPATIBILITY    1
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configPRINTF

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
    #ವ್ಯಾಖ್ಯಾನಿಸು configPRINTF( X )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configMAX

/* The application writer has not provided their own MAX macro, so define
 * the following generic implementation. */
    #ವ್ಯಾಖ್ಯಾನಿಸು configMAX( a, b )    ( ( ( a ) > ( b ) ) ? ( a ) : ( b ) )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configMIN

/* The application writer has not provided their own MIN macro, so define
 * the following generic implementation. */
    #ವ್ಯಾಖ್ಯಾನಿಸು configMIN( a, b )    ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ configENABLE_BACKWARD_COMPATIBILITY == 1
    #ವ್ಯಾಖ್ಯಾನಿಸು eTaskStateGet                 eTaskGetState
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_Tick_Type                  TickType_t
    #ವ್ಯಾಖ್ಯಾನಿಸು xTaskHandle                   TaskHandle_t
    #ವ್ಯಾಖ್ಯಾನಿಸು x_ಸರತಿ_Handle                  ಸರತಿ_Handle_t
    #ವ್ಯಾಖ್ಯಾನಿಸು xSemaphoreHandle              SemaphoreHandle_t
    #ವ್ಯಾಖ್ಯಾನಿಸು x_ಸರತಿ_ಗಣ_Handle               ಸರತಿ_ಗಣ_Handle_t
    #ವ್ಯಾಖ್ಯಾನಿಸು x_ಸರತಿ_ಗಣ_Member_Handle         ಸರತಿ_ಗಣ_Member_Handle_t
    #ವ್ಯಾಖ್ಯಾನಿಸು x_ಸಮಯ_ಹೊರಹೋಗು_Type                  ಸಮಯ_ಹೊರಹೋಗು_t
    #ವ್ಯಾಖ್ಯಾನಿಸು x_ಸ್ಮರಣೆ_Region                 ಸ್ಮರಣೆ_Region_t
    #ವ್ಯಾಖ್ಯಾನಿಸು xTaskParameters               TaskParameters_t
    #ವ್ಯಾಖ್ಯಾನಿಸು x_Task_ಸ್ಥಿತಿ_Type               Task_ಸ್ಥಿತಿ_t
    #ವ್ಯಾಖ್ಯಾನಿಸು xTimerHandle                  TimerHandle_t
    #ವ್ಯಾಖ್ಯಾನಿಸು xCoRoutineHandle              CoRoutineHandle_t
    #ವ್ಯಾಖ್ಯಾನಿಸು pd_TASK_HOOK_ಸಂಕೇತ              TaskHookFunction_t
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TICK_ದರ_MS              ಪೋರ್ಟ್_TICK_PERIOD_MS
    #ವ್ಯಾಖ್ಯಾನಿಸು pcTaskGetTaskName             pcTaskGetName
    #ವ್ಯಾಖ್ಯಾನಿಸು pcTimerGetTimerName           pcTimerGetName
    #ವ್ಯಾಖ್ಯಾನಿಸು pc_ಸರತಿ_Get_ಸರತಿ_Name           pc_ಸರತಿ_Get_Name
    #ವ್ಯಾಖ್ಯಾನಿಸು vTaskGetTaskInfo              vTaskGetInfo
    #ವ್ಯಾಖ್ಯಾನಿಸು x_Task_Get_Idle_ಸತತ_ಸಮಯ_Counter    ul_Task_Get_Idle_ಸತತ_ಸಮಯ_Counter

/* Backward compatibility within the scheduler code only - these definitions
 * are not really required but are included for completeness. */
    #ವ್ಯಾಖ್ಯಾನಿಸು tmrTIMER_CALLBACK             TimerCallbackFunction_t
    #ವ್ಯಾಖ್ಯಾನಿಸು pd_TASK_ಸಂಕೇತ                   TaskFunction_t
    #ವ್ಯಾಖ್ಯಾನಿಸು x_List_ಅಂಶ                     List_ಅಂಶ_t
    #ವ್ಯಾಖ್ಯಾನಿಸು xList                         List_t

/* For libraries that break the list data hiding, and access list structure
 * members directly (which is not supposed to be done). */
    #ವ್ಯಾಖ್ಯಾನಿಸು pxContainer                   pvContainer
#ಆದರೆ_ಅಂತ್ಯ /* configENABLE_BACKWARD_COMPATIBILITY */

#ಆದರೆ ( configUSE_ALTERNATIVE_API != 0 )
    #ದೋಷ The alternative API was deprecated some time ago, and was removed ಒಳ FreeRTOS V9.0 0
#ಆದರೆ_ಅಂತ್ಯ

/* Set configUSE_TASK_FPU_SUPPORT to 0 to omit floating point support even
 * if floating point hardware is otherwise supported by the FreeRTOS port in use.
 * This constant is not supported by all FreeRTOS ports that include floating
 * point support. */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configUSE_TASK_FPU_SUPPORT
    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_TASK_FPU_SUPPORT    1
#ಆದರೆ_ಅಂತ್ಯ

/* Set configENABLE_MPU to 1 to enable MPU support and 0 to disable it. This is
 * currently used in ARMv8M ports. */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configENABLE_MPU
    #ವ್ಯಾಖ್ಯಾನಿಸು configENABLE_MPU    0
#ಆದರೆ_ಅಂತ್ಯ

/* Set configENABLE_FPU to 1 to enable FPU support and 0 to disable it. This is
 * currently used in ARMv8M ports. */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configENABLE_FPU
    #ವ್ಯಾಖ್ಯಾನಿಸು configENABLE_FPU    1
#ಆದರೆ_ಅಂತ್ಯ

/* Set configENABLE_MVE to 1 to enable MVE support and 0 to disable it. This is
 * currently used in ARMv8M ports. */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configENABLE_MVE
    #ವ್ಯಾಖ್ಯಾನಿಸು configENABLE_MVE    0
#ಆದರೆ_ಅಂತ್ಯ

/* Set configENABLE_TRUSTZONE to 1 enable TrustZone support and 0 to disable it.
 * This is currently used in ARMv8M ports. */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configENABLE_TRUSTZONE
    #ವ್ಯಾಖ್ಯಾನಿಸು configENABLE_TRUSTZONE    1
#ಆದರೆ_ಅಂತ್ಯ

/* Set configRUN_FREERTOS_SECURE_ONLY to 1 to run the FreeRTOS ARMv8M port on
 * the Secure Side only. */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_ಸತತ_FREERTOS_SECURE_ONLY
    #ವ್ಯಾಖ್ಯಾನಿಸು config_ಸತತ_FREERTOS_SECURE_ONLY    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_ಸತತ_ADDITIONAL_TESTS
    #ವ್ಯಾಖ್ಯಾನಿಸು config_ಸತತ_ADDITIONAL_TESTS    0
#ಆದರೆ_ಅಂತ್ಯ


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
#ವ್ಯಾಖ್ಯಾನಿಸು tsk_STATIC_AND_DYNAMIC_ಹಂಚಿಕೆ_POSSIBLE                                                                                     \
    ( ( ( ಪೋರ್ಟ್_USING_MPU_WRAPPERS == 0 ) && ( config_SUPPORT_DYNAMIC_ಹಂಚಿಕೆ == 1 ) && ( config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 1 ) ) || \
      ( ( ಪೋರ್ಟ್_USING_MPU_WRAPPERS == 1 ) && ( config_SUPPORT_DYNAMIC_ಹಂಚಿಕೆ == 1 ) ) )

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
ರಚನೆ x_STATIC_LIST_ಅಂಶ
{
    #ಆದರೆ ( config_USE_LIST_ದತ್ತಾಂಶ_INTEGRITY_CHECK_ಬೈಟುಗಳು == 1 )
        TickType_t xDummy1;
    #ಆದರೆ_ಅಂತ್ಯ
    TickType_t xDummy2;
    ಶೂನ್ಯ * pvDummy3[ 4 ];
    #ಆದರೆ ( config_USE_LIST_ದತ್ತಾಂಶ_INTEGRITY_CHECK_ಬೈಟುಗಳು == 1 )
        TickType_t xDummy4;
    #ಆದರೆ_ಅಂತ್ಯ
};
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ x_STATIC_LIST_ಅಂಶ Static_List_ಅಂಶ_t;

#ಆದರೆ ( config_USE_MINI_LIST_ಅಂಶ == 1 )
    /* See the comments above the struct xSTATIC_LIST_ITEM definition. */
    ರಚನೆ x_STATIC_MINI_LIST_ಅಂಶ
    {
        #ಆದರೆ ( config_USE_LIST_ದತ್ತಾಂಶ_INTEGRITY_CHECK_ಬೈಟುಗಳು == 1 )
            TickType_t xDummy1;
        #ಆದರೆ_ಅಂತ್ಯ
        TickType_t xDummy2;
        ಶೂನ್ಯ * pvDummy3[ 2 ];
    };
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ x_STATIC_MINI_LIST_ಅಂಶ Static_Mini_List_ಅಂಶ_t;
#ಇಲ್ಲದಿದ್ದರೆ /* if ( configUSE_MINI_LIST_ITEM == 1 ) */
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ x_STATIC_LIST_ಅಂಶ      Static_Mini_List_ಅಂಶ_t;
#ಆದರೆ_ಅಂತ್ಯ /* if ( configUSE_MINI_LIST_ITEM == 1 ) */

/* See the comments above the struct xSTATIC_LIST_ITEM definition. */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ xSTATIC_LIST
{
    #ಆದರೆ ( config_USE_LIST_ದತ್ತಾಂಶ_INTEGRITY_CHECK_ಬೈಟುಗಳು == 1 )
        TickType_t xDummy1;
    #ಆದರೆ_ಅಂತ್ಯ
    UBaseType_t uxDummy2;
    ಶೂನ್ಯ * pvDummy3;
    Static_Mini_List_ಅಂಶ_t xDummy4;
    #ಆದರೆ ( config_USE_LIST_ದತ್ತಾಂಶ_INTEGRITY_CHECK_ಬೈಟುಗಳು == 1 )
        TickType_t xDummy5;
    #ಆದರೆ_ಅಂತ್ಯ
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
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ xSTATIC_TCB
{
    ಶೂನ್ಯ * pxDummy1;
    #ಆದರೆ ( ಪೋರ್ಟ್_USING_MPU_WRAPPERS == 1 )
        xMPU_SETTINGS xDummy2;
    #ಆದರೆ_ಅಂತ್ಯ
    Static_List_ಅಂಶ_t xDummy3[ 2 ];
    UBaseType_t uxDummy5;
    ಶೂನ್ಯ * pxDummy6;
    uint8_t ucDummy7[ configMAX_TASK_NAME_LEN ];
    #ಆದರೆ ( config_ಸಂಖ್ಯೆ_ದ_CORES > 1 )
        ತಳ_Type_t xDummyCoreID;
    #ಆದರೆ_ಅಂತ್ಯ /* configNUMBER_OF_CORES > 1 */
    #ಆದರೆ ( ( ಪೋರ್ಟ್_ರಾಶಿ_GROWTH > 0 ) || ( config_ದಾಖಲೆ_ರಾಶಿ_ಮೇಲಿನ_ವಿಳಾಸ == 1 ) )
        ಶೂನ್ಯ * pxDummy8;
    #ಆದರೆ_ಅಂತ್ಯ
    #ಆದರೆ ( ಪೋರ್ಟ್_CRITICAL_NESTING_ಒಳ_TCB == 1 )
        UBaseType_t uxDummy9;
    #ಆದರೆ_ಅಂತ್ಯ
    #ಆದರೆ ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy10[ 2 ];
    #ಆದರೆ_ಅಂತ್ಯ
    #ಆದರೆ ( configUSE_MUTEXES == 1 )
        UBaseType_t uxDummy12[ 2 ];
    #ಆದರೆ_ಅಂತ್ಯ
    #ಆದರೆ ( config_USE_APPLICATION_TASK_ಟ್ಯಾಗ್ == 1 )
        ಶೂನ್ಯ * pxDummy14;
    #ಆದರೆ_ಅಂತ್ಯ
    #ಆದರೆ ( config_ಅಂಕೆ_THREAD_LOCAL_STORAGE_ಸೂಚಕಗಳು > 0 )
        ಶೂನ್ಯ * pvDummy15[ config_ಅಂಕೆ_THREAD_LOCAL_STORAGE_ಸೂಚಕಗಳು ];
    #ಆದರೆ_ಅಂತ್ಯ
    #ಆದರೆ ( config_GENERATE_ಸತತ_ಸಮಯ_STATS == 1 )
        config_ಸತತ_ಸಮಯ_COUNTER_TYPE ulDummy16;
    #ಆದರೆ_ಅಂತ್ಯ
    #ಆದರೆ ( ( configUSE_NEWLIB_REENTRANT == 1 ) || ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 ) )
        config_TLS_ಖಂಡ_TYPE xDummy17;
    #ಆದರೆ_ಅಂತ್ಯ
    #ಆದರೆ ( configUSE_TASK_NOTIFICATIONS == 1 )
        uint32_t ulDummy18[ configTASK_NOTIFICATION_ARRAY_ENTRIES ];
        uint8_t ucDummy19[ configTASK_NOTIFICATION_ARRAY_ENTRIES ];
    #ಆದರೆ_ಅಂತ್ಯ
    #ಆದರೆ ( tsk_STATIC_AND_DYNAMIC_ಹಂಚಿಕೆ_POSSIBLE != 0 )
        uint8_t uxDummy20;
    #ಆದರೆ_ಅಂತ್ಯ

    #ಆದರೆ ( INCLUDE_xTaskAbortDelay == 1 )
        uint8_t ucDummy21;
    #ಆದರೆ_ಅಂತ್ಯ
    #ಆದರೆ ( configUSE_POSIX_ERRNO == 1 )
        ಪೂರ್ಣಾಂಕ iDummy22;
    #ಆದರೆ_ಅಂತ್ಯ
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
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ x_STATIC_ಸರತಿ
{
    ಶೂನ್ಯ * pvDummy1[ 3 ];

    ಒಕ್ಕೂಟ
    {
        ಶೂನ್ಯ * pvDummy2;
        UBaseType_t uxDummy2;
    } u;

    StaticList_t xDummy3[ 2 ];
    UBaseType_t uxDummy4[ 3 ];
    uint8_t ucDummy5[ 2 ];

    #ಆದರೆ ( ( config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 1 ) && ( config_SUPPORT_DYNAMIC_ಹಂಚಿಕೆ == 1 ) )
        uint8_t ucDummy6;
    #ಆದರೆ_ಅಂತ್ಯ

    #ಆದರೆ ( config_USE_ಸರತಿ_SETS == 1 )
        ಶೂನ್ಯ * pvDummy7;
    #ಆದರೆ_ಅಂತ್ಯ

    #ಆದರೆ ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy8;
        uint8_t ucDummy9;
    #ಆದರೆ_ಅಂತ್ಯ
    ಪೋರ್ಟ್_MUX_TYPE x_Dummy_ಸರತಿ_ಬೀಗ;
} Static_ಸರತಿ_t;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ Static_ಸರತಿ_t StaticSemaphore_t;

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
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ x_STATIC_EVENT_ಗುಂಪು
{
    TickType_t xDummy1;
    StaticList_t xDummy2;

    #ಆದರೆ ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy3;
    #ಆದರೆ_ಅಂತ್ಯ

    #ಆದರೆ ( ( config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 1 ) && ( config_SUPPORT_DYNAMIC_ಹಂಚಿಕೆ == 1 ) )
        uint8_t ucDummy4;
    #ಆದರೆ_ಅಂತ್ಯ
    ಪೋರ್ಟ್_MUX_TYPE x_Dummy_Event_ಗುಂಪು_ಬೀಗ;
} Static_Event_ಗುಂಪು_t;

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
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ xSTATIC_TIMER
{
    ಶೂನ್ಯ * pvDummy1;
    Static_List_ಅಂಶ_t xDummy2;
    TickType_t xDummy3;
    ಶೂನ್ಯ * pvDummy5;
    TaskFunction_t pvDummy6;
    #ಆದರೆ ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy7;
    #ಆದರೆ_ಅಂತ್ಯ
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
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ x_STATIC_ಹರಿವು_ಬಫರ್
{
    size_t uxDummy1[ 4 ];
    ಶೂನ್ಯ * pvDummy2[ 3 ];
    uint8_t ucDummy3;
    #ಆದರೆ ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t uxDummy4;
    #ಆದರೆ_ಅಂತ್ಯ
    #ಆದರೆ ( configUSE_SB_COMPLETED_CALLBACK == 1 )
        ಶೂನ್ಯ * pvDummy5[ 2 ];
    #ಆದರೆ_ಅಂತ್ಯ
    ಪೋರ್ಟ್_MUX_TYPE x_Dummy_ಹರಿವು_ಬಫರ್_ಬೀಗ;
} Static_ಹರಿವು_ಬಫರ್_t;

/* Message buffers are built on stream buffers. */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ Static_ಹರಿವು_ಬಫರ್_t Static_ಸಂದೇಶ_ಬಫರ್_t;

/* *INDENT-OFF* */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
    }
#ಆದರೆ_ಅಂತ್ಯ
/* *INDENT-ON* */

/*-----------------------------------------------------------
* IDF Compatibility
*----------------------------------------------------------*/

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ESP_PLATFORM

/* *INDENT-OFF* */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
    ಬಾಹ್ಯ "C" {
#ಆದರೆ_ಅಂತ್ಯ
/* *INDENT-ON* */

/*
 * Default values for trace macros added by ESP-IDF and are not part of Vanilla FreeRTOS
 */

    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ISR_EXIT_ಗಮ್ಯ_SCHEDULER
        #ವ್ಯಾಖ್ಯಾನಿಸು trace_ISR_EXIT_ಗಮ್ಯ_SCHEDULER()
    #ಆದರೆ_ಅಂತ್ಯ

    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceISR_EXIT
        #ವ್ಯಾಖ್ಯಾನಿಸು traceISR_EXIT()
    #ಆದರೆ_ಅಂತ್ಯ

    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceISR_ENTER
        #ವ್ಯಾಖ್ಯಾನಿಸು traceISR_ENTER( _n_ )
    #ಆದರೆ_ಅಂತ್ಯ

    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಸರತಿ_SEMAPHORE_RECEIVE
        #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಸರತಿ_SEMAPHORE_RECEIVE( px_ಸರತಿ )
    #ಆದರೆ_ಅಂತ್ಯ

    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಸರತಿ_GIVE_FROM_ISR
        #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಸರತಿ_GIVE_FROM_ISR( px_ಸರತಿ )
    #ಆದರೆ_ಅಂತ್ಯ

    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಸರತಿ_GIVE_FROM_ISR_FAILED
        #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಸರತಿ_GIVE_FROM_ISR_FAILED( px_ಸರತಿ )
    #ಆದರೆ_ಅಂತ್ಯ

/* *INDENT-OFF* */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
    }
#ಆದರೆ_ಅಂತ್ಯ
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
    #ಅನ್ತರ್ಭಾವ "freertos/idf_additions.h"

#ಆದರೆ_ಅಂತ್ಯ /* ESP_PLATFORM */

#ಆದರೆ_ಅಂತ್ಯ /* INC_FREERTOS_H */
