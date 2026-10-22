/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ "unity.h"
#ಅನ್ತರ್ಭಾವ "test_utils.h"

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

ಸ್ಥಿರ ಶೂನ್ಯ blocked_task(ಶೂನ್ಯ *arg)
{
    vTaskDelay(ಪೋರ್ಟ್_MAX_DELAY - 1);
    // Shouldn't need to self delete, but added for extra safety
    vTaskDelete(ಖಾಲಿ);
}

ಸ್ಥಿರ ಶೂನ್ಯ suspended_task(ಶೂನ್ಯ *arg)
{
    vTaskSuspend(ಖಾಲಿ);
    // Shouldn't need to self delete, but added for extra safety
    vTaskDelete(ಖಾಲಿ);
}

ಸ್ಥಿರ ಶೂನ್ಯ loop_task(ಶೂನ್ಯ *arg)
{
    // Short delay to allow other created tasks to run
    vTaskDelay(2);
    ಆಗ (1) {
        ;
    }
}

TEST_CASE("Test eTaskGetState", "[freertos]")
{
    TaskHandle_t blocked_tasks[CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES];
    TaskHandle_t suspended_tasks[CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES];
    TaskHandle_t ready_tasks[CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES];
    TaskHandle_t running_tasks[CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES];

    // Create tasks of each state on each core
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; i++) {
        TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_ಗಮ್ಯ_Core(blocked_task, "blkd", config_ಕನಿಷ್ಠತಮ_ರಾಶಿ_ಗಾತ್ರ * 2, ಖಾಲಿ, UNITY_FREERTOS_ಆದ್ಯತೆ - 1, &blocked_tasks[i], i));
        TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_ಗಮ್ಯ_Core(suspended_task, "susp", config_ಕನಿಷ್ಠತಮ_ರಾಶಿ_ಗಾತ್ರ * 2, ಖಾಲಿ, UNITY_FREERTOS_ಆದ್ಯತೆ - 1, &suspended_tasks[i], i));
        TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_ಗಮ್ಯ_Core(loop_task, "rdy", config_ಕನಿಷ್ಠತಮ_ರಾಶಿ_ಗಾತ್ರ * 2, ಖಾಲಿ, UNITY_FREERTOS_ಆದ್ಯತೆ - 1, &ready_tasks[i], i));
        ಆದರೆ (i == UNITY_FREERTOS_CPU) {
            running_tasks[i] = xTaskGetCurrentTaskHandle();
        } ಇಲ್ಲದಿದ್ದರೆ {
            x_Task_Create_Pinned_ಗಮ್ಯ_Core(loop_task, "run", config_ಕನಿಷ್ಠತಮ_ರಾಶಿ_ಗಾತ್ರ * 2, ಖಾಲಿ, UNITY_FREERTOS_ಆದ್ಯತೆ, &running_tasks[i], i);
        }
    }

    // Short delay to allow created tasks to run
    vTaskDelay(10);

    // Check the state of the created tasks
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; i++) {
        TEST_ASSERT_EQUAL(eBlocked, eTaskGetState(blocked_tasks[i]));
        TEST_ASSERT_EQUAL(eSuspended, eTaskGetState(suspended_tasks[i]));
        TEST_ASSERT_EQUAL(eReady, eTaskGetState(ready_tasks[i]));
        TEST_ASSERT_EQUAL(eRunning, eTaskGetState(running_tasks[i]));
    }

    // Clean up created tasks
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; i++) {
        vTaskDelete(blocked_tasks[i]);
        vTaskDelete(suspended_tasks[i]);
        vTaskDelete(ready_tasks[i]);
        ಆದರೆ (i != UNITY_FREERTOS_CPU) {
            vTaskDelete(running_tasks[i]);
        }
    }

    // Short delay to allow task memory to be cleaned
    vTaskDelay(10);
}
