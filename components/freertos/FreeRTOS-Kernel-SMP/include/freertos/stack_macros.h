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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਢੇਰ_MACROS_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਢੇਰ_MACROS_H

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
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_ਢੇਰ_ਹੱਦਬੰਦੀ_PADDING
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਢੇਰ_ਹੱਦਬੰਦੀ_PADDING    0
#ਜੇ_ਅੰਤ

#ਜੇ ( ( config_CHECK_FOR_ਢੇਰ_ਉਛਾਲ == 1 ) && ( ਪੋਰਟ_ਢੇਰ_GROWTH < 0 ) )

/* Only the current stack state is to be checked. */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ task_CHECK_FOR_ਢੇਰ_ਉਛਾਲ()                                                      \
    ਕਰੋ {                                                                                        \
        /* Is the currently saved stack pointer within the stack limit? */                      \
        ਜੇ( pxCurrentTCB->px_ਸਿਖਰ_ਦਾ_ਢੇਰ <= pxCurrentTCB->px_ਢੇਰ + ਪੋਰਟ_ਢੇਰ_ਹੱਦਬੰਦੀ_PADDING )     \
        {                                                                                       \
            ਅੱਖਰ * pc_ਉਛਾਲ_Task_Name = pxCurrentTCB->pcTaskName;                               \
            v_Application_ਢੇਰ_ਉਛਾਲ_Hook( ( TaskHandle_t ) pxCurrentTCB, pc_ਉਛਾਲ_Task_Name ); \
        }                                                                                       \
    } ਜਦੋਂ( 0 )

#ਜੇ_ਅੰਤ /* configCHECK_FOR_STACK_OVERFLOW == 1 */
/*-----------------------------------------------------------*/

#ਜੇ ( ( config_CHECK_FOR_ਢੇਰ_ਉਛਾਲ == 1 ) && ( ਪੋਰਟ_ਢੇਰ_GROWTH > 0 ) )

/* Only the current stack state is to be checked. */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ task_CHECK_FOR_ਢੇਰ_ਉਛਾਲ()                                                       \
    ਕਰੋ {                                                                                         \
                                                                                                 \
        /* Is the currently saved stack pointer within the stack limit? */                       \
        ਜੇ( pxCurrentTCB->px_ਸਿਖਰ_ਦਾ_ਢੇਰ >= pxCurrentTCB->px_ਸਿਰਾ_ਦਾ_ਢੇਰ - ਪੋਰਟ_ਢੇਰ_ਹੱਦਬੰਦੀ_PADDING ) \
        {                                                                                        \
            ਅੱਖਰ * pc_ਉਛਾਲ_Task_Name = pxCurrentTCB->pcTaskName;                                \
            v_Application_ਢੇਰ_ਉਛਾਲ_Hook( ( TaskHandle_t ) pxCurrentTCB, pc_ਉਛਾਲ_Task_Name );  \
        }                                                                                        \
    } ਜਦੋਂ( 0 )

#ਜੇ_ਅੰਤ /* configCHECK_FOR_STACK_OVERFLOW == 1 */
/*-----------------------------------------------------------*/

#ਜੇ ( ( config_CHECK_FOR_ਢੇਰ_ਉਛਾਲ > 1 ) && ( ਪੋਰਟ_ਢੇਰ_GROWTH < 0 ) )

    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ task_CHECK_FOR_ਢੇਰ_ਉਛਾਲ()                                                      \
    ਕਰੋ {                                                                                        \
        ਅਚਲ uint32_t * ਅਚਲ pul_ਢੇਰ = ( uint32_t * ) pxCurrentTCB->px_ਢੇਰ;                 \
        ਅਚਲ uint32_t ul_Check_ਮੁੱਲ = ( uint32_t ) 0xa5a5a5a5U;                                 \
                                                                                                \
        ਜੇ( ( pul_ਢੇਰ[ 0 ] != ul_Check_ਮੁੱਲ ) ||                                                \
            ( pul_ਢੇਰ[ 1 ] != ul_Check_ਮੁੱਲ ) ||                                                \
            ( pul_ਢੇਰ[ 2 ] != ul_Check_ਮੁੱਲ ) ||                                                \
            ( pul_ਢੇਰ[ 3 ] != ul_Check_ਮੁੱਲ ) )                                                 \
        {                                                                                       \
            ਅੱਖਰ * pc_ਉਛਾਲ_Task_Name = pxCurrentTCB->pcTaskName;                               \
            v_Application_ਢੇਰ_ਉਛਾਲ_Hook( ( TaskHandle_t ) pxCurrentTCB, pc_ਉਛਾਲ_Task_Name ); \
        }                                                                                       \
    } ਜਦੋਂ( 0 )

#ਜੇ_ਅੰਤ /* #if( configCHECK_FOR_STACK_OVERFLOW > 1 ) */
/*-----------------------------------------------------------*/

#ਜੇ ( ( config_CHECK_FOR_ਢੇਰ_ਉਛਾਲ > 1 ) && ( ਪੋਰਟ_ਢੇਰ_GROWTH > 0 ) )

    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ task_CHECK_FOR_ਢੇਰ_ਉਛਾਲ()                                                                                                \
    ਕਰੋ {                                                                                                                                  \
        int8_t * pc_ਸਿਰਾ_ਦਾ_ਢੇਰ = ( int8_t * ) pxCurrentTCB->px_ਸਿਰਾ_ਦਾ_ਢੇਰ;                                                                  \
        ਸਥਿਰ ਅਚਲ uint8_t uc_Expected_ਢੇਰ_ਬਾਈਟਾਂ[] = { tsk_ਢੇਰ_FILL_BYTE, tsk_ਢੇਰ_FILL_BYTE, tsk_ਢੇਰ_FILL_BYTE, tsk_ਢੇਰ_FILL_BYTE,   \
                                                        tsk_ਢੇਰ_FILL_BYTE, tsk_ਢੇਰ_FILL_BYTE, tsk_ਢੇਰ_FILL_BYTE, tsk_ਢੇਰ_FILL_BYTE,   \
                                                        tsk_ਢੇਰ_FILL_BYTE, tsk_ਢੇਰ_FILL_BYTE, tsk_ਢੇਰ_FILL_BYTE, tsk_ਢੇਰ_FILL_BYTE,   \
                                                        tsk_ਢੇਰ_FILL_BYTE, tsk_ਢੇਰ_FILL_BYTE, tsk_ਢੇਰ_FILL_BYTE, tsk_ਢੇਰ_FILL_BYTE,   \
                                                        tsk_ਢੇਰ_FILL_BYTE, tsk_ਢੇਰ_FILL_BYTE, tsk_ਢੇਰ_FILL_BYTE, tsk_ਢੇਰ_FILL_BYTE }; \
                                                                                                                                          \
                                                                                                                                          \
        pc_ਸਿਰਾ_ਦਾ_ਢੇਰ -= ਆਕਾਰ( uc_Expected_ਢੇਰ_ਬਾਈਟਾਂ );                                                                                   \
                                                                                                                                          \
        /* Has the extremity of the task stack ever been written over? */                                                                 \
        ਜੇ( memcmp( ( ਖਾਲੀ * ) pc_ਸਿਰਾ_ਦਾ_ਢੇਰ, ( ਖਾਲੀ * ) uc_Expected_ਢੇਰ_ਬਾਈਟਾਂ, ਆਕਾਰ( uc_Expected_ਢੇਰ_ਬਾਈਟਾਂ ) ) != 0 )                     \
        {                                                                                                                                 \
            ਅੱਖਰ * pc_ਉਛਾਲ_Task_Name = pxCurrentTCB->pcTaskName;                                                                         \
            v_Application_ਢੇਰ_ਉਛਾਲ_Hook( ( TaskHandle_t ) pxCurrentTCB, pc_ਉਛਾਲ_Task_Name );                                           \
        }                                                                                                                                 \
    } ਜਦੋਂ( 0 )

#ਜੇ_ਅੰਤ /* #if( configCHECK_FOR_STACK_OVERFLOW > 1 ) */
/*-----------------------------------------------------------*/

/* Remove stack overflow macro if not being used. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ task_CHECK_FOR_ਢੇਰ_ਉਛਾਲ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ task_CHECK_FOR_ਢੇਰ_ਉਛਾਲ()
#ਜੇ_ਅੰਤ



#ਜੇ_ਅੰਤ /* STACK_MACROS_H */
