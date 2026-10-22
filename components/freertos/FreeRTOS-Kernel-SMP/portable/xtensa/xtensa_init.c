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


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ XT_BOARD
#ਅਨ੍ਤਰ੍ਭਾਵ "xtensa/xtbsp.h"
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "xtensa_rtos.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_private/esp_clk.h"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ XT_RTOS_TIMER_INT

ਬਿਨਾਨਿਸ਼ਾਨੀ xt_tick_ਭਾਜਕ = 0;  /* cached number of cycles per tick */

ਖਾਲੀ xt_tick_ਭਾਜਕ_init(ਖਾਲੀ)
{
    xt_tick_ਭਾਜਕ = esp_clk_cpu_freq() / XT_TICK_ਪ੍ਰਤੀ_SEC;
}

/* Deprecated, to be removed */
ਪੂਰਨ xt_clock_freq(ਖਾਲੀ)
{
    ਵਾਪਸ esp_clk_cpu_freq();
}

#ਜੇ_ਅੰਤ /* XT_RTOS_TIMER_INT */
