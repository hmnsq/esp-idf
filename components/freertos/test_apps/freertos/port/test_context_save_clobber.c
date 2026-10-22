/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#شامل "sdkconfig.h"
#شامل "unity.h"
#شامل "esp_intr_alloc.h"

#اگر defined(__XTENSA__) && CONFIG_FREERTOS_CORETIMER_0
#شامل "xtensa/config/core-isa.h"
#شامل "xtensa/hal.h"
#اگر defined(XCHAL_HAVE_WINDOWED)
/* Regression test for a0 register being corrupted in _xt_context_save.
 *
 * The idea in this test is to have a function which recursively calls itself
 * with call4, eventually filling up all the register windows. At that point,
 * it does some lengthy operation. If an interrupt occurs at that point, and
 * corrupts a0 register of one of the windows, this will cause an exception
 * when the recursive function returns.
 */

/* See test_context_save_clober_func.S */
خارجی خالی test_context_save_clober_func(خالی);

ساکن خالی int_timer_handler(خالی *arg)
{
    xthal_سیٹ_ccompare(1, xthal_get_ccount() + 10000);
    (*(صحیح*) arg)++;
}

TEST_CASE("context save doesn't corrupt return address register", "[freertos]")
{
    /* set up an interrupt */
    intr_handle_t ih;
    صحیح int_triggered = 0;
    TEST_ESP_OK(esp_intr_alloc(ETS_اندرونی_TIMER1_INTR_ماخذ, 0, int_timer_handler, &int_triggered, &ih));
    xthal_سیٹ_ccompare(1, xthal_get_ccount() + 10000);

    /* fill all the windows and delay a bit, waiting for an interrupt to happen */
    test_context_save_clober_func();

    esp_intr_free(ih);
    TEST_ASSERT_GREATER_THAN(0, int_triggered);
}

#اگر_ختم // XCHAL_HAVE_WINDOWED
#اگر_ختم // __XTENSA__ && CONFIG_FREERTOS_CORETIMER_0
