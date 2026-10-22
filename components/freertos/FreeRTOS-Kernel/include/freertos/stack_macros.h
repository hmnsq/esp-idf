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

#ifndef تيندنن_MACROS_H
#define تيندنن_MACROS_H

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
#ifndef port_تيندنن_LIMIT_PADDING
    #define port_تيندنن_LIMIT_PADDING    0
#endif

#إذا ( ( config_CHECK_FOR_تيندنن_OVERFLOW == 1 ) && ( port_تيندنن_GROWTH < 0 ) )

/* Only the current stack state is to be checked. */
    #define task_CHECK_FOR_تيندنن_OVERFLOW( xCurCoreID )                                                                              \
    {                                                                                                                               \
        /* Is the currently saved stack pointer within the stack limit? */                                                          \
        إذا( pxCurrentTCBs[ xCurCoreID ]->px_اتس_Of_تيندنن <= pxCurrentTCBs[ xCurCoreID ]->px_تيندنن + port_تيندنن_LIMIT_PADDING )           \
        {                                                                                                                           \
            v_Application_تيندنن_Overflow_Hook( ( TaskHandle_t ) pxCurrentTCBs[ xCurCoreID ], pxCurrentTCBs[ xCurCoreID ]->pcTaskName ); \
        }                                                                                                                           \
    }

#endif /* configCHECK_FOR_STACK_OVERFLOW == 1 */
/*-----------------------------------------------------------*/

#إذا ( ( config_CHECK_FOR_تيندنن_OVERFLOW == 1 ) && ( port_تيندنن_GROWTH > 0 ) )

/* Only the current stack state is to be checked. */
    #define task_CHECK_FOR_تيندنن_OVERFLOW( xCurCoreID )                                                                              \
    {                                                                                                                               \
                                                                                                                                    \
        /* Is the currently saved stack pointer within the stack limit? */                                                          \
        إذا( pxCurrentTCBs[ xCurCoreID ]->px_اتس_Of_تيندنن >= pxCurrentTCBs[ xCurCoreID ]->px_End_Of_تيندنن - port_تيندنن_LIMIT_PADDING )      \
        {                                                                                                                           \
            v_Application_تيندنن_Overflow_Hook( ( TaskHandle_t ) pxCurrentTCBs[ xCurCoreID ], pxCurrentTCBs[ xCurCoreID ]->pcTaskName ); \
        }                                                                                                                           \
    }

#endif /* configCHECK_FOR_STACK_OVERFLOW == 1 */
/*-----------------------------------------------------------*/

#إذا ( ( config_CHECK_FOR_تيندنن_OVERFLOW > 1 ) && ( port_تيندنن_GROWTH < 0 ) )

    #define task_CHECK_FOR_تيندنن_OVERFLOW( xCurCoreID )                                                                              \
    {                                                                                                                               \
        ثابت uint32_t * ثابت pul_تيندنن = ( uint32_t * ) pxCurrentTCBs[ xCurCoreID ]->px_تيندنن;                                      \
        ثابت uint32_t ul_Check_نيلاي = ( uint32_t ) 0xa5a5a5a5;                                                                      \
                                                                                                                                    \
        إذا( ( pul_تيندنن[ 0 ] != ul_Check_نيلاي ) ||                                                                                    \
            ( pul_تيندنن[ 1 ] != ul_Check_نيلاي ) ||                                                                                    \
            ( pul_تيندنن[ 2 ] != ul_Check_نيلاي ) ||                                                                                    \
            ( pul_تيندنن[ 3 ] != ul_Check_نيلاي ) )                                                                                     \
        {                                                                                                                           \
            v_Application_تيندنن_Overflow_Hook( ( TaskHandle_t ) pxCurrentTCBs[ xCurCoreID ], pxCurrentTCBs[ xCurCoreID ]->pcTaskName ); \
        }                                                                                                                           \
    }

#endif /* #if( configCHECK_FOR_STACK_OVERFLOW > 1 ) */
/*-----------------------------------------------------------*/

#إذا ( ( config_CHECK_FOR_تيندنن_OVERFLOW > 1 ) && ( port_تيندنن_GROWTH > 0 ) )

    #define task_CHECK_FOR_تيندنن_OVERFLOW( xCurCoreID )                                                                                    \
    {                                                                                                                                     \
        int8_t * pc_End_Of_تيندنن = ( int8_t * ) pxCurrentTCBs[ xCurCoreID ]->px_End_Of_تيندنن;                                                   \
        ساكن ثابت uint8_t uc_Expected_تيندنن_Bytes[] = { tsk_تيندنن_FILL_BYTE, tsk_تيندنن_FILL_BYTE, tsk_تيندنن_FILL_BYTE, tsk_تيندنن_FILL_BYTE,   \
                                                        tsk_تيندنن_FILL_BYTE, tsk_تيندنن_FILL_BYTE, tsk_تيندنن_FILL_BYTE, tsk_تيندنن_FILL_BYTE,   \
                                                        tsk_تيندنن_FILL_BYTE, tsk_تيندنن_FILL_BYTE, tsk_تيندنن_FILL_BYTE, tsk_تيندنن_FILL_BYTE,   \
                                                        tsk_تيندنن_FILL_BYTE, tsk_تيندنن_FILL_BYTE, tsk_تيندنن_FILL_BYTE, tsk_تيندنن_FILL_BYTE,   \
                                                        tsk_تيندنن_FILL_BYTE, tsk_تيندنن_FILL_BYTE, tsk_تيندنن_FILL_BYTE, tsk_تيندنن_FILL_BYTE }; \
                                                                                                                                          \
                                                                                                                                          \
        pc_End_Of_تيندنن -= حجم( uc_Expected_تيندنن_Bytes );                                                                                   \
                                                                                                                                          \
        /* Has the extremity of the task stack ever been written over? */                                                                 \
        إذا( memcmp( ( فراغ * ) pc_End_Of_تيندنن, ( فراغ * ) uc_Expected_تيندنن_Bytes, حجم( uc_Expected_تيندنن_Bytes ) ) != 0 )                     \
        {                                                                                                                                 \
            v_Application_تيندنن_Overflow_Hook( ( TaskHandle_t ) pxCurrentTCBs[ xCurCoreID ], pxCurrentTCBs[ xCurCoreID ]->pcTaskName );       \
        }                                                                                                                                 \
    }

#endif /* #if( configCHECK_FOR_STACK_OVERFLOW > 1 ) */
/*-----------------------------------------------------------*/

/* Remove stack overflow macro if not being used. */
#ifndef task_CHECK_FOR_تيندنن_OVERFLOW
    #define task_CHECK_FOR_تيندنن_OVERFLOW( xCurCoreID )
#endif



#endif /* STACK_MACROS_H */
