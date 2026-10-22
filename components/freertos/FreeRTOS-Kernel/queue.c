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

#ضمن <stdlib.h>
#ضمن <string.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#تعریف_کړه MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#ضمن "FreeRTOS.h"
#ضمن "task.h"
#ضمن "queue.h"
/* Include private IDF API additions for critical thread safety macros */
#ضمن "esp_private/freertos_idf_additions_priv.h"

#إذا ( configUSE_CO_ROUTINES == 1 )
    #ضمن "croutine.h"
#د_که_پای

/* Lint e9021, e961 and e750 are suppressed as a MISRA exception justified
 * because the MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#تعریف_لغوه MPU_WRAPPERS_INCLUDED_FROM_API_FILE /*lint !e961 !e750 !e9021. */

/* Single core FreeRTOS uses queue locks to ensure that vTaskPlaceOnEventList()
 * calls are deterministic (as queue locks use scheduler suspension instead of
 * critical sections). However, the SMP implementation is non-deterministic
 * anyways, thus SMP can forego the use of queue locks (replaced with a critical
 * sections) in exchange for better queue performance. */
#إذا ( config_شمېره_د_CORES > 1 )
    #تعریف_کړه کتار_USE_LOCKS            0
    #تعریف_کړه کتار_UNLOCKED             ( ( int8_t ) 0 )
#وإلا /* configNUMBER_OF_CORES > 1 */
    #تعریف_کړه کتار_USE_LOCKS            1
    /* Constants used with the cRxLock and cTxLock structure members. */
    #تعریف_کړه کتار_UNLOCKED             ( ( int8_t ) -1 )
    #تعریف_کړه کتار_LOCKED_UNMODIFIED    ( ( int8_t ) 0 )
    #تعریف_کړه کتار_INT8_MAX             ( ( int8_t ) 127 )
#د_که_پای /* configNUMBER_OF_CORES > 1 */

/* When the Queue_t structure is used to represent a base queue its pcHead and
 * pcTail members are used as pointers into the queue storage area.  When the
 * Queue_t structure is used to represent a mutex pcHead and pcTail pointers are
 * not necessary, and the pcHead pointer is set to NULL to indicate that the
 * structure instead holds a pointer to the mutex holder (if any).  Map alternative
 * names to the pcHead and structure member to ensure the readability of the code
 * is maintained.  The QueuePointers_t and SemaphoreData_t types are used to form
 * a union as their usage is mutually exclusive dependent on what the queue is
 * being used for. */
#تعریف_کړه ux_کتار_Type            pc_سر
#تعریف_کړه کتار_کتار_IS_MUTEX    عدم

تعريفنوع بنية کتار_اشارې
{
    int8_t * pc_لکۍ;     /*< Points to the byte at the end of the queue storage area.  Once more byte is allocated than necessary to store the queue items, this is used as a marker. */
    int8_t * pcReadFrom; /*< Points to the last place that a queued item was read from when the structure is used as a queue. */
} کتار_اشارې_t;

تعريفنوع بنية Semaphore_معلومات
{
    TaskHandle_t xMutexHolder;        /*< The handle of the task that holds the mutex. */
    UBaseType_t ux_Recursive_Call_شمېر; /*< Maintains a count of the number of times a recursive mutex has been recursively 'taken' when the structure is used as a mutex. */
} Semaphore_معلومات_t;

/* Semaphores do not actually store or copy data, so have an item size of
 * zero. */
#تعریف_کړه کتار_SEMAPHORE_کتار_توکی_اوږدتیا    ( ( UBaseType_t ) 0 )
#تعریف_کړه کتار_MUTEX_GIVE_بلاک_وخت          ( ( TickType_t ) 0U )

#إذا ( configUSE_PREEMPTION == 0 )

/* If the cooperative scheduler is being used then a yield should not be
 * performed just because a higher priority task has been woken. */
    #تعریف_کړه کتار_YIELD_IF_USING_PREEMPTION()
#وإلا
    #تعریف_کړه کتار_YIELD_IF_USING_PREEMPTION()    پورټ_YIELD_WITHIN_API()
#د_که_پای

/*
 * Definition of the queue used by the scheduler.
 * Items are queued by copy, not reference.  See the following link for the
 * rationale: https://www.FreeRTOS.org/Embedded-RTOS-Queues.html
 */
تعريفنوع بنية کتار_Definition /* The old naming convention is used to prevent breaking kernel aware debuggers. */
{
    int8_t * pc_سر;           /*< Points to the beginning of the queue storage area. */
    int8_t * pc_Write_منزل;        /*< Points to the free next place in the storage area. */

    اتحاد
    {
        کتار_اشارې_t x_کتار;     /*< Data required exclusively when this structure is used as a queue. */
        Semaphore_معلومات_t xSemaphore; /*< Data required exclusively when this structure is used as a semaphore. */
    } u;

    List_t x_Tasks_انتظار_منزل_Send;             /*< List of tasks that are blocked waiting to post onto this queue.  Stored in priority order. */
    List_t x_Tasks_انتظار_منزل_Receive;          /*< List of tasks that are blocked waiting to read from this queue.  Stored in priority order. */

    متطاير UBaseType_t ux_Messages_انتظار; /*< The number of items currently in the queue. */
    UBaseType_t ux_اوږدتیا;                   /*< The length of the queue defined as the number of items it will hold, not the number of bytes. */
    UBaseType_t ux_توکی_اندازه;                 /*< The size of each items that the queue will hold. */

    #إذا ( کتار_USE_LOCKS == 1 )
        متطاير int8_t c_Rx_قلف; /*< Stores the number of items received from the queue (removed from the queue) while the queue was locked.  Set to queueUNLOCKED when the queue is not locked. */
        متطاير int8_t c_Tx_قلف; /*< Stores the number of items transmitted to the queue (added to the queue) while the queue was locked.  Set to queueUNLOCKED when the queue is not locked. */
    #د_که_پای /* queueUSE_LOCKS == 1 */

    #إذا ( ( config_SUPPORT_STATIC_ویش == 1 ) && ( config_SUPPORT_DYNAMIC_ویش == 1 ) )
        uint8_t ucStaticallyAllocated; /*< Set to pdTRUE if the memory used by the queue was statically allocated to ensure no attempt is made to free the memory. */
    #د_که_پای

    #إذا ( config_USE_کتار_SETS == 1 )
        بنية کتار_Definition * px_کتار_سېټ_Container;
    #د_که_پای

    #إذا ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_کتار_شمېره;
        uint8_t uc_کتار_Type;
    #د_که_پای

    پورټ_MUX_TYPE x_کتار_قلف; /* Spinlock required for SMP critical sections */
} x_کتار_2;

/* The old xQUEUE name is maintained above then typedefed to the new Queue_t
 * name below to enable the use of older kernel aware debuggers. */
تعريفنوع x_کتار_2 کتار_t;

/*-----------------------------------------------------------*/

/*
 * The queue registry is just a means for kernel aware debuggers to locate
 * queue structures.  It has no other purpose so is an optional component.
 */
#إذا ( config_کتار_REGISTRY_اندازه > 0 )

/* The type stored within the queue registry array.  This allows a name
 * to be assigned to each queue making kernel aware debugging a little
 * more user friendly. */
    تعريفنوع بنية کتار_REGISTRY_توکی
    {
        ثابت حرف * pc_کتار_Name; /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
        کتار_Handle_t xHandle;
    } x_کتار_Registry_توکی;

/* The old xQueueRegistryItem name is maintained above then typedefed to the
 * new xQueueRegistryItem name below to enable the use of older kernel aware
 * debuggers. */
    تعريفنوع x_کتار_Registry_توکی کتار_Registry_توکی_t;

/* The queue registry is simply an array of QueueRegistryItem_t structures.
 * The pcQueueName member of a structure being NULL is indicative of the
 * array position being vacant. */
    PRIVILEGED_معلومات کتار_Registry_توکی_t x_کتار_Registry[ config_کتار_REGISTRY_اندازه ];

    #إذا ( config_شمېره_د_CORES > 1 )
/* Spinlock required in SMP when accessing the queue registry */
        ساكن پورټ_MUX_TYPE x_کتار_Registry_قلف = پورټ_MUX_INITIALIZER_UNLOCKED;
    #د_که_پای /* configNUMBER_OF_CORES > 1 */

#د_که_پای /* configQUEUE_REGISTRY_SIZE */

#إذا ( کتار_USE_LOCKS == 1 )

/*
 * Unlocks a queue locked by a call to prvLockQueue.  Locking a queue does not
 * prevent an ISR from adding or removing items to the queue, but does prevent
 * an ISR from removing tasks from the queue event lists.  If an ISR finds a
 * queue is locked it will instead increment the appropriate queue lock count
 * to indicate that a task may require unblocking.  When the queue in unlocked
 * these lock counts are inspected, and the appropriate action taken.
 */
    ساكن فراغ prv_Unlock_کتار( کتار_t * ثابت px_کتار ) PRIVILEGED_FUNCTION;

/*
 * Uses a critical section to determine if there is any data in a queue.
 *
 * @return pdTRUE if the queue contains no items, otherwise pdFALSE.
 */
    ساكن بنسټ_Type_t prv_Is_کتار_تشه( ثابت کتار_t * px_کتار ) PRIVILEGED_FUNCTION;

/*
 * Uses a critical section to determine if there is any space in a queue.
 *
 * @return pdTRUE if there is no space, otherwise pdFALSE;
 */
    ساكن بنسټ_Type_t prv_Is_کتار_ډک( ثابت کتار_t * px_کتار ) PRIVILEGED_FUNCTION;
#د_که_پای /* queueUSE_LOCKS == 1 */

/*
 * Copies an item into the queue, either at the front of the queue or the
 * back of the queue.
 */
ساكن بنسټ_Type_t prv_Copy_معلومات_منزل_کتار( کتار_t * ثابت px_کتار,
                                      ثابت فراغ * pv_توکی_منزل_کتار,
                                      ثابت بنسټ_Type_t xPosition ) PRIVILEGED_FUNCTION;

/*
 * Copies an item out of a queue.
 */
ساكن فراغ prv_Copy_معلومات_From_کتار( کتار_t * ثابت px_کتار,
                                  فراغ * ثابت pv_بفر ) PRIVILEGED_FUNCTION;

#إذا ( config_USE_کتار_SETS == 1 )

/*
 * Checks to see if a queue is a member of a queue set, and if so, notifies
 * the queue set that the queue contains data.
 */
    ساكن بنسټ_Type_t prv_Notify_کتار_سېټ_Container( ثابت کتار_t * ثابت px_کتار ) PRIVILEGED_FUNCTION;
#د_که_پای

/*
 * Called after a Queue_t structure has been allocated either statically or
 * dynamically to fill in the structure's members.
 */
ساكن فراغ prv_Initialise_New_کتار( ثابت UBaseType_t ux_کتار_اوږدتیا,
                                   ثابت UBaseType_t ux_توکی_اندازه,
                                   uint8_t * puc_کتار_Storage,
                                   ثابت uint8_t uc_کتار_Type,
                                   کتار_t * px_New_کتار ) PRIVILEGED_FUNCTION;

/*
 * Mutexes are a special type of queue.  When a mutex is created, first the
 * queue is created, then prvInitialiseMutex() is called to configure the queue
 * as a mutex.
 */
#إذا ( configUSE_MUTEXES == 1 )
    ساكن فراغ prvInitialiseMutex( کتار_t * px_New_کتار ) PRIVILEGED_FUNCTION;
#د_که_پای

#إذا ( configUSE_MUTEXES == 1 )

/*
 * If a task waiting for a mutex causes the mutex holder to inherit a
 * priority, but the waiting task times out, then the holder should
 * disinherit the priority - but only down to the highest priority of any
 * other tasks that are waiting for the same mutex.  This function returns
 * that priority.
 */
    ساكن UBaseType_t prv_Get_Disinherit_لومړیتوب_After_Timeout( ثابت کتار_t * ثابت px_کتار ) PRIVILEGED_FUNCTION;
#د_که_پای
/*-----------------------------------------------------------*/

#إذا ( کتار_USE_LOCKS == 1 )

/*
 * Macro to mark a queue as locked.  Locking a queue prevents an ISR from
 * accessing the queue event lists.
 */
    #تعریف_کړه prv_قلف_کتار( px_کتار )                        \
    taskENTER_CRITICAL( &( px_کتار->x_کتار_قلف ) );        \
    {                                                      \
        إذا( ( px_کتار )->c_Rx_قلف == کتار_UNLOCKED )        \
        {                                                  \
            ( px_کتار )->c_Rx_قلف = کتار_LOCKED_UNMODIFIED; \
        }                                                  \
        إذا( ( px_کتار )->c_Tx_قلف == کتار_UNLOCKED )        \
        {                                                  \
            ( px_کتار )->c_Tx_قلف = کتار_LOCKED_UNMODIFIED; \
        }                                                  \
    }                                                      \
    taskEXIT_CRITICAL( &( px_کتار->x_کتار_قلف ) )

/*
 * Macro to increment cTxLock member of the queue data structure. It is
 * capped at the number of tasks in the system as we cannot unblock more
 * tasks than the number of tasks in the system.
 */
    #تعریف_کړه prv_Increment_کتار_Tx_قلف( px_کتار, c_Tx_قلف )                       \
    {                                                                         \
        ثابت UBaseType_t ux_شمېره_د_Tasks = ux_Task_Get_شمېره_د_Tasks();         \
        إذا( ( UBaseType_t ) ( c_Tx_قلف ) < ux_شمېره_د_Tasks )                   \
        {                                                                     \
            configASSERT( ( c_Tx_قلف ) != کتار_INT8_MAX );                     \
            ( px_کتار )->c_Tx_قلف = ( int8_t ) ( ( c_Tx_قلف ) + ( int8_t ) 1 ); \
        }                                                                     \
    }

/*
 * Macro to increment cRxLock member of the queue data structure. It is
 * capped at the number of tasks in the system as we cannot unblock more
 * tasks than the number of tasks in the system.
 */
    #تعریف_کړه prv_Increment_کتار_Rx_قلف( px_کتار, c_Rx_قلف )                       \
    {                                                                         \
        ثابت UBaseType_t ux_شمېره_د_Tasks = ux_Task_Get_شمېره_د_Tasks();         \
        إذا( ( UBaseType_t ) ( c_Rx_قلف ) < ux_شمېره_د_Tasks )                   \
        {                                                                     \
            configASSERT( ( c_Rx_قلف ) != کتار_INT8_MAX );                     \
            ( px_کتار )->c_Rx_قلف = ( int8_t ) ( ( c_Rx_قلف ) + ( int8_t ) 1 ); \
        }                                                                     \
    }
#د_که_پای /* queueUSE_LOCKS == 1 */
/*-----------------------------------------------------------*/

بنسټ_Type_t x_کتار_Generic_Reset( کتار_Handle_t x_کتار,
                               بنسټ_Type_t x_New_کتار )
{
    بنسټ_Type_t xReturn = pdPASS;
    کتار_t * ثابت px_کتار = x_کتار;

    configASSERT( px_کتار );

    إذا( x_New_کتار == pdTRUE )
    {
        پورټ_MUX_INITIALIZE( &( px_کتار->x_کتار_قلف ) );
    }

    إذا( ( px_کتار != عدم ) &&
        ( px_کتار->ux_اوږدتیا >= 1U ) &&
        /* Check for multiplication overflow. */
        ( ( SIZE_MAX / px_کتار->ux_اوږدتیا ) >= px_کتار->ux_توکی_اندازه ) )
    {
        taskENTER_CRITICAL( &( px_کتار->x_کتار_قلف ) );
        {
            px_کتار->u.x_کتار.pc_لکۍ = px_کتار->pc_سر + ( px_کتار->ux_اوږدتیا * px_کتار->ux_توکی_اندازه ); /*lint !e9016 Pointer arithmetic allowed on char types, especially when it assists conveying intent. */
            px_کتار->ux_Messages_انتظار = ( UBaseType_t ) 0U;
            px_کتار->pc_Write_منزل = px_کتار->pc_سر;
            px_کتار->u.x_کتار.pcReadFrom = px_کتار->pc_سر + ( ( px_کتار->ux_اوږدتیا - 1U ) * px_کتار->ux_توکی_اندازه ); /*lint !e9016 Pointer arithmetic allowed on char types, especially when it assists conveying intent. */
            #إذا ( کتار_USE_LOCKS == 1 )
            {
                px_کتار->c_Rx_قلف = کتار_UNLOCKED;
                px_کتار->c_Tx_قلف = کتار_UNLOCKED;
            }
            #د_که_پای /* queueUSE_LOCKS == 1 */

            إذا( x_New_کتار == pdFALSE )
            {
                /* If there are tasks blocked waiting to read from the queue, then
                 * the tasks will remain blocked as after this function exits the queue
                 * will still be empty.  If there are tasks blocked waiting to write to
                 * the queue, then one should be unblocked as after this function exits
                 * it will be possible to write to it. */
                إذا( list_LIST_IS_تشه( &( px_کتار->x_Tasks_انتظار_منزل_Send ) ) == pdFALSE )
                {
                    إذا( xTaskRemoveFromEventList( &( px_کتار->x_Tasks_انتظار_منزل_Send ) ) != pdFALSE )
                    {
                        کتار_YIELD_IF_USING_PREEMPTION();
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
                vListInitialise( &( px_کتار->x_Tasks_انتظار_منزل_Send ) );
                vListInitialise( &( px_کتار->x_Tasks_انتظار_منزل_Receive ) );
            }
        }
        taskEXIT_CRITICAL( &( px_کتار->x_کتار_قلف ) );
    }
    وإلا
    {
        xReturn = pdFAIL;
    }

    configASSERT( xReturn != pdFAIL );

    /* A value is returned for calling semantic consistency with previous
     * versions. */
    أرجع xReturn;
}
/*-----------------------------------------------------------*/

#إذا ( config_SUPPORT_STATIC_ویش == 1 )

    کتار_Handle_t x_کتار_Generic_Create_Static( ثابت UBaseType_t ux_کتار_اوږدتیا,
                                             ثابت UBaseType_t ux_توکی_اندازه,
                                             uint8_t * puc_کتار_Storage,
                                             Static_کتار_t * px_Static_کتار,
                                             ثابت uint8_t uc_کتار_Type )
    {
        کتار_t * px_New_کتار = عدم;

        /* The StaticQueue_t structure and the queue storage area must be
         * supplied. */
        configASSERT( px_Static_کتار );

        إذا( ( ux_کتار_اوږدتیا > ( UBaseType_t ) 0 ) &&
            ( px_Static_کتار != عدم ) &&

            /* A queue storage area should be provided if the item size is not 0, and
             * should not be provided if the item size is 0. */
            ( !( ( puc_کتار_Storage != عدم ) && ( ux_توکی_اندازه == 0 ) ) ) &&
            ( !( ( puc_کتار_Storage == عدم ) && ( ux_توکی_اندازه != 0 ) ) ) )
        {
            #إذا ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticQueue_t or StaticSemaphore_t equals the size of
                 * the real queue and semaphore structures. */
                متطاير size_t x_اندازه = حجم( Static_کتار_t );

                /* This assertion cannot be branch covered in unit tests */
                configASSERT( x_اندازه == حجم( کتار_t ) ); /* LCOV_EXCL_BR_LINE */
                ( فراغ ) x_اندازه;                             /* Keeps lint quiet when configASSERT() is not defined. */
            }
            #د_که_پای /* configASSERT_DEFINED */

            /* The address of a statically allocated queue was passed in, use it.
             * The address of a statically allocated storage area was also passed in
             * but is already set. */
            px_New_کتار = ( کتار_t * ) px_Static_کتار; /*lint !e740 !e9087 Unusual cast is ok as the structures are designed to have the same alignment, and the size is checked by an assert. */

            #إذا ( config_SUPPORT_DYNAMIC_ویش == 1 )
            {
                /* Queues can be allocated wither statically or dynamically, so
                 * note this queue was allocated statically in case the queue is
                 * later deleted. */
                px_New_کتار->ucStaticallyAllocated = pdTRUE;
            }
            #د_که_پای /* configSUPPORT_DYNAMIC_ALLOCATION */

            prv_Initialise_New_کتار( ux_کتار_اوږدتیا, ux_توکی_اندازه, puc_کتار_Storage, uc_کتار_Type, px_New_کتار );
        }
        وإلا
        {
            configASSERT( px_New_کتار );
            mtCOVERAGE_TEST_MARKER();
        }

        أرجع px_New_کتار;
    }

#د_که_پای /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

#إذا ( config_SUPPORT_STATIC_ویش == 1 )

    بنسټ_Type_t x_کتار_Generic_Get_Static_Buffers( کتار_Handle_t x_کتار,
                                              uint8_t ** ppuc_کتار_Storage,
                                              Static_کتار_t ** ppx_Static_کتار )
    {
        بنسټ_Type_t xReturn;
        کتار_t * ثابت px_کتار = x_کتار;

        configASSERT( px_کتار );
        configASSERT( ppx_Static_کتار );

        #إذا ( config_SUPPORT_DYNAMIC_ویش == 1 )
        {
            /* Check if the queue was statically allocated. */
            إذا( px_کتار->ucStaticallyAllocated == ( uint8_t ) pdTRUE )
            {
                إذا( ppuc_کتار_Storage != عدم )
                {
                    *ppuc_کتار_Storage = ( uint8_t * ) px_کتار->pc_سر;
                }

                *ppx_Static_کتار = ( Static_کتار_t * ) px_کتار;
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
            إذا( ppuc_کتار_Storage != عدم )
            {
                *ppuc_کتار_Storage = ( uint8_t * ) px_کتار->pc_سر;
            }

            *ppx_Static_کتار = ( Static_کتار_t * ) px_کتار;
            xReturn = pdTRUE;
        }
        #د_که_پای /* configSUPPORT_DYNAMIC_ALLOCATION */

        أرجع xReturn;
    }

#د_که_پای /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

#إذا ( config_SUPPORT_DYNAMIC_ویش == 1 )

    کتار_Handle_t x_کتار_Generic_Create( ثابت UBaseType_t ux_کتار_اوږدتیا,
                                       ثابت UBaseType_t ux_توکی_اندازه,
                                       ثابت uint8_t uc_کتار_Type )
    {
        کتار_t * px_New_کتار = عدم;
        size_t x_کتار_اندازه_دننه_بایټونه;
        uint8_t * puc_کتار_Storage;

        إذا( ( ux_کتار_اوږدتیا > ( UBaseType_t ) 0 ) &&
            /* Check for multiplication overflow. */
            ( ( SIZE_MAX / ux_کتار_اوږدتیا ) >= ux_توکی_اندازه ) &&
            /* Check for addition overflow. */
            ( ( SIZE_MAX - حجم( کتار_t ) ) >= ( ux_کتار_اوږدتیا * ux_توکی_اندازه ) ) )
        {
            /* Allocate enough space to hold the maximum number of items that
             * can be in the queue at any time.  It is valid for uxItemSize to be
             * zero in the case the queue is used as a semaphore. */
            x_کتار_اندازه_دننه_بایټونه = ( size_t ) ( ux_کتار_اوږدتیا * ux_توکی_اندازه ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */

            /* Allocate the queue and storage area.  Justification for MISRA
             * deviation as follows:  pvPortMalloc() always ensures returned memory
             * blocks are aligned per the requirements of the MCU stack.  In this case
             * pvPortMalloc() must return a pointer that is guaranteed to meet the
             * alignment requirements of the Queue_t structure - which in this case
             * is an int8_t *.  Therefore, whenever the stack alignment requirements
             * are greater than or equal to the pointer to char requirements the cast
             * is safe.  In other cases alignment requirements are not strict (one or
             * two bytes). */
            px_New_کتار = ( کتار_t * ) pv_پورټ_Malloc( حجم( کتار_t ) + x_کتار_اندازه_دننه_بایټونه ); /*lint !e9087 !e9079 see comment above. */

            إذا( px_New_کتار != عدم )
            {
                /* Jump past the queue structure to find the location of the queue
                 * storage area. */
                puc_کتار_Storage = ( uint8_t * ) px_New_کتار;
                puc_کتار_Storage += حجم( کتار_t ); /*lint !e9016 Pointer arithmetic allowed on char types, especially when it assists conveying intent. */

                #إذا ( config_SUPPORT_STATIC_ویش == 1 )
                {
                    /* Queues can be created either statically or dynamically, so
                     * note this task was created dynamically in case it is later
                     * deleted. */
                    px_New_کتار->ucStaticallyAllocated = pdFALSE;
                }
                #د_که_پای /* configSUPPORT_STATIC_ALLOCATION */

                prv_Initialise_New_کتار( ux_کتار_اوږدتیا, ux_توکی_اندازه, puc_کتار_Storage, uc_کتار_Type, px_New_کتار );
            }
            وإلا
            {
                trace_کتار_CREATE_FAILED( uc_کتار_Type );
                mtCOVERAGE_TEST_MARKER();
            }
        }
        وإلا
        {
            configASSERT( px_New_کتار );
            mtCOVERAGE_TEST_MARKER();
        }

        أرجع px_New_کتار;
    }

#د_که_پای /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

ساكن فراغ prv_Initialise_New_کتار( ثابت UBaseType_t ux_کتار_اوږدتیا,
                                   ثابت UBaseType_t ux_توکی_اندازه,
                                   uint8_t * puc_کتار_Storage,
                                   ثابت uint8_t uc_کتار_Type,
                                   کتار_t * px_New_کتار )
{
    /* Remove compiler warnings about unused parameters should
     * configUSE_TRACE_FACILITY not be set to 1. */
    ( فراغ ) uc_کتار_Type;

    إذا( ux_توکی_اندازه == ( UBaseType_t ) 0 )
    {
        /* No RAM was allocated for the queue storage area, but PC head cannot
         * be set to NULL because NULL is used as a key to say the queue is used as
         * a mutex.  Therefore just set pcHead to point to the queue as a benign
         * value that is known to be within the memory map. */
        px_New_کتار->pc_سر = ( int8_t * ) px_New_کتار;
    }
    وإلا
    {
        /* Set the head to the start of the queue storage area. */
        px_New_کتار->pc_سر = ( int8_t * ) puc_کتار_Storage;
    }

    /* Initialise the queue members as described where the queue type is
     * defined. */
    px_New_کتار->ux_اوږدتیا = ux_کتار_اوږدتیا;
    px_New_کتار->ux_توکی_اندازه = ux_توکی_اندازه;
    ( فراغ ) x_کتار_Generic_Reset( px_New_کتار, pdTRUE );

    #إذا ( configUSE_TRACE_FACILITY == 1 )
    {
        px_New_کتار->uc_کتار_Type = uc_کتار_Type;
    }
    #د_که_پای /* configUSE_TRACE_FACILITY */

    #إذا ( config_USE_کتار_SETS == 1 )
    {
        px_New_کتار->px_کتار_سېټ_Container = عدم;
    }
    #د_که_پای /* configUSE_QUEUE_SETS */

    trace_کتار_CREATE( px_New_کتار );
}
/*-----------------------------------------------------------*/

#إذا ( configUSE_MUTEXES == 1 )

    ساكن فراغ prvInitialiseMutex( کتار_t * px_New_کتار )
    {
        إذا( px_New_کتار != عدم )
        {
            /* The queue create function will set all the queue structure members
            * correctly for a generic queue, but this function is creating a
            * mutex.  Overwrite those members that need to be set differently -
            * in particular the information required for priority inheritance. */
            px_New_کتار->u.xSemaphore.xMutexHolder = عدم;
            px_New_کتار->ux_کتار_Type = کتار_کتار_IS_MUTEX;

            /* In case this is a recursive mutex. */
            px_New_کتار->u.xSemaphore.ux_Recursive_Call_شمېر = 0;

            /* Initialize the mutex's spinlock */
            پورټ_MUX_INITIALIZE( &( px_New_کتار->x_کتار_قلف ) );

            traceCREATE_MUTEX( px_New_کتار );

            /* Start with the semaphore in the expected state. */
            ( فراغ ) x_کتار_Generic_Send( px_New_کتار, عدم, ( TickType_t ) 0U, کتار_SEND_منزل_شا );
        }
        وإلا
        {
            traceCREATE_MUTEX_FAILED();
        }
    }

#د_که_پای /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

#إذا ( ( configUSE_MUTEXES == 1 ) && ( config_SUPPORT_DYNAMIC_ویش == 1 ) )

    کتار_Handle_t x_کتار_Create_Mutex( ثابت uint8_t uc_کتار_Type )
    {
        کتار_Handle_t x_New_کتار;
        ثابت UBaseType_t ux_Mutex_اوږدتیا = ( UBaseType_t ) 1, ux_Mutex_اندازه = ( UBaseType_t ) 0;

        x_New_کتار = x_کتار_Generic_Create( ux_Mutex_اوږدتیا, ux_Mutex_اندازه, uc_کتار_Type );
        prvInitialiseMutex( ( کتار_t * ) x_New_کتار );

        أرجع x_New_کتار;
    }

#د_که_پای /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

#إذا ( ( configUSE_MUTEXES == 1 ) && ( config_SUPPORT_STATIC_ویش == 1 ) )

    کتار_Handle_t x_کتار_Create_Mutex_Static( ثابت uint8_t uc_کتار_Type,
                                           Static_کتار_t * px_Static_کتار )
    {
        کتار_Handle_t x_New_کتار;
        ثابت UBaseType_t ux_Mutex_اوږدتیا = ( UBaseType_t ) 1, ux_Mutex_اندازه = ( UBaseType_t ) 0;

        /* Prevent compiler warnings about unused parameters if
         * configUSE_TRACE_FACILITY does not equal 1. */
        ( فراغ ) uc_کتار_Type;

        x_New_کتار = x_کتار_Generic_Create_Static( ux_Mutex_اوږدتیا, ux_Mutex_اندازه, عدم, px_Static_کتار, uc_کتار_Type );
        prvInitialiseMutex( ( کتار_t * ) x_New_کتار );

        أرجع x_New_کتار;
    }

#د_که_پای /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

#إذا ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) )

    TaskHandle_t x_کتار_Get_Mutex_Holder( کتار_Handle_t xSemaphore )
    {
        TaskHandle_t pxReturn;
        کتار_t * ثابت pxSemaphore = ( کتار_t * ) xSemaphore;

        configASSERT( xSemaphore );

        /* This function is called by xSemaphoreGetMutexHolder(), and should not
         * be called directly.  Note:  This is a good way of determining if the
         * calling task is the mutex holder, but not a good way of determining the
         * identity of the mutex holder, as the holder may change between the
         * following critical section exiting and the function returning. */
        taskENTER_CRITICAL( &( pxSemaphore->x_کتار_قلف ) );
        {
            إذا( pxSemaphore->ux_کتار_Type == کتار_کتار_IS_MUTEX )
            {
                pxReturn = pxSemaphore->u.xSemaphore.xMutexHolder;
            }
            وإلا
            {
                pxReturn = عدم;
            }
        }
        taskEXIT_CRITICAL( &( pxSemaphore->x_کتار_قلف ) );

        أرجع pxReturn;
    } /*lint !e818 xSemaphore cannot be a pointer to const because it is a typedef. */

#د_که_پای /* if ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) ) */
/*-----------------------------------------------------------*/

#إذا ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) )

    TaskHandle_t x_کتار_Get_Mutex_Holder_From_ISR( کتار_Handle_t xSemaphore )
    {
        TaskHandle_t pxReturn;

        configASSERT( xSemaphore );

        /* Mutexes cannot be used in interrupt service routines, so the mutex
         * holder should not change in an ISR, and therefore a critical section is
         * not required here. */
        إذا( ( ( کتار_t * ) xSemaphore )->ux_کتار_Type == کتار_کتار_IS_MUTEX )
        {
            pxReturn = ( ( کتار_t * ) xSemaphore )->u.xSemaphore.xMutexHolder;
        }
        وإلا
        {
            pxReturn = عدم;
        }

        أرجع pxReturn;
    } /*lint !e818 xSemaphore cannot be a pointer to const because it is a typedef. */

#د_که_پای /* if ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) ) */
/*-----------------------------------------------------------*/

#إذا ( configUSE_RECURSIVE_MUTEXES == 1 )

    بنسټ_Type_t x_کتار_Give_Mutex_Recursive( کتار_Handle_t xMutex )
    {
        بنسټ_Type_t xReturn;
        کتار_t * ثابت pxMutex = ( کتار_t * ) xMutex;

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
            ( pxMutex->u.xSemaphore.ux_Recursive_Call_شمېر )--;

            /* Has the recursive call count unwound to 0? */
            إذا( pxMutex->u.xSemaphore.ux_Recursive_Call_شمېر == ( UBaseType_t ) 0 )
            {
                /* Return the mutex.  This will automatically unblock any other
                 * task that might be waiting to access the mutex. */
                ( فراغ ) x_کتار_Generic_Send( pxMutex, عدم, کتار_MUTEX_GIVE_بلاک_وخت, کتار_SEND_منزل_شا );
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

        أرجع xReturn;
    }

#د_که_پای /* configUSE_RECURSIVE_MUTEXES */
/*-----------------------------------------------------------*/

#إذا ( configUSE_RECURSIVE_MUTEXES == 1 )

    بنسټ_Type_t x_کتار_Take_Mutex_Recursive( کتار_Handle_t xMutex,
                                         TickType_t x_Ticks_منزل_Wait )
    {
        بنسټ_Type_t xReturn;
        کتار_t * ثابت pxMutex = ( کتار_t * ) xMutex;

        configASSERT( pxMutex );

        /* Comments regarding mutual exclusion as per those within
         * xQueueGiveMutexRecursive(). */

        traceTAKE_MUTEX_RECURSIVE( pxMutex );

        إذا( pxMutex->u.xSemaphore.xMutexHolder == xTaskGetCurrentTaskHandle() )
        {
            ( pxMutex->u.xSemaphore.ux_Recursive_Call_شمېر )++;
            xReturn = pdPASS;
        }
        وإلا
        {
            xReturn = x_کتار_Semaphore_Take( pxMutex, x_Ticks_منزل_Wait );

            /* pdPASS will only be returned if the mutex was successfully
             * obtained.  The calling task may have entered the Blocked state
             * before reaching here. */
            إذا( xReturn != pdFAIL )
            {
                ( pxMutex->u.xSemaphore.ux_Recursive_Call_شمېر )++;
            }
            وإلا
            {
                traceTAKE_MUTEX_RECURSIVE_FAILED( pxMutex );
            }
        }

        أرجع xReturn;
    }

#د_که_پای /* configUSE_RECURSIVE_MUTEXES */
/*-----------------------------------------------------------*/

#إذا ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( config_SUPPORT_STATIC_ویش == 1 ) )

    کتار_Handle_t x_کتار_Create_Counting_Semaphore_Static( ثابت UBaseType_t ux_Max_شمېر,
                                                       ثابت UBaseType_t ux_Initial_شمېر,
                                                       Static_کتار_t * px_Static_کتار )
    {
        کتار_Handle_t xHandle = عدم;

        إذا( ( ux_Max_شمېر != 0 ) &&
            ( ux_Initial_شمېر <= ux_Max_شمېر ) )
        {
            xHandle = x_کتار_Generic_Create_Static( ux_Max_شمېر, کتار_SEMAPHORE_کتار_توکی_اوږدتیا, عدم, px_Static_کتار, کتار_کتار_TYPE_COUNTING_SEMAPHORE );

            إذا( xHandle != عدم )
            {
                ( ( کتار_t * ) xHandle )->ux_Messages_انتظار = ux_Initial_شمېر;

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

        أرجع xHandle;
    }

#د_که_پای /* ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) ) */
/*-----------------------------------------------------------*/

#إذا ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( config_SUPPORT_DYNAMIC_ویش == 1 ) )

    کتار_Handle_t x_کتار_Create_Counting_Semaphore( ثابت UBaseType_t ux_Max_شمېر,
                                                 ثابت UBaseType_t ux_Initial_شمېر )
    {
        کتار_Handle_t xHandle = عدم;

        إذا( ( ux_Max_شمېر != 0 ) &&
            ( ux_Initial_شمېر <= ux_Max_شمېر ) )
        {
            xHandle = x_کتار_Generic_Create( ux_Max_شمېر, کتار_SEMAPHORE_کتار_توکی_اوږدتیا, کتار_کتار_TYPE_COUNTING_SEMAPHORE );

            إذا( xHandle != عدم )
            {
                ( ( کتار_t * ) xHandle )->ux_Messages_انتظار = ux_Initial_شمېر;

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

        أرجع xHandle;
    }

#د_که_پای /* ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) ) */
/*-----------------------------------------------------------*/

بنسټ_Type_t x_کتار_Generic_Send( کتار_Handle_t x_کتار,
                              ثابت فراغ * ثابت pv_توکی_منزل_کتار,
                              TickType_t x_Ticks_منزل_Wait,
                              ثابت بنسټ_Type_t xCopyPosition )
{
    بنسټ_Type_t x_ثبت_وخت_سېټ = pdFALSE, xYieldRequired;
    وخت_بهر_t x_وخت_بهر_2;
    کتار_t * ثابت px_کتار = x_کتار;

    configASSERT( px_کتار );
    configASSERT( !( ( pv_توکی_منزل_کتار == عدم ) && ( px_کتار->ux_توکی_اندازه != ( UBaseType_t ) 0U ) ) );
    configASSERT( !( ( xCopyPosition == کتار_OVERWRITE ) && ( px_کتار->ux_اوږدتیا != 1 ) ) );
    #إذا ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_منزل_Wait != 0 ) ) );
    }
    #د_که_پای

    /*lint -save -e904 This function relaxes the coding standard somewhat to
     * allow return statements within the function itself.  This is done in the
     * interest of execution time efficiency. */
    لكل( ; ; )
    {
        taskENTER_CRITICAL( &( px_کتار->x_کتار_قلف ) );
        {
            /* Is there room on the queue now?  The running task must be the
             * highest priority task wanting to access the queue.  If the head item
             * in the queue is to be overwritten then it does not matter if the
             * queue is full. */
            إذا( ( px_کتار->ux_Messages_انتظار < px_کتار->ux_اوږدتیا ) || ( xCopyPosition == کتار_OVERWRITE ) )
            {
                trace_کتار_SEND( px_کتار );

                #إذا ( config_USE_کتار_SETS == 1 )
                {
                    ثابت UBaseType_t ux_پخوانی_Messages_انتظار = px_کتار->ux_Messages_انتظار;

                    xYieldRequired = prv_Copy_معلومات_منزل_کتار( px_کتار, pv_توکی_منزل_کتار, xCopyPosition );

                    إذا( px_کتار->px_کتار_سېټ_Container != عدم )
                    {
                        إذا( ( xCopyPosition == کتار_OVERWRITE ) && ( ux_پخوانی_Messages_انتظار != ( UBaseType_t ) 0 ) )
                        {
                            /* Do not notify the queue set as an existing item
                             * was overwritten in the queue so the number of items
                             * in the queue has not changed. */
                            mtCOVERAGE_TEST_MARKER();
                        }
                        وإلا إذا( prv_Notify_کتار_سېټ_Container( px_کتار ) != pdFALSE )
                        {
                            /* The queue is a member of a queue set, and posting
                             * to the queue set caused a higher priority task to
                             * unblock. A context switch is required. */
                            کتار_YIELD_IF_USING_PREEMPTION();
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
                        إذا( list_LIST_IS_تشه( &( px_کتار->x_Tasks_انتظار_منزل_Receive ) ) == pdFALSE )
                        {
                            إذا( xTaskRemoveFromEventList( &( px_کتار->x_Tasks_انتظار_منزل_Receive ) ) != pdFALSE )
                            {
                                /* The unblocked task has a priority higher than
                                 * our own so yield immediately.  Yes it is ok to
                                 * do this from within the critical section - the
                                 * kernel takes care of that. */
                                کتار_YIELD_IF_USING_PREEMPTION();
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
                            کتار_YIELD_IF_USING_PREEMPTION();
                        }
                        وإلا
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                }
                #وإلا /* configUSE_QUEUE_SETS */
                {
                    xYieldRequired = prv_Copy_معلومات_منزل_کتار( px_کتار, pv_توکی_منزل_کتار, xCopyPosition );

                    /* If there was a task waiting for data to arrive on the
                     * queue then unblock it now. */
                    إذا( list_LIST_IS_تشه( &( px_کتار->x_Tasks_انتظار_منزل_Receive ) ) == pdFALSE )
                    {
                        إذا( xTaskRemoveFromEventList( &( px_کتار->x_Tasks_انتظار_منزل_Receive ) ) != pdFALSE )
                        {
                            /* The unblocked task has a priority higher than
                             * our own so yield immediately.  Yes it is ok to do
                             * this from within the critical section - the kernel
                             * takes care of that. */
                            کتار_YIELD_IF_USING_PREEMPTION();
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
                        کتار_YIELD_IF_USING_PREEMPTION();
                    }
                    وإلا
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #د_که_پای /* configUSE_QUEUE_SETS */

                taskEXIT_CRITICAL( &( px_کتار->x_کتار_قلف ) );
                أرجع pdPASS;
            }
            وإلا
            {
                إذا( x_Ticks_منزل_Wait == ( TickType_t ) 0 )
                {
                    /* The queue was full and no block time is specified (or
                     * the block time has expired) so leave now. */
                    taskEXIT_CRITICAL( &( px_کتار->x_کتار_قلف ) );

                    /* Return to the original privilege level before exiting
                     * the function. */
                    trace_کتار_SEND_FAILED( px_کتار );
                    أرجع err_کتار_ډک;
                }
                وإلا إذا( x_ثبت_وخت_سېټ == pdFALSE )
                {
                    /* The queue was full and a block time was specified so
                     * configure the timeout structure. */
                    v_Task_دننی_سېټ_وخت_بهر_State( &x_وخت_بهر_2 );
                    x_ثبت_وخت_سېټ = pdTRUE;
                }
                وإلا
                {
                    /* Entry time was already set. */
                    mtCOVERAGE_TEST_MARKER();
                }
            }

            /* If queue locks ARE NOT being used:
             * - At this point, the queue is full and entry time has been set
             * - We simply check for a time out, block if not timed out, or
             *   return an error if we have timed out. */
            #إذا ( کتار_USE_LOCKS == 0 )
            {
                /* Update the timeout state to see if it has expired yet. */
                إذا( x_Task_Check_For_وخت_بهر( &x_وخت_بهر_2, &x_Ticks_منزل_Wait ) == pdFALSE )
                {
                    /* Not timed out yet. Block the current task. */
                    trace_BLOCKING_ON_کتار_SEND( px_کتار );
                    v_Task_ځای_پرځای_کړه_On_Event_List( &( px_کتار->x_Tasks_انتظار_منزل_Send ), x_Ticks_منزل_Wait );
                    پورټ_YIELD_WITHIN_API();
                }
                وإلا
                {
                    /* We have timed out. Return an error. */
                    taskEXIT_CRITICAL( &( px_کتار->x_کتار_قلف ) );
                    trace_کتار_SEND_FAILED( px_کتار );
                    أرجع err_کتار_ډک;
                }
            }
            #د_که_پای /* queueUSE_LOCKS == 0 */
        }
        taskEXIT_CRITICAL( &( px_کتار->x_کتار_قلف ) );

        /* If queue locks ARE being used:
         * - At this point, the queue is full and entry time has been set
         * - We follow the original procedure of locking the queue before
         *   attempting to block. */
        #إذا ( کتار_USE_LOCKS == 1 )
        {
            /* Interrupts and other tasks can send to and receive from the queue
             * now the critical section has been exited. */

            vTaskSuspendAll();
            prv_قلف_کتار( px_کتار );

            /* Update the timeout state to see if it has expired yet. */
            إذا( x_Task_Check_For_وخت_بهر( &x_وخت_بهر_2, &x_Ticks_منزل_Wait ) == pdFALSE )
            {
                إذا( prv_Is_کتار_ډک( px_کتار ) != pdFALSE )
                {
                    trace_BLOCKING_ON_کتار_SEND( px_کتار );
                    v_Task_ځای_پرځای_کړه_On_Event_List( &( px_کتار->x_Tasks_انتظار_منزل_Send ), x_Ticks_منزل_Wait );

                    /* Unlocking the queue means queue events can effect the
                     * event list. It is possible that interrupts occurring now
                     * remove this task from the event list again - but as the
                     * scheduler is suspended the task will go onto the pending
                     * ready list instead of the actual ready list. */
                    prv_Unlock_کتار( px_کتار );

                    /* Resuming the scheduler will move tasks from the pending
                     * ready list into the ready list - so it is feasible that this
                     * task is already in the ready list before it yields - in which
                     * case the yield will not cause a context switch unless there
                     * is also a higher priority task in the pending ready list. */
                    إذا( xTaskResumeAll() == pdFALSE )
                    {
                        پورټ_YIELD_WITHIN_API();
                    }
                }
                وإلا
                {
                    /* Try again. */
                    prv_Unlock_کتار( px_کتار );
                    ( فراغ ) xTaskResumeAll();
                }
            }
            وإلا
            {
                /* The timeout has expired. */
                prv_Unlock_کتار( px_کتار );
                ( فراغ ) xTaskResumeAll();

                trace_کتار_SEND_FAILED( px_کتار );
                أرجع err_کتار_ډک;
            }
        }
        #د_که_پای /* queueUSE_LOCKS == 1 */
    } /*lint -restore */
}
/*-----------------------------------------------------------*/

بنسټ_Type_t x_کتار_Generic_Send_From_ISR( کتار_Handle_t x_کتار,
                                     ثابت فراغ * ثابت pv_توکی_منزل_کتار,
                                     بنسټ_Type_t * ثابت px_Higher_لومړیتوب_Task_Woken,
                                     ثابت بنسټ_Type_t xCopyPosition )
{
    بنسټ_Type_t xReturn;
    UBaseType_t ux_Saved_مداخله_حالت;
    کتار_t * ثابت px_کتار = x_کتار;

    configASSERT( px_کتار );
    configASSERT( !( ( pv_توکی_منزل_کتار == عدم ) && ( px_کتار->ux_توکی_اندازه != ( UBaseType_t ) 0U ) ) );
    configASSERT( !( ( xCopyPosition == کتار_OVERWRITE ) && ( px_کتار->ux_اوږدتیا != 1 ) ) );

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
    پورټ_ASSERT_IF_مداخله_لومړیتوب_INVALID();

    /* Similar to xQueueGenericSend, except without blocking if there is no room
     * in the queue.  Also don't directly wake a task that was blocked on a queue
     * read, instead return a flag to say whether a context switch is required or
     * not (i.e. has a task with a higher priority than us been woken by this
     * post). */
    prv_ENTER_CRITICAL_OR_ماسک_ISR( &( px_کتار->x_کتار_قلف ), ux_Saved_مداخله_حالت );
    {
        إذا( ( px_کتار->ux_Messages_انتظار < px_کتار->ux_اوږدتیا ) || ( xCopyPosition == کتار_OVERWRITE ) )
        {
            #إذا ( کتار_USE_LOCKS == 1 )
                ثابت int8_t c_Tx_قلف = px_کتار->c_Tx_قلف;
            #وإلا
                /* Queue locks not used, so we treat it as unlocked. */
                ثابت int8_t c_Tx_قلف = کتار_UNLOCKED;
            #د_که_پای /* queueUSE_LOCKS == 1 */
            ثابت UBaseType_t ux_پخوانی_Messages_انتظار = px_کتار->ux_Messages_انتظار;

            trace_کتار_SEND_FROM_ISR( px_کتار );

            /* Semaphores use xQueueGiveFromISR(), so pxQueue will not be a
             *  semaphore or mutex.  That means prvCopyDataToQueue() cannot result
             *  in a task disinheriting a priority and prvCopyDataToQueue() can be
             *  called here even though the disinherit function does not check if
             *  the scheduler is suspended before accessing the ready lists. */
            ( فراغ ) prv_Copy_معلومات_منزل_کتار( px_کتار, pv_توکی_منزل_کتار, xCopyPosition );

            /* The event list is not altered if the queue is locked.  This will
             * be done when the queue is unlocked later. */
            إذا( c_Tx_قلف == کتار_UNLOCKED )
            {
                #إذا ( config_USE_کتار_SETS == 1 )
                {
                    إذا( px_کتار->px_کتار_سېټ_Container != عدم )
                    {
                        إذا( ( xCopyPosition == کتار_OVERWRITE ) && ( ux_پخوانی_Messages_انتظار != ( UBaseType_t ) 0 ) )
                        {
                            /* Do not notify the queue set as an existing item
                             * was overwritten in the queue so the number of items
                             * in the queue has not changed. */
                            mtCOVERAGE_TEST_MARKER();
                        }
                        وإلا إذا( prv_Notify_کتار_سېټ_Container( px_کتار ) != pdFALSE )
                        {
                            /* The queue is a member of a queue set, and posting
                             * to the queue set caused a higher priority task to
                             * unblock.  A context switch is required. */
                            إذا( px_Higher_لومړیتوب_Task_Woken != عدم )
                            {
                                *px_Higher_لومړیتوب_Task_Woken = pdTRUE;
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
                        إذا( list_LIST_IS_تشه( &( px_کتار->x_Tasks_انتظار_منزل_Receive ) ) == pdFALSE )
                        {
                            إذا( xTaskRemoveFromEventList( &( px_کتار->x_Tasks_انتظار_منزل_Receive ) ) != pdFALSE )
                            {
                                /* The task waiting has a higher priority so
                                 *  record that a context switch is required. */
                                إذا( px_Higher_لومړیتوب_Task_Woken != عدم )
                                {
                                    *px_Higher_لومړیتوب_Task_Woken = pdTRUE;
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
                    إذا( list_LIST_IS_تشه( &( px_کتار->x_Tasks_انتظار_منزل_Receive ) ) == pdFALSE )
                    {
                        إذا( xTaskRemoveFromEventList( &( px_کتار->x_Tasks_انتظار_منزل_Receive ) ) != pdFALSE )
                        {
                            /* The task waiting has a higher priority so record that a
                             * context switch is required. */
                            إذا( px_Higher_لومړیتوب_Task_Woken != عدم )
                            {
                                *px_Higher_لومړیتوب_Task_Woken = pdTRUE;
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
                    ( فراغ ) ux_پخوانی_Messages_انتظار;
                }
                #د_که_پای /* configUSE_QUEUE_SETS */
            }
            وإلا
            {
                #إذا ( کتار_USE_LOCKS == 1 )
                {
                    /* Increment the lock count so the task that unlocks the queue
                     * knows that data was posted while it was locked. */
                    prv_Increment_کتار_Tx_قلف( px_کتار, c_Tx_قلف );
                }
                #د_که_پای /* queueUSE_LOCKS == 1 */
            }

            xReturn = pdPASS;
        }
        وإلا
        {
            trace_کتار_SEND_FROM_ISR_FAILED( px_کتار );
            xReturn = err_کتار_ډک;
        }
    }
    prvEXIT_CRITICAL_OR_UNMASK_ISR( &( px_کتار->x_کتار_قلف ), ux_Saved_مداخله_حالت );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

بنسټ_Type_t x_کتار_Give_From_ISR( کتار_Handle_t x_کتار,
                              بنسټ_Type_t * ثابت px_Higher_لومړیتوب_Task_Woken )
{
    بنسټ_Type_t xReturn;
    UBaseType_t ux_Saved_مداخله_حالت;
    کتار_t * ثابت px_کتار = x_کتار;

    /* Similar to xQueueGenericSendFromISR() but used with semaphores where the
     * item size is 0.  Don't directly wake a task that was blocked on a queue
     * read, instead return a flag to say whether a context switch is required or
     * not (i.e. has a task with a higher priority than us been woken by this
     * post). */

    configASSERT( px_کتار );

    /* xQueueGenericSendFromISR() should be used instead of xQueueGiveFromISR()
     * if the item size is not 0. */
    configASSERT( px_کتار->ux_توکی_اندازه == 0 );

    /* Normally a mutex would not be given from an interrupt, especially if
     * there is a mutex holder, as priority inheritance makes no sense for an
     * interrupts, only tasks. */
    configASSERT( !( ( px_کتار->ux_کتار_Type == کتار_کتار_IS_MUTEX ) && ( px_کتار->u.xSemaphore.xMutexHolder != عدم ) ) );

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
    پورټ_ASSERT_IF_مداخله_لومړیتوب_INVALID();

    prv_ENTER_CRITICAL_OR_ماسک_ISR( &( px_کتار->x_کتار_قلف ), ux_Saved_مداخله_حالت );
    {
        ثابت UBaseType_t ux_Messages_انتظار = px_کتار->ux_Messages_انتظار;

        /* When the queue is used to implement a semaphore no data is ever
         * moved through the queue but it is still valid to see if the queue 'has
         * space'. */
        إذا( ux_Messages_انتظار < px_کتار->ux_اوږدتیا )
        {
            #إذا ( کتار_USE_LOCKS == 1 )
                ثابت int8_t c_Tx_قلف = px_کتار->c_Tx_قلف;
            #وإلا
                /* Queue locks not used, so we treat it as unlocked. */
                ثابت int8_t c_Tx_قلف = کتار_UNLOCKED;
            #د_که_پای /* queueUSE_LOCKS == 1 */

            /* Todo: Reconcile tracing differences (IDF-8183) */
            trace_کتار_GIVE_FROM_ISR( px_کتار );

            /* A task can only have an inherited priority if it is a mutex
             * holder - and if there is a mutex holder then the mutex cannot be
             * given from an ISR.  As this is the ISR version of the function it
             * can be assumed there is no mutex holder and no need to determine if
             * priority disinheritance is needed.  Simply increase the count of
             * messages (semaphores) available. */
            px_کتار->ux_Messages_انتظار = ux_Messages_انتظار + ( UBaseType_t ) 1;

            /* The event list is not altered if the queue is locked.  This will
             * be done when the queue is unlocked later. */
            إذا( c_Tx_قلف == کتار_UNLOCKED )
            {
                #إذا ( config_USE_کتار_SETS == 1 )
                {
                    إذا( px_کتار->px_کتار_سېټ_Container != عدم )
                    {
                        إذا( prv_Notify_کتار_سېټ_Container( px_کتار ) != pdFALSE )
                        {
                            /* The semaphore is a member of a queue set, and
                             * posting to the queue set caused a higher priority
                             * task to unblock.  A context switch is required. */
                            إذا( px_Higher_لومړیتوب_Task_Woken != عدم )
                            {
                                *px_Higher_لومړیتوب_Task_Woken = pdTRUE;
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
                        إذا( list_LIST_IS_تشه( &( px_کتار->x_Tasks_انتظار_منزل_Receive ) ) == pdFALSE )
                        {
                            إذا( xTaskRemoveFromEventList( &( px_کتار->x_Tasks_انتظار_منزل_Receive ) ) != pdFALSE )
                            {
                                /* The task waiting has a higher priority so
                                 *  record that a context switch is required. */
                                إذا( px_Higher_لومړیتوب_Task_Woken != عدم )
                                {
                                    *px_Higher_لومړیتوب_Task_Woken = pdTRUE;
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
                    إذا( list_LIST_IS_تشه( &( px_کتار->x_Tasks_انتظار_منزل_Receive ) ) == pdFALSE )
                    {
                        إذا( xTaskRemoveFromEventList( &( px_کتار->x_Tasks_انتظار_منزل_Receive ) ) != pdFALSE )
                        {
                            /* The task waiting has a higher priority so record that a
                             * context switch is required. */
                            إذا( px_Higher_لومړیتوب_Task_Woken != عدم )
                            {
                                *px_Higher_لومړیتوب_Task_Woken = pdTRUE;
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
                #د_که_پای /* configUSE_QUEUE_SETS */
            }
            وإلا
            {
                #إذا ( کتار_USE_LOCKS == 1 )
                {
                    /* Increment the lock count so the task that unlocks the queue
                     * knows that data was posted while it was locked. */
                    prv_Increment_کتار_Tx_قلف( px_کتار, c_Tx_قلف );
                }
                #د_که_پای /* queueUSE_LOCKS == 1 */
            }

            xReturn = pdPASS;
        }
        وإلا
        {
            /* Todo: Reconcile tracing differences (IDF-8183) */
            trace_کتار_GIVE_FROM_ISR_FAILED( px_کتار );
            xReturn = err_کتار_ډک;
        }
    }
    prvEXIT_CRITICAL_OR_UNMASK_ISR( &( px_کتار->x_کتار_قلف ), ux_Saved_مداخله_حالت );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

بنسټ_Type_t x_کتار_Receive( کتار_Handle_t x_کتار,
                          فراغ * ثابت pv_بفر,
                          TickType_t x_Ticks_منزل_Wait )
{
    بنسټ_Type_t x_ثبت_وخت_سېټ = pdFALSE;
    وخت_بهر_t x_وخت_بهر_2;
    کتار_t * ثابت px_کتار = x_کتار;

    /* Check the pointer is not NULL. */
    configASSERT( ( px_کتار ) );

    /* The buffer into which data is received can only be NULL if the data size
     * is zero (so no data is copied into the buffer). */
    configASSERT( !( ( ( pv_بفر ) == عدم ) && ( ( px_کتار )->ux_توکی_اندازه != ( UBaseType_t ) 0U ) ) );

    /* Cannot block if the scheduler is suspended. */
    #إذا ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_منزل_Wait != 0 ) ) );
    }
    #د_که_پای

    /*lint -save -e904  This function relaxes the coding standard somewhat to
     * allow return statements within the function itself.  This is done in the
     * interest of execution time efficiency. */
    لكل( ; ; )
    {
        taskENTER_CRITICAL( &( px_کتار->x_کتار_قلف ) );
        {
            ثابت UBaseType_t ux_Messages_انتظار = px_کتار->ux_Messages_انتظار;

            /* Is there data in the queue now?  To be running the calling task
             * must be the highest priority task wanting to access the queue. */
            إذا( ux_Messages_انتظار > ( UBaseType_t ) 0 )
            {
                /* Data available, remove one item. */
                prv_Copy_معلومات_From_کتار( px_کتار, pv_بفر );
                trace_کتار_RECEIVE( px_کتار );
                px_کتار->ux_Messages_انتظار = ux_Messages_انتظار - ( UBaseType_t ) 1;

                /* There is now space in the queue, were any tasks waiting to
                 * post to the queue?  If so, unblock the highest priority waiting
                 * task. */
                إذا( list_LIST_IS_تشه( &( px_کتار->x_Tasks_انتظار_منزل_Send ) ) == pdFALSE )
                {
                    إذا( xTaskRemoveFromEventList( &( px_کتار->x_Tasks_انتظار_منزل_Send ) ) != pdFALSE )
                    {
                        کتار_YIELD_IF_USING_PREEMPTION();
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

                taskEXIT_CRITICAL( &( px_کتار->x_کتار_قلف ) );
                أرجع pdPASS;
            }
            وإلا
            {
                إذا( x_Ticks_منزل_Wait == ( TickType_t ) 0 )
                {
                    /* The queue was empty and no block time is specified (or
                     * the block time has expired) so leave now. */
                    taskEXIT_CRITICAL( &( px_کتار->x_کتار_قلف ) );
                    trace_کتار_RECEIVE_FAILED( px_کتار );
                    أرجع err_کتار_تشه;
                }
                وإلا إذا( x_ثبت_وخت_سېټ == pdFALSE )
                {
                    /* The queue was empty and a block time was specified so
                     * configure the timeout structure. */
                    v_Task_دننی_سېټ_وخت_بهر_State( &x_وخت_بهر_2 );
                    x_ثبت_وخت_سېټ = pdTRUE;
                }
                وإلا
                {
                    /* Entry time was already set. */
                    mtCOVERAGE_TEST_MARKER();
                }
            }

            /* If queue locks ARE NOT being used:
             * - At this point, the queue is empty and entry time has been set
             * - We simply check for a time out, block if not timed out, or
             *   return an error if we have timed out. */
            #إذا ( کتار_USE_LOCKS == 0 )
            {
                /* Update the timeout state to see if it has expired yet. */
                إذا( x_Task_Check_For_وخت_بهر( &x_وخت_بهر_2, &x_Ticks_منزل_Wait ) == pdFALSE )
                {
                    /* Not timed out yet. Block the current task. */
                    trace_BLOCKING_ON_کتار_RECEIVE( px_کتار );
                    v_Task_ځای_پرځای_کړه_On_Event_List( &( px_کتار->x_Tasks_انتظار_منزل_Receive ), x_Ticks_منزل_Wait );
                    پورټ_YIELD_WITHIN_API();
                }
                وإلا
                {
                    /* We have timed out. Return an error. */
                    taskEXIT_CRITICAL( &( px_کتار->x_کتار_قلف ) );
                    trace_کتار_RECEIVE_FAILED( px_کتار );
                    أرجع err_کتار_تشه;
                }
            }
            #د_که_پای /* queueUSE_LOCKS == 0 */
        }
        taskEXIT_CRITICAL( &( px_کتار->x_کتار_قلف ) );

        /* If queue locks ARE being used:
         * - At this point, the queue is empty and entry time has been set
         * - We follow the original procedure for locking the queue before
         *   attempting to block. */
        #إذا ( کتار_USE_LOCKS == 1 )
        {
            /* Interrupts and other tasks can send to and receive from the queue
             * now the critical section has been exited. */

            vTaskSuspendAll();
            prv_قلف_کتار( px_کتار );

            /* Update the timeout state to see if it has expired yet. */
            إذا( x_Task_Check_For_وخت_بهر( &x_وخت_بهر_2, &x_Ticks_منزل_Wait ) == pdFALSE )
            {
                /* The timeout has not expired.  If the queue is still empty place
                 * the task on the list of tasks waiting to receive from the queue. */
                إذا( prv_Is_کتار_تشه( px_کتار ) != pdFALSE )
                {
                    trace_BLOCKING_ON_کتار_RECEIVE( px_کتار );
                    v_Task_ځای_پرځای_کړه_On_Event_List( &( px_کتار->x_Tasks_انتظار_منزل_Receive ), x_Ticks_منزل_Wait );
                    prv_Unlock_کتار( px_کتار );

                    إذا( xTaskResumeAll() == pdFALSE )
                    {
                        پورټ_YIELD_WITHIN_API();
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
                    prv_Unlock_کتار( px_کتار );
                    ( فراغ ) xTaskResumeAll();
                }
            }
            وإلا
            {
                /* Timed out.  If there is no data in the queue exit, otherwise loop
                 * back and attempt to read the data. */
                prv_Unlock_کتار( px_کتار );
                ( فراغ ) xTaskResumeAll();

                إذا( prv_Is_کتار_تشه( px_کتار ) != pdFALSE )
                {
                    trace_کتار_RECEIVE_FAILED( px_کتار );
                    أرجع err_کتار_تشه;
                }
                وإلا
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
        }
        #د_که_پای /* queueUSE_LOCKS == 1 */
    } /*lint -restore */
}
/*-----------------------------------------------------------*/

بنسټ_Type_t x_کتار_Semaphore_Take( کتار_Handle_t x_کتار,
                                TickType_t x_Ticks_منزل_Wait )
{
    بنسټ_Type_t x_ثبت_وخت_سېټ = pdFALSE;
    وخت_بهر_t x_وخت_بهر_2;
    کتار_t * ثابت px_کتار = x_کتار;

    #إذا ( configUSE_MUTEXES == 1 )
        بنسټ_Type_t xInheritanceOccurred = pdFALSE;
    #د_که_پای

    /* Check the queue pointer is not NULL. */
    configASSERT( ( px_کتار ) );

    /* Check this really is a semaphore, in which case the item size will be
     * 0. */
    configASSERT( px_کتار->ux_توکی_اندازه == 0 );

    /* Cannot block if the scheduler is suspended. */
    #إذا ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_منزل_Wait != 0 ) ) );
    }
    #د_که_پای

    /*lint -save -e904 This function relaxes the coding standard somewhat to allow return
     * statements within the function itself.  This is done in the interest
     * of execution time efficiency. */
    لكل( ; ; )
    {
        taskENTER_CRITICAL( &( px_کتار->x_کتار_قلف ) );
        {
            /* Semaphores are queues with an item size of 0, and where the
             * number of messages in the queue is the semaphore's count value. */
            ثابت UBaseType_t ux_Semaphore_شمېر = px_کتار->ux_Messages_انتظار;

            /* Is there data in the queue now?  To be running the calling task
             * must be the highest priority task wanting to access the queue. */
            إذا( ux_Semaphore_شمېر > ( UBaseType_t ) 0 )
            {
                /* Todo: Reconcile tracing differences (IDF-8183) */
                trace_کتار_SEMAPHORE_RECEIVE( px_کتار );

                /* Semaphores are queues with a data size of zero and where the
                 * messages waiting is the semaphore's count.  Reduce the count. */
                px_کتار->ux_Messages_انتظار = ux_Semaphore_شمېر - ( UBaseType_t ) 1;

                #إذا ( configUSE_MUTEXES == 1 )
                {
                    إذا( px_کتار->ux_کتار_Type == کتار_کتار_IS_MUTEX )
                    {
                        /* Record the information required to implement
                         * priority inheritance should it become necessary. */
                        px_کتار->u.xSemaphore.xMutexHolder = pv_Task_Increment_Mutex_Held_شمېر();
                    }
                    وإلا
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #د_که_پای /* configUSE_MUTEXES */

                /* Check to see if other tasks are blocked waiting to give the
                 * semaphore, and if so, unblock the highest priority such task. */
                إذا( list_LIST_IS_تشه( &( px_کتار->x_Tasks_انتظار_منزل_Send ) ) == pdFALSE )
                {
                    إذا( xTaskRemoveFromEventList( &( px_کتار->x_Tasks_انتظار_منزل_Send ) ) != pdFALSE )
                    {
                        کتار_YIELD_IF_USING_PREEMPTION();
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

                taskEXIT_CRITICAL( &( px_کتار->x_کتار_قلف ) );
                أرجع pdPASS;
            }
            وإلا
            {
                إذا( x_Ticks_منزل_Wait == ( TickType_t ) 0 )
                {
                    /* The semaphore count was 0 and no block time is specified
                     * (or the block time has expired) so exit now. */
                    taskEXIT_CRITICAL( &( px_کتار->x_کتار_قلف ) );
                    trace_کتار_RECEIVE_FAILED( px_کتار );
                    أرجع err_کتار_تشه;
                }
                وإلا إذا( x_ثبت_وخت_سېټ == pdFALSE )
                {
                    /* The semaphore count was 0 and a block time was specified
                     * so configure the timeout structure ready to block. */
                    v_Task_دننی_سېټ_وخت_بهر_State( &x_وخت_بهر_2 );
                    x_ثبت_وخت_سېټ = pdTRUE;
                }
                وإلا
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
            #إذا ( کتار_USE_LOCKS == 0 )
            {
                /* Update the timeout state to see if it has expired yet. */
                إذا( x_Task_Check_For_وخت_بهر( &x_وخت_بهر_2, &x_Ticks_منزل_Wait ) == pdFALSE )
                {
                    /* Not timed out yet. If this is a mutex, make the holder
                     * inherit our priority, then block the current task. */
                    trace_BLOCKING_ON_کتار_RECEIVE( px_کتار );
                    #إذا ( configUSE_MUTEXES == 1 )
                    {
                        إذا( px_کتار->ux_کتار_Type == کتار_کتار_IS_MUTEX )
                        {
                            xInheritanceOccurred = x_Task_لومړیتوب_Inherit( px_کتار->u.xSemaphore.xMutexHolder );
                        }
                        وإلا
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    #د_که_پای /* if ( configUSE_MUTEXES == 1 ) */
                    v_Task_ځای_پرځای_کړه_On_Event_List( &( px_کتار->x_Tasks_انتظار_منزل_Receive ), x_Ticks_منزل_Wait );
                    پورټ_YIELD_WITHIN_API();
                }
                وإلا
                {
                    /* We have timed out. If this is a mutex, make the holder
                     * disinherit our priority, then return an error. */
                    #إذا ( configUSE_MUTEXES == 1 )
                    {
                        إذا( xInheritanceOccurred != pdFALSE )
                        {
                            UBaseType_t ux_Highest_انتظار_لومړیتوب;
                            ux_Highest_انتظار_لومړیتوب = prv_Get_Disinherit_لومړیتوب_After_Timeout( px_کتار );
                            v_Task_لومړیتوب_Disinherit_After_Timeout( px_کتار->u.xSemaphore.xMutexHolder, ux_Highest_انتظار_لومړیتوب );
                        }
                    }
                    #د_که_پای /* configUSE_MUTEXES */
                    taskEXIT_CRITICAL( &( px_کتار->x_کتار_قلف ) );
                    trace_کتار_RECEIVE_FAILED( px_کتار );
                    أرجع err_کتار_تشه;
                }
            }
            #د_که_پای /* queueUSE_LOCKS == 0 */
        }
        taskEXIT_CRITICAL( &( px_کتار->x_کتار_قلف ) );

        /* If queue locks ARE being used:
         * - At this point, the semaphore/mutex is empty/held and entry time
         *   has been set.
         * - We follow the original procedure for locking the queue, inheriting
         *   priority, then attempting to block. */
        #إذا ( کتار_USE_LOCKS == 1 )
        {
            /* Interrupts and other tasks can give to and take from the semaphore
             * now the critical section has been exited. */

            vTaskSuspendAll();
            prv_قلف_کتار( px_کتار );

            /* Update the timeout state to see if it has expired yet. */
            إذا( x_Task_Check_For_وخت_بهر( &x_وخت_بهر_2, &x_Ticks_منزل_Wait ) == pdFALSE )
            {
                /* A block time is specified and not expired.  If the semaphore
                 * count is 0 then enter the Blocked state to wait for a semaphore to
                 * become available.  As semaphores are implemented with queues the
                 * queue being empty is equivalent to the semaphore count being 0. */
                إذا( prv_Is_کتار_تشه( px_کتار ) != pdFALSE )
                {
                    trace_BLOCKING_ON_کتار_RECEIVE( px_کتار );

                    #إذا ( configUSE_MUTEXES == 1 )
                    {
                        إذا( px_کتار->ux_کتار_Type == کتار_کتار_IS_MUTEX )
                        {
                            taskENTER_CRITICAL( &( px_کتار->x_کتار_قلف ) );
                            {
                                xInheritanceOccurred = x_Task_لومړیتوب_Inherit( px_کتار->u.xSemaphore.xMutexHolder );
                            }
                            taskEXIT_CRITICAL( &( px_کتار->x_کتار_قلف ) );
                        }
                        وإلا
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    #د_که_پای /* if ( configUSE_MUTEXES == 1 ) */

                    v_Task_ځای_پرځای_کړه_On_Event_List( &( px_کتار->x_Tasks_انتظار_منزل_Receive ), x_Ticks_منزل_Wait );
                    prv_Unlock_کتار( px_کتار );

                    إذا( xTaskResumeAll() == pdFALSE )
                    {
                        پورټ_YIELD_WITHIN_API();
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
                    prv_Unlock_کتار( px_کتار );
                    ( فراغ ) xTaskResumeAll();
                }
            }
            وإلا
            {
                /* Timed out. */
                prv_Unlock_کتار( px_کتار );
                ( فراغ ) xTaskResumeAll();

                /* If the semaphore count is 0 exit now as the timeout has
                 * expired.  Otherwise return to attempt to take the semaphore that is
                 * known to be available.  As semaphores are implemented by queues the
                 * queue being empty is equivalent to the semaphore count being 0. */
                إذا( prv_Is_کتار_تشه( px_کتار ) != pdFALSE )
                {
                    #إذا ( configUSE_MUTEXES == 1 )
                    {
                        /* xInheritanceOccurred could only have be set if
                         * pxQueue->uxQueueType == queueQUEUE_IS_MUTEX so no need to
                         * test the mutex type again to check it is actually a mutex. */
                        إذا( xInheritanceOccurred != pdFALSE )
                        {
                            taskENTER_CRITICAL( &( px_کتار->x_کتار_قلف ) );
                            {
                                UBaseType_t ux_Highest_انتظار_لومړیتوب;

                                /* This task blocking on the mutex caused another
                                 * task to inherit this task's priority.  Now this task
                                 * has timed out the priority should be disinherited
                                 * again, but only as low as the next highest priority
                                 * task that is waiting for the same mutex. */
                                ux_Highest_انتظار_لومړیتوب = prv_Get_Disinherit_لومړیتوب_After_Timeout( px_کتار );
                                v_Task_لومړیتوب_Disinherit_After_Timeout( px_کتار->u.xSemaphore.xMutexHolder, ux_Highest_انتظار_لومړیتوب );
                            }
                            taskEXIT_CRITICAL( &( px_کتار->x_کتار_قلف ) );
                        }
                    }
                    #د_که_پای /* configUSE_MUTEXES */

                    trace_کتار_RECEIVE_FAILED( px_کتار );
                    أرجع err_کتار_تشه;
                }
                وإلا
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
        }
        #د_که_پای /* queueUSE_LOCKS == 1 */
    } /*lint -restore */
}
/*-----------------------------------------------------------*/

بنسټ_Type_t x_کتار_Peek( کتار_Handle_t x_کتار,
                       فراغ * ثابت pv_بفر,
                       TickType_t x_Ticks_منزل_Wait )
{
    بنسټ_Type_t x_ثبت_وخت_سېټ = pdFALSE;
    وخت_بهر_t x_وخت_بهر_2;
    int8_t * pcOriginalReadPosition;
    کتار_t * ثابت px_کتار = x_کتار;

    /* Check the pointer is not NULL. */
    configASSERT( ( px_کتار ) );

    /* The buffer into which data is received can only be NULL if the data size
     * is zero (so no data is copied into the buffer. */
    configASSERT( !( ( ( pv_بفر ) == عدم ) && ( ( px_کتار )->ux_توکی_اندازه != ( UBaseType_t ) 0U ) ) );

    /* Cannot block if the scheduler is suspended. */
    #إذا ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_منزل_Wait != 0 ) ) );
    }
    #د_که_پای

    /*lint -save -e904  This function relaxes the coding standard somewhat to
     * allow return statements within the function itself.  This is done in the
     * interest of execution time efficiency. */
    لكل( ; ; )
    {
        taskENTER_CRITICAL( &( px_کتار->x_کتار_قلف ) );
        {
            ثابت UBaseType_t ux_Messages_انتظار = px_کتار->ux_Messages_انتظار;

            /* Is there data in the queue now?  To be running the calling task
             * must be the highest priority task wanting to access the queue. */
            إذا( ux_Messages_انتظار > ( UBaseType_t ) 0 )
            {
                /* Remember the read position so it can be reset after the data
                 * is read from the queue as this function is only peeking the
                 * data, not removing it. */
                pcOriginalReadPosition = px_کتار->u.x_کتار.pcReadFrom;

                prv_Copy_معلومات_From_کتار( px_کتار, pv_بفر );
                trace_کتار_PEEK( px_کتار );

                /* The data is not being removed, so reset the read pointer. */
                px_کتار->u.x_کتار.pcReadFrom = pcOriginalReadPosition;

                /* The data is being left in the queue, so see if there are
                 * any other tasks waiting for the data. */
                إذا( list_LIST_IS_تشه( &( px_کتار->x_Tasks_انتظار_منزل_Receive ) ) == pdFALSE )
                {
                    إذا( xTaskRemoveFromEventList( &( px_کتار->x_Tasks_انتظار_منزل_Receive ) ) != pdFALSE )
                    {
                        /* The task waiting has a higher priority than this task. */
                        کتار_YIELD_IF_USING_PREEMPTION();
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

                taskEXIT_CRITICAL( &( px_کتار->x_کتار_قلف ) );
                أرجع pdPASS;
            }
            وإلا
            {
                إذا( x_Ticks_منزل_Wait == ( TickType_t ) 0 )
                {
                    /* The queue was empty and no block time is specified (or
                     * the block time has expired) so leave now. */
                    taskEXIT_CRITICAL( &( px_کتار->x_کتار_قلف ) );
                    trace_کتار_PEEK_FAILED( px_کتار );
                    أرجع err_کتار_تشه;
                }
                وإلا إذا( x_ثبت_وخت_سېټ == pdFALSE )
                {
                    /* The queue was empty and a block time was specified so
                     * configure the timeout structure ready to enter the blocked
                     * state. */
                    v_Task_دننی_سېټ_وخت_بهر_State( &x_وخت_بهر_2 );
                    x_ثبت_وخت_سېټ = pdTRUE;
                }
                وإلا
                {
                    /* Entry time was already set. */
                    mtCOVERAGE_TEST_MARKER();
                }
            }

            /* If queue locks ARE NOT being used:
             * - At this point, the queue is empty and entry time has been set
             * - We simply check for a time out, block if not timed out, or
             *   return an error if we have timed out. */
            #إذا ( کتار_USE_LOCKS == 0 )
            {
                /* Update the timeout state to see if it has expired yet. */
                إذا( x_Task_Check_For_وخت_بهر( &x_وخت_بهر_2, &x_Ticks_منزل_Wait ) == pdFALSE )
                {
                    /* Not timed out yet. Block the current task. */
                    trace_BLOCKING_ON_کتار_PEEK( px_کتار );
                    v_Task_ځای_پرځای_کړه_On_Event_List( &( px_کتار->x_Tasks_انتظار_منزل_Receive ), x_Ticks_منزل_Wait );
                    پورټ_YIELD_WITHIN_API();
                }
                وإلا
                {
                    /* We have timed out. Return an error. */
                    taskEXIT_CRITICAL( &( px_کتار->x_کتار_قلف ) );
                    trace_کتار_PEEK_FAILED( px_کتار );
                    أرجع err_کتار_تشه;
                }
            }
            #د_که_پای /* queueUSE_LOCKS == 0 */
        }
        taskEXIT_CRITICAL( &( px_کتار->x_کتار_قلف ) );

        /* If queue locks ARE being used:
         * - At this point, the queue is empty and entry time has been set
         * - We follow the original procedure for locking the queue before
         *   attempting to block. */
        #إذا ( کتار_USE_LOCKS == 1 )
        {
            /* Interrupts and other tasks can send to and receive from the queue
             * now that the critical section has been exited. */

            vTaskSuspendAll();
            prv_قلف_کتار( px_کتار );

            /* Update the timeout state to see if it has expired yet. */
            إذا( x_Task_Check_For_وخت_بهر( &x_وخت_بهر_2, &x_Ticks_منزل_Wait ) == pdFALSE )
            {
                /* Timeout has not expired yet, check to see if there is data in the
                * queue now, and if not enter the Blocked state to wait for data. */
                إذا( prv_Is_کتار_تشه( px_کتار ) != pdFALSE )
                {
                    trace_BLOCKING_ON_کتار_PEEK( px_کتار );
                    v_Task_ځای_پرځای_کړه_On_Event_List( &( px_کتار->x_Tasks_انتظار_منزل_Receive ), x_Ticks_منزل_Wait );
                    prv_Unlock_کتار( px_کتار );

                    إذا( xTaskResumeAll() == pdFALSE )
                    {
                        پورټ_YIELD_WITHIN_API();
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
                    prv_Unlock_کتار( px_کتار );
                    ( فراغ ) xTaskResumeAll();
                }
            }
            وإلا
            {
                /* The timeout has expired.  If there is still no data in the queue
                 * exit, otherwise go back and try to read the data again. */
                prv_Unlock_کتار( px_کتار );
                ( فراغ ) xTaskResumeAll();

                إذا( prv_Is_کتار_تشه( px_کتار ) != pdFALSE )
                {
                    trace_کتار_PEEK_FAILED( px_کتار );
                    أرجع err_کتار_تشه;
                }
                وإلا
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
        }
        #د_که_پای /* queueUSE_LOCKS == 1 */
    } /*lint -restore */
}
/*-----------------------------------------------------------*/

بنسټ_Type_t x_کتار_Receive_From_ISR( کتار_Handle_t x_کتار,
                                 فراغ * ثابت pv_بفر,
                                 بنسټ_Type_t * ثابت px_Higher_لومړیتوب_Task_Woken )
{
    بنسټ_Type_t xReturn;
    UBaseType_t ux_Saved_مداخله_حالت;
    کتار_t * ثابت px_کتار = x_کتار;

    configASSERT( px_کتار );
    configASSERT( !( ( pv_بفر == عدم ) && ( px_کتار->ux_توکی_اندازه != ( UBaseType_t ) 0U ) ) );

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
    پورټ_ASSERT_IF_مداخله_لومړیتوب_INVALID();

    prv_ENTER_CRITICAL_OR_ماسک_ISR( &( px_کتار->x_کتار_قلف ), ux_Saved_مداخله_حالت );
    {
        ثابت UBaseType_t ux_Messages_انتظار = px_کتار->ux_Messages_انتظار;

        /* Cannot block in an ISR, so check there is data available. */
        إذا( ux_Messages_انتظار > ( UBaseType_t ) 0 )
        {
            #إذا ( کتار_USE_LOCKS == 1 )
                ثابت int8_t c_Rx_قلف = px_کتار->c_Rx_قلف;
            #وإلا
                /* Queue locks not used, so we treat it as unlocked. */
                ثابت int8_t c_Rx_قلف = کتار_UNLOCKED;
            #د_که_پای /* queueUSE_LOCKS == 1 */

            trace_کتار_RECEIVE_FROM_ISR( px_کتار );

            prv_Copy_معلومات_From_کتار( px_کتار, pv_بفر );
            px_کتار->ux_Messages_انتظار = ux_Messages_انتظار - ( UBaseType_t ) 1;

            /* If the queue is locked the event list will not be modified.
             * Instead update the lock count so the task that unlocks the queue
             * will know that an ISR has removed data while the queue was
             * locked. */
            إذا( c_Rx_قلف == کتار_UNLOCKED )
            {
                إذا( list_LIST_IS_تشه( &( px_کتار->x_Tasks_انتظار_منزل_Send ) ) == pdFALSE )
                {
                    إذا( xTaskRemoveFromEventList( &( px_کتار->x_Tasks_انتظار_منزل_Send ) ) != pdFALSE )
                    {
                        /* The task waiting has a higher priority than us so
                         * force a context switch. */
                        إذا( px_Higher_لومړیتوب_Task_Woken != عدم )
                        {
                            *px_Higher_لومړیتوب_Task_Woken = pdTRUE;
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
                #إذا ( کتار_USE_LOCKS == 1 )
                {
                    /* Increment the lock count so the task that unlocks the queue
                     * knows that data was removed while it was locked. */
                    prv_Increment_کتار_Rx_قلف( px_کتار, c_Rx_قلف );
                }
                #د_که_پای /* queueUSE_LOCKS == 1 */
            }

            xReturn = pdPASS;
        }
        وإلا
        {
            xReturn = pdFAIL;
            trace_کتار_RECEIVE_FROM_ISR_FAILED( px_کتار );
        }
    }
    prvEXIT_CRITICAL_OR_UNMASK_ISR( &( px_کتار->x_کتار_قلف ), ux_Saved_مداخله_حالت );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

بنسټ_Type_t x_کتار_Peek_From_ISR( کتار_Handle_t x_کتار,
                              فراغ * ثابت pv_بفر )
{
    بنسټ_Type_t xReturn;
    UBaseType_t ux_Saved_مداخله_حالت;
    int8_t * pcOriginalReadPosition;
    کتار_t * ثابت px_کتار = x_کتار;

    configASSERT( px_کتار );
    configASSERT( !( ( pv_بفر == عدم ) && ( px_کتار->ux_توکی_اندازه != ( UBaseType_t ) 0U ) ) );
    configASSERT( px_کتار->ux_توکی_اندازه != 0 ); /* Can't peek a semaphore. */

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
    پورټ_ASSERT_IF_مداخله_لومړیتوب_INVALID();

    prv_ENTER_CRITICAL_OR_ماسک_ISR( &( px_کتار->x_کتار_قلف ), ux_Saved_مداخله_حالت );
    {
        /* Cannot block in an ISR, so check there is data available. */
        إذا( px_کتار->ux_Messages_انتظار > ( UBaseType_t ) 0 )
        {
            trace_کتار_PEEK_FROM_ISR( px_کتار );

            /* Remember the read position so it can be reset as nothing is
             * actually being removed from the queue. */
            pcOriginalReadPosition = px_کتار->u.x_کتار.pcReadFrom;
            prv_Copy_معلومات_From_کتار( px_کتار, pv_بفر );
            px_کتار->u.x_کتار.pcReadFrom = pcOriginalReadPosition;

            xReturn = pdPASS;
        }
        وإلا
        {
            xReturn = pdFAIL;
            trace_کتار_PEEK_FROM_ISR_FAILED( px_کتار );
        }
    }
    prvEXIT_CRITICAL_OR_UNMASK_ISR( &( px_کتار->x_کتار_قلف ), ux_Saved_مداخله_حالت );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

UBaseType_t ux_کتار_Messages_انتظار( ثابت کتار_Handle_t x_کتار )
{
    UBaseType_t uxReturn;

    configASSERT( x_کتار );

    taskENTER_CRITICAL( &( ( ( کتار_t * ) x_کتار )->x_کتار_قلف ) );
    {
        uxReturn = ( ( کتار_t * ) x_کتار )->ux_Messages_انتظار;
    }
    taskEXIT_CRITICAL( &( ( ( کتار_t * ) x_کتار )->x_کتار_قلف ) );

    أرجع uxReturn;
} /*lint !e818 Pointer cannot be declared const as xQueue is a typedef not pointer. */
/*-----------------------------------------------------------*/

UBaseType_t ux_کتار_Spaces_شته( ثابت کتار_Handle_t x_کتار )
{
    UBaseType_t uxReturn;
    کتار_t * ثابت px_کتار = x_کتار;

    configASSERT( px_کتار );

    taskENTER_CRITICAL( &( px_کتار->x_کتار_قلف ) );
    {
        uxReturn = px_کتار->ux_اوږدتیا - px_کتار->ux_Messages_انتظار;
    }
    taskEXIT_CRITICAL( &( px_کتار->x_کتار_قلف ) );

    أرجع uxReturn;
} /*lint !e818 Pointer cannot be declared const as xQueue is a typedef not pointer. */
/*-----------------------------------------------------------*/

UBaseType_t ux_کتار_Messages_انتظار_From_ISR( ثابت کتار_Handle_t x_کتار )
{
    UBaseType_t uxReturn;
    کتار_t * ثابت px_کتار = x_کتار;

    configASSERT( px_کتار );
    uxReturn = px_کتار->ux_Messages_انتظار;

    أرجع uxReturn;
} /*lint !e818 Pointer cannot be declared const as xQueue is a typedef not pointer. */
/*-----------------------------------------------------------*/

فراغ v_کتار_Delete( کتار_Handle_t x_کتار )
{
    کتار_t * ثابت px_کتار = x_کتار;

    configASSERT( px_کتار );
    trace_کتار_DELETE( px_کتار );

    #إذا ( config_کتار_REGISTRY_اندازه > 0 )
    {
        v_کتار_Unregister_کتار( px_کتار );
    }
    #د_که_پای

    #إذا ( ( config_SUPPORT_DYNAMIC_ویش == 1 ) && ( config_SUPPORT_STATIC_ویش == 0 ) )
    {
        /* The queue can only have been allocated dynamically - free it
         * again. */
        v_پورټ_Free( px_کتار );
    }
    #وإلاإذا ( ( config_SUPPORT_DYNAMIC_ویش == 1 ) && ( config_SUPPORT_STATIC_ویش == 1 ) )
    {
        /* The queue could have been allocated statically or dynamically, so
         * check before attempting to free the memory. */
        إذا( px_کتار->ucStaticallyAllocated == ( uint8_t ) pdFALSE )
        {
            v_پورټ_Free( px_کتار );
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
        ( فراغ ) px_کتار;
    }
    #د_که_پای /* configSUPPORT_DYNAMIC_ALLOCATION */
}
/*-----------------------------------------------------------*/

#إذا ( configUSE_TRACE_FACILITY == 1 )

    UBaseType_t ux_کتار_Get_کتار_شمېره( کتار_Handle_t x_کتار )
    {
        أرجع ( ( کتار_t * ) x_کتار )->ux_کتار_شمېره;
    }

#د_که_پای /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#إذا ( configUSE_TRACE_FACILITY == 1 )

    فراغ v_کتار_سېټ_کتار_شمېره( کتار_Handle_t x_کتار,
                               UBaseType_t ux_کتار_شمېره )
    {
        ( ( کتار_t * ) x_کتار )->ux_کتار_شمېره = ux_کتار_شمېره;
    }

#د_که_پای /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#إذا ( configUSE_TRACE_FACILITY == 1 )

    uint8_t uc_کتار_Get_کتار_Type( کتار_Handle_t x_کتار )
    {
        أرجع ( ( کتار_t * ) x_کتار )->uc_کتار_Type;
    }

#د_که_پای /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#إذا ( configUSE_MUTEXES == 1 )

    ساكن UBaseType_t prv_Get_Disinherit_لومړیتوب_After_Timeout( ثابت کتار_t * ثابت px_کتار )
    {
        UBaseType_t ux_Highest_لومړیتوب_د_انتظار_Tasks;

        /* If a task waiting for a mutex causes the mutex holder to inherit a
         * priority, but the waiting task times out, then the holder should
         * disinherit the priority - but only down to the highest priority of any
         * other tasks that are waiting for the same mutex.  For this purpose,
         * return the priority of the highest priority task that is waiting for the
         * mutex. */
        إذا( list_CURRENT_LIST_اوږدتیا( &( px_کتار->x_Tasks_انتظار_منزل_Receive ) ) > 0U )
        {
            ux_Highest_لومړیتوب_د_انتظار_Tasks = ( UBaseType_t ) configMAX_PRIORITIES - ( UBaseType_t ) list_GET_توکی_ارزښت_د_سر_ثبت( &( px_کتار->x_Tasks_انتظار_منزل_Receive ) );
        }
        وإلا
        {
            ux_Highest_لومړیتوب_د_انتظار_Tasks = tsk_IDLE_لومړیتوب;
        }

        أرجع ux_Highest_لومړیتوب_د_انتظار_Tasks;
    }

#د_که_پای /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

ساكن بنسټ_Type_t prv_Copy_معلومات_منزل_کتار( کتار_t * ثابت px_کتار,
                                      ثابت فراغ * pv_توکی_منزل_کتار,
                                      ثابت بنسټ_Type_t xPosition )
{
    بنسټ_Type_t xReturn = pdFALSE;
    UBaseType_t ux_Messages_انتظار;

    /* This function is called from a critical section. */

    ux_Messages_انتظار = px_کتار->ux_Messages_انتظار;

    إذا( px_کتار->ux_توکی_اندازه == ( UBaseType_t ) 0 )
    {
        #إذا ( configUSE_MUTEXES == 1 )
        {
            إذا( px_کتار->ux_کتار_Type == کتار_کتار_IS_MUTEX )
            {
                /* The mutex is no longer being held. */
                xReturn = x_Task_لومړیتوب_Disinherit( px_کتار->u.xSemaphore.xMutexHolder );
                px_کتار->u.xSemaphore.xMutexHolder = عدم;
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        #د_که_پای /* configUSE_MUTEXES */
    }
    وإلا إذا( xPosition == کتار_SEND_منزل_شا )
    {
        ( فراغ ) حافظه_نقل( ( فراغ * ) px_کتار->pc_Write_منزل, pv_توکی_منزل_کتار, ( size_t ) px_کتار->ux_توکی_اندازه ); /*lint !e961 !e418 !e9087 MISRA exception as the casts are only redundant for some ports, plus previous logic ensures a null pointer can only be passed to memcpy() if the copy size is 0.  Cast to void required by function signature and safe as no alignment requirement and copy length specified in bytes. */
        px_کتار->pc_Write_منزل += px_کتار->ux_توکی_اندازه;                                                       /*lint !e9016 Pointer arithmetic on char types ok, especially in this use case where it is the clearest way of conveying intent. */

        إذا( px_کتار->pc_Write_منزل >= px_کتار->u.x_کتار.pc_لکۍ )                                             /*lint !e946 MISRA exception justified as comparison of pointers is the cleanest solution. */
        {
            px_کتار->pc_Write_منزل = px_کتار->pc_سر;
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    وإلا
    {
        ( فراغ ) حافظه_نقل( ( فراغ * ) px_کتار->u.x_کتار.pcReadFrom, pv_توکی_منزل_کتار, ( size_t ) px_کتار->ux_توکی_اندازه ); /*lint !e961 !e9087 !e418 MISRA exception as the casts are only redundant for some ports.  Cast to void required by function signature and safe as no alignment requirement and copy length specified in bytes.  Assert checks null pointer only used when length is 0. */
        px_کتار->u.x_کتار.pcReadFrom -= px_کتار->ux_توکی_اندازه;

        إذا( px_کتار->u.x_کتار.pcReadFrom < px_کتار->pc_سر ) /*lint !e946 MISRA exception justified as comparison of pointers is the cleanest solution. */
        {
            px_کتار->u.x_کتار.pcReadFrom = ( px_کتار->u.x_کتار.pc_لکۍ - px_کتار->ux_توکی_اندازه );
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }

        إذا( xPosition == کتار_OVERWRITE )
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

    px_کتار->ux_Messages_انتظار = ux_Messages_انتظار + ( UBaseType_t ) 1;

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

ساكن فراغ prv_Copy_معلومات_From_کتار( کتار_t * ثابت px_کتار,
                                  فراغ * ثابت pv_بفر )
{
    إذا( px_کتار->ux_توکی_اندازه != ( UBaseType_t ) 0 )
    {
        px_کتار->u.x_کتار.pcReadFrom += px_کتار->ux_توکی_اندازه;           /*lint !e9016 Pointer arithmetic on char types ok, especially in this use case where it is the clearest way of conveying intent. */

        إذا( px_کتار->u.x_کتار.pcReadFrom >= px_کتار->u.x_کتار.pc_لکۍ ) /*lint !e946 MISRA exception justified as use of the relational operator is the cleanest solutions. */
        {
            px_کتار->u.x_کتار.pcReadFrom = px_کتار->pc_سر;
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }

        ( فراغ ) حافظه_نقل( ( فراغ * ) pv_بفر, ( فراغ * ) px_کتار->u.x_کتار.pcReadFrom, ( size_t ) px_کتار->ux_توکی_اندازه ); /*lint !e961 !e418 !e9087 MISRA exception as the casts are only redundant for some ports.  Also previous logic ensures a null pointer can only be passed to memcpy() when the count is 0.  Cast to void required by function signature and safe as no alignment requirement and copy length specified in bytes. */
    }
}
/*-----------------------------------------------------------*/

#إذا ( کتار_USE_LOCKS == 1 )
    ساكن فراغ prv_Unlock_کتار( کتار_t * ثابت px_کتار )
    {
        /* THIS FUNCTION MUST BE CALLED WITH THE SCHEDULER SUSPENDED. */

        /* The lock counts contains the number of extra data items placed or
         * removed from the queue while the queue was locked.  When a queue is
         * locked items can be added or removed, but the event lists cannot be
         * updated. */
        taskENTER_CRITICAL( &( px_کتار->x_کتار_قلف ) );
        {
            int8_t c_Tx_قلف = px_کتار->c_Tx_قلف;

            /* See if data was added to the queue while it was locked. */
            طالما( c_Tx_قلف > کتار_LOCKED_UNMODIFIED )
            {
                /* Data was posted while the queue was locked.  Are any tasks
                 * blocked waiting for data to become available? */
                #إذا ( config_USE_کتار_SETS == 1 )
                {
                    إذا( px_کتار->px_کتار_سېټ_Container != عدم )
                    {
                        إذا( prv_Notify_کتار_سېټ_Container( px_کتار ) != pdFALSE )
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
                        إذا( list_LIST_IS_تشه( &( px_کتار->x_Tasks_انتظار_منزل_Receive ) ) == pdFALSE )
                        {
                            إذا( xTaskRemoveFromEventList( &( px_کتار->x_Tasks_انتظار_منزل_Receive ) ) != pdFALSE )
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
                    إذا( list_LIST_IS_تشه( &( px_کتار->x_Tasks_انتظار_منزل_Receive ) ) == pdFALSE )
                    {
                        إذا( xTaskRemoveFromEventList( &( px_کتار->x_Tasks_انتظار_منزل_Receive ) ) != pdFALSE )
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
                #د_که_پای /* configUSE_QUEUE_SETS */

                --c_Tx_قلف;
            }

            px_کتار->c_Tx_قلف = کتار_UNLOCKED;
        }
        taskEXIT_CRITICAL( &( px_کتار->x_کتار_قلف ) );

        /* Do the same for the Rx lock. */
        taskENTER_CRITICAL( &( px_کتار->x_کتار_قلف ) );
        {
            int8_t c_Rx_قلف = px_کتار->c_Rx_قلف;

            طالما( c_Rx_قلف > کتار_LOCKED_UNMODIFIED )
            {
                إذا( list_LIST_IS_تشه( &( px_کتار->x_Tasks_انتظار_منزل_Send ) ) == pdFALSE )
                {
                    إذا( xTaskRemoveFromEventList( &( px_کتار->x_Tasks_انتظار_منزل_Send ) ) != pdFALSE )
                    {
                        vTaskMissedYield();
                    }
                    وإلا
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }

                    --c_Rx_قلف;
                }
                وإلا
                {
                    اكسر;
                }
            }

            px_کتار->c_Rx_قلف = کتار_UNLOCKED;
        }
        taskEXIT_CRITICAL( &( px_کتار->x_کتار_قلف ) );
    }
#د_که_پای /* queueUSE_LOCKS == 1 */
/*-----------------------------------------------------------*/

#إذا ( کتار_USE_LOCKS == 1 )
    ساكن بنسټ_Type_t prv_Is_کتار_تشه( ثابت کتار_t * px_کتار )
    {
        بنسټ_Type_t xReturn;

        taskENTER_CRITICAL( &( ( ( کتار_t * ) px_کتار )->x_کتار_قلف ) );
        {
            إذا( px_کتار->ux_Messages_انتظار == ( UBaseType_t ) 0 )
            {
                xReturn = pdTRUE;
            }
            وإلا
            {
                xReturn = pdFALSE;
            }
        }
        taskEXIT_CRITICAL( &( ( ( کتار_t * ) px_کتار )->x_کتار_قلف ) );

        أرجع xReturn;
    }
#د_که_پای /* queueUSE_LOCKS == 1 */
/*-----------------------------------------------------------*/

بنسټ_Type_t x_کتار_Is_کتار_تشه_From_ISR( ثابت کتار_Handle_t x_کتار )
{
    بنسټ_Type_t xReturn;
    کتار_t * ثابت px_کتار = x_کتار;

    configASSERT( px_کتار );

    إذا( px_کتار->ux_Messages_انتظار == ( UBaseType_t ) 0 )
    {
        xReturn = pdTRUE;
    }
    وإلا
    {
        xReturn = pdFALSE;
    }

    أرجع xReturn;
} /*lint !e818 xQueue could not be pointer to const because it is a typedef. */
/*-----------------------------------------------------------*/

#إذا ( کتار_USE_LOCKS == 1 )
    ساكن بنسټ_Type_t prv_Is_کتار_ډک( ثابت کتار_t * px_کتار )
    {
        بنسټ_Type_t xReturn;

        taskENTER_CRITICAL( &( ( ( کتار_t * ) px_کتار )->x_کتار_قلف ) );
        {
            إذا( px_کتار->ux_Messages_انتظار == px_کتار->ux_اوږدتیا )
            {
                xReturn = pdTRUE;
            }
            وإلا
            {
                xReturn = pdFALSE;
            }
        }
        taskEXIT_CRITICAL( &( ( ( کتار_t * ) px_کتار )->x_کتار_قلف ) );

        أرجع xReturn;
    }
#د_که_پای /* queueUSE_LOCKS == 1 */
/*-----------------------------------------------------------*/

بنسټ_Type_t x_کتار_Is_کتار_ډک_From_ISR( ثابت کتار_Handle_t x_کتار )
{
    بنسټ_Type_t xReturn;
    کتار_t * ثابت px_کتار = x_کتار;

    configASSERT( px_کتار );

    إذا( px_کتار->ux_Messages_انتظار == px_کتار->ux_اوږدتیا )
    {
        xReturn = pdTRUE;
    }
    وإلا
    {
        xReturn = pdFALSE;
    }

    أرجع xReturn;
} /*lint !e818 xQueue could not be pointer to const because it is a typedef. */
/*-----------------------------------------------------------*/

#إذا ( configUSE_CO_ROUTINES == 1 )

    بنسټ_Type_t x_کتار_CRSend( کتار_Handle_t x_کتار,
                             ثابت فراغ * pv_توکی_منزل_کتار,
                             TickType_t x_Ticks_منزل_Wait )
    {
        بنسټ_Type_t xReturn;
        کتار_t * ثابت px_کتار = x_کتار;

        /* If the queue is already full we may have to block.  A critical section
         * is required to prevent an interrupt removing something from the queue
         * between the check to see if the queue is full and blocking on the queue. */
        پورټ_DISABLE_INTERRUPTS();
        {
            إذا( prv_Is_کتار_ډک( px_کتار ) != pdFALSE )
            {
                /* The queue is full - do we want to block or just leave without
                 * posting? */
                إذا( x_Ticks_منزل_Wait > ( TickType_t ) 0 )
                {
                    /* As this is called from a coroutine we cannot block directly, but
                     * return indicating that we need to block. */
                    v_Co_Routine_جمع_کړه_منزل_Delayed_List( x_Ticks_منزل_Wait, &( px_کتار->x_Tasks_انتظار_منزل_Send ) );
                    پورټ_ENABLE_INTERRUPTS();
                    أرجع err_کتار_BLOCKED;
                }
                وإلا
                {
                    پورټ_ENABLE_INTERRUPTS();
                    أرجع err_کتار_ډک;
                }
            }
        }
        پورټ_ENABLE_INTERRUPTS();

        پورټ_DISABLE_INTERRUPTS();
        {
            إذا( px_کتار->ux_Messages_انتظار < px_کتار->ux_اوږدتیا )
            {
                /* There is room in the queue, copy the data into the queue. */
                prv_Copy_معلومات_منزل_کتار( px_کتار, pv_توکی_منزل_کتار, کتار_SEND_منزل_شا );
                xReturn = pdPASS;

                /* Were any co-routines waiting for data to become available? */
                إذا( list_LIST_IS_تشه( &( px_کتار->x_Tasks_انتظار_منزل_Receive ) ) == pdFALSE )
                {
                    /* In this instance the co-routine could be placed directly
                     * into the ready list as we are within a critical section.
                     * Instead the same pending ready list mechanism is used as if
                     * the event were caused from within an interrupt. */
                    إذا( xCoRoutineRemoveFromEventList( &( px_کتار->x_Tasks_انتظار_منزل_Receive ) ) != pdFALSE )
                    {
                        /* The co-routine waiting has a higher priority so record
                         * that a yield might be appropriate. */
                        xReturn = err_کتار_YIELD;
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
                xReturn = err_کتار_ډک;
            }
        }
        پورټ_ENABLE_INTERRUPTS();

        أرجع xReturn;
    }

#د_که_پای /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#إذا ( configUSE_CO_ROUTINES == 1 )

    بنسټ_Type_t x_کتار_CRReceive( کتار_Handle_t x_کتار,
                                فراغ * pv_بفر,
                                TickType_t x_Ticks_منزل_Wait )
    {
        بنسټ_Type_t xReturn;
        کتار_t * ثابت px_کتار = x_کتار;

        /* If the queue is already empty we may have to block.  A critical section
         * is required to prevent an interrupt adding something to the queue
         * between the check to see if the queue is empty and blocking on the queue. */
        پورټ_DISABLE_INTERRUPTS();
        {
            إذا( px_کتار->ux_Messages_انتظار == ( UBaseType_t ) 0 )
            {
                /* There are no messages in the queue, do we want to block or just
                 * leave with nothing? */
                إذا( x_Ticks_منزل_Wait > ( TickType_t ) 0 )
                {
                    /* As this is a co-routine we cannot block directly, but return
                     * indicating that we need to block. */
                    v_Co_Routine_جمع_کړه_منزل_Delayed_List( x_Ticks_منزل_Wait, &( px_کتار->x_Tasks_انتظار_منزل_Receive ) );
                    پورټ_ENABLE_INTERRUPTS();
                    أرجع err_کتار_BLOCKED;
                }
                وإلا
                {
                    پورټ_ENABLE_INTERRUPTS();
                    أرجع err_کتار_ډک;
                }
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        پورټ_ENABLE_INTERRUPTS();

        پورټ_DISABLE_INTERRUPTS();
        {
            إذا( px_کتار->ux_Messages_انتظار > ( UBaseType_t ) 0 )
            {
                /* Data is available from the queue. */
                px_کتار->u.x_کتار.pcReadFrom += px_کتار->ux_توکی_اندازه;

                إذا( px_کتار->u.x_کتار.pcReadFrom >= px_کتار->u.x_کتار.pc_لکۍ )
                {
                    px_کتار->u.x_کتار.pcReadFrom = px_کتار->pc_سر;
                }
                وإلا
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                --( px_کتار->ux_Messages_انتظار );
                ( فراغ ) حافظه_نقل( ( فراغ * ) pv_بفر, ( فراغ * ) px_کتار->u.x_کتار.pcReadFrom, ( بینښه ) px_کتار->ux_توکی_اندازه );

                xReturn = pdPASS;

                /* Were any co-routines waiting for space to become available? */
                إذا( list_LIST_IS_تشه( &( px_کتار->x_Tasks_انتظار_منزل_Send ) ) == pdFALSE )
                {
                    /* In this instance the co-routine could be placed directly
                     * into the ready list as we are within a critical section.
                     * Instead the same pending ready list mechanism is used as if
                     * the event were caused from within an interrupt. */
                    إذا( xCoRoutineRemoveFromEventList( &( px_کتار->x_Tasks_انتظار_منزل_Send ) ) != pdFALSE )
                    {
                        xReturn = err_کتار_YIELD;
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
        پورټ_ENABLE_INTERRUPTS();

        أرجع xReturn;
    }

#د_که_پای /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#إذا ( configUSE_CO_ROUTINES == 1 )

    بنسټ_Type_t x_کتار_CRSend_From_ISR( کتار_Handle_t x_کتار,
                                    ثابت فراغ * pv_توکی_منزل_کتار,
                                    بنسټ_Type_t xCoRoutinePreviouslyWoken )
    {
        کتار_t * ثابت px_کتار = x_کتار;

        /* Cannot block within an ISR so if there is no space on the queue then
         * exit without doing anything. */
        إذا( px_کتار->ux_Messages_انتظار < px_کتار->ux_اوږدتیا )
        {
            prv_Copy_معلومات_منزل_کتار( px_کتار, pv_توکی_منزل_کتار, کتار_SEND_منزل_شا );

            /* We only want to wake one co-routine per ISR, so check that a
             * co-routine has not already been woken. */
            إذا( xCoRoutinePreviouslyWoken == pdFALSE )
            {
                إذا( list_LIST_IS_تشه( &( px_کتار->x_Tasks_انتظار_منزل_Receive ) ) == pdFALSE )
                {
                    إذا( xCoRoutineRemoveFromEventList( &( px_کتار->x_Tasks_انتظار_منزل_Receive ) ) != pdFALSE )
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

        أرجع xCoRoutinePreviouslyWoken;
    }

#د_که_پای /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#إذا ( configUSE_CO_ROUTINES == 1 )

    بنسټ_Type_t x_کتار_CRReceive_From_ISR( کتار_Handle_t x_کتار,
                                       فراغ * pv_بفر,
                                       بنسټ_Type_t * pxCoRoutineWoken )
    {
        بنسټ_Type_t xReturn;
        کتار_t * ثابت px_کتار = x_کتار;

        /* We cannot block from an ISR, so check there is data available. If
         * not then just leave without doing anything. */
        إذا( px_کتار->ux_Messages_انتظار > ( UBaseType_t ) 0 )
        {
            /* Copy the data from the queue. */
            px_کتار->u.x_کتار.pcReadFrom += px_کتار->ux_توکی_اندازه;

            إذا( px_کتار->u.x_کتار.pcReadFrom >= px_کتار->u.x_کتار.pc_لکۍ )
            {
                px_کتار->u.x_کتار.pcReadFrom = px_کتار->pc_سر;
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }

            --( px_کتار->ux_Messages_انتظار );
            ( فراغ ) حافظه_نقل( ( فراغ * ) pv_بفر, ( فراغ * ) px_کتار->u.x_کتار.pcReadFrom, ( بینښه ) px_کتار->ux_توکی_اندازه );

            إذا( ( *pxCoRoutineWoken ) == pdFALSE )
            {
                إذا( list_LIST_IS_تشه( &( px_کتار->x_Tasks_انتظار_منزل_Send ) ) == pdFALSE )
                {
                    إذا( xCoRoutineRemoveFromEventList( &( px_کتار->x_Tasks_انتظار_منزل_Send ) ) != pdFALSE )
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

        أرجع xReturn;
    }

#د_که_پای /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#إذا ( config_کتار_REGISTRY_اندازه > 0 )

    فراغ v_کتار_جمع_کړه_منزل_Registry( کتار_Handle_t x_کتار,
                              ثابت حرف * pc_کتار_Name ) /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
    {
        UBaseType_t ux;
        کتار_Registry_توکی_t * px_ثبت_منزل_Write = عدم;

        configASSERT( x_کتار );

        /* For SMP, we need to take the queue registry lock in case another
         * core updates the register simultaneously. */
        prvENTER_CRITICAL_SMP_ONLY( &x_کتار_Registry_قلف );
        {
            إذا( pc_کتار_Name != عدم )
            {
                /* See if there is an empty space in the registry.  A NULL name denotes
                 * a free slot. */
                لكل( ux = ( UBaseType_t ) 0U; ux < ( UBaseType_t ) config_کتار_REGISTRY_اندازه; ux++ )
                {
                    /* Replace an existing entry if the queue is already in the registry. */
                    إذا( x_کتار == x_کتار_Registry[ ux ].xHandle )
                    {
                        px_ثبت_منزل_Write = &( x_کتار_Registry[ ux ] );
                        اكسر;
                    }
                    /* Otherwise, store in the next empty location */
                    وإلا إذا( ( px_ثبت_منزل_Write == عدم ) && ( x_کتار_Registry[ ux ].pc_کتار_Name == عدم ) )
                    {
                        px_ثبت_منزل_Write = &( x_کتار_Registry[ ux ] );
                    }
                    وإلا
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
            }

            إذا( px_ثبت_منزل_Write != عدم )
            {
                /* Store the information on this queue. */
                px_ثبت_منزل_Write->pc_کتار_Name = pc_کتار_Name;
                px_ثبت_منزل_Write->xHandle = x_کتار;

                trace_کتار_REGISTRY_جمع_کړه( x_کتار, pc_کتار_Name );
            }
        }
        /* Release the previously taken queue registry lock. */
        prvEXIT_CRITICAL_SMP_ONLY( &x_کتار_Registry_قلف );
    }

#د_که_پای /* configQUEUE_REGISTRY_SIZE */
/*-----------------------------------------------------------*/

#إذا ( config_کتار_REGISTRY_اندازه > 0 )

    ثابت حرف * pc_کتار_Get_Name( کتار_Handle_t x_کتار ) /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
    {
        UBaseType_t ux;
        ثابت حرف * pcReturn = عدم; /*lint !e971 Unqualified char types are allowed for strings and single characters only. */

        configASSERT( x_کتار );

        /* For SMP, we need to take the queue registry lock in case another
         * core updates the register simultaneously. */
        prvENTER_CRITICAL_SMP_ONLY( &x_کتار_Registry_قلف );
        {
            /* Note there is nothing here to protect against another task adding or
             * removing entries from the registry while it is being searched. */

            لكل( ux = ( UBaseType_t ) 0U; ux < ( UBaseType_t ) config_کتار_REGISTRY_اندازه; ux++ )
            {
                إذا( x_کتار_Registry[ ux ].xHandle == x_کتار )
                {
                    pcReturn = x_کتار_Registry[ ux ].pc_کتار_Name;
                    اكسر;
                }
                وإلا
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
        }
        /* Release the previously taken queue registry lock. */
        prvEXIT_CRITICAL_SMP_ONLY( &x_کتار_Registry_قلف );

        أرجع pcReturn;
    } /*lint !e818 xQueue cannot be a pointer to const because it is a typedef. */

#د_که_پای /* configQUEUE_REGISTRY_SIZE */
/*-----------------------------------------------------------*/

#إذا ( config_کتار_REGISTRY_اندازه > 0 )

    فراغ v_کتار_Unregister_کتار( کتار_Handle_t x_کتار )
    {
        UBaseType_t ux;

        configASSERT( x_کتار );

        /* For SMP, we need to take the queue registry lock in case another
         * core updates the register simultaneously. */
        prvENTER_CRITICAL_SMP_ONLY( &x_کتار_Registry_قلف );
        {
            /* See if the handle of the queue being unregistered in actually in the
             * registry. */
            لكل( ux = ( UBaseType_t ) 0U; ux < ( UBaseType_t ) config_کتار_REGISTRY_اندازه; ux++ )
            {
                إذا( x_کتار_Registry[ ux ].xHandle == x_کتار )
                {
                    /* Set the name to NULL to show that this slot if free again. */
                    x_کتار_Registry[ ux ].pc_کتار_Name = عدم;

                    /* Set the handle to NULL to ensure the same queue handle cannot
                     * appear in the registry twice if it is added, removed, then
                     * added again. */
                    x_کتار_Registry[ ux ].xHandle = ( کتار_Handle_t ) 0;
                    اكسر;
                }
                وإلا
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
        }
        /* Release the previously taken queue registry lock. */
        prvEXIT_CRITICAL_SMP_ONLY( &x_کتار_Registry_قلف );
    } /*lint !e818 xQueue could not be pointer to const because it is a typedef. */

#د_که_پای /* configQUEUE_REGISTRY_SIZE */
/*-----------------------------------------------------------*/

#إذا ( configUSE_TIMERS == 1 )

    فراغ v_کتار_Wait_For_پیغام_Restricted( کتار_Handle_t x_کتار,
                                         TickType_t x_Ticks_منزل_Wait,
                                         ثابت بنسټ_Type_t xWaitIndefinitely )
    {
        کتار_t * ثابت px_کتار = x_کتار;

        /* This function should not be called by application code hence the
         * 'Restricted' in its name.  It is not part of the public API.  It is
         * designed for use by kernel code, and has special calling requirements.
         * It can result in vListInsert() being called on a list that can only
         * possibly ever have one item in it, so the list will be fast, but even
         * so it should be called with the scheduler locked and not from a critical
         * section. */

        /* For SMP, we need to take the queue's xQueueLock as we are about to
         * access the queue. */
        prvENTER_CRITICAL_SMP_ONLY( &( px_کتار->x_کتار_قلف ) );
        {
            #إذا ( کتار_USE_LOCKS == 1 )
            {
                /* Only do anything if there are no messages in the queue.  This function
                 *  will not actually cause the task to block, just place it on a blocked
                 *  list.  It will not block until the scheduler is unlocked - at which
                 *  time a yield will be performed.  If an item is added to the queue while
                 *  the queue is locked, and the calling task blocks on the queue, then the
                 *  calling task will be immediately unblocked when the queue is unlocked. */
                prv_قلف_کتار( px_کتار );
            }
            #د_که_پای /* queueUSE_LOCKS == 1 */

            إذا( px_کتار->ux_Messages_انتظار == ( UBaseType_t ) 0U )
            {
                /* There is nothing in the queue, block for the specified period. */
                v_Task_ځای_پرځای_کړه_On_Event_List_Restricted( &( px_کتار->x_Tasks_انتظار_منزل_Receive ), x_Ticks_منزل_Wait, xWaitIndefinitely );
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }

            #إذا ( کتار_USE_LOCKS == 1 )
            {
                prv_Unlock_کتار( px_کتار );
            }
            #د_که_پای /* queueUSE_LOCKS == 1 */
        }
        /* Release the previously taken xQueueLock. */
        prvEXIT_CRITICAL_SMP_ONLY( &( px_کتار->x_کتار_قلف ) );
    }

#د_که_پای /* configUSE_TIMERS */
/*-----------------------------------------------------------*/

#إذا ( ( config_USE_کتار_SETS == 1 ) && ( config_SUPPORT_DYNAMIC_ویش == 1 ) )

    کتار_سېټ_Handle_t x_کتار_Create_سېټ( ثابت UBaseType_t ux_Event_کتار_اوږدتیا )
    {
        کتار_سېټ_Handle_t px_کتار;

        px_کتار = x_کتار_Generic_Create( ux_Event_کتار_اوږدتیا, ( UBaseType_t ) حجم( کتار_t * ), کتار_کتار_TYPE_سېټ );

        أرجع px_کتار;
    }

#د_که_پای /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#إذا ( config_USE_کتار_SETS == 1 )

    بنسټ_Type_t x_کتار_جمع_کړه_منزل_سېټ( کتار_سېټ_Member_Handle_t x_کتار_Or_Semaphore,
                               کتار_سېټ_Handle_t x_کتار_سېټ )
    {
        بنسټ_Type_t xReturn;

        taskENTER_CRITICAL( &( ( ( کتار_t * ) x_کتار_Or_Semaphore )->x_کتار_قلف ) );
        {
            إذا( ( ( کتار_t * ) x_کتار_Or_Semaphore )->px_کتار_سېټ_Container != عدم )
            {
                /* Cannot add a queue/semaphore to more than one queue set. */
                xReturn = pdFAIL;
            }
            وإلا إذا( ( ( کتار_t * ) x_کتار_Or_Semaphore )->ux_Messages_انتظار != ( UBaseType_t ) 0 )
            {
                /* Cannot add a queue/semaphore to a queue set if there are already
                 * items in the queue/semaphore. */
                xReturn = pdFAIL;
            }
            وإلا
            {
                ( ( کتار_t * ) x_کتار_Or_Semaphore )->px_کتار_سېټ_Container = x_کتار_سېټ;
                xReturn = pdPASS;
            }
        }
        taskEXIT_CRITICAL( &( ( ( کتار_t * ) x_کتار_Or_Semaphore )->x_کتار_قلف ) );

        أرجع xReturn;
    }

#د_که_پای /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#إذا ( config_USE_کتار_SETS == 1 )

    بنسټ_Type_t x_کتار_Remove_From_سېټ( کتار_سېټ_Member_Handle_t x_کتار_Or_Semaphore,
                                    کتار_سېټ_Handle_t x_کتار_سېټ )
    {
        بنسټ_Type_t xReturn;
        کتار_t * ثابت px_کتار_Or_Semaphore = ( کتار_t * ) x_کتار_Or_Semaphore;

        إذا( px_کتار_Or_Semaphore->px_کتار_سېټ_Container != x_کتار_سېټ )
        {
            /* The queue was not a member of the set. */
            xReturn = pdFAIL;
        }
        وإلا إذا( px_کتار_Or_Semaphore->ux_Messages_انتظار != ( UBaseType_t ) 0 )
        {
            /* It is dangerous to remove a queue from a set when the queue is
             * not empty because the queue set will still hold pending events for
             * the queue. */
            xReturn = pdFAIL;
        }
        وإلا
        {
            taskENTER_CRITICAL( &( ( ( کتار_t * ) px_کتار_Or_Semaphore )->x_کتار_قلف ) );
            {
                /* The queue is no longer contained in the set. */
                px_کتار_Or_Semaphore->px_کتار_سېټ_Container = عدم;
            }
            taskEXIT_CRITICAL( &( ( ( کتار_t * ) px_کتار_Or_Semaphore )->x_کتار_قلف ) );
            xReturn = pdPASS;
        }

        أرجع xReturn;
    } /*lint !e818 xQueueSet could not be declared as pointing to const as it is a typedef. */

#د_که_پای /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#إذا ( config_USE_کتار_SETS == 1 )

    کتار_سېټ_Member_Handle_t x_کتار_Select_From_سېټ( کتار_سېټ_Handle_t x_کتار_سېټ,
                                                TickType_t ثابت x_Ticks_منزل_Wait )
    {
        کتار_سېټ_Member_Handle_t xReturn = عدم;

        ( فراغ ) x_کتار_Receive( ( کتار_Handle_t ) x_کتار_سېټ, &xReturn, x_Ticks_منزل_Wait ); /*lint !e961 Casting from one typedef to another is not redundant. */
        أرجع xReturn;
    }

#د_که_پای /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#إذا ( config_USE_کتار_SETS == 1 )

    کتار_سېټ_Member_Handle_t x_کتار_Select_From_سېټ_From_ISR( کتار_سېټ_Handle_t x_کتار_سېټ )
    {
        کتار_سېټ_Member_Handle_t xReturn = عدم;

        ( فراغ ) x_کتار_Receive_From_ISR( ( کتار_Handle_t ) x_کتار_سېټ, &xReturn, عدم ); /*lint !e961 Casting from one typedef to another is not redundant. */
        أرجع xReturn;
    }

#د_که_پای /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#إذا ( config_USE_کتار_SETS == 1 )

    ساكن بنسټ_Type_t prv_Notify_کتار_سېټ_Container( ثابت کتار_t * ثابت px_کتار )
    {
        کتار_t * px_کتار_سېټ_Container = px_کتار->px_کتار_سېټ_Container;
        بنسټ_Type_t xReturn = pdFALSE;

        /* This function must be called form a critical section. */

        /* The following line is not reachable in unit tests because every call
         * to prvNotifyQueueSetContainer is preceded by a check that
         * pxQueueSetContainer != NULL */
        configASSERT( px_کتار_سېټ_Container ); /* LCOV_EXCL_BR_LINE */
        configASSERT( px_کتار_سېټ_Container->ux_Messages_انتظار < px_کتار_سېټ_Container->ux_اوږدتیا );

        /* In SMP, queue sets have their own xQueueLock. Thus we need to also
         * acquire the queue set's xQueueLock before accessing it. */
        prv_ENTER_CRITICAL_خوندي_SMP_ONLY( &( px_کتار_سېټ_Container->x_کتار_قلف ) );
        {
            إذا( px_کتار_سېټ_Container->ux_Messages_انتظار < px_کتار_سېټ_Container->ux_اوږدتیا )
            {
                #إذا ( کتار_USE_LOCKS == 1 )
                    ثابت int8_t c_Tx_قلف = px_کتار_سېټ_Container->c_Tx_قلف;
                #وإلا
                    /* Queue locks not used, so we treat it as unlocked. */
                    ثابت int8_t c_Tx_قلف = کتار_UNLOCKED;
                #د_که_پای /* queueUSE_LOCKS == 1 */

                trace_کتار_سېټ_SEND( px_کتار_سېټ_Container );

                /* The data copied is the handle of the queue that contains data. */
                xReturn = prv_Copy_معلومات_منزل_کتار( px_کتار_سېټ_Container, &px_کتار, کتار_SEND_منزل_شا );

                إذا( c_Tx_قلف == کتار_UNLOCKED )
                {
                    إذا( list_LIST_IS_تشه( &( px_کتار_سېټ_Container->x_Tasks_انتظار_منزل_Receive ) ) == pdFALSE )
                    {
                        إذا( xTaskRemoveFromEventList( &( px_کتار_سېټ_Container->x_Tasks_انتظار_منزل_Receive ) ) != pdFALSE )
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
                    #إذا ( کتار_USE_LOCKS == 1 )
                    {
                        prv_Increment_کتار_Tx_قلف( px_کتار_سېټ_Container, c_Tx_قلف );
                    }
                    #د_که_پای /* queueUSE_LOCKS == 1 */
                }
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        /* Release the previously acquired queue set's xQueueLock. */
        prv_EXIT_CRITICAL_خوندي_SMP_ONLY( &( px_کتار_سېټ_Container->x_کتار_قلف ) );

        أرجع xReturn;
    }

#د_که_پای /* configUSE_QUEUE_SETS */
