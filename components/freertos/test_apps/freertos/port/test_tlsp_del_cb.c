/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن "sdkconfig.h"

#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن "unity.h"
#ضمن "test_utils.h"

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

#define رقم_TLSP        CONFIG_FREERTOS_THREAD_LOCAL_STORAGE_POINTERS

ساكن فراغ tlsp_del_cb(صحيح index, فراغ *tlsp)
{
    صحيح *val = (صحيح *)tlsp;

    // Check that the TLSP's initialization value is correct
    TEST_ASSERT_EQUAL(index, *val);
    // Set the TLSP's value again to a negative value to indicate that the del cb has ran
    *val = -*val;
}

ساكن فراغ tlsp_task(فراغ *arg)
{
    صحيح *tlsps = (صحيح *)arg;

    لكل (صحيح index = 0; index < رقم_TLSP; index++) {
        // Initialize the TLSPs to a positive value
        tlsps[index] = index;
        // Set TLSPs and deletion callbacks
        vTaskSetThreadLocalStoragePointerAndDelCallback(عدم, index, &tlsps[index], tlsp_del_cb);
    }

    // Self delete to trigger TLSP del cb
    vTaskDelete(عدم);
}

TEST_CASE("Test TLSP deletion callbacks", "[freertos]")
{
    TaskHandle_t tasks[CONFIG_FREERTOS_رقم_OF_CORES];
    صحيح tlsps[CONFIG_FREERTOS_رقم_OF_CORES][رقم_TLSP];

    لكل (صحيح i = 0; i < CONFIG_FREERTOS_رقم_OF_CORES; i++) {
        TEST_ASSERT_EQUAL(pdPASS, xTaskCreatePinnedToCore(tlsp_task, "tlsp_tsk", config_MINIMAL_مكدس_SIZE * 2, (فراغ *)&tlsps[i], UNITY_FREERTOS_PRIORITY - 1, &tasks[i], i));
    }
    // Significant delay to let tasks run and delete themselves
    vTaskDelay(100);

    // Check the values of the TLSPs to see if the del cb have ran
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_رقم_OF_CORES; i++) {
        لكل (صحيح index = 0; index < رقم_TLSP; index++) {
            // Del cb should have set the TLSP to a negative value
            TEST_ASSERT_EQUAL(-index, tlsps[i][index]);
        }
    }
}
