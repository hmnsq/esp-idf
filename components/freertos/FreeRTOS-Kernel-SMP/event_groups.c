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
#अन्तर्भाव <stdlib.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers. That should only be done when
 * task.h is included from an application file. */
#परिभाषय MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* FreeRTOS includes. */
#अन्तर्भाव "FreeRTOS.h"
#अन्तर्भाव "task.h"
#अन्तर्भाव "timers.h"
#अन्तर्भाव "event_groups.h"

/* The MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#अपरिभाषय MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* This entire source file will be skipped if the application is not configured
 * to include event groups functionality. This #if is closed at the very bottom
 * of this file. If you want to include event groups then ensure
 * configUSE_EVENT_GROUPS is set to 1 in FreeRTOSConfig.h. */
#यदि ( configUSE_EVENT_GROUPS == 1 )

    प्रकारनिर्देश संरचना Event_गणः_Def_t
    {
        Event_द्वयङ्काः_t ux_Event_द्वयङ्काः;
        List_t x_Tasks_प्रतीक्षा_For_द्वयङ्काः; /**< List of tasks waiting for a bit to be set. */

        #यदि ( configUSE_TRACE_FACILITY == 1 )
            UBaseType_t ux_Event_गणः_सङ्ख्या;
        #यदि_अन्तः

        #यदि ( ( config_SUPPORT_STATIC_आवण्टनम् == 1 ) && ( config_SUPPORT_DYNAMIC_आवण्टनम् == 1 ) )
            uint8_t ucStaticallyAllocated; /**< Set to pdTRUE if the event group is statically allocated to ensure no attempt is made to free the memory. */
        #यदि_अन्तः
    } Event_गणः_t;

/*-----------------------------------------------------------*/

/*
 * Test the bits set in uxCurrentEventBits to see if the wait condition is met.
 * The wait condition is defined by xWaitForAllBits.  If xWaitForAllBits is
 * pdTRUE then the wait condition is met if all the bits set in uxBitsToWaitFor
 * are also set in uxCurrentEventBits.  If xWaitForAllBits is pdFALSE then the
 * wait condition is met if any of the bits set in uxBitsToWait for are also set
 * in uxCurrentEventBits.
 */
    स्थिर आधारः_Type_t prvTestWaitCondition( ध्रुव Event_द्वयङ्काः_t ux_Current_Event_द्वयङ्काः,
                                            ध्रुव Event_द्वयङ्काः_t ux_द्वयङ्काः_गन्तव्यम्_Wait_For,
                                            ध्रुव आधारः_Type_t x_Wait_For_All_द्वयङ्काः ) PRIVILEGED_FUNCTION;

/*-----------------------------------------------------------*/

    #यदि ( config_SUPPORT_STATIC_आवण्टनम् == 1 )

        Event_गणः_Handle_t x_Event_गणः_Create_Static( Static_Event_गणः_t * px_Event_गणः_बफर )
        {
            Event_गणः_t * px_Event_द्वयङ्काः;

            trace_ENTER_x_Event_गणः_Create_Static( px_Event_गणः_बफर );

            /* A StaticEventGroup_t object must be provided. */
            configASSERT( px_Event_गणः_बफर );

            #यदि ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticEventGroup_t equals the size of the real
                 * event group structure. */
                चञ्चल size_t x_आकार = परिमाण( Static_Event_गणः_t );
                configASSERT( x_आकार == परिमाण( Event_गणः_t ) );
            }
            #यदि_अन्तः /* configASSERT_DEFINED */

            /* The user has provided a statically allocated event group - use it. */
            /* MISRA Ref 11.3.1 [Misaligned access] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
            /* coverity[misra_c_2012_rule_11_3_violation] */
            px_Event_द्वयङ्काः = ( Event_गणः_t * ) px_Event_गणः_बफर;

            यदि( px_Event_द्वयङ्काः != रिक्त )
            {
                px_Event_द्वयङ्काः->ux_Event_द्वयङ्काः = 0;
                vListInitialise( &( px_Event_द्वयङ्काः->x_Tasks_प्रतीक्षा_For_द्वयङ्काः ) );

                #यदि ( config_SUPPORT_DYNAMIC_आवण्टनम् == 1 )
                {
                    /* Both static and dynamic allocation can be used, so note that
                     * this event group was created statically in case the event group
                     * is later deleted. */
                    px_Event_द्वयङ्काः->ucStaticallyAllocated = pdTRUE;
                }
                #यदि_अन्तः /* configSUPPORT_DYNAMIC_ALLOCATION */

                trace_EVENT_गणः_CREATE( px_Event_द्वयङ्काः );
            }
            अन्यथा
            {
                /* xEventGroupCreateStatic should only ever be called with
                 * pxEventGroupBuffer pointing to a pre-allocated (compile time
                 * allocated) StaticEventGroup_t variable. */
                trace_EVENT_गणः_CREATE_FAILED();
            }

            trace_RETURN_x_Event_गणः_Create_Static( px_Event_द्वयङ्काः );

            प्रत्यागम px_Event_द्वयङ्काः;
        }

    #यदि_अन्तः /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

    #यदि ( config_SUPPORT_DYNAMIC_आवण्टनम् == 1 )

        Event_गणः_Handle_t x_Event_गणः_Create( शून्य )
        {
            Event_गणः_t * px_Event_द्वयङ्काः;

            trace_ENTER_x_Event_गणः_Create();

            /* MISRA Ref 11.5.1 [Malloc memory assignment] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
            /* coverity[misra_c_2012_rule_11_5_violation] */
            px_Event_द्वयङ्काः = ( Event_गणः_t * ) pv_पत्तनम्_Malloc( परिमाण( Event_गणः_t ) );

            यदि( px_Event_द्वयङ्काः != रिक्त )
            {
                px_Event_द्वयङ्काः->ux_Event_द्वयङ्काः = 0;
                vListInitialise( &( px_Event_द्वयङ्काः->x_Tasks_प्रतीक्षा_For_द्वयङ्काः ) );

                #यदि ( config_SUPPORT_STATIC_आवण्टनम् == 1 )
                {
                    /* Both static and dynamic allocation can be used, so note this
                     * event group was allocated statically in case the event group is
                     * later deleted. */
                    px_Event_द्वयङ्काः->ucStaticallyAllocated = pdFALSE;
                }
                #यदि_अन्तः /* configSUPPORT_STATIC_ALLOCATION */

                trace_EVENT_गणः_CREATE( px_Event_द्वयङ्काः );
            }
            अन्यथा
            {
                trace_EVENT_गणः_CREATE_FAILED();
            }

            trace_RETURN_x_Event_गणः_Create( px_Event_द्वयङ्काः );

            प्रत्यागम px_Event_द्वयङ्काः;
        }

    #यदि_अन्तः /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

    Event_द्वयङ्काः_t x_Event_गणः_Sync( Event_गणः_Handle_t x_Event_गणः,
                                 ध्रुव Event_द्वयङ्काः_t ux_द्वयङ्काः_गन्तव्यम्_समुच्चयः,
                                 ध्रुव Event_द्वयङ्काः_t ux_द्वयङ्काः_गन्तव्यम्_Wait_For,
                                 TickType_t x_Ticks_गन्तव्यम्_Wait )
    {
        Event_द्वयङ्काः_t ux_Original_Bit_मानम्, uxReturn;
        Event_गणः_t * px_Event_द्वयङ्काः = x_Event_गणः;
        आधारः_Type_t xAlreadyYielded;
        आधारः_Type_t xTimeoutOccurred = pdFALSE;

        trace_ENTER_x_Event_गणः_Sync( x_Event_गणः, ux_द्वयङ्काः_गन्तव्यम्_समुच्चयः, ux_द्वयङ्काः_गन्तव्यम्_Wait_For, x_Ticks_गन्तव्यम्_Wait );

        configASSERT( ( ux_द्वयङ्काः_गन्तव्यम्_Wait_For & event_EVENT_द्वयङ्काः_CONTROL_अष्टकानि ) == 0 );
        configASSERT( ux_द्वयङ्काः_गन्तव्यम्_Wait_For != 0 );
        #यदि ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
        {
            configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_गन्तव्यम्_Wait != 0 ) ) );
        }
        #यदि_अन्तः

        vTaskSuspendAll();
        {
            ux_Original_Bit_मानम् = px_Event_द्वयङ्काः->ux_Event_द्वयङ्काः;

            ( शून्य ) x_Event_गणः_समुच्चयः_द्वयङ्काः( x_Event_गणः, ux_द्वयङ्काः_गन्तव्यम्_समुच्चयः );

            यदि( ( ( ux_Original_Bit_मानम् | ux_द्वयङ्काः_गन्तव्यम्_समुच्चयः ) & ux_द्वयङ्काः_गन्तव्यम्_Wait_For ) == ux_द्वयङ्काः_गन्तव्यम्_Wait_For )
            {
                /* All the rendezvous bits are now set - no need to block. */
                uxReturn = ( ux_Original_Bit_मानम् | ux_द्वयङ्काः_गन्तव्यम्_समुच्चयः );

                /* Rendezvous always clear the bits.  They will have been cleared
                 * already unless this is the only task in the rendezvous. */
                px_Event_द्वयङ्काः->ux_Event_द्वयङ्काः &= ~ux_द्वयङ्काः_गन्तव्यम्_Wait_For;

                x_Ticks_गन्तव्यम्_Wait = 0;
            }
            अन्यथा
            {
                यदि( x_Ticks_गन्तव्यम्_Wait != ( TickType_t ) 0 )
                {
                    trace_EVENT_गणः_SYNC_खण्डम्( x_Event_गणः, ux_द्वयङ्काः_गन्तव्यम्_समुच्चयः, ux_द्वयङ्काः_गन्तव्यम्_Wait_For );

                    /* Store the bits that the calling task is waiting for in the
                     * task's event list item so the kernel knows when a match is
                     * found.  Then enter the blocked state. */
                    vTaskPlaceOnUnorderedEventList( &( px_Event_द्वयङ्काः->x_Tasks_प्रतीक्षा_For_द्वयङ्काः ), ( ux_द्वयङ्काः_गन्तव्यम्_Wait_For | eventCLEAR_EVENTS_ON_EXIT_BIT | event_WAIT_FOR_ALL_द्वयङ्काः ), x_Ticks_गन्तव्यम्_Wait );

                    /* This assignment is obsolete as uxReturn will get set after
                     * the task unblocks, but some compilers mistakenly generate a
                     * warning about uxReturn being returned without being set if the
                     * assignment is omitted. */
                    uxReturn = 0;
                }
                अन्यथा
                {
                    /* The rendezvous bits were not set, but no block time was
                     * specified - just return the current event bit value. */
                    uxReturn = px_Event_द्वयङ्काः->ux_Event_द्वयङ्काः;
                    xTimeoutOccurred = pdTRUE;
                }
            }
        }
        xAlreadyYielded = xTaskResumeAll();

        यदि( x_Ticks_गन्तव्यम्_Wait != ( TickType_t ) 0 )
        {
            यदि( xAlreadyYielded == pdFALSE )
            {
                taskYIELD_WITHIN_API();
            }
            अन्यथा
            {
                mtCOVERAGE_TEST_MARKER();
            }

            /* The task blocked to wait for its required bits to be set - at this
             * point either the required bits were set or the block time expired.  If
             * the required bits were set they will have been stored in the task's
             * event list item, and they should now be retrieved then cleared. */
            uxReturn = ux_Task_Reset_Event_Item_मानम्();

            यदि( ( uxReturn & event_UNBLOCKED_DUE_गन्तव्यम्_BIT_समुच्चयः ) == ( Event_द्वयङ्काः_t ) 0 )
            {
                /* The task timed out, just return the current event bit value. */
                taskENTER_CRITICAL();
                {
                    uxReturn = px_Event_द्वयङ्काः->ux_Event_द्वयङ्काः;

                    /* Although the task got here because it timed out before the
                     * bits it was waiting for were set, it is possible that since it
                     * unblocked another task has set the bits.  If this is the case
                     * then it needs to clear the bits before exiting. */
                    यदि( ( uxReturn & ux_द्वयङ्काः_गन्तव्यम्_Wait_For ) == ux_द्वयङ्काः_गन्तव्यम्_Wait_For )
                    {
                        px_Event_द्वयङ्काः->ux_Event_द्वयङ्काः &= ~ux_द्वयङ्काः_गन्तव्यम्_Wait_For;
                    }
                    अन्यथा
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                taskEXIT_CRITICAL();

                xTimeoutOccurred = pdTRUE;
            }
            अन्यथा
            {
                /* The task unblocked because the bits were set. */
            }

            /* Control bits might be set as the task had blocked should not be
             * returned. */
            uxReturn &= ~event_EVENT_द्वयङ्काः_CONTROL_अष्टकानि;
        }

        trace_EVENT_गणः_SYNC_अन्तः( x_Event_गणः, ux_द्वयङ्काः_गन्तव्यम्_समुच्चयः, ux_द्वयङ्काः_गन्तव्यम्_Wait_For, xTimeoutOccurred );

        /* Prevent compiler warnings when trace macros are not used. */
        ( शून्य ) xTimeoutOccurred;

        trace_RETURN_x_Event_गणः_Sync( uxReturn );

        प्रत्यागम uxReturn;
    }
/*-----------------------------------------------------------*/

    Event_द्वयङ्काः_t x_Event_गणः_Wait_द्वयङ्काः( Event_गणः_Handle_t x_Event_गणः,
                                     ध्रुव Event_द्वयङ्काः_t ux_द्वयङ्काः_गन्तव्यम्_Wait_For,
                                     ध्रुव आधारः_Type_t xClearOnExit,
                                     ध्रुव आधारः_Type_t x_Wait_For_All_द्वयङ्काः,
                                     TickType_t x_Ticks_गन्तव्यम्_Wait )
    {
        Event_गणः_t * px_Event_द्वयङ्काः = x_Event_गणः;
        Event_द्वयङ्काः_t uxReturn, ux_Control_द्वयङ्काः = 0;
        आधारः_Type_t xWaitConditionMet, xAlreadyYielded;
        आधारः_Type_t xTimeoutOccurred = pdFALSE;

        trace_ENTER_x_Event_गणः_Wait_द्वयङ्काः( x_Event_गणः, ux_द्वयङ्काः_गन्तव्यम्_Wait_For, xClearOnExit, x_Wait_For_All_द्वयङ्काः, x_Ticks_गन्तव्यम्_Wait );

        /* Check the user is not attempting to wait on the bits used by the kernel
         * itself, and that at least one bit is being requested. */
        configASSERT( x_Event_गणः );
        configASSERT( ( ux_द्वयङ्काः_गन्तव्यम्_Wait_For & event_EVENT_द्वयङ्काः_CONTROL_अष्टकानि ) == 0 );
        configASSERT( ux_द्वयङ्काः_गन्तव्यम्_Wait_For != 0 );
        #यदि ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
        {
            configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_गन्तव्यम्_Wait != 0 ) ) );
        }
        #यदि_अन्तः

        vTaskSuspendAll();
        {
            ध्रुव Event_द्वयङ्काः_t ux_Current_Event_द्वयङ्काः = px_Event_द्वयङ्काः->ux_Event_द्वयङ्काः;

            /* Check to see if the wait condition is already met or not. */
            xWaitConditionMet = prvTestWaitCondition( ux_Current_Event_द्वयङ्काः, ux_द्वयङ्काः_गन्तव्यम्_Wait_For, x_Wait_For_All_द्वयङ्काः );

            यदि( xWaitConditionMet != pdFALSE )
            {
                /* The wait condition has already been met so there is no need to
                 * block. */
                uxReturn = ux_Current_Event_द्वयङ्काः;
                x_Ticks_गन्तव्यम्_Wait = ( TickType_t ) 0;

                /* Clear the wait bits if requested to do so. */
                यदि( xClearOnExit != pdFALSE )
                {
                    px_Event_द्वयङ्काः->ux_Event_द्वयङ्काः &= ~ux_द्वयङ्काः_गन्तव्यम्_Wait_For;
                }
                अन्यथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            अन्यथा यदि( x_Ticks_गन्तव्यम्_Wait == ( TickType_t ) 0 )
            {
                /* The wait condition has not been met, but no block time was
                 * specified, so just return the current value. */
                uxReturn = ux_Current_Event_द्वयङ्काः;
                xTimeoutOccurred = pdTRUE;
            }
            अन्यथा
            {
                /* The task is going to block to wait for its required bits to be
                 * set.  uxControlBits are used to remember the specified behaviour of
                 * this call to xEventGroupWaitBits() - for use when the event bits
                 * unblock the task. */
                यदि( xClearOnExit != pdFALSE )
                {
                    ux_Control_द्वयङ्काः |= eventCLEAR_EVENTS_ON_EXIT_BIT;
                }
                अन्यथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                यदि( x_Wait_For_All_द्वयङ्काः != pdFALSE )
                {
                    ux_Control_द्वयङ्काः |= event_WAIT_FOR_ALL_द्वयङ्काः;
                }
                अन्यथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                /* Store the bits that the calling task is waiting for in the
                 * task's event list item so the kernel knows when a match is
                 * found.  Then enter the blocked state. */
                vTaskPlaceOnUnorderedEventList( &( px_Event_द्वयङ्काः->x_Tasks_प्रतीक्षा_For_द्वयङ्काः ), ( ux_द्वयङ्काः_गन्तव्यम्_Wait_For | ux_Control_द्वयङ्काः ), x_Ticks_गन्तव्यम्_Wait );

                /* This is obsolete as it will get set after the task unblocks, but
                 * some compilers mistakenly generate a warning about the variable
                 * being returned without being set if it is not done. */
                uxReturn = 0;

                trace_EVENT_गणः_WAIT_द्वयङ्काः_खण्डम्( x_Event_गणः, ux_द्वयङ्काः_गन्तव्यम्_Wait_For );
            }
        }
        xAlreadyYielded = xTaskResumeAll();

        यदि( x_Ticks_गन्तव्यम्_Wait != ( TickType_t ) 0 )
        {
            यदि( xAlreadyYielded == pdFALSE )
            {
                taskYIELD_WITHIN_API();
            }
            अन्यथा
            {
                mtCOVERAGE_TEST_MARKER();
            }

            /* The task blocked to wait for its required bits to be set - at this
             * point either the required bits were set or the block time expired.  If
             * the required bits were set they will have been stored in the task's
             * event list item, and they should now be retrieved then cleared. */
            uxReturn = ux_Task_Reset_Event_Item_मानम्();

            यदि( ( uxReturn & event_UNBLOCKED_DUE_गन्तव्यम्_BIT_समुच्चयः ) == ( Event_द्वयङ्काः_t ) 0 )
            {
                taskENTER_CRITICAL();
                {
                    /* The task timed out, just return the current event bit value. */
                    uxReturn = px_Event_द्वयङ्काः->ux_Event_द्वयङ्काः;

                    /* It is possible that the event bits were updated between this
                     * task leaving the Blocked state and running again. */
                    यदि( prvTestWaitCondition( uxReturn, ux_द्वयङ्काः_गन्तव्यम्_Wait_For, x_Wait_For_All_द्वयङ्काः ) != pdFALSE )
                    {
                        यदि( xClearOnExit != pdFALSE )
                        {
                            px_Event_द्वयङ्काः->ux_Event_द्वयङ्काः &= ~ux_द्वयङ्काः_गन्तव्यम्_Wait_For;
                        }
                        अन्यथा
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    अन्यथा
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }

                    xTimeoutOccurred = pdTRUE;
                }
                taskEXIT_CRITICAL();
            }
            अन्यथा
            {
                /* The task unblocked because the bits were set. */
            }

            /* The task blocked so control bits may have been set. */
            uxReturn &= ~event_EVENT_द्वयङ्काः_CONTROL_अष्टकानि;
        }

        trace_EVENT_गणः_WAIT_द्वयङ्काः_अन्तः( x_Event_गणः, ux_द्वयङ्काः_गन्तव्यम्_Wait_For, xTimeoutOccurred );

        /* Prevent compiler warnings when trace macros are not used. */
        ( शून्य ) xTimeoutOccurred;

        trace_RETURN_x_Event_गणः_Wait_द्वयङ्काः( uxReturn );

        प्रत्यागम uxReturn;
    }
/*-----------------------------------------------------------*/

    Event_द्वयङ्काः_t x_Event_गणः_Clear_द्वयङ्काः( Event_गणः_Handle_t x_Event_गणः,
                                      ध्रुव Event_द्वयङ्काः_t ux_द्वयङ्काः_गन्तव्यम्_Clear )
    {
        Event_गणः_t * px_Event_द्वयङ्काः = x_Event_गणः;
        Event_द्वयङ्काः_t uxReturn;

        trace_ENTER_x_Event_गणः_Clear_द्वयङ्काः( x_Event_गणः, ux_द्वयङ्काः_गन्तव्यम्_Clear );

        /* Check the user is not attempting to clear the bits used by the kernel
         * itself. */
        configASSERT( x_Event_गणः );
        configASSERT( ( ux_द्वयङ्काः_गन्तव्यम्_Clear & event_EVENT_द्वयङ्काः_CONTROL_अष्टकानि ) == 0 );

        taskENTER_CRITICAL();
        {
            trace_EVENT_गणः_CLEAR_द्वयङ्काः( x_Event_गणः, ux_द्वयङ्काः_गन्तव्यम्_Clear );

            /* The value returned is the event group value prior to the bits being
             * cleared. */
            uxReturn = px_Event_द्वयङ्काः->ux_Event_द्वयङ्काः;

            /* Clear the bits. */
            px_Event_द्वयङ्काः->ux_Event_द्वयङ्काः &= ~ux_द्वयङ्काः_गन्तव्यम्_Clear;
        }
        taskEXIT_CRITICAL();

        trace_RETURN_x_Event_गणः_Clear_द्वयङ्काः( uxReturn );

        प्रत्यागम uxReturn;
    }
/*-----------------------------------------------------------*/

    #यदि ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) )

        आधारः_Type_t x_Event_गणः_Clear_द्वयङ्काः_From_ISR( Event_गणः_Handle_t x_Event_गणः,
                                                ध्रुव Event_द्वयङ्काः_t ux_द्वयङ्काः_गन्तव्यम्_Clear )
        {
            आधारः_Type_t xReturn;

            trace_ENTER_x_Event_गणः_Clear_द्वयङ्काः_From_ISR( x_Event_गणः, ux_द्वयङ्काः_गन्तव्यम्_Clear );

            trace_EVENT_गणः_CLEAR_द्वयङ्काः_FROM_ISR( x_Event_गणः, ux_द्वयङ्काः_गन्तव्यम्_Clear );
            xReturn = xTimerPendFunctionCallFromISR( v_Event_गणः_Clear_द्वयङ्काः_Callback, ( शून्य * ) x_Event_गणः, ( uint32_t ) ux_द्वयङ्काः_गन्तव्यम्_Clear, रिक्त );

            trace_RETURN_x_Event_गणः_Clear_द्वयङ्काः_From_ISR( xReturn );

            प्रत्यागम xReturn;
        }

    #यदि_अन्तः /* if ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) ) */
/*-----------------------------------------------------------*/

    Event_द्वयङ्काः_t x_Event_गणः_Get_द्वयङ्काः_From_ISR( Event_गणः_Handle_t x_Event_गणः )
    {
        UBaseType_t ux_Saved_व्यवधानम्_स्थितिः;
        Event_गणः_t ध्रुव * ध्रुव px_Event_द्वयङ्काः = x_Event_गणः;
        Event_द्वयङ्काः_t uxReturn;

        trace_ENTER_x_Event_गणः_Get_द्वयङ्काः_From_ISR( x_Event_गणः );

        /* MISRA Ref 4.7.1 [Return value shall be checked] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
        /* coverity[misra_c_2012_directive_4_7_violation] */
        ux_Saved_व्यवधानम्_स्थितिः = taskENTER_CRITICAL_FROM_ISR();
        {
            uxReturn = px_Event_द्वयङ्काः->ux_Event_द्वयङ्काः;
        }
        taskEXIT_CRITICAL_FROM_ISR( ux_Saved_व्यवधानम्_स्थितिः );

        trace_RETURN_x_Event_गणः_Get_द्वयङ्काः_From_ISR( uxReturn );

        प्रत्यागम uxReturn;
    }
/*-----------------------------------------------------------*/

    Event_द्वयङ्काः_t x_Event_गणः_समुच्चयः_द्वयङ्काः( Event_गणः_Handle_t x_Event_गणः,
                                    ध्रुव Event_द्वयङ्काः_t ux_द्वयङ्काः_गन्तव्यम्_समुच्चयः )
    {
        ListItem_t * pxListItem;
        ListItem_t * px_अग्र;
        ListItem_t ध्रुव * px_List_अन्तः;
        List_t ध्रुव * pxList;
        Event_द्वयङ्काः_t ux_द्वयङ्काः_गन्तव्यम्_Clear = 0, ux_द्वयङ्काः_Waited_For, ux_Control_द्वयङ्काः;
        Event_गणः_t * px_Event_द्वयङ्काः = x_Event_गणः;
        आधारः_Type_t x_मेलनम्_प्राप्तम् = pdFALSE;

        trace_ENTER_x_Event_गणः_समुच्चयः_द्वयङ्काः( x_Event_गणः, ux_द्वयङ्काः_गन्तव्यम्_समुच्चयः );

        /* Check the user is not attempting to set the bits used by the kernel
         * itself. */
        configASSERT( x_Event_गणः );
        configASSERT( ( ux_द्वयङ्काः_गन्तव्यम्_समुच्चयः & event_EVENT_द्वयङ्काः_CONTROL_अष्टकानि ) == 0 );

        pxList = &( px_Event_द्वयङ्काः->x_Tasks_प्रतीक्षा_For_द्वयङ्काः );
        px_List_अन्तः = list_GET_अन्तः_MARKER( pxList );
        vTaskSuspendAll();
        {
            trace_EVENT_गणः_समुच्चयः_द्वयङ्काः( x_Event_गणः, ux_द्वयङ्काः_गन्तव्यम्_समुच्चयः );

            pxListItem = list_GET_शीर्ष_प्रविष्टिः( pxList );

            /* Set the bits. */
            px_Event_द्वयङ्काः->ux_Event_द्वयङ्काः |= ux_द्वयङ्काः_गन्तव्यम्_समुच्चयः;

            /* See if the new bit value should unblock any tasks. */
            यावत्( pxListItem != px_List_अन्तः )
            {
                px_अग्र = list_GET_अग्र( pxListItem );
                ux_द्वयङ्काः_Waited_For = list_GET_LIST_ITEM_मानम्( pxListItem );
                x_मेलनम्_प्राप्तम् = pdFALSE;

                /* Split the bits waited for from the control bits. */
                ux_Control_द्वयङ्काः = ux_द्वयङ्काः_Waited_For & event_EVENT_द्वयङ्काः_CONTROL_अष्टकानि;
                ux_द्वयङ्काः_Waited_For &= ~event_EVENT_द्वयङ्काः_CONTROL_अष्टकानि;

                यदि( ( ux_Control_द्वयङ्काः & event_WAIT_FOR_ALL_द्वयङ्काः ) == ( Event_द्वयङ्काः_t ) 0 )
                {
                    /* Just looking for single bit being set. */
                    यदि( ( ux_द्वयङ्काः_Waited_For & px_Event_द्वयङ्काः->ux_Event_द्वयङ्काः ) != ( Event_द्वयङ्काः_t ) 0 )
                    {
                        x_मेलनम्_प्राप्तम् = pdTRUE;
                    }
                    अन्यथा
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                अन्यथा यदि( ( ux_द्वयङ्काः_Waited_For & px_Event_द्वयङ्काः->ux_Event_द्वयङ्काः ) == ux_द्वयङ्काः_Waited_For )
                {
                    /* All bits are set. */
                    x_मेलनम्_प्राप्तम् = pdTRUE;
                }
                अन्यथा
                {
                    /* Need all bits to be set, but not all the bits were set. */
                }

                यदि( x_मेलनम्_प्राप्तम् != pdFALSE )
                {
                    /* The bits match.  Should the bits be cleared on exit? */
                    यदि( ( ux_Control_द्वयङ्काः & eventCLEAR_EVENTS_ON_EXIT_BIT ) != ( Event_द्वयङ्काः_t ) 0 )
                    {
                        ux_द्वयङ्काः_गन्तव्यम्_Clear |= ux_द्वयङ्काः_Waited_For;
                    }
                    अन्यथा
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }

                    /* Store the actual event flag value in the task's event list
                     * item before removing the task from the event list.  The
                     * eventUNBLOCKED_DUE_TO_BIT_SET bit is set so the task knows
                     * that is was unblocked due to its required bits matching, rather
                     * than because it timed out. */
                    vTaskRemoveFromUnorderedEventList( pxListItem, px_Event_द्वयङ्काः->ux_Event_द्वयङ्काः | event_UNBLOCKED_DUE_गन्तव्यम्_BIT_समुच्चयः );
                }

                /* Move onto the next list item.  Note pxListItem->pxNext is not
                 * used here as the list item may have been removed from the event list
                 * and inserted into the ready/pending reading list. */
                pxListItem = px_अग्र;
            }

            /* Clear any bits that matched when the eventCLEAR_EVENTS_ON_EXIT_BIT
             * bit was set in the control word. */
            px_Event_द्वयङ्काः->ux_Event_द्वयङ्काः &= ~ux_द्वयङ्काः_गन्तव्यम्_Clear;
        }
        ( शून्य ) xTaskResumeAll();

        trace_RETURN_x_Event_गणः_समुच्चयः_द्वयङ्काः( px_Event_द्वयङ्काः->ux_Event_द्वयङ्काः );

        प्रत्यागम px_Event_द्वयङ्काः->ux_Event_द्वयङ्काः;
    }
/*-----------------------------------------------------------*/

    शून्य v_Event_गणः_Delete( Event_गणः_Handle_t x_Event_गणः )
    {
        Event_गणः_t * px_Event_द्वयङ्काः = x_Event_गणः;
        ध्रुव List_t * px_Tasks_प्रतीक्षा_For_द्वयङ्काः;

        trace_ENTER_v_Event_गणः_Delete( x_Event_गणः );

        configASSERT( px_Event_द्वयङ्काः );

        px_Tasks_प्रतीक्षा_For_द्वयङ्काः = &( px_Event_द्वयङ्काः->x_Tasks_प्रतीक्षा_For_द्वयङ्काः );

        vTaskSuspendAll();
        {
            trace_EVENT_गणः_DELETE( x_Event_गणः );

            यावत्( list_CURRENT_LIST_दीर्घता( px_Tasks_प्रतीक्षा_For_द्वयङ्काः ) > ( UBaseType_t ) 0 )
            {
                /* Unblock the task, returning 0 as the event list is being deleted
                 * and cannot therefore have any bits set. */
                configASSERT( px_Tasks_प्रतीक्षा_For_द्वयङ्काः->x_List_अन्तः.px_अग्र != ( ध्रुव ListItem_t * ) &( px_Tasks_प्रतीक्षा_For_द्वयङ्काः->x_List_अन्तः ) );
                vTaskRemoveFromUnorderedEventList( px_Tasks_प्रतीक्षा_For_द्वयङ्काः->x_List_अन्तः.px_अग्र, event_UNBLOCKED_DUE_गन्तव्यम्_BIT_समुच्चयः );
            }
        }
        ( शून्य ) xTaskResumeAll();

        #यदि ( ( config_SUPPORT_DYNAMIC_आवण्टनम् == 1 ) && ( config_SUPPORT_STATIC_आवण्टनम् == 0 ) )
        {
            /* The event group can only have been allocated dynamically - free
             * it again. */
            v_पत्तनम्_Free( px_Event_द्वयङ्काः );
        }
        #नोचेद्यदि ( ( config_SUPPORT_DYNAMIC_आवण्टनम् == 1 ) && ( config_SUPPORT_STATIC_आवण्टनम् == 1 ) )
        {
            /* The event group could have been allocated statically or
             * dynamically, so check before attempting to free the memory. */
            यदि( px_Event_द्वयङ्काः->ucStaticallyAllocated == ( uint8_t ) pdFALSE )
            {
                v_पत्तनम्_Free( px_Event_द्वयङ्काः );
            }
            अन्यथा
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        #यदि_अन्तः /* configSUPPORT_DYNAMIC_ALLOCATION */

        trace_RETURN_v_Event_गणः_Delete();
    }
/*-----------------------------------------------------------*/

    #यदि ( config_SUPPORT_STATIC_आवण्टनम् == 1 )
        आधारः_Type_t x_Event_गणः_Get_Static_बफर( Event_गणः_Handle_t x_Event_गणः,
                                               Static_Event_गणः_t ** ppx_Event_गणः_बफर )
        {
            आधारः_Type_t xReturn;
            Event_गणः_t * px_Event_द्वयङ्काः = x_Event_गणः;

            trace_ENTER_x_Event_गणः_Get_Static_बफर( x_Event_गणः, ppx_Event_गणः_बफर );

            configASSERT( px_Event_द्वयङ्काः );
            configASSERT( ppx_Event_गणः_बफर );

            #यदि ( config_SUPPORT_DYNAMIC_आवण्टनम् == 1 )
            {
                /* Check if the event group was statically allocated. */
                यदि( px_Event_द्वयङ्काः->ucStaticallyAllocated == ( uint8_t ) pdTRUE )
                {
                    /* MISRA Ref 11.3.1 [Misaligned access] */
                    /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
                    /* coverity[misra_c_2012_rule_11_3_violation] */
                    *ppx_Event_गणः_बफर = ( Static_Event_गणः_t * ) px_Event_द्वयङ्काः;
                    xReturn = pdTRUE;
                }
                अन्यथा
                {
                    xReturn = pdFALSE;
                }
            }
            #अन्यथा /* configSUPPORT_DYNAMIC_ALLOCATION */
            {
                /* Event group must have been statically allocated. */
                /* MISRA Ref 11.3.1 [Misaligned access] */
                /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
                /* coverity[misra_c_2012_rule_11_3_violation] */
                *ppx_Event_गणः_बफर = ( Static_Event_गणः_t * ) px_Event_द्वयङ्काः;
                xReturn = pdTRUE;
            }
            #यदि_अन्तः /* configSUPPORT_DYNAMIC_ALLOCATION */

            trace_RETURN_x_Event_गणः_Get_Static_बफर( xReturn );

            प्रत्यागम xReturn;
        }
    #यदि_अन्तः /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

/* For internal use only - execute a 'set bits' command that was pended from
 * an interrupt. */
    शून्य v_Event_गणः_समुच्चयः_द्वयङ्काः_Callback( शून्य * pv_Event_गणः,
                                     uint32_t ul_द्वयङ्काः_गन्तव्यम्_समुच्चयः )
    {
        trace_ENTER_v_Event_गणः_समुच्चयः_द्वयङ्काः_Callback( pv_Event_गणः, ul_द्वयङ्काः_गन्तव्यम्_समुच्चयः );

        /* MISRA Ref 11.5.4 [Callback function parameter] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
        /* coverity[misra_c_2012_rule_11_5_violation] */
        ( शून्य ) x_Event_गणः_समुच्चयः_द्वयङ्काः( pv_Event_गणः, ( Event_द्वयङ्काः_t ) ul_द्वयङ्काः_गन्तव्यम्_समुच्चयः );

        trace_RETURN_v_Event_गणः_समुच्चयः_द्वयङ्काः_Callback();
    }
/*-----------------------------------------------------------*/

/* For internal use only - execute a 'clear bits' command that was pended from
 * an interrupt. */
    शून्य v_Event_गणः_Clear_द्वयङ्काः_Callback( शून्य * pv_Event_गणः,
                                       uint32_t ul_द्वयङ्काः_गन्तव्यम्_Clear )
    {
        trace_ENTER_v_Event_गणः_Clear_द्वयङ्काः_Callback( pv_Event_गणः, ul_द्वयङ्काः_गन्तव्यम्_Clear );

        /* MISRA Ref 11.5.4 [Callback function parameter] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
        /* coverity[misra_c_2012_rule_11_5_violation] */
        ( शून्य ) x_Event_गणः_Clear_द्वयङ्काः( pv_Event_गणः, ( Event_द्वयङ्काः_t ) ul_द्वयङ्काः_गन्तव्यम्_Clear );

        trace_RETURN_v_Event_गणः_Clear_द्वयङ्काः_Callback();
    }
/*-----------------------------------------------------------*/

    स्थिर आधारः_Type_t prvTestWaitCondition( ध्रुव Event_द्वयङ्काः_t ux_Current_Event_द्वयङ्काः,
                                            ध्रुव Event_द्वयङ्काः_t ux_द्वयङ्काः_गन्तव्यम्_Wait_For,
                                            ध्रुव आधारः_Type_t x_Wait_For_All_द्वयङ्काः )
    {
        आधारः_Type_t xWaitConditionMet = pdFALSE;

        यदि( x_Wait_For_All_द्वयङ्काः == pdFALSE )
        {
            /* Task only has to wait for one bit within uxBitsToWaitFor to be
             * set.  Is one already set? */
            यदि( ( ux_Current_Event_द्वयङ्काः & ux_द्वयङ्काः_गन्तव्यम्_Wait_For ) != ( Event_द्वयङ्काः_t ) 0 )
            {
                xWaitConditionMet = pdTRUE;
            }
            अन्यथा
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        अन्यथा
        {
            /* Task has to wait for all the bits in uxBitsToWaitFor to be set.
             * Are they set already? */
            यदि( ( ux_Current_Event_द्वयङ्काः & ux_द्वयङ्काः_गन्तव्यम्_Wait_For ) == ux_द्वयङ्काः_गन्तव्यम्_Wait_For )
            {
                xWaitConditionMet = pdTRUE;
            }
            अन्यथा
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }

        प्रत्यागम xWaitConditionMet;
    }
/*-----------------------------------------------------------*/

    #यदि ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) )

        आधारः_Type_t x_Event_गणः_समुच्चयः_द्वयङ्काः_From_ISR( Event_गणः_Handle_t x_Event_गणः,
                                              ध्रुव Event_द्वयङ्काः_t ux_द्वयङ्काः_गन्तव्यम्_समुच्चयः,
                                              आधारः_Type_t * px_Higher_प्राथमिकता_Task_Woken )
        {
            आधारः_Type_t xReturn;

            trace_ENTER_x_Event_गणः_समुच्चयः_द्वयङ्काः_From_ISR( x_Event_गणः, ux_द्वयङ्काः_गन्तव्यम्_समुच्चयः, px_Higher_प्राथमिकता_Task_Woken );

            trace_EVENT_गणः_समुच्चयः_द्वयङ्काः_FROM_ISR( x_Event_गणः, ux_द्वयङ्काः_गन्तव्यम्_समुच्चयः );
            xReturn = xTimerPendFunctionCallFromISR( v_Event_गणः_समुच्चयः_द्वयङ्काः_Callback, ( शून्य * ) x_Event_गणः, ( uint32_t ) ux_द्वयङ्काः_गन्तव्यम्_समुच्चयः, px_Higher_प्राथमिकता_Task_Woken );

            trace_RETURN_x_Event_गणः_समुच्चयः_द्वयङ्काः_From_ISR( xReturn );

            प्रत्यागम xReturn;
        }

    #यदि_अन्तः /* if ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) ) */
/*-----------------------------------------------------------*/

    #यदि ( configUSE_TRACE_FACILITY == 1 )

        UBaseType_t ux_Event_गणः_Get_सङ्ख्या( शून्य * x_Event_गणः )
        {
            UBaseType_t xReturn;

            /* MISRA Ref 11.5.2 [Opaque pointer] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
            /* coverity[misra_c_2012_rule_11_5_violation] */
            Event_गणः_t ध्रुव * px_Event_द्वयङ्काः = ( Event_गणः_t * ) x_Event_गणः;

            trace_ENTER_ux_Event_गणः_Get_सङ्ख्या( x_Event_गणः );

            यदि( x_Event_गणः == रिक्त )
            {
                xReturn = 0;
            }
            अन्यथा
            {
                xReturn = px_Event_द्वयङ्काः->ux_Event_गणः_सङ्ख्या;
            }

            trace_RETURN_ux_Event_गणः_Get_सङ्ख्या( xReturn );

            प्रत्यागम xReturn;
        }

    #यदि_अन्तः /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

    #यदि ( configUSE_TRACE_FACILITY == 1 )

        शून्य v_Event_गणः_समुच्चयः_सङ्ख्या( शून्य * x_Event_गणः,
                                   UBaseType_t ux_Event_गणः_सङ्ख्या )
        {
            trace_ENTER_v_Event_गणः_समुच्चयः_सङ्ख्या( x_Event_गणः, ux_Event_गणः_सङ्ख्या );

            /* MISRA Ref 11.5.2 [Opaque pointer] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
            /* coverity[misra_c_2012_rule_11_5_violation] */
            ( ( Event_गणः_t * ) x_Event_गणः )->ux_Event_गणः_सङ्ख्या = ux_Event_गणः_सङ्ख्या;

            trace_RETURN_v_Event_गणः_समुच्चयः_सङ्ख्या();
        }

    #यदि_अन्तः /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

/* This entire source file will be skipped if the application is not configured
 * to include event groups functionality. If you want to include event groups
 * then ensure configUSE_EVENT_GROUPS is set to 1 in FreeRTOSConfig.h. */
#यदि_अन्तः /* configUSE_EVENT_GROUPS == 1 */
