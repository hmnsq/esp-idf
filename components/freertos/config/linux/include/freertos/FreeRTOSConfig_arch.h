/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#لارښوونه once

/* Linux POSIX simulator specific configuration. This file is included in the common FreeRTOSConfig.h. */

#ضمن "sdkconfig.h"

/* ------------------------------------------------- FreeRTOS Config ---------------------------------------------------
 * - All Vanilla FreeRTOS configuration goes into this section
 * - Match upstream POSIX simulator example FreeRTOSConfig.h where possible. See following link for more details.
 *   https://github.com/FreeRTOS/FreeRTOS/blob/main/FreeRTOS/Demo/Posix_GCC/FreeRTOSConfig.h
 * ------------------------------------------------------------------------------------------------------------------ */

/* ------------------ Scheduler Related -------------------- */

#تعریف_کړه config_USE_پورټ_OPTIMISED_TASK_SELECTION    0

/* The stack allocated by FreeRTOS will be passed to a pthread.
 * pthread has a minimal stack size which currently is 16KB.
 * The rest is for additional structures of the POSIX/Linux port.
 * This is a magic number since PTHREAD_STACK_MIN seems to not be a constant. */
#تعریف_کړه config_اصغري_ډېری_اندازه                   ( ( ډېری_Type_t ) ( 0x4000 + 40 ) / حجم( ډېری_Type_t ) )
/* Currently not used in Linux POSIX simulator */
#تعریف_کړه config_MAX_API_CALL_مداخله_لومړیتوب      0

/* ---------------- Amazon SMP FreeRTOS -------------------- */

#إذا CONFIG_FREERTOS_SMP
    #تعریف_کړه configUSE_PASSIVE_IDLE_HOOK    0             /* Not implemented yet, TODO IDF-6654 */
#د_که_پای

/* ----------------------- System -------------------------- */

/* On the Linux simulator, we use the system-provided libc */
#تعریف_کړه configUSE_NEWLIB_REENTRANT                   0

#تعریف_کړه configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H    1

/* ----------------------- Memory  ------------------------- */

#تعریف_کړه config_ټول_ډېرۍ_اندازه    ( ( size_t ) ( 65 * 1024 ) )

/* ------------------- Run-time Stats ---------------------- */

#تعریف_کړه configUSE_TRACE_FACILITY    1

/* -------------------- API Includes ----------------------- */

/* Todo: Reconcile INCLUDE_option differences (IDF-8186) */
#تعریف_کړه INCLUDE_vTaskDelayUntil                 1
#تعریف_کړه INCLUDE_ux_Task_Get_ډېری_لوړ_Water_Mark2    0

/* ------------------------------------------------ ESP-IDF Additions --------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */
