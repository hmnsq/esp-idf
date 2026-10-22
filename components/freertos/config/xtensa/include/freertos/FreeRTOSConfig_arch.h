/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

/* Xtensa Architecture specific configuration. This file is included in the common FreeRTOSConfig.h. */

#અન્તર્ભાવ "sdkconfig.h"

/* Required for configuration-dependent settings. */
#અન્તર્ભાવ "xtensa_config.h"

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

#define configXT_SIMULATOR          0
#define configXT_BOARD              1                           /* Board mode */
#જો CONFIG_FREERTOS_CORETIMER_0
    #define configXT_TIMER_INDEX    0
#અથવાજો CONFIG_FREERTOS_CORETIMER_1
    #define configXT_TIMER_INDEX    1
#endif
#define configXT_INTEXC_HOOKS       0

#define configBENCHMARK             0

/* ------------------------------------------------- FreeRTOS Config ---------------------------------------------------
 * - All Vanilla FreeRTOS configuration goes into this section
 * ------------------------------------------------------------------------------------------------------------------ */

/* ------------------ Scheduler Related -------------------- */

#ifdef CONFIG_FREERTOS_OPTIMIZED_SCHEDULER
    #define configUSE_PORT_OPTIMISED_TASK_SELECTION    1
#નહીંતર
    #define configUSE_PORT_OPTIMISED_TASK_SELECTION    0
#endif /* CONFIG_FREERTOS_OPTIMIZED_SCHEDULER */
#define config_MINIMAL_ઢગલો_SIZE                       ( CONFIG_FREERTOS_IDLE_TASK_STACKSIZE + config_ઢગલો_OVERHEAD_TOTAL )
#define configMAX_API_CALL_INTERRUPT_PRIORITY          XCHAL_EXCM_LEVEL

/* ---------------- Amazon SMP FreeRTOS -------------------- */

#જો CONFIG_FREERTOS_SMP

/* This is always enabled to call IDF style idle hooks, by can be "--Wl,--wrap"
 * if users enable CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK. */
    #define configUSE_PASSIVE_IDLE_HOOK    1
#endif

/* ----------------------- System -------------------------- */

#જો CONFIG_LIBC_NEWLIB
#define configUSE_NEWLIB_REENTRANT    1
#નહીંતર
#define configUSE_NEWLIB_REENTRANT    0
#endif

/* - FreeRTOS provides default for configTLS_BLOCK_TYPE.
 * - We simply provide our own INIT and DEINIT functions
 * - We set "SET" to a blank macro since there is no need to set the reentrancy
 * pointer. All newlib functions calls __getreent. */
#જો CONFIG_FREERTOS_SMP
    #define configINIT_TLS_BLOCK( xTLSBlock, px_ટોચ_Of_ઢગલો )    esp_reent_init( &( xTLSBlock ) )
#નહીંતર /* CONFIG_FREERTOS_SMP */
    #define configINIT_TLS_BLOCK( xTLSBlock )                  esp_reent_init( &( xTLSBlock ) )
#endif /* CONFIG_FREERTOS_SMP */
#define configSET_TLS_BLOCK( xTLSBlock )
#define configDEINIT_TLS_BLOCK( xTLSBlock )                    _reclaim_reent( &( xTLSBlock ) )

#define configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H    1

/* ----------------------- Memory  ------------------------- */

/* This isn't used as FreeRTOS will only allocate from internal memory (see
 * heap_idf.c). We simply define this macro to span all non-statically-allocated
 * shared RAM. */
#define configTOTAL_HEAP_SIZE    ( &_heap_end - &_heap_start )

/* ------------------- Run-time Stats ---------------------- */

#જો CONFIG_FREERTOS_USE_TRACE_FACILITY
    /* Used by uxTaskGetSystemState(), and other trace facility functions */
    #define configUSE_TRACE_FACILITY    1
#endif /* CONFIG_FREERTOS_USE_TRACE_FACILITY */

/* -------------------- API Includes ----------------------- */

/* Todo: Reconcile INCLUDE_option differences (IDF-8186) */
#define INCLUDE_xTaskDelayUntil                 1
#define INCLUDE_ux_Task_Get_ઢગલો_ઉપલું_Water_Mark2    1

/* ------------------------------------------------ ESP-IDF Additions --------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */

/* The Xtensa port uses a separate interrupt stack. Adjust the stack size
 * to suit the needs of your specific application.
 * Size needs to be aligned to the stack increment, since the location of
 * the stack for the 2nd CPU will be calculated using configISR_STACK_SIZE.
 */
#define config_ઢગલો_ALIGNMENT       16
#ifndef config_ISR_ઢગલો_SIZE
    #define config_ISR_ઢગલો_SIZE    ( ( CONFIG_FREERTOS_ISR_STACKSIZE + config_ઢગલો_ALIGNMENT - 1 ) & ( ~( config_ઢગલો_ALIGNMENT - 1 ) ) )
#endif
