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

#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#ਅਨ੍ਤਰ੍ਭਾਵ "FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "queue.h"
/* Include private IDF API additions for critical thread safety macros */
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_private/freertos_idf_additions_priv.h"

#ਜੇ ( configUSE_CO_ROUTINES == 1 )
    #ਅਨ੍ਤਰ੍ਭਾਵ "croutine.h"
#ਜੇ_ਅੰਤ

/* Lint e9021, e961 and e750 are suppressed as a MISRA exception justified
 * because the MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ MPU_WRAPPERS_INCLUDED_FROM_API_FILE /*lint !e961 !e750 !e9021. */

/* Single core FreeRTOS uses queue locks to ensure that vTaskPlaceOnEventList()
 * calls are deterministic (as queue locks use scheduler suspension instead of
 * critical sections). However, the SMP implementation is non-deterministic
 * anyways, thus SMP can forego the use of queue locks (replaced with a critical
 * sections) in exchange for better queue performance. */
#ਜੇ ( config_ਸੰਖਿਆ_ਦਾ_CORES > 1 )
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਕਤਾਰ_USE_LOCKS            0
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਕਤਾਰ_UNLOCKED             ( ( int8_t ) 0 )
#ਨਹੀਂਤਾਂ /* configNUMBER_OF_CORES > 1 */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਕਤਾਰ_USE_LOCKS            1
    /* Constants used with the cRxLock and cTxLock structure members. */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਕਤਾਰ_UNLOCKED             ( ( int8_t ) -1 )
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਕਤਾਰ_LOCKED_UNMODIFIED    ( ( int8_t ) 0 )
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਕਤਾਰ_INT8_MAX             ( ( int8_t ) 127 )
#ਜੇ_ਅੰਤ /* configNUMBER_OF_CORES > 1 */

/* When the Queue_t structure is used to represent a base queue its pcHead and
 * pcTail members are used as pointers into the queue storage area.  When the
 * Queue_t structure is used to represent a mutex pcHead and pcTail pointers are
 * not necessary, and the pcHead pointer is set to NULL to indicate that the
 * structure instead holds a pointer to the mutex holder (if any).  Map alternative
 * names to the pcHead and structure member to ensure the readability of the code
 * is maintained.  The QueuePointers_t and SemaphoreData_t types are used to form
 * a union as their usage is mutually exclusive dependent on what the queue is
 * being used for. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ux_ਕਤਾਰ_Type            pc_ਸਿਰ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਕਤਾਰ_ਕਤਾਰ_IS_MUTEX    ਨਿਰਾ

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ਕਤਾਰ_ਸੂਚਕ_ਸਾਰੇ
{
    int8_t * pc_ਪੂਛ;     /*< Points to the byte at the end of the queue storage area.  Once more byte is allocated than necessary to store the queue items, this is used as a marker. */
    int8_t * pcReadFrom; /*< Points to the last place that a queued item was read from when the structure is used as a queue. */
} ਕਤਾਰ_ਸੂਚਕ_ਸਾਰੇ_t;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ Semaphore_ਅੰਕੜੇ
{
    TaskHandle_t xMutexHolder;        /*< The handle of the task that holds the mutex. */
    UBaseType_t ux_Recursive_Call_ਗਿਣਤੀ; /*< Maintains a count of the number of times a recursive mutex has been recursively 'taken' when the structure is used as a mutex. */
} Semaphore_ਅੰਕੜੇ_t;

/* Semaphores do not actually store or copy data, so have an item size of
 * zero. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਕਤਾਰ_SEMAPHORE_ਕਤਾਰ_ਵਸਤੂ_ਲੰਮਾਈ    ( ( UBaseType_t ) 0 )
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਕਤਾਰ_MUTEX_GIVE_ਖੰਡ_ਸਮਾਂ          ( ( TickType_t ) 0U )

#ਜੇ ( configUSE_PREEMPTION == 0 )

/* If the cooperative scheduler is being used then a yield should not be
 * performed just because a higher priority task has been woken. */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਕਤਾਰ_YIELD_IF_USING_PREEMPTION()
#ਨਹੀਂਤਾਂ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਕਤਾਰ_YIELD_IF_USING_PREEMPTION()    ਪੋਰਟ_YIELD_WITHIN_API()
#ਜੇ_ਅੰਤ

/*
 * Definition of the queue used by the scheduler.
 * Items are queued by copy, not reference.  See the following link for the
 * rationale: https://www.FreeRTOS.org/Embedded-RTOS-Queues.html
 */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ਕਤਾਰ_Definition /* The old naming convention is used to prevent breaking kernel aware debuggers. */
{
    int8_t * pc_ਸਿਰ;           /*< Points to the beginning of the queue storage area. */
    int8_t * pc_Write_ਮੰਜ਼ਿਲ;        /*< Points to the free next place in the storage area. */

    ਸੰਘ
    {
        ਕਤਾਰ_ਸੂਚਕ_ਸਾਰੇ_t x_ਕਤਾਰ;     /*< Data required exclusively when this structure is used as a queue. */
        Semaphore_ਅੰਕੜੇ_t xSemaphore; /*< Data required exclusively when this structure is used as a semaphore. */
    } u;

    List_t x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Send;             /*< List of tasks that are blocked waiting to post onto this queue.  Stored in priority order. */
    List_t x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive;          /*< List of tasks that are blocked waiting to read from this queue.  Stored in priority order. */

    ਅਸਥਿਰ UBaseType_t ux_Messages_ਉਡੀਕ; /*< The number of items currently in the queue. */
    UBaseType_t ux_ਲੰਮਾਈ;                   /*< The length of the queue defined as the number of items it will hold, not the number of bytes. */
    UBaseType_t ux_ਵਸਤੂ_ਮਾਪ;                 /*< The size of each items that the queue will hold. */

    #ਜੇ ( ਕਤਾਰ_USE_LOCKS == 1 )
        ਅਸਥਿਰ int8_t c_Rx_ਤਾਲਾ; /*< Stores the number of items received from the queue (removed from the queue) while the queue was locked.  Set to queueUNLOCKED when the queue is not locked. */
        ਅਸਥਿਰ int8_t c_Tx_ਤਾਲਾ; /*< Stores the number of items transmitted to the queue (added to the queue) while the queue was locked.  Set to queueUNLOCKED when the queue is not locked. */
    #ਜੇ_ਅੰਤ /* queueUSE_LOCKS == 1 */

    #ਜੇ ( ( config_SUPPORT_STATIC_ਵੰਡ == 1 ) && ( config_SUPPORT_DYNAMIC_ਵੰਡ == 1 ) )
        uint8_t ucStaticallyAllocated; /*< Set to pdTRUE if the memory used by the queue was statically allocated to ensure no attempt is made to free the memory. */
    #ਜੇ_ਅੰਤ

    #ਜੇ ( config_USE_ਕਤਾਰ_SETS == 1 )
        ਬਣਤਰ ਕਤਾਰ_Definition * px_ਕਤਾਰ_ਸੈੱਟ_Container;
    #ਜੇ_ਅੰਤ

    #ਜੇ ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_ਕਤਾਰ_ਸੰਖਿਆ;
        uint8_t uc_ਕਤਾਰ_Type;
    #ਜੇ_ਅੰਤ

    ਪੋਰਟ_MUX_TYPE x_ਕਤਾਰ_ਤਾਲਾ; /* Spinlock required for SMP critical sections */
} x_ਕਤਾਰ_2;

/* The old xQUEUE name is maintained above then typedefed to the new Queue_t
 * name below to enable the use of older kernel aware debuggers. */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ x_ਕਤਾਰ_2 ਕਤਾਰ_t;

/*-----------------------------------------------------------*/

/*
 * The queue registry is just a means for kernel aware debuggers to locate
 * queue structures.  It has no other purpose so is an optional component.
 */
#ਜੇ ( config_ਕਤਾਰ_REGISTRY_ਮਾਪ > 0 )

/* The type stored within the queue registry array.  This allows a name
 * to be assigned to each queue making kernel aware debugging a little
 * more user friendly. */
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ਕਤਾਰ_REGISTRY_ਵਸਤੂ
    {
        ਅਚਲ ਅੱਖਰ * pc_ਕਤਾਰ_Name; /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
        ਕਤਾਰ_Handle_t xHandle;
    } x_ਕਤਾਰ_Registry_ਵਸਤੂ;

/* The old xQueueRegistryItem name is maintained above then typedefed to the
 * new xQueueRegistryItem name below to enable the use of older kernel aware
 * debuggers. */
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ x_ਕਤਾਰ_Registry_ਵਸਤੂ ਕਤਾਰ_Registry_ਵਸਤੂ_t;

/* The queue registry is simply an array of QueueRegistryItem_t structures.
 * The pcQueueName member of a structure being NULL is indicative of the
 * array position being vacant. */
    PRIVILEGED_ਅੰਕੜੇ ਕਤਾਰ_Registry_ਵਸਤੂ_t x_ਕਤਾਰ_Registry[ config_ਕਤਾਰ_REGISTRY_ਮਾਪ ];

    #ਜੇ ( config_ਸੰਖਿਆ_ਦਾ_CORES > 1 )
/* Spinlock required in SMP when accessing the queue registry */
        ਸਥਿਰ ਪੋਰਟ_MUX_TYPE x_ਕਤਾਰ_Registry_ਤਾਲਾ = ਪੋਰਟ_MUX_INITIALIZER_UNLOCKED;
    #ਜੇ_ਅੰਤ /* configNUMBER_OF_CORES > 1 */

#ਜੇ_ਅੰਤ /* configQUEUE_REGISTRY_SIZE */

#ਜੇ ( ਕਤਾਰ_USE_LOCKS == 1 )

/*
 * Unlocks a queue locked by a call to prvLockQueue.  Locking a queue does not
 * prevent an ISR from adding or removing items to the queue, but does prevent
 * an ISR from removing tasks from the queue event lists.  If an ISR finds a
 * queue is locked it will instead increment the appropriate queue lock count
 * to indicate that a task may require unblocking.  When the queue in unlocked
 * these lock counts are inspected, and the appropriate action taken.
 */
    ਸਥਿਰ ਖਾਲੀ prv_Unlock_ਕਤਾਰ( ਕਤਾਰ_t * ਅਚਲ px_ਕਤਾਰ ) PRIVILEGED_FUNCTION;

/*
 * Uses a critical section to determine if there is any data in a queue.
 *
 * @return pdTRUE if the queue contains no items, otherwise pdFALSE.
 */
    ਸਥਿਰ ਅਧਾਰ_Type_t prv_Is_ਕਤਾਰ_ਖਾਲੀ_ਥਾਵਾਂ( ਅਚਲ ਕਤਾਰ_t * px_ਕਤਾਰ ) PRIVILEGED_FUNCTION;

/*
 * Uses a critical section to determine if there is any space in a queue.
 *
 * @return pdTRUE if there is no space, otherwise pdFALSE;
 */
    ਸਥਿਰ ਅਧਾਰ_Type_t prv_Is_ਕਤਾਰ_ਭਰਿਆ( ਅਚਲ ਕਤਾਰ_t * px_ਕਤਾਰ ) PRIVILEGED_FUNCTION;
#ਜੇ_ਅੰਤ /* queueUSE_LOCKS == 1 */

/*
 * Copies an item into the queue, either at the front of the queue or the
 * back of the queue.
 */
ਸਥਿਰ ਅਧਾਰ_Type_t prv_Copy_ਅੰਕੜੇ_ਮੰਜ਼ਿਲ_ਕਤਾਰ( ਕਤਾਰ_t * ਅਚਲ px_ਕਤਾਰ,
                                      ਅਚਲ ਖਾਲੀ * pv_ਵਸਤੂ_ਮੰਜ਼ਿਲ_ਕਤਾਰ,
                                      ਅਚਲ ਅਧਾਰ_Type_t xPosition ) PRIVILEGED_FUNCTION;

/*
 * Copies an item out of a queue.
 */
ਸਥਿਰ ਖਾਲੀ prv_Copy_ਅੰਕੜੇ_From_ਕਤਾਰ( ਕਤਾਰ_t * ਅਚਲ px_ਕਤਾਰ,
                                  ਖਾਲੀ * ਅਚਲ pv_ਬਫਰ ) PRIVILEGED_FUNCTION;

#ਜੇ ( config_USE_ਕਤਾਰ_SETS == 1 )

/*
 * Checks to see if a queue is a member of a queue set, and if so, notifies
 * the queue set that the queue contains data.
 */
    ਸਥਿਰ ਅਧਾਰ_Type_t prv_Notify_ਕਤਾਰ_ਸੈੱਟ_Container( ਅਚਲ ਕਤਾਰ_t * ਅਚਲ px_ਕਤਾਰ ) PRIVILEGED_FUNCTION;
#ਜੇ_ਅੰਤ

/*
 * Called after a Queue_t structure has been allocated either statically or
 * dynamically to fill in the structure's members.
 */
ਸਥਿਰ ਖਾਲੀ prv_Initialise_New_ਕਤਾਰ( ਅਚਲ UBaseType_t ux_ਕਤਾਰ_ਲੰਮਾਈ,
                                   ਅਚਲ UBaseType_t ux_ਵਸਤੂ_ਮਾਪ,
                                   uint8_t * puc_ਕਤਾਰ_Storage,
                                   ਅਚਲ uint8_t uc_ਕਤਾਰ_Type,
                                   ਕਤਾਰ_t * px_New_ਕਤਾਰ ) PRIVILEGED_FUNCTION;

/*
 * Mutexes are a special type of queue.  When a mutex is created, first the
 * queue is created, then prvInitialiseMutex() is called to configure the queue
 * as a mutex.
 */
#ਜੇ ( configUSE_MUTEXES == 1 )
    ਸਥਿਰ ਖਾਲੀ prvInitialiseMutex( ਕਤਾਰ_t * px_New_ਕਤਾਰ ) PRIVILEGED_FUNCTION;
#ਜੇ_ਅੰਤ

#ਜੇ ( configUSE_MUTEXES == 1 )

/*
 * If a task waiting for a mutex causes the mutex holder to inherit a
 * priority, but the waiting task times out, then the holder should
 * disinherit the priority - but only down to the highest priority of any
 * other tasks that are waiting for the same mutex.  This function returns
 * that priority.
 */
    ਸਥਿਰ UBaseType_t prv_Get_Disinherit_ਤਰਜੀਹ_After_Timeout( ਅਚਲ ਕਤਾਰ_t * ਅਚਲ px_ਕਤਾਰ ) PRIVILEGED_FUNCTION;
#ਜੇ_ਅੰਤ
/*-----------------------------------------------------------*/

#ਜੇ ( ਕਤਾਰ_USE_LOCKS == 1 )

/*
 * Macro to mark a queue as locked.  Locking a queue prevents an ISR from
 * accessing the queue event lists.
 */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ prv_ਤਾਲਾ_ਕਤਾਰ( px_ਕਤਾਰ )                        \
    taskENTER_CRITICAL( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ) );        \
    {                                                      \
        ਜੇ( ( px_ਕਤਾਰ )->c_Rx_ਤਾਲਾ == ਕਤਾਰ_UNLOCKED )        \
        {                                                  \
            ( px_ਕਤਾਰ )->c_Rx_ਤਾਲਾ = ਕਤਾਰ_LOCKED_UNMODIFIED; \
        }                                                  \
        ਜੇ( ( px_ਕਤਾਰ )->c_Tx_ਤਾਲਾ == ਕਤਾਰ_UNLOCKED )        \
        {                                                  \
            ( px_ਕਤਾਰ )->c_Tx_ਤਾਲਾ = ਕਤਾਰ_LOCKED_UNMODIFIED; \
        }                                                  \
    }                                                      \
    taskEXIT_CRITICAL( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ) )

/*
 * Macro to increment cTxLock member of the queue data structure. It is
 * capped at the number of tasks in the system as we cannot unblock more
 * tasks than the number of tasks in the system.
 */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ prv_Increment_ਕਤਾਰ_Tx_ਤਾਲਾ( px_ਕਤਾਰ, c_Tx_ਤਾਲਾ )                       \
    {                                                                         \
        ਅਚਲ UBaseType_t ux_ਸੰਖਿਆ_ਦਾ_Tasks = ux_Task_Get_ਸੰਖਿਆ_ਦਾ_Tasks();         \
        ਜੇ( ( UBaseType_t ) ( c_Tx_ਤਾਲਾ ) < ux_ਸੰਖਿਆ_ਦਾ_Tasks )                   \
        {                                                                     \
            configASSERT( ( c_Tx_ਤਾਲਾ ) != ਕਤਾਰ_INT8_MAX );                     \
            ( px_ਕਤਾਰ )->c_Tx_ਤਾਲਾ = ( int8_t ) ( ( c_Tx_ਤਾਲਾ ) + ( int8_t ) 1 ); \
        }                                                                     \
    }

/*
 * Macro to increment cRxLock member of the queue data structure. It is
 * capped at the number of tasks in the system as we cannot unblock more
 * tasks than the number of tasks in the system.
 */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ prv_Increment_ਕਤਾਰ_Rx_ਤਾਲਾ( px_ਕਤਾਰ, c_Rx_ਤਾਲਾ )                       \
    {                                                                         \
        ਅਚਲ UBaseType_t ux_ਸੰਖਿਆ_ਦਾ_Tasks = ux_Task_Get_ਸੰਖਿਆ_ਦਾ_Tasks();         \
        ਜੇ( ( UBaseType_t ) ( c_Rx_ਤਾਲਾ ) < ux_ਸੰਖਿਆ_ਦਾ_Tasks )                   \
        {                                                                     \
            configASSERT( ( c_Rx_ਤਾਲਾ ) != ਕਤਾਰ_INT8_MAX );                     \
            ( px_ਕਤਾਰ )->c_Rx_ਤਾਲਾ = ( int8_t ) ( ( c_Rx_ਤਾਲਾ ) + ( int8_t ) 1 ); \
        }                                                                     \
    }
#ਜੇ_ਅੰਤ /* queueUSE_LOCKS == 1 */
/*-----------------------------------------------------------*/

ਅਧਾਰ_Type_t x_ਕਤਾਰ_Generic_Reset( ਕਤਾਰ_Handle_t x_ਕਤਾਰ,
                               ਅਧਾਰ_Type_t x_New_ਕਤਾਰ )
{
    ਅਧਾਰ_Type_t xReturn = pdPASS;
    ਕਤਾਰ_t * ਅਚਲ px_ਕਤਾਰ = x_ਕਤਾਰ;

    configASSERT( px_ਕਤਾਰ );

    ਜੇ( x_New_ਕਤਾਰ == pdTRUE )
    {
        ਪੋਰਟ_MUX_INITIALIZE( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ) );
    }

    ਜੇ( ( px_ਕਤਾਰ != ਨਿਰਾ ) &&
        ( px_ਕਤਾਰ->ux_ਲੰਮਾਈ >= 1U ) &&
        /* Check for multiplication overflow. */
        ( ( SIZE_MAX / px_ਕਤਾਰ->ux_ਲੰਮਾਈ ) >= px_ਕਤਾਰ->ux_ਵਸਤੂ_ਮਾਪ ) )
    {
        taskENTER_CRITICAL( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ) );
        {
            px_ਕਤਾਰ->u.x_ਕਤਾਰ.pc_ਪੂਛ = px_ਕਤਾਰ->pc_ਸਿਰ + ( px_ਕਤਾਰ->ux_ਲੰਮਾਈ * px_ਕਤਾਰ->ux_ਵਸਤੂ_ਮਾਪ ); /*lint !e9016 Pointer arithmetic allowed on char types, especially when it assists conveying intent. */
            px_ਕਤਾਰ->ux_Messages_ਉਡੀਕ = ( UBaseType_t ) 0U;
            px_ਕਤਾਰ->pc_Write_ਮੰਜ਼ਿਲ = px_ਕਤਾਰ->pc_ਸਿਰ;
            px_ਕਤਾਰ->u.x_ਕਤਾਰ.pcReadFrom = px_ਕਤਾਰ->pc_ਸਿਰ + ( ( px_ਕਤਾਰ->ux_ਲੰਮਾਈ - 1U ) * px_ਕਤਾਰ->ux_ਵਸਤੂ_ਮਾਪ ); /*lint !e9016 Pointer arithmetic allowed on char types, especially when it assists conveying intent. */
            #ਜੇ ( ਕਤਾਰ_USE_LOCKS == 1 )
            {
                px_ਕਤਾਰ->c_Rx_ਤਾਲਾ = ਕਤਾਰ_UNLOCKED;
                px_ਕਤਾਰ->c_Tx_ਤਾਲਾ = ਕਤਾਰ_UNLOCKED;
            }
            #ਜੇ_ਅੰਤ /* queueUSE_LOCKS == 1 */

            ਜੇ( x_New_ਕਤਾਰ == pdFALSE )
            {
                /* If there are tasks blocked waiting to read from the queue, then
                 * the tasks will remain blocked as after this function exits the queue
                 * will still be empty.  If there are tasks blocked waiting to write to
                 * the queue, then one should be unblocked as after this function exits
                 * it will be possible to write to it. */
                ਜੇ( list_LIST_IS_ਖਾਲੀ_ਥਾਵਾਂ( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Send ) ) == pdFALSE )
                {
                    ਜੇ( xTaskRemoveFromEventList( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Send ) ) != pdFALSE )
                    {
                        ਕਤਾਰ_YIELD_IF_USING_PREEMPTION();
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
            }
            ਨਹੀਂਤਾਂ
            {
                /* Ensure the event queues start in the correct state. */
                vListInitialise( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Send ) );
                vListInitialise( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive ) );
            }
        }
        taskEXIT_CRITICAL( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ) );
    }
    ਨਹੀਂਤਾਂ
    {
        xReturn = pdFAIL;
    }

    configASSERT( xReturn != pdFAIL );

    /* A value is returned for calling semantic consistency with previous
     * versions. */
    ਵਾਪਸ xReturn;
}
/*-----------------------------------------------------------*/

#ਜੇ ( config_SUPPORT_STATIC_ਵੰਡ == 1 )

    ਕਤਾਰ_Handle_t x_ਕਤਾਰ_Generic_Create_Static( ਅਚਲ UBaseType_t ux_ਕਤਾਰ_ਲੰਮਾਈ,
                                             ਅਚਲ UBaseType_t ux_ਵਸਤੂ_ਮਾਪ,
                                             uint8_t * puc_ਕਤਾਰ_Storage,
                                             Static_ਕਤਾਰ_t * px_Static_ਕਤਾਰ,
                                             ਅਚਲ uint8_t uc_ਕਤਾਰ_Type )
    {
        ਕਤਾਰ_t * px_New_ਕਤਾਰ = ਨਿਰਾ;

        /* The StaticQueue_t structure and the queue storage area must be
         * supplied. */
        configASSERT( px_Static_ਕਤਾਰ );

        ਜੇ( ( ux_ਕਤਾਰ_ਲੰਮਾਈ > ( UBaseType_t ) 0 ) &&
            ( px_Static_ਕਤਾਰ != ਨਿਰਾ ) &&

            /* A queue storage area should be provided if the item size is not 0, and
             * should not be provided if the item size is 0. */
            ( !( ( puc_ਕਤਾਰ_Storage != ਨਿਰਾ ) && ( ux_ਵਸਤੂ_ਮਾਪ == 0 ) ) ) &&
            ( !( ( puc_ਕਤਾਰ_Storage == ਨਿਰਾ ) && ( ux_ਵਸਤੂ_ਮਾਪ != 0 ) ) ) )
        {
            #ਜੇ ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticQueue_t or StaticSemaphore_t equals the size of
                 * the real queue and semaphore structures. */
                ਅਸਥਿਰ size_t x_ਮਾਪ = ਆਕਾਰ( Static_ਕਤਾਰ_t );

                /* This assertion cannot be branch covered in unit tests */
                configASSERT( x_ਮਾਪ == ਆਕਾਰ( ਕਤਾਰ_t ) ); /* LCOV_EXCL_BR_LINE */
                ( ਖਾਲੀ ) x_ਮਾਪ;                             /* Keeps lint quiet when configASSERT() is not defined. */
            }
            #ਜੇ_ਅੰਤ /* configASSERT_DEFINED */

            /* The address of a statically allocated queue was passed in, use it.
             * The address of a statically allocated storage area was also passed in
             * but is already set. */
            px_New_ਕਤਾਰ = ( ਕਤਾਰ_t * ) px_Static_ਕਤਾਰ; /*lint !e740 !e9087 Unusual cast is ok as the structures are designed to have the same alignment, and the size is checked by an assert. */

            #ਜੇ ( config_SUPPORT_DYNAMIC_ਵੰਡ == 1 )
            {
                /* Queues can be allocated wither statically or dynamically, so
                 * note this queue was allocated statically in case the queue is
                 * later deleted. */
                px_New_ਕਤਾਰ->ucStaticallyAllocated = pdTRUE;
            }
            #ਜੇ_ਅੰਤ /* configSUPPORT_DYNAMIC_ALLOCATION */

            prv_Initialise_New_ਕਤਾਰ( ux_ਕਤਾਰ_ਲੰਮਾਈ, ux_ਵਸਤੂ_ਮਾਪ, puc_ਕਤਾਰ_Storage, uc_ਕਤਾਰ_Type, px_New_ਕਤਾਰ );
        }
        ਨਹੀਂਤਾਂ
        {
            configASSERT( px_New_ਕਤਾਰ );
            mtCOVERAGE_TEST_MARKER();
        }

        ਵਾਪਸ px_New_ਕਤਾਰ;
    }

#ਜੇ_ਅੰਤ /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

#ਜੇ ( config_SUPPORT_STATIC_ਵੰਡ == 1 )

    ਅਧਾਰ_Type_t x_ਕਤਾਰ_Generic_Get_Static_Buffers( ਕਤਾਰ_Handle_t x_ਕਤਾਰ,
                                              uint8_t ** ppuc_ਕਤਾਰ_Storage,
                                              Static_ਕਤਾਰ_t ** ppx_Static_ਕਤਾਰ )
    {
        ਅਧਾਰ_Type_t xReturn;
        ਕਤਾਰ_t * ਅਚਲ px_ਕਤਾਰ = x_ਕਤਾਰ;

        configASSERT( px_ਕਤਾਰ );
        configASSERT( ppx_Static_ਕਤਾਰ );

        #ਜੇ ( config_SUPPORT_DYNAMIC_ਵੰਡ == 1 )
        {
            /* Check if the queue was statically allocated. */
            ਜੇ( px_ਕਤਾਰ->ucStaticallyAllocated == ( uint8_t ) pdTRUE )
            {
                ਜੇ( ppuc_ਕਤਾਰ_Storage != ਨਿਰਾ )
                {
                    *ppuc_ਕਤਾਰ_Storage = ( uint8_t * ) px_ਕਤਾਰ->pc_ਸਿਰ;
                }

                *ppx_Static_ਕਤਾਰ = ( Static_ਕਤਾਰ_t * ) px_ਕਤਾਰ;
                xReturn = pdTRUE;
            }
            ਨਹੀਂਤਾਂ
            {
                xReturn = pdFALSE;
            }
        }
        #ਨਹੀਂਤਾਂ /* configSUPPORT_DYNAMIC_ALLOCATION */
        {
            /* Queue must have been statically allocated. */
            ਜੇ( ppuc_ਕਤਾਰ_Storage != ਨਿਰਾ )
            {
                *ppuc_ਕਤਾਰ_Storage = ( uint8_t * ) px_ਕਤਾਰ->pc_ਸਿਰ;
            }

            *ppx_Static_ਕਤਾਰ = ( Static_ਕਤਾਰ_t * ) px_ਕਤਾਰ;
            xReturn = pdTRUE;
        }
        #ਜੇ_ਅੰਤ /* configSUPPORT_DYNAMIC_ALLOCATION */

        ਵਾਪਸ xReturn;
    }

#ਜੇ_ਅੰਤ /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

#ਜੇ ( config_SUPPORT_DYNAMIC_ਵੰਡ == 1 )

    ਕਤਾਰ_Handle_t x_ਕਤਾਰ_Generic_Create( ਅਚਲ UBaseType_t ux_ਕਤਾਰ_ਲੰਮਾਈ,
                                       ਅਚਲ UBaseType_t ux_ਵਸਤੂ_ਮਾਪ,
                                       ਅਚਲ uint8_t uc_ਕਤਾਰ_Type )
    {
        ਕਤਾਰ_t * px_New_ਕਤਾਰ = ਨਿਰਾ;
        size_t x_ਕਤਾਰ_ਮਾਪ_ਵਿੱਚ_ਬਾਈਟਾਂ;
        uint8_t * puc_ਕਤਾਰ_Storage;

        ਜੇ( ( ux_ਕਤਾਰ_ਲੰਮਾਈ > ( UBaseType_t ) 0 ) &&
            /* Check for multiplication overflow. */
            ( ( SIZE_MAX / ux_ਕਤਾਰ_ਲੰਮਾਈ ) >= ux_ਵਸਤੂ_ਮਾਪ ) &&
            /* Check for addition overflow. */
            ( ( SIZE_MAX - ਆਕਾਰ( ਕਤਾਰ_t ) ) >= ( ux_ਕਤਾਰ_ਲੰਮਾਈ * ux_ਵਸਤੂ_ਮਾਪ ) ) )
        {
            /* Allocate enough space to hold the maximum number of items that
             * can be in the queue at any time.  It is valid for uxItemSize to be
             * zero in the case the queue is used as a semaphore. */
            x_ਕਤਾਰ_ਮਾਪ_ਵਿੱਚ_ਬਾਈਟਾਂ = ( size_t ) ( ux_ਕਤਾਰ_ਲੰਮਾਈ * ux_ਵਸਤੂ_ਮਾਪ ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */

            /* Allocate the queue and storage area.  Justification for MISRA
             * deviation as follows:  pvPortMalloc() always ensures returned memory
             * blocks are aligned per the requirements of the MCU stack.  In this case
             * pvPortMalloc() must return a pointer that is guaranteed to meet the
             * alignment requirements of the Queue_t structure - which in this case
             * is an int8_t *.  Therefore, whenever the stack alignment requirements
             * are greater than or equal to the pointer to char requirements the cast
             * is safe.  In other cases alignment requirements are not strict (one or
             * two bytes). */
            px_New_ਕਤਾਰ = ( ਕਤਾਰ_t * ) pv_ਪੋਰਟ_Malloc( ਆਕਾਰ( ਕਤਾਰ_t ) + x_ਕਤਾਰ_ਮਾਪ_ਵਿੱਚ_ਬਾਈਟਾਂ ); /*lint !e9087 !e9079 see comment above. */

            ਜੇ( px_New_ਕਤਾਰ != ਨਿਰਾ )
            {
                /* Jump past the queue structure to find the location of the queue
                 * storage area. */
                puc_ਕਤਾਰ_Storage = ( uint8_t * ) px_New_ਕਤਾਰ;
                puc_ਕਤਾਰ_Storage += ਆਕਾਰ( ਕਤਾਰ_t ); /*lint !e9016 Pointer arithmetic allowed on char types, especially when it assists conveying intent. */

                #ਜੇ ( config_SUPPORT_STATIC_ਵੰਡ == 1 )
                {
                    /* Queues can be created either statically or dynamically, so
                     * note this task was created dynamically in case it is later
                     * deleted. */
                    px_New_ਕਤਾਰ->ucStaticallyAllocated = pdFALSE;
                }
                #ਜੇ_ਅੰਤ /* configSUPPORT_STATIC_ALLOCATION */

                prv_Initialise_New_ਕਤਾਰ( ux_ਕਤਾਰ_ਲੰਮਾਈ, ux_ਵਸਤੂ_ਮਾਪ, puc_ਕਤਾਰ_Storage, uc_ਕਤਾਰ_Type, px_New_ਕਤਾਰ );
            }
            ਨਹੀਂਤਾਂ
            {
                trace_ਕਤਾਰ_CREATE_FAILED( uc_ਕਤਾਰ_Type );
                mtCOVERAGE_TEST_MARKER();
            }
        }
        ਨਹੀਂਤਾਂ
        {
            configASSERT( px_New_ਕਤਾਰ );
            mtCOVERAGE_TEST_MARKER();
        }

        ਵਾਪਸ px_New_ਕਤਾਰ;
    }

#ਜੇ_ਅੰਤ /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

ਸਥਿਰ ਖਾਲੀ prv_Initialise_New_ਕਤਾਰ( ਅਚਲ UBaseType_t ux_ਕਤਾਰ_ਲੰਮਾਈ,
                                   ਅਚਲ UBaseType_t ux_ਵਸਤੂ_ਮਾਪ,
                                   uint8_t * puc_ਕਤਾਰ_Storage,
                                   ਅਚਲ uint8_t uc_ਕਤਾਰ_Type,
                                   ਕਤਾਰ_t * px_New_ਕਤਾਰ )
{
    /* Remove compiler warnings about unused parameters should
     * configUSE_TRACE_FACILITY not be set to 1. */
    ( ਖਾਲੀ ) uc_ਕਤਾਰ_Type;

    ਜੇ( ux_ਵਸਤੂ_ਮਾਪ == ( UBaseType_t ) 0 )
    {
        /* No RAM was allocated for the queue storage area, but PC head cannot
         * be set to NULL because NULL is used as a key to say the queue is used as
         * a mutex.  Therefore just set pcHead to point to the queue as a benign
         * value that is known to be within the memory map. */
        px_New_ਕਤਾਰ->pc_ਸਿਰ = ( int8_t * ) px_New_ਕਤਾਰ;
    }
    ਨਹੀਂਤਾਂ
    {
        /* Set the head to the start of the queue storage area. */
        px_New_ਕਤਾਰ->pc_ਸਿਰ = ( int8_t * ) puc_ਕਤਾਰ_Storage;
    }

    /* Initialise the queue members as described where the queue type is
     * defined. */
    px_New_ਕਤਾਰ->ux_ਲੰਮਾਈ = ux_ਕਤਾਰ_ਲੰਮਾਈ;
    px_New_ਕਤਾਰ->ux_ਵਸਤੂ_ਮਾਪ = ux_ਵਸਤੂ_ਮਾਪ;
    ( ਖਾਲੀ ) x_ਕਤਾਰ_Generic_Reset( px_New_ਕਤਾਰ, pdTRUE );

    #ਜੇ ( configUSE_TRACE_FACILITY == 1 )
    {
        px_New_ਕਤਾਰ->uc_ਕਤਾਰ_Type = uc_ਕਤਾਰ_Type;
    }
    #ਜੇ_ਅੰਤ /* configUSE_TRACE_FACILITY */

    #ਜੇ ( config_USE_ਕਤਾਰ_SETS == 1 )
    {
        px_New_ਕਤਾਰ->px_ਕਤਾਰ_ਸੈੱਟ_Container = ਨਿਰਾ;
    }
    #ਜੇ_ਅੰਤ /* configUSE_QUEUE_SETS */

    trace_ਕਤਾਰ_CREATE( px_New_ਕਤਾਰ );
}
/*-----------------------------------------------------------*/

#ਜੇ ( configUSE_MUTEXES == 1 )

    ਸਥਿਰ ਖਾਲੀ prvInitialiseMutex( ਕਤਾਰ_t * px_New_ਕਤਾਰ )
    {
        ਜੇ( px_New_ਕਤਾਰ != ਨਿਰਾ )
        {
            /* The queue create function will set all the queue structure members
            * correctly for a generic queue, but this function is creating a
            * mutex.  Overwrite those members that need to be set differently -
            * in particular the information required for priority inheritance. */
            px_New_ਕਤਾਰ->u.xSemaphore.xMutexHolder = ਨਿਰਾ;
            px_New_ਕਤਾਰ->ux_ਕਤਾਰ_Type = ਕਤਾਰ_ਕਤਾਰ_IS_MUTEX;

            /* In case this is a recursive mutex. */
            px_New_ਕਤਾਰ->u.xSemaphore.ux_Recursive_Call_ਗਿਣਤੀ = 0;

            /* Initialize the mutex's spinlock */
            ਪੋਰਟ_MUX_INITIALIZE( &( px_New_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ) );

            traceCREATE_MUTEX( px_New_ਕਤਾਰ );

            /* Start with the semaphore in the expected state. */
            ( ਖਾਲੀ ) x_ਕਤਾਰ_Generic_Send( px_New_ਕਤਾਰ, ਨਿਰਾ, ( TickType_t ) 0U, ਕਤਾਰ_SEND_ਮੰਜ਼ਿਲ_ਪਿੱਠ );
        }
        ਨਹੀਂਤਾਂ
        {
            traceCREATE_MUTEX_FAILED();
        }
    }

#ਜੇ_ਅੰਤ /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

#ਜੇ ( ( configUSE_MUTEXES == 1 ) && ( config_SUPPORT_DYNAMIC_ਵੰਡ == 1 ) )

    ਕਤਾਰ_Handle_t x_ਕਤਾਰ_Create_Mutex( ਅਚਲ uint8_t uc_ਕਤਾਰ_Type )
    {
        ਕਤਾਰ_Handle_t x_New_ਕਤਾਰ;
        ਅਚਲ UBaseType_t ux_Mutex_ਲੰਮਾਈ = ( UBaseType_t ) 1, ux_Mutex_ਮਾਪ = ( UBaseType_t ) 0;

        x_New_ਕਤਾਰ = x_ਕਤਾਰ_Generic_Create( ux_Mutex_ਲੰਮਾਈ, ux_Mutex_ਮਾਪ, uc_ਕਤਾਰ_Type );
        prvInitialiseMutex( ( ਕਤਾਰ_t * ) x_New_ਕਤਾਰ );

        ਵਾਪਸ x_New_ਕਤਾਰ;
    }

#ਜੇ_ਅੰਤ /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

#ਜੇ ( ( configUSE_MUTEXES == 1 ) && ( config_SUPPORT_STATIC_ਵੰਡ == 1 ) )

    ਕਤਾਰ_Handle_t x_ਕਤਾਰ_Create_Mutex_Static( ਅਚਲ uint8_t uc_ਕਤਾਰ_Type,
                                           Static_ਕਤਾਰ_t * px_Static_ਕਤਾਰ )
    {
        ਕਤਾਰ_Handle_t x_New_ਕਤਾਰ;
        ਅਚਲ UBaseType_t ux_Mutex_ਲੰਮਾਈ = ( UBaseType_t ) 1, ux_Mutex_ਮਾਪ = ( UBaseType_t ) 0;

        /* Prevent compiler warnings about unused parameters if
         * configUSE_TRACE_FACILITY does not equal 1. */
        ( ਖਾਲੀ ) uc_ਕਤਾਰ_Type;

        x_New_ਕਤਾਰ = x_ਕਤਾਰ_Generic_Create_Static( ux_Mutex_ਲੰਮਾਈ, ux_Mutex_ਮਾਪ, ਨਿਰਾ, px_Static_ਕਤਾਰ, uc_ਕਤਾਰ_Type );
        prvInitialiseMutex( ( ਕਤਾਰ_t * ) x_New_ਕਤਾਰ );

        ਵਾਪਸ x_New_ਕਤਾਰ;
    }

#ਜੇ_ਅੰਤ /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

#ਜੇ ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) )

    TaskHandle_t x_ਕਤਾਰ_Get_Mutex_Holder( ਕਤਾਰ_Handle_t xSemaphore )
    {
        TaskHandle_t pxReturn;
        ਕਤਾਰ_t * ਅਚਲ pxSemaphore = ( ਕਤਾਰ_t * ) xSemaphore;

        configASSERT( xSemaphore );

        /* This function is called by xSemaphoreGetMutexHolder(), and should not
         * be called directly.  Note:  This is a good way of determining if the
         * calling task is the mutex holder, but not a good way of determining the
         * identity of the mutex holder, as the holder may change between the
         * following critical section exiting and the function returning. */
        taskENTER_CRITICAL( &( pxSemaphore->x_ਕਤਾਰ_ਤਾਲਾ ) );
        {
            ਜੇ( pxSemaphore->ux_ਕਤਾਰ_Type == ਕਤਾਰ_ਕਤਾਰ_IS_MUTEX )
            {
                pxReturn = pxSemaphore->u.xSemaphore.xMutexHolder;
            }
            ਨਹੀਂਤਾਂ
            {
                pxReturn = ਨਿਰਾ;
            }
        }
        taskEXIT_CRITICAL( &( pxSemaphore->x_ਕਤਾਰ_ਤਾਲਾ ) );

        ਵਾਪਸ pxReturn;
    } /*lint !e818 xSemaphore cannot be a pointer to const because it is a typedef. */

#ਜੇ_ਅੰਤ /* if ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) ) */
/*-----------------------------------------------------------*/

#ਜੇ ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) )

    TaskHandle_t x_ਕਤਾਰ_Get_Mutex_Holder_From_ISR( ਕਤਾਰ_Handle_t xSemaphore )
    {
        TaskHandle_t pxReturn;

        configASSERT( xSemaphore );

        /* Mutexes cannot be used in interrupt service routines, so the mutex
         * holder should not change in an ISR, and therefore a critical section is
         * not required here. */
        ਜੇ( ( ( ਕਤਾਰ_t * ) xSemaphore )->ux_ਕਤਾਰ_Type == ਕਤਾਰ_ਕਤਾਰ_IS_MUTEX )
        {
            pxReturn = ( ( ਕਤਾਰ_t * ) xSemaphore )->u.xSemaphore.xMutexHolder;
        }
        ਨਹੀਂਤਾਂ
        {
            pxReturn = ਨਿਰਾ;
        }

        ਵਾਪਸ pxReturn;
    } /*lint !e818 xSemaphore cannot be a pointer to const because it is a typedef. */

#ਜੇ_ਅੰਤ /* if ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) ) */
/*-----------------------------------------------------------*/

#ਜੇ ( configUSE_RECURSIVE_MUTEXES == 1 )

    ਅਧਾਰ_Type_t x_ਕਤਾਰ_Give_Mutex_Recursive( ਕਤਾਰ_Handle_t xMutex )
    {
        ਅਧਾਰ_Type_t xReturn;
        ਕਤਾਰ_t * ਅਚਲ pxMutex = ( ਕਤਾਰ_t * ) xMutex;

        configASSERT( pxMutex );

        /* If this is the task that holds the mutex then xMutexHolder will not
         * change outside of this task.  If this task does not hold the mutex then
         * pxMutexHolder can never coincidentally equal the tasks handle, and as
         * this is the only condition we are interested in it does not matter if
         * pxMutexHolder is accessed simultaneously by another task.  Therefore no
         * mutual exclusion is required to test the pxMutexHolder variable. */
        ਜੇ( pxMutex->u.xSemaphore.xMutexHolder == xTaskGetCurrentTaskHandle() )
        {
            traceGIVE_MUTEX_RECURSIVE( pxMutex );

            /* uxRecursiveCallCount cannot be zero if xMutexHolder is equal to
             * the task handle, therefore no underflow check is required.  Also,
             * uxRecursiveCallCount is only modified by the mutex holder, and as
             * there can only be one, no mutual exclusion is required to modify the
             * uxRecursiveCallCount member. */
            ( pxMutex->u.xSemaphore.ux_Recursive_Call_ਗਿਣਤੀ )--;

            /* Has the recursive call count unwound to 0? */
            ਜੇ( pxMutex->u.xSemaphore.ux_Recursive_Call_ਗਿਣਤੀ == ( UBaseType_t ) 0 )
            {
                /* Return the mutex.  This will automatically unblock any other
                 * task that might be waiting to access the mutex. */
                ( ਖਾਲੀ ) x_ਕਤਾਰ_Generic_Send( pxMutex, ਨਿਰਾ, ਕਤਾਰ_MUTEX_GIVE_ਖੰਡ_ਸਮਾਂ, ਕਤਾਰ_SEND_ਮੰਜ਼ਿਲ_ਪਿੱਠ );
            }
            ਨਹੀਂਤਾਂ
            {
                mtCOVERAGE_TEST_MARKER();
            }

            xReturn = pdPASS;
        }
        ਨਹੀਂਤਾਂ
        {
            /* The mutex cannot be given because the calling task is not the
             * holder. */
            xReturn = pdFAIL;

            traceGIVE_MUTEX_RECURSIVE_FAILED( pxMutex );
        }

        ਵਾਪਸ xReturn;
    }

#ਜੇ_ਅੰਤ /* configUSE_RECURSIVE_MUTEXES */
/*-----------------------------------------------------------*/

#ਜੇ ( configUSE_RECURSIVE_MUTEXES == 1 )

    ਅਧਾਰ_Type_t x_ਕਤਾਰ_Take_Mutex_Recursive( ਕਤਾਰ_Handle_t xMutex,
                                         TickType_t x_Ticks_ਮੰਜ਼ਿਲ_Wait )
    {
        ਅਧਾਰ_Type_t xReturn;
        ਕਤਾਰ_t * ਅਚਲ pxMutex = ( ਕਤਾਰ_t * ) xMutex;

        configASSERT( pxMutex );

        /* Comments regarding mutual exclusion as per those within
         * xQueueGiveMutexRecursive(). */

        traceTAKE_MUTEX_RECURSIVE( pxMutex );

        ਜੇ( pxMutex->u.xSemaphore.xMutexHolder == xTaskGetCurrentTaskHandle() )
        {
            ( pxMutex->u.xSemaphore.ux_Recursive_Call_ਗਿਣਤੀ )++;
            xReturn = pdPASS;
        }
        ਨਹੀਂਤਾਂ
        {
            xReturn = x_ਕਤਾਰ_Semaphore_Take( pxMutex, x_Ticks_ਮੰਜ਼ਿਲ_Wait );

            /* pdPASS will only be returned if the mutex was successfully
             * obtained.  The calling task may have entered the Blocked state
             * before reaching here. */
            ਜੇ( xReturn != pdFAIL )
            {
                ( pxMutex->u.xSemaphore.ux_Recursive_Call_ਗਿਣਤੀ )++;
            }
            ਨਹੀਂਤਾਂ
            {
                traceTAKE_MUTEX_RECURSIVE_FAILED( pxMutex );
            }
        }

        ਵਾਪਸ xReturn;
    }

#ਜੇ_ਅੰਤ /* configUSE_RECURSIVE_MUTEXES */
/*-----------------------------------------------------------*/

#ਜੇ ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( config_SUPPORT_STATIC_ਵੰਡ == 1 ) )

    ਕਤਾਰ_Handle_t x_ਕਤਾਰ_Create_Counting_Semaphore_Static( ਅਚਲ UBaseType_t ux_Max_ਗਿਣਤੀ,
                                                       ਅਚਲ UBaseType_t ux_Initial_ਗਿਣਤੀ,
                                                       Static_ਕਤਾਰ_t * px_Static_ਕਤਾਰ )
    {
        ਕਤਾਰ_Handle_t xHandle = ਨਿਰਾ;

        ਜੇ( ( ux_Max_ਗਿਣਤੀ != 0 ) &&
            ( ux_Initial_ਗਿਣਤੀ <= ux_Max_ਗਿਣਤੀ ) )
        {
            xHandle = x_ਕਤਾਰ_Generic_Create_Static( ux_Max_ਗਿਣਤੀ, ਕਤਾਰ_SEMAPHORE_ਕਤਾਰ_ਵਸਤੂ_ਲੰਮਾਈ, ਨਿਰਾ, px_Static_ਕਤਾਰ, ਕਤਾਰ_ਕਤਾਰ_TYPE_COUNTING_SEMAPHORE );

            ਜੇ( xHandle != ਨਿਰਾ )
            {
                ( ( ਕਤਾਰ_t * ) xHandle )->ux_Messages_ਉਡੀਕ = ux_Initial_ਗਿਣਤੀ;

                traceCREATE_COUNTING_SEMAPHORE();
            }
            ਨਹੀਂਤਾਂ
            {
                traceCREATE_COUNTING_SEMAPHORE_FAILED();
            }
        }
        ਨਹੀਂਤਾਂ
        {
            configASSERT( xHandle );
            mtCOVERAGE_TEST_MARKER();
        }

        ਵਾਪਸ xHandle;
    }

#ਜੇ_ਅੰਤ /* ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) ) */
/*-----------------------------------------------------------*/

#ਜੇ ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( config_SUPPORT_DYNAMIC_ਵੰਡ == 1 ) )

    ਕਤਾਰ_Handle_t x_ਕਤਾਰ_Create_Counting_Semaphore( ਅਚਲ UBaseType_t ux_Max_ਗਿਣਤੀ,
                                                 ਅਚਲ UBaseType_t ux_Initial_ਗਿਣਤੀ )
    {
        ਕਤਾਰ_Handle_t xHandle = ਨਿਰਾ;

        ਜੇ( ( ux_Max_ਗਿਣਤੀ != 0 ) &&
            ( ux_Initial_ਗਿਣਤੀ <= ux_Max_ਗਿਣਤੀ ) )
        {
            xHandle = x_ਕਤਾਰ_Generic_Create( ux_Max_ਗਿਣਤੀ, ਕਤਾਰ_SEMAPHORE_ਕਤਾਰ_ਵਸਤੂ_ਲੰਮਾਈ, ਕਤਾਰ_ਕਤਾਰ_TYPE_COUNTING_SEMAPHORE );

            ਜੇ( xHandle != ਨਿਰਾ )
            {
                ( ( ਕਤਾਰ_t * ) xHandle )->ux_Messages_ਉਡੀਕ = ux_Initial_ਗਿਣਤੀ;

                traceCREATE_COUNTING_SEMAPHORE();
            }
            ਨਹੀਂਤਾਂ
            {
                traceCREATE_COUNTING_SEMAPHORE_FAILED();
            }
        }
        ਨਹੀਂਤਾਂ
        {
            configASSERT( xHandle );
            mtCOVERAGE_TEST_MARKER();
        }

        ਵਾਪਸ xHandle;
    }

#ਜੇ_ਅੰਤ /* ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) ) */
/*-----------------------------------------------------------*/

ਅਧਾਰ_Type_t x_ਕਤਾਰ_Generic_Send( ਕਤਾਰ_Handle_t x_ਕਤਾਰ,
                              ਅਚਲ ਖਾਲੀ * ਅਚਲ pv_ਵਸਤੂ_ਮੰਜ਼ਿਲ_ਕਤਾਰ,
                              TickType_t x_Ticks_ਮੰਜ਼ਿਲ_Wait,
                              ਅਚਲ ਅਧਾਰ_Type_t xCopyPosition )
{
    ਅਧਾਰ_Type_t x_ਇੰਦਰਾਜ_ਸਮਾਂ_ਸੈੱਟ = pdFALSE, xYieldRequired;
    ਸਮਾਂ_ਬਾਹਰਲਾ_t x_ਸਮਾਂ_ਬਾਹਰਲਾ_2;
    ਕਤਾਰ_t * ਅਚਲ px_ਕਤਾਰ = x_ਕਤਾਰ;

    configASSERT( px_ਕਤਾਰ );
    configASSERT( !( ( pv_ਵਸਤੂ_ਮੰਜ਼ਿਲ_ਕਤਾਰ == ਨਿਰਾ ) && ( px_ਕਤਾਰ->ux_ਵਸਤੂ_ਮਾਪ != ( UBaseType_t ) 0U ) ) );
    configASSERT( !( ( xCopyPosition == ਕਤਾਰ_OVERWRITE ) && ( px_ਕਤਾਰ->ux_ਲੰਮਾਈ != 1 ) ) );
    #ਜੇ ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_ਮੰਜ਼ਿਲ_Wait != 0 ) ) );
    }
    #ਜੇ_ਅੰਤ

    /*lint -save -e904 This function relaxes the coding standard somewhat to
     * allow return statements within the function itself.  This is done in the
     * interest of execution time efficiency. */
    ਲਈ( ; ; )
    {
        taskENTER_CRITICAL( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ) );
        {
            /* Is there room on the queue now?  The running task must be the
             * highest priority task wanting to access the queue.  If the head item
             * in the queue is to be overwritten then it does not matter if the
             * queue is full. */
            ਜੇ( ( px_ਕਤਾਰ->ux_Messages_ਉਡੀਕ < px_ਕਤਾਰ->ux_ਲੰਮਾਈ ) || ( xCopyPosition == ਕਤਾਰ_OVERWRITE ) )
            {
                trace_ਕਤਾਰ_SEND( px_ਕਤਾਰ );

                #ਜੇ ( config_USE_ਕਤਾਰ_SETS == 1 )
                {
                    ਅਚਲ UBaseType_t ux_ਪਿਛਲਾ_Messages_ਉਡੀਕ = px_ਕਤਾਰ->ux_Messages_ਉਡੀਕ;

                    xYieldRequired = prv_Copy_ਅੰਕੜੇ_ਮੰਜ਼ਿਲ_ਕਤਾਰ( px_ਕਤਾਰ, pv_ਵਸਤੂ_ਮੰਜ਼ਿਲ_ਕਤਾਰ, xCopyPosition );

                    ਜੇ( px_ਕਤਾਰ->px_ਕਤਾਰ_ਸੈੱਟ_Container != ਨਿਰਾ )
                    {
                        ਜੇ( ( xCopyPosition == ਕਤਾਰ_OVERWRITE ) && ( ux_ਪਿਛਲਾ_Messages_ਉਡੀਕ != ( UBaseType_t ) 0 ) )
                        {
                            /* Do not notify the queue set as an existing item
                             * was overwritten in the queue so the number of items
                             * in the queue has not changed. */
                            mtCOVERAGE_TEST_MARKER();
                        }
                        ਨਹੀਂਤਾਂ ਜੇ( prv_Notify_ਕਤਾਰ_ਸੈੱਟ_Container( px_ਕਤਾਰ ) != pdFALSE )
                        {
                            /* The queue is a member of a queue set, and posting
                             * to the queue set caused a higher priority task to
                             * unblock. A context switch is required. */
                            ਕਤਾਰ_YIELD_IF_USING_PREEMPTION();
                        }
                        ਨਹੀਂਤਾਂ
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    ਨਹੀਂਤਾਂ
                    {
                        /* If there was a task waiting for data to arrive on the
                         * queue then unblock it now. */
                        ਜੇ( list_LIST_IS_ਖਾਲੀ_ਥਾਵਾਂ( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive ) ) == pdFALSE )
                        {
                            ਜੇ( xTaskRemoveFromEventList( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive ) ) != pdFALSE )
                            {
                                /* The unblocked task has a priority higher than
                                 * our own so yield immediately.  Yes it is ok to
                                 * do this from within the critical section - the
                                 * kernel takes care of that. */
                                ਕਤਾਰ_YIELD_IF_USING_PREEMPTION();
                            }
                            ਨਹੀਂਤਾਂ
                            {
                                mtCOVERAGE_TEST_MARKER();
                            }
                        }
                        ਨਹੀਂਤਾਂ ਜੇ( xYieldRequired != pdFALSE )
                        {
                            /* This path is a special case that will only get
                             * executed if the task was holding multiple mutexes
                             * and the mutexes were given back in an order that is
                             * different to that in which they were taken. */
                            ਕਤਾਰ_YIELD_IF_USING_PREEMPTION();
                        }
                        ਨਹੀਂਤਾਂ
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                }
                #ਨਹੀਂਤਾਂ /* configUSE_QUEUE_SETS */
                {
                    xYieldRequired = prv_Copy_ਅੰਕੜੇ_ਮੰਜ਼ਿਲ_ਕਤਾਰ( px_ਕਤਾਰ, pv_ਵਸਤੂ_ਮੰਜ਼ਿਲ_ਕਤਾਰ, xCopyPosition );

                    /* If there was a task waiting for data to arrive on the
                     * queue then unblock it now. */
                    ਜੇ( list_LIST_IS_ਖਾਲੀ_ਥਾਵਾਂ( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive ) ) == pdFALSE )
                    {
                        ਜੇ( xTaskRemoveFromEventList( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive ) ) != pdFALSE )
                        {
                            /* The unblocked task has a priority higher than
                             * our own so yield immediately.  Yes it is ok to do
                             * this from within the critical section - the kernel
                             * takes care of that. */
                            ਕਤਾਰ_YIELD_IF_USING_PREEMPTION();
                        }
                        ਨਹੀਂਤਾਂ
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    ਨਹੀਂਤਾਂ ਜੇ( xYieldRequired != pdFALSE )
                    {
                        /* This path is a special case that will only get
                         * executed if the task was holding multiple mutexes and
                         * the mutexes were given back in an order that is
                         * different to that in which they were taken. */
                        ਕਤਾਰ_YIELD_IF_USING_PREEMPTION();
                    }
                    ਨਹੀਂਤਾਂ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #ਜੇ_ਅੰਤ /* configUSE_QUEUE_SETS */

                taskEXIT_CRITICAL( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ) );
                ਵਾਪਸ pdPASS;
            }
            ਨਹੀਂਤਾਂ
            {
                ਜੇ( x_Ticks_ਮੰਜ਼ਿਲ_Wait == ( TickType_t ) 0 )
                {
                    /* The queue was full and no block time is specified (or
                     * the block time has expired) so leave now. */
                    taskEXIT_CRITICAL( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ) );

                    /* Return to the original privilege level before exiting
                     * the function. */
                    trace_ਕਤਾਰ_SEND_FAILED( px_ਕਤਾਰ );
                    ਵਾਪਸ err_ਕਤਾਰ_ਭਰਿਆ;
                }
                ਨਹੀਂਤਾਂ ਜੇ( x_ਇੰਦਰਾਜ_ਸਮਾਂ_ਸੈੱਟ == pdFALSE )
                {
                    /* The queue was full and a block time was specified so
                     * configure the timeout structure. */
                    v_Task_ਅੰਦਰੂਨੀ_ਸੈੱਟ_ਸਮਾਂ_ਬਾਹਰਲਾ_State( &x_ਸਮਾਂ_ਬਾਹਰਲਾ_2 );
                    x_ਇੰਦਰਾਜ_ਸਮਾਂ_ਸੈੱਟ = pdTRUE;
                }
                ਨਹੀਂਤਾਂ
                {
                    /* Entry time was already set. */
                    mtCOVERAGE_TEST_MARKER();
                }
            }

            /* If queue locks ARE NOT being used:
             * - At this point, the queue is full and entry time has been set
             * - We simply check for a time out, block if not timed out, or
             *   return an error if we have timed out. */
            #ਜੇ ( ਕਤਾਰ_USE_LOCKS == 0 )
            {
                /* Update the timeout state to see if it has expired yet. */
                ਜੇ( x_Task_Check_For_ਸਮਾਂ_ਬਾਹਰਲਾ( &x_ਸਮਾਂ_ਬਾਹਰਲਾ_2, &x_Ticks_ਮੰਜ਼ਿਲ_Wait ) == pdFALSE )
                {
                    /* Not timed out yet. Block the current task. */
                    trace_BLOCKING_ON_ਕਤਾਰ_SEND( px_ਕਤਾਰ );
                    v_Task_ਰੱਖੋ_On_Event_List( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Send ), x_Ticks_ਮੰਜ਼ਿਲ_Wait );
                    ਪੋਰਟ_YIELD_WITHIN_API();
                }
                ਨਹੀਂਤਾਂ
                {
                    /* We have timed out. Return an error. */
                    taskEXIT_CRITICAL( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ) );
                    trace_ਕਤਾਰ_SEND_FAILED( px_ਕਤਾਰ );
                    ਵਾਪਸ err_ਕਤਾਰ_ਭਰਿਆ;
                }
            }
            #ਜੇ_ਅੰਤ /* queueUSE_LOCKS == 0 */
        }
        taskEXIT_CRITICAL( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ) );

        /* If queue locks ARE being used:
         * - At this point, the queue is full and entry time has been set
         * - We follow the original procedure of locking the queue before
         *   attempting to block. */
        #ਜੇ ( ਕਤਾਰ_USE_LOCKS == 1 )
        {
            /* Interrupts and other tasks can send to and receive from the queue
             * now the critical section has been exited. */

            vTaskSuspendAll();
            prv_ਤਾਲਾ_ਕਤਾਰ( px_ਕਤਾਰ );

            /* Update the timeout state to see if it has expired yet. */
            ਜੇ( x_Task_Check_For_ਸਮਾਂ_ਬਾਹਰਲਾ( &x_ਸਮਾਂ_ਬਾਹਰਲਾ_2, &x_Ticks_ਮੰਜ਼ਿਲ_Wait ) == pdFALSE )
            {
                ਜੇ( prv_Is_ਕਤਾਰ_ਭਰਿਆ( px_ਕਤਾਰ ) != pdFALSE )
                {
                    trace_BLOCKING_ON_ਕਤਾਰ_SEND( px_ਕਤਾਰ );
                    v_Task_ਰੱਖੋ_On_Event_List( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Send ), x_Ticks_ਮੰਜ਼ਿਲ_Wait );

                    /* Unlocking the queue means queue events can effect the
                     * event list. It is possible that interrupts occurring now
                     * remove this task from the event list again - but as the
                     * scheduler is suspended the task will go onto the pending
                     * ready list instead of the actual ready list. */
                    prv_Unlock_ਕਤਾਰ( px_ਕਤਾਰ );

                    /* Resuming the scheduler will move tasks from the pending
                     * ready list into the ready list - so it is feasible that this
                     * task is already in the ready list before it yields - in which
                     * case the yield will not cause a context switch unless there
                     * is also a higher priority task in the pending ready list. */
                    ਜੇ( xTaskResumeAll() == pdFALSE )
                    {
                        ਪੋਰਟ_YIELD_WITHIN_API();
                    }
                }
                ਨਹੀਂਤਾਂ
                {
                    /* Try again. */
                    prv_Unlock_ਕਤਾਰ( px_ਕਤਾਰ );
                    ( ਖਾਲੀ ) xTaskResumeAll();
                }
            }
            ਨਹੀਂਤਾਂ
            {
                /* The timeout has expired. */
                prv_Unlock_ਕਤਾਰ( px_ਕਤਾਰ );
                ( ਖਾਲੀ ) xTaskResumeAll();

                trace_ਕਤਾਰ_SEND_FAILED( px_ਕਤਾਰ );
                ਵਾਪਸ err_ਕਤਾਰ_ਭਰਿਆ;
            }
        }
        #ਜੇ_ਅੰਤ /* queueUSE_LOCKS == 1 */
    } /*lint -restore */
}
/*-----------------------------------------------------------*/

ਅਧਾਰ_Type_t x_ਕਤਾਰ_Generic_Send_From_ISR( ਕਤਾਰ_Handle_t x_ਕਤਾਰ,
                                     ਅਚਲ ਖਾਲੀ * ਅਚਲ pv_ਵਸਤੂ_ਮੰਜ਼ਿਲ_ਕਤਾਰ,
                                     ਅਧਾਰ_Type_t * ਅਚਲ px_Higher_ਤਰਜੀਹ_Task_Woken,
                                     ਅਚਲ ਅਧਾਰ_Type_t xCopyPosition )
{
    ਅਧਾਰ_Type_t xReturn;
    UBaseType_t ux_Saved_ਰੁਕਾਵਟ_ਸਥਿਤੀ;
    ਕਤਾਰ_t * ਅਚਲ px_ਕਤਾਰ = x_ਕਤਾਰ;

    configASSERT( px_ਕਤਾਰ );
    configASSERT( !( ( pv_ਵਸਤੂ_ਮੰਜ਼ਿਲ_ਕਤਾਰ == ਨਿਰਾ ) && ( px_ਕਤਾਰ->ux_ਵਸਤੂ_ਮਾਪ != ( UBaseType_t ) 0U ) ) );
    configASSERT( !( ( xCopyPosition == ਕਤਾਰ_OVERWRITE ) && ( px_ਕਤਾਰ->ux_ਲੰਮਾਈ != 1 ) ) );

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
    ਪੋਰਟ_ASSERT_IF_ਰੁਕਾਵਟ_ਤਰਜੀਹ_INVALID();

    /* Similar to xQueueGenericSend, except without blocking if there is no room
     * in the queue.  Also don't directly wake a task that was blocked on a queue
     * read, instead return a flag to say whether a context switch is required or
     * not (i.e. has a task with a higher priority than us been woken by this
     * post). */
    prv_ENTER_CRITICAL_OR_ਨਕਾਬ_ISR( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ), ux_Saved_ਰੁਕਾਵਟ_ਸਥਿਤੀ );
    {
        ਜੇ( ( px_ਕਤਾਰ->ux_Messages_ਉਡੀਕ < px_ਕਤਾਰ->ux_ਲੰਮਾਈ ) || ( xCopyPosition == ਕਤਾਰ_OVERWRITE ) )
        {
            #ਜੇ ( ਕਤਾਰ_USE_LOCKS == 1 )
                ਅਚਲ int8_t c_Tx_ਤਾਲਾ = px_ਕਤਾਰ->c_Tx_ਤਾਲਾ;
            #ਨਹੀਂਤਾਂ
                /* Queue locks not used, so we treat it as unlocked. */
                ਅਚਲ int8_t c_Tx_ਤਾਲਾ = ਕਤਾਰ_UNLOCKED;
            #ਜੇ_ਅੰਤ /* queueUSE_LOCKS == 1 */
            ਅਚਲ UBaseType_t ux_ਪਿਛਲਾ_Messages_ਉਡੀਕ = px_ਕਤਾਰ->ux_Messages_ਉਡੀਕ;

            trace_ਕਤਾਰ_SEND_FROM_ISR( px_ਕਤਾਰ );

            /* Semaphores use xQueueGiveFromISR(), so pxQueue will not be a
             *  semaphore or mutex.  That means prvCopyDataToQueue() cannot result
             *  in a task disinheriting a priority and prvCopyDataToQueue() can be
             *  called here even though the disinherit function does not check if
             *  the scheduler is suspended before accessing the ready lists. */
            ( ਖਾਲੀ ) prv_Copy_ਅੰਕੜੇ_ਮੰਜ਼ਿਲ_ਕਤਾਰ( px_ਕਤਾਰ, pv_ਵਸਤੂ_ਮੰਜ਼ਿਲ_ਕਤਾਰ, xCopyPosition );

            /* The event list is not altered if the queue is locked.  This will
             * be done when the queue is unlocked later. */
            ਜੇ( c_Tx_ਤਾਲਾ == ਕਤਾਰ_UNLOCKED )
            {
                #ਜੇ ( config_USE_ਕਤਾਰ_SETS == 1 )
                {
                    ਜੇ( px_ਕਤਾਰ->px_ਕਤਾਰ_ਸੈੱਟ_Container != ਨਿਰਾ )
                    {
                        ਜੇ( ( xCopyPosition == ਕਤਾਰ_OVERWRITE ) && ( ux_ਪਿਛਲਾ_Messages_ਉਡੀਕ != ( UBaseType_t ) 0 ) )
                        {
                            /* Do not notify the queue set as an existing item
                             * was overwritten in the queue so the number of items
                             * in the queue has not changed. */
                            mtCOVERAGE_TEST_MARKER();
                        }
                        ਨਹੀਂਤਾਂ ਜੇ( prv_Notify_ਕਤਾਰ_ਸੈੱਟ_Container( px_ਕਤਾਰ ) != pdFALSE )
                        {
                            /* The queue is a member of a queue set, and posting
                             * to the queue set caused a higher priority task to
                             * unblock.  A context switch is required. */
                            ਜੇ( px_Higher_ਤਰਜੀਹ_Task_Woken != ਨਿਰਾ )
                            {
                                *px_Higher_ਤਰਜੀਹ_Task_Woken = pdTRUE;
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
                    }
                    ਨਹੀਂਤਾਂ
                    {
                        ਜੇ( list_LIST_IS_ਖਾਲੀ_ਥਾਵਾਂ( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive ) ) == pdFALSE )
                        {
                            ਜੇ( xTaskRemoveFromEventList( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive ) ) != pdFALSE )
                            {
                                /* The task waiting has a higher priority so
                                 *  record that a context switch is required. */
                                ਜੇ( px_Higher_ਤਰਜੀਹ_Task_Woken != ਨਿਰਾ )
                                {
                                    *px_Higher_ਤਰਜੀਹ_Task_Woken = pdTRUE;
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
                        }
                        ਨਹੀਂਤਾਂ
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                }
                #ਨਹੀਂਤਾਂ /* configUSE_QUEUE_SETS */
                {
                    ਜੇ( list_LIST_IS_ਖਾਲੀ_ਥਾਵਾਂ( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive ) ) == pdFALSE )
                    {
                        ਜੇ( xTaskRemoveFromEventList( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive ) ) != pdFALSE )
                        {
                            /* The task waiting has a higher priority so record that a
                             * context switch is required. */
                            ਜੇ( px_Higher_ਤਰਜੀਹ_Task_Woken != ਨਿਰਾ )
                            {
                                *px_Higher_ਤਰਜੀਹ_Task_Woken = pdTRUE;
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
                    }
                    ਨਹੀਂਤਾਂ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }

                    /* Not used in this path. */
                    ( ਖਾਲੀ ) ux_ਪਿਛਲਾ_Messages_ਉਡੀਕ;
                }
                #ਜੇ_ਅੰਤ /* configUSE_QUEUE_SETS */
            }
            ਨਹੀਂਤਾਂ
            {
                #ਜੇ ( ਕਤਾਰ_USE_LOCKS == 1 )
                {
                    /* Increment the lock count so the task that unlocks the queue
                     * knows that data was posted while it was locked. */
                    prv_Increment_ਕਤਾਰ_Tx_ਤਾਲਾ( px_ਕਤਾਰ, c_Tx_ਤਾਲਾ );
                }
                #ਜੇ_ਅੰਤ /* queueUSE_LOCKS == 1 */
            }

            xReturn = pdPASS;
        }
        ਨਹੀਂਤਾਂ
        {
            trace_ਕਤਾਰ_SEND_FROM_ISR_FAILED( px_ਕਤਾਰ );
            xReturn = err_ਕਤਾਰ_ਭਰਿਆ;
        }
    }
    prvEXIT_CRITICAL_OR_UNMASK_ISR( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ), ux_Saved_ਰੁਕਾਵਟ_ਸਥਿਤੀ );

    ਵਾਪਸ xReturn;
}
/*-----------------------------------------------------------*/

ਅਧਾਰ_Type_t x_ਕਤਾਰ_Give_From_ISR( ਕਤਾਰ_Handle_t x_ਕਤਾਰ,
                              ਅਧਾਰ_Type_t * ਅਚਲ px_Higher_ਤਰਜੀਹ_Task_Woken )
{
    ਅਧਾਰ_Type_t xReturn;
    UBaseType_t ux_Saved_ਰੁਕਾਵਟ_ਸਥਿਤੀ;
    ਕਤਾਰ_t * ਅਚਲ px_ਕਤਾਰ = x_ਕਤਾਰ;

    /* Similar to xQueueGenericSendFromISR() but used with semaphores where the
     * item size is 0.  Don't directly wake a task that was blocked on a queue
     * read, instead return a flag to say whether a context switch is required or
     * not (i.e. has a task with a higher priority than us been woken by this
     * post). */

    configASSERT( px_ਕਤਾਰ );

    /* xQueueGenericSendFromISR() should be used instead of xQueueGiveFromISR()
     * if the item size is not 0. */
    configASSERT( px_ਕਤਾਰ->ux_ਵਸਤੂ_ਮਾਪ == 0 );

    /* Normally a mutex would not be given from an interrupt, especially if
     * there is a mutex holder, as priority inheritance makes no sense for an
     * interrupts, only tasks. */
    configASSERT( !( ( px_ਕਤਾਰ->ux_ਕਤਾਰ_Type == ਕਤਾਰ_ਕਤਾਰ_IS_MUTEX ) && ( px_ਕਤਾਰ->u.xSemaphore.xMutexHolder != ਨਿਰਾ ) ) );

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
    ਪੋਰਟ_ASSERT_IF_ਰੁਕਾਵਟ_ਤਰਜੀਹ_INVALID();

    prv_ENTER_CRITICAL_OR_ਨਕਾਬ_ISR( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ), ux_Saved_ਰੁਕਾਵਟ_ਸਥਿਤੀ );
    {
        ਅਚਲ UBaseType_t ux_Messages_ਉਡੀਕ = px_ਕਤਾਰ->ux_Messages_ਉਡੀਕ;

        /* When the queue is used to implement a semaphore no data is ever
         * moved through the queue but it is still valid to see if the queue 'has
         * space'. */
        ਜੇ( ux_Messages_ਉਡੀਕ < px_ਕਤਾਰ->ux_ਲੰਮਾਈ )
        {
            #ਜੇ ( ਕਤਾਰ_USE_LOCKS == 1 )
                ਅਚਲ int8_t c_Tx_ਤਾਲਾ = px_ਕਤਾਰ->c_Tx_ਤਾਲਾ;
            #ਨਹੀਂਤਾਂ
                /* Queue locks not used, so we treat it as unlocked. */
                ਅਚਲ int8_t c_Tx_ਤਾਲਾ = ਕਤਾਰ_UNLOCKED;
            #ਜੇ_ਅੰਤ /* queueUSE_LOCKS == 1 */

            /* Todo: Reconcile tracing differences (IDF-8183) */
            trace_ਕਤਾਰ_GIVE_FROM_ISR( px_ਕਤਾਰ );

            /* A task can only have an inherited priority if it is a mutex
             * holder - and if there is a mutex holder then the mutex cannot be
             * given from an ISR.  As this is the ISR version of the function it
             * can be assumed there is no mutex holder and no need to determine if
             * priority disinheritance is needed.  Simply increase the count of
             * messages (semaphores) available. */
            px_ਕਤਾਰ->ux_Messages_ਉਡੀਕ = ux_Messages_ਉਡੀਕ + ( UBaseType_t ) 1;

            /* The event list is not altered if the queue is locked.  This will
             * be done when the queue is unlocked later. */
            ਜੇ( c_Tx_ਤਾਲਾ == ਕਤਾਰ_UNLOCKED )
            {
                #ਜੇ ( config_USE_ਕਤਾਰ_SETS == 1 )
                {
                    ਜੇ( px_ਕਤਾਰ->px_ਕਤਾਰ_ਸੈੱਟ_Container != ਨਿਰਾ )
                    {
                        ਜੇ( prv_Notify_ਕਤਾਰ_ਸੈੱਟ_Container( px_ਕਤਾਰ ) != pdFALSE )
                        {
                            /* The semaphore is a member of a queue set, and
                             * posting to the queue set caused a higher priority
                             * task to unblock.  A context switch is required. */
                            ਜੇ( px_Higher_ਤਰਜੀਹ_Task_Woken != ਨਿਰਾ )
                            {
                                *px_Higher_ਤਰਜੀਹ_Task_Woken = pdTRUE;
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
                    }
                    ਨਹੀਂਤਾਂ
                    {
                        ਜੇ( list_LIST_IS_ਖਾਲੀ_ਥਾਵਾਂ( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive ) ) == pdFALSE )
                        {
                            ਜੇ( xTaskRemoveFromEventList( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive ) ) != pdFALSE )
                            {
                                /* The task waiting has a higher priority so
                                 *  record that a context switch is required. */
                                ਜੇ( px_Higher_ਤਰਜੀਹ_Task_Woken != ਨਿਰਾ )
                                {
                                    *px_Higher_ਤਰਜੀਹ_Task_Woken = pdTRUE;
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
                        }
                        ਨਹੀਂਤਾਂ
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                }
                #ਨਹੀਂਤਾਂ /* configUSE_QUEUE_SETS */
                {
                    ਜੇ( list_LIST_IS_ਖਾਲੀ_ਥਾਵਾਂ( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive ) ) == pdFALSE )
                    {
                        ਜੇ( xTaskRemoveFromEventList( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive ) ) != pdFALSE )
                        {
                            /* The task waiting has a higher priority so record that a
                             * context switch is required. */
                            ਜੇ( px_Higher_ਤਰਜੀਹ_Task_Woken != ਨਿਰਾ )
                            {
                                *px_Higher_ਤਰਜੀਹ_Task_Woken = pdTRUE;
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
                    }
                    ਨਹੀਂਤਾਂ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #ਜੇ_ਅੰਤ /* configUSE_QUEUE_SETS */
            }
            ਨਹੀਂਤਾਂ
            {
                #ਜੇ ( ਕਤਾਰ_USE_LOCKS == 1 )
                {
                    /* Increment the lock count so the task that unlocks the queue
                     * knows that data was posted while it was locked. */
                    prv_Increment_ਕਤਾਰ_Tx_ਤਾਲਾ( px_ਕਤਾਰ, c_Tx_ਤਾਲਾ );
                }
                #ਜੇ_ਅੰਤ /* queueUSE_LOCKS == 1 */
            }

            xReturn = pdPASS;
        }
        ਨਹੀਂਤਾਂ
        {
            /* Todo: Reconcile tracing differences (IDF-8183) */
            trace_ਕਤਾਰ_GIVE_FROM_ISR_FAILED( px_ਕਤਾਰ );
            xReturn = err_ਕਤਾਰ_ਭਰਿਆ;
        }
    }
    prvEXIT_CRITICAL_OR_UNMASK_ISR( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ), ux_Saved_ਰੁਕਾਵਟ_ਸਥਿਤੀ );

    ਵਾਪਸ xReturn;
}
/*-----------------------------------------------------------*/

ਅਧਾਰ_Type_t x_ਕਤਾਰ_Receive( ਕਤਾਰ_Handle_t x_ਕਤਾਰ,
                          ਖਾਲੀ * ਅਚਲ pv_ਬਫਰ,
                          TickType_t x_Ticks_ਮੰਜ਼ਿਲ_Wait )
{
    ਅਧਾਰ_Type_t x_ਇੰਦਰਾਜ_ਸਮਾਂ_ਸੈੱਟ = pdFALSE;
    ਸਮਾਂ_ਬਾਹਰਲਾ_t x_ਸਮਾਂ_ਬਾਹਰਲਾ_2;
    ਕਤਾਰ_t * ਅਚਲ px_ਕਤਾਰ = x_ਕਤਾਰ;

    /* Check the pointer is not NULL. */
    configASSERT( ( px_ਕਤਾਰ ) );

    /* The buffer into which data is received can only be NULL if the data size
     * is zero (so no data is copied into the buffer). */
    configASSERT( !( ( ( pv_ਬਫਰ ) == ਨਿਰਾ ) && ( ( px_ਕਤਾਰ )->ux_ਵਸਤੂ_ਮਾਪ != ( UBaseType_t ) 0U ) ) );

    /* Cannot block if the scheduler is suspended. */
    #ਜੇ ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_ਮੰਜ਼ਿਲ_Wait != 0 ) ) );
    }
    #ਜੇ_ਅੰਤ

    /*lint -save -e904  This function relaxes the coding standard somewhat to
     * allow return statements within the function itself.  This is done in the
     * interest of execution time efficiency. */
    ਲਈ( ; ; )
    {
        taskENTER_CRITICAL( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ) );
        {
            ਅਚਲ UBaseType_t ux_Messages_ਉਡੀਕ = px_ਕਤਾਰ->ux_Messages_ਉਡੀਕ;

            /* Is there data in the queue now?  To be running the calling task
             * must be the highest priority task wanting to access the queue. */
            ਜੇ( ux_Messages_ਉਡੀਕ > ( UBaseType_t ) 0 )
            {
                /* Data available, remove one item. */
                prv_Copy_ਅੰਕੜੇ_From_ਕਤਾਰ( px_ਕਤਾਰ, pv_ਬਫਰ );
                trace_ਕਤਾਰ_RECEIVE( px_ਕਤਾਰ );
                px_ਕਤਾਰ->ux_Messages_ਉਡੀਕ = ux_Messages_ਉਡੀਕ - ( UBaseType_t ) 1;

                /* There is now space in the queue, were any tasks waiting to
                 * post to the queue?  If so, unblock the highest priority waiting
                 * task. */
                ਜੇ( list_LIST_IS_ਖਾਲੀ_ਥਾਵਾਂ( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Send ) ) == pdFALSE )
                {
                    ਜੇ( xTaskRemoveFromEventList( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Send ) ) != pdFALSE )
                    {
                        ਕਤਾਰ_YIELD_IF_USING_PREEMPTION();
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

                taskEXIT_CRITICAL( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ) );
                ਵਾਪਸ pdPASS;
            }
            ਨਹੀਂਤਾਂ
            {
                ਜੇ( x_Ticks_ਮੰਜ਼ਿਲ_Wait == ( TickType_t ) 0 )
                {
                    /* The queue was empty and no block time is specified (or
                     * the block time has expired) so leave now. */
                    taskEXIT_CRITICAL( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ) );
                    trace_ਕਤਾਰ_RECEIVE_FAILED( px_ਕਤਾਰ );
                    ਵਾਪਸ err_ਕਤਾਰ_ਖਾਲੀ_ਥਾਵਾਂ;
                }
                ਨਹੀਂਤਾਂ ਜੇ( x_ਇੰਦਰਾਜ_ਸਮਾਂ_ਸੈੱਟ == pdFALSE )
                {
                    /* The queue was empty and a block time was specified so
                     * configure the timeout structure. */
                    v_Task_ਅੰਦਰੂਨੀ_ਸੈੱਟ_ਸਮਾਂ_ਬਾਹਰਲਾ_State( &x_ਸਮਾਂ_ਬਾਹਰਲਾ_2 );
                    x_ਇੰਦਰਾਜ_ਸਮਾਂ_ਸੈੱਟ = pdTRUE;
                }
                ਨਹੀਂਤਾਂ
                {
                    /* Entry time was already set. */
                    mtCOVERAGE_TEST_MARKER();
                }
            }

            /* If queue locks ARE NOT being used:
             * - At this point, the queue is empty and entry time has been set
             * - We simply check for a time out, block if not timed out, or
             *   return an error if we have timed out. */
            #ਜੇ ( ਕਤਾਰ_USE_LOCKS == 0 )
            {
                /* Update the timeout state to see if it has expired yet. */
                ਜੇ( x_Task_Check_For_ਸਮਾਂ_ਬਾਹਰਲਾ( &x_ਸਮਾਂ_ਬਾਹਰਲਾ_2, &x_Ticks_ਮੰਜ਼ਿਲ_Wait ) == pdFALSE )
                {
                    /* Not timed out yet. Block the current task. */
                    trace_BLOCKING_ON_ਕਤਾਰ_RECEIVE( px_ਕਤਾਰ );
                    v_Task_ਰੱਖੋ_On_Event_List( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive ), x_Ticks_ਮੰਜ਼ਿਲ_Wait );
                    ਪੋਰਟ_YIELD_WITHIN_API();
                }
                ਨਹੀਂਤਾਂ
                {
                    /* We have timed out. Return an error. */
                    taskEXIT_CRITICAL( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ) );
                    trace_ਕਤਾਰ_RECEIVE_FAILED( px_ਕਤਾਰ );
                    ਵਾਪਸ err_ਕਤਾਰ_ਖਾਲੀ_ਥਾਵਾਂ;
                }
            }
            #ਜੇ_ਅੰਤ /* queueUSE_LOCKS == 0 */
        }
        taskEXIT_CRITICAL( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ) );

        /* If queue locks ARE being used:
         * - At this point, the queue is empty and entry time has been set
         * - We follow the original procedure for locking the queue before
         *   attempting to block. */
        #ਜੇ ( ਕਤਾਰ_USE_LOCKS == 1 )
        {
            /* Interrupts and other tasks can send to and receive from the queue
             * now the critical section has been exited. */

            vTaskSuspendAll();
            prv_ਤਾਲਾ_ਕਤਾਰ( px_ਕਤਾਰ );

            /* Update the timeout state to see if it has expired yet. */
            ਜੇ( x_Task_Check_For_ਸਮਾਂ_ਬਾਹਰਲਾ( &x_ਸਮਾਂ_ਬਾਹਰਲਾ_2, &x_Ticks_ਮੰਜ਼ਿਲ_Wait ) == pdFALSE )
            {
                /* The timeout has not expired.  If the queue is still empty place
                 * the task on the list of tasks waiting to receive from the queue. */
                ਜੇ( prv_Is_ਕਤਾਰ_ਖਾਲੀ_ਥਾਵਾਂ( px_ਕਤਾਰ ) != pdFALSE )
                {
                    trace_BLOCKING_ON_ਕਤਾਰ_RECEIVE( px_ਕਤਾਰ );
                    v_Task_ਰੱਖੋ_On_Event_List( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive ), x_Ticks_ਮੰਜ਼ਿਲ_Wait );
                    prv_Unlock_ਕਤਾਰ( px_ਕਤਾਰ );

                    ਜੇ( xTaskResumeAll() == pdFALSE )
                    {
                        ਪੋਰਟ_YIELD_WITHIN_API();
                    }
                    ਨਹੀਂਤਾਂ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                ਨਹੀਂਤਾਂ
                {
                    /* The queue contains data again.  Loop back to try and read the
                     * data. */
                    prv_Unlock_ਕਤਾਰ( px_ਕਤਾਰ );
                    ( ਖਾਲੀ ) xTaskResumeAll();
                }
            }
            ਨਹੀਂਤਾਂ
            {
                /* Timed out.  If there is no data in the queue exit, otherwise loop
                 * back and attempt to read the data. */
                prv_Unlock_ਕਤਾਰ( px_ਕਤਾਰ );
                ( ਖਾਲੀ ) xTaskResumeAll();

                ਜੇ( prv_Is_ਕਤਾਰ_ਖਾਲੀ_ਥਾਵਾਂ( px_ਕਤਾਰ ) != pdFALSE )
                {
                    trace_ਕਤਾਰ_RECEIVE_FAILED( px_ਕਤਾਰ );
                    ਵਾਪਸ err_ਕਤਾਰ_ਖਾਲੀ_ਥਾਵਾਂ;
                }
                ਨਹੀਂਤਾਂ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
        }
        #ਜੇ_ਅੰਤ /* queueUSE_LOCKS == 1 */
    } /*lint -restore */
}
/*-----------------------------------------------------------*/

ਅਧਾਰ_Type_t x_ਕਤਾਰ_Semaphore_Take( ਕਤਾਰ_Handle_t x_ਕਤਾਰ,
                                TickType_t x_Ticks_ਮੰਜ਼ਿਲ_Wait )
{
    ਅਧਾਰ_Type_t x_ਇੰਦਰਾਜ_ਸਮਾਂ_ਸੈੱਟ = pdFALSE;
    ਸਮਾਂ_ਬਾਹਰਲਾ_t x_ਸਮਾਂ_ਬਾਹਰਲਾ_2;
    ਕਤਾਰ_t * ਅਚਲ px_ਕਤਾਰ = x_ਕਤਾਰ;

    #ਜੇ ( configUSE_MUTEXES == 1 )
        ਅਧਾਰ_Type_t xInheritanceOccurred = pdFALSE;
    #ਜੇ_ਅੰਤ

    /* Check the queue pointer is not NULL. */
    configASSERT( ( px_ਕਤਾਰ ) );

    /* Check this really is a semaphore, in which case the item size will be
     * 0. */
    configASSERT( px_ਕਤਾਰ->ux_ਵਸਤੂ_ਮਾਪ == 0 );

    /* Cannot block if the scheduler is suspended. */
    #ਜੇ ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_ਮੰਜ਼ਿਲ_Wait != 0 ) ) );
    }
    #ਜੇ_ਅੰਤ

    /*lint -save -e904 This function relaxes the coding standard somewhat to allow return
     * statements within the function itself.  This is done in the interest
     * of execution time efficiency. */
    ਲਈ( ; ; )
    {
        taskENTER_CRITICAL( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ) );
        {
            /* Semaphores are queues with an item size of 0, and where the
             * number of messages in the queue is the semaphore's count value. */
            ਅਚਲ UBaseType_t ux_Semaphore_ਗਿਣਤੀ = px_ਕਤਾਰ->ux_Messages_ਉਡੀਕ;

            /* Is there data in the queue now?  To be running the calling task
             * must be the highest priority task wanting to access the queue. */
            ਜੇ( ux_Semaphore_ਗਿਣਤੀ > ( UBaseType_t ) 0 )
            {
                /* Todo: Reconcile tracing differences (IDF-8183) */
                trace_ਕਤਾਰ_SEMAPHORE_RECEIVE( px_ਕਤਾਰ );

                /* Semaphores are queues with a data size of zero and where the
                 * messages waiting is the semaphore's count.  Reduce the count. */
                px_ਕਤਾਰ->ux_Messages_ਉਡੀਕ = ux_Semaphore_ਗਿਣਤੀ - ( UBaseType_t ) 1;

                #ਜੇ ( configUSE_MUTEXES == 1 )
                {
                    ਜੇ( px_ਕਤਾਰ->ux_ਕਤਾਰ_Type == ਕਤਾਰ_ਕਤਾਰ_IS_MUTEX )
                    {
                        /* Record the information required to implement
                         * priority inheritance should it become necessary. */
                        px_ਕਤਾਰ->u.xSemaphore.xMutexHolder = pv_Task_Increment_Mutex_Held_ਗਿਣਤੀ();
                    }
                    ਨਹੀਂਤਾਂ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #ਜੇ_ਅੰਤ /* configUSE_MUTEXES */

                /* Check to see if other tasks are blocked waiting to give the
                 * semaphore, and if so, unblock the highest priority such task. */
                ਜੇ( list_LIST_IS_ਖਾਲੀ_ਥਾਵਾਂ( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Send ) ) == pdFALSE )
                {
                    ਜੇ( xTaskRemoveFromEventList( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Send ) ) != pdFALSE )
                    {
                        ਕਤਾਰ_YIELD_IF_USING_PREEMPTION();
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

                taskEXIT_CRITICAL( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ) );
                ਵਾਪਸ pdPASS;
            }
            ਨਹੀਂਤਾਂ
            {
                ਜੇ( x_Ticks_ਮੰਜ਼ਿਲ_Wait == ( TickType_t ) 0 )
                {
                    /* The semaphore count was 0 and no block time is specified
                     * (or the block time has expired) so exit now. */
                    taskEXIT_CRITICAL( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ) );
                    trace_ਕਤਾਰ_RECEIVE_FAILED( px_ਕਤਾਰ );
                    ਵਾਪਸ err_ਕਤਾਰ_ਖਾਲੀ_ਥਾਵਾਂ;
                }
                ਨਹੀਂਤਾਂ ਜੇ( x_ਇੰਦਰਾਜ_ਸਮਾਂ_ਸੈੱਟ == pdFALSE )
                {
                    /* The semaphore count was 0 and a block time was specified
                     * so configure the timeout structure ready to block. */
                    v_Task_ਅੰਦਰੂਨੀ_ਸੈੱਟ_ਸਮਾਂ_ਬਾਹਰਲਾ_State( &x_ਸਮਾਂ_ਬਾਹਰਲਾ_2 );
                    x_ਇੰਦਰਾਜ_ਸਮਾਂ_ਸੈੱਟ = pdTRUE;
                }
                ਨਹੀਂਤਾਂ
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
            #ਜੇ ( ਕਤਾਰ_USE_LOCKS == 0 )
            {
                /* Update the timeout state to see if it has expired yet. */
                ਜੇ( x_Task_Check_For_ਸਮਾਂ_ਬਾਹਰਲਾ( &x_ਸਮਾਂ_ਬਾਹਰਲਾ_2, &x_Ticks_ਮੰਜ਼ਿਲ_Wait ) == pdFALSE )
                {
                    /* Not timed out yet. If this is a mutex, make the holder
                     * inherit our priority, then block the current task. */
                    trace_BLOCKING_ON_ਕਤਾਰ_RECEIVE( px_ਕਤਾਰ );
                    #ਜੇ ( configUSE_MUTEXES == 1 )
                    {
                        ਜੇ( px_ਕਤਾਰ->ux_ਕਤਾਰ_Type == ਕਤਾਰ_ਕਤਾਰ_IS_MUTEX )
                        {
                            xInheritanceOccurred = x_Task_ਤਰਜੀਹ_Inherit( px_ਕਤਾਰ->u.xSemaphore.xMutexHolder );
                        }
                        ਨਹੀਂਤਾਂ
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    #ਜੇ_ਅੰਤ /* if ( configUSE_MUTEXES == 1 ) */
                    v_Task_ਰੱਖੋ_On_Event_List( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive ), x_Ticks_ਮੰਜ਼ਿਲ_Wait );
                    ਪੋਰਟ_YIELD_WITHIN_API();
                }
                ਨਹੀਂਤਾਂ
                {
                    /* We have timed out. If this is a mutex, make the holder
                     * disinherit our priority, then return an error. */
                    #ਜੇ ( configUSE_MUTEXES == 1 )
                    {
                        ਜੇ( xInheritanceOccurred != pdFALSE )
                        {
                            UBaseType_t ux_Highest_ਉਡੀਕ_ਤਰਜੀਹ;
                            ux_Highest_ਉਡੀਕ_ਤਰਜੀਹ = prv_Get_Disinherit_ਤਰਜੀਹ_After_Timeout( px_ਕਤਾਰ );
                            v_Task_ਤਰਜੀਹ_Disinherit_After_Timeout( px_ਕਤਾਰ->u.xSemaphore.xMutexHolder, ux_Highest_ਉਡੀਕ_ਤਰਜੀਹ );
                        }
                    }
                    #ਜੇ_ਅੰਤ /* configUSE_MUTEXES */
                    taskEXIT_CRITICAL( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ) );
                    trace_ਕਤਾਰ_RECEIVE_FAILED( px_ਕਤਾਰ );
                    ਵਾਪਸ err_ਕਤਾਰ_ਖਾਲੀ_ਥਾਵਾਂ;
                }
            }
            #ਜੇ_ਅੰਤ /* queueUSE_LOCKS == 0 */
        }
        taskEXIT_CRITICAL( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ) );

        /* If queue locks ARE being used:
         * - At this point, the semaphore/mutex is empty/held and entry time
         *   has been set.
         * - We follow the original procedure for locking the queue, inheriting
         *   priority, then attempting to block. */
        #ਜੇ ( ਕਤਾਰ_USE_LOCKS == 1 )
        {
            /* Interrupts and other tasks can give to and take from the semaphore
             * now the critical section has been exited. */

            vTaskSuspendAll();
            prv_ਤਾਲਾ_ਕਤਾਰ( px_ਕਤਾਰ );

            /* Update the timeout state to see if it has expired yet. */
            ਜੇ( x_Task_Check_For_ਸਮਾਂ_ਬਾਹਰਲਾ( &x_ਸਮਾਂ_ਬਾਹਰਲਾ_2, &x_Ticks_ਮੰਜ਼ਿਲ_Wait ) == pdFALSE )
            {
                /* A block time is specified and not expired.  If the semaphore
                 * count is 0 then enter the Blocked state to wait for a semaphore to
                 * become available.  As semaphores are implemented with queues the
                 * queue being empty is equivalent to the semaphore count being 0. */
                ਜੇ( prv_Is_ਕਤਾਰ_ਖਾਲੀ_ਥਾਵਾਂ( px_ਕਤਾਰ ) != pdFALSE )
                {
                    trace_BLOCKING_ON_ਕਤਾਰ_RECEIVE( px_ਕਤਾਰ );

                    #ਜੇ ( configUSE_MUTEXES == 1 )
                    {
                        ਜੇ( px_ਕਤਾਰ->ux_ਕਤਾਰ_Type == ਕਤਾਰ_ਕਤਾਰ_IS_MUTEX )
                        {
                            taskENTER_CRITICAL( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ) );
                            {
                                xInheritanceOccurred = x_Task_ਤਰਜੀਹ_Inherit( px_ਕਤਾਰ->u.xSemaphore.xMutexHolder );
                            }
                            taskEXIT_CRITICAL( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ) );
                        }
                        ਨਹੀਂਤਾਂ
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    #ਜੇ_ਅੰਤ /* if ( configUSE_MUTEXES == 1 ) */

                    v_Task_ਰੱਖੋ_On_Event_List( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive ), x_Ticks_ਮੰਜ਼ਿਲ_Wait );
                    prv_Unlock_ਕਤਾਰ( px_ਕਤਾਰ );

                    ਜੇ( xTaskResumeAll() == pdFALSE )
                    {
                        ਪੋਰਟ_YIELD_WITHIN_API();
                    }
                    ਨਹੀਂਤਾਂ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                ਨਹੀਂਤਾਂ
                {
                    /* There was no timeout and the semaphore count was not 0, so
                     * attempt to take the semaphore again. */
                    prv_Unlock_ਕਤਾਰ( px_ਕਤਾਰ );
                    ( ਖਾਲੀ ) xTaskResumeAll();
                }
            }
            ਨਹੀਂਤਾਂ
            {
                /* Timed out. */
                prv_Unlock_ਕਤਾਰ( px_ਕਤਾਰ );
                ( ਖਾਲੀ ) xTaskResumeAll();

                /* If the semaphore count is 0 exit now as the timeout has
                 * expired.  Otherwise return to attempt to take the semaphore that is
                 * known to be available.  As semaphores are implemented by queues the
                 * queue being empty is equivalent to the semaphore count being 0. */
                ਜੇ( prv_Is_ਕਤਾਰ_ਖਾਲੀ_ਥਾਵਾਂ( px_ਕਤਾਰ ) != pdFALSE )
                {
                    #ਜੇ ( configUSE_MUTEXES == 1 )
                    {
                        /* xInheritanceOccurred could only have be set if
                         * pxQueue->uxQueueType == queueQUEUE_IS_MUTEX so no need to
                         * test the mutex type again to check it is actually a mutex. */
                        ਜੇ( xInheritanceOccurred != pdFALSE )
                        {
                            taskENTER_CRITICAL( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ) );
                            {
                                UBaseType_t ux_Highest_ਉਡੀਕ_ਤਰਜੀਹ;

                                /* This task blocking on the mutex caused another
                                 * task to inherit this task's priority.  Now this task
                                 * has timed out the priority should be disinherited
                                 * again, but only as low as the next highest priority
                                 * task that is waiting for the same mutex. */
                                ux_Highest_ਉਡੀਕ_ਤਰਜੀਹ = prv_Get_Disinherit_ਤਰਜੀਹ_After_Timeout( px_ਕਤਾਰ );
                                v_Task_ਤਰਜੀਹ_Disinherit_After_Timeout( px_ਕਤਾਰ->u.xSemaphore.xMutexHolder, ux_Highest_ਉਡੀਕ_ਤਰਜੀਹ );
                            }
                            taskEXIT_CRITICAL( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ) );
                        }
                    }
                    #ਜੇ_ਅੰਤ /* configUSE_MUTEXES */

                    trace_ਕਤਾਰ_RECEIVE_FAILED( px_ਕਤਾਰ );
                    ਵਾਪਸ err_ਕਤਾਰ_ਖਾਲੀ_ਥਾਵਾਂ;
                }
                ਨਹੀਂਤਾਂ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
        }
        #ਜੇ_ਅੰਤ /* queueUSE_LOCKS == 1 */
    } /*lint -restore */
}
/*-----------------------------------------------------------*/

ਅਧਾਰ_Type_t x_ਕਤਾਰ_Peek( ਕਤਾਰ_Handle_t x_ਕਤਾਰ,
                       ਖਾਲੀ * ਅਚਲ pv_ਬਫਰ,
                       TickType_t x_Ticks_ਮੰਜ਼ਿਲ_Wait )
{
    ਅਧਾਰ_Type_t x_ਇੰਦਰਾਜ_ਸਮਾਂ_ਸੈੱਟ = pdFALSE;
    ਸਮਾਂ_ਬਾਹਰਲਾ_t x_ਸਮਾਂ_ਬਾਹਰਲਾ_2;
    int8_t * pcOriginalReadPosition;
    ਕਤਾਰ_t * ਅਚਲ px_ਕਤਾਰ = x_ਕਤਾਰ;

    /* Check the pointer is not NULL. */
    configASSERT( ( px_ਕਤਾਰ ) );

    /* The buffer into which data is received can only be NULL if the data size
     * is zero (so no data is copied into the buffer. */
    configASSERT( !( ( ( pv_ਬਫਰ ) == ਨਿਰਾ ) && ( ( px_ਕਤਾਰ )->ux_ਵਸਤੂ_ਮਾਪ != ( UBaseType_t ) 0U ) ) );

    /* Cannot block if the scheduler is suspended. */
    #ਜੇ ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_ਮੰਜ਼ਿਲ_Wait != 0 ) ) );
    }
    #ਜੇ_ਅੰਤ

    /*lint -save -e904  This function relaxes the coding standard somewhat to
     * allow return statements within the function itself.  This is done in the
     * interest of execution time efficiency. */
    ਲਈ( ; ; )
    {
        taskENTER_CRITICAL( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ) );
        {
            ਅਚਲ UBaseType_t ux_Messages_ਉਡੀਕ = px_ਕਤਾਰ->ux_Messages_ਉਡੀਕ;

            /* Is there data in the queue now?  To be running the calling task
             * must be the highest priority task wanting to access the queue. */
            ਜੇ( ux_Messages_ਉਡੀਕ > ( UBaseType_t ) 0 )
            {
                /* Remember the read position so it can be reset after the data
                 * is read from the queue as this function is only peeking the
                 * data, not removing it. */
                pcOriginalReadPosition = px_ਕਤਾਰ->u.x_ਕਤਾਰ.pcReadFrom;

                prv_Copy_ਅੰਕੜੇ_From_ਕਤਾਰ( px_ਕਤਾਰ, pv_ਬਫਰ );
                trace_ਕਤਾਰ_PEEK( px_ਕਤਾਰ );

                /* The data is not being removed, so reset the read pointer. */
                px_ਕਤਾਰ->u.x_ਕਤਾਰ.pcReadFrom = pcOriginalReadPosition;

                /* The data is being left in the queue, so see if there are
                 * any other tasks waiting for the data. */
                ਜੇ( list_LIST_IS_ਖਾਲੀ_ਥਾਵਾਂ( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive ) ) == pdFALSE )
                {
                    ਜੇ( xTaskRemoveFromEventList( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive ) ) != pdFALSE )
                    {
                        /* The task waiting has a higher priority than this task. */
                        ਕਤਾਰ_YIELD_IF_USING_PREEMPTION();
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

                taskEXIT_CRITICAL( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ) );
                ਵਾਪਸ pdPASS;
            }
            ਨਹੀਂਤਾਂ
            {
                ਜੇ( x_Ticks_ਮੰਜ਼ਿਲ_Wait == ( TickType_t ) 0 )
                {
                    /* The queue was empty and no block time is specified (or
                     * the block time has expired) so leave now. */
                    taskEXIT_CRITICAL( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ) );
                    trace_ਕਤਾਰ_PEEK_FAILED( px_ਕਤਾਰ );
                    ਵਾਪਸ err_ਕਤਾਰ_ਖਾਲੀ_ਥਾਵਾਂ;
                }
                ਨਹੀਂਤਾਂ ਜੇ( x_ਇੰਦਰਾਜ_ਸਮਾਂ_ਸੈੱਟ == pdFALSE )
                {
                    /* The queue was empty and a block time was specified so
                     * configure the timeout structure ready to enter the blocked
                     * state. */
                    v_Task_ਅੰਦਰੂਨੀ_ਸੈੱਟ_ਸਮਾਂ_ਬਾਹਰਲਾ_State( &x_ਸਮਾਂ_ਬਾਹਰਲਾ_2 );
                    x_ਇੰਦਰਾਜ_ਸਮਾਂ_ਸੈੱਟ = pdTRUE;
                }
                ਨਹੀਂਤਾਂ
                {
                    /* Entry time was already set. */
                    mtCOVERAGE_TEST_MARKER();
                }
            }

            /* If queue locks ARE NOT being used:
             * - At this point, the queue is empty and entry time has been set
             * - We simply check for a time out, block if not timed out, or
             *   return an error if we have timed out. */
            #ਜੇ ( ਕਤਾਰ_USE_LOCKS == 0 )
            {
                /* Update the timeout state to see if it has expired yet. */
                ਜੇ( x_Task_Check_For_ਸਮਾਂ_ਬਾਹਰਲਾ( &x_ਸਮਾਂ_ਬਾਹਰਲਾ_2, &x_Ticks_ਮੰਜ਼ਿਲ_Wait ) == pdFALSE )
                {
                    /* Not timed out yet. Block the current task. */
                    trace_BLOCKING_ON_ਕਤਾਰ_PEEK( px_ਕਤਾਰ );
                    v_Task_ਰੱਖੋ_On_Event_List( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive ), x_Ticks_ਮੰਜ਼ਿਲ_Wait );
                    ਪੋਰਟ_YIELD_WITHIN_API();
                }
                ਨਹੀਂਤਾਂ
                {
                    /* We have timed out. Return an error. */
                    taskEXIT_CRITICAL( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ) );
                    trace_ਕਤਾਰ_PEEK_FAILED( px_ਕਤਾਰ );
                    ਵਾਪਸ err_ਕਤਾਰ_ਖਾਲੀ_ਥਾਵਾਂ;
                }
            }
            #ਜੇ_ਅੰਤ /* queueUSE_LOCKS == 0 */
        }
        taskEXIT_CRITICAL( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ) );

        /* If queue locks ARE being used:
         * - At this point, the queue is empty and entry time has been set
         * - We follow the original procedure for locking the queue before
         *   attempting to block. */
        #ਜੇ ( ਕਤਾਰ_USE_LOCKS == 1 )
        {
            /* Interrupts and other tasks can send to and receive from the queue
             * now that the critical section has been exited. */

            vTaskSuspendAll();
            prv_ਤਾਲਾ_ਕਤਾਰ( px_ਕਤਾਰ );

            /* Update the timeout state to see if it has expired yet. */
            ਜੇ( x_Task_Check_For_ਸਮਾਂ_ਬਾਹਰਲਾ( &x_ਸਮਾਂ_ਬਾਹਰਲਾ_2, &x_Ticks_ਮੰਜ਼ਿਲ_Wait ) == pdFALSE )
            {
                /* Timeout has not expired yet, check to see if there is data in the
                * queue now, and if not enter the Blocked state to wait for data. */
                ਜੇ( prv_Is_ਕਤਾਰ_ਖਾਲੀ_ਥਾਵਾਂ( px_ਕਤਾਰ ) != pdFALSE )
                {
                    trace_BLOCKING_ON_ਕਤਾਰ_PEEK( px_ਕਤਾਰ );
                    v_Task_ਰੱਖੋ_On_Event_List( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive ), x_Ticks_ਮੰਜ਼ਿਲ_Wait );
                    prv_Unlock_ਕਤਾਰ( px_ਕਤਾਰ );

                    ਜੇ( xTaskResumeAll() == pdFALSE )
                    {
                        ਪੋਰਟ_YIELD_WITHIN_API();
                    }
                    ਨਹੀਂਤਾਂ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                ਨਹੀਂਤਾਂ
                {
                    /* There is data in the queue now, so don't enter the blocked
                     * state, instead return to try and obtain the data. */
                    prv_Unlock_ਕਤਾਰ( px_ਕਤਾਰ );
                    ( ਖਾਲੀ ) xTaskResumeAll();
                }
            }
            ਨਹੀਂਤਾਂ
            {
                /* The timeout has expired.  If there is still no data in the queue
                 * exit, otherwise go back and try to read the data again. */
                prv_Unlock_ਕਤਾਰ( px_ਕਤਾਰ );
                ( ਖਾਲੀ ) xTaskResumeAll();

                ਜੇ( prv_Is_ਕਤਾਰ_ਖਾਲੀ_ਥਾਵਾਂ( px_ਕਤਾਰ ) != pdFALSE )
                {
                    trace_ਕਤਾਰ_PEEK_FAILED( px_ਕਤਾਰ );
                    ਵਾਪਸ err_ਕਤਾਰ_ਖਾਲੀ_ਥਾਵਾਂ;
                }
                ਨਹੀਂਤਾਂ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
        }
        #ਜੇ_ਅੰਤ /* queueUSE_LOCKS == 1 */
    } /*lint -restore */
}
/*-----------------------------------------------------------*/

ਅਧਾਰ_Type_t x_ਕਤਾਰ_Receive_From_ISR( ਕਤਾਰ_Handle_t x_ਕਤਾਰ,
                                 ਖਾਲੀ * ਅਚਲ pv_ਬਫਰ,
                                 ਅਧਾਰ_Type_t * ਅਚਲ px_Higher_ਤਰਜੀਹ_Task_Woken )
{
    ਅਧਾਰ_Type_t xReturn;
    UBaseType_t ux_Saved_ਰੁਕਾਵਟ_ਸਥਿਤੀ;
    ਕਤਾਰ_t * ਅਚਲ px_ਕਤਾਰ = x_ਕਤਾਰ;

    configASSERT( px_ਕਤਾਰ );
    configASSERT( !( ( pv_ਬਫਰ == ਨਿਰਾ ) && ( px_ਕਤਾਰ->ux_ਵਸਤੂ_ਮਾਪ != ( UBaseType_t ) 0U ) ) );

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
    ਪੋਰਟ_ASSERT_IF_ਰੁਕਾਵਟ_ਤਰਜੀਹ_INVALID();

    prv_ENTER_CRITICAL_OR_ਨਕਾਬ_ISR( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ), ux_Saved_ਰੁਕਾਵਟ_ਸਥਿਤੀ );
    {
        ਅਚਲ UBaseType_t ux_Messages_ਉਡੀਕ = px_ਕਤਾਰ->ux_Messages_ਉਡੀਕ;

        /* Cannot block in an ISR, so check there is data available. */
        ਜੇ( ux_Messages_ਉਡੀਕ > ( UBaseType_t ) 0 )
        {
            #ਜੇ ( ਕਤਾਰ_USE_LOCKS == 1 )
                ਅਚਲ int8_t c_Rx_ਤਾਲਾ = px_ਕਤਾਰ->c_Rx_ਤਾਲਾ;
            #ਨਹੀਂਤਾਂ
                /* Queue locks not used, so we treat it as unlocked. */
                ਅਚਲ int8_t c_Rx_ਤਾਲਾ = ਕਤਾਰ_UNLOCKED;
            #ਜੇ_ਅੰਤ /* queueUSE_LOCKS == 1 */

            trace_ਕਤਾਰ_RECEIVE_FROM_ISR( px_ਕਤਾਰ );

            prv_Copy_ਅੰਕੜੇ_From_ਕਤਾਰ( px_ਕਤਾਰ, pv_ਬਫਰ );
            px_ਕਤਾਰ->ux_Messages_ਉਡੀਕ = ux_Messages_ਉਡੀਕ - ( UBaseType_t ) 1;

            /* If the queue is locked the event list will not be modified.
             * Instead update the lock count so the task that unlocks the queue
             * will know that an ISR has removed data while the queue was
             * locked. */
            ਜੇ( c_Rx_ਤਾਲਾ == ਕਤਾਰ_UNLOCKED )
            {
                ਜੇ( list_LIST_IS_ਖਾਲੀ_ਥਾਵਾਂ( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Send ) ) == pdFALSE )
                {
                    ਜੇ( xTaskRemoveFromEventList( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Send ) ) != pdFALSE )
                    {
                        /* The task waiting has a higher priority than us so
                         * force a context switch. */
                        ਜੇ( px_Higher_ਤਰਜੀਹ_Task_Woken != ਨਿਰਾ )
                        {
                            *px_Higher_ਤਰਜੀਹ_Task_Woken = pdTRUE;
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
                }
                ਨਹੀਂਤਾਂ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            ਨਹੀਂਤਾਂ
            {
                #ਜੇ ( ਕਤਾਰ_USE_LOCKS == 1 )
                {
                    /* Increment the lock count so the task that unlocks the queue
                     * knows that data was removed while it was locked. */
                    prv_Increment_ਕਤਾਰ_Rx_ਤਾਲਾ( px_ਕਤਾਰ, c_Rx_ਤਾਲਾ );
                }
                #ਜੇ_ਅੰਤ /* queueUSE_LOCKS == 1 */
            }

            xReturn = pdPASS;
        }
        ਨਹੀਂਤਾਂ
        {
            xReturn = pdFAIL;
            trace_ਕਤਾਰ_RECEIVE_FROM_ISR_FAILED( px_ਕਤਾਰ );
        }
    }
    prvEXIT_CRITICAL_OR_UNMASK_ISR( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ), ux_Saved_ਰੁਕਾਵਟ_ਸਥਿਤੀ );

    ਵਾਪਸ xReturn;
}
/*-----------------------------------------------------------*/

ਅਧਾਰ_Type_t x_ਕਤਾਰ_Peek_From_ISR( ਕਤਾਰ_Handle_t x_ਕਤਾਰ,
                              ਖਾਲੀ * ਅਚਲ pv_ਬਫਰ )
{
    ਅਧਾਰ_Type_t xReturn;
    UBaseType_t ux_Saved_ਰੁਕਾਵਟ_ਸਥਿਤੀ;
    int8_t * pcOriginalReadPosition;
    ਕਤਾਰ_t * ਅਚਲ px_ਕਤਾਰ = x_ਕਤਾਰ;

    configASSERT( px_ਕਤਾਰ );
    configASSERT( !( ( pv_ਬਫਰ == ਨਿਰਾ ) && ( px_ਕਤਾਰ->ux_ਵਸਤੂ_ਮਾਪ != ( UBaseType_t ) 0U ) ) );
    configASSERT( px_ਕਤਾਰ->ux_ਵਸਤੂ_ਮਾਪ != 0 ); /* Can't peek a semaphore. */

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
    ਪੋਰਟ_ASSERT_IF_ਰੁਕਾਵਟ_ਤਰਜੀਹ_INVALID();

    prv_ENTER_CRITICAL_OR_ਨਕਾਬ_ISR( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ), ux_Saved_ਰੁਕਾਵਟ_ਸਥਿਤੀ );
    {
        /* Cannot block in an ISR, so check there is data available. */
        ਜੇ( px_ਕਤਾਰ->ux_Messages_ਉਡੀਕ > ( UBaseType_t ) 0 )
        {
            trace_ਕਤਾਰ_PEEK_FROM_ISR( px_ਕਤਾਰ );

            /* Remember the read position so it can be reset as nothing is
             * actually being removed from the queue. */
            pcOriginalReadPosition = px_ਕਤਾਰ->u.x_ਕਤਾਰ.pcReadFrom;
            prv_Copy_ਅੰਕੜੇ_From_ਕਤਾਰ( px_ਕਤਾਰ, pv_ਬਫਰ );
            px_ਕਤਾਰ->u.x_ਕਤਾਰ.pcReadFrom = pcOriginalReadPosition;

            xReturn = pdPASS;
        }
        ਨਹੀਂਤਾਂ
        {
            xReturn = pdFAIL;
            trace_ਕਤਾਰ_PEEK_FROM_ISR_FAILED( px_ਕਤਾਰ );
        }
    }
    prvEXIT_CRITICAL_OR_UNMASK_ISR( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ), ux_Saved_ਰੁਕਾਵਟ_ਸਥਿਤੀ );

    ਵਾਪਸ xReturn;
}
/*-----------------------------------------------------------*/

UBaseType_t ux_ਕਤਾਰ_Messages_ਉਡੀਕ( ਅਚਲ ਕਤਾਰ_Handle_t x_ਕਤਾਰ )
{
    UBaseType_t uxReturn;

    configASSERT( x_ਕਤਾਰ );

    taskENTER_CRITICAL( &( ( ( ਕਤਾਰ_t * ) x_ਕਤਾਰ )->x_ਕਤਾਰ_ਤਾਲਾ ) );
    {
        uxReturn = ( ( ਕਤਾਰ_t * ) x_ਕਤਾਰ )->ux_Messages_ਉਡੀਕ;
    }
    taskEXIT_CRITICAL( &( ( ( ਕਤਾਰ_t * ) x_ਕਤਾਰ )->x_ਕਤਾਰ_ਤਾਲਾ ) );

    ਵਾਪਸ uxReturn;
} /*lint !e818 Pointer cannot be declared const as xQueue is a typedef not pointer. */
/*-----------------------------------------------------------*/

UBaseType_t ux_ਕਤਾਰ_Spaces_ਉਪਲਬਧ( ਅਚਲ ਕਤਾਰ_Handle_t x_ਕਤਾਰ )
{
    UBaseType_t uxReturn;
    ਕਤਾਰ_t * ਅਚਲ px_ਕਤਾਰ = x_ਕਤਾਰ;

    configASSERT( px_ਕਤਾਰ );

    taskENTER_CRITICAL( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ) );
    {
        uxReturn = px_ਕਤਾਰ->ux_ਲੰਮਾਈ - px_ਕਤਾਰ->ux_Messages_ਉਡੀਕ;
    }
    taskEXIT_CRITICAL( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ) );

    ਵਾਪਸ uxReturn;
} /*lint !e818 Pointer cannot be declared const as xQueue is a typedef not pointer. */
/*-----------------------------------------------------------*/

UBaseType_t ux_ਕਤਾਰ_Messages_ਉਡੀਕ_From_ISR( ਅਚਲ ਕਤਾਰ_Handle_t x_ਕਤਾਰ )
{
    UBaseType_t uxReturn;
    ਕਤਾਰ_t * ਅਚਲ px_ਕਤਾਰ = x_ਕਤਾਰ;

    configASSERT( px_ਕਤਾਰ );
    uxReturn = px_ਕਤਾਰ->ux_Messages_ਉਡੀਕ;

    ਵਾਪਸ uxReturn;
} /*lint !e818 Pointer cannot be declared const as xQueue is a typedef not pointer. */
/*-----------------------------------------------------------*/

ਖਾਲੀ v_ਕਤਾਰ_Delete( ਕਤਾਰ_Handle_t x_ਕਤਾਰ )
{
    ਕਤਾਰ_t * ਅਚਲ px_ਕਤਾਰ = x_ਕਤਾਰ;

    configASSERT( px_ਕਤਾਰ );
    trace_ਕਤਾਰ_DELETE( px_ਕਤਾਰ );

    #ਜੇ ( config_ਕਤਾਰ_REGISTRY_ਮਾਪ > 0 )
    {
        v_ਕਤਾਰ_Unregister_ਕਤਾਰ( px_ਕਤਾਰ );
    }
    #ਜੇ_ਅੰਤ

    #ਜੇ ( ( config_SUPPORT_DYNAMIC_ਵੰਡ == 1 ) && ( config_SUPPORT_STATIC_ਵੰਡ == 0 ) )
    {
        /* The queue can only have been allocated dynamically - free it
         * again. */
        v_ਪੋਰਟ_Free( px_ਕਤਾਰ );
    }
    #ਜਾਂਜੇ ( ( config_SUPPORT_DYNAMIC_ਵੰਡ == 1 ) && ( config_SUPPORT_STATIC_ਵੰਡ == 1 ) )
    {
        /* The queue could have been allocated statically or dynamically, so
         * check before attempting to free the memory. */
        ਜੇ( px_ਕਤਾਰ->ucStaticallyAllocated == ( uint8_t ) pdFALSE )
        {
            v_ਪੋਰਟ_Free( px_ਕਤਾਰ );
        }
        ਨਹੀਂਤਾਂ
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    #ਨਹੀਂਤਾਂ /* if ( ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) && ( configSUPPORT_STATIC_ALLOCATION == 0 ) ) */
    {
        /* The queue must have been statically allocated, so is not going to be
         * deleted.  Avoid compiler warnings about the unused parameter. */
        ( ਖਾਲੀ ) px_ਕਤਾਰ;
    }
    #ਜੇ_ਅੰਤ /* configSUPPORT_DYNAMIC_ALLOCATION */
}
/*-----------------------------------------------------------*/

#ਜੇ ( configUSE_TRACE_FACILITY == 1 )

    UBaseType_t ux_ਕਤਾਰ_Get_ਕਤਾਰ_ਸੰਖਿਆ( ਕਤਾਰ_Handle_t x_ਕਤਾਰ )
    {
        ਵਾਪਸ ( ( ਕਤਾਰ_t * ) x_ਕਤਾਰ )->ux_ਕਤਾਰ_ਸੰਖਿਆ;
    }

#ਜੇ_ਅੰਤ /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#ਜੇ ( configUSE_TRACE_FACILITY == 1 )

    ਖਾਲੀ v_ਕਤਾਰ_ਸੈੱਟ_ਕਤਾਰ_ਸੰਖਿਆ( ਕਤਾਰ_Handle_t x_ਕਤਾਰ,
                               UBaseType_t ux_ਕਤਾਰ_ਸੰਖਿਆ )
    {
        ( ( ਕਤਾਰ_t * ) x_ਕਤਾਰ )->ux_ਕਤਾਰ_ਸੰਖਿਆ = ux_ਕਤਾਰ_ਸੰਖਿਆ;
    }

#ਜੇ_ਅੰਤ /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#ਜੇ ( configUSE_TRACE_FACILITY == 1 )

    uint8_t uc_ਕਤਾਰ_Get_ਕਤਾਰ_Type( ਕਤਾਰ_Handle_t x_ਕਤਾਰ )
    {
        ਵਾਪਸ ( ( ਕਤਾਰ_t * ) x_ਕਤਾਰ )->uc_ਕਤਾਰ_Type;
    }

#ਜੇ_ਅੰਤ /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#ਜੇ ( configUSE_MUTEXES == 1 )

    ਸਥਿਰ UBaseType_t prv_Get_Disinherit_ਤਰਜੀਹ_After_Timeout( ਅਚਲ ਕਤਾਰ_t * ਅਚਲ px_ਕਤਾਰ )
    {
        UBaseType_t ux_Highest_ਤਰਜੀਹ_ਦਾ_ਉਡੀਕ_Tasks;

        /* If a task waiting for a mutex causes the mutex holder to inherit a
         * priority, but the waiting task times out, then the holder should
         * disinherit the priority - but only down to the highest priority of any
         * other tasks that are waiting for the same mutex.  For this purpose,
         * return the priority of the highest priority task that is waiting for the
         * mutex. */
        ਜੇ( list_CURRENT_LIST_ਲੰਮਾਈ( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive ) ) > 0U )
        {
            ux_Highest_ਤਰਜੀਹ_ਦਾ_ਉਡੀਕ_Tasks = ( UBaseType_t ) configMAX_PRIORITIES - ( UBaseType_t ) list_GET_ਵਸਤੂ_ਮੁੱਲ_ਦਾ_ਸਿਰ_ਇੰਦਰਾਜ( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive ) );
        }
        ਨਹੀਂਤਾਂ
        {
            ux_Highest_ਤਰਜੀਹ_ਦਾ_ਉਡੀਕ_Tasks = tsk_IDLE_ਤਰਜੀਹ;
        }

        ਵਾਪਸ ux_Highest_ਤਰਜੀਹ_ਦਾ_ਉਡੀਕ_Tasks;
    }

#ਜੇ_ਅੰਤ /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

ਸਥਿਰ ਅਧਾਰ_Type_t prv_Copy_ਅੰਕੜੇ_ਮੰਜ਼ਿਲ_ਕਤਾਰ( ਕਤਾਰ_t * ਅਚਲ px_ਕਤਾਰ,
                                      ਅਚਲ ਖਾਲੀ * pv_ਵਸਤੂ_ਮੰਜ਼ਿਲ_ਕਤਾਰ,
                                      ਅਚਲ ਅਧਾਰ_Type_t xPosition )
{
    ਅਧਾਰ_Type_t xReturn = pdFALSE;
    UBaseType_t ux_Messages_ਉਡੀਕ;

    /* This function is called from a critical section. */

    ux_Messages_ਉਡੀਕ = px_ਕਤਾਰ->ux_Messages_ਉਡੀਕ;

    ਜੇ( px_ਕਤਾਰ->ux_ਵਸਤੂ_ਮਾਪ == ( UBaseType_t ) 0 )
    {
        #ਜੇ ( configUSE_MUTEXES == 1 )
        {
            ਜੇ( px_ਕਤਾਰ->ux_ਕਤਾਰ_Type == ਕਤਾਰ_ਕਤਾਰ_IS_MUTEX )
            {
                /* The mutex is no longer being held. */
                xReturn = x_Task_ਤਰਜੀਹ_Disinherit( px_ਕਤਾਰ->u.xSemaphore.xMutexHolder );
                px_ਕਤਾਰ->u.xSemaphore.xMutexHolder = ਨਿਰਾ;
            }
            ਨਹੀਂਤਾਂ
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        #ਜੇ_ਅੰਤ /* configUSE_MUTEXES */
    }
    ਨਹੀਂਤਾਂ ਜੇ( xPosition == ਕਤਾਰ_SEND_ਮੰਜ਼ਿਲ_ਪਿੱਠ )
    {
        ( ਖਾਲੀ ) ਸਮ੍ਰਿਤੀ_ਨਕਲ( ( ਖਾਲੀ * ) px_ਕਤਾਰ->pc_Write_ਮੰਜ਼ਿਲ, pv_ਵਸਤੂ_ਮੰਜ਼ਿਲ_ਕਤਾਰ, ( size_t ) px_ਕਤਾਰ->ux_ਵਸਤੂ_ਮਾਪ ); /*lint !e961 !e418 !e9087 MISRA exception as the casts are only redundant for some ports, plus previous logic ensures a null pointer can only be passed to memcpy() if the copy size is 0.  Cast to void required by function signature and safe as no alignment requirement and copy length specified in bytes. */
        px_ਕਤਾਰ->pc_Write_ਮੰਜ਼ਿਲ += px_ਕਤਾਰ->ux_ਵਸਤੂ_ਮਾਪ;                                                       /*lint !e9016 Pointer arithmetic on char types ok, especially in this use case where it is the clearest way of conveying intent. */

        ਜੇ( px_ਕਤਾਰ->pc_Write_ਮੰਜ਼ਿਲ >= px_ਕਤਾਰ->u.x_ਕਤਾਰ.pc_ਪੂਛ )                                             /*lint !e946 MISRA exception justified as comparison of pointers is the cleanest solution. */
        {
            px_ਕਤਾਰ->pc_Write_ਮੰਜ਼ਿਲ = px_ਕਤਾਰ->pc_ਸਿਰ;
        }
        ਨਹੀਂਤਾਂ
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    ਨਹੀਂਤਾਂ
    {
        ( ਖਾਲੀ ) ਸਮ੍ਰਿਤੀ_ਨਕਲ( ( ਖਾਲੀ * ) px_ਕਤਾਰ->u.x_ਕਤਾਰ.pcReadFrom, pv_ਵਸਤੂ_ਮੰਜ਼ਿਲ_ਕਤਾਰ, ( size_t ) px_ਕਤਾਰ->ux_ਵਸਤੂ_ਮਾਪ ); /*lint !e961 !e9087 !e418 MISRA exception as the casts are only redundant for some ports.  Cast to void required by function signature and safe as no alignment requirement and copy length specified in bytes.  Assert checks null pointer only used when length is 0. */
        px_ਕਤਾਰ->u.x_ਕਤਾਰ.pcReadFrom -= px_ਕਤਾਰ->ux_ਵਸਤੂ_ਮਾਪ;

        ਜੇ( px_ਕਤਾਰ->u.x_ਕਤਾਰ.pcReadFrom < px_ਕਤਾਰ->pc_ਸਿਰ ) /*lint !e946 MISRA exception justified as comparison of pointers is the cleanest solution. */
        {
            px_ਕਤਾਰ->u.x_ਕਤਾਰ.pcReadFrom = ( px_ਕਤਾਰ->u.x_ਕਤਾਰ.pc_ਪੂਛ - px_ਕਤਾਰ->ux_ਵਸਤੂ_ਮਾਪ );
        }
        ਨਹੀਂਤਾਂ
        {
            mtCOVERAGE_TEST_MARKER();
        }

        ਜੇ( xPosition == ਕਤਾਰ_OVERWRITE )
        {
            ਜੇ( ux_Messages_ਉਡੀਕ > ( UBaseType_t ) 0 )
            {
                /* An item is not being added but overwritten, so subtract
                 * one from the recorded number of items in the queue so when
                 * one is added again below the number of recorded items remains
                 * correct. */
                --ux_Messages_ਉਡੀਕ;
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
    }

    px_ਕਤਾਰ->ux_Messages_ਉਡੀਕ = ux_Messages_ਉਡੀਕ + ( UBaseType_t ) 1;

    ਵਾਪਸ xReturn;
}
/*-----------------------------------------------------------*/

ਸਥਿਰ ਖਾਲੀ prv_Copy_ਅੰਕੜੇ_From_ਕਤਾਰ( ਕਤਾਰ_t * ਅਚਲ px_ਕਤਾਰ,
                                  ਖਾਲੀ * ਅਚਲ pv_ਬਫਰ )
{
    ਜੇ( px_ਕਤਾਰ->ux_ਵਸਤੂ_ਮਾਪ != ( UBaseType_t ) 0 )
    {
        px_ਕਤਾਰ->u.x_ਕਤਾਰ.pcReadFrom += px_ਕਤਾਰ->ux_ਵਸਤੂ_ਮਾਪ;           /*lint !e9016 Pointer arithmetic on char types ok, especially in this use case where it is the clearest way of conveying intent. */

        ਜੇ( px_ਕਤਾਰ->u.x_ਕਤਾਰ.pcReadFrom >= px_ਕਤਾਰ->u.x_ਕਤਾਰ.pc_ਪੂਛ ) /*lint !e946 MISRA exception justified as use of the relational operator is the cleanest solutions. */
        {
            px_ਕਤਾਰ->u.x_ਕਤਾਰ.pcReadFrom = px_ਕਤਾਰ->pc_ਸਿਰ;
        }
        ਨਹੀਂਤਾਂ
        {
            mtCOVERAGE_TEST_MARKER();
        }

        ( ਖਾਲੀ ) ਸਮ੍ਰਿਤੀ_ਨਕਲ( ( ਖਾਲੀ * ) pv_ਬਫਰ, ( ਖਾਲੀ * ) px_ਕਤਾਰ->u.x_ਕਤਾਰ.pcReadFrom, ( size_t ) px_ਕਤਾਰ->ux_ਵਸਤੂ_ਮਾਪ ); /*lint !e961 !e418 !e9087 MISRA exception as the casts are only redundant for some ports.  Also previous logic ensures a null pointer can only be passed to memcpy() when the count is 0.  Cast to void required by function signature and safe as no alignment requirement and copy length specified in bytes. */
    }
}
/*-----------------------------------------------------------*/

#ਜੇ ( ਕਤਾਰ_USE_LOCKS == 1 )
    ਸਥਿਰ ਖਾਲੀ prv_Unlock_ਕਤਾਰ( ਕਤਾਰ_t * ਅਚਲ px_ਕਤਾਰ )
    {
        /* THIS FUNCTION MUST BE CALLED WITH THE SCHEDULER SUSPENDED. */

        /* The lock counts contains the number of extra data items placed or
         * removed from the queue while the queue was locked.  When a queue is
         * locked items can be added or removed, but the event lists cannot be
         * updated. */
        taskENTER_CRITICAL( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ) );
        {
            int8_t c_Tx_ਤਾਲਾ = px_ਕਤਾਰ->c_Tx_ਤਾਲਾ;

            /* See if data was added to the queue while it was locked. */
            ਜਦੋਂ( c_Tx_ਤਾਲਾ > ਕਤਾਰ_LOCKED_UNMODIFIED )
            {
                /* Data was posted while the queue was locked.  Are any tasks
                 * blocked waiting for data to become available? */
                #ਜੇ ( config_USE_ਕਤਾਰ_SETS == 1 )
                {
                    ਜੇ( px_ਕਤਾਰ->px_ਕਤਾਰ_ਸੈੱਟ_Container != ਨਿਰਾ )
                    {
                        ਜੇ( prv_Notify_ਕਤਾਰ_ਸੈੱਟ_Container( px_ਕਤਾਰ ) != pdFALSE )
                        {
                            /* The queue is a member of a queue set, and posting to
                             * the queue set caused a higher priority task to unblock.
                             * A context switch is required. */
                            vTaskMissedYield();
                        }
                        ਨਹੀਂਤਾਂ
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    ਨਹੀਂਤਾਂ
                    {
                        /* Tasks that are removed from the event list will get
                         * added to the pending ready list as the scheduler is still
                         * suspended. */
                        ਜੇ( list_LIST_IS_ਖਾਲੀ_ਥਾਵਾਂ( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive ) ) == pdFALSE )
                        {
                            ਜੇ( xTaskRemoveFromEventList( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive ) ) != pdFALSE )
                            {
                                /* The task waiting has a higher priority so record that a
                                 * context switch is required. */
                                vTaskMissedYield();
                            }
                            ਨਹੀਂਤਾਂ
                            {
                                mtCOVERAGE_TEST_MARKER();
                            }
                        }
                        ਨਹੀਂਤਾਂ
                        {
                            ਰੋਕੋ;
                        }
                    }
                }
                #ਨਹੀਂਤਾਂ /* configUSE_QUEUE_SETS */
                {
                    /* Tasks that are removed from the event list will get added to
                     * the pending ready list as the scheduler is still suspended. */
                    ਜੇ( list_LIST_IS_ਖਾਲੀ_ਥਾਵਾਂ( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive ) ) == pdFALSE )
                    {
                        ਜੇ( xTaskRemoveFromEventList( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive ) ) != pdFALSE )
                        {
                            /* The task waiting has a higher priority so record that
                             * a context switch is required. */
                            vTaskMissedYield();
                        }
                        ਨਹੀਂਤਾਂ
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    ਨਹੀਂਤਾਂ
                    {
                        ਰੋਕੋ;
                    }
                }
                #ਜੇ_ਅੰਤ /* configUSE_QUEUE_SETS */

                --c_Tx_ਤਾਲਾ;
            }

            px_ਕਤਾਰ->c_Tx_ਤਾਲਾ = ਕਤਾਰ_UNLOCKED;
        }
        taskEXIT_CRITICAL( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ) );

        /* Do the same for the Rx lock. */
        taskENTER_CRITICAL( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ) );
        {
            int8_t c_Rx_ਤਾਲਾ = px_ਕਤਾਰ->c_Rx_ਤਾਲਾ;

            ਜਦੋਂ( c_Rx_ਤਾਲਾ > ਕਤਾਰ_LOCKED_UNMODIFIED )
            {
                ਜੇ( list_LIST_IS_ਖਾਲੀ_ਥਾਵਾਂ( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Send ) ) == pdFALSE )
                {
                    ਜੇ( xTaskRemoveFromEventList( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Send ) ) != pdFALSE )
                    {
                        vTaskMissedYield();
                    }
                    ਨਹੀਂਤਾਂ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }

                    --c_Rx_ਤਾਲਾ;
                }
                ਨਹੀਂਤਾਂ
                {
                    ਰੋਕੋ;
                }
            }

            px_ਕਤਾਰ->c_Rx_ਤਾਲਾ = ਕਤਾਰ_UNLOCKED;
        }
        taskEXIT_CRITICAL( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ) );
    }
#ਜੇ_ਅੰਤ /* queueUSE_LOCKS == 1 */
/*-----------------------------------------------------------*/

#ਜੇ ( ਕਤਾਰ_USE_LOCKS == 1 )
    ਸਥਿਰ ਅਧਾਰ_Type_t prv_Is_ਕਤਾਰ_ਖਾਲੀ_ਥਾਵਾਂ( ਅਚਲ ਕਤਾਰ_t * px_ਕਤਾਰ )
    {
        ਅਧਾਰ_Type_t xReturn;

        taskENTER_CRITICAL( &( ( ( ਕਤਾਰ_t * ) px_ਕਤਾਰ )->x_ਕਤਾਰ_ਤਾਲਾ ) );
        {
            ਜੇ( px_ਕਤਾਰ->ux_Messages_ਉਡੀਕ == ( UBaseType_t ) 0 )
            {
                xReturn = pdTRUE;
            }
            ਨਹੀਂਤਾਂ
            {
                xReturn = pdFALSE;
            }
        }
        taskEXIT_CRITICAL( &( ( ( ਕਤਾਰ_t * ) px_ਕਤਾਰ )->x_ਕਤਾਰ_ਤਾਲਾ ) );

        ਵਾਪਸ xReturn;
    }
#ਜੇ_ਅੰਤ /* queueUSE_LOCKS == 1 */
/*-----------------------------------------------------------*/

ਅਧਾਰ_Type_t x_ਕਤਾਰ_Is_ਕਤਾਰ_ਖਾਲੀ_ਥਾਵਾਂ_From_ISR( ਅਚਲ ਕਤਾਰ_Handle_t x_ਕਤਾਰ )
{
    ਅਧਾਰ_Type_t xReturn;
    ਕਤਾਰ_t * ਅਚਲ px_ਕਤਾਰ = x_ਕਤਾਰ;

    configASSERT( px_ਕਤਾਰ );

    ਜੇ( px_ਕਤਾਰ->ux_Messages_ਉਡੀਕ == ( UBaseType_t ) 0 )
    {
        xReturn = pdTRUE;
    }
    ਨਹੀਂਤਾਂ
    {
        xReturn = pdFALSE;
    }

    ਵਾਪਸ xReturn;
} /*lint !e818 xQueue could not be pointer to const because it is a typedef. */
/*-----------------------------------------------------------*/

#ਜੇ ( ਕਤਾਰ_USE_LOCKS == 1 )
    ਸਥਿਰ ਅਧਾਰ_Type_t prv_Is_ਕਤਾਰ_ਭਰਿਆ( ਅਚਲ ਕਤਾਰ_t * px_ਕਤਾਰ )
    {
        ਅਧਾਰ_Type_t xReturn;

        taskENTER_CRITICAL( &( ( ( ਕਤਾਰ_t * ) px_ਕਤਾਰ )->x_ਕਤਾਰ_ਤਾਲਾ ) );
        {
            ਜੇ( px_ਕਤਾਰ->ux_Messages_ਉਡੀਕ == px_ਕਤਾਰ->ux_ਲੰਮਾਈ )
            {
                xReturn = pdTRUE;
            }
            ਨਹੀਂਤਾਂ
            {
                xReturn = pdFALSE;
            }
        }
        taskEXIT_CRITICAL( &( ( ( ਕਤਾਰ_t * ) px_ਕਤਾਰ )->x_ਕਤਾਰ_ਤਾਲਾ ) );

        ਵਾਪਸ xReturn;
    }
#ਜੇ_ਅੰਤ /* queueUSE_LOCKS == 1 */
/*-----------------------------------------------------------*/

ਅਧਾਰ_Type_t x_ਕਤਾਰ_Is_ਕਤਾਰ_ਭਰਿਆ_From_ISR( ਅਚਲ ਕਤਾਰ_Handle_t x_ਕਤਾਰ )
{
    ਅਧਾਰ_Type_t xReturn;
    ਕਤਾਰ_t * ਅਚਲ px_ਕਤਾਰ = x_ਕਤਾਰ;

    configASSERT( px_ਕਤਾਰ );

    ਜੇ( px_ਕਤਾਰ->ux_Messages_ਉਡੀਕ == px_ਕਤਾਰ->ux_ਲੰਮਾਈ )
    {
        xReturn = pdTRUE;
    }
    ਨਹੀਂਤਾਂ
    {
        xReturn = pdFALSE;
    }

    ਵਾਪਸ xReturn;
} /*lint !e818 xQueue could not be pointer to const because it is a typedef. */
/*-----------------------------------------------------------*/

#ਜੇ ( configUSE_CO_ROUTINES == 1 )

    ਅਧਾਰ_Type_t x_ਕਤਾਰ_CRSend( ਕਤਾਰ_Handle_t x_ਕਤਾਰ,
                             ਅਚਲ ਖਾਲੀ * pv_ਵਸਤੂ_ਮੰਜ਼ਿਲ_ਕਤਾਰ,
                             TickType_t x_Ticks_ਮੰਜ਼ਿਲ_Wait )
    {
        ਅਧਾਰ_Type_t xReturn;
        ਕਤਾਰ_t * ਅਚਲ px_ਕਤਾਰ = x_ਕਤਾਰ;

        /* If the queue is already full we may have to block.  A critical section
         * is required to prevent an interrupt removing something from the queue
         * between the check to see if the queue is full and blocking on the queue. */
        ਪੋਰਟ_DISABLE_INTERRUPTS();
        {
            ਜੇ( prv_Is_ਕਤਾਰ_ਭਰਿਆ( px_ਕਤਾਰ ) != pdFALSE )
            {
                /* The queue is full - do we want to block or just leave without
                 * posting? */
                ਜੇ( x_Ticks_ਮੰਜ਼ਿਲ_Wait > ( TickType_t ) 0 )
                {
                    /* As this is called from a coroutine we cannot block directly, but
                     * return indicating that we need to block. */
                    v_Co_Routine_ਜੋੜੋ_ਮੰਜ਼ਿਲ_Delayed_List( x_Ticks_ਮੰਜ਼ਿਲ_Wait, &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Send ) );
                    ਪੋਰਟ_ENABLE_INTERRUPTS();
                    ਵਾਪਸ err_ਕਤਾਰ_BLOCKED;
                }
                ਨਹੀਂਤਾਂ
                {
                    ਪੋਰਟ_ENABLE_INTERRUPTS();
                    ਵਾਪਸ err_ਕਤਾਰ_ਭਰਿਆ;
                }
            }
        }
        ਪੋਰਟ_ENABLE_INTERRUPTS();

        ਪੋਰਟ_DISABLE_INTERRUPTS();
        {
            ਜੇ( px_ਕਤਾਰ->ux_Messages_ਉਡੀਕ < px_ਕਤਾਰ->ux_ਲੰਮਾਈ )
            {
                /* There is room in the queue, copy the data into the queue. */
                prv_Copy_ਅੰਕੜੇ_ਮੰਜ਼ਿਲ_ਕਤਾਰ( px_ਕਤਾਰ, pv_ਵਸਤੂ_ਮੰਜ਼ਿਲ_ਕਤਾਰ, ਕਤਾਰ_SEND_ਮੰਜ਼ਿਲ_ਪਿੱਠ );
                xReturn = pdPASS;

                /* Were any co-routines waiting for data to become available? */
                ਜੇ( list_LIST_IS_ਖਾਲੀ_ਥਾਵਾਂ( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive ) ) == pdFALSE )
                {
                    /* In this instance the co-routine could be placed directly
                     * into the ready list as we are within a critical section.
                     * Instead the same pending ready list mechanism is used as if
                     * the event were caused from within an interrupt. */
                    ਜੇ( xCoRoutineRemoveFromEventList( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive ) ) != pdFALSE )
                    {
                        /* The co-routine waiting has a higher priority so record
                         * that a yield might be appropriate. */
                        xReturn = err_ਕਤਾਰ_YIELD;
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
            }
            ਨਹੀਂਤਾਂ
            {
                xReturn = err_ਕਤਾਰ_ਭਰਿਆ;
            }
        }
        ਪੋਰਟ_ENABLE_INTERRUPTS();

        ਵਾਪਸ xReturn;
    }

#ਜੇ_ਅੰਤ /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#ਜੇ ( configUSE_CO_ROUTINES == 1 )

    ਅਧਾਰ_Type_t x_ਕਤਾਰ_CRReceive( ਕਤਾਰ_Handle_t x_ਕਤਾਰ,
                                ਖਾਲੀ * pv_ਬਫਰ,
                                TickType_t x_Ticks_ਮੰਜ਼ਿਲ_Wait )
    {
        ਅਧਾਰ_Type_t xReturn;
        ਕਤਾਰ_t * ਅਚਲ px_ਕਤਾਰ = x_ਕਤਾਰ;

        /* If the queue is already empty we may have to block.  A critical section
         * is required to prevent an interrupt adding something to the queue
         * between the check to see if the queue is empty and blocking on the queue. */
        ਪੋਰਟ_DISABLE_INTERRUPTS();
        {
            ਜੇ( px_ਕਤਾਰ->ux_Messages_ਉਡੀਕ == ( UBaseType_t ) 0 )
            {
                /* There are no messages in the queue, do we want to block or just
                 * leave with nothing? */
                ਜੇ( x_Ticks_ਮੰਜ਼ਿਲ_Wait > ( TickType_t ) 0 )
                {
                    /* As this is a co-routine we cannot block directly, but return
                     * indicating that we need to block. */
                    v_Co_Routine_ਜੋੜੋ_ਮੰਜ਼ਿਲ_Delayed_List( x_Ticks_ਮੰਜ਼ਿਲ_Wait, &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive ) );
                    ਪੋਰਟ_ENABLE_INTERRUPTS();
                    ਵਾਪਸ err_ਕਤਾਰ_BLOCKED;
                }
                ਨਹੀਂਤਾਂ
                {
                    ਪੋਰਟ_ENABLE_INTERRUPTS();
                    ਵਾਪਸ err_ਕਤਾਰ_ਭਰਿਆ;
                }
            }
            ਨਹੀਂਤਾਂ
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        ਪੋਰਟ_ENABLE_INTERRUPTS();

        ਪੋਰਟ_DISABLE_INTERRUPTS();
        {
            ਜੇ( px_ਕਤਾਰ->ux_Messages_ਉਡੀਕ > ( UBaseType_t ) 0 )
            {
                /* Data is available from the queue. */
                px_ਕਤਾਰ->u.x_ਕਤਾਰ.pcReadFrom += px_ਕਤਾਰ->ux_ਵਸਤੂ_ਮਾਪ;

                ਜੇ( px_ਕਤਾਰ->u.x_ਕਤਾਰ.pcReadFrom >= px_ਕਤਾਰ->u.x_ਕਤਾਰ.pc_ਪੂਛ )
                {
                    px_ਕਤਾਰ->u.x_ਕਤਾਰ.pcReadFrom = px_ਕਤਾਰ->pc_ਸਿਰ;
                }
                ਨਹੀਂਤਾਂ
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                --( px_ਕਤਾਰ->ux_Messages_ਉਡੀਕ );
                ( ਖਾਲੀ ) ਸਮ੍ਰਿਤੀ_ਨਕਲ( ( ਖਾਲੀ * ) pv_ਬਫਰ, ( ਖਾਲੀ * ) px_ਕਤਾਰ->u.x_ਕਤਾਰ.pcReadFrom, ( ਬਿਨਾਨਿਸ਼ਾਨੀ ) px_ਕਤਾਰ->ux_ਵਸਤੂ_ਮਾਪ );

                xReturn = pdPASS;

                /* Were any co-routines waiting for space to become available? */
                ਜੇ( list_LIST_IS_ਖਾਲੀ_ਥਾਵਾਂ( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Send ) ) == pdFALSE )
                {
                    /* In this instance the co-routine could be placed directly
                     * into the ready list as we are within a critical section.
                     * Instead the same pending ready list mechanism is used as if
                     * the event were caused from within an interrupt. */
                    ਜੇ( xCoRoutineRemoveFromEventList( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Send ) ) != pdFALSE )
                    {
                        xReturn = err_ਕਤਾਰ_YIELD;
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
            }
            ਨਹੀਂਤਾਂ
            {
                xReturn = pdFAIL;
            }
        }
        ਪੋਰਟ_ENABLE_INTERRUPTS();

        ਵਾਪਸ xReturn;
    }

#ਜੇ_ਅੰਤ /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#ਜੇ ( configUSE_CO_ROUTINES == 1 )

    ਅਧਾਰ_Type_t x_ਕਤਾਰ_CRSend_From_ISR( ਕਤਾਰ_Handle_t x_ਕਤਾਰ,
                                    ਅਚਲ ਖਾਲੀ * pv_ਵਸਤੂ_ਮੰਜ਼ਿਲ_ਕਤਾਰ,
                                    ਅਧਾਰ_Type_t xCoRoutinePreviouslyWoken )
    {
        ਕਤਾਰ_t * ਅਚਲ px_ਕਤਾਰ = x_ਕਤਾਰ;

        /* Cannot block within an ISR so if there is no space on the queue then
         * exit without doing anything. */
        ਜੇ( px_ਕਤਾਰ->ux_Messages_ਉਡੀਕ < px_ਕਤਾਰ->ux_ਲੰਮਾਈ )
        {
            prv_Copy_ਅੰਕੜੇ_ਮੰਜ਼ਿਲ_ਕਤਾਰ( px_ਕਤਾਰ, pv_ਵਸਤੂ_ਮੰਜ਼ਿਲ_ਕਤਾਰ, ਕਤਾਰ_SEND_ਮੰਜ਼ਿਲ_ਪਿੱਠ );

            /* We only want to wake one co-routine per ISR, so check that a
             * co-routine has not already been woken. */
            ਜੇ( xCoRoutinePreviouslyWoken == pdFALSE )
            {
                ਜੇ( list_LIST_IS_ਖਾਲੀ_ਥਾਵਾਂ( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive ) ) == pdFALSE )
                {
                    ਜੇ( xCoRoutineRemoveFromEventList( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive ) ) != pdFALSE )
                    {
                        ਵਾਪਸ pdTRUE;
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

        ਵਾਪਸ xCoRoutinePreviouslyWoken;
    }

#ਜੇ_ਅੰਤ /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#ਜੇ ( configUSE_CO_ROUTINES == 1 )

    ਅਧਾਰ_Type_t x_ਕਤਾਰ_CRReceive_From_ISR( ਕਤਾਰ_Handle_t x_ਕਤਾਰ,
                                       ਖਾਲੀ * pv_ਬਫਰ,
                                       ਅਧਾਰ_Type_t * pxCoRoutineWoken )
    {
        ਅਧਾਰ_Type_t xReturn;
        ਕਤਾਰ_t * ਅਚਲ px_ਕਤਾਰ = x_ਕਤਾਰ;

        /* We cannot block from an ISR, so check there is data available. If
         * not then just leave without doing anything. */
        ਜੇ( px_ਕਤਾਰ->ux_Messages_ਉਡੀਕ > ( UBaseType_t ) 0 )
        {
            /* Copy the data from the queue. */
            px_ਕਤਾਰ->u.x_ਕਤਾਰ.pcReadFrom += px_ਕਤਾਰ->ux_ਵਸਤੂ_ਮਾਪ;

            ਜੇ( px_ਕਤਾਰ->u.x_ਕਤਾਰ.pcReadFrom >= px_ਕਤਾਰ->u.x_ਕਤਾਰ.pc_ਪੂਛ )
            {
                px_ਕਤਾਰ->u.x_ਕਤਾਰ.pcReadFrom = px_ਕਤਾਰ->pc_ਸਿਰ;
            }
            ਨਹੀਂਤਾਂ
            {
                mtCOVERAGE_TEST_MARKER();
            }

            --( px_ਕਤਾਰ->ux_Messages_ਉਡੀਕ );
            ( ਖਾਲੀ ) ਸਮ੍ਰਿਤੀ_ਨਕਲ( ( ਖਾਲੀ * ) pv_ਬਫਰ, ( ਖਾਲੀ * ) px_ਕਤਾਰ->u.x_ਕਤਾਰ.pcReadFrom, ( ਬਿਨਾਨਿਸ਼ਾਨੀ ) px_ਕਤਾਰ->ux_ਵਸਤੂ_ਮਾਪ );

            ਜੇ( ( *pxCoRoutineWoken ) == pdFALSE )
            {
                ਜੇ( list_LIST_IS_ਖਾਲੀ_ਥਾਵਾਂ( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Send ) ) == pdFALSE )
                {
                    ਜੇ( xCoRoutineRemoveFromEventList( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Send ) ) != pdFALSE )
                    {
                        *pxCoRoutineWoken = pdTRUE;
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
            }
            ਨਹੀਂਤਾਂ
            {
                mtCOVERAGE_TEST_MARKER();
            }

            xReturn = pdPASS;
        }
        ਨਹੀਂਤਾਂ
        {
            xReturn = pdFAIL;
        }

        ਵਾਪਸ xReturn;
    }

#ਜੇ_ਅੰਤ /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#ਜੇ ( config_ਕਤਾਰ_REGISTRY_ਮਾਪ > 0 )

    ਖਾਲੀ v_ਕਤਾਰ_ਜੋੜੋ_ਮੰਜ਼ਿਲ_Registry( ਕਤਾਰ_Handle_t x_ਕਤਾਰ,
                              ਅਚਲ ਅੱਖਰ * pc_ਕਤਾਰ_Name ) /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
    {
        UBaseType_t ux;
        ਕਤਾਰ_Registry_ਵਸਤੂ_t * px_ਇੰਦਰਾਜ_ਮੰਜ਼ਿਲ_Write = ਨਿਰਾ;

        configASSERT( x_ਕਤਾਰ );

        /* For SMP, we need to take the queue registry lock in case another
         * core updates the register simultaneously. */
        prvENTER_CRITICAL_SMP_ONLY( &x_ਕਤਾਰ_Registry_ਤਾਲਾ );
        {
            ਜੇ( pc_ਕਤਾਰ_Name != ਨਿਰਾ )
            {
                /* See if there is an empty space in the registry.  A NULL name denotes
                 * a free slot. */
                ਲਈ( ux = ( UBaseType_t ) 0U; ux < ( UBaseType_t ) config_ਕਤਾਰ_REGISTRY_ਮਾਪ; ux++ )
                {
                    /* Replace an existing entry if the queue is already in the registry. */
                    ਜੇ( x_ਕਤਾਰ == x_ਕਤਾਰ_Registry[ ux ].xHandle )
                    {
                        px_ਇੰਦਰਾਜ_ਮੰਜ਼ਿਲ_Write = &( x_ਕਤਾਰ_Registry[ ux ] );
                        ਰੋਕੋ;
                    }
                    /* Otherwise, store in the next empty location */
                    ਨਹੀਂਤਾਂ ਜੇ( ( px_ਇੰਦਰਾਜ_ਮੰਜ਼ਿਲ_Write == ਨਿਰਾ ) && ( x_ਕਤਾਰ_Registry[ ux ].pc_ਕਤਾਰ_Name == ਨਿਰਾ ) )
                    {
                        px_ਇੰਦਰਾਜ_ਮੰਜ਼ਿਲ_Write = &( x_ਕਤਾਰ_Registry[ ux ] );
                    }
                    ਨਹੀਂਤਾਂ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
            }

            ਜੇ( px_ਇੰਦਰਾਜ_ਮੰਜ਼ਿਲ_Write != ਨਿਰਾ )
            {
                /* Store the information on this queue. */
                px_ਇੰਦਰਾਜ_ਮੰਜ਼ਿਲ_Write->pc_ਕਤਾਰ_Name = pc_ਕਤਾਰ_Name;
                px_ਇੰਦਰਾਜ_ਮੰਜ਼ਿਲ_Write->xHandle = x_ਕਤਾਰ;

                trace_ਕਤਾਰ_REGISTRY_ਜੋੜੋ( x_ਕਤਾਰ, pc_ਕਤਾਰ_Name );
            }
        }
        /* Release the previously taken queue registry lock. */
        prvEXIT_CRITICAL_SMP_ONLY( &x_ਕਤਾਰ_Registry_ਤਾਲਾ );
    }

#ਜੇ_ਅੰਤ /* configQUEUE_REGISTRY_SIZE */
/*-----------------------------------------------------------*/

#ਜੇ ( config_ਕਤਾਰ_REGISTRY_ਮਾਪ > 0 )

    ਅਚਲ ਅੱਖਰ * pc_ਕਤਾਰ_Get_Name( ਕਤਾਰ_Handle_t x_ਕਤਾਰ ) /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
    {
        UBaseType_t ux;
        ਅਚਲ ਅੱਖਰ * pcReturn = ਨਿਰਾ; /*lint !e971 Unqualified char types are allowed for strings and single characters only. */

        configASSERT( x_ਕਤਾਰ );

        /* For SMP, we need to take the queue registry lock in case another
         * core updates the register simultaneously. */
        prvENTER_CRITICAL_SMP_ONLY( &x_ਕਤਾਰ_Registry_ਤਾਲਾ );
        {
            /* Note there is nothing here to protect against another task adding or
             * removing entries from the registry while it is being searched. */

            ਲਈ( ux = ( UBaseType_t ) 0U; ux < ( UBaseType_t ) config_ਕਤਾਰ_REGISTRY_ਮਾਪ; ux++ )
            {
                ਜੇ( x_ਕਤਾਰ_Registry[ ux ].xHandle == x_ਕਤਾਰ )
                {
                    pcReturn = x_ਕਤਾਰ_Registry[ ux ].pc_ਕਤਾਰ_Name;
                    ਰੋਕੋ;
                }
                ਨਹੀਂਤਾਂ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
        }
        /* Release the previously taken queue registry lock. */
        prvEXIT_CRITICAL_SMP_ONLY( &x_ਕਤਾਰ_Registry_ਤਾਲਾ );

        ਵਾਪਸ pcReturn;
    } /*lint !e818 xQueue cannot be a pointer to const because it is a typedef. */

#ਜੇ_ਅੰਤ /* configQUEUE_REGISTRY_SIZE */
/*-----------------------------------------------------------*/

#ਜੇ ( config_ਕਤਾਰ_REGISTRY_ਮਾਪ > 0 )

    ਖਾਲੀ v_ਕਤਾਰ_Unregister_ਕਤਾਰ( ਕਤਾਰ_Handle_t x_ਕਤਾਰ )
    {
        UBaseType_t ux;

        configASSERT( x_ਕਤਾਰ );

        /* For SMP, we need to take the queue registry lock in case another
         * core updates the register simultaneously. */
        prvENTER_CRITICAL_SMP_ONLY( &x_ਕਤਾਰ_Registry_ਤਾਲਾ );
        {
            /* See if the handle of the queue being unregistered in actually in the
             * registry. */
            ਲਈ( ux = ( UBaseType_t ) 0U; ux < ( UBaseType_t ) config_ਕਤਾਰ_REGISTRY_ਮਾਪ; ux++ )
            {
                ਜੇ( x_ਕਤਾਰ_Registry[ ux ].xHandle == x_ਕਤਾਰ )
                {
                    /* Set the name to NULL to show that this slot if free again. */
                    x_ਕਤਾਰ_Registry[ ux ].pc_ਕਤਾਰ_Name = ਨਿਰਾ;

                    /* Set the handle to NULL to ensure the same queue handle cannot
                     * appear in the registry twice if it is added, removed, then
                     * added again. */
                    x_ਕਤਾਰ_Registry[ ux ].xHandle = ( ਕਤਾਰ_Handle_t ) 0;
                    ਰੋਕੋ;
                }
                ਨਹੀਂਤਾਂ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
        }
        /* Release the previously taken queue registry lock. */
        prvEXIT_CRITICAL_SMP_ONLY( &x_ਕਤਾਰ_Registry_ਤਾਲਾ );
    } /*lint !e818 xQueue could not be pointer to const because it is a typedef. */

#ਜੇ_ਅੰਤ /* configQUEUE_REGISTRY_SIZE */
/*-----------------------------------------------------------*/

#ਜੇ ( configUSE_TIMERS == 1 )

    ਖਾਲੀ v_ਕਤਾਰ_Wait_For_ਸੁਨੇਹਾ_Restricted( ਕਤਾਰ_Handle_t x_ਕਤਾਰ,
                                         TickType_t x_Ticks_ਮੰਜ਼ਿਲ_Wait,
                                         ਅਚਲ ਅਧਾਰ_Type_t xWaitIndefinitely )
    {
        ਕਤਾਰ_t * ਅਚਲ px_ਕਤਾਰ = x_ਕਤਾਰ;

        /* This function should not be called by application code hence the
         * 'Restricted' in its name.  It is not part of the public API.  It is
         * designed for use by kernel code, and has special calling requirements.
         * It can result in vListInsert() being called on a list that can only
         * possibly ever have one item in it, so the list will be fast, but even
         * so it should be called with the scheduler locked and not from a critical
         * section. */

        /* For SMP, we need to take the queue's xQueueLock as we are about to
         * access the queue. */
        prvENTER_CRITICAL_SMP_ONLY( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ) );
        {
            #ਜੇ ( ਕਤਾਰ_USE_LOCKS == 1 )
            {
                /* Only do anything if there are no messages in the queue.  This function
                 *  will not actually cause the task to block, just place it on a blocked
                 *  list.  It will not block until the scheduler is unlocked - at which
                 *  time a yield will be performed.  If an item is added to the queue while
                 *  the queue is locked, and the calling task blocks on the queue, then the
                 *  calling task will be immediately unblocked when the queue is unlocked. */
                prv_ਤਾਲਾ_ਕਤਾਰ( px_ਕਤਾਰ );
            }
            #ਜੇ_ਅੰਤ /* queueUSE_LOCKS == 1 */

            ਜੇ( px_ਕਤਾਰ->ux_Messages_ਉਡੀਕ == ( UBaseType_t ) 0U )
            {
                /* There is nothing in the queue, block for the specified period. */
                v_Task_ਰੱਖੋ_On_Event_List_Restricted( &( px_ਕਤਾਰ->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive ), x_Ticks_ਮੰਜ਼ਿਲ_Wait, xWaitIndefinitely );
            }
            ਨਹੀਂਤਾਂ
            {
                mtCOVERAGE_TEST_MARKER();
            }

            #ਜੇ ( ਕਤਾਰ_USE_LOCKS == 1 )
            {
                prv_Unlock_ਕਤਾਰ( px_ਕਤਾਰ );
            }
            #ਜੇ_ਅੰਤ /* queueUSE_LOCKS == 1 */
        }
        /* Release the previously taken xQueueLock. */
        prvEXIT_CRITICAL_SMP_ONLY( &( px_ਕਤਾਰ->x_ਕਤਾਰ_ਤਾਲਾ ) );
    }

#ਜੇ_ਅੰਤ /* configUSE_TIMERS */
/*-----------------------------------------------------------*/

#ਜੇ ( ( config_USE_ਕਤਾਰ_SETS == 1 ) && ( config_SUPPORT_DYNAMIC_ਵੰਡ == 1 ) )

    ਕਤਾਰ_ਸੈੱਟ_Handle_t x_ਕਤਾਰ_Create_ਸੈੱਟ( ਅਚਲ UBaseType_t ux_Event_ਕਤਾਰ_ਲੰਮਾਈ )
    {
        ਕਤਾਰ_ਸੈੱਟ_Handle_t px_ਕਤਾਰ;

        px_ਕਤਾਰ = x_ਕਤਾਰ_Generic_Create( ux_Event_ਕਤਾਰ_ਲੰਮਾਈ, ( UBaseType_t ) ਆਕਾਰ( ਕਤਾਰ_t * ), ਕਤਾਰ_ਕਤਾਰ_TYPE_ਸੈੱਟ );

        ਵਾਪਸ px_ਕਤਾਰ;
    }

#ਜੇ_ਅੰਤ /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#ਜੇ ( config_USE_ਕਤਾਰ_SETS == 1 )

    ਅਧਾਰ_Type_t x_ਕਤਾਰ_ਜੋੜੋ_ਮੰਜ਼ਿਲ_ਸੈੱਟ( ਕਤਾਰ_ਸੈੱਟ_Member_Handle_t x_ਕਤਾਰ_Or_Semaphore,
                               ਕਤਾਰ_ਸੈੱਟ_Handle_t x_ਕਤਾਰ_ਸੈੱਟ )
    {
        ਅਧਾਰ_Type_t xReturn;

        taskENTER_CRITICAL( &( ( ( ਕਤਾਰ_t * ) x_ਕਤਾਰ_Or_Semaphore )->x_ਕਤਾਰ_ਤਾਲਾ ) );
        {
            ਜੇ( ( ( ਕਤਾਰ_t * ) x_ਕਤਾਰ_Or_Semaphore )->px_ਕਤਾਰ_ਸੈੱਟ_Container != ਨਿਰਾ )
            {
                /* Cannot add a queue/semaphore to more than one queue set. */
                xReturn = pdFAIL;
            }
            ਨਹੀਂਤਾਂ ਜੇ( ( ( ਕਤਾਰ_t * ) x_ਕਤਾਰ_Or_Semaphore )->ux_Messages_ਉਡੀਕ != ( UBaseType_t ) 0 )
            {
                /* Cannot add a queue/semaphore to a queue set if there are already
                 * items in the queue/semaphore. */
                xReturn = pdFAIL;
            }
            ਨਹੀਂਤਾਂ
            {
                ( ( ਕਤਾਰ_t * ) x_ਕਤਾਰ_Or_Semaphore )->px_ਕਤਾਰ_ਸੈੱਟ_Container = x_ਕਤਾਰ_ਸੈੱਟ;
                xReturn = pdPASS;
            }
        }
        taskEXIT_CRITICAL( &( ( ( ਕਤਾਰ_t * ) x_ਕਤਾਰ_Or_Semaphore )->x_ਕਤਾਰ_ਤਾਲਾ ) );

        ਵਾਪਸ xReturn;
    }

#ਜੇ_ਅੰਤ /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#ਜੇ ( config_USE_ਕਤਾਰ_SETS == 1 )

    ਅਧਾਰ_Type_t x_ਕਤਾਰ_Remove_From_ਸੈੱਟ( ਕਤਾਰ_ਸੈੱਟ_Member_Handle_t x_ਕਤਾਰ_Or_Semaphore,
                                    ਕਤਾਰ_ਸੈੱਟ_Handle_t x_ਕਤਾਰ_ਸੈੱਟ )
    {
        ਅਧਾਰ_Type_t xReturn;
        ਕਤਾਰ_t * ਅਚਲ px_ਕਤਾਰ_Or_Semaphore = ( ਕਤਾਰ_t * ) x_ਕਤਾਰ_Or_Semaphore;

        ਜੇ( px_ਕਤਾਰ_Or_Semaphore->px_ਕਤਾਰ_ਸੈੱਟ_Container != x_ਕਤਾਰ_ਸੈੱਟ )
        {
            /* The queue was not a member of the set. */
            xReturn = pdFAIL;
        }
        ਨਹੀਂਤਾਂ ਜੇ( px_ਕਤਾਰ_Or_Semaphore->ux_Messages_ਉਡੀਕ != ( UBaseType_t ) 0 )
        {
            /* It is dangerous to remove a queue from a set when the queue is
             * not empty because the queue set will still hold pending events for
             * the queue. */
            xReturn = pdFAIL;
        }
        ਨਹੀਂਤਾਂ
        {
            taskENTER_CRITICAL( &( ( ( ਕਤਾਰ_t * ) px_ਕਤਾਰ_Or_Semaphore )->x_ਕਤਾਰ_ਤਾਲਾ ) );
            {
                /* The queue is no longer contained in the set. */
                px_ਕਤਾਰ_Or_Semaphore->px_ਕਤਾਰ_ਸੈੱਟ_Container = ਨਿਰਾ;
            }
            taskEXIT_CRITICAL( &( ( ( ਕਤਾਰ_t * ) px_ਕਤਾਰ_Or_Semaphore )->x_ਕਤਾਰ_ਤਾਲਾ ) );
            xReturn = pdPASS;
        }

        ਵਾਪਸ xReturn;
    } /*lint !e818 xQueueSet could not be declared as pointing to const as it is a typedef. */

#ਜੇ_ਅੰਤ /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#ਜੇ ( config_USE_ਕਤਾਰ_SETS == 1 )

    ਕਤਾਰ_ਸੈੱਟ_Member_Handle_t x_ਕਤਾਰ_Select_From_ਸੈੱਟ( ਕਤਾਰ_ਸੈੱਟ_Handle_t x_ਕਤਾਰ_ਸੈੱਟ,
                                                TickType_t ਅਚਲ x_Ticks_ਮੰਜ਼ਿਲ_Wait )
    {
        ਕਤਾਰ_ਸੈੱਟ_Member_Handle_t xReturn = ਨਿਰਾ;

        ( ਖਾਲੀ ) x_ਕਤਾਰ_Receive( ( ਕਤਾਰ_Handle_t ) x_ਕਤਾਰ_ਸੈੱਟ, &xReturn, x_Ticks_ਮੰਜ਼ਿਲ_Wait ); /*lint !e961 Casting from one typedef to another is not redundant. */
        ਵਾਪਸ xReturn;
    }

#ਜੇ_ਅੰਤ /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#ਜੇ ( config_USE_ਕਤਾਰ_SETS == 1 )

    ਕਤਾਰ_ਸੈੱਟ_Member_Handle_t x_ਕਤਾਰ_Select_From_ਸੈੱਟ_From_ISR( ਕਤਾਰ_ਸੈੱਟ_Handle_t x_ਕਤਾਰ_ਸੈੱਟ )
    {
        ਕਤਾਰ_ਸੈੱਟ_Member_Handle_t xReturn = ਨਿਰਾ;

        ( ਖਾਲੀ ) x_ਕਤਾਰ_Receive_From_ISR( ( ਕਤਾਰ_Handle_t ) x_ਕਤਾਰ_ਸੈੱਟ, &xReturn, ਨਿਰਾ ); /*lint !e961 Casting from one typedef to another is not redundant. */
        ਵਾਪਸ xReturn;
    }

#ਜੇ_ਅੰਤ /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#ਜੇ ( config_USE_ਕਤਾਰ_SETS == 1 )

    ਸਥਿਰ ਅਧਾਰ_Type_t prv_Notify_ਕਤਾਰ_ਸੈੱਟ_Container( ਅਚਲ ਕਤਾਰ_t * ਅਚਲ px_ਕਤਾਰ )
    {
        ਕਤਾਰ_t * px_ਕਤਾਰ_ਸੈੱਟ_Container = px_ਕਤਾਰ->px_ਕਤਾਰ_ਸੈੱਟ_Container;
        ਅਧਾਰ_Type_t xReturn = pdFALSE;

        /* This function must be called form a critical section. */

        /* The following line is not reachable in unit tests because every call
         * to prvNotifyQueueSetContainer is preceded by a check that
         * pxQueueSetContainer != NULL */
        configASSERT( px_ਕਤਾਰ_ਸੈੱਟ_Container ); /* LCOV_EXCL_BR_LINE */
        configASSERT( px_ਕਤਾਰ_ਸੈੱਟ_Container->ux_Messages_ਉਡੀਕ < px_ਕਤਾਰ_ਸੈੱਟ_Container->ux_ਲੰਮਾਈ );

        /* In SMP, queue sets have their own xQueueLock. Thus we need to also
         * acquire the queue set's xQueueLock before accessing it. */
        prv_ENTER_CRITICAL_ਸੁਰੱਖਿਅਤ_SMP_ONLY( &( px_ਕਤਾਰ_ਸੈੱਟ_Container->x_ਕਤਾਰ_ਤਾਲਾ ) );
        {
            ਜੇ( px_ਕਤਾਰ_ਸੈੱਟ_Container->ux_Messages_ਉਡੀਕ < px_ਕਤਾਰ_ਸੈੱਟ_Container->ux_ਲੰਮਾਈ )
            {
                #ਜੇ ( ਕਤਾਰ_USE_LOCKS == 1 )
                    ਅਚਲ int8_t c_Tx_ਤਾਲਾ = px_ਕਤਾਰ_ਸੈੱਟ_Container->c_Tx_ਤਾਲਾ;
                #ਨਹੀਂਤਾਂ
                    /* Queue locks not used, so we treat it as unlocked. */
                    ਅਚਲ int8_t c_Tx_ਤਾਲਾ = ਕਤਾਰ_UNLOCKED;
                #ਜੇ_ਅੰਤ /* queueUSE_LOCKS == 1 */

                trace_ਕਤਾਰ_ਸੈੱਟ_SEND( px_ਕਤਾਰ_ਸੈੱਟ_Container );

                /* The data copied is the handle of the queue that contains data. */
                xReturn = prv_Copy_ਅੰਕੜੇ_ਮੰਜ਼ਿਲ_ਕਤਾਰ( px_ਕਤਾਰ_ਸੈੱਟ_Container, &px_ਕਤਾਰ, ਕਤਾਰ_SEND_ਮੰਜ਼ਿਲ_ਪਿੱਠ );

                ਜੇ( c_Tx_ਤਾਲਾ == ਕਤਾਰ_UNLOCKED )
                {
                    ਜੇ( list_LIST_IS_ਖਾਲੀ_ਥਾਵਾਂ( &( px_ਕਤਾਰ_ਸੈੱਟ_Container->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive ) ) == pdFALSE )
                    {
                        ਜੇ( xTaskRemoveFromEventList( &( px_ਕਤਾਰ_ਸੈੱਟ_Container->x_Tasks_ਉਡੀਕ_ਮੰਜ਼ਿਲ_Receive ) ) != pdFALSE )
                        {
                            /* The task waiting has a higher priority. */
                            xReturn = pdTRUE;
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
                }
                ਨਹੀਂਤਾਂ
                {
                    #ਜੇ ( ਕਤਾਰ_USE_LOCKS == 1 )
                    {
                        prv_Increment_ਕਤਾਰ_Tx_ਤਾਲਾ( px_ਕਤਾਰ_ਸੈੱਟ_Container, c_Tx_ਤਾਲਾ );
                    }
                    #ਜੇ_ਅੰਤ /* queueUSE_LOCKS == 1 */
                }
            }
            ਨਹੀਂਤਾਂ
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        /* Release the previously acquired queue set's xQueueLock. */
        prv_EXIT_CRITICAL_ਸੁਰੱਖਿਅਤ_SMP_ONLY( &( px_ਕਤਾਰ_ਸੈੱਟ_Container->x_ਕਤਾਰ_ਤਾਲਾ ) );

        ਵਾਪਸ xReturn;
    }

#ਜੇ_ਅੰਤ /* configUSE_QUEUE_SETS */
