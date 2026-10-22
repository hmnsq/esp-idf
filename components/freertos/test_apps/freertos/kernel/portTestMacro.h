/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن "test_utils.h"

#وصف_ڪريو config_TEST_DEFAULT_ڍير_سائيز               4096
#وصف_ڪريو config_TEST_UNITY_TASK_ترجيح              UNITY_FREERTOS_ترجيح

#وصف_ڪريو پورٽ_TEST_REF_CLOCK_TYPE                     uint32_t
#وصف_ڪريو پورٽ_TEST_REF_CLOCK_INIT()                   ref_clock_init()
#وصف_ڪريو پورٽ_TEST_REF_CLOCK_DEINIT()                 ref_clock_deinit()
#وصف_ڪريو پورٽ_TEST_REF_CLOCK_GET_وقت()               ((uint32_t) ref_clock_get())
#وصف_ڪريو پورٽ_TEST_TICKS_منزل_REF_CLOCK(ticks)          ((ticks) * (1000000/config_TICK_شرح_HZ))
