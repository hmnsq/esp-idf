/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ "FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "task.h"
#ಅನ್ತರ್ಭಾವ "unity.h"
#ಅನ್ತರ್ಭಾವ "portTestMacro.h"

/* ------------------------------------------------------------------------------------------------------------------ */

/*
Test Priority Scheduling (Single Core)

Purpose:
    - Test that the single-core scheduler always schedules the highest priority ready task
Procedure:
    - Raise the unityTask priority to (configMAX_PRIORITIES - 1)
    - unityTask creates the following lower priority tasks
        - task_A (configMAX_PRIORITIES - 2)
        - task_B (configMAX_PRIORITIES - 3)
    - UnityTask blocks for a short period of time to allow task_A to run
    - Clean up and restore unityTask's original priority
Expected:
    - task_A should run after unityTask blocks
    - task_B should never have run
*/

#ಯದಿ ( CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES == 1 )

#define UNITY_TASK_DELAY_TICKS      10

ಸ್ಥಿರ BaseType_t task_A_ran;
ಸ್ಥಿರ BaseType_t task_B_ran;

ಸ್ಥಿರ ಶೂನ್ಯ task_A(ಶೂನ್ಯ *arg)
{
    task_A_ran = pdTRUE;
    /* Keeping spinning to prevent the lower priority task_B from running */
    ಯಾವತ್ (1) {
        ;
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ task_B(ಶೂನ್ಯ *arg)
{
    /* The following should never run due to task_B having a lower priority */
    task_B_ran = pdTRUE;
    ಯಾವತ್ (1) {
        ;
    }
}

TEST_CASE("Tasks: Test priority scheduling", "[freertos]")
{
    TaskHandle_t task_A_handle;
    TaskHandle_t task_B_handle;
    task_A_ran = pdFALSE;
    task_B_ran = pdFALSE;

    /* Raise the priority of the unityTask */
    vTaskPrioritySet(ರಿಕ್ತ, configMAX_PRIORITIES - 1);
    /* Create task_A and task_B */
    xTaskCreate(task_A, "task_A", config_TEST_DEFAULT_ರಾಶಿ_SIZE, (ಶೂನ್ಯ *)xTaskGetCurrentTaskHandle(), configMAX_PRIORITIES - 2, &task_A_handle);
    xTaskCreate(task_B, "task_B", config_TEST_DEFAULT_ರಾಶಿ_SIZE, (ಶೂನ್ಯ *)xTaskGetCurrentTaskHandle(), configMAX_PRIORITIES - 3, &task_B_handle);

    /* Block to allow task_A to be scheduled */
    vTaskDelay(UNITY_TASK_DELAY_TICKS);

    /* Test that only task_A has run */
    TEST_ASSERT_EQUAL(pdTRUE, task_A_ran);
    TEST_ASSERT_EQUAL(pdFALSE, task_B_ran);

    vTaskDelete(task_A_handle);
    vTaskDelete(task_B_handle);
    /* Restore the priority of the unityTask */
    vTaskPrioritySet(ರಿಕ್ತ, configTEST_UNITY_TASK_PRIORITY);
}

#endif /* CONFIG_FREERTOS_NUMBER_OF_CORES == 1 */
