/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unity.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "test_utils.h"

#ਜੇ CONFIG_FREERTOS_USE_APPLICATION_TASK_ਟੈਗ

ਸਥਿਰ ਅਧਾਰ_Type_t ਟੈਗ_cb(ਖਾਲੀ *arg)
{
    ਅਧਾਰ_Type_t *ਟੈਗ_cb_called = (ਅਧਾਰ_Type_t *)arg;
    *ਟੈਗ_cb_called = pdTRUE;
    ਵਾਪਸ pdTRUE;
}

TEST_CASE("Test application task tag", "[freertos]")
{
    ਅਧਾਰ_Type_t ਟੈਗ_cb_called = pdFALSE;

    // Set the app task tag for current task
    v_Task_ਸੈੱਟ_Application_Task_ਟੈਗ(ਨਿਰਾ, ਟੈਗ_cb);
    // Check app task tag is correct
    TEST_ASSERT_EQUAL(ਟੈਗ_cb, x_Task_Get_Application_Task_ਟੈਗ(ਨਿਰਾ));
    // Test the app task tag by calling it
    TEST_ASSERT_EQUAL(pdTRUE, xTaskCallApplicationTaskHook(ਨਿਰਾ, (ਖਾਲੀ *)&ਟੈਗ_cb_called));
    TEST_ASSERT_EQUAL(pdTRUE, ਟੈਗ_cb_called);
}

#ਜੇ_ਅੰਤ // CONFIG_FREERTOS_USE_APPLICATION_TASK_TAG
