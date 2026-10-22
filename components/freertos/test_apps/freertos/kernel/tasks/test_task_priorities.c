/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
  Unit tests for FreeRTOS task priority get/set
*/

#அந்தர்பாவ <esp_types.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <strings.h>
#அந்தர்பாவ "sdkconfig.h"
#அந்தர்பாவ "freertos/FreeRTOS.h"
#அந்தர்பாவ "freertos/task.h"
#அந்தர்பாவ "unity.h"
#அந்தர்பாவ "test_utils.h"

நிலையான வெறுமை counter_task(வெறுமை *param)
{
    நிலையற்ற uint32_t *counter = (நிலையற்ற uint32_t *)param;
    போது (1) {
        (*counter)++;
    }
}

TEST_CASE("Get/Set Priorities", "[freertos]")
{
    /* Two tasks per processor */
    TaskHandle_t tasks[CONFIG_FREERTOS_எண்_OF_CORES][2] = { 0 };
    unsigned நிலையற்ற counters[CONFIG_FREERTOS_எண்_OF_CORES][2] = { 0 };

    TEST_ASSERT_EQUAL(UNITY_FREERTOS_PRIORITY, uxTaskPriorityGet(வெற்று));

    /* create a matrix of counter tasks on each core */
    க்கு (முழு cpu = 0; cpu < CONFIG_FREERTOS_எண்_OF_CORES; cpu++) {
        க்கு (முழு task = 0; task < 2; task++) {
            xTaskCreatePinnedToCore(counter_task, "count", 2048, (வெறுமை *) & (counters[cpu][task]), UNITY_FREERTOS_PRIORITY - task, &(tasks[cpu][task]), cpu);
        }
    }

    /* check they were created with the expected priorities */
    க்கு (முழு cpu = 0; cpu < CONFIG_FREERTOS_எண்_OF_CORES; cpu++) {
        க்கு (முழு task = 0; task < 2; task++) {
            TEST_ASSERT_EQUAL(UNITY_FREERTOS_PRIORITY - task, uxTaskPriorityGet(tasks[cpu][task]));
        }
    }

    vTaskDelay(10);

    /* at this point, only the higher priority tasks (first index) should be counting */
    க்கு (முழு cpu = 0; cpu < CONFIG_FREERTOS_எண்_OF_CORES; cpu++) {
        TEST_ASSERT_NOT_EQUAL(0, counters[cpu][0]);
        TEST_ASSERT_EQUAL(0, counters[cpu][1]);
    }

    /* swap priorities! */
    க்கு (முழு cpu = 0; cpu < CONFIG_FREERTOS_எண்_OF_CORES; cpu++) {
        vTaskPrioritySet(tasks[cpu][0], UNITY_FREERTOS_PRIORITY - 1);
        vTaskPrioritySet(tasks[cpu][1], UNITY_FREERTOS_PRIORITY);
    }

    /* check priorities have swapped... */
    க்கு (முழு cpu = 0; cpu < CONFIG_FREERTOS_எண்_OF_CORES; cpu++) {
        TEST_ASSERT_EQUAL(UNITY_FREERTOS_PRIORITY - 1, uxTaskPriorityGet(tasks[cpu][0]));
        TEST_ASSERT_EQUAL(UNITY_FREERTOS_PRIORITY, uxTaskPriorityGet(tasks[cpu][1]));
    }

    /* check the tasks which are counting have also swapped now... */
    க்கு (முழு cpu = 0; cpu < CONFIG_FREERTOS_எண்_OF_CORES; cpu++) {
        unsigned old_counters[2];
        old_counters[0] = counters[cpu][0];
        old_counters[1] = counters[cpu][1];
        vTaskDelay(10);
        TEST_ASSERT_EQUAL(old_counters[0], counters[cpu][0]);
        TEST_ASSERT_NOT_EQUAL(old_counters[1], counters[cpu][1]);
    }

    /* clean up */
    க்கு (முழு cpu = 0; cpu < CONFIG_FREERTOS_எண்_OF_CORES; cpu++) {
        க்கு (முழு task = 0; task < 2; task++) {
            vTaskDelete(tasks[cpu][task]);
        }
    }
}
