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


#అన్తర్భావ <stdlib.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#నిర్వచించు MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#అన్తర్భావ "FreeRTOS.h"
#అన్తర్భావ "list.h"

/* The MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be
 * defined for the header files above, but not in this file, in order to
 * generate the correct privileged Vs unprivileged linkage and placement. */
#నిర్వచనం_రద్దు MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/*-----------------------------------------------------------
* PUBLIC LIST API documented in list.h
*----------------------------------------------------------*/

శూన్య vListInitialise( List_t * స్థిరరాశి pxList )
{
    traceENTER_vListInitialise( pxList );

    /* The list structure contains a list item which is used to mark the
     * end of the list.  To initialise the list the list end is inserted
     * as the only list entry. */
    pxList->px_సూచీ = ( List_అంశం_t * ) &( pxList->x_List_చివర );

    list_సమితి_మొదటి_LIST_అంశం_INTEGRITY_CHECK_విలువ( &( pxList->x_List_చివర ) );

    /* The list end value is the highest possible value in the list to
     * ensure it remains at the end of the list. */
    pxList->x_List_చివర.x_అంశం_విలువ = పోర్టు_MAX_DELAY;

    /* The list end next and previous pointers point to itself so we know
     * when the list is empty. */
    pxList->x_List_చివర.px_తదుపరి = ( List_అంశం_t * ) &( pxList->x_List_చివర );
    pxList->x_List_చివర.px_మునుపటి = ( List_అంశం_t * ) &( pxList->x_List_చివర );

    /* Initialize the remaining fields of xListEnd when it is a proper ListItem_t */
    #ఒకవేళ ( config_USE_MINI_LIST_అంశం == 0 )
    {
        pxList->x_List_చివర.pvOwner = ఖాళీ;
        pxList->x_List_చివర.pxContainer = ఖాళీ;
        list_సమితి_రెండవ_LIST_అంశం_INTEGRITY_CHECK_విలువ( &( pxList->x_List_చివర ) );
    }
    #అయితే_ముగింపు

    pxList->ux_సంఖ్య_యొక్క_వస్తువులు = ( UBaseType_t ) 0U;

    /* Write known values into the list if
     * configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES is set to 1. */
    list_సమితి_LIST_INTEGRITY_CHECK_1_విలువ( pxList );
    list_సమితి_LIST_INTEGRITY_CHECK_2_విలువ( pxList );

    traceRETURN_vListInitialise();
}
/*-----------------------------------------------------------*/

శూన్య v_List_Initialise_అంశం( List_అంశం_t * స్థిరరాశి px_అంశం )
{
    trace_ENTER_v_List_Initialise_అంశం( px_అంశం );

    /* Make sure the list item is not recorded as being on a list. */
    px_అంశం->pxContainer = ఖాళీ;

    /* Write known values into the list item if
     * configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES is set to 1. */
    list_సమితి_మొదటి_LIST_అంశం_INTEGRITY_CHECK_విలువ( px_అంశం );
    list_సమితి_రెండవ_LIST_అంశం_INTEGRITY_CHECK_విలువ( px_అంశం );

    trace_RETURN_v_List_Initialise_అంశం();
}
/*-----------------------------------------------------------*/

శూన్య v_List_చొప్పించు_చివర( List_t * స్థిరరాశి pxList,
                     List_అంశం_t * స్థిరరాశి px_New_List_అంశం )
{
    List_అంశం_t * స్థిరరాశి px_సూచీ = pxList->px_సూచీ;

    trace_ENTER_v_List_చొప్పించు_చివర( pxList, px_New_List_అంశం );

    /* Only effective when configASSERT() is also defined, these tests may catch
     * the list data structures being overwritten in memory.  They will not catch
     * data errors caused by incorrect configuration or use of FreeRTOS. */
    listTEST_LIST_INTEGRITY( pxList );
    list_TEST_LIST_అంశం_INTEGRITY( px_New_List_అంశం );

    /* Insert a new list item into pxList, but rather than sort the list,
     * makes the new list item the last item to be removed by a call to
     * listGET_OWNER_OF_NEXT_ENTRY(). */
    px_New_List_అంశం->px_తదుపరి = px_సూచీ;
    px_New_List_అంశం->px_మునుపటి = px_సూచీ->px_మునుపటి;

    /* Only used during decision coverage testing. */
    mtCOVERAGE_TEST_DELAY();

    px_సూచీ->px_మునుపటి->px_తదుపరి = px_New_List_అంశం;
    px_సూచీ->px_మునుపటి = px_New_List_అంశం;

    /* Remember which list the item is in. */
    px_New_List_అంశం->pxContainer = pxList;

    ( pxList->ux_సంఖ్య_యొక్క_వస్తువులు ) = ( UBaseType_t ) ( pxList->ux_సంఖ్య_యొక్క_వస్తువులు + 1U );

    trace_RETURN_v_List_చొప్పించు_చివర();
}
/*-----------------------------------------------------------*/

శూన్య v_List_చొప్పించు( List_t * స్థిరరాశి pxList,
                  List_అంశం_t * స్థిరరాశి px_New_List_అంశం )
{
    List_అంశం_t * pxIterator;
    స్థిరరాశి TickType_t x_విలువ_యొక్క_Insertion = px_New_List_అంశం->x_అంశం_విలువ;

    trace_ENTER_v_List_చొప్పించు( pxList, px_New_List_అంశం );

    /* Only effective when configASSERT() is also defined, these tests may catch
     * the list data structures being overwritten in memory.  They will not catch
     * data errors caused by incorrect configuration or use of FreeRTOS. */
    listTEST_LIST_INTEGRITY( pxList );
    list_TEST_LIST_అంశం_INTEGRITY( px_New_List_అంశం );

    /* Insert the new list item into the list, sorted in xItemValue order.
     *
     * If the list already contains a list item with the same item value then the
     * new list item should be placed after it.  This ensures that TCBs which are
     * stored in ready lists (all of which have the same xItemValue value) get a
     * share of the CPU.  However, if the xItemValue is the same as the back marker
     * the iteration loop below will not end.  Therefore the value is checked
     * first, and the algorithm slightly modified if necessary. */
    ఒకవేళ( x_విలువ_యొక్క_Insertion == పోర్టు_MAX_DELAY )
    {
        pxIterator = pxList->x_List_చివర.px_మునుపటి;
    }
    లేకపోతే
    {
        /* *** NOTE ***********************************************************
        *  If you find your application is crashing here then likely causes are
        *  listed below.  In addition see https://www.FreeRTOS.org/FAQHelp.html for
        *  more tips, and ensure configASSERT() is defined!
        *  https://www.FreeRTOS.org/a00110.html#configASSERT
        *
        *   1) Stack overflow -
        *      see https://www.FreeRTOS.org/Stacks-and-stack-overflow-checking.html
        *   2) Incorrect interrupt priority assignment, especially on Cortex-M
        *      parts where numerically high priority values denote low actual
        *      interrupt priorities, which can seem counter intuitive.  See
        *      https://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html and the definition
        *      of configMAX_SYSCALL_INTERRUPT_PRIORITY on
        *      https://www.FreeRTOS.org/a00110.html
        *   3) Calling an API function from within a critical section or when
        *      the scheduler is suspended, or calling an API function that does
        *      not end in "FromISR" from an interrupt.
        *   4) Using a queue or semaphore before it has been initialised or
        *      before the scheduler has been started (are interrupts firing
        *      before vTaskStartScheduler() has been called?).
        *   5) If the FreeRTOS port supports interrupt nesting then ensure that
        *      the priority of the tick interrupt is at or below
        *      configMAX_SYSCALL_INTERRUPT_PRIORITY.
        **********************************************************************/

        కొరకు( pxIterator = ( List_అంశం_t * ) &( pxList->x_List_చివర ); pxIterator->px_తదుపరి->x_అంశం_విలువ <= x_విలువ_యొక్క_Insertion; pxIterator = pxIterator->px_తదుపరి )
        {
            /* There is nothing to do here, just iterating to the wanted
             * insertion position. */
        }
    }

    px_New_List_అంశం->px_తదుపరి = pxIterator->px_తదుపరి;
    px_New_List_అంశం->px_తదుపరి->px_మునుపటి = px_New_List_అంశం;
    px_New_List_అంశం->px_మునుపటి = pxIterator;
    pxIterator->px_తదుపరి = px_New_List_అంశం;

    /* Remember which list the item is in.  This allows fast removal of the
     * item later. */
    px_New_List_అంశం->pxContainer = pxList;

    ( pxList->ux_సంఖ్య_యొక్క_వస్తువులు ) = ( UBaseType_t ) ( pxList->ux_సంఖ్య_యొక్క_వస్తువులు + 1U );

    trace_RETURN_v_List_చొప్పించు();
}
/*-----------------------------------------------------------*/


UBaseType_t uxListRemove( List_అంశం_t * స్థిరరాశి px_అంశం_గమ్యం_Remove )
{
    /* The list item knows which list it is in.  Obtain the list from the list
     * item. */
    List_t * స్థిరరాశి pxList = px_అంశం_గమ్యం_Remove->pxContainer;

    traceENTER_uxListRemove( px_అంశం_గమ్యం_Remove );

    px_అంశం_గమ్యం_Remove->px_తదుపరి->px_మునుపటి = px_అంశం_గమ్యం_Remove->px_మునుపటి;
    px_అంశం_గమ్యం_Remove->px_మునుపటి->px_తదుపరి = px_అంశం_గమ్యం_Remove->px_తదుపరి;

    /* Only used during decision coverage testing. */
    mtCOVERAGE_TEST_DELAY();

    /* Make sure the index is left pointing to a valid item. */
    ఒకవేళ( pxList->px_సూచీ == px_అంశం_గమ్యం_Remove )
    {
        pxList->px_సూచీ = px_అంశం_గమ్యం_Remove->px_మునుపటి;
    }
    లేకపోతే
    {
        mtCOVERAGE_TEST_MARKER();
    }

    px_అంశం_గమ్యం_Remove->pxContainer = ఖాళీ;
    ( pxList->ux_సంఖ్య_యొక్క_వస్తువులు ) = ( UBaseType_t ) ( pxList->ux_సంఖ్య_యొక్క_వస్తువులు - 1U );

    traceRETURN_uxListRemove( pxList->ux_సంఖ్య_యొక్క_వస్తువులు );

    తిరిగి pxList->ux_సంఖ్య_యొక్క_వస్తువులు;
}
/*-----------------------------------------------------------*/
