/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ଅନ୍ତର୍ଭାଵ <esp_types.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <inttypes.h>
#ଅନ୍ତର୍ଭାଵ "sdkconfig.h"
#ଅନ୍ତର୍ଭାଵ "freertos/FreeRTOS.h"
#ଅନ୍ତର୍ଭାଵ "freertos/task.h"
#ଅନ୍ତର୍ଭାଵ "freertos/semphr.h"
#ଅନ୍ତର୍ଭାଵ "freertos/queue.h"
#ଅନ୍ତର୍ଭାଵ "esp_intr_alloc.h"
#ଅନ୍ତର୍ଭାଵ "esp_cpu.h"
#ଅନ୍ତର୍ଭାଵ "unity.h"
#ଅନ୍ତର୍ଭାଵ "test_utils.h"
#ଅନ୍ତର୍ଭାଵ "freertos_performance.h"

#ଯଦି CONFIG_IDF_TARGET_ARCH_XTENSA
#ଅନ୍ତର୍ଭାଵ "xtensa/hal.h"
#ଅନ୍ତର୍ଭାଵ "xtensa_api.h"     // Replace with interrupt allocator API (IDF-3891)
#define TEST_SET_INT_MASK(mask) xt_set_intset(mask)
#define TEST_CLR_INT_MASK(mask) xt_set_intclear(mask)
#ନଚେତଯଦି CONFIG_IDF_TARGET_ARCH_RISCV
#ଅନ୍ତର୍ଭାଵ "riscv/interrupt.h"
#define TEST_SET_INT_MASK(mask) esprv_int_enable(mask)
#define TEST_CLR_INT_MASK(mask) esprv_int_disable(mask)
#endif

#ifndef __riscv // TODO: IDF-4416

#define SW_ISR_LEVEL_1  7

ସ୍ଥିର SemaphoreHandle_t sync;
ସ୍ଥିର SemaphoreHandle_t end_sema;
ସ୍ଥିର uint32_t cycle_before_trigger;
ସ୍ଥିର uint32_t cycle_before_exit;
ସ୍ଥିର uint32_t delta_enter_cycles = 0;
ସ୍ଥିର uint32_t delta_exit_cycles = 0;

ସ୍ଥିର ଶୂନ୍ୟ software_isr_using_parameter_vportyield(ଶୂନ୍ୟ *arg)
{
    (ଶୂନ୍ୟ)arg;
    BaseType_t yield;
    delta_enter_cycles += esp_cpu_get_cycle_ଗଣନା() - cycle_before_trigger;
    TEST_CLR_INT_MASK(1 << SW_ISR_LEVEL_1);

    xSemaphoreGiveFromISR(sync, &yield);
    portYIELD_FROM_ISR(yield);

    cycle_before_exit = esp_cpu_get_cycle_ଗଣନା();
}

ସ୍ଥିର ଶୂନ୍ୟ software_isr_using_no_argument_vportyield(ଶୂନ୍ୟ *arg)
{
    (ଶୂନ୍ୟ)arg;
    BaseType_t yield;
    delta_enter_cycles += esp_cpu_get_cycle_ଗଣନା() - cycle_before_trigger;

    TEST_CLR_INT_MASK(1 << SW_ISR_LEVEL_1);

    xSemaphoreGiveFromISR(sync, &yield);
    ଯଦି (yield) {
        portYIELD_FROM_ISR();
    }
    cycle_before_exit = esp_cpu_get_cycle_ଗଣନା();
}

ସ୍ଥିର ଶୂନ୍ୟ test_task(ଶୂନ୍ୟ *arg)
{
    (ଶୂନ୍ୟ) arg;

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < 10000; i++) {
        cycle_before_trigger = esp_cpu_get_cycle_ଗଣନା();
        TEST_SET_INT_MASK(1 << SW_ISR_LEVEL_1);
        xSemaphoreTake(sync, portMAX_DELAY);
        delta_exit_cycles += esp_cpu_get_cycle_ଗଣନା() - cycle_before_exit;
    }

    delta_enter_cycles /= 10000;
    delta_exit_cycles /= 10000;

    xSemaphoreGive(end_sema);
    vTaskDelete(ଖାଲି);
}

TEST_CASE("isr latency test vport-yield-from-isr with no parameter", "[freertos] [ignore]")
{
    intr_handle_t handle;
    esp_err_t err = esp_intr_alloc(ETS_INTERNAL_SW0_INTR_SOURCE, ESP_INTR_FLAG_LEVEL1, &software_isr_using_no_argument_vportyield, ଖାଲି, &handle);
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);

    sync = x_Semaphore_Create_ଦ୍ୱିମିକ();
    TEST_ASSERT(sync != ଖାଲି);
    end_sema = x_Semaphore_Create_ଦ୍ୱିମିକ();
    TEST_ASSERT(end_sema != ଖାଲି);
    xTaskCreatePinnedToCore(test_task, "tst", 4096, ଖାଲି, configMAX_PRIORITIES - 1, ଖାଲି, 0);
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
    esp_err_t err = esp_intr_alloc(ETS_INTERNAL_SW0_INTR_SOURCE, ESP_INTR_FLAG_LEVEL1, &software_isr_using_parameter_vportyield, ଖାଲି, &handle);
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);

    sync = x_Semaphore_Create_ଦ୍ୱିମିକ();
    TEST_ASSERT(sync != ଖାଲି);
    end_sema = x_Semaphore_Create_ଦ୍ୱିମିକ();
    TEST_ASSERT(end_sema != ଖାଲି);
    xTaskCreatePinnedToCore(test_task, "tst", 4096, ଖାଲି, configMAX_PRIORITIES - 1, ଖାଲି, 0);
    BaseType_t result = xSemaphoreTake(end_sema, portMAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, result);
    TEST_PERFORMANCE_LESS_THAN(ISR_ENTER_CYCLES, "%"PRIu32" cycles", delta_enter_cycles);
    TEST_PERFORMANCE_LESS_THAN(ISR_EXIT_CYCLES, "%"PRIu32" cycles", delta_exit_cycles);

    esp_intr_free(handle);
}

#endif // __riscv
