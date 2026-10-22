/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#అన్తర్భావ "test_utils.h"

#నిర్వచించు config_TEST_DEFAULT_దొంతర_కొలత               4096
#నిర్వచించు config_TEST_UNITY_TASK_ప్రాధాన్యత              UNITY_FREERTOS_ప్రాధాన్యత

#నిర్వచించు పోర్టు_TEST_REF_CLOCK_TYPE                     uint32_t
#నిర్వచించు పోర్టు_TEST_REF_CLOCK_INIT()                   ref_clock_init()
#నిర్వచించు పోర్టు_TEST_REF_CLOCK_DEINIT()                 ref_clock_deinit()
#నిర్వచించు పోర్టు_TEST_REF_CLOCK_GET_సమయం()               ((uint32_t) ref_clock_get())
#నిర్వచించు పోర్టు_TEST_TICKS_గమ్యం_REF_CLOCK(ticks)          ((ticks) * (1000000/config_TICK_రేటు_HZ))
