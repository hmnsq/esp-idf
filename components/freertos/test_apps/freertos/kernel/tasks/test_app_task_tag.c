/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ "unity.h"
#అన్తర్భావ "test_utils.h"

#ఒకవేళ CONFIG_FREERTOS_USE_APPLICATION_TASK_ట్యాగ్

స్థిర పునాది_Type_t ట్యాగ్_cb(శూన్య *arg)
{
    పునాది_Type_t *ట్యాగ్_cb_called = (పునాది_Type_t *)arg;
    *ట్యాగ్_cb_called = pdTRUE;
    తిరిగి pdTRUE;
}

TEST_CASE("Test application task tag", "[freertos]")
{
    పునాది_Type_t ట్యాగ్_cb_called = pdFALSE;

    // Set the app task tag for current task
    v_Task_సమితి_Application_Task_ట్యాగ్(ఖాళీ, ట్యాగ్_cb);
    // Check app task tag is correct
    TEST_ASSERT_EQUAL(ట్యాగ్_cb, x_Task_Get_Application_Task_ట్యాగ్(ఖాళీ));
    // Test the app task tag by calling it
    TEST_ASSERT_EQUAL(pdTRUE, xTaskCallApplicationTaskHook(ఖాళీ, (శూన్య *)&ట్యాగ్_cb_called));
    TEST_ASSERT_EQUAL(pdTRUE, ట్యాగ్_cb_called);
}

#అయితే_ముగింపు // CONFIG_FREERTOS_USE_APPLICATION_TASK_TAG
