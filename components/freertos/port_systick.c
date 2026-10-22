/*
 * SPDX-FileCopyrightText: 2017-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#شامل "sdkconfig.h"
#شامل <stdint.h>
#شامل <stdlib.h>
#شامل "FreeRTOS.h"
#شامل "task.h"
#اگر ( !CONFIG_FREERTOS_SMP && ( config_عدد_CORES > 1 ) )
/* Required for xTaskIncrementTickOtherCores() */
#شامل "esp_private/freertos_idf_additions_priv.h"
#اگر_ختم /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */

#اگر CONFIG_FREERTOS_SYSTICK_استعمال_کرتا_CCOUNT
#اگر CONFIG_FREERTOS_CORETIMER_0
#تعریف_کرو SYSTICK_INTR_ID     (ETS_اندرونی_TIMER0_INTR_ماخذ + ETS_اندرونی_INTR_ماخذ_OFF)
#ورنہ /* CONFIG_FREERTOS_CORETIMER_1 */
#تعریف_کرو SYSTICK_INTR_ID     (ETS_اندرونی_TIMER1_INTR_ماخذ + ETS_اندرونی_INTR_ماخذ_OFF)
#اگر_ختم
#ورنہ /* CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER */
#تعریف_کرو SYSTICK_INTR_ID     (ETS_SYSTIMER_TARGET0_INTR_ماخذ)
#اگر_ختم /* CONFIG_FREERTOS_SYSTICK_USES_CCOUNT */

بنیاد_Type_t x_پورٹ_Sys_Tick_Handler(خالی);

/* --------------------------------------------- SYSTIMER Implementation -----------------------------------------------
 * Implementation of a tick interrupt using the SYSTIMER perpiheral
 * Todo: Abstract setup and operation in a separate SYSTIMER driver (IDF-6976)
 * ------------------------------------------------------------------------------------------------------------------ */

#اگر CONFIG_FREERTOS_SYSTICK_استعمال_کرتا_SYSTIMER

#شامل "soc/periph_defs.h"
#شامل "hal/systimer_hal.h"
#شامل "hal/systimer_ll.h"
#شامل "esp_err.h"
#شامل "esp_intr_alloc.h"
#شامل "esp_private/systimer.h"
#شامل "esp_private/periph_ctrl.h"
#اگر_تعریف_شدہ CONFIG_PM_TRACE
#شامل "esp_private/pm_trace.h"
#اگر_ختم //CONFIG_PM_TRACE

_Static_assert(SOC_CPU_CORES_عدد <= SYSTIMER_LL_ALARM_عدد - 1, "the number of cores must match the number of core alarms in SYSTIMER");

خالی SysTickIsrHandler(خالی *arg);

ساکن uint32_t s_handled_systicks[config_عدد_CORES] = { 0 };

/* Systimer HAL layer object */
ساکن systimer_hal_context_t systimer_hal;

/**
 * @brief Set up the systimer peripheral to generate the tick interrupt
 *
 * Both timer alarms are configured in periodic mode.
 * It is done at the same time so SysTicks for both CPUs occur at the same time or very close.
 * Shifts a time of triggering interrupts for core 0 and core 1.
 */
خالی v_Systimer_تیاری(خالی)
{
    غیرعلامتی cpuid = x_پورٹ_Get_Core_ID();
#اگر_تعریف_شدہ CONFIG_FREERTOS_CORETIMER_SYSTIMER_LVL3
    ثابت غیرعلامتی level = ESP_INTR_FLAG_LEVEL3;
#ورنہ
    ثابت غیرعلامتی level = ESP_INTR_FLAG_LEVEL1;
#اگر_ختم
    /* set system timer interrupt vector */

    ESP_ERROR_CHECK(esp_intr_alloc(ETS_SYSTIMER_TARGET0_INTR_ماخذ + cpuid, ESP_INTR_FLAG_IRAM | level, SysTickIsrHandler, &systimer_hal, تہی));

    اگر (cpuid == 0) {
        PERIPH_RCC_ACQUIRE_ATOMIC(PERIPH_SYSTIMER_MODULE, ref_گنتی) {
            اگر (ref_گنتی == 0) {
                systimer_ll_enable_bus_clock(true);
                systimer_ll_enable_sys_clock(true);
                systimer_ll_reset_رجسٹر();
            }
        }
        systimer_hal_init(&systimer_hal);
        systimer_hal_tick_شرح_ops_t ops = {
            .ticks_مقصد_us = systimer_ticks_مقصد_us,
            .us_مقصد_ticks = systimer_us_مقصد_ticks,
        };
        systimer_hal_سیٹ_tick_شرح_ops(&systimer_hal, &ops);
        systimer_ll_سیٹ_شمار_قدر(systimer_hal.dev, SYSTIMER_شمار_OS_TICK, 0);
        systimer_ll_apply_شمار_قدر(systimer_hal.dev, SYSTIMER_شمار_OS_TICK);

        برائے (cpuid = 0; cpuid < SOC_CPU_CORES_عدد; cpuid++) {
            // Set stall option and alarm mode to default state. Below they will be set to a required state.
            systimer_hal_شمار_can_stall_by_cpu(&systimer_hal, SYSTIMER_شمار_OS_TICK, cpuid, false);
            uint32_t alarm_id = SYSTIMER_ALARM_OS_TICK_CORE0 + cpuid;
            systimer_hal_select_alarm_منوال(&systimer_hal, alarm_id, SYSTIMER_ALARM_منوال_ONESHOT);
        }

        برائے (cpuid = 0; cpuid < config_عدد_CORES; ++cpuid) {
            uint32_t alarm_id = SYSTIMER_ALARM_OS_TICK_CORE0 + cpuid;

            /* configure the timer */
            systimer_hal_connect_alarm_شمار(&systimer_hal, alarm_id, SYSTIMER_شمار_OS_TICK);
            systimer_hal_سیٹ_alarm_period(&systimer_hal, alarm_id, 1000000UL / CONFIG_FREERTOS_HZ);
            systimer_hal_select_alarm_منوال(&systimer_hal, alarm_id, SYSTIMER_ALARM_منوال_PERIOD);
            systimer_hal_شمار_can_stall_by_cpu(&systimer_hal, SYSTIMER_شمار_OS_TICK, cpuid, true);
            اگر (cpuid == 0) {
                systimer_hal_enable_alarm_int(&systimer_hal, alarm_id);
                systimer_hal_enable_شمار(&systimer_hal, SYSTIMER_شمار_OS_TICK);
#اگر_غیر_تعریف_شدہ CONFIG_FREERTOS_UNICORE
                // SysTick of core 0 and core 1 are shifted by half of period
                systimer_hal_شمار_قدر_advance(&systimer_hal, SYSTIMER_شمار_OS_TICK, 1000000UL / CONFIG_FREERTOS_HZ / 2);
#اگر_ختم
            }
        }
    } ورنہ {
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
خالی SysTickIsrHandler(خالی *arg)
{
    uint32_t cpuid = x_پورٹ_Get_Core_ID();
    systimer_hal_context_t *systimer_hal = (systimer_hal_context_t *)arg;
#اگر_تعریف_شدہ CONFIG_PM_TRACE
    ESP_PM_TRACE_ENTER(TICK, cpuid);
#اگر_ختم

    uint32_t alarm_id = SYSTIMER_ALARM_OS_TICK_CORE0 + cpuid;
    کرو {
        systimer_ll_clear_alarm_int(systimer_hal->dev, alarm_id);

        uint32_t diff = systimer_hal_get_شمار_قدر(systimer_hal, SYSTIMER_شمار_OS_TICK) / systimer_ll_get_alarm_period(systimer_hal->dev, alarm_id) - s_handled_systicks[cpuid];
        اگر (diff > 0) {
            اگر (s_handled_systicks[cpuid] == 0) {
                s_handled_systicks[cpuid] = diff;
                diff = 1;
            } ورنہ {
                s_handled_systicks[cpuid] += diff;
            }

            کرو {
                x_پورٹ_Sys_Tick_Handler();
            } جب (--diff);
        }
    } جب (systimer_ll_is_alarm_int_fired(systimer_hal->dev, alarm_id));

#اگر_تعریف_شدہ CONFIG_PM_TRACE
    ESP_PM_TRACE_EXIT(TICK, cpuid);
#اگر_ختم
}

uint32_t x_پورٹ_Sys_Tick_Claim_Elapsed_Ticks(صحیح cpu_id)
{
    uint32_t alarm_id = SYSTIMER_ALARM_OS_TICK_CORE0 + (uint32_t)cpu_id;
    uint32_t کل = systimer_hal_get_شمار_قدر(&systimer_hal, SYSTIMER_شمار_OS_TICK) / systimer_ll_get_alarm_period(systimer_hal.dev, alarm_id);
    uint32_t elapsed = کل - s_handled_systicks[cpu_id];
    اگر (elapsed == 0) {
        واپس 0;
    }
    s_handled_systicks[cpu_id] = کل;
    واپس elapsed;
}
#اگر_ختم /* CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER */

/* ------------------------------------------------ Common Port Tick ---------------------------------------------------
 * Tick related functions common across all ports
 * ------------------------------------------------------------------------------------------------------------------ */

/**
 * @brief Initialize the tick interrupt timer
 *
 * - CCOUNT timer is used if CONFIG_FREERTOS_SYSTICK_USES_CCOUNT is set
 * - SYSTIMER is used if CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER is set
 */
خالی v_پورٹ_تیاری_Timer(خالی)
{
#اگر CONFIG_FREERTOS_SYSTICK_استعمال_کرتا_CCOUNT
    خارجی خالی _frxt_tick_timer_init(خالی);
    خارجی خالی xt_tick_مقسوم_علیہ_init(خالی);
    /* Init the tick divisor value */
    xt_tick_مقسوم_علیہ_init();
    _frxt_tick_timer_init();
#ورنہ /* CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER */
    v_Systimer_تیاری();
#اگر_ختم /* CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER */
}

/**
 * @brief Handler of SysTick
 *
 * The function is called from:
 *  - _frxt_timer_int for xtensa with CONFIG_FREERTOS_SYSTICK_USES_CCOUNT
 *  - SysTickIsrHandler for xtensa with CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER
 *  - SysTickIsrHandler for riscv
 */
بنیاد_Type_t x_پورٹ_Sys_Tick_Handler(خالی)
{
#اگر configBENCHMARK
    portbenchmarkIntLatency();
#اگر_ختم //configBENCHMARK
    traceISR_ENTER(SYSTICK_INTR_ID);

    // Call IDF Tick Hook
    خارجی خالی esp_vApplicationTickHook(خالی);
    esp_vApplicationTickHook();

    // Call FreeRTOS Increment tick function
    بنیاد_Type_t xSwitchRequired;
#اگر CONFIG_FREERTOS_SMP
    UBaseType_t ux_Saved_حالت = taskENTER_CRITICAL_FROM_ISR();
    // Amazon SMP FreeRTOS requires that only core 0 calls xTaskIncrementTick()
#اگر ( config_عدد_CORES > 1 )
    اگر (پورٹ_GET_CORE_ID() == 0) {
        xSwitchRequired = xTaskIncrementTick();
    } ورنہ {
        xSwitchRequired = pdFALSE;
    }
#ورنہ /* configNUM_CORES > 1 */
    xSwitchRequired = xTaskIncrementTick();
#اگر_ختم /* configNUM_CORES > 1 */
    taskEXIT_CRITICAL_FROM_ISR(ux_Saved_حالت);
#ورنہ /* !CONFIG_FREERTOS_SMP */
#اگر ( config_عدد_CORES > 1 )
    /*
    Multi-core IDF FreeRTOS requires that...
        - core 0 calls xTaskIncrementTick()
        - core 1 calls xTaskIncrementTickOtherCores()
    */
    اگر (x_پورٹ_Get_Core_ID() == 0) {
        xSwitchRequired = xTaskIncrementTick();
    } ورنہ {
        xSwitchRequired = xTaskIncrementTickOtherCores();
    }
#ورنہ /* configNUM_CORES > 1 */
    /*
    Vanilla (single core) FreeRTOS expects that xTaskIncrementTick() cannot be interrupted (i.e., no nested
    interrupts). Thus we have to disable interrupts before calling it.
    */
    UBaseType_t ux_Saved_مداخلت_حالت = پورٹ_سیٹ_مداخلت_نقاب_FROM_ISR();
    xSwitchRequired = xTaskIncrementTick();
    پورٹ_CLEAR_مداخلت_نقاب_FROM_ISR(ux_Saved_مداخلت_حالت);
#اگر_ختم /* configNUM_CORES > 1 */
#اگر_ختم /* !CONFIG_FREERTOS_SMP */

    // Check if yield is required
    اگر (xSwitchRequired != pdFALSE) {
        پورٹ_YIELD_FROM_ISR();
    } ورنہ {
        traceISR_EXIT();
    }
    واپس xSwitchRequired;
}
