/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unity.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "test_utils.h"

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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਅੰਕ_TLSP        CONFIG_FREERTOS_THREAD_LOCAL_STORAGE_ਸੂਚਕ_ਸਾਰੇ

ਸਥਿਰ ਖਾਲੀ tlsp_del_cb(ਪੂਰਨ ਸੂਚਕਾਂਕ, ਖਾਲੀ *tlsp)
{
    ਪੂਰਨ *val = (ਪੂਰਨ *)tlsp;

    // Check that the TLSP's initialization value is correct
    TEST_ASSERT_EQUAL(ਸੂਚਕਾਂਕ, *val);
    // Set the TLSP's value again to a negative value to indicate that the del cb has ran
    *val = -*val;
}

ਸਥਿਰ ਖਾਲੀ tlsp_task(ਖਾਲੀ *arg)
{
    ਪੂਰਨ *tlsps = (ਪੂਰਨ *)arg;

    ਲਈ (ਪੂਰਨ ਸੂਚਕਾਂਕ = 0; ਸੂਚਕਾਂਕ < ਅੰਕ_TLSP; ਸੂਚਕਾਂਕ++) {
        // Initialize the TLSPs to a positive value
        tlsps[ਸੂਚਕਾਂਕ] = ਸੂਚਕਾਂਕ;
        // Set TLSPs and deletion callbacks
        v_Task_ਸੈੱਟ_Thread_Local_Storage_ਸੂਚਕ_And_Del_Callback(ਨਿਰਾ, ਸੂਚਕਾਂਕ, &tlsps[ਸੂਚਕਾਂਕ], tlsp_del_cb);
    }

    // Self delete to trigger TLSP del cb
    vTaskDelete(ਨਿਰਾ);
}

TEST_CASE("Test TLSP deletion callbacks", "[freertos]")
{
    TaskHandle_t tasks[CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES];
    ਪੂਰਨ tlsps[CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES][ਅੰਕ_TLSP];

    ਲਈ (ਪੂਰਨ i = 0; i < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; i++) {
        TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(tlsp_task, "tlsp_tsk", config_ਨਿਊਨਤਮ_ਢੇਰ_ਮਾਪ * 2, (ਖਾਲੀ *)&tlsps[i], UNITY_FREERTOS_ਤਰਜੀਹ - 1, &tasks[i], i));
    }
    // Significant delay to let tasks run and delete themselves
    vTaskDelay(100);

    // Check the values of the TLSPs to see if the del cb have ran
    ਲਈ (ਪੂਰਨ i = 0; i < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; i++) {
        ਲਈ (ਪੂਰਨ ਸੂਚਕਾਂਕ = 0; ਸੂਚਕਾਂਕ < ਅੰਕ_TLSP; ਸੂਚਕਾਂਕ++) {
            // Del cb should have set the TLSP to a negative value
            TEST_ASSERT_EQUAL(-ਸੂਚਕਾਂਕ, tlsps[i][ਸੂਚਕਾਂਕ]);
        }
    }
}
