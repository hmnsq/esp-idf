/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#అన్తర్భావ "sdkconfig.h"

#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ "unity.h"
#అన్తర్భావ "test_utils.h"

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

#నిర్వచించు అంకె_TLSP        CONFIG_FREERTOS_THREAD_LOCAL_STORAGE_సూచికలు

స్థిర శూన్య tlsp_del_cb(పూర్ణాంకం సూచీ, శూన్య *tlsp)
{
    పూర్ణాంకం *val = (పూర్ణాంకం *)tlsp;

    // Check that the TLSP's initialization value is correct
    TEST_ASSERT_EQUAL(సూచీ, *val);
    // Set the TLSP's value again to a negative value to indicate that the del cb has ran
    *val = -*val;
}

స్థిర శూన్య tlsp_task(శూన్య *arg)
{
    పూర్ణాంకం *tlsps = (పూర్ణాంకం *)arg;

    కొరకు (పూర్ణాంకం సూచీ = 0; సూచీ < అంకె_TLSP; సూచీ++) {
        // Initialize the TLSPs to a positive value
        tlsps[సూచీ] = సూచీ;
        // Set TLSPs and deletion callbacks
        v_Task_సమితి_Thread_Local_Storage_సూచిక_And_Del_Callback(ఖాళీ, సూచీ, &tlsps[సూచీ], tlsp_del_cb);
    }

    // Self delete to trigger TLSP del cb
    vTaskDelete(ఖాళీ);
}

TEST_CASE("Test TLSP deletion callbacks", "[freertos]")
{
    TaskHandle_t tasks[CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES];
    పూర్ణాంకం tlsps[CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES][అంకె_TLSP];

    కొరకు (పూర్ణాంకం i = 0; i < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; i++) {
        TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_గమ్యం_Core(tlsp_task, "tlsp_tsk", config_కనీస_దొంతర_కొలత * 2, (శూన్య *)&tlsps[i], UNITY_FREERTOS_ప్రాధాన్యత - 1, &tasks[i], i));
    }
    // Significant delay to let tasks run and delete themselves
    vTaskDelay(100);

    // Check the values of the TLSPs to see if the del cb have ran
    కొరకు (పూర్ణాంకం i = 0; i < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; i++) {
        కొరకు (పూర్ణాంకం సూచీ = 0; సూచీ < అంకె_TLSP; సూచీ++) {
            // Del cb should have set the TLSP to a negative value
            TEST_ASSERT_EQUAL(-సూచీ, tlsps[i][సూచీ]);
        }
    }
}
