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
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ vTaskDelay                            MPU_vTaskDelay
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTaskDelayUntil                       MPU_xTaskDelayUntil
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTaskAbortDelay                       MPU_xTaskAbortDelay
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ux_Task_ਤਰਜੀਹ_Get                     MPU_ux_Task_ਤਰਜੀਹ_Get
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ eTaskGetState                         MPU_eTaskGetState
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ vTaskGetInfo                          MPU_vTaskGetInfo
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ vTaskSuspend                          MPU_vTaskSuspend
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ vTaskResume                           MPU_vTaskResume
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_Task_Get_Tick_ਗਿਣਤੀ                     MPU_x_Task_Get_Tick_ਗਿਣਤੀ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ux_Task_Get_ਸੰਖਿਆ_ਦਾ_Tasks                MPU_ux_Task_Get_ਸੰਖਿਆ_ਦਾ_Tasks
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ux_Task_Get_ਢੇਰ_ਉੱਚਾ_Water_Mark           MPU_ux_Task_Get_ਢੇਰ_ਉੱਚਾ_Water_Mark
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ux_Task_Get_ਢੇਰ_ਉੱਚਾ_Water_Mark2          MPU_ux_Task_Get_ਢੇਰ_ਉੱਚਾ_Water_Mark2
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_Task_ਸੈੱਟ_Application_Task_ਟੈਗ            MPU_v_Task_ਸੈੱਟ_Application_Task_ਟੈਗ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_Task_Get_Application_Task_ਟੈਗ            MPU_x_Task_Get_Application_Task_ਟੈਗ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_Task_ਸੈੱਟ_Thread_Local_Storage_ਸੂਚਕ     MPU_v_Task_ਸੈੱਟ_Thread_Local_Storage_ਸੂਚਕ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pv_Task_Get_Thread_Local_Storage_ਸੂਚਕ    MPU_pv_Task_Get_Thread_Local_Storage_ਸੂਚਕ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTaskGetIdleTaskHandle                MPU_xTaskGetIdleTaskHandle
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ uxTaskGetSystemState                  MPU_uxTaskGetSystemState
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ul_Task_Get_Idle_ਲੜੀਵਾਰ_ਸਮਾਂ_Counter           MPU_ul_Task_Get_Idle_ਲੜੀਵਾਰ_ਸਮਾਂ_Counter
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ul_Task_Get_Idle_ਲੜੀਵਾਰ_ਸਮਾਂ_Percent           MPU_ul_Task_Get_Idle_ਲੜੀਵਾਰ_ਸਮਾਂ_Percent
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTaskGenericNotify                    MPU_xTaskGenericNotify
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTaskGenericNotifyWait                MPU_xTaskGenericNotifyWait
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ulTaskGenericNotifyTake               MPU_ulTaskGenericNotifyTake
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTaskGenericNotifyStateClear          MPU_xTaskGenericNotifyStateClear
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ul_Task_Generic_Notify_ਮੁੱਲ_Clear         MPU_ul_Task_Generic_Notify_ਮੁੱਲ_Clear
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_Task_ਸੈੱਟ_ਸਮਾਂ_ਬਾਹਰਲਾ_State                  MPU_v_Task_ਸੈੱਟ_ਸਮਾਂ_ਬਾਹਰਲਾ_State
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_Task_Check_For_ਸਮਾਂ_ਬਾਹਰਲਾ                  MPU_x_Task_Check_For_ਸਮਾਂ_ਬਾਹਰਲਾ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTaskGetCurrentTaskHandle             MPU_xTaskGetCurrentTaskHandle
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTaskGetSchedulerState                MPU_xTaskGetSchedulerState

        #ਜੇ ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ul_Task_Get_ਲੜੀਵਾਰ_ਸਮਾਂ_Counter           MPU_ul_Task_Get_ਲੜੀਵਾਰ_ਸਮਾਂ_Counter
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ul_Task_Get_ਲੜੀਵਾਰ_ਸਮਾਂ_Percent           MPU_ul_Task_Get_ਲੜੀਵਾਰ_ਸਮਾਂ_Percent
        #ਜੇ_ਅੰਤ /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Privileged only wrappers for Task APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTaskCreate                              MPU_xTaskCreate
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTaskCreateStatic                        MPU_xTaskCreateStatic
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ vTaskDelete                              MPU_vTaskDelete
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_Task_ਤਰਜੀਹ_ਸੈੱਟ                         MPU_v_Task_ਤਰਜੀਹ_ਸੈੱਟ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTaskGetHandle                           MPU_xTaskGetHandle
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTaskCallApplicationTaskHook             MPU_xTaskCallApplicationTaskHook

        #ਜੇ ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pcTaskGetName                        MPU_pcTaskGetName
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTaskCreateRestricted                MPU_xTaskCreateRestricted
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTaskCreateRestrictedStatic          MPU_xTaskCreateRestrictedStatic
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ vTaskAllocateMPURegions              MPU_vTaskAllocateMPURegions
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTaskGetStaticBuffers                MPU_xTaskGetStaticBuffers
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ux_Task_ਤਰਜੀਹ_Get_From_ISR             MPU_ux_Task_ਤਰਜੀਹ_Get_From_ISR
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ux_Task_ਅਧਾਰ_ਤਰਜੀਹ_Get                MPU_ux_Task_ਅਧਾਰ_ਤਰਜੀਹ_Get
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ux_Task_ਅਧਾਰ_ਤਰਜੀਹ_Get_From_ISR         MPU_ux_Task_ਅਧਾਰ_ਤਰਜੀਹ_Get_From_ISR
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTaskResumeFromISR                   MPU_xTaskResumeFromISR
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_Task_Get_Application_Task_ਟੈਗ_From_ISR    MPU_x_Task_Get_Application_Task_ਟੈਗ_From_ISR
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTaskGenericNotifyFromISR            MPU_xTaskGenericNotifyFromISR
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ vTaskGenericNotifyGiveFromISR        MPU_vTaskGenericNotifyGiveFromISR
        #ਜੇ_ਅੰਤ /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Map standard queue.h API functions to the MPU equivalents. */
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Generic_Send            MPU_x_ਕਤਾਰ_Generic_Send
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Receive                MPU_x_ਕਤਾਰ_Receive
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Peek                   MPU_x_ਕਤਾਰ_Peek
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Semaphore_Take          MPU_x_ਕਤਾਰ_Semaphore_Take
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ux_ਕਤਾਰ_Messages_ਉਡੀਕ       MPU_ux_ਕਤਾਰ_Messages_ਉਡੀਕ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ux_ਕਤਾਰ_Spaces_ਉਪਲਬਧ       MPU_ux_ਕਤਾਰ_Spaces_ਉਪਲਬਧ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Get_Mutex_Holder         MPU_x_ਕਤਾਰ_Get_Mutex_Holder
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Take_Mutex_Recursive     MPU_x_ਕਤਾਰ_Take_Mutex_Recursive
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Give_Mutex_Recursive     MPU_x_ਕਤਾਰ_Give_Mutex_Recursive
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_ਜੋੜੋ_ਮੰਜ਼ਿਲ_ਸੈੱਟ               MPU_x_ਕਤਾਰ_ਜੋੜੋ_ਮੰਜ਼ਿਲ_ਸੈੱਟ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Select_From_ਸੈੱਟ          MPU_x_ਕਤਾਰ_Select_From_ਸੈੱਟ

        #ਜੇ ( config_ਕਤਾਰ_REGISTRY_ਮਾਪ > 0 )
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_ਕਤਾਰ_ਜੋੜੋ_ਮੰਜ਼ਿਲ_Registry      MPU_v_ਕਤਾਰ_ਜੋੜੋ_ਮੰਜ਼ਿਲ_Registry
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_ਕਤਾਰ_Unregister_ਕਤਾਰ    MPU_v_ਕਤਾਰ_Unregister_ਕਤਾਰ
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pc_ਕਤਾਰ_Get_Name           MPU_pc_ਕਤਾਰ_Get_Name
        #ਜੇ_ਅੰਤ /* #if ( configQUEUE_REGISTRY_SIZE > 0 ) */

/* Privileged only wrappers for Queue APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_ਕਤਾਰ_Delete                           MPU_v_ਕਤਾਰ_Delete
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Create_Mutex                      MPU_x_ਕਤਾਰ_Create_Mutex
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Create_Mutex_Static                MPU_x_ਕਤਾਰ_Create_Mutex_Static
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Create_Counting_Semaphore          MPU_x_ਕਤਾਰ_Create_Counting_Semaphore
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Create_Counting_Semaphore_Static    MPU_x_ਕਤਾਰ_Create_Counting_Semaphore_Static
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Generic_Create                    MPU_x_ਕਤਾਰ_Generic_Create
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Generic_Create_Static              MPU_x_ਕਤਾਰ_Generic_Create_Static
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Generic_Reset                     MPU_x_ਕਤਾਰ_Generic_Reset
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Create_ਸੈੱਟ                        MPU_x_ਕਤਾਰ_Create_ਸੈੱਟ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Remove_From_ਸੈੱਟ                    MPU_x_ਕਤਾਰ_Remove_From_ਸੈੱਟ

        #ਜੇ ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Generic_Get_Static_Buffers      MPU_x_ਕਤਾਰ_Generic_Get_Static_Buffers
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Generic_Send_From_ISR           MPU_x_ਕਤਾਰ_Generic_Send_From_ISR
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Give_From_ISR                  MPU_x_ਕਤਾਰ_Give_From_ISR
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Peek_From_ISR                  MPU_x_ਕਤਾਰ_Peek_From_ISR
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Receive_From_ISR               MPU_x_ਕਤਾਰ_Receive_From_ISR
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Is_ਕਤਾਰ_ਖਾਲੀ_ਥਾਵਾਂ_From_ISR          MPU_x_ਕਤਾਰ_Is_ਕਤਾਰ_ਖਾਲੀ_ਥਾਵਾਂ_From_ISR
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Is_ਕਤਾਰ_ਭਰਿਆ_From_ISR           MPU_x_ਕਤਾਰ_Is_ਕਤਾਰ_ਭਰਿਆ_From_ISR
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ux_ਕਤਾਰ_Messages_ਉਡੀਕ_From_ISR      MPU_ux_ਕਤਾਰ_Messages_ਉਡੀਕ_From_ISR
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Get_Mutex_Holder_From_ISR        MPU_x_ਕਤਾਰ_Get_Mutex_Holder_From_ISR
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਕਤਾਰ_Select_From_ਸੈੱਟ_From_ISR         MPU_x_ਕਤਾਰ_Select_From_ਸੈੱਟ_From_ISR
        #ਜੇ_ਅੰਤ /* if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Map standard timer.h API functions to the MPU equivalents. */
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pvTimerGetTimerID                 MPU_pvTimerGetTimerID
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_Timer_ਸੈੱਟ_Timer_ID                  MPU_v_Timer_ਸੈੱਟ_Timer_ID
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_Timer_Is_Timer_ਸਰਗਰਮ               MPU_x_Timer_Is_Timer_ਸਰਗਰਮ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTimerGetTimerDaemonTaskHandle    MPU_xTimerGetTimerDaemonTaskHandle
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTimerGenericCommandFromTask      MPU_xTimerGenericCommandFromTask
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pcTimerGetName                    MPU_pcTimerGetName
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_Timer_ਸੈੱਟ_Reload_ਬਹੁਲਕ               MPU_v_Timer_ਸੈੱਟ_Reload_ਬਹੁਲਕ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ux_Timer_Get_Reload_ਬਹੁਲਕ              MPU_ux_Timer_Get_Reload_ਬਹੁਲਕ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTimerGetPeriod                   MPU_xTimerGetPeriod
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_Timer_Get_Expiry_ਸਮਾਂ               MPU_x_Timer_Get_Expiry_ਸਮਾਂ

/* Privileged only wrappers for Timer APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
        #ਜੇ ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_Timer_Get_Reload_ਬਹੁਲਕ            MPU_x_Timer_Get_Reload_ਬਹੁਲਕ
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTimerCreate                   MPU_xTimerCreate
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTimerCreateStatic             MPU_xTimerCreateStatic
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_Timer_Get_Static_ਬਫਰ          MPU_x_Timer_Get_Static_ਬਫਰ
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ xTimerGenericCommandFromISR    MPU_xTimerGenericCommandFromISR
        #ਜੇ_ਅੰਤ /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Map standard event_group.h API functions to the MPU equivalents. */
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_Event_ਸਮੂਹ_Wait_ਬਿੱਟ          MPU_x_Event_ਸਮੂਹ_Wait_ਬਿੱਟ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_Event_ਸਮੂਹ_Clear_ਬਿੱਟ         MPU_x_Event_ਸਮੂਹ_Clear_ਬਿੱਟ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ           MPU_x_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_Event_ਸਮੂਹ_Sync              MPU_x_Event_ਸਮੂਹ_Sync

        #ਜੇ ( ( configUSE_TRACE_FACILITY == 1 ) && ( configUSE_MPU_WRAPPERS_V1 == 0 ) )
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ux_Event_ਸਮੂਹ_Get_ਸੰਖਿਆ    MPU_ux_Event_ਸਮੂਹ_Get_ਸੰਖਿਆ
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_Event_ਸਮੂਹ_ਸੈੱਟ_ਸੰਖਿਆ     MPU_v_Event_ਸਮੂਹ_ਸੈੱਟ_ਸੰਖਿਆ
        #ਜੇ_ਅੰਤ /* #if ( ( configUSE_TRACE_FACILITY == 1 ) && ( configUSE_MPU_WRAPPERS_V1 == 0 ) ) */

/* Privileged only wrappers for Event Group APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_Event_ਸਮੂਹ_Create                  MPU_x_Event_ਸਮੂਹ_Create
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_Event_ਸਮੂਹ_Create_Static            MPU_x_Event_ਸਮੂਹ_Create_Static
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_Event_ਸਮੂਹ_Delete                  MPU_v_Event_ਸਮੂਹ_Delete

        #ਜੇ ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_Event_ਸਮੂਹ_Get_Static_ਬਫਰ     MPU_x_Event_ਸਮੂਹ_Get_Static_ਬਫਰ
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_Event_ਸਮੂਹ_Clear_ਬਿੱਟ_From_ISR    MPU_x_Event_ਸਮੂਹ_Clear_ਬਿੱਟ_From_ISR
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ_From_ISR      MPU_x_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ_From_ISR
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_Event_ਸਮੂਹ_Get_ਬਿੱਟ_From_ISR      MPU_x_Event_ਸਮੂਹ_Get_ਬਿੱਟ_From_ISR
        #ਜੇ_ਅੰਤ /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Map standard message/stream_buffer.h API functions to the MPU
 * equivalents. */
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਧਾਰਾ_ਬਫਰ_Send                      MPU_x_ਧਾਰਾ_ਬਫਰ_Send
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਧਾਰਾ_ਬਫਰ_Receive                   MPU_x_ਧਾਰਾ_ਬਫਰ_Receive
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਧਾਰਾ_ਬਫਰ_Is_ਭਰਿਆ                    MPU_x_ਧਾਰਾ_ਬਫਰ_Is_ਭਰਿਆ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਧਾਰਾ_ਬਫਰ_Is_ਖਾਲੀ_ਥਾਵਾਂ                   MPU_x_ਧਾਰਾ_ਬਫਰ_Is_ਖਾਲੀ_ਥਾਵਾਂ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਧਾਰਾ_ਬਫਰ_Spaces_ਉਪਲਬਧ           MPU_x_ਧਾਰਾ_ਬਫਰ_Spaces_ਉਪਲਬਧ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਧਾਰਾ_ਬਫਰ_ਬਾਈਟਾਂ_ਉਪਲਬਧ            MPU_x_ਧਾਰਾ_ਬਫਰ_ਬਾਈਟਾਂ_ਉਪਲਬਧ
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਧਾਰਾ_ਬਫਰ_ਸੈੱਟ_Trigger_Level           MPU_x_ਧਾਰਾ_ਬਫਰ_ਸੈੱਟ_Trigger_Level
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਧਾਰਾ_ਬਫਰ_ਅਗਲਾ_ਸੁਨੇਹਾ_ਲੰਮਾਈ_ਬਾਈਟਾਂ    MPU_x_ਧਾਰਾ_ਬਫਰ_ਅਗਲਾ_ਸੁਨੇਹਾ_ਲੰਮਾਈ_ਬਾਈਟਾਂ

/* Privileged only wrappers for Stream Buffer APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */

        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਧਾਰਾ_ਬਫਰ_Generic_Create                  MPU_x_ਧਾਰਾ_ਬਫਰ_Generic_Create
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਧਾਰਾ_ਬਫਰ_Generic_Create_Static            MPU_x_ਧਾਰਾ_ਬਫਰ_Generic_Create_Static
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_ਧਾਰਾ_ਬਫਰ_Delete                         MPU_v_ਧਾਰਾ_ਬਫਰ_Delete
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਧਾਰਾ_ਬਫਰ_Reset                          MPU_x_ਧਾਰਾ_ਬਫਰ_Reset

        #ਜੇ ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਧਾਰਾ_ਬਫਰ_Get_Static_Buffers           MPU_x_ਧਾਰਾ_ਬਫਰ_Get_Static_Buffers
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਧਾਰਾ_ਬਫਰ_Send_From_ISR                MPU_x_ਧਾਰਾ_ਬਫਰ_Send_From_ISR
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਧਾਰਾ_ਬਫਰ_Receive_From_ISR             MPU_x_ਧਾਰਾ_ਬਫਰ_Receive_From_ISR
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਧਾਰਾ_ਬਫਰ_Send_Completed_From_ISR       MPU_x_ਧਾਰਾ_ਬਫਰ_Send_Completed_From_ISR
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਧਾਰਾ_ਬਫਰ_Receive_Completed_From_ISR    MPU_x_ਧਾਰਾ_ਬਫਰ_Receive_Completed_From_ISR
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ x_ਧਾਰਾ_ਬਫਰ_Reset_From_ISR               MPU_x_ਧਾਰਾ_ਬਫਰ_Reset_From_ISR
        #ਜੇ_ਅੰਤ /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

        #ਜੇ ( ( configUSE_MPU_WRAPPERS_V1 == 0 ) && ( configENABLE_ACCESS_CONTROL_LIST == 1 ) )

            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_Grant_Access_ਮੰਜ਼ਿਲ_Task( xTask, x_Task_ਮੰਜ਼ਿਲ_Grant_Access )                        v_Grant_Access_ਮੰਜ਼ਿਲ_Kernel_Object( ( xTask ), ( int32_t ) ( x_Task_ਮੰਜ਼ਿਲ_Grant_Access ) )
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_Revoke_Access_ਮੰਜ਼ਿਲ_Task( xTask, x_Task_ਮੰਜ਼ਿਲ_Revoke_Access )                      v_Revoke_Access_ਮੰਜ਼ਿਲ_Kernel_Object( ( xTask ), ( int32_t ) ( x_Task_ਮੰਜ਼ਿਲ_Revoke_Access ) )

            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_Grant_Access_ਮੰਜ਼ਿਲ_Semaphore( xTask, x_Semaphore_ਮੰਜ਼ਿਲ_Grant_Access )              v_Grant_Access_ਮੰਜ਼ਿਲ_Kernel_Object( ( xTask ), ( int32_t ) ( x_Semaphore_ਮੰਜ਼ਿਲ_Grant_Access ) )
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_Revoke_Access_ਮੰਜ਼ਿਲ_Semaphore( xTask, x_Semaphore_ਮੰਜ਼ਿਲ_Revoke_Access )            v_Revoke_Access_ਮੰਜ਼ਿਲ_Kernel_Object( ( xTask ), ( int32_t ) ( x_Semaphore_ਮੰਜ਼ਿਲ_Revoke_Access ) )

            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_Grant_Access_ਮੰਜ਼ਿਲ_ਕਤਾਰ( xTask, x_ਕਤਾਰ_ਮੰਜ਼ਿਲ_Grant_Access )                      v_Grant_Access_ਮੰਜ਼ਿਲ_Kernel_Object( ( xTask ), ( int32_t ) ( x_ਕਤਾਰ_ਮੰਜ਼ਿਲ_Grant_Access ) )
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_Revoke_Access_ਮੰਜ਼ਿਲ_ਕਤਾਰ( xTask, x_ਕਤਾਰ_ਮੰਜ਼ਿਲ_Revoke_Access )                    v_Revoke_Access_ਮੰਜ਼ਿਲ_Kernel_Object( ( xTask ), ( int32_t ) ( x_ਕਤਾਰ_ਮੰਜ਼ਿਲ_Revoke_Access ) )

            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_Grant_Access_ਮੰਜ਼ਿਲ_ਕਤਾਰ_ਸੈੱਟ( xTask, x_ਕਤਾਰ_ਸੈੱਟ_ਮੰਜ਼ਿਲ_Grant_Access )                v_Grant_Access_ਮੰਜ਼ਿਲ_Kernel_Object( ( xTask ), ( int32_t ) ( x_ਕਤਾਰ_ਸੈੱਟ_ਮੰਜ਼ਿਲ_Grant_Access ) )
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_Revoke_Access_ਮੰਜ਼ਿਲ_ਕਤਾਰ_ਸੈੱਟ( xTask, x_ਕਤਾਰ_ਸੈੱਟ_ਮੰਜ਼ਿਲ_Revoke_Access )              v_Revoke_Access_ਮੰਜ਼ਿਲ_Kernel_Object( ( xTask ), ( int32_t ) ( x_ਕਤਾਰ_ਸੈੱਟ_ਮੰਜ਼ਿਲ_Revoke_Access ) )

            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_Grant_Access_ਮੰਜ਼ਿਲ_Event_ਸਮੂਹ( xTask, x_Event_ਸਮੂਹ_ਮੰਜ਼ਿਲ_Grant_Access )            v_Grant_Access_ਮੰਜ਼ਿਲ_Kernel_Object( ( xTask ), ( int32_t ) ( x_Event_ਸਮੂਹ_ਮੰਜ਼ਿਲ_Grant_Access ) )
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_Revoke_Access_ਮੰਜ਼ਿਲ_Event_ਸਮੂਹ( xTask, x_Event_ਸਮੂਹ_ਮੰਜ਼ਿਲ_Revoke_Access )          v_Revoke_Access_ਮੰਜ਼ਿਲ_Kernel_Object( ( xTask ), ( int32_t ) ( x_Event_ਸਮੂਹ_ਮੰਜ਼ਿਲ_Revoke_Access ) )

            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_Grant_Access_ਮੰਜ਼ਿਲ_ਧਾਰਾ_ਬਫਰ( xTask, x_ਧਾਰਾ_ਬਫਰ_ਮੰਜ਼ਿਲ_Grant_Access )        v_Grant_Access_ਮੰਜ਼ਿਲ_Kernel_Object( ( xTask ), ( int32_t ) ( x_ਧਾਰਾ_ਬਫਰ_ਮੰਜ਼ਿਲ_Grant_Access ) )
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_Revoke_Access_ਮੰਜ਼ਿਲ_ਧਾਰਾ_ਬਫਰ( xTask, x_ਧਾਰਾ_ਬਫਰ_ਮੰਜ਼ਿਲ_Revoke_Access )      v_Revoke_Access_ਮੰਜ਼ਿਲ_Kernel_Object( ( xTask ), ( int32_t ) ( x_ਧਾਰਾ_ਬਫਰ_ਮੰਜ਼ਿਲ_Revoke_Access ) )

            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_Grant_Access_ਮੰਜ਼ਿਲ_ਸੁਨੇਹਾ_ਬਫਰ( xTask, x_ਸੁਨੇਹਾ_ਬਫਰ_ਮੰਜ਼ਿਲ_Grant_Access )      v_Grant_Access_ਮੰਜ਼ਿਲ_Kernel_Object( ( xTask ), ( int32_t ) ( x_ਸੁਨੇਹਾ_ਬਫਰ_ਮੰਜ਼ਿਲ_Grant_Access ) )
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_Revoke_Access_ਮੰਜ਼ਿਲ_ਸੁਨੇਹਾ_ਬਫਰ( xTask, x_ਸੁਨੇਹਾ_ਬਫਰ_ਮੰਜ਼ਿਲ_Revoke_Access )    v_Revoke_Access_ਮੰਜ਼ਿਲ_Kernel_Object( ( xTask ), ( int32_t ) ( x_ਸੁਨੇਹਾ_ਬਫਰ_ਮੰਜ਼ਿਲ_Revoke_Access ) )

            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_Grant_Access_ਮੰਜ਼ਿਲ_Timer( xTask, x_Timer_ਮੰਜ਼ਿਲ_Grant_Access )                      v_Grant_Access_ਮੰਜ਼ਿਲ_Kernel_Object( ( xTask ), ( int32_t ) ( x_Timer_ਮੰਜ਼ਿਲ_Grant_Access ) )
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_Revoke_Access_ਮੰਜ਼ਿਲ_Timer( xTask, x_Timer_ਮੰਜ਼ਿਲ_Revoke_Access )                    v_Revoke_Access_ਮੰਜ਼ਿਲ_Kernel_Object( ( xTask ), ( int32_t ) ( x_Timer_ਮੰਜ਼ਿਲ_Revoke_Access ) )

        #ਜੇ_ਅੰਤ /* #if ( ( configUSE_MPU_WRAPPERS_V1 == 0 ) && ( configENABLE_ACCESS_CONTROL_LIST == 1 ) ) */

    #ਜੇ_ਅੰਤ /* MPU_WRAPPERS_INCLUDED_FROM_API_FILE */

    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PRIVILEGED_FUNCTION     __attribute__( ( section( "privileged_functions" ) ) )
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PRIVILEGED_ਅੰਕੜੇ         __attribute__( ( section( "privileged_data" ) ) )
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FREERTOS_SYSTEM_CALL    __attribute__( ( section( "freertos_system_calls" ) ) )

#ਨਹੀਂਤਾਂ /* portUSING_MPU_WRAPPERS */

    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PRIVILEGED_FUNCTION
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PRIVILEGED_ਅੰਕੜੇ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FREERTOS_SYSTEM_CALL

#ਜੇ_ਅੰਤ /* portUSING_MPU_WRAPPERS */


#ਜੇ_ਅੰਤ /* MPU_WRAPPERS_H */
