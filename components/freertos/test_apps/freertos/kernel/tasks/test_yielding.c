/*
 * SPDX-FileCopyrightText: 2022-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * Unit tests for FreeRTOS task yielding
 */

#ضمن "sdkconfig.h"
#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن "freertos/semphr.h"
#ضمن "unity.h"
#ضمن "test_utils.h"
#ضمن <string.h>

// Array to store the task ids of the test threads being yielded
ساكن متطاير uint32_t task_yield_sequence[3];

// Index variable to access the yield sequence array
ساكن متطاير uint32_t idx = 0;

// Lock to protect the shared variables to store task id
ساكن پورٹ_MUX_TYPE idx_جندرا;

// Synchronization variable to have a deterministic dispatch sequence of the test threads
ساكن متطاير bool task_sequence_ready;

// Synchronization variable between the test threads and the unity task
ساكن متطاير uint32_t گنتی;

// Lock variable to create a blocked task scenario
ساكن متطاير SemaphoreHandle_t task_mutex;

// Semaphore to synchronize yield test tasks
ساكن SemaphoreHandle_t yield_sync_sem;

// This helper macro is used to store the task id atomically
#تعریف_کرو STORE_TASK_ID(task_id)  ({ \
        پورٹ_ENTER_CRITICAL(&idx_جندرا); \
        task_yield_sequence[idx++] = task_id; \
        پورٹ_EXIT_CRITICAL(&idx_جندرا); \
})

/*
 * Test yielding for same priority tasks on the same core.
 *
 * The test performs the following actions:
 * - Creates 2 tasks with the same priority on the same core.
 * - Each task pushes its task_id on to a queue and then yields.
 * - Unity task checks the sequence of the tasks run once the yield_tasks are done.
 */
ساكن فراغ yield_task1(فراغ *arg)
{
    uint32_t task_id = (uint32_t)arg;

    /* Store task_id in the sequence array */
    STORE_TASK_ID(task_id);

    /* Give semaphore to unblock yield_task2, making it READY (not just setting a flag).
     * This ensures task2 is in the ready queue when we yield. */
    xSemaphoreGive(yield_sync_sem);

    /* Yield - now task2 is guaranteed to be READY and should run next */
    taskYIELD();

    /* Increment task count to notify unity task */
    گنتی++;

    /* Delete self */
    vTaskDelete(عدم);
}

ساكن فراغ yield_task2(فراغ *arg)
{
    uint32_t task_id = (uint32_t)arg;

    /* Block on semaphore - this ensures task1 runs first and we don't poll.
     * When task1 gives the semaphore, we transition directly to READY state. */
    xSemaphoreTake(yield_sync_sem, پورٹ_MAX_DELAY);

    /* Store task_id in the sequence array */
    STORE_TASK_ID(task_id);

    /* Yield */
    taskYIELD();

    /* Increment task count to notify unity task */
    گنتی++;

    /* Delete self */
    vTaskDelete(عدم);
}

TEST_CASE("Task yield must run the next ready task of the same priority", "[freertos]")
{
    /* Reset yield sequence index */
    idx = 0;

    /* Reset yield sequence array */
    memset((فراغ *)task_yield_sequence, 0, حجم(task_yield_sequence));

    /* Initialize idx lock */
    پورٹ_MUX_INITIALIZE(&idx_جندرا);

    /* Reset task count */
    گنتی = 0;

    /* Reset task sequence flag */
    task_sequence_ready = false;

    /* Create semaphore for synchronization - start empty so task2 blocks */
    yield_sync_sem = x_Semaphore_Create_ثنائی();
    TEST_ASSERT_NOT_NULL(yield_sync_sem);

    /* Create test tasks - order matters!
     * Task2 is created first and will immediately block on the semaphore.
     * Task1 is created second and will run first since task2 is blocked. */
    x_Task_Create_Pinned_منزل_Core(yield_task2, "yield_task2", 2048, (فراغ *)2, UNITY_FREERTOS_ترجیح - 1, عدم, UNITY_FREERTOS_CPU);
    vTaskDelay(1); /* Ensure task2 has blocked on semaphore before creating task1 */
    x_Task_Create_Pinned_منزل_Core(yield_task1, "yield_task1", 2048, (فراغ *)1, UNITY_FREERTOS_ترجیح - 1, عدم, UNITY_FREERTOS_CPU);

    /* Wait for the tasks to finish up */
    طالما (گنتی != 2) {
        vTaskDelay(10);
    }

    idx = 0;

    /* Verify that the yield is successful and the next ready task is run */
    TEST_ASSERT_EQUAL(1, task_yield_sequence[idx++]);
    TEST_ASSERT_EQUAL(2, task_yield_sequence[idx++]);

    /* Clean up semaphore */
    vSemaphoreDelete(yield_sync_sem);
}

/*
 * Test yielding behavior when a task is blocked
 *
 * The test performs the following actions:
 * - Creates 2 tasks with the same priority on the same core.
 * - One task blocks on a mutex.
 * - Second task does not contest for a mutex and yields.
 * - Unity task verifies that the blocked task is not scheduled unless it is ready to run.
 */
ساكن فراغ test_task1(فراغ *arg)
{
    uint32_t task_id = (uint32_t)arg;

    /* Block on mutex taken by the unity task */
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(task_mutex, پورٹ_MAX_DELAY));

    /* Store task_id in the sequence array */
    STORE_TASK_ID(task_id);

    /* Increment task count to notify unity task */
    گنتی++;

    /* Release mutex */
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(task_mutex));

    /* Delete self */
    vTaskDelete(عدم);
}

ساكن فراغ test_task2(فراغ *arg)
{
    uint32_t task_id = (uint32_t)arg;

    /* Store task_id in the sequence array */
    STORE_TASK_ID(task_id);

    /* Yield */
    taskYIELD();

    /* Store task_id in the sequence array */
    STORE_TASK_ID(task_id);

    /* Increment task count to notify unity task */
    گنتی++;

    /* Delete self */
    vTaskDelete(عدم);
}

TEST_CASE("Task yield must not run a blocked task", "[freertos]")
{
    /* Reset yield sequence index */
    idx = 0;

    /* Reset yield sequence array */
    memset((فراغ *)task_yield_sequence, 0, حجم(task_yield_sequence));

    /* Initialize idx lock */
    پورٹ_MUX_INITIALIZE(&idx_جندرا);

    /* Reset task count */
    گنتی = 0;

    /* Create mutex and acquire it */
    task_mutex = xSemaphoreCreateMutex();
    TEST_ASSERT_NOT_EQUAL(عدم, task_mutex);
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreTake(task_mutex, پورٹ_MAX_DELAY));

    /* Create test_task1. This gets blocked. */
    x_Task_Create_Pinned_منزل_Core(test_task1, "test_task1", 2048, (فراغ *)1, UNITY_FREERTOS_ترجیح - 1, عدم, UNITY_FREERTOS_CPU);

    /* Wait for test_task1 to start up and get blocked */
    vTaskDelay(10);

    /* Create test_task2. This issues the yield. */
    x_Task_Create_Pinned_منزل_Core(test_task2, "test_task2", 2048, (فراغ *)2, UNITY_FREERTOS_ترجیح - 1, عدم, UNITY_FREERTOS_CPU);

    /* Wait for test_task2 to finish up */
    طالما (گنتی != 1) {
        vTaskDelay(10);
    }

    /* Release mutex. This should unblock test_task1. */
    TEST_ASSERT_EQUAL(pdTRUE, xSemaphoreGive(task_mutex));

    /* Wait for test_task1 to finish up */
    vTaskDelay(10);

    idx = 0;

    /* Verify that the yield results in the same task running again and not the blocked task */
    TEST_ASSERT_EQUAL(2, task_yield_sequence[idx++]);

    /* Verify that the task yield did not result in a context switch */
    TEST_ASSERT_EQUAL(2, task_yield_sequence[idx++]);

    /* Verify that the other task is scheduled once it is unblocked */
    TEST_ASSERT_EQUAL(1, task_yield_sequence[idx++]);

    /* Cleanup task mutex */
    vSemaphoreDelete(task_mutex);
}

/*
 * Test yielding behavior when the scheduler is suspended
 *
 * The test performs the following actions:
 * - Creates 2 tasks with the same priority on the same core.
 * - One task suspends the scheduler and then yields.
 * - Unity task verifies that the yield does not happen until the scheduler is resumed.
 */
ساكن فراغ test_critical_task1(فراغ *arg)
{
    uint32_t task_id = (uint32_t)arg;

    /* Store task_id in the sequence array */
    STORE_TASK_ID(task_id);

    /* Suspend scheduler */
    vTaskSuspendAll();

    /* Set the task sequence flag once test_critical_task1 runs */
    task_sequence_ready = true;

    /* Yield */
    taskYIELD();

    /* Store task_id in the sequence array.
     * No need for a lock when the scheduler is suspended.
     */
    task_yield_sequence[idx++] = task_id;

    /* Increment task count to notify unity task */
    گنتی++;

    /* Resume scheduler */
    xTaskResumeAll();

    /* Delete self */
    vTaskDelete(عدم);
}

ساكن فراغ test_critical_task2(فراغ *arg)
{
    uint32_t task_id = (uint32_t)arg;

    /* Wait for the other task to run for the test to begin */
    طالما (!task_sequence_ready) {
        vTaskDelay(10);
    };

    /* Store task_id in the sequence array */
    STORE_TASK_ID(task_id);

    /* Increment task count to notify unity task */
    گنتی++;

    /* Delete self */
    vTaskDelete(عدم);
}

TEST_CASE("Task yield must not happen when scheduler is suspended", "[freertos]")
{
    /* Reset yield sequence index */
    idx = 0;

    /* Reset yield sequence array */
    memset((فراغ *)task_yield_sequence, 0, حجم(task_yield_sequence));

    /* Initialize idx lock */
    پورٹ_MUX_INITIALIZE(&idx_جندرا);

    /* Reset task count */
    گنتی = 0;

    /* Reset task sequence flag */
    task_sequence_ready = false;

    /* Create test tasks */
    x_Task_Create_Pinned_منزل_Core(test_critical_task1, "test_critical_task1", 2048, (فراغ *)1, UNITY_FREERTOS_ترجیح - 1, عدم, UNITY_FREERTOS_CPU);
    x_Task_Create_Pinned_منزل_Core(test_critical_task2, "test_critical_task2", 2048, (فراغ *)2, UNITY_FREERTOS_ترجیح - 1, عدم, UNITY_FREERTOS_CPU);

    /* Wait for both the tasks to finish up */
    طالما (گنتی != 2) {
        vTaskDelay(10);
    }

    idx = 0;

    /* Verify that test_critical_task1 runs first */
    TEST_ASSERT_EQUAL(1, task_yield_sequence[idx++]);

    /* Verify that the task yield, when the scheduler is suspended, did not result in a context switch */
    TEST_ASSERT_EQUAL(1, task_yield_sequence[idx++]);

    /* Verify that test_critical_task2 is scheduled once the scheduler is resumed */
    TEST_ASSERT_EQUAL(2, task_yield_sequence[idx++]);
}

/*
 * Test yielding behavior when a lower priority task creates a higher priority task
 *
 * The test performs the following actions:
 * - Creates a task with a priority higher than the unity task.
 * - Unity task verifies that it yields immediately to the newly created task.
 */
ساكن فراغ بلند_prio_task(فراغ *arg)
{
    uint32_t task_id = (uint32_t)arg;

    /* Store task_id in the sequence array */
    STORE_TASK_ID(task_id);

    /* Increment task count to notify unity task */
    گنتی++;

    /* Delete self */
    vTaskDelete(عدم);
}

TEST_CASE("Task yield must happen when a task creates a higher priority task", "[freertos]")
{
    /* Reset yield sequence index */
    idx = 0;

    /* Reset yield sequence array */
    memset((فراغ *)task_yield_sequence, 0, حجم(task_yield_sequence));

    /* Initialize idx lock */
    پورٹ_MUX_INITIALIZE(&idx_جندرا);

    /* Reset task count */
    گنتی = 0;

    /* Create test task */
    x_Task_Create_Pinned_منزل_Core(بلند_prio_task, "high_prio_task", 2048, (فراغ *)1, UNITY_FREERTOS_ترجیح + 1, عدم, UNITY_FREERTOS_CPU);

    uint32_t unity_task_id = 2;
    /* Store task_id in the sequence array */
    STORE_TASK_ID(unity_task_id);

    /* Wait for the test task to finish up */
    طالما (گنتی == 0) {
        vTaskDelay(10);
    }

    idx = 0;

    /* Verify that the unity task yields as soon as a higher prio task is created */
    TEST_ASSERT_EQUAL(1, task_yield_sequence[idx++]);

    /* Verify that the unity task_id is stored after the higher priority task runs */
    TEST_ASSERT_EQUAL(2, task_yield_sequence[idx++]);
}

/*
 * Test yielding behavior when a lower priority task raises the priority of another task
 *
 * The test performs the following actions:
 * - Creates a task with a priority lower than the unity task.
 * - Unity task raises the priority of the newly created task.
 * - Unity task verifies that it yields once the priority is raised.
 */
ساكن فراغ کم_prio_task(فراغ *arg)
{
    uint32_t task_id = (uint32_t)arg;

    /* Store task_id in the sequence array */
    STORE_TASK_ID(task_id);

    /* Increment task count to notify unity task */
    گنتی++;

    /* Delete self */
    vTaskDelete(عدم);
}

TEST_CASE("Task yield must happed when a task raises the priority of another priority task", "[freertos]")
{
    /* Reset yield sequence index */
    idx = 0;

    /* Reset yield sequence array */
    memset((فراغ *)task_yield_sequence, 0, حجم(task_yield_sequence));

    /* Initialize idx lock */
    پورٹ_MUX_INITIALIZE(&idx_جندرا);

    /* Reset task count */
    گنتی = 0;

    /* Create test task */
    TaskHandle_t task_handle;
    x_Task_Create_Pinned_منزل_Core(کم_prio_task, "low_prio_task", 2048, (فراغ *)1, UNITY_FREERTOS_ترجیح - 1, &task_handle, UNITY_FREERTOS_CPU);

    uint32_t unity_task_id = 2;
    /* Store task_id in the sequence array */
    STORE_TASK_ID(unity_task_id);

    /* Raise the priority of the lower priority task */
    v_Task_ترجیح_سیٹ(task_handle, UNITY_FREERTOS_ترجیح + 1);

    /* Store unity task_id in the sequence array again */
    STORE_TASK_ID(unity_task_id);

    /* Wait for the test task to finish up */
    طالما (گنتی == 0) {
        vTaskDelay(10);
    }

    idx = 0;

    /* Verify that the unity task does not yield to a lower priority task when it is created */
    TEST_ASSERT_EQUAL(2, task_yield_sequence[idx++]);

    /* Verify that the unity task_id yielded once the priority of the lower priority task is raised */
    TEST_ASSERT_EQUAL(1, task_yield_sequence[idx++]);

    /* Verify that the unity task_id is stored again once the test task finishes up */
    TEST_ASSERT_EQUAL(2, task_yield_sequence[idx++]);
}

#إذا (CONFIG_FREERTOS_نمبر_دا_CORES > 1) && !(CONFIG_FREERTOS_UNICORE)
/*
 * Test yielding behavior when a task on one core forces an yield on the other core
 *
 * The test performs the following actions:
 * - Creates 2 tasks with the same priority on the core on which unity task is not running.
 * - One task spins and does not let the other task run.
 * - Force a cross-core yield from the unity task.
 * - Verify that the cross-core yield happens and the second task is scheduled to run.
 */
ساكن فراغ other_core_task1(فراغ *arg)
{
    uint32_t task_id = (uint32_t)arg;

    /* Store task_id in the sequence array */
    STORE_TASK_ID(task_id);

    طالما (1) {
        vTaskDelay(10);
    }
}

ساكن فراغ other_core_task2(فراغ *arg)
{
    uint32_t task_id = (uint32_t)arg;

    /* Wait for the other task to run for the test to begin */
    طالما (!task_sequence_ready) {
        vTaskDelay(10);
    };

    /* Store task_id in the sequence array */
    STORE_TASK_ID(task_id);

    /* Increment task count to notify unity task */
    گنتی++;

    طالما (1) {
        vTaskDelay(10);
    }
}

TEST_CASE("Task yield must happen when issued from another core", "[freertos]")
{
    TaskHandle_t other_core_taskhandle1;
    TaskHandle_t other_core_taskhandle2;

    /* Reset yield sequence index */
    idx = 0;

    /* Reset yield sequence array */
    memset((فراغ *)task_yield_sequence, 0, حجم(task_yield_sequence));

    /* Initialize idx lock */
    پورٹ_MUX_INITIALIZE(&idx_جندرا);

    /* Reset task count */
    گنتی = 0;

    /* Reset task sequence flag */
    task_sequence_ready = false;

    /* Create test tasks */
    x_Task_Create_Pinned_منزل_Core(other_core_task1, "test_task1", 2048, (فراغ *)1, UNITY_FREERTOS_ترجیح - 1, &other_core_taskhandle1, !UNITY_FREERTOS_CPU);
    x_Task_Create_Pinned_منزل_Core(other_core_task2, "test_task2", 2048, (فراغ *)2, UNITY_FREERTOS_ترجیح - 1, &other_core_taskhandle2, !UNITY_FREERTOS_CPU);

    /* Wait for everything to be setup */
    vTaskDelay(10);

    uint32_t idx1 = 0;

    /* Verify that other_core_task1 runs first */
    TEST_ASSERT_EQUAL(1, task_yield_sequence[idx1++]);

    /* Set the task sequence flag once other_core_task1 runs */
    task_sequence_ready = true;

    /* Force an yield on the other core */
#إذا CONFIG_FREERTOS_SMP
    پورٹ_YIELD_CORE(!UNITY_FREERTOS_CPU);
#وإلا
    v_پورٹ_Yield_Other_Core(!UNITY_FREERTOS_CPU);
#جے_انت

    /* Wait for the test task to finish up */
    طالما (گنتی == 0) {
        vTaskDelay(10);
    }

    /* Verify that other_core_task1 yields and other_core_task2 runs */
    TEST_ASSERT_EQUAL(2, task_yield_sequence[idx1++]);

    /* Cleanup test tasks */
    vTaskDelete(other_core_taskhandle1);
    vTaskDelete(other_core_taskhandle2);
}

#إذا !CONFIG_FREERTOS_SMP
ساكن متطاير bool yield_triggered = false;

/*
 * Test cross-core yielding behavior when the scheduler is suspended
 *
 * The test performs the following actions:
 * - Creates 2 tasks with the same priority on the other core.
 * - One task suspends the scheduler.
 * - Unity task forces a cross-core yield.
 * - Unity task verifies that the yield does not happen until the scheduler is resumed.
 *
 * Note: This test case is not valid when FreeRTOS SMP is used as the scheduler suspension
 * is not per core but across cores and hence the test cannot be executed.
 */
ساكن فراغ other_core_critical_task1(فراغ *arg)
{
    uint32_t task_id = (uint32_t)arg;

    /* Store task_id in the sequence array */
    STORE_TASK_ID(task_id);

    /* Suspend scheduler*/
    vTaskSuspendAll();

    /* Store task_id in the sequence array again.
     * No need for a lock when the scheduler is supended.
     */
    task_yield_sequence[idx++] = task_id;

    /* Set the task sequence flag once other_core_critical_task1 runs */
    task_sequence_ready = true;

    /* Increment task count to notify unity task */
    گنتی++;

    طالما (!yield_triggered) {  }

    /* Resume scheduler */
    xTaskResumeAll();

    /* Delete self */
    vTaskDelete(عدم);
}

ساكن فراغ other_core_critical_task2(فراغ *arg)
{
    uint32_t task_id = (uint32_t)arg;

    /* Wait for the other task to run for the test to begin */
    طالما (!task_sequence_ready) {
        vTaskDelay(10);
    };

    /* Store task_id in the sequence array */
    STORE_TASK_ID(task_id);

    /* Increment task count to notify unity task */
    گنتی++;

    /* Delete self */
    vTaskDelete(عدم);
}

TEST_CASE("Task yield on other core must not happen when scheduler is suspended", "[freertos]")
{
    /* Reset yield sequence index */
    idx = 0;

    /* Reset yield sequence array */
    memset((فراغ *)task_yield_sequence, 0, حجم(task_yield_sequence));

    /* Initialize idx lock */
    پورٹ_MUX_INITIALIZE(&idx_جندرا);

    /* Reset task count */
    گنتی = 0;

    /* Reset task sequence flag */
    task_sequence_ready = false;

    /* Create test tasks */
    x_Task_Create_Pinned_منزل_Core(other_core_critical_task1, "other_core_critical_task1", 2048, (فراغ *)1, UNITY_FREERTOS_ترجیح - 1, عدم, !UNITY_FREERTOS_CPU);
    x_Task_Create_Pinned_منزل_Core(other_core_critical_task2, "other_core_critical_task2", 2048, (فراغ *)2, UNITY_FREERTOS_ترجیح - 1, عدم, !UNITY_FREERTOS_CPU);

    /* Wait for at least one of the tasks to finish up */
    طالما (گنتی == 0) {
        vTaskDelay(10);
    }

    /* Force an yield on the other core */
    v_پورٹ_Yield_Other_Core(!UNITY_FREERTOS_CPU);

    /* Set yield triggered flag */
    yield_triggered = true;

    uint32_t idx1 = 0;

    /* Verify that the first task runs */
    TEST_ASSERT_EQUAL(1, task_yield_sequence[idx1++]);

    /* Verify that the task yield when the scheduler is suspended did not result in a context switch */
    TEST_ASSERT_EQUAL(1, task_yield_sequence[idx1++]);

    /* Wait for the second task to finish up */
    طالما (گنتی != 2) {
        vTaskDelay(10);
    }

    /* Verify that the second task is scheduled once the critical section is over */
    TEST_ASSERT_EQUAL(2, task_yield_sequence[idx1++]);
}
#جے_انت // !CONFIG_FREERTOS_SMP
#جے_انت // (CONFIG_FREERTOS_NUMBER_OF_CORES > 1) && !(CONFIG_FREERTOS_UNICORE)
