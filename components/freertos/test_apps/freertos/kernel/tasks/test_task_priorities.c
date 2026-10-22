/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
  Unit tests for FreeRTOS task priority get/set
*/

#ਅਨ੍ਤਰ੍ਭਾਵ <esp_types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <strings.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unity.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "test_utils.h"

ਸਥਿਰ ਖਾਲੀ counter_task(ਖਾਲੀ *param)
{
    ਅਸਥਿਰ uint32_t *counter = (ਅਸਥਿਰ uint32_t *)param;
    ਜਦੋਂ (1) {
        (*counter)++;
    }
}

TEST_CASE("Get/Set Priorities", "[freertos]")
{
    /* Two tasks per processor */
    TaskHandle_t tasks[CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES][2] = { 0 };
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅਸਥਿਰ counters[CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES][2] = { 0 };

    TEST_ASSERT_EQUAL(UNITY_FREERTOS_ਤਰਜੀਹ, ux_Task_ਤਰਜੀਹ_Get(ਨਿਰਾ));

    /* create a matrix of counter tasks on each core */
    ਲਈ (ਪੂਰਨ cpu = 0; cpu < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; cpu++) {
        ਲਈ (ਪੂਰਨ task = 0; task < 2; task++) {
            x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(counter_task, "count", 2048, (ਖਾਲੀ *) & (counters[cpu][task]), UNITY_FREERTOS_ਤਰਜੀਹ - task, &(tasks[cpu][task]), cpu);
        }
    }

    /* check they were created with the expected priorities */
    ਲਈ (ਪੂਰਨ cpu = 0; cpu < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; cpu++) {
        ਲਈ (ਪੂਰਨ task = 0; task < 2; task++) {
            TEST_ASSERT_EQUAL(UNITY_FREERTOS_ਤਰਜੀਹ - task, ux_Task_ਤਰਜੀਹ_Get(tasks[cpu][task]));
        }
    }

    vTaskDelay(10);

    /* at this point, only the higher priority tasks (first index) should be counting */
    ਲਈ (ਪੂਰਨ cpu = 0; cpu < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; cpu++) {
        TEST_ASSERT_NOT_EQUAL(0, counters[cpu][0]);
        TEST_ASSERT_EQUAL(0, counters[cpu][1]);
    }

    /* swap priorities! */
    ਲਈ (ਪੂਰਨ cpu = 0; cpu < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; cpu++) {
        v_Task_ਤਰਜੀਹ_ਸੈੱਟ(tasks[cpu][0], UNITY_FREERTOS_ਤਰਜੀਹ - 1);
        v_Task_ਤਰਜੀਹ_ਸੈੱਟ(tasks[cpu][1], UNITY_FREERTOS_ਤਰਜੀਹ);
    }

    /* check priorities have swapped... */
    ਲਈ (ਪੂਰਨ cpu = 0; cpu < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; cpu++) {
        TEST_ASSERT_EQUAL(UNITY_FREERTOS_ਤਰਜੀਹ - 1, ux_Task_ਤਰਜੀਹ_Get(tasks[cpu][0]));
        TEST_ASSERT_EQUAL(UNITY_FREERTOS_ਤਰਜੀਹ, ux_Task_ਤਰਜੀਹ_Get(tasks[cpu][1]));
    }

    /* check the tasks which are counting have also swapped now... */
    ਲਈ (ਪੂਰਨ cpu = 0; cpu < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; cpu++) {
        ਬਿਨਾਨਿਸ਼ਾਨੀ old_counters[2];
        old_counters[0] = counters[cpu][0];
        old_counters[1] = counters[cpu][1];
        vTaskDelay(10);
        TEST_ASSERT_EQUAL(old_counters[0], counters[cpu][0]);
        TEST_ASSERT_NOT_EQUAL(old_counters[1], counters[cpu][1]);
    }

    /* clean up */
    ਲਈ (ਪੂਰਨ cpu = 0; cpu < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; cpu++) {
        ਲਈ (ਪੂਰਨ task = 0; task < 2; task++) {
            vTaskDelete(tasks[cpu][task]);
        }
    }
}
