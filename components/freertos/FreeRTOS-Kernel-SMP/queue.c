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

#ضمن <stdlib.h>
#ضمن <string.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#define MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#ضمن "FreeRTOS.h"
#ضمن "task.h"
#ضمن "queue.h"

#إذا ( configUSE_CO_ROUTINES == 1 )
    #ضمن "croutine.h"
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
#define queueQUEUE_IS_MUTEX       عدم

تعريفنوع بنية QueuePointers
{
    int8_t * pcTail;     /**< Points to the byte at the end of the queue storage area.  Once more byte is allocated than necessary to store the queue items, this is used as a marker. */
    int8_t * pcReadFrom; /**< Points to the last place that a queued item was read from when the structure is used as a queue. */
} QueuePointers_t;

تعريفنوع بنية SemaphoreData
{
    TaskHandle_t xMutexHolder;        /**< The handle of the task that holds the mutex. */
    UBaseType_t ux_Recursive_Call_گنتی; /**< Maintains a count of the number of times a recursive mutex has been recursively 'taken' when the structure is used as a mutex. */
} SemaphoreData_t;

/* Semaphores do not actually store or copy data, so have an item size of
 * zero. */
#define queueSEMAPHORE_QUEUE_ITEM_LENGTH    ( ( UBaseType_t ) 0 )
#define queueMUTEX_GIVE_BLOCK_TIME          ( ( TickType_t ) 0U )

#إذا ( configUSE_PREEMPTION == 0 )

/* If the cooperative scheduler is being used then a yield should not be
 * performed just because a higher priority task has been woken. */
    #define queueYIELD_IF_USING_PREEMPTION()
#وإلا
    #إذا ( config_نمبر_OF_CORES == 1 )
        #define queueYIELD_IF_USING_PREEMPTION()    portYIELD_WITHIN_API()
    #وإلا /* #if ( configNUMBER_OF_CORES == 1 ) */
        #define queueYIELD_IF_USING_PREEMPTION()    vTaskYieldWithinAPI()
    #endif /* #if ( configNUMBER_OF_CORES == 1 ) */
#endif

/*
 * Definition of the queue used by the scheduler.
 * Items are queued by copy, not reference.  See the following link for the
 * rationale: https://www.FreeRTOS.org/Embedded-RTOS-Queues.html
 */
تعريفنوع بنية QueueDefinition /* The old naming convention is used to prevent breaking kernel aware debuggers. */
{
    int8_t * pcHead;           /**< Points to the beginning of the queue storage area. */
    int8_t * pcWriteTo;        /**< Points to the free next place in the storage area. */

    اتحاد
    {
        QueuePointers_t xQueue;     /**< Data required exclusively when this structure is used as a queue. */
        SemaphoreData_t xSemaphore; /**< Data required exclusively when this structure is used as a semaphore. */
    } u;

    List_t xTasksWaitingToSend;             /**< List of tasks that are blocked waiting to post onto this queue.  Stored in priority order. */
    List_t xTasksWaitingToReceive;          /**< List of tasks that are blocked waiting to read from this queue.  Stored in priority order. */

    متطاير UBaseType_t uxMessagesWaiting; /**< The number of items currently in the queue. */
    UBaseType_t uxLength;                   /**< The length of the queue defined as the number of items it will hold, not the number of bytes. */
    UBaseType_t uxItemSize;                 /**< The size of each items that the queue will hold. */

    متطاير int8_t cRxLock;                /**< Stores the number of items received from the queue (removed from the queue) while the queue was locked.  Set to queueUNLOCKED when the queue is not locked. */
    متطاير int8_t cTxLock;                /**< Stores the number of items transmitted to the queue (added to the queue) while the queue was locked.  Set to queueUNLOCKED when the queue is not locked. */

    #إذا ( ( configSUPPORT_STATIC_ALLOCATION == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) )
        uint8_t ucStaticallyAllocated; /**< Set to pdTRUE if the memory used by the queue was statically allocated to ensure no attempt is made to free the memory. */
    #endif

    #إذا ( configUSE_QUEUE_SETS == 1 )
        بنية QueueDefinition * pxQueueSetContainer;
    #endif

    #إذا ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_Queue_نمبر;
        uint8_t ucQueueType;
    #endif
} xQUEUE;

/* The old xQUEUE name is maintained above then typedefed to the new Queue_t
 * name below to enable the use of older kernel aware debuggers. */
تعريفنوع xQUEUE Queue_t;

/*-----------------------------------------------------------*/

/*
 * The queue registry is just a means for kernel aware debuggers to locate
 * queue structures.  It has no other purpose so is an optional component.
 */
#إذا ( configQUEUE_REGISTRY_SIZE > 0 )

/* The type stored within the queue registry array.  This allows a name
 * to be assigned to each queue making kernel aware debugging a little
 * more user friendly. */
    تعريفنوع بنية QUEUE_REGISTRY_ITEM
    {
        ثابت حرف * pcQueueName;
        QueueHandle_t xHandle;
    } xQueueRegistryItem;

/* The old xQueueRegistryItem name is maintained above then typedefed to the
 * new xQueueRegistryItem name below to enable the use of older kernel aware
 * debuggers. */
    تعريفنوع xQueueRegistryItem QueueRegistryItem_t;

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
ساكن فراغ prvUnlockQueue( Queue_t * ثابت pxQueue ) PRIVILEGED_FUNCTION;

/*
 * Uses a critical section to determine if there is any data in a queue.
 *
 * @return pdTRUE if the queue contains no items, otherwise pdFALSE.
 */
ساكن BaseType_t prvIsQueueEmpty( ثابت Queue_t * pxQueue ) PRIVILEGED_FUNCTION;

/*
 * Uses a critical section to determine if there is any space in a queue.
 *
 * @return pdTRUE if there is no space, otherwise pdFALSE;
 */
ساكن BaseType_t prvIsQueueFull( ثابت Queue_t * pxQueue ) PRIVILEGED_FUNCTION;

/*
 * Copies an item into the queue, either at the front of the queue or the
 * back of the queue.
 */
ساكن BaseType_t prvCopyDataToQueue( Queue_t * ثابت pxQueue,
                                      ثابت فراغ * pvItemToQueue,
                                      ثابت BaseType_t xPosition ) PRIVILEGED_FUNCTION;

/*
 * Copies an item out of a queue.
 */
ساكن فراغ prvCopyDataFromQueue( Queue_t * ثابت pxQueue,
                                  فراغ * ثابت pvBuffer ) PRIVILEGED_FUNCTION;

#إذا ( configUSE_QUEUE_SETS == 1 )

/*
 * Checks to see if a queue is a member of a queue set, and if so, notifies
 * the queue set that the queue contains data.
 */
    ساكن BaseType_t prvNotifyQueueSetContainer( ثابت Queue_t * ثابت pxQueue ) PRIVILEGED_FUNCTION;
#endif

/*
 * Called after a Queue_t structure has been allocated either statically or
 * dynamically to fill in the structure's members.
 */
ساكن فراغ prvInitialiseNewQueue( ثابت UBaseType_t uxQueueLength,
                                   ثابت UBaseType_t uxItemSize,
                                   uint8_t * pucQueueStorage,
                                   ثابت uint8_t ucQueueType,
                                   Queue_t * pxNewQueue ) PRIVILEGED_FUNCTION;

/*
 * Mutexes are a special type of queue.  When a mutex is created, first the
 * queue is created, then prvInitialiseMutex() is called to configure the queue
 * as a mutex.
 */
#إذا ( configUSE_MUTEXES == 1 )
    ساكن فراغ prvInitialiseMutex( Queue_t * pxNewQueue ) PRIVILEGED_FUNCTION;
#endif

#إذا ( configUSE_MUTEXES == 1 )

/*
 * If a task waiting for a mutex causes the mutex holder to inherit a
 * priority, but the waiting task times out, then the holder should
 * disinherit the priority - but only down to the highest priority of any
 * other tasks that are waiting for the same mutex.  This function returns
 * that priority.
 */
    ساكن UBaseType_t prvGetDisinheritPriorityAfterTimeout( ثابت Queue_t * ثابت pxQueue ) PRIVILEGED_FUNCTION;
#endif
/*-----------------------------------------------------------*/

/*
 * Macro to mark a queue as locked.  Locking a queue prevents an ISR from
 * accessing the queue event lists.
 */
#define prvLockQueue( pxQueue )                            \
    taskENTER_CRITICAL();                                  \
    {                                                      \
        إذا( ( pxQueue )->cRxLock == queueUNLOCKED )        \
        {                                                  \
            ( pxQueue )->cRxLock = queueLOCKED_UNMODIFIED; \
        }                                                  \
        إذا( ( pxQueue )->cTxLock == queueUNLOCKED )        \
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
    افعل {                                                                      \
        ثابت UBaseType_t ux_نمبر_Of_Tasks = ux_Task_Get_نمبر_Of_Tasks();         \
        إذا( ( UBaseType_t ) ( cTxLock ) < ux_نمبر_Of_Tasks )                   \
        {                                                                     \
            configASSERT( ( cTxLock ) != queueINT8_MAX );                     \
            ( pxQueue )->cTxLock = ( int8_t ) ( ( cTxLock ) + ( int8_t ) 1 ); \
        }                                                                     \
    } طالما( 0 )

/*
 * Macro to increment cRxLock member of the queue data structure. It is
 * capped at the number of tasks in the system as we cannot unblock more
 * tasks than the number of tasks in the system.
 */
#define prvIncrementQueueRxLock( pxQueue, cRxLock )                           \
    افعل {                                                                      \
        ثابت UBaseType_t ux_نمبر_Of_Tasks = ux_Task_Get_نمبر_Of_Tasks();         \
        إذا( ( UBaseType_t ) ( cRxLock ) < ux_نمبر_Of_Tasks )                   \
        {                                                                     \
            configASSERT( ( cRxLock ) != queueINT8_MAX );                     \
            ( pxQueue )->cRxLock = ( int8_t ) ( ( cRxLock ) + ( int8_t ) 1 ); \
        }                                                                     \
    } طالما( 0 )
/*-----------------------------------------------------------*/

BaseType_t xQueueGenericReset( QueueHandle_t xQueue,
                               BaseType_t xNewQueue )
{
    BaseType_t xReturn = pdPASS;
    Queue_t * ثابت pxQueue = xQueue;

    traceENTER_xQueueGenericReset( xQueue, xNewQueue );

    configASSERT( pxQueue );

    إذا( ( pxQueue != عدم ) &&
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

            إذا( xNewQueue == pdFALSE )
            {
                /* If there are tasks blocked waiting to read from the queue, then
                 * the tasks will remain blocked as after this function exits the queue
                 * will still be empty.  If there are tasks blocked waiting to write to
                 * the queue, then one should be unblocked as after this function exits
                 * it will be possible to write to it. */
                إذا( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToSend ) ) == pdFALSE )
                {
                    إذا( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToSend ) ) != pdFALSE )
                    {
                        queueYIELD_IF_USING_PREEMPTION();
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
            }
            وإلا
            {
                /* Ensure the event queues start in the correct state. */
                vListInitialise( &( pxQueue->xTasksWaitingToSend ) );
                vListInitialise( &( pxQueue->xTasksWaitingToReceive ) );
            }
        }
        taskEXIT_CRITICAL();
    }
    وإلا
    {
        xReturn = pdFAIL;
    }

    configASSERT( xReturn != pdFAIL );

    /* A value is returned for calling semantic consistency with previous
     * versions. */
    traceRETURN_xQueueGenericReset( xReturn );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

#إذا ( configSUPPORT_STATIC_ALLOCATION == 1 )

    QueueHandle_t xQueueGenericCreateStatic( ثابت UBaseType_t uxQueueLength,
                                             ثابت UBaseType_t uxItemSize,
                                             uint8_t * pucQueueStorage,
                                             StaticQueue_t * pxStaticQueue,
                                             ثابت uint8_t ucQueueType )
    {
        Queue_t * pxNewQueue = عدم;

        traceENTER_xQueueGenericCreateStatic( uxQueueLength, uxItemSize, pucQueueStorage, pxStaticQueue, ucQueueType );

        /* The StaticQueue_t structure and the queue storage area must be
         * supplied. */
        configASSERT( pxStaticQueue );

        إذا( ( uxQueueLength > ( UBaseType_t ) 0 ) &&
            ( pxStaticQueue != عدم ) &&

            /* A queue storage area should be provided if the item size is not 0, and
             * should not be provided if the item size is 0. */
            ( !( ( pucQueueStorage != عدم ) && ( uxItemSize == 0U ) ) ) &&
            ( !( ( pucQueueStorage == عدم ) && ( uxItemSize != 0U ) ) ) )
        {
            #إذا ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticQueue_t or StaticSemaphore_t equals the size of
                 * the real queue and semaphore structures. */
                متطاير size_t xSize = حجم( StaticQueue_t );

                /* This assertion cannot be branch covered in unit tests */
                configASSERT( xSize == حجم( Queue_t ) ); /* LCOV_EXCL_BR_LINE */
                ( فراغ ) xSize;                             /* Prevent unused variable warning when configASSERT() is not defined. */
            }
            #endif /* configASSERT_DEFINED */

            /* The address of a statically allocated queue was passed in, use it.
             * The address of a statically allocated storage area was also passed in
             * but is already set. */
            /* MISRA Ref 11.3.1 [Misaligned access] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
            /* coverity[misra_c_2012_rule_11_3_violation] */
            pxNewQueue = ( Queue_t * ) pxStaticQueue;

            #إذا ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
            {
                /* Queues can be allocated either statically or dynamically, so
                 * note this queue was allocated statically in case the queue is
                 * later deleted. */
                pxNewQueue->ucStaticallyAllocated = pdTRUE;
            }
            #endif /* configSUPPORT_DYNAMIC_ALLOCATION */

            prvInitialiseNewQueue( uxQueueLength, uxItemSize, pucQueueStorage, ucQueueType, pxNewQueue );
        }
        وإلا
        {
            configASSERT( pxNewQueue );
            mtCOVERAGE_TEST_MARKER();
        }

        traceRETURN_xQueueGenericCreateStatic( pxNewQueue );

        أرجع pxNewQueue;
    }

#endif /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

#إذا ( configSUPPORT_STATIC_ALLOCATION == 1 )

    BaseType_t xQueueGenericGetStaticBuffers( QueueHandle_t xQueue,
                                              uint8_t ** ppucQueueStorage,
                                              StaticQueue_t ** ppxStaticQueue )
    {
        BaseType_t xReturn;
        Queue_t * ثابت pxQueue = xQueue;

        traceENTER_xQueueGenericGetStaticBuffers( xQueue, ppucQueueStorage, ppxStaticQueue );

        configASSERT( pxQueue );
        configASSERT( ppxStaticQueue );

        #إذا ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
        {
            /* Check if the queue was statically allocated. */
            إذا( pxQueue->ucStaticallyAllocated == ( uint8_t ) pdTRUE )
            {
                إذا( ppucQueueStorage != عدم )
                {
                    *ppucQueueStorage = ( uint8_t * ) pxQueue->pcHead;
                }

                /* MISRA Ref 11.3.1 [Misaligned access] */
                /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
                /* coverity[misra_c_2012_rule_11_3_violation] */
                *ppxStaticQueue = ( StaticQueue_t * ) pxQueue;
                xReturn = pdTRUE;
            }
            وإلا
            {
                xReturn = pdFALSE;
            }
        }
        #وإلا /* configSUPPORT_DYNAMIC_ALLOCATION */
        {
            /* Queue must have been statically allocated. */
            إذا( ppucQueueStorage != عدم )
            {
                *ppucQueueStorage = ( uint8_t * ) pxQueue->pcHead;
            }

            *ppxStaticQueue = ( StaticQueue_t * ) pxQueue;
            xReturn = pdTRUE;
        }
        #endif /* configSUPPORT_DYNAMIC_ALLOCATION */

        traceRETURN_xQueueGenericGetStaticBuffers( xReturn );

        أرجع xReturn;
    }

#endif /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

#إذا ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )

    QueueHandle_t xQueueGenericCreate( ثابت UBaseType_t uxQueueLength,
                                       ثابت UBaseType_t uxItemSize,
                                       ثابت uint8_t ucQueueType )
    {
        Queue_t * pxNewQueue = عدم;
        size_t xQueueSizeInBytes;
        uint8_t * pucQueueStorage;

        traceENTER_xQueueGenericCreate( uxQueueLength, uxItemSize, ucQueueType );

        إذا( ( uxQueueLength > ( UBaseType_t ) 0 ) &&
            /* Check for multiplication overflow. */
            ( ( SIZE_MAX / uxQueueLength ) >= uxItemSize ) &&
            /* Check for addition overflow. */
            ( ( UBaseType_t ) ( SIZE_MAX - حجم( Queue_t ) ) >= ( uxQueueLength * uxItemSize ) ) )
        {
            /* Allocate enough space to hold the maximum number of items that
             * can be in the queue at any time.  It is valid for uxItemSize to be
             * zero in the case the queue is used as a semaphore. */
            xQueueSizeInBytes = ( size_t ) ( ( size_t ) uxQueueLength * ( size_t ) uxItemSize );

            /* MISRA Ref 11.5.1 [Malloc memory assignment] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
            /* coverity[misra_c_2012_rule_11_5_violation] */
            pxNewQueue = ( Queue_t * ) pvPortMalloc( حجم( Queue_t ) + xQueueSizeInBytes );

            إذا( pxNewQueue != عدم )
            {
                /* Jump past the queue structure to find the location of the queue
                 * storage area. */
                pucQueueStorage = ( uint8_t * ) pxNewQueue;
                pucQueueStorage += حجم( Queue_t );

                #إذا ( configSUPPORT_STATIC_ALLOCATION == 1 )
                {
                    /* Queues can be created either statically or dynamically, so
                     * note this task was created dynamically in case it is later
                     * deleted. */
                    pxNewQueue->ucStaticallyAllocated = pdFALSE;
                }
                #endif /* configSUPPORT_STATIC_ALLOCATION */

                prvInitialiseNewQueue( uxQueueLength, uxItemSize, pucQueueStorage, ucQueueType, pxNewQueue );
            }
            وإلا
            {
                traceQUEUE_CREATE_FAILED( ucQueueType );
                mtCOVERAGE_TEST_MARKER();
            }
        }
        وإلا
        {
            configASSERT( pxNewQueue );
            mtCOVERAGE_TEST_MARKER();
        }

        traceRETURN_xQueueGenericCreate( pxNewQueue );

        أرجع pxNewQueue;
    }

#endif /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

ساكن فراغ prvInitialiseNewQueue( ثابت UBaseType_t uxQueueLength,
                                   ثابت UBaseType_t uxItemSize,
                                   uint8_t * pucQueueStorage,
                                   ثابت uint8_t ucQueueType,
                                   Queue_t * pxNewQueue )
{
    /* Remove compiler warnings about unused parameters should
     * configUSE_TRACE_FACILITY not be set to 1. */
    ( فراغ ) ucQueueType;

    إذا( uxItemSize == ( UBaseType_t ) 0 )
    {
        /* No RAM was allocated for the queue storage area, but PC head cannot
         * be set to NULL because NULL is used as a key to say the queue is used as
         * a mutex.  Therefore just set pcHead to point to the queue as a benign
         * value that is known to be within the memory map. */
        pxNewQueue->pcHead = ( int8_t * ) pxNewQueue;
    }
    وإلا
    {
        /* Set the head to the start of the queue storage area. */
        pxNewQueue->pcHead = ( int8_t * ) pucQueueStorage;
    }

    /* Initialise the queue members as described where the queue type is
     * defined. */
    pxNewQueue->uxLength = uxQueueLength;
    pxNewQueue->uxItemSize = uxItemSize;
    ( فراغ ) xQueueGenericReset( pxNewQueue, pdTRUE );

    #إذا ( configUSE_TRACE_FACILITY == 1 )
    {
        pxNewQueue->ucQueueType = ucQueueType;
    }
    #endif /* configUSE_TRACE_FACILITY */

    #إذا ( configUSE_QUEUE_SETS == 1 )
    {
        pxNewQueue->pxQueueSetContainer = عدم;
    }
    #endif /* configUSE_QUEUE_SETS */

    traceQUEUE_CREATE( pxNewQueue );
}
/*-----------------------------------------------------------*/

#إذا ( configUSE_MUTEXES == 1 )

    ساكن فراغ prvInitialiseMutex( Queue_t * pxNewQueue )
    {
        إذا( pxNewQueue != عدم )
        {
            /* The queue create function will set all the queue structure members
            * correctly for a generic queue, but this function is creating a
            * mutex.  Overwrite those members that need to be set differently -
            * in particular the information required for priority inheritance. */
            pxNewQueue->u.xSemaphore.xMutexHolder = عدم;
            pxNewQueue->uxQueueType = queueQUEUE_IS_MUTEX;

            /* In case this is a recursive mutex. */
            pxNewQueue->u.xSemaphore.ux_Recursive_Call_گنتی = 0;

            traceCREATE_MUTEX( pxNewQueue );

            /* Start with the semaphore in the expected state. */
            ( فراغ ) xQueueGenericSend( pxNewQueue, عدم, ( TickType_t ) 0U, queueSEND_TO_BACK );
        }
        وإلا
        {
            traceCREATE_MUTEX_FAILED();
        }
    }

#endif /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

#إذا ( ( configUSE_MUTEXES == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) )

    QueueHandle_t xQueueCreateMutex( ثابت uint8_t ucQueueType )
    {
        QueueHandle_t xNewQueue;
        ثابت UBaseType_t uxMutexLength = ( UBaseType_t ) 1, uxMutexSize = ( UBaseType_t ) 0;

        traceENTER_xQueueCreateMutex( ucQueueType );

        xNewQueue = xQueueGenericCreate( uxMutexLength, uxMutexSize, ucQueueType );
        prvInitialiseMutex( ( Queue_t * ) xNewQueue );

        traceRETURN_xQueueCreateMutex( xNewQueue );

        أرجع xNewQueue;
    }

#endif /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

#إذا ( ( configUSE_MUTEXES == 1 ) && ( configSUPPORT_STATIC_ALLOCATION == 1 ) )

    QueueHandle_t xQueueCreateMutexStatic( ثابت uint8_t ucQueueType,
                                           StaticQueue_t * pxStaticQueue )
    {
        QueueHandle_t xNewQueue;
        ثابت UBaseType_t uxMutexLength = ( UBaseType_t ) 1, uxMutexSize = ( UBaseType_t ) 0;

        traceENTER_xQueueCreateMutexStatic( ucQueueType, pxStaticQueue );

        /* Prevent compiler warnings about unused parameters if
         * configUSE_TRACE_FACILITY does not equal 1. */
        ( فراغ ) ucQueueType;

        xNewQueue = xQueueGenericCreateStatic( uxMutexLength, uxMutexSize, عدم, pxStaticQueue, ucQueueType );
        prvInitialiseMutex( ( Queue_t * ) xNewQueue );

        traceRETURN_xQueueCreateMutexStatic( xNewQueue );

        أرجع xNewQueue;
    }

#endif /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

#إذا ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) )

    TaskHandle_t xQueueGetMutexHolder( QueueHandle_t xSemaphore )
    {
        TaskHandle_t pxReturn;
        Queue_t * ثابت pxSemaphore = ( Queue_t * ) xSemaphore;

        traceENTER_xQueueGetMutexHolder( xSemaphore );

        configASSERT( xSemaphore );

        /* This function is called by xSemaphoreGetMutexHolder(), and should not
         * be called directly.  Note:  This is a good way of determining if the
         * calling task is the mutex holder, but not a good way of determining the
         * identity of the mutex holder, as the holder may change between the
         * following critical section exiting and the function returning. */
        taskENTER_CRITICAL();
        {
            إذا( pxSemaphore->uxQueueType == queueQUEUE_IS_MUTEX )
            {
                pxReturn = pxSemaphore->u.xSemaphore.xMutexHolder;
            }
            وإلا
            {
                pxReturn = عدم;
            }
        }
        taskEXIT_CRITICAL();

        traceRETURN_xQueueGetMutexHolder( pxReturn );

        أرجع pxReturn;
    }

#endif /* if ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) ) */
/*-----------------------------------------------------------*/

#إذا ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) )

    TaskHandle_t xQueueGetMutexHolderFromISR( QueueHandle_t xSemaphore )
    {
        TaskHandle_t pxReturn;

        traceENTER_xQueueGetMutexHolderFromISR( xSemaphore );

        configASSERT( xSemaphore );

        /* Mutexes cannot be used in interrupt service routines, so the mutex
         * holder should not change in an ISR, and therefore a critical section is
         * not required here. */
        إذا( ( ( Queue_t * ) xSemaphore )->uxQueueType == queueQUEUE_IS_MUTEX )
        {
            pxReturn = ( ( Queue_t * ) xSemaphore )->u.xSemaphore.xMutexHolder;
        }
        وإلا
        {
            pxReturn = عدم;
        }

        traceRETURN_xQueueGetMutexHolderFromISR( pxReturn );

        أرجع pxReturn;
    }

#endif /* if ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) ) */
/*-----------------------------------------------------------*/

#إذا ( configUSE_RECURSIVE_MUTEXES == 1 )

    BaseType_t xQueueGiveMutexRecursive( QueueHandle_t xMutex )
    {
        BaseType_t xReturn;
        Queue_t * ثابت pxMutex = ( Queue_t * ) xMutex;

        traceENTER_xQueueGiveMutexRecursive( xMutex );

        configASSERT( pxMutex );

        /* If this is the task that holds the mutex then xMutexHolder will not
         * change outside of this task.  If this task does not hold the mutex then
         * pxMutexHolder can never coincidentally equal the tasks handle, and as
         * this is the only condition we are interested in it does not matter if
         * pxMutexHolder is accessed simultaneously by another task.  Therefore no
         * mutual exclusion is required to test the pxMutexHolder variable. */
        إذا( pxMutex->u.xSemaphore.xMutexHolder == xTaskGetCurrentTaskHandle() )
        {
            traceGIVE_MUTEX_RECURSIVE( pxMutex );

            /* uxRecursiveCallCount cannot be zero if xMutexHolder is equal to
             * the task handle, therefore no underflow check is required.  Also,
             * uxRecursiveCallCount is only modified by the mutex holder, and as
             * there can only be one, no mutual exclusion is required to modify the
             * uxRecursiveCallCount member. */
            ( pxMutex->u.xSemaphore.ux_Recursive_Call_گنتی )--;

            /* Has the recursive call count unwound to 0? */
            إذا( pxMutex->u.xSemaphore.ux_Recursive_Call_گنتی == ( UBaseType_t ) 0 )
            {
                /* Return the mutex.  This will automatically unblock any other
                 * task that might be waiting to access the mutex. */
                ( فراغ ) xQueueGenericSend( pxMutex, عدم, queueMUTEX_GIVE_BLOCK_TIME, queueSEND_TO_BACK );
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }

            xReturn = pdPASS;
        }
        وإلا
        {
            /* The mutex cannot be given because the calling task is not the
             * holder. */
            xReturn = pdFAIL;

            traceGIVE_MUTEX_RECURSIVE_FAILED( pxMutex );
        }

        traceRETURN_xQueueGiveMutexRecursive( xReturn );

        أرجع xReturn;
    }

#endif /* configUSE_RECURSIVE_MUTEXES */
/*-----------------------------------------------------------*/

#إذا ( configUSE_RECURSIVE_MUTEXES == 1 )

    BaseType_t xQueueTakeMutexRecursive( QueueHandle_t xMutex,
                                         TickType_t xTicksToWait )
    {
        BaseType_t xReturn;
        Queue_t * ثابت pxMutex = ( Queue_t * ) xMutex;

        traceENTER_xQueueTakeMutexRecursive( xMutex, xTicksToWait );

        configASSERT( pxMutex );

        /* Comments regarding mutual exclusion as per those within
         * xQueueGiveMutexRecursive(). */

        traceTAKE_MUTEX_RECURSIVE( pxMutex );

        إذا( pxMutex->u.xSemaphore.xMutexHolder == xTaskGetCurrentTaskHandle() )
        {
            ( pxMutex->u.xSemaphore.ux_Recursive_Call_گنتی )++;
            xReturn = pdPASS;
        }
        وإلا
        {
            xReturn = xQueueSemaphoreTake( pxMutex, xTicksToWait );

            /* pdPASS will only be returned if the mutex was successfully
             * obtained.  The calling task may have entered the Blocked state
             * before reaching here. */
            إذا( xReturn != pdFAIL )
            {
                ( pxMutex->u.xSemaphore.ux_Recursive_Call_گنتی )++;
            }
            وإلا
            {
                traceTAKE_MUTEX_RECURSIVE_FAILED( pxMutex );
            }
        }

        traceRETURN_xQueueTakeMutexRecursive( xReturn );

        أرجع xReturn;
    }

#endif /* configUSE_RECURSIVE_MUTEXES */
/*-----------------------------------------------------------*/

#إذا ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( configSUPPORT_STATIC_ALLOCATION == 1 ) )

    QueueHandle_t xQueueCreateCountingSemaphoreStatic( ثابت UBaseType_t ux_Max_گنتی,
                                                       ثابت UBaseType_t ux_Initial_گنتی,
                                                       StaticQueue_t * pxStaticQueue )
    {
        QueueHandle_t xHandle = عدم;

        traceENTER_xQueueCreateCountingSemaphoreStatic( ux_Max_گنتی, ux_Initial_گنتی, pxStaticQueue );

        إذا( ( ux_Max_گنتی != 0U ) &&
            ( ux_Initial_گنتی <= ux_Max_گنتی ) )
        {
            xHandle = xQueueGenericCreateStatic( ux_Max_گنتی, queueSEMAPHORE_QUEUE_ITEM_LENGTH, عدم, pxStaticQueue, queueQUEUE_TYPE_COUNTING_SEMAPHORE );

            إذا( xHandle != عدم )
            {
                ( ( Queue_t * ) xHandle )->uxMessagesWaiting = ux_Initial_گنتی;

                traceCREATE_COUNTING_SEMAPHORE();
            }
            وإلا
            {
                traceCREATE_COUNTING_SEMAPHORE_FAILED();
            }
        }
        وإلا
        {
            configASSERT( xHandle );
            mtCOVERAGE_TEST_MARKER();
        }

        traceRETURN_xQueueCreateCountingSemaphoreStatic( xHandle );

        أرجع xHandle;
    }

#endif /* ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) ) */
/*-----------------------------------------------------------*/

#إذا ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) )

    QueueHandle_t xQueueCreateCountingSemaphore( ثابت UBaseType_t ux_Max_گنتی,
                                                 ثابت UBaseType_t ux_Initial_گنتی )
    {
        QueueHandle_t xHandle = عدم;

        traceENTER_xQueueCreateCountingSemaphore( ux_Max_گنتی, ux_Initial_گنتی );

        إذا( ( ux_Max_گنتی != 0U ) &&
            ( ux_Initial_گنتی <= ux_Max_گنتی ) )
        {
            xHandle = xQueueGenericCreate( ux_Max_گنتی, queueSEMAPHORE_QUEUE_ITEM_LENGTH, queueQUEUE_TYPE_COUNTING_SEMAPHORE );

            إذا( xHandle != عدم )
            {
                ( ( Queue_t * ) xHandle )->uxMessagesWaiting = ux_Initial_گنتی;

                traceCREATE_COUNTING_SEMAPHORE();
            }
            وإلا
            {
                traceCREATE_COUNTING_SEMAPHORE_FAILED();
            }
        }
        وإلا
        {
            configASSERT( xHandle );
            mtCOVERAGE_TEST_MARKER();
        }

        traceRETURN_xQueueCreateCountingSemaphore( xHandle );

        أرجع xHandle;
    }

#endif /* ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) ) */
/*-----------------------------------------------------------*/

BaseType_t xQueueGenericSend( QueueHandle_t xQueue,
                              ثابت فراغ * ثابت pvItemToQueue,
                              TickType_t xTicksToWait,
                              ثابت BaseType_t xCopyPosition )
{
    BaseType_t xEntryTimeSet = pdFALSE, xYieldRequired;
    TimeOut_t xTimeOut;
    Queue_t * ثابت pxQueue = xQueue;

    traceENTER_xQueueGenericSend( xQueue, pvItemToQueue, xTicksToWait, xCopyPosition );

    configASSERT( pxQueue );
    configASSERT( !( ( pvItemToQueue == عدم ) && ( pxQueue->uxItemSize != ( UBaseType_t ) 0U ) ) );
    configASSERT( !( ( xCopyPosition == queueOVERWRITE ) && ( pxQueue->uxLength != 1 ) ) );
    #إذا ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( xTicksToWait != 0 ) ) );
    }
    #endif

    لكل( ; ; )
    {
        taskENTER_CRITICAL();
        {
            /* Is there room on the queue now?  The running task must be the
             * highest priority task wanting to access the queue.  If the head item
             * in the queue is to be overwritten then it does not matter if the
             * queue is full. */
            إذا( ( pxQueue->uxMessagesWaiting < pxQueue->uxLength ) || ( xCopyPosition == queueOVERWRITE ) )
            {
                traceQUEUE_SEND( pxQueue );

                #إذا ( configUSE_QUEUE_SETS == 1 )
                {
                    ثابت UBaseType_t uxPreviousMessagesWaiting = pxQueue->uxMessagesWaiting;

                    xYieldRequired = prvCopyDataToQueue( pxQueue, pvItemToQueue, xCopyPosition );

                    إذا( pxQueue->pxQueueSetContainer != عدم )
                    {
                        إذا( ( xCopyPosition == queueOVERWRITE ) && ( uxPreviousMessagesWaiting != ( UBaseType_t ) 0 ) )
                        {
                            /* Do not notify the queue set as an existing item
                             * was overwritten in the queue so the number of items
                             * in the queue has not changed. */
                            mtCOVERAGE_TEST_MARKER();
                        }
                        وإلا إذا( prvNotifyQueueSetContainer( pxQueue ) != pdFALSE )
                        {
                            /* The queue is a member of a queue set, and posting
                             * to the queue set caused a higher priority task to
                             * unblock. A context switch is required. */
                            queueYIELD_IF_USING_PREEMPTION();
                        }
                        وإلا
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    وإلا
                    {
                        /* If there was a task waiting for data to arrive on the
                         * queue then unblock it now. */
                        إذا( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToReceive ) ) == pdFALSE )
                        {
                            إذا( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToReceive ) ) != pdFALSE )
                            {
                                /* The unblocked task has a priority higher than
                                 * our own so yield immediately.  Yes it is ok to
                                 * do this from within the critical section - the
                                 * kernel takes care of that. */
                                queueYIELD_IF_USING_PREEMPTION();
                            }
                            وإلا
                            {
                                mtCOVERAGE_TEST_MARKER();
                            }
                        }
                        وإلا إذا( xYieldRequired != pdFALSE )
                        {
                            /* This path is a special case that will only get
                             * executed if the task was holding multiple mutexes
                             * and the mutexes were given back in an order that is
                             * different to that in which they were taken. */
                            queueYIELD_IF_USING_PREEMPTION();
                        }
                        وإلا
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                }
                #وإلا /* configUSE_QUEUE_SETS */
                {
                    xYieldRequired = prvCopyDataToQueue( pxQueue, pvItemToQueue, xCopyPosition );

                    /* If there was a task waiting for data to arrive on the
                     * queue then unblock it now. */
                    إذا( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToReceive ) ) == pdFALSE )
                    {
                        إذا( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToReceive ) ) != pdFALSE )
                        {
                            /* The unblocked task has a priority higher than
                             * our own so yield immediately.  Yes it is ok to do
                             * this from within the critical section - the kernel
                             * takes care of that. */
                            queueYIELD_IF_USING_PREEMPTION();
                        }
                        وإلا
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    وإلا إذا( xYieldRequired != pdFALSE )
                    {
                        /* This path is a special case that will only get
                         * executed if the task was holding multiple mutexes and
                         * the mutexes were given back in an order that is
                         * different to that in which they were taken. */
                        queueYIELD_IF_USING_PREEMPTION();
                    }
                    وإلا
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #endif /* configUSE_QUEUE_SETS */

                taskEXIT_CRITICAL();

                traceRETURN_xQueueGenericSend( pdPASS );

                أرجع pdPASS;
            }
            وإلا
            {
                إذا( xTicksToWait == ( TickType_t ) 0 )
                {
                    /* The queue was full and no block time is specified (or
                     * the block time has expired) so leave now. */
                    taskEXIT_CRITICAL();

                    /* Return to the original privilege level before exiting
                     * the function. */
                    traceQUEUE_SEND_FAILED( pxQueue );
                    traceRETURN_xQueueGenericSend( errQUEUE_FULL );

                    أرجع errQUEUE_FULL;
                }
                وإلا إذا( xEntryTimeSet == pdFALSE )
                {
                    /* The queue was full and a block time was specified so
                     * configure the timeout structure. */
                    vTaskInternalSetTimeOutState( &xTimeOut );
                    xEntryTimeSet = pdTRUE;
                }
                وإلا
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
        إذا( xTaskCheckForTimeOut( &xTimeOut, &xTicksToWait ) == pdFALSE )
        {
            إذا( prvIsQueueFull( pxQueue ) != pdFALSE )
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
                إذا( xTaskResumeAll() == pdFALSE )
                {
                    taskYIELD_WITHIN_API();
                }
            }
            وإلا
            {
                /* Try again. */
                prvUnlockQueue( pxQueue );
                ( فراغ ) xTaskResumeAll();
            }
        }
        وإلا
        {
            /* The timeout has expired. */
            prvUnlockQueue( pxQueue );
            ( فراغ ) xTaskResumeAll();

            traceQUEUE_SEND_FAILED( pxQueue );
            traceRETURN_xQueueGenericSend( errQUEUE_FULL );

            أرجع errQUEUE_FULL;
        }
    }
}
/*-----------------------------------------------------------*/

BaseType_t xQueueGenericSendFromISR( QueueHandle_t xQueue,
                                     ثابت فراغ * ثابت pvItemToQueue,
                                     BaseType_t * ثابت pxHigherPriorityTaskWoken,
                                     ثابت BaseType_t xCopyPosition )
{
    BaseType_t xReturn;
    UBaseType_t uxSavedInterruptStatus;
    Queue_t * ثابت pxQueue = xQueue;

    traceENTER_xQueueGenericSendFromISR( xQueue, pvItemToQueue, pxHigherPriorityTaskWoken, xCopyPosition );

    configASSERT( pxQueue );
    configASSERT( !( ( pvItemToQueue == عدم ) && ( pxQueue->uxItemSize != ( UBaseType_t ) 0U ) ) );
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
        إذا( ( pxQueue->uxMessagesWaiting < pxQueue->uxLength ) || ( xCopyPosition == queueOVERWRITE ) )
        {
            ثابت int8_t cTxLock = pxQueue->cTxLock;
            ثابت UBaseType_t uxPreviousMessagesWaiting = pxQueue->uxMessagesWaiting;

            traceQUEUE_SEND_FROM_ISR( pxQueue );

            /* Semaphores use xQueueGiveFromISR(), so pxQueue will not be a
             *  semaphore or mutex.  That means prvCopyDataToQueue() cannot result
             *  in a task disinheriting a priority and prvCopyDataToQueue() can be
             *  called here even though the disinherit function does not check if
             *  the scheduler is suspended before accessing the ready lists. */
            ( فراغ ) prvCopyDataToQueue( pxQueue, pvItemToQueue, xCopyPosition );

            /* The event list is not altered if the queue is locked.  This will
             * be done when the queue is unlocked later. */
            إذا( cTxLock == queueUNLOCKED )
            {
                #إذا ( configUSE_QUEUE_SETS == 1 )
                {
                    إذا( pxQueue->pxQueueSetContainer != عدم )
                    {
                        إذا( ( xCopyPosition == queueOVERWRITE ) && ( uxPreviousMessagesWaiting != ( UBaseType_t ) 0 ) )
                        {
                            /* Do not notify the queue set as an existing item
                             * was overwritten in the queue so the number of items
                             * in the queue has not changed. */
                            mtCOVERAGE_TEST_MARKER();
                        }
                        وإلا إذا( prvNotifyQueueSetContainer( pxQueue ) != pdFALSE )
                        {
                            /* The queue is a member of a queue set, and posting
                             * to the queue set caused a higher priority task to
                             * unblock.  A context switch is required. */
                            إذا( pxHigherPriorityTaskWoken != عدم )
                            {
                                *pxHigherPriorityTaskWoken = pdTRUE;
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
                    }
                    وإلا
                    {
                        إذا( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToReceive ) ) == pdFALSE )
                        {
                            إذا( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToReceive ) ) != pdFALSE )
                            {
                                /* The task waiting has a higher priority so
                                 *  record that a context switch is required. */
                                إذا( pxHigherPriorityTaskWoken != عدم )
                                {
                                    *pxHigherPriorityTaskWoken = pdTRUE;
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
                        }
                        وإلا
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                }
                #وإلا /* configUSE_QUEUE_SETS */
                {
                    إذا( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToReceive ) ) == pdFALSE )
                    {
                        إذا( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToReceive ) ) != pdFALSE )
                        {
                            /* The task waiting has a higher priority so record that a
                             * context switch is required. */
                            إذا( pxHigherPriorityTaskWoken != عدم )
                            {
                                *pxHigherPriorityTaskWoken = pdTRUE;
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
                    }
                    وإلا
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }

                    /* Not used in this path. */
                    ( فراغ ) uxPreviousMessagesWaiting;
                }
                #endif /* configUSE_QUEUE_SETS */
            }
            وإلا
            {
                /* Increment the lock count so the task that unlocks the queue
                 * knows that data was posted while it was locked. */
                prvIncrementQueueTxLock( pxQueue, cTxLock );
            }

            xReturn = pdPASS;
        }
        وإلا
        {
            traceQUEUE_SEND_FROM_ISR_FAILED( pxQueue );
            xReturn = errQUEUE_FULL;
        }
    }
    taskEXIT_CRITICAL_FROM_ISR( uxSavedInterruptStatus );

    traceRETURN_xQueueGenericSendFromISR( xReturn );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t xQueueGiveFromISR( QueueHandle_t xQueue,
                              BaseType_t * ثابت pxHigherPriorityTaskWoken )
{
    BaseType_t xReturn;
    UBaseType_t uxSavedInterruptStatus;
    Queue_t * ثابت pxQueue = xQueue;

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
    configASSERT( !( ( pxQueue->uxQueueType == queueQUEUE_IS_MUTEX ) && ( pxQueue->u.xSemaphore.xMutexHolder != عدم ) ) );

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
        ثابت UBaseType_t uxMessagesWaiting = pxQueue->uxMessagesWaiting;

        /* When the queue is used to implement a semaphore no data is ever
         * moved through the queue but it is still valid to see if the queue 'has
         * space'. */
        إذا( uxMessagesWaiting < pxQueue->uxLength )
        {
            ثابت int8_t cTxLock = pxQueue->cTxLock;

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
            إذا( cTxLock == queueUNLOCKED )
            {
                #إذا ( configUSE_QUEUE_SETS == 1 )
                {
                    إذا( pxQueue->pxQueueSetContainer != عدم )
                    {
                        إذا( prvNotifyQueueSetContainer( pxQueue ) != pdFALSE )
                        {
                            /* The semaphore is a member of a queue set, and
                             * posting to the queue set caused a higher priority
                             * task to unblock.  A context switch is required. */
                            إذا( pxHigherPriorityTaskWoken != عدم )
                            {
                                *pxHigherPriorityTaskWoken = pdTRUE;
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
                    }
                    وإلا
                    {
                        إذا( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToReceive ) ) == pdFALSE )
                        {
                            إذا( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToReceive ) ) != pdFALSE )
                            {
                                /* The task waiting has a higher priority so
                                 *  record that a context switch is required. */
                                إذا( pxHigherPriorityTaskWoken != عدم )
                                {
                                    *pxHigherPriorityTaskWoken = pdTRUE;
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
                        }
                        وإلا
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                }
                #وإلا /* configUSE_QUEUE_SETS */
                {
                    إذا( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToReceive ) ) == pdFALSE )
                    {
                        إذا( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToReceive ) ) != pdFALSE )
                        {
                            /* The task waiting has a higher priority so record that a
                             * context switch is required. */
                            إذا( pxHigherPriorityTaskWoken != عدم )
                            {
                                *pxHigherPriorityTaskWoken = pdTRUE;
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
                    }
                    وإلا
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #endif /* configUSE_QUEUE_SETS */
            }
            وإلا
            {
                /* Increment the lock count so the task that unlocks the queue
                 * knows that data was posted while it was locked. */
                prvIncrementQueueTxLock( pxQueue, cTxLock );
            }

            xReturn = pdPASS;
        }
        وإلا
        {
            traceQUEUE_SEND_FROM_ISR_FAILED( pxQueue );
            xReturn = errQUEUE_FULL;
        }
    }
    taskEXIT_CRITICAL_FROM_ISR( uxSavedInterruptStatus );

    traceRETURN_xQueueGiveFromISR( xReturn );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t xQueueReceive( QueueHandle_t xQueue,
                          فراغ * ثابت pvBuffer,
                          TickType_t xTicksToWait )
{
    BaseType_t xEntryTimeSet = pdFALSE;
    TimeOut_t xTimeOut;
    Queue_t * ثابت pxQueue = xQueue;

    traceENTER_xQueueReceive( xQueue, pvBuffer, xTicksToWait );

    /* Check the pointer is not NULL. */
    configASSERT( ( pxQueue ) );

    /* The buffer into which data is received can only be NULL if the data size
     * is zero (so no data is copied into the buffer). */
    configASSERT( !( ( ( pvBuffer ) == عدم ) && ( ( pxQueue )->uxItemSize != ( UBaseType_t ) 0U ) ) );

    /* Cannot block if the scheduler is suspended. */
    #إذا ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( xTicksToWait != 0 ) ) );
    }
    #endif

    لكل( ; ; )
    {
        taskENTER_CRITICAL();
        {
            ثابت UBaseType_t uxMessagesWaiting = pxQueue->uxMessagesWaiting;

            /* Is there data in the queue now?  To be running the calling task
             * must be the highest priority task wanting to access the queue. */
            إذا( uxMessagesWaiting > ( UBaseType_t ) 0 )
            {
                /* Data available, remove one item. */
                prvCopyDataFromQueue( pxQueue, pvBuffer );
                traceQUEUE_RECEIVE( pxQueue );
                pxQueue->uxMessagesWaiting = ( UBaseType_t ) ( uxMessagesWaiting - ( UBaseType_t ) 1 );

                /* There is now space in the queue, were any tasks waiting to
                 * post to the queue?  If so, unblock the highest priority waiting
                 * task. */
                إذا( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToSend ) ) == pdFALSE )
                {
                    إذا( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToSend ) ) != pdFALSE )
                    {
                        queueYIELD_IF_USING_PREEMPTION();
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

                taskEXIT_CRITICAL();

                traceRETURN_xQueueReceive( pdPASS );

                أرجع pdPASS;
            }
            وإلا
            {
                إذا( xTicksToWait == ( TickType_t ) 0 )
                {
                    /* The queue was empty and no block time is specified (or
                     * the block time has expired) so leave now. */
                    taskEXIT_CRITICAL();

                    traceQUEUE_RECEIVE_FAILED( pxQueue );
                    traceRETURN_xQueueReceive( errQUEUE_EMPTY );

                    أرجع errQUEUE_EMPTY;
                }
                وإلا إذا( xEntryTimeSet == pdFALSE )
                {
                    /* The queue was empty and a block time was specified so
                     * configure the timeout structure. */
                    vTaskInternalSetTimeOutState( &xTimeOut );
                    xEntryTimeSet = pdTRUE;
                }
                وإلا
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
        إذا( xTaskCheckForTimeOut( &xTimeOut, &xTicksToWait ) == pdFALSE )
        {
            /* The timeout has not expired.  If the queue is still empty place
             * the task on the list of tasks waiting to receive from the queue. */
            إذا( prvIsQueueEmpty( pxQueue ) != pdFALSE )
            {
                traceBLOCKING_ON_QUEUE_RECEIVE( pxQueue );
                vTaskPlaceOnEventList( &( pxQueue->xTasksWaitingToReceive ), xTicksToWait );
                prvUnlockQueue( pxQueue );

                إذا( xTaskResumeAll() == pdFALSE )
                {
                    taskYIELD_WITHIN_API();
                }
                وإلا
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            وإلا
            {
                /* The queue contains data again.  Loop back to try and read the
                 * data. */
                prvUnlockQueue( pxQueue );
                ( فراغ ) xTaskResumeAll();
            }
        }
        وإلا
        {
            /* Timed out.  If there is no data in the queue exit, otherwise loop
             * back and attempt to read the data. */
            prvUnlockQueue( pxQueue );
            ( فراغ ) xTaskResumeAll();

            إذا( prvIsQueueEmpty( pxQueue ) != pdFALSE )
            {
                traceQUEUE_RECEIVE_FAILED( pxQueue );
                traceRETURN_xQueueReceive( errQUEUE_EMPTY );

                أرجع errQUEUE_EMPTY;
            }
            وإلا
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
    Queue_t * ثابت pxQueue = xQueue;

    #إذا ( configUSE_MUTEXES == 1 )
        BaseType_t xInheritanceOccurred = pdFALSE;
    #endif

    traceENTER_xQueueSemaphoreTake( xQueue, xTicksToWait );

    /* Check the queue pointer is not NULL. */
    configASSERT( ( pxQueue ) );

    /* Check this really is a semaphore, in which case the item size will be
     * 0. */
    configASSERT( pxQueue->uxItemSize == 0 );

    /* Cannot block if the scheduler is suspended. */
    #إذا ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( xTicksToWait != 0 ) ) );
    }
    #endif

    لكل( ; ; )
    {
        taskENTER_CRITICAL();
        {
            /* Semaphores are queues with an item size of 0, and where the
             * number of messages in the queue is the semaphore's count value. */
            ثابت UBaseType_t ux_Semaphore_گنتی = pxQueue->uxMessagesWaiting;

            /* Is there data in the queue now?  To be running the calling task
             * must be the highest priority task wanting to access the queue. */
            إذا( ux_Semaphore_گنتی > ( UBaseType_t ) 0 )
            {
                traceQUEUE_RECEIVE( pxQueue );

                /* Semaphores are queues with a data size of zero and where the
                 * messages waiting is the semaphore's count.  Reduce the count. */
                pxQueue->uxMessagesWaiting = ( UBaseType_t ) ( ux_Semaphore_گنتی - ( UBaseType_t ) 1 );

                #إذا ( configUSE_MUTEXES == 1 )
                {
                    إذا( pxQueue->uxQueueType == queueQUEUE_IS_MUTEX )
                    {
                        /* Record the information required to implement
                         * priority inheritance should it become necessary. */
                        pxQueue->u.xSemaphore.xMutexHolder = pv_Task_Increment_Mutex_Held_گنتی();
                    }
                    وإلا
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #endif /* configUSE_MUTEXES */

                /* Check to see if other tasks are blocked waiting to give the
                 * semaphore, and if so, unblock the highest priority such task. */
                إذا( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToSend ) ) == pdFALSE )
                {
                    إذا( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToSend ) ) != pdFALSE )
                    {
                        queueYIELD_IF_USING_PREEMPTION();
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

                taskEXIT_CRITICAL();

                traceRETURN_xQueueSemaphoreTake( pdPASS );

                أرجع pdPASS;
            }
            وإلا
            {
                إذا( xTicksToWait == ( TickType_t ) 0 )
                {
                    /* The semaphore count was 0 and no block time is specified
                     * (or the block time has expired) so exit now. */
                    taskEXIT_CRITICAL();

                    traceQUEUE_RECEIVE_FAILED( pxQueue );
                    traceRETURN_xQueueSemaphoreTake( errQUEUE_EMPTY );

                    أرجع errQUEUE_EMPTY;
                }
                وإلا إذا( xEntryTimeSet == pdFALSE )
                {
                    /* The semaphore count was 0 and a block time was specified
                     * so configure the timeout structure ready to block. */
                    vTaskInternalSetTimeOutState( &xTimeOut );
                    xEntryTimeSet = pdTRUE;
                }
                وإلا
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
        إذا( xTaskCheckForTimeOut( &xTimeOut, &xTicksToWait ) == pdFALSE )
        {
            /* A block time is specified and not expired.  If the semaphore
             * count is 0 then enter the Blocked state to wait for a semaphore to
             * become available.  As semaphores are implemented with queues the
             * queue being empty is equivalent to the semaphore count being 0. */
            إذا( prvIsQueueEmpty( pxQueue ) != pdFALSE )
            {
                traceBLOCKING_ON_QUEUE_RECEIVE( pxQueue );

                #إذا ( configUSE_MUTEXES == 1 )
                {
                    إذا( pxQueue->uxQueueType == queueQUEUE_IS_MUTEX )
                    {
                        taskENTER_CRITICAL();
                        {
                            xInheritanceOccurred = xTaskPriorityInherit( pxQueue->u.xSemaphore.xMutexHolder );
                        }
                        taskEXIT_CRITICAL();
                    }
                    وإلا
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #endif /* if ( configUSE_MUTEXES == 1 ) */

                vTaskPlaceOnEventList( &( pxQueue->xTasksWaitingToReceive ), xTicksToWait );
                prvUnlockQueue( pxQueue );

                إذا( xTaskResumeAll() == pdFALSE )
                {
                    taskYIELD_WITHIN_API();
                }
                وإلا
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            وإلا
            {
                /* There was no timeout and the semaphore count was not 0, so
                 * attempt to take the semaphore again. */
                prvUnlockQueue( pxQueue );
                ( فراغ ) xTaskResumeAll();
            }
        }
        وإلا
        {
            /* Timed out. */
            prvUnlockQueue( pxQueue );
            ( فراغ ) xTaskResumeAll();

            /* If the semaphore count is 0 exit now as the timeout has
             * expired.  Otherwise return to attempt to take the semaphore that is
             * known to be available.  As semaphores are implemented by queues the
             * queue being empty is equivalent to the semaphore count being 0. */
            إذا( prvIsQueueEmpty( pxQueue ) != pdFALSE )
            {
                #إذا ( configUSE_MUTEXES == 1 )
                {
                    /* xInheritanceOccurred could only have be set if
                     * pxQueue->uxQueueType == queueQUEUE_IS_MUTEX so no need to
                     * test the mutex type again to check it is actually a mutex. */
                    إذا( xInheritanceOccurred != pdFALSE )
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

                أرجع errQUEUE_EMPTY;
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
    }
}
/*-----------------------------------------------------------*/

BaseType_t xQueuePeek( QueueHandle_t xQueue,
                       فراغ * ثابت pvBuffer,
                       TickType_t xTicksToWait )
{
    BaseType_t xEntryTimeSet = pdFALSE;
    TimeOut_t xTimeOut;
    int8_t * pcOriginalReadPosition;
    Queue_t * ثابت pxQueue = xQueue;

    traceENTER_xQueuePeek( xQueue, pvBuffer, xTicksToWait );

    /* Check the pointer is not NULL. */
    configASSERT( ( pxQueue ) );

    /* The buffer into which data is received can only be NULL if the data size
     * is zero (so no data is copied into the buffer. */
    configASSERT( !( ( ( pvBuffer ) == عدم ) && ( ( pxQueue )->uxItemSize != ( UBaseType_t ) 0U ) ) );

    /* Cannot block if the scheduler is suspended. */
    #إذا ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( xTicksToWait != 0 ) ) );
    }
    #endif

    لكل( ; ; )
    {
        taskENTER_CRITICAL();
        {
            ثابت UBaseType_t uxMessagesWaiting = pxQueue->uxMessagesWaiting;

            /* Is there data in the queue now?  To be running the calling task
             * must be the highest priority task wanting to access the queue. */
            إذا( uxMessagesWaiting > ( UBaseType_t ) 0 )
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
                إذا( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToReceive ) ) == pdFALSE )
                {
                    إذا( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToReceive ) ) != pdFALSE )
                    {
                        /* The task waiting has a higher priority than this task. */
                        queueYIELD_IF_USING_PREEMPTION();
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

                taskEXIT_CRITICAL();

                traceRETURN_xQueuePeek( pdPASS );

                أرجع pdPASS;
            }
            وإلا
            {
                إذا( xTicksToWait == ( TickType_t ) 0 )
                {
                    /* The queue was empty and no block time is specified (or
                     * the block time has expired) so leave now. */
                    taskEXIT_CRITICAL();

                    traceQUEUE_PEEK_FAILED( pxQueue );
                    traceRETURN_xQueuePeek( errQUEUE_EMPTY );

                    أرجع errQUEUE_EMPTY;
                }
                وإلا إذا( xEntryTimeSet == pdFALSE )
                {
                    /* The queue was empty and a block time was specified so
                     * configure the timeout structure ready to enter the blocked
                     * state. */
                    vTaskInternalSetTimeOutState( &xTimeOut );
                    xEntryTimeSet = pdTRUE;
                }
                وإلا
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
        إذا( xTaskCheckForTimeOut( &xTimeOut, &xTicksToWait ) == pdFALSE )
        {
            /* Timeout has not expired yet, check to see if there is data in the
            * queue now, and if not enter the Blocked state to wait for data. */
            إذا( prvIsQueueEmpty( pxQueue ) != pdFALSE )
            {
                traceBLOCKING_ON_QUEUE_PEEK( pxQueue );
                vTaskPlaceOnEventList( &( pxQueue->xTasksWaitingToReceive ), xTicksToWait );
                prvUnlockQueue( pxQueue );

                إذا( xTaskResumeAll() == pdFALSE )
                {
                    taskYIELD_WITHIN_API();
                }
                وإلا
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            وإلا
            {
                /* There is data in the queue now, so don't enter the blocked
                 * state, instead return to try and obtain the data. */
                prvUnlockQueue( pxQueue );
                ( فراغ ) xTaskResumeAll();
            }
        }
        وإلا
        {
            /* The timeout has expired.  If there is still no data in the queue
             * exit, otherwise go back and try to read the data again. */
            prvUnlockQueue( pxQueue );
            ( فراغ ) xTaskResumeAll();

            إذا( prvIsQueueEmpty( pxQueue ) != pdFALSE )
            {
                traceQUEUE_PEEK_FAILED( pxQueue );
                traceRETURN_xQueuePeek( errQUEUE_EMPTY );

                أرجع errQUEUE_EMPTY;
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
    }
}
/*-----------------------------------------------------------*/

BaseType_t xQueueReceiveFromISR( QueueHandle_t xQueue,
                                 فراغ * ثابت pvBuffer,
                                 BaseType_t * ثابت pxHigherPriorityTaskWoken )
{
    BaseType_t xReturn;
    UBaseType_t uxSavedInterruptStatus;
    Queue_t * ثابت pxQueue = xQueue;

    traceENTER_xQueueReceiveFromISR( xQueue, pvBuffer, pxHigherPriorityTaskWoken );

    configASSERT( pxQueue );
    configASSERT( !( ( pvBuffer == عدم ) && ( pxQueue->uxItemSize != ( UBaseType_t ) 0U ) ) );

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
        ثابت UBaseType_t uxMessagesWaiting = pxQueue->uxMessagesWaiting;

        /* Cannot block in an ISR, so check there is data available. */
        إذا( uxMessagesWaiting > ( UBaseType_t ) 0 )
        {
            ثابت int8_t cRxLock = pxQueue->cRxLock;

            traceQUEUE_RECEIVE_FROM_ISR( pxQueue );

            prvCopyDataFromQueue( pxQueue, pvBuffer );
            pxQueue->uxMessagesWaiting = ( UBaseType_t ) ( uxMessagesWaiting - ( UBaseType_t ) 1 );

            /* If the queue is locked the event list will not be modified.
             * Instead update the lock count so the task that unlocks the queue
             * will know that an ISR has removed data while the queue was
             * locked. */
            إذا( cRxLock == queueUNLOCKED )
            {
                إذا( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToSend ) ) == pdFALSE )
                {
                    إذا( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToSend ) ) != pdFALSE )
                    {
                        /* The task waiting has a higher priority than us so
                         * force a context switch. */
                        إذا( pxHigherPriorityTaskWoken != عدم )
                        {
                            *pxHigherPriorityTaskWoken = pdTRUE;
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
                }
                وإلا
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            وإلا
            {
                /* Increment the lock count so the task that unlocks the queue
                 * knows that data was removed while it was locked. */
                prvIncrementQueueRxLock( pxQueue, cRxLock );
            }

            xReturn = pdPASS;
        }
        وإلا
        {
            xReturn = pdFAIL;
            traceQUEUE_RECEIVE_FROM_ISR_FAILED( pxQueue );
        }
    }
    taskEXIT_CRITICAL_FROM_ISR( uxSavedInterruptStatus );

    traceRETURN_xQueueReceiveFromISR( xReturn );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t xQueuePeekFromISR( QueueHandle_t xQueue,
                              فراغ * ثابت pvBuffer )
{
    BaseType_t xReturn;
    UBaseType_t uxSavedInterruptStatus;
    int8_t * pcOriginalReadPosition;
    Queue_t * ثابت pxQueue = xQueue;

    traceENTER_xQueuePeekFromISR( xQueue, pvBuffer );

    configASSERT( pxQueue );
    configASSERT( !( ( pvBuffer == عدم ) && ( pxQueue->uxItemSize != ( UBaseType_t ) 0U ) ) );
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
        إذا( pxQueue->uxMessagesWaiting > ( UBaseType_t ) 0 )
        {
            traceQUEUE_PEEK_FROM_ISR( pxQueue );

            /* Remember the read position so it can be reset as nothing is
             * actually being removed from the queue. */
            pcOriginalReadPosition = pxQueue->u.xQueue.pcReadFrom;
            prvCopyDataFromQueue( pxQueue, pvBuffer );
            pxQueue->u.xQueue.pcReadFrom = pcOriginalReadPosition;

            xReturn = pdPASS;
        }
        وإلا
        {
            xReturn = pdFAIL;
            traceQUEUE_PEEK_FROM_ISR_FAILED( pxQueue );
        }
    }
    taskEXIT_CRITICAL_FROM_ISR( uxSavedInterruptStatus );

    traceRETURN_xQueuePeekFromISR( xReturn );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

UBaseType_t uxQueueMessagesWaiting( ثابت QueueHandle_t xQueue )
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

    أرجع uxReturn;
}
/*-----------------------------------------------------------*/

UBaseType_t uxQueueSpacesAvailable( ثابت QueueHandle_t xQueue )
{
    UBaseType_t uxReturn;
    Queue_t * ثابت pxQueue = xQueue;

    traceENTER_uxQueueSpacesAvailable( xQueue );

    configASSERT( pxQueue );

    taskENTER_CRITICAL();
    {
        uxReturn = ( UBaseType_t ) ( pxQueue->uxLength - pxQueue->uxMessagesWaiting );
    }
    taskEXIT_CRITICAL();

    traceRETURN_uxQueueSpacesAvailable( uxReturn );

    أرجع uxReturn;
}
/*-----------------------------------------------------------*/

UBaseType_t uxQueueMessagesWaitingFromISR( ثابت QueueHandle_t xQueue )
{
    UBaseType_t uxReturn;
    Queue_t * ثابت pxQueue = xQueue;

    traceENTER_uxQueueMessagesWaitingFromISR( xQueue );

    configASSERT( pxQueue );
    uxReturn = pxQueue->uxMessagesWaiting;

    traceRETURN_uxQueueMessagesWaitingFromISR( uxReturn );

    أرجع uxReturn;
}
/*-----------------------------------------------------------*/

فراغ vQueueDelete( QueueHandle_t xQueue )
{
    Queue_t * ثابت pxQueue = xQueue;

    traceENTER_vQueueDelete( xQueue );

    configASSERT( pxQueue );
    traceQUEUE_DELETE( pxQueue );

    #إذا ( configQUEUE_REGISTRY_SIZE > 0 )
    {
        vQueueUnregisterQueue( pxQueue );
    }
    #endif

    #إذا ( ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) && ( configSUPPORT_STATIC_ALLOCATION == 0 ) )
    {
        /* The queue can only have been allocated dynamically - free it
         * again. */
        vPortFree( pxQueue );
    }
    #وإلاإذا ( ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) && ( configSUPPORT_STATIC_ALLOCATION == 1 ) )
    {
        /* The queue could have been allocated statically or dynamically, so
         * check before attempting to free the memory. */
        إذا( pxQueue->ucStaticallyAllocated == ( uint8_t ) pdFALSE )
        {
            vPortFree( pxQueue );
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    #وإلا /* if ( ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) && ( configSUPPORT_STATIC_ALLOCATION == 0 ) ) */
    {
        /* The queue must have been statically allocated, so is not going to be
         * deleted.  Avoid compiler warnings about the unused parameter. */
        ( فراغ ) pxQueue;
    }
    #endif /* configSUPPORT_DYNAMIC_ALLOCATION */

    traceRETURN_vQueueDelete();
}
/*-----------------------------------------------------------*/

#إذا ( configUSE_TRACE_FACILITY == 1 )

    UBaseType_t ux_Queue_Get_Queue_نمبر( QueueHandle_t xQueue )
    {
        trace_ENTER_ux_Queue_Get_Queue_نمبر( xQueue );

        trace_RETURN_ux_Queue_Get_Queue_نمبر( ( ( Queue_t * ) xQueue )->ux_Queue_نمبر );

        أرجع ( ( Queue_t * ) xQueue )->ux_Queue_نمبر;
    }

#endif /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#إذا ( configUSE_TRACE_FACILITY == 1 )

    فراغ v_Queue_Set_Queue_نمبر( QueueHandle_t xQueue,
                               UBaseType_t ux_Queue_نمبر )
    {
        trace_ENTER_v_Queue_Set_Queue_نمبر( xQueue, ux_Queue_نمبر );

        ( ( Queue_t * ) xQueue )->ux_Queue_نمبر = ux_Queue_نمبر;

        trace_RETURN_v_Queue_Set_Queue_نمبر();
    }

#endif /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#إذا ( configUSE_TRACE_FACILITY == 1 )

    uint8_t ucQueueGetQueueType( QueueHandle_t xQueue )
    {
        traceENTER_ucQueueGetQueueType( xQueue );

        traceRETURN_ucQueueGetQueueType( ( ( Queue_t * ) xQueue )->ucQueueType );

        أرجع ( ( Queue_t * ) xQueue )->ucQueueType;
    }

#endif /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

UBaseType_t uxQueueGetQueueItemSize( QueueHandle_t xQueue ) /* PRIVILEGED_FUNCTION */
{
    traceENTER_uxQueueGetQueueItemSize( xQueue );

    traceRETURN_uxQueueGetQueueItemSize( ( ( Queue_t * ) xQueue )->uxItemSize );

    أرجع ( ( Queue_t * ) xQueue )->uxItemSize;
}
/*-----------------------------------------------------------*/

UBaseType_t uxQueueGetQueueLength( QueueHandle_t xQueue ) /* PRIVILEGED_FUNCTION */
{
    traceENTER_uxQueueGetQueueLength( xQueue );

    traceRETURN_uxQueueGetQueueLength( ( ( Queue_t * ) xQueue )->uxLength );

    أرجع ( ( Queue_t * ) xQueue )->uxLength;
}
/*-----------------------------------------------------------*/

#إذا ( configUSE_MUTEXES == 1 )

    ساكن UBaseType_t prvGetDisinheritPriorityAfterTimeout( ثابت Queue_t * ثابت pxQueue )
    {
        UBaseType_t uxHighestPriorityOfWaitingTasks;

        /* If a task waiting for a mutex causes the mutex holder to inherit a
         * priority, but the waiting task times out, then the holder should
         * disinherit the priority - but only down to the highest priority of any
         * other tasks that are waiting for the same mutex.  For this purpose,
         * return the priority of the highest priority task that is waiting for the
         * mutex. */
        إذا( listCURRENT_LIST_LENGTH( &( pxQueue->xTasksWaitingToReceive ) ) > 0U )
        {
            uxHighestPriorityOfWaitingTasks = ( UBaseType_t ) ( ( UBaseType_t ) configMAX_PRIORITIES - ( UBaseType_t ) list_GET_ITEM_قدر_OF_HEAD_ENTRY( &( pxQueue->xTasksWaitingToReceive ) ) );
        }
        وإلا
        {
            uxHighestPriorityOfWaitingTasks = tskIDLE_PRIORITY;
        }

        أرجع uxHighestPriorityOfWaitingTasks;
    }

#endif /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

ساكن BaseType_t prvCopyDataToQueue( Queue_t * ثابت pxQueue,
                                      ثابت فراغ * pvItemToQueue,
                                      ثابت BaseType_t xPosition )
{
    BaseType_t xReturn = pdFALSE;
    UBaseType_t uxMessagesWaiting;

    /* This function is called from a critical section. */

    uxMessagesWaiting = pxQueue->uxMessagesWaiting;

    إذا( pxQueue->uxItemSize == ( UBaseType_t ) 0 )
    {
        #إذا ( configUSE_MUTEXES == 1 )
        {
            إذا( pxQueue->uxQueueType == queueQUEUE_IS_MUTEX )
            {
                /* The mutex is no longer being held. */
                xReturn = xTaskPriorityDisinherit( pxQueue->u.xSemaphore.xMutexHolder );
                pxQueue->u.xSemaphore.xMutexHolder = عدم;
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        #endif /* configUSE_MUTEXES */
    }
    وإلا إذا( xPosition == queueSEND_TO_BACK )
    {
        ( فراغ ) memcpy( ( فراغ * ) pxQueue->pcWriteTo, pvItemToQueue, ( size_t ) pxQueue->uxItemSize );
        pxQueue->pcWriteTo += pxQueue->uxItemSize;

        إذا( pxQueue->pcWriteTo >= pxQueue->u.xQueue.pcTail )
        {
            pxQueue->pcWriteTo = pxQueue->pcHead;
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    وإلا
    {
        ( فراغ ) memcpy( ( فراغ * ) pxQueue->u.xQueue.pcReadFrom, pvItemToQueue, ( size_t ) pxQueue->uxItemSize );
        pxQueue->u.xQueue.pcReadFrom -= pxQueue->uxItemSize;

        إذا( pxQueue->u.xQueue.pcReadFrom < pxQueue->pcHead )
        {
            pxQueue->u.xQueue.pcReadFrom = ( pxQueue->u.xQueue.pcTail - pxQueue->uxItemSize );
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }

        إذا( xPosition == queueOVERWRITE )
        {
            إذا( uxMessagesWaiting > ( UBaseType_t ) 0 )
            {
                /* An item is not being added but overwritten, so subtract
                 * one from the recorded number of items in the queue so when
                 * one is added again below the number of recorded items remains
                 * correct. */
                --uxMessagesWaiting;
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
    }

    pxQueue->uxMessagesWaiting = ( UBaseType_t ) ( uxMessagesWaiting + ( UBaseType_t ) 1 );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

ساكن فراغ prvCopyDataFromQueue( Queue_t * ثابت pxQueue,
                                  فراغ * ثابت pvBuffer )
{
    إذا( pxQueue->uxItemSize != ( UBaseType_t ) 0 )
    {
        pxQueue->u.xQueue.pcReadFrom += pxQueue->uxItemSize;

        إذا( pxQueue->u.xQueue.pcReadFrom >= pxQueue->u.xQueue.pcTail )
        {
            pxQueue->u.xQueue.pcReadFrom = pxQueue->pcHead;
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }

        ( فراغ ) memcpy( ( فراغ * ) pvBuffer, ( فراغ * ) pxQueue->u.xQueue.pcReadFrom, ( size_t ) pxQueue->uxItemSize );
    }
}
/*-----------------------------------------------------------*/

ساكن فراغ prvUnlockQueue( Queue_t * ثابت pxQueue )
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
        طالما( cTxLock > queueLOCKED_UNMODIFIED )
        {
            /* Data was posted while the queue was locked.  Are any tasks
             * blocked waiting for data to become available? */
            #إذا ( configUSE_QUEUE_SETS == 1 )
            {
                إذا( pxQueue->pxQueueSetContainer != عدم )
                {
                    إذا( prvNotifyQueueSetContainer( pxQueue ) != pdFALSE )
                    {
                        /* The queue is a member of a queue set, and posting to
                         * the queue set caused a higher priority task to unblock.
                         * A context switch is required. */
                        vTaskMissedYield();
                    }
                    وإلا
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                وإلا
                {
                    /* Tasks that are removed from the event list will get
                     * added to the pending ready list as the scheduler is still
                     * suspended. */
                    إذا( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToReceive ) ) == pdFALSE )
                    {
                        إذا( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToReceive ) ) != pdFALSE )
                        {
                            /* The task waiting has a higher priority so record that a
                             * context switch is required. */
                            vTaskMissedYield();
                        }
                        وإلا
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    وإلا
                    {
                        اكسر;
                    }
                }
            }
            #وإلا /* configUSE_QUEUE_SETS */
            {
                /* Tasks that are removed from the event list will get added to
                 * the pending ready list as the scheduler is still suspended. */
                إذا( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToReceive ) ) == pdFALSE )
                {
                    إذا( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToReceive ) ) != pdFALSE )
                    {
                        /* The task waiting has a higher priority so record that
                         * a context switch is required. */
                        vTaskMissedYield();
                    }
                    وإلا
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                وإلا
                {
                    اكسر;
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

        طالما( cRxLock > queueLOCKED_UNMODIFIED )
        {
            إذا( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToSend ) ) == pdFALSE )
            {
                إذا( xTaskRemoveFromEventList( &( pxQueue->xTasksWaitingToSend ) ) != pdFALSE )
                {
                    vTaskMissedYield();
                }
                وإلا
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                --cRxLock;
            }
            وإلا
            {
                اكسر;
            }
        }

        pxQueue->cRxLock = queueUNLOCKED;
    }
    taskEXIT_CRITICAL();
}
/*-----------------------------------------------------------*/

ساكن BaseType_t prvIsQueueEmpty( ثابت Queue_t * pxQueue )
{
    BaseType_t xReturn;

    taskENTER_CRITICAL();
    {
        إذا( pxQueue->uxMessagesWaiting == ( UBaseType_t ) 0 )
        {
            xReturn = pdTRUE;
        }
        وإلا
        {
            xReturn = pdFALSE;
        }
    }
    taskEXIT_CRITICAL();

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t xQueueIsQueueEmptyFromISR( ثابت QueueHandle_t xQueue )
{
    BaseType_t xReturn;
    Queue_t * ثابت pxQueue = xQueue;

    traceENTER_xQueueIsQueueEmptyFromISR( xQueue );

    configASSERT( pxQueue );

    إذا( pxQueue->uxMessagesWaiting == ( UBaseType_t ) 0 )
    {
        xReturn = pdTRUE;
    }
    وإلا
    {
        xReturn = pdFALSE;
    }

    traceRETURN_xQueueIsQueueEmptyFromISR( xReturn );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

ساكن BaseType_t prvIsQueueFull( ثابت Queue_t * pxQueue )
{
    BaseType_t xReturn;

    taskENTER_CRITICAL();
    {
        إذا( pxQueue->uxMessagesWaiting == pxQueue->uxLength )
        {
            xReturn = pdTRUE;
        }
        وإلا
        {
            xReturn = pdFALSE;
        }
    }
    taskEXIT_CRITICAL();

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

BaseType_t xQueueIsQueueFullFromISR( ثابت QueueHandle_t xQueue )
{
    BaseType_t xReturn;
    Queue_t * ثابت pxQueue = xQueue;

    traceENTER_xQueueIsQueueFullFromISR( xQueue );

    configASSERT( pxQueue );

    إذا( pxQueue->uxMessagesWaiting == pxQueue->uxLength )
    {
        xReturn = pdTRUE;
    }
    وإلا
    {
        xReturn = pdFALSE;
    }

    traceRETURN_xQueueIsQueueFullFromISR( xReturn );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

#إذا ( configUSE_CO_ROUTINES == 1 )

    BaseType_t xQueueCRSend( QueueHandle_t xQueue,
                             ثابت فراغ * pvItemToQueue,
                             TickType_t xTicksToWait )
    {
        BaseType_t xReturn;
        Queue_t * ثابت pxQueue = xQueue;

        traceENTER_xQueueCRSend( xQueue, pvItemToQueue, xTicksToWait );

        /* If the queue is already full we may have to block.  A critical section
         * is required to prevent an interrupt removing something from the queue
         * between the check to see if the queue is full and blocking on the queue. */
        portDISABLE_INTERRUPTS();
        {
            إذا( prvIsQueueFull( pxQueue ) != pdFALSE )
            {
                /* The queue is full - do we want to block or just leave without
                 * posting? */
                إذا( xTicksToWait > ( TickType_t ) 0 )
                {
                    /* As this is called from a coroutine we cannot block directly, but
                     * return indicating that we need to block. */
                    vCoRoutineAddToDelayedList( xTicksToWait, &( pxQueue->xTasksWaitingToSend ) );
                    portENABLE_INTERRUPTS();
                    أرجع errQUEUE_BLOCKED;
                }
                وإلا
                {
                    portENABLE_INTERRUPTS();
                    أرجع errQUEUE_FULL;
                }
            }
        }
        portENABLE_INTERRUPTS();

        portDISABLE_INTERRUPTS();
        {
            إذا( pxQueue->uxMessagesWaiting < pxQueue->uxLength )
            {
                /* There is room in the queue, copy the data into the queue. */
                prvCopyDataToQueue( pxQueue, pvItemToQueue, queueSEND_TO_BACK );
                xReturn = pdPASS;

                /* Were any co-routines waiting for data to become available? */
                إذا( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToReceive ) ) == pdFALSE )
                {
                    /* In this instance the co-routine could be placed directly
                     * into the ready list as we are within a critical section.
                     * Instead the same pending ready list mechanism is used as if
                     * the event were caused from within an interrupt. */
                    إذا( xCoRoutineRemoveFromEventList( &( pxQueue->xTasksWaitingToReceive ) ) != pdFALSE )
                    {
                        /* The co-routine waiting has a higher priority so record
                         * that a yield might be appropriate. */
                        xReturn = errQUEUE_YIELD;
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
            }
            وإلا
            {
                xReturn = errQUEUE_FULL;
            }
        }
        portENABLE_INTERRUPTS();

        traceRETURN_xQueueCRSend( xReturn );

        أرجع xReturn;
    }

#endif /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#إذا ( configUSE_CO_ROUTINES == 1 )

    BaseType_t xQueueCRReceive( QueueHandle_t xQueue,
                                فراغ * pvBuffer,
                                TickType_t xTicksToWait )
    {
        BaseType_t xReturn;
        Queue_t * ثابت pxQueue = xQueue;

        traceENTER_xQueueCRReceive( xQueue, pvBuffer, xTicksToWait );

        /* If the queue is already empty we may have to block.  A critical section
         * is required to prevent an interrupt adding something to the queue
         * between the check to see if the queue is empty and blocking on the queue. */
        portDISABLE_INTERRUPTS();
        {
            إذا( pxQueue->uxMessagesWaiting == ( UBaseType_t ) 0 )
            {
                /* There are no messages in the queue, do we want to block or just
                 * leave with nothing? */
                إذا( xTicksToWait > ( TickType_t ) 0 )
                {
                    /* As this is a co-routine we cannot block directly, but return
                     * indicating that we need to block. */
                    vCoRoutineAddToDelayedList( xTicksToWait, &( pxQueue->xTasksWaitingToReceive ) );
                    portENABLE_INTERRUPTS();
                    أرجع errQUEUE_BLOCKED;
                }
                وإلا
                {
                    portENABLE_INTERRUPTS();
                    أرجع errQUEUE_FULL;
                }
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        portENABLE_INTERRUPTS();

        portDISABLE_INTERRUPTS();
        {
            إذا( pxQueue->uxMessagesWaiting > ( UBaseType_t ) 0 )
            {
                /* Data is available from the queue. */
                pxQueue->u.xQueue.pcReadFrom += pxQueue->uxItemSize;

                إذا( pxQueue->u.xQueue.pcReadFrom >= pxQueue->u.xQueue.pcTail )
                {
                    pxQueue->u.xQueue.pcReadFrom = pxQueue->pcHead;
                }
                وإلا
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                --( pxQueue->uxMessagesWaiting );
                ( فراغ ) memcpy( ( فراغ * ) pvBuffer, ( فراغ * ) pxQueue->u.xQueue.pcReadFrom, ( unsigned ) pxQueue->uxItemSize );

                xReturn = pdPASS;

                /* Were any co-routines waiting for space to become available? */
                إذا( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToSend ) ) == pdFALSE )
                {
                    /* In this instance the co-routine could be placed directly
                     * into the ready list as we are within a critical section.
                     * Instead the same pending ready list mechanism is used as if
                     * the event were caused from within an interrupt. */
                    إذا( xCoRoutineRemoveFromEventList( &( pxQueue->xTasksWaitingToSend ) ) != pdFALSE )
                    {
                        xReturn = errQUEUE_YIELD;
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
            }
            وإلا
            {
                xReturn = pdFAIL;
            }
        }
        portENABLE_INTERRUPTS();

        traceRETURN_xQueueCRReceive( xReturn );

        أرجع xReturn;
    }

#endif /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#إذا ( configUSE_CO_ROUTINES == 1 )

    BaseType_t xQueueCRSendFromISR( QueueHandle_t xQueue,
                                    ثابت فراغ * pvItemToQueue,
                                    BaseType_t xCoRoutinePreviouslyWoken )
    {
        Queue_t * ثابت pxQueue = xQueue;

        traceENTER_xQueueCRSendFromISR( xQueue, pvItemToQueue, xCoRoutinePreviouslyWoken );

        /* Cannot block within an ISR so if there is no space on the queue then
         * exit without doing anything. */
        إذا( pxQueue->uxMessagesWaiting < pxQueue->uxLength )
        {
            prvCopyDataToQueue( pxQueue, pvItemToQueue, queueSEND_TO_BACK );

            /* We only want to wake one co-routine per ISR, so check that a
             * co-routine has not already been woken. */
            إذا( xCoRoutinePreviouslyWoken == pdFALSE )
            {
                إذا( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToReceive ) ) == pdFALSE )
                {
                    إذا( xCoRoutineRemoveFromEventList( &( pxQueue->xTasksWaitingToReceive ) ) != pdFALSE )
                    {
                        أرجع pdTRUE;
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

        traceRETURN_xQueueCRSendFromISR( xCoRoutinePreviouslyWoken );

        أرجع xCoRoutinePreviouslyWoken;
    }

#endif /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#إذا ( configUSE_CO_ROUTINES == 1 )

    BaseType_t xQueueCRReceiveFromISR( QueueHandle_t xQueue,
                                       فراغ * pvBuffer,
                                       BaseType_t * pxCoRoutineWoken )
    {
        BaseType_t xReturn;
        Queue_t * ثابت pxQueue = xQueue;

        traceENTER_xQueueCRReceiveFromISR( xQueue, pvBuffer, pxCoRoutineWoken );

        /* We cannot block from an ISR, so check there is data available. If
         * not then just leave without doing anything. */
        إذا( pxQueue->uxMessagesWaiting > ( UBaseType_t ) 0 )
        {
            /* Copy the data from the queue. */
            pxQueue->u.xQueue.pcReadFrom += pxQueue->uxItemSize;

            إذا( pxQueue->u.xQueue.pcReadFrom >= pxQueue->u.xQueue.pcTail )
            {
                pxQueue->u.xQueue.pcReadFrom = pxQueue->pcHead;
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }

            --( pxQueue->uxMessagesWaiting );
            ( فراغ ) memcpy( ( فراغ * ) pvBuffer, ( فراغ * ) pxQueue->u.xQueue.pcReadFrom, ( unsigned ) pxQueue->uxItemSize );

            إذا( ( *pxCoRoutineWoken ) == pdFALSE )
            {
                إذا( listLIST_IS_EMPTY( &( pxQueue->xTasksWaitingToSend ) ) == pdFALSE )
                {
                    إذا( xCoRoutineRemoveFromEventList( &( pxQueue->xTasksWaitingToSend ) ) != pdFALSE )
                    {
                        *pxCoRoutineWoken = pdTRUE;
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
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }

            xReturn = pdPASS;
        }
        وإلا
        {
            xReturn = pdFAIL;
        }

        traceRETURN_xQueueCRReceiveFromISR( xReturn );

        أرجع xReturn;
    }

#endif /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#إذا ( configQUEUE_REGISTRY_SIZE > 0 )

    فراغ vQueueAddToRegistry( QueueHandle_t xQueue,
                              ثابت حرف * pcQueueName )
    {
        UBaseType_t ux;
        QueueRegistryItem_t * pxEntryToWrite = عدم;

        traceENTER_vQueueAddToRegistry( xQueue, pcQueueName );

        configASSERT( xQueue );

        إذا( pcQueueName != عدم )
        {
            /* See if there is an empty space in the registry.  A NULL name denotes
             * a free slot. */
            لكل( ux = ( UBaseType_t ) 0U; ux < ( UBaseType_t ) configQUEUE_REGISTRY_SIZE; ux++ )
            {
                /* Replace an existing entry if the queue is already in the registry. */
                إذا( xQueue == xQueueRegistry[ ux ].xHandle )
                {
                    pxEntryToWrite = &( xQueueRegistry[ ux ] );
                    اكسر;
                }
                /* Otherwise, store in the next empty location */
                وإلا إذا( ( pxEntryToWrite == عدم ) && ( xQueueRegistry[ ux ].pcQueueName == عدم ) )
                {
                    pxEntryToWrite = &( xQueueRegistry[ ux ] );
                }
                وإلا
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
        }

        إذا( pxEntryToWrite != عدم )
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

#إذا ( configQUEUE_REGISTRY_SIZE > 0 )

    ثابت حرف * pcQueueGetName( QueueHandle_t xQueue )
    {
        UBaseType_t ux;
        ثابت حرف * pcReturn = عدم;

        traceENTER_pcQueueGetName( xQueue );

        configASSERT( xQueue );

        /* Note there is nothing here to protect against another task adding or
         * removing entries from the registry while it is being searched. */

        لكل( ux = ( UBaseType_t ) 0U; ux < ( UBaseType_t ) configQUEUE_REGISTRY_SIZE; ux++ )
        {
            إذا( xQueueRegistry[ ux ].xHandle == xQueue )
            {
                pcReturn = xQueueRegistry[ ux ].pcQueueName;
                اكسر;
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }

        traceRETURN_pcQueueGetName( pcReturn );

        أرجع pcReturn;
    }

#endif /* configQUEUE_REGISTRY_SIZE */
/*-----------------------------------------------------------*/

#إذا ( configQUEUE_REGISTRY_SIZE > 0 )

    فراغ vQueueUnregisterQueue( QueueHandle_t xQueue )
    {
        UBaseType_t ux;

        traceENTER_vQueueUnregisterQueue( xQueue );

        configASSERT( xQueue );

        /* See if the handle of the queue being unregistered in actually in the
         * registry. */
        لكل( ux = ( UBaseType_t ) 0U; ux < ( UBaseType_t ) configQUEUE_REGISTRY_SIZE; ux++ )
        {
            إذا( xQueueRegistry[ ux ].xHandle == xQueue )
            {
                /* Set the name to NULL to show that this slot if free again. */
                xQueueRegistry[ ux ].pcQueueName = عدم;

                /* Set the handle to NULL to ensure the same queue handle cannot
                 * appear in the registry twice if it is added, removed, then
                 * added again. */
                xQueueRegistry[ ux ].xHandle = ( QueueHandle_t ) 0;
                اكسر;
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }

        traceRETURN_vQueueUnregisterQueue();
    }

#endif /* configQUEUE_REGISTRY_SIZE */
/*-----------------------------------------------------------*/

#إذا ( configUSE_TIMERS == 1 )

    فراغ vQueueWaitForMessageRestricted( QueueHandle_t xQueue,
                                         TickType_t xTicksToWait,
                                         ثابت BaseType_t xWaitIndefinitely )
    {
        Queue_t * ثابت pxQueue = xQueue;

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

        إذا( pxQueue->uxMessagesWaiting == ( UBaseType_t ) 0U )
        {
            /* There is nothing in the queue, block for the specified period. */
            vTaskPlaceOnEventListRestricted( &( pxQueue->xTasksWaitingToReceive ), xTicksToWait, xWaitIndefinitely );
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }

        prvUnlockQueue( pxQueue );

        traceRETURN_vQueueWaitForMessageRestricted();
    }

#endif /* configUSE_TIMERS */
/*-----------------------------------------------------------*/

#إذا ( ( configUSE_QUEUE_SETS == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) )

    QueueSetHandle_t xQueueCreateSet( ثابت UBaseType_t uxEventQueueLength )
    {
        QueueSetHandle_t pxQueue;

        traceENTER_xQueueCreateSet( uxEventQueueLength );

        pxQueue = xQueueGenericCreate( uxEventQueueLength, ( UBaseType_t ) حجم( Queue_t * ), queueQUEUE_TYPE_SET );

        traceRETURN_xQueueCreateSet( pxQueue );

        أرجع pxQueue;
    }

#endif /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#إذا ( configUSE_QUEUE_SETS == 1 )

    BaseType_t xQueueAddToSet( QueueSetMemberHandle_t xQueueOrSemaphore,
                               QueueSetHandle_t xQueueSet )
    {
        BaseType_t xReturn;

        traceENTER_xQueueAddToSet( xQueueOrSemaphore, xQueueSet );

        taskENTER_CRITICAL();
        {
            إذا( ( ( Queue_t * ) xQueueOrSemaphore )->pxQueueSetContainer != عدم )
            {
                /* Cannot add a queue/semaphore to more than one queue set. */
                xReturn = pdFAIL;
            }
            وإلا إذا( ( ( Queue_t * ) xQueueOrSemaphore )->uxMessagesWaiting != ( UBaseType_t ) 0 )
            {
                /* Cannot add a queue/semaphore to a queue set if there are already
                 * items in the queue/semaphore. */
                xReturn = pdFAIL;
            }
            وإلا
            {
                ( ( Queue_t * ) xQueueOrSemaphore )->pxQueueSetContainer = xQueueSet;
                xReturn = pdPASS;
            }
        }
        taskEXIT_CRITICAL();

        traceRETURN_xQueueAddToSet( xReturn );

        أرجع xReturn;
    }

#endif /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#إذا ( configUSE_QUEUE_SETS == 1 )

    BaseType_t xQueueRemoveFromSet( QueueSetMemberHandle_t xQueueOrSemaphore,
                                    QueueSetHandle_t xQueueSet )
    {
        BaseType_t xReturn;
        Queue_t * ثابت pxQueueOrSemaphore = ( Queue_t * ) xQueueOrSemaphore;

        traceENTER_xQueueRemoveFromSet( xQueueOrSemaphore, xQueueSet );

        إذا( pxQueueOrSemaphore->pxQueueSetContainer != xQueueSet )
        {
            /* The queue was not a member of the set. */
            xReturn = pdFAIL;
        }
        وإلا إذا( pxQueueOrSemaphore->uxMessagesWaiting != ( UBaseType_t ) 0 )
        {
            /* It is dangerous to remove a queue from a set when the queue is
             * not empty because the queue set will still hold pending events for
             * the queue. */
            xReturn = pdFAIL;
        }
        وإلا
        {
            taskENTER_CRITICAL();
            {
                /* The queue is no longer contained in the set. */
                pxQueueOrSemaphore->pxQueueSetContainer = عدم;
            }
            taskEXIT_CRITICAL();
            xReturn = pdPASS;
        }

        traceRETURN_xQueueRemoveFromSet( xReturn );

        أرجع xReturn;
    }

#endif /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#إذا ( configUSE_QUEUE_SETS == 1 )

    QueueSetMemberHandle_t xQueueSelectFromSet( QueueSetHandle_t xQueueSet,
                                                TickType_t ثابت xTicksToWait )
    {
        QueueSetMemberHandle_t xReturn = عدم;

        traceENTER_xQueueSelectFromSet( xQueueSet, xTicksToWait );

        ( فراغ ) xQueueReceive( ( QueueHandle_t ) xQueueSet, &xReturn, xTicksToWait );

        traceRETURN_xQueueSelectFromSet( xReturn );

        أرجع xReturn;
    }

#endif /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#إذا ( configUSE_QUEUE_SETS == 1 )

    QueueSetMemberHandle_t xQueueSelectFromSetFromISR( QueueSetHandle_t xQueueSet )
    {
        QueueSetMemberHandle_t xReturn = عدم;

        traceENTER_xQueueSelectFromSetFromISR( xQueueSet );

        ( فراغ ) xQueueReceiveFromISR( ( QueueHandle_t ) xQueueSet, &xReturn, عدم );

        traceRETURN_xQueueSelectFromSetFromISR( xReturn );

        أرجع xReturn;
    }

#endif /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#إذا ( configUSE_QUEUE_SETS == 1 )

    ساكن BaseType_t prvNotifyQueueSetContainer( ثابت Queue_t * ثابت pxQueue )
    {
        Queue_t * pxQueueSetContainer = pxQueue->pxQueueSetContainer;
        BaseType_t xReturn = pdFALSE;

        /* This function must be called form a critical section. */

        /* The following line is not reachable in unit tests because every call
         * to prvNotifyQueueSetContainer is preceded by a check that
         * pxQueueSetContainer != NULL */
        configASSERT( pxQueueSetContainer ); /* LCOV_EXCL_BR_LINE */
        configASSERT( pxQueueSetContainer->uxMessagesWaiting < pxQueueSetContainer->uxLength );

        إذا( pxQueueSetContainer->uxMessagesWaiting < pxQueueSetContainer->uxLength )
        {
            ثابت int8_t cTxLock = pxQueueSetContainer->cTxLock;

            traceQUEUE_SET_SEND( pxQueueSetContainer );

            /* The data copied is the handle of the queue that contains data. */
            xReturn = prvCopyDataToQueue( pxQueueSetContainer, &pxQueue, queueSEND_TO_BACK );

            إذا( cTxLock == queueUNLOCKED )
            {
                إذا( listLIST_IS_EMPTY( &( pxQueueSetContainer->xTasksWaitingToReceive ) ) == pdFALSE )
                {
                    إذا( xTaskRemoveFromEventList( &( pxQueueSetContainer->xTasksWaitingToReceive ) ) != pdFALSE )
                    {
                        /* The task waiting has a higher priority. */
                        xReturn = pdTRUE;
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
            }
            وإلا
            {
                prvIncrementQueueTxLock( pxQueueSetContainer, cTxLock );
            }
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }

        أرجع xReturn;
    }

#endif /* configUSE_QUEUE_SETS */
