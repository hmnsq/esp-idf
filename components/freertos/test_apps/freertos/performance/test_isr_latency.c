/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ <esp_types.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <inttypes.h>
#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ "freertos/semphr.h"
#ಅನ್ತರ್ಭಾವ "freertos/queue.h"
#ಅನ್ತರ್ಭಾವ "esp_intr_alloc.h"
#ಅನ್ತರ್ಭಾವ "esp_cpu.h"
#ಅನ್ತರ್ಭಾವ "unity.h"
#ಅನ್ತರ್ಭಾವ "test_utils.h"
#ಅನ್ತರ್ಭಾವ "freertos_performance.h"

#ಯದಿ CONFIG_IDF_TARGET_ARCH_XTENSA
#ಅನ್ತರ್ಭಾವ "xtensa/hal.h"
#ಅನ್ತರ್ಭಾವ "xtensa_api.h"     // Replace with interrupt allocator API (IDF-3891)
#define TEST_SET_INT_MASK(mask) xt_set_intset(mask)
#define TEST_CLR_INT_MASK(mask) xt_set_intclear(mask)
#ನೋಚೇದ್ಯದಿ CONFIG_IDF_TARGET_ARCH_RISCV
#ಅನ್ತರ್ಭಾವ "riscv/interrupt.h"
#define TEST_SET_INT_MASK(mask) esprv_int_enable(mask)
#define TEST_CLR_INT_MASK(mask) esprv_int_disable(mask)
#endif

#ifndef __riscv // TODO: IDF-4416

#define SW_ISR_LEVEL_1  7

ಸ್ಥಿರ SemaphoreHandle_t sync;
ಸ್ಥಿರ SemaphoreHandle_t end_sema;
ಸ್ಥಿರ uint32_t cycle_before_trigger;
ಸ್ಥಿರ uint32_t cycle_before_exit;
ಸ್ಥಿರ uint32_t delta_enter_cycles = 0;
ಸ್ಥಿರ uint32_t delta_exit_cycles = 0;

ಸ್ಥಿರ ಶೂನ್ಯ software_isr_using_parameter_vportyield(ಶೂನ್ಯ *arg)
{
    (ಶೂನ್ಯ)arg;
    BaseType_t yield;
    delta_enter_cycles += esp_cpu_get_cycle_ಎಣಿಕೆ() - cycle_before_trigger;
    TEST_CLR_INT_MASK(1 << SW_ISR_LEVEL_1);

    xSemaphoreGiveFromISR(sync, &yield);
    portYIELD_FROM_ISR(yield);

    cycle_before_exit = esp_cpu_get_cycle_ಎಣಿಕೆ();
}

ಸ್ಥಿರ ಶೂನ್ಯ software_isr_using_no_argument_vportyield(ಶೂನ್ಯ *arg)
{
    (ಶೂನ್ಯ)arg;
    BaseType_t yield;
    delta_enter_cycles += esp_cpu_get_cycle_ಎಣಿಕೆ() - cycle_before_trigger;

    TEST_CLR_INT_MASK(1 << SW_ISR_LEVEL_1);

    xSemaphoreGiveFromISR(sync, &yield);
    ಯದಿ (yield) {
        portYIELD_FROM_ISR();
    }
    cycle_before_exit = esp_cpu_get_cycle_ಎಣಿಕೆ();
}

ಸ್ಥಿರ ಶೂನ್ಯ test_task(ಶೂನ್ಯ *arg)
{
    (ಶೂನ್ಯ) arg;

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < 10000; i++) {
        cycle_before_trigger = esp_cpu_get_cycle_ಎಣಿಕೆ();
        TEST_SET_INT_MASK(1 << SW_ISR_LEVEL_1);
        xSemaphoreTake(sync, portMAX_DELAY);
        delta_exit_cycles += esp_cpu_get_cycle_ಎಣಿಕೆ() - cycle_before_exit;
    }

    delta_enter_cycles /= 10000;
    delta_exit_cycles /= 10000;

    xSemaphoreGive(end_sema);
    vTaskDelete(ರಿಕ್ತ);
}

TEST_CASE("isr latency test vport-yield-from-isr with no parameter", "[freertos] [ignore]")
{
    intr_handle_t handle;
    esp_err_t err = esp_intr_alloc(ETS_INTERNAL_SW0_INTR_SOURCE, ESP_INTR_FLAG_LEVEL1, &software_isr_using_no_argument_vportyield, ರಿಕ್ತ, &handle);
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);

    sync = x_Semaphore_Create_ದ್ವಿಮಾನ();
    TEST_ASSERT(sync != ರಿಕ್ತ);
    end_sema = x_Semaphore_Create_ದ್ವಿಮಾನ();
    TEST_ASSERT(end_sema != ರಿಕ್ತ);
    xTaskCreatePinnedToCore(test_task, "tst", 4096, ರಿಕ್ತ, configMAX_PRIORITIES - 1, ರಿಕ್ತ, 0);
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
    esp_err_t err = esp_intr_alloc(ETS_INTERNAL_SW0_INTR_SOURCE, ESP_INTR_FLAG_LEVEL1, &software_isr_using_parameter_vportyield, ರಿಕ್ತ, &handle);
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);

    sync = x_Semaphore_Create_ದ್ವಿಮಾನ();
    TEST_ASSERT(sync != ರಿಕ್ತ);
    end_sema = x_Semaphore_Create_ದ್ವಿಮಾನ();
    TEST_ASSERT(end_sema != ರಿಕ್ತ);
    xTaskCreatePinnedToCore(test_task, "tst", 4096, ರಿಕ್ತ, configMAX_PRIORITIES - 1, ರಿಕ್ತ, 0);
    BaseType_t result = xSemaphoreTake(end_sema, portMAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, result);
    TEST_PERFORMANCE_LESS_THAN(ISR_ENTER_CYCLES, "%"PRIu32" cycles", delta_enter_cycles);
    TEST_PERFORMANCE_LESS_THAN(ISR_EXIT_CYCLES, "%"PRIu32" cycles", delta_exit_cycles);

    esp_intr_free(handle);
}

#endif // __riscv
