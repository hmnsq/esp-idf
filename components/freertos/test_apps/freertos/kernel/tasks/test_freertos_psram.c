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

#تعریف_کرو UNITY_FREERTOS_ترجیح 5

#اگر CONFIG_SPIRAM

ساکن خالی task_delete_itself(خالی *arg)
{
    لکھو("starting task\n");
    ulTaskNotifyTake(pdTRUE, پورٹ_MAX_DELAY);
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(تہی);
}

ساکن خالی task_should_never_تسلسل(خالی *arg)
{
    لکھو("ERROR: this task should not run, aborting\n");
    abort();
}

ساکن بنیاد_Type_t create_task(TaskFunction_t function,
                              size_t ڈھیر_سائز_2,
                              خالی *task_arg,
                              صحیح core_عدد,
                              UBaseType_t ڈھیر_caps,
                              TaskHandle_t *task_handle)
{
#اگر CONFIG_FREERTOS_SMP
    UBaseType_t core_affinity_نقاب = (core_عدد == -1) ? tskNO_AFFINITY : 1 << core_عدد;
    واپس prv_Task_Create_Dynamic_Affinity_سیٹ_With_Caps(function,
                                                   "self_delete",
                                                   ڈھیر_سائز_2,
                                                   task_arg,
                                                   UNITY_FREERTOS_ترجیح + 1,
                                                   core_affinity_نقاب,
                                                   ڈھیر_caps,
                                                   task_handle);
#ورنہ
    ثابت بنیاد_Type_t task_core_عدد = (core_عدد == -1) ? tskNO_AFFINITY : core_عدد;
    واپس prv_Task_Create_Dynamic_Pinned_مقصد_Core_With_Caps(function,
                                                    "self_delete",
                                                    ڈھیر_سائز_2,
                                                    task_arg,
                                                    UNITY_FREERTOS_ترجیح + 1,
                                                    task_core_عدد,
                                                    ڈھیر_caps,
                                                    task_handle);
#اگر_ختم
}

TEST_CASE("Out of memory failure", "[freertos][psram]")
{
    TaskHandle_t task_handle = تہی;
    ثابت size_t ڈھیر_سائز = 0x80000000; // far larger than the virtual address space on ESP32
    UBaseType_t ڈھیر_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8_ثنائی_ہندسہ);

    بنیاد_Type_t نتیجہ = create_task(task_should_never_تسلسل,
                                    ڈھیر_سائز,
                                    (خالی *)xTaskGetCurrentTaskHandle(),
                                    -1,
                                    ڈھیر_CAPS,
                                    &task_handle);
    TEST_ASSERT_EQUAL(err_COULD_NOT_ALLOCATE_REQUIRED_حافظہ, نتیجہ);
    (خالی)task_handle;
}

TEST_CASE("Task with stack memory in PSRAM", "[freertos][psram]")
{
    TaskHandle_t task_handle = تہی;
    ثابت size_t ڈھیر_سائز = 0x80000; // value is too large for any internal RAM on current chips
    UBaseType_t ڈھیر_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8_ثنائی_ہندسہ);

    بنیاد_Type_t نتیجہ = create_task(task_delete_itself,
                                    ڈھیر_سائز,
                                    (خالی *)xTaskGetCurrentTaskHandle(),
                                    -1,
                                    ڈھیر_CAPS,
                                    &task_handle);
    TEST_ASSERT_EQUAL(pdPASS, نتیجہ);

    // synchronize with the task to make sure we don't return too early, thus giving it enough time
    // to delete itself and giving the idle task time to clean up memory (see delay in tearDown()).
    xTaskNotifyGive(task_handle);
    ulTaskNotifyTake(pdTRUE, پورٹ_MAX_DELAY);
}

#اگر !CONFIG_FREERTOS_UNICORE
قسمتعریف ساخت {
    size_t recorded_core_عدد;
    TaskHandle_t والد_handle;
} report_corenum_info_t;

ساکن خالی task_report_corenum(خالی *arg)
{
    report_corenum_info_t *info = (report_corenum_info_t*) arg;
    info->recorded_core_عدد = xTaskGetCoreID(تہی);
    xTaskNotifyGive(info->والد_handle);
    vTaskSuspend(تہی);
}

/**
 * This test (roughly) verifies that xTaskCreateWithCapsAffinitySet() pins a task to the correct core.
 */
TEST_CASE("Task on specific core works", "[freertos][psram]")
{
    ثابت size_t ڈھیر_سائز = 0x1000;
    report_corenum_info_t corenum_info;
    TaskHandle_t task_handle;

    برائے (صحیح corenum = 0; corenum < CONFIG_SOC_CPU_CORES_عدد; corenum++) {
        corenum_info.recorded_core_عدد = 0xFFFFFFFF;
        corenum_info.والد_handle = xTaskGetCurrentTaskHandle();
        UBaseType_t ڈھیر_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8_ثنائی_ہندسہ);
        بنیاد_Type_t نتیجہ = create_task(task_report_corenum,
                                        ڈھیر_سائز,
                                        (خالی *) & (corenum_info),
                                        corenum,
                                        ڈھیر_CAPS,
                                        &task_handle);

        TEST_ASSERT_EQUAL(pdPASS, نتیجہ);

        // Wait for the created task to finish its job
        ulTaskNotifyTake(pdTRUE, پورٹ_MAX_DELAY);

        TEST_ASSERT_EQUAL((size_t) corenum, corenum_info.recorded_core_عدد);

        vTaskDelete(task_handle);

        // Add a short delay to allow the idle task to free any remaining task memory
        vTaskDelay(10);
    }
}
#اگر_ختم // !CONFIG_FREERTOS_UNICORE

#اگر_ختم // SPIRAM
