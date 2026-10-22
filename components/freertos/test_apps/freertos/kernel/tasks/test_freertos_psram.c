/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/idf_additions.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_private/freertos_idf_additions_priv.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unity.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_heap_caps.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unity_test_runner.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "memory_checks.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UNITY_FREERTOS_ਤਰਜੀਹ 5

#ਜੇ CONFIG_SPIRAM

ਸਥਿਰ ਖਾਲੀ task_delete_itself(ਖਾਲੀ *arg)
{
    ਲਿਖੋ("starting task\n");
    ulTaskNotifyTake(pdTRUE, ਪੋਰਟ_MAX_DELAY);
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(ਨਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ task_should_never_ਲੜੀਵਾਰ(ਖਾਲੀ *arg)
{
    ਲਿਖੋ("ERROR: this task should not run, aborting\n");
    abort();
}

ਸਥਿਰ ਅਧਾਰ_Type_t create_task(TaskFunction_t function,
                              size_t ਢੇਰ_ਮਾਪ_2,
                              ਖਾਲੀ *task_arg,
                              ਪੂਰਨ core_ਅੰਕ,
                              UBaseType_t ਢੇਰ_caps,
                              TaskHandle_t *task_handle)
{
#ਜੇ CONFIG_FREERTOS_SMP
    UBaseType_t core_affinity_ਨਕਾਬ = (core_ਅੰਕ == -1) ? tskNO_AFFINITY : 1 << core_ਅੰਕ;
    ਵਾਪਸ prv_Task_Create_Dynamic_Affinity_ਸੈੱਟ_With_Caps(function,
                                                   "self_delete",
                                                   ਢੇਰ_ਮਾਪ_2,
                                                   task_arg,
                                                   UNITY_FREERTOS_ਤਰਜੀਹ + 1,
                                                   core_affinity_ਨਕਾਬ,
                                                   ਢੇਰ_caps,
                                                   task_handle);
#ਨਹੀਂਤਾਂ
    ਅਚਲ ਅਧਾਰ_Type_t task_core_ਅੰਕ = (core_ਅੰਕ == -1) ? tskNO_AFFINITY : core_ਅੰਕ;
    ਵਾਪਸ prv_Task_Create_Dynamic_Pinned_ਮੰਜ਼ਿਲ_Core_With_Caps(function,
                                                    "self_delete",
                                                    ਢੇਰ_ਮਾਪ_2,
                                                    task_arg,
                                                    UNITY_FREERTOS_ਤਰਜੀਹ + 1,
                                                    task_core_ਅੰਕ,
                                                    ਢੇਰ_caps,
                                                    task_handle);
#ਜੇ_ਅੰਤ
}

TEST_CASE("Out of memory failure", "[freertos][psram]")
{
    TaskHandle_t task_handle = ਨਿਰਾ;
    ਅਚਲ size_t ਢੇਰ_ਮਾਪ = 0x80000000; // far larger than the virtual address space on ESP32
    UBaseType_t ਢੇਰ_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8_ਦੋਆਧਾਰੀ_ਅੰਕ);

    ਅਧਾਰ_Type_t ਨਤੀਜਾ = create_task(task_should_never_ਲੜੀਵਾਰ,
                                    ਢੇਰ_ਮਾਪ,
                                    (ਖਾਲੀ *)xTaskGetCurrentTaskHandle(),
                                    -1,
                                    ਢੇਰ_CAPS,
                                    &task_handle);
    TEST_ASSERT_EQUAL(err_COULD_NOT_ALLOCATE_REQUIRED_ਸਮ੍ਰਿਤੀ, ਨਤੀਜਾ);
    (ਖਾਲੀ)task_handle;
}

TEST_CASE("Task with stack memory in PSRAM", "[freertos][psram]")
{
    TaskHandle_t task_handle = ਨਿਰਾ;
    ਅਚਲ size_t ਢੇਰ_ਮਾਪ = 0x80000; // value is too large for any internal RAM on current chips
    UBaseType_t ਢੇਰ_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8_ਦੋਆਧਾਰੀ_ਅੰਕ);

    ਅਧਾਰ_Type_t ਨਤੀਜਾ = create_task(task_delete_itself,
                                    ਢੇਰ_ਮਾਪ,
                                    (ਖਾਲੀ *)xTaskGetCurrentTaskHandle(),
                                    -1,
                                    ਢੇਰ_CAPS,
                                    &task_handle);
    TEST_ASSERT_EQUAL(pdPASS, ਨਤੀਜਾ);

    // synchronize with the task to make sure we don't return too early, thus giving it enough time
    // to delete itself and giving the idle task time to clean up memory (see delay in tearDown()).
    xTaskNotifyGive(task_handle);
    ulTaskNotifyTake(pdTRUE, ਪੋਰਟ_MAX_DELAY);
}

#ਜੇ !CONFIG_FREERTOS_UNICORE
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    size_t recorded_core_ਅੰਕ;
    TaskHandle_t ਮਾਪਾ_handle;
} report_corenum_info_t;

ਸਥਿਰ ਖਾਲੀ task_report_corenum(ਖਾਲੀ *arg)
{
    report_corenum_info_t *info = (report_corenum_info_t*) arg;
    info->recorded_core_ਅੰਕ = xTaskGetCoreID(ਨਿਰਾ);
    xTaskNotifyGive(info->ਮਾਪਾ_handle);
    vTaskSuspend(ਨਿਰਾ);
}

/**
 * This test (roughly) verifies that xTaskCreateWithCapsAffinitySet() pins a task to the correct core.
 */
TEST_CASE("Task on specific core works", "[freertos][psram]")
{
    ਅਚਲ size_t ਢੇਰ_ਮਾਪ = 0x1000;
    report_corenum_info_t corenum_info;
    TaskHandle_t task_handle;

    ਲਈ (ਪੂਰਨ corenum = 0; corenum < CONFIG_SOC_CPU_CORES_ਅੰਕ; corenum++) {
        corenum_info.recorded_core_ਅੰਕ = 0xFFFFFFFF;
        corenum_info.ਮਾਪਾ_handle = xTaskGetCurrentTaskHandle();
        UBaseType_t ਢੇਰ_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8_ਦੋਆਧਾਰੀ_ਅੰਕ);
        ਅਧਾਰ_Type_t ਨਤੀਜਾ = create_task(task_report_corenum,
                                        ਢੇਰ_ਮਾਪ,
                                        (ਖਾਲੀ *) & (corenum_info),
                                        corenum,
                                        ਢੇਰ_CAPS,
                                        &task_handle);

        TEST_ASSERT_EQUAL(pdPASS, ਨਤੀਜਾ);

        // Wait for the created task to finish its job
        ulTaskNotifyTake(pdTRUE, ਪੋਰਟ_MAX_DELAY);

        TEST_ASSERT_EQUAL((size_t) corenum, corenum_info.recorded_core_ਅੰਕ);

        vTaskDelete(task_handle);

        // Add a short delay to allow the idle task to free any remaining task memory
        vTaskDelay(10);
    }
}
#ਜੇ_ਅੰਤ // !CONFIG_FREERTOS_UNICORE

#ਜੇ_ਅੰਤ // SPIRAM
