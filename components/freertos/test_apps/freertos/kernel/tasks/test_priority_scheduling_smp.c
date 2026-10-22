/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ "FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "task.h"
#ಅನ್ತರ್ಭಾವ "semphr.h"
#ಅನ್ತರ್ಭಾವ "unity.h"
#ಅನ್ತರ್ಭಾವ "portTestMacro.h"

/* ------------------------------------------------------------------------------------------------------------------ */

/*
Test Priority Scheduling SMP

Purpose:
    - Test that the SMP scheduler always schedules the highest priority ready tasks for each core
Procedure:
    - Raise the unityTask priority to (configMAX_PRIORITIES - 1)
    - unityTask creates the following lower priority tasks for each core
        - task_A (configMAX_PRIORITIES - 2) for each core
        - task_B (configMAX_PRIORITIES - 3) for each core
    - unityTask blocks for a short period of time to allow all of the task_As to run
    - Clean up and restore unityTask's original priority
Expected:
    - All of the task_As should be run by the scheduler
    - None of the task_Bs should have run
*/

#ಆದರೆ    ( defined( CONFIG_FREERTOS_SMP ) && ( CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES > 1 ) && ( config_ಸತತ_ಅಪವರ್ತ್ಯ_PRIORITIES == 1 ) ) \
    || ( !defined( CONFIG_FREERTOS_SMP ) && ( CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES > 1 ) )

#ವ್ಯಾಖ್ಯಾನಿಸು UNITY_TASK_DELAY_TICKS      10

ಸ್ಥಿರ ತಳ_Type_t task_A_ran[CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES];
ಸ್ಥಿರ ತಳ_Type_t task_B_ran[CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES];

ಸ್ಥಿರ ಶೂನ್ಯ task_A(ಶೂನ್ಯ *arg)
{
    ತಳ_Type_t task_idx = (ತಳ_Type_t) arg;
    task_A_ran[task_idx] = pdTRUE;
    /* Keeping spinning to prevent the lower priority task_B from running */
    ಆಗ (1) {
        ;
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ task_B(ಶೂನ್ಯ *arg)
{
    /* The following should never be run due to task_B having a lower priority */
    ತಳ_Type_t task_idx = (ತಳ_Type_t) arg;
    task_B_ran[task_idx] = pdTRUE;
    ಆಗ (1) {
        ;
    }
}

TEST_CASE("Tasks: Test priority scheduling (SMP)", "[freertos]")
{
    TaskHandle_t task_A_handles[CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES];
    TaskHandle_t task_B_handles[CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES];
    memset(task_A_ran, pdFALSE, ಗಾತ್ರ(task_A_ran));
    memset(task_B_ran, pdFALSE, ಗಾತ್ರ(task_B_ran));

    /* Raise the priority of the unityTask */
    v_Task_ಆದ್ಯತೆ_ಗಣ(ಖಾಲಿ, configMAX_PRIORITIES - 1);

    /* Create task_A for each core */
    ಗಾಗಿ (UBaseType_t x = 0; x < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; x++) {
        xTaskCreate(task_A, "task_A", config_TEST_DEFAULT_ರಾಶಿ_ಗಾತ್ರ, (ಶೂನ್ಯ *)x, configMAX_PRIORITIES - 2, &task_A_handles[x]);
    }

    /* Create task_B for each core */
    ಗಾಗಿ (UBaseType_t x = 0; x < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; x++) {
        xTaskCreate(task_B, "task_B", config_TEST_DEFAULT_ರಾಶಿ_ಗಾತ್ರ, (ಶೂನ್ಯ *)x, configMAX_PRIORITIES - 3, &task_B_handles[x]);
    }

    /* Block to ensure all the task_As to be scheduled */
    vTaskDelay(UNITY_TASK_DELAY_TICKS);

    /* Check that all the task_As have run, and none of the task_Bs have run */
    ಗಾಗಿ (UBaseType_t x = 0; x < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; x++) {
        TEST_ASSERT_EQUAL(pdTRUE, task_A_ran[x]);
        TEST_ASSERT_EQUAL(pdFALSE, task_B_ran[x]);
    }

    /* Cleanup */
    ಗಾಗಿ (UBaseType_t x = 0; x < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; x++) {
        vTaskDelete(task_A_handles[x]);
        vTaskDelete(task_B_handles[x]);
    }

    /* Restore the priority of the unityTask */
    v_Task_ಆದ್ಯತೆ_ಗಣ(ಖಾಲಿ, config_TEST_UNITY_TASK_ಆದ್ಯತೆ);
}

#ಆದರೆ_ಅಂತ್ಯ /*    ( defined( CONFIG_FREERTOS_SMP ) && ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) && ( configRUN_MULTIPLE_PRIORITIES == 1 ) )
          || ( !defined( CONFIG_FREERTOS_SMP ) && ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) ) */
