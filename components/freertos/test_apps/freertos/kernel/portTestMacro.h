/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#अन्तर्भाव "test_utils.h"

#परिभाषय config_TEST_DEFAULT_राशिः_आकार               4096
#परिभाषय config_TEST_UNITY_TASK_प्राथमिकता              UNITY_FREERTOS_प्राथमिकता

#परिभाषय पत्तनम्_TEST_REF_CLOCK_TYPE                     uint32_t
#परिभाषय पत्तनम्_TEST_REF_CLOCK_INIT()                   ref_clock_init()
#परिभाषय पत्तनम्_TEST_REF_CLOCK_DEINIT()                 ref_clock_deinit()
#परिभाषय पत्तनम्_TEST_REF_CLOCK_GET_TIME()               ((uint32_t) ref_clock_get())
#परिभाषय पत्तनम्_TEST_TICKS_गन्तव्यम्_REF_CLOCK(ticks)          ((ticks) * (1000000/config_TICK_दरः_HZ))
