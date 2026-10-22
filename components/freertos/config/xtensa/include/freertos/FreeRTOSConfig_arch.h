/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਹਦਾਇਤ once

/* Xtensa Architecture specific configuration. This file is included in the common FreeRTOSConfig.h. */

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"

/* Required for configuration-dependent settings. */
#ਅਨ੍ਤਰ੍ਭਾਵ "xtensa_config.h"

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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configXT_SIMULATOR          0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configXT_BOARD              1                           /* Board mode */
#ਜੇ CONFIG_FREERTOS_CORETIMER_0
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_XT_TIMER_ਸੂਚਕਾਂਕ    0
#ਜਾਂਜੇ CONFIG_FREERTOS_CORETIMER_1
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_XT_TIMER_ਸੂਚਕਾਂਕ    1
#ਜੇ_ਅੰਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configXT_INTEXC_HOOKS       0

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configBENCHMARK             0

/* ------------------------------------------------- FreeRTOS Config ---------------------------------------------------
 * - All Vanilla FreeRTOS configuration goes into this section
 * ------------------------------------------------------------------------------------------------------------------ */

/* ------------------ Scheduler Related -------------------- */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FREERTOS_OPTIMIZED_SCHEDULER
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_USE_ਪੋਰਟ_OPTIMISED_TASK_SELECTION    1
#ਨਹੀਂਤਾਂ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_USE_ਪੋਰਟ_OPTIMISED_TASK_SELECTION    0
#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_OPTIMIZED_SCHEDULER */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_ਨਿਊਨਤਮ_ਢੇਰ_ਮਾਪ                       ( CONFIG_FREERTOS_IDLE_TASK_STACKSIZE + config_ਢੇਰ_OVERHEAD_ਕੁੱਲ )
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_MAX_API_CALL_ਰੁਕਾਵਟ_ਤਰਜੀਹ          XCHAL_EXCM_LEVEL

/* ---------------- Amazon SMP FreeRTOS -------------------- */

#ਜੇ CONFIG_FREERTOS_SMP

/* This is always enabled to call IDF style idle hooks, by can be "--Wl,--wrap"
 * if users enable CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK. */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_PASSIVE_IDLE_HOOK    1
#ਜੇ_ਅੰਤ

/* ----------------------- System -------------------------- */

#ਜੇ CONFIG_LIBC_NEWLIB
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_NEWLIB_REENTRANT    1
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_NEWLIB_REENTRANT    0
#ਜੇ_ਅੰਤ

/* - FreeRTOS provides default for configTLS_BLOCK_TYPE.
 * - We simply provide our own INIT and DEINIT functions
 * - We set "SET" to a blank macro since there is no need to set the reentrancy
 * pointer. All newlib functions calls __getreent. */
#ਜੇ CONFIG_FREERTOS_SMP
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_INIT_TLS_ਖੰਡ( xTLSBlock, px_ਸਿਖਰ_ਦਾ_ਢੇਰ )    esp_reent_init( &( xTLSBlock ) )
#ਨਹੀਂਤਾਂ /* CONFIG_FREERTOS_SMP */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_INIT_TLS_ਖੰਡ( xTLSBlock )                  esp_reent_init( &( xTLSBlock ) )
#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_SMP */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_ਸੈੱਟ_TLS_ਖੰਡ( xTLSBlock )
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_DEINIT_TLS_ਖੰਡ( xTLSBlock )                    _reclaim_reent( &( xTLSBlock ) )

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H    1

/* ----------------------- Memory  ------------------------- */

/* This isn't used as FreeRTOS will only allocate from internal memory (see
 * heap_idf.c). We simply define this macro to span all non-statically-allocated
 * shared RAM. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_ਕੁੱਲ_ਢੇਰ_ਮਾਪ    ( &ਢੇਰ_ਸਿਰਾ - &ਢੇਰ_ਸ਼ੁਰੂ )

/* ------------------- Run-time Stats ---------------------- */

#ਜੇ CONFIG_FREERTOS_USE_TRACE_FACILITY
    /* Used by uxTaskGetSystemState(), and other trace facility functions */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_TRACE_FACILITY    1
#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_USE_TRACE_FACILITY */

/* -------------------- API Includes ----------------------- */

/* Todo: Reconcile INCLUDE_option differences (IDF-8186) */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_xTaskDelayUntil                 1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INCLUDE_ux_Task_Get_ਢੇਰ_ਉੱਚਾ_Water_Mark2    1

/* ------------------------------------------------ ESP-IDF Additions --------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */

/* The Xtensa port uses a separate interrupt stack. Adjust the stack size
 * to suit the needs of your specific application.
 * Size needs to be aligned to the stack increment, since the location of
 * the stack for the 2nd CPU will be calculated using configISR_STACK_SIZE.
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_ਢੇਰ_ALIGNMENT       16
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_ISR_ਢੇਰ_ਮਾਪ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_ISR_ਢੇਰ_ਮਾਪ    ( ( CONFIG_FREERTOS_ISR_STACKSIZE + config_ਢੇਰ_ALIGNMENT - 1 ) & ( ~( config_ਢੇਰ_ALIGNMENT - 1 ) ) )
#ਜੇ_ਅੰਤ
