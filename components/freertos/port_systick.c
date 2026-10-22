/*
 * SPDX-FileCopyrightText: 2017-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#अन्तर्भाव "sdkconfig.h"
#अन्तर्भाव <stdint.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव "FreeRTOS.h"
#अन्तर्भाव "task.h"
#यदि ( !CONFIG_FREERTOS_SMP && ( config_अङ्कः_CORES > 1 ) )
/* Required for xTaskIncrementTickOtherCores() */
#अन्तर्भाव "esp_private/freertos_idf_additions_priv.h"
#यदि_अन्तः /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */

#यदि CONFIG_FREERTOS_SYSTICK_उपयुङ्क्ते_CCOUNT
#यदि CONFIG_FREERTOS_CORETIMER_0
#परिभाषय SYSTICK_INTR_ID     (ETS_आन्तरिकम्_TIMER0_INTR_SOURCE + ETS_आन्तरिकम्_INTR_SOURCE_OFF)
#अन्यथा /* CONFIG_FREERTOS_CORETIMER_1 */
#परिभाषय SYSTICK_INTR_ID     (ETS_आन्तरिकम्_TIMER1_INTR_SOURCE + ETS_आन्तरिकम्_INTR_SOURCE_OFF)
#यदि_अन्तः
#अन्यथा /* CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER */
#परिभाषय SYSTICK_INTR_ID     (ETS_SYSTIMER_TARGET0_INTR_SOURCE)
#यदि_अन्तः /* CONFIG_FREERTOS_SYSTICK_USES_CCOUNT */

आधारः_Type_t x_पत्तनम्_Sys_Tick_Handler(शून्य);

/* --------------------------------------------- SYSTIMER Implementation -----------------------------------------------
 * Implementation of a tick interrupt using the SYSTIMER perpiheral
 * Todo: Abstract setup and operation in a separate SYSTIMER driver (IDF-6976)
 * ------------------------------------------------------------------------------------------------------------------ */

#यदि CONFIG_FREERTOS_SYSTICK_उपयुङ्क्ते_SYSTIMER

#अन्तर्भाव "soc/periph_defs.h"
#अन्तर्भाव "hal/systimer_hal.h"
#अन्तर्भाव "hal/systimer_ll.h"
#अन्तर्भाव "esp_err.h"
#अन्तर्भाव "esp_intr_alloc.h"
#अन्तर्भाव "esp_private/systimer.h"
#अन्तर्भाव "esp_private/periph_ctrl.h"
#यदि_परिभाषितम् CONFIG_PM_TRACE
#अन्तर्भाव "esp_private/pm_trace.h"
#यदि_अन्तः //CONFIG_PM_TRACE

_Static_assert(SOC_CPU_CORES_अङ्कः <= SYSTIMER_LL_ALARM_अङ्कः - 1, "the number of cores must match the number of core alarms in SYSTIMER");

शून्य SysTickIsrHandler(शून्य *arg);

स्थिर uint32_t s_handled_systicks[config_अङ्कः_CORES] = { 0 };

/* Systimer HAL layer object */
स्थिर systimer_hal_context_t systimer_hal;

/**
 * @brief Set up the systimer peripheral to generate the tick interrupt
 *
 * Both timer alarms are configured in periodic mode.
 * It is done at the same time so SysTicks for both CPUs occur at the same time or very close.
 * Shifts a time of triggering interrupts for core 0 and core 1.
 */
शून्य v_Systimer_सज्जता(शून्य)
{
    अचिह्नित cpuid = x_पत्तनम्_Get_Core_ID();
#यदि_परिभाषितम् CONFIG_FREERTOS_CORETIMER_SYSTIMER_LVL3
    ध्रुव अचिह्नित level = ESP_INTR_FLAG_LEVEL3;
#अन्यथा
    ध्रुव अचिह्नित level = ESP_INTR_FLAG_LEVEL1;
#यदि_अन्तः
    /* set system timer interrupt vector */

    ESP_ERROR_CHECK(esp_intr_alloc(ETS_SYSTIMER_TARGET0_INTR_SOURCE + cpuid, ESP_INTR_FLAG_IRAM | level, SysTickIsrHandler, &systimer_hal, रिक्त));

    यदि (cpuid == 0) {
        PERIPH_RCC_ACQUIRE_ATOMIC(PERIPH_SYSTIMER_MODULE, ref_गणनम्) {
            यदि (ref_गणनम् == 0) {
                systimer_ll_enable_bus_clock(true);
                systimer_ll_enable_sys_clock(true);
                systimer_ll_reset_पञ्जिकाकोशः();
            }
        }
        systimer_hal_init(&systimer_hal);
        systimer_hal_tick_दरः_ops_t ops = {
            .ticks_गन्तव्यम्_us = systimer_ticks_गन्तव्यम्_us,
            .us_गन्तव्यम्_ticks = systimer_us_गन्तव्यम्_ticks,
        };
        systimer_hal_समुच्चयः_tick_दरः_ops(&systimer_hal, &ops);
        systimer_ll_समुच्चयः_गणक_मानम्(systimer_hal.dev, SYSTIMER_गणक_OS_TICK, 0);
        systimer_ll_apply_गणक_मानम्(systimer_hal.dev, SYSTIMER_गणक_OS_TICK);

        प्रति (cpuid = 0; cpuid < SOC_CPU_CORES_अङ्कः; cpuid++) {
            // Set stall option and alarm mode to default state. Below they will be set to a required state.
            systimer_hal_गणक_can_stall_by_cpu(&systimer_hal, SYSTIMER_गणक_OS_TICK, cpuid, false);
            uint32_t alarm_id = SYSTIMER_ALARM_OS_TICK_CORE0 + cpuid;
            systimer_hal_select_alarm_बहुलकम्(&systimer_hal, alarm_id, SYSTIMER_ALARM_बहुलकम्_ONESHOT);
        }

        प्रति (cpuid = 0; cpuid < config_अङ्कः_CORES; ++cpuid) {
            uint32_t alarm_id = SYSTIMER_ALARM_OS_TICK_CORE0 + cpuid;

            /* configure the timer */
            systimer_hal_connect_alarm_गणक(&systimer_hal, alarm_id, SYSTIMER_गणक_OS_TICK);
            systimer_hal_समुच्चयः_alarm_period(&systimer_hal, alarm_id, 1000000UL / CONFIG_FREERTOS_HZ);
            systimer_hal_select_alarm_बहुलकम्(&systimer_hal, alarm_id, SYSTIMER_ALARM_बहुलकम्_PERIOD);
            systimer_hal_गणक_can_stall_by_cpu(&systimer_hal, SYSTIMER_गणक_OS_TICK, cpuid, true);
            यदि (cpuid == 0) {
                systimer_hal_enable_alarm_int(&systimer_hal, alarm_id);
                systimer_hal_enable_गणक(&systimer_hal, SYSTIMER_गणक_OS_TICK);
#यदि_अपरिभाषितम् CONFIG_FREERTOS_UNICORE
                // SysTick of core 0 and core 1 are shifted by half of period
                systimer_hal_गणक_मानम्_advance(&systimer_hal, SYSTIMER_गणक_OS_TICK, 1000000UL / CONFIG_FREERTOS_HZ / 2);
#यदि_अन्तः
            }
        }
    } अन्यथा {
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
शून्य SysTickIsrHandler(शून्य *arg)
{
    uint32_t cpuid = x_पत्तनम्_Get_Core_ID();
    systimer_hal_context_t *systimer_hal = (systimer_hal_context_t *)arg;
#यदि_परिभाषितम् CONFIG_PM_TRACE
    ESP_PM_TRACE_ENTER(TICK, cpuid);
#यदि_अन्तः

    uint32_t alarm_id = SYSTIMER_ALARM_OS_TICK_CORE0 + cpuid;
    कुरु {
        systimer_ll_clear_alarm_int(systimer_hal->dev, alarm_id);

        uint32_t diff = systimer_hal_get_गणक_मानम्(systimer_hal, SYSTIMER_गणक_OS_TICK) / systimer_ll_get_alarm_period(systimer_hal->dev, alarm_id) - s_handled_systicks[cpuid];
        यदि (diff > 0) {
            यदि (s_handled_systicks[cpuid] == 0) {
                s_handled_systicks[cpuid] = diff;
                diff = 1;
            } अन्यथा {
                s_handled_systicks[cpuid] += diff;
            }

            कुरु {
                x_पत्तनम्_Sys_Tick_Handler();
            } यावत् (--diff);
        }
    } यावत् (systimer_ll_is_alarm_int_fired(systimer_hal->dev, alarm_id));

#यदि_परिभाषितम् CONFIG_PM_TRACE
    ESP_PM_TRACE_EXIT(TICK, cpuid);
#यदि_अन्तः
}

uint32_t x_पत्तनम्_Sys_Tick_Claim_Elapsed_Ticks(पूर्णाङ्क cpu_id)
{
    uint32_t alarm_id = SYSTIMER_ALARM_OS_TICK_CORE0 + (uint32_t)cpu_id;
    uint32_t योग = systimer_hal_get_गणक_मानम्(&systimer_hal, SYSTIMER_गणक_OS_TICK) / systimer_ll_get_alarm_period(systimer_hal.dev, alarm_id);
    uint32_t elapsed = योग - s_handled_systicks[cpu_id];
    यदि (elapsed == 0) {
        प्रत्यागम 0;
    }
    s_handled_systicks[cpu_id] = योग;
    प्रत्यागम elapsed;
}
#यदि_अन्तः /* CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER */

/* ------------------------------------------------ Common Port Tick ---------------------------------------------------
 * Tick related functions common across all ports
 * ------------------------------------------------------------------------------------------------------------------ */

/**
 * @brief Initialize the tick interrupt timer
 *
 * - CCOUNT timer is used if CONFIG_FREERTOS_SYSTICK_USES_CCOUNT is set
 * - SYSTIMER is used if CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER is set
 */
शून्य v_पत्तनम्_सज्जता_Timer(शून्य)
{
#यदि CONFIG_FREERTOS_SYSTICK_उपयुङ्क्ते_CCOUNT
    बाह्य शून्य _frxt_tick_timer_init(शून्य);
    बाह्य शून्य xt_tick_भाजकः_init(शून्य);
    /* Init the tick divisor value */
    xt_tick_भाजकः_init();
    _frxt_tick_timer_init();
#अन्यथा /* CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER */
    v_Systimer_सज्जता();
#यदि_अन्तः /* CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER */
}

/**
 * @brief Handler of SysTick
 *
 * The function is called from:
 *  - _frxt_timer_int for xtensa with CONFIG_FREERTOS_SYSTICK_USES_CCOUNT
 *  - SysTickIsrHandler for xtensa with CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER
 *  - SysTickIsrHandler for riscv
 */
आधारः_Type_t x_पत्तनम्_Sys_Tick_Handler(शून्य)
{
#यदि configBENCHMARK
    portbenchmarkIntLatency();
#यदि_अन्तः //configBENCHMARK
    traceISR_ENTER(SYSTICK_INTR_ID);

    // Call IDF Tick Hook
    बाह्य शून्य esp_vApplicationTickHook(शून्य);
    esp_vApplicationTickHook();

    // Call FreeRTOS Increment tick function
    आधारः_Type_t xSwitchRequired;
#यदि CONFIG_FREERTOS_SMP
    UBaseType_t ux_Saved_स्थितिः = taskENTER_CRITICAL_FROM_ISR();
    // Amazon SMP FreeRTOS requires that only core 0 calls xTaskIncrementTick()
#यदि ( config_अङ्कः_CORES > 1 )
    यदि (पत्तनम्_GET_CORE_ID() == 0) {
        xSwitchRequired = xTaskIncrementTick();
    } अन्यथा {
        xSwitchRequired = pdFALSE;
    }
#अन्यथा /* configNUM_CORES > 1 */
    xSwitchRequired = xTaskIncrementTick();
#यदि_अन्तः /* configNUM_CORES > 1 */
    taskEXIT_CRITICAL_FROM_ISR(ux_Saved_स्थितिः);
#अन्यथा /* !CONFIG_FREERTOS_SMP */
#यदि ( config_अङ्कः_CORES > 1 )
    /*
    Multi-core IDF FreeRTOS requires that...
        - core 0 calls xTaskIncrementTick()
        - core 1 calls xTaskIncrementTickOtherCores()
    */
    यदि (x_पत्तनम्_Get_Core_ID() == 0) {
        xSwitchRequired = xTaskIncrementTick();
    } अन्यथा {
        xSwitchRequired = xTaskIncrementTickOtherCores();
    }
#अन्यथा /* configNUM_CORES > 1 */
    /*
    Vanilla (single core) FreeRTOS expects that xTaskIncrementTick() cannot be interrupted (i.e., no nested
    interrupts). Thus we have to disable interrupts before calling it.
    */
    UBaseType_t ux_Saved_व्यवधानम्_स्थितिः = पत्तनम्_समुच्चयः_व्यवधानम्_आवरणम्_FROM_ISR();
    xSwitchRequired = xTaskIncrementTick();
    पत्तनम्_CLEAR_व्यवधानम्_आवरणम्_FROM_ISR(ux_Saved_व्यवधानम्_स्थितिः);
#यदि_अन्तः /* configNUM_CORES > 1 */
#यदि_अन्तः /* !CONFIG_FREERTOS_SMP */

    // Check if yield is required
    यदि (xSwitchRequired != pdFALSE) {
        पत्तनम्_YIELD_FROM_ISR();
    } अन्यथा {
        traceISR_EXIT();
    }
    प्रत्यागम xSwitchRequired;
}
