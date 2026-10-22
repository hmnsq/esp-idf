/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#समावेश <stdio.h>
#समावेश "freertos/FreeRTOS.h"
#समावेश "freertos/task.h"
#समावेश "freertos/idf_additions.h"
#समावेश "esp_private/freertos_idf_additions_priv.h"
#समावेश "unity.h"
#समावेश "esp_heap_caps.h"
#समावेश "unity_test_runner.h"
#समावेश "memory_checks.h"

#घोषणा UNITY_FREERTOS_PRIORITY 5

#अगर CONFIG_SPIRAM

अटल व्योम task_delete_itself(व्योम *arg)
{
    म_लिखो("starting task\n");
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(शून्य);
}

अटल व्योम task_should_never_run(व्योम *arg)
{
    म_लिखो("ERROR: this task should not run, aborting\n");
    पात();
}

अटल BaseType_t create_task(TaskFunction_t function,
                              माप_प्रकार ढेर_आकार_2,
                              व्योम *task_arg,
                              पूर्णांक core_अंक,
                              UBaseType_t heap_caps,
                              TaskHandle_t *task_handle)
{
#अगर CONFIG_FREERTOS_SMP
    UBaseType_t core_affinity_mask = (core_अंक == -1) ? tskNO_AFFINITY : 1 << core_अंक;
    वापस prvTaskCreateDynamicAffinitySetWithCaps(function,
                                                   "self_delete",
                                                   ढेर_आकार_2,
                                                   task_arg,
                                                   UNITY_FREERTOS_PRIORITY + 1,
                                                   core_affinity_mask,
                                                   heap_caps,
                                                   task_handle);
#अन्यथा
    स्थिर BaseType_t task_core_अंक = (core_अंक == -1) ? tskNO_AFFINITY : core_अंक;
    वापस prvTaskCreateDynamicPinnedToCoreWithCaps(function,
                                                    "self_delete",
                                                    ढेर_आकार_2,
                                                    task_arg,
                                                    UNITY_FREERTOS_PRIORITY + 1,
                                                    task_core_अंक,
                                                    heap_caps,
                                                    task_handle);
#पूर्ण_अगर
}

TEST_CASE("Out of memory failure", "[freertos][psram]")
{
    TaskHandle_t task_handle = शून्य;
    स्थिर माप_प्रकार ढेर_आकार = 0x80000000; // far larger than the virtual address space on ESP32
    UBaseType_t HEAP_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

    BaseType_t परिणाम = create_task(task_should_never_run,
                                    ढेर_आकार,
                                    (व्योम *)xTaskGetCurrentTaskHandle(),
                                    -1,
                                    HEAP_CAPS,
                                    &task_handle);
    TEST_ASSERT_EQUAL(errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY, परिणाम);
    (व्योम)task_handle;
}

TEST_CASE("Task with stack memory in PSRAM", "[freertos][psram]")
{
    TaskHandle_t task_handle = शून्य;
    स्थिर माप_प्रकार ढेर_आकार = 0x80000; // value is too large for any internal RAM on current chips
    UBaseType_t HEAP_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

    BaseType_t परिणाम = create_task(task_delete_itself,
                                    ढेर_आकार,
                                    (व्योम *)xTaskGetCurrentTaskHandle(),
                                    -1,
                                    HEAP_CAPS,
                                    &task_handle);
    TEST_ASSERT_EQUAL(pdPASS, परिणाम);

    // synchronize with the task to make sure we don't return too early, thus giving it enough time
    // to delete itself and giving the idle task time to clean up memory (see delay in tearDown()).
    xTaskNotifyGive(task_handle);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
}

#अगर !CONFIG_FREERTOS_UNICORE
प्रकार काष्ठा {
    माप_प्रकार recorded_core_अंक;
    TaskHandle_t parent_handle;
} report_corenum_info_t;

अटल व्योम task_report_corenum(व्योम *arg)
{
    report_corenum_info_t *info = (report_corenum_info_t*) arg;
    info->recorded_core_अंक = xTaskGetCoreID(शून्य);
    xTaskNotifyGive(info->parent_handle);
    vTaskSuspend(शून्य);
}

/**
 * This test (roughly) verifies that xTaskCreateWithCapsAffinitySet() pins a task to the correct core.
 */
TEST_CASE("Task on specific core works", "[freertos][psram]")
{
    स्थिर माप_प्रकार ढेर_आकार = 0x1000;
    report_corenum_info_t corenum_info;
    TaskHandle_t task_handle;

    क्रम (पूर्णांक corenum = 0; corenum < CONFIG_SOC_CPU_CORES_अंक; corenum++) {
        corenum_info.recorded_core_अंक = 0xFFFFFFFF;
        corenum_info.parent_handle = xTaskGetCurrentTaskHandle();
        UBaseType_t HEAP_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
        BaseType_t परिणाम = create_task(task_report_corenum,
                                        ढेर_आकार,
                                        (व्योम *) & (corenum_info),
                                        corenum,
                                        HEAP_CAPS,
                                        &task_handle);

        TEST_ASSERT_EQUAL(pdPASS, परिणाम);

        // Wait for the created task to finish its job
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        TEST_ASSERT_EQUAL((माप_प्रकार) corenum, corenum_info.recorded_core_अंक);

        vTaskDelete(task_handle);

        // Add a short delay to allow the idle task to free any remaining task memory
        vTaskDelay(10);
    }
}
#पूर्ण_अगर // !CONFIG_FREERTOS_UNICORE

#पूर्ण_अगर // SPIRAM
