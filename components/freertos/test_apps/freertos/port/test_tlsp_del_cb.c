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

#وصف_ڪريو انگ_TLSP        CONFIG_FREERTOS_THREAD_LOCAL_STORAGE_اشارا

ساكن فراغ tlsp_del_cb(صحيح اشاريو, فراغ *tlsp)
{
    صحيح *val = (صحيح *)tlsp;

    // Check that the TLSP's initialization value is correct
    TEST_ASSERT_EQUAL(اشاريو, *val);
    // Set the TLSP's value again to a negative value to indicate that the del cb has ran
    *val = -*val;
}

ساكن فراغ tlsp_task(فراغ *arg)
{
    صحيح *tlsps = (صحيح *)arg;

    لكل (صحيح اشاريو = 0; اشاريو < انگ_TLSP; اشاريو++) {
        // Initialize the TLSPs to a positive value
        tlsps[اشاريو] = اشاريو;
        // Set TLSPs and deletion callbacks
        v_Task_سيٽ_Thread_Local_Storage_اشارو_And_Del_Callback(عدم, اشاريو, &tlsps[اشاريو], tlsp_del_cb);
    }

    // Self delete to trigger TLSP del cb
    vTaskDelete(عدم);
}

TEST_CASE("Test TLSP deletion callbacks", "[freertos]")
{
    TaskHandle_t tasks[CONFIG_FREERTOS_عدد_جو_CORES];
    صحيح tlsps[CONFIG_FREERTOS_عدد_جو_CORES][انگ_TLSP];

    لكل (صحيح i = 0; i < CONFIG_FREERTOS_عدد_جو_CORES; i++) {
        TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_منزل_Core(tlsp_task, "tlsp_tsk", config_گهٽتر_ڍير_سائيز * 2, (فراغ *)&tlsps[i], UNITY_FREERTOS_ترجيح - 1, &tasks[i], i));
    }
    // Significant delay to let tasks run and delete themselves
    vTaskDelay(100);

    // Check the values of the TLSPs to see if the del cb have ran
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_عدد_جو_CORES; i++) {
        لكل (صحيح اشاريو = 0; اشاريو < انگ_TLSP; اشاريو++) {
            // Del cb should have set the TLSP to a negative value
            TEST_ASSERT_EQUAL(-اشاريو, tlsps[i][اشاريو]);
        }
    }
}
