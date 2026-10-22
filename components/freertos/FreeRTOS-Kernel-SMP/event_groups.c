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

/* Standard includes. */
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers. That should only be done when
 * task.h is included from an application file. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* FreeRTOS includes. */
#ਅਨ੍ਤਰ੍ਭਾਵ "FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "timers.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "event_groups.h"

/* The MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* This entire source file will be skipped if the application is not configured
 * to include event groups functionality. This #if is closed at the very bottom
 * of this file. If you want to include event groups then ensure
 * configUSE_EVENT_GROUPS is set to 1 in FreeRTOSConfig.h. */
#ਜੇ ( configUSE_EVENT_GROUPS == 1 )

    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ Event_ਸਮੂਹ_Def_t
    {
        Event_ਬਿੱਟ_t ux_Event_ਬਿੱਟ;
        List_t x_Tasks_ਉਡੀਕ_For_ਬਿੱਟ; /**< List of tasks waiting for a bit to be set. */

        #ਜੇ ( configUSE_TRACE_FACILITY == 1 )
            UBaseType_t ux_Event_ਸਮੂਹ_ਸੰਖਿਆ;
        #ਜੇ_ਅੰਤ

        #ਜੇ ( ( config_SUPPORT_STATIC_ਵੰਡ == 1 ) && ( config_SUPPORT_DYNAMIC_ਵੰਡ == 1 ) )
            uint8_t ucStaticallyAllocated; /**< Set to pdTRUE if the event group is statically allocated to ensure no attempt is made to free the memory. */
        #ਜੇ_ਅੰਤ
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

            trace_ENTER_x_Event_ਸਮੂਹ_Create_Static( px_Event_ਸਮੂਹ_ਬਫਰ );

            /* A StaticEventGroup_t object must be provided. */
            configASSERT( px_Event_ਸਮੂਹ_ਬਫਰ );

            #ਜੇ ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticEventGroup_t equals the size of the real
                 * event group structure. */
                ਅਸਥਿਰ size_t x_ਮਾਪ = ਆਕਾਰ( Static_Event_ਸਮੂਹ_t );
                configASSERT( x_ਮਾਪ == ਆਕਾਰ( Event_ਸਮੂਹ_t ) );
            }
            #ਜੇ_ਅੰਤ /* configASSERT_DEFINED */

            /* The user has provided a statically allocated event group - use it. */
            /* MISRA Ref 11.3.1 [Misaligned access] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
            /* coverity[misra_c_2012_rule_11_3_violation] */
            px_Event_ਬਿੱਟ = ( Event_ਸਮੂਹ_t * ) px_Event_ਸਮੂਹ_ਬਫਰ;

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

                trace_EVENT_ਸਮੂਹ_CREATE( px_Event_ਬਿੱਟ );
            }
            ਨਹੀਂਤਾਂ
            {
                /* xEventGroupCreateStatic should only ever be called with
                 * pxEventGroupBuffer pointing to a pre-allocated (compile time
                 * allocated) StaticEventGroup_t variable. */
                trace_EVENT_ਸਮੂਹ_CREATE_FAILED();
            }

            trace_RETURN_x_Event_ਸਮੂਹ_Create_Static( px_Event_ਬਿੱਟ );

            ਵਾਪਸ px_Event_ਬਿੱਟ;
        }

    #ਜੇ_ਅੰਤ /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

    #ਜੇ ( config_SUPPORT_DYNAMIC_ਵੰਡ == 1 )

        Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ_Create( ਖਾਲੀ )
        {
            Event_ਸਮੂਹ_t * px_Event_ਬਿੱਟ;

            trace_ENTER_x_Event_ਸਮੂਹ_Create();

            /* MISRA Ref 11.5.1 [Malloc memory assignment] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
            /* coverity[misra_c_2012_rule_11_5_violation] */
            px_Event_ਬਿੱਟ = ( Event_ਸਮੂਹ_t * ) pv_ਪੋਰਟ_Malloc( ਆਕਾਰ( Event_ਸਮੂਹ_t ) );

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

                trace_EVENT_ਸਮੂਹ_CREATE( px_Event_ਬਿੱਟ );
            }
            ਨਹੀਂਤਾਂ
            {
                trace_EVENT_ਸਮੂਹ_CREATE_FAILED();
            }

            trace_RETURN_x_Event_ਸਮੂਹ_Create( px_Event_ਬਿੱਟ );

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

        trace_ENTER_x_Event_ਸਮੂਹ_Sync( x_Event_ਸਮੂਹ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For, x_Ticks_ਮੰਜ਼ਿਲ_Wait );

        configASSERT( ( ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For & event_EVENT_ਬਿੱਟ_CONTROL_ਬਾਈਟਾਂ ) == 0 );
        configASSERT( ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For != 0 );
        #ਜੇ ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
        {
            configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_ਮੰਜ਼ਿਲ_Wait != 0 ) ) );
        }
        #ਜੇ_ਅੰਤ

        vTaskSuspendAll();
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
        xAlreadyYielded = xTaskResumeAll();

        ਜੇ( x_Ticks_ਮੰਜ਼ਿਲ_Wait != ( TickType_t ) 0 )
        {
            ਜੇ( xAlreadyYielded == pdFALSE )
            {
                taskYIELD_WITHIN_API();
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
                taskENTER_CRITICAL();
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
                taskEXIT_CRITICAL();

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

        trace_RETURN_x_Event_ਸਮੂਹ_Sync( uxReturn );

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

        trace_ENTER_x_Event_ਸਮੂਹ_Wait_ਬਿੱਟ( x_Event_ਸਮੂਹ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For, xClearOnExit, x_Wait_For_All_ਬਿੱਟ, x_Ticks_ਮੰਜ਼ਿਲ_Wait );

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

        vTaskSuspendAll();
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
        xAlreadyYielded = xTaskResumeAll();

        ਜੇ( x_Ticks_ਮੰਜ਼ਿਲ_Wait != ( TickType_t ) 0 )
        {
            ਜੇ( xAlreadyYielded == pdFALSE )
            {
                taskYIELD_WITHIN_API();
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
                taskENTER_CRITICAL();
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
                taskEXIT_CRITICAL();
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

        trace_RETURN_x_Event_ਸਮੂਹ_Wait_ਬਿੱਟ( uxReturn );

        ਵਾਪਸ uxReturn;
    }
/*-----------------------------------------------------------*/

    Event_ਬਿੱਟ_t x_Event_ਸਮੂਹ_Clear_ਬਿੱਟ( Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ,
                                      ਅਚਲ Event_ਬਿੱਟ_t ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear )
    {
        Event_ਸਮੂਹ_t * px_Event_ਬਿੱਟ = x_Event_ਸਮੂਹ;
        Event_ਬਿੱਟ_t uxReturn;

        trace_ENTER_x_Event_ਸਮੂਹ_Clear_ਬਿੱਟ( x_Event_ਸਮੂਹ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear );

        /* Check the user is not attempting to clear the bits used by the kernel
         * itself. */
        configASSERT( x_Event_ਸਮੂਹ );
        configASSERT( ( ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear & event_EVENT_ਬਿੱਟ_CONTROL_ਬਾਈਟਾਂ ) == 0 );

        taskENTER_CRITICAL();
        {
            trace_EVENT_ਸਮੂਹ_CLEAR_ਬਿੱਟ( x_Event_ਸਮੂਹ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear );

            /* The value returned is the event group value prior to the bits being
             * cleared. */
            uxReturn = px_Event_ਬਿੱਟ->ux_Event_ਬਿੱਟ;

            /* Clear the bits. */
            px_Event_ਬਿੱਟ->ux_Event_ਬਿੱਟ &= ~ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear;
        }
        taskEXIT_CRITICAL();

        trace_RETURN_x_Event_ਸਮੂਹ_Clear_ਬਿੱਟ( uxReturn );

        ਵਾਪਸ uxReturn;
    }
/*-----------------------------------------------------------*/

    #ਜੇ ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) )

        ਅਧਾਰ_Type_t x_Event_ਸਮੂਹ_Clear_ਬਿੱਟ_From_ISR( Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ,
                                                ਅਚਲ Event_ਬਿੱਟ_t ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear )
        {
            ਅਧਾਰ_Type_t xReturn;

            trace_ENTER_x_Event_ਸਮੂਹ_Clear_ਬਿੱਟ_From_ISR( x_Event_ਸਮੂਹ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear );

            trace_EVENT_ਸਮੂਹ_CLEAR_ਬਿੱਟ_FROM_ISR( x_Event_ਸਮੂਹ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear );
            xReturn = xTimerPendFunctionCallFromISR( v_Event_ਸਮੂਹ_Clear_ਬਿੱਟ_Callback, ( ਖਾਲੀ * ) x_Event_ਸਮੂਹ, ( uint32_t ) ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear, ਨਿਰਾ );

            trace_RETURN_x_Event_ਸਮੂਹ_Clear_ਬਿੱਟ_From_ISR( xReturn );

            ਵਾਪਸ xReturn;
        }

    #ਜੇ_ਅੰਤ /* if ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) ) */
/*-----------------------------------------------------------*/

    Event_ਬਿੱਟ_t x_Event_ਸਮੂਹ_Get_ਬਿੱਟ_From_ISR( Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ )
    {
        UBaseType_t ux_Saved_ਰੁਕਾਵਟ_ਸਥਿਤੀ;
        Event_ਸਮੂਹ_t ਅਚਲ * ਅਚਲ px_Event_ਬਿੱਟ = x_Event_ਸਮੂਹ;
        Event_ਬਿੱਟ_t uxReturn;

        trace_ENTER_x_Event_ਸਮੂਹ_Get_ਬਿੱਟ_From_ISR( x_Event_ਸਮੂਹ );

        /* MISRA Ref 4.7.1 [Return value shall be checked] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
        /* coverity[misra_c_2012_directive_4_7_violation] */
        ux_Saved_ਰੁਕਾਵਟ_ਸਥਿਤੀ = taskENTER_CRITICAL_FROM_ISR();
        {
            uxReturn = px_Event_ਬਿੱਟ->ux_Event_ਬਿੱਟ;
        }
        taskEXIT_CRITICAL_FROM_ISR( ux_Saved_ਰੁਕਾਵਟ_ਸਥਿਤੀ );

        trace_RETURN_x_Event_ਸਮੂਹ_Get_ਬਿੱਟ_From_ISR( uxReturn );

        ਵਾਪਸ uxReturn;
    }
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

        trace_ENTER_x_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ( x_Event_ਸਮੂਹ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ );

        /* Check the user is not attempting to set the bits used by the kernel
         * itself. */
        configASSERT( x_Event_ਸਮੂਹ );
        configASSERT( ( ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ & event_EVENT_ਬਿੱਟ_CONTROL_ਬਾਈਟਾਂ ) == 0 );

        pxList = &( px_Event_ਬਿੱਟ->x_Tasks_ਉਡੀਕ_For_ਬਿੱਟ );
        px_List_ਸਿਰਾ = list_GET_ਸਿਰਾ_MARKER( pxList );
        vTaskSuspendAll();
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
        ( ਖਾਲੀ ) xTaskResumeAll();

        trace_RETURN_x_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ( px_Event_ਬਿੱਟ->ux_Event_ਬਿੱਟ );

        ਵਾਪਸ px_Event_ਬਿੱਟ->ux_Event_ਬਿੱਟ;
    }
/*-----------------------------------------------------------*/

    ਖਾਲੀ v_Event_ਸਮੂਹ_Delete( Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ )
    {
        Event_ਸਮੂਹ_t * px_Event_ਬਿੱਟ = x_Event_ਸਮੂਹ;
        ਅਚਲ List_t * px_Tasks_ਉਡੀਕ_For_ਬਿੱਟ;

        trace_ENTER_v_Event_ਸਮੂਹ_Delete( x_Event_ਸਮੂਹ );

        configASSERT( px_Event_ਬਿੱਟ );

        px_Tasks_ਉਡੀਕ_For_ਬਿੱਟ = &( px_Event_ਬਿੱਟ->x_Tasks_ਉਡੀਕ_For_ਬਿੱਟ );

        vTaskSuspendAll();
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
        ( ਖਾਲੀ ) xTaskResumeAll();

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

        trace_RETURN_v_Event_ਸਮੂਹ_Delete();
    }
/*-----------------------------------------------------------*/

    #ਜੇ ( config_SUPPORT_STATIC_ਵੰਡ == 1 )
        ਅਧਾਰ_Type_t x_Event_ਸਮੂਹ_Get_Static_ਬਫਰ( Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ,
                                               Static_Event_ਸਮੂਹ_t ** ppx_Event_ਸਮੂਹ_ਬਫਰ )
        {
            ਅਧਾਰ_Type_t xReturn;
            Event_ਸਮੂਹ_t * px_Event_ਬਿੱਟ = x_Event_ਸਮੂਹ;

            trace_ENTER_x_Event_ਸਮੂਹ_Get_Static_ਬਫਰ( x_Event_ਸਮੂਹ, ppx_Event_ਸਮੂਹ_ਬਫਰ );

            configASSERT( px_Event_ਬਿੱਟ );
            configASSERT( ppx_Event_ਸਮੂਹ_ਬਫਰ );

            #ਜੇ ( config_SUPPORT_DYNAMIC_ਵੰਡ == 1 )
            {
                /* Check if the event group was statically allocated. */
                ਜੇ( px_Event_ਬਿੱਟ->ucStaticallyAllocated == ( uint8_t ) pdTRUE )
                {
                    /* MISRA Ref 11.3.1 [Misaligned access] */
                    /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
                    /* coverity[misra_c_2012_rule_11_3_violation] */
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
                /* MISRA Ref 11.3.1 [Misaligned access] */
                /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
                /* coverity[misra_c_2012_rule_11_3_violation] */
                *ppx_Event_ਸਮੂਹ_ਬਫਰ = ( Static_Event_ਸਮੂਹ_t * ) px_Event_ਬਿੱਟ;
                xReturn = pdTRUE;
            }
            #ਜੇ_ਅੰਤ /* configSUPPORT_DYNAMIC_ALLOCATION */

            trace_RETURN_x_Event_ਸਮੂਹ_Get_Static_ਬਫਰ( xReturn );

            ਵਾਪਸ xReturn;
        }
    #ਜੇ_ਅੰਤ /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

/* For internal use only - execute a 'set bits' command that was pended from
 * an interrupt. */
    ਖਾਲੀ v_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ_Callback( ਖਾਲੀ * pv_Event_ਸਮੂਹ,
                                     uint32_t ul_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ )
    {
        trace_ENTER_v_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ_Callback( pv_Event_ਸਮੂਹ, ul_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ );

        /* MISRA Ref 11.5.4 [Callback function parameter] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
        /* coverity[misra_c_2012_rule_11_5_violation] */
        ( ਖਾਲੀ ) x_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ( pv_Event_ਸਮੂਹ, ( Event_ਬਿੱਟ_t ) ul_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ );

        trace_RETURN_v_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ_Callback();
    }
/*-----------------------------------------------------------*/

/* For internal use only - execute a 'clear bits' command that was pended from
 * an interrupt. */
    ਖਾਲੀ v_Event_ਸਮੂਹ_Clear_ਬਿੱਟ_Callback( ਖਾਲੀ * pv_Event_ਸਮੂਹ,
                                       uint32_t ul_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear )
    {
        trace_ENTER_v_Event_ਸਮੂਹ_Clear_ਬਿੱਟ_Callback( pv_Event_ਸਮੂਹ, ul_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear );

        /* MISRA Ref 11.5.4 [Callback function parameter] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
        /* coverity[misra_c_2012_rule_11_5_violation] */
        ( ਖਾਲੀ ) x_Event_ਸਮੂਹ_Clear_ਬਿੱਟ( pv_Event_ਸਮੂਹ, ( Event_ਬਿੱਟ_t ) ul_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear );

        trace_RETURN_v_Event_ਸਮੂਹ_Clear_ਬਿੱਟ_Callback();
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

            trace_ENTER_x_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ_From_ISR( x_Event_ਸਮੂਹ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ, px_Higher_ਤਰਜੀਹ_Task_Woken );

            trace_EVENT_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ_FROM_ISR( x_Event_ਸਮੂਹ, ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ );
            xReturn = xTimerPendFunctionCallFromISR( v_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ_Callback, ( ਖਾਲੀ * ) x_Event_ਸਮੂਹ, ( uint32_t ) ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ, px_Higher_ਤਰਜੀਹ_Task_Woken );

            trace_RETURN_x_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ_From_ISR( xReturn );

            ਵਾਪਸ xReturn;
        }

    #ਜੇ_ਅੰਤ /* if ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) ) */
/*-----------------------------------------------------------*/

    #ਜੇ ( configUSE_TRACE_FACILITY == 1 )

        UBaseType_t ux_Event_ਸਮੂਹ_Get_ਸੰਖਿਆ( ਖਾਲੀ * x_Event_ਸਮੂਹ )
        {
            UBaseType_t xReturn;

            /* MISRA Ref 11.5.2 [Opaque pointer] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
            /* coverity[misra_c_2012_rule_11_5_violation] */
            Event_ਸਮੂਹ_t ਅਚਲ * px_Event_ਬਿੱਟ = ( Event_ਸਮੂਹ_t * ) x_Event_ਸਮੂਹ;

            trace_ENTER_ux_Event_ਸਮੂਹ_Get_ਸੰਖਿਆ( x_Event_ਸਮੂਹ );

            ਜੇ( x_Event_ਸਮੂਹ == ਨਿਰਾ )
            {
                xReturn = 0;
            }
            ਨਹੀਂਤਾਂ
            {
                xReturn = px_Event_ਬਿੱਟ->ux_Event_ਸਮੂਹ_ਸੰਖਿਆ;
            }

            trace_RETURN_ux_Event_ਸਮੂਹ_Get_ਸੰਖਿਆ( xReturn );

            ਵਾਪਸ xReturn;
        }

    #ਜੇ_ਅੰਤ /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

    #ਜੇ ( configUSE_TRACE_FACILITY == 1 )

        ਖਾਲੀ v_Event_ਸਮੂਹ_ਸੈੱਟ_ਸੰਖਿਆ( ਖਾਲੀ * x_Event_ਸਮੂਹ,
                                   UBaseType_t ux_Event_ਸਮੂਹ_ਸੰਖਿਆ )
        {
            trace_ENTER_v_Event_ਸਮੂਹ_ਸੈੱਟ_ਸੰਖਿਆ( x_Event_ਸਮੂਹ, ux_Event_ਸਮੂਹ_ਸੰਖਿਆ );

            /* MISRA Ref 11.5.2 [Opaque pointer] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
            /* coverity[misra_c_2012_rule_11_5_violation] */
            ( ( Event_ਸਮੂਹ_t * ) x_Event_ਸਮੂਹ )->ux_Event_ਸਮੂਹ_ਸੰਖਿਆ = ux_Event_ਸਮੂਹ_ਸੰਖਿਆ;

            trace_RETURN_v_Event_ਸਮੂਹ_ਸੈੱਟ_ਸੰਖਿਆ();
        }

    #ਜੇ_ਅੰਤ /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

/* This entire source file will be skipped if the application is not configured
 * to include event groups functionality. If you want to include event groups
 * then ensure configUSE_EVENT_GROUPS is set to 1 in FreeRTOSConfig.h. */
#ਜੇ_ਅੰਤ /* configUSE_EVENT_GROUPS == 1 */
