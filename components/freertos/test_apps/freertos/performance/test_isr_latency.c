/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#অন্তর্ভাব <esp_types.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <inttypes.h>
#অন্তর্ভাব "sdkconfig.h"
#অন্তর্ভাব "freertos/FreeRTOS.h"
#অন্তর্ভাব "freertos/task.h"
#অন্তর্ভাব "freertos/semphr.h"
#অন্তর্ভাব "freertos/queue.h"
#অন্তর্ভাব "esp_intr_alloc.h"
#অন্তর্ভাব "esp_cpu.h"
#অন্তর্ভাব "unity.h"
#অন্তর্ভাব "test_utils.h"
#অন্তর্ভাব "freertos_performance.h"

#যদি CONFIG_IDF_TARGET_ARCH_XTENSA
#অন্তর্ভাব "xtensa/hal.h"
#অন্তর্ভাব "xtensa_api.h"     // Replace with interrupt allocator API (IDF-3891)
#define TEST_SET_INT_MASK(mask) xt_set_intset(mask)
#define TEST_CLR_INT_MASK(mask) xt_set_intclear(mask)
#নোচেদ্যদি CONFIG_IDF_TARGET_ARCH_RISCV
#অন্তর্ভাব "riscv/interrupt.h"
#define TEST_SET_INT_MASK(mask) esprv_int_enable(mask)
#define TEST_CLR_INT_MASK(mask) esprv_int_disable(mask)
#endif

#ifndef __riscv // TODO: IDF-4416

#define SW_ISR_LEVEL_1  7

স্থির SemaphoreHandle_t sync;
স্থির SemaphoreHandle_t end_sema;
স্থির uint32_t cycle_before_trigger;
স্থির uint32_t cycle_before_exit;
স্থির uint32_t delta_enter_cycles = 0;
স্থির uint32_t delta_exit_cycles = 0;

স্থির শূন্য software_isr_using_parameter_vportyield(শূন্য *arg)
{
    (শূন্য)arg;
    BaseType_t yield;
    delta_enter_cycles += esp_cpu_get_cycle_গুনতি() - cycle_before_trigger;
    TEST_CLR_INT_MASK(1 << SW_ISR_LEVEL_1);

    xSemaphoreGiveFromISR(sync, &yield);
    portYIELD_FROM_ISR(yield);

    cycle_before_exit = esp_cpu_get_cycle_গুনতি();
}

স্থির শূন্য software_isr_using_no_argument_vportyield(শূন্য *arg)
{
    (শূন্য)arg;
    BaseType_t yield;
    delta_enter_cycles += esp_cpu_get_cycle_গুনতি() - cycle_before_trigger;

    TEST_CLR_INT_MASK(1 << SW_ISR_LEVEL_1);

    xSemaphoreGiveFromISR(sync, &yield);
    যদি (yield) {
        portYIELD_FROM_ISR();
    }
    cycle_before_exit = esp_cpu_get_cycle_গুনতি();
}

স্থির শূন্য test_task(শূন্য *arg)
{
    (শূন্য) arg;

    প্রতি (পূর্ণাঙ্ক i = 0; i < 10000; i++) {
        cycle_before_trigger = esp_cpu_get_cycle_গুনতি();
        TEST_SET_INT_MASK(1 << SW_ISR_LEVEL_1);
        xSemaphoreTake(sync, portMAX_DELAY);
        delta_exit_cycles += esp_cpu_get_cycle_গুনতি() - cycle_before_exit;
    }

    delta_enter_cycles /= 10000;
    delta_exit_cycles /= 10000;

    xSemaphoreGive(end_sema);
    vTaskDelete(রিক্ত);
}

TEST_CASE("isr latency test vport-yield-from-isr with no parameter", "[freertos] [ignore]")
{
    intr_handle_t handle;
    esp_err_t err = esp_intr_alloc(ETS_INTERNAL_SW0_INTR_SOURCE, ESP_INTR_FLAG_LEVEL1, &software_isr_using_no_argument_vportyield, রিক্ত, &handle);
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);

    sync = x_Semaphore_Create_দ্বিমিক();
    TEST_ASSERT(sync != রিক্ত);
    end_sema = x_Semaphore_Create_দ্বিমিক();
    TEST_ASSERT(end_sema != রিক্ত);
    xTaskCreatePinnedToCore(test_task, "tst", 4096, রিক্ত, configMAX_PRIORITIES - 1, রিক্ত, 0);
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
    esp_err_t err = esp_intr_alloc(ETS_INTERNAL_SW0_INTR_SOURCE, ESP_INTR_FLAG_LEVEL1, &software_isr_using_parameter_vportyield, রিক্ত, &handle);
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);

    sync = x_Semaphore_Create_দ্বিমিক();
    TEST_ASSERT(sync != রিক্ত);
    end_sema = x_Semaphore_Create_দ্বিমিক();
    TEST_ASSERT(end_sema != রিক্ত);
    xTaskCreatePinnedToCore(test_task, "tst", 4096, রিক্ত, configMAX_PRIORITIES - 1, রিক্ত, 0);
    BaseType_t result = xSemaphoreTake(end_sema, portMAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, result);
    TEST_PERFORMANCE_LESS_THAN(ISR_ENTER_CYCLES, "%"PRIu32" cycles", delta_enter_cycles);
    TEST_PERFORMANCE_LESS_THAN(ISR_EXIT_CYCLES, "%"PRIu32" cycles", delta_exit_cycles);

    esp_intr_free(handle);
}

#endif // __riscv
