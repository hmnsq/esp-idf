/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#समावेश "sdkconfig.h"
#समावेश "FreeRTOS.h"
#समावेश "task.h"
#समावेश "unity.h"
#समावेश "portTestMacro.h"

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

#अगर ( CONFIG_FREERTOS_संख्या_OF_CORES == 1 )

#घोषणा UNITY_TASK_DELAY_TICKS      10

अटल BaseType_t task_A_ran;
अटल BaseType_t task_B_ran;

अटल व्योम task_A(व्योम *arg)
{
    task_A_ran = pdTRUE;
    /* Keeping spinning to prevent the lower priority task_B from running */
    जबतक (1) {
        ;
    }
}

अटल व्योम task_B(व्योम *arg)
{
    /* The following should never run due to task_B having a lower priority */
    task_B_ran = pdTRUE;
    जबतक (1) {
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
    vTaskPrioritySet(शून्य, configMAX_PRIORITIES - 1);
    /* Create task_A and task_B */
    xTaskCreate(task_A, "task_A", config_TEST_DEFAULT_ढेर_आकार, (व्योम *)xTaskGetCurrentTaskHandle(), configMAX_PRIORITIES - 2, &task_A_handle);
    xTaskCreate(task_B, "task_B", config_TEST_DEFAULT_ढेर_आकार, (व्योम *)xTaskGetCurrentTaskHandle(), configMAX_PRIORITIES - 3, &task_B_handle);

    /* Block to allow task_A to be scheduled */
    vTaskDelay(UNITY_TASK_DELAY_TICKS);

    /* Test that only task_A has run */
    TEST_ASSERT_EQUAL(pdTRUE, task_A_ran);
    TEST_ASSERT_EQUAL(pdFALSE, task_B_ran);

    vTaskDelete(task_A_handle);
    vTaskDelete(task_B_handle);
    /* Restore the priority of the unityTask */
    vTaskPrioritySet(शून्य, configTEST_UNITY_TASK_PRIORITY);
}

#पूर्ण_अगर /* CONFIG_FREERTOS_NUMBER_OF_CORES == 1 */
