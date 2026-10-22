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

#که_نه_تعریف_شوی INC_PICOLIBC_FREERTOS_H
#تعریف_کړه INC_PICOLIBC_FREERTOS_H

/* Use picolibc TLS support to allocate space for __thread variables,
 * initialize them at thread creation and set the TLS context at
 * thread switch time.
 *
 * See the picolibc TLS docs:
 * https://github.com/picolibc/picolibc/blob/main/doc/tls.md
 * for additional information. */

#ضمن <picotls.h>

#تعریف_کړه configUSE_C_RUNTIME_TLS_SUPPORT    1

#تعریف_کړه config_TLS_بلاک_TYPE               فراغ *

#تعریف_کړه picolibc_TLS_اندازه                   ( ( پورټ_اشاره_ګر_اندازه_TYPE ) tls_اندازه() )
#تعریف_کړه picolibc_ډېری_ALIGNMENT_ماسک       ( ( پورټ_اشاره_ګر_اندازه_TYPE ) پورټ_BYTE_ALIGNMENT_ماسک )

#إذا __PICOLIBC_MAJOR__ > 1 || __PICOLIBC_MINOR__ >= 8

/* Picolibc 1.8 and newer have explicit alignment values provided
 * by the _tls_align() inline */
    #تعریف_کړه picolibc_TLS_ALIGNMENT_ماسک    ( ( پورټ_اشاره_ګر_اندازه_TYPE ) ( _tls_align() - 1 ) )
#وإلا

/* For older Picolibc versions, use the general port alignment value */
    #تعریف_کړه picolibc_TLS_ALIGNMENT_ماسک    ( ( پورټ_اشاره_ګر_اندازه_TYPE ) پورټ_BYTE_ALIGNMENT_ماسک )
#د_که_پای

/* Allocate thread local storage block off the end of the
 * stack. The picolibcTLS_SIZE macro returns the size (in
 * bytes) of the total TLS area used by the application.
 * Calculate the top of stack address. */
#إذا ( پورټ_ډېری_GROWTH < 0 )

    #تعریف_کړه config_INIT_TLS_بلاک( xTLSBlock, px_سر_د_ډېری )                                  \
    افعل {                                                                                     \
        xTLSBlock = ( فراغ * ) ( ( ( ( پورټ_اشاره_ګر_اندازه_TYPE ) px_سر_د_ډېری ) -              \
                                   picolibc_TLS_اندازه ) &                                      \
                                 ~picolibc_TLS_ALIGNMENT_ماسک );                              \
        px_سر_د_ډېری = ( ډېری_Type_t * ) ( ( ( ( پورټ_اشاره_ګر_اندازه_TYPE ) xTLSBlock ) - 1 ) & \
                                           ~picolibc_ډېری_ALIGNMENT_ماسک );                  \
        _init_tls( xTLSBlock );                                                              \
    } طالما( 0 )
#وإلا /* portSTACK_GROWTH */
    #تعریف_کړه config_INIT_TLS_بلاک( xTLSBlock, px_سر_د_ډېری )                                          \
    افعل {                                                                                             \
        xTLSBlock = ( فراغ * ) ( ( ( پورټ_اشاره_ګر_اندازه_TYPE ) px_سر_د_ډېری +                          \
                                   picolibc_TLS_ALIGNMENT_ماسک ) & ~picolibc_TLS_ALIGNMENT_ماسک );     \
        px_سر_د_ډېری = ( ډېری_Type_t * ) ( ( ( ( ( پورټ_اشاره_ګر_اندازه_TYPE ) xTLSBlock ) +             \
                                               picolibc_TLS_اندازه ) + picolibc_ډېری_ALIGNMENT_ماسک ) & \
                                           ~picolibc_ډېری_ALIGNMENT_ماسک );                          \
        _init_tls( xTLSBlock );                                                                      \
    } طالما( 0 )
#د_که_پای /* portSTACK_GROWTH */

#تعریف_کړه config_سېټ_TLS_بلاک( xTLSBlock )    سېټ_tls( xTLSBlock )

#تعریف_کړه config_DEINIT_TLS_بلاک( xTLSBlock )

#د_که_پای /* INC_PICOLIBC_FREERTOS_H */
