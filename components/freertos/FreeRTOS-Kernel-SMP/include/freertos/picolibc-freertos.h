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

#جيڪڏهن_وصف_نه_ٿيل INC_PICOLIBC_FREERTOS_H
#وصف_ڪريو INC_PICOLIBC_FREERTOS_H

/* Use picolibc TLS support to allocate space for __thread variables,
 * initialize them at thread creation and set the TLS context at
 * thread switch time.
 *
 * See the picolibc TLS docs:
 * https://github.com/picolibc/picolibc/blob/main/doc/tls.md
 * for additional information. */

#ضمن <picotls.h>

#وصف_ڪريو configUSE_C_RUNTIME_TLS_SUPPORT    1

#وصف_ڪريو config_TLS_بلاڪ_TYPE               فراغ *

#وصف_ڪريو picolibc_TLS_سائيز                   ( ( پورٽ_اشارو_سائيز_TYPE ) tls_سائيز() )
#وصف_ڪريو picolibc_ڍير_ALIGNMENT_نقاب       ( ( پورٽ_اشارو_سائيز_TYPE ) پورٽ_BYTE_ALIGNMENT_نقاب )

#إذا __PICOLIBC_MAJOR__ > 1 || __PICOLIBC_MINOR__ >= 8

/* Picolibc 1.8 and newer have explicit alignment values provided
 * by the _tls_align() inline */
    #وصف_ڪريو picolibc_TLS_ALIGNMENT_نقاب    ( ( پورٽ_اشارو_سائيز_TYPE ) ( _tls_align() - 1 ) )
#وإلا

/* For older Picolibc versions, use the general port alignment value */
    #وصف_ڪريو picolibc_TLS_ALIGNMENT_نقاب    ( ( پورٽ_اشارو_سائيز_TYPE ) پورٽ_BYTE_ALIGNMENT_نقاب )
#جيڪڏهن_پورو

/* Allocate thread local storage block off the end of the
 * stack. The picolibcTLS_SIZE macro returns the size (in
 * bytes) of the total TLS area used by the application.
 * Calculate the top of stack address. */
#إذا ( پورٽ_ڍير_GROWTH < 0 )

    #وصف_ڪريو config_INIT_TLS_بلاڪ( xTLSBlock, px_چوٽي_جو_ڍير )                                  \
    افعل {                                                                                     \
        xTLSBlock = ( فراغ * ) ( ( ( ( پورٽ_اشارو_سائيز_TYPE ) px_چوٽي_جو_ڍير ) -              \
                                   picolibc_TLS_سائيز ) &                                      \
                                 ~picolibc_TLS_ALIGNMENT_نقاب );                              \
        px_چوٽي_جو_ڍير = ( ڍير_Type_t * ) ( ( ( ( پورٽ_اشارو_سائيز_TYPE ) xTLSBlock ) - 1 ) & \
                                           ~picolibc_ڍير_ALIGNMENT_نقاب );                  \
        _init_tls( xTLSBlock );                                                              \
    } طالما( 0 )
#وإلا /* portSTACK_GROWTH */
    #وصف_ڪريو config_INIT_TLS_بلاڪ( xTLSBlock, px_چوٽي_جو_ڍير )                                          \
    افعل {                                                                                             \
        xTLSBlock = ( فراغ * ) ( ( ( پورٽ_اشارو_سائيز_TYPE ) px_چوٽي_جو_ڍير +                          \
                                   picolibc_TLS_ALIGNMENT_نقاب ) & ~picolibc_TLS_ALIGNMENT_نقاب );     \
        px_چوٽي_جو_ڍير = ( ڍير_Type_t * ) ( ( ( ( ( پورٽ_اشارو_سائيز_TYPE ) xTLSBlock ) +             \
                                               picolibc_TLS_سائيز ) + picolibc_ڍير_ALIGNMENT_نقاب ) & \
                                           ~picolibc_ڍير_ALIGNMENT_نقاب );                          \
        _init_tls( xTLSBlock );                                                                      \
    } طالما( 0 )
#جيڪڏهن_پورو /* portSTACK_GROWTH */

#وصف_ڪريو config_سيٽ_TLS_بلاڪ( xTLSBlock )    سيٽ_tls( xTLSBlock )

#وصف_ڪريو config_DEINIT_TLS_بلاڪ( xTLSBlock )

#جيڪڏهن_پورو /* INC_PICOLIBC_FREERTOS_H */
