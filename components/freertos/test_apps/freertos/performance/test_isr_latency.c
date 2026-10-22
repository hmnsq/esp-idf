/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#அந்தர்பாவ <esp_types.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <inttypes.h>
#அந்தர்பாவ "sdkconfig.h"
#அந்தர்பாவ "freertos/FreeRTOS.h"
#அந்தர்பாவ "freertos/task.h"
#அந்தர்பாவ "freertos/semphr.h"
#அந்தர்பாவ "freertos/queue.h"
#அந்தர்பாவ "esp_intr_alloc.h"
#அந்தர்பாவ "esp_cpu.h"
#அந்தர்பாவ "unity.h"
#அந்தர்பாவ "test_utils.h"
#அந்தர்பாவ "freertos_performance.h"

#எனில் CONFIG_IDF_TARGET_ARCH_XTENSA
#அந்தர்பாவ "xtensa/hal.h"
#அந்தர்பாவ "xtensa_api.h"     // Replace with interrupt allocator API (IDF-3891)
#define TEST_SET_INT_MASK(mask) xt_set_intset(mask)
#define TEST_CLR_INT_MASK(mask) xt_set_intclear(mask)
#அல்லதெனில் CONFIG_IDF_TARGET_ARCH_RISCV
#அந்தர்பாவ "riscv/interrupt.h"
#define TEST_SET_INT_MASK(mask) esprv_int_enable(mask)
#define TEST_CLR_INT_MASK(mask) esprv_int_disable(mask)
#endif

#ifndef __riscv // TODO: IDF-4416

#define SW_ISR_LEVEL_1  7

நிலையான SemaphoreHandle_t sync;
நிலையான SemaphoreHandle_t end_sema;
நிலையான uint32_t cycle_before_trigger;
நிலையான uint32_t cycle_before_exit;
நிலையான uint32_t delta_enter_cycles = 0;
நிலையான uint32_t delta_exit_cycles = 0;

நிலையான வெறுமை software_isr_using_parameter_vportyield(வெறுமை *arg)
{
    (வெறுமை)arg;
    BaseType_t yield;
    delta_enter_cycles += esp_cpu_get_cycle_மொத்தம்() - cycle_before_trigger;
    TEST_CLR_INT_MASK(1 << SW_ISR_LEVEL_1);

    xSemaphoreGiveFromISR(sync, &yield);
    portYIELD_FROM_ISR(yield);

    cycle_before_exit = esp_cpu_get_cycle_மொத்தம்();
}

நிலையான வெறுமை software_isr_using_no_argument_vportyield(வெறுமை *arg)
{
    (வெறுமை)arg;
    BaseType_t yield;
    delta_enter_cycles += esp_cpu_get_cycle_மொத்தம்() - cycle_before_trigger;

    TEST_CLR_INT_MASK(1 << SW_ISR_LEVEL_1);

    xSemaphoreGiveFromISR(sync, &yield);
    எனில் (yield) {
        portYIELD_FROM_ISR();
    }
    cycle_before_exit = esp_cpu_get_cycle_மொத்தம்();
}

நிலையான வெறுமை test_task(வெறுமை *arg)
{
    (வெறுமை) arg;

    க்கு (முழு i = 0; i < 10000; i++) {
        cycle_before_trigger = esp_cpu_get_cycle_மொத்தம்();
        TEST_SET_INT_MASK(1 << SW_ISR_LEVEL_1);
        xSemaphoreTake(sync, portMAX_DELAY);
        delta_exit_cycles += esp_cpu_get_cycle_மொத்தம்() - cycle_before_exit;
    }

    delta_enter_cycles /= 10000;
    delta_exit_cycles /= 10000;

    xSemaphoreGive(end_sema);
    vTaskDelete(வெற்று);
}

TEST_CASE("isr latency test vport-yield-from-isr with no parameter", "[freertos] [ignore]")
{
    intr_handle_t handle;
    esp_err_t err = esp_intr_alloc(ETS_INTERNAL_SW0_INTR_SOURCE, ESP_INTR_FLAG_LEVEL1, &software_isr_using_no_argument_vportyield, வெற்று, &handle);
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);

    sync = x_Semaphore_Create_இருமம்();
    TEST_ASSERT(sync != வெற்று);
    end_sema = x_Semaphore_Create_இருமம்();
    TEST_ASSERT(end_sema != வெற்று);
    xTaskCreatePinnedToCore(test_task, "tst", 4096, வெற்று, configMAX_PRIORITIES - 1, வெற்று, 0);
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
    esp_err_t err = esp_intr_alloc(ETS_INTERNAL_SW0_INTR_SOURCE, ESP_INTR_FLAG_LEVEL1, &software_isr_using_parameter_vportyield, வெற்று, &handle);
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);

    sync = x_Semaphore_Create_இருமம்();
    TEST_ASSERT(sync != வெற்று);
    end_sema = x_Semaphore_Create_இருமம்();
    TEST_ASSERT(end_sema != வெற்று);
    xTaskCreatePinnedToCore(test_task, "tst", 4096, வெற்று, configMAX_PRIORITIES - 1, வெற்று, 0);
    BaseType_t result = xSemaphoreTake(end_sema, portMAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, result);
    TEST_PERFORMANCE_LESS_THAN(ISR_ENTER_CYCLES, "%"PRIu32" cycles", delta_enter_cycles);
    TEST_PERFORMANCE_LESS_THAN(ISR_EXIT_CYCLES, "%"PRIu32" cycles", delta_exit_cycles);

    esp_intr_free(handle);
}

#endif // __riscv
