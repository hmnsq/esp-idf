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

#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <string.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#ವ್ಯಾಖ್ಯಾನಿಸು MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#ಅನ್ತರ್ಭಾವ "FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "task.h"
#ಅನ್ತರ್ಭಾವ "queue.h"
/* Include private IDF API additions for critical thread safety macros */
#ಅನ್ತರ್ಭಾವ "esp_private/freertos_idf_additions_priv.h"

#ಆದರೆ ( configUSE_CO_ROUTINES == 1 )
    #ಅನ್ತರ್ಭಾವ "croutine.h"
#ಆದರೆ_ಅಂತ್ಯ

/* Lint e9021, e961 and e750 are suppressed as a MISRA exception justified
 * because the MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು MPU_WRAPPERS_INCLUDED_FROM_API_FILE /*lint !e961 !e750 !e9021. */

/* Single core FreeRTOS uses queue locks to ensure that vTaskPlaceOnEventList()
 * calls are deterministic (as queue locks use scheduler suspension instead of
 * critical sections). However, the SMP implementation is non-deterministic
 * anyways, thus SMP can forego the use of queue locks (replaced with a critical
 * sections) in exchange for better queue performance. */
#ಆದರೆ ( config_ಸಂಖ್ಯೆ_ದ_CORES > 1 )
    #ವ್ಯಾಖ್ಯಾನಿಸು ಸರತಿ_USE_LOCKS            0
    #ವ್ಯಾಖ್ಯಾನಿಸು ಸರತಿ_UNLOCKED             ( ( int8_t ) 0 )
#ಇಲ್ಲದಿದ್ದರೆ /* configNUMBER_OF_CORES > 1 */
    #ವ್ಯಾಖ್ಯಾನಿಸು ಸರತಿ_USE_LOCKS            1
    /* Constants used with the cRxLock and cTxLock structure members. */
    #ವ್ಯಾಖ್ಯಾನಿಸು ಸರತಿ_UNLOCKED             ( ( int8_t ) -1 )
    #ವ್ಯಾಖ್ಯಾನಿಸು ಸರತಿ_LOCKED_UNMODIFIED    ( ( int8_t ) 0 )
    #ವ್ಯಾಖ್ಯಾನಿಸು ಸರತಿ_INT8_MAX             ( ( int8_t ) 127 )
#ಆದರೆ_ಅಂತ್ಯ /* configNUMBER_OF_CORES > 1 */

/* When the Queue_t structure is used to represent a base queue its pcHead and
 * pcTail members are used as pointers into the queue storage area.  When the
 * Queue_t structure is used to represent a mutex pcHead and pcTail pointers are
 * not necessary, and the pcHead pointer is set to NULL to indicate that the
 * structure instead holds a pointer to the mutex holder (if any).  Map alternative
 * names to the pcHead and structure member to ensure the readability of the code
 * is maintained.  The QueuePointers_t and SemaphoreData_t types are used to form
 * a union as their usage is mutually exclusive dependent on what the queue is
 * being used for. */
#ವ್ಯಾಖ್ಯಾನಿಸು ux_ಸರತಿ_Type            pc_ತಲೆ
#ವ್ಯಾಖ್ಯಾನಿಸು ಸರತಿ_ಸರತಿ_IS_MUTEX    ಖಾಲಿ

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ಸರತಿ_ಸೂಚಕಗಳು
{
    int8_t * pc_ಬಾಲ;     /*< Points to the byte at the end of the queue storage area.  Once more byte is allocated than necessary to store the queue items, this is used as a marker. */
    int8_t * pcReadFrom; /*< Points to the last place that a queued item was read from when the structure is used as a queue. */
} ಸರತಿ_ಸೂಚಕಗಳು_t;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ Semaphore_ದತ್ತಾಂಶ
{
    TaskHandle_t xMutexHolder;        /*< The handle of the task that holds the mutex. */
    UBaseType_t ux_Recursive_Call_ಎಣಿಕೆ; /*< Maintains a count of the number of times a recursive mutex has been recursively 'taken' when the structure is used as a mutex. */
} Semaphore_ದತ್ತಾಂಶ_t;

/* Semaphores do not actually store or copy data, so have an item size of
 * zero. */
#ವ್ಯಾಖ್ಯಾನಿಸು ಸರತಿ_SEMAPHORE_ಸರತಿ_ಅಂಶ_ಉದ್ದಳತೆ    ( ( UBaseType_t ) 0 )
#ವ್ಯಾಖ್ಯಾನಿಸು ಸರತಿ_MUTEX_GIVE_ಖಂಡ_ಸಮಯ          ( ( TickType_t ) 0U )

#ಆದರೆ ( configUSE_PREEMPTION == 0 )

/* If the cooperative scheduler is being used then a yield should not be
 * performed just because a higher priority task has been woken. */
    #ವ್ಯಾಖ್ಯಾನಿಸು ಸರತಿ_YIELD_IF_USING_PREEMPTION()
#ಇಲ್ಲದಿದ್ದರೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು ಸರತಿ_YIELD_IF_USING_PREEMPTION()    ಪೋರ್ಟ್_YIELD_WITHIN_API()
#ಆದರೆ_ಅಂತ್ಯ

/*
 * Definition of the queue used by the scheduler.
 * Items are queued by copy, not reference.  See the following link for the
 * rationale: https://www.FreeRTOS.org/Embedded-RTOS-Queues.html
 */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ಸರತಿ_Definition /* The old naming convention is used to prevent breaking kernel aware debuggers. */
{
    int8_t * pc_ತಲೆ;           /*< Points to the beginning of the queue storage area. */
    int8_t * pc_Write_ಗಮ್ಯ;        /*< Points to the free next place in the storage area. */

    ಒಕ್ಕೂಟ
    {
        ಸರತಿ_ಸೂಚಕಗಳು_t x_ಸರತಿ;     /*< Data required exclusively when this structure is used as a queue. */
        Semaphore_ದತ್ತಾಂಶ_t xSemaphore; /*< Data required exclusively when this structure is used as a semaphore. */
    } u;

    List_t x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Send;             /*< List of tasks that are blocked waiting to post onto this queue.  Stored in priority order. */
    List_t x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive;          /*< List of tasks that are blocked waiting to read from this queue.  Stored in priority order. */

    ಅಸ್ಥಿರ UBaseType_t ux_Messages_ಕಾಯುವಿಕೆ; /*< The number of items currently in the queue. */
    UBaseType_t ux_ಉದ್ದಳತೆ;                   /*< The length of the queue defined as the number of items it will hold, not the number of bytes. */
    UBaseType_t ux_ಅಂಶ_ಗಾತ್ರ;                 /*< The size of each items that the queue will hold. */

    #ಆದರೆ ( ಸರತಿ_USE_LOCKS == 1 )
        ಅಸ್ಥಿರ int8_t c_Rx_ಬೀಗ; /*< Stores the number of items received from the queue (removed from the queue) while the queue was locked.  Set to queueUNLOCKED when the queue is not locked. */
        ಅಸ್ಥಿರ int8_t c_Tx_ಬೀಗ; /*< Stores the number of items transmitted to the queue (added to the queue) while the queue was locked.  Set to queueUNLOCKED when the queue is not locked. */
    #ಆದರೆ_ಅಂತ್ಯ /* queueUSE_LOCKS == 1 */

    #ಆದರೆ ( ( config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 1 ) && ( config_SUPPORT_DYNAMIC_ಹಂಚಿಕೆ == 1 ) )
        uint8_t ucStaticallyAllocated; /*< Set to pdTRUE if the memory used by the queue was statically allocated to ensure no attempt is made to free the memory. */
    #ಆದರೆ_ಅಂತ್ಯ

    #ಆದರೆ ( config_USE_ಸರತಿ_SETS == 1 )
        ರಚನೆ ಸರತಿ_Definition * px_ಸರತಿ_ಗಣ_Container;
    #ಆದರೆ_ಅಂತ್ಯ

    #ಆದರೆ ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_ಸರತಿ_ಸಂಖ್ಯೆ;
        uint8_t uc_ಸರತಿ_Type;
    #ಆದರೆ_ಅಂತ್ಯ

    ಪೋರ್ಟ್_MUX_TYPE x_ಸರತಿ_ಬೀಗ; /* Spinlock required for SMP critical sections */
} x_ಸರತಿ_2;

/* The old xQUEUE name is maintained above then typedefed to the new Queue_t
 * name below to enable the use of older kernel aware debuggers. */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ x_ಸರತಿ_2 ಸರತಿ_t;

/*-----------------------------------------------------------*/

/*
 * The queue registry is just a means for kernel aware debuggers to locate
 * queue structures.  It has no other purpose so is an optional component.
 */
#ಆದರೆ ( config_ಸರತಿ_REGISTRY_ಗಾತ್ರ > 0 )

/* The type stored within the queue registry array.  This allows a name
 * to be assigned to each queue making kernel aware debugging a little
 * more user friendly. */
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ಸರತಿ_REGISTRY_ಅಂಶ
    {
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * pc_ಸರತಿ_Name; /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
        ಸರತಿ_Handle_t xHandle;
    } x_ಸರತಿ_Registry_ಅಂಶ;

/* The old xQueueRegistryItem name is maintained above then typedefed to the
 * new xQueueRegistryItem name below to enable the use of older kernel aware
 * debuggers. */
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ x_ಸರತಿ_Registry_ಅಂಶ ಸರತಿ_Registry_ಅಂಶ_t;

/* The queue registry is simply an array of QueueRegistryItem_t structures.
 * The pcQueueName member of a structure being NULL is indicative of the
 * array position being vacant. */
    PRIVILEGED_ದತ್ತಾಂಶ ಸರತಿ_Registry_ಅಂಶ_t x_ಸರತಿ_Registry[ config_ಸರತಿ_REGISTRY_ಗಾತ್ರ ];

    #ಆದರೆ ( config_ಸಂಖ್ಯೆ_ದ_CORES > 1 )
/* Spinlock required in SMP when accessing the queue registry */
        ಸ್ಥಿರ ಪೋರ್ಟ್_MUX_TYPE x_ಸರತಿ_Registry_ಬೀಗ = ಪೋರ್ಟ್_MUX_INITIALIZER_UNLOCKED;
    #ಆದರೆ_ಅಂತ್ಯ /* configNUMBER_OF_CORES > 1 */

#ಆದರೆ_ಅಂತ್ಯ /* configQUEUE_REGISTRY_SIZE */

#ಆದರೆ ( ಸರತಿ_USE_LOCKS == 1 )

/*
 * Unlocks a queue locked by a call to prvLockQueue.  Locking a queue does not
 * prevent an ISR from adding or removing items to the queue, but does prevent
 * an ISR from removing tasks from the queue event lists.  If an ISR finds a
 * queue is locked it will instead increment the appropriate queue lock count
 * to indicate that a task may require unblocking.  When the queue in unlocked
 * these lock counts are inspected, and the appropriate action taken.
 */
    ಸ್ಥಿರ ಶೂನ್ಯ prv_Unlock_ಸರತಿ( ಸರತಿ_t * ಸ್ಥಿರಾಂಕ px_ಸರತಿ ) PRIVILEGED_FUNCTION;

/*
 * Uses a critical section to determine if there is any data in a queue.
 *
 * @return pdTRUE if the queue contains no items, otherwise pdFALSE.
 */
    ಸ್ಥಿರ ತಳ_Type_t prv_Is_ಸರತಿ_ಖಾಲಿಗಳು( ಸ್ಥಿರಾಂಕ ಸರತಿ_t * px_ಸರತಿ ) PRIVILEGED_FUNCTION;

/*
 * Uses a critical section to determine if there is any space in a queue.
 *
 * @return pdTRUE if there is no space, otherwise pdFALSE;
 */
    ಸ್ಥಿರ ತಳ_Type_t prv_Is_ಸರತಿ_ತುಂಬಿದ( ಸ್ಥಿರಾಂಕ ಸರತಿ_t * px_ಸರತಿ ) PRIVILEGED_FUNCTION;
#ಆದರೆ_ಅಂತ್ಯ /* queueUSE_LOCKS == 1 */

/*
 * Copies an item into the queue, either at the front of the queue or the
 * back of the queue.
 */
ಸ್ಥಿರ ತಳ_Type_t prv_Copy_ದತ್ತಾಂಶ_ಗಮ್ಯ_ಸರತಿ( ಸರತಿ_t * ಸ್ಥಿರಾಂಕ px_ಸರತಿ,
                                      ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ * pv_ಅಂಶ_ಗಮ್ಯ_ಸರತಿ,
                                      ಸ್ಥಿರಾಂಕ ತಳ_Type_t xPosition ) PRIVILEGED_FUNCTION;

/*
 * Copies an item out of a queue.
 */
ಸ್ಥಿರ ಶೂನ್ಯ prv_Copy_ದತ್ತಾಂಶ_From_ಸರತಿ( ಸರತಿ_t * ಸ್ಥಿರಾಂಕ px_ಸರತಿ,
                                  ಶೂನ್ಯ * ಸ್ಥಿರಾಂಕ pv_ಬಫರ್ ) PRIVILEGED_FUNCTION;

#ಆದರೆ ( config_USE_ಸರತಿ_SETS == 1 )

/*
 * Checks to see if a queue is a member of a queue set, and if so, notifies
 * the queue set that the queue contains data.
 */
    ಸ್ಥಿರ ತಳ_Type_t prv_Notify_ಸರತಿ_ಗಣ_Container( ಸ್ಥಿರಾಂಕ ಸರತಿ_t * ಸ್ಥಿರಾಂಕ px_ಸರತಿ ) PRIVILEGED_FUNCTION;
#ಆದರೆ_ಅಂತ್ಯ

/*
 * Called after a Queue_t structure has been allocated either statically or
 * dynamically to fill in the structure's members.
 */
ಸ್ಥಿರ ಶೂನ್ಯ prv_Initialise_New_ಸರತಿ( ಸ್ಥಿರಾಂಕ UBaseType_t ux_ಸರತಿ_ಉದ್ದಳತೆ,
                                   ಸ್ಥಿರಾಂಕ UBaseType_t ux_ಅಂಶ_ಗಾತ್ರ,
                                   uint8_t * puc_ಸರತಿ_Storage,
                                   ಸ್ಥಿರಾಂಕ uint8_t uc_ಸರತಿ_Type,
                                   ಸರತಿ_t * px_New_ಸರತಿ ) PRIVILEGED_FUNCTION;

/*
 * Mutexes are a special type of queue.  When a mutex is created, first the
 * queue is created, then prvInitialiseMutex() is called to configure the queue
 * as a mutex.
 */
#ಆದರೆ ( configUSE_MUTEXES == 1 )
    ಸ್ಥಿರ ಶೂನ್ಯ prvInitialiseMutex( ಸರತಿ_t * px_New_ಸರತಿ ) PRIVILEGED_FUNCTION;
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ ( configUSE_MUTEXES == 1 )

/*
 * If a task waiting for a mutex causes the mutex holder to inherit a
 * priority, but the waiting task times out, then the holder should
 * disinherit the priority - but only down to the highest priority of any
 * other tasks that are waiting for the same mutex.  This function returns
 * that priority.
 */
    ಸ್ಥಿರ UBaseType_t prv_Get_Disinherit_ಆದ್ಯತೆ_After_Timeout( ಸ್ಥಿರಾಂಕ ಸರತಿ_t * ಸ್ಥಿರಾಂಕ px_ಸರತಿ ) PRIVILEGED_FUNCTION;
#ಆದರೆ_ಅಂತ್ಯ
/*-----------------------------------------------------------*/

#ಆದರೆ ( ಸರತಿ_USE_LOCKS == 1 )

/*
 * Macro to mark a queue as locked.  Locking a queue prevents an ISR from
 * accessing the queue event lists.
 */
    #ವ್ಯಾಖ್ಯಾನಿಸು prv_ಬೀಗ_ಸರತಿ( px_ಸರತಿ )                        \
    taskENTER_CRITICAL( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ) );        \
    {                                                      \
        ಆದರೆ( ( px_ಸರತಿ )->c_Rx_ಬೀಗ == ಸರತಿ_UNLOCKED )        \
        {                                                  \
            ( px_ಸರತಿ )->c_Rx_ಬೀಗ = ಸರತಿ_LOCKED_UNMODIFIED; \
        }                                                  \
        ಆದರೆ( ( px_ಸರತಿ )->c_Tx_ಬೀಗ == ಸರತಿ_UNLOCKED )        \
        {                                                  \
            ( px_ಸರತಿ )->c_Tx_ಬೀಗ = ಸರತಿ_LOCKED_UNMODIFIED; \
        }                                                  \
    }                                                      \
    taskEXIT_CRITICAL( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ) )

/*
 * Macro to increment cTxLock member of the queue data structure. It is
 * capped at the number of tasks in the system as we cannot unblock more
 * tasks than the number of tasks in the system.
 */
    #ವ್ಯಾಖ್ಯಾನಿಸು prv_Increment_ಸರತಿ_Tx_ಬೀಗ( px_ಸರತಿ, c_Tx_ಬೀಗ )                       \
    {                                                                         \
        ಸ್ಥಿರಾಂಕ UBaseType_t ux_ಸಂಖ್ಯೆ_ದ_Tasks = ux_Task_Get_ಸಂಖ್ಯೆ_ದ_Tasks();         \
        ಆದರೆ( ( UBaseType_t ) ( c_Tx_ಬೀಗ ) < ux_ಸಂಖ್ಯೆ_ದ_Tasks )                   \
        {                                                                     \
            configASSERT( ( c_Tx_ಬೀಗ ) != ಸರತಿ_INT8_MAX );                     \
            ( px_ಸರತಿ )->c_Tx_ಬೀಗ = ( int8_t ) ( ( c_Tx_ಬೀಗ ) + ( int8_t ) 1 ); \
        }                                                                     \
    }

/*
 * Macro to increment cRxLock member of the queue data structure. It is
 * capped at the number of tasks in the system as we cannot unblock more
 * tasks than the number of tasks in the system.
 */
    #ವ್ಯಾಖ್ಯಾನಿಸು prv_Increment_ಸರತಿ_Rx_ಬೀಗ( px_ಸರತಿ, c_Rx_ಬೀಗ )                       \
    {                                                                         \
        ಸ್ಥಿರಾಂಕ UBaseType_t ux_ಸಂಖ್ಯೆ_ದ_Tasks = ux_Task_Get_ಸಂಖ್ಯೆ_ದ_Tasks();         \
        ಆದರೆ( ( UBaseType_t ) ( c_Rx_ಬೀಗ ) < ux_ಸಂಖ್ಯೆ_ದ_Tasks )                   \
        {                                                                     \
            configASSERT( ( c_Rx_ಬೀಗ ) != ಸರತಿ_INT8_MAX );                     \
            ( px_ಸರತಿ )->c_Rx_ಬೀಗ = ( int8_t ) ( ( c_Rx_ಬೀಗ ) + ( int8_t ) 1 ); \
        }                                                                     \
    }
#ಆದರೆ_ಅಂತ್ಯ /* queueUSE_LOCKS == 1 */
/*-----------------------------------------------------------*/

ತಳ_Type_t x_ಸರತಿ_Generic_Reset( ಸರತಿ_Handle_t x_ಸರತಿ,
                               ತಳ_Type_t x_New_ಸರತಿ )
{
    ತಳ_Type_t xReturn = pdPASS;
    ಸರತಿ_t * ಸ್ಥಿರಾಂಕ px_ಸರತಿ = x_ಸರತಿ;

    configASSERT( px_ಸರತಿ );

    ಆದರೆ( x_New_ಸರತಿ == pdTRUE )
    {
        ಪೋರ್ಟ್_MUX_INITIALIZE( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ) );
    }

    ಆದರೆ( ( px_ಸರತಿ != ಖಾಲಿ ) &&
        ( px_ಸರತಿ->ux_ಉದ್ದಳತೆ >= 1U ) &&
        /* Check for multiplication overflow. */
        ( ( SIZE_MAX / px_ಸರತಿ->ux_ಉದ್ದಳತೆ ) >= px_ಸರತಿ->ux_ಅಂಶ_ಗಾತ್ರ ) )
    {
        taskENTER_CRITICAL( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ) );
        {
            px_ಸರತಿ->u.x_ಸರತಿ.pc_ಬಾಲ = px_ಸರತಿ->pc_ತಲೆ + ( px_ಸರತಿ->ux_ಉದ್ದಳತೆ * px_ಸರತಿ->ux_ಅಂಶ_ಗಾತ್ರ ); /*lint !e9016 Pointer arithmetic allowed on char types, especially when it assists conveying intent. */
            px_ಸರತಿ->ux_Messages_ಕಾಯುವಿಕೆ = ( UBaseType_t ) 0U;
            px_ಸರತಿ->pc_Write_ಗಮ್ಯ = px_ಸರತಿ->pc_ತಲೆ;
            px_ಸರತಿ->u.x_ಸರತಿ.pcReadFrom = px_ಸರತಿ->pc_ತಲೆ + ( ( px_ಸರತಿ->ux_ಉದ್ದಳತೆ - 1U ) * px_ಸರತಿ->ux_ಅಂಶ_ಗಾತ್ರ ); /*lint !e9016 Pointer arithmetic allowed on char types, especially when it assists conveying intent. */
            #ಆದರೆ ( ಸರತಿ_USE_LOCKS == 1 )
            {
                px_ಸರತಿ->c_Rx_ಬೀಗ = ಸರತಿ_UNLOCKED;
                px_ಸರತಿ->c_Tx_ಬೀಗ = ಸರತಿ_UNLOCKED;
            }
            #ಆದರೆ_ಅಂತ್ಯ /* queueUSE_LOCKS == 1 */

            ಆದರೆ( x_New_ಸರತಿ == pdFALSE )
            {
                /* If there are tasks blocked waiting to read from the queue, then
                 * the tasks will remain blocked as after this function exits the queue
                 * will still be empty.  If there are tasks blocked waiting to write to
                 * the queue, then one should be unblocked as after this function exits
                 * it will be possible to write to it. */
                ಆದರೆ( list_LIST_IS_ಖಾಲಿಗಳು( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Send ) ) == pdFALSE )
                {
                    ಆದರೆ( xTaskRemoveFromEventList( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Send ) ) != pdFALSE )
                    {
                        ಸರತಿ_YIELD_IF_USING_PREEMPTION();
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
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                /* Ensure the event queues start in the correct state. */
                vListInitialise( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Send ) );
                vListInitialise( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive ) );
            }
        }
        taskEXIT_CRITICAL( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ) );
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        xReturn = pdFAIL;
    }

    configASSERT( xReturn != pdFAIL );

    /* A value is returned for calling semantic consistency with previous
     * versions. */
    ಹಿಂತಿರುಗಿ xReturn;
}
/*-----------------------------------------------------------*/

#ಆದರೆ ( config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 1 )

    ಸರತಿ_Handle_t x_ಸರತಿ_Generic_Create_Static( ಸ್ಥಿರಾಂಕ UBaseType_t ux_ಸರತಿ_ಉದ್ದಳತೆ,
                                             ಸ್ಥಿರಾಂಕ UBaseType_t ux_ಅಂಶ_ಗಾತ್ರ,
                                             uint8_t * puc_ಸರತಿ_Storage,
                                             Static_ಸರತಿ_t * px_Static_ಸರತಿ,
                                             ಸ್ಥಿರಾಂಕ uint8_t uc_ಸರತಿ_Type )
    {
        ಸರತಿ_t * px_New_ಸರತಿ = ಖಾಲಿ;

        /* The StaticQueue_t structure and the queue storage area must be
         * supplied. */
        configASSERT( px_Static_ಸರತಿ );

        ಆದರೆ( ( ux_ಸರತಿ_ಉದ್ದಳತೆ > ( UBaseType_t ) 0 ) &&
            ( px_Static_ಸರತಿ != ಖಾಲಿ ) &&

            /* A queue storage area should be provided if the item size is not 0, and
             * should not be provided if the item size is 0. */
            ( !( ( puc_ಸರತಿ_Storage != ಖಾಲಿ ) && ( ux_ಅಂಶ_ಗಾತ್ರ == 0 ) ) ) &&
            ( !( ( puc_ಸರತಿ_Storage == ಖಾಲಿ ) && ( ux_ಅಂಶ_ಗಾತ್ರ != 0 ) ) ) )
        {
            #ಆದರೆ ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticQueue_t or StaticSemaphore_t equals the size of
                 * the real queue and semaphore structures. */
                ಅಸ್ಥಿರ size_t x_ಗಾತ್ರ = ಗಾತ್ರ( Static_ಸರತಿ_t );

                /* This assertion cannot be branch covered in unit tests */
                configASSERT( x_ಗಾತ್ರ == ಗಾತ್ರ( ಸರತಿ_t ) ); /* LCOV_EXCL_BR_LINE */
                ( ಶೂನ್ಯ ) x_ಗಾತ್ರ;                             /* Keeps lint quiet when configASSERT() is not defined. */
            }
            #ಆದರೆ_ಅಂತ್ಯ /* configASSERT_DEFINED */

            /* The address of a statically allocated queue was passed in, use it.
             * The address of a statically allocated storage area was also passed in
             * but is already set. */
            px_New_ಸರತಿ = ( ಸರತಿ_t * ) px_Static_ಸರತಿ; /*lint !e740 !e9087 Unusual cast is ok as the structures are designed to have the same alignment, and the size is checked by an assert. */

            #ಆದರೆ ( config_SUPPORT_DYNAMIC_ಹಂಚಿಕೆ == 1 )
            {
                /* Queues can be allocated wither statically or dynamically, so
                 * note this queue was allocated statically in case the queue is
                 * later deleted. */
                px_New_ಸರತಿ->ucStaticallyAllocated = pdTRUE;
            }
            #ಆದರೆ_ಅಂತ್ಯ /* configSUPPORT_DYNAMIC_ALLOCATION */

            prv_Initialise_New_ಸರತಿ( ux_ಸರತಿ_ಉದ್ದಳತೆ, ux_ಅಂಶ_ಗಾತ್ರ, puc_ಸರತಿ_Storage, uc_ಸರತಿ_Type, px_New_ಸರತಿ );
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            configASSERT( px_New_ಸರತಿ );
            mtCOVERAGE_TEST_MARKER();
        }

        ಹಿಂತಿರುಗಿ px_New_ಸರತಿ;
    }

#ಆದರೆ_ಅಂತ್ಯ /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

#ಆದರೆ ( config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 1 )

    ತಳ_Type_t x_ಸರತಿ_Generic_Get_Static_Buffers( ಸರತಿ_Handle_t x_ಸರತಿ,
                                              uint8_t ** ppuc_ಸರತಿ_Storage,
                                              Static_ಸರತಿ_t ** ppx_Static_ಸರತಿ )
    {
        ತಳ_Type_t xReturn;
        ಸರತಿ_t * ಸ್ಥಿರಾಂಕ px_ಸರತಿ = x_ಸರತಿ;

        configASSERT( px_ಸರತಿ );
        configASSERT( ppx_Static_ಸರತಿ );

        #ಆದರೆ ( config_SUPPORT_DYNAMIC_ಹಂಚಿಕೆ == 1 )
        {
            /* Check if the queue was statically allocated. */
            ಆದರೆ( px_ಸರತಿ->ucStaticallyAllocated == ( uint8_t ) pdTRUE )
            {
                ಆದರೆ( ppuc_ಸರತಿ_Storage != ಖಾಲಿ )
                {
                    *ppuc_ಸರತಿ_Storage = ( uint8_t * ) px_ಸರತಿ->pc_ತಲೆ;
                }

                *ppx_Static_ಸರತಿ = ( Static_ಸರತಿ_t * ) px_ಸರತಿ;
                xReturn = pdTRUE;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                xReturn = pdFALSE;
            }
        }
        #ಇಲ್ಲದಿದ್ದರೆ /* configSUPPORT_DYNAMIC_ALLOCATION */
        {
            /* Queue must have been statically allocated. */
            ಆದರೆ( ppuc_ಸರತಿ_Storage != ಖಾಲಿ )
            {
                *ppuc_ಸರತಿ_Storage = ( uint8_t * ) px_ಸರತಿ->pc_ತಲೆ;
            }

            *ppx_Static_ಸರತಿ = ( Static_ಸರತಿ_t * ) px_ಸರತಿ;
            xReturn = pdTRUE;
        }
        #ಆದರೆ_ಅಂತ್ಯ /* configSUPPORT_DYNAMIC_ALLOCATION */

        ಹಿಂತಿರುಗಿ xReturn;
    }

#ಆದರೆ_ಅಂತ್ಯ /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

#ಆದರೆ ( config_SUPPORT_DYNAMIC_ಹಂಚಿಕೆ == 1 )

    ಸರತಿ_Handle_t x_ಸರತಿ_Generic_Create( ಸ್ಥಿರಾಂಕ UBaseType_t ux_ಸರತಿ_ಉದ್ದಳತೆ,
                                       ಸ್ಥಿರಾಂಕ UBaseType_t ux_ಅಂಶ_ಗಾತ್ರ,
                                       ಸ್ಥಿರಾಂಕ uint8_t uc_ಸರತಿ_Type )
    {
        ಸರತಿ_t * px_New_ಸರತಿ = ಖಾಲಿ;
        size_t x_ಸರತಿ_ಗಾತ್ರ_ಒಳ_ಬೈಟುಗಳು;
        uint8_t * puc_ಸರತಿ_Storage;

        ಆದರೆ( ( ux_ಸರತಿ_ಉದ್ದಳತೆ > ( UBaseType_t ) 0 ) &&
            /* Check for multiplication overflow. */
            ( ( SIZE_MAX / ux_ಸರತಿ_ಉದ್ದಳತೆ ) >= ux_ಅಂಶ_ಗಾತ್ರ ) &&
            /* Check for addition overflow. */
            ( ( SIZE_MAX - ಗಾತ್ರ( ಸರತಿ_t ) ) >= ( ux_ಸರತಿ_ಉದ್ದಳತೆ * ux_ಅಂಶ_ಗಾತ್ರ ) ) )
        {
            /* Allocate enough space to hold the maximum number of items that
             * can be in the queue at any time.  It is valid for uxItemSize to be
             * zero in the case the queue is used as a semaphore. */
            x_ಸರತಿ_ಗಾತ್ರ_ಒಳ_ಬೈಟುಗಳು = ( size_t ) ( ux_ಸರತಿ_ಉದ್ದಳತೆ * ux_ಅಂಶ_ಗಾತ್ರ ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */

            /* Allocate the queue and storage area.  Justification for MISRA
             * deviation as follows:  pvPortMalloc() always ensures returned memory
             * blocks are aligned per the requirements of the MCU stack.  In this case
             * pvPortMalloc() must return a pointer that is guaranteed to meet the
             * alignment requirements of the Queue_t structure - which in this case
             * is an int8_t *.  Therefore, whenever the stack alignment requirements
             * are greater than or equal to the pointer to char requirements the cast
             * is safe.  In other cases alignment requirements are not strict (one or
             * two bytes). */
            px_New_ಸರತಿ = ( ಸರತಿ_t * ) pv_ಪೋರ್ಟ್_Malloc( ಗಾತ್ರ( ಸರತಿ_t ) + x_ಸರತಿ_ಗಾತ್ರ_ಒಳ_ಬೈಟುಗಳು ); /*lint !e9087 !e9079 see comment above. */

            ಆದರೆ( px_New_ಸರತಿ != ಖಾಲಿ )
            {
                /* Jump past the queue structure to find the location of the queue
                 * storage area. */
                puc_ಸರತಿ_Storage = ( uint8_t * ) px_New_ಸರತಿ;
                puc_ಸರತಿ_Storage += ಗಾತ್ರ( ಸರತಿ_t ); /*lint !e9016 Pointer arithmetic allowed on char types, especially when it assists conveying intent. */

                #ಆದರೆ ( config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 1 )
                {
                    /* Queues can be created either statically or dynamically, so
                     * note this task was created dynamically in case it is later
                     * deleted. */
                    px_New_ಸರತಿ->ucStaticallyAllocated = pdFALSE;
                }
                #ಆದರೆ_ಅಂತ್ಯ /* configSUPPORT_STATIC_ALLOCATION */

                prv_Initialise_New_ಸರತಿ( ux_ಸರತಿ_ಉದ್ದಳತೆ, ux_ಅಂಶ_ಗಾತ್ರ, puc_ಸರತಿ_Storage, uc_ಸರತಿ_Type, px_New_ಸರತಿ );
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                trace_ಸರತಿ_CREATE_FAILED( uc_ಸರತಿ_Type );
                mtCOVERAGE_TEST_MARKER();
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            configASSERT( px_New_ಸರತಿ );
            mtCOVERAGE_TEST_MARKER();
        }

        ಹಿಂತಿರುಗಿ px_New_ಸರತಿ;
    }

#ಆದರೆ_ಅಂತ್ಯ /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ prv_Initialise_New_ಸರತಿ( ಸ್ಥಿರಾಂಕ UBaseType_t ux_ಸರತಿ_ಉದ್ದಳತೆ,
                                   ಸ್ಥಿರಾಂಕ UBaseType_t ux_ಅಂಶ_ಗಾತ್ರ,
                                   uint8_t * puc_ಸರತಿ_Storage,
                                   ಸ್ಥಿರಾಂಕ uint8_t uc_ಸರತಿ_Type,
                                   ಸರತಿ_t * px_New_ಸರತಿ )
{
    /* Remove compiler warnings about unused parameters should
     * configUSE_TRACE_FACILITY not be set to 1. */
    ( ಶೂನ್ಯ ) uc_ಸರತಿ_Type;

    ಆದರೆ( ux_ಅಂಶ_ಗಾತ್ರ == ( UBaseType_t ) 0 )
    {
        /* No RAM was allocated for the queue storage area, but PC head cannot
         * be set to NULL because NULL is used as a key to say the queue is used as
         * a mutex.  Therefore just set pcHead to point to the queue as a benign
         * value that is known to be within the memory map. */
        px_New_ಸರತಿ->pc_ತಲೆ = ( int8_t * ) px_New_ಸರತಿ;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        /* Set the head to the start of the queue storage area. */
        px_New_ಸರತಿ->pc_ತಲೆ = ( int8_t * ) puc_ಸರತಿ_Storage;
    }

    /* Initialise the queue members as described where the queue type is
     * defined. */
    px_New_ಸರತಿ->ux_ಉದ್ದಳತೆ = ux_ಸರತಿ_ಉದ್ದಳತೆ;
    px_New_ಸರತಿ->ux_ಅಂಶ_ಗಾತ್ರ = ux_ಅಂಶ_ಗಾತ್ರ;
    ( ಶೂನ್ಯ ) x_ಸರತಿ_Generic_Reset( px_New_ಸರತಿ, pdTRUE );

    #ಆದರೆ ( configUSE_TRACE_FACILITY == 1 )
    {
        px_New_ಸರತಿ->uc_ಸರತಿ_Type = uc_ಸರತಿ_Type;
    }
    #ಆದರೆ_ಅಂತ್ಯ /* configUSE_TRACE_FACILITY */

    #ಆದರೆ ( config_USE_ಸರತಿ_SETS == 1 )
    {
        px_New_ಸರತಿ->px_ಸರತಿ_ಗಣ_Container = ಖಾಲಿ;
    }
    #ಆದರೆ_ಅಂತ್ಯ /* configUSE_QUEUE_SETS */

    trace_ಸರತಿ_CREATE( px_New_ಸರತಿ );
}
/*-----------------------------------------------------------*/

#ಆದರೆ ( configUSE_MUTEXES == 1 )

    ಸ್ಥಿರ ಶೂನ್ಯ prvInitialiseMutex( ಸರತಿ_t * px_New_ಸರತಿ )
    {
        ಆದರೆ( px_New_ಸರತಿ != ಖಾಲಿ )
        {
            /* The queue create function will set all the queue structure members
            * correctly for a generic queue, but this function is creating a
            * mutex.  Overwrite those members that need to be set differently -
            * in particular the information required for priority inheritance. */
            px_New_ಸರತಿ->u.xSemaphore.xMutexHolder = ಖಾಲಿ;
            px_New_ಸರತಿ->ux_ಸರತಿ_Type = ಸರತಿ_ಸರತಿ_IS_MUTEX;

            /* In case this is a recursive mutex. */
            px_New_ಸರತಿ->u.xSemaphore.ux_Recursive_Call_ಎಣಿಕೆ = 0;

            /* Initialize the mutex's spinlock */
            ಪೋರ್ಟ್_MUX_INITIALIZE( &( px_New_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ) );

            traceCREATE_MUTEX( px_New_ಸರತಿ );

            /* Start with the semaphore in the expected state. */
            ( ಶೂನ್ಯ ) x_ಸರತಿ_Generic_Send( px_New_ಸರತಿ, ಖಾಲಿ, ( TickType_t ) 0U, ಸರತಿ_SEND_ಗಮ್ಯ_ಹಿಂಭಾಗ );
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            traceCREATE_MUTEX_FAILED();
        }
    }

#ಆದರೆ_ಅಂತ್ಯ /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

#ಆದರೆ ( ( configUSE_MUTEXES == 1 ) && ( config_SUPPORT_DYNAMIC_ಹಂಚಿಕೆ == 1 ) )

    ಸರತಿ_Handle_t x_ಸರತಿ_Create_Mutex( ಸ್ಥಿರಾಂಕ uint8_t uc_ಸರತಿ_Type )
    {
        ಸರತಿ_Handle_t x_New_ಸರತಿ;
        ಸ್ಥಿರಾಂಕ UBaseType_t ux_Mutex_ಉದ್ದಳತೆ = ( UBaseType_t ) 1, ux_Mutex_ಗಾತ್ರ = ( UBaseType_t ) 0;

        x_New_ಸರತಿ = x_ಸರತಿ_Generic_Create( ux_Mutex_ಉದ್ದಳತೆ, ux_Mutex_ಗಾತ್ರ, uc_ಸರತಿ_Type );
        prvInitialiseMutex( ( ಸರತಿ_t * ) x_New_ಸರತಿ );

        ಹಿಂತಿರುಗಿ x_New_ಸರತಿ;
    }

#ಆದರೆ_ಅಂತ್ಯ /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

#ಆದರೆ ( ( configUSE_MUTEXES == 1 ) && ( config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 1 ) )

    ಸರತಿ_Handle_t x_ಸರತಿ_Create_Mutex_Static( ಸ್ಥಿರಾಂಕ uint8_t uc_ಸರತಿ_Type,
                                           Static_ಸರತಿ_t * px_Static_ಸರತಿ )
    {
        ಸರತಿ_Handle_t x_New_ಸರತಿ;
        ಸ್ಥಿರಾಂಕ UBaseType_t ux_Mutex_ಉದ್ದಳತೆ = ( UBaseType_t ) 1, ux_Mutex_ಗಾತ್ರ = ( UBaseType_t ) 0;

        /* Prevent compiler warnings about unused parameters if
         * configUSE_TRACE_FACILITY does not equal 1. */
        ( ಶೂನ್ಯ ) uc_ಸರತಿ_Type;

        x_New_ಸರತಿ = x_ಸರತಿ_Generic_Create_Static( ux_Mutex_ಉದ್ದಳತೆ, ux_Mutex_ಗಾತ್ರ, ಖಾಲಿ, px_Static_ಸರತಿ, uc_ಸರತಿ_Type );
        prvInitialiseMutex( ( ಸರತಿ_t * ) x_New_ಸರತಿ );

        ಹಿಂತಿರುಗಿ x_New_ಸರತಿ;
    }

#ಆದರೆ_ಅಂತ್ಯ /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

#ಆದರೆ ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) )

    TaskHandle_t x_ಸರತಿ_Get_Mutex_Holder( ಸರತಿ_Handle_t xSemaphore )
    {
        TaskHandle_t pxReturn;
        ಸರತಿ_t * ಸ್ಥಿರಾಂಕ pxSemaphore = ( ಸರತಿ_t * ) xSemaphore;

        configASSERT( xSemaphore );

        /* This function is called by xSemaphoreGetMutexHolder(), and should not
         * be called directly.  Note:  This is a good way of determining if the
         * calling task is the mutex holder, but not a good way of determining the
         * identity of the mutex holder, as the holder may change between the
         * following critical section exiting and the function returning. */
        taskENTER_CRITICAL( &( pxSemaphore->x_ಸರತಿ_ಬೀಗ ) );
        {
            ಆದರೆ( pxSemaphore->ux_ಸರತಿ_Type == ಸರತಿ_ಸರತಿ_IS_MUTEX )
            {
                pxReturn = pxSemaphore->u.xSemaphore.xMutexHolder;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                pxReturn = ಖಾಲಿ;
            }
        }
        taskEXIT_CRITICAL( &( pxSemaphore->x_ಸರತಿ_ಬೀಗ ) );

        ಹಿಂತಿರುಗಿ pxReturn;
    } /*lint !e818 xSemaphore cannot be a pointer to const because it is a typedef. */

#ಆದರೆ_ಅಂತ್ಯ /* if ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) ) */
/*-----------------------------------------------------------*/

#ಆದರೆ ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) )

    TaskHandle_t x_ಸರತಿ_Get_Mutex_Holder_From_ISR( ಸರತಿ_Handle_t xSemaphore )
    {
        TaskHandle_t pxReturn;

        configASSERT( xSemaphore );

        /* Mutexes cannot be used in interrupt service routines, so the mutex
         * holder should not change in an ISR, and therefore a critical section is
         * not required here. */
        ಆದರೆ( ( ( ಸರತಿ_t * ) xSemaphore )->ux_ಸರತಿ_Type == ಸರತಿ_ಸರತಿ_IS_MUTEX )
        {
            pxReturn = ( ( ಸರತಿ_t * ) xSemaphore )->u.xSemaphore.xMutexHolder;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            pxReturn = ಖಾಲಿ;
        }

        ಹಿಂತಿರುಗಿ pxReturn;
    } /*lint !e818 xSemaphore cannot be a pointer to const because it is a typedef. */

#ಆದರೆ_ಅಂತ್ಯ /* if ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) ) */
/*-----------------------------------------------------------*/

#ಆದರೆ ( configUSE_RECURSIVE_MUTEXES == 1 )

    ತಳ_Type_t x_ಸರತಿ_Give_Mutex_Recursive( ಸರತಿ_Handle_t xMutex )
    {
        ತಳ_Type_t xReturn;
        ಸರತಿ_t * ಸ್ಥಿರಾಂಕ pxMutex = ( ಸರತಿ_t * ) xMutex;

        configASSERT( pxMutex );

        /* If this is the task that holds the mutex then xMutexHolder will not
         * change outside of this task.  If this task does not hold the mutex then
         * pxMutexHolder can never coincidentally equal the tasks handle, and as
         * this is the only condition we are interested in it does not matter if
         * pxMutexHolder is accessed simultaneously by another task.  Therefore no
         * mutual exclusion is required to test the pxMutexHolder variable. */
        ಆದರೆ( pxMutex->u.xSemaphore.xMutexHolder == xTaskGetCurrentTaskHandle() )
        {
            traceGIVE_MUTEX_RECURSIVE( pxMutex );

            /* uxRecursiveCallCount cannot be zero if xMutexHolder is equal to
             * the task handle, therefore no underflow check is required.  Also,
             * uxRecursiveCallCount is only modified by the mutex holder, and as
             * there can only be one, no mutual exclusion is required to modify the
             * uxRecursiveCallCount member. */
            ( pxMutex->u.xSemaphore.ux_Recursive_Call_ಎಣಿಕೆ )--;

            /* Has the recursive call count unwound to 0? */
            ಆದರೆ( pxMutex->u.xSemaphore.ux_Recursive_Call_ಎಣಿಕೆ == ( UBaseType_t ) 0 )
            {
                /* Return the mutex.  This will automatically unblock any other
                 * task that might be waiting to access the mutex. */
                ( ಶೂನ್ಯ ) x_ಸರತಿ_Generic_Send( pxMutex, ಖಾಲಿ, ಸರತಿ_MUTEX_GIVE_ಖಂಡ_ಸಮಯ, ಸರತಿ_SEND_ಗಮ್ಯ_ಹಿಂಭಾಗ );
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                mtCOVERAGE_TEST_MARKER();
            }

            xReturn = pdPASS;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            /* The mutex cannot be given because the calling task is not the
             * holder. */
            xReturn = pdFAIL;

            traceGIVE_MUTEX_RECURSIVE_FAILED( pxMutex );
        }

        ಹಿಂತಿರುಗಿ xReturn;
    }

#ಆದರೆ_ಅಂತ್ಯ /* configUSE_RECURSIVE_MUTEXES */
/*-----------------------------------------------------------*/

#ಆದರೆ ( configUSE_RECURSIVE_MUTEXES == 1 )

    ತಳ_Type_t x_ಸರತಿ_Take_Mutex_Recursive( ಸರತಿ_Handle_t xMutex,
                                         TickType_t x_Ticks_ಗಮ್ಯ_Wait )
    {
        ತಳ_Type_t xReturn;
        ಸರತಿ_t * ಸ್ಥಿರಾಂಕ pxMutex = ( ಸರತಿ_t * ) xMutex;

        configASSERT( pxMutex );

        /* Comments regarding mutual exclusion as per those within
         * xQueueGiveMutexRecursive(). */

        traceTAKE_MUTEX_RECURSIVE( pxMutex );

        ಆದರೆ( pxMutex->u.xSemaphore.xMutexHolder == xTaskGetCurrentTaskHandle() )
        {
            ( pxMutex->u.xSemaphore.ux_Recursive_Call_ಎಣಿಕೆ )++;
            xReturn = pdPASS;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            xReturn = x_ಸರತಿ_Semaphore_Take( pxMutex, x_Ticks_ಗಮ್ಯ_Wait );

            /* pdPASS will only be returned if the mutex was successfully
             * obtained.  The calling task may have entered the Blocked state
             * before reaching here. */
            ಆದರೆ( xReturn != pdFAIL )
            {
                ( pxMutex->u.xSemaphore.ux_Recursive_Call_ಎಣಿಕೆ )++;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                traceTAKE_MUTEX_RECURSIVE_FAILED( pxMutex );
            }
        }

        ಹಿಂತಿರುಗಿ xReturn;
    }

#ಆದರೆ_ಅಂತ್ಯ /* configUSE_RECURSIVE_MUTEXES */
/*-----------------------------------------------------------*/

#ಆದರೆ ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 1 ) )

    ಸರತಿ_Handle_t x_ಸರತಿ_Create_Counting_Semaphore_Static( ಸ್ಥಿರಾಂಕ UBaseType_t ux_Max_ಎಣಿಕೆ,
                                                       ಸ್ಥಿರಾಂಕ UBaseType_t ux_Initial_ಎಣಿಕೆ,
                                                       Static_ಸರತಿ_t * px_Static_ಸರತಿ )
    {
        ಸರತಿ_Handle_t xHandle = ಖಾಲಿ;

        ಆದರೆ( ( ux_Max_ಎಣಿಕೆ != 0 ) &&
            ( ux_Initial_ಎಣಿಕೆ <= ux_Max_ಎಣಿಕೆ ) )
        {
            xHandle = x_ಸರತಿ_Generic_Create_Static( ux_Max_ಎಣಿಕೆ, ಸರತಿ_SEMAPHORE_ಸರತಿ_ಅಂಶ_ಉದ್ದಳತೆ, ಖಾಲಿ, px_Static_ಸರತಿ, ಸರತಿ_ಸರತಿ_TYPE_COUNTING_SEMAPHORE );

            ಆದರೆ( xHandle != ಖಾಲಿ )
            {
                ( ( ಸರತಿ_t * ) xHandle )->ux_Messages_ಕಾಯುವಿಕೆ = ux_Initial_ಎಣಿಕೆ;

                traceCREATE_COUNTING_SEMAPHORE();
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                traceCREATE_COUNTING_SEMAPHORE_FAILED();
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            configASSERT( xHandle );
            mtCOVERAGE_TEST_MARKER();
        }

        ಹಿಂತಿರುಗಿ xHandle;
    }

#ಆದರೆ_ಅಂತ್ಯ /* ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) ) */
/*-----------------------------------------------------------*/

#ಆದರೆ ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( config_SUPPORT_DYNAMIC_ಹಂಚಿಕೆ == 1 ) )

    ಸರತಿ_Handle_t x_ಸರತಿ_Create_Counting_Semaphore( ಸ್ಥಿರಾಂಕ UBaseType_t ux_Max_ಎಣಿಕೆ,
                                                 ಸ್ಥಿರಾಂಕ UBaseType_t ux_Initial_ಎಣಿಕೆ )
    {
        ಸರತಿ_Handle_t xHandle = ಖಾಲಿ;

        ಆದರೆ( ( ux_Max_ಎಣಿಕೆ != 0 ) &&
            ( ux_Initial_ಎಣಿಕೆ <= ux_Max_ಎಣಿಕೆ ) )
        {
            xHandle = x_ಸರತಿ_Generic_Create( ux_Max_ಎಣಿಕೆ, ಸರತಿ_SEMAPHORE_ಸರತಿ_ಅಂಶ_ಉದ್ದಳತೆ, ಸರತಿ_ಸರತಿ_TYPE_COUNTING_SEMAPHORE );

            ಆದರೆ( xHandle != ಖಾಲಿ )
            {
                ( ( ಸರತಿ_t * ) xHandle )->ux_Messages_ಕಾಯುವಿಕೆ = ux_Initial_ಎಣಿಕೆ;

                traceCREATE_COUNTING_SEMAPHORE();
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                traceCREATE_COUNTING_SEMAPHORE_FAILED();
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            configASSERT( xHandle );
            mtCOVERAGE_TEST_MARKER();
        }

        ಹಿಂತಿರುಗಿ xHandle;
    }

#ಆದರೆ_ಅಂತ್ಯ /* ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) ) */
/*-----------------------------------------------------------*/

ತಳ_Type_t x_ಸರತಿ_Generic_Send( ಸರತಿ_Handle_t x_ಸರತಿ,
                              ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ * ಸ್ಥಿರಾಂಕ pv_ಅಂಶ_ಗಮ್ಯ_ಸರತಿ,
                              TickType_t x_Ticks_ಗಮ್ಯ_Wait,
                              ಸ್ಥಿರಾಂಕ ತಳ_Type_t xCopyPosition )
{
    ತಳ_Type_t x_ನಮೂದು_ಸಮಯ_ಗಣ = pdFALSE, xYieldRequired;
    ಸಮಯ_ಹೊರಹೋಗು_t x_ಸಮಯ_ಹೊರಹೋಗು_2;
    ಸರತಿ_t * ಸ್ಥಿರಾಂಕ px_ಸರತಿ = x_ಸರತಿ;

    configASSERT( px_ಸರತಿ );
    configASSERT( !( ( pv_ಅಂಶ_ಗಮ್ಯ_ಸರತಿ == ಖಾಲಿ ) && ( px_ಸರತಿ->ux_ಅಂಶ_ಗಾತ್ರ != ( UBaseType_t ) 0U ) ) );
    configASSERT( !( ( xCopyPosition == ಸರತಿ_OVERWRITE ) && ( px_ಸರತಿ->ux_ಉದ್ದಳತೆ != 1 ) ) );
    #ಆದರೆ ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_ಗಮ್ಯ_Wait != 0 ) ) );
    }
    #ಆದರೆ_ಅಂತ್ಯ

    /*lint -save -e904 This function relaxes the coding standard somewhat to
     * allow return statements within the function itself.  This is done in the
     * interest of execution time efficiency. */
    ಗಾಗಿ( ; ; )
    {
        taskENTER_CRITICAL( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ) );
        {
            /* Is there room on the queue now?  The running task must be the
             * highest priority task wanting to access the queue.  If the head item
             * in the queue is to be overwritten then it does not matter if the
             * queue is full. */
            ಆದರೆ( ( px_ಸರತಿ->ux_Messages_ಕಾಯುವಿಕೆ < px_ಸರತಿ->ux_ಉದ್ದಳತೆ ) || ( xCopyPosition == ಸರತಿ_OVERWRITE ) )
            {
                trace_ಸರತಿ_SEND( px_ಸರತಿ );

                #ಆದರೆ ( config_USE_ಸರತಿ_SETS == 1 )
                {
                    ಸ್ಥಿರಾಂಕ UBaseType_t ux_ಹಿಂದಿನ_Messages_ಕಾಯುವಿಕೆ = px_ಸರತಿ->ux_Messages_ಕಾಯುವಿಕೆ;

                    xYieldRequired = prv_Copy_ದತ್ತಾಂಶ_ಗಮ್ಯ_ಸರತಿ( px_ಸರತಿ, pv_ಅಂಶ_ಗಮ್ಯ_ಸರತಿ, xCopyPosition );

                    ಆದರೆ( px_ಸರತಿ->px_ಸರತಿ_ಗಣ_Container != ಖಾಲಿ )
                    {
                        ಆದರೆ( ( xCopyPosition == ಸರತಿ_OVERWRITE ) && ( ux_ಹಿಂದಿನ_Messages_ಕಾಯುವಿಕೆ != ( UBaseType_t ) 0 ) )
                        {
                            /* Do not notify the queue set as an existing item
                             * was overwritten in the queue so the number of items
                             * in the queue has not changed. */
                            mtCOVERAGE_TEST_MARKER();
                        }
                        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( prv_Notify_ಸರತಿ_ಗಣ_Container( px_ಸರತಿ ) != pdFALSE )
                        {
                            /* The queue is a member of a queue set, and posting
                             * to the queue set caused a higher priority task to
                             * unblock. A context switch is required. */
                            ಸರತಿ_YIELD_IF_USING_PREEMPTION();
                        }
                        ಇಲ್ಲದಿದ್ದರೆ
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                    {
                        /* If there was a task waiting for data to arrive on the
                         * queue then unblock it now. */
                        ಆದರೆ( list_LIST_IS_ಖಾಲಿಗಳು( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive ) ) == pdFALSE )
                        {
                            ಆದರೆ( xTaskRemoveFromEventList( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive ) ) != pdFALSE )
                            {
                                /* The unblocked task has a priority higher than
                                 * our own so yield immediately.  Yes it is ok to
                                 * do this from within the critical section - the
                                 * kernel takes care of that. */
                                ಸರತಿ_YIELD_IF_USING_PREEMPTION();
                            }
                            ಇಲ್ಲದಿದ್ದರೆ
                            {
                                mtCOVERAGE_TEST_MARKER();
                            }
                        }
                        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( xYieldRequired != pdFALSE )
                        {
                            /* This path is a special case that will only get
                             * executed if the task was holding multiple mutexes
                             * and the mutexes were given back in an order that is
                             * different to that in which they were taken. */
                            ಸರತಿ_YIELD_IF_USING_PREEMPTION();
                        }
                        ಇಲ್ಲದಿದ್ದರೆ
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                }
                #ಇಲ್ಲದಿದ್ದರೆ /* configUSE_QUEUE_SETS */
                {
                    xYieldRequired = prv_Copy_ದತ್ತಾಂಶ_ಗಮ್ಯ_ಸರತಿ( px_ಸರತಿ, pv_ಅಂಶ_ಗಮ್ಯ_ಸರತಿ, xCopyPosition );

                    /* If there was a task waiting for data to arrive on the
                     * queue then unblock it now. */
                    ಆದರೆ( list_LIST_IS_ಖಾಲಿಗಳು( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive ) ) == pdFALSE )
                    {
                        ಆದರೆ( xTaskRemoveFromEventList( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive ) ) != pdFALSE )
                        {
                            /* The unblocked task has a priority higher than
                             * our own so yield immediately.  Yes it is ok to do
                             * this from within the critical section - the kernel
                             * takes care of that. */
                            ಸರತಿ_YIELD_IF_USING_PREEMPTION();
                        }
                        ಇಲ್ಲದಿದ್ದರೆ
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( xYieldRequired != pdFALSE )
                    {
                        /* This path is a special case that will only get
                         * executed if the task was holding multiple mutexes and
                         * the mutexes were given back in an order that is
                         * different to that in which they were taken. */
                        ಸರತಿ_YIELD_IF_USING_PREEMPTION();
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #ಆದರೆ_ಅಂತ್ಯ /* configUSE_QUEUE_SETS */

                taskEXIT_CRITICAL( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ) );
                ಹಿಂತಿರುಗಿ pdPASS;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ಆದರೆ( x_Ticks_ಗಮ್ಯ_Wait == ( TickType_t ) 0 )
                {
                    /* The queue was full and no block time is specified (or
                     * the block time has expired) so leave now. */
                    taskEXIT_CRITICAL( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ) );

                    /* Return to the original privilege level before exiting
                     * the function. */
                    trace_ಸರತಿ_SEND_FAILED( px_ಸರತಿ );
                    ಹಿಂತಿರುಗಿ err_ಸರತಿ_ತುಂಬಿದ;
                }
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( x_ನಮೂದು_ಸಮಯ_ಗಣ == pdFALSE )
                {
                    /* The queue was full and a block time was specified so
                     * configure the timeout structure. */
                    v_Task_ಆಂತರಿಕ_ಗಣ_ಸಮಯ_ಹೊರಹೋಗು_State( &x_ಸಮಯ_ಹೊರಹೋಗು_2 );
                    x_ನಮೂದು_ಸಮಯ_ಗಣ = pdTRUE;
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    /* Entry time was already set. */
                    mtCOVERAGE_TEST_MARKER();
                }
            }

            /* If queue locks ARE NOT being used:
             * - At this point, the queue is full and entry time has been set
             * - We simply check for a time out, block if not timed out, or
             *   return an error if we have timed out. */
            #ಆದರೆ ( ಸರತಿ_USE_LOCKS == 0 )
            {
                /* Update the timeout state to see if it has expired yet. */
                ಆದರೆ( x_Task_Check_For_ಸಮಯ_ಹೊರಹೋಗು( &x_ಸಮಯ_ಹೊರಹೋಗು_2, &x_Ticks_ಗಮ್ಯ_Wait ) == pdFALSE )
                {
                    /* Not timed out yet. Block the current task. */
                    trace_BLOCKING_ON_ಸರತಿ_SEND( px_ಸರತಿ );
                    v_Task_ಇರಿಸು_On_Event_List( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Send ), x_Ticks_ಗಮ್ಯ_Wait );
                    ಪೋರ್ಟ್_YIELD_WITHIN_API();
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    /* We have timed out. Return an error. */
                    taskEXIT_CRITICAL( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ) );
                    trace_ಸರತಿ_SEND_FAILED( px_ಸರತಿ );
                    ಹಿಂತಿರುಗಿ err_ಸರತಿ_ತುಂಬಿದ;
                }
            }
            #ಆದರೆ_ಅಂತ್ಯ /* queueUSE_LOCKS == 0 */
        }
        taskEXIT_CRITICAL( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ) );

        /* If queue locks ARE being used:
         * - At this point, the queue is full and entry time has been set
         * - We follow the original procedure of locking the queue before
         *   attempting to block. */
        #ಆದರೆ ( ಸರತಿ_USE_LOCKS == 1 )
        {
            /* Interrupts and other tasks can send to and receive from the queue
             * now the critical section has been exited. */

            vTaskSuspendAll();
            prv_ಬೀಗ_ಸರತಿ( px_ಸರತಿ );

            /* Update the timeout state to see if it has expired yet. */
            ಆದರೆ( x_Task_Check_For_ಸಮಯ_ಹೊರಹೋಗು( &x_ಸಮಯ_ಹೊರಹೋಗು_2, &x_Ticks_ಗಮ್ಯ_Wait ) == pdFALSE )
            {
                ಆದರೆ( prv_Is_ಸರತಿ_ತುಂಬಿದ( px_ಸರತಿ ) != pdFALSE )
                {
                    trace_BLOCKING_ON_ಸರತಿ_SEND( px_ಸರತಿ );
                    v_Task_ಇರಿಸು_On_Event_List( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Send ), x_Ticks_ಗಮ್ಯ_Wait );

                    /* Unlocking the queue means queue events can effect the
                     * event list. It is possible that interrupts occurring now
                     * remove this task from the event list again - but as the
                     * scheduler is suspended the task will go onto the pending
                     * ready list instead of the actual ready list. */
                    prv_Unlock_ಸರತಿ( px_ಸರತಿ );

                    /* Resuming the scheduler will move tasks from the pending
                     * ready list into the ready list - so it is feasible that this
                     * task is already in the ready list before it yields - in which
                     * case the yield will not cause a context switch unless there
                     * is also a higher priority task in the pending ready list. */
                    ಆದರೆ( xTaskResumeAll() == pdFALSE )
                    {
                        ಪೋರ್ಟ್_YIELD_WITHIN_API();
                    }
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    /* Try again. */
                    prv_Unlock_ಸರತಿ( px_ಸರತಿ );
                    ( ಶೂನ್ಯ ) xTaskResumeAll();
                }
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                /* The timeout has expired. */
                prv_Unlock_ಸರತಿ( px_ಸರತಿ );
                ( ಶೂನ್ಯ ) xTaskResumeAll();

                trace_ಸರತಿ_SEND_FAILED( px_ಸರತಿ );
                ಹಿಂತಿರುಗಿ err_ಸರತಿ_ತುಂಬಿದ;
            }
        }
        #ಆದರೆ_ಅಂತ್ಯ /* queueUSE_LOCKS == 1 */
    } /*lint -restore */
}
/*-----------------------------------------------------------*/

ತಳ_Type_t x_ಸರತಿ_Generic_Send_From_ISR( ಸರತಿ_Handle_t x_ಸರತಿ,
                                     ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ * ಸ್ಥಿರಾಂಕ pv_ಅಂಶ_ಗಮ್ಯ_ಸರತಿ,
                                     ತಳ_Type_t * ಸ್ಥಿರಾಂಕ px_Higher_ಆದ್ಯತೆ_Task_Woken,
                                     ಸ್ಥಿರಾಂಕ ತಳ_Type_t xCopyPosition )
{
    ತಳ_Type_t xReturn;
    UBaseType_t ux_Saved_ಅಡಚಣೆ_ಸ್ಥಿತಿ;
    ಸರತಿ_t * ಸ್ಥಿರಾಂಕ px_ಸರತಿ = x_ಸರತಿ;

    configASSERT( px_ಸರತಿ );
    configASSERT( !( ( pv_ಅಂಶ_ಗಮ್ಯ_ಸರತಿ == ಖಾಲಿ ) && ( px_ಸರತಿ->ux_ಅಂಶ_ಗಾತ್ರ != ( UBaseType_t ) 0U ) ) );
    configASSERT( !( ( xCopyPosition == ಸರತಿ_OVERWRITE ) && ( px_ಸರತಿ->ux_ಉದ್ದಳತೆ != 1 ) ) );

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
    ಪೋರ್ಟ್_ASSERT_IF_ಅಡಚಣೆ_ಆದ್ಯತೆ_INVALID();

    /* Similar to xQueueGenericSend, except without blocking if there is no room
     * in the queue.  Also don't directly wake a task that was blocked on a queue
     * read, instead return a flag to say whether a context switch is required or
     * not (i.e. has a task with a higher priority than us been woken by this
     * post). */
    prv_ENTER_CRITICAL_OR_ಮುಖವಾಡ_ISR( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ), ux_Saved_ಅಡಚಣೆ_ಸ್ಥಿತಿ );
    {
        ಆದರೆ( ( px_ಸರತಿ->ux_Messages_ಕಾಯುವಿಕೆ < px_ಸರತಿ->ux_ಉದ್ದಳತೆ ) || ( xCopyPosition == ಸರತಿ_OVERWRITE ) )
        {
            #ಆದರೆ ( ಸರತಿ_USE_LOCKS == 1 )
                ಸ್ಥಿರಾಂಕ int8_t c_Tx_ಬೀಗ = px_ಸರತಿ->c_Tx_ಬೀಗ;
            #ಇಲ್ಲದಿದ್ದರೆ
                /* Queue locks not used, so we treat it as unlocked. */
                ಸ್ಥಿರಾಂಕ int8_t c_Tx_ಬೀಗ = ಸರತಿ_UNLOCKED;
            #ಆದರೆ_ಅಂತ್ಯ /* queueUSE_LOCKS == 1 */
            ಸ್ಥಿರಾಂಕ UBaseType_t ux_ಹಿಂದಿನ_Messages_ಕಾಯುವಿಕೆ = px_ಸರತಿ->ux_Messages_ಕಾಯುವಿಕೆ;

            trace_ಸರತಿ_SEND_FROM_ISR( px_ಸರತಿ );

            /* Semaphores use xQueueGiveFromISR(), so pxQueue will not be a
             *  semaphore or mutex.  That means prvCopyDataToQueue() cannot result
             *  in a task disinheriting a priority and prvCopyDataToQueue() can be
             *  called here even though the disinherit function does not check if
             *  the scheduler is suspended before accessing the ready lists. */
            ( ಶೂನ್ಯ ) prv_Copy_ದತ್ತಾಂಶ_ಗಮ್ಯ_ಸರತಿ( px_ಸರತಿ, pv_ಅಂಶ_ಗಮ್ಯ_ಸರತಿ, xCopyPosition );

            /* The event list is not altered if the queue is locked.  This will
             * be done when the queue is unlocked later. */
            ಆದರೆ( c_Tx_ಬೀಗ == ಸರತಿ_UNLOCKED )
            {
                #ಆದರೆ ( config_USE_ಸರತಿ_SETS == 1 )
                {
                    ಆದರೆ( px_ಸರತಿ->px_ಸರತಿ_ಗಣ_Container != ಖಾಲಿ )
                    {
                        ಆದರೆ( ( xCopyPosition == ಸರತಿ_OVERWRITE ) && ( ux_ಹಿಂದಿನ_Messages_ಕಾಯುವಿಕೆ != ( UBaseType_t ) 0 ) )
                        {
                            /* Do not notify the queue set as an existing item
                             * was overwritten in the queue so the number of items
                             * in the queue has not changed. */
                            mtCOVERAGE_TEST_MARKER();
                        }
                        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( prv_Notify_ಸರತಿ_ಗಣ_Container( px_ಸರತಿ ) != pdFALSE )
                        {
                            /* The queue is a member of a queue set, and posting
                             * to the queue set caused a higher priority task to
                             * unblock.  A context switch is required. */
                            ಆದರೆ( px_Higher_ಆದ್ಯತೆ_Task_Woken != ಖಾಲಿ )
                            {
                                *px_Higher_ಆದ್ಯತೆ_Task_Woken = pdTRUE;
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
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                    {
                        ಆದರೆ( list_LIST_IS_ಖಾಲಿಗಳು( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive ) ) == pdFALSE )
                        {
                            ಆದರೆ( xTaskRemoveFromEventList( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive ) ) != pdFALSE )
                            {
                                /* The task waiting has a higher priority so
                                 *  record that a context switch is required. */
                                ಆದರೆ( px_Higher_ಆದ್ಯತೆ_Task_Woken != ಖಾಲಿ )
                                {
                                    *px_Higher_ಆದ್ಯತೆ_Task_Woken = pdTRUE;
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
                        }
                        ಇಲ್ಲದಿದ್ದರೆ
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                }
                #ಇಲ್ಲದಿದ್ದರೆ /* configUSE_QUEUE_SETS */
                {
                    ಆದರೆ( list_LIST_IS_ಖಾಲಿಗಳು( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive ) ) == pdFALSE )
                    {
                        ಆದರೆ( xTaskRemoveFromEventList( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive ) ) != pdFALSE )
                        {
                            /* The task waiting has a higher priority so record that a
                             * context switch is required. */
                            ಆದರೆ( px_Higher_ಆದ್ಯತೆ_Task_Woken != ಖಾಲಿ )
                            {
                                *px_Higher_ಆದ್ಯತೆ_Task_Woken = pdTRUE;
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
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }

                    /* Not used in this path. */
                    ( ಶೂನ್ಯ ) ux_ಹಿಂದಿನ_Messages_ಕಾಯುವಿಕೆ;
                }
                #ಆದರೆ_ಅಂತ್ಯ /* configUSE_QUEUE_SETS */
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                #ಆದರೆ ( ಸರತಿ_USE_LOCKS == 1 )
                {
                    /* Increment the lock count so the task that unlocks the queue
                     * knows that data was posted while it was locked. */
                    prv_Increment_ಸರತಿ_Tx_ಬೀಗ( px_ಸರತಿ, c_Tx_ಬೀಗ );
                }
                #ಆದರೆ_ಅಂತ್ಯ /* queueUSE_LOCKS == 1 */
            }

            xReturn = pdPASS;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            trace_ಸರತಿ_SEND_FROM_ISR_FAILED( px_ಸರತಿ );
            xReturn = err_ಸರತಿ_ತುಂಬಿದ;
        }
    }
    prvEXIT_CRITICAL_OR_UNMASK_ISR( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ), ux_Saved_ಅಡಚಣೆ_ಸ್ಥಿತಿ );

    ಹಿಂತಿರುಗಿ xReturn;
}
/*-----------------------------------------------------------*/

ತಳ_Type_t x_ಸರತಿ_Give_From_ISR( ಸರತಿ_Handle_t x_ಸರತಿ,
                              ತಳ_Type_t * ಸ್ಥಿರಾಂಕ px_Higher_ಆದ್ಯತೆ_Task_Woken )
{
    ತಳ_Type_t xReturn;
    UBaseType_t ux_Saved_ಅಡಚಣೆ_ಸ್ಥಿತಿ;
    ಸರತಿ_t * ಸ್ಥಿರಾಂಕ px_ಸರತಿ = x_ಸರತಿ;

    /* Similar to xQueueGenericSendFromISR() but used with semaphores where the
     * item size is 0.  Don't directly wake a task that was blocked on a queue
     * read, instead return a flag to say whether a context switch is required or
     * not (i.e. has a task with a higher priority than us been woken by this
     * post). */

    configASSERT( px_ಸರತಿ );

    /* xQueueGenericSendFromISR() should be used instead of xQueueGiveFromISR()
     * if the item size is not 0. */
    configASSERT( px_ಸರತಿ->ux_ಅಂಶ_ಗಾತ್ರ == 0 );

    /* Normally a mutex would not be given from an interrupt, especially if
     * there is a mutex holder, as priority inheritance makes no sense for an
     * interrupts, only tasks. */
    configASSERT( !( ( px_ಸರತಿ->ux_ಸರತಿ_Type == ಸರತಿ_ಸರತಿ_IS_MUTEX ) && ( px_ಸರತಿ->u.xSemaphore.xMutexHolder != ಖಾಲಿ ) ) );

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
    ಪೋರ್ಟ್_ASSERT_IF_ಅಡಚಣೆ_ಆದ್ಯತೆ_INVALID();

    prv_ENTER_CRITICAL_OR_ಮುಖವಾಡ_ISR( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ), ux_Saved_ಅಡಚಣೆ_ಸ್ಥಿತಿ );
    {
        ಸ್ಥಿರಾಂಕ UBaseType_t ux_Messages_ಕಾಯುವಿಕೆ = px_ಸರತಿ->ux_Messages_ಕಾಯುವಿಕೆ;

        /* When the queue is used to implement a semaphore no data is ever
         * moved through the queue but it is still valid to see if the queue 'has
         * space'. */
        ಆದರೆ( ux_Messages_ಕಾಯುವಿಕೆ < px_ಸರತಿ->ux_ಉದ್ದಳತೆ )
        {
            #ಆದರೆ ( ಸರತಿ_USE_LOCKS == 1 )
                ಸ್ಥಿರಾಂಕ int8_t c_Tx_ಬೀಗ = px_ಸರತಿ->c_Tx_ಬೀಗ;
            #ಇಲ್ಲದಿದ್ದರೆ
                /* Queue locks not used, so we treat it as unlocked. */
                ಸ್ಥಿರಾಂಕ int8_t c_Tx_ಬೀಗ = ಸರತಿ_UNLOCKED;
            #ಆದರೆ_ಅಂತ್ಯ /* queueUSE_LOCKS == 1 */

            /* Todo: Reconcile tracing differences (IDF-8183) */
            trace_ಸರತಿ_GIVE_FROM_ISR( px_ಸರತಿ );

            /* A task can only have an inherited priority if it is a mutex
             * holder - and if there is a mutex holder then the mutex cannot be
             * given from an ISR.  As this is the ISR version of the function it
             * can be assumed there is no mutex holder and no need to determine if
             * priority disinheritance is needed.  Simply increase the count of
             * messages (semaphores) available. */
            px_ಸರತಿ->ux_Messages_ಕಾಯುವಿಕೆ = ux_Messages_ಕಾಯುವಿಕೆ + ( UBaseType_t ) 1;

            /* The event list is not altered if the queue is locked.  This will
             * be done when the queue is unlocked later. */
            ಆದರೆ( c_Tx_ಬೀಗ == ಸರತಿ_UNLOCKED )
            {
                #ಆದರೆ ( config_USE_ಸರತಿ_SETS == 1 )
                {
                    ಆದರೆ( px_ಸರತಿ->px_ಸರತಿ_ಗಣ_Container != ಖಾಲಿ )
                    {
                        ಆದರೆ( prv_Notify_ಸರತಿ_ಗಣ_Container( px_ಸರತಿ ) != pdFALSE )
                        {
                            /* The semaphore is a member of a queue set, and
                             * posting to the queue set caused a higher priority
                             * task to unblock.  A context switch is required. */
                            ಆದರೆ( px_Higher_ಆದ್ಯತೆ_Task_Woken != ಖಾಲಿ )
                            {
                                *px_Higher_ಆದ್ಯತೆ_Task_Woken = pdTRUE;
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
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                    {
                        ಆದರೆ( list_LIST_IS_ಖಾಲಿಗಳು( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive ) ) == pdFALSE )
                        {
                            ಆದರೆ( xTaskRemoveFromEventList( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive ) ) != pdFALSE )
                            {
                                /* The task waiting has a higher priority so
                                 *  record that a context switch is required. */
                                ಆದರೆ( px_Higher_ಆದ್ಯತೆ_Task_Woken != ಖಾಲಿ )
                                {
                                    *px_Higher_ಆದ್ಯತೆ_Task_Woken = pdTRUE;
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
                        }
                        ಇಲ್ಲದಿದ್ದರೆ
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                }
                #ಇಲ್ಲದಿದ್ದರೆ /* configUSE_QUEUE_SETS */
                {
                    ಆದರೆ( list_LIST_IS_ಖಾಲಿಗಳು( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive ) ) == pdFALSE )
                    {
                        ಆದರೆ( xTaskRemoveFromEventList( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive ) ) != pdFALSE )
                        {
                            /* The task waiting has a higher priority so record that a
                             * context switch is required. */
                            ಆದರೆ( px_Higher_ಆದ್ಯತೆ_Task_Woken != ಖಾಲಿ )
                            {
                                *px_Higher_ಆದ್ಯತೆ_Task_Woken = pdTRUE;
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
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #ಆದರೆ_ಅಂತ್ಯ /* configUSE_QUEUE_SETS */
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                #ಆದರೆ ( ಸರತಿ_USE_LOCKS == 1 )
                {
                    /* Increment the lock count so the task that unlocks the queue
                     * knows that data was posted while it was locked. */
                    prv_Increment_ಸರತಿ_Tx_ಬೀಗ( px_ಸರತಿ, c_Tx_ಬೀಗ );
                }
                #ಆದರೆ_ಅಂತ್ಯ /* queueUSE_LOCKS == 1 */
            }

            xReturn = pdPASS;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            /* Todo: Reconcile tracing differences (IDF-8183) */
            trace_ಸರತಿ_GIVE_FROM_ISR_FAILED( px_ಸರತಿ );
            xReturn = err_ಸರತಿ_ತುಂಬಿದ;
        }
    }
    prvEXIT_CRITICAL_OR_UNMASK_ISR( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ), ux_Saved_ಅಡಚಣೆ_ಸ್ಥಿತಿ );

    ಹಿಂತಿರುಗಿ xReturn;
}
/*-----------------------------------------------------------*/

ತಳ_Type_t x_ಸರತಿ_Receive( ಸರತಿ_Handle_t x_ಸರತಿ,
                          ಶೂನ್ಯ * ಸ್ಥಿರಾಂಕ pv_ಬಫರ್,
                          TickType_t x_Ticks_ಗಮ್ಯ_Wait )
{
    ತಳ_Type_t x_ನಮೂದು_ಸಮಯ_ಗಣ = pdFALSE;
    ಸಮಯ_ಹೊರಹೋಗು_t x_ಸಮಯ_ಹೊರಹೋಗು_2;
    ಸರತಿ_t * ಸ್ಥಿರಾಂಕ px_ಸರತಿ = x_ಸರತಿ;

    /* Check the pointer is not NULL. */
    configASSERT( ( px_ಸರತಿ ) );

    /* The buffer into which data is received can only be NULL if the data size
     * is zero (so no data is copied into the buffer). */
    configASSERT( !( ( ( pv_ಬಫರ್ ) == ಖಾಲಿ ) && ( ( px_ಸರತಿ )->ux_ಅಂಶ_ಗಾತ್ರ != ( UBaseType_t ) 0U ) ) );

    /* Cannot block if the scheduler is suspended. */
    #ಆದರೆ ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_ಗಮ್ಯ_Wait != 0 ) ) );
    }
    #ಆದರೆ_ಅಂತ್ಯ

    /*lint -save -e904  This function relaxes the coding standard somewhat to
     * allow return statements within the function itself.  This is done in the
     * interest of execution time efficiency. */
    ಗಾಗಿ( ; ; )
    {
        taskENTER_CRITICAL( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ) );
        {
            ಸ್ಥಿರಾಂಕ UBaseType_t ux_Messages_ಕಾಯುವಿಕೆ = px_ಸರತಿ->ux_Messages_ಕಾಯುವಿಕೆ;

            /* Is there data in the queue now?  To be running the calling task
             * must be the highest priority task wanting to access the queue. */
            ಆದರೆ( ux_Messages_ಕಾಯುವಿಕೆ > ( UBaseType_t ) 0 )
            {
                /* Data available, remove one item. */
                prv_Copy_ದತ್ತಾಂಶ_From_ಸರತಿ( px_ಸರತಿ, pv_ಬಫರ್ );
                trace_ಸರತಿ_RECEIVE( px_ಸರತಿ );
                px_ಸರತಿ->ux_Messages_ಕಾಯುವಿಕೆ = ux_Messages_ಕಾಯುವಿಕೆ - ( UBaseType_t ) 1;

                /* There is now space in the queue, were any tasks waiting to
                 * post to the queue?  If so, unblock the highest priority waiting
                 * task. */
                ಆದರೆ( list_LIST_IS_ಖಾಲಿಗಳು( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Send ) ) == pdFALSE )
                {
                    ಆದರೆ( xTaskRemoveFromEventList( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Send ) ) != pdFALSE )
                    {
                        ಸರತಿ_YIELD_IF_USING_PREEMPTION();
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

                taskEXIT_CRITICAL( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ) );
                ಹಿಂತಿರುಗಿ pdPASS;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ಆದರೆ( x_Ticks_ಗಮ್ಯ_Wait == ( TickType_t ) 0 )
                {
                    /* The queue was empty and no block time is specified (or
                     * the block time has expired) so leave now. */
                    taskEXIT_CRITICAL( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ) );
                    trace_ಸರತಿ_RECEIVE_FAILED( px_ಸರತಿ );
                    ಹಿಂತಿರುಗಿ err_ಸರತಿ_ಖಾಲಿಗಳು;
                }
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( x_ನಮೂದು_ಸಮಯ_ಗಣ == pdFALSE )
                {
                    /* The queue was empty and a block time was specified so
                     * configure the timeout structure. */
                    v_Task_ಆಂತರಿಕ_ಗಣ_ಸಮಯ_ಹೊರಹೋಗು_State( &x_ಸಮಯ_ಹೊರಹೋಗು_2 );
                    x_ನಮೂದು_ಸಮಯ_ಗಣ = pdTRUE;
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    /* Entry time was already set. */
                    mtCOVERAGE_TEST_MARKER();
                }
            }

            /* If queue locks ARE NOT being used:
             * - At this point, the queue is empty and entry time has been set
             * - We simply check for a time out, block if not timed out, or
             *   return an error if we have timed out. */
            #ಆದರೆ ( ಸರತಿ_USE_LOCKS == 0 )
            {
                /* Update the timeout state to see if it has expired yet. */
                ಆದರೆ( x_Task_Check_For_ಸಮಯ_ಹೊರಹೋಗು( &x_ಸಮಯ_ಹೊರಹೋಗು_2, &x_Ticks_ಗಮ್ಯ_Wait ) == pdFALSE )
                {
                    /* Not timed out yet. Block the current task. */
                    trace_BLOCKING_ON_ಸರತಿ_RECEIVE( px_ಸರತಿ );
                    v_Task_ಇರಿಸು_On_Event_List( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive ), x_Ticks_ಗಮ್ಯ_Wait );
                    ಪೋರ್ಟ್_YIELD_WITHIN_API();
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    /* We have timed out. Return an error. */
                    taskEXIT_CRITICAL( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ) );
                    trace_ಸರತಿ_RECEIVE_FAILED( px_ಸರತಿ );
                    ಹಿಂತಿರುಗಿ err_ಸರತಿ_ಖಾಲಿಗಳು;
                }
            }
            #ಆದರೆ_ಅಂತ್ಯ /* queueUSE_LOCKS == 0 */
        }
        taskEXIT_CRITICAL( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ) );

        /* If queue locks ARE being used:
         * - At this point, the queue is empty and entry time has been set
         * - We follow the original procedure for locking the queue before
         *   attempting to block. */
        #ಆದರೆ ( ಸರತಿ_USE_LOCKS == 1 )
        {
            /* Interrupts and other tasks can send to and receive from the queue
             * now the critical section has been exited. */

            vTaskSuspendAll();
            prv_ಬೀಗ_ಸರತಿ( px_ಸರತಿ );

            /* Update the timeout state to see if it has expired yet. */
            ಆದರೆ( x_Task_Check_For_ಸಮಯ_ಹೊರಹೋಗು( &x_ಸಮಯ_ಹೊರಹೋಗು_2, &x_Ticks_ಗಮ್ಯ_Wait ) == pdFALSE )
            {
                /* The timeout has not expired.  If the queue is still empty place
                 * the task on the list of tasks waiting to receive from the queue. */
                ಆದರೆ( prv_Is_ಸರತಿ_ಖಾಲಿಗಳು( px_ಸರತಿ ) != pdFALSE )
                {
                    trace_BLOCKING_ON_ಸರತಿ_RECEIVE( px_ಸರತಿ );
                    v_Task_ಇರಿಸು_On_Event_List( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive ), x_Ticks_ಗಮ್ಯ_Wait );
                    prv_Unlock_ಸರತಿ( px_ಸರತಿ );

                    ಆದರೆ( xTaskResumeAll() == pdFALSE )
                    {
                        ಪೋರ್ಟ್_YIELD_WITHIN_API();
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    /* The queue contains data again.  Loop back to try and read the
                     * data. */
                    prv_Unlock_ಸರತಿ( px_ಸರತಿ );
                    ( ಶೂನ್ಯ ) xTaskResumeAll();
                }
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                /* Timed out.  If there is no data in the queue exit, otherwise loop
                 * back and attempt to read the data. */
                prv_Unlock_ಸರತಿ( px_ಸರತಿ );
                ( ಶೂನ್ಯ ) xTaskResumeAll();

                ಆದರೆ( prv_Is_ಸರತಿ_ಖಾಲಿಗಳು( px_ಸರತಿ ) != pdFALSE )
                {
                    trace_ಸರತಿ_RECEIVE_FAILED( px_ಸರತಿ );
                    ಹಿಂತಿರುಗಿ err_ಸರತಿ_ಖಾಲಿಗಳು;
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
        }
        #ಆದರೆ_ಅಂತ್ಯ /* queueUSE_LOCKS == 1 */
    } /*lint -restore */
}
/*-----------------------------------------------------------*/

ತಳ_Type_t x_ಸರತಿ_Semaphore_Take( ಸರತಿ_Handle_t x_ಸರತಿ,
                                TickType_t x_Ticks_ಗಮ್ಯ_Wait )
{
    ತಳ_Type_t x_ನಮೂದು_ಸಮಯ_ಗಣ = pdFALSE;
    ಸಮಯ_ಹೊರಹೋಗು_t x_ಸಮಯ_ಹೊರಹೋಗು_2;
    ಸರತಿ_t * ಸ್ಥಿರಾಂಕ px_ಸರತಿ = x_ಸರತಿ;

    #ಆದರೆ ( configUSE_MUTEXES == 1 )
        ತಳ_Type_t xInheritanceOccurred = pdFALSE;
    #ಆದರೆ_ಅಂತ್ಯ

    /* Check the queue pointer is not NULL. */
    configASSERT( ( px_ಸರತಿ ) );

    /* Check this really is a semaphore, in which case the item size will be
     * 0. */
    configASSERT( px_ಸರತಿ->ux_ಅಂಶ_ಗಾತ್ರ == 0 );

    /* Cannot block if the scheduler is suspended. */
    #ಆದರೆ ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_ಗಮ್ಯ_Wait != 0 ) ) );
    }
    #ಆದರೆ_ಅಂತ್ಯ

    /*lint -save -e904 This function relaxes the coding standard somewhat to allow return
     * statements within the function itself.  This is done in the interest
     * of execution time efficiency. */
    ಗಾಗಿ( ; ; )
    {
        taskENTER_CRITICAL( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ) );
        {
            /* Semaphores are queues with an item size of 0, and where the
             * number of messages in the queue is the semaphore's count value. */
            ಸ್ಥಿರಾಂಕ UBaseType_t ux_Semaphore_ಎಣಿಕೆ = px_ಸರತಿ->ux_Messages_ಕಾಯುವಿಕೆ;

            /* Is there data in the queue now?  To be running the calling task
             * must be the highest priority task wanting to access the queue. */
            ಆದರೆ( ux_Semaphore_ಎಣಿಕೆ > ( UBaseType_t ) 0 )
            {
                /* Todo: Reconcile tracing differences (IDF-8183) */
                trace_ಸರತಿ_SEMAPHORE_RECEIVE( px_ಸರತಿ );

                /* Semaphores are queues with a data size of zero and where the
                 * messages waiting is the semaphore's count.  Reduce the count. */
                px_ಸರತಿ->ux_Messages_ಕಾಯುವಿಕೆ = ux_Semaphore_ಎಣಿಕೆ - ( UBaseType_t ) 1;

                #ಆದರೆ ( configUSE_MUTEXES == 1 )
                {
                    ಆದರೆ( px_ಸರತಿ->ux_ಸರತಿ_Type == ಸರತಿ_ಸರತಿ_IS_MUTEX )
                    {
                        /* Record the information required to implement
                         * priority inheritance should it become necessary. */
                        px_ಸರತಿ->u.xSemaphore.xMutexHolder = pv_Task_Increment_Mutex_Held_ಎಣಿಕೆ();
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #ಆದರೆ_ಅಂತ್ಯ /* configUSE_MUTEXES */

                /* Check to see if other tasks are blocked waiting to give the
                 * semaphore, and if so, unblock the highest priority such task. */
                ಆದರೆ( list_LIST_IS_ಖಾಲಿಗಳು( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Send ) ) == pdFALSE )
                {
                    ಆದರೆ( xTaskRemoveFromEventList( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Send ) ) != pdFALSE )
                    {
                        ಸರತಿ_YIELD_IF_USING_PREEMPTION();
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

                taskEXIT_CRITICAL( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ) );
                ಹಿಂತಿರುಗಿ pdPASS;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ಆದರೆ( x_Ticks_ಗಮ್ಯ_Wait == ( TickType_t ) 0 )
                {
                    /* The semaphore count was 0 and no block time is specified
                     * (or the block time has expired) so exit now. */
                    taskEXIT_CRITICAL( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ) );
                    trace_ಸರತಿ_RECEIVE_FAILED( px_ಸರತಿ );
                    ಹಿಂತಿರುಗಿ err_ಸರತಿ_ಖಾಲಿಗಳು;
                }
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( x_ನಮೂದು_ಸಮಯ_ಗಣ == pdFALSE )
                {
                    /* The semaphore count was 0 and a block time was specified
                     * so configure the timeout structure ready to block. */
                    v_Task_ಆಂತರಿಕ_ಗಣ_ಸಮಯ_ಹೊರಹೋಗು_State( &x_ಸಮಯ_ಹೊರಹೋಗು_2 );
                    x_ನಮೂದು_ಸಮಯ_ಗಣ = pdTRUE;
                }
                ಇಲ್ಲದಿದ್ದರೆ
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
            #ಆದರೆ ( ಸರತಿ_USE_LOCKS == 0 )
            {
                /* Update the timeout state to see if it has expired yet. */
                ಆದರೆ( x_Task_Check_For_ಸಮಯ_ಹೊರಹೋಗು( &x_ಸಮಯ_ಹೊರಹೋಗು_2, &x_Ticks_ಗಮ್ಯ_Wait ) == pdFALSE )
                {
                    /* Not timed out yet. If this is a mutex, make the holder
                     * inherit our priority, then block the current task. */
                    trace_BLOCKING_ON_ಸರತಿ_RECEIVE( px_ಸರತಿ );
                    #ಆದರೆ ( configUSE_MUTEXES == 1 )
                    {
                        ಆದರೆ( px_ಸರತಿ->ux_ಸರತಿ_Type == ಸರತಿ_ಸರತಿ_IS_MUTEX )
                        {
                            xInheritanceOccurred = x_Task_ಆದ್ಯತೆ_Inherit( px_ಸರತಿ->u.xSemaphore.xMutexHolder );
                        }
                        ಇಲ್ಲದಿದ್ದರೆ
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    #ಆದರೆ_ಅಂತ್ಯ /* if ( configUSE_MUTEXES == 1 ) */
                    v_Task_ಇರಿಸು_On_Event_List( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive ), x_Ticks_ಗಮ್ಯ_Wait );
                    ಪೋರ್ಟ್_YIELD_WITHIN_API();
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    /* We have timed out. If this is a mutex, make the holder
                     * disinherit our priority, then return an error. */
                    #ಆದರೆ ( configUSE_MUTEXES == 1 )
                    {
                        ಆದರೆ( xInheritanceOccurred != pdFALSE )
                        {
                            UBaseType_t ux_Highest_ಕಾಯುವಿಕೆ_ಆದ್ಯತೆ;
                            ux_Highest_ಕಾಯುವಿಕೆ_ಆದ್ಯತೆ = prv_Get_Disinherit_ಆದ್ಯತೆ_After_Timeout( px_ಸರತಿ );
                            v_Task_ಆದ್ಯತೆ_Disinherit_After_Timeout( px_ಸರತಿ->u.xSemaphore.xMutexHolder, ux_Highest_ಕಾಯುವಿಕೆ_ಆದ್ಯತೆ );
                        }
                    }
                    #ಆದರೆ_ಅಂತ್ಯ /* configUSE_MUTEXES */
                    taskEXIT_CRITICAL( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ) );
                    trace_ಸರತಿ_RECEIVE_FAILED( px_ಸರತಿ );
                    ಹಿಂತಿರುಗಿ err_ಸರತಿ_ಖಾಲಿಗಳು;
                }
            }
            #ಆದರೆ_ಅಂತ್ಯ /* queueUSE_LOCKS == 0 */
        }
        taskEXIT_CRITICAL( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ) );

        /* If queue locks ARE being used:
         * - At this point, the semaphore/mutex is empty/held and entry time
         *   has been set.
         * - We follow the original procedure for locking the queue, inheriting
         *   priority, then attempting to block. */
        #ಆದರೆ ( ಸರತಿ_USE_LOCKS == 1 )
        {
            /* Interrupts and other tasks can give to and take from the semaphore
             * now the critical section has been exited. */

            vTaskSuspendAll();
            prv_ಬೀಗ_ಸರತಿ( px_ಸರತಿ );

            /* Update the timeout state to see if it has expired yet. */
            ಆದರೆ( x_Task_Check_For_ಸಮಯ_ಹೊರಹೋಗು( &x_ಸಮಯ_ಹೊರಹೋಗು_2, &x_Ticks_ಗಮ್ಯ_Wait ) == pdFALSE )
            {
                /* A block time is specified and not expired.  If the semaphore
                 * count is 0 then enter the Blocked state to wait for a semaphore to
                 * become available.  As semaphores are implemented with queues the
                 * queue being empty is equivalent to the semaphore count being 0. */
                ಆದರೆ( prv_Is_ಸರತಿ_ಖಾಲಿಗಳು( px_ಸರತಿ ) != pdFALSE )
                {
                    trace_BLOCKING_ON_ಸರತಿ_RECEIVE( px_ಸರತಿ );

                    #ಆದರೆ ( configUSE_MUTEXES == 1 )
                    {
                        ಆದರೆ( px_ಸರತಿ->ux_ಸರತಿ_Type == ಸರತಿ_ಸರತಿ_IS_MUTEX )
                        {
                            taskENTER_CRITICAL( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ) );
                            {
                                xInheritanceOccurred = x_Task_ಆದ್ಯತೆ_Inherit( px_ಸರತಿ->u.xSemaphore.xMutexHolder );
                            }
                            taskEXIT_CRITICAL( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ) );
                        }
                        ಇಲ್ಲದಿದ್ದರೆ
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    #ಆದರೆ_ಅಂತ್ಯ /* if ( configUSE_MUTEXES == 1 ) */

                    v_Task_ಇರಿಸು_On_Event_List( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive ), x_Ticks_ಗಮ್ಯ_Wait );
                    prv_Unlock_ಸರತಿ( px_ಸರತಿ );

                    ಆದರೆ( xTaskResumeAll() == pdFALSE )
                    {
                        ಪೋರ್ಟ್_YIELD_WITHIN_API();
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    /* There was no timeout and the semaphore count was not 0, so
                     * attempt to take the semaphore again. */
                    prv_Unlock_ಸರತಿ( px_ಸರತಿ );
                    ( ಶೂನ್ಯ ) xTaskResumeAll();
                }
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                /* Timed out. */
                prv_Unlock_ಸರತಿ( px_ಸರತಿ );
                ( ಶೂನ್ಯ ) xTaskResumeAll();

                /* If the semaphore count is 0 exit now as the timeout has
                 * expired.  Otherwise return to attempt to take the semaphore that is
                 * known to be available.  As semaphores are implemented by queues the
                 * queue being empty is equivalent to the semaphore count being 0. */
                ಆದರೆ( prv_Is_ಸರತಿ_ಖಾಲಿಗಳು( px_ಸರತಿ ) != pdFALSE )
                {
                    #ಆದರೆ ( configUSE_MUTEXES == 1 )
                    {
                        /* xInheritanceOccurred could only have be set if
                         * pxQueue->uxQueueType == queueQUEUE_IS_MUTEX so no need to
                         * test the mutex type again to check it is actually a mutex. */
                        ಆದರೆ( xInheritanceOccurred != pdFALSE )
                        {
                            taskENTER_CRITICAL( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ) );
                            {
                                UBaseType_t ux_Highest_ಕಾಯುವಿಕೆ_ಆದ್ಯತೆ;

                                /* This task blocking on the mutex caused another
                                 * task to inherit this task's priority.  Now this task
                                 * has timed out the priority should be disinherited
                                 * again, but only as low as the next highest priority
                                 * task that is waiting for the same mutex. */
                                ux_Highest_ಕಾಯುವಿಕೆ_ಆದ್ಯತೆ = prv_Get_Disinherit_ಆದ್ಯತೆ_After_Timeout( px_ಸರತಿ );
                                v_Task_ಆದ್ಯತೆ_Disinherit_After_Timeout( px_ಸರತಿ->u.xSemaphore.xMutexHolder, ux_Highest_ಕಾಯುವಿಕೆ_ಆದ್ಯತೆ );
                            }
                            taskEXIT_CRITICAL( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ) );
                        }
                    }
                    #ಆದರೆ_ಅಂತ್ಯ /* configUSE_MUTEXES */

                    trace_ಸರತಿ_RECEIVE_FAILED( px_ಸರತಿ );
                    ಹಿಂತಿರುಗಿ err_ಸರತಿ_ಖಾಲಿಗಳು;
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
        }
        #ಆದರೆ_ಅಂತ್ಯ /* queueUSE_LOCKS == 1 */
    } /*lint -restore */
}
/*-----------------------------------------------------------*/

ತಳ_Type_t x_ಸರತಿ_Peek( ಸರತಿ_Handle_t x_ಸರತಿ,
                       ಶೂನ್ಯ * ಸ್ಥಿರಾಂಕ pv_ಬಫರ್,
                       TickType_t x_Ticks_ಗಮ್ಯ_Wait )
{
    ತಳ_Type_t x_ನಮೂದು_ಸಮಯ_ಗಣ = pdFALSE;
    ಸಮಯ_ಹೊರಹೋಗು_t x_ಸಮಯ_ಹೊರಹೋಗು_2;
    int8_t * pcOriginalReadPosition;
    ಸರತಿ_t * ಸ್ಥಿರಾಂಕ px_ಸರತಿ = x_ಸರತಿ;

    /* Check the pointer is not NULL. */
    configASSERT( ( px_ಸರತಿ ) );

    /* The buffer into which data is received can only be NULL if the data size
     * is zero (so no data is copied into the buffer. */
    configASSERT( !( ( ( pv_ಬಫರ್ ) == ಖಾಲಿ ) && ( ( px_ಸರತಿ )->ux_ಅಂಶ_ಗಾತ್ರ != ( UBaseType_t ) 0U ) ) );

    /* Cannot block if the scheduler is suspended. */
    #ಆದರೆ ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_ಗಮ್ಯ_Wait != 0 ) ) );
    }
    #ಆದರೆ_ಅಂತ್ಯ

    /*lint -save -e904  This function relaxes the coding standard somewhat to
     * allow return statements within the function itself.  This is done in the
     * interest of execution time efficiency. */
    ಗಾಗಿ( ; ; )
    {
        taskENTER_CRITICAL( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ) );
        {
            ಸ್ಥಿರಾಂಕ UBaseType_t ux_Messages_ಕಾಯುವಿಕೆ = px_ಸರತಿ->ux_Messages_ಕಾಯುವಿಕೆ;

            /* Is there data in the queue now?  To be running the calling task
             * must be the highest priority task wanting to access the queue. */
            ಆದರೆ( ux_Messages_ಕಾಯುವಿಕೆ > ( UBaseType_t ) 0 )
            {
                /* Remember the read position so it can be reset after the data
                 * is read from the queue as this function is only peeking the
                 * data, not removing it. */
                pcOriginalReadPosition = px_ಸರತಿ->u.x_ಸರತಿ.pcReadFrom;

                prv_Copy_ದತ್ತಾಂಶ_From_ಸರತಿ( px_ಸರತಿ, pv_ಬಫರ್ );
                trace_ಸರತಿ_PEEK( px_ಸರತಿ );

                /* The data is not being removed, so reset the read pointer. */
                px_ಸರತಿ->u.x_ಸರತಿ.pcReadFrom = pcOriginalReadPosition;

                /* The data is being left in the queue, so see if there are
                 * any other tasks waiting for the data. */
                ಆದರೆ( list_LIST_IS_ಖಾಲಿಗಳು( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive ) ) == pdFALSE )
                {
                    ಆದರೆ( xTaskRemoveFromEventList( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive ) ) != pdFALSE )
                    {
                        /* The task waiting has a higher priority than this task. */
                        ಸರತಿ_YIELD_IF_USING_PREEMPTION();
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

                taskEXIT_CRITICAL( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ) );
                ಹಿಂತಿರುಗಿ pdPASS;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ಆದರೆ( x_Ticks_ಗಮ್ಯ_Wait == ( TickType_t ) 0 )
                {
                    /* The queue was empty and no block time is specified (or
                     * the block time has expired) so leave now. */
                    taskEXIT_CRITICAL( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ) );
                    trace_ಸರತಿ_PEEK_FAILED( px_ಸರತಿ );
                    ಹಿಂತಿರುಗಿ err_ಸರತಿ_ಖಾಲಿಗಳು;
                }
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( x_ನಮೂದು_ಸಮಯ_ಗಣ == pdFALSE )
                {
                    /* The queue was empty and a block time was specified so
                     * configure the timeout structure ready to enter the blocked
                     * state. */
                    v_Task_ಆಂತರಿಕ_ಗಣ_ಸಮಯ_ಹೊರಹೋಗು_State( &x_ಸಮಯ_ಹೊರಹೋಗು_2 );
                    x_ನಮೂದು_ಸಮಯ_ಗಣ = pdTRUE;
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    /* Entry time was already set. */
                    mtCOVERAGE_TEST_MARKER();
                }
            }

            /* If queue locks ARE NOT being used:
             * - At this point, the queue is empty and entry time has been set
             * - We simply check for a time out, block if not timed out, or
             *   return an error if we have timed out. */
            #ಆದರೆ ( ಸರತಿ_USE_LOCKS == 0 )
            {
                /* Update the timeout state to see if it has expired yet. */
                ಆದರೆ( x_Task_Check_For_ಸಮಯ_ಹೊರಹೋಗು( &x_ಸಮಯ_ಹೊರಹೋಗು_2, &x_Ticks_ಗಮ್ಯ_Wait ) == pdFALSE )
                {
                    /* Not timed out yet. Block the current task. */
                    trace_BLOCKING_ON_ಸರತಿ_PEEK( px_ಸರತಿ );
                    v_Task_ಇರಿಸು_On_Event_List( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive ), x_Ticks_ಗಮ್ಯ_Wait );
                    ಪೋರ್ಟ್_YIELD_WITHIN_API();
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    /* We have timed out. Return an error. */
                    taskEXIT_CRITICAL( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ) );
                    trace_ಸರತಿ_PEEK_FAILED( px_ಸರತಿ );
                    ಹಿಂತಿರುಗಿ err_ಸರತಿ_ಖಾಲಿಗಳು;
                }
            }
            #ಆದರೆ_ಅಂತ್ಯ /* queueUSE_LOCKS == 0 */
        }
        taskEXIT_CRITICAL( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ) );

        /* If queue locks ARE being used:
         * - At this point, the queue is empty and entry time has been set
         * - We follow the original procedure for locking the queue before
         *   attempting to block. */
        #ಆದರೆ ( ಸರತಿ_USE_LOCKS == 1 )
        {
            /* Interrupts and other tasks can send to and receive from the queue
             * now that the critical section has been exited. */

            vTaskSuspendAll();
            prv_ಬೀಗ_ಸರತಿ( px_ಸರತಿ );

            /* Update the timeout state to see if it has expired yet. */
            ಆದರೆ( x_Task_Check_For_ಸಮಯ_ಹೊರಹೋಗು( &x_ಸಮಯ_ಹೊರಹೋಗು_2, &x_Ticks_ಗಮ್ಯ_Wait ) == pdFALSE )
            {
                /* Timeout has not expired yet, check to see if there is data in the
                * queue now, and if not enter the Blocked state to wait for data. */
                ಆದರೆ( prv_Is_ಸರತಿ_ಖಾಲಿಗಳು( px_ಸರತಿ ) != pdFALSE )
                {
                    trace_BLOCKING_ON_ಸರತಿ_PEEK( px_ಸರತಿ );
                    v_Task_ಇರಿಸು_On_Event_List( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive ), x_Ticks_ಗಮ್ಯ_Wait );
                    prv_Unlock_ಸರತಿ( px_ಸರತಿ );

                    ಆದರೆ( xTaskResumeAll() == pdFALSE )
                    {
                        ಪೋರ್ಟ್_YIELD_WITHIN_API();
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    /* There is data in the queue now, so don't enter the blocked
                     * state, instead return to try and obtain the data. */
                    prv_Unlock_ಸರತಿ( px_ಸರತಿ );
                    ( ಶೂನ್ಯ ) xTaskResumeAll();
                }
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                /* The timeout has expired.  If there is still no data in the queue
                 * exit, otherwise go back and try to read the data again. */
                prv_Unlock_ಸರತಿ( px_ಸರತಿ );
                ( ಶೂನ್ಯ ) xTaskResumeAll();

                ಆದರೆ( prv_Is_ಸರತಿ_ಖಾಲಿಗಳು( px_ಸರತಿ ) != pdFALSE )
                {
                    trace_ಸರತಿ_PEEK_FAILED( px_ಸರತಿ );
                    ಹಿಂತಿರುಗಿ err_ಸರತಿ_ಖಾಲಿಗಳು;
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
        }
        #ಆದರೆ_ಅಂತ್ಯ /* queueUSE_LOCKS == 1 */
    } /*lint -restore */
}
/*-----------------------------------------------------------*/

ತಳ_Type_t x_ಸರತಿ_Receive_From_ISR( ಸರತಿ_Handle_t x_ಸರತಿ,
                                 ಶೂನ್ಯ * ಸ್ಥಿರಾಂಕ pv_ಬಫರ್,
                                 ತಳ_Type_t * ಸ್ಥಿರಾಂಕ px_Higher_ಆದ್ಯತೆ_Task_Woken )
{
    ತಳ_Type_t xReturn;
    UBaseType_t ux_Saved_ಅಡಚಣೆ_ಸ್ಥಿತಿ;
    ಸರತಿ_t * ಸ್ಥಿರಾಂಕ px_ಸರತಿ = x_ಸರತಿ;

    configASSERT( px_ಸರತಿ );
    configASSERT( !( ( pv_ಬಫರ್ == ಖಾಲಿ ) && ( px_ಸರತಿ->ux_ಅಂಶ_ಗಾತ್ರ != ( UBaseType_t ) 0U ) ) );

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
    ಪೋರ್ಟ್_ASSERT_IF_ಅಡಚಣೆ_ಆದ್ಯತೆ_INVALID();

    prv_ENTER_CRITICAL_OR_ಮುಖವಾಡ_ISR( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ), ux_Saved_ಅಡಚಣೆ_ಸ್ಥಿತಿ );
    {
        ಸ್ಥಿರಾಂಕ UBaseType_t ux_Messages_ಕಾಯುವಿಕೆ = px_ಸರತಿ->ux_Messages_ಕಾಯುವಿಕೆ;

        /* Cannot block in an ISR, so check there is data available. */
        ಆದರೆ( ux_Messages_ಕಾಯುವಿಕೆ > ( UBaseType_t ) 0 )
        {
            #ಆದರೆ ( ಸರತಿ_USE_LOCKS == 1 )
                ಸ್ಥಿರಾಂಕ int8_t c_Rx_ಬೀಗ = px_ಸರತಿ->c_Rx_ಬೀಗ;
            #ಇಲ್ಲದಿದ್ದರೆ
                /* Queue locks not used, so we treat it as unlocked. */
                ಸ್ಥಿರಾಂಕ int8_t c_Rx_ಬೀಗ = ಸರತಿ_UNLOCKED;
            #ಆದರೆ_ಅಂತ್ಯ /* queueUSE_LOCKS == 1 */

            trace_ಸರತಿ_RECEIVE_FROM_ISR( px_ಸರತಿ );

            prv_Copy_ದತ್ತಾಂಶ_From_ಸರತಿ( px_ಸರತಿ, pv_ಬಫರ್ );
            px_ಸರತಿ->ux_Messages_ಕಾಯುವಿಕೆ = ux_Messages_ಕಾಯುವಿಕೆ - ( UBaseType_t ) 1;

            /* If the queue is locked the event list will not be modified.
             * Instead update the lock count so the task that unlocks the queue
             * will know that an ISR has removed data while the queue was
             * locked. */
            ಆದರೆ( c_Rx_ಬೀಗ == ಸರತಿ_UNLOCKED )
            {
                ಆದರೆ( list_LIST_IS_ಖಾಲಿಗಳು( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Send ) ) == pdFALSE )
                {
                    ಆದರೆ( xTaskRemoveFromEventList( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Send ) ) != pdFALSE )
                    {
                        /* The task waiting has a higher priority than us so
                         * force a context switch. */
                        ಆದರೆ( px_Higher_ಆದ್ಯತೆ_Task_Woken != ಖಾಲಿ )
                        {
                            *px_Higher_ಆದ್ಯತೆ_Task_Woken = pdTRUE;
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
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                #ಆದರೆ ( ಸರತಿ_USE_LOCKS == 1 )
                {
                    /* Increment the lock count so the task that unlocks the queue
                     * knows that data was removed while it was locked. */
                    prv_Increment_ಸರತಿ_Rx_ಬೀಗ( px_ಸರತಿ, c_Rx_ಬೀಗ );
                }
                #ಆದರೆ_ಅಂತ್ಯ /* queueUSE_LOCKS == 1 */
            }

            xReturn = pdPASS;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            xReturn = pdFAIL;
            trace_ಸರತಿ_RECEIVE_FROM_ISR_FAILED( px_ಸರತಿ );
        }
    }
    prvEXIT_CRITICAL_OR_UNMASK_ISR( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ), ux_Saved_ಅಡಚಣೆ_ಸ್ಥಿತಿ );

    ಹಿಂತಿರುಗಿ xReturn;
}
/*-----------------------------------------------------------*/

ತಳ_Type_t x_ಸರತಿ_Peek_From_ISR( ಸರತಿ_Handle_t x_ಸರತಿ,
                              ಶೂನ್ಯ * ಸ್ಥಿರಾಂಕ pv_ಬಫರ್ )
{
    ತಳ_Type_t xReturn;
    UBaseType_t ux_Saved_ಅಡಚಣೆ_ಸ್ಥಿತಿ;
    int8_t * pcOriginalReadPosition;
    ಸರತಿ_t * ಸ್ಥಿರಾಂಕ px_ಸರತಿ = x_ಸರತಿ;

    configASSERT( px_ಸರತಿ );
    configASSERT( !( ( pv_ಬಫರ್ == ಖಾಲಿ ) && ( px_ಸರತಿ->ux_ಅಂಶ_ಗಾತ್ರ != ( UBaseType_t ) 0U ) ) );
    configASSERT( px_ಸರತಿ->ux_ಅಂಶ_ಗಾತ್ರ != 0 ); /* Can't peek a semaphore. */

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
    ಪೋರ್ಟ್_ASSERT_IF_ಅಡಚಣೆ_ಆದ್ಯತೆ_INVALID();

    prv_ENTER_CRITICAL_OR_ಮುಖವಾಡ_ISR( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ), ux_Saved_ಅಡಚಣೆ_ಸ್ಥಿತಿ );
    {
        /* Cannot block in an ISR, so check there is data available. */
        ಆದರೆ( px_ಸರತಿ->ux_Messages_ಕಾಯುವಿಕೆ > ( UBaseType_t ) 0 )
        {
            trace_ಸರತಿ_PEEK_FROM_ISR( px_ಸರತಿ );

            /* Remember the read position so it can be reset as nothing is
             * actually being removed from the queue. */
            pcOriginalReadPosition = px_ಸರತಿ->u.x_ಸರತಿ.pcReadFrom;
            prv_Copy_ದತ್ತಾಂಶ_From_ಸರತಿ( px_ಸರತಿ, pv_ಬಫರ್ );
            px_ಸರತಿ->u.x_ಸರತಿ.pcReadFrom = pcOriginalReadPosition;

            xReturn = pdPASS;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            xReturn = pdFAIL;
            trace_ಸರತಿ_PEEK_FROM_ISR_FAILED( px_ಸರತಿ );
        }
    }
    prvEXIT_CRITICAL_OR_UNMASK_ISR( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ), ux_Saved_ಅಡಚಣೆ_ಸ್ಥಿತಿ );

    ಹಿಂತಿರುಗಿ xReturn;
}
/*-----------------------------------------------------------*/

UBaseType_t ux_ಸರತಿ_Messages_ಕಾಯುವಿಕೆ( ಸ್ಥಿರಾಂಕ ಸರತಿ_Handle_t x_ಸರತಿ )
{
    UBaseType_t uxReturn;

    configASSERT( x_ಸರತಿ );

    taskENTER_CRITICAL( &( ( ( ಸರತಿ_t * ) x_ಸರತಿ )->x_ಸರತಿ_ಬೀಗ ) );
    {
        uxReturn = ( ( ಸರತಿ_t * ) x_ಸರತಿ )->ux_Messages_ಕಾಯುವಿಕೆ;
    }
    taskEXIT_CRITICAL( &( ( ( ಸರತಿ_t * ) x_ಸರತಿ )->x_ಸರತಿ_ಬೀಗ ) );

    ಹಿಂತಿರುಗಿ uxReturn;
} /*lint !e818 Pointer cannot be declared const as xQueue is a typedef not pointer. */
/*-----------------------------------------------------------*/

UBaseType_t ux_ಸರತಿ_Spaces_ಲಭ್ಯ( ಸ್ಥಿರಾಂಕ ಸರತಿ_Handle_t x_ಸರತಿ )
{
    UBaseType_t uxReturn;
    ಸರತಿ_t * ಸ್ಥಿರಾಂಕ px_ಸರತಿ = x_ಸರತಿ;

    configASSERT( px_ಸರತಿ );

    taskENTER_CRITICAL( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ) );
    {
        uxReturn = px_ಸರತಿ->ux_ಉದ್ದಳತೆ - px_ಸರತಿ->ux_Messages_ಕಾಯುವಿಕೆ;
    }
    taskEXIT_CRITICAL( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ) );

    ಹಿಂತಿರುಗಿ uxReturn;
} /*lint !e818 Pointer cannot be declared const as xQueue is a typedef not pointer. */
/*-----------------------------------------------------------*/

UBaseType_t ux_ಸರತಿ_Messages_ಕಾಯುವಿಕೆ_From_ISR( ಸ್ಥಿರಾಂಕ ಸರತಿ_Handle_t x_ಸರತಿ )
{
    UBaseType_t uxReturn;
    ಸರತಿ_t * ಸ್ಥಿರಾಂಕ px_ಸರತಿ = x_ಸರತಿ;

    configASSERT( px_ಸರತಿ );
    uxReturn = px_ಸರತಿ->ux_Messages_ಕಾಯುವಿಕೆ;

    ಹಿಂತಿರುಗಿ uxReturn;
} /*lint !e818 Pointer cannot be declared const as xQueue is a typedef not pointer. */
/*-----------------------------------------------------------*/

ಶೂನ್ಯ v_ಸರತಿ_Delete( ಸರತಿ_Handle_t x_ಸರತಿ )
{
    ಸರತಿ_t * ಸ್ಥಿರಾಂಕ px_ಸರತಿ = x_ಸರತಿ;

    configASSERT( px_ಸರತಿ );
    trace_ಸರತಿ_DELETE( px_ಸರತಿ );

    #ಆದರೆ ( config_ಸರತಿ_REGISTRY_ಗಾತ್ರ > 0 )
    {
        v_ಸರತಿ_Unregister_ಸರತಿ( px_ಸರತಿ );
    }
    #ಆದರೆ_ಅಂತ್ಯ

    #ಆದರೆ ( ( config_SUPPORT_DYNAMIC_ಹಂಚಿಕೆ == 1 ) && ( config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 0 ) )
    {
        /* The queue can only have been allocated dynamically - free it
         * again. */
        v_ಪೋರ್ಟ್_Free( px_ಸರತಿ );
    }
    #ಅಥವಾಆದರೆ ( ( config_SUPPORT_DYNAMIC_ಹಂಚಿಕೆ == 1 ) && ( config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 1 ) )
    {
        /* The queue could have been allocated statically or dynamically, so
         * check before attempting to free the memory. */
        ಆದರೆ( px_ಸರತಿ->ucStaticallyAllocated == ( uint8_t ) pdFALSE )
        {
            v_ಪೋರ್ಟ್_Free( px_ಸರತಿ );
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    #ಇಲ್ಲದಿದ್ದರೆ /* if ( ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) && ( configSUPPORT_STATIC_ALLOCATION == 0 ) ) */
    {
        /* The queue must have been statically allocated, so is not going to be
         * deleted.  Avoid compiler warnings about the unused parameter. */
        ( ಶೂನ್ಯ ) px_ಸರತಿ;
    }
    #ಆದರೆ_ಅಂತ್ಯ /* configSUPPORT_DYNAMIC_ALLOCATION */
}
/*-----------------------------------------------------------*/

#ಆದರೆ ( configUSE_TRACE_FACILITY == 1 )

    UBaseType_t ux_ಸರತಿ_Get_ಸರತಿ_ಸಂಖ್ಯೆ( ಸರತಿ_Handle_t x_ಸರತಿ )
    {
        ಹಿಂತಿರುಗಿ ( ( ಸರತಿ_t * ) x_ಸರತಿ )->ux_ಸರತಿ_ಸಂಖ್ಯೆ;
    }

#ಆದರೆ_ಅಂತ್ಯ /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#ಆದರೆ ( configUSE_TRACE_FACILITY == 1 )

    ಶೂನ್ಯ v_ಸರತಿ_ಗಣ_ಸರತಿ_ಸಂಖ್ಯೆ( ಸರತಿ_Handle_t x_ಸರತಿ,
                               UBaseType_t ux_ಸರತಿ_ಸಂಖ್ಯೆ )
    {
        ( ( ಸರತಿ_t * ) x_ಸರತಿ )->ux_ಸರತಿ_ಸಂಖ್ಯೆ = ux_ಸರತಿ_ಸಂಖ್ಯೆ;
    }

#ಆದರೆ_ಅಂತ್ಯ /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#ಆದರೆ ( configUSE_TRACE_FACILITY == 1 )

    uint8_t uc_ಸರತಿ_Get_ಸರತಿ_Type( ಸರತಿ_Handle_t x_ಸರತಿ )
    {
        ಹಿಂತಿರುಗಿ ( ( ಸರತಿ_t * ) x_ಸರತಿ )->uc_ಸರತಿ_Type;
    }

#ಆದರೆ_ಅಂತ್ಯ /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#ಆದರೆ ( configUSE_MUTEXES == 1 )

    ಸ್ಥಿರ UBaseType_t prv_Get_Disinherit_ಆದ್ಯತೆ_After_Timeout( ಸ್ಥಿರಾಂಕ ಸರತಿ_t * ಸ್ಥಿರಾಂಕ px_ಸರತಿ )
    {
        UBaseType_t ux_Highest_ಆದ್ಯತೆ_ದ_ಕಾಯುವಿಕೆ_Tasks;

        /* If a task waiting for a mutex causes the mutex holder to inherit a
         * priority, but the waiting task times out, then the holder should
         * disinherit the priority - but only down to the highest priority of any
         * other tasks that are waiting for the same mutex.  For this purpose,
         * return the priority of the highest priority task that is waiting for the
         * mutex. */
        ಆದರೆ( list_CURRENT_LIST_ಉದ್ದಳತೆ( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive ) ) > 0U )
        {
            ux_Highest_ಆದ್ಯತೆ_ದ_ಕಾಯುವಿಕೆ_Tasks = ( UBaseType_t ) configMAX_PRIORITIES - ( UBaseType_t ) list_GET_ಅಂಶ_ಮೌಲ್ಯ_ದ_ತಲೆ_ನಮೂದು( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive ) );
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ux_Highest_ಆದ್ಯತೆ_ದ_ಕಾಯುವಿಕೆ_Tasks = tsk_IDLE_ಆದ್ಯತೆ;
        }

        ಹಿಂತಿರುಗಿ ux_Highest_ಆದ್ಯತೆ_ದ_ಕಾಯುವಿಕೆ_Tasks;
    }

#ಆದರೆ_ಅಂತ್ಯ /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

ಸ್ಥಿರ ತಳ_Type_t prv_Copy_ದತ್ತಾಂಶ_ಗಮ್ಯ_ಸರತಿ( ಸರತಿ_t * ಸ್ಥಿರಾಂಕ px_ಸರತಿ,
                                      ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ * pv_ಅಂಶ_ಗಮ್ಯ_ಸರತಿ,
                                      ಸ್ಥಿರಾಂಕ ತಳ_Type_t xPosition )
{
    ತಳ_Type_t xReturn = pdFALSE;
    UBaseType_t ux_Messages_ಕಾಯುವಿಕೆ;

    /* This function is called from a critical section. */

    ux_Messages_ಕಾಯುವಿಕೆ = px_ಸರತಿ->ux_Messages_ಕಾಯುವಿಕೆ;

    ಆದರೆ( px_ಸರತಿ->ux_ಅಂಶ_ಗಾತ್ರ == ( UBaseType_t ) 0 )
    {
        #ಆದರೆ ( configUSE_MUTEXES == 1 )
        {
            ಆದರೆ( px_ಸರತಿ->ux_ಸರತಿ_Type == ಸರತಿ_ಸರತಿ_IS_MUTEX )
            {
                /* The mutex is no longer being held. */
                xReturn = x_Task_ಆದ್ಯತೆ_Disinherit( px_ಸರತಿ->u.xSemaphore.xMutexHolder );
                px_ಸರತಿ->u.xSemaphore.xMutexHolder = ಖಾಲಿ;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        #ಆದರೆ_ಅಂತ್ಯ /* configUSE_MUTEXES */
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( xPosition == ಸರತಿ_SEND_ಗಮ್ಯ_ಹಿಂಭಾಗ )
    {
        ( ಶೂನ್ಯ ) ಸ್ಮರಣೆ_ನಕಲು( ( ಶೂನ್ಯ * ) px_ಸರತಿ->pc_Write_ಗಮ್ಯ, pv_ಅಂಶ_ಗಮ್ಯ_ಸರತಿ, ( size_t ) px_ಸರತಿ->ux_ಅಂಶ_ಗಾತ್ರ ); /*lint !e961 !e418 !e9087 MISRA exception as the casts are only redundant for some ports, plus previous logic ensures a null pointer can only be passed to memcpy() if the copy size is 0.  Cast to void required by function signature and safe as no alignment requirement and copy length specified in bytes. */
        px_ಸರತಿ->pc_Write_ಗಮ್ಯ += px_ಸರತಿ->ux_ಅಂಶ_ಗಾತ್ರ;                                                       /*lint !e9016 Pointer arithmetic on char types ok, especially in this use case where it is the clearest way of conveying intent. */

        ಆದರೆ( px_ಸರತಿ->pc_Write_ಗಮ್ಯ >= px_ಸರತಿ->u.x_ಸರತಿ.pc_ಬಾಲ )                                             /*lint !e946 MISRA exception justified as comparison of pointers is the cleanest solution. */
        {
            px_ಸರತಿ->pc_Write_ಗಮ್ಯ = px_ಸರತಿ->pc_ತಲೆ;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ( ಶೂನ್ಯ ) ಸ್ಮರಣೆ_ನಕಲು( ( ಶೂನ್ಯ * ) px_ಸರತಿ->u.x_ಸರತಿ.pcReadFrom, pv_ಅಂಶ_ಗಮ್ಯ_ಸರತಿ, ( size_t ) px_ಸರತಿ->ux_ಅಂಶ_ಗಾತ್ರ ); /*lint !e961 !e9087 !e418 MISRA exception as the casts are only redundant for some ports.  Cast to void required by function signature and safe as no alignment requirement and copy length specified in bytes.  Assert checks null pointer only used when length is 0. */
        px_ಸರತಿ->u.x_ಸರತಿ.pcReadFrom -= px_ಸರತಿ->ux_ಅಂಶ_ಗಾತ್ರ;

        ಆದರೆ( px_ಸರತಿ->u.x_ಸರತಿ.pcReadFrom < px_ಸರತಿ->pc_ತಲೆ ) /*lint !e946 MISRA exception justified as comparison of pointers is the cleanest solution. */
        {
            px_ಸರತಿ->u.x_ಸರತಿ.pcReadFrom = ( px_ಸರತಿ->u.x_ಸರತಿ.pc_ಬಾಲ - px_ಸರತಿ->ux_ಅಂಶ_ಗಾತ್ರ );
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            mtCOVERAGE_TEST_MARKER();
        }

        ಆದರೆ( xPosition == ಸರತಿ_OVERWRITE )
        {
            ಆದರೆ( ux_Messages_ಕಾಯುವಿಕೆ > ( UBaseType_t ) 0 )
            {
                /* An item is not being added but overwritten, so subtract
                 * one from the recorded number of items in the queue so when
                 * one is added again below the number of recorded items remains
                 * correct. */
                --ux_Messages_ಕಾಯುವಿಕೆ;
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
    }

    px_ಸರತಿ->ux_Messages_ಕಾಯುವಿಕೆ = ux_Messages_ಕಾಯುವಿಕೆ + ( UBaseType_t ) 1;

    ಹಿಂತಿರುಗಿ xReturn;
}
/*-----------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ prv_Copy_ದತ್ತಾಂಶ_From_ಸರತಿ( ಸರತಿ_t * ಸ್ಥಿರಾಂಕ px_ಸರತಿ,
                                  ಶೂನ್ಯ * ಸ್ಥಿರಾಂಕ pv_ಬಫರ್ )
{
    ಆದರೆ( px_ಸರತಿ->ux_ಅಂಶ_ಗಾತ್ರ != ( UBaseType_t ) 0 )
    {
        px_ಸರತಿ->u.x_ಸರತಿ.pcReadFrom += px_ಸರತಿ->ux_ಅಂಶ_ಗಾತ್ರ;           /*lint !e9016 Pointer arithmetic on char types ok, especially in this use case where it is the clearest way of conveying intent. */

        ಆದರೆ( px_ಸರತಿ->u.x_ಸರತಿ.pcReadFrom >= px_ಸರತಿ->u.x_ಸರತಿ.pc_ಬಾಲ ) /*lint !e946 MISRA exception justified as use of the relational operator is the cleanest solutions. */
        {
            px_ಸರತಿ->u.x_ಸರತಿ.pcReadFrom = px_ಸರತಿ->pc_ತಲೆ;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            mtCOVERAGE_TEST_MARKER();
        }

        ( ಶೂನ್ಯ ) ಸ್ಮರಣೆ_ನಕಲು( ( ಶೂನ್ಯ * ) pv_ಬಫರ್, ( ಶೂನ್ಯ * ) px_ಸರತಿ->u.x_ಸರತಿ.pcReadFrom, ( size_t ) px_ಸರತಿ->ux_ಅಂಶ_ಗಾತ್ರ ); /*lint !e961 !e418 !e9087 MISRA exception as the casts are only redundant for some ports.  Also previous logic ensures a null pointer can only be passed to memcpy() when the count is 0.  Cast to void required by function signature and safe as no alignment requirement and copy length specified in bytes. */
    }
}
/*-----------------------------------------------------------*/

#ಆದರೆ ( ಸರತಿ_USE_LOCKS == 1 )
    ಸ್ಥಿರ ಶೂನ್ಯ prv_Unlock_ಸರತಿ( ಸರತಿ_t * ಸ್ಥಿರಾಂಕ px_ಸರತಿ )
    {
        /* THIS FUNCTION MUST BE CALLED WITH THE SCHEDULER SUSPENDED. */

        /* The lock counts contains the number of extra data items placed or
         * removed from the queue while the queue was locked.  When a queue is
         * locked items can be added or removed, but the event lists cannot be
         * updated. */
        taskENTER_CRITICAL( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ) );
        {
            int8_t c_Tx_ಬೀಗ = px_ಸರತಿ->c_Tx_ಬೀಗ;

            /* See if data was added to the queue while it was locked. */
            ಆಗ( c_Tx_ಬೀಗ > ಸರತಿ_LOCKED_UNMODIFIED )
            {
                /* Data was posted while the queue was locked.  Are any tasks
                 * blocked waiting for data to become available? */
                #ಆದರೆ ( config_USE_ಸರತಿ_SETS == 1 )
                {
                    ಆದರೆ( px_ಸರತಿ->px_ಸರತಿ_ಗಣ_Container != ಖಾಲಿ )
                    {
                        ಆದರೆ( prv_Notify_ಸರತಿ_ಗಣ_Container( px_ಸರತಿ ) != pdFALSE )
                        {
                            /* The queue is a member of a queue set, and posting to
                             * the queue set caused a higher priority task to unblock.
                             * A context switch is required. */
                            vTaskMissedYield();
                        }
                        ಇಲ್ಲದಿದ್ದರೆ
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                    {
                        /* Tasks that are removed from the event list will get
                         * added to the pending ready list as the scheduler is still
                         * suspended. */
                        ಆದರೆ( list_LIST_IS_ಖಾಲಿಗಳು( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive ) ) == pdFALSE )
                        {
                            ಆದರೆ( xTaskRemoveFromEventList( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive ) ) != pdFALSE )
                            {
                                /* The task waiting has a higher priority so record that a
                                 * context switch is required. */
                                vTaskMissedYield();
                            }
                            ಇಲ್ಲದಿದ್ದರೆ
                            {
                                mtCOVERAGE_TEST_MARKER();
                            }
                        }
                        ಇಲ್ಲದಿದ್ದರೆ
                        {
                            ನಿಲ್ಲಿಸು;
                        }
                    }
                }
                #ಇಲ್ಲದಿದ್ದರೆ /* configUSE_QUEUE_SETS */
                {
                    /* Tasks that are removed from the event list will get added to
                     * the pending ready list as the scheduler is still suspended. */
                    ಆದರೆ( list_LIST_IS_ಖಾಲಿಗಳು( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive ) ) == pdFALSE )
                    {
                        ಆದರೆ( xTaskRemoveFromEventList( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive ) ) != pdFALSE )
                        {
                            /* The task waiting has a higher priority so record that
                             * a context switch is required. */
                            vTaskMissedYield();
                        }
                        ಇಲ್ಲದಿದ್ದರೆ
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                    {
                        ನಿಲ್ಲಿಸು;
                    }
                }
                #ಆದರೆ_ಅಂತ್ಯ /* configUSE_QUEUE_SETS */

                --c_Tx_ಬೀಗ;
            }

            px_ಸರತಿ->c_Tx_ಬೀಗ = ಸರತಿ_UNLOCKED;
        }
        taskEXIT_CRITICAL( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ) );

        /* Do the same for the Rx lock. */
        taskENTER_CRITICAL( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ) );
        {
            int8_t c_Rx_ಬೀಗ = px_ಸರತಿ->c_Rx_ಬೀಗ;

            ಆಗ( c_Rx_ಬೀಗ > ಸರತಿ_LOCKED_UNMODIFIED )
            {
                ಆದರೆ( list_LIST_IS_ಖಾಲಿಗಳು( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Send ) ) == pdFALSE )
                {
                    ಆದರೆ( xTaskRemoveFromEventList( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Send ) ) != pdFALSE )
                    {
                        vTaskMissedYield();
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }

                    --c_Rx_ಬೀಗ;
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    ನಿಲ್ಲಿಸು;
                }
            }

            px_ಸರತಿ->c_Rx_ಬೀಗ = ಸರತಿ_UNLOCKED;
        }
        taskEXIT_CRITICAL( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ) );
    }
#ಆದರೆ_ಅಂತ್ಯ /* queueUSE_LOCKS == 1 */
/*-----------------------------------------------------------*/

#ಆದರೆ ( ಸರತಿ_USE_LOCKS == 1 )
    ಸ್ಥಿರ ತಳ_Type_t prv_Is_ಸರತಿ_ಖಾಲಿಗಳು( ಸ್ಥಿರಾಂಕ ಸರತಿ_t * px_ಸರತಿ )
    {
        ತಳ_Type_t xReturn;

        taskENTER_CRITICAL( &( ( ( ಸರತಿ_t * ) px_ಸರತಿ )->x_ಸರತಿ_ಬೀಗ ) );
        {
            ಆದರೆ( px_ಸರತಿ->ux_Messages_ಕಾಯುವಿಕೆ == ( UBaseType_t ) 0 )
            {
                xReturn = pdTRUE;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                xReturn = pdFALSE;
            }
        }
        taskEXIT_CRITICAL( &( ( ( ಸರತಿ_t * ) px_ಸರತಿ )->x_ಸರತಿ_ಬೀಗ ) );

        ಹಿಂತಿರುಗಿ xReturn;
    }
#ಆದರೆ_ಅಂತ್ಯ /* queueUSE_LOCKS == 1 */
/*-----------------------------------------------------------*/

ತಳ_Type_t x_ಸರತಿ_Is_ಸರತಿ_ಖಾಲಿಗಳು_From_ISR( ಸ್ಥಿರಾಂಕ ಸರತಿ_Handle_t x_ಸರತಿ )
{
    ತಳ_Type_t xReturn;
    ಸರತಿ_t * ಸ್ಥಿರಾಂಕ px_ಸರತಿ = x_ಸರತಿ;

    configASSERT( px_ಸರತಿ );

    ಆದರೆ( px_ಸರತಿ->ux_Messages_ಕಾಯುವಿಕೆ == ( UBaseType_t ) 0 )
    {
        xReturn = pdTRUE;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        xReturn = pdFALSE;
    }

    ಹಿಂತಿರುಗಿ xReturn;
} /*lint !e818 xQueue could not be pointer to const because it is a typedef. */
/*-----------------------------------------------------------*/

#ಆದರೆ ( ಸರತಿ_USE_LOCKS == 1 )
    ಸ್ಥಿರ ತಳ_Type_t prv_Is_ಸರತಿ_ತುಂಬಿದ( ಸ್ಥಿರಾಂಕ ಸರತಿ_t * px_ಸರತಿ )
    {
        ತಳ_Type_t xReturn;

        taskENTER_CRITICAL( &( ( ( ಸರತಿ_t * ) px_ಸರತಿ )->x_ಸರತಿ_ಬೀಗ ) );
        {
            ಆದರೆ( px_ಸರತಿ->ux_Messages_ಕಾಯುವಿಕೆ == px_ಸರತಿ->ux_ಉದ್ದಳತೆ )
            {
                xReturn = pdTRUE;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                xReturn = pdFALSE;
            }
        }
        taskEXIT_CRITICAL( &( ( ( ಸರತಿ_t * ) px_ಸರತಿ )->x_ಸರತಿ_ಬೀಗ ) );

        ಹಿಂತಿರುಗಿ xReturn;
    }
#ಆದರೆ_ಅಂತ್ಯ /* queueUSE_LOCKS == 1 */
/*-----------------------------------------------------------*/

ತಳ_Type_t x_ಸರತಿ_Is_ಸರತಿ_ತುಂಬಿದ_From_ISR( ಸ್ಥಿರಾಂಕ ಸರತಿ_Handle_t x_ಸರತಿ )
{
    ತಳ_Type_t xReturn;
    ಸರತಿ_t * ಸ್ಥಿರಾಂಕ px_ಸರತಿ = x_ಸರತಿ;

    configASSERT( px_ಸರತಿ );

    ಆದರೆ( px_ಸರತಿ->ux_Messages_ಕಾಯುವಿಕೆ == px_ಸರತಿ->ux_ಉದ್ದಳತೆ )
    {
        xReturn = pdTRUE;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        xReturn = pdFALSE;
    }

    ಹಿಂತಿರುಗಿ xReturn;
} /*lint !e818 xQueue could not be pointer to const because it is a typedef. */
/*-----------------------------------------------------------*/

#ಆದರೆ ( configUSE_CO_ROUTINES == 1 )

    ತಳ_Type_t x_ಸರತಿ_CRSend( ಸರತಿ_Handle_t x_ಸರತಿ,
                             ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ * pv_ಅಂಶ_ಗಮ್ಯ_ಸರತಿ,
                             TickType_t x_Ticks_ಗಮ್ಯ_Wait )
    {
        ತಳ_Type_t xReturn;
        ಸರತಿ_t * ಸ್ಥಿರಾಂಕ px_ಸರತಿ = x_ಸರತಿ;

        /* If the queue is already full we may have to block.  A critical section
         * is required to prevent an interrupt removing something from the queue
         * between the check to see if the queue is full and blocking on the queue. */
        ಪೋರ್ಟ್_DISABLE_INTERRUPTS();
        {
            ಆದರೆ( prv_Is_ಸರತಿ_ತುಂಬಿದ( px_ಸರತಿ ) != pdFALSE )
            {
                /* The queue is full - do we want to block or just leave without
                 * posting? */
                ಆದರೆ( x_Ticks_ಗಮ್ಯ_Wait > ( TickType_t ) 0 )
                {
                    /* As this is called from a coroutine we cannot block directly, but
                     * return indicating that we need to block. */
                    v_Co_Routine_ಕೂಡಿಸು_ಗಮ್ಯ_Delayed_List( x_Ticks_ಗಮ್ಯ_Wait, &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Send ) );
                    ಪೋರ್ಟ್_ENABLE_INTERRUPTS();
                    ಹಿಂತಿರುಗಿ err_ಸರತಿ_BLOCKED;
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    ಪೋರ್ಟ್_ENABLE_INTERRUPTS();
                    ಹಿಂತಿರುಗಿ err_ಸರತಿ_ತುಂಬಿದ;
                }
            }
        }
        ಪೋರ್ಟ್_ENABLE_INTERRUPTS();

        ಪೋರ್ಟ್_DISABLE_INTERRUPTS();
        {
            ಆದರೆ( px_ಸರತಿ->ux_Messages_ಕಾಯುವಿಕೆ < px_ಸರತಿ->ux_ಉದ್ದಳತೆ )
            {
                /* There is room in the queue, copy the data into the queue. */
                prv_Copy_ದತ್ತಾಂಶ_ಗಮ್ಯ_ಸರತಿ( px_ಸರತಿ, pv_ಅಂಶ_ಗಮ್ಯ_ಸರತಿ, ಸರತಿ_SEND_ಗಮ್ಯ_ಹಿಂಭಾಗ );
                xReturn = pdPASS;

                /* Were any co-routines waiting for data to become available? */
                ಆದರೆ( list_LIST_IS_ಖಾಲಿಗಳು( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive ) ) == pdFALSE )
                {
                    /* In this instance the co-routine could be placed directly
                     * into the ready list as we are within a critical section.
                     * Instead the same pending ready list mechanism is used as if
                     * the event were caused from within an interrupt. */
                    ಆದರೆ( xCoRoutineRemoveFromEventList( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive ) ) != pdFALSE )
                    {
                        /* The co-routine waiting has a higher priority so record
                         * that a yield might be appropriate. */
                        xReturn = err_ಸರತಿ_YIELD;
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
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                xReturn = err_ಸರತಿ_ತುಂಬಿದ;
            }
        }
        ಪೋರ್ಟ್_ENABLE_INTERRUPTS();

        ಹಿಂತಿರುಗಿ xReturn;
    }

#ಆದರೆ_ಅಂತ್ಯ /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#ಆದರೆ ( configUSE_CO_ROUTINES == 1 )

    ತಳ_Type_t x_ಸರತಿ_CRReceive( ಸರತಿ_Handle_t x_ಸರತಿ,
                                ಶೂನ್ಯ * pv_ಬಫರ್,
                                TickType_t x_Ticks_ಗಮ್ಯ_Wait )
    {
        ತಳ_Type_t xReturn;
        ಸರತಿ_t * ಸ್ಥಿರಾಂಕ px_ಸರತಿ = x_ಸರತಿ;

        /* If the queue is already empty we may have to block.  A critical section
         * is required to prevent an interrupt adding something to the queue
         * between the check to see if the queue is empty and blocking on the queue. */
        ಪೋರ್ಟ್_DISABLE_INTERRUPTS();
        {
            ಆದರೆ( px_ಸರತಿ->ux_Messages_ಕಾಯುವಿಕೆ == ( UBaseType_t ) 0 )
            {
                /* There are no messages in the queue, do we want to block or just
                 * leave with nothing? */
                ಆದರೆ( x_Ticks_ಗಮ್ಯ_Wait > ( TickType_t ) 0 )
                {
                    /* As this is a co-routine we cannot block directly, but return
                     * indicating that we need to block. */
                    v_Co_Routine_ಕೂಡಿಸು_ಗಮ್ಯ_Delayed_List( x_Ticks_ಗಮ್ಯ_Wait, &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive ) );
                    ಪೋರ್ಟ್_ENABLE_INTERRUPTS();
                    ಹಿಂತಿರುಗಿ err_ಸರತಿ_BLOCKED;
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    ಪೋರ್ಟ್_ENABLE_INTERRUPTS();
                    ಹಿಂತಿರುಗಿ err_ಸರತಿ_ತುಂಬಿದ;
                }
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        ಪೋರ್ಟ್_ENABLE_INTERRUPTS();

        ಪೋರ್ಟ್_DISABLE_INTERRUPTS();
        {
            ಆದರೆ( px_ಸರತಿ->ux_Messages_ಕಾಯುವಿಕೆ > ( UBaseType_t ) 0 )
            {
                /* Data is available from the queue. */
                px_ಸರತಿ->u.x_ಸರತಿ.pcReadFrom += px_ಸರತಿ->ux_ಅಂಶ_ಗಾತ್ರ;

                ಆದರೆ( px_ಸರತಿ->u.x_ಸರತಿ.pcReadFrom >= px_ಸರತಿ->u.x_ಸರತಿ.pc_ಬಾಲ )
                {
                    px_ಸರತಿ->u.x_ಸರತಿ.pcReadFrom = px_ಸರತಿ->pc_ತಲೆ;
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                --( px_ಸರತಿ->ux_Messages_ಕಾಯುವಿಕೆ );
                ( ಶೂನ್ಯ ) ಸ್ಮರಣೆ_ನಕಲು( ( ಶೂನ್ಯ * ) pv_ಬಫರ್, ( ಶೂನ್ಯ * ) px_ಸರತಿ->u.x_ಸರತಿ.pcReadFrom, ( ಚಿಹ್ನೆರಹಿತ ) px_ಸರತಿ->ux_ಅಂಶ_ಗಾತ್ರ );

                xReturn = pdPASS;

                /* Were any co-routines waiting for space to become available? */
                ಆದರೆ( list_LIST_IS_ಖಾಲಿಗಳು( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Send ) ) == pdFALSE )
                {
                    /* In this instance the co-routine could be placed directly
                     * into the ready list as we are within a critical section.
                     * Instead the same pending ready list mechanism is used as if
                     * the event were caused from within an interrupt. */
                    ಆದರೆ( xCoRoutineRemoveFromEventList( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Send ) ) != pdFALSE )
                    {
                        xReturn = err_ಸರತಿ_YIELD;
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
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                xReturn = pdFAIL;
            }
        }
        ಪೋರ್ಟ್_ENABLE_INTERRUPTS();

        ಹಿಂತಿರುಗಿ xReturn;
    }

#ಆದರೆ_ಅಂತ್ಯ /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#ಆದರೆ ( configUSE_CO_ROUTINES == 1 )

    ತಳ_Type_t x_ಸರತಿ_CRSend_From_ISR( ಸರತಿ_Handle_t x_ಸರತಿ,
                                    ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ * pv_ಅಂಶ_ಗಮ್ಯ_ಸರತಿ,
                                    ತಳ_Type_t xCoRoutinePreviouslyWoken )
    {
        ಸರತಿ_t * ಸ್ಥಿರಾಂಕ px_ಸರತಿ = x_ಸರತಿ;

        /* Cannot block within an ISR so if there is no space on the queue then
         * exit without doing anything. */
        ಆದರೆ( px_ಸರತಿ->ux_Messages_ಕಾಯುವಿಕೆ < px_ಸರತಿ->ux_ಉದ್ದಳತೆ )
        {
            prv_Copy_ದತ್ತಾಂಶ_ಗಮ್ಯ_ಸರತಿ( px_ಸರತಿ, pv_ಅಂಶ_ಗಮ್ಯ_ಸರತಿ, ಸರತಿ_SEND_ಗಮ್ಯ_ಹಿಂಭಾಗ );

            /* We only want to wake one co-routine per ISR, so check that a
             * co-routine has not already been woken. */
            ಆದರೆ( xCoRoutinePreviouslyWoken == pdFALSE )
            {
                ಆದರೆ( list_LIST_IS_ಖಾಲಿಗಳು( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive ) ) == pdFALSE )
                {
                    ಆದರೆ( xCoRoutineRemoveFromEventList( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive ) ) != pdFALSE )
                    {
                        ಹಿಂತಿರುಗಿ pdTRUE;
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

        ಹಿಂತಿರುಗಿ xCoRoutinePreviouslyWoken;
    }

#ಆದರೆ_ಅಂತ್ಯ /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#ಆದರೆ ( configUSE_CO_ROUTINES == 1 )

    ತಳ_Type_t x_ಸರತಿ_CRReceive_From_ISR( ಸರತಿ_Handle_t x_ಸರತಿ,
                                       ಶೂನ್ಯ * pv_ಬಫರ್,
                                       ತಳ_Type_t * pxCoRoutineWoken )
    {
        ತಳ_Type_t xReturn;
        ಸರತಿ_t * ಸ್ಥಿರಾಂಕ px_ಸರತಿ = x_ಸರತಿ;

        /* We cannot block from an ISR, so check there is data available. If
         * not then just leave without doing anything. */
        ಆದರೆ( px_ಸರತಿ->ux_Messages_ಕಾಯುವಿಕೆ > ( UBaseType_t ) 0 )
        {
            /* Copy the data from the queue. */
            px_ಸರತಿ->u.x_ಸರತಿ.pcReadFrom += px_ಸರತಿ->ux_ಅಂಶ_ಗಾತ್ರ;

            ಆದರೆ( px_ಸರತಿ->u.x_ಸರತಿ.pcReadFrom >= px_ಸರತಿ->u.x_ಸರತಿ.pc_ಬಾಲ )
            {
                px_ಸರತಿ->u.x_ಸರತಿ.pcReadFrom = px_ಸರತಿ->pc_ತಲೆ;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                mtCOVERAGE_TEST_MARKER();
            }

            --( px_ಸರತಿ->ux_Messages_ಕಾಯುವಿಕೆ );
            ( ಶೂನ್ಯ ) ಸ್ಮರಣೆ_ನಕಲು( ( ಶೂನ್ಯ * ) pv_ಬಫರ್, ( ಶೂನ್ಯ * ) px_ಸರತಿ->u.x_ಸರತಿ.pcReadFrom, ( ಚಿಹ್ನೆರಹಿತ ) px_ಸರತಿ->ux_ಅಂಶ_ಗಾತ್ರ );

            ಆದರೆ( ( *pxCoRoutineWoken ) == pdFALSE )
            {
                ಆದರೆ( list_LIST_IS_ಖಾಲಿಗಳು( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Send ) ) == pdFALSE )
                {
                    ಆದರೆ( xCoRoutineRemoveFromEventList( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Send ) ) != pdFALSE )
                    {
                        *pxCoRoutineWoken = pdTRUE;
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
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                mtCOVERAGE_TEST_MARKER();
            }

            xReturn = pdPASS;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            xReturn = pdFAIL;
        }

        ಹಿಂತಿರುಗಿ xReturn;
    }

#ಆದರೆ_ಅಂತ್ಯ /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#ಆದರೆ ( config_ಸರತಿ_REGISTRY_ಗಾತ್ರ > 0 )

    ಶೂನ್ಯ v_ಸರತಿ_ಕೂಡಿಸು_ಗಮ್ಯ_Registry( ಸರತಿ_Handle_t x_ಸರತಿ,
                              ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * pc_ಸರತಿ_Name ) /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
    {
        UBaseType_t ux;
        ಸರತಿ_Registry_ಅಂಶ_t * px_ನಮೂದು_ಗಮ್ಯ_Write = ಖಾಲಿ;

        configASSERT( x_ಸರತಿ );

        /* For SMP, we need to take the queue registry lock in case another
         * core updates the register simultaneously. */
        prvENTER_CRITICAL_SMP_ONLY( &x_ಸರತಿ_Registry_ಬೀಗ );
        {
            ಆದರೆ( pc_ಸರತಿ_Name != ಖಾಲಿ )
            {
                /* See if there is an empty space in the registry.  A NULL name denotes
                 * a free slot. */
                ಗಾಗಿ( ux = ( UBaseType_t ) 0U; ux < ( UBaseType_t ) config_ಸರತಿ_REGISTRY_ಗಾತ್ರ; ux++ )
                {
                    /* Replace an existing entry if the queue is already in the registry. */
                    ಆದರೆ( x_ಸರತಿ == x_ಸರತಿ_Registry[ ux ].xHandle )
                    {
                        px_ನಮೂದು_ಗಮ್ಯ_Write = &( x_ಸರತಿ_Registry[ ux ] );
                        ನಿಲ್ಲಿಸು;
                    }
                    /* Otherwise, store in the next empty location */
                    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ( px_ನಮೂದು_ಗಮ್ಯ_Write == ಖಾಲಿ ) && ( x_ಸರತಿ_Registry[ ux ].pc_ಸರತಿ_Name == ಖಾಲಿ ) )
                    {
                        px_ನಮೂದು_ಗಮ್ಯ_Write = &( x_ಸರತಿ_Registry[ ux ] );
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
            }

            ಆದರೆ( px_ನಮೂದು_ಗಮ್ಯ_Write != ಖಾಲಿ )
            {
                /* Store the information on this queue. */
                px_ನಮೂದು_ಗಮ್ಯ_Write->pc_ಸರತಿ_Name = pc_ಸರತಿ_Name;
                px_ನಮೂದು_ಗಮ್ಯ_Write->xHandle = x_ಸರತಿ;

                trace_ಸರತಿ_REGISTRY_ಕೂಡಿಸು( x_ಸರತಿ, pc_ಸರತಿ_Name );
            }
        }
        /* Release the previously taken queue registry lock. */
        prvEXIT_CRITICAL_SMP_ONLY( &x_ಸರತಿ_Registry_ಬೀಗ );
    }

#ಆದರೆ_ಅಂತ್ಯ /* configQUEUE_REGISTRY_SIZE */
/*-----------------------------------------------------------*/

#ಆದರೆ ( config_ಸರತಿ_REGISTRY_ಗಾತ್ರ > 0 )

    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * pc_ಸರತಿ_Get_Name( ಸರತಿ_Handle_t x_ಸರತಿ ) /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
    {
        UBaseType_t ux;
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * pcReturn = ಖಾಲಿ; /*lint !e971 Unqualified char types are allowed for strings and single characters only. */

        configASSERT( x_ಸರತಿ );

        /* For SMP, we need to take the queue registry lock in case another
         * core updates the register simultaneously. */
        prvENTER_CRITICAL_SMP_ONLY( &x_ಸರತಿ_Registry_ಬೀಗ );
        {
            /* Note there is nothing here to protect against another task adding or
             * removing entries from the registry while it is being searched. */

            ಗಾಗಿ( ux = ( UBaseType_t ) 0U; ux < ( UBaseType_t ) config_ಸರತಿ_REGISTRY_ಗಾತ್ರ; ux++ )
            {
                ಆದರೆ( x_ಸರತಿ_Registry[ ux ].xHandle == x_ಸರತಿ )
                {
                    pcReturn = x_ಸರತಿ_Registry[ ux ].pc_ಸರತಿ_Name;
                    ನಿಲ್ಲಿಸು;
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
        }
        /* Release the previously taken queue registry lock. */
        prvEXIT_CRITICAL_SMP_ONLY( &x_ಸರತಿ_Registry_ಬೀಗ );

        ಹಿಂತಿರುಗಿ pcReturn;
    } /*lint !e818 xQueue cannot be a pointer to const because it is a typedef. */

#ಆದರೆ_ಅಂತ್ಯ /* configQUEUE_REGISTRY_SIZE */
/*-----------------------------------------------------------*/

#ಆದರೆ ( config_ಸರತಿ_REGISTRY_ಗಾತ್ರ > 0 )

    ಶೂನ್ಯ v_ಸರತಿ_Unregister_ಸರತಿ( ಸರತಿ_Handle_t x_ಸರತಿ )
    {
        UBaseType_t ux;

        configASSERT( x_ಸರತಿ );

        /* For SMP, we need to take the queue registry lock in case another
         * core updates the register simultaneously. */
        prvENTER_CRITICAL_SMP_ONLY( &x_ಸರತಿ_Registry_ಬೀಗ );
        {
            /* See if the handle of the queue being unregistered in actually in the
             * registry. */
            ಗಾಗಿ( ux = ( UBaseType_t ) 0U; ux < ( UBaseType_t ) config_ಸರತಿ_REGISTRY_ಗಾತ್ರ; ux++ )
            {
                ಆದರೆ( x_ಸರತಿ_Registry[ ux ].xHandle == x_ಸರತಿ )
                {
                    /* Set the name to NULL to show that this slot if free again. */
                    x_ಸರತಿ_Registry[ ux ].pc_ಸರತಿ_Name = ಖಾಲಿ;

                    /* Set the handle to NULL to ensure the same queue handle cannot
                     * appear in the registry twice if it is added, removed, then
                     * added again. */
                    x_ಸರತಿ_Registry[ ux ].xHandle = ( ಸರತಿ_Handle_t ) 0;
                    ನಿಲ್ಲಿಸು;
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
        }
        /* Release the previously taken queue registry lock. */
        prvEXIT_CRITICAL_SMP_ONLY( &x_ಸರತಿ_Registry_ಬೀಗ );
    } /*lint !e818 xQueue could not be pointer to const because it is a typedef. */

#ಆದರೆ_ಅಂತ್ಯ /* configQUEUE_REGISTRY_SIZE */
/*-----------------------------------------------------------*/

#ಆದರೆ ( configUSE_TIMERS == 1 )

    ಶೂನ್ಯ v_ಸರತಿ_Wait_For_ಸಂದೇಶ_Restricted( ಸರತಿ_Handle_t x_ಸರತಿ,
                                         TickType_t x_Ticks_ಗಮ್ಯ_Wait,
                                         ಸ್ಥಿರಾಂಕ ತಳ_Type_t xWaitIndefinitely )
    {
        ಸರತಿ_t * ಸ್ಥಿರಾಂಕ px_ಸರತಿ = x_ಸರತಿ;

        /* This function should not be called by application code hence the
         * 'Restricted' in its name.  It is not part of the public API.  It is
         * designed for use by kernel code, and has special calling requirements.
         * It can result in vListInsert() being called on a list that can only
         * possibly ever have one item in it, so the list will be fast, but even
         * so it should be called with the scheduler locked and not from a critical
         * section. */

        /* For SMP, we need to take the queue's xQueueLock as we are about to
         * access the queue. */
        prvENTER_CRITICAL_SMP_ONLY( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ) );
        {
            #ಆದರೆ ( ಸರತಿ_USE_LOCKS == 1 )
            {
                /* Only do anything if there are no messages in the queue.  This function
                 *  will not actually cause the task to block, just place it on a blocked
                 *  list.  It will not block until the scheduler is unlocked - at which
                 *  time a yield will be performed.  If an item is added to the queue while
                 *  the queue is locked, and the calling task blocks on the queue, then the
                 *  calling task will be immediately unblocked when the queue is unlocked. */
                prv_ಬೀಗ_ಸರತಿ( px_ಸರತಿ );
            }
            #ಆದರೆ_ಅಂತ್ಯ /* queueUSE_LOCKS == 1 */

            ಆದರೆ( px_ಸರತಿ->ux_Messages_ಕಾಯುವಿಕೆ == ( UBaseType_t ) 0U )
            {
                /* There is nothing in the queue, block for the specified period. */
                v_Task_ಇರಿಸು_On_Event_List_Restricted( &( px_ಸರತಿ->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive ), x_Ticks_ಗಮ್ಯ_Wait, xWaitIndefinitely );
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                mtCOVERAGE_TEST_MARKER();
            }

            #ಆದರೆ ( ಸರತಿ_USE_LOCKS == 1 )
            {
                prv_Unlock_ಸರತಿ( px_ಸರತಿ );
            }
            #ಆದರೆ_ಅಂತ್ಯ /* queueUSE_LOCKS == 1 */
        }
        /* Release the previously taken xQueueLock. */
        prvEXIT_CRITICAL_SMP_ONLY( &( px_ಸರತಿ->x_ಸರತಿ_ಬೀಗ ) );
    }

#ಆದರೆ_ಅಂತ್ಯ /* configUSE_TIMERS */
/*-----------------------------------------------------------*/

#ಆದರೆ ( ( config_USE_ಸರತಿ_SETS == 1 ) && ( config_SUPPORT_DYNAMIC_ಹಂಚಿಕೆ == 1 ) )

    ಸರತಿ_ಗಣ_Handle_t x_ಸರತಿ_Create_ಗಣ( ಸ್ಥಿರಾಂಕ UBaseType_t ux_Event_ಸರತಿ_ಉದ್ದಳತೆ )
    {
        ಸರತಿ_ಗಣ_Handle_t px_ಸರತಿ;

        px_ಸರತಿ = x_ಸರತಿ_Generic_Create( ux_Event_ಸರತಿ_ಉದ್ದಳತೆ, ( UBaseType_t ) ಗಾತ್ರ( ಸರತಿ_t * ), ಸರತಿ_ಸರತಿ_TYPE_ಗಣ );

        ಹಿಂತಿರುಗಿ px_ಸರತಿ;
    }

#ಆದರೆ_ಅಂತ್ಯ /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#ಆದರೆ ( config_USE_ಸರತಿ_SETS == 1 )

    ತಳ_Type_t x_ಸರತಿ_ಕೂಡಿಸು_ಗಮ್ಯ_ಗಣ( ಸರತಿ_ಗಣ_Member_Handle_t x_ಸರತಿ_Or_Semaphore,
                               ಸರತಿ_ಗಣ_Handle_t x_ಸರತಿ_ಗಣ )
    {
        ತಳ_Type_t xReturn;

        taskENTER_CRITICAL( &( ( ( ಸರತಿ_t * ) x_ಸರತಿ_Or_Semaphore )->x_ಸರತಿ_ಬೀಗ ) );
        {
            ಆದರೆ( ( ( ಸರತಿ_t * ) x_ಸರತಿ_Or_Semaphore )->px_ಸರತಿ_ಗಣ_Container != ಖಾಲಿ )
            {
                /* Cannot add a queue/semaphore to more than one queue set. */
                xReturn = pdFAIL;
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ( ( ಸರತಿ_t * ) x_ಸರತಿ_Or_Semaphore )->ux_Messages_ಕಾಯುವಿಕೆ != ( UBaseType_t ) 0 )
            {
                /* Cannot add a queue/semaphore to a queue set if there are already
                 * items in the queue/semaphore. */
                xReturn = pdFAIL;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ( ( ಸರತಿ_t * ) x_ಸರತಿ_Or_Semaphore )->px_ಸರತಿ_ಗಣ_Container = x_ಸರತಿ_ಗಣ;
                xReturn = pdPASS;
            }
        }
        taskEXIT_CRITICAL( &( ( ( ಸರತಿ_t * ) x_ಸರತಿ_Or_Semaphore )->x_ಸರತಿ_ಬೀಗ ) );

        ಹಿಂತಿರುಗಿ xReturn;
    }

#ಆದರೆ_ಅಂತ್ಯ /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#ಆದರೆ ( config_USE_ಸರತಿ_SETS == 1 )

    ತಳ_Type_t x_ಸರತಿ_Remove_From_ಗಣ( ಸರತಿ_ಗಣ_Member_Handle_t x_ಸರತಿ_Or_Semaphore,
                                    ಸರತಿ_ಗಣ_Handle_t x_ಸರತಿ_ಗಣ )
    {
        ತಳ_Type_t xReturn;
        ಸರತಿ_t * ಸ್ಥಿರಾಂಕ px_ಸರತಿ_Or_Semaphore = ( ಸರತಿ_t * ) x_ಸರತಿ_Or_Semaphore;

        ಆದರೆ( px_ಸರತಿ_Or_Semaphore->px_ಸರತಿ_ಗಣ_Container != x_ಸರತಿ_ಗಣ )
        {
            /* The queue was not a member of the set. */
            xReturn = pdFAIL;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( px_ಸರತಿ_Or_Semaphore->ux_Messages_ಕಾಯುವಿಕೆ != ( UBaseType_t ) 0 )
        {
            /* It is dangerous to remove a queue from a set when the queue is
             * not empty because the queue set will still hold pending events for
             * the queue. */
            xReturn = pdFAIL;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            taskENTER_CRITICAL( &( ( ( ಸರತಿ_t * ) px_ಸರತಿ_Or_Semaphore )->x_ಸರತಿ_ಬೀಗ ) );
            {
                /* The queue is no longer contained in the set. */
                px_ಸರತಿ_Or_Semaphore->px_ಸರತಿ_ಗಣ_Container = ಖಾಲಿ;
            }
            taskEXIT_CRITICAL( &( ( ( ಸರತಿ_t * ) px_ಸರತಿ_Or_Semaphore )->x_ಸರತಿ_ಬೀಗ ) );
            xReturn = pdPASS;
        }

        ಹಿಂತಿರುಗಿ xReturn;
    } /*lint !e818 xQueueSet could not be declared as pointing to const as it is a typedef. */

#ಆದರೆ_ಅಂತ್ಯ /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#ಆದರೆ ( config_USE_ಸರತಿ_SETS == 1 )

    ಸರತಿ_ಗಣ_Member_Handle_t x_ಸರತಿ_Select_From_ಗಣ( ಸರತಿ_ಗಣ_Handle_t x_ಸರತಿ_ಗಣ,
                                                TickType_t ಸ್ಥಿರಾಂಕ x_Ticks_ಗಮ್ಯ_Wait )
    {
        ಸರತಿ_ಗಣ_Member_Handle_t xReturn = ಖಾಲಿ;

        ( ಶೂನ್ಯ ) x_ಸರತಿ_Receive( ( ಸರತಿ_Handle_t ) x_ಸರತಿ_ಗಣ, &xReturn, x_Ticks_ಗಮ್ಯ_Wait ); /*lint !e961 Casting from one typedef to another is not redundant. */
        ಹಿಂತಿರುಗಿ xReturn;
    }

#ಆದರೆ_ಅಂತ್ಯ /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#ಆದರೆ ( config_USE_ಸರತಿ_SETS == 1 )

    ಸರತಿ_ಗಣ_Member_Handle_t x_ಸರತಿ_Select_From_ಗಣ_From_ISR( ಸರತಿ_ಗಣ_Handle_t x_ಸರತಿ_ಗಣ )
    {
        ಸರತಿ_ಗಣ_Member_Handle_t xReturn = ಖಾಲಿ;

        ( ಶೂನ್ಯ ) x_ಸರತಿ_Receive_From_ISR( ( ಸರತಿ_Handle_t ) x_ಸರತಿ_ಗಣ, &xReturn, ಖಾಲಿ ); /*lint !e961 Casting from one typedef to another is not redundant. */
        ಹಿಂತಿರುಗಿ xReturn;
    }

#ಆದರೆ_ಅಂತ್ಯ /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#ಆದರೆ ( config_USE_ಸರತಿ_SETS == 1 )

    ಸ್ಥಿರ ತಳ_Type_t prv_Notify_ಸರತಿ_ಗಣ_Container( ಸ್ಥಿರಾಂಕ ಸರತಿ_t * ಸ್ಥಿರಾಂಕ px_ಸರತಿ )
    {
        ಸರತಿ_t * px_ಸರತಿ_ಗಣ_Container = px_ಸರತಿ->px_ಸರತಿ_ಗಣ_Container;
        ತಳ_Type_t xReturn = pdFALSE;

        /* This function must be called form a critical section. */

        /* The following line is not reachable in unit tests because every call
         * to prvNotifyQueueSetContainer is preceded by a check that
         * pxQueueSetContainer != NULL */
        configASSERT( px_ಸರತಿ_ಗಣ_Container ); /* LCOV_EXCL_BR_LINE */
        configASSERT( px_ಸರತಿ_ಗಣ_Container->ux_Messages_ಕಾಯುವಿಕೆ < px_ಸರತಿ_ಗಣ_Container->ux_ಉದ್ದಳತೆ );

        /* In SMP, queue sets have their own xQueueLock. Thus we need to also
         * acquire the queue set's xQueueLock before accessing it. */
        prv_ENTER_CRITICAL_ಸುರಕ್ಷಿತ_SMP_ONLY( &( px_ಸರತಿ_ಗಣ_Container->x_ಸರತಿ_ಬೀಗ ) );
        {
            ಆದರೆ( px_ಸರತಿ_ಗಣ_Container->ux_Messages_ಕಾಯುವಿಕೆ < px_ಸರತಿ_ಗಣ_Container->ux_ಉದ್ದಳತೆ )
            {
                #ಆದರೆ ( ಸರತಿ_USE_LOCKS == 1 )
                    ಸ್ಥಿರಾಂಕ int8_t c_Tx_ಬೀಗ = px_ಸರತಿ_ಗಣ_Container->c_Tx_ಬೀಗ;
                #ಇಲ್ಲದಿದ್ದರೆ
                    /* Queue locks not used, so we treat it as unlocked. */
                    ಸ್ಥಿರಾಂಕ int8_t c_Tx_ಬೀಗ = ಸರತಿ_UNLOCKED;
                #ಆದರೆ_ಅಂತ್ಯ /* queueUSE_LOCKS == 1 */

                trace_ಸರತಿ_ಗಣ_SEND( px_ಸರತಿ_ಗಣ_Container );

                /* The data copied is the handle of the queue that contains data. */
                xReturn = prv_Copy_ದತ್ತಾಂಶ_ಗಮ್ಯ_ಸರತಿ( px_ಸರತಿ_ಗಣ_Container, &px_ಸರತಿ, ಸರತಿ_SEND_ಗಮ್ಯ_ಹಿಂಭಾಗ );

                ಆದರೆ( c_Tx_ಬೀಗ == ಸರತಿ_UNLOCKED )
                {
                    ಆದರೆ( list_LIST_IS_ಖಾಲಿಗಳು( &( px_ಸರತಿ_ಗಣ_Container->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive ) ) == pdFALSE )
                    {
                        ಆದರೆ( xTaskRemoveFromEventList( &( px_ಸರತಿ_ಗಣ_Container->x_Tasks_ಕಾಯುವಿಕೆ_ಗಮ್ಯ_Receive ) ) != pdFALSE )
                        {
                            /* The task waiting has a higher priority. */
                            xReturn = pdTRUE;
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
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    #ಆದರೆ ( ಸರತಿ_USE_LOCKS == 1 )
                    {
                        prv_Increment_ಸರತಿ_Tx_ಬೀಗ( px_ಸರತಿ_ಗಣ_Container, c_Tx_ಬೀಗ );
                    }
                    #ಆದರೆ_ಅಂತ್ಯ /* queueUSE_LOCKS == 1 */
                }
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        /* Release the previously acquired queue set's xQueueLock. */
        prv_EXIT_CRITICAL_ಸುರಕ್ಷಿತ_SMP_ONLY( &( px_ಸರತಿ_ಗಣ_Container->x_ಸರತಿ_ಬೀಗ ) );

        ಹಿಂತಿರುಗಿ xReturn;
    }

#ಆದರೆ_ಅಂತ್ಯ /* configUSE_QUEUE_SETS */
