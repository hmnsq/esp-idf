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

#अगर_अघोषित INC_PICOLIBC_FREERTOS_H
#घोषणा INC_PICOLIBC_FREERTOS_H

/* Use picolibc TLS support to allocate space for __thread variables,
 * initialize them at thread creation and set the TLS context at
 * thread switch time.
 *
 * See the picolibc TLS docs:
 * https://github.com/picolibc/picolibc/blob/main/doc/tls.md
 * for additional information. */

#समावेश <picotls.h>

#घोषणा configUSE_C_RUNTIME_TLS_SUPPORT    1

#घोषणा configTLS_BLOCK_TYPE               व्योम *

#घोषणा picolibc_TLS_आकार                   ( ( port_POINTER_आकार_TYPE ) tls_आकार() )
#घोषणा picolibc_ढेर_ALIGNMENT_MASK       ( ( port_POINTER_आकार_TYPE ) portBYTE_ALIGNMENT_MASK )

#अगर __PICOLIBC_MAJOR__ > 1 || __PICOLIBC_MINOR__ >= 8

/* Picolibc 1.8 and newer have explicit alignment values provided
 * by the _tls_align() inline */
    #घोषणा picolibcTLS_ALIGNMENT_MASK    ( ( port_POINTER_आकार_TYPE ) ( _tls_align() - 1 ) )
#अन्यथा

/* For older Picolibc versions, use the general port alignment value */
    #घोषणा picolibcTLS_ALIGNMENT_MASK    ( ( port_POINTER_आकार_TYPE ) portBYTE_ALIGNMENT_MASK )
#पूर्ण_अगर

/* Allocate thread local storage block off the end of the
 * stack. The picolibcTLS_SIZE macro returns the size (in
 * bytes) of the total TLS area used by the application.
 * Calculate the top of stack address. */
#अगर ( port_ढेर_GROWTH < 0 )

    #घोषणा configINIT_TLS_BLOCK( xTLSBlock, px_शीर्ष_Of_ढेर )                                  \
    करो {                                                                                     \
        xTLSBlock = ( व्योम * ) ( ( ( ( port_POINTER_आकार_TYPE ) px_शीर्ष_Of_ढेर ) -              \
                                   picolibc_TLS_आकार ) &                                      \
                                 ~picolibcTLS_ALIGNMENT_MASK );                              \
        px_शीर्ष_Of_ढेर = ( ढेर_Type_t * ) ( ( ( ( port_POINTER_आकार_TYPE ) xTLSBlock ) - 1 ) & \
                                           ~picolibc_ढेर_ALIGNMENT_MASK );                  \
        _init_tls( xTLSBlock );                                                              \
    } जबतक( 0 )
#अन्यथा /* portSTACK_GROWTH */
    #घोषणा configINIT_TLS_BLOCK( xTLSBlock, px_शीर्ष_Of_ढेर )                                          \
    करो {                                                                                             \
        xTLSBlock = ( व्योम * ) ( ( ( port_POINTER_आकार_TYPE ) px_शीर्ष_Of_ढेर +                          \
                                   picolibcTLS_ALIGNMENT_MASK ) & ~picolibcTLS_ALIGNMENT_MASK );     \
        px_शीर्ष_Of_ढेर = ( ढेर_Type_t * ) ( ( ( ( ( port_POINTER_आकार_TYPE ) xTLSBlock ) +             \
                                               picolibc_TLS_आकार ) + picolibc_ढेर_ALIGNMENT_MASK ) & \
                                           ~picolibc_ढेर_ALIGNMENT_MASK );                          \
        _init_tls( xTLSBlock );                                                                      \
    } जबतक( 0 )
#पूर्ण_अगर /* portSTACK_GROWTH */

#घोषणा configSET_TLS_BLOCK( xTLSBlock )    _set_tls( xTLSBlock )

#घोषणा configDEINIT_TLS_BLOCK( xTLSBlock )

#पूर्ण_अगर /* INC_PICOLIBC_FREERTOS_H */
