/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#هدايت once

/* RISC-V Architecture specific configuration. This file is included in the common FreeRTOSConfig.h. */

#ضمن "sdkconfig.h"

/* ------------------------------------------------- FreeRTOS Config ---------------------------------------------------
 * - All Vanilla FreeRTOS configuration goes into this section
 * ------------------------------------------------------------------------------------------------------------------ */

/* ------------------ Scheduler Related -------------------- */

#جيڪڏهن_وصف_ٿيل CONFIG_FREERTOS_OPTIMIZED_SCHEDULER
    #وصف_ڪريو config_USE_پورٽ_OPTIMISED_TASK_SELECTION    1
#وإلا
    #وصف_ڪريو config_USE_پورٽ_OPTIMISED_TASK_SELECTION    0
#جيڪڏهن_پورو /* CONFIG_FREERTOS_OPTIMIZED_SCHEDULER */
#وصف_ڪريو config_گهٽتر_ڍير_سائيز                       ( CONFIG_FREERTOS_IDLE_TASK_STACKSIZE + config_ڍير_OVERHEAD_ڪل )
#وصف_ڪريو config_MAX_API_CALL_مداخلت_ترجيح          0

/* ---------------- Amazon SMP FreeRTOS -------------------- */

#إذا CONFIG_FREERTOS_SMP

/* This is always enabled to call IDF style idle hooks, by can be "--Wl,--wrap"
 * if users enable CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK. */
    #وصف_ڪريو configUSE_PASSIVE_IDLE_HOOK    1
#جيڪڏهن_پورو

/* ----------------------- System -------------------------- */

#إذا CONFIG_LIBC_NEWLIB
#وصف_ڪريو configUSE_NEWLIB_REENTRANT    1
#وإلا
#وصف_ڪريو configUSE_NEWLIB_REENTRANT    0
#جيڪڏهن_پورو

/* - FreeRTOS provides default for configTLS_BLOCK_TYPE.
 * - We simply provide our own INIT and DEINIT functions
 * - We set "SET" to a blank macro since there is no need to set the reentrancy
 * pointer. All newlib functions calls __getreent. */
#إذا CONFIG_FREERTOS_SMP
    #وصف_ڪريو config_INIT_TLS_بلاڪ( xTLSBlock, px_چوٽي_جو_ڍير )    esp_reent_init( &( xTLSBlock ) )
#وإلا /* CONFIG_FREERTOS_SMP */
    #وصف_ڪريو config_INIT_TLS_بلاڪ( xTLSBlock )                  esp_reent_init( &( xTLSBlock ) )
#جيڪڏهن_پورو /* CONFIG_FREERTOS_SMP */
#وصف_ڪريو config_سيٽ_TLS_بلاڪ( xTLSBlock )
#وصف_ڪريو config_DEINIT_TLS_بلاڪ( xTLSBlock )                    _reclaim_reent( &( xTLSBlock ) )

#وصف_ڪريو configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H    1

/* ----------------------- Memory  ------------------------- */

/* This isn't used as FreeRTOS will only allocate from internal memory (see
 * heap_idf.c). We simply define this macro to span all non-statically-allocated
 * shared RAM. */
#وصف_ڪريو config_ڪل_ڍير_سائيز    ( &ڍير_پڇاڙي - &ڍير_شروعات )

/* ------------------- Run-time Stats ---------------------- */

#إذا CONFIG_FREERTOS_USE_TRACE_FACILITY
    /* Used by uxTaskGetSystemState(), and other trace facility functions */
    #وصف_ڪريو configUSE_TRACE_FACILITY    1
#جيڪڏهن_پورو /* CONFIG_FREERTOS_USE_TRACE_FACILITY */

/* -------------------- API Includes ----------------------- */

/* Todo: Reconcile INCLUDE_option differences (IDF-8186) */
#وصف_ڪريو INCLUDE_xTaskDelayUntil                 1
#وصف_ڪريو INCLUDE_ux_Task_Get_ڍير_مٿاهون_Water_Mark2    1

/* ------------------------------------------------ ESP-IDF Additions --------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */

#جيڪڏهن_وصف_نه_ٿيل config_ISR_ڍير_سائيز
    #وصف_ڪريو config_ISR_ڍير_سائيز    ( CONFIG_FREERTOS_ISR_STACKSIZE )
#جيڪڏهن_پورو
