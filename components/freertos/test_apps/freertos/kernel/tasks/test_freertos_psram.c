/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ "freertos/FreeRTOS.h"
#அந்தர்பாவ "freertos/task.h"
#அந்தர்பாவ "freertos/idf_additions.h"
#அந்தர்பாவ "esp_private/freertos_idf_additions_priv.h"
#அந்தர்பாவ "unity.h"
#அந்தர்பாவ "esp_heap_caps.h"
#அந்தர்பாவ "unity_test_runner.h"
#அந்தர்பாவ "memory_checks.h"

#define UNITY_FREERTOS_PRIORITY 5

#எனில் CONFIG_SPIRAM

நிலையான வெறுமை task_delete_itself(வெறுமை *arg)
{
    printf("starting task\n");
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(வெற்று);
}

நிலையான வெறுமை task_should_never_run(வெறுமை *arg)
{
    printf("ERROR: this task should not run, aborting\n");
    abort();
}

நிலையான BaseType_t create_task(TaskFunction_t function,
                              size_t அடுக்கு_size,
                              வெறுமை *task_arg,
                              முழு core_இலக்கம்,
                              UBaseType_t heap_caps,
                              TaskHandle_t *task_handle)
{
#எனில் CONFIG_FREERTOS_SMP
    UBaseType_t core_affinity_mask = (core_இலக்கம் == -1) ? tskNO_AFFINITY : 1 << core_இலக்கம்;
    திரும்பு prvTaskCreateDynamicAffinitySetWithCaps(function,
                                                   "self_delete",
                                                   அடுக்கு_size,
                                                   task_arg,
                                                   UNITY_FREERTOS_PRIORITY + 1,
                                                   core_affinity_mask,
                                                   heap_caps,
                                                   task_handle);
#இல்லையெனில்
    மாறிலி BaseType_t task_core_இலக்கம் = (core_இலக்கம் == -1) ? tskNO_AFFINITY : core_இலக்கம்;
    திரும்பு prvTaskCreateDynamicPinnedToCoreWithCaps(function,
                                                    "self_delete",
                                                    அடுக்கு_size,
                                                    task_arg,
                                                    UNITY_FREERTOS_PRIORITY + 1,
                                                    task_core_இலக்கம்,
                                                    heap_caps,
                                                    task_handle);
#endif
}

TEST_CASE("Out of memory failure", "[freertos][psram]")
{
    TaskHandle_t task_handle = வெற்று;
    மாறிலி size_t அடுக்கு_SIZE = 0x80000000; // far larger than the virtual address space on ESP32
    UBaseType_t HEAP_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

    BaseType_t result = create_task(task_should_never_run,
                                    அடுக்கு_SIZE,
                                    (வெறுமை *)xTaskGetCurrentTaskHandle(),
                                    -1,
                                    HEAP_CAPS,
                                    &task_handle);
    TEST_ASSERT_EQUAL(errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY, result);
    (வெறுமை)task_handle;
}

TEST_CASE("Task with stack memory in PSRAM", "[freertos][psram]")
{
    TaskHandle_t task_handle = வெற்று;
    மாறிலி size_t அடுக்கு_SIZE = 0x80000; // value is too large for any internal RAM on current chips
    UBaseType_t HEAP_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

    BaseType_t result = create_task(task_delete_itself,
                                    அடுக்கு_SIZE,
                                    (வெறுமை *)xTaskGetCurrentTaskHandle(),
                                    -1,
                                    HEAP_CAPS,
                                    &task_handle);
    TEST_ASSERT_EQUAL(pdPASS, result);

    // synchronize with the task to make sure we don't return too early, thus giving it enough time
    // to delete itself and giving the idle task time to clean up memory (see delay in tearDown()).
    xTaskNotifyGive(task_handle);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
}

#எனில் !CONFIG_FREERTOS_UNICORE
வகைவரை கட்டமைப்பு {
    size_t recorded_core_இலக்கம்;
    TaskHandle_t parent_handle;
} report_corenum_info_t;

நிலையான வெறுமை task_report_corenum(வெறுமை *arg)
{
    report_corenum_info_t *info = (report_corenum_info_t*) arg;
    info->recorded_core_இலக்கம் = xTaskGetCoreID(வெற்று);
    xTaskNotifyGive(info->parent_handle);
    vTaskSuspend(வெற்று);
}

/**
 * This test (roughly) verifies that xTaskCreateWithCapsAffinitySet() pins a task to the correct core.
 */
TEST_CASE("Task on specific core works", "[freertos][psram]")
{
    மாறிலி size_t அடுக்கு_SIZE = 0x1000;
    report_corenum_info_t corenum_info;
    TaskHandle_t task_handle;

    க்கு (முழு corenum = 0; corenum < CONFIG_SOC_CPU_CORES_இலக்கம்; corenum++) {
        corenum_info.recorded_core_இலக்கம் = 0xFFFFFFFF;
        corenum_info.parent_handle = xTaskGetCurrentTaskHandle();
        UBaseType_t HEAP_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
        BaseType_t result = create_task(task_report_corenum,
                                        அடுக்கு_SIZE,
                                        (வெறுமை *) & (corenum_info),
                                        corenum,
                                        HEAP_CAPS,
                                        &task_handle);

        TEST_ASSERT_EQUAL(pdPASS, result);

        // Wait for the created task to finish its job
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        TEST_ASSERT_EQUAL((size_t) corenum, corenum_info.recorded_core_இலக்கம்);

        vTaskDelete(task_handle);

        // Add a short delay to allow the idle task to free any remaining task memory
        vTaskDelay(10);
    }
}
#endif // !CONFIG_FREERTOS_UNICORE

#endif // SPIRAM
