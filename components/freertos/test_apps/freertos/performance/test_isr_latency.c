/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#અન્તર્ભાવ <esp_types.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <inttypes.h>
#અન્તર્ભાવ "sdkconfig.h"
#અન્તર્ભાવ "freertos/FreeRTOS.h"
#અન્તર્ભાવ "freertos/task.h"
#અન્તર્ભાવ "freertos/semphr.h"
#અન્તર્ભાવ "freertos/queue.h"
#અન્તર્ભાવ "esp_intr_alloc.h"
#અન્તર્ભાવ "esp_cpu.h"
#અન્તર્ભાવ "unity.h"
#અન્તર્ભાવ "test_utils.h"
#અન્તર્ભાવ "freertos_performance.h"

#જો CONFIG_IDF_TARGET_ARCH_XTENSA
#અન્તર્ભાવ "xtensa/hal.h"
#અન્તર્ભાવ "xtensa_api.h"     // Replace with interrupt allocator API (IDF-3891)
#define TEST_SET_INT_MASK(mask) xt_set_intset(mask)
#define TEST_CLR_INT_MASK(mask) xt_set_intclear(mask)
#અથવાજો CONFIG_IDF_TARGET_ARCH_RISCV
#અન્તર્ભાવ "riscv/interrupt.h"
#define TEST_SET_INT_MASK(mask) esprv_int_enable(mask)
#define TEST_CLR_INT_MASK(mask) esprv_int_disable(mask)
#endif

#ifndef __riscv // TODO: IDF-4416

#define SW_ISR_LEVEL_1  7

સ્થિર SemaphoreHandle_t sync;
સ્થિર SemaphoreHandle_t end_sema;
સ્થિર uint32_t cycle_before_trigger;
સ્થિર uint32_t cycle_before_exit;
સ્થિર uint32_t delta_enter_cycles = 0;
સ્થિર uint32_t delta_exit_cycles = 0;

સ્થિર શૂન્ય software_isr_using_parameter_vportyield(શૂન્ય *arg)
{
    (શૂન્ય)arg;
    BaseType_t yield;
    delta_enter_cycles += esp_cpu_get_cycle_ગણતરી() - cycle_before_trigger;
    TEST_CLR_INT_MASK(1 << SW_ISR_LEVEL_1);

    xSemaphoreGiveFromISR(sync, &yield);
    portYIELD_FROM_ISR(yield);

    cycle_before_exit = esp_cpu_get_cycle_ગણતરી();
}

સ્થિર શૂન્ય software_isr_using_no_argument_vportyield(શૂન્ય *arg)
{
    (શૂન્ય)arg;
    BaseType_t yield;
    delta_enter_cycles += esp_cpu_get_cycle_ગણતરી() - cycle_before_trigger;

    TEST_CLR_INT_MASK(1 << SW_ISR_LEVEL_1);

    xSemaphoreGiveFromISR(sync, &yield);
    જો (yield) {
        portYIELD_FROM_ISR();
    }
    cycle_before_exit = esp_cpu_get_cycle_ગણતરી();
}

સ્થિર શૂન્ય test_task(શૂન્ય *arg)
{
    (શૂન્ય) arg;

    માટે (પૂર્ણાંક i = 0; i < 10000; i++) {
        cycle_before_trigger = esp_cpu_get_cycle_ગણતરી();
        TEST_SET_INT_MASK(1 << SW_ISR_LEVEL_1);
        xSemaphoreTake(sync, portMAX_DELAY);
        delta_exit_cycles += esp_cpu_get_cycle_ગણતરી() - cycle_before_exit;
    }

    delta_enter_cycles /= 10000;
    delta_exit_cycles /= 10000;

    xSemaphoreGive(end_sema);
    vTaskDelete(ખાલી);
}

TEST_CASE("isr latency test vport-yield-from-isr with no parameter", "[freertos] [ignore]")
{
    intr_handle_t handle;
    esp_err_t err = esp_intr_alloc(ETS_INTERNAL_SW0_INTR_SOURCE, ESP_INTR_FLAG_LEVEL1, &software_isr_using_no_argument_vportyield, ખાલી, &handle);
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);

    sync = x_Semaphore_Create_દ્વિઅંકી();
    TEST_ASSERT(sync != ખાલી);
    end_sema = x_Semaphore_Create_દ્વિઅંકી();
    TEST_ASSERT(end_sema != ખાલી);
    xTaskCreatePinnedToCore(test_task, "tst", 4096, ખાલી, configMAX_PRIORITIES - 1, ખાલી, 0);
    vTaskDelay(100);
    BaseType_t result = xSemaphoreTake(end_sema, portMAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, result);
    TEST_PERFORMANCE_LESS_THAN(ISR_ENTER_CYCLES, "%"PRIu32" cycles", delta_enter_cycles);
    TEST_PERFORMANCE_LESS_THAN(ISR_EXIT_CYCLES, "%"PRIu32" cycles", delta_exit_cycles);

    esp_intr_free(handle);
}

TEST_CASE("isr latency test vport-yield-from-isr with parameter", "[freertos][ignore]")
{
    intr_handle_t handle;
    esp_err_t err = esp_intr_alloc(ETS_INTERNAL_SW0_INTR_SOURCE, ESP_INTR_FLAG_LEVEL1, &software_isr_using_parameter_vportyield, ખાલી, &handle);
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);

    sync = x_Semaphore_Create_દ્વિઅંકી();
    TEST_ASSERT(sync != ખાલી);
    end_sema = x_Semaphore_Create_દ્વિઅંકી();
    TEST_ASSERT(end_sema != ખાલી);
    xTaskCreatePinnedToCore(test_task, "tst", 4096, ખાલી, configMAX_PRIORITIES - 1, ખાલી, 0);
    BaseType_t result = xSemaphoreTake(end_sema, portMAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, result);
    TEST_PERFORMANCE_LESS_THAN(ISR_ENTER_CYCLES, "%"PRIu32" cycles", delta_enter_cycles);
    TEST_PERFORMANCE_LESS_THAN(ISR_EXIT_CYCLES, "%"PRIu32" cycles", delta_exit_cycles);

    esp_intr_free(handle);
}

#endif // __riscv
