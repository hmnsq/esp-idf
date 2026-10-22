/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
  Unit tests for FreeRTOS task priority get/set
*/

#അന്തര്ഭാവ <esp_types.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <strings.h>
#അന്തര്ഭാവ "sdkconfig.h"
#അന്തര്ഭാവ "freertos/FreeRTOS.h"
#അന്തര്ഭാവ "freertos/task.h"
#അന്തര്ഭാവ "unity.h"
#അന്തര്ഭാവ "test_utils.h"

സ്ഥിതം ശൂന്യം counter_task(ശൂന്യം *param)
{
    അസ്ഥിര uint32_t *counter = (അസ്ഥിര uint32_t *)param;
    ആകുമ്പോൾ (1) {
        (*counter)++;
    }
}

TEST_CASE("Get/Set Priorities", "[freertos]")
{
    /* Two tasks per processor */
    TaskHandle_t tasks[CONFIG_FREERTOS_സംഖ്യ_OF_CORES][2] = { 0 };
    unsigned അസ്ഥിര counters[CONFIG_FREERTOS_സംഖ്യ_OF_CORES][2] = { 0 };

    TEST_ASSERT_EQUAL(UNITY_FREERTOS_PRIORITY, uxTaskPriorityGet(ശൂന്യ));

    /* create a matrix of counter tasks on each core */
    വേണ്ടി (പൂർണ്ണസംഖ്യ cpu = 0; cpu < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; cpu++) {
        വേണ്ടി (പൂർണ്ണസംഖ്യ task = 0; task < 2; task++) {
            xTaskCreatePinnedToCore(counter_task, "count", 2048, (ശൂന്യം *) & (counters[cpu][task]), UNITY_FREERTOS_PRIORITY - task, &(tasks[cpu][task]), cpu);
        }
    }

    /* check they were created with the expected priorities */
    വേണ്ടി (പൂർണ്ണസംഖ്യ cpu = 0; cpu < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; cpu++) {
        വേണ്ടി (പൂർണ്ണസംഖ്യ task = 0; task < 2; task++) {
            TEST_ASSERT_EQUAL(UNITY_FREERTOS_PRIORITY - task, uxTaskPriorityGet(tasks[cpu][task]));
        }
    }

    vTaskDelay(10);

    /* at this point, only the higher priority tasks (first index) should be counting */
    വേണ്ടി (പൂർണ്ണസംഖ്യ cpu = 0; cpu < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; cpu++) {
        TEST_ASSERT_NOT_EQUAL(0, counters[cpu][0]);
        TEST_ASSERT_EQUAL(0, counters[cpu][1]);
    }

    /* swap priorities! */
    വേണ്ടി (പൂർണ്ണസംഖ്യ cpu = 0; cpu < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; cpu++) {
        vTaskPrioritySet(tasks[cpu][0], UNITY_FREERTOS_PRIORITY - 1);
        vTaskPrioritySet(tasks[cpu][1], UNITY_FREERTOS_PRIORITY);
    }

    /* check priorities have swapped... */
    വേണ്ടി (പൂർണ്ണസംഖ്യ cpu = 0; cpu < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; cpu++) {
        TEST_ASSERT_EQUAL(UNITY_FREERTOS_PRIORITY - 1, uxTaskPriorityGet(tasks[cpu][0]));
        TEST_ASSERT_EQUAL(UNITY_FREERTOS_PRIORITY, uxTaskPriorityGet(tasks[cpu][1]));
    }

    /* check the tasks which are counting have also swapped now... */
    വേണ്ടി (പൂർണ്ണസംഖ്യ cpu = 0; cpu < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; cpu++) {
        unsigned old_counters[2];
        old_counters[0] = counters[cpu][0];
        old_counters[1] = counters[cpu][1];
        vTaskDelay(10);
        TEST_ASSERT_EQUAL(old_counters[0], counters[cpu][0]);
        TEST_ASSERT_NOT_EQUAL(old_counters[1], counters[cpu][1]);
    }

    /* clean up */
    വേണ്ടി (പൂർണ്ണസംഖ്യ cpu = 0; cpu < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; cpu++) {
        വേണ്ടി (പൂർണ്ണസംഖ്യ task = 0; task < 2; task++) {
            vTaskDelete(tasks[cpu][task]);
        }
    }
}
