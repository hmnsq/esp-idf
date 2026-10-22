/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن <esp_types.h>
#ضمن <stdio.h>
#ضمن <inttypes.h>
#ضمن "sdkconfig.h"
#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن "freertos/semphr.h"
#ضمن "freertos/queue.h"
#ضمن "esp_intr_alloc.h"
#ضمن "esp_cpu.h"
#ضمن "unity.h"
#ضمن "test_utils.h"
#ضمن "freertos_performance.h"

#إذا CONFIG_IDF_TARGET_ARCH_XTENSA
#ضمن "xtensa/hal.h"
#ضمن "xtensa_api.h"     // Replace with interrupt allocator API (IDF-3891)
#define TEST_SET_INT_MASK(mask) xt_set_intset(mask)
#define TEST_CLR_INT_MASK(mask) xt_set_intclear(mask)
#وإلاإذا CONFIG_IDF_TARGET_ARCH_RISCV
#ضمن "riscv/interrupt.h"
#define TEST_SET_INT_MASK(mask) esprv_int_enable(mask)
#define TEST_CLR_INT_MASK(mask) esprv_int_disable(mask)
#endif

#ifndef __riscv // TODO: IDF-4416

#define SW_ISR_LEVEL_1  7

ساكن SemaphoreHandle_t sync;
ساكن SemaphoreHandle_t end_sema;
ساكن uint32_t cycle_before_trigger;
ساكن uint32_t cycle_before_exit;
ساكن uint32_t delta_enter_cycles = 0;
ساكن uint32_t delta_exit_cycles = 0;

ساكن فراغ software_isr_using_parameter_vportyield(فراغ *arg)
{
    (فراغ)arg;
    BaseType_t yield;
    delta_enter_cycles += esp_cpu_get_cycle_ژماردن() - cycle_before_trigger;
    TEST_CLR_INT_MASK(1 << SW_ISR_LEVEL_1);

    xSemaphoreGiveFromISR(sync, &yield);
    portYIELD_FROM_ISR(yield);

    cycle_before_exit = esp_cpu_get_cycle_ژماردن();
}

ساكن فراغ software_isr_using_no_argument_vportyield(فراغ *arg)
{
    (فراغ)arg;
    BaseType_t yield;
    delta_enter_cycles += esp_cpu_get_cycle_ژماردن() - cycle_before_trigger;

    TEST_CLR_INT_MASK(1 << SW_ISR_LEVEL_1);

    xSemaphoreGiveFromISR(sync, &yield);
    إذا (yield) {
        portYIELD_FROM_ISR();
    }
    cycle_before_exit = esp_cpu_get_cycle_ژماردن();
}

ساكن فراغ test_task(فراغ *arg)
{
    (فراغ) arg;

    لكل (صحيح i = 0; i < 10000; i++) {
        cycle_before_trigger = esp_cpu_get_cycle_ژماردن();
        TEST_SET_INT_MASK(1 << SW_ISR_LEVEL_1);
        xSemaphoreTake(sync, portMAX_DELAY);
        delta_exit_cycles += esp_cpu_get_cycle_ژماردن() - cycle_before_exit;
    }

    delta_enter_cycles /= 10000;
    delta_exit_cycles /= 10000;

    xSemaphoreGive(end_sema);
    vTaskDelete(عدم);
}

TEST_CASE("isr latency test vport-yield-from-isr with no parameter", "[freertos] [ignore]")
{
    intr_handle_t handle;
    esp_err_t err = esp_intr_alloc(ETS_INTERNAL_SW0_INTR_SOURCE, ESP_INTR_FLAG_LEVEL1, &software_isr_using_no_argument_vportyield, عدم, &handle);
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);

    sync = x_Semaphore_Create_دووانەیی();
    TEST_ASSERT(sync != عدم);
    end_sema = x_Semaphore_Create_دووانەیی();
    TEST_ASSERT(end_sema != عدم);
    xTaskCreatePinnedToCore(test_task, "tst", 4096, عدم, configMAX_PRIORITIES - 1, عدم, 0);
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
    esp_err_t err = esp_intr_alloc(ETS_INTERNAL_SW0_INTR_SOURCE, ESP_INTR_FLAG_LEVEL1, &software_isr_using_parameter_vportyield, عدم, &handle);
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);

    sync = x_Semaphore_Create_دووانەیی();
    TEST_ASSERT(sync != عدم);
    end_sema = x_Semaphore_Create_دووانەیی();
    TEST_ASSERT(end_sema != عدم);
    xTaskCreatePinnedToCore(test_task, "tst", 4096, عدم, configMAX_PRIORITIES - 1, عدم, 0);
    BaseType_t result = xSemaphoreTake(end_sema, portMAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, result);
    TEST_PERFORMANCE_LESS_THAN(ISR_ENTER_CYCLES, "%"PRIu32" cycles", delta_enter_cycles);
    TEST_PERFORMANCE_LESS_THAN(ISR_EXIT_CYCLES, "%"PRIu32" cycles", delta_exit_cycles);

    esp_intr_free(handle);
}

#endif // __riscv
