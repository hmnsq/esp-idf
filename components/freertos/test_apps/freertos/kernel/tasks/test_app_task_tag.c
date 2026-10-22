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

#إذا CONFIG_FREERTOS_USE_APPLICATION_TASK_نښان

ساكن بنسټ_Type_t نښان_cb(فراغ *arg)
{
    بنسټ_Type_t *نښان_cb_called = (بنسټ_Type_t *)arg;
    *نښان_cb_called = pdTRUE;
    أرجع pdTRUE;
}

TEST_CASE("Test application task tag", "[freertos]")
{
    بنسټ_Type_t نښان_cb_called = pdFALSE;

    // Set the app task tag for current task
    v_Task_سېټ_Application_Task_نښان(عدم, نښان_cb);
    // Check app task tag is correct
    TEST_ASSERT_EQUAL(نښان_cb, x_Task_Get_Application_Task_نښان(عدم));
    // Test the app task tag by calling it
    TEST_ASSERT_EQUAL(pdTRUE, xTaskCallApplicationTaskHook(عدم, (فراغ *)&نښان_cb_called));
    TEST_ASSERT_EQUAL(pdTRUE, نښان_cb_called);
}

#د_که_پای // CONFIG_FREERTOS_USE_APPLICATION_TASK_TAG
