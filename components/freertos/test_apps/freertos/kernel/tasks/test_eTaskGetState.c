/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unity.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "test_utils.h"

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

ਸਥਿਰ ਖਾਲੀ blocked_task(ਖਾਲੀ *arg)
{
    vTaskDelay(ਪੋਰਟ_MAX_DELAY - 1);
    // Shouldn't need to self delete, but added for extra safety
    vTaskDelete(ਨਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ suspended_task(ਖਾਲੀ *arg)
{
    vTaskSuspend(ਨਿਰਾ);
    // Shouldn't need to self delete, but added for extra safety
    vTaskDelete(ਨਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ loop_task(ਖਾਲੀ *arg)
{
    // Short delay to allow other created tasks to run
    vTaskDelay(2);
    ਜਦੋਂ (1) {
        ;
    }
}

TEST_CASE("Test eTaskGetState", "[freertos]")
{
    TaskHandle_t blocked_tasks[CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES];
    TaskHandle_t suspended_tasks[CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES];
    TaskHandle_t ready_tasks[CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES];
    TaskHandle_t running_tasks[CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES];

    // Create tasks of each state on each core
    ਲਈ (ਪੂਰਨ i = 0; i < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; i++) {
        TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(blocked_task, "blkd", config_ਨਿਊਨਤਮ_ਢੇਰ_ਮਾਪ * 2, ਨਿਰਾ, UNITY_FREERTOS_ਤਰਜੀਹ - 1, &blocked_tasks[i], i));
        TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(suspended_task, "susp", config_ਨਿਊਨਤਮ_ਢੇਰ_ਮਾਪ * 2, ਨਿਰਾ, UNITY_FREERTOS_ਤਰਜੀਹ - 1, &suspended_tasks[i], i));
        TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(loop_task, "rdy", config_ਨਿਊਨਤਮ_ਢੇਰ_ਮਾਪ * 2, ਨਿਰਾ, UNITY_FREERTOS_ਤਰਜੀਹ - 1, &ready_tasks[i], i));
        ਜੇ (i == UNITY_FREERTOS_CPU) {
            running_tasks[i] = xTaskGetCurrentTaskHandle();
        } ਨਹੀਂਤਾਂ {
            x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(loop_task, "run", config_ਨਿਊਨਤਮ_ਢੇਰ_ਮਾਪ * 2, ਨਿਰਾ, UNITY_FREERTOS_ਤਰਜੀਹ, &running_tasks[i], i);
        }
    }

    // Short delay to allow created tasks to run
    vTaskDelay(10);

    // Check the state of the created tasks
    ਲਈ (ਪੂਰਨ i = 0; i < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; i++) {
        TEST_ASSERT_EQUAL(eBlocked, eTaskGetState(blocked_tasks[i]));
        TEST_ASSERT_EQUAL(eSuspended, eTaskGetState(suspended_tasks[i]));
        TEST_ASSERT_EQUAL(eReady, eTaskGetState(ready_tasks[i]));
        TEST_ASSERT_EQUAL(eRunning, eTaskGetState(running_tasks[i]));
    }

    // Clean up created tasks
    ਲਈ (ਪੂਰਨ i = 0; i < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; i++) {
        vTaskDelete(blocked_tasks[i]);
        vTaskDelete(suspended_tasks[i]);
        vTaskDelete(ready_tasks[i]);
        ਜੇ (i != UNITY_FREERTOS_CPU) {
            vTaskDelete(running_tasks[i]);
        }
    }

    // Short delay to allow task memory to be cleaned
    vTaskDelay(10);
}
