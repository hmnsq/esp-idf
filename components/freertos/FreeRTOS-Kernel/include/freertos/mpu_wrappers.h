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
        #परिभाषय xTaskCreate                            MPU_xTaskCreate
        #परिभाषय xTaskCreateStatic                      MPU_xTaskCreateStatic
        #परिभाषय vTaskDelete                            MPU_vTaskDelete
        #परिभाषय vTaskDelay                             MPU_vTaskDelay
        #परिभाषय xTaskDelayUntil                        MPU_xTaskDelayUntil
        #परिभाषय xTaskAbortDelay                        MPU_xTaskAbortDelay
        #परिभाषय ux_Task_प्राथमिकता_Get                      MPU_ux_Task_प्राथमिकता_Get
        #परिभाषय eTaskGetState                          MPU_eTaskGetState
        #परिभाषय vTaskGetInfo                           MPU_vTaskGetInfo
        #परिभाषय v_Task_प्राथमिकता_समुच्चयः                       MPU_v_Task_प्राथमिकता_समुच्चयः
        #परिभाषय vTaskSuspend                           MPU_vTaskSuspend
        #परिभाषय vTaskResume                            MPU_vTaskResume
        #परिभाषय vTaskSuspendAll                        MPU_vTaskSuspendAll
        #परिभाषय xTaskResumeAll                         MPU_xTaskResumeAll
        #परिभाषय x_Task_Get_Tick_गणनम्                      MPU_x_Task_Get_Tick_गणनम्
        #परिभाषय ux_Task_Get_सङ्ख्या_सम्बन्धि_Tasks                 MPU_ux_Task_Get_सङ्ख्या_सम्बन्धि_Tasks
        #परिभाषय pcTaskGetName                          MPU_pcTaskGetName
        #परिभाषय xTaskGetHandle                         MPU_xTaskGetHandle
        #परिभाषय ux_Task_Get_राशिः_उच्चः_Water_Mark            MPU_ux_Task_Get_राशिः_उच्चः_Water_Mark
        #परिभाषय ux_Task_Get_राशिः_उच्चः_Water_Mark2           MPU_ux_Task_Get_राशिः_उच्चः_Water_Mark2
        #परिभाषय v_Task_समुच्चयः_Application_Task_चिह्नकम्             MPU_v_Task_समुच्चयः_Application_Task_चिह्नकम्
        #परिभाषय x_Task_Get_Application_Task_चिह्नकम्             MPU_x_Task_Get_Application_Task_चिह्नकम्
        #परिभाषय v_Task_समुच्चयः_Thread_Local_Storage_Pointer      MPU_v_Task_समुच्चयः_Thread_Local_Storage_Pointer
        #परिभाषय pvTaskGetThreadLocalStoragePointer     MPU_pvTaskGetThreadLocalStoragePointer
        #परिभाषय xTaskCallApplicationTaskHook           MPU_xTaskCallApplicationTaskHook
        #परिभाषय xTaskGetIdleTaskHandle                 MPU_xTaskGetIdleTaskHandle
        #परिभाषय uxTaskGetSystemState                   MPU_uxTaskGetSystemState
        #परिभाषय vTaskList                              MPU_vTaskList
        #परिभाषय v_Task_Get_सततम्_Time_Stats                   MPU_v_Task_Get_सततम्_Time_Stats
        #परिभाषय ul_Task_Get_Idle_सततम्_Time_गणक            MPU_ul_Task_Get_Idle_सततम्_Time_गणक
        #परिभाषय ul_Task_Get_Idle_सततम्_Time_Percent            MPU_ul_Task_Get_Idle_सततम्_Time_Percent
        #परिभाषय xTaskGenericNotify                     MPU_xTaskGenericNotify
        #परिभाषय xTaskGenericNotifyWait                 MPU_xTaskGenericNotifyWait
        #परिभाषय ulTaskGenericNotifyTake                MPU_ulTaskGenericNotifyTake
        #परिभाषय xTaskGenericNotifyStateClear           MPU_xTaskGenericNotifyStateClear
        #परिभाषय ul_Task_Generic_Notify_मानम्_Clear          MPU_ul_Task_Generic_Notify_मानम्_Clear
        #परिभाषय x_Task_Catch_ऊर्ध्वम्_Ticks                      MPU_x_Task_Catch_ऊर्ध्वम्_Ticks

        #परिभाषय xTaskGetCurrentTaskHandle              MPU_xTaskGetCurrentTaskHandle
        #परिभाषय v_Task_समुच्चयः_Time_बहिर्गमनम्_State                   MPU_v_Task_समुच्चयः_Time_बहिर्गमनम्_State
        #परिभाषय x_Task_Check_For_Time_बहिर्गमनम्                   MPU_x_Task_Check_For_Time_बहिर्गमनम्
        #परिभाषय xTaskGetSchedulerState                 MPU_xTaskGetSchedulerState

/* Map standard queue.h API functions to the MPU equivalents. */
        #परिभाषय xQueueGenericSend                      MPU_xQueueGenericSend
        #परिभाषय xQueueReceive                          MPU_xQueueReceive
        #परिभाषय xQueuePeek                             MPU_xQueuePeek
        #परिभाषय xQueueSemaphoreTake                    MPU_xQueueSemaphoreTake
        #परिभाषय ux_Queue_Messages_प्रतीक्षा                 MPU_ux_Queue_Messages_प्रतीक्षा
        #परिभाषय ux_Queue_Spaces_उपलब्धम्                 MPU_ux_Queue_Spaces_उपलब्धम्
        #परिभाषय vQueueDelete                           MPU_vQueueDelete
        #परिभाषय xQueueCreateMutex                      MPU_xQueueCreateMutex
        #परिभाषय xQueueCreateMutexStatic                MPU_xQueueCreateMutexStatic
        #परिभाषय xQueueCreateCountingSemaphore          MPU_xQueueCreateCountingSemaphore
        #परिभाषय xQueueCreateCountingSemaphoreStatic    MPU_xQueueCreateCountingSemaphoreStatic
        #परिभाषय xQueueGetMutexHolder                   MPU_xQueueGetMutexHolder
        #परिभाषय xQueueTakeMutexRecursive               MPU_xQueueTakeMutexRecursive
        #परिभाषय xQueueGiveMutexRecursive               MPU_xQueueGiveMutexRecursive
        #परिभाषय xQueueGenericCreate                    MPU_xQueueGenericCreate
        #परिभाषय xQueueGenericCreateStatic              MPU_xQueueGenericCreateStatic
        #परिभाषय x_Queue_Create_समुच्चयः                        MPU_x_Queue_Create_समुच्चयः
        #परिभाषय x_Queue_सङ्कलय_गन्तव्यम्_समुच्चयः                         MPU_x_Queue_सङ्कलय_गन्तव्यम्_समुच्चयः
        #परिभाषय x_Queue_Remove_From_समुच्चयः                    MPU_x_Queue_Remove_From_समुच्चयः
        #परिभाषय x_Queue_Select_From_समुच्चयः                    MPU_x_Queue_Select_From_समुच्चयः
        #परिभाषय xQueueGenericReset                     MPU_xQueueGenericReset

        #यदि ( config_QUEUE_REGISTRY_आकार > 0 )
            #परिभाषय v_Queue_सङ्कलय_गन्तव्यम्_Registry                MPU_v_Queue_सङ्कलय_गन्तव्यम्_Registry
            #परिभाषय vQueueUnregisterQueue              MPU_vQueueUnregisterQueue
            #परिभाषय pcQueueGetName                     MPU_pcQueueGetName
        #यदि_अन्तः

/* Map standard timer.h API functions to the MPU equivalents. */
        #परिभाषय pvTimerGetTimerID                      MPU_pvTimerGetTimerID
        #परिभाषय v_Timer_समुच्चयः_Timer_ID                       MPU_v_Timer_समुच्चयः_Timer_ID
        #परिभाषय x_Timer_Is_Timer_सक्रिय                    MPU_x_Timer_Is_Timer_सक्रिय
        #परिभाषय xTimerGetTimerDaemonTaskHandle         MPU_xTimerGetTimerDaemonTaskHandle
        #परिभाषय pcTimerGetName                         MPU_pcTimerGetName
        #परिभाषय v_Timer_समुच्चयः_Reload_बहुलकम्                    MPU_v_Timer_समुच्चयः_Reload_बहुलकम्
        #परिभाषय ux_Timer_Get_Reload_बहुलकम्                   MPU_ux_Timer_Get_Reload_बहुलकम्
        #परिभाषय xTimerGetPeriod                        MPU_xTimerGetPeriod
        #परिभाषय xTimerGetExpiryTime                    MPU_xTimerGetExpiryTime
        #परिभाषय xTimerGenericCommand                   MPU_xTimerGenericCommand

/* Map standard event_group.h API functions to the MPU equivalents. */
        #परिभाषय x_Event_गणः_Create                      MPU_x_Event_गणः_Create
        #परिभाषय x_Event_गणः_Create_Static                MPU_x_Event_गणः_Create_Static
        #परिभाषय x_Event_गणः_Wait_द्वयङ्काः                    MPU_x_Event_गणः_Wait_द्वयङ्काः
        #परिभाषय x_Event_गणः_Clear_द्वयङ्काः                   MPU_x_Event_गणः_Clear_द्वयङ्काः
        #परिभाषय x_Event_गणः_समुच्चयः_द्वयङ्काः                     MPU_x_Event_गणः_समुच्चयः_द्वयङ्काः
        #परिभाषय x_Event_गणः_Sync                        MPU_x_Event_गणः_Sync
        #परिभाषय v_Event_गणः_Delete                      MPU_v_Event_गणः_Delete

/* Map standard message/stream_buffer.h API functions to the MPU
 * equivalents. */
        #परिभाषय x_प्रवाहः_बफर_Send                      MPU_x_प्रवाहः_बफर_Send
        #परिभाषय x_प्रवाहः_बफर_Receive                   MPU_x_प्रवाहः_बफर_Receive
        #परिभाषय x_प्रवाहः_बफर_अग्र_सन्देशः_दीर्घता_अष्टकानि    MPU_x_प्रवाहः_बफर_अग्र_सन्देशः_दीर्घता_अष्टकानि
        #परिभाषय v_प्रवाहः_बफर_Delete                    MPU_v_प्रवाहः_बफर_Delete
        #परिभाषय x_प्रवाहः_बफर_Is_पूर्णम्                    MPU_x_प्रवाहः_बफर_Is_पूर्णम्
        #परिभाषय x_प्रवाहः_बफर_Is_रिक्तानि                   MPU_x_प्रवाहः_बफर_Is_रिक्तानि
        #परिभाषय x_प्रवाहः_बफर_Reset                     MPU_x_प्रवाहः_बफर_Reset
        #परिभाषय x_प्रवाहः_बफर_Spaces_उपलब्धम्           MPU_x_प्रवाहः_बफर_Spaces_उपलब्धम्
        #परिभाषय x_प्रवाहः_बफर_अष्टकानि_उपलब्धम्            MPU_x_प्रवाहः_बफर_अष्टकानि_उपलब्धम्
        #परिभाषय x_प्रवाहः_बफर_समुच्चयः_Trigger_Level           MPU_x_प्रवाहः_बफर_समुच्चयः_Trigger_Level
        #परिभाषय x_प्रवाहः_बफर_Generic_Create             MPU_x_प्रवाहः_बफर_Generic_Create
        #परिभाषय x_प्रवाहः_बफर_Generic_Create_Static       MPU_x_प्रवाहः_बफर_Generic_Create_Static


/* Remove the privileged function macro, but keep the PRIVILEGED_DATA
 * macro so applications can place data in privileged access sections
 * (useful when using statically allocated objects). */
        #परिभाषय PRIVILEGED_FUNCTION
        #परिभाषय PRIVILEGED_दत्त    __attribute__( ( section( "privileged_data" ) ) )
        #परिभाषय FREERTOS_SYSTEM_CALL

    #अन्यथा /* MPU_WRAPPERS_INCLUDED_FROM_API_FILE */

/* Ensure API functions go in the privileged execution section. */
        #परिभाषय PRIVILEGED_FUNCTION     __attribute__( ( section( "privileged_functions" ) ) )
        #परिभाषय PRIVILEGED_दत्त         __attribute__( ( section( "privileged_data" ) ) )
        #परिभाषय FREERTOS_SYSTEM_CALL    __attribute__( ( section( "freertos_system_calls" ) ) )

    #यदि_अन्तः /* MPU_WRAPPERS_INCLUDED_FROM_API_FILE */

#अन्यथा /* portUSING_MPU_WRAPPERS */

    #परिभाषय PRIVILEGED_FUNCTION
    #परिभाषय PRIVILEGED_दत्त
    #परिभाषय FREERTOS_SYSTEM_CALL

#यदि_अन्तः /* portUSING_MPU_WRAPPERS */


#यदि_अन्तः /* MPU_WRAPPERS_H */
