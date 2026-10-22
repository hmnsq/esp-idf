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

#यदि ( config_SUPPORT_STATIC_आवण्टनम् == 1 )

    आधारः_Type_t x_Task_Create_Pinned_गन्तव्यम्_Core_With_Caps( TaskFunction_t pv_Task_संहिता,
                                                ध्रुव वर्ण * ध्रुव pcName,
                                                ध्रुव config_राशिः_गभीरता_TYPE us_राशिः_गभीरता,
                                                शून्य * ध्रुव pvParameters,
                                                UBaseType_t ux_प्राथमिकता,
                                                TaskHandle_t * ध्रुव pvCreatedTask,
                                                ध्रुव आधारः_Type_t xCoreID,
                                                UBaseType_t ux_स्मृतिः_Caps )
    {
        TaskHandle_t xHandle;
        StaticTask_t * px_Task_बफर;
        राशिः_Type_t * px_राशिः;

        /* Allocate memory for the task's TCB. We use pvPortMalloc() here as the
         * TCB must be in internal memory. */
        px_Task_बफर = pv_पत्तनम्_Malloc( परिमाण( StaticTask_t ) );

        /* Allocate memory for the task's stack using the provided memory caps
         * */
        px_राशिः = heap_caps_malloc( ( ( size_t ) us_राशिः_गभीरता ) * परिमाण( राशिः_Type_t ), ( uint32_t ) ux_स्मृतिः_Caps );

        यदि( ( px_Task_बफर == रिक्त ) || ( px_राशिः == रिक्त ) )
        {
            गच्छ err;
        }

        /* Create the task using static creation API*/
        xHandle = x_Task_Create_Static_Pinned_गन्तव्यम्_Core( pv_Task_संहिता, pcName, us_राशिः_गभीरता, pvParameters, ux_प्राथमिकता, px_राशिः, px_Task_बफर, xCoreID );

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

        प्रत्यागम pdPASS;

err:
        heap_caps_free( px_राशिः );
        v_पत्तनम्_Free( px_Task_बफर );
        प्रत्यागम pdFAIL;
    }

#यदि_अन्तः /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#यदि ( config_SUPPORT_STATIC_आवण्टनम् == 1 )

    स्थिर शून्य prvTaskDeleteWithCaps( TaskHandle_t x_Task_गन्तव्यम्_Delete )
    {
        /* Return value unused if asserts are disabled */
        आधारः_Type_t __attribute__( ( अप्रयुक्तम् ) ) x_परिणाम;
        StaticTask_t * px_Task_बफर;
        राशिः_Type_t * pux_राशिः_बफर;

        /* The task to be deleted must not be running.
         * So we suspend the task before deleting it. */
        vTaskSuspend( x_Task_गन्तव्यम्_Delete );

        /* Wait until the task is no longer the current task on any core.
         *
         * Polling on pxCurrentTCBs[] (via xTaskGetCurrentTaskHandleForCore())
         * matches the predicate vTaskDelete() uses to choose between immediate
         * and IDLE-deferred TCB cleanup. By waiting on it here, we guarantee
         * that the vTaskDelete() call below takes the immediate path and that
         * prvDeleteTCB() runs synchronously before vTaskDelete() returns, so
         * the heap_caps_free() / vPortFree() that follow cannot race with the
         * IDLE task. */
        प्रति( ;; )
        {
            आधारः_Type_t xStillOnCore = pdFALSE;

            प्रति( आधारः_Type_t xCoreID = 0; xCoreID < config_सङ्ख्या_सम्बन्धि_CORES; xCoreID++ )
            {
                यदि( xTaskGetCurrentTaskHandleForCore( xCoreID ) == x_Task_गन्तव्यम्_Delete )
                {
                    xStillOnCore = pdTRUE;
                    भङ्ग;
                }
            }

            यदि( xStillOnCore == pdFALSE )
            {
                भङ्ग;
            }

            taskYIELD();
        }

        /* We can delete the task and free the memory buffers.
         * First, we must call `vTaskDelete` so that the port task delete callback is called.
         * On targets that have coprocessors, it may be possible that the stack pointer is modified (restored)
         * during this phase, hence, it must be done before getting the statuc buffers out of the task. */
        vTaskDelete( x_Task_गन्तव्यम्_Delete );

        /* Free the memory buffers */
        x_परिणाम = xTaskGetStaticBuffers( x_Task_गन्तव्यम्_Delete, &pux_राशिः_बफर, &px_Task_बफर );
        configASSERT( x_परिणाम == pdTRUE );
        configASSERT( pux_राशिः_बफर != रिक्त );
        configASSERT( px_Task_बफर != रिक्त );

        heap_caps_free( pux_राशिः_बफर );
        v_पत्तनम्_Free( px_Task_बफर );
    }

    स्थिर शून्य prvTaskDeleteWithCapsTask( शून्य * pvParameters )
    {
        TaskHandle_t x_Task_गन्तव्यम्_Delete = ( TaskHandle_t ) pvParameters;

        /* Delete the WithCaps task */
        prvTaskDeleteWithCaps( x_Task_गन्तव्यम्_Delete );

        /* Delete the temporary clean up task */
        vTaskDelete( रिक्त );
    }

    शून्य vTaskDeleteWithCaps( TaskHandle_t x_Task_गन्तव्यम्_Delete )
    {
        /* THIS FUNCTION SHOULD NOT BE CALLED FROM AN INTERRUPT CONTEXT. */
        /* TODO: Update it to use portASSERT_IF_IN_ISR() instead. (IDF-10540) */
        v_पत्तनम्_Assert_If_अन्तर्_ISR();

        TaskHandle_t xCurrentTaskHandle = xTaskGetCurrentTaskHandle();
        configASSERT( xCurrentTaskHandle != रिक्त );

        यदि( ( x_Task_गन्तव्यम्_Delete == रिक्त ) || ( x_Task_गन्तव्यम्_Delete == xCurrentTaskHandle ) )
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
            यदि( x_Task_Create_Pinned_गन्तव्यम्_Core( ( TaskFunction_t ) prvTaskDeleteWithCapsTask, "prvTaskDeleteWithCapsTask", config_अल्पिष्ठम्_राशिः_आकार, xCurrentTaskHandle, ux_Task_प्राथमिकता_Get( x_Task_गन्तव्यम्_Delete ), रिक्त, x_पत्तनम्_Get_Core_ID() ) != pdFAIL )
            {
                /* Although the current task should get preemted immediately when prvTaskDeleteWithCapsTask is created,
                 * for safety, we suspend the current task and wait for prvTaskDeleteWithCapsTask to delete it. */
                vTaskSuspend( x_Task_गन्तव्यम्_Delete );

                /* Should never reach here */
                ESP_LOGE( "freertos_additions", "%s: Failed to suspend the task to be deleted", __func__ );
                abort();
            }
            अन्यथा
            {
                /* Failed to create the task to delete the current task. */
                ESP_LOGE( "freertos_additions", "%s: Failed to create the task to delete the current task", __func__ );
                abort();
            }
        }

        /* Delete the WithCaps task */
        prvTaskDeleteWithCaps( x_Task_गन्तव्यम्_Delete );
    }

#यदि_अन्तः /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

/* ---------------------------------- Queue --------------------------------- */

#यदि ( config_SUPPORT_STATIC_आवण्टनम् == 1 )

    QueueHandle_t xQueueCreateWithCaps( UBaseType_t ux_Queue_दीर्घता,
                                        UBaseType_t ux_Item_आकार,
                                        UBaseType_t ux_स्मृतिः_Caps )
    {
        QueueHandle_t xQueue;
        StaticQueue_t * px_Queue_बफर;
        uint8_t * puc_Queue_Storage_बफर;

        /* Allocate memory for the queue using the provided memory caps */
        px_Queue_बफर = heap_caps_malloc( परिमाण( StaticQueue_t ), ( uint32_t ) ux_स्मृतिः_Caps );

        यदि( ux_Item_आकार == 0 )
        {
            puc_Queue_Storage_बफर = रिक्त;
        }
        अन्यथा
        {
            puc_Queue_Storage_बफर = heap_caps_malloc( ux_Queue_दीर्घता * ux_Item_आकार, ( uint32_t ) ux_स्मृतिः_Caps );
        }

        यदि( ( px_Queue_बफर == रिक्त ) || ( ( ux_Item_आकार > 0 ) && ( puc_Queue_Storage_बफर == रिक्त ) ) )
        {
            गच्छ err;
        }

        /* Create the queue using static creation API */
        xQueue = xQueueCreateStatic( ux_Queue_दीर्घता, ux_Item_आकार, puc_Queue_Storage_बफर, px_Queue_बफर );

        यदि( xQueue == रिक्त )
        {
            गच्छ err;
        }

        प्रत्यागम xQueue;

err:
        heap_caps_free( puc_Queue_Storage_बफर );
        heap_caps_free( px_Queue_बफर );
        प्रत्यागम रिक्त;
    }

#यदि_अन्तः /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#यदि ( config_SUPPORT_STATIC_आवण्टनम् == 1 )

    शून्य vQueueDeleteWithCaps( QueueHandle_t xQueue )
    {
        /* Return value unused if asserts are disabled */
        आधारः_Type_t __attribute__( ( अप्रयुक्तम् ) ) x_परिणाम;
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

#यदि_अन्तः /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

/* -------------------------------- Semaphore ------------------------------- */

#यदि ( config_SUPPORT_STATIC_आवण्टनम् == 1 )

    SemaphoreHandle_t xSemaphoreCreateGenericWithCaps( UBaseType_t ux_Max_गणनम्,
                                                       UBaseType_t ux_Initial_गणनम्,
                                                       ध्रुव uint8_t ucQueueType,
                                                       UBaseType_t ux_स्मृतिः_Caps )
    {
        SemaphoreHandle_t xSemaphore;
        StaticSemaphore_t * px_Semaphore_बफर;

        /* Allocate memory for the semaphore using the provided memory caps */
        px_Semaphore_बफर = heap_caps_malloc( परिमाण( StaticSemaphore_t ), ( uint32_t ) ux_स्मृतिः_Caps );

        यदि( px_Semaphore_बफर == रिक्त )
        {
            प्रत्यागम रिक्त;
        }

        /* Create the semaphore using static creation API */
        यदि( ucQueueType == queueQUEUE_TYPE_MUTEX )
        {
            xSemaphore = xSemaphoreCreateMutexStatic( px_Semaphore_बफर );
        }
        अन्यथा यदि( ucQueueType == queueQUEUE_TYPE_COUNTING_SEMAPHORE )
        {
            xSemaphore = xSemaphoreCreateCountingStatic( ux_Max_गणनम्, ux_Initial_गणनम्, px_Semaphore_बफर );
        }
        अन्यथा यदि( ucQueueType == queue_QUEUE_TYPE_द्विमान_SEMAPHORE )
        {
            xSemaphore = x_Semaphore_Create_द्विमान_Static( px_Semaphore_बफर );
        }
        अन्यथा /* ucQueueType == queueQUEUE_TYPE_RECURSIVE_MUTEX */
        {
            xSemaphore = xSemaphoreCreateRecursiveMutexStatic( px_Semaphore_बफर );
        }

        यदि( xSemaphore == रिक्त )
        {
            heap_caps_free( px_Semaphore_बफर );
        }

        प्रत्यागम xSemaphore;
    }

#यदि_अन्तः /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#यदि ( config_SUPPORT_STATIC_आवण्टनम् == 1 )

    शून्य vSemaphoreDeleteWithCaps( SemaphoreHandle_t xSemaphore )
    {
        /* Return value unused if asserts are disabled */
        आधारः_Type_t __attribute__( ( अप्रयुक्तम् ) ) x_परिणाम;
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

#यदि_अन्तः /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

/* ------------------------- Stream & Message Buffers ----------------------- */

#यदि ( config_SUPPORT_STATIC_आवण्टनम् == 1 )

    प्रवाहः_बफर_Handle_t x_प्रवाहः_बफर_Generic_Create_With_Caps( size_t x_बफर_आकार_अष्टकानि,
                                                             size_t x_Trigger_Level_अष्टकानि,
                                                             आधारः_Type_t x_Is_सन्देशः_बफर,
                                                             UBaseType_t ux_स्मृतिः_Caps )
    {
        प्रवाहः_बफर_Handle_t x_प्रवाहः_बफर;
        Static_प्रवाहः_बफर_t * px_Static_प्रवाहः_बफर;
        uint8_t * puc_प्रवाहः_बफर_Storage_Area;

        /* Allocate memory for the stream or message buffer using the provided
         * memory caps */
        px_Static_प्रवाहः_बफर = heap_caps_malloc( परिमाण( Static_प्रवाहः_बफर_t ), ( uint32_t ) ux_स्मृतिः_Caps );
        puc_प्रवाहः_बफर_Storage_Area = heap_caps_malloc( x_बफर_आकार_अष्टकानि, ( uint32_t ) ux_स्मृतिः_Caps );

        यदि( ( px_Static_प्रवाहः_बफर == रिक्त ) || ( puc_प्रवाहः_बफर_Storage_Area == रिक्त ) )
        {
            गच्छ err;
        }

        /* Create the stream or message buffer using static creation API */
        यदि( x_Is_सन्देशः_बफर == pdTRUE )
        {
            x_प्रवाहः_बफर = ( प्रवाहः_बफर_Handle_t ) x_सन्देशः_बफर_Create_Static( x_बफर_आकार_अष्टकानि, puc_प्रवाहः_बफर_Storage_Area, px_Static_प्रवाहः_बफर );
        }
        अन्यथा
        {
            x_प्रवाहः_बफर = x_प्रवाहः_बफर_Create_Static( x_बफर_आकार_अष्टकानि, x_Trigger_Level_अष्टकानि, puc_प्रवाहः_बफर_Storage_Area, px_Static_प्रवाहः_बफर );
        }

        यदि( x_प्रवाहः_बफर == रिक्त )
        {
            गच्छ err;
        }

        प्रत्यागम x_प्रवाहः_बफर;

err:
        heap_caps_free( puc_प्रवाहः_बफर_Storage_Area );
        heap_caps_free( px_Static_प्रवाहः_बफर );
        प्रत्यागम रिक्त;
    }

#यदि_अन्तः /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#यदि ( config_SUPPORT_STATIC_आवण्टनम् == 1 )

    शून्य v_प्रवाहः_बफर_Generic_Delete_With_Caps( प्रवाहः_बफर_Handle_t x_प्रवाहः_बफर,
                                             आधारः_Type_t x_Is_सन्देशः_बफर )
    {
        /* Return value unused if asserts are disabled */
        आधारः_Type_t __attribute__( ( अप्रयुक्तम् ) ) x_परिणाम;
        Static_प्रवाहः_बफर_t * px_Static_प्रवाहः_बफर;
        uint8_t * puc_प्रवाहः_बफर_Storage_Area;

        /* Retrieve the buffers used to create the stream or message buffer
         * before deleting it */
        यदि( x_Is_सन्देशः_बफर == pdTRUE )
        {
            x_परिणाम = x_सन्देशः_बफर_Get_Static_Buffers( x_प्रवाहः_बफर, &puc_प्रवाहः_बफर_Storage_Area, &px_Static_प्रवाहः_बफर );
        }
        अन्यथा
        {
            x_परिणाम = x_प्रवाहः_बफर_Get_Static_Buffers( x_प्रवाहः_बफर, &puc_प्रवाहः_बफर_Storage_Area, &px_Static_प्रवाहः_बफर );
        }

        configASSERT( x_परिणाम == pdTRUE );

        /* Delete the stream or message buffer */
        यदि( x_Is_सन्देशः_बफर == pdTRUE )
        {
            v_सन्देशः_बफर_Delete( x_प्रवाहः_बफर );
        }
        अन्यथा
        {
            v_प्रवाहः_बफर_Delete( x_प्रवाहः_बफर );
        }

        /* Free the memory buffers */
        heap_caps_free( px_Static_प्रवाहः_बफर );
        heap_caps_free( puc_प्रवाहः_बफर_Storage_Area );
    }

#यदि_अन्तः /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/
