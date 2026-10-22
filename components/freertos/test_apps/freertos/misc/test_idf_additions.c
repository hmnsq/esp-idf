/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ "freertos/queue.h"
#ಅನ್ತರ್ಭಾವ "freertos/semphr.h"
#ಅನ್ತರ್ಭಾವ "freertos/stream_buffer.h"
#ಅನ್ತರ್ಭಾವ "freertos/message_buffer.h"
#ಅನ್ತರ್ಭಾವ "freertos/event_groups.h"
#ಅನ್ತರ್ಭಾವ "freertos/idf_additions.h"
#ಅನ್ತರ್ಭಾವ "freertos/freertos_debug.h"
#ಅನ್ತರ್ಭಾವ "esp_memory_utils.h"
#ಅನ್ತರ್ಭಾವ "esp_heap_caps.h"
#ಅನ್ತರ್ಭಾವ "unity.h"
#ಅನ್ತರ್ಭಾವ "test_utils.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <inttypes.h>

/*
Test ESP-IDF FreeRTOS API additions from idf_additions.h:
    - ...Create...WithCaps() / ...DeleteWithCaps() memory capability helpers
    - xTaskCreatePinnedToCore() / xTaskCreateStaticPinnedToCore() pinning semantics
    - Task utility helpers (xTaskGetCoreID, xTaskGetStackStart, per-core handle queries, etc.)

Run only these cases from the Unity menu with the [idf_additions] tag filter.
 */

#ವ್ಯಾಖ್ಯಾನಿಸು OBJECT_ಸ್ಮರಣೆ_CAPS      (MALLOC_CAP_ಆಂತರಿಕ|MALLOC_CAP_8_ಬಿಟ್)
#ವ್ಯಾಖ್ಯಾನಿಸು SPIRAM_OBJECT_ಸ್ಮರಣೆ_CAPS (MALLOC_CAP_SPIRAM|MALLOC_CAP_8_ಬಿಟ್)

ಸ್ಥಿರ ಶೂನ್ಯ task_with_caps(ಶೂನ್ಯ *arg)
{
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskSuspend(ಖಾಲಿ);
}

ಸ್ಥಿರ ಶೂನ್ಯ task_with_caps_self_delete(ಶೂನ್ಯ *arg)
{
    /* Wait for the unity task to indicate that this task should delete itself */
    ulTaskNotifyTake(pdTRUE, ಪೋರ್ಟ್_MAX_DELAY);

    /* Although it is not recommended to self-delete a task with memory caps but this
     * is done intentionally to test for memory leaks */
    vTaskDeleteWithCaps(ಖಾಲಿ);
}

TEST_CASE("IDF additions: Task creation with memory caps and deletion from another task", "[freertos][idf_additions]")
{
    TaskHandle_t task_handle = ಖಾಲಿ;
    ರಾಶಿ_Type_t *pux_ರಾಶಿ_ಬಫರ್;
    StaticTask_t *px_Task_ಬಫರ್;

    // Create a task with caps
    TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_ಗಮ್ಯ_Core_With_Caps(task_with_caps, "task", 4096, (ಶೂನ್ಯ *)xTaskGetCurrentTaskHandle(), UNITY_FREERTOS_ಆದ್ಯತೆ + 1, &task_handle, UNITY_FREERTOS_CPU, OBJECT_ಸ್ಮರಣೆ_CAPS));
    TEST_ASSERT_NOT_EQUAL(ಖಾಲಿ, task_handle);
    // Get the task's memory
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetStaticBuffers(task_handle, &pux_ರಾಶಿ_ಬಫರ್, &px_Task_ಬಫರ್));
    TEST_ASSERT(esp_ptr_ಒಳ_dram(pux_ರಾಶಿ_ಬಫರ್));
    TEST_ASSERT(esp_ptr_ಒಳ_dram(px_Task_ಬಫರ್));
    // Wait for the created task to block
    ulTaskNotifyTake(pdTRUE, ಪೋರ್ಟ್_MAX_DELAY);
    // Delete the task
    vTaskDeleteWithCaps(task_handle);
}

TEST_CASE("IDF additions: Task creation with memory caps and self deletion", "[freertos][idf_additions]")
{
    TaskHandle_t task_handle = ಖಾಲಿ;
    ರಾಶಿ_Type_t *pux_ರಾಶಿ_ಬಫರ್;
    StaticTask_t *px_Task_ಬಫರ್;

    // Create a task with caps
    TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_ಗಮ್ಯ_Core_With_Caps(task_with_caps_self_delete, "task", 4096, (ಶೂನ್ಯ *)xTaskGetCurrentTaskHandle(), UNITY_FREERTOS_ಆದ್ಯತೆ + 1, &task_handle, UNITY_FREERTOS_CPU, OBJECT_ಸ್ಮರಣೆ_CAPS));
    TEST_ASSERT_NOT_EQUAL(ಖಾಲಿ, task_handle);
    // Get the task's memory
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetStaticBuffers(task_handle, &pux_ರಾಶಿ_ಬಫರ್, &px_Task_ಬಫರ್));
    TEST_ASSERT(esp_ptr_ಒಳ_dram(pux_ರಾಶಿ_ಬಫರ್));
    TEST_ASSERT(esp_ptr_ಒಳ_dram(px_Task_ಬಫರ್));
    // Notify the task to delete itself
    xTaskNotifyGive(task_handle);
}

#ಆದರೆ CONFIG_FREERTOS_TASK_CREATE_ALLOW_EXT_MEM

TEST_CASE("IDF additions: Task creation with SPIRAM memory caps and self deletion stress test", "[freertos][idf_additions]")
{
#ವ್ಯಾಖ್ಯಾನಿಸು TEST_ಅಂಕೆ_TASKS      5
#ವ್ಯಾಖ್ಯಾನಿಸು TEST_ಅಂಕೆ_ITERATIONS 1000
    TaskHandle_t task_handle[TEST_ಅಂಕೆ_TASKS];
    ರಾಶಿ_Type_t *pux_ರಾಶಿ_ಬಫರ್;
    StaticTask_t *px_Task_ಬಫರ್;

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < TEST_ಅಂಕೆ_ITERATIONS; j++) {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < TEST_ಅಂಕೆ_TASKS; i++) {
            // Create a task with caps
            TEST_ASSERT_EQUAL(pdPASS, xTaskCreateWithCaps(task_with_caps_self_delete, "task", 4096, ಖಾಲಿ, UNITY_FREERTOS_ಆದ್ಯತೆ, &task_handle[i], MALLOC_CAP_SPIRAM | MALLOC_CAP_8_ಬಿಟ್));
            // Get the task's memory
            TEST_ASSERT_EQUAL(pdTRUE, xTaskGetStaticBuffers(task_handle[i], &pux_ರಾಶಿ_ಬಫರ್, &px_Task_ಬಫರ್));
            TEST_ASSERT(esp_ptr_external_ram(pux_ರಾಶಿ_ಬಫರ್));
            TEST_ASSERT(esp_ptr_ಒಳ_dram(px_Task_ಬಫರ್));
        }

        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < TEST_ಅಂಕೆ_TASKS; i++) {
            // Notify the task to delete itself
            xTaskNotifyGive(task_handle[i]);
        }
        // Let the scheduler run the self-delete tasks.
        vTaskDelay(1);
    }
    // Allow the last batch to be freed by the cleanup task in vTaskDeleteWithCaps().
    vTaskDelay(50);
}

#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_TASK_CREATE_ALLOW_EXT_MEM */

#ಆದರೆ ( CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES > 1 )

ಸ್ಥಿರ ಶೂನ್ಯ task_with_caps_running_on_other_core(ಶೂನ್ಯ *arg)
{
    /* Notify the unity task that this task is running on the other core */
    xTaskNotifyGive((TaskHandle_t)arg);

    /* We make sure that this task is running on the other core */
    ಆಗ (1) {
        ;
    }
}

TEST_CASE("IDF additions: Task creation with memory caps and deletion from another core", "[freertos][idf_additions]")
{
    TaskHandle_t task_handle = ಖಾಲಿ;
    ರಾಶಿ_Type_t *pux_ರಾಶಿ_ಬಫರ್;
    StaticTask_t *px_Task_ಬಫರ್;

    // Create a task with caps on the other core
    TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_ಗಮ್ಯ_Core_With_Caps(task_with_caps_running_on_other_core, "task", 4096, (ಶೂನ್ಯ *)xTaskGetCurrentTaskHandle(), UNITY_FREERTOS_ಆದ್ಯತೆ + 1, &task_handle, !UNITY_FREERTOS_CPU, OBJECT_ಸ್ಮರಣೆ_CAPS));
    TEST_ASSERT_NOT_EQUAL(ಖಾಲಿ, task_handle);
    // Get the task's memory
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetStaticBuffers(task_handle, &pux_ರಾಶಿ_ಬಫರ್, &px_Task_ಬಫರ್));
    TEST_ASSERT(esp_ptr_ಒಳ_dram(pux_ರಾಶಿ_ಬಫರ್));
    TEST_ASSERT(esp_ptr_ಒಳ_dram(px_Task_ಬಫರ್));
    // Wait for the created task to start running
    ulTaskNotifyTake(pdTRUE, ಪೋರ್ಟ್_MAX_DELAY);
    // Delete the task from another core
    vTaskDeleteWithCaps(task_handle);
}

#ಆದರೆ_ಅಂತ್ಯ // CONFIG_FREERTOS_NUMBER_OF_CORES > 1

TEST_CASE("IDF additions: Queue creation with memory caps", "[freertos][idf_additions]")
{
    ಸರತಿ_Handle_t ಸರತಿ_handle;
    uint8_t *ಸರತಿ_storage;
    Static_ಸರತಿ_t *ಸರತಿ_obj;

    // Create queue with caps
    ಸರತಿ_handle = x_ಸರತಿ_Create_With_Caps(5, ಗಾತ್ರ(uint32_t), OBJECT_ಸ್ಮರಣೆ_CAPS);
    TEST_ASSERT_NOT_EQUAL(ಖಾಲಿ, ಸರತಿ_handle);
    // Get the queue's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_ಸರತಿ_Get_Static_Buffers(ಸರತಿ_handle, &ಸರತಿ_storage, &ಸರತಿ_obj));
    TEST_ASSERT(esp_ptr_ಒಳ_dram(ಸರತಿ_storage));
    TEST_ASSERT(esp_ptr_ಒಳ_dram(ಸರತಿ_obj));
    // Free the queue
    v_ಸರತಿ_Delete_With_Caps(ಸರತಿ_handle);
}

TEST_CASE("IDF additions: Semaphore creation with memory caps", "[freertos][idf_additions]")
{
    SemaphoreHandle_t sem_handle;
    StaticSemaphore_t *sem_obj;

    // Create binary semaphore with caps
    sem_handle = x_Semaphore_Create_ದ್ವಿಮಾನ_With_Caps(OBJECT_ಸ್ಮರಣೆ_CAPS);
    TEST_ASSERT_NOT_EQUAL(ಖಾಲಿ, sem_handle);
    // Get the binary semaphore's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_Semaphore_Get_Static_ಬಫರ್(sem_handle, &sem_obj));
    TEST_ASSERT(esp_ptr_ಒಳ_dram(sem_obj));
    // Free the binary semaphore
    vSemaphoreDeleteWithCaps(sem_handle);

    // Create counting semaphore with caps
    sem_handle = xSemaphoreCreateCountingWithCaps(4, 0, OBJECT_ಸ್ಮರಣೆ_CAPS);
    TEST_ASSERT_NOT_EQUAL(ಖಾಲಿ, sem_handle);
    // Get the counting semaphore's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_Semaphore_Get_Static_ಬಫರ್(sem_handle, &sem_obj));
    TEST_ASSERT(esp_ptr_ಒಳ_dram(sem_obj));
    // Free the counting semaphore
    vSemaphoreDeleteWithCaps(sem_handle);

    // Create mutex semaphore with caps
    sem_handle = xSemaphoreCreateMutexWithCaps(OBJECT_ಸ್ಮರಣೆ_CAPS);
    TEST_ASSERT_NOT_EQUAL(ಖಾಲಿ, sem_handle);
    // Get the mutex semaphore's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_Semaphore_Get_Static_ಬಫರ್(sem_handle, &sem_obj));
    TEST_ASSERT(esp_ptr_ಒಳ_dram(sem_obj));
    // Free the mutex semaphore
    vSemaphoreDeleteWithCaps(sem_handle);

    // Create recursive mutex with caps
    sem_handle = xSemaphoreCreateRecursiveMutexWithCaps(OBJECT_ಸ್ಮರಣೆ_CAPS);
    TEST_ASSERT_NOT_EQUAL(ಖಾಲಿ, sem_handle);
    // Get the recursive mutex's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_Semaphore_Get_Static_ಬಫರ್(sem_handle, &sem_obj));
    TEST_ASSERT(esp_ptr_ಒಳ_dram(sem_obj));
    // Free the recursive mutex
    vSemaphoreDeleteWithCaps(sem_handle);
}

TEST_CASE("IDF additions: Stream & message buffer creation with memory caps", "[freertos][idf_additions]")
{
    ಹರಿವು_ಬಫರ್_Handle_t ಹರಿವು_ಬಫರ್_handle;
    uint8_t *ಹರಿವು_ಬಫರ್_storage;
    Static_ಹರಿವು_ಬಫರ್_t *ಹರಿವು_ಬಫರ್_obj;

    // Create stream buffer with caps
    ಹರಿವು_ಬಫರ್_handle = x_ಹರಿವು_ಬಫರ್_Create_With_Caps(16, 0, OBJECT_ಸ್ಮರಣೆ_CAPS);
    TEST_ASSERT_NOT_EQUAL(ಖಾಲಿ, ಹರಿವು_ಬಫರ್_handle);
    // Get the stream buffer's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_ಹರಿವು_ಬಫರ್_Get_Static_Buffers(ಹರಿವು_ಬಫರ್_handle, &ಹರಿವು_ಬಫರ್_storage, &ಹರಿವು_ಬಫರ್_obj));
    TEST_ASSERT(esp_ptr_ಒಳ_dram(ಹರಿವು_ಬಫರ್_storage));
    TEST_ASSERT(esp_ptr_ಒಳ_dram(ಹರಿವು_ಬಫರ್_obj));
    // Free the stream buffer
    v_ಹರಿವು_ಬಫರ್_Delete_With_Caps(ಹರಿವು_ಬಫರ್_handle);

    ಸಂದೇಶ_ಬಫರ್_Handle_t ಸಂದೇಶ_ಬಫರ್_handle;
    uint8_t *ಸಂದೇಶ_ಬಫರ್_storage;
    Static_ಸಂದೇಶ_ಬಫರ್_t *ಸಂದೇಶ_ಬಫರ್_obj;

    // Create message buffer with caps
    ಸಂದೇಶ_ಬಫರ್_handle = x_ಸಂದೇಶ_ಬಫರ್_Create_With_Caps(16, OBJECT_ಸ್ಮರಣೆ_CAPS);
    TEST_ASSERT_NOT_EQUAL(ಖಾಲಿ, ಸಂದೇಶ_ಬಫರ್_handle);
    // Get the message buffer's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_ಸಂದೇಶ_ಬಫರ್_Get_Static_Buffers(ಸಂದೇಶ_ಬಫರ್_handle, &ಸಂದೇಶ_ಬಫರ್_storage, &ಸಂದೇಶ_ಬಫರ್_obj));
    TEST_ASSERT(esp_ptr_ಒಳ_dram(ಸಂದೇಶ_ಬಫರ್_storage));
    TEST_ASSERT(esp_ptr_ಒಳ_dram(ಸಂದೇಶ_ಬಫರ್_obj));
    // Free the message buffer
    v_ಸಂದೇಶ_ಬಫರ್_Delete_With_Caps(ಸಂದೇಶ_ಬಫರ್_handle);
}

TEST_CASE("IDF additions: Event group creation with memory caps", "[freertos][idf_additions]")
{
    Event_ಗುಂಪು_Handle_t evt_ಗುಂಪು_handle;
    Static_Event_ಗುಂಪು_t *evt_ಗುಂಪು_obj;

    // Create event group with caps
    evt_ಗುಂಪು_handle = x_Event_ಗುಂಪು_Create_With_Caps(OBJECT_ಸ್ಮರಣೆ_CAPS);
    TEST_ASSERT_NOT_EQUAL(ಖಾಲಿ, evt_ಗುಂಪು_handle);
    // Get the event group's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_Event_ಗುಂಪು_Get_Static_ಬಫರ್(evt_ಗುಂಪು_handle, &evt_ಗುಂಪು_obj));
    TEST_ASSERT(esp_ptr_ಒಳ_dram(evt_ಗುಂಪು_obj));
    // Free the event group
    v_Event_ಗುಂಪು_Delete_With_Caps(evt_ಗುಂಪು_handle);
}

TEST_CASE("IDF additions: WithCaps objects functional smoke test", "[freertos][idf_additions]")
{
    ಸರತಿ_Handle_t ಸರತಿ_handle;
    uint32_t send_val = 0xA5A5A5A5;
    uint32_t recv_val = 0;

    ಸರತಿ_handle = x_ಸರತಿ_Create_With_Caps(4, ಗಾತ್ರ(uint32_t), OBJECT_ಸ್ಮರಣೆ_CAPS);
    TEST_ASSERT_NOT_NULL(ಸರತಿ_handle);
    TEST_ASSERT_EQUAL(pdTRUE, x_ಸರತಿ_Send(ಸರತಿ_handle, &send_val, 0));
    TEST_ASSERT_EQUAL(pdTRUE, x_ಸರತಿ_Receive(ಸರತಿ_handle, &recv_val, 0));
    TEST_ASSERT_EQUAL(send_val, recv_val);
    v_ಸರತಿ_Delete_With_Caps(ಸರತಿ_handle);

    SemaphoreHandle_t sem_handle = x_Semaphore_Create_ದ್ವಿಮಾನ_With_Caps(OBJECT_ಸ್ಮರಣೆ_CAPS);
    TEST_ASSERT_NOT_NULL(sem_handle);
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(sem_handle));
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(sem_handle, 0));
    vSemaphoreDeleteWithCaps(sem_handle);

    ಹರಿವು_ಬಫರ್_Handle_t ಹರಿವು_ಬಫರ್_handle;
    uint8_t ಹರಿವು_tx[] = {0x01, 0x02, 0x03};
    uint8_t ಹರಿವು_rx[ಗಾತ್ರ(ಹರಿವು_tx)] = {0};

    ಹರಿವು_ಬಫರ್_handle = x_ಹರಿವು_ಬಫರ್_Create_With_Caps(32, 1, OBJECT_ಸ್ಮರಣೆ_CAPS);
    TEST_ASSERT_NOT_NULL(ಹರಿವು_ಬಫರ್_handle);
    TEST_ASSERT_EQUAL(ಗಾತ್ರ(ಹರಿವು_tx), x_ಹರಿವು_ಬಫರ್_Send(ಹರಿವು_ಬಫರ್_handle, ಹರಿವು_tx, ಗಾತ್ರ(ಹರಿವು_tx), 0));
    TEST_ASSERT_EQUAL(ಗಾತ್ರ(ಹರಿವು_rx), x_ಹರಿವು_ಬಫರ್_Receive(ಹರಿವು_ಬಫರ್_handle, ಹರಿವು_rx, ಗಾತ್ರ(ಹರಿವು_rx), 0));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ಹರಿವು_tx, ಹರಿವು_rx, ಗಾತ್ರ(ಹರಿವು_tx));
    v_ಹರಿವು_ಬಫರ್_Delete_With_Caps(ಹರಿವು_ಬಫರ್_handle);

    ಸಂದೇಶ_ಬಫರ್_Handle_t ಸಂದೇಶ_ಬಫರ್_handle;

    ಸಂದೇಶ_ಬಫರ್_handle = x_ಸಂದೇಶ_ಬಫರ್_Create_With_Caps(32, OBJECT_ಸ್ಮರಣೆ_CAPS);
    TEST_ASSERT_NOT_NULL(ಸಂದೇಶ_ಬಫರ್_handle);
    TEST_ASSERT_EQUAL(ಗಾತ್ರ(ಹರಿವು_tx), x_ಸಂದೇಶ_ಬಫರ್_Send(ಸಂದೇಶ_ಬಫರ್_handle, ಹರಿವು_tx, ಗಾತ್ರ(ಹರಿವು_tx), 0));
    TEST_ASSERT_EQUAL(ಗಾತ್ರ(ಹರಿವು_rx), x_ಸಂದೇಶ_ಬಫರ್_Receive(ಸಂದೇಶ_ಬಫರ್_handle, ಹರಿವು_rx, ಗಾತ್ರ(ಹರಿವು_rx), 0));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ಹರಿವು_tx, ಹರಿವು_rx, ಗಾತ್ರ(ಹರಿವು_tx));
    v_ಸಂದೇಶ_ಬಫರ್_Delete_With_Caps(ಸಂದೇಶ_ಬಫರ್_handle);

    Event_ಗುಂಪು_Handle_t evt_ಗುಂಪು_handle = x_Event_ಗುಂಪು_Create_With_Caps(OBJECT_ಸ್ಮರಣೆ_CAPS);
    TEST_ASSERT_NOT_NULL(evt_ಗುಂಪು_handle);
    TEST_ASSERT_ಬಿಟ್ಟುಗಳು(BIT0, BIT0, x_Event_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು(evt_ಗುಂಪು_handle, BIT0));
    TEST_ASSERT_ಬಿಟ್ಟುಗಳು(BIT0, BIT0, x_Event_ಗುಂಪು_Wait_ಬಿಟ್ಟುಗಳು(evt_ಗುಂಪು_handle, BIT0, pdTRUE, pdFALSE, 0));
    v_Event_ಗುಂಪು_Delete_With_Caps(evt_ಗುಂಪು_handle);
}

TEST_CASE("IDF additions: WithCaps creation fails on out of memory", "[freertos][idf_additions]")
{
    /* Size just past the total heap for these caps so allocation must fail on every target,
     * without fixed constants that can overflow size_t (e.g. stack_words * sizeof(StackType_t)). */
    ಸ್ಥಿರಾಂಕ size_t ಒಟ್ಟು_caps = ರಾಶಿ_caps_get_ಒಟ್ಟು_ಗಾತ್ರ(OBJECT_ಸ್ಮರಣೆ_CAPS);
    ಸ್ಥಿರಾಂಕ size_t oom_ಬೈಟುಗಳು = ಒಟ್ಟು_caps + 1024;
    ಸ್ಥಿರಾಂಕ UBaseType_t oom_ಸರತಿ_len = (UBaseType_t)((oom_ಬೈಟುಗಳು / ಗಾತ್ರ(uint32_t)) + 1);
    ಸ್ಥಿರಾಂಕ config_ರಾಶಿ_ಆಳ_TYPE oom_ರಾಶಿ_ಪದಗಳು =
        (config_ರಾಶಿ_ಆಳ_TYPE)((oom_ಬೈಟುಗಳು / ಗಾತ್ರ(ರಾಶಿ_Type_t)) + 1);
    TaskHandle_t task_handle = ಖಾಲಿ;

    TEST_ASSERT_NULL(x_ಸರತಿ_Create_With_Caps(oom_ಸರತಿ_len, ಗಾತ್ರ(uint32_t), OBJECT_ಸ್ಮರಣೆ_CAPS));
    TEST_ASSERT_NULL(x_ಹರಿವು_ಬಫರ್_Create_With_Caps(oom_ಬೈಟುಗಳು, 1, OBJECT_ಸ್ಮರಣೆ_CAPS));
    TEST_ASSERT_NULL(x_ಸಂದೇಶ_ಬಫರ್_Create_With_Caps(oom_ಬೈಟುಗಳು, OBJECT_ಸ್ಮರಣೆ_CAPS));
    TEST_ASSERT_EQUAL(pdFAIL, x_Task_Create_Pinned_ಗಮ್ಯ_Core_With_Caps(task_with_caps,
                                                              "oom_task",
                                                              oom_ರಾಶಿ_ಪದಗಳು,
                                                              ಖಾಲಿ,
                                                              UNITY_FREERTOS_ಆದ್ಯತೆ,
                                                              &task_handle,
                                                              UNITY_FREERTOS_CPU,
                                                              OBJECT_ಸ್ಮರಣೆ_CAPS));
    TEST_ASSERT_NULL(task_handle);
}

/* Create/delete coverage for remaining WithCaps types; Unity setUp/tearDown leak check verifies heap restore. */
TEST_CASE("IDF additions: WithCaps create delete restores heap", "[freertos][idf_additions]")
{
    ಸರತಿ_Handle_t ಸರತಿ_handle = x_ಸರತಿ_Create_With_Caps(4, ಗಾತ್ರ(uint32_t), OBJECT_ಸ್ಮರಣೆ_CAPS);
    TEST_ASSERT_NOT_NULL(ಸರತಿ_handle);
    v_ಸರತಿ_Delete_With_Caps(ಸರತಿ_handle);

    SemaphoreHandle_t sem_handle = xSemaphoreCreateMutexWithCaps(OBJECT_ಸ್ಮರಣೆ_CAPS);
    TEST_ASSERT_NOT_NULL(sem_handle);
    vSemaphoreDeleteWithCaps(sem_handle);

    ಹರಿವು_ಬಫರ್_Handle_t ಹರಿವು_ಬಫರ್_handle = x_ಹರಿವು_ಬಫರ್_Create_With_Caps(16, 1, OBJECT_ಸ್ಮರಣೆ_CAPS);
    TEST_ASSERT_NOT_NULL(ಹರಿವು_ಬಫರ್_handle);
    v_ಹರಿವು_ಬಫರ್_Delete_With_Caps(ಹರಿವು_ಬಫರ್_handle);

    ಸಂದೇಶ_ಬಫರ್_Handle_t ಸಂದೇಶ_ಬಫರ್_handle = x_ಸಂದೇಶ_ಬಫರ್_Create_With_Caps(16, OBJECT_ಸ್ಮರಣೆ_CAPS);
    TEST_ASSERT_NOT_NULL(ಸಂದೇಶ_ಬಫರ್_handle);
    v_ಸಂದೇಶ_ಬಫರ್_Delete_With_Caps(ಸಂದೇಶ_ಬಫರ್_handle);

    Event_ಗುಂಪು_Handle_t evt_ಗುಂಪು_handle = x_Event_ಗುಂಪು_Create_With_Caps(OBJECT_ಸ್ಮರಣೆ_CAPS);
    TEST_ASSERT_NOT_NULL(evt_ಗುಂಪು_handle);
    v_Event_ಗುಂಪು_Delete_With_Caps(evt_ಗುಂಪು_handle);
}

#ಆದರೆ CONFIG_SPIRAM

TEST_CASE("IDF additions: WithCaps objects allocate in SPIRAM when requested", "[freertos][idf_additions]")
{
    /* Queue/stream Static* objects honor SPIRAM caps; task TCBs remain internal via pvPortMalloc. */
    ಸರತಿ_Handle_t ಸರತಿ_handle;
    uint8_t *ಸರತಿ_storage;
    Static_ಸರತಿ_t *ಸರತಿ_obj;

    ಸರತಿ_handle = x_ಸರತಿ_Create_With_Caps(4, ಗಾತ್ರ(uint32_t), SPIRAM_OBJECT_ಸ್ಮರಣೆ_CAPS);
    TEST_ASSERT_NOT_NULL(ಸರತಿ_handle);
    TEST_ASSERT_EQUAL(pdTRUE, x_ಸರತಿ_Get_Static_Buffers(ಸರತಿ_handle, &ಸರತಿ_storage, &ಸರತಿ_obj));
    TEST_ASSERT(esp_ptr_external_ram(ಸರತಿ_storage));
    TEST_ASSERT(esp_ptr_external_ram(ಸರತಿ_obj));
    v_ಸರತಿ_Delete_With_Caps(ಸರತಿ_handle);

    ಹರಿವು_ಬಫರ್_Handle_t ಹರಿವು_ಬಫರ್_handle;
    uint8_t *ಹರಿವು_ಬಫರ್_storage;
    Static_ಹರಿವು_ಬಫರ್_t *ಹರಿವು_ಬಫರ್_obj;

    ಹರಿವು_ಬಫರ್_handle = x_ಹರಿವು_ಬಫರ್_Create_With_Caps(16, 1, SPIRAM_OBJECT_ಸ್ಮರಣೆ_CAPS);
    TEST_ASSERT_NOT_NULL(ಹರಿವು_ಬಫರ್_handle);
    TEST_ASSERT_EQUAL(pdTRUE, x_ಹರಿವು_ಬಫರ್_Get_Static_Buffers(ಹರಿವು_ಬಫರ್_handle, &ಹರಿವು_ಬಫರ್_storage, &ಹರಿವು_ಬಫರ್_obj));
    TEST_ASSERT(esp_ptr_external_ram(ಹರಿವು_ಬಫರ್_storage));
    TEST_ASSERT(esp_ptr_external_ram(ಹರಿವು_ಬಫರ್_obj));
    v_ಹರಿವು_ಬಫರ್_Delete_With_Caps(ಹರಿವು_ಬಫರ್_handle);
}

#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_SPIRAM */

/* ---------------------------------------------------------------------------------------------------------------------
 * Task utility API tests (xTaskGetCoreID, xTaskGetStackStart, per-core handle helpers, etc.)
 * --------------------------------------------------------------------------------------------------------------------- */

ಸ್ಥಿರ ಶೂನ್ಯ ರಾಶಿ_test_task(ಶೂನ್ಯ *arg)
{
    (ಶೂನ್ಯ)arg;
    vTaskSuspend(ಖಾಲಿ);
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    TaskHandle_t ಪೋಷಕ;
    ತಳ_Type_t expected_core;
    ತಳ_Type_t observed_core;
    TaskHandle_t self_handle;
} core_id_query_ctx_t;

ಸ್ಥಿರ ಶೂನ್ಯ pinned_core_id_task(ಶೂನ್ಯ *arg)
{
    core_id_query_ctx_t *ctx = (core_id_query_ctx_t *)arg;

    ctx->self_handle = xTaskGetCurrentTaskHandle();
    ctx->observed_core = x_ಪೋರ್ಟ್_Get_Core_ID();
    TEST_ASSERT_EQUAL(ctx->expected_core, ctx->observed_core);
    TEST_ASSERT_EQUAL(ctx->expected_core, xTaskGetCoreID(ಖಾಲಿ));
    TEST_ASSERT_EQUAL(ctx->expected_core, xTaskGetCoreID(ctx->self_handle));

    xTaskNotifyGive(ctx->ಪೋಷಕ);
    vTaskSuspend(ಖಾಲಿ);
}

TEST_CASE("IDF additions: xTaskGetCoreID for pinned tasks", "[freertos][idf_additions]")
{
    ಸ್ಥಿರಾಂಕ TaskHandle_t ಪೋಷಕ = xTaskGetCurrentTaskHandle();
    core_id_query_ctx_t ctx[CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES];

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ core = 0; core < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; core++) {
        ctx[core].ಪೋಷಕ = ಪೋಷಕ;
        ctx[core].expected_core = core;
        ctx[core].observed_core = -1;
        ctx[core].self_handle = ಖಾಲಿ;

        TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_ಗಮ್ಯ_Core(pinned_core_id_task,
                                                          "core_id",
                                                          2048,
                                                          &ctx[core],
                                                          UNITY_FREERTOS_ಆದ್ಯತೆ + 1,
                                                          ಖಾಲಿ,
                                                          core));
    }

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ core = 0; core < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; core++) {
        TEST_ASSERT_NOT_EQUAL(0, ulTaskNotifyTake(pdFALSE, pd_MS_ಗಮ್ಯ_TICKS(1000)));
        TEST_ASSERT_EQUAL(core, ctx[core].observed_core);
        TEST_ASSERT_EQUAL(core, xTaskGetCoreID(ctx[core].self_handle));
    }

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ core = 0; core < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; core++) {
        vTaskDelete(ctx[core].self_handle);
    }
    vTaskDelay(1);
}

#ಆದರೆ (CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES > 1) && !CONFIG_FREERTOS_SMP

ಸ್ಥಿರ SemaphoreHandle_t s_unpinned_ಮುಗಿದ_sem;
ಸ್ಥಿರ TaskHandle_t s_unpinned_task_handle;

ಸ್ಥಿರ ಶೂನ್ಯ unpinned_core_id_task(ಶೂನ್ಯ *arg)
{
    (ಶೂನ್ಯ)arg;
    s_unpinned_task_handle = xTaskGetCurrentTaskHandle();
    xSemaphoreGive(s_unpinned_ಮುಗಿದ_sem);
    vTaskSuspend(ಖಾಲಿ);
}

TEST_CASE("IDF additions: xTaskGetCoreID returns tskNO_AFFINITY for unpinned task", "[freertos][idf_additions]")
{
    s_unpinned_ಮುಗಿದ_sem = x_Semaphore_Create_ದ್ವಿಮಾನ();
    TEST_ASSERT_NOT_NULL(s_unpinned_ಮುಗಿದ_sem);
    s_unpinned_task_handle = ಖಾಲಿ;

    TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_ಗಮ್ಯ_Core(unpinned_core_id_task,
                                                      "unpinned",
                                                      2048,
                                                      ಖಾಲಿ,
                                                      UNITY_FREERTOS_ಆದ್ಯತೆ - 1,
                                                      ಖಾಲಿ,
                                                      tskNO_AFFINITY));
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(s_unpinned_ಮುಗಿದ_sem, pd_MS_ಗಮ್ಯ_TICKS(1000)));
    TEST_ASSERT_NOT_NULL(s_unpinned_task_handle);
    TEST_ASSERT_EQUAL(tskNO_AFFINITY, xTaskGetCoreID(s_unpinned_task_handle));

    vTaskDelete(s_unpinned_task_handle);
    vSemaphoreDelete(s_unpinned_ಮುಗಿದ_sem);
    vTaskDelay(1);
}

#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_NUMBER_OF_CORES > 1 && !CONFIG_FREERTOS_SMP */

#ಆದರೆ CONFIG_FREERTOS_UNICORE

TEST_CASE("IDF additions: xTaskGetCoreID returns 0 on unicore", "[freertos][idf_additions]")
{
    TEST_ASSERT_EQUAL(0, xTaskGetCoreID(ಖಾಲಿ));
    TEST_ASSERT_EQUAL(0, xTaskGetCoreID(xTaskGetCurrentTaskHandle()));
}

#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_UNICORE */

TEST_CASE("IDF additions: xTaskGetStackStart", "[freertos][idf_additions]")
{
    ರಾಶಿ_Type_t *current_ರಾಶಿ_ಮೂಲಕ_null = x_Task_Get_ರಾಶಿ_ಆರಂಭ(ಖಾಲಿ);
    ರಾಶಿ_Type_t *current_ರಾಶಿ_ಮೂಲಕ_handle = x_Task_Get_ರಾಶಿ_ಆರಂಭ(xTaskGetCurrentTaskHandle());

    TEST_ASSERT_NOT_NULL(current_ರಾಶಿ_ಮೂಲಕ_null);
    TEST_ASSERT_EQUAL_PTR(current_ರಾಶಿ_ಮೂಲಕ_null, current_ರಾಶಿ_ಮೂಲಕ_handle);

    TaskHandle_t caps_task = ಖಾಲಿ;
    ರಾಶಿ_Type_t *caps_ರಾಶಿ_ಬಫರ್ = ಖಾಲಿ;
    StaticTask_t *caps_task_ಬಫರ್ = ಖಾಲಿ;

    TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_ಗಮ್ಯ_Core_With_Caps(ರಾಶಿ_test_task,
                                                              "stack_caps",
                                                              4096,
                                                              ಖಾಲಿ,
                                                              UNITY_FREERTOS_ಆದ್ಯತೆ + 1,
                                                              &caps_task,
                                                              UNITY_FREERTOS_CPU,
                                                              OBJECT_ಸ್ಮರಣೆ_CAPS));
    TEST_ASSERT_NOT_NULL(caps_task);
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetStaticBuffers(caps_task, &caps_ರಾಶಿ_ಬಫರ್, &caps_task_ಬಫರ್));
    TEST_ASSERT_EQUAL_PTR(caps_ರಾಶಿ_ಬಫರ್, x_Task_Get_ರಾಶಿ_ಆರಂಭ(caps_task));

    vTaskDeleteWithCaps(caps_task);
    vTaskDelay(10);

#ವ್ಯಾಖ್ಯಾನಿಸು STATIC_ರಾಶಿ_ಬೈಟುಗಳು 4096
    ಸ್ಥಿರ ರಾಶಿ_Type_t static_ರಾಶಿ[STATIC_ರಾಶಿ_ಬೈಟುಗಳು / ಗಾತ್ರ(ರಾಶಿ_Type_t)];
    ಸ್ಥಿರ StaticTask_t static_task_ಬಫರ್;

    memset(&static_task_ಬಫರ್, 0, ಗಾತ್ರ(static_task_ಬಫರ್));

    TaskHandle_t static_task = x_Task_Create_Static_Pinned_ಗಮ್ಯ_Core(ರಾಶಿ_test_task,
                                                             "stack_static",
                                                             STATIC_ರಾಶಿ_ಬೈಟುಗಳು,
                                                             ಖಾಲಿ,
                                                             UNITY_FREERTOS_ಆದ್ಯತೆ + 1,
                                                             static_ರಾಶಿ,
                                                             &static_task_ಬಫರ್,
                                                             UNITY_FREERTOS_CPU);
    TEST_ASSERT_NOT_NULL(static_task);
    TEST_ASSERT_EQUAL_PTR(static_ರಾಶಿ, x_Task_Get_ರಾಶಿ_ಆರಂಭ(static_task));

    vTaskDelete(static_task);
    vTaskDelay(1);
}

#ಆದರೆ !CONFIG_FREERTOS_SMP

TEST_CASE("IDF additions: xTaskGetIdleTaskHandleForCore", "[freertos][idf_additions]")
{
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ core = 0; core < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; core++) {
        TaskHandle_t idle_for_core = xTaskGetIdleTaskHandleForCore(core);
        ಅಕ್ಷರ expected_name[configMAX_TASK_NAME_LEN];

        TEST_ASSERT_NOT_NULL(idle_for_core);
#ಆದರೆ CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES > 1
        snprintf(expected_name, ಗಾತ್ರ(expected_name), "IDLE%d", core);
#ಇಲ್ಲದಿದ್ದರೆ
        snprintf(expected_name, ಗಾತ್ರ(expected_name), "IDLE");
#ಆದರೆ_ಅಂತ್ಯ
        TEST_ASSERT_EQUAL_STRING(expected_name, pcTaskGetName(idle_for_core));
    }

#ಆದರೆ CONFIG_FREERTOS_UNICORE
    TEST_ASSERT_EQUAL(xTaskGetIdleTaskHandle(), xTaskGetIdleTaskHandleForCore(0));
#ಆದರೆ_ಅಂತ್ಯ
}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    TaskHandle_t ಪೋಷಕ;
    ತಳ_Type_t core;
    TaskHandle_t self_handle;
    TaskHandle_t queried_handle;
    ಶೂನ್ಯ * queried_tcb;
} ಪ್ರತಿ_core_handle_ctx_t;

ಸ್ಥಿರ ಶೂನ್ಯ running_on_core_task(ಶೂನ್ಯ *arg)
{
    ಪ್ರತಿ_core_handle_ctx_t *ctx = (ಪ್ರತಿ_core_handle_ctx_t *)arg;

    ctx->self_handle = xTaskGetCurrentTaskHandle();
    ctx->queried_handle = xTaskGetCurrentTaskHandleForCore(ctx->core);
    ctx->queried_tcb = pvTaskGetCurrentTCBForCore(ctx->core);

    xTaskNotifyGive(ctx->ಪೋಷಕ);
    vTaskSuspend(ಖಾಲಿ);
}

TEST_CASE("IDF additions: xTaskGetCurrentTaskHandleForCore and pvTaskGetCurrentTCBForCore", "[freertos][idf_additions]")
{
    ಸ್ಥಿರಾಂಕ TaskHandle_t ಪೋಷಕ = xTaskGetCurrentTaskHandle();

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ core = 0; core < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; core++) {
        ಪ್ರತಿ_core_handle_ctx_t ctx = {
            .ಪೋಷಕ = ಪೋಷಕ,
            .core = core,
            .self_handle = ಖಾಲಿ,
            .queried_handle = ಖಾಲಿ,
            .queried_tcb = ಖಾಲಿ,
        };

        ಆಗ (ulTaskNotifyTake(pdFALSE, 0) != 0) {
            ;
        }

        TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_ಗಮ್ಯ_Core(running_on_core_task,
                                                          "on_core",
                                                          2048,
                                                          &ctx,
                                                          UNITY_FREERTOS_ಆದ್ಯತೆ + 1,
                                                          ಖಾಲಿ,
                                                          core));

        TEST_ASSERT_NOT_EQUAL(0, ulTaskNotifyTake(pdFALSE, pd_MS_ಗಮ್ಯ_TICKS(1000)));

        TEST_ASSERT_EQUAL_PTR(ctx.self_handle, ctx.queried_handle);
        TEST_ASSERT_EQUAL_PTR((ಶೂನ್ಯ *)ctx.self_handle, ctx.queried_tcb);

        vTaskDelete(ctx.self_handle);
        vTaskDelay(1);
    }
}

#ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_FREERTOS_SMP */

#ಆದರೆ CONFIG_FREERTOS_GENERATE_ಸತತ_ಸಮಯ_STATS && !CONFIG_FREERTOS_SMP

TEST_CASE("IDF additions: ulTaskGetIdleRunTimeCounterForCore", "[freertos][idf_additions]")
{
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ core = 0; core < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; core++) {
        config_ಸತತ_ಸಮಯ_COUNTER_TYPE idle_counter = ul_Task_Get_Idle_ಸತತ_ಸಮಯ_Counter_For_Core(core);
        config_ಸತತ_ಸಮಯ_COUNTER_TYPE idle_percent = ul_Task_Get_Idle_ಸತತ_ಸಮಯ_Percent_For_Core(core);

        TEST_ASSERT_GREATER_OR_EQUAL(0, idle_counter);
        TEST_ASSERT_LESS_OR_EQUAL(100, idle_percent);
    }
}

#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS && !CONFIG_FREERTOS_SMP */

/* ---------------------------------------------------------------------------------------------------------------------
 * xTaskCreatePinnedToCore / xTaskCreateStaticPinnedToCore pinning semantics
 * --------------------------------------------------------------------------------------------------------------------- */

#ವ್ಯಾಖ್ಯಾನಿಸು PIN_SEMANTICS_SAMPLES     5
#ವ್ಯಾಖ್ಯಾನಿಸು PIN_SEMANTICS_ರಾಶಿ_ಬೈಟುಗಳು 4096

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    TaskHandle_t ಪೋಷಕ;
    ತಳ_Type_t requested_core;
    ತಳ_Type_t samples[PIN_SEMANTICS_SAMPLES];
    TaskHandle_t self_handle;
} pin_semantics_ctx_t;

ಸ್ಥಿರ ಶೂನ್ಯ drain_task_notifications(ಶೂನ್ಯ)
{
    ಆಗ (ulTaskNotifyTake(pdFALSE, 0) != 0) {
        ;
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ pin_semantics_task(ಶೂನ್ಯ *arg)
{
    pin_semantics_ctx_t *ctx = (pin_semantics_ctx_t *)arg;

    ctx->self_handle = xTaskGetCurrentTaskHandle();
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < PIN_SEMANTICS_SAMPLES; i++) {
        ctx->samples[i] = x_ಪೋರ್ಟ್_Get_Core_ID();
        ಆದರೆ (ctx->requested_core != tskNO_AFFINITY) {
            TEST_ASSERT_EQUAL(ctx->requested_core, ctx->samples[i]);
        } ಇಲ್ಲದಿದ್ದರೆ {
            TEST_ASSERT_GREATER_OR_EQUAL(0, ctx->samples[i]);
            TEST_ASSERT_LESS_THAN(CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES, ctx->samples[i]);
        }
        /* Yield and sleep so a wrongly pinned task would have a chance to migrate. */
        taskYIELD();
        vTaskDelay(1);
    }

    xTaskNotifyGive(ctx->ಪೋಷಕ);
    vTaskSuspend(ಖಾಲಿ);
}

ಸ್ಥಿರ ಶೂನ್ಯ verify_pin_samples(ಸ್ಥಿರಾಂಕ pin_semantics_ctx_t *ctx)
{
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < PIN_SEMANTICS_SAMPLES; i++) {
        ಆದರೆ (ctx->requested_core == tskNO_AFFINITY) {
            TEST_ASSERT_GREATER_OR_EQUAL(0, ctx->samples[i]);
            TEST_ASSERT_LESS_THAN(CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES, ctx->samples[i]);
        } ಇಲ್ಲದಿದ್ದರೆ {
            TEST_ASSERT_EQUAL(ctx->requested_core, ctx->samples[i]);
        }
    }
}

TEST_CASE("IDF additions: xTaskCreatePinnedToCore pinning semantics", "[freertos][idf_additions]")
{
    ಸ್ಥಿರಾಂಕ TaskHandle_t ಪೋಷಕ = xTaskGetCurrentTaskHandle();

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ core = 0; core < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; core++) {
        pin_semantics_ctx_t ctx = {
            .ಪೋಷಕ = ಪೋಷಕ,
            .requested_core = core,
            .self_handle = ಖಾಲಿ,
        };
        TaskHandle_t created = ಖಾಲಿ;

        drain_task_notifications();

        TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_ಗಮ್ಯ_Core(pin_semantics_task,
                                                          "pin_dyn",
                                                          PIN_SEMANTICS_ರಾಶಿ_ಬೈಟುಗಳು,
                                                          &ctx,
                                                          UNITY_FREERTOS_ಆದ್ಯತೆ + 1,
                                                          &created,
                                                          core));
        TEST_ASSERT_NOT_NULL(created);
        TEST_ASSERT_NOT_EQUAL(0, ulTaskNotifyTake(pdFALSE, pd_MS_ಗಮ್ಯ_TICKS(1000)));
        TEST_ASSERT_EQUAL_PTR(created, ctx.self_handle);
        TEST_ASSERT_EQUAL(core, xTaskGetCoreID(created));
        verify_pin_samples(&ctx);

        vTaskDelete(created);
        vTaskDelay(1);
    }
}

TEST_CASE("IDF additions: xTaskCreateStaticPinnedToCore pinning semantics", "[freertos][idf_additions]")
{
    ಸ್ಥಿರ ರಾಶಿ_Type_t pin_static_ರಾಶಿ[PIN_SEMANTICS_ರಾಶಿ_ಬೈಟುಗಳು / ಗಾತ್ರ(ರಾಶಿ_Type_t)];
    ಸ್ಥಿರ StaticTask_t pin_static_tcb;
    ಸ್ಥಿರಾಂಕ TaskHandle_t ಪೋಷಕ = xTaskGetCurrentTaskHandle();

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ core = 0; core < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; core++) {
        pin_semantics_ctx_t ctx = {
            .ಪೋಷಕ = ಪೋಷಕ,
            .requested_core = core,
            .self_handle = ಖಾಲಿ,
        };

        memset(&pin_static_tcb, 0, ಗಾತ್ರ(pin_static_tcb));
        drain_task_notifications();

        TaskHandle_t created = x_Task_Create_Static_Pinned_ಗಮ್ಯ_Core(pin_semantics_task,
                                                             "pin_static",
                                                             PIN_SEMANTICS_ರಾಶಿ_ಬೈಟುಗಳು,
                                                             &ctx,
                                                             UNITY_FREERTOS_ಆದ್ಯತೆ + 1,
                                                             pin_static_ರಾಶಿ,
                                                             &pin_static_tcb,
                                                             core);
        TEST_ASSERT_NOT_NULL(created);
        TEST_ASSERT_NOT_EQUAL(0, ulTaskNotifyTake(pdFALSE, pd_MS_ಗಮ್ಯ_TICKS(1000)));
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
        .ಪೋಷಕ = xTaskGetCurrentTaskHandle(),
        .requested_core = tskNO_AFFINITY,
        .self_handle = ಖಾಲಿ,
    };
    TaskHandle_t created = ಖಾಲಿ;

    drain_task_notifications();

    TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_ಗಮ್ಯ_Core(pin_semantics_task,
                                                      "pin_any",
                                                      PIN_SEMANTICS_ರಾಶಿ_ಬೈಟುಗಳು,
                                                      &ctx,
                                                      UNITY_FREERTOS_ಆದ್ಯತೆ + 1,
                                                      &created,
                                                      tskNO_AFFINITY));
    TEST_ASSERT_NOT_NULL(created);
    TEST_ASSERT_NOT_EQUAL(0, ulTaskNotifyTake(pdFALSE, pd_MS_ಗಮ್ಯ_TICKS(1000)));
    TEST_ASSERT_EQUAL_PTR(created, ctx.self_handle);
    verify_pin_samples(&ctx);

#ಆದರೆ CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES > 1
    TEST_ASSERT_EQUAL(tskNO_AFFINITY, xTaskGetCoreID(created));
#ಇಲ್ಲದಿದ್ದರೆ
    /* Unicore treats tskNO_AFFINITY as core 0. */
    TEST_ASSERT_EQUAL(0, xTaskGetCoreID(created));
    TEST_ASSERT_EQUAL(0, ctx.samples[0]);
#ಆದರೆ_ಅಂತ್ಯ

    vTaskDelete(created);
    vTaskDelay(1);
}
