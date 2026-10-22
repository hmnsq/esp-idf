/*
 * SPDX-FileCopyrightText: 2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ "unity.h"
#ಅನ್ತರ್ಭಾವ "test_utils.h"

#ಯದಿ CONFIG_FREERTOS_USE_APPLICATION_TASK_TAG

ಸ್ಥಿರ BaseType_t tag_cb(ಶೂನ್ಯ *arg)
{
    BaseType_t *tag_cb_called = (BaseType_t *)arg;
    *tag_cb_called = pdTRUE;
    ಪ್ರತ್ಯಾಗಮ pdTRUE;
}

TEST_CASE("Test application task tag", "[freertos]")
{
    BaseType_t tag_cb_called = pdFALSE;

    // Set the app task tag for current task
    vTaskSetApplicationTaskTag(ರಿಕ್ತ, tag_cb);
    // Check app task tag is correct
    TEST_ASSERT_EQUAL(tag_cb, xTaskGetApplicationTaskTag(ರಿಕ್ತ));
    // Test the app task tag by calling it
    TEST_ASSERT_EQUAL(pdTRUE, xTaskCallApplicationTaskHook(ರಿಕ್ತ, (ಶೂನ್ಯ *)&tag_cb_called));
    TEST_ASSERT_EQUAL(pdTRUE, tag_cb_called);
}

#endif // CONFIG_FREERTOS_USE_APPLICATION_TASK_TAG
