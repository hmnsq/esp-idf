/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن "sdkconfig.h"
#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن "freertos/queue.h"
#ضمن "freertos/semphr.h"
#ضمن "freertos/stream_buffer.h"
#ضمن "freertos/message_buffer.h"
#ضمن "freertos/event_groups.h"
#ضمن "freertos/idf_additions.h"
#ضمن "freertos/freertos_debug.h"
#ضمن "esp_memory_utils.h"
#ضمن "esp_heap_caps.h"
#ضمن "unity.h"
#ضمن "test_utils.h"
#ضمن <stdio.h>
#ضمن <string.h>
#ضمن <inttypes.h>

/*
Test ESP-IDF FreeRTOS API additions from idf_additions.h:
    - ...Create...WithCaps() / ...DeleteWithCaps() memory capability helpers
    - xTaskCreatePinnedToCore() / xTaskCreateStaticPinnedToCore() pinning semantics
    - Task utility helpers (xTaskGetCoreID, xTaskGetStackStart, per-core handle queries, etc.)

Run only these cases from the Unity menu with the [idf_additions] tag filter.
 */

#وصف_ڪريو OBJECT_يادگيري_CAPS      (MALLOC_CAP_اندروني|MALLOC_CAP_8_ٻٽو_انگ)
#وصف_ڪريو SPIRAM_OBJECT_يادگيري_CAPS (MALLOC_CAP_SPIRAM|MALLOC_CAP_8_ٻٽو_انگ)

ساكن فراغ task_with_caps(فراغ *arg)
{
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskSuspend(عدم);
}

ساكن فراغ task_with_caps_self_delete(فراغ *arg)
{
    /* Wait for the unity task to indicate that this task should delete itself */
    ulTaskNotifyTake(pdTRUE, پورٽ_MAX_DELAY);

    /* Although it is not recommended to self-delete a task with memory caps but this
     * is done intentionally to test for memory leaks */
    vTaskDeleteWithCaps(عدم);
}

TEST_CASE("IDF additions: Task creation with memory caps and deletion from another task", "[freertos][idf_additions]")
{
    TaskHandle_t task_handle = عدم;
    ڍير_Type_t *pux_ڍير_بفر;
    StaticTask_t *px_Task_بفر;

    // Create a task with caps
    TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_منزل_Core_With_Caps(task_with_caps, "task", 4096, (فراغ *)xTaskGetCurrentTaskHandle(), UNITY_FREERTOS_ترجيح + 1, &task_handle, UNITY_FREERTOS_CPU, OBJECT_يادگيري_CAPS));
    TEST_ASSERT_NOT_EQUAL(عدم, task_handle);
    // Get the task's memory
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetStaticBuffers(task_handle, &pux_ڍير_بفر, &px_Task_بفر));
    TEST_ASSERT(esp_ptr_اندريون_dram(pux_ڍير_بفر));
    TEST_ASSERT(esp_ptr_اندريون_dram(px_Task_بفر));
    // Wait for the created task to block
    ulTaskNotifyTake(pdTRUE, پورٽ_MAX_DELAY);
    // Delete the task
    vTaskDeleteWithCaps(task_handle);
}

TEST_CASE("IDF additions: Task creation with memory caps and self deletion", "[freertos][idf_additions]")
{
    TaskHandle_t task_handle = عدم;
    ڍير_Type_t *pux_ڍير_بفر;
    StaticTask_t *px_Task_بفر;

    // Create a task with caps
    TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_منزل_Core_With_Caps(task_with_caps_self_delete, "task", 4096, (فراغ *)xTaskGetCurrentTaskHandle(), UNITY_FREERTOS_ترجيح + 1, &task_handle, UNITY_FREERTOS_CPU, OBJECT_يادگيري_CAPS));
    TEST_ASSERT_NOT_EQUAL(عدم, task_handle);
    // Get the task's memory
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetStaticBuffers(task_handle, &pux_ڍير_بفر, &px_Task_بفر));
    TEST_ASSERT(esp_ptr_اندريون_dram(pux_ڍير_بفر));
    TEST_ASSERT(esp_ptr_اندريون_dram(px_Task_بفر));
    // Notify the task to delete itself
    xTaskNotifyGive(task_handle);
}

#إذا CONFIG_FREERTOS_TASK_CREATE_ALLOW_EXT_MEM

TEST_CASE("IDF additions: Task creation with SPIRAM memory caps and self deletion stress test", "[freertos][idf_additions]")
{
#وصف_ڪريو TEST_انگ_TASKS      5
#وصف_ڪريو TEST_انگ_ITERATIONS 1000
    TaskHandle_t task_handle[TEST_انگ_TASKS];
    ڍير_Type_t *pux_ڍير_بفر;
    StaticTask_t *px_Task_بفر;

    لكل (صحيح j = 0; j < TEST_انگ_ITERATIONS; j++) {
        لكل (صحيح i = 0; i < TEST_انگ_TASKS; i++) {
            // Create a task with caps
            TEST_ASSERT_EQUAL(pdPASS, xTaskCreateWithCaps(task_with_caps_self_delete, "task", 4096, عدم, UNITY_FREERTOS_ترجيح, &task_handle[i], MALLOC_CAP_SPIRAM | MALLOC_CAP_8_ٻٽو_انگ));
            // Get the task's memory
            TEST_ASSERT_EQUAL(pdTRUE, xTaskGetStaticBuffers(task_handle[i], &pux_ڍير_بفر, &px_Task_بفر));
            TEST_ASSERT(esp_ptr_external_ram(pux_ڍير_بفر));
            TEST_ASSERT(esp_ptr_اندريون_dram(px_Task_بفر));
        }

        لكل (صحيح i = 0; i < TEST_انگ_TASKS; i++) {
            // Notify the task to delete itself
            xTaskNotifyGive(task_handle[i]);
        }
        // Let the scheduler run the self-delete tasks.
        vTaskDelay(1);
    }
    // Allow the last batch to be freed by the cleanup task in vTaskDeleteWithCaps().
    vTaskDelay(50);
}

#جيڪڏهن_پورو /* CONFIG_FREERTOS_TASK_CREATE_ALLOW_EXT_MEM */

#إذا ( CONFIG_FREERTOS_عدد_جو_CORES > 1 )

ساكن فراغ task_with_caps_running_on_other_core(فراغ *arg)
{
    /* Notify the unity task that this task is running on the other core */
    xTaskNotifyGive((TaskHandle_t)arg);

    /* We make sure that this task is running on the other core */
    طالما (1) {
        ;
    }
}

TEST_CASE("IDF additions: Task creation with memory caps and deletion from another core", "[freertos][idf_additions]")
{
    TaskHandle_t task_handle = عدم;
    ڍير_Type_t *pux_ڍير_بفر;
    StaticTask_t *px_Task_بفر;

    // Create a task with caps on the other core
    TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_منزل_Core_With_Caps(task_with_caps_running_on_other_core, "task", 4096, (فراغ *)xTaskGetCurrentTaskHandle(), UNITY_FREERTOS_ترجيح + 1, &task_handle, !UNITY_FREERTOS_CPU, OBJECT_يادگيري_CAPS));
    TEST_ASSERT_NOT_EQUAL(عدم, task_handle);
    // Get the task's memory
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetStaticBuffers(task_handle, &pux_ڍير_بفر, &px_Task_بفر));
    TEST_ASSERT(esp_ptr_اندريون_dram(pux_ڍير_بفر));
    TEST_ASSERT(esp_ptr_اندريون_dram(px_Task_بفر));
    // Wait for the created task to start running
    ulTaskNotifyTake(pdTRUE, پورٽ_MAX_DELAY);
    // Delete the task from another core
    vTaskDeleteWithCaps(task_handle);
}

#جيڪڏهن_پورو // CONFIG_FREERTOS_NUMBER_OF_CORES > 1

TEST_CASE("IDF additions: Queue creation with memory caps", "[freertos][idf_additions]")
{
    قطار_Handle_t قطار_handle;
    uint8_t *قطار_storage;
    Static_قطار_t *قطار_obj;

    // Create queue with caps
    قطار_handle = x_قطار_Create_With_Caps(5, حجم(uint32_t), OBJECT_يادگيري_CAPS);
    TEST_ASSERT_NOT_EQUAL(عدم, قطار_handle);
    // Get the queue's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_قطار_Get_Static_Buffers(قطار_handle, &قطار_storage, &قطار_obj));
    TEST_ASSERT(esp_ptr_اندريون_dram(قطار_storage));
    TEST_ASSERT(esp_ptr_اندريون_dram(قطار_obj));
    // Free the queue
    v_قطار_Delete_With_Caps(قطار_handle);
}

TEST_CASE("IDF additions: Semaphore creation with memory caps", "[freertos][idf_additions]")
{
    SemaphoreHandle_t sem_handle;
    StaticSemaphore_t *sem_obj;

    // Create binary semaphore with caps
    sem_handle = x_Semaphore_Create_ٻٽو_With_Caps(OBJECT_يادگيري_CAPS);
    TEST_ASSERT_NOT_EQUAL(عدم, sem_handle);
    // Get the binary semaphore's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_Semaphore_Get_Static_بفر(sem_handle, &sem_obj));
    TEST_ASSERT(esp_ptr_اندريون_dram(sem_obj));
    // Free the binary semaphore
    vSemaphoreDeleteWithCaps(sem_handle);

    // Create counting semaphore with caps
    sem_handle = xSemaphoreCreateCountingWithCaps(4, 0, OBJECT_يادگيري_CAPS);
    TEST_ASSERT_NOT_EQUAL(عدم, sem_handle);
    // Get the counting semaphore's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_Semaphore_Get_Static_بفر(sem_handle, &sem_obj));
    TEST_ASSERT(esp_ptr_اندريون_dram(sem_obj));
    // Free the counting semaphore
    vSemaphoreDeleteWithCaps(sem_handle);

    // Create mutex semaphore with caps
    sem_handle = xSemaphoreCreateMutexWithCaps(OBJECT_يادگيري_CAPS);
    TEST_ASSERT_NOT_EQUAL(عدم, sem_handle);
    // Get the mutex semaphore's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_Semaphore_Get_Static_بفر(sem_handle, &sem_obj));
    TEST_ASSERT(esp_ptr_اندريون_dram(sem_obj));
    // Free the mutex semaphore
    vSemaphoreDeleteWithCaps(sem_handle);

    // Create recursive mutex with caps
    sem_handle = xSemaphoreCreateRecursiveMutexWithCaps(OBJECT_يادگيري_CAPS);
    TEST_ASSERT_NOT_EQUAL(عدم, sem_handle);
    // Get the recursive mutex's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_Semaphore_Get_Static_بفر(sem_handle, &sem_obj));
    TEST_ASSERT(esp_ptr_اندريون_dram(sem_obj));
    // Free the recursive mutex
    vSemaphoreDeleteWithCaps(sem_handle);
}

TEST_CASE("IDF additions: Stream & message buffer creation with memory caps", "[freertos][idf_additions]")
{
    وهڪرو_بفر_Handle_t وهڪرو_بفر_handle;
    uint8_t *وهڪرو_بفر_storage;
    Static_وهڪرو_بفر_t *وهڪرو_بفر_obj;

    // Create stream buffer with caps
    وهڪرو_بفر_handle = x_وهڪرو_بفر_Create_With_Caps(16, 0, OBJECT_يادگيري_CAPS);
    TEST_ASSERT_NOT_EQUAL(عدم, وهڪرو_بفر_handle);
    // Get the stream buffer's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_وهڪرو_بفر_Get_Static_Buffers(وهڪرو_بفر_handle, &وهڪرو_بفر_storage, &وهڪرو_بفر_obj));
    TEST_ASSERT(esp_ptr_اندريون_dram(وهڪرو_بفر_storage));
    TEST_ASSERT(esp_ptr_اندريون_dram(وهڪرو_بفر_obj));
    // Free the stream buffer
    v_وهڪرو_بفر_Delete_With_Caps(وهڪرو_بفر_handle);

    پيغام_بفر_Handle_t نياپو_بفر_handle;
    uint8_t *نياپو_بفر_storage;
    Static_پيغام_بفر_t *نياپو_بفر_obj;

    // Create message buffer with caps
    نياپو_بفر_handle = x_پيغام_بفر_Create_With_Caps(16, OBJECT_يادگيري_CAPS);
    TEST_ASSERT_NOT_EQUAL(عدم, نياپو_بفر_handle);
    // Get the message buffer's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_پيغام_بفر_Get_Static_Buffers(نياپو_بفر_handle, &نياپو_بفر_storage, &نياپو_بفر_obj));
    TEST_ASSERT(esp_ptr_اندريون_dram(نياپو_بفر_storage));
    TEST_ASSERT(esp_ptr_اندريون_dram(نياپو_بفر_obj));
    // Free the message buffer
    v_پيغام_بفر_Delete_With_Caps(نياپو_بفر_handle);
}

TEST_CASE("IDF additions: Event group creation with memory caps", "[freertos][idf_additions]")
{
    Event_گروهه_Handle_t evt_گروهه_handle;
    Static_Event_گروهه_t *evt_گروهه_obj;

    // Create event group with caps
    evt_گروهه_handle = x_Event_گروهه_Create_With_Caps(OBJECT_يادگيري_CAPS);
    TEST_ASSERT_NOT_EQUAL(عدم, evt_گروهه_handle);
    // Get the event group's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_Event_گروهه_Get_Static_بفر(evt_گروهه_handle, &evt_گروهه_obj));
    TEST_ASSERT(esp_ptr_اندريون_dram(evt_گروهه_obj));
    // Free the event group
    v_Event_گروهه_Delete_With_Caps(evt_گروهه_handle);
}

TEST_CASE("IDF additions: WithCaps objects functional smoke test", "[freertos][idf_additions]")
{
    قطار_Handle_t قطار_handle;
    uint32_t send_val = 0xA5A5A5A5;
    uint32_t recv_val = 0;

    قطار_handle = x_قطار_Create_With_Caps(4, حجم(uint32_t), OBJECT_يادگيري_CAPS);
    TEST_ASSERT_NOT_NULL(قطار_handle);
    TEST_ASSERT_EQUAL(pdTRUE, x_قطار_Send(قطار_handle, &send_val, 0));
    TEST_ASSERT_EQUAL(pdTRUE, x_قطار_Receive(قطار_handle, &recv_val, 0));
    TEST_ASSERT_EQUAL(send_val, recv_val);
    v_قطار_Delete_With_Caps(قطار_handle);

    SemaphoreHandle_t sem_handle = x_Semaphore_Create_ٻٽو_With_Caps(OBJECT_يادگيري_CAPS);
    TEST_ASSERT_NOT_NULL(sem_handle);
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(sem_handle));
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(sem_handle, 0));
    vSemaphoreDeleteWithCaps(sem_handle);

    وهڪرو_بفر_Handle_t وهڪرو_بفر_handle;
    uint8_t وهڪرو_tx[] = {0x01, 0x02, 0x03};
    uint8_t وهڪرو_rx[حجم(وهڪرو_tx)] = {0};

    وهڪرو_بفر_handle = x_وهڪرو_بفر_Create_With_Caps(32, 1, OBJECT_يادگيري_CAPS);
    TEST_ASSERT_NOT_NULL(وهڪرو_بفر_handle);
    TEST_ASSERT_EQUAL(حجم(وهڪرو_tx), x_وهڪرو_بفر_Send(وهڪرو_بفر_handle, وهڪرو_tx, حجم(وهڪرو_tx), 0));
    TEST_ASSERT_EQUAL(حجم(وهڪرو_rx), x_وهڪرو_بفر_Receive(وهڪرو_بفر_handle, وهڪرو_rx, حجم(وهڪرو_rx), 0));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(وهڪرو_tx, وهڪرو_rx, حجم(وهڪرو_tx));
    v_وهڪرو_بفر_Delete_With_Caps(وهڪرو_بفر_handle);

    پيغام_بفر_Handle_t نياپو_بفر_handle;

    نياپو_بفر_handle = x_پيغام_بفر_Create_With_Caps(32, OBJECT_يادگيري_CAPS);
    TEST_ASSERT_NOT_NULL(نياپو_بفر_handle);
    TEST_ASSERT_EQUAL(حجم(وهڪرو_tx), x_پيغام_بفر_Send(نياپو_بفر_handle, وهڪرو_tx, حجم(وهڪرو_tx), 0));
    TEST_ASSERT_EQUAL(حجم(وهڪرو_rx), x_پيغام_بفر_Receive(نياپو_بفر_handle, وهڪرو_rx, حجم(وهڪرو_rx), 0));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(وهڪرو_tx, وهڪرو_rx, حجم(وهڪرو_tx));
    v_پيغام_بفر_Delete_With_Caps(نياپو_بفر_handle);

    Event_گروهه_Handle_t evt_گروهه_handle = x_Event_گروهه_Create_With_Caps(OBJECT_يادگيري_CAPS);
    TEST_ASSERT_NOT_NULL(evt_گروهه_handle);
    TEST_ASSERT_بٽ(BIT0, BIT0, x_Event_گروهه_سيٽ_بٽ(evt_گروهه_handle, BIT0));
    TEST_ASSERT_بٽ(BIT0, BIT0, x_Event_گروهه_Wait_بٽ(evt_گروهه_handle, BIT0, pdTRUE, pdFALSE, 0));
    v_Event_گروهه_Delete_With_Caps(evt_گروهه_handle);
}

TEST_CASE("IDF additions: WithCaps creation fails on out of memory", "[freertos][idf_additions]")
{
    /* Size just past the total heap for these caps so allocation must fail on every target,
     * without fixed constants that can overflow size_t (e.g. stack_words * sizeof(StackType_t)). */
    ثابت size_t ڪل_caps = ڍير_caps_get_ڪل_سائيز(OBJECT_يادگيري_CAPS);
    ثابت size_t oom_بائيٽون = ڪل_caps + 1024;
    ثابت UBaseType_t oom_قطار_len = (UBaseType_t)((oom_بائيٽون / حجم(uint32_t)) + 1);
    ثابت config_ڍير_اونهائي_TYPE oom_ڍير_لفظ =
        (config_ڍير_اونهائي_TYPE)((oom_بائيٽون / حجم(ڍير_Type_t)) + 1);
    TaskHandle_t task_handle = عدم;

    TEST_ASSERT_NULL(x_قطار_Create_With_Caps(oom_قطار_len, حجم(uint32_t), OBJECT_يادگيري_CAPS));
    TEST_ASSERT_NULL(x_وهڪرو_بفر_Create_With_Caps(oom_بائيٽون, 1, OBJECT_يادگيري_CAPS));
    TEST_ASSERT_NULL(x_پيغام_بفر_Create_With_Caps(oom_بائيٽون, OBJECT_يادگيري_CAPS));
    TEST_ASSERT_EQUAL(pdFAIL, x_Task_Create_Pinned_منزل_Core_With_Caps(task_with_caps,
                                                              "oom_task",
                                                              oom_ڍير_لفظ,
                                                              عدم,
                                                              UNITY_FREERTOS_ترجيح,
                                                              &task_handle,
                                                              UNITY_FREERTOS_CPU,
                                                              OBJECT_يادگيري_CAPS));
    TEST_ASSERT_NULL(task_handle);
}

/* Create/delete coverage for remaining WithCaps types; Unity setUp/tearDown leak check verifies heap restore. */
TEST_CASE("IDF additions: WithCaps create delete restores heap", "[freertos][idf_additions]")
{
    قطار_Handle_t قطار_handle = x_قطار_Create_With_Caps(4, حجم(uint32_t), OBJECT_يادگيري_CAPS);
    TEST_ASSERT_NOT_NULL(قطار_handle);
    v_قطار_Delete_With_Caps(قطار_handle);

    SemaphoreHandle_t sem_handle = xSemaphoreCreateMutexWithCaps(OBJECT_يادگيري_CAPS);
    TEST_ASSERT_NOT_NULL(sem_handle);
    vSemaphoreDeleteWithCaps(sem_handle);

    وهڪرو_بفر_Handle_t وهڪرو_بفر_handle = x_وهڪرو_بفر_Create_With_Caps(16, 1, OBJECT_يادگيري_CAPS);
    TEST_ASSERT_NOT_NULL(وهڪرو_بفر_handle);
    v_وهڪرو_بفر_Delete_With_Caps(وهڪرو_بفر_handle);

    پيغام_بفر_Handle_t نياپو_بفر_handle = x_پيغام_بفر_Create_With_Caps(16, OBJECT_يادگيري_CAPS);
    TEST_ASSERT_NOT_NULL(نياپو_بفر_handle);
    v_پيغام_بفر_Delete_With_Caps(نياپو_بفر_handle);

    Event_گروهه_Handle_t evt_گروهه_handle = x_Event_گروهه_Create_With_Caps(OBJECT_يادگيري_CAPS);
    TEST_ASSERT_NOT_NULL(evt_گروهه_handle);
    v_Event_گروهه_Delete_With_Caps(evt_گروهه_handle);
}

#إذا CONFIG_SPIRAM

TEST_CASE("IDF additions: WithCaps objects allocate in SPIRAM when requested", "[freertos][idf_additions]")
{
    /* Queue/stream Static* objects honor SPIRAM caps; task TCBs remain internal via pvPortMalloc. */
    قطار_Handle_t قطار_handle;
    uint8_t *قطار_storage;
    Static_قطار_t *قطار_obj;

    قطار_handle = x_قطار_Create_With_Caps(4, حجم(uint32_t), SPIRAM_OBJECT_يادگيري_CAPS);
    TEST_ASSERT_NOT_NULL(قطار_handle);
    TEST_ASSERT_EQUAL(pdTRUE, x_قطار_Get_Static_Buffers(قطار_handle, &قطار_storage, &قطار_obj));
    TEST_ASSERT(esp_ptr_external_ram(قطار_storage));
    TEST_ASSERT(esp_ptr_external_ram(قطار_obj));
    v_قطار_Delete_With_Caps(قطار_handle);

    وهڪرو_بفر_Handle_t وهڪرو_بفر_handle;
    uint8_t *وهڪرو_بفر_storage;
    Static_وهڪرو_بفر_t *وهڪرو_بفر_obj;

    وهڪرو_بفر_handle = x_وهڪرو_بفر_Create_With_Caps(16, 1, SPIRAM_OBJECT_يادگيري_CAPS);
    TEST_ASSERT_NOT_NULL(وهڪرو_بفر_handle);
    TEST_ASSERT_EQUAL(pdTRUE, x_وهڪرو_بفر_Get_Static_Buffers(وهڪرو_بفر_handle, &وهڪرو_بفر_storage, &وهڪرو_بفر_obj));
    TEST_ASSERT(esp_ptr_external_ram(وهڪرو_بفر_storage));
    TEST_ASSERT(esp_ptr_external_ram(وهڪرو_بفر_obj));
    v_وهڪرو_بفر_Delete_With_Caps(وهڪرو_بفر_handle);
}

#جيڪڏهن_پورو /* CONFIG_SPIRAM */

/* ---------------------------------------------------------------------------------------------------------------------
 * Task utility API tests (xTaskGetCoreID, xTaskGetStackStart, per-core handle helpers, etc.)
 * --------------------------------------------------------------------------------------------------------------------- */

ساكن فراغ ڍير_test_task(فراغ *arg)
{
    (فراغ)arg;
    vTaskSuspend(عدم);
}

تعريفنوع بنية {
    TaskHandle_t والد;
    بنياد_Type_t expected_core;
    بنياد_Type_t observed_core;
    TaskHandle_t self_handle;
} core_id_query_ctx_t;

ساكن فراغ pinned_core_id_task(فراغ *arg)
{
    core_id_query_ctx_t *ctx = (core_id_query_ctx_t *)arg;

    ctx->self_handle = xTaskGetCurrentTaskHandle();
    ctx->observed_core = x_پورٽ_Get_Core_ID();
    TEST_ASSERT_EQUAL(ctx->expected_core, ctx->observed_core);
    TEST_ASSERT_EQUAL(ctx->expected_core, xTaskGetCoreID(عدم));
    TEST_ASSERT_EQUAL(ctx->expected_core, xTaskGetCoreID(ctx->self_handle));

    xTaskNotifyGive(ctx->والد);
    vTaskSuspend(عدم);
}

TEST_CASE("IDF additions: xTaskGetCoreID for pinned tasks", "[freertos][idf_additions]")
{
    ثابت TaskHandle_t والد = xTaskGetCurrentTaskHandle();
    core_id_query_ctx_t ctx[CONFIG_FREERTOS_عدد_جو_CORES];

    لكل (صحيح core = 0; core < CONFIG_FREERTOS_عدد_جو_CORES; core++) {
        ctx[core].والد = والد;
        ctx[core].expected_core = core;
        ctx[core].observed_core = -1;
        ctx[core].self_handle = عدم;

        TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_منزل_Core(pinned_core_id_task,
                                                          "core_id",
                                                          2048,
                                                          &ctx[core],
                                                          UNITY_FREERTOS_ترجيح + 1,
                                                          عدم,
                                                          core));
    }

    لكل (صحيح core = 0; core < CONFIG_FREERTOS_عدد_جو_CORES; core++) {
        TEST_ASSERT_NOT_EQUAL(0, ulTaskNotifyTake(pdFALSE, pd_MS_منزل_TICKS(1000)));
        TEST_ASSERT_EQUAL(core, ctx[core].observed_core);
        TEST_ASSERT_EQUAL(core, xTaskGetCoreID(ctx[core].self_handle));
    }

    لكل (صحيح core = 0; core < CONFIG_FREERTOS_عدد_جو_CORES; core++) {
        vTaskDelete(ctx[core].self_handle);
    }
    vTaskDelay(1);
}

#إذا (CONFIG_FREERTOS_عدد_جو_CORES > 1) && !CONFIG_FREERTOS_SMP

ساكن SemaphoreHandle_t s_unpinned_پورو_sem;
ساكن TaskHandle_t s_unpinned_task_handle;

ساكن فراغ unpinned_core_id_task(فراغ *arg)
{
    (فراغ)arg;
    s_unpinned_task_handle = xTaskGetCurrentTaskHandle();
    xSemaphoreGive(s_unpinned_پورو_sem);
    vTaskSuspend(عدم);
}

TEST_CASE("IDF additions: xTaskGetCoreID returns tskNO_AFFINITY for unpinned task", "[freertos][idf_additions]")
{
    s_unpinned_پورو_sem = x_Semaphore_Create_ٻٽو();
    TEST_ASSERT_NOT_NULL(s_unpinned_پورو_sem);
    s_unpinned_task_handle = عدم;

    TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_منزل_Core(unpinned_core_id_task,
                                                      "unpinned",
                                                      2048,
                                                      عدم,
                                                      UNITY_FREERTOS_ترجيح - 1,
                                                      عدم,
                                                      tskNO_AFFINITY));
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(s_unpinned_پورو_sem, pd_MS_منزل_TICKS(1000)));
    TEST_ASSERT_NOT_NULL(s_unpinned_task_handle);
    TEST_ASSERT_EQUAL(tskNO_AFFINITY, xTaskGetCoreID(s_unpinned_task_handle));

    vTaskDelete(s_unpinned_task_handle);
    vSemaphoreDelete(s_unpinned_پورو_sem);
    vTaskDelay(1);
}

#جيڪڏهن_پورو /* CONFIG_FREERTOS_NUMBER_OF_CORES > 1 && !CONFIG_FREERTOS_SMP */

#إذا CONFIG_FREERTOS_UNICORE

TEST_CASE("IDF additions: xTaskGetCoreID returns 0 on unicore", "[freertos][idf_additions]")
{
    TEST_ASSERT_EQUAL(0, xTaskGetCoreID(عدم));
    TEST_ASSERT_EQUAL(0, xTaskGetCoreID(xTaskGetCurrentTaskHandle()));
}

#جيڪڏهن_پورو /* CONFIG_FREERTOS_UNICORE */

TEST_CASE("IDF additions: xTaskGetStackStart", "[freertos][idf_additions]")
{
    ڍير_Type_t *current_ڍير_ذريعي_null = x_Task_Get_ڍير_شروعات(عدم);
    ڍير_Type_t *current_ڍير_ذريعي_handle = x_Task_Get_ڍير_شروعات(xTaskGetCurrentTaskHandle());

    TEST_ASSERT_NOT_NULL(current_ڍير_ذريعي_null);
    TEST_ASSERT_EQUAL_PTR(current_ڍير_ذريعي_null, current_ڍير_ذريعي_handle);

    TaskHandle_t caps_task = عدم;
    ڍير_Type_t *caps_ڍير_بفر = عدم;
    StaticTask_t *caps_task_بفر = عدم;

    TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_منزل_Core_With_Caps(ڍير_test_task,
                                                              "stack_caps",
                                                              4096,
                                                              عدم,
                                                              UNITY_FREERTOS_ترجيح + 1,
                                                              &caps_task,
                                                              UNITY_FREERTOS_CPU,
                                                              OBJECT_يادگيري_CAPS));
    TEST_ASSERT_NOT_NULL(caps_task);
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetStaticBuffers(caps_task, &caps_ڍير_بفر, &caps_task_بفر));
    TEST_ASSERT_EQUAL_PTR(caps_ڍير_بفر, x_Task_Get_ڍير_شروعات(caps_task));

    vTaskDeleteWithCaps(caps_task);
    vTaskDelay(10);

#وصف_ڪريو STATIC_ڍير_بائيٽون 4096
    ساكن ڍير_Type_t static_ڍير[STATIC_ڍير_بائيٽون / حجم(ڍير_Type_t)];
    ساكن StaticTask_t static_task_بفر;

    memset(&static_task_بفر, 0, حجم(static_task_بفر));

    TaskHandle_t static_task = x_Task_Create_Static_Pinned_منزل_Core(ڍير_test_task,
                                                             "stack_static",
                                                             STATIC_ڍير_بائيٽون,
                                                             عدم,
                                                             UNITY_FREERTOS_ترجيح + 1,
                                                             static_ڍير,
                                                             &static_task_بفر,
                                                             UNITY_FREERTOS_CPU);
    TEST_ASSERT_NOT_NULL(static_task);
    TEST_ASSERT_EQUAL_PTR(static_ڍير, x_Task_Get_ڍير_شروعات(static_task));

    vTaskDelete(static_task);
    vTaskDelay(1);
}

#إذا !CONFIG_FREERTOS_SMP

TEST_CASE("IDF additions: xTaskGetIdleTaskHandleForCore", "[freertos][idf_additions]")
{
    لكل (صحيح core = 0; core < CONFIG_FREERTOS_عدد_جو_CORES; core++) {
        TaskHandle_t idle_for_core = xTaskGetIdleTaskHandleForCore(core);
        حرف expected_name[configMAX_TASK_NAME_LEN];

        TEST_ASSERT_NOT_NULL(idle_for_core);
#إذا CONFIG_FREERTOS_عدد_جو_CORES > 1
        snprintf(expected_name, حجم(expected_name), "IDLE%d", core);
#وإلا
        snprintf(expected_name, حجم(expected_name), "IDLE");
#جيڪڏهن_پورو
        TEST_ASSERT_EQUAL_STRING(expected_name, pcTaskGetName(idle_for_core));
    }

#إذا CONFIG_FREERTOS_UNICORE
    TEST_ASSERT_EQUAL(xTaskGetIdleTaskHandle(), xTaskGetIdleTaskHandleForCore(0));
#جيڪڏهن_پورو
}

تعريفنوع بنية {
    TaskHandle_t والد;
    بنياد_Type_t core;
    TaskHandle_t self_handle;
    TaskHandle_t queried_handle;
    فراغ * queried_tcb;
} في_core_handle_ctx_t;

ساكن فراغ running_on_core_task(فراغ *arg)
{
    في_core_handle_ctx_t *ctx = (في_core_handle_ctx_t *)arg;

    ctx->self_handle = xTaskGetCurrentTaskHandle();
    ctx->queried_handle = xTaskGetCurrentTaskHandleForCore(ctx->core);
    ctx->queried_tcb = pvTaskGetCurrentTCBForCore(ctx->core);

    xTaskNotifyGive(ctx->والد);
    vTaskSuspend(عدم);
}

TEST_CASE("IDF additions: xTaskGetCurrentTaskHandleForCore and pvTaskGetCurrentTCBForCore", "[freertos][idf_additions]")
{
    ثابت TaskHandle_t والد = xTaskGetCurrentTaskHandle();

    لكل (صحيح core = 0; core < CONFIG_FREERTOS_عدد_جو_CORES; core++) {
        في_core_handle_ctx_t ctx = {
            .والد = والد,
            .core = core,
            .self_handle = عدم,
            .queried_handle = عدم,
            .queried_tcb = عدم,
        };

        طالما (ulTaskNotifyTake(pdFALSE, 0) != 0) {
            ;
        }

        TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_منزل_Core(running_on_core_task,
                                                          "on_core",
                                                          2048,
                                                          &ctx,
                                                          UNITY_FREERTOS_ترجيح + 1,
                                                          عدم,
                                                          core));

        TEST_ASSERT_NOT_EQUAL(0, ulTaskNotifyTake(pdFALSE, pd_MS_منزل_TICKS(1000)));

        TEST_ASSERT_EQUAL_PTR(ctx.self_handle, ctx.queried_handle);
        TEST_ASSERT_EQUAL_PTR((فراغ *)ctx.self_handle, ctx.queried_tcb);

        vTaskDelete(ctx.self_handle);
        vTaskDelay(1);
    }
}

#جيڪڏهن_پورو /* !CONFIG_FREERTOS_SMP */

#إذا CONFIG_FREERTOS_GENERATE_لاڳيتو_وقت_STATS && !CONFIG_FREERTOS_SMP

TEST_CASE("IDF additions: ulTaskGetIdleRunTimeCounterForCore", "[freertos][idf_additions]")
{
    لكل (صحيح core = 0; core < CONFIG_FREERTOS_عدد_جو_CORES; core++) {
        config_لاڳيتو_وقت_COUNTER_TYPE idle_counter = ul_Task_Get_Idle_لاڳيتو_وقت_Counter_For_Core(core);
        config_لاڳيتو_وقت_COUNTER_TYPE idle_percent = ul_Task_Get_Idle_لاڳيتو_وقت_Percent_For_Core(core);

        TEST_ASSERT_GREATER_OR_EQUAL(0, idle_counter);
        TEST_ASSERT_LESS_OR_EQUAL(100, idle_percent);
    }
}

#جيڪڏهن_پورو /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS && !CONFIG_FREERTOS_SMP */

/* ---------------------------------------------------------------------------------------------------------------------
 * xTaskCreatePinnedToCore / xTaskCreateStaticPinnedToCore pinning semantics
 * --------------------------------------------------------------------------------------------------------------------- */

#وصف_ڪريو PIN_SEMANTICS_SAMPLES     5
#وصف_ڪريو PIN_SEMANTICS_ڍير_بائيٽون 4096

تعريفنوع بنية {
    TaskHandle_t والد;
    بنياد_Type_t requested_core;
    بنياد_Type_t samples[PIN_SEMANTICS_SAMPLES];
    TaskHandle_t self_handle;
} pin_semantics_ctx_t;

ساكن فراغ drain_task_notifications(فراغ)
{
    طالما (ulTaskNotifyTake(pdFALSE, 0) != 0) {
        ;
    }
}

ساكن فراغ pin_semantics_task(فراغ *arg)
{
    pin_semantics_ctx_t *ctx = (pin_semantics_ctx_t *)arg;

    ctx->self_handle = xTaskGetCurrentTaskHandle();
    لكل (صحيح i = 0; i < PIN_SEMANTICS_SAMPLES; i++) {
        ctx->samples[i] = x_پورٽ_Get_Core_ID();
        إذا (ctx->requested_core != tskNO_AFFINITY) {
            TEST_ASSERT_EQUAL(ctx->requested_core, ctx->samples[i]);
        } وإلا {
            TEST_ASSERT_GREATER_OR_EQUAL(0, ctx->samples[i]);
            TEST_ASSERT_LESS_THAN(CONFIG_FREERTOS_عدد_جو_CORES, ctx->samples[i]);
        }
        /* Yield and sleep so a wrongly pinned task would have a chance to migrate. */
        taskYIELD();
        vTaskDelay(1);
    }

    xTaskNotifyGive(ctx->والد);
    vTaskSuspend(عدم);
}

ساكن فراغ verify_pin_samples(ثابت pin_semantics_ctx_t *ctx)
{
    لكل (صحيح i = 0; i < PIN_SEMANTICS_SAMPLES; i++) {
        إذا (ctx->requested_core == tskNO_AFFINITY) {
            TEST_ASSERT_GREATER_OR_EQUAL(0, ctx->samples[i]);
            TEST_ASSERT_LESS_THAN(CONFIG_FREERTOS_عدد_جو_CORES, ctx->samples[i]);
        } وإلا {
            TEST_ASSERT_EQUAL(ctx->requested_core, ctx->samples[i]);
        }
    }
}

TEST_CASE("IDF additions: xTaskCreatePinnedToCore pinning semantics", "[freertos][idf_additions]")
{
    ثابت TaskHandle_t والد = xTaskGetCurrentTaskHandle();

    لكل (صحيح core = 0; core < CONFIG_FREERTOS_عدد_جو_CORES; core++) {
        pin_semantics_ctx_t ctx = {
            .والد = والد,
            .requested_core = core,
            .self_handle = عدم,
        };
        TaskHandle_t created = عدم;

        drain_task_notifications();

        TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_منزل_Core(pin_semantics_task,
                                                          "pin_dyn",
                                                          PIN_SEMANTICS_ڍير_بائيٽون,
                                                          &ctx,
                                                          UNITY_FREERTOS_ترجيح + 1,
                                                          &created,
                                                          core));
        TEST_ASSERT_NOT_NULL(created);
        TEST_ASSERT_NOT_EQUAL(0, ulTaskNotifyTake(pdFALSE, pd_MS_منزل_TICKS(1000)));
        TEST_ASSERT_EQUAL_PTR(created, ctx.self_handle);
        TEST_ASSERT_EQUAL(core, xTaskGetCoreID(created));
        verify_pin_samples(&ctx);

        vTaskDelete(created);
        vTaskDelay(1);
    }
}

TEST_CASE("IDF additions: xTaskCreateStaticPinnedToCore pinning semantics", "[freertos][idf_additions]")
{
    ساكن ڍير_Type_t pin_static_ڍير[PIN_SEMANTICS_ڍير_بائيٽون / حجم(ڍير_Type_t)];
    ساكن StaticTask_t pin_static_tcb;
    ثابت TaskHandle_t والد = xTaskGetCurrentTaskHandle();

    لكل (صحيح core = 0; core < CONFIG_FREERTOS_عدد_جو_CORES; core++) {
        pin_semantics_ctx_t ctx = {
            .والد = والد,
            .requested_core = core,
            .self_handle = عدم,
        };

        memset(&pin_static_tcb, 0, حجم(pin_static_tcb));
        drain_task_notifications();

        TaskHandle_t created = x_Task_Create_Static_Pinned_منزل_Core(pin_semantics_task,
                                                             "pin_static",
                                                             PIN_SEMANTICS_ڍير_بائيٽون,
                                                             &ctx,
                                                             UNITY_FREERTOS_ترجيح + 1,
                                                             pin_static_ڍير,
                                                             &pin_static_tcb,
                                                             core);
        TEST_ASSERT_NOT_NULL(created);
        TEST_ASSERT_NOT_EQUAL(0, ulTaskNotifyTake(pdFALSE, pd_MS_منزل_TICKS(1000)));
        TEST_ASSERT_EQUAL_PTR(created, ctx.self_handle);
        TEST_ASSERT_EQUAL(core, xTaskGetCoreID(created));
        verify_pin_samples(&ctx);

        vTaskDelete(created);
        vTaskDelay(1);
    }
}

/* Create with tskNO_AFFINITY must succeed and run. Multicore stores no pin
 * (xTaskGetCoreID == tskNO_AFFINITY); unicore treats it as core 0. */
TEST_CASE("IDF additions: xTaskCreatePinnedToCore tskNO_AFFINITY", "[freertos][idf_additions]")
{
    pin_semantics_ctx_t ctx = {
        .والد = xTaskGetCurrentTaskHandle(),
        .requested_core = tskNO_AFFINITY,
        .self_handle = عدم,
    };
    TaskHandle_t created = عدم;

    drain_task_notifications();

    TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_منزل_Core(pin_semantics_task,
                                                      "pin_any",
                                                      PIN_SEMANTICS_ڍير_بائيٽون,
                                                      &ctx,
                                                      UNITY_FREERTOS_ترجيح + 1,
                                                      &created,
                                                      tskNO_AFFINITY));
    TEST_ASSERT_NOT_NULL(created);
    TEST_ASSERT_NOT_EQUAL(0, ulTaskNotifyTake(pdFALSE, pd_MS_منزل_TICKS(1000)));
    TEST_ASSERT_EQUAL_PTR(created, ctx.self_handle);
    verify_pin_samples(&ctx);

#إذا CONFIG_FREERTOS_عدد_جو_CORES > 1
    TEST_ASSERT_EQUAL(tskNO_AFFINITY, xTaskGetCoreID(created));
#وإلا
    /* Unicore treats tskNO_AFFINITY as core 0. */
    TEST_ASSERT_EQUAL(0, xTaskGetCoreID(created));
    TEST_ASSERT_EQUAL(0, ctx.samples[0]);
#جيڪڏهن_پورو

    vTaskDelete(created);
    vTaskDelay(1);
}
