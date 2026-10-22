/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#شامل "sdkconfig.h"
#شامل "freertos/FreeRTOS.h"
#شامل "freertos/task.h"
#شامل "unity.h"
#شامل "test_utils.h"

/*
Test eTaskGetState()

Purpose:
    - Test that eTaskGetState() returns the correct state for a particular task

Procedure:
    - Create tasks in every state (and repeat for each core)
    - Note: eDeleted is not tested due to needing to control when the idle tasks run
    - Call eTaskGetState() on each created task

Expected:
    - eTaskGetState() should return the correct state for each created task
*/

ساکن خالی blocked_task(خالی *arg)
{
    vTaskDelay(پورٹ_MAX_DELAY - 1);
    // Shouldn't need to self delete, but added for extra safety
    vTaskDelete(تہی);
}

ساکن خالی suspended_task(خالی *arg)
{
    vTaskSuspend(تہی);
    // Shouldn't need to self delete, but added for extra safety
    vTaskDelete(تہی);
}

ساکن خالی loop_task(خالی *arg)
{
    // Short delay to allow other created tasks to run
    vTaskDelay(2);
    جب (1) {
        ;
    }
}

TEST_CASE("Test eTaskGetState", "[freertos]")
{
    TaskHandle_t blocked_tasks[CONFIG_FREERTOS_نمبر_کا_CORES];
    TaskHandle_t suspended_tasks[CONFIG_FREERTOS_نمبر_کا_CORES];
    TaskHandle_t ready_tasks[CONFIG_FREERTOS_نمبر_کا_CORES];
    TaskHandle_t running_tasks[CONFIG_FREERTOS_نمبر_کا_CORES];

    // Create tasks of each state on each core
    برائے (صحیح i = 0; i < CONFIG_FREERTOS_نمبر_کا_CORES; i++) {
        TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_مقصد_Core(blocked_task, "blkd", config_اقل_ڈھیر_سائز * 2, تہی, UNITY_FREERTOS_ترجیح - 1, &blocked_tasks[i], i));
        TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_مقصد_Core(suspended_task, "susp", config_اقل_ڈھیر_سائز * 2, تہی, UNITY_FREERTOS_ترجیح - 1, &suspended_tasks[i], i));
        TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_مقصد_Core(loop_task, "rdy", config_اقل_ڈھیر_سائز * 2, تہی, UNITY_FREERTOS_ترجیح - 1, &ready_tasks[i], i));
        اگر (i == UNITY_FREERTOS_CPU) {
            running_tasks[i] = xTaskGetCurrentTaskHandle();
        } ورنہ {
            x_Task_Create_Pinned_مقصد_Core(loop_task, "run", config_اقل_ڈھیر_سائز * 2, تہی, UNITY_FREERTOS_ترجیح, &running_tasks[i], i);
        }
    }

    // Short delay to allow created tasks to run
    vTaskDelay(10);

    // Check the state of the created tasks
    برائے (صحیح i = 0; i < CONFIG_FREERTOS_نمبر_کا_CORES; i++) {
        TEST_ASSERT_EQUAL(eBlocked, eTaskGetState(blocked_tasks[i]));
        TEST_ASSERT_EQUAL(eSuspended, eTaskGetState(suspended_tasks[i]));
        TEST_ASSERT_EQUAL(eReady, eTaskGetState(ready_tasks[i]));
        TEST_ASSERT_EQUAL(eRunning, eTaskGetState(running_tasks[i]));
    }

    // Clean up created tasks
    برائے (صحیح i = 0; i < CONFIG_FREERTOS_نمبر_کا_CORES; i++) {
        vTaskDelete(blocked_tasks[i]);
        vTaskDelete(suspended_tasks[i]);
        vTaskDelete(ready_tasks[i]);
        اگر (i != UNITY_FREERTOS_CPU) {
            vTaskDelete(running_tasks[i]);
        }
    }

    // Short delay to allow task memory to be cleaned
    vTaskDelay(10);
}
