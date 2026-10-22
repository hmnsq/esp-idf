/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#അന്തര്ഭാവ "sdkconfig.h"
#അന്തര്ഭാവ "unity.h"
#അന്തര്ഭാവ "esp_intr_alloc.h"

#എങ്കിൽ defined(__XTENSA__) && CONFIG_FREERTOS_CORETIMER_0
#അന്തര്ഭാവ "xtensa/config/core-isa.h"
#അന്തര്ഭാവ "xtensa/hal.h"
#എങ്കിൽ defined(XCHAL_HAVE_WINDOWED)
/* Regression test for a0 register being corrupted in _xt_context_save.
 *
 * The idea in this test is to have a function which recursively calls itself
 * with call4, eventually filling up all the register windows. At that point,
 * it does some lengthy operation. If an interrupt occurs at that point, and
 * corrupts a0 register of one of the windows, this will cause an exception
 * when the recursive function returns.
 */

/* See test_context_save_clober_func.S */
ബാഹ്യ ശൂന്യം test_context_save_clober_func(ശൂന്യം);

സ്ഥിതം ശൂന്യം int_timer_handler(ശൂന്യം *arg)
{
    xthal_set_ccompare(1, xthal_get_ccount() + 10000);
    (*(പൂർണ്ണസംഖ്യ*) arg)++;
}

TEST_CASE("context save doesn't corrupt return address register", "[freertos]")
{
    /* set up an interrupt */
    intr_handle_t ih;
    പൂർണ്ണസംഖ്യ int_triggered = 0;
    TEST_ESP_OK(esp_intr_alloc(ETS_INTERNAL_TIMER1_INTR_SOURCE, 0, int_timer_handler, &int_triggered, &ih));
    xthal_set_ccompare(1, xthal_get_ccount() + 10000);

    /* fill all the windows and delay a bit, waiting for an interrupt to happen */
    test_context_save_clober_func();

    esp_intr_free(ih);
    TEST_ASSERT_GREATER_THAN(0, int_triggered);
}

#endif // XCHAL_HAVE_WINDOWED
#endif // __XTENSA__ && CONFIG_FREERTOS_CORETIMER_0
