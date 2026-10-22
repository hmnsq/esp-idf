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

#تعریف_کن UNITY_FREERTOS_اولویت 5

#إذا CONFIG_SPIRAM

ساكن فراغ task_delete_itself(فراغ *arg)
{
    چاپ_کن("starting task\n");
    ulTaskNotifyTake(pdTRUE, پورت_MAX_DELAY);
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(عدم);
}

ساكن فراغ task_should_never_پیاپی(فراغ *arg)
{
    چاپ_کن("ERROR: this task should not run, aborting\n");
    abort();
}

ساكن پایه_Type_t create_task(TaskFunction_t function,
                              size_t پشته_اندازه_2,
                              فراغ *task_arg,
                              صحيح core_رقم,
                              UBaseType_t انباشته_caps,
                              TaskHandle_t *task_handle)
{
#إذا CONFIG_FREERTOS_SMP
    UBaseType_t core_affinity_ماسک = (core_رقم == -1) ? tskNO_AFFINITY : 1 << core_رقم;
    أرجع prv_Task_Create_Dynamic_Affinity_مجموعه_With_Caps(function,
                                                   "self_delete",
                                                   پشته_اندازه_2,
                                                   task_arg,
                                                   UNITY_FREERTOS_اولویت + 1,
                                                   core_affinity_ماسک,
                                                   انباشته_caps,
                                                   task_handle);
#وإلا
    ثابت پایه_Type_t task_core_رقم = (core_رقم == -1) ? tskNO_AFFINITY : core_رقم;
    أرجع prv_Task_Create_Dynamic_Pinned_مقصد_Core_With_Caps(function,
                                                    "self_delete",
                                                    پشته_اندازه_2,
                                                    task_arg,
                                                    UNITY_FREERTOS_اولویت + 1,
                                                    task_core_رقم,
                                                    انباشته_caps,
                                                    task_handle);
#پایان_اگر
}

TEST_CASE("Out of memory failure", "[freertos][psram]")
{
    TaskHandle_t task_handle = عدم;
    ثابت size_t پشته_اندازه = 0x80000000; // far larger than the virtual address space on ESP32
    UBaseType_t انباشته_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8_بیت);

    پایه_Type_t نتیجه = create_task(task_should_never_پیاپی,
                                    پشته_اندازه,
                                    (فراغ *)xTaskGetCurrentTaskHandle(),
                                    -1,
                                    انباشته_CAPS,
                                    &task_handle);
    TEST_ASSERT_EQUAL(err_COULD_NOT_ALLOCATE_REQUIRED_حافظه, نتیجه);
    (فراغ)task_handle;
}

TEST_CASE("Task with stack memory in PSRAM", "[freertos][psram]")
{
    TaskHandle_t task_handle = عدم;
    ثابت size_t پشته_اندازه = 0x80000; // value is too large for any internal RAM on current chips
    UBaseType_t انباشته_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8_بیت);

    پایه_Type_t نتیجه = create_task(task_delete_itself,
                                    پشته_اندازه,
                                    (فراغ *)xTaskGetCurrentTaskHandle(),
                                    -1,
                                    انباشته_CAPS,
                                    &task_handle);
    TEST_ASSERT_EQUAL(pdPASS, نتیجه);

    // synchronize with the task to make sure we don't return too early, thus giving it enough time
    // to delete itself and giving the idle task time to clean up memory (see delay in tearDown()).
    xTaskNotifyGive(task_handle);
    ulTaskNotifyTake(pdTRUE, پورت_MAX_DELAY);
}

#إذا !CONFIG_FREERTOS_UNICORE
تعريفنوع بنية {
    size_t recorded_core_رقم;
    TaskHandle_t والد_handle;
} report_corenum_info_t;

ساكن فراغ task_report_corenum(فراغ *arg)
{
    report_corenum_info_t *info = (report_corenum_info_t*) arg;
    info->recorded_core_رقم = xTaskGetCoreID(عدم);
    xTaskNotifyGive(info->والد_handle);
    vTaskSuspend(عدم);
}

/**
 * This test (roughly) verifies that xTaskCreateWithCapsAffinitySet() pins a task to the correct core.
 */
TEST_CASE("Task on specific core works", "[freertos][psram]")
{
    ثابت size_t پشته_اندازه = 0x1000;
    report_corenum_info_t corenum_info;
    TaskHandle_t task_handle;

    لكل (صحيح corenum = 0; corenum < CONFIG_SOC_CPU_CORES_رقم; corenum++) {
        corenum_info.recorded_core_رقم = 0xFFFFFFFF;
        corenum_info.والد_handle = xTaskGetCurrentTaskHandle();
        UBaseType_t انباشته_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8_بیت);
        پایه_Type_t نتیجه = create_task(task_report_corenum,
                                        پشته_اندازه,
                                        (فراغ *) & (corenum_info),
                                        corenum,
                                        انباشته_CAPS,
                                        &task_handle);

        TEST_ASSERT_EQUAL(pdPASS, نتیجه);

        // Wait for the created task to finish its job
        ulTaskNotifyTake(pdTRUE, پورت_MAX_DELAY);

        TEST_ASSERT_EQUAL((size_t) corenum, corenum_info.recorded_core_رقم);

        vTaskDelete(task_handle);

        // Add a short delay to allow the idle task to free any remaining task memory
        vTaskDelay(10);
    }
}
#پایان_اگر // !CONFIG_FREERTOS_UNICORE

#پایان_اگر // SPIRAM
