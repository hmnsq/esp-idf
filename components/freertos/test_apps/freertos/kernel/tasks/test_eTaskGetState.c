/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#அந்தர்பாவ "sdkconfig.h"
#அந்தர்பாவ "freertos/FreeRTOS.h"
#அந்தர்பாவ "freertos/task.h"
#அந்தர்பாவ "unity.h"
#அந்தர்பாவ "test_utils.h"

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

நிலையான வெறுமை blocked_task(வெறுமை *arg)
{
    vTaskDelay(portMAX_DELAY - 1);
    // Shouldn't need to self delete, but added for extra safety
    vTaskDelete(வெற்று);
}

நிலையான வெறுமை suspended_task(வெறுமை *arg)
{
    vTaskSuspend(வெற்று);
    // Shouldn't need to self delete, but added for extra safety
    vTaskDelete(வெற்று);
}

நிலையான வெறுமை loop_task(வெறுமை *arg)
{
    // Short delay to allow other created tasks to run
    vTaskDelay(2);
    போது (1) {
        ;
    }
}

TEST_CASE("Test eTaskGetState", "[freertos]")
{
    TaskHandle_t blocked_tasks[CONFIG_FREERTOS_எண்_OF_CORES];
    TaskHandle_t suspended_tasks[CONFIG_FREERTOS_எண்_OF_CORES];
    TaskHandle_t ready_tasks[CONFIG_FREERTOS_எண்_OF_CORES];
    TaskHandle_t running_tasks[CONFIG_FREERTOS_எண்_OF_CORES];

    // Create tasks of each state on each core
    க்கு (முழு i = 0; i < CONFIG_FREERTOS_எண்_OF_CORES; i++) {
        TEST_ASSERT_EQUAL(pdPASS, xTaskCreatePinnedToCore(blocked_task, "blkd", config_MINIMAL_அடுக்கு_SIZE * 2, வெற்று, UNITY_FREERTOS_PRIORITY - 1, &blocked_tasks[i], i));
        TEST_ASSERT_EQUAL(pdPASS, xTaskCreatePinnedToCore(suspended_task, "susp", config_MINIMAL_அடுக்கு_SIZE * 2, வெற்று, UNITY_FREERTOS_PRIORITY - 1, &suspended_tasks[i], i));
        TEST_ASSERT_EQUAL(pdPASS, xTaskCreatePinnedToCore(loop_task, "rdy", config_MINIMAL_அடுக்கு_SIZE * 2, வெற்று, UNITY_FREERTOS_PRIORITY - 1, &ready_tasks[i], i));
        எனில் (i == UNITY_FREERTOS_CPU) {
            running_tasks[i] = xTaskGetCurrentTaskHandle();
        } இல்லையெனில் {
            xTaskCreatePinnedToCore(loop_task, "run", config_MINIMAL_அடுக்கு_SIZE * 2, வெற்று, UNITY_FREERTOS_PRIORITY, &running_tasks[i], i);
        }
    }

    // Short delay to allow created tasks to run
    vTaskDelay(10);

    // Check the state of the created tasks
    க்கு (முழு i = 0; i < CONFIG_FREERTOS_எண்_OF_CORES; i++) {
        TEST_ASSERT_EQUAL(eBlocked, eTaskGetState(blocked_tasks[i]));
        TEST_ASSERT_EQUAL(eSuspended, eTaskGetState(suspended_tasks[i]));
        TEST_ASSERT_EQUAL(eReady, eTaskGetState(ready_tasks[i]));
        TEST_ASSERT_EQUAL(eRunning, eTaskGetState(running_tasks[i]));
    }

    // Clean up created tasks
    க்கு (முழு i = 0; i < CONFIG_FREERTOS_எண்_OF_CORES; i++) {
        vTaskDelete(blocked_tasks[i]);
        vTaskDelete(suspended_tasks[i]);
        vTaskDelete(ready_tasks[i]);
        எனில் (i != UNITY_FREERTOS_CPU) {
            vTaskDelete(running_tasks[i]);
        }
    }

    // Short delay to allow task memory to be cleaned
    vTaskDelay(10);
}
