/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ <string.h>
#అన్తర్భావ "FreeRTOS.h"
#అన్తర్భావ "task.h"
#అన్తర్భావ "semphr.h"
#అన్తర్భావ "unity.h"
#అన్తర్భావ "portTestMacro.h"

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

#ఒకవేళ    ( defined( CONFIG_FREERTOS_SMP ) && ( CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES > 1 ) && ( config_నిరంతరం_గుణిజం_PRIORITIES == 1 ) ) \
    || ( !defined( CONFIG_FREERTOS_SMP ) && ( CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES > 1 ) )

#నిర్వచించు UNITY_TASK_DELAY_TICKS      10

స్థిర పునాది_Type_t task_A_ran[CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES];
స్థిర పునాది_Type_t task_B_ran[CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES];

స్థిర శూన్య task_A(శూన్య *arg)
{
    పునాది_Type_t task_idx = (పునాది_Type_t) arg;
    task_A_ran[task_idx] = pdTRUE;
    /* Keeping spinning to prevent the lower priority task_B from running */
    అయితే (1) {
        ;
    }
}

స్థిర శూన్య task_B(శూన్య *arg)
{
    /* The following should never be run due to task_B having a lower priority */
    పునాది_Type_t task_idx = (పునాది_Type_t) arg;
    task_B_ran[task_idx] = pdTRUE;
    అయితే (1) {
        ;
    }
}

TEST_CASE("Tasks: Test priority scheduling (SMP)", "[freertos]")
{
    TaskHandle_t task_A_handles[CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES];
    TaskHandle_t task_B_handles[CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES];
    memset(task_A_ran, pdFALSE, పరిమాణం(task_A_ran));
    memset(task_B_ran, pdFALSE, పరిమాణం(task_B_ran));

    /* Raise the priority of the unityTask */
    v_Task_ప్రాధాన్యత_సమితి(ఖాళీ, configMAX_PRIORITIES - 1);

    /* Create task_A for each core */
    కొరకు (UBaseType_t x = 0; x < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; x++) {
        xTaskCreate(task_A, "task_A", config_TEST_DEFAULT_దొంతర_కొలత, (శూన్య *)x, configMAX_PRIORITIES - 2, &task_A_handles[x]);
    }

    /* Create task_B for each core */
    కొరకు (UBaseType_t x = 0; x < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; x++) {
        xTaskCreate(task_B, "task_B", config_TEST_DEFAULT_దొంతర_కొలత, (శూన్య *)x, configMAX_PRIORITIES - 3, &task_B_handles[x]);
    }

    /* Block to ensure all the task_As to be scheduled */
    vTaskDelay(UNITY_TASK_DELAY_TICKS);

    /* Check that all the task_As have run, and none of the task_Bs have run */
    కొరకు (UBaseType_t x = 0; x < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; x++) {
        TEST_ASSERT_EQUAL(pdTRUE, task_A_ran[x]);
        TEST_ASSERT_EQUAL(pdFALSE, task_B_ran[x]);
    }

    /* Cleanup */
    కొరకు (UBaseType_t x = 0; x < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; x++) {
        vTaskDelete(task_A_handles[x]);
        vTaskDelete(task_B_handles[x]);
    }

    /* Restore the priority of the unityTask */
    v_Task_ప్రాధాన్యత_సమితి(ఖాళీ, config_TEST_UNITY_TASK_ప్రాధాన్యత);
}

#అయితే_ముగింపు /*    ( defined( CONFIG_FREERTOS_SMP ) && ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) && ( configRUN_MULTIPLE_PRIORITIES == 1 ) )
          || ( !defined( CONFIG_FREERTOS_SMP ) && ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) ) */
