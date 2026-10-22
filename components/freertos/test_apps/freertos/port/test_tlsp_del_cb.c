/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"

#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ "unity.h"
#ಅನ್ತರ್ಭಾವ "test_utils.h"

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

#ವ್ಯಾಖ್ಯಾನಿಸು ಅಂಕೆ_TLSP        CONFIG_FREERTOS_THREAD_LOCAL_STORAGE_ಸೂಚಕಗಳು

ಸ್ಥಿರ ಶೂನ್ಯ tlsp_del_cb(ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ, ಶೂನ್ಯ *tlsp)
{
    ಪೂರ್ಣಾಂಕ *val = (ಪೂರ್ಣಾಂಕ *)tlsp;

    // Check that the TLSP's initialization value is correct
    TEST_ASSERT_EQUAL(ಸೂಚ್ಯಂಕ, *val);
    // Set the TLSP's value again to a negative value to indicate that the del cb has ran
    *val = -*val;
}

ಸ್ಥಿರ ಶೂನ್ಯ tlsp_task(ಶೂನ್ಯ *arg)
{
    ಪೂರ್ಣಾಂಕ *tlsps = (ಪೂರ್ಣಾಂಕ *)arg;

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = 0; ಸೂಚ್ಯಂಕ < ಅಂಕೆ_TLSP; ಸೂಚ್ಯಂಕ++) {
        // Initialize the TLSPs to a positive value
        tlsps[ಸೂಚ್ಯಂಕ] = ಸೂಚ್ಯಂಕ;
        // Set TLSPs and deletion callbacks
        v_Task_ಗಣ_Thread_Local_Storage_ಸೂಚಕ_And_Del_Callback(ಖಾಲಿ, ಸೂಚ್ಯಂಕ, &tlsps[ಸೂಚ್ಯಂಕ], tlsp_del_cb);
    }

    // Self delete to trigger TLSP del cb
    vTaskDelete(ಖಾಲಿ);
}

TEST_CASE("Test TLSP deletion callbacks", "[freertos]")
{
    TaskHandle_t tasks[CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES];
    ಪೂರ್ಣಾಂಕ tlsps[CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES][ಅಂಕೆ_TLSP];

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; i++) {
        TEST_ASSERT_EQUAL(pdPASS, x_Task_Create_Pinned_ಗಮ್ಯ_Core(tlsp_task, "tlsp_tsk", config_ಕನಿಷ್ಠತಮ_ರಾಶಿ_ಗಾತ್ರ * 2, (ಶೂನ್ಯ *)&tlsps[i], UNITY_FREERTOS_ಆದ್ಯತೆ - 1, &tasks[i], i));
    }
    // Significant delay to let tasks run and delete themselves
    vTaskDelay(100);

    // Check the values of the TLSPs to see if the del cb have ran
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; i++) {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = 0; ಸೂಚ್ಯಂಕ < ಅಂಕೆ_TLSP; ಸೂಚ್ಯಂಕ++) {
            // Del cb should have set the TLSP to a negative value
            TEST_ASSERT_EQUAL(-ಸೂಚ್ಯಂಕ, tlsps[i][ಸೂಚ್ಯಂಕ]);
        }
    }
}
