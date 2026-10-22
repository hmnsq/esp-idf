/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
  Unit tests for FreeRTOS task priority get/set
*/

#અન્તર્ભાવ <esp_types.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <strings.h>
#અન્તર્ભાવ "sdkconfig.h"
#અન્તર્ભાવ "freertos/FreeRTOS.h"
#અન્તર્ભાવ "freertos/task.h"
#અન્તર્ભાવ "unity.h"
#અન્તર્ભાવ "test_utils.h"

સ્થિર શૂન્ય counter_task(શૂન્ય *param)
{
    અસ્થિર uint32_t *counter = (અસ્થિર uint32_t *)param;
    જ્યારે (1) {
        (*counter)++;
    }
}

TEST_CASE("Get/Set Priorities", "[freertos]")
{
    /* Two tasks per processor */
    TaskHandle_t tasks[CONFIG_FREERTOS_સંખ્યા_OF_CORES][2] = { 0 };
    unsigned અસ્થિર counters[CONFIG_FREERTOS_સંખ્યા_OF_CORES][2] = { 0 };

    TEST_ASSERT_EQUAL(UNITY_FREERTOS_PRIORITY, uxTaskPriorityGet(ખાલી));

    /* create a matrix of counter tasks on each core */
    માટે (પૂર્ણાંક cpu = 0; cpu < CONFIG_FREERTOS_સંખ્યા_OF_CORES; cpu++) {
        માટે (પૂર્ણાંક task = 0; task < 2; task++) {
            xTaskCreatePinnedToCore(counter_task, "count", 2048, (શૂન્ય *) & (counters[cpu][task]), UNITY_FREERTOS_PRIORITY - task, &(tasks[cpu][task]), cpu);
        }
    }

    /* check they were created with the expected priorities */
    માટે (પૂર્ણાંક cpu = 0; cpu < CONFIG_FREERTOS_સંખ્યા_OF_CORES; cpu++) {
        માટે (પૂર્ણાંક task = 0; task < 2; task++) {
            TEST_ASSERT_EQUAL(UNITY_FREERTOS_PRIORITY - task, uxTaskPriorityGet(tasks[cpu][task]));
        }
    }

    vTaskDelay(10);

    /* at this point, only the higher priority tasks (first index) should be counting */
    માટે (પૂર્ણાંક cpu = 0; cpu < CONFIG_FREERTOS_સંખ્યા_OF_CORES; cpu++) {
        TEST_ASSERT_NOT_EQUAL(0, counters[cpu][0]);
        TEST_ASSERT_EQUAL(0, counters[cpu][1]);
    }

    /* swap priorities! */
    માટે (પૂર્ણાંક cpu = 0; cpu < CONFIG_FREERTOS_સંખ્યા_OF_CORES; cpu++) {
        vTaskPrioritySet(tasks[cpu][0], UNITY_FREERTOS_PRIORITY - 1);
        vTaskPrioritySet(tasks[cpu][1], UNITY_FREERTOS_PRIORITY);
    }

    /* check priorities have swapped... */
    માટે (પૂર્ણાંક cpu = 0; cpu < CONFIG_FREERTOS_સંખ્યા_OF_CORES; cpu++) {
        TEST_ASSERT_EQUAL(UNITY_FREERTOS_PRIORITY - 1, uxTaskPriorityGet(tasks[cpu][0]));
        TEST_ASSERT_EQUAL(UNITY_FREERTOS_PRIORITY, uxTaskPriorityGet(tasks[cpu][1]));
    }

    /* check the tasks which are counting have also swapped now... */
    માટે (પૂર્ણાંક cpu = 0; cpu < CONFIG_FREERTOS_સંખ્યા_OF_CORES; cpu++) {
        unsigned old_counters[2];
        old_counters[0] = counters[cpu][0];
        old_counters[1] = counters[cpu][1];
        vTaskDelay(10);
        TEST_ASSERT_EQUAL(old_counters[0], counters[cpu][0]);
        TEST_ASSERT_NOT_EQUAL(old_counters[1], counters[cpu][1]);
    }

    /* clean up */
    માટે (પૂર્ણાંક cpu = 0; cpu < CONFIG_FREERTOS_સંખ્યા_OF_CORES; cpu++) {
        માટે (પૂર્ણાંક task = 0; task < 2; task++) {
            vTaskDelete(tasks[cpu][task]);
        }
    }
}
