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

#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#define MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#अन्तर्भाव "FreeRTOS.h"
#अन्तर्भाव "task.h"
#अन्तर्भाव "queue.h"

#यदि ( configUSE_CO_ROUTINES == 1 )
    #अन्तर्भाव "croutine.h"
#endif

/* The MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#undef MPU_WRAPPERS_INCLUDED_FROM_API_FILE


/* Constants used with the cRxLock and cTxLock structure members. */
#define queueUNLOCKED             ( ( int8_t ) -1 )
#define queueLOCKED_UNMODIFIED    ( ( int8_t ) 0 )
#define queueINT8_MAX             ( ( int8_t ) 127 )

/* When the Queue_t structure is used to represent a base queue its pcHead and
 * pcTail members are used as pointers into the queue storage area.  When the
 * Queue_t structure is used to represent a mutex pcHead and pcTail pointers are
 * not necessary, and the pcHead pointer is set to NULL to indicate that the
 * structure instead holds a pointer to the mutex holder (if any).  Map alternative
 * names to the pcHead and structure member to ensure the readability of the code
 * is maintained.  The QueuePointers_t and SemaphoreData_t types are used to form
 * a union as their usage is mutually exclusive dependent on what the queue is
 * being used for. */
#define uxQueueType               pcHead
#define queueQUEUE_IS_MUTEX       रिक्त

प्रकारनिर्देश संरचना QueuePointers
{
    int8_t * pcTail;     /**< Points to the byte at the end of the queue storage area.  Once more byte is allocated than necessary to store the queue items, this is used as a marker. */
    int8_t * pcReadFrom; /**< Points to the last place that a queued item was read from when the structure is used as a queue. */
} QueuePointers_t;

प्रकारनिर्देश संरचना SemaphoreData
{
    TaskHandle_t xMutexHolder;        /**< The handle of the task that holds the mutex. */
    UBaseType_t ux_Recursive_Call_मोजणी; /**< Maintains a count of the number of times a recursive mutex has been recursively 'taken' when the structure is used as a mutex. */
} SemaphoreData_t;

/* Semaphores do not actually store or copy data, so have an item size of
 * zero. */
#define queueSEMAPHORE_QUEUE_ITEM_LENGTH    ( ( UBaseType_t ) 0 )
#define queueMUTEX_GIVE_BLOCK_TIME          ( ( TickType_t ) 0U )

#यदि ( configUSE_PREEMPTION == 0 )

/* If the cooperative scheduler is being used then a yield should not be
 * performed just because a higher priority task has been woken. */
    #define queueYIELD_IF_USING_PREEMPTION()
#अन्यथा
    #यदि ( config_संख्या_OF_CORES == 1 )
        #define queueYIELD_IF_USING_PREEMPTION()    portYIELD_WITHIN_API()
    #अन्यथा /* #if ( configNUMBER_OF_CORES == 1 ) */
        #define queueYIELD_IF_USING_PREEMPTION()    vTaskYieldWithinAPI()
    #endif /* #if ( configNUMBER_OF_CORES == 1 ) */
#endif

/*
 * Definition of the queue used by the scheduler.
 * Items are queued by copy, not reference.  See the following link for the
 * rationale: https://www.FreeRTOS.org/Embedded-RTOS-Queues.html
 */
प्रकारनिर्देश संरचना QueueDefinition /* The old naming convention is used to prevent breaking kernel aware debuggers. */
{
    int8_t * pcHead;           /**< Points to the beginning of the queue storage area. */
    int8_t * pcWriteTo;        /**< Points to the free next place in the storage area. */

    सङ्घ
    {
        QueuePointers_t xQueue;     /**< Data required exclusively when this structure is used as a queue. */
        SemaphoreData_t xSemaphore; /**< Data required exclusively when this structure is used as a semaphore. */
    } u;

    List_t xTasksWaitingToSend;             /**< List of tasks that are blocked waiting to post onto this queue.  Stored in priority order. */
    List_t xTasksWaitingToReceive;          /**< List of tasks that are blocked waiting to read from this queue.  Stored in priority order. */

    चञ्चल UBaseType_t uxMessagesWaiting; /**< The number of items currently in the queue. */
    UBaseType_t uxLength;                   /**< The length of the queue defined as the number of items it will hold, not the number of bytes. */
    UBaseType_t uxItemSize;                 /**< The size of each items that the queue will hold. */

    चञ्चल int8_t cRxLock;                /**< Stores the number of items received from the queue (removed from the queue) while the queue was locked.  Set to queueUNLOCKED when the queue is not locked. */
    चञ्चल int8_t cTxLock;                /**< Stores the number of items transmitted to the queue (added to the queue) while the queue was locked.  Set to queueUNLOCKED when the queue is not locked. */

    #यदि ( ( configSUPPORT_STATIC_ALLOCATION == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) )
        uint8_t ucStaticallyAllocated; /**< Set to pdTRUE if the memory used by the queue was statically allocated to ensure no attempt is made to free the memory. */
    #endif

    #यदि ( configUSE_QUEUE_SETS == 1 )
        संरचना QueueDefinition * pxQueueSetContainer;
    #endif

    #यदि ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_Queue_संख्या;
        uint8_t ucQueueType;
    #endif
} xQUEUE;

/* The old xQUEUE name is maintained above then typedefed to the new Queue_t
 * name below to enable the use of older kernel aware debuggers. */
प्रकारनिर्देश xQUEUE Queue_t;

/*-----------------------------------------------------------*/

/*
 * The queue registry is just a means for kernel aware debuggers to locate
 * queue structures.  It has no other purpose so is an optional component.
 */
#यदि ( configQUEUE_REGISTRY_SIZE > 0 )

/* The type stored within the queue registry array.  This allows a name
 * to be assigned to each queue making kernel aware debugging a little
 * more user friendly. */
    प्रकारनिर्देश संरचना QUEUE_REGISTRY_ITEM
    {
        ध्रुव वर्ण * pcQueueName;
        QueueHandle_t xHandle;
    } xQueueRegistryItem;

/* The old xQueueRegistryItem name is maintained above then typedefed to the
 * new xQueueRegistryItem name below to enable the use of older kernel aware
 * debuggers. */
    प्रकारनिर्देश xQueueRegistryItem QueueRegistryItem_t;

/* The queue registry is simply an array of QueueRegistryItem_t structures.
 * The pcQueueName member of a structure being NULL is indicative of the
 * array position being vacant. */

/* MISRA Ref 8.4.2 [Declaration shall be visible] */
/* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-84 */
/* coverity[misra_c_2012_rule_8_4_violation] */
    PRIVILEGED_DATA QueueRegistryItem_t xQueueRegistry[ configQUEUE_REGISTRY_SIZE ];

#endif /* configQUEUE_REGISTRY_SIZE */

/*
 * Unlocks a queue locked by a call to prvLockQueue.  Locking a queue does not
 * prevent an ISR from adding or removing items to the queue, but does prevent
 * an ISR from removing tasks from the queue event lists.  If an ISR finds a
 * queue is locked it will instead increment the appropriate queue lock count
 * to indicate that a task may require unblocking.  When the queue in unlocked
 * these lock counts are inspected, and the appropriate action taken.
 */
स्थिर शून्य prvUnlockQueue( Queue_t * ध्रुव pxQueue ) PRIVILEGED_FUNCTION;

/*
 * Uses a critical section to determine if there is any data in a queue.
 *
 * @return pdTRUE if the queue contains no items, otherwise pdFALSE.
 */
स्थिर BaseType_t prvIsQueueEmpty( ध्रुव Queue_t * pxQueue ) PRIVILEGED_FUNCTION;

/*
 * Uses a critical section to determine if there is any space in a queue.
 *
 * @return pdTRUE if there is no space, otherwise pdFALSE;
 */
स्थिर BaseType_t prvIsQueueFull( ध्रुव Queue_t * pxQueue ) PRIVILEGED_FUNCTION;

/*
 * Copies an item into the queue, either at the front of the queue or the
 * back of the queue.
 */
स्थिर BaseType_t prvCopyDataToQueue( Queue_t * ध्रुव pxQueue,
                                      ध्रुव शून्य * pvItemToQueue,
                                      ध्रुव BaseType_t xPosition ) PRIVILEGED_FUNCTION;

/*
 * Copies an item out of a queue.
 */
स्थिर शून्य prvCopyDataFromQueue( Queue_t * ध्रुव pxQueue,
                                  शून्य * ध्रुव pvBuffer ) PRIVILEGED_FUNCTION;

#यदि ( configUSE_QUEUE_SETS == 1 )

/*
 * Checks to see if a queue is a member of a queue set, and if so, notifies
 * the queue set that the queue contains data.
 */
    स्थिर BaseType_t prvNotifyQueueSetContainer( ध्रुव Queue_t * ध्रुव pxQueue ) PRIVILEGED_FUNCTION;
#endif

/*
 * Called after a Queue_t structure has been allocated either statically or
 * dynamically to fill in the structure's members.
 */
स्थिर शून्य prvInitialiseNewQueue( ध्रुव UBaseType_t uxQueueLength,
                                   ध्रुव UBaseType_t uxItemSize,
                                   uint8_t * pucQueueStorage,
                                   ध्रुव uint8_t ucQueueType,
                                   Queue_t * pxNewQueue ) PRIVILEGED_FUNCTION;

/*
 * Mutexes are a special type of queue.  When a mutex is created, first the
 * queue is created, then prvInitialiseMutex() is called to configure the queue
 * as a mutex.
 */
#यदि ( configUSE_MUTEXES == 1 )
    स्थिर शून्य prvInitialiseMutex( Queue_t * pxNewQueue ) PRIVILEGED_FUNCTION;
#endif

#यदि ( configUSE_MUTEXES == 1 )

/*
 * If a task waiting for a mutex causes the mutex holder to inherit a
 * priority, but the waiting task times out, then the holder should
 * disinherit the priority - but only down to the highest priority of any
 * other tasks that are waiting for the same mutex.  This function returns
 * that priority.
 */
    स्थिर UBaseType_t prvGetDisinheritPriorityAfterTimeout( ध्रुव Queue_t * ध्रुव pxQueue ) PRIVILEGED_FUNCTION;
#endif
/*-----------------------------------------------------------*/

/*
 * Macro to mark a queue as locked.  Locking a queue prevents an ISR from
 * accessing the queue event lists.
 */
#define prvLockQueue( pxQueue )                            \
    taskENTER_CRITICAL();                                  \
    {                                                      \
        यदि( ( pxQueue )->cRxLock == queueUNLOCKED )        \
        {                                                  \
            ( pxQueue )->cRxLock = queueLOCKED_UNMODIFIED; \
        }                                                  \
        यदि( ( pxQueue )->cTxLock == queueUNLOCKED )        \
        {                                                  \
            ( pxQueue )->cTxLock = queueLOCKED_UNMODIFIED; \
        }                                                  \
    }                                                      \
    taskEXIT_CRITICAL()

/*
 * Macro to increment cTxLock member of the queue data structure. It is
 * capped at the number of tasks in the system as we cannot unblock more
 * tasks than the number of tasks in the system.
 */
#define prvIncrementQueueTxLock( pxQueue, cTxLock )                           \
    कुरु {                                                                      \
        ध्रुव UBaseType_t ux_संख्या_Of_Tasks = ux_Task_Get_संख्या_Of_Tasks();         \
        यदि( ( UBaseType_t ) ( cTxLock ) < ux_संख्या_Of_Tasks )                   \
        {                                                                     \
            configASSERT( ( cTxLock ) != queueINT8_MAX );                     \
            ( pxQueue )->cTxLock = ( int8_t ) ( ( cTxLock ) + ( int8_t ) 1 ); \
        }                                                                     \
    } यावत्( 0 )

/*
 * Macro to increment cRxLock member of the queue data structure. It is
 * capped at the number of tasks in the system as we cannot unblock more
 * tasks than the number of tasks in the system.
 */
#define prvIncrementQueueRxLock( pxQueue, cRxLock )                           \
    कुरु {                                                                      \
        ध्रुव UBaseType_t ux_संख्या_Of_Tasks = ux_Task_Get_संख्या_Of_Tasks();         \
        यदि( ( UBaseType_t ) ( cRxLock ) < ux_संख्या_Of_Tasks )                   \
        {                                                                     \
            configASSERT( ( cRxLock ) != queueINT8_MAX );                     \
            ( pxQueue )->cRxLock = ( int8_t ) ( ( cRxLock ) + ( int8_t ) 1 ); \
        }                                                                     \
    } यावत्( 0 )
/*-----------------------------------------------------------*/

BaseType_t xQueueGenericReset( QueueHandle_t xQueue,
                               BaseType_t xNewQueue )
{
    BaseType_t xReturn = pdPASS;
    Queue_t * ध्रुव pxQueue = xQueue;

    traceENTER_xQueueGenericReset( xQueue, xNewQueue );

    configASSERT( pxQueue );

    यदि( ( pxQueue != रिक्त ) &&
        ( pxQueue->uxLength >= 1U ) &&
        /* Check for multiplication overflow. */
        ( ( SIZE_MAX / pxQueue->uxLength ) >= pxQueue->uxItemSize ) )
    {
        taskENTER_CRITICAL();
        {
            pxQueue->u.xQueue.pcTail = pxQueue->pcHead + ( pxQueue->uxLength * pxQueue->uxItemSize );
            pxQueue->uxMessagesWaiting = ( UBaseType_t ) 0U;
            pxQueue->pcWriteTo = pxQueue->pcHead;
            pxQueue->u.xQueue.pcReadFrom = pxQueue->pcHead + ( ( pxQueue->uxLength - 1U ) * pxQueue->uxItemSize );
            pxQueue->cRxLock = queueUNLOCKED;
            pxQueue->cTxLock = queueUNLOCKED;

            यदि( xNewQueue == pdFALSE )
            {
                /* If there are tasks blocked waiting to read from the queue, then
                 * the tasks will remain blocked as after this function exits the queue
                 * will still be empty.  If there are tasks blocked waiting to write to
                 * the queue, then one should be unblocked as after this function exits
                 * it will be possible to write to it. */
                यदि( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToSend ) ) == pdFALSE )
                {
                    यदि( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToSend ) ) != pdFALSE )
                    {
                        queueYIELD_IF_USING_PREEMPTION();
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
            }
            अन्यथा
            {
                /* Ensure the event queues start in the correct state. */
                vListInitialise( &( pxQueue->xTasksWaitingToSend ) );
                vListInitialise( &( pxQueue->xTasksWaitingToReceive ) );
            }
        }
        taskEXIT_CRITICAL();
    }
    अन्यथा
    {
        xReturn = pdFAIL;
    }

    configASSERT( xReturn != pdFAIL );

    /* A value is returned for calling semantic consistency with previous
     * versions. */
    traceRETURN_xQueueGenericReset( xReturn );

    प्रत्यागम xReturn;
}
/*-----------------------------------------------------------*/

#यदि ( configSUPPORT_STATIC_ALLOCATION == 1 )

    QueueHandle_t xQueueGenericCreateStatic( ध्रुव UBaseType_t uxQueueLength,
                                             ध्रुव UBaseType_t uxItemSize,
                                             uint8_t * pucQueueStorage,
                                             StaticQueue_t * pxStaticQueue,
                                             ध्रुव uint8_t ucQueueType )
    {
        Queue_t * pxNewQueue = रिक्त;

        traceENTER_xQueueGenericCreateStatic( uxQueueLength, uxItemSize, pucQueueStorage, pxStaticQueue, ucQueueType );

        /* The StaticQueue_t structure and the queue storage area must be
         * supplied. */
        configASSERT( pxStaticQueue );

        यदि( ( uxQueueLength > ( UBaseType_t ) 0 ) &&
            ( pxStaticQueue != रिक्त ) &&

            /* A queue storage area should be provided if the item size is not 0, and
             * should not be provided if the item size is 0. */
            ( !( ( pucQueueStorage != रिक्त ) && ( uxItemSize == 0U ) ) ) &&
            ( !( ( pucQueueStorage == रिक्त ) && ( uxItemSize != 0U ) ) ) )
        {
            #यदि ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticQueue_t or StaticSemaphore_t equals the size of
                 * the real queue and semaphore structures. */
                चञ्चल size_t xSize = परिमाण( StaticQueue_t );

                /* This assertion cannot be branch covered in unit tests */
                configASSERT( xSize == परिमाण( Queue_t ) ); /* LCOV_EXCL_BR_LINE */
                ( शून्य ) xSize;                             /* Prevent unused variable warning when configASSERT() is not defined. */
            }
            #endif /* configASSERT_DEFINED */

            /* The address of a statically allocated queue was passed in, use it.
             * The address of a statically allocated storage area was also passed in
             * but is already set. */
            /* MISRA Ref 11.3.1 [Misaligned access] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
            /* coverity[misra_c_2012_rule_11_3_violation] */
            pxNewQueue = ( Queue_t * ) pxStaticQueue;

            #यदि ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
            {
                /* Queues can be allocated either statically or dynamically, so
                 * note this queue was allocated statically in case the queue is
                 * later deleted. */
                pxNewQueue->ucStaticallyAllocated = pdTRUE;
            }
            #endif /* configSUPPORT_DYNAMIC_ALLOCATION */

            prvInitialiseNewQueue( uxQueueLength, uxItemSize, pucQueueStorage, ucQueueType, pxNewQueue );
        }
        अन्यथा
        {
            configASSERT( pxNewQueue );
            mtCOVERAGE_TEST_MARKER();
        }

        traceRETURN_xQueueGenericCreateStatic( pxNewQueue );

        प्रत्यागम pxNewQueue;
    }

#endif /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

#यदि ( configSUPPORT_STATIC_ALLOCATION == 1 )

    BaseType_t xQueueGenericGetStaticBuffers( QueueHandle_t xQueue,
                                              uint8_t ** ppucQueueStorage,
                                              StaticQueue_t ** ppxStaticQueue )
    {
        BaseType_t xReturn;
        Queue_t * ध्रुव pxQueue = xQueue;

        traceENTER_xQueueGenericGetStaticBuffers( xQueue, ppucQueueStorage, ppxStaticQueue );

        configASSERT( pxQueue );
        configASSERT( ppxStaticQueue );

        #यदि ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
        {
            /* Check if the queue was statically allocated. */
            यदि( pxQueue->ucStaticallyAllocated == ( uint8_t ) pdTRUE )
            {
                यदि( ppucQueueStorage != रिक्त )
                {
                    *ppucQueueStorage = ( uint8_t * ) pxQueue->pcHead;
                }

                /* MISRA Ref 11.3.1 [Misaligned access] */
                /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
                /* coverity[misra_c_2012_rule_11_3_violation] */
                *ppxStaticQueue = ( StaticQueue_t * ) pxQueue;
                xReturn = pdTRUE;
            }
            अन्यथा
            {
                xReturn = pdFALSE;
            }
        }
        #अन्यथा /* configSUPPORT_DYNAMIC_ALLOCATION */
        {
            /* Queue must have been statically allocated. */
            यदि( ppucQueueStorage != रिक्त )
            {
                *ppucQueueStorage = ( uint8_t * ) pxQueue->pcHead;
            }

            *ppxStaticQueue = ( StaticQueue_t * ) pxQueue;
            xReturn = pdTRUE;
        }
        #endif /* configSUPPORT_DYNAMIC_ALLOCATION */

        traceRETURN_xQueueGenericGetStaticBuffers( xReturn );

        प्रत्यागम xReturn;
    }

#endif /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

#यदि ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )

    QueueHandle_t xQueueGenericCreate( ध्रुव UBaseType_t uxQueueLength,
                                       ध्रुव UBaseType_t uxItemSize,
                                       ध्रुव uint8_t ucQueueType )
    {
        Queue_t * pxNewQueue = रिक्त;
        size_t xQueueSizeInBytes;
        uint8_t * pucQueueStorage;

        traceENTER_xQueueGenericCreate( uxQueueLength, uxItemSize, ucQueueType );

        यदि( ( uxQueueLength > ( UBaseType_t ) 0 ) &&
            /* Check for multiplication overflow. */
            ( ( SIZE_MAX / uxQueueLength ) >= uxItemSize ) &&
            /* Check for addition overflow. */
            ( ( UBaseType_t ) ( SIZE_MAX - परिमाण( Queue_t ) ) >= ( uxQueueLength * uxItemSize ) ) )
        {
            /* Allocate enough space to hold the maximum number of items that
             * can be in the queue at any time.  It is valid for uxItemSize to be
             * zero in the case the queue is used as a semaphore. */
            xQueueSizeInBytes = ( size_t ) ( ( size_t ) uxQueueLength * ( size_t ) uxItemSize );

            /* MISRA Ref 11.5.1 [Malloc memory assignment] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
            /* coverity[misra_c_2012_rule_11_5_violation] */
            pxNewQueue = ( Queue_t * ) pvPortMalloc( परिमाण( Queue_t ) + xQueueSizeInBytes );

            यदि( pxNewQueue != रिक्त )
            {
                /* Jump past the queue structure to find the location of the queue
                 * storage area. */
                pucQueueStorage = ( uint8_t * ) pxNewQueue;
                pucQueueStorage += परिमाण( Queue_t );

                #यदि ( configSUPPORT_STATIC_ALLOCATION == 1 )
                {
                    /* Queues can be created either statically or dynamically, so
                     * note this task was created dynamically in case it is later
                     * deleted. */
                    pxNewQueue->ucStaticallyAllocated = pdFALSE;
                }
                #endif /* configSUPPORT_STATIC_ALLOCATION */

                prvInitialiseNewQueue( uxQueueLength, uxItemSize, pucQueueStorage, ucQueueType, pxNewQueue );
            }
            अन्यथा
            {
                traceQUEUE_CREATE_FAILED( ucQueueType );
                mtCOVERAGE_TEST_MARKER();
            }
        }
        अन्यथा
        {
            configASSERT( pxNewQueue );
            mtCOVERAGE_TEST_MARKER();
        }

        traceRETURN_xQueueGenericCreate( pxNewQueue );

        प्रत्यागम pxNewQueue;
    }

#endif /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

स्थिर शून्य prvInitialiseNewQueue( ध्रुव UBaseType_t uxQueueLength,
                                   ध्रुव UBaseType_t uxItemSize,
                                   uint8_t * pucQueueStorage,
                                   ध्रुव uint8_t ucQueueType,
                                   Queue_t * pxNewQueue )
{
    /* Remove compiler warnings about unused parameters should
     * configUSE_TRACE_FACILITY not be set to 1. */
    ( शून्य ) ucQueueType;

    यदि( uxItemSize == ( UBaseType_t ) 0 )
    {
        /* No RAM was allocated for the queue storage area, but PC head cannot
         * be set to NULL because NULL is used as a key to say the queue is used as
         * a mutex.  Therefore just set pcHead to point to the queue as a benign
         * value that is known to be within the memory map. */
        pxNewQueue->pcHead = ( int8_t * ) pxNewQueue;
    }
    अन्यथा
    {
        /* Set the head to the start of the queue storage area. */
        pxNewQueue->pcHead = ( int8_t * ) pucQueueStorage;
    }

    /* Initialise the queue members as described where the queue type is
     * defined. */
    pxNewQueue->uxLength = uxQueueLength;
    pxNewQueue->uxItemSize = uxItemSize;
    ( शून्य ) xQueueGenericReset( pxNewQueue, pdTRUE );

    #यदि ( configUSE_TRACE_FACILITY == 1 )
    {
        pxNewQueue->ucQueueType = ucQueueType;
    }
    #endif /* configUSE_TRACE_FACILITY */

    #यदि ( configUSE_QUEUE_SETS == 1 )
    {
        pxNewQueue->pxQueueSetContainer = रिक्त;
    }
    #endif /* configUSE_QUEUE_SETS */

    traceQUEUE_CREATE( pxNewQueue );
}
/*-----------------------------------------------------------*/

#यदि ( configUSE_MUTEXES == 1 )

    स्थिर शून्य prvInitialiseMutex( Queue_t * pxNewQueue )
    {
        यदि( pxNewQueue != रिक्त )
        {
            /* The queue create function will set all the queue structure members
            * correctly for a generic queue, but this function is creating a
            * mutex.  Overwrite those members that need to be set differently -
            * in particular the information required for priority inheritance. */
            pxNewQueue->u.xSemaphore.xMutexHolder = रिक्त;
            pxNewQueue->uxQueueType = queueQUEUE_IS_MUTEX;

            /* In case this is a recursive mutex. */
            pxNewQueue->u.xSemaphore.ux_Recursive_Call_मोजणी = 0;

            traceCREATE_MUTEX( pxNewQueue );

            /* Start with the semaphore in the expected state. */
            ( शून्य ) xQueueGenericSend( pxNewQueue, रिक्त, ( TickType_t ) 0U, queueSEND_TO_BACK );
        }
        अन्यथा
        {
            traceCREATE_MUTEX_FAILED();
        }
    }

#endif /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

#यदि ( ( configUSE_MUTEXES == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) )

    QueueHandle_t xQueueCreateMutex( ध्रुव uint8_t ucQueueType )
    {
        QueueHandle_t xNewQueue;
        ध्रुव UBaseType_t uxMutexLength = ( UBaseType_t ) 1, uxMutexSize = ( UBaseType_t ) 0;

        traceENTER_xQueueCreateMutex( ucQueueType );

        xNewQueue = xQueueGenericCreate( uxMutexLength, uxMutexSize, ucQueueType );
        prvInitialiseMutex( ( Queue_t * ) xNewQueue );

        traceRETURN_xQueueCreateMutex( xNewQueue );

        प्रत्यागम xNewQueue;
    }

#endif /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

#यदि ( ( configUSE_MUTEXES == 1 ) && ( configSUPPORT_STATIC_ALLOCATION == 1 ) )

    QueueHandle_t xQueueCreateMutexStatic( ध्रुव uint8_t ucQueueType,
                                           StaticQueue_t * pxStaticQueue )
    {
        QueueHandle_t xNewQueue;
        ध्रुव UBaseType_t uxMutexLength = ( UBaseType_t ) 1, uxMutexSize = ( UBaseType_t ) 0;

        traceENTER_xQueueCreateMutexStatic( ucQueueType, pxStaticQueue );

        /* Prevent compiler warnings about unused parameters if
         * configUSE_TRACE_FACILITY does not equal 1. */
        ( शून्य ) ucQueueType;

        xNewQueue = xQueueGenericCreateStatic( uxMutexLength, uxMutexSize, रिक्त, pxStaticQueue, ucQueueType );
        prvInitialiseMutex( ( Queue_t * ) xNewQueue );

        traceRETURN_xQueueCreateMutexStatic( xNewQueue );

        प्रत्यागम xNewQueue;
    }

#endif /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

#यदि ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) )

    TaskHandle_t xQueueGetMutexHolder( QueueHandle_t xSemaphore )
    {
        TaskHandle_t pxReturn;
        Queue_t * ध्रुव pxSemaphore = ( Queue_t * ) xSemaphore;

        traceENTER_xQueueGetMutexHolder( xSemaphore );

        configASSERT( xSemaphore );

        /* This function is called by xSemaphoreGetMutexHolder(), and should not
         * be called directly.  Note:  This is a good way of determining if the
         * calling task is the mutex holder, but not a good way of determining the
         * identity of the mutex holder, as the holder may change between the
         * following critical section exiting and the function returning. */
        taskENTER_CRITICAL();
        {
            यदि( pxSemaphore->uxQueueType == queueQUEUE_IS_MUTEX )
            {
                pxReturn = pxSemaphore->u.xSemaphore.xMutexHolder;
            }
            अन्यथा
            {
                pxReturn = रिक्त;
            }
        }
        taskEXIT_CRITICAL();

        traceRETURN_xQueueGetMutexHolder( pxReturn );

        प्रत्यागम pxReturn;
    }

#endif /* if ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) ) */
/*-----------------------------------------------------------*/

#यदि ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) )

    TaskHandle_t xQueueGetMutexHolderFromISR( QueueHandle_t xSemaphore )
    {
        TaskHandle_t pxReturn;

        traceENTER_xQueueGetMutexHolderFromISR( xSemaphore );

        configASSERT( xSemaphore );

        /* Mutexes cannot be used in interrupt service routines, so the mutex
         * holder should not change in an ISR, and therefore a critical section is
         * not required here. */
        यदि( ( ( Queue_t * ) xSemaphore )->uxQueueType == queueQUEUE_IS_MUTEX )
        {
            pxReturn = ( ( Queue_t * ) xSemaphore )->u.xSemaphore.xMutexHolder;
        }
        अन्यथा
        {
            pxReturn = रिक्त;
        }

        traceRETURN_xQueueGetMutexHolderFromISR( pxReturn );

        प्रत्यागम pxReturn;
    }

#endif /* if ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) ) */
/*-----------------------------------------------------------*/

#यदि ( configUSE_RECURSIVE_MUTEXES == 1 )

    BaseType_t xQueueGiveMutexRecursive( QueueHandle_t xMutex )
    {
        BaseType_t xReturn;
        Queue_t * ध्रुव pxMutex = ( Queue_t * ) xMutex;

        traceENTER_xQueueGiveMutexRecursive( xMutex );

        configASSERT( pxMutex );

        /* If this is the task that holds the mutex then xMutexHolder will not
         * change outside of this task.  If this task does not hold the mutex then
         * pxMutexHolder can never coincidentally equal the tasks handle, and as
         * this is the only condition we are interested in it does not matter if
         * pxMutexHolder is accessed simultaneously by another task.  Therefore no
         * mutual exclusion is required to test the pxMutexHolder variable. */
        यदि( pxMutex->u.xSemaphore.xMutexHolder == xTaskGetCurrentTaskHandle() )
        {
            traceGIVE_MUTEX_RECURSIVE( pxMutex );

            /* uxRecursiveCallCount cannot be zero if xMutexHolder is equal to
             * the task handle, therefore no underflow check is required.  Also,
             * uxRecursiveCallCount is only modified by the mutex holder, and as
             * there can only be one, no mutual exclusion is required to modify the
             * uxRecursiveCallCount member. */
            ( pxMutex->u.xSemaphore.ux_Recursive_Call_मोजणी )--;

            /* Has the recursive call count unwound to 0? */
            यदि( pxMutex->u.xSemaphore.ux_Recursive_Call_मोजणी == ( UBaseType_t ) 0 )
            {
                /* Return the mutex.  This will automatically unblock any other
                 * task that might be waiting to access the mutex. */
                ( शून्य ) xQueueGenericSend( pxMutex, रिक्त, queueMUTEX_GIVE_BLOCK_TIME, queueSEND_TO_BACK );
            }
            अन्यथा
            {
                mtCOVERAGE_TEST_MARKER();
            }

            xReturn = pdPASS;
        }
        अन्यथा
        {
            /* The mutex cannot be given because the calling task is not the
             * holder. */
            xReturn = pdFAIL;

            traceGIVE_MUTEX_RECURSIVE_FAILED( pxMutex );
        }

        traceRETURN_xQueueGiveMutexRecursive( xReturn );

        प्रत्यागम xReturn;
    }

#endif /* configUSE_RECURSIVE_MUTEXES */
/*-----------------------------------------------------------*/

#यदि ( configUSE_RECURSIVE_MUTEXES == 1 )

    BaseType_t xQueueTakeMutexRecursive( QueueHandle_t xMutex,
                                         TickType_t xTicksToWait )
    {
        BaseType_t xReturn;
        Queue_t * ध्रुव pxMutex = ( Queue_t * ) xMutex;

        traceENTER_xQueueTakeMutexRecursive( xMutex, xTicksToWait );

        configASSERT( pxMutex );

        /* Comments regarding mutual exclusion as per those within
         * xQueueGiveMutexRecursive(). */

        traceTAKE_MUTEX_RECURSIVE( pxMutex );

        यदि( pxMutex->u.xSemaphore.xMutexHolder == xTaskGetCurrentTaskHandle() )
        {
            ( pxMutex->u.xSemaphore.ux_Recursive_Call_मोजणी )++;
            xReturn = pdPASS;
        }
        अन्यथा
        {
            xReturn = xQueueSemaphoreTake( pxMutex, xTicksToWait );

            /* pdPASS will only be returned if the mutex was successfully
             * obtained.  The calling task may have entered the Blocked state
             * before reaching here. */
            यदि( xReturn != pdFAIL )
            {
                ( pxMutex->u.xSemaphore.ux_Recursive_Call_मोजणी )++;
            }
            अन्यथा
            {
                traceTAKE_MUTEX_RECURSIVE_FAILED( pxMutex );
            }
        }

        traceRETURN_xQueueTakeMutexRecursive( xReturn );

        प्रत्यागम xReturn;
    }

#endif /* configUSE_RECURSIVE_MUTEXES */
/*-----------------------------------------------------------*/

#यदि ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( configSUPPORT_STATIC_ALLOCATION == 1 ) )

    QueueHandle_t xQueueCreateCountingSemaphoreStatic( ध्रुव UBaseType_t ux_Max_मोजणी,
                                                       ध्रुव UBaseType_t ux_Initial_मोजणी,
                                                       StaticQueue_t * pxStaticQueue )
    {
        QueueHandle_t xHandle = रिक्त;

        traceENTER_xQueueCreateCountingSemaphoreStatic( ux_Max_मोजणी, ux_Initial_मोजणी, pxStaticQueue );

        यदि( ( ux_Max_मोजणी != 0U ) &&
            ( ux_Initial_मोजणी <= ux_Max_मोजणी ) )
        {
            xHandle = xQueueGenericCreateStatic( ux_Max_मोजणी, queueSEMAPHORE_QUEUE_ITEM_LENGTH, रिक्त, pxStaticQueue, queueQUEUE_TYPE_COUNTING_SEMAPHORE );

            यदि( xHandle != रिक्त )
            {
                ( ( Queue_t * ) xHandle )->uxMessagesWaiting = ux_Initial_मोजणी;

                traceCREATE_COUNTING_SEMAPHORE();
            }
            अन्यथा
            {
                traceCREATE_COUNTING_SEMAPHORE_FAILED();
            }
        }
        अन्यथा
        {
            configASSERT( xHandle );
            mtCOVERAGE_TEST_MARKER();
        }

        traceRETURN_xQueueCreateCountingSemaphoreStatic( xHandle );

        प्रत्यागम xHandle;
    }

#endif /* ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) ) */
/*-----------------------------------------------------------*/

#यदि ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) )

    QueueHandle_t xQueueCreateCountingSemaphore( ध्रुव UBaseType_t ux_Max_मोजणी,
                                                 ध्रुव UBaseType_t ux_Initial_मोजणी )
    {
        QueueHandle_t xHandle = रिक्त;

        traceENTER_xQueueCreateCountingSemaphore( ux_Max_मोजणी, ux_Initial_मोजणी );

        यदि( ( ux_Max_मोजणी != 0U ) &&
            ( ux_Initial_मोजणी <= ux_Max_मोजणी ) )
        {
            xHandle = xQueueGenericCreate( ux_Max_मोजणी, queueSEMAPHORE_QUEUE_ITEM_LENGTH, queueQUEUE_TYPE_COUNTING_SEMAPHORE );

            यदि( xHandle != रिक्त )
            {
                ( ( Queue_t * ) xHandle )->uxMessagesWaiting = ux_Initial_मोजणी;

                traceCREATE_COUNTING_SEMAPHORE();
            }
            अन्यथा
            {
                traceCREATE_COUNTING_SEMAPHORE_FAILED();
            }
        }
        अन्यथा
        {
            configASSERT( xHandle );
            mtCOVERAGE_TEST_MARKER();
        }

        traceRETURN_xQueueCreateCountingSemaphore( xHandle );

        प्रत्यागम xHandle;
    }

#endif /* ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) ) */
/*-----------------------------------------------------------*/

BaseType_t xQueueGenericSend( QueueHandle_t xQueue,
                              ध्रुव शून्य * ध्रुव pvItemToQueue,
                              TickType_t xTicksToWait,
                              ध्रुव BaseType_t xCopyPosition )
{
    BaseType_t xEntryTimeSet = pdFALSE, xYieldRequired;
    TimeOut_t xTimeOut;
    Queue_t * ध्रुव pxQueue = xQueue;

    traceENTER_xQueueGenericSend( xQueue, pvItemToQueue, xTicksToWait, xCopyPosition );

    configASSERT( pxQueue );
    configASSERT( !( ( pvItemToQueue == रिक्त ) && ( pxQueue->uxItemSize != ( UBaseType_t ) 0U ) ) );
    configASSERT( !( ( xCopyPosition == queueOVERWRITE ) && ( pxQueue->uxLength != 1 ) ) );
    #यदि ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( xTicksToWait != 0 ) ) );
    }
    #endif

    प्रति( ; ; )
    {
        taskENTER_CRITICAL();
        {
            /* Is there room on the queue now?  The running task must be the
             * highest priority task wanting to access the queue.  If the head item
             * in the queue is to be overwritten then it does not matter if the
             * queue is full. */
            यदि( ( pxQueue->uxMessagesWaiting < pxQueue->uxLength ) || ( xCopyPosition == queueOVERWRITE ) )
            {
                traceQUEUE_SEND( pxQueue );

                #यदि ( configUSE_QUEUE_SETS == 1 )
                {
                    ध्रुव UBaseType_t uxPreviousMessagesWaiting = pxQueue->uxMessagesWaiting;

                    xYieldRequired = prvCopyDataToQueue( pxQueue, pvItemToQueue, xCopyPosition );

                    यदि( pxQueue->pxQueueSetContainer != रिक्त )
                    {
                        यदि( ( xCopyPosition == queueOVERWRITE ) && ( uxPreviousMessagesWaiting != ( UBaseType_t ) 0 ) )
                        {
                            /* Do not notify the queue set as an existing item
                             * was overwritten in the queue so the number of items
                             * in the queue has not changed. */
                            mtCOVERAGE_TEST_MARKER();
                        }
                        अन्यथा यदि( prvNotifyQueueSetContainer( pxQueue ) != pdFALSE )
                        {
                            /* The queue is a member of a queue set, and posting
                             * to the queue set caused a higher priority task to
                             * unblock. A context switch is required. */
                            queueYIELD_IF_USING_PREEMPTION();
                        }
                        अन्यथा
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    अन्यथा
                    {
                        /* If there was a task waiting for data to arrive on the
                         * queue then unblock it now. */
                        यदि( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToReceive ) ) == pdFALSE )
                        {
                            यदि( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToReceive ) ) != pdFALSE )
                            {
                                /* The unblocked task has a priority higher than
                                 * our own so yield immediately.  Yes it is ok to
                                 * do this from within the critical section - the
                                 * kernel takes care of that. */
                                queueYIELD_IF_USING_PREEMPTION();
                            }
                            अन्यथा
                            {
                                mtCOVERAGE_TEST_MARKER();
                            }
                        }
                        अन्यथा यदि( xYieldRequired != pdFALSE )
                        {
                            /* This path is a special case that will only get
                             * executed if the task was holding multiple mutexes
                             * and the mutexes were given back in an order that is
                             * different to that in which they were taken. */
                            queueYIELD_IF_USING_PREEMPTION();
                        }
                        अन्यथा
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                }
                #अन्यथा /* configUSE_QUEUE_SETS */
                {
                    xYieldRequired = prvCopyDataToQueue( pxQueue, pvItemToQueue, xCopyPosition );

                    /* If there was a task waiting for data to arrive on the
                     * queue then unblock it now. */
                    यदि( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToReceive ) ) == pdFALSE )
                    {
                        यदि( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToReceive ) ) != pdFALSE )
                        {
                            /* The unblocked task has a priority higher than
                             * our own so yield immediately.  Yes it is ok to do
                             * this from within the critical section - the kernel
                             * takes care of that. */
                            queueYIELD_IF_USING_PREEMPTION();
                        }
                        अन्यथा
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    अन्यथा यदि( xYieldRequired != pdFALSE )
                    {
                        /* This path is a special case that will only get
                         * executed if the task was holding multiple mutexes and
                         * the mutexes were given back in an order that is
                         * different to that in which they were taken. */
                        queueYIELD_IF_USING_PREEMPTION();
                    }
                    अन्यथा
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #endif /* configUSE_QUEUE_SETS */

                taskEXIT_CRITICAL();

                traceRETURN_xQueueGenericSend( pdPASS );

                प्रत्यागम pdPASS;
            }
            अन्यथा
            {
                यदि( xTicksToWait == ( TickType_t ) 0 )
                {
                    /* The queue was full and no block time is specified (or
                     * the block time has expired) so leave now. */
                    taskEXIT_CRITICAL();

                    /* Return to the original privilege level before exiting
                     * the function. */
                    traceQUEUE_SEND_FAILED( pxQueue );
                    traceRETURN_xQueueGenericSend( errQUEUE_FULL );

                    प्रत्यागम errQUEUE_FULL;
                }
                अन्यथा यदि( xEntryTimeSet == pdFALSE )
                {
                    /* The queue was full and a block time was specified so
                     * configure the timeout structure. */
                    vTaskInternalSetTimeOutState( &xTimeOut );
                    xEntryTimeSet = pdTRUE;
                }
                अन्यथा
                {
                    /* Entry time was already set. */
                    mtCOVERAGE_TEST_MARKER();
                }
            }
        }
        taskEXIT_CRITICAL();

        /* Interrupts and other tasks can send to and receive from the queue
         * now the critical section has been exited. */

        vTaskSuspendAll();
        prvLockQueue( pxQueue );

        /* Update the timeout state to see if it has expired yet. */
        यदि( xTaskCheckForTimeOut( &xTimeOut, &xTicksToWait ) == pdFALSE )
        {
            यदि( prvIsQueueFull( pxQueue ) != pdFALSE )
            {
                traceBLOCKING_ON_QUEUE_SEND( pxQueue );
                vTaskPlaceOnEventList( &( pxQueue->xTasksWaitingToSend ), xTicksToWait );

                /* Unlocking the queue means queue events can effect the
                 * event list. It is possible that interrupts occurring now
                 * remove this task from the event list again - but as the
                 * scheduler is suspended the task will go onto the pending
                 * ready list instead of the actual ready list. */
                prvUnlockQueue( pxQueue );

                /* Resuming the scheduler will move tasks from the pending
                 * ready list into the ready list - so it is feasible that this
                 * task is already in the ready list before it yields - in which
                 * case the yield will not cause a context switch unless there
                 * is also a higher priority task in the pending ready list. */
                यदि( xTaskResumeAll() == pdFALSE )
                {
                    taskYIELD_WITHIN_API();
                }
            }
            अन्यथा
            {
                /* Try again. */
                prvUnlockQueue( pxQueue );
                ( शून्य ) xTaskResumeAll();
            }
        }
        अन्यथा
        {
            /* The timeout has expired. */
            prvUnlockQueue( pxQueue );
            ( शून्य ) xTaskResumeAll();

            traceQUEUE_SEND_FAILED( pxQueue );
            traceRETURN_xQueueGenericSend( errQUEUE_FULL );

            प्रत्यागम errQUEUE_FULL;
        }
    }
}
/*-----------------------------------------------------------*/

BaseType_t xQueueGenericSendFromISR( QueueHandle_t xQueue,
                                     ध्रुव शून्य * ध्रुव pvItemToQueue,
                                     BaseType_t * ध्रुव pxHigherPriorityTaskWoken,
                                     ध्रुव BaseType_t xCopyPosition )
{
    BaseType_t xReturn;
    UBaseType_t uxSavedInterruptStatus;
    Queue_t * ध्रुव pxQueue = xQueue;

    traceENTER_xQueueGenericSendFromISR( xQueue, pvItemToQueue, pxHigherPriorityTaskWoken, xCopyPosition );

    configASSERT( pxQueue );
    configASSERT( !( ( pvItemToQueue == रिक्त ) && ( pxQueue->uxItemSize != ( UBaseType_t ) 0U ) ) );
    configASSERT( !( ( xCopyPosition == queueOVERWRITE ) && ( pxQueue->uxLength != 1 ) ) );

    /* RTOS ports that support interrupt nesting have the concept of a maximum
     * system call (or maximum API call) interrupt priority.  Interrupts that are
     * above the maximum system call priority are kept permanently enabled, even
     * when the RTOS kernel is in a critical section, but cannot make any calls to
     * FreeRTOS API functions.  If configASSERT() is defined in FreeRTOSConfig.h
     * then portASSERT_IF_INTERRUPT_PRIORITY_INVALID() will result in an assertion
     * failure if a FreeRTOS API function is called from an interrupt that has been
     * assigned a priority above the configured maximum system call priority.
     * Only FreeRTOS functions that end in FromISR can be called from interrupts
     * that have been assigned a priority at or (logically) below the maximum
     * system call interrupt priority.  FreeRTOS maintains a separate interrupt
     * safe API to ensure interrupt entry is as fast and as simple as possible.
     * More information (albeit Cortex-M specific) is provided on the following
     * link: https://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html */
    portASSERT_IF_INTERRUPT_PRIORITY_INVALID();

    /* Similar to xQueueGenericSend, except without blocking if there is no room
     * in the queue.  Also don't directly wake a task that was blocked on a queue
     * read, instead return a flag to say whether a context switch is required or
     * not (i.e. has a task with a higher priority than us been woken by this
     * post). */
    /* MISRA Ref 4.7.1 [Return value shall be checked] */
    /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
    /* coverity[misra_c_2012_directive_4_7_violation] */
    uxSavedInterruptStatus = ( UBaseType_t ) taskENTER_CRITICAL_FROM_ISR();
    {
        यदि( ( pxQueue->uxMessagesWaiting < pxQueue->uxLength ) || ( xCopyPosition == queueOVERWRITE ) )
        {
            ध्रुव int8_t cTxLock = pxQueue->cTxLock;
            ध्रुव UBaseType_t uxPreviousMessagesWaiting = pxQueue->uxMessagesWaiting;

            traceQUEUE_SEND_FROM_ISR( pxQueue );

            /* Semaphores use xQueueGiveFromISR(), so pxQueue will not be a
             *  semaphore or mutex.  That means prvCopyDataToQueue() cannot result
             *  in a task disinheriting a priority and prvCopyDataToQueue() can be
             *  called here even though the disinherit function does not check if
             *  the scheduler is suspended before accessing the ready lists. */
            ( शून्य ) prvCopyDataToQueue( pxQueue, pvItemToQueue, xCopyPosition );

            /* The event list is not altered if the queue is locked.  This will
             * be done when the queue is unlocked later. */
            यदि( cTxLock == queueUNLOCKED )
            {
                #यदि ( configUSE_QUEUE_SETS == 1 )
                {
                    यदि( pxQueue->pxQueueSetContainer != रिक्त )
                    {
                        यदि( ( xCopyPosition == queueOVERWRITE ) && ( uxPreviousMessagesWaiting != ( UBaseType_t ) 0 ) )
                        {
                            /* Do not notify the queue set as an existing item
                             * was overwritten in the queue so the number of items
                             * in the queue has not changed. */
                            mtCOVERAGE_TEST_MARKER();
                        }
                        अन्यथा यदि( prvNotifyQueueSetContainer( pxQueue ) != pdFALSE )
                        {
                            /* The queue is a member of a queue set, and posting
                             * to the queue set caused a higher priority task to
                             * unblock.  A context switch is required. */
                            यदि( pxHigherPriorityTaskWoken != रिक्त )
                            {
                                *pxHigherPriorityTaskWoken = pdTRUE;
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
                    }
                    अन्यथा
                    {
                        यदि( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToReceive ) ) == pdFALSE )
                        {
                            यदि( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToReceive ) ) != pdFALSE )
                            {
                                /* The task waiting has a higher priority so
                                 *  record that a context switch is required. */
                                यदि( pxHigherPriorityTaskWoken != रिक्त )
                                {
                                    *pxHigherPriorityTaskWoken = pdTRUE;
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
                        }
                        अन्यथा
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                }
                #अन्यथा /* configUSE_QUEUE_SETS */
                {
                    यदि( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToReceive ) ) == pdFALSE )
                    {
                        यदि( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToReceive ) ) != pdFALSE )
                        {
                            /* The task waiting has a higher priority so record that a
                             * context switch is required. */
                            यदि( pxHigherPriorityTaskWoken != रिक्त )
                            {
                                *pxHigherPriorityTaskWoken = pdTRUE;
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
                    }
                    अन्यथा
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }

                    /* Not used in this path. */
                    ( शून्य ) uxPreviousMessagesWaiting;
                }
                #endif /* configUSE_QUEUE_SETS */
            }
            अन्यथा
            {
                /* Increment the lock count so the task that unlocks the queue
                 * knows that data was posted while it was locked. */
                prvIncrementQueueTxLock( pxQueue, cTxLock );
            }

            xReturn = pdPASS;
        }
        अन्यथा
        {
            traceQUEUE_SEND_FROM_ISR_FAILED( pxQueue );
            xReturn = errQUEUE_FULL;
        }
    }
    taskEXIT_CRITICAL_FROM_ISR( uxSavedInterruptStatus );

    traceRETURN_xQueueGenericSendFromISR( xReturn );

    प्रत्यागम xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t xQueueGiveFromISR( QueueHandle_t xQueue,
                              BaseType_t * ध्रुव pxHigherPriorityTaskWoken )
{
    BaseType_t xReturn;
    UBaseType_t uxSavedInterruptStatus;
    Queue_t * ध्रुव pxQueue = xQueue;

    traceENTER_xQueueGiveFromISR( xQueue, pxHigherPriorityTaskWoken );

    /* Similar to xQueueGenericSendFromISR() but used with semaphores where the
     * item size is 0.  Don't directly wake a task that was blocked on a queue
     * read, instead return a flag to say whether a context switch is required or
     * not (i.e. has a task with a higher priority than us been woken by this
     * post). */

    configASSERT( pxQueue );

    /* xQueueGenericSendFromISR() should be used instead of xQueueGiveFromISR()
     * if the item size is not 0. */
    configASSERT( pxQueue->uxItemSize == 0 );

    /* Normally a mutex would not be given from an interrupt, especially if
     * there is a mutex holder, as priority inheritance makes no sense for an
     * interrupts, only tasks. */
    configASSERT( !( ( pxQueue->uxQueueType == queueQUEUE_IS_MUTEX ) && ( pxQueue->u.xSemaphore.xMutexHolder != रिक्त ) ) );

    /* RTOS ports that support interrupt nesting have the concept of a maximum
     * system call (or maximum API call) interrupt priority.  Interrupts that are
     * above the maximum system call priority are kept permanently enabled, even
     * when the RTOS kernel is in a critical section, but cannot make any calls to
     * FreeRTOS API functions.  If configASSERT() is defined in FreeRTOSConfig.h
     * then portASSERT_IF_INTERRUPT_PRIORITY_INVALID() will result in an assertion
     * failure if a FreeRTOS API function is called from an interrupt that has been
     * assigned a priority above the configured maximum system call priority.
     * Only FreeRTOS functions that end in FromISR can be called from interrupts
     * that have been assigned a priority at or (logically) below the maximum
     * system call interrupt priority.  FreeRTOS maintains a separate interrupt
     * safe API to ensure interrupt entry is as fast and as simple as possible.
     * More information (albeit Cortex-M specific) is provided on the following
     * link: https://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html */
    portASSERT_IF_INTERRUPT_PRIORITY_INVALID();

    /* MISRA Ref 4.7.1 [Return value shall be checked] */
    /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
    /* coverity[misra_c_2012_directive_4_7_violation] */
    uxSavedInterruptStatus = ( UBaseType_t ) taskENTER_CRITICAL_FROM_ISR();
    {
        ध्रुव UBaseType_t uxMessagesWaiting = pxQueue->uxMessagesWaiting;

        /* When the queue is used to implement a semaphore no data is ever
         * moved through the queue but it is still valid to see if the queue 'has
         * space'. */
        यदि( uxMessagesWaiting < pxQueue->uxLength )
        {
            ध्रुव int8_t cTxLock = pxQueue->cTxLock;

            traceQUEUE_SEND_FROM_ISR( pxQueue );

            /* A task can only have an inherited priority if it is a mutex
             * holder - and if there is a mutex holder then the mutex cannot be
             * given from an ISR.  As this is the ISR version of the function it
             * can be assumed there is no mutex holder and no need to determine if
             * priority disinheritance is needed.  Simply increase the count of
             * messages (semaphores) available. */
            pxQueue->uxMessagesWaiting = ( UBaseType_t ) ( uxMessagesWaiting + ( UBaseType_t ) 1 );

            /* The event list is not altered if the queue is locked.  This will
             * be done when the queue is unlocked later. */
            यदि( cTxLock == queueUNLOCKED )
            {
                #यदि ( configUSE_QUEUE_SETS == 1 )
                {
                    यदि( pxQueue->pxQueueSetContainer != रिक्त )
                    {
                        यदि( prvNotifyQueueSetContainer( pxQueue ) != pdFALSE )
                        {
                            /* The semaphore is a member of a queue set, and
                             * posting to the queue set caused a higher priority
                             * task to unblock.  A context switch is required. */
                            यदि( pxHigherPriorityTaskWoken != रिक्त )
                            {
                                *pxHigherPriorityTaskWoken = pdTRUE;
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
                    }
                    अन्यथा
                    {
                        यदि( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToReceive ) ) == pdFALSE )
                        {
                            यदि( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToReceive ) ) != pdFALSE )
                            {
                                /* The task waiting has a higher priority so
                                 *  record that a context switch is required. */
                                यदि( pxHigherPriorityTaskWoken != रिक्त )
                                {
                                    *pxHigherPriorityTaskWoken = pdTRUE;
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
                        }
                        अन्यथा
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                }
                #अन्यथा /* configUSE_QUEUE_SETS */
                {
                    यदि( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToReceive ) ) == pdFALSE )
                    {
                        यदि( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToReceive ) ) != pdFALSE )
                        {
                            /* The task waiting has a higher priority so record that a
                             * context switch is required. */
                            यदि( pxHigherPriorityTaskWoken != रिक्त )
                            {
                                *pxHigherPriorityTaskWoken = pdTRUE;
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
                    }
                    अन्यथा
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #endif /* configUSE_QUEUE_SETS */
            }
            अन्यथा
            {
                /* Increment the lock count so the task that unlocks the queue
                 * knows that data was posted while it was locked. */
                prvIncrementQueueTxLock( pxQueue, cTxLock );
            }

            xReturn = pdPASS;
        }
        अन्यथा
        {
            traceQUEUE_SEND_FROM_ISR_FAILED( pxQueue );
            xReturn = errQUEUE_FULL;
        }
    }
    taskEXIT_CRITICAL_FROM_ISR( uxSavedInterruptStatus );

    traceRETURN_xQueueGiveFromISR( xReturn );

    प्रत्यागम xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t xQueueReceive( QueueHandle_t xQueue,
                          शून्य * ध्रुव pvBuffer,
                          TickType_t xTicksToWait )
{
    BaseType_t xEntryTimeSet = pdFALSE;
    TimeOut_t xTimeOut;
    Queue_t * ध्रुव pxQueue = xQueue;

    traceENTER_xQueueReceive( xQueue, pvBuffer, xTicksToWait );

    /* Check the pointer is not NULL. */
    configASSERT( ( pxQueue ) );

    /* The buffer into which data is received can only be NULL if the data size
     * is zero (so no data is copied into the buffer). */
    configASSERT( !( ( ( pvBuffer ) == रिक्त ) && ( ( pxQueue )->uxItemSize != ( UBaseType_t ) 0U ) ) );

    /* Cannot block if the scheduler is suspended. */
    #यदि ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( xTicksToWait != 0 ) ) );
    }
    #endif

    प्रति( ; ; )
    {
        taskENTER_CRITICAL();
        {
            ध्रुव UBaseType_t uxMessagesWaiting = pxQueue->uxMessagesWaiting;

            /* Is there data in the queue now?  To be running the calling task
             * must be the highest priority task wanting to access the queue. */
            यदि( uxMessagesWaiting > ( UBaseType_t ) 0 )
            {
                /* Data available, remove one item. */
                prvCopyDataFromQueue( pxQueue, pvBuffer );
                traceQUEUE_RECEIVE( pxQueue );
                pxQueue->uxMessagesWaiting = ( UBaseType_t ) ( uxMessagesWaiting - ( UBaseType_t ) 1 );

                /* There is now space in the queue, were any tasks waiting to
                 * post to the queue?  If so, unblock the highest priority waiting
                 * task. */
                यदि( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToSend ) ) == pdFALSE )
                {
                    यदि( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToSend ) ) != pdFALSE )
                    {
                        queueYIELD_IF_USING_PREEMPTION();
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

                taskEXIT_CRITICAL();

                traceRETURN_xQueueReceive( pdPASS );

                प्रत्यागम pdPASS;
            }
            अन्यथा
            {
                यदि( xTicksToWait == ( TickType_t ) 0 )
                {
                    /* The queue was empty and no block time is specified (or
                     * the block time has expired) so leave now. */
                    taskEXIT_CRITICAL();

                    traceQUEUE_RECEIVE_FAILED( pxQueue );
                    traceRETURN_xQueueReceive( errQUEUE_EMPTY );

                    प्रत्यागम errQUEUE_EMPTY;
                }
                अन्यथा यदि( xEntryTimeSet == pdFALSE )
                {
                    /* The queue was empty and a block time was specified so
                     * configure the timeout structure. */
                    vTaskInternalSetTimeOutState( &xTimeOut );
                    xEntryTimeSet = pdTRUE;
                }
                अन्यथा
                {
                    /* Entry time was already set. */
                    mtCOVERAGE_TEST_MARKER();
                }
            }
        }
        taskEXIT_CRITICAL();

        /* Interrupts and other tasks can send to and receive from the queue
         * now the critical section has been exited. */

        vTaskSuspendAll();
        prvLockQueue( pxQueue );

        /* Update the timeout state to see if it has expired yet. */
        यदि( xTaskCheckForTimeOut( &xTimeOut, &xTicksToWait ) == pdFALSE )
        {
            /* The timeout has not expired.  If the queue is still empty place
             * the task on the list of tasks waiting to receive from the queue. */
            यदि( prvIsQueueEmpty( pxQueue ) != pdFALSE )
            {
                traceBLOCKING_ON_QUEUE_RECEIVE( pxQueue );
                vTaskPlaceOnEventList( &( pxQueue->xTasksWaitingToReceive ), xTicksToWait );
                prvUnlockQueue( pxQueue );

                यदि( xTaskResumeAll() == pdFALSE )
                {
                    taskYIELD_WITHIN_API();
                }
                अन्यथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            अन्यथा
            {
                /* The queue contains data again.  Loop back to try and read the
                 * data. */
                prvUnlockQueue( pxQueue );
                ( शून्य ) xTaskResumeAll();
            }
        }
        अन्यथा
        {
            /* Timed out.  If there is no data in the queue exit, otherwise loop
             * back and attempt to read the data. */
            prvUnlockQueue( pxQueue );
            ( शून्य ) xTaskResumeAll();

            यदि( prvIsQueueEmpty( pxQueue ) != pdFALSE )
            {
                traceQUEUE_RECEIVE_FAILED( pxQueue );
                traceRETURN_xQueueReceive( errQUEUE_EMPTY );

                प्रत्यागम errQUEUE_EMPTY;
            }
            अन्यथा
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
    }
}
/*-----------------------------------------------------------*/

BaseType_t xQueueSemaphoreTake( QueueHandle_t xQueue,
                                TickType_t xTicksToWait )
{
    BaseType_t xEntryTimeSet = pdFALSE;
    TimeOut_t xTimeOut;
    Queue_t * ध्रुव pxQueue = xQueue;

    #यदि ( configUSE_MUTEXES == 1 )
        BaseType_t xInheritanceOccurred = pdFALSE;
    #endif

    traceENTER_xQueueSemaphoreTake( xQueue, xTicksToWait );

    /* Check the queue pointer is not NULL. */
    configASSERT( ( pxQueue ) );

    /* Check this really is a semaphore, in which case the item size will be
     * 0. */
    configASSERT( pxQueue->uxItemSize == 0 );

    /* Cannot block if the scheduler is suspended. */
    #यदि ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( xTicksToWait != 0 ) ) );
    }
    #endif

    प्रति( ; ; )
    {
        taskENTER_CRITICAL();
        {
            /* Semaphores are queues with an item size of 0, and where the
             * number of messages in the queue is the semaphore's count value. */
            ध्रुव UBaseType_t ux_Semaphore_मोजणी = pxQueue->uxMessagesWaiting;

            /* Is there data in the queue now?  To be running the calling task
             * must be the highest priority task wanting to access the queue. */
            यदि( ux_Semaphore_मोजणी > ( UBaseType_t ) 0 )
            {
                traceQUEUE_RECEIVE( pxQueue );

                /* Semaphores are queues with a data size of zero and where the
                 * messages waiting is the semaphore's count.  Reduce the count. */
                pxQueue->uxMessagesWaiting = ( UBaseType_t ) ( ux_Semaphore_मोजणी - ( UBaseType_t ) 1 );

                #यदि ( configUSE_MUTEXES == 1 )
                {
                    यदि( pxQueue->uxQueueType == queueQUEUE_IS_MUTEX )
                    {
                        /* Record the information required to implement
                         * priority inheritance should it become necessary. */
                        pxQueue->u.xSemaphore.xMutexHolder = pv_Task_Increment_Mutex_Held_मोजणी();
                    }
                    अन्यथा
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #endif /* configUSE_MUTEXES */

                /* Check to see if other tasks are blocked waiting to give the
                 * semaphore, and if so, unblock the highest priority such task. */
                यदि( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToSend ) ) == pdFALSE )
                {
                    यदि( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToSend ) ) != pdFALSE )
                    {
                        queueYIELD_IF_USING_PREEMPTION();
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

                taskEXIT_CRITICAL();

                traceRETURN_xQueueSemaphoreTake( pdPASS );

                प्रत्यागम pdPASS;
            }
            अन्यथा
            {
                यदि( xTicksToWait == ( TickType_t ) 0 )
                {
                    /* The semaphore count was 0 and no block time is specified
                     * (or the block time has expired) so exit now. */
                    taskEXIT_CRITICAL();

                    traceQUEUE_RECEIVE_FAILED( pxQueue );
                    traceRETURN_xQueueSemaphoreTake( errQUEUE_EMPTY );

                    प्रत्यागम errQUEUE_EMPTY;
                }
                अन्यथा यदि( xEntryTimeSet == pdFALSE )
                {
                    /* The semaphore count was 0 and a block time was specified
                     * so configure the timeout structure ready to block. */
                    vTaskInternalSetTimeOutState( &xTimeOut );
                    xEntryTimeSet = pdTRUE;
                }
                अन्यथा
                {
                    /* Entry time was already set. */
                    mtCOVERAGE_TEST_MARKER();
                }
            }
        }
        taskEXIT_CRITICAL();

        /* Interrupts and other tasks can give to and take from the semaphore
         * now the critical section has been exited. */

        vTaskSuspendAll();
        prvLockQueue( pxQueue );

        /* Update the timeout state to see if it has expired yet. */
        यदि( xTaskCheckForTimeOut( &xTimeOut, &xTicksToWait ) == pdFALSE )
        {
            /* A block time is specified and not expired.  If the semaphore
             * count is 0 then enter the Blocked state to wait for a semaphore to
             * become available.  As semaphores are implemented with queues the
             * queue being empty is equivalent to the semaphore count being 0. */
            यदि( prvIsQueueEmpty( pxQueue ) != pdFALSE )
            {
                traceBLOCKING_ON_QUEUE_RECEIVE( pxQueue );

                #यदि ( configUSE_MUTEXES == 1 )
                {
                    यदि( pxQueue->uxQueueType == queueQUEUE_IS_MUTEX )
                    {
                        taskENTER_CRITICAL();
                        {
                            xInheritanceOccurred = xTaskPriorityInherit( pxQueue->u.xSemaphore.xMutexHolder );
                        }
                        taskEXIT_CRITICAL();
                    }
                    अन्यथा
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #endif /* if ( configUSE_MUTEXES == 1 ) */

                vTaskPlaceOnEventList( &( pxQueue->xTasksWaitingToReceive ), xTicksToWait );
                prvUnlockQueue( pxQueue );

                यदि( xTaskResumeAll() == pdFALSE )
                {
                    taskYIELD_WITHIN_API();
                }
                अन्यथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            अन्यथा
            {
                /* There was no timeout and the semaphore count was not 0, so
                 * attempt to take the semaphore again. */
                prvUnlockQueue( pxQueue );
                ( शून्य ) xTaskResumeAll();
            }
        }
        अन्यथा
        {
            /* Timed out. */
            prvUnlockQueue( pxQueue );
            ( शून्य ) xTaskResumeAll();

            /* If the semaphore count is 0 exit now as the timeout has
             * expired.  Otherwise return to attempt to take the semaphore that is
             * known to be available.  As semaphores are implemented by queues the
             * queue being empty is equivalent to the semaphore count being 0. */
            यदि( prvIsQueueEmpty( pxQueue ) != pdFALSE )
            {
                #यदि ( configUSE_MUTEXES == 1 )
                {
                    /* xInheritanceOccurred could only have be set if
                     * pxQueue->uxQueueType == queueQUEUE_IS_MUTEX so no need to
                     * test the mutex type again to check it is actually a mutex. */
                    यदि( xInheritanceOccurred != pdFALSE )
                    {
                        taskENTER_CRITICAL();
                        {
                            UBaseType_t uxHighestWaitingPriority;

                            /* This task blocking on the mutex caused another
                             * task to inherit this task's priority.  Now this task
                             * has timed out the priority should be disinherited
                             * again, but only as low as the next highest priority
                             * task that is waiting for the same mutex. */
                            uxHighestWaitingPriority = prvGetDisinheritPriorityAfterTimeout( pxQueue );

                            /* vTaskPriorityDisinheritAfterTimeout uses the uxHighestWaitingPriority
                             * parameter to index pxReadyTasksLists when adding the task holding
                             * mutex to the ready list for its new priority. Coverity thinks that
                             * it can result in out-of-bounds access which is not true because
                             * uxHighestWaitingPriority, as returned by prvGetDisinheritPriorityAfterTimeout,
                             * is capped at ( configMAX_PRIORITIES - 1 ). */
                            /* coverity[overrun] */
                            vTaskPriorityDisinheritAfterTimeout( pxQueue->u.xSemaphore.xMutexHolder, uxHighestWaitingPriority );
                        }
                        taskEXIT_CRITICAL();
                    }
                }
                #endif /* configUSE_MUTEXES */

                traceQUEUE_RECEIVE_FAILED( pxQueue );
                traceRETURN_xQueueSemaphoreTake( errQUEUE_EMPTY );

                प्रत्यागम errQUEUE_EMPTY;
            }
            अन्यथा
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
    }
}
/*-----------------------------------------------------------*/

BaseType_t xQueuePeek( QueueHandle_t xQueue,
                       शून्य * ध्रुव pvBuffer,
                       TickType_t xTicksToWait )
{
    BaseType_t xEntryTimeSet = pdFALSE;
    TimeOut_t xTimeOut;
    int8_t * pcOriginalReadPosition;
    Queue_t * ध्रुव pxQueue = xQueue;

    traceENTER_xQueuePeek( xQueue, pvBuffer, xTicksToWait );

    /* Check the pointer is not NULL. */
    configASSERT( ( pxQueue ) );

    /* The buffer into which data is received can only be NULL if the data size
     * is zero (so no data is copied into the buffer. */
    configASSERT( !( ( ( pvBuffer ) == रिक्त ) && ( ( pxQueue )->uxItemSize != ( UBaseType_t ) 0U ) ) );

    /* Cannot block if the scheduler is suspended. */
    #यदि ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( xTicksToWait != 0 ) ) );
    }
    #endif

    प्रति( ; ; )
    {
        taskENTER_CRITICAL();
        {
            ध्रुव UBaseType_t uxMessagesWaiting = pxQueue->uxMessagesWaiting;

            /* Is there data in the queue now?  To be running the calling task
             * must be the highest priority task wanting to access the queue. */
            यदि( uxMessagesWaiting > ( UBaseType_t ) 0 )
            {
                /* Remember the read position so it can be reset after the data
                 * is read from the queue as this function is only peeking the
                 * data, not removing it. */
                pcOriginalReadPosition = pxQueue->u.xQueue.pcReadFrom;

                prvCopyDataFromQueue( pxQueue, pvBuffer );
                traceQUEUE_PEEK( pxQueue );

                /* The data is not being removed, so reset the read pointer. */
                pxQueue->u.xQueue.pcReadFrom = pcOriginalReadPosition;

                /* The data is being left in the queue, so see if there are
                 * any other tasks waiting for the data. */
                यदि( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToReceive ) ) == pdFALSE )
                {
                    यदि( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToReceive ) ) != pdFALSE )
                    {
                        /* The task waiting has a higher priority than this task. */
                        queueYIELD_IF_USING_PREEMPTION();
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

                taskEXIT_CRITICAL();

                traceRETURN_xQueuePeek( pdPASS );

                प्रत्यागम pdPASS;
            }
            अन्यथा
            {
                यदि( xTicksToWait == ( TickType_t ) 0 )
                {
                    /* The queue was empty and no block time is specified (or
                     * the block time has expired) so leave now. */
                    taskEXIT_CRITICAL();

                    traceQUEUE_PEEK_FAILED( pxQueue );
                    traceRETURN_xQueuePeek( errQUEUE_EMPTY );

                    प्रत्यागम errQUEUE_EMPTY;
                }
                अन्यथा यदि( xEntryTimeSet == pdFALSE )
                {
                    /* The queue was empty and a block time was specified so
                     * configure the timeout structure ready to enter the blocked
                     * state. */
                    vTaskInternalSetTimeOutState( &xTimeOut );
                    xEntryTimeSet = pdTRUE;
                }
                अन्यथा
                {
                    /* Entry time was already set. */
                    mtCOVERAGE_TEST_MARKER();
                }
            }
        }
        taskEXIT_CRITICAL();

        /* Interrupts and other tasks can send to and receive from the queue
         * now that the critical section has been exited. */

        vTaskSuspendAll();
        prvLockQueue( pxQueue );

        /* Update the timeout state to see if it has expired yet. */
        यदि( xTaskCheckForTimeOut( &xTimeOut, &xTicksToWait ) == pdFALSE )
        {
            /* Timeout has not expired yet, check to see if there is data in the
            * queue now, and if not enter the Blocked state to wait for data. */
            यदि( prvIsQueueEmpty( pxQueue ) != pdFALSE )
            {
                traceBLOCKING_ON_QUEUE_PEEK( pxQueue );
                vTaskPlaceOnEventList( &( pxQueue->xTasksWaitingToReceive ), xTicksToWait );
                prvUnlockQueue( pxQueue );

                यदि( xTaskResumeAll() == pdFALSE )
                {
                    taskYIELD_WITHIN_API();
                }
                अन्यथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            अन्यथा
            {
                /* There is data in the queue now, so don't enter the blocked
                 * state, instead return to try and obtain the data. */
                prvUnlockQueue( pxQueue );
                ( शून्य ) xTaskResumeAll();
            }
        }
        अन्यथा
        {
            /* The timeout has expired.  If there is still no data in the queue
             * exit, otherwise go back and try to read the data again. */
            prvUnlockQueue( pxQueue );
            ( शून्य ) xTaskResumeAll();

            यदि( prvIsQueueEmpty( pxQueue ) != pdFALSE )
            {
                traceQUEUE_PEEK_FAILED( pxQueue );
                traceRETURN_xQueuePeek( errQUEUE_EMPTY );

                प्रत्यागम errQUEUE_EMPTY;
            }
            अन्यथा
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
    }
}
/*-----------------------------------------------------------*/

BaseType_t xQueueReceiveFromISR( QueueHandle_t xQueue,
                                 शून्य * ध्रुव pvBuffer,
                                 BaseType_t * ध्रुव pxHigherPriorityTaskWoken )
{
    BaseType_t xReturn;
    UBaseType_t uxSavedInterruptStatus;
    Queue_t * ध्रुव pxQueue = xQueue;

    traceENTER_xQueueReceiveFromISR( xQueue, pvBuffer, pxHigherPriorityTaskWoken );

    configASSERT( pxQueue );
    configASSERT( !( ( pvBuffer == रिक्त ) && ( pxQueue->uxItemSize != ( UBaseType_t ) 0U ) ) );

    /* RTOS ports that support interrupt nesting have the concept of a maximum
     * system call (or maximum API call) interrupt priority.  Interrupts that are
     * above the maximum system call priority are kept permanently enabled, even
     * when the RTOS kernel is in a critical section, but cannot make any calls to
     * FreeRTOS API functions.  If configASSERT() is defined in FreeRTOSConfig.h
     * then portASSERT_IF_INTERRUPT_PRIORITY_INVALID() will result in an assertion
     * failure if a FreeRTOS API function is called from an interrupt that has been
     * assigned a priority above the configured maximum system call priority.
     * Only FreeRTOS functions that end in FromISR can be called from interrupts
     * that have been assigned a priority at or (logically) below the maximum
     * system call interrupt priority.  FreeRTOS maintains a separate interrupt
     * safe API to ensure interrupt entry is as fast and as simple as possible.
     * More information (albeit Cortex-M specific) is provided on the following
     * link: https://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html */
    portASSERT_IF_INTERRUPT_PRIORITY_INVALID();

    /* MISRA Ref 4.7.1 [Return value shall be checked] */
    /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
    /* coverity[misra_c_2012_directive_4_7_violation] */
    uxSavedInterruptStatus = ( UBaseType_t ) taskENTER_CRITICAL_FROM_ISR();
    {
        ध्रुव UBaseType_t uxMessagesWaiting = pxQueue->uxMessagesWaiting;

        /* Cannot block in an ISR, so check there is data available. */
        यदि( uxMessagesWaiting > ( UBaseType_t ) 0 )
        {
            ध्रुव int8_t cRxLock = pxQueue->cRxLock;

            traceQUEUE_RECEIVE_FROM_ISR( pxQueue );

            prvCopyDataFromQueue( pxQueue, pvBuffer );
            pxQueue->uxMessagesWaiting = ( UBaseType_t ) ( uxMessagesWaiting - ( UBaseType_t ) 1 );

            /* If the queue is locked the event list will not be modified.
             * Instead update the lock count so the task that unlocks the queue
             * will know that an ISR has removed data while the queue was
             * locked. */
            यदि( cRxLock == queueUNLOCKED )
            {
                यदि( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToSend ) ) == pdFALSE )
                {
                    यदि( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToSend ) ) != pdFALSE )
                    {
                        /* The task waiting has a higher priority than us so
                         * force a context switch. */
                        यदि( pxHigherPriorityTaskWoken != रिक्त )
                        {
                            *pxHigherPriorityTaskWoken = pdTRUE;
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
                }
                अन्यथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            अन्यथा
            {
                /* Increment the lock count so the task that unlocks the queue
                 * knows that data was removed while it was locked. */
                prvIncrementQueueRxLock( pxQueue, cRxLock );
            }

            xReturn = pdPASS;
        }
        अन्यथा
        {
            xReturn = pdFAIL;
            traceQUEUE_RECEIVE_FROM_ISR_FAILED( pxQueue );
        }
    }
    taskEXIT_CRITICAL_FROM_ISR( uxSavedInterruptStatus );

    traceRETURN_xQueueReceiveFromISR( xReturn );

    प्रत्यागम xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t xQueuePeekFromISR( QueueHandle_t xQueue,
                              शून्य * ध्रुव pvBuffer )
{
    BaseType_t xReturn;
    UBaseType_t uxSavedInterruptStatus;
    int8_t * pcOriginalReadPosition;
    Queue_t * ध्रुव pxQueue = xQueue;

    traceENTER_xQueuePeekFromISR( xQueue, pvBuffer );

    configASSERT( pxQueue );
    configASSERT( !( ( pvBuffer == रिक्त ) && ( pxQueue->uxItemSize != ( UBaseType_t ) 0U ) ) );
    configASSERT( pxQueue->uxItemSize != 0 ); /* Can't peek a semaphore. */

    /* RTOS ports that support interrupt nesting have the concept of a maximum
     * system call (or maximum API call) interrupt priority.  Interrupts that are
     * above the maximum system call priority are kept permanently enabled, even
     * when the RTOS kernel is in a critical section, but cannot make any calls to
     * FreeRTOS API functions.  If configASSERT() is defined in FreeRTOSConfig.h
     * then portASSERT_IF_INTERRUPT_PRIORITY_INVALID() will result in an assertion
     * failure if a FreeRTOS API function is called from an interrupt that has been
     * assigned a priority above the configured maximum system call priority.
     * Only FreeRTOS functions that end in FromISR can be called from interrupts
     * that have been assigned a priority at or (logically) below the maximum
     * system call interrupt priority.  FreeRTOS maintains a separate interrupt
     * safe API to ensure interrupt entry is as fast and as simple as possible.
     * More information (albeit Cortex-M specific) is provided on the following
     * link: https://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html */
    portASSERT_IF_INTERRUPT_PRIORITY_INVALID();

    /* MISRA Ref 4.7.1 [Return value shall be checked] */
    /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
    /* coverity[misra_c_2012_directive_4_7_violation] */
    uxSavedInterruptStatus = ( UBaseType_t ) taskENTER_CRITICAL_FROM_ISR();
    {
        /* Cannot block in an ISR, so check there is data available. */
        यदि( pxQueue->uxMessagesWaiting > ( UBaseType_t ) 0 )
        {
            traceQUEUE_PEEK_FROM_ISR( pxQueue );

            /* Remember the read position so it can be reset as nothing is
             * actually being removed from the queue. */
            pcOriginalReadPosition = pxQueue->u.xQueue.pcReadFrom;
            prvCopyDataFromQueue( pxQueue, pvBuffer );
            pxQueue->u.xQueue.pcReadFrom = pcOriginalReadPosition;

            xReturn = pdPASS;
        }
        अन्यथा
        {
            xReturn = pdFAIL;
            traceQUEUE_PEEK_FROM_ISR_FAILED( pxQueue );
        }
    }
    taskEXIT_CRITICAL_FROM_ISR( uxSavedInterruptStatus );

    traceRETURN_xQueuePeekFromISR( xReturn );

    प्रत्यागम xReturn;
}
/*-----------------------------------------------------------*/

UBaseType_t uxQueueMessagesWaiting( ध्रुव QueueHandle_t xQueue )
{
    UBaseType_t uxReturn;

    traceENTER_uxQueueMessagesWaiting( xQueue );

    configASSERT( xQueue );

    taskENTER_CRITICAL();
    {
        uxReturn = ( ( Queue_t * ) xQueue )->uxMessagesWaiting;
    }
    taskEXIT_CRITICAL();

    traceRETURN_uxQueueMessagesWaiting( uxReturn );

    प्रत्यागम uxReturn;
}
/*-----------------------------------------------------------*/

UBaseType_t uxQueueSpacesAvailable( ध्रुव QueueHandle_t xQueue )
{
    UBaseType_t uxReturn;
    Queue_t * ध्रुव pxQueue = xQueue;

    traceENTER_uxQueueSpacesAvailable( xQueue );

    configASSERT( pxQueue );

    taskENTER_CRITICAL();
    {
        uxReturn = ( UBaseType_t ) ( pxQueue->uxLength - pxQueue->uxMessagesWaiting );
    }
    taskEXIT_CRITICAL();

    traceRETURN_uxQueueSpacesAvailable( uxReturn );

    प्रत्यागम uxReturn;
}
/*-----------------------------------------------------------*/

UBaseType_t uxQueueMessagesWaitingFromISR( ध्रुव QueueHandle_t xQueue )
{
    UBaseType_t uxReturn;
    Queue_t * ध्रुव pxQueue = xQueue;

    traceENTER_uxQueueMessagesWaitingFromISR( xQueue );

    configASSERT( pxQueue );
    uxReturn = pxQueue->uxMessagesWaiting;

    traceRETURN_uxQueueMessagesWaitingFromISR( uxReturn );

    प्रत्यागम uxReturn;
}
/*-----------------------------------------------------------*/

शून्य vQueueDelete( QueueHandle_t xQueue )
{
    Queue_t * ध्रुव pxQueue = xQueue;

    traceENTER_vQueueDelete( xQueue );

    configASSERT( pxQueue );
    traceQUEUE_DELETE( pxQueue );

    #यदि ( configQUEUE_REGISTRY_SIZE > 0 )
    {
        vQueueUnregisterQueue( pxQueue );
    }
    #endif

    #यदि ( ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) && ( configSUPPORT_STATIC_ALLOCATION == 0 ) )
    {
        /* The queue can only have been allocated dynamically - free it
         * again. */
        vPortFree( pxQueue );
    }
    #नोचेद्यदि ( ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) && ( configSUPPORT_STATIC_ALLOCATION == 1 ) )
    {
        /* The queue could have been allocated statically or dynamically, so
         * check before attempting to free the memory. */
        यदि( pxQueue->ucStaticallyAllocated == ( uint8_t ) pdFALSE )
        {
            vPortFree( pxQueue );
        }
        अन्यथा
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    #अन्यथा /* if ( ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) && ( configSUPPORT_STATIC_ALLOCATION == 0 ) ) */
    {
        /* The queue must have been statically allocated, so is not going to be
         * deleted.  Avoid compiler warnings about the unused parameter. */
        ( शून्य ) pxQueue;
    }
    #endif /* configSUPPORT_DYNAMIC_ALLOCATION */

    traceRETURN_vQueueDelete();
}
/*-----------------------------------------------------------*/

#यदि ( configUSE_TRACE_FACILITY == 1 )

    UBaseType_t ux_Queue_Get_Queue_संख्या( QueueHandle_t xQueue )
    {
        trace_ENTER_ux_Queue_Get_Queue_संख्या( xQueue );

        trace_RETURN_ux_Queue_Get_Queue_संख्या( ( ( Queue_t * ) xQueue )->ux_Queue_संख्या );

        प्रत्यागम ( ( Queue_t * ) xQueue )->ux_Queue_संख्या;
    }

#endif /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#यदि ( configUSE_TRACE_FACILITY == 1 )

    शून्य v_Queue_Set_Queue_संख्या( QueueHandle_t xQueue,
                               UBaseType_t ux_Queue_संख्या )
    {
        trace_ENTER_v_Queue_Set_Queue_संख्या( xQueue, ux_Queue_संख्या );

        ( ( Queue_t * ) xQueue )->ux_Queue_संख्या = ux_Queue_संख्या;

        trace_RETURN_v_Queue_Set_Queue_संख्या();
    }

#endif /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#यदि ( configUSE_TRACE_FACILITY == 1 )

    uint8_t ucQueueGetQueueType( QueueHandle_t xQueue )
    {
        traceENTER_ucQueueGetQueueType( xQueue );

        traceRETURN_ucQueueGetQueueType( ( ( Queue_t * ) xQueue )->ucQueueType );

        प्रत्यागम ( ( Queue_t * ) xQueue )->ucQueueType;
    }

#endif /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

UBaseType_t uxQueueGetQueueItemSize( QueueHandle_t xQueue ) /* PRIVILEGED_FUNCTION */
{
    traceENTER_uxQueueGetQueueItemSize( xQueue );

    traceRETURN_uxQueueGetQueueItemSize( ( ( Queue_t * ) xQueue )->uxItemSize );

    प्रत्यागम ( ( Queue_t * ) xQueue )->uxItemSize;
}
/*-----------------------------------------------------------*/

UBaseType_t uxQueueGetQueueLength( QueueHandle_t xQueue ) /* PRIVILEGED_FUNCTION */
{
    traceENTER_uxQueueGetQueueLength( xQueue );

    traceRETURN_uxQueueGetQueueLength( ( ( Queue_t * ) xQueue )->uxLength );

    प्रत्यागम ( ( Queue_t * ) xQueue )->uxLength;
}
/*-----------------------------------------------------------*/

#यदि ( configUSE_MUTEXES == 1 )

    स्थिर UBaseType_t prvGetDisinheritPriorityAfterTimeout( ध्रुव Queue_t * ध्रुव pxQueue )
    {
        UBaseType_t uxHighestPriorityOfWaitingTasks;

        /* If a task waiting for a mutex causes the mutex holder to inherit a
         * priority, but the waiting task times out, then the holder should
         * disinherit the priority - but only down to the highest priority of any
         * other tasks that are waiting for the same mutex.  For this purpose,
         * return the priority of the highest priority task that is waiting for the
         * mutex. */
        यदि( listCURRENT_LIST_LENGTH( &( pxQueue->xTasksWaitingToReceive ) ) > 0U )
        {
            uxHighestPriorityOfWaitingTasks = ( UBaseType_t ) ( ( UBaseType_t ) configMAX_PRIORITIES - ( UBaseType_t ) list_GET_ITEM_मूल्य_OF_HEAD_ENTRY( &( pxQueue->xTasksWaitingToReceive ) ) );
        }
        अन्यथा
        {
            uxHighestPriorityOfWaitingTasks = tskIDLE_PRIORITY;
        }

        प्रत्यागम uxHighestPriorityOfWaitingTasks;
    }

#endif /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

स्थिर BaseType_t prvCopyDataToQueue( Queue_t * ध्रुव pxQueue,
                                      ध्रुव शून्य * pvItemToQueue,
                                      ध्रुव BaseType_t xPosition )
{
    BaseType_t xReturn = pdFALSE;
    UBaseType_t uxMessagesWaiting;

    /* This function is called from a critical section. */

    uxMessagesWaiting = pxQueue->uxMessagesWaiting;

    यदि( pxQueue->uxItemSize == ( UBaseType_t ) 0 )
    {
        #यदि ( configUSE_MUTEXES == 1 )
        {
            यदि( pxQueue->uxQueueType == queueQUEUE_IS_MUTEX )
            {
                /* The mutex is no longer being held. */
                xReturn = xTaskPriorityDisinherit( pxQueue->u.xSemaphore.xMutexHolder );
                pxQueue->u.xSemaphore.xMutexHolder = रिक्त;
            }
            अन्यथा
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        #endif /* configUSE_MUTEXES */
    }
    अन्यथा यदि( xPosition == queueSEND_TO_BACK )
    {
        ( शून्य ) memcpy( ( शून्य * ) pxQueue->pcWriteTo, pvItemToQueue, ( size_t ) pxQueue->uxItemSize );
        pxQueue->pcWriteTo += pxQueue->uxItemSize;

        यदि( pxQueue->pcWriteTo >= pxQueue->u.xQueue.pcTail )
        {
            pxQueue->pcWriteTo = pxQueue->pcHead;
        }
        अन्यथा
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    अन्यथा
    {
        ( शून्य ) memcpy( ( शून्य * ) pxQueue->u.xQueue.pcReadFrom, pvItemToQueue, ( size_t ) pxQueue->uxItemSize );
        pxQueue->u.xQueue.pcReadFrom -= pxQueue->uxItemSize;

        यदि( pxQueue->u.xQueue.pcReadFrom < pxQueue->pcHead )
        {
            pxQueue->u.xQueue.pcReadFrom = ( pxQueue->u.xQueue.pcTail - pxQueue->uxItemSize );
        }
        अन्यथा
        {
            mtCOVERAGE_TEST_MARKER();
        }

        यदि( xPosition == queueOVERWRITE )
        {
            यदि( uxMessagesWaiting > ( UBaseType_t ) 0 )
            {
                /* An item is not being added but overwritten, so subtract
                 * one from the recorded number of items in the queue so when
                 * one is added again below the number of recorded items remains
                 * correct. */
                --uxMessagesWaiting;
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
    }

    pxQueue->uxMessagesWaiting = ( UBaseType_t ) ( uxMessagesWaiting + ( UBaseType_t ) 1 );

    प्रत्यागम xReturn;
}
/*-----------------------------------------------------------*/

स्थिर शून्य prvCopyDataFromQueue( Queue_t * ध्रुव pxQueue,
                                  शून्य * ध्रुव pvBuffer )
{
    यदि( pxQueue->uxItemSize != ( UBaseType_t ) 0 )
    {
        pxQueue->u.xQueue.pcReadFrom += pxQueue->uxItemSize;

        यदि( pxQueue->u.xQueue.pcReadFrom >= pxQueue->u.xQueue.pcTail )
        {
            pxQueue->u.xQueue.pcReadFrom = pxQueue->pcHead;
        }
        अन्यथा
        {
            mtCOVERAGE_TEST_MARKER();
        }

        ( शून्य ) memcpy( ( शून्य * ) pvBuffer, ( शून्य * ) pxQueue->u.xQueue.pcReadFrom, ( size_t ) pxQueue->uxItemSize );
    }
}
/*-----------------------------------------------------------*/

स्थिर शून्य prvUnlockQueue( Queue_t * ध्रुव pxQueue )
{
    /* THIS FUNCTION MUST BE CALLED WITH THE SCHEDULER SUSPENDED. */

    /* The lock counts contains the number of extra data items placed or
     * removed from the queue while the queue was locked.  When a queue is
     * locked items can be added or removed, but the event lists cannot be
     * updated. */
    taskENTER_CRITICAL();
    {
        int8_t cTxLock = pxQueue->cTxLock;

        /* See if data was added to the queue while it was locked. */
        यावत्( cTxLock > queueLOCKED_UNMODIFIED )
        {
            /* Data was posted while the queue was locked.  Are any tasks
             * blocked waiting for data to become available? */
            #यदि ( configUSE_QUEUE_SETS == 1 )
            {
                यदि( pxQueue->pxQueueSetContainer != रिक्त )
                {
                    यदि( prvNotifyQueueSetContainer( pxQueue ) != pdFALSE )
                    {
                        /* The queue is a member of a queue set, and posting to
                         * the queue set caused a higher priority task to unblock.
                         * A context switch is required. */
                        vTaskMissedYield();
                    }
                    अन्यथा
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                अन्यथा
                {
                    /* Tasks that are removed from the event list will get
                     * added to the pending ready list as the scheduler is still
                     * suspended. */
                    यदि( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToReceive ) ) == pdFALSE )
                    {
                        यदि( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToReceive ) ) != pdFALSE )
                        {
                            /* The task waiting has a higher priority so record that a
                             * context switch is required. */
                            vTaskMissedYield();
                        }
                        अन्यथा
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    अन्यथा
                    {
                        भङ्ग;
                    }
                }
            }
            #अन्यथा /* configUSE_QUEUE_SETS */
            {
                /* Tasks that are removed from the event list will get added to
                 * the pending ready list as the scheduler is still suspended. */
                यदि( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToReceive ) ) == pdFALSE )
                {
                    यदि( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToReceive ) ) != pdFALSE )
                    {
                        /* The task waiting has a higher priority so record that
                         * a context switch is required. */
                        vTaskMissedYield();
                    }
                    अन्यथा
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                अन्यथा
                {
                    भङ्ग;
                }
            }
            #endif /* configUSE_QUEUE_SETS */

            --cTxLock;
        }

        pxQueue->cTxLock = queueUNLOCKED;
    }
    taskEXIT_CRITICAL();

    /* Do the same for the Rx lock. */
    taskENTER_CRITICAL();
    {
        int8_t cRxLock = pxQueue->cRxLock;

        यावत्( cRxLock > queueLOCKED_UNMODIFIED )
        {
            यदि( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToSend ) ) == pdFALSE )
            {
                यदि( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToSend ) ) != pdFALSE )
                {
                    vTaskMissedYield();
                }
                अन्यथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                --cRxLock;
            }
            अन्यथा
            {
                भङ्ग;
            }
        }

        pxQueue->cRxLock = queueUNLOCKED;
    }
    taskEXIT_CRITICAL();
}
/*-----------------------------------------------------------*/

स्थिर BaseType_t prvIsQueueEmpty( ध्रुव Queue_t * pxQueue )
{
    BaseType_t xReturn;

    taskENTER_CRITICAL();
    {
        यदि( pxQueue->uxMessagesWaiting == ( UBaseType_t ) 0 )
        {
            xReturn = pdTRUE;
        }
        अन्यथा
        {
            xReturn = pdFALSE;
        }
    }
    taskEXIT_CRITICAL();

    प्रत्यागम xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t xQueueIsQueueEmptyFromISR( ध्रुव QueueHandle_t xQueue )
{
    BaseType_t xReturn;
    Queue_t * ध्रुव pxQueue = xQueue;

    traceENTER_xQueueIsQueueEmptyFromISR( xQueue );

    configASSERT( pxQueue );

    यदि( pxQueue->uxMessagesWaiting == ( UBaseType_t ) 0 )
    {
        xReturn = pdTRUE;
    }
    अन्यथा
    {
        xReturn = pdFALSE;
    }

    traceRETURN_xQueueIsQueueEmptyFromISR( xReturn );

    प्रत्यागम xReturn;
}
/*-----------------------------------------------------------*/

स्थिर BaseType_t prvIsQueueFull( ध्रुव Queue_t * pxQueue )
{
    BaseType_t xReturn;

    taskENTER_CRITICAL();
    {
        यदि( pxQueue->uxMessagesWaiting == pxQueue->uxLength )
        {
            xReturn = pdTRUE;
        }
        अन्यथा
        {
            xReturn = pdFALSE;
        }
    }
    taskEXIT_CRITICAL();

    प्रत्यागम xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t xQueueIsQueueFullFromISR( ध्रुव QueueHandle_t xQueue )
{
    BaseType_t xReturn;
    Queue_t * ध्रुव pxQueue = xQueue;

    traceENTER_xQueueIsQueueFullFromISR( xQueue );

    configASSERT( pxQueue );

    यदि( pxQueue->uxMessagesWaiting == pxQueue->uxLength )
    {
        xReturn = pdTRUE;
    }
    अन्यथा
    {
        xReturn = pdFALSE;
    }

    traceRETURN_xQueueIsQueueFullFromISR( xReturn );

    प्रत्यागम xReturn;
}
/*-----------------------------------------------------------*/

#यदि ( configUSE_CO_ROUTINES == 1 )

    BaseType_t xQueueCRSend( QueueHandle_t xQueue,
                             ध्रुव शून्य * pvItemToQueue,
                             TickType_t xTicksToWait )
    {
        BaseType_t xReturn;
        Queue_t * ध्रुव pxQueue = xQueue;

        traceENTER_xQueueCRSend( xQueue, pvItemToQueue, xTicksToWait );

        /* If the queue is already full we may have to block.  A critical section
         * is required to prevent an interrupt removing something from the queue
         * between the check to see if the queue is full and blocking on the queue. */
        portDISABLE_INTERRUPTS();
        {
            यदि( prvIsQueueFull( pxQueue ) != pdFALSE )
            {
                /* The queue is full - do we want to block or just leave without
                 * posting? */
                यदि( xTicksToWait > ( TickType_t ) 0 )
                {
                    /* As this is called from a coroutine we cannot block directly, but
                     * return indicating that we need to block. */
                    vCoRoutineAddToDelayedList( xTicksToWait, &( pxQueue->xTasksWaitingToSend ) );
                    portENABLE_INTERRUPTS();
                    प्रत्यागम errQUEUE_BLOCKED;
                }
                अन्यथा
                {
                    portENABLE_INTERRUPTS();
                    प्रत्यागम errQUEUE_FULL;
                }
            }
        }
        portENABLE_INTERRUPTS();

        portDISABLE_INTERRUPTS();
        {
            यदि( pxQueue->uxMessagesWaiting < pxQueue->uxLength )
            {
                /* There is room in the queue, copy the data into the queue. */
                prvCopyDataToQueue( pxQueue, pvItemToQueue, queueSEND_TO_BACK );
                xReturn = pdPASS;

                /* Were any co-routines waiting for data to become available? */
                यदि( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToReceive ) ) == pdFALSE )
                {
                    /* In this instance the co-routine could be placed directly
                     * into the ready list as we are within a critical section.
                     * Instead the same pending ready list mechanism is used as if
                     * the event were caused from within an interrupt. */
                    यदि( xCoRoutineRemoveFromEventList( &( pxQueue->xTasksWaitingToReceive ) ) != pdFALSE )
                    {
                        /* The co-routine waiting has a higher priority so record
                         * that a yield might be appropriate. */
                        xReturn = errQUEUE_YIELD;
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
            }
            अन्यथा
            {
                xReturn = errQUEUE_FULL;
            }
        }
        portENABLE_INTERRUPTS();

        traceRETURN_xQueueCRSend( xReturn );

        प्रत्यागम xReturn;
    }

#endif /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#यदि ( configUSE_CO_ROUTINES == 1 )

    BaseType_t xQueueCRReceive( QueueHandle_t xQueue,
                                शून्य * pvBuffer,
                                TickType_t xTicksToWait )
    {
        BaseType_t xReturn;
        Queue_t * ध्रुव pxQueue = xQueue;

        traceENTER_xQueueCRReceive( xQueue, pvBuffer, xTicksToWait );

        /* If the queue is already empty we may have to block.  A critical section
         * is required to prevent an interrupt adding something to the queue
         * between the check to see if the queue is empty and blocking on the queue. */
        portDISABLE_INTERRUPTS();
        {
            यदि( pxQueue->uxMessagesWaiting == ( UBaseType_t ) 0 )
            {
                /* There are no messages in the queue, do we want to block or just
                 * leave with nothing? */
                यदि( xTicksToWait > ( TickType_t ) 0 )
                {
                    /* As this is a co-routine we cannot block directly, but return
                     * indicating that we need to block. */
                    vCoRoutineAddToDelayedList( xTicksToWait, &( pxQueue->xTasksWaitingToReceive ) );
                    portENABLE_INTERRUPTS();
                    प्रत्यागम errQUEUE_BLOCKED;
                }
                अन्यथा
                {
                    portENABLE_INTERRUPTS();
                    प्रत्यागम errQUEUE_FULL;
                }
            }
            अन्यथा
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        portENABLE_INTERRUPTS();

        portDISABLE_INTERRUPTS();
        {
            यदि( pxQueue->uxMessagesWaiting > ( UBaseType_t ) 0 )
            {
                /* Data is available from the queue. */
                pxQueue->u.xQueue.pcReadFrom += pxQueue->uxItemSize;

                यदि( pxQueue->u.xQueue.pcReadFrom >= pxQueue->u.xQueue.pcTail )
                {
                    pxQueue->u.xQueue.pcReadFrom = pxQueue->pcHead;
                }
                अन्यथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                --( pxQueue->uxMessagesWaiting );
                ( शून्य ) memcpy( ( शून्य * ) pvBuffer, ( शून्य * ) pxQueue->u.xQueue.pcReadFrom, ( unsigned ) pxQueue->uxItemSize );

                xReturn = pdPASS;

                /* Were any co-routines waiting for space to become available? */
                यदि( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToSend ) ) == pdFALSE )
                {
                    /* In this instance the co-routine could be placed directly
                     * into the ready list as we are within a critical section.
                     * Instead the same pending ready list mechanism is used as if
                     * the event were caused from within an interrupt. */
                    यदि( xCoRoutineRemoveFromEventList( &( pxQueue->xTasksWaitingToSend ) ) != pdFALSE )
                    {
                        xReturn = errQUEUE_YIELD;
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
            }
            अन्यथा
            {
                xReturn = pdFAIL;
            }
        }
        portENABLE_INTERRUPTS();

        traceRETURN_xQueueCRReceive( xReturn );

        प्रत्यागम xReturn;
    }

#endif /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#यदि ( configUSE_CO_ROUTINES == 1 )

    BaseType_t xQueueCRSendFromISR( QueueHandle_t xQueue,
                                    ध्रुव शून्य * pvItemToQueue,
                                    BaseType_t xCoRoutinePreviouslyWoken )
    {
        Queue_t * ध्रुव pxQueue = xQueue;

        traceENTER_xQueueCRSendFromISR( xQueue, pvItemToQueue, xCoRoutinePreviouslyWoken );

        /* Cannot block within an ISR so if there is no space on the queue then
         * exit without doing anything. */
        यदि( pxQueue->uxMessagesWaiting < pxQueue->uxLength )
        {
            prvCopyDataToQueue( pxQueue, pvItemToQueue, queueSEND_TO_BACK );

            /* We only want to wake one co-routine per ISR, so check that a
             * co-routine has not already been woken. */
            यदि( xCoRoutinePreviouslyWoken == pdFALSE )
            {
                यदि( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToReceive ) ) == pdFALSE )
                {
                    यदि( xCoRoutineRemoveFromEventList( &( pxQueue->xTasksWaitingToReceive ) ) != pdFALSE )
                    {
                        प्रत्यागम pdTRUE;
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

        traceRETURN_xQueueCRSendFromISR( xCoRoutinePreviouslyWoken );

        प्रत्यागम xCoRoutinePreviouslyWoken;
    }

#endif /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#यदि ( configUSE_CO_ROUTINES == 1 )

    BaseType_t xQueueCRReceiveFromISR( QueueHandle_t xQueue,
                                       शून्य * pvBuffer,
                                       BaseType_t * pxCoRoutineWoken )
    {
        BaseType_t xReturn;
        Queue_t * ध्रुव pxQueue = xQueue;

        traceENTER_xQueueCRReceiveFromISR( xQueue, pvBuffer, pxCoRoutineWoken );

        /* We cannot block from an ISR, so check there is data available. If
         * not then just leave without doing anything. */
        यदि( pxQueue->uxMessagesWaiting > ( UBaseType_t ) 0 )
        {
            /* Copy the data from the queue. */
            pxQueue->u.xQueue.pcReadFrom += pxQueue->uxItemSize;

            यदि( pxQueue->u.xQueue.pcReadFrom >= pxQueue->u.xQueue.pcTail )
            {
                pxQueue->u.xQueue.pcReadFrom = pxQueue->pcHead;
            }
            अन्यथा
            {
                mtCOVERAGE_TEST_MARKER();
            }

            --( pxQueue->uxMessagesWaiting );
            ( शून्य ) memcpy( ( शून्य * ) pvBuffer, ( शून्य * ) pxQueue->u.xQueue.pcReadFrom, ( unsigned ) pxQueue->uxItemSize );

            यदि( ( *pxCoRoutineWoken ) == pdFALSE )
            {
                यदि( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToSend ) ) == pdFALSE )
                {
                    यदि( xCoRoutineRemoveFromEventList( &( pxQueue->xTasksWaitingToSend ) ) != pdFALSE )
                    {
                        *pxCoRoutineWoken = pdTRUE;
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
            }
            अन्यथा
            {
                mtCOVERAGE_TEST_MARKER();
            }

            xReturn = pdPASS;
        }
        अन्यथा
        {
            xReturn = pdFAIL;
        }

        traceRETURN_xQueueCRReceiveFromISR( xReturn );

        प्रत्यागम xReturn;
    }

#endif /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#यदि ( configQUEUE_REGISTRY_SIZE > 0 )

    शून्य vQueueAddToRegistry( QueueHandle_t xQueue,
                              ध्रुव वर्ण * pcQueueName )
    {
        UBaseType_t ux;
        QueueRegistryItem_t * pxEntryToWrite = रिक्त;

        traceENTER_vQueueAddToRegistry( xQueue, pcQueueName );

        configASSERT( xQueue );

        यदि( pcQueueName != रिक्त )
        {
            /* See if there is an empty space in the registry.  A NULL name denotes
             * a free slot. */
            प्रति( ux = ( UBaseType_t ) 0U; ux < ( UBaseType_t ) configQUEUE_REGISTRY_SIZE; ux++ )
            {
                /* Replace an existing entry if the queue is already in the registry. */
                यदि( xQueue == xQueueRegistry[ ux ].xHandle )
                {
                    pxEntryToWrite = &( xQueueRegistry[ ux ] );
                    भङ्ग;
                }
                /* Otherwise, store in the next empty location */
                अन्यथा यदि( ( pxEntryToWrite == रिक्त ) && ( xQueueRegistry[ ux ].pcQueueName == रिक्त ) )
                {
                    pxEntryToWrite = &( xQueueRegistry[ ux ] );
                }
                अन्यथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
        }

        यदि( pxEntryToWrite != रिक्त )
        {
            /* Store the information on this queue. */
            pxEntryToWrite->pcQueueName = pcQueueName;
            pxEntryToWrite->xHandle = xQueue;

            traceQUEUE_REGISTRY_ADD( xQueue, pcQueueName );
        }

        traceRETURN_vQueueAddToRegistry();
    }

#endif /* configQUEUE_REGISTRY_SIZE */
/*-----------------------------------------------------------*/

#यदि ( configQUEUE_REGISTRY_SIZE > 0 )

    ध्रुव वर्ण * pcQueueGetName( QueueHandle_t xQueue )
    {
        UBaseType_t ux;
        ध्रुव वर्ण * pcReturn = रिक्त;

        traceENTER_pcQueueGetName( xQueue );

        configASSERT( xQueue );

        /* Note there is nothing here to protect against another task adding or
         * removing entries from the registry while it is being searched. */

        प्रति( ux = ( UBaseType_t ) 0U; ux < ( UBaseType_t ) configQUEUE_REGISTRY_SIZE; ux++ )
        {
            यदि( xQueueRegistry[ ux ].xHandle == xQueue )
            {
                pcReturn = xQueueRegistry[ ux ].pcQueueName;
                भङ्ग;
            }
            अन्यथा
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }

        traceRETURN_pcQueueGetName( pcReturn );

        प्रत्यागम pcReturn;
    }

#endif /* configQUEUE_REGISTRY_SIZE */
/*-----------------------------------------------------------*/

#यदि ( configQUEUE_REGISTRY_SIZE > 0 )

    शून्य vQueueUnregisterQueue( QueueHandle_t xQueue )
    {
        UBaseType_t ux;

        traceENTER_vQueueUnregisterQueue( xQueue );

        configASSERT( xQueue );

        /* See if the handle of the queue being unregistered in actually in the
         * registry. */
        प्रति( ux = ( UBaseType_t ) 0U; ux < ( UBaseType_t ) configQUEUE_REGISTRY_SIZE; ux++ )
        {
            यदि( xQueueRegistry[ ux ].xHandle == xQueue )
            {
                /* Set the name to NULL to show that this slot if free again. */
                xQueueRegistry[ ux ].pcQueueName = रिक्त;

                /* Set the handle to NULL to ensure the same queue handle cannot
                 * appear in the registry twice if it is added, removed, then
                 * added again. */
                xQueueRegistry[ ux ].xHandle = ( QueueHandle_t ) 0;
                भङ्ग;
            }
            अन्यथा
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }

        traceRETURN_vQueueUnregisterQueue();
    }

#endif /* configQUEUE_REGISTRY_SIZE */
/*-----------------------------------------------------------*/

#यदि ( configUSE_TIMERS == 1 )

    शून्य vQueueWaitForMessageRestricted( QueueHandle_t xQueue,
                                         TickType_t xTicksToWait,
                                         ध्रुव BaseType_t xWaitIndefinitely )
    {
        Queue_t * ध्रुव pxQueue = xQueue;

        traceENTER_vQueueWaitForMessageRestricted( xQueue, xTicksToWait, xWaitIndefinitely );

        /* This function should not be called by application code hence the
         * 'Restricted' in its name.  It is not part of the public API.  It is
         * designed for use by kernel code, and has special calling requirements.
         * It can result in vListInsert() being called on a list that can only
         * possibly ever have one item in it, so the list will be fast, but even
         * so it should be called with the scheduler locked and not from a critical
         * section. */

        /* Only do anything if there are no messages in the queue.  This function
         *  will not actually cause the task to block, just place it on a blocked
         *  list.  It will not block until the scheduler is unlocked - at which
         *  time a yield will be performed.  If an item is added to the queue while
         *  the queue is locked, and the calling task blocks on the queue, then the
         *  calling task will be immediately unblocked when the queue is unlocked. */
        prvLockQueue( pxQueue );

        यदि( pxQueue->uxMessagesWaiting == ( UBaseType_t ) 0U )
        {
            /* There is nothing in the queue, block for the specified period. */
            vTaskPlaceOnEventListRestricted( &( pxQueue->xTasksWaitingToReceive ), xTicksToWait, xWaitIndefinitely );
        }
        अन्यथा
        {
            mtCOVERAGE_TEST_MARKER();
        }

        prvUnlockQueue( pxQueue );

        traceRETURN_vQueueWaitForMessageRestricted();
    }

#endif /* configUSE_TIMERS */
/*-----------------------------------------------------------*/

#यदि ( ( configUSE_QUEUE_SETS == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) )

    QueueSetHandle_t xQueueCreateSet( ध्रुव UBaseType_t uxEventQueueLength )
    {
        QueueSetHandle_t pxQueue;

        traceENTER_xQueueCreateSet( uxEventQueueLength );

        pxQueue = xQueueGenericCreate( uxEventQueueLength, ( UBaseType_t ) परिमाण( Queue_t * ), queueQUEUE_TYPE_SET );

        traceRETURN_xQueueCreateSet( pxQueue );

        प्रत्यागम pxQueue;
    }

#endif /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#यदि ( configUSE_QUEUE_SETS == 1 )

    BaseType_t xQueueAddToSet( QueueSetMemberHandle_t xQueueOrSemaphore,
                               QueueSetHandle_t xQueueSet )
    {
        BaseType_t xReturn;

        traceENTER_xQueueAddToSet( xQueueOrSemaphore, xQueueSet );

        taskENTER_CRITICAL();
        {
            यदि( ( ( Queue_t * ) xQueueOrSemaphore )->pxQueueSetContainer != रिक्त )
            {
                /* Cannot add a queue/semaphore to more than one queue set. */
                xReturn = pdFAIL;
            }
            अन्यथा यदि( ( ( Queue_t * ) xQueueOrSemaphore )->uxMessagesWaiting != ( UBaseType_t ) 0 )
            {
                /* Cannot add a queue/semaphore to a queue set if there are already
                 * items in the queue/semaphore. */
                xReturn = pdFAIL;
            }
            अन्यथा
            {
                ( ( Queue_t * ) xQueueOrSemaphore )->pxQueueSetContainer = xQueueSet;
                xReturn = pdPASS;
            }
        }
        taskEXIT_CRITICAL();

        traceRETURN_xQueueAddToSet( xReturn );

        प्रत्यागम xReturn;
    }

#endif /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#यदि ( configUSE_QUEUE_SETS == 1 )

    BaseType_t xQueueRemoveFromSet( QueueSetMemberHandle_t xQueueOrSemaphore,
                                    QueueSetHandle_t xQueueSet )
    {
        BaseType_t xReturn;
        Queue_t * ध्रुव pxQueueOrSemaphore = ( Queue_t * ) xQueueOrSemaphore;

        traceENTER_xQueueRemoveFromSet( xQueueOrSemaphore, xQueueSet );

        यदि( pxQueueOrSemaphore->pxQueueSetContainer != xQueueSet )
        {
            /* The queue was not a member of the set. */
            xReturn = pdFAIL;
        }
        अन्यथा यदि( pxQueueOrSemaphore->uxMessagesWaiting != ( UBaseType_t ) 0 )
        {
            /* It is dangerous to remove a queue from a set when the queue is
             * not empty because the queue set will still hold pending events for
             * the queue. */
            xReturn = pdFAIL;
        }
        अन्यथा
        {
            taskENTER_CRITICAL();
            {
                /* The queue is no longer contained in the set. */
                pxQueueOrSemaphore->pxQueueSetContainer = रिक्त;
            }
            taskEXIT_CRITICAL();
            xReturn = pdPASS;
        }

        traceRETURN_xQueueRemoveFromSet( xReturn );

        प्रत्यागम xReturn;
    }

#endif /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#यदि ( configUSE_QUEUE_SETS == 1 )

    QueueSetMemberHandle_t xQueueSelectFromSet( QueueSetHandle_t xQueueSet,
                                                TickType_t ध्रुव xTicksToWait )
    {
        QueueSetMemberHandle_t xReturn = रिक्त;

        traceENTER_xQueueSelectFromSet( xQueueSet, xTicksToWait );

        ( शून्य ) xQueueReceive( ( QueueHandle_t ) xQueueSet, &xReturn, xTicksToWait );

        traceRETURN_xQueueSelectFromSet( xReturn );

        प्रत्यागम xReturn;
    }

#endif /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#यदि ( configUSE_QUEUE_SETS == 1 )

    QueueSetMemberHandle_t xQueueSelectFromSetFromISR( QueueSetHandle_t xQueueSet )
    {
        QueueSetMemberHandle_t xReturn = रिक्त;

        traceENTER_xQueueSelectFromSetFromISR( xQueueSet );

        ( शून्य ) xQueueReceiveFromISR( ( QueueHandle_t ) xQueueSet, &xReturn, रिक्त );

        traceRETURN_xQueueSelectFromSetFromISR( xReturn );

        प्रत्यागम xReturn;
    }

#endif /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#यदि ( configUSE_QUEUE_SETS == 1 )

    स्थिर BaseType_t prvNotifyQueueSetContainer( ध्रुव Queue_t * ध्रुव pxQueue )
    {
        Queue_t * pxQueueSetContainer = pxQueue->pxQueueSetContainer;
        BaseType_t xReturn = pdFALSE;

        /* This function must be called form a critical section. */

        /* The following line is not reachable in unit tests because every call
         * to prvNotifyQueueSetContainer is preceded by a check that
         * pxQueueSetContainer != NULL */
        configASSERT( pxQueueSetContainer ); /* LCOV_EXCL_BR_LINE */
        configASSERT( pxQueueSetContainer->uxMessagesWaiting < pxQueueSetContainer->uxLength );

        यदि( pxQueueSetContainer->uxMessagesWaiting < pxQueueSetContainer->uxLength )
        {
            ध्रुव int8_t cTxLock = pxQueueSetContainer->cTxLock;

            traceQUEUE_SET_SEND( pxQueueSetContainer );

            /* The data copied is the handle of the queue that contains data. */
            xReturn = prvCopyDataToQueue( pxQueueSetContainer, &pxQueue, queueSEND_TO_BACK );

            यदि( cTxLock == queueUNLOCKED )
            {
                यदि( listLIST_IS_EMPTY( &( pxQueueSetContainer->xTasksWaitingToReceive ) ) == pdFALSE )
                {
                    यदि( xTaskRemoveFromEventList( &( pxQueueSetContainer->xTasksWaitingToReceive ) ) != pdFALSE )
                    {
                        /* The task waiting has a higher priority. */
                        xReturn = pdTRUE;
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
            }
            अन्यथा
            {
                prvIncrementQueueTxLock( pxQueueSetContainer, cTxLock );
            }
        }
        अन्यथा
        {
            mtCOVERAGE_TEST_MARKER();
        }

        प्रत्यागम xReturn;
    }

#endif /* configUSE_QUEUE_SETS */
