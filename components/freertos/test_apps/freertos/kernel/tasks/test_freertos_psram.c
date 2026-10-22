/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن <stdio.h>
#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن "freertos/idf_additions.h"
#ضمن "esp_private/freertos_idf_additions_priv.h"
#ضمن "unity.h"
#ضمن "esp_heap_caps.h"
#ضمن "unity_test_runner.h"
#ضمن "memory_checks.h"

#تعریف_کړه UNITY_FREERTOS_لومړیتوب 5

#إذا CONFIG_SPIRAM

ساكن فراغ task_delete_itself(فراغ *arg)
{
    چاپ_کړه("starting task\n");
    ulTaskNotifyTake(pdTRUE, پورټ_MAX_DELAY);
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(عدم);
}

ساكن فراغ task_should_never_پرلپسې(فراغ *arg)
{
    چاپ_کړه("ERROR: this task should not run, aborting\n");
    abort();
}

ساكن بنسټ_Type_t create_task(TaskFunction_t function,
                              size_t ډېری_اندازه_2,
                              فراغ *task_arg,
                              صحيح core_عدد,
                              UBaseType_t ډېرۍ_caps,
                              TaskHandle_t *task_handle)
{
#إذا CONFIG_FREERTOS_SMP
    UBaseType_t core_affinity_ماسک = (core_عدد == -1) ? tskNO_AFFINITY : 1 << core_عدد;
    أرجع prv_Task_Create_Dynamic_Affinity_سېټ_With_Caps(function,
                                                   "self_delete",
                                                   ډېری_اندازه_2,
                                                   task_arg,
                                                   UNITY_FREERTOS_لومړیتوب + 1,
                                                   core_affinity_ماسک,
                                                   ډېرۍ_caps,
                                                   task_handle);
#وإلا
    ثابت بنسټ_Type_t task_core_عدد = (core_عدد == -1) ? tskNO_AFFINITY : core_عدد;
    أرجع prv_Task_Create_Dynamic_Pinned_منزل_Core_With_Caps(function,
                                                    "self_delete",
                                                    ډېری_اندازه_2,
                                                    task_arg,
                                                    UNITY_FREERTOS_لومړیتوب + 1,
                                                    task_core_عدد,
                                                    ډېرۍ_caps,
                                                    task_handle);
#د_که_پای
}

TEST_CASE("Out of memory failure", "[freertos][psram]")
{
    TaskHandle_t task_handle = عدم;
    ثابت size_t ډېری_اندازه = 0x80000000; // far larger than the virtual address space on ESP32
    UBaseType_t ډېرۍ_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8_بټ);

    بنسټ_Type_t پایله = create_task(task_should_never_پرلپسې,
                                    ډېری_اندازه,
                                    (فراغ *)xTaskGetCurrentTaskHandle(),
                                    -1,
                                    ډېرۍ_CAPS,
                                    &task_handle);
    TEST_ASSERT_EQUAL(err_COULD_NOT_ALLOCATE_REQUIRED_حافظه, پایله);
    (فراغ)task_handle;
}

TEST_CASE("Task with stack memory in PSRAM", "[freertos][psram]")
{
    TaskHandle_t task_handle = عدم;
    ثابت size_t ډېری_اندازه = 0x80000; // value is too large for any internal RAM on current chips
    UBaseType_t ډېرۍ_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8_بټ);

    بنسټ_Type_t پایله = create_task(task_delete_itself,
                                    ډېری_اندازه,
                                    (فراغ *)xTaskGetCurrentTaskHandle(),
                                    -1,
                                    ډېرۍ_CAPS,
                                    &task_handle);
    TEST_ASSERT_EQUAL(pdPASS, پایله);

    // synchronize with the task to make sure we don't return too early, thus giving it enough time
    // to delete itself and giving the idle task time to clean up memory (see delay in tearDown()).
    xTaskNotifyGive(task_handle);
    ulTaskNotifyTake(pdTRUE, پورټ_MAX_DELAY);
}

#إذا !CONFIG_FREERTOS_UNICORE
تعريفنوع بنية {
    size_t recorded_core_عدد;
    TaskHandle_t پلار_handle;
} report_corenum_info_t;

ساكن فراغ task_report_corenum(فراغ *arg)
{
    report_corenum_info_t *info = (report_corenum_info_t*) arg;
    info->recorded_core_عدد = xTaskGetCoreID(عدم);
    xTaskNotifyGive(info->پلار_handle);
    vTaskSuspend(عدم);
}

/**
 * This test (roughly) verifies that xTaskCreateWithCapsAffinitySet() pins a task to the correct core.
 */
TEST_CASE("Task on specific core works", "[freertos][psram]")
{
    ثابت size_t ډېری_اندازه = 0x1000;
    report_corenum_info_t corenum_info;
    TaskHandle_t task_handle;

    لكل (صحيح corenum = 0; corenum < CONFIG_SOC_CPU_CORES_عدد; corenum++) {
        corenum_info.recorded_core_عدد = 0xFFFFFFFF;
        corenum_info.پلار_handle = xTaskGetCurrentTaskHandle();
        UBaseType_t ډېرۍ_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8_بټ);
        بنسټ_Type_t پایله = create_task(task_report_corenum,
                                        ډېری_اندازه,
                                        (فراغ *) & (corenum_info),
                                        corenum,
                                        ډېرۍ_CAPS,
                                        &task_handle);

        TEST_ASSERT_EQUAL(pdPASS, پایله);

        // Wait for the created task to finish its job
        ulTaskNotifyTake(pdTRUE, پورټ_MAX_DELAY);

        TEST_ASSERT_EQUAL((size_t) corenum, corenum_info.recorded_core_عدد);

        vTaskDelete(task_handle);

        // Add a short delay to allow the idle task to free any remaining task memory
        vTaskDelay(10);
    }
}
#د_که_پای // !CONFIG_FREERTOS_UNICORE

#د_که_پای // SPIRAM
