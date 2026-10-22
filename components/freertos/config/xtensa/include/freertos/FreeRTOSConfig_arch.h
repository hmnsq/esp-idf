/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#సూచన once

/* Xtensa Architecture specific configuration. This file is included in the common FreeRTOSConfig.h. */

#అన్తర్భావ "sdkconfig.h"

/* Required for configuration-dependent settings. */
#అన్తర్భావ "xtensa_config.h"

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

#నిర్వచించు configXT_SIMULATOR          0
#నిర్వచించు configXT_BOARD              1                           /* Board mode */
#ఒకవేళ CONFIG_FREERTOS_CORETIMER_0
    #నిర్వచించు config_XT_TIMER_సూచీ    0
#లేదావేళ CONFIG_FREERTOS_CORETIMER_1
    #నిర్వచించు config_XT_TIMER_సూచీ    1
#అయితే_ముగింపు
#నిర్వచించు configXT_INTEXC_HOOKS       0

#నిర్వచించు configBENCHMARK             0

/* ------------------------------------------------- FreeRTOS Config ---------------------------------------------------
 * - All Vanilla FreeRTOS configuration goes into this section
 * ------------------------------------------------------------------------------------------------------------------ */

/* ------------------ Scheduler Related -------------------- */

#నిర్వచితమైతే CONFIG_FREERTOS_OPTIMIZED_SCHEDULER
    #నిర్వచించు config_USE_పోర్టు_OPTIMISED_TASK_SELECTION    1
#లేకపోతే
    #నిర్వచించు config_USE_పోర్టు_OPTIMISED_TASK_SELECTION    0
#అయితే_ముగింపు /* CONFIG_FREERTOS_OPTIMIZED_SCHEDULER */
#నిర్వచించు config_కనీస_దొంతర_కొలత                       ( CONFIG_FREERTOS_IDLE_TASK_STACKSIZE + config_దొంతర_OVERHEAD_మొత్తం )
#నిర్వచించు config_MAX_API_CALL_అంతరాయం_ప్రాధాన్యత          XCHAL_EXCM_LEVEL

/* ---------------- Amazon SMP FreeRTOS -------------------- */

#ఒకవేళ CONFIG_FREERTOS_SMP

/* This is always enabled to call IDF style idle hooks, by can be "--Wl,--wrap"
 * if users enable CONFIG_FREERTOS_USE_PASSIVE_IDLE_HOOK. */
    #నిర్వచించు configUSE_PASSIVE_IDLE_HOOK    1
#అయితే_ముగింపు

/* ----------------------- System -------------------------- */

#ఒకవేళ CONFIG_LIBC_NEWLIB
#నిర్వచించు configUSE_NEWLIB_REENTRANT    1
#లేకపోతే
#నిర్వచించు configUSE_NEWLIB_REENTRANT    0
#అయితే_ముగింపు

/* - FreeRTOS provides default for configTLS_BLOCK_TYPE.
 * - We simply provide our own INIT and DEINIT functions
 * - We set "SET" to a blank macro since there is no need to set the reentrancy
 * pointer. All newlib functions calls __getreent. */
#ఒకవేళ CONFIG_FREERTOS_SMP
    #నిర్వచించు config_INIT_TLS_ఖండం( xTLSBlock, px_శిఖరం_యొక్క_దొంతర )    esp_reent_init( &( xTLSBlock ) )
#లేకపోతే /* CONFIG_FREERTOS_SMP */
    #నిర్వచించు config_INIT_TLS_ఖండం( xTLSBlock )                  esp_reent_init( &( xTLSBlock ) )
#అయితే_ముగింపు /* CONFIG_FREERTOS_SMP */
#నిర్వచించు config_సమితి_TLS_ఖండం( xTLSBlock )
#నిర్వచించు config_DEINIT_TLS_ఖండం( xTLSBlock )                    _reclaim_reent( &( xTLSBlock ) )

#నిర్వచించు configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H    1

/* ----------------------- Memory  ------------------------- */

/* This isn't used as FreeRTOS will only allocate from internal memory (see
 * heap_idf.c). We simply define this macro to span all non-statically-allocated
 * shared RAM. */
#నిర్వచించు config_మొత్తం_కుప్ప_కొలత    ( &కుప్ప_చివర - &కుప్ప_ప్రారంభం )

/* ------------------- Run-time Stats ---------------------- */

#ఒకవేళ CONFIG_FREERTOS_USE_TRACE_FACILITY
    /* Used by uxTaskGetSystemState(), and other trace facility functions */
    #నిర్వచించు configUSE_TRACE_FACILITY    1
#అయితే_ముగింపు /* CONFIG_FREERTOS_USE_TRACE_FACILITY */

/* -------------------- API Includes ----------------------- */

/* Todo: Reconcile INCLUDE_option differences (IDF-8186) */
#నిర్వచించు INCLUDE_xTaskDelayUntil                 1
#నిర్వచించు INCLUDE_ux_Task_Get_దొంతర_ఎగువ_Water_Mark2    1

/* ------------------------------------------------ ESP-IDF Additions --------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */

/* The Xtensa port uses a separate interrupt stack. Adjust the stack size
 * to suit the needs of your specific application.
 * Size needs to be aligned to the stack increment, since the location of
 * the stack for the 2nd CPU will be calculated using configISR_STACK_SIZE.
 */
#నిర్వచించు config_దొంతర_ALIGNMENT       16
#నిర్వచితం_కాకపోతే config_ISR_దొంతర_కొలత
    #నిర్వచించు config_ISR_దొంతర_కొలత    ( ( CONFIG_FREERTOS_ISR_STACKSIZE + config_దొంతర_ALIGNMENT - 1 ) & ( ~( config_దొంతర_ALIGNMENT - 1 ) ) )
#అయితే_ముగింపు
