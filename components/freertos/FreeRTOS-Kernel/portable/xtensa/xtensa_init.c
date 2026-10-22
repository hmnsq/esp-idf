/*
 * SPDX-FileCopyrightText: 2015-2019 Cadence Design Systems, Inc.
 *
 * SPDX-License-Identifier: MIT
 *
 * SPDX-FileContributor: 2016-2022 Espressif Systems (Shanghai) CO LTD
 */
/*
 * Copyright (c) 2015-2019 Cadence Design Systems, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * XTENSA INITIALIZATION ROUTINES CODED IN C
 *
 * This file contains miscellaneous Xtensa RTOS-generic initialization functions
 * that are implemented in C.
 */


#నిర్వచితమైతే XT_BOARD
#అన్తర్భావ "xtensa/xtbsp.h"
#అయితే_ముగింపు

#అన్తర్భావ "xtensa_rtos.h"
#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ "esp_private/esp_clk.h"

#నిర్వచితమైతే XT_RTOS_TIMER_INT

గుర్తులేని xt_tick_భాజకం = 0;  /* cached number of cycles per tick */

శూన్య xt_tick_భాజకం_init(శూన్య)
{
    xt_tick_భాజకం = esp_clk_cpu_freq() / XT_TICK_చొప్పున_SEC;
}

/* Deprecated, to be removed */
పూర్ణాంకం xt_clock_freq(శూన్య)
{
    తిరిగి esp_clk_cpu_freq();
}

#అయితే_ముగింపు /* XT_RTOS_TIMER_INT */
