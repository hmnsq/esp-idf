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

#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#define MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#अन्तर्भाव "FreeRTOS.h"
#अन्तर्भाव "task.h"
#अन्तर्भाव "queue.h"
/* Include private IDF API additions for critical thread safety macros */
#अन्तर्भाव "esp_private/freertos_idf_additions_priv.h"

#सचे ( configUSE_CO_ROUTINES == 1 )
    #अन्तर्भाव "croutine.h"
#endif

/* Lint e9021, e961 and e750 are suppressed as a MISRA exception justified
 * because the MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#undef MPU_WRAPPERS_INCLUDED_FROM_API_FILE /*lint !e961 !e750 !e9021. */

/* Single core FreeRTOS uses queue locks to ensure that vTaskPlaceOnEventList()
 * calls are deterministic (as queue locks use scheduler suspension instead of
 * critical sections). However, the SMP implementation is non-deterministic
 * anyways, thus SMP can forego the use of queue locks (replaced with a critical
 * sections) in exchange for better queue performance. */
#सचे ( config_सङ्ख्या_OF_CORES > 1 )
    #define queueUSE_LOCKS            0
    #define queueUNLOCKED             ( ( int8_t ) 0 )
#अञ्ञथा /* configNUMBER_OF_CORES > 1 */
    #define queueUSE_LOCKS            1
    /* Constants used with the cRxLock and cTxLock structure members. */
    #define queueUNLOCKED             ( ( int8_t ) -1 )
    #define queueLOCKED_UNMODIFIED    ( ( int8_t ) 0 )
    #define queueINT8_MAX             ( ( int8_t ) 127 )
#endif /* configNUMBER_OF_CORES > 1 */

/* When the Queue_t structure is used to represent a base queue its pcHead and
 * pcTail members are used as pointers into the queue storage area.  When the
 * Queue_t structure is used to represent a mutex pcHead and pcTail pointers are
 * not necessary, and the pcHead pointer is set to NULL to indicate that the
 * structure instead holds a pointer to the mutex holder (if any).  Map alternative
 * names to the pcHead and structure member to ensure the readability of the code
 * is maintained.  The QueuePointers_t and SemaphoreData_t types are used to form
 * a union as their usage is mutually exclusive dependent on what the queue is
 * being used for. */
#define uxQueueType            pcHead
#define queueQUEUE_IS_MUTEX    रिक्त

प्रकारनिर्देश संरचना QueuePointers
{
    int8_t * pcTail;     /*< Points to the byte at the end of the queue storage area.  Once more byte is allocated than necessary to store the queue items, this is used as a marker. */
    int8_t * pcReadFrom; /*< Points to the last place that a queued item was read from when the structure is used as a queue. */
} QueuePointers_t;

प्रकारनिर्देश संरचना SemaphoreData
{
    TaskHandle_t xMutexHolder;        /*< The handle of the task that holds the mutex. */
    UBaseType_t ux_Recursive_Call_गणनम्; /*< Maintains a count of the number of times a recursive mutex has been recursively 'taken' when the structure is used as a mutex. */
} SemaphoreData_t;

/* Semaphores do not actually store or copy data, so have an item size of
 * zero. */
#define queueSEMAPHORE_QUEUE_ITEM_LENGTH    ( ( UBaseType_t ) 0 )
#define queueMUTEX_GIVE_BLOCK_TIME          ( ( TickType_t ) 0U )

#सचे ( configUSE_PREEMPTION == 0 )

/* If the cooperative scheduler is being used then a yield should not be
 * performed just because a higher priority task has been woken. */
    #define queueYIELD_IF_USING_PREEMPTION()
#अञ्ञथा
    #define queueYIELD_IF_USING_PREEMPTION()    portYIELD_WITHIN_API()
#endif

/*
 * Definition of the queue used by the scheduler.
 * Items are queued by copy, not reference.  See the following link for the
 * rationale: https://www.FreeRTOS.org/Embedded-RTOS-Queues.html
 */
प्रकारनिर्देश संरचना QueueDefinition /* The old naming convention is used to prevent breaking kernel aware debuggers. */
{
    int8_t * pcHead;           /*< Points to the beginning of the queue storage area. */
    int8_t * pcWriteTo;        /*< Points to the free next place in the storage area. */

    सङ्घ
    {
        QueuePointers_t xQueue;     /*< Data required exclusively when this structure is used as a queue. */
        SemaphoreData_t xSemaphore; /*< Data required exclusively when this structure is used as a semaphore. */
    } u;

    List_t xTasksWaitingToSend;             /*< List of tasks that are blocked waiting to post onto this queue.  Stored in priority order. */
    List_t xTasksWaitingToReceive;          /*< List of tasks that are blocked waiting to read from this queue.  Stored in priority order. */

    चञ्चल UBaseType_t uxMessagesWaiting; /*< The number of items currently in the queue. */
    UBaseType_t uxLength;                   /*< The length of the queue defined as the number of items it will hold, not the number of bytes. */
    UBaseType_t uxItemSize;                 /*< The size of each items that the queue will hold. */

    #सचे ( queueUSE_LOCKS == 1 )
        चञ्चल int8_t cRxLock; /*< Stores the number of items received from the queue (removed from the queue) while the queue was locked.  Set to queueUNLOCKED when the queue is not locked. */
        चञ्चल int8_t cTxLock; /*< Stores the number of items transmitted to the queue (added to the queue) while the queue was locked.  Set to queueUNLOCKED when the queue is not locked. */
    #endif /* queueUSE_LOCKS == 1 */

    #सचे ( ( configSUPPORT_STATIC_ALLOCATION == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) )
        uint8_t ucStaticallyAllocated; /*< Set to pdTRUE if the memory used by the queue was statically allocated to ensure no attempt is made to free the memory. */
    #endif

    #सचे ( configUSE_QUEUE_SETS == 1 )
        संरचना QueueDefinition * pxQueueSetContainer;
    #endif

    #सचे ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_Queue_सङ्ख्या;
        uint8_t ucQueueType;
    #endif

    portMUX_TYPE xQueueLock; /* Spinlock required for SMP critical sections */
} xQUEUE;

/* The old xQUEUE name is maintained above then typedefed to the new Queue_t
 * name below to enable the use of older kernel aware debuggers. */
प्रकारनिर्देश xQUEUE Queue_t;

/*-----------------------------------------------------------*/

/*
 * The queue registry is just a means for kernel aware debuggers to locate
 * queue structures.  It has no other purpose so is an optional component.
 */
#सचे ( configQUEUE_REGISTRY_SIZE > 0 )

/* The type stored within the queue registry array.  This allows a name
 * to be assigned to each queue making kernel aware debugging a little
 * more user friendly. */
    प्रकारनिर्देश संरचना QUEUE_REGISTRY_ITEM
    {
        ध्रुव वर्ण * pcQueueName; /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
        QueueHandle_t xHandle;
    } xQueueRegistryItem;

/* The old xQueueRegistryItem name is maintained above then typedefed to the
 * new xQueueRegistryItem name below to enable the use of older kernel aware
 * debuggers. */
    प्रकारनिर्देश xQueueRegistryItem QueueRegistryItem_t;

/* The queue registry is simply an array of QueueRegistryItem_t structures.
 * The pcQueueName member of a structure being NULL is indicative of the
 * array position being vacant. */
    PRIVILEGED_DATA QueueRegistryItem_t xQueueRegistry[ configQUEUE_REGISTRY_SIZE ];

    #सचे ( config_सङ्ख्या_OF_CORES > 1 )
/* Spinlock required in SMP when accessing the queue registry */
        स्थिर portMUX_TYPE xQueueRegistryLock = portMUX_INITIALIZER_UNLOCKED;
    #endif /* configNUMBER_OF_CORES > 1 */

#endif /* configQUEUE_REGISTRY_SIZE */

#सचे ( queueUSE_LOCKS == 1 )

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
#endif /* queueUSE_LOCKS == 1 */

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

#सचे ( configUSE_QUEUE_SETS == 1 )

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
#सचे ( configUSE_MUTEXES == 1 )
    स्थिर शून्य prvInitialiseMutex( Queue_t * pxNewQueue ) PRIVILEGED_FUNCTION;
#endif

#सचे ( configUSE_MUTEXES == 1 )

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

#सचे ( queueUSE_LOCKS == 1 )

/*
 * Macro to mark a queue as locked.  Locking a queue prevents an ISR from
 * accessing the queue event lists.
 */
    #define prvLockQueue( pxQueue )                        \
    taskENTER_CRITICAL( &( pxQueue->xQueueLock ) );        \
    {                                                      \
        सचे( ( pxQueue )->cRxLock == queueUNLOCKED )        \
        {                                                  \
            ( pxQueue )->cRxLock = queueLOCKED_UNMODIFIED; \
        }                                                  \
        सचे( ( pxQueue )->cTxLock == queueUNLOCKED )        \
        {                                                  \
            ( pxQueue )->cTxLock = queueLOCKED_UNMODIFIED; \
        }                                                  \
    }                                                      \
    taskEXIT_CRITICAL( &( pxQueue->xQueueLock ) )

/*
 * Macro to increment cTxLock member of the queue data structure. It is
 * capped at the number of tasks in the system as we cannot unblock more
 * tasks than the number of tasks in the system.
 */
    #define prvIncrementQueueTxLock( pxQueue, cTxLock )                       \
    {                                                                         \
        ध्रुव UBaseType_t ux_सङ्ख्या_Of_Tasks = ux_Task_Get_सङ्ख्या_Of_Tasks();         \
        सचे( ( UBaseType_t ) ( cTxLock ) < ux_सङ्ख्या_Of_Tasks )                   \
        {                                                                     \
            configASSERT( ( cTxLock ) != queueINT8_MAX );                     \
            ( pxQueue )->cTxLock = ( int8_t ) ( ( cTxLock ) + ( int8_t ) 1 ); \
        }                                                                     \
    }

/*
 * Macro to increment cRxLock member of the queue data structure. It is
 * capped at the number of tasks in the system as we cannot unblock more
 * tasks than the number of tasks in the system.
 */
    #define prvIncrementQueueRxLock( pxQueue, cRxLock )                       \
    {                                                                         \
        ध्रुव UBaseType_t ux_सङ्ख्या_Of_Tasks = ux_Task_Get_सङ्ख्या_Of_Tasks();         \
        सचे( ( UBaseType_t ) ( cRxLock ) < ux_सङ्ख्या_Of_Tasks )                   \
        {                                                                     \
            configASSERT( ( cRxLock ) != queueINT8_MAX );                     \
            ( pxQueue )->cRxLock = ( int8_t ) ( ( cRxLock ) + ( int8_t ) 1 ); \
        }                                                                     \
    }
#endif /* queueUSE_LOCKS == 1 */
/*-----------------------------------------------------------*/

BaseType_t xQueueGenericReset( QueueHandle_t xQueue,
                               BaseType_t xNewQueue )
{
    BaseType_t xReturn = pdPASS;
    Queue_t * ध्रुव pxQueue = xQueue;

    configASSERT( pxQueue );

    सचे( xNewQueue == pdTRUE )
    {
        portMUX_INITIALIZE( &( pxQueue->xQueueLock ) );
    }

    सचे( ( pxQueue != रिक्त ) &&
        ( pxQueue->uxLength >= 1U ) &&
        /* Check for multiplication overflow. */
        ( ( SIZE_MAX / pxQueue->uxLength ) >= pxQueue->uxItemSize ) )
    {
        taskENTER_CRITICAL( &( pxQueue->xQueueLock ) );
        {
            pxQueue->u.xQueue.pcTail = pxQueue->pcHead + ( pxQueue->uxLength * pxQueue->uxItemSize ); /*lint !e9016 Pointer arithmetic allowed on char types, especially when it assists conveying intent. */
            pxQueue->uxMessagesWaiting = ( UBaseType_t ) 0U;
            pxQueue->pcWriteTo = pxQueue->pcHead;
            pxQueue->u.xQueue.pcReadFrom = pxQueue->pcHead + ( ( pxQueue->uxLength - 1U ) * pxQueue->uxItemSize ); /*lint !e9016 Pointer arithmetic allowed on char types, especially when it assists conveying intent. */
            #सचे ( queueUSE_LOCKS == 1 )
            {
                pxQueue->cRxLock = queueUNLOCKED;
                pxQueue->cTxLock = queueUNLOCKED;
            }
            #endif /* queueUSE_LOCKS == 1 */

            सचे( xNewQueue == pdFALSE )
            {
                /* If there are tasks blocked waiting to read from the queue, then
                 * the tasks will remain blocked as after this function exits the queue
                 * will still be empty.  If there are tasks blocked waiting to write to
                 * the queue, then one should be unblocked as after this function exits
                 * it will be possible to write to it. */
                सचे( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToSend ) ) == pdFALSE )
                {
                    सचे( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToSend ) ) != pdFALSE )
                    {
                        queueYIELD_IF_USING_PREEMPTION();
                    }
                    अञ्ञथा
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                अञ्ञथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            अञ्ञथा
            {
                /* Ensure the event queues start in the correct state. */
                vListInitialise( &( pxQueue->xTasksWaitingToSend ) );
                vListInitialise( &( pxQueue->xTasksWaitingToReceive ) );
            }
        }
        taskEXIT_CRITICAL( &( pxQueue->xQueueLock ) );
    }
    अञ्ञथा
    {
        xReturn = pdFAIL;
    }

    configASSERT( xReturn != pdFAIL );

    /* A value is returned for calling semantic consistency with previous
     * versions. */
    पटिनिवत्त xReturn;
}
/*-----------------------------------------------------------*/

#सचे ( configSUPPORT_STATIC_ALLOCATION == 1 )

    QueueHandle_t xQueueGenericCreateStatic( ध्रुव UBaseType_t uxQueueLength,
                                             ध्रुव UBaseType_t uxItemSize,
                                             uint8_t * pucQueueStorage,
                                             StaticQueue_t * pxStaticQueue,
                                             ध्रुव uint8_t ucQueueType )
    {
        Queue_t * pxNewQueue = रिक्त;

        /* The StaticQueue_t structure and the queue storage area must be
         * supplied. */
        configASSERT( pxStaticQueue );

        सचे( ( uxQueueLength > ( UBaseType_t ) 0 ) &&
            ( pxStaticQueue != रिक्त ) &&

            /* A queue storage area should be provided if the item size is not 0, and
             * should not be provided if the item size is 0. */
            ( !( ( pucQueueStorage != रिक्त ) && ( uxItemSize == 0 ) ) ) &&
            ( !( ( pucQueueStorage == रिक्त ) && ( uxItemSize != 0 ) ) ) )
        {
            #सचे ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticQueue_t or StaticSemaphore_t equals the size of
                 * the real queue and semaphore structures. */
                चञ्चल size_t xSize = परिमाण( StaticQueue_t );

                /* This assertion cannot be branch covered in unit tests */
                configASSERT( xSize == परिमाण( Queue_t ) ); /* LCOV_EXCL_BR_LINE */
                ( शून्य ) xSize;                             /* Keeps lint quiet when configASSERT() is not defined. */
            }
            #endif /* configASSERT_DEFINED */

            /* The address of a statically allocated queue was passed in, use it.
             * The address of a statically allocated storage area was also passed in
             * but is already set. */
            pxNewQueue = ( Queue_t * ) pxStaticQueue; /*lint !e740 !e9087 Unusual cast is ok as the structures are designed to have the same alignment, and the size is checked by an assert. */

            #सचे ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
            {
                /* Queues can be allocated wither statically or dynamically, so
                 * note this queue was allocated statically in case the queue is
                 * later deleted. */
                pxNewQueue->ucStaticallyAllocated = pdTRUE;
            }
            #endif /* configSUPPORT_DYNAMIC_ALLOCATION */

            prvInitialiseNewQueue( uxQueueLength, uxItemSize, pucQueueStorage, ucQueueType, pxNewQueue );
        }
        अञ्ञथा
        {
            configASSERT( pxNewQueue );
            mtCOVERAGE_TEST_MARKER();
        }

        पटिनिवत्त pxNewQueue;
    }

#endif /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

#सचे ( configSUPPORT_STATIC_ALLOCATION == 1 )

    BaseType_t xQueueGenericGetStaticBuffers( QueueHandle_t xQueue,
                                              uint8_t ** ppucQueueStorage,
                                              StaticQueue_t ** ppxStaticQueue )
    {
        BaseType_t xReturn;
        Queue_t * ध्रुव pxQueue = xQueue;

        configASSERT( pxQueue );
        configASSERT( ppxStaticQueue );

        #सचे ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
        {
            /* Check if the queue was statically allocated. */
            सचे( pxQueue->ucStaticallyAllocated == ( uint8_t ) pdTRUE )
            {
                सचे( ppucQueueStorage != रिक्त )
                {
                    *ppucQueueStorage = ( uint8_t * ) pxQueue->pcHead;
                }

                *ppxStaticQueue = ( StaticQueue_t * ) pxQueue;
                xReturn = pdTRUE;
            }
            अञ्ञथा
            {
                xReturn = pdFALSE;
            }
        }
        #अञ्ञथा /* configSUPPORT_DYNAMIC_ALLOCATION */
        {
            /* Queue must have been statically allocated. */
            सचे( ppucQueueStorage != रिक्त )
            {
                *ppucQueueStorage = ( uint8_t * ) pxQueue->pcHead;
            }

            *ppxStaticQueue = ( StaticQueue_t * ) pxQueue;
            xReturn = pdTRUE;
        }
        #endif /* configSUPPORT_DYNAMIC_ALLOCATION */

        पटिनिवत्त xReturn;
    }

#endif /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

#सचे ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )

    QueueHandle_t xQueueGenericCreate( ध्रुव UBaseType_t uxQueueLength,
                                       ध्रुव UBaseType_t uxItemSize,
                                       ध्रुव uint8_t ucQueueType )
    {
        Queue_t * pxNewQueue = रिक्त;
        size_t xQueueSizeInBytes;
        uint8_t * pucQueueStorage;

        सचे( ( uxQueueLength > ( UBaseType_t ) 0 ) &&
            /* Check for multiplication overflow. */
            ( ( SIZE_MAX / uxQueueLength ) >= uxItemSize ) &&
            /* Check for addition overflow. */
            ( ( SIZE_MAX - परिमाण( Queue_t ) ) >= ( uxQueueLength * uxItemSize ) ) )
        {
            /* Allocate enough space to hold the maximum number of items that
             * can be in the queue at any time.  It is valid for uxItemSize to be
             * zero in the case the queue is used as a semaphore. */
            xQueueSizeInBytes = ( size_t ) ( uxQueueLength * uxItemSize ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */

            /* Allocate the queue and storage area.  Justification for MISRA
             * deviation as follows:  pvPortMalloc() always ensures returned memory
             * blocks are aligned per the requirements of the MCU stack.  In this case
             * pvPortMalloc() must return a pointer that is guaranteed to meet the
             * alignment requirements of the Queue_t structure - which in this case
             * is an int8_t *.  Therefore, whenever the stack alignment requirements
             * are greater than or equal to the pointer to char requirements the cast
             * is safe.  In other cases alignment requirements are not strict (one or
             * two bytes). */
            pxNewQueue = ( Queue_t * ) pvPortMalloc( परिमाण( Queue_t ) + xQueueSizeInBytes ); /*lint !e9087 !e9079 see comment above. */

            सचे( pxNewQueue != रिक्त )
            {
                /* Jump past the queue structure to find the location of the queue
                 * storage area. */
                pucQueueStorage = ( uint8_t * ) pxNewQueue;
                pucQueueStorage += परिमाण( Queue_t ); /*lint !e9016 Pointer arithmetic allowed on char types, especially when it assists conveying intent. */

                #सचे ( configSUPPORT_STATIC_ALLOCATION == 1 )
                {
                    /* Queues can be created either statically or dynamically, so
                     * note this task was created dynamically in case it is later
                     * deleted. */
                    pxNewQueue->ucStaticallyAllocated = pdFALSE;
                }
                #endif /* configSUPPORT_STATIC_ALLOCATION */

                prvInitialiseNewQueue( uxQueueLength, uxItemSize, pucQueueStorage, ucQueueType, pxNewQueue );
            }
            अञ्ञथा
            {
                traceQUEUE_CREATE_FAILED( ucQueueType );
                mtCOVERAGE_TEST_MARKER();
            }
        }
        अञ्ञथा
        {
            configASSERT( pxNewQueue );
            mtCOVERAGE_TEST_MARKER();
        }

        पटिनिवत्त pxNewQueue;
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

    सचे( uxItemSize == ( UBaseType_t ) 0 )
    {
        /* No RAM was allocated for the queue storage area, but PC head cannot
         * be set to NULL because NULL is used as a key to say the queue is used as
         * a mutex.  Therefore just set pcHead to point to the queue as a benign
         * value that is known to be within the memory map. */
        pxNewQueue->pcHead = ( int8_t * ) pxNewQueue;
    }
    अञ्ञथा
    {
        /* Set the head to the start of the queue storage area. */
        pxNewQueue->pcHead = ( int8_t * ) pucQueueStorage;
    }

    /* Initialise the queue members as described where the queue type is
     * defined. */
    pxNewQueue->uxLength = uxQueueLength;
    pxNewQueue->uxItemSize = uxItemSize;
    ( शून्य ) xQueueGenericReset( pxNewQueue, pdTRUE );

    #सचे ( configUSE_TRACE_FACILITY == 1 )
    {
        pxNewQueue->ucQueueType = ucQueueType;
    }
    #endif /* configUSE_TRACE_FACILITY */

    #सचे ( configUSE_QUEUE_SETS == 1 )
    {
        pxNewQueue->pxQueueSetContainer = रिक्त;
    }
    #endif /* configUSE_QUEUE_SETS */

    traceQUEUE_CREATE( pxNewQueue );
}
/*-----------------------------------------------------------*/

#सचे ( configUSE_MUTEXES == 1 )

    स्थिर शून्य prvInitialiseMutex( Queue_t * pxNewQueue )
    {
        सचे( pxNewQueue != रिक्त )
        {
            /* The queue create function will set all the queue structure members
            * correctly for a generic queue, but this function is creating a
            * mutex.  Overwrite those members that need to be set differently -
            * in particular the information required for priority inheritance. */
            pxNewQueue->u.xSemaphore.xMutexHolder = रिक्त;
            pxNewQueue->uxQueueType = queueQUEUE_IS_MUTEX;

            /* In case this is a recursive mutex. */
            pxNewQueue->u.xSemaphore.ux_Recursive_Call_गणनम् = 0;

            /* Initialize the mutex's spinlock */
            portMUX_INITIALIZE( &( pxNewQueue->xQueueLock ) );

            traceCREATE_MUTEX( pxNewQueue );

            /* Start with the semaphore in the expected state. */
            ( शून्य ) xQueueGenericSend( pxNewQueue, रिक्त, ( TickType_t ) 0U, queueSEND_TO_BACK );
        }
        अञ्ञथा
        {
            traceCREATE_MUTEX_FAILED();
        }
    }

#endif /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

#सचे ( ( configUSE_MUTEXES == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) )

    QueueHandle_t xQueueCreateMutex( ध्रुव uint8_t ucQueueType )
    {
        QueueHandle_t xNewQueue;
        ध्रुव UBaseType_t uxMutexLength = ( UBaseType_t ) 1, uxMutexSize = ( UBaseType_t ) 0;

        xNewQueue = xQueueGenericCreate( uxMutexLength, uxMutexSize, ucQueueType );
        prvInitialiseMutex( ( Queue_t * ) xNewQueue );

        पटिनिवत्त xNewQueue;
    }

#endif /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

#सचे ( ( configUSE_MUTEXES == 1 ) && ( configSUPPORT_STATIC_ALLOCATION == 1 ) )

    QueueHandle_t xQueueCreateMutexStatic( ध्रुव uint8_t ucQueueType,
                                           StaticQueue_t * pxStaticQueue )
    {
        QueueHandle_t xNewQueue;
        ध्रुव UBaseType_t uxMutexLength = ( UBaseType_t ) 1, uxMutexSize = ( UBaseType_t ) 0;

        /* Prevent compiler warnings about unused parameters if
         * configUSE_TRACE_FACILITY does not equal 1. */
        ( शून्य ) ucQueueType;

        xNewQueue = xQueueGenericCreateStatic( uxMutexLength, uxMutexSize, रिक्त, pxStaticQueue, ucQueueType );
        prvInitialiseMutex( ( Queue_t * ) xNewQueue );

        पटिनिवत्त xNewQueue;
    }

#endif /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

#सचे ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) )

    TaskHandle_t xQueueGetMutexHolder( QueueHandle_t xSemaphore )
    {
        TaskHandle_t pxReturn;
        Queue_t * ध्रुव pxSemaphore = ( Queue_t * ) xSemaphore;

        configASSERT( xSemaphore );

        /* This function is called by xSemaphoreGetMutexHolder(), and should not
         * be called directly.  Note:  This is a good way of determining if the
         * calling task is the mutex holder, but not a good way of determining the
         * identity of the mutex holder, as the holder may change between the
         * following critical section exiting and the function returning. */
        taskENTER_CRITICAL( &( pxSemaphore->xQueueLock ) );
        {
            सचे( pxSemaphore->uxQueueType == queueQUEUE_IS_MUTEX )
            {
                pxReturn = pxSemaphore->u.xSemaphore.xMutexHolder;
            }
            अञ्ञथा
            {
                pxReturn = रिक्त;
            }
        }
        taskEXIT_CRITICAL( &( pxSemaphore->xQueueLock ) );

        पटिनिवत्त pxReturn;
    } /*lint !e818 xSemaphore cannot be a pointer to const because it is a typedef. */

#endif /* if ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) ) */
/*-----------------------------------------------------------*/

#सचे ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) )

    TaskHandle_t xQueueGetMutexHolderFromISR( QueueHandle_t xSemaphore )
    {
        TaskHandle_t pxReturn;

        configASSERT( xSemaphore );

        /* Mutexes cannot be used in interrupt service routines, so the mutex
         * holder should not change in an ISR, and therefore a critical section is
         * not required here. */
        सचे( ( ( Queue_t * ) xSemaphore )->uxQueueType == queueQUEUE_IS_MUTEX )
        {
            pxReturn = ( ( Queue_t * ) xSemaphore )->u.xSemaphore.xMutexHolder;
        }
        अञ्ञथा
        {
            pxReturn = रिक्त;
        }

        पटिनिवत्त pxReturn;
    } /*lint !e818 xSemaphore cannot be a pointer to const because it is a typedef. */

#endif /* if ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) ) */
/*-----------------------------------------------------------*/

#सचे ( configUSE_RECURSIVE_MUTEXES == 1 )

    BaseType_t xQueueGiveMutexRecursive( QueueHandle_t xMutex )
    {
        BaseType_t xReturn;
        Queue_t * ध्रुव pxMutex = ( Queue_t * ) xMutex;

        configASSERT( pxMutex );

        /* If this is the task that holds the mutex then xMutexHolder will not
         * change outside of this task.  If this task does not hold the mutex then
         * pxMutexHolder can never coincidentally equal the tasks handle, and as
         * this is the only condition we are interested in it does not matter if
         * pxMutexHolder is accessed simultaneously by another task.  Therefore no
         * mutual exclusion is required to test the pxMutexHolder variable. */
        सचे( pxMutex->u.xSemaphore.xMutexHolder == xTaskGetCurrentTaskHandle() )
        {
            traceGIVE_MUTEX_RECURSIVE( pxMutex );

            /* uxRecursiveCallCount cannot be zero if xMutexHolder is equal to
             * the task handle, therefore no underflow check is required.  Also,
             * uxRecursiveCallCount is only modified by the mutex holder, and as
             * there can only be one, no mutual exclusion is required to modify the
             * uxRecursiveCallCount member. */
            ( pxMutex->u.xSemaphore.ux_Recursive_Call_गणनम् )--;

            /* Has the recursive call count unwound to 0? */
            सचे( pxMutex->u.xSemaphore.ux_Recursive_Call_गणनम् == ( UBaseType_t ) 0 )
            {
                /* Return the mutex.  This will automatically unblock any other
                 * task that might be waiting to access the mutex. */
                ( शून्य ) xQueueGenericSend( pxMutex, रिक्त, queueMUTEX_GIVE_BLOCK_TIME, queueSEND_TO_BACK );
            }
            अञ्ञथा
            {
                mtCOVERAGE_TEST_MARKER();
            }

            xReturn = pdPASS;
        }
        अञ्ञथा
        {
            /* The mutex cannot be given because the calling task is not the
             * holder. */
            xReturn = pdFAIL;

            traceGIVE_MUTEX_RECURSIVE_FAILED( pxMutex );
        }

        पटिनिवत्त xReturn;
    }

#endif /* configUSE_RECURSIVE_MUTEXES */
/*-----------------------------------------------------------*/

#सचे ( configUSE_RECURSIVE_MUTEXES == 1 )

    BaseType_t xQueueTakeMutexRecursive( QueueHandle_t xMutex,
                                         TickType_t xTicksToWait )
    {
        BaseType_t xReturn;
        Queue_t * ध्रुव pxMutex = ( Queue_t * ) xMutex;

        configASSERT( pxMutex );

        /* Comments regarding mutual exclusion as per those within
         * xQueueGiveMutexRecursive(). */

        traceTAKE_MUTEX_RECURSIVE( pxMutex );

        सचे( pxMutex->u.xSemaphore.xMutexHolder == xTaskGetCurrentTaskHandle() )
        {
            ( pxMutex->u.xSemaphore.ux_Recursive_Call_गणनम् )++;
            xReturn = pdPASS;
        }
        अञ्ञथा
        {
            xReturn = xQueueSemaphoreTake( pxMutex, xTicksToWait );

            /* pdPASS will only be returned if the mutex was successfully
             * obtained.  The calling task may have entered the Blocked state
             * before reaching here. */
            सचे( xReturn != pdFAIL )
            {
                ( pxMutex->u.xSemaphore.ux_Recursive_Call_गणनम् )++;
            }
            अञ्ञथा
            {
                traceTAKE_MUTEX_RECURSIVE_FAILED( pxMutex );
            }
        }

        पटिनिवत्त xReturn;
    }

#endif /* configUSE_RECURSIVE_MUTEXES */
/*-----------------------------------------------------------*/

#सचे ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( configSUPPORT_STATIC_ALLOCATION == 1 ) )

    QueueHandle_t xQueueCreateCountingSemaphoreStatic( ध्रुव UBaseType_t ux_Max_गणनम्,
                                                       ध्रुव UBaseType_t ux_Initial_गणनम्,
                                                       StaticQueue_t * pxStaticQueue )
    {
        QueueHandle_t xHandle = रिक्त;

        सचे( ( ux_Max_गणनम् != 0 ) &&
            ( ux_Initial_गणनम् <= ux_Max_गणनम् ) )
        {
            xHandle = xQueueGenericCreateStatic( ux_Max_गणनम्, queueSEMAPHORE_QUEUE_ITEM_LENGTH, रिक्त, pxStaticQueue, queueQUEUE_TYPE_COUNTING_SEMAPHORE );

            सचे( xHandle != रिक्त )
            {
                ( ( Queue_t * ) xHandle )->uxMessagesWaiting = ux_Initial_गणनम्;

                traceCREATE_COUNTING_SEMAPHORE();
            }
            अञ्ञथा
            {
                traceCREATE_COUNTING_SEMAPHORE_FAILED();
            }
        }
        अञ्ञथा
        {
            configASSERT( xHandle );
            mtCOVERAGE_TEST_MARKER();
        }

        पटिनिवत्त xHandle;
    }

#endif /* ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) ) */
/*-----------------------------------------------------------*/

#सचे ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) )

    QueueHandle_t xQueueCreateCountingSemaphore( ध्रुव UBaseType_t ux_Max_गणनम्,
                                                 ध्रुव UBaseType_t ux_Initial_गणनम् )
    {
        QueueHandle_t xHandle = रिक्त;

        सचे( ( ux_Max_गणनम् != 0 ) &&
            ( ux_Initial_गणनम् <= ux_Max_गणनम् ) )
        {
            xHandle = xQueueGenericCreate( ux_Max_गणनम्, queueSEMAPHORE_QUEUE_ITEM_LENGTH, queueQUEUE_TYPE_COUNTING_SEMAPHORE );

            सचे( xHandle != रिक्त )
            {
                ( ( Queue_t * ) xHandle )->uxMessagesWaiting = ux_Initial_गणनम्;

                traceCREATE_COUNTING_SEMAPHORE();
            }
            अञ्ञथा
            {
                traceCREATE_COUNTING_SEMAPHORE_FAILED();
            }
        }
        अञ्ञथा
        {
            configASSERT( xHandle );
            mtCOVERAGE_TEST_MARKER();
        }

        पटिनिवत्त xHandle;
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

    configASSERT( pxQueue );
    configASSERT( !( ( pvItemToQueue == रिक्त ) && ( pxQueue->uxItemSize != ( UBaseType_t ) 0U ) ) );
    configASSERT( !( ( xCopyPosition == queueOVERWRITE ) && ( pxQueue->uxLength != 1 ) ) );
    #सचे ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( xTicksToWait != 0 ) ) );
    }
    #endif

    /*lint -save -e904 This function relaxes the coding standard somewhat to
     * allow return statements within the function itself.  This is done in the
     * interest of execution time efficiency. */
    अत्थाय( ; ; )
    {
        taskENTER_CRITICAL( &( pxQueue->xQueueLock ) );
        {
            /* Is there room on the queue now?  The running task must be the
             * highest priority task wanting to access the queue.  If the head item
             * in the queue is to be overwritten then it does not matter if the
             * queue is full. */
            सचे( ( pxQueue->uxMessagesWaiting < pxQueue->uxLength ) || ( xCopyPosition == queueOVERWRITE ) )
            {
                traceQUEUE_SEND( pxQueue );

                #सचे ( configUSE_QUEUE_SETS == 1 )
                {
                    ध्रुव UBaseType_t uxPreviousMessagesWaiting = pxQueue->uxMessagesWaiting;

                    xYieldRequired = prvCopyDataToQueue( pxQueue, pvItemToQueue, xCopyPosition );

                    सचे( pxQueue->pxQueueSetContainer != रिक्त )
                    {
                        सचे( ( xCopyPosition == queueOVERWRITE ) && ( uxPreviousMessagesWaiting != ( UBaseType_t ) 0 ) )
                        {
                            /* Do not notify the queue set as an existing item
                             * was overwritten in the queue so the number of items
                             * in the queue has not changed. */
                            mtCOVERAGE_TEST_MARKER();
                        }
                        अञ्ञथा सचे( prvNotifyQueueSetContainer( pxQueue ) != pdFALSE )
                        {
                            /* The queue is a member of a queue set, and posting
                             * to the queue set caused a higher priority task to
                             * unblock. A context switch is required. */
                            queueYIELD_IF_USING_PREEMPTION();
                        }
                        अञ्ञथा
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    अञ्ञथा
                    {
                        /* If there was a task waiting for data to arrive on the
                         * queue then unblock it now. */
                        सचे( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToReceive ) ) == pdFALSE )
                        {
                            सचे( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToReceive ) ) != pdFALSE )
                            {
                                /* The unblocked task has a priority higher than
                                 * our own so yield immediately.  Yes it is ok to
                                 * do this from within the critical section - the
                                 * kernel takes care of that. */
                                queueYIELD_IF_USING_PREEMPTION();
                            }
                            अञ्ञथा
                            {
                                mtCOVERAGE_TEST_MARKER();
                            }
                        }
                        अञ्ञथा सचे( xYieldRequired != pdFALSE )
                        {
                            /* This path is a special case that will only get
                             * executed if the task was holding multiple mutexes
                             * and the mutexes were given back in an order that is
                             * different to that in which they were taken. */
                            queueYIELD_IF_USING_PREEMPTION();
                        }
                        अञ्ञथा
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                }
                #अञ्ञथा /* configUSE_QUEUE_SETS */
                {
                    xYieldRequired = prvCopyDataToQueue( pxQueue, pvItemToQueue, xCopyPosition );

                    /* If there was a task waiting for data to arrive on the
                     * queue then unblock it now. */
                    सचे( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToReceive ) ) == pdFALSE )
                    {
                        सचे( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToReceive ) ) != pdFALSE )
                        {
                            /* The unblocked task has a priority higher than
                             * our own so yield immediately.  Yes it is ok to do
                             * this from within the critical section - the kernel
                             * takes care of that. */
                            queueYIELD_IF_USING_PREEMPTION();
                        }
                        अञ्ञथा
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    अञ्ञथा सचे( xYieldRequired != pdFALSE )
                    {
                        /* This path is a special case that will only get
                         * executed if the task was holding multiple mutexes and
                         * the mutexes were given back in an order that is
                         * different to that in which they were taken. */
                        queueYIELD_IF_USING_PREEMPTION();
                    }
                    अञ्ञथा
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #endif /* configUSE_QUEUE_SETS */

                taskEXIT_CRITICAL( &( pxQueue->xQueueLock ) );
                पटिनिवत्त pdPASS;
            }
            अञ्ञथा
            {
                सचे( xTicksToWait == ( TickType_t ) 0 )
                {
                    /* The queue was full and no block time is specified (or
                     * the block time has expired) so leave now. */
                    taskEXIT_CRITICAL( &( pxQueue->xQueueLock ) );

                    /* Return to the original privilege level before exiting
                     * the function. */
                    traceQUEUE_SEND_FAILED( pxQueue );
                    पटिनिवत्त errQUEUE_FULL;
                }
                अञ्ञथा सचे( xEntryTimeSet == pdFALSE )
                {
                    /* The queue was full and a block time was specified so
                     * configure the timeout structure. */
                    vTaskInternalSetTimeOutState( &xTimeOut );
                    xEntryTimeSet = pdTRUE;
                }
                अञ्ञथा
                {
                    /* Entry time was already set. */
                    mtCOVERAGE_TEST_MARKER();
                }
            }

            /* If queue locks ARE NOT being used:
             * - At this point, the queue is full and entry time has been set
             * - We simply check for a time out, block if not timed out, or
             *   return an error if we have timed out. */
            #सचे ( queueUSE_LOCKS == 0 )
            {
                /* Update the timeout state to see if it has expired yet. */
                सचे( xTaskCheckForTimeOut( &xTimeOut, &xTicksToWait ) == pdFALSE )
                {
                    /* Not timed out yet. Block the current task. */
                    traceBLOCKING_ON_QUEUE_SEND( pxQueue );
                    vTaskPlaceOnEventList( &( pxQueue->xTasksWaitingToSend ), xTicksToWait );
                    portYIELD_WITHIN_API();
                }
                अञ्ञथा
                {
                    /* We have timed out. Return an error. */
                    taskEXIT_CRITICAL( &( pxQueue->xQueueLock ) );
                    traceQUEUE_SEND_FAILED( pxQueue );
                    पटिनिवत्त errQUEUE_FULL;
                }
            }
            #endif /* queueUSE_LOCKS == 0 */
        }
        taskEXIT_CRITICAL( &( pxQueue->xQueueLock ) );

        /* If queue locks ARE being used:
         * - At this point, the queue is full and entry time has been set
         * - We follow the original procedure of locking the queue before
         *   attempting to block. */
        #सचे ( queueUSE_LOCKS == 1 )
        {
            /* Interrupts and other tasks can send to and receive from the queue
             * now the critical section has been exited. */

            vTaskSuspendAll();
            prvLockQueue( pxQueue );

            /* Update the timeout state to see if it has expired yet. */
            सचे( xTaskCheckForTimeOut( &xTimeOut, &xTicksToWait ) == pdFALSE )
            {
                सचे( prvIsQueueFull( pxQueue ) != pdFALSE )
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
                    सचे( xTaskResumeAll() == pdFALSE )
                    {
                        portYIELD_WITHIN_API();
                    }
                }
                अञ्ञथा
                {
                    /* Try again. */
                    prvUnlockQueue( pxQueue );
                    ( शून्य ) xTaskResumeAll();
                }
            }
            अञ्ञथा
            {
                /* The timeout has expired. */
                prvUnlockQueue( pxQueue );
                ( शून्य ) xTaskResumeAll();

                traceQUEUE_SEND_FAILED( pxQueue );
                पटिनिवत्त errQUEUE_FULL;
            }
        }
        #endif /* queueUSE_LOCKS == 1 */
    } /*lint -restore */
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
    prvENTER_CRITICAL_OR_MASK_ISR( &( pxQueue->xQueueLock ), uxSavedInterruptStatus );
    {
        सचे( ( pxQueue->uxMessagesWaiting < pxQueue->uxLength ) || ( xCopyPosition == queueOVERWRITE ) )
        {
            #सचे ( queueUSE_LOCKS == 1 )
                ध्रुव int8_t cTxLock = pxQueue->cTxLock;
            #अञ्ञथा
                /* Queue locks not used, so we treat it as unlocked. */
                ध्रुव int8_t cTxLock = queueUNLOCKED;
            #endif /* queueUSE_LOCKS == 1 */
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
            सचे( cTxLock == queueUNLOCKED )
            {
                #सचे ( configUSE_QUEUE_SETS == 1 )
                {
                    सचे( pxQueue->pxQueueSetContainer != रिक्त )
                    {
                        सचे( ( xCopyPosition == queueOVERWRITE ) && ( uxPreviousMessagesWaiting != ( UBaseType_t ) 0 ) )
                        {
                            /* Do not notify the queue set as an existing item
                             * was overwritten in the queue so the number of items
                             * in the queue has not changed. */
                            mtCOVERAGE_TEST_MARKER();
                        }
                        अञ्ञथा सचे( prvNotifyQueueSetContainer( pxQueue ) != pdFALSE )
                        {
                            /* The queue is a member of a queue set, and posting
                             * to the queue set caused a higher priority task to
                             * unblock.  A context switch is required. */
                            सचे( pxHigherPriorityTaskWoken != रिक्त )
                            {
                                *pxHigherPriorityTaskWoken = pdTRUE;
                            }
                            अञ्ञथा
                            {
                                mtCOVERAGE_TEST_MARKER();
                            }
                        }
                        अञ्ञथा
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    अञ्ञथा
                    {
                        सचे( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToReceive ) ) == pdFALSE )
                        {
                            सचे( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToReceive ) ) != pdFALSE )
                            {
                                /* The task waiting has a higher priority so
                                 *  record that a context switch is required. */
                                सचे( pxHigherPriorityTaskWoken != रिक्त )
                                {
                                    *pxHigherPriorityTaskWoken = pdTRUE;
                                }
                                अञ्ञथा
                                {
                                    mtCOVERAGE_TEST_MARKER();
                                }
                            }
                            अञ्ञथा
                            {
                                mtCOVERAGE_TEST_MARKER();
                            }
                        }
                        अञ्ञथा
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                }
                #अञ्ञथा /* configUSE_QUEUE_SETS */
                {
                    सचे( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToReceive ) ) == pdFALSE )
                    {
                        सचे( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToReceive ) ) != pdFALSE )
                        {
                            /* The task waiting has a higher priority so record that a
                             * context switch is required. */
                            सचे( pxHigherPriorityTaskWoken != रिक्त )
                            {
                                *pxHigherPriorityTaskWoken = pdTRUE;
                            }
                            अञ्ञथा
                            {
                                mtCOVERAGE_TEST_MARKER();
                            }
                        }
                        अञ्ञथा
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    अञ्ञथा
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }

                    /* Not used in this path. */
                    ( शून्य ) uxPreviousMessagesWaiting;
                }
                #endif /* configUSE_QUEUE_SETS */
            }
            अञ्ञथा
            {
                #सचे ( queueUSE_LOCKS == 1 )
                {
                    /* Increment the lock count so the task that unlocks the queue
                     * knows that data was posted while it was locked. */
                    prvIncrementQueueTxLock( pxQueue, cTxLock );
                }
                #endif /* queueUSE_LOCKS == 1 */
            }

            xReturn = pdPASS;
        }
        अञ्ञथा
        {
            traceQUEUE_SEND_FROM_ISR_FAILED( pxQueue );
            xReturn = errQUEUE_FULL;
        }
    }
    prvEXIT_CRITICAL_OR_UNMASK_ISR( &( pxQueue->xQueueLock ), uxSavedInterruptStatus );

    पटिनिवत्त xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t xQueueGiveFromISR( QueueHandle_t xQueue,
                              BaseType_t * ध्रुव pxHigherPriorityTaskWoken )
{
    BaseType_t xReturn;
    UBaseType_t uxSavedInterruptStatus;
    Queue_t * ध्रुव pxQueue = xQueue;

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

    prvENTER_CRITICAL_OR_MASK_ISR( &( pxQueue->xQueueLock ), uxSavedInterruptStatus );
    {
        ध्रुव UBaseType_t uxMessagesWaiting = pxQueue->uxMessagesWaiting;

        /* When the queue is used to implement a semaphore no data is ever
         * moved through the queue but it is still valid to see if the queue 'has
         * space'. */
        सचे( uxMessagesWaiting < pxQueue->uxLength )
        {
            #सचे ( queueUSE_LOCKS == 1 )
                ध्रुव int8_t cTxLock = pxQueue->cTxLock;
            #अञ्ञथा
                /* Queue locks not used, so we treat it as unlocked. */
                ध्रुव int8_t cTxLock = queueUNLOCKED;
            #endif /* queueUSE_LOCKS == 1 */

            /* Todo: Reconcile tracing differences (IDF-8183) */
            traceQUEUE_GIVE_FROM_ISR( pxQueue );

            /* A task can only have an inherited priority if it is a mutex
             * holder - and if there is a mutex holder then the mutex cannot be
             * given from an ISR.  As this is the ISR version of the function it
             * can be assumed there is no mutex holder and no need to determine if
             * priority disinheritance is needed.  Simply increase the count of
             * messages (semaphores) available. */
            pxQueue->uxMessagesWaiting = uxMessagesWaiting + ( UBaseType_t ) 1;

            /* The event list is not altered if the queue is locked.  This will
             * be done when the queue is unlocked later. */
            सचे( cTxLock == queueUNLOCKED )
            {
                #सचे ( configUSE_QUEUE_SETS == 1 )
                {
                    सचे( pxQueue->pxQueueSetContainer != रिक्त )
                    {
                        सचे( prvNotifyQueueSetContainer( pxQueue ) != pdFALSE )
                        {
                            /* The semaphore is a member of a queue set, and
                             * posting to the queue set caused a higher priority
                             * task to unblock.  A context switch is required. */
                            सचे( pxHigherPriorityTaskWoken != रिक्त )
                            {
                                *pxHigherPriorityTaskWoken = pdTRUE;
                            }
                            अञ्ञथा
                            {
                                mtCOVERAGE_TEST_MARKER();
                            }
                        }
                        अञ्ञथा
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    अञ्ञथा
                    {
                        सचे( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToReceive ) ) == pdFALSE )
                        {
                            सचे( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToReceive ) ) != pdFALSE )
                            {
                                /* The task waiting has a higher priority so
                                 *  record that a context switch is required. */
                                सचे( pxHigherPriorityTaskWoken != रिक्त )
                                {
                                    *pxHigherPriorityTaskWoken = pdTRUE;
                                }
                                अञ्ञथा
                                {
                                    mtCOVERAGE_TEST_MARKER();
                                }
                            }
                            अञ्ञथा
                            {
                                mtCOVERAGE_TEST_MARKER();
                            }
                        }
                        अञ्ञथा
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                }
                #अञ्ञथा /* configUSE_QUEUE_SETS */
                {
                    सचे( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToReceive ) ) == pdFALSE )
                    {
                        सचे( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToReceive ) ) != pdFALSE )
                        {
                            /* The task waiting has a higher priority so record that a
                             * context switch is required. */
                            सचे( pxHigherPriorityTaskWoken != रिक्त )
                            {
                                *pxHigherPriorityTaskWoken = pdTRUE;
                            }
                            अञ्ञथा
                            {
                                mtCOVERAGE_TEST_MARKER();
                            }
                        }
                        अञ्ञथा
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    अञ्ञथा
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #endif /* configUSE_QUEUE_SETS */
            }
            अञ्ञथा
            {
                #सचे ( queueUSE_LOCKS == 1 )
                {
                    /* Increment the lock count so the task that unlocks the queue
                     * knows that data was posted while it was locked. */
                    prvIncrementQueueTxLock( pxQueue, cTxLock );
                }
                #endif /* queueUSE_LOCKS == 1 */
            }

            xReturn = pdPASS;
        }
        अञ्ञथा
        {
            /* Todo: Reconcile tracing differences (IDF-8183) */
            traceQUEUE_GIVE_FROM_ISR_FAILED( pxQueue );
            xReturn = errQUEUE_FULL;
        }
    }
    prvEXIT_CRITICAL_OR_UNMASK_ISR( &( pxQueue->xQueueLock ), uxSavedInterruptStatus );

    पटिनिवत्त xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t xQueueReceive( QueueHandle_t xQueue,
                          शून्य * ध्रुव pvBuffer,
                          TickType_t xTicksToWait )
{
    BaseType_t xEntryTimeSet = pdFALSE;
    TimeOut_t xTimeOut;
    Queue_t * ध्रुव pxQueue = xQueue;

    /* Check the pointer is not NULL. */
    configASSERT( ( pxQueue ) );

    /* The buffer into which data is received can only be NULL if the data size
     * is zero (so no data is copied into the buffer). */
    configASSERT( !( ( ( pvBuffer ) == रिक्त ) && ( ( pxQueue )->uxItemSize != ( UBaseType_t ) 0U ) ) );

    /* Cannot block if the scheduler is suspended. */
    #सचे ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( xTicksToWait != 0 ) ) );
    }
    #endif

    /*lint -save -e904  This function relaxes the coding standard somewhat to
     * allow return statements within the function itself.  This is done in the
     * interest of execution time efficiency. */
    अत्थाय( ; ; )
    {
        taskENTER_CRITICAL( &( pxQueue->xQueueLock ) );
        {
            ध्रुव UBaseType_t uxMessagesWaiting = pxQueue->uxMessagesWaiting;

            /* Is there data in the queue now?  To be running the calling task
             * must be the highest priority task wanting to access the queue. */
            सचे( uxMessagesWaiting > ( UBaseType_t ) 0 )
            {
                /* Data available, remove one item. */
                prvCopyDataFromQueue( pxQueue, pvBuffer );
                traceQUEUE_RECEIVE( pxQueue );
                pxQueue->uxMessagesWaiting = uxMessagesWaiting - ( UBaseType_t ) 1;

                /* There is now space in the queue, were any tasks waiting to
                 * post to the queue?  If so, unblock the highest priority waiting
                 * task. */
                सचे( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToSend ) ) == pdFALSE )
                {
                    सचे( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToSend ) ) != pdFALSE )
                    {
                        queueYIELD_IF_USING_PREEMPTION();
                    }
                    अञ्ञथा
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                अञ्ञथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                taskEXIT_CRITICAL( &( pxQueue->xQueueLock ) );
                पटिनिवत्त pdPASS;
            }
            अञ्ञथा
            {
                सचे( xTicksToWait == ( TickType_t ) 0 )
                {
                    /* The queue was empty and no block time is specified (or
                     * the block time has expired) so leave now. */
                    taskEXIT_CRITICAL( &( pxQueue->xQueueLock ) );
                    traceQUEUE_RECEIVE_FAILED( pxQueue );
                    पटिनिवत्त errQUEUE_EMPTY;
                }
                अञ्ञथा सचे( xEntryTimeSet == pdFALSE )
                {
                    /* The queue was empty and a block time was specified so
                     * configure the timeout structure. */
                    vTaskInternalSetTimeOutState( &xTimeOut );
                    xEntryTimeSet = pdTRUE;
                }
                अञ्ञथा
                {
                    /* Entry time was already set. */
                    mtCOVERAGE_TEST_MARKER();
                }
            }

            /* If queue locks ARE NOT being used:
             * - At this point, the queue is empty and entry time has been set
             * - We simply check for a time out, block if not timed out, or
             *   return an error if we have timed out. */
            #सचे ( queueUSE_LOCKS == 0 )
            {
                /* Update the timeout state to see if it has expired yet. */
                सचे( xTaskCheckForTimeOut( &xTimeOut, &xTicksToWait ) == pdFALSE )
                {
                    /* Not timed out yet. Block the current task. */
                    traceBLOCKING_ON_QUEUE_RECEIVE( pxQueue );
                    vTaskPlaceOnEventList( &( pxQueue->xTasksWaitingToReceive ), xTicksToWait );
                    portYIELD_WITHIN_API();
                }
                अञ्ञथा
                {
                    /* We have timed out. Return an error. */
                    taskEXIT_CRITICAL( &( pxQueue->xQueueLock ) );
                    traceQUEUE_RECEIVE_FAILED( pxQueue );
                    पटिनिवत्त errQUEUE_EMPTY;
                }
            }
            #endif /* queueUSE_LOCKS == 0 */
        }
        taskEXIT_CRITICAL( &( pxQueue->xQueueLock ) );

        /* If queue locks ARE being used:
         * - At this point, the queue is empty and entry time has been set
         * - We follow the original procedure for locking the queue before
         *   attempting to block. */
        #सचे ( queueUSE_LOCKS == 1 )
        {
            /* Interrupts and other tasks can send to and receive from the queue
             * now the critical section has been exited. */

            vTaskSuspendAll();
            prvLockQueue( pxQueue );

            /* Update the timeout state to see if it has expired yet. */
            सचे( xTaskCheckForTimeOut( &xTimeOut, &xTicksToWait ) == pdFALSE )
            {
                /* The timeout has not expired.  If the queue is still empty place
                 * the task on the list of tasks waiting to receive from the queue. */
                सचे( prvIsQueueEmpty( pxQueue ) != pdFALSE )
                {
                    traceBLOCKING_ON_QUEUE_RECEIVE( pxQueue );
                    vTaskPlaceOnEventList( &( pxQueue->xTasksWaitingToReceive ), xTicksToWait );
                    prvUnlockQueue( pxQueue );

                    सचे( xTaskResumeAll() == pdFALSE )
                    {
                        portYIELD_WITHIN_API();
                    }
                    अञ्ञथा
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                अञ्ञथा
                {
                    /* The queue contains data again.  Loop back to try and read the
                     * data. */
                    prvUnlockQueue( pxQueue );
                    ( शून्य ) xTaskResumeAll();
                }
            }
            अञ्ञथा
            {
                /* Timed out.  If there is no data in the queue exit, otherwise loop
                 * back and attempt to read the data. */
                prvUnlockQueue( pxQueue );
                ( शून्य ) xTaskResumeAll();

                सचे( prvIsQueueEmpty( pxQueue ) != pdFALSE )
                {
                    traceQUEUE_RECEIVE_FAILED( pxQueue );
                    पटिनिवत्त errQUEUE_EMPTY;
                }
                अञ्ञथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
        }
        #endif /* queueUSE_LOCKS == 1 */
    } /*lint -restore */
}
/*-----------------------------------------------------------*/

BaseType_t xQueueSemaphoreTake( QueueHandle_t xQueue,
                                TickType_t xTicksToWait )
{
    BaseType_t xEntryTimeSet = pdFALSE;
    TimeOut_t xTimeOut;
    Queue_t * ध्रुव pxQueue = xQueue;

    #सचे ( configUSE_MUTEXES == 1 )
        BaseType_t xInheritanceOccurred = pdFALSE;
    #endif

    /* Check the queue pointer is not NULL. */
    configASSERT( ( pxQueue ) );

    /* Check this really is a semaphore, in which case the item size will be
     * 0. */
    configASSERT( pxQueue->uxItemSize == 0 );

    /* Cannot block if the scheduler is suspended. */
    #सचे ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( xTicksToWait != 0 ) ) );
    }
    #endif

    /*lint -save -e904 This function relaxes the coding standard somewhat to allow return
     * statements within the function itself.  This is done in the interest
     * of execution time efficiency. */
    अत्थाय( ; ; )
    {
        taskENTER_CRITICAL( &( pxQueue->xQueueLock ) );
        {
            /* Semaphores are queues with an item size of 0, and where the
             * number of messages in the queue is the semaphore's count value. */
            ध्रुव UBaseType_t ux_Semaphore_गणनम् = pxQueue->uxMessagesWaiting;

            /* Is there data in the queue now?  To be running the calling task
             * must be the highest priority task wanting to access the queue. */
            सचे( ux_Semaphore_गणनम् > ( UBaseType_t ) 0 )
            {
                /* Todo: Reconcile tracing differences (IDF-8183) */
                traceQUEUE_SEMAPHORE_RECEIVE( pxQueue );

                /* Semaphores are queues with a data size of zero and where the
                 * messages waiting is the semaphore's count.  Reduce the count. */
                pxQueue->uxMessagesWaiting = ux_Semaphore_गणनम् - ( UBaseType_t ) 1;

                #सचे ( configUSE_MUTEXES == 1 )
                {
                    सचे( pxQueue->uxQueueType == queueQUEUE_IS_MUTEX )
                    {
                        /* Record the information required to implement
                         * priority inheritance should it become necessary. */
                        pxQueue->u.xSemaphore.xMutexHolder = pv_Task_Increment_Mutex_Held_गणनम्();
                    }
                    अञ्ञथा
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #endif /* configUSE_MUTEXES */

                /* Check to see if other tasks are blocked waiting to give the
                 * semaphore, and if so, unblock the highest priority such task. */
                सचे( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToSend ) ) == pdFALSE )
                {
                    सचे( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToSend ) ) != pdFALSE )
                    {
                        queueYIELD_IF_USING_PREEMPTION();
                    }
                    अञ्ञथा
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                अञ्ञथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                taskEXIT_CRITICAL( &( pxQueue->xQueueLock ) );
                पटिनिवत्त pdPASS;
            }
            अञ्ञथा
            {
                सचे( xTicksToWait == ( TickType_t ) 0 )
                {
                    /* The semaphore count was 0 and no block time is specified
                     * (or the block time has expired) so exit now. */
                    taskEXIT_CRITICAL( &( pxQueue->xQueueLock ) );
                    traceQUEUE_RECEIVE_FAILED( pxQueue );
                    पटिनिवत्त errQUEUE_EMPTY;
                }
                अञ्ञथा सचे( xEntryTimeSet == pdFALSE )
                {
                    /* The semaphore count was 0 and a block time was specified
                     * so configure the timeout structure ready to block. */
                    vTaskInternalSetTimeOutState( &xTimeOut );
                    xEntryTimeSet = pdTRUE;
                }
                अञ्ञथा
                {
                    /* Entry time was already set. */
                    mtCOVERAGE_TEST_MARKER();
                }
            }

            /* If queue locks ARE NOT being used:
             * - At this point, the semaphore/mutex is empty/held and entry time
             *   has been set.
             * - We simply check for a time out, inherit priority and block if
             *   not timed out, or return an error if we have timed out. */
            #सचे ( queueUSE_LOCKS == 0 )
            {
                /* Update the timeout state to see if it has expired yet. */
                सचे( xTaskCheckForTimeOut( &xTimeOut, &xTicksToWait ) == pdFALSE )
                {
                    /* Not timed out yet. If this is a mutex, make the holder
                     * inherit our priority, then block the current task. */
                    traceBLOCKING_ON_QUEUE_RECEIVE( pxQueue );
                    #सचे ( configUSE_MUTEXES == 1 )
                    {
                        सचे( pxQueue->uxQueueType == queueQUEUE_IS_MUTEX )
                        {
                            xInheritanceOccurred = xTaskPriorityInherit( pxQueue->u.xSemaphore.xMutexHolder );
                        }
                        अञ्ञथा
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    #endif /* if ( configUSE_MUTEXES == 1 ) */
                    vTaskPlaceOnEventList( &( pxQueue->xTasksWaitingToReceive ), xTicksToWait );
                    portYIELD_WITHIN_API();
                }
                अञ्ञथा
                {
                    /* We have timed out. If this is a mutex, make the holder
                     * disinherit our priority, then return an error. */
                    #सचे ( configUSE_MUTEXES == 1 )
                    {
                        सचे( xInheritanceOccurred != pdFALSE )
                        {
                            UBaseType_t uxHighestWaitingPriority;
                            uxHighestWaitingPriority = prvGetDisinheritPriorityAfterTimeout( pxQueue );
                            vTaskPriorityDisinheritAfterTimeout( pxQueue->u.xSemaphore.xMutexHolder, uxHighestWaitingPriority );
                        }
                    }
                    #endif /* configUSE_MUTEXES */
                    taskEXIT_CRITICAL( &( pxQueue->xQueueLock ) );
                    traceQUEUE_RECEIVE_FAILED( pxQueue );
                    पटिनिवत्त errQUEUE_EMPTY;
                }
            }
            #endif /* queueUSE_LOCKS == 0 */
        }
        taskEXIT_CRITICAL( &( pxQueue->xQueueLock ) );

        /* If queue locks ARE being used:
         * - At this point, the semaphore/mutex is empty/held and entry time
         *   has been set.
         * - We follow the original procedure for locking the queue, inheriting
         *   priority, then attempting to block. */
        #सचे ( queueUSE_LOCKS == 1 )
        {
            /* Interrupts and other tasks can give to and take from the semaphore
             * now the critical section has been exited. */

            vTaskSuspendAll();
            prvLockQueue( pxQueue );

            /* Update the timeout state to see if it has expired yet. */
            सचे( xTaskCheckForTimeOut( &xTimeOut, &xTicksToWait ) == pdFALSE )
            {
                /* A block time is specified and not expired.  If the semaphore
                 * count is 0 then enter the Blocked state to wait for a semaphore to
                 * become available.  As semaphores are implemented with queues the
                 * queue being empty is equivalent to the semaphore count being 0. */
                सचे( prvIsQueueEmpty( pxQueue ) != pdFALSE )
                {
                    traceBLOCKING_ON_QUEUE_RECEIVE( pxQueue );

                    #सचे ( configUSE_MUTEXES == 1 )
                    {
                        सचे( pxQueue->uxQueueType == queueQUEUE_IS_MUTEX )
                        {
                            taskENTER_CRITICAL( &( pxQueue->xQueueLock ) );
                            {
                                xInheritanceOccurred = xTaskPriorityInherit( pxQueue->u.xSemaphore.xMutexHolder );
                            }
                            taskEXIT_CRITICAL( &( pxQueue->xQueueLock ) );
                        }
                        अञ्ञथा
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    #endif /* if ( configUSE_MUTEXES == 1 ) */

                    vTaskPlaceOnEventList( &( pxQueue->xTasksWaitingToReceive ), xTicksToWait );
                    prvUnlockQueue( pxQueue );

                    सचे( xTaskResumeAll() == pdFALSE )
                    {
                        portYIELD_WITHIN_API();
                    }
                    अञ्ञथा
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                अञ्ञथा
                {
                    /* There was no timeout and the semaphore count was not 0, so
                     * attempt to take the semaphore again. */
                    prvUnlockQueue( pxQueue );
                    ( शून्य ) xTaskResumeAll();
                }
            }
            अञ्ञथा
            {
                /* Timed out. */
                prvUnlockQueue( pxQueue );
                ( शून्य ) xTaskResumeAll();

                /* If the semaphore count is 0 exit now as the timeout has
                 * expired.  Otherwise return to attempt to take the semaphore that is
                 * known to be available.  As semaphores are implemented by queues the
                 * queue being empty is equivalent to the semaphore count being 0. */
                सचे( prvIsQueueEmpty( pxQueue ) != pdFALSE )
                {
                    #सचे ( configUSE_MUTEXES == 1 )
                    {
                        /* xInheritanceOccurred could only have be set if
                         * pxQueue->uxQueueType == queueQUEUE_IS_MUTEX so no need to
                         * test the mutex type again to check it is actually a mutex. */
                        सचे( xInheritanceOccurred != pdFALSE )
                        {
                            taskENTER_CRITICAL( &( pxQueue->xQueueLock ) );
                            {
                                UBaseType_t uxHighestWaitingPriority;

                                /* This task blocking on the mutex caused another
                                 * task to inherit this task's priority.  Now this task
                                 * has timed out the priority should be disinherited
                                 * again, but only as low as the next highest priority
                                 * task that is waiting for the same mutex. */
                                uxHighestWaitingPriority = prvGetDisinheritPriorityAfterTimeout( pxQueue );
                                vTaskPriorityDisinheritAfterTimeout( pxQueue->u.xSemaphore.xMutexHolder, uxHighestWaitingPriority );
                            }
                            taskEXIT_CRITICAL( &( pxQueue->xQueueLock ) );
                        }
                    }
                    #endif /* configUSE_MUTEXES */

                    traceQUEUE_RECEIVE_FAILED( pxQueue );
                    पटिनिवत्त errQUEUE_EMPTY;
                }
                अञ्ञथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
        }
        #endif /* queueUSE_LOCKS == 1 */
    } /*lint -restore */
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

    /* Check the pointer is not NULL. */
    configASSERT( ( pxQueue ) );

    /* The buffer into which data is received can only be NULL if the data size
     * is zero (so no data is copied into the buffer. */
    configASSERT( !( ( ( pvBuffer ) == रिक्त ) && ( ( pxQueue )->uxItemSize != ( UBaseType_t ) 0U ) ) );

    /* Cannot block if the scheduler is suspended. */
    #सचे ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( xTicksToWait != 0 ) ) );
    }
    #endif

    /*lint -save -e904  This function relaxes the coding standard somewhat to
     * allow return statements within the function itself.  This is done in the
     * interest of execution time efficiency. */
    अत्थाय( ; ; )
    {
        taskENTER_CRITICAL( &( pxQueue->xQueueLock ) );
        {
            ध्रुव UBaseType_t uxMessagesWaiting = pxQueue->uxMessagesWaiting;

            /* Is there data in the queue now?  To be running the calling task
             * must be the highest priority task wanting to access the queue. */
            सचे( uxMessagesWaiting > ( UBaseType_t ) 0 )
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
                सचे( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToReceive ) ) == pdFALSE )
                {
                    सचे( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToReceive ) ) != pdFALSE )
                    {
                        /* The task waiting has a higher priority than this task. */
                        queueYIELD_IF_USING_PREEMPTION();
                    }
                    अञ्ञथा
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                अञ्ञथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                taskEXIT_CRITICAL( &( pxQueue->xQueueLock ) );
                पटिनिवत्त pdPASS;
            }
            अञ्ञथा
            {
                सचे( xTicksToWait == ( TickType_t ) 0 )
                {
                    /* The queue was empty and no block time is specified (or
                     * the block time has expired) so leave now. */
                    taskEXIT_CRITICAL( &( pxQueue->xQueueLock ) );
                    traceQUEUE_PEEK_FAILED( pxQueue );
                    पटिनिवत्त errQUEUE_EMPTY;
                }
                अञ्ञथा सचे( xEntryTimeSet == pdFALSE )
                {
                    /* The queue was empty and a block time was specified so
                     * configure the timeout structure ready to enter the blocked
                     * state. */
                    vTaskInternalSetTimeOutState( &xTimeOut );
                    xEntryTimeSet = pdTRUE;
                }
                अञ्ञथा
                {
                    /* Entry time was already set. */
                    mtCOVERAGE_TEST_MARKER();
                }
            }

            /* If queue locks ARE NOT being used:
             * - At this point, the queue is empty and entry time has been set
             * - We simply check for a time out, block if not timed out, or
             *   return an error if we have timed out. */
            #सचे ( queueUSE_LOCKS == 0 )
            {
                /* Update the timeout state to see if it has expired yet. */
                सचे( xTaskCheckForTimeOut( &xTimeOut, &xTicksToWait ) == pdFALSE )
                {
                    /* Not timed out yet. Block the current task. */
                    traceBLOCKING_ON_QUEUE_PEEK( pxQueue );
                    vTaskPlaceOnEventList( &( pxQueue->xTasksWaitingToReceive ), xTicksToWait );
                    portYIELD_WITHIN_API();
                }
                अञ्ञथा
                {
                    /* We have timed out. Return an error. */
                    taskEXIT_CRITICAL( &( pxQueue->xQueueLock ) );
                    traceQUEUE_PEEK_FAILED( pxQueue );
                    पटिनिवत्त errQUEUE_EMPTY;
                }
            }
            #endif /* queueUSE_LOCKS == 0 */
        }
        taskEXIT_CRITICAL( &( pxQueue->xQueueLock ) );

        /* If queue locks ARE being used:
         * - At this point, the queue is empty and entry time has been set
         * - We follow the original procedure for locking the queue before
         *   attempting to block. */
        #सचे ( queueUSE_LOCKS == 1 )
        {
            /* Interrupts and other tasks can send to and receive from the queue
             * now that the critical section has been exited. */

            vTaskSuspendAll();
            prvLockQueue( pxQueue );

            /* Update the timeout state to see if it has expired yet. */
            सचे( xTaskCheckForTimeOut( &xTimeOut, &xTicksToWait ) == pdFALSE )
            {
                /* Timeout has not expired yet, check to see if there is data in the
                * queue now, and if not enter the Blocked state to wait for data. */
                सचे( prvIsQueueEmpty( pxQueue ) != pdFALSE )
                {
                    traceBLOCKING_ON_QUEUE_PEEK( pxQueue );
                    vTaskPlaceOnEventList( &( pxQueue->xTasksWaitingToReceive ), xTicksToWait );
                    prvUnlockQueue( pxQueue );

                    सचे( xTaskResumeAll() == pdFALSE )
                    {
                        portYIELD_WITHIN_API();
                    }
                    अञ्ञथा
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                अञ्ञथा
                {
                    /* There is data in the queue now, so don't enter the blocked
                     * state, instead return to try and obtain the data. */
                    prvUnlockQueue( pxQueue );
                    ( शून्य ) xTaskResumeAll();
                }
            }
            अञ्ञथा
            {
                /* The timeout has expired.  If there is still no data in the queue
                 * exit, otherwise go back and try to read the data again. */
                prvUnlockQueue( pxQueue );
                ( शून्य ) xTaskResumeAll();

                सचे( prvIsQueueEmpty( pxQueue ) != pdFALSE )
                {
                    traceQUEUE_PEEK_FAILED( pxQueue );
                    पटिनिवत्त errQUEUE_EMPTY;
                }
                अञ्ञथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
        }
        #endif /* queueUSE_LOCKS == 1 */
    } /*lint -restore */
}
/*-----------------------------------------------------------*/

BaseType_t xQueueReceiveFromISR( QueueHandle_t xQueue,
                                 शून्य * ध्रुव pvBuffer,
                                 BaseType_t * ध्रुव pxHigherPriorityTaskWoken )
{
    BaseType_t xReturn;
    UBaseType_t uxSavedInterruptStatus;
    Queue_t * ध्रुव pxQueue = xQueue;

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

    prvENTER_CRITICAL_OR_MASK_ISR( &( pxQueue->xQueueLock ), uxSavedInterruptStatus );
    {
        ध्रुव UBaseType_t uxMessagesWaiting = pxQueue->uxMessagesWaiting;

        /* Cannot block in an ISR, so check there is data available. */
        सचे( uxMessagesWaiting > ( UBaseType_t ) 0 )
        {
            #सचे ( queueUSE_LOCKS == 1 )
                ध्रुव int8_t cRxLock = pxQueue->cRxLock;
            #अञ्ञथा
                /* Queue locks not used, so we treat it as unlocked. */
                ध्रुव int8_t cRxLock = queueUNLOCKED;
            #endif /* queueUSE_LOCKS == 1 */

            traceQUEUE_RECEIVE_FROM_ISR( pxQueue );

            prvCopyDataFromQueue( pxQueue, pvBuffer );
            pxQueue->uxMessagesWaiting = uxMessagesWaiting - ( UBaseType_t ) 1;

            /* If the queue is locked the event list will not be modified.
             * Instead update the lock count so the task that unlocks the queue
             * will know that an ISR has removed data while the queue was
             * locked. */
            सचे( cRxLock == queueUNLOCKED )
            {
                सचे( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToSend ) ) == pdFALSE )
                {
                    सचे( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToSend ) ) != pdFALSE )
                    {
                        /* The task waiting has a higher priority than us so
                         * force a context switch. */
                        सचे( pxHigherPriorityTaskWoken != रिक्त )
                        {
                            *pxHigherPriorityTaskWoken = pdTRUE;
                        }
                        अञ्ञथा
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    अञ्ञथा
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                अञ्ञथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            अञ्ञथा
            {
                #सचे ( queueUSE_LOCKS == 1 )
                {
                    /* Increment the lock count so the task that unlocks the queue
                     * knows that data was removed while it was locked. */
                    prvIncrementQueueRxLock( pxQueue, cRxLock );
                }
                #endif /* queueUSE_LOCKS == 1 */
            }

            xReturn = pdPASS;
        }
        अञ्ञथा
        {
            xReturn = pdFAIL;
            traceQUEUE_RECEIVE_FROM_ISR_FAILED( pxQueue );
        }
    }
    prvEXIT_CRITICAL_OR_UNMASK_ISR( &( pxQueue->xQueueLock ), uxSavedInterruptStatus );

    पटिनिवत्त xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t xQueuePeekFromISR( QueueHandle_t xQueue,
                              शून्य * ध्रुव pvBuffer )
{
    BaseType_t xReturn;
    UBaseType_t uxSavedInterruptStatus;
    int8_t * pcOriginalReadPosition;
    Queue_t * ध्रुव pxQueue = xQueue;

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

    prvENTER_CRITICAL_OR_MASK_ISR( &( pxQueue->xQueueLock ), uxSavedInterruptStatus );
    {
        /* Cannot block in an ISR, so check there is data available. */
        सचे( pxQueue->uxMessagesWaiting > ( UBaseType_t ) 0 )
        {
            traceQUEUE_PEEK_FROM_ISR( pxQueue );

            /* Remember the read position so it can be reset as nothing is
             * actually being removed from the queue. */
            pcOriginalReadPosition = pxQueue->u.xQueue.pcReadFrom;
            prvCopyDataFromQueue( pxQueue, pvBuffer );
            pxQueue->u.xQueue.pcReadFrom = pcOriginalReadPosition;

            xReturn = pdPASS;
        }
        अञ्ञथा
        {
            xReturn = pdFAIL;
            traceQUEUE_PEEK_FROM_ISR_FAILED( pxQueue );
        }
    }
    prvEXIT_CRITICAL_OR_UNMASK_ISR( &( pxQueue->xQueueLock ), uxSavedInterruptStatus );

    पटिनिवत्त xReturn;
}
/*-----------------------------------------------------------*/

UBaseType_t uxQueueMessagesWaiting( ध्रुव QueueHandle_t xQueue )
{
    UBaseType_t uxReturn;

    configASSERT( xQueue );

    taskENTER_CRITICAL( &( ( ( Queue_t * ) xQueue )->xQueueLock ) );
    {
        uxReturn = ( ( Queue_t * ) xQueue )->uxMessagesWaiting;
    }
    taskEXIT_CRITICAL( &( ( ( Queue_t * ) xQueue )->xQueueLock ) );

    पटिनिवत्त uxReturn;
} /*lint !e818 Pointer cannot be declared const as xQueue is a typedef not pointer. */
/*-----------------------------------------------------------*/

UBaseType_t uxQueueSpacesAvailable( ध्रुव QueueHandle_t xQueue )
{
    UBaseType_t uxReturn;
    Queue_t * ध्रुव pxQueue = xQueue;

    configASSERT( pxQueue );

    taskENTER_CRITICAL( &( pxQueue->xQueueLock ) );
    {
        uxReturn = pxQueue->uxLength - pxQueue->uxMessagesWaiting;
    }
    taskEXIT_CRITICAL( &( pxQueue->xQueueLock ) );

    पटिनिवत्त uxReturn;
} /*lint !e818 Pointer cannot be declared const as xQueue is a typedef not pointer. */
/*-----------------------------------------------------------*/

UBaseType_t uxQueueMessagesWaitingFromISR( ध्रुव QueueHandle_t xQueue )
{
    UBaseType_t uxReturn;
    Queue_t * ध्रुव pxQueue = xQueue;

    configASSERT( pxQueue );
    uxReturn = pxQueue->uxMessagesWaiting;

    पटिनिवत्त uxReturn;
} /*lint !e818 Pointer cannot be declared const as xQueue is a typedef not pointer. */
/*-----------------------------------------------------------*/

शून्य vQueueDelete( QueueHandle_t xQueue )
{
    Queue_t * ध्रुव pxQueue = xQueue;

    configASSERT( pxQueue );
    traceQUEUE_DELETE( pxQueue );

    #सचे ( configQUEUE_REGISTRY_SIZE > 0 )
    {
        vQueueUnregisterQueue( pxQueue );
    }
    #endif

    #सचे ( ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) && ( configSUPPORT_STATIC_ALLOCATION == 0 ) )
    {
        /* The queue can only have been allocated dynamically - free it
         * again. */
        vPortFree( pxQueue );
    }
    #नोचेद्यदि ( ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) && ( configSUPPORT_STATIC_ALLOCATION == 1 ) )
    {
        /* The queue could have been allocated statically or dynamically, so
         * check before attempting to free the memory. */
        सचे( pxQueue->ucStaticallyAllocated == ( uint8_t ) pdFALSE )
        {
            vPortFree( pxQueue );
        }
        अञ्ञथा
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    #अञ्ञथा /* if ( ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) && ( configSUPPORT_STATIC_ALLOCATION == 0 ) ) */
    {
        /* The queue must have been statically allocated, so is not going to be
         * deleted.  Avoid compiler warnings about the unused parameter. */
        ( शून्य ) pxQueue;
    }
    #endif /* configSUPPORT_DYNAMIC_ALLOCATION */
}
/*-----------------------------------------------------------*/

#सचे ( configUSE_TRACE_FACILITY == 1 )

    UBaseType_t ux_Queue_Get_Queue_सङ्ख्या( QueueHandle_t xQueue )
    {
        पटिनिवत्त ( ( Queue_t * ) xQueue )->ux_Queue_सङ्ख्या;
    }

#endif /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#सचे ( configUSE_TRACE_FACILITY == 1 )

    शून्य v_Queue_Set_Queue_सङ्ख्या( QueueHandle_t xQueue,
                               UBaseType_t ux_Queue_सङ्ख्या )
    {
        ( ( Queue_t * ) xQueue )->ux_Queue_सङ्ख्या = ux_Queue_सङ्ख्या;
    }

#endif /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#सचे ( configUSE_TRACE_FACILITY == 1 )

    uint8_t ucQueueGetQueueType( QueueHandle_t xQueue )
    {
        पटिनिवत्त ( ( Queue_t * ) xQueue )->ucQueueType;
    }

#endif /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#सचे ( configUSE_MUTEXES == 1 )

    स्थिर UBaseType_t prvGetDisinheritPriorityAfterTimeout( ध्रुव Queue_t * ध्रुव pxQueue )
    {
        UBaseType_t uxHighestPriorityOfWaitingTasks;

        /* If a task waiting for a mutex causes the mutex holder to inherit a
         * priority, but the waiting task times out, then the holder should
         * disinherit the priority - but only down to the highest priority of any
         * other tasks that are waiting for the same mutex.  For this purpose,
         * return the priority of the highest priority task that is waiting for the
         * mutex. */
        सचे( listCURRENT_LIST_LENGTH( &( pxQueue->xTasksWaitingToReceive ) ) > 0U )
        {
            uxHighestPriorityOfWaitingTasks = ( UBaseType_t ) configMAX_PRIORITIES - ( UBaseType_t ) list_GET_ITEM_मानम्_OF_HEAD_ENTRY( &( pxQueue->xTasksWaitingToReceive ) );
        }
        अञ्ञथा
        {
            uxHighestPriorityOfWaitingTasks = tskIDLE_PRIORITY;
        }

        पटिनिवत्त uxHighestPriorityOfWaitingTasks;
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

    सचे( pxQueue->uxItemSize == ( UBaseType_t ) 0 )
    {
        #सचे ( configUSE_MUTEXES == 1 )
        {
            सचे( pxQueue->uxQueueType == queueQUEUE_IS_MUTEX )
            {
                /* The mutex is no longer being held. */
                xReturn = xTaskPriorityDisinherit( pxQueue->u.xSemaphore.xMutexHolder );
                pxQueue->u.xSemaphore.xMutexHolder = रिक्त;
            }
            अञ्ञथा
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        #endif /* configUSE_MUTEXES */
    }
    अञ्ञथा सचे( xPosition == queueSEND_TO_BACK )
    {
        ( शून्य ) memcpy( ( शून्य * ) pxQueue->pcWriteTo, pvItemToQueue, ( size_t ) pxQueue->uxItemSize ); /*lint !e961 !e418 !e9087 MISRA exception as the casts are only redundant for some ports, plus previous logic ensures a null pointer can only be passed to memcpy() if the copy size is 0.  Cast to void required by function signature and safe as no alignment requirement and copy length specified in bytes. */
        pxQueue->pcWriteTo += pxQueue->uxItemSize;                                                       /*lint !e9016 Pointer arithmetic on char types ok, especially in this use case where it is the clearest way of conveying intent. */

        सचे( pxQueue->pcWriteTo >= pxQueue->u.xQueue.pcTail )                                             /*lint !e946 MISRA exception justified as comparison of pointers is the cleanest solution. */
        {
            pxQueue->pcWriteTo = pxQueue->pcHead;
        }
        अञ्ञथा
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    अञ्ञथा
    {
        ( शून्य ) memcpy( ( शून्य * ) pxQueue->u.xQueue.pcReadFrom, pvItemToQueue, ( size_t ) pxQueue->uxItemSize ); /*lint !e961 !e9087 !e418 MISRA exception as the casts are only redundant for some ports.  Cast to void required by function signature and safe as no alignment requirement and copy length specified in bytes.  Assert checks null pointer only used when length is 0. */
        pxQueue->u.xQueue.pcReadFrom -= pxQueue->uxItemSize;

        सचे( pxQueue->u.xQueue.pcReadFrom < pxQueue->pcHead ) /*lint !e946 MISRA exception justified as comparison of pointers is the cleanest solution. */
        {
            pxQueue->u.xQueue.pcReadFrom = ( pxQueue->u.xQueue.pcTail - pxQueue->uxItemSize );
        }
        अञ्ञथा
        {
            mtCOVERAGE_TEST_MARKER();
        }

        सचे( xPosition == queueOVERWRITE )
        {
            सचे( uxMessagesWaiting > ( UBaseType_t ) 0 )
            {
                /* An item is not being added but overwritten, so subtract
                 * one from the recorded number of items in the queue so when
                 * one is added again below the number of recorded items remains
                 * correct. */
                --uxMessagesWaiting;
            }
            अञ्ञथा
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        अञ्ञथा
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }

    pxQueue->uxMessagesWaiting = uxMessagesWaiting + ( UBaseType_t ) 1;

    पटिनिवत्त xReturn;
}
/*-----------------------------------------------------------*/

स्थिर शून्य prvCopyDataFromQueue( Queue_t * ध्रुव pxQueue,
                                  शून्य * ध्रुव pvBuffer )
{
    सचे( pxQueue->uxItemSize != ( UBaseType_t ) 0 )
    {
        pxQueue->u.xQueue.pcReadFrom += pxQueue->uxItemSize;           /*lint !e9016 Pointer arithmetic on char types ok, especially in this use case where it is the clearest way of conveying intent. */

        सचे( pxQueue->u.xQueue.pcReadFrom >= pxQueue->u.xQueue.pcTail ) /*lint !e946 MISRA exception justified as use of the relational operator is the cleanest solutions. */
        {
            pxQueue->u.xQueue.pcReadFrom = pxQueue->pcHead;
        }
        अञ्ञथा
        {
            mtCOVERAGE_TEST_MARKER();
        }

        ( शून्य ) memcpy( ( शून्य * ) pvBuffer, ( शून्य * ) pxQueue->u.xQueue.pcReadFrom, ( size_t ) pxQueue->uxItemSize ); /*lint !e961 !e418 !e9087 MISRA exception as the casts are only redundant for some ports.  Also previous logic ensures a null pointer can only be passed to memcpy() when the count is 0.  Cast to void required by function signature and safe as no alignment requirement and copy length specified in bytes. */
    }
}
/*-----------------------------------------------------------*/

#सचे ( queueUSE_LOCKS == 1 )
    स्थिर शून्य prvUnlockQueue( Queue_t * ध्रुव pxQueue )
    {
        /* THIS FUNCTION MUST BE CALLED WITH THE SCHEDULER SUSPENDED. */

        /* The lock counts contains the number of extra data items placed or
         * removed from the queue while the queue was locked.  When a queue is
         * locked items can be added or removed, but the event lists cannot be
         * updated. */
        taskENTER_CRITICAL( &( pxQueue->xQueueLock ) );
        {
            int8_t cTxLock = pxQueue->cTxLock;

            /* See if data was added to the queue while it was locked. */
            याव( cTxLock > queueLOCKED_UNMODIFIED )
            {
                /* Data was posted while the queue was locked.  Are any tasks
                 * blocked waiting for data to become available? */
                #सचे ( configUSE_QUEUE_SETS == 1 )
                {
                    सचे( pxQueue->pxQueueSetContainer != रिक्त )
                    {
                        सचे( prvNotifyQueueSetContainer( pxQueue ) != pdFALSE )
                        {
                            /* The queue is a member of a queue set, and posting to
                             * the queue set caused a higher priority task to unblock.
                             * A context switch is required. */
                            vTaskMissedYield();
                        }
                        अञ्ञथा
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    अञ्ञथा
                    {
                        /* Tasks that are removed from the event list will get
                         * added to the pending ready list as the scheduler is still
                         * suspended. */
                        सचे( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToReceive ) ) == pdFALSE )
                        {
                            सचे( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToReceive ) ) != pdFALSE )
                            {
                                /* The task waiting has a higher priority so record that a
                                 * context switch is required. */
                                vTaskMissedYield();
                            }
                            अञ्ञथा
                            {
                                mtCOVERAGE_TEST_MARKER();
                            }
                        }
                        अञ्ञथा
                        {
                            भङ्ग;
                        }
                    }
                }
                #अञ्ञथा /* configUSE_QUEUE_SETS */
                {
                    /* Tasks that are removed from the event list will get added to
                     * the pending ready list as the scheduler is still suspended. */
                    सचे( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToReceive ) ) == pdFALSE )
                    {
                        सचे( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToReceive ) ) != pdFALSE )
                        {
                            /* The task waiting has a higher priority so record that
                             * a context switch is required. */
                            vTaskMissedYield();
                        }
                        अञ्ञथा
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    अञ्ञथा
                    {
                        भङ्ग;
                    }
                }
                #endif /* configUSE_QUEUE_SETS */

                --cTxLock;
            }

            pxQueue->cTxLock = queueUNLOCKED;
        }
        taskEXIT_CRITICAL( &( pxQueue->xQueueLock ) );

        /* Do the same for the Rx lock. */
        taskENTER_CRITICAL( &( pxQueue->xQueueLock ) );
        {
            int8_t cRxLock = pxQueue->cRxLock;

            याव( cRxLock > queueLOCKED_UNMODIFIED )
            {
                सचे( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToSend ) ) == pdFALSE )
                {
                    सचे( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToSend ) ) != pdFALSE )
                    {
                        vTaskMissedYield();
                    }
                    अञ्ञथा
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }

                    --cRxLock;
                }
                अञ्ञथा
                {
                    भङ्ग;
                }
            }

            pxQueue->cRxLock = queueUNLOCKED;
        }
        taskEXIT_CRITICAL( &( pxQueue->xQueueLock ) );
    }
#endif /* queueUSE_LOCKS == 1 */
/*-----------------------------------------------------------*/

#सचे ( queueUSE_LOCKS == 1 )
    स्थिर BaseType_t prvIsQueueEmpty( ध्रुव Queue_t * pxQueue )
    {
        BaseType_t xReturn;

        taskENTER_CRITICAL( &( ( ( Queue_t * ) pxQueue )->xQueueLock ) );
        {
            सचे( pxQueue->uxMessagesWaiting == ( UBaseType_t ) 0 )
            {
                xReturn = pdTRUE;
            }
            अञ्ञथा
            {
                xReturn = pdFALSE;
            }
        }
        taskEXIT_CRITICAL( &( ( ( Queue_t * ) pxQueue )->xQueueLock ) );

        पटिनिवत्त xReturn;
    }
#endif /* queueUSE_LOCKS == 1 */
/*-----------------------------------------------------------*/

BaseType_t xQueueIsQueueEmptyFromISR( ध्रुव QueueHandle_t xQueue )
{
    BaseType_t xReturn;
    Queue_t * ध्रुव pxQueue = xQueue;

    configASSERT( pxQueue );

    सचे( pxQueue->uxMessagesWaiting == ( UBaseType_t ) 0 )
    {
        xReturn = pdTRUE;
    }
    अञ्ञथा
    {
        xReturn = pdFALSE;
    }

    पटिनिवत्त xReturn;
} /*lint !e818 xQueue could not be pointer to const because it is a typedef. */
/*-----------------------------------------------------------*/

#सचे ( queueUSE_LOCKS == 1 )
    स्थिर BaseType_t prvIsQueueFull( ध्रुव Queue_t * pxQueue )
    {
        BaseType_t xReturn;

        taskENTER_CRITICAL( &( ( ( Queue_t * ) pxQueue )->xQueueLock ) );
        {
            सचे( pxQueue->uxMessagesWaiting == pxQueue->uxLength )
            {
                xReturn = pdTRUE;
            }
            अञ्ञथा
            {
                xReturn = pdFALSE;
            }
        }
        taskEXIT_CRITICAL( &( ( ( Queue_t * ) pxQueue )->xQueueLock ) );

        पटिनिवत्त xReturn;
    }
#endif /* queueUSE_LOCKS == 1 */
/*-----------------------------------------------------------*/

BaseType_t xQueueIsQueueFullFromISR( ध्रुव QueueHandle_t xQueue )
{
    BaseType_t xReturn;
    Queue_t * ध्रुव pxQueue = xQueue;

    configASSERT( pxQueue );

    सचे( pxQueue->uxMessagesWaiting == pxQueue->uxLength )
    {
        xReturn = pdTRUE;
    }
    अञ्ञथा
    {
        xReturn = pdFALSE;
    }

    पटिनिवत्त xReturn;
} /*lint !e818 xQueue could not be pointer to const because it is a typedef. */
/*-----------------------------------------------------------*/

#सचे ( configUSE_CO_ROUTINES == 1 )

    BaseType_t xQueueCRSend( QueueHandle_t xQueue,
                             ध्रुव शून्य * pvItemToQueue,
                             TickType_t xTicksToWait )
    {
        BaseType_t xReturn;
        Queue_t * ध्रुव pxQueue = xQueue;

        /* If the queue is already full we may have to block.  A critical section
         * is required to prevent an interrupt removing something from the queue
         * between the check to see if the queue is full and blocking on the queue. */
        portDISABLE_INTERRUPTS();
        {
            सचे( prvIsQueueFull( pxQueue ) != pdFALSE )
            {
                /* The queue is full - do we want to block or just leave without
                 * posting? */
                सचे( xTicksToWait > ( TickType_t ) 0 )
                {
                    /* As this is called from a coroutine we cannot block directly, but
                     * return indicating that we need to block. */
                    vCoRoutineAddToDelayedList( xTicksToWait, &( pxQueue->xTasksWaitingToSend ) );
                    portENABLE_INTERRUPTS();
                    पटिनिवत्त errQUEUE_BLOCKED;
                }
                अञ्ञथा
                {
                    portENABLE_INTERRUPTS();
                    पटिनिवत्त errQUEUE_FULL;
                }
            }
        }
        portENABLE_INTERRUPTS();

        portDISABLE_INTERRUPTS();
        {
            सचे( pxQueue->uxMessagesWaiting < pxQueue->uxLength )
            {
                /* There is room in the queue, copy the data into the queue. */
                prvCopyDataToQueue( pxQueue, pvItemToQueue, queueSEND_TO_BACK );
                xReturn = pdPASS;

                /* Were any co-routines waiting for data to become available? */
                सचे( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToReceive ) ) == pdFALSE )
                {
                    /* In this instance the co-routine could be placed directly
                     * into the ready list as we are within a critical section.
                     * Instead the same pending ready list mechanism is used as if
                     * the event were caused from within an interrupt. */
                    सचे( xCoRoutineRemoveFromEventList( &( pxQueue->xTasksWaitingToReceive ) ) != pdFALSE )
                    {
                        /* The co-routine waiting has a higher priority so record
                         * that a yield might be appropriate. */
                        xReturn = errQUEUE_YIELD;
                    }
                    अञ्ञथा
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                अञ्ञथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            अञ्ञथा
            {
                xReturn = errQUEUE_FULL;
            }
        }
        portENABLE_INTERRUPTS();

        पटिनिवत्त xReturn;
    }

#endif /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#सचे ( configUSE_CO_ROUTINES == 1 )

    BaseType_t xQueueCRReceive( QueueHandle_t xQueue,
                                शून्य * pvBuffer,
                                TickType_t xTicksToWait )
    {
        BaseType_t xReturn;
        Queue_t * ध्रुव pxQueue = xQueue;

        /* If the queue is already empty we may have to block.  A critical section
         * is required to prevent an interrupt adding something to the queue
         * between the check to see if the queue is empty and blocking on the queue. */
        portDISABLE_INTERRUPTS();
        {
            सचे( pxQueue->uxMessagesWaiting == ( UBaseType_t ) 0 )
            {
                /* There are no messages in the queue, do we want to block or just
                 * leave with nothing? */
                सचे( xTicksToWait > ( TickType_t ) 0 )
                {
                    /* As this is a co-routine we cannot block directly, but return
                     * indicating that we need to block. */
                    vCoRoutineAddToDelayedList( xTicksToWait, &( pxQueue->xTasksWaitingToReceive ) );
                    portENABLE_INTERRUPTS();
                    पटिनिवत्त errQUEUE_BLOCKED;
                }
                अञ्ञथा
                {
                    portENABLE_INTERRUPTS();
                    पटिनिवत्त errQUEUE_FULL;
                }
            }
            अञ्ञथा
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        portENABLE_INTERRUPTS();

        portDISABLE_INTERRUPTS();
        {
            सचे( pxQueue->uxMessagesWaiting > ( UBaseType_t ) 0 )
            {
                /* Data is available from the queue. */
                pxQueue->u.xQueue.pcReadFrom += pxQueue->uxItemSize;

                सचे( pxQueue->u.xQueue.pcReadFrom >= pxQueue->u.xQueue.pcTail )
                {
                    pxQueue->u.xQueue.pcReadFrom = pxQueue->pcHead;
                }
                अञ्ञथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                --( pxQueue->uxMessagesWaiting );
                ( शून्य ) memcpy( ( शून्य * ) pvBuffer, ( शून्य * ) pxQueue->u.xQueue.pcReadFrom, ( unsigned ) pxQueue->uxItemSize );

                xReturn = pdPASS;

                /* Were any co-routines waiting for space to become available? */
                सचे( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToSend ) ) == pdFALSE )
                {
                    /* In this instance the co-routine could be placed directly
                     * into the ready list as we are within a critical section.
                     * Instead the same pending ready list mechanism is used as if
                     * the event were caused from within an interrupt. */
                    सचे( xCoRoutineRemoveFromEventList( &( pxQueue->xTasksWaitingToSend ) ) != pdFALSE )
                    {
                        xReturn = errQUEUE_YIELD;
                    }
                    अञ्ञथा
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                अञ्ञथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            अञ्ञथा
            {
                xReturn = pdFAIL;
            }
        }
        portENABLE_INTERRUPTS();

        पटिनिवत्त xReturn;
    }

#endif /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#सचे ( configUSE_CO_ROUTINES == 1 )

    BaseType_t xQueueCRSendFromISR( QueueHandle_t xQueue,
                                    ध्रुव शून्य * pvItemToQueue,
                                    BaseType_t xCoRoutinePreviouslyWoken )
    {
        Queue_t * ध्रुव pxQueue = xQueue;

        /* Cannot block within an ISR so if there is no space on the queue then
         * exit without doing anything. */
        सचे( pxQueue->uxMessagesWaiting < pxQueue->uxLength )
        {
            prvCopyDataToQueue( pxQueue, pvItemToQueue, queueSEND_TO_BACK );

            /* We only want to wake one co-routine per ISR, so check that a
             * co-routine has not already been woken. */
            सचे( xCoRoutinePreviouslyWoken == pdFALSE )
            {
                सचे( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToReceive ) ) == pdFALSE )
                {
                    सचे( xCoRoutineRemoveFromEventList( &( pxQueue->xTasksWaitingToReceive ) ) != pdFALSE )
                    {
                        पटिनिवत्त pdTRUE;
                    }
                    अञ्ञथा
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                अञ्ञथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            अञ्ञथा
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        अञ्ञथा
        {
            mtCOVERAGE_TEST_MARKER();
        }

        पटिनिवत्त xCoRoutinePreviouslyWoken;
    }

#endif /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#सचे ( configUSE_CO_ROUTINES == 1 )

    BaseType_t xQueueCRReceiveFromISR( QueueHandle_t xQueue,
                                       शून्य * pvBuffer,
                                       BaseType_t * pxCoRoutineWoken )
    {
        BaseType_t xReturn;
        Queue_t * ध्रुव pxQueue = xQueue;

        /* We cannot block from an ISR, so check there is data available. If
         * not then just leave without doing anything. */
        सचे( pxQueue->uxMessagesWaiting > ( UBaseType_t ) 0 )
        {
            /* Copy the data from the queue. */
            pxQueue->u.xQueue.pcReadFrom += pxQueue->uxItemSize;

            सचे( pxQueue->u.xQueue.pcReadFrom >= pxQueue->u.xQueue.pcTail )
            {
                pxQueue->u.xQueue.pcReadFrom = pxQueue->pcHead;
            }
            अञ्ञथा
            {
                mtCOVERAGE_TEST_MARKER();
            }

            --( pxQueue->uxMessagesWaiting );
            ( शून्य ) memcpy( ( शून्य * ) pvBuffer, ( शून्य * ) pxQueue->u.xQueue.pcReadFrom, ( unsigned ) pxQueue->uxItemSize );

            सचे( ( *pxCoRoutineWoken ) == pdFALSE )
            {
                सचे( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToSend ) ) == pdFALSE )
                {
                    सचे( xCoRoutineRemoveFromEventList( &( pxQueue->xTasksWaitingToSend ) ) != pdFALSE )
                    {
                        *pxCoRoutineWoken = pdTRUE;
                    }
                    अञ्ञथा
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                अञ्ञथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            अञ्ञथा
            {
                mtCOVERAGE_TEST_MARKER();
            }

            xReturn = pdPASS;
        }
        अञ्ञथा
        {
            xReturn = pdFAIL;
        }

        पटिनिवत्त xReturn;
    }

#endif /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#सचे ( configQUEUE_REGISTRY_SIZE > 0 )

    शून्य vQueueAddToRegistry( QueueHandle_t xQueue,
                              ध्रुव वर्ण * pcQueueName ) /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
    {
        UBaseType_t ux;
        QueueRegistryItem_t * pxEntryToWrite = रिक्त;

        configASSERT( xQueue );

        /* For SMP, we need to take the queue registry lock in case another
         * core updates the register simultaneously. */
        prvENTER_CRITICAL_SMP_ONLY( &xQueueRegistryLock );
        {
            सचे( pcQueueName != रिक्त )
            {
                /* See if there is an empty space in the registry.  A NULL name denotes
                 * a free slot. */
                अत्थाय( ux = ( UBaseType_t ) 0U; ux < ( UBaseType_t ) configQUEUE_REGISTRY_SIZE; ux++ )
                {
                    /* Replace an existing entry if the queue is already in the registry. */
                    सचे( xQueue == xQueueRegistry[ ux ].xHandle )
                    {
                        pxEntryToWrite = &( xQueueRegistry[ ux ] );
                        भङ्ग;
                    }
                    /* Otherwise, store in the next empty location */
                    अञ्ञथा सचे( ( pxEntryToWrite == रिक्त ) && ( xQueueRegistry[ ux ].pcQueueName == रिक्त ) )
                    {
                        pxEntryToWrite = &( xQueueRegistry[ ux ] );
                    }
                    अञ्ञथा
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
            }

            सचे( pxEntryToWrite != रिक्त )
            {
                /* Store the information on this queue. */
                pxEntryToWrite->pcQueueName = pcQueueName;
                pxEntryToWrite->xHandle = xQueue;

                traceQUEUE_REGISTRY_ADD( xQueue, pcQueueName );
            }
        }
        /* Release the previously taken queue registry lock. */
        prvEXIT_CRITICAL_SMP_ONLY( &xQueueRegistryLock );
    }

#endif /* configQUEUE_REGISTRY_SIZE */
/*-----------------------------------------------------------*/

#सचे ( configQUEUE_REGISTRY_SIZE > 0 )

    ध्रुव वर्ण * pcQueueGetName( QueueHandle_t xQueue ) /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
    {
        UBaseType_t ux;
        ध्रुव वर्ण * pcReturn = रिक्त; /*lint !e971 Unqualified char types are allowed for strings and single characters only. */

        configASSERT( xQueue );

        /* For SMP, we need to take the queue registry lock in case another
         * core updates the register simultaneously. */
        prvENTER_CRITICAL_SMP_ONLY( &xQueueRegistryLock );
        {
            /* Note there is nothing here to protect against another task adding or
             * removing entries from the registry while it is being searched. */

            अत्थाय( ux = ( UBaseType_t ) 0U; ux < ( UBaseType_t ) configQUEUE_REGISTRY_SIZE; ux++ )
            {
                सचे( xQueueRegistry[ ux ].xHandle == xQueue )
                {
                    pcReturn = xQueueRegistry[ ux ].pcQueueName;
                    भङ्ग;
                }
                अञ्ञथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
        }
        /* Release the previously taken queue registry lock. */
        prvEXIT_CRITICAL_SMP_ONLY( &xQueueRegistryLock );

        पटिनिवत्त pcReturn;
    } /*lint !e818 xQueue cannot be a pointer to const because it is a typedef. */

#endif /* configQUEUE_REGISTRY_SIZE */
/*-----------------------------------------------------------*/

#सचे ( configQUEUE_REGISTRY_SIZE > 0 )

    शून्य vQueueUnregisterQueue( QueueHandle_t xQueue )
    {
        UBaseType_t ux;

        configASSERT( xQueue );

        /* For SMP, we need to take the queue registry lock in case another
         * core updates the register simultaneously. */
        prvENTER_CRITICAL_SMP_ONLY( &xQueueRegistryLock );
        {
            /* See if the handle of the queue being unregistered in actually in the
             * registry. */
            अत्थाय( ux = ( UBaseType_t ) 0U; ux < ( UBaseType_t ) configQUEUE_REGISTRY_SIZE; ux++ )
            {
                सचे( xQueueRegistry[ ux ].xHandle == xQueue )
                {
                    /* Set the name to NULL to show that this slot if free again. */
                    xQueueRegistry[ ux ].pcQueueName = रिक्त;

                    /* Set the handle to NULL to ensure the same queue handle cannot
                     * appear in the registry twice if it is added, removed, then
                     * added again. */
                    xQueueRegistry[ ux ].xHandle = ( QueueHandle_t ) 0;
                    भङ्ग;
                }
                अञ्ञथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
        }
        /* Release the previously taken queue registry lock. */
        prvEXIT_CRITICAL_SMP_ONLY( &xQueueRegistryLock );
    } /*lint !e818 xQueue could not be pointer to const because it is a typedef. */

#endif /* configQUEUE_REGISTRY_SIZE */
/*-----------------------------------------------------------*/

#सचे ( configUSE_TIMERS == 1 )

    शून्य vQueueWaitForMessageRestricted( QueueHandle_t xQueue,
                                         TickType_t xTicksToWait,
                                         ध्रुव BaseType_t xWaitIndefinitely )
    {
        Queue_t * ध्रुव pxQueue = xQueue;

        /* This function should not be called by application code hence the
         * 'Restricted' in its name.  It is not part of the public API.  It is
         * designed for use by kernel code, and has special calling requirements.
         * It can result in vListInsert() being called on a list that can only
         * possibly ever have one item in it, so the list will be fast, but even
         * so it should be called with the scheduler locked and not from a critical
         * section. */

        /* For SMP, we need to take the queue's xQueueLock as we are about to
         * access the queue. */
        prvENTER_CRITICAL_SMP_ONLY( &( pxQueue->xQueueLock ) );
        {
            #सचे ( queueUSE_LOCKS == 1 )
            {
                /* Only do anything if there are no messages in the queue.  This function
                 *  will not actually cause the task to block, just place it on a blocked
                 *  list.  It will not block until the scheduler is unlocked - at which
                 *  time a yield will be performed.  If an item is added to the queue while
                 *  the queue is locked, and the calling task blocks on the queue, then the
                 *  calling task will be immediately unblocked when the queue is unlocked. */
                prvLockQueue( pxQueue );
            }
            #endif /* queueUSE_LOCKS == 1 */

            सचे( pxQueue->uxMessagesWaiting == ( UBaseType_t ) 0U )
            {
                /* There is nothing in the queue, block for the specified period. */
                vTaskPlaceOnEventListRestricted( &( pxQueue->xTasksWaitingToReceive ), xTicksToWait, xWaitIndefinitely );
            }
            अञ्ञथा
            {
                mtCOVERAGE_TEST_MARKER();
            }

            #सचे ( queueUSE_LOCKS == 1 )
            {
                prvUnlockQueue( pxQueue );
            }
            #endif /* queueUSE_LOCKS == 1 */
        }
        /* Release the previously taken xQueueLock. */
        prvEXIT_CRITICAL_SMP_ONLY( &( pxQueue->xQueueLock ) );
    }

#endif /* configUSE_TIMERS */
/*-----------------------------------------------------------*/

#सचे ( ( configUSE_QUEUE_SETS == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) )

    QueueSetHandle_t xQueueCreateSet( ध्रुव UBaseType_t uxEventQueueLength )
    {
        QueueSetHandle_t pxQueue;

        pxQueue = xQueueGenericCreate( uxEventQueueLength, ( UBaseType_t ) परिमाण( Queue_t * ), queueQUEUE_TYPE_SET );

        पटिनिवत्त pxQueue;
    }

#endif /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#सचे ( configUSE_QUEUE_SETS == 1 )

    BaseType_t xQueueAddToSet( QueueSetMemberHandle_t xQueueOrSemaphore,
                               QueueSetHandle_t xQueueSet )
    {
        BaseType_t xReturn;

        taskENTER_CRITICAL( &( ( ( Queue_t * ) xQueueOrSemaphore )->xQueueLock ) );
        {
            सचे( ( ( Queue_t * ) xQueueOrSemaphore )->pxQueueSetContainer != रिक्त )
            {
                /* Cannot add a queue/semaphore to more than one queue set. */
                xReturn = pdFAIL;
            }
            अञ्ञथा सचे( ( ( Queue_t * ) xQueueOrSemaphore )->uxMessagesWaiting != ( UBaseType_t ) 0 )
            {
                /* Cannot add a queue/semaphore to a queue set if there are already
                 * items in the queue/semaphore. */
                xReturn = pdFAIL;
            }
            अञ्ञथा
            {
                ( ( Queue_t * ) xQueueOrSemaphore )->pxQueueSetContainer = xQueueSet;
                xReturn = pdPASS;
            }
        }
        taskEXIT_CRITICAL( &( ( ( Queue_t * ) xQueueOrSemaphore )->xQueueLock ) );

        पटिनिवत्त xReturn;
    }

#endif /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#सचे ( configUSE_QUEUE_SETS == 1 )

    BaseType_t xQueueRemoveFromSet( QueueSetMemberHandle_t xQueueOrSemaphore,
                                    QueueSetHandle_t xQueueSet )
    {
        BaseType_t xReturn;
        Queue_t * ध्रुव pxQueueOrSemaphore = ( Queue_t * ) xQueueOrSemaphore;

        सचे( pxQueueOrSemaphore->pxQueueSetContainer != xQueueSet )
        {
            /* The queue was not a member of the set. */
            xReturn = pdFAIL;
        }
        अञ्ञथा सचे( pxQueueOrSemaphore->uxMessagesWaiting != ( UBaseType_t ) 0 )
        {
            /* It is dangerous to remove a queue from a set when the queue is
             * not empty because the queue set will still hold pending events for
             * the queue. */
            xReturn = pdFAIL;
        }
        अञ्ञथा
        {
            taskENTER_CRITICAL( &( ( ( Queue_t * ) pxQueueOrSemaphore )->xQueueLock ) );
            {
                /* The queue is no longer contained in the set. */
                pxQueueOrSemaphore->pxQueueSetContainer = रिक्त;
            }
            taskEXIT_CRITICAL( &( ( ( Queue_t * ) pxQueueOrSemaphore )->xQueueLock ) );
            xReturn = pdPASS;
        }

        पटिनिवत्त xReturn;
    } /*lint !e818 xQueueSet could not be declared as pointing to const as it is a typedef. */

#endif /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#सचे ( configUSE_QUEUE_SETS == 1 )

    QueueSetMemberHandle_t xQueueSelectFromSet( QueueSetHandle_t xQueueSet,
                                                TickType_t ध्रुव xTicksToWait )
    {
        QueueSetMemberHandle_t xReturn = रिक्त;

        ( शून्य ) xQueueReceive( ( QueueHandle_t ) xQueueSet, &xReturn, xTicksToWait ); /*lint !e961 Casting from one typedef to another is not redundant. */
        पटिनिवत्त xReturn;
    }

#endif /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#सचे ( configUSE_QUEUE_SETS == 1 )

    QueueSetMemberHandle_t xQueueSelectFromSetFromISR( QueueSetHandle_t xQueueSet )
    {
        QueueSetMemberHandle_t xReturn = रिक्त;

        ( शून्य ) xQueueReceiveFromISR( ( QueueHandle_t ) xQueueSet, &xReturn, रिक्त ); /*lint !e961 Casting from one typedef to another is not redundant. */
        पटिनिवत्त xReturn;
    }

#endif /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#सचे ( configUSE_QUEUE_SETS == 1 )

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

        /* In SMP, queue sets have their own xQueueLock. Thus we need to also
         * acquire the queue set's xQueueLock before accessing it. */
        prvENTER_CRITICAL_SAFE_SMP_ONLY( &( pxQueueSetContainer->xQueueLock ) );
        {
            सचे( pxQueueSetContainer->uxMessagesWaiting < pxQueueSetContainer->uxLength )
            {
                #सचे ( queueUSE_LOCKS == 1 )
                    ध्रुव int8_t cTxLock = pxQueueSetContainer->cTxLock;
                #अञ्ञथा
                    /* Queue locks not used, so we treat it as unlocked. */
                    ध्रुव int8_t cTxLock = queueUNLOCKED;
                #endif /* queueUSE_LOCKS == 1 */

                traceQUEUE_SET_SEND( pxQueueSetContainer );

                /* The data copied is the handle of the queue that contains data. */
                xReturn = prvCopyDataToQueue( pxQueueSetContainer, &pxQueue, queueSEND_TO_BACK );

                सचे( cTxLock == queueUNLOCKED )
                {
                    सचे( listLIST_IS_EMPTY( &( pxQueueSetContainer->xTasksWaitingToReceive ) ) == pdFALSE )
                    {
                        सचे( xTaskRemoveFromEventList( &( pxQueueSetContainer->xTasksWaitingToReceive ) ) != pdFALSE )
                        {
                            /* The task waiting has a higher priority. */
                            xReturn = pdTRUE;
                        }
                        अञ्ञथा
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    अञ्ञथा
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                अञ्ञथा
                {
                    #सचे ( queueUSE_LOCKS == 1 )
                    {
                        prvIncrementQueueTxLock( pxQueueSetContainer, cTxLock );
                    }
                    #endif /* queueUSE_LOCKS == 1 */
                }
            }
            अञ्ञथा
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        /* Release the previously acquired queue set's xQueueLock. */
        prvEXIT_CRITICAL_SAFE_SMP_ONLY( &( pxQueueSetContainer->xQueueLock ) );

        पटिनिवत्त xReturn;
    }

#endif /* configUSE_QUEUE_SETS */
