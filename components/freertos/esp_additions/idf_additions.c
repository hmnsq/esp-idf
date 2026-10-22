/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * This file contains the implementation for some the functions in
 * idf_additions.h
 */

#ضمن "sdkconfig.h"
#ضمن <stdint.h>
#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن "freertos/queue.h"
#ضمن "freertos/semphr.h"
#ضمن "freertos/stream_buffer.h"
#ضمن "freertos/message_buffer.h"
#ضمن "freertos/idf_additions.h"
#ضمن "esp_heap_caps.h"
#ضمن "esp_log.h"
#ضمن "freertos/portmacro.h"

/* -------------------------------------------- Creation With Memory Caps ------------------------------------------- */

/* ---------------------------------- Tasks --------------------------------- */

#إذا ( config_SUPPORT_STATIC_ویش == 1 )

    بنسټ_Type_t x_Task_Create_Pinned_منزل_Core_With_Caps( TaskFunction_t pv_Task_کوډ,
                                                ثابت حرف * ثابت pcName,
                                                ثابت config_ډېری_ژوروالی_TYPE us_ډېری_ژوروالی,
                                                فراغ * ثابت pvParameters,
                                                UBaseType_t ux_لومړیتوب,
                                                TaskHandle_t * ثابت pvCreatedTask,
                                                ثابت بنسټ_Type_t xCoreID,
                                                UBaseType_t ux_حافظه_Caps )
    {
        TaskHandle_t xHandle;
        StaticTask_t * px_Task_بفر;
        ډېری_Type_t * px_ډېری;

        /* Allocate memory for the task's TCB. We use pvPortMalloc() here as the
         * TCB must be in internal memory. */
        px_Task_بفر = pv_پورټ_Malloc( حجم( StaticTask_t ) );

        /* Allocate memory for the task's stack using the provided memory caps
         * */
        px_ډېری = ډېرۍ_caps_malloc( ( ( size_t ) us_ډېری_ژوروالی ) * حجم( ډېری_Type_t ), ( uint32_t ) ux_حافظه_Caps );

        إذا( ( px_Task_بفر == عدم ) || ( px_ډېری == عدم ) )
        {
            اذهب err;
        }

        /* Create the task using static creation API*/
        xHandle = x_Task_Create_Static_Pinned_منزل_Core( pv_Task_کوډ, pcName, us_ډېری_ژوروالی, pvParameters, ux_لومړیتوب, px_ډېری, px_Task_بفر, xCoreID );

        إذا( xHandle == عدم )
        {
            /* Failed to create task */
            اذهب err;
        }
        وإلا إذا( pvCreatedTask != عدم )
        {
            /* Task created successfully. Return the task handle */
            *pvCreatedTask = xHandle;
        }

        أرجع pdPASS;

err:
        ډېرۍ_caps_free( px_ډېری );
        v_پورټ_Free( px_Task_بفر );
        أرجع pdFAIL;
    }

#د_که_پای /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#إذا ( config_SUPPORT_STATIC_ویش == 1 )

    ساكن فراغ prvTaskDeleteWithCaps( TaskHandle_t x_Task_منزل_Delete )
    {
        /* Return value unused if asserts are disabled */
        بنسټ_Type_t __attribute__( ( ناکارول_شوی ) ) x_پایله;
        StaticTask_t * px_Task_بفر;
        ډېری_Type_t * pux_ډېری_بفر;

        /* The task to be deleted must not be running.
         * So we suspend the task before deleting it. */
        vTaskSuspend( x_Task_منزل_Delete );

        /* Wait until the task is no longer the current task on any core.
         *
         * Polling on pxCurrentTCBs[] (via xTaskGetCurrentTaskHandleForCore())
         * matches the predicate vTaskDelete() uses to choose between immediate
         * and IDLE-deferred TCB cleanup. By waiting on it here, we guarantee
         * that the vTaskDelete() call below takes the immediate path and that
         * prvDeleteTCB() runs synchronously before vTaskDelete() returns, so
         * the heap_caps_free() / vPortFree() that follow cannot race with the
         * IDLE task. */
        لكل( ;; )
        {
            بنسټ_Type_t xStillOnCore = pdFALSE;

            لكل( بنسټ_Type_t xCoreID = 0; xCoreID < config_شمېره_د_CORES; xCoreID++ )
            {
                إذا( xTaskGetCurrentTaskHandleForCore( xCoreID ) == x_Task_منزل_Delete )
                {
                    xStillOnCore = pdTRUE;
                    اكسر;
                }
            }

            إذا( xStillOnCore == pdFALSE )
            {
                اكسر;
            }

            taskYIELD();
        }

        /* We can delete the task and free the memory buffers.
         * First, we must call `vTaskDelete` so that the port task delete callback is called.
         * On targets that have coprocessors, it may be possible that the stack pointer is modified (restored)
         * during this phase, hence, it must be done before getting the statuc buffers out of the task. */
        vTaskDelete( x_Task_منزل_Delete );

        /* Free the memory buffers */
        x_پایله = xTaskGetStaticBuffers( x_Task_منزل_Delete, &pux_ډېری_بفر, &px_Task_بفر );
        configASSERT( x_پایله == pdTRUE );
        configASSERT( pux_ډېری_بفر != عدم );
        configASSERT( px_Task_بفر != عدم );

        ډېرۍ_caps_free( pux_ډېری_بفر );
        v_پورټ_Free( px_Task_بفر );
    }

    ساكن فراغ prvTaskDeleteWithCapsTask( فراغ * pvParameters )
    {
        TaskHandle_t x_Task_منزل_Delete = ( TaskHandle_t ) pvParameters;

        /* Delete the WithCaps task */
        prvTaskDeleteWithCaps( x_Task_منزل_Delete );

        /* Delete the temporary clean up task */
        vTaskDelete( عدم );
    }

    فراغ vTaskDeleteWithCaps( TaskHandle_t x_Task_منزل_Delete )
    {
        /* THIS FUNCTION SHOULD NOT BE CALLED FROM AN INTERRUPT CONTEXT. */
        /* TODO: Update it to use portASSERT_IF_IN_ISR() instead. (IDF-10540) */
        v_پورټ_Assert_If_دننه_ISR();

        TaskHandle_t xCurrentTaskHandle = xTaskGetCurrentTaskHandle();
        configASSERT( xCurrentTaskHandle != عدم );

        إذا( ( x_Task_منزل_Delete == عدم ) || ( x_Task_منزل_Delete == xCurrentTaskHandle ) )
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
            إذا( x_Task_Create_Pinned_منزل_Core( ( TaskFunction_t ) prvTaskDeleteWithCapsTask, "prvTaskDeleteWithCapsTask", config_اصغري_ډېری_اندازه, xCurrentTaskHandle, ux_Task_لومړیتوب_Get( x_Task_منزل_Delete ), عدم, x_پورټ_Get_Core_ID() ) != pdFAIL )
            {
                /* Although the current task should get preemted immediately when prvTaskDeleteWithCapsTask is created,
                 * for safety, we suspend the current task and wait for prvTaskDeleteWithCapsTask to delete it. */
                vTaskSuspend( x_Task_منزل_Delete );

                /* Should never reach here */
                ESP_LOGE( "freertos_additions", "%s: Failed to suspend the task to be deleted", __func__ );
                abort();
            }
            وإلا
            {
                /* Failed to create the task to delete the current task. */
                ESP_LOGE( "freertos_additions", "%s: Failed to create the task to delete the current task", __func__ );
                abort();
            }
        }

        /* Delete the WithCaps task */
        prvTaskDeleteWithCaps( x_Task_منزل_Delete );
    }

#د_که_پای /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

/* ---------------------------------- Queue --------------------------------- */

#إذا ( config_SUPPORT_STATIC_ویش == 1 )

    کتار_Handle_t x_کتار_Create_With_Caps( UBaseType_t ux_کتار_اوږدتیا,
                                        UBaseType_t ux_توکی_اندازه,
                                        UBaseType_t ux_حافظه_Caps )
    {
        کتار_Handle_t x_کتار;
        Static_کتار_t * px_کتار_بفر;
        uint8_t * puc_کتار_Storage_بفر;

        /* Allocate memory for the queue using the provided memory caps */
        px_کتار_بفر = ډېرۍ_caps_malloc( حجم( Static_کتار_t ), ( uint32_t ) ux_حافظه_Caps );

        إذا( ux_توکی_اندازه == 0 )
        {
            puc_کتار_Storage_بفر = عدم;
        }
        وإلا
        {
            puc_کتار_Storage_بفر = ډېرۍ_caps_malloc( ux_کتار_اوږدتیا * ux_توکی_اندازه, ( uint32_t ) ux_حافظه_Caps );
        }

        إذا( ( px_کتار_بفر == عدم ) || ( ( ux_توکی_اندازه > 0 ) && ( puc_کتار_Storage_بفر == عدم ) ) )
        {
            اذهب err;
        }

        /* Create the queue using static creation API */
        x_کتار = x_کتار_Create_Static( ux_کتار_اوږدتیا, ux_توکی_اندازه, puc_کتار_Storage_بفر, px_کتار_بفر );

        إذا( x_کتار == عدم )
        {
            اذهب err;
        }

        أرجع x_کتار;

err:
        ډېرۍ_caps_free( puc_کتار_Storage_بفر );
        ډېرۍ_caps_free( px_کتار_بفر );
        أرجع عدم;
    }

#د_که_پای /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#إذا ( config_SUPPORT_STATIC_ویش == 1 )

    فراغ v_کتار_Delete_With_Caps( کتار_Handle_t x_کتار )
    {
        /* Return value unused if asserts are disabled */
        بنسټ_Type_t __attribute__( ( ناکارول_شوی ) ) x_پایله;
        Static_کتار_t * px_کتار_بفر;
        uint8_t * puc_کتار_Storage_بفر;

        /* Retrieve the buffers used to create the queue before deleting it */
        x_پایله = x_کتار_Get_Static_Buffers( x_کتار, &puc_کتار_Storage_بفر, &px_کتار_بفر );
        configASSERT( x_پایله == pdTRUE );

        /* Delete the queue */
        v_کتار_Delete( x_کتار );

        /* Free the memory buffers */
        ډېرۍ_caps_free( px_کتار_بفر );
        ډېرۍ_caps_free( puc_کتار_Storage_بفر );
    }

#د_که_پای /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

/* -------------------------------- Semaphore ------------------------------- */

#إذا ( config_SUPPORT_STATIC_ویش == 1 )

    SemaphoreHandle_t xSemaphoreCreateGenericWithCaps( UBaseType_t ux_Max_شمېر,
                                                       UBaseType_t ux_Initial_شمېر,
                                                       ثابت uint8_t uc_کتار_Type,
                                                       UBaseType_t ux_حافظه_Caps )
    {
        SemaphoreHandle_t xSemaphore;
        StaticSemaphore_t * px_Semaphore_بفر;

        /* Allocate memory for the semaphore using the provided memory caps */
        px_Semaphore_بفر = ډېرۍ_caps_malloc( حجم( StaticSemaphore_t ), ( uint32_t ) ux_حافظه_Caps );

        إذا( px_Semaphore_بفر == عدم )
        {
            أرجع عدم;
        }

        /* Create the semaphore using static creation API */
        إذا( uc_کتار_Type == کتار_کتار_TYPE_MUTEX )
        {
            xSemaphore = xSemaphoreCreateMutexStatic( px_Semaphore_بفر );
        }
        وإلا إذا( uc_کتار_Type == کتار_کتار_TYPE_COUNTING_SEMAPHORE )
        {
            xSemaphore = xSemaphoreCreateCountingStatic( ux_Max_شمېر, ux_Initial_شمېر, px_Semaphore_بفر );
        }
        وإلا إذا( uc_کتار_Type == کتار_کتار_TYPE_دوګونی_SEMAPHORE )
        {
            xSemaphore = x_Semaphore_Create_دوګونی_Static( px_Semaphore_بفر );
        }
        وإلا /* ucQueueType == queueQUEUE_TYPE_RECURSIVE_MUTEX */
        {
            xSemaphore = xSemaphoreCreateRecursiveMutexStatic( px_Semaphore_بفر );
        }

        إذا( xSemaphore == عدم )
        {
            ډېرۍ_caps_free( px_Semaphore_بفر );
        }

        أرجع xSemaphore;
    }

#د_که_پای /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#إذا ( config_SUPPORT_STATIC_ویش == 1 )

    فراغ vSemaphoreDeleteWithCaps( SemaphoreHandle_t xSemaphore )
    {
        /* Return value unused if asserts are disabled */
        بنسټ_Type_t __attribute__( ( ناکارول_شوی ) ) x_پایله;
        StaticSemaphore_t * px_Semaphore_بفر;

        /* Retrieve the buffer used to create the semaphore before deleting it
         * */
        x_پایله = x_Semaphore_Get_Static_بفر( xSemaphore, &px_Semaphore_بفر );
        configASSERT( x_پایله == pdTRUE );

        /* Delete the semaphore */
        vSemaphoreDelete( xSemaphore );

        /* Free the memory buffer */
        ډېرۍ_caps_free( px_Semaphore_بفر );
    }

#د_که_پای /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

/* ------------------------- Stream & Message Buffers ----------------------- */

#إذا ( config_SUPPORT_STATIC_ویش == 1 )

    بهیر_بفر_Handle_t x_بهیر_بفر_Generic_Create_With_Caps( size_t x_بفر_اندازه_بایټونه,
                                                             size_t x_Trigger_Level_بایټونه,
                                                             بنسټ_Type_t x_Is_پیغام_بفر,
                                                             UBaseType_t ux_حافظه_Caps )
    {
        بهیر_بفر_Handle_t x_بهیر_بفر;
        Static_بهیر_بفر_t * px_Static_بهیر_بفر;
        uint8_t * puc_بهیر_بفر_Storage_Area;

        /* Allocate memory for the stream or message buffer using the provided
         * memory caps */
        px_Static_بهیر_بفر = ډېرۍ_caps_malloc( حجم( Static_بهیر_بفر_t ), ( uint32_t ) ux_حافظه_Caps );
        puc_بهیر_بفر_Storage_Area = ډېرۍ_caps_malloc( x_بفر_اندازه_بایټونه, ( uint32_t ) ux_حافظه_Caps );

        إذا( ( px_Static_بهیر_بفر == عدم ) || ( puc_بهیر_بفر_Storage_Area == عدم ) )
        {
            اذهب err;
        }

        /* Create the stream or message buffer using static creation API */
        إذا( x_Is_پیغام_بفر == pdTRUE )
        {
            x_بهیر_بفر = ( بهیر_بفر_Handle_t ) x_پیغام_بفر_Create_Static( x_بفر_اندازه_بایټونه, puc_بهیر_بفر_Storage_Area, px_Static_بهیر_بفر );
        }
        وإلا
        {
            x_بهیر_بفر = x_بهیر_بفر_Create_Static( x_بفر_اندازه_بایټونه, x_Trigger_Level_بایټونه, puc_بهیر_بفر_Storage_Area, px_Static_بهیر_بفر );
        }

        إذا( x_بهیر_بفر == عدم )
        {
            اذهب err;
        }

        أرجع x_بهیر_بفر;

err:
        ډېرۍ_caps_free( puc_بهیر_بفر_Storage_Area );
        ډېرۍ_caps_free( px_Static_بهیر_بفر );
        أرجع عدم;
    }

#د_که_پای /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#إذا ( config_SUPPORT_STATIC_ویش == 1 )

    فراغ v_بهیر_بفر_Generic_Delete_With_Caps( بهیر_بفر_Handle_t x_بهیر_بفر,
                                             بنسټ_Type_t x_Is_پیغام_بفر )
    {
        /* Return value unused if asserts are disabled */
        بنسټ_Type_t __attribute__( ( ناکارول_شوی ) ) x_پایله;
        Static_بهیر_بفر_t * px_Static_بهیر_بفر;
        uint8_t * puc_بهیر_بفر_Storage_Area;

        /* Retrieve the buffers used to create the stream or message buffer
         * before deleting it */
        إذا( x_Is_پیغام_بفر == pdTRUE )
        {
            x_پایله = x_پیغام_بفر_Get_Static_Buffers( x_بهیر_بفر, &puc_بهیر_بفر_Storage_Area, &px_Static_بهیر_بفر );
        }
        وإلا
        {
            x_پایله = x_بهیر_بفر_Get_Static_Buffers( x_بهیر_بفر, &puc_بهیر_بفر_Storage_Area, &px_Static_بهیر_بفر );
        }

        configASSERT( x_پایله == pdTRUE );

        /* Delete the stream or message buffer */
        إذا( x_Is_پیغام_بفر == pdTRUE )
        {
            v_پیغام_بفر_Delete( x_بهیر_بفر );
        }
        وإلا
        {
            v_بهیر_بفر_Delete( x_بهیر_بفر );
        }

        /* Free the memory buffers */
        ډېرۍ_caps_free( px_Static_بهیر_بفر );
        ډېرۍ_caps_free( puc_بهیر_بفر_Storage_Area );
    }

#د_که_پای /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/
