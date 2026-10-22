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

#تعریف_کرو TEST_MAX_TASKS_عدد      32
#تعریف_کرو عدد_TASKS_فی_LIST      2
#تعریف_کرو TASK_ترجیح           (configMAX_PRIORITIES - 2)

ساکن متلون bool آغاز_ready_flag = false;
ساکن متلون bool آغاز_resp_flag = false;
ساکن متلون bool stop_flag = false;

ساکن خالی ready_task(خالی *arg)
{
    جب (1) {
#اگر !CONFIG_FREERTOS_UNICORE
        taskDISABLE_INTERRUPTS();
        /*
        The main task (on core 0) has a higher priority than this task. Thus, by the time this flag is set, this task
        is guaranteed to running on core 1.
        */
        اگر (آغاز_ready_flag) {   // Is the test starting?
            آغاز_resp_flag = true;   // Indicate we are ready to start
            جب (!stop_flag) {
                ;   // Wait until test completes
            }

            // Reset the flags
            آغاز_ready_flag = false;
            آغاز_resp_flag = false;
            stop_flag = false;
        }
        taskENABLE_INTERRUPTS();
        // Short non-blocking delay to allow INT WDT to feed
        esp_rom_delay_us(1000);
#اگر_ختم // !CONFIG_FREERTOS_UNICORE
    }
}

ساکن خالی blocked_task(خالی *arg)
{
    // Delay for portMAX_DELAY - 1 as not to go on the suspended list
    vTaskDelay(پورٹ_MAX_DELAY - 1);
}

ساکن خالی suspended_task(خالی *arg)
{
    vTaskSuspend(تہی);
}

ساکن خالی تیاری(TaskHandle_t *task_list, صحیح *عدد_tasks_ret, UBaseType_t *old_ترجیح_ret)
{
    // Raise our priority so that we aren't preempted
    *old_ترجیح_ret = ux_Task_ترجیح_Get(تہی);
    v_Task_ترجیح_سیٹ(تہی, configMAX_PRIORITIES - 1);

    // Create tasks
    صحیح عدد_tasks = 0;
    برائے (صحیح i = 0; i < عدد_TASKS_فی_LIST; i++) {
        //Ready task
        xTaskCreate(ready_task, "ready", 1024, تہی, TASK_ترجیح, &(task_list[عدد_tasks]));
        عدد_tasks++;
        //Blocked task
        xTaskCreate(blocked_task, "blkd", 1024, تہی, TASK_ترجیح, &(task_list[عدد_tasks]));
        عدد_tasks++;
        //Suspended task
        xTaskCreate(suspended_task, "susp", 1024, تہی, TASK_ترجیح, &(task_list[عدد_tasks]));
        عدد_tasks++;
    }
    *عدد_tasks_ret = عدد_tasks;
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
    آغاز_ready_flag = true;
    جب (!آغاز_resp_flag) {
        ;
    }
#اگر_ختم // !CONFIG_FREERTOS_UNICORE
}

ساکن خالی check_snapshots(TaskHandle_t *task_list, صحیح عدد_tasks, TaskSnapshot_t *task_snapshots, UBaseType_t عدد_snapshots)
{
    // Check task snapshots. Every created task should be found in the task snapshot
    برائے (صحیح i = 0; i < عدد_tasks; i++) {
        bool ملا = false;
        برائے (صحیح j = 0; j < عدد_snapshots; j++) {
            اگر (task_list[i] == (TaskHandle_t)task_snapshots[j].pxTCB) {
                ملا = true;
                توڑو;
            }
        }
        TEST_ASSERT(ملا);
    }
}

ساکن خالی teardown(TaskHandle_t *task_list, صحیح عدد_tasks, UBaseType_t old_ترجیح)
{
#اگر !CONFIG_FREERTOS_UNICORE
    // Signal to the other core that it can exit the loop
    stop_flag = true;
#اگر_ختم // !CONFIG_FREERTOS_UNICORE
    // Re-enable interrupts on the current core.
    taskENABLE_INTERRUPTS();

    برائے (صحیح i = 0; i < عدد_tasks; i++) {
        vTaskDelete(task_list[i]);
    }
    // Restore priority
    v_Task_ترجیح_سیٹ(تہی, old_ترجیح);
    // Short delay to allow tasks to clean up
    vTaskDelay(10);
}

TEST_CASE("Task snapshot: Get all", "[freertos]")
{
    // Short delay to allow both cores to spin up
    vTaskDelay(10);

    TaskHandle_t task_list[TEST_MAX_TASKS_عدد];
    صحیح عدد_tasks;
    UBaseType_t old_ترجیح;
    تیاری(task_list, &عدد_tasks, &old_ترجیح);

    // Get task snapshots using uxTaskGetSnapshotAll()
    TaskSnapshot_t task_snapshots[TEST_MAX_TASKS_عدد];
    UBaseType_t tcb_سائز;
    UBaseType_t عدد_snapshots;
    عدد_snapshots = uxTaskGetSnapshotAll(task_snapshots, TEST_MAX_TASKS_عدد, &tcb_سائز);
    TEST_ASSERT_LESS_OR_EQUAL(TEST_MAX_TASKS_عدد, عدد_snapshots);

    check_snapshots(task_list, عدد_tasks, task_snapshots, عدد_snapshots);
    teardown(task_list, عدد_tasks, old_ترجیح);
}

TEST_CASE("Task snapshot: Iterate", "[freertos]")
{
    // Short delay to allow both cores to spin up
    vTaskDelay(10);

    TaskHandle_t task_list[TEST_MAX_TASKS_عدد];
    صحیح عدد_tasks;
    UBaseType_t old_ترجیح;
    تیاری(task_list, &عدد_tasks, &old_ترجیح);

    // Get task snapshots using xTaskGetNext() and vTaskGetSnapshot()
    TaskSnapshot_t task_snapshots[TEST_MAX_TASKS_عدد];
    UBaseType_t عدد_snapshots = 0;
    TaskIterator_t task_iterator = {0};
    جب (x_Task_Get_اگلا(&task_iterator) != -1) {
        اگر (task_iterator.pxTaskHandle != تہی) {
            // Get the task's snapshot
            بنیاد_Type_t نتیجہ_2 = vTaskGetSnapshot(task_iterator.pxTaskHandle, &task_snapshots[عدد_snapshots]);
            TEST_ASSERT_EQUAL(pdTRUE, نتیجہ_2);
            عدد_snapshots++;
        }
    }
    TEST_ASSERT_LESS_OR_EQUAL(TEST_MAX_TASKS_عدد, عدد_snapshots);

    check_snapshots(task_list, عدد_tasks, task_snapshots, عدد_snapshots);
    teardown(task_list, عدد_tasks, old_ترجیح);
}
