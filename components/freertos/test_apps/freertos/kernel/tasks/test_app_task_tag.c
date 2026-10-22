/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ଅନ୍ତର୍ଭାଵ "sdkconfig.h"
#ଅନ୍ତର୍ଭାଵ "freertos/FreeRTOS.h"
#ଅନ୍ତର୍ଭାଵ "freertos/task.h"
#ଅନ୍ତର୍ଭାଵ "unity.h"
#ଅନ୍ତର୍ଭାଵ "test_utils.h"

#ଯଦି CONFIG_FREERTOS_USE_APPLICATION_TASK_TAG

ସ୍ଥିର BaseType_t tag_cb(ଶୂନ୍ୟ *arg)
{
    BaseType_t *tag_cb_called = (BaseType_t *)arg;
    *tag_cb_called = pdTRUE;
    ଫେରନ୍ତୁ pdTRUE;
}

TEST_CASE("Test application task tag", "[freertos]")
{
    BaseType_t tag_cb_called = pdFALSE;

    // Set the app task tag for current task
    vTaskSetApplicationTaskTag(ଖାଲି, tag_cb);
    // Check app task tag is correct
    TEST_ASSERT_EQUAL(tag_cb, xTaskGetApplicationTaskTag(ଖାଲି));
    // Test the app task tag by calling it
    TEST_ASSERT_EQUAL(pdTRUE, xTaskCallApplicationTaskHook(ଖାଲି, (ଶୂନ୍ୟ *)&tag_cb_called));
    TEST_ASSERT_EQUAL(pdTRUE, tag_cb_called);
}

#endif // CONFIG_FREERTOS_USE_APPLICATION_TASK_TAG
