/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਅਨ੍ਤਰ੍ਭਾਵ <esp_types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <inttypes.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/semphr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/queue.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_intr_alloc.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_cpu.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unity.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "test_utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos_performance.h"

#ਜੇ CONFIG_IDF_ਨਿਸ਼ਾਨਾ_ARCH_XTENSA
#ਅਨ੍ਤਰ੍ਭਾਵ "xtensa/hal.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xtensa_api.h"     // Replace with interrupt allocator API (IDF-3891)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TEST_ਸੈੱਟ_INT_ਨਕਾਬ(ਨਕਾਬ) xt_ਸੈੱਟ_intset(ਨਕਾਬ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TEST_CLR_INT_ਨਕਾਬ(ਨਕਾਬ) xt_ਸੈੱਟ_intclear(ਨਕਾਬ)
#ਜਾਂਜੇ CONFIG_IDF_ਨਿਸ਼ਾਨਾ_ARCH_RISCV
#ਅਨ੍ਤਰ੍ਭਾਵ "riscv/interrupt.h"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TEST_ਸੈੱਟ_INT_ਨਕਾਬ(ਨਕਾਬ) esprv_int_enable(ਨਕਾਬ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TEST_CLR_INT_ਨਕਾਬ(ਨਕਾਬ) esprv_int_disable(ਨਕਾਬ)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ __riscv // TODO: IDF-4416

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SW_ISR_LEVEL_1  7

ਸਥਿਰ SemaphoreHandle_t sync;
ਸਥਿਰ SemaphoreHandle_t ਸਿਰਾ_sema;
ਸਥਿਰ uint32_t ਚੱਕਰ_before_trigger;
ਸਥਿਰ uint32_t ਚੱਕਰ_before_exit;
ਸਥਿਰ uint32_t delta_enter_ਚੱਕਰਾਂ = 0;
ਸਥਿਰ uint32_t delta_exit_ਚੱਕਰਾਂ = 0;

ਸਥਿਰ ਖਾਲੀ software_isr_using_parameter_vportyield(ਖਾਲੀ *arg)
{
    (ਖਾਲੀ)arg;
    ਅਧਾਰ_Type_t yield;
    delta_enter_ਚੱਕਰਾਂ += esp_cpu_get_ਚੱਕਰ_ਗਿਣਤੀ() - ਚੱਕਰ_before_trigger;
    TEST_CLR_INT_ਨਕਾਬ(1 << SW_ISR_LEVEL_1);

    xSemaphoreGiveFromISR(sync, &yield);
    ਪੋਰਟ_YIELD_FROM_ISR(yield);

    ਚੱਕਰ_before_exit = esp_cpu_get_ਚੱਕਰ_ਗਿਣਤੀ();
}

ਸਥਿਰ ਖਾਲੀ software_isr_using_no_ਦਲੀਲ_vportyield(ਖਾਲੀ *arg)
{
    (ਖਾਲੀ)arg;
    ਅਧਾਰ_Type_t yield;
    delta_enter_ਚੱਕਰਾਂ += esp_cpu_get_ਚੱਕਰ_ਗਿਣਤੀ() - ਚੱਕਰ_before_trigger;

    TEST_CLR_INT_ਨਕਾਬ(1 << SW_ISR_LEVEL_1);

    xSemaphoreGiveFromISR(sync, &yield);
    ਜੇ (yield) {
        ਪੋਰਟ_YIELD_FROM_ISR();
    }
    ਚੱਕਰ_before_exit = esp_cpu_get_ਚੱਕਰ_ਗਿਣਤੀ();
}

ਸਥਿਰ ਖਾਲੀ test_task(ਖਾਲੀ *arg)
{
    (ਖਾਲੀ) arg;

    ਲਈ (ਪੂਰਨ i = 0; i < 10000; i++) {
        ਚੱਕਰ_before_trigger = esp_cpu_get_ਚੱਕਰ_ਗਿਣਤੀ();
        TEST_ਸੈੱਟ_INT_ਨਕਾਬ(1 << SW_ISR_LEVEL_1);
        xSemaphoreTake(sync, ਪੋਰਟ_MAX_DELAY);
        delta_exit_ਚੱਕਰਾਂ += esp_cpu_get_ਚੱਕਰ_ਗਿਣਤੀ() - ਚੱਕਰ_before_exit;
    }

    delta_enter_ਚੱਕਰਾਂ /= 10000;
    delta_exit_ਚੱਕਰਾਂ /= 10000;

    xSemaphoreGive(ਸਿਰਾ_sema);
    vTaskDelete(ਨਿਰਾ);
}

TEST_CASE("isr latency test vport-yield-from-isr with no parameter", "[freertos] [ignore]")
{
    intr_handle_t handle;
    esp_err_t err = esp_intr_alloc(ETS_ਅੰਦਰੂਨੀ_SW0_INTR_ਸਰੋਤ, ESP_INTR_FLAG_LEVEL1, &software_isr_using_no_ਦਲੀਲ_vportyield, ਨਿਰਾ, &handle);
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);

    sync = x_Semaphore_Create_ਦੁਆਧਾਰੀ();
    TEST_ASSERT(sync != ਨਿਰਾ);
    ਸਿਰਾ_sema = x_Semaphore_Create_ਦੁਆਧਾਰੀ();
    TEST_ASSERT(ਸਿਰਾ_sema != ਨਿਰਾ);
    x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(test_task, "tst", 4096, ਨਿਰਾ, configMAX_PRIORITIES - 1, ਨਿਰਾ, 0);
    vTaskDelay(100);
    ਅਧਾਰ_Type_t ਨਤੀਜਾ = xSemaphoreTake(ਸਿਰਾ_sema, ਪੋਰਟ_MAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, ਨਤੀਜਾ);
    TEST_PERFORMANCE_LESS_THAN(ISR_ENTER_ਚੱਕਰਾਂ, "%"PRIu32" cycles", delta_enter_ਚੱਕਰਾਂ);
    TEST_PERFORMANCE_LESS_THAN(ISR_EXIT_ਚੱਕਰਾਂ, "%"PRIu32" cycles", delta_exit_ਚੱਕਰਾਂ);

    esp_intr_free(handle);
}

TEST_CASE("isr latency test vport-yield-from-isr with parameter", "[freertos][ignore]")
{
    intr_handle_t handle;
    esp_err_t err = esp_intr_alloc(ETS_ਅੰਦਰੂਨੀ_SW0_INTR_ਸਰੋਤ, ESP_INTR_FLAG_LEVEL1, &software_isr_using_parameter_vportyield, ਨਿਰਾ, &handle);
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);

    sync = x_Semaphore_Create_ਦੁਆਧਾਰੀ();
    TEST_ASSERT(sync != ਨਿਰਾ);
    ਸਿਰਾ_sema = x_Semaphore_Create_ਦੁਆਧਾਰੀ();
    TEST_ASSERT(ਸਿਰਾ_sema != ਨਿਰਾ);
    x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(test_task, "tst", 4096, ਨਿਰਾ, configMAX_PRIORITIES - 1, ਨਿਰਾ, 0);
    ਅਧਾਰ_Type_t ਨਤੀਜਾ = xSemaphoreTake(ਸਿਰਾ_sema, ਪੋਰਟ_MAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, ਨਤੀਜਾ);
    TEST_PERFORMANCE_LESS_THAN(ISR_ENTER_ਚੱਕਰਾਂ, "%"PRIu32" cycles", delta_enter_ਚੱਕਰਾਂ);
    TEST_PERFORMANCE_LESS_THAN(ISR_EXIT_ਚੱਕਰਾਂ, "%"PRIu32" cycles", delta_exit_ਚੱਕਰਾਂ);

    esp_intr_free(handle);
}

#ਜੇ_ਅੰਤ // __riscv
