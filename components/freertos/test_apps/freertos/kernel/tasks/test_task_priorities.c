/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
  Unit tests for FreeRTOS task priority get/set
*/

#ضمن <esp_types.h>
#ضمن <stdio.h>
#ضمن <strings.h>
#ضمن "sdkconfig.h"
#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن "unity.h"
#ضمن "test_utils.h"

ساكن فراغ counter_task(فراغ *param)
{
    متطاير uint32_t *counter = (متطاير uint32_t *)param;
    طالما (1) {
        (*counter)++;
    }
}

TEST_CASE("Get/Set Priorities", "[freertos]")
{
    /* Two tasks per processor */
    TaskHandle_t tasks[CONFIG_FREERTOS_عدد_از_CORES][2] = { 0 };
    بیعلامت متطاير counters[CONFIG_FREERTOS_عدد_از_CORES][2] = { 0 };

    TEST_ASSERT_EQUAL(UNITY_FREERTOS_اولویت, ux_Task_اولویت_Get(عدم));

    /* create a matrix of counter tasks on each core */
    لكل (صحيح cpu = 0; cpu < CONFIG_FREERTOS_عدد_از_CORES; cpu++) {
        لكل (صحيح task = 0; task < 2; task++) {
            x_Task_Create_Pinned_مقصد_Core(counter_task, "count", 2048, (فراغ *) & (counters[cpu][task]), UNITY_FREERTOS_اولویت - task, &(tasks[cpu][task]), cpu);
        }
    }

    /* check they were created with the expected priorities */
    لكل (صحيح cpu = 0; cpu < CONFIG_FREERTOS_عدد_از_CORES; cpu++) {
        لكل (صحيح task = 0; task < 2; task++) {
            TEST_ASSERT_EQUAL(UNITY_FREERTOS_اولویت - task, ux_Task_اولویت_Get(tasks[cpu][task]));
        }
    }

    vTaskDelay(10);

    /* at this point, only the higher priority tasks (first index) should be counting */
    لكل (صحيح cpu = 0; cpu < CONFIG_FREERTOS_عدد_از_CORES; cpu++) {
        TEST_ASSERT_NOT_EQUAL(0, counters[cpu][0]);
        TEST_ASSERT_EQUAL(0, counters[cpu][1]);
    }

    /* swap priorities! */
    لكل (صحيح cpu = 0; cpu < CONFIG_FREERTOS_عدد_از_CORES; cpu++) {
        v_Task_اولویت_مجموعه(tasks[cpu][0], UNITY_FREERTOS_اولویت - 1);
        v_Task_اولویت_مجموعه(tasks[cpu][1], UNITY_FREERTOS_اولویت);
    }

    /* check priorities have swapped... */
    لكل (صحيح cpu = 0; cpu < CONFIG_FREERTOS_عدد_از_CORES; cpu++) {
        TEST_ASSERT_EQUAL(UNITY_FREERTOS_اولویت - 1, ux_Task_اولویت_Get(tasks[cpu][0]));
        TEST_ASSERT_EQUAL(UNITY_FREERTOS_اولویت, ux_Task_اولویت_Get(tasks[cpu][1]));
    }

    /* check the tasks which are counting have also swapped now... */
    لكل (صحيح cpu = 0; cpu < CONFIG_FREERTOS_عدد_از_CORES; cpu++) {
        بیعلامت old_counters[2];
        old_counters[0] = counters[cpu][0];
        old_counters[1] = counters[cpu][1];
        vTaskDelay(10);
        TEST_ASSERT_EQUAL(old_counters[0], counters[cpu][0]);
        TEST_ASSERT_NOT_EQUAL(old_counters[1], counters[cpu][1]);
    }

    /* clean up */
    لكل (صحيح cpu = 0; cpu < CONFIG_FREERTOS_عدد_از_CORES; cpu++) {
        لكل (صحيح task = 0; task < 2; task++) {
            vTaskDelete(tasks[cpu][task]);
        }
    }
}
