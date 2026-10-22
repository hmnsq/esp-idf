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

#ifndef ଗଦା_MACROS_H
#define ଗଦା_MACROS_H

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
#ifndef port_ଗଦା_LIMIT_PADDING
    #define port_ଗଦା_LIMIT_PADDING    0
#endif

#ଯଦି ( ( config_CHECK_FOR_ଗଦା_OVERFLOW == 1 ) && ( port_ଗଦା_GROWTH < 0 ) )

/* Only the current stack state is to be checked. */
    #define task_CHECK_FOR_ଗଦା_OVERFLOW( xCurCoreID )                                                                              \
    {                                                                                                                               \
        /* Is the currently saved stack pointer within the stack limit? */                                                          \
        ଯଦି( pxCurrentTCBs[ xCurCoreID ]->px_ଶୀର୍ଷ_Of_ଗଦା <= pxCurrentTCBs[ xCurCoreID ]->px_ଗଦା + port_ଗଦା_LIMIT_PADDING )           \
        {                                                                                                                           \
            v_Application_ଗଦା_Overflow_Hook( ( TaskHandle_t ) pxCurrentTCBs[ xCurCoreID ], pxCurrentTCBs[ xCurCoreID ]->pcTaskName ); \
        }                                                                                                                           \
    }

#endif /* configCHECK_FOR_STACK_OVERFLOW == 1 */
/*-----------------------------------------------------------*/

#ଯଦି ( ( config_CHECK_FOR_ଗଦା_OVERFLOW == 1 ) && ( port_ଗଦା_GROWTH > 0 ) )

/* Only the current stack state is to be checked. */
    #define task_CHECK_FOR_ଗଦା_OVERFLOW( xCurCoreID )                                                                              \
    {                                                                                                                               \
                                                                                                                                    \
        /* Is the currently saved stack pointer within the stack limit? */                                                          \
        ଯଦି( pxCurrentTCBs[ xCurCoreID ]->px_ଶୀର୍ଷ_Of_ଗଦା >= pxCurrentTCBs[ xCurCoreID ]->px_End_Of_ଗଦା - port_ଗଦା_LIMIT_PADDING )      \
        {                                                                                                                           \
            v_Application_ଗଦା_Overflow_Hook( ( TaskHandle_t ) pxCurrentTCBs[ xCurCoreID ], pxCurrentTCBs[ xCurCoreID ]->pcTaskName ); \
        }                                                                                                                           \
    }

#endif /* configCHECK_FOR_STACK_OVERFLOW == 1 */
/*-----------------------------------------------------------*/

#ଯଦି ( ( config_CHECK_FOR_ଗଦା_OVERFLOW > 1 ) && ( port_ଗଦା_GROWTH < 0 ) )

    #define task_CHECK_FOR_ଗଦା_OVERFLOW( xCurCoreID )                                                                              \
    {                                                                                                                               \
        ଅଚଳ uint32_t * ଅଚଳ pul_ଗଦା = ( uint32_t * ) pxCurrentTCBs[ xCurCoreID ]->px_ଗଦା;                                      \
        ଅଚଳ uint32_t ul_Check_ମୂଲ୍ୟ = ( uint32_t ) 0xa5a5a5a5;                                                                      \
                                                                                                                                    \
        ଯଦି( ( pul_ଗଦା[ 0 ] != ul_Check_ମୂଲ୍ୟ ) ||                                                                                    \
            ( pul_ଗଦା[ 1 ] != ul_Check_ମୂଲ୍ୟ ) ||                                                                                    \
            ( pul_ଗଦା[ 2 ] != ul_Check_ମୂଲ୍ୟ ) ||                                                                                    \
            ( pul_ଗଦା[ 3 ] != ul_Check_ମୂଲ୍ୟ ) )                                                                                     \
        {                                                                                                                           \
            v_Application_ଗଦା_Overflow_Hook( ( TaskHandle_t ) pxCurrentTCBs[ xCurCoreID ], pxCurrentTCBs[ xCurCoreID ]->pcTaskName ); \
        }                                                                                                                           \
    }

#endif /* #if( configCHECK_FOR_STACK_OVERFLOW > 1 ) */
/*-----------------------------------------------------------*/

#ଯଦି ( ( config_CHECK_FOR_ଗଦା_OVERFLOW > 1 ) && ( port_ଗଦା_GROWTH > 0 ) )

    #define task_CHECK_FOR_ଗଦା_OVERFLOW( xCurCoreID )                                                                                    \
    {                                                                                                                                     \
        int8_t * pc_End_Of_ଗଦା = ( int8_t * ) pxCurrentTCBs[ xCurCoreID ]->px_End_Of_ଗଦା;                                                   \
        ସ୍ଥିର ଅଚଳ uint8_t uc_Expected_ଗଦା_Bytes[] = { tsk_ଗଦା_FILL_BYTE, tsk_ଗଦା_FILL_BYTE, tsk_ଗଦା_FILL_BYTE, tsk_ଗଦା_FILL_BYTE,   \
                                                        tsk_ଗଦା_FILL_BYTE, tsk_ଗଦା_FILL_BYTE, tsk_ଗଦା_FILL_BYTE, tsk_ଗଦା_FILL_BYTE,   \
                                                        tsk_ଗଦା_FILL_BYTE, tsk_ଗଦା_FILL_BYTE, tsk_ଗଦା_FILL_BYTE, tsk_ଗଦା_FILL_BYTE,   \
                                                        tsk_ଗଦା_FILL_BYTE, tsk_ଗଦା_FILL_BYTE, tsk_ଗଦା_FILL_BYTE, tsk_ଗଦା_FILL_BYTE,   \
                                                        tsk_ଗଦା_FILL_BYTE, tsk_ଗଦା_FILL_BYTE, tsk_ଗଦା_FILL_BYTE, tsk_ଗଦା_FILL_BYTE }; \
                                                                                                                                          \
                                                                                                                                          \
        pc_End_Of_ଗଦା -= ଆକାର( uc_Expected_ଗଦା_Bytes );                                                                                   \
                                                                                                                                          \
        /* Has the extremity of the task stack ever been written over? */                                                                 \
        ଯଦି( memcmp( ( ଶୂନ୍ୟ * ) pc_End_Of_ଗଦା, ( ଶୂନ୍ୟ * ) uc_Expected_ଗଦା_Bytes, ଆକାର( uc_Expected_ଗଦା_Bytes ) ) != 0 )                     \
        {                                                                                                                                 \
            v_Application_ଗଦା_Overflow_Hook( ( TaskHandle_t ) pxCurrentTCBs[ xCurCoreID ], pxCurrentTCBs[ xCurCoreID ]->pcTaskName );       \
        }                                                                                                                                 \
    }

#endif /* #if( configCHECK_FOR_STACK_OVERFLOW > 1 ) */
/*-----------------------------------------------------------*/

/* Remove stack overflow macro if not being used. */
#ifndef task_CHECK_FOR_ଗଦା_OVERFLOW
    #define task_CHECK_FOR_ଗଦା_OVERFLOW( xCurCoreID )
#endif



#endif /* STACK_MACROS_H */
