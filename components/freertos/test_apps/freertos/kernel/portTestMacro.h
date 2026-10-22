/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن "test_utils.h"

#تعریف_کرو config_TEST_DEFAULT_ڈھیر_ناپ               4096
#تعریف_کرو config_TEST_UNITY_TASK_ترجیح              UNITY_FREERTOS_ترجیح

#تعریف_کرو پورٹ_TEST_REF_CLOCK_TYPE                     uint32_t
#تعریف_کرو پورٹ_TEST_REF_CLOCK_INIT()                   ref_clock_init()
#تعریف_کرو پورٹ_TEST_REF_CLOCK_DEINIT()                 ref_clock_deinit()
#تعریف_کرو پورٹ_TEST_REF_CLOCK_GET_ویلا()               ((uint32_t) ref_clock_get())
#تعریف_کرو پورٹ_TEST_TICKS_منزل_REF_CLOCK(ticks)          ((ticks) * (1000000/config_TICK_شرح_HZ))
