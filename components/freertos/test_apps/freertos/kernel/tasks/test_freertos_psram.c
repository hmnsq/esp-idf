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

#تعریف_کرو UNITY_FREERTOS_ترجیح 5

#إذا CONFIG_SPIRAM

ساكن فراغ task_delete_itself(فراغ *arg)
{
    لکھو("starting task\n");
    ulTaskNotifyTake(pdTRUE, پورٹ_MAX_DELAY);
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(عدم);
}

ساكن فراغ task_should_never_لگاتار(فراغ *arg)
{
    لکھو("ERROR: this task should not run, aborting\n");
    abort();
}

ساكن ادھار_Type_t create_task(TaskFunction_t function,
                              size_t ڈھیر_ناپ_2,
                              فراغ *task_arg,
                              صحيح core_عدد,
                              UBaseType_t ڈھیر_caps,
                              TaskHandle_t *task_handle)
{
#إذا CONFIG_FREERTOS_SMP
    UBaseType_t core_affinity_نقاب = (core_عدد == -1) ? tskNO_AFFINITY : 1 << core_عدد;
    أرجع prv_Task_Create_Dynamic_Affinity_سیٹ_With_Caps(function,
                                                   "self_delete",
                                                   ڈھیر_ناپ_2,
                                                   task_arg,
                                                   UNITY_FREERTOS_ترجیح + 1,
                                                   core_affinity_نقاب,
                                                   ڈھیر_caps,
                                                   task_handle);
#وإلا
    ثابت ادھار_Type_t task_core_عدد = (core_عدد == -1) ? tskNO_AFFINITY : core_عدد;
    أرجع prv_Task_Create_Dynamic_Pinned_منزل_Core_With_Caps(function,
                                                    "self_delete",
                                                    ڈھیر_ناپ_2,
                                                    task_arg,
                                                    UNITY_FREERTOS_ترجیح + 1,
                                                    task_core_عدد,
                                                    ڈھیر_caps,
                                                    task_handle);
#جے_انت
}

TEST_CASE("Out of memory failure", "[freertos][psram]")
{
    TaskHandle_t task_handle = عدم;
    ثابت size_t ڈھیر_ناپ = 0x80000000; // far larger than the virtual address space on ESP32
    UBaseType_t ڈھیر_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8_ثنائی_ہندسہ);

    ادھار_Type_t نتیجہ = create_task(task_should_never_لگاتار,
                                    ڈھیر_ناپ,
                                    (فراغ *)xTaskGetCurrentTaskHandle(),
                                    -1,
                                    ڈھیر_CAPS,
                                    &task_handle);
    TEST_ASSERT_EQUAL(err_COULD_NOT_ALLOCATE_REQUIRED_حافظہ, نتیجہ);
    (فراغ)task_handle;
}

TEST_CASE("Task with stack memory in PSRAM", "[freertos][psram]")
{
    TaskHandle_t task_handle = عدم;
    ثابت size_t ڈھیر_ناپ = 0x80000; // value is too large for any internal RAM on current chips
    UBaseType_t ڈھیر_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8_ثنائی_ہندسہ);

    ادھار_Type_t نتیجہ = create_task(task_delete_itself,
                                    ڈھیر_ناپ,
                                    (فراغ *)xTaskGetCurrentTaskHandle(),
                                    -1,
                                    ڈھیر_CAPS,
                                    &task_handle);
    TEST_ASSERT_EQUAL(pdPASS, نتیجہ);

    // synchronize with the task to make sure we don't return too early, thus giving it enough time
    // to delete itself and giving the idle task time to clean up memory (see delay in tearDown()).
    xTaskNotifyGive(task_handle);
    ulTaskNotifyTake(pdTRUE, پورٹ_MAX_DELAY);
}

#إذا !CONFIG_FREERTOS_UNICORE
تعريفنوع بنية {
    size_t recorded_core_عدد;
    TaskHandle_t ماپا_handle;
} report_corenum_info_t;

ساكن فراغ task_report_corenum(فراغ *arg)
{
    report_corenum_info_t *info = (report_corenum_info_t*) arg;
    info->recorded_core_عدد = xTaskGetCoreID(عدم);
    xTaskNotifyGive(info->ماپا_handle);
    vTaskSuspend(عدم);
}

/**
 * This test (roughly) verifies that xTaskCreateWithCapsAffinitySet() pins a task to the correct core.
 */
TEST_CASE("Task on specific core works", "[freertos][psram]")
{
    ثابت size_t ڈھیر_ناپ = 0x1000;
    report_corenum_info_t corenum_info;
    TaskHandle_t task_handle;

    لكل (صحيح corenum = 0; corenum < CONFIG_SOC_CPU_CORES_عدد; corenum++) {
        corenum_info.recorded_core_عدد = 0xFFFFFFFF;
        corenum_info.ماپا_handle = xTaskGetCurrentTaskHandle();
        UBaseType_t ڈھیر_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8_ثنائی_ہندسہ);
        ادھار_Type_t نتیجہ = create_task(task_report_corenum,
                                        ڈھیر_ناپ,
                                        (فراغ *) & (corenum_info),
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
#جے_انت // !CONFIG_FREERTOS_UNICORE

#جے_انت // SPIRAM
