/*
 * FreeRTOS Kernel V10.5.1 (ESP-IDF SMP modified)
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-FileCopyrightText: 2021 Amazon.com, Inc. or its affiliates
 *
 * SPDX-License-Identifier: MIT
 *
 * SPDX-FileContributor: 2023 Espressif Systems (Shanghai) CO LTD
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
        #घोषणा xTaskCreate                            MPU_xTaskCreate
        #घोषणा xTaskCreateStatic                      MPU_xTaskCreateStatic
        #घोषणा vTaskDelete                            MPU_vTaskDelete
        #घोषणा vTaskDelay                             MPU_vTaskDelay
        #घोषणा xTaskDelayUntil                        MPU_xTaskDelayUntil
        #घोषणा xTaskAbortDelay                        MPU_xTaskAbortDelay
        #घोषणा uxTaskPriorityGet                      MPU_uxTaskPriorityGet
        #घोषणा eTaskGetState                          MPU_eTaskGetState
        #घोषणा vTaskGetInfo                           MPU_vTaskGetInfo
        #घोषणा vTaskPrioritySet                       MPU_vTaskPrioritySet
        #घोषणा vTaskSuspend                           MPU_vTaskSuspend
        #घोषणा vTaskResume                            MPU_vTaskResume
        #घोषणा vTaskSuspendAll                        MPU_vTaskSuspendAll
        #घोषणा xTaskResumeAll                         MPU_xTaskResumeAll
        #घोषणा x_Task_Get_Tick_गिनती                      MPU_x_Task_Get_Tick_गिनती
        #घोषणा ux_Task_Get_संख्या_Of_Tasks                 MPU_ux_Task_Get_संख्या_Of_Tasks
        #घोषणा pcTaskGetName                          MPU_pcTaskGetName
        #घोषणा xTaskGetHandle                         MPU_xTaskGetHandle
        #घोषणा ux_Task_Get_ढेर_ऊपरी_Water_Mark            MPU_ux_Task_Get_ढेर_ऊपरी_Water_Mark
        #घोषणा ux_Task_Get_ढेर_ऊपरी_Water_Mark2           MPU_ux_Task_Get_ढेर_ऊपरी_Water_Mark2
        #घोषणा vTaskSetApplicationTaskTag             MPU_vTaskSetApplicationTaskTag
        #घोषणा xTaskGetApplicationTaskTag             MPU_xTaskGetApplicationTaskTag
        #घोषणा vTaskSetThreadLocalStoragePointer      MPU_vTaskSetThreadLocalStoragePointer
        #घोषणा pvTaskGetThreadLocalStoragePointer     MPU_pvTaskGetThreadLocalStoragePointer
        #घोषणा xTaskCallApplicationTaskHook           MPU_xTaskCallApplicationTaskHook
        #घोषणा xTaskGetIdleTaskHandle                 MPU_xTaskGetIdleTaskHandle
        #घोषणा uxTaskGetSystemState                   MPU_uxTaskGetSystemState
        #घोषणा vTaskList                              MPU_vTaskList
        #घोषणा vTaskGetRunTimeStats                   MPU_vTaskGetRunTimeStats
        #घोषणा ul_Task_Get_Idle_Run_Time_गणक            MPU_ul_Task_Get_Idle_Run_Time_गणक
        #घोषणा ulTaskGetIdleRunTimePercent            MPU_ulTaskGetIdleRunTimePercent
        #घोषणा xTaskGenericNotify                     MPU_xTaskGenericNotify
        #घोषणा xTaskGenericNotifyWait                 MPU_xTaskGenericNotifyWait
        #घोषणा ulTaskGenericNotifyTake                MPU_ulTaskGenericNotifyTake
        #घोषणा xTaskGenericNotifyStateClear           MPU_xTaskGenericNotifyStateClear
        #घोषणा ul_Task_Generic_Notify_मूल्य_Clear          MPU_ul_Task_Generic_Notify_मूल्य_Clear
        #घोषणा xTaskCatchUpTicks                      MPU_xTaskCatchUpTicks

        #घोषणा xTaskGetCurrentTaskHandle              MPU_xTaskGetCurrentTaskHandle
        #घोषणा vTaskSetTimeOutState                   MPU_vTaskSetTimeOutState
        #घोषणा xTaskCheckForTimeOut                   MPU_xTaskCheckForTimeOut
        #घोषणा xTaskGetSchedulerState                 MPU_xTaskGetSchedulerState

/* Map standard queue.h API functions to the MPU equivalents. */
        #घोषणा xQueueGenericSend                      MPU_xQueueGenericSend
        #घोषणा xQueueReceive                          MPU_xQueueReceive
        #घोषणा xQueuePeek                             MPU_xQueuePeek
        #घोषणा xQueueSemaphoreTake                    MPU_xQueueSemaphoreTake
        #घोषणा uxQueueMessagesWaiting                 MPU_uxQueueMessagesWaiting
        #घोषणा uxQueueSpacesAvailable                 MPU_uxQueueSpacesAvailable
        #घोषणा vQueueDelete                           MPU_vQueueDelete
        #घोषणा xQueueCreateMutex                      MPU_xQueueCreateMutex
        #घोषणा xQueueCreateMutexStatic                MPU_xQueueCreateMutexStatic
        #घोषणा xQueueCreateCountingSemaphore          MPU_xQueueCreateCountingSemaphore
        #घोषणा xQueueCreateCountingSemaphoreStatic    MPU_xQueueCreateCountingSemaphoreStatic
        #घोषणा xQueueGetMutexHolder                   MPU_xQueueGetMutexHolder
        #घोषणा xQueueTakeMutexRecursive               MPU_xQueueTakeMutexRecursive
        #घोषणा xQueueGiveMutexRecursive               MPU_xQueueGiveMutexRecursive
        #घोषणा xQueueGenericCreate                    MPU_xQueueGenericCreate
        #घोषणा xQueueGenericCreateStatic              MPU_xQueueGenericCreateStatic
        #घोषणा xQueueCreateSet                        MPU_xQueueCreateSet
        #घोषणा xQueueAddToSet                         MPU_xQueueAddToSet
        #घोषणा xQueueRemoveFromSet                    MPU_xQueueRemoveFromSet
        #घोषणा xQueueSelectFromSet                    MPU_xQueueSelectFromSet
        #घोषणा xQueueGenericReset                     MPU_xQueueGenericReset

        #अगर ( config_QUEUE_REGISTRY_आकार > 0 )
            #घोषणा vQueueAddToRegistry                MPU_vQueueAddToRegistry
            #घोषणा vQueueUnregisterQueue              MPU_vQueueUnregisterQueue
            #घोषणा pcQueueGetName                     MPU_pcQueueGetName
        #पूर्ण_अगर

/* Map standard timer.h API functions to the MPU equivalents. */
        #घोषणा pvTimerGetTimerID                      MPU_pvTimerGetTimerID
        #घोषणा vTimerSetTimerID                       MPU_vTimerSetTimerID
        #घोषणा xTimerIsTimerActive                    MPU_xTimerIsTimerActive
        #घोषणा xTimerGetTimerDaemonTaskHandle         MPU_xTimerGetTimerDaemonTaskHandle
        #घोषणा pcTimerGetName                         MPU_pcTimerGetName
        #घोषणा vTimerSetReloadMode                    MPU_vTimerSetReloadMode
        #घोषणा uxTimerGetReloadMode                   MPU_uxTimerGetReloadMode
        #घोषणा xTimerGetPeriod                        MPU_xTimerGetPeriod
        #घोषणा xTimerGetExpiryTime                    MPU_xTimerGetExpiryTime
        #घोषणा xTimerGenericCommand                   MPU_xTimerGenericCommand

/* Map standard event_group.h API functions to the MPU equivalents. */
        #घोषणा xEventGroupCreate                      MPU_xEventGroupCreate
        #घोषणा xEventGroupCreateStatic                MPU_xEventGroupCreateStatic
        #घोषणा xEventGroupWaitBits                    MPU_xEventGroupWaitBits
        #घोषणा xEventGroupClearBits                   MPU_xEventGroupClearBits
        #घोषणा xEventGroupSetBits                     MPU_xEventGroupSetBits
        #घोषणा xEventGroupSync                        MPU_xEventGroupSync
        #घोषणा vEventGroupDelete                      MPU_vEventGroupDelete

/* Map standard message/stream_buffer.h API functions to the MPU
 * equivalents. */
        #घोषणा x_Stream_बफर_Send                      MPU_x_Stream_बफर_Send
        #घोषणा x_Stream_बफर_Receive                   MPU_x_Stream_बफर_Receive
        #घोषणा x_Stream_बफर_अग्र_Message_लंबाई_Bytes    MPU_x_Stream_बफर_अग्र_Message_लंबाई_Bytes
        #घोषणा v_Stream_बफर_Delete                    MPU_v_Stream_बफर_Delete
        #घोषणा x_Stream_बफर_Is_Full                    MPU_x_Stream_बफर_Is_Full
        #घोषणा x_Stream_बफर_Is_Empty                   MPU_x_Stream_बफर_Is_Empty
        #घोषणा x_Stream_बफर_Reset                     MPU_x_Stream_बफर_Reset
        #घोषणा x_Stream_बफर_Spaces_Available           MPU_x_Stream_बफर_Spaces_Available
        #घोषणा x_Stream_बफर_Bytes_Available            MPU_x_Stream_बफर_Bytes_Available
        #घोषणा x_Stream_बफर_Set_Trigger_Level           MPU_x_Stream_बफर_Set_Trigger_Level
        #घोषणा x_Stream_बफर_Generic_Create             MPU_x_Stream_बफर_Generic_Create
        #घोषणा x_Stream_बफर_Generic_Create_Static       MPU_x_Stream_बफर_Generic_Create_Static


/* Remove the privileged function macro, but keep the PRIVILEGED_DATA
 * macro so applications can place data in privileged access sections
 * (useful when using statically allocated objects). */
        #घोषणा PRIVILEGED_FUNCTION
        #घोषणा PRIVILEGED_तथ्य    __attribute__( ( section( "privileged_data" ) ) )
        #घोषणा FREERTOS_SYSTEM_CALL

    #अन्यथा /* MPU_WRAPPERS_INCLUDED_FROM_API_FILE */

/* Ensure API functions go in the privileged execution section. */
        #घोषणा PRIVILEGED_FUNCTION     __attribute__( ( section( "privileged_functions" ) ) )
        #घोषणा PRIVILEGED_तथ्य         __attribute__( ( section( "privileged_data" ) ) )
        #घोषणा FREERTOS_SYSTEM_CALL    __attribute__( ( section( "freertos_system_calls" ) ) )

    #पूर्ण_अगर /* MPU_WRAPPERS_INCLUDED_FROM_API_FILE */

#अन्यथा /* portUSING_MPU_WRAPPERS */

    #घोषणा PRIVILEGED_FUNCTION
    #घोषणा PRIVILEGED_तथ्य
    #घोषणा FREERTOS_SYSTEM_CALL

#पूर्ण_अगर /* portUSING_MPU_WRAPPERS */


#पूर्ण_अगर /* MPU_WRAPPERS_H */
