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

#اگر_غیر_تعریف_شدہ INC_PICOLIBC_FREERTOS_H
#تعریف_کرو INC_PICOLIBC_FREERTOS_H

/* Use picolibc TLS support to allocate space for __thread variables,
 * initialize them at thread creation and set the TLS context at
 * thread switch time.
 *
 * See the picolibc TLS docs:
 * https://github.com/picolibc/picolibc/blob/main/doc/tls.md
 * for additional information. */

#شامل <picotls.h>

#تعریف_کرو configUSE_C_RUNTIME_TLS_SUPPORT    1

#تعریف_کرو config_TLS_بلاک_TYPE               خالی *

#تعریف_کرو picolibc_TLS_سائز                   ( ( پورٹ_اشارہ_گر_سائز_TYPE ) tls_سائز() )
#تعریف_کرو picolibc_ڈھیر_ALIGNMENT_نقاب       ( ( پورٹ_اشارہ_گر_سائز_TYPE ) پورٹ_BYTE_ALIGNMENT_نقاب )

#اگر __PICOLIBC_MAJOR__ > 1 || __PICOLIBC_MINOR__ >= 8

/* Picolibc 1.8 and newer have explicit alignment values provided
 * by the _tls_align() inline */
    #تعریف_کرو picolibc_TLS_ALIGNMENT_نقاب    ( ( پورٹ_اشارہ_گر_سائز_TYPE ) ( _tls_align() - 1 ) )
#ورنہ

/* For older Picolibc versions, use the general port alignment value */
    #تعریف_کرو picolibc_TLS_ALIGNMENT_نقاب    ( ( پورٹ_اشارہ_گر_سائز_TYPE ) پورٹ_BYTE_ALIGNMENT_نقاب )
#اگر_ختم

/* Allocate thread local storage block off the end of the
 * stack. The picolibcTLS_SIZE macro returns the size (in
 * bytes) of the total TLS area used by the application.
 * Calculate the top of stack address. */
#اگر ( پورٹ_ڈھیر_GROWTH < 0 )

    #تعریف_کرو config_INIT_TLS_بلاک( xTLSBlock, px_چوٹی_کا_ڈھیر )                                  \
    کرو {                                                                                     \
        xTLSBlock = ( خالی * ) ( ( ( ( پورٹ_اشارہ_گر_سائز_TYPE ) px_چوٹی_کا_ڈھیر ) -              \
                                   picolibc_TLS_سائز ) &                                      \
                                 ~picolibc_TLS_ALIGNMENT_نقاب );                              \
        px_چوٹی_کا_ڈھیر = ( ڈھیر_Type_t * ) ( ( ( ( پورٹ_اشارہ_گر_سائز_TYPE ) xTLSBlock ) - 1 ) & \
                                           ~picolibc_ڈھیر_ALIGNMENT_نقاب );                  \
        _init_tls( xTLSBlock );                                                              \
    } جب( 0 )
#ورنہ /* portSTACK_GROWTH */
    #تعریف_کرو config_INIT_TLS_بلاک( xTLSBlock, px_چوٹی_کا_ڈھیر )                                          \
    کرو {                                                                                             \
        xTLSBlock = ( خالی * ) ( ( ( پورٹ_اشارہ_گر_سائز_TYPE ) px_چوٹی_کا_ڈھیر +                          \
                                   picolibc_TLS_ALIGNMENT_نقاب ) & ~picolibc_TLS_ALIGNMENT_نقاب );     \
        px_چوٹی_کا_ڈھیر = ( ڈھیر_Type_t * ) ( ( ( ( ( پورٹ_اشارہ_گر_سائز_TYPE ) xTLSBlock ) +             \
                                               picolibc_TLS_سائز ) + picolibc_ڈھیر_ALIGNMENT_نقاب ) & \
                                           ~picolibc_ڈھیر_ALIGNMENT_نقاب );                          \
        _init_tls( xTLSBlock );                                                                      \
    } جب( 0 )
#اگر_ختم /* portSTACK_GROWTH */

#تعریف_کرو config_سیٹ_TLS_بلاک( xTLSBlock )    سیٹ_tls( xTLSBlock )

#تعریف_کرو config_DEINIT_TLS_بلاک( xTLSBlock )

#اگر_ختم /* INC_PICOLIBC_FREERTOS_H */
