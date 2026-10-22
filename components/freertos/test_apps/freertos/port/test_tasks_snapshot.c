/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/freertos_debug.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_cpu.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_rom_sys.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unity.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TEST_MAX_TASKS_ਅੰਕ      32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਅੰਕ_TASKS_ਪ੍ਰਤੀ_LIST      2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TASK_ਤਰਜੀਹ           (configMAX_PRIORITIES - 2)

ਸਥਿਰ ਅਸਥਿਰ bool ਸ਼ੁਰੂ_ready_flag = false;
ਸਥਿਰ ਅਸਥਿਰ bool ਸ਼ੁਰੂ_resp_flag = false;
ਸਥਿਰ ਅਸਥਿਰ bool stop_flag = false;

ਸਥਿਰ ਖਾਲੀ ready_task(ਖਾਲੀ *arg)
{
    ਜਦੋਂ (1) {
#ਜੇ !CONFIG_FREERTOS_UNICORE
        taskDISABLE_INTERRUPTS();
        /*
        The main task (on core 0) has a higher priority than this task. Thus, by the time this flag is set, this task
        is guaranteed to running on core 1.
        */
        ਜੇ (ਸ਼ੁਰੂ_ready_flag) {   // Is the test starting?
            ਸ਼ੁਰੂ_resp_flag = true;   // Indicate we are ready to start
            ਜਦੋਂ (!stop_flag) {
                ;   // Wait until test completes
            }

            // Reset the flags
            ਸ਼ੁਰੂ_ready_flag = false;
            ਸ਼ੁਰੂ_resp_flag = false;
            stop_flag = false;
        }
        taskENABLE_INTERRUPTS();
        // Short non-blocking delay to allow INT WDT to feed
        esp_rom_delay_us(1000);
#ਜੇ_ਅੰਤ // !CONFIG_FREERTOS_UNICORE
    }
}

ਸਥਿਰ ਖਾਲੀ blocked_task(ਖਾਲੀ *arg)
{
    // Delay for portMAX_DELAY - 1 as not to go on the suspended list
    vTaskDelay(ਪੋਰਟ_MAX_DELAY - 1);
}

ਸਥਿਰ ਖਾਲੀ suspended_task(ਖਾਲੀ *arg)
{
    vTaskSuspend(ਨਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ ਤਿਆਰੀ(TaskHandle_t *task_list, ਪੂਰਨ *ਅੰਕ_tasks_ret, UBaseType_t *old_ਤਰਜੀਹ_ret)
{
    // Raise our priority so that we aren't preempted
    *old_ਤਰਜੀਹ_ret = ux_Task_ਤਰਜੀਹ_Get(ਨਿਰਾ);
    v_Task_ਤਰਜੀਹ_ਸੈੱਟ(ਨਿਰਾ, configMAX_PRIORITIES - 1);

    // Create tasks
    ਪੂਰਨ ਅੰਕ_tasks = 0;
    ਲਈ (ਪੂਰਨ i = 0; i < ਅੰਕ_TASKS_ਪ੍ਰਤੀ_LIST; i++) {
        //Ready task
        xTaskCreate(ready_task, "ready", 1024, ਨਿਰਾ, TASK_ਤਰਜੀਹ, &(task_list[ਅੰਕ_tasks]));
        ਅੰਕ_tasks++;
        //Blocked task
        xTaskCreate(blocked_task, "blkd", 1024, ਨਿਰਾ, TASK_ਤਰਜੀਹ, &(task_list[ਅੰਕ_tasks]));
        ਅੰਕ_tasks++;
        //Suspended task
        xTaskCreate(suspended_task, "susp", 1024, ਨਿਰਾ, TASK_ਤਰਜੀਹ, &(task_list[ਅੰਕ_tasks]));
        ਅੰਕ_tasks++;
    }
    *ਅੰਕ_tasks_ret = ਅੰਕ_tasks;
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
#ਜੇ !CONFIG_FREERTOS_UNICORE
    // For SMP, we need to do a 2-way handshake to ensure the other core spins with interrupts disabled.
    ਸ਼ੁਰੂ_ready_flag = true;
    ਜਦੋਂ (!ਸ਼ੁਰੂ_resp_flag) {
        ;
    }
#ਜੇ_ਅੰਤ // !CONFIG_FREERTOS_UNICORE
}

ਸਥਿਰ ਖਾਲੀ check_snapshots(TaskHandle_t *task_list, ਪੂਰਨ ਅੰਕ_tasks, TaskSnapshot_t *task_snapshots, UBaseType_t ਅੰਕ_snapshots)
{
    // Check task snapshots. Every created task should be found in the task snapshot
    ਲਈ (ਪੂਰਨ i = 0; i < ਅੰਕ_tasks; i++) {
        bool ਮਿਲਿਆ = false;
        ਲਈ (ਪੂਰਨ j = 0; j < ਅੰਕ_snapshots; j++) {
            ਜੇ (task_list[i] == (TaskHandle_t)task_snapshots[j].pxTCB) {
                ਮਿਲਿਆ = true;
                ਰੋਕੋ;
            }
        }
        TEST_ASSERT(ਮਿਲਿਆ);
    }
}

ਸਥਿਰ ਖਾਲੀ teardown(TaskHandle_t *task_list, ਪੂਰਨ ਅੰਕ_tasks, UBaseType_t old_ਤਰਜੀਹ)
{
#ਜੇ !CONFIG_FREERTOS_UNICORE
    // Signal to the other core that it can exit the loop
    stop_flag = true;
#ਜੇ_ਅੰਤ // !CONFIG_FREERTOS_UNICORE
    // Re-enable interrupts on the current core.
    taskENABLE_INTERRUPTS();

    ਲਈ (ਪੂਰਨ i = 0; i < ਅੰਕ_tasks; i++) {
        vTaskDelete(task_list[i]);
    }
    // Restore priority
    v_Task_ਤਰਜੀਹ_ਸੈੱਟ(ਨਿਰਾ, old_ਤਰਜੀਹ);
    // Short delay to allow tasks to clean up
    vTaskDelay(10);
}

TEST_CASE("Task snapshot: Get all", "[freertos]")
{
    // Short delay to allow both cores to spin up
    vTaskDelay(10);

    TaskHandle_t task_list[TEST_MAX_TASKS_ਅੰਕ];
    ਪੂਰਨ ਅੰਕ_tasks;
    UBaseType_t old_ਤਰਜੀਹ;
    ਤਿਆਰੀ(task_list, &ਅੰਕ_tasks, &old_ਤਰਜੀਹ);

    // Get task snapshots using uxTaskGetSnapshotAll()
    TaskSnapshot_t task_snapshots[TEST_MAX_TASKS_ਅੰਕ];
    UBaseType_t tcb_ਮਾਪ;
    UBaseType_t ਅੰਕ_snapshots;
    ਅੰਕ_snapshots = uxTaskGetSnapshotAll(task_snapshots, TEST_MAX_TASKS_ਅੰਕ, &tcb_ਮਾਪ);
    TEST_ASSERT_LESS_OR_EQUAL(TEST_MAX_TASKS_ਅੰਕ, ਅੰਕ_snapshots);

    check_snapshots(task_list, ਅੰਕ_tasks, task_snapshots, ਅੰਕ_snapshots);
    teardown(task_list, ਅੰਕ_tasks, old_ਤਰਜੀਹ);
}

TEST_CASE("Task snapshot: Iterate", "[freertos]")
{
    // Short delay to allow both cores to spin up
    vTaskDelay(10);

    TaskHandle_t task_list[TEST_MAX_TASKS_ਅੰਕ];
    ਪੂਰਨ ਅੰਕ_tasks;
    UBaseType_t old_ਤਰਜੀਹ;
    ਤਿਆਰੀ(task_list, &ਅੰਕ_tasks, &old_ਤਰਜੀਹ);

    // Get task snapshots using xTaskGetNext() and vTaskGetSnapshot()
    TaskSnapshot_t task_snapshots[TEST_MAX_TASKS_ਅੰਕ];
    UBaseType_t ਅੰਕ_snapshots = 0;
    TaskIterator_t task_iterator = {0};
    ਜਦੋਂ (x_Task_Get_ਅਗਲਾ(&task_iterator) != -1) {
        ਜੇ (task_iterator.pxTaskHandle != ਨਿਰਾ) {
            // Get the task's snapshot
            ਅਧਾਰ_Type_t ਨਤੀਜਾ_2 = vTaskGetSnapshot(task_iterator.pxTaskHandle, &task_snapshots[ਅੰਕ_snapshots]);
            TEST_ASSERT_EQUAL(pdTRUE, ਨਤੀਜਾ_2);
            ਅੰਕ_snapshots++;
        }
    }
    TEST_ASSERT_LESS_OR_EQUAL(TEST_MAX_TASKS_ਅੰਕ, ਅੰਕ_snapshots);

    check_snapshots(task_list, ਅੰਕ_tasks, task_snapshots, ਅੰਕ_snapshots);
    teardown(task_list, ਅੰਕ_tasks, old_ਤਰਜੀਹ);
}
