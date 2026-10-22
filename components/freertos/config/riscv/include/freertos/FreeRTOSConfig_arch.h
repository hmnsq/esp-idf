/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਹਦਾਇਤ once

/* RISC-V Architecture specific configuration. This file is included in the common FreeRTOSConfig.h. */

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"

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
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_MAX_API_CALL_ਰੁਕਾਵਟ_ਤਰਜੀਹ          0

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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_ISR_ਢੇਰ_ਮਾਪ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_ISR_ਢੇਰ_ਮਾਪ    ( CONFIG_FREERTOS_ISR_STACKSIZE )
#ਜੇ_ਅੰਤ
