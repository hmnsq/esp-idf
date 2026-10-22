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

#यदि_अपरिभाषितम् INC_PICOLIBC_FREERTOS_H
#परिभाषय INC_PICOLIBC_FREERTOS_H

/* Use picolibc TLS support to allocate space for __thread variables,
 * initialize them at thread creation and set the TLS context at
 * thread switch time.
 *
 * See the picolibc TLS docs:
 * https://github.com/picolibc/picolibc/blob/main/doc/tls.md
 * for additional information. */

#अन्तर्भाव <picotls.h>

#परिभाषय configUSE_C_RUNTIME_TLS_SUPPORT    1

#परिभाषय config_TLS_खण्डम्_TYPE               शून्य *

#परिभाषय picolibc_TLS_आकार                   ( ( पत्तनम्_POINTER_आकार_TYPE ) tls_आकार() )
#परिभाषय picolibc_राशिः_ALIGNMENT_आवरणम्       ( ( पत्तनम्_POINTER_आकार_TYPE ) पत्तनम्_BYTE_ALIGNMENT_आवरणम् )

#यदि __PICOLIBC_MAJOR__ > 1 || __PICOLIBC_MINOR__ >= 8

/* Picolibc 1.8 and newer have explicit alignment values provided
 * by the _tls_align() inline */
    #परिभाषय picolibc_TLS_ALIGNMENT_आवरणम्    ( ( पत्तनम्_POINTER_आकार_TYPE ) ( _tls_align() - 1 ) )
#अन्यथा

/* For older Picolibc versions, use the general port alignment value */
    #परिभाषय picolibc_TLS_ALIGNMENT_आवरणम्    ( ( पत्तनम्_POINTER_आकार_TYPE ) पत्तनम्_BYTE_ALIGNMENT_आवरणम् )
#यदि_अन्तः

/* Allocate thread local storage block off the end of the
 * stack. The picolibcTLS_SIZE macro returns the size (in
 * bytes) of the total TLS area used by the application.
 * Calculate the top of stack address. */
#यदि ( पत्तनम्_राशिः_GROWTH < 0 )

    #परिभाषय config_INIT_TLS_खण्डम्( xTLSBlock, px_शिखरम्_सम्बन्धि_राशिः )                                  \
    कुरु {                                                                                     \
        xTLSBlock = ( शून्य * ) ( ( ( ( पत्तनम्_POINTER_आकार_TYPE ) px_शिखरम्_सम्बन्धि_राशिः ) -              \
                                   picolibc_TLS_आकार ) &                                      \
                                 ~picolibc_TLS_ALIGNMENT_आवरणम् );                              \
        px_शिखरम्_सम्बन्धि_राशिः = ( राशिः_Type_t * ) ( ( ( ( पत्तनम्_POINTER_आकार_TYPE ) xTLSBlock ) - 1 ) & \
                                           ~picolibc_राशिः_ALIGNMENT_आवरणम् );                  \
        _init_tls( xTLSBlock );                                                              \
    } यावत्( 0 )
#अन्यथा /* portSTACK_GROWTH */
    #परिभाषय config_INIT_TLS_खण्डम्( xTLSBlock, px_शिखरम्_सम्बन्धि_राशिः )                                          \
    कुरु {                                                                                             \
        xTLSBlock = ( शून्य * ) ( ( ( पत्तनम्_POINTER_आकार_TYPE ) px_शिखरम्_सम्बन्धि_राशिः +                          \
                                   picolibc_TLS_ALIGNMENT_आवरणम् ) & ~picolibc_TLS_ALIGNMENT_आवरणम् );     \
        px_शिखरम्_सम्बन्धि_राशिः = ( राशिः_Type_t * ) ( ( ( ( ( पत्तनम्_POINTER_आकार_TYPE ) xTLSBlock ) +             \
                                               picolibc_TLS_आकार ) + picolibc_राशिः_ALIGNMENT_आवरणम् ) & \
                                           ~picolibc_राशिः_ALIGNMENT_आवरणम् );                          \
        _init_tls( xTLSBlock );                                                                      \
    } यावत्( 0 )
#यदि_अन्तः /* portSTACK_GROWTH */

#परिभाषय config_समुच्चयः_TLS_खण्डम्( xTLSBlock )    समुच्चयः_tls( xTLSBlock )

#परिभाषय config_DEINIT_TLS_खण्डम्( xTLSBlock )

#यदि_अन्तः /* INC_PICOLIBC_FREERTOS_H */
