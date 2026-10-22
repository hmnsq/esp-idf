/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#અન્તર્ભાવ "sdkconfig.h"

#અન્તર્ભાવ "freertos/FreeRTOS.h"
#અન્તર્ભાવ "freertos/task.h"
#અન્તર્ભાવ "unity.h"
#અન્તર્ભાવ "test_utils.h"

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

#define અંક_TLSP        CONFIG_FREERTOS_THREAD_LOCAL_STORAGE_POINTERS

સ્થિર શૂન્ય tlsp_del_cb(પૂર્ણાંક index, શૂન્ય *tlsp)
{
    પૂર્ણાંક *val = (પૂર્ણાંક *)tlsp;

    // Check that the TLSP's initialization value is correct
    TEST_ASSERT_EQUAL(index, *val);
    // Set the TLSP's value again to a negative value to indicate that the del cb has ran
    *val = -*val;
}

સ્થિર શૂન્ય tlsp_task(શૂન્ય *arg)
{
    પૂર્ણાંક *tlsps = (પૂર્ણાંક *)arg;

    માટે (પૂર્ણાંક index = 0; index < અંક_TLSP; index++) {
        // Initialize the TLSPs to a positive value
        tlsps[index] = index;
        // Set TLSPs and deletion callbacks
        vTaskSetThreadLocalStoragePointerAndDelCallback(ખાલી, index, &tlsps[index], tlsp_del_cb);
    }

    // Self delete to trigger TLSP del cb
    vTaskDelete(ખાલી);
}

TEST_CASE("Test TLSP deletion callbacks", "[freertos]")
{
    TaskHandle_t tasks[CONFIG_FREERTOS_સંખ્યા_OF_CORES];
    પૂર્ણાંક tlsps[CONFIG_FREERTOS_સંખ્યા_OF_CORES][અંક_TLSP];

    માટે (પૂર્ણાંક i = 0; i < CONFIG_FREERTOS_સંખ્યા_OF_CORES; i++) {
        TEST_ASSERT_EQUAL(pdPASS, xTaskCreatePinnedToCore(tlsp_task, "tlsp_tsk", config_MINIMAL_ઢગલો_SIZE * 2, (શૂન્ય *)&tlsps[i], UNITY_FREERTOS_PRIORITY - 1, &tasks[i], i));
    }
    // Significant delay to let tasks run and delete themselves
    vTaskDelay(100);

    // Check the values of the TLSPs to see if the del cb have ran
    માટે (પૂર્ણાંક i = 0; i < CONFIG_FREERTOS_સંખ્યા_OF_CORES; i++) {
        માટે (પૂર્ણાંક index = 0; index < અંક_TLSP; index++) {
            // Del cb should have set the TLSP to a negative value
            TEST_ASSERT_EQUAL(-index, tlsps[i][index]);
        }
    }
}
