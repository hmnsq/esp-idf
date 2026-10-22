/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن "sdkconfig.h"
#ضمن <stdio.h>
#ضمن <stdbool.h>
#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن "freertos/freertos_debug.h"
#ضمن "esp_cpu.h"
#ضمن "esp_rom_sys.h"
#ضمن "unity.h"
#ضمن "sdkconfig.h"

#تعریف_کرو TEST_MAX_TASKS_عدد      32
#تعریف_کرو عدد_TASKS_فی_LIST      2
#تعریف_کرو TASK_ترجیح           (configMAX_PRIORITIES - 2)

ساكن متطاير bool شروع_ready_flag = false;
ساكن متطاير bool شروع_resp_flag = false;
ساكن متطاير bool stop_flag = false;

ساكن فراغ ready_task(فراغ *arg)
{
    طالما (1) {
#إذا !CONFIG_FREERTOS_UNICORE
        taskDISABLE_INTERRUPTS();
        /*
        The main task (on core 0) has a higher priority than this task. Thus, by the time this flag is set, this task
        is guaranteed to running on core 1.
        */
        إذا (شروع_ready_flag) {   // Is the test starting?
            شروع_resp_flag = true;   // Indicate we are ready to start
            طالما (!stop_flag) {
                ;   // Wait until test completes
            }

            // Reset the flags
            شروع_ready_flag = false;
            شروع_resp_flag = false;
            stop_flag = false;
        }
        taskENABLE_INTERRUPTS();
        // Short non-blocking delay to allow INT WDT to feed
        esp_rom_delay_us(1000);
#جے_انت // !CONFIG_FREERTOS_UNICORE
    }
}

ساكن فراغ blocked_task(فراغ *arg)
{
    // Delay for portMAX_DELAY - 1 as not to go on the suspended list
    vTaskDelay(پورٹ_MAX_DELAY - 1);
}

ساكن فراغ suspended_task(فراغ *arg)
{
    vTaskSuspend(عدم);
}

ساكن فراغ تیاری(TaskHandle_t *task_list, صحيح *عدد_tasks_ret, UBaseType_t *old_ترجیح_ret)
{
    // Raise our priority so that we aren't preempted
    *old_ترجیح_ret = ux_Task_ترجیح_Get(عدم);
    v_Task_ترجیح_سیٹ(عدم, configMAX_PRIORITIES - 1);

    // Create tasks
    صحيح عدد_tasks = 0;
    لكل (صحيح i = 0; i < عدد_TASKS_فی_LIST; i++) {
        //Ready task
        xTaskCreate(ready_task, "ready", 1024, عدم, TASK_ترجیح, &(task_list[عدد_tasks]));
        عدد_tasks++;
        //Blocked task
        xTaskCreate(blocked_task, "blkd", 1024, عدم, TASK_ترجیح, &(task_list[عدد_tasks]));
        عدد_tasks++;
        //Suspended task
        xTaskCreate(suspended_task, "susp", 1024, عدم, TASK_ترجیح, &(task_list[عدد_tasks]));
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
#إذا !CONFIG_FREERTOS_UNICORE
    // For SMP, we need to do a 2-way handshake to ensure the other core spins with interrupts disabled.
    شروع_ready_flag = true;
    طالما (!شروع_resp_flag) {
        ;
    }
#جے_انت // !CONFIG_FREERTOS_UNICORE
}

ساكن فراغ check_snapshots(TaskHandle_t *task_list, صحيح عدد_tasks, TaskSnapshot_t *task_snapshots, UBaseType_t عدد_snapshots)
{
    // Check task snapshots. Every created task should be found in the task snapshot
    لكل (صحيح i = 0; i < عدد_tasks; i++) {
        bool لبھیا = false;
        لكل (صحيح j = 0; j < عدد_snapshots; j++) {
            إذا (task_list[i] == (TaskHandle_t)task_snapshots[j].pxTCB) {
                لبھیا = true;
                اكسر;
            }
        }
        TEST_ASSERT(لبھیا);
    }
}

ساكن فراغ teardown(TaskHandle_t *task_list, صحيح عدد_tasks, UBaseType_t old_ترجیح)
{
#إذا !CONFIG_FREERTOS_UNICORE
    // Signal to the other core that it can exit the loop
    stop_flag = true;
#جے_انت // !CONFIG_FREERTOS_UNICORE
    // Re-enable interrupts on the current core.
    taskENABLE_INTERRUPTS();

    لكل (صحيح i = 0; i < عدد_tasks; i++) {
        vTaskDelete(task_list[i]);
    }
    // Restore priority
    v_Task_ترجیح_سیٹ(عدم, old_ترجیح);
    // Short delay to allow tasks to clean up
    vTaskDelay(10);
}

TEST_CASE("Task snapshot: Get all", "[freertos]")
{
    // Short delay to allow both cores to spin up
    vTaskDelay(10);

    TaskHandle_t task_list[TEST_MAX_TASKS_عدد];
    صحيح عدد_tasks;
    UBaseType_t old_ترجیح;
    تیاری(task_list, &عدد_tasks, &old_ترجیح);

    // Get task snapshots using uxTaskGetSnapshotAll()
    TaskSnapshot_t task_snapshots[TEST_MAX_TASKS_عدد];
    UBaseType_t tcb_ناپ;
    UBaseType_t عدد_snapshots;
    عدد_snapshots = uxTaskGetSnapshotAll(task_snapshots, TEST_MAX_TASKS_عدد, &tcb_ناپ);
    TEST_ASSERT_LESS_OR_EQUAL(TEST_MAX_TASKS_عدد, عدد_snapshots);

    check_snapshots(task_list, عدد_tasks, task_snapshots, عدد_snapshots);
    teardown(task_list, عدد_tasks, old_ترجیح);
}

TEST_CASE("Task snapshot: Iterate", "[freertos]")
{
    // Short delay to allow both cores to spin up
    vTaskDelay(10);

    TaskHandle_t task_list[TEST_MAX_TASKS_عدد];
    صحيح عدد_tasks;
    UBaseType_t old_ترجیح;
    تیاری(task_list, &عدد_tasks, &old_ترجیح);

    // Get task snapshots using xTaskGetNext() and vTaskGetSnapshot()
    TaskSnapshot_t task_snapshots[TEST_MAX_TASKS_عدد];
    UBaseType_t عدد_snapshots = 0;
    TaskIterator_t task_iterator = {0};
    طالما (x_Task_Get_اگلا(&task_iterator) != -1) {
        إذا (task_iterator.pxTaskHandle != عدم) {
            // Get the task's snapshot
            ادھار_Type_t نتیجہ_2 = vTaskGetSnapshot(task_iterator.pxTaskHandle, &task_snapshots[عدد_snapshots]);
            TEST_ASSERT_EQUAL(pdTRUE, نتیجہ_2);
            عدد_snapshots++;
        }
    }
    TEST_ASSERT_LESS_OR_EQUAL(TEST_MAX_TASKS_عدد, عدد_snapshots);

    check_snapshots(task_list, عدد_tasks, task_snapshots, عدد_snapshots);
    teardown(task_list, عدد_tasks, old_ترجیح);
}
