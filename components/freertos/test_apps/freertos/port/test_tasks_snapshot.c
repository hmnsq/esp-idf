/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdbool.h>
#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ "freertos/freertos_debug.h"
#అన్తర్భావ "esp_cpu.h"
#అన్తర్భావ "esp_rom_sys.h"
#అన్తర్భావ "unity.h"
#అన్తర్భావ "sdkconfig.h"

#నిర్వచించు TEST_MAX_TASKS_అంకె      32
#నిర్వచించు అంకె_TASKS_చొప్పున_LIST      2
#నిర్వచించు TASK_ప్రాధాన్యత           (configMAX_PRIORITIES - 2)

స్థిర అస్థిర bool ప్రారంభం_ready_flag = false;
స్థిర అస్థిర bool ప్రారంభం_resp_flag = false;
స్థిర అస్థిర bool stop_flag = false;

స్థిర శూన్య ready_task(శూన్య *arg)
{
    అయితే (1) {
#ఒకవేళ !CONFIG_FREERTOS_UNICORE
        taskDISABLE_INTERRUPTS();
        /*
        The main task (on core 0) has a higher priority than this task. Thus, by the time this flag is set, this task
        is guaranteed to running on core 1.
        */
        ఒకవేళ (ప్రారంభం_ready_flag) {   // Is the test starting?
            ప్రారంభం_resp_flag = true;   // Indicate we are ready to start
            అయితే (!stop_flag) {
                ;   // Wait until test completes
            }

            // Reset the flags
            ప్రారంభం_ready_flag = false;
            ప్రారంభం_resp_flag = false;
            stop_flag = false;
        }
        taskENABLE_INTERRUPTS();
        // Short non-blocking delay to allow INT WDT to feed
        esp_rom_delay_us(1000);
#అయితే_ముగింపు // !CONFIG_FREERTOS_UNICORE
    }
}

స్థిర శూన్య blocked_task(శూన్య *arg)
{
    // Delay for portMAX_DELAY - 1 as not to go on the suspended list
    vTaskDelay(పోర్టు_MAX_DELAY - 1);
}

స్థిర శూన్య suspended_task(శూన్య *arg)
{
    vTaskSuspend(ఖాళీ);
}

స్థిర శూన్య అమరిక(TaskHandle_t *task_list, పూర్ణాంకం *అంకె_tasks_ret, UBaseType_t *old_ప్రాధాన్యత_ret)
{
    // Raise our priority so that we aren't preempted
    *old_ప్రాధాన్యత_ret = ux_Task_ప్రాధాన్యత_Get(ఖాళీ);
    v_Task_ప్రాధాన్యత_సమితి(ఖాళీ, configMAX_PRIORITIES - 1);

    // Create tasks
    పూర్ణాంకం అంకె_tasks = 0;
    కొరకు (పూర్ణాంకం i = 0; i < అంకె_TASKS_చొప్పున_LIST; i++) {
        //Ready task
        xTaskCreate(ready_task, "ready", 1024, ఖాళీ, TASK_ప్రాధాన్యత, &(task_list[అంకె_tasks]));
        అంకె_tasks++;
        //Blocked task
        xTaskCreate(blocked_task, "blkd", 1024, ఖాళీ, TASK_ప్రాధాన్యత, &(task_list[అంకె_tasks]));
        అంకె_tasks++;
        //Suspended task
        xTaskCreate(suspended_task, "susp", 1024, ఖాళీ, TASK_ప్రాధాన్యత, &(task_list[అంకె_tasks]));
        అంకె_tasks++;
    }
    *అంకె_tasks_ret = అంకె_tasks;
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
#ఒకవేళ !CONFIG_FREERTOS_UNICORE
    // For SMP, we need to do a 2-way handshake to ensure the other core spins with interrupts disabled.
    ప్రారంభం_ready_flag = true;
    అయితే (!ప్రారంభం_resp_flag) {
        ;
    }
#అయితే_ముగింపు // !CONFIG_FREERTOS_UNICORE
}

స్థిర శూన్య check_snapshots(TaskHandle_t *task_list, పూర్ణాంకం అంకె_tasks, TaskSnapshot_t *task_snapshots, UBaseType_t అంకె_snapshots)
{
    // Check task snapshots. Every created task should be found in the task snapshot
    కొరకు (పూర్ణాంకం i = 0; i < అంకె_tasks; i++) {
        bool దొరికింది = false;
        కొరకు (పూర్ణాంకం j = 0; j < అంకె_snapshots; j++) {
            ఒకవేళ (task_list[i] == (TaskHandle_t)task_snapshots[j].pxTCB) {
                దొరికింది = true;
                ఆపు;
            }
        }
        TEST_ASSERT(దొరికింది);
    }
}

స్థిర శూన్య teardown(TaskHandle_t *task_list, పూర్ణాంకం అంకె_tasks, UBaseType_t old_ప్రాధాన్యత)
{
#ఒకవేళ !CONFIG_FREERTOS_UNICORE
    // Signal to the other core that it can exit the loop
    stop_flag = true;
#అయితే_ముగింపు // !CONFIG_FREERTOS_UNICORE
    // Re-enable interrupts on the current core.
    taskENABLE_INTERRUPTS();

    కొరకు (పూర్ణాంకం i = 0; i < అంకె_tasks; i++) {
        vTaskDelete(task_list[i]);
    }
    // Restore priority
    v_Task_ప్రాధాన్యత_సమితి(ఖాళీ, old_ప్రాధాన్యత);
    // Short delay to allow tasks to clean up
    vTaskDelay(10);
}

TEST_CASE("Task snapshot: Get all", "[freertos]")
{
    // Short delay to allow both cores to spin up
    vTaskDelay(10);

    TaskHandle_t task_list[TEST_MAX_TASKS_అంకె];
    పూర్ణాంకం అంకె_tasks;
    UBaseType_t old_ప్రాధాన్యత;
    అమరిక(task_list, &అంకె_tasks, &old_ప్రాధాన్యత);

    // Get task snapshots using uxTaskGetSnapshotAll()
    TaskSnapshot_t task_snapshots[TEST_MAX_TASKS_అంకె];
    UBaseType_t tcb_కొలత;
    UBaseType_t అంకె_snapshots;
    అంకె_snapshots = uxTaskGetSnapshotAll(task_snapshots, TEST_MAX_TASKS_అంకె, &tcb_కొలత);
    TEST_ASSERT_LESS_OR_EQUAL(TEST_MAX_TASKS_అంకె, అంకె_snapshots);

    check_snapshots(task_list, అంకె_tasks, task_snapshots, అంకె_snapshots);
    teardown(task_list, అంకె_tasks, old_ప్రాధాన్యత);
}

TEST_CASE("Task snapshot: Iterate", "[freertos]")
{
    // Short delay to allow both cores to spin up
    vTaskDelay(10);

    TaskHandle_t task_list[TEST_MAX_TASKS_అంకె];
    పూర్ణాంకం అంకె_tasks;
    UBaseType_t old_ప్రాధాన్యత;
    అమరిక(task_list, &అంకె_tasks, &old_ప్రాధాన్యత);

    // Get task snapshots using xTaskGetNext() and vTaskGetSnapshot()
    TaskSnapshot_t task_snapshots[TEST_MAX_TASKS_అంకె];
    UBaseType_t అంకె_snapshots = 0;
    TaskIterator_t task_iterator = {0};
    అయితే (x_Task_Get_తదుపరి(&task_iterator) != -1) {
        ఒకవేళ (task_iterator.pxTaskHandle != ఖాళీ) {
            // Get the task's snapshot
            పునాది_Type_t ఫలితం_2 = vTaskGetSnapshot(task_iterator.pxTaskHandle, &task_snapshots[అంకె_snapshots]);
            TEST_ASSERT_EQUAL(pdTRUE, ఫలితం_2);
            అంకె_snapshots++;
        }
    }
    TEST_ASSERT_LESS_OR_EQUAL(TEST_MAX_TASKS_అంకె, అంకె_snapshots);

    check_snapshots(task_list, అంకె_tasks, task_snapshots, అంకె_snapshots);
    teardown(task_list, అంకె_tasks, old_ప్రాధాన్యత);
}
