/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#അന്തര്ഭാവ "sdkconfig.h"
#അന്തര്ഭാവ "freertos/FreeRTOS.h"
#അന്തര്ഭാവ "freertos/task.h"
#അന്തര്ഭാവ "unity.h"
#അന്തര്ഭാവ "test_utils.h"

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

സ്ഥിതം ശൂന്യം blocked_task(ശൂന്യം *arg)
{
    vTaskDelay(portMAX_DELAY - 1);
    // Shouldn't need to self delete, but added for extra safety
    vTaskDelete(ശൂന്യ);
}

സ്ഥിതം ശൂന്യം suspended_task(ശൂന്യം *arg)
{
    vTaskSuspend(ശൂന്യ);
    // Shouldn't need to self delete, but added for extra safety
    vTaskDelete(ശൂന്യ);
}

സ്ഥിതം ശൂന്യം loop_task(ശൂന്യം *arg)
{
    // Short delay to allow other created tasks to run
    vTaskDelay(2);
    ആകുമ്പോൾ (1) {
        ;
    }
}

TEST_CASE("Test eTaskGetState", "[freertos]")
{
    TaskHandle_t blocked_tasks[CONFIG_FREERTOS_സംഖ്യ_OF_CORES];
    TaskHandle_t suspended_tasks[CONFIG_FREERTOS_സംഖ്യ_OF_CORES];
    TaskHandle_t ready_tasks[CONFIG_FREERTOS_സംഖ്യ_OF_CORES];
    TaskHandle_t running_tasks[CONFIG_FREERTOS_സംഖ്യ_OF_CORES];

    // Create tasks of each state on each core
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; i++) {
        TEST_ASSERT_EQUAL(pdPASS, xTaskCreatePinnedToCore(blocked_task, "blkd", config_MINIMAL_അട്ടി_SIZE * 2, ശൂന്യ, UNITY_FREERTOS_PRIORITY - 1, &blocked_tasks[i], i));
        TEST_ASSERT_EQUAL(pdPASS, xTaskCreatePinnedToCore(suspended_task, "susp", config_MINIMAL_അട്ടി_SIZE * 2, ശൂന്യ, UNITY_FREERTOS_PRIORITY - 1, &suspended_tasks[i], i));
        TEST_ASSERT_EQUAL(pdPASS, xTaskCreatePinnedToCore(loop_task, "rdy", config_MINIMAL_അട്ടി_SIZE * 2, ശൂന്യ, UNITY_FREERTOS_PRIORITY - 1, &ready_tasks[i], i));
        എങ്കിൽ (i == UNITY_FREERTOS_CPU) {
            running_tasks[i] = xTaskGetCurrentTaskHandle();
        } അല്ലെങ്കിൽ {
            xTaskCreatePinnedToCore(loop_task, "run", config_MINIMAL_അട്ടി_SIZE * 2, ശൂന്യ, UNITY_FREERTOS_PRIORITY, &running_tasks[i], i);
        }
    }

    // Short delay to allow created tasks to run
    vTaskDelay(10);

    // Check the state of the created tasks
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; i++) {
        TEST_ASSERT_EQUAL(eBlocked, eTaskGetState(blocked_tasks[i]));
        TEST_ASSERT_EQUAL(eSuspended, eTaskGetState(suspended_tasks[i]));
        TEST_ASSERT_EQUAL(eReady, eTaskGetState(ready_tasks[i]));
        TEST_ASSERT_EQUAL(eRunning, eTaskGetState(running_tasks[i]));
    }

    // Clean up created tasks
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; i++) {
        vTaskDelete(blocked_tasks[i]);
        vTaskDelete(suspended_tasks[i]);
        vTaskDelete(ready_tasks[i]);
        എങ്കിൽ (i != UNITY_FREERTOS_CPU) {
            vTaskDelete(running_tasks[i]);
        }
    }

    // Short delay to allow task memory to be cleaned
    vTaskDelay(10);
}
