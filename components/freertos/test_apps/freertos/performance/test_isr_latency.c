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

#إذا CONFIG_IDF_موخه_ARCH_XTENSA
#ضمن "xtensa/hal.h"
#ضمن "xtensa_api.h"     // Replace with interrupt allocator API (IDF-3891)
#تعریف_کړه TEST_سېټ_INT_ماسک(ماسک) xt_سېټ_intset(ماسک)
#تعریف_کړه TEST_CLR_INT_ماسک(ماسک) xt_سېټ_intclear(ماسک)
#وإلاإذا CONFIG_IDF_موخه_ARCH_RISCV
#ضمن "riscv/interrupt.h"
#تعریف_کړه TEST_سېټ_INT_ماسک(ماسک) esprv_int_enable(ماسک)
#تعریف_کړه TEST_CLR_INT_ماسک(ماسک) esprv_int_disable(ماسک)
#د_که_پای

#که_نه_تعریف_شوی __riscv // TODO: IDF-4416

#تعریف_کړه SW_ISR_LEVEL_1  7

ساكن SemaphoreHandle_t sync;
ساكن SemaphoreHandle_t پای_ټکی_sema;
ساكن uint32_t دوره_before_trigger;
ساكن uint32_t دوره_before_exit;
ساكن uint32_t delta_enter_دورې = 0;
ساكن uint32_t delta_exit_دورې = 0;

ساكن فراغ software_isr_using_parameter_vportyield(فراغ *arg)
{
    (فراغ)arg;
    بنسټ_Type_t yield;
    delta_enter_دورې += esp_cpu_get_دوره_شمېر() - دوره_before_trigger;
    TEST_CLR_INT_ماسک(1 << SW_ISR_LEVEL_1);

    xSemaphoreGiveFromISR(sync, &yield);
    پورټ_YIELD_FROM_ISR(yield);

    دوره_before_exit = esp_cpu_get_دوره_شمېر();
}

ساكن فراغ software_isr_using_no_دلیل_vportyield(فراغ *arg)
{
    (فراغ)arg;
    بنسټ_Type_t yield;
    delta_enter_دورې += esp_cpu_get_دوره_شمېر() - دوره_before_trigger;

    TEST_CLR_INT_ماسک(1 << SW_ISR_LEVEL_1);

    xSemaphoreGiveFromISR(sync, &yield);
    إذا (yield) {
        پورټ_YIELD_FROM_ISR();
    }
    دوره_before_exit = esp_cpu_get_دوره_شمېر();
}

ساكن فراغ test_task(فراغ *arg)
{
    (فراغ) arg;

    لكل (صحيح i = 0; i < 10000; i++) {
        دوره_before_trigger = esp_cpu_get_دوره_شمېر();
        TEST_سېټ_INT_ماسک(1 << SW_ISR_LEVEL_1);
        xSemaphoreTake(sync, پورټ_MAX_DELAY);
        delta_exit_دورې += esp_cpu_get_دوره_شمېر() - دوره_before_exit;
    }

    delta_enter_دورې /= 10000;
    delta_exit_دورې /= 10000;

    xSemaphoreGive(پای_ټکی_sema);
    vTaskDelete(عدم);
}

TEST_CASE("isr latency test vport-yield-from-isr with no parameter", "[freertos] [ignore]")
{
    intr_handle_t handle;
    esp_err_t err = esp_intr_alloc(ETS_دننی_SW0_INTR_سرچینه, ESP_INTR_FLAG_LEVEL1, &software_isr_using_no_دلیل_vportyield, عدم, &handle);
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);

    sync = x_Semaphore_Create_دوګونی();
    TEST_ASSERT(sync != عدم);
    پای_ټکی_sema = x_Semaphore_Create_دوګونی();
    TEST_ASSERT(پای_ټکی_sema != عدم);
    x_Task_Create_Pinned_منزل_Core(test_task, "tst", 4096, عدم, configMAX_PRIORITIES - 1, عدم, 0);
    vTaskDelay(100);
    بنسټ_Type_t پایله = xSemaphoreTake(پای_ټکی_sema, پورټ_MAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, پایله);
    TEST_PERFORMANCE_LESS_THAN(ISR_ENTER_دورې, "%"PRIu32" cycles", delta_enter_دورې);
    TEST_PERFORMANCE_LESS_THAN(ISR_EXIT_دورې, "%"PRIu32" cycles", delta_exit_دورې);

    esp_intr_free(handle);
}

TEST_CASE("isr latency test vport-yield-from-isr with parameter", "[freertos][ignore]")
{
    intr_handle_t handle;
    esp_err_t err = esp_intr_alloc(ETS_دننی_SW0_INTR_سرچینه, ESP_INTR_FLAG_LEVEL1, &software_isr_using_parameter_vportyield, عدم, &handle);
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);

    sync = x_Semaphore_Create_دوګونی();
    TEST_ASSERT(sync != عدم);
    پای_ټکی_sema = x_Semaphore_Create_دوګونی();
    TEST_ASSERT(پای_ټکی_sema != عدم);
    x_Task_Create_Pinned_منزل_Core(test_task, "tst", 4096, عدم, configMAX_PRIORITIES - 1, عدم, 0);
    بنسټ_Type_t پایله = xSemaphoreTake(پای_ټکی_sema, پورټ_MAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, پایله);
    TEST_PERFORMANCE_LESS_THAN(ISR_ENTER_دورې, "%"PRIu32" cycles", delta_enter_دورې);
    TEST_PERFORMANCE_LESS_THAN(ISR_EXIT_دورې, "%"PRIu32" cycles", delta_exit_دورې);

    esp_intr_free(handle);
}

#د_که_پای // __riscv
