/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#شامل <esp_types.h>
#شامل <stdio.h>
#شامل <inttypes.h>
#شامل "sdkconfig.h"
#شامل "freertos/FreeRTOS.h"
#شامل "freertos/task.h"
#شامل "freertos/semphr.h"
#شامل "freertos/queue.h"
#شامل "esp_intr_alloc.h"
#شامل "esp_cpu.h"
#شامل "unity.h"
#شامل "test_utils.h"
#شامل "freertos_performance.h"

#اگر CONFIG_IDF_TARGET_ARCH_XTENSA
#شامل "xtensa/hal.h"
#شامل "xtensa_api.h"     // Replace with interrupt allocator API (IDF-3891)
#define TEST_SET_INT_MASK(mask) xt_set_intset(mask)
#define TEST_CLR_INT_MASK(mask) xt_set_intclear(mask)
#وگرنهاگر CONFIG_IDF_TARGET_ARCH_RISCV
#شامل "riscv/interrupt.h"
#define TEST_SET_INT_MASK(mask) esprv_int_enable(mask)
#define TEST_CLR_INT_MASK(mask) esprv_int_disable(mask)
#endif

#ifndef __riscv // TODO: IDF-4416

#define SW_ISR_LEVEL_1  7

ایستا SemaphoreHandle_t sync;
ایستا SemaphoreHandle_t end_sema;
ایستا uint32_t cycle_before_trigger;
ایستا uint32_t cycle_before_exit;
ایستا uint32_t delta_enter_cycles = 0;
ایستا uint32_t delta_exit_cycles = 0;

ایستا تهی software_isr_using_parameter_vportyield(تهی *arg)
{
    (تهی)arg;
    BaseType_t yield;
    delta_enter_cycles += esp_cpu_get_cycle_شمار() - cycle_before_trigger;
    TEST_CLR_INT_MASK(1 << SW_ISR_LEVEL_1);

    xSemaphoreGiveFromISR(sync, &yield);
    portYIELD_FROM_ISR(yield);

    cycle_before_exit = esp_cpu_get_cycle_شمار();
}

ایستا تهی software_isr_using_no_argument_vportyield(تهی *arg)
{
    (تهی)arg;
    BaseType_t yield;
    delta_enter_cycles += esp_cpu_get_cycle_شمار() - cycle_before_trigger;

    TEST_CLR_INT_MASK(1 << SW_ISR_LEVEL_1);

    xSemaphoreGiveFromISR(sync, &yield);
    اگر (yield) {
        portYIELD_FROM_ISR();
    }
    cycle_before_exit = esp_cpu_get_cycle_شمار();
}

ایستا تهی test_task(تهی *arg)
{
    (تهی) arg;

    برای (صحیح i = 0; i < 10000; i++) {
        cycle_before_trigger = esp_cpu_get_cycle_شمار();
        TEST_SET_INT_MASK(1 << SW_ISR_LEVEL_1);
        xSemaphoreTake(sync, portMAX_DELAY);
        delta_exit_cycles += esp_cpu_get_cycle_شمار() - cycle_before_exit;
    }

    delta_enter_cycles /= 10000;
    delta_exit_cycles /= 10000;

    xSemaphoreGive(end_sema);
    vTaskDelete(پوچ);
}

TEST_CASE("isr latency test vport-yield-from-isr with no parameter", "[freertos] [ignore]")
{
    intr_handle_t handle;
    esp_err_t err = esp_intr_alloc(ETS_INTERNAL_SW0_INTR_SOURCE, ESP_INTR_FLAG_LEVEL1, &software_isr_using_no_argument_vportyield, پوچ, &handle);
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);

    sync = x_Semaphore_Create_دودویی();
    TEST_ASSERT(sync != پوچ);
    end_sema = x_Semaphore_Create_دودویی();
    TEST_ASSERT(end_sema != پوچ);
    xTaskCreatePinnedToCore(test_task, "tst", 4096, پوچ, configMAX_PRIORITIES - 1, پوچ, 0);
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
    esp_err_t err = esp_intr_alloc(ETS_INTERNAL_SW0_INTR_SOURCE, ESP_INTR_FLAG_LEVEL1, &software_isr_using_parameter_vportyield, پوچ, &handle);
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);

    sync = x_Semaphore_Create_دودویی();
    TEST_ASSERT(sync != پوچ);
    end_sema = x_Semaphore_Create_دودویی();
    TEST_ASSERT(end_sema != پوچ);
    xTaskCreatePinnedToCore(test_task, "tst", 4096, پوچ, configMAX_PRIORITIES - 1, پوچ, 0);
    BaseType_t result = xSemaphoreTake(end_sema, portMAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, result);
    TEST_PERFORMANCE_LESS_THAN(ISR_ENTER_CYCLES, "%"PRIu32" cycles", delta_enter_cycles);
    TEST_PERFORMANCE_LESS_THAN(ISR_EXIT_CYCLES, "%"PRIu32" cycles", delta_exit_cycles);

    esp_intr_free(handle);
}

#endif // __riscv
