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

#ifndef चळत_MACROS_H
#define चळत_MACROS_H

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
#ifndef port_चळत_LIMIT_PADDING
    #define port_चळत_LIMIT_PADDING    0
#endif

#जर ( ( config_CHECK_FOR_चळत_OVERFLOW == 1 ) && ( port_चळत_GROWTH < 0 ) )

/* Only the current stack state is to be checked. */
    #define task_CHECK_FOR_चळत_OVERFLOW()                                                      \
    करा {                                                                                        \
        /* Is the currently saved stack pointer within the stack limit? */                      \
        जर( pxCurrentTCB->px_शीर्ष_Of_चळत <= pxCurrentTCB->px_चळत + port_चळत_LIMIT_PADDING )     \
        {                                                                                       \
            अक्षर * pcOverflowTaskName = pxCurrentTCB->pcTaskName;                               \
            v_Application_चळत_Overflow_Hook( ( TaskHandle_t ) pxCurrentTCB, pcOverflowTaskName ); \
        }                                                                                       \
    } पर्यंत( 0 )

#endif /* configCHECK_FOR_STACK_OVERFLOW == 1 */
/*-----------------------------------------------------------*/

#जर ( ( config_CHECK_FOR_चळत_OVERFLOW == 1 ) && ( port_चळत_GROWTH > 0 ) )

/* Only the current stack state is to be checked. */
    #define task_CHECK_FOR_चळत_OVERFLOW()                                                       \
    करा {                                                                                         \
                                                                                                 \
        /* Is the currently saved stack pointer within the stack limit? */                       \
        जर( pxCurrentTCB->px_शीर्ष_Of_चळत >= pxCurrentTCB->px_End_Of_चळत - port_चळत_LIMIT_PADDING ) \
        {                                                                                        \
            अक्षर * pcOverflowTaskName = pxCurrentTCB->pcTaskName;                                \
            v_Application_चळत_Overflow_Hook( ( TaskHandle_t ) pxCurrentTCB, pcOverflowTaskName );  \
        }                                                                                        \
    } पर्यंत( 0 )

#endif /* configCHECK_FOR_STACK_OVERFLOW == 1 */
/*-----------------------------------------------------------*/

#जर ( ( config_CHECK_FOR_चळत_OVERFLOW > 1 ) && ( port_चळत_GROWTH < 0 ) )

    #define task_CHECK_FOR_चळत_OVERFLOW()                                                      \
    करा {                                                                                        \
        स्थिरांक uint32_t * स्थिरांक pul_चळत = ( uint32_t * ) pxCurrentTCB->px_चळत;                 \
        स्थिरांक uint32_t ul_Check_मूल्य = ( uint32_t ) 0xa5a5a5a5U;                                 \
                                                                                                \
        जर( ( pul_चळत[ 0 ] != ul_Check_मूल्य ) ||                                                \
            ( pul_चळत[ 1 ] != ul_Check_मूल्य ) ||                                                \
            ( pul_चळत[ 2 ] != ul_Check_मूल्य ) ||                                                \
            ( pul_चळत[ 3 ] != ul_Check_मूल्य ) )                                                 \
        {                                                                                       \
            अक्षर * pcOverflowTaskName = pxCurrentTCB->pcTaskName;                               \
            v_Application_चळत_Overflow_Hook( ( TaskHandle_t ) pxCurrentTCB, pcOverflowTaskName ); \
        }                                                                                       \
    } पर्यंत( 0 )

#endif /* #if( configCHECK_FOR_STACK_OVERFLOW > 1 ) */
/*-----------------------------------------------------------*/

#जर ( ( config_CHECK_FOR_चळत_OVERFLOW > 1 ) && ( port_चळत_GROWTH > 0 ) )

    #define task_CHECK_FOR_चळत_OVERFLOW()                                                                                                \
    करा {                                                                                                                                  \
        int8_t * pc_End_Of_चळत = ( int8_t * ) pxCurrentTCB->px_End_Of_चळत;                                                                  \
        स्थिर स्थिरांक uint8_t uc_Expected_चळत_Bytes[] = { tsk_चळत_FILL_BYTE, tsk_चळत_FILL_BYTE, tsk_चळत_FILL_BYTE, tsk_चळत_FILL_BYTE,   \
                                                        tsk_चळत_FILL_BYTE, tsk_चळत_FILL_BYTE, tsk_चळत_FILL_BYTE, tsk_चळत_FILL_BYTE,   \
                                                        tsk_चळत_FILL_BYTE, tsk_चळत_FILL_BYTE, tsk_चळत_FILL_BYTE, tsk_चळत_FILL_BYTE,   \
                                                        tsk_चळत_FILL_BYTE, tsk_चळत_FILL_BYTE, tsk_चळत_FILL_BYTE, tsk_चळत_FILL_BYTE,   \
                                                        tsk_चळत_FILL_BYTE, tsk_चळत_FILL_BYTE, tsk_चळत_FILL_BYTE, tsk_चळत_FILL_BYTE }; \
                                                                                                                                          \
                                                                                                                                          \
        pc_End_Of_चळत -= आकार( uc_Expected_चळत_Bytes );                                                                                   \
                                                                                                                                          \
        /* Has the extremity of the task stack ever been written over? */                                                                 \
        जर( स_भेद( ( शून्य * ) pc_End_Of_चळत, ( शून्य * ) uc_Expected_चळत_Bytes, आकार( uc_Expected_चळत_Bytes ) ) != 0 )                     \
        {                                                                                                                                 \
            अक्षर * pcOverflowTaskName = pxCurrentTCB->pcTaskName;                                                                         \
            v_Application_चळत_Overflow_Hook( ( TaskHandle_t ) pxCurrentTCB, pcOverflowTaskName );                                           \
        }                                                                                                                                 \
    } पर्यंत( 0 )

#endif /* #if( configCHECK_FOR_STACK_OVERFLOW > 1 ) */
/*-----------------------------------------------------------*/

/* Remove stack overflow macro if not being used. */
#ifndef task_CHECK_FOR_चळत_OVERFLOW
    #define task_CHECK_FOR_चळत_OVERFLOW()
#endif



#endif /* STACK_MACROS_H */
