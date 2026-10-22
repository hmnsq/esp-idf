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

#ಆದರೆ CONFIG_IDF_ಗುರಿ_ARCH_XTENSA
#ಅನ್ತರ್ಭಾವ "xtensa/hal.h"
#ಅನ್ತರ್ಭಾವ "xtensa_api.h"     // Replace with interrupt allocator API (IDF-3891)
#ವ್ಯಾಖ್ಯಾನಿಸು TEST_ಗಣ_INT_ಮುಖವಾಡ(ಮುಖವಾಡ) xt_ಗಣ_intset(ಮುಖವಾಡ)
#ವ್ಯಾಖ್ಯಾನಿಸು TEST_CLR_INT_ಮುಖವಾಡ(ಮುಖವಾಡ) xt_ಗಣ_intclear(ಮುಖವಾಡ)
#ಅಥವಾಆದರೆ CONFIG_IDF_ಗುರಿ_ARCH_RISCV
#ಅನ್ತರ್ಭಾವ "riscv/interrupt.h"
#ವ್ಯಾಖ್ಯಾನಿಸು TEST_ಗಣ_INT_ಮುಖವಾಡ(ಮುಖವಾಡ) esprv_int_enable(ಮುಖವಾಡ)
#ವ್ಯಾಖ್ಯಾನಿಸು TEST_CLR_INT_ಮುಖವಾಡ(ಮುಖವಾಡ) esprv_int_disable(ಮುಖವಾಡ)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ __riscv // TODO: IDF-4416

#ವ್ಯಾಖ್ಯಾನಿಸು SW_ISR_LEVEL_1  7

ಸ್ಥಿರ SemaphoreHandle_t sync;
ಸ್ಥಿರ SemaphoreHandle_t ಕೊನೆ_sema;
ಸ್ಥಿರ uint32_t ಚಕ್ರ_before_trigger;
ಸ್ಥಿರ uint32_t ಚಕ್ರ_before_exit;
ಸ್ಥಿರ uint32_t delta_enter_ಚಕ್ರಗಳು = 0;
ಸ್ಥಿರ uint32_t delta_exit_ಚಕ್ರಗಳು = 0;

ಸ್ಥಿರ ಶೂನ್ಯ software_isr_using_parameter_vportyield(ಶೂನ್ಯ *arg)
{
    (ಶೂನ್ಯ)arg;
    ತಳ_Type_t yield;
    delta_enter_ಚಕ್ರಗಳು += esp_cpu_get_ಚಕ್ರ_ಎಣಿಕೆ() - ಚಕ್ರ_before_trigger;
    TEST_CLR_INT_ಮುಖವಾಡ(1 << SW_ISR_LEVEL_1);

    xSemaphoreGiveFromISR(sync, &yield);
    ಪೋರ್ಟ್_YIELD_FROM_ISR(yield);

    ಚಕ್ರ_before_exit = esp_cpu_get_ಚಕ್ರ_ಎಣಿಕೆ();
}

ಸ್ಥಿರ ಶೂನ್ಯ software_isr_using_no_ವಾದ_vportyield(ಶೂನ್ಯ *arg)
{
    (ಶೂನ್ಯ)arg;
    ತಳ_Type_t yield;
    delta_enter_ಚಕ್ರಗಳು += esp_cpu_get_ಚಕ್ರ_ಎಣಿಕೆ() - ಚಕ್ರ_before_trigger;

    TEST_CLR_INT_ಮುಖವಾಡ(1 << SW_ISR_LEVEL_1);

    xSemaphoreGiveFromISR(sync, &yield);
    ಆದರೆ (yield) {
        ಪೋರ್ಟ್_YIELD_FROM_ISR();
    }
    ಚಕ್ರ_before_exit = esp_cpu_get_ಚಕ್ರ_ಎಣಿಕೆ();
}

ಸ್ಥಿರ ಶೂನ್ಯ test_task(ಶೂನ್ಯ *arg)
{
    (ಶೂನ್ಯ) arg;

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 10000; i++) {
        ಚಕ್ರ_before_trigger = esp_cpu_get_ಚಕ್ರ_ಎಣಿಕೆ();
        TEST_ಗಣ_INT_ಮುಖವಾಡ(1 << SW_ISR_LEVEL_1);
        xSemaphoreTake(sync, ಪೋರ್ಟ್_MAX_DELAY);
        delta_exit_ಚಕ್ರಗಳು += esp_cpu_get_ಚಕ್ರ_ಎಣಿಕೆ() - ಚಕ್ರ_before_exit;
    }

    delta_enter_ಚಕ್ರಗಳು /= 10000;
    delta_exit_ಚಕ್ರಗಳು /= 10000;

    xSemaphoreGive(ಕೊನೆ_sema);
    vTaskDelete(ಖಾಲಿ);
}

TEST_CASE("isr latency test vport-yield-from-isr with no parameter", "[freertos] [ignore]")
{
    intr_handle_t handle;
    esp_err_t err = esp_intr_alloc(ETS_ಆಂತರಿಕ_SW0_INTR_ಮೂಲ, ESP_INTR_FLAG_LEVEL1, &software_isr_using_no_ವಾದ_vportyield, ಖಾಲಿ, &handle);
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);

    sync = x_Semaphore_Create_ದ್ವಿಮಾನ();
    TEST_ASSERT(sync != ಖಾಲಿ);
    ಕೊನೆ_sema = x_Semaphore_Create_ದ್ವಿಮಾನ();
    TEST_ASSERT(ಕೊನೆ_sema != ಖಾಲಿ);
    x_Task_Create_Pinned_ಗಮ್ಯ_Core(test_task, "tst", 4096, ಖಾಲಿ, configMAX_PRIORITIES - 1, ಖಾಲಿ, 0);
    vTaskDelay(100);
    ತಳ_Type_t ಫಲಿತಾಂಶ = xSemaphoreTake(ಕೊನೆ_sema, ಪೋರ್ಟ್_MAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, ಫಲಿತಾಂಶ);
    TEST_PERFORMANCE_LESS_THAN(ISR_ENTER_ಚಕ್ರಗಳು, "%"PRIu32" cycles", delta_enter_ಚಕ್ರಗಳು);
    TEST_PERFORMANCE_LESS_THAN(ISR_EXIT_ಚಕ್ರಗಳು, "%"PRIu32" cycles", delta_exit_ಚಕ್ರಗಳು);

    esp_intr_free(handle);
}

TEST_CASE("isr latency test vport-yield-from-isr with parameter", "[freertos][ignore]")
{
    intr_handle_t handle;
    esp_err_t err = esp_intr_alloc(ETS_ಆಂತರಿಕ_SW0_INTR_ಮೂಲ, ESP_INTR_FLAG_LEVEL1, &software_isr_using_parameter_vportyield, ಖಾಲಿ, &handle);
    TEST_ASSERT_EQUAL_HEX32(ESP_OK, err);

    sync = x_Semaphore_Create_ದ್ವಿಮಾನ();
    TEST_ASSERT(sync != ಖಾಲಿ);
    ಕೊನೆ_sema = x_Semaphore_Create_ದ್ವಿಮಾನ();
    TEST_ASSERT(ಕೊನೆ_sema != ಖಾಲಿ);
    x_Task_Create_Pinned_ಗಮ್ಯ_Core(test_task, "tst", 4096, ಖಾಲಿ, configMAX_PRIORITIES - 1, ಖಾಲಿ, 0);
    ತಳ_Type_t ಫಲಿತಾಂಶ = xSemaphoreTake(ಕೊನೆ_sema, ಪೋರ್ಟ್_MAX_DELAY);
    TEST_ASSERT_EQUAL_HEX32(pdTRUE, ಫಲಿತಾಂಶ);
    TEST_PERFORMANCE_LESS_THAN(ISR_ENTER_ಚಕ್ರಗಳು, "%"PRIu32" cycles", delta_enter_ಚಕ್ರಗಳು);
    TEST_PERFORMANCE_LESS_THAN(ISR_EXIT_ಚಕ್ರಗಳು, "%"PRIu32" cycles", delta_exit_ಚಕ್ರಗಳು);

    esp_intr_free(handle);
}

#ಆದರೆ_ಅಂತ್ಯ // __riscv
