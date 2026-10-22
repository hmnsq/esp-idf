/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ "freertos/queue.h"
#అన్తర్భావ "freertos/semphr.h"
#అన్తర్భావ "freertos/stream_buffer.h"
#అన్తర్భావ "freertos/message_buffer.h"
#అన్తర్భావ "freertos/event_groups.h"
#అన్తర్భావ "freertos/idf_additions.h"
#అన్తర్భావ "freertos/freertos_debug.h"
#అన్తర్భావ "esp_memory_utils.h"
#అన్తర్భావ "esp_heap_caps.h"
#అన్తర్భావ "unity.h"
#అన్తర్భావ "test_utils.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <inttypes.h>

/*
Test ESP-IDF FreeRTOS API additions from idf_additions.h:
    - ...Create...WithCaps() / ...DeleteWithCaps() memory capability helpers
    - xTaskCreatePinnedToCore() / xTaskCreateStaticPinnedToCore() pinning semantics
    - Task utility helpers (xTaskGetCoreID, xTaskGetStackStart, per-core handle queries, etc.)

Run only these cases from the Unity menu with the [idf_additions] tag filter.
 */

#నిర్వచించు OBJECT_జ్ఞాపకం_CAPS      (MALLOC_CAP_అంతర్గత|MALLOC_CAP_8_బిట్టు)
#నిర్వచించు SPIRAM_OBJECT_జ్ఞాపకం_CAPS (MALLOC_CAP_SPIRAM|MALLOC_CAP_8_బిట్టు)

స్థిర శూన్య task_with_caps(శూన్య *arg)
{
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskSuspend(ఖాళీ);
}

స్థిర శూన్య task_with_caps_self_delete(శూన్య *arg)
{
    /* Wait for the unity task to indicate that this task should delete itself */
    ulTaskNotifyTake(pdTRUE, పోర్టు_MAX_DELAY);

    /* Although it is not recommended to self-delete a task with memory caps but this
     * is done intentionally to test for memory leaks */
    vTaskDeleteWithCaps(ఖాళీ);
}

TEST_CASE("IDF additions: Task creation with memory caps and deletion from another task", "[freertos][idf_additions]")
{
    TaskHandle_t task_handle = ఖాళీ;
    దొంతర_Type_t *pux_దొంతర_బఫరు;
    StaticTask_t *px_Task_బఫరు;

    // Create a task with caps
    TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_గమ్యం_Core_With_Caps(task_with_caps, "task", 4096, (శూన్య *)xTaskGetCurrentTaskHandle(), UNITY_FREERTOS_ప్రాధాన్యత + 1, &task_handle, UNITY_FREERTOS_CPU, OBJECT_జ్ఞాపకం_CAPS));
    TEST_ASSERT_NOT_EQUAL(ఖాళీ, task_handle);
    // Get the task's memory
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetStaticBuffers(task_handle, &pux_దొంతర_బఫరు, &px_Task_బఫరు));
    TEST_ASSERT(esp_ptr_లోపల_dram(pux_దొంతర_బఫరు));
    TEST_ASSERT(esp_ptr_లోపల_dram(px_Task_బఫరు));
    // Wait for the created task to block
    ulTaskNotifyTake(pdTRUE, పోర్టు_MAX_DELAY);
    // Delete the task
    vTaskDeleteWithCaps(task_handle);
}

TEST_CASE("IDF additions: Task creation with memory caps and self deletion", "[freertos][idf_additions]")
{
    TaskHandle_t task_handle = ఖాళీ;
    దొంతర_Type_t *pux_దొంతర_బఫరు;
    StaticTask_t *px_Task_బఫరు;

    // Create a task with caps
    TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_గమ్యం_Core_With_Caps(task_with_caps_self_delete, "task", 4096, (శూన్య *)xTaskGetCurrentTaskHandle(), UNITY_FREERTOS_ప్రాధాన్యత + 1, &task_handle, UNITY_FREERTOS_CPU, OBJECT_జ్ఞాపకం_CAPS));
    TEST_ASSERT_NOT_EQUAL(ఖాళీ, task_handle);
    // Get the task's memory
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetStaticBuffers(task_handle, &pux_దొంతర_బఫరు, &px_Task_బఫరు));
    TEST_ASSERT(esp_ptr_లోపల_dram(pux_దొంతర_బఫరు));
    TEST_ASSERT(esp_ptr_లోపల_dram(px_Task_బఫరు));
    // Notify the task to delete itself
    xTaskNotifyGive(task_handle);
}

#ఒకవేళ CONFIG_FREERTOS_TASK_CREATE_ALLOW_EXT_MEM

TEST_CASE("IDF additions: Task creation with SPIRAM memory caps and self deletion stress test", "[freertos][idf_additions]")
{
#నిర్వచించు TEST_అంకె_TASKS      5
#నిర్వచించు TEST_అంకె_ITERATIONS 1000
    TaskHandle_t task_handle[TEST_అంకె_TASKS];
    దొంతర_Type_t *pux_దొంతర_బఫరు;
    StaticTask_t *px_Task_బఫరు;

    కొరకు (పూర్ణాంకం j = 0; j < TEST_అంకె_ITERATIONS; j++) {
        కొరకు (పూర్ణాంకం i = 0; i < TEST_అంకె_TASKS; i++) {
            // Create a task with caps
            TEST_ASSERT_EQUAL(pdPASS, xTaskCreateWithCaps(task_with_caps_self_delete, "task", 4096, ఖాళీ, UNITY_FREERTOS_ప్రాధాన్యత, &task_handle[i], MALLOC_CAP_SPIRAM | MALLOC_CAP_8_బిట్టు));
            // Get the task's memory
            TEST_ASSERT_EQUAL(pdTRUE, xTaskGetStaticBuffers(task_handle[i], &pux_దొంతర_బఫరు, &px_Task_బఫరు));
            TEST_ASSERT(esp_ptr_external_ram(pux_దొంతర_బఫరు));
            TEST_ASSERT(esp_ptr_లోపల_dram(px_Task_బఫరు));
        }

        కొరకు (పూర్ణాంకం i = 0; i < TEST_అంకె_TASKS; i++) {
            // Notify the task to delete itself
            xTaskNotifyGive(task_handle[i]);
        }
        // Let the scheduler run the self-delete tasks.
        vTaskDelay(1);
    }
    // Allow the last batch to be freed by the cleanup task in vTaskDeleteWithCaps().
    vTaskDelay(50);
}

#అయితే_ముగింపు /* CONFIG_FREERTOS_TASK_CREATE_ALLOW_EXT_MEM */

#ఒకవేళ ( CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES > 1 )

స్థిర శూన్య task_with_caps_running_on_other_core(శూన్య *arg)
{
    /* Notify the unity task that this task is running on the other core */
    xTaskNotifyGive((TaskHandle_t)arg);

    /* We make sure that this task is running on the other core */
    అయితే (1) {
        ;
    }
}

TEST_CASE("IDF additions: Task creation with memory caps and deletion from another core", "[freertos][idf_additions]")
{
    TaskHandle_t task_handle = ఖాళీ;
    దొంతర_Type_t *pux_దొంతర_బఫరు;
    StaticTask_t *px_Task_బఫరు;

    // Create a task with caps on the other core
    TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_గమ్యం_Core_With_Caps(task_with_caps_running_on_other_core, "task", 4096, (శూన్య *)xTaskGetCurrentTaskHandle(), UNITY_FREERTOS_ప్రాధాన్యత + 1, &task_handle, !UNITY_FREERTOS_CPU, OBJECT_జ్ఞాపకం_CAPS));
    TEST_ASSERT_NOT_EQUAL(ఖాళీ, task_handle);
    // Get the task's memory
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetStaticBuffers(task_handle, &pux_దొంతర_బఫరు, &px_Task_బఫరు));
    TEST_ASSERT(esp_ptr_లోపల_dram(pux_దొంతర_బఫరు));
    TEST_ASSERT(esp_ptr_లోపల_dram(px_Task_బఫరు));
    // Wait for the created task to start running
    ulTaskNotifyTake(pdTRUE, పోర్టు_MAX_DELAY);
    // Delete the task from another core
    vTaskDeleteWithCaps(task_handle);
}

#అయితే_ముగింపు // CONFIG_FREERTOS_NUMBER_OF_CORES > 1

TEST_CASE("IDF additions: Queue creation with memory caps", "[freertos][idf_additions]")
{
    వరుస_Handle_t వరుస_handle;
    uint8_t *వరుస_storage;
    Static_వరుస_t *వరుస_obj;

    // Create queue with caps
    వరుస_handle = x_వరుస_Create_With_Caps(5, పరిమాణం(uint32_t), OBJECT_జ్ఞాపకం_CAPS);
    TEST_ASSERT_NOT_EQUAL(ఖాళీ, వరుస_handle);
    // Get the queue's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_వరుస_Get_Static_Buffers(వరుస_handle, &వరుస_storage, &వరుస_obj));
    TEST_ASSERT(esp_ptr_లోపల_dram(వరుస_storage));
    TEST_ASSERT(esp_ptr_లోపల_dram(వరుస_obj));
    // Free the queue
    v_వరుస_Delete_With_Caps(వరుస_handle);
}

TEST_CASE("IDF additions: Semaphore creation with memory caps", "[freertos][idf_additions]")
{
    SemaphoreHandle_t sem_handle;
    StaticSemaphore_t *sem_obj;

    // Create binary semaphore with caps
    sem_handle = x_Semaphore_Create_ద్విమాన_With_Caps(OBJECT_జ్ఞాపకం_CAPS);
    TEST_ASSERT_NOT_EQUAL(ఖాళీ, sem_handle);
    // Get the binary semaphore's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_Semaphore_Get_Static_బఫరు(sem_handle, &sem_obj));
    TEST_ASSERT(esp_ptr_లోపల_dram(sem_obj));
    // Free the binary semaphore
    vSemaphoreDeleteWithCaps(sem_handle);

    // Create counting semaphore with caps
    sem_handle = xSemaphoreCreateCountingWithCaps(4, 0, OBJECT_జ్ఞాపకం_CAPS);
    TEST_ASSERT_NOT_EQUAL(ఖాళీ, sem_handle);
    // Get the counting semaphore's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_Semaphore_Get_Static_బఫరు(sem_handle, &sem_obj));
    TEST_ASSERT(esp_ptr_లోపల_dram(sem_obj));
    // Free the counting semaphore
    vSemaphoreDeleteWithCaps(sem_handle);

    // Create mutex semaphore with caps
    sem_handle = xSemaphoreCreateMutexWithCaps(OBJECT_జ్ఞాపకం_CAPS);
    TEST_ASSERT_NOT_EQUAL(ఖాళీ, sem_handle);
    // Get the mutex semaphore's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_Semaphore_Get_Static_బఫరు(sem_handle, &sem_obj));
    TEST_ASSERT(esp_ptr_లోపల_dram(sem_obj));
    // Free the mutex semaphore
    vSemaphoreDeleteWithCaps(sem_handle);

    // Create recursive mutex with caps
    sem_handle = xSemaphoreCreateRecursiveMutexWithCaps(OBJECT_జ్ఞాపకం_CAPS);
    TEST_ASSERT_NOT_EQUAL(ఖాళీ, sem_handle);
    // Get the recursive mutex's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_Semaphore_Get_Static_బఫరు(sem_handle, &sem_obj));
    TEST_ASSERT(esp_ptr_లోపల_dram(sem_obj));
    // Free the recursive mutex
    vSemaphoreDeleteWithCaps(sem_handle);
}

TEST_CASE("IDF additions: Stream & message buffer creation with memory caps", "[freertos][idf_additions]")
{
    ప్రవాహం_బఫరు_Handle_t ప్రవాహం_బఫరు_handle;
    uint8_t *ప్రవాహం_బఫరు_storage;
    Static_ప్రవాహం_బఫరు_t *ప్రవాహం_బఫరు_obj;

    // Create stream buffer with caps
    ప్రవాహం_బఫరు_handle = x_ప్రవాహం_బఫరు_Create_With_Caps(16, 0, OBJECT_జ్ఞాపకం_CAPS);
    TEST_ASSERT_NOT_EQUAL(ఖాళీ, ప్రవాహం_బఫరు_handle);
    // Get the stream buffer's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_ప్రవాహం_బఫరు_Get_Static_Buffers(ప్రవాహం_బఫరు_handle, &ప్రవాహం_బఫరు_storage, &ప్రవాహం_బఫరు_obj));
    TEST_ASSERT(esp_ptr_లోపల_dram(ప్రవాహం_బఫరు_storage));
    TEST_ASSERT(esp_ptr_లోపల_dram(ప్రవాహం_బఫరు_obj));
    // Free the stream buffer
    v_ప్రవాహం_బఫరు_Delete_With_Caps(ప్రవాహం_బఫరు_handle);

    సందేశం_బఫరు_Handle_t సందేశం_బఫరు_handle;
    uint8_t *సందేశం_బఫరు_storage;
    Static_సందేశం_బఫరు_t *సందేశం_బఫరు_obj;

    // Create message buffer with caps
    సందేశం_బఫరు_handle = x_సందేశం_బఫరు_Create_With_Caps(16, OBJECT_జ్ఞాపకం_CAPS);
    TEST_ASSERT_NOT_EQUAL(ఖాళీ, సందేశం_బఫరు_handle);
    // Get the message buffer's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_సందేశం_బఫరు_Get_Static_Buffers(సందేశం_బఫరు_handle, &సందేశం_బఫరు_storage, &సందేశం_బఫరు_obj));
    TEST_ASSERT(esp_ptr_లోపల_dram(సందేశం_బఫరు_storage));
    TEST_ASSERT(esp_ptr_లోపల_dram(సందేశం_బఫరు_obj));
    // Free the message buffer
    v_సందేశం_బఫరు_Delete_With_Caps(సందేశం_బఫరు_handle);
}

TEST_CASE("IDF additions: Event group creation with memory caps", "[freertos][idf_additions]")
{
    Event_సమూహం_Handle_t evt_సమూహం_handle;
    Static_Event_సమూహం_t *evt_సమూహం_obj;

    // Create event group with caps
    evt_సమూహం_handle = x_Event_సమూహం_Create_With_Caps(OBJECT_జ్ఞాపకం_CAPS);
    TEST_ASSERT_NOT_EQUAL(ఖాళీ, evt_సమూహం_handle);
    // Get the event group's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_Event_సమూహం_Get_Static_బఫరు(evt_సమూహం_handle, &evt_సమూహం_obj));
    TEST_ASSERT(esp_ptr_లోపల_dram(evt_సమూహం_obj));
    // Free the event group
    v_Event_సమూహం_Delete_With_Caps(evt_సమూహం_handle);
}

TEST_CASE("IDF additions: WithCaps objects functional smoke test", "[freertos][idf_additions]")
{
    వరుస_Handle_t వరుస_handle;
    uint32_t send_val = 0xA5A5A5A5;
    uint32_t recv_val = 0;

    వరుస_handle = x_వరుస_Create_With_Caps(4, పరిమాణం(uint32_t), OBJECT_జ్ఞాపకం_CAPS);
    TEST_ASSERT_NOT_NULL(వరుస_handle);
    TEST_ASSERT_EQUAL(pdTRUE, x_వరుస_Send(వరుస_handle, &send_val, 0));
    TEST_ASSERT_EQUAL(pdTRUE, x_వరుస_Receive(వరుస_handle, &recv_val, 0));
    TEST_ASSERT_EQUAL(send_val, recv_val);
    v_వరుస_Delete_With_Caps(వరుస_handle);

    SemaphoreHandle_t sem_handle = x_Semaphore_Create_ద్విమాన_With_Caps(OBJECT_జ్ఞాపకం_CAPS);
    TEST_ASSERT_NOT_NULL(sem_handle);
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(sem_handle));
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(sem_handle, 0));
    vSemaphoreDeleteWithCaps(sem_handle);

    ప్రవాహం_బఫరు_Handle_t ప్రవాహం_బఫరు_handle;
    uint8_t ప్రవాహం_tx[] = {0x01, 0x02, 0x03};
    uint8_t ప్రవాహం_rx[పరిమాణం(ప్రవాహం_tx)] = {0};

    ప్రవాహం_బఫరు_handle = x_ప్రవాహం_బఫరు_Create_With_Caps(32, 1, OBJECT_జ్ఞాపకం_CAPS);
    TEST_ASSERT_NOT_NULL(ప్రవాహం_బఫరు_handle);
    TEST_ASSERT_EQUAL(పరిమాణం(ప్రవాహం_tx), x_ప్రవాహం_బఫరు_Send(ప్రవాహం_బఫరు_handle, ప్రవాహం_tx, పరిమాణం(ప్రవాహం_tx), 0));
    TEST_ASSERT_EQUAL(పరిమాణం(ప్రవాహం_rx), x_ప్రవాహం_బఫరు_Receive(ప్రవాహం_బఫరు_handle, ప్రవాహం_rx, పరిమాణం(ప్రవాహం_rx), 0));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ప్రవాహం_tx, ప్రవాహం_rx, పరిమాణం(ప్రవాహం_tx));
    v_ప్రవాహం_బఫరు_Delete_With_Caps(ప్రవాహం_బఫరు_handle);

    సందేశం_బఫరు_Handle_t సందేశం_బఫరు_handle;

    సందేశం_బఫరు_handle = x_సందేశం_బఫరు_Create_With_Caps(32, OBJECT_జ్ఞాపకం_CAPS);
    TEST_ASSERT_NOT_NULL(సందేశం_బఫరు_handle);
    TEST_ASSERT_EQUAL(పరిమాణం(ప్రవాహం_tx), x_సందేశం_బఫరు_Send(సందేశం_బఫరు_handle, ప్రవాహం_tx, పరిమాణం(ప్రవాహం_tx), 0));
    TEST_ASSERT_EQUAL(పరిమాణం(ప్రవాహం_rx), x_సందేశం_బఫరు_Receive(సందేశం_బఫరు_handle, ప్రవాహం_rx, పరిమాణం(ప్రవాహం_rx), 0));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ప్రవాహం_tx, ప్రవాహం_rx, పరిమాణం(ప్రవాహం_tx));
    v_సందేశం_బఫరు_Delete_With_Caps(సందేశం_బఫరు_handle);

    Event_సమూహం_Handle_t evt_సమూహం_handle = x_Event_సమూహం_Create_With_Caps(OBJECT_జ్ఞాపకం_CAPS);
    TEST_ASSERT_NOT_NULL(evt_సమూహం_handle);
    TEST_ASSERT_బిట్లు(BIT0, BIT0, x_Event_సమూహం_సమితి_బిట్లు(evt_సమూహం_handle, BIT0));
    TEST_ASSERT_బిట్లు(BIT0, BIT0, x_Event_సమూహం_Wait_బిట్లు(evt_సమూహం_handle, BIT0, pdTRUE, pdFALSE, 0));
    v_Event_సమూహం_Delete_With_Caps(evt_సమూహం_handle);
}

TEST_CASE("IDF additions: WithCaps creation fails on out of memory", "[freertos][idf_additions]")
{
    /* Size just past the total heap for these caps so allocation must fail on every target,
     * without fixed constants that can overflow size_t (e.g. stack_words * sizeof(StackType_t)). */
    స్థిరరాశి size_t మొత్తం_caps = కుప్ప_caps_get_మొత్తం_కొలత(OBJECT_జ్ఞాపకం_CAPS);
    స్థిరరాశి size_t oom_బైట్లు = మొత్తం_caps + 1024;
    స్థిరరాశి UBaseType_t oom_వరుస_len = (UBaseType_t)((oom_బైట్లు / పరిమాణం(uint32_t)) + 1);
    స్థిరరాశి config_దొంతర_లోతు_TYPE oom_దొంతర_పదాలు =
        (config_దొంతర_లోతు_TYPE)((oom_బైట్లు / పరిమాణం(దొంతర_Type_t)) + 1);
    TaskHandle_t task_handle = ఖాళీ;

    TEST_ASSERT_NULL(x_వరుస_Create_With_Caps(oom_వరుస_len, పరిమాణం(uint32_t), OBJECT_జ్ఞాపకం_CAPS));
    TEST_ASSERT_NULL(x_ప్రవాహం_బఫరు_Create_With_Caps(oom_బైట్లు, 1, OBJECT_జ్ఞాపకం_CAPS));
    TEST_ASSERT_NULL(x_సందేశం_బఫరు_Create_With_Caps(oom_బైట్లు, OBJECT_జ్ఞాపకం_CAPS));
    TEST_ASSERT_EQUAL(pdFAIL, x_Task_Create_Pinned_గమ్యం_Core_With_Caps(task_with_caps,
                                                              "oom_task",
                                                              oom_దొంతర_పదాలు,
                                                              ఖాళీ,
                                                              UNITY_FREERTOS_ప్రాధాన్యత,
                                                              &task_handle,
                                                              UNITY_FREERTOS_CPU,
                                                              OBJECT_జ్ఞాపకం_CAPS));
    TEST_ASSERT_NULL(task_handle);
}

/* Create/delete coverage for remaining WithCaps types; Unity setUp/tearDown leak check verifies heap restore. */
TEST_CASE("IDF additions: WithCaps create delete restores heap", "[freertos][idf_additions]")
{
    వరుస_Handle_t వరుస_handle = x_వరుస_Create_With_Caps(4, పరిమాణం(uint32_t), OBJECT_జ్ఞాపకం_CAPS);
    TEST_ASSERT_NOT_NULL(వరుస_handle);
    v_వరుస_Delete_With_Caps(వరుస_handle);

    SemaphoreHandle_t sem_handle = xSemaphoreCreateMutexWithCaps(OBJECT_జ్ఞాపకం_CAPS);
    TEST_ASSERT_NOT_NULL(sem_handle);
    vSemaphoreDeleteWithCaps(sem_handle);

    ప్రవాహం_బఫరు_Handle_t ప్రవాహం_బఫరు_handle = x_ప్రవాహం_బఫరు_Create_With_Caps(16, 1, OBJECT_జ్ఞాపకం_CAPS);
    TEST_ASSERT_NOT_NULL(ప్రవాహం_బఫరు_handle);
    v_ప్రవాహం_బఫరు_Delete_With_Caps(ప్రవాహం_బఫరు_handle);

    సందేశం_బఫరు_Handle_t సందేశం_బఫరు_handle = x_సందేశం_బఫరు_Create_With_Caps(16, OBJECT_జ్ఞాపకం_CAPS);
    TEST_ASSERT_NOT_NULL(సందేశం_బఫరు_handle);
    v_సందేశం_బఫరు_Delete_With_Caps(సందేశం_బఫరు_handle);

    Event_సమూహం_Handle_t evt_సమూహం_handle = x_Event_సమూహం_Create_With_Caps(OBJECT_జ్ఞాపకం_CAPS);
    TEST_ASSERT_NOT_NULL(evt_సమూహం_handle);
    v_Event_సమూహం_Delete_With_Caps(evt_సమూహం_handle);
}

#ఒకవేళ CONFIG_SPIRAM

TEST_CASE("IDF additions: WithCaps objects allocate in SPIRAM when requested", "[freertos][idf_additions]")
{
    /* Queue/stream Static* objects honor SPIRAM caps; task TCBs remain internal via pvPortMalloc. */
    వరుస_Handle_t వరుస_handle;
    uint8_t *వరుస_storage;
    Static_వరుస_t *వరుస_obj;

    వరుస_handle = x_వరుస_Create_With_Caps(4, పరిమాణం(uint32_t), SPIRAM_OBJECT_జ్ఞాపకం_CAPS);
    TEST_ASSERT_NOT_NULL(వరుస_handle);
    TEST_ASSERT_EQUAL(pdTRUE, x_వరుస_Get_Static_Buffers(వరుస_handle, &వరుస_storage, &వరుస_obj));
    TEST_ASSERT(esp_ptr_external_ram(వరుస_storage));
    TEST_ASSERT(esp_ptr_external_ram(వరుస_obj));
    v_వరుస_Delete_With_Caps(వరుస_handle);

    ప్రవాహం_బఫరు_Handle_t ప్రవాహం_బఫరు_handle;
    uint8_t *ప్రవాహం_బఫరు_storage;
    Static_ప్రవాహం_బఫరు_t *ప్రవాహం_బఫరు_obj;

    ప్రవాహం_బఫరు_handle = x_ప్రవాహం_బఫరు_Create_With_Caps(16, 1, SPIRAM_OBJECT_జ్ఞాపకం_CAPS);
    TEST_ASSERT_NOT_NULL(ప్రవాహం_బఫరు_handle);
    TEST_ASSERT_EQUAL(pdTRUE, x_ప్రవాహం_బఫరు_Get_Static_Buffers(ప్రవాహం_బఫరు_handle, &ప్రవాహం_బఫరు_storage, &ప్రవాహం_బఫరు_obj));
    TEST_ASSERT(esp_ptr_external_ram(ప్రవాహం_బఫరు_storage));
    TEST_ASSERT(esp_ptr_external_ram(ప్రవాహం_బఫరు_obj));
    v_ప్రవాహం_బఫరు_Delete_With_Caps(ప్రవాహం_బఫరు_handle);
}

#అయితే_ముగింపు /* CONFIG_SPIRAM */

/* ---------------------------------------------------------------------------------------------------------------------
 * Task utility API tests (xTaskGetCoreID, xTaskGetStackStart, per-core handle helpers, etc.)
 * --------------------------------------------------------------------------------------------------------------------- */

స్థిర శూన్య దొంతర_test_task(శూన్య *arg)
{
    (శూన్య)arg;
    vTaskSuspend(ఖాళీ);
}

రకనిర్వచనం నిర్మాణం {
    TaskHandle_t మాతృ;
    పునాది_Type_t expected_core;
    పునాది_Type_t observed_core;
    TaskHandle_t self_handle;
} core_id_query_ctx_t;

స్థిర శూన్య pinned_core_id_task(శూన్య *arg)
{
    core_id_query_ctx_t *ctx = (core_id_query_ctx_t *)arg;

    ctx->self_handle = xTaskGetCurrentTaskHandle();
    ctx->observed_core = x_పోర్టు_Get_Core_ID();
    TEST_ASSERT_EQUAL(ctx->expected_core, ctx->observed_core);
    TEST_ASSERT_EQUAL(ctx->expected_core, xTaskGetCoreID(ఖాళీ));
    TEST_ASSERT_EQUAL(ctx->expected_core, xTaskGetCoreID(ctx->self_handle));

    xTaskNotifyGive(ctx->మాతృ);
    vTaskSuspend(ఖాళీ);
}

TEST_CASE("IDF additions: xTaskGetCoreID for pinned tasks", "[freertos][idf_additions]")
{
    స్థిరరాశి TaskHandle_t మాతృ = xTaskGetCurrentTaskHandle();
    core_id_query_ctx_t ctx[CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES];

    కొరకు (పూర్ణాంకం core = 0; core < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; core++) {
        ctx[core].మాతృ = మాతృ;
        ctx[core].expected_core = core;
        ctx[core].observed_core = -1;
        ctx[core].self_handle = ఖాళీ;

        TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_గమ్యం_Core(pinned_core_id_task,
                                                          "core_id",
                                                          2048,
                                                          &ctx[core],
                                                          UNITY_FREERTOS_ప్రాధాన్యత + 1,
                                                          ఖాళీ,
                                                          core));
    }

    కొరకు (పూర్ణాంకం core = 0; core < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; core++) {
        TEST_ASSERT_NOT_EQUAL(0, ulTaskNotifyTake(pdFALSE, pd_MS_గమ్యం_TICKS(1000)));
        TEST_ASSERT_EQUAL(core, ctx[core].observed_core);
        TEST_ASSERT_EQUAL(core, xTaskGetCoreID(ctx[core].self_handle));
    }

    కొరకు (పూర్ణాంకం core = 0; core < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; core++) {
        vTaskDelete(ctx[core].self_handle);
    }
    vTaskDelay(1);
}

#ఒకవేళ (CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES > 1) && !CONFIG_FREERTOS_SMP

స్థిర SemaphoreHandle_t s_unpinned_పూర్తి_sem;
స్థిర TaskHandle_t s_unpinned_task_handle;

స్థిర శూన్య unpinned_core_id_task(శూన్య *arg)
{
    (శూన్య)arg;
    s_unpinned_task_handle = xTaskGetCurrentTaskHandle();
    xSemaphoreGive(s_unpinned_పూర్తి_sem);
    vTaskSuspend(ఖాళీ);
}

TEST_CASE("IDF additions: xTaskGetCoreID returns tskNO_AFFINITY for unpinned task", "[freertos][idf_additions]")
{
    s_unpinned_పూర్తి_sem = x_Semaphore_Create_ద్విమాన();
    TEST_ASSERT_NOT_NULL(s_unpinned_పూర్తి_sem);
    s_unpinned_task_handle = ఖాళీ;

    TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_గమ్యం_Core(unpinned_core_id_task,
                                                      "unpinned",
                                                      2048,
                                                      ఖాళీ,
                                                      UNITY_FREERTOS_ప్రాధాన్యత - 1,
                                                      ఖాళీ,
                                                      tskNO_AFFINITY));
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(s_unpinned_పూర్తి_sem, pd_MS_గమ్యం_TICKS(1000)));
    TEST_ASSERT_NOT_NULL(s_unpinned_task_handle);
    TEST_ASSERT_EQUAL(tskNO_AFFINITY, xTaskGetCoreID(s_unpinned_task_handle));

    vTaskDelete(s_unpinned_task_handle);
    vSemaphoreDelete(s_unpinned_పూర్తి_sem);
    vTaskDelay(1);
}

#అయితే_ముగింపు /* CONFIG_FREERTOS_NUMBER_OF_CORES > 1 && !CONFIG_FREERTOS_SMP */

#ఒకవేళ CONFIG_FREERTOS_UNICORE

TEST_CASE("IDF additions: xTaskGetCoreID returns 0 on unicore", "[freertos][idf_additions]")
{
    TEST_ASSERT_EQUAL(0, xTaskGetCoreID(ఖాళీ));
    TEST_ASSERT_EQUAL(0, xTaskGetCoreID(xTaskGetCurrentTaskHandle()));
}

#అయితే_ముగింపు /* CONFIG_FREERTOS_UNICORE */

TEST_CASE("IDF additions: xTaskGetStackStart", "[freertos][idf_additions]")
{
    దొంతర_Type_t *current_దొంతర_ద్వారా_null = x_Task_Get_దొంతర_ప్రారంభం(ఖాళీ);
    దొంతర_Type_t *current_దొంతర_ద్వారా_handle = x_Task_Get_దొంతర_ప్రారంభం(xTaskGetCurrentTaskHandle());

    TEST_ASSERT_NOT_NULL(current_దొంతర_ద్వారా_null);
    TEST_ASSERT_EQUAL_PTR(current_దొంతర_ద్వారా_null, current_దొంతర_ద్వారా_handle);

    TaskHandle_t caps_task = ఖాళీ;
    దొంతర_Type_t *caps_దొంతర_బఫరు = ఖాళీ;
    StaticTask_t *caps_task_బఫరు = ఖాళీ;

    TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_గమ్యం_Core_With_Caps(దొంతర_test_task,
                                                              "stack_caps",
                                                              4096,
                                                              ఖాళీ,
                                                              UNITY_FREERTOS_ప్రాధాన్యత + 1,
                                                              &caps_task,
                                                              UNITY_FREERTOS_CPU,
                                                              OBJECT_జ్ఞాపకం_CAPS));
    TEST_ASSERT_NOT_NULL(caps_task);
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetStaticBuffers(caps_task, &caps_దొంతర_బఫరు, &caps_task_బఫరు));
    TEST_ASSERT_EQUAL_PTR(caps_దొంతర_బఫరు, x_Task_Get_దొంతర_ప్రారంభం(caps_task));

    vTaskDeleteWithCaps(caps_task);
    vTaskDelay(10);

#నిర్వచించు STATIC_దొంతర_బైట్లు 4096
    స్థిర దొంతర_Type_t static_దొంతర[STATIC_దొంతర_బైట్లు / పరిమాణం(దొంతర_Type_t)];
    స్థిర StaticTask_t static_task_బఫరు;

    memset(&static_task_బఫరు, 0, పరిమాణం(static_task_బఫరు));

    TaskHandle_t static_task = x_Task_Create_Static_Pinned_గమ్యం_Core(దొంతర_test_task,
                                                             "stack_static",
                                                             STATIC_దొంతర_బైట్లు,
                                                             ఖాళీ,
                                                             UNITY_FREERTOS_ప్రాధాన్యత + 1,
                                                             static_దొంతర,
                                                             &static_task_బఫరు,
                                                             UNITY_FREERTOS_CPU);
    TEST_ASSERT_NOT_NULL(static_task);
    TEST_ASSERT_EQUAL_PTR(static_దొంతర, x_Task_Get_దొంతర_ప్రారంభం(static_task));

    vTaskDelete(static_task);
    vTaskDelay(1);
}

#ఒకవేళ !CONFIG_FREERTOS_SMP

TEST_CASE("IDF additions: xTaskGetIdleTaskHandleForCore", "[freertos][idf_additions]")
{
    కొరకు (పూర్ణాంకం core = 0; core < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; core++) {
        TaskHandle_t idle_for_core = xTaskGetIdleTaskHandleForCore(core);
        అక్షరం expected_name[configMAX_TASK_NAME_LEN];

        TEST_ASSERT_NOT_NULL(idle_for_core);
#ఒకవేళ CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES > 1
        snprintf(expected_name, పరిమాణం(expected_name), "IDLE%d", core);
#లేకపోతే
        snprintf(expected_name, పరిమాణం(expected_name), "IDLE");
#అయితే_ముగింపు
        TEST_ASSERT_EQUAL_STRING(expected_name, pcTaskGetName(idle_for_core));
    }

#ఒకవేళ CONFIG_FREERTOS_UNICORE
    TEST_ASSERT_EQUAL(xTaskGetIdleTaskHandle(), xTaskGetIdleTaskHandleForCore(0));
#అయితే_ముగింపు
}

రకనిర్వచనం నిర్మాణం {
    TaskHandle_t మాతృ;
    పునాది_Type_t core;
    TaskHandle_t self_handle;
    TaskHandle_t queried_handle;
    శూన్య * queried_tcb;
} చొప్పున_core_handle_ctx_t;

స్థిర శూన్య running_on_core_task(శూన్య *arg)
{
    చొప్పున_core_handle_ctx_t *ctx = (చొప్పున_core_handle_ctx_t *)arg;

    ctx->self_handle = xTaskGetCurrentTaskHandle();
    ctx->queried_handle = xTaskGetCurrentTaskHandleForCore(ctx->core);
    ctx->queried_tcb = pvTaskGetCurrentTCBForCore(ctx->core);

    xTaskNotifyGive(ctx->మాతృ);
    vTaskSuspend(ఖాళీ);
}

TEST_CASE("IDF additions: xTaskGetCurrentTaskHandleForCore and pvTaskGetCurrentTCBForCore", "[freertos][idf_additions]")
{
    స్థిరరాశి TaskHandle_t మాతృ = xTaskGetCurrentTaskHandle();

    కొరకు (పూర్ణాంకం core = 0; core < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; core++) {
        చొప్పున_core_handle_ctx_t ctx = {
            .మాతృ = మాతృ,
            .core = core,
            .self_handle = ఖాళీ,
            .queried_handle = ఖాళీ,
            .queried_tcb = ఖాళీ,
        };

        అయితే (ulTaskNotifyTake(pdFALSE, 0) != 0) {
            ;
        }

        TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_గమ్యం_Core(running_on_core_task,
                                                          "on_core",
                                                          2048,
                                                          &ctx,
                                                          UNITY_FREERTOS_ప్రాధాన్యత + 1,
                                                          ఖాళీ,
                                                          core));

        TEST_ASSERT_NOT_EQUAL(0, ulTaskNotifyTake(pdFALSE, pd_MS_గమ్యం_TICKS(1000)));

        TEST_ASSERT_EQUAL_PTR(ctx.self_handle, ctx.queried_handle);
        TEST_ASSERT_EQUAL_PTR((శూన్య *)ctx.self_handle, ctx.queried_tcb);

        vTaskDelete(ctx.self_handle);
        vTaskDelay(1);
    }
}

#అయితే_ముగింపు /* !CONFIG_FREERTOS_SMP */

#ఒకవేళ CONFIG_FREERTOS_GENERATE_నిరంతరం_సమయం_STATS && !CONFIG_FREERTOS_SMP

TEST_CASE("IDF additions: ulTaskGetIdleRunTimeCounterForCore", "[freertos][idf_additions]")
{
    కొరకు (పూర్ణాంకం core = 0; core < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; core++) {
        config_నిరంతరం_సమయం_COUNTER_TYPE idle_counter = ul_Task_Get_Idle_నిరంతరం_సమయం_Counter_For_Core(core);
        config_నిరంతరం_సమయం_COUNTER_TYPE idle_percent = ul_Task_Get_Idle_నిరంతరం_సమయం_Percent_For_Core(core);

        TEST_ASSERT_GREATER_OR_EQUAL(0, idle_counter);
        TEST_ASSERT_LESS_OR_EQUAL(100, idle_percent);
    }
}

#అయితే_ముగింపు /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS && !CONFIG_FREERTOS_SMP */

/* ---------------------------------------------------------------------------------------------------------------------
 * xTaskCreatePinnedToCore / xTaskCreateStaticPinnedToCore pinning semantics
 * --------------------------------------------------------------------------------------------------------------------- */

#నిర్వచించు PIN_SEMANTICS_SAMPLES     5
#నిర్వచించు PIN_SEMANTICS_దొంతర_బైట్లు 4096

రకనిర్వచనం నిర్మాణం {
    TaskHandle_t మాతృ;
    పునాది_Type_t requested_core;
    పునాది_Type_t samples[PIN_SEMANTICS_SAMPLES];
    TaskHandle_t self_handle;
} pin_semantics_ctx_t;

స్థిర శూన్య drain_task_notifications(శూన్య)
{
    అయితే (ulTaskNotifyTake(pdFALSE, 0) != 0) {
        ;
    }
}

స్థిర శూన్య pin_semantics_task(శూన్య *arg)
{
    pin_semantics_ctx_t *ctx = (pin_semantics_ctx_t *)arg;

    ctx->self_handle = xTaskGetCurrentTaskHandle();
    కొరకు (పూర్ణాంకం i = 0; i < PIN_SEMANTICS_SAMPLES; i++) {
        ctx->samples[i] = x_పోర్టు_Get_Core_ID();
        ఒకవేళ (ctx->requested_core != tskNO_AFFINITY) {
            TEST_ASSERT_EQUAL(ctx->requested_core, ctx->samples[i]);
        } లేకపోతే {
            TEST_ASSERT_GREATER_OR_EQUAL(0, ctx->samples[i]);
            TEST_ASSERT_LESS_THAN(CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES, ctx->samples[i]);
        }
        /* Yield and sleep so a wrongly pinned task would have a chance to migrate. */
        taskYIELD();
        vTaskDelay(1);
    }

    xTaskNotifyGive(ctx->మాతృ);
    vTaskSuspend(ఖాళీ);
}

స్థిర శూన్య verify_pin_samples(స్థిరరాశి pin_semantics_ctx_t *ctx)
{
    కొరకు (పూర్ణాంకం i = 0; i < PIN_SEMANTICS_SAMPLES; i++) {
        ఒకవేళ (ctx->requested_core == tskNO_AFFINITY) {
            TEST_ASSERT_GREATER_OR_EQUAL(0, ctx->samples[i]);
            TEST_ASSERT_LESS_THAN(CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES, ctx->samples[i]);
        } లేకపోతే {
            TEST_ASSERT_EQUAL(ctx->requested_core, ctx->samples[i]);
        }
    }
}

TEST_CASE("IDF additions: xTaskCreatePinnedToCore pinning semantics", "[freertos][idf_additions]")
{
    స్థిరరాశి TaskHandle_t మాతృ = xTaskGetCurrentTaskHandle();

    కొరకు (పూర్ణాంకం core = 0; core < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; core++) {
        pin_semantics_ctx_t ctx = {
            .మాతృ = మాతృ,
            .requested_core = core,
            .self_handle = ఖాళీ,
        };
        TaskHandle_t created = ఖాళీ;

        drain_task_notifications();

        TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_గమ్యం_Core(pin_semantics_task,
                                                          "pin_dyn",
                                                          PIN_SEMANTICS_దొంతర_బైట్లు,
                                                          &ctx,
                                                          UNITY_FREERTOS_ప్రాధాన్యత + 1,
                                                          &created,
                                                          core));
        TEST_ASSERT_NOT_NULL(created);
        TEST_ASSERT_NOT_EQUAL(0, ulTaskNotifyTake(pdFALSE, pd_MS_గమ్యం_TICKS(1000)));
        TEST_ASSERT_EQUAL_PTR(created, ctx.self_handle);
        TEST_ASSERT_EQUAL(core, xTaskGetCoreID(created));
        verify_pin_samples(&ctx);

        vTaskDelete(created);
        vTaskDelay(1);
    }
}

TEST_CASE("IDF additions: xTaskCreateStaticPinnedToCore pinning semantics", "[freertos][idf_additions]")
{
    స్థిర దొంతర_Type_t pin_static_దొంతర[PIN_SEMANTICS_దొంతర_బైట్లు / పరిమాణం(దొంతర_Type_t)];
    స్థిర StaticTask_t pin_static_tcb;
    స్థిరరాశి TaskHandle_t మాతృ = xTaskGetCurrentTaskHandle();

    కొరకు (పూర్ణాంకం core = 0; core < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; core++) {
        pin_semantics_ctx_t ctx = {
            .మాతృ = మాతృ,
            .requested_core = core,
            .self_handle = ఖాళీ,
        };

        memset(&pin_static_tcb, 0, పరిమాణం(pin_static_tcb));
        drain_task_notifications();

        TaskHandle_t created = x_Task_Create_Static_Pinned_గమ్యం_Core(pin_semantics_task,
                                                             "pin_static",
                                                             PIN_SEMANTICS_దొంతర_బైట్లు,
                                                             &ctx,
                                                             UNITY_FREERTOS_ప్రాధాన్యత + 1,
                                                             pin_static_దొంతర,
                                                             &pin_static_tcb,
                                                             core);
        TEST_ASSERT_NOT_NULL(created);
        TEST_ASSERT_NOT_EQUAL(0, ulTaskNotifyTake(pdFALSE, pd_MS_గమ్యం_TICKS(1000)));
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
        .మాతృ = xTaskGetCurrentTaskHandle(),
        .requested_core = tskNO_AFFINITY,
        .self_handle = ఖాళీ,
    };
    TaskHandle_t created = ఖాళీ;

    drain_task_notifications();

    TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_గమ్యం_Core(pin_semantics_task,
                                                      "pin_any",
                                                      PIN_SEMANTICS_దొంతర_బైట్లు,
                                                      &ctx,
                                                      UNITY_FREERTOS_ప్రాధాన్యత + 1,
                                                      &created,
                                                      tskNO_AFFINITY));
    TEST_ASSERT_NOT_NULL(created);
    TEST_ASSERT_NOT_EQUAL(0, ulTaskNotifyTake(pdFALSE, pd_MS_గమ్యం_TICKS(1000)));
    TEST_ASSERT_EQUAL_PTR(created, ctx.self_handle);
    verify_pin_samples(&ctx);

#ఒకవేళ CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES > 1
    TEST_ASSERT_EQUAL(tskNO_AFFINITY, xTaskGetCoreID(created));
#లేకపోతే
    /* Unicore treats tskNO_AFFINITY as core 0. */
    TEST_ASSERT_EQUAL(0, xTaskGetCoreID(created));
    TEST_ASSERT_EQUAL(0, ctx.samples[0]);
#అయితే_ముగింపు

    vTaskDelete(created);
    vTaskDelay(1);
}
