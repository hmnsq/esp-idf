/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * This file contains the implementation for some the functions in
 * idf_additions.h
 */

#അന്തര്ഭാവ "sdkconfig.h"
#അന്തര്ഭാവ <stdint.h>
#അന്തര്ഭാവ "freertos/FreeRTOS.h"
#അന്തര്ഭാവ "freertos/task.h"
#അന്തര്ഭാവ "freertos/queue.h"
#അന്തര്ഭാവ "freertos/semphr.h"
#അന്തര്ഭാവ "freertos/stream_buffer.h"
#അന്തര്ഭാവ "freertos/message_buffer.h"
#അന്തര്ഭാവ "freertos/idf_additions.h"
#അന്തര്ഭാവ "esp_heap_caps.h"
#അന്തര്ഭാവ "esp_log.h"
#അന്തര്ഭാവ "freertos/portmacro.h"

/* -------------------------------------------- Creation With Memory Caps ------------------------------------------- */

/* ---------------------------------- Tasks --------------------------------- */

#എങ്കിൽ ( configSUPPORT_STATIC_ALLOCATION == 1 )

    BaseType_t xTaskCreatePinnedToCoreWithCaps( TaskFunction_t pvTaskCode,
                                                സ്ഥിരം അക്ഷരം * സ്ഥിരം pcName,
                                                സ്ഥിരം config_അട്ടി_DEPTH_TYPE us_അട്ടി_Depth,
                                                ശൂന്യം * സ്ഥിരം pvParameters,
                                                UBaseType_t uxPriority,
                                                TaskHandle_t * സ്ഥിരം pvCreatedTask,
                                                സ്ഥിരം BaseType_t xCoreID,
                                                UBaseType_t uxMemoryCaps )
    {
        TaskHandle_t xHandle;
        StaticTask_t * pxTaskBuffer;
        അട്ടി_Type_t * px_അട്ടി;

        /* Allocate memory for the task's TCB. We use pvPortMalloc() here as the
         * TCB must be in internal memory. */
        pxTaskBuffer = pvPortMalloc( വലുപ്പം( StaticTask_t ) );

        /* Allocate memory for the task's stack using the provided memory caps
         * */
        px_അട്ടി = heap_caps_malloc( ( ( size_t ) us_അട്ടി_Depth ) * വലുപ്പം( അട്ടി_Type_t ), ( uint32_t ) uxMemoryCaps );

        എങ്കിൽ( ( pxTaskBuffer == ശൂന്യ ) || ( px_അട്ടി == ശൂന്യ ) )
        {
            പോകുക err;
        }

        /* Create the task using static creation API*/
        xHandle = xTaskCreateStaticPinnedToCore( pvTaskCode, pcName, us_അട്ടി_Depth, pvParameters, uxPriority, px_അട്ടി, pxTaskBuffer, xCoreID );

        എങ്കിൽ( xHandle == ശൂന്യ )
        {
            /* Failed to create task */
            പോകുക err;
        }
        അല്ലെങ്കിൽ എങ്കിൽ( pvCreatedTask != ശൂന്യ )
        {
            /* Task created successfully. Return the task handle */
            *pvCreatedTask = xHandle;
        }

        തിരികെ pdPASS;

err:
        heap_caps_free( px_അട്ടി );
        vPortFree( pxTaskBuffer );
        തിരികെ pdFAIL;
    }

#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#എങ്കിൽ ( configSUPPORT_STATIC_ALLOCATION == 1 )

    സ്ഥിതം ശൂന്യം prvTaskDeleteWithCaps( TaskHandle_t xTaskToDelete )
    {
        /* Return value unused if asserts are disabled */
        BaseType_t __attribute__( ( unused ) ) xResult;
        StaticTask_t * pxTaskBuffer;
        അട്ടി_Type_t * pux_അട്ടി_Buffer;

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
        വേണ്ടി( ;; )
        {
            BaseType_t xStillOnCore = pdFALSE;

            വേണ്ടി( BaseType_t xCoreID = 0; xCoreID < config_സംഖ്യ_OF_CORES; xCoreID++ )
            {
                എങ്കിൽ( xTaskGetCurrentTaskHandleForCore( xCoreID ) == xTaskToDelete )
                {
                    xStillOnCore = pdTRUE;
                    നിർത്തുക;
                }
            }

            എങ്കിൽ( xStillOnCore == pdFALSE )
            {
                നിർത്തുക;
            }

            taskYIELD();
        }

        /* We can delete the task and free the memory buffers.
         * First, we must call `vTaskDelete` so that the port task delete callback is called.
         * On targets that have coprocessors, it may be possible that the stack pointer is modified (restored)
         * during this phase, hence, it must be done before getting the statuc buffers out of the task. */
        vTaskDelete( xTaskToDelete );

        /* Free the memory buffers */
        xResult = xTaskGetStaticBuffers( xTaskToDelete, &pux_അട്ടി_Buffer, &pxTaskBuffer );
        configASSERT( xResult == pdTRUE );
        configASSERT( pux_അട്ടി_Buffer != ശൂന്യ );
        configASSERT( pxTaskBuffer != ശൂന്യ );

        heap_caps_free( pux_അട്ടി_Buffer );
        vPortFree( pxTaskBuffer );
    }

    സ്ഥിതം ശൂന്യം prvTaskDeleteWithCapsTask( ശൂന്യം * pvParameters )
    {
        TaskHandle_t xTaskToDelete = ( TaskHandle_t ) pvParameters;

        /* Delete the WithCaps task */
        prvTaskDeleteWithCaps( xTaskToDelete );

        /* Delete the temporary clean up task */
        vTaskDelete( ശൂന്യ );
    }

    ശൂന്യം vTaskDeleteWithCaps( TaskHandle_t xTaskToDelete )
    {
        /* THIS FUNCTION SHOULD NOT BE CALLED FROM AN INTERRUPT CONTEXT. */
        /* TODO: Update it to use portASSERT_IF_IN_ISR() instead. (IDF-10540) */
        vPortAssertIfInISR();

        TaskHandle_t xCurrentTaskHandle = xTaskGetCurrentTaskHandle();
        configASSERT( xCurrentTaskHandle != ശൂന്യ );

        എങ്കിൽ( ( xTaskToDelete == ശൂന്യ ) || ( xTaskToDelete == xCurrentTaskHandle ) )
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
            എങ്കിൽ( xTaskCreatePinnedToCore( ( TaskFunction_t ) prvTaskDeleteWithCapsTask, "prvTaskDeleteWithCapsTask", config_MINIMAL_അട്ടി_SIZE, xCurrentTaskHandle, uxTaskPriorityGet( xTaskToDelete ), ശൂന്യ, xPortGetCoreID() ) != pdFAIL )
            {
                /* Although the current task should get preemted immediately when prvTaskDeleteWithCapsTask is created,
                 * for safety, we suspend the current task and wait for prvTaskDeleteWithCapsTask to delete it. */
                vTaskSuspend( xTaskToDelete );

                /* Should never reach here */
                ESP_LOGE( "freertos_additions", "%s: Failed to suspend the task to be deleted", __func__ );
                abort();
            }
            അല്ലെങ്കിൽ
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

#എങ്കിൽ ( configSUPPORT_STATIC_ALLOCATION == 1 )

    QueueHandle_t xQueueCreateWithCaps( UBaseType_t uxQueueLength,
                                        UBaseType_t uxItemSize,
                                        UBaseType_t uxMemoryCaps )
    {
        QueueHandle_t xQueue;
        StaticQueue_t * pxQueueBuffer;
        uint8_t * pucQueueStorageBuffer;

        /* Allocate memory for the queue using the provided memory caps */
        pxQueueBuffer = heap_caps_malloc( വലുപ്പം( StaticQueue_t ), ( uint32_t ) uxMemoryCaps );

        എങ്കിൽ( uxItemSize == 0 )
        {
            pucQueueStorageBuffer = ശൂന്യ;
        }
        അല്ലെങ്കിൽ
        {
            pucQueueStorageBuffer = heap_caps_malloc( uxQueueLength * uxItemSize, ( uint32_t ) uxMemoryCaps );
        }

        എങ്കിൽ( ( pxQueueBuffer == ശൂന്യ ) || ( ( uxItemSize > 0 ) && ( pucQueueStorageBuffer == ശൂന്യ ) ) )
        {
            പോകുക err;
        }

        /* Create the queue using static creation API */
        xQueue = xQueueCreateStatic( uxQueueLength, uxItemSize, pucQueueStorageBuffer, pxQueueBuffer );

        എങ്കിൽ( xQueue == ശൂന്യ )
        {
            പോകുക err;
        }

        തിരികെ xQueue;

err:
        heap_caps_free( pucQueueStorageBuffer );
        heap_caps_free( pxQueueBuffer );
        തിരികെ ശൂന്യ;
    }

#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#എങ്കിൽ ( configSUPPORT_STATIC_ALLOCATION == 1 )

    ശൂന്യം vQueueDeleteWithCaps( QueueHandle_t xQueue )
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

#എങ്കിൽ ( configSUPPORT_STATIC_ALLOCATION == 1 )

    SemaphoreHandle_t xSemaphoreCreateGenericWithCaps( UBaseType_t ux_Max_എണ്ണം,
                                                       UBaseType_t ux_Initial_എണ്ണം,
                                                       സ്ഥിരം uint8_t ucQueueType,
                                                       UBaseType_t uxMemoryCaps )
    {
        SemaphoreHandle_t xSemaphore;
        StaticSemaphore_t * pxSemaphoreBuffer;

        /* Allocate memory for the semaphore using the provided memory caps */
        pxSemaphoreBuffer = heap_caps_malloc( വലുപ്പം( StaticSemaphore_t ), ( uint32_t ) uxMemoryCaps );

        എങ്കിൽ( pxSemaphoreBuffer == ശൂന്യ )
        {
            തിരികെ ശൂന്യ;
        }

        /* Create the semaphore using static creation API */
        എങ്കിൽ( ucQueueType == queueQUEUE_TYPE_MUTEX )
        {
            xSemaphore = xSemaphoreCreateMutexStatic( pxSemaphoreBuffer );
        }
        അല്ലെങ്കിൽ എങ്കിൽ( ucQueueType == queueQUEUE_TYPE_COUNTING_SEMAPHORE )
        {
            xSemaphore = xSemaphoreCreateCountingStatic( ux_Max_എണ്ണം, ux_Initial_എണ്ണം, pxSemaphoreBuffer );
        }
        അല്ലെങ്കിൽ എങ്കിൽ( ucQueueType == queue_QUEUE_TYPE_ദ്വിമാന_SEMAPHORE )
        {
            xSemaphore = x_Semaphore_Create_ദ്വിമാന_Static( pxSemaphoreBuffer );
        }
        അല്ലെങ്കിൽ /* ucQueueType == queueQUEUE_TYPE_RECURSIVE_MUTEX */
        {
            xSemaphore = xSemaphoreCreateRecursiveMutexStatic( pxSemaphoreBuffer );
        }

        എങ്കിൽ( xSemaphore == ശൂന്യ )
        {
            heap_caps_free( pxSemaphoreBuffer );
        }

        തിരികെ xSemaphore;
    }

#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#എങ്കിൽ ( configSUPPORT_STATIC_ALLOCATION == 1 )

    ശൂന്യം vSemaphoreDeleteWithCaps( SemaphoreHandle_t xSemaphore )
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

#എങ്കിൽ ( configSUPPORT_STATIC_ALLOCATION == 1 )

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
        pxStaticStreamBuffer = heap_caps_malloc( വലുപ്പം( StaticStreamBuffer_t ), ( uint32_t ) uxMemoryCaps );
        pucStreamBufferStorageArea = heap_caps_malloc( xBufferSizeBytes, ( uint32_t ) uxMemoryCaps );

        എങ്കിൽ( ( pxStaticStreamBuffer == ശൂന്യ ) || ( pucStreamBufferStorageArea == ശൂന്യ ) )
        {
            പോകുക err;
        }

        /* Create the stream or message buffer using static creation API */
        എങ്കിൽ( xIsMessageBuffer == pdTRUE )
        {
            xStreamBuffer = ( StreamBufferHandle_t ) xMessageBufferCreateStatic( xBufferSizeBytes, pucStreamBufferStorageArea, pxStaticStreamBuffer );
        }
        അല്ലെങ്കിൽ
        {
            xStreamBuffer = xStreamBufferCreateStatic( xBufferSizeBytes, xTriggerLevelBytes, pucStreamBufferStorageArea, pxStaticStreamBuffer );
        }

        എങ്കിൽ( xStreamBuffer == ശൂന്യ )
        {
            പോകുക err;
        }

        തിരികെ xStreamBuffer;

err:
        heap_caps_free( pucStreamBufferStorageArea );
        heap_caps_free( pxStaticStreamBuffer );
        തിരികെ ശൂന്യ;
    }

#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#എങ്കിൽ ( configSUPPORT_STATIC_ALLOCATION == 1 )

    ശൂന്യം vStreamBufferGenericDeleteWithCaps( StreamBufferHandle_t xStreamBuffer,
                                             BaseType_t xIsMessageBuffer )
    {
        /* Return value unused if asserts are disabled */
        BaseType_t __attribute__( ( unused ) ) xResult;
        StaticStreamBuffer_t * pxStaticStreamBuffer;
        uint8_t * pucStreamBufferStorageArea;

        /* Retrieve the buffers used to create the stream or message buffer
         * before deleting it */
        എങ്കിൽ( xIsMessageBuffer == pdTRUE )
        {
            xResult = xMessageBufferGetStaticBuffers( xStreamBuffer, &pucStreamBufferStorageArea, &pxStaticStreamBuffer );
        }
        അല്ലെങ്കിൽ
        {
            xResult = xStreamBufferGetStaticBuffers( xStreamBuffer, &pucStreamBufferStorageArea, &pxStaticStreamBuffer );
        }

        configASSERT( xResult == pdTRUE );

        /* Delete the stream or message buffer */
        എങ്കിൽ( xIsMessageBuffer == pdTRUE )
        {
            vMessageBufferDelete( xStreamBuffer );
        }
        അല്ലെങ്കിൽ
        {
            vStreamBufferDelete( xStreamBuffer );
        }

        /* Free the memory buffers */
        heap_caps_free( pxStaticStreamBuffer );
        heap_caps_free( pucStreamBufferStorageArea );
    }

#endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/
