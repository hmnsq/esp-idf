/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#समावेश "sdkconfig.h"

#समावेश "freertos/FreeRTOS.h"
#समावेश "freertos/task.h"
#समावेश "unity.h"
#समावेश "test_utils.h"

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

#घोषणा अंक_TLSP        CONFIG_FREERTOS_THREAD_LOCAL_STORAGE_POINTERS

अटल व्योम tlsp_del_cb(पूर्णांक सूचकांक, व्योम *tlsp)
{
    पूर्णांक *val = (पूर्णांक *)tlsp;

    // Check that the TLSP's initialization value is correct
    TEST_ASSERT_EQUAL(सूचकांक, *val);
    // Set the TLSP's value again to a negative value to indicate that the del cb has ran
    *val = -*val;
}

अटल व्योम tlsp_task(व्योम *arg)
{
    पूर्णांक *tlsps = (पूर्णांक *)arg;

    क्रम (पूर्णांक सूचकांक = 0; सूचकांक < अंक_TLSP; सूचकांक++) {
        // Initialize the TLSPs to a positive value
        tlsps[सूचकांक] = सूचकांक;
        // Set TLSPs and deletion callbacks
        vTaskSetThreadLocalStoragePointerAndDelCallback(शून्य, सूचकांक, &tlsps[सूचकांक], tlsp_del_cb);
    }

    // Self delete to trigger TLSP del cb
    vTaskDelete(शून्य);
}

TEST_CASE("Test TLSP deletion callbacks", "[freertos]")
{
    TaskHandle_t tasks[CONFIG_FREERTOS_संख्या_OF_CORES];
    पूर्णांक tlsps[CONFIG_FREERTOS_संख्या_OF_CORES][अंक_TLSP];

    क्रम (पूर्णांक i = 0; i < CONFIG_FREERTOS_संख्या_OF_CORES; i++) {
        TEST_ASSERT_EQUAL(pdPASS, xTaskCreatePinnedToCore(tlsp_task, "tlsp_tsk", config_MINIMAL_ढेर_आकार * 2, (व्योम *)&tlsps[i], UNITY_FREERTOS_PRIORITY - 1, &tasks[i], i));
    }
    // Significant delay to let tasks run and delete themselves
    vTaskDelay(100);

    // Check the values of the TLSPs to see if the del cb have ran
    क्रम (पूर्णांक i = 0; i < CONFIG_FREERTOS_संख्या_OF_CORES; i++) {
        क्रम (पूर्णांक सूचकांक = 0; सूचकांक < अंक_TLSP; सूचकांक++) {
            // Del cb should have set the TLSP to a negative value
            TEST_ASSERT_EQUAL(-सूचकांक, tlsps[i][सूचकांक]);
        }
    }
}
