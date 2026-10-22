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
#ಅನ್ತರ್ಭಾವ <stdlib.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers. That should only be done when
 * task.h is included from an application file. */
#ವ್ಯಾಖ್ಯಾನಿಸು MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* FreeRTOS includes. */
#ಅನ್ತರ್ಭಾವ "FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "task.h"
#ಅನ್ತರ್ಭಾವ "timers.h"
#ಅನ್ತರ್ಭಾವ "event_groups.h"

/* The MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* This entire source file will be skipped if the application is not configured
 * to include event groups functionality. This #if is closed at the very bottom
 * of this file. If you want to include event groups then ensure
 * configUSE_EVENT_GROUPS is set to 1 in FreeRTOSConfig.h. */
#ಆದರೆ ( configUSE_EVENT_GROUPS == 1 )

    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ Event_ಗುಂಪು_Def_t
    {
        Event_ಬಿಟ್ಟುಗಳು_t ux_Event_ಬಿಟ್ಟುಗಳು;
        List_t x_Tasks_ಕಾಯುವಿಕೆ_For_ಬಿಟ್ಟುಗಳು; /**< List of tasks waiting for a bit to be set. */

        #ಆದರೆ ( configUSE_TRACE_FACILITY == 1 )
            UBaseType_t ux_Event_ಗುಂಪು_ಸಂಖ್ಯೆ;
        #ಆದರೆ_ಅಂತ್ಯ

        #ಆದರೆ ( ( config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 1 ) && ( config_SUPPORT_DYNAMIC_ಹಂಚಿಕೆ == 1 ) )
            uint8_t ucStaticallyAllocated; /**< Set to pdTRUE if the event group is statically allocated to ensure no attempt is made to free the memory. */
        #ಆದರೆ_ಅಂತ್ಯ
    } Event_ಗುಂಪು_t;

/*-----------------------------------------------------------*/

/*
 * Test the bits set in uxCurrentEventBits to see if the wait condition is met.
 * The wait condition is defined by xWaitForAllBits.  If xWaitForAllBits is
 * pdTRUE then the wait condition is met if all the bits set in uxBitsToWaitFor
 * are also set in uxCurrentEventBits.  If xWaitForAllBits is pdFALSE then the
 * wait condition is met if any of the bits set in uxBitsToWait for are also set
 * in uxCurrentEventBits.
 */
    ಸ್ಥಿರ ತಳ_Type_t prvTestWaitCondition( ಸ್ಥಿರಾಂಕ Event_ಬಿಟ್ಟುಗಳು_t ux_Current_Event_ಬಿಟ್ಟುಗಳು,
                                            ಸ್ಥಿರಾಂಕ Event_ಬಿಟ್ಟುಗಳು_t ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Wait_For,
                                            ಸ್ಥಿರಾಂಕ ತಳ_Type_t x_Wait_For_All_ಬಿಟ್ಟುಗಳು ) PRIVILEGED_FUNCTION;

/*-----------------------------------------------------------*/

    #ಆದರೆ ( config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 1 )

        Event_ಗುಂಪು_Handle_t x_Event_ಗುಂಪು_Create_Static( Static_Event_ಗುಂಪು_t * px_Event_ಗುಂಪು_ಬಫರ್ )
        {
            Event_ಗುಂಪು_t * px_Event_ಬಿಟ್ಟುಗಳು;

            trace_ENTER_x_Event_ಗುಂಪು_Create_Static( px_Event_ಗುಂಪು_ಬಫರ್ );

            /* A StaticEventGroup_t object must be provided. */
            configASSERT( px_Event_ಗುಂಪು_ಬಫರ್ );

            #ಆದರೆ ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticEventGroup_t equals the size of the real
                 * event group structure. */
                ಅಸ್ಥಿರ size_t x_ಗಾತ್ರ = ಗಾತ್ರ( Static_Event_ಗುಂಪು_t );
                configASSERT( x_ಗಾತ್ರ == ಗಾತ್ರ( Event_ಗುಂಪು_t ) );
            }
            #ಆದರೆ_ಅಂತ್ಯ /* configASSERT_DEFINED */

            /* The user has provided a statically allocated event group - use it. */
            /* MISRA Ref 11.3.1 [Misaligned access] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
            /* coverity[misra_c_2012_rule_11_3_violation] */
            px_Event_ಬಿಟ್ಟುಗಳು = ( Event_ಗುಂಪು_t * ) px_Event_ಗುಂಪು_ಬಫರ್;

            ಆದರೆ( px_Event_ಬಿಟ್ಟುಗಳು != ಖಾಲಿ )
            {
                px_Event_ಬಿಟ್ಟುಗಳು->ux_Event_ಬಿಟ್ಟುಗಳು = 0;
                vListInitialise( &( px_Event_ಬಿಟ್ಟುಗಳು->x_Tasks_ಕಾಯುವಿಕೆ_For_ಬಿಟ್ಟುಗಳು ) );

                #ಆದರೆ ( config_SUPPORT_DYNAMIC_ಹಂಚಿಕೆ == 1 )
                {
                    /* Both static and dynamic allocation can be used, so note that
                     * this event group was created statically in case the event group
                     * is later deleted. */
                    px_Event_ಬಿಟ್ಟುಗಳು->ucStaticallyAllocated = pdTRUE;
                }
                #ಆದರೆ_ಅಂತ್ಯ /* configSUPPORT_DYNAMIC_ALLOCATION */

                trace_EVENT_ಗುಂಪು_CREATE( px_Event_ಬಿಟ್ಟುಗಳು );
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                /* xEventGroupCreateStatic should only ever be called with
                 * pxEventGroupBuffer pointing to a pre-allocated (compile time
                 * allocated) StaticEventGroup_t variable. */
                trace_EVENT_ಗುಂಪು_CREATE_FAILED();
            }

            trace_RETURN_x_Event_ಗುಂಪು_Create_Static( px_Event_ಬಿಟ್ಟುಗಳು );

            ಹಿಂತಿರುಗಿ px_Event_ಬಿಟ್ಟುಗಳು;
        }

    #ಆದರೆ_ಅಂತ್ಯ /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

    #ಆದರೆ ( config_SUPPORT_DYNAMIC_ಹಂಚಿಕೆ == 1 )

        Event_ಗುಂಪು_Handle_t x_Event_ಗುಂಪು_Create( ಶೂನ್ಯ )
        {
            Event_ಗುಂಪು_t * px_Event_ಬಿಟ್ಟುಗಳು;

            trace_ENTER_x_Event_ಗುಂಪು_Create();

            /* MISRA Ref 11.5.1 [Malloc memory assignment] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
            /* coverity[misra_c_2012_rule_11_5_violation] */
            px_Event_ಬಿಟ್ಟುಗಳು = ( Event_ಗುಂಪು_t * ) pv_ಪೋರ್ಟ್_Malloc( ಗಾತ್ರ( Event_ಗುಂಪು_t ) );

            ಆದರೆ( px_Event_ಬಿಟ್ಟುಗಳು != ಖಾಲಿ )
            {
                px_Event_ಬಿಟ್ಟುಗಳು->ux_Event_ಬಿಟ್ಟುಗಳು = 0;
                vListInitialise( &( px_Event_ಬಿಟ್ಟುಗಳು->x_Tasks_ಕಾಯುವಿಕೆ_For_ಬಿಟ್ಟುಗಳು ) );

                #ಆದರೆ ( config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 1 )
                {
                    /* Both static and dynamic allocation can be used, so note this
                     * event group was allocated statically in case the event group is
                     * later deleted. */
                    px_Event_ಬಿಟ್ಟುಗಳು->ucStaticallyAllocated = pdFALSE;
                }
                #ಆದರೆ_ಅಂತ್ಯ /* configSUPPORT_STATIC_ALLOCATION */

                trace_EVENT_ಗುಂಪು_CREATE( px_Event_ಬಿಟ್ಟುಗಳು );
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                trace_EVENT_ಗುಂಪು_CREATE_FAILED();
            }

            trace_RETURN_x_Event_ಗುಂಪು_Create( px_Event_ಬಿಟ್ಟುಗಳು );

            ಹಿಂತಿರುಗಿ px_Event_ಬಿಟ್ಟುಗಳು;
        }

    #ಆದರೆ_ಅಂತ್ಯ /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

    Event_ಬಿಟ್ಟುಗಳು_t x_Event_ಗುಂಪು_Sync( Event_ಗುಂಪು_Handle_t x_Event_ಗುಂಪು,
                                 ಸ್ಥಿರಾಂಕ Event_ಬಿಟ್ಟುಗಳು_t ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_ಗಣ,
                                 ಸ್ಥಿರಾಂಕ Event_ಬಿಟ್ಟುಗಳು_t ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Wait_For,
                                 TickType_t x_Ticks_ಗಮ್ಯ_Wait )
    {
        Event_ಬಿಟ್ಟುಗಳು_t ux_Original_ಬಿಟ್_ಮೌಲ್ಯ, uxReturn;
        Event_ಗುಂಪು_t * px_Event_ಬಿಟ್ಟುಗಳು = x_Event_ಗುಂಪು;
        ತಳ_Type_t xAlreadyYielded;
        ತಳ_Type_t xTimeoutOccurred = pdFALSE;

        trace_ENTER_x_Event_ಗುಂಪು_Sync( x_Event_ಗುಂಪು, ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_ಗಣ, ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Wait_For, x_Ticks_ಗಮ್ಯ_Wait );

        configASSERT( ( ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Wait_For & event_EVENT_ಬಿಟ್ಟುಗಳು_CONTROL_ಬೈಟುಗಳು ) == 0 );
        configASSERT( ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Wait_For != 0 );
        #ಆದರೆ ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
        {
            configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_ಗಮ್ಯ_Wait != 0 ) ) );
        }
        #ಆದರೆ_ಅಂತ್ಯ

        vTaskSuspendAll();
        {
            ux_Original_ಬಿಟ್_ಮೌಲ್ಯ = px_Event_ಬಿಟ್ಟುಗಳು->ux_Event_ಬಿಟ್ಟುಗಳು;

            ( ಶೂನ್ಯ ) x_Event_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು( x_Event_ಗುಂಪು, ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_ಗಣ );

            ಆದರೆ( ( ( ux_Original_ಬಿಟ್_ಮೌಲ್ಯ | ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_ಗಣ ) & ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Wait_For ) == ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Wait_For )
            {
                /* All the rendezvous bits are now set - no need to block. */
                uxReturn = ( ux_Original_ಬಿಟ್_ಮೌಲ್ಯ | ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_ಗಣ );

                /* Rendezvous always clear the bits.  They will have been cleared
                 * already unless this is the only task in the rendezvous. */
                px_Event_ಬಿಟ್ಟುಗಳು->ux_Event_ಬಿಟ್ಟುಗಳು &= ~ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Wait_For;

                x_Ticks_ಗಮ್ಯ_Wait = 0;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ಆದರೆ( x_Ticks_ಗಮ್ಯ_Wait != ( TickType_t ) 0 )
                {
                    trace_EVENT_ಗುಂಪು_SYNC_ಖಂಡ( x_Event_ಗುಂಪು, ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_ಗಣ, ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Wait_For );

                    /* Store the bits that the calling task is waiting for in the
                     * task's event list item so the kernel knows when a match is
                     * found.  Then enter the blocked state. */
                    v_Task_ಇರಿಸು_On_Unordered_Event_List( &( px_Event_ಬಿಟ್ಟುಗಳು->x_Tasks_ಕಾಯುವಿಕೆ_For_ಬಿಟ್ಟುಗಳು ), ( ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Wait_For | event_CLEAR_EVENTS_ON_EXIT_ಬಿಟ್ | event_WAIT_FOR_ALL_ಬಿಟ್ಟುಗಳು ), x_Ticks_ಗಮ್ಯ_Wait );

                    /* This assignment is obsolete as uxReturn will get set after
                     * the task unblocks, but some compilers mistakenly generate a
                     * warning about uxReturn being returned without being set if the
                     * assignment is omitted. */
                    uxReturn = 0;
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    /* The rendezvous bits were not set, but no block time was
                     * specified - just return the current event bit value. */
                    uxReturn = px_Event_ಬಿಟ್ಟುಗಳು->ux_Event_ಬಿಟ್ಟುಗಳು;
                    xTimeoutOccurred = pdTRUE;
                }
            }
        }
        xAlreadyYielded = xTaskResumeAll();

        ಆದರೆ( x_Ticks_ಗಮ್ಯ_Wait != ( TickType_t ) 0 )
        {
            ಆದರೆ( xAlreadyYielded == pdFALSE )
            {
                taskYIELD_WITHIN_API();
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                mtCOVERAGE_TEST_MARKER();
            }

            /* The task blocked to wait for its required bits to be set - at this
             * point either the required bits were set or the block time expired.  If
             * the required bits were set they will have been stored in the task's
             * event list item, and they should now be retrieved then cleared. */
            uxReturn = ux_Task_Reset_Event_ಅಂಶ_ಮೌಲ್ಯ();

            ಆದರೆ( ( uxReturn & event_UNBLOCKED_DUE_ಗಮ್ಯ_ಬಿಟ್_ಗಣ ) == ( Event_ಬಿಟ್ಟುಗಳು_t ) 0 )
            {
                /* The task timed out, just return the current event bit value. */
                taskENTER_CRITICAL();
                {
                    uxReturn = px_Event_ಬಿಟ್ಟುಗಳು->ux_Event_ಬಿಟ್ಟುಗಳು;

                    /* Although the task got here because it timed out before the
                     * bits it was waiting for were set, it is possible that since it
                     * unblocked another task has set the bits.  If this is the case
                     * then it needs to clear the bits before exiting. */
                    ಆದರೆ( ( uxReturn & ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Wait_For ) == ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Wait_For )
                    {
                        px_Event_ಬಿಟ್ಟುಗಳು->ux_Event_ಬಿಟ್ಟುಗಳು &= ~ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Wait_For;
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                taskEXIT_CRITICAL();

                xTimeoutOccurred = pdTRUE;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                /* The task unblocked because the bits were set. */
            }

            /* Control bits might be set as the task had blocked should not be
             * returned. */
            uxReturn &= ~event_EVENT_ಬಿಟ್ಟುಗಳು_CONTROL_ಬೈಟುಗಳು;
        }

        trace_EVENT_ಗುಂಪು_SYNC_ಕೊನೆ( x_Event_ಗುಂಪು, ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_ಗಣ, ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Wait_For, xTimeoutOccurred );

        /* Prevent compiler warnings when trace macros are not used. */
        ( ಶೂನ್ಯ ) xTimeoutOccurred;

        trace_RETURN_x_Event_ಗುಂಪು_Sync( uxReturn );

        ಹಿಂತಿರುಗಿ uxReturn;
    }
/*-----------------------------------------------------------*/

    Event_ಬಿಟ್ಟುಗಳು_t x_Event_ಗುಂಪು_Wait_ಬಿಟ್ಟುಗಳು( Event_ಗುಂಪು_Handle_t x_Event_ಗುಂಪು,
                                     ಸ್ಥಿರಾಂಕ Event_ಬಿಟ್ಟುಗಳು_t ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Wait_For,
                                     ಸ್ಥಿರಾಂಕ ತಳ_Type_t xClearOnExit,
                                     ಸ್ಥಿರಾಂಕ ತಳ_Type_t x_Wait_For_All_ಬಿಟ್ಟುಗಳು,
                                     TickType_t x_Ticks_ಗಮ್ಯ_Wait )
    {
        Event_ಗುಂಪು_t * px_Event_ಬಿಟ್ಟುಗಳು = x_Event_ಗುಂಪು;
        Event_ಬಿಟ್ಟುಗಳು_t uxReturn, ux_Control_ಬಿಟ್ಟುಗಳು = 0;
        ತಳ_Type_t xWaitConditionMet, xAlreadyYielded;
        ತಳ_Type_t xTimeoutOccurred = pdFALSE;

        trace_ENTER_x_Event_ಗುಂಪು_Wait_ಬಿಟ್ಟುಗಳು( x_Event_ಗುಂಪು, ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Wait_For, xClearOnExit, x_Wait_For_All_ಬಿಟ್ಟುಗಳು, x_Ticks_ಗಮ್ಯ_Wait );

        /* Check the user is not attempting to wait on the bits used by the kernel
         * itself, and that at least one bit is being requested. */
        configASSERT( x_Event_ಗುಂಪು );
        configASSERT( ( ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Wait_For & event_EVENT_ಬಿಟ್ಟುಗಳು_CONTROL_ಬೈಟುಗಳು ) == 0 );
        configASSERT( ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Wait_For != 0 );
        #ಆದರೆ ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
        {
            configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_ಗಮ್ಯ_Wait != 0 ) ) );
        }
        #ಆದರೆ_ಅಂತ್ಯ

        vTaskSuspendAll();
        {
            ಸ್ಥಿರಾಂಕ Event_ಬಿಟ್ಟುಗಳು_t ux_Current_Event_ಬಿಟ್ಟುಗಳು = px_Event_ಬಿಟ್ಟುಗಳು->ux_Event_ಬಿಟ್ಟುಗಳು;

            /* Check to see if the wait condition is already met or not. */
            xWaitConditionMet = prvTestWaitCondition( ux_Current_Event_ಬಿಟ್ಟುಗಳು, ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Wait_For, x_Wait_For_All_ಬಿಟ್ಟುಗಳು );

            ಆದರೆ( xWaitConditionMet != pdFALSE )
            {
                /* The wait condition has already been met so there is no need to
                 * block. */
                uxReturn = ux_Current_Event_ಬಿಟ್ಟುಗಳು;
                x_Ticks_ಗಮ್ಯ_Wait = ( TickType_t ) 0;

                /* Clear the wait bits if requested to do so. */
                ಆದರೆ( xClearOnExit != pdFALSE )
                {
                    px_Event_ಬಿಟ್ಟುಗಳು->ux_Event_ಬಿಟ್ಟುಗಳು &= ~ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Wait_For;
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( x_Ticks_ಗಮ್ಯ_Wait == ( TickType_t ) 0 )
            {
                /* The wait condition has not been met, but no block time was
                 * specified, so just return the current value. */
                uxReturn = ux_Current_Event_ಬಿಟ್ಟುಗಳು;
                xTimeoutOccurred = pdTRUE;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                /* The task is going to block to wait for its required bits to be
                 * set.  uxControlBits are used to remember the specified behaviour of
                 * this call to xEventGroupWaitBits() - for use when the event bits
                 * unblock the task. */
                ಆದರೆ( xClearOnExit != pdFALSE )
                {
                    ux_Control_ಬಿಟ್ಟುಗಳು |= event_CLEAR_EVENTS_ON_EXIT_ಬಿಟ್;
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                ಆದರೆ( x_Wait_For_All_ಬಿಟ್ಟುಗಳು != pdFALSE )
                {
                    ux_Control_ಬಿಟ್ಟುಗಳು |= event_WAIT_FOR_ALL_ಬಿಟ್ಟುಗಳು;
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                /* Store the bits that the calling task is waiting for in the
                 * task's event list item so the kernel knows when a match is
                 * found.  Then enter the blocked state. */
                v_Task_ಇರಿಸು_On_Unordered_Event_List( &( px_Event_ಬಿಟ್ಟುಗಳು->x_Tasks_ಕಾಯುವಿಕೆ_For_ಬಿಟ್ಟುಗಳು ), ( ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Wait_For | ux_Control_ಬಿಟ್ಟುಗಳು ), x_Ticks_ಗಮ್ಯ_Wait );

                /* This is obsolete as it will get set after the task unblocks, but
                 * some compilers mistakenly generate a warning about the variable
                 * being returned without being set if it is not done. */
                uxReturn = 0;

                trace_EVENT_ಗುಂಪು_WAIT_ಬಿಟ್ಟುಗಳು_ಖಂಡ( x_Event_ಗುಂಪು, ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Wait_For );
            }
        }
        xAlreadyYielded = xTaskResumeAll();

        ಆದರೆ( x_Ticks_ಗಮ್ಯ_Wait != ( TickType_t ) 0 )
        {
            ಆದರೆ( xAlreadyYielded == pdFALSE )
            {
                taskYIELD_WITHIN_API();
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                mtCOVERAGE_TEST_MARKER();
            }

            /* The task blocked to wait for its required bits to be set - at this
             * point either the required bits were set or the block time expired.  If
             * the required bits were set they will have been stored in the task's
             * event list item, and they should now be retrieved then cleared. */
            uxReturn = ux_Task_Reset_Event_ಅಂಶ_ಮೌಲ್ಯ();

            ಆದರೆ( ( uxReturn & event_UNBLOCKED_DUE_ಗಮ್ಯ_ಬಿಟ್_ಗಣ ) == ( Event_ಬಿಟ್ಟುಗಳು_t ) 0 )
            {
                taskENTER_CRITICAL();
                {
                    /* The task timed out, just return the current event bit value. */
                    uxReturn = px_Event_ಬಿಟ್ಟುಗಳು->ux_Event_ಬಿಟ್ಟುಗಳು;

                    /* It is possible that the event bits were updated between this
                     * task leaving the Blocked state and running again. */
                    ಆದರೆ( prvTestWaitCondition( uxReturn, ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Wait_For, x_Wait_For_All_ಬಿಟ್ಟುಗಳು ) != pdFALSE )
                    {
                        ಆದರೆ( xClearOnExit != pdFALSE )
                        {
                            px_Event_ಬಿಟ್ಟುಗಳು->ux_Event_ಬಿಟ್ಟುಗಳು &= ~ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Wait_For;
                        }
                        ಇಲ್ಲದಿದ್ದರೆ
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }

                    xTimeoutOccurred = pdTRUE;
                }
                taskEXIT_CRITICAL();
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                /* The task unblocked because the bits were set. */
            }

            /* The task blocked so control bits may have been set. */
            uxReturn &= ~event_EVENT_ಬಿಟ್ಟುಗಳು_CONTROL_ಬೈಟುಗಳು;
        }

        trace_EVENT_ಗುಂಪು_WAIT_ಬಿಟ್ಟುಗಳು_ಕೊನೆ( x_Event_ಗುಂಪು, ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Wait_For, xTimeoutOccurred );

        /* Prevent compiler warnings when trace macros are not used. */
        ( ಶೂನ್ಯ ) xTimeoutOccurred;

        trace_RETURN_x_Event_ಗುಂಪು_Wait_ಬಿಟ್ಟುಗಳು( uxReturn );

        ಹಿಂತಿರುಗಿ uxReturn;
    }
/*-----------------------------------------------------------*/

    Event_ಬಿಟ್ಟುಗಳು_t x_Event_ಗುಂಪು_Clear_ಬಿಟ್ಟುಗಳು( Event_ಗುಂಪು_Handle_t x_Event_ಗುಂಪು,
                                      ಸ್ಥಿರಾಂಕ Event_ಬಿಟ್ಟುಗಳು_t ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Clear )
    {
        Event_ಗುಂಪು_t * px_Event_ಬಿಟ್ಟುಗಳು = x_Event_ಗುಂಪು;
        Event_ಬಿಟ್ಟುಗಳು_t uxReturn;

        trace_ENTER_x_Event_ಗುಂಪು_Clear_ಬಿಟ್ಟುಗಳು( x_Event_ಗುಂಪು, ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Clear );

        /* Check the user is not attempting to clear the bits used by the kernel
         * itself. */
        configASSERT( x_Event_ಗುಂಪು );
        configASSERT( ( ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Clear & event_EVENT_ಬಿಟ್ಟುಗಳು_CONTROL_ಬೈಟುಗಳು ) == 0 );

        taskENTER_CRITICAL();
        {
            trace_EVENT_ಗುಂಪು_CLEAR_ಬಿಟ್ಟುಗಳು( x_Event_ಗುಂಪು, ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Clear );

            /* The value returned is the event group value prior to the bits being
             * cleared. */
            uxReturn = px_Event_ಬಿಟ್ಟುಗಳು->ux_Event_ಬಿಟ್ಟುಗಳು;

            /* Clear the bits. */
            px_Event_ಬಿಟ್ಟುಗಳು->ux_Event_ಬಿಟ್ಟುಗಳು &= ~ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Clear;
        }
        taskEXIT_CRITICAL();

        trace_RETURN_x_Event_ಗುಂಪು_Clear_ಬಿಟ್ಟುಗಳು( uxReturn );

        ಹಿಂತಿರುಗಿ uxReturn;
    }
/*-----------------------------------------------------------*/

    #ಆದರೆ ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) )

        ತಳ_Type_t x_Event_ಗುಂಪು_Clear_ಬಿಟ್ಟುಗಳು_From_ISR( Event_ಗುಂಪು_Handle_t x_Event_ಗುಂಪು,
                                                ಸ್ಥಿರಾಂಕ Event_ಬಿಟ್ಟುಗಳು_t ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Clear )
        {
            ತಳ_Type_t xReturn;

            trace_ENTER_x_Event_ಗುಂಪು_Clear_ಬಿಟ್ಟುಗಳು_From_ISR( x_Event_ಗುಂಪು, ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Clear );

            trace_EVENT_ಗುಂಪು_CLEAR_ಬಿಟ್ಟುಗಳು_FROM_ISR( x_Event_ಗುಂಪು, ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Clear );
            xReturn = xTimerPendFunctionCallFromISR( v_Event_ಗುಂಪು_Clear_ಬಿಟ್ಟುಗಳು_Callback, ( ಶೂನ್ಯ * ) x_Event_ಗುಂಪು, ( uint32_t ) ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Clear, ಖಾಲಿ );

            trace_RETURN_x_Event_ಗುಂಪು_Clear_ಬಿಟ್ಟುಗಳು_From_ISR( xReturn );

            ಹಿಂತಿರುಗಿ xReturn;
        }

    #ಆದರೆ_ಅಂತ್ಯ /* if ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) ) */
/*-----------------------------------------------------------*/

    Event_ಬಿಟ್ಟುಗಳು_t x_Event_ಗುಂಪು_Get_ಬಿಟ್ಟುಗಳು_From_ISR( Event_ಗುಂಪು_Handle_t x_Event_ಗುಂಪು )
    {
        UBaseType_t ux_Saved_ಅಡಚಣೆ_ಸ್ಥಿತಿ;
        Event_ಗುಂಪು_t ಸ್ಥಿರಾಂಕ * ಸ್ಥಿರಾಂಕ px_Event_ಬಿಟ್ಟುಗಳು = x_Event_ಗುಂಪು;
        Event_ಬಿಟ್ಟುಗಳು_t uxReturn;

        trace_ENTER_x_Event_ಗುಂಪು_Get_ಬಿಟ್ಟುಗಳು_From_ISR( x_Event_ಗುಂಪು );

        /* MISRA Ref 4.7.1 [Return value shall be checked] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
        /* coverity[misra_c_2012_directive_4_7_violation] */
        ux_Saved_ಅಡಚಣೆ_ಸ್ಥಿತಿ = taskENTER_CRITICAL_FROM_ISR();
        {
            uxReturn = px_Event_ಬಿಟ್ಟುಗಳು->ux_Event_ಬಿಟ್ಟುಗಳು;
        }
        taskEXIT_CRITICAL_FROM_ISR( ux_Saved_ಅಡಚಣೆ_ಸ್ಥಿತಿ );

        trace_RETURN_x_Event_ಗುಂಪು_Get_ಬಿಟ್ಟುಗಳು_From_ISR( uxReturn );

        ಹಿಂತಿರುಗಿ uxReturn;
    }
/*-----------------------------------------------------------*/

    Event_ಬಿಟ್ಟುಗಳು_t x_Event_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು( Event_ಗುಂಪು_Handle_t x_Event_ಗುಂಪು,
                                    ಸ್ಥಿರಾಂಕ Event_ಬಿಟ್ಟುಗಳು_t ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_ಗಣ )
    {
        List_ಅಂಶ_t * px_List_ಅಂಶ;
        List_ಅಂಶ_t * px_ಮುಂದಿನ;
        List_ಅಂಶ_t ಸ್ಥಿರಾಂಕ * px_List_ಕೊನೆ;
        List_t ಸ್ಥಿರಾಂಕ * pxList;
        Event_ಬಿಟ್ಟುಗಳು_t ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Clear = 0, ux_ಬಿಟ್ಟುಗಳು_Waited_For, ux_Control_ಬಿಟ್ಟುಗಳು;
        Event_ಗುಂಪು_t * px_Event_ಬಿಟ್ಟುಗಳು = x_Event_ಗುಂಪು;
        ತಳ_Type_t x_ಹೊಂದಿಕೆ_ಸಿಕ್ಕಿತು = pdFALSE;

        trace_ENTER_x_Event_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು( x_Event_ಗುಂಪು, ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_ಗಣ );

        /* Check the user is not attempting to set the bits used by the kernel
         * itself. */
        configASSERT( x_Event_ಗುಂಪು );
        configASSERT( ( ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_ಗಣ & event_EVENT_ಬಿಟ್ಟುಗಳು_CONTROL_ಬೈಟುಗಳು ) == 0 );

        pxList = &( px_Event_ಬಿಟ್ಟುಗಳು->x_Tasks_ಕಾಯುವಿಕೆ_For_ಬಿಟ್ಟುಗಳು );
        px_List_ಕೊನೆ = list_GET_ಕೊನೆ_MARKER( pxList );
        vTaskSuspendAll();
        {
            trace_EVENT_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು( x_Event_ಗುಂಪು, ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_ಗಣ );

            px_List_ಅಂಶ = list_GET_ತಲೆ_ನಮೂದು( pxList );

            /* Set the bits. */
            px_Event_ಬಿಟ್ಟುಗಳು->ux_Event_ಬಿಟ್ಟುಗಳು |= ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_ಗಣ;

            /* See if the new bit value should unblock any tasks. */
            ಆಗ( px_List_ಅಂಶ != px_List_ಕೊನೆ )
            {
                px_ಮುಂದಿನ = list_GET_ಮುಂದಿನ( px_List_ಅಂಶ );
                ux_ಬಿಟ್ಟುಗಳು_Waited_For = list_GET_LIST_ಅಂಶ_ಮೌಲ್ಯ( px_List_ಅಂಶ );
                x_ಹೊಂದಿಕೆ_ಸಿಕ್ಕಿತು = pdFALSE;

                /* Split the bits waited for from the control bits. */
                ux_Control_ಬಿಟ್ಟುಗಳು = ux_ಬಿಟ್ಟುಗಳು_Waited_For & event_EVENT_ಬಿಟ್ಟುಗಳು_CONTROL_ಬೈಟುಗಳು;
                ux_ಬಿಟ್ಟುಗಳು_Waited_For &= ~event_EVENT_ಬಿಟ್ಟುಗಳು_CONTROL_ಬೈಟುಗಳು;

                ಆದರೆ( ( ux_Control_ಬಿಟ್ಟುಗಳು & event_WAIT_FOR_ALL_ಬಿಟ್ಟುಗಳು ) == ( Event_ಬಿಟ್ಟುಗಳು_t ) 0 )
                {
                    /* Just looking for single bit being set. */
                    ಆದರೆ( ( ux_ಬಿಟ್ಟುಗಳು_Waited_For & px_Event_ಬಿಟ್ಟುಗಳು->ux_Event_ಬಿಟ್ಟುಗಳು ) != ( Event_ಬಿಟ್ಟುಗಳು_t ) 0 )
                    {
                        x_ಹೊಂದಿಕೆ_ಸಿಕ್ಕಿತು = pdTRUE;
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ( ux_ಬಿಟ್ಟುಗಳು_Waited_For & px_Event_ಬಿಟ್ಟುಗಳು->ux_Event_ಬಿಟ್ಟುಗಳು ) == ux_ಬಿಟ್ಟುಗಳು_Waited_For )
                {
                    /* All bits are set. */
                    x_ಹೊಂದಿಕೆ_ಸಿಕ್ಕಿತು = pdTRUE;
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    /* Need all bits to be set, but not all the bits were set. */
                }

                ಆದರೆ( x_ಹೊಂದಿಕೆ_ಸಿಕ್ಕಿತು != pdFALSE )
                {
                    /* The bits match.  Should the bits be cleared on exit? */
                    ಆದರೆ( ( ux_Control_ಬಿಟ್ಟುಗಳು & event_CLEAR_EVENTS_ON_EXIT_ಬಿಟ್ ) != ( Event_ಬಿಟ್ಟುಗಳು_t ) 0 )
                    {
                        ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Clear |= ux_ಬಿಟ್ಟುಗಳು_Waited_For;
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }

                    /* Store the actual event flag value in the task's event list
                     * item before removing the task from the event list.  The
                     * eventUNBLOCKED_DUE_TO_BIT_SET bit is set so the task knows
                     * that is was unblocked due to its required bits matching, rather
                     * than because it timed out. */
                    vTaskRemoveFromUnorderedEventList( px_List_ಅಂಶ, px_Event_ಬಿಟ್ಟುಗಳು->ux_Event_ಬಿಟ್ಟುಗಳು | event_UNBLOCKED_DUE_ಗಮ್ಯ_ಬಿಟ್_ಗಣ );
                }

                /* Move onto the next list item.  Note pxListItem->pxNext is not
                 * used here as the list item may have been removed from the event list
                 * and inserted into the ready/pending reading list. */
                px_List_ಅಂಶ = px_ಮುಂದಿನ;
            }

            /* Clear any bits that matched when the eventCLEAR_EVENTS_ON_EXIT_BIT
             * bit was set in the control word. */
            px_Event_ಬಿಟ್ಟುಗಳು->ux_Event_ಬಿಟ್ಟುಗಳು &= ~ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Clear;
        }
        ( ಶೂನ್ಯ ) xTaskResumeAll();

        trace_RETURN_x_Event_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು( px_Event_ಬಿಟ್ಟುಗಳು->ux_Event_ಬಿಟ್ಟುಗಳು );

        ಹಿಂತಿರುಗಿ px_Event_ಬಿಟ್ಟುಗಳು->ux_Event_ಬಿಟ್ಟುಗಳು;
    }
/*-----------------------------------------------------------*/

    ಶೂನ್ಯ v_Event_ಗುಂಪು_Delete( Event_ಗುಂಪು_Handle_t x_Event_ಗುಂಪು )
    {
        Event_ಗುಂಪು_t * px_Event_ಬಿಟ್ಟುಗಳು = x_Event_ಗುಂಪು;
        ಸ್ಥಿರಾಂಕ List_t * px_Tasks_ಕಾಯುವಿಕೆ_For_ಬಿಟ್ಟುಗಳು;

        trace_ENTER_v_Event_ಗುಂಪು_Delete( x_Event_ಗುಂಪು );

        configASSERT( px_Event_ಬಿಟ್ಟುಗಳು );

        px_Tasks_ಕಾಯುವಿಕೆ_For_ಬಿಟ್ಟುಗಳು = &( px_Event_ಬಿಟ್ಟುಗಳು->x_Tasks_ಕಾಯುವಿಕೆ_For_ಬಿಟ್ಟುಗಳು );

        vTaskSuspendAll();
        {
            trace_EVENT_ಗುಂಪು_DELETE( x_Event_ಗುಂಪು );

            ಆಗ( list_CURRENT_LIST_ಉದ್ದಳತೆ( px_Tasks_ಕಾಯುವಿಕೆ_For_ಬಿಟ್ಟುಗಳು ) > ( UBaseType_t ) 0 )
            {
                /* Unblock the task, returning 0 as the event list is being deleted
                 * and cannot therefore have any bits set. */
                configASSERT( px_Tasks_ಕಾಯುವಿಕೆ_For_ಬಿಟ್ಟುಗಳು->x_List_ಕೊನೆ.px_ಮುಂದಿನ != ( ಸ್ಥಿರಾಂಕ List_ಅಂಶ_t * ) &( px_Tasks_ಕಾಯುವಿಕೆ_For_ಬಿಟ್ಟುಗಳು->x_List_ಕೊನೆ ) );
                vTaskRemoveFromUnorderedEventList( px_Tasks_ಕಾಯುವಿಕೆ_For_ಬಿಟ್ಟುಗಳು->x_List_ಕೊನೆ.px_ಮುಂದಿನ, event_UNBLOCKED_DUE_ಗಮ್ಯ_ಬಿಟ್_ಗಣ );
            }
        }
        ( ಶೂನ್ಯ ) xTaskResumeAll();

        #ಆದರೆ ( ( config_SUPPORT_DYNAMIC_ಹಂಚಿಕೆ == 1 ) && ( config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 0 ) )
        {
            /* The event group can only have been allocated dynamically - free
             * it again. */
            v_ಪೋರ್ಟ್_Free( px_Event_ಬಿಟ್ಟುಗಳು );
        }
        #ಅಥವಾಆದರೆ ( ( config_SUPPORT_DYNAMIC_ಹಂಚಿಕೆ == 1 ) && ( config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 1 ) )
        {
            /* The event group could have been allocated statically or
             * dynamically, so check before attempting to free the memory. */
            ಆದರೆ( px_Event_ಬಿಟ್ಟುಗಳು->ucStaticallyAllocated == ( uint8_t ) pdFALSE )
            {
                v_ಪೋರ್ಟ್_Free( px_Event_ಬಿಟ್ಟುಗಳು );
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        #ಆದರೆ_ಅಂತ್ಯ /* configSUPPORT_DYNAMIC_ALLOCATION */

        trace_RETURN_v_Event_ಗುಂಪು_Delete();
    }
/*-----------------------------------------------------------*/

    #ಆದರೆ ( config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 1 )
        ತಳ_Type_t x_Event_ಗುಂಪು_Get_Static_ಬಫರ್( Event_ಗುಂಪು_Handle_t x_Event_ಗುಂಪು,
                                               Static_Event_ಗುಂಪು_t ** ppx_Event_ಗುಂಪು_ಬಫರ್ )
        {
            ತಳ_Type_t xReturn;
            Event_ಗುಂಪು_t * px_Event_ಬಿಟ್ಟುಗಳು = x_Event_ಗುಂಪು;

            trace_ENTER_x_Event_ಗುಂಪು_Get_Static_ಬಫರ್( x_Event_ಗುಂಪು, ppx_Event_ಗುಂಪು_ಬಫರ್ );

            configASSERT( px_Event_ಬಿಟ್ಟುಗಳು );
            configASSERT( ppx_Event_ಗುಂಪು_ಬಫರ್ );

            #ಆದರೆ ( config_SUPPORT_DYNAMIC_ಹಂಚಿಕೆ == 1 )
            {
                /* Check if the event group was statically allocated. */
                ಆದರೆ( px_Event_ಬಿಟ್ಟುಗಳು->ucStaticallyAllocated == ( uint8_t ) pdTRUE )
                {
                    /* MISRA Ref 11.3.1 [Misaligned access] */
                    /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
                    /* coverity[misra_c_2012_rule_11_3_violation] */
                    *ppx_Event_ಗುಂಪು_ಬಫರ್ = ( Static_Event_ಗುಂಪು_t * ) px_Event_ಬಿಟ್ಟುಗಳು;
                    xReturn = pdTRUE;
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    xReturn = pdFALSE;
                }
            }
            #ಇಲ್ಲದಿದ್ದರೆ /* configSUPPORT_DYNAMIC_ALLOCATION */
            {
                /* Event group must have been statically allocated. */
                /* MISRA Ref 11.3.1 [Misaligned access] */
                /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
                /* coverity[misra_c_2012_rule_11_3_violation] */
                *ppx_Event_ಗುಂಪು_ಬಫರ್ = ( Static_Event_ಗುಂಪು_t * ) px_Event_ಬಿಟ್ಟುಗಳು;
                xReturn = pdTRUE;
            }
            #ಆದರೆ_ಅಂತ್ಯ /* configSUPPORT_DYNAMIC_ALLOCATION */

            trace_RETURN_x_Event_ಗುಂಪು_Get_Static_ಬಫರ್( xReturn );

            ಹಿಂತಿರುಗಿ xReturn;
        }
    #ಆದರೆ_ಅಂತ್ಯ /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

/* For internal use only - execute a 'set bits' command that was pended from
 * an interrupt. */
    ಶೂನ್ಯ v_Event_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು_Callback( ಶೂನ್ಯ * pv_Event_ಗುಂಪು,
                                     uint32_t ul_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_ಗಣ )
    {
        trace_ENTER_v_Event_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು_Callback( pv_Event_ಗುಂಪು, ul_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_ಗಣ );

        /* MISRA Ref 11.5.4 [Callback function parameter] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
        /* coverity[misra_c_2012_rule_11_5_violation] */
        ( ಶೂನ್ಯ ) x_Event_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು( pv_Event_ಗುಂಪು, ( Event_ಬಿಟ್ಟುಗಳು_t ) ul_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_ಗಣ );

        trace_RETURN_v_Event_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು_Callback();
    }
/*-----------------------------------------------------------*/

/* For internal use only - execute a 'clear bits' command that was pended from
 * an interrupt. */
    ಶೂನ್ಯ v_Event_ಗುಂಪು_Clear_ಬಿಟ್ಟುಗಳು_Callback( ಶೂನ್ಯ * pv_Event_ಗುಂಪು,
                                       uint32_t ul_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Clear )
    {
        trace_ENTER_v_Event_ಗುಂಪು_Clear_ಬಿಟ್ಟುಗಳು_Callback( pv_Event_ಗುಂಪು, ul_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Clear );

        /* MISRA Ref 11.5.4 [Callback function parameter] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
        /* coverity[misra_c_2012_rule_11_5_violation] */
        ( ಶೂನ್ಯ ) x_Event_ಗುಂಪು_Clear_ಬಿಟ್ಟುಗಳು( pv_Event_ಗುಂಪು, ( Event_ಬಿಟ್ಟುಗಳು_t ) ul_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Clear );

        trace_RETURN_v_Event_ಗುಂಪು_Clear_ಬಿಟ್ಟುಗಳು_Callback();
    }
/*-----------------------------------------------------------*/

    ಸ್ಥಿರ ತಳ_Type_t prvTestWaitCondition( ಸ್ಥಿರಾಂಕ Event_ಬಿಟ್ಟುಗಳು_t ux_Current_Event_ಬಿಟ್ಟುಗಳು,
                                            ಸ್ಥಿರಾಂಕ Event_ಬಿಟ್ಟುಗಳು_t ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Wait_For,
                                            ಸ್ಥಿರಾಂಕ ತಳ_Type_t x_Wait_For_All_ಬಿಟ್ಟುಗಳು )
    {
        ತಳ_Type_t xWaitConditionMet = pdFALSE;

        ಆದರೆ( x_Wait_For_All_ಬಿಟ್ಟುಗಳು == pdFALSE )
        {
            /* Task only has to wait for one bit within uxBitsToWaitFor to be
             * set.  Is one already set? */
            ಆದರೆ( ( ux_Current_Event_ಬಿಟ್ಟುಗಳು & ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Wait_For ) != ( Event_ಬಿಟ್ಟುಗಳು_t ) 0 )
            {
                xWaitConditionMet = pdTRUE;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            /* Task has to wait for all the bits in uxBitsToWaitFor to be set.
             * Are they set already? */
            ಆದರೆ( ( ux_Current_Event_ಬಿಟ್ಟುಗಳು & ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Wait_For ) == ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Wait_For )
            {
                xWaitConditionMet = pdTRUE;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }

        ಹಿಂತಿರುಗಿ xWaitConditionMet;
    }
/*-----------------------------------------------------------*/

    #ಆದರೆ ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) )

        ತಳ_Type_t x_Event_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು_From_ISR( Event_ಗುಂಪು_Handle_t x_Event_ಗುಂಪು,
                                              ಸ್ಥಿರಾಂಕ Event_ಬಿಟ್ಟುಗಳು_t ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_ಗಣ,
                                              ತಳ_Type_t * px_Higher_ಆದ್ಯತೆ_Task_Woken )
        {
            ತಳ_Type_t xReturn;

            trace_ENTER_x_Event_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು_From_ISR( x_Event_ಗುಂಪು, ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_ಗಣ, px_Higher_ಆದ್ಯತೆ_Task_Woken );

            trace_EVENT_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು_FROM_ISR( x_Event_ಗುಂಪು, ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_ಗಣ );
            xReturn = xTimerPendFunctionCallFromISR( v_Event_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು_Callback, ( ಶೂನ್ಯ * ) x_Event_ಗುಂಪು, ( uint32_t ) ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_ಗಣ, px_Higher_ಆದ್ಯತೆ_Task_Woken );

            trace_RETURN_x_Event_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು_From_ISR( xReturn );

            ಹಿಂತಿರುಗಿ xReturn;
        }

    #ಆದರೆ_ಅಂತ್ಯ /* if ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) ) */
/*-----------------------------------------------------------*/

    #ಆದರೆ ( configUSE_TRACE_FACILITY == 1 )

        UBaseType_t ux_Event_ಗುಂಪು_Get_ಸಂಖ್ಯೆ( ಶೂನ್ಯ * x_Event_ಗುಂಪು )
        {
            UBaseType_t xReturn;

            /* MISRA Ref 11.5.2 [Opaque pointer] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
            /* coverity[misra_c_2012_rule_11_5_violation] */
            Event_ಗುಂಪು_t ಸ್ಥಿರಾಂಕ * px_Event_ಬಿಟ್ಟುಗಳು = ( Event_ಗುಂಪು_t * ) x_Event_ಗುಂಪು;

            trace_ENTER_ux_Event_ಗುಂಪು_Get_ಸಂಖ್ಯೆ( x_Event_ಗುಂಪು );

            ಆದರೆ( x_Event_ಗುಂಪು == ಖಾಲಿ )
            {
                xReturn = 0;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                xReturn = px_Event_ಬಿಟ್ಟುಗಳು->ux_Event_ಗುಂಪು_ಸಂಖ್ಯೆ;
            }

            trace_RETURN_ux_Event_ಗುಂಪು_Get_ಸಂಖ್ಯೆ( xReturn );

            ಹಿಂತಿರುಗಿ xReturn;
        }

    #ಆದರೆ_ಅಂತ್ಯ /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

    #ಆದರೆ ( configUSE_TRACE_FACILITY == 1 )

        ಶೂನ್ಯ v_Event_ಗುಂಪು_ಗಣ_ಸಂಖ್ಯೆ( ಶೂನ್ಯ * x_Event_ಗುಂಪು,
                                   UBaseType_t ux_Event_ಗುಂಪು_ಸಂಖ್ಯೆ )
        {
            trace_ENTER_v_Event_ಗುಂಪು_ಗಣ_ಸಂಖ್ಯೆ( x_Event_ಗುಂಪು, ux_Event_ಗುಂಪು_ಸಂಖ್ಯೆ );

            /* MISRA Ref 11.5.2 [Opaque pointer] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
            /* coverity[misra_c_2012_rule_11_5_violation] */
            ( ( Event_ಗುಂಪು_t * ) x_Event_ಗುಂಪು )->ux_Event_ಗುಂಪು_ಸಂಖ್ಯೆ = ux_Event_ಗುಂಪು_ಸಂಖ್ಯೆ;

            trace_RETURN_v_Event_ಗುಂಪು_ಗಣ_ಸಂಖ್ಯೆ();
        }

    #ಆದರೆ_ಅಂತ್ಯ /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

/* This entire source file will be skipped if the application is not configured
 * to include event groups functionality. If you want to include event groups
 * then ensure configUSE_EVENT_GROUPS is set to 1 in FreeRTOSConfig.h. */
#ಆದರೆ_ಅಂತ್ಯ /* configUSE_EVENT_GROUPS == 1 */
