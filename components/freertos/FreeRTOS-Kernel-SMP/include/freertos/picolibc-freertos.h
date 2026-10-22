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

#నిర్వచితం_కాకపోతే INC_PICOLIBC_FREERTOS_H
#నిర్వచించు INC_PICOLIBC_FREERTOS_H

/* Use picolibc TLS support to allocate space for __thread variables,
 * initialize them at thread creation and set the TLS context at
 * thread switch time.
 *
 * See the picolibc TLS docs:
 * https://github.com/picolibc/picolibc/blob/main/doc/tls.md
 * for additional information. */

#అన్తర్భావ <picotls.h>

#నిర్వచించు configUSE_C_RUNTIME_TLS_SUPPORT    1

#నిర్వచించు config_TLS_ఖండం_TYPE               శూన్య *

#నిర్వచించు picolibc_TLS_కొలత                   ( ( పోర్టు_సూచిక_కొలత_TYPE ) tls_కొలత() )
#నిర్వచించు picolibc_దొంతర_ALIGNMENT_ముసుగు       ( ( పోర్టు_సూచిక_కొలత_TYPE ) పోర్టు_BYTE_ALIGNMENT_ముసుగు )

#ఒకవేళ __PICOLIBC_MAJOR__ > 1 || __PICOLIBC_MINOR__ >= 8

/* Picolibc 1.8 and newer have explicit alignment values provided
 * by the _tls_align() inline */
    #నిర్వచించు picolibc_TLS_ALIGNMENT_ముసుగు    ( ( పోర్టు_సూచిక_కొలత_TYPE ) ( _tls_align() - 1 ) )
#లేకపోతే

/* For older Picolibc versions, use the general port alignment value */
    #నిర్వచించు picolibc_TLS_ALIGNMENT_ముసుగు    ( ( పోర్టు_సూచిక_కొలత_TYPE ) పోర్టు_BYTE_ALIGNMENT_ముసుగు )
#అయితే_ముగింపు

/* Allocate thread local storage block off the end of the
 * stack. The picolibcTLS_SIZE macro returns the size (in
 * bytes) of the total TLS area used by the application.
 * Calculate the top of stack address. */
#ఒకవేళ ( పోర్టు_దొంతర_GROWTH < 0 )

    #నిర్వచించు config_INIT_TLS_ఖండం( xTLSBlock, px_శిఖరం_యొక్క_దొంతర )                                  \
    చేయి {                                                                                     \
        xTLSBlock = ( శూన్య * ) ( ( ( ( పోర్టు_సూచిక_కొలత_TYPE ) px_శిఖరం_యొక్క_దొంతర ) -              \
                                   picolibc_TLS_కొలత ) &                                      \
                                 ~picolibc_TLS_ALIGNMENT_ముసుగు );                              \
        px_శిఖరం_యొక్క_దొంతర = ( దొంతర_Type_t * ) ( ( ( ( పోర్టు_సూచిక_కొలత_TYPE ) xTLSBlock ) - 1 ) & \
                                           ~picolibc_దొంతర_ALIGNMENT_ముసుగు );                  \
        _init_tls( xTLSBlock );                                                              \
    } అయితే( 0 )
#లేకపోతే /* portSTACK_GROWTH */
    #నిర్వచించు config_INIT_TLS_ఖండం( xTLSBlock, px_శిఖరం_యొక్క_దొంతర )                                          \
    చేయి {                                                                                             \
        xTLSBlock = ( శూన్య * ) ( ( ( పోర్టు_సూచిక_కొలత_TYPE ) px_శిఖరం_యొక్క_దొంతర +                          \
                                   picolibc_TLS_ALIGNMENT_ముసుగు ) & ~picolibc_TLS_ALIGNMENT_ముసుగు );     \
        px_శిఖరం_యొక్క_దొంతర = ( దొంతర_Type_t * ) ( ( ( ( ( పోర్టు_సూచిక_కొలత_TYPE ) xTLSBlock ) +             \
                                               picolibc_TLS_కొలత ) + picolibc_దొంతర_ALIGNMENT_ముసుగు ) & \
                                           ~picolibc_దొంతర_ALIGNMENT_ముసుగు );                          \
        _init_tls( xTLSBlock );                                                                      \
    } అయితే( 0 )
#అయితే_ముగింపు /* portSTACK_GROWTH */

#నిర్వచించు config_సమితి_TLS_ఖండం( xTLSBlock )    సమితి_tls( xTLSBlock )

#నిర్వచించు config_DEINIT_TLS_ఖండం( xTLSBlock )

#అయితే_ముగింపు /* INC_PICOLIBC_FREERTOS_H */
