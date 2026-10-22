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

#ಆದರೆ CONFIG_FREERTOS_USE_APPLICATION_TASK_ಟ್ಯಾಗ್

ಸ್ಥಿರ ತಳ_Type_t ಟ್ಯಾಗ್_cb(ಶೂನ್ಯ *arg)
{
    ತಳ_Type_t *ಟ್ಯಾಗ್_cb_called = (ತಳ_Type_t *)arg;
    *ಟ್ಯಾಗ್_cb_called = pdTRUE;
    ಹಿಂತಿರುಗಿ pdTRUE;
}

TEST_CASE("Test application task tag", "[freertos]")
{
    ತಳ_Type_t ಟ್ಯಾಗ್_cb_called = pdFALSE;

    // Set the app task tag for current task
    v_Task_ಗಣ_Application_Task_ಟ್ಯಾಗ್(ಖಾಲಿ, ಟ್ಯಾಗ್_cb);
    // Check app task tag is correct
    TEST_ASSERT_EQUAL(ಟ್ಯಾಗ್_cb, x_Task_Get_Application_Task_ಟ್ಯಾಗ್(ಖಾಲಿ));
    // Test the app task tag by calling it
    TEST_ASSERT_EQUAL(pdTRUE, xTaskCallApplicationTaskHook(ಖಾಲಿ, (ಶೂನ್ಯ *)&ಟ್ಯಾಗ್_cb_called));
    TEST_ASSERT_EQUAL(pdTRUE, ಟ್ಯಾಗ್_cb_called);
}

#ಆದರೆ_ಅಂತ್ಯ // CONFIG_FREERTOS_USE_APPLICATION_TASK_TAG
