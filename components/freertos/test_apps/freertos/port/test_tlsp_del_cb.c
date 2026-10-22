/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ଅନ୍ତର୍ଭାଵ "sdkconfig.h"

#ଅନ୍ତର୍ଭାଵ "freertos/FreeRTOS.h"
#ଅନ୍ତର୍ଭାଵ "freertos/task.h"
#ଅନ୍ତର୍ଭାଵ "unity.h"
#ଅନ୍ତର୍ଭାଵ "test_utils.h"

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

#define ଅଙ୍କ_TLSP        CONFIG_FREERTOS_THREAD_LOCAL_STORAGE_POINTERS

ସ୍ଥିର ଶୂନ୍ୟ tlsp_del_cb(ପୂର୍ଣ୍ଣସଂଖ୍ୟା index, ଶୂନ୍ୟ *tlsp)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା *val = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା *)tlsp;

    // Check that the TLSP's initialization value is correct
    TEST_ASSERT_EQUAL(index, *val);
    // Set the TLSP's value again to a negative value to indicate that the del cb has ran
    *val = -*val;
}

ସ୍ଥିର ଶୂନ୍ୟ tlsp_task(ଶୂନ୍ୟ *arg)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା *tlsps = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା *)arg;

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = 0; index < ଅଙ୍କ_TLSP; index++) {
        // Initialize the TLSPs to a positive value
        tlsps[index] = index;
        // Set TLSPs and deletion callbacks
        vTaskSetThreadLocalStoragePointerAndDelCallback(ଖାଲି, index, &tlsps[index], tlsp_del_cb);
    }

    // Self delete to trigger TLSP del cb
    vTaskDelete(ଖାଲି);
}

TEST_CASE("Test TLSP deletion callbacks", "[freertos]")
{
    TaskHandle_t tasks[CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES];
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା tlsps[CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES][ଅଙ୍କ_TLSP];

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES; i++) {
        TEST_ASSERT_EQUAL(pdPASS, xTaskCreatePinnedToCore(tlsp_task, "tlsp_tsk", config_MINIMAL_ଗଦା_SIZE * 2, (ଶୂନ୍ୟ *)&tlsps[i], UNITY_FREERTOS_PRIORITY - 1, &tasks[i], i));
    }
    // Significant delay to let tasks run and delete themselves
    vTaskDelay(100);

    // Check the values of the TLSPs to see if the del cb have ran
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES; i++) {
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = 0; index < ଅଙ୍କ_TLSP; index++) {
            // Del cb should have set the TLSP to a negative value
            TEST_ASSERT_EQUAL(-index, tlsps[i][index]);
        }
    }
}
