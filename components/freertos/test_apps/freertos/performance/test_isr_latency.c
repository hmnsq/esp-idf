/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#अन्तर्भाव <esp_types.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <inttypes.h>
#अन्तर्भाव "sdkconfig.h"
#अन्तर्भाव "freertos/FreeRTOS.h"
#अन्तर्भाव "freertos/task.h"
#अन्तर्भाव "freertos/semphr.h"
#अन्तर्भाव "freertos/queue.h"
#अन्तर्भाव "esp_intr_alloc.h"
#अन्तर्भाव "esp_cpu.h"
#अन्तर्भाव "unity.h"
#अन्तर्भाव "test_utils.h"
#अन्तर्भाव "freertos_performance.h"

#यदि CONFIG_IDF_TARGET_ARCH_XTENSA
#अन्तर्भाव "xtensa/hal.h"
#अन्तर्भाव "xtensa_api.h"     // Replace with interrupt allocator API (IDF-3891)
#परिभाषय TEST_समुच्चयः_INT_आवरणम्(आवरणम्) xt_समुच्चयः_intset(आवरणम्)
#परिभाषय TEST_CLR_INT_आवरणम्(आवरणम्) xt_समुच्चयः_intclear(आवरणम्)
#नोचेद्यदि CONFIG_IDF_TARGET_ARCH_RISCV
#अन्तर्भाव "riscv/interrupt.h"
#परिभाषय TEST_समुच्चयः_INT_आवरणम्(आवरणम्) esprv_int_enable(आवरणम्)
#परिभाषय TEST_CLR_INT_आवरणम्(आवरणम्) esprv_int_disable(आवरणम्)
#यदि_अन्तः

#यदि_अपरिभाषितम् __riscv // TODO: IDF-4416

#परिभाषय SW_ISR_LEVEL_1  7

स्थिर SemaphoreHandle_t sync;
स्थिर SemaphoreHandle_t अन्तः_sema;
स्थिर uint32_t आवृत्तिः_before_trigger;
स्थिर uint32_t आवृत्तिः_before_exit;
स्थिर uint32_t delta_enter_आवृत्तयः = 0;
स्थिर uint32_t delta_exit_आवृत्तयः = 0;

स्थिर शून्य software_isr_using_parameter_vportyield(शून्य *arg)
{
    (शून्य)arg;
    आधारः_Type_t yield;
    delta_enter_आवृत्तयः += esp_cpu_get_आवृत्तिः_गणनम्() - आवृत्तिः_before_trigger;
    TEST_CLR_INT_आवरणम्(1 << SW_ISR_LEVEL_1);

    xSemaphoreGiveFromISR(sync, &yield);
    पत्तनम्_YIELD_FROM_ISR(yield);

    आवृत्तिः_before_exit = esp_cpu_get_आवृत्तिः_गणनम्();
}

स्थिर शून्य software_isr_using_no_तर्कः_vportyield(शून्य *arg)
{
    (शून्य)arg;
    आधारः_Type_t yield;
    delta_enter_आवृत्तयः += esp_cpu_get_आवृत्तिः_गणनम्() - आवृत्तिः_before_trigger;

    TEST_CLR_INT_आवरणम्(1 << SW_ISR_LEVEL_1);

    xSemaphoreGiveFromISR(sync, &yield);
    यदि (yield) {
        पत्तनम्_YIELD_FROM_ISR();
    }
    आवृत्तिः_before_exit = esp_cpu_get_आवृत्तिः_गणनम्();
}

स्थिर शून्य test_task(शून्य *arg)
{
    (शून्य) arg;

    प्रति (पूर्णाङ्क i = 0; i < 10000; i++) {
        आवृत्तिः_before_trigger = esp_cpu_get_आवृत्तिः_गणनम्();
        TEST_समुच्चयः_INT_आवरणम्(1 << SW_ISR_LEVEL_1);
        xSemaphoreTake(sync, पत्तनम्_MAX_DELAY);
        delta_exit_आवृत्तयः += esp_cpu_get_आवृत्तिः_गणनम्() - आवृत्तिः_before_exit;
    }

    delta_enter_आवृत्तयः /= 10000;
    delta_exit_आवृत्तयः /= 10000;

    xSemaphoreGive(अन्तः_sema);
    vTaskDelete(रिक्त);
}

TEST_CASE("isr latency test vport-yield-from-isr with no parameter", "[freertos] [ignore]")
{
    intr_handle_t handle;
    esp_err_t err = esp_intr_alloc(ETS_आन्तरिकम्_SW0_INTR_SOURCE, ESP_INTR_FLAG_LEVEL1, &software_isr_using_no_तर्कः_vportyield, रिक्त, &handle);
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);

    sync = x_Semaphore_Create_द्विमान();
    TEST_ASSERT(sync != रिक्त);
    अन्तः_sema = x_Semaphore_Create_द्विमान();
    TEST_ASSERT(अन्तः_sema != रिक्त);
    x_Task_Create_Pinned_गन्तव्यम्_Core(test_task, "tst", 4096, रिक्त, configMAX_PRIORITIES - 1, रिक्त, 0);
    vTaskDelay(100);
    आधारः_Type_t परिणाम = xSemaphoreTake(अन्तः_sema, पत्तनम्_MAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, परिणाम);
    TEST_PERFORMANCE_LESS_THAN(ISR_ENTER_आवृत्तयः, "%"PRIu32" cycles", delta_enter_आवृत्तयः);
    TEST_PERFORMANCE_LESS_THAN(ISR_EXIT_आवृत्तयः, "%"PRIu32" cycles", delta_exit_आवृत्तयः);

    esp_intr_free(handle);
}

TEST_CASE("isr latency test vport-yield-from-isr with parameter", "[freertos][ignore]")
{
    intr_handle_t handle;
    esp_err_t err = esp_intr_alloc(ETS_आन्तरिकम्_SW0_INTR_SOURCE, ESP_INTR_FLAG_LEVEL1, &software_isr_using_parameter_vportyield, रिक्त, &handle);
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);

    sync = x_Semaphore_Create_द्विमान();
    TEST_ASSERT(sync != रिक्त);
    अन्तः_sema = x_Semaphore_Create_द्विमान();
    TEST_ASSERT(अन्तः_sema != रिक्त);
    x_Task_Create_Pinned_गन्तव्यम्_Core(test_task, "tst", 4096, रिक्त, configMAX_PRIORITIES - 1, रिक्त, 0);
    आधारः_Type_t परिणाम = xSemaphoreTake(अन्तः_sema, पत्तनम्_MAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, परिणाम);
    TEST_PERFORMANCE_LESS_THAN(ISR_ENTER_आवृत्तयः, "%"PRIu32" cycles", delta_enter_आवृत्तयः);
    TEST_PERFORMANCE_LESS_THAN(ISR_EXIT_आवृत्तयः, "%"PRIu32" cycles", delta_exit_आवृत्तयः);

    esp_intr_free(handle);
}

#यदि_अन्तः // __riscv
