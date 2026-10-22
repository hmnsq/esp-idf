/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#لارښوونه once

/* RISC-V Architecture specific configuration. This file is included in the common FreeRTOSConfig.h. */

#ضمن "sdkconfig.h"

/* ------------------------------------------------- FreeRTOS Config ---------------------------------------------------
 * - All Vanilla FreeRTOS configuration goes into this section
 * ------------------------------------------------------------------------------------------------------------------ */

/* ------------------ Scheduler Related -------------------- */

#که_تعریف_شوی CONFIG_FREERTOS_OPTIMIZED_SCHEDULER
    #تعریف_کړه config_USE_پورټ_OPTIMISED_TASK_SELECTION    1
#وإلا
    #تعریف_کړه config_USE_پورټ_OPTIMISED_TASK_SELECTION    0
#د_که_پای /* CONFIG_FREERTOS_OPTIMIZED_SCHEDULER */
#تعریف_کړه config_اصغري_ډېری_اندازه                       ( CONFIG_FREERTOS_IDLE_TASK_STACKSIZE + config_ډېری_OVERHEAD_ټول )
#تعریف_کړه config_MAX_API_CALL_مداخله_لومړیتوب          0

/* ---------------- Amazon SMP FreeRTOS -------------------- */

#إذا CONFIG_FREERTOS_SMP

/* This is always enabled to call IDF style idle hooks, by can be "--Wl,--wrap"
 * if users enable CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK. */
    #تعریف_کړه configUSE_PASSIVE_IDLE_HOOK    1
#د_که_پای

/* ----------------------- System -------------------------- */

#إذا CONFIG_LIBC_NEWLIB
#تعریف_کړه configUSE_NEWLIB_REENTRANT    1
#وإلا
#تعریف_کړه configUSE_NEWLIB_REENTRANT    0
#د_که_پای

/* - FreeRTOS provides default for configTLS_BLOCK_TYPE.
 * - We simply provide our own INIT and DEINIT functions
 * - We set "SET" to a blank macro since there is no need to set the reentrancy
 * pointer. All newlib functions calls __getreent. */
#إذا CONFIG_FREERTOS_SMP
    #تعریف_کړه config_INIT_TLS_بلاک( xTLSBlock, px_سر_د_ډېری )    esp_reent_init( &( xTLSBlock ) )
#وإلا /* CONFIG_FREERTOS_SMP */
    #تعریف_کړه config_INIT_TLS_بلاک( xTLSBlock )                  esp_reent_init( &( xTLSBlock ) )
#د_که_پای /* CONFIG_FREERTOS_SMP */
#تعریف_کړه config_سېټ_TLS_بلاک( xTLSBlock )
#تعریف_کړه config_DEINIT_TLS_بلاک( xTLSBlock )                    _reclaim_reent( &( xTLSBlock ) )

#تعریف_کړه configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H    1

/* ----------------------- Memory  ------------------------- */

/* This isn't used as FreeRTOS will only allocate from internal memory (see
 * heap_idf.c). We simply define this macro to span all non-statically-allocated
 * shared RAM. */
#تعریف_کړه config_ټول_ډېرۍ_اندازه    ( &ډېرۍ_پای_ټکی - &ډېرۍ_پیل )

/* ------------------- Run-time Stats ---------------------- */

#إذا CONFIG_FREERTOS_USE_TRACE_FACILITY
    /* Used by uxTaskGetSystemState(), and other trace facility functions */
    #تعریف_کړه configUSE_TRACE_FACILITY    1
#د_که_پای /* CONFIG_FREERTOS_USE_TRACE_FACILITY */

/* -------------------- API Includes ----------------------- */

/* Todo: Reconcile INCLUDE_option differences (IDF-8186) */
#تعریف_کړه INCLUDE_xTaskDelayUntil                 1
#تعریف_کړه INCLUDE_ux_Task_Get_ډېری_لوړ_Water_Mark2    1

/* ------------------------------------------------ ESP-IDF Additions --------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */

#که_نه_تعریف_شوی config_ISR_ډېری_اندازه
    #تعریف_کړه config_ISR_ډېری_اندازه    ( CONFIG_FREERTOS_ISR_STACKSIZE )
#د_که_پای
