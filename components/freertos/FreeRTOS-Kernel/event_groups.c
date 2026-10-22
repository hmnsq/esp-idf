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
#شامل <stdlib.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#تعریف_کرو MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* FreeRTOS includes. */
#شامل "FreeRTOS.h"
#شامل "task.h"
#شامل "timers.h"
#شامل "event_groups.h"
/* Include private IDF API additions for critical thread safety macros */
#شامل "esp_private/freertos_idf_additions_priv.h"

/* Lint e961, e750 and e9021 are suppressed as a MISRA exception justified
 * because the MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#تعریف_منسوخ MPU_WRAPPERS_INCLUDED_FROM_API_FILE /*lint !e961 !e750 !e9021 See comment above. */

/* The following bit fields convey control information in a task's event list
 * item value.  It is important they don't clash with the
 * taskEVENT_LIST_ITEM_VALUE_IN_USE definition. */
#اگر config_USE_16_ثنائی_ہندسہ_TICKS == 1
    #تعریف_کرو event_CLEAR_EVENTS_ON_EXIT_ثنائی_ہندسہ    0x0100U
    #تعریف_کرو event_UNBLOCKED_DUE_مقصد_ثنائی_ہندسہ_سیٹ    0x0200U
    #تعریف_کرو event_WAIT_FOR_ALL_بٹ           0x0400U
    #تعریف_کرو event_EVENT_بٹ_CONTROL_بائٹیں    0xff00U
#ورنہ
    #تعریف_کرو event_CLEAR_EVENTS_ON_EXIT_ثنائی_ہندسہ    0x01000000UL
    #تعریف_کرو event_UNBLOCKED_DUE_مقصد_ثنائی_ہندسہ_سیٹ    0x02000000UL
    #تعریف_کرو event_WAIT_FOR_ALL_بٹ           0x04000000UL
    #تعریف_کرو event_EVENT_بٹ_CONTROL_بائٹیں    0xff000000UL
#اگر_ختم

قسمتعریف ساخت Event_گروہ_Def_t
{
    Event_بٹ_t ux_Event_بٹ;
    List_t x_Tasks_انتظار_For_بٹ; /*< List of tasks waiting for a bit to be set. */

    #اگر ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_Event_گروہ_نمبر;
    #اگر_ختم

    #اگر ( ( config_SUPPORT_STATIC_تخصیص == 1 ) && ( config_SUPPORT_DYNAMIC_تخصیص == 1 ) )
        uint8_t ucStaticallyAllocated; /*< Set to pdTRUE if the event group is statically allocated to ensure no attempt is made to free the memory. */
    #اگر_ختم

    پورٹ_MUX_TYPE x_Event_گروہ_تالا; /* Spinlock required for SMP critical sections */
} Event_گروہ_t;

/*-----------------------------------------------------------*/

/*
 * Test the bits set in uxCurrentEventBits to see if the wait condition is met.
 * The wait condition is defined by xWaitForAllBits.  If xWaitForAllBits is
 * pdTRUE then the wait condition is met if all the bits set in uxBitsToWaitFor
 * are also set in uxCurrentEventBits.  If xWaitForAllBits is pdFALSE then the
 * wait condition is met if any of the bits set in uxBitsToWait for are also set
 * in uxCurrentEventBits.
 */
ساکن بنیاد_Type_t prvTestWaitCondition( ثابت Event_بٹ_t ux_Current_Event_بٹ,
                                        ثابت Event_بٹ_t ux_بٹ_مقصد_Wait_For,
                                        ثابت بنیاد_Type_t x_Wait_For_All_بٹ ) PRIVILEGED_FUNCTION;

/*-----------------------------------------------------------*/

#اگر ( config_SUPPORT_STATIC_تخصیص == 1 )

    Event_گروہ_Handle_t x_Event_گروہ_Create_Static( Static_Event_گروہ_t * px_Event_گروہ_بفر )
    {
        Event_گروہ_t * px_Event_بٹ;

        /* A StaticEventGroup_t object must be provided. */
        configASSERT( px_Event_گروہ_بفر );

        #اگر ( configASSERT_DEFINED == 1 )
        {
            /* Sanity check that the size of the structure used to declare a
             * variable of type StaticEventGroup_t equals the size of the real
             * event group structure. */
            متلون size_t x_سائز = حجم( Static_Event_گروہ_t );
            configASSERT( x_سائز == حجم( Event_گروہ_t ) );
            ( خالی ) x_سائز; /* Prevent unused variable warning when configASSERT() is not used. */
        } /*lint !e529 xSize is referenced if configASSERT() is defined. */
        #اگر_ختم /* configASSERT_DEFINED */

        /* The user has provided a statically allocated event group - use it. */
        px_Event_بٹ = ( Event_گروہ_t * ) px_Event_گروہ_بفر; /*lint !e740 !e9087 EventGroup_t and StaticEventGroup_t are deliberately aliased for data hiding purposes and guaranteed to have the same size and alignment requirement - checked by configASSERT(). */

        اگر( px_Event_بٹ != تہی )
        {
            px_Event_بٹ->ux_Event_بٹ = 0;
            vListInitialise( &( px_Event_بٹ->x_Tasks_انتظار_For_بٹ ) );

            #اگر ( config_SUPPORT_DYNAMIC_تخصیص == 1 )
            {
                /* Both static and dynamic allocation can be used, so note that
                 * this event group was created statically in case the event group
                 * is later deleted. */
                px_Event_بٹ->ucStaticallyAllocated = pdTRUE;
            }
            #اگر_ختم /* configSUPPORT_DYNAMIC_ALLOCATION */

            /* Initialize the event group's spinlock. */
            پورٹ_MUX_INITIALIZE( &px_Event_بٹ->x_Event_گروہ_تالا );

            trace_EVENT_گروہ_CREATE( px_Event_بٹ );
        }
        ورنہ
        {
            /* xEventGroupCreateStatic should only ever be called with
             * pxEventGroupBuffer pointing to a pre-allocated (compile time
             * allocated) StaticEventGroup_t variable. */
            trace_EVENT_گروہ_CREATE_FAILED();
        }

        واپس px_Event_بٹ;
    }

#اگر_ختم /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

#اگر ( config_SUPPORT_DYNAMIC_تخصیص == 1 )

    Event_گروہ_Handle_t x_Event_گروہ_Create( خالی )
    {
        Event_گروہ_t * px_Event_بٹ;

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
        px_Event_بٹ = ( Event_گروہ_t * ) pv_پورٹ_Malloc( حجم( Event_گروہ_t ) ); /*lint !e9087 !e9079 see comment above. */

        اگر( px_Event_بٹ != تہی )
        {
            px_Event_بٹ->ux_Event_بٹ = 0;
            vListInitialise( &( px_Event_بٹ->x_Tasks_انتظار_For_بٹ ) );

            #اگر ( config_SUPPORT_STATIC_تخصیص == 1 )
            {
                /* Both static and dynamic allocation can be used, so note this
                 * event group was allocated statically in case the event group is
                 * later deleted. */
                px_Event_بٹ->ucStaticallyAllocated = pdFALSE;
            }
            #اگر_ختم /* configSUPPORT_STATIC_ALLOCATION */

            /* Initialize the event group's spinlock. */
            پورٹ_MUX_INITIALIZE( &px_Event_بٹ->x_Event_گروہ_تالا );

            trace_EVENT_گروہ_CREATE( px_Event_بٹ );
        }
        ورنہ
        {
            trace_EVENT_گروہ_CREATE_FAILED(); /*lint !e9063 Else branch only exists to allow tracing and does not generate code if trace macros are not defined. */
        }

        واپس px_Event_بٹ;
    }

#اگر_ختم /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

Event_بٹ_t x_Event_گروہ_Sync( Event_گروہ_Handle_t x_Event_گروہ,
                             ثابت Event_بٹ_t ux_بٹ_مقصد_سیٹ,
                             ثابت Event_بٹ_t ux_بٹ_مقصد_Wait_For,
                             TickType_t x_Ticks_مقصد_Wait )
{
    Event_بٹ_t ux_Original_ثنائی_ہندسہ_قدر, uxReturn;
    Event_گروہ_t * px_Event_بٹ = x_Event_گروہ;
    بنیاد_Type_t xAlreadyYielded;
    بنیاد_Type_t xTimeoutOccurred = pdFALSE;

    configASSERT( ( ux_بٹ_مقصد_Wait_For & event_EVENT_بٹ_CONTROL_بائٹیں ) == 0 );
    configASSERT( ux_بٹ_مقصد_Wait_For != 0 );
    #اگر ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_مقصد_Wait != 0 ) ) );
    }
    #اگر_ختم

    prvENTER_CRITICAL_OR_SUSPEND_ALL( &( px_Event_بٹ->x_Event_گروہ_تالا ) );
    {
        ux_Original_ثنائی_ہندسہ_قدر = px_Event_بٹ->ux_Event_بٹ;

        ( خالی ) x_Event_گروہ_سیٹ_بٹ( x_Event_گروہ, ux_بٹ_مقصد_سیٹ );

        اگر( ( ( ux_Original_ثنائی_ہندسہ_قدر | ux_بٹ_مقصد_سیٹ ) & ux_بٹ_مقصد_Wait_For ) == ux_بٹ_مقصد_Wait_For )
        {
            /* All the rendezvous bits are now set - no need to block. */
            uxReturn = ( ux_Original_ثنائی_ہندسہ_قدر | ux_بٹ_مقصد_سیٹ );

            /* Rendezvous always clear the bits.  They will have been cleared
             * already unless this is the only task in the rendezvous. */
            px_Event_بٹ->ux_Event_بٹ &= ~ux_بٹ_مقصد_Wait_For;

            x_Ticks_مقصد_Wait = 0;
        }
        ورنہ
        {
            اگر( x_Ticks_مقصد_Wait != ( TickType_t ) 0 )
            {
                trace_EVENT_گروہ_SYNC_بلاک( x_Event_گروہ, ux_بٹ_مقصد_سیٹ, ux_بٹ_مقصد_Wait_For );

                /* Store the bits that the calling task is waiting for in the
                 * task's event list item so the kernel knows when a match is
                 * found.  Then enter the blocked state. */
                v_Task_رکھو_On_Unordered_Event_List( &( px_Event_بٹ->x_Tasks_انتظار_For_بٹ ), ( ux_بٹ_مقصد_Wait_For | event_CLEAR_EVENTS_ON_EXIT_ثنائی_ہندسہ | event_WAIT_FOR_ALL_بٹ ), x_Ticks_مقصد_Wait );

                /* This assignment is obsolete as uxReturn will get set after
                 * the task unblocks, but some compilers mistakenly generate a
                 * warning about uxReturn being returned without being set if the
                 * assignment is omitted. */
                uxReturn = 0;
            }
            ورنہ
            {
                /* The rendezvous bits were not set, but no block time was
                 * specified - just return the current event bit value. */
                uxReturn = px_Event_بٹ->ux_Event_بٹ;
                xTimeoutOccurred = pdTRUE;
            }
        }
    }
    xAlreadyYielded = prvEXIT_CRITICAL_OR_RESUME_ALL( &( px_Event_بٹ->x_Event_گروہ_تالا ) );

    اگر( x_Ticks_مقصد_Wait != ( TickType_t ) 0 )
    {
        اگر( xAlreadyYielded == pdFALSE )
        {
            پورٹ_YIELD_WITHIN_API();
        }
        ورنہ
        {
            mtCOVERAGE_TEST_MARKER();
        }

        /* The task blocked to wait for its required bits to be set - at this
         * point either the required bits were set or the block time expired.  If
         * the required bits were set they will have been stored in the task's
         * event list item, and they should now be retrieved then cleared. */
        uxReturn = ux_Task_Reset_Event_شے_قدر();

        اگر( ( uxReturn & event_UNBLOCKED_DUE_مقصد_ثنائی_ہندسہ_سیٹ ) == ( Event_بٹ_t ) 0 )
        {
            /* The task timed out, just return the current event bit value. */
            taskENTER_CRITICAL( &( px_Event_بٹ->x_Event_گروہ_تالا ) );
            {
                uxReturn = px_Event_بٹ->ux_Event_بٹ;

                /* Although the task got here because it timed out before the
                 * bits it was waiting for were set, it is possible that since it
                 * unblocked another task has set the bits.  If this is the case
                 * then it needs to clear the bits before exiting. */
                اگر( ( uxReturn & ux_بٹ_مقصد_Wait_For ) == ux_بٹ_مقصد_Wait_For )
                {
                    px_Event_بٹ->ux_Event_بٹ &= ~ux_بٹ_مقصد_Wait_For;
                }
                ورنہ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            taskEXIT_CRITICAL( &( px_Event_بٹ->x_Event_گروہ_تالا ) );

            xTimeoutOccurred = pdTRUE;
        }
        ورنہ
        {
            /* The task unblocked because the bits were set. */
        }

        /* Control bits might be set as the task had blocked should not be
         * returned. */
        uxReturn &= ~event_EVENT_بٹ_CONTROL_بائٹیں;
    }

    trace_EVENT_گروہ_SYNC_انتہا( x_Event_گروہ, ux_بٹ_مقصد_سیٹ, ux_بٹ_مقصد_Wait_For, xTimeoutOccurred );

    /* Prevent compiler warnings when trace macros are not used. */
    ( خالی ) xTimeoutOccurred;

    واپس uxReturn;
}
/*-----------------------------------------------------------*/

Event_بٹ_t x_Event_گروہ_Wait_بٹ( Event_گروہ_Handle_t x_Event_گروہ,
                                 ثابت Event_بٹ_t ux_بٹ_مقصد_Wait_For,
                                 ثابت بنیاد_Type_t xClearOnExit,
                                 ثابت بنیاد_Type_t x_Wait_For_All_بٹ,
                                 TickType_t x_Ticks_مقصد_Wait )
{
    Event_گروہ_t * px_Event_بٹ = x_Event_گروہ;
    Event_بٹ_t uxReturn, ux_Control_بٹ = 0;
    بنیاد_Type_t xWaitConditionMet, xAlreadyYielded;
    بنیاد_Type_t xTimeoutOccurred = pdFALSE;

    /* Check the user is not attempting to wait on the bits used by the kernel
     * itself, and that at least one bit is being requested. */
    configASSERT( x_Event_گروہ );
    configASSERT( ( ux_بٹ_مقصد_Wait_For & event_EVENT_بٹ_CONTROL_بائٹیں ) == 0 );
    configASSERT( ux_بٹ_مقصد_Wait_For != 0 );
    #اگر ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_مقصد_Wait != 0 ) ) );
    }
    #اگر_ختم

    prvENTER_CRITICAL_OR_SUSPEND_ALL( &( px_Event_بٹ->x_Event_گروہ_تالا ) );
    {
        ثابت Event_بٹ_t ux_Current_Event_بٹ = px_Event_بٹ->ux_Event_بٹ;

        /* Check to see if the wait condition is already met or not. */
        xWaitConditionMet = prvTestWaitCondition( ux_Current_Event_بٹ, ux_بٹ_مقصد_Wait_For, x_Wait_For_All_بٹ );

        اگر( xWaitConditionMet != pdFALSE )
        {
            /* The wait condition has already been met so there is no need to
             * block. */
            uxReturn = ux_Current_Event_بٹ;
            x_Ticks_مقصد_Wait = ( TickType_t ) 0;

            /* Clear the wait bits if requested to do so. */
            اگر( xClearOnExit != pdFALSE )
            {
                px_Event_بٹ->ux_Event_بٹ &= ~ux_بٹ_مقصد_Wait_For;
            }
            ورنہ
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        ورنہ اگر( x_Ticks_مقصد_Wait == ( TickType_t ) 0 )
        {
            /* The wait condition has not been met, but no block time was
             * specified, so just return the current value. */
            uxReturn = ux_Current_Event_بٹ;
            xTimeoutOccurred = pdTRUE;
        }
        ورنہ
        {
            /* The task is going to block to wait for its required bits to be
             * set.  uxControlBits are used to remember the specified behaviour of
             * this call to xEventGroupWaitBits() - for use when the event bits
             * unblock the task. */
            اگر( xClearOnExit != pdFALSE )
            {
                ux_Control_بٹ |= event_CLEAR_EVENTS_ON_EXIT_ثنائی_ہندسہ;
            }
            ورنہ
            {
                mtCOVERAGE_TEST_MARKER();
            }

            اگر( x_Wait_For_All_بٹ != pdFALSE )
            {
                ux_Control_بٹ |= event_WAIT_FOR_ALL_بٹ;
            }
            ورنہ
            {
                mtCOVERAGE_TEST_MARKER();
            }

            /* Store the bits that the calling task is waiting for in the
             * task's event list item so the kernel knows when a match is
             * found.  Then enter the blocked state. */
            v_Task_رکھو_On_Unordered_Event_List( &( px_Event_بٹ->x_Tasks_انتظار_For_بٹ ), ( ux_بٹ_مقصد_Wait_For | ux_Control_بٹ ), x_Ticks_مقصد_Wait );

            /* This is obsolete as it will get set after the task unblocks, but
             * some compilers mistakenly generate a warning about the variable
             * being returned without being set if it is not done. */
            uxReturn = 0;

            trace_EVENT_گروہ_WAIT_بٹ_بلاک( x_Event_گروہ, ux_بٹ_مقصد_Wait_For );
        }
    }
    xAlreadyYielded = prvEXIT_CRITICAL_OR_RESUME_ALL( &( px_Event_بٹ->x_Event_گروہ_تالا ) );

    اگر( x_Ticks_مقصد_Wait != ( TickType_t ) 0 )
    {
        اگر( xAlreadyYielded == pdFALSE )
        {
            پورٹ_YIELD_WITHIN_API();
        }
        ورنہ
        {
            mtCOVERAGE_TEST_MARKER();
        }

        /* The task blocked to wait for its required bits to be set - at this
         * point either the required bits were set or the block time expired.  If
         * the required bits were set they will have been stored in the task's
         * event list item, and they should now be retrieved then cleared. */
        uxReturn = ux_Task_Reset_Event_شے_قدر();

        اگر( ( uxReturn & event_UNBLOCKED_DUE_مقصد_ثنائی_ہندسہ_سیٹ ) == ( Event_بٹ_t ) 0 )
        {
            taskENTER_CRITICAL( &( px_Event_بٹ->x_Event_گروہ_تالا ) );
            {
                /* The task timed out, just return the current event bit value. */
                uxReturn = px_Event_بٹ->ux_Event_بٹ;

                /* It is possible that the event bits were updated between this
                 * task leaving the Blocked state and running again. */
                اگر( prvTestWaitCondition( uxReturn, ux_بٹ_مقصد_Wait_For, x_Wait_For_All_بٹ ) != pdFALSE )
                {
                    اگر( xClearOnExit != pdFALSE )
                    {
                        px_Event_بٹ->ux_Event_بٹ &= ~ux_بٹ_مقصد_Wait_For;
                    }
                    ورنہ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                ورنہ
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                xTimeoutOccurred = pdTRUE;
            }
            taskEXIT_CRITICAL( &( px_Event_بٹ->x_Event_گروہ_تالا ) );
        }
        ورنہ
        {
            /* The task unblocked because the bits were set. */
        }

        /* The task blocked so control bits may have been set. */
        uxReturn &= ~event_EVENT_بٹ_CONTROL_بائٹیں;
    }

    trace_EVENT_گروہ_WAIT_بٹ_انتہا( x_Event_گروہ, ux_بٹ_مقصد_Wait_For, xTimeoutOccurred );

    /* Prevent compiler warnings when trace macros are not used. */
    ( خالی ) xTimeoutOccurred;

    واپس uxReturn;
}
/*-----------------------------------------------------------*/

Event_بٹ_t x_Event_گروہ_Clear_بٹ( Event_گروہ_Handle_t x_Event_گروہ,
                                  ثابت Event_بٹ_t ux_بٹ_مقصد_Clear )
{
    Event_گروہ_t * px_Event_بٹ = x_Event_گروہ;
    Event_بٹ_t uxReturn;

    /* Check the user is not attempting to clear the bits used by the kernel
     * itself. */
    configASSERT( x_Event_گروہ );
    configASSERT( ( ux_بٹ_مقصد_Clear & event_EVENT_بٹ_CONTROL_بائٹیں ) == 0 );

    taskENTER_CRITICAL( &( px_Event_بٹ->x_Event_گروہ_تالا ) );
    {
        trace_EVENT_گروہ_CLEAR_بٹ( x_Event_گروہ, ux_بٹ_مقصد_Clear );

        /* The value returned is the event group value prior to the bits being
         * cleared. */
        uxReturn = px_Event_بٹ->ux_Event_بٹ;

        /* Clear the bits. */
        px_Event_بٹ->ux_Event_بٹ &= ~ux_بٹ_مقصد_Clear;
    }
    taskEXIT_CRITICAL( &( px_Event_بٹ->x_Event_گروہ_تالا ) );

    واپس uxReturn;
}
/*-----------------------------------------------------------*/

#اگر ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) )

    بنیاد_Type_t x_Event_گروہ_Clear_بٹ_From_ISR( Event_گروہ_Handle_t x_Event_گروہ,
                                            ثابت Event_بٹ_t ux_بٹ_مقصد_Clear )
    {
        بنیاد_Type_t xReturn;

        trace_EVENT_گروہ_CLEAR_بٹ_FROM_ISR( x_Event_گروہ, ux_بٹ_مقصد_Clear );
        xReturn = xTimerPendFunctionCallFromISR( v_Event_گروہ_Clear_بٹ_Callback, ( خالی * ) x_Event_گروہ, ( uint32_t ) ux_بٹ_مقصد_Clear, تہی ); /*lint !e9087 Can't avoid cast to void* as a generic callback function not specific to this use case. Callback casts back to original type so safe. */

        واپس xReturn;
    }

#اگر_ختم /* if ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) ) */
/*-----------------------------------------------------------*/

Event_بٹ_t x_Event_گروہ_Get_بٹ_From_ISR( Event_گروہ_Handle_t x_Event_گروہ )
{
    UBaseType_t ux_Saved_مداخلت_حالت;
    Event_گروہ_t ثابت * ثابت px_Event_بٹ = x_Event_گروہ;
    Event_بٹ_t uxReturn;

    prv_ENTER_CRITICAL_OR_نقاب_ISR( ( پورٹ_MUX_TYPE * ) &( px_Event_بٹ->x_Event_گروہ_تالا ), ux_Saved_مداخلت_حالت );
    {
        uxReturn = px_Event_بٹ->ux_Event_بٹ;
    }
    prvEXIT_CRITICAL_OR_UNMASK_ISR( ( پورٹ_MUX_TYPE * ) &( px_Event_بٹ->x_Event_گروہ_تالا ), ux_Saved_مداخلت_حالت );

    واپس uxReturn;
} /*lint !e818 EventGroupHandle_t is a typedef used in other functions to so can't be pointer to const. */
/*-----------------------------------------------------------*/

Event_بٹ_t x_Event_گروہ_سیٹ_بٹ( Event_گروہ_Handle_t x_Event_گروہ,
                                ثابت Event_بٹ_t ux_بٹ_مقصد_سیٹ )
{
    List_شے_t * px_List_شے;
    List_شے_t * px_اگلا;
    List_شے_t ثابت * px_List_انتہا;
    List_t ثابت * pxList;
    Event_بٹ_t ux_بٹ_مقصد_Clear = 0, ux_بٹ_Waited_For, ux_Control_بٹ;
    Event_گروہ_t * px_Event_بٹ = x_Event_گروہ;
    بنیاد_Type_t x_مطابقت_ملا = pdFALSE;

    /* Check the user is not attempting to set the bits used by the kernel
     * itself. */
    configASSERT( x_Event_گروہ );
    configASSERT( ( ux_بٹ_مقصد_سیٹ & event_EVENT_بٹ_CONTROL_بائٹیں ) == 0 );

    pxList = &( px_Event_بٹ->x_Tasks_انتظار_For_بٹ );
    px_List_انتہا = list_GET_انتہا_MARKER( pxList ); /*lint !e826 !e740 !e9087 The mini list structure is used as the list end to save RAM.  This is checked and valid. */

    prvENTER_CRITICAL_OR_SUSPEND_ALL( &( px_Event_بٹ->x_Event_گروہ_تالا ) );
    #اگر ( config_نمبر_کا_CORES > 1 )

        /* We are about to traverse a task list which is a kernel data structure.
         * Thus we need to call prvTakeKernelLock() to take the kernel lock. */
        prv_Take_Kernel_تالا();
    #اگر_ختم /* configNUMBER_OF_CORES > 1 */
    {
        trace_EVENT_گروہ_سیٹ_بٹ( x_Event_گروہ, ux_بٹ_مقصد_سیٹ );

        px_List_شے = list_GET_سر_اندراج( pxList );

        /* Set the bits. */
        px_Event_بٹ->ux_Event_بٹ |= ux_بٹ_مقصد_سیٹ;

        /* See if the new bit value should unblock any tasks. */
        جب( px_List_شے != px_List_انتہا )
        {
            px_اگلا = list_GET_اگلا( px_List_شے );
            ux_بٹ_Waited_For = list_GET_LIST_شے_قدر( px_List_شے );
            x_مطابقت_ملا = pdFALSE;

            /* Split the bits waited for from the control bits. */
            ux_Control_بٹ = ux_بٹ_Waited_For & event_EVENT_بٹ_CONTROL_بائٹیں;
            ux_بٹ_Waited_For &= ~event_EVENT_بٹ_CONTROL_بائٹیں;

            اگر( ( ux_Control_بٹ & event_WAIT_FOR_ALL_بٹ ) == ( Event_بٹ_t ) 0 )
            {
                /* Just looking for single bit being set. */
                اگر( ( ux_بٹ_Waited_For & px_Event_بٹ->ux_Event_بٹ ) != ( Event_بٹ_t ) 0 )
                {
                    x_مطابقت_ملا = pdTRUE;
                }
                ورنہ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            ورنہ اگر( ( ux_بٹ_Waited_For & px_Event_بٹ->ux_Event_بٹ ) == ux_بٹ_Waited_For )
            {
                /* All bits are set. */
                x_مطابقت_ملا = pdTRUE;
            }
            ورنہ
            {
                /* Need all bits to be set, but not all the bits were set. */
            }

            اگر( x_مطابقت_ملا != pdFALSE )
            {
                /* The bits match.  Should the bits be cleared on exit? */
                اگر( ( ux_Control_بٹ & event_CLEAR_EVENTS_ON_EXIT_ثنائی_ہندسہ ) != ( Event_بٹ_t ) 0 )
                {
                    ux_بٹ_مقصد_Clear |= ux_بٹ_Waited_For;
                }
                ورنہ
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                /* Store the actual event flag value in the task's event list
                 * item before removing the task from the event list.  The
                 * eventUNBLOCKED_DUE_TO_BIT_SET bit is set so the task knows
                 * that is was unblocked due to its required bits matching, rather
                 * than because it timed out. */
                vTaskRemoveFromUnorderedEventList( px_List_شے, px_Event_بٹ->ux_Event_بٹ | event_UNBLOCKED_DUE_مقصد_ثنائی_ہندسہ_سیٹ );
            }

            /* Move onto the next list item.  Note pxListItem->pxNext is not
             * used here as the list item may have been removed from the event list
             * and inserted into the ready/pending reading list. */
            px_List_شے = px_اگلا;
        }

        /* Clear any bits that matched when the eventCLEAR_EVENTS_ON_EXIT_BIT
         * bit was set in the control word. */
        px_Event_بٹ->ux_Event_بٹ &= ~ux_بٹ_مقصد_Clear;
    }
    #اگر ( config_نمبر_کا_CORES > 1 )
        /* Release the previously taken kernel lock. */
        prv_Release_Kernel_تالا();
    #اگر_ختم /* configNUMBER_OF_CORES > 1 */
    ( خالی ) prvEXIT_CRITICAL_OR_RESUME_ALL( &( px_Event_بٹ->x_Event_گروہ_تالا ) );


    واپس px_Event_بٹ->ux_Event_بٹ;
}
/*-----------------------------------------------------------*/

خالی v_Event_گروہ_Delete( Event_گروہ_Handle_t x_Event_گروہ )
{
    Event_گروہ_t * px_Event_بٹ = x_Event_گروہ;
    ثابت List_t * px_Tasks_انتظار_For_بٹ;

    configASSERT( px_Event_بٹ );

    px_Tasks_انتظار_For_بٹ = &( px_Event_بٹ->x_Tasks_انتظار_For_بٹ );

    prvENTER_CRITICAL_OR_SUSPEND_ALL( &( px_Event_بٹ->x_Event_گروہ_تالا ) );
    #اگر ( config_نمبر_کا_CORES > 1 )

        /* We are about to traverse a task list which is a kernel data structure.
         * Thus we need to call prvTakeKernelLock() to take the kernel lock. */
        prv_Take_Kernel_تالا();
    #اگر_ختم /* configNUMBER_OF_CORES > 1 */
    {
        trace_EVENT_گروہ_DELETE( x_Event_گروہ );

        جب( list_CURRENT_LIST_لمبائی( px_Tasks_انتظار_For_بٹ ) > ( UBaseType_t ) 0 )
        {
            /* Unblock the task, returning 0 as the event list is being deleted
             * and cannot therefore have any bits set. */
            configASSERT( px_Tasks_انتظار_For_بٹ->x_List_انتہا.px_اگلا != ( ثابت List_شے_t * ) &( px_Tasks_انتظار_For_بٹ->x_List_انتہا ) );
            vTaskRemoveFromUnorderedEventList( px_Tasks_انتظار_For_بٹ->x_List_انتہا.px_اگلا, event_UNBLOCKED_DUE_مقصد_ثنائی_ہندسہ_سیٹ );
        }
    }
    #اگر ( config_نمبر_کا_CORES > 1 )
        /* Release the previously taken kernel lock. */
        prv_Release_Kernel_تالا();
    #اگر_ختم /* configNUMBER_OF_CORES > 1 */
    prvEXIT_CRITICAL_OR_RESUME_ALL( &( px_Event_بٹ->x_Event_گروہ_تالا ) );

    #اگر ( ( config_SUPPORT_DYNAMIC_تخصیص == 1 ) && ( config_SUPPORT_STATIC_تخصیص == 0 ) )
    {
        /* The event group can only have been allocated dynamically - free
         * it again. */
        v_پورٹ_Free( px_Event_بٹ );
    }
    #وگرنہ_اگر ( ( config_SUPPORT_DYNAMIC_تخصیص == 1 ) && ( config_SUPPORT_STATIC_تخصیص == 1 ) )
    {
        /* The event group could have been allocated statically or
         * dynamically, so check before attempting to free the memory. */
        اگر( px_Event_بٹ->ucStaticallyAllocated == ( uint8_t ) pdFALSE )
        {
            v_پورٹ_Free( px_Event_بٹ );
        }
        ورنہ
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    #اگر_ختم /* configSUPPORT_DYNAMIC_ALLOCATION */
}
/*-----------------------------------------------------------*/

#اگر ( config_SUPPORT_STATIC_تخصیص == 1 )
    بنیاد_Type_t x_Event_گروہ_Get_Static_بفر( Event_گروہ_Handle_t x_Event_گروہ,
                                           Static_Event_گروہ_t ** ppx_Event_گروہ_بفر )
    {
        بنیاد_Type_t xReturn;
        Event_گروہ_t * px_Event_بٹ = x_Event_گروہ;

        configASSERT( px_Event_بٹ );
        configASSERT( ppx_Event_گروہ_بفر );

        #اگر ( config_SUPPORT_DYNAMIC_تخصیص == 1 )
        {
            /* Check if the event group was statically allocated. */
            اگر( px_Event_بٹ->ucStaticallyAllocated == ( uint8_t ) pdTRUE )
            {
                *ppx_Event_گروہ_بفر = ( Static_Event_گروہ_t * ) px_Event_بٹ;
                xReturn = pdTRUE;
            }
            ورنہ
            {
                xReturn = pdFALSE;
            }
        }
        #ورنہ /* configSUPPORT_DYNAMIC_ALLOCATION */
        {
            /* Event group must have been statically allocated. */
            *ppx_Event_گروہ_بفر = ( Static_Event_گروہ_t * ) px_Event_بٹ;
            xReturn = pdTRUE;
        }
        #اگر_ختم /* configSUPPORT_DYNAMIC_ALLOCATION */

        واپس xReturn;
    }
#اگر_ختم /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

/* For internal use only - execute a 'set bits' command that was pended from
 * an interrupt. */
خالی v_Event_گروہ_سیٹ_بٹ_Callback( خالی * pv_Event_گروہ,
                                 ثابت uint32_t ul_بٹ_مقصد_سیٹ )
{
    ( خالی ) x_Event_گروہ_سیٹ_بٹ( pv_Event_گروہ, ( Event_بٹ_t ) ul_بٹ_مقصد_سیٹ ); /*lint !e9079 Can't avoid cast to void* as a generic timer callback prototype. Callback casts back to original type so safe. */
}
/*-----------------------------------------------------------*/

/* For internal use only - execute a 'clear bits' command that was pended from
 * an interrupt. */
خالی v_Event_گروہ_Clear_بٹ_Callback( خالی * pv_Event_گروہ,
                                   ثابت uint32_t ul_بٹ_مقصد_Clear )
{
    ( خالی ) x_Event_گروہ_Clear_بٹ( pv_Event_گروہ, ( Event_بٹ_t ) ul_بٹ_مقصد_Clear ); /*lint !e9079 Can't avoid cast to void* as a generic timer callback prototype. Callback casts back to original type so safe. */
}
/*-----------------------------------------------------------*/

ساکن بنیاد_Type_t prvTestWaitCondition( ثابت Event_بٹ_t ux_Current_Event_بٹ,
                                        ثابت Event_بٹ_t ux_بٹ_مقصد_Wait_For,
                                        ثابت بنیاد_Type_t x_Wait_For_All_بٹ )
{
    بنیاد_Type_t xWaitConditionMet = pdFALSE;

    اگر( x_Wait_For_All_بٹ == pdFALSE )
    {
        /* Task only has to wait for one bit within uxBitsToWaitFor to be
         * set.  Is one already set? */
        اگر( ( ux_Current_Event_بٹ & ux_بٹ_مقصد_Wait_For ) != ( Event_بٹ_t ) 0 )
        {
            xWaitConditionMet = pdTRUE;
        }
        ورنہ
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    ورنہ
    {
        /* Task has to wait for all the bits in uxBitsToWaitFor to be set.
         * Are they set already? */
        اگر( ( ux_Current_Event_بٹ & ux_بٹ_مقصد_Wait_For ) == ux_بٹ_مقصد_Wait_For )
        {
            xWaitConditionMet = pdTRUE;
        }
        ورنہ
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }

    واپس xWaitConditionMet;
}
/*-----------------------------------------------------------*/

#اگر ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) )

    بنیاد_Type_t x_Event_گروہ_سیٹ_بٹ_From_ISR( Event_گروہ_Handle_t x_Event_گروہ,
                                          ثابت Event_بٹ_t ux_بٹ_مقصد_سیٹ,
                                          بنیاد_Type_t * px_Higher_ترجیح_Task_Woken )
    {
        بنیاد_Type_t xReturn;

        trace_EVENT_گروہ_سیٹ_بٹ_FROM_ISR( x_Event_گروہ, ux_بٹ_مقصد_سیٹ );
        xReturn = xTimerPendFunctionCallFromISR( v_Event_گروہ_سیٹ_بٹ_Callback, ( خالی * ) x_Event_گروہ, ( uint32_t ) ux_بٹ_مقصد_سیٹ, px_Higher_ترجیح_Task_Woken ); /*lint !e9087 Can't avoid cast to void* as a generic callback function not specific to this use case. Callback casts back to original type so safe. */

        واپس xReturn;
    }

#اگر_ختم /* if ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) ) */
/*-----------------------------------------------------------*/

#اگر ( configUSE_TRACE_FACILITY == 1 )

    UBaseType_t ux_Event_گروہ_Get_نمبر( خالی * x_Event_گروہ )
    {
        UBaseType_t xReturn;
        Event_گروہ_t ثابت * px_Event_بٹ = ( Event_گروہ_t * ) x_Event_گروہ; /*lint !e9087 !e9079 EventGroupHandle_t is a pointer to an EventGroup_t, but EventGroupHandle_t is kept opaque outside of this file for data hiding purposes. */

        اگر( x_Event_گروہ == تہی )
        {
            xReturn = 0;
        }
        ورنہ
        {
            xReturn = px_Event_بٹ->ux_Event_گروہ_نمبر;
        }

        واپس xReturn;
    }

#اگر_ختم /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#اگر ( configUSE_TRACE_FACILITY == 1 )

    خالی v_Event_گروہ_سیٹ_نمبر( خالی * x_Event_گروہ,
                               UBaseType_t ux_Event_گروہ_نمبر )
    {
        ( ( Event_گروہ_t * ) x_Event_گروہ )->ux_Event_گروہ_نمبر = ux_Event_گروہ_نمبر; /*lint !e9087 !e9079 EventGroupHandle_t is a pointer to an EventGroup_t, but EventGroupHandle_t is kept opaque outside of this file for data hiding purposes. */
    }

#اگر_ختم /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/
