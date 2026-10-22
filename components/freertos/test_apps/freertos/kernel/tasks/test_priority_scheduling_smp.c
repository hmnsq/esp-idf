/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن "sdkconfig.h"
#ضمن <string.h>
#ضمن "FreeRTOS.h"
#ضمن "task.h"
#ضمن "semphr.h"
#ضمن "unity.h"
#ضمن "portTestMacro.h"

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

#إذا    ( defined( CONFIG_FREERTOS_SMP ) && ( CONFIG_FREERTOS_عدد_از_CORES > 1 ) && ( config_پیاپی_مضرب_PRIORITIES == 1 ) ) \
    || ( !defined( CONFIG_FREERTOS_SMP ) && ( CONFIG_FREERTOS_عدد_از_CORES > 1 ) )

#تعریف_کن UNITY_TASK_DELAY_TICKS      10

ساكن پایه_Type_t task_A_ran[CONFIG_FREERTOS_عدد_از_CORES];
ساكن پایه_Type_t task_B_ran[CONFIG_FREERTOS_عدد_از_CORES];

ساكن فراغ task_A(فراغ *arg)
{
    پایه_Type_t task_idx = (پایه_Type_t) arg;
    task_A_ran[task_idx] = pdTRUE;
    /* Keeping spinning to prevent the lower priority task_B from running */
    طالما (1) {
        ;
    }
}

ساكن فراغ task_B(فراغ *arg)
{
    /* The following should never be run due to task_B having a lower priority */
    پایه_Type_t task_idx = (پایه_Type_t) arg;
    task_B_ran[task_idx] = pdTRUE;
    طالما (1) {
        ;
    }
}

TEST_CASE("Tasks: Test priority scheduling (SMP)", "[freertos]")
{
    TaskHandle_t task_A_handles[CONFIG_FREERTOS_عدد_از_CORES];
    TaskHandle_t task_B_handles[CONFIG_FREERTOS_عدد_از_CORES];
    memset(task_A_ran, pdFALSE, حجم(task_A_ran));
    memset(task_B_ran, pdFALSE, حجم(task_B_ran));

    /* Raise the priority of the unityTask */
    v_Task_اولویت_مجموعه(عدم, configMAX_PRIORITIES - 1);

    /* Create task_A for each core */
    لكل (UBaseType_t x = 0; x < CONFIG_FREERTOS_عدد_از_CORES; x++) {
        xTaskCreate(task_A, "task_A", config_TEST_DEFAULT_پشته_اندازه, (فراغ *)x, configMAX_PRIORITIES - 2, &task_A_handles[x]);
    }

    /* Create task_B for each core */
    لكل (UBaseType_t x = 0; x < CONFIG_FREERTOS_عدد_از_CORES; x++) {
        xTaskCreate(task_B, "task_B", config_TEST_DEFAULT_پشته_اندازه, (فراغ *)x, configMAX_PRIORITIES - 3, &task_B_handles[x]);
    }

    /* Block to ensure all the task_As to be scheduled */
    vTaskDelay(UNITY_TASK_DELAY_TICKS);

    /* Check that all the task_As have run, and none of the task_Bs have run */
    لكل (UBaseType_t x = 0; x < CONFIG_FREERTOS_عدد_از_CORES; x++) {
        TEST_ASSERT_EQUAL(pdTRUE, task_A_ran[x]);
        TEST_ASSERT_EQUAL(pdFALSE, task_B_ran[x]);
    }

    /* Cleanup */
    لكل (UBaseType_t x = 0; x < CONFIG_FREERTOS_عدد_از_CORES; x++) {
        vTaskDelete(task_A_handles[x]);
        vTaskDelete(task_B_handles[x]);
    }

    /* Restore the priority of the unityTask */
    v_Task_اولویت_مجموعه(عدم, config_TEST_UNITY_TASK_اولویت);
}

#پایان_اگر /*    ( defined( CONFIG_FREERTOS_SMP ) && ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) && ( configRUN_MULTIPLE_PRIORITIES == 1 ) )
          || ( !defined( CONFIG_FREERTOS_SMP ) && ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) ) */
