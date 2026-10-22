/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#निर्देशः once

/* Linux POSIX simulator specific configuration. This file is included in the common FreeRTOSConfig.h. */

#अन्तर्भाव "sdkconfig.h"

/* ------------------------------------------------- FreeRTOS Config ---------------------------------------------------
 * - All Vanilla FreeRTOS configuration goes into this section
 * - Match upstream POSIX simulator example FreeRTOSConfig.h where possible. See following link for more details.
 *   https://github.com/FreeRTOS/FreeRTOS/blob/main/FreeRTOS/Demo/Posix_GCC/FreeRTOSConfig.h
 * ------------------------------------------------------------------------------------------------------------------ */

/* ------------------ Scheduler Related -------------------- */

#परिभाषय config_USE_पत्तनम्_OPTIMISED_TASK_SELECTION    0

/* The stack allocated by FreeRTOS will be passed to a pthread.
 * pthread has a minimal stack size which currently is 16KB.
 * The rest is for additional structures of the POSIX/Linux port.
 * This is a magic number since PTHREAD_STACK_MIN seems to not be a constant. */
#परिभाषय config_अल्पिष्ठम्_राशिः_आकार                   ( ( राशिः_Type_t ) ( 0x4000 + 40 ) / परिमाण( राशिः_Type_t ) )
/* Currently not used in Linux POSIX simulator */
#परिभाषय config_MAX_API_CALL_व्यवधानम्_प्राथमिकता      0

/* ---------------- Amazon SMP FreeRTOS -------------------- */

#यदि CONFIG_FREERTOS_SMP
    #परिभाषय configUSE_PASSIVE_IDLE_HOOK    0             /* Not implemented yet, TODO IDF-6654 */
#यदि_अन्तः

/* ----------------------- System -------------------------- */

/* On the Linux simulator, we use the system-provided libc */
#परिभाषय configUSE_NEWLIB_REENTRANT                   0

#परिभाषय configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H    1

/* ----------------------- Memory  ------------------------- */

#परिभाषय config_योग_HEAP_आकार    ( ( size_t ) ( 65 * 1024 ) )

/* ------------------- Run-time Stats ---------------------- */

#परिभाषय configUSE_TRACE_FACILITY    1

/* -------------------- API Includes ----------------------- */

/* Todo: Reconcile INCLUDE_option differences (IDF-8186) */
#परिभाषय INCLUDE_vTaskDelayUntil                 1
#परिभाषय INCLUDE_ux_Task_Get_राशिः_उच्चः_Water_Mark2    0

/* ------------------------------------------------ ESP-IDF Additions --------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */
