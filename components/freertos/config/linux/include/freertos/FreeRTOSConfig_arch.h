/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಸೂಚನೆ once

/* Linux POSIX simulator specific configuration. This file is included in the common FreeRTOSConfig.h. */

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"

/* ------------------------------------------------- FreeRTOS Config ---------------------------------------------------
 * - All Vanilla FreeRTOS configuration goes into this section
 * - Match upstream POSIX simulator example FreeRTOSConfig.h where possible. See following link for more details.
 *   https://github.com/FreeRTOS/FreeRTOS/blob/main/FreeRTOS/Demo/Posix_GCC/FreeRTOSConfig.h
 * ------------------------------------------------------------------------------------------------------------------ */

/* ------------------ Scheduler Related -------------------- */

#ವ್ಯಾಖ್ಯಾನಿಸು config_USE_ಪೋರ್ಟ್_OPTIMISED_TASK_SELECTION    0

/* The stack allocated by FreeRTOS will be passed to a pthread.
 * pthread has a minimal stack size which currently is 16KB.
 * The rest is for additional structures of the POSIX/Linux port.
 * This is a magic number since PTHREAD_STACK_MIN seems to not be a constant. */
#ವ್ಯಾಖ್ಯಾನಿಸು config_ಕನಿಷ್ಠತಮ_ರಾಶಿ_ಗಾತ್ರ                   ( ( ರಾಶಿ_Type_t ) ( 0x4000 + 40 ) / ಗಾತ್ರ( ರಾಶಿ_Type_t ) )
/* Currently not used in Linux POSIX simulator */
#ವ್ಯಾಖ್ಯಾನಿಸು config_MAX_API_CALL_ಅಡಚಣೆ_ಆದ್ಯತೆ      0

/* ---------------- Amazon SMP FreeRTOS -------------------- */

#ಆದರೆ CONFIG_FREERTOS_SMP
    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_PASSIVE_IDLE_HOOK    0             /* Not implemented yet, TODO IDF-6654 */
#ಆದರೆ_ಅಂತ್ಯ

/* ----------------------- System -------------------------- */

/* On the Linux simulator, we use the system-provided libc */
#ವ್ಯಾಖ್ಯಾನಿಸು configUSE_NEWLIB_REENTRANT                   0

#ವ್ಯಾಖ್ಯಾನಿಸು configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H    1

/* ----------------------- Memory  ------------------------- */

#ವ್ಯಾಖ್ಯಾನಿಸು config_ಒಟ್ಟು_ರಾಶಿ_ಗಾತ್ರ    ( ( size_t ) ( 65 * 1024 ) )

/* ------------------- Run-time Stats ---------------------- */

#ವ್ಯಾಖ್ಯಾನಿಸು configUSE_TRACE_FACILITY    1

/* -------------------- API Includes ----------------------- */

/* Todo: Reconcile INCLUDE_option differences (IDF-8186) */
#ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_vTaskDelayUntil                 1
#ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_ux_Task_Get_ರಾಶಿ_ಮೇಲಿನ_Water_Mark2    0

/* ------------------------------------------------ ESP-IDF Additions --------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */
