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


#समावेश <stdlib.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#घोषणा MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#समावेश "FreeRTOS.h"
#समावेश "list.h"

/* The MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be
 * defined for the header files above, but not in this file, in order to
 * generate the correct privileged Vs unprivileged linkage and placement. */
#अघोषित MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/*-----------------------------------------------------------
* PUBLIC LIST API documented in list.h
*----------------------------------------------------------*/

व्योम vListInitialise( List_t * स्थिर pxList )
{
    traceENTER_vListInitialise( pxList );

    /* The list structure contains a list item which is used to mark the
     * end of the list.  To initialise the list the list end is inserted
     * as the only list entry. */
    pxList->px_सूचकांक = ( ListItem_t * ) &( pxList->xListEnd );

    list_SET_FIRST_LIST_ITEM_INTEGRITY_CHECK_मूल्य( &( pxList->xListEnd ) );

    /* The list end value is the highest possible value in the list to
     * ensure it remains at the end of the list. */
    pxList->xListEnd.x_Item_मूल्य = portMAX_DELAY;

    /* The list end next and previous pointers point to itself so we know
     * when the list is empty. */
    pxList->xListEnd.px_अग्र = ( ListItem_t * ) &( pxList->xListEnd );
    pxList->xListEnd.pxPrevious = ( ListItem_t * ) &( pxList->xListEnd );

    /* Initialize the remaining fields of xListEnd when it is a proper ListItem_t */
    #अगर ( configUSE_MINI_LIST_ITEM == 0 )
    {
        pxList->xListEnd.pvOwner = शून्य;
        pxList->xListEnd.pxContainer = शून्य;
        list_SET_SECOND_LIST_ITEM_INTEGRITY_CHECK_मूल्य( &( pxList->xListEnd ) );
    }
    #पूर्ण_अगर

    pxList->ux_संख्या_Of_वस्तुएँ = ( UBaseType_t ) 0U;

    /* Write known values into the list if
     * configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES is set to 1. */
    list_SET_LIST_INTEGRITY_CHECK_1_मूल्य( pxList );
    list_SET_LIST_INTEGRITY_CHECK_2_मूल्य( pxList );

    traceRETURN_vListInitialise();
}
/*-----------------------------------------------------------*/

व्योम vListInitialiseItem( ListItem_t * स्थिर pxItem )
{
    traceENTER_vListInitialiseItem( pxItem );

    /* Make sure the list item is not recorded as being on a list. */
    pxItem->pxContainer = शून्य;

    /* Write known values into the list item if
     * configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES is set to 1. */
    list_SET_FIRST_LIST_ITEM_INTEGRITY_CHECK_मूल्य( pxItem );
    list_SET_SECOND_LIST_ITEM_INTEGRITY_CHECK_मूल्य( pxItem );

    traceRETURN_vListInitialiseItem();
}
/*-----------------------------------------------------------*/

व्योम v_List_जोड़ो_End( List_t * स्थिर pxList,
                     ListItem_t * स्थिर pxNewListItem )
{
    ListItem_t * स्थिर px_सूचकांक = pxList->px_सूचकांक;

    trace_ENTER_v_List_जोड़ो_End( pxList, pxNewListItem );

    /* Only effective when configASSERT() is also defined, these tests may catch
     * the list data structures being overwritten in memory.  They will not catch
     * data errors caused by incorrect configuration or use of FreeRTOS. */
    listTEST_LIST_INTEGRITY( pxList );
    listTEST_LIST_ITEM_INTEGRITY( pxNewListItem );

    /* Insert a new list item into pxList, but rather than sort the list,
     * makes the new list item the last item to be removed by a call to
     * listGET_OWNER_OF_NEXT_ENTRY(). */
    pxNewListItem->px_अग्र = px_सूचकांक;
    pxNewListItem->pxPrevious = px_सूचकांक->pxPrevious;

    /* Only used during decision coverage testing. */
    mtCOVERAGE_TEST_DELAY();

    px_सूचकांक->pxPrevious->px_अग्र = pxNewListItem;
    px_सूचकांक->pxPrevious = pxNewListItem;

    /* Remember which list the item is in. */
    pxNewListItem->pxContainer = pxList;

    ( pxList->ux_संख्या_Of_वस्तुएँ ) = ( UBaseType_t ) ( pxList->ux_संख्या_Of_वस्तुएँ + 1U );

    trace_RETURN_v_List_जोड़ो_End();
}
/*-----------------------------------------------------------*/

व्योम v_List_जोड़ो( List_t * स्थिर pxList,
                  ListItem_t * स्थिर pxNewListItem )
{
    ListItem_t * pxIterator;
    स्थिर TickType_t x_मूल्य_Of_Insertion = pxNewListItem->x_Item_मूल्य;

    trace_ENTER_v_List_जोड़ो( pxList, pxNewListItem );

    /* Only effective when configASSERT() is also defined, these tests may catch
     * the list data structures being overwritten in memory.  They will not catch
     * data errors caused by incorrect configuration or use of FreeRTOS. */
    listTEST_LIST_INTEGRITY( pxList );
    listTEST_LIST_ITEM_INTEGRITY( pxNewListItem );

    /* Insert the new list item into the list, sorted in xItemValue order.
     *
     * If the list already contains a list item with the same item value then the
     * new list item should be placed after it.  This ensures that TCBs which are
     * stored in ready lists (all of which have the same xItemValue value) get a
     * share of the CPU.  However, if the xItemValue is the same as the back marker
     * the iteration loop below will not end.  Therefore the value is checked
     * first, and the algorithm slightly modified if necessary. */
    अगर( x_मूल्य_Of_Insertion == portMAX_DELAY )
    {
        pxIterator = pxList->xListEnd.pxPrevious;
    }
    अन्यथा
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

        क्रम( pxIterator = ( ListItem_t * ) &( pxList->xListEnd ); pxIterator->px_अग्र->x_Item_मूल्य <= x_मूल्य_Of_Insertion; pxIterator = pxIterator->px_अग्र )
        {
            /* There is nothing to do here, just iterating to the wanted
             * insertion position. */
        }
    }

    pxNewListItem->px_अग्र = pxIterator->px_अग्र;
    pxNewListItem->px_अग्र->pxPrevious = pxNewListItem;
    pxNewListItem->pxPrevious = pxIterator;
    pxIterator->px_अग्र = pxNewListItem;

    /* Remember which list the item is in.  This allows fast removal of the
     * item later. */
    pxNewListItem->pxContainer = pxList;

    ( pxList->ux_संख्या_Of_वस्तुएँ ) = ( UBaseType_t ) ( pxList->ux_संख्या_Of_वस्तुएँ + 1U );

    trace_RETURN_v_List_जोड़ो();
}
/*-----------------------------------------------------------*/


UBaseType_t uxListRemove( ListItem_t * स्थिर pxItemToRemove )
{
    /* The list item knows which list it is in.  Obtain the list from the list
     * item. */
    List_t * स्थिर pxList = pxItemToRemove->pxContainer;

    traceENTER_uxListRemove( pxItemToRemove );

    pxItemToRemove->px_अग्र->pxPrevious = pxItemToRemove->pxPrevious;
    pxItemToRemove->pxPrevious->px_अग्र = pxItemToRemove->px_अग्र;

    /* Only used during decision coverage testing. */
    mtCOVERAGE_TEST_DELAY();

    /* Make sure the index is left pointing to a valid item. */
    अगर( pxList->px_सूचकांक == pxItemToRemove )
    {
        pxList->px_सूचकांक = pxItemToRemove->pxPrevious;
    }
    अन्यथा
    {
        mtCOVERAGE_TEST_MARKER();
    }

    pxItemToRemove->pxContainer = शून्य;
    ( pxList->ux_संख्या_Of_वस्तुएँ ) = ( UBaseType_t ) ( pxList->ux_संख्या_Of_वस्तुएँ - 1U );

    traceRETURN_uxListRemove( pxList->ux_संख्या_Of_वस्तुएँ );

    वापस pxList->ux_संख्या_Of_वस्तुएँ;
}
/*-----------------------------------------------------------*/
