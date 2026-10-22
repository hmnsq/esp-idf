/*
 * SPDX-FileCopyrightText: 2017-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ <stdint.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ "FreeRTOS.h"
#అన్తర్భావ "task.h"
#ఒకవేళ ( !CONFIG_FREERTOS_SMP && ( config_అంకె_CORES > 1 ) )
/* Required for xTaskIncrementTickOtherCores() */
#అన్తర్భావ "esp_private/freertos_idf_additions_priv.h"
#అయితే_ముగింపు /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */

#ఒకవేళ CONFIG_FREERTOS_SYSTICK_ఉపయోగిస్తుంది_CCOUNT
#ఒకవేళ CONFIG_FREERTOS_CORETIMER_0
#నిర్వచించు SYSTICK_INTR_ID     (ETS_అంతర్గత_TIMER0_INTR_ఆధారం + ETS_అంతర్గత_INTR_ఆధారం_OFF)
#లేకపోతే /* CONFIG_FREERTOS_CORETIMER_1 */
#నిర్వచించు SYSTICK_INTR_ID     (ETS_అంతర్గత_TIMER1_INTR_ఆధారం + ETS_అంతర్గత_INTR_ఆధారం_OFF)
#అయితే_ముగింపు
#లేకపోతే /* CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER */
#నిర్వచించు SYSTICK_INTR_ID     (ETS_SYSTIMER_TARGET0_INTR_ఆధారం)
#అయితే_ముగింపు /* CONFIG_FREERTOS_SYSTICK_USES_CCOUNT */

పునాది_Type_t x_పోర్టు_Sys_Tick_Handler(శూన్య);

/* --------------------------------------------- SYSTIMER Implementation -----------------------------------------------
 * Implementation of a tick interrupt using the SYSTIMER perpiheral
 * Todo: Abstract setup and operation in a separate SYSTIMER driver (IDF-6976)
 * ------------------------------------------------------------------------------------------------------------------ */

#ఒకవేళ CONFIG_FREERTOS_SYSTICK_ఉపయోగిస్తుంది_SYSTIMER

#అన్తర్భావ "soc/periph_defs.h"
#అన్తర్భావ "hal/systimer_hal.h"
#అన్తర్భావ "hal/systimer_ll.h"
#అన్తర్భావ "esp_err.h"
#అన్తర్భావ "esp_intr_alloc.h"
#అన్తర్భావ "esp_private/systimer.h"
#అన్తర్భావ "esp_private/periph_ctrl.h"
#నిర్వచితమైతే CONFIG_PM_TRACE
#అన్తర్భావ "esp_private/pm_trace.h"
#అయితే_ముగింపు //CONFIG_PM_TRACE

_Static_assert(SOC_CPU_CORES_అంకె <= SYSTIMER_LL_ALARM_అంకె - 1, "the number of cores must match the number of core alarms in SYSTIMER");

శూన్య SysTickIsrHandler(శూన్య *arg);

స్థిర uint32_t s_handled_systicks[config_అంకె_CORES] = { 0 };

/* Systimer HAL layer object */
స్థిర systimer_hal_context_t systimer_hal;

/**
 * @brief Set up the systimer peripheral to generate the tick interrupt
 *
 * Both timer alarms are configured in periodic mode.
 * It is done at the same time so SysTicks for both CPUs occur at the same time or very close.
 * Shifts a time of triggering interrupts for core 0 and core 1.
 */
శూన్య v_Systimer_అమరిక(శూన్య)
{
    గుర్తులేని cpuid = x_పోర్టు_Get_Core_ID();
#నిర్వచితమైతే CONFIG_FREERTOS_CORETIMER_SYSTIMER_LVL3
    స్థిరరాశి గుర్తులేని level = ESP_INTR_FLAG_LEVEL3;
#లేకపోతే
    స్థిరరాశి గుర్తులేని level = ESP_INTR_FLAG_LEVEL1;
#అయితే_ముగింపు
    /* set system timer interrupt vector */

    ESP_ERROR_CHECK(esp_intr_alloc(ETS_SYSTIMER_TARGET0_INTR_ఆధారం + cpuid, ESP_INTR_FLAG_IRAM | level, SysTickIsrHandler, &systimer_hal, ఖాళీ));

    ఒకవేళ (cpuid == 0) {
        PERIPH_RCC_ACQUIRE_ATOMIC(PERIPH_SYSTIMER_MODULE, ref_లెక్క) {
            ఒకవేళ (ref_లెక్క == 0) {
                systimer_ll_enable_bus_clock(true);
                systimer_ll_enable_sys_clock(true);
                systimer_ll_reset_రిజిస్టరు();
            }
        }
        systimer_hal_init(&systimer_hal);
        systimer_hal_tick_రేటు_ops_t ops = {
            .ticks_గమ్యం_us = systimer_ticks_గమ్యం_us,
            .us_గమ్యం_ticks = systimer_us_గమ్యం_ticks,
        };
        systimer_hal_సమితి_tick_రేటు_ops(&systimer_hal, &ops);
        systimer_ll_సమితి_counter_విలువ(systimer_hal.dev, SYSTIMER_COUNTER_OS_TICK, 0);
        systimer_ll_apply_counter_విలువ(systimer_hal.dev, SYSTIMER_COUNTER_OS_TICK);

        కొరకు (cpuid = 0; cpuid < SOC_CPU_CORES_అంకె; cpuid++) {
            // Set stall option and alarm mode to default state. Below they will be set to a required state.
            systimer_hal_counter_can_stall_by_cpu(&systimer_hal, SYSTIMER_COUNTER_OS_TICK, cpuid, false);
            uint32_t alarm_id = SYSTIMER_ALARM_OS_TICK_CORE0 + cpuid;
            systimer_hal_select_alarm_బహుళకం(&systimer_hal, alarm_id, SYSTIMER_ALARM_బహుళకం_ONESHOT);
        }

        కొరకు (cpuid = 0; cpuid < config_అంకె_CORES; ++cpuid) {
            uint32_t alarm_id = SYSTIMER_ALARM_OS_TICK_CORE0 + cpuid;

            /* configure the timer */
            systimer_hal_connect_alarm_counter(&systimer_hal, alarm_id, SYSTIMER_COUNTER_OS_TICK);
            systimer_hal_సమితి_alarm_period(&systimer_hal, alarm_id, 1000000UL / CONFIG_FREERTOS_HZ);
            systimer_hal_select_alarm_బహుళకం(&systimer_hal, alarm_id, SYSTIMER_ALARM_బహుళకం_PERIOD);
            systimer_hal_counter_can_stall_by_cpu(&systimer_hal, SYSTIMER_COUNTER_OS_TICK, cpuid, true);
            ఒకవేళ (cpuid == 0) {
                systimer_hal_enable_alarm_int(&systimer_hal, alarm_id);
                systimer_hal_enable_counter(&systimer_hal, SYSTIMER_COUNTER_OS_TICK);
#నిర్వచితం_కాకపోతే CONFIG_FREERTOS_UNICORE
                // SysTick of core 0 and core 1 are shifted by half of period
                systimer_hal_counter_విలువ_advance(&systimer_hal, SYSTIMER_COUNTER_OS_TICK, 1000000UL / CONFIG_FREERTOS_HZ / 2);
#అయితే_ముగింపు
            }
        }
    } లేకపోతే {
        uint32_t alarm_id = SYSTIMER_ALARM_OS_TICK_CORE0 + cpuid;
        systimer_hal_enable_alarm_int(&systimer_hal, alarm_id);
    }
}

/**
 * @brief Systimer interrupt handler.
 *
 * The Systimer interrupt for SysTick works in periodic mode no need to calc the next alarm.
 * If a timer interrupt is ever serviced more than one tick late, it is necessary to process multiple ticks.
 */
శూన్య SysTickIsrHandler(శూన్య *arg)
{
    uint32_t cpuid = x_పోర్టు_Get_Core_ID();
    systimer_hal_context_t *systimer_hal = (systimer_hal_context_t *)arg;
#నిర్వచితమైతే CONFIG_PM_TRACE
    ESP_PM_TRACE_ENTER(TICK, cpuid);
#అయితే_ముగింపు

    uint32_t alarm_id = SYSTIMER_ALARM_OS_TICK_CORE0 + cpuid;
    చేయి {
        systimer_ll_clear_alarm_int(systimer_hal->dev, alarm_id);

        uint32_t diff = systimer_hal_get_counter_విలువ(systimer_hal, SYSTIMER_COUNTER_OS_TICK) / systimer_ll_get_alarm_period(systimer_hal->dev, alarm_id) - s_handled_systicks[cpuid];
        ఒకవేళ (diff > 0) {
            ఒకవేళ (s_handled_systicks[cpuid] == 0) {
                s_handled_systicks[cpuid] = diff;
                diff = 1;
            } లేకపోతే {
                s_handled_systicks[cpuid] += diff;
            }

            చేయి {
                x_పోర్టు_Sys_Tick_Handler();
            } అయితే (--diff);
        }
    } అయితే (systimer_ll_is_alarm_int_fired(systimer_hal->dev, alarm_id));

#నిర్వచితమైతే CONFIG_PM_TRACE
    ESP_PM_TRACE_EXIT(TICK, cpuid);
#అయితే_ముగింపు
}

uint32_t x_పోర్టు_Sys_Tick_Claim_Elapsed_Ticks(పూర్ణాంకం cpu_id)
{
    uint32_t alarm_id = SYSTIMER_ALARM_OS_TICK_CORE0 + (uint32_t)cpu_id;
    uint32_t మొత్తం = systimer_hal_get_counter_విలువ(&systimer_hal, SYSTIMER_COUNTER_OS_TICK) / systimer_ll_get_alarm_period(systimer_hal.dev, alarm_id);
    uint32_t elapsed = మొత్తం - s_handled_systicks[cpu_id];
    ఒకవేళ (elapsed == 0) {
        తిరిగి 0;
    }
    s_handled_systicks[cpu_id] = మొత్తం;
    తిరిగి elapsed;
}
#అయితే_ముగింపు /* CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER */

/* ------------------------------------------------ Common Port Tick ---------------------------------------------------
 * Tick related functions common across all ports
 * ------------------------------------------------------------------------------------------------------------------ */

/**
 * @brief Initialize the tick interrupt timer
 *
 * - CCOUNT timer is used if CONFIG_FREERTOS_SYSTICK_USES_CCOUNT is set
 * - SYSTIMER is used if CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER is set
 */
శూన్య v_పోర్టు_అమరిక_Timer(శూన్య)
{
#ఒకవేళ CONFIG_FREERTOS_SYSTICK_ఉపయోగిస్తుంది_CCOUNT
    బాహ్య శూన్య _frxt_tick_timer_init(శూన్య);
    బాహ్య శూన్య xt_tick_భాజకం_init(శూన్య);
    /* Init the tick divisor value */
    xt_tick_భాజకం_init();
    _frxt_tick_timer_init();
#లేకపోతే /* CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER */
    v_Systimer_అమరిక();
#అయితే_ముగింపు /* CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER */
}

/**
 * @brief Handler of SysTick
 *
 * The function is called from:
 *  - _frxt_timer_int for xtensa with CONFIG_FREERTOS_SYSTICK_USES_CCOUNT
 *  - SysTickIsrHandler for xtensa with CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER
 *  - SysTickIsrHandler for riscv
 */
పునాది_Type_t x_పోర్టు_Sys_Tick_Handler(శూన్య)
{
#ఒకవేళ configBENCHMARK
    portbenchmarkIntLatency();
#అయితే_ముగింపు //configBENCHMARK
    traceISR_ENTER(SYSTICK_INTR_ID);

    // Call IDF Tick Hook
    బాహ్య శూన్య esp_vApplicationTickHook(శూన్య);
    esp_vApplicationTickHook();

    // Call FreeRTOS Increment tick function
    పునాది_Type_t xSwitchRequired;
#ఒకవేళ CONFIG_FREERTOS_SMP
    UBaseType_t ux_Saved_స్థితి = taskENTER_CRITICAL_FROM_ISR();
    // Amazon SMP FreeRTOS requires that only core 0 calls xTaskIncrementTick()
#ఒకవేళ ( config_అంకె_CORES > 1 )
    ఒకవేళ (పోర్టు_GET_CORE_ID() == 0) {
        xSwitchRequired = xTaskIncrementTick();
    } లేకపోతే {
        xSwitchRequired = pdFALSE;
    }
#లేకపోతే /* configNUM_CORES > 1 */
    xSwitchRequired = xTaskIncrementTick();
#అయితే_ముగింపు /* configNUM_CORES > 1 */
    taskEXIT_CRITICAL_FROM_ISR(ux_Saved_స్థితి);
#లేకపోతే /* !CONFIG_FREERTOS_SMP */
#ఒకవేళ ( config_అంకె_CORES > 1 )
    /*
    Multi-core IDF FreeRTOS requires that...
        - core 0 calls xTaskIncrementTick()
        - core 1 calls xTaskIncrementTickOtherCores()
    */
    ఒకవేళ (x_పోర్టు_Get_Core_ID() == 0) {
        xSwitchRequired = xTaskIncrementTick();
    } లేకపోతే {
        xSwitchRequired = xTaskIncrementTickOtherCores();
    }
#లేకపోతే /* configNUM_CORES > 1 */
    /*
    Vanilla (single core) FreeRTOS expects that xTaskIncrementTick() cannot be interrupted (i.e., no nested
    interrupts). Thus we have to disable interrupts before calling it.
    */
    UBaseType_t ux_Saved_అంతరాయం_స్థితి = పోర్టు_సమితి_అంతరాయం_ముసుగు_FROM_ISR();
    xSwitchRequired = xTaskIncrementTick();
    పోర్టు_CLEAR_అంతరాయం_ముసుగు_FROM_ISR(ux_Saved_అంతరాయం_స్థితి);
#అయితే_ముగింపు /* configNUM_CORES > 1 */
#అయితే_ముగింపు /* !CONFIG_FREERTOS_SMP */

    // Check if yield is required
    ఒకవేళ (xSwitchRequired != pdFALSE) {
        పోర్టు_YIELD_FROM_ISR();
    } లేకపోతే {
        traceISR_EXIT();
    }
    తిరిగి xSwitchRequired;
}
