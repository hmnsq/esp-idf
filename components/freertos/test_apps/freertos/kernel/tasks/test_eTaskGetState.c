/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ "unity.h"
#అన్తర్భావ "test_utils.h"

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

స్థిర శూన్య blocked_task(శూన్య *arg)
{
    vTaskDelay(పోర్టు_MAX_DELAY - 1);
    // Shouldn't need to self delete, but added for extra safety
    vTaskDelete(ఖాళీ);
}

స్థిర శూన్య suspended_task(శూన్య *arg)
{
    vTaskSuspend(ఖాళీ);
    // Shouldn't need to self delete, but added for extra safety
    vTaskDelete(ఖాళీ);
}

స్థిర శూన్య loop_task(శూన్య *arg)
{
    // Short delay to allow other created tasks to run
    vTaskDelay(2);
    అయితే (1) {
        ;
    }
}

TEST_CASE("Test eTaskGetState", "[freertos]")
{
    TaskHandle_t blocked_tasks[CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES];
    TaskHandle_t suspended_tasks[CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES];
    TaskHandle_t ready_tasks[CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES];
    TaskHandle_t running_tasks[CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES];

    // Create tasks of each state on each core
    కొరకు (పూర్ణాంకం i = 0; i < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; i++) {
        TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_గమ్యం_Core(blocked_task, "blkd", config_కనీస_దొంతర_కొలత * 2, ఖాళీ, UNITY_FREERTOS_ప్రాధాన్యత - 1, &blocked_tasks[i], i));
        TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_గమ్యం_Core(suspended_task, "susp", config_కనీస_దొంతర_కొలత * 2, ఖాళీ, UNITY_FREERTOS_ప్రాధాన్యత - 1, &suspended_tasks[i], i));
        TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_గమ్యం_Core(loop_task, "rdy", config_కనీస_దొంతర_కొలత * 2, ఖాళీ, UNITY_FREERTOS_ప్రాధాన్యత - 1, &ready_tasks[i], i));
        ఒకవేళ (i == UNITY_FREERTOS_CPU) {
            running_tasks[i] = xTaskGetCurrentTaskHandle();
        } లేకపోతే {
            x_Task_Create_Pinned_గమ్యం_Core(loop_task, "run", config_కనీస_దొంతర_కొలత * 2, ఖాళీ, UNITY_FREERTOS_ప్రాధాన్యత, &running_tasks[i], i);
        }
    }

    // Short delay to allow created tasks to run
    vTaskDelay(10);

    // Check the state of the created tasks
    కొరకు (పూర్ణాంకం i = 0; i < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; i++) {
        TEST_ASSERT_EQUAL(eBlocked, eTaskGetState(blocked_tasks[i]));
        TEST_ASSERT_EQUAL(eSuspended, eTaskGetState(suspended_tasks[i]));
        TEST_ASSERT_EQUAL(eReady, eTaskGetState(ready_tasks[i]));
        TEST_ASSERT_EQUAL(eRunning, eTaskGetState(running_tasks[i]));
    }

    // Clean up created tasks
    కొరకు (పూర్ణాంకం i = 0; i < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; i++) {
        vTaskDelete(blocked_tasks[i]);
        vTaskDelete(suspended_tasks[i]);
        vTaskDelete(ready_tasks[i]);
        ఒకవేళ (i != UNITY_FREERTOS_CPU) {
            vTaskDelete(running_tasks[i]);
        }
    }

    // Short delay to allow task memory to be cleaned
    vTaskDelay(10);
}
