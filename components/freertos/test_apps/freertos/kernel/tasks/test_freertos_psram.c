/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#అన్తర్భావ <stdio.h>
#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ "freertos/idf_additions.h"
#అన్తర్భావ "esp_private/freertos_idf_additions_priv.h"
#అన్తర్భావ "unity.h"
#అన్తర్భావ "esp_heap_caps.h"
#అన్తర్భావ "unity_test_runner.h"
#అన్తర్భావ "memory_checks.h"

#నిర్వచించు UNITY_FREERTOS_ప్రాధాన్యత 5

#ఒకవేళ CONFIG_SPIRAM

స్థిర శూన్య task_delete_itself(శూన్య *arg)
{
    రాయి("starting task\n");
    ulTaskNotifyTake(pdTRUE, పోర్టు_MAX_DELAY);
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(ఖాళీ);
}

స్థిర శూన్య task_should_never_నిరంతరం(శూన్య *arg)
{
    రాయి("ERROR: this task should not run, aborting\n");
    abort();
}

స్థిర పునాది_Type_t create_task(TaskFunction_t function,
                              size_t దొంతర_కొలత_2,
                              శూన్య *task_arg,
                              పూర్ణాంకం core_అంకె,
                              UBaseType_t కుప్ప_caps,
                              TaskHandle_t *task_handle)
{
#ఒకవేళ CONFIG_FREERTOS_SMP
    UBaseType_t core_affinity_ముసుగు = (core_అంకె == -1) ? tskNO_AFFINITY : 1 << core_అంకె;
    తిరిగి prv_Task_Create_Dynamic_Affinity_సమితి_With_Caps(function,
                                                   "self_delete",
                                                   దొంతర_కొలత_2,
                                                   task_arg,
                                                   UNITY_FREERTOS_ప్రాధాన్యత + 1,
                                                   core_affinity_ముసుగు,
                                                   కుప్ప_caps,
                                                   task_handle);
#లేకపోతే
    స్థిరరాశి పునాది_Type_t task_core_అంకె = (core_అంకె == -1) ? tskNO_AFFINITY : core_అంకె;
    తిరిగి prv_Task_Create_Dynamic_Pinned_గమ్యం_Core_With_Caps(function,
                                                    "self_delete",
                                                    దొంతర_కొలత_2,
                                                    task_arg,
                                                    UNITY_FREERTOS_ప్రాధాన్యత + 1,
                                                    task_core_అంకె,
                                                    కుప్ప_caps,
                                                    task_handle);
#అయితే_ముగింపు
}

TEST_CASE("Out of memory failure", "[freertos][psram]")
{
    TaskHandle_t task_handle = ఖాళీ;
    స్థిరరాశి size_t దొంతర_కొలత = 0x80000000; // far larger than the virtual address space on ESP32
    UBaseType_t కుప్ప_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8_బిట్టు);

    పునాది_Type_t ఫలితం = create_task(task_should_never_నిరంతరం,
                                    దొంతర_కొలత,
                                    (శూన్య *)xTaskGetCurrentTaskHandle(),
                                    -1,
                                    కుప్ప_CAPS,
                                    &task_handle);
    TEST_ASSERT_EQUAL(err_COULD_NOT_ALLOCATE_REQUIRED_జ్ఞాపకం, ఫలితం);
    (శూన్య)task_handle;
}

TEST_CASE("Task with stack memory in PSRAM", "[freertos][psram]")
{
    TaskHandle_t task_handle = ఖాళీ;
    స్థిరరాశి size_t దొంతర_కొలత = 0x80000; // value is too large for any internal RAM on current chips
    UBaseType_t కుప్ప_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8_బిట్టు);

    పునాది_Type_t ఫలితం = create_task(task_delete_itself,
                                    దొంతర_కొలత,
                                    (శూన్య *)xTaskGetCurrentTaskHandle(),
                                    -1,
                                    కుప్ప_CAPS,
                                    &task_handle);
    TEST_ASSERT_EQUAL(pdPASS, ఫలితం);

    // synchronize with the task to make sure we don't return too early, thus giving it enough time
    // to delete itself and giving the idle task time to clean up memory (see delay in tearDown()).
    xTaskNotifyGive(task_handle);
    ulTaskNotifyTake(pdTRUE, పోర్టు_MAX_DELAY);
}

#ఒకవేళ !CONFIG_FREERTOS_UNICORE
రకనిర్వచనం నిర్మాణం {
    size_t recorded_core_అంకె;
    TaskHandle_t మాతృ_handle;
} report_corenum_info_t;

స్థిర శూన్య task_report_corenum(శూన్య *arg)
{
    report_corenum_info_t *info = (report_corenum_info_t*) arg;
    info->recorded_core_అంకె = xTaskGetCoreID(ఖాళీ);
    xTaskNotifyGive(info->మాతృ_handle);
    vTaskSuspend(ఖాళీ);
}

/**
 * This test (roughly) verifies that xTaskCreateWithCapsAffinitySet() pins a task to the correct core.
 */
TEST_CASE("Task on specific core works", "[freertos][psram]")
{
    స్థిరరాశి size_t దొంతర_కొలత = 0x1000;
    report_corenum_info_t corenum_info;
    TaskHandle_t task_handle;

    కొరకు (పూర్ణాంకం corenum = 0; corenum < CONFIG_SOC_CPU_CORES_అంకె; corenum++) {
        corenum_info.recorded_core_అంకె = 0xFFFFFFFF;
        corenum_info.మాతృ_handle = xTaskGetCurrentTaskHandle();
        UBaseType_t కుప్ప_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8_బిట్టు);
        పునాది_Type_t ఫలితం = create_task(task_report_corenum,
                                        దొంతర_కొలత,
                                        (శూన్య *) & (corenum_info),
                                        corenum,
                                        కుప్ప_CAPS,
                                        &task_handle);

        TEST_ASSERT_EQUAL(pdPASS, ఫలితం);

        // Wait for the created task to finish its job
        ulTaskNotifyTake(pdTRUE, పోర్టు_MAX_DELAY);

        TEST_ASSERT_EQUAL((size_t) corenum, corenum_info.recorded_core_అంకె);

        vTaskDelete(task_handle);

        // Add a short delay to allow the idle task to free any remaining task memory
        vTaskDelay(10);
    }
}
#అయితే_ముగింపు // !CONFIG_FREERTOS_UNICORE

#అయితే_ముగింపు // SPIRAM
