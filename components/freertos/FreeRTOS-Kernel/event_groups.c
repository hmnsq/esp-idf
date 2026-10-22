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

/* Standard includes. */
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* FreeRTOS includes. */
#ਅਨ੍ਤਰ੍ਭਾਵ "FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "timers.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "event_groups.h"
/* Include private IDF API additions for critical thread safety macros */
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_private/freertos_idf_additions_priv.h"

/* Lint e961, e750 and e9021 are suppressed as a MISRA exception justified
 * because the MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ MPU_WRAPPERS_INCLUDED_FROM_API_FILE /*lint !e961 !e750 !e9021 See comment above. */

/* The following bit fields convey control information in a task's event list
 * item value.  It is important they don't clash with the
 * taskEVENT_LIST_ITEM_VALUE_IN_USE definition. */
#ਜੇ config_USE_16_ਦੋਆਧਾਰੀ_ਅੰਕ_TICKS == 1
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ event_CLEAR_EVENTS_ON_EXIT_ਦੋਆਧਾਰੀ_ਅੰਕ    0x0100U
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ event_UNBLOCKED_DUE_ਮੰਜ਼ਿਲ_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ    0x0200U
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ event_WAIT_FOR_ALL_ਬਿੱਟ           0x0400U
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ event_EVENT_ਬਿੱਟ_CONTROL_ਬਾਈਟਾਂ    0xff00U
#ਨਹੀਂਤਾਂ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ event_CLEAR_EVENTS_ON_EXIT_ਦੋਆਧਾਰੀ_ਅੰਕ    0x01000000UL
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ event_UNBLOCKED_DUE_ਮੰਜ਼ਿਲ_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ    0x02000000UL
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ event_WAIT_FOR_ALL_ਬਿੱਟ           0x04000000UL
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ event_EVENT_ਬਿੱਟ_CONTROL_ਬਾਈਟਾਂ    0xff000000UL
#ਜੇ_ਅੰਤ

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ Event_ਸਮੂਹ_Def_t
{
    Event_ਬਿੱਟ_t ux_Event_ਬਿੱਟ;
    List_t x_Tasks_ਉਡੀਕ_For_ਬਿੱਟ; /*< List of tasks waiting for a bit to be set. */

    #ਜੇ ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_Event_ਸਮੂਹ_ਸੰਖਿਆ;
    #ਜੇ_ਅੰਤ

    #ਜੇ ( ( config_SUPPORT_STATIC_ਵੰਡ == 1 ) && ( config_SUPPORT_DYNAMIC_ਵੰਡ == 1 ) )
        uint8_t ucStaticallyAllocated; /*< Set to pdTRUE if the event group is statically allocated to ensure no attempt is made to free the memory. */
    #ਜੇ_ਅੰਤ

    ਪੋਰਟ_MUX_TYPE x_Event_ਸਮੂਹ_ਤਾਲਾ; /* Spinlock required for SMP critical sections */
} Event_ਸਮੂਹ_t;

/*-----------------------------------------------------------*/

/*
 * Test the bits set in uxCurrentEventBits to see if the wait condition is met.
 * The wait condition is defined by xWaitForAllBits.  If xWaitForAllBits is
 * pdTRUE then the wait condition is met if all the bits set in uxBitsToWaitFor
 * are also set in uxCurrentEventBits.  If xWaitForAllBits is pdFALSE then the
 * wait condition is met if any of the bits set in uxBitsToWait for are also set
 * in uxCurrentEventBits.
 */
ਸਥਿਰ ਅਧਾਰ_Type_t prvTestWaitCondition( ਅਚਲ Event_ਬਿੱਟ_t ux_Current_Event_ਬਿੱਟ,
                                        ਅਚਲ Event_ਬਿੱਟ_t ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For,
                                        ਅਚਲ ਅਧਾਰ_Type_t x_Wait_For_All_ਬਿੱਟ ) PRIVILEGED_FUNCTION;

/*-----------------------------------------------------------*/

#ਜੇ ( config_SUPPORT_STATIC_ਵੰਡ == 1 )

    Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ_Create_Static( Static_Event_ਸਮੂਹ_t * px_Event_ਸਮੂਹ_ਬਫਰ )
    {
        Event_ਸਮੂਹ_t * px_Event_ਬਿੱਟ;

        /* A StaticEventGroup_t object must be provided. */
        configASSERT( px_Event_ਸਮੂਹ_ਬਫਰ );

        #ਜੇ ( configASSERT_DEFINED == 1 )
        {
            /* Sanity check that the size of the structure used to declare a
             * variable of type StaticEventGroup_t equals the size of the real
             * event group structure. */
            ਅਸਥਿਰ size_t x_ਮਾਪ = ਆਕਾਰ( Static_Event_ਸਮੂਹ_t );
            configASSERT( x_ਮਾਪ == ਆਕਾਰ( Event_ਸਮੂਹ_t ) );
            ( ਖਾਲੀ ) x_ਮਾਪ; /* Prevent unused variable warning when configASSERT() is not used. */
        } /*lint !e529 xSize is referenced if configASSERT() is defined. */
        #ਜੇ_ਅੰਤ /* configASSERT_DEFINED */

        /* The user has provided a statically allocated event group - use it. */
        px_Event_ਬਿੱਟ = ( Event_ਸਮੂਹ_t * ) px_Event_ਸਮੂਹ_ਬਫਰ; /*lint !e740 !e9087 EventGroup_t and StaticEventGroup_t are deliberately aliased for data hiding purposes and guaranteed to have the same size and alignment requirement - checked by configASSERT(). */

        ਜੇ( px_Event_ਬਿੱਟ != ਨਿਰਾ )
        {
            px_Event_ਬਿੱਟ->ux_Event_ਬਿੱਟ = 0;
            vListInitialise( &( px_Event_ਬਿੱਟ->x_Tasks_ਉਡੀਕ_For_ਬਿੱਟ ) );

            #ਜੇ ( config_SUPPORT_DYNAMIC_ਵੰਡ == 1 )
            {
                /* Both static and dynamic allocation can be used, so note that
                 * this event group was created statically in case the event group
                 * is later deleted. */
                px_Event_ਬਿੱਟ->ucStaticallyAllocated = pdTRUE;
            }
            #ਜੇ_ਅੰਤ /* configSUPPORT_DYNAMIC_ALLOCATION */

            /* Initialize the event group's spinlock. */
            ਪੋਰਟ_MUX_INITIALIZE( &px_Event_ਬਿੱਟ->x_Event_ਸਮੂਹ_ਤਾਲਾ );

            trace_EVENT_ਸਮੂਹ_CREATE( px_Event_ਬਿੱਟ );
        }
        ਨਹੀਂਤਾਂ
        {
            /* xEventGroupCreateStatic should only ever be called with
             * pxEventGroupBuffer pointing to a pre-allocated (compile time
             * allocated) StaticEventGroup_t variable. */
            trace_EVENT_ਸਮੂਹ_CREATE_FAILED();
        }

        ਵਾਪਸ px_Event_ਬਿੱਟ;
    }

#ਜੇ_ਅੰਤ /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

#ਜੇ ( config_SUPPORT_DYNAMIC_ਵੰਡ == 1 )

    Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ_Create( ਖਾਲੀ )
    {
        Event_ਸਮੂਹ_t * px_Event_ਬਿੱਟ;

        /* Allocate the event group.  Justification for MISRA deviation as
         * follows:  pvPortMalloc() always ensures returned memory blocks are
         * aligned per the requirements of the MCU stack.  In this case
         * pvPortMalloc() must return a pointer that is guaranteed to meet the
         * alignment requirements of the EventGroup_t structure - which (if you
         * follow it through) is the alignment requirements of the TickType_t type
         * (EventBits_t being of TickType_t itself).  Therefore, whenever the
         * stack alignment requirements are greater than or equal to the
         * TickType_t alignment requirements the cast is safe.  In other cases,
         * where the natural word size of the architecture is less than
         * sizeof( TickType_t ), the TickType_t variables will be accessed in two
         * or more reads operations, and the alignment requirements is only that
         * of each individual read. */
        px_Event_ਬਿੱਟ = ( Event_ਸਮੂਹ_t * ) pv_ਪੋਰਟ_Malloc( ਆਕਾਰ( Event_ਸਮੂਹ_t ) ); /*lint !e9087 !e9079 see comment above. */

        ਜੇ( px_Event_ਬਿੱਟ != ਨਿਰਾ )
        {
            px_Event_ਬਿੱਟ->ux_Event_ਬਿੱਟ = 0;
            vListInitialise( &( px_Event_ਬਿੱਟ->x_Tasks_ਉਡੀਕ_For_ਬਿੱਟ ) );

            #ਜੇ ( config_SUPPORT_STATIC_ਵੰਡ == 1 )
            {
                /* Both static and dynamic allocation can be used, so note this
                 * event group was allocated statically in case the event group is
                 * later deleted. */
                px_Event_ਬਿੱਟ->ucStaticallyAllocated = pdFALSE;
            }
            #ਜੇ_ਅੰਤ /* configSUPPORT_STATIC_ALLOCATION */

            /* Initialize the event group's spinlock. */
            ਪੋਰਟ_MUX_INITIALIZE( &px_Event_ਬਿੱਟ->x_Event_ਸਮੂਹ_ਤਾਲਾ );

            trace_EVENT_ਸਮੂਹ_CREATE( px_Event_ਬਿੱਟ );
        }
        ਨਹੀਂਤਾਂ
        {
            trace_EVENT_ਸਮੂਹ_CREATE_FAILED(); /*lint !e9063 Else branch only exists to allow tracing and does not generate code if trace macros are not defined. */
        }

        ਵਾਪਸ px_Event_ਬਿੱਟ;
    }

#ਜੇ_ਅੰਤ /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

Event_ਬਿੱਟ_t x_Event_ਸਮੂਹ_Sync( Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ,
                             ਅਚਲ Event_ਬਿੱਟ_t ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ,
                             ਅਚਲ Event_ਬਿੱਟ_t ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For,
                             TickType_t x_Ticks_ਮੰਜ਼ਿਲ_Wait )
{
    Event_ਬਿੱਟ_t ux_Original_ਦੋਆਧਾਰੀ_ਅੰਕ_ਮੁੱਲ, uxReturn;
    Event_ਸਮੂਹ_t * px_Event_ਬਿੱਟ = x_Event_ਸਮੂਹ;
    ਅਧਾਰ_Type_t xAlreadyYielded;
    ਅਧਾਰ_Type_t xTimeoutOccurred = pdFALSE;

    configASSERT( ( ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For & event_EVENT_ਬਿੱਟ_CONTROL_ਬਾਈਟਾਂ ) == 0 );
    configASSERT( ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For != 0 );
    #ਜੇ ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_ਮੰਜ਼ਿਲ_Wait != 0 ) ) );
    }
    #ਜੇ_ਅੰਤ

    prvENTER_CRITICAL_OR_SUSPEND_ALL( &( px_Event_ਬਿੱਟ->x_Event_ਸਮੂਹ_ਤਾਲਾ ) );
    {
        ux_Original_ਦੋਆਧਾਰੀ_ਅੰਕ_ਮੁੱਲ = px_Event_ਬਿੱਟ->ux_Event_ਬਿੱਟ;

        ( ਖਾਲੀ ) x_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ( x_Event_ਸਮੂਹ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ );

        ਜੇ( ( ( ux_Original_ਦੋਆਧਾਰੀ_ਅੰਕ_ਮੁੱਲ | ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ ) & ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For ) == ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For )
        {
            /* All the rendezvous bits are now set - no need to block. */
            uxReturn = ( ux_Original_ਦੋਆਧਾਰੀ_ਅੰਕ_ਮੁੱਲ | ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ );

            /* Rendezvous always clear the bits.  They will have been cleared
             * already unless this is the only task in the rendezvous. */
            px_Event_ਬਿੱਟ->ux_Event_ਬਿੱਟ &= ~ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For;

            x_Ticks_ਮੰਜ਼ਿਲ_Wait = 0;
        }
        ਨਹੀਂਤਾਂ
        {
            ਜੇ( x_Ticks_ਮੰਜ਼ਿਲ_Wait != ( TickType_t ) 0 )
            {
                trace_EVENT_ਸਮੂਹ_SYNC_ਖੰਡ( x_Event_ਸਮੂਹ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For );

                /* Store the bits that the calling task is waiting for in the
                 * task's event list item so the kernel knows when a match is
                 * found.  Then enter the blocked state. */
                v_Task_ਰੱਖੋ_On_Unordered_Event_List( &( px_Event_ਬਿੱਟ->x_Tasks_ਉਡੀਕ_For_ਬਿੱਟ ), ( ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For | event_CLEAR_EVENTS_ON_EXIT_ਦੋਆਧਾਰੀ_ਅੰਕ | event_WAIT_FOR_ALL_ਬਿੱਟ ), x_Ticks_ਮੰਜ਼ਿਲ_Wait );

                /* This assignment is obsolete as uxReturn will get set after
                 * the task unblocks, but some compilers mistakenly generate a
                 * warning about uxReturn being returned without being set if the
                 * assignment is omitted. */
                uxReturn = 0;
            }
            ਨਹੀਂਤਾਂ
            {
                /* The rendezvous bits were not set, but no block time was
                 * specified - just return the current event bit value. */
                uxReturn = px_Event_ਬਿੱਟ->ux_Event_ਬਿੱਟ;
                xTimeoutOccurred = pdTRUE;
            }
        }
    }
    xAlreadyYielded = prvEXIT_CRITICAL_OR_RESUME_ALL( &( px_Event_ਬਿੱਟ->x_Event_ਸਮੂਹ_ਤਾਲਾ ) );

    ਜੇ( x_Ticks_ਮੰਜ਼ਿਲ_Wait != ( TickType_t ) 0 )
    {
        ਜੇ( xAlreadyYielded == pdFALSE )
        {
            ਪੋਰਟ_YIELD_WITHIN_API();
        }
        ਨਹੀਂਤਾਂ
        {
            mtCOVERAGE_TEST_MARKER();
        }

        /* The task blocked to wait for its required bits to be set - at this
         * point either the required bits were set or the block time expired.  If
         * the required bits were set they will have been stored in the task's
         * event list item, and they should now be retrieved then cleared. */
        uxReturn = ux_Task_Reset_Event_ਵਸਤੂ_ਮੁੱਲ();

        ਜੇ( ( uxReturn & event_UNBLOCKED_DUE_ਮੰਜ਼ਿਲ_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ ) == ( Event_ਬਿੱਟ_t ) 0 )
        {
            /* The task timed out, just return the current event bit value. */
            taskENTER_CRITICAL( &( px_Event_ਬਿੱਟ->x_Event_ਸਮੂਹ_ਤਾਲਾ ) );
            {
                uxReturn = px_Event_ਬਿੱਟ->ux_Event_ਬਿੱਟ;

                /* Although the task got here because it timed out before the
                 * bits it was waiting for were set, it is possible that since it
                 * unblocked another task has set the bits.  If this is the case
                 * then it needs to clear the bits before exiting. */
                ਜੇ( ( uxReturn & ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For ) == ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For )
                {
                    px_Event_ਬਿੱਟ->ux_Event_ਬਿੱਟ &= ~ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For;
                }
                ਨਹੀਂਤਾਂ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            taskEXIT_CRITICAL( &( px_Event_ਬਿੱਟ->x_Event_ਸਮੂਹ_ਤਾਲਾ ) );

            xTimeoutOccurred = pdTRUE;
        }
        ਨਹੀਂਤਾਂ
        {
            /* The task unblocked because the bits were set. */
        }

        /* Control bits might be set as the task had blocked should not be
         * returned. */
        uxReturn &= ~event_EVENT_ਬਿੱਟ_CONTROL_ਬਾਈਟਾਂ;
    }

    trace_EVENT_ਸਮੂਹ_SYNC_ਸਿਰਾ( x_Event_ਸਮੂਹ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For, xTimeoutOccurred );

    /* Prevent compiler warnings when trace macros are not used. */
    ( ਖਾਲੀ ) xTimeoutOccurred;

    ਵਾਪਸ uxReturn;
}
/*-----------------------------------------------------------*/

Event_ਬਿੱਟ_t x_Event_ਸਮੂਹ_Wait_ਬਿੱਟ( Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ,
                                 ਅਚਲ Event_ਬਿੱਟ_t ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For,
                                 ਅਚਲ ਅਧਾਰ_Type_t xClearOnExit,
                                 ਅਚਲ ਅਧਾਰ_Type_t x_Wait_For_All_ਬਿੱਟ,
                                 TickType_t x_Ticks_ਮੰਜ਼ਿਲ_Wait )
{
    Event_ਸਮੂਹ_t * px_Event_ਬਿੱਟ = x_Event_ਸਮੂਹ;
    Event_ਬਿੱਟ_t uxReturn, ux_Control_ਬਿੱਟ = 0;
    ਅਧਾਰ_Type_t xWaitConditionMet, xAlreadyYielded;
    ਅਧਾਰ_Type_t xTimeoutOccurred = pdFALSE;

    /* Check the user is not attempting to wait on the bits used by the kernel
     * itself, and that at least one bit is being requested. */
    configASSERT( x_Event_ਸਮੂਹ );
    configASSERT( ( ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For & event_EVENT_ਬਿੱਟ_CONTROL_ਬਾਈਟਾਂ ) == 0 );
    configASSERT( ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For != 0 );
    #ਜੇ ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_ਮੰਜ਼ਿਲ_Wait != 0 ) ) );
    }
    #ਜੇ_ਅੰਤ

    prvENTER_CRITICAL_OR_SUSPEND_ALL( &( px_Event_ਬਿੱਟ->x_Event_ਸਮੂਹ_ਤਾਲਾ ) );
    {
        ਅਚਲ Event_ਬਿੱਟ_t ux_Current_Event_ਬਿੱਟ = px_Event_ਬਿੱਟ->ux_Event_ਬਿੱਟ;

        /* Check to see if the wait condition is already met or not. */
        xWaitConditionMet = prvTestWaitCondition( ux_Current_Event_ਬਿੱਟ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For, x_Wait_For_All_ਬਿੱਟ );

        ਜੇ( xWaitConditionMet != pdFALSE )
        {
            /* The wait condition has already been met so there is no need to
             * block. */
            uxReturn = ux_Current_Event_ਬਿੱਟ;
            x_Ticks_ਮੰਜ਼ਿਲ_Wait = ( TickType_t ) 0;

            /* Clear the wait bits if requested to do so. */
            ਜੇ( xClearOnExit != pdFALSE )
            {
                px_Event_ਬਿੱਟ->ux_Event_ਬਿੱਟ &= ~ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For;
            }
            ਨਹੀਂਤਾਂ
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        ਨਹੀਂਤਾਂ ਜੇ( x_Ticks_ਮੰਜ਼ਿਲ_Wait == ( TickType_t ) 0 )
        {
            /* The wait condition has not been met, but no block time was
             * specified, so just return the current value. */
            uxReturn = ux_Current_Event_ਬਿੱਟ;
            xTimeoutOccurred = pdTRUE;
        }
        ਨਹੀਂਤਾਂ
        {
            /* The task is going to block to wait for its required bits to be
             * set.  uxControlBits are used to remember the specified behaviour of
             * this call to xEventGroupWaitBits() - for use when the event bits
             * unblock the task. */
            ਜੇ( xClearOnExit != pdFALSE )
            {
                ux_Control_ਬਿੱਟ |= event_CLEAR_EVENTS_ON_EXIT_ਦੋਆਧਾਰੀ_ਅੰਕ;
            }
            ਨਹੀਂਤਾਂ
            {
                mtCOVERAGE_TEST_MARKER();
            }

            ਜੇ( x_Wait_For_All_ਬਿੱਟ != pdFALSE )
            {
                ux_Control_ਬਿੱਟ |= event_WAIT_FOR_ALL_ਬਿੱਟ;
            }
            ਨਹੀਂਤਾਂ
            {
                mtCOVERAGE_TEST_MARKER();
            }

            /* Store the bits that the calling task is waiting for in the
             * task's event list item so the kernel knows when a match is
             * found.  Then enter the blocked state. */
            v_Task_ਰੱਖੋ_On_Unordered_Event_List( &( px_Event_ਬਿੱਟ->x_Tasks_ਉਡੀਕ_For_ਬਿੱਟ ), ( ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For | ux_Control_ਬਿੱਟ ), x_Ticks_ਮੰਜ਼ਿਲ_Wait );

            /* This is obsolete as it will get set after the task unblocks, but
             * some compilers mistakenly generate a warning about the variable
             * being returned without being set if it is not done. */
            uxReturn = 0;

            trace_EVENT_ਸਮੂਹ_WAIT_ਬਿੱਟ_ਖੰਡ( x_Event_ਸਮੂਹ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For );
        }
    }
    xAlreadyYielded = prvEXIT_CRITICAL_OR_RESUME_ALL( &( px_Event_ਬਿੱਟ->x_Event_ਸਮੂਹ_ਤਾਲਾ ) );

    ਜੇ( x_Ticks_ਮੰਜ਼ਿਲ_Wait != ( TickType_t ) 0 )
    {
        ਜੇ( xAlreadyYielded == pdFALSE )
        {
            ਪੋਰਟ_YIELD_WITHIN_API();
        }
        ਨਹੀਂਤਾਂ
        {
            mtCOVERAGE_TEST_MARKER();
        }

        /* The task blocked to wait for its required bits to be set - at this
         * point either the required bits were set or the block time expired.  If
         * the required bits were set they will have been stored in the task's
         * event list item, and they should now be retrieved then cleared. */
        uxReturn = ux_Task_Reset_Event_ਵਸਤੂ_ਮੁੱਲ();

        ਜੇ( ( uxReturn & event_UNBLOCKED_DUE_ਮੰਜ਼ਿਲ_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ ) == ( Event_ਬਿੱਟ_t ) 0 )
        {
            taskENTER_CRITICAL( &( px_Event_ਬਿੱਟ->x_Event_ਸਮੂਹ_ਤਾਲਾ ) );
            {
                /* The task timed out, just return the current event bit value. */
                uxReturn = px_Event_ਬਿੱਟ->ux_Event_ਬਿੱਟ;

                /* It is possible that the event bits were updated between this
                 * task leaving the Blocked state and running again. */
                ਜੇ( prvTestWaitCondition( uxReturn, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For, x_Wait_For_All_ਬਿੱਟ ) != pdFALSE )
                {
                    ਜੇ( xClearOnExit != pdFALSE )
                    {
                        px_Event_ਬਿੱਟ->ux_Event_ਬਿੱਟ &= ~ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For;
                    }
                    ਨਹੀਂਤਾਂ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                ਨਹੀਂਤਾਂ
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                xTimeoutOccurred = pdTRUE;
            }
            taskEXIT_CRITICAL( &( px_Event_ਬਿੱਟ->x_Event_ਸਮੂਹ_ਤਾਲਾ ) );
        }
        ਨਹੀਂਤਾਂ
        {
            /* The task unblocked because the bits were set. */
        }

        /* The task blocked so control bits may have been set. */
        uxReturn &= ~event_EVENT_ਬਿੱਟ_CONTROL_ਬਾਈਟਾਂ;
    }

    trace_EVENT_ਸਮੂਹ_WAIT_ਬਿੱਟ_ਸਿਰਾ( x_Event_ਸਮੂਹ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For, xTimeoutOccurred );

    /* Prevent compiler warnings when trace macros are not used. */
    ( ਖਾਲੀ ) xTimeoutOccurred;

    ਵਾਪਸ uxReturn;
}
/*-----------------------------------------------------------*/

Event_ਬਿੱਟ_t x_Event_ਸਮੂਹ_Clear_ਬਿੱਟ( Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ,
                                  ਅਚਲ Event_ਬਿੱਟ_t ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear )
{
    Event_ਸਮੂਹ_t * px_Event_ਬਿੱਟ = x_Event_ਸਮੂਹ;
    Event_ਬਿੱਟ_t uxReturn;

    /* Check the user is not attempting to clear the bits used by the kernel
     * itself. */
    configASSERT( x_Event_ਸਮੂਹ );
    configASSERT( ( ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear & event_EVENT_ਬਿੱਟ_CONTROL_ਬਾਈਟਾਂ ) == 0 );

    taskENTER_CRITICAL( &( px_Event_ਬਿੱਟ->x_Event_ਸਮੂਹ_ਤਾਲਾ ) );
    {
        trace_EVENT_ਸਮੂਹ_CLEAR_ਬਿੱਟ( x_Event_ਸਮੂਹ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear );

        /* The value returned is the event group value prior to the bits being
         * cleared. */
        uxReturn = px_Event_ਬਿੱਟ->ux_Event_ਬਿੱਟ;

        /* Clear the bits. */
        px_Event_ਬਿੱਟ->ux_Event_ਬਿੱਟ &= ~ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear;
    }
    taskEXIT_CRITICAL( &( px_Event_ਬਿੱਟ->x_Event_ਸਮੂਹ_ਤਾਲਾ ) );

    ਵਾਪਸ uxReturn;
}
/*-----------------------------------------------------------*/

#ਜੇ ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) )

    ਅਧਾਰ_Type_t x_Event_ਸਮੂਹ_Clear_ਬਿੱਟ_From_ISR( Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ,
                                            ਅਚਲ Event_ਬਿੱਟ_t ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear )
    {
        ਅਧਾਰ_Type_t xReturn;

        trace_EVENT_ਸਮੂਹ_CLEAR_ਬਿੱਟ_FROM_ISR( x_Event_ਸਮੂਹ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear );
        xReturn = xTimerPendFunctionCallFromISR( v_Event_ਸਮੂਹ_Clear_ਬਿੱਟ_Callback, ( ਖਾਲੀ * ) x_Event_ਸਮੂਹ, ( uint32_t ) ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear, ਨਿਰਾ ); /*lint !e9087 Can't avoid cast to void* as a generic callback function not specific to this use case. Callback casts back to original type so safe. */

        ਵਾਪਸ xReturn;
    }

#ਜੇ_ਅੰਤ /* if ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) ) */
/*-----------------------------------------------------------*/

Event_ਬਿੱਟ_t x_Event_ਸਮੂਹ_Get_ਬਿੱਟ_From_ISR( Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ )
{
    UBaseType_t ux_Saved_ਰੁਕਾਵਟ_ਸਥਿਤੀ;
    Event_ਸਮੂਹ_t ਅਚਲ * ਅਚਲ px_Event_ਬਿੱਟ = x_Event_ਸਮੂਹ;
    Event_ਬਿੱਟ_t uxReturn;

    prv_ENTER_CRITICAL_OR_ਨਕਾਬ_ISR( ( ਪੋਰਟ_MUX_TYPE * ) &( px_Event_ਬਿੱਟ->x_Event_ਸਮੂਹ_ਤਾਲਾ ), ux_Saved_ਰੁਕਾਵਟ_ਸਥਿਤੀ );
    {
        uxReturn = px_Event_ਬਿੱਟ->ux_Event_ਬਿੱਟ;
    }
    prvEXIT_CRITICAL_OR_UNMASK_ISR( ( ਪੋਰਟ_MUX_TYPE * ) &( px_Event_ਬਿੱਟ->x_Event_ਸਮੂਹ_ਤਾਲਾ ), ux_Saved_ਰੁਕਾਵਟ_ਸਥਿਤੀ );

    ਵਾਪਸ uxReturn;
} /*lint !e818 EventGroupHandle_t is a typedef used in other functions to so can't be pointer to const. */
/*-----------------------------------------------------------*/

Event_ਬਿੱਟ_t x_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ( Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ,
                                ਅਚਲ Event_ਬਿੱਟ_t ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ )
{
    List_ਵਸਤੂ_t * px_List_ਵਸਤੂ;
    List_ਵਸਤੂ_t * px_ਅਗਲਾ;
    List_ਵਸਤੂ_t ਅਚਲ * px_List_ਸਿਰਾ;
    List_t ਅਚਲ * pxList;
    Event_ਬਿੱਟ_t ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear = 0, ux_ਬਿੱਟ_Waited_For, ux_Control_ਬਿੱਟ;
    Event_ਸਮੂਹ_t * px_Event_ਬਿੱਟ = x_Event_ਸਮੂਹ;
    ਅਧਾਰ_Type_t x_ਮੇਲ_ਮਿਲਿਆ = pdFALSE;

    /* Check the user is not attempting to set the bits used by the kernel
     * itself. */
    configASSERT( x_Event_ਸਮੂਹ );
    configASSERT( ( ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ & event_EVENT_ਬਿੱਟ_CONTROL_ਬਾਈਟਾਂ ) == 0 );

    pxList = &( px_Event_ਬਿੱਟ->x_Tasks_ਉਡੀਕ_For_ਬਿੱਟ );
    px_List_ਸਿਰਾ = list_GET_ਸਿਰਾ_MARKER( pxList ); /*lint !e826 !e740 !e9087 The mini list structure is used as the list end to save RAM.  This is checked and valid. */

    prvENTER_CRITICAL_OR_SUSPEND_ALL( &( px_Event_ਬਿੱਟ->x_Event_ਸਮੂਹ_ਤਾਲਾ ) );
    #ਜੇ ( config_ਸੰਖਿਆ_ਦਾ_CORES > 1 )

        /* We are about to traverse a task list which is a kernel data structure.
         * Thus we need to call prvTakeKernelLock() to take the kernel lock. */
        prv_Take_Kernel_ਤਾਲਾ();
    #ਜੇ_ਅੰਤ /* configNUMBER_OF_CORES > 1 */
    {
        trace_EVENT_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ( x_Event_ਸਮੂਹ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ );

        px_List_ਵਸਤੂ = list_GET_ਸਿਰ_ਇੰਦਰਾਜ( pxList );

        /* Set the bits. */
        px_Event_ਬਿੱਟ->ux_Event_ਬਿੱਟ |= ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ;

        /* See if the new bit value should unblock any tasks. */
        ਜਦੋਂ( px_List_ਵਸਤੂ != px_List_ਸਿਰਾ )
        {
            px_ਅਗਲਾ = list_GET_ਅਗਲਾ( px_List_ਵਸਤੂ );
            ux_ਬਿੱਟ_Waited_For = list_GET_LIST_ਵਸਤੂ_ਮੁੱਲ( px_List_ਵਸਤੂ );
            x_ਮੇਲ_ਮਿਲਿਆ = pdFALSE;

            /* Split the bits waited for from the control bits. */
            ux_Control_ਬਿੱਟ = ux_ਬਿੱਟ_Waited_For & event_EVENT_ਬਿੱਟ_CONTROL_ਬਾਈਟਾਂ;
            ux_ਬਿੱਟ_Waited_For &= ~event_EVENT_ਬਿੱਟ_CONTROL_ਬਾਈਟਾਂ;

            ਜੇ( ( ux_Control_ਬਿੱਟ & event_WAIT_FOR_ALL_ਬਿੱਟ ) == ( Event_ਬਿੱਟ_t ) 0 )
            {
                /* Just looking for single bit being set. */
                ਜੇ( ( ux_ਬਿੱਟ_Waited_For & px_Event_ਬਿੱਟ->ux_Event_ਬਿੱਟ ) != ( Event_ਬਿੱਟ_t ) 0 )
                {
                    x_ਮੇਲ_ਮਿਲਿਆ = pdTRUE;
                }
                ਨਹੀਂਤਾਂ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            ਨਹੀਂਤਾਂ ਜੇ( ( ux_ਬਿੱਟ_Waited_For & px_Event_ਬਿੱਟ->ux_Event_ਬਿੱਟ ) == ux_ਬਿੱਟ_Waited_For )
            {
                /* All bits are set. */
                x_ਮੇਲ_ਮਿਲਿਆ = pdTRUE;
            }
            ਨਹੀਂਤਾਂ
            {
                /* Need all bits to be set, but not all the bits were set. */
            }

            ਜੇ( x_ਮੇਲ_ਮਿਲਿਆ != pdFALSE )
            {
                /* The bits match.  Should the bits be cleared on exit? */
                ਜੇ( ( ux_Control_ਬਿੱਟ & event_CLEAR_EVENTS_ON_EXIT_ਦੋਆਧਾਰੀ_ਅੰਕ ) != ( Event_ਬਿੱਟ_t ) 0 )
                {
                    ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear |= ux_ਬਿੱਟ_Waited_For;
                }
                ਨਹੀਂਤਾਂ
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                /* Store the actual event flag value in the task's event list
                 * item before removing the task from the event list.  The
                 * eventUNBLOCKED_DUE_TO_BIT_SET bit is set so the task knows
                 * that is was unblocked due to its required bits matching, rather
                 * than because it timed out. */
                vTaskRemoveFromUnorderedEventList( px_List_ਵਸਤੂ, px_Event_ਬਿੱਟ->ux_Event_ਬਿੱਟ | event_UNBLOCKED_DUE_ਮੰਜ਼ਿਲ_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ );
            }

            /* Move onto the next list item.  Note pxListItem->pxNext is not
             * used here as the list item may have been removed from the event list
             * and inserted into the ready/pending reading list. */
            px_List_ਵਸਤੂ = px_ਅਗਲਾ;
        }

        /* Clear any bits that matched when the eventCLEAR_EVENTS_ON_EXIT_BIT
         * bit was set in the control word. */
        px_Event_ਬਿੱਟ->ux_Event_ਬਿੱਟ &= ~ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear;
    }
    #ਜੇ ( config_ਸੰਖਿਆ_ਦਾ_CORES > 1 )
        /* Release the previously taken kernel lock. */
        prv_Release_Kernel_ਤਾਲਾ();
    #ਜੇ_ਅੰਤ /* configNUMBER_OF_CORES > 1 */
    ( ਖਾਲੀ ) prvEXIT_CRITICAL_OR_RESUME_ALL( &( px_Event_ਬਿੱਟ->x_Event_ਸਮੂਹ_ਤਾਲਾ ) );


    ਵਾਪਸ px_Event_ਬਿੱਟ->ux_Event_ਬਿੱਟ;
}
/*-----------------------------------------------------------*/

ਖਾਲੀ v_Event_ਸਮੂਹ_Delete( Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ )
{
    Event_ਸਮੂਹ_t * px_Event_ਬਿੱਟ = x_Event_ਸਮੂਹ;
    ਅਚਲ List_t * px_Tasks_ਉਡੀਕ_For_ਬਿੱਟ;

    configASSERT( px_Event_ਬਿੱਟ );

    px_Tasks_ਉਡੀਕ_For_ਬਿੱਟ = &( px_Event_ਬਿੱਟ->x_Tasks_ਉਡੀਕ_For_ਬਿੱਟ );

    prvENTER_CRITICAL_OR_SUSPEND_ALL( &( px_Event_ਬਿੱਟ->x_Event_ਸਮੂਹ_ਤਾਲਾ ) );
    #ਜੇ ( config_ਸੰਖਿਆ_ਦਾ_CORES > 1 )

        /* We are about to traverse a task list which is a kernel data structure.
         * Thus we need to call prvTakeKernelLock() to take the kernel lock. */
        prv_Take_Kernel_ਤਾਲਾ();
    #ਜੇ_ਅੰਤ /* configNUMBER_OF_CORES > 1 */
    {
        trace_EVENT_ਸਮੂਹ_DELETE( x_Event_ਸਮੂਹ );

        ਜਦੋਂ( list_CURRENT_LIST_ਲੰਮਾਈ( px_Tasks_ਉਡੀਕ_For_ਬਿੱਟ ) > ( UBaseType_t ) 0 )
        {
            /* Unblock the task, returning 0 as the event list is being deleted
             * and cannot therefore have any bits set. */
            configASSERT( px_Tasks_ਉਡੀਕ_For_ਬਿੱਟ->x_List_ਸਿਰਾ.px_ਅਗਲਾ != ( ਅਚਲ List_ਵਸਤੂ_t * ) &( px_Tasks_ਉਡੀਕ_For_ਬਿੱਟ->x_List_ਸਿਰਾ ) );
            vTaskRemoveFromUnorderedEventList( px_Tasks_ਉਡੀਕ_For_ਬਿੱਟ->x_List_ਸਿਰਾ.px_ਅਗਲਾ, event_UNBLOCKED_DUE_ਮੰਜ਼ਿਲ_ਦੋਆਧਾਰੀ_ਅੰਕ_ਸੈੱਟ );
        }
    }
    #ਜੇ ( config_ਸੰਖਿਆ_ਦਾ_CORES > 1 )
        /* Release the previously taken kernel lock. */
        prv_Release_Kernel_ਤਾਲਾ();
    #ਜੇ_ਅੰਤ /* configNUMBER_OF_CORES > 1 */
    prvEXIT_CRITICAL_OR_RESUME_ALL( &( px_Event_ਬਿੱਟ->x_Event_ਸਮੂਹ_ਤਾਲਾ ) );

    #ਜੇ ( ( config_SUPPORT_DYNAMIC_ਵੰਡ == 1 ) && ( config_SUPPORT_STATIC_ਵੰਡ == 0 ) )
    {
        /* The event group can only have been allocated dynamically - free
         * it again. */
        v_ਪੋਰਟ_Free( px_Event_ਬਿੱਟ );
    }
    #ਜਾਂਜੇ ( ( config_SUPPORT_DYNAMIC_ਵੰਡ == 1 ) && ( config_SUPPORT_STATIC_ਵੰਡ == 1 ) )
    {
        /* The event group could have been allocated statically or
         * dynamically, so check before attempting to free the memory. */
        ਜੇ( px_Event_ਬਿੱਟ->ucStaticallyAllocated == ( uint8_t ) pdFALSE )
        {
            v_ਪੋਰਟ_Free( px_Event_ਬਿੱਟ );
        }
        ਨਹੀਂਤਾਂ
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    #ਜੇ_ਅੰਤ /* configSUPPORT_DYNAMIC_ALLOCATION */
}
/*-----------------------------------------------------------*/

#ਜੇ ( config_SUPPORT_STATIC_ਵੰਡ == 1 )
    ਅਧਾਰ_Type_t x_Event_ਸਮੂਹ_Get_Static_ਬਫਰ( Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ,
                                           Static_Event_ਸਮੂਹ_t ** ppx_Event_ਸਮੂਹ_ਬਫਰ )
    {
        ਅਧਾਰ_Type_t xReturn;
        Event_ਸਮੂਹ_t * px_Event_ਬਿੱਟ = x_Event_ਸਮੂਹ;

        configASSERT( px_Event_ਬਿੱਟ );
        configASSERT( ppx_Event_ਸਮੂਹ_ਬਫਰ );

        #ਜੇ ( config_SUPPORT_DYNAMIC_ਵੰਡ == 1 )
        {
            /* Check if the event group was statically allocated. */
            ਜੇ( px_Event_ਬਿੱਟ->ucStaticallyAllocated == ( uint8_t ) pdTRUE )
            {
                *ppx_Event_ਸਮੂਹ_ਬਫਰ = ( Static_Event_ਸਮੂਹ_t * ) px_Event_ਬਿੱਟ;
                xReturn = pdTRUE;
            }
            ਨਹੀਂਤਾਂ
            {
                xReturn = pdFALSE;
            }
        }
        #ਨਹੀਂਤਾਂ /* configSUPPORT_DYNAMIC_ALLOCATION */
        {
            /* Event group must have been statically allocated. */
            *ppx_Event_ਸਮੂਹ_ਬਫਰ = ( Static_Event_ਸਮੂਹ_t * ) px_Event_ਬਿੱਟ;
            xReturn = pdTRUE;
        }
        #ਜੇ_ਅੰਤ /* configSUPPORT_DYNAMIC_ALLOCATION */

        ਵਾਪਸ xReturn;
    }
#ਜੇ_ਅੰਤ /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

/* For internal use only - execute a 'set bits' command that was pended from
 * an interrupt. */
ਖਾਲੀ v_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ_Callback( ਖਾਲੀ * pv_Event_ਸਮੂਹ,
                                 ਅਚਲ uint32_t ul_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ )
{
    ( ਖਾਲੀ ) x_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ( pv_Event_ਸਮੂਹ, ( Event_ਬਿੱਟ_t ) ul_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ ); /*lint !e9079 Can't avoid cast to void* as a generic timer callback prototype. Callback casts back to original type so safe. */
}
/*-----------------------------------------------------------*/

/* For internal use only - execute a 'clear bits' command that was pended from
 * an interrupt. */
ਖਾਲੀ v_Event_ਸਮੂਹ_Clear_ਬਿੱਟ_Callback( ਖਾਲੀ * pv_Event_ਸਮੂਹ,
                                   ਅਚਲ uint32_t ul_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear )
{
    ( ਖਾਲੀ ) x_Event_ਸਮੂਹ_Clear_ਬਿੱਟ( pv_Event_ਸਮੂਹ, ( Event_ਬਿੱਟ_t ) ul_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear ); /*lint !e9079 Can't avoid cast to void* as a generic timer callback prototype. Callback casts back to original type so safe. */
}
/*-----------------------------------------------------------*/

ਸਥਿਰ ਅਧਾਰ_Type_t prvTestWaitCondition( ਅਚਲ Event_ਬਿੱਟ_t ux_Current_Event_ਬਿੱਟ,
                                        ਅਚਲ Event_ਬਿੱਟ_t ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For,
                                        ਅਚਲ ਅਧਾਰ_Type_t x_Wait_For_All_ਬਿੱਟ )
{
    ਅਧਾਰ_Type_t xWaitConditionMet = pdFALSE;

    ਜੇ( x_Wait_For_All_ਬਿੱਟ == pdFALSE )
    {
        /* Task only has to wait for one bit within uxBitsToWaitFor to be
         * set.  Is one already set? */
        ਜੇ( ( ux_Current_Event_ਬਿੱਟ & ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For ) != ( Event_ਬਿੱਟ_t ) 0 )
        {
            xWaitConditionMet = pdTRUE;
        }
        ਨਹੀਂਤਾਂ
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    ਨਹੀਂਤਾਂ
    {
        /* Task has to wait for all the bits in uxBitsToWaitFor to be set.
         * Are they set already? */
        ਜੇ( ( ux_Current_Event_ਬਿੱਟ & ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For ) == ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For )
        {
            xWaitConditionMet = pdTRUE;
        }
        ਨਹੀਂਤਾਂ
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }

    ਵਾਪਸ xWaitConditionMet;
}
/*-----------------------------------------------------------*/

#ਜੇ ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) )

    ਅਧਾਰ_Type_t x_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ_From_ISR( Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ,
                                          ਅਚਲ Event_ਬਿੱਟ_t ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ,
                                          ਅਧਾਰ_Type_t * px_Higher_ਤਰਜੀਹ_Task_Woken )
    {
        ਅਧਾਰ_Type_t xReturn;

        trace_EVENT_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ_FROM_ISR( x_Event_ਸਮੂਹ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ );
        xReturn = xTimerPendFunctionCallFromISR( v_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ_Callback, ( ਖਾਲੀ * ) x_Event_ਸਮੂਹ, ( uint32_t ) ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ, px_Higher_ਤਰਜੀਹ_Task_Woken ); /*lint !e9087 Can't avoid cast to void* as a generic callback function not specific to this use case. Callback casts back to original type so safe. */

        ਵਾਪਸ xReturn;
    }

#ਜੇ_ਅੰਤ /* if ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) ) */
/*-----------------------------------------------------------*/

#ਜੇ ( configUSE_TRACE_FACILITY == 1 )

    UBaseType_t ux_Event_ਸਮੂਹ_Get_ਸੰਖਿਆ( ਖਾਲੀ * x_Event_ਸਮੂਹ )
    {
        UBaseType_t xReturn;
        Event_ਸਮੂਹ_t ਅਚਲ * px_Event_ਬਿੱਟ = ( Event_ਸਮੂਹ_t * ) x_Event_ਸਮੂਹ; /*lint !e9087 !e9079 EventGroupHandle_t is a pointer to an EventGroup_t, but EventGroupHandle_t is kept opaque outside of this file for data hiding purposes. */

        ਜੇ( x_Event_ਸਮੂਹ == ਨਿਰਾ )
        {
            xReturn = 0;
        }
        ਨਹੀਂਤਾਂ
        {
            xReturn = px_Event_ਬਿੱਟ->ux_Event_ਸਮੂਹ_ਸੰਖਿਆ;
        }

        ਵਾਪਸ xReturn;
    }

#ਜੇ_ਅੰਤ /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#ਜੇ ( configUSE_TRACE_FACILITY == 1 )

    ਖਾਲੀ v_Event_ਸਮੂਹ_ਸੈੱਟ_ਸੰਖਿਆ( ਖਾਲੀ * x_Event_ਸਮੂਹ,
                               UBaseType_t ux_Event_ਸਮੂਹ_ਸੰਖਿਆ )
    {
        ( ( Event_ਸਮੂਹ_t * ) x_Event_ਸਮੂਹ )->ux_Event_ਸਮੂਹ_ਸੰਖਿਆ = ux_Event_ਸਮੂਹ_ਸੰਖਿਆ; /*lint !e9087 !e9079 EventGroupHandle_t is a pointer to an EventGroup_t, but EventGroupHandle_t is kept opaque outside of this file for data hiding purposes. */
    }

#ਜੇ_ਅੰਤ /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/
