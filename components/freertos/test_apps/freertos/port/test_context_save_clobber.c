/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ "unity.h"
#అన్తర్భావ "esp_intr_alloc.h"

#ఒకవేళ defined(__XTENSA__) && CONFIG_FREERTOS_CORETIMER_0
#అన్తర్భావ "xtensa/config/core-isa.h"
#అన్తర్భావ "xtensa/hal.h"
#ఒకవేళ defined(XCHAL_HAVE_WINDOWED)
/* Regression test for a0 register being corrupted in _xt_context_save.
 *
 * The idea in this test is to have a function which recursively calls itself
 * with call4, eventually filling up all the register windows. At that point,
 * it does some lengthy operation. If an interrupt occurs at that point, and
 * corrupts a0 register of one of the windows, this will cause an exception
 * when the recursive function returns.
 */

/* See test_context_save_clober_func.S */
బాహ్య శూన్య test_context_save_clober_func(శూన్య);

స్థిర శూన్య int_timer_handler(శూన్య *arg)
{
    xthal_సమితి_ccompare(1, xthal_get_ccount() + 10000);
    (*(పూర్ణాంకం*) arg)++;
}

TEST_CASE("context save doesn't corrupt return address register", "[freertos]")
{
    /* set up an interrupt */
    intr_handle_t ih;
    పూర్ణాంకం int_triggered = 0;
    TEST_ESP_OK(esp_intr_alloc(ETS_అంతర్గత_TIMER1_INTR_ఆధారం, 0, int_timer_handler, &int_triggered, &ih));
    xthal_సమితి_ccompare(1, xthal_get_ccount() + 10000);

    /* fill all the windows and delay a bit, waiting for an interrupt to happen */
    test_context_save_clober_func();

    esp_intr_free(ih);
    TEST_ASSERT_GREATER_THAN(0, int_triggered);
}

#అయితే_ముగింపు // XCHAL_HAVE_WINDOWED
#అయితే_ముగింపు // __XTENSA__ && CONFIG_FREERTOS_CORETIMER_0
