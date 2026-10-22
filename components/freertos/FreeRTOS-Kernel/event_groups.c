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
#وصف_ڪريو MPU_WRAPPERS_INCLUDED_FROM_API_FILE

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
#وصف_رد MPU_WRAPPERS_INCLUDED_FROM_API_FILE /*lint !e961 !e750 !e9021 See comment above. */

/* The following bit fields convey control information in a task's event list
 * item value.  It is important they don't clash with the
 * taskEVENT_LIST_ITEM_VALUE_IN_USE definition. */
#إذا config_USE_16_ٻٽو_انگ_TICKS == 1
    #وصف_ڪريو event_CLEAR_EVENTS_ON_EXIT_ٻٽو_انگ    0x0100U
    #وصف_ڪريو event_UNBLOCKED_DUE_منزل_ٻٽو_انگ_سيٽ    0x0200U
    #وصف_ڪريو event_WAIT_FOR_ALL_بٽ           0x0400U
    #وصف_ڪريو event_EVENT_بٽ_CONTROL_بائيٽون    0xff00U
#وإلا
    #وصف_ڪريو event_CLEAR_EVENTS_ON_EXIT_ٻٽو_انگ    0x01000000UL
    #وصف_ڪريو event_UNBLOCKED_DUE_منزل_ٻٽو_انگ_سيٽ    0x02000000UL
    #وصف_ڪريو event_WAIT_FOR_ALL_بٽ           0x04000000UL
    #وصف_ڪريو event_EVENT_بٽ_CONTROL_بائيٽون    0xff000000UL
#جيڪڏهن_پورو

تعريفنوع بنية Event_گروهه_Def_t
{
    Event_بٽ_t ux_Event_بٽ;
    List_t x_Tasks_انتظار_For_بٽ; /*< List of tasks waiting for a bit to be set. */

    #إذا ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_Event_گروهه_عدد;
    #جيڪڏهن_پورو

    #إذا ( ( config_SUPPORT_STATIC_ورهاست == 1 ) && ( config_SUPPORT_DYNAMIC_ورهاست == 1 ) )
        uint8_t ucStaticallyAllocated; /*< Set to pdTRUE if the event group is statically allocated to ensure no attempt is made to free the memory. */
    #جيڪڏهن_پورو

    پورٽ_MUX_TYPE x_Event_گروهه_تالو; /* Spinlock required for SMP critical sections */
} Event_گروهه_t;

/*-----------------------------------------------------------*/

/*
 * Test the bits set in uxCurrentEventBits to see if the wait condition is met.
 * The wait condition is defined by xWaitForAllBits.  If xWaitForAllBits is
 * pdTRUE then the wait condition is met if all the bits set in uxBitsToWaitFor
 * are also set in uxCurrentEventBits.  If xWaitForAllBits is pdFALSE then the
 * wait condition is met if any of the bits set in uxBitsToWait for are also set
 * in uxCurrentEventBits.
 */
ساكن بنياد_Type_t prvTestWaitCondition( ثابت Event_بٽ_t ux_Current_Event_بٽ,
                                        ثابت Event_بٽ_t ux_بٽ_منزل_Wait_For,
                                        ثابت بنياد_Type_t x_Wait_For_All_بٽ ) PRIVILEGED_FUNCTION;

/*-----------------------------------------------------------*/

#إذا ( config_SUPPORT_STATIC_ورهاست == 1 )

    Event_گروهه_Handle_t x_Event_گروهه_Create_Static( Static_Event_گروهه_t * px_Event_گروهه_بفر )
    {
        Event_گروهه_t * px_Event_بٽ;

        /* A StaticEventGroup_t object must be provided. */
        configASSERT( px_Event_گروهه_بفر );

        #إذا ( configASSERT_DEFINED == 1 )
        {
            /* Sanity check that the size of the structure used to declare a
             * variable of type StaticEventGroup_t equals the size of the real
             * event group structure. */
            متطاير size_t x_سائيز = حجم( Static_Event_گروهه_t );
            configASSERT( x_سائيز == حجم( Event_گروهه_t ) );
            ( فراغ ) x_سائيز; /* Prevent unused variable warning when configASSERT() is not used. */
        } /*lint !e529 xSize is referenced if configASSERT() is defined. */
        #جيڪڏهن_پورو /* configASSERT_DEFINED */

        /* The user has provided a statically allocated event group - use it. */
        px_Event_بٽ = ( Event_گروهه_t * ) px_Event_گروهه_بفر; /*lint !e740 !e9087 EventGroup_t and StaticEventGroup_t are deliberately aliased for data hiding purposes and guaranteed to have the same size and alignment requirement - checked by configASSERT(). */

        إذا( px_Event_بٽ != عدم )
        {
            px_Event_بٽ->ux_Event_بٽ = 0;
            vListInitialise( &( px_Event_بٽ->x_Tasks_انتظار_For_بٽ ) );

            #إذا ( config_SUPPORT_DYNAMIC_ورهاست == 1 )
            {
                /* Both static and dynamic allocation can be used, so note that
                 * this event group was created statically in case the event group
                 * is later deleted. */
                px_Event_بٽ->ucStaticallyAllocated = pdTRUE;
            }
            #جيڪڏهن_پورو /* configSUPPORT_DYNAMIC_ALLOCATION */

            /* Initialize the event group's spinlock. */
            پورٽ_MUX_INITIALIZE( &px_Event_بٽ->x_Event_گروهه_تالو );

            trace_EVENT_گروهه_CREATE( px_Event_بٽ );
        }
        وإلا
        {
            /* xEventGroupCreateStatic should only ever be called with
             * pxEventGroupBuffer pointing to a pre-allocated (compile time
             * allocated) StaticEventGroup_t variable. */
            trace_EVENT_گروهه_CREATE_FAILED();
        }

        أرجع px_Event_بٽ;
    }

#جيڪڏهن_پورو /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

#إذا ( config_SUPPORT_DYNAMIC_ورهاست == 1 )

    Event_گروهه_Handle_t x_Event_گروهه_Create( فراغ )
    {
        Event_گروهه_t * px_Event_بٽ;

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
        px_Event_بٽ = ( Event_گروهه_t * ) pv_پورٽ_Malloc( حجم( Event_گروهه_t ) ); /*lint !e9087 !e9079 see comment above. */

        إذا( px_Event_بٽ != عدم )
        {
            px_Event_بٽ->ux_Event_بٽ = 0;
            vListInitialise( &( px_Event_بٽ->x_Tasks_انتظار_For_بٽ ) );

            #إذا ( config_SUPPORT_STATIC_ورهاست == 1 )
            {
                /* Both static and dynamic allocation can be used, so note this
                 * event group was allocated statically in case the event group is
                 * later deleted. */
                px_Event_بٽ->ucStaticallyAllocated = pdFALSE;
            }
            #جيڪڏهن_پورو /* configSUPPORT_STATIC_ALLOCATION */

            /* Initialize the event group's spinlock. */
            پورٽ_MUX_INITIALIZE( &px_Event_بٽ->x_Event_گروهه_تالو );

            trace_EVENT_گروهه_CREATE( px_Event_بٽ );
        }
        وإلا
        {
            trace_EVENT_گروهه_CREATE_FAILED(); /*lint !e9063 Else branch only exists to allow tracing and does not generate code if trace macros are not defined. */
        }

        أرجع px_Event_بٽ;
    }

#جيڪڏهن_پورو /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

Event_بٽ_t x_Event_گروهه_Sync( Event_گروهه_Handle_t x_Event_گروهه,
                             ثابت Event_بٽ_t ux_بٽ_منزل_سيٽ,
                             ثابت Event_بٽ_t ux_بٽ_منزل_Wait_For,
                             TickType_t x_Ticks_منزل_Wait )
{
    Event_بٽ_t ux_Original_ٻٽو_انگ_قدر, uxReturn;
    Event_گروهه_t * px_Event_بٽ = x_Event_گروهه;
    بنياد_Type_t xAlreadyYielded;
    بنياد_Type_t xTimeoutOccurred = pdFALSE;

    configASSERT( ( ux_بٽ_منزل_Wait_For & event_EVENT_بٽ_CONTROL_بائيٽون ) == 0 );
    configASSERT( ux_بٽ_منزل_Wait_For != 0 );
    #إذا ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_منزل_Wait != 0 ) ) );
    }
    #جيڪڏهن_پورو

    prvENTER_CRITICAL_OR_SUSPEND_ALL( &( px_Event_بٽ->x_Event_گروهه_تالو ) );
    {
        ux_Original_ٻٽو_انگ_قدر = px_Event_بٽ->ux_Event_بٽ;

        ( فراغ ) x_Event_گروهه_سيٽ_بٽ( x_Event_گروهه, ux_بٽ_منزل_سيٽ );

        إذا( ( ( ux_Original_ٻٽو_انگ_قدر | ux_بٽ_منزل_سيٽ ) & ux_بٽ_منزل_Wait_For ) == ux_بٽ_منزل_Wait_For )
        {
            /* All the rendezvous bits are now set - no need to block. */
            uxReturn = ( ux_Original_ٻٽو_انگ_قدر | ux_بٽ_منزل_سيٽ );

            /* Rendezvous always clear the bits.  They will have been cleared
             * already unless this is the only task in the rendezvous. */
            px_Event_بٽ->ux_Event_بٽ &= ~ux_بٽ_منزل_Wait_For;

            x_Ticks_منزل_Wait = 0;
        }
        وإلا
        {
            إذا( x_Ticks_منزل_Wait != ( TickType_t ) 0 )
            {
                trace_EVENT_گروهه_SYNC_بلاڪ( x_Event_گروهه, ux_بٽ_منزل_سيٽ, ux_بٽ_منزل_Wait_For );

                /* Store the bits that the calling task is waiting for in the
                 * task's event list item so the kernel knows when a match is
                 * found.  Then enter the blocked state. */
                v_Task_رکو_On_Unordered_Event_List( &( px_Event_بٽ->x_Tasks_انتظار_For_بٽ ), ( ux_بٽ_منزل_Wait_For | event_CLEAR_EVENTS_ON_EXIT_ٻٽو_انگ | event_WAIT_FOR_ALL_بٽ ), x_Ticks_منزل_Wait );

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
                uxReturn = px_Event_بٽ->ux_Event_بٽ;
                xTimeoutOccurred = pdTRUE;
            }
        }
    }
    xAlreadyYielded = prvEXIT_CRITICAL_OR_RESUME_ALL( &( px_Event_بٽ->x_Event_گروهه_تالو ) );

    إذا( x_Ticks_منزل_Wait != ( TickType_t ) 0 )
    {
        إذا( xAlreadyYielded == pdFALSE )
        {
            پورٽ_YIELD_WITHIN_API();
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }

        /* The task blocked to wait for its required bits to be set - at this
         * point either the required bits were set or the block time expired.  If
         * the required bits were set they will have been stored in the task's
         * event list item, and they should now be retrieved then cleared. */
        uxReturn = ux_Task_Reset_Event_شي_قدر();

        إذا( ( uxReturn & event_UNBLOCKED_DUE_منزل_ٻٽو_انگ_سيٽ ) == ( Event_بٽ_t ) 0 )
        {
            /* The task timed out, just return the current event bit value. */
            taskENTER_CRITICAL( &( px_Event_بٽ->x_Event_گروهه_تالو ) );
            {
                uxReturn = px_Event_بٽ->ux_Event_بٽ;

                /* Although the task got here because it timed out before the
                 * bits it was waiting for were set, it is possible that since it
                 * unblocked another task has set the bits.  If this is the case
                 * then it needs to clear the bits before exiting. */
                إذا( ( uxReturn & ux_بٽ_منزل_Wait_For ) == ux_بٽ_منزل_Wait_For )
                {
                    px_Event_بٽ->ux_Event_بٽ &= ~ux_بٽ_منزل_Wait_For;
                }
                وإلا
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            taskEXIT_CRITICAL( &( px_Event_بٽ->x_Event_گروهه_تالو ) );

            xTimeoutOccurred = pdTRUE;
        }
        وإلا
        {
            /* The task unblocked because the bits were set. */
        }

        /* Control bits might be set as the task had blocked should not be
         * returned. */
        uxReturn &= ~event_EVENT_بٽ_CONTROL_بائيٽون;
    }

    trace_EVENT_گروهه_SYNC_پڇاڙي( x_Event_گروهه, ux_بٽ_منزل_سيٽ, ux_بٽ_منزل_Wait_For, xTimeoutOccurred );

    /* Prevent compiler warnings when trace macros are not used. */
    ( فراغ ) xTimeoutOccurred;

    أرجع uxReturn;
}
/*-----------------------------------------------------------*/

Event_بٽ_t x_Event_گروهه_Wait_بٽ( Event_گروهه_Handle_t x_Event_گروهه,
                                 ثابت Event_بٽ_t ux_بٽ_منزل_Wait_For,
                                 ثابت بنياد_Type_t xClearOnExit,
                                 ثابت بنياد_Type_t x_Wait_For_All_بٽ,
                                 TickType_t x_Ticks_منزل_Wait )
{
    Event_گروهه_t * px_Event_بٽ = x_Event_گروهه;
    Event_بٽ_t uxReturn, ux_Control_بٽ = 0;
    بنياد_Type_t xWaitConditionMet, xAlreadyYielded;
    بنياد_Type_t xTimeoutOccurred = pdFALSE;

    /* Check the user is not attempting to wait on the bits used by the kernel
     * itself, and that at least one bit is being requested. */
    configASSERT( x_Event_گروهه );
    configASSERT( ( ux_بٽ_منزل_Wait_For & event_EVENT_بٽ_CONTROL_بائيٽون ) == 0 );
    configASSERT( ux_بٽ_منزل_Wait_For != 0 );
    #إذا ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_منزل_Wait != 0 ) ) );
    }
    #جيڪڏهن_پورو

    prvENTER_CRITICAL_OR_SUSPEND_ALL( &( px_Event_بٽ->x_Event_گروهه_تالو ) );
    {
        ثابت Event_بٽ_t ux_Current_Event_بٽ = px_Event_بٽ->ux_Event_بٽ;

        /* Check to see if the wait condition is already met or not. */
        xWaitConditionMet = prvTestWaitCondition( ux_Current_Event_بٽ, ux_بٽ_منزل_Wait_For, x_Wait_For_All_بٽ );

        إذا( xWaitConditionMet != pdFALSE )
        {
            /* The wait condition has already been met so there is no need to
             * block. */
            uxReturn = ux_Current_Event_بٽ;
            x_Ticks_منزل_Wait = ( TickType_t ) 0;

            /* Clear the wait bits if requested to do so. */
            إذا( xClearOnExit != pdFALSE )
            {
                px_Event_بٽ->ux_Event_بٽ &= ~ux_بٽ_منزل_Wait_For;
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
            uxReturn = ux_Current_Event_بٽ;
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
                ux_Control_بٽ |= event_CLEAR_EVENTS_ON_EXIT_ٻٽو_انگ;
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }

            إذا( x_Wait_For_All_بٽ != pdFALSE )
            {
                ux_Control_بٽ |= event_WAIT_FOR_ALL_بٽ;
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }

            /* Store the bits that the calling task is waiting for in the
             * task's event list item so the kernel knows when a match is
             * found.  Then enter the blocked state. */
            v_Task_رکو_On_Unordered_Event_List( &( px_Event_بٽ->x_Tasks_انتظار_For_بٽ ), ( ux_بٽ_منزل_Wait_For | ux_Control_بٽ ), x_Ticks_منزل_Wait );

            /* This is obsolete as it will get set after the task unblocks, but
             * some compilers mistakenly generate a warning about the variable
             * being returned without being set if it is not done. */
            uxReturn = 0;

            trace_EVENT_گروهه_WAIT_بٽ_بلاڪ( x_Event_گروهه, ux_بٽ_منزل_Wait_For );
        }
    }
    xAlreadyYielded = prvEXIT_CRITICAL_OR_RESUME_ALL( &( px_Event_بٽ->x_Event_گروهه_تالو ) );

    إذا( x_Ticks_منزل_Wait != ( TickType_t ) 0 )
    {
        إذا( xAlreadyYielded == pdFALSE )
        {
            پورٽ_YIELD_WITHIN_API();
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }

        /* The task blocked to wait for its required bits to be set - at this
         * point either the required bits were set or the block time expired.  If
         * the required bits were set they will have been stored in the task's
         * event list item, and they should now be retrieved then cleared. */
        uxReturn = ux_Task_Reset_Event_شي_قدر();

        إذا( ( uxReturn & event_UNBLOCKED_DUE_منزل_ٻٽو_انگ_سيٽ ) == ( Event_بٽ_t ) 0 )
        {
            taskENTER_CRITICAL( &( px_Event_بٽ->x_Event_گروهه_تالو ) );
            {
                /* The task timed out, just return the current event bit value. */
                uxReturn = px_Event_بٽ->ux_Event_بٽ;

                /* It is possible that the event bits were updated between this
                 * task leaving the Blocked state and running again. */
                إذا( prvTestWaitCondition( uxReturn, ux_بٽ_منزل_Wait_For, x_Wait_For_All_بٽ ) != pdFALSE )
                {
                    إذا( xClearOnExit != pdFALSE )
                    {
                        px_Event_بٽ->ux_Event_بٽ &= ~ux_بٽ_منزل_Wait_For;
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
            taskEXIT_CRITICAL( &( px_Event_بٽ->x_Event_گروهه_تالو ) );
        }
        وإلا
        {
            /* The task unblocked because the bits were set. */
        }

        /* The task blocked so control bits may have been set. */
        uxReturn &= ~event_EVENT_بٽ_CONTROL_بائيٽون;
    }

    trace_EVENT_گروهه_WAIT_بٽ_پڇاڙي( x_Event_گروهه, ux_بٽ_منزل_Wait_For, xTimeoutOccurred );

    /* Prevent compiler warnings when trace macros are not used. */
    ( فراغ ) xTimeoutOccurred;

    أرجع uxReturn;
}
/*-----------------------------------------------------------*/

Event_بٽ_t x_Event_گروهه_Clear_بٽ( Event_گروهه_Handle_t x_Event_گروهه,
                                  ثابت Event_بٽ_t ux_بٽ_منزل_Clear )
{
    Event_گروهه_t * px_Event_بٽ = x_Event_گروهه;
    Event_بٽ_t uxReturn;

    /* Check the user is not attempting to clear the bits used by the kernel
     * itself. */
    configASSERT( x_Event_گروهه );
    configASSERT( ( ux_بٽ_منزل_Clear & event_EVENT_بٽ_CONTROL_بائيٽون ) == 0 );

    taskENTER_CRITICAL( &( px_Event_بٽ->x_Event_گروهه_تالو ) );
    {
        trace_EVENT_گروهه_CLEAR_بٽ( x_Event_گروهه, ux_بٽ_منزل_Clear );

        /* The value returned is the event group value prior to the bits being
         * cleared. */
        uxReturn = px_Event_بٽ->ux_Event_بٽ;

        /* Clear the bits. */
        px_Event_بٽ->ux_Event_بٽ &= ~ux_بٽ_منزل_Clear;
    }
    taskEXIT_CRITICAL( &( px_Event_بٽ->x_Event_گروهه_تالو ) );

    أرجع uxReturn;
}
/*-----------------------------------------------------------*/

#إذا ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) )

    بنياد_Type_t x_Event_گروهه_Clear_بٽ_From_ISR( Event_گروهه_Handle_t x_Event_گروهه,
                                            ثابت Event_بٽ_t ux_بٽ_منزل_Clear )
    {
        بنياد_Type_t xReturn;

        trace_EVENT_گروهه_CLEAR_بٽ_FROM_ISR( x_Event_گروهه, ux_بٽ_منزل_Clear );
        xReturn = xTimerPendFunctionCallFromISR( v_Event_گروهه_Clear_بٽ_Callback, ( فراغ * ) x_Event_گروهه, ( uint32_t ) ux_بٽ_منزل_Clear, عدم ); /*lint !e9087 Can't avoid cast to void* as a generic callback function not specific to this use case. Callback casts back to original type so safe. */

        أرجع xReturn;
    }

#جيڪڏهن_پورو /* if ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) ) */
/*-----------------------------------------------------------*/

Event_بٽ_t x_Event_گروهه_Get_بٽ_From_ISR( Event_گروهه_Handle_t x_Event_گروهه )
{
    UBaseType_t ux_Saved_مداخلت_حالت;
    Event_گروهه_t ثابت * ثابت px_Event_بٽ = x_Event_گروهه;
    Event_بٽ_t uxReturn;

    prv_ENTER_CRITICAL_OR_نقاب_ISR( ( پورٽ_MUX_TYPE * ) &( px_Event_بٽ->x_Event_گروهه_تالو ), ux_Saved_مداخلت_حالت );
    {
        uxReturn = px_Event_بٽ->ux_Event_بٽ;
    }
    prvEXIT_CRITICAL_OR_UNMASK_ISR( ( پورٽ_MUX_TYPE * ) &( px_Event_بٽ->x_Event_گروهه_تالو ), ux_Saved_مداخلت_حالت );

    أرجع uxReturn;
} /*lint !e818 EventGroupHandle_t is a typedef used in other functions to so can't be pointer to const. */
/*-----------------------------------------------------------*/

Event_بٽ_t x_Event_گروهه_سيٽ_بٽ( Event_گروهه_Handle_t x_Event_گروهه,
                                ثابت Event_بٽ_t ux_بٽ_منزل_سيٽ )
{
    List_شي_t * px_List_شي;
    List_شي_t * px_ايندڙ;
    List_شي_t ثابت * px_List_پڇاڙي;
    List_t ثابت * pxList;
    Event_بٽ_t ux_بٽ_منزل_Clear = 0, ux_بٽ_Waited_For, ux_Control_بٽ;
    Event_گروهه_t * px_Event_بٽ = x_Event_گروهه;
    بنياد_Type_t x_ميلاپ_مليو = pdFALSE;

    /* Check the user is not attempting to set the bits used by the kernel
     * itself. */
    configASSERT( x_Event_گروهه );
    configASSERT( ( ux_بٽ_منزل_سيٽ & event_EVENT_بٽ_CONTROL_بائيٽون ) == 0 );

    pxList = &( px_Event_بٽ->x_Tasks_انتظار_For_بٽ );
    px_List_پڇاڙي = list_GET_پڇاڙي_MARKER( pxList ); /*lint !e826 !e740 !e9087 The mini list structure is used as the list end to save RAM.  This is checked and valid. */

    prvENTER_CRITICAL_OR_SUSPEND_ALL( &( px_Event_بٽ->x_Event_گروهه_تالو ) );
    #إذا ( config_عدد_جو_CORES > 1 )

        /* We are about to traverse a task list which is a kernel data structure.
         * Thus we need to call prvTakeKernelLock() to take the kernel lock. */
        prv_Take_Kernel_تالو();
    #جيڪڏهن_پورو /* configNUMBER_OF_CORES > 1 */
    {
        trace_EVENT_گروهه_سيٽ_بٽ( x_Event_گروهه, ux_بٽ_منزل_سيٽ );

        px_List_شي = list_GET_سر_داخلا( pxList );

        /* Set the bits. */
        px_Event_بٽ->ux_Event_بٽ |= ux_بٽ_منزل_سيٽ;

        /* See if the new bit value should unblock any tasks. */
        طالما( px_List_شي != px_List_پڇاڙي )
        {
            px_ايندڙ = list_GET_ايندڙ( px_List_شي );
            ux_بٽ_Waited_For = list_GET_LIST_شي_قدر( px_List_شي );
            x_ميلاپ_مليو = pdFALSE;

            /* Split the bits waited for from the control bits. */
            ux_Control_بٽ = ux_بٽ_Waited_For & event_EVENT_بٽ_CONTROL_بائيٽون;
            ux_بٽ_Waited_For &= ~event_EVENT_بٽ_CONTROL_بائيٽون;

            إذا( ( ux_Control_بٽ & event_WAIT_FOR_ALL_بٽ ) == ( Event_بٽ_t ) 0 )
            {
                /* Just looking for single bit being set. */
                إذا( ( ux_بٽ_Waited_For & px_Event_بٽ->ux_Event_بٽ ) != ( Event_بٽ_t ) 0 )
                {
                    x_ميلاپ_مليو = pdTRUE;
                }
                وإلا
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            وإلا إذا( ( ux_بٽ_Waited_For & px_Event_بٽ->ux_Event_بٽ ) == ux_بٽ_Waited_For )
            {
                /* All bits are set. */
                x_ميلاپ_مليو = pdTRUE;
            }
            وإلا
            {
                /* Need all bits to be set, but not all the bits were set. */
            }

            إذا( x_ميلاپ_مليو != pdFALSE )
            {
                /* The bits match.  Should the bits be cleared on exit? */
                إذا( ( ux_Control_بٽ & event_CLEAR_EVENTS_ON_EXIT_ٻٽو_انگ ) != ( Event_بٽ_t ) 0 )
                {
                    ux_بٽ_منزل_Clear |= ux_بٽ_Waited_For;
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
                vTaskRemoveFromUnorderedEventList( px_List_شي, px_Event_بٽ->ux_Event_بٽ | event_UNBLOCKED_DUE_منزل_ٻٽو_انگ_سيٽ );
            }

            /* Move onto the next list item.  Note pxListItem->pxNext is not
             * used here as the list item may have been removed from the event list
             * and inserted into the ready/pending reading list. */
            px_List_شي = px_ايندڙ;
        }

        /* Clear any bits that matched when the eventCLEAR_EVENTS_ON_EXIT_BIT
         * bit was set in the control word. */
        px_Event_بٽ->ux_Event_بٽ &= ~ux_بٽ_منزل_Clear;
    }
    #إذا ( config_عدد_جو_CORES > 1 )
        /* Release the previously taken kernel lock. */
        prv_Release_Kernel_تالو();
    #جيڪڏهن_پورو /* configNUMBER_OF_CORES > 1 */
    ( فراغ ) prvEXIT_CRITICAL_OR_RESUME_ALL( &( px_Event_بٽ->x_Event_گروهه_تالو ) );


    أرجع px_Event_بٽ->ux_Event_بٽ;
}
/*-----------------------------------------------------------*/

فراغ v_Event_گروهه_Delete( Event_گروهه_Handle_t x_Event_گروهه )
{
    Event_گروهه_t * px_Event_بٽ = x_Event_گروهه;
    ثابت List_t * px_Tasks_انتظار_For_بٽ;

    configASSERT( px_Event_بٽ );

    px_Tasks_انتظار_For_بٽ = &( px_Event_بٽ->x_Tasks_انتظار_For_بٽ );

    prvENTER_CRITICAL_OR_SUSPEND_ALL( &( px_Event_بٽ->x_Event_گروهه_تالو ) );
    #إذا ( config_عدد_جو_CORES > 1 )

        /* We are about to traverse a task list which is a kernel data structure.
         * Thus we need to call prvTakeKernelLock() to take the kernel lock. */
        prv_Take_Kernel_تالو();
    #جيڪڏهن_پورو /* configNUMBER_OF_CORES > 1 */
    {
        trace_EVENT_گروهه_DELETE( x_Event_گروهه );

        طالما( list_CURRENT_LIST_ڊيگهائي( px_Tasks_انتظار_For_بٽ ) > ( UBaseType_t ) 0 )
        {
            /* Unblock the task, returning 0 as the event list is being deleted
             * and cannot therefore have any bits set. */
            configASSERT( px_Tasks_انتظار_For_بٽ->x_List_پڇاڙي.px_ايندڙ != ( ثابت List_شي_t * ) &( px_Tasks_انتظار_For_بٽ->x_List_پڇاڙي ) );
            vTaskRemoveFromUnorderedEventList( px_Tasks_انتظار_For_بٽ->x_List_پڇاڙي.px_ايندڙ, event_UNBLOCKED_DUE_منزل_ٻٽو_انگ_سيٽ );
        }
    }
    #إذا ( config_عدد_جو_CORES > 1 )
        /* Release the previously taken kernel lock. */
        prv_Release_Kernel_تالو();
    #جيڪڏهن_پورو /* configNUMBER_OF_CORES > 1 */
    prvEXIT_CRITICAL_OR_RESUME_ALL( &( px_Event_بٽ->x_Event_گروهه_تالو ) );

    #إذا ( ( config_SUPPORT_DYNAMIC_ورهاست == 1 ) && ( config_SUPPORT_STATIC_ورهاست == 0 ) )
    {
        /* The event group can only have been allocated dynamically - free
         * it again. */
        v_پورٽ_Free( px_Event_بٽ );
    }
    #وإلاإذا ( ( config_SUPPORT_DYNAMIC_ورهاست == 1 ) && ( config_SUPPORT_STATIC_ورهاست == 1 ) )
    {
        /* The event group could have been allocated statically or
         * dynamically, so check before attempting to free the memory. */
        إذا( px_Event_بٽ->ucStaticallyAllocated == ( uint8_t ) pdFALSE )
        {
            v_پورٽ_Free( px_Event_بٽ );
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    #جيڪڏهن_پورو /* configSUPPORT_DYNAMIC_ALLOCATION */
}
/*-----------------------------------------------------------*/

#إذا ( config_SUPPORT_STATIC_ورهاست == 1 )
    بنياد_Type_t x_Event_گروهه_Get_Static_بفر( Event_گروهه_Handle_t x_Event_گروهه,
                                           Static_Event_گروهه_t ** ppx_Event_گروهه_بفر )
    {
        بنياد_Type_t xReturn;
        Event_گروهه_t * px_Event_بٽ = x_Event_گروهه;

        configASSERT( px_Event_بٽ );
        configASSERT( ppx_Event_گروهه_بفر );

        #إذا ( config_SUPPORT_DYNAMIC_ورهاست == 1 )
        {
            /* Check if the event group was statically allocated. */
            إذا( px_Event_بٽ->ucStaticallyAllocated == ( uint8_t ) pdTRUE )
            {
                *ppx_Event_گروهه_بفر = ( Static_Event_گروهه_t * ) px_Event_بٽ;
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
            *ppx_Event_گروهه_بفر = ( Static_Event_گروهه_t * ) px_Event_بٽ;
            xReturn = pdTRUE;
        }
        #جيڪڏهن_پورو /* configSUPPORT_DYNAMIC_ALLOCATION */

        أرجع xReturn;
    }
#جيڪڏهن_پورو /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

/* For internal use only - execute a 'set bits' command that was pended from
 * an interrupt. */
فراغ v_Event_گروهه_سيٽ_بٽ_Callback( فراغ * pv_Event_گروهه,
                                 ثابت uint32_t ul_بٽ_منزل_سيٽ )
{
    ( فراغ ) x_Event_گروهه_سيٽ_بٽ( pv_Event_گروهه, ( Event_بٽ_t ) ul_بٽ_منزل_سيٽ ); /*lint !e9079 Can't avoid cast to void* as a generic timer callback prototype. Callback casts back to original type so safe. */
}
/*-----------------------------------------------------------*/

/* For internal use only - execute a 'clear bits' command that was pended from
 * an interrupt. */
فراغ v_Event_گروهه_Clear_بٽ_Callback( فراغ * pv_Event_گروهه,
                                   ثابت uint32_t ul_بٽ_منزل_Clear )
{
    ( فراغ ) x_Event_گروهه_Clear_بٽ( pv_Event_گروهه, ( Event_بٽ_t ) ul_بٽ_منزل_Clear ); /*lint !e9079 Can't avoid cast to void* as a generic timer callback prototype. Callback casts back to original type so safe. */
}
/*-----------------------------------------------------------*/

ساكن بنياد_Type_t prvTestWaitCondition( ثابت Event_بٽ_t ux_Current_Event_بٽ,
                                        ثابت Event_بٽ_t ux_بٽ_منزل_Wait_For,
                                        ثابت بنياد_Type_t x_Wait_For_All_بٽ )
{
    بنياد_Type_t xWaitConditionMet = pdFALSE;

    إذا( x_Wait_For_All_بٽ == pdFALSE )
    {
        /* Task only has to wait for one bit within uxBitsToWaitFor to be
         * set.  Is one already set? */
        إذا( ( ux_Current_Event_بٽ & ux_بٽ_منزل_Wait_For ) != ( Event_بٽ_t ) 0 )
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
        إذا( ( ux_Current_Event_بٽ & ux_بٽ_منزل_Wait_For ) == ux_بٽ_منزل_Wait_For )
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

    بنياد_Type_t x_Event_گروهه_سيٽ_بٽ_From_ISR( Event_گروهه_Handle_t x_Event_گروهه,
                                          ثابت Event_بٽ_t ux_بٽ_منزل_سيٽ,
                                          بنياد_Type_t * px_Higher_ترجيح_Task_Woken )
    {
        بنياد_Type_t xReturn;

        trace_EVENT_گروهه_سيٽ_بٽ_FROM_ISR( x_Event_گروهه, ux_بٽ_منزل_سيٽ );
        xReturn = xTimerPendFunctionCallFromISR( v_Event_گروهه_سيٽ_بٽ_Callback, ( فراغ * ) x_Event_گروهه, ( uint32_t ) ux_بٽ_منزل_سيٽ, px_Higher_ترجيح_Task_Woken ); /*lint !e9087 Can't avoid cast to void* as a generic callback function not specific to this use case. Callback casts back to original type so safe. */

        أرجع xReturn;
    }

#جيڪڏهن_پورو /* if ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) ) */
/*-----------------------------------------------------------*/

#إذا ( configUSE_TRACE_FACILITY == 1 )

    UBaseType_t ux_Event_گروهه_Get_عدد( فراغ * x_Event_گروهه )
    {
        UBaseType_t xReturn;
        Event_گروهه_t ثابت * px_Event_بٽ = ( Event_گروهه_t * ) x_Event_گروهه; /*lint !e9087 !e9079 EventGroupHandle_t is a pointer to an EventGroup_t, but EventGroupHandle_t is kept opaque outside of this file for data hiding purposes. */

        إذا( x_Event_گروهه == عدم )
        {
            xReturn = 0;
        }
        وإلا
        {
            xReturn = px_Event_بٽ->ux_Event_گروهه_عدد;
        }

        أرجع xReturn;
    }

#جيڪڏهن_پورو /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#إذا ( configUSE_TRACE_FACILITY == 1 )

    فراغ v_Event_گروهه_سيٽ_عدد( فراغ * x_Event_گروهه,
                               UBaseType_t ux_Event_گروهه_عدد )
    {
        ( ( Event_گروهه_t * ) x_Event_گروهه )->ux_Event_گروهه_عدد = ux_Event_گروهه_عدد; /*lint !e9087 !e9079 EventGroupHandle_t is a pointer to an EventGroup_t, but EventGroupHandle_t is kept opaque outside of this file for data hiding purposes. */
    }

#جيڪڏهن_پورو /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/
