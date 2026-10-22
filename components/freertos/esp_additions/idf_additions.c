/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * This file contains the implementation for some the functions in
 * idf_additions.h
 */

#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ <stdint.h>
#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ "freertos/queue.h"
#అన్తర్భావ "freertos/semphr.h"
#అన్తర్భావ "freertos/stream_buffer.h"
#అన్తర్భావ "freertos/message_buffer.h"
#అన్తర్భావ "freertos/idf_additions.h"
#అన్తర్భావ "esp_heap_caps.h"
#అన్తర్భావ "esp_log.h"
#అన్తర్భావ "freertos/portmacro.h"

/* -------------------------------------------- Creation With Memory Caps ------------------------------------------- */

/* ---------------------------------- Tasks --------------------------------- */

#ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )

    పునాది_Type_t x_Task_Create_Pinned_గమ్యం_Core_With_Caps( TaskFunction_t pv_Task_సంకేతం,
                                                స్థిరరాశి అక్షరం * స్థిరరాశి pcName,
                                                స్థిరరాశి config_దొంతర_లోతు_TYPE us_దొంతర_లోతు,
                                                శూన్య * స్థిరరాశి pvParameters,
                                                UBaseType_t ux_ప్రాధాన్యత,
                                                TaskHandle_t * స్థిరరాశి pvCreatedTask,
                                                స్థిరరాశి పునాది_Type_t xCoreID,
                                                UBaseType_t ux_జ్ఞాపకం_Caps )
    {
        TaskHandle_t xHandle;
        StaticTask_t * px_Task_బఫరు;
        దొంతర_Type_t * px_దొంతర;

        /* Allocate memory for the task's TCB. We use pvPortMalloc() here as the
         * TCB must be in internal memory. */
        px_Task_బఫరు = pv_పోర్టు_Malloc( పరిమాణం( StaticTask_t ) );

        /* Allocate memory for the task's stack using the provided memory caps
         * */
        px_దొంతర = కుప్ప_caps_malloc( ( ( size_t ) us_దొంతర_లోతు ) * పరిమాణం( దొంతర_Type_t ), ( uint32_t ) ux_జ్ఞాపకం_Caps );

        ఒకవేళ( ( px_Task_బఫరు == ఖాళీ ) || ( px_దొంతర == ఖాళీ ) )
        {
            వెళ్ళు err;
        }

        /* Create the task using static creation API*/
        xHandle = x_Task_Create_Static_Pinned_గమ్యం_Core( pv_Task_సంకేతం, pcName, us_దొంతర_లోతు, pvParameters, ux_ప్రాధాన్యత, px_దొంతర, px_Task_బఫరు, xCoreID );

        ఒకవేళ( xHandle == ఖాళీ )
        {
            /* Failed to create task */
            వెళ్ళు err;
        }
        లేకపోతే ఒకవేళ( pvCreatedTask != ఖాళీ )
        {
            /* Task created successfully. Return the task handle */
            *pvCreatedTask = xHandle;
        }

        తిరిగి pdPASS;

err:
        కుప్ప_caps_free( px_దొంతర );
        v_పోర్టు_Free( px_Task_బఫరు );
        తిరిగి pdFAIL;
    }

#అయితే_ముగింపు /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )

    స్థిర శూన్య prvTaskDeleteWithCaps( TaskHandle_t x_Task_గమ్యం_Delete )
    {
        /* Return value unused if asserts are disabled */
        పునాది_Type_t __attribute__( ( వాడని ) ) x_ఫలితం;
        StaticTask_t * px_Task_బఫరు;
        దొంతర_Type_t * pux_దొంతర_బఫరు;

        /* The task to be deleted must not be running.
         * So we suspend the task before deleting it. */
        vTaskSuspend( x_Task_గమ్యం_Delete );

        /* Wait until the task is no longer the current task on any core.
         *
         * Polling on pxCurrentTCBs[] (via xTaskGetCurrentTaskHandleForCore())
         * matches the predicate vTaskDelete() uses to choose between immediate
         * and IDLE-deferred TCB cleanup. By waiting on it here, we guarantee
         * that the vTaskDelete() call below takes the immediate path and that
         * prvDeleteTCB() runs synchronously before vTaskDelete() returns, so
         * the heap_caps_free() / vPortFree() that follow cannot race with the
         * IDLE task. */
        కొరకు( ;; )
        {
            పునాది_Type_t xStillOnCore = pdFALSE;

            కొరకు( పునాది_Type_t xCoreID = 0; xCoreID < config_సంఖ్య_యొక్క_CORES; xCoreID++ )
            {
                ఒకవేళ( xTaskGetCurrentTaskHandleForCore( xCoreID ) == x_Task_గమ్యం_Delete )
                {
                    xStillOnCore = pdTRUE;
                    ఆపు;
                }
            }

            ఒకవేళ( xStillOnCore == pdFALSE )
            {
                ఆపు;
            }

            taskYIELD();
        }

        /* We can delete the task and free the memory buffers.
         * First, we must call `vTaskDelete` so that the port task delete callback is called.
         * On targets that have coprocessors, it may be possible that the stack pointer is modified (restored)
         * during this phase, hence, it must be done before getting the statuc buffers out of the task. */
        vTaskDelete( x_Task_గమ్యం_Delete );

        /* Free the memory buffers */
        x_ఫలితం = xTaskGetStaticBuffers( x_Task_గమ్యం_Delete, &pux_దొంతర_బఫరు, &px_Task_బఫరు );
        configASSERT( x_ఫలితం == pdTRUE );
        configASSERT( pux_దొంతర_బఫరు != ఖాళీ );
        configASSERT( px_Task_బఫరు != ఖాళీ );

        కుప్ప_caps_free( pux_దొంతర_బఫరు );
        v_పోర్టు_Free( px_Task_బఫరు );
    }

    స్థిర శూన్య prvTaskDeleteWithCapsTask( శూన్య * pvParameters )
    {
        TaskHandle_t x_Task_గమ్యం_Delete = ( TaskHandle_t ) pvParameters;

        /* Delete the WithCaps task */
        prvTaskDeleteWithCaps( x_Task_గమ్యం_Delete );

        /* Delete the temporary clean up task */
        vTaskDelete( ఖాళీ );
    }

    శూన్య vTaskDeleteWithCaps( TaskHandle_t x_Task_గమ్యం_Delete )
    {
        /* THIS FUNCTION SHOULD NOT BE CALLED FROM AN INTERRUPT CONTEXT. */
        /* TODO: Update it to use portASSERT_IF_IN_ISR() instead. (IDF-10540) */
        v_పోర్టు_Assert_If_లోపల_ISR();

        TaskHandle_t xCurrentTaskHandle = xTaskGetCurrentTaskHandle();
        configASSERT( xCurrentTaskHandle != ఖాళీ );

        ఒకవేళ( ( x_Task_గమ్యం_Delete == ఖాళీ ) || ( x_Task_గమ్యం_Delete == xCurrentTaskHandle ) )
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
            ఒకవేళ( x_Task_Create_Pinned_గమ్యం_Core( ( TaskFunction_t ) prvTaskDeleteWithCapsTask, "prvTaskDeleteWithCapsTask", config_కనీస_దొంతర_కొలత, xCurrentTaskHandle, ux_Task_ప్రాధాన్యత_Get( x_Task_గమ్యం_Delete ), ఖాళీ, x_పోర్టు_Get_Core_ID() ) != pdFAIL )
            {
                /* Although the current task should get preemted immediately when prvTaskDeleteWithCapsTask is created,
                 * for safety, we suspend the current task and wait for prvTaskDeleteWithCapsTask to delete it. */
                vTaskSuspend( x_Task_గమ్యం_Delete );

                /* Should never reach here */
                ESP_LOGE( "freertos_additions", "%s: Failed to suspend the task to be deleted", __func__ );
                abort();
            }
            లేకపోతే
            {
                /* Failed to create the task to delete the current task. */
                ESP_LOGE( "freertos_additions", "%s: Failed to create the task to delete the current task", __func__ );
                abort();
            }
        }

        /* Delete the WithCaps task */
        prvTaskDeleteWithCaps( x_Task_గమ్యం_Delete );
    }

#అయితే_ముగింపు /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

/* ---------------------------------- Queue --------------------------------- */

#ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )

    వరుస_Handle_t x_వరుస_Create_With_Caps( UBaseType_t ux_వరుస_నిడివి,
                                        UBaseType_t ux_అంశం_కొలత,
                                        UBaseType_t ux_జ్ఞాపకం_Caps )
    {
        వరుస_Handle_t x_వరుస;
        Static_వరుస_t * px_వరుస_బఫరు;
        uint8_t * puc_వరుస_Storage_బఫరు;

        /* Allocate memory for the queue using the provided memory caps */
        px_వరుస_బఫరు = కుప్ప_caps_malloc( పరిమాణం( Static_వరుస_t ), ( uint32_t ) ux_జ్ఞాపకం_Caps );

        ఒకవేళ( ux_అంశం_కొలత == 0 )
        {
            puc_వరుస_Storage_బఫరు = ఖాళీ;
        }
        లేకపోతే
        {
            puc_వరుస_Storage_బఫరు = కుప్ప_caps_malloc( ux_వరుస_నిడివి * ux_అంశం_కొలత, ( uint32_t ) ux_జ్ఞాపకం_Caps );
        }

        ఒకవేళ( ( px_వరుస_బఫరు == ఖాళీ ) || ( ( ux_అంశం_కొలత > 0 ) && ( puc_వరుస_Storage_బఫరు == ఖాళీ ) ) )
        {
            వెళ్ళు err;
        }

        /* Create the queue using static creation API */
        x_వరుస = x_వరుస_Create_Static( ux_వరుస_నిడివి, ux_అంశం_కొలత, puc_వరుస_Storage_బఫరు, px_వరుస_బఫరు );

        ఒకవేళ( x_వరుస == ఖాళీ )
        {
            వెళ్ళు err;
        }

        తిరిగి x_వరుస;

err:
        కుప్ప_caps_free( puc_వరుస_Storage_బఫరు );
        కుప్ప_caps_free( px_వరుస_బఫరు );
        తిరిగి ఖాళీ;
    }

#అయితే_ముగింపు /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )

    శూన్య v_వరుస_Delete_With_Caps( వరుస_Handle_t x_వరుస )
    {
        /* Return value unused if asserts are disabled */
        పునాది_Type_t __attribute__( ( వాడని ) ) x_ఫలితం;
        Static_వరుస_t * px_వరుస_బఫరు;
        uint8_t * puc_వరుస_Storage_బఫరు;

        /* Retrieve the buffers used to create the queue before deleting it */
        x_ఫలితం = x_వరుస_Get_Static_Buffers( x_వరుస, &puc_వరుస_Storage_బఫరు, &px_వరుస_బఫరు );
        configASSERT( x_ఫలితం == pdTRUE );

        /* Delete the queue */
        v_వరుస_Delete( x_వరుస );

        /* Free the memory buffers */
        కుప్ప_caps_free( px_వరుస_బఫరు );
        కుప్ప_caps_free( puc_వరుస_Storage_బఫరు );
    }

#అయితే_ముగింపు /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

/* -------------------------------- Semaphore ------------------------------- */

#ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )

    SemaphoreHandle_t xSemaphoreCreateGenericWithCaps( UBaseType_t ux_Max_లెక్క,
                                                       UBaseType_t ux_Initial_లెక్క,
                                                       స్థిరరాశి uint8_t uc_వరుస_Type,
                                                       UBaseType_t ux_జ్ఞాపకం_Caps )
    {
        SemaphoreHandle_t xSemaphore;
        StaticSemaphore_t * px_Semaphore_బఫరు;

        /* Allocate memory for the semaphore using the provided memory caps */
        px_Semaphore_బఫరు = కుప్ప_caps_malloc( పరిమాణం( StaticSemaphore_t ), ( uint32_t ) ux_జ్ఞాపకం_Caps );

        ఒకవేళ( px_Semaphore_బఫరు == ఖాళీ )
        {
            తిరిగి ఖాళీ;
        }

        /* Create the semaphore using static creation API */
        ఒకవేళ( uc_వరుస_Type == వరుస_వరుస_TYPE_MUTEX )
        {
            xSemaphore = xSemaphoreCreateMutexStatic( px_Semaphore_బఫరు );
        }
        లేకపోతే ఒకవేళ( uc_వరుస_Type == వరుస_వరుస_TYPE_COUNTING_SEMAPHORE )
        {
            xSemaphore = xSemaphoreCreateCountingStatic( ux_Max_లెక్క, ux_Initial_లెక్క, px_Semaphore_బఫరు );
        }
        లేకపోతే ఒకవేళ( uc_వరుస_Type == వరుస_వరుస_TYPE_ద్విమాన_SEMAPHORE )
        {
            xSemaphore = x_Semaphore_Create_ద్విమాన_Static( px_Semaphore_బఫరు );
        }
        లేకపోతే /* ucQueueType == queueQUEUE_TYPE_RECURSIVE_MUTEX */
        {
            xSemaphore = xSemaphoreCreateRecursiveMutexStatic( px_Semaphore_బఫరు );
        }

        ఒకవేళ( xSemaphore == ఖాళీ )
        {
            కుప్ప_caps_free( px_Semaphore_బఫరు );
        }

        తిరిగి xSemaphore;
    }

#అయితే_ముగింపు /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )

    శూన్య vSemaphoreDeleteWithCaps( SemaphoreHandle_t xSemaphore )
    {
        /* Return value unused if asserts are disabled */
        పునాది_Type_t __attribute__( ( వాడని ) ) x_ఫలితం;
        StaticSemaphore_t * px_Semaphore_బఫరు;

        /* Retrieve the buffer used to create the semaphore before deleting it
         * */
        x_ఫలితం = x_Semaphore_Get_Static_బఫరు( xSemaphore, &px_Semaphore_బఫరు );
        configASSERT( x_ఫలితం == pdTRUE );

        /* Delete the semaphore */
        vSemaphoreDelete( xSemaphore );

        /* Free the memory buffer */
        కుప్ప_caps_free( px_Semaphore_బఫరు );
    }

#అయితే_ముగింపు /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

/* ------------------------- Stream & Message Buffers ----------------------- */

#ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )

    ప్రవాహం_బఫరు_Handle_t x_ప్రవాహం_బఫరు_Generic_Create_With_Caps( size_t x_బఫరు_కొలత_బైట్లు,
                                                             size_t x_Trigger_Level_బైట్లు,
                                                             పునాది_Type_t x_Is_సందేశం_బఫరు,
                                                             UBaseType_t ux_జ్ఞాపకం_Caps )
    {
        ప్రవాహం_బఫరు_Handle_t x_ప్రవాహం_బఫరు;
        Static_ప్రవాహం_బఫరు_t * px_Static_ప్రవాహం_బఫరు;
        uint8_t * puc_ప్రవాహం_బఫరు_Storage_Area;

        /* Allocate memory for the stream or message buffer using the provided
         * memory caps */
        px_Static_ప్రవాహం_బఫరు = కుప్ప_caps_malloc( పరిమాణం( Static_ప్రవాహం_బఫరు_t ), ( uint32_t ) ux_జ్ఞాపకం_Caps );
        puc_ప్రవాహం_బఫరు_Storage_Area = కుప్ప_caps_malloc( x_బఫరు_కొలత_బైట్లు, ( uint32_t ) ux_జ్ఞాపకం_Caps );

        ఒకవేళ( ( px_Static_ప్రవాహం_బఫరు == ఖాళీ ) || ( puc_ప్రవాహం_బఫరు_Storage_Area == ఖాళీ ) )
        {
            వెళ్ళు err;
        }

        /* Create the stream or message buffer using static creation API */
        ఒకవేళ( x_Is_సందేశం_బఫరు == pdTRUE )
        {
            x_ప్రవాహం_బఫరు = ( ప్రవాహం_బఫరు_Handle_t ) x_సందేశం_బఫరు_Create_Static( x_బఫరు_కొలత_బైట్లు, puc_ప్రవాహం_బఫరు_Storage_Area, px_Static_ప్రవాహం_బఫరు );
        }
        లేకపోతే
        {
            x_ప్రవాహం_బఫరు = x_ప్రవాహం_బఫరు_Create_Static( x_బఫరు_కొలత_బైట్లు, x_Trigger_Level_బైట్లు, puc_ప్రవాహం_బఫరు_Storage_Area, px_Static_ప్రవాహం_బఫరు );
        }

        ఒకవేళ( x_ప్రవాహం_బఫరు == ఖాళీ )
        {
            వెళ్ళు err;
        }

        తిరిగి x_ప్రవాహం_బఫరు;

err:
        కుప్ప_caps_free( puc_ప్రవాహం_బఫరు_Storage_Area );
        కుప్ప_caps_free( px_Static_ప్రవాహం_బఫరు );
        తిరిగి ఖాళీ;
    }

#అయితే_ముగింపు /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )

    శూన్య v_ప్రవాహం_బఫరు_Generic_Delete_With_Caps( ప్రవాహం_బఫరు_Handle_t x_ప్రవాహం_బఫరు,
                                             పునాది_Type_t x_Is_సందేశం_బఫరు )
    {
        /* Return value unused if asserts are disabled */
        పునాది_Type_t __attribute__( ( వాడని ) ) x_ఫలితం;
        Static_ప్రవాహం_బఫరు_t * px_Static_ప్రవాహం_బఫరు;
        uint8_t * puc_ప్రవాహం_బఫరు_Storage_Area;

        /* Retrieve the buffers used to create the stream or message buffer
         * before deleting it */
        ఒకవేళ( x_Is_సందేశం_బఫరు == pdTRUE )
        {
            x_ఫలితం = x_సందేశం_బఫరు_Get_Static_Buffers( x_ప్రవాహం_బఫరు, &puc_ప్రవాహం_బఫరు_Storage_Area, &px_Static_ప్రవాహం_బఫరు );
        }
        లేకపోతే
        {
            x_ఫలితం = x_ప్రవాహం_బఫరు_Get_Static_Buffers( x_ప్రవాహం_బఫరు, &puc_ప్రవాహం_బఫరు_Storage_Area, &px_Static_ప్రవాహం_బఫరు );
        }

        configASSERT( x_ఫలితం == pdTRUE );

        /* Delete the stream or message buffer */
        ఒకవేళ( x_Is_సందేశం_బఫరు == pdTRUE )
        {
            v_సందేశం_బఫరు_Delete( x_ప్రవాహం_బఫరు );
        }
        లేకపోతే
        {
            v_ప్రవాహం_బఫరు_Delete( x_ప్రవాహం_బఫరు );
        }

        /* Free the memory buffers */
        కుప్ప_caps_free( px_Static_ప్రవాహం_బఫరు );
        కుప్ప_caps_free( puc_ప్రవాహం_బఫరు_Storage_Area );
    }

#అయితే_ముగింపు /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/
