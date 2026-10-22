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

#إذا ( config_SUPPORT_STATIC_تخصیص == 1 )

    پایه_Type_t x_Task_Create_Pinned_مقصد_Core_With_Caps( TaskFunction_t pv_Task_کد,
                                                ثابت حرف * ثابت pcName,
                                                ثابت config_پشته_عمق_TYPE us_پشته_عمق,
                                                فراغ * ثابت pvParameters,
                                                UBaseType_t ux_اولویت,
                                                TaskHandle_t * ثابت pvCreatedTask,
                                                ثابت پایه_Type_t xCoreID,
                                                UBaseType_t ux_حافظه_Caps )
    {
        TaskHandle_t xHandle;
        StaticTask_t * px_Task_میانگیر;
        پشته_Type_t * px_پشته;

        /* Allocate memory for the task's TCB. We use pvPortMalloc() here as the
         * TCB must be in internal memory. */
        px_Task_میانگیر = pv_پورت_Malloc( حجم( StaticTask_t ) );

        /* Allocate memory for the task's stack using the provided memory caps
         * */
        px_پشته = انباشته_caps_malloc( ( ( size_t ) us_پشته_عمق ) * حجم( پشته_Type_t ), ( uint32_t ) ux_حافظه_Caps );

        إذا( ( px_Task_میانگیر == عدم ) || ( px_پشته == عدم ) )
        {
            اذهب err;
        }

        /* Create the task using static creation API*/
        xHandle = x_Task_Create_Static_Pinned_مقصد_Core( pv_Task_کد, pcName, us_پشته_عمق, pvParameters, ux_اولویت, px_پشته, px_Task_میانگیر, xCoreID );

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
        انباشته_caps_free( px_پشته );
        v_پورت_Free( px_Task_میانگیر );
        أرجع pdFAIL;
    }

#پایان_اگر /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#إذا ( config_SUPPORT_STATIC_تخصیص == 1 )

    ساكن فراغ prvTaskDeleteWithCaps( TaskHandle_t x_Task_مقصد_Delete )
    {
        /* Return value unused if asserts are disabled */
        پایه_Type_t __attribute__( ( استفادهنشده ) ) x_نتیجه;
        StaticTask_t * px_Task_میانگیر;
        پشته_Type_t * pux_پشته_میانگیر;

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
        لكل( ;; )
        {
            پایه_Type_t xStillOnCore = pdFALSE;

            لكل( پایه_Type_t xCoreID = 0; xCoreID < config_عدد_از_CORES; xCoreID++ )
            {
                إذا( xTaskGetCurrentTaskHandleForCore( xCoreID ) == x_Task_مقصد_Delete )
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
        vTaskDelete( x_Task_مقصد_Delete );

        /* Free the memory buffers */
        x_نتیجه = xTaskGetStaticBuffers( x_Task_مقصد_Delete, &pux_پشته_میانگیر, &px_Task_میانگیر );
        configASSERT( x_نتیجه == pdTRUE );
        configASSERT( pux_پشته_میانگیر != عدم );
        configASSERT( px_Task_میانگیر != عدم );

        انباشته_caps_free( pux_پشته_میانگیر );
        v_پورت_Free( px_Task_میانگیر );
    }

    ساكن فراغ prvTaskDeleteWithCapsTask( فراغ * pvParameters )
    {
        TaskHandle_t x_Task_مقصد_Delete = ( TaskHandle_t ) pvParameters;

        /* Delete the WithCaps task */
        prvTaskDeleteWithCaps( x_Task_مقصد_Delete );

        /* Delete the temporary clean up task */
        vTaskDelete( عدم );
    }

    فراغ vTaskDeleteWithCaps( TaskHandle_t x_Task_مقصد_Delete )
    {
        /* THIS FUNCTION SHOULD NOT BE CALLED FROM AN INTERRUPT CONTEXT. */
        /* TODO: Update it to use portASSERT_IF_IN_ISR() instead. (IDF-10540) */
        v_پورت_Assert_If_درون_ISR();

        TaskHandle_t xCurrentTaskHandle = xTaskGetCurrentTaskHandle();
        configASSERT( xCurrentTaskHandle != عدم );

        إذا( ( x_Task_مقصد_Delete == عدم ) || ( x_Task_مقصد_Delete == xCurrentTaskHandle ) )
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
            إذا( x_Task_Create_Pinned_مقصد_Core( ( TaskFunction_t ) prvTaskDeleteWithCapsTask, "prvTaskDeleteWithCapsTask", config_کمینه_پشته_اندازه, xCurrentTaskHandle, ux_Task_اولویت_Get( x_Task_مقصد_Delete ), عدم, x_پورت_Get_Core_ID() ) != pdFAIL )
            {
                /* Although the current task should get preemted immediately when prvTaskDeleteWithCapsTask is created,
                 * for safety, we suspend the current task and wait for prvTaskDeleteWithCapsTask to delete it. */
                vTaskSuspend( x_Task_مقصد_Delete );

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
        prvTaskDeleteWithCaps( x_Task_مقصد_Delete );
    }

#پایان_اگر /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

/* ---------------------------------- Queue --------------------------------- */

#إذا ( config_SUPPORT_STATIC_تخصیص == 1 )

    صف_Handle_t x_صف_Create_With_Caps( UBaseType_t ux_صف_درازی,
                                        UBaseType_t ux_قلم_اندازه,
                                        UBaseType_t ux_حافظه_Caps )
    {
        صف_Handle_t x_صف;
        Static_صف_t * px_صف_میانگیر;
        uint8_t * puc_صف_Storage_میانگیر;

        /* Allocate memory for the queue using the provided memory caps */
        px_صف_میانگیر = انباشته_caps_malloc( حجم( Static_صف_t ), ( uint32_t ) ux_حافظه_Caps );

        إذا( ux_قلم_اندازه == 0 )
        {
            puc_صف_Storage_میانگیر = عدم;
        }
        وإلا
        {
            puc_صف_Storage_میانگیر = انباشته_caps_malloc( ux_صف_درازی * ux_قلم_اندازه, ( uint32_t ) ux_حافظه_Caps );
        }

        إذا( ( px_صف_میانگیر == عدم ) || ( ( ux_قلم_اندازه > 0 ) && ( puc_صف_Storage_میانگیر == عدم ) ) )
        {
            اذهب err;
        }

        /* Create the queue using static creation API */
        x_صف = x_صف_Create_Static( ux_صف_درازی, ux_قلم_اندازه, puc_صف_Storage_میانگیر, px_صف_میانگیر );

        إذا( x_صف == عدم )
        {
            اذهب err;
        }

        أرجع x_صف;

err:
        انباشته_caps_free( puc_صف_Storage_میانگیر );
        انباشته_caps_free( px_صف_میانگیر );
        أرجع عدم;
    }

#پایان_اگر /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#إذا ( config_SUPPORT_STATIC_تخصیص == 1 )

    فراغ v_صف_Delete_With_Caps( صف_Handle_t x_صف )
    {
        /* Return value unused if asserts are disabled */
        پایه_Type_t __attribute__( ( استفادهنشده ) ) x_نتیجه;
        Static_صف_t * px_صف_میانگیر;
        uint8_t * puc_صف_Storage_میانگیر;

        /* Retrieve the buffers used to create the queue before deleting it */
        x_نتیجه = x_صف_Get_Static_Buffers( x_صف, &puc_صف_Storage_میانگیر, &px_صف_میانگیر );
        configASSERT( x_نتیجه == pdTRUE );

        /* Delete the queue */
        v_صف_Delete( x_صف );

        /* Free the memory buffers */
        انباشته_caps_free( px_صف_میانگیر );
        انباشته_caps_free( puc_صف_Storage_میانگیر );
    }

#پایان_اگر /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

/* -------------------------------- Semaphore ------------------------------- */

#إذا ( config_SUPPORT_STATIC_تخصیص == 1 )

    SemaphoreHandle_t xSemaphoreCreateGenericWithCaps( UBaseType_t ux_Max_شمار,
                                                       UBaseType_t ux_Initial_شمار,
                                                       ثابت uint8_t uc_صف_Type,
                                                       UBaseType_t ux_حافظه_Caps )
    {
        SemaphoreHandle_t xSemaphore;
        StaticSemaphore_t * px_Semaphore_میانگیر;

        /* Allocate memory for the semaphore using the provided memory caps */
        px_Semaphore_میانگیر = انباشته_caps_malloc( حجم( StaticSemaphore_t ), ( uint32_t ) ux_حافظه_Caps );

        إذا( px_Semaphore_میانگیر == عدم )
        {
            أرجع عدم;
        }

        /* Create the semaphore using static creation API */
        إذا( uc_صف_Type == صف_صف_TYPE_MUTEX )
        {
            xSemaphore = xSemaphoreCreateMutexStatic( px_Semaphore_میانگیر );
        }
        وإلا إذا( uc_صف_Type == صف_صف_TYPE_COUNTING_SEMAPHORE )
        {
            xSemaphore = xSemaphoreCreateCountingStatic( ux_Max_شمار, ux_Initial_شمار, px_Semaphore_میانگیر );
        }
        وإلا إذا( uc_صف_Type == صف_صف_TYPE_دودویی_SEMAPHORE )
        {
            xSemaphore = x_Semaphore_Create_دودویی_Static( px_Semaphore_میانگیر );
        }
        وإلا /* ucQueueType == queueQUEUE_TYPE_RECURSIVE_MUTEX */
        {
            xSemaphore = xSemaphoreCreateRecursiveMutexStatic( px_Semaphore_میانگیر );
        }

        إذا( xSemaphore == عدم )
        {
            انباشته_caps_free( px_Semaphore_میانگیر );
        }

        أرجع xSemaphore;
    }

#پایان_اگر /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#إذا ( config_SUPPORT_STATIC_تخصیص == 1 )

    فراغ vSemaphoreDeleteWithCaps( SemaphoreHandle_t xSemaphore )
    {
        /* Return value unused if asserts are disabled */
        پایه_Type_t __attribute__( ( استفادهنشده ) ) x_نتیجه;
        StaticSemaphore_t * px_Semaphore_میانگیر;

        /* Retrieve the buffer used to create the semaphore before deleting it
         * */
        x_نتیجه = x_Semaphore_Get_Static_میانگیر( xSemaphore, &px_Semaphore_میانگیر );
        configASSERT( x_نتیجه == pdTRUE );

        /* Delete the semaphore */
        vSemaphoreDelete( xSemaphore );

        /* Free the memory buffer */
        انباشته_caps_free( px_Semaphore_میانگیر );
    }

#پایان_اگر /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

/* ------------------------- Stream & Message Buffers ----------------------- */

#إذا ( config_SUPPORT_STATIC_تخصیص == 1 )

    جریان_میانگیر_Handle_t x_جریان_میانگیر_Generic_Create_With_Caps( size_t x_میانگیر_اندازه_بایتها,
                                                             size_t x_Trigger_Level_بایتها,
                                                             پایه_Type_t x_Is_پیام_میانگیر,
                                                             UBaseType_t ux_حافظه_Caps )
    {
        جریان_میانگیر_Handle_t x_جریان_میانگیر;
        Static_جریان_میانگیر_t * px_Static_جریان_میانگیر;
        uint8_t * puc_جریان_میانگیر_Storage_Area;

        /* Allocate memory for the stream or message buffer using the provided
         * memory caps */
        px_Static_جریان_میانگیر = انباشته_caps_malloc( حجم( Static_جریان_میانگیر_t ), ( uint32_t ) ux_حافظه_Caps );
        puc_جریان_میانگیر_Storage_Area = انباشته_caps_malloc( x_میانگیر_اندازه_بایتها, ( uint32_t ) ux_حافظه_Caps );

        إذا( ( px_Static_جریان_میانگیر == عدم ) || ( puc_جریان_میانگیر_Storage_Area == عدم ) )
        {
            اذهب err;
        }

        /* Create the stream or message buffer using static creation API */
        إذا( x_Is_پیام_میانگیر == pdTRUE )
        {
            x_جریان_میانگیر = ( جریان_میانگیر_Handle_t ) x_پیام_میانگیر_Create_Static( x_میانگیر_اندازه_بایتها, puc_جریان_میانگیر_Storage_Area, px_Static_جریان_میانگیر );
        }
        وإلا
        {
            x_جریان_میانگیر = x_جریان_میانگیر_Create_Static( x_میانگیر_اندازه_بایتها, x_Trigger_Level_بایتها, puc_جریان_میانگیر_Storage_Area, px_Static_جریان_میانگیر );
        }

        إذا( x_جریان_میانگیر == عدم )
        {
            اذهب err;
        }

        أرجع x_جریان_میانگیر;

err:
        انباشته_caps_free( puc_جریان_میانگیر_Storage_Area );
        انباشته_caps_free( px_Static_جریان_میانگیر );
        أرجع عدم;
    }

#پایان_اگر /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#إذا ( config_SUPPORT_STATIC_تخصیص == 1 )

    فراغ v_جریان_میانگیر_Generic_Delete_With_Caps( جریان_میانگیر_Handle_t x_جریان_میانگیر,
                                             پایه_Type_t x_Is_پیام_میانگیر )
    {
        /* Return value unused if asserts are disabled */
        پایه_Type_t __attribute__( ( استفادهنشده ) ) x_نتیجه;
        Static_جریان_میانگیر_t * px_Static_جریان_میانگیر;
        uint8_t * puc_جریان_میانگیر_Storage_Area;

        /* Retrieve the buffers used to create the stream or message buffer
         * before deleting it */
        إذا( x_Is_پیام_میانگیر == pdTRUE )
        {
            x_نتیجه = x_پیام_میانگیر_Get_Static_Buffers( x_جریان_میانگیر, &puc_جریان_میانگیر_Storage_Area, &px_Static_جریان_میانگیر );
        }
        وإلا
        {
            x_نتیجه = x_جریان_میانگیر_Get_Static_Buffers( x_جریان_میانگیر, &puc_جریان_میانگیر_Storage_Area, &px_Static_جریان_میانگیر );
        }

        configASSERT( x_نتیجه == pdTRUE );

        /* Delete the stream or message buffer */
        إذا( x_Is_پیام_میانگیر == pdTRUE )
        {
            v_پیام_میانگیر_Delete( x_جریان_میانگیر );
        }
        وإلا
        {
            v_جریان_میانگیر_Delete( x_جریان_میانگیر );
        }

        /* Free the memory buffers */
        انباشته_caps_free( px_Static_جریان_میانگیر );
        انباشته_caps_free( puc_جریان_میانگیر_Storage_Area );
    }

#پایان_اگر /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/
