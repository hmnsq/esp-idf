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

#جيڪڏهن_وصف_نه_ٿيل INC_NEWLIB_FREERTOS_H
#وصف_ڪريو INC_NEWLIB_FREERTOS_H

/* Note Newlib support has been included by popular demand, but is not
 * used by the FreeRTOS maintainers themselves.  FreeRTOS is not
 * responsible for resulting newlib operation.  User must be familiar with
 * newlib and must provide system-wide implementations of the necessary
 * stubs. Be warned that (at the time of writing) the current newlib design
 * implements a system-wide malloc() that must be provided with locks.
 *
 * See the third party link http://www.nadler.com/embedded/newlibAndFreeRTOS.html
 * for additional information. */

#ضمن <reent.h>

#وصف_ڪريو configUSE_C_RUNTIME_TLS_SUPPORT    1

#جيڪڏهن_وصف_نه_ٿيل config_TLS_بلاڪ_TYPE
    #وصف_ڪريو config_TLS_بلاڪ_TYPE           بنية _reent
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل config_INIT_TLS_بلاڪ
    #وصف_ڪريو config_INIT_TLS_بلاڪ( xTLSBlock, px_چوٽي_جو_ڍير )    _REENT_INIT_PTR( &( xTLSBlock ) )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل config_سيٽ_TLS_بلاڪ
    #وصف_ڪريو config_سيٽ_TLS_بلاڪ( xTLSBlock )    ( _impure_ptr = &( xTLSBlock ) )
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل config_DEINIT_TLS_بلاڪ
    #وصف_ڪريو config_DEINIT_TLS_بلاڪ( xTLSBlock )    _reclaim_reent( &( xTLSBlock ) )
#جيڪڏهن_پورو

#جيڪڏهن_پورو /* INC_NEWLIB_FREERTOS_H */
