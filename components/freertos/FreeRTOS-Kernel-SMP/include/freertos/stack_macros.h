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

#ifndef স্তূপ_MACROS_H
#define স্তূপ_MACROS_H

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
#ifndef port_স্তূপ_LIMIT_PADDING
    #define port_স্তূপ_LIMIT_PADDING    0
#endif

#যদি ( ( config_CHECK_FOR_স্তূপ_OVERFLOW == 1 ) && ( port_স্তূপ_GROWTH < 0 ) )

/* Only the current stack state is to be checked. */
    #define task_CHECK_FOR_স্তূপ_OVERFLOW()                                                      \
    কৰা {                                                                                        \
        /* Is the currently saved stack pointer within the stack limit? */                      \
        যদি( pxCurrentTCB->px_শীৰ্ষ_Of_স্তূপ <= pxCurrentTCB->px_স্তূপ + port_স্তূপ_LIMIT_PADDING )     \
        {                                                                                       \
            ৱর্ণ * pcOverflowTaskName = pxCurrentTCB->pcTaskName;                               \
            v_Application_স্তূপ_Overflow_Hook( ( TaskHandle_t ) pxCurrentTCB, pcOverflowTaskName ); \
        }                                                                                       \
    } যেতিয়ালৈ( 0 )

#endif /* configCHECK_FOR_STACK_OVERFLOW == 1 */
/*-----------------------------------------------------------*/

#যদি ( ( config_CHECK_FOR_স্তূপ_OVERFLOW == 1 ) && ( port_স্তূপ_GROWTH > 0 ) )

/* Only the current stack state is to be checked. */
    #define task_CHECK_FOR_স্তূপ_OVERFLOW()                                                       \
    কৰা {                                                                                         \
                                                                                                 \
        /* Is the currently saved stack pointer within the stack limit? */                       \
        যদি( pxCurrentTCB->px_শীৰ্ষ_Of_স্তূপ >= pxCurrentTCB->px_End_Of_স্তূপ - port_স্তূপ_LIMIT_PADDING ) \
        {                                                                                        \
            ৱর্ণ * pcOverflowTaskName = pxCurrentTCB->pcTaskName;                                \
            v_Application_স্তূপ_Overflow_Hook( ( TaskHandle_t ) pxCurrentTCB, pcOverflowTaskName );  \
        }                                                                                        \
    } যেতিয়ালৈ( 0 )

#endif /* configCHECK_FOR_STACK_OVERFLOW == 1 */
/*-----------------------------------------------------------*/

#যদি ( ( config_CHECK_FOR_স্তূপ_OVERFLOW > 1 ) && ( port_স্তূপ_GROWTH < 0 ) )

    #define task_CHECK_FOR_স্তূপ_OVERFLOW()                                                      \
    কৰা {                                                                                        \
        ধ্রুৱ uint32_t * ধ্রুৱ pul_স্তূপ = ( uint32_t * ) pxCurrentTCB->px_স্তূপ;                 \
        ধ্রুৱ uint32_t ul_Check_মান = ( uint32_t ) 0xa5a5a5a5U;                                 \
                                                                                                \
        যদি( ( pul_স্তূপ[ 0 ] != ul_Check_মান ) ||                                                \
            ( pul_স্তূপ[ 1 ] != ul_Check_মান ) ||                                                \
            ( pul_স্তূপ[ 2 ] != ul_Check_মান ) ||                                                \
            ( pul_স্তূপ[ 3 ] != ul_Check_মান ) )                                                 \
        {                                                                                       \
            ৱর্ণ * pcOverflowTaskName = pxCurrentTCB->pcTaskName;                               \
            v_Application_স্তূপ_Overflow_Hook( ( TaskHandle_t ) pxCurrentTCB, pcOverflowTaskName ); \
        }                                                                                       \
    } যেতিয়ালৈ( 0 )

#endif /* #if( configCHECK_FOR_STACK_OVERFLOW > 1 ) */
/*-----------------------------------------------------------*/

#যদি ( ( config_CHECK_FOR_স্তূপ_OVERFLOW > 1 ) && ( port_স্তূপ_GROWTH > 0 ) )

    #define task_CHECK_FOR_স্তূপ_OVERFLOW()                                                                                                \
    কৰা {                                                                                                                                  \
        int8_t * pc_End_Of_স্তূপ = ( int8_t * ) pxCurrentTCB->px_End_Of_স্তূপ;                                                                  \
        স্থির ধ্রুৱ uint8_t uc_Expected_স্তূপ_Bytes[] = { tsk_স্তূপ_FILL_BYTE, tsk_স্তূপ_FILL_BYTE, tsk_স্তূপ_FILL_BYTE, tsk_স্তূপ_FILL_BYTE,   \
                                                        tsk_স্তূপ_FILL_BYTE, tsk_স্তূপ_FILL_BYTE, tsk_স্তূপ_FILL_BYTE, tsk_স্তূপ_FILL_BYTE,   \
                                                        tsk_স্তূপ_FILL_BYTE, tsk_স্তূপ_FILL_BYTE, tsk_স্তূপ_FILL_BYTE, tsk_স্তূপ_FILL_BYTE,   \
                                                        tsk_স্তূপ_FILL_BYTE, tsk_স্তূপ_FILL_BYTE, tsk_স্তূপ_FILL_BYTE, tsk_স্তূপ_FILL_BYTE,   \
                                                        tsk_স্তূপ_FILL_BYTE, tsk_স্তূপ_FILL_BYTE, tsk_স্তূপ_FILL_BYTE, tsk_স্তূপ_FILL_BYTE }; \
                                                                                                                                          \
                                                                                                                                          \
        pc_End_Of_স্তূপ -= পরিমাণ( uc_Expected_স্তূপ_Bytes );                                                                                   \
                                                                                                                                          \
        /* Has the extremity of the task stack ever been written over? */                                                                 \
        যদি( memcmp( ( শূন্য * ) pc_End_Of_স্তূপ, ( শূন্য * ) uc_Expected_স্তূপ_Bytes, পরিমাণ( uc_Expected_স্তূপ_Bytes ) ) != 0 )                     \
        {                                                                                                                                 \
            ৱর্ণ * pcOverflowTaskName = pxCurrentTCB->pcTaskName;                                                                         \
            v_Application_স্তূপ_Overflow_Hook( ( TaskHandle_t ) pxCurrentTCB, pcOverflowTaskName );                                           \
        }                                                                                                                                 \
    } যেতিয়ালৈ( 0 )

#endif /* #if( configCHECK_FOR_STACK_OVERFLOW > 1 ) */
/*-----------------------------------------------------------*/

/* Remove stack overflow macro if not being used. */
#ifndef task_CHECK_FOR_স্তূপ_OVERFLOW
    #define task_CHECK_FOR_স্তূপ_OVERFLOW()
#endif



#endif /* STACK_MACROS_H */
