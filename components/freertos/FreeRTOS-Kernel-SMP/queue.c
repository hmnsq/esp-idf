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
#تعریف_کن MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#ضمن "FreeRTOS.h"
#ضمن "task.h"
#ضمن "queue.h"

#إذا ( configUSE_CO_ROUTINES == 1 )
    #ضمن "croutine.h"
#پایان_اگر

/* The MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#لغو_تعریف MPU_WRAPPERS_INCLUDED_FROM_API_FILE


/* Constants used with the cRxLock and cTxLock structure members. */
#تعریف_کن صف_UNLOCKED             ( ( int8_t ) -1 )
#تعریف_کن صف_LOCKED_UNMODIFIED    ( ( int8_t ) 0 )
#تعریف_کن صف_INT8_MAX             ( ( int8_t ) 127 )

/* When the Queue_t structure is used to represent a base queue its pcHead and
 * pcTail members are used as pointers into the queue storage area.  When the
 * Queue_t structure is used to represent a mutex pcHead and pcTail pointers are
 * not necessary, and the pcHead pointer is set to NULL to indicate that the
 * structure instead holds a pointer to the mutex holder (if any).  Map alternative
 * names to the pcHead and structure member to ensure the readability of the code
 * is maintained.  The QueuePointers_t and SemaphoreData_t types are used to form
 * a union as their usage is mutually exclusive dependent on what the queue is
 * being used for. */
#تعریف_کن ux_صف_Type               pc_سر
#تعریف_کن صف_صف_IS_MUTEX       عدم

تعريفنوع بنية صف_اشارهگرها
{
    int8_t * pc_دم;     /**< Points to the byte at the end of the queue storage area.  Once more byte is allocated than necessary to store the queue items, this is used as a marker. */
    int8_t * pcReadFrom; /**< Points to the last place that a queued item was read from when the structure is used as a queue. */
} صف_اشارهگرها_t;

تعريفنوع بنية Semaphore_داده
{
    TaskHandle_t xMutexHolder;        /**< The handle of the task that holds the mutex. */
    UBaseType_t ux_Recursive_Call_شمار; /**< Maintains a count of the number of times a recursive mutex has been recursively 'taken' when the structure is used as a mutex. */
} Semaphore_داده_t;

/* Semaphores do not actually store or copy data, so have an item size of
 * zero. */
#تعریف_کن صف_SEMAPHORE_صف_قلم_درازی    ( ( UBaseType_t ) 0 )
#تعریف_کن صف_MUTEX_GIVE_بلوک_زمان          ( ( TickType_t ) 0U )

#إذا ( configUSE_PREEMPTION == 0 )

/* If the cooperative scheduler is being used then a yield should not be
 * performed just because a higher priority task has been woken. */
    #تعریف_کن صف_YIELD_IF_USING_PREEMPTION()
#وإلا
    #إذا ( config_عدد_از_CORES == 1 )
        #تعریف_کن صف_YIELD_IF_USING_PREEMPTION()    پورت_YIELD_WITHIN_API()
    #وإلا /* #if ( configNUMBER_OF_CORES == 1 ) */
        #تعریف_کن صف_YIELD_IF_USING_PREEMPTION()    vTaskYieldWithinAPI()
    #پایان_اگر /* #if ( configNUMBER_OF_CORES == 1 ) */
#پایان_اگر

/*
 * Definition of the queue used by the scheduler.
 * Items are queued by copy, not reference.  See the following link for the
 * rationale: https://www.FreeRTOS.org/Embedded-RTOS-Queues.html
 */
تعريفنوع بنية صف_Definition /* The old naming convention is used to prevent breaking kernel aware debuggers. */
{
    int8_t * pc_سر;           /**< Points to the beginning of the queue storage area. */
    int8_t * pc_Write_مقصد;        /**< Points to the free next place in the storage area. */

    اتحاد
    {
        صف_اشارهگرها_t x_صف;     /**< Data required exclusively when this structure is used as a queue. */
        Semaphore_داده_t xSemaphore; /**< Data required exclusively when this structure is used as a semaphore. */
    } u;

    List_t x_Tasks_انتظار_مقصد_Send;             /**< List of tasks that are blocked waiting to post onto this queue.  Stored in priority order. */
    List_t x_Tasks_انتظار_مقصد_Receive;          /**< List of tasks that are blocked waiting to read from this queue.  Stored in priority order. */

    متطاير UBaseType_t ux_Messages_انتظار; /**< The number of items currently in the queue. */
    UBaseType_t ux_درازی;                   /**< The length of the queue defined as the number of items it will hold, not the number of bytes. */
    UBaseType_t ux_قلم_اندازه;                 /**< The size of each items that the queue will hold. */

    متطاير int8_t c_Rx_قفل;                /**< Stores the number of items received from the queue (removed from the queue) while the queue was locked.  Set to queueUNLOCKED when the queue is not locked. */
    متطاير int8_t c_Tx_قفل;                /**< Stores the number of items transmitted to the queue (added to the queue) while the queue was locked.  Set to queueUNLOCKED when the queue is not locked. */

    #إذا ( ( config_SUPPORT_STATIC_تخصیص == 1 ) && ( config_SUPPORT_DYNAMIC_تخصیص == 1 ) )
        uint8_t ucStaticallyAllocated; /**< Set to pdTRUE if the memory used by the queue was statically allocated to ensure no attempt is made to free the memory. */
    #پایان_اگر

    #إذا ( config_USE_صف_SETS == 1 )
        بنية صف_Definition * px_صف_مجموعه_Container;
    #پایان_اگر

    #إذا ( configUSE_TRACE_FACILITY == 1 )
        UBaseType_t ux_صف_عدد;
        uint8_t uc_صف_Type;
    #پایان_اگر
} x_صف_2;

/* The old xQUEUE name is maintained above then typedefed to the new Queue_t
 * name below to enable the use of older kernel aware debuggers. */
تعريفنوع x_صف_2 صف_t;

/*-----------------------------------------------------------*/

/*
 * The queue registry is just a means for kernel aware debuggers to locate
 * queue structures.  It has no other purpose so is an optional component.
 */
#إذا ( config_صف_REGISTRY_اندازه > 0 )

/* The type stored within the queue registry array.  This allows a name
 * to be assigned to each queue making kernel aware debugging a little
 * more user friendly. */
    تعريفنوع بنية صف_REGISTRY_قلم
    {
        ثابت حرف * pc_صف_Name;
        صف_Handle_t xHandle;
    } x_صف_Registry_قلم;

/* The old xQueueRegistryItem name is maintained above then typedefed to the
 * new xQueueRegistryItem name below to enable the use of older kernel aware
 * debuggers. */
    تعريفنوع x_صف_Registry_قلم صف_Registry_قلم_t;

/* The queue registry is simply an array of QueueRegistryItem_t structures.
 * The pcQueueName member of a structure being NULL is indicative of the
 * array position being vacant. */

/* MISRA Ref 8.4.2 [Declaration shall be visible] */
/* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-84 */
/* coverity[misra_c_2012_rule_8_4_violation] */
    PRIVILEGED_داده صف_Registry_قلم_t x_صف_Registry[ config_صف_REGISTRY_اندازه ];

#پایان_اگر /* configQUEUE_REGISTRY_SIZE */

/*
 * Unlocks a queue locked by a call to prvLockQueue.  Locking a queue does not
 * prevent an ISR from adding or removing items to the queue, but does prevent
 * an ISR from removing tasks from the queue event lists.  If an ISR finds a
 * queue is locked it will instead increment the appropriate queue lock count
 * to indicate that a task may require unblocking.  When the queue in unlocked
 * these lock counts are inspected, and the appropriate action taken.
 */
ساكن فراغ prv_Unlock_صف( صف_t * ثابت px_صف ) PRIVILEGED_FUNCTION;

/*
 * Uses a critical section to determine if there is any data in a queue.
 *
 * @return pdTRUE if the queue contains no items, otherwise pdFALSE.
 */
ساكن پایه_Type_t prv_Is_صف_خالی( ثابت صف_t * px_صف ) PRIVILEGED_FUNCTION;

/*
 * Uses a critical section to determine if there is any space in a queue.
 *
 * @return pdTRUE if there is no space, otherwise pdFALSE;
 */
ساكن پایه_Type_t prv_Is_صف_پر( ثابت صف_t * px_صف ) PRIVILEGED_FUNCTION;

/*
 * Copies an item into the queue, either at the front of the queue or the
 * back of the queue.
 */
ساكن پایه_Type_t prv_Copy_داده_مقصد_صف( صف_t * ثابت px_صف,
                                      ثابت فراغ * pv_قلم_مقصد_صف,
                                      ثابت پایه_Type_t xPosition ) PRIVILEGED_FUNCTION;

/*
 * Copies an item out of a queue.
 */
ساكن فراغ prv_Copy_داده_From_صف( صف_t * ثابت px_صف,
                                  فراغ * ثابت pv_میانگیر ) PRIVILEGED_FUNCTION;

#إذا ( config_USE_صف_SETS == 1 )

/*
 * Checks to see if a queue is a member of a queue set, and if so, notifies
 * the queue set that the queue contains data.
 */
    ساكن پایه_Type_t prv_Notify_صف_مجموعه_Container( ثابت صف_t * ثابت px_صف ) PRIVILEGED_FUNCTION;
#پایان_اگر

/*
 * Called after a Queue_t structure has been allocated either statically or
 * dynamically to fill in the structure's members.
 */
ساكن فراغ prv_Initialise_New_صف( ثابت UBaseType_t ux_صف_درازی,
                                   ثابت UBaseType_t ux_قلم_اندازه,
                                   uint8_t * puc_صف_Storage,
                                   ثابت uint8_t uc_صف_Type,
                                   صف_t * px_New_صف ) PRIVILEGED_FUNCTION;

/*
 * Mutexes are a special type of queue.  When a mutex is created, first the
 * queue is created, then prvInitialiseMutex() is called to configure the queue
 * as a mutex.
 */
#إذا ( configUSE_MUTEXES == 1 )
    ساكن فراغ prvInitialiseMutex( صف_t * px_New_صف ) PRIVILEGED_FUNCTION;
#پایان_اگر

#إذا ( configUSE_MUTEXES == 1 )

/*
 * If a task waiting for a mutex causes the mutex holder to inherit a
 * priority, but the waiting task times out, then the holder should
 * disinherit the priority - but only down to the highest priority of any
 * other tasks that are waiting for the same mutex.  This function returns
 * that priority.
 */
    ساكن UBaseType_t prv_Get_Disinherit_اولویت_After_Timeout( ثابت صف_t * ثابت px_صف ) PRIVILEGED_FUNCTION;
#پایان_اگر
/*-----------------------------------------------------------*/

/*
 * Macro to mark a queue as locked.  Locking a queue prevents an ISR from
 * accessing the queue event lists.
 */
#تعریف_کن prv_قفل_صف( px_صف )                            \
    taskENTER_CRITICAL();                                  \
    {                                                      \
        إذا( ( px_صف )->c_Rx_قفل == صف_UNLOCKED )        \
        {                                                  \
            ( px_صف )->c_Rx_قفل = صف_LOCKED_UNMODIFIED; \
        }                                                  \
        إذا( ( px_صف )->c_Tx_قفل == صف_UNLOCKED )        \
        {                                                  \
            ( px_صف )->c_Tx_قفل = صف_LOCKED_UNMODIFIED; \
        }                                                  \
    }                                                      \
    taskEXIT_CRITICAL()

/*
 * Macro to increment cTxLock member of the queue data structure. It is
 * capped at the number of tasks in the system as we cannot unblock more
 * tasks than the number of tasks in the system.
 */
#تعریف_کن prv_Increment_صف_Tx_قفل( px_صف, c_Tx_قفل )                           \
    افعل {                                                                      \
        ثابت UBaseType_t ux_عدد_از_Tasks = ux_Task_Get_عدد_از_Tasks();         \
        إذا( ( UBaseType_t ) ( c_Tx_قفل ) < ux_عدد_از_Tasks )                   \
        {                                                                     \
            configASSERT( ( c_Tx_قفل ) != صف_INT8_MAX );                     \
            ( px_صف )->c_Tx_قفل = ( int8_t ) ( ( c_Tx_قفل ) + ( int8_t ) 1 ); \
        }                                                                     \
    } طالما( 0 )

/*
 * Macro to increment cRxLock member of the queue data structure. It is
 * capped at the number of tasks in the system as we cannot unblock more
 * tasks than the number of tasks in the system.
 */
#تعریف_کن prv_Increment_صف_Rx_قفل( px_صف, c_Rx_قفل )                           \
    افعل {                                                                      \
        ثابت UBaseType_t ux_عدد_از_Tasks = ux_Task_Get_عدد_از_Tasks();         \
        إذا( ( UBaseType_t ) ( c_Rx_قفل ) < ux_عدد_از_Tasks )                   \
        {                                                                     \
            configASSERT( ( c_Rx_قفل ) != صف_INT8_MAX );                     \
            ( px_صف )->c_Rx_قفل = ( int8_t ) ( ( c_Rx_قفل ) + ( int8_t ) 1 ); \
        }                                                                     \
    } طالما( 0 )
/*-----------------------------------------------------------*/

پایه_Type_t x_صف_Generic_Reset( صف_Handle_t x_صف,
                               پایه_Type_t x_New_صف )
{
    پایه_Type_t xReturn = pdPASS;
    صف_t * ثابت px_صف = x_صف;

    trace_ENTER_x_صف_Generic_Reset( x_صف, x_New_صف );

    configASSERT( px_صف );

    إذا( ( px_صف != عدم ) &&
        ( px_صف->ux_درازی >= 1U ) &&
        /* Check for multiplication overflow. */
        ( ( SIZE_MAX / px_صف->ux_درازی ) >= px_صف->ux_قلم_اندازه ) )
    {
        taskENTER_CRITICAL();
        {
            px_صف->u.x_صف.pc_دم = px_صف->pc_سر + ( px_صف->ux_درازی * px_صف->ux_قلم_اندازه );
            px_صف->ux_Messages_انتظار = ( UBaseType_t ) 0U;
            px_صف->pc_Write_مقصد = px_صف->pc_سر;
            px_صف->u.x_صف.pcReadFrom = px_صف->pc_سر + ( ( px_صف->ux_درازی - 1U ) * px_صف->ux_قلم_اندازه );
            px_صف->c_Rx_قفل = صف_UNLOCKED;
            px_صف->c_Tx_قفل = صف_UNLOCKED;

            إذا( x_New_صف == pdFALSE )
            {
                /* If there are tasks blocked waiting to read from the queue, then
                 * the tasks will remain blocked as after this function exits the queue
                 * will still be empty.  If there are tasks blocked waiting to write to
                 * the queue, then one should be unblocked as after this function exits
                 * it will be possible to write to it. */
                إذا( list_LIST_IS_خالی( &( px_صف->x_Tasks_انتظار_مقصد_Send ) ) == pdFALSE )
                {
                    إذا( xTaskRemoveFromEventList( &( px_صف->x_Tasks_انتظار_مقصد_Send ) ) != pdFALSE )
                    {
                        صف_YIELD_IF_USING_PREEMPTION();
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
                vListInitialise( &( px_صف->x_Tasks_انتظار_مقصد_Send ) );
                vListInitialise( &( px_صف->x_Tasks_انتظار_مقصد_Receive ) );
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
    trace_RETURN_x_صف_Generic_Reset( xReturn );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

#إذا ( config_SUPPORT_STATIC_تخصیص == 1 )

    صف_Handle_t x_صف_Generic_Create_Static( ثابت UBaseType_t ux_صف_درازی,
                                             ثابت UBaseType_t ux_قلم_اندازه,
                                             uint8_t * puc_صف_Storage,
                                             Static_صف_t * px_Static_صف,
                                             ثابت uint8_t uc_صف_Type )
    {
        صف_t * px_New_صف = عدم;

        trace_ENTER_x_صف_Generic_Create_Static( ux_صف_درازی, ux_قلم_اندازه, puc_صف_Storage, px_Static_صف, uc_صف_Type );

        /* The StaticQueue_t structure and the queue storage area must be
         * supplied. */
        configASSERT( px_Static_صف );

        إذا( ( ux_صف_درازی > ( UBaseType_t ) 0 ) &&
            ( px_Static_صف != عدم ) &&

            /* A queue storage area should be provided if the item size is not 0, and
             * should not be provided if the item size is 0. */
            ( !( ( puc_صف_Storage != عدم ) && ( ux_قلم_اندازه == 0U ) ) ) &&
            ( !( ( puc_صف_Storage == عدم ) && ( ux_قلم_اندازه != 0U ) ) ) )
        {
            #إذا ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticQueue_t or StaticSemaphore_t equals the size of
                 * the real queue and semaphore structures. */
                متطاير size_t x_اندازه = حجم( Static_صف_t );

                /* This assertion cannot be branch covered in unit tests */
                configASSERT( x_اندازه == حجم( صف_t ) ); /* LCOV_EXCL_BR_LINE */
                ( فراغ ) x_اندازه;                             /* Prevent unused variable warning when configASSERT() is not defined. */
            }
            #پایان_اگر /* configASSERT_DEFINED */

            /* The address of a statically allocated queue was passed in, use it.
             * The address of a statically allocated storage area was also passed in
             * but is already set. */
            /* MISRA Ref 11.3.1 [Misaligned access] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
            /* coverity[misra_c_2012_rule_11_3_violation] */
            px_New_صف = ( صف_t * ) px_Static_صف;

            #إذا ( config_SUPPORT_DYNAMIC_تخصیص == 1 )
            {
                /* Queues can be allocated either statically or dynamically, so
                 * note this queue was allocated statically in case the queue is
                 * later deleted. */
                px_New_صف->ucStaticallyAllocated = pdTRUE;
            }
            #پایان_اگر /* configSUPPORT_DYNAMIC_ALLOCATION */

            prv_Initialise_New_صف( ux_صف_درازی, ux_قلم_اندازه, puc_صف_Storage, uc_صف_Type, px_New_صف );
        }
        وإلا
        {
            configASSERT( px_New_صف );
            mtCOVERAGE_TEST_MARKER();
        }

        trace_RETURN_x_صف_Generic_Create_Static( px_New_صف );

        أرجع px_New_صف;
    }

#پایان_اگر /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

#إذا ( config_SUPPORT_STATIC_تخصیص == 1 )

    پایه_Type_t x_صف_Generic_Get_Static_Buffers( صف_Handle_t x_صف,
                                              uint8_t ** ppuc_صف_Storage,
                                              Static_صف_t ** ppx_Static_صف )
    {
        پایه_Type_t xReturn;
        صف_t * ثابت px_صف = x_صف;

        trace_ENTER_x_صف_Generic_Get_Static_Buffers( x_صف, ppuc_صف_Storage, ppx_Static_صف );

        configASSERT( px_صف );
        configASSERT( ppx_Static_صف );

        #إذا ( config_SUPPORT_DYNAMIC_تخصیص == 1 )
        {
            /* Check if the queue was statically allocated. */
            إذا( px_صف->ucStaticallyAllocated == ( uint8_t ) pdTRUE )
            {
                إذا( ppuc_صف_Storage != عدم )
                {
                    *ppuc_صف_Storage = ( uint8_t * ) px_صف->pc_سر;
                }

                /* MISRA Ref 11.3.1 [Misaligned access] */
                /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
                /* coverity[misra_c_2012_rule_11_3_violation] */
                *ppx_Static_صف = ( Static_صف_t * ) px_صف;
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
            إذا( ppuc_صف_Storage != عدم )
            {
                *ppuc_صف_Storage = ( uint8_t * ) px_صف->pc_سر;
            }

            *ppx_Static_صف = ( Static_صف_t * ) px_صف;
            xReturn = pdTRUE;
        }
        #پایان_اگر /* configSUPPORT_DYNAMIC_ALLOCATION */

        trace_RETURN_x_صف_Generic_Get_Static_Buffers( xReturn );

        أرجع xReturn;
    }

#پایان_اگر /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

#إذا ( config_SUPPORT_DYNAMIC_تخصیص == 1 )

    صف_Handle_t x_صف_Generic_Create( ثابت UBaseType_t ux_صف_درازی,
                                       ثابت UBaseType_t ux_قلم_اندازه,
                                       ثابت uint8_t uc_صف_Type )
    {
        صف_t * px_New_صف = عدم;
        size_t x_صف_اندازه_درون_بایتها;
        uint8_t * puc_صف_Storage;

        trace_ENTER_x_صف_Generic_Create( ux_صف_درازی, ux_قلم_اندازه, uc_صف_Type );

        إذا( ( ux_صف_درازی > ( UBaseType_t ) 0 ) &&
            /* Check for multiplication overflow. */
            ( ( SIZE_MAX / ux_صف_درازی ) >= ux_قلم_اندازه ) &&
            /* Check for addition overflow. */
            ( ( UBaseType_t ) ( SIZE_MAX - حجم( صف_t ) ) >= ( ux_صف_درازی * ux_قلم_اندازه ) ) )
        {
            /* Allocate enough space to hold the maximum number of items that
             * can be in the queue at any time.  It is valid for uxItemSize to be
             * zero in the case the queue is used as a semaphore. */
            x_صف_اندازه_درون_بایتها = ( size_t ) ( ( size_t ) ux_صف_درازی * ( size_t ) ux_قلم_اندازه );

            /* MISRA Ref 11.5.1 [Malloc memory assignment] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
            /* coverity[misra_c_2012_rule_11_5_violation] */
            px_New_صف = ( صف_t * ) pv_پورت_Malloc( حجم( صف_t ) + x_صف_اندازه_درون_بایتها );

            إذا( px_New_صف != عدم )
            {
                /* Jump past the queue structure to find the location of the queue
                 * storage area. */
                puc_صف_Storage = ( uint8_t * ) px_New_صف;
                puc_صف_Storage += حجم( صف_t );

                #إذا ( config_SUPPORT_STATIC_تخصیص == 1 )
                {
                    /* Queues can be created either statically or dynamically, so
                     * note this task was created dynamically in case it is later
                     * deleted. */
                    px_New_صف->ucStaticallyAllocated = pdFALSE;
                }
                #پایان_اگر /* configSUPPORT_STATIC_ALLOCATION */

                prv_Initialise_New_صف( ux_صف_درازی, ux_قلم_اندازه, puc_صف_Storage, uc_صف_Type, px_New_صف );
            }
            وإلا
            {
                trace_صف_CREATE_FAILED( uc_صف_Type );
                mtCOVERAGE_TEST_MARKER();
            }
        }
        وإلا
        {
            configASSERT( px_New_صف );
            mtCOVERAGE_TEST_MARKER();
        }

        trace_RETURN_x_صف_Generic_Create( px_New_صف );

        أرجع px_New_صف;
    }

#پایان_اگر /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

ساكن فراغ prv_Initialise_New_صف( ثابت UBaseType_t ux_صف_درازی,
                                   ثابت UBaseType_t ux_قلم_اندازه,
                                   uint8_t * puc_صف_Storage,
                                   ثابت uint8_t uc_صف_Type,
                                   صف_t * px_New_صف )
{
    /* Remove compiler warnings about unused parameters should
     * configUSE_TRACE_FACILITY not be set to 1. */
    ( فراغ ) uc_صف_Type;

    إذا( ux_قلم_اندازه == ( UBaseType_t ) 0 )
    {
        /* No RAM was allocated for the queue storage area, but PC head cannot
         * be set to NULL because NULL is used as a key to say the queue is used as
         * a mutex.  Therefore just set pcHead to point to the queue as a benign
         * value that is known to be within the memory map. */
        px_New_صف->pc_سر = ( int8_t * ) px_New_صف;
    }
    وإلا
    {
        /* Set the head to the start of the queue storage area. */
        px_New_صف->pc_سر = ( int8_t * ) puc_صف_Storage;
    }

    /* Initialise the queue members as described where the queue type is
     * defined. */
    px_New_صف->ux_درازی = ux_صف_درازی;
    px_New_صف->ux_قلم_اندازه = ux_قلم_اندازه;
    ( فراغ ) x_صف_Generic_Reset( px_New_صف, pdTRUE );

    #إذا ( configUSE_TRACE_FACILITY == 1 )
    {
        px_New_صف->uc_صف_Type = uc_صف_Type;
    }
    #پایان_اگر /* configUSE_TRACE_FACILITY */

    #إذا ( config_USE_صف_SETS == 1 )
    {
        px_New_صف->px_صف_مجموعه_Container = عدم;
    }
    #پایان_اگر /* configUSE_QUEUE_SETS */

    trace_صف_CREATE( px_New_صف );
}
/*-----------------------------------------------------------*/

#إذا ( configUSE_MUTEXES == 1 )

    ساكن فراغ prvInitialiseMutex( صف_t * px_New_صف )
    {
        إذا( px_New_صف != عدم )
        {
            /* The queue create function will set all the queue structure members
            * correctly for a generic queue, but this function is creating a
            * mutex.  Overwrite those members that need to be set differently -
            * in particular the information required for priority inheritance. */
            px_New_صف->u.xSemaphore.xMutexHolder = عدم;
            px_New_صف->ux_صف_Type = صف_صف_IS_MUTEX;

            /* In case this is a recursive mutex. */
            px_New_صف->u.xSemaphore.ux_Recursive_Call_شمار = 0;

            traceCREATE_MUTEX( px_New_صف );

            /* Start with the semaphore in the expected state. */
            ( فراغ ) x_صف_Generic_Send( px_New_صف, عدم, ( TickType_t ) 0U, صف_SEND_مقصد_پشت );
        }
        وإلا
        {
            traceCREATE_MUTEX_FAILED();
        }
    }

#پایان_اگر /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

#إذا ( ( configUSE_MUTEXES == 1 ) && ( config_SUPPORT_DYNAMIC_تخصیص == 1 ) )

    صف_Handle_t x_صف_Create_Mutex( ثابت uint8_t uc_صف_Type )
    {
        صف_Handle_t x_New_صف;
        ثابت UBaseType_t ux_Mutex_درازی = ( UBaseType_t ) 1, ux_Mutex_اندازه = ( UBaseType_t ) 0;

        trace_ENTER_x_صف_Create_Mutex( uc_صف_Type );

        x_New_صف = x_صف_Generic_Create( ux_Mutex_درازی, ux_Mutex_اندازه, uc_صف_Type );
        prvInitialiseMutex( ( صف_t * ) x_New_صف );

        trace_RETURN_x_صف_Create_Mutex( x_New_صف );

        أرجع x_New_صف;
    }

#پایان_اگر /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

#إذا ( ( configUSE_MUTEXES == 1 ) && ( config_SUPPORT_STATIC_تخصیص == 1 ) )

    صف_Handle_t x_صف_Create_Mutex_Static( ثابت uint8_t uc_صف_Type,
                                           Static_صف_t * px_Static_صف )
    {
        صف_Handle_t x_New_صف;
        ثابت UBaseType_t ux_Mutex_درازی = ( UBaseType_t ) 1, ux_Mutex_اندازه = ( UBaseType_t ) 0;

        trace_ENTER_x_صف_Create_Mutex_Static( uc_صف_Type, px_Static_صف );

        /* Prevent compiler warnings about unused parameters if
         * configUSE_TRACE_FACILITY does not equal 1. */
        ( فراغ ) uc_صف_Type;

        x_New_صف = x_صف_Generic_Create_Static( ux_Mutex_درازی, ux_Mutex_اندازه, عدم, px_Static_صف, uc_صف_Type );
        prvInitialiseMutex( ( صف_t * ) x_New_صف );

        trace_RETURN_x_صف_Create_Mutex_Static( x_New_صف );

        أرجع x_New_صف;
    }

#پایان_اگر /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

#إذا ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) )

    TaskHandle_t x_صف_Get_Mutex_Holder( صف_Handle_t xSemaphore )
    {
        TaskHandle_t pxReturn;
        صف_t * ثابت pxSemaphore = ( صف_t * ) xSemaphore;

        trace_ENTER_x_صف_Get_Mutex_Holder( xSemaphore );

        configASSERT( xSemaphore );

        /* This function is called by xSemaphoreGetMutexHolder(), and should not
         * be called directly.  Note:  This is a good way of determining if the
         * calling task is the mutex holder, but not a good way of determining the
         * identity of the mutex holder, as the holder may change between the
         * following critical section exiting and the function returning. */
        taskENTER_CRITICAL();
        {
            إذا( pxSemaphore->ux_صف_Type == صف_صف_IS_MUTEX )
            {
                pxReturn = pxSemaphore->u.xSemaphore.xMutexHolder;
            }
            وإلا
            {
                pxReturn = عدم;
            }
        }
        taskEXIT_CRITICAL();

        trace_RETURN_x_صف_Get_Mutex_Holder( pxReturn );

        أرجع pxReturn;
    }

#پایان_اگر /* if ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) ) */
/*-----------------------------------------------------------*/

#إذا ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) )

    TaskHandle_t x_صف_Get_Mutex_Holder_From_ISR( صف_Handle_t xSemaphore )
    {
        TaskHandle_t pxReturn;

        trace_ENTER_x_صف_Get_Mutex_Holder_From_ISR( xSemaphore );

        configASSERT( xSemaphore );

        /* Mutexes cannot be used in interrupt service routines, so the mutex
         * holder should not change in an ISR, and therefore a critical section is
         * not required here. */
        إذا( ( ( صف_t * ) xSemaphore )->ux_صف_Type == صف_صف_IS_MUTEX )
        {
            pxReturn = ( ( صف_t * ) xSemaphore )->u.xSemaphore.xMutexHolder;
        }
        وإلا
        {
            pxReturn = عدم;
        }

        trace_RETURN_x_صف_Get_Mutex_Holder_From_ISR( pxReturn );

        أرجع pxReturn;
    }

#پایان_اگر /* if ( ( configUSE_MUTEXES == 1 ) && ( INCLUDE_xSemaphoreGetMutexHolder == 1 ) ) */
/*-----------------------------------------------------------*/

#إذا ( configUSE_RECURSIVE_MUTEXES == 1 )

    پایه_Type_t x_صف_Give_Mutex_Recursive( صف_Handle_t xMutex )
    {
        پایه_Type_t xReturn;
        صف_t * ثابت pxMutex = ( صف_t * ) xMutex;

        trace_ENTER_x_صف_Give_Mutex_Recursive( xMutex );

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
            ( pxMutex->u.xSemaphore.ux_Recursive_Call_شمار )--;

            /* Has the recursive call count unwound to 0? */
            إذا( pxMutex->u.xSemaphore.ux_Recursive_Call_شمار == ( UBaseType_t ) 0 )
            {
                /* Return the mutex.  This will automatically unblock any other
                 * task that might be waiting to access the mutex. */
                ( فراغ ) x_صف_Generic_Send( pxMutex, عدم, صف_MUTEX_GIVE_بلوک_زمان, صف_SEND_مقصد_پشت );
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

        trace_RETURN_x_صف_Give_Mutex_Recursive( xReturn );

        أرجع xReturn;
    }

#پایان_اگر /* configUSE_RECURSIVE_MUTEXES */
/*-----------------------------------------------------------*/

#إذا ( configUSE_RECURSIVE_MUTEXES == 1 )

    پایه_Type_t x_صف_Take_Mutex_Recursive( صف_Handle_t xMutex,
                                         TickType_t x_Ticks_مقصد_Wait )
    {
        پایه_Type_t xReturn;
        صف_t * ثابت pxMutex = ( صف_t * ) xMutex;

        trace_ENTER_x_صف_Take_Mutex_Recursive( xMutex, x_Ticks_مقصد_Wait );

        configASSERT( pxMutex );

        /* Comments regarding mutual exclusion as per those within
         * xQueueGiveMutexRecursive(). */

        traceTAKE_MUTEX_RECURSIVE( pxMutex );

        إذا( pxMutex->u.xSemaphore.xMutexHolder == xTaskGetCurrentTaskHandle() )
        {
            ( pxMutex->u.xSemaphore.ux_Recursive_Call_شمار )++;
            xReturn = pdPASS;
        }
        وإلا
        {
            xReturn = x_صف_Semaphore_Take( pxMutex, x_Ticks_مقصد_Wait );

            /* pdPASS will only be returned if the mutex was successfully
             * obtained.  The calling task may have entered the Blocked state
             * before reaching here. */
            إذا( xReturn != pdFAIL )
            {
                ( pxMutex->u.xSemaphore.ux_Recursive_Call_شمار )++;
            }
            وإلا
            {
                traceTAKE_MUTEX_RECURSIVE_FAILED( pxMutex );
            }
        }

        trace_RETURN_x_صف_Take_Mutex_Recursive( xReturn );

        أرجع xReturn;
    }

#پایان_اگر /* configUSE_RECURSIVE_MUTEXES */
/*-----------------------------------------------------------*/

#إذا ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( config_SUPPORT_STATIC_تخصیص == 1 ) )

    صف_Handle_t x_صف_Create_Counting_Semaphore_Static( ثابت UBaseType_t ux_Max_شمار,
                                                       ثابت UBaseType_t ux_Initial_شمار,
                                                       Static_صف_t * px_Static_صف )
    {
        صف_Handle_t xHandle = عدم;

        trace_ENTER_x_صف_Create_Counting_Semaphore_Static( ux_Max_شمار, ux_Initial_شمار, px_Static_صف );

        إذا( ( ux_Max_شمار != 0U ) &&
            ( ux_Initial_شمار <= ux_Max_شمار ) )
        {
            xHandle = x_صف_Generic_Create_Static( ux_Max_شمار, صف_SEMAPHORE_صف_قلم_درازی, عدم, px_Static_صف, صف_صف_TYPE_COUNTING_SEMAPHORE );

            إذا( xHandle != عدم )
            {
                ( ( صف_t * ) xHandle )->ux_Messages_انتظار = ux_Initial_شمار;

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

        trace_RETURN_x_صف_Create_Counting_Semaphore_Static( xHandle );

        أرجع xHandle;
    }

#پایان_اگر /* ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) ) */
/*-----------------------------------------------------------*/

#إذا ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( config_SUPPORT_DYNAMIC_تخصیص == 1 ) )

    صف_Handle_t x_صف_Create_Counting_Semaphore( ثابت UBaseType_t ux_Max_شمار,
                                                 ثابت UBaseType_t ux_Initial_شمار )
    {
        صف_Handle_t xHandle = عدم;

        trace_ENTER_x_صف_Create_Counting_Semaphore( ux_Max_شمار, ux_Initial_شمار );

        إذا( ( ux_Max_شمار != 0U ) &&
            ( ux_Initial_شمار <= ux_Max_شمار ) )
        {
            xHandle = x_صف_Generic_Create( ux_Max_شمار, صف_SEMAPHORE_صف_قلم_درازی, صف_صف_TYPE_COUNTING_SEMAPHORE );

            إذا( xHandle != عدم )
            {
                ( ( صف_t * ) xHandle )->ux_Messages_انتظار = ux_Initial_شمار;

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

        trace_RETURN_x_صف_Create_Counting_Semaphore( xHandle );

        أرجع xHandle;
    }

#پایان_اگر /* ( ( configUSE_COUNTING_SEMAPHORES == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) ) */
/*-----------------------------------------------------------*/

پایه_Type_t x_صف_Generic_Send( صف_Handle_t x_صف,
                              ثابت فراغ * ثابت pv_قلم_مقصد_صف,
                              TickType_t x_Ticks_مقصد_Wait,
                              ثابت پایه_Type_t xCopyPosition )
{
    پایه_Type_t x_مدخل_زمان_مجموعه = pdFALSE, xYieldRequired;
    زمان_بیرون_t x_زمان_بیرون_2;
    صف_t * ثابت px_صف = x_صف;

    trace_ENTER_x_صف_Generic_Send( x_صف, pv_قلم_مقصد_صف, x_Ticks_مقصد_Wait, xCopyPosition );

    configASSERT( px_صف );
    configASSERT( !( ( pv_قلم_مقصد_صف == عدم ) && ( px_صف->ux_قلم_اندازه != ( UBaseType_t ) 0U ) ) );
    configASSERT( !( ( xCopyPosition == صف_OVERWRITE ) && ( px_صف->ux_درازی != 1 ) ) );
    #إذا ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_مقصد_Wait != 0 ) ) );
    }
    #پایان_اگر

    لكل( ; ; )
    {
        taskENTER_CRITICAL();
        {
            /* Is there room on the queue now?  The running task must be the
             * highest priority task wanting to access the queue.  If the head item
             * in the queue is to be overwritten then it does not matter if the
             * queue is full. */
            إذا( ( px_صف->ux_Messages_انتظار < px_صف->ux_درازی ) || ( xCopyPosition == صف_OVERWRITE ) )
            {
                trace_صف_SEND( px_صف );

                #إذا ( config_USE_صف_SETS == 1 )
                {
                    ثابت UBaseType_t ux_قبلی_Messages_انتظار = px_صف->ux_Messages_انتظار;

                    xYieldRequired = prv_Copy_داده_مقصد_صف( px_صف, pv_قلم_مقصد_صف, xCopyPosition );

                    إذا( px_صف->px_صف_مجموعه_Container != عدم )
                    {
                        إذا( ( xCopyPosition == صف_OVERWRITE ) && ( ux_قبلی_Messages_انتظار != ( UBaseType_t ) 0 ) )
                        {
                            /* Do not notify the queue set as an existing item
                             * was overwritten in the queue so the number of items
                             * in the queue has not changed. */
                            mtCOVERAGE_TEST_MARKER();
                        }
                        وإلا إذا( prv_Notify_صف_مجموعه_Container( px_صف ) != pdFALSE )
                        {
                            /* The queue is a member of a queue set, and posting
                             * to the queue set caused a higher priority task to
                             * unblock. A context switch is required. */
                            صف_YIELD_IF_USING_PREEMPTION();
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
                        إذا( list_LIST_IS_خالی( &( px_صف->x_Tasks_انتظار_مقصد_Receive ) ) == pdFALSE )
                        {
                            إذا( xTaskRemoveFromEventList( &( px_صف->x_Tasks_انتظار_مقصد_Receive ) ) != pdFALSE )
                            {
                                /* The unblocked task has a priority higher than
                                 * our own so yield immediately.  Yes it is ok to
                                 * do this from within the critical section - the
                                 * kernel takes care of that. */
                                صف_YIELD_IF_USING_PREEMPTION();
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
                            صف_YIELD_IF_USING_PREEMPTION();
                        }
                        وإلا
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }
                    }
                }
                #وإلا /* configUSE_QUEUE_SETS */
                {
                    xYieldRequired = prv_Copy_داده_مقصد_صف( px_صف, pv_قلم_مقصد_صف, xCopyPosition );

                    /* If there was a task waiting for data to arrive on the
                     * queue then unblock it now. */
                    إذا( list_LIST_IS_خالی( &( px_صف->x_Tasks_انتظار_مقصد_Receive ) ) == pdFALSE )
                    {
                        إذا( xTaskRemoveFromEventList( &( px_صف->x_Tasks_انتظار_مقصد_Receive ) ) != pdFALSE )
                        {
                            /* The unblocked task has a priority higher than
                             * our own so yield immediately.  Yes it is ok to do
                             * this from within the critical section - the kernel
                             * takes care of that. */
                            صف_YIELD_IF_USING_PREEMPTION();
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
                        صف_YIELD_IF_USING_PREEMPTION();
                    }
                    وإلا
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #پایان_اگر /* configUSE_QUEUE_SETS */

                taskEXIT_CRITICAL();

                trace_RETURN_x_صف_Generic_Send( pdPASS );

                أرجع pdPASS;
            }
            وإلا
            {
                إذا( x_Ticks_مقصد_Wait == ( TickType_t ) 0 )
                {
                    /* The queue was full and no block time is specified (or
                     * the block time has expired) so leave now. */
                    taskEXIT_CRITICAL();

                    /* Return to the original privilege level before exiting
                     * the function. */
                    trace_صف_SEND_FAILED( px_صف );
                    trace_RETURN_x_صف_Generic_Send( err_صف_پر );

                    أرجع err_صف_پر;
                }
                وإلا إذا( x_مدخل_زمان_مجموعه == pdFALSE )
                {
                    /* The queue was full and a block time was specified so
                     * configure the timeout structure. */
                    v_Task_داخلی_مجموعه_زمان_بیرون_State( &x_زمان_بیرون_2 );
                    x_مدخل_زمان_مجموعه = pdTRUE;
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
        prv_قفل_صف( px_صف );

        /* Update the timeout state to see if it has expired yet. */
        إذا( x_Task_Check_For_زمان_بیرون( &x_زمان_بیرون_2, &x_Ticks_مقصد_Wait ) == pdFALSE )
        {
            إذا( prv_Is_صف_پر( px_صف ) != pdFALSE )
            {
                trace_BLOCKING_ON_صف_SEND( px_صف );
                v_Task_قراربده_On_Event_List( &( px_صف->x_Tasks_انتظار_مقصد_Send ), x_Ticks_مقصد_Wait );

                /* Unlocking the queue means queue events can effect the
                 * event list. It is possible that interrupts occurring now
                 * remove this task from the event list again - but as the
                 * scheduler is suspended the task will go onto the pending
                 * ready list instead of the actual ready list. */
                prv_Unlock_صف( px_صف );

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
                prv_Unlock_صف( px_صف );
                ( فراغ ) xTaskResumeAll();
            }
        }
        وإلا
        {
            /* The timeout has expired. */
            prv_Unlock_صف( px_صف );
            ( فراغ ) xTaskResumeAll();

            trace_صف_SEND_FAILED( px_صف );
            trace_RETURN_x_صف_Generic_Send( err_صف_پر );

            أرجع err_صف_پر;
        }
    }
}
/*-----------------------------------------------------------*/

پایه_Type_t x_صف_Generic_Send_From_ISR( صف_Handle_t x_صف,
                                     ثابت فراغ * ثابت pv_قلم_مقصد_صف,
                                     پایه_Type_t * ثابت px_Higher_اولویت_Task_Woken,
                                     ثابت پایه_Type_t xCopyPosition )
{
    پایه_Type_t xReturn;
    UBaseType_t ux_Saved_وقفه_وضعیت;
    صف_t * ثابت px_صف = x_صف;

    trace_ENTER_x_صف_Generic_Send_From_ISR( x_صف, pv_قلم_مقصد_صف, px_Higher_اولویت_Task_Woken, xCopyPosition );

    configASSERT( px_صف );
    configASSERT( !( ( pv_قلم_مقصد_صف == عدم ) && ( px_صف->ux_قلم_اندازه != ( UBaseType_t ) 0U ) ) );
    configASSERT( !( ( xCopyPosition == صف_OVERWRITE ) && ( px_صف->ux_درازی != 1 ) ) );

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
    پورت_ASSERT_IF_وقفه_اولویت_INVALID();

    /* Similar to xQueueGenericSend, except without blocking if there is no room
     * in the queue.  Also don't directly wake a task that was blocked on a queue
     * read, instead return a flag to say whether a context switch is required or
     * not (i.e. has a task with a higher priority than us been woken by this
     * post). */
    /* MISRA Ref 4.7.1 [Return value shall be checked] */
    /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
    /* coverity[misra_c_2012_directive_4_7_violation] */
    ux_Saved_وقفه_وضعیت = ( UBaseType_t ) taskENTER_CRITICAL_FROM_ISR();
    {
        إذا( ( px_صف->ux_Messages_انتظار < px_صف->ux_درازی ) || ( xCopyPosition == صف_OVERWRITE ) )
        {
            ثابت int8_t c_Tx_قفل = px_صف->c_Tx_قفل;
            ثابت UBaseType_t ux_قبلی_Messages_انتظار = px_صف->ux_Messages_انتظار;

            trace_صف_SEND_FROM_ISR( px_صف );

            /* Semaphores use xQueueGiveFromISR(), so pxQueue will not be a
             *  semaphore or mutex.  That means prvCopyDataToQueue() cannot result
             *  in a task disinheriting a priority and prvCopyDataToQueue() can be
             *  called here even though the disinherit function does not check if
             *  the scheduler is suspended before accessing the ready lists. */
            ( فراغ ) prv_Copy_داده_مقصد_صف( px_صف, pv_قلم_مقصد_صف, xCopyPosition );

            /* The event list is not altered if the queue is locked.  This will
             * be done when the queue is unlocked later. */
            إذا( c_Tx_قفل == صف_UNLOCKED )
            {
                #إذا ( config_USE_صف_SETS == 1 )
                {
                    إذا( px_صف->px_صف_مجموعه_Container != عدم )
                    {
                        إذا( ( xCopyPosition == صف_OVERWRITE ) && ( ux_قبلی_Messages_انتظار != ( UBaseType_t ) 0 ) )
                        {
                            /* Do not notify the queue set as an existing item
                             * was overwritten in the queue so the number of items
                             * in the queue has not changed. */
                            mtCOVERAGE_TEST_MARKER();
                        }
                        وإلا إذا( prv_Notify_صف_مجموعه_Container( px_صف ) != pdFALSE )
                        {
                            /* The queue is a member of a queue set, and posting
                             * to the queue set caused a higher priority task to
                             * unblock.  A context switch is required. */
                            إذا( px_Higher_اولویت_Task_Woken != عدم )
                            {
                                *px_Higher_اولویت_Task_Woken = pdTRUE;
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
                        إذا( list_LIST_IS_خالی( &( px_صف->x_Tasks_انتظار_مقصد_Receive ) ) == pdFALSE )
                        {
                            إذا( xTaskRemoveFromEventList( &( px_صف->x_Tasks_انتظار_مقصد_Receive ) ) != pdFALSE )
                            {
                                /* The task waiting has a higher priority so
                                 *  record that a context switch is required. */
                                إذا( px_Higher_اولویت_Task_Woken != عدم )
                                {
                                    *px_Higher_اولویت_Task_Woken = pdTRUE;
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
                    إذا( list_LIST_IS_خالی( &( px_صف->x_Tasks_انتظار_مقصد_Receive ) ) == pdFALSE )
                    {
                        إذا( xTaskRemoveFromEventList( &( px_صف->x_Tasks_انتظار_مقصد_Receive ) ) != pdFALSE )
                        {
                            /* The task waiting has a higher priority so record that a
                             * context switch is required. */
                            إذا( px_Higher_اولویت_Task_Woken != عدم )
                            {
                                *px_Higher_اولویت_Task_Woken = pdTRUE;
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
                    ( فراغ ) ux_قبلی_Messages_انتظار;
                }
                #پایان_اگر /* configUSE_QUEUE_SETS */
            }
            وإلا
            {
                /* Increment the lock count so the task that unlocks the queue
                 * knows that data was posted while it was locked. */
                prv_Increment_صف_Tx_قفل( px_صف, c_Tx_قفل );
            }

            xReturn = pdPASS;
        }
        وإلا
        {
            trace_صف_SEND_FROM_ISR_FAILED( px_صف );
            xReturn = err_صف_پر;
        }
    }
    taskEXIT_CRITICAL_FROM_ISR( ux_Saved_وقفه_وضعیت );

    trace_RETURN_x_صف_Generic_Send_From_ISR( xReturn );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

پایه_Type_t x_صف_Give_From_ISR( صف_Handle_t x_صف,
                              پایه_Type_t * ثابت px_Higher_اولویت_Task_Woken )
{
    پایه_Type_t xReturn;
    UBaseType_t ux_Saved_وقفه_وضعیت;
    صف_t * ثابت px_صف = x_صف;

    trace_ENTER_x_صف_Give_From_ISR( x_صف, px_Higher_اولویت_Task_Woken );

    /* Similar to xQueueGenericSendFromISR() but used with semaphores where the
     * item size is 0.  Don't directly wake a task that was blocked on a queue
     * read, instead return a flag to say whether a context switch is required or
     * not (i.e. has a task with a higher priority than us been woken by this
     * post). */

    configASSERT( px_صف );

    /* xQueueGenericSendFromISR() should be used instead of xQueueGiveFromISR()
     * if the item size is not 0. */
    configASSERT( px_صف->ux_قلم_اندازه == 0 );

    /* Normally a mutex would not be given from an interrupt, especially if
     * there is a mutex holder, as priority inheritance makes no sense for an
     * interrupts, only tasks. */
    configASSERT( !( ( px_صف->ux_صف_Type == صف_صف_IS_MUTEX ) && ( px_صف->u.xSemaphore.xMutexHolder != عدم ) ) );

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
    پورت_ASSERT_IF_وقفه_اولویت_INVALID();

    /* MISRA Ref 4.7.1 [Return value shall be checked] */
    /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
    /* coverity[misra_c_2012_directive_4_7_violation] */
    ux_Saved_وقفه_وضعیت = ( UBaseType_t ) taskENTER_CRITICAL_FROM_ISR();
    {
        ثابت UBaseType_t ux_Messages_انتظار = px_صف->ux_Messages_انتظار;

        /* When the queue is used to implement a semaphore no data is ever
         * moved through the queue but it is still valid to see if the queue 'has
         * space'. */
        إذا( ux_Messages_انتظار < px_صف->ux_درازی )
        {
            ثابت int8_t c_Tx_قفل = px_صف->c_Tx_قفل;

            trace_صف_SEND_FROM_ISR( px_صف );

            /* A task can only have an inherited priority if it is a mutex
             * holder - and if there is a mutex holder then the mutex cannot be
             * given from an ISR.  As this is the ISR version of the function it
             * can be assumed there is no mutex holder and no need to determine if
             * priority disinheritance is needed.  Simply increase the count of
             * messages (semaphores) available. */
            px_صف->ux_Messages_انتظار = ( UBaseType_t ) ( ux_Messages_انتظار + ( UBaseType_t ) 1 );

            /* The event list is not altered if the queue is locked.  This will
             * be done when the queue is unlocked later. */
            إذا( c_Tx_قفل == صف_UNLOCKED )
            {
                #إذا ( config_USE_صف_SETS == 1 )
                {
                    إذا( px_صف->px_صف_مجموعه_Container != عدم )
                    {
                        إذا( prv_Notify_صف_مجموعه_Container( px_صف ) != pdFALSE )
                        {
                            /* The semaphore is a member of a queue set, and
                             * posting to the queue set caused a higher priority
                             * task to unblock.  A context switch is required. */
                            إذا( px_Higher_اولویت_Task_Woken != عدم )
                            {
                                *px_Higher_اولویت_Task_Woken = pdTRUE;
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
                        إذا( list_LIST_IS_خالی( &( px_صف->x_Tasks_انتظار_مقصد_Receive ) ) == pdFALSE )
                        {
                            إذا( xTaskRemoveFromEventList( &( px_صف->x_Tasks_انتظار_مقصد_Receive ) ) != pdFALSE )
                            {
                                /* The task waiting has a higher priority so
                                 *  record that a context switch is required. */
                                إذا( px_Higher_اولویت_Task_Woken != عدم )
                                {
                                    *px_Higher_اولویت_Task_Woken = pdTRUE;
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
                    إذا( list_LIST_IS_خالی( &( px_صف->x_Tasks_انتظار_مقصد_Receive ) ) == pdFALSE )
                    {
                        إذا( xTaskRemoveFromEventList( &( px_صف->x_Tasks_انتظار_مقصد_Receive ) ) != pdFALSE )
                        {
                            /* The task waiting has a higher priority so record that a
                             * context switch is required. */
                            إذا( px_Higher_اولویت_Task_Woken != عدم )
                            {
                                *px_Higher_اولویت_Task_Woken = pdTRUE;
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
                #پایان_اگر /* configUSE_QUEUE_SETS */
            }
            وإلا
            {
                /* Increment the lock count so the task that unlocks the queue
                 * knows that data was posted while it was locked. */
                prv_Increment_صف_Tx_قفل( px_صف, c_Tx_قفل );
            }

            xReturn = pdPASS;
        }
        وإلا
        {
            trace_صف_SEND_FROM_ISR_FAILED( px_صف );
            xReturn = err_صف_پر;
        }
    }
    taskEXIT_CRITICAL_FROM_ISR( ux_Saved_وقفه_وضعیت );

    trace_RETURN_x_صف_Give_From_ISR( xReturn );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

پایه_Type_t x_صف_Receive( صف_Handle_t x_صف,
                          فراغ * ثابت pv_میانگیر,
                          TickType_t x_Ticks_مقصد_Wait )
{
    پایه_Type_t x_مدخل_زمان_مجموعه = pdFALSE;
    زمان_بیرون_t x_زمان_بیرون_2;
    صف_t * ثابت px_صف = x_صف;

    trace_ENTER_x_صف_Receive( x_صف, pv_میانگیر, x_Ticks_مقصد_Wait );

    /* Check the pointer is not NULL. */
    configASSERT( ( px_صف ) );

    /* The buffer into which data is received can only be NULL if the data size
     * is zero (so no data is copied into the buffer). */
    configASSERT( !( ( ( pv_میانگیر ) == عدم ) && ( ( px_صف )->ux_قلم_اندازه != ( UBaseType_t ) 0U ) ) );

    /* Cannot block if the scheduler is suspended. */
    #إذا ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_مقصد_Wait != 0 ) ) );
    }
    #پایان_اگر

    لكل( ; ; )
    {
        taskENTER_CRITICAL();
        {
            ثابت UBaseType_t ux_Messages_انتظار = px_صف->ux_Messages_انتظار;

            /* Is there data in the queue now?  To be running the calling task
             * must be the highest priority task wanting to access the queue. */
            إذا( ux_Messages_انتظار > ( UBaseType_t ) 0 )
            {
                /* Data available, remove one item. */
                prv_Copy_داده_From_صف( px_صف, pv_میانگیر );
                trace_صف_RECEIVE( px_صف );
                px_صف->ux_Messages_انتظار = ( UBaseType_t ) ( ux_Messages_انتظار - ( UBaseType_t ) 1 );

                /* There is now space in the queue, were any tasks waiting to
                 * post to the queue?  If so, unblock the highest priority waiting
                 * task. */
                إذا( list_LIST_IS_خالی( &( px_صف->x_Tasks_انتظار_مقصد_Send ) ) == pdFALSE )
                {
                    إذا( xTaskRemoveFromEventList( &( px_صف->x_Tasks_انتظار_مقصد_Send ) ) != pdFALSE )
                    {
                        صف_YIELD_IF_USING_PREEMPTION();
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

                trace_RETURN_x_صف_Receive( pdPASS );

                أرجع pdPASS;
            }
            وإلا
            {
                إذا( x_Ticks_مقصد_Wait == ( TickType_t ) 0 )
                {
                    /* The queue was empty and no block time is specified (or
                     * the block time has expired) so leave now. */
                    taskEXIT_CRITICAL();

                    trace_صف_RECEIVE_FAILED( px_صف );
                    trace_RETURN_x_صف_Receive( err_صف_خالی );

                    أرجع err_صف_خالی;
                }
                وإلا إذا( x_مدخل_زمان_مجموعه == pdFALSE )
                {
                    /* The queue was empty and a block time was specified so
                     * configure the timeout structure. */
                    v_Task_داخلی_مجموعه_زمان_بیرون_State( &x_زمان_بیرون_2 );
                    x_مدخل_زمان_مجموعه = pdTRUE;
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
        prv_قفل_صف( px_صف );

        /* Update the timeout state to see if it has expired yet. */
        إذا( x_Task_Check_For_زمان_بیرون( &x_زمان_بیرون_2, &x_Ticks_مقصد_Wait ) == pdFALSE )
        {
            /* The timeout has not expired.  If the queue is still empty place
             * the task on the list of tasks waiting to receive from the queue. */
            إذا( prv_Is_صف_خالی( px_صف ) != pdFALSE )
            {
                trace_BLOCKING_ON_صف_RECEIVE( px_صف );
                v_Task_قراربده_On_Event_List( &( px_صف->x_Tasks_انتظار_مقصد_Receive ), x_Ticks_مقصد_Wait );
                prv_Unlock_صف( px_صف );

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
                prv_Unlock_صف( px_صف );
                ( فراغ ) xTaskResumeAll();
            }
        }
        وإلا
        {
            /* Timed out.  If there is no data in the queue exit, otherwise loop
             * back and attempt to read the data. */
            prv_Unlock_صف( px_صف );
            ( فراغ ) xTaskResumeAll();

            إذا( prv_Is_صف_خالی( px_صف ) != pdFALSE )
            {
                trace_صف_RECEIVE_FAILED( px_صف );
                trace_RETURN_x_صف_Receive( err_صف_خالی );

                أرجع err_صف_خالی;
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
    }
}
/*-----------------------------------------------------------*/

پایه_Type_t x_صف_Semaphore_Take( صف_Handle_t x_صف,
                                TickType_t x_Ticks_مقصد_Wait )
{
    پایه_Type_t x_مدخل_زمان_مجموعه = pdFALSE;
    زمان_بیرون_t x_زمان_بیرون_2;
    صف_t * ثابت px_صف = x_صف;

    #إذا ( configUSE_MUTEXES == 1 )
        پایه_Type_t xInheritanceOccurred = pdFALSE;
    #پایان_اگر

    trace_ENTER_x_صف_Semaphore_Take( x_صف, x_Ticks_مقصد_Wait );

    /* Check the queue pointer is not NULL. */
    configASSERT( ( px_صف ) );

    /* Check this really is a semaphore, in which case the item size will be
     * 0. */
    configASSERT( px_صف->ux_قلم_اندازه == 0 );

    /* Cannot block if the scheduler is suspended. */
    #إذا ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_مقصد_Wait != 0 ) ) );
    }
    #پایان_اگر

    لكل( ; ; )
    {
        taskENTER_CRITICAL();
        {
            /* Semaphores are queues with an item size of 0, and where the
             * number of messages in the queue is the semaphore's count value. */
            ثابت UBaseType_t ux_Semaphore_شمار = px_صف->ux_Messages_انتظار;

            /* Is there data in the queue now?  To be running the calling task
             * must be the highest priority task wanting to access the queue. */
            إذا( ux_Semaphore_شمار > ( UBaseType_t ) 0 )
            {
                trace_صف_RECEIVE( px_صف );

                /* Semaphores are queues with a data size of zero and where the
                 * messages waiting is the semaphore's count.  Reduce the count. */
                px_صف->ux_Messages_انتظار = ( UBaseType_t ) ( ux_Semaphore_شمار - ( UBaseType_t ) 1 );

                #إذا ( configUSE_MUTEXES == 1 )
                {
                    إذا( px_صف->ux_صف_Type == صف_صف_IS_MUTEX )
                    {
                        /* Record the information required to implement
                         * priority inheritance should it become necessary. */
                        px_صف->u.xSemaphore.xMutexHolder = pv_Task_Increment_Mutex_Held_شمار();
                    }
                    وإلا
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #پایان_اگر /* configUSE_MUTEXES */

                /* Check to see if other tasks are blocked waiting to give the
                 * semaphore, and if so, unblock the highest priority such task. */
                إذا( list_LIST_IS_خالی( &( px_صف->x_Tasks_انتظار_مقصد_Send ) ) == pdFALSE )
                {
                    إذا( xTaskRemoveFromEventList( &( px_صف->x_Tasks_انتظار_مقصد_Send ) ) != pdFALSE )
                    {
                        صف_YIELD_IF_USING_PREEMPTION();
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

                trace_RETURN_x_صف_Semaphore_Take( pdPASS );

                أرجع pdPASS;
            }
            وإلا
            {
                إذا( x_Ticks_مقصد_Wait == ( TickType_t ) 0 )
                {
                    /* The semaphore count was 0 and no block time is specified
                     * (or the block time has expired) so exit now. */
                    taskEXIT_CRITICAL();

                    trace_صف_RECEIVE_FAILED( px_صف );
                    trace_RETURN_x_صف_Semaphore_Take( err_صف_خالی );

                    أرجع err_صف_خالی;
                }
                وإلا إذا( x_مدخل_زمان_مجموعه == pdFALSE )
                {
                    /* The semaphore count was 0 and a block time was specified
                     * so configure the timeout structure ready to block. */
                    v_Task_داخلی_مجموعه_زمان_بیرون_State( &x_زمان_بیرون_2 );
                    x_مدخل_زمان_مجموعه = pdTRUE;
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
        prv_قفل_صف( px_صف );

        /* Update the timeout state to see if it has expired yet. */
        إذا( x_Task_Check_For_زمان_بیرون( &x_زمان_بیرون_2, &x_Ticks_مقصد_Wait ) == pdFALSE )
        {
            /* A block time is specified and not expired.  If the semaphore
             * count is 0 then enter the Blocked state to wait for a semaphore to
             * become available.  As semaphores are implemented with queues the
             * queue being empty is equivalent to the semaphore count being 0. */
            إذا( prv_Is_صف_خالی( px_صف ) != pdFALSE )
            {
                trace_BLOCKING_ON_صف_RECEIVE( px_صف );

                #إذا ( configUSE_MUTEXES == 1 )
                {
                    إذا( px_صف->ux_صف_Type == صف_صف_IS_MUTEX )
                    {
                        taskENTER_CRITICAL();
                        {
                            xInheritanceOccurred = x_Task_اولویت_Inherit( px_صف->u.xSemaphore.xMutexHolder );
                        }
                        taskEXIT_CRITICAL();
                    }
                    وإلا
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #پایان_اگر /* if ( configUSE_MUTEXES == 1 ) */

                v_Task_قراربده_On_Event_List( &( px_صف->x_Tasks_انتظار_مقصد_Receive ), x_Ticks_مقصد_Wait );
                prv_Unlock_صف( px_صف );

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
                prv_Unlock_صف( px_صف );
                ( فراغ ) xTaskResumeAll();
            }
        }
        وإلا
        {
            /* Timed out. */
            prv_Unlock_صف( px_صف );
            ( فراغ ) xTaskResumeAll();

            /* If the semaphore count is 0 exit now as the timeout has
             * expired.  Otherwise return to attempt to take the semaphore that is
             * known to be available.  As semaphores are implemented by queues the
             * queue being empty is equivalent to the semaphore count being 0. */
            إذا( prv_Is_صف_خالی( px_صف ) != pdFALSE )
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
                            UBaseType_t ux_Highest_انتظار_اولویت;

                            /* This task blocking on the mutex caused another
                             * task to inherit this task's priority.  Now this task
                             * has timed out the priority should be disinherited
                             * again, but only as low as the next highest priority
                             * task that is waiting for the same mutex. */
                            ux_Highest_انتظار_اولویت = prv_Get_Disinherit_اولویت_After_Timeout( px_صف );

                            /* vTaskPriorityDisinheritAfterTimeout uses the uxHighestWaitingPriority
                             * parameter to index pxReadyTasksLists when adding the task holding
                             * mutex to the ready list for its new priority. Coverity thinks that
                             * it can result in out-of-bounds access which is not true because
                             * uxHighestWaitingPriority, as returned by prvGetDisinheritPriorityAfterTimeout,
                             * is capped at ( configMAX_PRIORITIES - 1 ). */
                            /* coverity[overrun] */
                            v_Task_اولویت_Disinherit_After_Timeout( px_صف->u.xSemaphore.xMutexHolder, ux_Highest_انتظار_اولویت );
                        }
                        taskEXIT_CRITICAL();
                    }
                }
                #پایان_اگر /* configUSE_MUTEXES */

                trace_صف_RECEIVE_FAILED( px_صف );
                trace_RETURN_x_صف_Semaphore_Take( err_صف_خالی );

                أرجع err_صف_خالی;
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
    }
}
/*-----------------------------------------------------------*/

پایه_Type_t x_صف_Peek( صف_Handle_t x_صف,
                       فراغ * ثابت pv_میانگیر,
                       TickType_t x_Ticks_مقصد_Wait )
{
    پایه_Type_t x_مدخل_زمان_مجموعه = pdFALSE;
    زمان_بیرون_t x_زمان_بیرون_2;
    int8_t * pcOriginalReadPosition;
    صف_t * ثابت px_صف = x_صف;

    trace_ENTER_x_صف_Peek( x_صف, pv_میانگیر, x_Ticks_مقصد_Wait );

    /* Check the pointer is not NULL. */
    configASSERT( ( px_صف ) );

    /* The buffer into which data is received can only be NULL if the data size
     * is zero (so no data is copied into the buffer. */
    configASSERT( !( ( ( pv_میانگیر ) == عدم ) && ( ( px_صف )->ux_قلم_اندازه != ( UBaseType_t ) 0U ) ) );

    /* Cannot block if the scheduler is suspended. */
    #إذا ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
    {
        configASSERT( !( ( xTaskGetSchedulerState() == taskSCHEDULER_SUSPENDED ) && ( x_Ticks_مقصد_Wait != 0 ) ) );
    }
    #پایان_اگر

    لكل( ; ; )
    {
        taskENTER_CRITICAL();
        {
            ثابت UBaseType_t ux_Messages_انتظار = px_صف->ux_Messages_انتظار;

            /* Is there data in the queue now?  To be running the calling task
             * must be the highest priority task wanting to access the queue. */
            إذا( ux_Messages_انتظار > ( UBaseType_t ) 0 )
            {
                /* Remember the read position so it can be reset after the data
                 * is read from the queue as this function is only peeking the
                 * data, not removing it. */
                pcOriginalReadPosition = px_صف->u.x_صف.pcReadFrom;

                prv_Copy_داده_From_صف( px_صف, pv_میانگیر );
                trace_صف_PEEK( px_صف );

                /* The data is not being removed, so reset the read pointer. */
                px_صف->u.x_صف.pcReadFrom = pcOriginalReadPosition;

                /* The data is being left in the queue, so see if there are
                 * any other tasks waiting for the data. */
                إذا( list_LIST_IS_خالی( &( px_صف->x_Tasks_انتظار_مقصد_Receive ) ) == pdFALSE )
                {
                    إذا( xTaskRemoveFromEventList( &( px_صف->x_Tasks_انتظار_مقصد_Receive ) ) != pdFALSE )
                    {
                        /* The task waiting has a higher priority than this task. */
                        صف_YIELD_IF_USING_PREEMPTION();
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

                trace_RETURN_x_صف_Peek( pdPASS );

                أرجع pdPASS;
            }
            وإلا
            {
                إذا( x_Ticks_مقصد_Wait == ( TickType_t ) 0 )
                {
                    /* The queue was empty and no block time is specified (or
                     * the block time has expired) so leave now. */
                    taskEXIT_CRITICAL();

                    trace_صف_PEEK_FAILED( px_صف );
                    trace_RETURN_x_صف_Peek( err_صف_خالی );

                    أرجع err_صف_خالی;
                }
                وإلا إذا( x_مدخل_زمان_مجموعه == pdFALSE )
                {
                    /* The queue was empty and a block time was specified so
                     * configure the timeout structure ready to enter the blocked
                     * state. */
                    v_Task_داخلی_مجموعه_زمان_بیرون_State( &x_زمان_بیرون_2 );
                    x_مدخل_زمان_مجموعه = pdTRUE;
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
        prv_قفل_صف( px_صف );

        /* Update the timeout state to see if it has expired yet. */
        إذا( x_Task_Check_For_زمان_بیرون( &x_زمان_بیرون_2, &x_Ticks_مقصد_Wait ) == pdFALSE )
        {
            /* Timeout has not expired yet, check to see if there is data in the
            * queue now, and if not enter the Blocked state to wait for data. */
            إذا( prv_Is_صف_خالی( px_صف ) != pdFALSE )
            {
                trace_BLOCKING_ON_صف_PEEK( px_صف );
                v_Task_قراربده_On_Event_List( &( px_صف->x_Tasks_انتظار_مقصد_Receive ), x_Ticks_مقصد_Wait );
                prv_Unlock_صف( px_صف );

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
                prv_Unlock_صف( px_صف );
                ( فراغ ) xTaskResumeAll();
            }
        }
        وإلا
        {
            /* The timeout has expired.  If there is still no data in the queue
             * exit, otherwise go back and try to read the data again. */
            prv_Unlock_صف( px_صف );
            ( فراغ ) xTaskResumeAll();

            إذا( prv_Is_صف_خالی( px_صف ) != pdFALSE )
            {
                trace_صف_PEEK_FAILED( px_صف );
                trace_RETURN_x_صف_Peek( err_صف_خالی );

                أرجع err_صف_خالی;
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
    }
}
/*-----------------------------------------------------------*/

پایه_Type_t x_صف_Receive_From_ISR( صف_Handle_t x_صف,
                                 فراغ * ثابت pv_میانگیر,
                                 پایه_Type_t * ثابت px_Higher_اولویت_Task_Woken )
{
    پایه_Type_t xReturn;
    UBaseType_t ux_Saved_وقفه_وضعیت;
    صف_t * ثابت px_صف = x_صف;

    trace_ENTER_x_صف_Receive_From_ISR( x_صف, pv_میانگیر, px_Higher_اولویت_Task_Woken );

    configASSERT( px_صف );
    configASSERT( !( ( pv_میانگیر == عدم ) && ( px_صف->ux_قلم_اندازه != ( UBaseType_t ) 0U ) ) );

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
    پورت_ASSERT_IF_وقفه_اولویت_INVALID();

    /* MISRA Ref 4.7.1 [Return value shall be checked] */
    /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
    /* coverity[misra_c_2012_directive_4_7_violation] */
    ux_Saved_وقفه_وضعیت = ( UBaseType_t ) taskENTER_CRITICAL_FROM_ISR();
    {
        ثابت UBaseType_t ux_Messages_انتظار = px_صف->ux_Messages_انتظار;

        /* Cannot block in an ISR, so check there is data available. */
        إذا( ux_Messages_انتظار > ( UBaseType_t ) 0 )
        {
            ثابت int8_t c_Rx_قفل = px_صف->c_Rx_قفل;

            trace_صف_RECEIVE_FROM_ISR( px_صف );

            prv_Copy_داده_From_صف( px_صف, pv_میانگیر );
            px_صف->ux_Messages_انتظار = ( UBaseType_t ) ( ux_Messages_انتظار - ( UBaseType_t ) 1 );

            /* If the queue is locked the event list will not be modified.
             * Instead update the lock count so the task that unlocks the queue
             * will know that an ISR has removed data while the queue was
             * locked. */
            إذا( c_Rx_قفل == صف_UNLOCKED )
            {
                إذا( list_LIST_IS_خالی( &( px_صف->x_Tasks_انتظار_مقصد_Send ) ) == pdFALSE )
                {
                    إذا( xTaskRemoveFromEventList( &( px_صف->x_Tasks_انتظار_مقصد_Send ) ) != pdFALSE )
                    {
                        /* The task waiting has a higher priority than us so
                         * force a context switch. */
                        إذا( px_Higher_اولویت_Task_Woken != عدم )
                        {
                            *px_Higher_اولویت_Task_Woken = pdTRUE;
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
                prv_Increment_صف_Rx_قفل( px_صف, c_Rx_قفل );
            }

            xReturn = pdPASS;
        }
        وإلا
        {
            xReturn = pdFAIL;
            trace_صف_RECEIVE_FROM_ISR_FAILED( px_صف );
        }
    }
    taskEXIT_CRITICAL_FROM_ISR( ux_Saved_وقفه_وضعیت );

    trace_RETURN_x_صف_Receive_From_ISR( xReturn );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

پایه_Type_t x_صف_Peek_From_ISR( صف_Handle_t x_صف,
                              فراغ * ثابت pv_میانگیر )
{
    پایه_Type_t xReturn;
    UBaseType_t ux_Saved_وقفه_وضعیت;
    int8_t * pcOriginalReadPosition;
    صف_t * ثابت px_صف = x_صف;

    trace_ENTER_x_صف_Peek_From_ISR( x_صف, pv_میانگیر );

    configASSERT( px_صف );
    configASSERT( !( ( pv_میانگیر == عدم ) && ( px_صف->ux_قلم_اندازه != ( UBaseType_t ) 0U ) ) );
    configASSERT( px_صف->ux_قلم_اندازه != 0 ); /* Can't peek a semaphore. */

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
    پورت_ASSERT_IF_وقفه_اولویت_INVALID();

    /* MISRA Ref 4.7.1 [Return value shall be checked] */
    /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#dir-47 */
    /* coverity[misra_c_2012_directive_4_7_violation] */
    ux_Saved_وقفه_وضعیت = ( UBaseType_t ) taskENTER_CRITICAL_FROM_ISR();
    {
        /* Cannot block in an ISR, so check there is data available. */
        إذا( px_صف->ux_Messages_انتظار > ( UBaseType_t ) 0 )
        {
            trace_صف_PEEK_FROM_ISR( px_صف );

            /* Remember the read position so it can be reset as nothing is
             * actually being removed from the queue. */
            pcOriginalReadPosition = px_صف->u.x_صف.pcReadFrom;
            prv_Copy_داده_From_صف( px_صف, pv_میانگیر );
            px_صف->u.x_صف.pcReadFrom = pcOriginalReadPosition;

            xReturn = pdPASS;
        }
        وإلا
        {
            xReturn = pdFAIL;
            trace_صف_PEEK_FROM_ISR_FAILED( px_صف );
        }
    }
    taskEXIT_CRITICAL_FROM_ISR( ux_Saved_وقفه_وضعیت );

    trace_RETURN_x_صف_Peek_From_ISR( xReturn );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

UBaseType_t ux_صف_Messages_انتظار( ثابت صف_Handle_t x_صف )
{
    UBaseType_t uxReturn;

    trace_ENTER_ux_صف_Messages_انتظار( x_صف );

    configASSERT( x_صف );

    taskENTER_CRITICAL();
    {
        uxReturn = ( ( صف_t * ) x_صف )->ux_Messages_انتظار;
    }
    taskEXIT_CRITICAL();

    trace_RETURN_ux_صف_Messages_انتظار( uxReturn );

    أرجع uxReturn;
}
/*-----------------------------------------------------------*/

UBaseType_t ux_صف_Spaces_موجود( ثابت صف_Handle_t x_صف )
{
    UBaseType_t uxReturn;
    صف_t * ثابت px_صف = x_صف;

    trace_ENTER_ux_صف_Spaces_موجود( x_صف );

    configASSERT( px_صف );

    taskENTER_CRITICAL();
    {
        uxReturn = ( UBaseType_t ) ( px_صف->ux_درازی - px_صف->ux_Messages_انتظار );
    }
    taskEXIT_CRITICAL();

    trace_RETURN_ux_صف_Spaces_موجود( uxReturn );

    أرجع uxReturn;
}
/*-----------------------------------------------------------*/

UBaseType_t ux_صف_Messages_انتظار_From_ISR( ثابت صف_Handle_t x_صف )
{
    UBaseType_t uxReturn;
    صف_t * ثابت px_صف = x_صف;

    trace_ENTER_ux_صف_Messages_انتظار_From_ISR( x_صف );

    configASSERT( px_صف );
    uxReturn = px_صف->ux_Messages_انتظار;

    trace_RETURN_ux_صف_Messages_انتظار_From_ISR( uxReturn );

    أرجع uxReturn;
}
/*-----------------------------------------------------------*/

فراغ v_صف_Delete( صف_Handle_t x_صف )
{
    صف_t * ثابت px_صف = x_صف;

    trace_ENTER_v_صف_Delete( x_صف );

    configASSERT( px_صف );
    trace_صف_DELETE( px_صف );

    #إذا ( config_صف_REGISTRY_اندازه > 0 )
    {
        v_صف_Unregister_صف( px_صف );
    }
    #پایان_اگر

    #إذا ( ( config_SUPPORT_DYNAMIC_تخصیص == 1 ) && ( config_SUPPORT_STATIC_تخصیص == 0 ) )
    {
        /* The queue can only have been allocated dynamically - free it
         * again. */
        v_پورت_Free( px_صف );
    }
    #وإلاإذا ( ( config_SUPPORT_DYNAMIC_تخصیص == 1 ) && ( config_SUPPORT_STATIC_تخصیص == 1 ) )
    {
        /* The queue could have been allocated statically or dynamically, so
         * check before attempting to free the memory. */
        إذا( px_صف->ucStaticallyAllocated == ( uint8_t ) pdFALSE )
        {
            v_پورت_Free( px_صف );
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
        ( فراغ ) px_صف;
    }
    #پایان_اگر /* configSUPPORT_DYNAMIC_ALLOCATION */

    trace_RETURN_v_صف_Delete();
}
/*-----------------------------------------------------------*/

#إذا ( configUSE_TRACE_FACILITY == 1 )

    UBaseType_t ux_صف_Get_صف_عدد( صف_Handle_t x_صف )
    {
        trace_ENTER_ux_صف_Get_صف_عدد( x_صف );

        trace_RETURN_ux_صف_Get_صف_عدد( ( ( صف_t * ) x_صف )->ux_صف_عدد );

        أرجع ( ( صف_t * ) x_صف )->ux_صف_عدد;
    }

#پایان_اگر /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#إذا ( configUSE_TRACE_FACILITY == 1 )

    فراغ v_صف_مجموعه_صف_عدد( صف_Handle_t x_صف,
                               UBaseType_t ux_صف_عدد )
    {
        trace_ENTER_v_صف_مجموعه_صف_عدد( x_صف, ux_صف_عدد );

        ( ( صف_t * ) x_صف )->ux_صف_عدد = ux_صف_عدد;

        trace_RETURN_v_صف_مجموعه_صف_عدد();
    }

#پایان_اگر /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

#إذا ( configUSE_TRACE_FACILITY == 1 )

    uint8_t uc_صف_Get_صف_Type( صف_Handle_t x_صف )
    {
        trace_ENTER_uc_صف_Get_صف_Type( x_صف );

        trace_RETURN_uc_صف_Get_صف_Type( ( ( صف_t * ) x_صف )->uc_صف_Type );

        أرجع ( ( صف_t * ) x_صف )->uc_صف_Type;
    }

#پایان_اگر /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

UBaseType_t ux_صف_Get_صف_قلم_اندازه( صف_Handle_t x_صف ) /* PRIVILEGED_FUNCTION */
{
    trace_ENTER_ux_صف_Get_صف_قلم_اندازه( x_صف );

    trace_RETURN_ux_صف_Get_صف_قلم_اندازه( ( ( صف_t * ) x_صف )->ux_قلم_اندازه );

    أرجع ( ( صف_t * ) x_صف )->ux_قلم_اندازه;
}
/*-----------------------------------------------------------*/

UBaseType_t ux_صف_Get_صف_درازی( صف_Handle_t x_صف ) /* PRIVILEGED_FUNCTION */
{
    trace_ENTER_ux_صف_Get_صف_درازی( x_صف );

    trace_RETURN_ux_صف_Get_صف_درازی( ( ( صف_t * ) x_صف )->ux_درازی );

    أرجع ( ( صف_t * ) x_صف )->ux_درازی;
}
/*-----------------------------------------------------------*/

#إذا ( configUSE_MUTEXES == 1 )

    ساكن UBaseType_t prv_Get_Disinherit_اولویت_After_Timeout( ثابت صف_t * ثابت px_صف )
    {
        UBaseType_t ux_Highest_اولویت_از_انتظار_Tasks;

        /* If a task waiting for a mutex causes the mutex holder to inherit a
         * priority, but the waiting task times out, then the holder should
         * disinherit the priority - but only down to the highest priority of any
         * other tasks that are waiting for the same mutex.  For this purpose,
         * return the priority of the highest priority task that is waiting for the
         * mutex. */
        إذا( list_CURRENT_LIST_درازی( &( px_صف->x_Tasks_انتظار_مقصد_Receive ) ) > 0U )
        {
            ux_Highest_اولویت_از_انتظار_Tasks = ( UBaseType_t ) ( ( UBaseType_t ) configMAX_PRIORITIES - ( UBaseType_t ) list_GET_قلم_مقدار_از_سر_مدخل( &( px_صف->x_Tasks_انتظار_مقصد_Receive ) ) );
        }
        وإلا
        {
            ux_Highest_اولویت_از_انتظار_Tasks = tsk_IDLE_اولویت;
        }

        أرجع ux_Highest_اولویت_از_انتظار_Tasks;
    }

#پایان_اگر /* configUSE_MUTEXES */
/*-----------------------------------------------------------*/

ساكن پایه_Type_t prv_Copy_داده_مقصد_صف( صف_t * ثابت px_صف,
                                      ثابت فراغ * pv_قلم_مقصد_صف,
                                      ثابت پایه_Type_t xPosition )
{
    پایه_Type_t xReturn = pdFALSE;
    UBaseType_t ux_Messages_انتظار;

    /* This function is called from a critical section. */

    ux_Messages_انتظار = px_صف->ux_Messages_انتظار;

    إذا( px_صف->ux_قلم_اندازه == ( UBaseType_t ) 0 )
    {
        #إذا ( configUSE_MUTEXES == 1 )
        {
            إذا( px_صف->ux_صف_Type == صف_صف_IS_MUTEX )
            {
                /* The mutex is no longer being held. */
                xReturn = x_Task_اولویت_Disinherit( px_صف->u.xSemaphore.xMutexHolder );
                px_صف->u.xSemaphore.xMutexHolder = عدم;
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        #پایان_اگر /* configUSE_MUTEXES */
    }
    وإلا إذا( xPosition == صف_SEND_مقصد_پشت )
    {
        ( فراغ ) کپی_حافظه( ( فراغ * ) px_صف->pc_Write_مقصد, pv_قلم_مقصد_صف, ( size_t ) px_صف->ux_قلم_اندازه );
        px_صف->pc_Write_مقصد += px_صف->ux_قلم_اندازه;

        إذا( px_صف->pc_Write_مقصد >= px_صف->u.x_صف.pc_دم )
        {
            px_صف->pc_Write_مقصد = px_صف->pc_سر;
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }
    }
    وإلا
    {
        ( فراغ ) کپی_حافظه( ( فراغ * ) px_صف->u.x_صف.pcReadFrom, pv_قلم_مقصد_صف, ( size_t ) px_صف->ux_قلم_اندازه );
        px_صف->u.x_صف.pcReadFrom -= px_صف->ux_قلم_اندازه;

        إذا( px_صف->u.x_صف.pcReadFrom < px_صف->pc_سر )
        {
            px_صف->u.x_صف.pcReadFrom = ( px_صف->u.x_صف.pc_دم - px_صف->ux_قلم_اندازه );
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }

        إذا( xPosition == صف_OVERWRITE )
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

    px_صف->ux_Messages_انتظار = ( UBaseType_t ) ( ux_Messages_انتظار + ( UBaseType_t ) 1 );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

ساكن فراغ prv_Copy_داده_From_صف( صف_t * ثابت px_صف,
                                  فراغ * ثابت pv_میانگیر )
{
    إذا( px_صف->ux_قلم_اندازه != ( UBaseType_t ) 0 )
    {
        px_صف->u.x_صف.pcReadFrom += px_صف->ux_قلم_اندازه;

        إذا( px_صف->u.x_صف.pcReadFrom >= px_صف->u.x_صف.pc_دم )
        {
            px_صف->u.x_صف.pcReadFrom = px_صف->pc_سر;
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }

        ( فراغ ) کپی_حافظه( ( فراغ * ) pv_میانگیر, ( فراغ * ) px_صف->u.x_صف.pcReadFrom, ( size_t ) px_صف->ux_قلم_اندازه );
    }
}
/*-----------------------------------------------------------*/

ساكن فراغ prv_Unlock_صف( صف_t * ثابت px_صف )
{
    /* THIS FUNCTION MUST BE CALLED WITH THE SCHEDULER SUSPENDED. */

    /* The lock counts contains the number of extra data items placed or
     * removed from the queue while the queue was locked.  When a queue is
     * locked items can be added or removed, but the event lists cannot be
     * updated. */
    taskENTER_CRITICAL();
    {
        int8_t c_Tx_قفل = px_صف->c_Tx_قفل;

        /* See if data was added to the queue while it was locked. */
        طالما( c_Tx_قفل > صف_LOCKED_UNMODIFIED )
        {
            /* Data was posted while the queue was locked.  Are any tasks
             * blocked waiting for data to become available? */
            #إذا ( config_USE_صف_SETS == 1 )
            {
                إذا( px_صف->px_صف_مجموعه_Container != عدم )
                {
                    إذا( prv_Notify_صف_مجموعه_Container( px_صف ) != pdFALSE )
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
                    إذا( list_LIST_IS_خالی( &( px_صف->x_Tasks_انتظار_مقصد_Receive ) ) == pdFALSE )
                    {
                        إذا( xTaskRemoveFromEventList( &( px_صف->x_Tasks_انتظار_مقصد_Receive ) ) != pdFALSE )
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
                إذا( list_LIST_IS_خالی( &( px_صف->x_Tasks_انتظار_مقصد_Receive ) ) == pdFALSE )
                {
                    إذا( xTaskRemoveFromEventList( &( px_صف->x_Tasks_انتظار_مقصد_Receive ) ) != pdFALSE )
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
            #پایان_اگر /* configUSE_QUEUE_SETS */

            --c_Tx_قفل;
        }

        px_صف->c_Tx_قفل = صف_UNLOCKED;
    }
    taskEXIT_CRITICAL();

    /* Do the same for the Rx lock. */
    taskENTER_CRITICAL();
    {
        int8_t c_Rx_قفل = px_صف->c_Rx_قفل;

        طالما( c_Rx_قفل > صف_LOCKED_UNMODIFIED )
        {
            إذا( list_LIST_IS_خالی( &( px_صف->x_Tasks_انتظار_مقصد_Send ) ) == pdFALSE )
            {
                إذا( xTaskRemoveFromEventList( &( px_صف->x_Tasks_انتظار_مقصد_Send ) ) != pdFALSE )
                {
                    vTaskMissedYield();
                }
                وإلا
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                --c_Rx_قفل;
            }
            وإلا
            {
                اكسر;
            }
        }

        px_صف->c_Rx_قفل = صف_UNLOCKED;
    }
    taskEXIT_CRITICAL();
}
/*-----------------------------------------------------------*/

ساكن پایه_Type_t prv_Is_صف_خالی( ثابت صف_t * px_صف )
{
    پایه_Type_t xReturn;

    taskENTER_CRITICAL();
    {
        إذا( px_صف->ux_Messages_انتظار == ( UBaseType_t ) 0 )
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

پایه_Type_t x_صف_Is_صف_خالی_From_ISR( ثابت صف_Handle_t x_صف )
{
    پایه_Type_t xReturn;
    صف_t * ثابت px_صف = x_صف;

    trace_ENTER_x_صف_Is_صف_خالی_From_ISR( x_صف );

    configASSERT( px_صف );

    إذا( px_صف->ux_Messages_انتظار == ( UBaseType_t ) 0 )
    {
        xReturn = pdTRUE;
    }
    وإلا
    {
        xReturn = pdFALSE;
    }

    trace_RETURN_x_صف_Is_صف_خالی_From_ISR( xReturn );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

ساكن پایه_Type_t prv_Is_صف_پر( ثابت صف_t * px_صف )
{
    پایه_Type_t xReturn;

    taskENTER_CRITICAL();
    {
        إذا( px_صف->ux_Messages_انتظار == px_صف->ux_درازی )
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

پایه_Type_t x_صف_Is_صف_پر_From_ISR( ثابت صف_Handle_t x_صف )
{
    پایه_Type_t xReturn;
    صف_t * ثابت px_صف = x_صف;

    trace_ENTER_x_صف_Is_صف_پر_From_ISR( x_صف );

    configASSERT( px_صف );

    إذا( px_صف->ux_Messages_انتظار == px_صف->ux_درازی )
    {
        xReturn = pdTRUE;
    }
    وإلا
    {
        xReturn = pdFALSE;
    }

    trace_RETURN_x_صف_Is_صف_پر_From_ISR( xReturn );

    أرجع xReturn;
}
/*-----------------------------------------------------------*/

#إذا ( configUSE_CO_ROUTINES == 1 )

    پایه_Type_t x_صف_CRSend( صف_Handle_t x_صف,
                             ثابت فراغ * pv_قلم_مقصد_صف,
                             TickType_t x_Ticks_مقصد_Wait )
    {
        پایه_Type_t xReturn;
        صف_t * ثابت px_صف = x_صف;

        trace_ENTER_x_صف_CRSend( x_صف, pv_قلم_مقصد_صف, x_Ticks_مقصد_Wait );

        /* If the queue is already full we may have to block.  A critical section
         * is required to prevent an interrupt removing something from the queue
         * between the check to see if the queue is full and blocking on the queue. */
        پورت_DISABLE_INTERRUPTS();
        {
            إذا( prv_Is_صف_پر( px_صف ) != pdFALSE )
            {
                /* The queue is full - do we want to block or just leave without
                 * posting? */
                إذا( x_Ticks_مقصد_Wait > ( TickType_t ) 0 )
                {
                    /* As this is called from a coroutine we cannot block directly, but
                     * return indicating that we need to block. */
                    v_Co_Routine_جمع_کن_مقصد_Delayed_List( x_Ticks_مقصد_Wait, &( px_صف->x_Tasks_انتظار_مقصد_Send ) );
                    پورت_ENABLE_INTERRUPTS();
                    أرجع err_صف_BLOCKED;
                }
                وإلا
                {
                    پورت_ENABLE_INTERRUPTS();
                    أرجع err_صف_پر;
                }
            }
        }
        پورت_ENABLE_INTERRUPTS();

        پورت_DISABLE_INTERRUPTS();
        {
            إذا( px_صف->ux_Messages_انتظار < px_صف->ux_درازی )
            {
                /* There is room in the queue, copy the data into the queue. */
                prv_Copy_داده_مقصد_صف( px_صف, pv_قلم_مقصد_صف, صف_SEND_مقصد_پشت );
                xReturn = pdPASS;

                /* Were any co-routines waiting for data to become available? */
                إذا( list_LIST_IS_خالی( &( px_صف->x_Tasks_انتظار_مقصد_Receive ) ) == pdFALSE )
                {
                    /* In this instance the co-routine could be placed directly
                     * into the ready list as we are within a critical section.
                     * Instead the same pending ready list mechanism is used as if
                     * the event were caused from within an interrupt. */
                    إذا( xCoRoutineRemoveFromEventList( &( px_صف->x_Tasks_انتظار_مقصد_Receive ) ) != pdFALSE )
                    {
                        /* The co-routine waiting has a higher priority so record
                         * that a yield might be appropriate. */
                        xReturn = err_صف_YIELD;
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
                xReturn = err_صف_پر;
            }
        }
        پورت_ENABLE_INTERRUPTS();

        trace_RETURN_x_صف_CRSend( xReturn );

        أرجع xReturn;
    }

#پایان_اگر /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#إذا ( configUSE_CO_ROUTINES == 1 )

    پایه_Type_t x_صف_CRReceive( صف_Handle_t x_صف,
                                فراغ * pv_میانگیر,
                                TickType_t x_Ticks_مقصد_Wait )
    {
        پایه_Type_t xReturn;
        صف_t * ثابت px_صف = x_صف;

        trace_ENTER_x_صف_CRReceive( x_صف, pv_میانگیر, x_Ticks_مقصد_Wait );

        /* If the queue is already empty we may have to block.  A critical section
         * is required to prevent an interrupt adding something to the queue
         * between the check to see if the queue is empty and blocking on the queue. */
        پورت_DISABLE_INTERRUPTS();
        {
            إذا( px_صف->ux_Messages_انتظار == ( UBaseType_t ) 0 )
            {
                /* There are no messages in the queue, do we want to block or just
                 * leave with nothing? */
                إذا( x_Ticks_مقصد_Wait > ( TickType_t ) 0 )
                {
                    /* As this is a co-routine we cannot block directly, but return
                     * indicating that we need to block. */
                    v_Co_Routine_جمع_کن_مقصد_Delayed_List( x_Ticks_مقصد_Wait, &( px_صف->x_Tasks_انتظار_مقصد_Receive ) );
                    پورت_ENABLE_INTERRUPTS();
                    أرجع err_صف_BLOCKED;
                }
                وإلا
                {
                    پورت_ENABLE_INTERRUPTS();
                    أرجع err_صف_پر;
                }
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        پورت_ENABLE_INTERRUPTS();

        پورت_DISABLE_INTERRUPTS();
        {
            إذا( px_صف->ux_Messages_انتظار > ( UBaseType_t ) 0 )
            {
                /* Data is available from the queue. */
                px_صف->u.x_صف.pcReadFrom += px_صف->ux_قلم_اندازه;

                إذا( px_صف->u.x_صف.pcReadFrom >= px_صف->u.x_صف.pc_دم )
                {
                    px_صف->u.x_صف.pcReadFrom = px_صف->pc_سر;
                }
                وإلا
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                --( px_صف->ux_Messages_انتظار );
                ( فراغ ) کپی_حافظه( ( فراغ * ) pv_میانگیر, ( فراغ * ) px_صف->u.x_صف.pcReadFrom, ( بیعلامت ) px_صف->ux_قلم_اندازه );

                xReturn = pdPASS;

                /* Were any co-routines waiting for space to become available? */
                إذا( list_LIST_IS_خالی( &( px_صف->x_Tasks_انتظار_مقصد_Send ) ) == pdFALSE )
                {
                    /* In this instance the co-routine could be placed directly
                     * into the ready list as we are within a critical section.
                     * Instead the same pending ready list mechanism is used as if
                     * the event were caused from within an interrupt. */
                    إذا( xCoRoutineRemoveFromEventList( &( px_صف->x_Tasks_انتظار_مقصد_Send ) ) != pdFALSE )
                    {
                        xReturn = err_صف_YIELD;
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
        پورت_ENABLE_INTERRUPTS();

        trace_RETURN_x_صف_CRReceive( xReturn );

        أرجع xReturn;
    }

#پایان_اگر /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#إذا ( configUSE_CO_ROUTINES == 1 )

    پایه_Type_t x_صف_CRSend_From_ISR( صف_Handle_t x_صف,
                                    ثابت فراغ * pv_قلم_مقصد_صف,
                                    پایه_Type_t xCoRoutinePreviouslyWoken )
    {
        صف_t * ثابت px_صف = x_صف;

        trace_ENTER_x_صف_CRSend_From_ISR( x_صف, pv_قلم_مقصد_صف, xCoRoutinePreviouslyWoken );

        /* Cannot block within an ISR so if there is no space on the queue then
         * exit without doing anything. */
        إذا( px_صف->ux_Messages_انتظار < px_صف->ux_درازی )
        {
            prv_Copy_داده_مقصد_صف( px_صف, pv_قلم_مقصد_صف, صف_SEND_مقصد_پشت );

            /* We only want to wake one co-routine per ISR, so check that a
             * co-routine has not already been woken. */
            إذا( xCoRoutinePreviouslyWoken == pdFALSE )
            {
                إذا( list_LIST_IS_خالی( &( px_صف->x_Tasks_انتظار_مقصد_Receive ) ) == pdFALSE )
                {
                    إذا( xCoRoutineRemoveFromEventList( &( px_صف->x_Tasks_انتظار_مقصد_Receive ) ) != pdFALSE )
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

        trace_RETURN_x_صف_CRSend_From_ISR( xCoRoutinePreviouslyWoken );

        أرجع xCoRoutinePreviouslyWoken;
    }

#پایان_اگر /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#إذا ( configUSE_CO_ROUTINES == 1 )

    پایه_Type_t x_صف_CRReceive_From_ISR( صف_Handle_t x_صف,
                                       فراغ * pv_میانگیر,
                                       پایه_Type_t * pxCoRoutineWoken )
    {
        پایه_Type_t xReturn;
        صف_t * ثابت px_صف = x_صف;

        trace_ENTER_x_صف_CRReceive_From_ISR( x_صف, pv_میانگیر, pxCoRoutineWoken );

        /* We cannot block from an ISR, so check there is data available. If
         * not then just leave without doing anything. */
        إذا( px_صف->ux_Messages_انتظار > ( UBaseType_t ) 0 )
        {
            /* Copy the data from the queue. */
            px_صف->u.x_صف.pcReadFrom += px_صف->ux_قلم_اندازه;

            إذا( px_صف->u.x_صف.pcReadFrom >= px_صف->u.x_صف.pc_دم )
            {
                px_صف->u.x_صف.pcReadFrom = px_صف->pc_سر;
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }

            --( px_صف->ux_Messages_انتظار );
            ( فراغ ) کپی_حافظه( ( فراغ * ) pv_میانگیر, ( فراغ * ) px_صف->u.x_صف.pcReadFrom, ( بیعلامت ) px_صف->ux_قلم_اندازه );

            إذا( ( *pxCoRoutineWoken ) == pdFALSE )
            {
                إذا( list_LIST_IS_خالی( &( px_صف->x_Tasks_انتظار_مقصد_Send ) ) == pdFALSE )
                {
                    إذا( xCoRoutineRemoveFromEventList( &( px_صف->x_Tasks_انتظار_مقصد_Send ) ) != pdFALSE )
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

        trace_RETURN_x_صف_CRReceive_From_ISR( xReturn );

        أرجع xReturn;
    }

#پایان_اگر /* configUSE_CO_ROUTINES */
/*-----------------------------------------------------------*/

#إذا ( config_صف_REGISTRY_اندازه > 0 )

    فراغ v_صف_جمع_کن_مقصد_Registry( صف_Handle_t x_صف,
                              ثابت حرف * pc_صف_Name )
    {
        UBaseType_t ux;
        صف_Registry_قلم_t * px_مدخل_مقصد_Write = عدم;

        trace_ENTER_v_صف_جمع_کن_مقصد_Registry( x_صف, pc_صف_Name );

        configASSERT( x_صف );

        إذا( pc_صف_Name != عدم )
        {
            /* See if there is an empty space in the registry.  A NULL name denotes
             * a free slot. */
            لكل( ux = ( UBaseType_t ) 0U; ux < ( UBaseType_t ) config_صف_REGISTRY_اندازه; ux++ )
            {
                /* Replace an existing entry if the queue is already in the registry. */
                إذا( x_صف == x_صف_Registry[ ux ].xHandle )
                {
                    px_مدخل_مقصد_Write = &( x_صف_Registry[ ux ] );
                    اكسر;
                }
                /* Otherwise, store in the next empty location */
                وإلا إذا( ( px_مدخل_مقصد_Write == عدم ) && ( x_صف_Registry[ ux ].pc_صف_Name == عدم ) )
                {
                    px_مدخل_مقصد_Write = &( x_صف_Registry[ ux ] );
                }
                وإلا
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
        }

        إذا( px_مدخل_مقصد_Write != عدم )
        {
            /* Store the information on this queue. */
            px_مدخل_مقصد_Write->pc_صف_Name = pc_صف_Name;
            px_مدخل_مقصد_Write->xHandle = x_صف;

            trace_صف_REGISTRY_جمع_کن( x_صف, pc_صف_Name );
        }

        trace_RETURN_v_صف_جمع_کن_مقصد_Registry();
    }

#پایان_اگر /* configQUEUE_REGISTRY_SIZE */
/*-----------------------------------------------------------*/

#إذا ( config_صف_REGISTRY_اندازه > 0 )

    ثابت حرف * pc_صف_Get_Name( صف_Handle_t x_صف )
    {
        UBaseType_t ux;
        ثابت حرف * pcReturn = عدم;

        trace_ENTER_pc_صف_Get_Name( x_صف );

        configASSERT( x_صف );

        /* Note there is nothing here to protect against another task adding or
         * removing entries from the registry while it is being searched. */

        لكل( ux = ( UBaseType_t ) 0U; ux < ( UBaseType_t ) config_صف_REGISTRY_اندازه; ux++ )
        {
            إذا( x_صف_Registry[ ux ].xHandle == x_صف )
            {
                pcReturn = x_صف_Registry[ ux ].pc_صف_Name;
                اكسر;
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }

        trace_RETURN_pc_صف_Get_Name( pcReturn );

        أرجع pcReturn;
    }

#پایان_اگر /* configQUEUE_REGISTRY_SIZE */
/*-----------------------------------------------------------*/

#إذا ( config_صف_REGISTRY_اندازه > 0 )

    فراغ v_صف_Unregister_صف( صف_Handle_t x_صف )
    {
        UBaseType_t ux;

        trace_ENTER_v_صف_Unregister_صف( x_صف );

        configASSERT( x_صف );

        /* See if the handle of the queue being unregistered in actually in the
         * registry. */
        لكل( ux = ( UBaseType_t ) 0U; ux < ( UBaseType_t ) config_صف_REGISTRY_اندازه; ux++ )
        {
            إذا( x_صف_Registry[ ux ].xHandle == x_صف )
            {
                /* Set the name to NULL to show that this slot if free again. */
                x_صف_Registry[ ux ].pc_صف_Name = عدم;

                /* Set the handle to NULL to ensure the same queue handle cannot
                 * appear in the registry twice if it is added, removed, then
                 * added again. */
                x_صف_Registry[ ux ].xHandle = ( صف_Handle_t ) 0;
                اكسر;
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }

        trace_RETURN_v_صف_Unregister_صف();
    }

#پایان_اگر /* configQUEUE_REGISTRY_SIZE */
/*-----------------------------------------------------------*/

#إذا ( configUSE_TIMERS == 1 )

    فراغ v_صف_Wait_For_پیام_Restricted( صف_Handle_t x_صف,
                                         TickType_t x_Ticks_مقصد_Wait,
                                         ثابت پایه_Type_t xWaitIndefinitely )
    {
        صف_t * ثابت px_صف = x_صف;

        trace_ENTER_v_صف_Wait_For_پیام_Restricted( x_صف, x_Ticks_مقصد_Wait, xWaitIndefinitely );

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
        prv_قفل_صف( px_صف );

        إذا( px_صف->ux_Messages_انتظار == ( UBaseType_t ) 0U )
        {
            /* There is nothing in the queue, block for the specified period. */
            v_Task_قراربده_On_Event_List_Restricted( &( px_صف->x_Tasks_انتظار_مقصد_Receive ), x_Ticks_مقصد_Wait, xWaitIndefinitely );
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }

        prv_Unlock_صف( px_صف );

        trace_RETURN_v_صف_Wait_For_پیام_Restricted();
    }

#پایان_اگر /* configUSE_TIMERS */
/*-----------------------------------------------------------*/

#إذا ( ( config_USE_صف_SETS == 1 ) && ( config_SUPPORT_DYNAMIC_تخصیص == 1 ) )

    صف_مجموعه_Handle_t x_صف_Create_مجموعه( ثابت UBaseType_t ux_Event_صف_درازی )
    {
        صف_مجموعه_Handle_t px_صف;

        trace_ENTER_x_صف_Create_مجموعه( ux_Event_صف_درازی );

        px_صف = x_صف_Generic_Create( ux_Event_صف_درازی, ( UBaseType_t ) حجم( صف_t * ), صف_صف_TYPE_مجموعه );

        trace_RETURN_x_صف_Create_مجموعه( px_صف );

        أرجع px_صف;
    }

#پایان_اگر /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#إذا ( config_USE_صف_SETS == 1 )

    پایه_Type_t x_صف_جمع_کن_مقصد_مجموعه( صف_مجموعه_Member_Handle_t x_صف_Or_Semaphore,
                               صف_مجموعه_Handle_t x_صف_مجموعه )
    {
        پایه_Type_t xReturn;

        trace_ENTER_x_صف_جمع_کن_مقصد_مجموعه( x_صف_Or_Semaphore, x_صف_مجموعه );

        taskENTER_CRITICAL();
        {
            إذا( ( ( صف_t * ) x_صف_Or_Semaphore )->px_صف_مجموعه_Container != عدم )
            {
                /* Cannot add a queue/semaphore to more than one queue set. */
                xReturn = pdFAIL;
            }
            وإلا إذا( ( ( صف_t * ) x_صف_Or_Semaphore )->ux_Messages_انتظار != ( UBaseType_t ) 0 )
            {
                /* Cannot add a queue/semaphore to a queue set if there are already
                 * items in the queue/semaphore. */
                xReturn = pdFAIL;
            }
            وإلا
            {
                ( ( صف_t * ) x_صف_Or_Semaphore )->px_صف_مجموعه_Container = x_صف_مجموعه;
                xReturn = pdPASS;
            }
        }
        taskEXIT_CRITICAL();

        trace_RETURN_x_صف_جمع_کن_مقصد_مجموعه( xReturn );

        أرجع xReturn;
    }

#پایان_اگر /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#إذا ( config_USE_صف_SETS == 1 )

    پایه_Type_t x_صف_Remove_From_مجموعه( صف_مجموعه_Member_Handle_t x_صف_Or_Semaphore,
                                    صف_مجموعه_Handle_t x_صف_مجموعه )
    {
        پایه_Type_t xReturn;
        صف_t * ثابت px_صف_Or_Semaphore = ( صف_t * ) x_صف_Or_Semaphore;

        trace_ENTER_x_صف_Remove_From_مجموعه( x_صف_Or_Semaphore, x_صف_مجموعه );

        إذا( px_صف_Or_Semaphore->px_صف_مجموعه_Container != x_صف_مجموعه )
        {
            /* The queue was not a member of the set. */
            xReturn = pdFAIL;
        }
        وإلا إذا( px_صف_Or_Semaphore->ux_Messages_انتظار != ( UBaseType_t ) 0 )
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
                px_صف_Or_Semaphore->px_صف_مجموعه_Container = عدم;
            }
            taskEXIT_CRITICAL();
            xReturn = pdPASS;
        }

        trace_RETURN_x_صف_Remove_From_مجموعه( xReturn );

        أرجع xReturn;
    }

#پایان_اگر /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#إذا ( config_USE_صف_SETS == 1 )

    صف_مجموعه_Member_Handle_t x_صف_Select_From_مجموعه( صف_مجموعه_Handle_t x_صف_مجموعه,
                                                TickType_t ثابت x_Ticks_مقصد_Wait )
    {
        صف_مجموعه_Member_Handle_t xReturn = عدم;

        trace_ENTER_x_صف_Select_From_مجموعه( x_صف_مجموعه, x_Ticks_مقصد_Wait );

        ( فراغ ) x_صف_Receive( ( صف_Handle_t ) x_صف_مجموعه, &xReturn, x_Ticks_مقصد_Wait );

        trace_RETURN_x_صف_Select_From_مجموعه( xReturn );

        أرجع xReturn;
    }

#پایان_اگر /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#إذا ( config_USE_صف_SETS == 1 )

    صف_مجموعه_Member_Handle_t x_صف_Select_From_مجموعه_From_ISR( صف_مجموعه_Handle_t x_صف_مجموعه )
    {
        صف_مجموعه_Member_Handle_t xReturn = عدم;

        trace_ENTER_x_صف_Select_From_مجموعه_From_ISR( x_صف_مجموعه );

        ( فراغ ) x_صف_Receive_From_ISR( ( صف_Handle_t ) x_صف_مجموعه, &xReturn, عدم );

        trace_RETURN_x_صف_Select_From_مجموعه_From_ISR( xReturn );

        أرجع xReturn;
    }

#پایان_اگر /* configUSE_QUEUE_SETS */
/*-----------------------------------------------------------*/

#إذا ( config_USE_صف_SETS == 1 )

    ساكن پایه_Type_t prv_Notify_صف_مجموعه_Container( ثابت صف_t * ثابت px_صف )
    {
        صف_t * px_صف_مجموعه_Container = px_صف->px_صف_مجموعه_Container;
        پایه_Type_t xReturn = pdFALSE;

        /* This function must be called form a critical section. */

        /* The following line is not reachable in unit tests because every call
         * to prvNotifyQueueSetContainer is preceded by a check that
         * pxQueueSetContainer != NULL */
        configASSERT( px_صف_مجموعه_Container ); /* LCOV_EXCL_BR_LINE */
        configASSERT( px_صف_مجموعه_Container->ux_Messages_انتظار < px_صف_مجموعه_Container->ux_درازی );

        إذا( px_صف_مجموعه_Container->ux_Messages_انتظار < px_صف_مجموعه_Container->ux_درازی )
        {
            ثابت int8_t c_Tx_قفل = px_صف_مجموعه_Container->c_Tx_قفل;

            trace_صف_مجموعه_SEND( px_صف_مجموعه_Container );

            /* The data copied is the handle of the queue that contains data. */
            xReturn = prv_Copy_داده_مقصد_صف( px_صف_مجموعه_Container, &px_صف, صف_SEND_مقصد_پشت );

            إذا( c_Tx_قفل == صف_UNLOCKED )
            {
                إذا( list_LIST_IS_خالی( &( px_صف_مجموعه_Container->x_Tasks_انتظار_مقصد_Receive ) ) == pdFALSE )
                {
                    إذا( xTaskRemoveFromEventList( &( px_صف_مجموعه_Container->x_Tasks_انتظار_مقصد_Receive ) ) != pdFALSE )
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
                prv_Increment_صف_Tx_قفل( px_صف_مجموعه_Container, c_Tx_قفل );
            }
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }

        أرجع xReturn;
    }

#پایان_اگر /* configUSE_QUEUE_SETS */
