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
#ضمن <stdlib.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#تعریف_کړه MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* FreeRTOS includes. */
#ضمن "FreeRTOS.h"
#ضمن "task.h"
#ضمن "timers.h"
#ضمن "event_groups.h"
/* Include private IDF API additions for critical thread safety macros */
#ضمن "esp_private/freertos_idf_additions_priv.h"

/* Lint e961, e750 and e9021 are suppressed as a MISRA exception justified
 * because the MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#تعریف_لغوه MPU_WRAPPERS_INCLUDED_FROM_API_FILE /*lint !e961 !e750 !e9021 See comment above. */

/* The following bit fields convey control information in a task's event list
 * item value.  It is important they don't clash with the
 * taskEVENT_LIST_ITEM_VALUE_IN_USE definition. */
#إذا config_USE_16_بټ_TICKS == 1
    #تعریف_کړه event_CLEAR_EVENTS_ON_EXIT_بټ    0x0100U
    #تعریف_کړه event_UNBLOCKED_DUE_منزل_بټ_سېټ    0x0200U
    #تعریف_کړه event_WAIT_FOR_ALL_بټونه           0x0400U
    #تعریف_کړه event_EVENT_بټونه_CONTROL_بایټونه    0xff00U
#وإلا
    #تعریف_کړه event_CLEAR_EVENTS_ON_EXIT_بټ    0x01000000UL
    #تعریف_کړه event_UNBLOCKED_DUE_منزل_بټ_سېټ    0x02000000UL
    #تعریف_کړه event_WAIT_FOR_ALL_بټونه           0x04000000UL
    #تعریف_کړه event_EVENT_بټونه_CONTROL_بایټونه    0xff000000UL
#د_که_پای

تعريفنوع بنية Event_ډله_Def_t
{
    Event_بټونه_t ux_Event_بټونه;
    List_t x_Tasks_انتظار_For_بټونه; /*< List of tasks waiting for a bit to be set. */

    #إذا ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_Event_ډله_شمېره;
    #د_که_پای

    #إذا ( ( config_SUPPORT_STATIC_ویش == 1 ) && ( config_SUPPORT_DYNAMIC_ویش == 1 ) )
        uint8_t ucStaticallyAllocated; /*< Set to pdTRUE if the event group is statically allocated to ensure no attempt is made to free the memory. */
    #د_که_پای

    پورټ_MUX_TYPE x_Event_ډله_قلف; /* Spinlock required for SMP critical sections */
} Event_ډله_t;

/*-----------------------------------------------------------*/

/*
 * Test the bits set in uxCurrentEventBits to see if the wait condition is met.
 * The wait condition is defined by xWaitForAllBits.  If xWaitForAllBits is
 * pdTRUE then the wait condition is met if all the bits set in uxBitsToWaitFor
 * are also set in uxCurrentEventBits.  If xWaitForAllBits is pdFALSE then the
 * wait condition is met if any of the bits set in uxBitsToWait for are also set
 * in uxCurrentEventBits.
 */
ساكن بنسټ_Type_t prvTestWaitCondition( ثابت Event_بټونه_t ux_Current_Event_بټونه,
                                        ثابت Event_بټونه_t ux_بټونه_منزل_Wait_For,
                                        ثابت بنسټ_Type_t x_Wait_For_All_بټونه ) PRIVILEGED_FUNCTION;

/*-----------------------------------------------------------*/

#إذا ( config_SUPPORT_STATIC_ویش == 1 )

    Event_ډله_Handle_t x_Event_ډله_Create_Static( Static_Event_ډله_t * px_Event_ډله_بفر )
    {
        Event_ډله_t * px_Event_بټونه;

        /* A StaticEventGroup_t object must be provided. */
        configASSERT( px_Event_ډله_بفر );

        #إذا ( configASSERT_DEFINED == 1 )
        {
            /* Sanity check that the size of the structure used to declare a
             * variable of type StaticEventGroup_t equals the size of the real
             * event group structure. */
            متطاير size_t x_اندازه = حجم( Static_Event_ډله_t );
            configASSERT( x_اندازه == حجم( Event_ډله_t ) );
            ( فراغ ) x_اندازه; /* Prevent unused variable warning when configASSERT() is not used. */
        } /*lint !e529 xSize is referenced if configASSERT() is defined. */
        #د_که_پای /* configASSERT_DEFINED */

        /* The user has provided a statically allocated event group - use it. */
        px_Event_بټونه = ( Event_ډله_t * ) px_Event_ډله_بفر; /*lint !e740 !e9087 EventGroup_t and StaticEventGroup_t are deliberately aliased for data hiding purposes and guaranteed to have the same size and alignment requirement - checked by configASSERT(). */

        إذا( px_Event_بټونه != عدم )
        {
            px_Event_بټونه->ux_Event_بټونه = 0;
            vListInitialise( &( px_Event_بټونه->x_Tasks_انتظار_For_بټونه ) );

            #إذا ( config_SUPPORT_DYNAMIC_ویش == 1 )
            {
                /* Both static and dynamic allocation can be used, so note that
                 * this event group was created statically in case the event group
                 * is later deleted. */
                px_Event_بټونه->ucStaticallyAllocated = pdTRUE;
            }
            #د_که_پای /* configSUPPORT_DYNAMIC_ALLOCATION */

            /* Initialize the event group's spinlock. */
            پورټ_MUX_INITIALIZE( &px_Event_بټونه->x_Event_ډله_قلف );

            trace_EVENT_ډله_CREATE( px_Event_بټونه );
        }
        وإلا
        {
            /* xEventGroupCreateStatic should only ever be called with
             * pxEventGroupBuffer pointing to a pre-allocated (compile time
             * allocated) StaticEventGroup_t variable. */
            trace_EVENT_ډله_CREATE_FAILED();
        }

        أرجع px_Event_بټونه;
    }

#د_که_پای /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

#إذا ( config_SUPPORT_DYNAMIC_ویش == 1 )

    Event_ډله_Handle_t x_Event_ډله_Create( فراغ )
    {
        Event_ډله_t * px_Event_بټونه;

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
        px_Event_بټونه = ( Event_ډله_t * ) pv_پورټ_Malloc( حجم( Event_ډله_t ) ); /*lint !e9087 !e9079 see comment above. */

        إذا( px_Event_بټونه != عدم )
        {
            px_Event_بټونه->ux_Event_بټونه = 0;
            vListInitialise( &( px_Event_بټونه->x_Tasks_انتظار_For_بټونه ) );

            #إذا ( config_SUPPORT_STATIC_ویش == 1 )
            {
                /* Both static and dynamic allocation can be used, so note this
                 * event group was allocated statically in case the event group is
                 * later deleted. */
                px_Event_بټونه->ucStaticallyAllocated = pdFALSE;
            }
            #د_که_پای /* configSUPPORT_STATIC_ALLOCATION */

            /* Initialize the event group's spinlock. */
            پورټ_MUX_INITIALIZE( &px_Event_بټونه->x_Event_ډله_قلف );

            trace_EVENT_ډله_CREATE( px_Event_بټونه );
        }
        وإلا
        {
            trace_EVENT_ډله_CREATE_FAILED(); /*lint !e9063 Else branch only exists to allow tracing and does not generate code if trace macros are not defined. */
        }

        أرجع px_Event_بټونه;
    }

#د_که_پای /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

Event_بټونه_t x_Event_ډله_Sync( Event_ډله_Handle_t x_Event_ډله,
                             ثابت Event_بټونه_t ux_بټونه_منزل_سېټ,
                             ثابت Event_بټونه_t ux_بټونه_منزل_Wait_For,
                             TickType_t x_Ticks_منزل_Wait )
{
    Event_بټونه_t ux_Original_بټ_ارزښت, uxReturn;
    Event_ډله_t * px_Event_بټونه = x_Event_ډله;
    بنسټ_Type_t xAlreadyYielded;
    بنسټ_Type_t xTimeoutOccurred = pdFALSE;

    configASSERT( ( ux_بټونه_منزل_Wait_For & event_EVENT_بټونه_CONTROL_بایټونه ) == 0 );
    configASSERT( ux_بټونه_منزل_Wait_For != 0 );
    #إذا ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_منزل_Wait != 0 ) ) );
    }
    #د_که_پای

    prvENTER_CRITICAL_OR_SUSPEND_ALL( &( px_Event_بټونه->x_Event_ډله_قلف ) );
    {
        ux_Original_بټ_ارزښت = px_Event_بټونه->ux_Event_بټونه;

        ( فراغ ) x_Event_ډله_سېټ_بټونه( x_Event_ډله, ux_بټونه_منزل_سېټ );

        إذا( ( ( ux_Original_بټ_ارزښت | ux_بټونه_منزل_سېټ ) & ux_بټونه_منزل_Wait_For ) == ux_بټونه_منزل_Wait_For )
        {
            /* All the rendezvous bits are now set - no need to block. */
            uxReturn = ( ux_Original_بټ_ارزښت | ux_بټونه_منزل_سېټ );

            /* Rendezvous always clear the bits.  They will have been cleared
             * already unless this is the only task in the rendezvous. */
            px_Event_بټونه->ux_Event_بټونه &= ~ux_بټونه_منزل_Wait_For;

            x_Ticks_منزل_Wait = 0;
        }
        وإلا
        {
            إذا( x_Ticks_منزل_Wait != ( TickType_t ) 0 )
            {
                trace_EVENT_ډله_SYNC_بلاک( x_Event_ډله, ux_بټونه_منزل_سېټ, ux_بټونه_منزل_Wait_For );

                /* Store the bits that the calling task is waiting for in the
                 * task's event list item so the kernel knows when a match is
                 * found.  Then enter the blocked state. */
                v_Task_ځای_پرځای_کړه_On_Unordered_Event_List( &( px_Event_بټونه->x_Tasks_انتظار_For_بټونه ), ( ux_بټونه_منزل_Wait_For | event_CLEAR_EVENTS_ON_EXIT_بټ | event_WAIT_FOR_ALL_بټونه ), x_Ticks_منزل_Wait );

                /* This assignment is obsolete as uxReturn will get set after
                 * the task unblocks, but some compilers mistakenly generate a
                 * warning about uxReturn being returned without being set if the
                 * assignment is omitted. */
                uxReturn = 0;
            }
            وإلا
            {
                /* The rendezvous bits were not set, but no block time was
                 * specified - just return the current event bit value. */
                uxReturn = px_Event_بټونه->ux_Event_بټونه;
                xTimeoutOccurred = pdTRUE;
            }
        }
    }
    xAlreadyYielded = prvEXIT_CRITICAL_OR_RESUME_ALL( &( px_Event_بټونه->x_Event_ډله_قلف ) );

    إذا( x_Ticks_منزل_Wait != ( TickType_t ) 0 )
    {
        إذا( xAlreadyYielded == pdFALSE )
        {
            پورټ_YIELD_WITHIN_API();
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }

        /* The task blocked to wait for its required bits to be set - at this
         * point either the required bits were set or the block time expired.  If
         * the required bits were set they will have been stored in the task's
         * event list item, and they should now be retrieved then cleared. */
        uxReturn = ux_Task_Reset_Event_توکی_ارزښت();

        إذا( ( uxReturn & event_UNBLOCKED_DUE_منزل_بټ_سېټ ) == ( Event_بټونه_t ) 0 )
        {
            /* The task timed out, just return the current event bit value. */
            taskENTER_CRITICAL( &( px_Event_بټونه->x_Event_ډله_قلف ) );
            {
                uxReturn = px_Event_بټونه->ux_Event_بټونه;

                /* Although the task got here because it timed out before the
                 * bits it was waiting for were set, it is possible that since it
                 * unblocked another task has set the bits.  If this is the case
                 * then it needs to clear the bits before exiting. */
                إذا( ( uxReturn & ux_بټونه_منزل_Wait_For ) == ux_بټونه_منزل_Wait_For )
                {
                    px_Event_بټونه->ux_Event_بټونه &= ~ux_بټونه_منزل_Wait_For;
                }
                وإلا
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            taskEXIT_CRITICAL( &( px_Event_بټونه->x_Event_ډله_قلف ) );

            xTimeoutOccurred = pdTRUE;
        }
        وإلا
        {
            /* The task unblocked because the bits were set. */
        }

        /* Control bits might be set as the task had blocked should not be
         * returned. */
        uxReturn &= ~event_EVENT_بټونه_CONTROL_بایټونه;
    }

    trace_EVENT_ډله_SYNC_پای_ټکی( x_Event_ډله, ux_بټونه_منزل_سېټ, ux_بټونه_منزل_Wait_For, xTimeoutOccurred );

    /* Prevent compiler warnings when trace macros are not used. */
    ( فراغ ) xTimeoutOccurred;

    أرجع uxReturn;
}
/*-----------------------------------------------------------*/

Event_بټونه_t x_Event_ډله_Wait_بټونه( Event_ډله_Handle_t x_Event_ډله,
                                 ثابت Event_بټونه_t ux_بټونه_منزل_Wait_For,
                                 ثابت بنسټ_Type_t xClearOnExit,
                                 ثابت بنسټ_Type_t x_Wait_For_All_بټونه,
                                 TickType_t x_Ticks_منزل_Wait )
{
    Event_ډله_t * px_Event_بټونه = x_Event_ډله;
    Event_بټونه_t uxReturn, ux_Control_بټونه = 0;
    بنسټ_Type_t xWaitConditionMet, xAlreadyYielded;
    بنسټ_Type_t xTimeoutOccurred = pdFALSE;

    /* Check the user is not attempting to wait on the bits used by the kernel
     * itself, and that at least one bit is being requested. */
    configASSERT( x_Event_ډله );
    configASSERT( ( ux_بټونه_منزل_Wait_For & event_EVENT_بټونه_CONTROL_بایټونه ) == 0 );
    configASSERT( ux_بټونه_منزل_Wait_For != 0 );
    #إذا ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_منزل_Wait != 0 ) ) );
    }
    #د_که_پای

    prvENTER_CRITICAL_OR_SUSPEND_ALL( &( px_Event_بټونه->x_Event_ډله_قلف ) );
    {
        ثابت Event_بټونه_t ux_Current_Event_بټونه = px_Event_بټونه->ux_Event_بټونه;

        /* Check to see if the wait condition is already met or not. */
        xWaitConditionMet = prvTestWaitCondition( ux_Current_Event_بټونه, ux_بټونه_منزل_Wait_For, x_Wait_For_All_بټونه );

        إذا( xWaitConditionMet != pdFALSE )
        {
            /* The wait condition has already been met so there is no need to
             * block. */
            uxReturn = ux_Current_Event_بټونه;
            x_Ticks_منزل_Wait = ( TickType_t ) 0;

            /* Clear the wait bits if requested to do so. */
            إذا( xClearOnExit != pdFALSE )
            {
                px_Event_بټونه->ux_Event_بټونه &= ~ux_بټونه_منزل_Wait_For;
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        وإلا إذا( x_Ticks_منزل_Wait == ( TickType_t ) 0 )
        {
            /* The wait condition has not been met, but no block time was
             * specified, so just return the current value. */
            uxReturn = ux_Current_Event_بټونه;
            xTimeoutOccurred = pdTRUE;
        }
        وإلا
        {
            /* The task is going to block to wait for its required bits to be
             * set.  uxControlBits are used to remember the specified behaviour of
             * this call to xEventGroupWaitBits() - for use when the event bits
             * unblock the task. */
            إذا( xClearOnExit != pdFALSE )
            {
                ux_Control_بټونه |= event_CLEAR_EVENTS_ON_EXIT_بټ;
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }

            إذا( x_Wait_For_All_بټونه != pdFALSE )
            {
                ux_Control_بټونه |= event_WAIT_FOR_ALL_بټونه;
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }

            /* Store the bits that the calling task is waiting for in the
             * task's event list item so the kernel knows when a match is
             * found.  Then enter the blocked state. */
            v_Task_ځای_پرځای_کړه_On_Unordered_Event_List( &( px_Event_بټونه->x_Tasks_انتظار_For_بټونه ), ( ux_بټونه_منزل_Wait_For | ux_Control_بټونه ), x_Ticks_منزل_Wait );

            /* This is obsolete as it will get set after the task unblocks, but
             * some compilers mistakenly generate a warning about the variable
             * being returned without being set if it is not done. */
            uxReturn = 0;

            trace_EVENT_ډله_WAIT_بټونه_بلاک( x_Event_ډله, ux_بټونه_منزل_Wait_For );
        }
    }
    xAlreadyYielded = prvEXIT_CRITICAL_OR_RESUME_ALL( &( px_Event_بټونه->x_Event_ډله_قلف ) );

    إذا( x_Ticks_منزل_Wait != ( TickType_t ) 0 )
    {
        إذا( xAlreadyYielded == pdFALSE )
        {
            پورټ_YIELD_WITHIN_API();
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }

        /* The task blocked to wait for its required bits to be set - at this
         * point either the required bits were set or the block time expired.  If
         * the required bits were set they will have been stored in the task's
         * event list item, and they should now be retrieved then cleared. */
        uxReturn = ux_Task_Reset_Event_توکی_ارزښت();

        إذا( ( uxReturn & event_UNBLOCKED_DUE_منزل_بټ_سېټ ) == ( Event_بټونه_t ) 0 )
        {
            taskENTER_CRITICAL( &( px_Event_بټونه->x_Event_ډله_قلف ) );
            {
                /* The task timed out, just return the current event bit value. */
                uxReturn = px_Event_بټونه->ux_Event_بټونه;

                /* It is possible that the event bits were updated between this
                 * task leaving the Blocked state and running again. */
                إذا( prvTestWaitCondition( uxReturn, ux_بټونه_منزل_Wait_For, x_Wait_For_All_بټونه ) != pdFALSE )
                {
                    إذا( xClearOnExit != pdFALSE )
                    {
                        px_Event_بټونه->ux_Event_بټونه &= ~ux_بټونه_منزل_Wait_For;
                    }
                    وإلا
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                وإلا
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                xTimeoutOccurred = pdTRUE;
            }
            taskEXIT_CRITICAL( &( px_Event_بټونه->x_Event_ډله_قلف ) );
        }
        وإلا
        {
            /* The task unblocked because the bits were set. */
        }

        /* The task blocked so control bits may have been set. */
        uxReturn &= ~event_EVENT_بټونه_CONTROL_بایټونه;
    }

    trace_EVENT_ډله_WAIT_بټونه_پای_ټکی( x_Event_ډله, ux_بټونه_منزل_Wait_For, xTimeoutOccurred );

    /* Prevent compiler warnings when trace macros are not used. */
    ( فراغ ) xTimeoutOccurred;

    أرجع uxReturn;
}
/*-----------------------------------------------------------*/

Event_بټونه_t x_Event_ډله_Clear_بټونه( Event_ډله_Handle_t x_Event_ډله,
                                  ثابت Event_بټونه_t ux_بټونه_منزل_Clear )
{
    Event_ډله_t * px_Event_بټونه = x_Event_ډله;
    Event_بټونه_t uxReturn;

    /* Check the user is not attempting to clear the bits used by the kernel
     * itself. */
    configASSERT( x_Event_ډله );
    configASSERT( ( ux_بټونه_منزل_Clear & event_EVENT_بټونه_CONTROL_بایټونه ) == 0 );

    taskENTER_CRITICAL( &( px_Event_بټونه->x_Event_ډله_قلف ) );
    {
        trace_EVENT_ډله_CLEAR_بټونه( x_Event_ډله, ux_بټونه_منزل_Clear );

        /* The value returned is the event group value prior to the bits being
         * cleared. */
        uxReturn = px_Event_بټونه->ux_Event_بټونه;

        /* Clear the bits. */
        px_Event_بټونه->ux_Event_بټونه &= ~ux_بټونه_منزل_Clear;
    }
    taskEXIT_CRITICAL( &( px_Event_بټونه->x_Event_ډله_قلف ) );

    أرجع uxReturn;
}
/*-----------------------------------------------------------*/

#إذا ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) )

    بنسټ_Type_t x_Event_ډله_Clear_بټونه_From_ISR( Event_ډله_Handle_t x_Event_ډله,
                                            ثابت Event_بټونه_t ux_بټونه_منزل_Clear )
    {
        بنسټ_Type_t xReturn;

        trace_EVENT_ډله_CLEAR_بټونه_FROM_ISR( x_Event_ډله, ux_بټونه_منزل_Clear );
        xReturn = xTimerPendFunctionCallFromISR( v_Event_ډله_Clear_بټونه_Callback, ( فراغ * ) x_Event_ډله, ( uint32_t ) ux_بټونه_منزل_Clear, عدم ); /*lint !e9087 Can't avoid cast to void* as a generic callback function not specific to this use case. Callback casts back to original type so safe. */

        أرجع xReturn;
    }

#د_که_پای /* if ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) ) */
/*-----------------------------------------------------------*/

Event_بټونه_t x_Event_ډله_Get_بټونه_From_ISR( Event_ډله_Handle_t x_Event_ډله )
{
    UBaseType_t ux_Saved_مداخله_حالت;
    Event_ډله_t ثابت * ثابت px_Event_بټونه = x_Event_ډله;
    Event_بټونه_t uxReturn;

    prv_ENTER_CRITICAL_OR_ماسک_ISR( ( پورټ_MUX_TYPE * ) &( px_Event_بټونه->x_Event_ډله_قلف ), ux_Saved_مداخله_حالت );
    {
        uxReturn = px_Event_بټونه->ux_Event_بټونه;
    }
    prvEXIT_CRITICAL_OR_UNMASK_ISR( ( پورټ_MUX_TYPE * ) &( px_Event_بټونه->x_Event_ډله_قلف ), ux_Saved_مداخله_حالت );

    أرجع uxReturn;
} /*lint !e818 EventGroupHandle_t is a typedef used in other functions to so can't be pointer to const. */
/*-----------------------------------------------------------*/

Event_بټونه_t x_Event_ډله_سېټ_بټونه( Event_ډله_Handle_t x_Event_ډله,
                                ثابت Event_بټونه_t ux_بټونه_منزل_سېټ )
{
    List_توکی_t * px_List_توکی;
    List_توکی_t * px_بل;
    List_توکی_t ثابت * px_List_پای_ټکی;
    List_t ثابت * pxList;
    Event_بټونه_t ux_بټونه_منزل_Clear = 0, ux_بټونه_Waited_For, ux_Control_بټونه;
    Event_ډله_t * px_Event_بټونه = x_Event_ډله;
    بنسټ_Type_t x_سمون_وموندل = pdFALSE;

    /* Check the user is not attempting to set the bits used by the kernel
     * itself. */
    configASSERT( x_Event_ډله );
    configASSERT( ( ux_بټونه_منزل_سېټ & event_EVENT_بټونه_CONTROL_بایټونه ) == 0 );

    pxList = &( px_Event_بټونه->x_Tasks_انتظار_For_بټونه );
    px_List_پای_ټکی = list_GET_پای_ټکی_MARKER( pxList ); /*lint !e826 !e740 !e9087 The mini list structure is used as the list end to save RAM.  This is checked and valid. */

    prvENTER_CRITICAL_OR_SUSPEND_ALL( &( px_Event_بټونه->x_Event_ډله_قلف ) );
    #إذا ( config_شمېره_د_CORES > 1 )

        /* We are about to traverse a task list which is a kernel data structure.
         * Thus we need to call prvTakeKernelLock() to take the kernel lock. */
        prv_Take_Kernel_قلف();
    #د_که_پای /* configNUMBER_OF_CORES > 1 */
    {
        trace_EVENT_ډله_سېټ_بټونه( x_Event_ډله, ux_بټونه_منزل_سېټ );

        px_List_توکی = list_GET_سر_ثبت( pxList );

        /* Set the bits. */
        px_Event_بټونه->ux_Event_بټونه |= ux_بټونه_منزل_سېټ;

        /* See if the new bit value should unblock any tasks. */
        طالما( px_List_توکی != px_List_پای_ټکی )
        {
            px_بل = list_GET_بل( px_List_توکی );
            ux_بټونه_Waited_For = list_GET_LIST_توکی_ارزښت( px_List_توکی );
            x_سمون_وموندل = pdFALSE;

            /* Split the bits waited for from the control bits. */
            ux_Control_بټونه = ux_بټونه_Waited_For & event_EVENT_بټونه_CONTROL_بایټونه;
            ux_بټونه_Waited_For &= ~event_EVENT_بټونه_CONTROL_بایټونه;

            إذا( ( ux_Control_بټونه & event_WAIT_FOR_ALL_بټونه ) == ( Event_بټونه_t ) 0 )
            {
                /* Just looking for single bit being set. */
                إذا( ( ux_بټونه_Waited_For & px_Event_بټونه->ux_Event_بټونه ) != ( Event_بټونه_t ) 0 )
                {
                    x_سمون_وموندل = pdTRUE;
                }
                وإلا
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            وإلا إذا( ( ux_بټونه_Waited_For & px_Event_بټونه->ux_Event_بټونه ) == ux_بټونه_Waited_For )
            {
                /* All bits are set. */
                x_سمون_وموندل = pdTRUE;
            }
            وإلا
            {
                /* Need all bits to be set, but not all the bits were set. */
            }

            إذا( x_سمون_وموندل != pdFALSE )
            {
                /* The bits match.  Should the bits be cleared on exit? */
                إذا( ( ux_Control_بټونه & event_CLEAR_EVENTS_ON_EXIT_بټ ) != ( Event_بټونه_t ) 0 )
                {
                    ux_بټونه_منزل_Clear |= ux_بټونه_Waited_For;
                }
                وإلا
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                /* Store the actual event flag value in the task's event list
                 * item before removing the task from the event list.  The
                 * eventUNBLOCKED_DUE_TO_BIT_SET bit is set so the task knows
                 * that is was unblocked due to its required bits matching, rather
                 * than because it timed out. */
                vTaskRemoveFromUnorderedEventList( px_List_توکی, px_Event_بټونه->ux_Event_بټونه | event_UNBLOCKED_DUE_منزل_بټ_سېټ );
            }

            /* Move onto the next list item.  Note pxListItem->pxNext is not
             * used here as the list item may have been removed from the event list
             * and inserted into the ready/pending reading list. */
            px_List_توکی = px_بل;
        }

        /* Clear any bits that matched when the eventCLEAR_EVENTS_ON_EXIT_BIT
         * bit was set in the control word. */
        px_Event_بټونه->ux_Event_بټونه &= ~ux_بټونه_منزل_Clear;
    }
    #إذا ( config_شمېره_د_CORES > 1 )
        /* Release the previously taken kernel lock. */
        prv_Release_Kernel_قلف();
    #د_که_پای /* configNUMBER_OF_CORES > 1 */
    ( فراغ ) prvEXIT_CRITICAL_OR_RESUME_ALL( &( px_Event_بټونه->x_Event_ډله_قلف ) );


    أرجع px_Event_بټونه->ux_Event_بټونه;
}
/*-----------------------------------------------------------*/

فراغ v_Event_ډله_Delete( Event_ډله_Handle_t x_Event_ډله )
{
    Event_ډله_t * px_Event_بټونه = x_Event_ډله;
    ثابت List_t * px_Tasks_انتظار_For_بټونه;

    configASSERT( px_Event_بټونه );

    px_Tasks_انتظار_For_بټونه = &( px_Event_بټونه->x_Tasks_انتظار_For_بټونه );

    prvENTER_CRITICAL_OR_SUSPEND_ALL( &( px_Event_بټونه->x_Event_ډله_قلف ) );
    #إذا ( config_شمېره_د_CORES > 1 )

        /* We are about to traverse a task list which is a kernel data structure.
         * Thus we need to call prvTakeKernelLock() to take the kernel lock. */
        prv_Take_Kernel_قلف();
    #د_که_پای /* configNUMBER_OF_CORES > 1 */
    {
        trace_EVENT_ډله_DELETE( x_Event_ډله );

        طالما( list_CURRENT_LIST_اوږدتیا( px_Tasks_انتظار_For_بټونه ) > ( UBaseType_t ) 0 )
        {
            /* Unblock the task, returning 0 as the event list is being deleted
             * and cannot therefore have any bits set. */
            configASSERT( px_Tasks_انتظار_For_بټونه->x_List_پای_ټکی.px_بل != ( ثابت List_توکی_t * ) &( px_Tasks_انتظار_For_بټونه->x_List_پای_ټکی ) );
            vTaskRemoveFromUnorderedEventList( px_Tasks_انتظار_For_بټونه->x_List_پای_ټکی.px_بل, event_UNBLOCKED_DUE_منزل_بټ_سېټ );
        }
    }
    #إذا ( config_شمېره_د_CORES > 1 )
        /* Release the previously taken kernel lock. */
        prv_Release_Kernel_قلف();
    #د_که_پای /* configNUMBER_OF_CORES > 1 */
    prvEXIT_CRITICAL_OR_RESUME_ALL( &( px_Event_بټونه->x_Event_ډله_قلف ) );

    #إذا ( ( config_SUPPORT_DYNAMIC_ویش == 1 ) && ( config_SUPPORT_STATIC_ویش == 0 ) )
    {
        /* The event group can only have been allocated dynamically - free
         * it again. */
        v_پورټ_Free( px_Event_بټونه );
    }
    #وإلاإذا ( ( config_SUPPORT_DYNAMIC_ویش == 1 ) && ( config_SUPPORT_STATIC_ویش == 1 ) )
    {
        /* The event group could have been allocated statically or
         * dynamically, so check before attempting to free the memory. */
        إذا( px_Event_بټونه->ucStaticallyAllocated == ( uint8_t ) pdFALSE )
        {
            v_پورټ_Free( px_Event_بټونه );
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    #د_که_پای /* configSUPPORT_DYNAMIC_ALLOCATION */
}
/*-----------------------------------------------------------*/

#إذا ( config_SUPPORT_STATIC_ویش == 1 )
    بنسټ_Type_t x_Event_ډله_Get_Static_بفر( Event_ډله_Handle_t x_Event_ډله,
                                           Static_Event_ډله_t ** ppx_Event_ډله_بفر )
    {
        بنسټ_Type_t xReturn;
        Event_ډله_t * px_Event_بټونه = x_Event_ډله;

        configASSERT( px_Event_بټونه );
        configASSERT( ppx_Event_ډله_بفر );

        #إذا ( config_SUPPORT_DYNAMIC_ویش == 1 )
        {
            /* Check if the event group was statically allocated. */
            إذا( px_Event_بټونه->ucStaticallyAllocated == ( uint8_t ) pdTRUE )
            {
                *ppx_Event_ډله_بفر = ( Static_Event_ډله_t * ) px_Event_بټونه;
                xReturn = pdTRUE;
            }
            وإلا
            {
                xReturn = pdFALSE;
            }
        }
        #وإلا /* configSUPPORT_DYNAMIC_ALLOCATION */
        {
            /* Event group must have been statically allocated. */
            *ppx_Event_ډله_بفر = ( Static_Event_ډله_t * ) px_Event_بټونه;
            xReturn = pdTRUE;
        }
        #د_که_پای /* configSUPPORT_DYNAMIC_ALLOCATION */

        أرجع xReturn;
    }
#د_که_پای /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

/* For internal use only - execute a 'set bits' command that was pended from
 * an interrupt. */
فراغ v_Event_ډله_سېټ_بټونه_Callback( فراغ * pv_Event_ډله,
                                 ثابت uint32_t ul_بټونه_منزل_سېټ )
{
    ( فراغ ) x_Event_ډله_سېټ_بټونه( pv_Event_ډله, ( Event_بټونه_t ) ul_بټونه_منزل_سېټ ); /*lint !e9079 Can't avoid cast to void* as a generic timer callback prototype. Callback casts back to original type so safe. */
}
/*-----------------------------------------------------------*/

/* For internal use only - execute a 'clear bits' command that was pended from
 * an interrupt. */
فراغ v_Event_ډله_Clear_بټونه_Callback( فراغ * pv_Event_ډله,
                                   ثابت uint32_t ul_بټونه_منزل_Clear )
{
    ( فراغ ) x_Event_ډله_Clear_بټونه( pv_Event_ډله, ( Event_بټونه_t ) ul_بټونه_منزل_Clear ); /*lint !e9079 Can't avoid cast to void* as a generic timer callback prototype. Callback casts back to original type so safe. */
}
/*-----------------------------------------------------------*/

ساكن بنسټ_Type_t prvTestWaitCondition( ثابت Event_بټونه_t ux_Current_Event_بټونه,
                                        ثابت Event_بټونه_t ux_بټونه_منزل_Wait_For,
                                        ثابت بنسټ_Type_t x_Wait_For_All_بټونه )
{
    بنسټ_Type_t xWaitConditionMet = pdFALSE;

    إذا( x_Wait_For_All_بټونه == pdFALSE )
    {
        /* Task only has to wait for one bit within uxBitsToWaitFor to be
         * set.  Is one already set? */
        إذا( ( ux_Current_Event_بټونه & ux_بټونه_منزل_Wait_For ) != ( Event_بټونه_t ) 0 )
        {
            xWaitConditionMet = pdTRUE;
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    وإلا
    {
        /* Task has to wait for all the bits in uxBitsToWaitFor to be set.
         * Are they set already? */
        إذا( ( ux_Current_Event_بټونه & ux_بټونه_منزل_Wait_For ) == ux_بټونه_منزل_Wait_For )
        {
            xWaitConditionMet = pdTRUE;
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }

    أرجع xWaitConditionMet;
}
/*-----------------------------------------------------------*/

#إذا ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) )

    بنسټ_Type_t x_Event_ډله_سېټ_بټونه_From_ISR( Event_ډله_Handle_t x_Event_ډله,
                                          ثابت Event_بټونه_t ux_بټونه_منزل_سېټ,
                                          بنسټ_Type_t * px_Higher_لومړیتوب_Task_Woken )
    {
        بنسټ_Type_t xReturn;

        trace_EVENT_ډله_سېټ_بټونه_FROM_ISR( x_Event_ډله, ux_بټونه_منزل_سېټ );
        xReturn = xTimerPendFunctionCallFromISR( v_Event_ډله_سېټ_بټونه_Callback, ( فراغ * ) x_Event_ډله, ( uint32_t ) ux_بټونه_منزل_سېټ, px_Higher_لومړیتوب_Task_Woken ); /*lint !e9087 Can't avoid cast to void* as a generic callback function not specific to this use case. Callback casts back to original type so safe. */

        أرجع xReturn;
    }

#د_که_پای /* if ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) ) */
/*-----------------------------------------------------------*/

#إذا ( configUSE_TRACE_FACILITY == 1 )

    UBaseType_t ux_Event_ډله_Get_شمېره( فراغ * x_Event_ډله )
    {
        UBaseType_t xReturn;
        Event_ډله_t ثابت * px_Event_بټونه = ( Event_ډله_t * ) x_Event_ډله; /*lint !e9087 !e9079 EventGroupHandle_t is a pointer to an EventGroup_t, but EventGroupHandle_t is kept opaque outside of this file for data hiding purposes. */

        إذا( x_Event_ډله == عدم )
        {
            xReturn = 0;
        }
        وإلا
        {
            xReturn = px_Event_بټونه->ux_Event_ډله_شمېره;
        }

        أرجع xReturn;
    }

#د_که_پای /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#إذا ( configUSE_TRACE_FACILITY == 1 )

    فراغ v_Event_ډله_سېټ_شمېره( فراغ * x_Event_ډله,
                               UBaseType_t ux_Event_ډله_شمېره )
    {
        ( ( Event_ډله_t * ) x_Event_ډله )->ux_Event_ډله_شمېره = ux_Event_ډله_شمېره; /*lint !e9087 !e9079 EventGroupHandle_t is a pointer to an EventGroup_t, but EventGroupHandle_t is kept opaque outside of this file for data hiding purposes. */
    }

#د_که_پای /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/
