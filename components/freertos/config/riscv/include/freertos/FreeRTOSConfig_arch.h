/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#رهنمود once

/* RISC-V Architecture specific configuration. This file is included in the common FreeRTOSConfig.h. */

#ضمن "sdkconfig.h"

/* ------------------------------------------------- FreeRTOS Config ---------------------------------------------------
 * - All Vanilla FreeRTOS configuration goes into this section
 * ------------------------------------------------------------------------------------------------------------------ */

/* ------------------ Scheduler Related -------------------- */

#اگر_تعریف_شده CONFIG_FREERTOS_OPTIMIZED_SCHEDULER
    #تعریف_کن config_USE_پورت_OPTIMISED_TASK_SELECTION    1
#وإلا
    #تعریف_کن config_USE_پورت_OPTIMISED_TASK_SELECTION    0
#پایان_اگر /* CONFIG_FREERTOS_OPTIMIZED_SCHEDULER */
#تعریف_کن config_کمینه_پشته_اندازه                       ( CONFIG_FREERTOS_IDLE_TASK_STACKSIZE + config_پشته_OVERHEAD_مجموع )
#تعریف_کن config_MAX_API_CALL_وقفه_اولویت          0

/* ---------------- Amazon SMP FreeRTOS -------------------- */

#إذا CONFIG_FREERTOS_SMP

/* This is always enabled to call IDF style idle hooks, by can be "--Wl,--wrap"
 * if users enable CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK. */
    #تعریف_کن configUSE_PASSIVE_IDLE_HOOK    1
#پایان_اگر

/* ----------------------- System -------------------------- */

#إذا CONFIG_LIBC_NEWLIB
#تعریف_کن configUSE_NEWLIB_REENTRANT    1
#وإلا
#تعریف_کن configUSE_NEWLIB_REENTRANT    0
#پایان_اگر

/* - FreeRTOS provides default for configTLS_BLOCK_TYPE.
 * - We simply provide our own INIT and DEINIT functions
 * - We set "SET" to a blank macro since there is no need to set the reentrancy
 * pointer. All newlib functions calls __getreent. */
#إذا CONFIG_FREERTOS_SMP
    #تعریف_کن config_INIT_TLS_بلوک( xTLSBlock, px_بالاترین_از_پشته )    esp_reent_init( &( xTLSBlock ) )
#وإلا /* CONFIG_FREERTOS_SMP */
    #تعریف_کن config_INIT_TLS_بلوک( xTLSBlock )                  esp_reent_init( &( xTLSBlock ) )
#پایان_اگر /* CONFIG_FREERTOS_SMP */
#تعریف_کن config_مجموعه_TLS_بلوک( xTLSBlock )
#تعریف_کن config_DEINIT_TLS_بلوک( xTLSBlock )                    _reclaim_reent( &( xTLSBlock ) )

#تعریف_کن configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H    1

/* ----------------------- Memory  ------------------------- */

/* This isn't used as FreeRTOS will only allocate from internal memory (see
 * heap_idf.c). We simply define this macro to span all non-statically-allocated
 * shared RAM. */
#تعریف_کن config_مجموع_انباشته_اندازه    ( &انباشته_انتها - &انباشته_شروع )

/* ------------------- Run-time Stats ---------------------- */

#إذا CONFIG_FREERTOS_USE_TRACE_FACILITY
    /* Used by uxTaskGetSystemState(), and other trace facility functions */
    #تعریف_کن configUSE_TRACE_FACILITY    1
#پایان_اگر /* CONFIG_FREERTOS_USE_TRACE_FACILITY */

/* -------------------- API Includes ----------------------- */

/* Todo: Reconcile INCLUDE_option differences (IDF-8186) */
#تعریف_کن INCLUDE_xTaskDelayUntil                 1
#تعریف_کن INCLUDE_ux_Task_Get_پشته_بالا_Water_Mark2    1

/* ------------------------------------------------ ESP-IDF Additions --------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */

#اگر_تعریف_نشده config_ISR_پشته_اندازه
    #تعریف_کن config_ISR_پشته_اندازه    ( CONFIG_FREERTOS_ISR_STACKSIZE )
#پایان_اگر
