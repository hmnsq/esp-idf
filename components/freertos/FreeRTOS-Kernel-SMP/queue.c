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

#شامل <stdlib.h>
#شامل <string.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#تعریف_کرو MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#شامل "FreeRTOS.h"
#شامل "task.h"
#شامل "queue.h"

#اگر ( configUSE_CO_ROUTINES == 1 )
    #شامل "croutine.h"
#اگر_ختم

/* The MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#تعریف_منسوخ MPU_WRAPPERS_INCLUDED_FROM_API_FILE


/* Constants used with the cRxLock and cTxLock structure members. */
#تعریف_کرو قطار_UNLOCKED             ( ( int8_t ) -1 )
#تعریف_کرو قطار_LOCKED_UNMODIFIED    ( ( int8_t ) 0 )
#تعریف_کرو قطار_INT8_MAX             ( ( int8_t ) 127 )

/* When the Queue_t structure is used to represent a base queue its pcHead and
 * pcTail members are used as pointers into the queue storage area.  When the
 * Queue_t structure is used to represent a mutex pcHead and pcTail pointers are
 * not necessary, and the pcHead pointer is set to NULL to indicate that the
 * structure instead holds a pointer to the mutex holder (if any).  Map alternative
 * names to the pcHead and structure member to ensure the readability of the code
 * is maintained.  The QueuePointers_t and SemaphoreData_t types are used to form
 * a union as their usage is mutually exclusive dependent on what the queue is
 * being used for. */
#تعریف_کرو ux_قطار_Type               pc_سر
#تعریف_کرو قطار_قطار_IS_MUTEX       تہی

قسمتعریف ساخت قطار_اشارے
{
    int8_t * pc_دم;     /**< Points to the byte at the end of the queue storage area.  Once more byte is allocated than necessary to store the queue items, this is used as a marker. */
    int8_t * pcReadFrom; /**< Points to the last place that a queued item was read from when the structure is used as a queue. */
} قطار_اشارے_t;

قسمتعریف ساخت Semaphore_ڈیٹا
{
    TaskHandle_t xMutexHolder;        /**< The handle of the task that holds the mutex. */
    UBaseType_t ux_Recursive_Call_گنتی; /**< Maintains a count of the number of times a recursive mutex has been recursively 'taken' when the structure is used as a mutex. */
} Semaphore_ڈیٹا_t;

/* Semaphores do not actually store or copy data, so have an item size of
 * zero. */
#تعریف_کرو قطار_SEMAPHORE_قطار_شے_لمبائی    ( ( UBaseType_t ) 0 )
#تعریف_کرو قطار_MUTEX_GIVE_بلاک_وقت          ( ( TickType_t ) 0U )

#اگر ( configUSE_PREEMPTION == 0 )

/* If the cooperative scheduler is being used then a yield should not be
 * performed just because a higher priority task has been woken. */
    #تعریف_کرو قطار_YIELD_IF_USING_PREEMPTION()
#ورنہ
    #اگر ( config_نمبر_کا_CORES == 1 )
        #تعریف_کرو قطار_YIELD_IF_USING_PREEMPTION()    پورٹ_YIELD_WITHIN_API()
    #ورنہ /* #if ( configNUMBER_OF_CORES == 1 ) */
        #تعریف_کرو قطار_YIELD_IF_USING_PREEMPTION()    vTaskYieldWithinAPI()
    #اگر_ختم /* #if ( configNUMBER_OF_CORES == 1 ) */
#اگر_ختم

/*
 * Definition of the queue used by the scheduler.
 * Items are queued by copy, not reference.  See the following link for the
 * rationale: https://www.FreeRTOS.org/Embedded-RTOS-Queues.html
 */
قسمتعریف ساخت قطار_Definition /* The old naming convention is used to prevent breaking kernel aware debuggers. */
{
    int8_t * pc_سر;           /**< Points to the beginning of the queue storage area. */
    int8_t * pc_Write_مقصد;        /**< Points to the free next place in the storage area. */

    اتحاد
    {
        قطار_اشارے_t x_قطار;     /**< Data required exclusively when this structure is used as a queue. */
        Semaphore_ڈیٹا_t xSemaphore; /**< Data required exclusively when this structure is used as a semaphore. */
    } u;

    List_t x_Tasks_انتظار_مقصد_Send;             /**< List of tasks that are blocked waiting to post onto this queue.  Stored in priority order. */
    List_t x_Tasks_انتظار_مقصد_Receive;          /**< List of tasks that are blocked waiting to read from this queue.  Stored in priority order. */

    متلون UBaseType_t ux_Messages_انتظار; /**< The number of items currently in the queue. */
    UBaseType_t ux_لمبائی;                   /**< The length of the queue defined as the number of items it will hold, not the number of bytes. */
    UBaseType_t ux_شے_سائز;                 /**< The size of each items that the queue will hold. */

    متلون int8_t c_Rx_تالا;                /**< Stores the number of items received from the queue (removed from the queue) while the queue was locked.  Set to queueUNLOCKED when the queue is not locked. */
    متلون int8_t c_Tx_تالا;                /**< Stores the number of items transmitted to the queue (added to the queue) while the queue was locked.  Set to queueUNLOCKED when the queue is not locked. */

    #اگر ( ( config_SUPPORT_STATIC_تخصیص == 1 ) && ( config_SUPPORT_DYNAMIC_تخصیص == 1 ) )
        uint8_t ucStaticallyAllocated; /**< Set to pdTRUE if the memory used by the queue was statically allocated to ensure no attempt is made to free the memory. */
    #اگر_ختم

    #اگر ( config_USE_قطار_SETS == 1 )
        ساخت قطار_Definition * px_قطار_سیٹ_Container;
    #اگر_ختم

    #اگر ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_قطار_نمبر;
        uint8_t uc_قطار_Type;
    #اگر_ختم
} x_قطار_2;

/* The old xQUEUE name is maintained above then typedefed to the new Queue_t
 * name below to enable the use of older kernel aware debuggers. */
قسمتعریف x_قطار_2 قطار_t;

/*-----------------------------------------------------------*/

/*
 * The queue registry is just a means for kernel aware debuggers to locate
 * queue structures.  It has no other purpose so is an optional component.
 */
#اگر ( config_قطار_REGISTRY_سائز > 0 )

/* The type stored within the queue registry array.  This allows a name
 * to be assigned to each queue making kernel aware debugging a little
 * more user friendly. */
    قسمتعریف ساخت قطار_REGISTRY_شے
    {
        ثابت حرف * pc_قطار_Name;
        قطار_Handle_t xHandle;
    } x_قطار_Registry_شے;

/* The old xQueueRegistryItem name is maintained above then typedefed to the
 * new xQueueRegistryItem name below to enable the use of older kernel aware
 * debuggers. */
    قسمتعریف x_قطار_Registry_شے قطار_Registry_شے_t;

/* The queue registry is simply an array of QueueRegistryItem_t structures.
 * The pcQueueName member of a structure being NULL is indicative of the
 * array position being vacant. */

/* MISRA Ref 8.4.2 [Declaration shall be visible] */
/* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-84 */
/* coverity[misra_c_2012_rule_8_4_violation] */
    PRIVILEGED_ڈیٹا قطار_Registry_شے_t x_قطار_Registry[ config_قطار_REGISTRY_سائز ];

#اگر_ختم /* configQUEUE_REGISTRY_SIZE */

/*
 * Unlocks a queue locked by a call to prvLockQueue.  Locking a queue does not
 * prevent an ISR from adding or removing items to the queue, but does prevent
 * an ISR from removing tasks from the queue event lists.  If an ISR finds a
 * queue is locked it will instead increment the appropriate queue lock count
 * to indicate that a task may require unblocking.  When the queue in unlocked
 * these lock counts are inspected, and the appropriate action taken.
 */
ساکن خالی prv_Unlock_قطار( قطار_t * ثابت px_قطار ) PRIVILEGED_FUNCTION;

/*
 * Uses a critical section to determine if there is any data in a queue.
 *
 * @return pdTRUE if the queue contains no items, otherwise pdFALSE.
 */
ساکن بنیاد_Type_t prv_Is_قطار_خالی_خانے( ثابت قطار_t * px_قطار ) PRIVILEGED_FUNCTION;

/*
 * Uses a critical section to determine if there is any space in a queue.
 *
 * @return pdTRUE if there is no space, otherwise pdFALSE;
 */
ساکن بنیاد_Type_t prv_Is_قطار_بھرا( ثابت قطار_t * px_قطار ) PRIVILEGED_FUNCTION;

/*
 * Copies an item into the queue, either at the front of the queue or the
 * back of the queue.
 */
ساکن بنیاد_Type_t prv_Copy_ڈیٹا_مقصد_قطار( قطار_t * ثابت px_قطار,
                                      ثابت خالی * pv_شے_مقصد_قطار,
                                      ثابت بنیاد_Type_t xPosition ) PRIVILEGED_FUNCTION;

/*
 * Copies an item out of a queue.
 */
ساکن خالی prv_Copy_ڈیٹا_From_قطار( قطار_t * ثابت px_قطار,
                                  خالی * ثابت pv_بفر ) PRIVILEGED_FUNCTION;

#اگر ( config_USE_قطار_SETS == 1 )

/*
 * Checks to see if a queue is a member of a queue set, and if so, notifies
 * the queue set that the queue contains data.
 */
    ساکن بنیاد_Type_t prv_Notify_قطار_سیٹ_Container( ثابت قطار_t * ثابت px_قطار ) PRIVILEGED_FUNCTION;
#اگر_ختم

/*
 * Called after a Queue_t structure has been allocated either statically or
 * dynamically to fill in the structure's members.
 */
ساکن خالی prv_Initialise_New_قطار( ثابت UBaseType_t ux_قطار_لمبائی,
                                   ثابت UBaseType_t ux_شے_سائز,
                                   uint8_t * puc_قطار_Storage,
                                   ثابت uint8_t uc_قطار_Type,
                                   قطار_t * px_New_قطار ) PRIVILEGED_FUNCTION;

/*
 * Mutexes are a special type of queue.  When a mutex is created, first the
 * queue is created, then prvInitialiseMutex() is called to configure the queue
 * as a mutex.
 */
#اگر ( configUSE_MUTEXES == 1 )
    ساکن خالی prvInitialiseMutex( قطار_t * px_New_قطار ) PRIVILEGED_FUNCTION;
#اگر_ختم

#اگر ( configUSE_MUTEXES == 1 )

/*
 * If a task waiting for a mutex causes the mutex holder to inherit a
 * priority, but the waiting task times out, then the holder should
 * disinherit the priority - but only down to the highest priority of any
 * other tasks that are waiting for the same mutex.  This function returns
 * that priority.
 */
    ساکن UBaseType_t prv_Get_Disinherit_ترجیح_After_Timeout( ثابت قطار_t * ثابت px_قطار ) PRIVILEGED_FUNCTION;
#اگر_ختم
/*-----------------------------------------------------------*/

/*
 * Macro to mark a queue as locked.  Locking a queue prevents an ISR from
 * accessing the queue event lists.
 */
#تعریف_کرو prv_تالا_قطار( px_قطار )                            \
    taskENTER_CRITICAL();                                  \
    {                                                      \
        اگر( ( px_قطار )->c_Rx_تالا == قطار_UNLOCKED )        \
        {                                                  \
            ( px_قطار )->c_Rx_تالا = قطار_LOCKED_UNMODIFIED; \
        }                                                  \
        اگر( ( px_قطار )->c_Tx_تالا == قطار_UNLOCKED )        \
        {                                                  \
            ( px_قطار )->c_Tx_تالا = قطار_LOCKED_UNMODIFIED; \
        }                                                  \
    }                                                      \
    taskEXIT_CRITICAL()

/*
 * Macro to increment cTxLock member of the queue data structure. It is
 * capped at the number of tasks in the system as we cannot unblock more
 * tasks than the number of tasks in the system.
 */
#تعریف_کرو prv_Increment_قطار_Tx_تالا( px_قطار, c_Tx_تالا )                           \
    کرو {                                                                      \
        ثابت UBaseType_t ux_نمبر_کا_Tasks = ux_Task_Get_نمبر_کا_Tasks();         \
        اگر( ( UBaseType_t ) ( c_Tx_تالا ) < ux_نمبر_کا_Tasks )                   \
        {                                                                     \
            configASSERT( ( c_Tx_تالا ) != قطار_INT8_MAX );                     \
            ( px_قطار )->c_Tx_تالا = ( int8_t ) ( ( c_Tx_تالا ) + ( int8_t ) 1 ); \
        }                                                                     \
    } جب( 0 )

/*
 * Macro to increment cRxLock member of the queue data structure. It is
 * capped at the number of tasks in the system as we cannot unblock more
 * tasks than the number of tasks in the system.
 */
#تعریف_کرو prv_Increment_قطار_Rx_تالا( px_قطار, c_Rx_تالا )                           \
    کرو {                                                                      \
        ثابت UBaseType_t ux_نمبر_کا_Tasks = ux_Task_Get_نمبر_کا_Tasks();         \
        اگر( ( UBaseType_t ) ( c_Rx_تالا ) < ux_نمبر_کا_Tasks )                   \
        {                                                                     \
            configASSERT( ( c_Rx_تالا ) != قطار_INT8_MAX );                     \
            ( px_قطار )->c_Rx_تالا = ( int8_t ) ( ( c_Rx_تالا ) + ( int8_t ) 1 ); \
        }                                                                     \
    } جب( 0 )
/*-----------------------------------------------------------*/

بنیاد_Type_t x_قطار_Generic_Reset( قطار_Handle_t x_قطار,
                               بنیاد_Type_t x_New_قطار )
{
    بنیاد_Type_t xReturn = pdPASS;
    قطار_t * ثابت px_قطار = x_قطار;

    trace_ENTER_x_قطار_Generic_Reset( x_قطار, x_New_قطار );

    configASSERT( px_قطار );

    اگر( ( px_قطار != تہی ) &&
        ( px_قطار->ux_لمبائی >= 1U ) &&
        /* Check for multiplication overflow. */
        ( ( SIZE_MAX / px_قطار->ux_لمبائی ) >= px_قطار->ux_شے_سائز ) )
    {
        taskENTER_CRITICAL();
        {
            px_قطار->u.x_قطار.pc_دم = px_قطار->pc_سر + ( px_قطار->ux_لمبائی * px_قطار->ux_شے_سائز );
            px_قطار->ux_Messages_انتظار = ( UBaseType_t ) 0U;
            px_قطار->pc_Write_مقصد = px_قطار->pc_سر;
            px_قطار->u.x_قطار.pcReadFrom = px_قطار->pc_سر + ( ( px_قطار->ux_لمبائی - 1U ) * px_قطار->ux_شے_سائز );
            px_قطار->c_Rx_تالا = قطار_UNLOCKED;
            px_قطار->c_Tx_تالا = قطار_UNLOCKED;

            اگر( x_New_قطار == pdFALSE )
            {
                /* If there are tasks blocked waiting to read from the queue, then
                 * the tasks will remain blocked as after this function exits the queue
                 * will still be empty.  If there are tasks blocked waiting to write to
                 * the queue, then one should be unblocked as after this function exits
                 * it will be possible to write to it. */
                اگر( list_LIST_IS_خالی_خانے( &( px_قطار->x_Tasks_انتظار_مقصد_Send ) ) == pdFALSE )
                {
                    اگر( xTaskRemoveFromEventList( &( px_قطار->x_Tasks_انتظار_مقصد_Send ) ) != pdFALSE )
                    {
                        قطار_YIELD_IF_USING_PREEMPTION();
                    }
                    ورنہ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                ورنہ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            ورنہ
            {
                /* Ensure the event queues start in the correct state. */
                vListInitialise( &( px_قطار->x_Tasks_انتظار_مقصد_Send ) );
                vListInitialise( &( px_قطار->x_Tasks_انتظار_مقصد_Receive ) );
            }
        }
        taskEXIT_CRITICAL();
    }
    ورنہ
    {
        xReturn = pdFAIL;
    }

    configASSERT( xReturn != pdFAIL );

    /* A value is returned for calling semantic consistency with previous
     * versions. */
    trace_RETURN_x_قطار_Generic_Reset( xReturn );

    واپس xReturn;
}
/*-----------------------------------------------------------*/

#اگر ( config_SUPPORT_STATIC_تخصیص == 1 )

    قطار_Handle_t x_قطار_Generic_Create_Static( ثابت UBaseType_t ux_قطار_لمبائی,
                                             ثابت UBaseType_t ux_شے_سائز,
                                             uint8_t * puc_قطار_Storage,
                                             Static_قطار_t * px_Static_قطار,
                                             ثابت uint8_t uc_قطار_Type )
    {
        قطار_t * px_New_قطار = تہی;

        trace_ENTER_x_قطار_Generic_Create_Static( ux_قطار_لمبائی, ux_شے_سائز, puc_قطار_Storage, px_Static_قطار, uc_قطار_Type );

        /* The StaticQueue_t structure and the queue storage area must be
         * supplied. */
        configASSERT( px_Static_قطار );

        اگر( ( ux_قطار_لمبائی > ( UBaseType_t ) 0 ) &&
            ( px_Static_قطار != تہی ) &&

            /* A queue storage area should be provided if the item size is not 0, and
             * should not be provided if the item size is 0. */
            ( !( ( puc_قطار_Storage != تہی ) && ( ux_شے_سائز == 0U ) ) ) &&
            ( !( ( puc_قطار_Storage == تہی ) && ( ux_شے_سائز != 0U ) ) ) )
        {
            #اگر ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticQueue_t or StaticSemaphore_t equals the size of
                 * the real queue and semaphore structures. */
                متلون size_t x_سائز = حجم( Static_قطار_t );

                /* This assertion cannot be branch covered in unit tests */
                configASSERT( x_سائز == حجم( قطار_t ) ); /* LCOV_EXCL_BR_LINE */
                ( خالی ) x_سائز;                             /* Prevent unused variable warning when configASSERT() is not defined. */
            }
            #اگر_ختم /* configASSERT_DEFINED */

            /* The address of a statically allocated queue was passed in, use it.
             * The address of a statically allocated storage area was also passed in
             * but is already set. */
            /* MISRA Ref 11.3.1 [Misaligned access] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
            /* coverity[misra_c_2012_rule_11_3_violation] */
            px_New_قطار = ( قطار_t * ) px_Static_قطار;

            #اگر ( config_SUPPORT_DYNAMIC_تخصیص == 1 )
            {
                /* Queues can be allocated either statically or dynamically, so
                 * note this queue was allocated statically in case the queue is
                 * later deleted. */
                px_New_قطار->ucStaticallyAllocated = pdTRUE;
            }
            #اگر_ختم /* configSUPPORT_DYNAMIC_ALLOCATION */

            prv_Initialise_New_قطار( ux_قطار_لمبائی, ux_شے_سائز, puc_قطار_Storage, uc_قطار_Type, px_New_قطار );
        }
        ورنہ
        {
            configASSERT( px_New_قطار );
            mtCOVERAGE_TEST_MARKER();
        }

        trace_RETURN_x_قطار_Generic_Create_Static( px_New_قطار );

        واپس px_New_قطار;
    }

#اگر_ختم /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

#اگر ( config_SUPPORT_STATIC_تخصیص == 1 )

    بنیاد_Type_t x_قطار_Generic_Get_Static_Buffers( قطار_Handle_t x_قطار,
                                              uint8_t ** ppuc_قطار_Storage,
                                              Static_قطار_t ** ppx_Static_قطار )
    {
        بنیاد_Type_t xReturn;
        قطار_t * ثابت px_قطار = x_قطار;

        trace_ENTER_x_قطار_Generic_Get_Static_Buffers( x_قطار, ppuc_قطار_Storage, ppx_Static_قطار );

        configASSERT( px_قطار );
        configASSERT( ppx_Static_قطار );

        #اگر ( config_SUPPORT_DYNAMIC_تخصیص == 1 )
        {
            /* Check if the queue was statically allocated. */
            اگر( px_قطار->ucStaticallyAllocated == ( uint8_t ) pdTRUE )
            {
                اگر( ppuc_قطار_Storage != تہی )
                {
                    *ppuc_قطار_Storage = ( uint8_t * ) px_قطار->pc_سر;
                }

                /* MISRA Ref 11.3.1 [Misaligned access] */
                /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
                /* coverity[misra_c_2012_rule_11_3_violation] */
                *ppx_Static_قطار = ( Static_قطار_t * ) px_قطار;
                xReturn = pdTRUE;
            }
            ورنہ
            {
                xReturn = pdFALSE;
            }
        }
        #ورنہ /* configSUPPORT_DYNAMIC_ALLOCATION */
        {
            /* Queue must have been statically allocated. */
            اگر( ppuc_قطار_Storage != تہی )
            {
                *ppuc_قطار_Storage = ( uint8_t * ) px_قطار->pc_سر;
            }

            *ppx_Static_قطار = ( Static_قطار_t * ) px_قطار;
            xReturn = pdTRUE;
        }
        #اگر_ختم /* configSUPPORT_DYNAMIC_ALLOCATION */

        trace_RETURN_x_قطار_Generic_Get_Static_Buffers( xReturn );

        واپس xReturn;
    }

#اگر_ختم /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

#اگر ( config_SUPPORT_DYNAMIC_تخصیص == 1 )

    قطار_Handle_t x_قطار_Generic_Create( ثابت UBaseType_t ux_قطار_لمبائی,
                                       ثابت UBaseType_t ux_شے_سائز,
                                       ثابت uint8_t uc_قطار_Type )
    {
        قطار_t * px_New_قطار = تہی;
        size_t x_قطار_سائز_اندر_بائٹیں;
        uint8_t * puc_قطار_Storage;

        trace_ENTER_x_قطار_Generic_Create( ux_قطار_لمبائی, ux_شے_سائز, uc_قطار_Type );

        اگر( ( ux_قطار_لمبائی > ( UBaseType_t ) 0 ) &&
            /* Check for multiplication overflow. */
            ( ( SIZE_MAX / ux_قطار_لمبائی ) >= ux_شے_سائز ) &&
            /* Check for addition overflow. */
            ( ( UBaseType_t ) ( SIZE_MAX - حجم( قطار_t ) ) >= ( ux_قطار_لمبائی * ux_شے_سائز ) ) )
        {
            /* Allocate enough space to hold the maximum number of items that
             * can be in the queue at any time.  It is valid for uxItemSize to be
             * zero in the case the queue is used as a semaphore. */
            x_قطار_سائز_اندر_بائٹیں = ( size_t ) ( ( size_t ) ux_قطار_لمبائی * ( size_t ) ux_شے_سائز );

            /* MISRA Ref 11.5.1 [Malloc memory assignment] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
            /* coverity[misra_c_2012_rule_11_5_violation] */
            px_New_قطار = ( قطار_t * ) pv_پورٹ_Malloc( حجم( قطار_t ) + x_قطار_سائز_اندر_بائٹیں );

            اگر( px_New_قطار != تہی )
            {
                /* Jump past the queue structure to find the location of the queue
                 * storage area. */
                puc_قطار_Storage = ( uint8_t * ) px_New_قطار;
                puc_قطار_Storage += حجم( قطار_t );

                #اگر ( config_SUPPORT_STATIC_تخصیص == 1 )
                {
                    /* Queues can be created either statically or dynamically, so
                     * note this task was created dynamically in case it is later
                     * deleted. */
                    px_New_قطار->ucStaticallyAllocated = pdFALSE;
                }
                #اگر_ختم /* configSUPPORT_STATIC_ALLOCATION */

                prv_Initialise_New_قطار( ux_قطار_لمبائی, ux_شے_سائز, puc_قطار_Storage, uc_قطار_Type, px_New_قطار );
            }
            ورنہ
            {
                trace_قطار_CREATE_FAILED( uc_قطار_Type );
                mtCOVERAGE_TEST_MARKER();
            }
        }
        ورنہ
        {
            configASSERT( px_New_قطار );
            mtCOVERAGE_TEST_MARKER();
        }

        trace_RETURN_x_قطار_Generic_Create( px_New_قطار );

        واپس px_New_قطار;
    }

#اگر_ختم /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

ساکن خالی prv_Initialise_New_قطار( ثابت UBaseType_t ux_قطار_لمبائی,
                                   ثابت UBaseType_t ux_شے_سائز,
                                   uint8_t * puc_قطار_Storage,
                                   ثابت uint8_t uc_قطار_Type,
                                   قطار_t * px_New_قطار )
{
    /* Remove compiler warnings about unused parameters should
     * configUSE_TRACE_FACILITY not be set to 1. */
    ( خالی ) uc_قطار_Type;

    اگر( ux_شے_سائز == ( UBaseType_t ) 0 )
    {
        /* No RAM was allocated for the queue storage area, but PC head cannot
         * be set to NULL because NULL is used as a key to say the queue is used as
         * a mutex.  Therefore just set pcHead to point to the queue as a benign
         * value that is known to be within the memory map. */
        px_New_قطار->pc_سر = ( int8_t * ) px_New_قطار;
    }
    ورنہ
    {
        /* Set the head to the start of the queue storage area. */
        px_New_قطار->pc_سر = ( int8_t * ) puc_قطار_Storage;
    }

    /* Initialise the queue members as described where the queue type is
     * defined. */
    px_New_قطار->ux_لمبائی = ux_قطار_لمبائی;
    px_New_قطار->ux_شے_سائز = ux_شے_سائز;
    ( خالی ) x_قطار_Generic_Reset( px_New_قطار, pdTRUE );

    #اگر ( configUSE_TRACE_FACILITY == 1 )
    {
        px_New_قطار->uc_قطار_Type = uc_قطار_Type;
    }
    #اگر_ختم /* configUSE_TRACE_FACILITY */

    #اگر ( config_USE_قطار_SETS == 1 )
    {
        px_New_قطار->px_قطار_سیٹ_Container = تہی;
    }
    #اگر_ختم /* configUSE_QUEUE_SETS */

    trace_قطار_CREATE( px_New_قطار );
}
/*-----------------------------------------------------------*/

#اگر ( configUSE_MUTEXES == 1 )

    ساکن خالی prvInitialiseMutex( قطار_t * px_New_قطار )
    {
        اگر( px_New_قطار != تہی )
        {
            /* The queue create function will set all the queue structure members
            * correctly for a generic queue, but this function is creating a
            * mutex.  Overwrite those members that need to be set differently -
            * in particular the information required for priority inheritance. */
            px_New_قطار->u.xSemaphore.xMutexHolder = تہی;
            px_New_قطار->ux_قطار_Type = قطار_قطار_IS_MUTEX;

            /* In case this is a recursive mutex. */
            px_New_قطار->u.xSemaphore.ux_Recursive_Call_گنتی = 0;

            traceCREATE_MUTEX( px_New_قطار );

            /* Start with the semaphore in the expected state. */
            ( خالی ) x_قطار_Generic_Send( px_New_قطار, تہی, ( TickType_t ) 0U, قطار_SEND_مقصد_عقب );
        }
        ورنہ
        {
            traceCREATE_MUTEX_FAILED();
        }
    }

#اگر_ختم /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

#اگر ( ( configUSE_MUTEXES == 1 ) && ( config_SUPPORT_DYNAMIC_تخصیص == 1 ) )

    قطار_Handle_t x_قطار_Create_Mutex( ثابت uint8_t uc_قطار_Type )
    {
        قطار_Handle_t x_New_قطار;
        ثابت UBaseType_t ux_Mutex_لمبائی = ( UBaseType_t ) 1, ux_Mutex_سائز = ( UBaseType_t ) 0;

        trace_ENTER_x_قطار_Create_Mutex( uc_قطار_Type );

        x_New_قطار = x_قطار_Generic_Create( ux_Mutex_لمبائی, ux_Mutex_سائز, uc_قطار_Type );
        prvInitialiseMutex( ( قطار_t * ) x_New_قطار );

        trace_RETURN_x_قطار_Create_Mutex( x_New_قطار );

        واپس x_New_قطار;
    }

#اگر_ختم /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

#اگر ( ( configUSE_MUTEXES == 1 ) && ( config_SUPPORT_STATIC_تخصیص == 1 ) )

    قطار_Handle_t x_قطار_Create_Mutex_Static( ثابت uint8_t uc_قطار_Type,
                                           Static_قطار_t * px_Static_قطار )
    {
        قطار_Handle_t x_New_قطار;
        ثابت UBaseType_t ux_Mutex_لمبائی = ( UBaseType_t ) 1, ux_Mutex_سائز = ( UBaseType_t ) 0;

        trace_ENTER_x_قطار_Create_Mutex_Static( uc_قطار_Type, px_Static_قطار );

        /* Prevent compiler warnings about unused parameters if
         * configUSE_TRACE_FACILITY does not equal 1. */
        ( خالی ) uc_قطار_Type;

        x_New_قطار = x_قطار_Generic_Create_Static( ux_Mutex_لمبائی, ux_Mutex_سائز, تہی, px_Static_قطار, uc_قطار_Type );
        prvInitialiseMutex( ( قطار_t * ) x_New_قطار );

        trace_RETURN_x_قطار_Create_Mutex_Static( x_New_قطار );

        واپس x_New_قطار;
    }

#اگر_ختم /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

#اگر ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) )

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
            اگر( pxSemaphore->ux_قطار_Type == قطار_قطار_IS_MUTEX )
            {
                pxReturn = pxSemaphore->u.xSemaphore.xMutexHolder;
            }
            ورنہ
            {
                pxReturn = تہی;
            }
        }
        taskEXIT_CRITICAL();

        trace_RETURN_x_قطار_Get_Mutex_Holder( pxReturn );

        واپس pxReturn;
    }

#اگر_ختم /* if ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) ) */
/*-----------------------------------------------------------*/

#اگر ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) )

    TaskHandle_t x_قطار_Get_Mutex_Holder_From_ISR( قطار_Handle_t xSemaphore )
    {
        TaskHandle_t pxReturn;

        trace_ENTER_x_قطار_Get_Mutex_Holder_From_ISR( xSemaphore );

        configASSERT( xSemaphore );

        /* Mutexes cannot be used in interrupt service routines, so the mutex
         * holder should not change in an ISR, and therefore a critical section is
         * not required here. */
        اگر( ( ( قطار_t * ) xSemaphore )->ux_قطار_Type == قطار_قطار_IS_MUTEX )
        {
            pxReturn = ( ( قطار_t * ) xSemaphore )->u.xSemaphore.xMutexHolder;
        }
        ورنہ
        {
            pxReturn = تہی;
        }

        trace_RETURN_x_قطار_Get_Mutex_Holder_From_ISR( pxReturn );

        واپس pxReturn;
    }

#اگر_ختم /* if ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) ) */
/*-----------------------------------------------------------*/

#اگر ( configUSE_RECURSIVE_MUTEXES == 1 )

    بنیاد_Type_t x_قطار_Give_Mutex_Recursive( قطار_Handle_t xMutex )
    {
        بنیاد_Type_t xReturn;
        قطار_t * ثابت pxMutex = ( قطار_t * ) xMutex;

        trace_ENTER_x_قطار_Give_Mutex_Recursive( xMutex );

        configASSERT( pxMutex );

        /* If this is the task that holds the mutex then xMutexHolder will not
         * change outside of this task.  If this task does not hold the mutex then
         * pxMutexHolder can never coincidentally equal the tasks handle, and as
         * this is the only condition we are interested in it does not matter if
         * pxMutexHolder is accessed simultaneously by another task.  Therefore no
         * mutual exclusion is required to test the pxMutexHolder variable. */
        اگر( pxMutex->u.xSemaphore.xMutexHolder == xTaskGetCurrentTaskHandle() )
        {
            traceGIVE_MUTEX_RECURSIVE( pxMutex );

            /* uxRecursiveCallCount cannot be zero if xMutexHolder is equal to
             * the task handle, therefore no underflow check is required.  Also,
             * uxRecursiveCallCount is only modified by the mutex holder, and as
             * there can only be one, no mutual exclusion is required to modify the
             * uxRecursiveCallCount member. */
            ( pxMutex->u.xSemaphore.ux_Recursive_Call_گنتی )--;

            /* Has the recursive call count unwound to 0? */
            اگر( pxMutex->u.xSemaphore.ux_Recursive_Call_گنتی == ( UBaseType_t ) 0 )
            {
                /* Return the mutex.  This will automatically unblock any other
                 * task that might be waiting to access the mutex. */
                ( خالی ) x_قطار_Generic_Send( pxMutex, تہی, قطار_MUTEX_GIVE_بلاک_وقت, قطار_SEND_مقصد_عقب );
            }
            ورنہ
            {
                mtCOVERAGE_TEST_MARKER();
            }

            xReturn = pdPASS;
        }
        ورنہ
        {
            /* The mutex cannot be given because the calling task is not the
             * holder. */
            xReturn = pdFAIL;

            traceGIVE_MUTEX_RECURSIVE_FAILED( pxMutex );
        }

        trace_RETURN_x_قطار_Give_Mutex_Recursive( xReturn );

        واپس xReturn;
    }

#اگر_ختم /* configUSE_RECURSIVE_MUTEXES */
/*-----------------------------------------------------------*/

#اگر ( configUSE_RECURSIVE_MUTEXES == 1 )

    بنیاد_Type_t x_قطار_Take_Mutex_Recursive( قطار_Handle_t xMutex,
                                         TickType_t x_Ticks_مقصد_Wait )
    {
        بنیاد_Type_t xReturn;
        قطار_t * ثابت pxMutex = ( قطار_t * ) xMutex;

        trace_ENTER_x_قطار_Take_Mutex_Recursive( xMutex, x_Ticks_مقصد_Wait );

        configASSERT( pxMutex );

        /* Comments regarding mutual exclusion as per those within
         * xQueueGiveMutexRecursive(). */

        traceTAKE_MUTEX_RECURSIVE( pxMutex );

        اگر( pxMutex->u.xSemaphore.xMutexHolder == xTaskGetCurrentTaskHandle() )
        {
            ( pxMutex->u.xSemaphore.ux_Recursive_Call_گنتی )++;
            xReturn = pdPASS;
        }
        ورنہ
        {
            xReturn = x_قطار_Semaphore_Take( pxMutex, x_Ticks_مقصد_Wait );

            /* pdPASS will only be returned if the mutex was successfully
             * obtained.  The calling task may have entered the Blocked state
             * before reaching here. */
            اگر( xReturn != pdFAIL )
            {
                ( pxMutex->u.xSemaphore.ux_Recursive_Call_گنتی )++;
            }
            ورنہ
            {
                traceTAKE_MUTEX_RECURSIVE_FAILED( pxMutex );
            }
        }

        trace_RETURN_x_قطار_Take_Mutex_Recursive( xReturn );

        واپس xReturn;
    }

#اگر_ختم /* configUSE_RECURSIVE_MUTEXES */
/*-----------------------------------------------------------*/

#اگر ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( config_SUPPORT_STATIC_تخصیص == 1 ) )

    قطار_Handle_t x_قطار_Create_Counting_Semaphore_Static( ثابت UBaseType_t ux_Max_گنتی,
                                                       ثابت UBaseType_t ux_Initial_گنتی,
                                                       Static_قطار_t * px_Static_قطار )
    {
        قطار_Handle_t xHandle = تہی;

        trace_ENTER_x_قطار_Create_Counting_Semaphore_Static( ux_Max_گنتی, ux_Initial_گنتی, px_Static_قطار );

        اگر( ( ux_Max_گنتی != 0U ) &&
            ( ux_Initial_گنتی <= ux_Max_گنتی ) )
        {
            xHandle = x_قطار_Generic_Create_Static( ux_Max_گنتی, قطار_SEMAPHORE_قطار_شے_لمبائی, تہی, px_Static_قطار, قطار_قطار_TYPE_COUNTING_SEMAPHORE );

            اگر( xHandle != تہی )
            {
                ( ( قطار_t * ) xHandle )->ux_Messages_انتظار = ux_Initial_گنتی;

                traceCREATE_COUNTING_SEMAPHORE();
            }
            ورنہ
            {
                traceCREATE_COUNTING_SEMAPHORE_FAILED();
            }
        }
        ورنہ
        {
            configASSERT( xHandle );
            mtCOVERAGE_TEST_MARKER();
        }

        trace_RETURN_x_قطار_Create_Counting_Semaphore_Static( xHandle );

        واپس xHandle;
    }

#اگر_ختم /* ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) ) */
/*-----------------------------------------------------------*/

#اگر ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( config_SUPPORT_DYNAMIC_تخصیص == 1 ) )

    قطار_Handle_t x_قطار_Create_Counting_Semaphore( ثابت UBaseType_t ux_Max_گنتی,
                                                 ثابت UBaseType_t ux_Initial_گنتی )
    {
        قطار_Handle_t xHandle = تہی;

        trace_ENTER_x_قطار_Create_Counting_Semaphore( ux_Max_گنتی, ux_Initial_گنتی );

        اگر( ( ux_Max_گنتی != 0U ) &&
            ( ux_Initial_گنتی <= ux_Max_گنتی ) )
        {
            xHandle = x_قطار_Generic_Create( ux_Max_گنتی, قطار_SEMAPHORE_قطار_شے_لمبائی, قطار_قطار_TYPE_COUNTING_SEMAPHORE );

            اگر( xHandle != تہی )
            {
                ( ( قطار_t * ) xHandle )->ux_Messages_انتظار = ux_Initial_گنتی;

                traceCREATE_COUNTING_SEMAPHORE();
            }
            ورنہ
            {
                traceCREATE_COUNTING_SEMAPHORE_FAILED();
            }
        }
        ورنہ
        {
            configASSERT( xHandle );
            mtCOVERAGE_TEST_MARKER();
        }

        trace_RETURN_x_قطار_Create_Counting_Semaphore( xHandle );

        واپس xHandle;
    }

#اگر_ختم /* ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) ) */
/*-----------------------------------------------------------*/

بنیاد_Type_t x_قطار_Generic_Send( قطار_Handle_t x_قطار,
                              ثابت خالی * ثابت pv_شے_مقصد_قطار,
                              TickType_t x_Ticks_مقصد_Wait,
                              ثابت بنیاد_Type_t xCopyPosition )
{
    بنیاد_Type_t x_اندراج_وقت_سیٹ = pdFALSE, xYieldRequired;
    وقت_برآمد_t x_وقت_برآمد_2;
    قطار_t * ثابت px_قطار = x_قطار;

    trace_ENTER_x_قطار_Generic_Send( x_قطار, pv_شے_مقصد_قطار, x_Ticks_مقصد_Wait, xCopyPosition );

    configASSERT( px_قطار );
    configASSERT( !( ( pv_شے_مقصد_قطار == تہی ) && ( px_قطار->ux_شے_سائز != ( UBaseType_t ) 0U ) ) );
    configASSERT( !( ( xCopyPosition == قطار_OVERWRITE ) && ( px_قطار->ux_لمبائی != 1 ) ) );
    #اگر ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_مقصد_Wait != 0 ) ) );
    }
    #اگر_ختم

    برائے( ; ; )
    {
        taskENTER_CRITICAL();
        {
            /* Is there room on the queue now?  The running task must be the
             * highest priority task wanting to access the queue.  If the head item
             * in the queue is to be overwritten then it does not matter if the
             * queue is full. */
            اگر( ( px_قطار->ux_Messages_انتظار < px_قطار->ux_لمبائی ) || ( xCopyPosition == قطار_OVERWRITE ) )
            {
                trace_قطار_SEND( px_قطار );

                #اگر ( config_USE_قطار_SETS == 1 )
                {
                    ثابت UBaseType_t ux_پچھلا_Messages_انتظار = px_قطار->ux_Messages_انتظار;

                    xYieldRequired = prv_Copy_ڈیٹا_مقصد_قطار( px_قطار, pv_شے_مقصد_قطار, xCopyPosition );

                    اگر( px_قطار->px_قطار_سیٹ_Container != تہی )
                    {
                        اگر( ( xCopyPosition == قطار_OVERWRITE ) && ( ux_پچھلا_Messages_انتظار != ( UBaseType_t ) 0 ) )
                        {
                            /* Do not notify the queue set as an existing item
                             * was overwritten in the queue so the number of items
                             * in the queue has not changed. */
                            mtCOVERAGE_TEST_MARKER();
                        }
                        ورنہ اگر( prv_Notify_قطار_سیٹ_Container( px_قطار ) != pdFALSE )
                        {
                            /* The queue is a member of a queue set, and posting
                             * to the queue set caused a higher priority task to
                             * unblock. A context switch is required. */
                            قطار_YIELD_IF_USING_PREEMPTION();
                        }
                        ورنہ
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    ورنہ
                    {
                        /* If there was a task waiting for data to arrive on the
                         * queue then unblock it now. */
                        اگر( list_LIST_IS_خالی_خانے( &( px_قطار->x_Tasks_انتظار_مقصد_Receive ) ) == pdFALSE )
                        {
                            اگر( xTaskRemoveFromEventList( &( px_قطار->x_Tasks_انتظار_مقصد_Receive ) ) != pdFALSE )
                            {
                                /* The unblocked task has a priority higher than
                                 * our own so yield immediately.  Yes it is ok to
                                 * do this from within the critical section - the
                                 * kernel takes care of that. */
                                قطار_YIELD_IF_USING_PREEMPTION();
                            }
                            ورنہ
                            {
                                mtCOVERAGE_TEST_MARKER();
                            }
                        }
                        ورنہ اگر( xYieldRequired != pdFALSE )
                        {
                            /* This path is a special case that will only get
                             * executed if the task was holding multiple mutexes
                             * and the mutexes were given back in an order that is
                             * different to that in which they were taken. */
                            قطار_YIELD_IF_USING_PREEMPTION();
                        }
                        ورنہ
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                }
                #ورنہ /* configUSE_QUEUE_SETS */
                {
                    xYieldRequired = prv_Copy_ڈیٹا_مقصد_قطار( px_قطار, pv_شے_مقصد_قطار, xCopyPosition );

                    /* If there was a task waiting for data to arrive on the
                     * queue then unblock it now. */
                    اگر( list_LIST_IS_خالی_خانے( &( px_قطار->x_Tasks_انتظار_مقصد_Receive ) ) == pdFALSE )
                    {
                        اگر( xTaskRemoveFromEventList( &( px_قطار->x_Tasks_انتظار_مقصد_Receive ) ) != pdFALSE )
                        {
                            /* The unblocked task has a priority higher than
                             * our own so yield immediately.  Yes it is ok to do
                             * this from within the critical section - the kernel
                             * takes care of that. */
                            قطار_YIELD_IF_USING_PREEMPTION();
                        }
                        ورنہ
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    ورنہ اگر( xYieldRequired != pdFALSE )
                    {
                        /* This path is a special case that will only get
                         * executed if the task was holding multiple mutexes and
                         * the mutexes were given back in an order that is
                         * different to that in which they were taken. */
                        قطار_YIELD_IF_USING_PREEMPTION();
                    }
                    ورنہ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #اگر_ختم /* configUSE_QUEUE_SETS */

                taskEXIT_CRITICAL();

                trace_RETURN_x_قطار_Generic_Send( pdPASS );

                واپس pdPASS;
            }
            ورنہ
            {
                اگر( x_Ticks_مقصد_Wait == ( TickType_t ) 0 )
                {
                    /* The queue was full and no block time is specified (or
                     * the block time has expired) so leave now. */
                    taskEXIT_CRITICAL();

                    /* Return to the original privilege level before exiting
                     * the function. */
                    trace_قطار_SEND_FAILED( px_قطار );
                    trace_RETURN_x_قطار_Generic_Send( err_قطار_بھرا );

                    واپس err_قطار_بھرا;
                }
                ورنہ اگر( x_اندراج_وقت_سیٹ == pdFALSE )
                {
                    /* The queue was full and a block time was specified so
                     * configure the timeout structure. */
                    v_Task_اندرونی_سیٹ_وقت_برآمد_State( &x_وقت_برآمد_2 );
                    x_اندراج_وقت_سیٹ = pdTRUE;
                }
                ورنہ
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
        prv_تالا_قطار( px_قطار );

        /* Update the timeout state to see if it has expired yet. */
        اگر( x_Task_Check_For_وقت_برآمد( &x_وقت_برآمد_2, &x_Ticks_مقصد_Wait ) == pdFALSE )
        {
            اگر( prv_Is_قطار_بھرا( px_قطار ) != pdFALSE )
            {
                trace_BLOCKING_ON_قطار_SEND( px_قطار );
                v_Task_رکھو_On_Event_List( &( px_قطار->x_Tasks_انتظار_مقصد_Send ), x_Ticks_مقصد_Wait );

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
                اگر( xTaskResumeAll() == pdFALSE )
                {
                    taskYIELD_WITHIN_API();
                }
            }
            ورنہ
            {
                /* Try again. */
                prv_Unlock_قطار( px_قطار );
                ( خالی ) xTaskResumeAll();
            }
        }
        ورنہ
        {
            /* The timeout has expired. */
            prv_Unlock_قطار( px_قطار );
            ( خالی ) xTaskResumeAll();

            trace_قطار_SEND_FAILED( px_قطار );
            trace_RETURN_x_قطار_Generic_Send( err_قطار_بھرا );

            واپس err_قطار_بھرا;
        }
    }
}
/*-----------------------------------------------------------*/

بنیاد_Type_t x_قطار_Generic_Send_From_ISR( قطار_Handle_t x_قطار,
                                     ثابت خالی * ثابت pv_شے_مقصد_قطار,
                                     بنیاد_Type_t * ثابت px_Higher_ترجیح_Task_Woken,
                                     ثابت بنیاد_Type_t xCopyPosition )
{
    بنیاد_Type_t xReturn;
    UBaseType_t ux_Saved_مداخلت_حالت;
    قطار_t * ثابت px_قطار = x_قطار;

    trace_ENTER_x_قطار_Generic_Send_From_ISR( x_قطار, pv_شے_مقصد_قطار, px_Higher_ترجیح_Task_Woken, xCopyPosition );

    configASSERT( px_قطار );
    configASSERT( !( ( pv_شے_مقصد_قطار == تہی ) && ( px_قطار->ux_شے_سائز != ( UBaseType_t ) 0U ) ) );
    configASSERT( !( ( xCopyPosition == قطار_OVERWRITE ) && ( px_قطار->ux_لمبائی != 1 ) ) );

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
    پورٹ_ASSERT_IF_مداخلت_ترجیح_INVALID();

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
        اگر( ( px_قطار->ux_Messages_انتظار < px_قطار->ux_لمبائی ) || ( xCopyPosition == قطار_OVERWRITE ) )
        {
            ثابت int8_t c_Tx_تالا = px_قطار->c_Tx_تالا;
            ثابت UBaseType_t ux_پچھلا_Messages_انتظار = px_قطار->ux_Messages_انتظار;

            trace_قطار_SEND_FROM_ISR( px_قطار );

            /* Semaphores use xQueueGiveFromISR(), so pxQueue will not be a
             *  semaphore or mutex.  That means prvCopyDataToQueue() cannot result
             *  in a task disinheriting a priority and prvCopyDataToQueue() can be
             *  called here even though the disinherit function does not check if
             *  the scheduler is suspended before accessing the ready lists. */
            ( خالی ) prv_Copy_ڈیٹا_مقصد_قطار( px_قطار, pv_شے_مقصد_قطار, xCopyPosition );

            /* The event list is not altered if the queue is locked.  This will
             * be done when the queue is unlocked later. */
            اگر( c_Tx_تالا == قطار_UNLOCKED )
            {
                #اگر ( config_USE_قطار_SETS == 1 )
                {
                    اگر( px_قطار->px_قطار_سیٹ_Container != تہی )
                    {
                        اگر( ( xCopyPosition == قطار_OVERWRITE ) && ( ux_پچھلا_Messages_انتظار != ( UBaseType_t ) 0 ) )
                        {
                            /* Do not notify the queue set as an existing item
                             * was overwritten in the queue so the number of items
                             * in the queue has not changed. */
                            mtCOVERAGE_TEST_MARKER();
                        }
                        ورنہ اگر( prv_Notify_قطار_سیٹ_Container( px_قطار ) != pdFALSE )
                        {
                            /* The queue is a member of a queue set, and posting
                             * to the queue set caused a higher priority task to
                             * unblock.  A context switch is required. */
                            اگر( px_Higher_ترجیح_Task_Woken != تہی )
                            {
                                *px_Higher_ترجیح_Task_Woken = pdTRUE;
                            }
                            ورنہ
                            {
                                mtCOVERAGE_TEST_MARKER();
                            }
                        }
                        ورنہ
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    ورنہ
                    {
                        اگر( list_LIST_IS_خالی_خانے( &( px_قطار->x_Tasks_انتظار_مقصد_Receive ) ) == pdFALSE )
                        {
                            اگر( xTaskRemoveFromEventList( &( px_قطار->x_Tasks_انتظار_مقصد_Receive ) ) != pdFALSE )
                            {
                                /* The task waiting has a higher priority so
                                 *  record that a context switch is required. */
                                اگر( px_Higher_ترجیح_Task_Woken != تہی )
                                {
                                    *px_Higher_ترجیح_Task_Woken = pdTRUE;
                                }
                                ورنہ
                                {
                                    mtCOVERAGE_TEST_MARKER();
                                }
                            }
                            ورنہ
                            {
                                mtCOVERAGE_TEST_MARKER();
                            }
                        }
                        ورنہ
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                }
                #ورنہ /* configUSE_QUEUE_SETS */
                {
                    اگر( list_LIST_IS_خالی_خانے( &( px_قطار->x_Tasks_انتظار_مقصد_Receive ) ) == pdFALSE )
                    {
                        اگر( xTaskRemoveFromEventList( &( px_قطار->x_Tasks_انتظار_مقصد_Receive ) ) != pdFALSE )
                        {
                            /* The task waiting has a higher priority so record that a
                             * context switch is required. */
                            اگر( px_Higher_ترجیح_Task_Woken != تہی )
                            {
                                *px_Higher_ترجیح_Task_Woken = pdTRUE;
                            }
                            ورنہ
                            {
                                mtCOVERAGE_TEST_MARKER();
                            }
                        }
                        ورنہ
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    ورنہ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }

                    /* Not used in this path. */
                    ( خالی ) ux_پچھلا_Messages_انتظار;
                }
                #اگر_ختم /* configUSE_QUEUE_SETS */
            }
            ورنہ
            {
                /* Increment the lock count so the task that unlocks the queue
                 * knows that data was posted while it was locked. */
                prv_Increment_قطار_Tx_تالا( px_قطار, c_Tx_تالا );
            }

            xReturn = pdPASS;
        }
        ورنہ
        {
            trace_قطار_SEND_FROM_ISR_FAILED( px_قطار );
            xReturn = err_قطار_بھرا;
        }
    }
    taskEXIT_CRITICAL_FROM_ISR( ux_Saved_مداخلت_حالت );

    trace_RETURN_x_قطار_Generic_Send_From_ISR( xReturn );

    واپس xReturn;
}
/*-----------------------------------------------------------*/

بنیاد_Type_t x_قطار_Give_From_ISR( قطار_Handle_t x_قطار,
                              بنیاد_Type_t * ثابت px_Higher_ترجیح_Task_Woken )
{
    بنیاد_Type_t xReturn;
    UBaseType_t ux_Saved_مداخلت_حالت;
    قطار_t * ثابت px_قطار = x_قطار;

    trace_ENTER_x_قطار_Give_From_ISR( x_قطار, px_Higher_ترجیح_Task_Woken );

    /* Similar to xQueueGenericSendFromISR() but used with semaphores where the
     * item size is 0.  Don't directly wake a task that was blocked on a queue
     * read, instead return a flag to say whether a context switch is required or
     * not (i.e. has a task with a higher priority than us been woken by this
     * post). */

    configASSERT( px_قطار );

    /* xQueueGenericSendFromISR() should be used instead of xQueueGiveFromISR()
     * if the item size is not 0. */
    configASSERT( px_قطار->ux_شے_سائز == 0 );

    /* Normally a mutex would not be given from an interrupt, especially if
     * there is a mutex holder, as priority inheritance makes no sense for an
     * interrupts, only tasks. */
    configASSERT( !( ( px_قطار->ux_قطار_Type == قطار_قطار_IS_MUTEX ) && ( px_قطار->u.xSemaphore.xMutexHolder != تہی ) ) );

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
    پورٹ_ASSERT_IF_مداخلت_ترجیح_INVALID();

    /* MISRA Ref 4.7.1 [Return value shall be checked] */
    /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
    /* coverity[misra_c_2012_directive_4_7_violation] */
    ux_Saved_مداخلت_حالت = ( UBaseType_t ) taskENTER_CRITICAL_FROM_ISR();
    {
        ثابت UBaseType_t ux_Messages_انتظار = px_قطار->ux_Messages_انتظار;

        /* When the queue is used to implement a semaphore no data is ever
         * moved through the queue but it is still valid to see if the queue 'has
         * space'. */
        اگر( ux_Messages_انتظار < px_قطار->ux_لمبائی )
        {
            ثابت int8_t c_Tx_تالا = px_قطار->c_Tx_تالا;

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
            اگر( c_Tx_تالا == قطار_UNLOCKED )
            {
                #اگر ( config_USE_قطار_SETS == 1 )
                {
                    اگر( px_قطار->px_قطار_سیٹ_Container != تہی )
                    {
                        اگر( prv_Notify_قطار_سیٹ_Container( px_قطار ) != pdFALSE )
                        {
                            /* The semaphore is a member of a queue set, and
                             * posting to the queue set caused a higher priority
                             * task to unblock.  A context switch is required. */
                            اگر( px_Higher_ترجیح_Task_Woken != تہی )
                            {
                                *px_Higher_ترجیح_Task_Woken = pdTRUE;
                            }
                            ورنہ
                            {
                                mtCOVERAGE_TEST_MARKER();
                            }
                        }
                        ورنہ
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    ورنہ
                    {
                        اگر( list_LIST_IS_خالی_خانے( &( px_قطار->x_Tasks_انتظار_مقصد_Receive ) ) == pdFALSE )
                        {
                            اگر( xTaskRemoveFromEventList( &( px_قطار->x_Tasks_انتظار_مقصد_Receive ) ) != pdFALSE )
                            {
                                /* The task waiting has a higher priority so
                                 *  record that a context switch is required. */
                                اگر( px_Higher_ترجیح_Task_Woken != تہی )
                                {
                                    *px_Higher_ترجیح_Task_Woken = pdTRUE;
                                }
                                ورنہ
                                {
                                    mtCOVERAGE_TEST_MARKER();
                                }
                            }
                            ورنہ
                            {
                                mtCOVERAGE_TEST_MARKER();
                            }
                        }
                        ورنہ
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                }
                #ورنہ /* configUSE_QUEUE_SETS */
                {
                    اگر( list_LIST_IS_خالی_خانے( &( px_قطار->x_Tasks_انتظار_مقصد_Receive ) ) == pdFALSE )
                    {
                        اگر( xTaskRemoveFromEventList( &( px_قطار->x_Tasks_انتظار_مقصد_Receive ) ) != pdFALSE )
                        {
                            /* The task waiting has a higher priority so record that a
                             * context switch is required. */
                            اگر( px_Higher_ترجیح_Task_Woken != تہی )
                            {
                                *px_Higher_ترجیح_Task_Woken = pdTRUE;
                            }
                            ورنہ
                            {
                                mtCOVERAGE_TEST_MARKER();
                            }
                        }
                        ورنہ
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    ورنہ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #اگر_ختم /* configUSE_QUEUE_SETS */
            }
            ورنہ
            {
                /* Increment the lock count so the task that unlocks the queue
                 * knows that data was posted while it was locked. */
                prv_Increment_قطار_Tx_تالا( px_قطار, c_Tx_تالا );
            }

            xReturn = pdPASS;
        }
        ورنہ
        {
            trace_قطار_SEND_FROM_ISR_FAILED( px_قطار );
            xReturn = err_قطار_بھرا;
        }
    }
    taskEXIT_CRITICAL_FROM_ISR( ux_Saved_مداخلت_حالت );

    trace_RETURN_x_قطار_Give_From_ISR( xReturn );

    واپس xReturn;
}
/*-----------------------------------------------------------*/

بنیاد_Type_t x_قطار_Receive( قطار_Handle_t x_قطار,
                          خالی * ثابت pv_بفر,
                          TickType_t x_Ticks_مقصد_Wait )
{
    بنیاد_Type_t x_اندراج_وقت_سیٹ = pdFALSE;
    وقت_برآمد_t x_وقت_برآمد_2;
    قطار_t * ثابت px_قطار = x_قطار;

    trace_ENTER_x_قطار_Receive( x_قطار, pv_بفر, x_Ticks_مقصد_Wait );

    /* Check the pointer is not NULL. */
    configASSERT( ( px_قطار ) );

    /* The buffer into which data is received can only be NULL if the data size
     * is zero (so no data is copied into the buffer). */
    configASSERT( !( ( ( pv_بفر ) == تہی ) && ( ( px_قطار )->ux_شے_سائز != ( UBaseType_t ) 0U ) ) );

    /* Cannot block if the scheduler is suspended. */
    #اگر ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_مقصد_Wait != 0 ) ) );
    }
    #اگر_ختم

    برائے( ; ; )
    {
        taskENTER_CRITICAL();
        {
            ثابت UBaseType_t ux_Messages_انتظار = px_قطار->ux_Messages_انتظار;

            /* Is there data in the queue now?  To be running the calling task
             * must be the highest priority task wanting to access the queue. */
            اگر( ux_Messages_انتظار > ( UBaseType_t ) 0 )
            {
                /* Data available, remove one item. */
                prv_Copy_ڈیٹا_From_قطار( px_قطار, pv_بفر );
                trace_قطار_RECEIVE( px_قطار );
                px_قطار->ux_Messages_انتظار = ( UBaseType_t ) ( ux_Messages_انتظار - ( UBaseType_t ) 1 );

                /* There is now space in the queue, were any tasks waiting to
                 * post to the queue?  If so, unblock the highest priority waiting
                 * task. */
                اگر( list_LIST_IS_خالی_خانے( &( px_قطار->x_Tasks_انتظار_مقصد_Send ) ) == pdFALSE )
                {
                    اگر( xTaskRemoveFromEventList( &( px_قطار->x_Tasks_انتظار_مقصد_Send ) ) != pdFALSE )
                    {
                        قطار_YIELD_IF_USING_PREEMPTION();
                    }
                    ورنہ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                ورنہ
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                taskEXIT_CRITICAL();

                trace_RETURN_x_قطار_Receive( pdPASS );

                واپس pdPASS;
            }
            ورنہ
            {
                اگر( x_Ticks_مقصد_Wait == ( TickType_t ) 0 )
                {
                    /* The queue was empty and no block time is specified (or
                     * the block time has expired) so leave now. */
                    taskEXIT_CRITICAL();

                    trace_قطار_RECEIVE_FAILED( px_قطار );
                    trace_RETURN_x_قطار_Receive( err_قطار_خالی_خانے );

                    واپس err_قطار_خالی_خانے;
                }
                ورنہ اگر( x_اندراج_وقت_سیٹ == pdFALSE )
                {
                    /* The queue was empty and a block time was specified so
                     * configure the timeout structure. */
                    v_Task_اندرونی_سیٹ_وقت_برآمد_State( &x_وقت_برآمد_2 );
                    x_اندراج_وقت_سیٹ = pdTRUE;
                }
                ورنہ
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
        prv_تالا_قطار( px_قطار );

        /* Update the timeout state to see if it has expired yet. */
        اگر( x_Task_Check_For_وقت_برآمد( &x_وقت_برآمد_2, &x_Ticks_مقصد_Wait ) == pdFALSE )
        {
            /* The timeout has not expired.  If the queue is still empty place
             * the task on the list of tasks waiting to receive from the queue. */
            اگر( prv_Is_قطار_خالی_خانے( px_قطار ) != pdFALSE )
            {
                trace_BLOCKING_ON_قطار_RECEIVE( px_قطار );
                v_Task_رکھو_On_Event_List( &( px_قطار->x_Tasks_انتظار_مقصد_Receive ), x_Ticks_مقصد_Wait );
                prv_Unlock_قطار( px_قطار );

                اگر( xTaskResumeAll() == pdFALSE )
                {
                    taskYIELD_WITHIN_API();
                }
                ورنہ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            ورنہ
            {
                /* The queue contains data again.  Loop back to try and read the
                 * data. */
                prv_Unlock_قطار( px_قطار );
                ( خالی ) xTaskResumeAll();
            }
        }
        ورنہ
        {
            /* Timed out.  If there is no data in the queue exit, otherwise loop
             * back and attempt to read the data. */
            prv_Unlock_قطار( px_قطار );
            ( خالی ) xTaskResumeAll();

            اگر( prv_Is_قطار_خالی_خانے( px_قطار ) != pdFALSE )
            {
                trace_قطار_RECEIVE_FAILED( px_قطار );
                trace_RETURN_x_قطار_Receive( err_قطار_خالی_خانے );

                واپس err_قطار_خالی_خانے;
            }
            ورنہ
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
    }
}
/*-----------------------------------------------------------*/

بنیاد_Type_t x_قطار_Semaphore_Take( قطار_Handle_t x_قطار,
                                TickType_t x_Ticks_مقصد_Wait )
{
    بنیاد_Type_t x_اندراج_وقت_سیٹ = pdFALSE;
    وقت_برآمد_t x_وقت_برآمد_2;
    قطار_t * ثابت px_قطار = x_قطار;

    #اگر ( configUSE_MUTEXES == 1 )
        بنیاد_Type_t xInheritanceOccurred = pdFALSE;
    #اگر_ختم

    trace_ENTER_x_قطار_Semaphore_Take( x_قطار, x_Ticks_مقصد_Wait );

    /* Check the queue pointer is not NULL. */
    configASSERT( ( px_قطار ) );

    /* Check this really is a semaphore, in which case the item size will be
     * 0. */
    configASSERT( px_قطار->ux_شے_سائز == 0 );

    /* Cannot block if the scheduler is suspended. */
    #اگر ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_مقصد_Wait != 0 ) ) );
    }
    #اگر_ختم

    برائے( ; ; )
    {
        taskENTER_CRITICAL();
        {
            /* Semaphores are queues with an item size of 0, and where the
             * number of messages in the queue is the semaphore's count value. */
            ثابت UBaseType_t ux_Semaphore_گنتی = px_قطار->ux_Messages_انتظار;

            /* Is there data in the queue now?  To be running the calling task
             * must be the highest priority task wanting to access the queue. */
            اگر( ux_Semaphore_گنتی > ( UBaseType_t ) 0 )
            {
                trace_قطار_RECEIVE( px_قطار );

                /* Semaphores are queues with a data size of zero and where the
                 * messages waiting is the semaphore's count.  Reduce the count. */
                px_قطار->ux_Messages_انتظار = ( UBaseType_t ) ( ux_Semaphore_گنتی - ( UBaseType_t ) 1 );

                #اگر ( configUSE_MUTEXES == 1 )
                {
                    اگر( px_قطار->ux_قطار_Type == قطار_قطار_IS_MUTEX )
                    {
                        /* Record the information required to implement
                         * priority inheritance should it become necessary. */
                        px_قطار->u.xSemaphore.xMutexHolder = pv_Task_Increment_Mutex_Held_گنتی();
                    }
                    ورنہ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #اگر_ختم /* configUSE_MUTEXES */

                /* Check to see if other tasks are blocked waiting to give the
                 * semaphore, and if so, unblock the highest priority such task. */
                اگر( list_LIST_IS_خالی_خانے( &( px_قطار->x_Tasks_انتظار_مقصد_Send ) ) == pdFALSE )
                {
                    اگر( xTaskRemoveFromEventList( &( px_قطار->x_Tasks_انتظار_مقصد_Send ) ) != pdFALSE )
                    {
                        قطار_YIELD_IF_USING_PREEMPTION();
                    }
                    ورنہ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                ورنہ
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                taskEXIT_CRITICAL();

                trace_RETURN_x_قطار_Semaphore_Take( pdPASS );

                واپس pdPASS;
            }
            ورنہ
            {
                اگر( x_Ticks_مقصد_Wait == ( TickType_t ) 0 )
                {
                    /* The semaphore count was 0 and no block time is specified
                     * (or the block time has expired) so exit now. */
                    taskEXIT_CRITICAL();

                    trace_قطار_RECEIVE_FAILED( px_قطار );
                    trace_RETURN_x_قطار_Semaphore_Take( err_قطار_خالی_خانے );

                    واپس err_قطار_خالی_خانے;
                }
                ورنہ اگر( x_اندراج_وقت_سیٹ == pdFALSE )
                {
                    /* The semaphore count was 0 and a block time was specified
                     * so configure the timeout structure ready to block. */
                    v_Task_اندرونی_سیٹ_وقت_برآمد_State( &x_وقت_برآمد_2 );
                    x_اندراج_وقت_سیٹ = pdTRUE;
                }
                ورنہ
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
        prv_تالا_قطار( px_قطار );

        /* Update the timeout state to see if it has expired yet. */
        اگر( x_Task_Check_For_وقت_برآمد( &x_وقت_برآمد_2, &x_Ticks_مقصد_Wait ) == pdFALSE )
        {
            /* A block time is specified and not expired.  If the semaphore
             * count is 0 then enter the Blocked state to wait for a semaphore to
             * become available.  As semaphores are implemented with queues the
             * queue being empty is equivalent to the semaphore count being 0. */
            اگر( prv_Is_قطار_خالی_خانے( px_قطار ) != pdFALSE )
            {
                trace_BLOCKING_ON_قطار_RECEIVE( px_قطار );

                #اگر ( configUSE_MUTEXES == 1 )
                {
                    اگر( px_قطار->ux_قطار_Type == قطار_قطار_IS_MUTEX )
                    {
                        taskENTER_CRITICAL();
                        {
                            xInheritanceOccurred = x_Task_ترجیح_Inherit( px_قطار->u.xSemaphore.xMutexHolder );
                        }
                        taskEXIT_CRITICAL();
                    }
                    ورنہ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #اگر_ختم /* if ( configUSE_MUTEXES == 1 ) */

                v_Task_رکھو_On_Event_List( &( px_قطار->x_Tasks_انتظار_مقصد_Receive ), x_Ticks_مقصد_Wait );
                prv_Unlock_قطار( px_قطار );

                اگر( xTaskResumeAll() == pdFALSE )
                {
                    taskYIELD_WITHIN_API();
                }
                ورنہ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            ورنہ
            {
                /* There was no timeout and the semaphore count was not 0, so
                 * attempt to take the semaphore again. */
                prv_Unlock_قطار( px_قطار );
                ( خالی ) xTaskResumeAll();
            }
        }
        ورنہ
        {
            /* Timed out. */
            prv_Unlock_قطار( px_قطار );
            ( خالی ) xTaskResumeAll();

            /* If the semaphore count is 0 exit now as the timeout has
             * expired.  Otherwise return to attempt to take the semaphore that is
             * known to be available.  As semaphores are implemented by queues the
             * queue being empty is equivalent to the semaphore count being 0. */
            اگر( prv_Is_قطار_خالی_خانے( px_قطار ) != pdFALSE )
            {
                #اگر ( configUSE_MUTEXES == 1 )
                {
                    /* xInheritanceOccurred could only have be set if
                     * pxQueue->uxQueueType == queueQUEUE_IS_MUTEX so no need to
                     * test the mutex type again to check it is actually a mutex. */
                    اگر( xInheritanceOccurred != pdFALSE )
                    {
                        taskENTER_CRITICAL();
                        {
                            UBaseType_t ux_Highest_انتظار_ترجیح;

                            /* This task blocking on the mutex caused another
                             * task to inherit this task's priority.  Now this task
                             * has timed out the priority should be disinherited
                             * again, but only as low as the next highest priority
                             * task that is waiting for the same mutex. */
                            ux_Highest_انتظار_ترجیح = prv_Get_Disinherit_ترجیح_After_Timeout( px_قطار );

                            /* vTaskPriorityDisinheritAfterTimeout uses the uxHighestWaitingPriority
                             * parameter to index pxReadyTasksLists when adding the task holding
                             * mutex to the ready list for its new priority. Coverity thinks that
                             * it can result in out-of-bounds access which is not true because
                             * uxHighestWaitingPriority, as returned by prvGetDisinheritPriorityAfterTimeout,
                             * is capped at ( configMAX_PRIORITIES - 1 ). */
                            /* coverity[overrun] */
                            v_Task_ترجیح_Disinherit_After_Timeout( px_قطار->u.xSemaphore.xMutexHolder, ux_Highest_انتظار_ترجیح );
                        }
                        taskEXIT_CRITICAL();
                    }
                }
                #اگر_ختم /* configUSE_MUTEXES */

                trace_قطار_RECEIVE_FAILED( px_قطار );
                trace_RETURN_x_قطار_Semaphore_Take( err_قطار_خالی_خانے );

                واپس err_قطار_خالی_خانے;
            }
            ورنہ
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
    }
}
/*-----------------------------------------------------------*/

بنیاد_Type_t x_قطار_Peek( قطار_Handle_t x_قطار,
                       خالی * ثابت pv_بفر,
                       TickType_t x_Ticks_مقصد_Wait )
{
    بنیاد_Type_t x_اندراج_وقت_سیٹ = pdFALSE;
    وقت_برآمد_t x_وقت_برآمد_2;
    int8_t * pcOriginalReadPosition;
    قطار_t * ثابت px_قطار = x_قطار;

    trace_ENTER_x_قطار_Peek( x_قطار, pv_بفر, x_Ticks_مقصد_Wait );

    /* Check the pointer is not NULL. */
    configASSERT( ( px_قطار ) );

    /* The buffer into which data is received can only be NULL if the data size
     * is zero (so no data is copied into the buffer. */
    configASSERT( !( ( ( pv_بفر ) == تہی ) && ( ( px_قطار )->ux_شے_سائز != ( UBaseType_t ) 0U ) ) );

    /* Cannot block if the scheduler is suspended. */
    #اگر ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_مقصد_Wait != 0 ) ) );
    }
    #اگر_ختم

    برائے( ; ; )
    {
        taskENTER_CRITICAL();
        {
            ثابت UBaseType_t ux_Messages_انتظار = px_قطار->ux_Messages_انتظار;

            /* Is there data in the queue now?  To be running the calling task
             * must be the highest priority task wanting to access the queue. */
            اگر( ux_Messages_انتظار > ( UBaseType_t ) 0 )
            {
                /* Remember the read position so it can be reset after the data
                 * is read from the queue as this function is only peeking the
                 * data, not removing it. */
                pcOriginalReadPosition = px_قطار->u.x_قطار.pcReadFrom;

                prv_Copy_ڈیٹا_From_قطار( px_قطار, pv_بفر );
                trace_قطار_PEEK( px_قطار );

                /* The data is not being removed, so reset the read pointer. */
                px_قطار->u.x_قطار.pcReadFrom = pcOriginalReadPosition;

                /* The data is being left in the queue, so see if there are
                 * any other tasks waiting for the data. */
                اگر( list_LIST_IS_خالی_خانے( &( px_قطار->x_Tasks_انتظار_مقصد_Receive ) ) == pdFALSE )
                {
                    اگر( xTaskRemoveFromEventList( &( px_قطار->x_Tasks_انتظار_مقصد_Receive ) ) != pdFALSE )
                    {
                        /* The task waiting has a higher priority than this task. */
                        قطار_YIELD_IF_USING_PREEMPTION();
                    }
                    ورنہ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                ورنہ
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                taskEXIT_CRITICAL();

                trace_RETURN_x_قطار_Peek( pdPASS );

                واپس pdPASS;
            }
            ورنہ
            {
                اگر( x_Ticks_مقصد_Wait == ( TickType_t ) 0 )
                {
                    /* The queue was empty and no block time is specified (or
                     * the block time has expired) so leave now. */
                    taskEXIT_CRITICAL();

                    trace_قطار_PEEK_FAILED( px_قطار );
                    trace_RETURN_x_قطار_Peek( err_قطار_خالی_خانے );

                    واپس err_قطار_خالی_خانے;
                }
                ورنہ اگر( x_اندراج_وقت_سیٹ == pdFALSE )
                {
                    /* The queue was empty and a block time was specified so
                     * configure the timeout structure ready to enter the blocked
                     * state. */
                    v_Task_اندرونی_سیٹ_وقت_برآمد_State( &x_وقت_برآمد_2 );
                    x_اندراج_وقت_سیٹ = pdTRUE;
                }
                ورنہ
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
        prv_تالا_قطار( px_قطار );

        /* Update the timeout state to see if it has expired yet. */
        اگر( x_Task_Check_For_وقت_برآمد( &x_وقت_برآمد_2, &x_Ticks_مقصد_Wait ) == pdFALSE )
        {
            /* Timeout has not expired yet, check to see if there is data in the
            * queue now, and if not enter the Blocked state to wait for data. */
            اگر( prv_Is_قطار_خالی_خانے( px_قطار ) != pdFALSE )
            {
                trace_BLOCKING_ON_قطار_PEEK( px_قطار );
                v_Task_رکھو_On_Event_List( &( px_قطار->x_Tasks_انتظار_مقصد_Receive ), x_Ticks_مقصد_Wait );
                prv_Unlock_قطار( px_قطار );

                اگر( xTaskResumeAll() == pdFALSE )
                {
                    taskYIELD_WITHIN_API();
                }
                ورنہ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            ورنہ
            {
                /* There is data in the queue now, so don't enter the blocked
                 * state, instead return to try and obtain the data. */
                prv_Unlock_قطار( px_قطار );
                ( خالی ) xTaskResumeAll();
            }
        }
        ورنہ
        {
            /* The timeout has expired.  If there is still no data in the queue
             * exit, otherwise go back and try to read the data again. */
            prv_Unlock_قطار( px_قطار );
            ( خالی ) xTaskResumeAll();

            اگر( prv_Is_قطار_خالی_خانے( px_قطار ) != pdFALSE )
            {
                trace_قطار_PEEK_FAILED( px_قطار );
                trace_RETURN_x_قطار_Peek( err_قطار_خالی_خانے );

                واپس err_قطار_خالی_خانے;
            }
            ورنہ
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
    }
}
/*-----------------------------------------------------------*/

بنیاد_Type_t x_قطار_Receive_From_ISR( قطار_Handle_t x_قطار,
                                 خالی * ثابت pv_بفر,
                                 بنیاد_Type_t * ثابت px_Higher_ترجیح_Task_Woken )
{
    بنیاد_Type_t xReturn;
    UBaseType_t ux_Saved_مداخلت_حالت;
    قطار_t * ثابت px_قطار = x_قطار;

    trace_ENTER_x_قطار_Receive_From_ISR( x_قطار, pv_بفر, px_Higher_ترجیح_Task_Woken );

    configASSERT( px_قطار );
    configASSERT( !( ( pv_بفر == تہی ) && ( px_قطار->ux_شے_سائز != ( UBaseType_t ) 0U ) ) );

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
    پورٹ_ASSERT_IF_مداخلت_ترجیح_INVALID();

    /* MISRA Ref 4.7.1 [Return value shall be checked] */
    /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
    /* coverity[misra_c_2012_directive_4_7_violation] */
    ux_Saved_مداخلت_حالت = ( UBaseType_t ) taskENTER_CRITICAL_FROM_ISR();
    {
        ثابت UBaseType_t ux_Messages_انتظار = px_قطار->ux_Messages_انتظار;

        /* Cannot block in an ISR, so check there is data available. */
        اگر( ux_Messages_انتظار > ( UBaseType_t ) 0 )
        {
            ثابت int8_t c_Rx_تالا = px_قطار->c_Rx_تالا;

            trace_قطار_RECEIVE_FROM_ISR( px_قطار );

            prv_Copy_ڈیٹا_From_قطار( px_قطار, pv_بفر );
            px_قطار->ux_Messages_انتظار = ( UBaseType_t ) ( ux_Messages_انتظار - ( UBaseType_t ) 1 );

            /* If the queue is locked the event list will not be modified.
             * Instead update the lock count so the task that unlocks the queue
             * will know that an ISR has removed data while the queue was
             * locked. */
            اگر( c_Rx_تالا == قطار_UNLOCKED )
            {
                اگر( list_LIST_IS_خالی_خانے( &( px_قطار->x_Tasks_انتظار_مقصد_Send ) ) == pdFALSE )
                {
                    اگر( xTaskRemoveFromEventList( &( px_قطار->x_Tasks_انتظار_مقصد_Send ) ) != pdFALSE )
                    {
                        /* The task waiting has a higher priority than us so
                         * force a context switch. */
                        اگر( px_Higher_ترجیح_Task_Woken != تہی )
                        {
                            *px_Higher_ترجیح_Task_Woken = pdTRUE;
                        }
                        ورنہ
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    ورنہ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                ورنہ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            ورنہ
            {
                /* Increment the lock count so the task that unlocks the queue
                 * knows that data was removed while it was locked. */
                prv_Increment_قطار_Rx_تالا( px_قطار, c_Rx_تالا );
            }

            xReturn = pdPASS;
        }
        ورنہ
        {
            xReturn = pdFAIL;
            trace_قطار_RECEIVE_FROM_ISR_FAILED( px_قطار );
        }
    }
    taskEXIT_CRITICAL_FROM_ISR( ux_Saved_مداخلت_حالت );

    trace_RETURN_x_قطار_Receive_From_ISR( xReturn );

    واپس xReturn;
}
/*-----------------------------------------------------------*/

بنیاد_Type_t x_قطار_Peek_From_ISR( قطار_Handle_t x_قطار,
                              خالی * ثابت pv_بفر )
{
    بنیاد_Type_t xReturn;
    UBaseType_t ux_Saved_مداخلت_حالت;
    int8_t * pcOriginalReadPosition;
    قطار_t * ثابت px_قطار = x_قطار;

    trace_ENTER_x_قطار_Peek_From_ISR( x_قطار, pv_بفر );

    configASSERT( px_قطار );
    configASSERT( !( ( pv_بفر == تہی ) && ( px_قطار->ux_شے_سائز != ( UBaseType_t ) 0U ) ) );
    configASSERT( px_قطار->ux_شے_سائز != 0 ); /* Can't peek a semaphore. */

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
    پورٹ_ASSERT_IF_مداخلت_ترجیح_INVALID();

    /* MISRA Ref 4.7.1 [Return value shall be checked] */
    /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
    /* coverity[misra_c_2012_directive_4_7_violation] */
    ux_Saved_مداخلت_حالت = ( UBaseType_t ) taskENTER_CRITICAL_FROM_ISR();
    {
        /* Cannot block in an ISR, so check there is data available. */
        اگر( px_قطار->ux_Messages_انتظار > ( UBaseType_t ) 0 )
        {
            trace_قطار_PEEK_FROM_ISR( px_قطار );

            /* Remember the read position so it can be reset as nothing is
             * actually being removed from the queue. */
            pcOriginalReadPosition = px_قطار->u.x_قطار.pcReadFrom;
            prv_Copy_ڈیٹا_From_قطار( px_قطار, pv_بفر );
            px_قطار->u.x_قطار.pcReadFrom = pcOriginalReadPosition;

            xReturn = pdPASS;
        }
        ورنہ
        {
            xReturn = pdFAIL;
            trace_قطار_PEEK_FROM_ISR_FAILED( px_قطار );
        }
    }
    taskEXIT_CRITICAL_FROM_ISR( ux_Saved_مداخلت_حالت );

    trace_RETURN_x_قطار_Peek_From_ISR( xReturn );

    واپس xReturn;
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

    واپس uxReturn;
}
/*-----------------------------------------------------------*/

UBaseType_t ux_قطار_Spaces_دستیاب( ثابت قطار_Handle_t x_قطار )
{
    UBaseType_t uxReturn;
    قطار_t * ثابت px_قطار = x_قطار;

    trace_ENTER_ux_قطار_Spaces_دستیاب( x_قطار );

    configASSERT( px_قطار );

    taskENTER_CRITICAL();
    {
        uxReturn = ( UBaseType_t ) ( px_قطار->ux_لمبائی - px_قطار->ux_Messages_انتظار );
    }
    taskEXIT_CRITICAL();

    trace_RETURN_ux_قطار_Spaces_دستیاب( uxReturn );

    واپس uxReturn;
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

    واپس uxReturn;
}
/*-----------------------------------------------------------*/

خالی v_قطار_Delete( قطار_Handle_t x_قطار )
{
    قطار_t * ثابت px_قطار = x_قطار;

    trace_ENTER_v_قطار_Delete( x_قطار );

    configASSERT( px_قطار );
    trace_قطار_DELETE( px_قطار );

    #اگر ( config_قطار_REGISTRY_سائز > 0 )
    {
        v_قطار_Unregister_قطار( px_قطار );
    }
    #اگر_ختم

    #اگر ( ( config_SUPPORT_DYNAMIC_تخصیص == 1 ) && ( config_SUPPORT_STATIC_تخصیص == 0 ) )
    {
        /* The queue can only have been allocated dynamically - free it
         * again. */
        v_پورٹ_Free( px_قطار );
    }
    #وگرنہ_اگر ( ( config_SUPPORT_DYNAMIC_تخصیص == 1 ) && ( config_SUPPORT_STATIC_تخصیص == 1 ) )
    {
        /* The queue could have been allocated statically or dynamically, so
         * check before attempting to free the memory. */
        اگر( px_قطار->ucStaticallyAllocated == ( uint8_t ) pdFALSE )
        {
            v_پورٹ_Free( px_قطار );
        }
        ورنہ
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    #ورنہ /* if ( ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) && ( configSUPPORT_STATIC_ALLOCATION == 0 ) ) */
    {
        /* The queue must have been statically allocated, so is not going to be
         * deleted.  Avoid compiler warnings about the unused parameter. */
        ( خالی ) px_قطار;
    }
    #اگر_ختم /* configSUPPORT_DYNAMIC_ALLOCATION */

    trace_RETURN_v_قطار_Delete();
}
/*-----------------------------------------------------------*/

#اگر ( configUSE_TRACE_FACILITY == 1 )

    UBaseType_t ux_قطار_Get_قطار_نمبر( قطار_Handle_t x_قطار )
    {
        trace_ENTER_ux_قطار_Get_قطار_نمبر( x_قطار );

        trace_RETURN_ux_قطار_Get_قطار_نمبر( ( ( قطار_t * ) x_قطار )->ux_قطار_نمبر );

        واپس ( ( قطار_t * ) x_قطار )->ux_قطار_نمبر;
    }

#اگر_ختم /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#اگر ( configUSE_TRACE_FACILITY == 1 )

    خالی v_قطار_سیٹ_قطار_نمبر( قطار_Handle_t x_قطار,
                               UBaseType_t ux_قطار_نمبر )
    {
        trace_ENTER_v_قطار_سیٹ_قطار_نمبر( x_قطار, ux_قطار_نمبر );

        ( ( قطار_t * ) x_قطار )->ux_قطار_نمبر = ux_قطار_نمبر;

        trace_RETURN_v_قطار_سیٹ_قطار_نمبر();
    }

#اگر_ختم /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#اگر ( configUSE_TRACE_FACILITY == 1 )

    uint8_t uc_قطار_Get_قطار_Type( قطار_Handle_t x_قطار )
    {
        trace_ENTER_uc_قطار_Get_قطار_Type( x_قطار );

        trace_RETURN_uc_قطار_Get_قطار_Type( ( ( قطار_t * ) x_قطار )->uc_قطار_Type );

        واپس ( ( قطار_t * ) x_قطار )->uc_قطار_Type;
    }

#اگر_ختم /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

UBaseType_t ux_قطار_Get_قطار_شے_سائز( قطار_Handle_t x_قطار ) /* PRIVILEGED_FUNCTION */
{
    trace_ENTER_ux_قطار_Get_قطار_شے_سائز( x_قطار );

    trace_RETURN_ux_قطار_Get_قطار_شے_سائز( ( ( قطار_t * ) x_قطار )->ux_شے_سائز );

    واپس ( ( قطار_t * ) x_قطار )->ux_شے_سائز;
}
/*-----------------------------------------------------------*/

UBaseType_t ux_قطار_Get_قطار_لمبائی( قطار_Handle_t x_قطار ) /* PRIVILEGED_FUNCTION */
{
    trace_ENTER_ux_قطار_Get_قطار_لمبائی( x_قطار );

    trace_RETURN_ux_قطار_Get_قطار_لمبائی( ( ( قطار_t * ) x_قطار )->ux_لمبائی );

    واپس ( ( قطار_t * ) x_قطار )->ux_لمبائی;
}
/*-----------------------------------------------------------*/

#اگر ( configUSE_MUTEXES == 1 )

    ساکن UBaseType_t prv_Get_Disinherit_ترجیح_After_Timeout( ثابت قطار_t * ثابت px_قطار )
    {
        UBaseType_t ux_Highest_ترجیح_کا_انتظار_Tasks;

        /* If a task waiting for a mutex causes the mutex holder to inherit a
         * priority, but the waiting task times out, then the holder should
         * disinherit the priority - but only down to the highest priority of any
         * other tasks that are waiting for the same mutex.  For this purpose,
         * return the priority of the highest priority task that is waiting for the
         * mutex. */
        اگر( list_CURRENT_LIST_لمبائی( &( px_قطار->x_Tasks_انتظار_مقصد_Receive ) ) > 0U )
        {
            ux_Highest_ترجیح_کا_انتظار_Tasks = ( UBaseType_t ) ( ( UBaseType_t ) configMAX_PRIORITIES - ( UBaseType_t ) list_GET_شے_قدر_کا_سر_اندراج( &( px_قطار->x_Tasks_انتظار_مقصد_Receive ) ) );
        }
        ورنہ
        {
            ux_Highest_ترجیح_کا_انتظار_Tasks = tsk_IDLE_ترجیح;
        }

        واپس ux_Highest_ترجیح_کا_انتظار_Tasks;
    }

#اگر_ختم /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

ساکن بنیاد_Type_t prv_Copy_ڈیٹا_مقصد_قطار( قطار_t * ثابت px_قطار,
                                      ثابت خالی * pv_شے_مقصد_قطار,
                                      ثابت بنیاد_Type_t xPosition )
{
    بنیاد_Type_t xReturn = pdFALSE;
    UBaseType_t ux_Messages_انتظار;

    /* This function is called from a critical section. */

    ux_Messages_انتظار = px_قطار->ux_Messages_انتظار;

    اگر( px_قطار->ux_شے_سائز == ( UBaseType_t ) 0 )
    {
        #اگر ( configUSE_MUTEXES == 1 )
        {
            اگر( px_قطار->ux_قطار_Type == قطار_قطار_IS_MUTEX )
            {
                /* The mutex is no longer being held. */
                xReturn = x_Task_ترجیح_Disinherit( px_قطار->u.xSemaphore.xMutexHolder );
                px_قطار->u.xSemaphore.xMutexHolder = تہی;
            }
            ورنہ
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        #اگر_ختم /* configUSE_MUTEXES */
    }
    ورنہ اگر( xPosition == قطار_SEND_مقصد_عقب )
    {
        ( خالی ) حافظہ_نقل( ( خالی * ) px_قطار->pc_Write_مقصد, pv_شے_مقصد_قطار, ( size_t ) px_قطار->ux_شے_سائز );
        px_قطار->pc_Write_مقصد += px_قطار->ux_شے_سائز;

        اگر( px_قطار->pc_Write_مقصد >= px_قطار->u.x_قطار.pc_دم )
        {
            px_قطار->pc_Write_مقصد = px_قطار->pc_سر;
        }
        ورنہ
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    ورنہ
    {
        ( خالی ) حافظہ_نقل( ( خالی * ) px_قطار->u.x_قطار.pcReadFrom, pv_شے_مقصد_قطار, ( size_t ) px_قطار->ux_شے_سائز );
        px_قطار->u.x_قطار.pcReadFrom -= px_قطار->ux_شے_سائز;

        اگر( px_قطار->u.x_قطار.pcReadFrom < px_قطار->pc_سر )
        {
            px_قطار->u.x_قطار.pcReadFrom = ( px_قطار->u.x_قطار.pc_دم - px_قطار->ux_شے_سائز );
        }
        ورنہ
        {
            mtCOVERAGE_TEST_MARKER();
        }

        اگر( xPosition == قطار_OVERWRITE )
        {
            اگر( ux_Messages_انتظار > ( UBaseType_t ) 0 )
            {
                /* An item is not being added but overwritten, so subtract
                 * one from the recorded number of items in the queue so when
                 * one is added again below the number of recorded items remains
                 * correct. */
                --ux_Messages_انتظار;
            }
            ورنہ
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        ورنہ
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }

    px_قطار->ux_Messages_انتظار = ( UBaseType_t ) ( ux_Messages_انتظار + ( UBaseType_t ) 1 );

    واپس xReturn;
}
/*-----------------------------------------------------------*/

ساکن خالی prv_Copy_ڈیٹا_From_قطار( قطار_t * ثابت px_قطار,
                                  خالی * ثابت pv_بفر )
{
    اگر( px_قطار->ux_شے_سائز != ( UBaseType_t ) 0 )
    {
        px_قطار->u.x_قطار.pcReadFrom += px_قطار->ux_شے_سائز;

        اگر( px_قطار->u.x_قطار.pcReadFrom >= px_قطار->u.x_قطار.pc_دم )
        {
            px_قطار->u.x_قطار.pcReadFrom = px_قطار->pc_سر;
        }
        ورنہ
        {
            mtCOVERAGE_TEST_MARKER();
        }

        ( خالی ) حافظہ_نقل( ( خالی * ) pv_بفر, ( خالی * ) px_قطار->u.x_قطار.pcReadFrom, ( size_t ) px_قطار->ux_شے_سائز );
    }
}
/*-----------------------------------------------------------*/

ساکن خالی prv_Unlock_قطار( قطار_t * ثابت px_قطار )
{
    /* THIS FUNCTION MUST BE CALLED WITH THE SCHEDULER SUSPENDED. */

    /* The lock counts contains the number of extra data items placed or
     * removed from the queue while the queue was locked.  When a queue is
     * locked items can be added or removed, but the event lists cannot be
     * updated. */
    taskENTER_CRITICAL();
    {
        int8_t c_Tx_تالا = px_قطار->c_Tx_تالا;

        /* See if data was added to the queue while it was locked. */
        جب( c_Tx_تالا > قطار_LOCKED_UNMODIFIED )
        {
            /* Data was posted while the queue was locked.  Are any tasks
             * blocked waiting for data to become available? */
            #اگر ( config_USE_قطار_SETS == 1 )
            {
                اگر( px_قطار->px_قطار_سیٹ_Container != تہی )
                {
                    اگر( prv_Notify_قطار_سیٹ_Container( px_قطار ) != pdFALSE )
                    {
                        /* The queue is a member of a queue set, and posting to
                         * the queue set caused a higher priority task to unblock.
                         * A context switch is required. */
                        vTaskMissedYield();
                    }
                    ورنہ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                ورنہ
                {
                    /* Tasks that are removed from the event list will get
                     * added to the pending ready list as the scheduler is still
                     * suspended. */
                    اگر( list_LIST_IS_خالی_خانے( &( px_قطار->x_Tasks_انتظار_مقصد_Receive ) ) == pdFALSE )
                    {
                        اگر( xTaskRemoveFromEventList( &( px_قطار->x_Tasks_انتظار_مقصد_Receive ) ) != pdFALSE )
                        {
                            /* The task waiting has a higher priority so record that a
                             * context switch is required. */
                            vTaskMissedYield();
                        }
                        ورنہ
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                    ورنہ
                    {
                        توڑو;
                    }
                }
            }
            #ورنہ /* configUSE_QUEUE_SETS */
            {
                /* Tasks that are removed from the event list will get added to
                 * the pending ready list as the scheduler is still suspended. */
                اگر( list_LIST_IS_خالی_خانے( &( px_قطار->x_Tasks_انتظار_مقصد_Receive ) ) == pdFALSE )
                {
                    اگر( xTaskRemoveFromEventList( &( px_قطار->x_Tasks_انتظار_مقصد_Receive ) ) != pdFALSE )
                    {
                        /* The task waiting has a higher priority so record that
                         * a context switch is required. */
                        vTaskMissedYield();
                    }
                    ورنہ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                ورنہ
                {
                    توڑو;
                }
            }
            #اگر_ختم /* configUSE_QUEUE_SETS */

            --c_Tx_تالا;
        }

        px_قطار->c_Tx_تالا = قطار_UNLOCKED;
    }
    taskEXIT_CRITICAL();

    /* Do the same for the Rx lock. */
    taskENTER_CRITICAL();
    {
        int8_t c_Rx_تالا = px_قطار->c_Rx_تالا;

        جب( c_Rx_تالا > قطار_LOCKED_UNMODIFIED )
        {
            اگر( list_LIST_IS_خالی_خانے( &( px_قطار->x_Tasks_انتظار_مقصد_Send ) ) == pdFALSE )
            {
                اگر( xTaskRemoveFromEventList( &( px_قطار->x_Tasks_انتظار_مقصد_Send ) ) != pdFALSE )
                {
                    vTaskMissedYield();
                }
                ورنہ
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                --c_Rx_تالا;
            }
            ورنہ
            {
                توڑو;
            }
        }

        px_قطار->c_Rx_تالا = قطار_UNLOCKED;
    }
    taskEXIT_CRITICAL();
}
/*-----------------------------------------------------------*/

ساکن بنیاد_Type_t prv_Is_قطار_خالی_خانے( ثابت قطار_t * px_قطار )
{
    بنیاد_Type_t xReturn;

    taskENTER_CRITICAL();
    {
        اگر( px_قطار->ux_Messages_انتظار == ( UBaseType_t ) 0 )
        {
            xReturn = pdTRUE;
        }
        ورنہ
        {
            xReturn = pdFALSE;
        }
    }
    taskEXIT_CRITICAL();

    واپس xReturn;
}
/*-----------------------------------------------------------*/

بنیاد_Type_t x_قطار_Is_قطار_خالی_خانے_From_ISR( ثابت قطار_Handle_t x_قطار )
{
    بنیاد_Type_t xReturn;
    قطار_t * ثابت px_قطار = x_قطار;

    trace_ENTER_x_قطار_Is_قطار_خالی_خانے_From_ISR( x_قطار );

    configASSERT( px_قطار );

    اگر( px_قطار->ux_Messages_انتظار == ( UBaseType_t ) 0 )
    {
        xReturn = pdTRUE;
    }
    ورنہ
    {
        xReturn = pdFALSE;
    }

    trace_RETURN_x_قطار_Is_قطار_خالی_خانے_From_ISR( xReturn );

    واپس xReturn;
}
/*-----------------------------------------------------------*/

ساکن بنیاد_Type_t prv_Is_قطار_بھرا( ثابت قطار_t * px_قطار )
{
    بنیاد_Type_t xReturn;

    taskENTER_CRITICAL();
    {
        اگر( px_قطار->ux_Messages_انتظار == px_قطار->ux_لمبائی )
        {
            xReturn = pdTRUE;
        }
        ورنہ
        {
            xReturn = pdFALSE;
        }
    }
    taskEXIT_CRITICAL();

    واپس xReturn;
}
/*-----------------------------------------------------------*/

بنیاد_Type_t x_قطار_Is_قطار_بھرا_From_ISR( ثابت قطار_Handle_t x_قطار )
{
    بنیاد_Type_t xReturn;
    قطار_t * ثابت px_قطار = x_قطار;

    trace_ENTER_x_قطار_Is_قطار_بھرا_From_ISR( x_قطار );

    configASSERT( px_قطار );

    اگر( px_قطار->ux_Messages_انتظار == px_قطار->ux_لمبائی )
    {
        xReturn = pdTRUE;
    }
    ورنہ
    {
        xReturn = pdFALSE;
    }

    trace_RETURN_x_قطار_Is_قطار_بھرا_From_ISR( xReturn );

    واپس xReturn;
}
/*-----------------------------------------------------------*/

#اگر ( configUSE_CO_ROUTINES == 1 )

    بنیاد_Type_t x_قطار_CRSend( قطار_Handle_t x_قطار,
                             ثابت خالی * pv_شے_مقصد_قطار,
                             TickType_t x_Ticks_مقصد_Wait )
    {
        بنیاد_Type_t xReturn;
        قطار_t * ثابت px_قطار = x_قطار;

        trace_ENTER_x_قطار_CRSend( x_قطار, pv_شے_مقصد_قطار, x_Ticks_مقصد_Wait );

        /* If the queue is already full we may have to block.  A critical section
         * is required to prevent an interrupt removing something from the queue
         * between the check to see if the queue is full and blocking on the queue. */
        پورٹ_DISABLE_INTERRUPTS();
        {
            اگر( prv_Is_قطار_بھرا( px_قطار ) != pdFALSE )
            {
                /* The queue is full - do we want to block or just leave without
                 * posting? */
                اگر( x_Ticks_مقصد_Wait > ( TickType_t ) 0 )
                {
                    /* As this is called from a coroutine we cannot block directly, but
                     * return indicating that we need to block. */
                    v_Co_Routine_جوڑو_مقصد_Delayed_List( x_Ticks_مقصد_Wait, &( px_قطار->x_Tasks_انتظار_مقصد_Send ) );
                    پورٹ_ENABLE_INTERRUPTS();
                    واپس err_قطار_BLOCKED;
                }
                ورنہ
                {
                    پورٹ_ENABLE_INTERRUPTS();
                    واپس err_قطار_بھرا;
                }
            }
        }
        پورٹ_ENABLE_INTERRUPTS();

        پورٹ_DISABLE_INTERRUPTS();
        {
            اگر( px_قطار->ux_Messages_انتظار < px_قطار->ux_لمبائی )
            {
                /* There is room in the queue, copy the data into the queue. */
                prv_Copy_ڈیٹا_مقصد_قطار( px_قطار, pv_شے_مقصد_قطار, قطار_SEND_مقصد_عقب );
                xReturn = pdPASS;

                /* Were any co-routines waiting for data to become available? */
                اگر( list_LIST_IS_خالی_خانے( &( px_قطار->x_Tasks_انتظار_مقصد_Receive ) ) == pdFALSE )
                {
                    /* In this instance the co-routine could be placed directly
                     * into the ready list as we are within a critical section.
                     * Instead the same pending ready list mechanism is used as if
                     * the event were caused from within an interrupt. */
                    اگر( xCoRoutineRemoveFromEventList( &( px_قطار->x_Tasks_انتظار_مقصد_Receive ) ) != pdFALSE )
                    {
                        /* The co-routine waiting has a higher priority so record
                         * that a yield might be appropriate. */
                        xReturn = err_قطار_YIELD;
                    }
                    ورنہ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                ورنہ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            ورنہ
            {
                xReturn = err_قطار_بھرا;
            }
        }
        پورٹ_ENABLE_INTERRUPTS();

        trace_RETURN_x_قطار_CRSend( xReturn );

        واپس xReturn;
    }

#اگر_ختم /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#اگر ( configUSE_CO_ROUTINES == 1 )

    بنیاد_Type_t x_قطار_CRReceive( قطار_Handle_t x_قطار,
                                خالی * pv_بفر,
                                TickType_t x_Ticks_مقصد_Wait )
    {
        بنیاد_Type_t xReturn;
        قطار_t * ثابت px_قطار = x_قطار;

        trace_ENTER_x_قطار_CRReceive( x_قطار, pv_بفر, x_Ticks_مقصد_Wait );

        /* If the queue is already empty we may have to block.  A critical section
         * is required to prevent an interrupt adding something to the queue
         * between the check to see if the queue is empty and blocking on the queue. */
        پورٹ_DISABLE_INTERRUPTS();
        {
            اگر( px_قطار->ux_Messages_انتظار == ( UBaseType_t ) 0 )
            {
                /* There are no messages in the queue, do we want to block or just
                 * leave with nothing? */
                اگر( x_Ticks_مقصد_Wait > ( TickType_t ) 0 )
                {
                    /* As this is a co-routine we cannot block directly, but return
                     * indicating that we need to block. */
                    v_Co_Routine_جوڑو_مقصد_Delayed_List( x_Ticks_مقصد_Wait, &( px_قطار->x_Tasks_انتظار_مقصد_Receive ) );
                    پورٹ_ENABLE_INTERRUPTS();
                    واپس err_قطار_BLOCKED;
                }
                ورنہ
                {
                    پورٹ_ENABLE_INTERRUPTS();
                    واپس err_قطار_بھرا;
                }
            }
            ورنہ
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        پورٹ_ENABLE_INTERRUPTS();

        پورٹ_DISABLE_INTERRUPTS();
        {
            اگر( px_قطار->ux_Messages_انتظار > ( UBaseType_t ) 0 )
            {
                /* Data is available from the queue. */
                px_قطار->u.x_قطار.pcReadFrom += px_قطار->ux_شے_سائز;

                اگر( px_قطار->u.x_قطار.pcReadFrom >= px_قطار->u.x_قطار.pc_دم )
                {
                    px_قطار->u.x_قطار.pcReadFrom = px_قطار->pc_سر;
                }
                ورنہ
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                --( px_قطار->ux_Messages_انتظار );
                ( خالی ) حافظہ_نقل( ( خالی * ) pv_بفر, ( خالی * ) px_قطار->u.x_قطار.pcReadFrom, ( غیرعلامتی ) px_قطار->ux_شے_سائز );

                xReturn = pdPASS;

                /* Were any co-routines waiting for space to become available? */
                اگر( list_LIST_IS_خالی_خانے( &( px_قطار->x_Tasks_انتظار_مقصد_Send ) ) == pdFALSE )
                {
                    /* In this instance the co-routine could be placed directly
                     * into the ready list as we are within a critical section.
                     * Instead the same pending ready list mechanism is used as if
                     * the event were caused from within an interrupt. */
                    اگر( xCoRoutineRemoveFromEventList( &( px_قطار->x_Tasks_انتظار_مقصد_Send ) ) != pdFALSE )
                    {
                        xReturn = err_قطار_YIELD;
                    }
                    ورنہ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                ورنہ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            ورنہ
            {
                xReturn = pdFAIL;
            }
        }
        پورٹ_ENABLE_INTERRUPTS();

        trace_RETURN_x_قطار_CRReceive( xReturn );

        واپس xReturn;
    }

#اگر_ختم /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#اگر ( configUSE_CO_ROUTINES == 1 )

    بنیاد_Type_t x_قطار_CRSend_From_ISR( قطار_Handle_t x_قطار,
                                    ثابت خالی * pv_شے_مقصد_قطار,
                                    بنیاد_Type_t xCoRoutinePreviouslyWoken )
    {
        قطار_t * ثابت px_قطار = x_قطار;

        trace_ENTER_x_قطار_CRSend_From_ISR( x_قطار, pv_شے_مقصد_قطار, xCoRoutinePreviouslyWoken );

        /* Cannot block within an ISR so if there is no space on the queue then
         * exit without doing anything. */
        اگر( px_قطار->ux_Messages_انتظار < px_قطار->ux_لمبائی )
        {
            prv_Copy_ڈیٹا_مقصد_قطار( px_قطار, pv_شے_مقصد_قطار, قطار_SEND_مقصد_عقب );

            /* We only want to wake one co-routine per ISR, so check that a
             * co-routine has not already been woken. */
            اگر( xCoRoutinePreviouslyWoken == pdFALSE )
            {
                اگر( list_LIST_IS_خالی_خانے( &( px_قطار->x_Tasks_انتظار_مقصد_Receive ) ) == pdFALSE )
                {
                    اگر( xCoRoutineRemoveFromEventList( &( px_قطار->x_Tasks_انتظار_مقصد_Receive ) ) != pdFALSE )
                    {
                        واپس pdTRUE;
                    }
                    ورنہ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                ورنہ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            ورنہ
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        ورنہ
        {
            mtCOVERAGE_TEST_MARKER();
        }

        trace_RETURN_x_قطار_CRSend_From_ISR( xCoRoutinePreviouslyWoken );

        واپس xCoRoutinePreviouslyWoken;
    }

#اگر_ختم /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#اگر ( configUSE_CO_ROUTINES == 1 )

    بنیاد_Type_t x_قطار_CRReceive_From_ISR( قطار_Handle_t x_قطار,
                                       خالی * pv_بفر,
                                       بنیاد_Type_t * pxCoRoutineWoken )
    {
        بنیاد_Type_t xReturn;
        قطار_t * ثابت px_قطار = x_قطار;

        trace_ENTER_x_قطار_CRReceive_From_ISR( x_قطار, pv_بفر, pxCoRoutineWoken );

        /* We cannot block from an ISR, so check there is data available. If
         * not then just leave without doing anything. */
        اگر( px_قطار->ux_Messages_انتظار > ( UBaseType_t ) 0 )
        {
            /* Copy the data from the queue. */
            px_قطار->u.x_قطار.pcReadFrom += px_قطار->ux_شے_سائز;

            اگر( px_قطار->u.x_قطار.pcReadFrom >= px_قطار->u.x_قطار.pc_دم )
            {
                px_قطار->u.x_قطار.pcReadFrom = px_قطار->pc_سر;
            }
            ورنہ
            {
                mtCOVERAGE_TEST_MARKER();
            }

            --( px_قطار->ux_Messages_انتظار );
            ( خالی ) حافظہ_نقل( ( خالی * ) pv_بفر, ( خالی * ) px_قطار->u.x_قطار.pcReadFrom, ( غیرعلامتی ) px_قطار->ux_شے_سائز );

            اگر( ( *pxCoRoutineWoken ) == pdFALSE )
            {
                اگر( list_LIST_IS_خالی_خانے( &( px_قطار->x_Tasks_انتظار_مقصد_Send ) ) == pdFALSE )
                {
                    اگر( xCoRoutineRemoveFromEventList( &( px_قطار->x_Tasks_انتظار_مقصد_Send ) ) != pdFALSE )
                    {
                        *pxCoRoutineWoken = pdTRUE;
                    }
                    ورنہ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                ورنہ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            ورنہ
            {
                mtCOVERAGE_TEST_MARKER();
            }

            xReturn = pdPASS;
        }
        ورنہ
        {
            xReturn = pdFAIL;
        }

        trace_RETURN_x_قطار_CRReceive_From_ISR( xReturn );

        واپس xReturn;
    }

#اگر_ختم /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#اگر ( config_قطار_REGISTRY_سائز > 0 )

    خالی v_قطار_جوڑو_مقصد_Registry( قطار_Handle_t x_قطار,
                              ثابت حرف * pc_قطار_Name )
    {
        UBaseType_t ux;
        قطار_Registry_شے_t * px_اندراج_مقصد_Write = تہی;

        trace_ENTER_v_قطار_جوڑو_مقصد_Registry( x_قطار, pc_قطار_Name );

        configASSERT( x_قطار );

        اگر( pc_قطار_Name != تہی )
        {
            /* See if there is an empty space in the registry.  A NULL name denotes
             * a free slot. */
            برائے( ux = ( UBaseType_t ) 0U; ux < ( UBaseType_t ) config_قطار_REGISTRY_سائز; ux++ )
            {
                /* Replace an existing entry if the queue is already in the registry. */
                اگر( x_قطار == x_قطار_Registry[ ux ].xHandle )
                {
                    px_اندراج_مقصد_Write = &( x_قطار_Registry[ ux ] );
                    توڑو;
                }
                /* Otherwise, store in the next empty location */
                ورنہ اگر( ( px_اندراج_مقصد_Write == تہی ) && ( x_قطار_Registry[ ux ].pc_قطار_Name == تہی ) )
                {
                    px_اندراج_مقصد_Write = &( x_قطار_Registry[ ux ] );
                }
                ورنہ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
        }

        اگر( px_اندراج_مقصد_Write != تہی )
        {
            /* Store the information on this queue. */
            px_اندراج_مقصد_Write->pc_قطار_Name = pc_قطار_Name;
            px_اندراج_مقصد_Write->xHandle = x_قطار;

            trace_قطار_REGISTRY_جوڑو( x_قطار, pc_قطار_Name );
        }

        trace_RETURN_v_قطار_جوڑو_مقصد_Registry();
    }

#اگر_ختم /* configQUEUE_REGISTRY_SIZE */
/*-----------------------------------------------------------*/

#اگر ( config_قطار_REGISTRY_سائز > 0 )

    ثابت حرف * pc_قطار_Get_Name( قطار_Handle_t x_قطار )
    {
        UBaseType_t ux;
        ثابت حرف * pcReturn = تہی;

        trace_ENTER_pc_قطار_Get_Name( x_قطار );

        configASSERT( x_قطار );

        /* Note there is nothing here to protect against another task adding or
         * removing entries from the registry while it is being searched. */

        برائے( ux = ( UBaseType_t ) 0U; ux < ( UBaseType_t ) config_قطار_REGISTRY_سائز; ux++ )
        {
            اگر( x_قطار_Registry[ ux ].xHandle == x_قطار )
            {
                pcReturn = x_قطار_Registry[ ux ].pc_قطار_Name;
                توڑو;
            }
            ورنہ
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }

        trace_RETURN_pc_قطار_Get_Name( pcReturn );

        واپس pcReturn;
    }

#اگر_ختم /* configQUEUE_REGISTRY_SIZE */
/*-----------------------------------------------------------*/

#اگر ( config_قطار_REGISTRY_سائز > 0 )

    خالی v_قطار_Unregister_قطار( قطار_Handle_t x_قطار )
    {
        UBaseType_t ux;

        trace_ENTER_v_قطار_Unregister_قطار( x_قطار );

        configASSERT( x_قطار );

        /* See if the handle of the queue being unregistered in actually in the
         * registry. */
        برائے( ux = ( UBaseType_t ) 0U; ux < ( UBaseType_t ) config_قطار_REGISTRY_سائز; ux++ )
        {
            اگر( x_قطار_Registry[ ux ].xHandle == x_قطار )
            {
                /* Set the name to NULL to show that this slot if free again. */
                x_قطار_Registry[ ux ].pc_قطار_Name = تہی;

                /* Set the handle to NULL to ensure the same queue handle cannot
                 * appear in the registry twice if it is added, removed, then
                 * added again. */
                x_قطار_Registry[ ux ].xHandle = ( قطار_Handle_t ) 0;
                توڑو;
            }
            ورنہ
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }

        trace_RETURN_v_قطار_Unregister_قطار();
    }

#اگر_ختم /* configQUEUE_REGISTRY_SIZE */
/*-----------------------------------------------------------*/

#اگر ( configUSE_TIMERS == 1 )

    خالی v_قطار_Wait_For_پیغام_Restricted( قطار_Handle_t x_قطار,
                                         TickType_t x_Ticks_مقصد_Wait,
                                         ثابت بنیاد_Type_t xWaitIndefinitely )
    {
        قطار_t * ثابت px_قطار = x_قطار;

        trace_ENTER_v_قطار_Wait_For_پیغام_Restricted( x_قطار, x_Ticks_مقصد_Wait, xWaitIndefinitely );

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
        prv_تالا_قطار( px_قطار );

        اگر( px_قطار->ux_Messages_انتظار == ( UBaseType_t ) 0U )
        {
            /* There is nothing in the queue, block for the specified period. */
            v_Task_رکھو_On_Event_List_Restricted( &( px_قطار->x_Tasks_انتظار_مقصد_Receive ), x_Ticks_مقصد_Wait, xWaitIndefinitely );
        }
        ورنہ
        {
            mtCOVERAGE_TEST_MARKER();
        }

        prv_Unlock_قطار( px_قطار );

        trace_RETURN_v_قطار_Wait_For_پیغام_Restricted();
    }

#اگر_ختم /* configUSE_TIMERS */
/*-----------------------------------------------------------*/

#اگر ( ( config_USE_قطار_SETS == 1 ) && ( config_SUPPORT_DYNAMIC_تخصیص == 1 ) )

    قطار_سیٹ_Handle_t x_قطار_Create_سیٹ( ثابت UBaseType_t ux_Event_قطار_لمبائی )
    {
        قطار_سیٹ_Handle_t px_قطار;

        trace_ENTER_x_قطار_Create_سیٹ( ux_Event_قطار_لمبائی );

        px_قطار = x_قطار_Generic_Create( ux_Event_قطار_لمبائی, ( UBaseType_t ) حجم( قطار_t * ), قطار_قطار_TYPE_سیٹ );

        trace_RETURN_x_قطار_Create_سیٹ( px_قطار );

        واپس px_قطار;
    }

#اگر_ختم /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#اگر ( config_USE_قطار_SETS == 1 )

    بنیاد_Type_t x_قطار_جوڑو_مقصد_سیٹ( قطار_سیٹ_Member_Handle_t x_قطار_Or_Semaphore,
                               قطار_سیٹ_Handle_t x_قطار_سیٹ )
    {
        بنیاد_Type_t xReturn;

        trace_ENTER_x_قطار_جوڑو_مقصد_سیٹ( x_قطار_Or_Semaphore, x_قطار_سیٹ );

        taskENTER_CRITICAL();
        {
            اگر( ( ( قطار_t * ) x_قطار_Or_Semaphore )->px_قطار_سیٹ_Container != تہی )
            {
                /* Cannot add a queue/semaphore to more than one queue set. */
                xReturn = pdFAIL;
            }
            ورنہ اگر( ( ( قطار_t * ) x_قطار_Or_Semaphore )->ux_Messages_انتظار != ( UBaseType_t ) 0 )
            {
                /* Cannot add a queue/semaphore to a queue set if there are already
                 * items in the queue/semaphore. */
                xReturn = pdFAIL;
            }
            ورنہ
            {
                ( ( قطار_t * ) x_قطار_Or_Semaphore )->px_قطار_سیٹ_Container = x_قطار_سیٹ;
                xReturn = pdPASS;
            }
        }
        taskEXIT_CRITICAL();

        trace_RETURN_x_قطار_جوڑو_مقصد_سیٹ( xReturn );

        واپس xReturn;
    }

#اگر_ختم /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#اگر ( config_USE_قطار_SETS == 1 )

    بنیاد_Type_t x_قطار_Remove_From_سیٹ( قطار_سیٹ_Member_Handle_t x_قطار_Or_Semaphore,
                                    قطار_سیٹ_Handle_t x_قطار_سیٹ )
    {
        بنیاد_Type_t xReturn;
        قطار_t * ثابت px_قطار_Or_Semaphore = ( قطار_t * ) x_قطار_Or_Semaphore;

        trace_ENTER_x_قطار_Remove_From_سیٹ( x_قطار_Or_Semaphore, x_قطار_سیٹ );

        اگر( px_قطار_Or_Semaphore->px_قطار_سیٹ_Container != x_قطار_سیٹ )
        {
            /* The queue was not a member of the set. */
            xReturn = pdFAIL;
        }
        ورنہ اگر( px_قطار_Or_Semaphore->ux_Messages_انتظار != ( UBaseType_t ) 0 )
        {
            /* It is dangerous to remove a queue from a set when the queue is
             * not empty because the queue set will still hold pending events for
             * the queue. */
            xReturn = pdFAIL;
        }
        ورنہ
        {
            taskENTER_CRITICAL();
            {
                /* The queue is no longer contained in the set. */
                px_قطار_Or_Semaphore->px_قطار_سیٹ_Container = تہی;
            }
            taskEXIT_CRITICAL();
            xReturn = pdPASS;
        }

        trace_RETURN_x_قطار_Remove_From_سیٹ( xReturn );

        واپس xReturn;
    }

#اگر_ختم /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#اگر ( config_USE_قطار_SETS == 1 )

    قطار_سیٹ_Member_Handle_t x_قطار_Select_From_سیٹ( قطار_سیٹ_Handle_t x_قطار_سیٹ,
                                                TickType_t ثابت x_Ticks_مقصد_Wait )
    {
        قطار_سیٹ_Member_Handle_t xReturn = تہی;

        trace_ENTER_x_قطار_Select_From_سیٹ( x_قطار_سیٹ, x_Ticks_مقصد_Wait );

        ( خالی ) x_قطار_Receive( ( قطار_Handle_t ) x_قطار_سیٹ, &xReturn, x_Ticks_مقصد_Wait );

        trace_RETURN_x_قطار_Select_From_سیٹ( xReturn );

        واپس xReturn;
    }

#اگر_ختم /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#اگر ( config_USE_قطار_SETS == 1 )

    قطار_سیٹ_Member_Handle_t x_قطار_Select_From_سیٹ_From_ISR( قطار_سیٹ_Handle_t x_قطار_سیٹ )
    {
        قطار_سیٹ_Member_Handle_t xReturn = تہی;

        trace_ENTER_x_قطار_Select_From_سیٹ_From_ISR( x_قطار_سیٹ );

        ( خالی ) x_قطار_Receive_From_ISR( ( قطار_Handle_t ) x_قطار_سیٹ, &xReturn, تہی );

        trace_RETURN_x_قطار_Select_From_سیٹ_From_ISR( xReturn );

        واپس xReturn;
    }

#اگر_ختم /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#اگر ( config_USE_قطار_SETS == 1 )

    ساکن بنیاد_Type_t prv_Notify_قطار_سیٹ_Container( ثابت قطار_t * ثابت px_قطار )
    {
        قطار_t * px_قطار_سیٹ_Container = px_قطار->px_قطار_سیٹ_Container;
        بنیاد_Type_t xReturn = pdFALSE;

        /* This function must be called form a critical section. */

        /* The following line is not reachable in unit tests because every call
         * to prvNotifyQueueSetContainer is preceded by a check that
         * pxQueueSetContainer != NULL */
        configASSERT( px_قطار_سیٹ_Container ); /* LCOV_EXCL_BR_LINE */
        configASSERT( px_قطار_سیٹ_Container->ux_Messages_انتظار < px_قطار_سیٹ_Container->ux_لمبائی );

        اگر( px_قطار_سیٹ_Container->ux_Messages_انتظار < px_قطار_سیٹ_Container->ux_لمبائی )
        {
            ثابت int8_t c_Tx_تالا = px_قطار_سیٹ_Container->c_Tx_تالا;

            trace_قطار_سیٹ_SEND( px_قطار_سیٹ_Container );

            /* The data copied is the handle of the queue that contains data. */
            xReturn = prv_Copy_ڈیٹا_مقصد_قطار( px_قطار_سیٹ_Container, &px_قطار, قطار_SEND_مقصد_عقب );

            اگر( c_Tx_تالا == قطار_UNLOCKED )
            {
                اگر( list_LIST_IS_خالی_خانے( &( px_قطار_سیٹ_Container->x_Tasks_انتظار_مقصد_Receive ) ) == pdFALSE )
                {
                    اگر( xTaskRemoveFromEventList( &( px_قطار_سیٹ_Container->x_Tasks_انتظار_مقصد_Receive ) ) != pdFALSE )
                    {
                        /* The task waiting has a higher priority. */
                        xReturn = pdTRUE;
                    }
                    ورنہ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                ورنہ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            ورنہ
            {
                prv_Increment_قطار_Tx_تالا( px_قطار_سیٹ_Container, c_Tx_تالا );
            }
        }
        ورنہ
        {
            mtCOVERAGE_TEST_MARKER();
        }

        واپس xReturn;
    }

#اگر_ختم /* configUSE_QUEUE_SETS */
