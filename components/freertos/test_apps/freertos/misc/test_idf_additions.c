/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#समावेश "sdkconfig.h"
#समावेश "freertos/FreeRTOS.h"
#समावेश "freertos/task.h"
#समावेश "freertos/queue.h"
#समावेश "freertos/semphr.h"
#समावेश "freertos/stream_buffer.h"
#समावेश "freertos/message_buffer.h"
#समावेश "freertos/event_groups.h"
#समावेश "freertos/idf_additions.h"
#समावेश "freertos/freertos_debug.h"
#समावेश "esp_memory_utils.h"
#समावेश "esp_heap_caps.h"
#समावेश "unity.h"
#समावेश "test_utils.h"
#समावेश <stdio.h>
#समावेश <string.h>
#समावेश <inttypes.h>

/*
Test ESP-IDF FreeRTOS API additions from idf_additions.h:
    - ...Create...WithCaps() / ...DeleteWithCaps() memory capability helpers
    - xTaskCreatePinnedToCore() / xTaskCreateStaticPinnedToCore() pinning semantics
    - Task utility helpers (xTaskGetCoreID, xTaskGetStackStart, per-core handle queries, etc.)

Run only these cases from the Unity menu with the [idf_additions] tag filter.
 */

#घोषणा OBJECT_MEMORY_CAPS      (MALLOC_CAP_INTERNAL|MALLOC_CAP_8BIT)
#घोषणा SPIRAM_OBJECT_MEMORY_CAPS (MALLOC_CAP_SPIRAM|MALLOC_CAP_8BIT)

अटल व्योम task_with_caps(व्योम *arg)
{
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskSuspend(शून्य);
}

अटल व्योम task_with_caps_self_delete(व्योम *arg)
{
    /* Wait for the unity task to indicate that this task should delete itself */
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    /* Although it is not recommended to self-delete a task with memory caps but this
     * is done intentionally to test for memory leaks */
    vTaskDeleteWithCaps(शून्य);
}

TEST_CASE("IDF additions: Task creation with memory caps and deletion from another task", "[freertos][idf_additions]")
{
    TaskHandle_t task_handle = शून्य;
    ढेर_Type_t *pux_ढेर_बफर;
    StaticTask_t *px_Task_बफर;

    // Create a task with caps
    TEST_ASSERT_EQUAL(pdPASS, xTaskCreatePinnedToCoreWithCaps(task_with_caps, "task", 4096, (व्योम *)xTaskGetCurrentTaskHandle(), UNITY_FREERTOS_PRIORITY + 1, &task_handle, UNITY_FREERTOS_CPU, OBJECT_MEMORY_CAPS));
    TEST_ASSERT_NOT_EQUAL(शून्य, task_handle);
    // Get the task's memory
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetStaticBuffers(task_handle, &pux_ढेर_बफर, &px_Task_बफर));
    TEST_ASSERT(esp_ptr_in_dram(pux_ढेर_बफर));
    TEST_ASSERT(esp_ptr_in_dram(px_Task_बफर));
    // Wait for the created task to block
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    // Delete the task
    vTaskDeleteWithCaps(task_handle);
}

TEST_CASE("IDF additions: Task creation with memory caps and self deletion", "[freertos][idf_additions]")
{
    TaskHandle_t task_handle = शून्य;
    ढेर_Type_t *pux_ढेर_बफर;
    StaticTask_t *px_Task_बफर;

    // Create a task with caps
    TEST_ASSERT_EQUAL(pdPASS, xTaskCreatePinnedToCoreWithCaps(task_with_caps_self_delete, "task", 4096, (व्योम *)xTaskGetCurrentTaskHandle(), UNITY_FREERTOS_PRIORITY + 1, &task_handle, UNITY_FREERTOS_CPU, OBJECT_MEMORY_CAPS));
    TEST_ASSERT_NOT_EQUAL(शून्य, task_handle);
    // Get the task's memory
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetStaticBuffers(task_handle, &pux_ढेर_बफर, &px_Task_बफर));
    TEST_ASSERT(esp_ptr_in_dram(pux_ढेर_बफर));
    TEST_ASSERT(esp_ptr_in_dram(px_Task_बफर));
    // Notify the task to delete itself
    xTaskNotifyGive(task_handle);
}

#अगर CONFIG_FREERTOS_TASK_CREATE_ALLOW_EXT_MEM

TEST_CASE("IDF additions: Task creation with SPIRAM memory caps and self deletion stress test", "[freertos][idf_additions]")
{
#घोषणा TEST_अंक_TASKS      5
#घोषणा TEST_अंक_ITERATIONS 1000
    TaskHandle_t task_handle[TEST_अंक_TASKS];
    ढेर_Type_t *pux_ढेर_बफर;
    StaticTask_t *px_Task_बफर;

    क्रम (पूर्णांक j = 0; j < TEST_अंक_ITERATIONS; j++) {
        क्रम (पूर्णांक i = 0; i < TEST_अंक_TASKS; i++) {
            // Create a task with caps
            TEST_ASSERT_EQUAL(pdPASS, xTaskCreateWithCaps(task_with_caps_self_delete, "task", 4096, शून्य, UNITY_FREERTOS_PRIORITY, &task_handle[i], MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
            // Get the task's memory
            TEST_ASSERT_EQUAL(pdTRUE, xTaskGetStaticBuffers(task_handle[i], &pux_ढेर_बफर, &px_Task_बफर));
            TEST_ASSERT(esp_ptr_external_ram(pux_ढेर_बफर));
            TEST_ASSERT(esp_ptr_in_dram(px_Task_बफर));
        }

        क्रम (पूर्णांक i = 0; i < TEST_अंक_TASKS; i++) {
            // Notify the task to delete itself
            xTaskNotifyGive(task_handle[i]);
        }
        // Let the scheduler run the self-delete tasks.
        vTaskDelay(1);
    }
    // Allow the last batch to be freed by the cleanup task in vTaskDeleteWithCaps().
    vTaskDelay(50);
}

#पूर्ण_अगर /* CONFIG_FREERTOS_TASK_CREATE_ALLOW_EXT_MEM */

#अगर ( CONFIG_FREERTOS_संख्या_OF_CORES > 1 )

अटल व्योम task_with_caps_running_on_other_core(व्योम *arg)
{
    /* Notify the unity task that this task is running on the other core */
    xTaskNotifyGive((TaskHandle_t)arg);

    /* We make sure that this task is running on the other core */
    जबतक (1) {
        ;
    }
}

TEST_CASE("IDF additions: Task creation with memory caps and deletion from another core", "[freertos][idf_additions]")
{
    TaskHandle_t task_handle = शून्य;
    ढेर_Type_t *pux_ढेर_बफर;
    StaticTask_t *px_Task_बफर;

    // Create a task with caps on the other core
    TEST_ASSERT_EQUAL(pdPASS, xTaskCreatePinnedToCoreWithCaps(task_with_caps_running_on_other_core, "task", 4096, (व्योम *)xTaskGetCurrentTaskHandle(), UNITY_FREERTOS_PRIORITY + 1, &task_handle, !UNITY_FREERTOS_CPU, OBJECT_MEMORY_CAPS));
    TEST_ASSERT_NOT_EQUAL(शून्य, task_handle);
    // Get the task's memory
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetStaticBuffers(task_handle, &pux_ढेर_बफर, &px_Task_बफर));
    TEST_ASSERT(esp_ptr_in_dram(pux_ढेर_बफर));
    TEST_ASSERT(esp_ptr_in_dram(px_Task_बफर));
    // Wait for the created task to start running
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    // Delete the task from another core
    vTaskDeleteWithCaps(task_handle);
}

#पूर्ण_अगर // CONFIG_FREERTOS_NUMBER_OF_CORES > 1

TEST_CASE("IDF additions: Queue creation with memory caps", "[freertos][idf_additions]")
{
    QueueHandle_t queue_handle;
    uint8_t *queue_storage;
    StaticQueue_t *queue_obj;

    // Create queue with caps
    queue_handle = xQueueCreateWithCaps(5, माप(uint32_t), OBJECT_MEMORY_CAPS);
    TEST_ASSERT_NOT_EQUAL(शून्य, queue_handle);
    // Get the queue's memory
    TEST_ASSERT_EQUAL(pdTRUE, xQueueGetStaticBuffers(queue_handle, &queue_storage, &queue_obj));
    TEST_ASSERT(esp_ptr_in_dram(queue_storage));
    TEST_ASSERT(esp_ptr_in_dram(queue_obj));
    // Free the queue
    vQueueDeleteWithCaps(queue_handle);
}

TEST_CASE("IDF additions: Semaphore creation with memory caps", "[freertos][idf_additions]")
{
    SemaphoreHandle_t sem_handle;
    StaticSemaphore_t *sem_obj;

    // Create binary semaphore with caps
    sem_handle = x_Semaphore_Create_द्विआधारी_With_Caps(OBJECT_MEMORY_CAPS);
    TEST_ASSERT_NOT_EQUAL(शून्य, sem_handle);
    // Get the binary semaphore's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_Semaphore_Get_Static_बफर(sem_handle, &sem_obj));
    TEST_ASSERT(esp_ptr_in_dram(sem_obj));
    // Free the binary semaphore
    vSemaphoreDeleteWithCaps(sem_handle);

    // Create counting semaphore with caps
    sem_handle = xSemaphoreCreateCountingWithCaps(4, 0, OBJECT_MEMORY_CAPS);
    TEST_ASSERT_NOT_EQUAL(शून्य, sem_handle);
    // Get the counting semaphore's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_Semaphore_Get_Static_बफर(sem_handle, &sem_obj));
    TEST_ASSERT(esp_ptr_in_dram(sem_obj));
    // Free the counting semaphore
    vSemaphoreDeleteWithCaps(sem_handle);

    // Create mutex semaphore with caps
    sem_handle = xSemaphoreCreateMutexWithCaps(OBJECT_MEMORY_CAPS);
    TEST_ASSERT_NOT_EQUAL(शून्य, sem_handle);
    // Get the mutex semaphore's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_Semaphore_Get_Static_बफर(sem_handle, &sem_obj));
    TEST_ASSERT(esp_ptr_in_dram(sem_obj));
    // Free the mutex semaphore
    vSemaphoreDeleteWithCaps(sem_handle);

    // Create recursive mutex with caps
    sem_handle = xSemaphoreCreateRecursiveMutexWithCaps(OBJECT_MEMORY_CAPS);
    TEST_ASSERT_NOT_EQUAL(शून्य, sem_handle);
    // Get the recursive mutex's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_Semaphore_Get_Static_बफर(sem_handle, &sem_obj));
    TEST_ASSERT(esp_ptr_in_dram(sem_obj));
    // Free the recursive mutex
    vSemaphoreDeleteWithCaps(sem_handle);
}

TEST_CASE("IDF additions: Stream & message buffer creation with memory caps", "[freertos][idf_additions]")
{
    Stream_बफर_Handle_t stream_बफर_handle;
    uint8_t *stream_बफर_storage;
    Static_Stream_बफर_t *stream_बफर_obj;

    // Create stream buffer with caps
    stream_बफर_handle = x_Stream_बफर_Create_With_Caps(16, 0, OBJECT_MEMORY_CAPS);
    TEST_ASSERT_NOT_EQUAL(शून्य, stream_बफर_handle);
    // Get the stream buffer's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_Stream_बफर_Get_Static_Buffers(stream_बफर_handle, &stream_बफर_storage, &stream_बफर_obj));
    TEST_ASSERT(esp_ptr_in_dram(stream_बफर_storage));
    TEST_ASSERT(esp_ptr_in_dram(stream_बफर_obj));
    // Free the stream buffer
    v_Stream_बफर_Delete_With_Caps(stream_बफर_handle);

    Message_बफर_Handle_t संदेश_बफर_handle;
    uint8_t *संदेश_बफर_storage;
    Static_Message_बफर_t *संदेश_बफर_obj;

    // Create message buffer with caps
    संदेश_बफर_handle = x_Message_बफर_Create_With_Caps(16, OBJECT_MEMORY_CAPS);
    TEST_ASSERT_NOT_EQUAL(शून्य, संदेश_बफर_handle);
    // Get the message buffer's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_Message_बफर_Get_Static_Buffers(संदेश_बफर_handle, &संदेश_बफर_storage, &संदेश_बफर_obj));
    TEST_ASSERT(esp_ptr_in_dram(संदेश_बफर_storage));
    TEST_ASSERT(esp_ptr_in_dram(संदेश_बफर_obj));
    // Free the message buffer
    v_Message_बफर_Delete_With_Caps(संदेश_बफर_handle);
}

TEST_CASE("IDF additions: Event group creation with memory caps", "[freertos][idf_additions]")
{
    EventGroupHandle_t evt_group_handle;
    StaticEventGroup_t *evt_group_obj;

    // Create event group with caps
    evt_group_handle = xEventGroupCreateWithCaps(OBJECT_MEMORY_CAPS);
    TEST_ASSERT_NOT_EQUAL(शून्य, evt_group_handle);
    // Get the event group's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_Event_Group_Get_Static_बफर(evt_group_handle, &evt_group_obj));
    TEST_ASSERT(esp_ptr_in_dram(evt_group_obj));
    // Free the event group
    vEventGroupDeleteWithCaps(evt_group_handle);
}

TEST_CASE("IDF additions: WithCaps objects functional smoke test", "[freertos][idf_additions]")
{
    QueueHandle_t queue_handle;
    uint32_t send_val = 0xA5A5A5A5;
    uint32_t recv_val = 0;

    queue_handle = xQueueCreateWithCaps(4, माप(uint32_t), OBJECT_MEMORY_CAPS);
    TEST_ASSERT_NOT_NULL(queue_handle);
    TEST_ASSERT_EQUAL(pdTRUE, xQueueSend(queue_handle, &send_val, 0));
    TEST_ASSERT_EQUAL(pdTRUE, xQueueReceive(queue_handle, &recv_val, 0));
    TEST_ASSERT_EQUAL(send_val, recv_val);
    vQueueDeleteWithCaps(queue_handle);

    SemaphoreHandle_t sem_handle = x_Semaphore_Create_द्विआधारी_With_Caps(OBJECT_MEMORY_CAPS);
    TEST_ASSERT_NOT_NULL(sem_handle);
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(sem_handle));
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(sem_handle, 0));
    vSemaphoreDeleteWithCaps(sem_handle);

    Stream_बफर_Handle_t stream_बफर_handle;
    uint8_t stream_tx[] = {0x01, 0x02, 0x03};
    uint8_t stream_rx[माप(stream_tx)] = {0};

    stream_बफर_handle = x_Stream_बफर_Create_With_Caps(32, 1, OBJECT_MEMORY_CAPS);
    TEST_ASSERT_NOT_NULL(stream_बफर_handle);
    TEST_ASSERT_EQUAL(माप(stream_tx), x_Stream_बफर_Send(stream_बफर_handle, stream_tx, माप(stream_tx), 0));
    TEST_ASSERT_EQUAL(माप(stream_rx), x_Stream_बफर_Receive(stream_बफर_handle, stream_rx, माप(stream_rx), 0));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(stream_tx, stream_rx, माप(stream_tx));
    v_Stream_बफर_Delete_With_Caps(stream_बफर_handle);

    Message_बफर_Handle_t संदेश_बफर_handle;

    संदेश_बफर_handle = x_Message_बफर_Create_With_Caps(32, OBJECT_MEMORY_CAPS);
    TEST_ASSERT_NOT_NULL(संदेश_बफर_handle);
    TEST_ASSERT_EQUAL(माप(stream_tx), x_Message_बफर_Send(संदेश_बफर_handle, stream_tx, माप(stream_tx), 0));
    TEST_ASSERT_EQUAL(माप(stream_rx), x_Message_बफर_Receive(संदेश_बफर_handle, stream_rx, माप(stream_rx), 0));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(stream_tx, stream_rx, माप(stream_tx));
    v_Message_बफर_Delete_With_Caps(संदेश_बफर_handle);

    EventGroupHandle_t evt_group_handle = xEventGroupCreateWithCaps(OBJECT_MEMORY_CAPS);
    TEST_ASSERT_NOT_NULL(evt_group_handle);
    TEST_ASSERT_BITS(BIT0, BIT0, xEventGroupSetBits(evt_group_handle, BIT0));
    TEST_ASSERT_BITS(BIT0, BIT0, xEventGroupWaitBits(evt_group_handle, BIT0, pdTRUE, pdFALSE, 0));
    vEventGroupDeleteWithCaps(evt_group_handle);
}

TEST_CASE("IDF additions: WithCaps creation fails on out of memory", "[freertos][idf_additions]")
{
    /* Size just past the total heap for these caps so allocation must fail on every target,
     * without fixed constants that can overflow size_t (e.g. stack_words * sizeof(StackType_t)). */
    स्थिर माप_प्रकार योग_caps = heap_caps_get_योग_आकार(OBJECT_MEMORY_CAPS);
    स्थिर माप_प्रकार oom_bytes = योग_caps + 1024;
    स्थिर UBaseType_t oom_queue_len = (UBaseType_t)((oom_bytes / माप(uint32_t)) + 1);
    स्थिर config_ढेर_DEPTH_TYPE oom_ढेर_शब्दगण =
        (config_ढेर_DEPTH_TYPE)((oom_bytes / माप(ढेर_Type_t)) + 1);
    TaskHandle_t task_handle = शून्य;

    TEST_ASSERT_NULL(xQueueCreateWithCaps(oom_queue_len, माप(uint32_t), OBJECT_MEMORY_CAPS));
    TEST_ASSERT_NULL(x_Stream_बफर_Create_With_Caps(oom_bytes, 1, OBJECT_MEMORY_CAPS));
    TEST_ASSERT_NULL(x_Message_बफर_Create_With_Caps(oom_bytes, OBJECT_MEMORY_CAPS));
    TEST_ASSERT_EQUAL(pdFAIL, xTaskCreatePinnedToCoreWithCaps(task_with_caps,
                                                              "oom_task",
                                                              oom_ढेर_शब्दगण,
                                                              शून्य,
                                                              UNITY_FREERTOS_PRIORITY,
                                                              &task_handle,
                                                              UNITY_FREERTOS_CPU,
                                                              OBJECT_MEMORY_CAPS));
    TEST_ASSERT_NULL(task_handle);
}

/* Create/delete coverage for remaining WithCaps types; Unity setUp/tearDown leak check verifies heap restore. */
TEST_CASE("IDF additions: WithCaps create delete restores heap", "[freertos][idf_additions]")
{
    QueueHandle_t queue_handle = xQueueCreateWithCaps(4, माप(uint32_t), OBJECT_MEMORY_CAPS);
    TEST_ASSERT_NOT_NULL(queue_handle);
    vQueueDeleteWithCaps(queue_handle);

    SemaphoreHandle_t sem_handle = xSemaphoreCreateMutexWithCaps(OBJECT_MEMORY_CAPS);
    TEST_ASSERT_NOT_NULL(sem_handle);
    vSemaphoreDeleteWithCaps(sem_handle);

    Stream_बफर_Handle_t stream_बफर_handle = x_Stream_बफर_Create_With_Caps(16, 1, OBJECT_MEMORY_CAPS);
    TEST_ASSERT_NOT_NULL(stream_बफर_handle);
    v_Stream_बफर_Delete_With_Caps(stream_बफर_handle);

    Message_बफर_Handle_t संदेश_बफर_handle = x_Message_बफर_Create_With_Caps(16, OBJECT_MEMORY_CAPS);
    TEST_ASSERT_NOT_NULL(संदेश_बफर_handle);
    v_Message_बफर_Delete_With_Caps(संदेश_बफर_handle);

    EventGroupHandle_t evt_group_handle = xEventGroupCreateWithCaps(OBJECT_MEMORY_CAPS);
    TEST_ASSERT_NOT_NULL(evt_group_handle);
    vEventGroupDeleteWithCaps(evt_group_handle);
}

#अगर CONFIG_SPIRAM

TEST_CASE("IDF additions: WithCaps objects allocate in SPIRAM when requested", "[freertos][idf_additions]")
{
    /* Queue/stream Static* objects honor SPIRAM caps; task TCBs remain internal via pvPortMalloc. */
    QueueHandle_t queue_handle;
    uint8_t *queue_storage;
    StaticQueue_t *queue_obj;

    queue_handle = xQueueCreateWithCaps(4, माप(uint32_t), SPIRAM_OBJECT_MEMORY_CAPS);
    TEST_ASSERT_NOT_NULL(queue_handle);
    TEST_ASSERT_EQUAL(pdTRUE, xQueueGetStaticBuffers(queue_handle, &queue_storage, &queue_obj));
    TEST_ASSERT(esp_ptr_external_ram(queue_storage));
    TEST_ASSERT(esp_ptr_external_ram(queue_obj));
    vQueueDeleteWithCaps(queue_handle);

    Stream_बफर_Handle_t stream_बफर_handle;
    uint8_t *stream_बफर_storage;
    Static_Stream_बफर_t *stream_बफर_obj;

    stream_बफर_handle = x_Stream_बफर_Create_With_Caps(16, 1, SPIRAM_OBJECT_MEMORY_CAPS);
    TEST_ASSERT_NOT_NULL(stream_बफर_handle);
    TEST_ASSERT_EQUAL(pdTRUE, x_Stream_बफर_Get_Static_Buffers(stream_बफर_handle, &stream_बफर_storage, &stream_बफर_obj));
    TEST_ASSERT(esp_ptr_external_ram(stream_बफर_storage));
    TEST_ASSERT(esp_ptr_external_ram(stream_बफर_obj));
    v_Stream_बफर_Delete_With_Caps(stream_बफर_handle);
}

#पूर्ण_अगर /* CONFIG_SPIRAM */

/* ---------------------------------------------------------------------------------------------------------------------
 * Task utility API tests (xTaskGetCoreID, xTaskGetStackStart, per-core handle helpers, etc.)
 * --------------------------------------------------------------------------------------------------------------------- */

अटल व्योम ढेर_test_task(व्योम *arg)
{
    (व्योम)arg;
    vTaskSuspend(शून्य);
}

प्रकार काष्ठा {
    TaskHandle_t parent;
    BaseType_t expected_core;
    BaseType_t observed_core;
    TaskHandle_t self_handle;
} core_id_query_ctx_t;

अटल व्योम pinned_core_id_task(व्योम *arg)
{
    core_id_query_ctx_t *ctx = (core_id_query_ctx_t *)arg;

    ctx->self_handle = xTaskGetCurrentTaskHandle();
    ctx->observed_core = xPortGetCoreID();
    TEST_ASSERT_EQUAL(ctx->expected_core, ctx->observed_core);
    TEST_ASSERT_EQUAL(ctx->expected_core, xTaskGetCoreID(शून्य));
    TEST_ASSERT_EQUAL(ctx->expected_core, xTaskGetCoreID(ctx->self_handle));

    xTaskNotifyGive(ctx->parent);
    vTaskSuspend(शून्य);
}

TEST_CASE("IDF additions: xTaskGetCoreID for pinned tasks", "[freertos][idf_additions]")
{
    स्थिर TaskHandle_t parent = xTaskGetCurrentTaskHandle();
    core_id_query_ctx_t ctx[CONFIG_FREERTOS_संख्या_OF_CORES];

    क्रम (पूर्णांक core = 0; core < CONFIG_FREERTOS_संख्या_OF_CORES; core++) {
        ctx[core].parent = parent;
        ctx[core].expected_core = core;
        ctx[core].observed_core = -1;
        ctx[core].self_handle = शून्य;

        TEST_ASSERT_EQUAL(pdPASS, xTaskCreatePinnedToCore(pinned_core_id_task,
                                                          "core_id",
                                                          2048,
                                                          &ctx[core],
                                                          UNITY_FREERTOS_PRIORITY + 1,
                                                          शून्य,
                                                          core));
    }

    क्रम (पूर्णांक core = 0; core < CONFIG_FREERTOS_संख्या_OF_CORES; core++) {
        TEST_ASSERT_NOT_EQUAL(0, ulTaskNotifyTake(pdFALSE, pdMS_TO_TICKS(1000)));
        TEST_ASSERT_EQUAL(core, ctx[core].observed_core);
        TEST_ASSERT_EQUAL(core, xTaskGetCoreID(ctx[core].self_handle));
    }

    क्रम (पूर्णांक core = 0; core < CONFIG_FREERTOS_संख्या_OF_CORES; core++) {
        vTaskDelete(ctx[core].self_handle);
    }
    vTaskDelay(1);
}

#अगर (CONFIG_FREERTOS_संख्या_OF_CORES > 1) && !CONFIG_FREERTOS_SMP

अटल SemaphoreHandle_t s_unpinned_done_sem;
अटल TaskHandle_t s_unpinned_task_handle;

अटल व्योम unpinned_core_id_task(व्योम *arg)
{
    (व्योम)arg;
    s_unpinned_task_handle = xTaskGetCurrentTaskHandle();
    xSemaphoreGive(s_unpinned_done_sem);
    vTaskSuspend(शून्य);
}

TEST_CASE("IDF additions: xTaskGetCoreID returns tskNO_AFFINITY for unpinned task", "[freertos][idf_additions]")
{
    s_unpinned_done_sem = x_Semaphore_Create_द्विआधारी();
    TEST_ASSERT_NOT_NULL(s_unpinned_done_sem);
    s_unpinned_task_handle = शून्य;

    TEST_ASSERT_EQUAL(pdPASS, xTaskCreatePinnedToCore(unpinned_core_id_task,
                                                      "unpinned",
                                                      2048,
                                                      शून्य,
                                                      UNITY_FREERTOS_PRIORITY - 1,
                                                      शून्य,
                                                      tskNO_AFFINITY));
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(s_unpinned_done_sem, pdMS_TO_TICKS(1000)));
    TEST_ASSERT_NOT_NULL(s_unpinned_task_handle);
    TEST_ASSERT_EQUAL(tskNO_AFFINITY, xTaskGetCoreID(s_unpinned_task_handle));

    vTaskDelete(s_unpinned_task_handle);
    vSemaphoreDelete(s_unpinned_done_sem);
    vTaskDelay(1);
}

#पूर्ण_अगर /* CONFIG_FREERTOS_NUMBER_OF_CORES > 1 && !CONFIG_FREERTOS_SMP */

#अगर CONFIG_FREERTOS_UNICORE

TEST_CASE("IDF additions: xTaskGetCoreID returns 0 on unicore", "[freertos][idf_additions]")
{
    TEST_ASSERT_EQUAL(0, xTaskGetCoreID(शून्य));
    TEST_ASSERT_EQUAL(0, xTaskGetCoreID(xTaskGetCurrentTaskHandle()));
}

#पूर्ण_अगर /* CONFIG_FREERTOS_UNICORE */

TEST_CASE("IDF additions: xTaskGetStackStart", "[freertos][idf_additions]")
{
    ढेर_Type_t *current_ढेर_via_null = x_Task_Get_ढेर_Start(शून्य);
    ढेर_Type_t *current_ढेर_via_handle = x_Task_Get_ढेर_Start(xTaskGetCurrentTaskHandle());

    TEST_ASSERT_NOT_NULL(current_ढेर_via_null);
    TEST_ASSERT_EQUAL_PTR(current_ढेर_via_null, current_ढेर_via_handle);

    TaskHandle_t caps_task = शून्य;
    ढेर_Type_t *caps_ढेर_बफर = शून्य;
    StaticTask_t *caps_task_बफर = शून्य;

    TEST_ASSERT_EQUAL(pdPASS, xTaskCreatePinnedToCoreWithCaps(ढेर_test_task,
                                                              "stack_caps",
                                                              4096,
                                                              शून्य,
                                                              UNITY_FREERTOS_PRIORITY + 1,
                                                              &caps_task,
                                                              UNITY_FREERTOS_CPU,
                                                              OBJECT_MEMORY_CAPS));
    TEST_ASSERT_NOT_NULL(caps_task);
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetStaticBuffers(caps_task, &caps_ढेर_बफर, &caps_task_बफर));
    TEST_ASSERT_EQUAL_PTR(caps_ढेर_बफर, x_Task_Get_ढेर_Start(caps_task));

    vTaskDeleteWithCaps(caps_task);
    vTaskDelay(10);

#घोषणा STATIC_ढेर_BYTES 4096
    अटल ढेर_Type_t static_ढेर[STATIC_ढेर_BYTES / माप(ढेर_Type_t)];
    अटल StaticTask_t static_task_बफर;

    स_रखो(&static_task_बफर, 0, माप(static_task_बफर));

    TaskHandle_t static_task = xTaskCreateStaticPinnedToCore(ढेर_test_task,
                                                             "stack_static",
                                                             STATIC_ढेर_BYTES,
                                                             शून्य,
                                                             UNITY_FREERTOS_PRIORITY + 1,
                                                             static_ढेर,
                                                             &static_task_बफर,
                                                             UNITY_FREERTOS_CPU);
    TEST_ASSERT_NOT_NULL(static_task);
    TEST_ASSERT_EQUAL_PTR(static_ढेर, x_Task_Get_ढेर_Start(static_task));

    vTaskDelete(static_task);
    vTaskDelay(1);
}

#अगर !CONFIG_FREERTOS_SMP

TEST_CASE("IDF additions: xTaskGetIdleTaskHandleForCore", "[freertos][idf_additions]")
{
    क्रम (पूर्णांक core = 0; core < CONFIG_FREERTOS_संख्या_OF_CORES; core++) {
        TaskHandle_t idle_for_core = xTaskGetIdleTaskHandleForCore(core);
        अक्षर expected_name[configMAX_TASK_NAME_LEN];

        TEST_ASSERT_NOT_NULL(idle_for_core);
#अगर CONFIG_FREERTOS_संख्या_OF_CORES > 1
        snprintf(expected_name, माप(expected_name), "IDLE%d", core);
#अन्यथा
        snprintf(expected_name, माप(expected_name), "IDLE");
#पूर्ण_अगर
        TEST_ASSERT_EQUAL_STRING(expected_name, pcTaskGetName(idle_for_core));
    }

#अगर CONFIG_FREERTOS_UNICORE
    TEST_ASSERT_EQUAL(xTaskGetIdleTaskHandle(), xTaskGetIdleTaskHandleForCore(0));
#पूर्ण_अगर
}

प्रकार काष्ठा {
    TaskHandle_t parent;
    BaseType_t core;
    TaskHandle_t self_handle;
    TaskHandle_t queried_handle;
    व्योम * queried_tcb;
} per_core_handle_ctx_t;

अटल व्योम running_on_core_task(व्योम *arg)
{
    per_core_handle_ctx_t *ctx = (per_core_handle_ctx_t *)arg;

    ctx->self_handle = xTaskGetCurrentTaskHandle();
    ctx->queried_handle = xTaskGetCurrentTaskHandleForCore(ctx->core);
    ctx->queried_tcb = pvTaskGetCurrentTCBForCore(ctx->core);

    xTaskNotifyGive(ctx->parent);
    vTaskSuspend(शून्य);
}

TEST_CASE("IDF additions: xTaskGetCurrentTaskHandleForCore and pvTaskGetCurrentTCBForCore", "[freertos][idf_additions]")
{
    स्थिर TaskHandle_t parent = xTaskGetCurrentTaskHandle();

    क्रम (पूर्णांक core = 0; core < CONFIG_FREERTOS_संख्या_OF_CORES; core++) {
        per_core_handle_ctx_t ctx = {
            .parent = parent,
            .core = core,
            .self_handle = शून्य,
            .queried_handle = शून्य,
            .queried_tcb = शून्य,
        };

        जबतक (ulTaskNotifyTake(pdFALSE, 0) != 0) {
            ;
        }

        TEST_ASSERT_EQUAL(pdPASS, xTaskCreatePinnedToCore(running_on_core_task,
                                                          "on_core",
                                                          2048,
                                                          &ctx,
                                                          UNITY_FREERTOS_PRIORITY + 1,
                                                          शून्य,
                                                          core));

        TEST_ASSERT_NOT_EQUAL(0, ulTaskNotifyTake(pdFALSE, pdMS_TO_TICKS(1000)));

        TEST_ASSERT_EQUAL_PTR(ctx.self_handle, ctx.queried_handle);
        TEST_ASSERT_EQUAL_PTR((व्योम *)ctx.self_handle, ctx.queried_tcb);

        vTaskDelete(ctx.self_handle);
        vTaskDelay(1);
    }
}

#पूर्ण_अगर /* !CONFIG_FREERTOS_SMP */

#अगर CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS && !CONFIG_FREERTOS_SMP

TEST_CASE("IDF additions: ulTaskGetIdleRunTimeCounterForCore", "[freertos][idf_additions]")
{
    क्रम (पूर्णांक core = 0; core < CONFIG_FREERTOS_संख्या_OF_CORES; core++) {
        config_RUN_TIME_गणक_TYPE idle_गणक = ul_Task_Get_Idle_Run_Time_गणक_For_Core(core);
        config_RUN_TIME_गणक_TYPE idle_percent = ulTaskGetIdleRunTimePercentForCore(core);

        TEST_ASSERT_GREATER_OR_EQUAL(0, idle_गणक);
        TEST_ASSERT_LESS_OR_EQUAL(100, idle_percent);
    }
}

#पूर्ण_अगर /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS && !CONFIG_FREERTOS_SMP */

/* ---------------------------------------------------------------------------------------------------------------------
 * xTaskCreatePinnedToCore / xTaskCreateStaticPinnedToCore pinning semantics
 * --------------------------------------------------------------------------------------------------------------------- */

#घोषणा PIN_SEMANTICS_SAMPLES     5
#घोषणा PIN_SEMANTICS_ढेर_BYTES 4096

प्रकार काष्ठा {
    TaskHandle_t parent;
    BaseType_t requested_core;
    BaseType_t samples[PIN_SEMANTICS_SAMPLES];
    TaskHandle_t self_handle;
} pin_semantics_ctx_t;

अटल व्योम drain_task_notifications(व्योम)
{
    जबतक (ulTaskNotifyTake(pdFALSE, 0) != 0) {
        ;
    }
}

अटल व्योम pin_semantics_task(व्योम *arg)
{
    pin_semantics_ctx_t *ctx = (pin_semantics_ctx_t *)arg;

    ctx->self_handle = xTaskGetCurrentTaskHandle();
    क्रम (पूर्णांक i = 0; i < PIN_SEMANTICS_SAMPLES; i++) {
        ctx->samples[i] = xPortGetCoreID();
        अगर (ctx->requested_core != tskNO_AFFINITY) {
            TEST_ASSERT_EQUAL(ctx->requested_core, ctx->samples[i]);
        } अन्यथा {
            TEST_ASSERT_GREATER_OR_EQUAL(0, ctx->samples[i]);
            TEST_ASSERT_LESS_THAN(CONFIG_FREERTOS_संख्या_OF_CORES, ctx->samples[i]);
        }
        /* Yield and sleep so a wrongly pinned task would have a chance to migrate. */
        taskYIELD();
        vTaskDelay(1);
    }

    xTaskNotifyGive(ctx->parent);
    vTaskSuspend(शून्य);
}

अटल व्योम verify_pin_samples(स्थिर pin_semantics_ctx_t *ctx)
{
    क्रम (पूर्णांक i = 0; i < PIN_SEMANTICS_SAMPLES; i++) {
        अगर (ctx->requested_core == tskNO_AFFINITY) {
            TEST_ASSERT_GREATER_OR_EQUAL(0, ctx->samples[i]);
            TEST_ASSERT_LESS_THAN(CONFIG_FREERTOS_संख्या_OF_CORES, ctx->samples[i]);
        } अन्यथा {
            TEST_ASSERT_EQUAL(ctx->requested_core, ctx->samples[i]);
        }
    }
}

TEST_CASE("IDF additions: xTaskCreatePinnedToCore pinning semantics", "[freertos][idf_additions]")
{
    स्थिर TaskHandle_t parent = xTaskGetCurrentTaskHandle();

    क्रम (पूर्णांक core = 0; core < CONFIG_FREERTOS_संख्या_OF_CORES; core++) {
        pin_semantics_ctx_t ctx = {
            .parent = parent,
            .requested_core = core,
            .self_handle = शून्य,
        };
        TaskHandle_t created = शून्य;

        drain_task_notifications();

        TEST_ASSERT_EQUAL(pdPASS, xTaskCreatePinnedToCore(pin_semantics_task,
                                                          "pin_dyn",
                                                          PIN_SEMANTICS_ढेर_BYTES,
                                                          &ctx,
                                                          UNITY_FREERTOS_PRIORITY + 1,
                                                          &created,
                                                          core));
        TEST_ASSERT_NOT_NULL(created);
        TEST_ASSERT_NOT_EQUAL(0, ulTaskNotifyTake(pdFALSE, pdMS_TO_TICKS(1000)));
        TEST_ASSERT_EQUAL_PTR(created, ctx.self_handle);
        TEST_ASSERT_EQUAL(core, xTaskGetCoreID(created));
        verify_pin_samples(&ctx);

        vTaskDelete(created);
        vTaskDelay(1);
    }
}

TEST_CASE("IDF additions: xTaskCreateStaticPinnedToCore pinning semantics", "[freertos][idf_additions]")
{
    अटल ढेर_Type_t pin_static_ढेर[PIN_SEMANTICS_ढेर_BYTES / माप(ढेर_Type_t)];
    अटल StaticTask_t pin_static_tcb;
    स्थिर TaskHandle_t parent = xTaskGetCurrentTaskHandle();

    क्रम (पूर्णांक core = 0; core < CONFIG_FREERTOS_संख्या_OF_CORES; core++) {
        pin_semantics_ctx_t ctx = {
            .parent = parent,
            .requested_core = core,
            .self_handle = शून्य,
        };

        स_रखो(&pin_static_tcb, 0, माप(pin_static_tcb));
        drain_task_notifications();

        TaskHandle_t created = xTaskCreateStaticPinnedToCore(pin_semantics_task,
                                                             "pin_static",
                                                             PIN_SEMANTICS_ढेर_BYTES,
                                                             &ctx,
                                                             UNITY_FREERTOS_PRIORITY + 1,
                                                             pin_static_ढेर,
                                                             &pin_static_tcb,
                                                             core);
        TEST_ASSERT_NOT_NULL(created);
        TEST_ASSERT_NOT_EQUAL(0, ulTaskNotifyTake(pdFALSE, pdMS_TO_TICKS(1000)));
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
        .parent = xTaskGetCurrentTaskHandle(),
        .requested_core = tskNO_AFFINITY,
        .self_handle = शून्य,
    };
    TaskHandle_t created = शून्य;

    drain_task_notifications();

    TEST_ASSERT_EQUAL(pdPASS, xTaskCreatePinnedToCore(pin_semantics_task,
                                                      "pin_any",
                                                      PIN_SEMANTICS_ढेर_BYTES,
                                                      &ctx,
                                                      UNITY_FREERTOS_PRIORITY + 1,
                                                      &created,
                                                      tskNO_AFFINITY));
    TEST_ASSERT_NOT_NULL(created);
    TEST_ASSERT_NOT_EQUAL(0, ulTaskNotifyTake(pdFALSE, pdMS_TO_TICKS(1000)));
    TEST_ASSERT_EQUAL_PTR(created, ctx.self_handle);
    verify_pin_samples(&ctx);

#अगर CONFIG_FREERTOS_संख्या_OF_CORES > 1
    TEST_ASSERT_EQUAL(tskNO_AFFINITY, xTaskGetCoreID(created));
#अन्यथा
    /* Unicore treats tskNO_AFFINITY as core 0. */
    TEST_ASSERT_EQUAL(0, xTaskGetCoreID(created));
    TEST_ASSERT_EQUAL(0, ctx.samples[0]);
#पूर्ण_अगर

    vTaskDelete(created);
    vTaskDelay(1);
}
