/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#अन्तर्भाव "sdkconfig.h"
#अन्तर्भाव "freertos/FreeRTOS.h"
#अन्तर्भाव "freertos/task.h"
#अन्तर्भाव "unity.h"
#अन्तर्भाव "test_utils.h"

#यदि CONFIG_FREERTOS_USE_APPLICATION_TASK_चिह्नकम्

स्थिर आधारः_Type_t चिह्नकम्_cb(शून्य *arg)
{
    आधारः_Type_t *चिह्नकम्_cb_called = (आधारः_Type_t *)arg;
    *चिह्नकम्_cb_called = pdTRUE;
    प्रत्यागम pdTRUE;
}

TEST_CASE("Test application task tag", "[freertos]")
{
    आधारः_Type_t चिह्नकम्_cb_called = pdFALSE;

    // Set the app task tag for current task
    v_Task_समुच्चयः_Application_Task_चिह्नकम्(रिक्त, चिह्नकम्_cb);
    // Check app task tag is correct
    TEST_ASSERT_EQUAL(चिह्नकम्_cb, x_Task_Get_Application_Task_चिह्नकम्(रिक्त));
    // Test the app task tag by calling it
    TEST_ASSERT_EQUAL(pdTRUE, xTaskCallApplicationTaskHook(रिक्त, (शून्य *)&चिह्नकम्_cb_called));
    TEST_ASSERT_EQUAL(pdTRUE, चिह्नकम्_cb_called);
}

#यदि_अन्तः // CONFIG_FREERTOS_USE_APPLICATION_TASK_TAG
