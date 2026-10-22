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

#إذا ( config_SUPPORT_STATIC_ورهاست == 1 )

    بنياد_Type_t x_Task_Create_Pinned_منزل_Core_With_Caps( TaskFunction_t pv_Task_ڪوڊ,
                                                ثابت حرف * ثابت pcName,
                                                ثابت config_ڍير_اونهائي_TYPE us_ڍير_اونهائي,
                                                فراغ * ثابت pvParameters,
                                                UBaseType_t ux_ترجيح,
                                                TaskHandle_t * ثابت pvCreatedTask,
                                                ثابت بنياد_Type_t xCoreID,
                                                UBaseType_t ux_يادگيري_Caps )
    {
        TaskHandle_t xHandle;
        StaticTask_t * px_Task_بفر;
        ڍير_Type_t * px_ڍير;

        /* Allocate memory for the task's TCB. We use pvPortMalloc() here as the
         * TCB must be in internal memory. */
        px_Task_بفر = pv_پورٽ_Malloc( حجم( StaticTask_t ) );

        /* Allocate memory for the task's stack using the provided memory caps
         * */
        px_ڍير = ڍير_caps_malloc( ( ( size_t ) us_ڍير_اونهائي ) * حجم( ڍير_Type_t ), ( uint32_t ) ux_يادگيري_Caps );

        إذا( ( px_Task_بفر == عدم ) || ( px_ڍير == عدم ) )
        {
            اذهب err;
        }

        /* Create the task using static creation API*/
        xHandle = x_Task_Create_Static_Pinned_منزل_Core( pv_Task_ڪوڊ, pcName, us_ڍير_اونهائي, pvParameters, ux_ترجيح, px_ڍير, px_Task_بفر, xCoreID );

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
        ڍير_caps_free( px_ڍير );
        v_پورٽ_Free( px_Task_بفر );
        أرجع pdFAIL;
    }

#جيڪڏهن_پورو /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#إذا ( config_SUPPORT_STATIC_ورهاست == 1 )

    ساكن فراغ prvTaskDeleteWithCaps( TaskHandle_t x_Task_منزل_Delete )
    {
        /* Return value unused if asserts are disabled */
        بنياد_Type_t __attribute__( ( اڻ_استعمال_ٿيل ) ) x_نتيجو;
        StaticTask_t * px_Task_بفر;
        ڍير_Type_t * pux_ڍير_بفر;

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
            بنياد_Type_t xStillOnCore = pdFALSE;

            لكل( بنياد_Type_t xCoreID = 0; xCoreID < config_عدد_جو_CORES; xCoreID++ )
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
        x_نتيجو = xTaskGetStaticBuffers( x_Task_منزل_Delete, &pux_ڍير_بفر, &px_Task_بفر );
        configASSERT( x_نتيجو == pdTRUE );
        configASSERT( pux_ڍير_بفر != عدم );
        configASSERT( px_Task_بفر != عدم );

        ڍير_caps_free( pux_ڍير_بفر );
        v_پورٽ_Free( px_Task_بفر );
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
        v_پورٽ_Assert_If_اندريون_ISR();

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
            إذا( x_Task_Create_Pinned_منزل_Core( ( TaskFunction_t ) prvTaskDeleteWithCapsTask, "prvTaskDeleteWithCapsTask", config_گهٽتر_ڍير_سائيز, xCurrentTaskHandle, ux_Task_ترجيح_Get( x_Task_منزل_Delete ), عدم, x_پورٽ_Get_Core_ID() ) != pdFAIL )
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

#جيڪڏهن_پورو /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

/* ---------------------------------- Queue --------------------------------- */

#إذا ( config_SUPPORT_STATIC_ورهاست == 1 )

    قطار_Handle_t x_قطار_Create_With_Caps( UBaseType_t ux_قطار_ڊيگهائي,
                                        UBaseType_t ux_شي_سائيز,
                                        UBaseType_t ux_يادگيري_Caps )
    {
        قطار_Handle_t x_قطار;
        Static_قطار_t * px_قطار_بفر;
        uint8_t * puc_قطار_Storage_بفر;

        /* Allocate memory for the queue using the provided memory caps */
        px_قطار_بفر = ڍير_caps_malloc( حجم( Static_قطار_t ), ( uint32_t ) ux_يادگيري_Caps );

        إذا( ux_شي_سائيز == 0 )
        {
            puc_قطار_Storage_بفر = عدم;
        }
        وإلا
        {
            puc_قطار_Storage_بفر = ڍير_caps_malloc( ux_قطار_ڊيگهائي * ux_شي_سائيز, ( uint32_t ) ux_يادگيري_Caps );
        }

        إذا( ( px_قطار_بفر == عدم ) || ( ( ux_شي_سائيز > 0 ) && ( puc_قطار_Storage_بفر == عدم ) ) )
        {
            اذهب err;
        }

        /* Create the queue using static creation API */
        x_قطار = x_قطار_Create_Static( ux_قطار_ڊيگهائي, ux_شي_سائيز, puc_قطار_Storage_بفر, px_قطار_بفر );

        إذا( x_قطار == عدم )
        {
            اذهب err;
        }

        أرجع x_قطار;

err:
        ڍير_caps_free( puc_قطار_Storage_بفر );
        ڍير_caps_free( px_قطار_بفر );
        أرجع عدم;
    }

#جيڪڏهن_پورو /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#إذا ( config_SUPPORT_STATIC_ورهاست == 1 )

    فراغ v_قطار_Delete_With_Caps( قطار_Handle_t x_قطار )
    {
        /* Return value unused if asserts are disabled */
        بنياد_Type_t __attribute__( ( اڻ_استعمال_ٿيل ) ) x_نتيجو;
        Static_قطار_t * px_قطار_بفر;
        uint8_t * puc_قطار_Storage_بفر;

        /* Retrieve the buffers used to create the queue before deleting it */
        x_نتيجو = x_قطار_Get_Static_Buffers( x_قطار, &puc_قطار_Storage_بفر, &px_قطار_بفر );
        configASSERT( x_نتيجو == pdTRUE );

        /* Delete the queue */
        v_قطار_Delete( x_قطار );

        /* Free the memory buffers */
        ڍير_caps_free( px_قطار_بفر );
        ڍير_caps_free( puc_قطار_Storage_بفر );
    }

#جيڪڏهن_پورو /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

/* -------------------------------- Semaphore ------------------------------- */

#إذا ( config_SUPPORT_STATIC_ورهاست == 1 )

    SemaphoreHandle_t xSemaphoreCreateGenericWithCaps( UBaseType_t ux_Max_ڳڻپ,
                                                       UBaseType_t ux_Initial_ڳڻپ,
                                                       ثابت uint8_t uc_قطار_Type,
                                                       UBaseType_t ux_يادگيري_Caps )
    {
        SemaphoreHandle_t xSemaphore;
        StaticSemaphore_t * px_Semaphore_بفر;

        /* Allocate memory for the semaphore using the provided memory caps */
        px_Semaphore_بفر = ڍير_caps_malloc( حجم( StaticSemaphore_t ), ( uint32_t ) ux_يادگيري_Caps );

        إذا( px_Semaphore_بفر == عدم )
        {
            أرجع عدم;
        }

        /* Create the semaphore using static creation API */
        إذا( uc_قطار_Type == قطار_قطار_TYPE_MUTEX )
        {
            xSemaphore = xSemaphoreCreateMutexStatic( px_Semaphore_بفر );
        }
        وإلا إذا( uc_قطار_Type == قطار_قطار_TYPE_COUNTING_SEMAPHORE )
        {
            xSemaphore = xSemaphoreCreateCountingStatic( ux_Max_ڳڻپ, ux_Initial_ڳڻپ, px_Semaphore_بفر );
        }
        وإلا إذا( uc_قطار_Type == قطار_قطار_TYPE_ٻٽو_SEMAPHORE )
        {
            xSemaphore = x_Semaphore_Create_ٻٽو_Static( px_Semaphore_بفر );
        }
        وإلا /* ucQueueType == queueQUEUE_TYPE_RECURSIVE_MUTEX */
        {
            xSemaphore = xSemaphoreCreateRecursiveMutexStatic( px_Semaphore_بفر );
        }

        إذا( xSemaphore == عدم )
        {
            ڍير_caps_free( px_Semaphore_بفر );
        }

        أرجع xSemaphore;
    }

#جيڪڏهن_پورو /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#إذا ( config_SUPPORT_STATIC_ورهاست == 1 )

    فراغ vSemaphoreDeleteWithCaps( SemaphoreHandle_t xSemaphore )
    {
        /* Return value unused if asserts are disabled */
        بنياد_Type_t __attribute__( ( اڻ_استعمال_ٿيل ) ) x_نتيجو;
        StaticSemaphore_t * px_Semaphore_بفر;

        /* Retrieve the buffer used to create the semaphore before deleting it
         * */
        x_نتيجو = x_Semaphore_Get_Static_بفر( xSemaphore, &px_Semaphore_بفر );
        configASSERT( x_نتيجو == pdTRUE );

        /* Delete the semaphore */
        vSemaphoreDelete( xSemaphore );

        /* Free the memory buffer */
        ڍير_caps_free( px_Semaphore_بفر );
    }

#جيڪڏهن_پورو /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

/* ------------------------- Stream & Message Buffers ----------------------- */

#إذا ( config_SUPPORT_STATIC_ورهاست == 1 )

    وهڪرو_بفر_Handle_t x_وهڪرو_بفر_Generic_Create_With_Caps( size_t x_بفر_سائيز_بائيٽون,
                                                             size_t x_Trigger_Level_بائيٽون,
                                                             بنياد_Type_t x_Is_پيغام_بفر,
                                                             UBaseType_t ux_يادگيري_Caps )
    {
        وهڪرو_بفر_Handle_t x_وهڪرو_بفر;
        Static_وهڪرو_بفر_t * px_Static_وهڪرو_بفر;
        uint8_t * puc_وهڪرو_بفر_Storage_Area;

        /* Allocate memory for the stream or message buffer using the provided
         * memory caps */
        px_Static_وهڪرو_بفر = ڍير_caps_malloc( حجم( Static_وهڪرو_بفر_t ), ( uint32_t ) ux_يادگيري_Caps );
        puc_وهڪرو_بفر_Storage_Area = ڍير_caps_malloc( x_بفر_سائيز_بائيٽون, ( uint32_t ) ux_يادگيري_Caps );

        إذا( ( px_Static_وهڪرو_بفر == عدم ) || ( puc_وهڪرو_بفر_Storage_Area == عدم ) )
        {
            اذهب err;
        }

        /* Create the stream or message buffer using static creation API */
        إذا( x_Is_پيغام_بفر == pdTRUE )
        {
            x_وهڪرو_بفر = ( وهڪرو_بفر_Handle_t ) x_پيغام_بفر_Create_Static( x_بفر_سائيز_بائيٽون, puc_وهڪرو_بفر_Storage_Area, px_Static_وهڪرو_بفر );
        }
        وإلا
        {
            x_وهڪرو_بفر = x_وهڪرو_بفر_Create_Static( x_بفر_سائيز_بائيٽون, x_Trigger_Level_بائيٽون, puc_وهڪرو_بفر_Storage_Area, px_Static_وهڪرو_بفر );
        }

        إذا( x_وهڪرو_بفر == عدم )
        {
            اذهب err;
        }

        أرجع x_وهڪرو_بفر;

err:
        ڍير_caps_free( puc_وهڪرو_بفر_Storage_Area );
        ڍير_caps_free( px_Static_وهڪرو_بفر );
        أرجع عدم;
    }

#جيڪڏهن_پورو /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#إذا ( config_SUPPORT_STATIC_ورهاست == 1 )

    فراغ v_وهڪرو_بفر_Generic_Delete_With_Caps( وهڪرو_بفر_Handle_t x_وهڪرو_بفر,
                                             بنياد_Type_t x_Is_پيغام_بفر )
    {
        /* Return value unused if asserts are disabled */
        بنياد_Type_t __attribute__( ( اڻ_استعمال_ٿيل ) ) x_نتيجو;
        Static_وهڪرو_بفر_t * px_Static_وهڪرو_بفر;
        uint8_t * puc_وهڪرو_بفر_Storage_Area;

        /* Retrieve the buffers used to create the stream or message buffer
         * before deleting it */
        إذا( x_Is_پيغام_بفر == pdTRUE )
        {
            x_نتيجو = x_پيغام_بفر_Get_Static_Buffers( x_وهڪرو_بفر, &puc_وهڪرو_بفر_Storage_Area, &px_Static_وهڪرو_بفر );
        }
        وإلا
        {
            x_نتيجو = x_وهڪرو_بفر_Get_Static_Buffers( x_وهڪرو_بفر, &puc_وهڪرو_بفر_Storage_Area, &px_Static_وهڪرو_بفر );
        }

        configASSERT( x_نتيجو == pdTRUE );

        /* Delete the stream or message buffer */
        إذا( x_Is_پيغام_بفر == pdTRUE )
        {
            v_پيغام_بفر_Delete( x_وهڪرو_بفر );
        }
        وإلا
        {
            v_وهڪرو_بفر_Delete( x_وهڪرو_بفر );
        }

        /* Free the memory buffers */
        ڍير_caps_free( px_Static_وهڪرو_بفر );
        ڍير_caps_free( puc_وهڪرو_بفر_Storage_Area );
    }

#جيڪڏهن_پورو /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/
