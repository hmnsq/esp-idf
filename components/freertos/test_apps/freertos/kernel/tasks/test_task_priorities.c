/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
  Unit tests for FreeRTOS task priority get/set
*/

#अन्तर्भाव <esp_types.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <strings.h>
#अन्तर्भाव "sdkconfig.h"
#अन्तर्भाव "freertos/FreeRTOS.h"
#अन्तर्भाव "freertos/task.h"
#अन्तर्भाव "unity.h"
#अन्तर्भाव "test_utils.h"

स्थिर शून्य counter_task(शून्य *param)
{
    चञ्चल uint32_t *counter = (चञ्चल uint32_t *)param;
    जाव (1) {
        (*counter)++;
    }
}

TEST_CASE("Get/Set Priorities", "[freertos]")
{
    /* Two tasks per processor */
    TaskHandle_t tasks[CONFIG_FREERTOS_सङ्ख्या_OF_CORES][2] = { 0 };
    unsigned चञ्चल counters[CONFIG_FREERTOS_सङ्ख्या_OF_CORES][2] = { 0 };

    TEST_ASSERT_EQUAL(UNITY_FREERTOS_PRIORITY, uxTaskPriorityGet(रिक्त));

    /* create a matrix of counter tasks on each core */
    प्रति (पूर्णाङ्क cpu = 0; cpu < CONFIG_FREERTOS_सङ्ख्या_OF_CORES; cpu++) {
        प्रति (पूर्णाङ्क task = 0; task < 2; task++) {
            xTaskCreatePinnedToCore(counter_task, "count", 2048, (शून्य *) & (counters[cpu][task]), UNITY_FREERTOS_PRIORITY - task, &(tasks[cpu][task]), cpu);
        }
    }

    /* check they were created with the expected priorities */
    प्रति (पूर्णाङ्क cpu = 0; cpu < CONFIG_FREERTOS_सङ्ख्या_OF_CORES; cpu++) {
        प्रति (पूर्णाङ्क task = 0; task < 2; task++) {
            TEST_ASSERT_EQUAL(UNITY_FREERTOS_PRIORITY - task, uxTaskPriorityGet(tasks[cpu][task]));
        }
    }

    vTaskDelay(10);

    /* at this point, only the higher priority tasks (first index) should be counting */
    प्रति (पूर्णाङ्क cpu = 0; cpu < CONFIG_FREERTOS_सङ्ख्या_OF_CORES; cpu++) {
        TEST_ASSERT_NOT_EQUAL(0, counters[cpu][0]);
        TEST_ASSERT_EQUAL(0, counters[cpu][1]);
    }

    /* swap priorities! */
    प्रति (पूर्णाङ्क cpu = 0; cpu < CONFIG_FREERTOS_सङ्ख्या_OF_CORES; cpu++) {
        vTaskPrioritySet(tasks[cpu][0], UNITY_FREERTOS_PRIORITY - 1);
        vTaskPrioritySet(tasks[cpu][1], UNITY_FREERTOS_PRIORITY);
    }

    /* check priorities have swapped... */
    प्रति (पूर्णाङ्क cpu = 0; cpu < CONFIG_FREERTOS_सङ्ख्या_OF_CORES; cpu++) {
        TEST_ASSERT_EQUAL(UNITY_FREERTOS_PRIORITY - 1, uxTaskPriorityGet(tasks[cpu][0]));
        TEST_ASSERT_EQUAL(UNITY_FREERTOS_PRIORITY, uxTaskPriorityGet(tasks[cpu][1]));
    }

    /* check the tasks which are counting have also swapped now... */
    प्रति (पूर्णाङ्क cpu = 0; cpu < CONFIG_FREERTOS_सङ्ख्या_OF_CORES; cpu++) {
        unsigned old_counters[2];
        old_counters[0] = counters[cpu][0];
        old_counters[1] = counters[cpu][1];
        vTaskDelay(10);
        TEST_ASSERT_EQUAL(old_counters[0], counters[cpu][0]);
        TEST_ASSERT_NOT_EQUAL(old_counters[1], counters[cpu][1]);
    }

    /* clean up */
    प्रति (पूर्णाङ्क cpu = 0; cpu < CONFIG_FREERTOS_सङ्ख्या_OF_CORES; cpu++) {
        प्रति (पूर्णाङ्क task = 0; task < 2; task++) {
            vTaskDelete(tasks[cpu][task]);
        }
    }
}
