/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن "test_utils.h"

#تعریف_کړه config_TEST_DEFAULT_ډېری_اندازه               4096
#تعریف_کړه config_TEST_UNITY_TASK_لومړیتوب              UNITY_FREERTOS_لومړیتوب

#تعریف_کړه پورټ_TEST_REF_CLOCK_TYPE                     uint32_t
#تعریف_کړه پورټ_TEST_REF_CLOCK_INIT()                   ref_clock_init()
#تعریف_کړه پورټ_TEST_REF_CLOCK_DEINIT()                 ref_clock_deinit()
#تعریف_کړه پورټ_TEST_REF_CLOCK_GET_وخت()               ((uint32_t) ref_clock_get())
#تعریف_کړه پورټ_TEST_TICKS_منزل_REF_CLOCK(ticks)          ((ticks) * (1000000/config_TICK_شرح_HZ))
