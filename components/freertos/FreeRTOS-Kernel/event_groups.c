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
#تعریف_کن MPU_WRAPPERS_INCLUDED_FROM_API_FILE

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
#لغو_تعریف MPU_WRAPPERS_INCLUDED_FROM_API_FILE /*lint !e961 !e750 !e9021 See comment above. */

/* The following bit fields convey control information in a task's event list
 * item value.  It is important they don't clash with the
 * taskEVENT_LIST_ITEM_VALUE_IN_USE definition. */
#إذا config_USE_16_بیت_TICKS == 1
    #تعریف_کن event_CLEAR_EVENTS_ON_EXIT_بیت    0x0100U
    #تعریف_کن event_UNBLOCKED_DUE_مقصد_بیت_مجموعه    0x0200U
    #تعریف_کن event_WAIT_FOR_ALL_بیتها           0x0400U
    #تعریف_کن event_EVENT_بیتها_CONTROL_بایتها    0xff00U
#وإلا
    #تعریف_کن event_CLEAR_EVENTS_ON_EXIT_بیت    0x01000000UL
    #تعریف_کن event_UNBLOCKED_DUE_مقصد_بیت_مجموعه    0x02000000UL
    #تعریف_کن event_WAIT_FOR_ALL_بیتها           0x04000000UL
    #تعریف_کن event_EVENT_بیتها_CONTROL_بایتها    0xff000000UL
#پایان_اگر

تعريفنوع بنية Event_گروه_Def_t
{
    Event_بیتها_t ux_Event_بیتها;
    List_t x_Tasks_انتظار_For_بیتها; /*< List of tasks waiting for a bit to be set. */

    #إذا ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_Event_گروه_عدد;
    #پایان_اگر

    #إذا ( ( config_SUPPORT_STATIC_تخصیص == 1 ) && ( config_SUPPORT_DYNAMIC_تخصیص == 1 ) )
        uint8_t ucStaticallyAllocated; /*< Set to pdTRUE if the event group is statically allocated to ensure no attempt is made to free the memory. */
    #پایان_اگر

    پورت_MUX_TYPE x_Event_گروه_قفل; /* Spinlock required for SMP critical sections */
} Event_گروه_t;

/*-----------------------------------------------------------*/

/*
 * Test the bits set in uxCurrentEventBits to see if the wait condition is met.
 * The wait condition is defined by xWaitForAllBits.  If xWaitForAllBits is
 * pdTRUE then the wait condition is met if all the bits set in uxBitsToWaitFor
 * are also set in uxCurrentEventBits.  If xWaitForAllBits is pdFALSE then the
 * wait condition is met if any of the bits set in uxBitsToWait for are also set
 * in uxCurrentEventBits.
 */
ساكن پایه_Type_t prvTestWaitCondition( ثابت Event_بیتها_t ux_Current_Event_بیتها,
                                        ثابت Event_بیتها_t ux_بیتها_مقصد_Wait_For,
                                        ثابت پایه_Type_t x_Wait_For_All_بیتها ) PRIVILEGED_FUNCTION;

/*-----------------------------------------------------------*/

#إذا ( config_SUPPORT_STATIC_تخصیص == 1 )

    Event_گروه_Handle_t x_Event_گروه_Create_Static( Static_Event_گروه_t * px_Event_گروه_میانگیر )
    {
        Event_گروه_t * px_Event_بیتها;

        /* A StaticEventGroup_t object must be provided. */
        configASSERT( px_Event_گروه_میانگیر );

        #إذا ( configASSERT_DEFINED == 1 )
        {
            /* Sanity check that the size of the structure used to declare a
             * variable of type StaticEventGroup_t equals the size of the real
             * event group structure. */
            متطاير size_t x_اندازه = حجم( Static_Event_گروه_t );
            configASSERT( x_اندازه == حجم( Event_گروه_t ) );
            ( فراغ ) x_اندازه; /* Prevent unused variable warning when configASSERT() is not used. */
        } /*lint !e529 xSize is referenced if configASSERT() is defined. */
        #پایان_اگر /* configASSERT_DEFINED */

        /* The user has provided a statically allocated event group - use it. */
        px_Event_بیتها = ( Event_گروه_t * ) px_Event_گروه_میانگیر; /*lint !e740 !e9087 EventGroup_t and StaticEventGroup_t are deliberately aliased for data hiding purposes and guaranteed to have the same size and alignment requirement - checked by configASSERT(). */

        إذا( px_Event_بیتها != عدم )
        {
            px_Event_بیتها->ux_Event_بیتها = 0;
            vListInitialise( &( px_Event_بیتها->x_Tasks_انتظار_For_بیتها ) );

            #إذا ( config_SUPPORT_DYNAMIC_تخصیص == 1 )
            {
                /* Both static and dynamic allocation can be used, so note that
                 * this event group was created statically in case the event group
                 * is later deleted. */
                px_Event_بیتها->ucStaticallyAllocated = pdTRUE;
            }
            #پایان_اگر /* configSUPPORT_DYNAMIC_ALLOCATION */

            /* Initialize the event group's spinlock. */
            پورت_MUX_INITIALIZE( &px_Event_بیتها->x_Event_گروه_قفل );

            trace_EVENT_گروه_CREATE( px_Event_بیتها );
        }
        وإلا
        {
            /* xEventGroupCreateStatic should only ever be called with
             * pxEventGroupBuffer pointing to a pre-allocated (compile time
             * allocated) StaticEventGroup_t variable. */
            trace_EVENT_گروه_CREATE_FAILED();
        }

        أرجع px_Event_بیتها;
    }

#پایان_اگر /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

#إذا ( config_SUPPORT_DYNAMIC_تخصیص == 1 )

    Event_گروه_Handle_t x_Event_گروه_Create( فراغ )
    {
        Event_گروه_t * px_Event_بیتها;

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
        px_Event_بیتها = ( Event_گروه_t * ) pv_پورت_Malloc( حجم( Event_گروه_t ) ); /*lint !e9087 !e9079 see comment above. */

        إذا( px_Event_بیتها != عدم )
        {
            px_Event_بیتها->ux_Event_بیتها = 0;
            vListInitialise( &( px_Event_بیتها->x_Tasks_انتظار_For_بیتها ) );

            #إذا ( config_SUPPORT_STATIC_تخصیص == 1 )
            {
                /* Both static and dynamic allocation can be used, so note this
                 * event group was allocated statically in case the event group is
                 * later deleted. */
                px_Event_بیتها->ucStaticallyAllocated = pdFALSE;
            }
            #پایان_اگر /* configSUPPORT_STATIC_ALLOCATION */

            /* Initialize the event group's spinlock. */
            پورت_MUX_INITIALIZE( &px_Event_بیتها->x_Event_گروه_قفل );

            trace_EVENT_گروه_CREATE( px_Event_بیتها );
        }
        وإلا
        {
            trace_EVENT_گروه_CREATE_FAILED(); /*lint !e9063 Else branch only exists to allow tracing and does not generate code if trace macros are not defined. */
        }

        أرجع px_Event_بیتها;
    }

#پایان_اگر /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

Event_بیتها_t x_Event_گروه_Sync( Event_گروه_Handle_t x_Event_گروه,
                             ثابت Event_بیتها_t ux_بیتها_مقصد_مجموعه,
                             ثابت Event_بیتها_t ux_بیتها_مقصد_Wait_For,
                             TickType_t x_Ticks_مقصد_Wait )
{
    Event_بیتها_t ux_Original_بیت_مقدار, uxReturn;
    Event_گروه_t * px_Event_بیتها = x_Event_گروه;
    پایه_Type_t xAlreadyYielded;
    پایه_Type_t xTimeoutOccurred = pdFALSE;

    configASSERT( ( ux_بیتها_مقصد_Wait_For & event_EVENT_بیتها_CONTROL_بایتها ) == 0 );
    configASSERT( ux_بیتها_مقصد_Wait_For != 0 );
    #إذا ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_مقصد_Wait != 0 ) ) );
    }
    #پایان_اگر

    prvENTER_CRITICAL_OR_SUSPEND_ALL( &( px_Event_بیتها->x_Event_گروه_قفل ) );
    {
        ux_Original_بیت_مقدار = px_Event_بیتها->ux_Event_بیتها;

        ( فراغ ) x_Event_گروه_مجموعه_بیتها( x_Event_گروه, ux_بیتها_مقصد_مجموعه );

        إذا( ( ( ux_Original_بیت_مقدار | ux_بیتها_مقصد_مجموعه ) & ux_بیتها_مقصد_Wait_For ) == ux_بیتها_مقصد_Wait_For )
        {
            /* All the rendezvous bits are now set - no need to block. */
            uxReturn = ( ux_Original_بیت_مقدار | ux_بیتها_مقصد_مجموعه );

            /* Rendezvous always clear the bits.  They will have been cleared
             * already unless this is the only task in the rendezvous. */
            px_Event_بیتها->ux_Event_بیتها &= ~ux_بیتها_مقصد_Wait_For;

            x_Ticks_مقصد_Wait = 0;
        }
        وإلا
        {
            إذا( x_Ticks_مقصد_Wait != ( TickType_t ) 0 )
            {
                trace_EVENT_گروه_SYNC_بلوک( x_Event_گروه, ux_بیتها_مقصد_مجموعه, ux_بیتها_مقصد_Wait_For );

                /* Store the bits that the calling task is waiting for in the
                 * task's event list item so the kernel knows when a match is
                 * found.  Then enter the blocked state. */
                v_Task_قراربده_On_Unordered_Event_List( &( px_Event_بیتها->x_Tasks_انتظار_For_بیتها ), ( ux_بیتها_مقصد_Wait_For | event_CLEAR_EVENTS_ON_EXIT_بیت | event_WAIT_FOR_ALL_بیتها ), x_Ticks_مقصد_Wait );

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
                uxReturn = px_Event_بیتها->ux_Event_بیتها;
                xTimeoutOccurred = pdTRUE;
            }
        }
    }
    xAlreadyYielded = prvEXIT_CRITICAL_OR_RESUME_ALL( &( px_Event_بیتها->x_Event_گروه_قفل ) );

    إذا( x_Ticks_مقصد_Wait != ( TickType_t ) 0 )
    {
        إذا( xAlreadyYielded == pdFALSE )
        {
            پورت_YIELD_WITHIN_API();
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }

        /* The task blocked to wait for its required bits to be set - at this
         * point either the required bits were set or the block time expired.  If
         * the required bits were set they will have been stored in the task's
         * event list item, and they should now be retrieved then cleared. */
        uxReturn = ux_Task_Reset_Event_قلم_مقدار();

        إذا( ( uxReturn & event_UNBLOCKED_DUE_مقصد_بیت_مجموعه ) == ( Event_بیتها_t ) 0 )
        {
            /* The task timed out, just return the current event bit value. */
            taskENTER_CRITICAL( &( px_Event_بیتها->x_Event_گروه_قفل ) );
            {
                uxReturn = px_Event_بیتها->ux_Event_بیتها;

                /* Although the task got here because it timed out before the
                 * bits it was waiting for were set, it is possible that since it
                 * unblocked another task has set the bits.  If this is the case
                 * then it needs to clear the bits before exiting. */
                إذا( ( uxReturn & ux_بیتها_مقصد_Wait_For ) == ux_بیتها_مقصد_Wait_For )
                {
                    px_Event_بیتها->ux_Event_بیتها &= ~ux_بیتها_مقصد_Wait_For;
                }
                وإلا
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            taskEXIT_CRITICAL( &( px_Event_بیتها->x_Event_گروه_قفل ) );

            xTimeoutOccurred = pdTRUE;
        }
        وإلا
        {
            /* The task unblocked because the bits were set. */
        }

        /* Control bits might be set as the task had blocked should not be
         * returned. */
        uxReturn &= ~event_EVENT_بیتها_CONTROL_بایتها;
    }

    trace_EVENT_گروه_SYNC_انتها( x_Event_گروه, ux_بیتها_مقصد_مجموعه, ux_بیتها_مقصد_Wait_For, xTimeoutOccurred );

    /* Prevent compiler warnings when trace macros are not used. */
    ( فراغ ) xTimeoutOccurred;

    أرجع uxReturn;
}
/*-----------------------------------------------------------*/

Event_بیتها_t x_Event_گروه_Wait_بیتها( Event_گروه_Handle_t x_Event_گروه,
                                 ثابت Event_بیتها_t ux_بیتها_مقصد_Wait_For,
                                 ثابت پایه_Type_t xClearOnExit,
                                 ثابت پایه_Type_t x_Wait_For_All_بیتها,
                                 TickType_t x_Ticks_مقصد_Wait )
{
    Event_گروه_t * px_Event_بیتها = x_Event_گروه;
    Event_بیتها_t uxReturn, ux_Control_بیتها = 0;
    پایه_Type_t xWaitConditionMet, xAlreadyYielded;
    پایه_Type_t xTimeoutOccurred = pdFALSE;

    /* Check the user is not attempting to wait on the bits used by the kernel
     * itself, and that at least one bit is being requested. */
    configASSERT( x_Event_گروه );
    configASSERT( ( ux_بیتها_مقصد_Wait_For & event_EVENT_بیتها_CONTROL_بایتها ) == 0 );
    configASSERT( ux_بیتها_مقصد_Wait_For != 0 );
    #إذا ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_مقصد_Wait != 0 ) ) );
    }
    #پایان_اگر

    prvENTER_CRITICAL_OR_SUSPEND_ALL( &( px_Event_بیتها->x_Event_گروه_قفل ) );
    {
        ثابت Event_بیتها_t ux_Current_Event_بیتها = px_Event_بیتها->ux_Event_بیتها;

        /* Check to see if the wait condition is already met or not. */
        xWaitConditionMet = prvTestWaitCondition( ux_Current_Event_بیتها, ux_بیتها_مقصد_Wait_For, x_Wait_For_All_بیتها );

        إذا( xWaitConditionMet != pdFALSE )
        {
            /* The wait condition has already been met so there is no need to
             * block. */
            uxReturn = ux_Current_Event_بیتها;
            x_Ticks_مقصد_Wait = ( TickType_t ) 0;

            /* Clear the wait bits if requested to do so. */
            إذا( xClearOnExit != pdFALSE )
            {
                px_Event_بیتها->ux_Event_بیتها &= ~ux_بیتها_مقصد_Wait_For;
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        وإلا إذا( x_Ticks_مقصد_Wait == ( TickType_t ) 0 )
        {
            /* The wait condition has not been met, but no block time was
             * specified, so just return the current value. */
            uxReturn = ux_Current_Event_بیتها;
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
                ux_Control_بیتها |= event_CLEAR_EVENTS_ON_EXIT_بیت;
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }

            إذا( x_Wait_For_All_بیتها != pdFALSE )
            {
                ux_Control_بیتها |= event_WAIT_FOR_ALL_بیتها;
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }

            /* Store the bits that the calling task is waiting for in the
             * task's event list item so the kernel knows when a match is
             * found.  Then enter the blocked state. */
            v_Task_قراربده_On_Unordered_Event_List( &( px_Event_بیتها->x_Tasks_انتظار_For_بیتها ), ( ux_بیتها_مقصد_Wait_For | ux_Control_بیتها ), x_Ticks_مقصد_Wait );

            /* This is obsolete as it will get set after the task unblocks, but
             * some compilers mistakenly generate a warning about the variable
             * being returned without being set if it is not done. */
            uxReturn = 0;

            trace_EVENT_گروه_WAIT_بیتها_بلوک( x_Event_گروه, ux_بیتها_مقصد_Wait_For );
        }
    }
    xAlreadyYielded = prvEXIT_CRITICAL_OR_RESUME_ALL( &( px_Event_بیتها->x_Event_گروه_قفل ) );

    إذا( x_Ticks_مقصد_Wait != ( TickType_t ) 0 )
    {
        إذا( xAlreadyYielded == pdFALSE )
        {
            پورت_YIELD_WITHIN_API();
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }

        /* The task blocked to wait for its required bits to be set - at this
         * point either the required bits were set or the block time expired.  If
         * the required bits were set they will have been stored in the task's
         * event list item, and they should now be retrieved then cleared. */
        uxReturn = ux_Task_Reset_Event_قلم_مقدار();

        إذا( ( uxReturn & event_UNBLOCKED_DUE_مقصد_بیت_مجموعه ) == ( Event_بیتها_t ) 0 )
        {
            taskENTER_CRITICAL( &( px_Event_بیتها->x_Event_گروه_قفل ) );
            {
                /* The task timed out, just return the current event bit value. */
                uxReturn = px_Event_بیتها->ux_Event_بیتها;

                /* It is possible that the event bits were updated between this
                 * task leaving the Blocked state and running again. */
                إذا( prvTestWaitCondition( uxReturn, ux_بیتها_مقصد_Wait_For, x_Wait_For_All_بیتها ) != pdFALSE )
                {
                    إذا( xClearOnExit != pdFALSE )
                    {
                        px_Event_بیتها->ux_Event_بیتها &= ~ux_بیتها_مقصد_Wait_For;
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
            taskEXIT_CRITICAL( &( px_Event_بیتها->x_Event_گروه_قفل ) );
        }
        وإلا
        {
            /* The task unblocked because the bits were set. */
        }

        /* The task blocked so control bits may have been set. */
        uxReturn &= ~event_EVENT_بیتها_CONTROL_بایتها;
    }

    trace_EVENT_گروه_WAIT_بیتها_انتها( x_Event_گروه, ux_بیتها_مقصد_Wait_For, xTimeoutOccurred );

    /* Prevent compiler warnings when trace macros are not used. */
    ( فراغ ) xTimeoutOccurred;

    أرجع uxReturn;
}
/*-----------------------------------------------------------*/

Event_بیتها_t x_Event_گروه_Clear_بیتها( Event_گروه_Handle_t x_Event_گروه,
                                  ثابت Event_بیتها_t ux_بیتها_مقصد_Clear )
{
    Event_گروه_t * px_Event_بیتها = x_Event_گروه;
    Event_بیتها_t uxReturn;

    /* Check the user is not attempting to clear the bits used by the kernel
     * itself. */
    configASSERT( x_Event_گروه );
    configASSERT( ( ux_بیتها_مقصد_Clear & event_EVENT_بیتها_CONTROL_بایتها ) == 0 );

    taskENTER_CRITICAL( &( px_Event_بیتها->x_Event_گروه_قفل ) );
    {
        trace_EVENT_گروه_CLEAR_بیتها( x_Event_گروه, ux_بیتها_مقصد_Clear );

        /* The value returned is the event group value prior to the bits being
         * cleared. */
        uxReturn = px_Event_بیتها->ux_Event_بیتها;

        /* Clear the bits. */
        px_Event_بیتها->ux_Event_بیتها &= ~ux_بیتها_مقصد_Clear;
    }
    taskEXIT_CRITICAL( &( px_Event_بیتها->x_Event_گروه_قفل ) );

    أرجع uxReturn;
}
/*-----------------------------------------------------------*/

#إذا ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) )

    پایه_Type_t x_Event_گروه_Clear_بیتها_From_ISR( Event_گروه_Handle_t x_Event_گروه,
                                            ثابت Event_بیتها_t ux_بیتها_مقصد_Clear )
    {
        پایه_Type_t xReturn;

        trace_EVENT_گروه_CLEAR_بیتها_FROM_ISR( x_Event_گروه, ux_بیتها_مقصد_Clear );
        xReturn = xTimerPendFunctionCallFromISR( v_Event_گروه_Clear_بیتها_Callback, ( فراغ * ) x_Event_گروه, ( uint32_t ) ux_بیتها_مقصد_Clear, عدم ); /*lint !e9087 Can't avoid cast to void* as a generic callback function not specific to this use case. Callback casts back to original type so safe. */

        أرجع xReturn;
    }

#پایان_اگر /* if ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) ) */
/*-----------------------------------------------------------*/

Event_بیتها_t x_Event_گروه_Get_بیتها_From_ISR( Event_گروه_Handle_t x_Event_گروه )
{
    UBaseType_t ux_Saved_وقفه_وضعیت;
    Event_گروه_t ثابت * ثابت px_Event_بیتها = x_Event_گروه;
    Event_بیتها_t uxReturn;

    prv_ENTER_CRITICAL_OR_ماسک_ISR( ( پورت_MUX_TYPE * ) &( px_Event_بیتها->x_Event_گروه_قفل ), ux_Saved_وقفه_وضعیت );
    {
        uxReturn = px_Event_بیتها->ux_Event_بیتها;
    }
    prvEXIT_CRITICAL_OR_UNMASK_ISR( ( پورت_MUX_TYPE * ) &( px_Event_بیتها->x_Event_گروه_قفل ), ux_Saved_وقفه_وضعیت );

    أرجع uxReturn;
} /*lint !e818 EventGroupHandle_t is a typedef used in other functions to so can't be pointer to const. */
/*-----------------------------------------------------------*/

Event_بیتها_t x_Event_گروه_مجموعه_بیتها( Event_گروه_Handle_t x_Event_گروه,
                                ثابت Event_بیتها_t ux_بیتها_مقصد_مجموعه )
{
    List_قلم_t * px_List_قلم;
    List_قلم_t * px_بعدی;
    List_قلم_t ثابت * px_List_انتها;
    List_t ثابت * pxList;
    Event_بیتها_t ux_بیتها_مقصد_Clear = 0, ux_بیتها_Waited_For, ux_Control_بیتها;
    Event_گروه_t * px_Event_بیتها = x_Event_گروه;
    پایه_Type_t x_تطابق_یافت = pdFALSE;

    /* Check the user is not attempting to set the bits used by the kernel
     * itself. */
    configASSERT( x_Event_گروه );
    configASSERT( ( ux_بیتها_مقصد_مجموعه & event_EVENT_بیتها_CONTROL_بایتها ) == 0 );

    pxList = &( px_Event_بیتها->x_Tasks_انتظار_For_بیتها );
    px_List_انتها = list_GET_انتها_MARKER( pxList ); /*lint !e826 !e740 !e9087 The mini list structure is used as the list end to save RAM.  This is checked and valid. */

    prvENTER_CRITICAL_OR_SUSPEND_ALL( &( px_Event_بیتها->x_Event_گروه_قفل ) );
    #إذا ( config_عدد_از_CORES > 1 )

        /* We are about to traverse a task list which is a kernel data structure.
         * Thus we need to call prvTakeKernelLock() to take the kernel lock. */
        prv_Take_Kernel_قفل();
    #پایان_اگر /* configNUMBER_OF_CORES > 1 */
    {
        trace_EVENT_گروه_مجموعه_بیتها( x_Event_گروه, ux_بیتها_مقصد_مجموعه );

        px_List_قلم = list_GET_سر_مدخل( pxList );

        /* Set the bits. */
        px_Event_بیتها->ux_Event_بیتها |= ux_بیتها_مقصد_مجموعه;

        /* See if the new bit value should unblock any tasks. */
        طالما( px_List_قلم != px_List_انتها )
        {
            px_بعدی = list_GET_بعدی( px_List_قلم );
            ux_بیتها_Waited_For = list_GET_LIST_قلم_مقدار( px_List_قلم );
            x_تطابق_یافت = pdFALSE;

            /* Split the bits waited for from the control bits. */
            ux_Control_بیتها = ux_بیتها_Waited_For & event_EVENT_بیتها_CONTROL_بایتها;
            ux_بیتها_Waited_For &= ~event_EVENT_بیتها_CONTROL_بایتها;

            إذا( ( ux_Control_بیتها & event_WAIT_FOR_ALL_بیتها ) == ( Event_بیتها_t ) 0 )
            {
                /* Just looking for single bit being set. */
                إذا( ( ux_بیتها_Waited_For & px_Event_بیتها->ux_Event_بیتها ) != ( Event_بیتها_t ) 0 )
                {
                    x_تطابق_یافت = pdTRUE;
                }
                وإلا
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            وإلا إذا( ( ux_بیتها_Waited_For & px_Event_بیتها->ux_Event_بیتها ) == ux_بیتها_Waited_For )
            {
                /* All bits are set. */
                x_تطابق_یافت = pdTRUE;
            }
            وإلا
            {
                /* Need all bits to be set, but not all the bits were set. */
            }

            إذا( x_تطابق_یافت != pdFALSE )
            {
                /* The bits match.  Should the bits be cleared on exit? */
                إذا( ( ux_Control_بیتها & event_CLEAR_EVENTS_ON_EXIT_بیت ) != ( Event_بیتها_t ) 0 )
                {
                    ux_بیتها_مقصد_Clear |= ux_بیتها_Waited_For;
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
                vTaskRemoveFromUnorderedEventList( px_List_قلم, px_Event_بیتها->ux_Event_بیتها | event_UNBLOCKED_DUE_مقصد_بیت_مجموعه );
            }

            /* Move onto the next list item.  Note pxListItem->pxNext is not
             * used here as the list item may have been removed from the event list
             * and inserted into the ready/pending reading list. */
            px_List_قلم = px_بعدی;
        }

        /* Clear any bits that matched when the eventCLEAR_EVENTS_ON_EXIT_BIT
         * bit was set in the control word. */
        px_Event_بیتها->ux_Event_بیتها &= ~ux_بیتها_مقصد_Clear;
    }
    #إذا ( config_عدد_از_CORES > 1 )
        /* Release the previously taken kernel lock. */
        prv_Release_Kernel_قفل();
    #پایان_اگر /* configNUMBER_OF_CORES > 1 */
    ( فراغ ) prvEXIT_CRITICAL_OR_RESUME_ALL( &( px_Event_بیتها->x_Event_گروه_قفل ) );


    أرجع px_Event_بیتها->ux_Event_بیتها;
}
/*-----------------------------------------------------------*/

فراغ v_Event_گروه_Delete( Event_گروه_Handle_t x_Event_گروه )
{
    Event_گروه_t * px_Event_بیتها = x_Event_گروه;
    ثابت List_t * px_Tasks_انتظار_For_بیتها;

    configASSERT( px_Event_بیتها );

    px_Tasks_انتظار_For_بیتها = &( px_Event_بیتها->x_Tasks_انتظار_For_بیتها );

    prvENTER_CRITICAL_OR_SUSPEND_ALL( &( px_Event_بیتها->x_Event_گروه_قفل ) );
    #إذا ( config_عدد_از_CORES > 1 )

        /* We are about to traverse a task list which is a kernel data structure.
         * Thus we need to call prvTakeKernelLock() to take the kernel lock. */
        prv_Take_Kernel_قفل();
    #پایان_اگر /* configNUMBER_OF_CORES > 1 */
    {
        trace_EVENT_گروه_DELETE( x_Event_گروه );

        طالما( list_CURRENT_LIST_درازی( px_Tasks_انتظار_For_بیتها ) > ( UBaseType_t ) 0 )
        {
            /* Unblock the task, returning 0 as the event list is being deleted
             * and cannot therefore have any bits set. */
            configASSERT( px_Tasks_انتظار_For_بیتها->x_List_انتها.px_بعدی != ( ثابت List_قلم_t * ) &( px_Tasks_انتظار_For_بیتها->x_List_انتها ) );
            vTaskRemoveFromUnorderedEventList( px_Tasks_انتظار_For_بیتها->x_List_انتها.px_بعدی, event_UNBLOCKED_DUE_مقصد_بیت_مجموعه );
        }
    }
    #إذا ( config_عدد_از_CORES > 1 )
        /* Release the previously taken kernel lock. */
        prv_Release_Kernel_قفل();
    #پایان_اگر /* configNUMBER_OF_CORES > 1 */
    prvEXIT_CRITICAL_OR_RESUME_ALL( &( px_Event_بیتها->x_Event_گروه_قفل ) );

    #إذا ( ( config_SUPPORT_DYNAMIC_تخصیص == 1 ) && ( config_SUPPORT_STATIC_تخصیص == 0 ) )
    {
        /* The event group can only have been allocated dynamically - free
         * it again. */
        v_پورت_Free( px_Event_بیتها );
    }
    #وإلاإذا ( ( config_SUPPORT_DYNAMIC_تخصیص == 1 ) && ( config_SUPPORT_STATIC_تخصیص == 1 ) )
    {
        /* The event group could have been allocated statically or
         * dynamically, so check before attempting to free the memory. */
        إذا( px_Event_بیتها->ucStaticallyAllocated == ( uint8_t ) pdFALSE )
        {
            v_پورت_Free( px_Event_بیتها );
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    #پایان_اگر /* configSUPPORT_DYNAMIC_ALLOCATION */
}
/*-----------------------------------------------------------*/

#إذا ( config_SUPPORT_STATIC_تخصیص == 1 )
    پایه_Type_t x_Event_گروه_Get_Static_میانگیر( Event_گروه_Handle_t x_Event_گروه,
                                           Static_Event_گروه_t ** ppx_Event_گروه_میانگیر )
    {
        پایه_Type_t xReturn;
        Event_گروه_t * px_Event_بیتها = x_Event_گروه;

        configASSERT( px_Event_بیتها );
        configASSERT( ppx_Event_گروه_میانگیر );

        #إذا ( config_SUPPORT_DYNAMIC_تخصیص == 1 )
        {
            /* Check if the event group was statically allocated. */
            إذا( px_Event_بیتها->ucStaticallyAllocated == ( uint8_t ) pdTRUE )
            {
                *ppx_Event_گروه_میانگیر = ( Static_Event_گروه_t * ) px_Event_بیتها;
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
            *ppx_Event_گروه_میانگیر = ( Static_Event_گروه_t * ) px_Event_بیتها;
            xReturn = pdTRUE;
        }
        #پایان_اگر /* configSUPPORT_DYNAMIC_ALLOCATION */

        أرجع xReturn;
    }
#پایان_اگر /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

/* For internal use only - execute a 'set bits' command that was pended from
 * an interrupt. */
فراغ v_Event_گروه_مجموعه_بیتها_Callback( فراغ * pv_Event_گروه,
                                 ثابت uint32_t ul_بیتها_مقصد_مجموعه )
{
    ( فراغ ) x_Event_گروه_مجموعه_بیتها( pv_Event_گروه, ( Event_بیتها_t ) ul_بیتها_مقصد_مجموعه ); /*lint !e9079 Can't avoid cast to void* as a generic timer callback prototype. Callback casts back to original type so safe. */
}
/*-----------------------------------------------------------*/

/* For internal use only - execute a 'clear bits' command that was pended from
 * an interrupt. */
فراغ v_Event_گروه_Clear_بیتها_Callback( فراغ * pv_Event_گروه,
                                   ثابت uint32_t ul_بیتها_مقصد_Clear )
{
    ( فراغ ) x_Event_گروه_Clear_بیتها( pv_Event_گروه, ( Event_بیتها_t ) ul_بیتها_مقصد_Clear ); /*lint !e9079 Can't avoid cast to void* as a generic timer callback prototype. Callback casts back to original type so safe. */
}
/*-----------------------------------------------------------*/

ساكن پایه_Type_t prvTestWaitCondition( ثابت Event_بیتها_t ux_Current_Event_بیتها,
                                        ثابت Event_بیتها_t ux_بیتها_مقصد_Wait_For,
                                        ثابت پایه_Type_t x_Wait_For_All_بیتها )
{
    پایه_Type_t xWaitConditionMet = pdFALSE;

    إذا( x_Wait_For_All_بیتها == pdFALSE )
    {
        /* Task only has to wait for one bit within uxBitsToWaitFor to be
         * set.  Is one already set? */
        إذا( ( ux_Current_Event_بیتها & ux_بیتها_مقصد_Wait_For ) != ( Event_بیتها_t ) 0 )
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
        إذا( ( ux_Current_Event_بیتها & ux_بیتها_مقصد_Wait_For ) == ux_بیتها_مقصد_Wait_For )
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

    پایه_Type_t x_Event_گروه_مجموعه_بیتها_From_ISR( Event_گروه_Handle_t x_Event_گروه,
                                          ثابت Event_بیتها_t ux_بیتها_مقصد_مجموعه,
                                          پایه_Type_t * px_Higher_اولویت_Task_Woken )
    {
        پایه_Type_t xReturn;

        trace_EVENT_گروه_مجموعه_بیتها_FROM_ISR( x_Event_گروه, ux_بیتها_مقصد_مجموعه );
        xReturn = xTimerPendFunctionCallFromISR( v_Event_گروه_مجموعه_بیتها_Callback, ( فراغ * ) x_Event_گروه, ( uint32_t ) ux_بیتها_مقصد_مجموعه, px_Higher_اولویت_Task_Woken ); /*lint !e9087 Can't avoid cast to void* as a generic callback function not specific to this use case. Callback casts back to original type so safe. */

        أرجع xReturn;
    }

#پایان_اگر /* if ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) ) */
/*-----------------------------------------------------------*/

#إذا ( configUSE_TRACE_FACILITY == 1 )

    UBaseType_t ux_Event_گروه_Get_عدد( فراغ * x_Event_گروه )
    {
        UBaseType_t xReturn;
        Event_گروه_t ثابت * px_Event_بیتها = ( Event_گروه_t * ) x_Event_گروه; /*lint !e9087 !e9079 EventGroupHandle_t is a pointer to an EventGroup_t, but EventGroupHandle_t is kept opaque outside of this file for data hiding purposes. */

        إذا( x_Event_گروه == عدم )
        {
            xReturn = 0;
        }
        وإلا
        {
            xReturn = px_Event_بیتها->ux_Event_گروه_عدد;
        }

        أرجع xReturn;
    }

#پایان_اگر /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#إذا ( configUSE_TRACE_FACILITY == 1 )

    فراغ v_Event_گروه_مجموعه_عدد( فراغ * x_Event_گروه,
                               UBaseType_t ux_Event_گروه_عدد )
    {
        ( ( Event_گروه_t * ) x_Event_گروه )->ux_Event_گروه_عدد = ux_Event_گروه_عدد; /*lint !e9087 !e9079 EventGroupHandle_t is a pointer to an EventGroup_t, but EventGroupHandle_t is kept opaque outside of this file for data hiding purposes. */
    }

#پایان_اگر /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/
