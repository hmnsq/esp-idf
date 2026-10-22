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

#إذا CONFIG_IDF_هدف_ARCH_XTENSA
#ضمن "xtensa/hal.h"
#ضمن "xtensa_api.h"     // Replace with interrupt allocator API (IDF-3891)
#تعریف_کن TEST_مجموعه_INT_ماسک(ماسک) xt_مجموعه_intset(ماسک)
#تعریف_کن TEST_CLR_INT_ماسک(ماسک) xt_مجموعه_intclear(ماسک)
#وإلاإذا CONFIG_IDF_هدف_ARCH_RISCV
#ضمن "riscv/interrupt.h"
#تعریف_کن TEST_مجموعه_INT_ماسک(ماسک) esprv_int_enable(ماسک)
#تعریف_کن TEST_CLR_INT_ماسک(ماسک) esprv_int_disable(ماسک)
#پایان_اگر

#اگر_تعریف_نشده __riscv // TODO: IDF-4416

#تعریف_کن SW_ISR_LEVEL_1  7

ساكن SemaphoreHandle_t sync;
ساكن SemaphoreHandle_t انتها_sema;
ساكن uint32_t چرخه_before_trigger;
ساكن uint32_t چرخه_before_exit;
ساكن uint32_t delta_enter_چرخهها = 0;
ساكن uint32_t delta_exit_چرخهها = 0;

ساكن فراغ software_isr_using_parameter_vportyield(فراغ *arg)
{
    (فراغ)arg;
    پایه_Type_t yield;
    delta_enter_چرخهها += esp_cpu_get_چرخه_شمار() - چرخه_before_trigger;
    TEST_CLR_INT_ماسک(1 << SW_ISR_LEVEL_1);

    xSemaphoreGiveFromISR(sync, &yield);
    پورت_YIELD_FROM_ISR(yield);

    چرخه_before_exit = esp_cpu_get_چرخه_شمار();
}

ساكن فراغ software_isr_using_no_آرگومان_vportyield(فراغ *arg)
{
    (فراغ)arg;
    پایه_Type_t yield;
    delta_enter_چرخهها += esp_cpu_get_چرخه_شمار() - چرخه_before_trigger;

    TEST_CLR_INT_ماسک(1 << SW_ISR_LEVEL_1);

    xSemaphoreGiveFromISR(sync, &yield);
    إذا (yield) {
        پورت_YIELD_FROM_ISR();
    }
    چرخه_before_exit = esp_cpu_get_چرخه_شمار();
}

ساكن فراغ test_task(فراغ *arg)
{
    (فراغ) arg;

    لكل (صحيح i = 0; i < 10000; i++) {
        چرخه_before_trigger = esp_cpu_get_چرخه_شمار();
        TEST_مجموعه_INT_ماسک(1 << SW_ISR_LEVEL_1);
        xSemaphoreTake(sync, پورت_MAX_DELAY);
        delta_exit_چرخهها += esp_cpu_get_چرخه_شمار() - چرخه_before_exit;
    }

    delta_enter_چرخهها /= 10000;
    delta_exit_چرخهها /= 10000;

    xSemaphoreGive(انتها_sema);
    vTaskDelete(عدم);
}

TEST_CASE("isr latency test vport-yield-from-isr with no parameter", "[freertos] [ignore]")
{
    intr_handle_t handle;
    esp_err_t err = esp_intr_alloc(ETS_داخلی_SW0_INTR_منبع, ESP_INTR_FLAG_LEVEL1, &software_isr_using_no_آرگومان_vportyield, عدم, &handle);
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);

    sync = x_Semaphore_Create_دودویی();
    TEST_ASSERT(sync != عدم);
    انتها_sema = x_Semaphore_Create_دودویی();
    TEST_ASSERT(انتها_sema != عدم);
    x_Task_Create_Pinned_مقصد_Core(test_task, "tst", 4096, عدم, configMAX_PRIORITIES - 1, عدم, 0);
    vTaskDelay(100);
    پایه_Type_t نتیجه = xSemaphoreTake(انتها_sema, پورت_MAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, نتیجه);
    TEST_PERFORMANCE_LESS_THAN(ISR_ENTER_چرخهها, "%"PRIu32" cycles", delta_enter_چرخهها);
    TEST_PERFORMANCE_LESS_THAN(ISR_EXIT_چرخهها, "%"PRIu32" cycles", delta_exit_چرخهها);

    esp_intr_free(handle);
}

TEST_CASE("isr latency test vport-yield-from-isr with parameter", "[freertos][ignore]")
{
    intr_handle_t handle;
    esp_err_t err = esp_intr_alloc(ETS_داخلی_SW0_INTR_منبع, ESP_INTR_FLAG_LEVEL1, &software_isr_using_parameter_vportyield, عدم, &handle);
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);

    sync = x_Semaphore_Create_دودویی();
    TEST_ASSERT(sync != عدم);
    انتها_sema = x_Semaphore_Create_دودویی();
    TEST_ASSERT(انتها_sema != عدم);
    x_Task_Create_Pinned_مقصد_Core(test_task, "tst", 4096, عدم, configMAX_PRIORITIES - 1, عدم, 0);
    پایه_Type_t نتیجه = xSemaphoreTake(انتها_sema, پورت_MAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, نتیجه);
    TEST_PERFORMANCE_LESS_THAN(ISR_ENTER_چرخهها, "%"PRIu32" cycles", delta_enter_چرخهها);
    TEST_PERFORMANCE_LESS_THAN(ISR_EXIT_چرخهها, "%"PRIu32" cycles", delta_exit_چرخهها);

    esp_intr_free(handle);
}

#پایان_اگر // __riscv
