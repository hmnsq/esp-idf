/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#അന്തര്ഭാവ "sdkconfig.h"
#അന്തര്ഭാവ "freertos/FreeRTOS.h"
#അന്തര്ഭാവ "freertos/task.h"
#അന്തര്ഭാവ "freertos/queue.h"
#അന്തര്ഭാവ "freertos/semphr.h"
#അന്തര്ഭാവ "freertos/stream_buffer.h"
#അന്തര്ഭാവ "freertos/message_buffer.h"
#അന്തര്ഭാവ "freertos/event_groups.h"
#അന്തര്ഭാവ "freertos/idf_additions.h"
#അന്തര്ഭാവ "freertos/freertos_debug.h"
#അന്തര്ഭാവ "esp_memory_utils.h"
#അന്തര്ഭാവ "esp_heap_caps.h"
#അന്തര്ഭാവ "unity.h"
#അന്തര്ഭാവ "test_utils.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <inttypes.h>

/*
Test ESP-IDF FreeRTOS API additions from idf_additions.h:
    - ...Create...WithCaps() / ...DeleteWithCaps() memory capability helpers
    - xTaskCreatePinnedToCore() / xTaskCreateStaticPinnedToCore() pinning semantics
    - Task utility helpers (xTaskGetCoreID, xTaskGetStackStart, per-core handle queries, etc.)

Run only these cases from the Unity menu with the [idf_additions] tag filter.
 */

#define OBJECT_MEMORY_CAPS      (MALLOC_CAP_INTERNAL|MALLOC_CAP_8BIT)
#define SPIRAM_OBJECT_MEMORY_CAPS (MALLOC_CAP_SPIRAM|MALLOC_CAP_8BIT)

സ്ഥിതം ശൂന്യം task_with_caps(ശൂന്യം *arg)
{
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskSuspend(ശൂന്യ);
}

സ്ഥിതം ശൂന്യം task_with_caps_self_delete(ശൂന്യം *arg)
{
    /* Wait for the unity task to indicate that this task should delete itself */
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    /* Although it is not recommended to self-delete a task with memory caps but this
     * is done intentionally to test for memory leaks */
    vTaskDeleteWithCaps(ശൂന്യ);
}

TEST_CASE("IDF additions: Task creation with memory caps and deletion from another task", "[freertos][idf_additions]")
{
    TaskHandle_t task_handle = ശൂന്യ;
    അട്ടി_Type_t *pux_അട്ടി_Buffer;
    StaticTask_t *pxTaskBuffer;

    // Create a task with caps
    TEST_ASSERT_EQUAL(pdPASS, xTaskCreatePinnedToCoreWithCaps(task_with_caps, "task", 4096, (ശൂന്യം *)xTaskGetCurrentTaskHandle(), UNITY_FREERTOS_PRIORITY + 1, &task_handle, UNITY_FREERTOS_CPU, OBJECT_MEMORY_CAPS));
    TEST_ASSERT_NOT_EQUAL(ശൂന്യ, task_handle);
    // Get the task's memory
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetStaticBuffers(task_handle, &pux_അട്ടി_Buffer, &pxTaskBuffer));
    TEST_ASSERT(esp_ptr_in_dram(pux_അട്ടി_Buffer));
    TEST_ASSERT(esp_ptr_in_dram(pxTaskBuffer));
    // Wait for the created task to block
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    // Delete the task
    vTaskDeleteWithCaps(task_handle);
}

TEST_CASE("IDF additions: Task creation with memory caps and self deletion", "[freertos][idf_additions]")
{
    TaskHandle_t task_handle = ശൂന്യ;
    അട്ടി_Type_t *pux_അട്ടി_Buffer;
    StaticTask_t *pxTaskBuffer;

    // Create a task with caps
    TEST_ASSERT_EQUAL(pdPASS, xTaskCreatePinnedToCoreWithCaps(task_with_caps_self_delete, "task", 4096, (ശൂന്യം *)xTaskGetCurrentTaskHandle(), UNITY_FREERTOS_PRIORITY + 1, &task_handle, UNITY_FREERTOS_CPU, OBJECT_MEMORY_CAPS));
    TEST_ASSERT_NOT_EQUAL(ശൂന്യ, task_handle);
    // Get the task's memory
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetStaticBuffers(task_handle, &pux_അട്ടി_Buffer, &pxTaskBuffer));
    TEST_ASSERT(esp_ptr_in_dram(pux_അട്ടി_Buffer));
    TEST_ASSERT(esp_ptr_in_dram(pxTaskBuffer));
    // Notify the task to delete itself
    xTaskNotifyGive(task_handle);
}

#എങ്കിൽ CONFIG_FREERTOS_TASK_CREATE_ALLOW_EXT_MEM

TEST_CASE("IDF additions: Task creation with SPIRAM memory caps and self deletion stress test", "[freertos][idf_additions]")
{
#define TEST_അക്കം_TASKS      5
#define TEST_അക്കം_ITERATIONS 1000
    TaskHandle_t task_handle[TEST_അക്കം_TASKS];
    അട്ടി_Type_t *pux_അട്ടി_Buffer;
    StaticTask_t *pxTaskBuffer;

    വേണ്ടി (പൂർണ്ണസംഖ്യ j = 0; j < TEST_അക്കം_ITERATIONS; j++) {
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < TEST_അക്കം_TASKS; i++) {
            // Create a task with caps
            TEST_ASSERT_EQUAL(pdPASS, xTaskCreateWithCaps(task_with_caps_self_delete, "task", 4096, ശൂന്യ, UNITY_FREERTOS_PRIORITY, &task_handle[i], MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT));
            // Get the task's memory
            TEST_ASSERT_EQUAL(pdTRUE, xTaskGetStaticBuffers(task_handle[i], &pux_അട്ടി_Buffer, &pxTaskBuffer));
            TEST_ASSERT(esp_ptr_external_ram(pux_അട്ടി_Buffer));
            TEST_ASSERT(esp_ptr_in_dram(pxTaskBuffer));
        }

        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < TEST_അക്കം_TASKS; i++) {
            // Notify the task to delete itself
            xTaskNotifyGive(task_handle[i]);
        }
        // Let the scheduler run the self-delete tasks.
        vTaskDelay(1);
    }
    // Allow the last batch to be freed by the cleanup task in vTaskDeleteWithCaps().
    vTaskDelay(50);
}

#endif /* CONFIG_FREERTOS_TASK_CREATE_ALLOW_EXT_MEM */

#എങ്കിൽ ( CONFIG_FREERTOS_സംഖ്യ_OF_CORES > 1 )

സ്ഥിതം ശൂന്യം task_with_caps_running_on_other_core(ശൂന്യം *arg)
{
    /* Notify the unity task that this task is running on the other core */
    xTaskNotifyGive((TaskHandle_t)arg);

    /* We make sure that this task is running on the other core */
    ആകുമ്പോൾ (1) {
        ;
    }
}

TEST_CASE("IDF additions: Task creation with memory caps and deletion from another core", "[freertos][idf_additions]")
{
    TaskHandle_t task_handle = ശൂന്യ;
    അട്ടി_Type_t *pux_അട്ടി_Buffer;
    StaticTask_t *pxTaskBuffer;

    // Create a task with caps on the other core
    TEST_ASSERT_EQUAL(pdPASS, xTaskCreatePinnedToCoreWithCaps(task_with_caps_running_on_other_core, "task", 4096, (ശൂന്യം *)xTaskGetCurrentTaskHandle(), UNITY_FREERTOS_PRIORITY + 1, &task_handle, !UNITY_FREERTOS_CPU, OBJECT_MEMORY_CAPS));
    TEST_ASSERT_NOT_EQUAL(ശൂന്യ, task_handle);
    // Get the task's memory
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetStaticBuffers(task_handle, &pux_അട്ടി_Buffer, &pxTaskBuffer));
    TEST_ASSERT(esp_ptr_in_dram(pux_അട്ടി_Buffer));
    TEST_ASSERT(esp_ptr_in_dram(pxTaskBuffer));
    // Wait for the created task to start running
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    // Delete the task from another core
    vTaskDeleteWithCaps(task_handle);
}

#endif // CONFIG_FREERTOS_NUMBER_OF_CORES > 1

TEST_CASE("IDF additions: Queue creation with memory caps", "[freertos][idf_additions]")
{
    QueueHandle_t queue_handle;
    uint8_t *queue_storage;
    StaticQueue_t *queue_obj;

    // Create queue with caps
    queue_handle = xQueueCreateWithCaps(5, വലുപ്പം(uint32_t), OBJECT_MEMORY_CAPS);
    TEST_ASSERT_NOT_EQUAL(ശൂന്യ, queue_handle);
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
    sem_handle = x_Semaphore_Create_ദ്വിമാന_With_Caps(OBJECT_MEMORY_CAPS);
    TEST_ASSERT_NOT_EQUAL(ശൂന്യ, sem_handle);
    // Get the binary semaphore's memory
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGetStaticBuffer(sem_handle, &sem_obj));
    TEST_ASSERT(esp_ptr_in_dram(sem_obj));
    // Free the binary semaphore
    vSemaphoreDeleteWithCaps(sem_handle);

    // Create counting semaphore with caps
    sem_handle = xSemaphoreCreateCountingWithCaps(4, 0, OBJECT_MEMORY_CAPS);
    TEST_ASSERT_NOT_EQUAL(ശൂന്യ, sem_handle);
    // Get the counting semaphore's memory
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGetStaticBuffer(sem_handle, &sem_obj));
    TEST_ASSERT(esp_ptr_in_dram(sem_obj));
    // Free the counting semaphore
    vSemaphoreDeleteWithCaps(sem_handle);

    // Create mutex semaphore with caps
    sem_handle = xSemaphoreCreateMutexWithCaps(OBJECT_MEMORY_CAPS);
    TEST_ASSERT_NOT_EQUAL(ശൂന്യ, sem_handle);
    // Get the mutex semaphore's memory
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGetStaticBuffer(sem_handle, &sem_obj));
    TEST_ASSERT(esp_ptr_in_dram(sem_obj));
    // Free the mutex semaphore
    vSemaphoreDeleteWithCaps(sem_handle);

    // Create recursive mutex with caps
    sem_handle = xSemaphoreCreateRecursiveMutexWithCaps(OBJECT_MEMORY_CAPS);
    TEST_ASSERT_NOT_EQUAL(ശൂന്യ, sem_handle);
    // Get the recursive mutex's memory
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGetStaticBuffer(sem_handle, &sem_obj));
    TEST_ASSERT(esp_ptr_in_dram(sem_obj));
    // Free the recursive mutex
    vSemaphoreDeleteWithCaps(sem_handle);
}

TEST_CASE("IDF additions: Stream & message buffer creation with memory caps", "[freertos][idf_additions]")
{
    StreamBufferHandle_t stream_buffer_handle;
    uint8_t *stream_buffer_storage;
    StaticStreamBuffer_t *stream_buffer_obj;

    // Create stream buffer with caps
    stream_buffer_handle = xStreamBufferCreateWithCaps(16, 0, OBJECT_MEMORY_CAPS);
    TEST_ASSERT_NOT_EQUAL(ശൂന്യ, stream_buffer_handle);
    // Get the stream buffer's memory
    TEST_ASSERT_EQUAL(pdTRUE, xStreamBufferGetStaticBuffers(stream_buffer_handle, &stream_buffer_storage, &stream_buffer_obj));
    TEST_ASSERT(esp_ptr_in_dram(stream_buffer_storage));
    TEST_ASSERT(esp_ptr_in_dram(stream_buffer_obj));
    // Free the stream buffer
    vStreamBufferDeleteWithCaps(stream_buffer_handle);

    MessageBufferHandle_t സന്ദേശം_buffer_handle;
    uint8_t *സന്ദേശം_buffer_storage;
    StaticMessageBuffer_t *സന്ദേശം_buffer_obj;

    // Create message buffer with caps
    സന്ദേശം_buffer_handle = xMessageBufferCreateWithCaps(16, OBJECT_MEMORY_CAPS);
    TEST_ASSERT_NOT_EQUAL(ശൂന്യ, സന്ദേശം_buffer_handle);
    // Get the message buffer's memory
    TEST_ASSERT_EQUAL(pdTRUE, xMessageBufferGetStaticBuffers(സന്ദേശം_buffer_handle, &സന്ദേശം_buffer_storage, &സന്ദേശം_buffer_obj));
    TEST_ASSERT(esp_ptr_in_dram(സന്ദേശം_buffer_storage));
    TEST_ASSERT(esp_ptr_in_dram(സന്ദേശം_buffer_obj));
    // Free the message buffer
    vMessageBufferDeleteWithCaps(സന്ദേശം_buffer_handle);
}

TEST_CASE("IDF additions: Event group creation with memory caps", "[freertos][idf_additions]")
{
    EventGroupHandle_t evt_group_handle;
    StaticEventGroup_t *evt_group_obj;

    // Create event group with caps
    evt_group_handle = xEventGroupCreateWithCaps(OBJECT_MEMORY_CAPS);
    TEST_ASSERT_NOT_EQUAL(ശൂന്യ, evt_group_handle);
    // Get the event group's memory
    TEST_ASSERT_EQUAL(pdTRUE, xEventGroupGetStaticBuffer(evt_group_handle, &evt_group_obj));
    TEST_ASSERT(esp_ptr_in_dram(evt_group_obj));
    // Free the event group
    vEventGroupDeleteWithCaps(evt_group_handle);
}

TEST_CASE("IDF additions: WithCaps objects functional smoke test", "[freertos][idf_additions]")
{
    QueueHandle_t queue_handle;
    uint32_t send_val = 0xA5A5A5A5;
    uint32_t recv_val = 0;

    queue_handle = xQueueCreateWithCaps(4, വലുപ്പം(uint32_t), OBJECT_MEMORY_CAPS);
    TEST_ASSERT_NOT_NULL(queue_handle);
    TEST_ASSERT_EQUAL(pdTRUE, xQueueSend(queue_handle, &send_val, 0));
    TEST_ASSERT_EQUAL(pdTRUE, xQueueReceive(queue_handle, &recv_val, 0));
    TEST_ASSERT_EQUAL(send_val, recv_val);
    vQueueDeleteWithCaps(queue_handle);

    SemaphoreHandle_t sem_handle = x_Semaphore_Create_ദ്വിമാന_With_Caps(OBJECT_MEMORY_CAPS);
    TEST_ASSERT_NOT_NULL(sem_handle);
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(sem_handle));
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(sem_handle, 0));
    vSemaphoreDeleteWithCaps(sem_handle);

    StreamBufferHandle_t stream_buffer_handle;
    uint8_t stream_tx[] = {0x01, 0x02, 0x03};
    uint8_t stream_rx[വലുപ്പം(stream_tx)] = {0};

    stream_buffer_handle = xStreamBufferCreateWithCaps(32, 1, OBJECT_MEMORY_CAPS);
    TEST_ASSERT_NOT_NULL(stream_buffer_handle);
    TEST_ASSERT_EQUAL(വലുപ്പം(stream_tx), xStreamBufferSend(stream_buffer_handle, stream_tx, വലുപ്പം(stream_tx), 0));
    TEST_ASSERT_EQUAL(വലുപ്പം(stream_rx), xStreamBufferReceive(stream_buffer_handle, stream_rx, വലുപ്പം(stream_rx), 0));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(stream_tx, stream_rx, വലുപ്പം(stream_tx));
    vStreamBufferDeleteWithCaps(stream_buffer_handle);

    MessageBufferHandle_t സന്ദേശം_buffer_handle;

    സന്ദേശം_buffer_handle = xMessageBufferCreateWithCaps(32, OBJECT_MEMORY_CAPS);
    TEST_ASSERT_NOT_NULL(സന്ദേശം_buffer_handle);
    TEST_ASSERT_EQUAL(വലുപ്പം(stream_tx), xMessageBufferSend(സന്ദേശം_buffer_handle, stream_tx, വലുപ്പം(stream_tx), 0));
    TEST_ASSERT_EQUAL(വലുപ്പം(stream_rx), xMessageBufferReceive(സന്ദേശം_buffer_handle, stream_rx, വലുപ്പം(stream_rx), 0));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(stream_tx, stream_rx, വലുപ്പം(stream_tx));
    vMessageBufferDeleteWithCaps(സന്ദേശം_buffer_handle);

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
    സ്ഥിരം size_t total_caps = heap_caps_get_total_size(OBJECT_MEMORY_CAPS);
    സ്ഥിരം size_t oom_bytes = total_caps + 1024;
    സ്ഥിരം UBaseType_t oom_queue_len = (UBaseType_t)((oom_bytes / വലുപ്പം(uint32_t)) + 1);
    സ്ഥിരം config_അട്ടി_DEPTH_TYPE oom_അട്ടി_വാക്കുകൾ =
        (config_അട്ടി_DEPTH_TYPE)((oom_bytes / വലുപ്പം(അട്ടി_Type_t)) + 1);
    TaskHandle_t task_handle = ശൂന്യ;

    TEST_ASSERT_NULL(xQueueCreateWithCaps(oom_queue_len, വലുപ്പം(uint32_t), OBJECT_MEMORY_CAPS));
    TEST_ASSERT_NULL(xStreamBufferCreateWithCaps(oom_bytes, 1, OBJECT_MEMORY_CAPS));
    TEST_ASSERT_NULL(xMessageBufferCreateWithCaps(oom_bytes, OBJECT_MEMORY_CAPS));
    TEST_ASSERT_EQUAL(pdFAIL, xTaskCreatePinnedToCoreWithCaps(task_with_caps,
                                                              "oom_task",
                                                              oom_അട്ടി_വാക്കുകൾ,
                                                              ശൂന്യ,
                                                              UNITY_FREERTOS_PRIORITY,
                                                              &task_handle,
                                                              UNITY_FREERTOS_CPU,
                                                              OBJECT_MEMORY_CAPS));
    TEST_ASSERT_NULL(task_handle);
}

/* Create/delete coverage for remaining WithCaps types; Unity setUp/tearDown leak check verifies heap restore. */
TEST_CASE("IDF additions: WithCaps create delete restores heap", "[freertos][idf_additions]")
{
    QueueHandle_t queue_handle = xQueueCreateWithCaps(4, വലുപ്പം(uint32_t), OBJECT_MEMORY_CAPS);
    TEST_ASSERT_NOT_NULL(queue_handle);
    vQueueDeleteWithCaps(queue_handle);

    SemaphoreHandle_t sem_handle = xSemaphoreCreateMutexWithCaps(OBJECT_MEMORY_CAPS);
    TEST_ASSERT_NOT_NULL(sem_handle);
    vSemaphoreDeleteWithCaps(sem_handle);

    StreamBufferHandle_t stream_buffer_handle = xStreamBufferCreateWithCaps(16, 1, OBJECT_MEMORY_CAPS);
    TEST_ASSERT_NOT_NULL(stream_buffer_handle);
    vStreamBufferDeleteWithCaps(stream_buffer_handle);

    MessageBufferHandle_t സന്ദേശം_buffer_handle = xMessageBufferCreateWithCaps(16, OBJECT_MEMORY_CAPS);
    TEST_ASSERT_NOT_NULL(സന്ദേശം_buffer_handle);
    vMessageBufferDeleteWithCaps(സന്ദേശം_buffer_handle);

    EventGroupHandle_t evt_group_handle = xEventGroupCreateWithCaps(OBJECT_MEMORY_CAPS);
    TEST_ASSERT_NOT_NULL(evt_group_handle);
    vEventGroupDeleteWithCaps(evt_group_handle);
}

#എങ്കിൽ CONFIG_SPIRAM

TEST_CASE("IDF additions: WithCaps objects allocate in SPIRAM when requested", "[freertos][idf_additions]")
{
    /* Queue/stream Static* objects honor SPIRAM caps; task TCBs remain internal via pvPortMalloc. */
    QueueHandle_t queue_handle;
    uint8_t *queue_storage;
    StaticQueue_t *queue_obj;

    queue_handle = xQueueCreateWithCaps(4, വലുപ്പം(uint32_t), SPIRAM_OBJECT_MEMORY_CAPS);
    TEST_ASSERT_NOT_NULL(queue_handle);
    TEST_ASSERT_EQUAL(pdTRUE, xQueueGetStaticBuffers(queue_handle, &queue_storage, &queue_obj));
    TEST_ASSERT(esp_ptr_external_ram(queue_storage));
    TEST_ASSERT(esp_ptr_external_ram(queue_obj));
    vQueueDeleteWithCaps(queue_handle);

    StreamBufferHandle_t stream_buffer_handle;
    uint8_t *stream_buffer_storage;
    StaticStreamBuffer_t *stream_buffer_obj;

    stream_buffer_handle = xStreamBufferCreateWithCaps(16, 1, SPIRAM_OBJECT_MEMORY_CAPS);
    TEST_ASSERT_NOT_NULL(stream_buffer_handle);
    TEST_ASSERT_EQUAL(pdTRUE, xStreamBufferGetStaticBuffers(stream_buffer_handle, &stream_buffer_storage, &stream_buffer_obj));
    TEST_ASSERT(esp_ptr_external_ram(stream_buffer_storage));
    TEST_ASSERT(esp_ptr_external_ram(stream_buffer_obj));
    vStreamBufferDeleteWithCaps(stream_buffer_handle);
}

#endif /* CONFIG_SPIRAM */

/* ---------------------------------------------------------------------------------------------------------------------
 * Task utility API tests (xTaskGetCoreID, xTaskGetStackStart, per-core handle helpers, etc.)
 * --------------------------------------------------------------------------------------------------------------------- */

സ്ഥിതം ശൂന്യം അട്ടി_test_task(ശൂന്യം *arg)
{
    (ശൂന്യം)arg;
    vTaskSuspend(ശൂന്യ);
}

തരനിർവചനം ഘടന {
    TaskHandle_t parent;
    BaseType_t expected_core;
    BaseType_t observed_core;
    TaskHandle_t self_handle;
} core_id_query_ctx_t;

സ്ഥിതം ശൂന്യം pinned_core_id_task(ശൂന്യം *arg)
{
    core_id_query_ctx_t *ctx = (core_id_query_ctx_t *)arg;

    ctx->self_handle = xTaskGetCurrentTaskHandle();
    ctx->observed_core = xPortGetCoreID();
    TEST_ASSERT_EQUAL(ctx->expected_core, ctx->observed_core);
    TEST_ASSERT_EQUAL(ctx->expected_core, xTaskGetCoreID(ശൂന്യ));
    TEST_ASSERT_EQUAL(ctx->expected_core, xTaskGetCoreID(ctx->self_handle));

    xTaskNotifyGive(ctx->parent);
    vTaskSuspend(ശൂന്യ);
}

TEST_CASE("IDF additions: xTaskGetCoreID for pinned tasks", "[freertos][idf_additions]")
{
    സ്ഥിരം TaskHandle_t parent = xTaskGetCurrentTaskHandle();
    core_id_query_ctx_t ctx[CONFIG_FREERTOS_സംഖ്യ_OF_CORES];

    വേണ്ടി (പൂർണ്ണസംഖ്യ core = 0; core < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; core++) {
        ctx[core].parent = parent;
        ctx[core].expected_core = core;
        ctx[core].observed_core = -1;
        ctx[core].self_handle = ശൂന്യ;

        TEST_ASSERT_EQUAL(pdPASS, xTaskCreatePinnedToCore(pinned_core_id_task,
                                                          "core_id",
                                                          2048,
                                                          &ctx[core],
                                                          UNITY_FREERTOS_PRIORITY + 1,
                                                          ശൂന്യ,
                                                          core));
    }

    വേണ്ടി (പൂർണ്ണസംഖ്യ core = 0; core < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; core++) {
        TEST_ASSERT_NOT_EQUAL(0, ulTaskNotifyTake(pdFALSE, pdMS_TO_TICKS(1000)));
        TEST_ASSERT_EQUAL(core, ctx[core].observed_core);
        TEST_ASSERT_EQUAL(core, xTaskGetCoreID(ctx[core].self_handle));
    }

    വേണ്ടി (പൂർണ്ണസംഖ്യ core = 0; core < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; core++) {
        vTaskDelete(ctx[core].self_handle);
    }
    vTaskDelay(1);
}

#എങ്കിൽ (CONFIG_FREERTOS_സംഖ്യ_OF_CORES > 1) && !CONFIG_FREERTOS_SMP

സ്ഥിതം SemaphoreHandle_t s_unpinned_done_sem;
സ്ഥിതം TaskHandle_t s_unpinned_task_handle;

സ്ഥിതം ശൂന്യം unpinned_core_id_task(ശൂന്യം *arg)
{
    (ശൂന്യം)arg;
    s_unpinned_task_handle = xTaskGetCurrentTaskHandle();
    xSemaphoreGive(s_unpinned_done_sem);
    vTaskSuspend(ശൂന്യ);
}

TEST_CASE("IDF additions: xTaskGetCoreID returns tskNO_AFFINITY for unpinned task", "[freertos][idf_additions]")
{
    s_unpinned_done_sem = x_Semaphore_Create_ദ്വിമാന();
    TEST_ASSERT_NOT_NULL(s_unpinned_done_sem);
    s_unpinned_task_handle = ശൂന്യ;

    TEST_ASSERT_EQUAL(pdPASS, xTaskCreatePinnedToCore(unpinned_core_id_task,
                                                      "unpinned",
                                                      2048,
                                                      ശൂന്യ,
                                                      UNITY_FREERTOS_PRIORITY - 1,
                                                      ശൂന്യ,
                                                      tskNO_AFFINITY));
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(s_unpinned_done_sem, pdMS_TO_TICKS(1000)));
    TEST_ASSERT_NOT_NULL(s_unpinned_task_handle);
    TEST_ASSERT_EQUAL(tskNO_AFFINITY, xTaskGetCoreID(s_unpinned_task_handle));

    vTaskDelete(s_unpinned_task_handle);
    vSemaphoreDelete(s_unpinned_done_sem);
    vTaskDelay(1);
}

#endif /* CONFIG_FREERTOS_NUMBER_OF_CORES > 1 && !CONFIG_FREERTOS_SMP */

#എങ്കിൽ CONFIG_FREERTOS_UNICORE

TEST_CASE("IDF additions: xTaskGetCoreID returns 0 on unicore", "[freertos][idf_additions]")
{
    TEST_ASSERT_EQUAL(0, xTaskGetCoreID(ശൂന്യ));
    TEST_ASSERT_EQUAL(0, xTaskGetCoreID(xTaskGetCurrentTaskHandle()));
}

#endif /* CONFIG_FREERTOS_UNICORE */

TEST_CASE("IDF additions: xTaskGetStackStart", "[freertos][idf_additions]")
{
    അട്ടി_Type_t *current_അട്ടി_via_null = x_Task_Get_അട്ടി_Start(ശൂന്യ);
    അട്ടി_Type_t *current_അട്ടി_via_handle = x_Task_Get_അട്ടി_Start(xTaskGetCurrentTaskHandle());

    TEST_ASSERT_NOT_NULL(current_അട്ടി_via_null);
    TEST_ASSERT_EQUAL_PTR(current_അട്ടി_via_null, current_അട്ടി_via_handle);

    TaskHandle_t caps_task = ശൂന്യ;
    അട്ടി_Type_t *caps_അട്ടി_buffer = ശൂന്യ;
    StaticTask_t *caps_task_buffer = ശൂന്യ;

    TEST_ASSERT_EQUAL(pdPASS, xTaskCreatePinnedToCoreWithCaps(അട്ടി_test_task,
                                                              "stack_caps",
                                                              4096,
                                                              ശൂന്യ,
                                                              UNITY_FREERTOS_PRIORITY + 1,
                                                              &caps_task,
                                                              UNITY_FREERTOS_CPU,
                                                              OBJECT_MEMORY_CAPS));
    TEST_ASSERT_NOT_NULL(caps_task);
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetStaticBuffers(caps_task, &caps_അട്ടി_buffer, &caps_task_buffer));
    TEST_ASSERT_EQUAL_PTR(caps_അട്ടി_buffer, x_Task_Get_അട്ടി_Start(caps_task));

    vTaskDeleteWithCaps(caps_task);
    vTaskDelay(10);

#define STATIC_അട്ടി_BYTES 4096
    സ്ഥിതം അട്ടി_Type_t static_അട്ടി[STATIC_അട്ടി_BYTES / വലുപ്പം(അട്ടി_Type_t)];
    സ്ഥിതം StaticTask_t static_task_buffer;

    memset(&static_task_buffer, 0, വലുപ്പം(static_task_buffer));

    TaskHandle_t static_task = xTaskCreateStaticPinnedToCore(അട്ടി_test_task,
                                                             "stack_static",
                                                             STATIC_അട്ടി_BYTES,
                                                             ശൂന്യ,
                                                             UNITY_FREERTOS_PRIORITY + 1,
                                                             static_അട്ടി,
                                                             &static_task_buffer,
                                                             UNITY_FREERTOS_CPU);
    TEST_ASSERT_NOT_NULL(static_task);
    TEST_ASSERT_EQUAL_PTR(static_അട്ടി, x_Task_Get_അട്ടി_Start(static_task));

    vTaskDelete(static_task);
    vTaskDelay(1);
}

#എങ്കിൽ !CONFIG_FREERTOS_SMP

TEST_CASE("IDF additions: xTaskGetIdleTaskHandleForCore", "[freertos][idf_additions]")
{
    വേണ്ടി (പൂർണ്ണസംഖ്യ core = 0; core < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; core++) {
        TaskHandle_t idle_for_core = xTaskGetIdleTaskHandleForCore(core);
        അക്ഷരം expected_name[configMAX_TASK_NAME_LEN];

        TEST_ASSERT_NOT_NULL(idle_for_core);
#എങ്കിൽ CONFIG_FREERTOS_സംഖ്യ_OF_CORES > 1
        snprintf(expected_name, വലുപ്പം(expected_name), "IDLE%d", core);
#അല്ലെങ്കിൽ
        snprintf(expected_name, വലുപ്പം(expected_name), "IDLE");
#endif
        TEST_ASSERT_EQUAL_STRING(expected_name, pcTaskGetName(idle_for_core));
    }

#എങ്കിൽ CONFIG_FREERTOS_UNICORE
    TEST_ASSERT_EQUAL(xTaskGetIdleTaskHandle(), xTaskGetIdleTaskHandleForCore(0));
#endif
}

തരനിർവചനം ഘടന {
    TaskHandle_t parent;
    BaseType_t core;
    TaskHandle_t self_handle;
    TaskHandle_t queried_handle;
    ശൂന്യം * queried_tcb;
} per_core_handle_ctx_t;

സ്ഥിതം ശൂന്യം running_on_core_task(ശൂന്യം *arg)
{
    per_core_handle_ctx_t *ctx = (per_core_handle_ctx_t *)arg;

    ctx->self_handle = xTaskGetCurrentTaskHandle();
    ctx->queried_handle = xTaskGetCurrentTaskHandleForCore(ctx->core);
    ctx->queried_tcb = pvTaskGetCurrentTCBForCore(ctx->core);

    xTaskNotifyGive(ctx->parent);
    vTaskSuspend(ശൂന്യ);
}

TEST_CASE("IDF additions: xTaskGetCurrentTaskHandleForCore and pvTaskGetCurrentTCBForCore", "[freertos][idf_additions]")
{
    സ്ഥിരം TaskHandle_t parent = xTaskGetCurrentTaskHandle();

    വേണ്ടി (പൂർണ്ണസംഖ്യ core = 0; core < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; core++) {
        per_core_handle_ctx_t ctx = {
            .parent = parent,
            .core = core,
            .self_handle = ശൂന്യ,
            .queried_handle = ശൂന്യ,
            .queried_tcb = ശൂന്യ,
        };

        ആകുമ്പോൾ (ulTaskNotifyTake(pdFALSE, 0) != 0) {
            ;
        }

        TEST_ASSERT_EQUAL(pdPASS, xTaskCreatePinnedToCore(running_on_core_task,
                                                          "on_core",
                                                          2048,
                                                          &ctx,
                                                          UNITY_FREERTOS_PRIORITY + 1,
                                                          ശൂന്യ,
                                                          core));

        TEST_ASSERT_NOT_EQUAL(0, ulTaskNotifyTake(pdFALSE, pdMS_TO_TICKS(1000)));

        TEST_ASSERT_EQUAL_PTR(ctx.self_handle, ctx.queried_handle);
        TEST_ASSERT_EQUAL_PTR((ശൂന്യം *)ctx.self_handle, ctx.queried_tcb);

        vTaskDelete(ctx.self_handle);
        vTaskDelay(1);
    }
}

#endif /* !CONFIG_FREERTOS_SMP */

#എങ്കിൽ CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS && !CONFIG_FREERTOS_SMP

TEST_CASE("IDF additions: ulTaskGetIdleRunTimeCounterForCore", "[freertos][idf_additions]")
{
    വേണ്ടി (പൂർണ്ണസംഖ്യ core = 0; core < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; core++) {
        configRUN_TIME_COUNTER_TYPE idle_counter = ulTaskGetIdleRunTimeCounterForCore(core);
        configRUN_TIME_COUNTER_TYPE idle_percent = ulTaskGetIdleRunTimePercentForCore(core);

        TEST_ASSERT_GREATER_OR_EQUAL(0, idle_counter);
        TEST_ASSERT_LESS_OR_EQUAL(100, idle_percent);
    }
}

#endif /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS && !CONFIG_FREERTOS_SMP */

/* ---------------------------------------------------------------------------------------------------------------------
 * xTaskCreatePinnedToCore / xTaskCreateStaticPinnedToCore pinning semantics
 * --------------------------------------------------------------------------------------------------------------------- */

#define PIN_SEMANTICS_SAMPLES     5
#define PIN_SEMANTICS_അട്ടി_BYTES 4096

തരനിർവചനം ഘടന {
    TaskHandle_t parent;
    BaseType_t requested_core;
    BaseType_t samples[PIN_SEMANTICS_SAMPLES];
    TaskHandle_t self_handle;
} pin_semantics_ctx_t;

സ്ഥിതം ശൂന്യം drain_task_notifications(ശൂന്യം)
{
    ആകുമ്പോൾ (ulTaskNotifyTake(pdFALSE, 0) != 0) {
        ;
    }
}

സ്ഥിതം ശൂന്യം pin_semantics_task(ശൂന്യം *arg)
{
    pin_semantics_ctx_t *ctx = (pin_semantics_ctx_t *)arg;

    ctx->self_handle = xTaskGetCurrentTaskHandle();
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < PIN_SEMANTICS_SAMPLES; i++) {
        ctx->samples[i] = xPortGetCoreID();
        എങ്കിൽ (ctx->requested_core != tskNO_AFFINITY) {
            TEST_ASSERT_EQUAL(ctx->requested_core, ctx->samples[i]);
        } അല്ലെങ്കിൽ {
            TEST_ASSERT_GREATER_OR_EQUAL(0, ctx->samples[i]);
            TEST_ASSERT_LESS_THAN(CONFIG_FREERTOS_സംഖ്യ_OF_CORES, ctx->samples[i]);
        }
        /* Yield and sleep so a wrongly pinned task would have a chance to migrate. */
        taskYIELD();
        vTaskDelay(1);
    }

    xTaskNotifyGive(ctx->parent);
    vTaskSuspend(ശൂന്യ);
}

സ്ഥിതം ശൂന്യം verify_pin_samples(സ്ഥിരം pin_semantics_ctx_t *ctx)
{
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < PIN_SEMANTICS_SAMPLES; i++) {
        എങ്കിൽ (ctx->requested_core == tskNO_AFFINITY) {
            TEST_ASSERT_GREATER_OR_EQUAL(0, ctx->samples[i]);
            TEST_ASSERT_LESS_THAN(CONFIG_FREERTOS_സംഖ്യ_OF_CORES, ctx->samples[i]);
        } അല്ലെങ്കിൽ {
            TEST_ASSERT_EQUAL(ctx->requested_core, ctx->samples[i]);
        }
    }
}

TEST_CASE("IDF additions: xTaskCreatePinnedToCore pinning semantics", "[freertos][idf_additions]")
{
    സ്ഥിരം TaskHandle_t parent = xTaskGetCurrentTaskHandle();

    വേണ്ടി (പൂർണ്ണസംഖ്യ core = 0; core < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; core++) {
        pin_semantics_ctx_t ctx = {
            .parent = parent,
            .requested_core = core,
            .self_handle = ശൂന്യ,
        };
        TaskHandle_t created = ശൂന്യ;

        drain_task_notifications();

        TEST_ASSERT_EQUAL(pdPASS, xTaskCreatePinnedToCore(pin_semantics_task,
                                                          "pin_dyn",
                                                          PIN_SEMANTICS_അട്ടി_BYTES,
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
    സ്ഥിതം അട്ടി_Type_t pin_static_അട്ടി[PIN_SEMANTICS_അട്ടി_BYTES / വലുപ്പം(അട്ടി_Type_t)];
    സ്ഥിതം StaticTask_t pin_static_tcb;
    സ്ഥിരം TaskHandle_t parent = xTaskGetCurrentTaskHandle();

    വേണ്ടി (പൂർണ്ണസംഖ്യ core = 0; core < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; core++) {
        pin_semantics_ctx_t ctx = {
            .parent = parent,
            .requested_core = core,
            .self_handle = ശൂന്യ,
        };

        memset(&pin_static_tcb, 0, വലുപ്പം(pin_static_tcb));
        drain_task_notifications();

        TaskHandle_t created = xTaskCreateStaticPinnedToCore(pin_semantics_task,
                                                             "pin_static",
                                                             PIN_SEMANTICS_അട്ടി_BYTES,
                                                             &ctx,
                                                             UNITY_FREERTOS_PRIORITY + 1,
                                                             pin_static_അട്ടി,
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
        .self_handle = ശൂന്യ,
    };
    TaskHandle_t created = ശൂന്യ;

    drain_task_notifications();

    TEST_ASSERT_EQUAL(pdPASS, xTaskCreatePinnedToCore(pin_semantics_task,
                                                      "pin_any",
                                                      PIN_SEMANTICS_അട്ടി_BYTES,
                                                      &ctx,
                                                      UNITY_FREERTOS_PRIORITY + 1,
                                                      &created,
                                                      tskNO_AFFINITY));
    TEST_ASSERT_NOT_NULL(created);
    TEST_ASSERT_NOT_EQUAL(0, ulTaskNotifyTake(pdFALSE, pdMS_TO_TICKS(1000)));
    TEST_ASSERT_EQUAL_PTR(created, ctx.self_handle);
    verify_pin_samples(&ctx);

#എങ്കിൽ CONFIG_FREERTOS_സംഖ്യ_OF_CORES > 1
    TEST_ASSERT_EQUAL(tskNO_AFFINITY, xTaskGetCoreID(created));
#അല്ലെങ്കിൽ
    /* Unicore treats tskNO_AFFINITY as core 0. */
    TEST_ASSERT_EQUAL(0, xTaskGetCoreID(created));
    TEST_ASSERT_EQUAL(0, ctx.samples[0]);
#endif

    vTaskDelete(created);
    vTaskDelay(1);
}
