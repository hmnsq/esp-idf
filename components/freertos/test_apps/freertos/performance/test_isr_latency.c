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

#إذا CONFIG_IDF_نشانہ_ARCH_XTENSA
#ضمن "xtensa/hal.h"
#ضمن "xtensa_api.h"     // Replace with interrupt allocator API (IDF-3891)
#تعریف_کرو TEST_سیٹ_INT_نقاب(نقاب) xt_سیٹ_intset(نقاب)
#تعریف_کرو TEST_CLR_INT_نقاب(نقاب) xt_سیٹ_intclear(نقاب)
#وإلاإذا CONFIG_IDF_نشانہ_ARCH_RISCV
#ضمن "riscv/interrupt.h"
#تعریف_کرو TEST_سیٹ_INT_نقاب(نقاب) esprv_int_enable(نقاب)
#تعریف_کرو TEST_CLR_INT_نقاب(نقاب) esprv_int_disable(نقاب)
#جے_انت

#جے_بنا_تعریف __riscv // TODO: IDF-4416

#تعریف_کرو SW_ISR_LEVEL_1  7

ساكن SemaphoreHandle_t sync;
ساكن SemaphoreHandle_t سرا_sema;
ساكن uint32_t چکر_before_trigger;
ساكن uint32_t چکر_before_exit;
ساكن uint32_t delta_enter_چکراں = 0;
ساكن uint32_t delta_exit_چکراں = 0;

ساكن فراغ software_isr_using_parameter_vportyield(فراغ *arg)
{
    (فراغ)arg;
    ادھار_Type_t yield;
    delta_enter_چکراں += esp_cpu_get_چکر_گنتی() - چکر_before_trigger;
    TEST_CLR_INT_نقاب(1 << SW_ISR_LEVEL_1);

    xSemaphoreGiveFromISR(sync, &yield);
    پورٹ_YIELD_FROM_ISR(yield);

    چکر_before_exit = esp_cpu_get_چکر_گنتی();
}

ساكن فراغ software_isr_using_no_دلیل_vportyield(فراغ *arg)
{
    (فراغ)arg;
    ادھار_Type_t yield;
    delta_enter_چکراں += esp_cpu_get_چکر_گنتی() - چکر_before_trigger;

    TEST_CLR_INT_نقاب(1 << SW_ISR_LEVEL_1);

    xSemaphoreGiveFromISR(sync, &yield);
    إذا (yield) {
        پورٹ_YIELD_FROM_ISR();
    }
    چکر_before_exit = esp_cpu_get_چکر_گنتی();
}

ساكن فراغ test_task(فراغ *arg)
{
    (فراغ) arg;

    لكل (صحيح i = 0; i < 10000; i++) {
        چکر_before_trigger = esp_cpu_get_چکر_گنتی();
        TEST_سیٹ_INT_نقاب(1 << SW_ISR_LEVEL_1);
        xSemaphoreTake(sync, پورٹ_MAX_DELAY);
        delta_exit_چکراں += esp_cpu_get_چکر_گنتی() - چکر_before_exit;
    }

    delta_enter_چکراں /= 10000;
    delta_exit_چکراں /= 10000;

    xSemaphoreGive(سرا_sema);
    vTaskDelete(عدم);
}

TEST_CASE("isr latency test vport-yield-from-isr with no parameter", "[freertos] [ignore]")
{
    intr_handle_t handle;
    esp_err_t err = esp_intr_alloc(ETS_اندرونی_SW0_INTR_سروت, ESP_INTR_FLAG_LEVEL1, &software_isr_using_no_دلیل_vportyield, عدم, &handle);
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);

    sync = x_Semaphore_Create_ثنائی();
    TEST_ASSERT(sync != عدم);
    سرا_sema = x_Semaphore_Create_ثنائی();
    TEST_ASSERT(سرا_sema != عدم);
    x_Task_Create_Pinned_منزل_Core(test_task, "tst", 4096, عدم, configMAX_PRIORITIES - 1, عدم, 0);
    vTaskDelay(100);
    ادھار_Type_t نتیجہ = xSemaphoreTake(سرا_sema, پورٹ_MAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, نتیجہ);
    TEST_PERFORMANCE_LESS_THAN(ISR_ENTER_چکراں, "%"PRIu32" cycles", delta_enter_چکراں);
    TEST_PERFORMANCE_LESS_THAN(ISR_EXIT_چکراں, "%"PRIu32" cycles", delta_exit_چکراں);

    esp_intr_free(handle);
}

TEST_CASE("isr latency test vport-yield-from-isr with parameter", "[freertos][ignore]")
{
    intr_handle_t handle;
    esp_err_t err = esp_intr_alloc(ETS_اندرونی_SW0_INTR_سروت, ESP_INTR_FLAG_LEVEL1, &software_isr_using_parameter_vportyield, عدم, &handle);
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);

    sync = x_Semaphore_Create_ثنائی();
    TEST_ASSERT(sync != عدم);
    سرا_sema = x_Semaphore_Create_ثنائی();
    TEST_ASSERT(سرا_sema != عدم);
    x_Task_Create_Pinned_منزل_Core(test_task, "tst", 4096, عدم, configMAX_PRIORITIES - 1, عدم, 0);
    ادھار_Type_t نتیجہ = xSemaphoreTake(سرا_sema, پورٹ_MAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, نتیجہ);
    TEST_PERFORMANCE_LESS_THAN(ISR_ENTER_چکراں, "%"PRIu32" cycles", delta_enter_چکراں);
    TEST_PERFORMANCE_LESS_THAN(ISR_EXIT_چکراں, "%"PRIu32" cycles", delta_exit_چکراں);

    esp_intr_free(handle);
}

#جے_انت // __riscv
