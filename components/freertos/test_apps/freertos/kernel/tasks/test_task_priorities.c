/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
  Unit tests for FreeRTOS task priority get/set
*/

#ಅನ್ತರ್ಭಾವ <esp_types.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <strings.h>
#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ "unity.h"
#ಅನ್ತರ್ಭಾವ "test_utils.h"

ಸ್ಥಿರ ಶೂನ್ಯ counter_task(ಶೂನ್ಯ *param)
{
    ಚಞ್ಚಲ uint32_t *counter = (ಚಞ್ಚಲ uint32_t *)param;
    ಯಾವತ್ (1) {
        (*counter)++;
    }
}

TEST_CASE("Get/Set Priorities", "[freertos]")
{
    /* Two tasks per processor */
    TaskHandle_t tasks[CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES][2] = { 0 };
    unsigned ಚಞ್ಚಲ counters[CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES][2] = { 0 };

    TEST_ASSERT_EQUAL(UNITY_FREERTOS_PRIORITY, uxTaskPriorityGet(ರಿಕ್ತ));

    /* create a matrix of counter tasks on each core */
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ cpu = 0; cpu < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES; cpu++) {
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ task = 0; task < 2; task++) {
            xTaskCreatePinnedToCore(counter_task, "count", 2048, (ಶೂನ್ಯ *) & (counters[cpu][task]), UNITY_FREERTOS_PRIORITY - task, &(tasks[cpu][task]), cpu);
        }
    }

    /* check they were created with the expected priorities */
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ cpu = 0; cpu < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES; cpu++) {
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ task = 0; task < 2; task++) {
            TEST_ASSERT_EQUAL(UNITY_FREERTOS_PRIORITY - task, uxTaskPriorityGet(tasks[cpu][task]));
        }
    }

    vTaskDelay(10);

    /* at this point, only the higher priority tasks (first index) should be counting */
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ cpu = 0; cpu < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES; cpu++) {
        TEST_ASSERT_NOT_EQUAL(0, counters[cpu][0]);
        TEST_ASSERT_EQUAL(0, counters[cpu][1]);
    }

    /* swap priorities! */
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ cpu = 0; cpu < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES; cpu++) {
        vTaskPrioritySet(tasks[cpu][0], UNITY_FREERTOS_PRIORITY - 1);
        vTaskPrioritySet(tasks[cpu][1], UNITY_FREERTOS_PRIORITY);
    }

    /* check priorities have swapped... */
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ cpu = 0; cpu < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES; cpu++) {
        TEST_ASSERT_EQUAL(UNITY_FREERTOS_PRIORITY - 1, uxTaskPriorityGet(tasks[cpu][0]));
        TEST_ASSERT_EQUAL(UNITY_FREERTOS_PRIORITY, uxTaskPriorityGet(tasks[cpu][1]));
    }

    /* check the tasks which are counting have also swapped now... */
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ cpu = 0; cpu < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES; cpu++) {
        unsigned old_counters[2];
        old_counters[0] = counters[cpu][0];
        old_counters[1] = counters[cpu][1];
        vTaskDelay(10);
        TEST_ASSERT_EQUAL(old_counters[0], counters[cpu][0]);
        TEST_ASSERT_NOT_EQUAL(old_counters[1], counters[cpu][1]);
    }

    /* clean up */
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ cpu = 0; cpu < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES; cpu++) {
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ task = 0; task < 2; task++) {
            vTaskDelete(tasks[cpu][task]);
        }
    }
}
