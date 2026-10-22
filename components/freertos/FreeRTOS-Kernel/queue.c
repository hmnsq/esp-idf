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

#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <string.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#నిర్వచించు MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#అన్తర్భావ "FreeRTOS.h"
#అన్తర్భావ "task.h"
#అన్తర్భావ "queue.h"
/* Include private IDF API additions for critical thread safety macros */
#అన్తర్భావ "esp_private/freertos_idf_additions_priv.h"

#ఒకవేళ ( configUSE_CO_ROUTINES == 1 )
    #అన్తర్భావ "croutine.h"
#అయితే_ముగింపు

/* Lint e9021, e961 and e750 are suppressed as a MISRA exception justified
 * because the MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#నిర్వచనం_రద్దు MPU_WRAPPERS_INCLUDED_FROM_API_FILE /*lint !e961 !e750 !e9021. */

/* Single core FreeRTOS uses queue locks to ensure that vTaskPlaceOnEventList()
 * calls are deterministic (as queue locks use scheduler suspension instead of
 * critical sections). However, the SMP implementation is non-deterministic
 * anyways, thus SMP can forego the use of queue locks (replaced with a critical
 * sections) in exchange for better queue performance. */
#ఒకవేళ ( config_సంఖ్య_యొక్క_CORES > 1 )
    #నిర్వచించు వరుస_USE_LOCKS            0
    #నిర్వచించు వరుస_UNLOCKED             ( ( int8_t ) 0 )
#లేకపోతే /* configNUMBER_OF_CORES > 1 */
    #నిర్వచించు వరుస_USE_LOCKS            1
    /* Constants used with the cRxLock and cTxLock structure members. */
    #నిర్వచించు వరుస_UNLOCKED             ( ( int8_t ) -1 )
    #నిర్వచించు వరుస_LOCKED_UNMODIFIED    ( ( int8_t ) 0 )
    #నిర్వచించు వరుస_INT8_MAX             ( ( int8_t ) 127 )
#అయితే_ముగింపు /* configNUMBER_OF_CORES > 1 */

/* When the Queue_t structure is used to represent a base queue its pcHead and
 * pcTail members are used as pointers into the queue storage area.  When the
 * Queue_t structure is used to represent a mutex pcHead and pcTail pointers are
 * not necessary, and the pcHead pointer is set to NULL to indicate that the
 * structure instead holds a pointer to the mutex holder (if any).  Map alternative
 * names to the pcHead and structure member to ensure the readability of the code
 * is maintained.  The QueuePointers_t and SemaphoreData_t types are used to form
 * a union as their usage is mutually exclusive dependent on what the queue is
 * being used for. */
#నిర్వచించు ux_వరుస_Type            pc_తల
#నిర్వచించు వరుస_వరుస_IS_MUTEX    ఖాళీ

రకనిర్వచనం నిర్మాణం వరుస_సూచికలు
{
    int8_t * pc_తోక;     /*< Points to the byte at the end of the queue storage area.  Once more byte is allocated than necessary to store the queue items, this is used as a marker. */
    int8_t * pcReadFrom; /*< Points to the last place that a queued item was read from when the structure is used as a queue. */
} వరుస_సూచికలు_t;

రకనిర్వచనం నిర్మాణం Semaphore_దత్తాంశం
{
    TaskHandle_t xMutexHolder;        /*< The handle of the task that holds the mutex. */
    UBaseType_t ux_Recursive_Call_లెక్క; /*< Maintains a count of the number of times a recursive mutex has been recursively 'taken' when the structure is used as a mutex. */
} Semaphore_దత్తాంశం_t;

/* Semaphores do not actually store or copy data, so have an item size of
 * zero. */
#నిర్వచించు వరుస_SEMAPHORE_వరుస_అంశం_నిడివి    ( ( UBaseType_t ) 0 )
#నిర్వచించు వరుస_MUTEX_GIVE_ఖండం_సమయం          ( ( TickType_t ) 0U )

#ఒకవేళ ( configUSE_PREEMPTION == 0 )

/* If the cooperative scheduler is being used then a yield should not be
 * performed just because a higher priority task has been woken. */
    #నిర్వచించు వరుస_YIELD_IF_USING_PREEMPTION()
#లేకపోతే
    #నిర్వచించు వరుస_YIELD_IF_USING_PREEMPTION()    పోర్టు_YIELD_WITHIN_API()
#అయితే_ముగింపు

/*
 * Definition of the queue used by the scheduler.
 * Items are queued by copy, not reference.  See the following link for the
 * rationale: https://www.FreeRTOS.org/Embedded-RTOS-Queues.html
 */
రకనిర్వచనం నిర్మాణం వరుస_Definition /* The old naming convention is used to prevent breaking kernel aware debuggers. */
{
    int8_t * pc_తల;           /*< Points to the beginning of the queue storage area. */
    int8_t * pc_Write_గమ్యం;        /*< Points to the free next place in the storage area. */

    సంఘం
    {
        వరుస_సూచికలు_t x_వరుస;     /*< Data required exclusively when this structure is used as a queue. */
        Semaphore_దత్తాంశం_t xSemaphore; /*< Data required exclusively when this structure is used as a semaphore. */
    } u;

    List_t x_Tasks_నిరీక్షణ_గమ్యం_Send;             /*< List of tasks that are blocked waiting to post onto this queue.  Stored in priority order. */
    List_t x_Tasks_నిరీక్షణ_గమ్యం_Receive;          /*< List of tasks that are blocked waiting to read from this queue.  Stored in priority order. */

    అస్థిర UBaseType_t ux_Messages_నిరీక్షణ; /*< The number of items currently in the queue. */
    UBaseType_t ux_నిడివి;                   /*< The length of the queue defined as the number of items it will hold, not the number of bytes. */
    UBaseType_t ux_అంశం_కొలత;                 /*< The size of each items that the queue will hold. */

    #ఒకవేళ ( వరుస_USE_LOCKS == 1 )
        అస్థిర int8_t c_Rx_తాళం; /*< Stores the number of items received from the queue (removed from the queue) while the queue was locked.  Set to queueUNLOCKED when the queue is not locked. */
        అస్థిర int8_t c_Tx_తాళం; /*< Stores the number of items transmitted to the queue (added to the queue) while the queue was locked.  Set to queueUNLOCKED when the queue is not locked. */
    #అయితే_ముగింపు /* queueUSE_LOCKS == 1 */

    #ఒకవేళ ( ( config_SUPPORT_STATIC_కేటాయింపు == 1 ) && ( config_SUPPORT_DYNAMIC_కేటాయింపు == 1 ) )
        uint8_t ucStaticallyAllocated; /*< Set to pdTRUE if the memory used by the queue was statically allocated to ensure no attempt is made to free the memory. */
    #అయితే_ముగింపు

    #ఒకవేళ ( config_USE_వరుస_SETS == 1 )
        నిర్మాణం వరుస_Definition * px_వరుస_సమితి_Container;
    #అయితే_ముగింపు

    #ఒకవేళ ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_వరుస_సంఖ్య;
        uint8_t uc_వరుస_Type;
    #అయితే_ముగింపు

    పోర్టు_MUX_TYPE x_వరుస_తాళం; /* Spinlock required for SMP critical sections */
} x_వరుస_2;

/* The old xQUEUE name is maintained above then typedefed to the new Queue_t
 * name below to enable the use of older kernel aware debuggers. */
రకనిర్వచనం x_వరుస_2 వరుస_t;

/*-----------------------------------------------------------*/

/*
 * The queue registry is just a means for kernel aware debuggers to locate
 * queue structures.  It has no other purpose so is an optional component.
 */
#ఒకవేళ ( config_వరుస_REGISTRY_కొలత > 0 )

/* The type stored within the queue registry array.  This allows a name
 * to be assigned to each queue making kernel aware debugging a little
 * more user friendly. */
    రకనిర్వచనం నిర్మాణం వరుస_REGISTRY_అంశం
    {
        స్థిరరాశి అక్షరం * pc_వరుస_Name; /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
        వరుస_Handle_t xHandle;
    } x_వరుస_Registry_అంశం;

/* The old xQueueRegistryItem name is maintained above then typedefed to the
 * new xQueueRegistryItem name below to enable the use of older kernel aware
 * debuggers. */
    రకనిర్వచనం x_వరుస_Registry_అంశం వరుస_Registry_అంశం_t;

/* The queue registry is simply an array of QueueRegistryItem_t structures.
 * The pcQueueName member of a structure being NULL is indicative of the
 * array position being vacant. */
    PRIVILEGED_దత్తాంశం వరుస_Registry_అంశం_t x_వరుస_Registry[ config_వరుస_REGISTRY_కొలత ];

    #ఒకవేళ ( config_సంఖ్య_యొక్క_CORES > 1 )
/* Spinlock required in SMP when accessing the queue registry */
        స్థిర పోర్టు_MUX_TYPE x_వరుస_Registry_తాళం = పోర్టు_MUX_INITIALIZER_UNLOCKED;
    #అయితే_ముగింపు /* configNUMBER_OF_CORES > 1 */

#అయితే_ముగింపు /* configQUEUE_REGISTRY_SIZE */

#ఒకవేళ ( వరుస_USE_LOCKS == 1 )

/*
 * Unlocks a queue locked by a call to prvLockQueue.  Locking a queue does not
 * prevent an ISR from adding or removing items to the queue, but does prevent
 * an ISR from removing tasks from the queue event lists.  If an ISR finds a
 * queue is locked it will instead increment the appropriate queue lock count
 * to indicate that a task may require unblocking.  When the queue in unlocked
 * these lock counts are inspected, and the appropriate action taken.
 */
    స్థిర శూన్య prv_Unlock_వరుస( వరుస_t * స్థిరరాశి px_వరుస ) PRIVILEGED_FUNCTION;

/*
 * Uses a critical section to determine if there is any data in a queue.
 *
 * @return pdTRUE if the queue contains no items, otherwise pdFALSE.
 */
    స్థిర పునాది_Type_t prv_Is_వరుస_ఖాళీలు( స్థిరరాశి వరుస_t * px_వరుస ) PRIVILEGED_FUNCTION;

/*
 * Uses a critical section to determine if there is any space in a queue.
 *
 * @return pdTRUE if there is no space, otherwise pdFALSE;
 */
    స్థిర పునాది_Type_t prv_Is_వరుస_నిండిన( స్థిరరాశి వరుస_t * px_వరుస ) PRIVILEGED_FUNCTION;
#అయితే_ముగింపు /* queueUSE_LOCKS == 1 */

/*
 * Copies an item into the queue, either at the front of the queue or the
 * back of the queue.
 */
స్థిర పునాది_Type_t prv_Copy_దత్తాంశం_గమ్యం_వరుస( వరుస_t * స్థిరరాశి px_వరుస,
                                      స్థిరరాశి శూన్య * pv_అంశం_గమ్యం_వరుస,
                                      స్థిరరాశి పునాది_Type_t xPosition ) PRIVILEGED_FUNCTION;

/*
 * Copies an item out of a queue.
 */
స్థిర శూన్య prv_Copy_దత్తాంశం_From_వరుస( వరుస_t * స్థిరరాశి px_వరుస,
                                  శూన్య * స్థిరరాశి pv_బఫరు ) PRIVILEGED_FUNCTION;

#ఒకవేళ ( config_USE_వరుస_SETS == 1 )

/*
 * Checks to see if a queue is a member of a queue set, and if so, notifies
 * the queue set that the queue contains data.
 */
    స్థిర పునాది_Type_t prv_Notify_వరుస_సమితి_Container( స్థిరరాశి వరుస_t * స్థిరరాశి px_వరుస ) PRIVILEGED_FUNCTION;
#అయితే_ముగింపు

/*
 * Called after a Queue_t structure has been allocated either statically or
 * dynamically to fill in the structure's members.
 */
స్థిర శూన్య prv_Initialise_New_వరుస( స్థిరరాశి UBaseType_t ux_వరుస_నిడివి,
                                   స్థిరరాశి UBaseType_t ux_అంశం_కొలత,
                                   uint8_t * puc_వరుస_Storage,
                                   స్థిరరాశి uint8_t uc_వరుస_Type,
                                   వరుస_t * px_New_వరుస ) PRIVILEGED_FUNCTION;

/*
 * Mutexes are a special type of queue.  When a mutex is created, first the
 * queue is created, then prvInitialiseMutex() is called to configure the queue
 * as a mutex.
 */
#ఒకవేళ ( configUSE_MUTEXES == 1 )
    స్థిర శూన్య prvInitialiseMutex( వరుస_t * px_New_వరుస ) PRIVILEGED_FUNCTION;
#అయితే_ముగింపు

#ఒకవేళ ( configUSE_MUTEXES == 1 )

/*
 * If a task waiting for a mutex causes the mutex holder to inherit a
 * priority, but the waiting task times out, then the holder should
 * disinherit the priority - but only down to the highest priority of any
 * other tasks that are waiting for the same mutex.  This function returns
 * that priority.
 */
    స్థిర UBaseType_t prv_Get_Disinherit_ప్రాధాన్యత_After_Timeout( స్థిరరాశి వరుస_t * స్థిరరాశి px_వరుస ) PRIVILEGED_FUNCTION;
#అయితే_ముగింపు
/*-----------------------------------------------------------*/

#ఒకవేళ ( వరుస_USE_LOCKS == 1 )

/*
 * Macro to mark a queue as locked.  Locking a queue prevents an ISR from
 * accessing the queue event lists.
 */
    #నిర్వచించు prv_తాళం_వరుస( px_వరుస )                        \
    taskENTER_CRITICAL( &( px_వరుస->x_వరుస_తాళం ) );        \
    {                                                      \
        ఒకవేళ( ( px_వరుస )->c_Rx_తాళం == వరుస_UNLOCKED )        \
        {                                                  \
            ( px_వరుస )->c_Rx_తాళం = వరుస_LOCKED_UNMODIFIED; \
        }                                                  \
        ఒకవేళ( ( px_వరుస )->c_Tx_తాళం == వరుస_UNLOCKED )        \
        {                                                  \
            ( px_వరుస )->c_Tx_తాళం = వరుస_LOCKED_UNMODIFIED; \
        }                                                  \
    }                                                      \
    taskEXIT_CRITICAL( &( px_వరుస->x_వరుస_తాళం ) )

/*
 * Macro to increment cTxLock member of the queue data structure. It is
 * capped at the number of tasks in the system as we cannot unblock more
 * tasks than the number of tasks in the system.
 */
    #నిర్వచించు prv_Increment_వరుస_Tx_తాళం( px_వరుస, c_Tx_తాళం )                       \
    {                                                                         \
        స్థిరరాశి UBaseType_t ux_సంఖ్య_యొక్క_Tasks = ux_Task_Get_సంఖ్య_యొక్క_Tasks();         \
        ఒకవేళ( ( UBaseType_t ) ( c_Tx_తాళం ) < ux_సంఖ్య_యొక్క_Tasks )                   \
        {                                                                     \
            configASSERT( ( c_Tx_తాళం ) != వరుస_INT8_MAX );                     \
            ( px_వరుస )->c_Tx_తాళం = ( int8_t ) ( ( c_Tx_తాళం ) + ( int8_t ) 1 ); \
        }                                                                     \
    }

/*
 * Macro to increment cRxLock member of the queue data structure. It is
 * capped at the number of tasks in the system as we cannot unblock more
 * tasks than the number of tasks in the system.
 */
    #నిర్వచించు prv_Increment_వరుస_Rx_తాళం( px_వరుస, c_Rx_తాళం )                       \
    {                                                                         \
        స్థిరరాశి UBaseType_t ux_సంఖ్య_యొక్క_Tasks = ux_Task_Get_సంఖ్య_యొక్క_Tasks();         \
        ఒకవేళ( ( UBaseType_t ) ( c_Rx_తాళం ) < ux_సంఖ్య_యొక్క_Tasks )                   \
        {                                                                     \
            configASSERT( ( c_Rx_తాళం ) != వరుస_INT8_MAX );                     \
            ( px_వరుస )->c_Rx_తాళం = ( int8_t ) ( ( c_Rx_తాళం ) + ( int8_t ) 1 ); \
        }                                                                     \
    }
#అయితే_ముగింపు /* queueUSE_LOCKS == 1 */
/*-----------------------------------------------------------*/

పునాది_Type_t x_వరుస_Generic_Reset( వరుస_Handle_t x_వరుస,
                               పునాది_Type_t x_New_వరుస )
{
    పునాది_Type_t xReturn = pdPASS;
    వరుస_t * స్థిరరాశి px_వరుస = x_వరుస;

    configASSERT( px_వరుస );

    ఒకవేళ( x_New_వరుస == pdTRUE )
    {
        పోర్టు_MUX_INITIALIZE( &( px_వరుస->x_వరుస_తాళం ) );
    }

    ఒకవేళ( ( px_వరుస != ఖాళీ ) &&
        ( px_వరుస->ux_నిడివి >= 1U ) &&
        /* Check for multiplication overflow. */
        ( ( SIZE_MAX / px_వరుస->ux_నిడివి ) >= px_వరుస->ux_అంశం_కొలత ) )
    {
        taskENTER_CRITICAL( &( px_వరుస->x_వరుస_తాళం ) );
        {
            px_వరుస->u.x_వరుస.pc_తోక = px_వరుస->pc_తల + ( px_వరుస->ux_నిడివి * px_వరుస->ux_అంశం_కొలత ); /*lint !e9016 Pointer arithmetic allowed on char types, especially when it assists conveying intent. */
            px_వరుస->ux_Messages_నిరీక్షణ = ( UBaseType_t ) 0U;
            px_వరుస->pc_Write_గమ్యం = px_వరుస->pc_తల;
            px_వరుస->u.x_వరుస.pcReadFrom = px_వరుస->pc_తల + ( ( px_వరుస->ux_నిడివి - 1U ) * px_వరుస->ux_అంశం_కొలత ); /*lint !e9016 Pointer arithmetic allowed on char types, especially when it assists conveying intent. */
            #ఒకవేళ ( వరుస_USE_LOCKS == 1 )
            {
                px_వరుస->c_Rx_తాళం = వరుస_UNLOCKED;
                px_వరుస->c_Tx_తాళం = వరుస_UNLOCKED;
            }
            #అయితే_ముగింపు /* queueUSE_LOCKS == 1 */

            ఒకవేళ( x_New_వరుస == pdFALSE )
            {
                /* If there are tasks blocked waiting to read from the queue, then
                 * the tasks will remain blocked as after this function exits the queue
                 * will still be empty.  If there are tasks blocked waiting to write to
                 * the queue, then one should be unblocked as after this function exits
                 * it will be possible to write to it. */
                ఒకవేళ( list_LIST_IS_ఖాళీలు( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Send ) ) == pdFALSE )
                {
                    ఒకవేళ( xTaskRemoveFromEventList( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Send ) ) != pdFALSE )
                    {
                        వరుస_YIELD_IF_USING_PREEMPTION();
                    }
                    లేకపోతే
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                లేకపోతే
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            లేకపోతే
            {
                /* Ensure the event queues start in the correct state. */
                vListInitialise( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Send ) );
                vListInitialise( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Receive ) );
            }
        }
        taskEXIT_CRITICAL( &( px_వరుస->x_వరుస_తాళం ) );
    }
    లేకపోతే
    {
        xReturn = pdFAIL;
    }

    configASSERT( xReturn != pdFAIL );

    /* A value is returned for calling semantic consistency with previous
     * versions. */
    తిరిగి xReturn;
}
/*-----------------------------------------------------------*/

#ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )

    వరుస_Handle_t x_వరుస_Generic_Create_Static( స్థిరరాశి UBaseType_t ux_వరుస_నిడివి,
                                             స్థిరరాశి UBaseType_t ux_అంశం_కొలత,
                                             uint8_t * puc_వరుస_Storage,
                                             Static_వరుస_t * px_Static_వరుస,
                                             స్థిరరాశి uint8_t uc_వరుస_Type )
    {
        వరుస_t * px_New_వరుస = ఖాళీ;

        /* The StaticQueue_t structure and the queue storage area must be
         * supplied. */
        configASSERT( px_Static_వరుస );

        ఒకవేళ( ( ux_వరుస_నిడివి > ( UBaseType_t ) 0 ) &&
            ( px_Static_వరుస != ఖాళీ ) &&

            /* A queue storage area should be provided if the item size is not 0, and
             * should not be provided if the item size is 0. */
            ( !( ( puc_వరుస_Storage != ఖాళీ ) && ( ux_అంశం_కొలత == 0 ) ) ) &&
            ( !( ( puc_వరుస_Storage == ఖాళీ ) && ( ux_అంశం_కొలత != 0 ) ) ) )
        {
            #ఒకవేళ ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticQueue_t or StaticSemaphore_t equals the size of
                 * the real queue and semaphore structures. */
                అస్థిర size_t x_కొలత = పరిమాణం( Static_వరుస_t );

                /* This assertion cannot be branch covered in unit tests */
                configASSERT( x_కొలత == పరిమాణం( వరుస_t ) ); /* LCOV_EXCL_BR_LINE */
                ( శూన్య ) x_కొలత;                             /* Keeps lint quiet when configASSERT() is not defined. */
            }
            #అయితే_ముగింపు /* configASSERT_DEFINED */

            /* The address of a statically allocated queue was passed in, use it.
             * The address of a statically allocated storage area was also passed in
             * but is already set. */
            px_New_వరుస = ( వరుస_t * ) px_Static_వరుస; /*lint !e740 !e9087 Unusual cast is ok as the structures are designed to have the same alignment, and the size is checked by an assert. */

            #ఒకవేళ ( config_SUPPORT_DYNAMIC_కేటాయింపు == 1 )
            {
                /* Queues can be allocated wither statically or dynamically, so
                 * note this queue was allocated statically in case the queue is
                 * later deleted. */
                px_New_వరుస->ucStaticallyAllocated = pdTRUE;
            }
            #అయితే_ముగింపు /* configSUPPORT_DYNAMIC_ALLOCATION */

            prv_Initialise_New_వరుస( ux_వరుస_నిడివి, ux_అంశం_కొలత, puc_వరుస_Storage, uc_వరుస_Type, px_New_వరుస );
        }
        లేకపోతే
        {
            configASSERT( px_New_వరుస );
            mtCOVERAGE_TEST_MARKER();
        }

        తిరిగి px_New_వరుస;
    }

#అయితే_ముగింపు /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

#ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )

    పునాది_Type_t x_వరుస_Generic_Get_Static_Buffers( వరుస_Handle_t x_వరుస,
                                              uint8_t ** ppuc_వరుస_Storage,
                                              Static_వరుస_t ** ppx_Static_వరుస )
    {
        పునాది_Type_t xReturn;
        వరుస_t * స్థిరరాశి px_వరుస = x_వరుస;

        configASSERT( px_వరుస );
        configASSERT( ppx_Static_వరుస );

        #ఒకవేళ ( config_SUPPORT_DYNAMIC_కేటాయింపు == 1 )
        {
            /* Check if the queue was statically allocated. */
            ఒకవేళ( px_వరుస->ucStaticallyAllocated == ( uint8_t ) pdTRUE )
            {
                ఒకవేళ( ppuc_వరుస_Storage != ఖాళీ )
                {
                    *ppuc_వరుస_Storage = ( uint8_t * ) px_వరుస->pc_తల;
                }

                *ppx_Static_వరుస = ( Static_వరుస_t * ) px_వరుస;
                xReturn = pdTRUE;
            }
            లేకపోతే
            {
                xReturn = pdFALSE;
            }
        }
        #లేకపోతే /* configSUPPORT_DYNAMIC_ALLOCATION */
        {
            /* Queue must have been statically allocated. */
            ఒకవేళ( ppuc_వరుస_Storage != ఖాళీ )
            {
                *ppuc_వరుస_Storage = ( uint8_t * ) px_వరుస->pc_తల;
            }

            *ppx_Static_వరుస = ( Static_వరుస_t * ) px_వరుస;
            xReturn = pdTRUE;
        }
        #అయితే_ముగింపు /* configSUPPORT_DYNAMIC_ALLOCATION */

        తిరిగి xReturn;
    }

#అయితే_ముగింపు /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

#ఒకవేళ ( config_SUPPORT_DYNAMIC_కేటాయింపు == 1 )

    వరుస_Handle_t x_వరుస_Generic_Create( స్థిరరాశి UBaseType_t ux_వరుస_నిడివి,
                                       స్థిరరాశి UBaseType_t ux_అంశం_కొలత,
                                       స్థిరరాశి uint8_t uc_వరుస_Type )
    {
        వరుస_t * px_New_వరుస = ఖాళీ;
        size_t x_వరుస_కొలత_లోపల_బైట్లు;
        uint8_t * puc_వరుస_Storage;

        ఒకవేళ( ( ux_వరుస_నిడివి > ( UBaseType_t ) 0 ) &&
            /* Check for multiplication overflow. */
            ( ( SIZE_MAX / ux_వరుస_నిడివి ) >= ux_అంశం_కొలత ) &&
            /* Check for addition overflow. */
            ( ( SIZE_MAX - పరిమాణం( వరుస_t ) ) >= ( ux_వరుస_నిడివి * ux_అంశం_కొలత ) ) )
        {
            /* Allocate enough space to hold the maximum number of items that
             * can be in the queue at any time.  It is valid for uxItemSize to be
             * zero in the case the queue is used as a semaphore. */
            x_వరుస_కొలత_లోపల_బైట్లు = ( size_t ) ( ux_వరుస_నిడివి * ux_అంశం_కొలత ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */

            /* Allocate the queue and storage area.  Justification for MISRA
             * deviation as follows:  pvPortMalloc() always ensures returned memory
             * blocks are aligned per the requirements of the MCU stack.  In this case
             * pvPortMalloc() must return a pointer that is guaranteed to meet the
             * alignment requirements of the Queue_t structure - which in this case
             * is an int8_t *.  Therefore, whenever the stack alignment requirements
             * are greater than or equal to the pointer to char requirements the cast
             * is safe.  In other cases alignment requirements are not strict (one or
             * two bytes). */
            px_New_వరుస = ( వరుస_t * ) pv_పోర్టు_Malloc( పరిమాణం( వరుస_t ) + x_వరుస_కొలత_లోపల_బైట్లు ); /*lint !e9087 !e9079 see comment above. */

            ఒకవేళ( px_New_వరుస != ఖాళీ )
            {
                /* Jump past the queue structure to find the location of the queue
                 * storage area. */
                puc_వరుస_Storage = ( uint8_t * ) px_New_వరుస;
                puc_వరుస_Storage += పరిమాణం( వరుస_t ); /*lint !e9016 Pointer arithmetic allowed on char types, especially when it assists conveying intent. */

                #ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )
                {
                    /* Queues can be created either statically or dynamically, so
                     * note this task was created dynamically in case it is later
                     * deleted. */
                    px_New_వరుస->ucStaticallyAllocated = pdFALSE;
                }
                #అయితే_ముగింపు /* configSUPPORT_STATIC_ALLOCATION */

                prv_Initialise_New_వరుస( ux_వరుస_నిడివి, ux_అంశం_కొలత, puc_వరుస_Storage, uc_వరుస_Type, px_New_వరుస );
            }
            లేకపోతే
            {
                trace_వరుస_CREATE_FAILED( uc_వరుస_Type );
                mtCOVERAGE_TEST_MARKER();
            }
        }
        లేకపోతే
        {
            configASSERT( px_New_వరుస );
            mtCOVERAGE_TEST_MARKER();
        }

        తిరిగి px_New_వరుస;
    }

#అయితే_ముగింపు /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

స్థిర శూన్య prv_Initialise_New_వరుస( స్థిరరాశి UBaseType_t ux_వరుస_నిడివి,
                                   స్థిరరాశి UBaseType_t ux_అంశం_కొలత,
                                   uint8_t * puc_వరుస_Storage,
                                   స్థిరరాశి uint8_t uc_వరుస_Type,
                                   వరుస_t * px_New_వరుస )
{
    /* Remove compiler warnings about unused parameters should
     * configUSE_TRACE_FACILITY not be set to 1. */
    ( శూన్య ) uc_వరుస_Type;

    ఒకవేళ( ux_అంశం_కొలత == ( UBaseType_t ) 0 )
    {
        /* No RAM was allocated for the queue storage area, but PC head cannot
         * be set to NULL because NULL is used as a key to say the queue is used as
         * a mutex.  Therefore just set pcHead to point to the queue as a benign
         * value that is known to be within the memory map. */
        px_New_వరుస->pc_తల = ( int8_t * ) px_New_వరుస;
    }
    లేకపోతే
    {
        /* Set the head to the start of the queue storage area. */
        px_New_వరుస->pc_తల = ( int8_t * ) puc_వరుస_Storage;
    }

    /* Initialise the queue members as described where the queue type is
     * defined. */
    px_New_వరుస->ux_నిడివి = ux_వరుస_నిడివి;
    px_New_వరుస->ux_అంశం_కొలత = ux_అంశం_కొలత;
    ( శూన్య ) x_వరుస_Generic_Reset( px_New_వరుస, pdTRUE );

    #ఒకవేళ ( configUSE_TRACE_FACILITY == 1 )
    {
        px_New_వరుస->uc_వరుస_Type = uc_వరుస_Type;
    }
    #అయితే_ముగింపు /* configUSE_TRACE_FACILITY */

    #ఒకవేళ ( config_USE_వరుస_SETS == 1 )
    {
        px_New_వరుస->px_వరుస_సమితి_Container = ఖాళీ;
    }
    #అయితే_ముగింపు /* configUSE_QUEUE_SETS */

    trace_వరుస_CREATE( px_New_వరుస );
}
/*-----------------------------------------------------------*/

#ఒకవేళ ( configUSE_MUTEXES == 1 )

    స్థిర శూన్య prvInitialiseMutex( వరుస_t * px_New_వరుస )
    {
        ఒకవేళ( px_New_వరుస != ఖాళీ )
        {
            /* The queue create function will set all the queue structure members
            * correctly for a generic queue, but this function is creating a
            * mutex.  Overwrite those members that need to be set differently -
            * in particular the information required for priority inheritance. */
            px_New_వరుస->u.xSemaphore.xMutexHolder = ఖాళీ;
            px_New_వరుస->ux_వరుస_Type = వరుస_వరుస_IS_MUTEX;

            /* In case this is a recursive mutex. */
            px_New_వరుస->u.xSemaphore.ux_Recursive_Call_లెక్క = 0;

            /* Initialize the mutex's spinlock */
            పోర్టు_MUX_INITIALIZE( &( px_New_వరుస->x_వరుస_తాళం ) );

            traceCREATE_MUTEX( px_New_వరుస );

            /* Start with the semaphore in the expected state. */
            ( శూన్య ) x_వరుస_Generic_Send( px_New_వరుస, ఖాళీ, ( TickType_t ) 0U, వరుస_SEND_గమ్యం_వెనుక );
        }
        లేకపోతే
        {
            traceCREATE_MUTEX_FAILED();
        }
    }

#అయితే_ముగింపు /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

#ఒకవేళ ( ( configUSE_MUTEXES == 1 ) && ( config_SUPPORT_DYNAMIC_కేటాయింపు == 1 ) )

    వరుస_Handle_t x_వరుస_Create_Mutex( స్థిరరాశి uint8_t uc_వరుస_Type )
    {
        వరుస_Handle_t x_New_వరుస;
        స్థిరరాశి UBaseType_t ux_Mutex_నిడివి = ( UBaseType_t ) 1, ux_Mutex_కొలత = ( UBaseType_t ) 0;

        x_New_వరుస = x_వరుస_Generic_Create( ux_Mutex_నిడివి, ux_Mutex_కొలత, uc_వరుస_Type );
        prvInitialiseMutex( ( వరుస_t * ) x_New_వరుస );

        తిరిగి x_New_వరుస;
    }

#అయితే_ముగింపు /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

#ఒకవేళ ( ( configUSE_MUTEXES == 1 ) && ( config_SUPPORT_STATIC_కేటాయింపు == 1 ) )

    వరుస_Handle_t x_వరుస_Create_Mutex_Static( స్థిరరాశి uint8_t uc_వరుస_Type,
                                           Static_వరుస_t * px_Static_వరుస )
    {
        వరుస_Handle_t x_New_వరుస;
        స్థిరరాశి UBaseType_t ux_Mutex_నిడివి = ( UBaseType_t ) 1, ux_Mutex_కొలత = ( UBaseType_t ) 0;

        /* Prevent compiler warnings about unused parameters if
         * configUSE_TRACE_FACILITY does not equal 1. */
        ( శూన్య ) uc_వరుస_Type;

        x_New_వరుస = x_వరుస_Generic_Create_Static( ux_Mutex_నిడివి, ux_Mutex_కొలత, ఖాళీ, px_Static_వరుస, uc_వరుస_Type );
        prvInitialiseMutex( ( వరుస_t * ) x_New_వరుస );

        తిరిగి x_New_వరుస;
    }

#అయితే_ముగింపు /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

#ఒకవేళ ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) )

    TaskHandle_t x_వరుస_Get_Mutex_Holder( వరుస_Handle_t xSemaphore )
    {
        TaskHandle_t pxReturn;
        వరుస_t * స్థిరరాశి pxSemaphore = ( వరుస_t * ) xSemaphore;

        configASSERT( xSemaphore );

        /* This function is called by xSemaphoreGetMutexHolder(), and should not
         * be called directly.  Note:  This is a good way of determining if the
         * calling task is the mutex holder, but not a good way of determining the
         * identity of the mutex holder, as the holder may change between the
         * following critical section exiting and the function returning. */
        taskENTER_CRITICAL( &( pxSemaphore->x_వరుస_తాళం ) );
        {
            ఒకవేళ( pxSemaphore->ux_వరుస_Type == వరుస_వరుస_IS_MUTEX )
            {
                pxReturn = pxSemaphore->u.xSemaphore.xMutexHolder;
            }
            లేకపోతే
            {
                pxReturn = ఖాళీ;
            }
        }
        taskEXIT_CRITICAL( &( pxSemaphore->x_వరుస_తాళం ) );

        తిరిగి pxReturn;
    } /*lint !e818 xSemaphore cannot be a pointer to const because it is a typedef. */

#అయితే_ముగింపు /* if ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) ) */
/*-----------------------------------------------------------*/

#ఒకవేళ ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) )

    TaskHandle_t x_వరుస_Get_Mutex_Holder_From_ISR( వరుస_Handle_t xSemaphore )
    {
        TaskHandle_t pxReturn;

        configASSERT( xSemaphore );

        /* Mutexes cannot be used in interrupt service routines, so the mutex
         * holder should not change in an ISR, and therefore a critical section is
         * not required here. */
        ఒకవేళ( ( ( వరుస_t * ) xSemaphore )->ux_వరుస_Type == వరుస_వరుస_IS_MUTEX )
        {
            pxReturn = ( ( వరుస_t * ) xSemaphore )->u.xSemaphore.xMutexHolder;
        }
        లేకపోతే
        {
            pxReturn = ఖాళీ;
        }

        తిరిగి pxReturn;
    } /*lint !e818 xSemaphore cannot be a pointer to const because it is a typedef. */

#అయితే_ముగింపు /* if ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) ) */
/*-----------------------------------------------------------*/

#ఒకవేళ ( configUSE_RECURSIVE_MUTEXES == 1 )

    పునాది_Type_t x_వరుస_Give_Mutex_Recursive( వరుస_Handle_t xMutex )
    {
        పునాది_Type_t xReturn;
        వరుస_t * స్థిరరాశి pxMutex = ( వరుస_t * ) xMutex;

        configASSERT( pxMutex );

        /* If this is the task that holds the mutex then xMutexHolder will not
         * change outside of this task.  If this task does not hold the mutex then
         * pxMutexHolder can never coincidentally equal the tasks handle, and as
         * this is the only condition we are interested in it does not matter if
         * pxMutexHolder is accessed simultaneously by another task.  Therefore no
         * mutual exclusion is required to test the pxMutexHolder variable. */
        ఒకవేళ( pxMutex->u.xSemaphore.xMutexHolder == xTaskGetCurrentTaskHandle() )
        {
            traceGIVE_MUTEX_RECURSIVE( pxMutex );

            /* uxRecursiveCallCount cannot be zero if xMutexHolder is equal to
             * the task handle, therefore no underflow check is required.  Also,
             * uxRecursiveCallCount is only modified by the mutex holder, and as
             * there can only be one, no mutual exclusion is required to modify the
             * uxRecursiveCallCount member. */
            ( pxMutex->u.xSemaphore.ux_Recursive_Call_లెక్క )--;

            /* Has the recursive call count unwound to 0? */
            ఒకవేళ( pxMutex->u.xSemaphore.ux_Recursive_Call_లెక్క == ( UBaseType_t ) 0 )
            {
                /* Return the mutex.  This will automatically unblock any other
                 * task that might be waiting to access the mutex. */
                ( శూన్య ) x_వరుస_Generic_Send( pxMutex, ఖాళీ, వరుస_MUTEX_GIVE_ఖండం_సమయం, వరుస_SEND_గమ్యం_వెనుక );
            }
            లేకపోతే
            {
                mtCOVERAGE_TEST_MARKER();
            }

            xReturn = pdPASS;
        }
        లేకపోతే
        {
            /* The mutex cannot be given because the calling task is not the
             * holder. */
            xReturn = pdFAIL;

            traceGIVE_MUTEX_RECURSIVE_FAILED( pxMutex );
        }

        తిరిగి xReturn;
    }

#అయితే_ముగింపు /* configUSE_RECURSIVE_MUTEXES */
/*-----------------------------------------------------------*/

#ఒకవేళ ( configUSE_RECURSIVE_MUTEXES == 1 )

    పునాది_Type_t x_వరుస_Take_Mutex_Recursive( వరుస_Handle_t xMutex,
                                         TickType_t x_Ticks_గమ్యం_Wait )
    {
        పునాది_Type_t xReturn;
        వరుస_t * స్థిరరాశి pxMutex = ( వరుస_t * ) xMutex;

        configASSERT( pxMutex );

        /* Comments regarding mutual exclusion as per those within
         * xQueueGiveMutexRecursive(). */

        traceTAKE_MUTEX_RECURSIVE( pxMutex );

        ఒకవేళ( pxMutex->u.xSemaphore.xMutexHolder == xTaskGetCurrentTaskHandle() )
        {
            ( pxMutex->u.xSemaphore.ux_Recursive_Call_లెక్క )++;
            xReturn = pdPASS;
        }
        లేకపోతే
        {
            xReturn = x_వరుస_Semaphore_Take( pxMutex, x_Ticks_గమ్యం_Wait );

            /* pdPASS will only be returned if the mutex was successfully
             * obtained.  The calling task may have entered the Blocked state
             * before reaching here. */
            ఒకవేళ( xReturn != pdFAIL )
            {
                ( pxMutex->u.xSemaphore.ux_Recursive_Call_లెక్క )++;
            }
            లేకపోతే
            {
                traceTAKE_MUTEX_RECURSIVE_FAILED( pxMutex );
            }
        }

        తిరిగి xReturn;
    }

#అయితే_ముగింపు /* configUSE_RECURSIVE_MUTEXES */
/*-----------------------------------------------------------*/

#ఒకవేళ ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( config_SUPPORT_STATIC_కేటాయింపు == 1 ) )

    వరుస_Handle_t x_వరుస_Create_Counting_Semaphore_Static( స్థిరరాశి UBaseType_t ux_Max_లెక్క,
                                                       స్థిరరాశి UBaseType_t ux_Initial_లెక్క,
                                                       Static_వరుస_t * px_Static_వరుస )
    {
        వరుస_Handle_t xHandle = ఖాళీ;

        ఒకవేళ( ( ux_Max_లెక్క != 0 ) &&
            ( ux_Initial_లెక్క <= ux_Max_లెక్క ) )
        {
            xHandle = x_వరుస_Generic_Create_Static( ux_Max_లెక్క, వరుస_SEMAPHORE_వరుస_అంశం_నిడివి, ఖాళీ, px_Static_వరుస, వరుస_వరుస_TYPE_COUNTING_SEMAPHORE );

            ఒకవేళ( xHandle != ఖాళీ )
            {
                ( ( వరుస_t * ) xHandle )->ux_Messages_నిరీక్షణ = ux_Initial_లెక్క;

                traceCREATE_COUNTING_SEMAPHORE();
            }
            లేకపోతే
            {
                traceCREATE_COUNTING_SEMAPHORE_FAILED();
            }
        }
        లేకపోతే
        {
            configASSERT( xHandle );
            mtCOVERAGE_TEST_MARKER();
        }

        తిరిగి xHandle;
    }

#అయితే_ముగింపు /* ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) ) */
/*-----------------------------------------------------------*/

#ఒకవేళ ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( config_SUPPORT_DYNAMIC_కేటాయింపు == 1 ) )

    వరుస_Handle_t x_వరుస_Create_Counting_Semaphore( స్థిరరాశి UBaseType_t ux_Max_లెక్క,
                                                 స్థిరరాశి UBaseType_t ux_Initial_లెక్క )
    {
        వరుస_Handle_t xHandle = ఖాళీ;

        ఒకవేళ( ( ux_Max_లెక్క != 0 ) &&
            ( ux_Initial_లెక్క <= ux_Max_లెక్క ) )
        {
            xHandle = x_వరుస_Generic_Create( ux_Max_లెక్క, వరుస_SEMAPHORE_వరుస_అంశం_నిడివి, వరుస_వరుస_TYPE_COUNTING_SEMAPHORE );

            ఒకవేళ( xHandle != ఖాళీ )
            {
                ( ( వరుస_t * ) xHandle )->ux_Messages_నిరీక్షణ = ux_Initial_లెక్క;

                traceCREATE_COUNTING_SEMAPHORE();
            }
            లేకపోతే
            {
                traceCREATE_COUNTING_SEMAPHORE_FAILED();
            }
        }
        లేకపోతే
        {
            configASSERT( xHandle );
            mtCOVERAGE_TEST_MARKER();
        }

        తిరిగి xHandle;
    }

#అయితే_ముగింపు /* ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) ) */
/*-----------------------------------------------------------*/

పునాది_Type_t x_వరుస_Generic_Send( వరుస_Handle_t x_వరుస,
                              స్థిరరాశి శూన్య * స్థిరరాశి pv_అంశం_గమ్యం_వరుస,
                              TickType_t x_Ticks_గమ్యం_Wait,
                              స్థిరరాశి పునాది_Type_t xCopyPosition )
{
    పునాది_Type_t x_నమోదు_సమయం_సమితి = pdFALSE, xYieldRequired;
    సమయం_బహిర్గతం_t x_సమయం_బహిర్గతం_2;
    వరుస_t * స్థిరరాశి px_వరుస = x_వరుస;

    configASSERT( px_వరుస );
    configASSERT( !( ( pv_అంశం_గమ్యం_వరుస == ఖాళీ ) && ( px_వరుస->ux_అంశం_కొలత != ( UBaseType_t ) 0U ) ) );
    configASSERT( !( ( xCopyPosition == వరుస_OVERWRITE ) && ( px_వరుస->ux_నిడివి != 1 ) ) );
    #ఒకవేళ ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_గమ్యం_Wait != 0 ) ) );
    }
    #అయితే_ముగింపు

    /*lint -save -e904 This function relaxes the coding standard somewhat to
     * allow return statements within the function itself.  This is done in the
     * interest of execution time efficiency. */
    కొరకు( ; ; )
    {
        taskENTER_CRITICAL( &( px_వరుస->x_వరుస_తాళం ) );
        {
            /* Is there room on the queue now?  The running task must be the
             * highest priority task wanting to access the queue.  If the head item
             * in the queue is to be overwritten then it does not matter if the
             * queue is full. */
            ఒకవేళ( ( px_వరుస->ux_Messages_నిరీక్షణ < px_వరుస->ux_నిడివి ) || ( xCopyPosition == వరుస_OVERWRITE ) )
            {
                trace_వరుస_SEND( px_వరుస );

                #ఒకవేళ ( config_USE_వరుస_SETS == 1 )
                {
                    స్థిరరాశి UBaseType_t ux_మునుపటి_Messages_నిరీక్షణ = px_వరుస->ux_Messages_నిరీక్షణ;

                    xYieldRequired = prv_Copy_దత్తాంశం_గమ్యం_వరుస( px_వరుస, pv_అంశం_గమ్యం_వరుస, xCopyPosition );

                    ఒకవేళ( px_వరుస->px_వరుస_సమితి_Container != ఖాళీ )
                    {
                        ఒకవేళ( ( xCopyPosition == వరుస_OVERWRITE ) && ( ux_మునుపటి_Messages_నిరీక్షణ != ( UBaseType_t ) 0 ) )
                        {
                            /* Do not notify the queue set as an existing item
                             * was overwritten in the queue so the number of items
                             * in the queue has not changed. */
                            mtCOVERAGE_TEST_MARKER();
                        }
                        లేకపోతే ఒకవేళ( prv_Notify_వరుస_సమితి_Container( px_వరుస ) != pdFALSE )
                        {
                            /* The queue is a member of a queue set, and posting
                             * to the queue set caused a higher priority task to
                             * unblock. A context switch is required. */
                            వరుస_YIELD_IF_USING_PREEMPTION();
                        }
                        లేకపోతే
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    లేకపోతే
                    {
                        /* If there was a task waiting for data to arrive on the
                         * queue then unblock it now. */
                        ఒకవేళ( list_LIST_IS_ఖాళీలు( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Receive ) ) == pdFALSE )
                        {
                            ఒకవేళ( xTaskRemoveFromEventList( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Receive ) ) != pdFALSE )
                            {
                                /* The unblocked task has a priority higher than
                                 * our own so yield immediately.  Yes it is ok to
                                 * do this from within the critical section - the
                                 * kernel takes care of that. */
                                వరుస_YIELD_IF_USING_PREEMPTION();
                            }
                            లేకపోతే
                            {
                                mtCOVERAGE_TEST_MARKER();
                            }
                        }
                        లేకపోతే ఒకవేళ( xYieldRequired != pdFALSE )
                        {
                            /* This path is a special case that will only get
                             * executed if the task was holding multiple mutexes
                             * and the mutexes were given back in an order that is
                             * different to that in which they were taken. */
                            వరుస_YIELD_IF_USING_PREEMPTION();
                        }
                        లేకపోతే
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                }
                #లేకపోతే /* configUSE_QUEUE_SETS */
                {
                    xYieldRequired = prv_Copy_దత్తాంశం_గమ్యం_వరుస( px_వరుస, pv_అంశం_గమ్యం_వరుస, xCopyPosition );

                    /* If there was a task waiting for data to arrive on the
                     * queue then unblock it now. */
                    ఒకవేళ( list_LIST_IS_ఖాళీలు( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Receive ) ) == pdFALSE )
                    {
                        ఒకవేళ( xTaskRemoveFromEventList( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Receive ) ) != pdFALSE )
                        {
                            /* The unblocked task has a priority higher than
                             * our own so yield immediately.  Yes it is ok to do
                             * this from within the critical section - the kernel
                             * takes care of that. */
                            వరుస_YIELD_IF_USING_PREEMPTION();
                        }
                        లేకపోతే
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    లేకపోతే ఒకవేళ( xYieldRequired != pdFALSE )
                    {
                        /* This path is a special case that will only get
                         * executed if the task was holding multiple mutexes and
                         * the mutexes were given back in an order that is
                         * different to that in which they were taken. */
                        వరుస_YIELD_IF_USING_PREEMPTION();
                    }
                    లేకపోతే
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #అయితే_ముగింపు /* configUSE_QUEUE_SETS */

                taskEXIT_CRITICAL( &( px_వరుస->x_వరుస_తాళం ) );
                తిరిగి pdPASS;
            }
            లేకపోతే
            {
                ఒకవేళ( x_Ticks_గమ్యం_Wait == ( TickType_t ) 0 )
                {
                    /* The queue was full and no block time is specified (or
                     * the block time has expired) so leave now. */
                    taskEXIT_CRITICAL( &( px_వరుస->x_వరుస_తాళం ) );

                    /* Return to the original privilege level before exiting
                     * the function. */
                    trace_వరుస_SEND_FAILED( px_వరుస );
                    తిరిగి err_వరుస_నిండిన;
                }
                లేకపోతే ఒకవేళ( x_నమోదు_సమయం_సమితి == pdFALSE )
                {
                    /* The queue was full and a block time was specified so
                     * configure the timeout structure. */
                    v_Task_అంతర్గత_సమితి_సమయం_బహిర్గతం_State( &x_సమయం_బహిర్గతం_2 );
                    x_నమోదు_సమయం_సమితి = pdTRUE;
                }
                లేకపోతే
                {
                    /* Entry time was already set. */
                    mtCOVERAGE_TEST_MARKER();
                }
            }

            /* If queue locks ARE NOT being used:
             * - At this point, the queue is full and entry time has been set
             * - We simply check for a time out, block if not timed out, or
             *   return an error if we have timed out. */
            #ఒకవేళ ( వరుస_USE_LOCKS == 0 )
            {
                /* Update the timeout state to see if it has expired yet. */
                ఒకవేళ( x_Task_Check_For_సమయం_బహిర్గతం( &x_సమయం_బహిర్గతం_2, &x_Ticks_గమ్యం_Wait ) == pdFALSE )
                {
                    /* Not timed out yet. Block the current task. */
                    trace_BLOCKING_ON_వరుస_SEND( px_వరుస );
                    v_Task_ఉంచు_On_Event_List( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Send ), x_Ticks_గమ్యం_Wait );
                    పోర్టు_YIELD_WITHIN_API();
                }
                లేకపోతే
                {
                    /* We have timed out. Return an error. */
                    taskEXIT_CRITICAL( &( px_వరుస->x_వరుస_తాళం ) );
                    trace_వరుస_SEND_FAILED( px_వరుస );
                    తిరిగి err_వరుస_నిండిన;
                }
            }
            #అయితే_ముగింపు /* queueUSE_LOCKS == 0 */
        }
        taskEXIT_CRITICAL( &( px_వరుస->x_వరుస_తాళం ) );

        /* If queue locks ARE being used:
         * - At this point, the queue is full and entry time has been set
         * - We follow the original procedure of locking the queue before
         *   attempting to block. */
        #ఒకవేళ ( వరుస_USE_LOCKS == 1 )
        {
            /* Interrupts and other tasks can send to and receive from the queue
             * now the critical section has been exited. */

            vTaskSuspendAll();
            prv_తాళం_వరుస( px_వరుస );

            /* Update the timeout state to see if it has expired yet. */
            ఒకవేళ( x_Task_Check_For_సమయం_బహిర్గతం( &x_సమయం_బహిర్గతం_2, &x_Ticks_గమ్యం_Wait ) == pdFALSE )
            {
                ఒకవేళ( prv_Is_వరుస_నిండిన( px_వరుస ) != pdFALSE )
                {
                    trace_BLOCKING_ON_వరుస_SEND( px_వరుస );
                    v_Task_ఉంచు_On_Event_List( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Send ), x_Ticks_గమ్యం_Wait );

                    /* Unlocking the queue means queue events can effect the
                     * event list. It is possible that interrupts occurring now
                     * remove this task from the event list again - but as the
                     * scheduler is suspended the task will go onto the pending
                     * ready list instead of the actual ready list. */
                    prv_Unlock_వరుస( px_వరుస );

                    /* Resuming the scheduler will move tasks from the pending
                     * ready list into the ready list - so it is feasible that this
                     * task is already in the ready list before it yields - in which
                     * case the yield will not cause a context switch unless there
                     * is also a higher priority task in the pending ready list. */
                    ఒకవేళ( xTaskResumeAll() == pdFALSE )
                    {
                        పోర్టు_YIELD_WITHIN_API();
                    }
                }
                లేకపోతే
                {
                    /* Try again. */
                    prv_Unlock_వరుస( px_వరుస );
                    ( శూన్య ) xTaskResumeAll();
                }
            }
            లేకపోతే
            {
                /* The timeout has expired. */
                prv_Unlock_వరుస( px_వరుస );
                ( శూన్య ) xTaskResumeAll();

                trace_వరుస_SEND_FAILED( px_వరుస );
                తిరిగి err_వరుస_నిండిన;
            }
        }
        #అయితే_ముగింపు /* queueUSE_LOCKS == 1 */
    } /*lint -restore */
}
/*-----------------------------------------------------------*/

పునాది_Type_t x_వరుస_Generic_Send_From_ISR( వరుస_Handle_t x_వరుస,
                                     స్థిరరాశి శూన్య * స్థిరరాశి pv_అంశం_గమ్యం_వరుస,
                                     పునాది_Type_t * స్థిరరాశి px_Higher_ప్రాధాన్యత_Task_Woken,
                                     స్థిరరాశి పునాది_Type_t xCopyPosition )
{
    పునాది_Type_t xReturn;
    UBaseType_t ux_Saved_అంతరాయం_స్థితి;
    వరుస_t * స్థిరరాశి px_వరుస = x_వరుస;

    configASSERT( px_వరుస );
    configASSERT( !( ( pv_అంశం_గమ్యం_వరుస == ఖాళీ ) && ( px_వరుస->ux_అంశం_కొలత != ( UBaseType_t ) 0U ) ) );
    configASSERT( !( ( xCopyPosition == వరుస_OVERWRITE ) && ( px_వరుస->ux_నిడివి != 1 ) ) );

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
    పోర్టు_ASSERT_IF_అంతరాయం_ప్రాధాన్యత_INVALID();

    /* Similar to xQueueGenericSend, except without blocking if there is no room
     * in the queue.  Also don't directly wake a task that was blocked on a queue
     * read, instead return a flag to say whether a context switch is required or
     * not (i.e. has a task with a higher priority than us been woken by this
     * post). */
    prv_ENTER_CRITICAL_OR_ముసుగు_ISR( &( px_వరుస->x_వరుస_తాళం ), ux_Saved_అంతరాయం_స్థితి );
    {
        ఒకవేళ( ( px_వరుస->ux_Messages_నిరీక్షణ < px_వరుస->ux_నిడివి ) || ( xCopyPosition == వరుస_OVERWRITE ) )
        {
            #ఒకవేళ ( వరుస_USE_LOCKS == 1 )
                స్థిరరాశి int8_t c_Tx_తాళం = px_వరుస->c_Tx_తాళం;
            #లేకపోతే
                /* Queue locks not used, so we treat it as unlocked. */
                స్థిరరాశి int8_t c_Tx_తాళం = వరుస_UNLOCKED;
            #అయితే_ముగింపు /* queueUSE_LOCKS == 1 */
            స్థిరరాశి UBaseType_t ux_మునుపటి_Messages_నిరీక్షణ = px_వరుస->ux_Messages_నిరీక్షణ;

            trace_వరుస_SEND_FROM_ISR( px_వరుస );

            /* Semaphores use xQueueGiveFromISR(), so pxQueue will not be a
             *  semaphore or mutex.  That means prvCopyDataToQueue() cannot result
             *  in a task disinheriting a priority and prvCopyDataToQueue() can be
             *  called here even though the disinherit function does not check if
             *  the scheduler is suspended before accessing the ready lists. */
            ( శూన్య ) prv_Copy_దత్తాంశం_గమ్యం_వరుస( px_వరుస, pv_అంశం_గమ్యం_వరుస, xCopyPosition );

            /* The event list is not altered if the queue is locked.  This will
             * be done when the queue is unlocked later. */
            ఒకవేళ( c_Tx_తాళం == వరుస_UNLOCKED )
            {
                #ఒకవేళ ( config_USE_వరుస_SETS == 1 )
                {
                    ఒకవేళ( px_వరుస->px_వరుస_సమితి_Container != ఖాళీ )
                    {
                        ఒకవేళ( ( xCopyPosition == వరుస_OVERWRITE ) && ( ux_మునుపటి_Messages_నిరీక్షణ != ( UBaseType_t ) 0 ) )
                        {
                            /* Do not notify the queue set as an existing item
                             * was overwritten in the queue so the number of items
                             * in the queue has not changed. */
                            mtCOVERAGE_TEST_MARKER();
                        }
                        లేకపోతే ఒకవేళ( prv_Notify_వరుస_సమితి_Container( px_వరుస ) != pdFALSE )
                        {
                            /* The queue is a member of a queue set, and posting
                             * to the queue set caused a higher priority task to
                             * unblock.  A context switch is required. */
                            ఒకవేళ( px_Higher_ప్రాధాన్యత_Task_Woken != ఖాళీ )
                            {
                                *px_Higher_ప్రాధాన్యత_Task_Woken = pdTRUE;
                            }
                            లేకపోతే
                            {
                                mtCOVERAGE_TEST_MARKER();
                            }
                        }
                        లేకపోతే
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    లేకపోతే
                    {
                        ఒకవేళ( list_LIST_IS_ఖాళీలు( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Receive ) ) == pdFALSE )
                        {
                            ఒకవేళ( xTaskRemoveFromEventList( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Receive ) ) != pdFALSE )
                            {
                                /* The task waiting has a higher priority so
                                 *  record that a context switch is required. */
                                ఒకవేళ( px_Higher_ప్రాధాన్యత_Task_Woken != ఖాళీ )
                                {
                                    *px_Higher_ప్రాధాన్యత_Task_Woken = pdTRUE;
                                }
                                లేకపోతే
                                {
                                    mtCOVERAGE_TEST_MARKER();
                                }
                            }
                            లేకపోతే
                            {
                                mtCOVERAGE_TEST_MARKER();
                            }
                        }
                        లేకపోతే
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                }
                #లేకపోతే /* configUSE_QUEUE_SETS */
                {
                    ఒకవేళ( list_LIST_IS_ఖాళీలు( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Receive ) ) == pdFALSE )
                    {
                        ఒకవేళ( xTaskRemoveFromEventList( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Receive ) ) != pdFALSE )
                        {
                            /* The task waiting has a higher priority so record that a
                             * context switch is required. */
                            ఒకవేళ( px_Higher_ప్రాధాన్యత_Task_Woken != ఖాళీ )
                            {
                                *px_Higher_ప్రాధాన్యత_Task_Woken = pdTRUE;
                            }
                            లేకపోతే
                            {
                                mtCOVERAGE_TEST_MARKER();
                            }
                        }
                        లేకపోతే
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    లేకపోతే
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }

                    /* Not used in this path. */
                    ( శూన్య ) ux_మునుపటి_Messages_నిరీక్షణ;
                }
                #అయితే_ముగింపు /* configUSE_QUEUE_SETS */
            }
            లేకపోతే
            {
                #ఒకవేళ ( వరుస_USE_LOCKS == 1 )
                {
                    /* Increment the lock count so the task that unlocks the queue
                     * knows that data was posted while it was locked. */
                    prv_Increment_వరుస_Tx_తాళం( px_వరుస, c_Tx_తాళం );
                }
                #అయితే_ముగింపు /* queueUSE_LOCKS == 1 */
            }

            xReturn = pdPASS;
        }
        లేకపోతే
        {
            trace_వరుస_SEND_FROM_ISR_FAILED( px_వరుస );
            xReturn = err_వరుస_నిండిన;
        }
    }
    prvEXIT_CRITICAL_OR_UNMASK_ISR( &( px_వరుస->x_వరుస_తాళం ), ux_Saved_అంతరాయం_స్థితి );

    తిరిగి xReturn;
}
/*-----------------------------------------------------------*/

పునాది_Type_t x_వరుస_Give_From_ISR( వరుస_Handle_t x_వరుస,
                              పునాది_Type_t * స్థిరరాశి px_Higher_ప్రాధాన్యత_Task_Woken )
{
    పునాది_Type_t xReturn;
    UBaseType_t ux_Saved_అంతరాయం_స్థితి;
    వరుస_t * స్థిరరాశి px_వరుస = x_వరుస;

    /* Similar to xQueueGenericSendFromISR() but used with semaphores where the
     * item size is 0.  Don't directly wake a task that was blocked on a queue
     * read, instead return a flag to say whether a context switch is required or
     * not (i.e. has a task with a higher priority than us been woken by this
     * post). */

    configASSERT( px_వరుస );

    /* xQueueGenericSendFromISR() should be used instead of xQueueGiveFromISR()
     * if the item size is not 0. */
    configASSERT( px_వరుస->ux_అంశం_కొలత == 0 );

    /* Normally a mutex would not be given from an interrupt, especially if
     * there is a mutex holder, as priority inheritance makes no sense for an
     * interrupts, only tasks. */
    configASSERT( !( ( px_వరుస->ux_వరుస_Type == వరుస_వరుస_IS_MUTEX ) && ( px_వరుస->u.xSemaphore.xMutexHolder != ఖాళీ ) ) );

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
    పోర్టు_ASSERT_IF_అంతరాయం_ప్రాధాన్యత_INVALID();

    prv_ENTER_CRITICAL_OR_ముసుగు_ISR( &( px_వరుస->x_వరుస_తాళం ), ux_Saved_అంతరాయం_స్థితి );
    {
        స్థిరరాశి UBaseType_t ux_Messages_నిరీక్షణ = px_వరుస->ux_Messages_నిరీక్షణ;

        /* When the queue is used to implement a semaphore no data is ever
         * moved through the queue but it is still valid to see if the queue 'has
         * space'. */
        ఒకవేళ( ux_Messages_నిరీక్షణ < px_వరుస->ux_నిడివి )
        {
            #ఒకవేళ ( వరుస_USE_LOCKS == 1 )
                స్థిరరాశి int8_t c_Tx_తాళం = px_వరుస->c_Tx_తాళం;
            #లేకపోతే
                /* Queue locks not used, so we treat it as unlocked. */
                స్థిరరాశి int8_t c_Tx_తాళం = వరుస_UNLOCKED;
            #అయితే_ముగింపు /* queueUSE_LOCKS == 1 */

            /* Todo: Reconcile tracing differences (IDF-8183) */
            trace_వరుస_GIVE_FROM_ISR( px_వరుస );

            /* A task can only have an inherited priority if it is a mutex
             * holder - and if there is a mutex holder then the mutex cannot be
             * given from an ISR.  As this is the ISR version of the function it
             * can be assumed there is no mutex holder and no need to determine if
             * priority disinheritance is needed.  Simply increase the count of
             * messages (semaphores) available. */
            px_వరుస->ux_Messages_నిరీక్షణ = ux_Messages_నిరీక్షణ + ( UBaseType_t ) 1;

            /* The event list is not altered if the queue is locked.  This will
             * be done when the queue is unlocked later. */
            ఒకవేళ( c_Tx_తాళం == వరుస_UNLOCKED )
            {
                #ఒకవేళ ( config_USE_వరుస_SETS == 1 )
                {
                    ఒకవేళ( px_వరుస->px_వరుస_సమితి_Container != ఖాళీ )
                    {
                        ఒకవేళ( prv_Notify_వరుస_సమితి_Container( px_వరుస ) != pdFALSE )
                        {
                            /* The semaphore is a member of a queue set, and
                             * posting to the queue set caused a higher priority
                             * task to unblock.  A context switch is required. */
                            ఒకవేళ( px_Higher_ప్రాధాన్యత_Task_Woken != ఖాళీ )
                            {
                                *px_Higher_ప్రాధాన్యత_Task_Woken = pdTRUE;
                            }
                            లేకపోతే
                            {
                                mtCOVERAGE_TEST_MARKER();
                            }
                        }
                        లేకపోతే
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    లేకపోతే
                    {
                        ఒకవేళ( list_LIST_IS_ఖాళీలు( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Receive ) ) == pdFALSE )
                        {
                            ఒకవేళ( xTaskRemoveFromEventList( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Receive ) ) != pdFALSE )
                            {
                                /* The task waiting has a higher priority so
                                 *  record that a context switch is required. */
                                ఒకవేళ( px_Higher_ప్రాధాన్యత_Task_Woken != ఖాళీ )
                                {
                                    *px_Higher_ప్రాధాన్యత_Task_Woken = pdTRUE;
                                }
                                లేకపోతే
                                {
                                    mtCOVERAGE_TEST_MARKER();
                                }
                            }
                            లేకపోతే
                            {
                                mtCOVERAGE_TEST_MARKER();
                            }
                        }
                        లేకపోతే
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                }
                #లేకపోతే /* configUSE_QUEUE_SETS */
                {
                    ఒకవేళ( list_LIST_IS_ఖాళీలు( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Receive ) ) == pdFALSE )
                    {
                        ఒకవేళ( xTaskRemoveFromEventList( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Receive ) ) != pdFALSE )
                        {
                            /* The task waiting has a higher priority so record that a
                             * context switch is required. */
                            ఒకవేళ( px_Higher_ప్రాధాన్యత_Task_Woken != ఖాళీ )
                            {
                                *px_Higher_ప్రాధాన్యత_Task_Woken = pdTRUE;
                            }
                            లేకపోతే
                            {
                                mtCOVERAGE_TEST_MARKER();
                            }
                        }
                        లేకపోతే
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    లేకపోతే
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #అయితే_ముగింపు /* configUSE_QUEUE_SETS */
            }
            లేకపోతే
            {
                #ఒకవేళ ( వరుస_USE_LOCKS == 1 )
                {
                    /* Increment the lock count so the task that unlocks the queue
                     * knows that data was posted while it was locked. */
                    prv_Increment_వరుస_Tx_తాళం( px_వరుస, c_Tx_తాళం );
                }
                #అయితే_ముగింపు /* queueUSE_LOCKS == 1 */
            }

            xReturn = pdPASS;
        }
        లేకపోతే
        {
            /* Todo: Reconcile tracing differences (IDF-8183) */
            trace_వరుస_GIVE_FROM_ISR_FAILED( px_వరుస );
            xReturn = err_వరుస_నిండిన;
        }
    }
    prvEXIT_CRITICAL_OR_UNMASK_ISR( &( px_వరుస->x_వరుస_తాళం ), ux_Saved_అంతరాయం_స్థితి );

    తిరిగి xReturn;
}
/*-----------------------------------------------------------*/

పునాది_Type_t x_వరుస_Receive( వరుస_Handle_t x_వరుస,
                          శూన్య * స్థిరరాశి pv_బఫరు,
                          TickType_t x_Ticks_గమ్యం_Wait )
{
    పునాది_Type_t x_నమోదు_సమయం_సమితి = pdFALSE;
    సమయం_బహిర్గతం_t x_సమయం_బహిర్గతం_2;
    వరుస_t * స్థిరరాశి px_వరుస = x_వరుస;

    /* Check the pointer is not NULL. */
    configASSERT( ( px_వరుస ) );

    /* The buffer into which data is received can only be NULL if the data size
     * is zero (so no data is copied into the buffer). */
    configASSERT( !( ( ( pv_బఫరు ) == ఖాళీ ) && ( ( px_వరుస )->ux_అంశం_కొలత != ( UBaseType_t ) 0U ) ) );

    /* Cannot block if the scheduler is suspended. */
    #ఒకవేళ ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_గమ్యం_Wait != 0 ) ) );
    }
    #అయితే_ముగింపు

    /*lint -save -e904  This function relaxes the coding standard somewhat to
     * allow return statements within the function itself.  This is done in the
     * interest of execution time efficiency. */
    కొరకు( ; ; )
    {
        taskENTER_CRITICAL( &( px_వరుస->x_వరుస_తాళం ) );
        {
            స్థిరరాశి UBaseType_t ux_Messages_నిరీక్షణ = px_వరుస->ux_Messages_నిరీక్షణ;

            /* Is there data in the queue now?  To be running the calling task
             * must be the highest priority task wanting to access the queue. */
            ఒకవేళ( ux_Messages_నిరీక్షణ > ( UBaseType_t ) 0 )
            {
                /* Data available, remove one item. */
                prv_Copy_దత్తాంశం_From_వరుస( px_వరుస, pv_బఫరు );
                trace_వరుస_RECEIVE( px_వరుస );
                px_వరుస->ux_Messages_నిరీక్షణ = ux_Messages_నిరీక్షణ - ( UBaseType_t ) 1;

                /* There is now space in the queue, were any tasks waiting to
                 * post to the queue?  If so, unblock the highest priority waiting
                 * task. */
                ఒకవేళ( list_LIST_IS_ఖాళీలు( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Send ) ) == pdFALSE )
                {
                    ఒకవేళ( xTaskRemoveFromEventList( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Send ) ) != pdFALSE )
                    {
                        వరుస_YIELD_IF_USING_PREEMPTION();
                    }
                    లేకపోతే
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                లేకపోతే
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                taskEXIT_CRITICAL( &( px_వరుస->x_వరుస_తాళం ) );
                తిరిగి pdPASS;
            }
            లేకపోతే
            {
                ఒకవేళ( x_Ticks_గమ్యం_Wait == ( TickType_t ) 0 )
                {
                    /* The queue was empty and no block time is specified (or
                     * the block time has expired) so leave now. */
                    taskEXIT_CRITICAL( &( px_వరుస->x_వరుస_తాళం ) );
                    trace_వరుస_RECEIVE_FAILED( px_వరుస );
                    తిరిగి err_వరుస_ఖాళీలు;
                }
                లేకపోతే ఒకవేళ( x_నమోదు_సమయం_సమితి == pdFALSE )
                {
                    /* The queue was empty and a block time was specified so
                     * configure the timeout structure. */
                    v_Task_అంతర్గత_సమితి_సమయం_బహిర్గతం_State( &x_సమయం_బహిర్గతం_2 );
                    x_నమోదు_సమయం_సమితి = pdTRUE;
                }
                లేకపోతే
                {
                    /* Entry time was already set. */
                    mtCOVERAGE_TEST_MARKER();
                }
            }

            /* If queue locks ARE NOT being used:
             * - At this point, the queue is empty and entry time has been set
             * - We simply check for a time out, block if not timed out, or
             *   return an error if we have timed out. */
            #ఒకవేళ ( వరుస_USE_LOCKS == 0 )
            {
                /* Update the timeout state to see if it has expired yet. */
                ఒకవేళ( x_Task_Check_For_సమయం_బహిర్గతం( &x_సమయం_బహిర్గతం_2, &x_Ticks_గమ్యం_Wait ) == pdFALSE )
                {
                    /* Not timed out yet. Block the current task. */
                    trace_BLOCKING_ON_వరుస_RECEIVE( px_వరుస );
                    v_Task_ఉంచు_On_Event_List( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Receive ), x_Ticks_గమ్యం_Wait );
                    పోర్టు_YIELD_WITHIN_API();
                }
                లేకపోతే
                {
                    /* We have timed out. Return an error. */
                    taskEXIT_CRITICAL( &( px_వరుస->x_వరుస_తాళం ) );
                    trace_వరుస_RECEIVE_FAILED( px_వరుస );
                    తిరిగి err_వరుస_ఖాళీలు;
                }
            }
            #అయితే_ముగింపు /* queueUSE_LOCKS == 0 */
        }
        taskEXIT_CRITICAL( &( px_వరుస->x_వరుస_తాళం ) );

        /* If queue locks ARE being used:
         * - At this point, the queue is empty and entry time has been set
         * - We follow the original procedure for locking the queue before
         *   attempting to block. */
        #ఒకవేళ ( వరుస_USE_LOCKS == 1 )
        {
            /* Interrupts and other tasks can send to and receive from the queue
             * now the critical section has been exited. */

            vTaskSuspendAll();
            prv_తాళం_వరుస( px_వరుస );

            /* Update the timeout state to see if it has expired yet. */
            ఒకవేళ( x_Task_Check_For_సమయం_బహిర్గతం( &x_సమయం_బహిర్గతం_2, &x_Ticks_గమ్యం_Wait ) == pdFALSE )
            {
                /* The timeout has not expired.  If the queue is still empty place
                 * the task on the list of tasks waiting to receive from the queue. */
                ఒకవేళ( prv_Is_వరుస_ఖాళీలు( px_వరుస ) != pdFALSE )
                {
                    trace_BLOCKING_ON_వరుస_RECEIVE( px_వరుస );
                    v_Task_ఉంచు_On_Event_List( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Receive ), x_Ticks_గమ్యం_Wait );
                    prv_Unlock_వరుస( px_వరుస );

                    ఒకవేళ( xTaskResumeAll() == pdFALSE )
                    {
                        పోర్టు_YIELD_WITHIN_API();
                    }
                    లేకపోతే
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                లేకపోతే
                {
                    /* The queue contains data again.  Loop back to try and read the
                     * data. */
                    prv_Unlock_వరుస( px_వరుస );
                    ( శూన్య ) xTaskResumeAll();
                }
            }
            లేకపోతే
            {
                /* Timed out.  If there is no data in the queue exit, otherwise loop
                 * back and attempt to read the data. */
                prv_Unlock_వరుస( px_వరుస );
                ( శూన్య ) xTaskResumeAll();

                ఒకవేళ( prv_Is_వరుస_ఖాళీలు( px_వరుస ) != pdFALSE )
                {
                    trace_వరుస_RECEIVE_FAILED( px_వరుస );
                    తిరిగి err_వరుస_ఖాళీలు;
                }
                లేకపోతే
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
        }
        #అయితే_ముగింపు /* queueUSE_LOCKS == 1 */
    } /*lint -restore */
}
/*-----------------------------------------------------------*/

పునాది_Type_t x_వరుస_Semaphore_Take( వరుస_Handle_t x_వరుస,
                                TickType_t x_Ticks_గమ్యం_Wait )
{
    పునాది_Type_t x_నమోదు_సమయం_సమితి = pdFALSE;
    సమయం_బహిర్గతం_t x_సమయం_బహిర్గతం_2;
    వరుస_t * స్థిరరాశి px_వరుస = x_వరుస;

    #ఒకవేళ ( configUSE_MUTEXES == 1 )
        పునాది_Type_t xInheritanceOccurred = pdFALSE;
    #అయితే_ముగింపు

    /* Check the queue pointer is not NULL. */
    configASSERT( ( px_వరుస ) );

    /* Check this really is a semaphore, in which case the item size will be
     * 0. */
    configASSERT( px_వరుస->ux_అంశం_కొలత == 0 );

    /* Cannot block if the scheduler is suspended. */
    #ఒకవేళ ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_గమ్యం_Wait != 0 ) ) );
    }
    #అయితే_ముగింపు

    /*lint -save -e904 This function relaxes the coding standard somewhat to allow return
     * statements within the function itself.  This is done in the interest
     * of execution time efficiency. */
    కొరకు( ; ; )
    {
        taskENTER_CRITICAL( &( px_వరుస->x_వరుస_తాళం ) );
        {
            /* Semaphores are queues with an item size of 0, and where the
             * number of messages in the queue is the semaphore's count value. */
            స్థిరరాశి UBaseType_t ux_Semaphore_లెక్క = px_వరుస->ux_Messages_నిరీక్షణ;

            /* Is there data in the queue now?  To be running the calling task
             * must be the highest priority task wanting to access the queue. */
            ఒకవేళ( ux_Semaphore_లెక్క > ( UBaseType_t ) 0 )
            {
                /* Todo: Reconcile tracing differences (IDF-8183) */
                trace_వరుస_SEMAPHORE_RECEIVE( px_వరుస );

                /* Semaphores are queues with a data size of zero and where the
                 * messages waiting is the semaphore's count.  Reduce the count. */
                px_వరుస->ux_Messages_నిరీక్షణ = ux_Semaphore_లెక్క - ( UBaseType_t ) 1;

                #ఒకవేళ ( configUSE_MUTEXES == 1 )
                {
                    ఒకవేళ( px_వరుస->ux_వరుస_Type == వరుస_వరుస_IS_MUTEX )
                    {
                        /* Record the information required to implement
                         * priority inheritance should it become necessary. */
                        px_వరుస->u.xSemaphore.xMutexHolder = pv_Task_Increment_Mutex_Held_లెక్క();
                    }
                    లేకపోతే
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #అయితే_ముగింపు /* configUSE_MUTEXES */

                /* Check to see if other tasks are blocked waiting to give the
                 * semaphore, and if so, unblock the highest priority such task. */
                ఒకవేళ( list_LIST_IS_ఖాళీలు( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Send ) ) == pdFALSE )
                {
                    ఒకవేళ( xTaskRemoveFromEventList( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Send ) ) != pdFALSE )
                    {
                        వరుస_YIELD_IF_USING_PREEMPTION();
                    }
                    లేకపోతే
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                లేకపోతే
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                taskEXIT_CRITICAL( &( px_వరుస->x_వరుస_తాళం ) );
                తిరిగి pdPASS;
            }
            లేకపోతే
            {
                ఒకవేళ( x_Ticks_గమ్యం_Wait == ( TickType_t ) 0 )
                {
                    /* The semaphore count was 0 and no block time is specified
                     * (or the block time has expired) so exit now. */
                    taskEXIT_CRITICAL( &( px_వరుస->x_వరుస_తాళం ) );
                    trace_వరుస_RECEIVE_FAILED( px_వరుస );
                    తిరిగి err_వరుస_ఖాళీలు;
                }
                లేకపోతే ఒకవేళ( x_నమోదు_సమయం_సమితి == pdFALSE )
                {
                    /* The semaphore count was 0 and a block time was specified
                     * so configure the timeout structure ready to block. */
                    v_Task_అంతర్గత_సమితి_సమయం_బహిర్గతం_State( &x_సమయం_బహిర్గతం_2 );
                    x_నమోదు_సమయం_సమితి = pdTRUE;
                }
                లేకపోతే
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
            #ఒకవేళ ( వరుస_USE_LOCKS == 0 )
            {
                /* Update the timeout state to see if it has expired yet. */
                ఒకవేళ( x_Task_Check_For_సమయం_బహిర్గతం( &x_సమయం_బహిర్గతం_2, &x_Ticks_గమ్యం_Wait ) == pdFALSE )
                {
                    /* Not timed out yet. If this is a mutex, make the holder
                     * inherit our priority, then block the current task. */
                    trace_BLOCKING_ON_వరుస_RECEIVE( px_వరుస );
                    #ఒకవేళ ( configUSE_MUTEXES == 1 )
                    {
                        ఒకవేళ( px_వరుస->ux_వరుస_Type == వరుస_వరుస_IS_MUTEX )
                        {
                            xInheritanceOccurred = x_Task_ప్రాధాన్యత_Inherit( px_వరుస->u.xSemaphore.xMutexHolder );
                        }
                        లేకపోతే
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    #అయితే_ముగింపు /* if ( configUSE_MUTEXES == 1 ) */
                    v_Task_ఉంచు_On_Event_List( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Receive ), x_Ticks_గమ్యం_Wait );
                    పోర్టు_YIELD_WITHIN_API();
                }
                లేకపోతే
                {
                    /* We have timed out. If this is a mutex, make the holder
                     * disinherit our priority, then return an error. */
                    #ఒకవేళ ( configUSE_MUTEXES == 1 )
                    {
                        ఒకవేళ( xInheritanceOccurred != pdFALSE )
                        {
                            UBaseType_t ux_Highest_నిరీక్షణ_ప్రాధాన్యత;
                            ux_Highest_నిరీక్షణ_ప్రాధాన్యత = prv_Get_Disinherit_ప్రాధాన్యత_After_Timeout( px_వరుస );
                            v_Task_ప్రాధాన్యత_Disinherit_After_Timeout( px_వరుస->u.xSemaphore.xMutexHolder, ux_Highest_నిరీక్షణ_ప్రాధాన్యత );
                        }
                    }
                    #అయితే_ముగింపు /* configUSE_MUTEXES */
                    taskEXIT_CRITICAL( &( px_వరుస->x_వరుస_తాళం ) );
                    trace_వరుస_RECEIVE_FAILED( px_వరుస );
                    తిరిగి err_వరుస_ఖాళీలు;
                }
            }
            #అయితే_ముగింపు /* queueUSE_LOCKS == 0 */
        }
        taskEXIT_CRITICAL( &( px_వరుస->x_వరుస_తాళం ) );

        /* If queue locks ARE being used:
         * - At this point, the semaphore/mutex is empty/held and entry time
         *   has been set.
         * - We follow the original procedure for locking the queue, inheriting
         *   priority, then attempting to block. */
        #ఒకవేళ ( వరుస_USE_LOCKS == 1 )
        {
            /* Interrupts and other tasks can give to and take from the semaphore
             * now the critical section has been exited. */

            vTaskSuspendAll();
            prv_తాళం_వరుస( px_వరుస );

            /* Update the timeout state to see if it has expired yet. */
            ఒకవేళ( x_Task_Check_For_సమయం_బహిర్గతం( &x_సమయం_బహిర్గతం_2, &x_Ticks_గమ్యం_Wait ) == pdFALSE )
            {
                /* A block time is specified and not expired.  If the semaphore
                 * count is 0 then enter the Blocked state to wait for a semaphore to
                 * become available.  As semaphores are implemented with queues the
                 * queue being empty is equivalent to the semaphore count being 0. */
                ఒకవేళ( prv_Is_వరుస_ఖాళీలు( px_వరుస ) != pdFALSE )
                {
                    trace_BLOCKING_ON_వరుస_RECEIVE( px_వరుస );

                    #ఒకవేళ ( configUSE_MUTEXES == 1 )
                    {
                        ఒకవేళ( px_వరుస->ux_వరుస_Type == వరుస_వరుస_IS_MUTEX )
                        {
                            taskENTER_CRITICAL( &( px_వరుస->x_వరుస_తాళం ) );
                            {
                                xInheritanceOccurred = x_Task_ప్రాధాన్యత_Inherit( px_వరుస->u.xSemaphore.xMutexHolder );
                            }
                            taskEXIT_CRITICAL( &( px_వరుస->x_వరుస_తాళం ) );
                        }
                        లేకపోతే
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    #అయితే_ముగింపు /* if ( configUSE_MUTEXES == 1 ) */

                    v_Task_ఉంచు_On_Event_List( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Receive ), x_Ticks_గమ్యం_Wait );
                    prv_Unlock_వరుస( px_వరుస );

                    ఒకవేళ( xTaskResumeAll() == pdFALSE )
                    {
                        పోర్టు_YIELD_WITHIN_API();
                    }
                    లేకపోతే
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                లేకపోతే
                {
                    /* There was no timeout and the semaphore count was not 0, so
                     * attempt to take the semaphore again. */
                    prv_Unlock_వరుస( px_వరుస );
                    ( శూన్య ) xTaskResumeAll();
                }
            }
            లేకపోతే
            {
                /* Timed out. */
                prv_Unlock_వరుస( px_వరుస );
                ( శూన్య ) xTaskResumeAll();

                /* If the semaphore count is 0 exit now as the timeout has
                 * expired.  Otherwise return to attempt to take the semaphore that is
                 * known to be available.  As semaphores are implemented by queues the
                 * queue being empty is equivalent to the semaphore count being 0. */
                ఒకవేళ( prv_Is_వరుస_ఖాళీలు( px_వరుస ) != pdFALSE )
                {
                    #ఒకవేళ ( configUSE_MUTEXES == 1 )
                    {
                        /* xInheritanceOccurred could only have be set if
                         * pxQueue->uxQueueType == queueQUEUE_IS_MUTEX so no need to
                         * test the mutex type again to check it is actually a mutex. */
                        ఒకవేళ( xInheritanceOccurred != pdFALSE )
                        {
                            taskENTER_CRITICAL( &( px_వరుస->x_వరుస_తాళం ) );
                            {
                                UBaseType_t ux_Highest_నిరీక్షణ_ప్రాధాన్యత;

                                /* This task blocking on the mutex caused another
                                 * task to inherit this task's priority.  Now this task
                                 * has timed out the priority should be disinherited
                                 * again, but only as low as the next highest priority
                                 * task that is waiting for the same mutex. */
                                ux_Highest_నిరీక్షణ_ప్రాధాన్యత = prv_Get_Disinherit_ప్రాధాన్యత_After_Timeout( px_వరుస );
                                v_Task_ప్రాధాన్యత_Disinherit_After_Timeout( px_వరుస->u.xSemaphore.xMutexHolder, ux_Highest_నిరీక్షణ_ప్రాధాన్యత );
                            }
                            taskEXIT_CRITICAL( &( px_వరుస->x_వరుస_తాళం ) );
                        }
                    }
                    #అయితే_ముగింపు /* configUSE_MUTEXES */

                    trace_వరుస_RECEIVE_FAILED( px_వరుస );
                    తిరిగి err_వరుస_ఖాళీలు;
                }
                లేకపోతే
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
        }
        #అయితే_ముగింపు /* queueUSE_LOCKS == 1 */
    } /*lint -restore */
}
/*-----------------------------------------------------------*/

పునాది_Type_t x_వరుస_Peek( వరుస_Handle_t x_వరుస,
                       శూన్య * స్థిరరాశి pv_బఫరు,
                       TickType_t x_Ticks_గమ్యం_Wait )
{
    పునాది_Type_t x_నమోదు_సమయం_సమితి = pdFALSE;
    సమయం_బహిర్గతం_t x_సమయం_బహిర్గతం_2;
    int8_t * pcOriginalReadPosition;
    వరుస_t * స్థిరరాశి px_వరుస = x_వరుస;

    /* Check the pointer is not NULL. */
    configASSERT( ( px_వరుస ) );

    /* The buffer into which data is received can only be NULL if the data size
     * is zero (so no data is copied into the buffer. */
    configASSERT( !( ( ( pv_బఫరు ) == ఖాళీ ) && ( ( px_వరుస )->ux_అంశం_కొలత != ( UBaseType_t ) 0U ) ) );

    /* Cannot block if the scheduler is suspended. */
    #ఒకవేళ ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_గమ్యం_Wait != 0 ) ) );
    }
    #అయితే_ముగింపు

    /*lint -save -e904  This function relaxes the coding standard somewhat to
     * allow return statements within the function itself.  This is done in the
     * interest of execution time efficiency. */
    కొరకు( ; ; )
    {
        taskENTER_CRITICAL( &( px_వరుస->x_వరుస_తాళం ) );
        {
            స్థిరరాశి UBaseType_t ux_Messages_నిరీక్షణ = px_వరుస->ux_Messages_నిరీక్షణ;

            /* Is there data in the queue now?  To be running the calling task
             * must be the highest priority task wanting to access the queue. */
            ఒకవేళ( ux_Messages_నిరీక్షణ > ( UBaseType_t ) 0 )
            {
                /* Remember the read position so it can be reset after the data
                 * is read from the queue as this function is only peeking the
                 * data, not removing it. */
                pcOriginalReadPosition = px_వరుస->u.x_వరుస.pcReadFrom;

                prv_Copy_దత్తాంశం_From_వరుస( px_వరుస, pv_బఫరు );
                trace_వరుస_PEEK( px_వరుస );

                /* The data is not being removed, so reset the read pointer. */
                px_వరుస->u.x_వరుస.pcReadFrom = pcOriginalReadPosition;

                /* The data is being left in the queue, so see if there are
                 * any other tasks waiting for the data. */
                ఒకవేళ( list_LIST_IS_ఖాళీలు( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Receive ) ) == pdFALSE )
                {
                    ఒకవేళ( xTaskRemoveFromEventList( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Receive ) ) != pdFALSE )
                    {
                        /* The task waiting has a higher priority than this task. */
                        వరుస_YIELD_IF_USING_PREEMPTION();
                    }
                    లేకపోతే
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                లేకపోతే
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                taskEXIT_CRITICAL( &( px_వరుస->x_వరుస_తాళం ) );
                తిరిగి pdPASS;
            }
            లేకపోతే
            {
                ఒకవేళ( x_Ticks_గమ్యం_Wait == ( TickType_t ) 0 )
                {
                    /* The queue was empty and no block time is specified (or
                     * the block time has expired) so leave now. */
                    taskEXIT_CRITICAL( &( px_వరుస->x_వరుస_తాళం ) );
                    trace_వరుస_PEEK_FAILED( px_వరుస );
                    తిరిగి err_వరుస_ఖాళీలు;
                }
                లేకపోతే ఒకవేళ( x_నమోదు_సమయం_సమితి == pdFALSE )
                {
                    /* The queue was empty and a block time was specified so
                     * configure the timeout structure ready to enter the blocked
                     * state. */
                    v_Task_అంతర్గత_సమితి_సమయం_బహిర్గతం_State( &x_సమయం_బహిర్గతం_2 );
                    x_నమోదు_సమయం_సమితి = pdTRUE;
                }
                లేకపోతే
                {
                    /* Entry time was already set. */
                    mtCOVERAGE_TEST_MARKER();
                }
            }

            /* If queue locks ARE NOT being used:
             * - At this point, the queue is empty and entry time has been set
             * - We simply check for a time out, block if not timed out, or
             *   return an error if we have timed out. */
            #ఒకవేళ ( వరుస_USE_LOCKS == 0 )
            {
                /* Update the timeout state to see if it has expired yet. */
                ఒకవేళ( x_Task_Check_For_సమయం_బహిర్గతం( &x_సమయం_బహిర్గతం_2, &x_Ticks_గమ్యం_Wait ) == pdFALSE )
                {
                    /* Not timed out yet. Block the current task. */
                    trace_BLOCKING_ON_వరుస_PEEK( px_వరుస );
                    v_Task_ఉంచు_On_Event_List( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Receive ), x_Ticks_గమ్యం_Wait );
                    పోర్టు_YIELD_WITHIN_API();
                }
                లేకపోతే
                {
                    /* We have timed out. Return an error. */
                    taskEXIT_CRITICAL( &( px_వరుస->x_వరుస_తాళం ) );
                    trace_వరుస_PEEK_FAILED( px_వరుస );
                    తిరిగి err_వరుస_ఖాళీలు;
                }
            }
            #అయితే_ముగింపు /* queueUSE_LOCKS == 0 */
        }
        taskEXIT_CRITICAL( &( px_వరుస->x_వరుస_తాళం ) );

        /* If queue locks ARE being used:
         * - At this point, the queue is empty and entry time has been set
         * - We follow the original procedure for locking the queue before
         *   attempting to block. */
        #ఒకవేళ ( వరుస_USE_LOCKS == 1 )
        {
            /* Interrupts and other tasks can send to and receive from the queue
             * now that the critical section has been exited. */

            vTaskSuspendAll();
            prv_తాళం_వరుస( px_వరుస );

            /* Update the timeout state to see if it has expired yet. */
            ఒకవేళ( x_Task_Check_For_సమయం_బహిర్గతం( &x_సమయం_బహిర్గతం_2, &x_Ticks_గమ్యం_Wait ) == pdFALSE )
            {
                /* Timeout has not expired yet, check to see if there is data in the
                * queue now, and if not enter the Blocked state to wait for data. */
                ఒకవేళ( prv_Is_వరుస_ఖాళీలు( px_వరుస ) != pdFALSE )
                {
                    trace_BLOCKING_ON_వరుస_PEEK( px_వరుస );
                    v_Task_ఉంచు_On_Event_List( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Receive ), x_Ticks_గమ్యం_Wait );
                    prv_Unlock_వరుస( px_వరుస );

                    ఒకవేళ( xTaskResumeAll() == pdFALSE )
                    {
                        పోర్టు_YIELD_WITHIN_API();
                    }
                    లేకపోతే
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                లేకపోతే
                {
                    /* There is data in the queue now, so don't enter the blocked
                     * state, instead return to try and obtain the data. */
                    prv_Unlock_వరుస( px_వరుస );
                    ( శూన్య ) xTaskResumeAll();
                }
            }
            లేకపోతే
            {
                /* The timeout has expired.  If there is still no data in the queue
                 * exit, otherwise go back and try to read the data again. */
                prv_Unlock_వరుస( px_వరుస );
                ( శూన్య ) xTaskResumeAll();

                ఒకవేళ( prv_Is_వరుస_ఖాళీలు( px_వరుస ) != pdFALSE )
                {
                    trace_వరుస_PEEK_FAILED( px_వరుస );
                    తిరిగి err_వరుస_ఖాళీలు;
                }
                లేకపోతే
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
        }
        #అయితే_ముగింపు /* queueUSE_LOCKS == 1 */
    } /*lint -restore */
}
/*-----------------------------------------------------------*/

పునాది_Type_t x_వరుస_Receive_From_ISR( వరుస_Handle_t x_వరుస,
                                 శూన్య * స్థిరరాశి pv_బఫరు,
                                 పునాది_Type_t * స్థిరరాశి px_Higher_ప్రాధాన్యత_Task_Woken )
{
    పునాది_Type_t xReturn;
    UBaseType_t ux_Saved_అంతరాయం_స్థితి;
    వరుస_t * స్థిరరాశి px_వరుస = x_వరుస;

    configASSERT( px_వరుస );
    configASSERT( !( ( pv_బఫరు == ఖాళీ ) && ( px_వరుస->ux_అంశం_కొలత != ( UBaseType_t ) 0U ) ) );

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
    పోర్టు_ASSERT_IF_అంతరాయం_ప్రాధాన్యత_INVALID();

    prv_ENTER_CRITICAL_OR_ముసుగు_ISR( &( px_వరుస->x_వరుస_తాళం ), ux_Saved_అంతరాయం_స్థితి );
    {
        స్థిరరాశి UBaseType_t ux_Messages_నిరీక్షణ = px_వరుస->ux_Messages_నిరీక్షణ;

        /* Cannot block in an ISR, so check there is data available. */
        ఒకవేళ( ux_Messages_నిరీక్షణ > ( UBaseType_t ) 0 )
        {
            #ఒకవేళ ( వరుస_USE_LOCKS == 1 )
                స్థిరరాశి int8_t c_Rx_తాళం = px_వరుస->c_Rx_తాళం;
            #లేకపోతే
                /* Queue locks not used, so we treat it as unlocked. */
                స్థిరరాశి int8_t c_Rx_తాళం = వరుస_UNLOCKED;
            #అయితే_ముగింపు /* queueUSE_LOCKS == 1 */

            trace_వరుస_RECEIVE_FROM_ISR( px_వరుస );

            prv_Copy_దత్తాంశం_From_వరుస( px_వరుస, pv_బఫరు );
            px_వరుస->ux_Messages_నిరీక్షణ = ux_Messages_నిరీక్షణ - ( UBaseType_t ) 1;

            /* If the queue is locked the event list will not be modified.
             * Instead update the lock count so the task that unlocks the queue
             * will know that an ISR has removed data while the queue was
             * locked. */
            ఒకవేళ( c_Rx_తాళం == వరుస_UNLOCKED )
            {
                ఒకవేళ( list_LIST_IS_ఖాళీలు( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Send ) ) == pdFALSE )
                {
                    ఒకవేళ( xTaskRemoveFromEventList( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Send ) ) != pdFALSE )
                    {
                        /* The task waiting has a higher priority than us so
                         * force a context switch. */
                        ఒకవేళ( px_Higher_ప్రాధాన్యత_Task_Woken != ఖాళీ )
                        {
                            *px_Higher_ప్రాధాన్యత_Task_Woken = pdTRUE;
                        }
                        లేకపోతే
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    లేకపోతే
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                లేకపోతే
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            లేకపోతే
            {
                #ఒకవేళ ( వరుస_USE_LOCKS == 1 )
                {
                    /* Increment the lock count so the task that unlocks the queue
                     * knows that data was removed while it was locked. */
                    prv_Increment_వరుస_Rx_తాళం( px_వరుస, c_Rx_తాళం );
                }
                #అయితే_ముగింపు /* queueUSE_LOCKS == 1 */
            }

            xReturn = pdPASS;
        }
        లేకపోతే
        {
            xReturn = pdFAIL;
            trace_వరుస_RECEIVE_FROM_ISR_FAILED( px_వరుస );
        }
    }
    prvEXIT_CRITICAL_OR_UNMASK_ISR( &( px_వరుస->x_వరుస_తాళం ), ux_Saved_అంతరాయం_స్థితి );

    తిరిగి xReturn;
}
/*-----------------------------------------------------------*/

పునాది_Type_t x_వరుస_Peek_From_ISR( వరుస_Handle_t x_వరుస,
                              శూన్య * స్థిరరాశి pv_బఫరు )
{
    పునాది_Type_t xReturn;
    UBaseType_t ux_Saved_అంతరాయం_స్థితి;
    int8_t * pcOriginalReadPosition;
    వరుస_t * స్థిరరాశి px_వరుస = x_వరుస;

    configASSERT( px_వరుస );
    configASSERT( !( ( pv_బఫరు == ఖాళీ ) && ( px_వరుస->ux_అంశం_కొలత != ( UBaseType_t ) 0U ) ) );
    configASSERT( px_వరుస->ux_అంశం_కొలత != 0 ); /* Can't peek a semaphore. */

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
    పోర్టు_ASSERT_IF_అంతరాయం_ప్రాధాన్యత_INVALID();

    prv_ENTER_CRITICAL_OR_ముసుగు_ISR( &( px_వరుస->x_వరుస_తాళం ), ux_Saved_అంతరాయం_స్థితి );
    {
        /* Cannot block in an ISR, so check there is data available. */
        ఒకవేళ( px_వరుస->ux_Messages_నిరీక్షణ > ( UBaseType_t ) 0 )
        {
            trace_వరుస_PEEK_FROM_ISR( px_వరుస );

            /* Remember the read position so it can be reset as nothing is
             * actually being removed from the queue. */
            pcOriginalReadPosition = px_వరుస->u.x_వరుస.pcReadFrom;
            prv_Copy_దత్తాంశం_From_వరుస( px_వరుస, pv_బఫరు );
            px_వరుస->u.x_వరుస.pcReadFrom = pcOriginalReadPosition;

            xReturn = pdPASS;
        }
        లేకపోతే
        {
            xReturn = pdFAIL;
            trace_వరుస_PEEK_FROM_ISR_FAILED( px_వరుస );
        }
    }
    prvEXIT_CRITICAL_OR_UNMASK_ISR( &( px_వరుస->x_వరుస_తాళం ), ux_Saved_అంతరాయం_స్థితి );

    తిరిగి xReturn;
}
/*-----------------------------------------------------------*/

UBaseType_t ux_వరుస_Messages_నిరీక్షణ( స్థిరరాశి వరుస_Handle_t x_వరుస )
{
    UBaseType_t uxReturn;

    configASSERT( x_వరుస );

    taskENTER_CRITICAL( &( ( ( వరుస_t * ) x_వరుస )->x_వరుస_తాళం ) );
    {
        uxReturn = ( ( వరుస_t * ) x_వరుస )->ux_Messages_నిరీక్షణ;
    }
    taskEXIT_CRITICAL( &( ( ( వరుస_t * ) x_వరుస )->x_వరుస_తాళం ) );

    తిరిగి uxReturn;
} /*lint !e818 Pointer cannot be declared const as xQueue is a typedef not pointer. */
/*-----------------------------------------------------------*/

UBaseType_t ux_వరుస_Spaces_అందుబాటు( స్థిరరాశి వరుస_Handle_t x_వరుస )
{
    UBaseType_t uxReturn;
    వరుస_t * స్థిరరాశి px_వరుస = x_వరుస;

    configASSERT( px_వరుస );

    taskENTER_CRITICAL( &( px_వరుస->x_వరుస_తాళం ) );
    {
        uxReturn = px_వరుస->ux_నిడివి - px_వరుస->ux_Messages_నిరీక్షణ;
    }
    taskEXIT_CRITICAL( &( px_వరుస->x_వరుస_తాళం ) );

    తిరిగి uxReturn;
} /*lint !e818 Pointer cannot be declared const as xQueue is a typedef not pointer. */
/*-----------------------------------------------------------*/

UBaseType_t ux_వరుస_Messages_నిరీక్షణ_From_ISR( స్థిరరాశి వరుస_Handle_t x_వరుస )
{
    UBaseType_t uxReturn;
    వరుస_t * స్థిరరాశి px_వరుస = x_వరుస;

    configASSERT( px_వరుస );
    uxReturn = px_వరుస->ux_Messages_నిరీక్షణ;

    తిరిగి uxReturn;
} /*lint !e818 Pointer cannot be declared const as xQueue is a typedef not pointer. */
/*-----------------------------------------------------------*/

శూన్య v_వరుస_Delete( వరుస_Handle_t x_వరుస )
{
    వరుస_t * స్థిరరాశి px_వరుస = x_వరుస;

    configASSERT( px_వరుస );
    trace_వరుస_DELETE( px_వరుస );

    #ఒకవేళ ( config_వరుస_REGISTRY_కొలత > 0 )
    {
        v_వరుస_Unregister_వరుస( px_వరుస );
    }
    #అయితే_ముగింపు

    #ఒకవేళ ( ( config_SUPPORT_DYNAMIC_కేటాయింపు == 1 ) && ( config_SUPPORT_STATIC_కేటాయింపు == 0 ) )
    {
        /* The queue can only have been allocated dynamically - free it
         * again. */
        v_పోర్టు_Free( px_వరుస );
    }
    #లేదావేళ ( ( config_SUPPORT_DYNAMIC_కేటాయింపు == 1 ) && ( config_SUPPORT_STATIC_కేటాయింపు == 1 ) )
    {
        /* The queue could have been allocated statically or dynamically, so
         * check before attempting to free the memory. */
        ఒకవేళ( px_వరుస->ucStaticallyAllocated == ( uint8_t ) pdFALSE )
        {
            v_పోర్టు_Free( px_వరుస );
        }
        లేకపోతే
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    #లేకపోతే /* if ( ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) && ( configSUPPORT_STATIC_ALLOCATION == 0 ) ) */
    {
        /* The queue must have been statically allocated, so is not going to be
         * deleted.  Avoid compiler warnings about the unused parameter. */
        ( శూన్య ) px_వరుస;
    }
    #అయితే_ముగింపు /* configSUPPORT_DYNAMIC_ALLOCATION */
}
/*-----------------------------------------------------------*/

#ఒకవేళ ( configUSE_TRACE_FACILITY == 1 )

    UBaseType_t ux_వరుస_Get_వరుస_సంఖ్య( వరుస_Handle_t x_వరుస )
    {
        తిరిగి ( ( వరుస_t * ) x_వరుస )->ux_వరుస_సంఖ్య;
    }

#అయితే_ముగింపు /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#ఒకవేళ ( configUSE_TRACE_FACILITY == 1 )

    శూన్య v_వరుస_సమితి_వరుస_సంఖ్య( వరుస_Handle_t x_వరుస,
                               UBaseType_t ux_వరుస_సంఖ్య )
    {
        ( ( వరుస_t * ) x_వరుస )->ux_వరుస_సంఖ్య = ux_వరుస_సంఖ్య;
    }

#అయితే_ముగింపు /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#ఒకవేళ ( configUSE_TRACE_FACILITY == 1 )

    uint8_t uc_వరుస_Get_వరుస_Type( వరుస_Handle_t x_వరుస )
    {
        తిరిగి ( ( వరుస_t * ) x_వరుస )->uc_వరుస_Type;
    }

#అయితే_ముగింపు /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#ఒకవేళ ( configUSE_MUTEXES == 1 )

    స్థిర UBaseType_t prv_Get_Disinherit_ప్రాధాన్యత_After_Timeout( స్థిరరాశి వరుస_t * స్థిరరాశి px_వరుస )
    {
        UBaseType_t ux_Highest_ప్రాధాన్యత_యొక్క_నిరీక్షణ_Tasks;

        /* If a task waiting for a mutex causes the mutex holder to inherit a
         * priority, but the waiting task times out, then the holder should
         * disinherit the priority - but only down to the highest priority of any
         * other tasks that are waiting for the same mutex.  For this purpose,
         * return the priority of the highest priority task that is waiting for the
         * mutex. */
        ఒకవేళ( list_CURRENT_LIST_నిడివి( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Receive ) ) > 0U )
        {
            ux_Highest_ప్రాధాన్యత_యొక్క_నిరీక్షణ_Tasks = ( UBaseType_t ) configMAX_PRIORITIES - ( UBaseType_t ) list_GET_అంశం_విలువ_యొక్క_తల_నమోదు( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Receive ) );
        }
        లేకపోతే
        {
            ux_Highest_ప్రాధాన్యత_యొక్క_నిరీక్షణ_Tasks = tsk_IDLE_ప్రాధాన్యత;
        }

        తిరిగి ux_Highest_ప్రాధాన్యత_యొక్క_నిరీక్షణ_Tasks;
    }

#అయితే_ముగింపు /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

స్థిర పునాది_Type_t prv_Copy_దత్తాంశం_గమ్యం_వరుస( వరుస_t * స్థిరరాశి px_వరుస,
                                      స్థిరరాశి శూన్య * pv_అంశం_గమ్యం_వరుస,
                                      స్థిరరాశి పునాది_Type_t xPosition )
{
    పునాది_Type_t xReturn = pdFALSE;
    UBaseType_t ux_Messages_నిరీక్షణ;

    /* This function is called from a critical section. */

    ux_Messages_నిరీక్షణ = px_వరుస->ux_Messages_నిరీక్షణ;

    ఒకవేళ( px_వరుస->ux_అంశం_కొలత == ( UBaseType_t ) 0 )
    {
        #ఒకవేళ ( configUSE_MUTEXES == 1 )
        {
            ఒకవేళ( px_వరుస->ux_వరుస_Type == వరుస_వరుస_IS_MUTEX )
            {
                /* The mutex is no longer being held. */
                xReturn = x_Task_ప్రాధాన్యత_Disinherit( px_వరుస->u.xSemaphore.xMutexHolder );
                px_వరుస->u.xSemaphore.xMutexHolder = ఖాళీ;
            }
            లేకపోతే
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        #అయితే_ముగింపు /* configUSE_MUTEXES */
    }
    లేకపోతే ఒకవేళ( xPosition == వరుస_SEND_గమ్యం_వెనుక )
    {
        ( శూన్య ) జ్ఞాపకం_నకలు( ( శూన్య * ) px_వరుస->pc_Write_గమ్యం, pv_అంశం_గమ్యం_వరుస, ( size_t ) px_వరుస->ux_అంశం_కొలత ); /*lint !e961 !e418 !e9087 MISRA exception as the casts are only redundant for some ports, plus previous logic ensures a null pointer can only be passed to memcpy() if the copy size is 0.  Cast to void required by function signature and safe as no alignment requirement and copy length specified in bytes. */
        px_వరుస->pc_Write_గమ్యం += px_వరుస->ux_అంశం_కొలత;                                                       /*lint !e9016 Pointer arithmetic on char types ok, especially in this use case where it is the clearest way of conveying intent. */

        ఒకవేళ( px_వరుస->pc_Write_గమ్యం >= px_వరుస->u.x_వరుస.pc_తోక )                                             /*lint !e946 MISRA exception justified as comparison of pointers is the cleanest solution. */
        {
            px_వరుస->pc_Write_గమ్యం = px_వరుస->pc_తల;
        }
        లేకపోతే
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    లేకపోతే
    {
        ( శూన్య ) జ్ఞాపకం_నకలు( ( శూన్య * ) px_వరుస->u.x_వరుస.pcReadFrom, pv_అంశం_గమ్యం_వరుస, ( size_t ) px_వరుస->ux_అంశం_కొలత ); /*lint !e961 !e9087 !e418 MISRA exception as the casts are only redundant for some ports.  Cast to void required by function signature and safe as no alignment requirement and copy length specified in bytes.  Assert checks null pointer only used when length is 0. */
        px_వరుస->u.x_వరుస.pcReadFrom -= px_వరుస->ux_అంశం_కొలత;

        ఒకవేళ( px_వరుస->u.x_వరుస.pcReadFrom < px_వరుస->pc_తల ) /*lint !e946 MISRA exception justified as comparison of pointers is the cleanest solution. */
        {
            px_వరుస->u.x_వరుస.pcReadFrom = ( px_వరుస->u.x_వరుస.pc_తోక - px_వరుస->ux_అంశం_కొలత );
        }
        లేకపోతే
        {
            mtCOVERAGE_TEST_MARKER();
        }

        ఒకవేళ( xPosition == వరుస_OVERWRITE )
        {
            ఒకవేళ( ux_Messages_నిరీక్షణ > ( UBaseType_t ) 0 )
            {
                /* An item is not being added but overwritten, so subtract
                 * one from the recorded number of items in the queue so when
                 * one is added again below the number of recorded items remains
                 * correct. */
                --ux_Messages_నిరీక్షణ;
            }
            లేకపోతే
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        లేకపోతే
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }

    px_వరుస->ux_Messages_నిరీక్షణ = ux_Messages_నిరీక్షణ + ( UBaseType_t ) 1;

    తిరిగి xReturn;
}
/*-----------------------------------------------------------*/

స్థిర శూన్య prv_Copy_దత్తాంశం_From_వరుస( వరుస_t * స్థిరరాశి px_వరుస,
                                  శూన్య * స్థిరరాశి pv_బఫరు )
{
    ఒకవేళ( px_వరుస->ux_అంశం_కొలత != ( UBaseType_t ) 0 )
    {
        px_వరుస->u.x_వరుస.pcReadFrom += px_వరుస->ux_అంశం_కొలత;           /*lint !e9016 Pointer arithmetic on char types ok, especially in this use case where it is the clearest way of conveying intent. */

        ఒకవేళ( px_వరుస->u.x_వరుస.pcReadFrom >= px_వరుస->u.x_వరుస.pc_తోక ) /*lint !e946 MISRA exception justified as use of the relational operator is the cleanest solutions. */
        {
            px_వరుస->u.x_వరుస.pcReadFrom = px_వరుస->pc_తల;
        }
        లేకపోతే
        {
            mtCOVERAGE_TEST_MARKER();
        }

        ( శూన్య ) జ్ఞాపకం_నకలు( ( శూన్య * ) pv_బఫరు, ( శూన్య * ) px_వరుస->u.x_వరుస.pcReadFrom, ( size_t ) px_వరుస->ux_అంశం_కొలత ); /*lint !e961 !e418 !e9087 MISRA exception as the casts are only redundant for some ports.  Also previous logic ensures a null pointer can only be passed to memcpy() when the count is 0.  Cast to void required by function signature and safe as no alignment requirement and copy length specified in bytes. */
    }
}
/*-----------------------------------------------------------*/

#ఒకవేళ ( వరుస_USE_LOCKS == 1 )
    స్థిర శూన్య prv_Unlock_వరుస( వరుస_t * స్థిరరాశి px_వరుస )
    {
        /* THIS FUNCTION MUST BE CALLED WITH THE SCHEDULER SUSPENDED. */

        /* The lock counts contains the number of extra data items placed or
         * removed from the queue while the queue was locked.  When a queue is
         * locked items can be added or removed, but the event lists cannot be
         * updated. */
        taskENTER_CRITICAL( &( px_వరుస->x_వరుస_తాళం ) );
        {
            int8_t c_Tx_తాళం = px_వరుస->c_Tx_తాళం;

            /* See if data was added to the queue while it was locked. */
            అయితే( c_Tx_తాళం > వరుస_LOCKED_UNMODIFIED )
            {
                /* Data was posted while the queue was locked.  Are any tasks
                 * blocked waiting for data to become available? */
                #ఒకవేళ ( config_USE_వరుస_SETS == 1 )
                {
                    ఒకవేళ( px_వరుస->px_వరుస_సమితి_Container != ఖాళీ )
                    {
                        ఒకవేళ( prv_Notify_వరుస_సమితి_Container( px_వరుస ) != pdFALSE )
                        {
                            /* The queue is a member of a queue set, and posting to
                             * the queue set caused a higher priority task to unblock.
                             * A context switch is required. */
                            vTaskMissedYield();
                        }
                        లేకపోతే
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    లేకపోతే
                    {
                        /* Tasks that are removed from the event list will get
                         * added to the pending ready list as the scheduler is still
                         * suspended. */
                        ఒకవేళ( list_LIST_IS_ఖాళీలు( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Receive ) ) == pdFALSE )
                        {
                            ఒకవేళ( xTaskRemoveFromEventList( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Receive ) ) != pdFALSE )
                            {
                                /* The task waiting has a higher priority so record that a
                                 * context switch is required. */
                                vTaskMissedYield();
                            }
                            లేకపోతే
                            {
                                mtCOVERAGE_TEST_MARKER();
                            }
                        }
                        లేకపోతే
                        {
                            ఆపు;
                        }
                    }
                }
                #లేకపోతే /* configUSE_QUEUE_SETS */
                {
                    /* Tasks that are removed from the event list will get added to
                     * the pending ready list as the scheduler is still suspended. */
                    ఒకవేళ( list_LIST_IS_ఖాళీలు( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Receive ) ) == pdFALSE )
                    {
                        ఒకవేళ( xTaskRemoveFromEventList( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Receive ) ) != pdFALSE )
                        {
                            /* The task waiting has a higher priority so record that
                             * a context switch is required. */
                            vTaskMissedYield();
                        }
                        లేకపోతే
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    లేకపోతే
                    {
                        ఆపు;
                    }
                }
                #అయితే_ముగింపు /* configUSE_QUEUE_SETS */

                --c_Tx_తాళం;
            }

            px_వరుస->c_Tx_తాళం = వరుస_UNLOCKED;
        }
        taskEXIT_CRITICAL( &( px_వరుస->x_వరుస_తాళం ) );

        /* Do the same for the Rx lock. */
        taskENTER_CRITICAL( &( px_వరుస->x_వరుస_తాళం ) );
        {
            int8_t c_Rx_తాళం = px_వరుస->c_Rx_తాళం;

            అయితే( c_Rx_తాళం > వరుస_LOCKED_UNMODIFIED )
            {
                ఒకవేళ( list_LIST_IS_ఖాళీలు( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Send ) ) == pdFALSE )
                {
                    ఒకవేళ( xTaskRemoveFromEventList( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Send ) ) != pdFALSE )
                    {
                        vTaskMissedYield();
                    }
                    లేకపోతే
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }

                    --c_Rx_తాళం;
                }
                లేకపోతే
                {
                    ఆపు;
                }
            }

            px_వరుస->c_Rx_తాళం = వరుస_UNLOCKED;
        }
        taskEXIT_CRITICAL( &( px_వరుస->x_వరుస_తాళం ) );
    }
#అయితే_ముగింపు /* queueUSE_LOCKS == 1 */
/*-----------------------------------------------------------*/

#ఒకవేళ ( వరుస_USE_LOCKS == 1 )
    స్థిర పునాది_Type_t prv_Is_వరుస_ఖాళీలు( స్థిరరాశి వరుస_t * px_వరుస )
    {
        పునాది_Type_t xReturn;

        taskENTER_CRITICAL( &( ( ( వరుస_t * ) px_వరుస )->x_వరుస_తాళం ) );
        {
            ఒకవేళ( px_వరుస->ux_Messages_నిరీక్షణ == ( UBaseType_t ) 0 )
            {
                xReturn = pdTRUE;
            }
            లేకపోతే
            {
                xReturn = pdFALSE;
            }
        }
        taskEXIT_CRITICAL( &( ( ( వరుస_t * ) px_వరుస )->x_వరుస_తాళం ) );

        తిరిగి xReturn;
    }
#అయితే_ముగింపు /* queueUSE_LOCKS == 1 */
/*-----------------------------------------------------------*/

పునాది_Type_t x_వరుస_Is_వరుస_ఖాళీలు_From_ISR( స్థిరరాశి వరుస_Handle_t x_వరుస )
{
    పునాది_Type_t xReturn;
    వరుస_t * స్థిరరాశి px_వరుస = x_వరుస;

    configASSERT( px_వరుస );

    ఒకవేళ( px_వరుస->ux_Messages_నిరీక్షణ == ( UBaseType_t ) 0 )
    {
        xReturn = pdTRUE;
    }
    లేకపోతే
    {
        xReturn = pdFALSE;
    }

    తిరిగి xReturn;
} /*lint !e818 xQueue could not be pointer to const because it is a typedef. */
/*-----------------------------------------------------------*/

#ఒకవేళ ( వరుస_USE_LOCKS == 1 )
    స్థిర పునాది_Type_t prv_Is_వరుస_నిండిన( స్థిరరాశి వరుస_t * px_వరుస )
    {
        పునాది_Type_t xReturn;

        taskENTER_CRITICAL( &( ( ( వరుస_t * ) px_వరుస )->x_వరుస_తాళం ) );
        {
            ఒకవేళ( px_వరుస->ux_Messages_నిరీక్షణ == px_వరుస->ux_నిడివి )
            {
                xReturn = pdTRUE;
            }
            లేకపోతే
            {
                xReturn = pdFALSE;
            }
        }
        taskEXIT_CRITICAL( &( ( ( వరుస_t * ) px_వరుస )->x_వరుస_తాళం ) );

        తిరిగి xReturn;
    }
#అయితే_ముగింపు /* queueUSE_LOCKS == 1 */
/*-----------------------------------------------------------*/

పునాది_Type_t x_వరుస_Is_వరుస_నిండిన_From_ISR( స్థిరరాశి వరుస_Handle_t x_వరుస )
{
    పునాది_Type_t xReturn;
    వరుస_t * స్థిరరాశి px_వరుస = x_వరుస;

    configASSERT( px_వరుస );

    ఒకవేళ( px_వరుస->ux_Messages_నిరీక్షణ == px_వరుస->ux_నిడివి )
    {
        xReturn = pdTRUE;
    }
    లేకపోతే
    {
        xReturn = pdFALSE;
    }

    తిరిగి xReturn;
} /*lint !e818 xQueue could not be pointer to const because it is a typedef. */
/*-----------------------------------------------------------*/

#ఒకవేళ ( configUSE_CO_ROUTINES == 1 )

    పునాది_Type_t x_వరుస_CRSend( వరుస_Handle_t x_వరుస,
                             స్థిరరాశి శూన్య * pv_అంశం_గమ్యం_వరుస,
                             TickType_t x_Ticks_గమ్యం_Wait )
    {
        పునాది_Type_t xReturn;
        వరుస_t * స్థిరరాశి px_వరుస = x_వరుస;

        /* If the queue is already full we may have to block.  A critical section
         * is required to prevent an interrupt removing something from the queue
         * between the check to see if the queue is full and blocking on the queue. */
        పోర్టు_DISABLE_INTERRUPTS();
        {
            ఒకవేళ( prv_Is_వరుస_నిండిన( px_వరుస ) != pdFALSE )
            {
                /* The queue is full - do we want to block or just leave without
                 * posting? */
                ఒకవేళ( x_Ticks_గమ్యం_Wait > ( TickType_t ) 0 )
                {
                    /* As this is called from a coroutine we cannot block directly, but
                     * return indicating that we need to block. */
                    v_Co_Routine_కూడు_గమ్యం_Delayed_List( x_Ticks_గమ్యం_Wait, &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Send ) );
                    పోర్టు_ENABLE_INTERRUPTS();
                    తిరిగి err_వరుస_BLOCKED;
                }
                లేకపోతే
                {
                    పోర్టు_ENABLE_INTERRUPTS();
                    తిరిగి err_వరుస_నిండిన;
                }
            }
        }
        పోర్టు_ENABLE_INTERRUPTS();

        పోర్టు_DISABLE_INTERRUPTS();
        {
            ఒకవేళ( px_వరుస->ux_Messages_నిరీక్షణ < px_వరుస->ux_నిడివి )
            {
                /* There is room in the queue, copy the data into the queue. */
                prv_Copy_దత్తాంశం_గమ్యం_వరుస( px_వరుస, pv_అంశం_గమ్యం_వరుస, వరుస_SEND_గమ్యం_వెనుక );
                xReturn = pdPASS;

                /* Were any co-routines waiting for data to become available? */
                ఒకవేళ( list_LIST_IS_ఖాళీలు( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Receive ) ) == pdFALSE )
                {
                    /* In this instance the co-routine could be placed directly
                     * into the ready list as we are within a critical section.
                     * Instead the same pending ready list mechanism is used as if
                     * the event were caused from within an interrupt. */
                    ఒకవేళ( xCoRoutineRemoveFromEventList( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Receive ) ) != pdFALSE )
                    {
                        /* The co-routine waiting has a higher priority so record
                         * that a yield might be appropriate. */
                        xReturn = err_వరుస_YIELD;
                    }
                    లేకపోతే
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                లేకపోతే
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            లేకపోతే
            {
                xReturn = err_వరుస_నిండిన;
            }
        }
        పోర్టు_ENABLE_INTERRUPTS();

        తిరిగి xReturn;
    }

#అయితే_ముగింపు /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#ఒకవేళ ( configUSE_CO_ROUTINES == 1 )

    పునాది_Type_t x_వరుస_CRReceive( వరుస_Handle_t x_వరుస,
                                శూన్య * pv_బఫరు,
                                TickType_t x_Ticks_గమ్యం_Wait )
    {
        పునాది_Type_t xReturn;
        వరుస_t * స్థిరరాశి px_వరుస = x_వరుస;

        /* If the queue is already empty we may have to block.  A critical section
         * is required to prevent an interrupt adding something to the queue
         * between the check to see if the queue is empty and blocking on the queue. */
        పోర్టు_DISABLE_INTERRUPTS();
        {
            ఒకవేళ( px_వరుస->ux_Messages_నిరీక్షణ == ( UBaseType_t ) 0 )
            {
                /* There are no messages in the queue, do we want to block or just
                 * leave with nothing? */
                ఒకవేళ( x_Ticks_గమ్యం_Wait > ( TickType_t ) 0 )
                {
                    /* As this is a co-routine we cannot block directly, but return
                     * indicating that we need to block. */
                    v_Co_Routine_కూడు_గమ్యం_Delayed_List( x_Ticks_గమ్యం_Wait, &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Receive ) );
                    పోర్టు_ENABLE_INTERRUPTS();
                    తిరిగి err_వరుస_BLOCKED;
                }
                లేకపోతే
                {
                    పోర్టు_ENABLE_INTERRUPTS();
                    తిరిగి err_వరుస_నిండిన;
                }
            }
            లేకపోతే
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        పోర్టు_ENABLE_INTERRUPTS();

        పోర్టు_DISABLE_INTERRUPTS();
        {
            ఒకవేళ( px_వరుస->ux_Messages_నిరీక్షణ > ( UBaseType_t ) 0 )
            {
                /* Data is available from the queue. */
                px_వరుస->u.x_వరుస.pcReadFrom += px_వరుస->ux_అంశం_కొలత;

                ఒకవేళ( px_వరుస->u.x_వరుస.pcReadFrom >= px_వరుస->u.x_వరుస.pc_తోక )
                {
                    px_వరుస->u.x_వరుస.pcReadFrom = px_వరుస->pc_తల;
                }
                లేకపోతే
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                --( px_వరుస->ux_Messages_నిరీక్షణ );
                ( శూన్య ) జ్ఞాపకం_నకలు( ( శూన్య * ) pv_బఫరు, ( శూన్య * ) px_వరుస->u.x_వరుస.pcReadFrom, ( గుర్తులేని ) px_వరుస->ux_అంశం_కొలత );

                xReturn = pdPASS;

                /* Were any co-routines waiting for space to become available? */
                ఒకవేళ( list_LIST_IS_ఖాళీలు( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Send ) ) == pdFALSE )
                {
                    /* In this instance the co-routine could be placed directly
                     * into the ready list as we are within a critical section.
                     * Instead the same pending ready list mechanism is used as if
                     * the event were caused from within an interrupt. */
                    ఒకవేళ( xCoRoutineRemoveFromEventList( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Send ) ) != pdFALSE )
                    {
                        xReturn = err_వరుస_YIELD;
                    }
                    లేకపోతే
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                లేకపోతే
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            లేకపోతే
            {
                xReturn = pdFAIL;
            }
        }
        పోర్టు_ENABLE_INTERRUPTS();

        తిరిగి xReturn;
    }

#అయితే_ముగింపు /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#ఒకవేళ ( configUSE_CO_ROUTINES == 1 )

    పునాది_Type_t x_వరుస_CRSend_From_ISR( వరుస_Handle_t x_వరుస,
                                    స్థిరరాశి శూన్య * pv_అంశం_గమ్యం_వరుస,
                                    పునాది_Type_t xCoRoutinePreviouslyWoken )
    {
        వరుస_t * స్థిరరాశి px_వరుస = x_వరుస;

        /* Cannot block within an ISR so if there is no space on the queue then
         * exit without doing anything. */
        ఒకవేళ( px_వరుస->ux_Messages_నిరీక్షణ < px_వరుస->ux_నిడివి )
        {
            prv_Copy_దత్తాంశం_గమ్యం_వరుస( px_వరుస, pv_అంశం_గమ్యం_వరుస, వరుస_SEND_గమ్యం_వెనుక );

            /* We only want to wake one co-routine per ISR, so check that a
             * co-routine has not already been woken. */
            ఒకవేళ( xCoRoutinePreviouslyWoken == pdFALSE )
            {
                ఒకవేళ( list_LIST_IS_ఖాళీలు( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Receive ) ) == pdFALSE )
                {
                    ఒకవేళ( xCoRoutineRemoveFromEventList( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Receive ) ) != pdFALSE )
                    {
                        తిరిగి pdTRUE;
                    }
                    లేకపోతే
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                లేకపోతే
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            లేకపోతే
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        లేకపోతే
        {
            mtCOVERAGE_TEST_MARKER();
        }

        తిరిగి xCoRoutinePreviouslyWoken;
    }

#అయితే_ముగింపు /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#ఒకవేళ ( configUSE_CO_ROUTINES == 1 )

    పునాది_Type_t x_వరుస_CRReceive_From_ISR( వరుస_Handle_t x_వరుస,
                                       శూన్య * pv_బఫరు,
                                       పునాది_Type_t * pxCoRoutineWoken )
    {
        పునాది_Type_t xReturn;
        వరుస_t * స్థిరరాశి px_వరుస = x_వరుస;

        /* We cannot block from an ISR, so check there is data available. If
         * not then just leave without doing anything. */
        ఒకవేళ( px_వరుస->ux_Messages_నిరీక్షణ > ( UBaseType_t ) 0 )
        {
            /* Copy the data from the queue. */
            px_వరుస->u.x_వరుస.pcReadFrom += px_వరుస->ux_అంశం_కొలత;

            ఒకవేళ( px_వరుస->u.x_వరుస.pcReadFrom >= px_వరుస->u.x_వరుస.pc_తోక )
            {
                px_వరుస->u.x_వరుస.pcReadFrom = px_వరుస->pc_తల;
            }
            లేకపోతే
            {
                mtCOVERAGE_TEST_MARKER();
            }

            --( px_వరుస->ux_Messages_నిరీక్షణ );
            ( శూన్య ) జ్ఞాపకం_నకలు( ( శూన్య * ) pv_బఫరు, ( శూన్య * ) px_వరుస->u.x_వరుస.pcReadFrom, ( గుర్తులేని ) px_వరుస->ux_అంశం_కొలత );

            ఒకవేళ( ( *pxCoRoutineWoken ) == pdFALSE )
            {
                ఒకవేళ( list_LIST_IS_ఖాళీలు( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Send ) ) == pdFALSE )
                {
                    ఒకవేళ( xCoRoutineRemoveFromEventList( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Send ) ) != pdFALSE )
                    {
                        *pxCoRoutineWoken = pdTRUE;
                    }
                    లేకపోతే
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                లేకపోతే
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            లేకపోతే
            {
                mtCOVERAGE_TEST_MARKER();
            }

            xReturn = pdPASS;
        }
        లేకపోతే
        {
            xReturn = pdFAIL;
        }

        తిరిగి xReturn;
    }

#అయితే_ముగింపు /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#ఒకవేళ ( config_వరుస_REGISTRY_కొలత > 0 )

    శూన్య v_వరుస_కూడు_గమ్యం_Registry( వరుస_Handle_t x_వరుస,
                              స్థిరరాశి అక్షరం * pc_వరుస_Name ) /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
    {
        UBaseType_t ux;
        వరుస_Registry_అంశం_t * px_నమోదు_గమ్యం_Write = ఖాళీ;

        configASSERT( x_వరుస );

        /* For SMP, we need to take the queue registry lock in case another
         * core updates the register simultaneously. */
        prvENTER_CRITICAL_SMP_ONLY( &x_వరుస_Registry_తాళం );
        {
            ఒకవేళ( pc_వరుస_Name != ఖాళీ )
            {
                /* See if there is an empty space in the registry.  A NULL name denotes
                 * a free slot. */
                కొరకు( ux = ( UBaseType_t ) 0U; ux < ( UBaseType_t ) config_వరుస_REGISTRY_కొలత; ux++ )
                {
                    /* Replace an existing entry if the queue is already in the registry. */
                    ఒకవేళ( x_వరుస == x_వరుస_Registry[ ux ].xHandle )
                    {
                        px_నమోదు_గమ్యం_Write = &( x_వరుస_Registry[ ux ] );
                        ఆపు;
                    }
                    /* Otherwise, store in the next empty location */
                    లేకపోతే ఒకవేళ( ( px_నమోదు_గమ్యం_Write == ఖాళీ ) && ( x_వరుస_Registry[ ux ].pc_వరుస_Name == ఖాళీ ) )
                    {
                        px_నమోదు_గమ్యం_Write = &( x_వరుస_Registry[ ux ] );
                    }
                    లేకపోతే
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
            }

            ఒకవేళ( px_నమోదు_గమ్యం_Write != ఖాళీ )
            {
                /* Store the information on this queue. */
                px_నమోదు_గమ్యం_Write->pc_వరుస_Name = pc_వరుస_Name;
                px_నమోదు_గమ్యం_Write->xHandle = x_వరుస;

                trace_వరుస_REGISTRY_కూడు( x_వరుస, pc_వరుస_Name );
            }
        }
        /* Release the previously taken queue registry lock. */
        prvEXIT_CRITICAL_SMP_ONLY( &x_వరుస_Registry_తాళం );
    }

#అయితే_ముగింపు /* configQUEUE_REGISTRY_SIZE */
/*-----------------------------------------------------------*/

#ఒకవేళ ( config_వరుస_REGISTRY_కొలత > 0 )

    స్థిరరాశి అక్షరం * pc_వరుస_Get_Name( వరుస_Handle_t x_వరుస ) /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
    {
        UBaseType_t ux;
        స్థిరరాశి అక్షరం * pcReturn = ఖాళీ; /*lint !e971 Unqualified char types are allowed for strings and single characters only. */

        configASSERT( x_వరుస );

        /* For SMP, we need to take the queue registry lock in case another
         * core updates the register simultaneously. */
        prvENTER_CRITICAL_SMP_ONLY( &x_వరుస_Registry_తాళం );
        {
            /* Note there is nothing here to protect against another task adding or
             * removing entries from the registry while it is being searched. */

            కొరకు( ux = ( UBaseType_t ) 0U; ux < ( UBaseType_t ) config_వరుస_REGISTRY_కొలత; ux++ )
            {
                ఒకవేళ( x_వరుస_Registry[ ux ].xHandle == x_వరుస )
                {
                    pcReturn = x_వరుస_Registry[ ux ].pc_వరుస_Name;
                    ఆపు;
                }
                లేకపోతే
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
        }
        /* Release the previously taken queue registry lock. */
        prvEXIT_CRITICAL_SMP_ONLY( &x_వరుస_Registry_తాళం );

        తిరిగి pcReturn;
    } /*lint !e818 xQueue cannot be a pointer to const because it is a typedef. */

#అయితే_ముగింపు /* configQUEUE_REGISTRY_SIZE */
/*-----------------------------------------------------------*/

#ఒకవేళ ( config_వరుస_REGISTRY_కొలత > 0 )

    శూన్య v_వరుస_Unregister_వరుస( వరుస_Handle_t x_వరుస )
    {
        UBaseType_t ux;

        configASSERT( x_వరుస );

        /* For SMP, we need to take the queue registry lock in case another
         * core updates the register simultaneously. */
        prvENTER_CRITICAL_SMP_ONLY( &x_వరుస_Registry_తాళం );
        {
            /* See if the handle of the queue being unregistered in actually in the
             * registry. */
            కొరకు( ux = ( UBaseType_t ) 0U; ux < ( UBaseType_t ) config_వరుస_REGISTRY_కొలత; ux++ )
            {
                ఒకవేళ( x_వరుస_Registry[ ux ].xHandle == x_వరుస )
                {
                    /* Set the name to NULL to show that this slot if free again. */
                    x_వరుస_Registry[ ux ].pc_వరుస_Name = ఖాళీ;

                    /* Set the handle to NULL to ensure the same queue handle cannot
                     * appear in the registry twice if it is added, removed, then
                     * added again. */
                    x_వరుస_Registry[ ux ].xHandle = ( వరుస_Handle_t ) 0;
                    ఆపు;
                }
                లేకపోతే
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
        }
        /* Release the previously taken queue registry lock. */
        prvEXIT_CRITICAL_SMP_ONLY( &x_వరుస_Registry_తాళం );
    } /*lint !e818 xQueue could not be pointer to const because it is a typedef. */

#అయితే_ముగింపు /* configQUEUE_REGISTRY_SIZE */
/*-----------------------------------------------------------*/

#ఒకవేళ ( configUSE_TIMERS == 1 )

    శూన్య v_వరుస_Wait_For_సందేశం_Restricted( వరుస_Handle_t x_వరుస,
                                         TickType_t x_Ticks_గమ్యం_Wait,
                                         స్థిరరాశి పునాది_Type_t xWaitIndefinitely )
    {
        వరుస_t * స్థిరరాశి px_వరుస = x_వరుస;

        /* This function should not be called by application code hence the
         * 'Restricted' in its name.  It is not part of the public API.  It is
         * designed for use by kernel code, and has special calling requirements.
         * It can result in vListInsert() being called on a list that can only
         * possibly ever have one item in it, so the list will be fast, but even
         * so it should be called with the scheduler locked and not from a critical
         * section. */

        /* For SMP, we need to take the queue's xQueueLock as we are about to
         * access the queue. */
        prvENTER_CRITICAL_SMP_ONLY( &( px_వరుస->x_వరుస_తాళం ) );
        {
            #ఒకవేళ ( వరుస_USE_LOCKS == 1 )
            {
                /* Only do anything if there are no messages in the queue.  This function
                 *  will not actually cause the task to block, just place it on a blocked
                 *  list.  It will not block until the scheduler is unlocked - at which
                 *  time a yield will be performed.  If an item is added to the queue while
                 *  the queue is locked, and the calling task blocks on the queue, then the
                 *  calling task will be immediately unblocked when the queue is unlocked. */
                prv_తాళం_వరుస( px_వరుస );
            }
            #అయితే_ముగింపు /* queueUSE_LOCKS == 1 */

            ఒకవేళ( px_వరుస->ux_Messages_నిరీక్షణ == ( UBaseType_t ) 0U )
            {
                /* There is nothing in the queue, block for the specified period. */
                v_Task_ఉంచు_On_Event_List_Restricted( &( px_వరుస->x_Tasks_నిరీక్షణ_గమ్యం_Receive ), x_Ticks_గమ్యం_Wait, xWaitIndefinitely );
            }
            లేకపోతే
            {
                mtCOVERAGE_TEST_MARKER();
            }

            #ఒకవేళ ( వరుస_USE_LOCKS == 1 )
            {
                prv_Unlock_వరుస( px_వరుస );
            }
            #అయితే_ముగింపు /* queueUSE_LOCKS == 1 */
        }
        /* Release the previously taken xQueueLock. */
        prvEXIT_CRITICAL_SMP_ONLY( &( px_వరుస->x_వరుస_తాళం ) );
    }

#అయితే_ముగింపు /* configUSE_TIMERS */
/*-----------------------------------------------------------*/

#ఒకవేళ ( ( config_USE_వరుస_SETS == 1 ) && ( config_SUPPORT_DYNAMIC_కేటాయింపు == 1 ) )

    వరుస_సమితి_Handle_t x_వరుస_Create_సమితి( స్థిరరాశి UBaseType_t ux_Event_వరుస_నిడివి )
    {
        వరుస_సమితి_Handle_t px_వరుస;

        px_వరుస = x_వరుస_Generic_Create( ux_Event_వరుస_నిడివి, ( UBaseType_t ) పరిమాణం( వరుస_t * ), వరుస_వరుస_TYPE_సమితి );

        తిరిగి px_వరుస;
    }

#అయితే_ముగింపు /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#ఒకవేళ ( config_USE_వరుస_SETS == 1 )

    పునాది_Type_t x_వరుస_కూడు_గమ్యం_సమితి( వరుస_సమితి_Member_Handle_t x_వరుస_Or_Semaphore,
                               వరుస_సమితి_Handle_t x_వరుస_సమితి )
    {
        పునాది_Type_t xReturn;

        taskENTER_CRITICAL( &( ( ( వరుస_t * ) x_వరుస_Or_Semaphore )->x_వరుస_తాళం ) );
        {
            ఒకవేళ( ( ( వరుస_t * ) x_వరుస_Or_Semaphore )->px_వరుస_సమితి_Container != ఖాళీ )
            {
                /* Cannot add a queue/semaphore to more than one queue set. */
                xReturn = pdFAIL;
            }
            లేకపోతే ఒకవేళ( ( ( వరుస_t * ) x_వరుస_Or_Semaphore )->ux_Messages_నిరీక్షణ != ( UBaseType_t ) 0 )
            {
                /* Cannot add a queue/semaphore to a queue set if there are already
                 * items in the queue/semaphore. */
                xReturn = pdFAIL;
            }
            లేకపోతే
            {
                ( ( వరుస_t * ) x_వరుస_Or_Semaphore )->px_వరుస_సమితి_Container = x_వరుస_సమితి;
                xReturn = pdPASS;
            }
        }
        taskEXIT_CRITICAL( &( ( ( వరుస_t * ) x_వరుస_Or_Semaphore )->x_వరుస_తాళం ) );

        తిరిగి xReturn;
    }

#అయితే_ముగింపు /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#ఒకవేళ ( config_USE_వరుస_SETS == 1 )

    పునాది_Type_t x_వరుస_Remove_From_సమితి( వరుస_సమితి_Member_Handle_t x_వరుస_Or_Semaphore,
                                    వరుస_సమితి_Handle_t x_వరుస_సమితి )
    {
        పునాది_Type_t xReturn;
        వరుస_t * స్థిరరాశి px_వరుస_Or_Semaphore = ( వరుస_t * ) x_వరుస_Or_Semaphore;

        ఒకవేళ( px_వరుస_Or_Semaphore->px_వరుస_సమితి_Container != x_వరుస_సమితి )
        {
            /* The queue was not a member of the set. */
            xReturn = pdFAIL;
        }
        లేకపోతే ఒకవేళ( px_వరుస_Or_Semaphore->ux_Messages_నిరీక్షణ != ( UBaseType_t ) 0 )
        {
            /* It is dangerous to remove a queue from a set when the queue is
             * not empty because the queue set will still hold pending events for
             * the queue. */
            xReturn = pdFAIL;
        }
        లేకపోతే
        {
            taskENTER_CRITICAL( &( ( ( వరుస_t * ) px_వరుస_Or_Semaphore )->x_వరుస_తాళం ) );
            {
                /* The queue is no longer contained in the set. */
                px_వరుస_Or_Semaphore->px_వరుస_సమితి_Container = ఖాళీ;
            }
            taskEXIT_CRITICAL( &( ( ( వరుస_t * ) px_వరుస_Or_Semaphore )->x_వరుస_తాళం ) );
            xReturn = pdPASS;
        }

        తిరిగి xReturn;
    } /*lint !e818 xQueueSet could not be declared as pointing to const as it is a typedef. */

#అయితే_ముగింపు /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#ఒకవేళ ( config_USE_వరుస_SETS == 1 )

    వరుస_సమితి_Member_Handle_t x_వరుస_Select_From_సమితి( వరుస_సమితి_Handle_t x_వరుస_సమితి,
                                                TickType_t స్థిరరాశి x_Ticks_గమ్యం_Wait )
    {
        వరుస_సమితి_Member_Handle_t xReturn = ఖాళీ;

        ( శూన్య ) x_వరుస_Receive( ( వరుస_Handle_t ) x_వరుస_సమితి, &xReturn, x_Ticks_గమ్యం_Wait ); /*lint !e961 Casting from one typedef to another is not redundant. */
        తిరిగి xReturn;
    }

#అయితే_ముగింపు /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#ఒకవేళ ( config_USE_వరుస_SETS == 1 )

    వరుస_సమితి_Member_Handle_t x_వరుస_Select_From_సమితి_From_ISR( వరుస_సమితి_Handle_t x_వరుస_సమితి )
    {
        వరుస_సమితి_Member_Handle_t xReturn = ఖాళీ;

        ( శూన్య ) x_వరుస_Receive_From_ISR( ( వరుస_Handle_t ) x_వరుస_సమితి, &xReturn, ఖాళీ ); /*lint !e961 Casting from one typedef to another is not redundant. */
        తిరిగి xReturn;
    }

#అయితే_ముగింపు /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#ఒకవేళ ( config_USE_వరుస_SETS == 1 )

    స్థిర పునాది_Type_t prv_Notify_వరుస_సమితి_Container( స్థిరరాశి వరుస_t * స్థిరరాశి px_వరుస )
    {
        వరుస_t * px_వరుస_సమితి_Container = px_వరుస->px_వరుస_సమితి_Container;
        పునాది_Type_t xReturn = pdFALSE;

        /* This function must be called form a critical section. */

        /* The following line is not reachable in unit tests because every call
         * to prvNotifyQueueSetContainer is preceded by a check that
         * pxQueueSetContainer != NULL */
        configASSERT( px_వరుస_సమితి_Container ); /* LCOV_EXCL_BR_LINE */
        configASSERT( px_వరుస_సమితి_Container->ux_Messages_నిరీక్షణ < px_వరుస_సమితి_Container->ux_నిడివి );

        /* In SMP, queue sets have their own xQueueLock. Thus we need to also
         * acquire the queue set's xQueueLock before accessing it. */
        prv_ENTER_CRITICAL_సురక్షిత_SMP_ONLY( &( px_వరుస_సమితి_Container->x_వరుస_తాళం ) );
        {
            ఒకవేళ( px_వరుస_సమితి_Container->ux_Messages_నిరీక్షణ < px_వరుస_సమితి_Container->ux_నిడివి )
            {
                #ఒకవేళ ( వరుస_USE_LOCKS == 1 )
                    స్థిరరాశి int8_t c_Tx_తాళం = px_వరుస_సమితి_Container->c_Tx_తాళం;
                #లేకపోతే
                    /* Queue locks not used, so we treat it as unlocked. */
                    స్థిరరాశి int8_t c_Tx_తాళం = వరుస_UNLOCKED;
                #అయితే_ముగింపు /* queueUSE_LOCKS == 1 */

                trace_వరుస_సమితి_SEND( px_వరుస_సమితి_Container );

                /* The data copied is the handle of the queue that contains data. */
                xReturn = prv_Copy_దత్తాంశం_గమ్యం_వరుస( px_వరుస_సమితి_Container, &px_వరుస, వరుస_SEND_గమ్యం_వెనుక );

                ఒకవేళ( c_Tx_తాళం == వరుస_UNLOCKED )
                {
                    ఒకవేళ( list_LIST_IS_ఖాళీలు( &( px_వరుస_సమితి_Container->x_Tasks_నిరీక్షణ_గమ్యం_Receive ) ) == pdFALSE )
                    {
                        ఒకవేళ( xTaskRemoveFromEventList( &( px_వరుస_సమితి_Container->x_Tasks_నిరీక్షణ_గమ్యం_Receive ) ) != pdFALSE )
                        {
                            /* The task waiting has a higher priority. */
                            xReturn = pdTRUE;
                        }
                        లేకపోతే
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    లేకపోతే
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                లేకపోతే
                {
                    #ఒకవేళ ( వరుస_USE_LOCKS == 1 )
                    {
                        prv_Increment_వరుస_Tx_తాళం( px_వరుస_సమితి_Container, c_Tx_తాళం );
                    }
                    #అయితే_ముగింపు /* queueUSE_LOCKS == 1 */
                }
            }
            లేకపోతే
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        /* Release the previously acquired queue set's xQueueLock. */
        prv_EXIT_CRITICAL_సురక్షిత_SMP_ONLY( &( px_వరుస_సమితి_Container->x_వరుస_తాళం ) );

        తిరిగి xReturn;
    }

#అయితే_ముగింపు /* configUSE_QUEUE_SETS */
