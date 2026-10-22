/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#அந்தர்பாவ "sdkconfig.h"
#அந்தர்பாவ "freertos/FreeRTOS.h"
#அந்தர்பாவ "freertos/task.h"
#அந்தர்பாவ "unity.h"
#அந்தர்பாவ "test_utils.h"

#எனில் CONFIG_FREERTOS_USE_APPLICATION_TASK_TAG

நிலையான BaseType_t tag_cb(வெறுமை *arg)
{
    BaseType_t *tag_cb_called = (BaseType_t *)arg;
    *tag_cb_called = pdTRUE;
    திரும்பு pdTRUE;
}

TEST_CASE("Test application task tag", "[freertos]")
{
    BaseType_t tag_cb_called = pdFALSE;

    // Set the app task tag for current task
    vTaskSetApplicationTaskTag(வெற்று, tag_cb);
    // Check app task tag is correct
    TEST_ASSERT_EQUAL(tag_cb, xTaskGetApplicationTaskTag(வெற்று));
    // Test the app task tag by calling it
    TEST_ASSERT_EQUAL(pdTRUE, xTaskCallApplicationTaskHook(வெற்று, (வெறுமை *)&tag_cb_called));
    TEST_ASSERT_EQUAL(pdTRUE, tag_cb_called);
}

#endif // CONFIG_FREERTOS_USE_APPLICATION_TASK_TAG
