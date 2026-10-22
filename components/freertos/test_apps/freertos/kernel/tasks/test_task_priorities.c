/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
  Unit tests for FreeRTOS task priority get/set
*/

#ଅନ୍ତର୍ଭାଵ <esp_types.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <strings.h>
#ଅନ୍ତର୍ଭାଵ "sdkconfig.h"
#ଅନ୍ତର୍ଭାଵ "freertos/FreeRTOS.h"
#ଅନ୍ତର୍ଭାଵ "freertos/task.h"
#ଅନ୍ତର୍ଭାଵ "unity.h"
#ଅନ୍ତର୍ଭାଵ "test_utils.h"

ସ୍ଥିର ଶୂନ୍ୟ counter_task(ଶୂନ୍ୟ *param)
{
    ଅସ୍ଥିର uint32_t *counter = (ଅସ୍ଥିର uint32_t *)param;
    ଯେତେବେଳେ (1) {
        (*counter)++;
    }
}

TEST_CASE("Get/Set Priorities", "[freertos]")
{
    /* Two tasks per processor */
    TaskHandle_t tasks[CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES][2] = { 0 };
    unsigned ଅସ୍ଥିର counters[CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES][2] = { 0 };

    TEST_ASSERT_EQUAL(UNITY_FREERTOS_PRIORITY, uxTaskPriorityGet(ଖାଲି));

    /* create a matrix of counter tasks on each core */
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu = 0; cpu < CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES; cpu++) {
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା task = 0; task < 2; task++) {
            xTaskCreatePinnedToCore(counter_task, "count", 2048, (ଶୂନ୍ୟ *) & (counters[cpu][task]), UNITY_FREERTOS_PRIORITY - task, &(tasks[cpu][task]), cpu);
        }
    }

    /* check they were created with the expected priorities */
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu = 0; cpu < CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES; cpu++) {
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା task = 0; task < 2; task++) {
            TEST_ASSERT_EQUAL(UNITY_FREERTOS_PRIORITY - task, uxTaskPriorityGet(tasks[cpu][task]));
        }
    }

    vTaskDelay(10);

    /* at this point, only the higher priority tasks (first index) should be counting */
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu = 0; cpu < CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES; cpu++) {
        TEST_ASSERT_NOT_EQUAL(0, counters[cpu][0]);
        TEST_ASSERT_EQUAL(0, counters[cpu][1]);
    }

    /* swap priorities! */
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu = 0; cpu < CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES; cpu++) {
        vTaskPrioritySet(tasks[cpu][0], UNITY_FREERTOS_PRIORITY - 1);
        vTaskPrioritySet(tasks[cpu][1], UNITY_FREERTOS_PRIORITY);
    }

    /* check priorities have swapped... */
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu = 0; cpu < CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES; cpu++) {
        TEST_ASSERT_EQUAL(UNITY_FREERTOS_PRIORITY - 1, uxTaskPriorityGet(tasks[cpu][0]));
        TEST_ASSERT_EQUAL(UNITY_FREERTOS_PRIORITY, uxTaskPriorityGet(tasks[cpu][1]));
    }

    /* check the tasks which are counting have also swapped now... */
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu = 0; cpu < CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES; cpu++) {
        unsigned old_counters[2];
        old_counters[0] = counters[cpu][0];
        old_counters[1] = counters[cpu][1];
        vTaskDelay(10);
        TEST_ASSERT_EQUAL(old_counters[0], counters[cpu][0]);
        TEST_ASSERT_NOT_EQUAL(old_counters[1], counters[cpu][1]);
    }

    /* clean up */
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpu = 0; cpu < CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES; cpu++) {
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା task = 0; task < 2; task++) {
            vTaskDelete(tasks[cpu][task]);
        }
    }
}
