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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ INC_PICOLIBC_FREERTOS_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INC_PICOLIBC_FREERTOS_H

/* Use picolibc TLS support to allocate space for __thread variables,
 * initialize them at thread creation and set the TLS context at
 * thread switch time.
 *
 * See the picolibc TLS docs:
 * https://github.com/picolibc/picolibc/blob/main/doc/tls.md
 * for additional information. */

#ਅਨ੍ਤਰ੍ਭਾਵ <picotls.h>

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configUSE_C_RUNTIME_TLS_SUPPORT    1

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_TLS_ਖੰਡ_TYPE               ਖਾਲੀ *

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ picolibc_TLS_ਮਾਪ                   ( ( ਪੋਰਟ_ਸੂਚਕ_ਮਾਪ_TYPE ) tls_ਮਾਪ() )
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ picolibc_ਢੇਰ_ALIGNMENT_ਨਕਾਬ       ( ( ਪੋਰਟ_ਸੂਚਕ_ਮਾਪ_TYPE ) ਪੋਰਟ_BYTE_ALIGNMENT_ਨਕਾਬ )

#ਜੇ __PICOLIBC_MAJOR__ > 1 || __PICOLIBC_MINOR__ >= 8

/* Picolibc 1.8 and newer have explicit alignment values provided
 * by the _tls_align() inline */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ picolibc_TLS_ALIGNMENT_ਨਕਾਬ    ( ( ਪੋਰਟ_ਸੂਚਕ_ਮਾਪ_TYPE ) ( _tls_align() - 1 ) )
#ਨਹੀਂਤਾਂ

/* For older Picolibc versions, use the general port alignment value */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ picolibc_TLS_ALIGNMENT_ਨਕਾਬ    ( ( ਪੋਰਟ_ਸੂਚਕ_ਮਾਪ_TYPE ) ਪੋਰਟ_BYTE_ALIGNMENT_ਨਕਾਬ )
#ਜੇ_ਅੰਤ

/* Allocate thread local storage block off the end of the
 * stack. The picolibcTLS_SIZE macro returns the size (in
 * bytes) of the total TLS area used by the application.
 * Calculate the top of stack address. */
#ਜੇ ( ਪੋਰਟ_ਢੇਰ_GROWTH < 0 )

    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_INIT_TLS_ਖੰਡ( xTLSBlock, px_ਸਿਖਰ_ਦਾ_ਢੇਰ )                                  \
    ਕਰੋ {                                                                                     \
        xTLSBlock = ( ਖਾਲੀ * ) ( ( ( ( ਪੋਰਟ_ਸੂਚਕ_ਮਾਪ_TYPE ) px_ਸਿਖਰ_ਦਾ_ਢੇਰ ) -              \
                                   picolibc_TLS_ਮਾਪ ) &                                      \
                                 ~picolibc_TLS_ALIGNMENT_ਨਕਾਬ );                              \
        px_ਸਿਖਰ_ਦਾ_ਢੇਰ = ( ਢੇਰ_Type_t * ) ( ( ( ( ਪੋਰਟ_ਸੂਚਕ_ਮਾਪ_TYPE ) xTLSBlock ) - 1 ) & \
                                           ~picolibc_ਢੇਰ_ALIGNMENT_ਨਕਾਬ );                  \
        _init_tls( xTLSBlock );                                                              \
    } ਜਦੋਂ( 0 )
#ਨਹੀਂਤਾਂ /* portSTACK_GROWTH */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_INIT_TLS_ਖੰਡ( xTLSBlock, px_ਸਿਖਰ_ਦਾ_ਢੇਰ )                                          \
    ਕਰੋ {                                                                                             \
        xTLSBlock = ( ਖਾਲੀ * ) ( ( ( ਪੋਰਟ_ਸੂਚਕ_ਮਾਪ_TYPE ) px_ਸਿਖਰ_ਦਾ_ਢੇਰ +                          \
                                   picolibc_TLS_ALIGNMENT_ਨਕਾਬ ) & ~picolibc_TLS_ALIGNMENT_ਨਕਾਬ );     \
        px_ਸਿਖਰ_ਦਾ_ਢੇਰ = ( ਢੇਰ_Type_t * ) ( ( ( ( ( ਪੋਰਟ_ਸੂਚਕ_ਮਾਪ_TYPE ) xTLSBlock ) +             \
                                               picolibc_TLS_ਮਾਪ ) + picolibc_ਢੇਰ_ALIGNMENT_ਨਕਾਬ ) & \
                                           ~picolibc_ਢੇਰ_ALIGNMENT_ਨਕਾਬ );                          \
        _init_tls( xTLSBlock );                                                                      \
    } ਜਦੋਂ( 0 )
#ਜੇ_ਅੰਤ /* portSTACK_GROWTH */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_ਸੈੱਟ_TLS_ਖੰਡ( xTLSBlock )    ਸੈੱਟ_tls( xTLSBlock )

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_DEINIT_TLS_ਖੰਡ( xTLSBlock )

#ਜੇ_ਅੰਤ /* INC_PICOLIBC_FREERTOS_H */
