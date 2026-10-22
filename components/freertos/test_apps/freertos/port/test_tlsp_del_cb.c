/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#അന്തര്ഭാവ "sdkconfig.h"

#അന്തര്ഭാവ "freertos/FreeRTOS.h"
#അന്തര്ഭാവ "freertos/task.h"
#അന്തര്ഭാവ "unity.h"
#അന്തര്ഭാവ "test_utils.h"

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

#define അക്കം_TLSP        CONFIG_FREERTOS_THREAD_LOCAL_STORAGE_POINTERS

സ്ഥിതം ശൂന്യം tlsp_del_cb(പൂർണ്ണസംഖ്യ index, ശൂന്യം *tlsp)
{
    പൂർണ്ണസംഖ്യ *val = (പൂർണ്ണസംഖ്യ *)tlsp;

    // Check that the TLSP's initialization value is correct
    TEST_ASSERT_EQUAL(index, *val);
    // Set the TLSP's value again to a negative value to indicate that the del cb has ran
    *val = -*val;
}

സ്ഥിതം ശൂന്യം tlsp_task(ശൂന്യം *arg)
{
    പൂർണ്ണസംഖ്യ *tlsps = (പൂർണ്ണസംഖ്യ *)arg;

    വേണ്ടി (പൂർണ്ണസംഖ്യ index = 0; index < അക്കം_TLSP; index++) {
        // Initialize the TLSPs to a positive value
        tlsps[index] = index;
        // Set TLSPs and deletion callbacks
        vTaskSetThreadLocalStoragePointerAndDelCallback(ശൂന്യ, index, &tlsps[index], tlsp_del_cb);
    }

    // Self delete to trigger TLSP del cb
    vTaskDelete(ശൂന്യ);
}

TEST_CASE("Test TLSP deletion callbacks", "[freertos]")
{
    TaskHandle_t tasks[CONFIG_FREERTOS_സംഖ്യ_OF_CORES];
    പൂർണ്ണസംഖ്യ tlsps[CONFIG_FREERTOS_സംഖ്യ_OF_CORES][അക്കം_TLSP];

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; i++) {
        TEST_ASSERT_EQUAL(pdPASS, xTaskCreatePinnedToCore(tlsp_task, "tlsp_tsk", config_MINIMAL_അട്ടി_SIZE * 2, (ശൂന്യം *)&tlsps[i], UNITY_FREERTOS_PRIORITY - 1, &tasks[i], i));
    }
    // Significant delay to let tasks run and delete themselves
    vTaskDelay(100);

    // Check the values of the TLSPs to see if the del cb have ran
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; i++) {
        വേണ്ടി (പൂർണ്ണസംഖ്യ index = 0; index < അക്കം_TLSP; index++) {
            // Del cb should have set the TLSP to a negative value
            TEST_ASSERT_EQUAL(-index, tlsps[i][index]);
        }
    }
}
