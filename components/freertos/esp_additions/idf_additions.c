/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * This file contains the implementation for some the functions in
 * idf_additions.h
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/queue.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/semphr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/stream_buffer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/message_buffer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/idf_additions.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_heap_caps.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_log.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/portmacro.h"

/* -------------------------------------------- Creation With Memory Caps ------------------------------------------- */

/* ---------------------------------- Tasks --------------------------------- */

#ਜੇ ( config_SUPPORT_STATIC_ਵੰਡ == 1 )

    ਅਧਾਰ_Type_t x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core_With_Caps( TaskFunction_t pv_Task_ਕੋਡ,
                                                ਅਚਲ ਅੱਖਰ * ਅਚਲ pcName,
                                                ਅਚਲ config_ਢੇਰ_ਡੂੰਘਾਈ_TYPE us_ਢੇਰ_ਡੂੰਘਾਈ,
                                                ਖਾਲੀ * ਅਚਲ pvParameters,
                                                UBaseType_t ux_ਤਰਜੀਹ,
                                                TaskHandle_t * ਅਚਲ pvCreatedTask,
                                                ਅਚਲ ਅਧਾਰ_Type_t xCoreID,
                                                UBaseType_t ux_ਸਮ੍ਰਿਤੀ_Caps )
    {
        TaskHandle_t xHandle;
        StaticTask_t * px_Task_ਬਫਰ;
        ਢੇਰ_Type_t * px_ਢੇਰ;

        /* Allocate memory for the task's TCB. We use pvPortMalloc() here as the
         * TCB must be in internal memory. */
        px_Task_ਬਫਰ = pv_ਪੋਰਟ_Malloc( ਆਕਾਰ( StaticTask_t ) );

        /* Allocate memory for the task's stack using the provided memory caps
         * */
        px_ਢੇਰ = ਢੇਰ_caps_malloc( ( ( size_t ) us_ਢੇਰ_ਡੂੰਘਾਈ ) * ਆਕਾਰ( ਢੇਰ_Type_t ), ( uint32_t ) ux_ਸਮ੍ਰਿਤੀ_Caps );

        ਜੇ( ( px_Task_ਬਫਰ == ਨਿਰਾ ) || ( px_ਢੇਰ == ਨਿਰਾ ) )
        {
            ਜਾਓ err;
        }

        /* Create the task using static creation API*/
        xHandle = x_Task_Create_Static_Pinned_ਮੰਜ਼ਿਲ_Core( pv_Task_ਕੋਡ, pcName, us_ਢੇਰ_ਡੂੰਘਾਈ, pvParameters, ux_ਤਰਜੀਹ, px_ਢੇਰ, px_Task_ਬਫਰ, xCoreID );

        ਜੇ( xHandle == ਨਿਰਾ )
        {
            /* Failed to create task */
            ਜਾਓ err;
        }
        ਨਹੀਂਤਾਂ ਜੇ( pvCreatedTask != ਨਿਰਾ )
        {
            /* Task created successfully. Return the task handle */
            *pvCreatedTask = xHandle;
        }

        ਵਾਪਸ pdPASS;

err:
        ਢੇਰ_caps_free( px_ਢੇਰ );
        v_ਪੋਰਟ_Free( px_Task_ਬਫਰ );
        ਵਾਪਸ pdFAIL;
    }

#ਜੇ_ਅੰਤ /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#ਜੇ ( config_SUPPORT_STATIC_ਵੰਡ == 1 )

    ਸਥਿਰ ਖਾਲੀ prvTaskDeleteWithCaps( TaskHandle_t x_Task_ਮੰਜ਼ਿਲ_Delete )
    {
        /* Return value unused if asserts are disabled */
        ਅਧਾਰ_Type_t __attribute__( ( ਅਣਵਰਤਿਆ ) ) x_ਨਤੀਜਾ;
        StaticTask_t * px_Task_ਬਫਰ;
        ਢੇਰ_Type_t * pux_ਢੇਰ_ਬਫਰ;

        /* The task to be deleted must not be running.
         * So we suspend the task before deleting it. */
        vTaskSuspend( x_Task_ਮੰਜ਼ਿਲ_Delete );

        /* Wait until the task is no longer the current task on any core.
         *
         * Polling on pxCurrentTCBs[] (via xTaskGetCurrentTaskHandleForCore())
         * matches the predicate vTaskDelete() uses to choose between immediate
         * and IDLE-deferred TCB cleanup. By waiting on it here, we guarantee
         * that the vTaskDelete() call below takes the immediate path and that
         * prvDeleteTCB() runs synchronously before vTaskDelete() returns, so
         * the heap_caps_free() / vPortFree() that follow cannot race with the
         * IDLE task. */
        ਲਈ( ;; )
        {
            ਅਧਾਰ_Type_t xStillOnCore = pdFALSE;

            ਲਈ( ਅਧਾਰ_Type_t xCoreID = 0; xCoreID < config_ਸੰਖਿਆ_ਦਾ_CORES; xCoreID++ )
            {
                ਜੇ( xTaskGetCurrentTaskHandleForCore( xCoreID ) == x_Task_ਮੰਜ਼ਿਲ_Delete )
                {
                    xStillOnCore = pdTRUE;
                    ਰੋਕੋ;
                }
            }

            ਜੇ( xStillOnCore == pdFALSE )
            {
                ਰੋਕੋ;
            }

            taskYIELD();
        }

        /* We can delete the task and free the memory buffers.
         * First, we must call `vTaskDelete` so that the port task delete callback is called.
         * On targets that have coprocessors, it may be possible that the stack pointer is modified (restored)
         * during this phase, hence, it must be done before getting the statuc buffers out of the task. */
        vTaskDelete( x_Task_ਮੰਜ਼ਿਲ_Delete );

        /* Free the memory buffers */
        x_ਨਤੀਜਾ = xTaskGetStaticBuffers( x_Task_ਮੰਜ਼ਿਲ_Delete, &pux_ਢੇਰ_ਬਫਰ, &px_Task_ਬਫਰ );
        configASSERT( x_ਨਤੀਜਾ == pdTRUE );
        configASSERT( pux_ਢੇਰ_ਬਫਰ != ਨਿਰਾ );
        configASSERT( px_Task_ਬਫਰ != ਨਿਰਾ );

        ਢੇਰ_caps_free( pux_ਢੇਰ_ਬਫਰ );
        v_ਪੋਰਟ_Free( px_Task_ਬਫਰ );
    }

    ਸਥਿਰ ਖਾਲੀ prvTaskDeleteWithCapsTask( ਖਾਲੀ * pvParameters )
    {
        TaskHandle_t x_Task_ਮੰਜ਼ਿਲ_Delete = ( TaskHandle_t ) pvParameters;

        /* Delete the WithCaps task */
        prvTaskDeleteWithCaps( x_Task_ਮੰਜ਼ਿਲ_Delete );

        /* Delete the temporary clean up task */
        vTaskDelete( ਨਿਰਾ );
    }

    ਖਾਲੀ vTaskDeleteWithCaps( TaskHandle_t x_Task_ਮੰਜ਼ਿਲ_Delete )
    {
        /* THIS FUNCTION SHOULD NOT BE CALLED FROM AN INTERRUPT CONTEXT. */
        /* TODO: Update it to use portASSERT_IF_IN_ISR() instead. (IDF-10540) */
        v_ਪੋਰਟ_Assert_If_ਵਿੱਚ_ISR();

        TaskHandle_t xCurrentTaskHandle = xTaskGetCurrentTaskHandle();
        configASSERT( xCurrentTaskHandle != ਨਿਰਾ );

        ਜੇ( ( x_Task_ਮੰਜ਼ਿਲ_Delete == ਨਿਰਾ ) || ( x_Task_ਮੰਜ਼ਿਲ_Delete == xCurrentTaskHandle ) )
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
            ਜੇ( x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core( ( TaskFunction_t ) prvTaskDeleteWithCapsTask, "prvTaskDeleteWithCapsTask", config_ਨਿਊਨਤਮ_ਢੇਰ_ਮਾਪ, xCurrentTaskHandle, ux_Task_ਤਰਜੀਹ_Get( x_Task_ਮੰਜ਼ਿਲ_Delete ), ਨਿਰਾ, x_ਪੋਰਟ_Get_Core_ID() ) != pdFAIL )
            {
                /* Although the current task should get preemted immediately when prvTaskDeleteWithCapsTask is created,
                 * for safety, we suspend the current task and wait for prvTaskDeleteWithCapsTask to delete it. */
                vTaskSuspend( x_Task_ਮੰਜ਼ਿਲ_Delete );

                /* Should never reach here */
                ESP_LOGE( "freertos_additions", "%s: Failed to suspend the task to be deleted", __func__ );
                abort();
            }
            ਨਹੀਂਤਾਂ
            {
                /* Failed to create the task to delete the current task. */
                ESP_LOGE( "freertos_additions", "%s: Failed to create the task to delete the current task", __func__ );
                abort();
            }
        }

        /* Delete the WithCaps task */
        prvTaskDeleteWithCaps( x_Task_ਮੰਜ਼ਿਲ_Delete );
    }

#ਜੇ_ਅੰਤ /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

/* ---------------------------------- Queue --------------------------------- */

#ਜੇ ( config_SUPPORT_STATIC_ਵੰਡ == 1 )

    ਕਤਾਰ_Handle_t x_ਕਤਾਰ_Create_With_Caps( UBaseType_t ux_ਕਤਾਰ_ਲੰਮਾਈ,
                                        UBaseType_t ux_ਵਸਤੂ_ਮਾਪ,
                                        UBaseType_t ux_ਸਮ੍ਰਿਤੀ_Caps )
    {
        ਕਤਾਰ_Handle_t x_ਕਤਾਰ;
        Static_ਕਤਾਰ_t * px_ਕਤਾਰ_ਬਫਰ;
        uint8_t * puc_ਕਤਾਰ_Storage_ਬਫਰ;

        /* Allocate memory for the queue using the provided memory caps */
        px_ਕਤਾਰ_ਬਫਰ = ਢੇਰ_caps_malloc( ਆਕਾਰ( Static_ਕਤਾਰ_t ), ( uint32_t ) ux_ਸਮ੍ਰਿਤੀ_Caps );

        ਜੇ( ux_ਵਸਤੂ_ਮਾਪ == 0 )
        {
            puc_ਕਤਾਰ_Storage_ਬਫਰ = ਨਿਰਾ;
        }
        ਨਹੀਂਤਾਂ
        {
            puc_ਕਤਾਰ_Storage_ਬਫਰ = ਢੇਰ_caps_malloc( ux_ਕਤਾਰ_ਲੰਮਾਈ * ux_ਵਸਤੂ_ਮਾਪ, ( uint32_t ) ux_ਸਮ੍ਰਿਤੀ_Caps );
        }

        ਜੇ( ( px_ਕਤਾਰ_ਬਫਰ == ਨਿਰਾ ) || ( ( ux_ਵਸਤੂ_ਮਾਪ > 0 ) && ( puc_ਕਤਾਰ_Storage_ਬਫਰ == ਨਿਰਾ ) ) )
        {
            ਜਾਓ err;
        }

        /* Create the queue using static creation API */
        x_ਕਤਾਰ = x_ਕਤਾਰ_Create_Static( ux_ਕਤਾਰ_ਲੰਮਾਈ, ux_ਵਸਤੂ_ਮਾਪ, puc_ਕਤਾਰ_Storage_ਬਫਰ, px_ਕਤਾਰ_ਬਫਰ );

        ਜੇ( x_ਕਤਾਰ == ਨਿਰਾ )
        {
            ਜਾਓ err;
        }

        ਵਾਪਸ x_ਕਤਾਰ;

err:
        ਢੇਰ_caps_free( puc_ਕਤਾਰ_Storage_ਬਫਰ );
        ਢੇਰ_caps_free( px_ਕਤਾਰ_ਬਫਰ );
        ਵਾਪਸ ਨਿਰਾ;
    }

#ਜੇ_ਅੰਤ /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#ਜੇ ( config_SUPPORT_STATIC_ਵੰਡ == 1 )

    ਖਾਲੀ v_ਕਤਾਰ_Delete_With_Caps( ਕਤਾਰ_Handle_t x_ਕਤਾਰ )
    {
        /* Return value unused if asserts are disabled */
        ਅਧਾਰ_Type_t __attribute__( ( ਅਣਵਰਤਿਆ ) ) x_ਨਤੀਜਾ;
        Static_ਕਤਾਰ_t * px_ਕਤਾਰ_ਬਫਰ;
        uint8_t * puc_ਕਤਾਰ_Storage_ਬਫਰ;

        /* Retrieve the buffers used to create the queue before deleting it */
        x_ਨਤੀਜਾ = x_ਕਤਾਰ_Get_Static_Buffers( x_ਕਤਾਰ, &puc_ਕਤਾਰ_Storage_ਬਫਰ, &px_ਕਤਾਰ_ਬਫਰ );
        configASSERT( x_ਨਤੀਜਾ == pdTRUE );

        /* Delete the queue */
        v_ਕਤਾਰ_Delete( x_ਕਤਾਰ );

        /* Free the memory buffers */
        ਢੇਰ_caps_free( px_ਕਤਾਰ_ਬਫਰ );
        ਢੇਰ_caps_free( puc_ਕਤਾਰ_Storage_ਬਫਰ );
    }

#ਜੇ_ਅੰਤ /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

/* -------------------------------- Semaphore ------------------------------- */

#ਜੇ ( config_SUPPORT_STATIC_ਵੰਡ == 1 )

    SemaphoreHandle_t xSemaphoreCreateGenericWithCaps( UBaseType_t ux_Max_ਗਿਣਤੀ,
                                                       UBaseType_t ux_Initial_ਗਿਣਤੀ,
                                                       ਅਚਲ uint8_t uc_ਕਤਾਰ_Type,
                                                       UBaseType_t ux_ਸਮ੍ਰਿਤੀ_Caps )
    {
        SemaphoreHandle_t xSemaphore;
        StaticSemaphore_t * px_Semaphore_ਬਫਰ;

        /* Allocate memory for the semaphore using the provided memory caps */
        px_Semaphore_ਬਫਰ = ਢੇਰ_caps_malloc( ਆਕਾਰ( StaticSemaphore_t ), ( uint32_t ) ux_ਸਮ੍ਰਿਤੀ_Caps );

        ਜੇ( px_Semaphore_ਬਫਰ == ਨਿਰਾ )
        {
            ਵਾਪਸ ਨਿਰਾ;
        }

        /* Create the semaphore using static creation API */
        ਜੇ( uc_ਕਤਾਰ_Type == ਕਤਾਰ_ਕਤਾਰ_TYPE_MUTEX )
        {
            xSemaphore = xSemaphoreCreateMutexStatic( px_Semaphore_ਬਫਰ );
        }
        ਨਹੀਂਤਾਂ ਜੇ( uc_ਕਤਾਰ_Type == ਕਤਾਰ_ਕਤਾਰ_TYPE_COUNTING_SEMAPHORE )
        {
            xSemaphore = xSemaphoreCreateCountingStatic( ux_Max_ਗਿਣਤੀ, ux_Initial_ਗਿਣਤੀ, px_Semaphore_ਬਫਰ );
        }
        ਨਹੀਂਤਾਂ ਜੇ( uc_ਕਤਾਰ_Type == ਕਤਾਰ_ਕਤਾਰ_TYPE_ਦੁਆਧਾਰੀ_SEMAPHORE )
        {
            xSemaphore = x_Semaphore_Create_ਦੁਆਧਾਰੀ_Static( px_Semaphore_ਬਫਰ );
        }
        ਨਹੀਂਤਾਂ /* ucQueueType == queueQUEUE_TYPE_RECURSIVE_MUTEX */
        {
            xSemaphore = xSemaphoreCreateRecursiveMutexStatic( px_Semaphore_ਬਫਰ );
        }

        ਜੇ( xSemaphore == ਨਿਰਾ )
        {
            ਢੇਰ_caps_free( px_Semaphore_ਬਫਰ );
        }

        ਵਾਪਸ xSemaphore;
    }

#ਜੇ_ਅੰਤ /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#ਜੇ ( config_SUPPORT_STATIC_ਵੰਡ == 1 )

    ਖਾਲੀ vSemaphoreDeleteWithCaps( SemaphoreHandle_t xSemaphore )
    {
        /* Return value unused if asserts are disabled */
        ਅਧਾਰ_Type_t __attribute__( ( ਅਣਵਰਤਿਆ ) ) x_ਨਤੀਜਾ;
        StaticSemaphore_t * px_Semaphore_ਬਫਰ;

        /* Retrieve the buffer used to create the semaphore before deleting it
         * */
        x_ਨਤੀਜਾ = x_Semaphore_Get_Static_ਬਫਰ( xSemaphore, &px_Semaphore_ਬਫਰ );
        configASSERT( x_ਨਤੀਜਾ == pdTRUE );

        /* Delete the semaphore */
        vSemaphoreDelete( xSemaphore );

        /* Free the memory buffer */
        ਢੇਰ_caps_free( px_Semaphore_ਬਫਰ );
    }

#ਜੇ_ਅੰਤ /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

/* ------------------------- Stream & Message Buffers ----------------------- */

#ਜੇ ( config_SUPPORT_STATIC_ਵੰਡ == 1 )

    ਧਾਰਾ_ਬਫਰ_Handle_t x_ਧਾਰਾ_ਬਫਰ_Generic_Create_With_Caps( size_t x_ਬਫਰ_ਮਾਪ_ਬਾਈਟਾਂ,
                                                             size_t x_Trigger_Level_ਬਾਈਟਾਂ,
                                                             ਅਧਾਰ_Type_t x_Is_ਸੁਨੇਹਾ_ਬਫਰ,
                                                             UBaseType_t ux_ਸਮ੍ਰਿਤੀ_Caps )
    {
        ਧਾਰਾ_ਬਫਰ_Handle_t x_ਧਾਰਾ_ਬਫਰ;
        Static_ਧਾਰਾ_ਬਫਰ_t * px_Static_ਧਾਰਾ_ਬਫਰ;
        uint8_t * puc_ਧਾਰਾ_ਬਫਰ_Storage_Area;

        /* Allocate memory for the stream or message buffer using the provided
         * memory caps */
        px_Static_ਧਾਰਾ_ਬਫਰ = ਢੇਰ_caps_malloc( ਆਕਾਰ( Static_ਧਾਰਾ_ਬਫਰ_t ), ( uint32_t ) ux_ਸਮ੍ਰਿਤੀ_Caps );
        puc_ਧਾਰਾ_ਬਫਰ_Storage_Area = ਢੇਰ_caps_malloc( x_ਬਫਰ_ਮਾਪ_ਬਾਈਟਾਂ, ( uint32_t ) ux_ਸਮ੍ਰਿਤੀ_Caps );

        ਜੇ( ( px_Static_ਧਾਰਾ_ਬਫਰ == ਨਿਰਾ ) || ( puc_ਧਾਰਾ_ਬਫਰ_Storage_Area == ਨਿਰਾ ) )
        {
            ਜਾਓ err;
        }

        /* Create the stream or message buffer using static creation API */
        ਜੇ( x_Is_ਸੁਨੇਹਾ_ਬਫਰ == pdTRUE )
        {
            x_ਧਾਰਾ_ਬਫਰ = ( ਧਾਰਾ_ਬਫਰ_Handle_t ) x_ਸੁਨੇਹਾ_ਬਫਰ_Create_Static( x_ਬਫਰ_ਮਾਪ_ਬਾਈਟਾਂ, puc_ਧਾਰਾ_ਬਫਰ_Storage_Area, px_Static_ਧਾਰਾ_ਬਫਰ );
        }
        ਨਹੀਂਤਾਂ
        {
            x_ਧਾਰਾ_ਬਫਰ = x_ਧਾਰਾ_ਬਫਰ_Create_Static( x_ਬਫਰ_ਮਾਪ_ਬਾਈਟਾਂ, x_Trigger_Level_ਬਾਈਟਾਂ, puc_ਧਾਰਾ_ਬਫਰ_Storage_Area, px_Static_ਧਾਰਾ_ਬਫਰ );
        }

        ਜੇ( x_ਧਾਰਾ_ਬਫਰ == ਨਿਰਾ )
        {
            ਜਾਓ err;
        }

        ਵਾਪਸ x_ਧਾਰਾ_ਬਫਰ;

err:
        ਢੇਰ_caps_free( puc_ਧਾਰਾ_ਬਫਰ_Storage_Area );
        ਢੇਰ_caps_free( px_Static_ਧਾਰਾ_ਬਫਰ );
        ਵਾਪਸ ਨਿਰਾ;
    }

#ਜੇ_ਅੰਤ /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#ਜੇ ( config_SUPPORT_STATIC_ਵੰਡ == 1 )

    ਖਾਲੀ v_ਧਾਰਾ_ਬਫਰ_Generic_Delete_With_Caps( ਧਾਰਾ_ਬਫਰ_Handle_t x_ਧਾਰਾ_ਬਫਰ,
                                             ਅਧਾਰ_Type_t x_Is_ਸੁਨੇਹਾ_ਬਫਰ )
    {
        /* Return value unused if asserts are disabled */
        ਅਧਾਰ_Type_t __attribute__( ( ਅਣਵਰਤਿਆ ) ) x_ਨਤੀਜਾ;
        Static_ਧਾਰਾ_ਬਫਰ_t * px_Static_ਧਾਰਾ_ਬਫਰ;
        uint8_t * puc_ਧਾਰਾ_ਬਫਰ_Storage_Area;

        /* Retrieve the buffers used to create the stream or message buffer
         * before deleting it */
        ਜੇ( x_Is_ਸੁਨੇਹਾ_ਬਫਰ == pdTRUE )
        {
            x_ਨਤੀਜਾ = x_ਸੁਨੇਹਾ_ਬਫਰ_Get_Static_Buffers( x_ਧਾਰਾ_ਬਫਰ, &puc_ਧਾਰਾ_ਬਫਰ_Storage_Area, &px_Static_ਧਾਰਾ_ਬਫਰ );
        }
        ਨਹੀਂਤਾਂ
        {
            x_ਨਤੀਜਾ = x_ਧਾਰਾ_ਬਫਰ_Get_Static_Buffers( x_ਧਾਰਾ_ਬਫਰ, &puc_ਧਾਰਾ_ਬਫਰ_Storage_Area, &px_Static_ਧਾਰਾ_ਬਫਰ );
        }

        configASSERT( x_ਨਤੀਜਾ == pdTRUE );

        /* Delete the stream or message buffer */
        ਜੇ( x_Is_ਸੁਨੇਹਾ_ਬਫਰ == pdTRUE )
        {
            v_ਸੁਨੇਹਾ_ਬਫਰ_Delete( x_ਧਾਰਾ_ਬਫਰ );
        }
        ਨਹੀਂਤਾਂ
        {
            v_ਧਾਰਾ_ਬਫਰ_Delete( x_ਧਾਰਾ_ਬਫਰ );
        }

        /* Free the memory buffers */
        ਢੇਰ_caps_free( px_Static_ਧਾਰਾ_ਬਫਰ );
        ਢੇਰ_caps_free( puc_ਧਾਰਾ_ਬਫਰ_Storage_Area );
    }

#ਜੇ_ਅੰਤ /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/
