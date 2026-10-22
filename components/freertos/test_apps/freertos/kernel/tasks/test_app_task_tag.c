/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#شامل "sdkconfig.h"
#شامل "freertos/FreeRTOS.h"
#شامل "freertos/task.h"
#شامل "unity.h"
#شامل "test_utils.h"

#اگر CONFIG_FREERTOS_USE_APPLICATION_TASK_لیبل

ساکن بنیاد_Type_t لیبل_cb(خالی *arg)
{
    بنیاد_Type_t *لیبل_cb_called = (بنیاد_Type_t *)arg;
    *لیبل_cb_called = pdTRUE;
    واپس pdTRUE;
}

TEST_CASE("Test application task tag", "[freertos]")
{
    بنیاد_Type_t لیبل_cb_called = pdFALSE;

    // Set the app task tag for current task
    v_Task_سیٹ_Application_Task_لیبل(تہی, لیبل_cb);
    // Check app task tag is correct
    TEST_ASSERT_EQUAL(لیبل_cb, x_Task_Get_Application_Task_لیبل(تہی));
    // Test the app task tag by calling it
    TEST_ASSERT_EQUAL(pdTRUE, xTaskCallApplicationTaskHook(تہی, (خالی *)&لیبل_cb_called));
    TEST_ASSERT_EQUAL(pdTRUE, لیبل_cb_called);
}

#اگر_ختم // CONFIG_FREERTOS_USE_APPLICATION_TASK_TAG
