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

#నిర్వచితం_కాకపోతే దొంతర_MACROS_H
#నిర్వచించు దొంతర_MACROS_H

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
#నిర్వచితం_కాకపోతే పోర్టు_దొంతర_హద్దు_PADDING
    #నిర్వచించు పోర్టు_దొంతర_హద్దు_PADDING    0
#అయితే_ముగింపు

#ఒకవేళ ( ( config_CHECK_FOR_దొంతర_అధికప్రవాహం == 1 ) && ( పోర్టు_దొంతర_GROWTH < 0 ) )

/* Only the current stack state is to be checked. */
    #నిర్వచించు task_CHECK_FOR_దొంతర_అధికప్రవాహం( xCurCoreID )                                                                              \
    {                                                                                                                               \
        /* Is the currently saved stack pointer within the stack limit? */                                                          \
        ఒకవేళ( pxCurrentTCBs[ xCurCoreID ]->px_శిఖరం_యొక్క_దొంతర <= pxCurrentTCBs[ xCurCoreID ]->px_దొంతర + పోర్టు_దొంతర_హద్దు_PADDING )           \
        {                                                                                                                           \
            v_Application_దొంతర_అధికప్రవాహం_Hook( ( TaskHandle_t ) pxCurrentTCBs[ xCurCoreID ], pxCurrentTCBs[ xCurCoreID ]->pcTaskName ); \
        }                                                                                                                           \
    }

#అయితే_ముగింపు /* configCHECK_FOR_STACK_OVERFLOW == 1 */
/*-----------------------------------------------------------*/

#ఒకవేళ ( ( config_CHECK_FOR_దొంతర_అధికప్రవాహం == 1 ) && ( పోర్టు_దొంతర_GROWTH > 0 ) )

/* Only the current stack state is to be checked. */
    #నిర్వచించు task_CHECK_FOR_దొంతర_అధికప్రవాహం( xCurCoreID )                                                                              \
    {                                                                                                                               \
                                                                                                                                    \
        /* Is the currently saved stack pointer within the stack limit? */                                                          \
        ఒకవేళ( pxCurrentTCBs[ xCurCoreID ]->px_శిఖరం_యొక్క_దొంతర >= pxCurrentTCBs[ xCurCoreID ]->px_చివర_యొక్క_దొంతర - పోర్టు_దొంతర_హద్దు_PADDING )      \
        {                                                                                                                           \
            v_Application_దొంతర_అధికప్రవాహం_Hook( ( TaskHandle_t ) pxCurrentTCBs[ xCurCoreID ], pxCurrentTCBs[ xCurCoreID ]->pcTaskName ); \
        }                                                                                                                           \
    }

#అయితే_ముగింపు /* configCHECK_FOR_STACK_OVERFLOW == 1 */
/*-----------------------------------------------------------*/

#ఒకవేళ ( ( config_CHECK_FOR_దొంతర_అధికప్రవాహం > 1 ) && ( పోర్టు_దొంతర_GROWTH < 0 ) )

    #నిర్వచించు task_CHECK_FOR_దొంతర_అధికప్రవాహం( xCurCoreID )                                                                              \
    {                                                                                                                               \
        స్థిరరాశి uint32_t * స్థిరరాశి pul_దొంతర = ( uint32_t * ) pxCurrentTCBs[ xCurCoreID ]->px_దొంతర;                                      \
        స్థిరరాశి uint32_t ul_Check_విలువ = ( uint32_t ) 0xa5a5a5a5;                                                                      \
                                                                                                                                    \
        ఒకవేళ( ( pul_దొంతర[ 0 ] != ul_Check_విలువ ) ||                                                                                    \
            ( pul_దొంతర[ 1 ] != ul_Check_విలువ ) ||                                                                                    \
            ( pul_దొంతర[ 2 ] != ul_Check_విలువ ) ||                                                                                    \
            ( pul_దొంతర[ 3 ] != ul_Check_విలువ ) )                                                                                     \
        {                                                                                                                           \
            v_Application_దొంతర_అధికప్రవాహం_Hook( ( TaskHandle_t ) pxCurrentTCBs[ xCurCoreID ], pxCurrentTCBs[ xCurCoreID ]->pcTaskName ); \
        }                                                                                                                           \
    }

#అయితే_ముగింపు /* #if( configCHECK_FOR_STACK_OVERFLOW > 1 ) */
/*-----------------------------------------------------------*/

#ఒకవేళ ( ( config_CHECK_FOR_దొంతర_అధికప్రవాహం > 1 ) && ( పోర్టు_దొంతర_GROWTH > 0 ) )

    #నిర్వచించు task_CHECK_FOR_దొంతర_అధికప్రవాహం( xCurCoreID )                                                                                    \
    {                                                                                                                                     \
        int8_t * pc_చివర_యొక్క_దొంతర = ( int8_t * ) pxCurrentTCBs[ xCurCoreID ]->px_చివర_యొక్క_దొంతర;                                                   \
        స్థిర స్థిరరాశి uint8_t uc_Expected_దొంతర_బైట్లు[] = { tsk_దొంతర_FILL_BYTE, tsk_దొంతర_FILL_BYTE, tsk_దొంతర_FILL_BYTE, tsk_దొంతర_FILL_BYTE,   \
                                                        tsk_దొంతర_FILL_BYTE, tsk_దొంతర_FILL_BYTE, tsk_దొంతర_FILL_BYTE, tsk_దొంతర_FILL_BYTE,   \
                                                        tsk_దొంతర_FILL_BYTE, tsk_దొంతర_FILL_BYTE, tsk_దొంతర_FILL_BYTE, tsk_దొంతర_FILL_BYTE,   \
                                                        tsk_దొంతర_FILL_BYTE, tsk_దొంతర_FILL_BYTE, tsk_దొంతర_FILL_BYTE, tsk_దొంతర_FILL_BYTE,   \
                                                        tsk_దొంతర_FILL_BYTE, tsk_దొంతర_FILL_BYTE, tsk_దొంతర_FILL_BYTE, tsk_దొంతర_FILL_BYTE }; \
                                                                                                                                          \
                                                                                                                                          \
        pc_చివర_యొక్క_దొంతర -= పరిమాణం( uc_Expected_దొంతర_బైట్లు );                                                                                   \
                                                                                                                                          \
        /* Has the extremity of the task stack ever been written over? */                                                                 \
        ఒకవేళ( memcmp( ( శూన్య * ) pc_చివర_యొక్క_దొంతర, ( శూన్య * ) uc_Expected_దొంతర_బైట్లు, పరిమాణం( uc_Expected_దొంతర_బైట్లు ) ) != 0 )                     \
        {                                                                                                                                 \
            v_Application_దొంతర_అధికప్రవాహం_Hook( ( TaskHandle_t ) pxCurrentTCBs[ xCurCoreID ], pxCurrentTCBs[ xCurCoreID ]->pcTaskName );       \
        }                                                                                                                                 \
    }

#అయితే_ముగింపు /* #if( configCHECK_FOR_STACK_OVERFLOW > 1 ) */
/*-----------------------------------------------------------*/

/* Remove stack overflow macro if not being used. */
#నిర్వచితం_కాకపోతే task_CHECK_FOR_దొంతర_అధికప్రవాహం
    #నిర్వచించు task_CHECK_FOR_దొంతర_అధికప్రవాహం( xCurCoreID )
#అయితే_ముగింపు



#అయితే_ముగింపు /* STACK_MACROS_H */
