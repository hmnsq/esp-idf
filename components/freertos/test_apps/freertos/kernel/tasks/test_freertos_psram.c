/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#अन्तर्भाव <stdio.h>
#अन्तर्भाव "freertos/FreeRTOS.h"
#अन्तर्भाव "freertos/task.h"
#अन्तर्भाव "freertos/idf_additions.h"
#अन्तर्भाव "esp_private/freertos_idf_additions_priv.h"
#अन्तर्भाव "unity.h"
#अन्तर्भाव "esp_heap_caps.h"
#अन्तर्भाव "unity_test_runner.h"
#अन्तर्भाव "memory_checks.h"

#define UNITY_FREERTOS_PRIORITY 5

#जइ CONFIG_SPIRAM

स्थिर शून्य task_delete_itself(शून्य *arg)
{
    printf("starting task\n");
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(रिक्त);
}

स्थिर शून्य task_should_never_run(शून्य *arg)
{
    printf("ERROR: this task should not run, aborting\n");
    abort();
}

स्थिर BaseType_t create_task(TaskFunction_t function,
                              size_t राशिः_size,
                              शून्य *task_arg,
                              पूर्णाङ्क core_अङ्कः,
                              UBaseType_t heap_caps,
                              TaskHandle_t *task_handle)
{
#जइ CONFIG_FREERTOS_SMP
    UBaseType_t core_affinity_mask = (core_अङ्कः == -1) ? tskNO_AFFINITY : 1 << core_अङ्कः;
    प्रत्यागम prvTaskCreateDynamicAffinitySetWithCaps(function,
                                                   "self_delete",
                                                   राशिः_size,
                                                   task_arg,
                                                   UNITY_FREERTOS_PRIORITY + 1,
                                                   core_affinity_mask,
                                                   heap_caps,
                                                   task_handle);
#अण्णहा
    ध्रुव BaseType_t task_core_अङ्कः = (core_अङ्कः == -1) ? tskNO_AFFINITY : core_अङ्कः;
    प्रत्यागम prvTaskCreateDynamicPinnedToCoreWithCaps(function,
                                                    "self_delete",
                                                    राशिः_size,
                                                    task_arg,
                                                    UNITY_FREERTOS_PRIORITY + 1,
                                                    task_core_अङ्कः,
                                                    heap_caps,
                                                    task_handle);
#endif
}

TEST_CASE("Out of memory failure", "[freertos][psram]")
{
    TaskHandle_t task_handle = रिक्त;
    ध्रुव size_t राशिः_SIZE = 0x80000000; // far larger than the virtual address space on ESP32
    UBaseType_t HEAP_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

    BaseType_t result = create_task(task_should_never_run,
                                    राशिः_SIZE,
                                    (शून्य *)xTaskGetCurrentTaskHandle(),
                                    -1,
                                    HEAP_CAPS,
                                    &task_handle);
    TEST_ASSERT_EQUAL(errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY, result);
    (शून्य)task_handle;
}

TEST_CASE("Task with stack memory in PSRAM", "[freertos][psram]")
{
    TaskHandle_t task_handle = रिक्त;
    ध्रुव size_t राशिः_SIZE = 0x80000; // value is too large for any internal RAM on current chips
    UBaseType_t HEAP_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

    BaseType_t result = create_task(task_delete_itself,
                                    राशिः_SIZE,
                                    (शून्य *)xTaskGetCurrentTaskHandle(),
                                    -1,
                                    HEAP_CAPS,
                                    &task_handle);
    TEST_ASSERT_EQUAL(pdPASS, result);

    // synchronize with the task to make sure we don't return too early, thus giving it enough time
    // to delete itself and giving the idle task time to clean up memory (see delay in tearDown()).
    xTaskNotifyGive(task_handle);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
}

#जइ !CONFIG_FREERTOS_UNICORE
प्रकारनिर्देश संरचना {
    size_t recorded_core_अङ्कः;
    TaskHandle_t parent_handle;
} report_corenum_info_t;

स्थिर शून्य task_report_corenum(शून्य *arg)
{
    report_corenum_info_t *info = (report_corenum_info_t*) arg;
    info->recorded_core_अङ्कः = xTaskGetCoreID(रिक्त);
    xTaskNotifyGive(info->parent_handle);
    vTaskSuspend(रिक्त);
}

/**
 * This test (roughly) verifies that xTaskCreateWithCapsAffinitySet() pins a task to the correct core.
 */
TEST_CASE("Task on specific core works", "[freertos][psram]")
{
    ध्रुव size_t राशिः_SIZE = 0x1000;
    report_corenum_info_t corenum_info;
    TaskHandle_t task_handle;

    प्रति (पूर्णाङ्क corenum = 0; corenum < CONFIG_SOC_CPU_CORES_अङ्कः; corenum++) {
        corenum_info.recorded_core_अङ्कः = 0xFFFFFFFF;
        corenum_info.parent_handle = xTaskGetCurrentTaskHandle();
        UBaseType_t HEAP_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
        BaseType_t result = create_task(task_report_corenum,
                                        राशिः_SIZE,
                                        (शून्य *) & (corenum_info),
                                        corenum,
                                        HEAP_CAPS,
                                        &task_handle);

        TEST_ASSERT_EQUAL(pdPASS, result);

        // Wait for the created task to finish its job
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        TEST_ASSERT_EQUAL((size_t) corenum, corenum_info.recorded_core_अङ्कः);

        vTaskDelete(task_handle);

        // Add a short delay to allow the idle task to free any remaining task memory
        vTaskDelay(10);
    }
}
#endif // !CONFIG_FREERTOS_UNICORE

#endif // SPIRAM
