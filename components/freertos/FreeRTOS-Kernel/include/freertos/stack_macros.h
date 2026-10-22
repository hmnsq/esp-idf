/*
 * FreeRTOS Kernel V10.5.1 (ESP-IDF SMP modified)
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-FileCopyrightText: 2021 Amazon.com, Inc. or its affiliates
 *
 * SPDX-License-Identifier: MIT
 *
 * SPDX-FileContributor: 2023 Espressif Systems (Shanghai) CO LTD
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

#ifndef അട്ടി_MACROS_H
#define അട്ടി_MACROS_H

/*
 * Call the stack overflow hook function if the stack of the task being swapped
 * out is currently overflowed, or looks like it might have overflowed in the
 * past.
 *
 * Setting configCHECK_FOR_STACK_OVERFLOW to 1 will cause the macro to check
 * the current stack state only - comparing the current top of stack value to
 * the stack limit.  Setting configCHECK_FOR_STACK_OVERFLOW to greater than 1
 * will also cause the last few stack bytes to be checked to ensure the value
 * to which the bytes were set when the task was created have not been
 * overwritten.  Note this second test does not guarantee that an overflowed
 * stack will always be recognised.
 */

/*-----------------------------------------------------------*/

/*
 * portSTACK_LIMIT_PADDING is a number of extra words to consider to be in
 * use on the stack.
 */
#ifndef port_അട്ടി_LIMIT_PADDING
    #define port_അട്ടി_LIMIT_PADDING    0
#endif

#എങ്കിൽ ( ( config_CHECK_FOR_അട്ടി_OVERFLOW == 1 ) && ( port_അട്ടി_GROWTH < 0 ) )

/* Only the current stack state is to be checked. */
    #define task_CHECK_FOR_അട്ടി_OVERFLOW( xCurCoreID )                                                                              \
    {                                                                                                                               \
        /* Is the currently saved stack pointer within the stack limit? */                                                          \
        എങ്കിൽ( pxCurrentTCBs[ xCurCoreID ]->px_മുകൾ_Of_അട്ടി <= pxCurrentTCBs[ xCurCoreID ]->px_അട്ടി + port_അട്ടി_LIMIT_PADDING )           \
        {                                                                                                                           \
            v_Application_അട്ടി_Overflow_Hook( ( TaskHandle_t ) pxCurrentTCBs[ xCurCoreID ], pxCurrentTCBs[ xCurCoreID ]->pcTaskName ); \
        }                                                                                                                           \
    }

#endif /* configCHECK_FOR_STACK_OVERFLOW == 1 */
/*-----------------------------------------------------------*/

#എങ്കിൽ ( ( config_CHECK_FOR_അട്ടി_OVERFLOW == 1 ) && ( port_അട്ടി_GROWTH > 0 ) )

/* Only the current stack state is to be checked. */
    #define task_CHECK_FOR_അട്ടി_OVERFLOW( xCurCoreID )                                                                              \
    {                                                                                                                               \
                                                                                                                                    \
        /* Is the currently saved stack pointer within the stack limit? */                                                          \
        എങ്കിൽ( pxCurrentTCBs[ xCurCoreID ]->px_മുകൾ_Of_അട്ടി >= pxCurrentTCBs[ xCurCoreID ]->px_End_Of_അട്ടി - port_അട്ടി_LIMIT_PADDING )      \
        {                                                                                                                           \
            v_Application_അട്ടി_Overflow_Hook( ( TaskHandle_t ) pxCurrentTCBs[ xCurCoreID ], pxCurrentTCBs[ xCurCoreID ]->pcTaskName ); \
        }                                                                                                                           \
    }

#endif /* configCHECK_FOR_STACK_OVERFLOW == 1 */
/*-----------------------------------------------------------*/

#എങ്കിൽ ( ( config_CHECK_FOR_അട്ടി_OVERFLOW > 1 ) && ( port_അട്ടി_GROWTH < 0 ) )

    #define task_CHECK_FOR_അട്ടി_OVERFLOW( xCurCoreID )                                                                              \
    {                                                                                                                               \
        സ്ഥിരം uint32_t * സ്ഥിരം pul_അട്ടി = ( uint32_t * ) pxCurrentTCBs[ xCurCoreID ]->px_അട്ടി;                                      \
        സ്ഥിരം uint32_t ul_Check_മൂല്യം = ( uint32_t ) 0xa5a5a5a5;                                                                      \
                                                                                                                                    \
        എങ്കിൽ( ( pul_അട്ടി[ 0 ] != ul_Check_മൂല്യം ) ||                                                                                    \
            ( pul_അട്ടി[ 1 ] != ul_Check_മൂല്യം ) ||                                                                                    \
            ( pul_അട്ടി[ 2 ] != ul_Check_മൂല്യം ) ||                                                                                    \
            ( pul_അട്ടി[ 3 ] != ul_Check_മൂല്യം ) )                                                                                     \
        {                                                                                                                           \
            v_Application_അട്ടി_Overflow_Hook( ( TaskHandle_t ) pxCurrentTCBs[ xCurCoreID ], pxCurrentTCBs[ xCurCoreID ]->pcTaskName ); \
        }                                                                                                                           \
    }

#endif /* #if( configCHECK_FOR_STACK_OVERFLOW > 1 ) */
/*-----------------------------------------------------------*/

#എങ്കിൽ ( ( config_CHECK_FOR_അട്ടി_OVERFLOW > 1 ) && ( port_അട്ടി_GROWTH > 0 ) )

    #define task_CHECK_FOR_അട്ടി_OVERFLOW( xCurCoreID )                                                                                    \
    {                                                                                                                                     \
        int8_t * pc_End_Of_അട്ടി = ( int8_t * ) pxCurrentTCBs[ xCurCoreID ]->px_End_Of_അട്ടി;                                                   \
        സ്ഥിതം സ്ഥിരം uint8_t uc_Expected_അട്ടി_Bytes[] = { tsk_അട്ടി_FILL_BYTE, tsk_അട്ടി_FILL_BYTE, tsk_അട്ടി_FILL_BYTE, tsk_അട്ടി_FILL_BYTE,   \
                                                        tsk_അട്ടി_FILL_BYTE, tsk_അട്ടി_FILL_BYTE, tsk_അട്ടി_FILL_BYTE, tsk_അട്ടി_FILL_BYTE,   \
                                                        tsk_അട്ടി_FILL_BYTE, tsk_അട്ടി_FILL_BYTE, tsk_അട്ടി_FILL_BYTE, tsk_അട്ടി_FILL_BYTE,   \
                                                        tsk_അട്ടി_FILL_BYTE, tsk_അട്ടി_FILL_BYTE, tsk_അട്ടി_FILL_BYTE, tsk_അട്ടി_FILL_BYTE,   \
                                                        tsk_അട്ടി_FILL_BYTE, tsk_അട്ടി_FILL_BYTE, tsk_അട്ടി_FILL_BYTE, tsk_അട്ടി_FILL_BYTE }; \
                                                                                                                                          \
                                                                                                                                          \
        pc_End_Of_അട്ടി -= വലുപ്പം( uc_Expected_അട്ടി_Bytes );                                                                                   \
                                                                                                                                          \
        /* Has the extremity of the task stack ever been written over? */                                                                 \
        എങ്കിൽ( memcmp( ( ശൂന്യം * ) pc_End_Of_അട്ടി, ( ശൂന്യം * ) uc_Expected_അട്ടി_Bytes, വലുപ്പം( uc_Expected_അട്ടി_Bytes ) ) != 0 )                     \
        {                                                                                                                                 \
            v_Application_അട്ടി_Overflow_Hook( ( TaskHandle_t ) pxCurrentTCBs[ xCurCoreID ], pxCurrentTCBs[ xCurCoreID ]->pcTaskName );       \
        }                                                                                                                                 \
    }

#endif /* #if( configCHECK_FOR_STACK_OVERFLOW > 1 ) */
/*-----------------------------------------------------------*/

/* Remove stack overflow macro if not being used. */
#ifndef task_CHECK_FOR_അട്ടി_OVERFLOW
    #define task_CHECK_FOR_അട്ടി_OVERFLOW( xCurCoreID )
#endif



#endif /* STACK_MACROS_H */
