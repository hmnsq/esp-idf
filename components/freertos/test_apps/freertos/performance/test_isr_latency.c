/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#അന്തര്ഭാവ <esp_types.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <inttypes.h>
#അന്തര്ഭാവ "sdkconfig.h"
#അന്തര്ഭാവ "freertos/FreeRTOS.h"
#അന്തര്ഭാവ "freertos/task.h"
#അന്തര്ഭാവ "freertos/semphr.h"
#അന്തര്ഭാവ "freertos/queue.h"
#അന്തര്ഭാവ "esp_intr_alloc.h"
#അന്തര്ഭാവ "esp_cpu.h"
#അന്തര്ഭാവ "unity.h"
#അന്തര്ഭാവ "test_utils.h"
#അന്തര്ഭാവ "freertos_performance.h"

#എങ്കിൽ CONFIG_IDF_TARGET_ARCH_XTENSA
#അന്തര്ഭാവ "xtensa/hal.h"
#അന്തര്ഭാവ "xtensa_api.h"     // Replace with interrupt allocator API (IDF-3891)
#define TEST_SET_INT_MASK(mask) xt_set_intset(mask)
#define TEST_CLR_INT_MASK(mask) xt_set_intclear(mask)
#അല്ലെങ്കിലെങ്കിൽ CONFIG_IDF_TARGET_ARCH_RISCV
#അന്തര്ഭാവ "riscv/interrupt.h"
#define TEST_SET_INT_MASK(mask) esprv_int_enable(mask)
#define TEST_CLR_INT_MASK(mask) esprv_int_disable(mask)
#endif

#ifndef __riscv // TODO: IDF-4416

#define SW_ISR_LEVEL_1  7

സ്ഥിതം SemaphoreHandle_t sync;
സ്ഥിതം SemaphoreHandle_t end_sema;
സ്ഥിതം uint32_t cycle_before_trigger;
സ്ഥിതം uint32_t cycle_before_exit;
സ്ഥിതം uint32_t delta_enter_cycles = 0;
സ്ഥിതം uint32_t delta_exit_cycles = 0;

സ്ഥിതം ശൂന്യം software_isr_using_parameter_vportyield(ശൂന്യം *arg)
{
    (ശൂന്യം)arg;
    BaseType_t yield;
    delta_enter_cycles += esp_cpu_get_cycle_എണ്ണം() - cycle_before_trigger;
    TEST_CLR_INT_MASK(1 << SW_ISR_LEVEL_1);

    xSemaphoreGiveFromISR(sync, &yield);
    portYIELD_FROM_ISR(yield);

    cycle_before_exit = esp_cpu_get_cycle_എണ്ണം();
}

സ്ഥിതം ശൂന്യം software_isr_using_no_argument_vportyield(ശൂന്യം *arg)
{
    (ശൂന്യം)arg;
    BaseType_t yield;
    delta_enter_cycles += esp_cpu_get_cycle_എണ്ണം() - cycle_before_trigger;

    TEST_CLR_INT_MASK(1 << SW_ISR_LEVEL_1);

    xSemaphoreGiveFromISR(sync, &yield);
    എങ്കിൽ (yield) {
        portYIELD_FROM_ISR();
    }
    cycle_before_exit = esp_cpu_get_cycle_എണ്ണം();
}

സ്ഥിതം ശൂന്യം test_task(ശൂന്യം *arg)
{
    (ശൂന്യം) arg;

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < 10000; i++) {
        cycle_before_trigger = esp_cpu_get_cycle_എണ്ണം();
        TEST_SET_INT_MASK(1 << SW_ISR_LEVEL_1);
        xSemaphoreTake(sync, portMAX_DELAY);
        delta_exit_cycles += esp_cpu_get_cycle_എണ്ണം() - cycle_before_exit;
    }

    delta_enter_cycles /= 10000;
    delta_exit_cycles /= 10000;

    xSemaphoreGive(end_sema);
    vTaskDelete(ശൂന്യ);
}

TEST_CASE("isr latency test vport-yield-from-isr with no parameter", "[freertos] [ignore]")
{
    intr_handle_t handle;
    esp_err_t err = esp_intr_alloc(ETS_INTERNAL_SW0_INTR_SOURCE, ESP_INTR_FLAG_LEVEL1, &software_isr_using_no_argument_vportyield, ശൂന്യ, &handle);
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);

    sync = x_Semaphore_Create_ദ്വിമാന();
    TEST_ASSERT(sync != ശൂന്യ);
    end_sema = x_Semaphore_Create_ദ്വിമാന();
    TEST_ASSERT(end_sema != ശൂന്യ);
    xTaskCreatePinnedToCore(test_task, "tst", 4096, ശൂന്യ, configMAX_PRIORITIES - 1, ശൂന്യ, 0);
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
    esp_err_t err = esp_intr_alloc(ETS_INTERNAL_SW0_INTR_SOURCE, ESP_INTR_FLAG_LEVEL1, &software_isr_using_parameter_vportyield, ശൂന്യ, &handle);
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);

    sync = x_Semaphore_Create_ദ്വിമാന();
    TEST_ASSERT(sync != ശൂന്യ);
    end_sema = x_Semaphore_Create_ദ്വിമാന();
    TEST_ASSERT(end_sema != ശൂന്യ);
    xTaskCreatePinnedToCore(test_task, "tst", 4096, ശൂന്യ, configMAX_PRIORITIES - 1, ശൂന്യ, 0);
    BaseType_t result = xSemaphoreTake(end_sema, portMAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, result);
    TEST_PERFORMANCE_LESS_THAN(ISR_ENTER_CYCLES, "%"PRIu32" cycles", delta_enter_cycles);
    TEST_PERFORMANCE_LESS_THAN(ISR_EXIT_CYCLES, "%"PRIu32" cycles", delta_exit_cycles);

    esp_intr_free(handle);
}

#endif // __riscv
