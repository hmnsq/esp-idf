/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unity.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "portTestMacro.h"

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

#ਜੇ ( CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES == 1 )

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UNITY_TASK_DELAY_TICKS      10

ਸਥਿਰ ਅਧਾਰ_Type_t task_A_ran;
ਸਥਿਰ ਅਧਾਰ_Type_t task_B_ran;

ਸਥਿਰ ਖਾਲੀ task_A(ਖਾਲੀ *arg)
{
    task_A_ran = pdTRUE;
    /* Keeping spinning to prevent the lower priority task_B from running */
    ਜਦੋਂ (1) {
        ;
    }
}

ਸਥਿਰ ਖਾਲੀ task_B(ਖਾਲੀ *arg)
{
    /* The following should never run due to task_B having a lower priority */
    task_B_ran = pdTRUE;
    ਜਦੋਂ (1) {
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
    v_Task_ਤਰਜੀਹ_ਸੈੱਟ(ਨਿਰਾ, configMAX_PRIORITIES - 1);
    /* Create task_A and task_B */
    xTaskCreate(task_A, "task_A", config_TEST_DEFAULT_ਢੇਰ_ਮਾਪ, (ਖਾਲੀ *)xTaskGetCurrentTaskHandle(), configMAX_PRIORITIES - 2, &task_A_handle);
    xTaskCreate(task_B, "task_B", config_TEST_DEFAULT_ਢੇਰ_ਮਾਪ, (ਖਾਲੀ *)xTaskGetCurrentTaskHandle(), configMAX_PRIORITIES - 3, &task_B_handle);

    /* Block to allow task_A to be scheduled */
    vTaskDelay(UNITY_TASK_DELAY_TICKS);

    /* Test that only task_A has run */
    TEST_ASSERT_EQUAL(pdTRUE, task_A_ran);
    TEST_ASSERT_EQUAL(pdFALSE, task_B_ran);

    vTaskDelete(task_A_handle);
    vTaskDelete(task_B_handle);
    /* Restore the priority of the unityTask */
    v_Task_ਤਰਜੀਹ_ਸੈੱਟ(ਨਿਰਾ, config_TEST_UNITY_TASK_ਤਰਜੀਹ);
}

#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_NUMBER_OF_CORES == 1 */
