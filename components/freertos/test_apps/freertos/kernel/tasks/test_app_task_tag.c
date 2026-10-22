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

#إذا CONFIG_FREERTOS_USE_APPLICATION_TASK_ٽيگ

ساكن بنياد_Type_t ٽيگ_cb(فراغ *arg)
{
    بنياد_Type_t *ٽيگ_cb_called = (بنياد_Type_t *)arg;
    *ٽيگ_cb_called = pdTRUE;
    أرجع pdTRUE;
}

TEST_CASE("Test application task tag", "[freertos]")
{
    بنياد_Type_t ٽيگ_cb_called = pdFALSE;

    // Set the app task tag for current task
    v_Task_سيٽ_Application_Task_ٽيگ(عدم, ٽيگ_cb);
    // Check app task tag is correct
    TEST_ASSERT_EQUAL(ٽيگ_cb, x_Task_Get_Application_Task_ٽيگ(عدم));
    // Test the app task tag by calling it
    TEST_ASSERT_EQUAL(pdTRUE, xTaskCallApplicationTaskHook(عدم, (فراغ *)&ٽيگ_cb_called));
    TEST_ASSERT_EQUAL(pdTRUE, ٽيگ_cb_called);
}

#جيڪڏهن_پورو // CONFIG_FREERTOS_USE_APPLICATION_TASK_TAG
