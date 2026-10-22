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

#که_نه_تعریف_شوی MPU_WRAPPERS_H
#تعریف_کړه MPU_WRAPPERS_H

/* This file redefines API functions to be called through a wrapper macro, but
 * only for ports that are using the MPU. */
#إذا ( پورټ_USING_MPU_WRAPPERS == 1 )

/* MPU_WRAPPERS_INCLUDED_FROM_API_FILE will be defined when this file is
 * included from queue.c or task.c to prevent it from having an effect within
 * those files. */
    #که_نه_تعریف_شوی MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/*
 * Map standard (non MPU) API functions to equivalents that start
 * "MPU_".  This will cause the application code to call the MPU_
 * version, which wraps the non-MPU version with privilege promoting
 * then demoting code, so the kernel code always runs will full
 * privileges.
 */

/* Map standard task.h API functions to the MPU equivalents. */
        #تعریف_کړه vTaskDelay                            MPU_vTaskDelay
        #تعریف_کړه xTaskDelayUntil                       MPU_xTaskDelayUntil
        #تعریف_کړه xTaskAbortDelay                       MPU_xTaskAbortDelay
        #تعریف_کړه ux_Task_لومړیتوب_Get                     MPU_ux_Task_لومړیتوب_Get
        #تعریف_کړه eTaskGetState                         MPU_eTaskGetState
        #تعریف_کړه vTaskGetInfo                          MPU_vTaskGetInfo
        #تعریف_کړه vTaskSuspend                          MPU_vTaskSuspend
        #تعریف_کړه vTaskResume                           MPU_vTaskResume
        #تعریف_کړه x_Task_Get_Tick_شمېر                     MPU_x_Task_Get_Tick_شمېر
        #تعریف_کړه ux_Task_Get_شمېره_د_Tasks                MPU_ux_Task_Get_شمېره_د_Tasks
        #تعریف_کړه ux_Task_Get_ډېری_لوړ_Water_Mark           MPU_ux_Task_Get_ډېری_لوړ_Water_Mark
        #تعریف_کړه ux_Task_Get_ډېری_لوړ_Water_Mark2          MPU_ux_Task_Get_ډېری_لوړ_Water_Mark2
        #تعریف_کړه v_Task_سېټ_Application_Task_نښان            MPU_v_Task_سېټ_Application_Task_نښان
        #تعریف_کړه x_Task_Get_Application_Task_نښان            MPU_x_Task_Get_Application_Task_نښان
        #تعریف_کړه v_Task_سېټ_Thread_Local_Storage_اشاره_ګر     MPU_v_Task_سېټ_Thread_Local_Storage_اشاره_ګر
        #تعریف_کړه pv_Task_Get_Thread_Local_Storage_اشاره_ګر    MPU_pv_Task_Get_Thread_Local_Storage_اشاره_ګر
        #تعریف_کړه xTaskGetIdleTaskHandle                MPU_xTaskGetIdleTaskHandle
        #تعریف_کړه uxTaskGetSystemState                  MPU_uxTaskGetSystemState
        #تعریف_کړه ul_Task_Get_Idle_پرلپسې_وخت_Counter           MPU_ul_Task_Get_Idle_پرلپسې_وخت_Counter
        #تعریف_کړه ul_Task_Get_Idle_پرلپسې_وخت_Percent           MPU_ul_Task_Get_Idle_پرلپسې_وخت_Percent
        #تعریف_کړه xTaskGenericNotify                    MPU_xTaskGenericNotify
        #تعریف_کړه xTaskGenericNotifyWait                MPU_xTaskGenericNotifyWait
        #تعریف_کړه ulTaskGenericNotifyTake               MPU_ulTaskGenericNotifyTake
        #تعریف_کړه xTaskGenericNotifyStateClear          MPU_xTaskGenericNotifyStateClear
        #تعریف_کړه ul_Task_Generic_Notify_ارزښت_Clear         MPU_ul_Task_Generic_Notify_ارزښت_Clear
        #تعریف_کړه v_Task_سېټ_وخت_بهر_State                  MPU_v_Task_سېټ_وخت_بهر_State
        #تعریف_کړه x_Task_Check_For_وخت_بهر                  MPU_x_Task_Check_For_وخت_بهر
        #تعریف_کړه xTaskGetCurrentTaskHandle             MPU_xTaskGetCurrentTaskHandle
        #تعریف_کړه xTaskGetSchedulerState                MPU_xTaskGetSchedulerState

        #إذا ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #تعریف_کړه ul_Task_Get_پرلپسې_وخت_Counter           MPU_ul_Task_Get_پرلپسې_وخت_Counter
            #تعریف_کړه ul_Task_Get_پرلپسې_وخت_Percent           MPU_ul_Task_Get_پرلپسې_وخت_Percent
        #د_که_پای /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Privileged only wrappers for Task APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
        #تعریف_کړه xTaskCreate                              MPU_xTaskCreate
        #تعریف_کړه xTaskCreateStatic                        MPU_xTaskCreateStatic
        #تعریف_کړه vTaskDelete                              MPU_vTaskDelete
        #تعریف_کړه v_Task_لومړیتوب_سېټ                         MPU_v_Task_لومړیتوب_سېټ
        #تعریف_کړه xTaskGetHandle                           MPU_xTaskGetHandle
        #تعریف_کړه xTaskCallApplicationTaskHook             MPU_xTaskCallApplicationTaskHook

        #إذا ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #تعریف_کړه pcTaskGetName                        MPU_pcTaskGetName
            #تعریف_کړه xTaskCreateRestricted                MPU_xTaskCreateRestricted
            #تعریف_کړه xTaskCreateRestrictedStatic          MPU_xTaskCreateRestrictedStatic
            #تعریف_کړه vTaskAllocateMPURegions              MPU_vTaskAllocateMPURegions
            #تعریف_کړه xTaskGetStaticBuffers                MPU_xTaskGetStaticBuffers
            #تعریف_کړه ux_Task_لومړیتوب_Get_From_ISR             MPU_ux_Task_لومړیتوب_Get_From_ISR
            #تعریف_کړه ux_Task_بنسټ_لومړیتوب_Get                MPU_ux_Task_بنسټ_لومړیتوب_Get
            #تعریف_کړه ux_Task_بنسټ_لومړیتوب_Get_From_ISR         MPU_ux_Task_بنسټ_لومړیتوب_Get_From_ISR
            #تعریف_کړه xTaskResumeFromISR                   MPU_xTaskResumeFromISR
            #تعریف_کړه x_Task_Get_Application_Task_نښان_From_ISR    MPU_x_Task_Get_Application_Task_نښان_From_ISR
            #تعریف_کړه xTaskGenericNotifyFromISR            MPU_xTaskGenericNotifyFromISR
            #تعریف_کړه vTaskGenericNotifyGiveFromISR        MPU_vTaskGenericNotifyGiveFromISR
        #د_که_پای /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Map standard queue.h API functions to the MPU equivalents. */
        #تعریف_کړه x_کتار_Generic_Send            MPU_x_کتار_Generic_Send
        #تعریف_کړه x_کتار_Receive                MPU_x_کتار_Receive
        #تعریف_کړه x_کتار_Peek                   MPU_x_کتار_Peek
        #تعریف_کړه x_کتار_Semaphore_Take          MPU_x_کتار_Semaphore_Take
        #تعریف_کړه ux_کتار_Messages_انتظار       MPU_ux_کتار_Messages_انتظار
        #تعریف_کړه ux_کتار_Spaces_شته       MPU_ux_کتار_Spaces_شته
        #تعریف_کړه x_کتار_Get_Mutex_Holder         MPU_x_کتار_Get_Mutex_Holder
        #تعریف_کړه x_کتار_Take_Mutex_Recursive     MPU_x_کتار_Take_Mutex_Recursive
        #تعریف_کړه x_کتار_Give_Mutex_Recursive     MPU_x_کتار_Give_Mutex_Recursive
        #تعریف_کړه x_کتار_جمع_کړه_منزل_سېټ               MPU_x_کتار_جمع_کړه_منزل_سېټ
        #تعریف_کړه x_کتار_Select_From_سېټ          MPU_x_کتار_Select_From_سېټ

        #إذا ( config_کتار_REGISTRY_اندازه > 0 )
            #تعریف_کړه v_کتار_جمع_کړه_منزل_Registry      MPU_v_کتار_جمع_کړه_منزل_Registry
            #تعریف_کړه v_کتار_Unregister_کتار    MPU_v_کتار_Unregister_کتار
            #تعریف_کړه pc_کتار_Get_Name           MPU_pc_کتار_Get_Name
        #د_که_پای /* #if ( configQUEUE_REGISTRY_SIZE > 0 ) */

/* Privileged only wrappers for Queue APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
        #تعریف_کړه v_کتار_Delete                           MPU_v_کتار_Delete
        #تعریف_کړه x_کتار_Create_Mutex                      MPU_x_کتار_Create_Mutex
        #تعریف_کړه x_کتار_Create_Mutex_Static                MPU_x_کتار_Create_Mutex_Static
        #تعریف_کړه x_کتار_Create_Counting_Semaphore          MPU_x_کتار_Create_Counting_Semaphore
        #تعریف_کړه x_کتار_Create_Counting_Semaphore_Static    MPU_x_کتار_Create_Counting_Semaphore_Static
        #تعریف_کړه x_کتار_Generic_Create                    MPU_x_کتار_Generic_Create
        #تعریف_کړه x_کتار_Generic_Create_Static              MPU_x_کتار_Generic_Create_Static
        #تعریف_کړه x_کتار_Generic_Reset                     MPU_x_کتار_Generic_Reset
        #تعریف_کړه x_کتار_Create_سېټ                        MPU_x_کتار_Create_سېټ
        #تعریف_کړه x_کتار_Remove_From_سېټ                    MPU_x_کتار_Remove_From_سېټ

        #إذا ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #تعریف_کړه x_کتار_Generic_Get_Static_Buffers      MPU_x_کتار_Generic_Get_Static_Buffers
            #تعریف_کړه x_کتار_Generic_Send_From_ISR           MPU_x_کتار_Generic_Send_From_ISR
            #تعریف_کړه x_کتار_Give_From_ISR                  MPU_x_کتار_Give_From_ISR
            #تعریف_کړه x_کتار_Peek_From_ISR                  MPU_x_کتار_Peek_From_ISR
            #تعریف_کړه x_کتار_Receive_From_ISR               MPU_x_کتار_Receive_From_ISR
            #تعریف_کړه x_کتار_Is_کتار_تشه_From_ISR          MPU_x_کتار_Is_کتار_تشه_From_ISR
            #تعریف_کړه x_کتار_Is_کتار_ډک_From_ISR           MPU_x_کتار_Is_کتار_ډک_From_ISR
            #تعریف_کړه ux_کتار_Messages_انتظار_From_ISR      MPU_ux_کتار_Messages_انتظار_From_ISR
            #تعریف_کړه x_کتار_Get_Mutex_Holder_From_ISR        MPU_x_کتار_Get_Mutex_Holder_From_ISR
            #تعریف_کړه x_کتار_Select_From_سېټ_From_ISR         MPU_x_کتار_Select_From_سېټ_From_ISR
        #د_که_پای /* if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Map standard timer.h API functions to the MPU equivalents. */
        #تعریف_کړه pvTimerGetTimerID                 MPU_pvTimerGetTimerID
        #تعریف_کړه v_Timer_سېټ_Timer_ID                  MPU_v_Timer_سېټ_Timer_ID
        #تعریف_کړه x_Timer_Is_Timer_فعال               MPU_x_Timer_Is_Timer_فعال
        #تعریف_کړه xTimerGetTimerDaemonTaskHandle    MPU_xTimerGetTimerDaemonTaskHandle
        #تعریف_کړه xTimerGenericCommandFromTask      MPU_xTimerGenericCommandFromTask
        #تعریف_کړه pcTimerGetName                    MPU_pcTimerGetName
        #تعریف_کړه v_Timer_سېټ_Reload_ډېرپېښ               MPU_v_Timer_سېټ_Reload_ډېرپېښ
        #تعریف_کړه ux_Timer_Get_Reload_ډېرپېښ              MPU_ux_Timer_Get_Reload_ډېرپېښ
        #تعریف_کړه xTimerGetPeriod                   MPU_xTimerGetPeriod
        #تعریف_کړه x_Timer_Get_Expiry_وخت               MPU_x_Timer_Get_Expiry_وخت

/* Privileged only wrappers for Timer APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
        #إذا ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #تعریف_کړه x_Timer_Get_Reload_ډېرپېښ            MPU_x_Timer_Get_Reload_ډېرپېښ
            #تعریف_کړه xTimerCreate                   MPU_xTimerCreate
            #تعریف_کړه xTimerCreateStatic             MPU_xTimerCreateStatic
            #تعریف_کړه x_Timer_Get_Static_بفر          MPU_x_Timer_Get_Static_بفر
            #تعریف_کړه xTimerGenericCommandFromISR    MPU_xTimerGenericCommandFromISR
        #د_که_پای /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Map standard event_group.h API functions to the MPU equivalents. */
        #تعریف_کړه x_Event_ډله_Wait_بټونه          MPU_x_Event_ډله_Wait_بټونه
        #تعریف_کړه x_Event_ډله_Clear_بټونه         MPU_x_Event_ډله_Clear_بټونه
        #تعریف_کړه x_Event_ډله_سېټ_بټونه           MPU_x_Event_ډله_سېټ_بټونه
        #تعریف_کړه x_Event_ډله_Sync              MPU_x_Event_ډله_Sync

        #إذا ( ( configUSE_TRACE_FACILITY == 1 ) && ( configUSE_MPU_WRAPPERS_V1 == 0 ) )
            #تعریف_کړه ux_Event_ډله_Get_شمېره    MPU_ux_Event_ډله_Get_شمېره
            #تعریف_کړه v_Event_ډله_سېټ_شمېره     MPU_v_Event_ډله_سېټ_شمېره
        #د_که_پای /* #if ( ( configUSE_TRACE_FACILITY == 1 ) && ( configUSE_MPU_WRAPPERS_V1 == 0 ) ) */

/* Privileged only wrappers for Event Group APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
        #تعریف_کړه x_Event_ډله_Create                  MPU_x_Event_ډله_Create
        #تعریف_کړه x_Event_ډله_Create_Static            MPU_x_Event_ډله_Create_Static
        #تعریف_کړه v_Event_ډله_Delete                  MPU_v_Event_ډله_Delete

        #إذا ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #تعریف_کړه x_Event_ډله_Get_Static_بفر     MPU_x_Event_ډله_Get_Static_بفر
            #تعریف_کړه x_Event_ډله_Clear_بټونه_From_ISR    MPU_x_Event_ډله_Clear_بټونه_From_ISR
            #تعریف_کړه x_Event_ډله_سېټ_بټونه_From_ISR      MPU_x_Event_ډله_سېټ_بټونه_From_ISR
            #تعریف_کړه x_Event_ډله_Get_بټونه_From_ISR      MPU_x_Event_ډله_Get_بټونه_From_ISR
        #د_که_پای /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Map standard message/stream_buffer.h API functions to the MPU
 * equivalents. */
        #تعریف_کړه x_بهیر_بفر_Send                      MPU_x_بهیر_بفر_Send
        #تعریف_کړه x_بهیر_بفر_Receive                   MPU_x_بهیر_بفر_Receive
        #تعریف_کړه x_بهیر_بفر_Is_ډک                    MPU_x_بهیر_بفر_Is_ډک
        #تعریف_کړه x_بهیر_بفر_Is_تشه                   MPU_x_بهیر_بفر_Is_تشه
        #تعریف_کړه x_بهیر_بفر_Spaces_شته           MPU_x_بهیر_بفر_Spaces_شته
        #تعریف_کړه x_بهیر_بفر_بایټونه_شته            MPU_x_بهیر_بفر_بایټونه_شته
        #تعریف_کړه x_بهیر_بفر_سېټ_Trigger_Level           MPU_x_بهیر_بفر_سېټ_Trigger_Level
        #تعریف_کړه x_بهیر_بفر_بل_پیغام_اوږدتیا_بایټونه    MPU_x_بهیر_بفر_بل_پیغام_اوږدتیا_بایټونه

/* Privileged only wrappers for Stream Buffer APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */

        #تعریف_کړه x_بهیر_بفر_Generic_Create                  MPU_x_بهیر_بفر_Generic_Create
        #تعریف_کړه x_بهیر_بفر_Generic_Create_Static            MPU_x_بهیر_بفر_Generic_Create_Static
        #تعریف_کړه v_بهیر_بفر_Delete                         MPU_v_بهیر_بفر_Delete
        #تعریف_کړه x_بهیر_بفر_Reset                          MPU_x_بهیر_بفر_Reset

        #إذا ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #تعریف_کړه x_بهیر_بفر_Get_Static_Buffers           MPU_x_بهیر_بفر_Get_Static_Buffers
            #تعریف_کړه x_بهیر_بفر_Send_From_ISR                MPU_x_بهیر_بفر_Send_From_ISR
            #تعریف_کړه x_بهیر_بفر_Receive_From_ISR             MPU_x_بهیر_بفر_Receive_From_ISR
            #تعریف_کړه x_بهیر_بفر_Send_Completed_From_ISR       MPU_x_بهیر_بفر_Send_Completed_From_ISR
            #تعریف_کړه x_بهیر_بفر_Receive_Completed_From_ISR    MPU_x_بهیر_بفر_Receive_Completed_From_ISR
            #تعریف_کړه x_بهیر_بفر_Reset_From_ISR               MPU_x_بهیر_بفر_Reset_From_ISR
        #د_که_پای /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

        #إذا ( ( configUSE_MPU_WRAPPERS_V1 == 0 ) && ( configENABLE_ACCESS_CONTROL_LIST == 1 ) )

            #تعریف_کړه v_Grant_Access_منزل_Task( xTask, x_Task_منزل_Grant_Access )                        v_Grant_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_Task_منزل_Grant_Access ) )
            #تعریف_کړه v_Revoke_Access_منزل_Task( xTask, x_Task_منزل_Revoke_Access )                      v_Revoke_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_Task_منزل_Revoke_Access ) )

            #تعریف_کړه v_Grant_Access_منزل_Semaphore( xTask, x_Semaphore_منزل_Grant_Access )              v_Grant_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_Semaphore_منزل_Grant_Access ) )
            #تعریف_کړه v_Revoke_Access_منزل_Semaphore( xTask, x_Semaphore_منزل_Revoke_Access )            v_Revoke_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_Semaphore_منزل_Revoke_Access ) )

            #تعریف_کړه v_Grant_Access_منزل_کتار( xTask, x_کتار_منزل_Grant_Access )                      v_Grant_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_کتار_منزل_Grant_Access ) )
            #تعریف_کړه v_Revoke_Access_منزل_کتار( xTask, x_کتار_منزل_Revoke_Access )                    v_Revoke_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_کتار_منزل_Revoke_Access ) )

            #تعریف_کړه v_Grant_Access_منزل_کتار_سېټ( xTask, x_کتار_سېټ_منزل_Grant_Access )                v_Grant_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_کتار_سېټ_منزل_Grant_Access ) )
            #تعریف_کړه v_Revoke_Access_منزل_کتار_سېټ( xTask, x_کتار_سېټ_منزل_Revoke_Access )              v_Revoke_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_کتار_سېټ_منزل_Revoke_Access ) )

            #تعریف_کړه v_Grant_Access_منزل_Event_ډله( xTask, x_Event_ډله_منزل_Grant_Access )            v_Grant_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_Event_ډله_منزل_Grant_Access ) )
            #تعریف_کړه v_Revoke_Access_منزل_Event_ډله( xTask, x_Event_ډله_منزل_Revoke_Access )          v_Revoke_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_Event_ډله_منزل_Revoke_Access ) )

            #تعریف_کړه v_Grant_Access_منزل_بهیر_بفر( xTask, x_بهیر_بفر_منزل_Grant_Access )        v_Grant_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_بهیر_بفر_منزل_Grant_Access ) )
            #تعریف_کړه v_Revoke_Access_منزل_بهیر_بفر( xTask, x_بهیر_بفر_منزل_Revoke_Access )      v_Revoke_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_بهیر_بفر_منزل_Revoke_Access ) )

            #تعریف_کړه v_Grant_Access_منزل_پیغام_بفر( xTask, x_پیغام_بفر_منزل_Grant_Access )      v_Grant_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_پیغام_بفر_منزل_Grant_Access ) )
            #تعریف_کړه v_Revoke_Access_منزل_پیغام_بفر( xTask, x_پیغام_بفر_منزل_Revoke_Access )    v_Revoke_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_پیغام_بفر_منزل_Revoke_Access ) )

            #تعریف_کړه v_Grant_Access_منزل_Timer( xTask, x_Timer_منزل_Grant_Access )                      v_Grant_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_Timer_منزل_Grant_Access ) )
            #تعریف_کړه v_Revoke_Access_منزل_Timer( xTask, x_Timer_منزل_Revoke_Access )                    v_Revoke_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_Timer_منزل_Revoke_Access ) )

        #د_که_پای /* #if ( ( configUSE_MPU_WRAPPERS_V1 == 0 ) && ( configENABLE_ACCESS_CONTROL_LIST == 1 ) ) */

    #د_که_پای /* MPU_WRAPPERS_INCLUDED_FROM_API_FILE */

    #تعریف_کړه PRIVILEGED_FUNCTION     __attribute__( ( section( "privileged_functions" ) ) )
    #تعریف_کړه PRIVILEGED_معلومات         __attribute__( ( section( "privileged_data" ) ) )
    #تعریف_کړه FREERTOS_SYSTEM_CALL    __attribute__( ( section( "freertos_system_calls" ) ) )

#وإلا /* portUSING_MPU_WRAPPERS */

    #تعریف_کړه PRIVILEGED_FUNCTION
    #تعریف_کړه PRIVILEGED_معلومات
    #تعریف_کړه FREERTOS_SYSTEM_CALL

#د_که_پای /* portUSING_MPU_WRAPPERS */


#د_که_پای /* MPU_WRAPPERS_H */
