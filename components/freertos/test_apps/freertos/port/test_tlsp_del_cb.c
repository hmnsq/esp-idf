/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#অন্তর্ভাৱ "sdkconfig.h"

#অন্তর্ভাৱ "freertos/FreeRTOS.h"
#অন্তর্ভাৱ "freertos/task.h"
#অন্তর্ভাৱ "unity.h"
#অন্তর্ভাৱ "test_utils.h"

/*
Test TLSP Deletion Callbacks

Purpose:
    - Test that TLSP Deletion Callbacks can be registered
    - Test that TLSP Deletion Callbacks are called when a task is deleted

Procedure:
    - Create a task on each core along with an array of integers to act as TLSP data
    - Each task should initialize their integers to a particular value (i.e., the index value)
    - Each task should register those integers as TLSPs along with a deletion callback
    - Each task should self delete to trigger the TLSP deletion callback
    - The TLSP deletion callback should indicate that it has run by negating the integer values

Expected:
    - The TLSP deletion callback should check that the correct TLSP is provided by checking the TLSPs initialization
    value (i.e., should be set to the index value)
    - After deletion, the integer values should be negated to indicate deletion callback execution
*/

#define অঙ্ক_TLSP        CONFIG_FREERTOS_THREAD_LOCAL_STORAGE_POINTERS

স্থির শূন্য tlsp_del_cb(পূর্ণাঙ্ক index, শূন্য *tlsp)
{
    পূর্ণাঙ্ক *val = (পূর্ণাঙ্ক *)tlsp;

    // Check that the TLSP's initialization value is correct
    TEST_ASSERT_EQUAL(index, *val);
    // Set the TLSP's value again to a negative value to indicate that the del cb has ran
    *val = -*val;
}

স্থির শূন্য tlsp_task(শূন্য *arg)
{
    পূর্ণাঙ্ক *tlsps = (পূর্ণাঙ্ক *)arg;

    বাবে (পূর্ণাঙ্ক index = 0; index < অঙ্ক_TLSP; index++) {
        // Initialize the TLSPs to a positive value
        tlsps[index] = index;
        // Set TLSPs and deletion callbacks
        vTaskSetThreadLocalStoragePointerAndDelCallback(রিক্ত, index, &tlsps[index], tlsp_del_cb);
    }

    // Self delete to trigger TLSP del cb
    vTaskDelete(রিক্ত);
}

TEST_CASE("Test TLSP deletion callbacks", "[freertos]")
{
    TaskHandle_t tasks[CONFIG_FREERTOS_সংখ্যা_OF_CORES];
    পূর্ণাঙ্ক tlsps[CONFIG_FREERTOS_সংখ্যা_OF_CORES][অঙ্ক_TLSP];

    বাবে (পূর্ণাঙ্ক i = 0; i < CONFIG_FREERTOS_সংখ্যা_OF_CORES; i++) {
        TEST_ASSERT_EQUAL(pdPASS, xTaskCreatePinnedToCore(tlsp_task, "tlsp_tsk", config_MINIMAL_স্তূপ_SIZE * 2, (শূন্য *)&tlsps[i], UNITY_FREERTOS_PRIORITY - 1, &tasks[i], i));
    }
    // Significant delay to let tasks run and delete themselves
    vTaskDelay(100);

    // Check the values of the TLSPs to see if the del cb have ran
    বাবে (পূর্ণাঙ্ক i = 0; i < CONFIG_FREERTOS_সংখ্যা_OF_CORES; i++) {
        বাবে (পূর্ণাঙ্ক index = 0; index < অঙ্ক_TLSP; index++) {
            // Del cb should have set the TLSP to a negative value
            TEST_ASSERT_EQUAL(-index, tlsps[i][index]);
        }
    }
}
