/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#సూచన once

/* RISC-V Architecture specific configuration. This file is included in the common FreeRTOSConfig.h. */

#అన్తర్భావ "sdkconfig.h"

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
#నిర్వచించు config_MAX_API_CALL_అంతరాయం_ప్రాధాన్యత          0

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

#నిర్వచితం_కాకపోతే config_ISR_దొంతర_కొలత
    #నిర్వచించు config_ISR_దొంతర_కొలత    ( CONFIG_FREERTOS_ISR_STACKSIZE )
#అయితే_ముగింపు
