/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * This file contains the implementation for some the functions in
 * idf_additions.h
 */

#அந்தர்பாவ "sdkconfig.h"
#அந்தர்பாவ <stdint.h>
#அந்தர்பாவ "freertos/FreeRTOS.h"
#அந்தர்பாவ "freertos/task.h"
#அந்தர்பாவ "freertos/queue.h"
#அந்தர்பாவ "freertos/semphr.h"
#அந்தர்பாவ "freertos/stream_buffer.h"
#அந்தர்பாவ "freertos/message_buffer.h"
#அந்தர்பாவ "freertos/idf_additions.h"
#அந்தர்பாவ "esp_heap_caps.h"
#அந்தர்பாவ "esp_log.h"
#அந்தர்பாவ "freertos/portmacro.h"

/* -------------------------------------------- Creation With Memory Caps ------------------------------------------- */

/* ---------------------------------- Tasks --------------------------------- */

#எனில் ( configSUPPORT_STATIC_ALLOCATION == 1 )

    BaseType_t xTaskCreatePinnedToCoreWithCaps( TaskFunction_t pvTaskCode,
                                                மாறிலி எழுத்து * மாறிலி pcName,
                                                மாறிலி config_அடுக்கு_DEPTH_TYPE us_அடுக்கு_Depth,
                                                வெறுமை * மாறிலி pvParameters,
                                                UBaseType_t uxPriority,
                                                TaskHandle_t * மாறிலி pvCreatedTask,
                                                மாறிலி BaseType_t xCoreID,
                                                UBaseType_t uxMemoryCaps )
    {
        TaskHandle_t xHandle;
        StaticTask_t * pxTaskBuffer;
        அடுக்கு_Type_t * px_அடுக்கு;

        /* Allocate memory for the task's TCB. We use pvPortMalloc() here as the
         * TCB must be in internal memory. */
        pxTaskBuffer = pvPortMalloc( அளவு( StaticTask_t ) );

        /* Allocate memory for the task's stack using the provided memory caps
         * */
        px_அடுக்கு = heap_caps_malloc( ( ( size_t ) us_அடுக்கு_Depth ) * அளவு( அடுக்கு_Type_t ), ( uint32_t ) uxMemoryCaps );

        எனில்( ( pxTaskBuffer == வெற்று ) || ( px_அடுக்கு == வெற்று ) )
        {
            செல் err;
        }

        /* Create the task using static creation API*/
        xHandle = xTaskCreateStaticPinnedToCore( pvTaskCode, pcName, us_அடுக்கு_Depth, pvParameters, uxPriority, px_அடுக்கு, pxTaskBuffer, xCoreID );

        எனில்( xHandle == வெற்று )
        {
            /* Failed to create task */
            செல் err;
        }
        இல்லையெனில் எனில்( pvCreatedTask != வெற்று )
        {
            /* Task created successfully. Return the task handle */
            *pvCreatedTask = xHandle;
        }

        திரும்பு pdPASS;

err:
        heap_caps_free( px_அடுக்கு );
        vPortFree( pxTaskBuffer );
        திரும்பு pdFAIL;
    }

#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#எனில் ( configSUPPORT_STATIC_ALLOCATION == 1 )

    நிலையான வெறுமை prvTaskDeleteWithCaps( TaskHandle_t xTaskToDelete )
    {
        /* Return value unused if asserts are disabled */
        BaseType_t __attribute__( ( unused ) ) xResult;
        StaticTask_t * pxTaskBuffer;
        அடுக்கு_Type_t * pux_அடுக்கு_Buffer;

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
        க்கு( ;; )
        {
            BaseType_t xStillOnCore = pdFALSE;

            க்கு( BaseType_t xCoreID = 0; xCoreID < config_எண்_OF_CORES; xCoreID++ )
            {
                எனில்( xTaskGetCurrentTaskHandleForCore( xCoreID ) == xTaskToDelete )
                {
                    xStillOnCore = pdTRUE;
                    நிறுத்து;
                }
            }

            எனில்( xStillOnCore == pdFALSE )
            {
                நிறுத்து;
            }

            taskYIELD();
        }

        /* We can delete the task and free the memory buffers.
         * First, we must call `vTaskDelete` so that the port task delete callback is called.
         * On targets that have coprocessors, it may be possible that the stack pointer is modified (restored)
         * during this phase, hence, it must be done before getting the statuc buffers out of the task. */
        vTaskDelete( xTaskToDelete );

        /* Free the memory buffers */
        xResult = xTaskGetStaticBuffers( xTaskToDelete, &pux_அடுக்கு_Buffer, &pxTaskBuffer );
        configASSERT( xResult == pdTRUE );
        configASSERT( pux_அடுக்கு_Buffer != வெற்று );
        configASSERT( pxTaskBuffer != வெற்று );

        heap_caps_free( pux_அடுக்கு_Buffer );
        vPortFree( pxTaskBuffer );
    }

    நிலையான வெறுமை prvTaskDeleteWithCapsTask( வெறுமை * pvParameters )
    {
        TaskHandle_t xTaskToDelete = ( TaskHandle_t ) pvParameters;

        /* Delete the WithCaps task */
        prvTaskDeleteWithCaps( xTaskToDelete );

        /* Delete the temporary clean up task */
        vTaskDelete( வெற்று );
    }

    வெறுமை vTaskDeleteWithCaps( TaskHandle_t xTaskToDelete )
    {
        /* THIS FUNCTION SHOULD NOT BE CALLED FROM AN INTERRUPT CONTEXT. */
        /* TODO: Update it to use portASSERT_IF_IN_ISR() instead. (IDF-10540) */
        vPortAssertIfInISR();

        TaskHandle_t xCurrentTaskHandle = xTaskGetCurrentTaskHandle();
        configASSERT( xCurrentTaskHandle != வெற்று );

        எனில்( ( xTaskToDelete == வெற்று ) || ( xTaskToDelete == xCurrentTaskHandle ) )
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
            எனில்( xTaskCreatePinnedToCore( ( TaskFunction_t ) prvTaskDeleteWithCapsTask, "prvTaskDeleteWithCapsTask", config_MINIMAL_அடுக்கு_SIZE, xCurrentTaskHandle, uxTaskPriorityGet( xTaskToDelete ), வெற்று, xPortGetCoreID() ) != pdFAIL )
            {
                /* Although the current task should get preemted immediately when prvTaskDeleteWithCapsTask is created,
                 * for safety, we suspend the current task and wait for prvTaskDeleteWithCapsTask to delete it. */
                vTaskSuspend( xTaskToDelete );

                /* Should never reach here */
                ESP_LOGE( "freertos_additions", "%s: Failed to suspend the task to be deleted", __func__ );
                abort();
            }
            இல்லையெனில்
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

#எனில் ( configSUPPORT_STATIC_ALLOCATION == 1 )

    QueueHandle_t xQueueCreateWithCaps( UBaseType_t uxQueueLength,
                                        UBaseType_t uxItemSize,
                                        UBaseType_t uxMemoryCaps )
    {
        QueueHandle_t xQueue;
        StaticQueue_t * pxQueueBuffer;
        uint8_t * pucQueueStorageBuffer;

        /* Allocate memory for the queue using the provided memory caps */
        pxQueueBuffer = heap_caps_malloc( அளவு( StaticQueue_t ), ( uint32_t ) uxMemoryCaps );

        எனில்( uxItemSize == 0 )
        {
            pucQueueStorageBuffer = வெற்று;
        }
        இல்லையெனில்
        {
            pucQueueStorageBuffer = heap_caps_malloc( uxQueueLength * uxItemSize, ( uint32_t ) uxMemoryCaps );
        }

        எனில்( ( pxQueueBuffer == வெற்று ) || ( ( uxItemSize > 0 ) && ( pucQueueStorageBuffer == வெற்று ) ) )
        {
            செல் err;
        }

        /* Create the queue using static creation API */
        xQueue = xQueueCreateStatic( uxQueueLength, uxItemSize, pucQueueStorageBuffer, pxQueueBuffer );

        எனில்( xQueue == வெற்று )
        {
            செல் err;
        }

        திரும்பு xQueue;

err:
        heap_caps_free( pucQueueStorageBuffer );
        heap_caps_free( pxQueueBuffer );
        திரும்பு வெற்று;
    }

#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#எனில் ( configSUPPORT_STATIC_ALLOCATION == 1 )

    வெறுமை vQueueDeleteWithCaps( QueueHandle_t xQueue )
    {
        /* Return value unused if asserts are disabled */
        BaseType_t __attribute__( ( unused ) ) xResult;
        StaticQueue_t * pxQueueBuffer;
        uint8_t * pucQueueStorageBuffer;

        /* Retrieve the buffers used to create the queue before deleting it */
        xResult = xQueueGetStaticBuffers( xQueue, &pucQueueStorageBuffer, &pxQueueBuffer );
        configASSERT( xResult == pdTRUE );

        /* Delete the queue */
        vQueueDelete( xQueue );

        /* Free the memory buffers */
        heap_caps_free( pxQueueBuffer );
        heap_caps_free( pucQueueStorageBuffer );
    }

#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

/* -------------------------------- Semaphore ------------------------------- */

#எனில் ( configSUPPORT_STATIC_ALLOCATION == 1 )

    SemaphoreHandle_t xSemaphoreCreateGenericWithCaps( UBaseType_t ux_Max_மொத்தம்,
                                                       UBaseType_t ux_Initial_மொத்தம்,
                                                       மாறிலி uint8_t ucQueueType,
                                                       UBaseType_t uxMemoryCaps )
    {
        SemaphoreHandle_t xSemaphore;
        StaticSemaphore_t * pxSemaphoreBuffer;

        /* Allocate memory for the semaphore using the provided memory caps */
        pxSemaphoreBuffer = heap_caps_malloc( அளவு( StaticSemaphore_t ), ( uint32_t ) uxMemoryCaps );

        எனில்( pxSemaphoreBuffer == வெற்று )
        {
            திரும்பு வெற்று;
        }

        /* Create the semaphore using static creation API */
        எனில்( ucQueueType == queueQUEUE_TYPE_MUTEX )
        {
            xSemaphore = xSemaphoreCreateMutexStatic( pxSemaphoreBuffer );
        }
        இல்லையெனில் எனில்( ucQueueType == queueQUEUE_TYPE_COUNTING_SEMAPHORE )
        {
            xSemaphore = xSemaphoreCreateCountingStatic( ux_Max_மொத்தம், ux_Initial_மொத்தம், pxSemaphoreBuffer );
        }
        இல்லையெனில் எனில்( ucQueueType == queue_QUEUE_TYPE_இருமம்_SEMAPHORE )
        {
            xSemaphore = x_Semaphore_Create_இருமம்_Static( pxSemaphoreBuffer );
        }
        இல்லையெனில் /* ucQueueType == queueQUEUE_TYPE_RECURSIVE_MUTEX */
        {
            xSemaphore = xSemaphoreCreateRecursiveMutexStatic( pxSemaphoreBuffer );
        }

        எனில்( xSemaphore == வெற்று )
        {
            heap_caps_free( pxSemaphoreBuffer );
        }

        திரும்பு xSemaphore;
    }

#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#எனில் ( configSUPPORT_STATIC_ALLOCATION == 1 )

    வெறுமை vSemaphoreDeleteWithCaps( SemaphoreHandle_t xSemaphore )
    {
        /* Return value unused if asserts are disabled */
        BaseType_t __attribute__( ( unused ) ) xResult;
        StaticSemaphore_t * pxSemaphoreBuffer;

        /* Retrieve the buffer used to create the semaphore before deleting it
         * */
        xResult = xSemaphoreGetStaticBuffer( xSemaphore, &pxSemaphoreBuffer );
        configASSERT( xResult == pdTRUE );

        /* Delete the semaphore */
        vSemaphoreDelete( xSemaphore );

        /* Free the memory buffer */
        heap_caps_free( pxSemaphoreBuffer );
    }

#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

/* ------------------------- Stream & Message Buffers ----------------------- */

#எனில் ( configSUPPORT_STATIC_ALLOCATION == 1 )

    StreamBufferHandle_t xStreamBufferGenericCreateWithCaps( size_t xBufferSizeBytes,
                                                             size_t xTriggerLevelBytes,
                                                             BaseType_t xIsMessageBuffer,
                                                             UBaseType_t uxMemoryCaps )
    {
        StreamBufferHandle_t xStreamBuffer;
        StaticStreamBuffer_t * pxStaticStreamBuffer;
        uint8_t * pucStreamBufferStorageArea;

        /* Allocate memory for the stream or message buffer using the provided
         * memory caps */
        pxStaticStreamBuffer = heap_caps_malloc( அளவு( StaticStreamBuffer_t ), ( uint32_t ) uxMemoryCaps );
        pucStreamBufferStorageArea = heap_caps_malloc( xBufferSizeBytes, ( uint32_t ) uxMemoryCaps );

        எனில்( ( pxStaticStreamBuffer == வெற்று ) || ( pucStreamBufferStorageArea == வெற்று ) )
        {
            செல் err;
        }

        /* Create the stream or message buffer using static creation API */
        எனில்( xIsMessageBuffer == pdTRUE )
        {
            xStreamBuffer = ( StreamBufferHandle_t ) xMessageBufferCreateStatic( xBufferSizeBytes, pucStreamBufferStorageArea, pxStaticStreamBuffer );
        }
        இல்லையெனில்
        {
            xStreamBuffer = xStreamBufferCreateStatic( xBufferSizeBytes, xTriggerLevelBytes, pucStreamBufferStorageArea, pxStaticStreamBuffer );
        }

        எனில்( xStreamBuffer == வெற்று )
        {
            செல் err;
        }

        திரும்பு xStreamBuffer;

err:
        heap_caps_free( pucStreamBufferStorageArea );
        heap_caps_free( pxStaticStreamBuffer );
        திரும்பு வெற்று;
    }

#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#எனில் ( configSUPPORT_STATIC_ALLOCATION == 1 )

    வெறுமை vStreamBufferGenericDeleteWithCaps( StreamBufferHandle_t xStreamBuffer,
                                             BaseType_t xIsMessageBuffer )
    {
        /* Return value unused if asserts are disabled */
        BaseType_t __attribute__( ( unused ) ) xResult;
        StaticStreamBuffer_t * pxStaticStreamBuffer;
        uint8_t * pucStreamBufferStorageArea;

        /* Retrieve the buffers used to create the stream or message buffer
         * before deleting it */
        எனில்( xIsMessageBuffer == pdTRUE )
        {
            xResult = xMessageBufferGetStaticBuffers( xStreamBuffer, &pucStreamBufferStorageArea, &pxStaticStreamBuffer );
        }
        இல்லையெனில்
        {
            xResult = xStreamBufferGetStaticBuffers( xStreamBuffer, &pucStreamBufferStorageArea, &pxStaticStreamBuffer );
        }

        configASSERT( xResult == pdTRUE );

        /* Delete the stream or message buffer */
        எனில்( xIsMessageBuffer == pdTRUE )
        {
            vMessageBufferDelete( xStreamBuffer );
        }
        இல்லையெனில்
        {
            vStreamBufferDelete( xStreamBuffer );
        }

        /* Free the memory buffers */
        heap_caps_free( pxStaticStreamBuffer );
        heap_caps_free( pucStreamBufferStorageArea );
    }

#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/
