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

/* Acceptable values for configTICK_TYPE_WIDTH_IN_BITS. */
#ವ್ಯಾಖ್ಯಾನಿಸು TICK_TYPE_ಅಗಲ_16_ಬಿಟ್ಟುಗಳು    0
#ವ್ಯಾಖ್ಯಾನಿಸು TICK_TYPE_ಅಗಲ_32_ಬಿಟ್ಟುಗಳು    1
#ವ್ಯಾಖ್ಯಾನಿಸು TICK_TYPE_ಅಗಲ_64_ಬಿಟ್ಟುಗಳು    2

/* Application specific configuration options. */
#ಅನ್ತರ್ಭಾವ "FreeRTOSConfig.h"

#ಆದರೆ !defined( config_USE_16_ಬಿಟ್_TICKS ) && !defined( config_TICK_TYPE_ಅಗಲ_ಒಳ_ಬಿಟ್ಟುಗಳು )
    #ದೋಷ Missing definition:  One ದ config_USE_16_ಬಿಟ್_TICKS and config_TICK_TYPE_ಅಗಲ_ಒಳ_ಬಿಟ್ಟುಗಳು must be defined ಒಳ FreeRTOSConfig.h.  See the Configuration section ದ the FreeRTOS API documentation ಗಾಗಿ details.
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined( config_USE_16_ಬಿಟ್_TICKS ) && defined( config_TICK_TYPE_ಅಗಲ_ಒಳ_ಬಿಟ್ಟುಗಳು )
    #ದೋಷ Only one ದ config_USE_16_ಬಿಟ್_TICKS and config_TICK_TYPE_ಅಗಲ_ಒಳ_ಬಿಟ್ಟುಗಳು must be defined ಒಳ FreeRTOSConfig.h.  See the Configuration section ದ the FreeRTOS API documentation ಗಾಗಿ details.
#ಆದರೆ_ಅಂತ್ಯ

/* Define configTICK_TYPE_WIDTH_IN_BITS according to the
 * value of configUSE_16_BIT_TICKS for backward compatibility. */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_TICK_TYPE_ಅಗಲ_ಒಳ_ಬಿಟ್ಟುಗಳು
    #ಆದರೆ ( config_USE_16_ಬಿಟ್_TICKS == 1 )
        #ವ್ಯಾಖ್ಯಾನಿಸು config_TICK_TYPE_ಅಗಲ_ಒಳ_ಬಿಟ್ಟುಗಳು    TICK_TYPE_ಅಗಲ_16_ಬಿಟ್ಟುಗಳು
    #ಇಲ್ಲದಿದ್ದರೆ
        #ವ್ಯಾಖ್ಯಾನಿಸು config_TICK_TYPE_ಅಗಲ_ಒಳ_ಬಿಟ್ಟುಗಳು    TICK_TYPE_ಅಗಲ_32_ಬಿಟ್ಟುಗಳು
    #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

/* Set configUSE_MPU_WRAPPERS_V1 to 1 to use MPU wrappers v1. */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configUSE_MPU_WRAPPERS_V1
    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_MPU_WRAPPERS_V1    0
#ಆದರೆ_ಅಂತ್ಯ

/* Set configENABLE_ACCESS_CONTROL_LIST to 1 to enable access control list support. */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configENABLE_ACCESS_CONTROL_LIST
    #ವ್ಯಾಖ್ಯಾನಿಸು configENABLE_ACCESS_CONTROL_LIST    0
#ಆದರೆ_ಅಂತ್ಯ

/* Set default value of configNUMBER_OF_CORES to 1 to use single core FreeRTOS. */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_ಸಂಖ್ಯೆ_ದ_CORES
    #ವ್ಯಾಖ್ಯಾನಿಸು config_ಸಂಖ್ಯೆ_ದ_CORES    1
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configUSE_MALLOC_FAILED_HOOK
    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_MALLOC_FAILED_HOOK    0
#ಆದರೆ_ಅಂತ್ಯ

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

    #ಅನ್ತರ್ಭಾವ "newlib-freertos.h"

#ಆದರೆ_ಅಂತ್ಯ /* if ( configUSE_NEWLIB_REENTRANT == 1 ) */

/* Must be defaulted before configUSE_PICOLIBC_TLS is used below. */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configUSE_PICOLIBC_TLS
    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_PICOLIBC_TLS    0
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ ( configUSE_PICOLIBC_TLS == 1 )

    #ಅನ್ತರ್ಭಾವ "picolibc-freertos.h"

#ಆದರೆ_ಅಂತ್ಯ /* if ( configUSE_PICOLIBC_TLS == 1 ) */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configUSE_C_RUNTIME_TLS_SUPPORT
    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_C_RUNTIME_TLS_SUPPORT    0
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 )

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
#ಆದರೆ_ಅಂತ್ಯ /* if ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 ) */

/*
 * Check all the required application specific macros have been defined.
 * These macros are application specific and (as downloaded) are defined
 * within FreeRTOSConfig.h.
 */

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

#ಆದರೆ ( config_ಸಂಖ್ಯೆ_ದ_CORES > 1 )
    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configUSE_PASSIVE_IDLE_HOOK
        #ದೋಷ Missing definition:  configUSE_PASSIVE_IDLE_HOOK must be defined ಒಳ FreeRTOSConfig.h as either 1 or 0.  See the Configuration section ದ the FreeRTOS API documentation ಗಾಗಿ details.
    #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configUSE_TICK_HOOK
    #ದೋಷ Missing definition:  configUSE_TICK_HOOK must be defined ಒಳ FreeRTOSConfig.h as either 1 or 0.  See the Configuration section ದ the FreeRTOS API documentation ಗಾಗಿ details.
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ ( ( config_TICK_TYPE_ಅಗಲ_ಒಳ_ಬಿಟ್ಟುಗಳು != TICK_TYPE_ಅಗಲ_16_ಬಿಟ್ಟುಗಳು ) && \
    ( config_TICK_TYPE_ಅಗಲ_ಒಳ_ಬಿಟ್ಟುಗಳು != TICK_TYPE_ಅಗಲ_32_ಬಿಟ್ಟುಗಳು ) &&   \
    ( config_TICK_TYPE_ಅಗಲ_ಒಳ_ಬಿಟ್ಟುಗಳು != TICK_TYPE_ಅಗಲ_64_ಬಿಟ್ಟುಗಳು ) )
    #ದೋಷ Macro config_TICK_TYPE_ಅಗಲ_ಒಳ_ಬಿಟ್ಟುಗಳು is defined ಗಮ್ಯ incorrect ಮೌಲ್ಯ.  See the Configuration section ದ the FreeRTOS API documentation ಗಾಗಿ details.
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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configUSE_EVENT_GROUPS
    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_EVENT_GROUPS    1
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_USE_ಹರಿವು_BUFFERS
    #ವ್ಯಾಖ್ಯಾನಿಸು config_USE_ಹರಿವು_BUFFERS    1
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configUSE_DAEMON_TASK_STARTUP_HOOK
    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_DAEMON_TASK_STARTUP_HOOK    0
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ ( configUSE_DAEMON_TASK_STARTUP_HOOK != 0 )
    #ಆದರೆ ( configUSE_TIMERS == 0 )
        #ದೋಷ configUSE_DAEMON_TASK_STARTUP_HOOK is ಗಣ, but the daemon task is not created because configUSE_TIMERS is 0.
    #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configUSE_COUNTING_SEMAPHORES
    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_COUNTING_SEMAPHORES    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configUSE_TASK_PREEMPTION_DISABLE
    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_TASK_PREEMPTION_DISABLE    0
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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configCHECK_HANDLER_INSTALLATION
    #ವ್ಯಾಖ್ಯಾನಿಸು configCHECK_HANDLER_INSTALLATION    1
#ಇಲ್ಲದಿದ್ದರೆ

/* The application has explicitly defined configCHECK_HANDLER_INSTALLATION
 * to 1. The checks requires configASSERT() to be defined. */
    #ಆದರೆ ( ( configCHECK_HANDLER_INSTALLATION == 1 ) && ( configASSERT_DEFINED == 0 ) )
        #ದೋಷ You must define configASSERT() when configCHECK_HANDLER_INSTALLATION is 1.
    #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_ಸ್ಮರಣೆ_BARRIER
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ಸ್ಮರಣೆ_BARRIER()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_SOFTWARE_BARRIER
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_SOFTWARE_BARRIER()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_ಸತತ_ಅಪವರ್ತ್ಯ_PRIORITIES
    #ವ್ಯಾಖ್ಯಾನಿಸು config_ಸತತ_ಅಪವರ್ತ್ಯ_PRIORITIES    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_GET_CORE_ID

    #ಆದರೆ ( config_ಸಂಖ್ಯೆ_ದ_CORES == 1 )
        #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_GET_CORE_ID()    0
    #ಇಲ್ಲದಿದ್ದರೆ
        #ದೋಷ config_ಸಂಖ್ಯೆ_ದ_CORES is ಗಣ ಗಮ್ಯ ಇನ್ನಷ್ಟು than 1 then ಪೋರ್ಟ್_GET_CORE_ID must also be defined.
    #ಆದರೆ_ಅಂತ್ಯ /* configNUMBER_OF_CORES */

#ಆದರೆ_ಅಂತ್ಯ /* portGET_CORE_ID */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_YIELD_CORE

    #ಆದರೆ ( config_ಸಂಖ್ಯೆ_ದ_CORES == 1 )
        #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_YIELD_CORE( x )    ಪೋರ್ಟ್_YIELD()
    #ಇಲ್ಲದಿದ್ದರೆ
        #ದೋಷ config_ಸಂಖ್ಯೆ_ದ_CORES is ಗಣ ಗಮ್ಯ ಇನ್ನಷ್ಟು than 1 then ಪೋರ್ಟ್_YIELD_CORE must also be defined.
    #ಆದರೆ_ಅಂತ್ಯ /* configNUMBER_OF_CORES */

#ಆದರೆ_ಅಂತ್ಯ /* portYIELD_CORE */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ

    #ಆದರೆ ( config_ಸಂಖ್ಯೆ_ದ_CORES > 1 )
        #ದೋಷ ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ is required ಒಳ SMP
    #ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ /* portSET_INTERRUPT_MASK */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_CLEAR_ಅಡಚಣೆ_ಮುಖವಾಡ

    #ಆದರೆ ( config_ಸಂಖ್ಯೆ_ದ_CORES > 1 )
        #ದೋಷ ಪೋರ್ಟ್_CLEAR_ಅಡಚಣೆ_ಮುಖವಾಡ is required ಒಳ SMP
    #ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ /* portCLEAR_INTERRUPT_MASK */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_RELEASE_TASK_ಬೀಗ

    #ಆದರೆ ( config_ಸಂಖ್ಯೆ_ದ_CORES == 1 )
        #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_RELEASE_TASK_ಬೀಗ()
    #ಇಲ್ಲದಿದ್ದರೆ
        #ದೋಷ ಪೋರ್ಟ್_RELEASE_TASK_ಬೀಗ is required ಒಳ SMP
    #ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ /* portRELEASE_TASK_LOCK */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_GET_TASK_ಬೀಗ

    #ಆದರೆ ( config_ಸಂಖ್ಯೆ_ದ_CORES == 1 )
        #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_GET_TASK_ಬೀಗ()
    #ಇಲ್ಲದಿದ್ದರೆ
        #ದೋಷ ಪೋರ್ಟ್_GET_TASK_ಬೀಗ is required ಒಳ SMP
    #ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ /* portGET_TASK_LOCK */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_RELEASE_ISR_ಬೀಗ

    #ಆದರೆ ( config_ಸಂಖ್ಯೆ_ದ_CORES == 1 )
        #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_RELEASE_ISR_ಬೀಗ()
    #ಇಲ್ಲದಿದ್ದರೆ
        #ದೋಷ ಪೋರ್ಟ್_RELEASE_ISR_ಬೀಗ is required ಒಳ SMP
    #ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ /* portRELEASE_ISR_LOCK */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_GET_ISR_ಬೀಗ

    #ಆದರೆ ( config_ಸಂಖ್ಯೆ_ದ_CORES == 1 )
        #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_GET_ISR_ಬೀಗ()
    #ಇಲ್ಲದಿದ್ದರೆ
        #ದೋಷ ಪೋರ್ಟ್_GET_ISR_ಬೀಗ is required ಒಳ SMP
    #ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ /* portGET_ISR_LOCK */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_ENTER_CRITICAL_FROM_ISR

    #ಆದರೆ ( config_ಸಂಖ್ಯೆ_ದ_CORES > 1 )
        #ದೋಷ ಪೋರ್ಟ್_ENTER_CRITICAL_FROM_ISR is required ಒಳ SMP
    #ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_EXIT_CRITICAL_FROM_ISR

    #ಆದರೆ ( config_ಸಂಖ್ಯೆ_ದ_CORES > 1 )
        #ದೋಷ ಪೋರ್ಟ್_EXIT_CRITICAL_FROM_ISR is required ಒಳ SMP
    #ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configUSE_CORE_AFFINITY
    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_CORE_AFFINITY    0
#ಆದರೆ_ಅಂತ್ಯ /* configUSE_CORE_AFFINITY */

#ಆದರೆ ( ( config_ಸಂಖ್ಯೆ_ದ_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configTASK_DEFAULT_CORE_AFFINITY
        #ವ್ಯಾಖ್ಯಾನಿಸು configTASK_DEFAULT_CORE_AFFINITY    tskNO_AFFINITY
    #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configUSE_PASSIVE_IDLE_HOOK
    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_PASSIVE_IDLE_HOOK    0
#ಆದರೆ_ಅಂತ್ಯ /* configUSE_PASSIVE_IDLE_HOOK */

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

    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_TIMER_CALLBACK_ATTRIBUTE
        #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TIMER_CALLBACK_ATTRIBUTE
    #ಆದರೆ_ಅಂತ್ಯ /* portTIMER_CALLBACK_ATTRIBUTE */

#ಆದರೆ_ಅಂತ್ಯ /* configUSE_TIMERS */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_HAS_NESTED_INTERRUPTS
    #ಆದರೆ defined( ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR ) && defined( ಪೋರ್ಟ್_CLEAR_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR )
        #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_HAS_NESTED_INTERRUPTS    1
    #ಇಲ್ಲದಿದ್ದರೆ
        #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_HAS_NESTED_INTERRUPTS    0
    #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR
    #ಆದರೆ ( ಪೋರ್ಟ್_HAS_NESTED_INTERRUPTS == 1 )
        #ದೋಷ ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR must be defined ಗಾಗಿ ports that support nested interrupts (i.e. ಪೋರ್ಟ್_HAS_NESTED_INTERRUPTS is ಗಣ ಗಮ್ಯ 1)
    #ಇಲ್ಲದಿದ್ದರೆ
        #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR()    0
    #ಆದರೆ_ಅಂತ್ಯ
#ಇಲ್ಲದಿದ್ದರೆ
    #ಆದರೆ ( ಪೋರ್ಟ್_HAS_NESTED_INTERRUPTS == 0 )
        #ದೋಷ ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR must not be defined ಗಾಗಿ ports that ಮಾಡು not support nested interrupts (i.e. ಪೋರ್ಟ್_HAS_NESTED_INTERRUPTS is ಗಣ ಗಮ್ಯ 0)
    #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_CLEAR_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR
    #ಆದರೆ ( ಪೋರ್ಟ್_HAS_NESTED_INTERRUPTS == 1 )
        #ದೋಷ ಪೋರ್ಟ್_CLEAR_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR must be defined ಗಾಗಿ ports that support nested interrupts  (i.e. ಪೋರ್ಟ್_HAS_NESTED_INTERRUPTS is ಗಣ ಗಮ್ಯ 1)
    #ಇಲ್ಲದಿದ್ದರೆ
        #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_CLEAR_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR( ux_Saved_ಸ್ಥಿತಿ_ಮೌಲ್ಯ )    ( ಶೂನ್ಯ ) ( ux_Saved_ಸ್ಥಿತಿ_ಮೌಲ್ಯ )
    #ಆದರೆ_ಅಂತ್ಯ
#ಇಲ್ಲದಿದ್ದರೆ
    #ಆದರೆ ( ಪೋರ್ಟ್_HAS_NESTED_INTERRUPTS == 0 )
        #ದೋಷ ಪೋರ್ಟ್_CLEAR_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR must not be defined ಗಾಗಿ ports that ಮಾಡು not support nested interrupts (i.e. ಪೋರ್ಟ್_HAS_NESTED_INTERRUPTS is ಗಣ ಗಮ್ಯ 0)
    #ಆದರೆ_ಅಂತ್ಯ
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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_TASK_SWITCH_HOOK
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TASK_SWITCH_HOOK( pxTCB )    ( ಶೂನ್ಯ ) ( pxTCB )
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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_MOVED_TASK_ಗಮ್ಯ_DELAYED_LIST
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_MOVED_TASK_ಗಮ್ಯ_DELAYED_LIST()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_MOVED_TASK_ಗಮ್ಯ_ಅತಿಪ್ರವಾಹ_DELAYED_LIST
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_MOVED_TASK_ಗಮ್ಯ_ಅತಿಪ್ರವಾಹ_DELAYED_LIST()
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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ISR_EXIT_ಗಮ್ಯ_SCHEDULER
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ISR_EXIT_ಗಮ್ಯ_SCHEDULER()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceISR_EXIT
    #ವ್ಯಾಖ್ಯಾನಿಸು traceISR_EXIT()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceISR_ENTER
    #ವ್ಯಾಖ್ಯಾನಿಸು traceISR_ENTER()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಹರಿವು_ಬಫರ್_CREATE_FAILED
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಹರಿವು_ಬಫರ್_CREATE_FAILED( x_ಹರಿವು_ಬಫರ್_Type )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಹರಿವು_ಬಫರ್_CREATE_STATIC_FAILED
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಹರಿವು_ಬಫರ್_CREATE_STATIC_FAILED( xReturn, x_ಹರಿವು_ಬಫರ್_Type )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಹರಿವು_ಬಫರ್_CREATE
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಹರಿವು_ಬಫರ್_CREATE( px_ಹರಿವು_ಬಫರ್, x_ಹರಿವು_ಬಫರ್_Type )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಹರಿವು_ಬಫರ್_DELETE
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಹರಿವು_ಬಫರ್_DELETE( x_ಹರಿವು_ಬಫರ್ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಹರಿವು_ಬಫರ್_RESET
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಹರಿವು_ಬಫರ್_RESET( x_ಹರಿವು_ಬಫರ್ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ಹರಿವು_ಬಫರ್_RESET_FROM_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ಹರಿವು_ಬಫರ್_RESET_FROM_ISR( x_ಹರಿವು_ಬಫರ್ )
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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_Event_ಗುಂಪು_Create_Static
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_Event_ಗುಂಪು_Create_Static( px_Event_ಗುಂಪು_ಬಫರ್ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_Event_ಗುಂಪು_Create_Static
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_Event_ಗುಂಪು_Create_Static( px_Event_ಬಿಟ್ಟುಗಳು )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_Event_ಗುಂಪು_Create
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_Event_ಗುಂಪು_Create()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_Event_ಗುಂಪು_Create
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_Event_ಗುಂಪು_Create( px_Event_ಬಿಟ್ಟುಗಳು )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_Event_ಗುಂಪು_Sync
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_Event_ಗುಂಪು_Sync( x_Event_ಗುಂಪು, ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_ಗಣ, ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Wait_For, x_Ticks_ಗಮ್ಯ_Wait )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_Event_ಗುಂಪು_Sync
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_Event_ಗುಂಪು_Sync( uxReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_Event_ಗುಂಪು_Wait_ಬಿಟ್ಟುಗಳು
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_Event_ಗುಂಪು_Wait_ಬಿಟ್ಟುಗಳು( x_Event_ಗುಂಪು, ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Wait_For, xClearOnExit, x_Wait_For_All_ಬಿಟ್ಟುಗಳು, x_Ticks_ಗಮ್ಯ_Wait )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_Event_ಗುಂಪು_Wait_ಬಿಟ್ಟುಗಳು
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_Event_ಗುಂಪು_Wait_ಬಿಟ್ಟುಗಳು( uxReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_Event_ಗುಂಪು_Clear_ಬಿಟ್ಟುಗಳು
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_Event_ಗುಂಪು_Clear_ಬಿಟ್ಟುಗಳು( x_Event_ಗುಂಪು, ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Clear )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_Event_ಗುಂಪು_Clear_ಬಿಟ್ಟುಗಳು
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_Event_ಗುಂಪು_Clear_ಬಿಟ್ಟುಗಳು( uxReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_Event_ಗುಂಪು_Clear_ಬಿಟ್ಟುಗಳು_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_Event_ಗುಂಪು_Clear_ಬಿಟ್ಟುಗಳು_From_ISR( x_Event_ಗುಂಪು, ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Clear )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_Event_ಗುಂಪು_Clear_ಬಿಟ್ಟುಗಳು_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_Event_ಗುಂಪು_Clear_ಬಿಟ್ಟುಗಳು_From_ISR( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_Event_ಗುಂಪು_Get_ಬಿಟ್ಟುಗಳು_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_Event_ಗುಂಪು_Get_ಬಿಟ್ಟುಗಳು_From_ISR( x_Event_ಗುಂಪು )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_Event_ಗುಂಪು_Get_ಬಿಟ್ಟುಗಳು_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_Event_ಗುಂಪು_Get_ಬಿಟ್ಟುಗಳು_From_ISR( uxReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_Event_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_Event_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು( x_Event_ಗುಂಪು, ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_ಗಣ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_Event_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_Event_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು( ux_Event_ಬಿಟ್ಟುಗಳು )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_v_Event_ಗುಂಪು_Delete
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_v_Event_ಗುಂಪು_Delete( x_Event_ಗುಂಪು )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_v_Event_ಗುಂಪು_Delete
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_v_Event_ಗುಂಪು_Delete()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_Event_ಗುಂಪು_Get_Static_ಬಫರ್
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_Event_ಗುಂಪು_Get_Static_ಬಫರ್( x_Event_ಗುಂಪು, ppx_Event_ಗುಂಪು_ಬಫರ್ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_Event_ಗುಂಪು_Get_Static_ಬಫರ್
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_Event_ಗುಂಪು_Get_Static_ಬಫರ್( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_v_Event_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು_Callback
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_v_Event_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು_Callback( pv_Event_ಗುಂಪು, ul_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_ಗಣ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_v_Event_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು_Callback
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_v_Event_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು_Callback()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_v_Event_ಗುಂಪು_Clear_ಬಿಟ್ಟುಗಳು_Callback
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_v_Event_ಗುಂಪು_Clear_ಬಿಟ್ಟುಗಳು_Callback( pv_Event_ಗುಂಪು, ul_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Clear )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_v_Event_ಗುಂಪು_Clear_ಬಿಟ್ಟುಗಳು_Callback
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_v_Event_ಗುಂಪು_Clear_ಬಿಟ್ಟುಗಳು_Callback()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_Event_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_Event_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು_From_ISR( x_Event_ಗುಂಪು, ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_ಗಣ, px_Higher_ಆದ್ಯತೆ_Task_Woken )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_Event_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_Event_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು_From_ISR( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_ux_Event_ಗುಂಪು_Get_ಸಂಖ್ಯೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_ux_Event_ಗುಂಪು_Get_ಸಂಖ್ಯೆ( x_Event_ಗುಂಪು )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_ux_Event_ಗುಂಪು_Get_ಸಂಖ್ಯೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_ux_Event_ಗುಂಪು_Get_ಸಂಖ್ಯೆ( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_v_Event_ಗುಂಪು_ಗಣ_ಸಂಖ್ಯೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_v_Event_ಗುಂಪು_ಗಣ_ಸಂಖ್ಯೆ( x_Event_ಗುಂಪು, ux_Event_ಗುಂಪು_ಸಂಖ್ಯೆ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_v_Event_ಗುಂಪು_ಗಣ_ಸಂಖ್ಯೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_v_Event_ಗುಂಪು_ಗಣ_ಸಂಖ್ಯೆ()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಸರತಿ_Generic_Reset
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಸರತಿ_Generic_Reset( x_ಸರತಿ, x_New_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಸರತಿ_Generic_Reset
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಸರತಿ_Generic_Reset( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಸರತಿ_Generic_Create_Static
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಸರತಿ_Generic_Create_Static( ux_ಸರತಿ_ಉದ್ದಳತೆ, ux_ಅಂಶ_ಗಾತ್ರ, puc_ಸರತಿ_Storage, px_Static_ಸರತಿ, uc_ಸರತಿ_Type )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಸರತಿ_Generic_Create_Static
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಸರತಿ_Generic_Create_Static( px_New_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಸರತಿ_Generic_Get_Static_Buffers
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಸರತಿ_Generic_Get_Static_Buffers( x_ಸರತಿ, ppuc_ಸರತಿ_Storage, ppx_Static_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಸರತಿ_Generic_Get_Static_Buffers
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಸರತಿ_Generic_Get_Static_Buffers( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಸರತಿ_Generic_Create
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಸರತಿ_Generic_Create( ux_ಸರತಿ_ಉದ್ದಳತೆ, ux_ಅಂಶ_ಗಾತ್ರ, uc_ಸರತಿ_Type )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಸರತಿ_Generic_Create
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಸರತಿ_Generic_Create( px_New_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಸರತಿ_Create_Mutex
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಸರತಿ_Create_Mutex( uc_ಸರತಿ_Type )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಸರತಿ_Create_Mutex
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಸರತಿ_Create_Mutex( x_New_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಸರತಿ_Create_Mutex_Static
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಸರತಿ_Create_Mutex_Static( uc_ಸರತಿ_Type, px_Static_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಸರತಿ_Create_Mutex_Static
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಸರತಿ_Create_Mutex_Static( x_New_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಸರತಿ_Get_Mutex_Holder
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಸರತಿ_Get_Mutex_Holder( xSemaphore )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಸರತಿ_Get_Mutex_Holder
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಸರತಿ_Get_Mutex_Holder( pxReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಸರತಿ_Get_Mutex_Holder_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಸರತಿ_Get_Mutex_Holder_From_ISR( xSemaphore )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಸರತಿ_Get_Mutex_Holder_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಸರತಿ_Get_Mutex_Holder_From_ISR( pxReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಸರತಿ_Give_Mutex_Recursive
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಸರತಿ_Give_Mutex_Recursive( xMutex )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಸರತಿ_Give_Mutex_Recursive
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಸರತಿ_Give_Mutex_Recursive( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಸರತಿ_Take_Mutex_Recursive
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಸರತಿ_Take_Mutex_Recursive( xMutex, x_Ticks_ಗಮ್ಯ_Wait )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಸರತಿ_Take_Mutex_Recursive
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಸರತಿ_Take_Mutex_Recursive( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಸರತಿ_Create_Counting_Semaphore_Static
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಸರತಿ_Create_Counting_Semaphore_Static( ux_Max_ಎಣಿಕೆ, ux_Initial_ಎಣಿಕೆ, px_Static_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಸರತಿ_Create_Counting_Semaphore_Static
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಸರತಿ_Create_Counting_Semaphore_Static( xHandle )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಸರತಿ_Create_Counting_Semaphore
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಸರತಿ_Create_Counting_Semaphore( ux_Max_ಎಣಿಕೆ, ux_Initial_ಎಣಿಕೆ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಸರತಿ_Create_Counting_Semaphore
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಸರತಿ_Create_Counting_Semaphore( xHandle )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಸರತಿ_Generic_Send
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಸರತಿ_Generic_Send( x_ಸರತಿ, pv_ಅಂಶ_ಗಮ್ಯ_ಸರತಿ, x_Ticks_ಗಮ್ಯ_Wait, xCopyPosition )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಸರತಿ_Generic_Send
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಸರತಿ_Generic_Send( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಸರತಿ_Generic_Send_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಸರತಿ_Generic_Send_From_ISR( x_ಸರತಿ, pv_ಅಂಶ_ಗಮ್ಯ_ಸರತಿ, px_Higher_ಆದ್ಯತೆ_Task_Woken, xCopyPosition )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಸರತಿ_Generic_Send_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಸರತಿ_Generic_Send_From_ISR( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಸರತಿ_Give_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಸರತಿ_Give_From_ISR( x_ಸರತಿ, px_Higher_ಆದ್ಯತೆ_Task_Woken )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಸರತಿ_Give_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಸರತಿ_Give_From_ISR( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಸರತಿ_Receive
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಸರತಿ_Receive( x_ಸರತಿ, pv_ಬಫರ್, x_Ticks_ಗಮ್ಯ_Wait )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಸರತಿ_Receive
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಸರತಿ_Receive( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಸರತಿ_Semaphore_Take
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಸರತಿ_Semaphore_Take( x_ಸರತಿ, x_Ticks_ಗಮ್ಯ_Wait )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಸರತಿ_Semaphore_Take
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಸರತಿ_Semaphore_Take( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಸರತಿ_Peek
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಸರತಿ_Peek( x_ಸರತಿ, pv_ಬಫರ್, x_Ticks_ಗಮ್ಯ_Wait )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಸರತಿ_Peek
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಸರತಿ_Peek( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಸರತಿ_Receive_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಸರತಿ_Receive_From_ISR( x_ಸರತಿ, pv_ಬಫರ್, px_Higher_ಆದ್ಯತೆ_Task_Woken )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಸರತಿ_Receive_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಸರತಿ_Receive_From_ISR( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಸರತಿ_Peek_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಸರತಿ_Peek_From_ISR( x_ಸರತಿ, pv_ಬಫರ್ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಸರತಿ_Peek_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಸರತಿ_Peek_From_ISR( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_ux_ಸರತಿ_Messages_ಕಾಯುವಿಕೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_ux_ಸರತಿ_Messages_ಕಾಯುವಿಕೆ( x_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_ux_ಸರತಿ_Messages_ಕಾಯುವಿಕೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_ux_ಸರತಿ_Messages_ಕಾಯುವಿಕೆ( uxReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_ux_ಸರತಿ_Spaces_ಲಭ್ಯ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_ux_ಸರತಿ_Spaces_ಲಭ್ಯ( x_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_ux_ಸರತಿ_Spaces_ಲಭ್ಯ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_ux_ಸರತಿ_Spaces_ಲಭ್ಯ( uxReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_ux_ಸರತಿ_Messages_ಕಾಯುವಿಕೆ_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_ux_ಸರತಿ_Messages_ಕಾಯುವಿಕೆ_From_ISR( x_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_ux_ಸರತಿ_Messages_ಕಾಯುವಿಕೆ_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_ux_ಸರತಿ_Messages_ಕಾಯುವಿಕೆ_From_ISR( uxReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_v_ಸರತಿ_Delete
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_v_ಸರತಿ_Delete( x_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_v_ಸರತಿ_Delete
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_v_ಸರತಿ_Delete()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_ux_ಸರತಿ_Get_ಸರತಿ_ಸಂಖ್ಯೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_ux_ಸರತಿ_Get_ಸರತಿ_ಸಂಖ್ಯೆ( x_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_ux_ಸರತಿ_Get_ಸರತಿ_ಸಂಖ್ಯೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_ux_ಸರತಿ_Get_ಸರತಿ_ಸಂಖ್ಯೆ( ux_ಸರತಿ_ಸಂಖ್ಯೆ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_v_ಸರತಿ_ಗಣ_ಸರತಿ_ಸಂಖ್ಯೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_v_ಸರತಿ_ಗಣ_ಸರತಿ_ಸಂಖ್ಯೆ( x_ಸರತಿ, ux_ಸರತಿ_ಸಂಖ್ಯೆ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_v_ಸರತಿ_ಗಣ_ಸರತಿ_ಸಂಖ್ಯೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_v_ಸರತಿ_ಗಣ_ಸರತಿ_ಸಂಖ್ಯೆ()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_uc_ಸರತಿ_Get_ಸರತಿ_Type
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_uc_ಸರತಿ_Get_ಸರತಿ_Type( x_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_uc_ಸರತಿ_Get_ಸರತಿ_Type
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_uc_ಸರತಿ_Get_ಸರತಿ_Type( uc_ಸರತಿ_Type )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_ux_ಸರತಿ_Get_ಸರತಿ_ಅಂಶ_ಗಾತ್ರ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_ux_ಸರತಿ_Get_ಸರತಿ_ಅಂಶ_ಗಾತ್ರ( x_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_ux_ಸರತಿ_Get_ಸರತಿ_ಅಂಶ_ಗಾತ್ರ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_ux_ಸರತಿ_Get_ಸರತಿ_ಅಂಶ_ಗಾತ್ರ( ux_ಅಂಶ_ಗಾತ್ರ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_ux_ಸರತಿ_Get_ಸರತಿ_ಉದ್ದಳತೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_ux_ಸರತಿ_Get_ಸರತಿ_ಉದ್ದಳತೆ( x_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_ux_ಸರತಿ_Get_ಸರತಿ_ಉದ್ದಳತೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_ux_ಸರತಿ_Get_ಸರತಿ_ಉದ್ದಳತೆ( ux_ಉದ್ದಳತೆ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಸರತಿ_Is_ಸರತಿ_ಖಾಲಿಗಳು_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಸರತಿ_Is_ಸರತಿ_ಖಾಲಿಗಳು_From_ISR( x_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಸರತಿ_Is_ಸರತಿ_ಖಾಲಿಗಳು_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಸರತಿ_Is_ಸರತಿ_ಖಾಲಿಗಳು_From_ISR( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಸರತಿ_Is_ಸರತಿ_ತುಂಬಿದ_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಸರತಿ_Is_ಸರತಿ_ತುಂಬಿದ_From_ISR( x_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಸರತಿ_Is_ಸರತಿ_ತುಂಬಿದ_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಸರತಿ_Is_ಸರತಿ_ತುಂಬಿದ_From_ISR( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಸರತಿ_CRSend
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಸರತಿ_CRSend( x_ಸರತಿ, pv_ಅಂಶ_ಗಮ್ಯ_ಸರತಿ, x_Ticks_ಗಮ್ಯ_Wait )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಸರತಿ_CRSend
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಸರತಿ_CRSend( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಸರತಿ_CRReceive
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಸರತಿ_CRReceive( x_ಸರತಿ, pv_ಬಫರ್, x_Ticks_ಗಮ್ಯ_Wait )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಸರತಿ_CRReceive
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಸರತಿ_CRReceive( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಸರತಿ_CRSend_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಸರತಿ_CRSend_From_ISR( x_ಸರತಿ, pv_ಅಂಶ_ಗಮ್ಯ_ಸರತಿ, xCoRoutinePreviouslyWoken )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಸರತಿ_CRSend_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಸರತಿ_CRSend_From_ISR( xCoRoutinePreviouslyWoken )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಸರತಿ_CRReceive_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಸರತಿ_CRReceive_From_ISR( x_ಸರತಿ, pv_ಬಫರ್, pxCoRoutineWoken )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಸರತಿ_CRReceive_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಸರತಿ_CRReceive_From_ISR( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_v_ಸರತಿ_ಕೂಡಿಸು_ಗಮ್ಯ_Registry
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_v_ಸರತಿ_ಕೂಡಿಸು_ಗಮ್ಯ_Registry( x_ಸರತಿ, pc_ಸರತಿ_Name )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_v_ಸರತಿ_ಕೂಡಿಸು_ಗಮ್ಯ_Registry
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_v_ಸರತಿ_ಕೂಡಿಸು_ಗಮ್ಯ_Registry()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_pc_ಸರತಿ_Get_Name
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_pc_ಸರತಿ_Get_Name( x_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_pc_ಸರತಿ_Get_Name
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_pc_ಸರತಿ_Get_Name( pcReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_v_ಸರತಿ_Unregister_ಸರತಿ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_v_ಸರತಿ_Unregister_ಸರತಿ( x_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_v_ಸರತಿ_Unregister_ಸರತಿ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_v_ಸರತಿ_Unregister_ಸರತಿ()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_v_ಸರತಿ_Wait_For_ಸಂದೇಶ_Restricted
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_v_ಸರತಿ_Wait_For_ಸಂದೇಶ_Restricted( x_ಸರತಿ, x_Ticks_ಗಮ್ಯ_Wait, xWaitIndefinitely )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_v_ಸರತಿ_Wait_For_ಸಂದೇಶ_Restricted
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_v_ಸರತಿ_Wait_For_ಸಂದೇಶ_Restricted()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಸರತಿ_Create_ಗಣ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಸರತಿ_Create_ಗಣ( ux_Event_ಸರತಿ_ಉದ್ದಳತೆ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಸರತಿ_Create_ಗಣ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಸರತಿ_Create_ಗಣ( px_ಸರತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಸರತಿ_ಕೂಡಿಸು_ಗಮ್ಯ_ಗಣ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಸರತಿ_ಕೂಡಿಸು_ಗಮ್ಯ_ಗಣ( x_ಸರತಿ_Or_Semaphore, x_ಸರತಿ_ಗಣ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಸರತಿ_ಕೂಡಿಸು_ಗಮ್ಯ_ಗಣ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಸರತಿ_ಕೂಡಿಸು_ಗಮ್ಯ_ಗಣ( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಸರತಿ_Remove_From_ಗಣ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಸರತಿ_Remove_From_ಗಣ( x_ಸರತಿ_Or_Semaphore, x_ಸರತಿ_ಗಣ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಸರತಿ_Remove_From_ಗಣ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಸರತಿ_Remove_From_ಗಣ( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಸರತಿ_Select_From_ಗಣ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಸರತಿ_Select_From_ಗಣ( x_ಸರತಿ_ಗಣ, x_Ticks_ಗಮ್ಯ_Wait )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಸರತಿ_Select_From_ಗಣ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಸರತಿ_Select_From_ಗಣ( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಸರತಿ_Select_From_ಗಣ_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಸರತಿ_Select_From_ಗಣ_From_ISR( x_ಸರತಿ_ಗಣ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಸರತಿ_Select_From_ಗಣ_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಸರತಿ_Select_From_ಗಣ_From_ISR( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_xTimerCreateTimerTask
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_xTimerCreateTimerTask()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_xTimerCreateTimerTask
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_xTimerCreateTimerTask( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_xTimerCreate
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_xTimerCreate( pcTimerName, x_Timer_Period_ಒಳ_Ticks, xAutoReload, pvTimerID, pxCallbackFunction )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_xTimerCreate
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_xTimerCreate( pxNewTimer )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_xTimerCreateStatic
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_xTimerCreateStatic( pcTimerName, x_Timer_Period_ಒಳ_Ticks, xAutoReload, pvTimerID, pxCallbackFunction, px_Timer_ಬಫರ್ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_xTimerCreateStatic
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_xTimerCreateStatic( pxNewTimer )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_xTimerGenericCommandFromTask
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_xTimerGenericCommandFromTask( xTimer, xCommandID, x_Optional_ಮೌಲ್ಯ, px_Higher_ಆದ್ಯತೆ_Task_Woken, x_Ticks_ಗಮ್ಯ_Wait )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_xTimerGenericCommandFromTask
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_xTimerGenericCommandFromTask( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_xTimerGenericCommandFromISR
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_xTimerGenericCommandFromISR( xTimer, xCommandID, x_Optional_ಮೌಲ್ಯ, px_Higher_ಆದ್ಯತೆ_Task_Woken, x_Ticks_ಗಮ್ಯ_Wait )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_xTimerGenericCommandFromISR
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_xTimerGenericCommandFromISR( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_xTimerGetTimerDaemonTaskHandle
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_xTimerGetTimerDaemonTaskHandle()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_xTimerGetTimerDaemonTaskHandle
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_xTimerGetTimerDaemonTaskHandle( xTimerTaskHandle )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_xTimerGetPeriod
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_xTimerGetPeriod( xTimer )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_xTimerGetPeriod
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_xTimerGetPeriod( x_Timer_Period_ಒಳ_Ticks )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_v_Timer_ಗಣ_Reload_ಬಹುಲಕ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_v_Timer_ಗಣ_Reload_ಬಹುಲಕ( xTimer, xAutoReload )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_v_Timer_ಗಣ_Reload_ಬಹುಲಕ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_v_Timer_ಗಣ_Reload_ಬಹುಲಕ()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_Timer_Get_Reload_ಬಹುಲಕ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_Timer_Get_Reload_ಬಹುಲಕ( xTimer )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_Timer_Get_Reload_ಬಹುಲಕ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_Timer_Get_Reload_ಬಹುಲಕ( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_ux_Timer_Get_Reload_ಬಹುಲಕ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_ux_Timer_Get_Reload_ಬಹುಲಕ( xTimer )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_ux_Timer_Get_Reload_ಬಹುಲಕ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_ux_Timer_Get_Reload_ಬಹುಲಕ( uxReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_Timer_Get_Expiry_ಸಮಯ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_Timer_Get_Expiry_ಸಮಯ( xTimer )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_Timer_Get_Expiry_ಸಮಯ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_Timer_Get_Expiry_ಸಮಯ( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_Timer_Get_Static_ಬಫರ್
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_Timer_Get_Static_ಬಫರ್( xTimer, ppx_Timer_ಬಫರ್ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_Timer_Get_Static_ಬಫರ್
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_Timer_Get_Static_ಬಫರ್( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_pcTimerGetName
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_pcTimerGetName( xTimer )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_pcTimerGetName
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_pcTimerGetName( pcTimerName )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_Timer_Is_Timer_ಸಕ್ರಿಯ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_Timer_Is_Timer_ಸಕ್ರಿಯ( xTimer )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_Timer_Is_Timer_ಸಕ್ರಿಯ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_Timer_Is_Timer_ಸಕ್ರಿಯ( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_pvTimerGetTimerID
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_pvTimerGetTimerID( xTimer )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_pvTimerGetTimerID
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_pvTimerGetTimerID( pvReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_v_Timer_ಗಣ_Timer_ID
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_v_Timer_ಗಣ_Timer_ID( xTimer, pvNewID )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_v_Timer_ಗಣ_Timer_ID
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_v_Timer_ಗಣ_Timer_ID()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_xTimerPendFunctionCallFromISR
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_xTimerPendFunctionCallFromISR( x_Function_ಗಮ್ಯ_Pend, pvParameter1, ulParameter2, px_Higher_ಆದ್ಯತೆ_Task_Woken )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_xTimerPendFunctionCallFromISR
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_xTimerPendFunctionCallFromISR( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_xTimerPendFunctionCall
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_xTimerPendFunctionCall( x_Function_ಗಮ್ಯ_Pend, pvParameter1, ulParameter2, x_Ticks_ಗಮ್ಯ_Wait )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_xTimerPendFunctionCall
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_xTimerPendFunctionCall( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_ux_Timer_Get_Timer_ಸಂಖ್ಯೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_ux_Timer_Get_Timer_ಸಂಖ್ಯೆ( xTimer )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_ux_Timer_Get_Timer_ಸಂಖ್ಯೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_ux_Timer_Get_Timer_ಸಂಖ್ಯೆ( ux_Timer_ಸಂಖ್ಯೆ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_v_Timer_ಗಣ_Timer_ಸಂಖ್ಯೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_v_Timer_ಗಣ_Timer_ಸಂಖ್ಯೆ( xTimer, ux_Timer_ಸಂಖ್ಯೆ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_v_Timer_ಗಣ_Timer_ಸಂಖ್ಯೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_v_Timer_ಗಣ_Timer_ಸಂಖ್ಯೆ()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_xTaskCreateStatic
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_xTaskCreateStatic( px_Task_ಸಂಕೇತ, pcName, ux_ರಾಶಿ_ಆಳ, pvParameters, ux_ಆದ್ಯತೆ, pux_ರಾಶಿ_ಬಫರ್, px_Task_ಬಫರ್ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_xTaskCreateStatic
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_xTaskCreateStatic( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_Task_Create_Static_Affinity_ಗಣ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_Task_Create_Static_Affinity_ಗಣ( px_Task_ಸಂಕೇತ, pcName, ux_ರಾಶಿ_ಆಳ, pvParameters, ux_ಆದ್ಯತೆ, pux_ರಾಶಿ_ಬಫರ್, px_Task_ಬಫರ್, ux_Core_Affinity_ಮುಖವಾಡ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_Task_Create_Static_Affinity_ಗಣ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_Task_Create_Static_Affinity_ಗಣ( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_xTaskCreateRestrictedStatic
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_xTaskCreateRestrictedStatic( pxTaskDefinition, pxCreatedTask )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_xTaskCreateRestrictedStatic
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_xTaskCreateRestrictedStatic( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_Task_Create_Restricted_Static_Affinity_ಗಣ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_Task_Create_Restricted_Static_Affinity_ಗಣ( pxTaskDefinition, ux_Core_Affinity_ಮುಖವಾಡ, pxCreatedTask )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_Task_Create_Restricted_Static_Affinity_ಗಣ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_Task_Create_Restricted_Static_Affinity_ಗಣ( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_xTaskCreateRestricted
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_xTaskCreateRestricted( pxTaskDefinition, pxCreatedTask )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_xTaskCreateRestricted
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_xTaskCreateRestricted( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_Task_Create_Restricted_Affinity_ಗಣ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_Task_Create_Restricted_Affinity_ಗಣ( pxTaskDefinition, ux_Core_Affinity_ಮುಖವಾಡ, pxCreatedTask )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_Task_Create_Restricted_Affinity_ಗಣ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_Task_Create_Restricted_Affinity_ಗಣ( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_xTaskCreate
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_xTaskCreate( px_Task_ಸಂಕೇತ, pcName, ux_ರಾಶಿ_ಆಳ, pvParameters, ux_ಆದ್ಯತೆ, pxCreatedTask )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_xTaskCreate
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_xTaskCreate( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_Task_Create_Affinity_ಗಣ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_Task_Create_Affinity_ಗಣ( px_Task_ಸಂಕೇತ, pcName, ux_ರಾಶಿ_ಆಳ, pvParameters, ux_ಆದ್ಯತೆ, ux_Core_Affinity_ಮುಖವಾಡ, pxCreatedTask )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_Task_Create_Affinity_ಗಣ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_Task_Create_Affinity_ಗಣ( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_vTaskDelete
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_vTaskDelete( x_Task_ಗಮ್ಯ_Delete )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_vTaskDelete
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_vTaskDelete()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_xTaskDelayUntil
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_xTaskDelayUntil( px_ಹಿಂದಿನ_Wake_ಸಮಯ, x_ಸಮಯ_Increment )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_xTaskDelayUntil
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_xTaskDelayUntil( xShouldDelay )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_vTaskDelay
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_vTaskDelay( x_Ticks_ಗಮ್ಯ_Delay )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_vTaskDelay
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_vTaskDelay()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_eTaskGetState
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_eTaskGetState( xTask )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_eTaskGetState
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_eTaskGetState( eReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_ux_Task_ಆದ್ಯತೆ_Get
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_ux_Task_ಆದ್ಯತೆ_Get( xTask )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_ux_Task_ಆದ್ಯತೆ_Get
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_ux_Task_ಆದ್ಯತೆ_Get( uxReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_ux_Task_ಆದ್ಯತೆ_Get_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_ux_Task_ಆದ್ಯತೆ_Get_From_ISR( xTask )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_ux_Task_ಆದ್ಯತೆ_Get_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_ux_Task_ಆದ್ಯತೆ_Get_From_ISR( uxReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_ux_Task_ತಳ_ಆದ್ಯತೆ_Get
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_ux_Task_ತಳ_ಆದ್ಯತೆ_Get( xTask )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_ux_Task_ತಳ_ಆದ್ಯತೆ_Get
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_ux_Task_ತಳ_ಆದ್ಯತೆ_Get( uxReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_ux_Task_ತಳ_ಆದ್ಯತೆ_Get_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_ux_Task_ತಳ_ಆದ್ಯತೆ_Get_From_ISR( xTask )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_ux_Task_ತಳ_ಆದ್ಯತೆ_Get_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_ux_Task_ತಳ_ಆದ್ಯತೆ_Get_From_ISR( uxReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_v_Task_ಆದ್ಯತೆ_ಗಣ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_v_Task_ಆದ್ಯತೆ_ಗಣ( xTask, ux_New_ಆದ್ಯತೆ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_v_Task_ಆದ್ಯತೆ_ಗಣ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_v_Task_ಆದ್ಯತೆ_ಗಣ()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_v_Task_Core_Affinity_ಗಣ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_v_Task_Core_Affinity_ಗಣ( xTask, ux_Core_Affinity_ಮುಖವಾಡ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_v_Task_Core_Affinity_ಗಣ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_v_Task_Core_Affinity_ಗಣ()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_vTaskCoreAffinityGet
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_vTaskCoreAffinityGet( xTask )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_vTaskCoreAffinityGet
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_vTaskCoreAffinityGet( ux_Core_Affinity_ಮುಖವಾಡ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_vTaskPreemptionDisable
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_vTaskPreemptionDisable( xTask )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_vTaskPreemptionDisable
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_vTaskPreemptionDisable()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_vTaskPreemptionEnable
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_vTaskPreemptionEnable( xTask )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_vTaskPreemptionEnable
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_vTaskPreemptionEnable()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_vTaskSuspend
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_vTaskSuspend( x_Task_ಗಮ್ಯ_Suspend )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_vTaskSuspend
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_vTaskSuspend()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_vTaskResume
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_vTaskResume( x_Task_ಗಮ್ಯ_Resume )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_vTaskResume
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_vTaskResume()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_xTaskResumeFromISR
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_xTaskResumeFromISR( x_Task_ಗಮ್ಯ_Resume )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_xTaskResumeFromISR
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_xTaskResumeFromISR( xYieldRequired )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_v_Task_ಆರಂಭ_Scheduler
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_v_Task_ಆರಂಭ_Scheduler()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_v_Task_ಆರಂಭ_Scheduler
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_v_Task_ಆರಂಭ_Scheduler()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_v_Task_ಕೊನೆ_Scheduler
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_v_Task_ಕೊನೆ_Scheduler()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_v_Task_ಕೊನೆ_Scheduler
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_v_Task_ಕೊನೆ_Scheduler()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_vTaskSuspendAll
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_vTaskSuspendAll()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_vTaskSuspendAll
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_vTaskSuspendAll()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_xTaskResumeAll
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_xTaskResumeAll()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_xTaskResumeAll
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_xTaskResumeAll( xAlreadyYielded )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_Task_Get_Tick_ಎಣಿಕೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_Task_Get_Tick_ಎಣಿಕೆ()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_Task_Get_Tick_ಎಣಿಕೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_Task_Get_Tick_ಎಣಿಕೆ( xTicks )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_Task_Get_Tick_ಎಣಿಕೆ_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_Task_Get_Tick_ಎಣಿಕೆ_From_ISR()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_Task_Get_Tick_ಎಣಿಕೆ_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_Task_Get_Tick_ಎಣಿಕೆ_From_ISR( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_ux_Task_Get_ಸಂಖ್ಯೆ_ದ_Tasks
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_ux_Task_Get_ಸಂಖ್ಯೆ_ದ_Tasks()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_ux_Task_Get_ಸಂಖ್ಯೆ_ದ_Tasks
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_ux_Task_Get_ಸಂಖ್ಯೆ_ದ_Tasks( ux_Current_ಸಂಖ್ಯೆ_ದ_Tasks )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_pcTaskGetName
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_pcTaskGetName( x_Task_ಗಮ್ಯ_Query )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_pcTaskGetName
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_pcTaskGetName( pcTaskName )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_xTaskGetHandle
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_xTaskGetHandle( pc_Name_ಗಮ್ಯ_Query )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_xTaskGetHandle
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_xTaskGetHandle( pxTCB )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_xTaskGetStaticBuffers
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_xTaskGetStaticBuffers( xTask, ppux_ರಾಶಿ_ಬಫರ್, ppx_Task_ಬಫರ್ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_xTaskGetStaticBuffers
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_xTaskGetStaticBuffers( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_uxTaskGetSystemState
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_uxTaskGetSystemState( px_Task_ಸ್ಥಿತಿ_Array, ux_Array_ಗಾತ್ರ, pul_ಒಟ್ಟು_ಸತತ_ಸಮಯ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_uxTaskGetSystemState
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_uxTaskGetSystemState( uxTask )
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ ( config_ಸಂಖ್ಯೆ_ದ_CORES == 1 )
    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_xTaskGetIdleTaskHandle
        #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_xTaskGetIdleTaskHandle()
    #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ ( config_ಸಂಖ್ಯೆ_ದ_CORES == 1 )
    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_xTaskGetIdleTaskHandle
        #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_xTaskGetIdleTaskHandle( xIdleTaskHandle )
    #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_xTaskGetIdleTaskHandleForCore
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_xTaskGetIdleTaskHandleForCore( xCoreID )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_xTaskGetIdleTaskHandleForCore
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_xTaskGetIdleTaskHandleForCore( xIdleTaskHandle )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_v_Task_ಹೆಜ್ಜೆ_Tick
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_v_Task_ಹೆಜ್ಜೆ_Tick( x_Ticks_ಗಮ್ಯ_Jump )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_v_Task_ಹೆಜ್ಜೆ_Tick
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_v_Task_ಹೆಜ್ಜೆ_Tick()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_Task_Catch_ಮೇಲೆ_Ticks
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_Task_Catch_ಮೇಲೆ_Ticks( x_Ticks_ಗಮ್ಯ_Catch_ಮೇಲೆ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_Task_Catch_ಮೇಲೆ_Ticks
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_Task_Catch_ಮೇಲೆ_Ticks( xYieldOccurred )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_xTaskAbortDelay
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_xTaskAbortDelay( xTask )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_xTaskAbortDelay
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_xTaskAbortDelay( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_xTaskIncrementTick
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_xTaskIncrementTick()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_xTaskIncrementTick
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_xTaskIncrementTick( xSwitchRequired )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_v_Task_ಗಣ_Application_Task_ಟ್ಯಾಗ್
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_v_Task_ಗಣ_Application_Task_ಟ್ಯಾಗ್( xTask, pxHookFunction )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_v_Task_ಗಣ_Application_Task_ಟ್ಯಾಗ್
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_v_Task_ಗಣ_Application_Task_ಟ್ಯಾಗ್()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_Task_Get_Application_Task_ಟ್ಯಾಗ್
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_Task_Get_Application_Task_ಟ್ಯಾಗ್( xTask )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_Task_Get_Application_Task_ಟ್ಯಾಗ್
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_Task_Get_Application_Task_ಟ್ಯಾಗ್( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_Task_Get_Application_Task_ಟ್ಯಾಗ್_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_Task_Get_Application_Task_ಟ್ಯಾಗ್_From_ISR( xTask )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_Task_Get_Application_Task_ಟ್ಯಾಗ್_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_Task_Get_Application_Task_ಟ್ಯಾಗ್_From_ISR( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_xTaskCallApplicationTaskHook
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_xTaskCallApplicationTaskHook( xTask, pvParameter )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_xTaskCallApplicationTaskHook
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_xTaskCallApplicationTaskHook( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_vTaskSwitchContext
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_vTaskSwitchContext()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_vTaskSwitchContext
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_vTaskSwitchContext()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_v_Task_ಇರಿಸು_On_Event_List
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_v_Task_ಇರಿಸು_On_Event_List( pxEventList, x_Ticks_ಗಮ್ಯ_Wait )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_v_Task_ಇರಿಸು_On_Event_List
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_v_Task_ಇರಿಸು_On_Event_List()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_v_Task_ಇರಿಸು_On_Unordered_Event_List
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_v_Task_ಇರಿಸು_On_Unordered_Event_List( pxEventList, x_ಅಂಶ_ಮೌಲ್ಯ, x_Ticks_ಗಮ್ಯ_Wait )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_v_Task_ಇರಿಸು_On_Unordered_Event_List
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_v_Task_ಇರಿಸು_On_Unordered_Event_List()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_v_Task_ಇರಿಸು_On_Event_List_Restricted
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_v_Task_ಇರಿಸು_On_Event_List_Restricted( pxEventList, x_Ticks_ಗಮ್ಯ_Wait, xWaitIndefinitely )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_v_Task_ಇರಿಸು_On_Event_List_Restricted
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_v_Task_ಇರಿಸು_On_Event_List_Restricted()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_xTaskRemoveFromEventList
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_xTaskRemoveFromEventList( pxEventList )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_xTaskRemoveFromEventList
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_xTaskRemoveFromEventList( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_vTaskRemoveFromUnorderedEventList
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_vTaskRemoveFromUnorderedEventList( px_Event_List_ಅಂಶ, x_ಅಂಶ_ಮೌಲ್ಯ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_vTaskRemoveFromUnorderedEventList
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_vTaskRemoveFromUnorderedEventList()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_v_Task_ಗಣ_ಸಮಯ_ಹೊರಹೋಗು_State
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_v_Task_ಗಣ_ಸಮಯ_ಹೊರಹೋಗು_State( px_ಸಮಯ_ಹೊರಹೋಗು )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_v_Task_ಗಣ_ಸಮಯ_ಹೊರಹೋಗು_State
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_v_Task_ಗಣ_ಸಮಯ_ಹೊರಹೋಗು_State()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_v_Task_ಆಂತರಿಕ_ಗಣ_ಸಮಯ_ಹೊರಹೋಗು_State
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_v_Task_ಆಂತರಿಕ_ಗಣ_ಸಮಯ_ಹೊರಹೋಗು_State( px_ಸಮಯ_ಹೊರಹೋಗು )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_v_Task_ಆಂತರಿಕ_ಗಣ_ಸಮಯ_ಹೊರಹೋಗು_State
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_v_Task_ಆಂತರಿಕ_ಗಣ_ಸಮಯ_ಹೊರಹೋಗು_State()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_Task_Check_For_ಸಮಯ_ಹೊರಹೋಗು
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_Task_Check_For_ಸಮಯ_ಹೊರಹೋಗು( px_ಸಮಯ_ಹೊರಹೋಗು, px_Ticks_ಗಮ್ಯ_Wait )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_Task_Check_For_ಸಮಯ_ಹೊರಹೋಗು
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_Task_Check_For_ಸಮಯ_ಹೊರಹೋಗು( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_vTaskMissedYield
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_vTaskMissedYield()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_vTaskMissedYield
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_vTaskMissedYield()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_ux_Task_Get_Task_ಸಂಖ್ಯೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_ux_Task_Get_Task_ಸಂಖ್ಯೆ( xTask )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_ux_Task_Get_Task_ಸಂಖ್ಯೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_ux_Task_Get_Task_ಸಂಖ್ಯೆ( uxReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_v_Task_ಗಣ_Task_ಸಂಖ್ಯೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_v_Task_ಗಣ_Task_ಸಂಖ್ಯೆ( xTask, uxHandle )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_v_Task_ಗಣ_Task_ಸಂಖ್ಯೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_v_Task_ಗಣ_Task_ಸಂಖ್ಯೆ()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_e_Task_Confirm_Sleep_ಬಹುಲಕ_ಸ್ಥಿತಿ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_e_Task_Confirm_Sleep_ಬಹುಲಕ_ಸ್ಥಿತಿ()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_e_Task_Confirm_Sleep_ಬಹುಲಕ_ಸ್ಥಿತಿ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_e_Task_Confirm_Sleep_ಬಹುಲಕ_ಸ್ಥಿತಿ( eReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_v_Task_ಗಣ_Thread_Local_Storage_ಸೂಚಕ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_v_Task_ಗಣ_Thread_Local_Storage_ಸೂಚಕ( x_Task_ಗಮ್ಯ_ಗಣ, x_ಸೂಚ್ಯಂಕ, pv_ಮೌಲ್ಯ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_v_Task_ಗಣ_Thread_Local_Storage_ಸೂಚಕ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_v_Task_ಗಣ_Thread_Local_Storage_ಸೂಚಕ()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_pv_Task_Get_Thread_Local_Storage_ಸೂಚಕ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_pv_Task_Get_Thread_Local_Storage_ಸೂಚಕ( x_Task_ಗಮ್ಯ_Query, x_ಸೂಚ್ಯಂಕ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_pv_Task_Get_Thread_Local_Storage_ಸೂಚಕ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_pv_Task_Get_Thread_Local_Storage_ಸೂಚಕ( pvReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_vTaskAllocateMPURegions
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_vTaskAllocateMPURegions( x_Task_ಗಮ್ಯ_Modify, pxRegions )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_vTaskAllocateMPURegions
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_vTaskAllocateMPURegions()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_vTaskGetInfo
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_vTaskGetInfo( xTask, px_Task_ಸ್ಥಿತಿ, x_Get_Free_ರಾಶಿ_Space, eState )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_vTaskGetInfo
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_vTaskGetInfo()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_ux_Task_Get_ರಾಶಿ_ಮೇಲಿನ_Water_Mark2
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_ux_Task_Get_ರಾಶಿ_ಮೇಲಿನ_Water_Mark2( xTask )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_ux_Task_Get_ರಾಶಿ_ಮೇಲಿನ_Water_Mark2
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_ux_Task_Get_ರಾಶಿ_ಮೇಲಿನ_Water_Mark2( uxReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_ux_Task_Get_ರಾಶಿ_ಮೇಲಿನ_Water_Mark
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_ux_Task_Get_ರಾಶಿ_ಮೇಲಿನ_Water_Mark( xTask )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_ux_Task_Get_ರಾಶಿ_ಮೇಲಿನ_Water_Mark
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_ux_Task_Get_ರಾಶಿ_ಮೇಲಿನ_Water_Mark( uxReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_xTaskGetCurrentTaskHandle
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_xTaskGetCurrentTaskHandle()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_xTaskGetCurrentTaskHandle
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_xTaskGetCurrentTaskHandle( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_xTaskGetCurrentTaskHandleForCore
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_xTaskGetCurrentTaskHandleForCore( xCoreID )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_xTaskGetCurrentTaskHandleForCore
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_xTaskGetCurrentTaskHandleForCore( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_xTaskGetSchedulerState
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_xTaskGetSchedulerState()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_xTaskGetSchedulerState
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_xTaskGetSchedulerState( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_Task_ಆದ್ಯತೆ_Inherit
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_Task_ಆದ್ಯತೆ_Inherit( pxMutexHolder )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_Task_ಆದ್ಯತೆ_Inherit
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_Task_ಆದ್ಯತೆ_Inherit( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_Task_ಆದ್ಯತೆ_Disinherit
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_Task_ಆದ್ಯತೆ_Disinherit( pxMutexHolder )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_Task_ಆದ್ಯತೆ_Disinherit
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_Task_ಆದ್ಯತೆ_Disinherit( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_v_Task_ಆದ್ಯತೆ_Disinherit_After_Timeout
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_v_Task_ಆದ್ಯತೆ_Disinherit_After_Timeout( pxMutexHolder, ux_Highest_ಆದ್ಯತೆ_ಕಾಯುವಿಕೆ_Task )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_v_Task_ಆದ್ಯತೆ_Disinherit_After_Timeout
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_v_Task_ಆದ್ಯತೆ_Disinherit_After_Timeout()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_vTaskYieldWithinAPI
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_vTaskYieldWithinAPI()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_vTaskYieldWithinAPI
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_vTaskYieldWithinAPI()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_vTaskEnterCritical
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_vTaskEnterCritical()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_vTaskEnterCritical
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_vTaskEnterCritical()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_vTaskEnterCriticalFromISR
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_vTaskEnterCriticalFromISR()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_vTaskEnterCriticalFromISR
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_vTaskEnterCriticalFromISR( ux_Saved_ಅಡಚಣೆ_ಸ್ಥಿತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_vTaskExitCritical
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_vTaskExitCritical()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_vTaskExitCritical
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_vTaskExitCritical()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_vTaskExitCriticalFromISR
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_vTaskExitCriticalFromISR( ux_Saved_ಅಡಚಣೆ_ಸ್ಥಿತಿ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_vTaskExitCriticalFromISR
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_vTaskExitCriticalFromISR()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_vTaskListTasks
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_vTaskListTasks( pc_Write_ಬಫರ್, ux_ಬಫರ್_ಉದ್ದಳತೆ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_vTaskListTasks
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_vTaskListTasks()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_v_Task_Get_ಸತತ_ಸಮಯ_Statistics
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_v_Task_Get_ಸತತ_ಸಮಯ_Statistics( pc_Write_ಬಫರ್, ux_ಬಫರ್_ಉದ್ದಳತೆ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_v_Task_Get_ಸತತ_ಸಮಯ_Statistics
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_v_Task_Get_ಸತತ_ಸಮಯ_Statistics()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_ux_Task_Reset_Event_ಅಂಶ_ಮೌಲ್ಯ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_ux_Task_Reset_Event_ಅಂಶ_ಮೌಲ್ಯ()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_ux_Task_Reset_Event_ಅಂಶ_ಮೌಲ್ಯ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_ux_Task_Reset_Event_ಅಂಶ_ಮೌಲ್ಯ( uxReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_pv_Task_Increment_Mutex_Held_ಎಣಿಕೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_pv_Task_Increment_Mutex_Held_ಎಣಿಕೆ()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_pv_Task_Increment_Mutex_Held_ಎಣಿಕೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_pv_Task_Increment_Mutex_Held_ಎಣಿಕೆ( pxTCB )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_ulTaskGenericNotifyTake
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_ulTaskGenericNotifyTake( ux_ಸೂಚ್ಯಂಕ_ಗಮ್ಯ_Wait_On, x_Clear_ಎಣಿಕೆ_On_Exit, x_Ticks_ಗಮ್ಯ_Wait )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_ulTaskGenericNotifyTake
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_ulTaskGenericNotifyTake( ulReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_xTaskGenericNotifyWait
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_xTaskGenericNotifyWait( ux_ಸೂಚ್ಯಂಕ_ಗಮ್ಯ_Wait_On, ul_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Clear_On_ನಮೂದು, ul_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Clear_On_Exit, pul_Notification_ಮೌಲ್ಯ, x_Ticks_ಗಮ್ಯ_Wait )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_xTaskGenericNotifyWait
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_xTaskGenericNotifyWait( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_xTaskGenericNotify
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_xTaskGenericNotify( x_Task_ಗಮ್ಯ_Notify, ux_ಸೂಚ್ಯಂಕ_ಗಮ್ಯ_Notify, ul_ಮೌಲ್ಯ, eAction, pul_ಹಿಂದಿನ_Notification_ಮೌಲ್ಯ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_xTaskGenericNotify
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_xTaskGenericNotify( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_xTaskGenericNotifyFromISR
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_xTaskGenericNotifyFromISR( x_Task_ಗಮ್ಯ_Notify, ux_ಸೂಚ್ಯಂಕ_ಗಮ್ಯ_Notify, ul_ಮೌಲ್ಯ, eAction, pul_ಹಿಂದಿನ_Notification_ಮೌಲ್ಯ, px_Higher_ಆದ್ಯತೆ_Task_Woken )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_xTaskGenericNotifyFromISR
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_xTaskGenericNotifyFromISR( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_vTaskGenericNotifyGiveFromISR
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_vTaskGenericNotifyGiveFromISR( x_Task_ಗಮ್ಯ_Notify, ux_ಸೂಚ್ಯಂಕ_ಗಮ್ಯ_Notify, px_Higher_ಆದ್ಯತೆ_Task_Woken )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_vTaskGenericNotifyGiveFromISR
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_vTaskGenericNotifyGiveFromISR()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_xTaskGenericNotifyStateClear
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_xTaskGenericNotifyStateClear( xTask, ux_ಸೂಚ್ಯಂಕ_ಗಮ್ಯ_Clear )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_xTaskGenericNotifyStateClear
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_xTaskGenericNotifyStateClear( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_ul_Task_Generic_Notify_ಮೌಲ್ಯ_Clear
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_ul_Task_Generic_Notify_ಮೌಲ್ಯ_Clear( xTask, ux_ಸೂಚ್ಯಂಕ_ಗಮ್ಯ_Clear, ul_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Clear )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_ul_Task_Generic_Notify_ಮೌಲ್ಯ_Clear
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_ul_Task_Generic_Notify_ಮೌಲ್ಯ_Clear( ulReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_ul_Task_Get_ಸತತ_ಸಮಯ_Counter
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_ul_Task_Get_ಸತತ_ಸಮಯ_Counter( xTask )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_ul_Task_Get_ಸತತ_ಸಮಯ_Counter
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_ul_Task_Get_ಸತತ_ಸಮಯ_Counter( ul_ಸತತ_ಸಮಯ_Counter )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_ul_Task_Get_ಸತತ_ಸಮಯ_Percent
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_ul_Task_Get_ಸತತ_ಸಮಯ_Percent( xTask )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_ul_Task_Get_ಸತತ_ಸಮಯ_Percent
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_ul_Task_Get_ಸತತ_ಸಮಯ_Percent( ulReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_ul_Task_Get_Idle_ಸತತ_ಸಮಯ_Counter
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_ul_Task_Get_Idle_ಸತತ_ಸಮಯ_Counter()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_ul_Task_Get_Idle_ಸತತ_ಸಮಯ_Counter
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_ul_Task_Get_Idle_ಸತತ_ಸಮಯ_Counter( ulReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_ul_Task_Get_Idle_ಸತತ_ಸಮಯ_Percent
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_ul_Task_Get_Idle_ಸತತ_ಸಮಯ_Percent()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_ul_Task_Get_Idle_ಸತತ_ಸಮಯ_Percent
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_ul_Task_Get_Idle_ಸತತ_ಸಮಯ_Percent( ulReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_xTaskGetMPUSettings
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_xTaskGetMPUSettings( xTask )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_xTaskGetMPUSettings
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_xTaskGetMPUSettings( xMPUSettings )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಹರಿವು_ಬಫರ್_Generic_Create
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಹರಿವು_ಬಫರ್_Generic_Create( x_ಬಫರ್_ಗಾತ್ರ_ಬೈಟುಗಳು, x_Trigger_Level_ಬೈಟುಗಳು, x_ಹರಿವು_ಬಫರ್_Type, pxSendCompletedCallback, pxReceiveCompletedCallback )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಹರಿವು_ಬಫರ್_Generic_Create
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಹರಿವು_ಬಫರ್_Generic_Create( pv_Allocated_ಸ್ಮರಣೆ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಹರಿವು_ಬಫರ್_Generic_Create_Static
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಹರಿವು_ಬಫರ್_Generic_Create_Static( x_ಬಫರ್_ಗಾತ್ರ_ಬೈಟುಗಳು, x_Trigger_Level_ಬೈಟುಗಳು, x_ಹರಿವು_ಬಫರ್_Type, puc_ಹರಿವು_ಬಫರ್_Storage_Area, px_Static_ಹರಿವು_ಬಫರ್, pxSendCompletedCallback, pxReceiveCompletedCallback )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಹರಿವು_ಬಫರ್_Generic_Create_Static
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಹರಿವು_ಬಫರ್_Generic_Create_Static( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಹರಿವು_ಬಫರ್_Get_Static_Buffers
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಹರಿವು_ಬಫರ್_Get_Static_Buffers( x_ಹರಿವು_ಬಫರ್, ppuc_ಹರಿವು_ಬಫರ್_Storage_Area, ppx_Static_ಹರಿವು_ಬಫರ್ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಹರಿವು_ಬಫರ್_Get_Static_Buffers
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಹರಿವು_ಬಫರ್_Get_Static_Buffers( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_v_ಹರಿವು_ಬಫರ್_Delete
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_v_ಹರಿವು_ಬಫರ್_Delete( x_ಹರಿವು_ಬಫರ್ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_v_ಹರಿವು_ಬಫರ್_Delete
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_v_ಹರಿವು_ಬಫರ್_Delete()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಹರಿವು_ಬಫರ್_Reset
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಹರಿವು_ಬಫರ್_Reset( x_ಹರಿವು_ಬಫರ್ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಹರಿವು_ಬಫರ್_Reset
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಹರಿವು_ಬಫರ್_Reset( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಹರಿವು_ಬಫರ್_Reset_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಹರಿವು_ಬಫರ್_Reset_From_ISR( x_ಹರಿವು_ಬಫರ್ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಹರಿವು_ಬಫರ್_Reset_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಹರಿವು_ಬಫರ್_Reset_From_ISR( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಹರಿವು_ಬಫರ್_ಗಣ_Trigger_Level
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಹರಿವು_ಬಫರ್_ಗಣ_Trigger_Level( x_ಹರಿವು_ಬಫರ್, xTriggerLevel )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಹರಿವು_ಬಫರ್_ಗಣ_Trigger_Level
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಹರಿವು_ಬಫರ್_ಗಣ_Trigger_Level( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಹರಿವು_ಬಫರ್_Spaces_ಲಭ್ಯ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಹರಿವು_ಬಫರ್_Spaces_ಲಭ್ಯ( x_ಹರಿವು_ಬಫರ್ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಹರಿವು_ಬಫರ್_Spaces_ಲಭ್ಯ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಹರಿವು_ಬಫರ್_Spaces_ಲಭ್ಯ( xSpace )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಹರಿವು_ಬಫರ್_ಬೈಟುಗಳು_ಲಭ್ಯ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಹರಿವು_ಬಫರ್_ಬೈಟುಗಳು_ಲಭ್ಯ( x_ಹರಿವು_ಬಫರ್ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಹರಿವು_ಬಫರ್_ಬೈಟುಗಳು_ಲಭ್ಯ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಹರಿವು_ಬಫರ್_ಬೈಟುಗಳು_ಲಭ್ಯ( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಹರಿವು_ಬಫರ್_Send
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಹರಿವು_ಬಫರ್_Send( x_ಹರಿವು_ಬಫರ್, pv_Tx_ದತ್ತಾಂಶ, x_ದತ್ತಾಂಶ_ಉದ್ದಳತೆ_ಬೈಟುಗಳು, x_Ticks_ಗಮ್ಯ_Wait )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಹರಿವು_ಬಫರ್_Send
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಹರಿವು_ಬಫರ್_Send( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಹರಿವು_ಬಫರ್_Send_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಹರಿವು_ಬಫರ್_Send_From_ISR( x_ಹರಿವು_ಬಫರ್, pv_Tx_ದತ್ತಾಂಶ, x_ದತ್ತಾಂಶ_ಉದ್ದಳತೆ_ಬೈಟುಗಳು, px_Higher_ಆದ್ಯತೆ_Task_Woken )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಹರಿವು_ಬಫರ್_Send_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಹರಿವು_ಬಫರ್_Send_From_ISR( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಹರಿವು_ಬಫರ್_Receive
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಹರಿವು_ಬಫರ್_Receive( x_ಹರಿವು_ಬಫರ್, pv_Rx_ದತ್ತಾಂಶ, x_ಬಫರ್_ಉದ್ದಳತೆ_ಬೈಟುಗಳು, x_Ticks_ಗಮ್ಯ_Wait )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಹರಿವು_ಬಫರ್_Receive
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಹರಿವು_ಬಫರ್_Receive( x_Received_ಉದ್ದಳತೆ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಹರಿವು_ಬಫರ್_ಮುಂದಿನ_ಸಂದೇಶ_ಉದ್ದಳತೆ_ಬೈಟುಗಳು
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಹರಿವು_ಬಫರ್_ಮುಂದಿನ_ಸಂದೇಶ_ಉದ್ದಳತೆ_ಬೈಟುಗಳು( x_ಹರಿವು_ಬಫರ್ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಹರಿವು_ಬಫರ್_ಮುಂದಿನ_ಸಂದೇಶ_ಉದ್ದಳತೆ_ಬೈಟುಗಳು
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಹರಿವು_ಬಫರ್_ಮುಂದಿನ_ಸಂದೇಶ_ಉದ್ದಳತೆ_ಬೈಟುಗಳು( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಹರಿವು_ಬಫರ್_Receive_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಹರಿವು_ಬಫರ್_Receive_From_ISR( x_ಹರಿವು_ಬಫರ್, pv_Rx_ದತ್ತಾಂಶ, x_ಬಫರ್_ಉದ್ದಳತೆ_ಬೈಟುಗಳು, px_Higher_ಆದ್ಯತೆ_Task_Woken )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಹರಿವು_ಬಫರ್_Receive_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಹರಿವು_ಬಫರ್_Receive_From_ISR( x_Received_ಉದ್ದಳತೆ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಹರಿವು_ಬಫರ್_Is_ಖಾಲಿಗಳು
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಹರಿವು_ಬಫರ್_Is_ಖಾಲಿಗಳು( x_ಹರಿವು_ಬಫರ್ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಹರಿವು_ಬಫರ್_Is_ಖಾಲಿಗಳು
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಹರಿವು_ಬಫರ್_Is_ಖಾಲಿಗಳು( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಹರಿವು_ಬಫರ್_Is_ತುಂಬಿದ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಹರಿವು_ಬಫರ್_Is_ತುಂಬಿದ( x_ಹರಿವು_ಬಫರ್ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಹರಿವು_ಬಫರ್_Is_ತುಂಬಿದ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಹರಿವು_ಬಫರ್_Is_ತುಂಬಿದ( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಹರಿವು_ಬಫರ್_Send_Completed_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಹರಿವು_ಬಫರ್_Send_Completed_From_ISR( x_ಹರಿವು_ಬಫರ್, px_Higher_ಆದ್ಯತೆ_Task_Woken )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಹರಿವು_ಬಫರ್_Send_Completed_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಹರಿವು_ಬಫರ್_Send_Completed_From_ISR( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_x_ಹರಿವು_ಬಫರ್_Receive_Completed_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_x_ಹರಿವು_ಬಫರ್_Receive_Completed_From_ISR( x_ಹರಿವು_ಬಫರ್, px_Higher_ಆದ್ಯತೆ_Task_Woken )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_x_ಹರಿವು_ಬಫರ್_Receive_Completed_From_ISR
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_x_ಹರಿವು_ಬಫರ್_Receive_Completed_From_ISR( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_ux_ಹರಿವು_ಬಫರ್_Get_ಹರಿವು_ಬಫರ್_Notification_ಸೂಚ್ಯಂಕ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_ux_ಹರಿವು_ಬಫರ್_Get_ಹರಿವು_ಬಫರ್_Notification_ಸೂಚ್ಯಂಕ( x_ಹರಿವು_ಬಫರ್ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_ux_ಹರಿವು_ಬಫರ್_Get_ಹರಿವು_ಬಫರ್_Notification_ಸೂಚ್ಯಂಕ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_ux_ಹರಿವು_ಬಫರ್_Get_ಹರಿವು_ಬಫರ್_Notification_ಸೂಚ್ಯಂಕ( ux_Notification_ಸೂಚ್ಯಂಕ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_v_ಹರಿವು_ಬಫರ್_ಗಣ_ಹರಿವು_ಬಫರ್_Notification_ಸೂಚ್ಯಂಕ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_v_ಹರಿವು_ಬಫರ್_ಗಣ_ಹರಿವು_ಬಫರ್_Notification_ಸೂಚ್ಯಂಕ( x_ಹರಿವು_ಬಫರ್, ux_Notification_ಸೂಚ್ಯಂಕ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_v_ಹರಿವು_ಬಫರ್_ಗಣ_ಹರಿವು_ಬಫರ್_Notification_ಸೂಚ್ಯಂಕ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_v_ಹರಿವು_ಬಫರ್_ಗಣ_ಹರಿವು_ಬಫರ್_Notification_ಸೂಚ್ಯಂಕ()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_ux_ಹರಿವು_ಬಫರ್_Get_ಹರಿವು_ಬಫರ್_ಸಂಖ್ಯೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_ux_ಹರಿವು_ಬಫರ್_Get_ಹರಿವು_ಬಫರ್_ಸಂಖ್ಯೆ( x_ಹರಿವು_ಬಫರ್ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_ux_ಹರಿವು_ಬಫರ್_Get_ಹರಿವು_ಬಫರ್_ಸಂಖ್ಯೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_ux_ಹರಿವು_ಬಫರ್_Get_ಹರಿವು_ಬಫರ್_ಸಂಖ್ಯೆ( ux_ಹರಿವು_ಬಫರ್_ಸಂಖ್ಯೆ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_v_ಹರಿವು_ಬಫರ್_ಗಣ_ಹರಿವು_ಬಫರ್_ಸಂಖ್ಯೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_v_ಹರಿವು_ಬಫರ್_ಗಣ_ಹರಿವು_ಬಫರ್_ಸಂಖ್ಯೆ( x_ಹರಿವು_ಬಫರ್, ux_ಹರಿವು_ಬಫರ್_ಸಂಖ್ಯೆ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_v_ಹರಿವು_ಬಫರ್_ಗಣ_ಹರಿವು_ಬಫರ್_ಸಂಖ್ಯೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_v_ಹರಿವು_ಬಫರ್_ಗಣ_ಹರಿವು_ಬಫರ್_ಸಂಖ್ಯೆ()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_uc_ಹರಿವು_ಬಫರ್_Get_ಹರಿವು_ಬಫರ್_Type
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_uc_ಹರಿವು_ಬಫರ್_Get_ಹರಿವು_ಬಫರ್_Type( x_ಹರಿವು_ಬಫರ್ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_uc_ಹರಿವು_ಬಫರ್_Get_ಹರಿವು_ಬಫರ್_Type
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_uc_ಹರಿವು_ಬಫರ್_Get_ಹರಿವು_ಬಫರ್_Type( uc_ಹರಿವು_ಬಫರ್_Type )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_vListInitialise
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_vListInitialise( pxList )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_vListInitialise
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_vListInitialise()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_v_List_Initialise_ಅಂಶ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_v_List_Initialise_ಅಂಶ( px_ಅಂಶ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_v_List_Initialise_ಅಂಶ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_v_List_Initialise_ಅಂಶ()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_v_List_ಸೇರಿಸು_ಕೊನೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_v_List_ಸೇರಿಸು_ಕೊನೆ( pxList, px_New_List_ಅಂಶ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_v_List_ಸೇರಿಸು_ಕೊನೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_v_List_ಸೇರಿಸು_ಕೊನೆ()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_v_List_ಸೇರಿಸು
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_v_List_ಸೇರಿಸು( pxList, px_New_List_ಅಂಶ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_v_List_ಸೇರಿಸು
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_v_List_ಸೇರಿಸು()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_uxListRemove
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_uxListRemove( px_ಅಂಶ_ಗಮ್ಯ_Remove )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_uxListRemove
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_uxListRemove( ux_ಸಂಖ್ಯೆ_ದ_ವಸ್ತುಗಳು )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_xCoRoutineCreate
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_xCoRoutineCreate( px_Co_Routine_ಸಂಕೇತ, ux_ಆದ್ಯತೆ, ux_ಸೂಚ್ಯಂಕ )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_xCoRoutineCreate
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_xCoRoutineCreate( xReturn )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_v_Co_Routine_ಕೂಡಿಸು_ಗಮ್ಯ_Delayed_List
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_v_Co_Routine_ಕೂಡಿಸು_ಗಮ್ಯ_Delayed_List( x_Ticks_ಗಮ್ಯ_Delay, pxEventList )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_v_Co_Routine_ಕೂಡಿಸು_ಗಮ್ಯ_Delayed_List
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_v_Co_Routine_ಕೂಡಿಸು_ಗಮ್ಯ_Delayed_List()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_ENTER_v_Co_Routine_ವೇಳಾಪಟ್ಟಿ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_ENTER_v_Co_Routine_ವೇಳಾಪಟ್ಟಿ()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ trace_RETURN_v_Co_Routine_ವೇಳಾಪಟ್ಟಿ
    #ವ್ಯಾಖ್ಯಾನಿಸು trace_RETURN_v_Co_Routine_ವೇಳಾಪಟ್ಟಿ()
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceENTER_xCoRoutineRemoveFromEventList
    #ವ್ಯಾಖ್ಯಾನಿಸು traceENTER_xCoRoutineRemoveFromEventList( pxEventList )
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ traceRETURN_xCoRoutineRemoveFromEventList
    #ವ್ಯಾಖ್ಯಾನಿಸು traceRETURN_xCoRoutineRemoveFromEventList( xReturn )
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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_ENABLE_ರಾಶಿ_PROTECTOR
    #ವ್ಯಾಖ್ಯಾನಿಸು config_ENABLE_ರಾಶಿ_PROTECTOR    0
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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_KERNEL_PROVIDED_STATIC_ಸ್ಮರಣೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು config_KERNEL_PROVIDED_STATIC_ಸ್ಮರಣೆ    0
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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_STATS_ಬಫರ್_MAX_ಉದ್ದಳತೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು config_STATS_ಬಫರ್_MAX_ಉದ್ದಳತೆ    0xFFFF
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_ರಾಶಿ_ಆಳ_TYPE

/* Defaults to StackType_t for backward compatibility, but can be overridden
 * in FreeRTOSConfig.h if StackType_t is too restrictive. */
    #ವ್ಯಾಖ್ಯಾನಿಸು config_ರಾಶಿ_ಆಳ_TYPE    ರಾಶಿ_Type_t
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

#ಆದರೆ ( ( config_ಸತತ_ಅಪವರ್ತ್ಯ_PRIORITIES == 0 ) && ( configUSE_TASK_PREEMPTION_DISABLE != 0 ) )
    #ದೋಷ config_ಸತತ_ಅಪವರ್ತ್ಯ_PRIORITIES must be ಗಣ ಗಮ್ಯ 1 ಗಮ್ಯ use task preemption disable
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ ( ( configUSE_PREEMPTION == 0 ) && ( configUSE_TASK_PREEMPTION_DISABLE != 0 ) )
    #ದೋಷ configUSE_PREEMPTION must be ಗಣ ಗಮ್ಯ 1 ಗಮ್ಯ use task preemption disable
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ ( ( config_ಸಂಖ್ಯೆ_ದ_CORES == 1 ) && ( configUSE_TASK_PREEMPTION_DISABLE != 0 ) )
    #ದೋಷ configUSE_TASK_PREEMPTION_DISABLE is not supported ಒಳ single core FreeRTOS
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ ( ( config_ಸಂಖ್ಯೆ_ದ_CORES == 1 ) && ( configUSE_CORE_AFFINITY != 0 ) )
    #ದೋಷ configUSE_CORE_AFFINITY is not supported ಒಳ single core FreeRTOS
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ ( ( config_ಸಂಖ್ಯೆ_ದ_CORES > 1 ) && ( config_USE_ಪೋರ್ಟ್_OPTIMISED_TASK_SELECTION != 0 ) )
    #ದೋಷ config_USE_ಪೋರ್ಟ್_OPTIMISED_TASK_SELECTION is not supported ಒಳ SMP FreeRTOS
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

/* The following config allows infinite loop control. For example, control the
 * infinite loop in idle task function when performing unit tests. */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configCONTROL_INFINITE_LOOP
    #ವ್ಯಾಖ್ಯಾನಿಸು configCONTROL_INFINITE_LOOP()
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
    #ಆದರೆ ( configUSE_CORE_AFFINITY == 1 ) && ( config_ಸಂಖ್ಯೆ_ದ_CORES > 1 )
        UBaseType_t uxDummy26;
    #ಆದರೆ_ಅಂತ್ಯ
    Static_List_ಅಂಶ_t xDummy3[ 2 ];
    UBaseType_t uxDummy5;
    ಶೂನ್ಯ * pxDummy6;
    #ಆದರೆ ( config_ಸಂಖ್ಯೆ_ದ_CORES > 1 )
        ತಳ_Type_t xDummy23;
        UBaseType_t uxDummy24;
    #ಆದರೆ_ಅಂತ್ಯ
    uint8_t ucDummy7[ configMAX_TASK_NAME_LEN ];
    #ಆದರೆ ( configUSE_TASK_PREEMPTION_DISABLE == 1 )
        ತಳ_Type_t xDummy25;
    #ಆದರೆ_ಅಂತ್ಯ
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
    #ಆದರೆ ( configUSE_C_RUNTIME_TLS_SUPPORT == 1 )
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
    UBaseType_t uxDummy6;
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
