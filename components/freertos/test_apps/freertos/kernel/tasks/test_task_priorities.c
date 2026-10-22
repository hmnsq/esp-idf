/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
  Unit tests for FreeRTOS task priority get/set
*/

#অন্তর্ভাৱ <esp_types.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <strings.h>
#অন্তর্ভাৱ "sdkconfig.h"
#অন্তর্ভাৱ "freertos/FreeRTOS.h"
#অন্তর্ভাৱ "freertos/task.h"
#অন্তর্ভাৱ "unity.h"
#অন্তর্ভাৱ "test_utils.h"

স্থির শূন্য counter_task(শূন্য *param)
{
    চঞ্চল uint32_t *counter = (চঞ্চল uint32_t *)param;
    যেতিয়ালৈ (1) {
        (*counter)++;
    }
}

TEST_CASE("Get/Set Priorities", "[freertos]")
{
    /* Two tasks per processor */
    TaskHandle_t tasks[CONFIG_FREERTOS_সংখ্যা_OF_CORES][2] = { 0 };
    unsigned চঞ্চল counters[CONFIG_FREERTOS_সংখ্যা_OF_CORES][2] = { 0 };

    TEST_ASSERT_EQUAL(UNITY_FREERTOS_PRIORITY, uxTaskPriorityGet(রিক্ত));

    /* create a matrix of counter tasks on each core */
    বাবে (পূর্ণাঙ্ক cpu = 0; cpu < CONFIG_FREERTOS_সংখ্যা_OF_CORES; cpu++) {
        বাবে (পূর্ণাঙ্ক task = 0; task < 2; task++) {
            xTaskCreatePinnedToCore(counter_task, "count", 2048, (শূন্য *) & (counters[cpu][task]), UNITY_FREERTOS_PRIORITY - task, &(tasks[cpu][task]), cpu);
        }
    }

    /* check they were created with the expected priorities */
    বাবে (পূর্ণাঙ্ক cpu = 0; cpu < CONFIG_FREERTOS_সংখ্যা_OF_CORES; cpu++) {
        বাবে (পূর্ণাঙ্ক task = 0; task < 2; task++) {
            TEST_ASSERT_EQUAL(UNITY_FREERTOS_PRIORITY - task, uxTaskPriorityGet(tasks[cpu][task]));
        }
    }

    vTaskDelay(10);

    /* at this point, only the higher priority tasks (first index) should be counting */
    বাবে (পূর্ণাঙ্ক cpu = 0; cpu < CONFIG_FREERTOS_সংখ্যা_OF_CORES; cpu++) {
        TEST_ASSERT_NOT_EQUAL(0, counters[cpu][0]);
        TEST_ASSERT_EQUAL(0, counters[cpu][1]);
    }

    /* swap priorities! */
    বাবে (পূর্ণাঙ্ক cpu = 0; cpu < CONFIG_FREERTOS_সংখ্যা_OF_CORES; cpu++) {
        vTaskPrioritySet(tasks[cpu][0], UNITY_FREERTOS_PRIORITY - 1);
        vTaskPrioritySet(tasks[cpu][1], UNITY_FREERTOS_PRIORITY);
    }

    /* check priorities have swapped... */
    বাবে (পূর্ণাঙ্ক cpu = 0; cpu < CONFIG_FREERTOS_সংখ্যা_OF_CORES; cpu++) {
        TEST_ASSERT_EQUAL(UNITY_FREERTOS_PRIORITY - 1, uxTaskPriorityGet(tasks[cpu][0]));
        TEST_ASSERT_EQUAL(UNITY_FREERTOS_PRIORITY, uxTaskPriorityGet(tasks[cpu][1]));
    }

    /* check the tasks which are counting have also swapped now... */
    বাবে (পূর্ণাঙ্ক cpu = 0; cpu < CONFIG_FREERTOS_সংখ্যা_OF_CORES; cpu++) {
        unsigned old_counters[2];
        old_counters[0] = counters[cpu][0];
        old_counters[1] = counters[cpu][1];
        vTaskDelay(10);
        TEST_ASSERT_EQUAL(old_counters[0], counters[cpu][0]);
        TEST_ASSERT_NOT_EQUAL(old_counters[1], counters[cpu][1]);
    }

    /* clean up */
    বাবে (পূর্ণাঙ্ক cpu = 0; cpu < CONFIG_FREERTOS_সংখ্যা_OF_CORES; cpu++) {
        বাবে (পূর্ণাঙ্ক task = 0; task < 2; task++) {
            vTaskDelete(tasks[cpu][task]);
        }
    }
}
