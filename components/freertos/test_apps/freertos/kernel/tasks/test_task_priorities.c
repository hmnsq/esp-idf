/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
  Unit tests for FreeRTOS task priority get/set
*/

#समावेश <esp_types.h>
#समावेश <stdio.h>
#समावेश <strings.h>
#समावेश "sdkconfig.h"
#समावेश "freertos/FreeRTOS.h"
#समावेश "freertos/task.h"
#समावेश "unity.h"
#समावेश "test_utils.h"

अटल व्योम गणक_task(व्योम *param)
{
    अस्थिर uint32_t *गणक = (अस्थिर uint32_t *)param;
    जबतक (1) {
        (*गणक)++;
    }
}

TEST_CASE("Get/Set Priorities", "[freertos]")
{
    /* Two tasks per processor */
    TaskHandle_t tasks[CONFIG_FREERTOS_संख्या_OF_CORES][2] = { 0 };
    अचिन्हित अस्थिर counters[CONFIG_FREERTOS_संख्या_OF_CORES][2] = { 0 };

    TEST_ASSERT_EQUAL(UNITY_FREERTOS_PRIORITY, uxTaskPriorityGet(शून्य));

    /* create a matrix of counter tasks on each core */
    क्रम (पूर्णांक cpu = 0; cpu < CONFIG_FREERTOS_संख्या_OF_CORES; cpu++) {
        क्रम (पूर्णांक task = 0; task < 2; task++) {
            xTaskCreatePinnedToCore(गणक_task, "count", 2048, (व्योम *) & (counters[cpu][task]), UNITY_FREERTOS_PRIORITY - task, &(tasks[cpu][task]), cpu);
        }
    }

    /* check they were created with the expected priorities */
    क्रम (पूर्णांक cpu = 0; cpu < CONFIG_FREERTOS_संख्या_OF_CORES; cpu++) {
        क्रम (पूर्णांक task = 0; task < 2; task++) {
            TEST_ASSERT_EQUAL(UNITY_FREERTOS_PRIORITY - task, uxTaskPriorityGet(tasks[cpu][task]));
        }
    }

    vTaskDelay(10);

    /* at this point, only the higher priority tasks (first index) should be counting */
    क्रम (पूर्णांक cpu = 0; cpu < CONFIG_FREERTOS_संख्या_OF_CORES; cpu++) {
        TEST_ASSERT_NOT_EQUAL(0, counters[cpu][0]);
        TEST_ASSERT_EQUAL(0, counters[cpu][1]);
    }

    /* swap priorities! */
    क्रम (पूर्णांक cpu = 0; cpu < CONFIG_FREERTOS_संख्या_OF_CORES; cpu++) {
        vTaskPrioritySet(tasks[cpu][0], UNITY_FREERTOS_PRIORITY - 1);
        vTaskPrioritySet(tasks[cpu][1], UNITY_FREERTOS_PRIORITY);
    }

    /* check priorities have swapped... */
    क्रम (पूर्णांक cpu = 0; cpu < CONFIG_FREERTOS_संख्या_OF_CORES; cpu++) {
        TEST_ASSERT_EQUAL(UNITY_FREERTOS_PRIORITY - 1, uxTaskPriorityGet(tasks[cpu][0]));
        TEST_ASSERT_EQUAL(UNITY_FREERTOS_PRIORITY, uxTaskPriorityGet(tasks[cpu][1]));
    }

    /* check the tasks which are counting have also swapped now... */
    क्रम (पूर्णांक cpu = 0; cpu < CONFIG_FREERTOS_संख्या_OF_CORES; cpu++) {
        अचिन्हित old_counters[2];
        old_counters[0] = counters[cpu][0];
        old_counters[1] = counters[cpu][1];
        vTaskDelay(10);
        TEST_ASSERT_EQUAL(old_counters[0], counters[cpu][0]);
        TEST_ASSERT_NOT_EQUAL(old_counters[1], counters[cpu][1]);
    }

    /* clean up */
    क्रम (पूर्णांक cpu = 0; cpu < CONFIG_FREERTOS_संख्या_OF_CORES; cpu++) {
        क्रम (पूर्णांक task = 0; task < 2; task++) {
            vTaskDelete(tasks[cpu][task]);
        }
    }
}
