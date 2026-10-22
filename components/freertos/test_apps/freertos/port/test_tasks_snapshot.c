/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#شامل "sdkconfig.h"
#شامل <stdio.h>
#شامل <stdbool.h>
#شامل "freertos/FreeRTOS.h"
#شامل "freertos/task.h"
#شامل "freertos/freertos_debug.h"
#شامل "esp_cpu.h"
#شامل "esp_rom_sys.h"
#شامل "unity.h"
#شامل "sdkconfig.h"

#define TEST_MAX_TASKS_رقم      32
#define رقم_TASKS_PER_LIST      2
#define TASK_PRIORITY           (configMAX_PRIORITIES - 2)

ایستا فرار bool start_ready_flag = false;
ایستا فرار bool start_resp_flag = false;
ایستا فرار bool stop_flag = false;

ایستا تهی ready_task(تهی *arg)
{
    وقتی (1) {
#اگر !CONFIG_FREERTOS_UNICORE
        taskDISABLE_INTERRUPTS();
        /*
        The main task (on core 0) has a higher priority than this task. Thus, by the time this flag is set, this task
        is guaranteed to running on core 1.
        */
        اگر (start_ready_flag) {   // Is the test starting?
            start_resp_flag = true;   // Indicate we are ready to start
            وقتی (!stop_flag) {
                ;   // Wait until test completes
            }

            // Reset the flags
            start_ready_flag = false;
            start_resp_flag = false;
            stop_flag = false;
        }
        taskENABLE_INTERRUPTS();
        // Short non-blocking delay to allow INT WDT to feed
        esp_rom_delay_us(1000);
#endif // !CONFIG_FREERTOS_UNICORE
    }
}

ایستا تهی blocked_task(تهی *arg)
{
    // Delay for portMAX_DELAY - 1 as not to go on the suspended list
    vTaskDelay(portMAX_DELAY - 1);
}

ایستا تهی suspended_task(تهی *arg)
{
    vTaskSuspend(پوچ);
}

ایستا تهی setup(TaskHandle_t *task_list, صحیح *رقم_tasks_ret, UBaseType_t *old_priority_ret)
{
    // Raise our priority so that we aren't preempted
    *old_priority_ret = uxTaskPriorityGet(پوچ);
    vTaskPrioritySet(پوچ, configMAX_PRIORITIES - 1);

    // Create tasks
    صحیح رقم_tasks = 0;
    برای (صحیح i = 0; i < رقم_TASKS_PER_LIST; i++) {
        //Ready task
        xTaskCreate(ready_task, "ready", 1024, پوچ, TASK_PRIORITY, &(task_list[رقم_tasks]));
        رقم_tasks++;
        //Blocked task
        xTaskCreate(blocked_task, "blkd", 1024, پوچ, TASK_PRIORITY, &(task_list[رقم_tasks]));
        رقم_tasks++;
        //Suspended task
        xTaskCreate(suspended_task, "susp", 1024, پوچ, TASK_PRIORITY, &(task_list[رقم_tasks]));
        رقم_tasks++;
    }
    *رقم_tasks_ret = رقم_tasks;
    // Short delay to allow tasks to spin up
    vTaskDelay(10);

    /*
    Task snapshot functions are only meant to be called when FreeRTOS is no longer running (e.g., during a panic),
    thus assumes that nothing else (interrupt or the other core) will access the task lists simultaneously. However, for
    unit tests, FreeRTOS is still running. So we must guarantee thread safe access another way.

    - For single core, simply disabling interrupts will guarantee that the current task is the only thread to access the
      task lists
    - For SMP, we also need to stop the other core from accessing the task lists. Thus, we use a 2-way handshake to put
      the other core into a while loop with interrupts disabled.
    */
    // Disable interrupts so that the current core isn't preempted.
    taskDISABLE_INTERRUPTS();
#اگر !CONFIG_FREERTOS_UNICORE
    // For SMP, we need to do a 2-way handshake to ensure the other core spins with interrupts disabled.
    start_ready_flag = true;
    وقتی (!start_resp_flag) {
        ;
    }
#endif // !CONFIG_FREERTOS_UNICORE
}

ایستا تهی check_snapshots(TaskHandle_t *task_list, صحیح رقم_tasks, TaskSnapshot_t *task_snapshots, UBaseType_t رقم_snapshots)
{
    // Check task snapshots. Every created task should be found in the task snapshot
    برای (صحیح i = 0; i < رقم_tasks; i++) {
        bool found = false;
        برای (صحیح j = 0; j < رقم_snapshots; j++) {
            اگر (task_list[i] == (TaskHandle_t)task_snapshots[j].pxTCB) {
                found = true;
                بشکن;
            }
        }
        TEST_ASSERT(found);
    }
}

ایستا تهی teardown(TaskHandle_t *task_list, صحیح رقم_tasks, UBaseType_t old_priority)
{
#اگر !CONFIG_FREERTOS_UNICORE
    // Signal to the other core that it can exit the loop
    stop_flag = true;
#endif // !CONFIG_FREERTOS_UNICORE
    // Re-enable interrupts on the current core.
    taskENABLE_INTERRUPTS();

    برای (صحیح i = 0; i < رقم_tasks; i++) {
        vTaskDelete(task_list[i]);
    }
    // Restore priority
    vTaskPrioritySet(پوچ, old_priority);
    // Short delay to allow tasks to clean up
    vTaskDelay(10);
}

TEST_CASE("Task snapshot: Get all", "[freertos]")
{
    // Short delay to allow both cores to spin up
    vTaskDelay(10);

    TaskHandle_t task_list[TEST_MAX_TASKS_رقم];
    صحیح رقم_tasks;
    UBaseType_t old_priority;
    setup(task_list, &رقم_tasks, &old_priority);

    // Get task snapshots using uxTaskGetSnapshotAll()
    TaskSnapshot_t task_snapshots[TEST_MAX_TASKS_رقم];
    UBaseType_t tcb_size;
    UBaseType_t رقم_snapshots;
    رقم_snapshots = uxTaskGetSnapshotAll(task_snapshots, TEST_MAX_TASKS_رقم, &tcb_size);
    TEST_ASSERT_LESS_OR_EQUAL(TEST_MAX_TASKS_رقم, رقم_snapshots);

    check_snapshots(task_list, رقم_tasks, task_snapshots, رقم_snapshots);
    teardown(task_list, رقم_tasks, old_priority);
}

TEST_CASE("Task snapshot: Iterate", "[freertos]")
{
    // Short delay to allow both cores to spin up
    vTaskDelay(10);

    TaskHandle_t task_list[TEST_MAX_TASKS_رقم];
    صحیح رقم_tasks;
    UBaseType_t old_priority;
    setup(task_list, &رقم_tasks, &old_priority);

    // Get task snapshots using xTaskGetNext() and vTaskGetSnapshot()
    TaskSnapshot_t task_snapshots[TEST_MAX_TASKS_رقم];
    UBaseType_t رقم_snapshots = 0;
    TaskIterator_t task_iterator = {0};
    وقتی (xTaskGetNext(&task_iterator) != -1) {
        اگر (task_iterator.pxTaskHandle != پوچ) {
            // Get the task's snapshot
            BaseType_t Result = vTaskGetSnapshot(task_iterator.pxTaskHandle, &task_snapshots[رقم_snapshots]);
            TEST_ASSERT_EQUAL(pdTRUE, Result);
            رقم_snapshots++;
        }
    }
    TEST_ASSERT_LESS_OR_EQUAL(TEST_MAX_TASKS_رقم, رقم_snapshots);

    check_snapshots(task_list, رقم_tasks, task_snapshots, رقم_snapshots);
    teardown(task_list, رقم_tasks, old_priority);
}
