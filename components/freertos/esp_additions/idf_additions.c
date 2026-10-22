/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * This file contains the implementation for some the functions in
 * idf_additions.h
 */

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ "freertos/queue.h"
#ಅನ್ತರ್ಭಾವ "freertos/semphr.h"
#ಅನ್ತರ್ಭಾವ "freertos/stream_buffer.h"
#ಅನ್ತರ್ಭಾವ "freertos/message_buffer.h"
#ಅನ್ತರ್ಭಾವ "freertos/idf_additions.h"
#ಅನ್ತರ್ಭಾವ "esp_heap_caps.h"
#ಅನ್ತರ್ಭಾವ "esp_log.h"
#ಅನ್ತರ್ಭಾವ "freertos/portmacro.h"

/* -------------------------------------------- Creation With Memory Caps ------------------------------------------- */

/* ---------------------------------- Tasks --------------------------------- */

#ಯದಿ ( configSUPPORT_STATIC_ALLOCATION == 1 )

    BaseType_t xTaskCreatePinnedToCoreWithCaps( TaskFunction_t pvTaskCode,
                                                ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ pcName,
                                                ಧ್ರುವ config_ರಾಶಿ_DEPTH_TYPE us_ರಾಶಿ_Depth,
                                                ಶೂನ್ಯ * ಧ್ರುವ pvParameters,
                                                UBaseType_t uxPriority,
                                                TaskHandle_t * ಧ್ರುವ pvCreatedTask,
                                                ಧ್ರುವ BaseType_t xCoreID,
                                                UBaseType_t uxMemoryCaps )
    {
        TaskHandle_t xHandle;
        StaticTask_t * pxTaskBuffer;
        ರಾಶಿ_Type_t * px_ರಾಶಿ;

        /* Allocate memory for the task's TCB. We use pvPortMalloc() here as the
         * TCB must be in internal memory. */
        pxTaskBuffer = pvPortMalloc( ಪರಿಮಾಣ( StaticTask_t ) );

        /* Allocate memory for the task's stack using the provided memory caps
         * */
        px_ರಾಶಿ = heap_caps_malloc( ( ( size_t ) us_ರಾಶಿ_Depth ) * ಪರಿಮಾಣ( ರಾಶಿ_Type_t ), ( uint32_t ) uxMemoryCaps );

        ಯದಿ( ( pxTaskBuffer == ರಿಕ್ತ ) || ( px_ರಾಶಿ == ರಿಕ್ತ ) )
        {
            ಗಚ್ಛ err;
        }

        /* Create the task using static creation API*/
        xHandle = xTaskCreateStaticPinnedToCore( pvTaskCode, pcName, us_ರಾಶಿ_Depth, pvParameters, uxPriority, px_ರಾಶಿ, pxTaskBuffer, xCoreID );

        ಯದಿ( xHandle == ರಿಕ್ತ )
        {
            /* Failed to create task */
            ಗಚ್ಛ err;
        }
        ಅನ್ಯಥಾ ಯದಿ( pvCreatedTask != ರಿಕ್ತ )
        {
            /* Task created successfully. Return the task handle */
            *pvCreatedTask = xHandle;
        }

        ಪ್ರತ್ಯಾಗಮ pdPASS;

err:
        heap_caps_free( px_ರಾಶಿ );
        vPortFree( pxTaskBuffer );
        ಪ್ರತ್ಯಾಗಮ pdFAIL;
    }

#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#ಯದಿ ( configSUPPORT_STATIC_ALLOCATION == 1 )

    ಸ್ಥಿರ ಶೂನ್ಯ prvTaskDeleteWithCaps( TaskHandle_t xTaskToDelete )
    {
        /* Return value unused if asserts are disabled */
        BaseType_t __attribute__( ( unused ) ) xResult;
        StaticTask_t * pxTaskBuffer;
        ರಾಶಿ_Type_t * pux_ರಾಶಿ_Buffer;

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
        ಪ್ರತಿ( ;; )
        {
            BaseType_t xStillOnCore = pdFALSE;

            ಪ್ರತಿ( BaseType_t xCoreID = 0; xCoreID < config_ಸಂಖ್ಯೆ_OF_CORES; xCoreID++ )
            {
                ಯದಿ( xTaskGetCurrentTaskHandleForCore( xCoreID ) == xTaskToDelete )
                {
                    xStillOnCore = pdTRUE;
                    ಭಙ್ಗ;
                }
            }

            ಯದಿ( xStillOnCore == pdFALSE )
            {
                ಭಙ್ಗ;
            }

            taskYIELD();
        }

        /* We can delete the task and free the memory buffers.
         * First, we must call `vTaskDelete` so that the port task delete callback is called.
         * On targets that have coprocessors, it may be possible that the stack pointer is modified (restored)
         * during this phase, hence, it must be done before getting the statuc buffers out of the task. */
        vTaskDelete( xTaskToDelete );

        /* Free the memory buffers */
        xResult = xTaskGetStaticBuffers( xTaskToDelete, &pux_ರಾಶಿ_Buffer, &pxTaskBuffer );
        configASSERT( xResult == pdTRUE );
        configASSERT( pux_ರಾಶಿ_Buffer != ರಿಕ್ತ );
        configASSERT( pxTaskBuffer != ರಿಕ್ತ );

        heap_caps_free( pux_ರಾಶಿ_Buffer );
        vPortFree( pxTaskBuffer );
    }

    ಸ್ಥಿರ ಶೂನ್ಯ prvTaskDeleteWithCapsTask( ಶೂನ್ಯ * pvParameters )
    {
        TaskHandle_t xTaskToDelete = ( TaskHandle_t ) pvParameters;

        /* Delete the WithCaps task */
        prvTaskDeleteWithCaps( xTaskToDelete );

        /* Delete the temporary clean up task */
        vTaskDelete( ರಿಕ್ತ );
    }

    ಶೂನ್ಯ vTaskDeleteWithCaps( TaskHandle_t xTaskToDelete )
    {
        /* THIS FUNCTION SHOULD NOT BE CALLED FROM AN INTERRUPT CONTEXT. */
        /* TODO: Update it to use portASSERT_IF_IN_ISR() instead. (IDF-10540) */
        vPortAssertIfInISR();

        TaskHandle_t xCurrentTaskHandle = xTaskGetCurrentTaskHandle();
        configASSERT( xCurrentTaskHandle != ರಿಕ್ತ );

        ಯದಿ( ( xTaskToDelete == ರಿಕ್ತ ) || ( xTaskToDelete == xCurrentTaskHandle ) )
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
            ಯದಿ( xTaskCreatePinnedToCore( ( TaskFunction_t ) prvTaskDeleteWithCapsTask, "prvTaskDeleteWithCapsTask", config_MINIMAL_ರಾಶಿ_SIZE, xCurrentTaskHandle, uxTaskPriorityGet( xTaskToDelete ), ರಿಕ್ತ, xPortGetCoreID() ) != pdFAIL )
            {
                /* Although the current task should get preemted immediately when prvTaskDeleteWithCapsTask is created,
                 * for safety, we suspend the current task and wait for prvTaskDeleteWithCapsTask to delete it. */
                vTaskSuspend( xTaskToDelete );

                /* Should never reach here */
                ESP_LOGE( "freertos_additions", "%s: Failed to suspend the task to be deleted", __func__ );
                abort();
            }
            ಅನ್ಯಥಾ
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

#ಯದಿ ( configSUPPORT_STATIC_ALLOCATION == 1 )

    QueueHandle_t xQueueCreateWithCaps( UBaseType_t uxQueueLength,
                                        UBaseType_t uxItemSize,
                                        UBaseType_t uxMemoryCaps )
    {
        QueueHandle_t xQueue;
        StaticQueue_t * pxQueueBuffer;
        uint8_t * pucQueueStorageBuffer;

        /* Allocate memory for the queue using the provided memory caps */
        pxQueueBuffer = heap_caps_malloc( ಪರಿಮಾಣ( StaticQueue_t ), ( uint32_t ) uxMemoryCaps );

        ಯದಿ( uxItemSize == 0 )
        {
            pucQueueStorageBuffer = ರಿಕ್ತ;
        }
        ಅನ್ಯಥಾ
        {
            pucQueueStorageBuffer = heap_caps_malloc( uxQueueLength * uxItemSize, ( uint32_t ) uxMemoryCaps );
        }

        ಯದಿ( ( pxQueueBuffer == ರಿಕ್ತ ) || ( ( uxItemSize > 0 ) && ( pucQueueStorageBuffer == ರಿಕ್ತ ) ) )
        {
            ಗಚ್ಛ err;
        }

        /* Create the queue using static creation API */
        xQueue = xQueueCreateStatic( uxQueueLength, uxItemSize, pucQueueStorageBuffer, pxQueueBuffer );

        ಯದಿ( xQueue == ರಿಕ್ತ )
        {
            ಗಚ್ಛ err;
        }

        ಪ್ರತ್ಯಾಗಮ xQueue;

err:
        heap_caps_free( pucQueueStorageBuffer );
        heap_caps_free( pxQueueBuffer );
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#ಯದಿ ( configSUPPORT_STATIC_ALLOCATION == 1 )

    ಶೂನ್ಯ vQueueDeleteWithCaps( QueueHandle_t xQueue )
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

#ಯದಿ ( configSUPPORT_STATIC_ALLOCATION == 1 )

    SemaphoreHandle_t xSemaphoreCreateGenericWithCaps( UBaseType_t ux_Max_ಎಣಿಕೆ,
                                                       UBaseType_t ux_Initial_ಎಣಿಕೆ,
                                                       ಧ್ರುವ uint8_t ucQueueType,
                                                       UBaseType_t uxMemoryCaps )
    {
        SemaphoreHandle_t xSemaphore;
        StaticSemaphore_t * pxSemaphoreBuffer;

        /* Allocate memory for the semaphore using the provided memory caps */
        pxSemaphoreBuffer = heap_caps_malloc( ಪರಿಮಾಣ( StaticSemaphore_t ), ( uint32_t ) uxMemoryCaps );

        ಯದಿ( pxSemaphoreBuffer == ರಿಕ್ತ )
        {
            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        }

        /* Create the semaphore using static creation API */
        ಯದಿ( ucQueueType == queueQUEUE_TYPE_MUTEX )
        {
            xSemaphore = xSemaphoreCreateMutexStatic( pxSemaphoreBuffer );
        }
        ಅನ್ಯಥಾ ಯದಿ( ucQueueType == queueQUEUE_TYPE_COUNTING_SEMAPHORE )
        {
            xSemaphore = xSemaphoreCreateCountingStatic( ux_Max_ಎಣಿಕೆ, ux_Initial_ಎಣಿಕೆ, pxSemaphoreBuffer );
        }
        ಅನ್ಯಥಾ ಯದಿ( ucQueueType == queue_QUEUE_TYPE_ದ್ವಿಮಾನ_SEMAPHORE )
        {
            xSemaphore = x_Semaphore_Create_ದ್ವಿಮಾನ_Static( pxSemaphoreBuffer );
        }
        ಅನ್ಯಥಾ /* ucQueueType == queueQUEUE_TYPE_RECURSIVE_MUTEX */
        {
            xSemaphore = xSemaphoreCreateRecursiveMutexStatic( pxSemaphoreBuffer );
        }

        ಯದಿ( xSemaphore == ರಿಕ್ತ )
        {
            heap_caps_free( pxSemaphoreBuffer );
        }

        ಪ್ರತ್ಯಾಗಮ xSemaphore;
    }

#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#ಯದಿ ( configSUPPORT_STATIC_ALLOCATION == 1 )

    ಶೂನ್ಯ vSemaphoreDeleteWithCaps( SemaphoreHandle_t xSemaphore )
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

#ಯದಿ ( configSUPPORT_STATIC_ALLOCATION == 1 )

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
        pxStaticStreamBuffer = heap_caps_malloc( ಪರಿಮಾಣ( StaticStreamBuffer_t ), ( uint32_t ) uxMemoryCaps );
        pucStreamBufferStorageArea = heap_caps_malloc( xBufferSizeBytes, ( uint32_t ) uxMemoryCaps );

        ಯದಿ( ( pxStaticStreamBuffer == ರಿಕ್ತ ) || ( pucStreamBufferStorageArea == ರಿಕ್ತ ) )
        {
            ಗಚ್ಛ err;
        }

        /* Create the stream or message buffer using static creation API */
        ಯದಿ( xIsMessageBuffer == pdTRUE )
        {
            xStreamBuffer = ( StreamBufferHandle_t ) xMessageBufferCreateStatic( xBufferSizeBytes, pucStreamBufferStorageArea, pxStaticStreamBuffer );
        }
        ಅನ್ಯಥಾ
        {
            xStreamBuffer = xStreamBufferCreateStatic( xBufferSizeBytes, xTriggerLevelBytes, pucStreamBufferStorageArea, pxStaticStreamBuffer );
        }

        ಯದಿ( xStreamBuffer == ರಿಕ್ತ )
        {
            ಗಚ್ಛ err;
        }

        ಪ್ರತ್ಯಾಗಮ xStreamBuffer;

err:
        heap_caps_free( pucStreamBufferStorageArea );
        heap_caps_free( pxStaticStreamBuffer );
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#ಯದಿ ( configSUPPORT_STATIC_ALLOCATION == 1 )

    ಶೂನ್ಯ vStreamBufferGenericDeleteWithCaps( StreamBufferHandle_t xStreamBuffer,
                                             BaseType_t xIsMessageBuffer )
    {
        /* Return value unused if asserts are disabled */
        BaseType_t __attribute__( ( unused ) ) xResult;
        StaticStreamBuffer_t * pxStaticStreamBuffer;
        uint8_t * pucStreamBufferStorageArea;

        /* Retrieve the buffers used to create the stream or message buffer
         * before deleting it */
        ಯದಿ( xIsMessageBuffer == pdTRUE )
        {
            xResult = xMessageBufferGetStaticBuffers( xStreamBuffer, &pucStreamBufferStorageArea, &pxStaticStreamBuffer );
        }
        ಅನ್ಯಥಾ
        {
            xResult = xStreamBufferGetStaticBuffers( xStreamBuffer, &pucStreamBufferStorageArea, &pxStaticStreamBuffer );
        }

        configASSERT( xResult == pdTRUE );

        /* Delete the stream or message buffer */
        ಯದಿ( xIsMessageBuffer == pdTRUE )
        {
            vMessageBufferDelete( xStreamBuffer );
        }
        ಅನ್ಯಥಾ
        {
            vStreamBufferDelete( xStreamBuffer );
        }

        /* Free the memory buffers */
        heap_caps_free( pxStaticStreamBuffer );
        heap_caps_free( pucStreamBufferStorageArea );
    }

#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/
