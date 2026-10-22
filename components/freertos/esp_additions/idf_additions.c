/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * This file contains the implementation for some the functions in
 * idf_additions.h
 */

#अन्तर्भाव "sdkconfig.h"
#अन्तर्भाव <stdint.h>
#अन्तर्भाव "freertos/FreeRTOS.h"
#अन्तर्भाव "freertos/task.h"
#अन्तर्भाव "freertos/queue.h"
#अन्तर्भाव "freertos/semphr.h"
#अन्तर्भाव "freertos/stream_buffer.h"
#अन्तर्भाव "freertos/message_buffer.h"
#अन्तर्भाव "freertos/idf_additions.h"
#अन्तर्भाव "esp_heap_caps.h"
#अन्तर्भाव "esp_log.h"
#अन्तर्भाव "freertos/portmacro.h"

/* -------------------------------------------- Creation With Memory Caps ------------------------------------------- */

/* ---------------------------------- Tasks --------------------------------- */

#यदि ( configSUPPORT_STATIC_ALLOCATION == 1 )

    BaseType_t xTaskCreatePinnedToCoreWithCaps( TaskFunction_t pvTaskCode,
                                                अचर वर्ण * अचर pcName,
                                                अचर config_थाक_DEPTH_TYPE us_थाक_Depth,
                                                शून्य * अचर pvParameters,
                                                UBaseType_t uxPriority,
                                                TaskHandle_t * अचर pvCreatedTask,
                                                अचर BaseType_t xCoreID,
                                                UBaseType_t uxMemoryCaps )
    {
        TaskHandle_t xHandle;
        StaticTask_t * pxTaskBuffer;
        थाक_Type_t * px_थाक;

        /* Allocate memory for the task's TCB. We use pvPortMalloc() here as the
         * TCB must be in internal memory. */
        pxTaskBuffer = pvPortMalloc( परिमाण( StaticTask_t ) );

        /* Allocate memory for the task's stack using the provided memory caps
         * */
        px_थाक = heap_caps_malloc( ( ( माप_प्रकार ) us_थाक_Depth ) * परिमाण( थाक_Type_t ), ( uint32_t ) uxMemoryCaps );

        यदि( ( pxTaskBuffer == रिक्त ) || ( px_थाक == रिक्त ) )
        {
            गच्छ err;
        }

        /* Create the task using static creation API*/
        xHandle = xTaskCreateStaticPinnedToCore( pvTaskCode, pcName, us_थाक_Depth, pvParameters, uxPriority, px_थाक, pxTaskBuffer, xCoreID );

        यदि( xHandle == रिक्त )
        {
            /* Failed to create task */
            गच्छ err;
        }
        अन्यथा यदि( pvCreatedTask != रिक्त )
        {
            /* Task created successfully. Return the task handle */
            *pvCreatedTask = xHandle;
        }

        फर्काऊ pdPASS;

err:
        heap_caps_free( px_थाक );
        vPortFree( pxTaskBuffer );
        फर्काऊ pdFAIL;
    }

#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#यदि ( configSUPPORT_STATIC_ALLOCATION == 1 )

    स्थिर शून्य prvTaskDeleteWithCaps( TaskHandle_t xTaskToDelete )
    {
        /* Return value unused if asserts are disabled */
        BaseType_t __attribute__( ( unused ) ) xResult;
        StaticTask_t * pxTaskBuffer;
        थाक_Type_t * pux_थाक_Buffer;

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
        लागि( ;; )
        {
            BaseType_t xStillOnCore = pdFALSE;

            लागि( BaseType_t xCoreID = 0; xCoreID < config_सङ्ख्या_OF_CORES; xCoreID++ )
            {
                यदि( xTaskGetCurrentTaskHandleForCore( xCoreID ) == xTaskToDelete )
                {
                    xStillOnCore = pdTRUE;
                    रोक;
                }
            }

            यदि( xStillOnCore == pdFALSE )
            {
                रोक;
            }

            taskYIELD();
        }

        /* We can delete the task and free the memory buffers.
         * First, we must call `vTaskDelete` so that the port task delete callback is called.
         * On targets that have coprocessors, it may be possible that the stack pointer is modified (restored)
         * during this phase, hence, it must be done before getting the statuc buffers out of the task. */
        vTaskDelete( xTaskToDelete );

        /* Free the memory buffers */
        xResult = xTaskGetStaticBuffers( xTaskToDelete, &pux_थाक_Buffer, &pxTaskBuffer );
        configASSERT( xResult == pdTRUE );
        configASSERT( pux_थाक_Buffer != रिक्त );
        configASSERT( pxTaskBuffer != रिक्त );

        heap_caps_free( pux_थाक_Buffer );
        vPortFree( pxTaskBuffer );
    }

    स्थिर शून्य prvTaskDeleteWithCapsTask( शून्य * pvParameters )
    {
        TaskHandle_t xTaskToDelete = ( TaskHandle_t ) pvParameters;

        /* Delete the WithCaps task */
        prvTaskDeleteWithCaps( xTaskToDelete );

        /* Delete the temporary clean up task */
        vTaskDelete( रिक्त );
    }

    शून्य vTaskDeleteWithCaps( TaskHandle_t xTaskToDelete )
    {
        /* THIS FUNCTION SHOULD NOT BE CALLED FROM AN INTERRUPT CONTEXT. */
        /* TODO: Update it to use portASSERT_IF_IN_ISR() instead. (IDF-10540) */
        vPortAssertIfInISR();

        TaskHandle_t xCurrentTaskHandle = xTaskGetCurrentTaskHandle();
        configASSERT( xCurrentTaskHandle != रिक्त );

        यदि( ( xTaskToDelete == रिक्त ) || ( xTaskToDelete == xCurrentTaskHandle ) )
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
            यदि( xTaskCreatePinnedToCore( ( TaskFunction_t ) prvTaskDeleteWithCapsTask, "prvTaskDeleteWithCapsTask", config_MINIMAL_थाक_SIZE, xCurrentTaskHandle, uxTaskPriorityGet( xTaskToDelete ), रिक्त, xPortGetCoreID() ) != pdFAIL )
            {
                /* Although the current task should get preemted immediately when prvTaskDeleteWithCapsTask is created,
                 * for safety, we suspend the current task and wait for prvTaskDeleteWithCapsTask to delete it. */
                vTaskSuspend( xTaskToDelete );

                /* Should never reach here */
                ESP_LOGE( "freertos_additions", "%s: Failed to suspend the task to be deleted", __func__ );
                पात();
            }
            अन्यथा
            {
                /* Failed to create the task to delete the current task. */
                ESP_LOGE( "freertos_additions", "%s: Failed to create the task to delete the current task", __func__ );
                पात();
            }
        }

        /* Delete the WithCaps task */
        prvTaskDeleteWithCaps( xTaskToDelete );
    }

#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

/* ---------------------------------- Queue --------------------------------- */

#यदि ( configSUPPORT_STATIC_ALLOCATION == 1 )

    QueueHandle_t xQueueCreateWithCaps( UBaseType_t uxQueueLength,
                                        UBaseType_t uxItemSize,
                                        UBaseType_t uxMemoryCaps )
    {
        QueueHandle_t xQueue;
        StaticQueue_t * pxQueueBuffer;
        uint8_t * pucQueueStorageBuffer;

        /* Allocate memory for the queue using the provided memory caps */
        pxQueueBuffer = heap_caps_malloc( परिमाण( StaticQueue_t ), ( uint32_t ) uxMemoryCaps );

        यदि( uxItemSize == 0 )
        {
            pucQueueStorageBuffer = रिक्त;
        }
        अन्यथा
        {
            pucQueueStorageBuffer = heap_caps_malloc( uxQueueLength * uxItemSize, ( uint32_t ) uxMemoryCaps );
        }

        यदि( ( pxQueueBuffer == रिक्त ) || ( ( uxItemSize > 0 ) && ( pucQueueStorageBuffer == रिक्त ) ) )
        {
            गच्छ err;
        }

        /* Create the queue using static creation API */
        xQueue = xQueueCreateStatic( uxQueueLength, uxItemSize, pucQueueStorageBuffer, pxQueueBuffer );

        यदि( xQueue == रिक्त )
        {
            गच्छ err;
        }

        फर्काऊ xQueue;

err:
        heap_caps_free( pucQueueStorageBuffer );
        heap_caps_free( pxQueueBuffer );
        फर्काऊ रिक्त;
    }

#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#यदि ( configSUPPORT_STATIC_ALLOCATION == 1 )

    शून्य vQueueDeleteWithCaps( QueueHandle_t xQueue )
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

#यदि ( configSUPPORT_STATIC_ALLOCATION == 1 )

    SemaphoreHandle_t xSemaphoreCreateGenericWithCaps( UBaseType_t ux_Max_गन्ती,
                                                       UBaseType_t ux_Initial_गन्ती,
                                                       अचर uint8_t ucQueueType,
                                                       UBaseType_t uxMemoryCaps )
    {
        SemaphoreHandle_t xSemaphore;
        StaticSemaphore_t * pxSemaphoreBuffer;

        /* Allocate memory for the semaphore using the provided memory caps */
        pxSemaphoreBuffer = heap_caps_malloc( परिमाण( StaticSemaphore_t ), ( uint32_t ) uxMemoryCaps );

        यदि( pxSemaphoreBuffer == रिक्त )
        {
            फर्काऊ रिक्त;
        }

        /* Create the semaphore using static creation API */
        यदि( ucQueueType == queueQUEUE_TYPE_MUTEX )
        {
            xSemaphore = xSemaphoreCreateMutexStatic( pxSemaphoreBuffer );
        }
        अन्यथा यदि( ucQueueType == queueQUEUE_TYPE_COUNTING_SEMAPHORE )
        {
            xSemaphore = xSemaphoreCreateCountingStatic( ux_Max_गन्ती, ux_Initial_गन्ती, pxSemaphoreBuffer );
        }
        अन्यथा यदि( ucQueueType == queue_QUEUE_TYPE_द्विआधारी_SEMAPHORE )
        {
            xSemaphore = x_Semaphore_Create_द्विआधारी_Static( pxSemaphoreBuffer );
        }
        अन्यथा /* ucQueueType == queueQUEUE_TYPE_RECURSIVE_MUTEX */
        {
            xSemaphore = xSemaphoreCreateRecursiveMutexStatic( pxSemaphoreBuffer );
        }

        यदि( xSemaphore == रिक्त )
        {
            heap_caps_free( pxSemaphoreBuffer );
        }

        फर्काऊ xSemaphore;
    }

#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#यदि ( configSUPPORT_STATIC_ALLOCATION == 1 )

    शून्य vSemaphoreDeleteWithCaps( SemaphoreHandle_t xSemaphore )
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

#यदि ( configSUPPORT_STATIC_ALLOCATION == 1 )

    StreamBufferHandle_t xStreamBufferGenericCreateWithCaps( माप_प्रकार xBufferSizeBytes,
                                                             माप_प्रकार xTriggerLevelBytes,
                                                             BaseType_t xIsMessageBuffer,
                                                             UBaseType_t uxMemoryCaps )
    {
        StreamBufferHandle_t xStreamBuffer;
        StaticStreamBuffer_t * pxStaticStreamBuffer;
        uint8_t * pucStreamBufferStorageArea;

        /* Allocate memory for the stream or message buffer using the provided
         * memory caps */
        pxStaticStreamBuffer = heap_caps_malloc( परिमाण( StaticStreamBuffer_t ), ( uint32_t ) uxMemoryCaps );
        pucStreamBufferStorageArea = heap_caps_malloc( xBufferSizeBytes, ( uint32_t ) uxMemoryCaps );

        यदि( ( pxStaticStreamBuffer == रिक्त ) || ( pucStreamBufferStorageArea == रिक्त ) )
        {
            गच्छ err;
        }

        /* Create the stream or message buffer using static creation API */
        यदि( xIsMessageBuffer == pdTRUE )
        {
            xStreamBuffer = ( StreamBufferHandle_t ) xMessageBufferCreateStatic( xBufferSizeBytes, pucStreamBufferStorageArea, pxStaticStreamBuffer );
        }
        अन्यथा
        {
            xStreamBuffer = xStreamBufferCreateStatic( xBufferSizeBytes, xTriggerLevelBytes, pucStreamBufferStorageArea, pxStaticStreamBuffer );
        }

        यदि( xStreamBuffer == रिक्त )
        {
            गच्छ err;
        }

        फर्काऊ xStreamBuffer;

err:
        heap_caps_free( pucStreamBufferStorageArea );
        heap_caps_free( pxStaticStreamBuffer );
        फर्काऊ रिक्त;
    }

#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#यदि ( configSUPPORT_STATIC_ALLOCATION == 1 )

    शून्य vStreamBufferGenericDeleteWithCaps( StreamBufferHandle_t xStreamBuffer,
                                             BaseType_t xIsMessageBuffer )
    {
        /* Return value unused if asserts are disabled */
        BaseType_t __attribute__( ( unused ) ) xResult;
        StaticStreamBuffer_t * pxStaticStreamBuffer;
        uint8_t * pucStreamBufferStorageArea;

        /* Retrieve the buffers used to create the stream or message buffer
         * before deleting it */
        यदि( xIsMessageBuffer == pdTRUE )
        {
            xResult = xMessageBufferGetStaticBuffers( xStreamBuffer, &pucStreamBufferStorageArea, &pxStaticStreamBuffer );
        }
        अन्यथा
        {
            xResult = xStreamBufferGetStaticBuffers( xStreamBuffer, &pucStreamBufferStorageArea, &pxStaticStreamBuffer );
        }

        configASSERT( xResult == pdTRUE );

        /* Delete the stream or message buffer */
        यदि( xIsMessageBuffer == pdTRUE )
        {
            vMessageBufferDelete( xStreamBuffer );
        }
        अन्यथा
        {
            vStreamBufferDelete( xStreamBuffer );
        }

        /* Free the memory buffers */
        heap_caps_free( pxStaticStreamBuffer );
        heap_caps_free( pucStreamBufferStorageArea );
    }

#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/
