/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

/* RISC-V Architecture specific configuration. This file is included in the common FreeRTOSConfig.h. */

#অন্তর্ভাব "sdkconfig.h"

/* ------------------------------------------------- FreeRTOS Config ---------------------------------------------------
 * - All Vanilla FreeRTOS configuration goes into this section
 * ------------------------------------------------------------------------------------------------------------------ */

/* ------------------ Scheduler Related -------------------- */

#ifdef CONFIG_FREERTOS_OPTIMIZED_SCHEDULER
    #define configUSE_PORT_OPTIMISED_TASK_SELECTION    1
#অন্যথা
    #define configUSE_PORT_OPTIMISED_TASK_SELECTION    0
#endif /* CONFIG_FREERTOS_OPTIMIZED_SCHEDULER */
#define config_MINIMAL_স্তূপ_আকার                       ( CONFIG_FREERTOS_IDLE_TASK_STACKSIZE + config_স্তূপ_OVERHEAD_যোগ )
#define configMAX_API_CALL_INTERRUPT_PRIORITY          0

/* ---------------- Amazon SMP FreeRTOS -------------------- */

#যদি CONFIG_FREERTOS_SMP

/* This is always enabled to call IDF style idle hooks, by can be "--Wl,--wrap"
 * if users enable CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK. */
    #define configUSE_PASSIVE_IDLE_HOOK    1
#endif

/* ----------------------- System -------------------------- */

#যদি CONFIG_LIBC_NEWLIB
#define configUSE_NEWLIB_REENTRANT    1
#অন্যথা
#define configUSE_NEWLIB_REENTRANT    0
#endif

/* - FreeRTOS provides default for configTLS_BLOCK_TYPE.
 * - We simply provide our own INIT and DEINIT functions
 * - We set "SET" to a blank macro since there is no need to set the reentrancy
 * pointer. All newlib functions calls __getreent. */
#যদি CONFIG_FREERTOS_SMP
    #define configINIT_TLS_BLOCK( xTLSBlock, px_শীর্ষ_Of_স্তূপ )    esp_reent_init( &( xTLSBlock ) )
#অন্যথা /* CONFIG_FREERTOS_SMP */
    #define configINIT_TLS_BLOCK( xTLSBlock )                  esp_reent_init( &( xTLSBlock ) )
#endif /* CONFIG_FREERTOS_SMP */
#define configSET_TLS_BLOCK( xTLSBlock )
#define configDEINIT_TLS_BLOCK( xTLSBlock )                    _reclaim_reent( &( xTLSBlock ) )

#define configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H    1

/* ----------------------- Memory  ------------------------- */

/* This isn't used as FreeRTOS will only allocate from internal memory (see
 * heap_idf.c). We simply define this macro to span all non-statically-allocated
 * shared RAM. */
#define config_যোগ_HEAP_আকার    ( &_heap_end - &_heap_start )

/* ------------------- Run-time Stats ---------------------- */

#যদি CONFIG_FREERTOS_USE_TRACE_FACILITY
    /* Used by uxTaskGetSystemState(), and other trace facility functions */
    #define configUSE_TRACE_FACILITY    1
#endif /* CONFIG_FREERTOS_USE_TRACE_FACILITY */

/* -------------------- API Includes ----------------------- */

/* Todo: Reconcile INCLUDE_option differences (IDF-8186) */
#define INCLUDE_xTaskDelayUntil                 1
#define INCLUDE_ux_Task_Get_স্তূপ_উপরের_Water_Mark2    1

/* ------------------------------------------------ ESP-IDF Additions --------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */

#ifndef config_ISR_স্তূপ_আকার
    #define config_ISR_স্তূপ_আকার    ( CONFIG_FREERTOS_ISR_STACKSIZE )
#endif
