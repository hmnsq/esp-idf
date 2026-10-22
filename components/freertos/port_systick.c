/*
 * SPDX-FileCopyrightText: 2017-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ଅନ୍ତର୍ଭାଵ "sdkconfig.h"
#ଅନ୍ତର୍ଭାଵ <stdint.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ "FreeRTOS.h"
#ଅନ୍ତର୍ଭାଵ "task.h"
#ଯଦି ( !CONFIG_FREERTOS_SMP && ( config_ଅଙ୍କ_CORES > 1 ) )
/* Required for xTaskIncrementTickOtherCores() */
#ଅନ୍ତର୍ଭାଵ "esp_private/freertos_idf_additions_priv.h"
#endif /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */

#ଯଦି CONFIG_FREERTOS_SYSTICK_USES_CCOUNT
#ଯଦି CONFIG_FREERTOS_CORETIMER_0
#define SYSTICK_INTR_ID     (ETS_INTERNAL_TIMER0_INTR_SOURCE + ETS_INTERNAL_INTR_SOURCE_OFF)
#ଅନ୍ୟଥା /* CONFIG_FREERTOS_CORETIMER_1 */
#define SYSTICK_INTR_ID     (ETS_INTERNAL_TIMER1_INTR_SOURCE + ETS_INTERNAL_INTR_SOURCE_OFF)
#endif
#ଅନ୍ୟଥା /* CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER */
#define SYSTICK_INTR_ID     (ETS_SYSTIMER_TARGET0_INTR_SOURCE)
#endif /* CONFIG_FREERTOS_SYSTICK_USES_CCOUNT */

BaseType_t xPortSysTickHandler(ଶୂନ୍ୟ);

/* --------------------------------------------- SYSTIMER Implementation -----------------------------------------------
 * Implementation of a tick interrupt using the SYSTIMER perpiheral
 * Todo: Abstract setup and operation in a separate SYSTIMER driver (IDF-6976)
 * ------------------------------------------------------------------------------------------------------------------ */

#ଯଦି CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER

#ଅନ୍ତର୍ଭାଵ "soc/periph_defs.h"
#ଅନ୍ତର୍ଭାଵ "hal/systimer_hal.h"
#ଅନ୍ତର୍ଭାଵ "hal/systimer_ll.h"
#ଅନ୍ତର୍ଭାଵ "esp_err.h"
#ଅନ୍ତର୍ଭାଵ "esp_intr_alloc.h"
#ଅନ୍ତର୍ଭାଵ "esp_private/systimer.h"
#ଅନ୍ତର୍ଭାଵ "esp_private/periph_ctrl.h"
#ifdef CONFIG_PM_TRACE
#ଅନ୍ତର୍ଭାଵ "esp_private/pm_trace.h"
#endif //CONFIG_PM_TRACE

_Static_assert(SOC_CPU_CORES_ଅଙ୍କ <= SYSTIMER_LL_ALARM_ଅଙ୍କ - 1, "the number of cores must match the number of core alarms in SYSTIMER");

ଶୂନ୍ୟ SysTickIsrHandler(ଶୂନ୍ୟ *arg);

ସ୍ଥିର uint32_t s_handled_systicks[config_ଅଙ୍କ_CORES] = { 0 };

/* Systimer HAL layer object */
ସ୍ଥିର systimer_hal_context_t systimer_hal;

/**
 * @brief Set up the systimer peripheral to generate the tick interrupt
 *
 * Both timer alarms are configured in periodic mode.
 * It is done at the same time so SysTicks for both CPUs occur at the same time or very close.
 * Shifts a time of triggering interrupts for core 0 and core 1.
 */
ଶୂନ୍ୟ vSystimerSetup(ଶୂନ୍ୟ)
{
    unsigned cpuid = xPortGetCoreID();
#ifdef CONFIG_FREERTOS_CORETIMER_SYSTIMER_LVL3
    ଅଚଳ unsigned level = ESP_INTR_FLAG_LEVEL3;
#ଅନ୍ୟଥା
    ଅଚଳ unsigned level = ESP_INTR_FLAG_LEVEL1;
#endif
    /* set system timer interrupt vector */

    ESP_ERROR_CHECK(esp_intr_alloc(ETS_SYSTIMER_TARGET0_INTR_SOURCE + cpuid, ESP_INTR_FLAG_IRAM | level, SysTickIsrHandler, &systimer_hal, ଖାଲି));

    ଯଦି (cpuid == 0) {
        PERIPH_RCC_ACQUIRE_ATOMIC(PERIPH_SYSTIMER_MODULE, ref_ଗଣନା) {
            ଯଦି (ref_ଗଣନା == 0) {
                systimer_ll_enable_bus_clock(true);
                systimer_ll_enable_sys_clock(true);
                systimer_ll_reset_register();
            }
        }
        systimer_hal_init(&systimer_hal);
        systimer_hal_tick_rate_ops_t ops = {
            .ticks_to_us = systimer_ticks_to_us,
            .us_to_ticks = systimer_us_to_ticks,
        };
        systimer_hal_set_tick_rate_ops(&systimer_hal, &ops);
        systimer_ll_set_counter_ମୂଲ୍ୟ(systimer_hal.dev, SYSTIMER_COUNTER_OS_TICK, 0);
        systimer_ll_apply_counter_ମୂଲ୍ୟ(systimer_hal.dev, SYSTIMER_COUNTER_OS_TICK);

        ପାଇଁ (cpuid = 0; cpuid < SOC_CPU_CORES_ଅଙ୍କ; cpuid++) {
            // Set stall option and alarm mode to default state. Below they will be set to a required state.
            systimer_hal_counter_can_stall_by_cpu(&systimer_hal, SYSTIMER_COUNTER_OS_TICK, cpuid, false);
            uint32_t alarm_id = SYSTIMER_ALARM_OS_TICK_CORE0 + cpuid;
            systimer_hal_select_alarm_mode(&systimer_hal, alarm_id, SYSTIMER_ALARM_MODE_ONESHOT);
        }

        ପାଇଁ (cpuid = 0; cpuid < config_ଅଙ୍କ_CORES; ++cpuid) {
            uint32_t alarm_id = SYSTIMER_ALARM_OS_TICK_CORE0 + cpuid;

            /* configure the timer */
            systimer_hal_connect_alarm_counter(&systimer_hal, alarm_id, SYSTIMER_COUNTER_OS_TICK);
            systimer_hal_set_alarm_period(&systimer_hal, alarm_id, 1000000UL / CONFIG_FREERTOS_HZ);
            systimer_hal_select_alarm_mode(&systimer_hal, alarm_id, SYSTIMER_ALARM_MODE_PERIOD);
            systimer_hal_counter_can_stall_by_cpu(&systimer_hal, SYSTIMER_COUNTER_OS_TICK, cpuid, true);
            ଯଦି (cpuid == 0) {
                systimer_hal_enable_alarm_int(&systimer_hal, alarm_id);
                systimer_hal_enable_counter(&systimer_hal, SYSTIMER_COUNTER_OS_TICK);
#ifndef CONFIG_FREERTOS_UNICORE
                // SysTick of core 0 and core 1 are shifted by half of period
                systimer_hal_counter_ମୂଲ୍ୟ_advance(&systimer_hal, SYSTIMER_COUNTER_OS_TICK, 1000000UL / CONFIG_FREERTOS_HZ / 2);
#endif
            }
        }
    } ଅନ୍ୟଥା {
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
ଶୂନ୍ୟ SysTickIsrHandler(ଶୂନ୍ୟ *arg)
{
    uint32_t cpuid = xPortGetCoreID();
    systimer_hal_context_t *systimer_hal = (systimer_hal_context_t *)arg;
#ifdef CONFIG_PM_TRACE
    ESP_PM_TRACE_ENTER(TICK, cpuid);
#endif

    uint32_t alarm_id = SYSTIMER_ALARM_OS_TICK_CORE0 + cpuid;
    କର {
        systimer_ll_clear_alarm_int(systimer_hal->dev, alarm_id);

        uint32_t diff = systimer_hal_get_counter_ମୂଲ୍ୟ(systimer_hal, SYSTIMER_COUNTER_OS_TICK) / systimer_ll_get_alarm_period(systimer_hal->dev, alarm_id) - s_handled_systicks[cpuid];
        ଯଦି (diff > 0) {
            ଯଦି (s_handled_systicks[cpuid] == 0) {
                s_handled_systicks[cpuid] = diff;
                diff = 1;
            } ଅନ୍ୟଥା {
                s_handled_systicks[cpuid] += diff;
            }

            କର {
                xPortSysTickHandler();
            } ଯେତେବେଳେ (--diff);
        }
    } ଯେତେବେଳେ (systimer_ll_is_alarm_int_fired(systimer_hal->dev, alarm_id));

#ifdef CONFIG_PM_TRACE
    ESP_PM_TRACE_EXIT(TICK, cpuid);
#endif
}

uint32_t xPortSysTickClaimElapsedTicks(ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu_id)
{
    uint32_t alarm_id = SYSTIMER_ALARM_OS_TICK_CORE0 + (uint32_t)cpu_id;
    uint32_t total = systimer_hal_get_counter_ମୂଲ୍ୟ(&systimer_hal, SYSTIMER_COUNTER_OS_TICK) / systimer_ll_get_alarm_period(systimer_hal.dev, alarm_id);
    uint32_t elapsed = total - s_handled_systicks[cpu_id];
    ଯଦି (elapsed == 0) {
        ଫେରନ୍ତୁ 0;
    }
    s_handled_systicks[cpu_id] = total;
    ଫେରନ୍ତୁ elapsed;
}
#endif /* CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER */

/* ------------------------------------------------ Common Port Tick ---------------------------------------------------
 * Tick related functions common across all ports
 * ------------------------------------------------------------------------------------------------------------------ */

/**
 * @brief Initialize the tick interrupt timer
 *
 * - CCOUNT timer is used if CONFIG_FREERTOS_SYSTICK_USES_CCOUNT is set
 * - SYSTIMER is used if CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER is set
 */
ଶୂନ୍ୟ vPortSetupTimer(ଶୂନ୍ୟ)
{
#ଯଦି CONFIG_FREERTOS_SYSTICK_USES_CCOUNT
    ବାହ୍ୟ ଶୂନ୍ୟ _frxt_tick_timer_init(ଶୂନ୍ୟ);
    ବାହ୍ୟ ଶୂନ୍ୟ xt_tick_ଭାଜକ_init(ଶୂନ୍ୟ);
    /* Init the tick divisor value */
    xt_tick_ଭାଜକ_init();
    _frxt_tick_timer_init();
#ଅନ୍ୟଥା /* CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER */
    vSystimerSetup();
#endif /* CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER */
}

/**
 * @brief Handler of SysTick
 *
 * The function is called from:
 *  - _frxt_timer_int for xtensa with CONFIG_FREERTOS_SYSTICK_USES_CCOUNT
 *  - SysTickIsrHandler for xtensa with CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER
 *  - SysTickIsrHandler for riscv
 */
BaseType_t xPortSysTickHandler(ଶୂନ୍ୟ)
{
#ଯଦି configBENCHMARK
    portbenchmarkIntLatency();
#endif //configBENCHMARK
    traceISR_ENTER(SYSTICK_INTR_ID);

    // Call IDF Tick Hook
    ବାହ୍ୟ ଶୂନ୍ୟ esp_vApplicationTickHook(ଶୂନ୍ୟ);
    esp_vApplicationTickHook();

    // Call FreeRTOS Increment tick function
    BaseType_t xSwitchRequired;
#ଯଦି CONFIG_FREERTOS_SMP
    UBaseType_t uxSavedStatus = taskENTER_CRITICAL_FROM_ISR();
    // Amazon SMP FreeRTOS requires that only core 0 calls xTaskIncrementTick()
#ଯଦି ( config_ଅଙ୍କ_CORES > 1 )
    ଯଦି (portGET_CORE_ID() == 0) {
        xSwitchRequired = xTaskIncrementTick();
    } ଅନ୍ୟଥା {
        xSwitchRequired = pdFALSE;
    }
#ଅନ୍ୟଥା /* configNUM_CORES > 1 */
    xSwitchRequired = xTaskIncrementTick();
#endif /* configNUM_CORES > 1 */
    taskEXIT_CRITICAL_FROM_ISR(uxSavedStatus);
#ଅନ୍ୟଥା /* !CONFIG_FREERTOS_SMP */
#ଯଦି ( config_ଅଙ୍କ_CORES > 1 )
    /*
    Multi-core IDF FreeRTOS requires that...
        - core 0 calls xTaskIncrementTick()
        - core 1 calls xTaskIncrementTickOtherCores()
    */
    ଯଦି (xPortGetCoreID() == 0) {
        xSwitchRequired = xTaskIncrementTick();
    } ଅନ୍ୟଥା {
        xSwitchRequired = xTaskIncrementTickOtherCores();
    }
#ଅନ୍ୟଥା /* configNUM_CORES > 1 */
    /*
    Vanilla (single core) FreeRTOS expects that xTaskIncrementTick() cannot be interrupted (i.e., no nested
    interrupts). Thus we have to disable interrupts before calling it.
    */
    UBaseType_t uxSavedInterruptStatus = portSET_INTERRUPT_MASK_FROM_ISR();
    xSwitchRequired = xTaskIncrementTick();
    portCLEAR_INTERRUPT_MASK_FROM_ISR(uxSavedInterruptStatus);
#endif /* configNUM_CORES > 1 */
#endif /* !CONFIG_FREERTOS_SMP */

    // Check if yield is required
    ଯଦି (xSwitchRequired != pdFALSE) {
        portYIELD_FROM_ISR();
    } ଅନ୍ୟଥା {
        traceISR_EXIT();
    }
    ଫେରନ୍ତୁ xSwitchRequired;
}
