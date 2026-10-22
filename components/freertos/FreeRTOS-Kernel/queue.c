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
#परिभाषय MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#अन्तर्भाव "FreeRTOS.h"
#अन्तर्भाव "task.h"
#अन्तर्भाव "queue.h"
/* Include private IDF API additions for critical thread safety macros */
#अन्तर्भाव "esp_private/freertos_idf_additions_priv.h"

#यदि ( configUSE_CO_ROUTINES == 1 )
    #अन्तर्भाव "croutine.h"
#यदि_अन्तः

/* Lint e9021, e961 and e750 are suppressed as a MISRA exception justified
 * because the MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#अपरिभाषय MPU_WRAPPERS_INCLUDED_FROM_API_FILE /*lint !e961 !e750 !e9021. */

/* Single core FreeRTOS uses queue locks to ensure that vTaskPlaceOnEventList()
 * calls are deterministic (as queue locks use scheduler suspension instead of
 * critical sections). However, the SMP implementation is non-deterministic
 * anyways, thus SMP can forego the use of queue locks (replaced with a critical
 * sections) in exchange for better queue performance. */
#यदि ( config_सङ्ख्या_सम्बन्धि_CORES > 1 )
    #परिभाषय queueUSE_LOCKS            0
    #परिभाषय queueUNLOCKED             ( ( int8_t ) 0 )
#अन्यथा /* configNUMBER_OF_CORES > 1 */
    #परिभाषय queueUSE_LOCKS            1
    /* Constants used with the cRxLock and cTxLock structure members. */
    #परिभाषय queueUNLOCKED             ( ( int8_t ) -1 )
    #परिभाषय queueLOCKED_UNMODIFIED    ( ( int8_t ) 0 )
    #परिभाषय queueINT8_MAX             ( ( int8_t ) 127 )
#यदि_अन्तः /* configNUMBER_OF_CORES > 1 */

/* When the Queue_t structure is used to represent a base queue its pcHead and
 * pcTail members are used as pointers into the queue storage area.  When the
 * Queue_t structure is used to represent a mutex pcHead and pcTail pointers are
 * not necessary, and the pcHead pointer is set to NULL to indicate that the
 * structure instead holds a pointer to the mutex holder (if any).  Map alternative
 * names to the pcHead and structure member to ensure the readability of the code
 * is maintained.  The QueuePointers_t and SemaphoreData_t types are used to form
 * a union as their usage is mutually exclusive dependent on what the queue is
 * being used for. */
#परिभाषय uxQueueType            pc_शीर्ष
#परिभाषय queueQUEUE_IS_MUTEX    रिक्त

प्रकारनिर्देश संरचना Queue_सूचकाः
{
    int8_t * pc_पुच्छ;     /*< Points to the byte at the end of the queue storage area.  Once more byte is allocated than necessary to store the queue items, this is used as a marker. */
    int8_t * pcReadFrom; /*< Points to the last place that a queued item was read from when the structure is used as a queue. */
} Queue_सूचकाः_t;

प्रकारनिर्देश संरचना Semaphore_दत्त
{
    TaskHandle_t xMutexHolder;        /*< The handle of the task that holds the mutex. */
    UBaseType_t ux_Recursive_Call_गणनम्; /*< Maintains a count of the number of times a recursive mutex has been recursively 'taken' when the structure is used as a mutex. */
} Semaphore_दत्त_t;

/* Semaphores do not actually store or copy data, so have an item size of
 * zero. */
#परिभाषय queue_SEMAPHORE_QUEUE_ITEM_दीर्घता    ( ( UBaseType_t ) 0 )
#परिभाषय queue_MUTEX_GIVE_खण्डम्_TIME          ( ( TickType_t ) 0U )

#यदि ( configUSE_PREEMPTION == 0 )

/* If the cooperative scheduler is being used then a yield should not be
 * performed just because a higher priority task has been woken. */
    #परिभाषय queueYIELD_IF_USING_PREEMPTION()
#अन्यथा
    #परिभाषय queueYIELD_IF_USING_PREEMPTION()    पत्तनम्_YIELD_WITHIN_API()
#यदि_अन्तः

/*
 * Definition of the queue used by the scheduler.
 * Items are queued by copy, not reference.  See the following link for the
 * rationale: https://www.FreeRTOS.org/Embedded-RTOS-Queues.html
 */
प्रकारनिर्देश संरचना QueueDefinition /* The old naming convention is used to prevent breaking kernel aware debuggers. */
{
    int8_t * pc_शीर्ष;           /*< Points to the beginning of the queue storage area. */
    int8_t * pc_Write_गन्तव्यम्;        /*< Points to the free next place in the storage area. */

    सङ्घ
    {
        Queue_सूचकाः_t xQueue;     /*< Data required exclusively when this structure is used as a queue. */
        Semaphore_दत्त_t xSemaphore; /*< Data required exclusively when this structure is used as a semaphore. */
    } u;

    List_t x_Tasks_प्रतीक्षा_गन्तव्यम्_Send;             /*< List of tasks that are blocked waiting to post onto this queue.  Stored in priority order. */
    List_t x_Tasks_प्रतीक्षा_गन्तव्यम्_Receive;          /*< List of tasks that are blocked waiting to read from this queue.  Stored in priority order. */

    चञ्चल UBaseType_t ux_Messages_प्रतीक्षा; /*< The number of items currently in the queue. */
    UBaseType_t ux_दीर्घता;                   /*< The length of the queue defined as the number of items it will hold, not the number of bytes. */
    UBaseType_t ux_Item_आकार;                 /*< The size of each items that the queue will hold. */

    #यदि ( queueUSE_LOCKS == 1 )
        चञ्चल int8_t c_Rx_तालकम्; /*< Stores the number of items received from the queue (removed from the queue) while the queue was locked.  Set to queueUNLOCKED when the queue is not locked. */
        चञ्चल int8_t c_Tx_तालकम्; /*< Stores the number of items transmitted to the queue (added to the queue) while the queue was locked.  Set to queueUNLOCKED when the queue is not locked. */
    #यदि_अन्तः /* queueUSE_LOCKS == 1 */

    #यदि ( ( config_SUPPORT_STATIC_आवण्टनम् == 1 ) && ( config_SUPPORT_DYNAMIC_आवण्टनम् == 1 ) )
        uint8_t ucStaticallyAllocated; /*< Set to pdTRUE if the memory used by the queue was statically allocated to ensure no attempt is made to free the memory. */
    #यदि_अन्तः

    #यदि ( configUSE_QUEUE_SETS == 1 )
        संरचना QueueDefinition * px_Queue_समुच्चयः_Container;
    #यदि_अन्तः

    #यदि ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_Queue_सङ्ख्या;
        uint8_t ucQueueType;
    #यदि_अन्तः

    पत्तनम्_MUX_TYPE x_Queue_तालकम्; /* Spinlock required for SMP critical sections */
} xQUEUE;

/* The old xQUEUE name is maintained above then typedefed to the new Queue_t
 * name below to enable the use of older kernel aware debuggers. */
प्रकारनिर्देश xQUEUE Queue_t;

/*-----------------------------------------------------------*/

/*
 * The queue registry is just a means for kernel aware debuggers to locate
 * queue structures.  It has no other purpose so is an optional component.
 */
#यदि ( config_QUEUE_REGISTRY_आकार > 0 )

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
    PRIVILEGED_दत्त QueueRegistryItem_t xQueueRegistry[ config_QUEUE_REGISTRY_आकार ];

    #यदि ( config_सङ्ख्या_सम्बन्धि_CORES > 1 )
/* Spinlock required in SMP when accessing the queue registry */
        स्थिर पत्तनम्_MUX_TYPE x_Queue_Registry_तालकम् = पत्तनम्_MUX_INITIALIZER_UNLOCKED;
    #यदि_अन्तः /* configNUMBER_OF_CORES > 1 */

#यदि_अन्तः /* configQUEUE_REGISTRY_SIZE */

#यदि ( queueUSE_LOCKS == 1 )

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
    स्थिर आधारः_Type_t prv_Is_Queue_रिक्तानि( ध्रुव Queue_t * pxQueue ) PRIVILEGED_FUNCTION;

/*
 * Uses a critical section to determine if there is any space in a queue.
 *
 * @return pdTRUE if there is no space, otherwise pdFALSE;
 */
    स्थिर आधारः_Type_t prv_Is_Queue_पूर्णम्( ध्रुव Queue_t * pxQueue ) PRIVILEGED_FUNCTION;
#यदि_अन्तः /* queueUSE_LOCKS == 1 */

/*
 * Copies an item into the queue, either at the front of the queue or the
 * back of the queue.
 */
स्थिर आधारः_Type_t prv_Copy_दत्त_गन्तव्यम्_Queue( Queue_t * ध्रुव pxQueue,
                                      ध्रुव शून्य * pv_Item_गन्तव्यम्_Queue,
                                      ध्रुव आधारः_Type_t xPosition ) PRIVILEGED_FUNCTION;

/*
 * Copies an item out of a queue.
 */
स्थिर शून्य prv_Copy_दत्त_From_Queue( Queue_t * ध्रुव pxQueue,
                                  शून्य * ध्रुव pv_बफर ) PRIVILEGED_FUNCTION;

#यदि ( configUSE_QUEUE_SETS == 1 )

/*
 * Checks to see if a queue is a member of a queue set, and if so, notifies
 * the queue set that the queue contains data.
 */
    स्थिर आधारः_Type_t prv_Notify_Queue_समुच्चयः_Container( ध्रुव Queue_t * ध्रुव pxQueue ) PRIVILEGED_FUNCTION;
#यदि_अन्तः

/*
 * Called after a Queue_t structure has been allocated either statically or
 * dynamically to fill in the structure's members.
 */
स्थिर शून्य prvInitialiseNewQueue( ध्रुव UBaseType_t ux_Queue_दीर्घता,
                                   ध्रुव UBaseType_t ux_Item_आकार,
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
#यदि_अन्तः

#यदि ( configUSE_MUTEXES == 1 )

/*
 * If a task waiting for a mutex causes the mutex holder to inherit a
 * priority, but the waiting task times out, then the holder should
 * disinherit the priority - but only down to the highest priority of any
 * other tasks that are waiting for the same mutex.  This function returns
 * that priority.
 */
    स्थिर UBaseType_t prv_Get_Disinherit_प्राथमिकता_After_Timeout( ध्रुव Queue_t * ध्रुव pxQueue ) PRIVILEGED_FUNCTION;
#यदि_अन्तः
/*-----------------------------------------------------------*/

#यदि ( queueUSE_LOCKS == 1 )

/*
 * Macro to mark a queue as locked.  Locking a queue prevents an ISR from
 * accessing the queue event lists.
 */
    #परिभाषय prv_तालकम्_Queue( pxQueue )                        \
    taskENTER_CRITICAL( &( pxQueue->x_Queue_तालकम् ) );        \
    {                                                      \
        यदि( ( pxQueue )->c_Rx_तालकम् == queueUNLOCKED )        \
        {                                                  \
            ( pxQueue )->c_Rx_तालकम् = queueLOCKED_UNMODIFIED; \
        }                                                  \
        यदि( ( pxQueue )->c_Tx_तालकम् == queueUNLOCKED )        \
        {                                                  \
            ( pxQueue )->c_Tx_तालकम् = queueLOCKED_UNMODIFIED; \
        }                                                  \
    }                                                      \
    taskEXIT_CRITICAL( &( pxQueue->x_Queue_तालकम् ) )

/*
 * Macro to increment cTxLock member of the queue data structure. It is
 * capped at the number of tasks in the system as we cannot unblock more
 * tasks than the number of tasks in the system.
 */
    #परिभाषय prv_Increment_Queue_Tx_तालकम्( pxQueue, c_Tx_तालकम् )                       \
    {                                                                         \
        ध्रुव UBaseType_t ux_सङ्ख्या_सम्बन्धि_Tasks = ux_Task_Get_सङ्ख्या_सम्बन्धि_Tasks();         \
        यदि( ( UBaseType_t ) ( c_Tx_तालकम् ) < ux_सङ्ख्या_सम्बन्धि_Tasks )                   \
        {                                                                     \
            configASSERT( ( c_Tx_तालकम् ) != queueINT8_MAX );                     \
            ( pxQueue )->c_Tx_तालकम् = ( int8_t ) ( ( c_Tx_तालकम् ) + ( int8_t ) 1 ); \
        }                                                                     \
    }

/*
 * Macro to increment cRxLock member of the queue data structure. It is
 * capped at the number of tasks in the system as we cannot unblock more
 * tasks than the number of tasks in the system.
 */
    #परिभाषय prv_Increment_Queue_Rx_तालकम्( pxQueue, c_Rx_तालकम् )                       \
    {                                                                         \
        ध्रुव UBaseType_t ux_सङ्ख्या_सम्बन्धि_Tasks = ux_Task_Get_सङ्ख्या_सम्बन्धि_Tasks();         \
        यदि( ( UBaseType_t ) ( c_Rx_तालकम् ) < ux_सङ्ख्या_सम्बन्धि_Tasks )                   \
        {                                                                     \
            configASSERT( ( c_Rx_तालकम् ) != queueINT8_MAX );                     \
            ( pxQueue )->c_Rx_तालकम् = ( int8_t ) ( ( c_Rx_तालकम् ) + ( int8_t ) 1 ); \
        }                                                                     \
    }
#यदि_अन्तः /* queueUSE_LOCKS == 1 */
/*-----------------------------------------------------------*/

आधारः_Type_t xQueueGenericReset( QueueHandle_t xQueue,
                               आधारः_Type_t xNewQueue )
{
    आधारः_Type_t xReturn = pdPASS;
    Queue_t * ध्रुव pxQueue = xQueue;

    configASSERT( pxQueue );

    यदि( xNewQueue == pdTRUE )
    {
        पत्तनम्_MUX_INITIALIZE( &( pxQueue->x_Queue_तालकम् ) );
    }

    यदि( ( pxQueue != रिक्त ) &&
        ( pxQueue->ux_दीर्घता >= 1U ) &&
        /* Check for multiplication overflow. */
        ( ( SIZE_MAX / pxQueue->ux_दीर्घता ) >= pxQueue->ux_Item_आकार ) )
    {
        taskENTER_CRITICAL( &( pxQueue->x_Queue_तालकम् ) );
        {
            pxQueue->u.xQueue.pc_पुच्छ = pxQueue->pc_शीर्ष + ( pxQueue->ux_दीर्घता * pxQueue->ux_Item_आकार ); /*lint !e9016 Pointer arithmetic allowed on char types, especially when it assists conveying intent. */
            pxQueue->ux_Messages_प्रतीक्षा = ( UBaseType_t ) 0U;
            pxQueue->pc_Write_गन्तव्यम् = pxQueue->pc_शीर्ष;
            pxQueue->u.xQueue.pcReadFrom = pxQueue->pc_शीर्ष + ( ( pxQueue->ux_दीर्घता - 1U ) * pxQueue->ux_Item_आकार ); /*lint !e9016 Pointer arithmetic allowed on char types, especially when it assists conveying intent. */
            #यदि ( queueUSE_LOCKS == 1 )
            {
                pxQueue->c_Rx_तालकम् = queueUNLOCKED;
                pxQueue->c_Tx_तालकम् = queueUNLOCKED;
            }
            #यदि_अन्तः /* queueUSE_LOCKS == 1 */

            यदि( xNewQueue == pdFALSE )
            {
                /* If there are tasks blocked waiting to read from the queue, then
                 * the tasks will remain blocked as after this function exits the queue
                 * will still be empty.  If there are tasks blocked waiting to write to
                 * the queue, then one should be unblocked as after this function exits
                 * it will be possible to write to it. */
                यदि( list_LIST_IS_रिक्तानि( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Send ) ) == pdFALSE )
                {
                    यदि( xTaskRemoveFromEventList( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Send ) ) != pdFALSE )
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
                vListInitialise( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Send ) );
                vListInitialise( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Receive ) );
            }
        }
        taskEXIT_CRITICAL( &( pxQueue->x_Queue_तालकम् ) );
    }
    अन्यथा
    {
        xReturn = pdFAIL;
    }

    configASSERT( xReturn != pdFAIL );

    /* A value is returned for calling semantic consistency with previous
     * versions. */
    प्रत्यागम xReturn;
}
/*-----------------------------------------------------------*/

#यदि ( config_SUPPORT_STATIC_आवण्टनम् == 1 )

    QueueHandle_t xQueueGenericCreateStatic( ध्रुव UBaseType_t ux_Queue_दीर्घता,
                                             ध्रुव UBaseType_t ux_Item_आकार,
                                             uint8_t * pucQueueStorage,
                                             StaticQueue_t * pxStaticQueue,
                                             ध्रुव uint8_t ucQueueType )
    {
        Queue_t * pxNewQueue = रिक्त;

        /* The StaticQueue_t structure and the queue storage area must be
         * supplied. */
        configASSERT( pxStaticQueue );

        यदि( ( ux_Queue_दीर्घता > ( UBaseType_t ) 0 ) &&
            ( pxStaticQueue != रिक्त ) &&

            /* A queue storage area should be provided if the item size is not 0, and
             * should not be provided if the item size is 0. */
            ( !( ( pucQueueStorage != रिक्त ) && ( ux_Item_आकार == 0 ) ) ) &&
            ( !( ( pucQueueStorage == रिक्त ) && ( ux_Item_आकार != 0 ) ) ) )
        {
            #यदि ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticQueue_t or StaticSemaphore_t equals the size of
                 * the real queue and semaphore structures. */
                चञ्चल size_t x_आकार = परिमाण( StaticQueue_t );

                /* This assertion cannot be branch covered in unit tests */
                configASSERT( x_आकार == परिमाण( Queue_t ) ); /* LCOV_EXCL_BR_LINE */
                ( शून्य ) x_आकार;                             /* Keeps lint quiet when configASSERT() is not defined. */
            }
            #यदि_अन्तः /* configASSERT_DEFINED */

            /* The address of a statically allocated queue was passed in, use it.
             * The address of a statically allocated storage area was also passed in
             * but is already set. */
            pxNewQueue = ( Queue_t * ) pxStaticQueue; /*lint !e740 !e9087 Unusual cast is ok as the structures are designed to have the same alignment, and the size is checked by an assert. */

            #यदि ( config_SUPPORT_DYNAMIC_आवण्टनम् == 1 )
            {
                /* Queues can be allocated wither statically or dynamically, so
                 * note this queue was allocated statically in case the queue is
                 * later deleted. */
                pxNewQueue->ucStaticallyAllocated = pdTRUE;
            }
            #यदि_अन्तः /* configSUPPORT_DYNAMIC_ALLOCATION */

            prvInitialiseNewQueue( ux_Queue_दीर्घता, ux_Item_आकार, pucQueueStorage, ucQueueType, pxNewQueue );
        }
        अन्यथा
        {
            configASSERT( pxNewQueue );
            mtCOVERAGE_TEST_MARKER();
        }

        प्रत्यागम pxNewQueue;
    }

#यदि_अन्तः /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

#यदि ( config_SUPPORT_STATIC_आवण्टनम् == 1 )

    आधारः_Type_t xQueueGenericGetStaticBuffers( QueueHandle_t xQueue,
                                              uint8_t ** ppucQueueStorage,
                                              StaticQueue_t ** ppxStaticQueue )
    {
        आधारः_Type_t xReturn;
        Queue_t * ध्रुव pxQueue = xQueue;

        configASSERT( pxQueue );
        configASSERT( ppxStaticQueue );

        #यदि ( config_SUPPORT_DYNAMIC_आवण्टनम् == 1 )
        {
            /* Check if the queue was statically allocated. */
            यदि( pxQueue->ucStaticallyAllocated == ( uint8_t ) pdTRUE )
            {
                यदि( ppucQueueStorage != रिक्त )
                {
                    *ppucQueueStorage = ( uint8_t * ) pxQueue->pc_शीर्ष;
                }

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
                *ppucQueueStorage = ( uint8_t * ) pxQueue->pc_शीर्ष;
            }

            *ppxStaticQueue = ( StaticQueue_t * ) pxQueue;
            xReturn = pdTRUE;
        }
        #यदि_अन्तः /* configSUPPORT_DYNAMIC_ALLOCATION */

        प्रत्यागम xReturn;
    }

#यदि_अन्तः /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

#यदि ( config_SUPPORT_DYNAMIC_आवण्टनम् == 1 )

    QueueHandle_t xQueueGenericCreate( ध्रुव UBaseType_t ux_Queue_दीर्घता,
                                       ध्रुव UBaseType_t ux_Item_आकार,
                                       ध्रुव uint8_t ucQueueType )
    {
        Queue_t * pxNewQueue = रिक्त;
        size_t x_Queue_आकार_अन्तर्_अष्टकानि;
        uint8_t * pucQueueStorage;

        यदि( ( ux_Queue_दीर्घता > ( UBaseType_t ) 0 ) &&
            /* Check for multiplication overflow. */
            ( ( SIZE_MAX / ux_Queue_दीर्घता ) >= ux_Item_आकार ) &&
            /* Check for addition overflow. */
            ( ( SIZE_MAX - परिमाण( Queue_t ) ) >= ( ux_Queue_दीर्घता * ux_Item_आकार ) ) )
        {
            /* Allocate enough space to hold the maximum number of items that
             * can be in the queue at any time.  It is valid for uxItemSize to be
             * zero in the case the queue is used as a semaphore. */
            x_Queue_आकार_अन्तर्_अष्टकानि = ( size_t ) ( ux_Queue_दीर्घता * ux_Item_आकार ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */

            /* Allocate the queue and storage area.  Justification for MISRA
             * deviation as follows:  pvPortMalloc() always ensures returned memory
             * blocks are aligned per the requirements of the MCU stack.  In this case
             * pvPortMalloc() must return a pointer that is guaranteed to meet the
             * alignment requirements of the Queue_t structure - which in this case
             * is an int8_t *.  Therefore, whenever the stack alignment requirements
             * are greater than or equal to the pointer to char requirements the cast
             * is safe.  In other cases alignment requirements are not strict (one or
             * two bytes). */
            pxNewQueue = ( Queue_t * ) pv_पत्तनम्_Malloc( परिमाण( Queue_t ) + x_Queue_आकार_अन्तर्_अष्टकानि ); /*lint !e9087 !e9079 see comment above. */

            यदि( pxNewQueue != रिक्त )
            {
                /* Jump past the queue structure to find the location of the queue
                 * storage area. */
                pucQueueStorage = ( uint8_t * ) pxNewQueue;
                pucQueueStorage += परिमाण( Queue_t ); /*lint !e9016 Pointer arithmetic allowed on char types, especially when it assists conveying intent. */

                #यदि ( config_SUPPORT_STATIC_आवण्टनम् == 1 )
                {
                    /* Queues can be created either statically or dynamically, so
                     * note this task was created dynamically in case it is later
                     * deleted. */
                    pxNewQueue->ucStaticallyAllocated = pdFALSE;
                }
                #यदि_अन्तः /* configSUPPORT_STATIC_ALLOCATION */

                prvInitialiseNewQueue( ux_Queue_दीर्घता, ux_Item_आकार, pucQueueStorage, ucQueueType, pxNewQueue );
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

        प्रत्यागम pxNewQueue;
    }

#यदि_अन्तः /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

स्थिर शून्य prvInitialiseNewQueue( ध्रुव UBaseType_t ux_Queue_दीर्घता,
                                   ध्रुव UBaseType_t ux_Item_आकार,
                                   uint8_t * pucQueueStorage,
                                   ध्रुव uint8_t ucQueueType,
                                   Queue_t * pxNewQueue )
{
    /* Remove compiler warnings about unused parameters should
     * configUSE_TRACE_FACILITY not be set to 1. */
    ( शून्य ) ucQueueType;

    यदि( ux_Item_आकार == ( UBaseType_t ) 0 )
    {
        /* No RAM was allocated for the queue storage area, but PC head cannot
         * be set to NULL because NULL is used as a key to say the queue is used as
         * a mutex.  Therefore just set pcHead to point to the queue as a benign
         * value that is known to be within the memory map. */
        pxNewQueue->pc_शीर्ष = ( int8_t * ) pxNewQueue;
    }
    अन्यथा
    {
        /* Set the head to the start of the queue storage area. */
        pxNewQueue->pc_शीर्ष = ( int8_t * ) pucQueueStorage;
    }

    /* Initialise the queue members as described where the queue type is
     * defined. */
    pxNewQueue->ux_दीर्घता = ux_Queue_दीर्घता;
    pxNewQueue->ux_Item_आकार = ux_Item_आकार;
    ( शून्य ) xQueueGenericReset( pxNewQueue, pdTRUE );

    #यदि ( configUSE_TRACE_FACILITY == 1 )
    {
        pxNewQueue->ucQueueType = ucQueueType;
    }
    #यदि_अन्तः /* configUSE_TRACE_FACILITY */

    #यदि ( configUSE_QUEUE_SETS == 1 )
    {
        pxNewQueue->px_Queue_समुच्चयः_Container = रिक्त;
    }
    #यदि_अन्तः /* configUSE_QUEUE_SETS */

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
            pxNewQueue->u.xSemaphore.ux_Recursive_Call_गणनम् = 0;

            /* Initialize the mutex's spinlock */
            पत्तनम्_MUX_INITIALIZE( &( pxNewQueue->x_Queue_तालकम् ) );

            traceCREATE_MUTEX( pxNewQueue );

            /* Start with the semaphore in the expected state. */
            ( शून्य ) xQueueGenericSend( pxNewQueue, रिक्त, ( TickType_t ) 0U, queue_SEND_गन्तव्यम्_पृष्ठभागः );
        }
        अन्यथा
        {
            traceCREATE_MUTEX_FAILED();
        }
    }

#यदि_अन्तः /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

#यदि ( ( configUSE_MUTEXES == 1 ) && ( config_SUPPORT_DYNAMIC_आवण्टनम् == 1 ) )

    QueueHandle_t xQueueCreateMutex( ध्रुव uint8_t ucQueueType )
    {
        QueueHandle_t xNewQueue;
        ध्रुव UBaseType_t ux_Mutex_दीर्घता = ( UBaseType_t ) 1, ux_Mutex_आकार = ( UBaseType_t ) 0;

        xNewQueue = xQueueGenericCreate( ux_Mutex_दीर्घता, ux_Mutex_आकार, ucQueueType );
        prvInitialiseMutex( ( Queue_t * ) xNewQueue );

        प्रत्यागम xNewQueue;
    }

#यदि_अन्तः /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

#यदि ( ( configUSE_MUTEXES == 1 ) && ( config_SUPPORT_STATIC_आवण्टनम् == 1 ) )

    QueueHandle_t xQueueCreateMutexStatic( ध्रुव uint8_t ucQueueType,
                                           StaticQueue_t * pxStaticQueue )
    {
        QueueHandle_t xNewQueue;
        ध्रुव UBaseType_t ux_Mutex_दीर्घता = ( UBaseType_t ) 1, ux_Mutex_आकार = ( UBaseType_t ) 0;

        /* Prevent compiler warnings about unused parameters if
         * configUSE_TRACE_FACILITY does not equal 1. */
        ( शून्य ) ucQueueType;

        xNewQueue = xQueueGenericCreateStatic( ux_Mutex_दीर्घता, ux_Mutex_आकार, रिक्त, pxStaticQueue, ucQueueType );
        prvInitialiseMutex( ( Queue_t * ) xNewQueue );

        प्रत्यागम xNewQueue;
    }

#यदि_अन्तः /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

#यदि ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) )

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
        taskENTER_CRITICAL( &( pxSemaphore->x_Queue_तालकम् ) );
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
        taskEXIT_CRITICAL( &( pxSemaphore->x_Queue_तालकम् ) );

        प्रत्यागम pxReturn;
    } /*lint !e818 xSemaphore cannot be a pointer to const because it is a typedef. */

#यदि_अन्तः /* if ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) ) */
/*-----------------------------------------------------------*/

#यदि ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) )

    TaskHandle_t xQueueGetMutexHolderFromISR( QueueHandle_t xSemaphore )
    {
        TaskHandle_t pxReturn;

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

        प्रत्यागम pxReturn;
    } /*lint !e818 xSemaphore cannot be a pointer to const because it is a typedef. */

#यदि_अन्तः /* if ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) ) */
/*-----------------------------------------------------------*/

#यदि ( configUSE_RECURSIVE_MUTEXES == 1 )

    आधारः_Type_t xQueueGiveMutexRecursive( QueueHandle_t xMutex )
    {
        आधारः_Type_t xReturn;
        Queue_t * ध्रुव pxMutex = ( Queue_t * ) xMutex;

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
            ( pxMutex->u.xSemaphore.ux_Recursive_Call_गणनम् )--;

            /* Has the recursive call count unwound to 0? */
            यदि( pxMutex->u.xSemaphore.ux_Recursive_Call_गणनम् == ( UBaseType_t ) 0 )
            {
                /* Return the mutex.  This will automatically unblock any other
                 * task that might be waiting to access the mutex. */
                ( शून्य ) xQueueGenericSend( pxMutex, रिक्त, queue_MUTEX_GIVE_खण्डम्_TIME, queue_SEND_गन्तव्यम्_पृष्ठभागः );
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

        प्रत्यागम xReturn;
    }

#यदि_अन्तः /* configUSE_RECURSIVE_MUTEXES */
/*-----------------------------------------------------------*/

#यदि ( configUSE_RECURSIVE_MUTEXES == 1 )

    आधारः_Type_t xQueueTakeMutexRecursive( QueueHandle_t xMutex,
                                         TickType_t x_Ticks_गन्तव्यम्_Wait )
    {
        आधारः_Type_t xReturn;
        Queue_t * ध्रुव pxMutex = ( Queue_t * ) xMutex;

        configASSERT( pxMutex );

        /* Comments regarding mutual exclusion as per those within
         * xQueueGiveMutexRecursive(). */

        traceTAKE_MUTEX_RECURSIVE( pxMutex );

        यदि( pxMutex->u.xSemaphore.xMutexHolder == xTaskGetCurrentTaskHandle() )
        {
            ( pxMutex->u.xSemaphore.ux_Recursive_Call_गणनम् )++;
            xReturn = pdPASS;
        }
        अन्यथा
        {
            xReturn = xQueueSemaphoreTake( pxMutex, x_Ticks_गन्तव्यम्_Wait );

            /* pdPASS will only be returned if the mutex was successfully
             * obtained.  The calling task may have entered the Blocked state
             * before reaching here. */
            यदि( xReturn != pdFAIL )
            {
                ( pxMutex->u.xSemaphore.ux_Recursive_Call_गणनम् )++;
            }
            अन्यथा
            {
                traceTAKE_MUTEX_RECURSIVE_FAILED( pxMutex );
            }
        }

        प्रत्यागम xReturn;
    }

#यदि_अन्तः /* configUSE_RECURSIVE_MUTEXES */
/*-----------------------------------------------------------*/

#यदि ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( config_SUPPORT_STATIC_आवण्टनम् == 1 ) )

    QueueHandle_t xQueueCreateCountingSemaphoreStatic( ध्रुव UBaseType_t ux_Max_गणनम्,
                                                       ध्रुव UBaseType_t ux_Initial_गणनम्,
                                                       StaticQueue_t * pxStaticQueue )
    {
        QueueHandle_t xHandle = रिक्त;

        यदि( ( ux_Max_गणनम् != 0 ) &&
            ( ux_Initial_गणनम् <= ux_Max_गणनम् ) )
        {
            xHandle = xQueueGenericCreateStatic( ux_Max_गणनम्, queue_SEMAPHORE_QUEUE_ITEM_दीर्घता, रिक्त, pxStaticQueue, queueQUEUE_TYPE_COUNTING_SEMAPHORE );

            यदि( xHandle != रिक्त )
            {
                ( ( Queue_t * ) xHandle )->ux_Messages_प्रतीक्षा = ux_Initial_गणनम्;

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

        प्रत्यागम xHandle;
    }

#यदि_अन्तः /* ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) ) */
/*-----------------------------------------------------------*/

#यदि ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( config_SUPPORT_DYNAMIC_आवण्टनम् == 1 ) )

    QueueHandle_t xQueueCreateCountingSemaphore( ध्रुव UBaseType_t ux_Max_गणनम्,
                                                 ध्रुव UBaseType_t ux_Initial_गणनम् )
    {
        QueueHandle_t xHandle = रिक्त;

        यदि( ( ux_Max_गणनम् != 0 ) &&
            ( ux_Initial_गणनम् <= ux_Max_गणनम् ) )
        {
            xHandle = xQueueGenericCreate( ux_Max_गणनम्, queue_SEMAPHORE_QUEUE_ITEM_दीर्घता, queueQUEUE_TYPE_COUNTING_SEMAPHORE );

            यदि( xHandle != रिक्त )
            {
                ( ( Queue_t * ) xHandle )->ux_Messages_प्रतीक्षा = ux_Initial_गणनम्;

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

        प्रत्यागम xHandle;
    }

#यदि_अन्तः /* ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) ) */
/*-----------------------------------------------------------*/

आधारः_Type_t xQueueGenericSend( QueueHandle_t xQueue,
                              ध्रुव शून्य * ध्रुव pv_Item_गन्तव्यम्_Queue,
                              TickType_t x_Ticks_गन्तव्यम्_Wait,
                              ध्रुव आधारः_Type_t xCopyPosition )
{
    आधारः_Type_t x_प्रविष्टिः_Time_समुच्चयः = pdFALSE, xYieldRequired;
    Time_बहिर्गमनम्_t x_Time_बहिर्गमनम्;
    Queue_t * ध्रुव pxQueue = xQueue;

    configASSERT( pxQueue );
    configASSERT( !( ( pv_Item_गन्तव्यम्_Queue == रिक्त ) && ( pxQueue->ux_Item_आकार != ( UBaseType_t ) 0U ) ) );
    configASSERT( !( ( xCopyPosition == queueOVERWRITE ) && ( pxQueue->ux_दीर्घता != 1 ) ) );
    #यदि ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_गन्तव्यम्_Wait != 0 ) ) );
    }
    #यदि_अन्तः

    /*lint -save -e904 This function relaxes the coding standard somewhat to
     * allow return statements within the function itself.  This is done in the
     * interest of execution time efficiency. */
    प्रति( ; ; )
    {
        taskENTER_CRITICAL( &( pxQueue->x_Queue_तालकम् ) );
        {
            /* Is there room on the queue now?  The running task must be the
             * highest priority task wanting to access the queue.  If the head item
             * in the queue is to be overwritten then it does not matter if the
             * queue is full. */
            यदि( ( pxQueue->ux_Messages_प्रतीक्षा < pxQueue->ux_दीर्घता ) || ( xCopyPosition == queueOVERWRITE ) )
            {
                traceQUEUE_SEND( pxQueue );

                #यदि ( configUSE_QUEUE_SETS == 1 )
                {
                    ध्रुव UBaseType_t ux_Previous_Messages_प्रतीक्षा = pxQueue->ux_Messages_प्रतीक्षा;

                    xYieldRequired = prv_Copy_दत्त_गन्तव्यम्_Queue( pxQueue, pv_Item_गन्तव्यम्_Queue, xCopyPosition );

                    यदि( pxQueue->px_Queue_समुच्चयः_Container != रिक्त )
                    {
                        यदि( ( xCopyPosition == queueOVERWRITE ) && ( ux_Previous_Messages_प्रतीक्षा != ( UBaseType_t ) 0 ) )
                        {
                            /* Do not notify the queue set as an existing item
                             * was overwritten in the queue so the number of items
                             * in the queue has not changed. */
                            mtCOVERAGE_TEST_MARKER();
                        }
                        अन्यथा यदि( prv_Notify_Queue_समुच्चयः_Container( pxQueue ) != pdFALSE )
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
                        यदि( list_LIST_IS_रिक्तानि( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Receive ) ) == pdFALSE )
                        {
                            यदि( xTaskRemoveFromEventList( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Receive ) ) != pdFALSE )
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
                    xYieldRequired = prv_Copy_दत्त_गन्तव्यम्_Queue( pxQueue, pv_Item_गन्तव्यम्_Queue, xCopyPosition );

                    /* If there was a task waiting for data to arrive on the
                     * queue then unblock it now. */
                    यदि( list_LIST_IS_रिक्तानि( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Receive ) ) == pdFALSE )
                    {
                        यदि( xTaskRemoveFromEventList( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Receive ) ) != pdFALSE )
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
                #यदि_अन्तः /* configUSE_QUEUE_SETS */

                taskEXIT_CRITICAL( &( pxQueue->x_Queue_तालकम् ) );
                प्रत्यागम pdPASS;
            }
            अन्यथा
            {
                यदि( x_Ticks_गन्तव्यम्_Wait == ( TickType_t ) 0 )
                {
                    /* The queue was full and no block time is specified (or
                     * the block time has expired) so leave now. */
                    taskEXIT_CRITICAL( &( pxQueue->x_Queue_तालकम् ) );

                    /* Return to the original privilege level before exiting
                     * the function. */
                    traceQUEUE_SEND_FAILED( pxQueue );
                    प्रत्यागम err_QUEUE_पूर्णम्;
                }
                अन्यथा यदि( x_प्रविष्टिः_Time_समुच्चयः == pdFALSE )
                {
                    /* The queue was full and a block time was specified so
                     * configure the timeout structure. */
                    v_Task_आन्तरिकम्_समुच्चयः_Time_बहिर्गमनम्_State( &x_Time_बहिर्गमनम् );
                    x_प्रविष्टिः_Time_समुच्चयः = pdTRUE;
                }
                अन्यथा
                {
                    /* Entry time was already set. */
                    mtCOVERAGE_TEST_MARKER();
                }
            }

            /* If queue locks ARE NOT being used:
             * - At this point, the queue is full and entry time has been set
             * - We simply check for a time out, block if not timed out, or
             *   return an error if we have timed out. */
            #यदि ( queueUSE_LOCKS == 0 )
            {
                /* Update the timeout state to see if it has expired yet. */
                यदि( x_Task_Check_For_Time_बहिर्गमनम्( &x_Time_बहिर्गमनम्, &x_Ticks_गन्तव्यम्_Wait ) == pdFALSE )
                {
                    /* Not timed out yet. Block the current task. */
                    traceBLOCKING_ON_QUEUE_SEND( pxQueue );
                    vTaskPlaceOnEventList( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Send ), x_Ticks_गन्तव्यम्_Wait );
                    पत्तनम्_YIELD_WITHIN_API();
                }
                अन्यथा
                {
                    /* We have timed out. Return an error. */
                    taskEXIT_CRITICAL( &( pxQueue->x_Queue_तालकम् ) );
                    traceQUEUE_SEND_FAILED( pxQueue );
                    प्रत्यागम err_QUEUE_पूर्णम्;
                }
            }
            #यदि_अन्तः /* queueUSE_LOCKS == 0 */
        }
        taskEXIT_CRITICAL( &( pxQueue->x_Queue_तालकम् ) );

        /* If queue locks ARE being used:
         * - At this point, the queue is full and entry time has been set
         * - We follow the original procedure of locking the queue before
         *   attempting to block. */
        #यदि ( queueUSE_LOCKS == 1 )
        {
            /* Interrupts and other tasks can send to and receive from the queue
             * now the critical section has been exited. */

            vTaskSuspendAll();
            prv_तालकम्_Queue( pxQueue );

            /* Update the timeout state to see if it has expired yet. */
            यदि( x_Task_Check_For_Time_बहिर्गमनम्( &x_Time_बहिर्गमनम्, &x_Ticks_गन्तव्यम्_Wait ) == pdFALSE )
            {
                यदि( prv_Is_Queue_पूर्णम्( pxQueue ) != pdFALSE )
                {
                    traceBLOCKING_ON_QUEUE_SEND( pxQueue );
                    vTaskPlaceOnEventList( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Send ), x_Ticks_गन्तव्यम्_Wait );

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
                        पत्तनम्_YIELD_WITHIN_API();
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
                प्रत्यागम err_QUEUE_पूर्णम्;
            }
        }
        #यदि_अन्तः /* queueUSE_LOCKS == 1 */
    } /*lint -restore */
}
/*-----------------------------------------------------------*/

आधारः_Type_t xQueueGenericSendFromISR( QueueHandle_t xQueue,
                                     ध्रुव शून्य * ध्रुव pv_Item_गन्तव्यम्_Queue,
                                     आधारः_Type_t * ध्रुव px_Higher_प्राथमिकता_Task_Woken,
                                     ध्रुव आधारः_Type_t xCopyPosition )
{
    आधारः_Type_t xReturn;
    UBaseType_t ux_Saved_व्यवधानम्_स्थितिः;
    Queue_t * ध्रुव pxQueue = xQueue;

    configASSERT( pxQueue );
    configASSERT( !( ( pv_Item_गन्तव्यम्_Queue == रिक्त ) && ( pxQueue->ux_Item_आकार != ( UBaseType_t ) 0U ) ) );
    configASSERT( !( ( xCopyPosition == queueOVERWRITE ) && ( pxQueue->ux_दीर्घता != 1 ) ) );

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
    पत्तनम्_ASSERT_IF_व्यवधानम्_प्राथमिकता_INVALID();

    /* Similar to xQueueGenericSend, except without blocking if there is no room
     * in the queue.  Also don't directly wake a task that was blocked on a queue
     * read, instead return a flag to say whether a context switch is required or
     * not (i.e. has a task with a higher priority than us been woken by this
     * post). */
    prv_ENTER_CRITICAL_OR_आवरणम्_ISR( &( pxQueue->x_Queue_तालकम् ), ux_Saved_व्यवधानम्_स्थितिः );
    {
        यदि( ( pxQueue->ux_Messages_प्रतीक्षा < pxQueue->ux_दीर्घता ) || ( xCopyPosition == queueOVERWRITE ) )
        {
            #यदि ( queueUSE_LOCKS == 1 )
                ध्रुव int8_t c_Tx_तालकम् = pxQueue->c_Tx_तालकम्;
            #अन्यथा
                /* Queue locks not used, so we treat it as unlocked. */
                ध्रुव int8_t c_Tx_तालकम् = queueUNLOCKED;
            #यदि_अन्तः /* queueUSE_LOCKS == 1 */
            ध्रुव UBaseType_t ux_Previous_Messages_प्रतीक्षा = pxQueue->ux_Messages_प्रतीक्षा;

            traceQUEUE_SEND_FROM_ISR( pxQueue );

            /* Semaphores use xQueueGiveFromISR(), so pxQueue will not be a
             *  semaphore or mutex.  That means prvCopyDataToQueue() cannot result
             *  in a task disinheriting a priority and prvCopyDataToQueue() can be
             *  called here even though the disinherit function does not check if
             *  the scheduler is suspended before accessing the ready lists. */
            ( शून्य ) prv_Copy_दत्त_गन्तव्यम्_Queue( pxQueue, pv_Item_गन्तव्यम्_Queue, xCopyPosition );

            /* The event list is not altered if the queue is locked.  This will
             * be done when the queue is unlocked later. */
            यदि( c_Tx_तालकम् == queueUNLOCKED )
            {
                #यदि ( configUSE_QUEUE_SETS == 1 )
                {
                    यदि( pxQueue->px_Queue_समुच्चयः_Container != रिक्त )
                    {
                        यदि( ( xCopyPosition == queueOVERWRITE ) && ( ux_Previous_Messages_प्रतीक्षा != ( UBaseType_t ) 0 ) )
                        {
                            /* Do not notify the queue set as an existing item
                             * was overwritten in the queue so the number of items
                             * in the queue has not changed. */
                            mtCOVERAGE_TEST_MARKER();
                        }
                        अन्यथा यदि( prv_Notify_Queue_समुच्चयः_Container( pxQueue ) != pdFALSE )
                        {
                            /* The queue is a member of a queue set, and posting
                             * to the queue set caused a higher priority task to
                             * unblock.  A context switch is required. */
                            यदि( px_Higher_प्राथमिकता_Task_Woken != रिक्त )
                            {
                                *px_Higher_प्राथमिकता_Task_Woken = pdTRUE;
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
                        यदि( list_LIST_IS_रिक्तानि( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Receive ) ) == pdFALSE )
                        {
                            यदि( xTaskRemoveFromEventList( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Receive ) ) != pdFALSE )
                            {
                                /* The task waiting has a higher priority so
                                 *  record that a context switch is required. */
                                यदि( px_Higher_प्राथमिकता_Task_Woken != रिक्त )
                                {
                                    *px_Higher_प्राथमिकता_Task_Woken = pdTRUE;
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
                    यदि( list_LIST_IS_रिक्तानि( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Receive ) ) == pdFALSE )
                    {
                        यदि( xTaskRemoveFromEventList( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Receive ) ) != pdFALSE )
                        {
                            /* The task waiting has a higher priority so record that a
                             * context switch is required. */
                            यदि( px_Higher_प्राथमिकता_Task_Woken != रिक्त )
                            {
                                *px_Higher_प्राथमिकता_Task_Woken = pdTRUE;
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
                    ( शून्य ) ux_Previous_Messages_प्रतीक्षा;
                }
                #यदि_अन्तः /* configUSE_QUEUE_SETS */
            }
            अन्यथा
            {
                #यदि ( queueUSE_LOCKS == 1 )
                {
                    /* Increment the lock count so the task that unlocks the queue
                     * knows that data was posted while it was locked. */
                    prv_Increment_Queue_Tx_तालकम्( pxQueue, c_Tx_तालकम् );
                }
                #यदि_अन्तः /* queueUSE_LOCKS == 1 */
            }

            xReturn = pdPASS;
        }
        अन्यथा
        {
            traceQUEUE_SEND_FROM_ISR_FAILED( pxQueue );
            xReturn = err_QUEUE_पूर्णम्;
        }
    }
    prvEXIT_CRITICAL_OR_UNMASK_ISR( &( pxQueue->x_Queue_तालकम् ), ux_Saved_व्यवधानम्_स्थितिः );

    प्रत्यागम xReturn;
}
/*-----------------------------------------------------------*/

आधारः_Type_t xQueueGiveFromISR( QueueHandle_t xQueue,
                              आधारः_Type_t * ध्रुव px_Higher_प्राथमिकता_Task_Woken )
{
    आधारः_Type_t xReturn;
    UBaseType_t ux_Saved_व्यवधानम्_स्थितिः;
    Queue_t * ध्रुव pxQueue = xQueue;

    /* Similar to xQueueGenericSendFromISR() but used with semaphores where the
     * item size is 0.  Don't directly wake a task that was blocked on a queue
     * read, instead return a flag to say whether a context switch is required or
     * not (i.e. has a task with a higher priority than us been woken by this
     * post). */

    configASSERT( pxQueue );

    /* xQueueGenericSendFromISR() should be used instead of xQueueGiveFromISR()
     * if the item size is not 0. */
    configASSERT( pxQueue->ux_Item_आकार == 0 );

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
    पत्तनम्_ASSERT_IF_व्यवधानम्_प्राथमिकता_INVALID();

    prv_ENTER_CRITICAL_OR_आवरणम्_ISR( &( pxQueue->x_Queue_तालकम् ), ux_Saved_व्यवधानम्_स्थितिः );
    {
        ध्रुव UBaseType_t ux_Messages_प्रतीक्षा = pxQueue->ux_Messages_प्रतीक्षा;

        /* When the queue is used to implement a semaphore no data is ever
         * moved through the queue but it is still valid to see if the queue 'has
         * space'. */
        यदि( ux_Messages_प्रतीक्षा < pxQueue->ux_दीर्घता )
        {
            #यदि ( queueUSE_LOCKS == 1 )
                ध्रुव int8_t c_Tx_तालकम् = pxQueue->c_Tx_तालकम्;
            #अन्यथा
                /* Queue locks not used, so we treat it as unlocked. */
                ध्रुव int8_t c_Tx_तालकम् = queueUNLOCKED;
            #यदि_अन्तः /* queueUSE_LOCKS == 1 */

            /* Todo: Reconcile tracing differences (IDF-8183) */
            traceQUEUE_GIVE_FROM_ISR( pxQueue );

            /* A task can only have an inherited priority if it is a mutex
             * holder - and if there is a mutex holder then the mutex cannot be
             * given from an ISR.  As this is the ISR version of the function it
             * can be assumed there is no mutex holder and no need to determine if
             * priority disinheritance is needed.  Simply increase the count of
             * messages (semaphores) available. */
            pxQueue->ux_Messages_प्रतीक्षा = ux_Messages_प्रतीक्षा + ( UBaseType_t ) 1;

            /* The event list is not altered if the queue is locked.  This will
             * be done when the queue is unlocked later. */
            यदि( c_Tx_तालकम् == queueUNLOCKED )
            {
                #यदि ( configUSE_QUEUE_SETS == 1 )
                {
                    यदि( pxQueue->px_Queue_समुच्चयः_Container != रिक्त )
                    {
                        यदि( prv_Notify_Queue_समुच्चयः_Container( pxQueue ) != pdFALSE )
                        {
                            /* The semaphore is a member of a queue set, and
                             * posting to the queue set caused a higher priority
                             * task to unblock.  A context switch is required. */
                            यदि( px_Higher_प्राथमिकता_Task_Woken != रिक्त )
                            {
                                *px_Higher_प्राथमिकता_Task_Woken = pdTRUE;
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
                        यदि( list_LIST_IS_रिक्तानि( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Receive ) ) == pdFALSE )
                        {
                            यदि( xTaskRemoveFromEventList( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Receive ) ) != pdFALSE )
                            {
                                /* The task waiting has a higher priority so
                                 *  record that a context switch is required. */
                                यदि( px_Higher_प्राथमिकता_Task_Woken != रिक्त )
                                {
                                    *px_Higher_प्राथमिकता_Task_Woken = pdTRUE;
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
                    यदि( list_LIST_IS_रिक्तानि( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Receive ) ) == pdFALSE )
                    {
                        यदि( xTaskRemoveFromEventList( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Receive ) ) != pdFALSE )
                        {
                            /* The task waiting has a higher priority so record that a
                             * context switch is required. */
                            यदि( px_Higher_प्राथमिकता_Task_Woken != रिक्त )
                            {
                                *px_Higher_प्राथमिकता_Task_Woken = pdTRUE;
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
                #यदि_अन्तः /* configUSE_QUEUE_SETS */
            }
            अन्यथा
            {
                #यदि ( queueUSE_LOCKS == 1 )
                {
                    /* Increment the lock count so the task that unlocks the queue
                     * knows that data was posted while it was locked. */
                    prv_Increment_Queue_Tx_तालकम्( pxQueue, c_Tx_तालकम् );
                }
                #यदि_अन्तः /* queueUSE_LOCKS == 1 */
            }

            xReturn = pdPASS;
        }
        अन्यथा
        {
            /* Todo: Reconcile tracing differences (IDF-8183) */
            traceQUEUE_GIVE_FROM_ISR_FAILED( pxQueue );
            xReturn = err_QUEUE_पूर्णम्;
        }
    }
    prvEXIT_CRITICAL_OR_UNMASK_ISR( &( pxQueue->x_Queue_तालकम् ), ux_Saved_व्यवधानम्_स्थितिः );

    प्रत्यागम xReturn;
}
/*-----------------------------------------------------------*/

आधारः_Type_t xQueueReceive( QueueHandle_t xQueue,
                          शून्य * ध्रुव pv_बफर,
                          TickType_t x_Ticks_गन्तव्यम्_Wait )
{
    आधारः_Type_t x_प्रविष्टिः_Time_समुच्चयः = pdFALSE;
    Time_बहिर्गमनम्_t x_Time_बहिर्गमनम्;
    Queue_t * ध्रुव pxQueue = xQueue;

    /* Check the pointer is not NULL. */
    configASSERT( ( pxQueue ) );

    /* The buffer into which data is received can only be NULL if the data size
     * is zero (so no data is copied into the buffer). */
    configASSERT( !( ( ( pv_बफर ) == रिक्त ) && ( ( pxQueue )->ux_Item_आकार != ( UBaseType_t ) 0U ) ) );

    /* Cannot block if the scheduler is suspended. */
    #यदि ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_गन्तव्यम्_Wait != 0 ) ) );
    }
    #यदि_अन्तः

    /*lint -save -e904  This function relaxes the coding standard somewhat to
     * allow return statements within the function itself.  This is done in the
     * interest of execution time efficiency. */
    प्रति( ; ; )
    {
        taskENTER_CRITICAL( &( pxQueue->x_Queue_तालकम् ) );
        {
            ध्रुव UBaseType_t ux_Messages_प्रतीक्षा = pxQueue->ux_Messages_प्रतीक्षा;

            /* Is there data in the queue now?  To be running the calling task
             * must be the highest priority task wanting to access the queue. */
            यदि( ux_Messages_प्रतीक्षा > ( UBaseType_t ) 0 )
            {
                /* Data available, remove one item. */
                prv_Copy_दत्त_From_Queue( pxQueue, pv_बफर );
                traceQUEUE_RECEIVE( pxQueue );
                pxQueue->ux_Messages_प्रतीक्षा = ux_Messages_प्रतीक्षा - ( UBaseType_t ) 1;

                /* There is now space in the queue, were any tasks waiting to
                 * post to the queue?  If so, unblock the highest priority waiting
                 * task. */
                यदि( list_LIST_IS_रिक्तानि( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Send ) ) == pdFALSE )
                {
                    यदि( xTaskRemoveFromEventList( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Send ) ) != pdFALSE )
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

                taskEXIT_CRITICAL( &( pxQueue->x_Queue_तालकम् ) );
                प्रत्यागम pdPASS;
            }
            अन्यथा
            {
                यदि( x_Ticks_गन्तव्यम्_Wait == ( TickType_t ) 0 )
                {
                    /* The queue was empty and no block time is specified (or
                     * the block time has expired) so leave now. */
                    taskEXIT_CRITICAL( &( pxQueue->x_Queue_तालकम् ) );
                    traceQUEUE_RECEIVE_FAILED( pxQueue );
                    प्रत्यागम err_QUEUE_रिक्तानि;
                }
                अन्यथा यदि( x_प्रविष्टिः_Time_समुच्चयः == pdFALSE )
                {
                    /* The queue was empty and a block time was specified so
                     * configure the timeout structure. */
                    v_Task_आन्तरिकम्_समुच्चयः_Time_बहिर्गमनम्_State( &x_Time_बहिर्गमनम् );
                    x_प्रविष्टिः_Time_समुच्चयः = pdTRUE;
                }
                अन्यथा
                {
                    /* Entry time was already set. */
                    mtCOVERAGE_TEST_MARKER();
                }
            }

            /* If queue locks ARE NOT being used:
             * - At this point, the queue is empty and entry time has been set
             * - We simply check for a time out, block if not timed out, or
             *   return an error if we have timed out. */
            #यदि ( queueUSE_LOCKS == 0 )
            {
                /* Update the timeout state to see if it has expired yet. */
                यदि( x_Task_Check_For_Time_बहिर्गमनम्( &x_Time_बहिर्गमनम्, &x_Ticks_गन्तव्यम्_Wait ) == pdFALSE )
                {
                    /* Not timed out yet. Block the current task. */
                    traceBLOCKING_ON_QUEUE_RECEIVE( pxQueue );
                    vTaskPlaceOnEventList( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Receive ), x_Ticks_गन्तव्यम्_Wait );
                    पत्तनम्_YIELD_WITHIN_API();
                }
                अन्यथा
                {
                    /* We have timed out. Return an error. */
                    taskEXIT_CRITICAL( &( pxQueue->x_Queue_तालकम् ) );
                    traceQUEUE_RECEIVE_FAILED( pxQueue );
                    प्रत्यागम err_QUEUE_रिक्तानि;
                }
            }
            #यदि_अन्तः /* queueUSE_LOCKS == 0 */
        }
        taskEXIT_CRITICAL( &( pxQueue->x_Queue_तालकम् ) );

        /* If queue locks ARE being used:
         * - At this point, the queue is empty and entry time has been set
         * - We follow the original procedure for locking the queue before
         *   attempting to block. */
        #यदि ( queueUSE_LOCKS == 1 )
        {
            /* Interrupts and other tasks can send to and receive from the queue
             * now the critical section has been exited. */

            vTaskSuspendAll();
            prv_तालकम्_Queue( pxQueue );

            /* Update the timeout state to see if it has expired yet. */
            यदि( x_Task_Check_For_Time_बहिर्गमनम्( &x_Time_बहिर्गमनम्, &x_Ticks_गन्तव्यम्_Wait ) == pdFALSE )
            {
                /* The timeout has not expired.  If the queue is still empty place
                 * the task on the list of tasks waiting to receive from the queue. */
                यदि( prv_Is_Queue_रिक्तानि( pxQueue ) != pdFALSE )
                {
                    traceBLOCKING_ON_QUEUE_RECEIVE( pxQueue );
                    vTaskPlaceOnEventList( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Receive ), x_Ticks_गन्तव्यम्_Wait );
                    prvUnlockQueue( pxQueue );

                    यदि( xTaskResumeAll() == pdFALSE )
                    {
                        पत्तनम्_YIELD_WITHIN_API();
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

                यदि( prv_Is_Queue_रिक्तानि( pxQueue ) != pdFALSE )
                {
                    traceQUEUE_RECEIVE_FAILED( pxQueue );
                    प्रत्यागम err_QUEUE_रिक्तानि;
                }
                अन्यथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
        }
        #यदि_अन्तः /* queueUSE_LOCKS == 1 */
    } /*lint -restore */
}
/*-----------------------------------------------------------*/

आधारः_Type_t xQueueSemaphoreTake( QueueHandle_t xQueue,
                                TickType_t x_Ticks_गन्तव्यम्_Wait )
{
    आधारः_Type_t x_प्रविष्टिः_Time_समुच्चयः = pdFALSE;
    Time_बहिर्गमनम्_t x_Time_बहिर्गमनम्;
    Queue_t * ध्रुव pxQueue = xQueue;

    #यदि ( configUSE_MUTEXES == 1 )
        आधारः_Type_t xInheritanceOccurred = pdFALSE;
    #यदि_अन्तः

    /* Check the queue pointer is not NULL. */
    configASSERT( ( pxQueue ) );

    /* Check this really is a semaphore, in which case the item size will be
     * 0. */
    configASSERT( pxQueue->ux_Item_आकार == 0 );

    /* Cannot block if the scheduler is suspended. */
    #यदि ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_गन्तव्यम्_Wait != 0 ) ) );
    }
    #यदि_अन्तः

    /*lint -save -e904 This function relaxes the coding standard somewhat to allow return
     * statements within the function itself.  This is done in the interest
     * of execution time efficiency. */
    प्रति( ; ; )
    {
        taskENTER_CRITICAL( &( pxQueue->x_Queue_तालकम् ) );
        {
            /* Semaphores are queues with an item size of 0, and where the
             * number of messages in the queue is the semaphore's count value. */
            ध्रुव UBaseType_t ux_Semaphore_गणनम् = pxQueue->ux_Messages_प्रतीक्षा;

            /* Is there data in the queue now?  To be running the calling task
             * must be the highest priority task wanting to access the queue. */
            यदि( ux_Semaphore_गणनम् > ( UBaseType_t ) 0 )
            {
                /* Todo: Reconcile tracing differences (IDF-8183) */
                traceQUEUE_SEMAPHORE_RECEIVE( pxQueue );

                /* Semaphores are queues with a data size of zero and where the
                 * messages waiting is the semaphore's count.  Reduce the count. */
                pxQueue->ux_Messages_प्रतीक्षा = ux_Semaphore_गणनम् - ( UBaseType_t ) 1;

                #यदि ( configUSE_MUTEXES == 1 )
                {
                    यदि( pxQueue->uxQueueType == queueQUEUE_IS_MUTEX )
                    {
                        /* Record the information required to implement
                         * priority inheritance should it become necessary. */
                        pxQueue->u.xSemaphore.xMutexHolder = pv_Task_Increment_Mutex_Held_गणनम्();
                    }
                    अन्यथा
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #यदि_अन्तः /* configUSE_MUTEXES */

                /* Check to see if other tasks are blocked waiting to give the
                 * semaphore, and if so, unblock the highest priority such task. */
                यदि( list_LIST_IS_रिक्तानि( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Send ) ) == pdFALSE )
                {
                    यदि( xTaskRemoveFromEventList( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Send ) ) != pdFALSE )
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

                taskEXIT_CRITICAL( &( pxQueue->x_Queue_तालकम् ) );
                प्रत्यागम pdPASS;
            }
            अन्यथा
            {
                यदि( x_Ticks_गन्तव्यम्_Wait == ( TickType_t ) 0 )
                {
                    /* The semaphore count was 0 and no block time is specified
                     * (or the block time has expired) so exit now. */
                    taskEXIT_CRITICAL( &( pxQueue->x_Queue_तालकम् ) );
                    traceQUEUE_RECEIVE_FAILED( pxQueue );
                    प्रत्यागम err_QUEUE_रिक्तानि;
                }
                अन्यथा यदि( x_प्रविष्टिः_Time_समुच्चयः == pdFALSE )
                {
                    /* The semaphore count was 0 and a block time was specified
                     * so configure the timeout structure ready to block. */
                    v_Task_आन्तरिकम्_समुच्चयः_Time_बहिर्गमनम्_State( &x_Time_बहिर्गमनम् );
                    x_प्रविष्टिः_Time_समुच्चयः = pdTRUE;
                }
                अन्यथा
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
            #यदि ( queueUSE_LOCKS == 0 )
            {
                /* Update the timeout state to see if it has expired yet. */
                यदि( x_Task_Check_For_Time_बहिर्गमनम्( &x_Time_बहिर्गमनम्, &x_Ticks_गन्तव्यम्_Wait ) == pdFALSE )
                {
                    /* Not timed out yet. If this is a mutex, make the holder
                     * inherit our priority, then block the current task. */
                    traceBLOCKING_ON_QUEUE_RECEIVE( pxQueue );
                    #यदि ( configUSE_MUTEXES == 1 )
                    {
                        यदि( pxQueue->uxQueueType == queueQUEUE_IS_MUTEX )
                        {
                            xInheritanceOccurred = x_Task_प्राथमिकता_Inherit( pxQueue->u.xSemaphore.xMutexHolder );
                        }
                        अन्यथा
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    #यदि_अन्तः /* if ( configUSE_MUTEXES == 1 ) */
                    vTaskPlaceOnEventList( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Receive ), x_Ticks_गन्तव्यम्_Wait );
                    पत्तनम्_YIELD_WITHIN_API();
                }
                अन्यथा
                {
                    /* We have timed out. If this is a mutex, make the holder
                     * disinherit our priority, then return an error. */
                    #यदि ( configUSE_MUTEXES == 1 )
                    {
                        यदि( xInheritanceOccurred != pdFALSE )
                        {
                            UBaseType_t ux_Highest_प्रतीक्षा_प्राथमिकता;
                            ux_Highest_प्रतीक्षा_प्राथमिकता = prv_Get_Disinherit_प्राथमिकता_After_Timeout( pxQueue );
                            v_Task_प्राथमिकता_Disinherit_After_Timeout( pxQueue->u.xSemaphore.xMutexHolder, ux_Highest_प्रतीक्षा_प्राथमिकता );
                        }
                    }
                    #यदि_अन्तः /* configUSE_MUTEXES */
                    taskEXIT_CRITICAL( &( pxQueue->x_Queue_तालकम् ) );
                    traceQUEUE_RECEIVE_FAILED( pxQueue );
                    प्रत्यागम err_QUEUE_रिक्तानि;
                }
            }
            #यदि_अन्तः /* queueUSE_LOCKS == 0 */
        }
        taskEXIT_CRITICAL( &( pxQueue->x_Queue_तालकम् ) );

        /* If queue locks ARE being used:
         * - At this point, the semaphore/mutex is empty/held and entry time
         *   has been set.
         * - We follow the original procedure for locking the queue, inheriting
         *   priority, then attempting to block. */
        #यदि ( queueUSE_LOCKS == 1 )
        {
            /* Interrupts and other tasks can give to and take from the semaphore
             * now the critical section has been exited. */

            vTaskSuspendAll();
            prv_तालकम्_Queue( pxQueue );

            /* Update the timeout state to see if it has expired yet. */
            यदि( x_Task_Check_For_Time_बहिर्गमनम्( &x_Time_बहिर्गमनम्, &x_Ticks_गन्तव्यम्_Wait ) == pdFALSE )
            {
                /* A block time is specified and not expired.  If the semaphore
                 * count is 0 then enter the Blocked state to wait for a semaphore to
                 * become available.  As semaphores are implemented with queues the
                 * queue being empty is equivalent to the semaphore count being 0. */
                यदि( prv_Is_Queue_रिक्तानि( pxQueue ) != pdFALSE )
                {
                    traceBLOCKING_ON_QUEUE_RECEIVE( pxQueue );

                    #यदि ( configUSE_MUTEXES == 1 )
                    {
                        यदि( pxQueue->uxQueueType == queueQUEUE_IS_MUTEX )
                        {
                            taskENTER_CRITICAL( &( pxQueue->x_Queue_तालकम् ) );
                            {
                                xInheritanceOccurred = x_Task_प्राथमिकता_Inherit( pxQueue->u.xSemaphore.xMutexHolder );
                            }
                            taskEXIT_CRITICAL( &( pxQueue->x_Queue_तालकम् ) );
                        }
                        अन्यथा
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    #यदि_अन्तः /* if ( configUSE_MUTEXES == 1 ) */

                    vTaskPlaceOnEventList( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Receive ), x_Ticks_गन्तव्यम्_Wait );
                    prvUnlockQueue( pxQueue );

                    यदि( xTaskResumeAll() == pdFALSE )
                    {
                        पत्तनम्_YIELD_WITHIN_API();
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
                यदि( prv_Is_Queue_रिक्तानि( pxQueue ) != pdFALSE )
                {
                    #यदि ( configUSE_MUTEXES == 1 )
                    {
                        /* xInheritanceOccurred could only have be set if
                         * pxQueue->uxQueueType == queueQUEUE_IS_MUTEX so no need to
                         * test the mutex type again to check it is actually a mutex. */
                        यदि( xInheritanceOccurred != pdFALSE )
                        {
                            taskENTER_CRITICAL( &( pxQueue->x_Queue_तालकम् ) );
                            {
                                UBaseType_t ux_Highest_प्रतीक्षा_प्राथमिकता;

                                /* This task blocking on the mutex caused another
                                 * task to inherit this task's priority.  Now this task
                                 * has timed out the priority should be disinherited
                                 * again, but only as low as the next highest priority
                                 * task that is waiting for the same mutex. */
                                ux_Highest_प्रतीक्षा_प्राथमिकता = prv_Get_Disinherit_प्राथमिकता_After_Timeout( pxQueue );
                                v_Task_प्राथमिकता_Disinherit_After_Timeout( pxQueue->u.xSemaphore.xMutexHolder, ux_Highest_प्रतीक्षा_प्राथमिकता );
                            }
                            taskEXIT_CRITICAL( &( pxQueue->x_Queue_तालकम् ) );
                        }
                    }
                    #यदि_अन्तः /* configUSE_MUTEXES */

                    traceQUEUE_RECEIVE_FAILED( pxQueue );
                    प्रत्यागम err_QUEUE_रिक्तानि;
                }
                अन्यथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
        }
        #यदि_अन्तः /* queueUSE_LOCKS == 1 */
    } /*lint -restore */
}
/*-----------------------------------------------------------*/

आधारः_Type_t xQueuePeek( QueueHandle_t xQueue,
                       शून्य * ध्रुव pv_बफर,
                       TickType_t x_Ticks_गन्तव्यम्_Wait )
{
    आधारः_Type_t x_प्रविष्टिः_Time_समुच्चयः = pdFALSE;
    Time_बहिर्गमनम्_t x_Time_बहिर्गमनम्;
    int8_t * pcOriginalReadPosition;
    Queue_t * ध्रुव pxQueue = xQueue;

    /* Check the pointer is not NULL. */
    configASSERT( ( pxQueue ) );

    /* The buffer into which data is received can only be NULL if the data size
     * is zero (so no data is copied into the buffer. */
    configASSERT( !( ( ( pv_बफर ) == रिक्त ) && ( ( pxQueue )->ux_Item_आकार != ( UBaseType_t ) 0U ) ) );

    /* Cannot block if the scheduler is suspended. */
    #यदि ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_गन्तव्यम्_Wait != 0 ) ) );
    }
    #यदि_अन्तः

    /*lint -save -e904  This function relaxes the coding standard somewhat to
     * allow return statements within the function itself.  This is done in the
     * interest of execution time efficiency. */
    प्रति( ; ; )
    {
        taskENTER_CRITICAL( &( pxQueue->x_Queue_तालकम् ) );
        {
            ध्रुव UBaseType_t ux_Messages_प्रतीक्षा = pxQueue->ux_Messages_प्रतीक्षा;

            /* Is there data in the queue now?  To be running the calling task
             * must be the highest priority task wanting to access the queue. */
            यदि( ux_Messages_प्रतीक्षा > ( UBaseType_t ) 0 )
            {
                /* Remember the read position so it can be reset after the data
                 * is read from the queue as this function is only peeking the
                 * data, not removing it. */
                pcOriginalReadPosition = pxQueue->u.xQueue.pcReadFrom;

                prv_Copy_दत्त_From_Queue( pxQueue, pv_बफर );
                traceQUEUE_PEEK( pxQueue );

                /* The data is not being removed, so reset the read pointer. */
                pxQueue->u.xQueue.pcReadFrom = pcOriginalReadPosition;

                /* The data is being left in the queue, so see if there are
                 * any other tasks waiting for the data. */
                यदि( list_LIST_IS_रिक्तानि( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Receive ) ) == pdFALSE )
                {
                    यदि( xTaskRemoveFromEventList( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Receive ) ) != pdFALSE )
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

                taskEXIT_CRITICAL( &( pxQueue->x_Queue_तालकम् ) );
                प्रत्यागम pdPASS;
            }
            अन्यथा
            {
                यदि( x_Ticks_गन्तव्यम्_Wait == ( TickType_t ) 0 )
                {
                    /* The queue was empty and no block time is specified (or
                     * the block time has expired) so leave now. */
                    taskEXIT_CRITICAL( &( pxQueue->x_Queue_तालकम् ) );
                    traceQUEUE_PEEK_FAILED( pxQueue );
                    प्रत्यागम err_QUEUE_रिक्तानि;
                }
                अन्यथा यदि( x_प्रविष्टिः_Time_समुच्चयः == pdFALSE )
                {
                    /* The queue was empty and a block time was specified so
                     * configure the timeout structure ready to enter the blocked
                     * state. */
                    v_Task_आन्तरिकम्_समुच्चयः_Time_बहिर्गमनम्_State( &x_Time_बहिर्गमनम् );
                    x_प्रविष्टिः_Time_समुच्चयः = pdTRUE;
                }
                अन्यथा
                {
                    /* Entry time was already set. */
                    mtCOVERAGE_TEST_MARKER();
                }
            }

            /* If queue locks ARE NOT being used:
             * - At this point, the queue is empty and entry time has been set
             * - We simply check for a time out, block if not timed out, or
             *   return an error if we have timed out. */
            #यदि ( queueUSE_LOCKS == 0 )
            {
                /* Update the timeout state to see if it has expired yet. */
                यदि( x_Task_Check_For_Time_बहिर्गमनम्( &x_Time_बहिर्गमनम्, &x_Ticks_गन्तव्यम्_Wait ) == pdFALSE )
                {
                    /* Not timed out yet. Block the current task. */
                    traceBLOCKING_ON_QUEUE_PEEK( pxQueue );
                    vTaskPlaceOnEventList( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Receive ), x_Ticks_गन्तव्यम्_Wait );
                    पत्तनम्_YIELD_WITHIN_API();
                }
                अन्यथा
                {
                    /* We have timed out. Return an error. */
                    taskEXIT_CRITICAL( &( pxQueue->x_Queue_तालकम् ) );
                    traceQUEUE_PEEK_FAILED( pxQueue );
                    प्रत्यागम err_QUEUE_रिक्तानि;
                }
            }
            #यदि_अन्तः /* queueUSE_LOCKS == 0 */
        }
        taskEXIT_CRITICAL( &( pxQueue->x_Queue_तालकम् ) );

        /* If queue locks ARE being used:
         * - At this point, the queue is empty and entry time has been set
         * - We follow the original procedure for locking the queue before
         *   attempting to block. */
        #यदि ( queueUSE_LOCKS == 1 )
        {
            /* Interrupts and other tasks can send to and receive from the queue
             * now that the critical section has been exited. */

            vTaskSuspendAll();
            prv_तालकम्_Queue( pxQueue );

            /* Update the timeout state to see if it has expired yet. */
            यदि( x_Task_Check_For_Time_बहिर्गमनम्( &x_Time_बहिर्गमनम्, &x_Ticks_गन्तव्यम्_Wait ) == pdFALSE )
            {
                /* Timeout has not expired yet, check to see if there is data in the
                * queue now, and if not enter the Blocked state to wait for data. */
                यदि( prv_Is_Queue_रिक्तानि( pxQueue ) != pdFALSE )
                {
                    traceBLOCKING_ON_QUEUE_PEEK( pxQueue );
                    vTaskPlaceOnEventList( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Receive ), x_Ticks_गन्तव्यम्_Wait );
                    prvUnlockQueue( pxQueue );

                    यदि( xTaskResumeAll() == pdFALSE )
                    {
                        पत्तनम्_YIELD_WITHIN_API();
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

                यदि( prv_Is_Queue_रिक्तानि( pxQueue ) != pdFALSE )
                {
                    traceQUEUE_PEEK_FAILED( pxQueue );
                    प्रत्यागम err_QUEUE_रिक्तानि;
                }
                अन्यथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
        }
        #यदि_अन्तः /* queueUSE_LOCKS == 1 */
    } /*lint -restore */
}
/*-----------------------------------------------------------*/

आधारः_Type_t xQueueReceiveFromISR( QueueHandle_t xQueue,
                                 शून्य * ध्रुव pv_बफर,
                                 आधारः_Type_t * ध्रुव px_Higher_प्राथमिकता_Task_Woken )
{
    आधारः_Type_t xReturn;
    UBaseType_t ux_Saved_व्यवधानम्_स्थितिः;
    Queue_t * ध्रुव pxQueue = xQueue;

    configASSERT( pxQueue );
    configASSERT( !( ( pv_बफर == रिक्त ) && ( pxQueue->ux_Item_आकार != ( UBaseType_t ) 0U ) ) );

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
    पत्तनम्_ASSERT_IF_व्यवधानम्_प्राथमिकता_INVALID();

    prv_ENTER_CRITICAL_OR_आवरणम्_ISR( &( pxQueue->x_Queue_तालकम् ), ux_Saved_व्यवधानम्_स्थितिः );
    {
        ध्रुव UBaseType_t ux_Messages_प्रतीक्षा = pxQueue->ux_Messages_प्रतीक्षा;

        /* Cannot block in an ISR, so check there is data available. */
        यदि( ux_Messages_प्रतीक्षा > ( UBaseType_t ) 0 )
        {
            #यदि ( queueUSE_LOCKS == 1 )
                ध्रुव int8_t c_Rx_तालकम् = pxQueue->c_Rx_तालकम्;
            #अन्यथा
                /* Queue locks not used, so we treat it as unlocked. */
                ध्रुव int8_t c_Rx_तालकम् = queueUNLOCKED;
            #यदि_अन्तः /* queueUSE_LOCKS == 1 */

            traceQUEUE_RECEIVE_FROM_ISR( pxQueue );

            prv_Copy_दत्त_From_Queue( pxQueue, pv_बफर );
            pxQueue->ux_Messages_प्रतीक्षा = ux_Messages_प्रतीक्षा - ( UBaseType_t ) 1;

            /* If the queue is locked the event list will not be modified.
             * Instead update the lock count so the task that unlocks the queue
             * will know that an ISR has removed data while the queue was
             * locked. */
            यदि( c_Rx_तालकम् == queueUNLOCKED )
            {
                यदि( list_LIST_IS_रिक्तानि( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Send ) ) == pdFALSE )
                {
                    यदि( xTaskRemoveFromEventList( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Send ) ) != pdFALSE )
                    {
                        /* The task waiting has a higher priority than us so
                         * force a context switch. */
                        यदि( px_Higher_प्राथमिकता_Task_Woken != रिक्त )
                        {
                            *px_Higher_प्राथमिकता_Task_Woken = pdTRUE;
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
                #यदि ( queueUSE_LOCKS == 1 )
                {
                    /* Increment the lock count so the task that unlocks the queue
                     * knows that data was removed while it was locked. */
                    prv_Increment_Queue_Rx_तालकम्( pxQueue, c_Rx_तालकम् );
                }
                #यदि_अन्तः /* queueUSE_LOCKS == 1 */
            }

            xReturn = pdPASS;
        }
        अन्यथा
        {
            xReturn = pdFAIL;
            traceQUEUE_RECEIVE_FROM_ISR_FAILED( pxQueue );
        }
    }
    prvEXIT_CRITICAL_OR_UNMASK_ISR( &( pxQueue->x_Queue_तालकम् ), ux_Saved_व्यवधानम्_स्थितिः );

    प्रत्यागम xReturn;
}
/*-----------------------------------------------------------*/

आधारः_Type_t xQueuePeekFromISR( QueueHandle_t xQueue,
                              शून्य * ध्रुव pv_बफर )
{
    आधारः_Type_t xReturn;
    UBaseType_t ux_Saved_व्यवधानम्_स्थितिः;
    int8_t * pcOriginalReadPosition;
    Queue_t * ध्रुव pxQueue = xQueue;

    configASSERT( pxQueue );
    configASSERT( !( ( pv_बफर == रिक्त ) && ( pxQueue->ux_Item_आकार != ( UBaseType_t ) 0U ) ) );
    configASSERT( pxQueue->ux_Item_आकार != 0 ); /* Can't peek a semaphore. */

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
    पत्तनम्_ASSERT_IF_व्यवधानम्_प्राथमिकता_INVALID();

    prv_ENTER_CRITICAL_OR_आवरणम्_ISR( &( pxQueue->x_Queue_तालकम् ), ux_Saved_व्यवधानम्_स्थितिः );
    {
        /* Cannot block in an ISR, so check there is data available. */
        यदि( pxQueue->ux_Messages_प्रतीक्षा > ( UBaseType_t ) 0 )
        {
            traceQUEUE_PEEK_FROM_ISR( pxQueue );

            /* Remember the read position so it can be reset as nothing is
             * actually being removed from the queue. */
            pcOriginalReadPosition = pxQueue->u.xQueue.pcReadFrom;
            prv_Copy_दत्त_From_Queue( pxQueue, pv_बफर );
            pxQueue->u.xQueue.pcReadFrom = pcOriginalReadPosition;

            xReturn = pdPASS;
        }
        अन्यथा
        {
            xReturn = pdFAIL;
            traceQUEUE_PEEK_FROM_ISR_FAILED( pxQueue );
        }
    }
    prvEXIT_CRITICAL_OR_UNMASK_ISR( &( pxQueue->x_Queue_तालकम् ), ux_Saved_व्यवधानम्_स्थितिः );

    प्रत्यागम xReturn;
}
/*-----------------------------------------------------------*/

UBaseType_t ux_Queue_Messages_प्रतीक्षा( ध्रुव QueueHandle_t xQueue )
{
    UBaseType_t uxReturn;

    configASSERT( xQueue );

    taskENTER_CRITICAL( &( ( ( Queue_t * ) xQueue )->x_Queue_तालकम् ) );
    {
        uxReturn = ( ( Queue_t * ) xQueue )->ux_Messages_प्रतीक्षा;
    }
    taskEXIT_CRITICAL( &( ( ( Queue_t * ) xQueue )->x_Queue_तालकम् ) );

    प्रत्यागम uxReturn;
} /*lint !e818 Pointer cannot be declared const as xQueue is a typedef not pointer. */
/*-----------------------------------------------------------*/

UBaseType_t ux_Queue_Spaces_उपलब्धम्( ध्रुव QueueHandle_t xQueue )
{
    UBaseType_t uxReturn;
    Queue_t * ध्रुव pxQueue = xQueue;

    configASSERT( pxQueue );

    taskENTER_CRITICAL( &( pxQueue->x_Queue_तालकम् ) );
    {
        uxReturn = pxQueue->ux_दीर्घता - pxQueue->ux_Messages_प्रतीक्षा;
    }
    taskEXIT_CRITICAL( &( pxQueue->x_Queue_तालकम् ) );

    प्रत्यागम uxReturn;
} /*lint !e818 Pointer cannot be declared const as xQueue is a typedef not pointer. */
/*-----------------------------------------------------------*/

UBaseType_t ux_Queue_Messages_प्रतीक्षा_From_ISR( ध्रुव QueueHandle_t xQueue )
{
    UBaseType_t uxReturn;
    Queue_t * ध्रुव pxQueue = xQueue;

    configASSERT( pxQueue );
    uxReturn = pxQueue->ux_Messages_प्रतीक्षा;

    प्रत्यागम uxReturn;
} /*lint !e818 Pointer cannot be declared const as xQueue is a typedef not pointer. */
/*-----------------------------------------------------------*/

शून्य vQueueDelete( QueueHandle_t xQueue )
{
    Queue_t * ध्रुव pxQueue = xQueue;

    configASSERT( pxQueue );
    traceQUEUE_DELETE( pxQueue );

    #यदि ( config_QUEUE_REGISTRY_आकार > 0 )
    {
        vQueueUnregisterQueue( pxQueue );
    }
    #यदि_अन्तः

    #यदि ( ( config_SUPPORT_DYNAMIC_आवण्टनम् == 1 ) && ( config_SUPPORT_STATIC_आवण्टनम् == 0 ) )
    {
        /* The queue can only have been allocated dynamically - free it
         * again. */
        v_पत्तनम्_Free( pxQueue );
    }
    #नोचेद्यदि ( ( config_SUPPORT_DYNAMIC_आवण्टनम् == 1 ) && ( config_SUPPORT_STATIC_आवण्टनम् == 1 ) )
    {
        /* The queue could have been allocated statically or dynamically, so
         * check before attempting to free the memory. */
        यदि( pxQueue->ucStaticallyAllocated == ( uint8_t ) pdFALSE )
        {
            v_पत्तनम्_Free( pxQueue );
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
    #यदि_अन्तः /* configSUPPORT_DYNAMIC_ALLOCATION */
}
/*-----------------------------------------------------------*/

#यदि ( configUSE_TRACE_FACILITY == 1 )

    UBaseType_t ux_Queue_Get_Queue_सङ्ख्या( QueueHandle_t xQueue )
    {
        प्रत्यागम ( ( Queue_t * ) xQueue )->ux_Queue_सङ्ख्या;
    }

#यदि_अन्तः /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#यदि ( configUSE_TRACE_FACILITY == 1 )

    शून्य v_Queue_समुच्चयः_Queue_सङ्ख्या( QueueHandle_t xQueue,
                               UBaseType_t ux_Queue_सङ्ख्या )
    {
        ( ( Queue_t * ) xQueue )->ux_Queue_सङ्ख्या = ux_Queue_सङ्ख्या;
    }

#यदि_अन्तः /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#यदि ( configUSE_TRACE_FACILITY == 1 )

    uint8_t ucQueueGetQueueType( QueueHandle_t xQueue )
    {
        प्रत्यागम ( ( Queue_t * ) xQueue )->ucQueueType;
    }

#यदि_अन्तः /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#यदि ( configUSE_MUTEXES == 1 )

    स्थिर UBaseType_t prv_Get_Disinherit_प्राथमिकता_After_Timeout( ध्रुव Queue_t * ध्रुव pxQueue )
    {
        UBaseType_t ux_Highest_प्राथमिकता_सम्बन्धि_प्रतीक्षा_Tasks;

        /* If a task waiting for a mutex causes the mutex holder to inherit a
         * priority, but the waiting task times out, then the holder should
         * disinherit the priority - but only down to the highest priority of any
         * other tasks that are waiting for the same mutex.  For this purpose,
         * return the priority of the highest priority task that is waiting for the
         * mutex. */
        यदि( list_CURRENT_LIST_दीर्घता( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Receive ) ) > 0U )
        {
            ux_Highest_प्राथमिकता_सम्बन्धि_प्रतीक्षा_Tasks = ( UBaseType_t ) configMAX_PRIORITIES - ( UBaseType_t ) list_GET_ITEM_मानम्_सम्बन्धि_शीर्ष_प्रविष्टिः( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Receive ) );
        }
        अन्यथा
        {
            ux_Highest_प्राथमिकता_सम्बन्धि_प्रतीक्षा_Tasks = tsk_IDLE_प्राथमिकता;
        }

        प्रत्यागम ux_Highest_प्राथमिकता_सम्बन्धि_प्रतीक्षा_Tasks;
    }

#यदि_अन्तः /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

स्थिर आधारः_Type_t prv_Copy_दत्त_गन्तव्यम्_Queue( Queue_t * ध्रुव pxQueue,
                                      ध्रुव शून्य * pv_Item_गन्तव्यम्_Queue,
                                      ध्रुव आधारः_Type_t xPosition )
{
    आधारः_Type_t xReturn = pdFALSE;
    UBaseType_t ux_Messages_प्रतीक्षा;

    /* This function is called from a critical section. */

    ux_Messages_प्रतीक्षा = pxQueue->ux_Messages_प्रतीक्षा;

    यदि( pxQueue->ux_Item_आकार == ( UBaseType_t ) 0 )
    {
        #यदि ( configUSE_MUTEXES == 1 )
        {
            यदि( pxQueue->uxQueueType == queueQUEUE_IS_MUTEX )
            {
                /* The mutex is no longer being held. */
                xReturn = x_Task_प्राथमिकता_Disinherit( pxQueue->u.xSemaphore.xMutexHolder );
                pxQueue->u.xSemaphore.xMutexHolder = रिक्त;
            }
            अन्यथा
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        #यदि_अन्तः /* configUSE_MUTEXES */
    }
    अन्यथा यदि( xPosition == queue_SEND_गन्तव्यम्_पृष्ठभागः )
    {
        ( शून्य ) स्मृतिं_प्रतिलिख( ( शून्य * ) pxQueue->pc_Write_गन्तव्यम्, pv_Item_गन्तव्यम्_Queue, ( size_t ) pxQueue->ux_Item_आकार ); /*lint !e961 !e418 !e9087 MISRA exception as the casts are only redundant for some ports, plus previous logic ensures a null pointer can only be passed to memcpy() if the copy size is 0.  Cast to void required by function signature and safe as no alignment requirement and copy length specified in bytes. */
        pxQueue->pc_Write_गन्तव्यम् += pxQueue->ux_Item_आकार;                                                       /*lint !e9016 Pointer arithmetic on char types ok, especially in this use case where it is the clearest way of conveying intent. */

        यदि( pxQueue->pc_Write_गन्तव्यम् >= pxQueue->u.xQueue.pc_पुच्छ )                                             /*lint !e946 MISRA exception justified as comparison of pointers is the cleanest solution. */
        {
            pxQueue->pc_Write_गन्तव्यम् = pxQueue->pc_शीर्ष;
        }
        अन्यथा
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    अन्यथा
    {
        ( शून्य ) स्मृतिं_प्रतिलिख( ( शून्य * ) pxQueue->u.xQueue.pcReadFrom, pv_Item_गन्तव्यम्_Queue, ( size_t ) pxQueue->ux_Item_आकार ); /*lint !e961 !e9087 !e418 MISRA exception as the casts are only redundant for some ports.  Cast to void required by function signature and safe as no alignment requirement and copy length specified in bytes.  Assert checks null pointer only used when length is 0. */
        pxQueue->u.xQueue.pcReadFrom -= pxQueue->ux_Item_आकार;

        यदि( pxQueue->u.xQueue.pcReadFrom < pxQueue->pc_शीर्ष ) /*lint !e946 MISRA exception justified as comparison of pointers is the cleanest solution. */
        {
            pxQueue->u.xQueue.pcReadFrom = ( pxQueue->u.xQueue.pc_पुच्छ - pxQueue->ux_Item_आकार );
        }
        अन्यथा
        {
            mtCOVERAGE_TEST_MARKER();
        }

        यदि( xPosition == queueOVERWRITE )
        {
            यदि( ux_Messages_प्रतीक्षा > ( UBaseType_t ) 0 )
            {
                /* An item is not being added but overwritten, so subtract
                 * one from the recorded number of items in the queue so when
                 * one is added again below the number of recorded items remains
                 * correct. */
                --ux_Messages_प्रतीक्षा;
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

    pxQueue->ux_Messages_प्रतीक्षा = ux_Messages_प्रतीक्षा + ( UBaseType_t ) 1;

    प्रत्यागम xReturn;
}
/*-----------------------------------------------------------*/

स्थिर शून्य prv_Copy_दत्त_From_Queue( Queue_t * ध्रुव pxQueue,
                                  शून्य * ध्रुव pv_बफर )
{
    यदि( pxQueue->ux_Item_आकार != ( UBaseType_t ) 0 )
    {
        pxQueue->u.xQueue.pcReadFrom += pxQueue->ux_Item_आकार;           /*lint !e9016 Pointer arithmetic on char types ok, especially in this use case where it is the clearest way of conveying intent. */

        यदि( pxQueue->u.xQueue.pcReadFrom >= pxQueue->u.xQueue.pc_पुच्छ ) /*lint !e946 MISRA exception justified as use of the relational operator is the cleanest solutions. */
        {
            pxQueue->u.xQueue.pcReadFrom = pxQueue->pc_शीर्ष;
        }
        अन्यथा
        {
            mtCOVERAGE_TEST_MARKER();
        }

        ( शून्य ) स्मृतिं_प्रतिलिख( ( शून्य * ) pv_बफर, ( शून्य * ) pxQueue->u.xQueue.pcReadFrom, ( size_t ) pxQueue->ux_Item_आकार ); /*lint !e961 !e418 !e9087 MISRA exception as the casts are only redundant for some ports.  Also previous logic ensures a null pointer can only be passed to memcpy() when the count is 0.  Cast to void required by function signature and safe as no alignment requirement and copy length specified in bytes. */
    }
}
/*-----------------------------------------------------------*/

#यदि ( queueUSE_LOCKS == 1 )
    स्थिर शून्य prvUnlockQueue( Queue_t * ध्रुव pxQueue )
    {
        /* THIS FUNCTION MUST BE CALLED WITH THE SCHEDULER SUSPENDED. */

        /* The lock counts contains the number of extra data items placed or
         * removed from the queue while the queue was locked.  When a queue is
         * locked items can be added or removed, but the event lists cannot be
         * updated. */
        taskENTER_CRITICAL( &( pxQueue->x_Queue_तालकम् ) );
        {
            int8_t c_Tx_तालकम् = pxQueue->c_Tx_तालकम्;

            /* See if data was added to the queue while it was locked. */
            यावत्( c_Tx_तालकम् > queueLOCKED_UNMODIFIED )
            {
                /* Data was posted while the queue was locked.  Are any tasks
                 * blocked waiting for data to become available? */
                #यदि ( configUSE_QUEUE_SETS == 1 )
                {
                    यदि( pxQueue->px_Queue_समुच्चयः_Container != रिक्त )
                    {
                        यदि( prv_Notify_Queue_समुच्चयः_Container( pxQueue ) != pdFALSE )
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
                        यदि( list_LIST_IS_रिक्तानि( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Receive ) ) == pdFALSE )
                        {
                            यदि( xTaskRemoveFromEventList( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Receive ) ) != pdFALSE )
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
                    यदि( list_LIST_IS_रिक्तानि( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Receive ) ) == pdFALSE )
                    {
                        यदि( xTaskRemoveFromEventList( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Receive ) ) != pdFALSE )
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
                #यदि_अन्तः /* configUSE_QUEUE_SETS */

                --c_Tx_तालकम्;
            }

            pxQueue->c_Tx_तालकम् = queueUNLOCKED;
        }
        taskEXIT_CRITICAL( &( pxQueue->x_Queue_तालकम् ) );

        /* Do the same for the Rx lock. */
        taskENTER_CRITICAL( &( pxQueue->x_Queue_तालकम् ) );
        {
            int8_t c_Rx_तालकम् = pxQueue->c_Rx_तालकम्;

            यावत्( c_Rx_तालकम् > queueLOCKED_UNMODIFIED )
            {
                यदि( list_LIST_IS_रिक्तानि( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Send ) ) == pdFALSE )
                {
                    यदि( xTaskRemoveFromEventList( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Send ) ) != pdFALSE )
                    {
                        vTaskMissedYield();
                    }
                    अन्यथा
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }

                    --c_Rx_तालकम्;
                }
                अन्यथा
                {
                    भङ्ग;
                }
            }

            pxQueue->c_Rx_तालकम् = queueUNLOCKED;
        }
        taskEXIT_CRITICAL( &( pxQueue->x_Queue_तालकम् ) );
    }
#यदि_अन्तः /* queueUSE_LOCKS == 1 */
/*-----------------------------------------------------------*/

#यदि ( queueUSE_LOCKS == 1 )
    स्थिर आधारः_Type_t prv_Is_Queue_रिक्तानि( ध्रुव Queue_t * pxQueue )
    {
        आधारः_Type_t xReturn;

        taskENTER_CRITICAL( &( ( ( Queue_t * ) pxQueue )->x_Queue_तालकम् ) );
        {
            यदि( pxQueue->ux_Messages_प्रतीक्षा == ( UBaseType_t ) 0 )
            {
                xReturn = pdTRUE;
            }
            अन्यथा
            {
                xReturn = pdFALSE;
            }
        }
        taskEXIT_CRITICAL( &( ( ( Queue_t * ) pxQueue )->x_Queue_तालकम् ) );

        प्रत्यागम xReturn;
    }
#यदि_अन्तः /* queueUSE_LOCKS == 1 */
/*-----------------------------------------------------------*/

आधारः_Type_t x_Queue_Is_Queue_रिक्तानि_From_ISR( ध्रुव QueueHandle_t xQueue )
{
    आधारः_Type_t xReturn;
    Queue_t * ध्रुव pxQueue = xQueue;

    configASSERT( pxQueue );

    यदि( pxQueue->ux_Messages_प्रतीक्षा == ( UBaseType_t ) 0 )
    {
        xReturn = pdTRUE;
    }
    अन्यथा
    {
        xReturn = pdFALSE;
    }

    प्रत्यागम xReturn;
} /*lint !e818 xQueue could not be pointer to const because it is a typedef. */
/*-----------------------------------------------------------*/

#यदि ( queueUSE_LOCKS == 1 )
    स्थिर आधारः_Type_t prv_Is_Queue_पूर्णम्( ध्रुव Queue_t * pxQueue )
    {
        आधारः_Type_t xReturn;

        taskENTER_CRITICAL( &( ( ( Queue_t * ) pxQueue )->x_Queue_तालकम् ) );
        {
            यदि( pxQueue->ux_Messages_प्रतीक्षा == pxQueue->ux_दीर्घता )
            {
                xReturn = pdTRUE;
            }
            अन्यथा
            {
                xReturn = pdFALSE;
            }
        }
        taskEXIT_CRITICAL( &( ( ( Queue_t * ) pxQueue )->x_Queue_तालकम् ) );

        प्रत्यागम xReturn;
    }
#यदि_अन्तः /* queueUSE_LOCKS == 1 */
/*-----------------------------------------------------------*/

आधारः_Type_t x_Queue_Is_Queue_पूर्णम्_From_ISR( ध्रुव QueueHandle_t xQueue )
{
    आधारः_Type_t xReturn;
    Queue_t * ध्रुव pxQueue = xQueue;

    configASSERT( pxQueue );

    यदि( pxQueue->ux_Messages_प्रतीक्षा == pxQueue->ux_दीर्घता )
    {
        xReturn = pdTRUE;
    }
    अन्यथा
    {
        xReturn = pdFALSE;
    }

    प्रत्यागम xReturn;
} /*lint !e818 xQueue could not be pointer to const because it is a typedef. */
/*-----------------------------------------------------------*/

#यदि ( configUSE_CO_ROUTINES == 1 )

    आधारः_Type_t xQueueCRSend( QueueHandle_t xQueue,
                             ध्रुव शून्य * pv_Item_गन्तव्यम्_Queue,
                             TickType_t x_Ticks_गन्तव्यम्_Wait )
    {
        आधारः_Type_t xReturn;
        Queue_t * ध्रुव pxQueue = xQueue;

        /* If the queue is already full we may have to block.  A critical section
         * is required to prevent an interrupt removing something from the queue
         * between the check to see if the queue is full and blocking on the queue. */
        पत्तनम्_DISABLE_INTERRUPTS();
        {
            यदि( prv_Is_Queue_पूर्णम्( pxQueue ) != pdFALSE )
            {
                /* The queue is full - do we want to block or just leave without
                 * posting? */
                यदि( x_Ticks_गन्तव्यम्_Wait > ( TickType_t ) 0 )
                {
                    /* As this is called from a coroutine we cannot block directly, but
                     * return indicating that we need to block. */
                    v_Co_Routine_सङ्कलय_गन्तव्यम्_Delayed_List( x_Ticks_गन्तव्यम्_Wait, &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Send ) );
                    पत्तनम्_ENABLE_INTERRUPTS();
                    प्रत्यागम errQUEUE_BLOCKED;
                }
                अन्यथा
                {
                    पत्तनम्_ENABLE_INTERRUPTS();
                    प्रत्यागम err_QUEUE_पूर्णम्;
                }
            }
        }
        पत्तनम्_ENABLE_INTERRUPTS();

        पत्तनम्_DISABLE_INTERRUPTS();
        {
            यदि( pxQueue->ux_Messages_प्रतीक्षा < pxQueue->ux_दीर्घता )
            {
                /* There is room in the queue, copy the data into the queue. */
                prv_Copy_दत्त_गन्तव्यम्_Queue( pxQueue, pv_Item_गन्तव्यम्_Queue, queue_SEND_गन्तव्यम्_पृष्ठभागः );
                xReturn = pdPASS;

                /* Were any co-routines waiting for data to become available? */
                यदि( list_LIST_IS_रिक्तानि( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Receive ) ) == pdFALSE )
                {
                    /* In this instance the co-routine could be placed directly
                     * into the ready list as we are within a critical section.
                     * Instead the same pending ready list mechanism is used as if
                     * the event were caused from within an interrupt. */
                    यदि( xCoRoutineRemoveFromEventList( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Receive ) ) != pdFALSE )
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
                xReturn = err_QUEUE_पूर्णम्;
            }
        }
        पत्तनम्_ENABLE_INTERRUPTS();

        प्रत्यागम xReturn;
    }

#यदि_अन्तः /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#यदि ( configUSE_CO_ROUTINES == 1 )

    आधारः_Type_t xQueueCRReceive( QueueHandle_t xQueue,
                                शून्य * pv_बफर,
                                TickType_t x_Ticks_गन्तव्यम्_Wait )
    {
        आधारः_Type_t xReturn;
        Queue_t * ध्रुव pxQueue = xQueue;

        /* If the queue is already empty we may have to block.  A critical section
         * is required to prevent an interrupt adding something to the queue
         * between the check to see if the queue is empty and blocking on the queue. */
        पत्तनम्_DISABLE_INTERRUPTS();
        {
            यदि( pxQueue->ux_Messages_प्रतीक्षा == ( UBaseType_t ) 0 )
            {
                /* There are no messages in the queue, do we want to block or just
                 * leave with nothing? */
                यदि( x_Ticks_गन्तव्यम्_Wait > ( TickType_t ) 0 )
                {
                    /* As this is a co-routine we cannot block directly, but return
                     * indicating that we need to block. */
                    v_Co_Routine_सङ्कलय_गन्तव्यम्_Delayed_List( x_Ticks_गन्तव्यम्_Wait, &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Receive ) );
                    पत्तनम्_ENABLE_INTERRUPTS();
                    प्रत्यागम errQUEUE_BLOCKED;
                }
                अन्यथा
                {
                    पत्तनम्_ENABLE_INTERRUPTS();
                    प्रत्यागम err_QUEUE_पूर्णम्;
                }
            }
            अन्यथा
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        पत्तनम्_ENABLE_INTERRUPTS();

        पत्तनम्_DISABLE_INTERRUPTS();
        {
            यदि( pxQueue->ux_Messages_प्रतीक्षा > ( UBaseType_t ) 0 )
            {
                /* Data is available from the queue. */
                pxQueue->u.xQueue.pcReadFrom += pxQueue->ux_Item_आकार;

                यदि( pxQueue->u.xQueue.pcReadFrom >= pxQueue->u.xQueue.pc_पुच्छ )
                {
                    pxQueue->u.xQueue.pcReadFrom = pxQueue->pc_शीर्ष;
                }
                अन्यथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                --( pxQueue->ux_Messages_प्रतीक्षा );
                ( शून्य ) स्मृतिं_प्रतिलिख( ( शून्य * ) pv_बफर, ( शून्य * ) pxQueue->u.xQueue.pcReadFrom, ( अचिह्नित ) pxQueue->ux_Item_आकार );

                xReturn = pdPASS;

                /* Were any co-routines waiting for space to become available? */
                यदि( list_LIST_IS_रिक्तानि( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Send ) ) == pdFALSE )
                {
                    /* In this instance the co-routine could be placed directly
                     * into the ready list as we are within a critical section.
                     * Instead the same pending ready list mechanism is used as if
                     * the event were caused from within an interrupt. */
                    यदि( xCoRoutineRemoveFromEventList( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Send ) ) != pdFALSE )
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
        पत्तनम्_ENABLE_INTERRUPTS();

        प्रत्यागम xReturn;
    }

#यदि_अन्तः /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#यदि ( configUSE_CO_ROUTINES == 1 )

    आधारः_Type_t xQueueCRSendFromISR( QueueHandle_t xQueue,
                                    ध्रुव शून्य * pv_Item_गन्तव्यम्_Queue,
                                    आधारः_Type_t xCoRoutinePreviouslyWoken )
    {
        Queue_t * ध्रुव pxQueue = xQueue;

        /* Cannot block within an ISR so if there is no space on the queue then
         * exit without doing anything. */
        यदि( pxQueue->ux_Messages_प्रतीक्षा < pxQueue->ux_दीर्घता )
        {
            prv_Copy_दत्त_गन्तव्यम्_Queue( pxQueue, pv_Item_गन्तव्यम्_Queue, queue_SEND_गन्तव्यम्_पृष्ठभागः );

            /* We only want to wake one co-routine per ISR, so check that a
             * co-routine has not already been woken. */
            यदि( xCoRoutinePreviouslyWoken == pdFALSE )
            {
                यदि( list_LIST_IS_रिक्तानि( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Receive ) ) == pdFALSE )
                {
                    यदि( xCoRoutineRemoveFromEventList( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Receive ) ) != pdFALSE )
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

        प्रत्यागम xCoRoutinePreviouslyWoken;
    }

#यदि_अन्तः /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#यदि ( configUSE_CO_ROUTINES == 1 )

    आधारः_Type_t xQueueCRReceiveFromISR( QueueHandle_t xQueue,
                                       शून्य * pv_बफर,
                                       आधारः_Type_t * pxCoRoutineWoken )
    {
        आधारः_Type_t xReturn;
        Queue_t * ध्रुव pxQueue = xQueue;

        /* We cannot block from an ISR, so check there is data available. If
         * not then just leave without doing anything. */
        यदि( pxQueue->ux_Messages_प्रतीक्षा > ( UBaseType_t ) 0 )
        {
            /* Copy the data from the queue. */
            pxQueue->u.xQueue.pcReadFrom += pxQueue->ux_Item_आकार;

            यदि( pxQueue->u.xQueue.pcReadFrom >= pxQueue->u.xQueue.pc_पुच्छ )
            {
                pxQueue->u.xQueue.pcReadFrom = pxQueue->pc_शीर्ष;
            }
            अन्यथा
            {
                mtCOVERAGE_TEST_MARKER();
            }

            --( pxQueue->ux_Messages_प्रतीक्षा );
            ( शून्य ) स्मृतिं_प्रतिलिख( ( शून्य * ) pv_बफर, ( शून्य * ) pxQueue->u.xQueue.pcReadFrom, ( अचिह्नित ) pxQueue->ux_Item_आकार );

            यदि( ( *pxCoRoutineWoken ) == pdFALSE )
            {
                यदि( list_LIST_IS_रिक्तानि( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Send ) ) == pdFALSE )
                {
                    यदि( xCoRoutineRemoveFromEventList( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Send ) ) != pdFALSE )
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

        प्रत्यागम xReturn;
    }

#यदि_अन्तः /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#यदि ( config_QUEUE_REGISTRY_आकार > 0 )

    शून्य v_Queue_सङ्कलय_गन्तव्यम्_Registry( QueueHandle_t xQueue,
                              ध्रुव वर्ण * pcQueueName ) /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
    {
        UBaseType_t ux;
        QueueRegistryItem_t * px_प्रविष्टिः_गन्तव्यम्_Write = रिक्त;

        configASSERT( xQueue );

        /* For SMP, we need to take the queue registry lock in case another
         * core updates the register simultaneously. */
        prvENTER_CRITICAL_SMP_ONLY( &x_Queue_Registry_तालकम् );
        {
            यदि( pcQueueName != रिक्त )
            {
                /* See if there is an empty space in the registry.  A NULL name denotes
                 * a free slot. */
                प्रति( ux = ( UBaseType_t ) 0U; ux < ( UBaseType_t ) config_QUEUE_REGISTRY_आकार; ux++ )
                {
                    /* Replace an existing entry if the queue is already in the registry. */
                    यदि( xQueue == xQueueRegistry[ ux ].xHandle )
                    {
                        px_प्रविष्टिः_गन्तव्यम्_Write = &( xQueueRegistry[ ux ] );
                        भङ्ग;
                    }
                    /* Otherwise, store in the next empty location */
                    अन्यथा यदि( ( px_प्रविष्टिः_गन्तव्यम्_Write == रिक्त ) && ( xQueueRegistry[ ux ].pcQueueName == रिक्त ) )
                    {
                        px_प्रविष्टिः_गन्तव्यम्_Write = &( xQueueRegistry[ ux ] );
                    }
                    अन्यथा
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
            }

            यदि( px_प्रविष्टिः_गन्तव्यम्_Write != रिक्त )
            {
                /* Store the information on this queue. */
                px_प्रविष्टिः_गन्तव्यम्_Write->pcQueueName = pcQueueName;
                px_प्रविष्टिः_गन्तव्यम्_Write->xHandle = xQueue;

                trace_QUEUE_REGISTRY_सङ्कलय( xQueue, pcQueueName );
            }
        }
        /* Release the previously taken queue registry lock. */
        prvEXIT_CRITICAL_SMP_ONLY( &x_Queue_Registry_तालकम् );
    }

#यदि_अन्तः /* configQUEUE_REGISTRY_SIZE */
/*-----------------------------------------------------------*/

#यदि ( config_QUEUE_REGISTRY_आकार > 0 )

    ध्रुव वर्ण * pcQueueGetName( QueueHandle_t xQueue ) /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
    {
        UBaseType_t ux;
        ध्रुव वर्ण * pcReturn = रिक्त; /*lint !e971 Unqualified char types are allowed for strings and single characters only. */

        configASSERT( xQueue );

        /* For SMP, we need to take the queue registry lock in case another
         * core updates the register simultaneously. */
        prvENTER_CRITICAL_SMP_ONLY( &x_Queue_Registry_तालकम् );
        {
            /* Note there is nothing here to protect against another task adding or
             * removing entries from the registry while it is being searched. */

            प्रति( ux = ( UBaseType_t ) 0U; ux < ( UBaseType_t ) config_QUEUE_REGISTRY_आकार; ux++ )
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
        }
        /* Release the previously taken queue registry lock. */
        prvEXIT_CRITICAL_SMP_ONLY( &x_Queue_Registry_तालकम् );

        प्रत्यागम pcReturn;
    } /*lint !e818 xQueue cannot be a pointer to const because it is a typedef. */

#यदि_अन्तः /* configQUEUE_REGISTRY_SIZE */
/*-----------------------------------------------------------*/

#यदि ( config_QUEUE_REGISTRY_आकार > 0 )

    शून्य vQueueUnregisterQueue( QueueHandle_t xQueue )
    {
        UBaseType_t ux;

        configASSERT( xQueue );

        /* For SMP, we need to take the queue registry lock in case another
         * core updates the register simultaneously. */
        prvENTER_CRITICAL_SMP_ONLY( &x_Queue_Registry_तालकम् );
        {
            /* See if the handle of the queue being unregistered in actually in the
             * registry. */
            प्रति( ux = ( UBaseType_t ) 0U; ux < ( UBaseType_t ) config_QUEUE_REGISTRY_आकार; ux++ )
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
        }
        /* Release the previously taken queue registry lock. */
        prvEXIT_CRITICAL_SMP_ONLY( &x_Queue_Registry_तालकम् );
    } /*lint !e818 xQueue could not be pointer to const because it is a typedef. */

#यदि_अन्तः /* configQUEUE_REGISTRY_SIZE */
/*-----------------------------------------------------------*/

#यदि ( configUSE_TIMERS == 1 )

    शून्य v_Queue_Wait_For_सन्देशः_Restricted( QueueHandle_t xQueue,
                                         TickType_t x_Ticks_गन्तव्यम्_Wait,
                                         ध्रुव आधारः_Type_t xWaitIndefinitely )
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
        prvENTER_CRITICAL_SMP_ONLY( &( pxQueue->x_Queue_तालकम् ) );
        {
            #यदि ( queueUSE_LOCKS == 1 )
            {
                /* Only do anything if there are no messages in the queue.  This function
                 *  will not actually cause the task to block, just place it on a blocked
                 *  list.  It will not block until the scheduler is unlocked - at which
                 *  time a yield will be performed.  If an item is added to the queue while
                 *  the queue is locked, and the calling task blocks on the queue, then the
                 *  calling task will be immediately unblocked when the queue is unlocked. */
                prv_तालकम्_Queue( pxQueue );
            }
            #यदि_अन्तः /* queueUSE_LOCKS == 1 */

            यदि( pxQueue->ux_Messages_प्रतीक्षा == ( UBaseType_t ) 0U )
            {
                /* There is nothing in the queue, block for the specified period. */
                vTaskPlaceOnEventListRestricted( &( pxQueue->x_Tasks_प्रतीक्षा_गन्तव्यम्_Receive ), x_Ticks_गन्तव्यम्_Wait, xWaitIndefinitely );
            }
            अन्यथा
            {
                mtCOVERAGE_TEST_MARKER();
            }

            #यदि ( queueUSE_LOCKS == 1 )
            {
                prvUnlockQueue( pxQueue );
            }
            #यदि_अन्तः /* queueUSE_LOCKS == 1 */
        }
        /* Release the previously taken xQueueLock. */
        prvEXIT_CRITICAL_SMP_ONLY( &( pxQueue->x_Queue_तालकम् ) );
    }

#यदि_अन्तः /* configUSE_TIMERS */
/*-----------------------------------------------------------*/

#यदि ( ( configUSE_QUEUE_SETS == 1 ) && ( config_SUPPORT_DYNAMIC_आवण्टनम् == 1 ) )

    Queue_समुच्चयः_Handle_t x_Queue_Create_समुच्चयः( ध्रुव UBaseType_t ux_Event_Queue_दीर्घता )
    {
        Queue_समुच्चयः_Handle_t pxQueue;

        pxQueue = xQueueGenericCreate( ux_Event_Queue_दीर्घता, ( UBaseType_t ) परिमाण( Queue_t * ), queue_QUEUE_TYPE_समुच्चयः );

        प्रत्यागम pxQueue;
    }

#यदि_अन्तः /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#यदि ( configUSE_QUEUE_SETS == 1 )

    आधारः_Type_t x_Queue_सङ्कलय_गन्तव्यम्_समुच्चयः( Queue_समुच्चयः_Member_Handle_t xQueueOrSemaphore,
                               Queue_समुच्चयः_Handle_t x_Queue_समुच्चयः )
    {
        आधारः_Type_t xReturn;

        taskENTER_CRITICAL( &( ( ( Queue_t * ) xQueueOrSemaphore )->x_Queue_तालकम् ) );
        {
            यदि( ( ( Queue_t * ) xQueueOrSemaphore )->px_Queue_समुच्चयः_Container != रिक्त )
            {
                /* Cannot add a queue/semaphore to more than one queue set. */
                xReturn = pdFAIL;
            }
            अन्यथा यदि( ( ( Queue_t * ) xQueueOrSemaphore )->ux_Messages_प्रतीक्षा != ( UBaseType_t ) 0 )
            {
                /* Cannot add a queue/semaphore to a queue set if there are already
                 * items in the queue/semaphore. */
                xReturn = pdFAIL;
            }
            अन्यथा
            {
                ( ( Queue_t * ) xQueueOrSemaphore )->px_Queue_समुच्चयः_Container = x_Queue_समुच्चयः;
                xReturn = pdPASS;
            }
        }
        taskEXIT_CRITICAL( &( ( ( Queue_t * ) xQueueOrSemaphore )->x_Queue_तालकम् ) );

        प्रत्यागम xReturn;
    }

#यदि_अन्तः /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#यदि ( configUSE_QUEUE_SETS == 1 )

    आधारः_Type_t x_Queue_Remove_From_समुच्चयः( Queue_समुच्चयः_Member_Handle_t xQueueOrSemaphore,
                                    Queue_समुच्चयः_Handle_t x_Queue_समुच्चयः )
    {
        आधारः_Type_t xReturn;
        Queue_t * ध्रुव pxQueueOrSemaphore = ( Queue_t * ) xQueueOrSemaphore;

        यदि( pxQueueOrSemaphore->px_Queue_समुच्चयः_Container != x_Queue_समुच्चयः )
        {
            /* The queue was not a member of the set. */
            xReturn = pdFAIL;
        }
        अन्यथा यदि( pxQueueOrSemaphore->ux_Messages_प्रतीक्षा != ( UBaseType_t ) 0 )
        {
            /* It is dangerous to remove a queue from a set when the queue is
             * not empty because the queue set will still hold pending events for
             * the queue. */
            xReturn = pdFAIL;
        }
        अन्यथा
        {
            taskENTER_CRITICAL( &( ( ( Queue_t * ) pxQueueOrSemaphore )->x_Queue_तालकम् ) );
            {
                /* The queue is no longer contained in the set. */
                pxQueueOrSemaphore->px_Queue_समुच्चयः_Container = रिक्त;
            }
            taskEXIT_CRITICAL( &( ( ( Queue_t * ) pxQueueOrSemaphore )->x_Queue_तालकम् ) );
            xReturn = pdPASS;
        }

        प्रत्यागम xReturn;
    } /*lint !e818 xQueueSet could not be declared as pointing to const as it is a typedef. */

#यदि_अन्तः /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#यदि ( configUSE_QUEUE_SETS == 1 )

    Queue_समुच्चयः_Member_Handle_t x_Queue_Select_From_समुच्चयः( Queue_समुच्चयः_Handle_t x_Queue_समुच्चयः,
                                                TickType_t ध्रुव x_Ticks_गन्तव्यम्_Wait )
    {
        Queue_समुच्चयः_Member_Handle_t xReturn = रिक्त;

        ( शून्य ) xQueueReceive( ( QueueHandle_t ) x_Queue_समुच्चयः, &xReturn, x_Ticks_गन्तव्यम्_Wait ); /*lint !e961 Casting from one typedef to another is not redundant. */
        प्रत्यागम xReturn;
    }

#यदि_अन्तः /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#यदि ( configUSE_QUEUE_SETS == 1 )

    Queue_समुच्चयः_Member_Handle_t x_Queue_Select_From_समुच्चयः_From_ISR( Queue_समुच्चयः_Handle_t x_Queue_समुच्चयः )
    {
        Queue_समुच्चयः_Member_Handle_t xReturn = रिक्त;

        ( शून्य ) xQueueReceiveFromISR( ( QueueHandle_t ) x_Queue_समुच्चयः, &xReturn, रिक्त ); /*lint !e961 Casting from one typedef to another is not redundant. */
        प्रत्यागम xReturn;
    }

#यदि_अन्तः /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#यदि ( configUSE_QUEUE_SETS == 1 )

    स्थिर आधारः_Type_t prv_Notify_Queue_समुच्चयः_Container( ध्रुव Queue_t * ध्रुव pxQueue )
    {
        Queue_t * px_Queue_समुच्चयः_Container = pxQueue->px_Queue_समुच्चयः_Container;
        आधारः_Type_t xReturn = pdFALSE;

        /* This function must be called form a critical section. */

        /* The following line is not reachable in unit tests because every call
         * to prvNotifyQueueSetContainer is preceded by a check that
         * pxQueueSetContainer != NULL */
        configASSERT( px_Queue_समुच्चयः_Container ); /* LCOV_EXCL_BR_LINE */
        configASSERT( px_Queue_समुच्चयः_Container->ux_Messages_प्रतीक्षा < px_Queue_समुच्चयः_Container->ux_दीर्घता );

        /* In SMP, queue sets have their own xQueueLock. Thus we need to also
         * acquire the queue set's xQueueLock before accessing it. */
        prvENTER_CRITICAL_SAFE_SMP_ONLY( &( px_Queue_समुच्चयः_Container->x_Queue_तालकम् ) );
        {
            यदि( px_Queue_समुच्चयः_Container->ux_Messages_प्रतीक्षा < px_Queue_समुच्चयः_Container->ux_दीर्घता )
            {
                #यदि ( queueUSE_LOCKS == 1 )
                    ध्रुव int8_t c_Tx_तालकम् = px_Queue_समुच्चयः_Container->c_Tx_तालकम्;
                #अन्यथा
                    /* Queue locks not used, so we treat it as unlocked. */
                    ध्रुव int8_t c_Tx_तालकम् = queueUNLOCKED;
                #यदि_अन्तः /* queueUSE_LOCKS == 1 */

                trace_QUEUE_समुच्चयः_SEND( px_Queue_समुच्चयः_Container );

                /* The data copied is the handle of the queue that contains data. */
                xReturn = prv_Copy_दत्त_गन्तव्यम्_Queue( px_Queue_समुच्चयः_Container, &pxQueue, queue_SEND_गन्तव्यम्_पृष्ठभागः );

                यदि( c_Tx_तालकम् == queueUNLOCKED )
                {
                    यदि( list_LIST_IS_रिक्तानि( &( px_Queue_समुच्चयः_Container->x_Tasks_प्रतीक्षा_गन्तव्यम्_Receive ) ) == pdFALSE )
                    {
                        यदि( xTaskRemoveFromEventList( &( px_Queue_समुच्चयः_Container->x_Tasks_प्रतीक्षा_गन्तव्यम्_Receive ) ) != pdFALSE )
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
                    #यदि ( queueUSE_LOCKS == 1 )
                    {
                        prv_Increment_Queue_Tx_तालकम्( px_Queue_समुच्चयः_Container, c_Tx_तालकम् );
                    }
                    #यदि_अन्तः /* queueUSE_LOCKS == 1 */
                }
            }
            अन्यथा
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        /* Release the previously acquired queue set's xQueueLock. */
        prvEXIT_CRITICAL_SAFE_SMP_ONLY( &( px_Queue_समुच्चयः_Container->x_Queue_तालकम् ) );

        प्रत्यागम xReturn;
    }

#यदि_अन्तः /* configUSE_QUEUE_SETS */
