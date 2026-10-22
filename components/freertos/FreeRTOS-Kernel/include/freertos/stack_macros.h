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

#اگر_غیر_تعریف_شدہ ڈھیر_MACROS_H
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
#اگر_غیر_تعریف_شدہ پورٹ_ڈھیر_حد_بندی_PADDING
    #تعریف_کرو پورٹ_ڈھیر_حد_بندی_PADDING    0
#اگر_ختم

#اگر ( ( config_CHECK_FOR_ڈھیر_حد_سے_تجاوز == 1 ) && ( پورٹ_ڈھیر_GROWTH < 0 ) )

/* Only the current stack state is to be checked. */
    #تعریف_کرو task_CHECK_FOR_ڈھیر_حد_سے_تجاوز( xCurCoreID )                                                                              \
    {                                                                                                                               \
        /* Is the currently saved stack pointer within the stack limit? */                                                          \
        اگر( pxCurrentTCBs[ xCurCoreID ]->px_چوٹی_کا_ڈھیر <= pxCurrentTCBs[ xCurCoreID ]->px_ڈھیر + پورٹ_ڈھیر_حد_بندی_PADDING )           \
        {                                                                                                                           \
            v_Application_ڈھیر_حد_سے_تجاوز_Hook( ( TaskHandle_t ) pxCurrentTCBs[ xCurCoreID ], pxCurrentTCBs[ xCurCoreID ]->pcTaskName ); \
        }                                                                                                                           \
    }

#اگر_ختم /* configCHECK_FOR_STACK_OVERFLOW == 1 */
/*-----------------------------------------------------------*/

#اگر ( ( config_CHECK_FOR_ڈھیر_حد_سے_تجاوز == 1 ) && ( پورٹ_ڈھیر_GROWTH > 0 ) )

/* Only the current stack state is to be checked. */
    #تعریف_کرو task_CHECK_FOR_ڈھیر_حد_سے_تجاوز( xCurCoreID )                                                                              \
    {                                                                                                                               \
                                                                                                                                    \
        /* Is the currently saved stack pointer within the stack limit? */                                                          \
        اگر( pxCurrentTCBs[ xCurCoreID ]->px_چوٹی_کا_ڈھیر >= pxCurrentTCBs[ xCurCoreID ]->px_انتہا_کا_ڈھیر - پورٹ_ڈھیر_حد_بندی_PADDING )      \
        {                                                                                                                           \
            v_Application_ڈھیر_حد_سے_تجاوز_Hook( ( TaskHandle_t ) pxCurrentTCBs[ xCurCoreID ], pxCurrentTCBs[ xCurCoreID ]->pcTaskName ); \
        }                                                                                                                           \
    }

#اگر_ختم /* configCHECK_FOR_STACK_OVERFLOW == 1 */
/*-----------------------------------------------------------*/

#اگر ( ( config_CHECK_FOR_ڈھیر_حد_سے_تجاوز > 1 ) && ( پورٹ_ڈھیر_GROWTH < 0 ) )

    #تعریف_کرو task_CHECK_FOR_ڈھیر_حد_سے_تجاوز( xCurCoreID )                                                                              \
    {                                                                                                                               \
        ثابت uint32_t * ثابت pul_ڈھیر = ( uint32_t * ) pxCurrentTCBs[ xCurCoreID ]->px_ڈھیر;                                      \
        ثابت uint32_t ul_Check_قدر = ( uint32_t ) 0xa5a5a5a5;                                                                      \
                                                                                                                                    \
        اگر( ( pul_ڈھیر[ 0 ] != ul_Check_قدر ) ||                                                                                    \
            ( pul_ڈھیر[ 1 ] != ul_Check_قدر ) ||                                                                                    \
            ( pul_ڈھیر[ 2 ] != ul_Check_قدر ) ||                                                                                    \
            ( pul_ڈھیر[ 3 ] != ul_Check_قدر ) )                                                                                     \
        {                                                                                                                           \
            v_Application_ڈھیر_حد_سے_تجاوز_Hook( ( TaskHandle_t ) pxCurrentTCBs[ xCurCoreID ], pxCurrentTCBs[ xCurCoreID ]->pcTaskName ); \
        }                                                                                                                           \
    }

#اگر_ختم /* #if( configCHECK_FOR_STACK_OVERFLOW > 1 ) */
/*-----------------------------------------------------------*/

#اگر ( ( config_CHECK_FOR_ڈھیر_حد_سے_تجاوز > 1 ) && ( پورٹ_ڈھیر_GROWTH > 0 ) )

    #تعریف_کرو task_CHECK_FOR_ڈھیر_حد_سے_تجاوز( xCurCoreID )                                                                                    \
    {                                                                                                                                     \
        int8_t * pc_انتہا_کا_ڈھیر = ( int8_t * ) pxCurrentTCBs[ xCurCoreID ]->px_انتہا_کا_ڈھیر;                                                   \
        ساکن ثابت uint8_t uc_Expected_ڈھیر_بائٹیں[] = { tsk_ڈھیر_FILL_BYTE, tsk_ڈھیر_FILL_BYTE, tsk_ڈھیر_FILL_BYTE, tsk_ڈھیر_FILL_BYTE,   \
                                                        tsk_ڈھیر_FILL_BYTE, tsk_ڈھیر_FILL_BYTE, tsk_ڈھیر_FILL_BYTE, tsk_ڈھیر_FILL_BYTE,   \
                                                        tsk_ڈھیر_FILL_BYTE, tsk_ڈھیر_FILL_BYTE, tsk_ڈھیر_FILL_BYTE, tsk_ڈھیر_FILL_BYTE,   \
                                                        tsk_ڈھیر_FILL_BYTE, tsk_ڈھیر_FILL_BYTE, tsk_ڈھیر_FILL_BYTE, tsk_ڈھیر_FILL_BYTE,   \
                                                        tsk_ڈھیر_FILL_BYTE, tsk_ڈھیر_FILL_BYTE, tsk_ڈھیر_FILL_BYTE, tsk_ڈھیر_FILL_BYTE }; \
                                                                                                                                          \
                                                                                                                                          \
        pc_انتہا_کا_ڈھیر -= حجم( uc_Expected_ڈھیر_بائٹیں );                                                                                   \
                                                                                                                                          \
        /* Has the extremity of the task stack ever been written over? */                                                                 \
        اگر( memcmp( ( خالی * ) pc_انتہا_کا_ڈھیر, ( خالی * ) uc_Expected_ڈھیر_بائٹیں, حجم( uc_Expected_ڈھیر_بائٹیں ) ) != 0 )                     \
        {                                                                                                                                 \
            v_Application_ڈھیر_حد_سے_تجاوز_Hook( ( TaskHandle_t ) pxCurrentTCBs[ xCurCoreID ], pxCurrentTCBs[ xCurCoreID ]->pcTaskName );       \
        }                                                                                                                                 \
    }

#اگر_ختم /* #if( configCHECK_FOR_STACK_OVERFLOW > 1 ) */
/*-----------------------------------------------------------*/

/* Remove stack overflow macro if not being used. */
#اگر_غیر_تعریف_شدہ task_CHECK_FOR_ڈھیر_حد_سے_تجاوز
    #تعریف_کرو task_CHECK_FOR_ڈھیر_حد_سے_تجاوز( xCurCoreID )
#اگر_ختم



#اگر_ختم /* STACK_MACROS_H */
