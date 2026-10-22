/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಸೂಚನೆ once

/* RISC-V Architecture specific configuration. This file is included in the common FreeRTOSConfig.h. */

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"

/* ------------------------------------------------- FreeRTOS Config ---------------------------------------------------
 * - All Vanilla FreeRTOS configuration goes into this section
 * ------------------------------------------------------------------------------------------------------------------ */

/* ------------------ Scheduler Related -------------------- */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FREERTOS_OPTIMIZED_SCHEDULER
    #ವ್ಯಾಖ್ಯಾನಿಸು config_USE_ಪೋರ್ಟ್_OPTIMISED_TASK_SELECTION    1
#ಇಲ್ಲದಿದ್ದರೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು config_USE_ಪೋರ್ಟ್_OPTIMISED_TASK_SELECTION    0
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_OPTIMIZED_SCHEDULER */
#ವ್ಯಾಖ್ಯಾನಿಸು config_ಕನಿಷ್ಠತಮ_ರಾಶಿ_ಗಾತ್ರ                       ( CONFIG_FREERTOS_IDLE_TASK_STACKSIZE + config_ರಾಶಿ_OVERHEAD_ಒಟ್ಟು )
#ವ್ಯಾಖ್ಯಾನಿಸು config_MAX_API_CALL_ಅಡಚಣೆ_ಆದ್ಯತೆ          0

/* ---------------- Amazon SMP FreeRTOS -------------------- */

#ಆದರೆ CONFIG_FREERTOS_SMP

/* This is always enabled to call IDF style idle hooks, by can be "--Wl,--wrap"
 * if users enable CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK. */
    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_PASSIVE_IDLE_HOOK    1
#ಆದರೆ_ಅಂತ್ಯ

/* ----------------------- System -------------------------- */

#ಆದರೆ CONFIG_LIBC_NEWLIB
#ವ್ಯಾಖ್ಯಾನಿಸು configUSE_NEWLIB_REENTRANT    1
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು configUSE_NEWLIB_REENTRANT    0
#ಆದರೆ_ಅಂತ್ಯ

/* - FreeRTOS provides default for configTLS_BLOCK_TYPE.
 * - We simply provide our own INIT and DEINIT functions
 * - We set "SET" to a blank macro since there is no need to set the reentrancy
 * pointer. All newlib functions calls __getreent. */
#ಆದರೆ CONFIG_FREERTOS_SMP
    #ವ್ಯಾಖ್ಯಾನಿಸು config_INIT_TLS_ಖಂಡ( xTLSBlock, px_ತುದಿ_ದ_ರಾಶಿ )    esp_reent_init( &( xTLSBlock ) )
#ಇಲ್ಲದಿದ್ದರೆ /* CONFIG_FREERTOS_SMP */
    #ವ್ಯಾಖ್ಯಾನಿಸು config_INIT_TLS_ಖಂಡ( xTLSBlock )                  esp_reent_init( &( xTLSBlock ) )
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_SMP */
#ವ್ಯಾಖ್ಯಾನಿಸು config_ಗಣ_TLS_ಖಂಡ( xTLSBlock )
#ವ್ಯಾಖ್ಯಾನಿಸು config_DEINIT_TLS_ಖಂಡ( xTLSBlock )                    _reclaim_reent( &( xTLSBlock ) )

#ವ್ಯಾಖ್ಯಾನಿಸು configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H    1

/* ----------------------- Memory  ------------------------- */

/* This isn't used as FreeRTOS will only allocate from internal memory (see
 * heap_idf.c). We simply define this macro to span all non-statically-allocated
 * shared RAM. */
#ವ್ಯಾಖ್ಯಾನಿಸು config_ಒಟ್ಟು_ರಾಶಿ_ಗಾತ್ರ    ( &ರಾಶಿ_ಕೊನೆ - &ರಾಶಿ_ಆರಂಭ )

/* ------------------- Run-time Stats ---------------------- */

#ಆದರೆ CONFIG_FREERTOS_USE_TRACE_FACILITY
    /* Used by uxTaskGetSystemState(), and other trace facility functions */
    #ವ್ಯಾಖ್ಯಾನಿಸು configUSE_TRACE_FACILITY    1
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_USE_TRACE_FACILITY */

/* -------------------- API Includes ----------------------- */

/* Todo: Reconcile INCLUDE_option differences (IDF-8186) */
#ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_xTaskDelayUntil                 1
#ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_ux_Task_Get_ರಾಶಿ_ಮೇಲಿನ_Water_Mark2    1

/* ------------------------------------------------ ESP-IDF Additions --------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_ISR_ರಾಶಿ_ಗಾತ್ರ
    #ವ್ಯಾಖ್ಯಾನಿಸು config_ISR_ರಾಶಿ_ಗಾತ್ರ    ( CONFIG_FREERTOS_ISR_STACKSIZE )
#ಆದರೆ_ಅಂತ್ಯ
