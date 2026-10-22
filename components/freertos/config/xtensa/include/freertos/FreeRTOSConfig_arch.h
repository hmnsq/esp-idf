/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ہدایت once

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

#تعریف_کرو configXT_SIMULATOR          0
#تعریف_کرو configXT_BOARD              1                           /* Board mode */
#إذا CONFIG_FREERTOS_CORETIMER_0
    #تعریف_کرو config_XT_TIMER_اشاریہ    0
#وإلاإذا CONFIG_FREERTOS_CORETIMER_1
    #تعریف_کرو config_XT_TIMER_اشاریہ    1
#جے_انت
#تعریف_کرو configXT_INTEXC_HOOKS       0

#تعریف_کرو configBENCHMARK             0

/* ------------------------------------------------- FreeRTOS Config ---------------------------------------------------
 * - All Vanilla FreeRTOS configuration goes into this section
 * ------------------------------------------------------------------------------------------------------------------ */

/* ------------------ Scheduler Related -------------------- */

#جے_تعریف_شدہ CONFIG_FREERTOS_OPTIMIZED_SCHEDULER
    #تعریف_کرو config_USE_پورٹ_OPTIMISED_TASK_SELECTION    1
#وإلا
    #تعریف_کرو config_USE_پورٹ_OPTIMISED_TASK_SELECTION    0
#جے_انت /* CONFIG_FREERTOS_OPTIMIZED_SCHEDULER */
#تعریف_کرو config_اقل_ڈھیر_ناپ                       ( CONFIG_FREERTOS_IDLE_TASK_STACKSIZE + config_ڈھیر_OVERHEAD_کل )
#تعریف_کرو config_MAX_API_CALL_رکاوٹ_ترجیح          XCHAL_EXCM_LEVEL

/* ---------------- Amazon SMP FreeRTOS -------------------- */

#إذا CONFIG_FREERTOS_SMP

/* This is always enabled to call IDF style idle hooks, by can be "--Wl,--wrap"
 * if users enable CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK. */
    #تعریف_کرو configUSE_PASSIVE_IDLE_HOOK    1
#جے_انت

/* ----------------------- System -------------------------- */

#إذا CONFIG_LIBC_NEWLIB
#تعریف_کرو configUSE_NEWLIB_REENTRANT    1
#وإلا
#تعریف_کرو configUSE_NEWLIB_REENTRANT    0
#جے_انت

/* - FreeRTOS provides default for configTLS_BLOCK_TYPE.
 * - We simply provide our own INIT and DEINIT functions
 * - We set "SET" to a blank macro since there is no need to set the reentrancy
 * pointer. All newlib functions calls __getreent. */
#إذا CONFIG_FREERTOS_SMP
    #تعریف_کرو config_INIT_TLS_بلاک( xTLSBlock, px_چوٹی_دا_ڈھیر )    esp_reent_init( &( xTLSBlock ) )
#وإلا /* CONFIG_FREERTOS_SMP */
    #تعریف_کرو config_INIT_TLS_بلاک( xTLSBlock )                  esp_reent_init( &( xTLSBlock ) )
#جے_انت /* CONFIG_FREERTOS_SMP */
#تعریف_کرو config_سیٹ_TLS_بلاک( xTLSBlock )
#تعریف_کرو config_DEINIT_TLS_بلاک( xTLSBlock )                    _reclaim_reent( &( xTLSBlock ) )

#تعریف_کرو configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H    1

/* ----------------------- Memory  ------------------------- */

/* This isn't used as FreeRTOS will only allocate from internal memory (see
 * heap_idf.c). We simply define this macro to span all non-statically-allocated
 * shared RAM. */
#تعریف_کرو config_کل_ڈھیر_ناپ    ( &ڈھیر_سرا - &ڈھیر_شروع )

/* ------------------- Run-time Stats ---------------------- */

#إذا CONFIG_FREERTOS_USE_TRACE_FACILITY
    /* Used by uxTaskGetSystemState(), and other trace facility functions */
    #تعریف_کرو configUSE_TRACE_FACILITY    1
#جے_انت /* CONFIG_FREERTOS_USE_TRACE_FACILITY */

/* -------------------- API Includes ----------------------- */

/* Todo: Reconcile INCLUDE_option differences (IDF-8186) */
#تعریف_کرو INCLUDE_xTaskDelayUntil                 1
#تعریف_کرو INCLUDE_ux_Task_Get_ڈھیر_بلند_Water_Mark2    1

/* ------------------------------------------------ ESP-IDF Additions --------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */

/* The Xtensa port uses a separate interrupt stack. Adjust the stack size
 * to suit the needs of your specific application.
 * Size needs to be aligned to the stack increment, since the location of
 * the stack for the 2nd CPU will be calculated using configISR_STACK_SIZE.
 */
#تعریف_کرو config_ڈھیر_ALIGNMENT       16
#جے_بنا_تعریف config_ISR_ڈھیر_ناپ
    #تعریف_کرو config_ISR_ڈھیر_ناپ    ( ( CONFIG_FREERTOS_ISR_STACKSIZE + config_ڈھیر_ALIGNMENT - 1 ) & ( ~( config_ڈھیر_ALIGNMENT - 1 ) ) )
#جے_انت
