/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಸೂಚನೆ once

/* Xtensa Architecture specific configuration. This file is included in the common FreeRTOSConfig.h. */

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"

/* Required for configuration-dependent settings. */
#ಅನ್ತರ್ಭಾವ "xtensa_config.h"

/* -------------------------------------------- Xtensa Additional Config  ----------------------------------------------
 * - Provide Xtensa definitions usually given by -D option when building with xt-make (see readme_xtensa.txt)
 * - xtensa_rtos.h and xtensa_timer.h will default some of these values
 *      - XT_SIMULATOR         configXT_SIMULATOR
 *      - XT_BOARD             configXT_BOARD
 *      - XT_CLOCK_FREQ        Should not be defined as we are using XT_BOARD mode
 *      - XT_TICK_PER_SEC      Defaults to configTICK_RATE_HZ
 *      - XT_TIMER_INDEX       Defaults to configXT_TIMER_INDEX
 *      - XT_INTEXC_HOOKS      Defaults to configXT_INTEXC_HOOKS
 *      - XT_USE_OVLY          We don't define this (unused)
 *      - XT_USE_SWPRI         We don't define this (unused)
 * ------------------------------------------------------------------------------------------------------------------ */

#ವ್ಯಾಖ್ಯಾನಿಸು configXT_SIMULATOR          0
#ವ್ಯಾಖ್ಯಾನಿಸು configXT_BOARD              1                           /* Board mode */
#ಆದರೆ CONFIG_FREERTOS_CORETIMER_0
    #ವ್ಯಾಖ್ಯಾನಿಸು config_XT_TIMER_ಸೂಚ್ಯಂಕ    0
#ಅಥವಾಆದರೆ CONFIG_FREERTOS_CORETIMER_1
    #ವ್ಯಾಖ್ಯಾನಿಸು config_XT_TIMER_ಸೂಚ್ಯಂಕ    1
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿಸು configXT_INTEXC_HOOKS       0

#ವ್ಯಾಖ್ಯಾನಿಸು configBENCHMARK             0

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
#ವ್ಯಾಖ್ಯಾನಿಸು config_MAX_API_CALL_ಅಡಚಣೆ_ಆದ್ಯತೆ          XCHAL_EXCM_LEVEL

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

/* The Xtensa port uses a separate interrupt stack. Adjust the stack size
 * to suit the needs of your specific application.
 * Size needs to be aligned to the stack increment, since the location of
 * the stack for the 2nd CPU will be calculated using configISR_STACK_SIZE.
 */
#ವ್ಯಾಖ್ಯಾನಿಸು config_ರಾಶಿ_ALIGNMENT       16
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_ISR_ರಾಶಿ_ಗಾತ್ರ
    #ವ್ಯಾಖ್ಯಾನಿಸು config_ISR_ರಾಶಿ_ಗಾತ್ರ    ( ( CONFIG_FREERTOS_ISR_STACKSIZE + config_ರಾಶಿ_ALIGNMENT - 1 ) & ( ~( config_ರಾಶಿ_ALIGNMENT - 1 ) ) )
#ಆದರೆ_ಅಂತ್ಯ
