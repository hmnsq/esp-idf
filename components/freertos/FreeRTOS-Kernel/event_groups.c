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
#समावेश <stdlib.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#घोषणा MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/* FreeRTOS includes. */
#समावेश "FreeRTOS.h"
#समावेश "task.h"
#समावेश "timers.h"
#समावेश "event_groups.h"
/* Include private IDF API additions for critical thread safety macros */
#समावेश "esp_private/freertos_idf_additions_priv.h"

/* Lint e961, e750 and e9021 are suppressed as a MISRA exception justified
 * because the MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#अघोषित MPU_WRAPPERS_INCLUDED_FROM_API_FILE /*lint !e961 !e750 !e9021 See comment above. */

/* The following bit fields convey control information in a task's event list
 * item value.  It is important they don't clash with the
 * taskEVENT_LIST_ITEM_VALUE_IN_USE definition. */
#अगर configUSE_16_BIT_TICKS == 1
    #घोषणा eventCLEAR_EVENTS_ON_EXIT_BIT    0x0100U
    #घोषणा eventUNBLOCKED_DUE_TO_BIT_SET    0x0200U
    #घोषणा eventWAIT_FOR_ALL_BITS           0x0400U
    #घोषणा eventEVENT_BITS_CONTROL_BYTES    0xff00U
#अन्यथा
    #घोषणा eventCLEAR_EVENTS_ON_EXIT_BIT    0x01000000UL
    #घोषणा eventUNBLOCKED_DUE_TO_BIT_SET    0x02000000UL
    #घोषणा eventWAIT_FOR_ALL_BITS           0x04000000UL
    #घोषणा eventEVENT_BITS_CONTROL_BYTES    0xff000000UL
#पूर्ण_अगर

प्रकार काष्ठा EventGroupDef_t
{
    EventBits_t uxEventBits;
    List_t xTasksWaitingForBits; /*< List of tasks waiting for a bit to be set. */

    #अगर ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_Event_Group_संख्या;
    #पूर्ण_अगर

    #अगर ( ( configSUPPORT_STATIC_ALLOCATION == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) )
        uint8_t ucStaticallyAllocated; /*< Set to pdTRUE if the event group is statically allocated to ensure no attempt is made to free the memory. */
    #पूर्ण_अगर

    portMUX_TYPE xEventGroupLock; /* Spinlock required for SMP critical sections */
} EventGroup_t;

/*-----------------------------------------------------------*/

/*
 * Test the bits set in uxCurrentEventBits to see if the wait condition is met.
 * The wait condition is defined by xWaitForAllBits.  If xWaitForAllBits is
 * pdTRUE then the wait condition is met if all the bits set in uxBitsToWaitFor
 * are also set in uxCurrentEventBits.  If xWaitForAllBits is pdFALSE then the
 * wait condition is met if any of the bits set in uxBitsToWait for are also set
 * in uxCurrentEventBits.
 */
अटल BaseType_t prvTestWaitCondition( स्थिर EventBits_t uxCurrentEventBits,
                                        स्थिर EventBits_t uxBitsToWaitFor,
                                        स्थिर BaseType_t xWaitForAllBits ) PRIVILEGED_FUNCTION;

/*-----------------------------------------------------------*/

#अगर ( configSUPPORT_STATIC_ALLOCATION == 1 )

    EventGroupHandle_t xEventGroupCreateStatic( StaticEventGroup_t * px_Event_Group_बफर )
    {
        EventGroup_t * pxEventBits;

        /* A StaticEventGroup_t object must be provided. */
        configASSERT( px_Event_Group_बफर );

        #अगर ( configASSERT_DEFINED == 1 )
        {
            /* Sanity check that the size of the structure used to declare a
             * variable of type StaticEventGroup_t equals the size of the real
             * event group structure. */
            अस्थिर माप_प्रकार x_आकार = माप( StaticEventGroup_t );
            configASSERT( x_आकार == माप( EventGroup_t ) );
            ( व्योम ) x_आकार; /* Prevent unused variable warning when configASSERT() is not used. */
        } /*lint !e529 xSize is referenced if configASSERT() is defined. */
        #पूर्ण_अगर /* configASSERT_DEFINED */

        /* The user has provided a statically allocated event group - use it. */
        pxEventBits = ( EventGroup_t * ) px_Event_Group_बफर; /*lint !e740 !e9087 EventGroup_t and StaticEventGroup_t are deliberately aliased for data hiding purposes and guaranteed to have the same size and alignment requirement - checked by configASSERT(). */

        अगर( pxEventBits != शून्य )
        {
            pxEventBits->uxEventBits = 0;
            vListInitialise( &( pxEventBits->xTasksWaitingForBits ) );

            #अगर ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
            {
                /* Both static and dynamic allocation can be used, so note that
                 * this event group was created statically in case the event group
                 * is later deleted. */
                pxEventBits->ucStaticallyAllocated = pdTRUE;
            }
            #पूर्ण_अगर /* configSUPPORT_DYNAMIC_ALLOCATION */

            /* Initialize the event group's spinlock. */
            portMUX_INITIALIZE( &pxEventBits->xEventGroupLock );

            traceEVENT_GROUP_CREATE( pxEventBits );
        }
        अन्यथा
        {
            /* xEventGroupCreateStatic should only ever be called with
             * pxEventGroupBuffer pointing to a pre-allocated (compile time
             * allocated) StaticEventGroup_t variable. */
            traceEVENT_GROUP_CREATE_FAILED();
        }

        वापस pxEventBits;
    }

#पूर्ण_अगर /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

#अगर ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )

    EventGroupHandle_t xEventGroupCreate( व्योम )
    {
        EventGroup_t * pxEventBits;

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
        pxEventBits = ( EventGroup_t * ) pvPortMalloc( माप( EventGroup_t ) ); /*lint !e9087 !e9079 see comment above. */

        अगर( pxEventBits != शून्य )
        {
            pxEventBits->uxEventBits = 0;
            vListInitialise( &( pxEventBits->xTasksWaitingForBits ) );

            #अगर ( configSUPPORT_STATIC_ALLOCATION == 1 )
            {
                /* Both static and dynamic allocation can be used, so note this
                 * event group was allocated statically in case the event group is
                 * later deleted. */
                pxEventBits->ucStaticallyAllocated = pdFALSE;
            }
            #पूर्ण_अगर /* configSUPPORT_STATIC_ALLOCATION */

            /* Initialize the event group's spinlock. */
            portMUX_INITIALIZE( &pxEventBits->xEventGroupLock );

            traceEVENT_GROUP_CREATE( pxEventBits );
        }
        अन्यथा
        {
            traceEVENT_GROUP_CREATE_FAILED(); /*lint !e9063 Else branch only exists to allow tracing and does not generate code if trace macros are not defined. */
        }

        वापस pxEventBits;
    }

#पूर्ण_अगर /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

EventBits_t xEventGroupSync( EventGroupHandle_t xEventGroup,
                             स्थिर EventBits_t uxBitsToSet,
                             स्थिर EventBits_t uxBitsToWaitFor,
                             TickType_t xTicksToWait )
{
    EventBits_t ux_Original_Bit_मूल्य, uxReturn;
    EventGroup_t * pxEventBits = xEventGroup;
    BaseType_t xAlreadyYielded;
    BaseType_t xTimeoutOccurred = pdFALSE;

    configASSERT( ( uxBitsToWaitFor & eventEVENT_BITS_CONTROL_BYTES ) == 0 );
    configASSERT( uxBitsToWaitFor != 0 );
    #अगर ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( xTicksToWait != 0 ) ) );
    }
    #पूर्ण_अगर

    prvENTER_CRITICAL_OR_SUSPEND_ALL( &( pxEventBits->xEventGroupLock ) );
    {
        ux_Original_Bit_मूल्य = pxEventBits->uxEventBits;

        ( व्योम ) xEventGroupSetBits( xEventGroup, uxBitsToSet );

        अगर( ( ( ux_Original_Bit_मूल्य | uxBitsToSet ) & uxBitsToWaitFor ) == uxBitsToWaitFor )
        {
            /* All the rendezvous bits are now set - no need to block. */
            uxReturn = ( ux_Original_Bit_मूल्य | uxBitsToSet );

            /* Rendezvous always clear the bits.  They will have been cleared
             * already unless this is the only task in the rendezvous. */
            pxEventBits->uxEventBits &= ~uxBitsToWaitFor;

            xTicksToWait = 0;
        }
        अन्यथा
        {
            अगर( xTicksToWait != ( TickType_t ) 0 )
            {
                traceEVENT_GROUP_SYNC_BLOCK( xEventGroup, uxBitsToSet, uxBitsToWaitFor );

                /* Store the bits that the calling task is waiting for in the
                 * task's event list item so the kernel knows when a match is
                 * found.  Then enter the blocked state. */
                vTaskPlaceOnUnorderedEventList( &( pxEventBits->xTasksWaitingForBits ), ( uxBitsToWaitFor | eventCLEAR_EVENTS_ON_EXIT_BIT | eventWAIT_FOR_ALL_BITS ), xTicksToWait );

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
                uxReturn = pxEventBits->uxEventBits;
                xTimeoutOccurred = pdTRUE;
            }
        }
    }
    xAlreadyYielded = prvEXIT_CRITICAL_OR_RESUME_ALL( &( pxEventBits->xEventGroupLock ) );

    अगर( xTicksToWait != ( TickType_t ) 0 )
    {
        अगर( xAlreadyYielded == pdFALSE )
        {
            portYIELD_WITHIN_API();
        }
        अन्यथा
        {
            mtCOVERAGE_TEST_MARKER();
        }

        /* The task blocked to wait for its required bits to be set - at this
         * point either the required bits were set or the block time expired.  If
         * the required bits were set they will have been stored in the task's
         * event list item, and they should now be retrieved then cleared. */
        uxReturn = ux_Task_Reset_Event_Item_मूल्य();

        अगर( ( uxReturn & eventUNBLOCKED_DUE_TO_BIT_SET ) == ( EventBits_t ) 0 )
        {
            /* The task timed out, just return the current event bit value. */
            taskENTER_CRITICAL( &( pxEventBits->xEventGroupLock ) );
            {
                uxReturn = pxEventBits->uxEventBits;

                /* Although the task got here because it timed out before the
                 * bits it was waiting for were set, it is possible that since it
                 * unblocked another task has set the bits.  If this is the case
                 * then it needs to clear the bits before exiting. */
                अगर( ( uxReturn & uxBitsToWaitFor ) == uxBitsToWaitFor )
                {
                    pxEventBits->uxEventBits &= ~uxBitsToWaitFor;
                }
                अन्यथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            taskEXIT_CRITICAL( &( pxEventBits->xEventGroupLock ) );

            xTimeoutOccurred = pdTRUE;
        }
        अन्यथा
        {
            /* The task unblocked because the bits were set. */
        }

        /* Control bits might be set as the task had blocked should not be
         * returned. */
        uxReturn &= ~eventEVENT_BITS_CONTROL_BYTES;
    }

    traceEVENT_GROUP_SYNC_END( xEventGroup, uxBitsToSet, uxBitsToWaitFor, xTimeoutOccurred );

    /* Prevent compiler warnings when trace macros are not used. */
    ( व्योम ) xTimeoutOccurred;

    वापस uxReturn;
}
/*-----------------------------------------------------------*/

EventBits_t xEventGroupWaitBits( EventGroupHandle_t xEventGroup,
                                 स्थिर EventBits_t uxBitsToWaitFor,
                                 स्थिर BaseType_t xClearOnExit,
                                 स्थिर BaseType_t xWaitForAllBits,
                                 TickType_t xTicksToWait )
{
    EventGroup_t * pxEventBits = xEventGroup;
    EventBits_t uxReturn, uxControlBits = 0;
    BaseType_t xWaitConditionMet, xAlreadyYielded;
    BaseType_t xTimeoutOccurred = pdFALSE;

    /* Check the user is not attempting to wait on the bits used by the kernel
     * itself, and that at least one bit is being requested. */
    configASSERT( xEventGroup );
    configASSERT( ( uxBitsToWaitFor & eventEVENT_BITS_CONTROL_BYTES ) == 0 );
    configASSERT( uxBitsToWaitFor != 0 );
    #अगर ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( xTicksToWait != 0 ) ) );
    }
    #पूर्ण_अगर

    prvENTER_CRITICAL_OR_SUSPEND_ALL( &( pxEventBits->xEventGroupLock ) );
    {
        स्थिर EventBits_t uxCurrentEventBits = pxEventBits->uxEventBits;

        /* Check to see if the wait condition is already met or not. */
        xWaitConditionMet = prvTestWaitCondition( uxCurrentEventBits, uxBitsToWaitFor, xWaitForAllBits );

        अगर( xWaitConditionMet != pdFALSE )
        {
            /* The wait condition has already been met so there is no need to
             * block. */
            uxReturn = uxCurrentEventBits;
            xTicksToWait = ( TickType_t ) 0;

            /* Clear the wait bits if requested to do so. */
            अगर( xClearOnExit != pdFALSE )
            {
                pxEventBits->uxEventBits &= ~uxBitsToWaitFor;
            }
            अन्यथा
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        अन्यथा अगर( xTicksToWait == ( TickType_t ) 0 )
        {
            /* The wait condition has not been met, but no block time was
             * specified, so just return the current value. */
            uxReturn = uxCurrentEventBits;
            xTimeoutOccurred = pdTRUE;
        }
        अन्यथा
        {
            /* The task is going to block to wait for its required bits to be
             * set.  uxControlBits are used to remember the specified behaviour of
             * this call to xEventGroupWaitBits() - for use when the event bits
             * unblock the task. */
            अगर( xClearOnExit != pdFALSE )
            {
                uxControlBits |= eventCLEAR_EVENTS_ON_EXIT_BIT;
            }
            अन्यथा
            {
                mtCOVERAGE_TEST_MARKER();
            }

            अगर( xWaitForAllBits != pdFALSE )
            {
                uxControlBits |= eventWAIT_FOR_ALL_BITS;
            }
            अन्यथा
            {
                mtCOVERAGE_TEST_MARKER();
            }

            /* Store the bits that the calling task is waiting for in the
             * task's event list item so the kernel knows when a match is
             * found.  Then enter the blocked state. */
            vTaskPlaceOnUnorderedEventList( &( pxEventBits->xTasksWaitingForBits ), ( uxBitsToWaitFor | uxControlBits ), xTicksToWait );

            /* This is obsolete as it will get set after the task unblocks, but
             * some compilers mistakenly generate a warning about the variable
             * being returned without being set if it is not done. */
            uxReturn = 0;

            traceEVENT_GROUP_WAIT_BITS_BLOCK( xEventGroup, uxBitsToWaitFor );
        }
    }
    xAlreadyYielded = prvEXIT_CRITICAL_OR_RESUME_ALL( &( pxEventBits->xEventGroupLock ) );

    अगर( xTicksToWait != ( TickType_t ) 0 )
    {
        अगर( xAlreadyYielded == pdFALSE )
        {
            portYIELD_WITHIN_API();
        }
        अन्यथा
        {
            mtCOVERAGE_TEST_MARKER();
        }

        /* The task blocked to wait for its required bits to be set - at this
         * point either the required bits were set or the block time expired.  If
         * the required bits were set they will have been stored in the task's
         * event list item, and they should now be retrieved then cleared. */
        uxReturn = ux_Task_Reset_Event_Item_मूल्य();

        अगर( ( uxReturn & eventUNBLOCKED_DUE_TO_BIT_SET ) == ( EventBits_t ) 0 )
        {
            taskENTER_CRITICAL( &( pxEventBits->xEventGroupLock ) );
            {
                /* The task timed out, just return the current event bit value. */
                uxReturn = pxEventBits->uxEventBits;

                /* It is possible that the event bits were updated between this
                 * task leaving the Blocked state and running again. */
                अगर( prvTestWaitCondition( uxReturn, uxBitsToWaitFor, xWaitForAllBits ) != pdFALSE )
                {
                    अगर( xClearOnExit != pdFALSE )
                    {
                        pxEventBits->uxEventBits &= ~uxBitsToWaitFor;
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
            taskEXIT_CRITICAL( &( pxEventBits->xEventGroupLock ) );
        }
        अन्यथा
        {
            /* The task unblocked because the bits were set. */
        }

        /* The task blocked so control bits may have been set. */
        uxReturn &= ~eventEVENT_BITS_CONTROL_BYTES;
    }

    traceEVENT_GROUP_WAIT_BITS_END( xEventGroup, uxBitsToWaitFor, xTimeoutOccurred );

    /* Prevent compiler warnings when trace macros are not used. */
    ( व्योम ) xTimeoutOccurred;

    वापस uxReturn;
}
/*-----------------------------------------------------------*/

EventBits_t xEventGroupClearBits( EventGroupHandle_t xEventGroup,
                                  स्थिर EventBits_t uxBitsToClear )
{
    EventGroup_t * pxEventBits = xEventGroup;
    EventBits_t uxReturn;

    /* Check the user is not attempting to clear the bits used by the kernel
     * itself. */
    configASSERT( xEventGroup );
    configASSERT( ( uxBitsToClear & eventEVENT_BITS_CONTROL_BYTES ) == 0 );

    taskENTER_CRITICAL( &( pxEventBits->xEventGroupLock ) );
    {
        traceEVENT_GROUP_CLEAR_BITS( xEventGroup, uxBitsToClear );

        /* The value returned is the event group value prior to the bits being
         * cleared. */
        uxReturn = pxEventBits->uxEventBits;

        /* Clear the bits. */
        pxEventBits->uxEventBits &= ~uxBitsToClear;
    }
    taskEXIT_CRITICAL( &( pxEventBits->xEventGroupLock ) );

    वापस uxReturn;
}
/*-----------------------------------------------------------*/

#अगर ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) )

    BaseType_t xEventGroupClearBitsFromISR( EventGroupHandle_t xEventGroup,
                                            स्थिर EventBits_t uxBitsToClear )
    {
        BaseType_t xReturn;

        traceEVENT_GROUP_CLEAR_BITS_FROM_ISR( xEventGroup, uxBitsToClear );
        xReturn = xTimerPendFunctionCallFromISR( vEventGroupClearBitsCallback, ( व्योम * ) xEventGroup, ( uint32_t ) uxBitsToClear, शून्य ); /*lint !e9087 Can't avoid cast to void* as a generic callback function not specific to this use case. Callback casts back to original type so safe. */

        वापस xReturn;
    }

#पूर्ण_अगर /* if ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) ) */
/*-----------------------------------------------------------*/

EventBits_t xEventGroupGetBitsFromISR( EventGroupHandle_t xEventGroup )
{
    UBaseType_t uxSavedInterruptStatus;
    EventGroup_t स्थिर * स्थिर pxEventBits = xEventGroup;
    EventBits_t uxReturn;

    prvENTER_CRITICAL_OR_MASK_ISR( ( portMUX_TYPE * ) &( pxEventBits->xEventGroupLock ), uxSavedInterruptStatus );
    {
        uxReturn = pxEventBits->uxEventBits;
    }
    prvEXIT_CRITICAL_OR_UNMASK_ISR( ( portMUX_TYPE * ) &( pxEventBits->xEventGroupLock ), uxSavedInterruptStatus );

    वापस uxReturn;
} /*lint !e818 EventGroupHandle_t is a typedef used in other functions to so can't be pointer to const. */
/*-----------------------------------------------------------*/

EventBits_t xEventGroupSetBits( EventGroupHandle_t xEventGroup,
                                स्थिर EventBits_t uxBitsToSet )
{
    ListItem_t * pxListItem;
    ListItem_t * px_अग्र;
    ListItem_t स्थिर * pxListEnd;
    List_t स्थिर * pxList;
    EventBits_t uxBitsToClear = 0, uxBitsWaitedFor, uxControlBits;
    EventGroup_t * pxEventBits = xEventGroup;
    BaseType_t xMatchFound = pdFALSE;

    /* Check the user is not attempting to set the bits used by the kernel
     * itself. */
    configASSERT( xEventGroup );
    configASSERT( ( uxBitsToSet & eventEVENT_BITS_CONTROL_BYTES ) == 0 );

    pxList = &( pxEventBits->xTasksWaitingForBits );
    pxListEnd = listGET_END_MARKER( pxList ); /*lint !e826 !e740 !e9087 The mini list structure is used as the list end to save RAM.  This is checked and valid. */

    prvENTER_CRITICAL_OR_SUSPEND_ALL( &( pxEventBits->xEventGroupLock ) );
    #अगर ( config_संख्या_OF_CORES > 1 )

        /* We are about to traverse a task list which is a kernel data structure.
         * Thus we need to call prvTakeKernelLock() to take the kernel lock. */
        prvTakeKernelLock();
    #पूर्ण_अगर /* configNUMBER_OF_CORES > 1 */
    {
        traceEVENT_GROUP_SET_BITS( xEventGroup, uxBitsToSet );

        pxListItem = list_GET_शीर्ष_ENTRY( pxList );

        /* Set the bits. */
        pxEventBits->uxEventBits |= uxBitsToSet;

        /* See if the new bit value should unblock any tasks. */
        जबतक( pxListItem != pxListEnd )
        {
            px_अग्र = list_GET_अग्र( pxListItem );
            uxBitsWaitedFor = list_GET_LIST_ITEM_मूल्य( pxListItem );
            xMatchFound = pdFALSE;

            /* Split the bits waited for from the control bits. */
            uxControlBits = uxBitsWaitedFor & eventEVENT_BITS_CONTROL_BYTES;
            uxBitsWaitedFor &= ~eventEVENT_BITS_CONTROL_BYTES;

            अगर( ( uxControlBits & eventWAIT_FOR_ALL_BITS ) == ( EventBits_t ) 0 )
            {
                /* Just looking for single bit being set. */
                अगर( ( uxBitsWaitedFor & pxEventBits->uxEventBits ) != ( EventBits_t ) 0 )
                {
                    xMatchFound = pdTRUE;
                }
                अन्यथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            अन्यथा अगर( ( uxBitsWaitedFor & pxEventBits->uxEventBits ) == uxBitsWaitedFor )
            {
                /* All bits are set. */
                xMatchFound = pdTRUE;
            }
            अन्यथा
            {
                /* Need all bits to be set, but not all the bits were set. */
            }

            अगर( xMatchFound != pdFALSE )
            {
                /* The bits match.  Should the bits be cleared on exit? */
                अगर( ( uxControlBits & eventCLEAR_EVENTS_ON_EXIT_BIT ) != ( EventBits_t ) 0 )
                {
                    uxBitsToClear |= uxBitsWaitedFor;
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
                vTaskRemoveFromUnorderedEventList( pxListItem, pxEventBits->uxEventBits | eventUNBLOCKED_DUE_TO_BIT_SET );
            }

            /* Move onto the next list item.  Note pxListItem->pxNext is not
             * used here as the list item may have been removed from the event list
             * and inserted into the ready/pending reading list. */
            pxListItem = px_अग्र;
        }

        /* Clear any bits that matched when the eventCLEAR_EVENTS_ON_EXIT_BIT
         * bit was set in the control word. */
        pxEventBits->uxEventBits &= ~uxBitsToClear;
    }
    #अगर ( config_संख्या_OF_CORES > 1 )
        /* Release the previously taken kernel lock. */
        prvReleaseKernelLock();
    #पूर्ण_अगर /* configNUMBER_OF_CORES > 1 */
    ( व्योम ) prvEXIT_CRITICAL_OR_RESUME_ALL( &( pxEventBits->xEventGroupLock ) );


    वापस pxEventBits->uxEventBits;
}
/*-----------------------------------------------------------*/

व्योम vEventGroupDelete( EventGroupHandle_t xEventGroup )
{
    EventGroup_t * pxEventBits = xEventGroup;
    स्थिर List_t * pxTasksWaitingForBits;

    configASSERT( pxEventBits );

    pxTasksWaitingForBits = &( pxEventBits->xTasksWaitingForBits );

    prvENTER_CRITICAL_OR_SUSPEND_ALL( &( pxEventBits->xEventGroupLock ) );
    #अगर ( config_संख्या_OF_CORES > 1 )

        /* We are about to traverse a task list which is a kernel data structure.
         * Thus we need to call prvTakeKernelLock() to take the kernel lock. */
        prvTakeKernelLock();
    #पूर्ण_अगर /* configNUMBER_OF_CORES > 1 */
    {
        traceEVENT_GROUP_DELETE( xEventGroup );

        जबतक( list_CURRENT_LIST_लंबाई( pxTasksWaitingForBits ) > ( UBaseType_t ) 0 )
        {
            /* Unblock the task, returning 0 as the event list is being deleted
             * and cannot therefore have any bits set. */
            configASSERT( pxTasksWaitingForBits->xListEnd.px_अग्र != ( स्थिर ListItem_t * ) &( pxTasksWaitingForBits->xListEnd ) );
            vTaskRemoveFromUnorderedEventList( pxTasksWaitingForBits->xListEnd.px_अग्र, eventUNBLOCKED_DUE_TO_BIT_SET );
        }
    }
    #अगर ( config_संख्या_OF_CORES > 1 )
        /* Release the previously taken kernel lock. */
        prvReleaseKernelLock();
    #पूर्ण_अगर /* configNUMBER_OF_CORES > 1 */
    prvEXIT_CRITICAL_OR_RESUME_ALL( &( pxEventBits->xEventGroupLock ) );

    #अगर ( ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) && ( configSUPPORT_STATIC_ALLOCATION == 0 ) )
    {
        /* The event group can only have been allocated dynamically - free
         * it again. */
        vPortFree( pxEventBits );
    }
    #या_अगर ( ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) && ( configSUPPORT_STATIC_ALLOCATION == 1 ) )
    {
        /* The event group could have been allocated statically or
         * dynamically, so check before attempting to free the memory. */
        अगर( pxEventBits->ucStaticallyAllocated == ( uint8_t ) pdFALSE )
        {
            vPortFree( pxEventBits );
        }
        अन्यथा
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    #पूर्ण_अगर /* configSUPPORT_DYNAMIC_ALLOCATION */
}
/*-----------------------------------------------------------*/

#अगर ( configSUPPORT_STATIC_ALLOCATION == 1 )
    BaseType_t x_Event_Group_Get_Static_बफर( EventGroupHandle_t xEventGroup,
                                           StaticEventGroup_t ** ppx_Event_Group_बफर )
    {
        BaseType_t xReturn;
        EventGroup_t * pxEventBits = xEventGroup;

        configASSERT( pxEventBits );
        configASSERT( ppx_Event_Group_बफर );

        #अगर ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
        {
            /* Check if the event group was statically allocated. */
            अगर( pxEventBits->ucStaticallyAllocated == ( uint8_t ) pdTRUE )
            {
                *ppx_Event_Group_बफर = ( StaticEventGroup_t * ) pxEventBits;
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
            *ppx_Event_Group_बफर = ( StaticEventGroup_t * ) pxEventBits;
            xReturn = pdTRUE;
        }
        #पूर्ण_अगर /* configSUPPORT_DYNAMIC_ALLOCATION */

        वापस xReturn;
    }
#पूर्ण_अगर /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

/* For internal use only - execute a 'set bits' command that was pended from
 * an interrupt. */
व्योम vEventGroupSetBitsCallback( व्योम * pvEventGroup,
                                 स्थिर uint32_t ulBitsToSet )
{
    ( व्योम ) xEventGroupSetBits( pvEventGroup, ( EventBits_t ) ulBitsToSet ); /*lint !e9079 Can't avoid cast to void* as a generic timer callback prototype. Callback casts back to original type so safe. */
}
/*-----------------------------------------------------------*/

/* For internal use only - execute a 'clear bits' command that was pended from
 * an interrupt. */
व्योम vEventGroupClearBitsCallback( व्योम * pvEventGroup,
                                   स्थिर uint32_t ulBitsToClear )
{
    ( व्योम ) xEventGroupClearBits( pvEventGroup, ( EventBits_t ) ulBitsToClear ); /*lint !e9079 Can't avoid cast to void* as a generic timer callback prototype. Callback casts back to original type so safe. */
}
/*-----------------------------------------------------------*/

अटल BaseType_t prvTestWaitCondition( स्थिर EventBits_t uxCurrentEventBits,
                                        स्थिर EventBits_t uxBitsToWaitFor,
                                        स्थिर BaseType_t xWaitForAllBits )
{
    BaseType_t xWaitConditionMet = pdFALSE;

    अगर( xWaitForAllBits == pdFALSE )
    {
        /* Task only has to wait for one bit within uxBitsToWaitFor to be
         * set.  Is one already set? */
        अगर( ( uxCurrentEventBits & uxBitsToWaitFor ) != ( EventBits_t ) 0 )
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
        अगर( ( uxCurrentEventBits & uxBitsToWaitFor ) == uxBitsToWaitFor )
        {
            xWaitConditionMet = pdTRUE;
        }
        अन्यथा
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }

    वापस xWaitConditionMet;
}
/*-----------------------------------------------------------*/

#अगर ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) )

    BaseType_t xEventGroupSetBitsFromISR( EventGroupHandle_t xEventGroup,
                                          स्थिर EventBits_t uxBitsToSet,
                                          BaseType_t * pxHigherPriorityTaskWoken )
    {
        BaseType_t xReturn;

        traceEVENT_GROUP_SET_BITS_FROM_ISR( xEventGroup, uxBitsToSet );
        xReturn = xTimerPendFunctionCallFromISR( vEventGroupSetBitsCallback, ( व्योम * ) xEventGroup, ( uint32_t ) uxBitsToSet, pxHigherPriorityTaskWoken ); /*lint !e9087 Can't avoid cast to void* as a generic callback function not specific to this use case. Callback casts back to original type so safe. */

        वापस xReturn;
    }

#पूर्ण_अगर /* if ( ( configUSE_TRACE_FACILITY == 1 ) && ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 1 ) ) */
/*-----------------------------------------------------------*/

#अगर ( configUSE_TRACE_FACILITY == 1 )

    UBaseType_t ux_Event_Group_Get_संख्या( व्योम * xEventGroup )
    {
        UBaseType_t xReturn;
        EventGroup_t स्थिर * pxEventBits = ( EventGroup_t * ) xEventGroup; /*lint !e9087 !e9079 EventGroupHandle_t is a pointer to an EventGroup_t, but EventGroupHandle_t is kept opaque outside of this file for data hiding purposes. */

        अगर( xEventGroup == शून्य )
        {
            xReturn = 0;
        }
        अन्यथा
        {
            xReturn = pxEventBits->ux_Event_Group_संख्या;
        }

        वापस xReturn;
    }

#पूर्ण_अगर /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#अगर ( configUSE_TRACE_FACILITY == 1 )

    व्योम v_Event_Group_Set_संख्या( व्योम * xEventGroup,
                               UBaseType_t ux_Event_Group_संख्या )
    {
        ( ( EventGroup_t * ) xEventGroup )->ux_Event_Group_संख्या = ux_Event_Group_संख्या; /*lint !e9087 !e9079 EventGroupHandle_t is a pointer to an EventGroup_t, but EventGroupHandle_t is kept opaque outside of this file for data hiding purposes. */
    }

#पूर्ण_अगर /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/
