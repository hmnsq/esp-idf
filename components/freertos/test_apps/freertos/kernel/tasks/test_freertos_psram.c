/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ "freertos/FreeRTOS.h"
#ଅନ୍ତର୍ଭାଵ "freertos/task.h"
#ଅନ୍ତର୍ଭାଵ "freertos/idf_additions.h"
#ଅନ୍ତର୍ଭାଵ "esp_private/freertos_idf_additions_priv.h"
#ଅନ୍ତର୍ଭାଵ "unity.h"
#ଅନ୍ତର୍ଭାଵ "esp_heap_caps.h"
#ଅନ୍ତର୍ଭାଵ "unity_test_runner.h"
#ଅନ୍ତର୍ଭାଵ "memory_checks.h"

#define UNITY_FREERTOS_PRIORITY 5

#ଯଦି CONFIG_SPIRAM

ସ୍ଥିର ଶୂନ୍ୟ task_delete_itself(ଶୂନ୍ୟ *arg)
{
    printf("starting task\n");
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(ଖାଲି);
}

ସ୍ଥିର ଶୂନ୍ୟ task_should_never_run(ଶୂନ୍ୟ *arg)
{
    printf("ERROR: this task should not run, aborting\n");
    abort();
}

ସ୍ଥିର BaseType_t create_task(TaskFunction_t function,
                              size_t ଗଦା_size,
                              ଶୂନ୍ୟ *task_arg,
                              ପୂର୍ଣ୍ଣସଂଖ୍ୟା core_ଅଙ୍କ,
                              UBaseType_t heap_caps,
                              TaskHandle_t *task_handle)
{
#ଯଦି CONFIG_FREERTOS_SMP
    UBaseType_t core_affinity_mask = (core_ଅଙ୍କ == -1) ? tskNO_AFFINITY : 1 << core_ଅଙ୍କ;
    ଫେରନ୍ତୁ prvTaskCreateDynamicAffinitySetWithCaps(function,
                                                   "self_delete",
                                                   ଗଦା_size,
                                                   task_arg,
                                                   UNITY_FREERTOS_PRIORITY + 1,
                                                   core_affinity_mask,
                                                   heap_caps,
                                                   task_handle);
#ଅନ୍ୟଥା
    ଅଚଳ BaseType_t task_core_ଅଙ୍କ = (core_ଅଙ୍କ == -1) ? tskNO_AFFINITY : core_ଅଙ୍କ;
    ଫେରନ୍ତୁ prvTaskCreateDynamicPinnedToCoreWithCaps(function,
                                                    "self_delete",
                                                    ଗଦା_size,
                                                    task_arg,
                                                    UNITY_FREERTOS_PRIORITY + 1,
                                                    task_core_ଅଙ୍କ,
                                                    heap_caps,
                                                    task_handle);
#endif
}

TEST_CASE("Out of memory failure", "[freertos][psram]")
{
    TaskHandle_t task_handle = ଖାଲି;
    ଅଚଳ size_t ଗଦା_SIZE = 0x80000000; // far larger than the virtual address space on ESP32
    UBaseType_t HEAP_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

    BaseType_t result = create_task(task_should_never_run,
                                    ଗଦା_SIZE,
                                    (ଶୂନ୍ୟ *)xTaskGetCurrentTaskHandle(),
                                    -1,
                                    HEAP_CAPS,
                                    &task_handle);
    TEST_ASSERT_EQUAL(errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY, result);
    (ଶୂନ୍ୟ)task_handle;
}

TEST_CASE("Task with stack memory in PSRAM", "[freertos][psram]")
{
    TaskHandle_t task_handle = ଖାଲି;
    ଅଚଳ size_t ଗଦା_SIZE = 0x80000; // value is too large for any internal RAM on current chips
    UBaseType_t HEAP_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

    BaseType_t result = create_task(task_delete_itself,
                                    ଗଦା_SIZE,
                                    (ଶୂନ୍ୟ *)xTaskGetCurrentTaskHandle(),
                                    -1,
                                    HEAP_CAPS,
                                    &task_handle);
    TEST_ASSERT_EQUAL(pdPASS, result);

    // synchronize with the task to make sure we don't return too early, thus giving it enough time
    // to delete itself and giving the idle task time to clean up memory (see delay in tearDown()).
    xTaskNotifyGive(task_handle);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
}

#ଯଦି !CONFIG_FREERTOS_UNICORE
ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    size_t recorded_core_ଅଙ୍କ;
    TaskHandle_t parent_handle;
} report_corenum_info_t;

ସ୍ଥିର ଶୂନ୍ୟ task_report_corenum(ଶୂନ୍ୟ *arg)
{
    report_corenum_info_t *info = (report_corenum_info_t*) arg;
    info->recorded_core_ଅଙ୍କ = xTaskGetCoreID(ଖାଲି);
    xTaskNotifyGive(info->parent_handle);
    vTaskSuspend(ଖାଲି);
}

/**
 * This test (roughly) verifies that xTaskCreateWithCapsAffinitySet() pins a task to the correct core.
 */
TEST_CASE("Task on specific core works", "[freertos][psram]")
{
    ଅଚଳ size_t ଗଦା_SIZE = 0x1000;
    report_corenum_info_t corenum_info;
    TaskHandle_t task_handle;

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା corenum = 0; corenum < CONFIG_SOC_CPU_CORES_ଅଙ୍କ; corenum++) {
        corenum_info.recorded_core_ଅଙ୍କ = 0xFFFFFFFF;
        corenum_info.parent_handle = xTaskGetCurrentTaskHandle();
        UBaseType_t HEAP_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
        BaseType_t result = create_task(task_report_corenum,
                                        ଗଦା_SIZE,
                                        (ଶୂନ୍ୟ *) & (corenum_info),
                                        corenum,
                                        HEAP_CAPS,
                                        &task_handle);

        TEST_ASSERT_EQUAL(pdPASS, result);

        // Wait for the created task to finish its job
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        TEST_ASSERT_EQUAL((size_t) corenum, corenum_info.recorded_core_ଅଙ୍କ);

        vTaskDelete(task_handle);

        // Add a short delay to allow the idle task to free any remaining task memory
        vTaskDelay(10);
    }
}
#endif // !CONFIG_FREERTOS_UNICORE

#endif // SPIRAM
