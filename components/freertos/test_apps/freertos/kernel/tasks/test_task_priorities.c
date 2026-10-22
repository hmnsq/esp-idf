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
    ಅಸ್ಥಿರ uint32_t *counter = (ಅಸ್ಥಿರ uint32_t *)param;
    ಆಗ (1) {
        (*counter)++;
    }
}

TEST_CASE("Get/Set Priorities", "[freertos]")
{
    /* Two tasks per processor */
    TaskHandle_t tasks[CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES][2] = { 0 };
    ಚಿಹ್ನೆರಹಿತ ಅಸ್ಥಿರ counters[CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES][2] = { 0 };

    TEST_ASSERT_EQUAL(UNITY_FREERTOS_ಆದ್ಯತೆ, ux_Task_ಆದ್ಯತೆ_Get(ಖಾಲಿ));

    /* create a matrix of counter tasks on each core */
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ cpu = 0; cpu < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; cpu++) {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ task = 0; task < 2; task++) {
            x_Task_Create_Pinned_ಗಮ್ಯ_Core(counter_task, "count", 2048, (ಶೂನ್ಯ *) & (counters[cpu][task]), UNITY_FREERTOS_ಆದ್ಯತೆ - task, &(tasks[cpu][task]), cpu);
        }
    }

    /* check they were created with the expected priorities */
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ cpu = 0; cpu < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; cpu++) {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ task = 0; task < 2; task++) {
            TEST_ASSERT_EQUAL(UNITY_FREERTOS_ಆದ್ಯತೆ - task, ux_Task_ಆದ್ಯತೆ_Get(tasks[cpu][task]));
        }
    }

    vTaskDelay(10);

    /* at this point, only the higher priority tasks (first index) should be counting */
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ cpu = 0; cpu < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; cpu++) {
        TEST_ASSERT_NOT_EQUAL(0, counters[cpu][0]);
        TEST_ASSERT_EQUAL(0, counters[cpu][1]);
    }

    /* swap priorities! */
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ cpu = 0; cpu < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; cpu++) {
        v_Task_ಆದ್ಯತೆ_ಗಣ(tasks[cpu][0], UNITY_FREERTOS_ಆದ್ಯತೆ - 1);
        v_Task_ಆದ್ಯತೆ_ಗಣ(tasks[cpu][1], UNITY_FREERTOS_ಆದ್ಯತೆ);
    }

    /* check priorities have swapped... */
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ cpu = 0; cpu < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; cpu++) {
        TEST_ASSERT_EQUAL(UNITY_FREERTOS_ಆದ್ಯತೆ - 1, ux_Task_ಆದ್ಯತೆ_Get(tasks[cpu][0]));
        TEST_ASSERT_EQUAL(UNITY_FREERTOS_ಆದ್ಯತೆ, ux_Task_ಆದ್ಯತೆ_Get(tasks[cpu][1]));
    }

    /* check the tasks which are counting have also swapped now... */
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ cpu = 0; cpu < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; cpu++) {
        ಚಿಹ್ನೆರಹಿತ old_counters[2];
        old_counters[0] = counters[cpu][0];
        old_counters[1] = counters[cpu][1];
        vTaskDelay(10);
        TEST_ASSERT_EQUAL(old_counters[0], counters[cpu][0]);
        TEST_ASSERT_NOT_EQUAL(old_counters[1], counters[cpu][1]);
    }

    /* clean up */
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ cpu = 0; cpu < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; cpu++) {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ task = 0; task < 2; task++) {
            vTaskDelete(tasks[cpu][task]);
        }
    }
}
