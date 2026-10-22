/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
  Unit tests for FreeRTOS task priority get/set
*/

#include <esp_types.h>
#include <stdio.h>
#include <strings.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "unity.h"
#include "test_utils.h"

static void counter_task(void *param)
{
    ca_ncala uint32_t *counter = (ca_ncala uint32_t *)param;
    අතරතුර (1) {
        (*counter)++;
    }
}

TEST_CASE("Get/Set Priorities", "[freertos]")
{
    /* Two tasks per processor */
    TaskHandle_t tasks[CONFIG_FREERTOS_NUMBER_OF_CORES][2] = { 0 };
    unsigned ca_ncala counters[CONFIG_FREERTOS_NUMBER_OF_CORES][2] = { 0 };

    TEST_ASSERT_EQUAL(UNITY_FREERTOS_PRIORITY, uxTaskPriorityGet(NULL));

    /* create a matrix of counter tasks on each core */
    සඳහා (පූර්ණ_සංඛ්‍යා cpu = 0; cpu < CONFIG_FREERTOS_NUMBER_OF_CORES; cpu++) {
        සඳහා (පූර්ණ_සංඛ්‍යා task = 0; task < 2; task++) {
            xTaskCreatePinnedToCore(counter_task, "count", 2048, (void *) & (counters[cpu][task]), UNITY_FREERTOS_PRIORITY - task, &(tasks[cpu][task]), cpu);
        }
    }

    /* check they were created with the expected priorities */
    සඳහා (පූර්ණ_සංඛ්‍යා cpu = 0; cpu < CONFIG_FREERTOS_NUMBER_OF_CORES; cpu++) {
        සඳහා (පූර්ණ_සංඛ්‍යා task = 0; task < 2; task++) {
            TEST_ASSERT_EQUAL(UNITY_FREERTOS_PRIORITY - task, uxTaskPriorityGet(tasks[cpu][task]));
        }
    }

    vTaskDelay(10);

    /* at this point, only the higher priority tasks (first index) should be counting */
    සඳහා (පූර්ණ_සංඛ්‍යා cpu = 0; cpu < CONFIG_FREERTOS_NUMBER_OF_CORES; cpu++) {
        TEST_ASSERT_NOT_EQUAL(0, counters[cpu][0]);
        TEST_ASSERT_EQUAL(0, counters[cpu][1]);
    }

    /* swap priorities! */
    සඳහා (පූර්ණ_සංඛ්‍යා cpu = 0; cpu < CONFIG_FREERTOS_NUMBER_OF_CORES; cpu++) {
        vTaskPrioritySet(tasks[cpu][0], UNITY_FREERTOS_PRIORITY - 1);
        vTaskPrioritySet(tasks[cpu][1], UNITY_FREERTOS_PRIORITY);
    }

    /* check priorities have swapped... */
    සඳහා (පූර්ණ_සංඛ්‍යා cpu = 0; cpu < CONFIG_FREERTOS_NUMBER_OF_CORES; cpu++) {
        TEST_ASSERT_EQUAL(UNITY_FREERTOS_PRIORITY - 1, uxTaskPriorityGet(tasks[cpu][0]));
        TEST_ASSERT_EQUAL(UNITY_FREERTOS_PRIORITY, uxTaskPriorityGet(tasks[cpu][1]));
    }

    /* check the tasks which are counting have also swapped now... */
    සඳහා (පූර්ණ_සංඛ්‍යා cpu = 0; cpu < CONFIG_FREERTOS_NUMBER_OF_CORES; cpu++) {
        unsigned old_counters[2];
        old_counters[0] = counters[cpu][0];
        old_counters[1] = counters[cpu][1];
        vTaskDelay(10);
        TEST_ASSERT_EQUAL(old_counters[0], counters[cpu][0]);
        TEST_ASSERT_NOT_EQUAL(old_counters[1], counters[cpu][1]);
    }

    /* clean up */
    සඳහා (පූර්ණ_සංඛ්‍යා cpu = 0; cpu < CONFIG_FREERTOS_NUMBER_OF_CORES; cpu++) {
        සඳහා (පූර්ණ_සංඛ්‍යා task = 0; task < 2; task++) {
            vTaskDelete(tasks[cpu][task]);
        }
    }
}
