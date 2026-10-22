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

#अगर_अघोषित MPU_WRAPPERS_H
#घोषणा MPU_WRAPPERS_H

/* This file redefines API functions to be called through a wrapper macro, but
 * only for ports that are using the MPU. */
#अगर ( portUSING_MPU_WRAPPERS == 1 )

/* MPU_WRAPPERS_INCLUDED_FROM_API_FILE will be defined when this file is
 * included from queue.c or task.c to prevent it from having an effect within
 * those files. */
    #अगर_अघोषित MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/*
 * Map standard (non MPU) API functions to equivalents that start
 * "MPU_".  This will cause the application code to call the MPU_
 * version, which wraps the non-MPU version with privilege promoting
 * then demoting code, so the kernel code always runs will full
 * privileges.
 */

/* Map standard task.h API functions to the MPU equivalents. */
        #घोषणा vTaskDelay                            MPU_vTaskDelay
        #घोषणा xTaskDelayUntil                       MPU_xTaskDelayUntil
        #घोषणा xTaskAbortDelay                       MPU_xTaskAbortDelay
        #घोषणा uxTaskPriorityGet                     MPU_uxTaskPriorityGet
        #घोषणा eTaskGetState                         MPU_eTaskGetState
        #घोषणा vTaskGetInfo                          MPU_vTaskGetInfo
        #घोषणा vTaskSuspend                          MPU_vTaskSuspend
        #घोषणा vTaskResume                           MPU_vTaskResume
        #घोषणा x_Task_Get_Tick_गिनती                     MPU_x_Task_Get_Tick_गिनती
        #घोषणा ux_Task_Get_संख्या_Of_Tasks                MPU_ux_Task_Get_संख्या_Of_Tasks
        #घोषणा ux_Task_Get_ढेर_ऊपरी_Water_Mark           MPU_ux_Task_Get_ढेर_ऊपरी_Water_Mark
        #घोषणा ux_Task_Get_ढेर_ऊपरी_Water_Mark2          MPU_ux_Task_Get_ढेर_ऊपरी_Water_Mark2
        #घोषणा vTaskSetApplicationTaskTag            MPU_vTaskSetApplicationTaskTag
        #घोषणा xTaskGetApplicationTaskTag            MPU_xTaskGetApplicationTaskTag
        #घोषणा vTaskSetThreadLocalStoragePointer     MPU_vTaskSetThreadLocalStoragePointer
        #घोषणा pvTaskGetThreadLocalStoragePointer    MPU_pvTaskGetThreadLocalStoragePointer
        #घोषणा xTaskGetIdleTaskHandle                MPU_xTaskGetIdleTaskHandle
        #घोषणा uxTaskGetSystemState                  MPU_uxTaskGetSystemState
        #घोषणा ul_Task_Get_Idle_Run_Time_गणक           MPU_ul_Task_Get_Idle_Run_Time_गणक
        #घोषणा ulTaskGetIdleRunTimePercent           MPU_ulTaskGetIdleRunTimePercent
        #घोषणा xTaskGenericNotify                    MPU_xTaskGenericNotify
        #घोषणा xTaskGenericNotifyWait                MPU_xTaskGenericNotifyWait
        #घोषणा ulTaskGenericNotifyTake               MPU_ulTaskGenericNotifyTake
        #घोषणा xTaskGenericNotifyStateClear          MPU_xTaskGenericNotifyStateClear
        #घोषणा ul_Task_Generic_Notify_मूल्य_Clear         MPU_ul_Task_Generic_Notify_मूल्य_Clear
        #घोषणा vTaskSetTimeOutState                  MPU_vTaskSetTimeOutState
        #घोषणा xTaskCheckForTimeOut                  MPU_xTaskCheckForTimeOut
        #घोषणा xTaskGetCurrentTaskHandle             MPU_xTaskGetCurrentTaskHandle
        #घोषणा xTaskGetSchedulerState                MPU_xTaskGetSchedulerState

        #अगर ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #घोषणा ul_Task_Get_Run_Time_गणक           MPU_ul_Task_Get_Run_Time_गणक
            #घोषणा ulTaskGetRunTimePercent           MPU_ulTaskGetRunTimePercent
        #पूर्ण_अगर /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Privileged only wrappers for Task APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
        #घोषणा xTaskCreate                              MPU_xTaskCreate
        #घोषणा xTaskCreateStatic                        MPU_xTaskCreateStatic
        #घोषणा vTaskDelete                              MPU_vTaskDelete
        #घोषणा vTaskPrioritySet                         MPU_vTaskPrioritySet
        #घोषणा xTaskGetHandle                           MPU_xTaskGetHandle
        #घोषणा xTaskCallApplicationTaskHook             MPU_xTaskCallApplicationTaskHook

        #अगर ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #घोषणा pcTaskGetName                        MPU_pcTaskGetName
            #घोषणा xTaskCreateRestricted                MPU_xTaskCreateRestricted
            #घोषणा xTaskCreateRestrictedStatic          MPU_xTaskCreateRestrictedStatic
            #घोषणा vTaskAllocateMPURegions              MPU_vTaskAllocateMPURegions
            #घोषणा xTaskGetStaticBuffers                MPU_xTaskGetStaticBuffers
            #घोषणा uxTaskPriorityGetFromISR             MPU_uxTaskPriorityGetFromISR
            #घोषणा uxTaskBasePriorityGet                MPU_uxTaskBasePriorityGet
            #घोषणा uxTaskBasePriorityGetFromISR         MPU_uxTaskBasePriorityGetFromISR
            #घोषणा xTaskResumeFromISR                   MPU_xTaskResumeFromISR
            #घोषणा xTaskGetApplicationTaskTagFromISR    MPU_xTaskGetApplicationTaskTagFromISR
            #घोषणा xTaskGenericNotifyFromISR            MPU_xTaskGenericNotifyFromISR
            #घोषणा vTaskGenericNotifyGiveFromISR        MPU_vTaskGenericNotifyGiveFromISR
        #पूर्ण_अगर /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Map standard queue.h API functions to the MPU equivalents. */
        #घोषणा xQueueGenericSend            MPU_xQueueGenericSend
        #घोषणा xQueueReceive                MPU_xQueueReceive
        #घोषणा xQueuePeek                   MPU_xQueuePeek
        #घोषणा xQueueSemaphoreTake          MPU_xQueueSemaphoreTake
        #घोषणा uxQueueMessagesWaiting       MPU_uxQueueMessagesWaiting
        #घोषणा uxQueueSpacesAvailable       MPU_uxQueueSpacesAvailable
        #घोषणा xQueueGetMutexHolder         MPU_xQueueGetMutexHolder
        #घोषणा xQueueTakeMutexRecursive     MPU_xQueueTakeMutexRecursive
        #घोषणा xQueueGiveMutexRecursive     MPU_xQueueGiveMutexRecursive
        #घोषणा xQueueAddToSet               MPU_xQueueAddToSet
        #घोषणा xQueueSelectFromSet          MPU_xQueueSelectFromSet

        #अगर ( config_QUEUE_REGISTRY_आकार > 0 )
            #घोषणा vQueueAddToRegistry      MPU_vQueueAddToRegistry
            #घोषणा vQueueUnregisterQueue    MPU_vQueueUnregisterQueue
            #घोषणा pcQueueGetName           MPU_pcQueueGetName
        #पूर्ण_अगर /* #if ( configQUEUE_REGISTRY_SIZE > 0 ) */

/* Privileged only wrappers for Queue APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
        #घोषणा vQueueDelete                           MPU_vQueueDelete
        #घोषणा xQueueCreateMutex                      MPU_xQueueCreateMutex
        #घोषणा xQueueCreateMutexStatic                MPU_xQueueCreateMutexStatic
        #घोषणा xQueueCreateCountingSemaphore          MPU_xQueueCreateCountingSemaphore
        #घोषणा xQueueCreateCountingSemaphoreStatic    MPU_xQueueCreateCountingSemaphoreStatic
        #घोषणा xQueueGenericCreate                    MPU_xQueueGenericCreate
        #घोषणा xQueueGenericCreateStatic              MPU_xQueueGenericCreateStatic
        #घोषणा xQueueGenericReset                     MPU_xQueueGenericReset
        #घोषणा xQueueCreateSet                        MPU_xQueueCreateSet
        #घोषणा xQueueRemoveFromSet                    MPU_xQueueRemoveFromSet

        #अगर ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #घोषणा xQueueGenericGetStaticBuffers      MPU_xQueueGenericGetStaticBuffers
            #घोषणा xQueueGenericSendFromISR           MPU_xQueueGenericSendFromISR
            #घोषणा xQueueGiveFromISR                  MPU_xQueueGiveFromISR
            #घोषणा xQueuePeekFromISR                  MPU_xQueuePeekFromISR
            #घोषणा xQueueReceiveFromISR               MPU_xQueueReceiveFromISR
            #घोषणा xQueueIsQueueEmptyFromISR          MPU_xQueueIsQueueEmptyFromISR
            #घोषणा xQueueIsQueueFullFromISR           MPU_xQueueIsQueueFullFromISR
            #घोषणा uxQueueMessagesWaitingFromISR      MPU_uxQueueMessagesWaitingFromISR
            #घोषणा xQueueGetMutexHolderFromISR        MPU_xQueueGetMutexHolderFromISR
            #घोषणा xQueueSelectFromSetFromISR         MPU_xQueueSelectFromSetFromISR
        #पूर्ण_अगर /* if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Map standard timer.h API functions to the MPU equivalents. */
        #घोषणा pvTimerGetTimerID                 MPU_pvTimerGetTimerID
        #घोषणा vTimerSetTimerID                  MPU_vTimerSetTimerID
        #घोषणा xTimerIsTimerActive               MPU_xTimerIsTimerActive
        #घोषणा xTimerGetTimerDaemonTaskHandle    MPU_xTimerGetTimerDaemonTaskHandle
        #घोषणा xTimerGenericCommandFromTask      MPU_xTimerGenericCommandFromTask
        #घोषणा pcTimerGetName                    MPU_pcTimerGetName
        #घोषणा vTimerSetReloadMode               MPU_vTimerSetReloadMode
        #घोषणा uxTimerGetReloadMode              MPU_uxTimerGetReloadMode
        #घोषणा xTimerGetPeriod                   MPU_xTimerGetPeriod
        #घोषणा xTimerGetExpiryTime               MPU_xTimerGetExpiryTime

/* Privileged only wrappers for Timer APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
        #अगर ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #घोषणा xTimerGetReloadMode            MPU_xTimerGetReloadMode
            #घोषणा xTimerCreate                   MPU_xTimerCreate
            #घोषणा xTimerCreateStatic             MPU_xTimerCreateStatic
            #घोषणा x_Timer_Get_Static_बफर          MPU_x_Timer_Get_Static_बफर
            #घोषणा xTimerGenericCommandFromISR    MPU_xTimerGenericCommandFromISR
        #पूर्ण_अगर /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Map standard event_group.h API functions to the MPU equivalents. */
        #घोषणा xEventGroupWaitBits          MPU_xEventGroupWaitBits
        #घोषणा xEventGroupClearBits         MPU_xEventGroupClearBits
        #घोषणा xEventGroupSetBits           MPU_xEventGroupSetBits
        #घोषणा xEventGroupSync              MPU_xEventGroupSync

        #अगर ( ( configUSE_TRACE_FACILITY == 1 ) && ( configUSE_MPU_WRAPPERS_V1 == 0 ) )
            #घोषणा ux_Event_Group_Get_संख्या    MPU_ux_Event_Group_Get_संख्या
            #घोषणा v_Event_Group_Set_संख्या     MPU_v_Event_Group_Set_संख्या
        #पूर्ण_अगर /* #if ( ( configUSE_TRACE_FACILITY == 1 ) && ( configUSE_MPU_WRAPPERS_V1 == 0 ) ) */

/* Privileged only wrappers for Event Group APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
        #घोषणा xEventGroupCreate                  MPU_xEventGroupCreate
        #घोषणा xEventGroupCreateStatic            MPU_xEventGroupCreateStatic
        #घोषणा vEventGroupDelete                  MPU_vEventGroupDelete

        #अगर ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #घोषणा x_Event_Group_Get_Static_बफर     MPU_x_Event_Group_Get_Static_बफर
            #घोषणा xEventGroupClearBitsFromISR    MPU_xEventGroupClearBitsFromISR
            #घोषणा xEventGroupSetBitsFromISR      MPU_xEventGroupSetBitsFromISR
            #घोषणा xEventGroupGetBitsFromISR      MPU_xEventGroupGetBitsFromISR
        #पूर्ण_अगर /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Map standard message/stream_buffer.h API functions to the MPU
 * equivalents. */
        #घोषणा x_Stream_बफर_Send                      MPU_x_Stream_बफर_Send
        #घोषणा x_Stream_बफर_Receive                   MPU_x_Stream_बफर_Receive
        #घोषणा x_Stream_बफर_Is_Full                    MPU_x_Stream_बफर_Is_Full
        #घोषणा x_Stream_बफर_Is_Empty                   MPU_x_Stream_बफर_Is_Empty
        #घोषणा x_Stream_बफर_Spaces_Available           MPU_x_Stream_बफर_Spaces_Available
        #घोषणा x_Stream_बफर_Bytes_Available            MPU_x_Stream_बफर_Bytes_Available
        #घोषणा x_Stream_बफर_Set_Trigger_Level           MPU_x_Stream_बफर_Set_Trigger_Level
        #घोषणा x_Stream_बफर_अग्र_Message_लंबाई_Bytes    MPU_x_Stream_बफर_अग्र_Message_लंबाई_Bytes

/* Privileged only wrappers for Stream Buffer APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */

        #घोषणा x_Stream_बफर_Generic_Create                  MPU_x_Stream_बफर_Generic_Create
        #घोषणा x_Stream_बफर_Generic_Create_Static            MPU_x_Stream_बफर_Generic_Create_Static
        #घोषणा v_Stream_बफर_Delete                         MPU_v_Stream_बफर_Delete
        #घोषणा x_Stream_बफर_Reset                          MPU_x_Stream_बफर_Reset

        #अगर ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #घोषणा x_Stream_बफर_Get_Static_Buffers           MPU_x_Stream_बफर_Get_Static_Buffers
            #घोषणा x_Stream_बफर_Send_From_ISR                MPU_x_Stream_बफर_Send_From_ISR
            #घोषणा x_Stream_बफर_Receive_From_ISR             MPU_x_Stream_बफर_Receive_From_ISR
            #घोषणा x_Stream_बफर_Send_Completed_From_ISR       MPU_x_Stream_बफर_Send_Completed_From_ISR
            #घोषणा x_Stream_बफर_Receive_Completed_From_ISR    MPU_x_Stream_बफर_Receive_Completed_From_ISR
            #घोषणा x_Stream_बफर_Reset_From_ISR               MPU_x_Stream_बफर_Reset_From_ISR
        #पूर्ण_अगर /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

        #अगर ( ( configUSE_MPU_WRAPPERS_V1 == 0 ) && ( configENABLE_ACCESS_CONTROL_LIST == 1 ) )

            #घोषणा vGrantAccessToTask( xTask, xTaskToGrantAccess )                        vGrantAccessToKernelObject( ( xTask ), ( int32_t ) ( xTaskToGrantAccess ) )
            #घोषणा vRevokeAccessToTask( xTask, xTaskToRevokeAccess )                      vRevokeAccessToKernelObject( ( xTask ), ( int32_t ) ( xTaskToRevokeAccess ) )

            #घोषणा vGrantAccessToSemaphore( xTask, xSemaphoreToGrantAccess )              vGrantAccessToKernelObject( ( xTask ), ( int32_t ) ( xSemaphoreToGrantAccess ) )
            #घोषणा vRevokeAccessToSemaphore( xTask, xSemaphoreToRevokeAccess )            vRevokeAccessToKernelObject( ( xTask ), ( int32_t ) ( xSemaphoreToRevokeAccess ) )

            #घोषणा vGrantAccessToQueue( xTask, xQueueToGrantAccess )                      vGrantAccessToKernelObject( ( xTask ), ( int32_t ) ( xQueueToGrantAccess ) )
            #घोषणा vRevokeAccessToQueue( xTask, xQueueToRevokeAccess )                    vRevokeAccessToKernelObject( ( xTask ), ( int32_t ) ( xQueueToRevokeAccess ) )

            #घोषणा vGrantAccessToQueueSet( xTask, xQueueSetToGrantAccess )                vGrantAccessToKernelObject( ( xTask ), ( int32_t ) ( xQueueSetToGrantAccess ) )
            #घोषणा vRevokeAccessToQueueSet( xTask, xQueueSetToRevokeAccess )              vRevokeAccessToKernelObject( ( xTask ), ( int32_t ) ( xQueueSetToRevokeAccess ) )

            #घोषणा vGrantAccessToEventGroup( xTask, xEventGroupToGrantAccess )            vGrantAccessToKernelObject( ( xTask ), ( int32_t ) ( xEventGroupToGrantAccess ) )
            #घोषणा vRevokeAccessToEventGroup( xTask, xEventGroupToRevokeAccess )          vRevokeAccessToKernelObject( ( xTask ), ( int32_t ) ( xEventGroupToRevokeAccess ) )

            #घोषणा v_Grant_Access_To_Stream_बफर( xTask, x_Stream_बफर_To_Grant_Access )        vGrantAccessToKernelObject( ( xTask ), ( int32_t ) ( x_Stream_बफर_To_Grant_Access ) )
            #घोषणा v_Revoke_Access_To_Stream_बफर( xTask, x_Stream_बफर_To_Revoke_Access )      vRevokeAccessToKernelObject( ( xTask ), ( int32_t ) ( x_Stream_बफर_To_Revoke_Access ) )

            #घोषणा v_Grant_Access_To_Message_बफर( xTask, x_Message_बफर_To_Grant_Access )      vGrantAccessToKernelObject( ( xTask ), ( int32_t ) ( x_Message_बफर_To_Grant_Access ) )
            #घोषणा v_Revoke_Access_To_Message_बफर( xTask, x_Message_बफर_To_Revoke_Access )    vRevokeAccessToKernelObject( ( xTask ), ( int32_t ) ( x_Message_बफर_To_Revoke_Access ) )

            #घोषणा vGrantAccessToTimer( xTask, xTimerToGrantAccess )                      vGrantAccessToKernelObject( ( xTask ), ( int32_t ) ( xTimerToGrantAccess ) )
            #घोषणा vRevokeAccessToTimer( xTask, xTimerToRevokeAccess )                    vRevokeAccessToKernelObject( ( xTask ), ( int32_t ) ( xTimerToRevokeAccess ) )

        #पूर्ण_अगर /* #if ( ( configUSE_MPU_WRAPPERS_V1 == 0 ) && ( configENABLE_ACCESS_CONTROL_LIST == 1 ) ) */

    #पूर्ण_अगर /* MPU_WRAPPERS_INCLUDED_FROM_API_FILE */

    #घोषणा PRIVILEGED_FUNCTION     __attribute__( ( section( "privileged_functions" ) ) )
    #घोषणा PRIVILEGED_तथ्य         __attribute__( ( section( "privileged_data" ) ) )
    #घोषणा FREERTOS_SYSTEM_CALL    __attribute__( ( section( "freertos_system_calls" ) ) )

#अन्यथा /* portUSING_MPU_WRAPPERS */

    #घोषणा PRIVILEGED_FUNCTION
    #घोषणा PRIVILEGED_तथ्य
    #घोषणा FREERTOS_SYSTEM_CALL

#पूर्ण_अगर /* portUSING_MPU_WRAPPERS */


#पूर्ण_अगर /* MPU_WRAPPERS_H */
