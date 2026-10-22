/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#هدايت once

/* Linux POSIX simulator specific configuration. This file is included in the common FreeRTOSConfig.h. */

#ضمن "sdkconfig.h"

/* ------------------------------------------------- FreeRTOS Config ---------------------------------------------------
 * - All Vanilla FreeRTOS configuration goes into this section
 * - Match upstream POSIX simulator example FreeRTOSConfig.h where possible. See following link for more details.
 *   https://github.com/FreeRTOS/FreeRTOS/blob/main/FreeRTOS/Demo/Posix_GCC/FreeRTOSConfig.h
 * ------------------------------------------------------------------------------------------------------------------ */

/* ------------------ Scheduler Related -------------------- */

#وصف_ڪريو config_USE_پورٽ_OPTIMISED_TASK_SELECTION    0

/* The stack allocated by FreeRTOS will be passed to a pthread.
 * pthread has a minimal stack size which currently is 16KB.
 * The rest is for additional structures of the POSIX/Linux port.
 * This is a magic number since PTHREAD_STACK_MIN seems to not be a constant. */
#وصف_ڪريو config_گهٽتر_ڍير_سائيز                   ( ( ڍير_Type_t ) ( 0x4000 + 40 ) / حجم( ڍير_Type_t ) )
/* Currently not used in Linux POSIX simulator */
#وصف_ڪريو config_MAX_API_CALL_مداخلت_ترجيح      0

/* ---------------- Amazon SMP FreeRTOS -------------------- */

#إذا CONFIG_FREERTOS_SMP
    #وصف_ڪريو configUSE_PASSIVE_IDLE_HOOK    0             /* Not implemented yet, TODO IDF-6654 */
#جيڪڏهن_پورو

/* ----------------------- System -------------------------- */

/* On the Linux simulator, we use the system-provided libc */
#وصف_ڪريو configUSE_NEWLIB_REENTRANT                   0

#وصف_ڪريو configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H    1

/* ----------------------- Memory  ------------------------- */

#وصف_ڪريو config_ڪل_ڍير_سائيز    ( ( size_t ) ( 65 * 1024 ) )

/* ------------------- Run-time Stats ---------------------- */

#وصف_ڪريو configUSE_TRACE_FACILITY    1

/* -------------------- API Includes ----------------------- */

/* Todo: Reconcile INCLUDE_option differences (IDF-8186) */
#وصف_ڪريو INCLUDE_vTaskDelayUntil                 1
#وصف_ڪريو INCLUDE_ux_Task_Get_ڍير_مٿاهون_Water_Mark2    0

/* ------------------------------------------------ ESP-IDF Additions --------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */
