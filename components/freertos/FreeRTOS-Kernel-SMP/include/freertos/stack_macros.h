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

#جے_بنا_تعریف ڈھیر_MACROS_H
#تعریف_کرو ڈھیر_MACROS_H

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
#جے_بنا_تعریف پورٹ_ڈھیر_حد_بندی_PADDING
    #تعریف_کرو پورٹ_ڈھیر_حد_بندی_PADDING    0
#جے_انت

#إذا ( ( config_CHECK_FOR_ڈھیر_حد_توں_ودھ == 1 ) && ( پورٹ_ڈھیر_GROWTH < 0 ) )

/* Only the current stack state is to be checked. */
    #تعریف_کرو task_CHECK_FOR_ڈھیر_حد_توں_ودھ()                                                      \
    افعل {                                                                                        \
        /* Is the currently saved stack pointer within the stack limit? */                      \
        إذا( pxCurrentTCB->px_چوٹی_دا_ڈھیر <= pxCurrentTCB->px_ڈھیر + پورٹ_ڈھیر_حد_بندی_PADDING )     \
        {                                                                                       \
            حرف * pc_حد_توں_ودھ_Task_Name = pxCurrentTCB->pcTaskName;                               \
            v_Application_ڈھیر_حد_توں_ودھ_Hook( ( TaskHandle_t ) pxCurrentTCB, pc_حد_توں_ودھ_Task_Name ); \
        }                                                                                       \
    } طالما( 0 )

#جے_انت /* configCHECK_FOR_STACK_OVERFLOW == 1 */
/*-----------------------------------------------------------*/

#إذا ( ( config_CHECK_FOR_ڈھیر_حد_توں_ودھ == 1 ) && ( پورٹ_ڈھیر_GROWTH > 0 ) )

/* Only the current stack state is to be checked. */
    #تعریف_کرو task_CHECK_FOR_ڈھیر_حد_توں_ودھ()                                                       \
    افعل {                                                                                         \
                                                                                                 \
        /* Is the currently saved stack pointer within the stack limit? */                       \
        إذا( pxCurrentTCB->px_چوٹی_دا_ڈھیر >= pxCurrentTCB->px_سرا_دا_ڈھیر - پورٹ_ڈھیر_حد_بندی_PADDING ) \
        {                                                                                        \
            حرف * pc_حد_توں_ودھ_Task_Name = pxCurrentTCB->pcTaskName;                                \
            v_Application_ڈھیر_حد_توں_ودھ_Hook( ( TaskHandle_t ) pxCurrentTCB, pc_حد_توں_ودھ_Task_Name );  \
        }                                                                                        \
    } طالما( 0 )

#جے_انت /* configCHECK_FOR_STACK_OVERFLOW == 1 */
/*-----------------------------------------------------------*/

#إذا ( ( config_CHECK_FOR_ڈھیر_حد_توں_ودھ > 1 ) && ( پورٹ_ڈھیر_GROWTH < 0 ) )

    #تعریف_کرو task_CHECK_FOR_ڈھیر_حد_توں_ودھ()                                                      \
    افعل {                                                                                        \
        ثابت uint32_t * ثابت pul_ڈھیر = ( uint32_t * ) pxCurrentTCB->px_ڈھیر;                 \
        ثابت uint32_t ul_Check_قدر = ( uint32_t ) 0xa5a5a5a5U;                                 \
                                                                                                \
        إذا( ( pul_ڈھیر[ 0 ] != ul_Check_قدر ) ||                                                \
            ( pul_ڈھیر[ 1 ] != ul_Check_قدر ) ||                                                \
            ( pul_ڈھیر[ 2 ] != ul_Check_قدر ) ||                                                \
            ( pul_ڈھیر[ 3 ] != ul_Check_قدر ) )                                                 \
        {                                                                                       \
            حرف * pc_حد_توں_ودھ_Task_Name = pxCurrentTCB->pcTaskName;                               \
            v_Application_ڈھیر_حد_توں_ودھ_Hook( ( TaskHandle_t ) pxCurrentTCB, pc_حد_توں_ودھ_Task_Name ); \
        }                                                                                       \
    } طالما( 0 )

#جے_انت /* #if( configCHECK_FOR_STACK_OVERFLOW > 1 ) */
/*-----------------------------------------------------------*/

#إذا ( ( config_CHECK_FOR_ڈھیر_حد_توں_ودھ > 1 ) && ( پورٹ_ڈھیر_GROWTH > 0 ) )

    #تعریف_کرو task_CHECK_FOR_ڈھیر_حد_توں_ودھ()                                                                                                \
    افعل {                                                                                                                                  \
        int8_t * pc_سرا_دا_ڈھیر = ( int8_t * ) pxCurrentTCB->px_سرا_دا_ڈھیر;                                                                  \
        ساكن ثابت uint8_t uc_Expected_ڈھیر_بائٹاں[] = { tsk_ڈھیر_FILL_BYTE, tsk_ڈھیر_FILL_BYTE, tsk_ڈھیر_FILL_BYTE, tsk_ڈھیر_FILL_BYTE,   \
                                                        tsk_ڈھیر_FILL_BYTE, tsk_ڈھیر_FILL_BYTE, tsk_ڈھیر_FILL_BYTE, tsk_ڈھیر_FILL_BYTE,   \
                                                        tsk_ڈھیر_FILL_BYTE, tsk_ڈھیر_FILL_BYTE, tsk_ڈھیر_FILL_BYTE, tsk_ڈھیر_FILL_BYTE,   \
                                                        tsk_ڈھیر_FILL_BYTE, tsk_ڈھیر_FILL_BYTE, tsk_ڈھیر_FILL_BYTE, tsk_ڈھیر_FILL_BYTE,   \
                                                        tsk_ڈھیر_FILL_BYTE, tsk_ڈھیر_FILL_BYTE, tsk_ڈھیر_FILL_BYTE, tsk_ڈھیر_FILL_BYTE }; \
                                                                                                                                          \
                                                                                                                                          \
        pc_سرا_دا_ڈھیر -= حجم( uc_Expected_ڈھیر_بائٹاں );                                                                                   \
                                                                                                                                          \
        /* Has the extremity of the task stack ever been written over? */                                                                 \
        إذا( memcmp( ( فراغ * ) pc_سرا_دا_ڈھیر, ( فراغ * ) uc_Expected_ڈھیر_بائٹاں, حجم( uc_Expected_ڈھیر_بائٹاں ) ) != 0 )                     \
        {                                                                                                                                 \
            حرف * pc_حد_توں_ودھ_Task_Name = pxCurrentTCB->pcTaskName;                                                                         \
            v_Application_ڈھیر_حد_توں_ودھ_Hook( ( TaskHandle_t ) pxCurrentTCB, pc_حد_توں_ودھ_Task_Name );                                           \
        }                                                                                                                                 \
    } طالما( 0 )

#جے_انت /* #if( configCHECK_FOR_STACK_OVERFLOW > 1 ) */
/*-----------------------------------------------------------*/

/* Remove stack overflow macro if not being used. */
#جے_بنا_تعریف task_CHECK_FOR_ڈھیر_حد_توں_ودھ
    #تعریف_کرو task_CHECK_FOR_ڈھیر_حد_توں_ودھ()
#جے_انت



#جے_انت /* STACK_MACROS_H */
