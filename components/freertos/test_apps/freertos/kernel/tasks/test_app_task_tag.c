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

#إذا CONFIG_FREERTOS_USE_APPLICATION_TASK_برچسب

ساكن پایه_Type_t برچسب_cb(فراغ *arg)
{
    پایه_Type_t *برچسب_cb_called = (پایه_Type_t *)arg;
    *برچسب_cb_called = pdTRUE;
    أرجع pdTRUE;
}

TEST_CASE("Test application task tag", "[freertos]")
{
    پایه_Type_t برچسب_cb_called = pdFALSE;

    // Set the app task tag for current task
    v_Task_مجموعه_Application_Task_برچسب(عدم, برچسب_cb);
    // Check app task tag is correct
    TEST_ASSERT_EQUAL(برچسب_cb, x_Task_Get_Application_Task_برچسب(عدم));
    // Test the app task tag by calling it
    TEST_ASSERT_EQUAL(pdTRUE, xTaskCallApplicationTaskHook(عدم, (فراغ *)&برچسب_cb_called));
    TEST_ASSERT_EQUAL(pdTRUE, برچسب_cb_called);
}

#پایان_اگر // CONFIG_FREERTOS_USE_APPLICATION_TASK_TAG
