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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ INC_PICOLIBC_FREERTOS_H
#ವ್ಯಾಖ್ಯಾನಿಸು INC_PICOLIBC_FREERTOS_H

/* Use picolibc TLS support to allocate space for __thread variables,
 * initialize them at thread creation and set the TLS context at
 * thread switch time.
 *
 * See the picolibc TLS docs:
 * https://github.com/picolibc/picolibc/blob/main/doc/tls.md
 * for additional information. */

#ಅನ್ತರ್ಭಾವ <picotls.h>

#ವ್ಯಾಖ್ಯಾನಿಸು configUSE_C_RUNTIME_TLS_SUPPORT    1

#ವ್ಯಾಖ್ಯಾನಿಸು config_TLS_ಖಂಡ_TYPE               ಶೂನ್ಯ *

#ವ್ಯಾಖ್ಯಾನಿಸು picolibc_TLS_ಗಾತ್ರ                   ( ( ಪೋರ್ಟ್_ಸೂಚಕ_ಗಾತ್ರ_TYPE ) tls_ಗಾತ್ರ() )
#ವ್ಯಾಖ್ಯಾನಿಸು picolibc_ರಾಶಿ_ALIGNMENT_ಮುಖವಾಡ       ( ( ಪೋರ್ಟ್_ಸೂಚಕ_ಗಾತ್ರ_TYPE ) ಪೋರ್ಟ್_BYTE_ALIGNMENT_ಮುಖವಾಡ )

#ಆದರೆ __PICOLIBC_MAJOR__ > 1 || __PICOLIBC_MINOR__ >= 8

/* Picolibc 1.8 and newer have explicit alignment values provided
 * by the _tls_align() inline */
    #ವ್ಯಾಖ್ಯಾನಿಸು picolibc_TLS_ALIGNMENT_ಮುಖವಾಡ    ( ( ಪೋರ್ಟ್_ಸೂಚಕ_ಗಾತ್ರ_TYPE ) ( _tls_align() - 1 ) )
#ಇಲ್ಲದಿದ್ದರೆ

/* For older Picolibc versions, use the general port alignment value */
    #ವ್ಯಾಖ್ಯಾನಿಸು picolibc_TLS_ALIGNMENT_ಮುಖವಾಡ    ( ( ಪೋರ್ಟ್_ಸೂಚಕ_ಗಾತ್ರ_TYPE ) ಪೋರ್ಟ್_BYTE_ALIGNMENT_ಮುಖವಾಡ )
#ಆದರೆ_ಅಂತ್ಯ

/* Allocate thread local storage block off the end of the
 * stack. The picolibcTLS_SIZE macro returns the size (in
 * bytes) of the total TLS area used by the application.
 * Calculate the top of stack address. */
#ಆದರೆ ( ಪೋರ್ಟ್_ರಾಶಿ_GROWTH < 0 )

    #ವ್ಯಾಖ್ಯಾನಿಸು config_INIT_TLS_ಖಂಡ( xTLSBlock, px_ತುದಿ_ದ_ರಾಶಿ )                                  \
    ಮಾಡು {                                                                                     \
        xTLSBlock = ( ಶೂನ್ಯ * ) ( ( ( ( ಪೋರ್ಟ್_ಸೂಚಕ_ಗಾತ್ರ_TYPE ) px_ತುದಿ_ದ_ರಾಶಿ ) -              \
                                   picolibc_TLS_ಗಾತ್ರ ) &                                      \
                                 ~picolibc_TLS_ALIGNMENT_ಮುಖವಾಡ );                              \
        px_ತುದಿ_ದ_ರಾಶಿ = ( ರಾಶಿ_Type_t * ) ( ( ( ( ಪೋರ್ಟ್_ಸೂಚಕ_ಗಾತ್ರ_TYPE ) xTLSBlock ) - 1 ) & \
                                           ~picolibc_ರಾಶಿ_ALIGNMENT_ಮುಖವಾಡ );                  \
        _init_tls( xTLSBlock );                                                              \
    } ಆಗ( 0 )
#ಇಲ್ಲದಿದ್ದರೆ /* portSTACK_GROWTH */
    #ವ್ಯಾಖ್ಯಾನಿಸು config_INIT_TLS_ಖಂಡ( xTLSBlock, px_ತುದಿ_ದ_ರಾಶಿ )                                          \
    ಮಾಡು {                                                                                             \
        xTLSBlock = ( ಶೂನ್ಯ * ) ( ( ( ಪೋರ್ಟ್_ಸೂಚಕ_ಗಾತ್ರ_TYPE ) px_ತುದಿ_ದ_ರಾಶಿ +                          \
                                   picolibc_TLS_ALIGNMENT_ಮುಖವಾಡ ) & ~picolibc_TLS_ALIGNMENT_ಮುಖವಾಡ );     \
        px_ತುದಿ_ದ_ರಾಶಿ = ( ರಾಶಿ_Type_t * ) ( ( ( ( ( ಪೋರ್ಟ್_ಸೂಚಕ_ಗಾತ್ರ_TYPE ) xTLSBlock ) +             \
                                               picolibc_TLS_ಗಾತ್ರ ) + picolibc_ರಾಶಿ_ALIGNMENT_ಮುಖವಾಡ ) & \
                                           ~picolibc_ರಾಶಿ_ALIGNMENT_ಮುಖವಾಡ );                          \
        _init_tls( xTLSBlock );                                                                      \
    } ಆಗ( 0 )
#ಆದರೆ_ಅಂತ್ಯ /* portSTACK_GROWTH */

#ವ್ಯಾಖ್ಯಾನಿಸು config_ಗಣ_TLS_ಖಂಡ( xTLSBlock )    ಗಣ_tls( xTLSBlock )

#ವ್ಯಾಖ್ಯಾನಿಸು config_DEINIT_TLS_ಖಂಡ( xTLSBlock )

#ಆದರೆ_ಅಂತ್ಯ /* INC_PICOLIBC_FREERTOS_H */
