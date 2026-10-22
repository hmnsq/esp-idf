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

#जर CONFIG_SPIRAM

स्थिर शून्य task_delete_itself(शून्य *arg)
{
    म_लिखो("starting task\n");
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(रिक्त);
}

स्थिर शून्य task_should_never_run(शून्य *arg)
{
    म_लिखो("ERROR: this task should not run, aborting\n");
    पात();
}

स्थिर BaseType_t create_task(TaskFunction_t function,
                              माप_प्रकार चळत_size,
                              शून्य *task_arg,
                              पूर्णांक core_अंक,
                              UBaseType_t heap_caps,
                              TaskHandle_t *task_handle)
{
#जर CONFIG_FREERTOS_SMP
    UBaseType_t core_affinity_mask = (core_अंक == -1) ? tskNO_AFFINITY : 1 << core_अंक;
    परत prvTaskCreateDynamicAffinitySetWithCaps(function,
                                                   "self_delete",
                                                   चळत_size,
                                                   task_arg,
                                                   UNITY_FREERTOS_PRIORITY + 1,
                                                   core_affinity_mask,
                                                   heap_caps,
                                                   task_handle);
#अन्यथा
    स्थिरांक BaseType_t task_core_अंक = (core_अंक == -1) ? tskNO_AFFINITY : core_अंक;
    परत prvTaskCreateDynamicPinnedToCoreWithCaps(function,
                                                    "self_delete",
                                                    चळत_size,
                                                    task_arg,
                                                    UNITY_FREERTOS_PRIORITY + 1,
                                                    task_core_अंक,
                                                    heap_caps,
                                                    task_handle);
#endif
}

TEST_CASE("Out of memory failure", "[freertos][psram]")
{
    TaskHandle_t task_handle = रिक्त;
    स्थिरांक माप_प्रकार चळत_SIZE = 0x80000000; // far larger than the virtual address space on ESP32
    UBaseType_t HEAP_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

    BaseType_t result = create_task(task_should_never_run,
                                    चळत_SIZE,
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
    स्थिरांक माप_प्रकार चळत_SIZE = 0x80000; // value is too large for any internal RAM on current chips
    UBaseType_t HEAP_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

    BaseType_t result = create_task(task_delete_itself,
                                    चळत_SIZE,
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

#जर !CONFIG_FREERTOS_UNICORE
प्रकारव्याख्या संरचना {
    माप_प्रकार recorded_core_अंक;
    TaskHandle_t parent_handle;
} report_corenum_info_t;

स्थिर शून्य task_report_corenum(शून्य *arg)
{
    report_corenum_info_t *info = (report_corenum_info_t*) arg;
    info->recorded_core_अंक = xTaskGetCoreID(रिक्त);
    xTaskNotifyGive(info->parent_handle);
    vTaskSuspend(रिक्त);
}

/**
 * This test (roughly) verifies that xTaskCreateWithCapsAffinitySet() pins a task to the correct core.
 */
TEST_CASE("Task on specific core works", "[freertos][psram]")
{
    स्थिरांक माप_प्रकार चळत_SIZE = 0x1000;
    report_corenum_info_t corenum_info;
    TaskHandle_t task_handle;

    साठी (पूर्णांक corenum = 0; corenum < CONFIG_SOC_CPU_CORES_अंक; corenum++) {
        corenum_info.recorded_core_अंक = 0xFFFFFFFF;
        corenum_info.parent_handle = xTaskGetCurrentTaskHandle();
        UBaseType_t HEAP_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
        BaseType_t result = create_task(task_report_corenum,
                                        चळत_SIZE,
                                        (शून्य *) & (corenum_info),
                                        corenum,
                                        HEAP_CAPS,
                                        &task_handle);

        TEST_ASSERT_EQUAL(pdPASS, result);

        // Wait for the created task to finish its job
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        TEST_ASSERT_EQUAL((माप_प्रकार) corenum, corenum_info.recorded_core_अंक);

        vTaskDelete(task_handle);

        // Add a short delay to allow the idle task to free any remaining task memory
        vTaskDelay(10);
    }
}
#endif // !CONFIG_FREERTOS_UNICORE

#endif // SPIRAM
