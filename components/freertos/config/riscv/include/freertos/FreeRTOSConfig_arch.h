/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ہدایت once

/* RISC-V Architecture specific configuration. This file is included in the common FreeRTOSConfig.h. */

#شامل "sdkconfig.h"

/* ------------------------------------------------- FreeRTOS Config ---------------------------------------------------
 * - All Vanilla FreeRTOS configuration goes into this section
 * ------------------------------------------------------------------------------------------------------------------ */

/* ------------------ Scheduler Related -------------------- */

#اگر_تعریف_شدہ CONFIG_FREERTOS_OPTIMIZED_SCHEDULER
    #تعریف_کرو config_USE_پورٹ_OPTIMISED_TASK_SELECTION    1
#ورنہ
    #تعریف_کرو config_USE_پورٹ_OPTIMISED_TASK_SELECTION    0
#اگر_ختم /* CONFIG_FREERTOS_OPTIMIZED_SCHEDULER */
#تعریف_کرو config_اقل_ڈھیر_سائز                       ( CONFIG_FREERTOS_IDLE_TASK_STACKSIZE + config_ڈھیر_OVERHEAD_کل )
#تعریف_کرو config_MAX_API_CALL_مداخلت_ترجیح          0

/* ---------------- Amazon SMP FreeRTOS -------------------- */

#اگر CONFIG_FREERTOS_SMP

/* This is always enabled to call IDF style idle hooks, by can be "--Wl,--wrap"
 * if users enable CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK. */
    #تعریف_کرو configUSE_PASSIVE_IDLE_HOOK    1
#اگر_ختم

/* ----------------------- System -------------------------- */

#اگر CONFIG_LIBC_NEWLIB
#تعریف_کرو configUSE_NEWLIB_REENTRANT    1
#ورنہ
#تعریف_کرو configUSE_NEWLIB_REENTRANT    0
#اگر_ختم

/* - FreeRTOS provides default for configTLS_BLOCK_TYPE.
 * - We simply provide our own INIT and DEINIT functions
 * - We set "SET" to a blank macro since there is no need to set the reentrancy
 * pointer. All newlib functions calls __getreent. */
#اگر CONFIG_FREERTOS_SMP
    #تعریف_کرو config_INIT_TLS_بلاک( xTLSBlock, px_چوٹی_کا_ڈھیر )    esp_reent_init( &( xTLSBlock ) )
#ورنہ /* CONFIG_FREERTOS_SMP */
    #تعریف_کرو config_INIT_TLS_بلاک( xTLSBlock )                  esp_reent_init( &( xTLSBlock ) )
#اگر_ختم /* CONFIG_FREERTOS_SMP */
#تعریف_کرو config_سیٹ_TLS_بلاک( xTLSBlock )
#تعریف_کرو config_DEINIT_TLS_بلاک( xTLSBlock )                    _reclaim_reent( &( xTLSBlock ) )

#تعریف_کرو configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H    1

/* ----------------------- Memory  ------------------------- */

/* This isn't used as FreeRTOS will only allocate from internal memory (see
 * heap_idf.c). We simply define this macro to span all non-statically-allocated
 * shared RAM. */
#تعریف_کرو config_کل_ڈھیر_سائز    ( &ڈھیر_انتہا - &ڈھیر_آغاز )

/* ------------------- Run-time Stats ---------------------- */

#اگر CONFIG_FREERTOS_USE_TRACE_FACILITY
    /* Used by uxTaskGetSystemState(), and other trace facility functions */
    #تعریف_کرو configUSE_TRACE_FACILITY    1
#اگر_ختم /* CONFIG_FREERTOS_USE_TRACE_FACILITY */

/* -------------------- API Includes ----------------------- */

/* Todo: Reconcile INCLUDE_option differences (IDF-8186) */
#تعریف_کرو INCLUDE_xTaskDelayUntil                 1
#تعریف_کرو INCLUDE_ux_Task_Get_ڈھیر_بلند_Water_Mark2    1

/* ------------------------------------------------ ESP-IDF Additions --------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */

#اگر_غیر_تعریف_شدہ config_ISR_ڈھیر_سائز
    #تعریف_کرو config_ISR_ڈھیر_سائز    ( CONFIG_FREERTOS_ISR_STACKSIZE )
#اگر_ختم
