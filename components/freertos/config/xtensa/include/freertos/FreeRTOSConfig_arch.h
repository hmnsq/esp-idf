/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#निर्देशः once

/* Xtensa Architecture specific configuration. This file is included in the common FreeRTOSConfig.h. */

#अन्तर्भाव "sdkconfig.h"

/* Required for configuration-dependent settings. */
#अन्तर्भाव "xtensa_config.h"

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

#परिभाषय configXT_SIMULATOR          0
#परिभाषय configXT_BOARD              1                           /* Board mode */
#यदि CONFIG_FREERTOS_CORETIMER_0
    #परिभाषय config_XT_TIMER_सूचकांक    0
#नोचेद्यदि CONFIG_FREERTOS_CORETIMER_1
    #परिभाषय config_XT_TIMER_सूचकांक    1
#यदि_अन्तः
#परिभाषय configXT_INTEXC_HOOKS       0

#परिभाषय configBENCHMARK             0

/* ------------------------------------------------- FreeRTOS Config ---------------------------------------------------
 * - All Vanilla FreeRTOS configuration goes into this section
 * ------------------------------------------------------------------------------------------------------------------ */

/* ------------------ Scheduler Related -------------------- */

#यदि_परिभाषितम् CONFIG_FREERTOS_OPTIMIZED_SCHEDULER
    #परिभाषय config_USE_पत्तनम्_OPTIMISED_TASK_SELECTION    1
#अन्यथा
    #परिभाषय config_USE_पत्तनम्_OPTIMISED_TASK_SELECTION    0
#यदि_अन्तः /* CONFIG_FREERTOS_OPTIMIZED_SCHEDULER */
#परिभाषय config_अल्पिष्ठम्_राशिः_आकार                       ( CONFIG_FREERTOS_IDLE_TASK_STACKSIZE + config_राशिः_OVERHEAD_योग )
#परिभाषय config_MAX_API_CALL_व्यवधानम्_प्राथमिकता          XCHAL_EXCM_LEVEL

/* ---------------- Amazon SMP FreeRTOS -------------------- */

#यदि CONFIG_FREERTOS_SMP

/* This is always enabled to call IDF style idle hooks, by can be "--Wl,--wrap"
 * if users enable CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK. */
    #परिभाषय configUSE_PASSIVE_IDLE_HOOK    1
#यदि_अन्तः

/* ----------------------- System -------------------------- */

#यदि CONFIG_LIBC_NEWLIB
#परिभाषय configUSE_NEWLIB_REENTRANT    1
#अन्यथा
#परिभाषय configUSE_NEWLIB_REENTRANT    0
#यदि_अन्तः

/* - FreeRTOS provides default for configTLS_BLOCK_TYPE.
 * - We simply provide our own INIT and DEINIT functions
 * - We set "SET" to a blank macro since there is no need to set the reentrancy
 * pointer. All newlib functions calls __getreent. */
#यदि CONFIG_FREERTOS_SMP
    #परिभाषय config_INIT_TLS_खण्डम्( xTLSBlock, px_शिखरम्_सम्बन्धि_राशिः )    esp_reent_init( &( xTLSBlock ) )
#अन्यथा /* CONFIG_FREERTOS_SMP */
    #परिभाषय config_INIT_TLS_खण्डम्( xTLSBlock )                  esp_reent_init( &( xTLSBlock ) )
#यदि_अन्तः /* CONFIG_FREERTOS_SMP */
#परिभाषय config_समुच्चयः_TLS_खण्डम्( xTLSBlock )
#परिभाषय config_DEINIT_TLS_खण्डम्( xTLSBlock )                    _reclaim_reent( &( xTLSBlock ) )

#परिभाषय configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H    1

/* ----------------------- Memory  ------------------------- */

/* This isn't used as FreeRTOS will only allocate from internal memory (see
 * heap_idf.c). We simply define this macro to span all non-statically-allocated
 * shared RAM. */
#परिभाषय config_योग_HEAP_आकार    ( &heap_अन्तः - &_heap_start )

/* ------------------- Run-time Stats ---------------------- */

#यदि CONFIG_FREERTOS_USE_TRACE_FACILITY
    /* Used by uxTaskGetSystemState(), and other trace facility functions */
    #परिभाषय configUSE_TRACE_FACILITY    1
#यदि_अन्तः /* CONFIG_FREERTOS_USE_TRACE_FACILITY */

/* -------------------- API Includes ----------------------- */

/* Todo: Reconcile INCLUDE_option differences (IDF-8186) */
#परिभाषय INCLUDE_xTaskDelayUntil                 1
#परिभाषय INCLUDE_ux_Task_Get_राशिः_उच्चः_Water_Mark2    1

/* ------------------------------------------------ ESP-IDF Additions --------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */

/* The Xtensa port uses a separate interrupt stack. Adjust the stack size
 * to suit the needs of your specific application.
 * Size needs to be aligned to the stack increment, since the location of
 * the stack for the 2nd CPU will be calculated using configISR_STACK_SIZE.
 */
#परिभाषय config_राशिः_ALIGNMENT       16
#यदि_अपरिभाषितम् config_ISR_राशिः_आकार
    #परिभाषय config_ISR_राशिः_आकार    ( ( CONFIG_FREERTOS_ISR_STACKSIZE + config_राशिः_ALIGNMENT - 1 ) & ( ~( config_राशिः_ALIGNMENT - 1 ) ) )
#यदि_अन्तः
