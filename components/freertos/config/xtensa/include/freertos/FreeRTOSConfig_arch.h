/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#आशय once

/* Xtensa Architecture specific configuration. This file is included in the common FreeRTOSConfig.h. */

#समावेश "sdkconfig.h"

/* Required for configuration-dependent settings. */
#समावेश "xtensa_config.h"

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

#घोषणा configXT_SIMULATOR          0
#घोषणा configXT_BOARD              1                           /* Board mode */
#अगर CONFIG_FREERTOS_CORETIMER_0
    #घोषणा config_XT_TIMER_सूचकांक    0
#या_अगर CONFIG_FREERTOS_CORETIMER_1
    #घोषणा config_XT_TIMER_सूचकांक    1
#पूर्ण_अगर
#घोषणा configXT_INTEXC_HOOKS       0

#घोषणा configBENCHMARK             0

/* ------------------------------------------------- FreeRTOS Config ---------------------------------------------------
 * - All Vanilla FreeRTOS configuration goes into this section
 * ------------------------------------------------------------------------------------------------------------------ */

/* ------------------ Scheduler Related -------------------- */

#अगर_घोषित CONFIG_FREERTOS_OPTIMIZED_SCHEDULER
    #घोषणा configUSE_PORT_OPTIMISED_TASK_SELECTION    1
#अन्यथा
    #घोषणा configUSE_PORT_OPTIMISED_TASK_SELECTION    0
#पूर्ण_अगर /* CONFIG_FREERTOS_OPTIMIZED_SCHEDULER */
#घोषणा config_MINIMAL_ढेर_आकार                       ( CONFIG_FREERTOS_IDLE_TASK_STACKSIZE + config_ढेर_OVERHEAD_योग )
#घोषणा configMAX_API_CALL_INTERRUPT_PRIORITY          XCHAL_EXCM_LEVEL

/* ---------------- Amazon SMP FreeRTOS -------------------- */

#अगर CONFIG_FREERTOS_SMP

/* This is always enabled to call IDF style idle hooks, by can be "--Wl,--wrap"
 * if users enable CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK. */
    #घोषणा configUSE_PASSIVE_IDLE_HOOK    1
#पूर्ण_अगर

/* ----------------------- System -------------------------- */

#अगर CONFIG_LIBC_NEWLIB
#घोषणा configUSE_NEWLIB_REENTRANT    1
#अन्यथा
#घोषणा configUSE_NEWLIB_REENTRANT    0
#पूर्ण_अगर

/* - FreeRTOS provides default for configTLS_BLOCK_TYPE.
 * - We simply provide our own INIT and DEINIT functions
 * - We set "SET" to a blank macro since there is no need to set the reentrancy
 * pointer. All newlib functions calls __getreent. */
#अगर CONFIG_FREERTOS_SMP
    #घोषणा configINIT_TLS_BLOCK( xTLSBlock, px_शीर्ष_Of_ढेर )    esp_reent_init( &( xTLSBlock ) )
#अन्यथा /* CONFIG_FREERTOS_SMP */
    #घोषणा configINIT_TLS_BLOCK( xTLSBlock )                  esp_reent_init( &( xTLSBlock ) )
#पूर्ण_अगर /* CONFIG_FREERTOS_SMP */
#घोषणा configSET_TLS_BLOCK( xTLSBlock )
#घोषणा configDEINIT_TLS_BLOCK( xTLSBlock )                    _reclaim_reent( &( xTLSBlock ) )

#घोषणा configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H    1

/* ----------------------- Memory  ------------------------- */

/* This isn't used as FreeRTOS will only allocate from internal memory (see
 * heap_idf.c). We simply define this macro to span all non-statically-allocated
 * shared RAM. */
#घोषणा config_योग_HEAP_आकार    ( &_heap_end - &_heap_start )

/* ------------------- Run-time Stats ---------------------- */

#अगर CONFIG_FREERTOS_USE_TRACE_FACILITY
    /* Used by uxTaskGetSystemState(), and other trace facility functions */
    #घोषणा configUSE_TRACE_FACILITY    1
#पूर्ण_अगर /* CONFIG_FREERTOS_USE_TRACE_FACILITY */

/* -------------------- API Includes ----------------------- */

/* Todo: Reconcile INCLUDE_option differences (IDF-8186) */
#घोषणा INCLUDE_xTaskDelayUntil                 1
#घोषणा INCLUDE_ux_Task_Get_ढेर_ऊपरी_Water_Mark2    1

/* ------------------------------------------------ ESP-IDF Additions --------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */

/* The Xtensa port uses a separate interrupt stack. Adjust the stack size
 * to suit the needs of your specific application.
 * Size needs to be aligned to the stack increment, since the location of
 * the stack for the 2nd CPU will be calculated using configISR_STACK_SIZE.
 */
#घोषणा config_ढेर_ALIGNMENT       16
#अगर_अघोषित config_ISR_ढेर_आकार
    #घोषणा config_ISR_ढेर_आकार    ( ( CONFIG_FREERTOS_ISR_STACKSIZE + config_ढेर_ALIGNMENT - 1 ) & ( ~( config_ढेर_ALIGNMENT - 1 ) ) )
#पूर्ण_अगर
