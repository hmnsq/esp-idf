/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#هدايت once

/* Xtensa Architecture specific configuration. This file is included in the common FreeRTOSConfig.h. */

#ضمن "sdkconfig.h"

/* Required for configuration-dependent settings. */
#ضمن "xtensa_config.h"

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

#وصف_ڪريو configXT_SIMULATOR          0
#وصف_ڪريو configXT_BOARD              1                           /* Board mode */
#إذا CONFIG_FREERTOS_CORETIMER_0
    #وصف_ڪريو config_XT_TIMER_اشاريو    0
#وإلاإذا CONFIG_FREERTOS_CORETIMER_1
    #وصف_ڪريو config_XT_TIMER_اشاريو    1
#جيڪڏهن_پورو
#وصف_ڪريو configXT_INTEXC_HOOKS       0

#وصف_ڪريو configBENCHMARK             0

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
#وصف_ڪريو config_MAX_API_CALL_مداخلت_ترجيح          XCHAL_EXCM_LEVEL

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

/* The Xtensa port uses a separate interrupt stack. Adjust the stack size
 * to suit the needs of your specific application.
 * Size needs to be aligned to the stack increment, since the location of
 * the stack for the 2nd CPU will be calculated using configISR_STACK_SIZE.
 */
#وصف_ڪريو config_ڍير_ALIGNMENT       16
#جيڪڏهن_وصف_نه_ٿيل config_ISR_ڍير_سائيز
    #وصف_ڪريو config_ISR_ڍير_سائيز    ( ( CONFIG_FREERTOS_ISR_STACKSIZE + config_ڍير_ALIGNMENT - 1 ) & ( ~( config_ڍير_ALIGNMENT - 1 ) ) )
#جيڪڏهن_پورو
