/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * This file contains the implementation for some the functions in
 * idf_additions.h
 */

#شامل "sdkconfig.h"
#شامل <stdint.h>
#شامل "freertos/FreeRTOS.h"
#شامل "freertos/task.h"
#شامل "freertos/queue.h"
#شامل "freertos/semphr.h"
#شامل "freertos/stream_buffer.h"
#شامل "freertos/message_buffer.h"
#شامل "freertos/idf_additions.h"
#شامل "esp_heap_caps.h"
#شامل "esp_log.h"
#شامل "freertos/portmacro.h"

/* -------------------------------------------- Creation With Memory Caps ------------------------------------------- */

/* ---------------------------------- Tasks --------------------------------- */

#اگر ( config_SUPPORT_STATIC_تخصیص == 1 )

    بنیاد_Type_t x_Task_Create_Pinned_مقصد_Core_With_Caps( TaskFunction_t pv_Task_رمز,
                                                ثابت حرف * ثابت pcName,
                                                ثابت config_ڈھیر_گہرائی_TYPE us_ڈھیر_گہرائی,
                                                خالی * ثابت pvParameters,
                                                UBaseType_t ux_ترجیح,
                                                TaskHandle_t * ثابت pvCreatedTask,
                                                ثابت بنیاد_Type_t xCoreID,
                                                UBaseType_t ux_حافظہ_Caps )
    {
        TaskHandle_t xHandle;
        StaticTask_t * px_Task_بفر;
        ڈھیر_Type_t * px_ڈھیر;

        /* Allocate memory for the task's TCB. We use pvPortMalloc() here as the
         * TCB must be in internal memory. */
        px_Task_بفر = pv_پورٹ_Malloc( حجم( StaticTask_t ) );

        /* Allocate memory for the task's stack using the provided memory caps
         * */
        px_ڈھیر = ڈھیر_caps_malloc( ( ( size_t ) us_ڈھیر_گہرائی ) * حجم( ڈھیر_Type_t ), ( uint32_t ) ux_حافظہ_Caps );

        اگر( ( px_Task_بفر == تہی ) || ( px_ڈھیر == تہی ) )
        {
            جاؤ err;
        }

        /* Create the task using static creation API*/
        xHandle = x_Task_Create_Static_Pinned_مقصد_Core( pv_Task_رمز, pcName, us_ڈھیر_گہرائی, pvParameters, ux_ترجیح, px_ڈھیر, px_Task_بفر, xCoreID );

        اگر( xHandle == تہی )
        {
            /* Failed to create task */
            جاؤ err;
        }
        ورنہ اگر( pvCreatedTask != تہی )
        {
            /* Task created successfully. Return the task handle */
            *pvCreatedTask = xHandle;
        }

        واپس pdPASS;

err:
        ڈھیر_caps_free( px_ڈھیر );
        v_پورٹ_Free( px_Task_بفر );
        واپس pdFAIL;
    }

#اگر_ختم /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#اگر ( config_SUPPORT_STATIC_تخصیص == 1 )

    ساکن خالی prvTaskDeleteWithCaps( TaskHandle_t x_Task_مقصد_Delete )
    {
        /* Return value unused if asserts are disabled */
        بنیاد_Type_t __attribute__( ( غیر_مستعمل ) ) x_نتیجہ;
        StaticTask_t * px_Task_بفر;
        ڈھیر_Type_t * pux_ڈھیر_بفر;

        /* The task to be deleted must not be running.
         * So we suspend the task before deleting it. */
        vTaskSuspend( x_Task_مقصد_Delete );

        /* Wait until the task is no longer the current task on any core.
         *
         * Polling on pxCurrentTCBs[] (via xTaskGetCurrentTaskHandleForCore())
         * matches the predicate vTaskDelete() uses to choose between immediate
         * and IDLE-deferred TCB cleanup. By waiting on it here, we guarantee
         * that the vTaskDelete() call below takes the immediate path and that
         * prvDeleteTCB() runs synchronously before vTaskDelete() returns, so
         * the heap_caps_free() / vPortFree() that follow cannot race with the
         * IDLE task. */
        برائے( ;; )
        {
            بنیاد_Type_t xStillOnCore = pdFALSE;

            برائے( بنیاد_Type_t xCoreID = 0; xCoreID < config_نمبر_کا_CORES; xCoreID++ )
            {
                اگر( xTaskGetCurrentTaskHandleForCore( xCoreID ) == x_Task_مقصد_Delete )
                {
                    xStillOnCore = pdTRUE;
                    توڑو;
                }
            }

            اگر( xStillOnCore == pdFALSE )
            {
                توڑو;
            }

            taskYIELD();
        }

        /* We can delete the task and free the memory buffers.
         * First, we must call `vTaskDelete` so that the port task delete callback is called.
         * On targets that have coprocessors, it may be possible that the stack pointer is modified (restored)
         * during this phase, hence, it must be done before getting the statuc buffers out of the task. */
        vTaskDelete( x_Task_مقصد_Delete );

        /* Free the memory buffers */
        x_نتیجہ = xTaskGetStaticBuffers( x_Task_مقصد_Delete, &pux_ڈھیر_بفر, &px_Task_بفر );
        configASSERT( x_نتیجہ == pdTRUE );
        configASSERT( pux_ڈھیر_بفر != تہی );
        configASSERT( px_Task_بفر != تہی );

        ڈھیر_caps_free( pux_ڈھیر_بفر );
        v_پورٹ_Free( px_Task_بفر );
    }

    ساکن خالی prvTaskDeleteWithCapsTask( خالی * pvParameters )
    {
        TaskHandle_t x_Task_مقصد_Delete = ( TaskHandle_t ) pvParameters;

        /* Delete the WithCaps task */
        prvTaskDeleteWithCaps( x_Task_مقصد_Delete );

        /* Delete the temporary clean up task */
        vTaskDelete( تہی );
    }

    خالی vTaskDeleteWithCaps( TaskHandle_t x_Task_مقصد_Delete )
    {
        /* THIS FUNCTION SHOULD NOT BE CALLED FROM AN INTERRUPT CONTEXT. */
        /* TODO: Update it to use portASSERT_IF_IN_ISR() instead. (IDF-10540) */
        v_پورٹ_Assert_If_اندر_ISR();

        TaskHandle_t xCurrentTaskHandle = xTaskGetCurrentTaskHandle();
        configASSERT( xCurrentTaskHandle != تہی );

        اگر( ( x_Task_مقصد_Delete == تہی ) || ( x_Task_مقصد_Delete == xCurrentTaskHandle ) )
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
            اگر( x_Task_Create_Pinned_مقصد_Core( ( TaskFunction_t ) prvTaskDeleteWithCapsTask, "prvTaskDeleteWithCapsTask", config_اقل_ڈھیر_سائز, xCurrentTaskHandle, ux_Task_ترجیح_Get( x_Task_مقصد_Delete ), تہی, x_پورٹ_Get_Core_ID() ) != pdFAIL )
            {
                /* Although the current task should get preemted immediately when prvTaskDeleteWithCapsTask is created,
                 * for safety, we suspend the current task and wait for prvTaskDeleteWithCapsTask to delete it. */
                vTaskSuspend( x_Task_مقصد_Delete );

                /* Should never reach here */
                ESP_LOGE( "freertos_additions", "%s: Failed to suspend the task to be deleted", __func__ );
                abort();
            }
            ورنہ
            {
                /* Failed to create the task to delete the current task. */
                ESP_LOGE( "freertos_additions", "%s: Failed to create the task to delete the current task", __func__ );
                abort();
            }
        }

        /* Delete the WithCaps task */
        prvTaskDeleteWithCaps( x_Task_مقصد_Delete );
    }

#اگر_ختم /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

/* ---------------------------------- Queue --------------------------------- */

#اگر ( config_SUPPORT_STATIC_تخصیص == 1 )

    قطار_Handle_t x_قطار_Create_With_Caps( UBaseType_t ux_قطار_لمبائی,
                                        UBaseType_t ux_شے_سائز,
                                        UBaseType_t ux_حافظہ_Caps )
    {
        قطار_Handle_t x_قطار;
        Static_قطار_t * px_قطار_بفر;
        uint8_t * puc_قطار_Storage_بفر;

        /* Allocate memory for the queue using the provided memory caps */
        px_قطار_بفر = ڈھیر_caps_malloc( حجم( Static_قطار_t ), ( uint32_t ) ux_حافظہ_Caps );

        اگر( ux_شے_سائز == 0 )
        {
            puc_قطار_Storage_بفر = تہی;
        }
        ورنہ
        {
            puc_قطار_Storage_بفر = ڈھیر_caps_malloc( ux_قطار_لمبائی * ux_شے_سائز, ( uint32_t ) ux_حافظہ_Caps );
        }

        اگر( ( px_قطار_بفر == تہی ) || ( ( ux_شے_سائز > 0 ) && ( puc_قطار_Storage_بفر == تہی ) ) )
        {
            جاؤ err;
        }

        /* Create the queue using static creation API */
        x_قطار = x_قطار_Create_Static( ux_قطار_لمبائی, ux_شے_سائز, puc_قطار_Storage_بفر, px_قطار_بفر );

        اگر( x_قطار == تہی )
        {
            جاؤ err;
        }

        واپس x_قطار;

err:
        ڈھیر_caps_free( puc_قطار_Storage_بفر );
        ڈھیر_caps_free( px_قطار_بفر );
        واپس تہی;
    }

#اگر_ختم /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#اگر ( config_SUPPORT_STATIC_تخصیص == 1 )

    خالی v_قطار_Delete_With_Caps( قطار_Handle_t x_قطار )
    {
        /* Return value unused if asserts are disabled */
        بنیاد_Type_t __attribute__( ( غیر_مستعمل ) ) x_نتیجہ;
        Static_قطار_t * px_قطار_بفر;
        uint8_t * puc_قطار_Storage_بفر;

        /* Retrieve the buffers used to create the queue before deleting it */
        x_نتیجہ = x_قطار_Get_Static_Buffers( x_قطار, &puc_قطار_Storage_بفر, &px_قطار_بفر );
        configASSERT( x_نتیجہ == pdTRUE );

        /* Delete the queue */
        v_قطار_Delete( x_قطار );

        /* Free the memory buffers */
        ڈھیر_caps_free( px_قطار_بفر );
        ڈھیر_caps_free( puc_قطار_Storage_بفر );
    }

#اگر_ختم /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

/* -------------------------------- Semaphore ------------------------------- */

#اگر ( config_SUPPORT_STATIC_تخصیص == 1 )

    SemaphoreHandle_t xSemaphoreCreateGenericWithCaps( UBaseType_t ux_Max_گنتی,
                                                       UBaseType_t ux_Initial_گنتی,
                                                       ثابت uint8_t uc_قطار_Type,
                                                       UBaseType_t ux_حافظہ_Caps )
    {
        SemaphoreHandle_t xSemaphore;
        StaticSemaphore_t * px_Semaphore_بفر;

        /* Allocate memory for the semaphore using the provided memory caps */
        px_Semaphore_بفر = ڈھیر_caps_malloc( حجم( StaticSemaphore_t ), ( uint32_t ) ux_حافظہ_Caps );

        اگر( px_Semaphore_بفر == تہی )
        {
            واپس تہی;
        }

        /* Create the semaphore using static creation API */
        اگر( uc_قطار_Type == قطار_قطار_TYPE_MUTEX )
        {
            xSemaphore = xSemaphoreCreateMutexStatic( px_Semaphore_بفر );
        }
        ورنہ اگر( uc_قطار_Type == قطار_قطار_TYPE_COUNTING_SEMAPHORE )
        {
            xSemaphore = xSemaphoreCreateCountingStatic( ux_Max_گنتی, ux_Initial_گنتی, px_Semaphore_بفر );
        }
        ورنہ اگر( uc_قطار_Type == قطار_قطار_TYPE_ثنائی_SEMAPHORE )
        {
            xSemaphore = x_Semaphore_Create_ثنائی_Static( px_Semaphore_بفر );
        }
        ورنہ /* ucQueueType == queueQUEUE_TYPE_RECURSIVE_MUTEX */
        {
            xSemaphore = xSemaphoreCreateRecursiveMutexStatic( px_Semaphore_بفر );
        }

        اگر( xSemaphore == تہی )
        {
            ڈھیر_caps_free( px_Semaphore_بفر );
        }

        واپس xSemaphore;
    }

#اگر_ختم /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#اگر ( config_SUPPORT_STATIC_تخصیص == 1 )

    خالی vSemaphoreDeleteWithCaps( SemaphoreHandle_t xSemaphore )
    {
        /* Return value unused if asserts are disabled */
        بنیاد_Type_t __attribute__( ( غیر_مستعمل ) ) x_نتیجہ;
        StaticSemaphore_t * px_Semaphore_بفر;

        /* Retrieve the buffer used to create the semaphore before deleting it
         * */
        x_نتیجہ = x_Semaphore_Get_Static_بفر( xSemaphore, &px_Semaphore_بفر );
        configASSERT( x_نتیجہ == pdTRUE );

        /* Delete the semaphore */
        vSemaphoreDelete( xSemaphore );

        /* Free the memory buffer */
        ڈھیر_caps_free( px_Semaphore_بفر );
    }

#اگر_ختم /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

/* ------------------------- Stream & Message Buffers ----------------------- */

#اگر ( config_SUPPORT_STATIC_تخصیص == 1 )

    دھارا_بفر_Handle_t x_دھارا_بفر_Generic_Create_With_Caps( size_t x_بفر_سائز_بائٹیں,
                                                             size_t x_Trigger_Level_بائٹیں,
                                                             بنیاد_Type_t x_Is_پیغام_بفر,
                                                             UBaseType_t ux_حافظہ_Caps )
    {
        دھارا_بفر_Handle_t x_دھارا_بفر;
        Static_دھارا_بفر_t * px_Static_دھارا_بفر;
        uint8_t * puc_دھارا_بفر_Storage_Area;

        /* Allocate memory for the stream or message buffer using the provided
         * memory caps */
        px_Static_دھارا_بفر = ڈھیر_caps_malloc( حجم( Static_دھارا_بفر_t ), ( uint32_t ) ux_حافظہ_Caps );
        puc_دھارا_بفر_Storage_Area = ڈھیر_caps_malloc( x_بفر_سائز_بائٹیں, ( uint32_t ) ux_حافظہ_Caps );

        اگر( ( px_Static_دھارا_بفر == تہی ) || ( puc_دھارا_بفر_Storage_Area == تہی ) )
        {
            جاؤ err;
        }

        /* Create the stream or message buffer using static creation API */
        اگر( x_Is_پیغام_بفر == pdTRUE )
        {
            x_دھارا_بفر = ( دھارا_بفر_Handle_t ) x_پیغام_بفر_Create_Static( x_بفر_سائز_بائٹیں, puc_دھارا_بفر_Storage_Area, px_Static_دھارا_بفر );
        }
        ورنہ
        {
            x_دھارا_بفر = x_دھارا_بفر_Create_Static( x_بفر_سائز_بائٹیں, x_Trigger_Level_بائٹیں, puc_دھارا_بفر_Storage_Area, px_Static_دھارا_بفر );
        }

        اگر( x_دھارا_بفر == تہی )
        {
            جاؤ err;
        }

        واپس x_دھارا_بفر;

err:
        ڈھیر_caps_free( puc_دھارا_بفر_Storage_Area );
        ڈھیر_caps_free( px_Static_دھارا_بفر );
        واپس تہی;
    }

#اگر_ختم /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#اگر ( config_SUPPORT_STATIC_تخصیص == 1 )

    خالی v_دھارا_بفر_Generic_Delete_With_Caps( دھارا_بفر_Handle_t x_دھارا_بفر,
                                             بنیاد_Type_t x_Is_پیغام_بفر )
    {
        /* Return value unused if asserts are disabled */
        بنیاد_Type_t __attribute__( ( غیر_مستعمل ) ) x_نتیجہ;
        Static_دھارا_بفر_t * px_Static_دھارا_بفر;
        uint8_t * puc_دھارا_بفر_Storage_Area;

        /* Retrieve the buffers used to create the stream or message buffer
         * before deleting it */
        اگر( x_Is_پیغام_بفر == pdTRUE )
        {
            x_نتیجہ = x_پیغام_بفر_Get_Static_Buffers( x_دھارا_بفر, &puc_دھارا_بفر_Storage_Area, &px_Static_دھارا_بفر );
        }
        ورنہ
        {
            x_نتیجہ = x_دھارا_بفر_Get_Static_Buffers( x_دھارا_بفر, &puc_دھارا_بفر_Storage_Area, &px_Static_دھارا_بفر );
        }

        configASSERT( x_نتیجہ == pdTRUE );

        /* Delete the stream or message buffer */
        اگر( x_Is_پیغام_بفر == pdTRUE )
        {
            v_پیغام_بفر_Delete( x_دھارا_بفر );
        }
        ورنہ
        {
            v_دھارا_بفر_Delete( x_دھارا_بفر );
        }

        /* Free the memory buffers */
        ڈھیر_caps_free( px_Static_دھارا_بفر );
        ڈھیر_caps_free( puc_دھارا_بفر_Storage_Area );
    }

#اگر_ختم /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/
