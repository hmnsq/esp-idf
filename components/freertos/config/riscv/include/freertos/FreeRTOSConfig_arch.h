/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

/* RISC-V Architecture specific configuration. This file is included in the common FreeRTOSConfig.h. */

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"

/* ------------------------------------------------- FreeRTOS Config ---------------------------------------------------
 * - All Vanilla FreeRTOS configuration goes into this section
 * ------------------------------------------------------------------------------------------------------------------ */

/* ------------------ Scheduler Related -------------------- */

#ifdef CONFIG_FREERTOS_OPTIMIZED_SCHEDULER
    #define configUSE_PORT_OPTIMISED_TASK_SELECTION    1
#ಅನ್ಯಥಾ
    #define configUSE_PORT_OPTIMISED_TASK_SELECTION    0
#endif /* CONFIG_FREERTOS_OPTIMIZED_SCHEDULER */
#define config_MINIMAL_ರಾಶಿ_SIZE                       ( CONFIG_FREERTOS_IDLE_TASK_STACKSIZE + config_ರಾಶಿ_OVERHEAD_TOTAL )
#define configMAX_API_CALL_INTERRUPT_PRIORITY          0

/* ---------------- Amazon SMP FreeRTOS -------------------- */

#ಯದಿ CONFIG_FREERTOS_SMP

/* This is always enabled to call IDF style idle hooks, by can be "--Wl,--wrap"
 * if users enable CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK. */
    #define configUSE_PASSIVE_IDLE_HOOK    1
#endif

/* ----------------------- System -------------------------- */

#ಯದಿ CONFIG_LIBC_NEWLIB
#define configUSE_NEWLIB_REENTRANT    1
#ಅನ್ಯಥಾ
#define configUSE_NEWLIB_REENTRANT    0
#endif

/* - FreeRTOS provides default for configTLS_BLOCK_TYPE.
 * - We simply provide our own INIT and DEINIT functions
 * - We set "SET" to a blank macro since there is no need to set the reentrancy
 * pointer. All newlib functions calls __getreent. */
#ಯದಿ CONFIG_FREERTOS_SMP
    #define configINIT_TLS_BLOCK( xTLSBlock, px_ತುದಿ_Of_ರಾಶಿ )    esp_reent_init( &( xTLSBlock ) )
#ಅನ್ಯಥಾ /* CONFIG_FREERTOS_SMP */
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

#ಯದಿ CONFIG_FREERTOS_USE_TRACE_FACILITY
    /* Used by uxTaskGetSystemState(), and other trace facility functions */
    #define configUSE_TRACE_FACILITY    1
#endif /* CONFIG_FREERTOS_USE_TRACE_FACILITY */

/* -------------------- API Includes ----------------------- */

/* Todo: Reconcile INCLUDE_option differences (IDF-8186) */
#define INCLUDE_xTaskDelayUntil                 1
#define INCLUDE_ux_Task_Get_ರಾಶಿ_ಮೇಲಿನ_Water_Mark2    1

/* ------------------------------------------------ ESP-IDF Additions --------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */

#ifndef config_ISR_ರಾಶಿ_SIZE
    #define config_ISR_ರಾಶಿ_SIZE    ( CONFIG_FREERTOS_ISR_STACKSIZE )
#endif
