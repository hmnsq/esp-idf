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

#define UNITY_FREERTOS_PRIORITY 5

#ಯದಿ CONFIG_SPIRAM

ಸ್ಥಿರ ಶೂನ್ಯ task_delete_itself(ಶೂನ್ಯ *arg)
{
    printf("starting task\n");
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(ರಿಕ್ತ);
}

ಸ್ಥಿರ ಶೂನ್ಯ task_should_never_run(ಶೂನ್ಯ *arg)
{
    printf("ERROR: this task should not run, aborting\n");
    abort();
}

ಸ್ಥಿರ BaseType_t create_task(TaskFunction_t function,
                              size_t ರಾಶಿ_size,
                              ಶೂನ್ಯ *task_arg,
                              ಪೂರ್ಣಾಙ್ಕ core_ಅಂಕೆ,
                              UBaseType_t heap_caps,
                              TaskHandle_t *task_handle)
{
#ಯದಿ CONFIG_FREERTOS_SMP
    UBaseType_t core_affinity_mask = (core_ಅಂಕೆ == -1) ? tskNO_AFFINITY : 1 << core_ಅಂಕೆ;
    ಪ್ರತ್ಯಾಗಮ prvTaskCreateDynamicAffinitySetWithCaps(function,
                                                   "self_delete",
                                                   ರಾಶಿ_size,
                                                   task_arg,
                                                   UNITY_FREERTOS_PRIORITY + 1,
                                                   core_affinity_mask,
                                                   heap_caps,
                                                   task_handle);
#ಅನ್ಯಥಾ
    ಧ್ರುವ BaseType_t task_core_ಅಂಕೆ = (core_ಅಂಕೆ == -1) ? tskNO_AFFINITY : core_ಅಂಕೆ;
    ಪ್ರತ್ಯಾಗಮ prvTaskCreateDynamicPinnedToCoreWithCaps(function,
                                                    "self_delete",
                                                    ರಾಶಿ_size,
                                                    task_arg,
                                                    UNITY_FREERTOS_PRIORITY + 1,
                                                    task_core_ಅಂಕೆ,
                                                    heap_caps,
                                                    task_handle);
#endif
}

TEST_CASE("Out of memory failure", "[freertos][psram]")
{
    TaskHandle_t task_handle = ರಿಕ್ತ;
    ಧ್ರುವ size_t ರಾಶಿ_SIZE = 0x80000000; // far larger than the virtual address space on ESP32
    UBaseType_t HEAP_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

    BaseType_t result = create_task(task_should_never_run,
                                    ರಾಶಿ_SIZE,
                                    (ಶೂನ್ಯ *)xTaskGetCurrentTaskHandle(),
                                    -1,
                                    HEAP_CAPS,
                                    &task_handle);
    TEST_ASSERT_EQUAL(errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY, result);
    (ಶೂನ್ಯ)task_handle;
}

TEST_CASE("Task with stack memory in PSRAM", "[freertos][psram]")
{
    TaskHandle_t task_handle = ರಿಕ್ತ;
    ಧ್ರುವ size_t ರಾಶಿ_SIZE = 0x80000; // value is too large for any internal RAM on current chips
    UBaseType_t HEAP_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

    BaseType_t result = create_task(task_delete_itself,
                                    ರಾಶಿ_SIZE,
                                    (ಶೂನ್ಯ *)xTaskGetCurrentTaskHandle(),
                                    -1,
                                    HEAP_CAPS,
                                    &task_handle);
    TEST_ASSERT_EQUAL(pdPASS, result);

    // synchronize with the task to make sure we don't return too early, thus giving it enough time
    // to delete itself and giving the idle task time to clean up memory (see delay in tearDown()).
    xTaskNotifyGive(task_handle);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
}

#ಯದಿ !CONFIG_FREERTOS_UNICORE
ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    size_t recorded_core_ಅಂಕೆ;
    TaskHandle_t parent_handle;
} report_corenum_info_t;

ಸ್ಥಿರ ಶೂನ್ಯ task_report_corenum(ಶೂನ್ಯ *arg)
{
    report_corenum_info_t *info = (report_corenum_info_t*) arg;
    info->recorded_core_ಅಂಕೆ = xTaskGetCoreID(ರಿಕ್ತ);
    xTaskNotifyGive(info->parent_handle);
    vTaskSuspend(ರಿಕ್ತ);
}

/**
 * This test (roughly) verifies that xTaskCreateWithCapsAffinitySet() pins a task to the correct core.
 */
TEST_CASE("Task on specific core works", "[freertos][psram]")
{
    ಧ್ರುವ size_t ರಾಶಿ_SIZE = 0x1000;
    report_corenum_info_t corenum_info;
    TaskHandle_t task_handle;

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ corenum = 0; corenum < CONFIG_SOC_CPU_CORES_ಅಂಕೆ; corenum++) {
        corenum_info.recorded_core_ಅಂಕೆ = 0xFFFFFFFF;
        corenum_info.parent_handle = xTaskGetCurrentTaskHandle();
        UBaseType_t HEAP_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
        BaseType_t result = create_task(task_report_corenum,
                                        ರಾಶಿ_SIZE,
                                        (ಶೂನ್ಯ *) & (corenum_info),
                                        corenum,
                                        HEAP_CAPS,
                                        &task_handle);

        TEST_ASSERT_EQUAL(pdPASS, result);

        // Wait for the created task to finish its job
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        TEST_ASSERT_EQUAL((size_t) corenum, corenum_info.recorded_core_ಅಂಕೆ);

        vTaskDelete(task_handle);

        // Add a short delay to allow the idle task to free any remaining task memory
        vTaskDelay(10);
    }
}
#endif // !CONFIG_FREERTOS_UNICORE

#endif // SPIRAM
