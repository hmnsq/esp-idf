/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#અન્તર્ભાવ "test_utils.h"

#define config_TEST_DEFAULT_ઢગલો_SIZE               4096
#define configTEST_UNITY_TASK_PRIORITY              UNITY_FREERTOS_PRIORITY

#define portTEST_REF_CLOCK_TYPE                     uint32_t
#define portTEST_REF_CLOCK_INIT()                   ref_clock_init()
#define portTEST_REF_CLOCK_DEINIT()                 ref_clock_deinit()
#define portTEST_REF_CLOCK_GET_TIME()               ((uint32_t) ref_clock_get())
#define portTEST_TICKS_TO_REF_CLOCK(ticks)          ((ticks) * (1000000/configTICK_RATE_HZ))
