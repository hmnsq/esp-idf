/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ "freertos/FreeRTOS.h"
#અન્તર્ભાવ "freertos/task.h"
#અન્તર્ભાવ "freertos/idf_additions.h"
#અન્તર્ભાવ "esp_private/freertos_idf_additions_priv.h"
#અન્તર્ભાવ "unity.h"
#અન્તર્ભાવ "esp_heap_caps.h"
#અન્તર્ભાવ "unity_test_runner.h"
#અન્તર્ભાવ "memory_checks.h"

#define UNITY_FREERTOS_PRIORITY 5

#જો CONFIG_SPIRAM

સ્થિર શૂન્ય task_delete_itself(શૂન્ય *arg)
{
    printf("starting task\n");
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(ખાલી);
}

સ્થિર શૂન્ય task_should_never_run(શૂન્ય *arg)
{
    printf("ERROR: this task should not run, aborting\n");
    abort();
}

સ્થિર BaseType_t create_task(TaskFunction_t function,
                              size_t ઢગલો_size,
                              શૂન્ય *task_arg,
                              પૂર્ણાંક core_અંક,
                              UBaseType_t heap_caps,
                              TaskHandle_t *task_handle)
{
#જો CONFIG_FREERTOS_SMP
    UBaseType_t core_affinity_mask = (core_અંક == -1) ? tskNO_AFFINITY : 1 << core_અંક;
    પાછા prvTaskCreateDynamicAffinitySetWithCaps(function,
                                                   "self_delete",
                                                   ઢગલો_size,
                                                   task_arg,
                                                   UNITY_FREERTOS_PRIORITY + 1,
                                                   core_affinity_mask,
                                                   heap_caps,
                                                   task_handle);
#નહીંતર
    અચલ BaseType_t task_core_અંક = (core_અંક == -1) ? tskNO_AFFINITY : core_અંક;
    પાછા prvTaskCreateDynamicPinnedToCoreWithCaps(function,
                                                    "self_delete",
                                                    ઢગલો_size,
                                                    task_arg,
                                                    UNITY_FREERTOS_PRIORITY + 1,
                                                    task_core_અંક,
                                                    heap_caps,
                                                    task_handle);
#endif
}

TEST_CASE("Out of memory failure", "[freertos][psram]")
{
    TaskHandle_t task_handle = ખાલી;
    અચલ size_t ઢગલો_SIZE = 0x80000000; // far larger than the virtual address space on ESP32
    UBaseType_t HEAP_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

    BaseType_t result = create_task(task_should_never_run,
                                    ઢગલો_SIZE,
                                    (શૂન્ય *)xTaskGetCurrentTaskHandle(),
                                    -1,
                                    HEAP_CAPS,
                                    &task_handle);
    TEST_ASSERT_EQUAL(errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY, result);
    (શૂન્ય)task_handle;
}

TEST_CASE("Task with stack memory in PSRAM", "[freertos][psram]")
{
    TaskHandle_t task_handle = ખાલી;
    અચલ size_t ઢગલો_SIZE = 0x80000; // value is too large for any internal RAM on current chips
    UBaseType_t HEAP_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

    BaseType_t result = create_task(task_delete_itself,
                                    ઢગલો_SIZE,
                                    (શૂન્ય *)xTaskGetCurrentTaskHandle(),
                                    -1,
                                    HEAP_CAPS,
                                    &task_handle);
    TEST_ASSERT_EQUAL(pdPASS, result);

    // synchronize with the task to make sure we don't return too early, thus giving it enough time
    // to delete itself and giving the idle task time to clean up memory (see delay in tearDown()).
    xTaskNotifyGive(task_handle);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
}

#જો !CONFIG_FREERTOS_UNICORE
પ્રકારવ્યાખ્યા સંરચના {
    size_t recorded_core_અંક;
    TaskHandle_t parent_handle;
} report_corenum_info_t;

સ્થિર શૂન્ય task_report_corenum(શૂન્ય *arg)
{
    report_corenum_info_t *info = (report_corenum_info_t*) arg;
    info->recorded_core_અંક = xTaskGetCoreID(ખાલી);
    xTaskNotifyGive(info->parent_handle);
    vTaskSuspend(ખાલી);
}

/**
 * This test (roughly) verifies that xTaskCreateWithCapsAffinitySet() pins a task to the correct core.
 */
TEST_CASE("Task on specific core works", "[freertos][psram]")
{
    અચલ size_t ઢગલો_SIZE = 0x1000;
    report_corenum_info_t corenum_info;
    TaskHandle_t task_handle;

    માટે (પૂર્ણાંક corenum = 0; corenum < CONFIG_SOC_CPU_CORES_અંક; corenum++) {
        corenum_info.recorded_core_અંક = 0xFFFFFFFF;
        corenum_info.parent_handle = xTaskGetCurrentTaskHandle();
        UBaseType_t HEAP_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
        BaseType_t result = create_task(task_report_corenum,
                                        ઢગલો_SIZE,
                                        (શૂન્ય *) & (corenum_info),
                                        corenum,
                                        HEAP_CAPS,
                                        &task_handle);

        TEST_ASSERT_EQUAL(pdPASS, result);

        // Wait for the created task to finish its job
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        TEST_ASSERT_EQUAL((size_t) corenum, corenum_info.recorded_core_અંક);

        vTaskDelete(task_handle);

        // Add a short delay to allow the idle task to free any remaining task memory
        vTaskDelay(10);
    }
}
#endif // !CONFIG_FREERTOS_UNICORE

#endif // SPIRAM
