/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਹਦਾਇਤ once

/* Linux POSIX simulator specific configuration. This file is included in the common FreeRTOSConfig.h. */

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"

/* ------------------------------------------------- FreeRTOS Config ---------------------------------------------------
 * - All Vanilla FreeRTOS configuration goes into this section
 * - Match upstream POSIX simulator example FreeRTOSConfig.h where possible. See following link for more details.
 *   https://github.com/FreeRTOS/FreeRTOS/blob/main/FreeRTOS/Demo/Posix_GCC/FreeRTOSConfig.h
 * ------------------------------------------------------------------------------------------------------------------ */

/* ------------------ Scheduler Related -------------------- */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_USE_ਪੋਰਟ_OPTIMISED_TASK_SELECTION    0

/* The stack allocated by FreeRTOS will be passed to a pthread.
 * pthread has a minimal stack size which currently is 16KB.
 * The rest is for additional structures of the POSIX/Linux port.
 * This is a magic number since PTHREAD_STACK_MIN seems to not be a constant. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_ਨਿਊਨਤਮ_ਢੇਰ_ਮਾਪ                   ( ( ਢੇਰ_Type_t ) ( 0x4000 + 40 ) / ਆਕਾਰ( ਢੇਰ_Type_t ) )
/* Currently not used in Linux POSIX simulator */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_MAX_API_CALL_ਰੁਕਾਵਟ_ਤਰਜੀਹ      0

/* ---------------- Amazon SMP FreeRTOS -------------------- */

#ਜੇ CONFIG_FREERTOS_SMP
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_PASSIVE_IDLE_HOOK    0             /* Not implemented yet, TODO IDF-6654 */
#ਜੇ_ਅੰਤ

/* ----------------------- System -------------------------- */

/* On the Linux simulator, we use the system-provided libc */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_NEWLIB_REENTRANT                   0

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H    1

/* ----------------------- Memory  ------------------------- */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_ਕੁੱਲ_ਢੇਰ_ਮਾਪ    ( ( size_t ) ( 65 * 1024 ) )

/* ------------------- Run-time Stats ---------------------- */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_TRACE_FACILITY    1

/* -------------------- API Includes ----------------------- */

/* Todo: Reconcile INCLUDE_option differences (IDF-8186) */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_vTaskDelayUntil                 1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_ux_Task_Get_ਢੇਰ_ਉੱਚਾ_Water_Mark2    0

/* ------------------------------------------------ ESP-IDF Additions --------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */
