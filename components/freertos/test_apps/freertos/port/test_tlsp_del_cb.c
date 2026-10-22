/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#شامل "sdkconfig.h"

#شامل "freertos/FreeRTOS.h"
#شامل "freertos/task.h"
#شامل "unity.h"
#شامل "test_utils.h"

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

#تعریف_کرو عدد_TLSP        CONFIG_FREERTOS_THREAD_LOCAL_STORAGE_اشارے

ساکن خالی tlsp_del_cb(صحیح اشاریہ, خالی *tlsp)
{
    صحیح *val = (صحیح *)tlsp;

    // Check that the TLSP's initialization value is correct
    TEST_ASSERT_EQUAL(اشاریہ, *val);
    // Set the TLSP's value again to a negative value to indicate that the del cb has ran
    *val = -*val;
}

ساکن خالی tlsp_task(خالی *arg)
{
    صحیح *tlsps = (صحیح *)arg;

    برائے (صحیح اشاریہ = 0; اشاریہ < عدد_TLSP; اشاریہ++) {
        // Initialize the TLSPs to a positive value
        tlsps[اشاریہ] = اشاریہ;
        // Set TLSPs and deletion callbacks
        v_Task_سیٹ_Thread_Local_Storage_اشارہ_گر_And_Del_Callback(تہی, اشاریہ, &tlsps[اشاریہ], tlsp_del_cb);
    }

    // Self delete to trigger TLSP del cb
    vTaskDelete(تہی);
}

TEST_CASE("Test TLSP deletion callbacks", "[freertos]")
{
    TaskHandle_t tasks[CONFIG_FREERTOS_نمبر_کا_CORES];
    صحیح tlsps[CONFIG_FREERTOS_نمبر_کا_CORES][عدد_TLSP];

    برائے (صحیح i = 0; i < CONFIG_FREERTOS_نمبر_کا_CORES; i++) {
        TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_مقصد_Core(tlsp_task, "tlsp_tsk", config_اقل_ڈھیر_سائز * 2, (خالی *)&tlsps[i], UNITY_FREERTOS_ترجیح - 1, &tasks[i], i));
    }
    // Significant delay to let tasks run and delete themselves
    vTaskDelay(100);

    // Check the values of the TLSPs to see if the del cb have ran
    برائے (صحیح i = 0; i < CONFIG_FREERTOS_نمبر_کا_CORES; i++) {
        برائے (صحیح اشاریہ = 0; اشاریہ < عدد_TLSP; اشاریہ++) {
            // Del cb should have set the TLSP to a negative value
            TEST_ASSERT_EQUAL(-اشاریہ, tlsps[i][اشاریہ]);
        }
    }
}
