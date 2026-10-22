/*
 * SPDX-FileCopyrightText: 2017-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن "sdkconfig.h"
#ضمن <stdint.h>
#ضمن <stdlib.h>
#ضمن "FreeRTOS.h"
#ضمن "task.h"
#إذا ( !CONFIG_FREERTOS_SMP && ( config_عدد_CORES > 1 ) )
/* Required for xTaskIncrementTickOtherCores() */
#ضمن "esp_private/freertos_idf_additions_priv.h"
#د_که_پای /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */

#إذا CONFIG_FREERTOS_SYSTICK_کاروي_CCOUNT
#إذا CONFIG_FREERTOS_CORETIMER_0
#تعریف_کړه SYSTICK_INTR_ID     (ETS_دننی_TIMER0_INTR_سرچینه + ETS_دننی_INTR_سرچینه_OFF)
#وإلا /* CONFIG_FREERTOS_CORETIMER_1 */
#تعریف_کړه SYSTICK_INTR_ID     (ETS_دننی_TIMER1_INTR_سرچینه + ETS_دننی_INTR_سرچینه_OFF)
#د_که_پای
#وإلا /* CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER */
#تعریف_کړه SYSTICK_INTR_ID     (ETS_SYSTIMER_TARGET0_INTR_سرچینه)
#د_که_پای /* CONFIG_FREERTOS_SYSTICK_USES_CCOUNT */

بنسټ_Type_t x_پورټ_Sys_Tick_Handler(فراغ);

/* --------------------------------------------- SYSTIMER Implementation -----------------------------------------------
 * Implementation of a tick interrupt using the SYSTIMER perpiheral
 * Todo: Abstract setup and operation in a separate SYSTIMER driver (IDF-6976)
 * ------------------------------------------------------------------------------------------------------------------ */

#إذا CONFIG_FREERTOS_SYSTICK_کاروي_SYSTIMER

#ضمن "soc/periph_defs.h"
#ضمن "hal/systimer_hal.h"
#ضمن "hal/systimer_ll.h"
#ضمن "esp_err.h"
#ضمن "esp_intr_alloc.h"
#ضمن "esp_private/systimer.h"
#ضمن "esp_private/periph_ctrl.h"
#که_تعریف_شوی CONFIG_PM_TRACE
#ضمن "esp_private/pm_trace.h"
#د_که_پای //CONFIG_PM_TRACE

_Static_assert(SOC_CPU_CORES_عدد <= SYSTIMER_LL_ALARM_عدد - 1, "the number of cores must match the number of core alarms in SYSTIMER");

فراغ SysTickIsrHandler(فراغ *arg);

ساكن uint32_t s_handled_systicks[config_عدد_CORES] = { 0 };

/* Systimer HAL layer object */
ساكن systimer_hal_context_t systimer_hal;

/**
 * @brief Set up the systimer peripheral to generate the tick interrupt
 *
 * Both timer alarms are configured in periodic mode.
 * It is done at the same time so SysTicks for both CPUs occur at the same time or very close.
 * Shifts a time of triggering interrupts for core 0 and core 1.
 */
فراغ v_Systimer_تیاری(فراغ)
{
    بینښه cpuid = x_پورټ_Get_Core_ID();
#که_تعریف_شوی CONFIG_FREERTOS_CORETIMER_SYSTIMER_LVL3
    ثابت بینښه level = ESP_INTR_FLAG_LEVEL3;
#وإلا
    ثابت بینښه level = ESP_INTR_FLAG_LEVEL1;
#د_که_پای
    /* set system timer interrupt vector */

    ESP_ERROR_CHECK(esp_intr_alloc(ETS_SYSTIMER_TARGET0_INTR_سرچینه + cpuid, ESP_INTR_FLAG_IRAM | level, SysTickIsrHandler, &systimer_hal, عدم));

    إذا (cpuid == 0) {
        PERIPH_RCC_ACQUIRE_ATOMIC(PERIPH_SYSTIMER_MODULE, ref_شمېر) {
            إذا (ref_شمېر == 0) {
                systimer_ll_enable_bus_clock(true);
                systimer_ll_enable_sys_clock(true);
                systimer_ll_reset_ثبتوونکی();
            }
        }
        systimer_hal_init(&systimer_hal);
        systimer_hal_tick_شرح_ops_t ops = {
            .ticks_منزل_us = systimer_ticks_منزل_us,
            .us_منزل_ticks = systimer_us_منزل_ticks,
        };
        systimer_hal_سېټ_tick_شرح_ops(&systimer_hal, &ops);
        systimer_ll_سېټ_counter_ارزښت(systimer_hal.dev, SYSTIMER_COUNTER_OS_TICK, 0);
        systimer_ll_apply_counter_ارزښت(systimer_hal.dev, SYSTIMER_COUNTER_OS_TICK);

        لكل (cpuid = 0; cpuid < SOC_CPU_CORES_عدد; cpuid++) {
            // Set stall option and alarm mode to default state. Below they will be set to a required state.
            systimer_hal_counter_can_stall_by_cpu(&systimer_hal, SYSTIMER_COUNTER_OS_TICK, cpuid, false);
            uint32_t alarm_id = SYSTIMER_ALARM_OS_TICK_CORE0 + cpuid;
            systimer_hal_select_alarm_ډېرپېښ(&systimer_hal, alarm_id, SYSTIMER_ALARM_ډېرپېښ_ONESHOT);
        }

        لكل (cpuid = 0; cpuid < config_عدد_CORES; ++cpuid) {
            uint32_t alarm_id = SYSTIMER_ALARM_OS_TICK_CORE0 + cpuid;

            /* configure the timer */
            systimer_hal_connect_alarm_counter(&systimer_hal, alarm_id, SYSTIMER_COUNTER_OS_TICK);
            systimer_hal_سېټ_alarm_period(&systimer_hal, alarm_id, 1000000UL / CONFIG_FREERTOS_HZ);
            systimer_hal_select_alarm_ډېرپېښ(&systimer_hal, alarm_id, SYSTIMER_ALARM_ډېرپېښ_PERIOD);
            systimer_hal_counter_can_stall_by_cpu(&systimer_hal, SYSTIMER_COUNTER_OS_TICK, cpuid, true);
            إذا (cpuid == 0) {
                systimer_hal_enable_alarm_int(&systimer_hal, alarm_id);
                systimer_hal_enable_counter(&systimer_hal, SYSTIMER_COUNTER_OS_TICK);
#که_نه_تعریف_شوی CONFIG_FREERTOS_UNICORE
                // SysTick of core 0 and core 1 are shifted by half of period
                systimer_hal_counter_ارزښت_advance(&systimer_hal, SYSTIMER_COUNTER_OS_TICK, 1000000UL / CONFIG_FREERTOS_HZ / 2);
#د_که_پای
            }
        }
    } وإلا {
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
فراغ SysTickIsrHandler(فراغ *arg)
{
    uint32_t cpuid = x_پورټ_Get_Core_ID();
    systimer_hal_context_t *systimer_hal = (systimer_hal_context_t *)arg;
#که_تعریف_شوی CONFIG_PM_TRACE
    ESP_PM_TRACE_ENTER(TICK, cpuid);
#د_که_پای

    uint32_t alarm_id = SYSTIMER_ALARM_OS_TICK_CORE0 + cpuid;
    افعل {
        systimer_ll_clear_alarm_int(systimer_hal->dev, alarm_id);

        uint32_t diff = systimer_hal_get_counter_ارزښت(systimer_hal, SYSTIMER_COUNTER_OS_TICK) / systimer_ll_get_alarm_period(systimer_hal->dev, alarm_id) - s_handled_systicks[cpuid];
        إذا (diff > 0) {
            إذا (s_handled_systicks[cpuid] == 0) {
                s_handled_systicks[cpuid] = diff;
                diff = 1;
            } وإلا {
                s_handled_systicks[cpuid] += diff;
            }

            افعل {
                x_پورټ_Sys_Tick_Handler();
            } طالما (--diff);
        }
    } طالما (systimer_ll_is_alarm_int_fired(systimer_hal->dev, alarm_id));

#که_تعریف_شوی CONFIG_PM_TRACE
    ESP_PM_TRACE_EXIT(TICK, cpuid);
#د_که_پای
}

uint32_t x_پورټ_Sys_Tick_Claim_Elapsed_Ticks(صحيح cpu_id)
{
    uint32_t alarm_id = SYSTIMER_ALARM_OS_TICK_CORE0 + (uint32_t)cpu_id;
    uint32_t ټول = systimer_hal_get_counter_ارزښت(&systimer_hal, SYSTIMER_COUNTER_OS_TICK) / systimer_ll_get_alarm_period(systimer_hal.dev, alarm_id);
    uint32_t elapsed = ټول - s_handled_systicks[cpu_id];
    إذا (elapsed == 0) {
        أرجع 0;
    }
    s_handled_systicks[cpu_id] = ټول;
    أرجع elapsed;
}
#د_که_پای /* CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER */

/* ------------------------------------------------ Common Port Tick ---------------------------------------------------
 * Tick related functions common across all ports
 * ------------------------------------------------------------------------------------------------------------------ */

/**
 * @brief Initialize the tick interrupt timer
 *
 * - CCOUNT timer is used if CONFIG_FREERTOS_SYSTICK_USES_CCOUNT is set
 * - SYSTIMER is used if CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER is set
 */
فراغ v_پورټ_تیاری_Timer(فراغ)
{
#إذا CONFIG_FREERTOS_SYSTICK_کاروي_CCOUNT
    خارجي فراغ _frxt_tick_timer_init(فراغ);
    خارجي فراغ xt_tick_وېشونکی_init(فراغ);
    /* Init the tick divisor value */
    xt_tick_وېشونکی_init();
    _frxt_tick_timer_init();
#وإلا /* CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER */
    v_Systimer_تیاری();
#د_که_پای /* CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER */
}

/**
 * @brief Handler of SysTick
 *
 * The function is called from:
 *  - _frxt_timer_int for xtensa with CONFIG_FREERTOS_SYSTICK_USES_CCOUNT
 *  - SysTickIsrHandler for xtensa with CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER
 *  - SysTickIsrHandler for riscv
 */
بنسټ_Type_t x_پورټ_Sys_Tick_Handler(فراغ)
{
#إذا configBENCHMARK
    portbenchmarkIntLatency();
#د_که_پای //configBENCHMARK
    traceISR_ENTER(SYSTICK_INTR_ID);

    // Call IDF Tick Hook
    خارجي فراغ esp_vApplicationTickHook(فراغ);
    esp_vApplicationTickHook();

    // Call FreeRTOS Increment tick function
    بنسټ_Type_t xSwitchRequired;
#إذا CONFIG_FREERTOS_SMP
    UBaseType_t ux_Saved_حالت = taskENTER_CRITICAL_FROM_ISR();
    // Amazon SMP FreeRTOS requires that only core 0 calls xTaskIncrementTick()
#إذا ( config_عدد_CORES > 1 )
    إذا (پورټ_GET_CORE_ID() == 0) {
        xSwitchRequired = xTaskIncrementTick();
    } وإلا {
        xSwitchRequired = pdFALSE;
    }
#وإلا /* configNUM_CORES > 1 */
    xSwitchRequired = xTaskIncrementTick();
#د_که_پای /* configNUM_CORES > 1 */
    taskEXIT_CRITICAL_FROM_ISR(ux_Saved_حالت);
#وإلا /* !CONFIG_FREERTOS_SMP */
#إذا ( config_عدد_CORES > 1 )
    /*
    Multi-core IDF FreeRTOS requires that...
        - core 0 calls xTaskIncrementTick()
        - core 1 calls xTaskIncrementTickOtherCores()
    */
    إذا (x_پورټ_Get_Core_ID() == 0) {
        xSwitchRequired = xTaskIncrementTick();
    } وإلا {
        xSwitchRequired = xTaskIncrementTickOtherCores();
    }
#وإلا /* configNUM_CORES > 1 */
    /*
    Vanilla (single core) FreeRTOS expects that xTaskIncrementTick() cannot be interrupted (i.e., no nested
    interrupts). Thus we have to disable interrupts before calling it.
    */
    UBaseType_t ux_Saved_مداخله_حالت = پورټ_سېټ_مداخله_ماسک_FROM_ISR();
    xSwitchRequired = xTaskIncrementTick();
    پورټ_CLEAR_مداخله_ماسک_FROM_ISR(ux_Saved_مداخله_حالت);
#د_که_پای /* configNUM_CORES > 1 */
#د_که_پای /* !CONFIG_FREERTOS_SMP */

    // Check if yield is required
    إذا (xSwitchRequired != pdFALSE) {
        پورټ_YIELD_FROM_ISR();
    } وإلا {
        traceISR_EXIT();
    }
    أرجع xSwitchRequired;
}
