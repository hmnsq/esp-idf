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

#اگر CONFIG_IDF_ہدف_ARCH_XTENSA
#شامل "xtensa/hal.h"
#شامل "xtensa_api.h"     // Replace with interrupt allocator API (IDF-3891)
#تعریف_کرو TEST_سیٹ_INT_نقاب(نقاب) xt_سیٹ_intset(نقاب)
#تعریف_کرو TEST_CLR_INT_نقاب(نقاب) xt_سیٹ_intclear(نقاب)
#وگرنہ_اگر CONFIG_IDF_ہدف_ARCH_RISCV
#شامل "riscv/interrupt.h"
#تعریف_کرو TEST_سیٹ_INT_نقاب(نقاب) esprv_int_enable(نقاب)
#تعریف_کرو TEST_CLR_INT_نقاب(نقاب) esprv_int_disable(نقاب)
#اگر_ختم

#اگر_غیر_تعریف_شدہ __riscv // TODO: IDF-4416

#تعریف_کرو SW_ISR_LEVEL_1  7

ساکن SemaphoreHandle_t sync;
ساکن SemaphoreHandle_t انتہا_sema;
ساکن uint32_t چکر_before_trigger;
ساکن uint32_t چکر_before_exit;
ساکن uint32_t delta_enter_ادوار = 0;
ساکن uint32_t delta_exit_ادوار = 0;

ساکن خالی software_isr_using_parameter_vportyield(خالی *arg)
{
    (خالی)arg;
    بنیاد_Type_t yield;
    delta_enter_ادوار += esp_cpu_get_چکر_گنتی() - چکر_before_trigger;
    TEST_CLR_INT_نقاب(1 << SW_ISR_LEVEL_1);

    xSemaphoreGiveFromISR(sync, &yield);
    پورٹ_YIELD_FROM_ISR(yield);

    چکر_before_exit = esp_cpu_get_چکر_گنتی();
}

ساکن خالی software_isr_using_no_دلیل_vportyield(خالی *arg)
{
    (خالی)arg;
    بنیاد_Type_t yield;
    delta_enter_ادوار += esp_cpu_get_چکر_گنتی() - چکر_before_trigger;

    TEST_CLR_INT_نقاب(1 << SW_ISR_LEVEL_1);

    xSemaphoreGiveFromISR(sync, &yield);
    اگر (yield) {
        پورٹ_YIELD_FROM_ISR();
    }
    چکر_before_exit = esp_cpu_get_چکر_گنتی();
}

ساکن خالی test_task(خالی *arg)
{
    (خالی) arg;

    برائے (صحیح i = 0; i < 10000; i++) {
        چکر_before_trigger = esp_cpu_get_چکر_گنتی();
        TEST_سیٹ_INT_نقاب(1 << SW_ISR_LEVEL_1);
        xSemaphoreTake(sync, پورٹ_MAX_DELAY);
        delta_exit_ادوار += esp_cpu_get_چکر_گنتی() - چکر_before_exit;
    }

    delta_enter_ادوار /= 10000;
    delta_exit_ادوار /= 10000;

    xSemaphoreGive(انتہا_sema);
    vTaskDelete(تہی);
}

TEST_CASE("isr latency test vport-yield-from-isr with no parameter", "[freertos] [ignore]")
{
    intr_handle_t handle;
    esp_err_t err = esp_intr_alloc(ETS_اندرونی_SW0_INTR_ماخذ, ESP_INTR_FLAG_LEVEL1, &software_isr_using_no_دلیل_vportyield, تہی, &handle);
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);

    sync = x_Semaphore_Create_ثنائی();
    TEST_ASSERT(sync != تہی);
    انتہا_sema = x_Semaphore_Create_ثنائی();
    TEST_ASSERT(انتہا_sema != تہی);
    x_Task_Create_Pinned_مقصد_Core(test_task, "tst", 4096, تہی, configMAX_PRIORITIES - 1, تہی, 0);
    vTaskDelay(100);
    بنیاد_Type_t نتیجہ = xSemaphoreTake(انتہا_sema, پورٹ_MAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, نتیجہ);
    TEST_PERFORMANCE_LESS_THAN(ISR_ENTER_ادوار, "%"PRIu32" cycles", delta_enter_ادوار);
    TEST_PERFORMANCE_LESS_THAN(ISR_EXIT_ادوار, "%"PRIu32" cycles", delta_exit_ادوار);

    esp_intr_free(handle);
}

TEST_CASE("isr latency test vport-yield-from-isr with parameter", "[freertos][ignore]")
{
    intr_handle_t handle;
    esp_err_t err = esp_intr_alloc(ETS_اندرونی_SW0_INTR_ماخذ, ESP_INTR_FLAG_LEVEL1, &software_isr_using_parameter_vportyield, تہی, &handle);
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);

    sync = x_Semaphore_Create_ثنائی();
    TEST_ASSERT(sync != تہی);
    انتہا_sema = x_Semaphore_Create_ثنائی();
    TEST_ASSERT(انتہا_sema != تہی);
    x_Task_Create_Pinned_مقصد_Core(test_task, "tst", 4096, تہی, configMAX_PRIORITIES - 1, تہی, 0);
    بنیاد_Type_t نتیجہ = xSemaphoreTake(انتہا_sema, پورٹ_MAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, نتیجہ);
    TEST_PERFORMANCE_LESS_THAN(ISR_ENTER_ادوار, "%"PRIu32" cycles", delta_enter_ادوار);
    TEST_PERFORMANCE_LESS_THAN(ISR_EXIT_ادوار, "%"PRIu32" cycles", delta_exit_ادوار);

    esp_intr_free(handle);
}

#اگر_ختم // __riscv
