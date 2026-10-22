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


#جے_تعریف_شدہ XT_BOARD
#ضمن "xtensa/xtbsp.h"
#جے_انت

#ضمن "xtensa_rtos.h"
#ضمن "sdkconfig.h"
#ضمن "esp_private/esp_clk.h"

#جے_تعریف_شدہ XT_RTOS_TIMER_INT

بنانشانی xt_tick_مقسوم_علیہ = 0;  /* cached number of cycles per tick */

فراغ xt_tick_مقسوم_علیہ_init(فراغ)
{
    xt_tick_مقسوم_علیہ = esp_clk_cpu_freq() / XT_TICK_فی_SEC;
}

/* Deprecated, to be removed */
صحيح xt_clock_freq(فراغ)
{
    أرجع esp_clk_cpu_freq();
}

#جے_انت /* XT_RTOS_TIMER_INT */
