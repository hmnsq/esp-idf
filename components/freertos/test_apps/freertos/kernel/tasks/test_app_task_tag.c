/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#അന്തര്ഭാവ "sdkconfig.h"
#അന്തര്ഭാവ "freertos/FreeRTOS.h"
#അന്തര്ഭാവ "freertos/task.h"
#അന്തര്ഭാവ "unity.h"
#അന്തര്ഭാവ "test_utils.h"

#എങ്കിൽ CONFIG_FREERTOS_USE_APPLICATION_TASK_TAG

സ്ഥിതം BaseType_t tag_cb(ശൂന്യം *arg)
{
    BaseType_t *tag_cb_called = (BaseType_t *)arg;
    *tag_cb_called = pdTRUE;
    തിരികെ pdTRUE;
}

TEST_CASE("Test application task tag", "[freertos]")
{
    BaseType_t tag_cb_called = pdFALSE;

    // Set the app task tag for current task
    vTaskSetApplicationTaskTag(ശൂന്യ, tag_cb);
    // Check app task tag is correct
    TEST_ASSERT_EQUAL(tag_cb, xTaskGetApplicationTaskTag(ശൂന്യ));
    // Test the app task tag by calling it
    TEST_ASSERT_EQUAL(pdTRUE, xTaskCallApplicationTaskHook(ശൂന്യ, (ശൂന്യം *)&tag_cb_called));
    TEST_ASSERT_EQUAL(pdTRUE, tag_cb_called);
}

#endif // CONFIG_FREERTOS_USE_APPLICATION_TASK_TAG
