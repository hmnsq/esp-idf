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
#وصف_ڪريو TEST_سيٽ_INT_نقاب(نقاب) xt_سيٽ_intset(نقاب)
#وصف_ڪريو TEST_CLR_INT_نقاب(نقاب) xt_سيٽ_intclear(نقاب)
#وإلاإذا CONFIG_IDF_هدف_ARCH_RISCV
#ضمن "riscv/interrupt.h"
#وصف_ڪريو TEST_سيٽ_INT_نقاب(نقاب) esprv_int_enable(نقاب)
#وصف_ڪريو TEST_CLR_INT_نقاب(نقاب) esprv_int_disable(نقاب)
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل __riscv // TODO: IDF-4416

#وصف_ڪريو SW_ISR_LEVEL_1  7

ساكن SemaphoreHandle_t sync;
ساكن SemaphoreHandle_t پڇاڙي_sema;
ساكن uint32_t چڪر_before_trigger;
ساكن uint32_t چڪر_before_exit;
ساكن uint32_t delta_enter_چڪرن = 0;
ساكن uint32_t delta_exit_چڪرن = 0;

ساكن فراغ software_isr_using_parameter_vportyield(فراغ *arg)
{
    (فراغ)arg;
    بنياد_Type_t yield;
    delta_enter_چڪرن += esp_cpu_get_چڪر_ڳڻپ() - چڪر_before_trigger;
    TEST_CLR_INT_نقاب(1 << SW_ISR_LEVEL_1);

    xSemaphoreGiveFromISR(sync, &yield);
    پورٽ_YIELD_FROM_ISR(yield);

    چڪر_before_exit = esp_cpu_get_چڪر_ڳڻپ();
}

ساكن فراغ software_isr_using_no_دليل_vportyield(فراغ *arg)
{
    (فراغ)arg;
    بنياد_Type_t yield;
    delta_enter_چڪرن += esp_cpu_get_چڪر_ڳڻپ() - چڪر_before_trigger;

    TEST_CLR_INT_نقاب(1 << SW_ISR_LEVEL_1);

    xSemaphoreGiveFromISR(sync, &yield);
    إذا (yield) {
        پورٽ_YIELD_FROM_ISR();
    }
    چڪر_before_exit = esp_cpu_get_چڪر_ڳڻپ();
}

ساكن فراغ test_task(فراغ *arg)
{
    (فراغ) arg;

    لكل (صحيح i = 0; i < 10000; i++) {
        چڪر_before_trigger = esp_cpu_get_چڪر_ڳڻپ();
        TEST_سيٽ_INT_نقاب(1 << SW_ISR_LEVEL_1);
        xSemaphoreTake(sync, پورٽ_MAX_DELAY);
        delta_exit_چڪرن += esp_cpu_get_چڪر_ڳڻپ() - چڪر_before_exit;
    }

    delta_enter_چڪرن /= 10000;
    delta_exit_چڪرن /= 10000;

    xSemaphoreGive(پڇاڙي_sema);
    vTaskDelete(عدم);
}

TEST_CASE("isr latency test vport-yield-from-isr with no parameter", "[freertos] [ignore]")
{
    intr_handle_t handle;
    esp_err_t err = esp_intr_alloc(ETS_اندروني_SW0_INTR_ذريعو, ESP_INTR_FLAG_LEVEL1, &software_isr_using_no_دليل_vportyield, عدم, &handle);
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);

    sync = x_Semaphore_Create_ٻٽو();
    TEST_ASSERT(sync != عدم);
    پڇاڙي_sema = x_Semaphore_Create_ٻٽو();
    TEST_ASSERT(پڇاڙي_sema != عدم);
    x_Task_Create_Pinned_منزل_Core(test_task, "tst", 4096, عدم, configMAX_PRIORITIES - 1, عدم, 0);
    vTaskDelay(100);
    بنياد_Type_t نتيجو = xSemaphoreTake(پڇاڙي_sema, پورٽ_MAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, نتيجو);
    TEST_PERFORMANCE_LESS_THAN(ISR_ENTER_چڪرن, "%"PRIu32" cycles", delta_enter_چڪرن);
    TEST_PERFORMANCE_LESS_THAN(ISR_EXIT_چڪرن, "%"PRIu32" cycles", delta_exit_چڪرن);

    esp_intr_free(handle);
}

TEST_CASE("isr latency test vport-yield-from-isr with parameter", "[freertos][ignore]")
{
    intr_handle_t handle;
    esp_err_t err = esp_intr_alloc(ETS_اندروني_SW0_INTR_ذريعو, ESP_INTR_FLAG_LEVEL1, &software_isr_using_parameter_vportyield, عدم, &handle);
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);

    sync = x_Semaphore_Create_ٻٽو();
    TEST_ASSERT(sync != عدم);
    پڇاڙي_sema = x_Semaphore_Create_ٻٽو();
    TEST_ASSERT(پڇاڙي_sema != عدم);
    x_Task_Create_Pinned_منزل_Core(test_task, "tst", 4096, عدم, configMAX_PRIORITIES - 1, عدم, 0);
    بنياد_Type_t نتيجو = xSemaphoreTake(پڇاڙي_sema, پورٽ_MAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, نتيجو);
    TEST_PERFORMANCE_LESS_THAN(ISR_ENTER_چڪرن, "%"PRIu32" cycles", delta_enter_چڪرن);
    TEST_PERFORMANCE_LESS_THAN(ISR_EXIT_چڪرن, "%"PRIu32" cycles", delta_exit_چڪرن);

    esp_intr_free(handle);
}

#جيڪڏهن_پورو // __riscv
