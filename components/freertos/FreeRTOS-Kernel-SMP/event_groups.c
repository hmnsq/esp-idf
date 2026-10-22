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
#అన్తర్భావ <stdlib.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers. That should only be done when
 * task.h is included from an application file. */
#నిర్వచించు MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* FreeRTOS includes. */
#అన్తర్భావ "FreeRTOS.h"
#అన్తర్భావ "task.h"
#అన్తర్భావ "timers.h"
#అన్తర్భావ "event_groups.h"

/* The MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#నిర్వచనం_రద్దు MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* This entire source file will be skipped if the application is not configured
 * to include event groups functionality. This #if is closed at the very bottom
 * of this file. If you want to include event groups then ensure
 * configUSE_EVENT_GROUPS is set to 1 in FreeRTOSConfig.h. */
#ఒకవేళ ( configUSE_EVENT_GROUPS == 1 )

    రకనిర్వచనం నిర్మాణం Event_సమూహం_Def_t
    {
        Event_బిట్లు_t ux_Event_బిట్లు;
        List_t x_Tasks_నిరీక్షణ_For_బిట్లు; /**< List of tasks waiting for a bit to be set. */

        #ఒకవేళ ( configUSE_TRACE_FACILITY == 1 )
            UBaseType_t ux_Event_సమూహం_సంఖ్య;
        #అయితే_ముగింపు

        #ఒకవేళ ( ( config_SUPPORT_STATIC_కేటాయింపు == 1 ) && ( config_SUPPORT_DYNAMIC_కేటాయింపు == 1 ) )
            uint8_t ucStaticallyAllocated; /**< Set to pdTRUE if the event group is statically allocated to ensure no attempt is made to free the memory. */
        #అయితే_ముగింపు
    } Event_సమూహం_t;

/*-----------------------------------------------------------*/

/*
 * Test the bits set in uxCurrentEventBits to see if the wait condition is met.
 * The wait condition is defined by xWaitForAllBits.  If xWaitForAllBits is
 * pdTRUE then the wait condition is met if all the bits set in uxBitsToWaitFor
 * are also set in uxCurrentEventBits.  If xWaitForAllBits is pdFALSE then the
 * wait condition is met if any of the bits set in uxBitsToWait for are also set
 * in uxCurrentEventBits.
 */
    స్థిర పునాది_Type_t prvTestWaitCondition( స్థిరరాశి Event_బిట్లు_t ux_Current_Event_బిట్లు,
                                            స్థిరరాశి Event_బిట్లు_t ux_బిట్లు_గమ్యం_Wait_For,
                                            స్థిరరాశి పునాది_Type_t x_Wait_For_All_బిట్లు ) PRIVILEGED_FUNCTION;

/*-----------------------------------------------------------*/

    #ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )

        Event_సమూహం_Handle_t x_Event_సమూహం_Create_Static( Static_Event_సమూహం_t * px_Event_సమూహం_బఫరు )
        {
            Event_సమూహం_t * px_Event_బిట్లు;

            trace_ENTER_x_Event_సమూహం_Create_Static( px_Event_సమూహం_బఫరు );

            /* A StaticEventGroup_t object must be provided. */
            configASSERT( px_Event_సమూహం_బఫరు );

            #ఒకవేళ ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticEventGroup_t equals the size of the real
                 * event group structure. */
                అస్థిర size_t x_కొలత = పరిమాణం( Static_Event_సమూహం_t );
                configASSERT( x_కొలత == పరిమాణం( Event_సమూహం_t ) );
            }
            #అయితే_ముగింపు /* configASSERT_DEFINED */

            /* The user has provided a statically allocated event group - use it. */
            /* MISRA Ref 11.3.1 [Misaligned access] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
            /* coverity[misra_c_2012_rule_11_3_violation] */
            px_Event_బిట్లు = ( Event_సమూహం_t * ) px_Event_సమూహం_బఫరు;

            ఒకవేళ( px_Event_బిట్లు != ఖాళీ )
            {
                px_Event_బిట్లు->ux_Event_బిట్లు = 0;
                vListInitialise( &( px_Event_బిట్లు->x_Tasks_నిరీక్షణ_For_బిట్లు ) );

                #ఒకవేళ ( config_SUPPORT_DYNAMIC_కేటాయింపు == 1 )
                {
                    /* Both static and dynamic allocation can be used, so note that
                     * this event group was created statically in case the event group
                     * is later deleted. */
                    px_Event_బిట్లు->ucStaticallyAllocated = pdTRUE;
                }
                #అయితే_ముగింపు /* configSUPPORT_DYNAMIC_ALLOCATION */

                trace_EVENT_సమూహం_CREATE( px_Event_బిట్లు );
            }
            లేకపోతే
            {
                /* xEventGroupCreateStatic should only ever be called with
                 * pxEventGroupBuffer pointing to a pre-allocated (compile time
                 * allocated) StaticEventGroup_t variable. */
                trace_EVENT_సమూహం_CREATE_FAILED();
            }

            trace_RETURN_x_Event_సమూహం_Create_Static( px_Event_బిట్లు );

            తిరిగి px_Event_బిట్లు;
        }

    #అయితే_ముగింపు /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

    #ఒకవేళ ( config_SUPPORT_DYNAMIC_కేటాయింపు == 1 )

        Event_సమూహం_Handle_t x_Event_సమూహం_Create( శూన్య )
        {
            Event_సమూహం_t * px_Event_బిట్లు;

            trace_ENTER_x_Event_సమూహం_Create();

            /* MISRA Ref 11.5.1 [Malloc memory assignment] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
            /* coverity[misra_c_2012_rule_11_5_violation] */
            px_Event_బిట్లు = ( Event_సమూహం_t * ) pv_పోర్టు_Malloc( పరిమాణం( Event_సమూహం_t ) );

            ఒకవేళ( px_Event_బిట్లు != ఖాళీ )
            {
                px_Event_బిట్లు->ux_Event_బిట్లు = 0;
                vListInitialise( &( px_Event_బిట్లు->x_Tasks_నిరీక్షణ_For_బిట్లు ) );

                #ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )
                {
                    /* Both static and dynamic allocation can be used, so note this
                     * event group was allocated statically in case the event group is
                     * later deleted. */
                    px_Event_బిట్లు->ucStaticallyAllocated = pdFALSE;
                }
                #అయితే_ముగింపు /* configSUPPORT_STATIC_ALLOCATION */

                trace_EVENT_సమూహం_CREATE( px_Event_బిట్లు );
            }
            లేకపోతే
            {
                trace_EVENT_సమూహం_CREATE_FAILED();
            }

            trace_RETURN_x_Event_సమూహం_Create( px_Event_బిట్లు );

            తిరిగి px_Event_బిట్లు;
        }

    #అయితే_ముగింపు /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

    Event_బిట్లు_t x_Event_సమూహం_Sync( Event_సమూహం_Handle_t x_Event_సమూహం,
                                 స్థిరరాశి Event_బిట్లు_t ux_బిట్లు_గమ్యం_సమితి,
                                 స్థిరరాశి Event_బిట్లు_t ux_బిట్లు_గమ్యం_Wait_For,
                                 TickType_t x_Ticks_గమ్యం_Wait )
    {
        Event_బిట్లు_t ux_Original_బిట్టు_విలువ, uxReturn;
        Event_సమూహం_t * px_Event_బిట్లు = x_Event_సమూహం;
        పునాది_Type_t xAlreadyYielded;
        పునాది_Type_t xTimeoutOccurred = pdFALSE;

        trace_ENTER_x_Event_సమూహం_Sync( x_Event_సమూహం, ux_బిట్లు_గమ్యం_సమితి, ux_బిట్లు_గమ్యం_Wait_For, x_Ticks_గమ్యం_Wait );

        configASSERT( ( ux_బిట్లు_గమ్యం_Wait_For & event_EVENT_బిట్లు_CONTROL_బైట్లు ) == 0 );
        configASSERT( ux_బిట్లు_గమ్యం_Wait_For != 0 );
        #ఒకవేళ ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
        {
            configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_గమ్యం_Wait != 0 ) ) );
        }
        #అయితే_ముగింపు

        vTaskSuspendAll();
        {
            ux_Original_బిట్టు_విలువ = px_Event_బిట్లు->ux_Event_బిట్లు;

            ( శూన్య ) x_Event_సమూహం_సమితి_బిట్లు( x_Event_సమూహం, ux_బిట్లు_గమ్యం_సమితి );

            ఒకవేళ( ( ( ux_Original_బిట్టు_విలువ | ux_బిట్లు_గమ్యం_సమితి ) & ux_బిట్లు_గమ్యం_Wait_For ) == ux_బిట్లు_గమ్యం_Wait_For )
            {
                /* All the rendezvous bits are now set - no need to block. */
                uxReturn = ( ux_Original_బిట్టు_విలువ | ux_బిట్లు_గమ్యం_సమితి );

                /* Rendezvous always clear the bits.  They will have been cleared
                 * already unless this is the only task in the rendezvous. */
                px_Event_బిట్లు->ux_Event_బిట్లు &= ~ux_బిట్లు_గమ్యం_Wait_For;

                x_Ticks_గమ్యం_Wait = 0;
            }
            లేకపోతే
            {
                ఒకవేళ( x_Ticks_గమ్యం_Wait != ( TickType_t ) 0 )
                {
                    trace_EVENT_సమూహం_SYNC_ఖండం( x_Event_సమూహం, ux_బిట్లు_గమ్యం_సమితి, ux_బిట్లు_గమ్యం_Wait_For );

                    /* Store the bits that the calling task is waiting for in the
                     * task's event list item so the kernel knows when a match is
                     * found.  Then enter the blocked state. */
                    v_Task_ఉంచు_On_Unordered_Event_List( &( px_Event_బిట్లు->x_Tasks_నిరీక్షణ_For_బిట్లు ), ( ux_బిట్లు_గమ్యం_Wait_For | event_CLEAR_EVENTS_ON_EXIT_బిట్టు | event_WAIT_FOR_ALL_బిట్లు ), x_Ticks_గమ్యం_Wait );

                    /* This assignment is obsolete as uxReturn will get set after
                     * the task unblocks, but some compilers mistakenly generate a
                     * warning about uxReturn being returned without being set if the
                     * assignment is omitted. */
                    uxReturn = 0;
                }
                లేకపోతే
                {
                    /* The rendezvous bits were not set, but no block time was
                     * specified - just return the current event bit value. */
                    uxReturn = px_Event_బిట్లు->ux_Event_బిట్లు;
                    xTimeoutOccurred = pdTRUE;
                }
            }
        }
        xAlreadyYielded = xTaskResumeAll();

        ఒకవేళ( x_Ticks_గమ్యం_Wait != ( TickType_t ) 0 )
        {
            ఒకవేళ( xAlreadyYielded == pdFALSE )
            {
                taskYIELD_WITHIN_API();
            }
            లేకపోతే
            {
                mtCOVERAGE_TEST_MARKER();
            }

            /* The task blocked to wait for its required bits to be set - at this
             * point either the required bits were set or the block time expired.  If
             * the required bits were set they will have been stored in the task's
             * event list item, and they should now be retrieved then cleared. */
            uxReturn = ux_Task_Reset_Event_అంశం_విలువ();

            ఒకవేళ( ( uxReturn & event_UNBLOCKED_DUE_గమ్యం_బిట్టు_సమితి ) == ( Event_బిట్లు_t ) 0 )
            {
                /* The task timed out, just return the current event bit value. */
                taskENTER_CRITICAL();
                {
                    uxReturn = px_Event_బిట్లు->ux_Event_బిట్లు;

                    /* Although the task got here because it timed out before the
                     * bits it was waiting for were set, it is possible that since it
                     * unblocked another task has set the bits.  If this is the case
                     * then it needs to clear the bits before exiting. */
                    ఒకవేళ( ( uxReturn & ux_బిట్లు_గమ్యం_Wait_For ) == ux_బిట్లు_గమ్యం_Wait_For )
                    {
                        px_Event_బిట్లు->ux_Event_బిట్లు &= ~ux_బిట్లు_గమ్యం_Wait_For;
                    }
                    లేకపోతే
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                taskEXIT_CRITICAL();

                xTimeoutOccurred = pdTRUE;
            }
            లేకపోతే
            {
                /* The task unblocked because the bits were set. */
            }

            /* Control bits might be set as the task had blocked should not be
             * returned. */
            uxReturn &= ~event_EVENT_బిట్లు_CONTROL_బైట్లు;
        }

        trace_EVENT_సమూహం_SYNC_చివర( x_Event_సమూహం, ux_బిట్లు_గమ్యం_సమితి, ux_బిట్లు_గమ్యం_Wait_For, xTimeoutOccurred );

        /* Prevent compiler warnings when trace macros are not used. */
        ( శూన్య ) xTimeoutOccurred;

        trace_RETURN_x_Event_సమూహం_Sync( uxReturn );

        తిరిగి uxReturn;
    }
/*-----------------------------------------------------------*/

    Event_బిట్లు_t x_Event_సమూహం_Wait_బిట్లు( Event_సమూహం_Handle_t x_Event_సమూహం,
                                     స్థిరరాశి Event_బిట్లు_t ux_బిట్లు_గమ్యం_Wait_For,
                                     స్థిరరాశి పునాది_Type_t xClearOnExit,
                                     స్థిరరాశి పునాది_Type_t x_Wait_For_All_బిట్లు,
                                     TickType_t x_Ticks_గమ్యం_Wait )
    {
        Event_సమూహం_t * px_Event_బిట్లు = x_Event_సమూహం;
        Event_బిట్లు_t uxReturn, ux_Control_బిట్లు = 0;
        పునాది_Type_t xWaitConditionMet, xAlreadyYielded;
        పునాది_Type_t xTimeoutOccurred = pdFALSE;

        trace_ENTER_x_Event_సమూహం_Wait_బిట్లు( x_Event_సమూహం, ux_బిట్లు_గమ్యం_Wait_For, xClearOnExit, x_Wait_For_All_బిట్లు, x_Ticks_గమ్యం_Wait );

        /* Check the user is not attempting to wait on the bits used by the kernel
         * itself, and that at least one bit is being requested. */
        configASSERT( x_Event_సమూహం );
        configASSERT( ( ux_బిట్లు_గమ్యం_Wait_For & event_EVENT_బిట్లు_CONTROL_బైట్లు ) == 0 );
        configASSERT( ux_బిట్లు_గమ్యం_Wait_For != 0 );
        #ఒకవేళ ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
        {
            configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_గమ్యం_Wait != 0 ) ) );
        }
        #అయితే_ముగింపు

        vTaskSuspendAll();
        {
            స్థిరరాశి Event_బిట్లు_t ux_Current_Event_బిట్లు = px_Event_బిట్లు->ux_Event_బిట్లు;

            /* Check to see if the wait condition is already met or not. */
            xWaitConditionMet = prvTestWaitCondition( ux_Current_Event_బిట్లు, ux_బిట్లు_గమ్యం_Wait_For, x_Wait_For_All_బిట్లు );

            ఒకవేళ( xWaitConditionMet != pdFALSE )
            {
                /* The wait condition has already been met so there is no need to
                 * block. */
                uxReturn = ux_Current_Event_బిట్లు;
                x_Ticks_గమ్యం_Wait = ( TickType_t ) 0;

                /* Clear the wait bits if requested to do so. */
                ఒకవేళ( xClearOnExit != pdFALSE )
                {
                    px_Event_బిట్లు->ux_Event_బిట్లు &= ~ux_బిట్లు_గమ్యం_Wait_For;
                }
                లేకపోతే
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            లేకపోతే ఒకవేళ( x_Ticks_గమ్యం_Wait == ( TickType_t ) 0 )
            {
                /* The wait condition has not been met, but no block time was
                 * specified, so just return the current value. */
                uxReturn = ux_Current_Event_బిట్లు;
                xTimeoutOccurred = pdTRUE;
            }
            లేకపోతే
            {
                /* The task is going to block to wait for its required bits to be
                 * set.  uxControlBits are used to remember the specified behaviour of
                 * this call to xEventGroupWaitBits() - for use when the event bits
                 * unblock the task. */
                ఒకవేళ( xClearOnExit != pdFALSE )
                {
                    ux_Control_బిట్లు |= event_CLEAR_EVENTS_ON_EXIT_బిట్టు;
                }
                లేకపోతే
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                ఒకవేళ( x_Wait_For_All_బిట్లు != pdFALSE )
                {
                    ux_Control_బిట్లు |= event_WAIT_FOR_ALL_బిట్లు;
                }
                లేకపోతే
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                /* Store the bits that the calling task is waiting for in the
                 * task's event list item so the kernel knows when a match is
                 * found.  Then enter the blocked state. */
                v_Task_ఉంచు_On_Unordered_Event_List( &( px_Event_బిట్లు->x_Tasks_నిరీక్షణ_For_బిట్లు ), ( ux_బిట్లు_గమ్యం_Wait_For | ux_Control_బిట్లు ), x_Ticks_గమ్యం_Wait );

                /* This is obsolete as it will get set after the task unblocks, but
                 * some compilers mistakenly generate a warning about the variable
                 * being returned without being set if it is not done. */
                uxReturn = 0;

                trace_EVENT_సమూహం_WAIT_బిట్లు_ఖండం( x_Event_సమూహం, ux_బిట్లు_గమ్యం_Wait_For );
            }
        }
        xAlreadyYielded = xTaskResumeAll();

        ఒకవేళ( x_Ticks_గమ్యం_Wait != ( TickType_t ) 0 )
        {
            ఒకవేళ( xAlreadyYielded == pdFALSE )
            {
                taskYIELD_WITHIN_API();
            }
            లేకపోతే
            {
                mtCOVERAGE_TEST_MARKER();
            }

            /* The task blocked to wait for its required bits to be set - at this
             * point either the required bits were set or the block time expired.  If
             * the required bits were set they will have been stored in the task's
             * event list item, and they should now be retrieved then cleared. */
            uxReturn = ux_Task_Reset_Event_అంశం_విలువ();

            ఒకవేళ( ( uxReturn & event_UNBLOCKED_DUE_గమ్యం_బిట్టు_సమితి ) == ( Event_బిట్లు_t ) 0 )
            {
                taskENTER_CRITICAL();
                {
                    /* The task timed out, just return the current event bit value. */
                    uxReturn = px_Event_బిట్లు->ux_Event_బిట్లు;

                    /* It is possible that the event bits were updated between this
                     * task leaving the Blocked state and running again. */
                    ఒకవేళ( prvTestWaitCondition( uxReturn, ux_బిట్లు_గమ్యం_Wait_For, x_Wait_For_All_బిట్లు ) != pdFALSE )
                    {
                        ఒకవేళ( xClearOnExit != pdFALSE )
                        {
                            px_Event_బిట్లు->ux_Event_బిట్లు &= ~ux_బిట్లు_గమ్యం_Wait_For;
                        }
                        లేకపోతే
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    లేకపోతే
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }

                    xTimeoutOccurred = pdTRUE;
                }
                taskEXIT_CRITICAL();
            }
            లేకపోతే
            {
                /* The task unblocked because the bits were set. */
            }

            /* The task blocked so control bits may have been set. */
            uxReturn &= ~event_EVENT_బిట్లు_CONTROL_బైట్లు;
        }

        trace_EVENT_సమూహం_WAIT_బిట్లు_చివర( x_Event_సమూహం, ux_బిట్లు_గమ్యం_Wait_For, xTimeoutOccurred );

        /* Prevent compiler warnings when trace macros are not used. */
        ( శూన్య ) xTimeoutOccurred;

        trace_RETURN_x_Event_సమూహం_Wait_బిట్లు( uxReturn );

        తిరిగి uxReturn;
    }
/*-----------------------------------------------------------*/

    Event_బిట్లు_t x_Event_సమూహం_Clear_బిట్లు( Event_సమూహం_Handle_t x_Event_సమూహం,
                                      స్థిరరాశి Event_బిట్లు_t ux_బిట్లు_గమ్యం_Clear )
    {
        Event_సమూహం_t * px_Event_బిట్లు = x_Event_సమూహం;
        Event_బిట్లు_t uxReturn;

        trace_ENTER_x_Event_సమూహం_Clear_బిట్లు( x_Event_సమూహం, ux_బిట్లు_గమ్యం_Clear );

        /* Check the user is not attempting to clear the bits used by the kernel
         * itself. */
        configASSERT( x_Event_సమూహం );
        configASSERT( ( ux_బిట్లు_గమ్యం_Clear & event_EVENT_బిట్లు_CONTROL_బైట్లు ) == 0 );

        taskENTER_CRITICAL();
        {
            trace_EVENT_సమూహం_CLEAR_బిట్లు( x_Event_సమూహం, ux_బిట్లు_గమ్యం_Clear );

            /* The value returned is the event group value prior to the bits being
             * cleared. */
            uxReturn = px_Event_బిట్లు->ux_Event_బిట్లు;

            /* Clear the bits. */
            px_Event_బిట్లు->ux_Event_బిట్లు &= ~ux_బిట్లు_గమ్యం_Clear;
        }
        taskEXIT_CRITICAL();

        trace_RETURN_x_Event_సమూహం_Clear_బిట్లు( uxReturn );

        తిరిగి uxReturn;
    }
/*-----------------------------------------------------------*/

    #ఒకవేళ ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) )

        పునాది_Type_t x_Event_సమూహం_Clear_బిట్లు_From_ISR( Event_సమూహం_Handle_t x_Event_సమూహం,
                                                స్థిరరాశి Event_బిట్లు_t ux_బిట్లు_గమ్యం_Clear )
        {
            పునాది_Type_t xReturn;

            trace_ENTER_x_Event_సమూహం_Clear_బిట్లు_From_ISR( x_Event_సమూహం, ux_బిట్లు_గమ్యం_Clear );

            trace_EVENT_సమూహం_CLEAR_బిట్లు_FROM_ISR( x_Event_సమూహం, ux_బిట్లు_గమ్యం_Clear );
            xReturn = xTimerPendFunctionCallFromISR( v_Event_సమూహం_Clear_బిట్లు_Callback, ( శూన్య * ) x_Event_సమూహం, ( uint32_t ) ux_బిట్లు_గమ్యం_Clear, ఖాళీ );

            trace_RETURN_x_Event_సమూహం_Clear_బిట్లు_From_ISR( xReturn );

            తిరిగి xReturn;
        }

    #అయితే_ముగింపు /* if ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) ) */
/*-----------------------------------------------------------*/

    Event_బిట్లు_t x_Event_సమూహం_Get_బిట్లు_From_ISR( Event_సమూహం_Handle_t x_Event_సమూహం )
    {
        UBaseType_t ux_Saved_అంతరాయం_స్థితి;
        Event_సమూహం_t స్థిరరాశి * స్థిరరాశి px_Event_బిట్లు = x_Event_సమూహం;
        Event_బిట్లు_t uxReturn;

        trace_ENTER_x_Event_సమూహం_Get_బిట్లు_From_ISR( x_Event_సమూహం );

        /* MISRA Ref 4.7.1 [Return value shall be checked] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
        /* coverity[misra_c_2012_directive_4_7_violation] */
        ux_Saved_అంతరాయం_స్థితి = taskENTER_CRITICAL_FROM_ISR();
        {
            uxReturn = px_Event_బిట్లు->ux_Event_బిట్లు;
        }
        taskEXIT_CRITICAL_FROM_ISR( ux_Saved_అంతరాయం_స్థితి );

        trace_RETURN_x_Event_సమూహం_Get_బిట్లు_From_ISR( uxReturn );

        తిరిగి uxReturn;
    }
/*-----------------------------------------------------------*/

    Event_బిట్లు_t x_Event_సమూహం_సమితి_బిట్లు( Event_సమూహం_Handle_t x_Event_సమూహం,
                                    స్థిరరాశి Event_బిట్లు_t ux_బిట్లు_గమ్యం_సమితి )
    {
        List_అంశం_t * px_List_అంశం;
        List_అంశం_t * px_తదుపరి;
        List_అంశం_t స్థిరరాశి * px_List_చివర;
        List_t స్థిరరాశి * pxList;
        Event_బిట్లు_t ux_బిట్లు_గమ్యం_Clear = 0, ux_బిట్లు_Waited_For, ux_Control_బిట్లు;
        Event_సమూహం_t * px_Event_బిట్లు = x_Event_సమూహం;
        పునాది_Type_t x_సరిపోలిక_దొరికింది = pdFALSE;

        trace_ENTER_x_Event_సమూహం_సమితి_బిట్లు( x_Event_సమూహం, ux_బిట్లు_గమ్యం_సమితి );

        /* Check the user is not attempting to set the bits used by the kernel
         * itself. */
        configASSERT( x_Event_సమూహం );
        configASSERT( ( ux_బిట్లు_గమ్యం_సమితి & event_EVENT_బిట్లు_CONTROL_బైట్లు ) == 0 );

        pxList = &( px_Event_బిట్లు->x_Tasks_నిరీక్షణ_For_బిట్లు );
        px_List_చివర = list_GET_చివర_MARKER( pxList );
        vTaskSuspendAll();
        {
            trace_EVENT_సమూహం_సమితి_బిట్లు( x_Event_సమూహం, ux_బిట్లు_గమ్యం_సమితి );

            px_List_అంశం = list_GET_తల_నమోదు( pxList );

            /* Set the bits. */
            px_Event_బిట్లు->ux_Event_బిట్లు |= ux_బిట్లు_గమ్యం_సమితి;

            /* See if the new bit value should unblock any tasks. */
            అయితే( px_List_అంశం != px_List_చివర )
            {
                px_తదుపరి = list_GET_తదుపరి( px_List_అంశం );
                ux_బిట్లు_Waited_For = list_GET_LIST_అంశం_విలువ( px_List_అంశం );
                x_సరిపోలిక_దొరికింది = pdFALSE;

                /* Split the bits waited for from the control bits. */
                ux_Control_బిట్లు = ux_బిట్లు_Waited_For & event_EVENT_బిట్లు_CONTROL_బైట్లు;
                ux_బిట్లు_Waited_For &= ~event_EVENT_బిట్లు_CONTROL_బైట్లు;

                ఒకవేళ( ( ux_Control_బిట్లు & event_WAIT_FOR_ALL_బిట్లు ) == ( Event_బిట్లు_t ) 0 )
                {
                    /* Just looking for single bit being set. */
                    ఒకవేళ( ( ux_బిట్లు_Waited_For & px_Event_బిట్లు->ux_Event_బిట్లు ) != ( Event_బిట్లు_t ) 0 )
                    {
                        x_సరిపోలిక_దొరికింది = pdTRUE;
                    }
                    లేకపోతే
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                లేకపోతే ఒకవేళ( ( ux_బిట్లు_Waited_For & px_Event_బిట్లు->ux_Event_బిట్లు ) == ux_బిట్లు_Waited_For )
                {
                    /* All bits are set. */
                    x_సరిపోలిక_దొరికింది = pdTRUE;
                }
                లేకపోతే
                {
                    /* Need all bits to be set, but not all the bits were set. */
                }

                ఒకవేళ( x_సరిపోలిక_దొరికింది != pdFALSE )
                {
                    /* The bits match.  Should the bits be cleared on exit? */
                    ఒకవేళ( ( ux_Control_బిట్లు & event_CLEAR_EVENTS_ON_EXIT_బిట్టు ) != ( Event_బిట్లు_t ) 0 )
                    {
                        ux_బిట్లు_గమ్యం_Clear |= ux_బిట్లు_Waited_For;
                    }
                    లేకపోతే
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }

                    /* Store the actual event flag value in the task's event list
                     * item before removing the task from the event list.  The
                     * eventUNBLOCKED_DUE_TO_BIT_SET bit is set so the task knows
                     * that is was unblocked due to its required bits matching, rather
                     * than because it timed out. */
                    vTaskRemoveFromUnorderedEventList( px_List_అంశం, px_Event_బిట్లు->ux_Event_బిట్లు | event_UNBLOCKED_DUE_గమ్యం_బిట్టు_సమితి );
                }

                /* Move onto the next list item.  Note pxListItem->pxNext is not
                 * used here as the list item may have been removed from the event list
                 * and inserted into the ready/pending reading list. */
                px_List_అంశం = px_తదుపరి;
            }

            /* Clear any bits that matched when the eventCLEAR_EVENTS_ON_EXIT_BIT
             * bit was set in the control word. */
            px_Event_బిట్లు->ux_Event_బిట్లు &= ~ux_బిట్లు_గమ్యం_Clear;
        }
        ( శూన్య ) xTaskResumeAll();

        trace_RETURN_x_Event_సమూహం_సమితి_బిట్లు( px_Event_బిట్లు->ux_Event_బిట్లు );

        తిరిగి px_Event_బిట్లు->ux_Event_బిట్లు;
    }
/*-----------------------------------------------------------*/

    శూన్య v_Event_సమూహం_Delete( Event_సమూహం_Handle_t x_Event_సమూహం )
    {
        Event_సమూహం_t * px_Event_బిట్లు = x_Event_సమూహం;
        స్థిరరాశి List_t * px_Tasks_నిరీక్షణ_For_బిట్లు;

        trace_ENTER_v_Event_సమూహం_Delete( x_Event_సమూహం );

        configASSERT( px_Event_బిట్లు );

        px_Tasks_నిరీక్షణ_For_బిట్లు = &( px_Event_బిట్లు->x_Tasks_నిరీక్షణ_For_బిట్లు );

        vTaskSuspendAll();
        {
            trace_EVENT_సమూహం_DELETE( x_Event_సమూహం );

            అయితే( list_CURRENT_LIST_నిడివి( px_Tasks_నిరీక్షణ_For_బిట్లు ) > ( UBaseType_t ) 0 )
            {
                /* Unblock the task, returning 0 as the event list is being deleted
                 * and cannot therefore have any bits set. */
                configASSERT( px_Tasks_నిరీక్షణ_For_బిట్లు->x_List_చివర.px_తదుపరి != ( స్థిరరాశి List_అంశం_t * ) &( px_Tasks_నిరీక్షణ_For_బిట్లు->x_List_చివర ) );
                vTaskRemoveFromUnorderedEventList( px_Tasks_నిరీక్షణ_For_బిట్లు->x_List_చివర.px_తదుపరి, event_UNBLOCKED_DUE_గమ్యం_బిట్టు_సమితి );
            }
        }
        ( శూన్య ) xTaskResumeAll();

        #ఒకవేళ ( ( config_SUPPORT_DYNAMIC_కేటాయింపు == 1 ) && ( config_SUPPORT_STATIC_కేటాయింపు == 0 ) )
        {
            /* The event group can only have been allocated dynamically - free
             * it again. */
            v_పోర్టు_Free( px_Event_బిట్లు );
        }
        #లేదావేళ ( ( config_SUPPORT_DYNAMIC_కేటాయింపు == 1 ) && ( config_SUPPORT_STATIC_కేటాయింపు == 1 ) )
        {
            /* The event group could have been allocated statically or
             * dynamically, so check before attempting to free the memory. */
            ఒకవేళ( px_Event_బిట్లు->ucStaticallyAllocated == ( uint8_t ) pdFALSE )
            {
                v_పోర్టు_Free( px_Event_బిట్లు );
            }
            లేకపోతే
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        #అయితే_ముగింపు /* configSUPPORT_DYNAMIC_ALLOCATION */

        trace_RETURN_v_Event_సమూహం_Delete();
    }
/*-----------------------------------------------------------*/

    #ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )
        పునాది_Type_t x_Event_సమూహం_Get_Static_బఫరు( Event_సమూహం_Handle_t x_Event_సమూహం,
                                               Static_Event_సమూహం_t ** ppx_Event_సమూహం_బఫరు )
        {
            పునాది_Type_t xReturn;
            Event_సమూహం_t * px_Event_బిట్లు = x_Event_సమూహం;

            trace_ENTER_x_Event_సమూహం_Get_Static_బఫరు( x_Event_సమూహం, ppx_Event_సమూహం_బఫరు );

            configASSERT( px_Event_బిట్లు );
            configASSERT( ppx_Event_సమూహం_బఫరు );

            #ఒకవేళ ( config_SUPPORT_DYNAMIC_కేటాయింపు == 1 )
            {
                /* Check if the event group was statically allocated. */
                ఒకవేళ( px_Event_బిట్లు->ucStaticallyAllocated == ( uint8_t ) pdTRUE )
                {
                    /* MISRA Ref 11.3.1 [Misaligned access] */
                    /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
                    /* coverity[misra_c_2012_rule_11_3_violation] */
                    *ppx_Event_సమూహం_బఫరు = ( Static_Event_సమూహం_t * ) px_Event_బిట్లు;
                    xReturn = pdTRUE;
                }
                లేకపోతే
                {
                    xReturn = pdFALSE;
                }
            }
            #లేకపోతే /* configSUPPORT_DYNAMIC_ALLOCATION */
            {
                /* Event group must have been statically allocated. */
                /* MISRA Ref 11.3.1 [Misaligned access] */
                /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
                /* coverity[misra_c_2012_rule_11_3_violation] */
                *ppx_Event_సమూహం_బఫరు = ( Static_Event_సమూహం_t * ) px_Event_బిట్లు;
                xReturn = pdTRUE;
            }
            #అయితే_ముగింపు /* configSUPPORT_DYNAMIC_ALLOCATION */

            trace_RETURN_x_Event_సమూహం_Get_Static_బఫరు( xReturn );

            తిరిగి xReturn;
        }
    #అయితే_ముగింపు /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

/* For internal use only - execute a 'set bits' command that was pended from
 * an interrupt. */
    శూన్య v_Event_సమూహం_సమితి_బిట్లు_Callback( శూన్య * pv_Event_సమూహం,
                                     uint32_t ul_బిట్లు_గమ్యం_సమితి )
    {
        trace_ENTER_v_Event_సమూహం_సమితి_బిట్లు_Callback( pv_Event_సమూహం, ul_బిట్లు_గమ్యం_సమితి );

        /* MISRA Ref 11.5.4 [Callback function parameter] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
        /* coverity[misra_c_2012_rule_11_5_violation] */
        ( శూన్య ) x_Event_సమూహం_సమితి_బిట్లు( pv_Event_సమూహం, ( Event_బిట్లు_t ) ul_బిట్లు_గమ్యం_సమితి );

        trace_RETURN_v_Event_సమూహం_సమితి_బిట్లు_Callback();
    }
/*-----------------------------------------------------------*/

/* For internal use only - execute a 'clear bits' command that was pended from
 * an interrupt. */
    శూన్య v_Event_సమూహం_Clear_బిట్లు_Callback( శూన్య * pv_Event_సమూహం,
                                       uint32_t ul_బిట్లు_గమ్యం_Clear )
    {
        trace_ENTER_v_Event_సమూహం_Clear_బిట్లు_Callback( pv_Event_సమూహం, ul_బిట్లు_గమ్యం_Clear );

        /* MISRA Ref 11.5.4 [Callback function parameter] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
        /* coverity[misra_c_2012_rule_11_5_violation] */
        ( శూన్య ) x_Event_సమూహం_Clear_బిట్లు( pv_Event_సమూహం, ( Event_బిట్లు_t ) ul_బిట్లు_గమ్యం_Clear );

        trace_RETURN_v_Event_సమూహం_Clear_బిట్లు_Callback();
    }
/*-----------------------------------------------------------*/

    స్థిర పునాది_Type_t prvTestWaitCondition( స్థిరరాశి Event_బిట్లు_t ux_Current_Event_బిట్లు,
                                            స్థిరరాశి Event_బిట్లు_t ux_బిట్లు_గమ్యం_Wait_For,
                                            స్థిరరాశి పునాది_Type_t x_Wait_For_All_బిట్లు )
    {
        పునాది_Type_t xWaitConditionMet = pdFALSE;

        ఒకవేళ( x_Wait_For_All_బిట్లు == pdFALSE )
        {
            /* Task only has to wait for one bit within uxBitsToWaitFor to be
             * set.  Is one already set? */
            ఒకవేళ( ( ux_Current_Event_బిట్లు & ux_బిట్లు_గమ్యం_Wait_For ) != ( Event_బిట్లు_t ) 0 )
            {
                xWaitConditionMet = pdTRUE;
            }
            లేకపోతే
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        లేకపోతే
        {
            /* Task has to wait for all the bits in uxBitsToWaitFor to be set.
             * Are they set already? */
            ఒకవేళ( ( ux_Current_Event_బిట్లు & ux_బిట్లు_గమ్యం_Wait_For ) == ux_బిట్లు_గమ్యం_Wait_For )
            {
                xWaitConditionMet = pdTRUE;
            }
            లేకపోతే
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }

        తిరిగి xWaitConditionMet;
    }
/*-----------------------------------------------------------*/

    #ఒకవేళ ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) )

        పునాది_Type_t x_Event_సమూహం_సమితి_బిట్లు_From_ISR( Event_సమూహం_Handle_t x_Event_సమూహం,
                                              స్థిరరాశి Event_బిట్లు_t ux_బిట్లు_గమ్యం_సమితి,
                                              పునాది_Type_t * px_Higher_ప్రాధాన్యత_Task_Woken )
        {
            పునాది_Type_t xReturn;

            trace_ENTER_x_Event_సమూహం_సమితి_బిట్లు_From_ISR( x_Event_సమూహం, ux_బిట్లు_గమ్యం_సమితి, px_Higher_ప్రాధాన్యత_Task_Woken );

            trace_EVENT_సమూహం_సమితి_బిట్లు_FROM_ISR( x_Event_సమూహం, ux_బిట్లు_గమ్యం_సమితి );
            xReturn = xTimerPendFunctionCallFromISR( v_Event_సమూహం_సమితి_బిట్లు_Callback, ( శూన్య * ) x_Event_సమూహం, ( uint32_t ) ux_బిట్లు_గమ్యం_సమితి, px_Higher_ప్రాధాన్యత_Task_Woken );

            trace_RETURN_x_Event_సమూహం_సమితి_బిట్లు_From_ISR( xReturn );

            తిరిగి xReturn;
        }

    #అయితే_ముగింపు /* if ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) ) */
/*-----------------------------------------------------------*/

    #ఒకవేళ ( configUSE_TRACE_FACILITY == 1 )

        UBaseType_t ux_Event_సమూహం_Get_సంఖ్య( శూన్య * x_Event_సమూహం )
        {
            UBaseType_t xReturn;

            /* MISRA Ref 11.5.2 [Opaque pointer] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
            /* coverity[misra_c_2012_rule_11_5_violation] */
            Event_సమూహం_t స్థిరరాశి * px_Event_బిట్లు = ( Event_సమూహం_t * ) x_Event_సమూహం;

            trace_ENTER_ux_Event_సమూహం_Get_సంఖ్య( x_Event_సమూహం );

            ఒకవేళ( x_Event_సమూహం == ఖాళీ )
            {
                xReturn = 0;
            }
            లేకపోతే
            {
                xReturn = px_Event_బిట్లు->ux_Event_సమూహం_సంఖ్య;
            }

            trace_RETURN_ux_Event_సమూహం_Get_సంఖ్య( xReturn );

            తిరిగి xReturn;
        }

    #అయితే_ముగింపు /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

    #ఒకవేళ ( configUSE_TRACE_FACILITY == 1 )

        శూన్య v_Event_సమూహం_సమితి_సంఖ్య( శూన్య * x_Event_సమూహం,
                                   UBaseType_t ux_Event_సమూహం_సంఖ్య )
        {
            trace_ENTER_v_Event_సమూహం_సమితి_సంఖ్య( x_Event_సమూహం, ux_Event_సమూహం_సంఖ్య );

            /* MISRA Ref 11.5.2 [Opaque pointer] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
            /* coverity[misra_c_2012_rule_11_5_violation] */
            ( ( Event_సమూహం_t * ) x_Event_సమూహం )->ux_Event_సమూహం_సంఖ్య = ux_Event_సమూహం_సంఖ్య;

            trace_RETURN_v_Event_సమూహం_సమితి_సంఖ్య();
        }

    #అయితే_ముగింపు /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

/* This entire source file will be skipped if the application is not configured
 * to include event groups functionality. If you want to include event groups
 * then ensure configUSE_EVENT_GROUPS is set to 1 in FreeRTOSConfig.h. */
#అయితే_ముగింపు /* configUSE_EVENT_GROUPS == 1 */
