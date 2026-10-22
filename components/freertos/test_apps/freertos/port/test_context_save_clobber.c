/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unity.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_intr_alloc.h"

#ਜੇ defined(__XTENSA__) && CONFIG_FREERTOS_CORETIMER_0
#ਅਨ੍ਤਰ੍ਭਾਵ "xtensa/config/core-isa.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xtensa/hal.h"
#ਜੇ defined(XCHAL_HAVE_WINDOWED)
/* Regression test for a0 register being corrupted in _xt_context_save.
 *
 * The idea in this test is to have a function which recursively calls itself
 * with call4, eventually filling up all the register windows. At that point,
 * it does some lengthy operation. If an interrupt occurs at that point, and
 * corrupts a0 register of one of the windows, this will cause an exception
 * when the recursive function returns.
 */

/* See test_context_save_clober_func.S */
ਬਾਹਰੀ ਖਾਲੀ test_context_save_clober_func(ਖਾਲੀ);

ਸਥਿਰ ਖਾਲੀ int_timer_handler(ਖਾਲੀ *arg)
{
    xthal_ਸੈੱਟ_ccompare(1, xthal_get_ccount() + 10000);
    (*(ਪੂਰਨ*) arg)++;
}

TEST_CASE("context save doesn't corrupt return address register", "[freertos]")
{
    /* set up an interrupt */
    intr_handle_t ih;
    ਪੂਰਨ int_triggered = 0;
    TEST_ESP_OK(esp_intr_alloc(ETS_ਅੰਦਰੂਨੀ_TIMER1_INTR_ਸਰੋਤ, 0, int_timer_handler, &int_triggered, &ih));
    xthal_ਸੈੱਟ_ccompare(1, xthal_get_ccount() + 10000);

    /* fill all the windows and delay a bit, waiting for an interrupt to happen */
    test_context_save_clober_func();

    esp_intr_free(ih);
    TEST_ASSERT_GREATER_THAN(0, int_triggered);
}

#ਜੇ_ਅੰਤ // XCHAL_HAVE_WINDOWED
#ਜੇ_ਅੰਤ // __XTENSA__ && CONFIG_FREERTOS_CORETIMER_0
