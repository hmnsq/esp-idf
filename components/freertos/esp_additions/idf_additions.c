/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * This file contains the implementation for some the functions in
 * idf_additions.h
 */

#অন্তর্ভাব "sdkconfig.h"
#অন্তর্ভাব <stdint.h>
#অন্তর্ভাব "freertos/FreeRTOS.h"
#অন্তর্ভাব "freertos/task.h"
#অন্তর্ভাব "freertos/queue.h"
#অন্তর্ভাব "freertos/semphr.h"
#অন্তর্ভাব "freertos/stream_buffer.h"
#অন্তর্ভাব "freertos/message_buffer.h"
#অন্তর্ভাব "freertos/idf_additions.h"
#অন্তর্ভাব "esp_heap_caps.h"
#অন্তর্ভাব "esp_log.h"
#অন্তর্ভাব "freertos/portmacro.h"

/* -------------------------------------------- Creation With Memory Caps ------------------------------------------- */

/* ---------------------------------- Tasks --------------------------------- */

#যদি ( configSUPPORT_STATIC_ALLOCATION == 1 )

    BaseType_t xTaskCreatePinnedToCoreWithCaps( TaskFunction_t pvTaskCode,
                                                ধ্রুবক অক্ষর * ধ্রুবক pcName,
                                                ধ্রুবক config_স্তূপ_DEPTH_TYPE us_স্তূপ_Depth,
                                                শূন্য * ধ্রুবক pvParameters,
                                                UBaseType_t uxPriority,
                                                TaskHandle_t * ধ্রুবক pvCreatedTask,
                                                ধ্রুবক BaseType_t xCoreID,
                                                UBaseType_t uxMemoryCaps )
    {
        TaskHandle_t xHandle;
        StaticTask_t * px_Task_বাফার;
        স্তূপ_Type_t * px_স্তূপ;

        /* Allocate memory for the task's TCB. We use pvPortMalloc() here as the
         * TCB must be in internal memory. */
        px_Task_বাফার = pvPortMalloc( আকার( StaticTask_t ) );

        /* Allocate memory for the task's stack using the provided memory caps
         * */
        px_স্তূপ = heap_caps_malloc( ( ( size_t ) us_স্তূপ_Depth ) * আকার( স্তূপ_Type_t ), ( uint32_t ) uxMemoryCaps );

        যদি( ( px_Task_বাফার == ফাঁকা ) || ( px_স্তূপ == ফাঁকা ) )
        {
            যাও err;
        }

        /* Create the task using static creation API*/
        xHandle = xTaskCreateStaticPinnedToCore( pvTaskCode, pcName, us_স্তূপ_Depth, pvParameters, uxPriority, px_স্তূপ, px_Task_বাফার, xCoreID );

        যদি( xHandle == ফাঁকা )
        {
            /* Failed to create task */
            যাও err;
        }
        অন্যথা যদি( pvCreatedTask != ফাঁকা )
        {
            /* Task created successfully. Return the task handle */
            *pvCreatedTask = xHandle;
        }

        ফেরত pdPASS;

err:
        heap_caps_free( px_স্তূপ );
        vPortFree( px_Task_বাফার );
        ফেরত pdFAIL;
    }

#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#যদি ( configSUPPORT_STATIC_ALLOCATION == 1 )

    স্থির শূন্য prvTaskDeleteWithCaps( TaskHandle_t xTaskToDelete )
    {
        /* Return value unused if asserts are disabled */
        BaseType_t __attribute__( ( unused ) ) x_ফলাফল;
        StaticTask_t * px_Task_বাফার;
        স্তূপ_Type_t * pux_স্তূপ_বাফার;

        /* The task to be deleted must not be running.
         * So we suspend the task before deleting it. */
        vTaskSuspend( xTaskToDelete );

        /* Wait until the task is no longer the current task on any core.
         *
         * Polling on pxCurrentTCBs[] (via xTaskGetCurrentTaskHandleForCore())
         * matches the predicate vTaskDelete() uses to choose between immediate
         * and IDLE-deferred TCB cleanup. By waiting on it here, we guarantee
         * that the vTaskDelete() call below takes the immediate path and that
         * prvDeleteTCB() runs synchronously before vTaskDelete() returns, so
         * the heap_caps_free() / vPortFree() that follow cannot race with the
         * IDLE task. */
        জন্য( ;; )
        {
            BaseType_t xStillOnCore = pdFALSE;

            জন্য( BaseType_t xCoreID = 0; xCoreID < config_সংখ্যা_OF_CORES; xCoreID++ )
            {
                যদি( xTaskGetCurrentTaskHandleForCore( xCoreID ) == xTaskToDelete )
                {
                    xStillOnCore = pdTRUE;
                    বিরতি;
                }
            }

            যদি( xStillOnCore == pdFALSE )
            {
                বিরতি;
            }

            taskYIELD();
        }

        /* We can delete the task and free the memory buffers.
         * First, we must call `vTaskDelete` so that the port task delete callback is called.
         * On targets that have coprocessors, it may be possible that the stack pointer is modified (restored)
         * during this phase, hence, it must be done before getting the statuc buffers out of the task. */
        vTaskDelete( xTaskToDelete );

        /* Free the memory buffers */
        x_ফলাফল = xTaskGetStaticBuffers( xTaskToDelete, &pux_স্তূপ_বাফার, &px_Task_বাফার );
        configASSERT( x_ফলাফল == pdTRUE );
        configASSERT( pux_স্তূপ_বাফার != ফাঁকা );
        configASSERT( px_Task_বাফার != ফাঁকা );

        heap_caps_free( pux_স্তূপ_বাফার );
        vPortFree( px_Task_বাফার );
    }

    স্থির শূন্য prvTaskDeleteWithCapsTask( শূন্য * pvParameters )
    {
        TaskHandle_t xTaskToDelete = ( TaskHandle_t ) pvParameters;

        /* Delete the WithCaps task */
        prvTaskDeleteWithCaps( xTaskToDelete );

        /* Delete the temporary clean up task */
        vTaskDelete( ফাঁকা );
    }

    শূন্য vTaskDeleteWithCaps( TaskHandle_t xTaskToDelete )
    {
        /* THIS FUNCTION SHOULD NOT BE CALLED FROM AN INTERRUPT CONTEXT. */
        /* TODO: Update it to use portASSERT_IF_IN_ISR() instead. (IDF-10540) */
        vPortAssertIfInISR();

        TaskHandle_t xCurrentTaskHandle = xTaskGetCurrentTaskHandle();
        configASSERT( xCurrentTaskHandle != ফাঁকা );

        যদি( ( xTaskToDelete == ফাঁকা ) || ( xTaskToDelete == xCurrentTaskHandle ) )
        {
            /* The WithCaps task is deleting itself. While, the task can put itself on the
             * xTasksWaitingTermination list via the vTaskDelete() call, the idle
             * task will not free the task TCB and stack memories we created statically
             * during xTaskCreateWithCaps() or xTaskCreatePinnedToCoreWithCaps(). This
             * task will never be rescheduled once it is on the xTasksWaitingTermination
             * list and will not be able to clear the memories. Therefore, it will leak memory.
             *
             * To avoid this, we create a new "temporary clean up" task to delete the current task.
             * This task is created at the priority of the task to be deleted with the same core
             * affitinty. Its limited purpose is to delete the self-deleting task created WithCaps.
             *
             * This approach has the following problems -
             * 1. Once a WithCaps task deletes itself via vTaskDeleteWithCaps(), it may end up in the
             *    suspended tasks lists for a short time before being deleted. This can give an incorrect
             *    picture about the system state.
             *
             * 2. This approach is wasteful and can be error prone. The temporary clean up task will need
             *    system resources to get scheduled and cleanup the WithCaps task. It can be a problem if the system
             *    has several self-deleting WithCaps tasks.
             *
             * TODO: A better approach could be either -
             *
             * 1. Delegate memory management to the application/user. This way the kernel needn't bother about freeing
             *    the memory (like other static memory task creation APIs like xTaskCreateStatic()) (IDF-10521)
             *
             * 2. Have a post deletion hook/callback from the IDLE task to notify higher layers when it is safe to
             *    perform activities such as clearing up the TCB and stack memories. (IDF-10522) */
            যদি( xTaskCreatePinnedToCore( ( TaskFunction_t ) prvTaskDeleteWithCapsTask, "prvTaskDeleteWithCapsTask", config_MINIMAL_স্তূপ_আকার, xCurrentTaskHandle, uxTaskPriorityGet( xTaskToDelete ), ফাঁকা, xPortGetCoreID() ) != pdFAIL )
            {
                /* Although the current task should get preemted immediately when prvTaskDeleteWithCapsTask is created,
                 * for safety, we suspend the current task and wait for prvTaskDeleteWithCapsTask to delete it. */
                vTaskSuspend( xTaskToDelete );

                /* Should never reach here */
                ESP_LOGE( "freertos_additions", "%s: Failed to suspend the task to be deleted", __func__ );
                abort();
            }
            অন্যথা
            {
                /* Failed to create the task to delete the current task. */
                ESP_LOGE( "freertos_additions", "%s: Failed to create the task to delete the current task", __func__ );
                abort();
            }
        }

        /* Delete the WithCaps task */
        prvTaskDeleteWithCaps( xTaskToDelete );
    }

#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

/* ---------------------------------- Queue --------------------------------- */

#যদি ( configSUPPORT_STATIC_ALLOCATION == 1 )

    QueueHandle_t xQueueCreateWithCaps( UBaseType_t ux_Queue_দৈর্ঘ্য,
                                        UBaseType_t ux_Item_আকার,
                                        UBaseType_t uxMemoryCaps )
    {
        QueueHandle_t xQueue;
        StaticQueue_t * px_Queue_বাফার;
        uint8_t * puc_Queue_Storage_বাফার;

        /* Allocate memory for the queue using the provided memory caps */
        px_Queue_বাফার = heap_caps_malloc( আকার( StaticQueue_t ), ( uint32_t ) uxMemoryCaps );

        যদি( ux_Item_আকার == 0 )
        {
            puc_Queue_Storage_বাফার = ফাঁকা;
        }
        অন্যথা
        {
            puc_Queue_Storage_বাফার = heap_caps_malloc( ux_Queue_দৈর্ঘ্য * ux_Item_আকার, ( uint32_t ) uxMemoryCaps );
        }

        যদি( ( px_Queue_বাফার == ফাঁকা ) || ( ( ux_Item_আকার > 0 ) && ( puc_Queue_Storage_বাফার == ফাঁকা ) ) )
        {
            যাও err;
        }

        /* Create the queue using static creation API */
        xQueue = xQueueCreateStatic( ux_Queue_দৈর্ঘ্য, ux_Item_আকার, puc_Queue_Storage_বাফার, px_Queue_বাফার );

        যদি( xQueue == ফাঁকা )
        {
            যাও err;
        }

        ফেরত xQueue;

err:
        heap_caps_free( puc_Queue_Storage_বাফার );
        heap_caps_free( px_Queue_বাফার );
        ফেরত ফাঁকা;
    }

#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#যদি ( configSUPPORT_STATIC_ALLOCATION == 1 )

    শূন্য vQueueDeleteWithCaps( QueueHandle_t xQueue )
    {
        /* Return value unused if asserts are disabled */
        BaseType_t __attribute__( ( unused ) ) x_ফলাফল;
        StaticQueue_t * px_Queue_বাফার;
        uint8_t * puc_Queue_Storage_বাফার;

        /* Retrieve the buffers used to create the queue before deleting it */
        x_ফলাফল = xQueueGetStaticBuffers( xQueue, &puc_Queue_Storage_বাফার, &px_Queue_বাফার );
        configASSERT( x_ফলাফল == pdTRUE );

        /* Delete the queue */
        vQueueDelete( xQueue );

        /* Free the memory buffers */
        heap_caps_free( px_Queue_বাফার );
        heap_caps_free( puc_Queue_Storage_বাফার );
    }

#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

/* -------------------------------- Semaphore ------------------------------- */

#যদি ( configSUPPORT_STATIC_ALLOCATION == 1 )

    SemaphoreHandle_t xSemaphoreCreateGenericWithCaps( UBaseType_t ux_Max_গুনতি,
                                                       UBaseType_t ux_Initial_গুনতি,
                                                       ধ্রুবক uint8_t ucQueueType,
                                                       UBaseType_t uxMemoryCaps )
    {
        SemaphoreHandle_t xSemaphore;
        StaticSemaphore_t * px_Semaphore_বাফার;

        /* Allocate memory for the semaphore using the provided memory caps */
        px_Semaphore_বাফার = heap_caps_malloc( আকার( StaticSemaphore_t ), ( uint32_t ) uxMemoryCaps );

        যদি( px_Semaphore_বাফার == ফাঁকা )
        {
            ফেরত ফাঁকা;
        }

        /* Create the semaphore using static creation API */
        যদি( ucQueueType == queueQUEUE_TYPE_MUTEX )
        {
            xSemaphore = xSemaphoreCreateMutexStatic( px_Semaphore_বাফার );
        }
        অন্যথা যদি( ucQueueType == queueQUEUE_TYPE_COUNTING_SEMAPHORE )
        {
            xSemaphore = xSemaphoreCreateCountingStatic( ux_Max_গুনতি, ux_Initial_গুনতি, px_Semaphore_বাফার );
        }
        অন্যথা যদি( ucQueueType == queue_QUEUE_TYPE_দ্বিমিক_SEMAPHORE )
        {
            xSemaphore = x_Semaphore_Create_দ্বিমিক_Static( px_Semaphore_বাফার );
        }
        অন্যথা /* ucQueueType == queueQUEUE_TYPE_RECURSIVE_MUTEX */
        {
            xSemaphore = xSemaphoreCreateRecursiveMutexStatic( px_Semaphore_বাফার );
        }

        যদি( xSemaphore == ফাঁকা )
        {
            heap_caps_free( px_Semaphore_বাফার );
        }

        ফেরত xSemaphore;
    }

#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#যদি ( configSUPPORT_STATIC_ALLOCATION == 1 )

    শূন্য vSemaphoreDeleteWithCaps( SemaphoreHandle_t xSemaphore )
    {
        /* Return value unused if asserts are disabled */
        BaseType_t __attribute__( ( unused ) ) x_ফলাফল;
        StaticSemaphore_t * px_Semaphore_বাফার;

        /* Retrieve the buffer used to create the semaphore before deleting it
         * */
        x_ফলাফল = x_Semaphore_Get_Static_বাফার( xSemaphore, &px_Semaphore_বাফার );
        configASSERT( x_ফলাফল == pdTRUE );

        /* Delete the semaphore */
        vSemaphoreDelete( xSemaphore );

        /* Free the memory buffer */
        heap_caps_free( px_Semaphore_বাফার );
    }

#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

/* ------------------------- Stream & Message Buffers ----------------------- */

#যদি ( configSUPPORT_STATIC_ALLOCATION == 1 )

    Stream_বাফার_Handle_t x_Stream_বাফার_Generic_Create_With_Caps( size_t x_বাফার_আকার_Bytes,
                                                             size_t xTriggerLevelBytes,
                                                             BaseType_t x_Is_Message_বাফার,
                                                             UBaseType_t uxMemoryCaps )
    {
        Stream_বাফার_Handle_t x_Stream_বাফার;
        Static_Stream_বাফার_t * px_Static_Stream_বাফার;
        uint8_t * puc_Stream_বাফার_Storage_Area;

        /* Allocate memory for the stream or message buffer using the provided
         * memory caps */
        px_Static_Stream_বাফার = heap_caps_malloc( আকার( Static_Stream_বাফার_t ), ( uint32_t ) uxMemoryCaps );
        puc_Stream_বাফার_Storage_Area = heap_caps_malloc( x_বাফার_আকার_Bytes, ( uint32_t ) uxMemoryCaps );

        যদি( ( px_Static_Stream_বাফার == ফাঁকা ) || ( puc_Stream_বাফার_Storage_Area == ফাঁকা ) )
        {
            যাও err;
        }

        /* Create the stream or message buffer using static creation API */
        যদি( x_Is_Message_বাফার == pdTRUE )
        {
            x_Stream_বাফার = ( Stream_বাফার_Handle_t ) x_Message_বাফার_Create_Static( x_বাফার_আকার_Bytes, puc_Stream_বাফার_Storage_Area, px_Static_Stream_বাফার );
        }
        অন্যথা
        {
            x_Stream_বাফার = x_Stream_বাফার_Create_Static( x_বাফার_আকার_Bytes, xTriggerLevelBytes, puc_Stream_বাফার_Storage_Area, px_Static_Stream_বাফার );
        }

        যদি( x_Stream_বাফার == ফাঁকা )
        {
            যাও err;
        }

        ফেরত x_Stream_বাফার;

err:
        heap_caps_free( puc_Stream_বাফার_Storage_Area );
        heap_caps_free( px_Static_Stream_বাফার );
        ফেরত ফাঁকা;
    }

#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#যদি ( configSUPPORT_STATIC_ALLOCATION == 1 )

    শূন্য v_Stream_বাফার_Generic_Delete_With_Caps( Stream_বাফার_Handle_t x_Stream_বাফার,
                                             BaseType_t x_Is_Message_বাফার )
    {
        /* Return value unused if asserts are disabled */
        BaseType_t __attribute__( ( unused ) ) x_ফলাফল;
        Static_Stream_বাফার_t * px_Static_Stream_বাফার;
        uint8_t * puc_Stream_বাফার_Storage_Area;

        /* Retrieve the buffers used to create the stream or message buffer
         * before deleting it */
        যদি( x_Is_Message_বাফার == pdTRUE )
        {
            x_ফলাফল = x_Message_বাফার_Get_Static_Buffers( x_Stream_বাফার, &puc_Stream_বাফার_Storage_Area, &px_Static_Stream_বাফার );
        }
        অন্যথা
        {
            x_ফলাফল = x_Stream_বাফার_Get_Static_Buffers( x_Stream_বাফার, &puc_Stream_বাফার_Storage_Area, &px_Static_Stream_বাফার );
        }

        configASSERT( x_ফলাফল == pdTRUE );

        /* Delete the stream or message buffer */
        যদি( x_Is_Message_বাফার == pdTRUE )
        {
            v_Message_বাফার_Delete( x_Stream_বাফার );
        }
        অন্যথা
        {
            v_Stream_বাফার_Delete( x_Stream_বাফার );
        }

        /* Free the memory buffers */
        heap_caps_free( px_Static_Stream_বাফার );
        heap_caps_free( puc_Stream_বাফার_Storage_Area );
    }

#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/
