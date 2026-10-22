/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/queue.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/semphr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/stream_buffer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/message_buffer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/event_groups.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/idf_additions.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/freertos_debug.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_memory_utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_heap_caps.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unity.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "test_utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <inttypes.h>

/*
Test ESP-IDF FreeRTOS API additions from idf_additions.h:
    - ...Create...WithCaps() / ...DeleteWithCaps() memory capability helpers
    - xTaskCreatePinnedToCore() / xTaskCreateStaticPinnedToCore() pinning semantics
    - Task utility helpers (xTaskGetCoreID, xTaskGetStackStart, per-core handle queries, etc.)

Run only these cases from the Unity menu with the [idf_additions] tag filter.
 */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OBJECT_ਸਮ੍ਰਿਤੀ_CAPS      (MALLOC_CAP_ਅੰਦਰੂਨੀ|MALLOC_CAP_8_ਦੋਆਧਾਰੀ_ਅੰਕ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SPIRAM_OBJECT_ਸਮ੍ਰਿਤੀ_CAPS (MALLOC_CAP_SPIRAM|MALLOC_CAP_8_ਦੋਆਧਾਰੀ_ਅੰਕ)

ਸਥਿਰ ਖਾਲੀ task_with_caps(ਖਾਲੀ *arg)
{
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskSuspend(ਨਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ task_with_caps_self_delete(ਖਾਲੀ *arg)
{
    /* Wait for the unity task to indicate that this task should delete itself */
    ulTaskNotifyTake(pdTRUE, ਪੋਰਟ_MAX_DELAY);

    /* Although it is not recommended to self-delete a task with memory caps but this
     * is done intentionally to test for memory leaks */
    vTaskDeleteWithCaps(ਨਿਰਾ);
}

TEST_CASE("IDF additions: Task creation with memory caps and deletion from another task", "[freertos][idf_additions]")
{
    TaskHandle_t task_handle = ਨਿਰਾ;
    ਢੇਰ_Type_t *pux_ਢੇਰ_ਬਫਰ;
    StaticTask_t *px_Task_ਬਫਰ;

    // Create a task with caps
    TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core_With_Caps(task_with_caps, "task", 4096, (ਖਾਲੀ *)xTaskGetCurrentTaskHandle(), UNITY_FREERTOS_ਤਰਜੀਹ + 1, &task_handle, UNITY_FREERTOS_CPU, OBJECT_ਸਮ੍ਰਿਤੀ_CAPS));
    TEST_ASSERT_NOT_EQUAL(ਨਿਰਾ, task_handle);
    // Get the task's memory
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetStaticBuffers(task_handle, &pux_ਢੇਰ_ਬਫਰ, &px_Task_ਬਫਰ));
    TEST_ASSERT(esp_ptr_ਵਿੱਚ_dram(pux_ਢੇਰ_ਬਫਰ));
    TEST_ASSERT(esp_ptr_ਵਿੱਚ_dram(px_Task_ਬਫਰ));
    // Wait for the created task to block
    ulTaskNotifyTake(pdTRUE, ਪੋਰਟ_MAX_DELAY);
    // Delete the task
    vTaskDeleteWithCaps(task_handle);
}

TEST_CASE("IDF additions: Task creation with memory caps and self deletion", "[freertos][idf_additions]")
{
    TaskHandle_t task_handle = ਨਿਰਾ;
    ਢੇਰ_Type_t *pux_ਢੇਰ_ਬਫਰ;
    StaticTask_t *px_Task_ਬਫਰ;

    // Create a task with caps
    TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core_With_Caps(task_with_caps_self_delete, "task", 4096, (ਖਾਲੀ *)xTaskGetCurrentTaskHandle(), UNITY_FREERTOS_ਤਰਜੀਹ + 1, &task_handle, UNITY_FREERTOS_CPU, OBJECT_ਸਮ੍ਰਿਤੀ_CAPS));
    TEST_ASSERT_NOT_EQUAL(ਨਿਰਾ, task_handle);
    // Get the task's memory
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetStaticBuffers(task_handle, &pux_ਢੇਰ_ਬਫਰ, &px_Task_ਬਫਰ));
    TEST_ASSERT(esp_ptr_ਵਿੱਚ_dram(pux_ਢੇਰ_ਬਫਰ));
    TEST_ASSERT(esp_ptr_ਵਿੱਚ_dram(px_Task_ਬਫਰ));
    // Notify the task to delete itself
    xTaskNotifyGive(task_handle);
}

#ਜੇ CONFIG_FREERTOS_TASK_CREATE_ALLOW_EXT_MEM

TEST_CASE("IDF additions: Task creation with SPIRAM memory caps and self deletion stress test", "[freertos][idf_additions]")
{
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TEST_ਅੰਕ_TASKS      5
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TEST_ਅੰਕ_ITERATIONS 1000
    TaskHandle_t task_handle[TEST_ਅੰਕ_TASKS];
    ਢੇਰ_Type_t *pux_ਢੇਰ_ਬਫਰ;
    StaticTask_t *px_Task_ਬਫਰ;

    ਲਈ (ਪੂਰਨ j = 0; j < TEST_ਅੰਕ_ITERATIONS; j++) {
        ਲਈ (ਪੂਰਨ i = 0; i < TEST_ਅੰਕ_TASKS; i++) {
            // Create a task with caps
            TEST_ASSERT_EQUAL(pdPASS, xTaskCreateWithCaps(task_with_caps_self_delete, "task", 4096, ਨਿਰਾ, UNITY_FREERTOS_ਤਰਜੀਹ, &task_handle[i], MALLOC_CAP_SPIRAM | MALLOC_CAP_8_ਦੋਆਧਾਰੀ_ਅੰਕ));
            // Get the task's memory
            TEST_ASSERT_EQUAL(pdTRUE, xTaskGetStaticBuffers(task_handle[i], &pux_ਢੇਰ_ਬਫਰ, &px_Task_ਬਫਰ));
            TEST_ASSERT(esp_ptr_external_ram(pux_ਢੇਰ_ਬਫਰ));
            TEST_ASSERT(esp_ptr_ਵਿੱਚ_dram(px_Task_ਬਫਰ));
        }

        ਲਈ (ਪੂਰਨ i = 0; i < TEST_ਅੰਕ_TASKS; i++) {
            // Notify the task to delete itself
            xTaskNotifyGive(task_handle[i]);
        }
        // Let the scheduler run the self-delete tasks.
        vTaskDelay(1);
    }
    // Allow the last batch to be freed by the cleanup task in vTaskDeleteWithCaps().
    vTaskDelay(50);
}

#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_TASK_CREATE_ALLOW_EXT_MEM */

#ਜੇ ( CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES > 1 )

ਸਥਿਰ ਖਾਲੀ task_with_caps_running_on_other_core(ਖਾਲੀ *arg)
{
    /* Notify the unity task that this task is running on the other core */
    xTaskNotifyGive((TaskHandle_t)arg);

    /* We make sure that this task is running on the other core */
    ਜਦੋਂ (1) {
        ;
    }
}

TEST_CASE("IDF additions: Task creation with memory caps and deletion from another core", "[freertos][idf_additions]")
{
    TaskHandle_t task_handle = ਨਿਰਾ;
    ਢੇਰ_Type_t *pux_ਢੇਰ_ਬਫਰ;
    StaticTask_t *px_Task_ਬਫਰ;

    // Create a task with caps on the other core
    TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core_With_Caps(task_with_caps_running_on_other_core, "task", 4096, (ਖਾਲੀ *)xTaskGetCurrentTaskHandle(), UNITY_FREERTOS_ਤਰਜੀਹ + 1, &task_handle, !UNITY_FREERTOS_CPU, OBJECT_ਸਮ੍ਰਿਤੀ_CAPS));
    TEST_ASSERT_NOT_EQUAL(ਨਿਰਾ, task_handle);
    // Get the task's memory
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetStaticBuffers(task_handle, &pux_ਢੇਰ_ਬਫਰ, &px_Task_ਬਫਰ));
    TEST_ASSERT(esp_ptr_ਵਿੱਚ_dram(pux_ਢੇਰ_ਬਫਰ));
    TEST_ASSERT(esp_ptr_ਵਿੱਚ_dram(px_Task_ਬਫਰ));
    // Wait for the created task to start running
    ulTaskNotifyTake(pdTRUE, ਪੋਰਟ_MAX_DELAY);
    // Delete the task from another core
    vTaskDeleteWithCaps(task_handle);
}

#ਜੇ_ਅੰਤ // CONFIG_FREERTOS_NUMBER_OF_CORES > 1

TEST_CASE("IDF additions: Queue creation with memory caps", "[freertos][idf_additions]")
{
    ਕਤਾਰ_Handle_t ਕਤਾਰ_handle;
    uint8_t *ਕਤਾਰ_storage;
    Static_ਕਤਾਰ_t *ਕਤਾਰ_obj;

    // Create queue with caps
    ਕਤਾਰ_handle = x_ਕਤਾਰ_Create_With_Caps(5, ਆਕਾਰ(uint32_t), OBJECT_ਸਮ੍ਰਿਤੀ_CAPS);
    TEST_ASSERT_NOT_EQUAL(ਨਿਰਾ, ਕਤਾਰ_handle);
    // Get the queue's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_ਕਤਾਰ_Get_Static_Buffers(ਕਤਾਰ_handle, &ਕਤਾਰ_storage, &ਕਤਾਰ_obj));
    TEST_ASSERT(esp_ptr_ਵਿੱਚ_dram(ਕਤਾਰ_storage));
    TEST_ASSERT(esp_ptr_ਵਿੱਚ_dram(ਕਤਾਰ_obj));
    // Free the queue
    v_ਕਤਾਰ_Delete_With_Caps(ਕਤਾਰ_handle);
}

TEST_CASE("IDF additions: Semaphore creation with memory caps", "[freertos][idf_additions]")
{
    SemaphoreHandle_t sem_handle;
    StaticSemaphore_t *sem_obj;

    // Create binary semaphore with caps
    sem_handle = x_Semaphore_Create_ਦੁਆਧਾਰੀ_With_Caps(OBJECT_ਸਮ੍ਰਿਤੀ_CAPS);
    TEST_ASSERT_NOT_EQUAL(ਨਿਰਾ, sem_handle);
    // Get the binary semaphore's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_Semaphore_Get_Static_ਬਫਰ(sem_handle, &sem_obj));
    TEST_ASSERT(esp_ptr_ਵਿੱਚ_dram(sem_obj));
    // Free the binary semaphore
    vSemaphoreDeleteWithCaps(sem_handle);

    // Create counting semaphore with caps
    sem_handle = xSemaphoreCreateCountingWithCaps(4, 0, OBJECT_ਸਮ੍ਰਿਤੀ_CAPS);
    TEST_ASSERT_NOT_EQUAL(ਨਿਰਾ, sem_handle);
    // Get the counting semaphore's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_Semaphore_Get_Static_ਬਫਰ(sem_handle, &sem_obj));
    TEST_ASSERT(esp_ptr_ਵਿੱਚ_dram(sem_obj));
    // Free the counting semaphore
    vSemaphoreDeleteWithCaps(sem_handle);

    // Create mutex semaphore with caps
    sem_handle = xSemaphoreCreateMutexWithCaps(OBJECT_ਸਮ੍ਰਿਤੀ_CAPS);
    TEST_ASSERT_NOT_EQUAL(ਨਿਰਾ, sem_handle);
    // Get the mutex semaphore's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_Semaphore_Get_Static_ਬਫਰ(sem_handle, &sem_obj));
    TEST_ASSERT(esp_ptr_ਵਿੱਚ_dram(sem_obj));
    // Free the mutex semaphore
    vSemaphoreDeleteWithCaps(sem_handle);

    // Create recursive mutex with caps
    sem_handle = xSemaphoreCreateRecursiveMutexWithCaps(OBJECT_ਸਮ੍ਰਿਤੀ_CAPS);
    TEST_ASSERT_NOT_EQUAL(ਨਿਰਾ, sem_handle);
    // Get the recursive mutex's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_Semaphore_Get_Static_ਬਫਰ(sem_handle, &sem_obj));
    TEST_ASSERT(esp_ptr_ਵਿੱਚ_dram(sem_obj));
    // Free the recursive mutex
    vSemaphoreDeleteWithCaps(sem_handle);
}

TEST_CASE("IDF additions: Stream & message buffer creation with memory caps", "[freertos][idf_additions]")
{
    ਧਾਰਾ_ਬਫਰ_Handle_t ਧਾਰਾ_ਬਫਰ_handle;
    uint8_t *ਧਾਰਾ_ਬਫਰ_storage;
    Static_ਧਾਰਾ_ਬਫਰ_t *ਧਾਰਾ_ਬਫਰ_obj;

    // Create stream buffer with caps
    ਧਾਰਾ_ਬਫਰ_handle = x_ਧਾਰਾ_ਬਫਰ_Create_With_Caps(16, 0, OBJECT_ਸਮ੍ਰਿਤੀ_CAPS);
    TEST_ASSERT_NOT_EQUAL(ਨਿਰਾ, ਧਾਰਾ_ਬਫਰ_handle);
    // Get the stream buffer's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_ਧਾਰਾ_ਬਫਰ_Get_Static_Buffers(ਧਾਰਾ_ਬਫਰ_handle, &ਧਾਰਾ_ਬਫਰ_storage, &ਧਾਰਾ_ਬਫਰ_obj));
    TEST_ASSERT(esp_ptr_ਵਿੱਚ_dram(ਧਾਰਾ_ਬਫਰ_storage));
    TEST_ASSERT(esp_ptr_ਵਿੱਚ_dram(ਧਾਰਾ_ਬਫਰ_obj));
    // Free the stream buffer
    v_ਧਾਰਾ_ਬਫਰ_Delete_With_Caps(ਧਾਰਾ_ਬਫਰ_handle);

    ਸੁਨੇਹਾ_ਬਫਰ_Handle_t ਸੁਨੇਹਾ_ਬਫਰ_handle;
    uint8_t *ਸੁਨੇਹਾ_ਬਫਰ_storage;
    Static_ਸੁਨੇਹਾ_ਬਫਰ_t *ਸੁਨੇਹਾ_ਬਫਰ_obj;

    // Create message buffer with caps
    ਸੁਨੇਹਾ_ਬਫਰ_handle = x_ਸੁਨੇਹਾ_ਬਫਰ_Create_With_Caps(16, OBJECT_ਸਮ੍ਰਿਤੀ_CAPS);
    TEST_ASSERT_NOT_EQUAL(ਨਿਰਾ, ਸੁਨੇਹਾ_ਬਫਰ_handle);
    // Get the message buffer's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_ਸੁਨੇਹਾ_ਬਫਰ_Get_Static_Buffers(ਸੁਨੇਹਾ_ਬਫਰ_handle, &ਸੁਨੇਹਾ_ਬਫਰ_storage, &ਸੁਨੇਹਾ_ਬਫਰ_obj));
    TEST_ASSERT(esp_ptr_ਵਿੱਚ_dram(ਸੁਨੇਹਾ_ਬਫਰ_storage));
    TEST_ASSERT(esp_ptr_ਵਿੱਚ_dram(ਸੁਨੇਹਾ_ਬਫਰ_obj));
    // Free the message buffer
    v_ਸੁਨੇਹਾ_ਬਫਰ_Delete_With_Caps(ਸੁਨੇਹਾ_ਬਫਰ_handle);
}

TEST_CASE("IDF additions: Event group creation with memory caps", "[freertos][idf_additions]")
{
    Event_ਸਮੂਹ_Handle_t evt_ਸਮੂਹ_handle;
    Static_Event_ਸਮੂਹ_t *evt_ਸਮੂਹ_obj;

    // Create event group with caps
    evt_ਸਮੂਹ_handle = x_Event_ਸਮੂਹ_Create_With_Caps(OBJECT_ਸਮ੍ਰਿਤੀ_CAPS);
    TEST_ASSERT_NOT_EQUAL(ਨਿਰਾ, evt_ਸਮੂਹ_handle);
    // Get the event group's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_Event_ਸਮੂਹ_Get_Static_ਬਫਰ(evt_ਸਮੂਹ_handle, &evt_ਸਮੂਹ_obj));
    TEST_ASSERT(esp_ptr_ਵਿੱਚ_dram(evt_ਸਮੂਹ_obj));
    // Free the event group
    v_Event_ਸਮੂਹ_Delete_With_Caps(evt_ਸਮੂਹ_handle);
}

TEST_CASE("IDF additions: WithCaps objects functional smoke test", "[freertos][idf_additions]")
{
    ਕਤਾਰ_Handle_t ਕਤਾਰ_handle;
    uint32_t send_val = 0xA5A5A5A5;
    uint32_t recv_val = 0;

    ਕਤਾਰ_handle = x_ਕਤਾਰ_Create_With_Caps(4, ਆਕਾਰ(uint32_t), OBJECT_ਸਮ੍ਰਿਤੀ_CAPS);
    TEST_ASSERT_NOT_NULL(ਕਤਾਰ_handle);
    TEST_ASSERT_EQUAL(pdTRUE, x_ਕਤਾਰ_Send(ਕਤਾਰ_handle, &send_val, 0));
    TEST_ASSERT_EQUAL(pdTRUE, x_ਕਤਾਰ_Receive(ਕਤਾਰ_handle, &recv_val, 0));
    TEST_ASSERT_EQUAL(send_val, recv_val);
    v_ਕਤਾਰ_Delete_With_Caps(ਕਤਾਰ_handle);

    SemaphoreHandle_t sem_handle = x_Semaphore_Create_ਦੁਆਧਾਰੀ_With_Caps(OBJECT_ਸਮ੍ਰਿਤੀ_CAPS);
    TEST_ASSERT_NOT_NULL(sem_handle);
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(sem_handle));
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(sem_handle, 0));
    vSemaphoreDeleteWithCaps(sem_handle);

    ਧਾਰਾ_ਬਫਰ_Handle_t ਧਾਰਾ_ਬਫਰ_handle;
    uint8_t ਧਾਰਾ_tx[] = {0x01, 0x02, 0x03};
    uint8_t ਧਾਰਾ_rx[ਆਕਾਰ(ਧਾਰਾ_tx)] = {0};

    ਧਾਰਾ_ਬਫਰ_handle = x_ਧਾਰਾ_ਬਫਰ_Create_With_Caps(32, 1, OBJECT_ਸਮ੍ਰਿਤੀ_CAPS);
    TEST_ASSERT_NOT_NULL(ਧਾਰਾ_ਬਫਰ_handle);
    TEST_ASSERT_EQUAL(ਆਕਾਰ(ਧਾਰਾ_tx), x_ਧਾਰਾ_ਬਫਰ_Send(ਧਾਰਾ_ਬਫਰ_handle, ਧਾਰਾ_tx, ਆਕਾਰ(ਧਾਰਾ_tx), 0));
    TEST_ASSERT_EQUAL(ਆਕਾਰ(ਧਾਰਾ_rx), x_ਧਾਰਾ_ਬਫਰ_Receive(ਧਾਰਾ_ਬਫਰ_handle, ਧਾਰਾ_rx, ਆਕਾਰ(ਧਾਰਾ_rx), 0));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ਧਾਰਾ_tx, ਧਾਰਾ_rx, ਆਕਾਰ(ਧਾਰਾ_tx));
    v_ਧਾਰਾ_ਬਫਰ_Delete_With_Caps(ਧਾਰਾ_ਬਫਰ_handle);

    ਸੁਨੇਹਾ_ਬਫਰ_Handle_t ਸੁਨੇਹਾ_ਬਫਰ_handle;

    ਸੁਨੇਹਾ_ਬਫਰ_handle = x_ਸੁਨੇਹਾ_ਬਫਰ_Create_With_Caps(32, OBJECT_ਸਮ੍ਰਿਤੀ_CAPS);
    TEST_ASSERT_NOT_NULL(ਸੁਨੇਹਾ_ਬਫਰ_handle);
    TEST_ASSERT_EQUAL(ਆਕਾਰ(ਧਾਰਾ_tx), x_ਸੁਨੇਹਾ_ਬਫਰ_Send(ਸੁਨੇਹਾ_ਬਫਰ_handle, ਧਾਰਾ_tx, ਆਕਾਰ(ਧਾਰਾ_tx), 0));
    TEST_ASSERT_EQUAL(ਆਕਾਰ(ਧਾਰਾ_rx), x_ਸੁਨੇਹਾ_ਬਫਰ_Receive(ਸੁਨੇਹਾ_ਬਫਰ_handle, ਧਾਰਾ_rx, ਆਕਾਰ(ਧਾਰਾ_rx), 0));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ਧਾਰਾ_tx, ਧਾਰਾ_rx, ਆਕਾਰ(ਧਾਰਾ_tx));
    v_ਸੁਨੇਹਾ_ਬਫਰ_Delete_With_Caps(ਸੁਨੇਹਾ_ਬਫਰ_handle);

    Event_ਸਮੂਹ_Handle_t evt_ਸਮੂਹ_handle = x_Event_ਸਮੂਹ_Create_With_Caps(OBJECT_ਸਮ੍ਰਿਤੀ_CAPS);
    TEST_ASSERT_NOT_NULL(evt_ਸਮੂਹ_handle);
    TEST_ASSERT_ਬਿੱਟ(BIT0, BIT0, x_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ(evt_ਸਮੂਹ_handle, BIT0));
    TEST_ASSERT_ਬਿੱਟ(BIT0, BIT0, x_Event_ਸਮੂਹ_Wait_ਬਿੱਟ(evt_ਸਮੂਹ_handle, BIT0, pdTRUE, pdFALSE, 0));
    v_Event_ਸਮੂਹ_Delete_With_Caps(evt_ਸਮੂਹ_handle);
}

TEST_CASE("IDF additions: WithCaps creation fails on out of memory", "[freertos][idf_additions]")
{
    /* Size just past the total heap for these caps so allocation must fail on every target,
     * without fixed constants that can overflow size_t (e.g. stack_words * sizeof(StackType_t)). */
    ਅਚਲ size_t ਕੁੱਲ_caps = ਢੇਰ_caps_get_ਕੁੱਲ_ਮਾਪ(OBJECT_ਸਮ੍ਰਿਤੀ_CAPS);
    ਅਚਲ size_t oom_ਬਾਈਟਾਂ = ਕੁੱਲ_caps + 1024;
    ਅਚਲ UBaseType_t oom_ਕਤਾਰ_len = (UBaseType_t)((oom_ਬਾਈਟਾਂ / ਆਕਾਰ(uint32_t)) + 1);
    ਅਚਲ config_ਢੇਰ_ਡੂੰਘਾਈ_TYPE oom_ਢੇਰ_ਸ਼ਬਦ =
        (config_ਢੇਰ_ਡੂੰਘਾਈ_TYPE)((oom_ਬਾਈਟਾਂ / ਆਕਾਰ(ਢੇਰ_Type_t)) + 1);
    TaskHandle_t task_handle = ਨਿਰਾ;

    TEST_ASSERT_NULL(x_ਕਤਾਰ_Create_With_Caps(oom_ਕਤਾਰ_len, ਆਕਾਰ(uint32_t), OBJECT_ਸਮ੍ਰਿਤੀ_CAPS));
    TEST_ASSERT_NULL(x_ਧਾਰਾ_ਬਫਰ_Create_With_Caps(oom_ਬਾਈਟਾਂ, 1, OBJECT_ਸਮ੍ਰਿਤੀ_CAPS));
    TEST_ASSERT_NULL(x_ਸੁਨੇਹਾ_ਬਫਰ_Create_With_Caps(oom_ਬਾਈਟਾਂ, OBJECT_ਸਮ੍ਰਿਤੀ_CAPS));
    TEST_ASSERT_EQUAL(pdFAIL, x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core_With_Caps(task_with_caps,
                                                              "oom_task",
                                                              oom_ਢੇਰ_ਸ਼ਬਦ,
                                                              ਨਿਰਾ,
                                                              UNITY_FREERTOS_ਤਰਜੀਹ,
                                                              &task_handle,
                                                              UNITY_FREERTOS_CPU,
                                                              OBJECT_ਸਮ੍ਰਿਤੀ_CAPS));
    TEST_ASSERT_NULL(task_handle);
}

/* Create/delete coverage for remaining WithCaps types; Unity setUp/tearDown leak check verifies heap restore. */
TEST_CASE("IDF additions: WithCaps create delete restores heap", "[freertos][idf_additions]")
{
    ਕਤਾਰ_Handle_t ਕਤਾਰ_handle = x_ਕਤਾਰ_Create_With_Caps(4, ਆਕਾਰ(uint32_t), OBJECT_ਸਮ੍ਰਿਤੀ_CAPS);
    TEST_ASSERT_NOT_NULL(ਕਤਾਰ_handle);
    v_ਕਤਾਰ_Delete_With_Caps(ਕਤਾਰ_handle);

    SemaphoreHandle_t sem_handle = xSemaphoreCreateMutexWithCaps(OBJECT_ਸਮ੍ਰਿਤੀ_CAPS);
    TEST_ASSERT_NOT_NULL(sem_handle);
    vSemaphoreDeleteWithCaps(sem_handle);

    ਧਾਰਾ_ਬਫਰ_Handle_t ਧਾਰਾ_ਬਫਰ_handle = x_ਧਾਰਾ_ਬਫਰ_Create_With_Caps(16, 1, OBJECT_ਸਮ੍ਰਿਤੀ_CAPS);
    TEST_ASSERT_NOT_NULL(ਧਾਰਾ_ਬਫਰ_handle);
    v_ਧਾਰਾ_ਬਫਰ_Delete_With_Caps(ਧਾਰਾ_ਬਫਰ_handle);

    ਸੁਨੇਹਾ_ਬਫਰ_Handle_t ਸੁਨੇਹਾ_ਬਫਰ_handle = x_ਸੁਨੇਹਾ_ਬਫਰ_Create_With_Caps(16, OBJECT_ਸਮ੍ਰਿਤੀ_CAPS);
    TEST_ASSERT_NOT_NULL(ਸੁਨੇਹਾ_ਬਫਰ_handle);
    v_ਸੁਨੇਹਾ_ਬਫਰ_Delete_With_Caps(ਸੁਨੇਹਾ_ਬਫਰ_handle);

    Event_ਸਮੂਹ_Handle_t evt_ਸਮੂਹ_handle = x_Event_ਸਮੂਹ_Create_With_Caps(OBJECT_ਸਮ੍ਰਿਤੀ_CAPS);
    TEST_ASSERT_NOT_NULL(evt_ਸਮੂਹ_handle);
    v_Event_ਸਮੂਹ_Delete_With_Caps(evt_ਸਮੂਹ_handle);
}

#ਜੇ CONFIG_SPIRAM

TEST_CASE("IDF additions: WithCaps objects allocate in SPIRAM when requested", "[freertos][idf_additions]")
{
    /* Queue/stream Static* objects honor SPIRAM caps; task TCBs remain internal via pvPortMalloc. */
    ਕਤਾਰ_Handle_t ਕਤਾਰ_handle;
    uint8_t *ਕਤਾਰ_storage;
    Static_ਕਤਾਰ_t *ਕਤਾਰ_obj;

    ਕਤਾਰ_handle = x_ਕਤਾਰ_Create_With_Caps(4, ਆਕਾਰ(uint32_t), SPIRAM_OBJECT_ਸਮ੍ਰਿਤੀ_CAPS);
    TEST_ASSERT_NOT_NULL(ਕਤਾਰ_handle);
    TEST_ASSERT_EQUAL(pdTRUE, x_ਕਤਾਰ_Get_Static_Buffers(ਕਤਾਰ_handle, &ਕਤਾਰ_storage, &ਕਤਾਰ_obj));
    TEST_ASSERT(esp_ptr_external_ram(ਕਤਾਰ_storage));
    TEST_ASSERT(esp_ptr_external_ram(ਕਤਾਰ_obj));
    v_ਕਤਾਰ_Delete_With_Caps(ਕਤਾਰ_handle);

    ਧਾਰਾ_ਬਫਰ_Handle_t ਧਾਰਾ_ਬਫਰ_handle;
    uint8_t *ਧਾਰਾ_ਬਫਰ_storage;
    Static_ਧਾਰਾ_ਬਫਰ_t *ਧਾਰਾ_ਬਫਰ_obj;

    ਧਾਰਾ_ਬਫਰ_handle = x_ਧਾਰਾ_ਬਫਰ_Create_With_Caps(16, 1, SPIRAM_OBJECT_ਸਮ੍ਰਿਤੀ_CAPS);
    TEST_ASSERT_NOT_NULL(ਧਾਰਾ_ਬਫਰ_handle);
    TEST_ASSERT_EQUAL(pdTRUE, x_ਧਾਰਾ_ਬਫਰ_Get_Static_Buffers(ਧਾਰਾ_ਬਫਰ_handle, &ਧਾਰਾ_ਬਫਰ_storage, &ਧਾਰਾ_ਬਫਰ_obj));
    TEST_ASSERT(esp_ptr_external_ram(ਧਾਰਾ_ਬਫਰ_storage));
    TEST_ASSERT(esp_ptr_external_ram(ਧਾਰਾ_ਬਫਰ_obj));
    v_ਧਾਰਾ_ਬਫਰ_Delete_With_Caps(ਧਾਰਾ_ਬਫਰ_handle);
}

#ਜੇ_ਅੰਤ /* CONFIG_SPIRAM */

/* ---------------------------------------------------------------------------------------------------------------------
 * Task utility API tests (xTaskGetCoreID, xTaskGetStackStart, per-core handle helpers, etc.)
 * --------------------------------------------------------------------------------------------------------------------- */

ਸਥਿਰ ਖਾਲੀ ਢੇਰ_test_task(ਖਾਲੀ *arg)
{
    (ਖਾਲੀ)arg;
    vTaskSuspend(ਨਿਰਾ);
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    TaskHandle_t ਮਾਪਾ;
    ਅਧਾਰ_Type_t expected_core;
    ਅਧਾਰ_Type_t observed_core;
    TaskHandle_t self_handle;
} core_id_query_ctx_t;

ਸਥਿਰ ਖਾਲੀ pinned_core_id_task(ਖਾਲੀ *arg)
{
    core_id_query_ctx_t *ctx = (core_id_query_ctx_t *)arg;

    ctx->self_handle = xTaskGetCurrentTaskHandle();
    ctx->observed_core = x_ਪੋਰਟ_Get_Core_ID();
    TEST_ASSERT_EQUAL(ctx->expected_core, ctx->observed_core);
    TEST_ASSERT_EQUAL(ctx->expected_core, xTaskGetCoreID(ਨਿਰਾ));
    TEST_ASSERT_EQUAL(ctx->expected_core, xTaskGetCoreID(ctx->self_handle));

    xTaskNotifyGive(ctx->ਮਾਪਾ);
    vTaskSuspend(ਨਿਰਾ);
}

TEST_CASE("IDF additions: xTaskGetCoreID for pinned tasks", "[freertos][idf_additions]")
{
    ਅਚਲ TaskHandle_t ਮਾਪਾ = xTaskGetCurrentTaskHandle();
    core_id_query_ctx_t ctx[CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES];

    ਲਈ (ਪੂਰਨ core = 0; core < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; core++) {
        ctx[core].ਮਾਪਾ = ਮਾਪਾ;
        ctx[core].expected_core = core;
        ctx[core].observed_core = -1;
        ctx[core].self_handle = ਨਿਰਾ;

        TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(pinned_core_id_task,
                                                          "core_id",
                                                          2048,
                                                          &ctx[core],
                                                          UNITY_FREERTOS_ਤਰਜੀਹ + 1,
                                                          ਨਿਰਾ,
                                                          core));
    }

    ਲਈ (ਪੂਰਨ core = 0; core < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; core++) {
        TEST_ASSERT_NOT_EQUAL(0, ulTaskNotifyTake(pdFALSE, pd_MS_ਮੰਜ਼ਿਲ_TICKS(1000)));
        TEST_ASSERT_EQUAL(core, ctx[core].observed_core);
        TEST_ASSERT_EQUAL(core, xTaskGetCoreID(ctx[core].self_handle));
    }

    ਲਈ (ਪੂਰਨ core = 0; core < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; core++) {
        vTaskDelete(ctx[core].self_handle);
    }
    vTaskDelay(1);
}

#ਜੇ (CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES > 1) && !CONFIG_FREERTOS_SMP

ਸਥਿਰ SemaphoreHandle_t s_unpinned_ਪੂਰਾ_sem;
ਸਥਿਰ TaskHandle_t s_unpinned_task_handle;

ਸਥਿਰ ਖਾਲੀ unpinned_core_id_task(ਖਾਲੀ *arg)
{
    (ਖਾਲੀ)arg;
    s_unpinned_task_handle = xTaskGetCurrentTaskHandle();
    xSemaphoreGive(s_unpinned_ਪੂਰਾ_sem);
    vTaskSuspend(ਨਿਰਾ);
}

TEST_CASE("IDF additions: xTaskGetCoreID returns tskNO_AFFINITY for unpinned task", "[freertos][idf_additions]")
{
    s_unpinned_ਪੂਰਾ_sem = x_Semaphore_Create_ਦੁਆਧਾਰੀ();
    TEST_ASSERT_NOT_NULL(s_unpinned_ਪੂਰਾ_sem);
    s_unpinned_task_handle = ਨਿਰਾ;

    TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(unpinned_core_id_task,
                                                      "unpinned",
                                                      2048,
                                                      ਨਿਰਾ,
                                                      UNITY_FREERTOS_ਤਰਜੀਹ - 1,
                                                      ਨਿਰਾ,
                                                      tskNO_AFFINITY));
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(s_unpinned_ਪੂਰਾ_sem, pd_MS_ਮੰਜ਼ਿਲ_TICKS(1000)));
    TEST_ASSERT_NOT_NULL(s_unpinned_task_handle);
    TEST_ASSERT_EQUAL(tskNO_AFFINITY, xTaskGetCoreID(s_unpinned_task_handle));

    vTaskDelete(s_unpinned_task_handle);
    vSemaphoreDelete(s_unpinned_ਪੂਰਾ_sem);
    vTaskDelay(1);
}

#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_NUMBER_OF_CORES > 1 && !CONFIG_FREERTOS_SMP */

#ਜੇ CONFIG_FREERTOS_UNICORE

TEST_CASE("IDF additions: xTaskGetCoreID returns 0 on unicore", "[freertos][idf_additions]")
{
    TEST_ASSERT_EQUAL(0, xTaskGetCoreID(ਨਿਰਾ));
    TEST_ASSERT_EQUAL(0, xTaskGetCoreID(xTaskGetCurrentTaskHandle()));
}

#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_UNICORE */

TEST_CASE("IDF additions: xTaskGetStackStart", "[freertos][idf_additions]")
{
    ਢੇਰ_Type_t *current_ਢੇਰ_ਰਾਹੀਂ_null = x_Task_Get_ਢੇਰ_ਸ਼ੁਰੂ(ਨਿਰਾ);
    ਢੇਰ_Type_t *current_ਢੇਰ_ਰਾਹੀਂ_handle = x_Task_Get_ਢੇਰ_ਸ਼ੁਰੂ(xTaskGetCurrentTaskHandle());

    TEST_ASSERT_NOT_NULL(current_ਢੇਰ_ਰਾਹੀਂ_null);
    TEST_ASSERT_EQUAL_PTR(current_ਢੇਰ_ਰਾਹੀਂ_null, current_ਢੇਰ_ਰਾਹੀਂ_handle);

    TaskHandle_t caps_task = ਨਿਰਾ;
    ਢੇਰ_Type_t *caps_ਢੇਰ_ਬਫਰ = ਨਿਰਾ;
    StaticTask_t *caps_task_ਬਫਰ = ਨਿਰਾ;

    TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core_With_Caps(ਢੇਰ_test_task,
                                                              "stack_caps",
                                                              4096,
                                                              ਨਿਰਾ,
                                                              UNITY_FREERTOS_ਤਰਜੀਹ + 1,
                                                              &caps_task,
                                                              UNITY_FREERTOS_CPU,
                                                              OBJECT_ਸਮ੍ਰਿਤੀ_CAPS));
    TEST_ASSERT_NOT_NULL(caps_task);
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetStaticBuffers(caps_task, &caps_ਢੇਰ_ਬਫਰ, &caps_task_ਬਫਰ));
    TEST_ASSERT_EQUAL_PTR(caps_ਢੇਰ_ਬਫਰ, x_Task_Get_ਢੇਰ_ਸ਼ੁਰੂ(caps_task));

    vTaskDeleteWithCaps(caps_task);
    vTaskDelay(10);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STATIC_ਢੇਰ_ਬਾਈਟਾਂ 4096
    ਸਥਿਰ ਢੇਰ_Type_t static_ਢੇਰ[STATIC_ਢੇਰ_ਬਾਈਟਾਂ / ਆਕਾਰ(ਢੇਰ_Type_t)];
    ਸਥਿਰ StaticTask_t static_task_ਬਫਰ;

    memset(&static_task_ਬਫਰ, 0, ਆਕਾਰ(static_task_ਬਫਰ));

    TaskHandle_t static_task = x_Task_Create_Static_Pinned_ਮੰਜ਼ਿਲ_Core(ਢੇਰ_test_task,
                                                             "stack_static",
                                                             STATIC_ਢੇਰ_ਬਾਈਟਾਂ,
                                                             ਨਿਰਾ,
                                                             UNITY_FREERTOS_ਤਰਜੀਹ + 1,
                                                             static_ਢੇਰ,
                                                             &static_task_ਬਫਰ,
                                                             UNITY_FREERTOS_CPU);
    TEST_ASSERT_NOT_NULL(static_task);
    TEST_ASSERT_EQUAL_PTR(static_ਢੇਰ, x_Task_Get_ਢੇਰ_ਸ਼ੁਰੂ(static_task));

    vTaskDelete(static_task);
    vTaskDelay(1);
}

#ਜੇ !CONFIG_FREERTOS_SMP

TEST_CASE("IDF additions: xTaskGetIdleTaskHandleForCore", "[freertos][idf_additions]")
{
    ਲਈ (ਪੂਰਨ core = 0; core < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; core++) {
        TaskHandle_t idle_for_core = xTaskGetIdleTaskHandleForCore(core);
        ਅੱਖਰ expected_name[configMAX_TASK_NAME_LEN];

        TEST_ASSERT_NOT_NULL(idle_for_core);
#ਜੇ CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES > 1
        snprintf(expected_name, ਆਕਾਰ(expected_name), "IDLE%d", core);
#ਨਹੀਂਤਾਂ
        snprintf(expected_name, ਆਕਾਰ(expected_name), "IDLE");
#ਜੇ_ਅੰਤ
        TEST_ASSERT_EQUAL_STRING(expected_name, pcTaskGetName(idle_for_core));
    }

#ਜੇ CONFIG_FREERTOS_UNICORE
    TEST_ASSERT_EQUAL(xTaskGetIdleTaskHandle(), xTaskGetIdleTaskHandleForCore(0));
#ਜੇ_ਅੰਤ
}

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    TaskHandle_t ਮਾਪਾ;
    ਅਧਾਰ_Type_t core;
    TaskHandle_t self_handle;
    TaskHandle_t queried_handle;
    ਖਾਲੀ * queried_tcb;
} ਪ੍ਰਤੀ_core_handle_ctx_t;

ਸਥਿਰ ਖਾਲੀ running_on_core_task(ਖਾਲੀ *arg)
{
    ਪ੍ਰਤੀ_core_handle_ctx_t *ctx = (ਪ੍ਰਤੀ_core_handle_ctx_t *)arg;

    ctx->self_handle = xTaskGetCurrentTaskHandle();
    ctx->queried_handle = xTaskGetCurrentTaskHandleForCore(ctx->core);
    ctx->queried_tcb = pvTaskGetCurrentTCBForCore(ctx->core);

    xTaskNotifyGive(ctx->ਮਾਪਾ);
    vTaskSuspend(ਨਿਰਾ);
}

TEST_CASE("IDF additions: xTaskGetCurrentTaskHandleForCore and pvTaskGetCurrentTCBForCore", "[freertos][idf_additions]")
{
    ਅਚਲ TaskHandle_t ਮਾਪਾ = xTaskGetCurrentTaskHandle();

    ਲਈ (ਪੂਰਨ core = 0; core < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; core++) {
        ਪ੍ਰਤੀ_core_handle_ctx_t ctx = {
            .ਮਾਪਾ = ਮਾਪਾ,
            .core = core,
            .self_handle = ਨਿਰਾ,
            .queried_handle = ਨਿਰਾ,
            .queried_tcb = ਨਿਰਾ,
        };

        ਜਦੋਂ (ulTaskNotifyTake(pdFALSE, 0) != 0) {
            ;
        }

        TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(running_on_core_task,
                                                          "on_core",
                                                          2048,
                                                          &ctx,
                                                          UNITY_FREERTOS_ਤਰਜੀਹ + 1,
                                                          ਨਿਰਾ,
                                                          core));

        TEST_ASSERT_NOT_EQUAL(0, ulTaskNotifyTake(pdFALSE, pd_MS_ਮੰਜ਼ਿਲ_TICKS(1000)));

        TEST_ASSERT_EQUAL_PTR(ctx.self_handle, ctx.queried_handle);
        TEST_ASSERT_EQUAL_PTR((ਖਾਲੀ *)ctx.self_handle, ctx.queried_tcb);

        vTaskDelete(ctx.self_handle);
        vTaskDelay(1);
    }
}

#ਜੇ_ਅੰਤ /* !CONFIG_FREERTOS_SMP */

#ਜੇ CONFIG_FREERTOS_GENERATE_ਲੜੀਵਾਰ_ਸਮਾਂ_STATS && !CONFIG_FREERTOS_SMP

TEST_CASE("IDF additions: ulTaskGetIdleRunTimeCounterForCore", "[freertos][idf_additions]")
{
    ਲਈ (ਪੂਰਨ core = 0; core < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; core++) {
        config_ਲੜੀਵਾਰ_ਸਮਾਂ_COUNTER_TYPE idle_counter = ul_Task_Get_Idle_ਲੜੀਵਾਰ_ਸਮਾਂ_Counter_For_Core(core);
        config_ਲੜੀਵਾਰ_ਸਮਾਂ_COUNTER_TYPE idle_percent = ul_Task_Get_Idle_ਲੜੀਵਾਰ_ਸਮਾਂ_Percent_For_Core(core);

        TEST_ASSERT_GREATER_OR_EQUAL(0, idle_counter);
        TEST_ASSERT_LESS_OR_EQUAL(100, idle_percent);
    }
}

#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS && !CONFIG_FREERTOS_SMP */

/* ---------------------------------------------------------------------------------------------------------------------
 * xTaskCreatePinnedToCore / xTaskCreateStaticPinnedToCore pinning semantics
 * --------------------------------------------------------------------------------------------------------------------- */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PIN_SEMANTICS_SAMPLES     5
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PIN_SEMANTICS_ਢੇਰ_ਬਾਈਟਾਂ 4096

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    TaskHandle_t ਮਾਪਾ;
    ਅਧਾਰ_Type_t requested_core;
    ਅਧਾਰ_Type_t samples[PIN_SEMANTICS_SAMPLES];
    TaskHandle_t self_handle;
} pin_semantics_ctx_t;

ਸਥਿਰ ਖਾਲੀ drain_task_notifications(ਖਾਲੀ)
{
    ਜਦੋਂ (ulTaskNotifyTake(pdFALSE, 0) != 0) {
        ;
    }
}

ਸਥਿਰ ਖਾਲੀ pin_semantics_task(ਖਾਲੀ *arg)
{
    pin_semantics_ctx_t *ctx = (pin_semantics_ctx_t *)arg;

    ctx->self_handle = xTaskGetCurrentTaskHandle();
    ਲਈ (ਪੂਰਨ i = 0; i < PIN_SEMANTICS_SAMPLES; i++) {
        ctx->samples[i] = x_ਪੋਰਟ_Get_Core_ID();
        ਜੇ (ctx->requested_core != tskNO_AFFINITY) {
            TEST_ASSERT_EQUAL(ctx->requested_core, ctx->samples[i]);
        } ਨਹੀਂਤਾਂ {
            TEST_ASSERT_GREATER_OR_EQUAL(0, ctx->samples[i]);
            TEST_ASSERT_LESS_THAN(CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES, ctx->samples[i]);
        }
        /* Yield and sleep so a wrongly pinned task would have a chance to migrate. */
        taskYIELD();
        vTaskDelay(1);
    }

    xTaskNotifyGive(ctx->ਮਾਪਾ);
    vTaskSuspend(ਨਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ verify_pin_samples(ਅਚਲ pin_semantics_ctx_t *ctx)
{
    ਲਈ (ਪੂਰਨ i = 0; i < PIN_SEMANTICS_SAMPLES; i++) {
        ਜੇ (ctx->requested_core == tskNO_AFFINITY) {
            TEST_ASSERT_GREATER_OR_EQUAL(0, ctx->samples[i]);
            TEST_ASSERT_LESS_THAN(CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES, ctx->samples[i]);
        } ਨਹੀਂਤਾਂ {
            TEST_ASSERT_EQUAL(ctx->requested_core, ctx->samples[i]);
        }
    }
}

TEST_CASE("IDF additions: xTaskCreatePinnedToCore pinning semantics", "[freertos][idf_additions]")
{
    ਅਚਲ TaskHandle_t ਮਾਪਾ = xTaskGetCurrentTaskHandle();

    ਲਈ (ਪੂਰਨ core = 0; core < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; core++) {
        pin_semantics_ctx_t ctx = {
            .ਮਾਪਾ = ਮਾਪਾ,
            .requested_core = core,
            .self_handle = ਨਿਰਾ,
        };
        TaskHandle_t created = ਨਿਰਾ;

        drain_task_notifications();

        TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(pin_semantics_task,
                                                          "pin_dyn",
                                                          PIN_SEMANTICS_ਢੇਰ_ਬਾਈਟਾਂ,
                                                          &ctx,
                                                          UNITY_FREERTOS_ਤਰਜੀਹ + 1,
                                                          &created,
                                                          core));
        TEST_ASSERT_NOT_NULL(created);
        TEST_ASSERT_NOT_EQUAL(0, ulTaskNotifyTake(pdFALSE, pd_MS_ਮੰਜ਼ਿਲ_TICKS(1000)));
        TEST_ASSERT_EQUAL_PTR(created, ctx.self_handle);
        TEST_ASSERT_EQUAL(core, xTaskGetCoreID(created));
        verify_pin_samples(&ctx);

        vTaskDelete(created);
        vTaskDelay(1);
    }
}

TEST_CASE("IDF additions: xTaskCreateStaticPinnedToCore pinning semantics", "[freertos][idf_additions]")
{
    ਸਥਿਰ ਢੇਰ_Type_t pin_static_ਢੇਰ[PIN_SEMANTICS_ਢੇਰ_ਬਾਈਟਾਂ / ਆਕਾਰ(ਢੇਰ_Type_t)];
    ਸਥਿਰ StaticTask_t pin_static_tcb;
    ਅਚਲ TaskHandle_t ਮਾਪਾ = xTaskGetCurrentTaskHandle();

    ਲਈ (ਪੂਰਨ core = 0; core < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; core++) {
        pin_semantics_ctx_t ctx = {
            .ਮਾਪਾ = ਮਾਪਾ,
            .requested_core = core,
            .self_handle = ਨਿਰਾ,
        };

        memset(&pin_static_tcb, 0, ਆਕਾਰ(pin_static_tcb));
        drain_task_notifications();

        TaskHandle_t created = x_Task_Create_Static_Pinned_ਮੰਜ਼ਿਲ_Core(pin_semantics_task,
                                                             "pin_static",
                                                             PIN_SEMANTICS_ਢੇਰ_ਬਾਈਟਾਂ,
                                                             &ctx,
                                                             UNITY_FREERTOS_ਤਰਜੀਹ + 1,
                                                             pin_static_ਢੇਰ,
                                                             &pin_static_tcb,
                                                             core);
        TEST_ASSERT_NOT_NULL(created);
        TEST_ASSERT_NOT_EQUAL(0, ulTaskNotifyTake(pdFALSE, pd_MS_ਮੰਜ਼ਿਲ_TICKS(1000)));
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
        .ਮਾਪਾ = xTaskGetCurrentTaskHandle(),
        .requested_core = tskNO_AFFINITY,
        .self_handle = ਨਿਰਾ,
    };
    TaskHandle_t created = ਨਿਰਾ;

    drain_task_notifications();

    TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(pin_semantics_task,
                                                      "pin_any",
                                                      PIN_SEMANTICS_ਢੇਰ_ਬਾਈਟਾਂ,
                                                      &ctx,
                                                      UNITY_FREERTOS_ਤਰਜੀਹ + 1,
                                                      &created,
                                                      tskNO_AFFINITY));
    TEST_ASSERT_NOT_NULL(created);
    TEST_ASSERT_NOT_EQUAL(0, ulTaskNotifyTake(pdFALSE, pd_MS_ਮੰਜ਼ਿਲ_TICKS(1000)));
    TEST_ASSERT_EQUAL_PTR(created, ctx.self_handle);
    verify_pin_samples(&ctx);

#ਜੇ CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES > 1
    TEST_ASSERT_EQUAL(tskNO_AFFINITY, xTaskGetCoreID(created));
#ਨਹੀਂਤਾਂ
    /* Unicore treats tskNO_AFFINITY as core 0. */
    TEST_ASSERT_EQUAL(0, xTaskGetCoreID(created));
    TEST_ASSERT_EQUAL(0, ctx.samples[0]);
#ਜੇ_ਅੰਤ

    vTaskDelete(created);
    vTaskDelay(1);
}
