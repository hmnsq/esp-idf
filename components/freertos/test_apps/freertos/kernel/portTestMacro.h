/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن "test_utils.h"

#تعریف_کن config_TEST_DEFAULT_پشته_اندازه               4096
#تعریف_کن config_TEST_UNITY_TASK_اولویت              UNITY_FREERTOS_اولویت

#تعریف_کن پورت_TEST_REF_CLOCK_TYPE                     uint32_t
#تعریف_کن پورت_TEST_REF_CLOCK_INIT()                   ref_clock_init()
#تعریف_کن پورت_TEST_REF_CLOCK_DEINIT()                 ref_clock_deinit()
#تعریف_کن پورت_TEST_REF_CLOCK_GET_زمان()               ((uint32_t) ref_clock_get())
#تعریف_کن پورت_TEST_TICKS_مقصد_REF_CLOCK(ticks)          ((ticks) * (1000000/config_TICK_نرخ_HZ))
