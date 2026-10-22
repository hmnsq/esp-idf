/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#समावेश "test_utils.h"

#घोषणा config_TEST_DEFAULT_ढेर_आकार               4096
#घोषणा configTEST_UNITY_TASK_PRIORITY              UNITY_FREERTOS_PRIORITY

#घोषणा portTEST_REF_CLOCK_TYPE                     uint32_t
#घोषणा portTEST_REF_CLOCK_INIT()                   ref_clock_init()
#घोषणा portTEST_REF_CLOCK_DEINIT()                 ref_clock_deinit()
#घोषणा portTEST_REF_CLOCK_GET_TIME()               ((uint32_t) ref_clock_get())
#घोषणा portTEST_TICKS_TO_REF_CLOCK(ticks)          ((ticks) * (1000000/configTICK_RATE_HZ))
