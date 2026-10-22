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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ MPU_WRAPPERS_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MPU_WRAPPERS_H

/* This file redefines API functions to be called through a wrapper macro, but
 * only for ports that are using the MPU. */
#ਜੇ ( ਪੋਰਟ_USING_MPU_WRAPPERS == 1 )

/* MPU_WRAPPERS_INCLUDED_FROM_API_FILE will be defined when this file is
 * included from queue.c or task.c to prevent it from having an effect within
 * those files. */
    #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/*
 * Map standard (non MPU) API functions to equivalents that start
 * "MPU_".  This will cause the application code to call the MPU_
 * version, which wraps the non-MPU version with privilege promoting
 * then demoting code, so the kernel code always runs will full
 * privileges.
 */

/* Map standard task.h API functions to the MPU equivalents. */
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTaskCreate                            MPU_xTaskCreate
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTaskCreateStatic                      MPU_xTaskCreateStatic
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ vTaskDelete                            MPU_vTaskDelete
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ vTaskDelay                             MPU_vTaskDelay
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTaskDelayUntil                        MPU_xTaskDelayUntil
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTaskAbortDelay                        MPU_xTaskAbortDelay
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ux_Task_ਤਰਜੀਹ_Get                      MPU_ux_Task_ਤਰਜੀਹ_Get
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ eTaskGetState                          MPU_eTaskGetState
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ vTaskGetInfo                           MPU_vTaskGetInfo
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_Task_ਤਰਜੀਹ_ਸੈੱਟ                       MPU_v_Task_ਤਰਜੀਹ_ਸੈੱਟ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ vTaskSuspend                           MPU_vTaskSuspend
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ vTaskResume                            MPU_vTaskResume
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ vTaskSuspendAll                        MPU_vTaskSuspendAll
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTaskResumeAll                         MPU_xTaskResumeAll
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_Task_Get_Tick_ਗਿਣਤੀ                      MPU_x_Task_Get_Tick_ਗਿਣਤੀ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ux_Task_Get_ਸੰਖਿਆ_ਦਾ_Tasks                 MPU_ux_Task_Get_ਸੰਖਿਆ_ਦਾ_Tasks
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pcTaskGetName                          MPU_pcTaskGetName
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTaskGetHandle                         MPU_xTaskGetHandle
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ux_Task_Get_ਢੇਰ_ਉੱਚਾ_Water_Mark            MPU_ux_Task_Get_ਢੇਰ_ਉੱਚਾ_Water_Mark
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ux_Task_Get_ਢੇਰ_ਉੱਚਾ_Water_Mark2           MPU_ux_Task_Get_ਢੇਰ_ਉੱਚਾ_Water_Mark2
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_Task_ਸੈੱਟ_Application_Task_ਟੈਗ             MPU_v_Task_ਸੈੱਟ_Application_Task_ਟੈਗ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_Task_Get_Application_Task_ਟੈਗ             MPU_x_Task_Get_Application_Task_ਟੈਗ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_Task_ਸੈੱਟ_Thread_Local_Storage_ਸੂਚਕ      MPU_v_Task_ਸੈੱਟ_Thread_Local_Storage_ਸੂਚਕ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pv_Task_Get_Thread_Local_Storage_ਸੂਚਕ     MPU_pv_Task_Get_Thread_Local_Storage_ਸੂਚਕ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTaskCallApplicationTaskHook           MPU_xTaskCallApplicationTaskHook
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTaskGetIdleTaskHandle                 MPU_xTaskGetIdleTaskHandle
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ uxTaskGetSystemState                   MPU_uxTaskGetSystemState
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ vTaskList                              MPU_vTaskList
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_Task_Get_ਲੜੀਵਾਰ_ਸਮਾਂ_Stats                   MPU_v_Task_Get_ਲੜੀਵਾਰ_ਸਮਾਂ_Stats
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ul_Task_Get_Idle_ਲੜੀਵਾਰ_ਸਮਾਂ_Counter            MPU_ul_Task_Get_Idle_ਲੜੀਵਾਰ_ਸਮਾਂ_Counter
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ul_Task_Get_Idle_ਲੜੀਵਾਰ_ਸਮਾਂ_Percent            MPU_ul_Task_Get_Idle_ਲੜੀਵਾਰ_ਸਮਾਂ_Percent
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTaskGenericNotify                     MPU_xTaskGenericNotify
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTaskGenericNotifyWait                 MPU_xTaskGenericNotifyWait
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ulTaskGenericNotifyTake                MPU_ulTaskGenericNotifyTake
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTaskGenericNotifyStateClear           MPU_xTaskGenericNotifyStateClear
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ul_Task_Generic_Notify_ਮੁੱਲ_Clear          MPU_ul_Task_Generic_Notify_ਮੁੱਲ_Clear
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_Task_Catch_ਉੱਪਰ_Ticks                      MPU_x_Task_Catch_ਉੱਪਰ_Ticks

        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTaskGetCurrentTaskHandle              MPU_xTaskGetCurrentTaskHandle
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_Task_ਸੈੱਟ_ਸਮਾਂ_ਬਾਹਰਲਾ_State                   MPU_v_Task_ਸੈੱਟ_ਸਮਾਂ_ਬਾਹਰਲਾ_State
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_Task_Check_For_ਸਮਾਂ_ਬਾਹਰਲਾ                   MPU_x_Task_Check_For_ਸਮਾਂ_ਬਾਹਰਲਾ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTaskGetSchedulerState                 MPU_xTaskGetSchedulerState

/* Map standard queue.h API functions to the MPU equivalents. */
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Generic_Send                      MPU_x_ਕਤਾਰ_Generic_Send
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Receive                          MPU_x_ਕਤਾਰ_Receive
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Peek                             MPU_x_ਕਤਾਰ_Peek
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Semaphore_Take                    MPU_x_ਕਤਾਰ_Semaphore_Take
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ux_ਕਤਾਰ_Messages_ਉਡੀਕ                 MPU_ux_ਕਤਾਰ_Messages_ਉਡੀਕ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ux_ਕਤਾਰ_Spaces_ਉਪਲਬਧ                 MPU_ux_ਕਤਾਰ_Spaces_ਉਪਲਬਧ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_ਕਤਾਰ_Delete                           MPU_v_ਕਤਾਰ_Delete
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Create_Mutex                      MPU_x_ਕਤਾਰ_Create_Mutex
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Create_Mutex_Static                MPU_x_ਕਤਾਰ_Create_Mutex_Static
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Create_Counting_Semaphore          MPU_x_ਕਤਾਰ_Create_Counting_Semaphore
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Create_Counting_Semaphore_Static    MPU_x_ਕਤਾਰ_Create_Counting_Semaphore_Static
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Get_Mutex_Holder                   MPU_x_ਕਤਾਰ_Get_Mutex_Holder
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Take_Mutex_Recursive               MPU_x_ਕਤਾਰ_Take_Mutex_Recursive
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Give_Mutex_Recursive               MPU_x_ਕਤਾਰ_Give_Mutex_Recursive
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Generic_Create                    MPU_x_ਕਤਾਰ_Generic_Create
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Generic_Create_Static              MPU_x_ਕਤਾਰ_Generic_Create_Static
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Create_ਸੈੱਟ                        MPU_x_ਕਤਾਰ_Create_ਸੈੱਟ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_ਜੋੜੋ_ਮੰਜ਼ਿਲ_ਸੈੱਟ                         MPU_x_ਕਤਾਰ_ਜੋੜੋ_ਮੰਜ਼ਿਲ_ਸੈੱਟ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Remove_From_ਸੈੱਟ                    MPU_x_ਕਤਾਰ_Remove_From_ਸੈੱਟ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Select_From_ਸੈੱਟ                    MPU_x_ਕਤਾਰ_Select_From_ਸੈੱਟ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Generic_Reset                     MPU_x_ਕਤਾਰ_Generic_Reset

        #ਜੇ ( config_ਕਤਾਰ_REGISTRY_ਮਾਪ > 0 )
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_ਕਤਾਰ_ਜੋੜੋ_ਮੰਜ਼ਿਲ_Registry                MPU_v_ਕਤਾਰ_ਜੋੜੋ_ਮੰਜ਼ਿਲ_Registry
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_ਕਤਾਰ_Unregister_ਕਤਾਰ              MPU_v_ਕਤਾਰ_Unregister_ਕਤਾਰ
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pc_ਕਤਾਰ_Get_Name                     MPU_pc_ਕਤਾਰ_Get_Name
        #ਜੇ_ਅੰਤ

/* Map standard timer.h API functions to the MPU equivalents. */
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pvTimerGetTimerID                      MPU_pvTimerGetTimerID
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_Timer_ਸੈੱਟ_Timer_ID                       MPU_v_Timer_ਸੈੱਟ_Timer_ID
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_Timer_Is_Timer_ਸਰਗਰਮ                    MPU_x_Timer_Is_Timer_ਸਰਗਰਮ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTimerGetTimerDaemonTaskHandle         MPU_xTimerGetTimerDaemonTaskHandle
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pcTimerGetName                         MPU_pcTimerGetName
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_Timer_ਸੈੱਟ_Reload_ਬਹੁਲਕ                    MPU_v_Timer_ਸੈੱਟ_Reload_ਬਹੁਲਕ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ux_Timer_Get_Reload_ਬਹੁਲਕ                   MPU_ux_Timer_Get_Reload_ਬਹੁਲਕ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTimerGetPeriod                        MPU_xTimerGetPeriod
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_Timer_Get_Expiry_ਸਮਾਂ                    MPU_x_Timer_Get_Expiry_ਸਮਾਂ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTimerGenericCommand                   MPU_xTimerGenericCommand

/* Map standard event_group.h API functions to the MPU equivalents. */
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_Event_ਸਮੂਹ_Create                      MPU_x_Event_ਸਮੂਹ_Create
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_Event_ਸਮੂਹ_Create_Static                MPU_x_Event_ਸਮੂਹ_Create_Static
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_Event_ਸਮੂਹ_Wait_ਬਿੱਟ                    MPU_x_Event_ਸਮੂਹ_Wait_ਬਿੱਟ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_Event_ਸਮੂਹ_Clear_ਬਿੱਟ                   MPU_x_Event_ਸਮੂਹ_Clear_ਬਿੱਟ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ                     MPU_x_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_Event_ਸਮੂਹ_Sync                        MPU_x_Event_ਸਮੂਹ_Sync
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_Event_ਸਮੂਹ_Delete                      MPU_v_Event_ਸਮੂਹ_Delete

/* Map standard message/stream_buffer.h API functions to the MPU
 * equivalents. */
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਧਾਰਾ_ਬਫਰ_Send                      MPU_x_ਧਾਰਾ_ਬਫਰ_Send
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਧਾਰਾ_ਬਫਰ_Receive                   MPU_x_ਧਾਰਾ_ਬਫਰ_Receive
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਧਾਰਾ_ਬਫਰ_ਅਗਲਾ_ਸੁਨੇਹਾ_ਲੰਮਾਈ_ਬਾਈਟਾਂ    MPU_x_ਧਾਰਾ_ਬਫਰ_ਅਗਲਾ_ਸੁਨੇਹਾ_ਲੰਮਾਈ_ਬਾਈਟਾਂ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_ਧਾਰਾ_ਬਫਰ_Delete                    MPU_v_ਧਾਰਾ_ਬਫਰ_Delete
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਧਾਰਾ_ਬਫਰ_Is_ਭਰਿਆ                    MPU_x_ਧਾਰਾ_ਬਫਰ_Is_ਭਰਿਆ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਧਾਰਾ_ਬਫਰ_Is_ਖਾਲੀ_ਥਾਵਾਂ                   MPU_x_ਧਾਰਾ_ਬਫਰ_Is_ਖਾਲੀ_ਥਾਵਾਂ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਧਾਰਾ_ਬਫਰ_Reset                     MPU_x_ਧਾਰਾ_ਬਫਰ_Reset
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਧਾਰਾ_ਬਫਰ_Spaces_ਉਪਲਬਧ           MPU_x_ਧਾਰਾ_ਬਫਰ_Spaces_ਉਪਲਬਧ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਧਾਰਾ_ਬਫਰ_ਬਾਈਟਾਂ_ਉਪਲਬਧ            MPU_x_ਧਾਰਾ_ਬਫਰ_ਬਾਈਟਾਂ_ਉਪਲਬਧ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਧਾਰਾ_ਬਫਰ_ਸੈੱਟ_Trigger_Level           MPU_x_ਧਾਰਾ_ਬਫਰ_ਸੈੱਟ_Trigger_Level
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਧਾਰਾ_ਬਫਰ_Generic_Create             MPU_x_ਧਾਰਾ_ਬਫਰ_Generic_Create
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਧਾਰਾ_ਬਫਰ_Generic_Create_Static       MPU_x_ਧਾਰਾ_ਬਫਰ_Generic_Create_Static


/* Remove the privileged function macro, but keep the PRIVILEGED_DATA
 * macro so applications can place data in privileged access sections
 * (useful when using statically allocated objects). */
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PRIVILEGED_FUNCTION
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PRIVILEGED_ਅੰਕੜੇ    __attribute__( ( section( "privileged_data" ) ) )
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FREERTOS_SYSTEM_CALL

    #ਨਹੀਂਤਾਂ /* MPU_WRAPPERS_INCLUDED_FROM_API_FILE */

/* Ensure API functions go in the privileged execution section. */
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PRIVILEGED_FUNCTION     __attribute__( ( section( "privileged_functions" ) ) )
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PRIVILEGED_ਅੰਕੜੇ         __attribute__( ( section( "privileged_data" ) ) )
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FREERTOS_SYSTEM_CALL    __attribute__( ( section( "freertos_system_calls" ) ) )

    #ਜੇ_ਅੰਤ /* MPU_WRAPPERS_INCLUDED_FROM_API_FILE */

#ਨਹੀਂਤਾਂ /* portUSING_MPU_WRAPPERS */

    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PRIVILEGED_FUNCTION
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PRIVILEGED_ਅੰਕੜੇ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FREERTOS_SYSTEM_CALL

#ਜੇ_ਅੰਤ /* portUSING_MPU_WRAPPERS */


#ਜੇ_ਅੰਤ /* MPU_WRAPPERS_H */
