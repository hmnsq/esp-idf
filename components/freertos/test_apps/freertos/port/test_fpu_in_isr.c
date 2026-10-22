/*
 * SPDX-FileCopyrightText: 2022-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#അന്തര്ഭാവ "sdkconfig.h"
#അന്തര്ഭാവ <math.h>
#അന്തര്ഭാവ "soc/soc_caps.h"
#അന്തര്ഭാവ "freertos/FreeRTOS.h"
#അന്തര്ഭാവ "freertos/task.h"
#അന്തര്ഭാവ "freertos/semphr.h"
#അന്തര്ഭാവ "unity.h"
#അന്തര്ഭാവ "test_utils.h"

#എങ്കിൽ SOC_CPU_HAS_FPU && CONFIG_FREERTOS_FPU_IN_ISR

// We can use xtensa API here as currently, non of the RISC-V targets have an FPU
#അന്തര്ഭാവ "xtensa_api.h"     // Replace with interrupt allocator API (IDF-3891)
#അന്തര്ഭാവ "esp_intr_alloc.h"

#define SW_ISR_LEVEL_1      7

സ്ഥിതം ശൂന്യം fpu_isr(ശൂന്യം *arg)
{
    // Clear the interrupt
    xt_set_intclear(1 << SW_ISR_LEVEL_1);
    /*
    Use the FPU
    - We test using a calculation that will cause a change in mantissa and exponent for extra thoroughness
    - cosf(0.0f) should return 1.0f, thus we are simply doubling test_float every iteration.
    - Therefore, we should end up with (0.01) * (2^8) = 2.56 at the end of the loop
    */
    അസ്ഥിര ദശാംശം test_float = 0.01f;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < 8; i++) {
        test_float = test_float * 2.0f * cosf(0.0f);
    }
    // We allow a 0.1% delta on the final result in case of any loss of precision from floating point calculations
    TEST_ASSERT_FLOAT_WITHIN(0.00256f, 2.56f, test_float);
}

/* ------------------------------------------------------------------------------------------------------------------ */

/*
Test FPU usage from a level 1 ISR

Purpose:
    - Test that the FPU can be used from a level 1 ISR
    - Test that the ISR using the FPU does not corrupt the interrupted task's FPU context
Procedure:
    - Allocate a level 1 ISR
    - Task uses the FPU then triggers the ISR
    - ISR uses the FPU as well (forcing the task's FPU context to be saved)
    - Task continues using the FPU (forcing its FPU context to be restored)
Expected:
    - ISR should use the FPU without issue
    - The interrupted task can continue using the FPU without issue
*/

TEST_CASE("FPU: Usage in level 1 ISR", "[freertos]")
{
    intr_handle_t isr_handle;
    TEST_ESP_OK(esp_intr_alloc(ETS_INTERNAL_SW0_INTR_SOURCE, ESP_INTR_FLAG_LEVEL1, &fpu_isr, ശൂന്യ, &isr_handle));
    /*
    Use the FPU (calculate a different value than in the ISR)
    - We test using a calculation that will cause a change in mantissa and exponent for extra thoroughness
    - cosf(0.0f) should return 1.0f, thus we are simply dividing test_float every iteration.
    */
    // We should end up with (2.56) / (2^4) = 0.16 at the end of the first loop
    അസ്ഥിര ദശാംശം test_float = 2.56f;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < 4; i++) {
        test_float = test_float / (2.0f * cosf(0.0f));
    }
    // We allow a 0.1% delta on the final result in case of any loss of precision from floating point calculations
    TEST_ASSERT_FLOAT_WITHIN(0.00016f, 0.16f, test_float);

    // Trigger the ISR
    xt_set_intset(1 << SW_ISR_LEVEL_1);

    // Continue using the FPU from a task context after the interrupt returns
    // We should end up with (0.16) / (2^4) = 0.01 at the end of the first loop
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < 4; i++) {
        test_float = test_float / (2.0f * cosf(0.0f));
    }
    // We allow a 0.1% delta on the final result in case of any loss of precision from floating point calculations
    TEST_ASSERT_FLOAT_WITHIN(0.00001f, 0.01f, test_float);

    // Free the ISR for ongoing tests
    TEST_ESP_OK(esp_intr_free(isr_handle));
    vTaskDelay(10); // Short delay to allow task memory to be freed
}

/* ------------------------------------------------------------------------------------------------------------------ */

/*
Test FPU usage in ISR does not affect an unpinned tasks

Purpose:
    - Test that the ISR using the FPU will not affect the interrupted task's affinity
Procedure:
    - Create an unpinned task
    - Unpinned task disables scheduling/preemption to ensure that it does not switch cores
    - Unpinned task allocates an ISR then triggers the ISR
    - The ISR interrupts the unpinned task then uses the FPU
    - Task reenables scheduling/preemption and cleans up
Expected:
    - The ISR using the FPU will not affect the unpinned task's affinity
*/

സ്ഥിതം ശൂന്യം unpinned_task(ശൂന്യം *arg)
{
    // Disable scheduling/preemption to make sure the current task doesn't switch cores
#എങ്കിൽ ( ( CONFIG_FREERTOS_SMP ) && ( !CONFIG_FREERTOS_UNICORE ) )
    vTaskPreemptionDisable(ശൂന്യ);
#അല്ലെങ്കിൽ
    vTaskSuspendAll();
#endif
    // Check that the task is unpinned
#എങ്കിൽ !CONFIG_FREERTOS_UNICORE
#എങ്കിൽ CONFIG_FREERTOS_SMP
    TEST_ASSERT_EQUAL(tskNO_AFFINITY, vTaskCoreAffinityGet(ശൂന്യ));
#അല്ലെങ്കിൽ
    TEST_ASSERT_EQUAL(tskNO_AFFINITY, xTaskGetCoreID(ശൂന്യ));
#endif
#endif // !CONFIG_FREERTOS_UNICORE

    // Allocate an ISR to use the FPU
    intr_handle_t isr_handle;
    TEST_ESP_OK(esp_intr_alloc(ETS_INTERNAL_SW0_INTR_SOURCE, ESP_INTR_FLAG_LEVEL1, &fpu_isr, ശൂന്യ, &isr_handle));
    // Trigger the ISR
    xt_set_intset(1 << SW_ISR_LEVEL_1);

    // Task should remain unpinned after the ISR uses the FPU
#എങ്കിൽ !CONFIG_FREERTOS_UNICORE
#എങ്കിൽ CONFIG_FREERTOS_SMP
    TEST_ASSERT_EQUAL(tskNO_AFFINITY, vTaskCoreAffinityGet(ശൂന്യ));
#അല്ലെങ്കിൽ
    TEST_ASSERT_EQUAL(tskNO_AFFINITY, xTaskGetCoreID(ശൂന്യ));
#endif
#endif // !CONFIG_FREERTOS_UNICORE
    // Re-enable scheduling/preemption
#എങ്കിൽ ( ( CONFIG_FREERTOS_SMP ) && ( !CONFIG_FREERTOS_UNICORE ) )
    vTaskPreemptionEnable(ശൂന്യ);
#അല്ലെങ്കിൽ
    xTaskResumeAll();
#endif

    // Free the ISR for ongoing tests
    TEST_ESP_OK(esp_intr_free(isr_handle));

    // Indicate done and self delete
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(ശൂന്യ);
}

TEST_CASE("FPU: Level 1 ISR does not affect unpinned task", "[freertos]")
{
    TaskHandle_t unity_task_handle = xTaskGetCurrentTaskHandle();
    xTaskCreate(unpinned_task, "unpin", 2048, (ശൂന്യം *)unity_task_handle, UNITY_FREERTOS_PRIORITY + 1, ശൂന്യ);
    // Wait for task to complete
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    vTaskDelay(10); // Short delay to allow task memory to be freed
}

#endif // SOC_CPU_HAS_FPU && CONFIG_FREERTOS_FPU_IN_ISR
