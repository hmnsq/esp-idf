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

#وصف_ڪريو UNITY_FREERTOS_ترجيح 5

#إذا CONFIG_SPIRAM

ساكن فراغ task_delete_itself(فراغ *arg)
{
    لکو("starting task\n");
    ulTaskNotifyTake(pdTRUE, پورٽ_MAX_DELAY);
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(عدم);
}

ساكن فراغ task_should_never_لاڳيتو(فراغ *arg)
{
    لکو("ERROR: this task should not run, aborting\n");
    abort();
}

ساكن بنياد_Type_t create_task(TaskFunction_t function,
                              size_t ڍير_سائيز_2,
                              فراغ *task_arg,
                              صحيح core_انگ,
                              UBaseType_t ڍير_caps,
                              TaskHandle_t *task_handle)
{
#إذا CONFIG_FREERTOS_SMP
    UBaseType_t core_affinity_نقاب = (core_انگ == -1) ? tskNO_AFFINITY : 1 << core_انگ;
    أرجع prv_Task_Create_Dynamic_Affinity_سيٽ_With_Caps(function,
                                                   "self_delete",
                                                   ڍير_سائيز_2,
                                                   task_arg,
                                                   UNITY_FREERTOS_ترجيح + 1,
                                                   core_affinity_نقاب,
                                                   ڍير_caps,
                                                   task_handle);
#وإلا
    ثابت بنياد_Type_t task_core_انگ = (core_انگ == -1) ? tskNO_AFFINITY : core_انگ;
    أرجع prv_Task_Create_Dynamic_Pinned_منزل_Core_With_Caps(function,
                                                    "self_delete",
                                                    ڍير_سائيز_2,
                                                    task_arg,
                                                    UNITY_FREERTOS_ترجيح + 1,
                                                    task_core_انگ,
                                                    ڍير_caps,
                                                    task_handle);
#جيڪڏهن_پورو
}

TEST_CASE("Out of memory failure", "[freertos][psram]")
{
    TaskHandle_t task_handle = عدم;
    ثابت size_t ڍير_سائيز = 0x80000000; // far larger than the virtual address space on ESP32
    UBaseType_t ڍير_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8_ٻٽو_انگ);

    بنياد_Type_t نتيجو = create_task(task_should_never_لاڳيتو,
                                    ڍير_سائيز,
                                    (فراغ *)xTaskGetCurrentTaskHandle(),
                                    -1,
                                    ڍير_CAPS,
                                    &task_handle);
    TEST_ASSERT_EQUAL(err_COULD_NOT_ALLOCATE_REQUIRED_يادگيري, نتيجو);
    (فراغ)task_handle;
}

TEST_CASE("Task with stack memory in PSRAM", "[freertos][psram]")
{
    TaskHandle_t task_handle = عدم;
    ثابت size_t ڍير_سائيز = 0x80000; // value is too large for any internal RAM on current chips
    UBaseType_t ڍير_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8_ٻٽو_انگ);

    بنياد_Type_t نتيجو = create_task(task_delete_itself,
                                    ڍير_سائيز,
                                    (فراغ *)xTaskGetCurrentTaskHandle(),
                                    -1,
                                    ڍير_CAPS,
                                    &task_handle);
    TEST_ASSERT_EQUAL(pdPASS, نتيجو);

    // synchronize with the task to make sure we don't return too early, thus giving it enough time
    // to delete itself and giving the idle task time to clean up memory (see delay in tearDown()).
    xTaskNotifyGive(task_handle);
    ulTaskNotifyTake(pdTRUE, پورٽ_MAX_DELAY);
}

#إذا !CONFIG_FREERTOS_UNICORE
تعريفنوع بنية {
    size_t recorded_core_انگ;
    TaskHandle_t والد_handle;
} report_corenum_info_t;

ساكن فراغ task_report_corenum(فراغ *arg)
{
    report_corenum_info_t *info = (report_corenum_info_t*) arg;
    info->recorded_core_انگ = xTaskGetCoreID(عدم);
    xTaskNotifyGive(info->والد_handle);
    vTaskSuspend(عدم);
}

/**
 * This test (roughly) verifies that xTaskCreateWithCapsAffinitySet() pins a task to the correct core.
 */
TEST_CASE("Task on specific core works", "[freertos][psram]")
{
    ثابت size_t ڍير_سائيز = 0x1000;
    report_corenum_info_t corenum_info;
    TaskHandle_t task_handle;

    لكل (صحيح corenum = 0; corenum < CONFIG_SOC_CPU_CORES_انگ; corenum++) {
        corenum_info.recorded_core_انگ = 0xFFFFFFFF;
        corenum_info.والد_handle = xTaskGetCurrentTaskHandle();
        UBaseType_t ڍير_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8_ٻٽو_انگ);
        بنياد_Type_t نتيجو = create_task(task_report_corenum,
                                        ڍير_سائيز,
                                        (فراغ *) & (corenum_info),
                                        corenum,
                                        ڍير_CAPS,
                                        &task_handle);

        TEST_ASSERT_EQUAL(pdPASS, نتيجو);

        // Wait for the created task to finish its job
        ulTaskNotifyTake(pdTRUE, پورٽ_MAX_DELAY);

        TEST_ASSERT_EQUAL((size_t) corenum, corenum_info.recorded_core_انگ);

        vTaskDelete(task_handle);

        // Add a short delay to allow the idle task to free any remaining task memory
        vTaskDelay(10);
    }
}
#جيڪڏهن_پورو // !CONFIG_FREERTOS_UNICORE

#جيڪڏهن_پورو // SPIRAM
