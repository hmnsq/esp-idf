/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "test_utils.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_TEST_DEFAULT_ਢੇਰ_ਮਾਪ               4096
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_TEST_UNITY_TASK_ਤਰਜੀਹ              UNITY_FREERTOS_ਤਰਜੀਹ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TEST_REF_CLOCK_TYPE                     uint32_t
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TEST_REF_CLOCK_INIT()                   ref_clock_init()
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TEST_REF_CLOCK_DEINIT()                 ref_clock_deinit()
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TEST_REF_CLOCK_GET_ਸਮਾਂ()               ((uint32_t) ref_clock_get())
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_TEST_TICKS_ਮੰਜ਼ਿਲ_REF_CLOCK(ticks)          ((ticks) * (1000000/config_TICK_ਦਰ_HZ))
