/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব "freertos/FreeRTOS.h"
#অন্তর্ভাব "freertos/task.h"
#অন্তর্ভাব "freertos/idf_additions.h"
#অন্তর্ভাব "esp_private/freertos_idf_additions_priv.h"
#অন্তর্ভাব "unity.h"
#অন্তর্ভাব "esp_heap_caps.h"
#অন্তর্ভাব "unity_test_runner.h"
#অন্তর্ভাব "memory_checks.h"

#define UNITY_FREERTOS_PRIORITY 5

#যদি CONFIG_SPIRAM

স্থির শূন্য task_delete_itself(শূন্য *arg)
{
    printf("starting task\n");
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(রিক্ত);
}

স্থির শূন্য task_should_never_run(শূন্য *arg)
{
    printf("ERROR: this task should not run, aborting\n");
    abort();
}

স্থির BaseType_t create_task(TaskFunction_t function,
                              size_t স্তূপ_size,
                              শূন্য *task_arg,
                              পূর্ণাঙ্ক core_অঙ্ক,
                              UBaseType_t heap_caps,
                              TaskHandle_t *task_handle)
{
#যদি CONFIG_FREERTOS_SMP
    UBaseType_t core_affinity_mask = (core_অঙ্ক == -1) ? tskNO_AFFINITY : 1 << core_অঙ্ক;
    প্রত্যাগম prvTaskCreateDynamicAffinitySetWithCaps(function,
                                                   "self_delete",
                                                   স্তূপ_size,
                                                   task_arg,
                                                   UNITY_FREERTOS_PRIORITY + 1,
                                                   core_affinity_mask,
                                                   heap_caps,
                                                   task_handle);
#অন্যথা
    ধ্রুব BaseType_t task_core_অঙ্ক = (core_অঙ্ক == -1) ? tskNO_AFFINITY : core_অঙ্ক;
    প্রত্যাগম prvTaskCreateDynamicPinnedToCoreWithCaps(function,
                                                    "self_delete",
                                                    স্তূপ_size,
                                                    task_arg,
                                                    UNITY_FREERTOS_PRIORITY + 1,
                                                    task_core_অঙ্ক,
                                                    heap_caps,
                                                    task_handle);
#endif
}

TEST_CASE("Out of memory failure", "[freertos][psram]")
{
    TaskHandle_t task_handle = রিক্ত;
    ধ্রুব size_t স্তূপ_SIZE = 0x80000000; // far larger than the virtual address space on ESP32
    UBaseType_t HEAP_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

    BaseType_t result = create_task(task_should_never_run,
                                    স্তূপ_SIZE,
                                    (শূন্য *)xTaskGetCurrentTaskHandle(),
                                    -1,
                                    HEAP_CAPS,
                                    &task_handle);
    TEST_ASSERT_EQUAL(errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY, result);
    (শূন্য)task_handle;
}

TEST_CASE("Task with stack memory in PSRAM", "[freertos][psram]")
{
    TaskHandle_t task_handle = রিক্ত;
    ধ্রুব size_t স্তূপ_SIZE = 0x80000; // value is too large for any internal RAM on current chips
    UBaseType_t HEAP_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

    BaseType_t result = create_task(task_delete_itself,
                                    স্তূপ_SIZE,
                                    (শূন্য *)xTaskGetCurrentTaskHandle(),
                                    -1,
                                    HEAP_CAPS,
                                    &task_handle);
    TEST_ASSERT_EQUAL(pdPASS, result);

    // synchronize with the task to make sure we don't return too early, thus giving it enough time
    // to delete itself and giving the idle task time to clean up memory (see delay in tearDown()).
    xTaskNotifyGive(task_handle);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
}

#যদি !CONFIG_FREERTOS_UNICORE
প্রকারনির্দেশ সংরচনা {
    size_t recorded_core_অঙ্ক;
    TaskHandle_t parent_handle;
} report_corenum_info_t;

স্থির শূন্য task_report_corenum(শূন্য *arg)
{
    report_corenum_info_t *info = (report_corenum_info_t*) arg;
    info->recorded_core_অঙ্ক = xTaskGetCoreID(রিক্ত);
    xTaskNotifyGive(info->parent_handle);
    vTaskSuspend(রিক্ত);
}

/**
 * This test (roughly) verifies that xTaskCreateWithCapsAffinitySet() pins a task to the correct core.
 */
TEST_CASE("Task on specific core works", "[freertos][psram]")
{
    ধ্রুব size_t স্তূপ_SIZE = 0x1000;
    report_corenum_info_t corenum_info;
    TaskHandle_t task_handle;

    প্রতি (পূর্ণাঙ্ক corenum = 0; corenum < CONFIG_SOC_CPU_CORES_অঙ্ক; corenum++) {
        corenum_info.recorded_core_অঙ্ক = 0xFFFFFFFF;
        corenum_info.parent_handle = xTaskGetCurrentTaskHandle();
        UBaseType_t HEAP_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
        BaseType_t result = create_task(task_report_corenum,
                                        স্তূপ_SIZE,
                                        (শূন্য *) & (corenum_info),
                                        corenum,
                                        HEAP_CAPS,
                                        &task_handle);

        TEST_ASSERT_EQUAL(pdPASS, result);

        // Wait for the created task to finish its job
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        TEST_ASSERT_EQUAL((size_t) corenum, corenum_info.recorded_core_অঙ্ক);

        vTaskDelete(task_handle);

        // Add a short delay to allow the idle task to free any remaining task memory
        vTaskDelay(10);
    }
}
#endif // !CONFIG_FREERTOS_UNICORE

#endif // SPIRAM
