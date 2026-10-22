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

#تعریف_کړه OBJECT_حافظه_CAPS      (MALLOC_CAP_دننی|MALLOC_CAP_8_بټ)
#تعریف_کړه SPIRAM_OBJECT_حافظه_CAPS (MALLOC_CAP_SPIRAM|MALLOC_CAP_8_بټ)

ساكن فراغ task_with_caps(فراغ *arg)
{
    xTaskNotifyGive((TaskHandle_t)arg);
    vTaskSuspend(عدم);
}

ساكن فراغ task_with_caps_self_delete(فراغ *arg)
{
    /* Wait for the unity task to indicate that this task should delete itself */
    ulTaskNotifyTake(pdTRUE, پورټ_MAX_DELAY);

    /* Although it is not recommended to self-delete a task with memory caps but this
     * is done intentionally to test for memory leaks */
    vTaskDeleteWithCaps(عدم);
}

TEST_CASE("IDF additions: Task creation with memory caps and deletion from another task", "[freertos][idf_additions]")
{
    TaskHandle_t task_handle = عدم;
    ډېری_Type_t *pux_ډېری_بفر;
    StaticTask_t *px_Task_بفر;

    // Create a task with caps
    TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_منزل_Core_With_Caps(task_with_caps, "task", 4096, (فراغ *)xTaskGetCurrentTaskHandle(), UNITY_FREERTOS_لومړیتوب + 1, &task_handle, UNITY_FREERTOS_CPU, OBJECT_حافظه_CAPS));
    TEST_ASSERT_NOT_EQUAL(عدم, task_handle);
    // Get the task's memory
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetStaticBuffers(task_handle, &pux_ډېری_بفر, &px_Task_بفر));
    TEST_ASSERT(esp_ptr_دننه_dram(pux_ډېری_بفر));
    TEST_ASSERT(esp_ptr_دننه_dram(px_Task_بفر));
    // Wait for the created task to block
    ulTaskNotifyTake(pdTRUE, پورټ_MAX_DELAY);
    // Delete the task
    vTaskDeleteWithCaps(task_handle);
}

TEST_CASE("IDF additions: Task creation with memory caps and self deletion", "[freertos][idf_additions]")
{
    TaskHandle_t task_handle = عدم;
    ډېری_Type_t *pux_ډېری_بفر;
    StaticTask_t *px_Task_بفر;

    // Create a task with caps
    TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_منزل_Core_With_Caps(task_with_caps_self_delete, "task", 4096, (فراغ *)xTaskGetCurrentTaskHandle(), UNITY_FREERTOS_لومړیتوب + 1, &task_handle, UNITY_FREERTOS_CPU, OBJECT_حافظه_CAPS));
    TEST_ASSERT_NOT_EQUAL(عدم, task_handle);
    // Get the task's memory
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetStaticBuffers(task_handle, &pux_ډېری_بفر, &px_Task_بفر));
    TEST_ASSERT(esp_ptr_دننه_dram(pux_ډېری_بفر));
    TEST_ASSERT(esp_ptr_دننه_dram(px_Task_بفر));
    // Notify the task to delete itself
    xTaskNotifyGive(task_handle);
}

#إذا CONFIG_FREERTOS_TASK_CREATE_ALLOW_EXT_MEM

TEST_CASE("IDF additions: Task creation with SPIRAM memory caps and self deletion stress test", "[freertos][idf_additions]")
{
#تعریف_کړه TEST_عدد_TASKS      5
#تعریف_کړه TEST_عدد_ITERATIONS 1000
    TaskHandle_t task_handle[TEST_عدد_TASKS];
    ډېری_Type_t *pux_ډېری_بفر;
    StaticTask_t *px_Task_بفر;

    لكل (صحيح j = 0; j < TEST_عدد_ITERATIONS; j++) {
        لكل (صحيح i = 0; i < TEST_عدد_TASKS; i++) {
            // Create a task with caps
            TEST_ASSERT_EQUAL(pdPASS, xTaskCreateWithCaps(task_with_caps_self_delete, "task", 4096, عدم, UNITY_FREERTOS_لومړیتوب, &task_handle[i], MALLOC_CAP_SPIRAM | MALLOC_CAP_8_بټ));
            // Get the task's memory
            TEST_ASSERT_EQUAL(pdTRUE, xTaskGetStaticBuffers(task_handle[i], &pux_ډېری_بفر, &px_Task_بفر));
            TEST_ASSERT(esp_ptr_external_ram(pux_ډېری_بفر));
            TEST_ASSERT(esp_ptr_دننه_dram(px_Task_بفر));
        }

        لكل (صحيح i = 0; i < TEST_عدد_TASKS; i++) {
            // Notify the task to delete itself
            xTaskNotifyGive(task_handle[i]);
        }
        // Let the scheduler run the self-delete tasks.
        vTaskDelay(1);
    }
    // Allow the last batch to be freed by the cleanup task in vTaskDeleteWithCaps().
    vTaskDelay(50);
}

#د_که_پای /* CONFIG_FREERTOS_TASK_CREATE_ALLOW_EXT_MEM */

#إذا ( CONFIG_FREERTOS_شمېره_د_CORES > 1 )

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
    ډېری_Type_t *pux_ډېری_بفر;
    StaticTask_t *px_Task_بفر;

    // Create a task with caps on the other core
    TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_منزل_Core_With_Caps(task_with_caps_running_on_other_core, "task", 4096, (فراغ *)xTaskGetCurrentTaskHandle(), UNITY_FREERTOS_لومړیتوب + 1, &task_handle, !UNITY_FREERTOS_CPU, OBJECT_حافظه_CAPS));
    TEST_ASSERT_NOT_EQUAL(عدم, task_handle);
    // Get the task's memory
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetStaticBuffers(task_handle, &pux_ډېری_بفر, &px_Task_بفر));
    TEST_ASSERT(esp_ptr_دننه_dram(pux_ډېری_بفر));
    TEST_ASSERT(esp_ptr_دننه_dram(px_Task_بفر));
    // Wait for the created task to start running
    ulTaskNotifyTake(pdTRUE, پورټ_MAX_DELAY);
    // Delete the task from another core
    vTaskDeleteWithCaps(task_handle);
}

#د_که_پای // CONFIG_FREERTOS_NUMBER_OF_CORES > 1

TEST_CASE("IDF additions: Queue creation with memory caps", "[freertos][idf_additions]")
{
    کتار_Handle_t کتار_handle;
    uint8_t *کتار_storage;
    Static_کتار_t *کتار_obj;

    // Create queue with caps
    کتار_handle = x_کتار_Create_With_Caps(5, حجم(uint32_t), OBJECT_حافظه_CAPS);
    TEST_ASSERT_NOT_EQUAL(عدم, کتار_handle);
    // Get the queue's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_کتار_Get_Static_Buffers(کتار_handle, &کتار_storage, &کتار_obj));
    TEST_ASSERT(esp_ptr_دننه_dram(کتار_storage));
    TEST_ASSERT(esp_ptr_دننه_dram(کتار_obj));
    // Free the queue
    v_کتار_Delete_With_Caps(کتار_handle);
}

TEST_CASE("IDF additions: Semaphore creation with memory caps", "[freertos][idf_additions]")
{
    SemaphoreHandle_t sem_handle;
    StaticSemaphore_t *sem_obj;

    // Create binary semaphore with caps
    sem_handle = x_Semaphore_Create_دوګونی_With_Caps(OBJECT_حافظه_CAPS);
    TEST_ASSERT_NOT_EQUAL(عدم, sem_handle);
    // Get the binary semaphore's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_Semaphore_Get_Static_بفر(sem_handle, &sem_obj));
    TEST_ASSERT(esp_ptr_دننه_dram(sem_obj));
    // Free the binary semaphore
    vSemaphoreDeleteWithCaps(sem_handle);

    // Create counting semaphore with caps
    sem_handle = xSemaphoreCreateCountingWithCaps(4, 0, OBJECT_حافظه_CAPS);
    TEST_ASSERT_NOT_EQUAL(عدم, sem_handle);
    // Get the counting semaphore's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_Semaphore_Get_Static_بفر(sem_handle, &sem_obj));
    TEST_ASSERT(esp_ptr_دننه_dram(sem_obj));
    // Free the counting semaphore
    vSemaphoreDeleteWithCaps(sem_handle);

    // Create mutex semaphore with caps
    sem_handle = xSemaphoreCreateMutexWithCaps(OBJECT_حافظه_CAPS);
    TEST_ASSERT_NOT_EQUAL(عدم, sem_handle);
    // Get the mutex semaphore's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_Semaphore_Get_Static_بفر(sem_handle, &sem_obj));
    TEST_ASSERT(esp_ptr_دننه_dram(sem_obj));
    // Free the mutex semaphore
    vSemaphoreDeleteWithCaps(sem_handle);

    // Create recursive mutex with caps
    sem_handle = xSemaphoreCreateRecursiveMutexWithCaps(OBJECT_حافظه_CAPS);
    TEST_ASSERT_NOT_EQUAL(عدم, sem_handle);
    // Get the recursive mutex's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_Semaphore_Get_Static_بفر(sem_handle, &sem_obj));
    TEST_ASSERT(esp_ptr_دننه_dram(sem_obj));
    // Free the recursive mutex
    vSemaphoreDeleteWithCaps(sem_handle);
}

TEST_CASE("IDF additions: Stream & message buffer creation with memory caps", "[freertos][idf_additions]")
{
    بهیر_بفر_Handle_t بهیر_بفر_handle;
    uint8_t *بهیر_بفر_storage;
    Static_بهیر_بفر_t *بهیر_بفر_obj;

    // Create stream buffer with caps
    بهیر_بفر_handle = x_بهیر_بفر_Create_With_Caps(16, 0, OBJECT_حافظه_CAPS);
    TEST_ASSERT_NOT_EQUAL(عدم, بهیر_بفر_handle);
    // Get the stream buffer's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_بهیر_بفر_Get_Static_Buffers(بهیر_بفر_handle, &بهیر_بفر_storage, &بهیر_بفر_obj));
    TEST_ASSERT(esp_ptr_دننه_dram(بهیر_بفر_storage));
    TEST_ASSERT(esp_ptr_دننه_dram(بهیر_بفر_obj));
    // Free the stream buffer
    v_بهیر_بفر_Delete_With_Caps(بهیر_بفر_handle);

    پیغام_بفر_Handle_t پیغام_بفر_handle;
    uint8_t *پیغام_بفر_storage;
    Static_پیغام_بفر_t *پیغام_بفر_obj;

    // Create message buffer with caps
    پیغام_بفر_handle = x_پیغام_بفر_Create_With_Caps(16, OBJECT_حافظه_CAPS);
    TEST_ASSERT_NOT_EQUAL(عدم, پیغام_بفر_handle);
    // Get the message buffer's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_پیغام_بفر_Get_Static_Buffers(پیغام_بفر_handle, &پیغام_بفر_storage, &پیغام_بفر_obj));
    TEST_ASSERT(esp_ptr_دننه_dram(پیغام_بفر_storage));
    TEST_ASSERT(esp_ptr_دننه_dram(پیغام_بفر_obj));
    // Free the message buffer
    v_پیغام_بفر_Delete_With_Caps(پیغام_بفر_handle);
}

TEST_CASE("IDF additions: Event group creation with memory caps", "[freertos][idf_additions]")
{
    Event_ډله_Handle_t evt_ډله_handle;
    Static_Event_ډله_t *evt_ډله_obj;

    // Create event group with caps
    evt_ډله_handle = x_Event_ډله_Create_With_Caps(OBJECT_حافظه_CAPS);
    TEST_ASSERT_NOT_EQUAL(عدم, evt_ډله_handle);
    // Get the event group's memory
    TEST_ASSERT_EQUAL(pdTRUE, x_Event_ډله_Get_Static_بفر(evt_ډله_handle, &evt_ډله_obj));
    TEST_ASSERT(esp_ptr_دننه_dram(evt_ډله_obj));
    // Free the event group
    v_Event_ډله_Delete_With_Caps(evt_ډله_handle);
}

TEST_CASE("IDF additions: WithCaps objects functional smoke test", "[freertos][idf_additions]")
{
    کتار_Handle_t کتار_handle;
    uint32_t send_val = 0xA5A5A5A5;
    uint32_t recv_val = 0;

    کتار_handle = x_کتار_Create_With_Caps(4, حجم(uint32_t), OBJECT_حافظه_CAPS);
    TEST_ASSERT_NOT_NULL(کتار_handle);
    TEST_ASSERT_EQUAL(pdTRUE, x_کتار_Send(کتار_handle, &send_val, 0));
    TEST_ASSERT_EQUAL(pdTRUE, x_کتار_Receive(کتار_handle, &recv_val, 0));
    TEST_ASSERT_EQUAL(send_val, recv_val);
    v_کتار_Delete_With_Caps(کتار_handle);

    SemaphoreHandle_t sem_handle = x_Semaphore_Create_دوګونی_With_Caps(OBJECT_حافظه_CAPS);
    TEST_ASSERT_NOT_NULL(sem_handle);
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(sem_handle));
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(sem_handle, 0));
    vSemaphoreDeleteWithCaps(sem_handle);

    بهیر_بفر_Handle_t بهیر_بفر_handle;
    uint8_t بهیر_tx[] = {0x01, 0x02, 0x03};
    uint8_t بهیر_rx[حجم(بهیر_tx)] = {0};

    بهیر_بفر_handle = x_بهیر_بفر_Create_With_Caps(32, 1, OBJECT_حافظه_CAPS);
    TEST_ASSERT_NOT_NULL(بهیر_بفر_handle);
    TEST_ASSERT_EQUAL(حجم(بهیر_tx), x_بهیر_بفر_Send(بهیر_بفر_handle, بهیر_tx, حجم(بهیر_tx), 0));
    TEST_ASSERT_EQUAL(حجم(بهیر_rx), x_بهیر_بفر_Receive(بهیر_بفر_handle, بهیر_rx, حجم(بهیر_rx), 0));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(بهیر_tx, بهیر_rx, حجم(بهیر_tx));
    v_بهیر_بفر_Delete_With_Caps(بهیر_بفر_handle);

    پیغام_بفر_Handle_t پیغام_بفر_handle;

    پیغام_بفر_handle = x_پیغام_بفر_Create_With_Caps(32, OBJECT_حافظه_CAPS);
    TEST_ASSERT_NOT_NULL(پیغام_بفر_handle);
    TEST_ASSERT_EQUAL(حجم(بهیر_tx), x_پیغام_بفر_Send(پیغام_بفر_handle, بهیر_tx, حجم(بهیر_tx), 0));
    TEST_ASSERT_EQUAL(حجم(بهیر_rx), x_پیغام_بفر_Receive(پیغام_بفر_handle, بهیر_rx, حجم(بهیر_rx), 0));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(بهیر_tx, بهیر_rx, حجم(بهیر_tx));
    v_پیغام_بفر_Delete_With_Caps(پیغام_بفر_handle);

    Event_ډله_Handle_t evt_ډله_handle = x_Event_ډله_Create_With_Caps(OBJECT_حافظه_CAPS);
    TEST_ASSERT_NOT_NULL(evt_ډله_handle);
    TEST_ASSERT_بټونه(BIT0, BIT0, x_Event_ډله_سېټ_بټونه(evt_ډله_handle, BIT0));
    TEST_ASSERT_بټونه(BIT0, BIT0, x_Event_ډله_Wait_بټونه(evt_ډله_handle, BIT0, pdTRUE, pdFALSE, 0));
    v_Event_ډله_Delete_With_Caps(evt_ډله_handle);
}

TEST_CASE("IDF additions: WithCaps creation fails on out of memory", "[freertos][idf_additions]")
{
    /* Size just past the total heap for these caps so allocation must fail on every target,
     * without fixed constants that can overflow size_t (e.g. stack_words * sizeof(StackType_t)). */
    ثابت size_t ټول_caps = ډېرۍ_caps_get_ټول_اندازه(OBJECT_حافظه_CAPS);
    ثابت size_t oom_بایټونه = ټول_caps + 1024;
    ثابت UBaseType_t oom_کتار_len = (UBaseType_t)((oom_بایټونه / حجم(uint32_t)) + 1);
    ثابت config_ډېری_ژوروالی_TYPE oom_ډېری_ټکي =
        (config_ډېری_ژوروالی_TYPE)((oom_بایټونه / حجم(ډېری_Type_t)) + 1);
    TaskHandle_t task_handle = عدم;

    TEST_ASSERT_NULL(x_کتار_Create_With_Caps(oom_کتار_len, حجم(uint32_t), OBJECT_حافظه_CAPS));
    TEST_ASSERT_NULL(x_بهیر_بفر_Create_With_Caps(oom_بایټونه, 1, OBJECT_حافظه_CAPS));
    TEST_ASSERT_NULL(x_پیغام_بفر_Create_With_Caps(oom_بایټونه, OBJECT_حافظه_CAPS));
    TEST_ASSERT_EQUAL(pdFAIL, x_Task_Create_Pinned_منزل_Core_With_Caps(task_with_caps,
                                                              "oom_task",
                                                              oom_ډېری_ټکي,
                                                              عدم,
                                                              UNITY_FREERTOS_لومړیتوب,
                                                              &task_handle,
                                                              UNITY_FREERTOS_CPU,
                                                              OBJECT_حافظه_CAPS));
    TEST_ASSERT_NULL(task_handle);
}

/* Create/delete coverage for remaining WithCaps types; Unity setUp/tearDown leak check verifies heap restore. */
TEST_CASE("IDF additions: WithCaps create delete restores heap", "[freertos][idf_additions]")
{
    کتار_Handle_t کتار_handle = x_کتار_Create_With_Caps(4, حجم(uint32_t), OBJECT_حافظه_CAPS);
    TEST_ASSERT_NOT_NULL(کتار_handle);
    v_کتار_Delete_With_Caps(کتار_handle);

    SemaphoreHandle_t sem_handle = xSemaphoreCreateMutexWithCaps(OBJECT_حافظه_CAPS);
    TEST_ASSERT_NOT_NULL(sem_handle);
    vSemaphoreDeleteWithCaps(sem_handle);

    بهیر_بفر_Handle_t بهیر_بفر_handle = x_بهیر_بفر_Create_With_Caps(16, 1, OBJECT_حافظه_CAPS);
    TEST_ASSERT_NOT_NULL(بهیر_بفر_handle);
    v_بهیر_بفر_Delete_With_Caps(بهیر_بفر_handle);

    پیغام_بفر_Handle_t پیغام_بفر_handle = x_پیغام_بفر_Create_With_Caps(16, OBJECT_حافظه_CAPS);
    TEST_ASSERT_NOT_NULL(پیغام_بفر_handle);
    v_پیغام_بفر_Delete_With_Caps(پیغام_بفر_handle);

    Event_ډله_Handle_t evt_ډله_handle = x_Event_ډله_Create_With_Caps(OBJECT_حافظه_CAPS);
    TEST_ASSERT_NOT_NULL(evt_ډله_handle);
    v_Event_ډله_Delete_With_Caps(evt_ډله_handle);
}

#إذا CONFIG_SPIRAM

TEST_CASE("IDF additions: WithCaps objects allocate in SPIRAM when requested", "[freertos][idf_additions]")
{
    /* Queue/stream Static* objects honor SPIRAM caps; task TCBs remain internal via pvPortMalloc. */
    کتار_Handle_t کتار_handle;
    uint8_t *کتار_storage;
    Static_کتار_t *کتار_obj;

    کتار_handle = x_کتار_Create_With_Caps(4, حجم(uint32_t), SPIRAM_OBJECT_حافظه_CAPS);
    TEST_ASSERT_NOT_NULL(کتار_handle);
    TEST_ASSERT_EQUAL(pdTRUE, x_کتار_Get_Static_Buffers(کتار_handle, &کتار_storage, &کتار_obj));
    TEST_ASSERT(esp_ptr_external_ram(کتار_storage));
    TEST_ASSERT(esp_ptr_external_ram(کتار_obj));
    v_کتار_Delete_With_Caps(کتار_handle);

    بهیر_بفر_Handle_t بهیر_بفر_handle;
    uint8_t *بهیر_بفر_storage;
    Static_بهیر_بفر_t *بهیر_بفر_obj;

    بهیر_بفر_handle = x_بهیر_بفر_Create_With_Caps(16, 1, SPIRAM_OBJECT_حافظه_CAPS);
    TEST_ASSERT_NOT_NULL(بهیر_بفر_handle);
    TEST_ASSERT_EQUAL(pdTRUE, x_بهیر_بفر_Get_Static_Buffers(بهیر_بفر_handle, &بهیر_بفر_storage, &بهیر_بفر_obj));
    TEST_ASSERT(esp_ptr_external_ram(بهیر_بفر_storage));
    TEST_ASSERT(esp_ptr_external_ram(بهیر_بفر_obj));
    v_بهیر_بفر_Delete_With_Caps(بهیر_بفر_handle);
}

#د_که_پای /* CONFIG_SPIRAM */

/* ---------------------------------------------------------------------------------------------------------------------
 * Task utility API tests (xTaskGetCoreID, xTaskGetStackStart, per-core handle helpers, etc.)
 * --------------------------------------------------------------------------------------------------------------------- */

ساكن فراغ ډېری_test_task(فراغ *arg)
{
    (فراغ)arg;
    vTaskSuspend(عدم);
}

تعريفنوع بنية {
    TaskHandle_t پلار;
    بنسټ_Type_t expected_core;
    بنسټ_Type_t observed_core;
    TaskHandle_t self_handle;
} core_id_query_ctx_t;

ساكن فراغ pinned_core_id_task(فراغ *arg)
{
    core_id_query_ctx_t *ctx = (core_id_query_ctx_t *)arg;

    ctx->self_handle = xTaskGetCurrentTaskHandle();
    ctx->observed_core = x_پورټ_Get_Core_ID();
    TEST_ASSERT_EQUAL(ctx->expected_core, ctx->observed_core);
    TEST_ASSERT_EQUAL(ctx->expected_core, xTaskGetCoreID(عدم));
    TEST_ASSERT_EQUAL(ctx->expected_core, xTaskGetCoreID(ctx->self_handle));

    xTaskNotifyGive(ctx->پلار);
    vTaskSuspend(عدم);
}

TEST_CASE("IDF additions: xTaskGetCoreID for pinned tasks", "[freertos][idf_additions]")
{
    ثابت TaskHandle_t پلار = xTaskGetCurrentTaskHandle();
    core_id_query_ctx_t ctx[CONFIG_FREERTOS_شمېره_د_CORES];

    لكل (صحيح core = 0; core < CONFIG_FREERTOS_شمېره_د_CORES; core++) {
        ctx[core].پلار = پلار;
        ctx[core].expected_core = core;
        ctx[core].observed_core = -1;
        ctx[core].self_handle = عدم;

        TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_منزل_Core(pinned_core_id_task,
                                                          "core_id",
                                                          2048,
                                                          &ctx[core],
                                                          UNITY_FREERTOS_لومړیتوب + 1,
                                                          عدم,
                                                          core));
    }

    لكل (صحيح core = 0; core < CONFIG_FREERTOS_شمېره_د_CORES; core++) {
        TEST_ASSERT_NOT_EQUAL(0, ulTaskNotifyTake(pdFALSE, pd_MS_منزل_TICKS(1000)));
        TEST_ASSERT_EQUAL(core, ctx[core].observed_core);
        TEST_ASSERT_EQUAL(core, xTaskGetCoreID(ctx[core].self_handle));
    }

    لكل (صحيح core = 0; core < CONFIG_FREERTOS_شمېره_د_CORES; core++) {
        vTaskDelete(ctx[core].self_handle);
    }
    vTaskDelay(1);
}

#إذا (CONFIG_FREERTOS_شمېره_د_CORES > 1) && !CONFIG_FREERTOS_SMP

ساكن SemaphoreHandle_t s_unpinned_بشپړ_sem;
ساكن TaskHandle_t s_unpinned_task_handle;

ساكن فراغ unpinned_core_id_task(فراغ *arg)
{
    (فراغ)arg;
    s_unpinned_task_handle = xTaskGetCurrentTaskHandle();
    xSemaphoreGive(s_unpinned_بشپړ_sem);
    vTaskSuspend(عدم);
}

TEST_CASE("IDF additions: xTaskGetCoreID returns tskNO_AFFINITY for unpinned task", "[freertos][idf_additions]")
{
    s_unpinned_بشپړ_sem = x_Semaphore_Create_دوګونی();
    TEST_ASSERT_NOT_NULL(s_unpinned_بشپړ_sem);
    s_unpinned_task_handle = عدم;

    TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_منزل_Core(unpinned_core_id_task,
                                                      "unpinned",
                                                      2048,
                                                      عدم,
                                                      UNITY_FREERTOS_لومړیتوب - 1,
                                                      عدم,
                                                      tskNO_AFFINITY));
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(s_unpinned_بشپړ_sem, pd_MS_منزل_TICKS(1000)));
    TEST_ASSERT_NOT_NULL(s_unpinned_task_handle);
    TEST_ASSERT_EQUAL(tskNO_AFFINITY, xTaskGetCoreID(s_unpinned_task_handle));

    vTaskDelete(s_unpinned_task_handle);
    vSemaphoreDelete(s_unpinned_بشپړ_sem);
    vTaskDelay(1);
}

#د_که_پای /* CONFIG_FREERTOS_NUMBER_OF_CORES > 1 && !CONFIG_FREERTOS_SMP */

#إذا CONFIG_FREERTOS_UNICORE

TEST_CASE("IDF additions: xTaskGetCoreID returns 0 on unicore", "[freertos][idf_additions]")
{
    TEST_ASSERT_EQUAL(0, xTaskGetCoreID(عدم));
    TEST_ASSERT_EQUAL(0, xTaskGetCoreID(xTaskGetCurrentTaskHandle()));
}

#د_که_پای /* CONFIG_FREERTOS_UNICORE */

TEST_CASE("IDF additions: xTaskGetStackStart", "[freertos][idf_additions]")
{
    ډېری_Type_t *current_ډېری_له_لارې_null = x_Task_Get_ډېری_پیل(عدم);
    ډېری_Type_t *current_ډېری_له_لارې_handle = x_Task_Get_ډېری_پیل(xTaskGetCurrentTaskHandle());

    TEST_ASSERT_NOT_NULL(current_ډېری_له_لارې_null);
    TEST_ASSERT_EQUAL_PTR(current_ډېری_له_لارې_null, current_ډېری_له_لارې_handle);

    TaskHandle_t caps_task = عدم;
    ډېری_Type_t *caps_ډېری_بفر = عدم;
    StaticTask_t *caps_task_بفر = عدم;

    TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_منزل_Core_With_Caps(ډېری_test_task,
                                                              "stack_caps",
                                                              4096,
                                                              عدم,
                                                              UNITY_FREERTOS_لومړیتوب + 1,
                                                              &caps_task,
                                                              UNITY_FREERTOS_CPU,
                                                              OBJECT_حافظه_CAPS));
    TEST_ASSERT_NOT_NULL(caps_task);
    TEST_ASSERT_EQUAL(pdTRUE, xTaskGetStaticBuffers(caps_task, &caps_ډېری_بفر, &caps_task_بفر));
    TEST_ASSERT_EQUAL_PTR(caps_ډېری_بفر, x_Task_Get_ډېری_پیل(caps_task));

    vTaskDeleteWithCaps(caps_task);
    vTaskDelay(10);

#تعریف_کړه STATIC_ډېری_بایټونه 4096
    ساكن ډېری_Type_t static_ډېری[STATIC_ډېری_بایټونه / حجم(ډېری_Type_t)];
    ساكن StaticTask_t static_task_بفر;

    memset(&static_task_بفر, 0, حجم(static_task_بفر));

    TaskHandle_t static_task = x_Task_Create_Static_Pinned_منزل_Core(ډېری_test_task,
                                                             "stack_static",
                                                             STATIC_ډېری_بایټونه,
                                                             عدم,
                                                             UNITY_FREERTOS_لومړیتوب + 1,
                                                             static_ډېری,
                                                             &static_task_بفر,
                                                             UNITY_FREERTOS_CPU);
    TEST_ASSERT_NOT_NULL(static_task);
    TEST_ASSERT_EQUAL_PTR(static_ډېری, x_Task_Get_ډېری_پیل(static_task));

    vTaskDelete(static_task);
    vTaskDelay(1);
}

#إذا !CONFIG_FREERTOS_SMP

TEST_CASE("IDF additions: xTaskGetIdleTaskHandleForCore", "[freertos][idf_additions]")
{
    لكل (صحيح core = 0; core < CONFIG_FREERTOS_شمېره_د_CORES; core++) {
        TaskHandle_t idle_for_core = xTaskGetIdleTaskHandleForCore(core);
        حرف expected_name[configMAX_TASK_NAME_LEN];

        TEST_ASSERT_NOT_NULL(idle_for_core);
#إذا CONFIG_FREERTOS_شمېره_د_CORES > 1
        snprintf(expected_name, حجم(expected_name), "IDLE%d", core);
#وإلا
        snprintf(expected_name, حجم(expected_name), "IDLE");
#د_که_پای
        TEST_ASSERT_EQUAL_STRING(expected_name, pcTaskGetName(idle_for_core));
    }

#إذا CONFIG_FREERTOS_UNICORE
    TEST_ASSERT_EQUAL(xTaskGetIdleTaskHandle(), xTaskGetIdleTaskHandleForCore(0));
#د_که_پای
}

تعريفنوع بنية {
    TaskHandle_t پلار;
    بنسټ_Type_t core;
    TaskHandle_t self_handle;
    TaskHandle_t queried_handle;
    فراغ * queried_tcb;
} فی_core_handle_ctx_t;

ساكن فراغ running_on_core_task(فراغ *arg)
{
    فی_core_handle_ctx_t *ctx = (فی_core_handle_ctx_t *)arg;

    ctx->self_handle = xTaskGetCurrentTaskHandle();
    ctx->queried_handle = xTaskGetCurrentTaskHandleForCore(ctx->core);
    ctx->queried_tcb = pvTaskGetCurrentTCBForCore(ctx->core);

    xTaskNotifyGive(ctx->پلار);
    vTaskSuspend(عدم);
}

TEST_CASE("IDF additions: xTaskGetCurrentTaskHandleForCore and pvTaskGetCurrentTCBForCore", "[freertos][idf_additions]")
{
    ثابت TaskHandle_t پلار = xTaskGetCurrentTaskHandle();

    لكل (صحيح core = 0; core < CONFIG_FREERTOS_شمېره_د_CORES; core++) {
        فی_core_handle_ctx_t ctx = {
            .پلار = پلار,
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
                                                          UNITY_FREERTOS_لومړیتوب + 1,
                                                          عدم,
                                                          core));

        TEST_ASSERT_NOT_EQUAL(0, ulTaskNotifyTake(pdFALSE, pd_MS_منزل_TICKS(1000)));

        TEST_ASSERT_EQUAL_PTR(ctx.self_handle, ctx.queried_handle);
        TEST_ASSERT_EQUAL_PTR((فراغ *)ctx.self_handle, ctx.queried_tcb);

        vTaskDelete(ctx.self_handle);
        vTaskDelay(1);
    }
}

#د_که_پای /* !CONFIG_FREERTOS_SMP */

#إذا CONFIG_FREERTOS_GENERATE_پرلپسې_وخت_STATS && !CONFIG_FREERTOS_SMP

TEST_CASE("IDF additions: ulTaskGetIdleRunTimeCounterForCore", "[freertos][idf_additions]")
{
    لكل (صحيح core = 0; core < CONFIG_FREERTOS_شمېره_د_CORES; core++) {
        config_پرلپسې_وخت_COUNTER_TYPE idle_counter = ul_Task_Get_Idle_پرلپسې_وخت_Counter_For_Core(core);
        config_پرلپسې_وخت_COUNTER_TYPE idle_percent = ul_Task_Get_Idle_پرلپسې_وخت_Percent_For_Core(core);

        TEST_ASSERT_GREATER_OR_EQUAL(0, idle_counter);
        TEST_ASSERT_LESS_OR_EQUAL(100, idle_percent);
    }
}

#د_که_پای /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS && !CONFIG_FREERTOS_SMP */

/* ---------------------------------------------------------------------------------------------------------------------
 * xTaskCreatePinnedToCore / xTaskCreateStaticPinnedToCore pinning semantics
 * --------------------------------------------------------------------------------------------------------------------- */

#تعریف_کړه PIN_SEMANTICS_SAMPLES     5
#تعریف_کړه PIN_SEMANTICS_ډېری_بایټونه 4096

تعريفنوع بنية {
    TaskHandle_t پلار;
    بنسټ_Type_t requested_core;
    بنسټ_Type_t samples[PIN_SEMANTICS_SAMPLES];
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
        ctx->samples[i] = x_پورټ_Get_Core_ID();
        إذا (ctx->requested_core != tskNO_AFFINITY) {
            TEST_ASSERT_EQUAL(ctx->requested_core, ctx->samples[i]);
        } وإلا {
            TEST_ASSERT_GREATER_OR_EQUAL(0, ctx->samples[i]);
            TEST_ASSERT_LESS_THAN(CONFIG_FREERTOS_شمېره_د_CORES, ctx->samples[i]);
        }
        /* Yield and sleep so a wrongly pinned task would have a chance to migrate. */
        taskYIELD();
        vTaskDelay(1);
    }

    xTaskNotifyGive(ctx->پلار);
    vTaskSuspend(عدم);
}

ساكن فراغ verify_pin_samples(ثابت pin_semantics_ctx_t *ctx)
{
    لكل (صحيح i = 0; i < PIN_SEMANTICS_SAMPLES; i++) {
        إذا (ctx->requested_core == tskNO_AFFINITY) {
            TEST_ASSERT_GREATER_OR_EQUAL(0, ctx->samples[i]);
            TEST_ASSERT_LESS_THAN(CONFIG_FREERTOS_شمېره_د_CORES, ctx->samples[i]);
        } وإلا {
            TEST_ASSERT_EQUAL(ctx->requested_core, ctx->samples[i]);
        }
    }
}

TEST_CASE("IDF additions: xTaskCreatePinnedToCore pinning semantics", "[freertos][idf_additions]")
{
    ثابت TaskHandle_t پلار = xTaskGetCurrentTaskHandle();

    لكل (صحيح core = 0; core < CONFIG_FREERTOS_شمېره_د_CORES; core++) {
        pin_semantics_ctx_t ctx = {
            .پلار = پلار,
            .requested_core = core,
            .self_handle = عدم,
        };
        TaskHandle_t created = عدم;

        drain_task_notifications();

        TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_منزل_Core(pin_semantics_task,
                                                          "pin_dyn",
                                                          PIN_SEMANTICS_ډېری_بایټونه,
                                                          &ctx,
                                                          UNITY_FREERTOS_لومړیتوب + 1,
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
    ساكن ډېری_Type_t pin_static_ډېری[PIN_SEMANTICS_ډېری_بایټونه / حجم(ډېری_Type_t)];
    ساكن StaticTask_t pin_static_tcb;
    ثابت TaskHandle_t پلار = xTaskGetCurrentTaskHandle();

    لكل (صحيح core = 0; core < CONFIG_FREERTOS_شمېره_د_CORES; core++) {
        pin_semantics_ctx_t ctx = {
            .پلار = پلار,
            .requested_core = core,
            .self_handle = عدم,
        };

        memset(&pin_static_tcb, 0, حجم(pin_static_tcb));
        drain_task_notifications();

        TaskHandle_t created = x_Task_Create_Static_Pinned_منزل_Core(pin_semantics_task,
                                                             "pin_static",
                                                             PIN_SEMANTICS_ډېری_بایټونه,
                                                             &ctx,
                                                             UNITY_FREERTOS_لومړیتوب + 1,
                                                             pin_static_ډېری,
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
        .پلار = xTaskGetCurrentTaskHandle(),
        .requested_core = tskNO_AFFINITY,
        .self_handle = عدم,
    };
    TaskHandle_t created = عدم;

    drain_task_notifications();

    TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_منزل_Core(pin_semantics_task,
                                                      "pin_any",
                                                      PIN_SEMANTICS_ډېری_بایټونه,
                                                      &ctx,
                                                      UNITY_FREERTOS_لومړیتوب + 1,
                                                      &created,
                                                      tskNO_AFFINITY));
    TEST_ASSERT_NOT_NULL(created);
    TEST_ASSERT_NOT_EQUAL(0, ulTaskNotifyTake(pdFALSE, pd_MS_منزل_TICKS(1000)));
    TEST_ASSERT_EQUAL_PTR(created, ctx.self_handle);
    verify_pin_samples(&ctx);

#إذا CONFIG_FREERTOS_شمېره_د_CORES > 1
    TEST_ASSERT_EQUAL(tskNO_AFFINITY, xTaskGetCoreID(created));
#وإلا
    /* Unicore treats tskNO_AFFINITY as core 0. */
    TEST_ASSERT_EQUAL(0, xTaskGetCoreID(created));
    TEST_ASSERT_EQUAL(0, ctx.samples[0]);
#د_که_پای

    vTaskDelete(created);
    vTaskDelay(1);
}
