/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
  Unit tests for FreeRTOS task priority get/set
*/

#شامل <esp_types.h>
#شامل <stdio.h>
#شامل <strings.h>
#شامل "sdkconfig.h"
#شامل "freertos/FreeRTOS.h"
#شامل "freertos/task.h"
#شامل "unity.h"
#شامل "test_utils.h"

ساکن خالی شمار_task(خالی *param)
{
    متلون uint32_t *شمار_2 = (متلون uint32_t *)param;
    جب (1) {
        (*شمار_2)++;
    }
}

TEST_CASE("Get/Set Priorities", "[freertos]")
{
    /* Two tasks per processor */
    TaskHandle_t tasks[CONFIG_FREERTOS_نمبر_کا_CORES][2] = { 0 };
    غیرعلامتی متلون counters[CONFIG_FREERTOS_نمبر_کا_CORES][2] = { 0 };

    TEST_ASSERT_EQUAL(UNITY_FREERTOS_ترجیح, ux_Task_ترجیح_Get(تہی));

    /* create a matrix of counter tasks on each core */
    برائے (صحیح cpu = 0; cpu < CONFIG_FREERTOS_نمبر_کا_CORES; cpu++) {
        برائے (صحیح task = 0; task < 2; task++) {
            x_Task_Create_Pinned_مقصد_Core(شمار_task, "count", 2048, (خالی *) & (counters[cpu][task]), UNITY_FREERTOS_ترجیح - task, &(tasks[cpu][task]), cpu);
        }
    }

    /* check they were created with the expected priorities */
    برائے (صحیح cpu = 0; cpu < CONFIG_FREERTOS_نمبر_کا_CORES; cpu++) {
        برائے (صحیح task = 0; task < 2; task++) {
            TEST_ASSERT_EQUAL(UNITY_FREERTOS_ترجیح - task, ux_Task_ترجیح_Get(tasks[cpu][task]));
        }
    }

    vTaskDelay(10);

    /* at this point, only the higher priority tasks (first index) should be counting */
    برائے (صحیح cpu = 0; cpu < CONFIG_FREERTOS_نمبر_کا_CORES; cpu++) {
        TEST_ASSERT_NOT_EQUAL(0, counters[cpu][0]);
        TEST_ASSERT_EQUAL(0, counters[cpu][1]);
    }

    /* swap priorities! */
    برائے (صحیح cpu = 0; cpu < CONFIG_FREERTOS_نمبر_کا_CORES; cpu++) {
        v_Task_ترجیح_سیٹ(tasks[cpu][0], UNITY_FREERTOS_ترجیح - 1);
        v_Task_ترجیح_سیٹ(tasks[cpu][1], UNITY_FREERTOS_ترجیح);
    }

    /* check priorities have swapped... */
    برائے (صحیح cpu = 0; cpu < CONFIG_FREERTOS_نمبر_کا_CORES; cpu++) {
        TEST_ASSERT_EQUAL(UNITY_FREERTOS_ترجیح - 1, ux_Task_ترجیح_Get(tasks[cpu][0]));
        TEST_ASSERT_EQUAL(UNITY_FREERTOS_ترجیح, ux_Task_ترجیح_Get(tasks[cpu][1]));
    }

    /* check the tasks which are counting have also swapped now... */
    برائے (صحیح cpu = 0; cpu < CONFIG_FREERTOS_نمبر_کا_CORES; cpu++) {
        غیرعلامتی old_counters[2];
        old_counters[0] = counters[cpu][0];
        old_counters[1] = counters[cpu][1];
        vTaskDelay(10);
        TEST_ASSERT_EQUAL(old_counters[0], counters[cpu][0]);
        TEST_ASSERT_NOT_EQUAL(old_counters[1], counters[cpu][1]);
    }

    /* clean up */
    برائے (صحیح cpu = 0; cpu < CONFIG_FREERTOS_نمبر_کا_CORES; cpu++) {
        برائے (صحیح task = 0; task < 2; task++) {
            vTaskDelete(tasks[cpu][task]);
        }
    }
}
