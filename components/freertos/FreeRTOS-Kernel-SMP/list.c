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


#ضمن <stdlib.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#تعریف_کړه MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#ضمن "FreeRTOS.h"
#ضمن "list.h"

/* The MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be
 * defined for the header files above, but not in this file, in order to
 * generate the correct privileged Vs unprivileged linkage and placement. */
#تعریف_لغوه MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/*-----------------------------------------------------------
* PUBLIC LIST API documented in list.h
*----------------------------------------------------------*/

فراغ vListInitialise( List_t * ثابت pxList )
{
    traceENTER_vListInitialise( pxList );

    /* The list structure contains a list item which is used to mark the
     * end of the list.  To initialise the list the list end is inserted
     * as the only list entry. */
    pxList->px_شاخص = ( List_توکی_t * ) &( pxList->x_List_پای_ټکی );

    list_سېټ_لومړی_LIST_توکی_INTEGRITY_CHECK_ارزښت( &( pxList->x_List_پای_ټکی ) );

    /* The list end value is the highest possible value in the list to
     * ensure it remains at the end of the list. */
    pxList->x_List_پای_ټکی.x_توکی_ارزښت = پورټ_MAX_DELAY;

    /* The list end next and previous pointers point to itself so we know
     * when the list is empty. */
    pxList->x_List_پای_ټکی.px_بل = ( List_توکی_t * ) &( pxList->x_List_پای_ټکی );
    pxList->x_List_پای_ټکی.px_پخوانی = ( List_توکی_t * ) &( pxList->x_List_پای_ټکی );

    /* Initialize the remaining fields of xListEnd when it is a proper ListItem_t */
    #إذا ( config_USE_MINI_LIST_توکی == 0 )
    {
        pxList->x_List_پای_ټکی.pvOwner = عدم;
        pxList->x_List_پای_ټکی.pxContainer = عدم;
        list_سېټ_دوهم_LIST_توکی_INTEGRITY_CHECK_ارزښت( &( pxList->x_List_پای_ټکی ) );
    }
    #د_که_پای

    pxList->ux_شمېره_د_توکي = ( UBaseType_t ) 0U;

    /* Write known values into the list if
     * configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES is set to 1. */
    list_سېټ_LIST_INTEGRITY_CHECK_1_ارزښت( pxList );
    list_سېټ_LIST_INTEGRITY_CHECK_2_ارزښت( pxList );

    traceRETURN_vListInitialise();
}
/*-----------------------------------------------------------*/

فراغ v_List_Initialise_توکی( List_توکی_t * ثابت px_توکی )
{
    trace_ENTER_v_List_Initialise_توکی( px_توکی );

    /* Make sure the list item is not recorded as being on a list. */
    px_توکی->pxContainer = عدم;

    /* Write known values into the list item if
     * configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES is set to 1. */
    list_سېټ_لومړی_LIST_توکی_INTEGRITY_CHECK_ارزښت( px_توکی );
    list_سېټ_دوهم_LIST_توکی_INTEGRITY_CHECK_ارزښت( px_توکی );

    trace_RETURN_v_List_Initialise_توکی();
}
/*-----------------------------------------------------------*/

فراغ v_List_ورننباسه_پای_ټکی( List_t * ثابت pxList,
                     List_توکی_t * ثابت px_New_List_توکی )
{
    List_توکی_t * ثابت px_شاخص = pxList->px_شاخص;

    trace_ENTER_v_List_ورننباسه_پای_ټکی( pxList, px_New_List_توکی );

    /* Only effective when configASSERT() is also defined, these tests may catch
     * the list data structures being overwritten in memory.  They will not catch
     * data errors caused by incorrect configuration or use of FreeRTOS. */
    listTEST_LIST_INTEGRITY( pxList );
    list_TEST_LIST_توکی_INTEGRITY( px_New_List_توکی );

    /* Insert a new list item into pxList, but rather than sort the list,
     * makes the new list item the last item to be removed by a call to
     * listGET_OWNER_OF_NEXT_ENTRY(). */
    px_New_List_توکی->px_بل = px_شاخص;
    px_New_List_توکی->px_پخوانی = px_شاخص->px_پخوانی;

    /* Only used during decision coverage testing. */
    mtCOVERAGE_TEST_DELAY();

    px_شاخص->px_پخوانی->px_بل = px_New_List_توکی;
    px_شاخص->px_پخوانی = px_New_List_توکی;

    /* Remember which list the item is in. */
    px_New_List_توکی->pxContainer = pxList;

    ( pxList->ux_شمېره_د_توکي ) = ( UBaseType_t ) ( pxList->ux_شمېره_د_توکي + 1U );

    trace_RETURN_v_List_ورننباسه_پای_ټکی();
}
/*-----------------------------------------------------------*/

فراغ v_List_ورننباسه( List_t * ثابت pxList,
                  List_توکی_t * ثابت px_New_List_توکی )
{
    List_توکی_t * pxIterator;
    ثابت TickType_t x_ارزښت_د_Insertion = px_New_List_توکی->x_توکی_ارزښت;

    trace_ENTER_v_List_ورننباسه( pxList, px_New_List_توکی );

    /* Only effective when configASSERT() is also defined, these tests may catch
     * the list data structures being overwritten in memory.  They will not catch
     * data errors caused by incorrect configuration or use of FreeRTOS. */
    listTEST_LIST_INTEGRITY( pxList );
    list_TEST_LIST_توکی_INTEGRITY( px_New_List_توکی );

    /* Insert the new list item into the list, sorted in xItemValue order.
     *
     * If the list already contains a list item with the same item value then the
     * new list item should be placed after it.  This ensures that TCBs which are
     * stored in ready lists (all of which have the same xItemValue value) get a
     * share of the CPU.  However, if the xItemValue is the same as the back marker
     * the iteration loop below will not end.  Therefore the value is checked
     * first, and the algorithm slightly modified if necessary. */
    إذا( x_ارزښت_د_Insertion == پورټ_MAX_DELAY )
    {
        pxIterator = pxList->x_List_پای_ټکی.px_پخوانی;
    }
    وإلا
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

        لكل( pxIterator = ( List_توکی_t * ) &( pxList->x_List_پای_ټکی ); pxIterator->px_بل->x_توکی_ارزښت <= x_ارزښت_د_Insertion; pxIterator = pxIterator->px_بل )
        {
            /* There is nothing to do here, just iterating to the wanted
             * insertion position. */
        }
    }

    px_New_List_توکی->px_بل = pxIterator->px_بل;
    px_New_List_توکی->px_بل->px_پخوانی = px_New_List_توکی;
    px_New_List_توکی->px_پخوانی = pxIterator;
    pxIterator->px_بل = px_New_List_توکی;

    /* Remember which list the item is in.  This allows fast removal of the
     * item later. */
    px_New_List_توکی->pxContainer = pxList;

    ( pxList->ux_شمېره_د_توکي ) = ( UBaseType_t ) ( pxList->ux_شمېره_د_توکي + 1U );

    trace_RETURN_v_List_ورننباسه();
}
/*-----------------------------------------------------------*/


UBaseType_t uxListRemove( List_توکی_t * ثابت px_توکی_منزل_Remove )
{
    /* The list item knows which list it is in.  Obtain the list from the list
     * item. */
    List_t * ثابت pxList = px_توکی_منزل_Remove->pxContainer;

    traceENTER_uxListRemove( px_توکی_منزل_Remove );

    px_توکی_منزل_Remove->px_بل->px_پخوانی = px_توکی_منزل_Remove->px_پخوانی;
    px_توکی_منزل_Remove->px_پخوانی->px_بل = px_توکی_منزل_Remove->px_بل;

    /* Only used during decision coverage testing. */
    mtCOVERAGE_TEST_DELAY();

    /* Make sure the index is left pointing to a valid item. */
    إذا( pxList->px_شاخص == px_توکی_منزل_Remove )
    {
        pxList->px_شاخص = px_توکی_منزل_Remove->px_پخوانی;
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
    }

    px_توکی_منزل_Remove->pxContainer = عدم;
    ( pxList->ux_شمېره_د_توکي ) = ( UBaseType_t ) ( pxList->ux_شمېره_د_توکي - 1U );

    traceRETURN_uxListRemove( pxList->ux_شمېره_د_توکي );

    أرجع pxList->ux_شمېره_د_توکي;
}
/*-----------------------------------------------------------*/
