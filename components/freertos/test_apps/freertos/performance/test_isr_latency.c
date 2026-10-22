/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#समावेश <esp_types.h>
#समावेश <stdio.h>
#समावेश <inttypes.h>
#समावेश "sdkconfig.h"
#समावेश "freertos/FreeRTOS.h"
#समावेश "freertos/task.h"
#समावेश "freertos/semphr.h"
#समावेश "freertos/queue.h"
#समावेश "esp_intr_alloc.h"
#समावेश "esp_cpu.h"
#समावेश "unity.h"
#समावेश "test_utils.h"
#समावेश "freertos_performance.h"

#अगर CONFIG_IDF_TARGET_ARCH_XTENSA
#समावेश "xtensa/hal.h"
#समावेश "xtensa_api.h"     // Replace with interrupt allocator API (IDF-3891)
#घोषणा TEST_SET_INT_MASK(mask) xt_set_intset(mask)
#घोषणा TEST_CLR_INT_MASK(mask) xt_set_intclear(mask)
#या_अगर CONFIG_IDF_TARGET_ARCH_RISCV
#समावेश "riscv/interrupt.h"
#घोषणा TEST_SET_INT_MASK(mask) esprv_int_enable(mask)
#घोषणा TEST_CLR_INT_MASK(mask) esprv_int_disable(mask)
#पूर्ण_अगर

#अगर_अघोषित __riscv // TODO: IDF-4416

#घोषणा SW_ISR_LEVEL_1  7

अटल SemaphoreHandle_t sync;
अटल SemaphoreHandle_t end_sema;
अटल uint32_t cycle_before_trigger;
अटल uint32_t cycle_before_exit;
अटल uint32_t delta_enter_cycles = 0;
अटल uint32_t delta_exit_cycles = 0;

अटल व्योम software_isr_using_parameter_vportyield(व्योम *arg)
{
    (व्योम)arg;
    BaseType_t yield;
    delta_enter_cycles += esp_cpu_get_cycle_गिनती() - cycle_before_trigger;
    TEST_CLR_INT_MASK(1 << SW_ISR_LEVEL_1);

    xSemaphoreGiveFromISR(sync, &yield);
    portYIELD_FROM_ISR(yield);

    cycle_before_exit = esp_cpu_get_cycle_गिनती();
}

अटल व्योम software_isr_using_no_argument_vportyield(व्योम *arg)
{
    (व्योम)arg;
    BaseType_t yield;
    delta_enter_cycles += esp_cpu_get_cycle_गिनती() - cycle_before_trigger;

    TEST_CLR_INT_MASK(1 << SW_ISR_LEVEL_1);

    xSemaphoreGiveFromISR(sync, &yield);
    अगर (yield) {
        portYIELD_FROM_ISR();
    }
    cycle_before_exit = esp_cpu_get_cycle_गिनती();
}

अटल व्योम test_task(व्योम *arg)
{
    (व्योम) arg;

    क्रम (पूर्णांक i = 0; i < 10000; i++) {
        cycle_before_trigger = esp_cpu_get_cycle_गिनती();
        TEST_SET_INT_MASK(1 << SW_ISR_LEVEL_1);
        xSemaphoreTake(sync, portMAX_DELAY);
        delta_exit_cycles += esp_cpu_get_cycle_गिनती() - cycle_before_exit;
    }

    delta_enter_cycles /= 10000;
    delta_exit_cycles /= 10000;

    xSemaphoreGive(end_sema);
    vTaskDelete(शून्य);
}

TEST_CASE("isr latency test vport-yield-from-isr with no parameter", "[freertos] [ignore]")
{
    intr_handle_t handle;
    esp_err_t err = esp_intr_alloc(ETS_INTERNAL_SW0_INTR_SOURCE, ESP_INTR_FLAG_LEVEL1, &software_isr_using_no_argument_vportyield, शून्य, &handle);
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);

    sync = x_Semaphore_Create_द्विआधारी();
    TEST_ASSERT(sync != शून्य);
    end_sema = x_Semaphore_Create_द्विआधारी();
    TEST_ASSERT(end_sema != शून्य);
    xTaskCreatePinnedToCore(test_task, "tst", 4096, शून्य, configMAX_PRIORITIES - 1, शून्य, 0);
    vTaskDelay(100);
    BaseType_t परिणाम = xSemaphoreTake(end_sema, portMAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, परिणाम);
    TEST_PERFORMANCE_LESS_THAN(ISR_ENTER_CYCLES, "%"PRIu32" cycles", delta_enter_cycles);
    TEST_PERFORMANCE_LESS_THAN(ISR_EXIT_CYCLES, "%"PRIu32" cycles", delta_exit_cycles);

    esp_intr_free(handle);
}

TEST_CASE("isr latency test vport-yield-from-isr with parameter", "[freertos][ignore]")
{
    intr_handle_t handle;
    esp_err_t err = esp_intr_alloc(ETS_INTERNAL_SW0_INTR_SOURCE, ESP_INTR_FLAG_LEVEL1, &software_isr_using_parameter_vportyield, शून्य, &handle);
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);

    sync = x_Semaphore_Create_द्विआधारी();
    TEST_ASSERT(sync != शून्य);
    end_sema = x_Semaphore_Create_द्विआधारी();
    TEST_ASSERT(end_sema != शून्य);
    xTaskCreatePinnedToCore(test_task, "tst", 4096, शून्य, configMAX_PRIORITIES - 1, शून्य, 0);
    BaseType_t परिणाम = xSemaphoreTake(end_sema, portMAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, परिणाम);
    TEST_PERFORMANCE_LESS_THAN(ISR_ENTER_CYCLES, "%"PRIu32" cycles", delta_enter_cycles);
    TEST_PERFORMANCE_LESS_THAN(ISR_EXIT_CYCLES, "%"PRIu32" cycles", delta_exit_cycles);

    esp_intr_free(handle);
}

#पूर्ण_अगर // __riscv
