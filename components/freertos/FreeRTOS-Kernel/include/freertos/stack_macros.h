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

#جيڪڏهن_وصف_نه_ٿيل ڍير_MACROS_H
#وصف_ڪريو ڍير_MACROS_H

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
#جيڪڏهن_وصف_نه_ٿيل پورٽ_ڍير_حد_بندي_PADDING
    #وصف_ڪريو پورٽ_ڍير_حد_بندي_PADDING    0
#جيڪڏهن_پورو

#إذا ( ( config_CHECK_FOR_ڍير_حد_کان_مٿي == 1 ) && ( پورٽ_ڍير_GROWTH < 0 ) )

/* Only the current stack state is to be checked. */
    #وصف_ڪريو task_CHECK_FOR_ڍير_حد_کان_مٿي( xCurCoreID )                                                                              \
    {                                                                                                                               \
        /* Is the currently saved stack pointer within the stack limit? */                                                          \
        إذا( pxCurrentTCBs[ xCurCoreID ]->px_چوٽي_جو_ڍير <= pxCurrentTCBs[ xCurCoreID ]->px_ڍير + پورٽ_ڍير_حد_بندي_PADDING )           \
        {                                                                                                                           \
            v_Application_ڍير_حد_کان_مٿي_Hook( ( TaskHandle_t ) pxCurrentTCBs[ xCurCoreID ], pxCurrentTCBs[ xCurCoreID ]->pcTaskName ); \
        }                                                                                                                           \
    }

#جيڪڏهن_پورو /* configCHECK_FOR_STACK_OVERFLOW == 1 */
/*-----------------------------------------------------------*/

#إذا ( ( config_CHECK_FOR_ڍير_حد_کان_مٿي == 1 ) && ( پورٽ_ڍير_GROWTH > 0 ) )

/* Only the current stack state is to be checked. */
    #وصف_ڪريو task_CHECK_FOR_ڍير_حد_کان_مٿي( xCurCoreID )                                                                              \
    {                                                                                                                               \
                                                                                                                                    \
        /* Is the currently saved stack pointer within the stack limit? */                                                          \
        إذا( pxCurrentTCBs[ xCurCoreID ]->px_چوٽي_جو_ڍير >= pxCurrentTCBs[ xCurCoreID ]->px_پڇاڙي_جو_ڍير - پورٽ_ڍير_حد_بندي_PADDING )      \
        {                                                                                                                           \
            v_Application_ڍير_حد_کان_مٿي_Hook( ( TaskHandle_t ) pxCurrentTCBs[ xCurCoreID ], pxCurrentTCBs[ xCurCoreID ]->pcTaskName ); \
        }                                                                                                                           \
    }

#جيڪڏهن_پورو /* configCHECK_FOR_STACK_OVERFLOW == 1 */
/*-----------------------------------------------------------*/

#إذا ( ( config_CHECK_FOR_ڍير_حد_کان_مٿي > 1 ) && ( پورٽ_ڍير_GROWTH < 0 ) )

    #وصف_ڪريو task_CHECK_FOR_ڍير_حد_کان_مٿي( xCurCoreID )                                                                              \
    {                                                                                                                               \
        ثابت uint32_t * ثابت pul_ڍير = ( uint32_t * ) pxCurrentTCBs[ xCurCoreID ]->px_ڍير;                                      \
        ثابت uint32_t ul_Check_قدر = ( uint32_t ) 0xa5a5a5a5;                                                                      \
                                                                                                                                    \
        إذا( ( pul_ڍير[ 0 ] != ul_Check_قدر ) ||                                                                                    \
            ( pul_ڍير[ 1 ] != ul_Check_قدر ) ||                                                                                    \
            ( pul_ڍير[ 2 ] != ul_Check_قدر ) ||                                                                                    \
            ( pul_ڍير[ 3 ] != ul_Check_قدر ) )                                                                                     \
        {                                                                                                                           \
            v_Application_ڍير_حد_کان_مٿي_Hook( ( TaskHandle_t ) pxCurrentTCBs[ xCurCoreID ], pxCurrentTCBs[ xCurCoreID ]->pcTaskName ); \
        }                                                                                                                           \
    }

#جيڪڏهن_پورو /* #if( configCHECK_FOR_STACK_OVERFLOW > 1 ) */
/*-----------------------------------------------------------*/

#إذا ( ( config_CHECK_FOR_ڍير_حد_کان_مٿي > 1 ) && ( پورٽ_ڍير_GROWTH > 0 ) )

    #وصف_ڪريو task_CHECK_FOR_ڍير_حد_کان_مٿي( xCurCoreID )                                                                                    \
    {                                                                                                                                     \
        int8_t * pc_پڇاڙي_جو_ڍير = ( int8_t * ) pxCurrentTCBs[ xCurCoreID ]->px_پڇاڙي_جو_ڍير;                                                   \
        ساكن ثابت uint8_t uc_Expected_ڍير_بائيٽون[] = { tsk_ڍير_FILL_BYTE, tsk_ڍير_FILL_BYTE, tsk_ڍير_FILL_BYTE, tsk_ڍير_FILL_BYTE,   \
                                                        tsk_ڍير_FILL_BYTE, tsk_ڍير_FILL_BYTE, tsk_ڍير_FILL_BYTE, tsk_ڍير_FILL_BYTE,   \
                                                        tsk_ڍير_FILL_BYTE, tsk_ڍير_FILL_BYTE, tsk_ڍير_FILL_BYTE, tsk_ڍير_FILL_BYTE,   \
                                                        tsk_ڍير_FILL_BYTE, tsk_ڍير_FILL_BYTE, tsk_ڍير_FILL_BYTE, tsk_ڍير_FILL_BYTE,   \
                                                        tsk_ڍير_FILL_BYTE, tsk_ڍير_FILL_BYTE, tsk_ڍير_FILL_BYTE, tsk_ڍير_FILL_BYTE }; \
                                                                                                                                          \
                                                                                                                                          \
        pc_پڇاڙي_جو_ڍير -= حجم( uc_Expected_ڍير_بائيٽون );                                                                                   \
                                                                                                                                          \
        /* Has the extremity of the task stack ever been written over? */                                                                 \
        إذا( memcmp( ( فراغ * ) pc_پڇاڙي_جو_ڍير, ( فراغ * ) uc_Expected_ڍير_بائيٽون, حجم( uc_Expected_ڍير_بائيٽون ) ) != 0 )                     \
        {                                                                                                                                 \
            v_Application_ڍير_حد_کان_مٿي_Hook( ( TaskHandle_t ) pxCurrentTCBs[ xCurCoreID ], pxCurrentTCBs[ xCurCoreID ]->pcTaskName );       \
        }                                                                                                                                 \
    }

#جيڪڏهن_پورو /* #if( configCHECK_FOR_STACK_OVERFLOW > 1 ) */
/*-----------------------------------------------------------*/

/* Remove stack overflow macro if not being used. */
#جيڪڏهن_وصف_نه_ٿيل task_CHECK_FOR_ڍير_حد_کان_مٿي
    #وصف_ڪريو task_CHECK_FOR_ڍير_حد_کان_مٿي( xCurCoreID )
#جيڪڏهن_پورو



#جيڪڏهن_پورو /* STACK_MACROS_H */
