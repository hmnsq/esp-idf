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

#अगर_अघोषित ढेर_MACROS_H
#घोषणा ढेर_MACROS_H

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
#अगर_अघोषित port_ढेर_LIMIT_PADDING
    #घोषणा port_ढेर_LIMIT_PADDING    0
#पूर्ण_अगर

#अगर ( ( config_CHECK_FOR_ढेर_OVERFLOW == 1 ) && ( port_ढेर_GROWTH < 0 ) )

/* Only the current stack state is to be checked. */
    #घोषणा task_CHECK_FOR_ढेर_OVERFLOW()                                                      \
    करो {                                                                                        \
        /* Is the currently saved stack pointer within the stack limit? */                      \
        अगर( pxCurrentTCB->px_शीर्ष_Of_ढेर <= pxCurrentTCB->px_ढेर + port_ढेर_LIMIT_PADDING )     \
        {                                                                                       \
            अक्षर * pcOverflowTaskName = pxCurrentTCB->pcTaskName;                               \
            v_Application_ढेर_Overflow_Hook( ( TaskHandle_t ) pxCurrentTCB, pcOverflowTaskName ); \
        }                                                                                       \
    } जबतक( 0 )

#पूर्ण_अगर /* configCHECK_FOR_STACK_OVERFLOW == 1 */
/*-----------------------------------------------------------*/

#अगर ( ( config_CHECK_FOR_ढेर_OVERFLOW == 1 ) && ( port_ढेर_GROWTH > 0 ) )

/* Only the current stack state is to be checked. */
    #घोषणा task_CHECK_FOR_ढेर_OVERFLOW()                                                       \
    करो {                                                                                         \
                                                                                                 \
        /* Is the currently saved stack pointer within the stack limit? */                       \
        अगर( pxCurrentTCB->px_शीर्ष_Of_ढेर >= pxCurrentTCB->px_End_Of_ढेर - port_ढेर_LIMIT_PADDING ) \
        {                                                                                        \
            अक्षर * pcOverflowTaskName = pxCurrentTCB->pcTaskName;                                \
            v_Application_ढेर_Overflow_Hook( ( TaskHandle_t ) pxCurrentTCB, pcOverflowTaskName );  \
        }                                                                                        \
    } जबतक( 0 )

#पूर्ण_अगर /* configCHECK_FOR_STACK_OVERFLOW == 1 */
/*-----------------------------------------------------------*/

#अगर ( ( config_CHECK_FOR_ढेर_OVERFLOW > 1 ) && ( port_ढेर_GROWTH < 0 ) )

    #घोषणा task_CHECK_FOR_ढेर_OVERFLOW()                                                      \
    करो {                                                                                        \
        स्थिर uint32_t * स्थिर pul_ढेर = ( uint32_t * ) pxCurrentTCB->px_ढेर;                 \
        स्थिर uint32_t ul_Check_मूल्य = ( uint32_t ) 0xa5a5a5a5U;                                 \
                                                                                                \
        अगर( ( pul_ढेर[ 0 ] != ul_Check_मूल्य ) ||                                                \
            ( pul_ढेर[ 1 ] != ul_Check_मूल्य ) ||                                                \
            ( pul_ढेर[ 2 ] != ul_Check_मूल्य ) ||                                                \
            ( pul_ढेर[ 3 ] != ul_Check_मूल्य ) )                                                 \
        {                                                                                       \
            अक्षर * pcOverflowTaskName = pxCurrentTCB->pcTaskName;                               \
            v_Application_ढेर_Overflow_Hook( ( TaskHandle_t ) pxCurrentTCB, pcOverflowTaskName ); \
        }                                                                                       \
    } जबतक( 0 )

#पूर्ण_अगर /* #if( configCHECK_FOR_STACK_OVERFLOW > 1 ) */
/*-----------------------------------------------------------*/

#अगर ( ( config_CHECK_FOR_ढेर_OVERFLOW > 1 ) && ( port_ढेर_GROWTH > 0 ) )

    #घोषणा task_CHECK_FOR_ढेर_OVERFLOW()                                                                                                \
    करो {                                                                                                                                  \
        int8_t * pc_End_Of_ढेर = ( int8_t * ) pxCurrentTCB->px_End_Of_ढेर;                                                                  \
        अटल स्थिर uint8_t uc_Expected_ढेर_Bytes[] = { tsk_ढेर_FILL_BYTE, tsk_ढेर_FILL_BYTE, tsk_ढेर_FILL_BYTE, tsk_ढेर_FILL_BYTE,   \
                                                        tsk_ढेर_FILL_BYTE, tsk_ढेर_FILL_BYTE, tsk_ढेर_FILL_BYTE, tsk_ढेर_FILL_BYTE,   \
                                                        tsk_ढेर_FILL_BYTE, tsk_ढेर_FILL_BYTE, tsk_ढेर_FILL_BYTE, tsk_ढेर_FILL_BYTE,   \
                                                        tsk_ढेर_FILL_BYTE, tsk_ढेर_FILL_BYTE, tsk_ढेर_FILL_BYTE, tsk_ढेर_FILL_BYTE,   \
                                                        tsk_ढेर_FILL_BYTE, tsk_ढेर_FILL_BYTE, tsk_ढेर_FILL_BYTE, tsk_ढेर_FILL_BYTE }; \
                                                                                                                                          \
                                                                                                                                          \
        pc_End_Of_ढेर -= माप( uc_Expected_ढेर_Bytes );                                                                                   \
                                                                                                                                          \
        /* Has the extremity of the task stack ever been written over? */                                                                 \
        अगर( स_भेद( ( व्योम * ) pc_End_Of_ढेर, ( व्योम * ) uc_Expected_ढेर_Bytes, माप( uc_Expected_ढेर_Bytes ) ) != 0 )                     \
        {                                                                                                                                 \
            अक्षर * pcOverflowTaskName = pxCurrentTCB->pcTaskName;                                                                         \
            v_Application_ढेर_Overflow_Hook( ( TaskHandle_t ) pxCurrentTCB, pcOverflowTaskName );                                           \
        }                                                                                                                                 \
    } जबतक( 0 )

#पूर्ण_अगर /* #if( configCHECK_FOR_STACK_OVERFLOW > 1 ) */
/*-----------------------------------------------------------*/

/* Remove stack overflow macro if not being used. */
#अगर_अघोषित task_CHECK_FOR_ढेर_OVERFLOW
    #घोषणा task_CHECK_FOR_ढेर_OVERFLOW()
#पूर्ण_अगर



#पूर्ण_अगर /* STACK_MACROS_H */
