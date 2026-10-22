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

#ಆದರೆ ( config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 1 )

    ತಳ_Type_t x_Task_Create_Pinned_ಗಮ್ಯ_Core_With_Caps( TaskFunction_t pv_Task_ಸಂಕೇತ,
                                                ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ pcName,
                                                ಸ್ಥಿರಾಂಕ config_ರಾಶಿ_ಆಳ_TYPE us_ರಾಶಿ_ಆಳ,
                                                ಶೂನ್ಯ * ಸ್ಥಿರಾಂಕ pvParameters,
                                                UBaseType_t ux_ಆದ್ಯತೆ,
                                                TaskHandle_t * ಸ್ಥಿರಾಂಕ pvCreatedTask,
                                                ಸ್ಥಿರಾಂಕ ತಳ_Type_t xCoreID,
                                                UBaseType_t ux_ಸ್ಮರಣೆ_Caps )
    {
        TaskHandle_t xHandle;
        StaticTask_t * px_Task_ಬಫರ್;
        ರಾಶಿ_Type_t * px_ರಾಶಿ;

        /* Allocate memory for the task's TCB. We use pvPortMalloc() here as the
         * TCB must be in internal memory. */
        px_Task_ಬಫರ್ = pv_ಪೋರ್ಟ್_Malloc( ಗಾತ್ರ( StaticTask_t ) );

        /* Allocate memory for the task's stack using the provided memory caps
         * */
        px_ರಾಶಿ = ರಾಶಿ_caps_malloc( ( ( size_t ) us_ರಾಶಿ_ಆಳ ) * ಗಾತ್ರ( ರಾಶಿ_Type_t ), ( uint32_t ) ux_ಸ್ಮರಣೆ_Caps );

        ಆದರೆ( ( px_Task_ಬಫರ್ == ಖಾಲಿ ) || ( px_ರಾಶಿ == ಖಾಲಿ ) )
        {
            ಹೋಗು err;
        }

        /* Create the task using static creation API*/
        xHandle = x_Task_Create_Static_Pinned_ಗಮ್ಯ_Core( pv_Task_ಸಂಕೇತ, pcName, us_ರಾಶಿ_ಆಳ, pvParameters, ux_ಆದ್ಯತೆ, px_ರಾಶಿ, px_Task_ಬಫರ್, xCoreID );

        ಆದರೆ( xHandle == ಖಾಲಿ )
        {
            /* Failed to create task */
            ಹೋಗು err;
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( pvCreatedTask != ಖಾಲಿ )
        {
            /* Task created successfully. Return the task handle */
            *pvCreatedTask = xHandle;
        }

        ಹಿಂತಿರುಗಿ pdPASS;

err:
        ರಾಶಿ_caps_free( px_ರಾಶಿ );
        v_ಪೋರ್ಟ್_Free( px_Task_ಬಫರ್ );
        ಹಿಂತಿರುಗಿ pdFAIL;
    }

#ಆದರೆ_ಅಂತ್ಯ /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#ಆದರೆ ( config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 1 )

    ಸ್ಥಿರ ಶೂನ್ಯ prvTaskDeleteWithCaps( TaskHandle_t x_Task_ಗಮ್ಯ_Delete )
    {
        /* Return value unused if asserts are disabled */
        ತಳ_Type_t __attribute__( ( ಬಳಸದ ) ) x_ಫಲಿತಾಂಶ;
        StaticTask_t * px_Task_ಬಫರ್;
        ರಾಶಿ_Type_t * pux_ರಾಶಿ_ಬಫರ್;

        /* The task to be deleted must not be running.
         * So we suspend the task before deleting it. */
        vTaskSuspend( x_Task_ಗಮ್ಯ_Delete );

        /* Wait until the task is no longer the current task on any core.
         *
         * Polling on pxCurrentTCBs[] (via xTaskGetCurrentTaskHandleForCore())
         * matches the predicate vTaskDelete() uses to choose between immediate
         * and IDLE-deferred TCB cleanup. By waiting on it here, we guarantee
         * that the vTaskDelete() call below takes the immediate path and that
         * prvDeleteTCB() runs synchronously before vTaskDelete() returns, so
         * the heap_caps_free() / vPortFree() that follow cannot race with the
         * IDLE task. */
        ಗಾಗಿ( ;; )
        {
            ತಳ_Type_t xStillOnCore = pdFALSE;

            ಗಾಗಿ( ತಳ_Type_t xCoreID = 0; xCoreID < config_ಸಂಖ್ಯೆ_ದ_CORES; xCoreID++ )
            {
                ಆದರೆ( xTaskGetCurrentTaskHandleForCore( xCoreID ) == x_Task_ಗಮ್ಯ_Delete )
                {
                    xStillOnCore = pdTRUE;
                    ನಿಲ್ಲಿಸು;
                }
            }

            ಆದರೆ( xStillOnCore == pdFALSE )
            {
                ನಿಲ್ಲಿಸು;
            }

            taskYIELD();
        }

        /* We can delete the task and free the memory buffers.
         * First, we must call `vTaskDelete` so that the port task delete callback is called.
         * On targets that have coprocessors, it may be possible that the stack pointer is modified (restored)
         * during this phase, hence, it must be done before getting the statuc buffers out of the task. */
        vTaskDelete( x_Task_ಗಮ್ಯ_Delete );

        /* Free the memory buffers */
        x_ಫಲಿತಾಂಶ = xTaskGetStaticBuffers( x_Task_ಗಮ್ಯ_Delete, &pux_ರಾಶಿ_ಬಫರ್, &px_Task_ಬಫರ್ );
        configASSERT( x_ಫಲಿತಾಂಶ == pdTRUE );
        configASSERT( pux_ರಾಶಿ_ಬಫರ್ != ಖಾಲಿ );
        configASSERT( px_Task_ಬಫರ್ != ಖಾಲಿ );

        ರಾಶಿ_caps_free( pux_ರಾಶಿ_ಬಫರ್ );
        v_ಪೋರ್ಟ್_Free( px_Task_ಬಫರ್ );
    }

    ಸ್ಥಿರ ಶೂನ್ಯ prvTaskDeleteWithCapsTask( ಶೂನ್ಯ * pvParameters )
    {
        TaskHandle_t x_Task_ಗಮ್ಯ_Delete = ( TaskHandle_t ) pvParameters;

        /* Delete the WithCaps task */
        prvTaskDeleteWithCaps( x_Task_ಗಮ್ಯ_Delete );

        /* Delete the temporary clean up task */
        vTaskDelete( ಖಾಲಿ );
    }

    ಶೂನ್ಯ vTaskDeleteWithCaps( TaskHandle_t x_Task_ಗಮ್ಯ_Delete )
    {
        /* THIS FUNCTION SHOULD NOT BE CALLED FROM AN INTERRUPT CONTEXT. */
        /* TODO: Update it to use portASSERT_IF_IN_ISR() instead. (IDF-10540) */
        v_ಪೋರ್ಟ್_Assert_If_ಒಳ_ISR();

        TaskHandle_t xCurrentTaskHandle = xTaskGetCurrentTaskHandle();
        configASSERT( xCurrentTaskHandle != ಖಾಲಿ );

        ಆದರೆ( ( x_Task_ಗಮ್ಯ_Delete == ಖಾಲಿ ) || ( x_Task_ಗಮ್ಯ_Delete == xCurrentTaskHandle ) )
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
            ಆದರೆ( x_Task_Create_Pinned_ಗಮ್ಯ_Core( ( TaskFunction_t ) prvTaskDeleteWithCapsTask, "prvTaskDeleteWithCapsTask", config_ಕನಿಷ್ಠತಮ_ರಾಶಿ_ಗಾತ್ರ, xCurrentTaskHandle, ux_Task_ಆದ್ಯತೆ_Get( x_Task_ಗಮ್ಯ_Delete ), ಖಾಲಿ, x_ಪೋರ್ಟ್_Get_Core_ID() ) != pdFAIL )
            {
                /* Although the current task should get preemted immediately when prvTaskDeleteWithCapsTask is created,
                 * for safety, we suspend the current task and wait for prvTaskDeleteWithCapsTask to delete it. */
                vTaskSuspend( x_Task_ಗಮ್ಯ_Delete );

                /* Should never reach here */
                ESP_LOGE( "freertos_additions", "%s: Failed to suspend the task to be deleted", __func__ );
                abort();
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                /* Failed to create the task to delete the current task. */
                ESP_LOGE( "freertos_additions", "%s: Failed to create the task to delete the current task", __func__ );
                abort();
            }
        }

        /* Delete the WithCaps task */
        prvTaskDeleteWithCaps( x_Task_ಗಮ್ಯ_Delete );
    }

#ಆದರೆ_ಅಂತ್ಯ /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

/* ---------------------------------- Queue --------------------------------- */

#ಆದರೆ ( config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 1 )

    ಸರತಿ_Handle_t x_ಸರತಿ_Create_With_Caps( UBaseType_t ux_ಸರತಿ_ಉದ್ದಳತೆ,
                                        UBaseType_t ux_ಅಂಶ_ಗಾತ್ರ,
                                        UBaseType_t ux_ಸ್ಮರಣೆ_Caps )
    {
        ಸರತಿ_Handle_t x_ಸರತಿ;
        Static_ಸರತಿ_t * px_ಸರತಿ_ಬಫರ್;
        uint8_t * puc_ಸರತಿ_Storage_ಬಫರ್;

        /* Allocate memory for the queue using the provided memory caps */
        px_ಸರತಿ_ಬಫರ್ = ರಾಶಿ_caps_malloc( ಗಾತ್ರ( Static_ಸರತಿ_t ), ( uint32_t ) ux_ಸ್ಮರಣೆ_Caps );

        ಆದರೆ( ux_ಅಂಶ_ಗಾತ್ರ == 0 )
        {
            puc_ಸರತಿ_Storage_ಬಫರ್ = ಖಾಲಿ;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            puc_ಸರತಿ_Storage_ಬಫರ್ = ರಾಶಿ_caps_malloc( ux_ಸರತಿ_ಉದ್ದಳತೆ * ux_ಅಂಶ_ಗಾತ್ರ, ( uint32_t ) ux_ಸ್ಮರಣೆ_Caps );
        }

        ಆದರೆ( ( px_ಸರತಿ_ಬಫರ್ == ಖಾಲಿ ) || ( ( ux_ಅಂಶ_ಗಾತ್ರ > 0 ) && ( puc_ಸರತಿ_Storage_ಬಫರ್ == ಖಾಲಿ ) ) )
        {
            ಹೋಗು err;
        }

        /* Create the queue using static creation API */
        x_ಸರತಿ = x_ಸರತಿ_Create_Static( ux_ಸರತಿ_ಉದ್ದಳತೆ, ux_ಅಂಶ_ಗಾತ್ರ, puc_ಸರತಿ_Storage_ಬಫರ್, px_ಸರತಿ_ಬಫರ್ );

        ಆದರೆ( x_ಸರತಿ == ಖಾಲಿ )
        {
            ಹೋಗು err;
        }

        ಹಿಂತಿರುಗಿ x_ಸರತಿ;

err:
        ರಾಶಿ_caps_free( puc_ಸರತಿ_Storage_ಬಫರ್ );
        ರಾಶಿ_caps_free( px_ಸರತಿ_ಬಫರ್ );
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

#ಆದರೆ_ಅಂತ್ಯ /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#ಆದರೆ ( config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 1 )

    ಶೂನ್ಯ v_ಸರತಿ_Delete_With_Caps( ಸರತಿ_Handle_t x_ಸರತಿ )
    {
        /* Return value unused if asserts are disabled */
        ತಳ_Type_t __attribute__( ( ಬಳಸದ ) ) x_ಫಲಿತಾಂಶ;
        Static_ಸರತಿ_t * px_ಸರತಿ_ಬಫರ್;
        uint8_t * puc_ಸರತಿ_Storage_ಬಫರ್;

        /* Retrieve the buffers used to create the queue before deleting it */
        x_ಫಲಿತಾಂಶ = x_ಸರತಿ_Get_Static_Buffers( x_ಸರತಿ, &puc_ಸರತಿ_Storage_ಬಫರ್, &px_ಸರತಿ_ಬಫರ್ );
        configASSERT( x_ಫಲಿತಾಂಶ == pdTRUE );

        /* Delete the queue */
        v_ಸರತಿ_Delete( x_ಸರತಿ );

        /* Free the memory buffers */
        ರಾಶಿ_caps_free( px_ಸರತಿ_ಬಫರ್ );
        ರಾಶಿ_caps_free( puc_ಸರತಿ_Storage_ಬಫರ್ );
    }

#ಆದರೆ_ಅಂತ್ಯ /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

/* -------------------------------- Semaphore ------------------------------- */

#ಆದರೆ ( config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 1 )

    SemaphoreHandle_t xSemaphoreCreateGenericWithCaps( UBaseType_t ux_Max_ಎಣಿಕೆ,
                                                       UBaseType_t ux_Initial_ಎಣಿಕೆ,
                                                       ಸ್ಥಿರಾಂಕ uint8_t uc_ಸರತಿ_Type,
                                                       UBaseType_t ux_ಸ್ಮರಣೆ_Caps )
    {
        SemaphoreHandle_t xSemaphore;
        StaticSemaphore_t * px_Semaphore_ಬಫರ್;

        /* Allocate memory for the semaphore using the provided memory caps */
        px_Semaphore_ಬಫರ್ = ರಾಶಿ_caps_malloc( ಗಾತ್ರ( StaticSemaphore_t ), ( uint32_t ) ux_ಸ್ಮರಣೆ_Caps );

        ಆದರೆ( px_Semaphore_ಬಫರ್ == ಖಾಲಿ )
        {
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }

        /* Create the semaphore using static creation API */
        ಆದರೆ( uc_ಸರತಿ_Type == ಸರತಿ_ಸರತಿ_TYPE_MUTEX )
        {
            xSemaphore = xSemaphoreCreateMutexStatic( px_Semaphore_ಬಫರ್ );
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( uc_ಸರತಿ_Type == ಸರತಿ_ಸರತಿ_TYPE_COUNTING_SEMAPHORE )
        {
            xSemaphore = xSemaphoreCreateCountingStatic( ux_Max_ಎಣಿಕೆ, ux_Initial_ಎಣಿಕೆ, px_Semaphore_ಬಫರ್ );
        }
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( uc_ಸರತಿ_Type == ಸರತಿ_ಸರತಿ_TYPE_ದ್ವಿಮಾನ_SEMAPHORE )
        {
            xSemaphore = x_Semaphore_Create_ದ್ವಿಮಾನ_Static( px_Semaphore_ಬಫರ್ );
        }
        ಇಲ್ಲದಿದ್ದರೆ /* ucQueueType == queueQUEUE_TYPE_RECURSIVE_MUTEX */
        {
            xSemaphore = xSemaphoreCreateRecursiveMutexStatic( px_Semaphore_ಬಫರ್ );
        }

        ಆದರೆ( xSemaphore == ಖಾಲಿ )
        {
            ರಾಶಿ_caps_free( px_Semaphore_ಬಫರ್ );
        }

        ಹಿಂತಿರುಗಿ xSemaphore;
    }

#ಆದರೆ_ಅಂತ್ಯ /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#ಆದರೆ ( config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 1 )

    ಶೂನ್ಯ vSemaphoreDeleteWithCaps( SemaphoreHandle_t xSemaphore )
    {
        /* Return value unused if asserts are disabled */
        ತಳ_Type_t __attribute__( ( ಬಳಸದ ) ) x_ಫಲಿತಾಂಶ;
        StaticSemaphore_t * px_Semaphore_ಬಫರ್;

        /* Retrieve the buffer used to create the semaphore before deleting it
         * */
        x_ಫಲಿತಾಂಶ = x_Semaphore_Get_Static_ಬಫರ್( xSemaphore, &px_Semaphore_ಬಫರ್ );
        configASSERT( x_ಫಲಿತಾಂಶ == pdTRUE );

        /* Delete the semaphore */
        vSemaphoreDelete( xSemaphore );

        /* Free the memory buffer */
        ರಾಶಿ_caps_free( px_Semaphore_ಬಫರ್ );
    }

#ಆದರೆ_ಅಂತ್ಯ /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

/* ------------------------- Stream & Message Buffers ----------------------- */

#ಆದರೆ ( config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 1 )

    ಹರಿವು_ಬಫರ್_Handle_t x_ಹರಿವು_ಬಫರ್_Generic_Create_With_Caps( size_t x_ಬಫರ್_ಗಾತ್ರ_ಬೈಟುಗಳು,
                                                             size_t x_Trigger_Level_ಬೈಟುಗಳು,
                                                             ತಳ_Type_t x_Is_ಸಂದೇಶ_ಬಫರ್,
                                                             UBaseType_t ux_ಸ್ಮರಣೆ_Caps )
    {
        ಹರಿವು_ಬಫರ್_Handle_t x_ಹರಿವು_ಬಫರ್;
        Static_ಹರಿವು_ಬಫರ್_t * px_Static_ಹರಿವು_ಬಫರ್;
        uint8_t * puc_ಹರಿವು_ಬಫರ್_Storage_Area;

        /* Allocate memory for the stream or message buffer using the provided
         * memory caps */
        px_Static_ಹರಿವು_ಬಫರ್ = ರಾಶಿ_caps_malloc( ಗಾತ್ರ( Static_ಹರಿವು_ಬಫರ್_t ), ( uint32_t ) ux_ಸ್ಮರಣೆ_Caps );
        puc_ಹರಿವು_ಬಫರ್_Storage_Area = ರಾಶಿ_caps_malloc( x_ಬಫರ್_ಗಾತ್ರ_ಬೈಟುಗಳು, ( uint32_t ) ux_ಸ್ಮರಣೆ_Caps );

        ಆದರೆ( ( px_Static_ಹರಿವು_ಬಫರ್ == ಖಾಲಿ ) || ( puc_ಹರಿವು_ಬಫರ್_Storage_Area == ಖಾಲಿ ) )
        {
            ಹೋಗು err;
        }

        /* Create the stream or message buffer using static creation API */
        ಆದರೆ( x_Is_ಸಂದೇಶ_ಬಫರ್ == pdTRUE )
        {
            x_ಹರಿವು_ಬಫರ್ = ( ಹರಿವು_ಬಫರ್_Handle_t ) x_ಸಂದೇಶ_ಬಫರ್_Create_Static( x_ಬಫರ್_ಗಾತ್ರ_ಬೈಟುಗಳು, puc_ಹರಿವು_ಬಫರ್_Storage_Area, px_Static_ಹರಿವು_ಬಫರ್ );
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            x_ಹರಿವು_ಬಫರ್ = x_ಹರಿವು_ಬಫರ್_Create_Static( x_ಬಫರ್_ಗಾತ್ರ_ಬೈಟುಗಳು, x_Trigger_Level_ಬೈಟುಗಳು, puc_ಹರಿವು_ಬಫರ್_Storage_Area, px_Static_ಹರಿವು_ಬಫರ್ );
        }

        ಆದರೆ( x_ಹರಿವು_ಬಫರ್ == ಖಾಲಿ )
        {
            ಹೋಗು err;
        }

        ಹಿಂತಿರುಗಿ x_ಹರಿವು_ಬಫರ್;

err:
        ರಾಶಿ_caps_free( puc_ಹರಿವು_ಬಫರ್_Storage_Area );
        ರಾಶಿ_caps_free( px_Static_ಹರಿವು_ಬಫರ್ );
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

#ಆದರೆ_ಅಂತ್ಯ /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#ಆದರೆ ( config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 1 )

    ಶೂನ್ಯ v_ಹರಿವು_ಬಫರ್_Generic_Delete_With_Caps( ಹರಿವು_ಬಫರ್_Handle_t x_ಹರಿವು_ಬಫರ್,
                                             ತಳ_Type_t x_Is_ಸಂದೇಶ_ಬಫರ್ )
    {
        /* Return value unused if asserts are disabled */
        ತಳ_Type_t __attribute__( ( ಬಳಸದ ) ) x_ಫಲಿತಾಂಶ;
        Static_ಹರಿವು_ಬಫರ್_t * px_Static_ಹರಿವು_ಬಫರ್;
        uint8_t * puc_ಹರಿವು_ಬಫರ್_Storage_Area;

        /* Retrieve the buffers used to create the stream or message buffer
         * before deleting it */
        ಆದರೆ( x_Is_ಸಂದೇಶ_ಬಫರ್ == pdTRUE )
        {
            x_ಫಲಿತಾಂಶ = x_ಸಂದೇಶ_ಬಫರ್_Get_Static_Buffers( x_ಹರಿವು_ಬಫರ್, &puc_ಹರಿವು_ಬಫರ್_Storage_Area, &px_Static_ಹರಿವು_ಬಫರ್ );
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            x_ಫಲಿತಾಂಶ = x_ಹರಿವು_ಬಫರ್_Get_Static_Buffers( x_ಹರಿವು_ಬಫರ್, &puc_ಹರಿವು_ಬಫರ್_Storage_Area, &px_Static_ಹರಿವು_ಬಫರ್ );
        }

        configASSERT( x_ಫಲಿತಾಂಶ == pdTRUE );

        /* Delete the stream or message buffer */
        ಆದರೆ( x_Is_ಸಂದೇಶ_ಬಫರ್ == pdTRUE )
        {
            v_ಸಂದೇಶ_ಬಫರ್_Delete( x_ಹರಿವು_ಬಫರ್ );
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            v_ಹರಿವು_ಬಫರ್_Delete( x_ಹರಿವು_ಬಫರ್ );
        }

        /* Free the memory buffers */
        ರಾಶಿ_caps_free( px_Static_ಹರಿವು_ಬಫರ್ );
        ರಾಶಿ_caps_free( puc_ಹರಿವು_ಬಫರ್_Storage_Area );
    }

#ಆದರೆ_ಅಂತ್ಯ /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/
