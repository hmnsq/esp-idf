/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
  Unit tests for FreeRTOS task priority get/set
*/

#అన్తర్భావ <esp_types.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <strings.h>
#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ "unity.h"
#అన్తర్భావ "test_utils.h"

స్థిర శూన్య counter_task(శూన్య *param)
{
    అస్థిర uint32_t *counter = (అస్థిర uint32_t *)param;
    అయితే (1) {
        (*counter)++;
    }
}

TEST_CASE("Get/Set Priorities", "[freertos]")
{
    /* Two tasks per processor */
    TaskHandle_t tasks[CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES][2] = { 0 };
    గుర్తులేని అస్థిర counters[CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES][2] = { 0 };

    TEST_ASSERT_EQUAL(UNITY_FREERTOS_ప్రాధాన్యత, ux_Task_ప్రాధాన్యత_Get(ఖాళీ));

    /* create a matrix of counter tasks on each core */
    కొరకు (పూర్ణాంకం cpu = 0; cpu < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; cpu++) {
        కొరకు (పూర్ణాంకం task = 0; task < 2; task++) {
            x_Task_Create_Pinned_గమ్యం_Core(counter_task, "count", 2048, (శూన్య *) & (counters[cpu][task]), UNITY_FREERTOS_ప్రాధాన్యత - task, &(tasks[cpu][task]), cpu);
        }
    }

    /* check they were created with the expected priorities */
    కొరకు (పూర్ణాంకం cpu = 0; cpu < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; cpu++) {
        కొరకు (పూర్ణాంకం task = 0; task < 2; task++) {
            TEST_ASSERT_EQUAL(UNITY_FREERTOS_ప్రాధాన్యత - task, ux_Task_ప్రాధాన్యత_Get(tasks[cpu][task]));
        }
    }

    vTaskDelay(10);

    /* at this point, only the higher priority tasks (first index) should be counting */
    కొరకు (పూర్ణాంకం cpu = 0; cpu < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; cpu++) {
        TEST_ASSERT_NOT_EQUAL(0, counters[cpu][0]);
        TEST_ASSERT_EQUAL(0, counters[cpu][1]);
    }

    /* swap priorities! */
    కొరకు (పూర్ణాంకం cpu = 0; cpu < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; cpu++) {
        v_Task_ప్రాధాన్యత_సమితి(tasks[cpu][0], UNITY_FREERTOS_ప్రాధాన్యత - 1);
        v_Task_ప్రాధాన్యత_సమితి(tasks[cpu][1], UNITY_FREERTOS_ప్రాధాన్యత);
    }

    /* check priorities have swapped... */
    కొరకు (పూర్ణాంకం cpu = 0; cpu < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; cpu++) {
        TEST_ASSERT_EQUAL(UNITY_FREERTOS_ప్రాధాన్యత - 1, ux_Task_ప్రాధాన్యత_Get(tasks[cpu][0]));
        TEST_ASSERT_EQUAL(UNITY_FREERTOS_ప్రాధాన్యత, ux_Task_ప్రాధాన్యత_Get(tasks[cpu][1]));
    }

    /* check the tasks which are counting have also swapped now... */
    కొరకు (పూర్ణాంకం cpu = 0; cpu < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; cpu++) {
        గుర్తులేని old_counters[2];
        old_counters[0] = counters[cpu][0];
        old_counters[1] = counters[cpu][1];
        vTaskDelay(10);
        TEST_ASSERT_EQUAL(old_counters[0], counters[cpu][0]);
        TEST_ASSERT_NOT_EQUAL(old_counters[1], counters[cpu][1]);
    }

    /* clean up */
    కొరకు (పూర్ణాంకం cpu = 0; cpu < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; cpu++) {
        కొరకు (పూర్ణాంకం task = 0; task < 2; task++) {
            vTaskDelete(tasks[cpu][task]);
        }
    }
}
