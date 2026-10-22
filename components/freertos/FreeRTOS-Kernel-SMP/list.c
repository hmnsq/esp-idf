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
#تعریف_کن MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#ضمن "FreeRTOS.h"
#ضمن "list.h"

/* The MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be
 * defined for the header files above, but not in this file, in order to
 * generate the correct privileged Vs unprivileged linkage and placement. */
#لغو_تعریف MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/*-----------------------------------------------------------
* PUBLIC LIST API documented in list.h
*----------------------------------------------------------*/

فراغ vListInitialise( List_t * ثابت pxList )
{
    traceENTER_vListInitialise( pxList );

    /* The list structure contains a list item which is used to mark the
     * end of the list.  To initialise the list the list end is inserted
     * as the only list entry. */
    pxList->px_شاخص = ( List_قلم_t * ) &( pxList->x_List_انتها );

    list_مجموعه_اولی_LIST_قلم_INTEGRITY_CHECK_مقدار( &( pxList->x_List_انتها ) );

    /* The list end value is the highest possible value in the list to
     * ensure it remains at the end of the list. */
    pxList->x_List_انتها.x_قلم_مقدار = پورت_MAX_DELAY;

    /* The list end next and previous pointers point to itself so we know
     * when the list is empty. */
    pxList->x_List_انتها.px_بعدی = ( List_قلم_t * ) &( pxList->x_List_انتها );
    pxList->x_List_انتها.px_قبلی = ( List_قلم_t * ) &( pxList->x_List_انتها );

    /* Initialize the remaining fields of xListEnd when it is a proper ListItem_t */
    #إذا ( config_USE_MINI_LIST_قلم == 0 )
    {
        pxList->x_List_انتها.pvOwner = عدم;
        pxList->x_List_انتها.pxContainer = عدم;
        list_مجموعه_دومی_LIST_قلم_INTEGRITY_CHECK_مقدار( &( pxList->x_List_انتها ) );
    }
    #پایان_اگر

    pxList->ux_عدد_از_اقلام = ( UBaseType_t ) 0U;

    /* Write known values into the list if
     * configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES is set to 1. */
    list_مجموعه_LIST_INTEGRITY_CHECK_1_مقدار( pxList );
    list_مجموعه_LIST_INTEGRITY_CHECK_2_مقدار( pxList );

    traceRETURN_vListInitialise();
}
/*-----------------------------------------------------------*/

فراغ v_List_Initialise_قلم( List_قلم_t * ثابت px_قلم )
{
    trace_ENTER_v_List_Initialise_قلم( px_قلم );

    /* Make sure the list item is not recorded as being on a list. */
    px_قلم->pxContainer = عدم;

    /* Write known values into the list item if
     * configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES is set to 1. */
    list_مجموعه_اولی_LIST_قلم_INTEGRITY_CHECK_مقدار( px_قلم );
    list_مجموعه_دومی_LIST_قلم_INTEGRITY_CHECK_مقدار( px_قلم );

    trace_RETURN_v_List_Initialise_قلم();
}
/*-----------------------------------------------------------*/

فراغ v_List_درج_انتها( List_t * ثابت pxList,
                     List_قلم_t * ثابت px_New_List_قلم )
{
    List_قلم_t * ثابت px_شاخص = pxList->px_شاخص;

    trace_ENTER_v_List_درج_انتها( pxList, px_New_List_قلم );

    /* Only effective when configASSERT() is also defined, these tests may catch
     * the list data structures being overwritten in memory.  They will not catch
     * data errors caused by incorrect configuration or use of FreeRTOS. */
    listTEST_LIST_INTEGRITY( pxList );
    list_TEST_LIST_قلم_INTEGRITY( px_New_List_قلم );

    /* Insert a new list item into pxList, but rather than sort the list,
     * makes the new list item the last item to be removed by a call to
     * listGET_OWNER_OF_NEXT_ENTRY(). */
    px_New_List_قلم->px_بعدی = px_شاخص;
    px_New_List_قلم->px_قبلی = px_شاخص->px_قبلی;

    /* Only used during decision coverage testing. */
    mtCOVERAGE_TEST_DELAY();

    px_شاخص->px_قبلی->px_بعدی = px_New_List_قلم;
    px_شاخص->px_قبلی = px_New_List_قلم;

    /* Remember which list the item is in. */
    px_New_List_قلم->pxContainer = pxList;

    ( pxList->ux_عدد_از_اقلام ) = ( UBaseType_t ) ( pxList->ux_عدد_از_اقلام + 1U );

    trace_RETURN_v_List_درج_انتها();
}
/*-----------------------------------------------------------*/

فراغ v_List_درج( List_t * ثابت pxList,
                  List_قلم_t * ثابت px_New_List_قلم )
{
    List_قلم_t * pxIterator;
    ثابت TickType_t x_مقدار_از_Insertion = px_New_List_قلم->x_قلم_مقدار;

    trace_ENTER_v_List_درج( pxList, px_New_List_قلم );

    /* Only effective when configASSERT() is also defined, these tests may catch
     * the list data structures being overwritten in memory.  They will not catch
     * data errors caused by incorrect configuration or use of FreeRTOS. */
    listTEST_LIST_INTEGRITY( pxList );
    list_TEST_LIST_قلم_INTEGRITY( px_New_List_قلم );

    /* Insert the new list item into the list, sorted in xItemValue order.
     *
     * If the list already contains a list item with the same item value then the
     * new list item should be placed after it.  This ensures that TCBs which are
     * stored in ready lists (all of which have the same xItemValue value) get a
     * share of the CPU.  However, if the xItemValue is the same as the back marker
     * the iteration loop below will not end.  Therefore the value is checked
     * first, and the algorithm slightly modified if necessary. */
    إذا( x_مقدار_از_Insertion == پورت_MAX_DELAY )
    {
        pxIterator = pxList->x_List_انتها.px_قبلی;
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

        لكل( pxIterator = ( List_قلم_t * ) &( pxList->x_List_انتها ); pxIterator->px_بعدی->x_قلم_مقدار <= x_مقدار_از_Insertion; pxIterator = pxIterator->px_بعدی )
        {
            /* There is nothing to do here, just iterating to the wanted
             * insertion position. */
        }
    }

    px_New_List_قلم->px_بعدی = pxIterator->px_بعدی;
    px_New_List_قلم->px_بعدی->px_قبلی = px_New_List_قلم;
    px_New_List_قلم->px_قبلی = pxIterator;
    pxIterator->px_بعدی = px_New_List_قلم;

    /* Remember which list the item is in.  This allows fast removal of the
     * item later. */
    px_New_List_قلم->pxContainer = pxList;

    ( pxList->ux_عدد_از_اقلام ) = ( UBaseType_t ) ( pxList->ux_عدد_از_اقلام + 1U );

    trace_RETURN_v_List_درج();
}
/*-----------------------------------------------------------*/


UBaseType_t uxListRemove( List_قلم_t * ثابت px_قلم_مقصد_Remove )
{
    /* The list item knows which list it is in.  Obtain the list from the list
     * item. */
    List_t * ثابت pxList = px_قلم_مقصد_Remove->pxContainer;

    traceENTER_uxListRemove( px_قلم_مقصد_Remove );

    px_قلم_مقصد_Remove->px_بعدی->px_قبلی = px_قلم_مقصد_Remove->px_قبلی;
    px_قلم_مقصد_Remove->px_قبلی->px_بعدی = px_قلم_مقصد_Remove->px_بعدی;

    /* Only used during decision coverage testing. */
    mtCOVERAGE_TEST_DELAY();

    /* Make sure the index is left pointing to a valid item. */
    إذا( pxList->px_شاخص == px_قلم_مقصد_Remove )
    {
        pxList->px_شاخص = px_قلم_مقصد_Remove->px_قبلی;
    }
    وإلا
    {
        mtCOVERAGE_TEST_MARKER();
    }

    px_قلم_مقصد_Remove->pxContainer = عدم;
    ( pxList->ux_عدد_از_اقلام ) = ( UBaseType_t ) ( pxList->ux_عدد_از_اقلام - 1U );

    traceRETURN_uxListRemove( pxList->ux_عدد_از_اقلام );

    أرجع pxList->ux_عدد_از_اقلام;
}
/*-----------------------------------------------------------*/
