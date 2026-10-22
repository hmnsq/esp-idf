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

#सचे CONFIG_IDF_TARGET_ARCH_XTENSA
#अन्तर्भाव "xtensa/hal.h"
#अन्तर्भाव "xtensa_api.h"     // Replace with interrupt allocator API (IDF-3891)
#define TEST_SET_INT_MASK(mask) xt_set_intset(mask)
#define TEST_CLR_INT_MASK(mask) xt_set_intclear(mask)
#नोचेद्यदि CONFIG_IDF_TARGET_ARCH_RISCV
#अन्तर्भाव "riscv/interrupt.h"
#define TEST_SET_INT_MASK(mask) esprv_int_enable(mask)
#define TEST_CLR_INT_MASK(mask) esprv_int_disable(mask)
#endif

#ifndef __riscv // TODO: IDF-4416

#define SW_ISR_LEVEL_1  7

स्थिर SemaphoreHandle_t sync;
स्थिर SemaphoreHandle_t end_sema;
स्थिर uint32_t cycle_before_trigger;
स्थिर uint32_t cycle_before_exit;
स्थिर uint32_t delta_enter_cycles = 0;
स्थिर uint32_t delta_exit_cycles = 0;

स्थिर शून्य software_isr_using_parameter_vportyield(शून्य *arg)
{
    (शून्य)arg;
    BaseType_t yield;
    delta_enter_cycles += esp_cpu_get_cycle_गणनम्() - cycle_before_trigger;
    TEST_CLR_INT_MASK(1 << SW_ISR_LEVEL_1);

    xSemaphoreGiveFromISR(sync, &yield);
    portYIELD_FROM_ISR(yield);

    cycle_before_exit = esp_cpu_get_cycle_गणनम्();
}

स्थिर शून्य software_isr_using_no_argument_vportyield(शून्य *arg)
{
    (शून्य)arg;
    BaseType_t yield;
    delta_enter_cycles += esp_cpu_get_cycle_गणनम्() - cycle_before_trigger;

    TEST_CLR_INT_MASK(1 << SW_ISR_LEVEL_1);

    xSemaphoreGiveFromISR(sync, &yield);
    सचे (yield) {
        portYIELD_FROM_ISR();
    }
    cycle_before_exit = esp_cpu_get_cycle_गणनम्();
}

स्थिर शून्य test_task(शून्य *arg)
{
    (शून्य) arg;

    अत्थाय (पूर्णाङ्क i = 0; i < 10000; i++) {
        cycle_before_trigger = esp_cpu_get_cycle_गणनम्();
        TEST_SET_INT_MASK(1 << SW_ISR_LEVEL_1);
        xSemaphoreTake(sync, portMAX_DELAY);
        delta_exit_cycles += esp_cpu_get_cycle_गणनम्() - cycle_before_exit;
    }

    delta_enter_cycles /= 10000;
    delta_exit_cycles /= 10000;

    xSemaphoreGive(end_sema);
    vTaskDelete(रिक्त);
}

TEST_CASE("isr latency test vport-yield-from-isr with no parameter", "[freertos] [ignore]")
{
    intr_handle_t handle;
    esp_err_t err = esp_intr_alloc(ETS_INTERNAL_SW0_INTR_SOURCE, ESP_INTR_FLAG_LEVEL1, &software_isr_using_no_argument_vportyield, रिक्त, &handle);
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);

    sync = x_Semaphore_Create_द्विमान();
    TEST_ASSERT(sync != रिक्त);
    end_sema = x_Semaphore_Create_द्विमान();
    TEST_ASSERT(end_sema != रिक्त);
    xTaskCreatePinnedToCore(test_task, "tst", 4096, रिक्त, configMAX_PRIORITIES - 1, रिक्त, 0);
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
    esp_err_t err = esp_intr_alloc(ETS_INTERNAL_SW0_INTR_SOURCE, ESP_INTR_FLAG_LEVEL1, &software_isr_using_parameter_vportyield, रिक्त, &handle);
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);

    sync = x_Semaphore_Create_द्विमान();
    TEST_ASSERT(sync != रिक्त);
    end_sema = x_Semaphore_Create_द्विमान();
    TEST_ASSERT(end_sema != रिक्त);
    xTaskCreatePinnedToCore(test_task, "tst", 4096, रिक्त, configMAX_PRIORITIES - 1, रिक्त, 0);
    BaseType_t result = xSemaphoreTake(end_sema, portMAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, result);
    TEST_PERFORMANCE_LESS_THAN(ISR_ENTER_CYCLES, "%"PRIu32" cycles", delta_enter_cycles);
    TEST_PERFORMANCE_LESS_THAN(ISR_EXIT_CYCLES, "%"PRIu32" cycles", delta_exit_cycles);

    esp_intr_free(handle);
}

#endif // __riscv
