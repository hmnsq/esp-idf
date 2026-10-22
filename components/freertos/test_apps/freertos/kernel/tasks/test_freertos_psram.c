/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#شامل <stdio.h>
#شامل "freertos/FreeRTOS.h"
#شامل "freertos/task.h"
#شامل "freertos/idf_additions.h"
#شامل "esp_private/freertos_idf_additions_priv.h"
#شامل "unity.h"
#شامل "esp_heap_caps.h"
#شامل "unity_test_runner.h"
#شامل "memory_checks.h"

#define UNITY_FREERTOS_PRIORITY 5

#اگر CONFIG_SPIRAM

ایستا تهی task_delete_itself(تهی *arg)
{
    printf("starting task\n");
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(پوچ);
}

ایستا تهی task_should_never_run(تهی *arg)
{
    printf("ERROR: this task should not run, aborting\n");
    abort();
}

ایستا BaseType_t create_task(TaskFunction_t function,
                              size_t پشته_size,
                              تهی *task_arg,
                              صحیح core_رقم,
                              UBaseType_t heap_caps,
                              TaskHandle_t *task_handle)
{
#اگر CONFIG_FREERTOS_SMP
    UBaseType_t core_affinity_mask = (core_رقم == -1) ? tskNO_AFFINITY : 1 << core_رقم;
    بازگشت prvTaskCreateDynamicAffinitySetWithCaps(function,
                                                   "self_delete",
                                                   پشته_size,
                                                   task_arg,
                                                   UNITY_FREERTOS_PRIORITY + 1,
                                                   core_affinity_mask,
                                                   heap_caps,
                                                   task_handle);
#وگرنه
    ثابت BaseType_t task_core_رقم = (core_رقم == -1) ? tskNO_AFFINITY : core_رقم;
    بازگشت prvTaskCreateDynamicPinnedToCoreWithCaps(function,
                                                    "self_delete",
                                                    پشته_size,
                                                    task_arg,
                                                    UNITY_FREERTOS_PRIORITY + 1,
                                                    task_core_رقم,
                                                    heap_caps,
                                                    task_handle);
#endif
}

TEST_CASE("Out of memory failure", "[freertos][psram]")
{
    TaskHandle_t task_handle = پوچ;
    ثابت size_t پشته_SIZE = 0x80000000; // far larger than the virtual address space on ESP32
    UBaseType_t HEAP_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

    BaseType_t result = create_task(task_should_never_run,
                                    پشته_SIZE,
                                    (تهی *)xTaskGetCurrentTaskHandle(),
                                    -1,
                                    HEAP_CAPS,
                                    &task_handle);
    TEST_ASSERT_EQUAL(errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY, result);
    (تهی)task_handle;
}

TEST_CASE("Task with stack memory in PSRAM", "[freertos][psram]")
{
    TaskHandle_t task_handle = پوچ;
    ثابت size_t پشته_SIZE = 0x80000; // value is too large for any internal RAM on current chips
    UBaseType_t HEAP_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

    BaseType_t result = create_task(task_delete_itself,
                                    پشته_SIZE,
                                    (تهی *)xTaskGetCurrentTaskHandle(),
                                    -1,
                                    HEAP_CAPS,
                                    &task_handle);
    TEST_ASSERT_EQUAL(pdPASS, result);

    // synchronize with the task to make sure we don't return too early, thus giving it enough time
    // to delete itself and giving the idle task time to clean up memory (see delay in tearDown()).
    xTaskNotifyGive(task_handle);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
}

#اگر !CONFIG_FREERTOS_UNICORE
تعریفنوع ساختار {
    size_t recorded_core_رقم;
    TaskHandle_t parent_handle;
} report_corenum_info_t;

ایستا تهی task_report_corenum(تهی *arg)
{
    report_corenum_info_t *info = (report_corenum_info_t*) arg;
    info->recorded_core_رقم = xTaskGetCoreID(پوچ);
    xTaskNotifyGive(info->parent_handle);
    vTaskSuspend(پوچ);
}

/**
 * This test (roughly) verifies that xTaskCreateWithCapsAffinitySet() pins a task to the correct core.
 */
TEST_CASE("Task on specific core works", "[freertos][psram]")
{
    ثابت size_t پشته_SIZE = 0x1000;
    report_corenum_info_t corenum_info;
    TaskHandle_t task_handle;

    برای (صحیح corenum = 0; corenum < CONFIG_SOC_CPU_CORES_رقم; corenum++) {
        corenum_info.recorded_core_رقم = 0xFFFFFFFF;
        corenum_info.parent_handle = xTaskGetCurrentTaskHandle();
        UBaseType_t HEAP_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
        BaseType_t result = create_task(task_report_corenum,
                                        پشته_SIZE,
                                        (تهی *) & (corenum_info),
                                        corenum,
                                        HEAP_CAPS,
                                        &task_handle);

        TEST_ASSERT_EQUAL(pdPASS, result);

        // Wait for the created task to finish its job
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        TEST_ASSERT_EQUAL((size_t) corenum, corenum_info.recorded_core_رقم);

        vTaskDelete(task_handle);

        // Add a short delay to allow the idle task to free any remaining task memory
        vTaskDelay(10);
    }
}
#endif // !CONFIG_FREERTOS_UNICORE

#endif // SPIRAM
