/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * This file contains the implementation for some the functions in
 * idf_additions.h
 */

#समावेश "sdkconfig.h"
#समावेश <stdint.h>
#समावेश "freertos/FreeRTOS.h"
#समावेश "freertos/task.h"
#समावेश "freertos/queue.h"
#समावेश "freertos/semphr.h"
#समावेश "freertos/stream_buffer.h"
#समावेश "freertos/message_buffer.h"
#समावेश "freertos/idf_additions.h"
#समावेश "esp_heap_caps.h"
#समावेश "esp_log.h"
#समावेश "freertos/portmacro.h"

/* -------------------------------------------- Creation With Memory Caps ------------------------------------------- */

/* ---------------------------------- Tasks --------------------------------- */

#अगर ( configSUPPORT_STATIC_ALLOCATION == 1 )

    BaseType_t xTaskCreatePinnedToCoreWithCaps( TaskFunction_t pvTaskCode,
                                                स्थिर अक्षर * स्थिर pcName,
                                                स्थिर config_ढेर_DEPTH_TYPE us_ढेर_Depth,
                                                व्योम * स्थिर pvParameters,
                                                UBaseType_t uxPriority,
                                                TaskHandle_t * स्थिर pvCreatedTask,
                                                स्थिर BaseType_t xCoreID,
                                                UBaseType_t uxMemoryCaps )
    {
        TaskHandle_t xHandle;
        StaticTask_t * px_Task_बफर;
        ढेर_Type_t * px_ढेर;

        /* Allocate memory for the task's TCB. We use pvPortMalloc() here as the
         * TCB must be in internal memory. */
        px_Task_बफर = pvPortMalloc( माप( StaticTask_t ) );

        /* Allocate memory for the task's stack using the provided memory caps
         * */
        px_ढेर = heap_caps_malloc( ( ( माप_प्रकार ) us_ढेर_Depth ) * माप( ढेर_Type_t ), ( uint32_t ) uxMemoryCaps );

        अगर( ( px_Task_बफर == शून्य ) || ( px_ढेर == शून्य ) )
        {
            जाओ err;
        }

        /* Create the task using static creation API*/
        xHandle = xTaskCreateStaticPinnedToCore( pvTaskCode, pcName, us_ढेर_Depth, pvParameters, uxPriority, px_ढेर, px_Task_बफर, xCoreID );

        अगर( xHandle == शून्य )
        {
            /* Failed to create task */
            जाओ err;
        }
        अन्यथा अगर( pvCreatedTask != शून्य )
        {
            /* Task created successfully. Return the task handle */
            *pvCreatedTask = xHandle;
        }

        वापस pdPASS;

err:
        heap_caps_free( px_ढेर );
        vPortFree( px_Task_बफर );
        वापस pdFAIL;
    }

#पूर्ण_अगर /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#अगर ( configSUPPORT_STATIC_ALLOCATION == 1 )

    अटल व्योम prvTaskDeleteWithCaps( TaskHandle_t xTaskToDelete )
    {
        /* Return value unused if asserts are disabled */
        BaseType_t __attribute__( ( unused ) ) x_परिणाम;
        StaticTask_t * px_Task_बफर;
        ढेर_Type_t * pux_ढेर_बफर;

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
        क्रम( ;; )
        {
            BaseType_t xStillOnCore = pdFALSE;

            क्रम( BaseType_t xCoreID = 0; xCoreID < config_संख्या_OF_CORES; xCoreID++ )
            {
                अगर( xTaskGetCurrentTaskHandleForCore( xCoreID ) == xTaskToDelete )
                {
                    xStillOnCore = pdTRUE;
                    अवरोध;
                }
            }

            अगर( xStillOnCore == pdFALSE )
            {
                अवरोध;
            }

            taskYIELD();
        }

        /* We can delete the task and free the memory buffers.
         * First, we must call `vTaskDelete` so that the port task delete callback is called.
         * On targets that have coprocessors, it may be possible that the stack pointer is modified (restored)
         * during this phase, hence, it must be done before getting the statuc buffers out of the task. */
        vTaskDelete( xTaskToDelete );

        /* Free the memory buffers */
        x_परिणाम = xTaskGetStaticBuffers( xTaskToDelete, &pux_ढेर_बफर, &px_Task_बफर );
        configASSERT( x_परिणाम == pdTRUE );
        configASSERT( pux_ढेर_बफर != शून्य );
        configASSERT( px_Task_बफर != शून्य );

        heap_caps_free( pux_ढेर_बफर );
        vPortFree( px_Task_बफर );
    }

    अटल व्योम prvTaskDeleteWithCapsTask( व्योम * pvParameters )
    {
        TaskHandle_t xTaskToDelete = ( TaskHandle_t ) pvParameters;

        /* Delete the WithCaps task */
        prvTaskDeleteWithCaps( xTaskToDelete );

        /* Delete the temporary clean up task */
        vTaskDelete( शून्य );
    }

    व्योम vTaskDeleteWithCaps( TaskHandle_t xTaskToDelete )
    {
        /* THIS FUNCTION SHOULD NOT BE CALLED FROM AN INTERRUPT CONTEXT. */
        /* TODO: Update it to use portASSERT_IF_IN_ISR() instead. (IDF-10540) */
        vPortAssertIfInISR();

        TaskHandle_t xCurrentTaskHandle = xTaskGetCurrentTaskHandle();
        configASSERT( xCurrentTaskHandle != शून्य );

        अगर( ( xTaskToDelete == शून्य ) || ( xTaskToDelete == xCurrentTaskHandle ) )
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
            अगर( xTaskCreatePinnedToCore( ( TaskFunction_t ) prvTaskDeleteWithCapsTask, "prvTaskDeleteWithCapsTask", config_MINIMAL_ढेर_आकार, xCurrentTaskHandle, uxTaskPriorityGet( xTaskToDelete ), शून्य, xPortGetCoreID() ) != pdFAIL )
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

#पूर्ण_अगर /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

/* ---------------------------------- Queue --------------------------------- */

#अगर ( configSUPPORT_STATIC_ALLOCATION == 1 )

    QueueHandle_t xQueueCreateWithCaps( UBaseType_t ux_Queue_लंबाई,
                                        UBaseType_t ux_Item_आकार,
                                        UBaseType_t uxMemoryCaps )
    {
        QueueHandle_t xQueue;
        StaticQueue_t * px_Queue_बफर;
        uint8_t * puc_Queue_Storage_बफर;

        /* Allocate memory for the queue using the provided memory caps */
        px_Queue_बफर = heap_caps_malloc( माप( StaticQueue_t ), ( uint32_t ) uxMemoryCaps );

        अगर( ux_Item_आकार == 0 )
        {
            puc_Queue_Storage_बफर = शून्य;
        }
        अन्यथा
        {
            puc_Queue_Storage_बफर = heap_caps_malloc( ux_Queue_लंबाई * ux_Item_आकार, ( uint32_t ) uxMemoryCaps );
        }

        अगर( ( px_Queue_बफर == शून्य ) || ( ( ux_Item_आकार > 0 ) && ( puc_Queue_Storage_बफर == शून्य ) ) )
        {
            जाओ err;
        }

        /* Create the queue using static creation API */
        xQueue = xQueueCreateStatic( ux_Queue_लंबाई, ux_Item_आकार, puc_Queue_Storage_बफर, px_Queue_बफर );

        अगर( xQueue == शून्य )
        {
            जाओ err;
        }

        वापस xQueue;

err:
        heap_caps_free( puc_Queue_Storage_बफर );
        heap_caps_free( px_Queue_बफर );
        वापस शून्य;
    }

#पूर्ण_अगर /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#अगर ( configSUPPORT_STATIC_ALLOCATION == 1 )

    व्योम vQueueDeleteWithCaps( QueueHandle_t xQueue )
    {
        /* Return value unused if asserts are disabled */
        BaseType_t __attribute__( ( unused ) ) x_परिणाम;
        StaticQueue_t * px_Queue_बफर;
        uint8_t * puc_Queue_Storage_बफर;

        /* Retrieve the buffers used to create the queue before deleting it */
        x_परिणाम = xQueueGetStaticBuffers( xQueue, &puc_Queue_Storage_बफर, &px_Queue_बफर );
        configASSERT( x_परिणाम == pdTRUE );

        /* Delete the queue */
        vQueueDelete( xQueue );

        /* Free the memory buffers */
        heap_caps_free( px_Queue_बफर );
        heap_caps_free( puc_Queue_Storage_बफर );
    }

#पूर्ण_अगर /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

/* -------------------------------- Semaphore ------------------------------- */

#अगर ( configSUPPORT_STATIC_ALLOCATION == 1 )

    SemaphoreHandle_t xSemaphoreCreateGenericWithCaps( UBaseType_t ux_Max_गिनती,
                                                       UBaseType_t ux_Initial_गिनती,
                                                       स्थिर uint8_t ucQueueType,
                                                       UBaseType_t uxMemoryCaps )
    {
        SemaphoreHandle_t xSemaphore;
        StaticSemaphore_t * px_Semaphore_बफर;

        /* Allocate memory for the semaphore using the provided memory caps */
        px_Semaphore_बफर = heap_caps_malloc( माप( StaticSemaphore_t ), ( uint32_t ) uxMemoryCaps );

        अगर( px_Semaphore_बफर == शून्य )
        {
            वापस शून्य;
        }

        /* Create the semaphore using static creation API */
        अगर( ucQueueType == queueQUEUE_TYPE_MUTEX )
        {
            xSemaphore = xSemaphoreCreateMutexStatic( px_Semaphore_बफर );
        }
        अन्यथा अगर( ucQueueType == queueQUEUE_TYPE_COUNTING_SEMAPHORE )
        {
            xSemaphore = xSemaphoreCreateCountingStatic( ux_Max_गिनती, ux_Initial_गिनती, px_Semaphore_बफर );
        }
        अन्यथा अगर( ucQueueType == queue_QUEUE_TYPE_द्विआधारी_SEMAPHORE )
        {
            xSemaphore = x_Semaphore_Create_द्विआधारी_Static( px_Semaphore_बफर );
        }
        अन्यथा /* ucQueueType == queueQUEUE_TYPE_RECURSIVE_MUTEX */
        {
            xSemaphore = xSemaphoreCreateRecursiveMutexStatic( px_Semaphore_बफर );
        }

        अगर( xSemaphore == शून्य )
        {
            heap_caps_free( px_Semaphore_बफर );
        }

        वापस xSemaphore;
    }

#पूर्ण_अगर /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#अगर ( configSUPPORT_STATIC_ALLOCATION == 1 )

    व्योम vSemaphoreDeleteWithCaps( SemaphoreHandle_t xSemaphore )
    {
        /* Return value unused if asserts are disabled */
        BaseType_t __attribute__( ( unused ) ) x_परिणाम;
        StaticSemaphore_t * px_Semaphore_बफर;

        /* Retrieve the buffer used to create the semaphore before deleting it
         * */
        x_परिणाम = x_Semaphore_Get_Static_बफर( xSemaphore, &px_Semaphore_बफर );
        configASSERT( x_परिणाम == pdTRUE );

        /* Delete the semaphore */
        vSemaphoreDelete( xSemaphore );

        /* Free the memory buffer */
        heap_caps_free( px_Semaphore_बफर );
    }

#पूर्ण_अगर /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

/* ------------------------- Stream & Message Buffers ----------------------- */

#अगर ( configSUPPORT_STATIC_ALLOCATION == 1 )

    Stream_बफर_Handle_t x_Stream_बफर_Generic_Create_With_Caps( माप_प्रकार x_बफर_आकार_Bytes,
                                                             माप_प्रकार xTriggerLevelBytes,
                                                             BaseType_t x_Is_Message_बफर,
                                                             UBaseType_t uxMemoryCaps )
    {
        Stream_बफर_Handle_t x_Stream_बफर;
        Static_Stream_बफर_t * px_Static_Stream_बफर;
        uint8_t * puc_Stream_बफर_Storage_Area;

        /* Allocate memory for the stream or message buffer using the provided
         * memory caps */
        px_Static_Stream_बफर = heap_caps_malloc( माप( Static_Stream_बफर_t ), ( uint32_t ) uxMemoryCaps );
        puc_Stream_बफर_Storage_Area = heap_caps_malloc( x_बफर_आकार_Bytes, ( uint32_t ) uxMemoryCaps );

        अगर( ( px_Static_Stream_बफर == शून्य ) || ( puc_Stream_बफर_Storage_Area == शून्य ) )
        {
            जाओ err;
        }

        /* Create the stream or message buffer using static creation API */
        अगर( x_Is_Message_बफर == pdTRUE )
        {
            x_Stream_बफर = ( Stream_बफर_Handle_t ) x_Message_बफर_Create_Static( x_बफर_आकार_Bytes, puc_Stream_बफर_Storage_Area, px_Static_Stream_बफर );
        }
        अन्यथा
        {
            x_Stream_बफर = x_Stream_बफर_Create_Static( x_बफर_आकार_Bytes, xTriggerLevelBytes, puc_Stream_बफर_Storage_Area, px_Static_Stream_बफर );
        }

        अगर( x_Stream_बफर == शून्य )
        {
            जाओ err;
        }

        वापस x_Stream_बफर;

err:
        heap_caps_free( puc_Stream_बफर_Storage_Area );
        heap_caps_free( px_Static_Stream_बफर );
        वापस शून्य;
    }

#पूर्ण_अगर /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#अगर ( configSUPPORT_STATIC_ALLOCATION == 1 )

    व्योम v_Stream_बफर_Generic_Delete_With_Caps( Stream_बफर_Handle_t x_Stream_बफर,
                                             BaseType_t x_Is_Message_बफर )
    {
        /* Return value unused if asserts are disabled */
        BaseType_t __attribute__( ( unused ) ) x_परिणाम;
        Static_Stream_बफर_t * px_Static_Stream_बफर;
        uint8_t * puc_Stream_बफर_Storage_Area;

        /* Retrieve the buffers used to create the stream or message buffer
         * before deleting it */
        अगर( x_Is_Message_बफर == pdTRUE )
        {
            x_परिणाम = x_Message_बफर_Get_Static_Buffers( x_Stream_बफर, &puc_Stream_बफर_Storage_Area, &px_Static_Stream_बफर );
        }
        अन्यथा
        {
            x_परिणाम = x_Stream_बफर_Get_Static_Buffers( x_Stream_बफर, &puc_Stream_बफर_Storage_Area, &px_Static_Stream_बफर );
        }

        configASSERT( x_परिणाम == pdTRUE );

        /* Delete the stream or message buffer */
        अगर( x_Is_Message_बफर == pdTRUE )
        {
            v_Message_बफर_Delete( x_Stream_बफर );
        }
        अन्यथा
        {
            v_Stream_बफर_Delete( x_Stream_बफर );
        }

        /* Free the memory buffers */
        heap_caps_free( px_Static_Stream_बफर );
        heap_caps_free( puc_Stream_बफर_Storage_Area );
    }

#पूर्ण_अगर /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/
