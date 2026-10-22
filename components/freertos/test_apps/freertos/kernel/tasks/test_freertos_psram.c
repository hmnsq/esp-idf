/*
 * SPDX-FileCopyrightText: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ "freertos/FreeRTOS.h"
#അന്തര്ഭാവ "freertos/task.h"
#അന്തര്ഭാവ "freertos/idf_additions.h"
#അന്തര്ഭാവ "esp_private/freertos_idf_additions_priv.h"
#അന്തര്ഭാവ "unity.h"
#അന്തര്ഭാവ "esp_heap_caps.h"
#അന്തര്ഭാവ "unity_test_runner.h"
#അന്തര്ഭാവ "memory_checks.h"

#define UNITY_FREERTOS_PRIORITY 5

#എങ്കിൽ CONFIG_SPIRAM

സ്ഥിതം ശൂന്യം task_delete_itself(ശൂന്യം *arg)
{
    printf("starting task\n");
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskDelete(ശൂന്യ);
}

സ്ഥിതം ശൂന്യം task_should_never_run(ശൂന്യം *arg)
{
    printf("ERROR: this task should not run, aborting\n");
    abort();
}

സ്ഥിതം BaseType_t create_task(TaskFunction_t function,
                              size_t അട്ടി_size,
                              ശൂന്യം *task_arg,
                              പൂർണ്ണസംഖ്യ core_അക്കം,
                              UBaseType_t heap_caps,
                              TaskHandle_t *task_handle)
{
#എങ്കിൽ CONFIG_FREERTOS_SMP
    UBaseType_t core_affinity_mask = (core_അക്കം == -1) ? tskNO_AFFINITY : 1 << core_അക്കം;
    തിരികെ prvTaskCreateDynamicAffinitySetWithCaps(function,
                                                   "self_delete",
                                                   അട്ടി_size,
                                                   task_arg,
                                                   UNITY_FREERTOS_PRIORITY + 1,
                                                   core_affinity_mask,
                                                   heap_caps,
                                                   task_handle);
#അല്ലെങ്കിൽ
    സ്ഥിരം BaseType_t task_core_അക്കം = (core_അക്കം == -1) ? tskNO_AFFINITY : core_അക്കം;
    തിരികെ prvTaskCreateDynamicPinnedToCoreWithCaps(function,
                                                    "self_delete",
                                                    അട്ടി_size,
                                                    task_arg,
                                                    UNITY_FREERTOS_PRIORITY + 1,
                                                    task_core_അക്കം,
                                                    heap_caps,
                                                    task_handle);
#endif
}

TEST_CASE("Out of memory failure", "[freertos][psram]")
{
    TaskHandle_t task_handle = ശൂന്യ;
    സ്ഥിരം size_t അട്ടി_SIZE = 0x80000000; // far larger than the virtual address space on ESP32
    UBaseType_t HEAP_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

    BaseType_t result = create_task(task_should_never_run,
                                    അട്ടി_SIZE,
                                    (ശൂന്യം *)xTaskGetCurrentTaskHandle(),
                                    -1,
                                    HEAP_CAPS,
                                    &task_handle);
    TEST_ASSERT_EQUAL(errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY, result);
    (ശൂന്യം)task_handle;
}

TEST_CASE("Task with stack memory in PSRAM", "[freertos][psram]")
{
    TaskHandle_t task_handle = ശൂന്യ;
    സ്ഥിരം size_t അട്ടി_SIZE = 0x80000; // value is too large for any internal RAM on current chips
    UBaseType_t HEAP_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);

    BaseType_t result = create_task(task_delete_itself,
                                    അട്ടി_SIZE,
                                    (ശൂന്യം *)xTaskGetCurrentTaskHandle(),
                                    -1,
                                    HEAP_CAPS,
                                    &task_handle);
    TEST_ASSERT_EQUAL(pdPASS, result);

    // synchronize with the task to make sure we don't return too early, thus giving it enough time
    // to delete itself and giving the idle task time to clean up memory (see delay in tearDown()).
    xTaskNotifyGive(task_handle);
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
}

#എങ്കിൽ !CONFIG_FREERTOS_UNICORE
തരനിർവചനം ഘടന {
    size_t recorded_core_അക്കം;
    TaskHandle_t parent_handle;
} report_corenum_info_t;

സ്ഥിതം ശൂന്യം task_report_corenum(ശൂന്യം *arg)
{
    report_corenum_info_t *info = (report_corenum_info_t*) arg;
    info->recorded_core_അക്കം = xTaskGetCoreID(ശൂന്യ);
    xTaskNotifyGive(info->parent_handle);
    vTaskSuspend(ശൂന്യ);
}

/**
 * This test (roughly) verifies that xTaskCreateWithCapsAffinitySet() pins a task to the correct core.
 */
TEST_CASE("Task on specific core works", "[freertos][psram]")
{
    സ്ഥിരം size_t അട്ടി_SIZE = 0x1000;
    report_corenum_info_t corenum_info;
    TaskHandle_t task_handle;

    വേണ്ടി (പൂർണ്ണസംഖ്യ corenum = 0; corenum < CONFIG_SOC_CPU_CORES_അക്കം; corenum++) {
        corenum_info.recorded_core_അക്കം = 0xFFFFFFFF;
        corenum_info.parent_handle = xTaskGetCurrentTaskHandle();
        UBaseType_t HEAP_CAPS = (MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
        BaseType_t result = create_task(task_report_corenum,
                                        അട്ടി_SIZE,
                                        (ശൂന്യം *) & (corenum_info),
                                        corenum,
                                        HEAP_CAPS,
                                        &task_handle);

        TEST_ASSERT_EQUAL(pdPASS, result);

        // Wait for the created task to finish its job
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        TEST_ASSERT_EQUAL((size_t) corenum, corenum_info.recorded_core_അക്കം);

        vTaskDelete(task_handle);

        // Add a short delay to allow the idle task to free any remaining task memory
        vTaskDelay(10);
    }
}
#endif // !CONFIG_FREERTOS_UNICORE

#endif // SPIRAM
