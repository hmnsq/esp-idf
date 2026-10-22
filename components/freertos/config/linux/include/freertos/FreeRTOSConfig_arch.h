/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ہدایت once

/* Linux POSIX simulator specific configuration. This file is included in the common FreeRTOSConfig.h. */

#شامل "sdkconfig.h"

/* ------------------------------------------------- FreeRTOS Config ---------------------------------------------------
 * - All Vanilla FreeRTOS configuration goes into this section
 * - Match upstream POSIX simulator example FreeRTOSConfig.h where possible. See following link for more details.
 *   https://github.com/FreeRTOS/FreeRTOS/blob/main/FreeRTOS/Demo/Posix_GCC/FreeRTOSConfig.h
 * ------------------------------------------------------------------------------------------------------------------ */

/* ------------------ Scheduler Related -------------------- */

#تعریف_کرو config_USE_پورٹ_OPTIMISED_TASK_SELECTION    0

/* The stack allocated by FreeRTOS will be passed to a pthread.
 * pthread has a minimal stack size which currently is 16KB.
 * The rest is for additional structures of the POSIX/Linux port.
 * This is a magic number since PTHREAD_STACK_MIN seems to not be a constant. */
#تعریف_کرو config_اقل_ڈھیر_سائز                   ( ( ڈھیر_Type_t ) ( 0x4000 + 40 ) / حجم( ڈھیر_Type_t ) )
/* Currently not used in Linux POSIX simulator */
#تعریف_کرو config_MAX_API_CALL_مداخلت_ترجیح      0

/* ---------------- Amazon SMP FreeRTOS -------------------- */

#اگر CONFIG_FREERTOS_SMP
    #تعریف_کرو configUSE_PASSIVE_IDLE_HOOK    0             /* Not implemented yet, TODO IDF-6654 */
#اگر_ختم

/* ----------------------- System -------------------------- */

/* On the Linux simulator, we use the system-provided libc */
#تعریف_کرو configUSE_NEWLIB_REENTRANT                   0

#تعریف_کرو configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H    1

/* ----------------------- Memory  ------------------------- */

#تعریف_کرو config_کل_ڈھیر_سائز    ( ( size_t ) ( 65 * 1024 ) )

/* ------------------- Run-time Stats ---------------------- */

#تعریف_کرو configUSE_TRACE_FACILITY    1

/* -------------------- API Includes ----------------------- */

/* Todo: Reconcile INCLUDE_option differences (IDF-8186) */
#تعریف_کرو INCLUDE_vTaskDelayUntil                 1
#تعریف_کرو INCLUDE_ux_Task_Get_ڈھیر_بلند_Water_Mark2    0

/* ------------------------------------------------ ESP-IDF Additions --------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */
