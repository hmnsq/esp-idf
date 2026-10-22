/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#অন্তর্ভাব "sdkconfig.h"

#অন্তর্ভাব "freertos/FreeRTOS.h"
#অন্তর্ভাব "freertos/task.h"
#অন্তর্ভাব "unity.h"
#অন্তর্ভাব "test_utils.h"

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

স্থির শূন্য tlsp_del_cb(পূর্ণসংখ্যা সূচক, শূন্য *tlsp)
{
    পূর্ণসংখ্যা *val = (পূর্ণসংখ্যা *)tlsp;

    // Check that the TLSP's initialization value is correct
    TEST_ASSERT_EQUAL(সূচক, *val);
    // Set the TLSP's value again to a negative value to indicate that the del cb has ran
    *val = -*val;
}

স্থির শূন্য tlsp_task(শূন্য *arg)
{
    পূর্ণসংখ্যা *tlsps = (পূর্ণসংখ্যা *)arg;

    জন্য (পূর্ণসংখ্যা সূচক = 0; সূচক < অঙ্ক_TLSP; সূচক++) {
        // Initialize the TLSPs to a positive value
        tlsps[সূচক] = সূচক;
        // Set TLSPs and deletion callbacks
        vTaskSetThreadLocalStoragePointerAndDelCallback(ফাঁকা, সূচক, &tlsps[সূচক], tlsp_del_cb);
    }

    // Self delete to trigger TLSP del cb
    vTaskDelete(ফাঁকা);
}

TEST_CASE("Test TLSP deletion callbacks", "[freertos]")
{
    TaskHandle_t tasks[CONFIG_FREERTOS_সংখ্যা_OF_CORES];
    পূর্ণসংখ্যা tlsps[CONFIG_FREERTOS_সংখ্যা_OF_CORES][অঙ্ক_TLSP];

    জন্য (পূর্ণসংখ্যা i = 0; i < CONFIG_FREERTOS_সংখ্যা_OF_CORES; i++) {
        TEST_ASSERT_EQUAL(pdPASS, xTaskCreatePinnedToCore(tlsp_task, "tlsp_tsk", config_MINIMAL_স্তূপ_আকার * 2, (শূন্য *)&tlsps[i], UNITY_FREERTOS_PRIORITY - 1, &tasks[i], i));
    }
    // Significant delay to let tasks run and delete themselves
    vTaskDelay(100);

    // Check the values of the TLSPs to see if the del cb have ran
    জন্য (পূর্ণসংখ্যা i = 0; i < CONFIG_FREERTOS_সংখ্যা_OF_CORES; i++) {
        জন্য (পূর্ণসংখ্যা সূচক = 0; সূচক < অঙ্ক_TLSP; সূচক++) {
            // Del cb should have set the TLSP to a negative value
            TEST_ASSERT_EQUAL(-সূচক, tlsps[i][সূচক]);
        }
    }
}
