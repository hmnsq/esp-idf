/*
 * FreeRTOS Kernel V11.1.0
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * SPDX-FileCopyrightText: 2021 Amazon.com, Inc. or its affiliates
 *
 * SPDX-License-Identifier: MIT
 *
 * SPDX-FileContributor: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ INC_NEWLIB_FREERTOS_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INC_NEWLIB_FREERTOS_H

/* Note Newlib support has been included by popular demand, but is not
 * used by the FreeRTOS maintainers themselves.  FreeRTOS is not
 * responsible for resulting newlib operation.  User must be familiar with
 * newlib and must provide system-wide implementations of the necessary
 * stubs. Be warned that (at the time of writing) the current newlib design
 * implements a system-wide malloc() that must be provided with locks.
 *
 * See the third party link http://www.nadler.com/embedded/newlibAndFreeRTOS.html
 * for additional information. */

#ਅਨ੍ਤਰ੍ਭਾਵ <reent.h>

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_C_RUNTIME_TLS_SUPPORT    1

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_TLS_ਖੰਡ_TYPE
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_TLS_ਖੰਡ_TYPE           ਬਣਤਰ _reent
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_INIT_TLS_ਖੰਡ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_INIT_TLS_ਖੰਡ( xTLSBlock, px_ਸਿਖਰ_ਦਾ_ਢੇਰ )    _REENT_INIT_PTR( &( xTLSBlock ) )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_ਸੈੱਟ_TLS_ਖੰਡ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_ਸੈੱਟ_TLS_ਖੰਡ( xTLSBlock )    ( _impure_ptr = &( xTLSBlock ) )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_DEINIT_TLS_ਖੰਡ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_DEINIT_TLS_ਖੰਡ( xTLSBlock )    _reclaim_reent( &( xTLSBlock ) )
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ /* INC_NEWLIB_FREERTOS_H */
