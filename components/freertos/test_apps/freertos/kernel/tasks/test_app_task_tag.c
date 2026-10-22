/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن "sdkconfig.h"
#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن "unity.h"
#ضمن "test_utils.h"

#إذا CONFIG_FREERTOS_USE_APPLICATION_TASK_ٹیگ

ساكن ادھار_Type_t ٹیگ_cb(فراغ *arg)
{
    ادھار_Type_t *ٹیگ_cb_called = (ادھار_Type_t *)arg;
    *ٹیگ_cb_called = pdTRUE;
    أرجع pdTRUE;
}

TEST_CASE("Test application task tag", "[freertos]")
{
    ادھار_Type_t ٹیگ_cb_called = pdFALSE;

    // Set the app task tag for current task
    v_Task_سیٹ_Application_Task_ٹیگ(عدم, ٹیگ_cb);
    // Check app task tag is correct
    TEST_ASSERT_EQUAL(ٹیگ_cb, x_Task_Get_Application_Task_ٹیگ(عدم));
    // Test the app task tag by calling it
    TEST_ASSERT_EQUAL(pdTRUE, xTaskCallApplicationTaskHook(عدم, (فراغ *)&ٹیگ_cb_called));
    TEST_ASSERT_EQUAL(pdTRUE, ٹیگ_cb_called);
}

#جے_انت // CONFIG_FREERTOS_USE_APPLICATION_TASK_TAG
