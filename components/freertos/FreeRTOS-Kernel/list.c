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


#ಅನ್ತರ್ಭಾವ <stdlib.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#ವ್ಯಾಖ್ಯಾನಿಸು MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#ಅನ್ತರ್ಭಾವ "FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "list.h"

/* Lint e9021, e961 and e750 are suppressed as a MISRA exception justified
 * because the MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be
 * defined for the header files above, but not in this file, in order to
 * generate the correct privileged Vs unprivileged linkage and placement. */
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು MPU_WRAPPERS_INCLUDED_FROM_API_FILE /*lint !e961 !e750 !e9021. */

/*-----------------------------------------------------------
* PUBLIC LIST API documented in list.h
*----------------------------------------------------------*/

ಶೂನ್ಯ vListInitialise( List_t * ಸ್ಥಿರಾಂಕ pxList )
{
    /* The list structure contains a list item which is used to mark the
     * end of the list.  To initialise the list the list end is inserted
     * as the only list entry. */
    pxList->px_ಸೂಚ್ಯಂಕ = ( List_ಅಂಶ_t * ) &( pxList->x_List_ಕೊನೆ ); /*lint !e826 !e740 !e9087 The mini list structure is used as the list end to save RAM.  This is checked and valid. */

    list_ಗಣ_ಮೊದಲ_LIST_ಅಂಶ_INTEGRITY_CHECK_ಮೌಲ್ಯ( &( pxList->x_List_ಕೊನೆ ) );

    /* The list end value is the highest possible value in the list to
     * ensure it remains at the end of the list. */
    pxList->x_List_ಕೊನೆ.x_ಅಂಶ_ಮೌಲ್ಯ = ಪೋರ್ಟ್_MAX_DELAY;

    /* The list end next and previous pointers point to itself so we know
     * when the list is empty. */
    pxList->x_List_ಕೊನೆ.px_ಮುಂದಿನ = ( List_ಅಂಶ_t * ) &( pxList->x_List_ಕೊನೆ );     /*lint !e826 !e740 !e9087 The mini list structure is used as the list end to save RAM.  This is checked and valid. */
    pxList->x_List_ಕೊನೆ.px_ಹಿಂದಿನ = ( List_ಅಂಶ_t * ) &( pxList->x_List_ಕೊನೆ ); /*lint !e826 !e740 !e9087 The mini list structure is used as the list end to save RAM.  This is checked and valid. */

    /* Initialize the remaining fields of xListEnd when it is a proper ListItem_t */
    #ಆದರೆ ( config_USE_MINI_LIST_ಅಂಶ == 0 )
    {
        pxList->x_List_ಕೊನೆ.pvOwner = ಖಾಲಿ;
        pxList->x_List_ಕೊನೆ.pxContainer = ಖಾಲಿ;
        list_ಗಣ_ಎರಡನೇ_LIST_ಅಂಶ_INTEGRITY_CHECK_ಮೌಲ್ಯ( &( pxList->x_List_ಕೊನೆ ) );
    }
    #ಆದರೆ_ಅಂತ್ಯ

    pxList->ux_ಸಂಖ್ಯೆ_ದ_ವಸ್ತುಗಳು = ( UBaseType_t ) 0U;

    /* Write known values into the list if
     * configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES is set to 1. */
    list_ಗಣ_LIST_INTEGRITY_CHECK_1_ಮೌಲ್ಯ( pxList );
    list_ಗಣ_LIST_INTEGRITY_CHECK_2_ಮೌಲ್ಯ( pxList );
}
/*-----------------------------------------------------------*/

ಶೂನ್ಯ v_List_Initialise_ಅಂಶ( List_ಅಂಶ_t * ಸ್ಥಿರಾಂಕ px_ಅಂಶ )
{
    /* Make sure the list item is not recorded as being on a list. */
    px_ಅಂಶ->pxContainer = ಖಾಲಿ;

    /* Write known values into the list item if
     * configUSE_LIST_DATA_INTEGRITY_CHECK_BYTES is set to 1. */
    list_ಗಣ_ಮೊದಲ_LIST_ಅಂಶ_INTEGRITY_CHECK_ಮೌಲ್ಯ( px_ಅಂಶ );
    list_ಗಣ_ಎರಡನೇ_LIST_ಅಂಶ_INTEGRITY_CHECK_ಮೌಲ್ಯ( px_ಅಂಶ );
}
/*-----------------------------------------------------------*/

ಶೂನ್ಯ v_List_ಸೇರಿಸು_ಕೊನೆ( List_t * ಸ್ಥಿರಾಂಕ pxList,
                     List_ಅಂಶ_t * ಸ್ಥಿರಾಂಕ px_New_List_ಅಂಶ )
{
    List_ಅಂಶ_t * ಸ್ಥಿರಾಂಕ px_ಸೂಚ್ಯಂಕ = pxList->px_ಸೂಚ್ಯಂಕ;

    /* Only effective when configASSERT() is also defined, these tests may catch
     * the list data structures being overwritten in memory.  They will not catch
     * data errors caused by incorrect configuration or use of FreeRTOS. */
    listTEST_LIST_INTEGRITY( pxList );
    list_TEST_LIST_ಅಂಶ_INTEGRITY( px_New_List_ಅಂಶ );

    /* Insert a new list item into pxList, but rather than sort the list,
     * makes the new list item the last item to be removed by a call to
     * listGET_OWNER_OF_NEXT_ENTRY(). */
    px_New_List_ಅಂಶ->px_ಮುಂದಿನ = px_ಸೂಚ್ಯಂಕ;
    px_New_List_ಅಂಶ->px_ಹಿಂದಿನ = px_ಸೂಚ್ಯಂಕ->px_ಹಿಂದಿನ;

    /* Only used during decision coverage testing. */
    mtCOVERAGE_TEST_DELAY();

    px_ಸೂಚ್ಯಂಕ->px_ಹಿಂದಿನ->px_ಮುಂದಿನ = px_New_List_ಅಂಶ;
    px_ಸೂಚ್ಯಂಕ->px_ಹಿಂದಿನ = px_New_List_ಅಂಶ;

    /* Remember which list the item is in. */
    px_New_List_ಅಂಶ->pxContainer = pxList;

    ( pxList->ux_ಸಂಖ್ಯೆ_ದ_ವಸ್ತುಗಳು )++;
}
/*-----------------------------------------------------------*/

ಶೂನ್ಯ v_List_ಸೇರಿಸು( List_t * ಸ್ಥಿರಾಂಕ pxList,
                  List_ಅಂಶ_t * ಸ್ಥಿರಾಂಕ px_New_List_ಅಂಶ )
{
    List_ಅಂಶ_t * pxIterator;
    ಸ್ಥಿರಾಂಕ TickType_t x_ಮೌಲ್ಯ_ದ_Insertion = px_New_List_ಅಂಶ->x_ಅಂಶ_ಮೌಲ್ಯ;

    /* Only effective when configASSERT() is also defined, these tests may catch
     * the list data structures being overwritten in memory.  They will not catch
     * data errors caused by incorrect configuration or use of FreeRTOS. */
    listTEST_LIST_INTEGRITY( pxList );
    list_TEST_LIST_ಅಂಶ_INTEGRITY( px_New_List_ಅಂಶ );

    /* Insert the new list item into the list, sorted in xItemValue order.
     *
     * If the list already contains a list item with the same item value then the
     * new list item should be placed after it.  This ensures that TCBs which are
     * stored in ready lists (all of which have the same xItemValue value) get a
     * share of the CPU.  However, if the xItemValue is the same as the back marker
     * the iteration loop below will not end.  Therefore the value is checked
     * first, and the algorithm slightly modified if necessary. */
    ಆದರೆ( x_ಮೌಲ್ಯ_ದ_Insertion == ಪೋರ್ಟ್_MAX_DELAY )
    {
        pxIterator = pxList->x_List_ಕೊನೆ.px_ಹಿಂದಿನ;
    }
    ಇಲ್ಲದಿದ್ದರೆ
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

        ಗಾಗಿ( pxIterator = ( List_ಅಂಶ_t * ) &( pxList->x_List_ಕೊನೆ ); pxIterator->px_ಮುಂದಿನ->x_ಅಂಶ_ಮೌಲ್ಯ <= x_ಮೌಲ್ಯ_ದ_Insertion; pxIterator = pxIterator->px_ಮುಂದಿನ ) /*lint !e826 !e740 !e9087 The mini list structure is used as the list end to save RAM.  This is checked and valid. *//*lint !e440 The iterator moves to a different value, not xValueOfInsertion. */
        {
            /* There is nothing to do here, just iterating to the wanted
             * insertion position. */
        }
    }

    px_New_List_ಅಂಶ->px_ಮುಂದಿನ = pxIterator->px_ಮುಂದಿನ;
    px_New_List_ಅಂಶ->px_ಮುಂದಿನ->px_ಹಿಂದಿನ = px_New_List_ಅಂಶ;
    px_New_List_ಅಂಶ->px_ಹಿಂದಿನ = pxIterator;
    pxIterator->px_ಮುಂದಿನ = px_New_List_ಅಂಶ;

    /* Remember which list the item is in.  This allows fast removal of the
     * item later. */
    px_New_List_ಅಂಶ->pxContainer = pxList;

    ( pxList->ux_ಸಂಖ್ಯೆ_ದ_ವಸ್ತುಗಳು )++;
}
/*-----------------------------------------------------------*/

UBaseType_t uxListRemove( List_ಅಂಶ_t * ಸ್ಥಿರಾಂಕ px_ಅಂಶ_ಗಮ್ಯ_Remove )
{
/* The list item knows which list it is in.  Obtain the list from the list
 * item. */
    List_t * ಸ್ಥಿರಾಂಕ pxList = px_ಅಂಶ_ಗಮ್ಯ_Remove->pxContainer;

    px_ಅಂಶ_ಗಮ್ಯ_Remove->px_ಮುಂದಿನ->px_ಹಿಂದಿನ = px_ಅಂಶ_ಗಮ್ಯ_Remove->px_ಹಿಂದಿನ;
    px_ಅಂಶ_ಗಮ್ಯ_Remove->px_ಹಿಂದಿನ->px_ಮುಂದಿನ = px_ಅಂಶ_ಗಮ್ಯ_Remove->px_ಮುಂದಿನ;

    /* Only used during decision coverage testing. */
    mtCOVERAGE_TEST_DELAY();

    /* Make sure the index is left pointing to a valid item. */
    ಆದರೆ( pxList->px_ಸೂಚ್ಯಂಕ == px_ಅಂಶ_ಗಮ್ಯ_Remove )
    {
        pxList->px_ಸೂಚ್ಯಂಕ = px_ಅಂಶ_ಗಮ್ಯ_Remove->px_ಹಿಂದಿನ;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        mtCOVERAGE_TEST_MARKER();
    }

    px_ಅಂಶ_ಗಮ್ಯ_Remove->pxContainer = ಖಾಲಿ;
    ( pxList->ux_ಸಂಖ್ಯೆ_ದ_ವಸ್ತುಗಳು )--;

    ಹಿಂತಿರುಗಿ pxList->ux_ಸಂಖ್ಯೆ_ದ_ವಸ್ತುಗಳು;
}
/*-----------------------------------------------------------*/
