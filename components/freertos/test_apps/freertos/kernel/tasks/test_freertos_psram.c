/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ "freertos/idf_additions.h"
#ಅನ್ತರ್ಭಾವ "esp_private/freertos_idf_additions_priv.h"
#ಅನ್ತರ್ಭಾವ "unity.h"
#ಅನ್ತರ್ಭಾವ "esp_heap_caps.h"
#ಅನ್ತರ್ಭಾವ "unity_test_runner.h"
#ಅನ್ತರ್ಭಾವ "memory_checks.h"

#ವ್ಯಾಖ್ಯಾನಿಸು UNITY_FREERTOS_ಆದ್ಯತೆ 5

#ಆದರೆ CONFIG_SPIRAM

ಸ್ಥಿರ ಶೂನ್ಯ task_delete_itself(ಶೂನ್ಯ *arg)
{
    ಬರೆ("starting task\n");
    ulTaskNotifyTake(pdTRUE, ಪೋರ್ಟ್_MAX_DELAY);
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(ಖಾಲಿ);
}

ಸ್ಥಿರ ಶೂನ್ಯ task_should_never_ಸತತ(ಶೂನ್ಯ *arg)
{
    ಬರೆ("ERROR: this task should not run, aborting\n");
    abort();
}

ಸ್ಥಿರ ತಳ_Type_t create_task(TaskFunction_t function,
                              size_t ರಾಶಿ_ಗಾತ್ರ_2,
                              ಶೂನ್ಯ *task_arg,
                              ಪೂರ್ಣಾಂಕ core_ಅಂಕೆ,
                              UBaseType_t ರಾಶಿ_caps,
                              TaskHandle_t *task_handle)
{
#ಆದರೆ CONFIG_FREERTOS_SMP
    UBaseType_t core_affinity_ಮುಖವಾಡ = (core_ಅಂಕೆ == -1) ? tskNO_AFFINITY : 1 << core_ಅಂಕೆ;
    ಹಿಂತಿರುಗಿ prv_Task_Create_Dynamic_Affinity_ಗಣ_With_Caps(function,
                                                   "self_delete",
                                                   ರಾಶಿ_ಗಾತ್ರ_2,
                                                   task_arg,
                                                   UNITY_FREERTOS_ಆದ್ಯತೆ + 1,
                                                   core_affinity_ಮುಖವಾಡ,
                                                   ರಾಶಿ_caps,
                                                   task_handle);
#ಇಲ್ಲದಿದ್ದರೆ
    ಸ್ಥಿರಾಂಕ ತಳ_Type_t task_core_ಅಂಕೆ = (core_ಅಂಕೆ == -1) ? tskNO_AFFINITY : core_ಅಂಕೆ;
    ಹಿಂತಿರುಗಿ prv_Task_Create_Dynamic_Pinned_ಗಮ್ಯ_Core_With_Caps(function,
                                                    "self_delete",
                                                    ರಾಶಿ_ಗಾತ್ರ_2,
                                                    task_arg,
                                                    UNITY_FREERTOS_ಆದ್ಯತೆ + 1,
                                                    task_core_ಅಂಕೆ,
                                                    ರಾಶಿ_caps,
                                                    task_handle);
#ಆದರೆ_ಅಂತ್ಯ
}

TEST_CASE("Out of memory failure", "[freertos][psram]")
{
    TaskHandle_t task_handle = ಖಾಲಿ;
    ಸ್ಥಿರಾಂಕ size_t ರಾಶಿ_ಗಾತ್ರ = 0x80000000; // far larger than the virtual address space on ESP32
    UBaseType_t ರಾಶಿ_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8_ಬಿಟ್);

    ತಳ_Type_t ಫಲಿತಾಂಶ = create_task(task_should_never_ಸತತ,
                                    ರಾಶಿ_ಗಾತ್ರ,
                                    (ಶೂನ್ಯ *)xTaskGetCurrentTaskHandle(),
                                    -1,
                                    ರಾಶಿ_CAPS,
                                    &task_handle);
    TEST_ASSERT_EQUAL(err_COULD_NOT_ALLOCATE_REQUIRED_ಸ್ಮರಣೆ, ಫಲಿತಾಂಶ);
    (ಶೂನ್ಯ)task_handle;
}

TEST_CASE("Task with stack memory in PSRAM", "[freertos][psram]")
{
    TaskHandle_t task_handle = ಖಾಲಿ;
    ಸ್ಥಿರಾಂಕ size_t ರಾಶಿ_ಗಾತ್ರ = 0x80000; // value is too large for any internal RAM on current chips
    UBaseType_t ರಾಶಿ_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8_ಬಿಟ್);

    ತಳ_Type_t ಫಲಿತಾಂಶ = create_task(task_delete_itself,
                                    ರಾಶಿ_ಗಾತ್ರ,
                                    (ಶೂನ್ಯ *)xTaskGetCurrentTaskHandle(),
                                    -1,
                                    ರಾಶಿ_CAPS,
                                    &task_handle);
    TEST_ASSERT_EQUAL(pdPASS, ಫಲಿತಾಂಶ);

    // synchronize with the task to make sure we don't return too early, thus giving it enough time
    // to delete itself and giving the idle task time to clean up memory (see delay in tearDown()).
    xTaskNotifyGive(task_handle);
    ulTaskNotifyTake(pdTRUE, ಪೋರ್ಟ್_MAX_DELAY);
}

#ಆದರೆ !CONFIG_FREERTOS_UNICORE
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    size_t recorded_core_ಅಂಕೆ;
    TaskHandle_t ಪೋಷಕ_handle;
} report_corenum_info_t;

ಸ್ಥಿರ ಶೂನ್ಯ task_report_corenum(ಶೂನ್ಯ *arg)
{
    report_corenum_info_t *info = (report_corenum_info_t*) arg;
    info->recorded_core_ಅಂಕೆ = xTaskGetCoreID(ಖಾಲಿ);
    xTaskNotifyGive(info->ಪೋಷಕ_handle);
    vTaskSuspend(ಖಾಲಿ);
}

/**
 * This test (roughly) verifies that xTaskCreateWithCapsAffinitySet() pins a task to the correct core.
 */
TEST_CASE("Task on specific core works", "[freertos][psram]")
{
    ಸ್ಥಿರಾಂಕ size_t ರಾಶಿ_ಗಾತ್ರ = 0x1000;
    report_corenum_info_t corenum_info;
    TaskHandle_t task_handle;

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ corenum = 0; corenum < CONFIG_SOC_CPU_CORES_ಅಂಕೆ; corenum++) {
        corenum_info.recorded_core_ಅಂಕೆ = 0xFFFFFFFF;
        corenum_info.ಪೋಷಕ_handle = xTaskGetCurrentTaskHandle();
        UBaseType_t ರಾಶಿ_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8_ಬಿಟ್);
        ತಳ_Type_t ಫಲಿತಾಂಶ = create_task(task_report_corenum,
                                        ರಾಶಿ_ಗಾತ್ರ,
                                        (ಶೂನ್ಯ *) & (corenum_info),
                                        corenum,
                                        ರಾಶಿ_CAPS,
                                        &task_handle);

        TEST_ASSERT_EQUAL(pdPASS, ಫಲಿತಾಂಶ);

        // Wait for the created task to finish its job
        ulTaskNotifyTake(pdTRUE, ಪೋರ್ಟ್_MAX_DELAY);

        TEST_ASSERT_EQUAL((size_t) corenum, corenum_info.recorded_core_ಅಂಕೆ);

        vTaskDelete(task_handle);

        // Add a short delay to allow the idle task to free any remaining task memory
        vTaskDelay(10);
    }
}
#ಆದರೆ_ಅಂತ್ಯ // !CONFIG_FREERTOS_UNICORE

#ಆದರೆ_ಅಂತ್ಯ // SPIRAM
