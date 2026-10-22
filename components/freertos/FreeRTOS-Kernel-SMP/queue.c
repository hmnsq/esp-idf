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
#وصف_ڪريو MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#ضمن "FreeRTOS.h"
#ضمن "task.h"
#ضمن "queue.h"

#إذا ( configUSE_CO_ROUTINES == 1 )
    #ضمن "croutine.h"
#جيڪڏهن_پورو

/* The MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#وصف_رد MPU_WRAPPERS_INCLUDED_FROM_API_FILE


/* Constants used with the cRxLock and cTxLock structure members. */
#وصف_ڪريو قطار_UNLOCKED             ( ( int8_t ) -1 )
#وصف_ڪريو قطار_LOCKED_UNMODIFIED    ( ( int8_t ) 0 )
#وصف_ڪريو قطار_INT8_MAX             ( ( int8_t ) 127 )

/* When the Queue_t structure is used to represent a base queue its pcHead and
 * pcTail members are used as pointers into the queue storage area.  When the
 * Queue_t structure is used to represent a mutex pcHead and pcTail pointers are
 * not necessary, and the pcHead pointer is set to NULL to indicate that the
 * structure instead holds a pointer to the mutex holder (if any).  Map alternative
 * names to the pcHead and structure member to ensure the readability of the code
 * is maintained.  The QueuePointers_t and SemaphoreData_t types are used to form
 * a union as their usage is mutually exclusive dependent on what the queue is
 * being used for. */
#وصف_ڪريو ux_قطار_Type               pc_سر
#وصف_ڪريو قطار_قطار_IS_MUTEX       عدم

تعريفنوع بنية قطار_اشارا
{
    int8_t * pc_پڇ;     /**< Points to the byte at the end of the queue storage area.  Once more byte is allocated than necessary to store the queue items, this is used as a marker. */
    int8_t * pcReadFrom; /**< Points to the last place that a queued item was read from when the structure is used as a queue. */
} قطار_اشارا_t;

تعريفنوع بنية Semaphore_معلومات
{
    TaskHandle_t xMutexHolder;        /**< The handle of the task that holds the mutex. */
    UBaseType_t ux_Recursive_Call_ڳڻپ; /**< Maintains a count of the number of times a recursive mutex has been recursively 'taken' when the structure is used as a mutex. */
} Semaphore_معلومات_t;

/* Semaphores do not actually store or copy data, so have an item size of
 * zero. */
#وصف_ڪريو قطار_SEMAPHORE_قطار_شي_ڊيگهائي    ( ( UBaseType_t ) 0 )
#وصف_ڪريو قطار_MUTEX_GIVE_بلاڪ_وقت          ( ( TickType_t ) 0U )

#إذا ( configUSE_PREEMPTION == 0 )

/* If the cooperative scheduler is being used then a yield should not be
 * performed just because a higher priority task has been woken. */
    #وصف_ڪريو قطار_YIELD_IF_USING_PREEMPTION()
#وإلا
    #إذا ( config_عدد_جو_CORES == 1 )
        #وصف_ڪريو قطار_YIELD_IF_USING_PREEMPTION()    پورٽ_YIELD_WITHIN_API()
    #وإلا /* #if ( configNUMBER_OF_CORES == 1 ) */
        #وصف_ڪريو قطار_YIELD_IF_USING_PREEMPTION()    vTaskYieldWithinAPI()
    #جيڪڏهن_پورو /* #if ( configNUMBER_OF_CORES == 1 ) */
#جيڪڏهن_پورو

/*
 * Definition of the queue used by the scheduler.
 * Items are queued by copy, not reference.  See the following link for the
 * rationale: https://www.FreeRTOS.org/Embedded-RTOS-Queues.html
 */
تعريفنوع بنية قطار_Definition /* The old naming convention is used to prevent breaking kernel aware debuggers. */
{
    int8_t * pc_سر;           /**< Points to the beginning of the queue storage area. */
    int8_t * pc_Write_منزل;        /**< Points to the free next place in the storage area. */

    اتحاد
    {
        قطار_اشارا_t x_قطار;     /**< Data required exclusively when this structure is used as a queue. */
        Semaphore_معلومات_t xSemaphore; /**< Data required exclusively when this structure is used as a semaphore. */
    } u;

    List_t x_Tasks_انتظار_منزل_Send;             /**< List of tasks that are blocked waiting to post onto this queue.  Stored in priority order. */
    List_t x_Tasks_انتظار_منزل_Receive;          /**< List of tasks that are blocked waiting to read from this queue.  Stored in priority order. */

    متطاير UBaseType_t ux_Messages_انتظار; /**< The number of items currently in the queue. */
    UBaseType_t ux_ڊيگهائي;                   /**< The length of the queue defined as the number of items it will hold, not the number of bytes. */
    UBaseType_t ux_شي_سائيز;                 /**< The size of each items that the queue will hold. */

    متطاير int8_t c_Rx_تالو;                /**< Stores the number of items received from the queue (removed from the queue) while the queue was locked.  Set to queueUNLOCKED when the queue is not locked. */
    متطاير int8_t c_Tx_تالو;                /**< Stores the number of items transmitted to the queue (added to the queue) while the queue was locked.  Set to queueUNLOCKED when the queue is not locked. */

    #إذا ( ( config_SUPPORT_STATIC_ورهاست == 1 ) && ( config_SUPPORT_DYNAMIC_ورهاست == 1 ) )
        uint8_t ucStaticallyAllocated; /**< Set to pdTRUE if the memory used by the queue was statically allocated to ensure no attempt is made to free the memory. */
    #جيڪڏهن_پورو

    #إذا ( config_USE_قطار_SETS == 1 )
        بنية قطار_Definition * px_قطار_سيٽ_Container;
    #جيڪڏهن_پورو

    #إذا ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_قطار_عدد;
        uint8_t uc_قطار_Type;
    #جيڪڏهن_پورو
} x_قطار_2;

/* The old xQUEUE name is maintained above then typedefed to the new Queue_t
 * name below to enable the use of older kernel aware debuggers. */
تعريفنوع x_قطار_2 قطار_t;

/*-----------------------------------------------------------*/

/*
 * The queue registry is just a means for kernel aware debuggers to locate
 * queue structures.  It has no other purpose so is an optional component.
 */
#إذا ( config_قطار_REGISTRY_سائيز > 0 )

/* The type stored within the queue registry array.  This allows a name
 * to be assigned to each queue making kernel aware debugging a little
 * more user friendly. */
    تعريفنوع بنية قطار_REGISTRY_شي
    {
        ثابت حرف * pc_قطار_Name;
        قطار_Handle_t xHandle;
    } x_قطار_Registry_شي;

/* The old xQueueRegistryItem name is maintained above then typedefed to the
 * new xQueueRegistryItem name below to enable the use of older kernel aware
 * debuggers. */
    تعريفنوع x_قطار_Registry_شي قطار_Registry_شي_t;

/* The queue registry is simply an array of QueueRegistryItem_t structures.
 * The pcQueueName member of a structure being NULL is indicative of the
 * array position being vacant. */

/* MISRA Ref 8.4.2 [Declaration shall be visible] */
/* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-84 */
/* coverity[misra_c_2012_rule_8_4_violation] */
    PRIVILEGED_معلومات قطار_Registry_شي_t x_قطار_Registry[ config_قطار_REGISTRY_سائيز ];

#جيڪڏهن_پورو /* configQUEUE_REGISTRY_SIZE */

/*
 * Unlocks a queue locked by a call to prvLockQueue.  Locking a queue does not
 * prevent an ISR from adding or removing items to the queue, but does prevent
 * an ISR from removing tasks from the queue event lists.  If an ISR finds a
 * queue is locked it will instead increment the appropriate queue lock count
 * to indicate that a task may require unblocking.  When the queue in unlocked
 * these lock counts are inspected, and the appropriate action taken.
 */
ساكن فراغ prv_Unlock_قطار( قطار_t * ثابت px_قطار ) PRIVILEGED_FUNCTION;

/*
 * Uses a critical section to determine if there is any data in a queue.
 *
 * @return pdTRUE if the queue contains no items, otherwise pdFALSE.
 */
ساكن بنياد_Type_t prv_Is_قطار_خالي_جايون( ثابت قطار_t * px_قطار ) PRIVILEGED_FUNCTION;

/*
 * Uses a critical section to determine if there is any space in a queue.
 *
 * @return pdTRUE if there is no space, otherwise pdFALSE;
 */
ساكن بنياد_Type_t prv_Is_قطار_ڀريل( ثابت قطار_t * px_قطار ) PRIVILEGED_FUNCTION;

/*
 * Copies an item into the queue, either at the front of the queue or the
 * back of the queue.
 */
ساكن بنياد_Type_t prv_Copy_معلومات_منزل_قطار( قطار_t * ثابت px_قطار,
                                      ثابت فراغ * pv_شي_منزل_قطار,
                                      ثابت بنياد_Type_t xPosition ) PRIVILEGED_FUNCTION;

/*
 * Copies an item out of a queue.
 */
ساكن فراغ prv_Copy_معلومات_From_قطار( قطار_t * ثابت px_قطار,
                                  فراغ * ثابت pv_بفر ) PRIVILEGED_FUNCTION;

#إذا ( config_USE_قطار_SETS == 1 )

/*
 * Checks to see if a queue is a member of a queue set, and if so, notifies
 * the queue set that the queue contains data.
 */
    ساكن بنياد_Type_t prv_Notify_قطار_سيٽ_Container( ثابت قطار_t * ثابت px_قطار ) PRIVILEGED_FUNCTION;
#جيڪڏهن_پورو

/*
 * Called after a Queue_t structure has been allocated either statically or
 * dynamically to fill in the structure's members.
 */
ساكن فراغ prv_Initialise_New_قطار( ثابت UBaseType_t ux_قطار_ڊيگهائي,
                                   ثابت UBaseType_t ux_شي_سائيز,
                                   uint8_t * puc_قطار_Storage,
                                   ثابت uint8_t uc_قطار_Type,
                                   قطار_t * px_New_قطار ) PRIVILEGED_FUNCTION;

/*
 * Mutexes are a special type of queue.  When a mutex is created, first the
 * queue is created, then prvInitialiseMutex() is called to configure the queue
 * as a mutex.
 */
#إذا ( configUSE_MUTEXES == 1 )
    ساكن فراغ prvInitialiseMutex( قطار_t * px_New_قطار ) PRIVILEGED_FUNCTION;
#جيڪڏهن_پورو

#إذا ( configUSE_MUTEXES == 1 )

/*
 * If a task waiting for a mutex causes the mutex holder to inherit a
 * priority, but the waiting task times out, then the holder should
 * disinherit the priority - but only down to the highest priority of any
 * other tasks that are waiting for the same mutex.  This function returns
 * that priority.
 */
    ساكن UBaseType_t prv_Get_Disinherit_ترجيح_After_Timeout( ثابت قطار_t * ثابت px_قطار ) PRIVILEGED_FUNCTION;
#جيڪڏهن_پورو
/*-----------------------------------------------------------*/

/*
 * Macro to mark a queue as locked.  Locking a queue prevents an ISR from
 * accessing the queue event lists.
 */
#وصف_ڪريو prv_تالو_قطار( px_قطار )                            \
    taskENTER_CRITICAL();                                  \
    {                                                      \
        إذا( ( px_قطار )->c_Rx_تالو == قطار_UNLOCKED )        \
        {                                                  \
            ( px_قطار )->c_Rx_تالو = قطار_LOCKED_UNMODIFIED; \
        }                                                  \
        إذا( ( px_قطار )->c_Tx_تالو == قطار_UNLOCKED )        \
        {                                                  \
            ( px_قطار )->c_Tx_تالو = قطار_LOCKED_UNMODIFIED; \
        }                                                  \
    }                                                      \
    taskEXIT_CRITICAL()

/*
 * Macro to increment cTxLock member of the queue data structure. It is
 * capped at the number of tasks in the system as we cannot unblock more
 * tasks than the number of tasks in the system.
 */
#وصف_ڪريو prv_Increment_قطار_Tx_تالو( px_قطار, c_Tx_تالو )                           \
    افعل {                                                                      \
        ثابت UBaseType_t ux_عدد_جو_Tasks = ux_Task_Get_عدد_جو_Tasks();         \
        إذا( ( UBaseType_t ) ( c_Tx_تالو ) < ux_عدد_جو_Tasks )                   \
        {                                                                     \
            configASSERT( ( c_Tx_تالو ) != قطار_INT8_MAX );                     \
            ( px_قطار )->c_Tx_تالو = ( int8_t ) ( ( c_Tx_تالو ) + ( int8_t ) 1 ); \
        }                                                                     \
    } طالما( 0 )

/*
 * Macro to increment cRxLock member of the queue data structure. It is
 * capped at the number of tasks in the system as we cannot unblock more
 * tasks than the number of tasks in the system.
 */
#وصف_ڪريو prv_Increment_قطار_Rx_تالو( px_قطار, c_Rx_تالو )                           \
    افعل {                                                                      \
        ثابت UBaseType_t ux_عدد_جو_Tasks = ux_Task_Get_عدد_جو_Tasks();         \
        إذا( ( UBaseType_t ) ( c_Rx_تالو ) < ux_عدد_جو_Tasks )                   \
        {                                                                     \
            configASSERT( ( c_Rx_تالو ) != قطار_INT8_MAX );                     \
            ( px_قطار )->c_Rx_تالو = ( int8_t ) ( ( c_Rx_تالو ) + ( int8_t ) 1 ); \
        }                                                                     \
    } طالما( 0 )
/*-----------------------------------------------------------*/

بنياد_Type_t x_قطار_Generic_Reset( قطار_Handle_t x_قطار,
                               بنياد_Type_t x_New_قطار )
{
    بنياد_Type_t xReturn = pdPASS;
    قطار_t * ثابت px_قطار = x_قطار;

    trace_ENTER_x_قطار_Generic_Reset( x_قطار, x_New_قطار );

    configASSERT( px_قطار );

    إذا( ( px_قطار != عدم ) &&
        ( px_قطار->ux_ڊيگهائي >= 1U ) &&
        /* Check for multiplication overflow. */
        ( ( SIZE_MAX / px_قطار->ux_ڊيگهائي ) >= px_قطار->ux_شي_سائيز ) )
    {
        taskENTER_CRITICAL();
        {
            px_قطار->u.x_قطار.pc_پڇ = px_قطار->pc_سر + ( px_قطار->ux_ڊيگهائي * px_قطار->ux_شي_سائيز );
            px_قطار->ux_Messages_انتظار = ( UBaseType_t ) 0U;
            px_قطار->pc_Write_منزل = px_قطار->pc_سر;
            px_قطار->u.x_قطار.pcReadFrom = px_قطار->pc_سر + ( ( px_قطار->ux_ڊيگهائي - 1U ) * px_قطار->ux_شي_سائيز );
            px_قطار->c_Rx_تالو = قطار_UNLOCKED;
            px_قطار->c_Tx_تالو = قطار_UNLOCKED;

            إذا( x_New_قطار == pdFALSE )
            {
                /* If there are tasks blocked waiting to read from the queue, then
                 * the tasks will remain blocked as after this function exits the queue
                 * will still be empty.  If there are tasks blocked waiting to write to
                 * the queue, then one should be unblocked as after this function exits
                 * it will be possible to write to it. */
                إذا( list_LIST_IS_خالي_جايون( &( px_قطار->x_Tasks_انتظار_منزل_Send ) ) == pdFALSE )
                {
                    إذا( xTaskRemoveFromEventList( &( px_قطار->x_Tasks_انتظار_منزل_Send ) ) != pdFALSE )
                    {
                        قطار_YIELD_IF_USING_PREEMPTION();
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
                vListInitialise( &( px_قطار->x_Tasks_انتظار_منزل_Send ) );
                vListInitialise( &( px_قطار->x_Tasks_انتظار_منزل_Receive ) );
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
    trace_RETURN_x_قطار_Generic_Reset( xReturn );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

#إذا ( config_SUPPORT_STATIC_ورهاست == 1 )

    قطار_Handle_t x_قطار_Generic_Create_Static( ثابت UBaseType_t ux_قطار_ڊيگهائي,
                                             ثابت UBaseType_t ux_شي_سائيز,
                                             uint8_t * puc_قطار_Storage,
                                             Static_قطار_t * px_Static_قطار,
                                             ثابت uint8_t uc_قطار_Type )
    {
        قطار_t * px_New_قطار = عدم;

        trace_ENTER_x_قطار_Generic_Create_Static( ux_قطار_ڊيگهائي, ux_شي_سائيز, puc_قطار_Storage, px_Static_قطار, uc_قطار_Type );

        /* The StaticQueue_t structure and the queue storage area must be
         * supplied. */
        configASSERT( px_Static_قطار );

        إذا( ( ux_قطار_ڊيگهائي > ( UBaseType_t ) 0 ) &&
            ( px_Static_قطار != عدم ) &&

            /* A queue storage area should be provided if the item size is not 0, and
             * should not be provided if the item size is 0. */
            ( !( ( puc_قطار_Storage != عدم ) && ( ux_شي_سائيز == 0U ) ) ) &&
            ( !( ( puc_قطار_Storage == عدم ) && ( ux_شي_سائيز != 0U ) ) ) )
        {
            #إذا ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticQueue_t or StaticSemaphore_t equals the size of
                 * the real queue and semaphore structures. */
                متطاير size_t x_سائيز = حجم( Static_قطار_t );

                /* This assertion cannot be branch covered in unit tests */
                configASSERT( x_سائيز == حجم( قطار_t ) ); /* LCOV_EXCL_BR_LINE */
                ( فراغ ) x_سائيز;                             /* Prevent unused variable warning when configASSERT() is not defined. */
            }
            #جيڪڏهن_پورو /* configASSERT_DEFINED */

            /* The address of a statically allocated queue was passed in, use it.
             * The address of a statically allocated storage area was also passed in
             * but is already set. */
            /* MISRA Ref 11.3.1 [Misaligned access] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
            /* coverity[misra_c_2012_rule_11_3_violation] */
            px_New_قطار = ( قطار_t * ) px_Static_قطار;

            #إذا ( config_SUPPORT_DYNAMIC_ورهاست == 1 )
            {
                /* Queues can be allocated either statically or dynamically, so
                 * note this queue was allocated statically in case the queue is
                 * later deleted. */
                px_New_قطار->ucStaticallyAllocated = pdTRUE;
            }
            #جيڪڏهن_پورو /* configSUPPORT_DYNAMIC_ALLOCATION */

            prv_Initialise_New_قطار( ux_قطار_ڊيگهائي, ux_شي_سائيز, puc_قطار_Storage, uc_قطار_Type, px_New_قطار );
        }
        وإلا
        {
            configASSERT( px_New_قطار );
            mtCOVERAGE_TEST_MARKER();
        }

        trace_RETURN_x_قطار_Generic_Create_Static( px_New_قطار );

        أرجع px_New_قطار;
    }

#جيڪڏهن_پورو /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

#إذا ( config_SUPPORT_STATIC_ورهاست == 1 )

    بنياد_Type_t x_قطار_Generic_Get_Static_Buffers( قطار_Handle_t x_قطار,
                                              uint8_t ** ppuc_قطار_Storage,
                                              Static_قطار_t ** ppx_Static_قطار )
    {
        بنياد_Type_t xReturn;
        قطار_t * ثابت px_قطار = x_قطار;

        trace_ENTER_x_قطار_Generic_Get_Static_Buffers( x_قطار, ppuc_قطار_Storage, ppx_Static_قطار );

        configASSERT( px_قطار );
        configASSERT( ppx_Static_قطار );

        #إذا ( config_SUPPORT_DYNAMIC_ورهاست == 1 )
        {
            /* Check if the queue was statically allocated. */
            إذا( px_قطار->ucStaticallyAllocated == ( uint8_t ) pdTRUE )
            {
                إذا( ppuc_قطار_Storage != عدم )
                {
                    *ppuc_قطار_Storage = ( uint8_t * ) px_قطار->pc_سر;
                }

                /* MISRA Ref 11.3.1 [Misaligned access] */
                /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
                /* coverity[misra_c_2012_rule_11_3_violation] */
                *ppx_Static_قطار = ( Static_قطار_t * ) px_قطار;
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
            إذا( ppuc_قطار_Storage != عدم )
            {
                *ppuc_قطار_Storage = ( uint8_t * ) px_قطار->pc_سر;
            }

            *ppx_Static_قطار = ( Static_قطار_t * ) px_قطار;
            xReturn = pdTRUE;
        }
        #جيڪڏهن_پورو /* configSUPPORT_DYNAMIC_ALLOCATION */

        trace_RETURN_x_قطار_Generic_Get_Static_Buffers( xReturn );

        أرجع xReturn;
    }

#جيڪڏهن_پورو /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

#إذا ( config_SUPPORT_DYNAMIC_ورهاست == 1 )

    قطار_Handle_t x_قطار_Generic_Create( ثابت UBaseType_t ux_قطار_ڊيگهائي,
                                       ثابت UBaseType_t ux_شي_سائيز,
                                       ثابت uint8_t uc_قطار_Type )
    {
        قطار_t * px_New_قطار = عدم;
        size_t x_قطار_سائيز_اندريون_بائيٽون;
        uint8_t * puc_قطار_Storage;

        trace_ENTER_x_قطار_Generic_Create( ux_قطار_ڊيگهائي, ux_شي_سائيز, uc_قطار_Type );

        إذا( ( ux_قطار_ڊيگهائي > ( UBaseType_t ) 0 ) &&
            /* Check for multiplication overflow. */
            ( ( SIZE_MAX / ux_قطار_ڊيگهائي ) >= ux_شي_سائيز ) &&
            /* Check for addition overflow. */
            ( ( UBaseType_t ) ( SIZE_MAX - حجم( قطار_t ) ) >= ( ux_قطار_ڊيگهائي * ux_شي_سائيز ) ) )
        {
            /* Allocate enough space to hold the maximum number of items that
             * can be in the queue at any time.  It is valid for uxItemSize to be
             * zero in the case the queue is used as a semaphore. */
            x_قطار_سائيز_اندريون_بائيٽون = ( size_t ) ( ( size_t ) ux_قطار_ڊيگهائي * ( size_t ) ux_شي_سائيز );

            /* MISRA Ref 11.5.1 [Malloc memory assignment] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
            /* coverity[misra_c_2012_rule_11_5_violation] */
            px_New_قطار = ( قطار_t * ) pv_پورٽ_Malloc( حجم( قطار_t ) + x_قطار_سائيز_اندريون_بائيٽون );

            إذا( px_New_قطار != عدم )
            {
                /* Jump past the queue structure to find the location of the queue
                 * storage area. */
                puc_قطار_Storage = ( uint8_t * ) px_New_قطار;
                puc_قطار_Storage += حجم( قطار_t );

                #إذا ( config_SUPPORT_STATIC_ورهاست == 1 )
                {
                    /* Queues can be created either statically or dynamically, so
                     * note this task was created dynamically in case it is later
                     * deleted. */
                    px_New_قطار->ucStaticallyAllocated = pdFALSE;
                }
                #جيڪڏهن_پورو /* configSUPPORT_STATIC_ALLOCATION */

                prv_Initialise_New_قطار( ux_قطار_ڊيگهائي, ux_شي_سائيز, puc_قطار_Storage, uc_قطار_Type, px_New_قطار );
            }
            وإلا
            {
                trace_قطار_CREATE_FAILED( uc_قطار_Type );
                mtCOVERAGE_TEST_MARKER();
            }
        }
        وإلا
        {
            configASSERT( px_New_قطار );
            mtCOVERAGE_TEST_MARKER();
        }

        trace_RETURN_x_قطار_Generic_Create( px_New_قطار );

        أرجع px_New_قطار;
    }

#جيڪڏهن_پورو /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

ساكن فراغ prv_Initialise_New_قطار( ثابت UBaseType_t ux_قطار_ڊيگهائي,
                                   ثابت UBaseType_t ux_شي_سائيز,
                                   uint8_t * puc_قطار_Storage,
                                   ثابت uint8_t uc_قطار_Type,
                                   قطار_t * px_New_قطار )
{
    /* Remove compiler warnings about unused parameters should
     * configUSE_TRACE_FACILITY not be set to 1. */
    ( فراغ ) uc_قطار_Type;

    إذا( ux_شي_سائيز == ( UBaseType_t ) 0 )
    {
        /* No RAM was allocated for the queue storage area, but PC head cannot
         * be set to NULL because NULL is used as a key to say the queue is used as
         * a mutex.  Therefore just set pcHead to point to the queue as a benign
         * value that is known to be within the memory map. */
        px_New_قطار->pc_سر = ( int8_t * ) px_New_قطار;
    }
    وإلا
    {
        /* Set the head to the start of the queue storage area. */
        px_New_قطار->pc_سر = ( int8_t * ) puc_قطار_Storage;
    }

    /* Initialise the queue members as described where the queue type is
     * defined. */
    px_New_قطار->ux_ڊيگهائي = ux_قطار_ڊيگهائي;
    px_New_قطار->ux_شي_سائيز = ux_شي_سائيز;
    ( فراغ ) x_قطار_Generic_Reset( px_New_قطار, pdTRUE );

    #إذا ( configUSE_TRACE_FACILITY == 1 )
    {
        px_New_قطار->uc_قطار_Type = uc_قطار_Type;
    }
    #جيڪڏهن_پورو /* configUSE_TRACE_FACILITY */

    #إذا ( config_USE_قطار_SETS == 1 )
    {
        px_New_قطار->px_قطار_سيٽ_Container = عدم;
    }
    #جيڪڏهن_پورو /* configUSE_QUEUE_SETS */

    trace_قطار_CREATE( px_New_قطار );
}
/*-----------------------------------------------------------*/

#إذا ( configUSE_MUTEXES == 1 )

    ساكن فراغ prvInitialiseMutex( قطار_t * px_New_قطار )
    {
        إذا( px_New_قطار != عدم )
        {
            /* The queue create function will set all the queue structure members
            * correctly for a generic queue, but this function is creating a
            * mutex.  Overwrite those members that need to be set differently -
            * in particular the information required for priority inheritance. */
            px_New_قطار->u.xSemaphore.xMutexHolder = عدم;
            px_New_قطار->ux_قطار_Type = قطار_قطار_IS_MUTEX;

            /* In case this is a recursive mutex. */
            px_New_قطار->u.xSemaphore.ux_Recursive_Call_ڳڻپ = 0;

            traceCREATE_MUTEX( px_New_قطار );

            /* Start with the semaphore in the expected state. */
            ( فراغ ) x_قطار_Generic_Send( px_New_قطار, عدم, ( TickType_t ) 0U, قطار_SEND_منزل_پٺ );
        }
        وإلا
        {
            traceCREATE_MUTEX_FAILED();
        }
    }

#جيڪڏهن_پورو /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

#إذا ( ( configUSE_MUTEXES == 1 ) && ( config_SUPPORT_DYNAMIC_ورهاست == 1 ) )

    قطار_Handle_t x_قطار_Create_Mutex( ثابت uint8_t uc_قطار_Type )
    {
        قطار_Handle_t x_New_قطار;
        ثابت UBaseType_t ux_Mutex_ڊيگهائي = ( UBaseType_t ) 1, ux_Mutex_سائيز = ( UBaseType_t ) 0;

        trace_ENTER_x_قطار_Create_Mutex( uc_قطار_Type );

        x_New_قطار = x_قطار_Generic_Create( ux_Mutex_ڊيگهائي, ux_Mutex_سائيز, uc_قطار_Type );
        prvInitialiseMutex( ( قطار_t * ) x_New_قطار );

        trace_RETURN_x_قطار_Create_Mutex( x_New_قطار );

        أرجع x_New_قطار;
    }

#جيڪڏهن_پورو /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

#إذا ( ( configUSE_MUTEXES == 1 ) && ( config_SUPPORT_STATIC_ورهاست == 1 ) )

    قطار_Handle_t x_قطار_Create_Mutex_Static( ثابت uint8_t uc_قطار_Type,
                                           Static_قطار_t * px_Static_قطار )
    {
        قطار_Handle_t x_New_قطار;
        ثابت UBaseType_t ux_Mutex_ڊيگهائي = ( UBaseType_t ) 1, ux_Mutex_سائيز = ( UBaseType_t ) 0;

        trace_ENTER_x_قطار_Create_Mutex_Static( uc_قطار_Type, px_Static_قطار );

        /* Prevent compiler warnings about unused parameters if
         * configUSE_TRACE_FACILITY does not equal 1. */
        ( فراغ ) uc_قطار_Type;

        x_New_قطار = x_قطار_Generic_Create_Static( ux_Mutex_ڊيگهائي, ux_Mutex_سائيز, عدم, px_Static_قطار, uc_قطار_Type );
        prvInitialiseMutex( ( قطار_t * ) x_New_قطار );

        trace_RETURN_x_قطار_Create_Mutex_Static( x_New_قطار );

        أرجع x_New_قطار;
    }

#جيڪڏهن_پورو /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

#إذا ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) )

    TaskHandle_t x_قطار_Get_Mutex_Holder( قطار_Handle_t xSemaphore )
    {
        TaskHandle_t pxReturn;
        قطار_t * ثابت pxSemaphore = ( قطار_t * ) xSemaphore;

        trace_ENTER_x_قطار_Get_Mutex_Holder( xSemaphore );

        configASSERT( xSemaphore );

        /* This function is called by xSemaphoreGetMutexHolder(), and should not
         * be called directly.  Note:  This is a good way of determining if the
         * calling task is the mutex holder, but not a good way of determining the
         * identity of the mutex holder, as the holder may change between the
         * following critical section exiting and the function returning. */
        taskENTER_CRITICAL();
        {
            إذا( pxSemaphore->ux_قطار_Type == قطار_قطار_IS_MUTEX )
            {
                pxReturn = pxSemaphore->u.xSemaphore.xMutexHolder;
            }
            وإلا
            {
                pxReturn = عدم;
            }
        }
        taskEXIT_CRITICAL();

        trace_RETURN_x_قطار_Get_Mutex_Holder( pxReturn );

        أرجع pxReturn;
    }

#جيڪڏهن_پورو /* if ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) ) */
/*-----------------------------------------------------------*/

#إذا ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) )

    TaskHandle_t x_قطار_Get_Mutex_Holder_From_ISR( قطار_Handle_t xSemaphore )
    {
        TaskHandle_t pxReturn;

        trace_ENTER_x_قطار_Get_Mutex_Holder_From_ISR( xSemaphore );

        configASSERT( xSemaphore );

        /* Mutexes cannot be used in interrupt service routines, so the mutex
         * holder should not change in an ISR, and therefore a critical section is
         * not required here. */
        إذا( ( ( قطار_t * ) xSemaphore )->ux_قطار_Type == قطار_قطار_IS_MUTEX )
        {
            pxReturn = ( ( قطار_t * ) xSemaphore )->u.xSemaphore.xMutexHolder;
        }
        وإلا
        {
            pxReturn = عدم;
        }

        trace_RETURN_x_قطار_Get_Mutex_Holder_From_ISR( pxReturn );

        أرجع pxReturn;
    }

#جيڪڏهن_پورو /* if ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) ) */
/*-----------------------------------------------------------*/

#إذا ( configUSE_RECURSIVE_MUTEXES == 1 )

    بنياد_Type_t x_قطار_Give_Mutex_Recursive( قطار_Handle_t xMutex )
    {
        بنياد_Type_t xReturn;
        قطار_t * ثابت pxMutex = ( قطار_t * ) xMutex;

        trace_ENTER_x_قطار_Give_Mutex_Recursive( xMutex );

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
            ( pxMutex->u.xSemaphore.ux_Recursive_Call_ڳڻپ )--;

            /* Has the recursive call count unwound to 0? */
            إذا( pxMutex->u.xSemaphore.ux_Recursive_Call_ڳڻپ == ( UBaseType_t ) 0 )
            {
                /* Return the mutex.  This will automatically unblock any other
                 * task that might be waiting to access the mutex. */
                ( فراغ ) x_قطار_Generic_Send( pxMutex, عدم, قطار_MUTEX_GIVE_بلاڪ_وقت, قطار_SEND_منزل_پٺ );
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

        trace_RETURN_x_قطار_Give_Mutex_Recursive( xReturn );

        أرجع xReturn;
    }

#جيڪڏهن_پورو /* configUSE_RECURSIVE_MUTEXES */
/*-----------------------------------------------------------*/

#إذا ( configUSE_RECURSIVE_MUTEXES == 1 )

    بنياد_Type_t x_قطار_Take_Mutex_Recursive( قطار_Handle_t xMutex,
                                         TickType_t x_Ticks_منزل_Wait )
    {
        بنياد_Type_t xReturn;
        قطار_t * ثابت pxMutex = ( قطار_t * ) xMutex;

        trace_ENTER_x_قطار_Take_Mutex_Recursive( xMutex, x_Ticks_منزل_Wait );

        configASSERT( pxMutex );

        /* Comments regarding mutual exclusion as per those within
         * xQueueGiveMutexRecursive(). */

        traceTAKE_MUTEX_RECURSIVE( pxMutex );

        إذا( pxMutex->u.xSemaphore.xMutexHolder == xTaskGetCurrentTaskHandle() )
        {
            ( pxMutex->u.xSemaphore.ux_Recursive_Call_ڳڻپ )++;
            xReturn = pdPASS;
        }
        وإلا
        {
            xReturn = x_قطار_Semaphore_Take( pxMutex, x_Ticks_منزل_Wait );

            /* pdPASS will only be returned if the mutex was successfully
             * obtained.  The calling task may have entered the Blocked state
             * before reaching here. */
            إذا( xReturn != pdFAIL )
            {
                ( pxMutex->u.xSemaphore.ux_Recursive_Call_ڳڻپ )++;
            }
            وإلا
            {
                traceTAKE_MUTEX_RECURSIVE_FAILED( pxMutex );
            }
        }

        trace_RETURN_x_قطار_Take_Mutex_Recursive( xReturn );

        أرجع xReturn;
    }

#جيڪڏهن_پورو /* configUSE_RECURSIVE_MUTEXES */
/*-----------------------------------------------------------*/

#إذا ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( config_SUPPORT_STATIC_ورهاست == 1 ) )

    قطار_Handle_t x_قطار_Create_Counting_Semaphore_Static( ثابت UBaseType_t ux_Max_ڳڻپ,
                                                       ثابت UBaseType_t ux_Initial_ڳڻپ,
                                                       Static_قطار_t * px_Static_قطار )
    {
        قطار_Handle_t xHandle = عدم;

        trace_ENTER_x_قطار_Create_Counting_Semaphore_Static( ux_Max_ڳڻپ, ux_Initial_ڳڻپ, px_Static_قطار );

        إذا( ( ux_Max_ڳڻپ != 0U ) &&
            ( ux_Initial_ڳڻپ <= ux_Max_ڳڻپ ) )
        {
            xHandle = x_قطار_Generic_Create_Static( ux_Max_ڳڻپ, قطار_SEMAPHORE_قطار_شي_ڊيگهائي, عدم, px_Static_قطار, قطار_قطار_TYPE_COUNTING_SEMAPHORE );

            إذا( xHandle != عدم )
            {
                ( ( قطار_t * ) xHandle )->ux_Messages_انتظار = ux_Initial_ڳڻپ;

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

        trace_RETURN_x_قطار_Create_Counting_Semaphore_Static( xHandle );

        أرجع xHandle;
    }

#جيڪڏهن_پورو /* ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) ) */
/*-----------------------------------------------------------*/

#إذا ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( config_SUPPORT_DYNAMIC_ورهاست == 1 ) )

    قطار_Handle_t x_قطار_Create_Counting_Semaphore( ثابت UBaseType_t ux_Max_ڳڻپ,
                                                 ثابت UBaseType_t ux_Initial_ڳڻپ )
    {
        قطار_Handle_t xHandle = عدم;

        trace_ENTER_x_قطار_Create_Counting_Semaphore( ux_Max_ڳڻپ, ux_Initial_ڳڻپ );

        إذا( ( ux_Max_ڳڻپ != 0U ) &&
            ( ux_Initial_ڳڻپ <= ux_Max_ڳڻپ ) )
        {
            xHandle = x_قطار_Generic_Create( ux_Max_ڳڻپ, قطار_SEMAPHORE_قطار_شي_ڊيگهائي, قطار_قطار_TYPE_COUNTING_SEMAPHORE );

            إذا( xHandle != عدم )
            {
                ( ( قطار_t * ) xHandle )->ux_Messages_انتظار = ux_Initial_ڳڻپ;

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

        trace_RETURN_x_قطار_Create_Counting_Semaphore( xHandle );

        أرجع xHandle;
    }

#جيڪڏهن_پورو /* ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) ) */
/*-----------------------------------------------------------*/

بنياد_Type_t x_قطار_Generic_Send( قطار_Handle_t x_قطار,
                              ثابت فراغ * ثابت pv_شي_منزل_قطار,
                              TickType_t x_Ticks_منزل_Wait,
                              ثابت بنياد_Type_t xCopyPosition )
{
    بنياد_Type_t x_داخلا_وقت_سيٽ = pdFALSE, xYieldRequired;
    وقت_ٻاهريون_t x_وقت_ٻاهريون_2;
    قطار_t * ثابت px_قطار = x_قطار;

    trace_ENTER_x_قطار_Generic_Send( x_قطار, pv_شي_منزل_قطار, x_Ticks_منزل_Wait, xCopyPosition );

    configASSERT( px_قطار );
    configASSERT( !( ( pv_شي_منزل_قطار == عدم ) && ( px_قطار->ux_شي_سائيز != ( UBaseType_t ) 0U ) ) );
    configASSERT( !( ( xCopyPosition == قطار_OVERWRITE ) && ( px_قطار->ux_ڊيگهائي != 1 ) ) );
    #إذا ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_منزل_Wait != 0 ) ) );
    }
    #جيڪڏهن_پورو

    لكل( ; ; )
    {
        taskENTER_CRITICAL();
        {
            /* Is there room on the queue now?  The running task must be the
             * highest priority task wanting to access the queue.  If the head item
             * in the queue is to be overwritten then it does not matter if the
             * queue is full. */
            إذا( ( px_قطار->ux_Messages_انتظار < px_قطار->ux_ڊيگهائي ) || ( xCopyPosition == قطار_OVERWRITE ) )
            {
                trace_قطار_SEND( px_قطار );

                #إذا ( config_USE_قطار_SETS == 1 )
                {
                    ثابت UBaseType_t ux_اڳيون_Messages_انتظار = px_قطار->ux_Messages_انتظار;

                    xYieldRequired = prv_Copy_معلومات_منزل_قطار( px_قطار, pv_شي_منزل_قطار, xCopyPosition );

                    إذا( px_قطار->px_قطار_سيٽ_Container != عدم )
                    {
                        إذا( ( xCopyPosition == قطار_OVERWRITE ) && ( ux_اڳيون_Messages_انتظار != ( UBaseType_t ) 0 ) )
                        {
                            /* Do not notify the queue set as an existing item
                             * was overwritten in the queue so the number of items
                             * in the queue has not changed. */
                            mtCOVERAGE_TEST_MARKER();
                        }
                        وإلا إذا( prv_Notify_قطار_سيٽ_Container( px_قطار ) != pdFALSE )
                        {
                            /* The queue is a member of a queue set, and posting
                             * to the queue set caused a higher priority task to
                             * unblock. A context switch is required. */
                            قطار_YIELD_IF_USING_PREEMPTION();
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
                        إذا( list_LIST_IS_خالي_جايون( &( px_قطار->x_Tasks_انتظار_منزل_Receive ) ) == pdFALSE )
                        {
                            إذا( xTaskRemoveFromEventList( &( px_قطار->x_Tasks_انتظار_منزل_Receive ) ) != pdFALSE )
                            {
                                /* The unblocked task has a priority higher than
                                 * our own so yield immediately.  Yes it is ok to
                                 * do this from within the critical section - the
                                 * kernel takes care of that. */
                                قطار_YIELD_IF_USING_PREEMPTION();
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
                            قطار_YIELD_IF_USING_PREEMPTION();
                        }
                        وإلا
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                }
                #وإلا /* configUSE_QUEUE_SETS */
                {
                    xYieldRequired = prv_Copy_معلومات_منزل_قطار( px_قطار, pv_شي_منزل_قطار, xCopyPosition );

                    /* If there was a task waiting for data to arrive on the
                     * queue then unblock it now. */
                    إذا( list_LIST_IS_خالي_جايون( &( px_قطار->x_Tasks_انتظار_منزل_Receive ) ) == pdFALSE )
                    {
                        إذا( xTaskRemoveFromEventList( &( px_قطار->x_Tasks_انتظار_منزل_Receive ) ) != pdFALSE )
                        {
                            /* The unblocked task has a priority higher than
                             * our own so yield immediately.  Yes it is ok to do
                             * this from within the critical section - the kernel
                             * takes care of that. */
                            قطار_YIELD_IF_USING_PREEMPTION();
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
                        قطار_YIELD_IF_USING_PREEMPTION();
                    }
                    وإلا
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #جيڪڏهن_پورو /* configUSE_QUEUE_SETS */

                taskEXIT_CRITICAL();

                trace_RETURN_x_قطار_Generic_Send( pdPASS );

                أرجع pdPASS;
            }
            وإلا
            {
                إذا( x_Ticks_منزل_Wait == ( TickType_t ) 0 )
                {
                    /* The queue was full and no block time is specified (or
                     * the block time has expired) so leave now. */
                    taskEXIT_CRITICAL();

                    /* Return to the original privilege level before exiting
                     * the function. */
                    trace_قطار_SEND_FAILED( px_قطار );
                    trace_RETURN_x_قطار_Generic_Send( err_قطار_ڀريل );

                    أرجع err_قطار_ڀريل;
                }
                وإلا إذا( x_داخلا_وقت_سيٽ == pdFALSE )
                {
                    /* The queue was full and a block time was specified so
                     * configure the timeout structure. */
                    v_Task_اندروني_سيٽ_وقت_ٻاهريون_State( &x_وقت_ٻاهريون_2 );
                    x_داخلا_وقت_سيٽ = pdTRUE;
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
        prv_تالو_قطار( px_قطار );

        /* Update the timeout state to see if it has expired yet. */
        إذا( x_Task_Check_For_وقت_ٻاهريون( &x_وقت_ٻاهريون_2, &x_Ticks_منزل_Wait ) == pdFALSE )
        {
            إذا( prv_Is_قطار_ڀريل( px_قطار ) != pdFALSE )
            {
                trace_BLOCKING_ON_قطار_SEND( px_قطار );
                v_Task_رکو_On_Event_List( &( px_قطار->x_Tasks_انتظار_منزل_Send ), x_Ticks_منزل_Wait );

                /* Unlocking the queue means queue events can effect the
                 * event list. It is possible that interrupts occurring now
                 * remove this task from the event list again - but as the
                 * scheduler is suspended the task will go onto the pending
                 * ready list instead of the actual ready list. */
                prv_Unlock_قطار( px_قطار );

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
                prv_Unlock_قطار( px_قطار );
                ( فراغ ) xTaskResumeAll();
            }
        }
        وإلا
        {
            /* The timeout has expired. */
            prv_Unlock_قطار( px_قطار );
            ( فراغ ) xTaskResumeAll();

            trace_قطار_SEND_FAILED( px_قطار );
            trace_RETURN_x_قطار_Generic_Send( err_قطار_ڀريل );

            أرجع err_قطار_ڀريل;
        }
    }
}
/*-----------------------------------------------------------*/

بنياد_Type_t x_قطار_Generic_Send_From_ISR( قطار_Handle_t x_قطار,
                                     ثابت فراغ * ثابت pv_شي_منزل_قطار,
                                     بنياد_Type_t * ثابت px_Higher_ترجيح_Task_Woken,
                                     ثابت بنياد_Type_t xCopyPosition )
{
    بنياد_Type_t xReturn;
    UBaseType_t ux_Saved_مداخلت_حالت;
    قطار_t * ثابت px_قطار = x_قطار;

    trace_ENTER_x_قطار_Generic_Send_From_ISR( x_قطار, pv_شي_منزل_قطار, px_Higher_ترجيح_Task_Woken, xCopyPosition );

    configASSERT( px_قطار );
    configASSERT( !( ( pv_شي_منزل_قطار == عدم ) && ( px_قطار->ux_شي_سائيز != ( UBaseType_t ) 0U ) ) );
    configASSERT( !( ( xCopyPosition == قطار_OVERWRITE ) && ( px_قطار->ux_ڊيگهائي != 1 ) ) );

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
    پورٽ_ASSERT_IF_مداخلت_ترجيح_INVALID();

    /* Similar to xQueueGenericSend, except without blocking if there is no room
     * in the queue.  Also don't directly wake a task that was blocked on a queue
     * read, instead return a flag to say whether a context switch is required or
     * not (i.e. has a task with a higher priority than us been woken by this
     * post). */
    /* MISRA Ref 4.7.1 [Return value shall be checked] */
    /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
    /* coverity[misra_c_2012_directive_4_7_violation] */
    ux_Saved_مداخلت_حالت = ( UBaseType_t ) taskENTER_CRITICAL_FROM_ISR();
    {
        إذا( ( px_قطار->ux_Messages_انتظار < px_قطار->ux_ڊيگهائي ) || ( xCopyPosition == قطار_OVERWRITE ) )
        {
            ثابت int8_t c_Tx_تالو = px_قطار->c_Tx_تالو;
            ثابت UBaseType_t ux_اڳيون_Messages_انتظار = px_قطار->ux_Messages_انتظار;

            trace_قطار_SEND_FROM_ISR( px_قطار );

            /* Semaphores use xQueueGiveFromISR(), so pxQueue will not be a
             *  semaphore or mutex.  That means prvCopyDataToQueue() cannot result
             *  in a task disinheriting a priority and prvCopyDataToQueue() can be
             *  called here even though the disinherit function does not check if
             *  the scheduler is suspended before accessing the ready lists. */
            ( فراغ ) prv_Copy_معلومات_منزل_قطار( px_قطار, pv_شي_منزل_قطار, xCopyPosition );

            /* The event list is not altered if the queue is locked.  This will
             * be done when the queue is unlocked later. */
            إذا( c_Tx_تالو == قطار_UNLOCKED )
            {
                #إذا ( config_USE_قطار_SETS == 1 )
                {
                    إذا( px_قطار->px_قطار_سيٽ_Container != عدم )
                    {
                        إذا( ( xCopyPosition == قطار_OVERWRITE ) && ( ux_اڳيون_Messages_انتظار != ( UBaseType_t ) 0 ) )
                        {
                            /* Do not notify the queue set as an existing item
                             * was overwritten in the queue so the number of items
                             * in the queue has not changed. */
                            mtCOVERAGE_TEST_MARKER();
                        }
                        وإلا إذا( prv_Notify_قطار_سيٽ_Container( px_قطار ) != pdFALSE )
                        {
                            /* The queue is a member of a queue set, and posting
                             * to the queue set caused a higher priority task to
                             * unblock.  A context switch is required. */
                            إذا( px_Higher_ترجيح_Task_Woken != عدم )
                            {
                                *px_Higher_ترجيح_Task_Woken = pdTRUE;
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
                        إذا( list_LIST_IS_خالي_جايون( &( px_قطار->x_Tasks_انتظار_منزل_Receive ) ) == pdFALSE )
                        {
                            إذا( xTaskRemoveFromEventList( &( px_قطار->x_Tasks_انتظار_منزل_Receive ) ) != pdFALSE )
                            {
                                /* The task waiting has a higher priority so
                                 *  record that a context switch is required. */
                                إذا( px_Higher_ترجيح_Task_Woken != عدم )
                                {
                                    *px_Higher_ترجيح_Task_Woken = pdTRUE;
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
                    إذا( list_LIST_IS_خالي_جايون( &( px_قطار->x_Tasks_انتظار_منزل_Receive ) ) == pdFALSE )
                    {
                        إذا( xTaskRemoveFromEventList( &( px_قطار->x_Tasks_انتظار_منزل_Receive ) ) != pdFALSE )
                        {
                            /* The task waiting has a higher priority so record that a
                             * context switch is required. */
                            إذا( px_Higher_ترجيح_Task_Woken != عدم )
                            {
                                *px_Higher_ترجيح_Task_Woken = pdTRUE;
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
                    ( فراغ ) ux_اڳيون_Messages_انتظار;
                }
                #جيڪڏهن_پورو /* configUSE_QUEUE_SETS */
            }
            وإلا
            {
                /* Increment the lock count so the task that unlocks the queue
                 * knows that data was posted while it was locked. */
                prv_Increment_قطار_Tx_تالو( px_قطار, c_Tx_تالو );
            }

            xReturn = pdPASS;
        }
        وإلا
        {
            trace_قطار_SEND_FROM_ISR_FAILED( px_قطار );
            xReturn = err_قطار_ڀريل;
        }
    }
    taskEXIT_CRITICAL_FROM_ISR( ux_Saved_مداخلت_حالت );

    trace_RETURN_x_قطار_Generic_Send_From_ISR( xReturn );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

بنياد_Type_t x_قطار_Give_From_ISR( قطار_Handle_t x_قطار,
                              بنياد_Type_t * ثابت px_Higher_ترجيح_Task_Woken )
{
    بنياد_Type_t xReturn;
    UBaseType_t ux_Saved_مداخلت_حالت;
    قطار_t * ثابت px_قطار = x_قطار;

    trace_ENTER_x_قطار_Give_From_ISR( x_قطار, px_Higher_ترجيح_Task_Woken );

    /* Similar to xQueueGenericSendFromISR() but used with semaphores where the
     * item size is 0.  Don't directly wake a task that was blocked on a queue
     * read, instead return a flag to say whether a context switch is required or
     * not (i.e. has a task with a higher priority than us been woken by this
     * post). */

    configASSERT( px_قطار );

    /* xQueueGenericSendFromISR() should be used instead of xQueueGiveFromISR()
     * if the item size is not 0. */
    configASSERT( px_قطار->ux_شي_سائيز == 0 );

    /* Normally a mutex would not be given from an interrupt, especially if
     * there is a mutex holder, as priority inheritance makes no sense for an
     * interrupts, only tasks. */
    configASSERT( !( ( px_قطار->ux_قطار_Type == قطار_قطار_IS_MUTEX ) && ( px_قطار->u.xSemaphore.xMutexHolder != عدم ) ) );

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
    پورٽ_ASSERT_IF_مداخلت_ترجيح_INVALID();

    /* MISRA Ref 4.7.1 [Return value shall be checked] */
    /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
    /* coverity[misra_c_2012_directive_4_7_violation] */
    ux_Saved_مداخلت_حالت = ( UBaseType_t ) taskENTER_CRITICAL_FROM_ISR();
    {
        ثابت UBaseType_t ux_Messages_انتظار = px_قطار->ux_Messages_انتظار;

        /* When the queue is used to implement a semaphore no data is ever
         * moved through the queue but it is still valid to see if the queue 'has
         * space'. */
        إذا( ux_Messages_انتظار < px_قطار->ux_ڊيگهائي )
        {
            ثابت int8_t c_Tx_تالو = px_قطار->c_Tx_تالو;

            trace_قطار_SEND_FROM_ISR( px_قطار );

            /* A task can only have an inherited priority if it is a mutex
             * holder - and if there is a mutex holder then the mutex cannot be
             * given from an ISR.  As this is the ISR version of the function it
             * can be assumed there is no mutex holder and no need to determine if
             * priority disinheritance is needed.  Simply increase the count of
             * messages (semaphores) available. */
            px_قطار->ux_Messages_انتظار = ( UBaseType_t ) ( ux_Messages_انتظار + ( UBaseType_t ) 1 );

            /* The event list is not altered if the queue is locked.  This will
             * be done when the queue is unlocked later. */
            إذا( c_Tx_تالو == قطار_UNLOCKED )
            {
                #إذا ( config_USE_قطار_SETS == 1 )
                {
                    إذا( px_قطار->px_قطار_سيٽ_Container != عدم )
                    {
                        إذا( prv_Notify_قطار_سيٽ_Container( px_قطار ) != pdFALSE )
                        {
                            /* The semaphore is a member of a queue set, and
                             * posting to the queue set caused a higher priority
                             * task to unblock.  A context switch is required. */
                            إذا( px_Higher_ترجيح_Task_Woken != عدم )
                            {
                                *px_Higher_ترجيح_Task_Woken = pdTRUE;
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
                        إذا( list_LIST_IS_خالي_جايون( &( px_قطار->x_Tasks_انتظار_منزل_Receive ) ) == pdFALSE )
                        {
                            إذا( xTaskRemoveFromEventList( &( px_قطار->x_Tasks_انتظار_منزل_Receive ) ) != pdFALSE )
                            {
                                /* The task waiting has a higher priority so
                                 *  record that a context switch is required. */
                                إذا( px_Higher_ترجيح_Task_Woken != عدم )
                                {
                                    *px_Higher_ترجيح_Task_Woken = pdTRUE;
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
                    إذا( list_LIST_IS_خالي_جايون( &( px_قطار->x_Tasks_انتظار_منزل_Receive ) ) == pdFALSE )
                    {
                        إذا( xTaskRemoveFromEventList( &( px_قطار->x_Tasks_انتظار_منزل_Receive ) ) != pdFALSE )
                        {
                            /* The task waiting has a higher priority so record that a
                             * context switch is required. */
                            إذا( px_Higher_ترجيح_Task_Woken != عدم )
                            {
                                *px_Higher_ترجيح_Task_Woken = pdTRUE;
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
                #جيڪڏهن_پورو /* configUSE_QUEUE_SETS */
            }
            وإلا
            {
                /* Increment the lock count so the task that unlocks the queue
                 * knows that data was posted while it was locked. */
                prv_Increment_قطار_Tx_تالو( px_قطار, c_Tx_تالو );
            }

            xReturn = pdPASS;
        }
        وإلا
        {
            trace_قطار_SEND_FROM_ISR_FAILED( px_قطار );
            xReturn = err_قطار_ڀريل;
        }
    }
    taskEXIT_CRITICAL_FROM_ISR( ux_Saved_مداخلت_حالت );

    trace_RETURN_x_قطار_Give_From_ISR( xReturn );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

بنياد_Type_t x_قطار_Receive( قطار_Handle_t x_قطار,
                          فراغ * ثابت pv_بفر,
                          TickType_t x_Ticks_منزل_Wait )
{
    بنياد_Type_t x_داخلا_وقت_سيٽ = pdFALSE;
    وقت_ٻاهريون_t x_وقت_ٻاهريون_2;
    قطار_t * ثابت px_قطار = x_قطار;

    trace_ENTER_x_قطار_Receive( x_قطار, pv_بفر, x_Ticks_منزل_Wait );

    /* Check the pointer is not NULL. */
    configASSERT( ( px_قطار ) );

    /* The buffer into which data is received can only be NULL if the data size
     * is zero (so no data is copied into the buffer). */
    configASSERT( !( ( ( pv_بفر ) == عدم ) && ( ( px_قطار )->ux_شي_سائيز != ( UBaseType_t ) 0U ) ) );

    /* Cannot block if the scheduler is suspended. */
    #إذا ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_منزل_Wait != 0 ) ) );
    }
    #جيڪڏهن_پورو

    لكل( ; ; )
    {
        taskENTER_CRITICAL();
        {
            ثابت UBaseType_t ux_Messages_انتظار = px_قطار->ux_Messages_انتظار;

            /* Is there data in the queue now?  To be running the calling task
             * must be the highest priority task wanting to access the queue. */
            إذا( ux_Messages_انتظار > ( UBaseType_t ) 0 )
            {
                /* Data available, remove one item. */
                prv_Copy_معلومات_From_قطار( px_قطار, pv_بفر );
                trace_قطار_RECEIVE( px_قطار );
                px_قطار->ux_Messages_انتظار = ( UBaseType_t ) ( ux_Messages_انتظار - ( UBaseType_t ) 1 );

                /* There is now space in the queue, were any tasks waiting to
                 * post to the queue?  If so, unblock the highest priority waiting
                 * task. */
                إذا( list_LIST_IS_خالي_جايون( &( px_قطار->x_Tasks_انتظار_منزل_Send ) ) == pdFALSE )
                {
                    إذا( xTaskRemoveFromEventList( &( px_قطار->x_Tasks_انتظار_منزل_Send ) ) != pdFALSE )
                    {
                        قطار_YIELD_IF_USING_PREEMPTION();
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

                trace_RETURN_x_قطار_Receive( pdPASS );

                أرجع pdPASS;
            }
            وإلا
            {
                إذا( x_Ticks_منزل_Wait == ( TickType_t ) 0 )
                {
                    /* The queue was empty and no block time is specified (or
                     * the block time has expired) so leave now. */
                    taskEXIT_CRITICAL();

                    trace_قطار_RECEIVE_FAILED( px_قطار );
                    trace_RETURN_x_قطار_Receive( err_قطار_خالي_جايون );

                    أرجع err_قطار_خالي_جايون;
                }
                وإلا إذا( x_داخلا_وقت_سيٽ == pdFALSE )
                {
                    /* The queue was empty and a block time was specified so
                     * configure the timeout structure. */
                    v_Task_اندروني_سيٽ_وقت_ٻاهريون_State( &x_وقت_ٻاهريون_2 );
                    x_داخلا_وقت_سيٽ = pdTRUE;
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
        prv_تالو_قطار( px_قطار );

        /* Update the timeout state to see if it has expired yet. */
        إذا( x_Task_Check_For_وقت_ٻاهريون( &x_وقت_ٻاهريون_2, &x_Ticks_منزل_Wait ) == pdFALSE )
        {
            /* The timeout has not expired.  If the queue is still empty place
             * the task on the list of tasks waiting to receive from the queue. */
            إذا( prv_Is_قطار_خالي_جايون( px_قطار ) != pdFALSE )
            {
                trace_BLOCKING_ON_قطار_RECEIVE( px_قطار );
                v_Task_رکو_On_Event_List( &( px_قطار->x_Tasks_انتظار_منزل_Receive ), x_Ticks_منزل_Wait );
                prv_Unlock_قطار( px_قطار );

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
                prv_Unlock_قطار( px_قطار );
                ( فراغ ) xTaskResumeAll();
            }
        }
        وإلا
        {
            /* Timed out.  If there is no data in the queue exit, otherwise loop
             * back and attempt to read the data. */
            prv_Unlock_قطار( px_قطار );
            ( فراغ ) xTaskResumeAll();

            إذا( prv_Is_قطار_خالي_جايون( px_قطار ) != pdFALSE )
            {
                trace_قطار_RECEIVE_FAILED( px_قطار );
                trace_RETURN_x_قطار_Receive( err_قطار_خالي_جايون );

                أرجع err_قطار_خالي_جايون;
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
    }
}
/*-----------------------------------------------------------*/

بنياد_Type_t x_قطار_Semaphore_Take( قطار_Handle_t x_قطار,
                                TickType_t x_Ticks_منزل_Wait )
{
    بنياد_Type_t x_داخلا_وقت_سيٽ = pdFALSE;
    وقت_ٻاهريون_t x_وقت_ٻاهريون_2;
    قطار_t * ثابت px_قطار = x_قطار;

    #إذا ( configUSE_MUTEXES == 1 )
        بنياد_Type_t xInheritanceOccurred = pdFALSE;
    #جيڪڏهن_پورو

    trace_ENTER_x_قطار_Semaphore_Take( x_قطار, x_Ticks_منزل_Wait );

    /* Check the queue pointer is not NULL. */
    configASSERT( ( px_قطار ) );

    /* Check this really is a semaphore, in which case the item size will be
     * 0. */
    configASSERT( px_قطار->ux_شي_سائيز == 0 );

    /* Cannot block if the scheduler is suspended. */
    #إذا ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_منزل_Wait != 0 ) ) );
    }
    #جيڪڏهن_پورو

    لكل( ; ; )
    {
        taskENTER_CRITICAL();
        {
            /* Semaphores are queues with an item size of 0, and where the
             * number of messages in the queue is the semaphore's count value. */
            ثابت UBaseType_t ux_Semaphore_ڳڻپ = px_قطار->ux_Messages_انتظار;

            /* Is there data in the queue now?  To be running the calling task
             * must be the highest priority task wanting to access the queue. */
            إذا( ux_Semaphore_ڳڻپ > ( UBaseType_t ) 0 )
            {
                trace_قطار_RECEIVE( px_قطار );

                /* Semaphores are queues with a data size of zero and where the
                 * messages waiting is the semaphore's count.  Reduce the count. */
                px_قطار->ux_Messages_انتظار = ( UBaseType_t ) ( ux_Semaphore_ڳڻپ - ( UBaseType_t ) 1 );

                #إذا ( configUSE_MUTEXES == 1 )
                {
                    إذا( px_قطار->ux_قطار_Type == قطار_قطار_IS_MUTEX )
                    {
                        /* Record the information required to implement
                         * priority inheritance should it become necessary. */
                        px_قطار->u.xSemaphore.xMutexHolder = pv_Task_Increment_Mutex_Held_ڳڻپ();
                    }
                    وإلا
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #جيڪڏهن_پورو /* configUSE_MUTEXES */

                /* Check to see if other tasks are blocked waiting to give the
                 * semaphore, and if so, unblock the highest priority such task. */
                إذا( list_LIST_IS_خالي_جايون( &( px_قطار->x_Tasks_انتظار_منزل_Send ) ) == pdFALSE )
                {
                    إذا( xTaskRemoveFromEventList( &( px_قطار->x_Tasks_انتظار_منزل_Send ) ) != pdFALSE )
                    {
                        قطار_YIELD_IF_USING_PREEMPTION();
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

                trace_RETURN_x_قطار_Semaphore_Take( pdPASS );

                أرجع pdPASS;
            }
            وإلا
            {
                إذا( x_Ticks_منزل_Wait == ( TickType_t ) 0 )
                {
                    /* The semaphore count was 0 and no block time is specified
                     * (or the block time has expired) so exit now. */
                    taskEXIT_CRITICAL();

                    trace_قطار_RECEIVE_FAILED( px_قطار );
                    trace_RETURN_x_قطار_Semaphore_Take( err_قطار_خالي_جايون );

                    أرجع err_قطار_خالي_جايون;
                }
                وإلا إذا( x_داخلا_وقت_سيٽ == pdFALSE )
                {
                    /* The semaphore count was 0 and a block time was specified
                     * so configure the timeout structure ready to block. */
                    v_Task_اندروني_سيٽ_وقت_ٻاهريون_State( &x_وقت_ٻاهريون_2 );
                    x_داخلا_وقت_سيٽ = pdTRUE;
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
        prv_تالو_قطار( px_قطار );

        /* Update the timeout state to see if it has expired yet. */
        إذا( x_Task_Check_For_وقت_ٻاهريون( &x_وقت_ٻاهريون_2, &x_Ticks_منزل_Wait ) == pdFALSE )
        {
            /* A block time is specified and not expired.  If the semaphore
             * count is 0 then enter the Blocked state to wait for a semaphore to
             * become available.  As semaphores are implemented with queues the
             * queue being empty is equivalent to the semaphore count being 0. */
            إذا( prv_Is_قطار_خالي_جايون( px_قطار ) != pdFALSE )
            {
                trace_BLOCKING_ON_قطار_RECEIVE( px_قطار );

                #إذا ( configUSE_MUTEXES == 1 )
                {
                    إذا( px_قطار->ux_قطار_Type == قطار_قطار_IS_MUTEX )
                    {
                        taskENTER_CRITICAL();
                        {
                            xInheritanceOccurred = x_Task_ترجيح_Inherit( px_قطار->u.xSemaphore.xMutexHolder );
                        }
                        taskEXIT_CRITICAL();
                    }
                    وإلا
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #جيڪڏهن_پورو /* if ( configUSE_MUTEXES == 1 ) */

                v_Task_رکو_On_Event_List( &( px_قطار->x_Tasks_انتظار_منزل_Receive ), x_Ticks_منزل_Wait );
                prv_Unlock_قطار( px_قطار );

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
                prv_Unlock_قطار( px_قطار );
                ( فراغ ) xTaskResumeAll();
            }
        }
        وإلا
        {
            /* Timed out. */
            prv_Unlock_قطار( px_قطار );
            ( فراغ ) xTaskResumeAll();

            /* If the semaphore count is 0 exit now as the timeout has
             * expired.  Otherwise return to attempt to take the semaphore that is
             * known to be available.  As semaphores are implemented by queues the
             * queue being empty is equivalent to the semaphore count being 0. */
            إذا( prv_Is_قطار_خالي_جايون( px_قطار ) != pdFALSE )
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
                            UBaseType_t ux_Highest_انتظار_ترجيح;

                            /* This task blocking on the mutex caused another
                             * task to inherit this task's priority.  Now this task
                             * has timed out the priority should be disinherited
                             * again, but only as low as the next highest priority
                             * task that is waiting for the same mutex. */
                            ux_Highest_انتظار_ترجيح = prv_Get_Disinherit_ترجيح_After_Timeout( px_قطار );

                            /* vTaskPriorityDisinheritAfterTimeout uses the uxHighestWaitingPriority
                             * parameter to index pxReadyTasksLists when adding the task holding
                             * mutex to the ready list for its new priority. Coverity thinks that
                             * it can result in out-of-bounds access which is not true because
                             * uxHighestWaitingPriority, as returned by prvGetDisinheritPriorityAfterTimeout,
                             * is capped at ( configMAX_PRIORITIES - 1 ). */
                            /* coverity[overrun] */
                            v_Task_ترجيح_Disinherit_After_Timeout( px_قطار->u.xSemaphore.xMutexHolder, ux_Highest_انتظار_ترجيح );
                        }
                        taskEXIT_CRITICAL();
                    }
                }
                #جيڪڏهن_پورو /* configUSE_MUTEXES */

                trace_قطار_RECEIVE_FAILED( px_قطار );
                trace_RETURN_x_قطار_Semaphore_Take( err_قطار_خالي_جايون );

                أرجع err_قطار_خالي_جايون;
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
    }
}
/*-----------------------------------------------------------*/

بنياد_Type_t x_قطار_Peek( قطار_Handle_t x_قطار,
                       فراغ * ثابت pv_بفر,
                       TickType_t x_Ticks_منزل_Wait )
{
    بنياد_Type_t x_داخلا_وقت_سيٽ = pdFALSE;
    وقت_ٻاهريون_t x_وقت_ٻاهريون_2;
    int8_t * pcOriginalReadPosition;
    قطار_t * ثابت px_قطار = x_قطار;

    trace_ENTER_x_قطار_Peek( x_قطار, pv_بفر, x_Ticks_منزل_Wait );

    /* Check the pointer is not NULL. */
    configASSERT( ( px_قطار ) );

    /* The buffer into which data is received can only be NULL if the data size
     * is zero (so no data is copied into the buffer. */
    configASSERT( !( ( ( pv_بفر ) == عدم ) && ( ( px_قطار )->ux_شي_سائيز != ( UBaseType_t ) 0U ) ) );

    /* Cannot block if the scheduler is suspended. */
    #إذا ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_منزل_Wait != 0 ) ) );
    }
    #جيڪڏهن_پورو

    لكل( ; ; )
    {
        taskENTER_CRITICAL();
        {
            ثابت UBaseType_t ux_Messages_انتظار = px_قطار->ux_Messages_انتظار;

            /* Is there data in the queue now?  To be running the calling task
             * must be the highest priority task wanting to access the queue. */
            إذا( ux_Messages_انتظار > ( UBaseType_t ) 0 )
            {
                /* Remember the read position so it can be reset after the data
                 * is read from the queue as this function is only peeking the
                 * data, not removing it. */
                pcOriginalReadPosition = px_قطار->u.x_قطار.pcReadFrom;

                prv_Copy_معلومات_From_قطار( px_قطار, pv_بفر );
                trace_قطار_PEEK( px_قطار );

                /* The data is not being removed, so reset the read pointer. */
                px_قطار->u.x_قطار.pcReadFrom = pcOriginalReadPosition;

                /* The data is being left in the queue, so see if there are
                 * any other tasks waiting for the data. */
                إذا( list_LIST_IS_خالي_جايون( &( px_قطار->x_Tasks_انتظار_منزل_Receive ) ) == pdFALSE )
                {
                    إذا( xTaskRemoveFromEventList( &( px_قطار->x_Tasks_انتظار_منزل_Receive ) ) != pdFALSE )
                    {
                        /* The task waiting has a higher priority than this task. */
                        قطار_YIELD_IF_USING_PREEMPTION();
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

                trace_RETURN_x_قطار_Peek( pdPASS );

                أرجع pdPASS;
            }
            وإلا
            {
                إذا( x_Ticks_منزل_Wait == ( TickType_t ) 0 )
                {
                    /* The queue was empty and no block time is specified (or
                     * the block time has expired) so leave now. */
                    taskEXIT_CRITICAL();

                    trace_قطار_PEEK_FAILED( px_قطار );
                    trace_RETURN_x_قطار_Peek( err_قطار_خالي_جايون );

                    أرجع err_قطار_خالي_جايون;
                }
                وإلا إذا( x_داخلا_وقت_سيٽ == pdFALSE )
                {
                    /* The queue was empty and a block time was specified so
                     * configure the timeout structure ready to enter the blocked
                     * state. */
                    v_Task_اندروني_سيٽ_وقت_ٻاهريون_State( &x_وقت_ٻاهريون_2 );
                    x_داخلا_وقت_سيٽ = pdTRUE;
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
        prv_تالو_قطار( px_قطار );

        /* Update the timeout state to see if it has expired yet. */
        إذا( x_Task_Check_For_وقت_ٻاهريون( &x_وقت_ٻاهريون_2, &x_Ticks_منزل_Wait ) == pdFALSE )
        {
            /* Timeout has not expired yet, check to see if there is data in the
            * queue now, and if not enter the Blocked state to wait for data. */
            إذا( prv_Is_قطار_خالي_جايون( px_قطار ) != pdFALSE )
            {
                trace_BLOCKING_ON_قطار_PEEK( px_قطار );
                v_Task_رکو_On_Event_List( &( px_قطار->x_Tasks_انتظار_منزل_Receive ), x_Ticks_منزل_Wait );
                prv_Unlock_قطار( px_قطار );

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
                prv_Unlock_قطار( px_قطار );
                ( فراغ ) xTaskResumeAll();
            }
        }
        وإلا
        {
            /* The timeout has expired.  If there is still no data in the queue
             * exit, otherwise go back and try to read the data again. */
            prv_Unlock_قطار( px_قطار );
            ( فراغ ) xTaskResumeAll();

            إذا( prv_Is_قطار_خالي_جايون( px_قطار ) != pdFALSE )
            {
                trace_قطار_PEEK_FAILED( px_قطار );
                trace_RETURN_x_قطار_Peek( err_قطار_خالي_جايون );

                أرجع err_قطار_خالي_جايون;
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
    }
}
/*-----------------------------------------------------------*/

بنياد_Type_t x_قطار_Receive_From_ISR( قطار_Handle_t x_قطار,
                                 فراغ * ثابت pv_بفر,
                                 بنياد_Type_t * ثابت px_Higher_ترجيح_Task_Woken )
{
    بنياد_Type_t xReturn;
    UBaseType_t ux_Saved_مداخلت_حالت;
    قطار_t * ثابت px_قطار = x_قطار;

    trace_ENTER_x_قطار_Receive_From_ISR( x_قطار, pv_بفر, px_Higher_ترجيح_Task_Woken );

    configASSERT( px_قطار );
    configASSERT( !( ( pv_بفر == عدم ) && ( px_قطار->ux_شي_سائيز != ( UBaseType_t ) 0U ) ) );

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
    پورٽ_ASSERT_IF_مداخلت_ترجيح_INVALID();

    /* MISRA Ref 4.7.1 [Return value shall be checked] */
    /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
    /* coverity[misra_c_2012_directive_4_7_violation] */
    ux_Saved_مداخلت_حالت = ( UBaseType_t ) taskENTER_CRITICAL_FROM_ISR();
    {
        ثابت UBaseType_t ux_Messages_انتظار = px_قطار->ux_Messages_انتظار;

        /* Cannot block in an ISR, so check there is data available. */
        إذا( ux_Messages_انتظار > ( UBaseType_t ) 0 )
        {
            ثابت int8_t c_Rx_تالو = px_قطار->c_Rx_تالو;

            trace_قطار_RECEIVE_FROM_ISR( px_قطار );

            prv_Copy_معلومات_From_قطار( px_قطار, pv_بفر );
            px_قطار->ux_Messages_انتظار = ( UBaseType_t ) ( ux_Messages_انتظار - ( UBaseType_t ) 1 );

            /* If the queue is locked the event list will not be modified.
             * Instead update the lock count so the task that unlocks the queue
             * will know that an ISR has removed data while the queue was
             * locked. */
            إذا( c_Rx_تالو == قطار_UNLOCKED )
            {
                إذا( list_LIST_IS_خالي_جايون( &( px_قطار->x_Tasks_انتظار_منزل_Send ) ) == pdFALSE )
                {
                    إذا( xTaskRemoveFromEventList( &( px_قطار->x_Tasks_انتظار_منزل_Send ) ) != pdFALSE )
                    {
                        /* The task waiting has a higher priority than us so
                         * force a context switch. */
                        إذا( px_Higher_ترجيح_Task_Woken != عدم )
                        {
                            *px_Higher_ترجيح_Task_Woken = pdTRUE;
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
                prv_Increment_قطار_Rx_تالو( px_قطار, c_Rx_تالو );
            }

            xReturn = pdPASS;
        }
        وإلا
        {
            xReturn = pdFAIL;
            trace_قطار_RECEIVE_FROM_ISR_FAILED( px_قطار );
        }
    }
    taskEXIT_CRITICAL_FROM_ISR( ux_Saved_مداخلت_حالت );

    trace_RETURN_x_قطار_Receive_From_ISR( xReturn );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

بنياد_Type_t x_قطار_Peek_From_ISR( قطار_Handle_t x_قطار,
                              فراغ * ثابت pv_بفر )
{
    بنياد_Type_t xReturn;
    UBaseType_t ux_Saved_مداخلت_حالت;
    int8_t * pcOriginalReadPosition;
    قطار_t * ثابت px_قطار = x_قطار;

    trace_ENTER_x_قطار_Peek_From_ISR( x_قطار, pv_بفر );

    configASSERT( px_قطار );
    configASSERT( !( ( pv_بفر == عدم ) && ( px_قطار->ux_شي_سائيز != ( UBaseType_t ) 0U ) ) );
    configASSERT( px_قطار->ux_شي_سائيز != 0 ); /* Can't peek a semaphore. */

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
    پورٽ_ASSERT_IF_مداخلت_ترجيح_INVALID();

    /* MISRA Ref 4.7.1 [Return value shall be checked] */
    /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
    /* coverity[misra_c_2012_directive_4_7_violation] */
    ux_Saved_مداخلت_حالت = ( UBaseType_t ) taskENTER_CRITICAL_FROM_ISR();
    {
        /* Cannot block in an ISR, so check there is data available. */
        إذا( px_قطار->ux_Messages_انتظار > ( UBaseType_t ) 0 )
        {
            trace_قطار_PEEK_FROM_ISR( px_قطار );

            /* Remember the read position so it can be reset as nothing is
             * actually being removed from the queue. */
            pcOriginalReadPosition = px_قطار->u.x_قطار.pcReadFrom;
            prv_Copy_معلومات_From_قطار( px_قطار, pv_بفر );
            px_قطار->u.x_قطار.pcReadFrom = pcOriginalReadPosition;

            xReturn = pdPASS;
        }
        وإلا
        {
            xReturn = pdFAIL;
            trace_قطار_PEEK_FROM_ISR_FAILED( px_قطار );
        }
    }
    taskEXIT_CRITICAL_FROM_ISR( ux_Saved_مداخلت_حالت );

    trace_RETURN_x_قطار_Peek_From_ISR( xReturn );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

UBaseType_t ux_قطار_Messages_انتظار( ثابت قطار_Handle_t x_قطار )
{
    UBaseType_t uxReturn;

    trace_ENTER_ux_قطار_Messages_انتظار( x_قطار );

    configASSERT( x_قطار );

    taskENTER_CRITICAL();
    {
        uxReturn = ( ( قطار_t * ) x_قطار )->ux_Messages_انتظار;
    }
    taskEXIT_CRITICAL();

    trace_RETURN_ux_قطار_Messages_انتظار( uxReturn );

    أرجع uxReturn;
}
/*-----------------------------------------------------------*/

UBaseType_t ux_قطار_Spaces_موجود( ثابت قطار_Handle_t x_قطار )
{
    UBaseType_t uxReturn;
    قطار_t * ثابت px_قطار = x_قطار;

    trace_ENTER_ux_قطار_Spaces_موجود( x_قطار );

    configASSERT( px_قطار );

    taskENTER_CRITICAL();
    {
        uxReturn = ( UBaseType_t ) ( px_قطار->ux_ڊيگهائي - px_قطار->ux_Messages_انتظار );
    }
    taskEXIT_CRITICAL();

    trace_RETURN_ux_قطار_Spaces_موجود( uxReturn );

    أرجع uxReturn;
}
/*-----------------------------------------------------------*/

UBaseType_t ux_قطار_Messages_انتظار_From_ISR( ثابت قطار_Handle_t x_قطار )
{
    UBaseType_t uxReturn;
    قطار_t * ثابت px_قطار = x_قطار;

    trace_ENTER_ux_قطار_Messages_انتظار_From_ISR( x_قطار );

    configASSERT( px_قطار );
    uxReturn = px_قطار->ux_Messages_انتظار;

    trace_RETURN_ux_قطار_Messages_انتظار_From_ISR( uxReturn );

    أرجع uxReturn;
}
/*-----------------------------------------------------------*/

فراغ v_قطار_Delete( قطار_Handle_t x_قطار )
{
    قطار_t * ثابت px_قطار = x_قطار;

    trace_ENTER_v_قطار_Delete( x_قطار );

    configASSERT( px_قطار );
    trace_قطار_DELETE( px_قطار );

    #إذا ( config_قطار_REGISTRY_سائيز > 0 )
    {
        v_قطار_Unregister_قطار( px_قطار );
    }
    #جيڪڏهن_پورو

    #إذا ( ( config_SUPPORT_DYNAMIC_ورهاست == 1 ) && ( config_SUPPORT_STATIC_ورهاست == 0 ) )
    {
        /* The queue can only have been allocated dynamically - free it
         * again. */
        v_پورٽ_Free( px_قطار );
    }
    #وإلاإذا ( ( config_SUPPORT_DYNAMIC_ورهاست == 1 ) && ( config_SUPPORT_STATIC_ورهاست == 1 ) )
    {
        /* The queue could have been allocated statically or dynamically, so
         * check before attempting to free the memory. */
        إذا( px_قطار->ucStaticallyAllocated == ( uint8_t ) pdFALSE )
        {
            v_پورٽ_Free( px_قطار );
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
        ( فراغ ) px_قطار;
    }
    #جيڪڏهن_پورو /* configSUPPORT_DYNAMIC_ALLOCATION */

    trace_RETURN_v_قطار_Delete();
}
/*-----------------------------------------------------------*/

#إذا ( configUSE_TRACE_FACILITY == 1 )

    UBaseType_t ux_قطار_Get_قطار_عدد( قطار_Handle_t x_قطار )
    {
        trace_ENTER_ux_قطار_Get_قطار_عدد( x_قطار );

        trace_RETURN_ux_قطار_Get_قطار_عدد( ( ( قطار_t * ) x_قطار )->ux_قطار_عدد );

        أرجع ( ( قطار_t * ) x_قطار )->ux_قطار_عدد;
    }

#جيڪڏهن_پورو /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#إذا ( configUSE_TRACE_FACILITY == 1 )

    فراغ v_قطار_سيٽ_قطار_عدد( قطار_Handle_t x_قطار,
                               UBaseType_t ux_قطار_عدد )
    {
        trace_ENTER_v_قطار_سيٽ_قطار_عدد( x_قطار, ux_قطار_عدد );

        ( ( قطار_t * ) x_قطار )->ux_قطار_عدد = ux_قطار_عدد;

        trace_RETURN_v_قطار_سيٽ_قطار_عدد();
    }

#جيڪڏهن_پورو /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#إذا ( configUSE_TRACE_FACILITY == 1 )

    uint8_t uc_قطار_Get_قطار_Type( قطار_Handle_t x_قطار )
    {
        trace_ENTER_uc_قطار_Get_قطار_Type( x_قطار );

        trace_RETURN_uc_قطار_Get_قطار_Type( ( ( قطار_t * ) x_قطار )->uc_قطار_Type );

        أرجع ( ( قطار_t * ) x_قطار )->uc_قطار_Type;
    }

#جيڪڏهن_پورو /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

UBaseType_t ux_قطار_Get_قطار_شي_سائيز( قطار_Handle_t x_قطار ) /* PRIVILEGED_FUNCTION */
{
    trace_ENTER_ux_قطار_Get_قطار_شي_سائيز( x_قطار );

    trace_RETURN_ux_قطار_Get_قطار_شي_سائيز( ( ( قطار_t * ) x_قطار )->ux_شي_سائيز );

    أرجع ( ( قطار_t * ) x_قطار )->ux_شي_سائيز;
}
/*-----------------------------------------------------------*/

UBaseType_t ux_قطار_Get_قطار_ڊيگهائي( قطار_Handle_t x_قطار ) /* PRIVILEGED_FUNCTION */
{
    trace_ENTER_ux_قطار_Get_قطار_ڊيگهائي( x_قطار );

    trace_RETURN_ux_قطار_Get_قطار_ڊيگهائي( ( ( قطار_t * ) x_قطار )->ux_ڊيگهائي );

    أرجع ( ( قطار_t * ) x_قطار )->ux_ڊيگهائي;
}
/*-----------------------------------------------------------*/

#إذا ( configUSE_MUTEXES == 1 )

    ساكن UBaseType_t prv_Get_Disinherit_ترجيح_After_Timeout( ثابت قطار_t * ثابت px_قطار )
    {
        UBaseType_t ux_Highest_ترجيح_جو_انتظار_Tasks;

        /* If a task waiting for a mutex causes the mutex holder to inherit a
         * priority, but the waiting task times out, then the holder should
         * disinherit the priority - but only down to the highest priority of any
         * other tasks that are waiting for the same mutex.  For this purpose,
         * return the priority of the highest priority task that is waiting for the
         * mutex. */
        إذا( list_CURRENT_LIST_ڊيگهائي( &( px_قطار->x_Tasks_انتظار_منزل_Receive ) ) > 0U )
        {
            ux_Highest_ترجيح_جو_انتظار_Tasks = ( UBaseType_t ) ( ( UBaseType_t ) configMAX_PRIORITIES - ( UBaseType_t ) list_GET_شي_قدر_جو_سر_داخلا( &( px_قطار->x_Tasks_انتظار_منزل_Receive ) ) );
        }
        وإلا
        {
            ux_Highest_ترجيح_جو_انتظار_Tasks = tsk_IDLE_ترجيح;
        }

        أرجع ux_Highest_ترجيح_جو_انتظار_Tasks;
    }

#جيڪڏهن_پورو /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

ساكن بنياد_Type_t prv_Copy_معلومات_منزل_قطار( قطار_t * ثابت px_قطار,
                                      ثابت فراغ * pv_شي_منزل_قطار,
                                      ثابت بنياد_Type_t xPosition )
{
    بنياد_Type_t xReturn = pdFALSE;
    UBaseType_t ux_Messages_انتظار;

    /* This function is called from a critical section. */

    ux_Messages_انتظار = px_قطار->ux_Messages_انتظار;

    إذا( px_قطار->ux_شي_سائيز == ( UBaseType_t ) 0 )
    {
        #إذا ( configUSE_MUTEXES == 1 )
        {
            إذا( px_قطار->ux_قطار_Type == قطار_قطار_IS_MUTEX )
            {
                /* The mutex is no longer being held. */
                xReturn = x_Task_ترجيح_Disinherit( px_قطار->u.xSemaphore.xMutexHolder );
                px_قطار->u.xSemaphore.xMutexHolder = عدم;
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        #جيڪڏهن_پورو /* configUSE_MUTEXES */
    }
    وإلا إذا( xPosition == قطار_SEND_منزل_پٺ )
    {
        ( فراغ ) يادگيري_نقل( ( فراغ * ) px_قطار->pc_Write_منزل, pv_شي_منزل_قطار, ( size_t ) px_قطار->ux_شي_سائيز );
        px_قطار->pc_Write_منزل += px_قطار->ux_شي_سائيز;

        إذا( px_قطار->pc_Write_منزل >= px_قطار->u.x_قطار.pc_پڇ )
        {
            px_قطار->pc_Write_منزل = px_قطار->pc_سر;
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    وإلا
    {
        ( فراغ ) يادگيري_نقل( ( فراغ * ) px_قطار->u.x_قطار.pcReadFrom, pv_شي_منزل_قطار, ( size_t ) px_قطار->ux_شي_سائيز );
        px_قطار->u.x_قطار.pcReadFrom -= px_قطار->ux_شي_سائيز;

        إذا( px_قطار->u.x_قطار.pcReadFrom < px_قطار->pc_سر )
        {
            px_قطار->u.x_قطار.pcReadFrom = ( px_قطار->u.x_قطار.pc_پڇ - px_قطار->ux_شي_سائيز );
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }

        إذا( xPosition == قطار_OVERWRITE )
        {
            إذا( ux_Messages_انتظار > ( UBaseType_t ) 0 )
            {
                /* An item is not being added but overwritten, so subtract
                 * one from the recorded number of items in the queue so when
                 * one is added again below the number of recorded items remains
                 * correct. */
                --ux_Messages_انتظار;
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

    px_قطار->ux_Messages_انتظار = ( UBaseType_t ) ( ux_Messages_انتظار + ( UBaseType_t ) 1 );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

ساكن فراغ prv_Copy_معلومات_From_قطار( قطار_t * ثابت px_قطار,
                                  فراغ * ثابت pv_بفر )
{
    إذا( px_قطار->ux_شي_سائيز != ( UBaseType_t ) 0 )
    {
        px_قطار->u.x_قطار.pcReadFrom += px_قطار->ux_شي_سائيز;

        إذا( px_قطار->u.x_قطار.pcReadFrom >= px_قطار->u.x_قطار.pc_پڇ )
        {
            px_قطار->u.x_قطار.pcReadFrom = px_قطار->pc_سر;
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }

        ( فراغ ) يادگيري_نقل( ( فراغ * ) pv_بفر, ( فراغ * ) px_قطار->u.x_قطار.pcReadFrom, ( size_t ) px_قطار->ux_شي_سائيز );
    }
}
/*-----------------------------------------------------------*/

ساكن فراغ prv_Unlock_قطار( قطار_t * ثابت px_قطار )
{
    /* THIS FUNCTION MUST BE CALLED WITH THE SCHEDULER SUSPENDED. */

    /* The lock counts contains the number of extra data items placed or
     * removed from the queue while the queue was locked.  When a queue is
     * locked items can be added or removed, but the event lists cannot be
     * updated. */
    taskENTER_CRITICAL();
    {
        int8_t c_Tx_تالو = px_قطار->c_Tx_تالو;

        /* See if data was added to the queue while it was locked. */
        طالما( c_Tx_تالو > قطار_LOCKED_UNMODIFIED )
        {
            /* Data was posted while the queue was locked.  Are any tasks
             * blocked waiting for data to become available? */
            #إذا ( config_USE_قطار_SETS == 1 )
            {
                إذا( px_قطار->px_قطار_سيٽ_Container != عدم )
                {
                    إذا( prv_Notify_قطار_سيٽ_Container( px_قطار ) != pdFALSE )
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
                    إذا( list_LIST_IS_خالي_جايون( &( px_قطار->x_Tasks_انتظار_منزل_Receive ) ) == pdFALSE )
                    {
                        إذا( xTaskRemoveFromEventList( &( px_قطار->x_Tasks_انتظار_منزل_Receive ) ) != pdFALSE )
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
                إذا( list_LIST_IS_خالي_جايون( &( px_قطار->x_Tasks_انتظار_منزل_Receive ) ) == pdFALSE )
                {
                    إذا( xTaskRemoveFromEventList( &( px_قطار->x_Tasks_انتظار_منزل_Receive ) ) != pdFALSE )
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
            #جيڪڏهن_پورو /* configUSE_QUEUE_SETS */

            --c_Tx_تالو;
        }

        px_قطار->c_Tx_تالو = قطار_UNLOCKED;
    }
    taskEXIT_CRITICAL();

    /* Do the same for the Rx lock. */
    taskENTER_CRITICAL();
    {
        int8_t c_Rx_تالو = px_قطار->c_Rx_تالو;

        طالما( c_Rx_تالو > قطار_LOCKED_UNMODIFIED )
        {
            إذا( list_LIST_IS_خالي_جايون( &( px_قطار->x_Tasks_انتظار_منزل_Send ) ) == pdFALSE )
            {
                إذا( xTaskRemoveFromEventList( &( px_قطار->x_Tasks_انتظار_منزل_Send ) ) != pdFALSE )
                {
                    vTaskMissedYield();
                }
                وإلا
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                --c_Rx_تالو;
            }
            وإلا
            {
                اكسر;
            }
        }

        px_قطار->c_Rx_تالو = قطار_UNLOCKED;
    }
    taskEXIT_CRITICAL();
}
/*-----------------------------------------------------------*/

ساكن بنياد_Type_t prv_Is_قطار_خالي_جايون( ثابت قطار_t * px_قطار )
{
    بنياد_Type_t xReturn;

    taskENTER_CRITICAL();
    {
        إذا( px_قطار->ux_Messages_انتظار == ( UBaseType_t ) 0 )
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

بنياد_Type_t x_قطار_Is_قطار_خالي_جايون_From_ISR( ثابت قطار_Handle_t x_قطار )
{
    بنياد_Type_t xReturn;
    قطار_t * ثابت px_قطار = x_قطار;

    trace_ENTER_x_قطار_Is_قطار_خالي_جايون_From_ISR( x_قطار );

    configASSERT( px_قطار );

    إذا( px_قطار->ux_Messages_انتظار == ( UBaseType_t ) 0 )
    {
        xReturn = pdTRUE;
    }
    وإلا
    {
        xReturn = pdFALSE;
    }

    trace_RETURN_x_قطار_Is_قطار_خالي_جايون_From_ISR( xReturn );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

ساكن بنياد_Type_t prv_Is_قطار_ڀريل( ثابت قطار_t * px_قطار )
{
    بنياد_Type_t xReturn;

    taskENTER_CRITICAL();
    {
        إذا( px_قطار->ux_Messages_انتظار == px_قطار->ux_ڊيگهائي )
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

بنياد_Type_t x_قطار_Is_قطار_ڀريل_From_ISR( ثابت قطار_Handle_t x_قطار )
{
    بنياد_Type_t xReturn;
    قطار_t * ثابت px_قطار = x_قطار;

    trace_ENTER_x_قطار_Is_قطار_ڀريل_From_ISR( x_قطار );

    configASSERT( px_قطار );

    إذا( px_قطار->ux_Messages_انتظار == px_قطار->ux_ڊيگهائي )
    {
        xReturn = pdTRUE;
    }
    وإلا
    {
        xReturn = pdFALSE;
    }

    trace_RETURN_x_قطار_Is_قطار_ڀريل_From_ISR( xReturn );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

#إذا ( configUSE_CO_ROUTINES == 1 )

    بنياد_Type_t x_قطار_CRSend( قطار_Handle_t x_قطار,
                             ثابت فراغ * pv_شي_منزل_قطار,
                             TickType_t x_Ticks_منزل_Wait )
    {
        بنياد_Type_t xReturn;
        قطار_t * ثابت px_قطار = x_قطار;

        trace_ENTER_x_قطار_CRSend( x_قطار, pv_شي_منزل_قطار, x_Ticks_منزل_Wait );

        /* If the queue is already full we may have to block.  A critical section
         * is required to prevent an interrupt removing something from the queue
         * between the check to see if the queue is full and blocking on the queue. */
        پورٽ_DISABLE_INTERRUPTS();
        {
            إذا( prv_Is_قطار_ڀريل( px_قطار ) != pdFALSE )
            {
                /* The queue is full - do we want to block or just leave without
                 * posting? */
                إذا( x_Ticks_منزل_Wait > ( TickType_t ) 0 )
                {
                    /* As this is called from a coroutine we cannot block directly, but
                     * return indicating that we need to block. */
                    v_Co_Routine_جوڙيو_منزل_Delayed_List( x_Ticks_منزل_Wait, &( px_قطار->x_Tasks_انتظار_منزل_Send ) );
                    پورٽ_ENABLE_INTERRUPTS();
                    أرجع err_قطار_BLOCKED;
                }
                وإلا
                {
                    پورٽ_ENABLE_INTERRUPTS();
                    أرجع err_قطار_ڀريل;
                }
            }
        }
        پورٽ_ENABLE_INTERRUPTS();

        پورٽ_DISABLE_INTERRUPTS();
        {
            إذا( px_قطار->ux_Messages_انتظار < px_قطار->ux_ڊيگهائي )
            {
                /* There is room in the queue, copy the data into the queue. */
                prv_Copy_معلومات_منزل_قطار( px_قطار, pv_شي_منزل_قطار, قطار_SEND_منزل_پٺ );
                xReturn = pdPASS;

                /* Were any co-routines waiting for data to become available? */
                إذا( list_LIST_IS_خالي_جايون( &( px_قطار->x_Tasks_انتظار_منزل_Receive ) ) == pdFALSE )
                {
                    /* In this instance the co-routine could be placed directly
                     * into the ready list as we are within a critical section.
                     * Instead the same pending ready list mechanism is used as if
                     * the event were caused from within an interrupt. */
                    إذا( xCoRoutineRemoveFromEventList( &( px_قطار->x_Tasks_انتظار_منزل_Receive ) ) != pdFALSE )
                    {
                        /* The co-routine waiting has a higher priority so record
                         * that a yield might be appropriate. */
                        xReturn = err_قطار_YIELD;
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
                xReturn = err_قطار_ڀريل;
            }
        }
        پورٽ_ENABLE_INTERRUPTS();

        trace_RETURN_x_قطار_CRSend( xReturn );

        أرجع xReturn;
    }

#جيڪڏهن_پورو /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#إذا ( configUSE_CO_ROUTINES == 1 )

    بنياد_Type_t x_قطار_CRReceive( قطار_Handle_t x_قطار,
                                فراغ * pv_بفر,
                                TickType_t x_Ticks_منزل_Wait )
    {
        بنياد_Type_t xReturn;
        قطار_t * ثابت px_قطار = x_قطار;

        trace_ENTER_x_قطار_CRReceive( x_قطار, pv_بفر, x_Ticks_منزل_Wait );

        /* If the queue is already empty we may have to block.  A critical section
         * is required to prevent an interrupt adding something to the queue
         * between the check to see if the queue is empty and blocking on the queue. */
        پورٽ_DISABLE_INTERRUPTS();
        {
            إذا( px_قطار->ux_Messages_انتظار == ( UBaseType_t ) 0 )
            {
                /* There are no messages in the queue, do we want to block or just
                 * leave with nothing? */
                إذا( x_Ticks_منزل_Wait > ( TickType_t ) 0 )
                {
                    /* As this is a co-routine we cannot block directly, but return
                     * indicating that we need to block. */
                    v_Co_Routine_جوڙيو_منزل_Delayed_List( x_Ticks_منزل_Wait, &( px_قطار->x_Tasks_انتظار_منزل_Receive ) );
                    پورٽ_ENABLE_INTERRUPTS();
                    أرجع err_قطار_BLOCKED;
                }
                وإلا
                {
                    پورٽ_ENABLE_INTERRUPTS();
                    أرجع err_قطار_ڀريل;
                }
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        پورٽ_ENABLE_INTERRUPTS();

        پورٽ_DISABLE_INTERRUPTS();
        {
            إذا( px_قطار->ux_Messages_انتظار > ( UBaseType_t ) 0 )
            {
                /* Data is available from the queue. */
                px_قطار->u.x_قطار.pcReadFrom += px_قطار->ux_شي_سائيز;

                إذا( px_قطار->u.x_قطار.pcReadFrom >= px_قطار->u.x_قطار.pc_پڇ )
                {
                    px_قطار->u.x_قطار.pcReadFrom = px_قطار->pc_سر;
                }
                وإلا
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                --( px_قطار->ux_Messages_انتظار );
                ( فراغ ) يادگيري_نقل( ( فراغ * ) pv_بفر, ( فراغ * ) px_قطار->u.x_قطار.pcReadFrom, ( بنانشان ) px_قطار->ux_شي_سائيز );

                xReturn = pdPASS;

                /* Were any co-routines waiting for space to become available? */
                إذا( list_LIST_IS_خالي_جايون( &( px_قطار->x_Tasks_انتظار_منزل_Send ) ) == pdFALSE )
                {
                    /* In this instance the co-routine could be placed directly
                     * into the ready list as we are within a critical section.
                     * Instead the same pending ready list mechanism is used as if
                     * the event were caused from within an interrupt. */
                    إذا( xCoRoutineRemoveFromEventList( &( px_قطار->x_Tasks_انتظار_منزل_Send ) ) != pdFALSE )
                    {
                        xReturn = err_قطار_YIELD;
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
        پورٽ_ENABLE_INTERRUPTS();

        trace_RETURN_x_قطار_CRReceive( xReturn );

        أرجع xReturn;
    }

#جيڪڏهن_پورو /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#إذا ( configUSE_CO_ROUTINES == 1 )

    بنياد_Type_t x_قطار_CRSend_From_ISR( قطار_Handle_t x_قطار,
                                    ثابت فراغ * pv_شي_منزل_قطار,
                                    بنياد_Type_t xCoRoutinePreviouslyWoken )
    {
        قطار_t * ثابت px_قطار = x_قطار;

        trace_ENTER_x_قطار_CRSend_From_ISR( x_قطار, pv_شي_منزل_قطار, xCoRoutinePreviouslyWoken );

        /* Cannot block within an ISR so if there is no space on the queue then
         * exit without doing anything. */
        إذا( px_قطار->ux_Messages_انتظار < px_قطار->ux_ڊيگهائي )
        {
            prv_Copy_معلومات_منزل_قطار( px_قطار, pv_شي_منزل_قطار, قطار_SEND_منزل_پٺ );

            /* We only want to wake one co-routine per ISR, so check that a
             * co-routine has not already been woken. */
            إذا( xCoRoutinePreviouslyWoken == pdFALSE )
            {
                إذا( list_LIST_IS_خالي_جايون( &( px_قطار->x_Tasks_انتظار_منزل_Receive ) ) == pdFALSE )
                {
                    إذا( xCoRoutineRemoveFromEventList( &( px_قطار->x_Tasks_انتظار_منزل_Receive ) ) != pdFALSE )
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

        trace_RETURN_x_قطار_CRSend_From_ISR( xCoRoutinePreviouslyWoken );

        أرجع xCoRoutinePreviouslyWoken;
    }

#جيڪڏهن_پورو /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#إذا ( configUSE_CO_ROUTINES == 1 )

    بنياد_Type_t x_قطار_CRReceive_From_ISR( قطار_Handle_t x_قطار,
                                       فراغ * pv_بفر,
                                       بنياد_Type_t * pxCoRoutineWoken )
    {
        بنياد_Type_t xReturn;
        قطار_t * ثابت px_قطار = x_قطار;

        trace_ENTER_x_قطار_CRReceive_From_ISR( x_قطار, pv_بفر, pxCoRoutineWoken );

        /* We cannot block from an ISR, so check there is data available. If
         * not then just leave without doing anything. */
        إذا( px_قطار->ux_Messages_انتظار > ( UBaseType_t ) 0 )
        {
            /* Copy the data from the queue. */
            px_قطار->u.x_قطار.pcReadFrom += px_قطار->ux_شي_سائيز;

            إذا( px_قطار->u.x_قطار.pcReadFrom >= px_قطار->u.x_قطار.pc_پڇ )
            {
                px_قطار->u.x_قطار.pcReadFrom = px_قطار->pc_سر;
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }

            --( px_قطار->ux_Messages_انتظار );
            ( فراغ ) يادگيري_نقل( ( فراغ * ) pv_بفر, ( فراغ * ) px_قطار->u.x_قطار.pcReadFrom, ( بنانشان ) px_قطار->ux_شي_سائيز );

            إذا( ( *pxCoRoutineWoken ) == pdFALSE )
            {
                إذا( list_LIST_IS_خالي_جايون( &( px_قطار->x_Tasks_انتظار_منزل_Send ) ) == pdFALSE )
                {
                    إذا( xCoRoutineRemoveFromEventList( &( px_قطار->x_Tasks_انتظار_منزل_Send ) ) != pdFALSE )
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

        trace_RETURN_x_قطار_CRReceive_From_ISR( xReturn );

        أرجع xReturn;
    }

#جيڪڏهن_پورو /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#إذا ( config_قطار_REGISTRY_سائيز > 0 )

    فراغ v_قطار_جوڙيو_منزل_Registry( قطار_Handle_t x_قطار,
                              ثابت حرف * pc_قطار_Name )
    {
        UBaseType_t ux;
        قطار_Registry_شي_t * px_داخلا_منزل_Write = عدم;

        trace_ENTER_v_قطار_جوڙيو_منزل_Registry( x_قطار, pc_قطار_Name );

        configASSERT( x_قطار );

        إذا( pc_قطار_Name != عدم )
        {
            /* See if there is an empty space in the registry.  A NULL name denotes
             * a free slot. */
            لكل( ux = ( UBaseType_t ) 0U; ux < ( UBaseType_t ) config_قطار_REGISTRY_سائيز; ux++ )
            {
                /* Replace an existing entry if the queue is already in the registry. */
                إذا( x_قطار == x_قطار_Registry[ ux ].xHandle )
                {
                    px_داخلا_منزل_Write = &( x_قطار_Registry[ ux ] );
                    اكسر;
                }
                /* Otherwise, store in the next empty location */
                وإلا إذا( ( px_داخلا_منزل_Write == عدم ) && ( x_قطار_Registry[ ux ].pc_قطار_Name == عدم ) )
                {
                    px_داخلا_منزل_Write = &( x_قطار_Registry[ ux ] );
                }
                وإلا
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
        }

        إذا( px_داخلا_منزل_Write != عدم )
        {
            /* Store the information on this queue. */
            px_داخلا_منزل_Write->pc_قطار_Name = pc_قطار_Name;
            px_داخلا_منزل_Write->xHandle = x_قطار;

            trace_قطار_REGISTRY_جوڙيو( x_قطار, pc_قطار_Name );
        }

        trace_RETURN_v_قطار_جوڙيو_منزل_Registry();
    }

#جيڪڏهن_پورو /* configQUEUE_REGISTRY_SIZE */
/*-----------------------------------------------------------*/

#إذا ( config_قطار_REGISTRY_سائيز > 0 )

    ثابت حرف * pc_قطار_Get_Name( قطار_Handle_t x_قطار )
    {
        UBaseType_t ux;
        ثابت حرف * pcReturn = عدم;

        trace_ENTER_pc_قطار_Get_Name( x_قطار );

        configASSERT( x_قطار );

        /* Note there is nothing here to protect against another task adding or
         * removing entries from the registry while it is being searched. */

        لكل( ux = ( UBaseType_t ) 0U; ux < ( UBaseType_t ) config_قطار_REGISTRY_سائيز; ux++ )
        {
            إذا( x_قطار_Registry[ ux ].xHandle == x_قطار )
            {
                pcReturn = x_قطار_Registry[ ux ].pc_قطار_Name;
                اكسر;
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }

        trace_RETURN_pc_قطار_Get_Name( pcReturn );

        أرجع pcReturn;
    }

#جيڪڏهن_پورو /* configQUEUE_REGISTRY_SIZE */
/*-----------------------------------------------------------*/

#إذا ( config_قطار_REGISTRY_سائيز > 0 )

    فراغ v_قطار_Unregister_قطار( قطار_Handle_t x_قطار )
    {
        UBaseType_t ux;

        trace_ENTER_v_قطار_Unregister_قطار( x_قطار );

        configASSERT( x_قطار );

        /* See if the handle of the queue being unregistered in actually in the
         * registry. */
        لكل( ux = ( UBaseType_t ) 0U; ux < ( UBaseType_t ) config_قطار_REGISTRY_سائيز; ux++ )
        {
            إذا( x_قطار_Registry[ ux ].xHandle == x_قطار )
            {
                /* Set the name to NULL to show that this slot if free again. */
                x_قطار_Registry[ ux ].pc_قطار_Name = عدم;

                /* Set the handle to NULL to ensure the same queue handle cannot
                 * appear in the registry twice if it is added, removed, then
                 * added again. */
                x_قطار_Registry[ ux ].xHandle = ( قطار_Handle_t ) 0;
                اكسر;
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }

        trace_RETURN_v_قطار_Unregister_قطار();
    }

#جيڪڏهن_پورو /* configQUEUE_REGISTRY_SIZE */
/*-----------------------------------------------------------*/

#إذا ( configUSE_TIMERS == 1 )

    فراغ v_قطار_Wait_For_پيغام_Restricted( قطار_Handle_t x_قطار,
                                         TickType_t x_Ticks_منزل_Wait,
                                         ثابت بنياد_Type_t xWaitIndefinitely )
    {
        قطار_t * ثابت px_قطار = x_قطار;

        trace_ENTER_v_قطار_Wait_For_پيغام_Restricted( x_قطار, x_Ticks_منزل_Wait, xWaitIndefinitely );

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
        prv_تالو_قطار( px_قطار );

        إذا( px_قطار->ux_Messages_انتظار == ( UBaseType_t ) 0U )
        {
            /* There is nothing in the queue, block for the specified period. */
            v_Task_رکو_On_Event_List_Restricted( &( px_قطار->x_Tasks_انتظار_منزل_Receive ), x_Ticks_منزل_Wait, xWaitIndefinitely );
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }

        prv_Unlock_قطار( px_قطار );

        trace_RETURN_v_قطار_Wait_For_پيغام_Restricted();
    }

#جيڪڏهن_پورو /* configUSE_TIMERS */
/*-----------------------------------------------------------*/

#إذا ( ( config_USE_قطار_SETS == 1 ) && ( config_SUPPORT_DYNAMIC_ورهاست == 1 ) )

    قطار_سيٽ_Handle_t x_قطار_Create_سيٽ( ثابت UBaseType_t ux_Event_قطار_ڊيگهائي )
    {
        قطار_سيٽ_Handle_t px_قطار;

        trace_ENTER_x_قطار_Create_سيٽ( ux_Event_قطار_ڊيگهائي );

        px_قطار = x_قطار_Generic_Create( ux_Event_قطار_ڊيگهائي, ( UBaseType_t ) حجم( قطار_t * ), قطار_قطار_TYPE_سيٽ );

        trace_RETURN_x_قطار_Create_سيٽ( px_قطار );

        أرجع px_قطار;
    }

#جيڪڏهن_پورو /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#إذا ( config_USE_قطار_SETS == 1 )

    بنياد_Type_t x_قطار_جوڙيو_منزل_سيٽ( قطار_سيٽ_Member_Handle_t x_قطار_Or_Semaphore,
                               قطار_سيٽ_Handle_t x_قطار_سيٽ )
    {
        بنياد_Type_t xReturn;

        trace_ENTER_x_قطار_جوڙيو_منزل_سيٽ( x_قطار_Or_Semaphore, x_قطار_سيٽ );

        taskENTER_CRITICAL();
        {
            إذا( ( ( قطار_t * ) x_قطار_Or_Semaphore )->px_قطار_سيٽ_Container != عدم )
            {
                /* Cannot add a queue/semaphore to more than one queue set. */
                xReturn = pdFAIL;
            }
            وإلا إذا( ( ( قطار_t * ) x_قطار_Or_Semaphore )->ux_Messages_انتظار != ( UBaseType_t ) 0 )
            {
                /* Cannot add a queue/semaphore to a queue set if there are already
                 * items in the queue/semaphore. */
                xReturn = pdFAIL;
            }
            وإلا
            {
                ( ( قطار_t * ) x_قطار_Or_Semaphore )->px_قطار_سيٽ_Container = x_قطار_سيٽ;
                xReturn = pdPASS;
            }
        }
        taskEXIT_CRITICAL();

        trace_RETURN_x_قطار_جوڙيو_منزل_سيٽ( xReturn );

        أرجع xReturn;
    }

#جيڪڏهن_پورو /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#إذا ( config_USE_قطار_SETS == 1 )

    بنياد_Type_t x_قطار_Remove_From_سيٽ( قطار_سيٽ_Member_Handle_t x_قطار_Or_Semaphore,
                                    قطار_سيٽ_Handle_t x_قطار_سيٽ )
    {
        بنياد_Type_t xReturn;
        قطار_t * ثابت px_قطار_Or_Semaphore = ( قطار_t * ) x_قطار_Or_Semaphore;

        trace_ENTER_x_قطار_Remove_From_سيٽ( x_قطار_Or_Semaphore, x_قطار_سيٽ );

        إذا( px_قطار_Or_Semaphore->px_قطار_سيٽ_Container != x_قطار_سيٽ )
        {
            /* The queue was not a member of the set. */
            xReturn = pdFAIL;
        }
        وإلا إذا( px_قطار_Or_Semaphore->ux_Messages_انتظار != ( UBaseType_t ) 0 )
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
                px_قطار_Or_Semaphore->px_قطار_سيٽ_Container = عدم;
            }
            taskEXIT_CRITICAL();
            xReturn = pdPASS;
        }

        trace_RETURN_x_قطار_Remove_From_سيٽ( xReturn );

        أرجع xReturn;
    }

#جيڪڏهن_پورو /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#إذا ( config_USE_قطار_SETS == 1 )

    قطار_سيٽ_Member_Handle_t x_قطار_Select_From_سيٽ( قطار_سيٽ_Handle_t x_قطار_سيٽ,
                                                TickType_t ثابت x_Ticks_منزل_Wait )
    {
        قطار_سيٽ_Member_Handle_t xReturn = عدم;

        trace_ENTER_x_قطار_Select_From_سيٽ( x_قطار_سيٽ, x_Ticks_منزل_Wait );

        ( فراغ ) x_قطار_Receive( ( قطار_Handle_t ) x_قطار_سيٽ, &xReturn, x_Ticks_منزل_Wait );

        trace_RETURN_x_قطار_Select_From_سيٽ( xReturn );

        أرجع xReturn;
    }

#جيڪڏهن_پورو /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#إذا ( config_USE_قطار_SETS == 1 )

    قطار_سيٽ_Member_Handle_t x_قطار_Select_From_سيٽ_From_ISR( قطار_سيٽ_Handle_t x_قطار_سيٽ )
    {
        قطار_سيٽ_Member_Handle_t xReturn = عدم;

        trace_ENTER_x_قطار_Select_From_سيٽ_From_ISR( x_قطار_سيٽ );

        ( فراغ ) x_قطار_Receive_From_ISR( ( قطار_Handle_t ) x_قطار_سيٽ, &xReturn, عدم );

        trace_RETURN_x_قطار_Select_From_سيٽ_From_ISR( xReturn );

        أرجع xReturn;
    }

#جيڪڏهن_پورو /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#إذا ( config_USE_قطار_SETS == 1 )

    ساكن بنياد_Type_t prv_Notify_قطار_سيٽ_Container( ثابت قطار_t * ثابت px_قطار )
    {
        قطار_t * px_قطار_سيٽ_Container = px_قطار->px_قطار_سيٽ_Container;
        بنياد_Type_t xReturn = pdFALSE;

        /* This function must be called form a critical section. */

        /* The following line is not reachable in unit tests because every call
         * to prvNotifyQueueSetContainer is preceded by a check that
         * pxQueueSetContainer != NULL */
        configASSERT( px_قطار_سيٽ_Container ); /* LCOV_EXCL_BR_LINE */
        configASSERT( px_قطار_سيٽ_Container->ux_Messages_انتظار < px_قطار_سيٽ_Container->ux_ڊيگهائي );

        إذا( px_قطار_سيٽ_Container->ux_Messages_انتظار < px_قطار_سيٽ_Container->ux_ڊيگهائي )
        {
            ثابت int8_t c_Tx_تالو = px_قطار_سيٽ_Container->c_Tx_تالو;

            trace_قطار_سيٽ_SEND( px_قطار_سيٽ_Container );

            /* The data copied is the handle of the queue that contains data. */
            xReturn = prv_Copy_معلومات_منزل_قطار( px_قطار_سيٽ_Container, &px_قطار, قطار_SEND_منزل_پٺ );

            إذا( c_Tx_تالو == قطار_UNLOCKED )
            {
                إذا( list_LIST_IS_خالي_جايون( &( px_قطار_سيٽ_Container->x_Tasks_انتظار_منزل_Receive ) ) == pdFALSE )
                {
                    إذا( xTaskRemoveFromEventList( &( px_قطار_سيٽ_Container->x_Tasks_انتظار_منزل_Receive ) ) != pdFALSE )
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
                prv_Increment_قطار_Tx_تالو( px_قطار_سيٽ_Container, c_Tx_تالو );
            }
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }

        أرجع xReturn;
    }

#جيڪڏهن_پورو /* configUSE_QUEUE_SETS */
