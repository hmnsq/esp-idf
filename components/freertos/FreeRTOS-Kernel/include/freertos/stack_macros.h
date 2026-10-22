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

#यदि_अपरिभाषितम् राशिः_MACROS_H
#परिभाषय राशिः_MACROS_H

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
#यदि_अपरिभाषितम् पत्तनम्_राशिः_सीमा_PADDING
    #परिभाषय पत्तनम्_राशिः_सीमा_PADDING    0
#यदि_अन्तः

#यदि ( ( config_CHECK_FOR_राशिः_अतिप्रवाहः == 1 ) && ( पत्तनम्_राशिः_GROWTH < 0 ) )

/* Only the current stack state is to be checked. */
    #परिभाषय task_CHECK_FOR_राशिः_अतिप्रवाहः( xCurCoreID )                                                                              \
    {                                                                                                                               \
        /* Is the currently saved stack pointer within the stack limit? */                                                          \
        यदि( pxCurrentTCBs[ xCurCoreID ]->px_शिखरम्_सम्बन्धि_राशिः <= pxCurrentTCBs[ xCurCoreID ]->px_राशिः + पत्तनम्_राशिः_सीमा_PADDING )           \
        {                                                                                                                           \
            v_Application_राशिः_अतिप्रवाहः_Hook( ( TaskHandle_t ) pxCurrentTCBs[ xCurCoreID ], pxCurrentTCBs[ xCurCoreID ]->pcTaskName ); \
        }                                                                                                                           \
    }

#यदि_अन्तः /* configCHECK_FOR_STACK_OVERFLOW == 1 */
/*-----------------------------------------------------------*/

#यदि ( ( config_CHECK_FOR_राशिः_अतिप्रवाहः == 1 ) && ( पत्तनम्_राशिः_GROWTH > 0 ) )

/* Only the current stack state is to be checked. */
    #परिभाषय task_CHECK_FOR_राशिः_अतिप्रवाहः( xCurCoreID )                                                                              \
    {                                                                                                                               \
                                                                                                                                    \
        /* Is the currently saved stack pointer within the stack limit? */                                                          \
        यदि( pxCurrentTCBs[ xCurCoreID ]->px_शिखरम्_सम्बन्धि_राशिः >= pxCurrentTCBs[ xCurCoreID ]->px_अन्तः_सम्बन्धि_राशिः - पत्तनम्_राशिः_सीमा_PADDING )      \
        {                                                                                                                           \
            v_Application_राशिः_अतिप्रवाहः_Hook( ( TaskHandle_t ) pxCurrentTCBs[ xCurCoreID ], pxCurrentTCBs[ xCurCoreID ]->pcTaskName ); \
        }                                                                                                                           \
    }

#यदि_अन्तः /* configCHECK_FOR_STACK_OVERFLOW == 1 */
/*-----------------------------------------------------------*/

#यदि ( ( config_CHECK_FOR_राशिः_अतिप्रवाहः > 1 ) && ( पत्तनम्_राशिः_GROWTH < 0 ) )

    #परिभाषय task_CHECK_FOR_राशिः_अतिप्रवाहः( xCurCoreID )                                                                              \
    {                                                                                                                               \
        ध्रुव uint32_t * ध्रुव pul_राशिः = ( uint32_t * ) pxCurrentTCBs[ xCurCoreID ]->px_राशिः;                                      \
        ध्रुव uint32_t ul_Check_मानम् = ( uint32_t ) 0xa5a5a5a5;                                                                      \
                                                                                                                                    \
        यदि( ( pul_राशिः[ 0 ] != ul_Check_मानम् ) ||                                                                                    \
            ( pul_राशिः[ 1 ] != ul_Check_मानम् ) ||                                                                                    \
            ( pul_राशिः[ 2 ] != ul_Check_मानम् ) ||                                                                                    \
            ( pul_राशिः[ 3 ] != ul_Check_मानम् ) )                                                                                     \
        {                                                                                                                           \
            v_Application_राशिः_अतिप्रवाहः_Hook( ( TaskHandle_t ) pxCurrentTCBs[ xCurCoreID ], pxCurrentTCBs[ xCurCoreID ]->pcTaskName ); \
        }                                                                                                                           \
    }

#यदि_अन्तः /* #if( configCHECK_FOR_STACK_OVERFLOW > 1 ) */
/*-----------------------------------------------------------*/

#यदि ( ( config_CHECK_FOR_राशिः_अतिप्रवाहः > 1 ) && ( पत्तनम्_राशिः_GROWTH > 0 ) )

    #परिभाषय task_CHECK_FOR_राशिः_अतिप्रवाहः( xCurCoreID )                                                                                    \
    {                                                                                                                                     \
        int8_t * pc_अन्तः_सम्बन्धि_राशिः = ( int8_t * ) pxCurrentTCBs[ xCurCoreID ]->px_अन्तः_सम्बन्धि_राशिः;                                                   \
        स्थिर ध्रुव uint8_t uc_Expected_राशिः_अष्टकानि[] = { tsk_राशिः_FILL_BYTE, tsk_राशिः_FILL_BYTE, tsk_राशिः_FILL_BYTE, tsk_राशिः_FILL_BYTE,   \
                                                        tsk_राशिः_FILL_BYTE, tsk_राशिः_FILL_BYTE, tsk_राशिः_FILL_BYTE, tsk_राशिः_FILL_BYTE,   \
                                                        tsk_राशिः_FILL_BYTE, tsk_राशिः_FILL_BYTE, tsk_राशिः_FILL_BYTE, tsk_राशिः_FILL_BYTE,   \
                                                        tsk_राशिः_FILL_BYTE, tsk_राशिः_FILL_BYTE, tsk_राशिः_FILL_BYTE, tsk_राशिः_FILL_BYTE,   \
                                                        tsk_राशिः_FILL_BYTE, tsk_राशिः_FILL_BYTE, tsk_राशिः_FILL_BYTE, tsk_राशिः_FILL_BYTE }; \
                                                                                                                                          \
                                                                                                                                          \
        pc_अन्तः_सम्बन्धि_राशिः -= परिमाण( uc_Expected_राशिः_अष्टकानि );                                                                                   \
                                                                                                                                          \
        /* Has the extremity of the task stack ever been written over? */                                                                 \
        यदि( memcmp( ( शून्य * ) pc_अन्तः_सम्बन्धि_राशिः, ( शून्य * ) uc_Expected_राशिः_अष्टकानि, परिमाण( uc_Expected_राशिः_अष्टकानि ) ) != 0 )                     \
        {                                                                                                                                 \
            v_Application_राशिः_अतिप्रवाहः_Hook( ( TaskHandle_t ) pxCurrentTCBs[ xCurCoreID ], pxCurrentTCBs[ xCurCoreID ]->pcTaskName );       \
        }                                                                                                                                 \
    }

#यदि_अन्तः /* #if( configCHECK_FOR_STACK_OVERFLOW > 1 ) */
/*-----------------------------------------------------------*/

/* Remove stack overflow macro if not being used. */
#यदि_अपरिभाषितम् task_CHECK_FOR_राशिः_अतिप्रवाहः
    #परिभाषय task_CHECK_FOR_राशिः_अतिप्रवाहः( xCurCoreID )
#यदि_अन्तः



#यदि_अन्तः /* STACK_MACROS_H */
