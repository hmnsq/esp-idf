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

#اگر_تعریف_نشده پشته_MACROS_H
#تعریف_کن پشته_MACROS_H

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
#اگر_تعریف_نشده پورت_پشته_حد_PADDING
    #تعریف_کن پورت_پشته_حد_PADDING    0
#پایان_اگر

#إذا ( ( config_CHECK_FOR_پشته_سرریز == 1 ) && ( پورت_پشته_GROWTH < 0 ) )

/* Only the current stack state is to be checked. */
    #تعریف_کن task_CHECK_FOR_پشته_سرریز()                                                      \
    افعل {                                                                                        \
        /* Is the currently saved stack pointer within the stack limit? */                      \
        إذا( pxCurrentTCB->px_بالاترین_از_پشته <= pxCurrentTCB->px_پشته + پورت_پشته_حد_PADDING )     \
        {                                                                                       \
            حرف * pc_سرریز_Task_Name = pxCurrentTCB->pcTaskName;                               \
            v_Application_پشته_سرریز_Hook( ( TaskHandle_t ) pxCurrentTCB, pc_سرریز_Task_Name ); \
        }                                                                                       \
    } طالما( 0 )

#پایان_اگر /* configCHECK_FOR_STACK_OVERFLOW == 1 */
/*-----------------------------------------------------------*/

#إذا ( ( config_CHECK_FOR_پشته_سرریز == 1 ) && ( پورت_پشته_GROWTH > 0 ) )

/* Only the current stack state is to be checked. */
    #تعریف_کن task_CHECK_FOR_پشته_سرریز()                                                       \
    افعل {                                                                                         \
                                                                                                 \
        /* Is the currently saved stack pointer within the stack limit? */                       \
        إذا( pxCurrentTCB->px_بالاترین_از_پشته >= pxCurrentTCB->px_انتها_از_پشته - پورت_پشته_حد_PADDING ) \
        {                                                                                        \
            حرف * pc_سرریز_Task_Name = pxCurrentTCB->pcTaskName;                                \
            v_Application_پشته_سرریز_Hook( ( TaskHandle_t ) pxCurrentTCB, pc_سرریز_Task_Name );  \
        }                                                                                        \
    } طالما( 0 )

#پایان_اگر /* configCHECK_FOR_STACK_OVERFLOW == 1 */
/*-----------------------------------------------------------*/

#إذا ( ( config_CHECK_FOR_پشته_سرریز > 1 ) && ( پورت_پشته_GROWTH < 0 ) )

    #تعریف_کن task_CHECK_FOR_پشته_سرریز()                                                      \
    افعل {                                                                                        \
        ثابت uint32_t * ثابت pul_پشته = ( uint32_t * ) pxCurrentTCB->px_پشته;                 \
        ثابت uint32_t ul_Check_مقدار = ( uint32_t ) 0xa5a5a5a5U;                                 \
                                                                                                \
        إذا( ( pul_پشته[ 0 ] != ul_Check_مقدار ) ||                                                \
            ( pul_پشته[ 1 ] != ul_Check_مقدار ) ||                                                \
            ( pul_پشته[ 2 ] != ul_Check_مقدار ) ||                                                \
            ( pul_پشته[ 3 ] != ul_Check_مقدار ) )                                                 \
        {                                                                                       \
            حرف * pc_سرریز_Task_Name = pxCurrentTCB->pcTaskName;                               \
            v_Application_پشته_سرریز_Hook( ( TaskHandle_t ) pxCurrentTCB, pc_سرریز_Task_Name ); \
        }                                                                                       \
    } طالما( 0 )

#پایان_اگر /* #if( configCHECK_FOR_STACK_OVERFLOW > 1 ) */
/*-----------------------------------------------------------*/

#إذا ( ( config_CHECK_FOR_پشته_سرریز > 1 ) && ( پورت_پشته_GROWTH > 0 ) )

    #تعریف_کن task_CHECK_FOR_پشته_سرریز()                                                                                                \
    افعل {                                                                                                                                  \
        int8_t * pc_انتها_از_پشته = ( int8_t * ) pxCurrentTCB->px_انتها_از_پشته;                                                                  \
        ساكن ثابت uint8_t uc_Expected_پشته_بایتها[] = { tsk_پشته_FILL_BYTE, tsk_پشته_FILL_BYTE, tsk_پشته_FILL_BYTE, tsk_پشته_FILL_BYTE,   \
                                                        tsk_پشته_FILL_BYTE, tsk_پشته_FILL_BYTE, tsk_پشته_FILL_BYTE, tsk_پشته_FILL_BYTE,   \
                                                        tsk_پشته_FILL_BYTE, tsk_پشته_FILL_BYTE, tsk_پشته_FILL_BYTE, tsk_پشته_FILL_BYTE,   \
                                                        tsk_پشته_FILL_BYTE, tsk_پشته_FILL_BYTE, tsk_پشته_FILL_BYTE, tsk_پشته_FILL_BYTE,   \
                                                        tsk_پشته_FILL_BYTE, tsk_پشته_FILL_BYTE, tsk_پشته_FILL_BYTE, tsk_پشته_FILL_BYTE }; \
                                                                                                                                          \
                                                                                                                                          \
        pc_انتها_از_پشته -= حجم( uc_Expected_پشته_بایتها );                                                                                   \
                                                                                                                                          \
        /* Has the extremity of the task stack ever been written over? */                                                                 \
        إذا( memcmp( ( فراغ * ) pc_انتها_از_پشته, ( فراغ * ) uc_Expected_پشته_بایتها, حجم( uc_Expected_پشته_بایتها ) ) != 0 )                     \
        {                                                                                                                                 \
            حرف * pc_سرریز_Task_Name = pxCurrentTCB->pcTaskName;                                                                         \
            v_Application_پشته_سرریز_Hook( ( TaskHandle_t ) pxCurrentTCB, pc_سرریز_Task_Name );                                           \
        }                                                                                                                                 \
    } طالما( 0 )

#پایان_اگر /* #if( configCHECK_FOR_STACK_OVERFLOW > 1 ) */
/*-----------------------------------------------------------*/

/* Remove stack overflow macro if not being used. */
#اگر_تعریف_نشده task_CHECK_FOR_پشته_سرریز
    #تعریف_کن task_CHECK_FOR_پشته_سرریز()
#پایان_اگر



#پایان_اگر /* STACK_MACROS_H */
