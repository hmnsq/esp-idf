/*
 * FreeRTOS Kernel V11.1.0
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * SPDX-FileCopyrightText: 2021 Amazon.com, Inc. or its affiliates
 *
 * SPDX-License-Identifier: MIT
 *
 * SPDX-FileContributor: 2023-2024 Espressif Systems (Shanghai) CO LTD
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

#यदि_अपरिभाषितम् MPU_WRAPPERS_H
#परिभाषय MPU_WRAPPERS_H

/* This file redefines API functions to be called through a wrapper macro, but
 * only for ports that are using the MPU. */
#यदि ( पत्तनम्_USING_MPU_WRAPPERS == 1 )

/* MPU_WRAPPERS_INCLUDED_FROM_API_FILE will be defined when this file is
 * included from queue.c or task.c to prevent it from having an effect within
 * those files. */
    #यदि_अपरिभाषितम् MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/*
 * Map standard (non MPU) API functions to equivalents that start
 * "MPU_".  This will cause the application code to call the MPU_
 * version, which wraps the non-MPU version with privilege promoting
 * then demoting code, so the kernel code always runs will full
 * privileges.
 */

/* Map standard task.h API functions to the MPU equivalents. */
        #परिभाषय vTaskDelay                            MPU_vTaskDelay
        #परिभाषय xTaskDelayUntil                       MPU_xTaskDelayUntil
        #परिभाषय xTaskAbortDelay                       MPU_xTaskAbortDelay
        #परिभाषय ux_Task_प्राथमिकता_Get                     MPU_ux_Task_प्राथमिकता_Get
        #परिभाषय eTaskGetState                         MPU_eTaskGetState
        #परिभाषय vTaskGetInfo                          MPU_vTaskGetInfo
        #परिभाषय vTaskSuspend                          MPU_vTaskSuspend
        #परिभाषय vTaskResume                           MPU_vTaskResume
        #परिभाषय x_Task_Get_Tick_गणनम्                     MPU_x_Task_Get_Tick_गणनम्
        #परिभाषय ux_Task_Get_सङ्ख्या_सम्बन्धि_Tasks                MPU_ux_Task_Get_सङ्ख्या_सम्बन्धि_Tasks
        #परिभाषय ux_Task_Get_राशिः_उच्चः_Water_Mark           MPU_ux_Task_Get_राशिः_उच्चः_Water_Mark
        #परिभाषय ux_Task_Get_राशिः_उच्चः_Water_Mark2          MPU_ux_Task_Get_राशिः_उच्चः_Water_Mark2
        #परिभाषय v_Task_समुच्चयः_Application_Task_चिह्नकम्            MPU_v_Task_समुच्चयः_Application_Task_चिह्नकम्
        #परिभाषय x_Task_Get_Application_Task_चिह्नकम्            MPU_x_Task_Get_Application_Task_चिह्नकम्
        #परिभाषय v_Task_समुच्चयः_Thread_Local_Storage_Pointer     MPU_v_Task_समुच्चयः_Thread_Local_Storage_Pointer
        #परिभाषय pvTaskGetThreadLocalStoragePointer    MPU_pvTaskGetThreadLocalStoragePointer
        #परिभाषय xTaskGetIdleTaskHandle                MPU_xTaskGetIdleTaskHandle
        #परिभाषय uxTaskGetSystemState                  MPU_uxTaskGetSystemState
        #परिभाषय ul_Task_Get_Idle_सततम्_Time_गणक           MPU_ul_Task_Get_Idle_सततम्_Time_गणक
        #परिभाषय ul_Task_Get_Idle_सततम्_Time_Percent           MPU_ul_Task_Get_Idle_सततम्_Time_Percent
        #परिभाषय xTaskGenericNotify                    MPU_xTaskGenericNotify
        #परिभाषय xTaskGenericNotifyWait                MPU_xTaskGenericNotifyWait
        #परिभाषय ulTaskGenericNotifyTake               MPU_ulTaskGenericNotifyTake
        #परिभाषय xTaskGenericNotifyStateClear          MPU_xTaskGenericNotifyStateClear
        #परिभाषय ul_Task_Generic_Notify_मानम्_Clear         MPU_ul_Task_Generic_Notify_मानम्_Clear
        #परिभाषय v_Task_समुच्चयः_Time_बहिर्गमनम्_State                  MPU_v_Task_समुच्चयः_Time_बहिर्गमनम्_State
        #परिभाषय x_Task_Check_For_Time_बहिर्गमनम्                  MPU_x_Task_Check_For_Time_बहिर्गमनम्
        #परिभाषय xTaskGetCurrentTaskHandle             MPU_xTaskGetCurrentTaskHandle
        #परिभाषय xTaskGetSchedulerState                MPU_xTaskGetSchedulerState

        #यदि ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #परिभाषय ul_Task_Get_सततम्_Time_गणक           MPU_ul_Task_Get_सततम्_Time_गणक
            #परिभाषय ul_Task_Get_सततम्_Time_Percent           MPU_ul_Task_Get_सततम्_Time_Percent
        #यदि_अन्तः /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Privileged only wrappers for Task APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
        #परिभाषय xTaskCreate                              MPU_xTaskCreate
        #परिभाषय xTaskCreateStatic                        MPU_xTaskCreateStatic
        #परिभाषय vTaskDelete                              MPU_vTaskDelete
        #परिभाषय v_Task_प्राथमिकता_समुच्चयः                         MPU_v_Task_प्राथमिकता_समुच्चयः
        #परिभाषय xTaskGetHandle                           MPU_xTaskGetHandle
        #परिभाषय xTaskCallApplicationTaskHook             MPU_xTaskCallApplicationTaskHook

        #यदि ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #परिभाषय pcTaskGetName                        MPU_pcTaskGetName
            #परिभाषय xTaskCreateRestricted                MPU_xTaskCreateRestricted
            #परिभाषय xTaskCreateRestrictedStatic          MPU_xTaskCreateRestrictedStatic
            #परिभाषय vTaskAllocateMPURegions              MPU_vTaskAllocateMPURegions
            #परिभाषय xTaskGetStaticBuffers                MPU_xTaskGetStaticBuffers
            #परिभाषय ux_Task_प्राथमिकता_Get_From_ISR             MPU_ux_Task_प्राथमिकता_Get_From_ISR
            #परिभाषय ux_Task_आधारः_प्राथमिकता_Get                MPU_ux_Task_आधारः_प्राथमिकता_Get
            #परिभाषय ux_Task_आधारः_प्राथमिकता_Get_From_ISR         MPU_ux_Task_आधारः_प्राथमिकता_Get_From_ISR
            #परिभाषय xTaskResumeFromISR                   MPU_xTaskResumeFromISR
            #परिभाषय x_Task_Get_Application_Task_चिह्नकम्_From_ISR    MPU_x_Task_Get_Application_Task_चिह्नकम्_From_ISR
            #परिभाषय xTaskGenericNotifyFromISR            MPU_xTaskGenericNotifyFromISR
            #परिभाषय vTaskGenericNotifyGiveFromISR        MPU_vTaskGenericNotifyGiveFromISR
        #यदि_अन्तः /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Map standard queue.h API functions to the MPU equivalents. */
        #परिभाषय xQueueGenericSend            MPU_xQueueGenericSend
        #परिभाषय xQueueReceive                MPU_xQueueReceive
        #परिभाषय xQueuePeek                   MPU_xQueuePeek
        #परिभाषय xQueueSemaphoreTake          MPU_xQueueSemaphoreTake
        #परिभाषय ux_Queue_Messages_प्रतीक्षा       MPU_ux_Queue_Messages_प्रतीक्षा
        #परिभाषय ux_Queue_Spaces_उपलब्धम्       MPU_ux_Queue_Spaces_उपलब्धम्
        #परिभाषय xQueueGetMutexHolder         MPU_xQueueGetMutexHolder
        #परिभाषय xQueueTakeMutexRecursive     MPU_xQueueTakeMutexRecursive
        #परिभाषय xQueueGiveMutexRecursive     MPU_xQueueGiveMutexRecursive
        #परिभाषय x_Queue_सङ्कलय_गन्तव्यम्_समुच्चयः               MPU_x_Queue_सङ्कलय_गन्तव्यम्_समुच्चयः
        #परिभाषय x_Queue_Select_From_समुच्चयः          MPU_x_Queue_Select_From_समुच्चयः

        #यदि ( config_QUEUE_REGISTRY_आकार > 0 )
            #परिभाषय v_Queue_सङ्कलय_गन्तव्यम्_Registry      MPU_v_Queue_सङ्कलय_गन्तव्यम्_Registry
            #परिभाषय vQueueUnregisterQueue    MPU_vQueueUnregisterQueue
            #परिभाषय pcQueueGetName           MPU_pcQueueGetName
        #यदि_अन्तः /* #if ( configQUEUE_REGISTRY_SIZE > 0 ) */

/* Privileged only wrappers for Queue APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
        #परिभाषय vQueueDelete                           MPU_vQueueDelete
        #परिभाषय xQueueCreateMutex                      MPU_xQueueCreateMutex
        #परिभाषय xQueueCreateMutexStatic                MPU_xQueueCreateMutexStatic
        #परिभाषय xQueueCreateCountingSemaphore          MPU_xQueueCreateCountingSemaphore
        #परिभाषय xQueueCreateCountingSemaphoreStatic    MPU_xQueueCreateCountingSemaphoreStatic
        #परिभाषय xQueueGenericCreate                    MPU_xQueueGenericCreate
        #परिभाषय xQueueGenericCreateStatic              MPU_xQueueGenericCreateStatic
        #परिभाषय xQueueGenericReset                     MPU_xQueueGenericReset
        #परिभाषय x_Queue_Create_समुच्चयः                        MPU_x_Queue_Create_समुच्चयः
        #परिभाषय x_Queue_Remove_From_समुच्चयः                    MPU_x_Queue_Remove_From_समुच्चयः

        #यदि ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #परिभाषय xQueueGenericGetStaticBuffers      MPU_xQueueGenericGetStaticBuffers
            #परिभाषय xQueueGenericSendFromISR           MPU_xQueueGenericSendFromISR
            #परिभाषय xQueueGiveFromISR                  MPU_xQueueGiveFromISR
            #परिभाषय xQueuePeekFromISR                  MPU_xQueuePeekFromISR
            #परिभाषय xQueueReceiveFromISR               MPU_xQueueReceiveFromISR
            #परिभाषय x_Queue_Is_Queue_रिक्तानि_From_ISR          MPU_x_Queue_Is_Queue_रिक्तानि_From_ISR
            #परिभाषय x_Queue_Is_Queue_पूर्णम्_From_ISR           MPU_x_Queue_Is_Queue_पूर्णम्_From_ISR
            #परिभाषय ux_Queue_Messages_प्रतीक्षा_From_ISR      MPU_ux_Queue_Messages_प्रतीक्षा_From_ISR
            #परिभाषय xQueueGetMutexHolderFromISR        MPU_xQueueGetMutexHolderFromISR
            #परिभाषय x_Queue_Select_From_समुच्चयः_From_ISR         MPU_x_Queue_Select_From_समुच्चयः_From_ISR
        #यदि_अन्तः /* if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Map standard timer.h API functions to the MPU equivalents. */
        #परिभाषय pvTimerGetTimerID                 MPU_pvTimerGetTimerID
        #परिभाषय v_Timer_समुच्चयः_Timer_ID                  MPU_v_Timer_समुच्चयः_Timer_ID
        #परिभाषय x_Timer_Is_Timer_सक्रिय               MPU_x_Timer_Is_Timer_सक्रिय
        #परिभाषय xTimerGetTimerDaemonTaskHandle    MPU_xTimerGetTimerDaemonTaskHandle
        #परिभाषय xTimerGenericCommandFromTask      MPU_xTimerGenericCommandFromTask
        #परिभाषय pcTimerGetName                    MPU_pcTimerGetName
        #परिभाषय v_Timer_समुच्चयः_Reload_बहुलकम्               MPU_v_Timer_समुच्चयः_Reload_बहुलकम्
        #परिभाषय ux_Timer_Get_Reload_बहुलकम्              MPU_ux_Timer_Get_Reload_बहुलकम्
        #परिभाषय xTimerGetPeriod                   MPU_xTimerGetPeriod
        #परिभाषय xTimerGetExpiryTime               MPU_xTimerGetExpiryTime

/* Privileged only wrappers for Timer APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
        #यदि ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #परिभाषय x_Timer_Get_Reload_बहुलकम्            MPU_x_Timer_Get_Reload_बहुलकम्
            #परिभाषय xTimerCreate                   MPU_xTimerCreate
            #परिभाषय xTimerCreateStatic             MPU_xTimerCreateStatic
            #परिभाषय x_Timer_Get_Static_बफर          MPU_x_Timer_Get_Static_बफर
            #परिभाषय xTimerGenericCommandFromISR    MPU_xTimerGenericCommandFromISR
        #यदि_अन्तः /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Map standard event_group.h API functions to the MPU equivalents. */
        #परिभाषय x_Event_गणः_Wait_द्वयङ्काः          MPU_x_Event_गणः_Wait_द्वयङ्काः
        #परिभाषय x_Event_गणः_Clear_द्वयङ्काः         MPU_x_Event_गणः_Clear_द्वयङ्काः
        #परिभाषय x_Event_गणः_समुच्चयः_द्वयङ्काः           MPU_x_Event_गणः_समुच्चयः_द्वयङ्काः
        #परिभाषय x_Event_गणः_Sync              MPU_x_Event_गणः_Sync

        #यदि ( ( configUSE_TRACE_FACILITY == 1 ) && ( configUSE_MPU_WRAPPERS_V1 == 0 ) )
            #परिभाषय ux_Event_गणः_Get_सङ्ख्या    MPU_ux_Event_गणः_Get_सङ्ख्या
            #परिभाषय v_Event_गणः_समुच्चयः_सङ्ख्या     MPU_v_Event_गणः_समुच्चयः_सङ्ख्या
        #यदि_अन्तः /* #if ( ( configUSE_TRACE_FACILITY == 1 ) && ( configUSE_MPU_WRAPPERS_V1 == 0 ) ) */

/* Privileged only wrappers for Event Group APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
        #परिभाषय x_Event_गणः_Create                  MPU_x_Event_गणः_Create
        #परिभाषय x_Event_गणः_Create_Static            MPU_x_Event_गणः_Create_Static
        #परिभाषय v_Event_गणः_Delete                  MPU_v_Event_गणः_Delete

        #यदि ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #परिभाषय x_Event_गणः_Get_Static_बफर     MPU_x_Event_गणः_Get_Static_बफर
            #परिभाषय x_Event_गणः_Clear_द्वयङ्काः_From_ISR    MPU_x_Event_गणः_Clear_द्वयङ्काः_From_ISR
            #परिभाषय x_Event_गणः_समुच्चयः_द्वयङ्काः_From_ISR      MPU_x_Event_गणः_समुच्चयः_द्वयङ्काः_From_ISR
            #परिभाषय x_Event_गणः_Get_द्वयङ्काः_From_ISR      MPU_x_Event_गणः_Get_द्वयङ्काः_From_ISR
        #यदि_अन्तः /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Map standard message/stream_buffer.h API functions to the MPU
 * equivalents. */
        #परिभाषय x_प्रवाहः_बफर_Send                      MPU_x_प्रवाहः_बफर_Send
        #परिभाषय x_प्रवाहः_बफर_Receive                   MPU_x_प्रवाहः_बफर_Receive
        #परिभाषय x_प्रवाहः_बफर_Is_पूर्णम्                    MPU_x_प्रवाहः_बफर_Is_पूर्णम्
        #परिभाषय x_प्रवाहः_बफर_Is_रिक्तानि                   MPU_x_प्रवाहः_बफर_Is_रिक्तानि
        #परिभाषय x_प्रवाहः_बफर_Spaces_उपलब्धम्           MPU_x_प्रवाहः_बफर_Spaces_उपलब्धम्
        #परिभाषय x_प्रवाहः_बफर_अष्टकानि_उपलब्धम्            MPU_x_प्रवाहः_बफर_अष्टकानि_उपलब्धम्
        #परिभाषय x_प्रवाहः_बफर_समुच्चयः_Trigger_Level           MPU_x_प्रवाहः_बफर_समुच्चयः_Trigger_Level
        #परिभाषय x_प्रवाहः_बफर_अग्र_सन्देशः_दीर्घता_अष्टकानि    MPU_x_प्रवाहः_बफर_अग्र_सन्देशः_दीर्घता_अष्टकानि

/* Privileged only wrappers for Stream Buffer APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */

        #परिभाषय x_प्रवाहः_बफर_Generic_Create                  MPU_x_प्रवाहः_बफर_Generic_Create
        #परिभाषय x_प्रवाहः_बफर_Generic_Create_Static            MPU_x_प्रवाहः_बफर_Generic_Create_Static
        #परिभाषय v_प्रवाहः_बफर_Delete                         MPU_v_प्रवाहः_बफर_Delete
        #परिभाषय x_प्रवाहः_बफर_Reset                          MPU_x_प्रवाहः_बफर_Reset

        #यदि ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #परिभाषय x_प्रवाहः_बफर_Get_Static_Buffers           MPU_x_प्रवाहः_बफर_Get_Static_Buffers
            #परिभाषय x_प्रवाहः_बफर_Send_From_ISR                MPU_x_प्रवाहः_बफर_Send_From_ISR
            #परिभाषय x_प्रवाहः_बफर_Receive_From_ISR             MPU_x_प्रवाहः_बफर_Receive_From_ISR
            #परिभाषय x_प्रवाहः_बफर_Send_Completed_From_ISR       MPU_x_प्रवाहः_बफर_Send_Completed_From_ISR
            #परिभाषय x_प्रवाहः_बफर_Receive_Completed_From_ISR    MPU_x_प्रवाहः_बफर_Receive_Completed_From_ISR
            #परिभाषय x_प्रवाहः_बफर_Reset_From_ISR               MPU_x_प्रवाहः_बफर_Reset_From_ISR
        #यदि_अन्तः /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

        #यदि ( ( configUSE_MPU_WRAPPERS_V1 == 0 ) && ( configENABLE_ACCESS_CONTROL_LIST == 1 ) )

            #परिभाषय v_Grant_Access_गन्तव्यम्_Task( xTask, x_Task_गन्तव्यम्_Grant_Access )                        v_Grant_Access_गन्तव्यम्_Kernel_Object( ( xTask ), ( int32_t ) ( x_Task_गन्तव्यम्_Grant_Access ) )
            #परिभाषय v_Revoke_Access_गन्तव्यम्_Task( xTask, x_Task_गन्तव्यम्_Revoke_Access )                      v_Revoke_Access_गन्तव्यम्_Kernel_Object( ( xTask ), ( int32_t ) ( x_Task_गन्तव्यम्_Revoke_Access ) )

            #परिभाषय v_Grant_Access_गन्तव्यम्_Semaphore( xTask, x_Semaphore_गन्तव्यम्_Grant_Access )              v_Grant_Access_गन्तव्यम्_Kernel_Object( ( xTask ), ( int32_t ) ( x_Semaphore_गन्तव्यम्_Grant_Access ) )
            #परिभाषय v_Revoke_Access_गन्तव्यम्_Semaphore( xTask, x_Semaphore_गन्तव्यम्_Revoke_Access )            v_Revoke_Access_गन्तव्यम्_Kernel_Object( ( xTask ), ( int32_t ) ( x_Semaphore_गन्तव्यम्_Revoke_Access ) )

            #परिभाषय v_Grant_Access_गन्तव्यम्_Queue( xTask, x_Queue_गन्तव्यम्_Grant_Access )                      v_Grant_Access_गन्तव्यम्_Kernel_Object( ( xTask ), ( int32_t ) ( x_Queue_गन्तव्यम्_Grant_Access ) )
            #परिभाषय v_Revoke_Access_गन्तव्यम्_Queue( xTask, x_Queue_गन्तव्यम्_Revoke_Access )                    v_Revoke_Access_गन्तव्यम्_Kernel_Object( ( xTask ), ( int32_t ) ( x_Queue_गन्तव्यम्_Revoke_Access ) )

            #परिभाषय v_Grant_Access_गन्तव्यम्_Queue_समुच्चयः( xTask, x_Queue_समुच्चयः_गन्तव्यम्_Grant_Access )                v_Grant_Access_गन्तव्यम्_Kernel_Object( ( xTask ), ( int32_t ) ( x_Queue_समुच्चयः_गन्तव्यम्_Grant_Access ) )
            #परिभाषय v_Revoke_Access_गन्तव्यम्_Queue_समुच्चयः( xTask, x_Queue_समुच्चयः_गन्तव्यम्_Revoke_Access )              v_Revoke_Access_गन्तव्यम्_Kernel_Object( ( xTask ), ( int32_t ) ( x_Queue_समुच्चयः_गन्तव्यम्_Revoke_Access ) )

            #परिभाषय v_Grant_Access_गन्तव्यम्_Event_गणः( xTask, x_Event_गणः_गन्तव्यम्_Grant_Access )            v_Grant_Access_गन्तव्यम्_Kernel_Object( ( xTask ), ( int32_t ) ( x_Event_गणः_गन्तव्यम्_Grant_Access ) )
            #परिभाषय v_Revoke_Access_गन्तव्यम्_Event_गणः( xTask, x_Event_गणः_गन्तव्यम्_Revoke_Access )          v_Revoke_Access_गन्तव्यम्_Kernel_Object( ( xTask ), ( int32_t ) ( x_Event_गणः_गन्तव्यम्_Revoke_Access ) )

            #परिभाषय v_Grant_Access_गन्तव्यम्_प्रवाहः_बफर( xTask, x_प्रवाहः_बफर_गन्तव्यम्_Grant_Access )        v_Grant_Access_गन्तव्यम्_Kernel_Object( ( xTask ), ( int32_t ) ( x_प्रवाहः_बफर_गन्तव्यम्_Grant_Access ) )
            #परिभाषय v_Revoke_Access_गन्तव्यम्_प्रवाहः_बफर( xTask, x_प्रवाहः_बफर_गन्तव्यम्_Revoke_Access )      v_Revoke_Access_गन्तव्यम्_Kernel_Object( ( xTask ), ( int32_t ) ( x_प्रवाहः_बफर_गन्तव्यम्_Revoke_Access ) )

            #परिभाषय v_Grant_Access_गन्तव्यम्_सन्देशः_बफर( xTask, x_सन्देशः_बफर_गन्तव्यम्_Grant_Access )      v_Grant_Access_गन्तव्यम्_Kernel_Object( ( xTask ), ( int32_t ) ( x_सन्देशः_बफर_गन्तव्यम्_Grant_Access ) )
            #परिभाषय v_Revoke_Access_गन्तव्यम्_सन्देशः_बफर( xTask, x_सन्देशः_बफर_गन्तव्यम्_Revoke_Access )    v_Revoke_Access_गन्तव्यम्_Kernel_Object( ( xTask ), ( int32_t ) ( x_सन्देशः_बफर_गन्तव्यम्_Revoke_Access ) )

            #परिभाषय v_Grant_Access_गन्तव्यम्_Timer( xTask, x_Timer_गन्तव्यम्_Grant_Access )                      v_Grant_Access_गन्तव्यम्_Kernel_Object( ( xTask ), ( int32_t ) ( x_Timer_गन्तव्यम्_Grant_Access ) )
            #परिभाषय v_Revoke_Access_गन्तव्यम्_Timer( xTask, x_Timer_गन्तव्यम्_Revoke_Access )                    v_Revoke_Access_गन्तव्यम्_Kernel_Object( ( xTask ), ( int32_t ) ( x_Timer_गन्तव्यम्_Revoke_Access ) )

        #यदि_अन्तः /* #if ( ( configUSE_MPU_WRAPPERS_V1 == 0 ) && ( configENABLE_ACCESS_CONTROL_LIST == 1 ) ) */

    #यदि_अन्तः /* MPU_WRAPPERS_INCLUDED_FROM_API_FILE */

    #परिभाषय PRIVILEGED_FUNCTION     __attribute__( ( section( "privileged_functions" ) ) )
    #परिभाषय PRIVILEGED_दत्त         __attribute__( ( section( "privileged_data" ) ) )
    #परिभाषय FREERTOS_SYSTEM_CALL    __attribute__( ( section( "freertos_system_calls" ) ) )

#अन्यथा /* portUSING_MPU_WRAPPERS */

    #परिभाषय PRIVILEGED_FUNCTION
    #परिभाषय PRIVILEGED_दत्त
    #परिभाषय FREERTOS_SYSTEM_CALL

#यदि_अन्तः /* portUSING_MPU_WRAPPERS */


#यदि_अन्तः /* MPU_WRAPPERS_H */
