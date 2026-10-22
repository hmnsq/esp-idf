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

#اگر_تعریف_نشده MPU_WRAPPERS_H
#تعریف_کن MPU_WRAPPERS_H

/* This file redefines API functions to be called through a wrapper macro, but
 * only for ports that are using the MPU. */
#إذا ( پورت_USING_MPU_WRAPPERS == 1 )

/* MPU_WRAPPERS_INCLUDED_FROM_API_FILE will be defined when this file is
 * included from queue.c or task.c to prevent it from having an effect within
 * those files. */
    #اگر_تعریف_نشده MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/*
 * Map standard (non MPU) API functions to equivalents that start
 * "MPU_".  This will cause the application code to call the MPU_
 * version, which wraps the non-MPU version with privilege promoting
 * then demoting code, so the kernel code always runs will full
 * privileges.
 */

/* Map standard task.h API functions to the MPU equivalents. */
        #تعریف_کن vTaskDelay                            MPU_vTaskDelay
        #تعریف_کن xTaskDelayUntil                       MPU_xTaskDelayUntil
        #تعریف_کن xTaskAbortDelay                       MPU_xTaskAbortDelay
        #تعریف_کن ux_Task_اولویت_Get                     MPU_ux_Task_اولویت_Get
        #تعریف_کن eTaskGetState                         MPU_eTaskGetState
        #تعریف_کن vTaskGetInfo                          MPU_vTaskGetInfo
        #تعریف_کن vTaskSuspend                          MPU_vTaskSuspend
        #تعریف_کن vTaskResume                           MPU_vTaskResume
        #تعریف_کن x_Task_Get_Tick_شمار                     MPU_x_Task_Get_Tick_شمار
        #تعریف_کن ux_Task_Get_عدد_از_Tasks                MPU_ux_Task_Get_عدد_از_Tasks
        #تعریف_کن ux_Task_Get_پشته_بالا_Water_Mark           MPU_ux_Task_Get_پشته_بالا_Water_Mark
        #تعریف_کن ux_Task_Get_پشته_بالا_Water_Mark2          MPU_ux_Task_Get_پشته_بالا_Water_Mark2
        #تعریف_کن v_Task_مجموعه_Application_Task_برچسب            MPU_v_Task_مجموعه_Application_Task_برچسب
        #تعریف_کن x_Task_Get_Application_Task_برچسب            MPU_x_Task_Get_Application_Task_برچسب
        #تعریف_کن v_Task_مجموعه_Thread_Local_Storage_اشاره_گر     MPU_v_Task_مجموعه_Thread_Local_Storage_اشاره_گر
        #تعریف_کن pv_Task_Get_Thread_Local_Storage_اشاره_گر    MPU_pv_Task_Get_Thread_Local_Storage_اشاره_گر
        #تعریف_کن xTaskGetIdleTaskHandle                MPU_xTaskGetIdleTaskHandle
        #تعریف_کن uxTaskGetSystemState                  MPU_uxTaskGetSystemState
        #تعریف_کن ul_Task_Get_Idle_پیاپی_زمان_Counter           MPU_ul_Task_Get_Idle_پیاپی_زمان_Counter
        #تعریف_کن ul_Task_Get_Idle_پیاپی_زمان_Percent           MPU_ul_Task_Get_Idle_پیاپی_زمان_Percent
        #تعریف_کن xTaskGenericNotify                    MPU_xTaskGenericNotify
        #تعریف_کن xTaskGenericNotifyWait                MPU_xTaskGenericNotifyWait
        #تعریف_کن ulTaskGenericNotifyTake               MPU_ulTaskGenericNotifyTake
        #تعریف_کن xTaskGenericNotifyStateClear          MPU_xTaskGenericNotifyStateClear
        #تعریف_کن ul_Task_Generic_Notify_مقدار_Clear         MPU_ul_Task_Generic_Notify_مقدار_Clear
        #تعریف_کن v_Task_مجموعه_زمان_بیرون_State                  MPU_v_Task_مجموعه_زمان_بیرون_State
        #تعریف_کن x_Task_Check_For_زمان_بیرون                  MPU_x_Task_Check_For_زمان_بیرون
        #تعریف_کن xTaskGetCurrentTaskHandle             MPU_xTaskGetCurrentTaskHandle
        #تعریف_کن xTaskGetSchedulerState                MPU_xTaskGetSchedulerState

        #إذا ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #تعریف_کن ul_Task_Get_پیاپی_زمان_Counter           MPU_ul_Task_Get_پیاپی_زمان_Counter
            #تعریف_کن ul_Task_Get_پیاپی_زمان_Percent           MPU_ul_Task_Get_پیاپی_زمان_Percent
        #پایان_اگر /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Privileged only wrappers for Task APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
        #تعریف_کن xTaskCreate                              MPU_xTaskCreate
        #تعریف_کن xTaskCreateStatic                        MPU_xTaskCreateStatic
        #تعریف_کن vTaskDelete                              MPU_vTaskDelete
        #تعریف_کن v_Task_اولویت_مجموعه                         MPU_v_Task_اولویت_مجموعه
        #تعریف_کن xTaskGetHandle                           MPU_xTaskGetHandle
        #تعریف_کن xTaskCallApplicationTaskHook             MPU_xTaskCallApplicationTaskHook

        #إذا ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #تعریف_کن pcTaskGetName                        MPU_pcTaskGetName
            #تعریف_کن xTaskCreateRestricted                MPU_xTaskCreateRestricted
            #تعریف_کن xTaskCreateRestrictedStatic          MPU_xTaskCreateRestrictedStatic
            #تعریف_کن vTaskAllocateMPURegions              MPU_vTaskAllocateMPURegions
            #تعریف_کن xTaskGetStaticBuffers                MPU_xTaskGetStaticBuffers
            #تعریف_کن ux_Task_اولویت_Get_From_ISR             MPU_ux_Task_اولویت_Get_From_ISR
            #تعریف_کن ux_Task_پایه_اولویت_Get                MPU_ux_Task_پایه_اولویت_Get
            #تعریف_کن ux_Task_پایه_اولویت_Get_From_ISR         MPU_ux_Task_پایه_اولویت_Get_From_ISR
            #تعریف_کن xTaskResumeFromISR                   MPU_xTaskResumeFromISR
            #تعریف_کن x_Task_Get_Application_Task_برچسب_From_ISR    MPU_x_Task_Get_Application_Task_برچسب_From_ISR
            #تعریف_کن xTaskGenericNotifyFromISR            MPU_xTaskGenericNotifyFromISR
            #تعریف_کن vTaskGenericNotifyGiveFromISR        MPU_vTaskGenericNotifyGiveFromISR
        #پایان_اگر /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Map standard queue.h API functions to the MPU equivalents. */
        #تعریف_کن x_صف_Generic_Send            MPU_x_صف_Generic_Send
        #تعریف_کن x_صف_Receive                MPU_x_صف_Receive
        #تعریف_کن x_صف_Peek                   MPU_x_صف_Peek
        #تعریف_کن x_صف_Semaphore_Take          MPU_x_صف_Semaphore_Take
        #تعریف_کن ux_صف_Messages_انتظار       MPU_ux_صف_Messages_انتظار
        #تعریف_کن ux_صف_Spaces_موجود       MPU_ux_صف_Spaces_موجود
        #تعریف_کن x_صف_Get_Mutex_Holder         MPU_x_صف_Get_Mutex_Holder
        #تعریف_کن x_صف_Take_Mutex_Recursive     MPU_x_صف_Take_Mutex_Recursive
        #تعریف_کن x_صف_Give_Mutex_Recursive     MPU_x_صف_Give_Mutex_Recursive
        #تعریف_کن x_صف_جمع_کن_مقصد_مجموعه               MPU_x_صف_جمع_کن_مقصد_مجموعه
        #تعریف_کن x_صف_Select_From_مجموعه          MPU_x_صف_Select_From_مجموعه

        #إذا ( config_صف_REGISTRY_اندازه > 0 )
            #تعریف_کن v_صف_جمع_کن_مقصد_Registry      MPU_v_صف_جمع_کن_مقصد_Registry
            #تعریف_کن v_صف_Unregister_صف    MPU_v_صف_Unregister_صف
            #تعریف_کن pc_صف_Get_Name           MPU_pc_صف_Get_Name
        #پایان_اگر /* #if ( configQUEUE_REGISTRY_SIZE > 0 ) */

/* Privileged only wrappers for Queue APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
        #تعریف_کن v_صف_Delete                           MPU_v_صف_Delete
        #تعریف_کن x_صف_Create_Mutex                      MPU_x_صف_Create_Mutex
        #تعریف_کن x_صف_Create_Mutex_Static                MPU_x_صف_Create_Mutex_Static
        #تعریف_کن x_صف_Create_Counting_Semaphore          MPU_x_صف_Create_Counting_Semaphore
        #تعریف_کن x_صف_Create_Counting_Semaphore_Static    MPU_x_صف_Create_Counting_Semaphore_Static
        #تعریف_کن x_صف_Generic_Create                    MPU_x_صف_Generic_Create
        #تعریف_کن x_صف_Generic_Create_Static              MPU_x_صف_Generic_Create_Static
        #تعریف_کن x_صف_Generic_Reset                     MPU_x_صف_Generic_Reset
        #تعریف_کن x_صف_Create_مجموعه                        MPU_x_صف_Create_مجموعه
        #تعریف_کن x_صف_Remove_From_مجموعه                    MPU_x_صف_Remove_From_مجموعه

        #إذا ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #تعریف_کن x_صف_Generic_Get_Static_Buffers      MPU_x_صف_Generic_Get_Static_Buffers
            #تعریف_کن x_صف_Generic_Send_From_ISR           MPU_x_صف_Generic_Send_From_ISR
            #تعریف_کن x_صف_Give_From_ISR                  MPU_x_صف_Give_From_ISR
            #تعریف_کن x_صف_Peek_From_ISR                  MPU_x_صف_Peek_From_ISR
            #تعریف_کن x_صف_Receive_From_ISR               MPU_x_صف_Receive_From_ISR
            #تعریف_کن x_صف_Is_صف_خالی_From_ISR          MPU_x_صف_Is_صف_خالی_From_ISR
            #تعریف_کن x_صف_Is_صف_پر_From_ISR           MPU_x_صف_Is_صف_پر_From_ISR
            #تعریف_کن ux_صف_Messages_انتظار_From_ISR      MPU_ux_صف_Messages_انتظار_From_ISR
            #تعریف_کن x_صف_Get_Mutex_Holder_From_ISR        MPU_x_صف_Get_Mutex_Holder_From_ISR
            #تعریف_کن x_صف_Select_From_مجموعه_From_ISR         MPU_x_صف_Select_From_مجموعه_From_ISR
        #پایان_اگر /* if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Map standard timer.h API functions to the MPU equivalents. */
        #تعریف_کن pvTimerGetTimerID                 MPU_pvTimerGetTimerID
        #تعریف_کن v_Timer_مجموعه_Timer_ID                  MPU_v_Timer_مجموعه_Timer_ID
        #تعریف_کن x_Timer_Is_Timer_فعال               MPU_x_Timer_Is_Timer_فعال
        #تعریف_کن xTimerGetTimerDaemonTaskHandle    MPU_xTimerGetTimerDaemonTaskHandle
        #تعریف_کن xTimerGenericCommandFromTask      MPU_xTimerGenericCommandFromTask
        #تعریف_کن pcTimerGetName                    MPU_pcTimerGetName
        #تعریف_کن v_Timer_مجموعه_Reload_نما               MPU_v_Timer_مجموعه_Reload_نما
        #تعریف_کن ux_Timer_Get_Reload_نما              MPU_ux_Timer_Get_Reload_نما
        #تعریف_کن xTimerGetPeriod                   MPU_xTimerGetPeriod
        #تعریف_کن x_Timer_Get_Expiry_زمان               MPU_x_Timer_Get_Expiry_زمان

/* Privileged only wrappers for Timer APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
        #إذا ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #تعریف_کن x_Timer_Get_Reload_نما            MPU_x_Timer_Get_Reload_نما
            #تعریف_کن xTimerCreate                   MPU_xTimerCreate
            #تعریف_کن xTimerCreateStatic             MPU_xTimerCreateStatic
            #تعریف_کن x_Timer_Get_Static_میانگیر          MPU_x_Timer_Get_Static_میانگیر
            #تعریف_کن xTimerGenericCommandFromISR    MPU_xTimerGenericCommandFromISR
        #پایان_اگر /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Map standard event_group.h API functions to the MPU equivalents. */
        #تعریف_کن x_Event_گروه_Wait_بیتها          MPU_x_Event_گروه_Wait_بیتها
        #تعریف_کن x_Event_گروه_Clear_بیتها         MPU_x_Event_گروه_Clear_بیتها
        #تعریف_کن x_Event_گروه_مجموعه_بیتها           MPU_x_Event_گروه_مجموعه_بیتها
        #تعریف_کن x_Event_گروه_Sync              MPU_x_Event_گروه_Sync

        #إذا ( ( configUSE_TRACE_FACILITY == 1 ) && ( configUSE_MPU_WRAPPERS_V1 == 0 ) )
            #تعریف_کن ux_Event_گروه_Get_عدد    MPU_ux_Event_گروه_Get_عدد
            #تعریف_کن v_Event_گروه_مجموعه_عدد     MPU_v_Event_گروه_مجموعه_عدد
        #پایان_اگر /* #if ( ( configUSE_TRACE_FACILITY == 1 ) && ( configUSE_MPU_WRAPPERS_V1 == 0 ) ) */

/* Privileged only wrappers for Event Group APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
        #تعریف_کن x_Event_گروه_Create                  MPU_x_Event_گروه_Create
        #تعریف_کن x_Event_گروه_Create_Static            MPU_x_Event_گروه_Create_Static
        #تعریف_کن v_Event_گروه_Delete                  MPU_v_Event_گروه_Delete

        #إذا ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #تعریف_کن x_Event_گروه_Get_Static_میانگیر     MPU_x_Event_گروه_Get_Static_میانگیر
            #تعریف_کن x_Event_گروه_Clear_بیتها_From_ISR    MPU_x_Event_گروه_Clear_بیتها_From_ISR
            #تعریف_کن x_Event_گروه_مجموعه_بیتها_From_ISR      MPU_x_Event_گروه_مجموعه_بیتها_From_ISR
            #تعریف_کن x_Event_گروه_Get_بیتها_From_ISR      MPU_x_Event_گروه_Get_بیتها_From_ISR
        #پایان_اگر /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Map standard message/stream_buffer.h API functions to the MPU
 * equivalents. */
        #تعریف_کن x_جریان_میانگیر_Send                      MPU_x_جریان_میانگیر_Send
        #تعریف_کن x_جریان_میانگیر_Receive                   MPU_x_جریان_میانگیر_Receive
        #تعریف_کن x_جریان_میانگیر_Is_پر                    MPU_x_جریان_میانگیر_Is_پر
        #تعریف_کن x_جریان_میانگیر_Is_خالی                   MPU_x_جریان_میانگیر_Is_خالی
        #تعریف_کن x_جریان_میانگیر_Spaces_موجود           MPU_x_جریان_میانگیر_Spaces_موجود
        #تعریف_کن x_جریان_میانگیر_بایتها_موجود            MPU_x_جریان_میانگیر_بایتها_موجود
        #تعریف_کن x_جریان_میانگیر_مجموعه_Trigger_Level           MPU_x_جریان_میانگیر_مجموعه_Trigger_Level
        #تعریف_کن x_جریان_میانگیر_بعدی_پیام_درازی_بایتها    MPU_x_جریان_میانگیر_بعدی_پیام_درازی_بایتها

/* Privileged only wrappers for Stream Buffer APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */

        #تعریف_کن x_جریان_میانگیر_Generic_Create                  MPU_x_جریان_میانگیر_Generic_Create
        #تعریف_کن x_جریان_میانگیر_Generic_Create_Static            MPU_x_جریان_میانگیر_Generic_Create_Static
        #تعریف_کن v_جریان_میانگیر_Delete                         MPU_v_جریان_میانگیر_Delete
        #تعریف_کن x_جریان_میانگیر_Reset                          MPU_x_جریان_میانگیر_Reset

        #إذا ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #تعریف_کن x_جریان_میانگیر_Get_Static_Buffers           MPU_x_جریان_میانگیر_Get_Static_Buffers
            #تعریف_کن x_جریان_میانگیر_Send_From_ISR                MPU_x_جریان_میانگیر_Send_From_ISR
            #تعریف_کن x_جریان_میانگیر_Receive_From_ISR             MPU_x_جریان_میانگیر_Receive_From_ISR
            #تعریف_کن x_جریان_میانگیر_Send_Completed_From_ISR       MPU_x_جریان_میانگیر_Send_Completed_From_ISR
            #تعریف_کن x_جریان_میانگیر_Receive_Completed_From_ISR    MPU_x_جریان_میانگیر_Receive_Completed_From_ISR
            #تعریف_کن x_جریان_میانگیر_Reset_From_ISR               MPU_x_جریان_میانگیر_Reset_From_ISR
        #پایان_اگر /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

        #إذا ( ( configUSE_MPU_WRAPPERS_V1 == 0 ) && ( configENABLE_ACCESS_CONTROL_LIST == 1 ) )

            #تعریف_کن v_Grant_Access_مقصد_Task( xTask, x_Task_مقصد_Grant_Access )                        v_Grant_Access_مقصد_Kernel_Object( ( xTask ), ( int32_t ) ( x_Task_مقصد_Grant_Access ) )
            #تعریف_کن v_Revoke_Access_مقصد_Task( xTask, x_Task_مقصد_Revoke_Access )                      v_Revoke_Access_مقصد_Kernel_Object( ( xTask ), ( int32_t ) ( x_Task_مقصد_Revoke_Access ) )

            #تعریف_کن v_Grant_Access_مقصد_Semaphore( xTask, x_Semaphore_مقصد_Grant_Access )              v_Grant_Access_مقصد_Kernel_Object( ( xTask ), ( int32_t ) ( x_Semaphore_مقصد_Grant_Access ) )
            #تعریف_کن v_Revoke_Access_مقصد_Semaphore( xTask, x_Semaphore_مقصد_Revoke_Access )            v_Revoke_Access_مقصد_Kernel_Object( ( xTask ), ( int32_t ) ( x_Semaphore_مقصد_Revoke_Access ) )

            #تعریف_کن v_Grant_Access_مقصد_صف( xTask, x_صف_مقصد_Grant_Access )                      v_Grant_Access_مقصد_Kernel_Object( ( xTask ), ( int32_t ) ( x_صف_مقصد_Grant_Access ) )
            #تعریف_کن v_Revoke_Access_مقصد_صف( xTask, x_صف_مقصد_Revoke_Access )                    v_Revoke_Access_مقصد_Kernel_Object( ( xTask ), ( int32_t ) ( x_صف_مقصد_Revoke_Access ) )

            #تعریف_کن v_Grant_Access_مقصد_صف_مجموعه( xTask, x_صف_مجموعه_مقصد_Grant_Access )                v_Grant_Access_مقصد_Kernel_Object( ( xTask ), ( int32_t ) ( x_صف_مجموعه_مقصد_Grant_Access ) )
            #تعریف_کن v_Revoke_Access_مقصد_صف_مجموعه( xTask, x_صف_مجموعه_مقصد_Revoke_Access )              v_Revoke_Access_مقصد_Kernel_Object( ( xTask ), ( int32_t ) ( x_صف_مجموعه_مقصد_Revoke_Access ) )

            #تعریف_کن v_Grant_Access_مقصد_Event_گروه( xTask, x_Event_گروه_مقصد_Grant_Access )            v_Grant_Access_مقصد_Kernel_Object( ( xTask ), ( int32_t ) ( x_Event_گروه_مقصد_Grant_Access ) )
            #تعریف_کن v_Revoke_Access_مقصد_Event_گروه( xTask, x_Event_گروه_مقصد_Revoke_Access )          v_Revoke_Access_مقصد_Kernel_Object( ( xTask ), ( int32_t ) ( x_Event_گروه_مقصد_Revoke_Access ) )

            #تعریف_کن v_Grant_Access_مقصد_جریان_میانگیر( xTask, x_جریان_میانگیر_مقصد_Grant_Access )        v_Grant_Access_مقصد_Kernel_Object( ( xTask ), ( int32_t ) ( x_جریان_میانگیر_مقصد_Grant_Access ) )
            #تعریف_کن v_Revoke_Access_مقصد_جریان_میانگیر( xTask, x_جریان_میانگیر_مقصد_Revoke_Access )      v_Revoke_Access_مقصد_Kernel_Object( ( xTask ), ( int32_t ) ( x_جریان_میانگیر_مقصد_Revoke_Access ) )

            #تعریف_کن v_Grant_Access_مقصد_پیام_میانگیر( xTask, x_پیام_میانگیر_مقصد_Grant_Access )      v_Grant_Access_مقصد_Kernel_Object( ( xTask ), ( int32_t ) ( x_پیام_میانگیر_مقصد_Grant_Access ) )
            #تعریف_کن v_Revoke_Access_مقصد_پیام_میانگیر( xTask, x_پیام_میانگیر_مقصد_Revoke_Access )    v_Revoke_Access_مقصد_Kernel_Object( ( xTask ), ( int32_t ) ( x_پیام_میانگیر_مقصد_Revoke_Access ) )

            #تعریف_کن v_Grant_Access_مقصد_Timer( xTask, x_Timer_مقصد_Grant_Access )                      v_Grant_Access_مقصد_Kernel_Object( ( xTask ), ( int32_t ) ( x_Timer_مقصد_Grant_Access ) )
            #تعریف_کن v_Revoke_Access_مقصد_Timer( xTask, x_Timer_مقصد_Revoke_Access )                    v_Revoke_Access_مقصد_Kernel_Object( ( xTask ), ( int32_t ) ( x_Timer_مقصد_Revoke_Access ) )

        #پایان_اگر /* #if ( ( configUSE_MPU_WRAPPERS_V1 == 0 ) && ( configENABLE_ACCESS_CONTROL_LIST == 1 ) ) */

    #پایان_اگر /* MPU_WRAPPERS_INCLUDED_FROM_API_FILE */

    #تعریف_کن PRIVILEGED_FUNCTION     __attribute__( ( section( "privileged_functions" ) ) )
    #تعریف_کن PRIVILEGED_داده         __attribute__( ( section( "privileged_data" ) ) )
    #تعریف_کن FREERTOS_SYSTEM_CALL    __attribute__( ( section( "freertos_system_calls" ) ) )

#وإلا /* portUSING_MPU_WRAPPERS */

    #تعریف_کن PRIVILEGED_FUNCTION
    #تعریف_کن PRIVILEGED_داده
    #تعریف_کن FREERTOS_SYSTEM_CALL

#پایان_اگر /* portUSING_MPU_WRAPPERS */


#پایان_اگر /* MPU_WRAPPERS_H */
