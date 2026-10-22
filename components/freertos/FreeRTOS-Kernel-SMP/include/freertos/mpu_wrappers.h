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

#اگر_غیر_تعریف_شدہ MPU_WRAPPERS_H
#تعریف_کرو MPU_WRAPPERS_H

/* This file redefines API functions to be called through a wrapper macro, but
 * only for ports that are using the MPU. */
#اگر ( پورٹ_USING_MPU_WRAPPERS == 1 )

/* MPU_WRAPPERS_INCLUDED_FROM_API_FILE will be defined when this file is
 * included from queue.c or task.c to prevent it from having an effect within
 * those files. */
    #اگر_غیر_تعریف_شدہ MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/*
 * Map standard (non MPU) API functions to equivalents that start
 * "MPU_".  This will cause the application code to call the MPU_
 * version, which wraps the non-MPU version with privilege promoting
 * then demoting code, so the kernel code always runs will full
 * privileges.
 */

/* Map standard task.h API functions to the MPU equivalents. */
        #تعریف_کرو vTaskDelay                            MPU_vTaskDelay
        #تعریف_کرو xTaskDelayUntil                       MPU_xTaskDelayUntil
        #تعریف_کرو xTaskAbortDelay                       MPU_xTaskAbortDelay
        #تعریف_کرو ux_Task_ترجیح_Get                     MPU_ux_Task_ترجیح_Get
        #تعریف_کرو eTaskGetState                         MPU_eTaskGetState
        #تعریف_کرو vTaskGetInfo                          MPU_vTaskGetInfo
        #تعریف_کرو vTaskSuspend                          MPU_vTaskSuspend
        #تعریف_کرو vTaskResume                           MPU_vTaskResume
        #تعریف_کرو x_Task_Get_Tick_گنتی                     MPU_x_Task_Get_Tick_گنتی
        #تعریف_کرو ux_Task_Get_نمبر_کا_Tasks                MPU_ux_Task_Get_نمبر_کا_Tasks
        #تعریف_کرو ux_Task_Get_ڈھیر_بلند_Water_Mark           MPU_ux_Task_Get_ڈھیر_بلند_Water_Mark
        #تعریف_کرو ux_Task_Get_ڈھیر_بلند_Water_Mark2          MPU_ux_Task_Get_ڈھیر_بلند_Water_Mark2
        #تعریف_کرو v_Task_سیٹ_Application_Task_لیبل            MPU_v_Task_سیٹ_Application_Task_لیبل
        #تعریف_کرو x_Task_Get_Application_Task_لیبل            MPU_x_Task_Get_Application_Task_لیبل
        #تعریف_کرو v_Task_سیٹ_Thread_Local_Storage_اشارہ_گر     MPU_v_Task_سیٹ_Thread_Local_Storage_اشارہ_گر
        #تعریف_کرو pv_Task_Get_Thread_Local_Storage_اشارہ_گر    MPU_pv_Task_Get_Thread_Local_Storage_اشارہ_گر
        #تعریف_کرو xTaskGetIdleTaskHandle                MPU_xTaskGetIdleTaskHandle
        #تعریف_کرو uxTaskGetSystemState                  MPU_uxTaskGetSystemState
        #تعریف_کرو ul_Task_Get_Idle_تسلسل_وقت_شمار           MPU_ul_Task_Get_Idle_تسلسل_وقت_شمار
        #تعریف_کرو ul_Task_Get_Idle_تسلسل_وقت_Percent           MPU_ul_Task_Get_Idle_تسلسل_وقت_Percent
        #تعریف_کرو xTaskGenericNotify                    MPU_xTaskGenericNotify
        #تعریف_کرو xTaskGenericNotifyWait                MPU_xTaskGenericNotifyWait
        #تعریف_کرو ulTaskGenericNotifyTake               MPU_ulTaskGenericNotifyTake
        #تعریف_کرو xTaskGenericNotifyStateClear          MPU_xTaskGenericNotifyStateClear
        #تعریف_کرو ul_Task_Generic_Notify_قدر_Clear         MPU_ul_Task_Generic_Notify_قدر_Clear
        #تعریف_کرو v_Task_سیٹ_وقت_برآمد_State                  MPU_v_Task_سیٹ_وقت_برآمد_State
        #تعریف_کرو x_Task_Check_For_وقت_برآمد                  MPU_x_Task_Check_For_وقت_برآمد
        #تعریف_کرو xTaskGetCurrentTaskHandle             MPU_xTaskGetCurrentTaskHandle
        #تعریف_کرو xTaskGetSchedulerState                MPU_xTaskGetSchedulerState

        #اگر ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #تعریف_کرو ul_Task_Get_تسلسل_وقت_شمار           MPU_ul_Task_Get_تسلسل_وقت_شمار
            #تعریف_کرو ul_Task_Get_تسلسل_وقت_Percent           MPU_ul_Task_Get_تسلسل_وقت_Percent
        #اگر_ختم /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Privileged only wrappers for Task APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
        #تعریف_کرو xTaskCreate                              MPU_xTaskCreate
        #تعریف_کرو xTaskCreateStatic                        MPU_xTaskCreateStatic
        #تعریف_کرو vTaskDelete                              MPU_vTaskDelete
        #تعریف_کرو v_Task_ترجیح_سیٹ                         MPU_v_Task_ترجیح_سیٹ
        #تعریف_کرو xTaskGetHandle                           MPU_xTaskGetHandle
        #تعریف_کرو xTaskCallApplicationTaskHook             MPU_xTaskCallApplicationTaskHook

        #اگر ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #تعریف_کرو pcTaskGetName                        MPU_pcTaskGetName
            #تعریف_کرو xTaskCreateRestricted                MPU_xTaskCreateRestricted
            #تعریف_کرو xTaskCreateRestrictedStatic          MPU_xTaskCreateRestrictedStatic
            #تعریف_کرو vTaskAllocateMPURegions              MPU_vTaskAllocateMPURegions
            #تعریف_کرو xTaskGetStaticBuffers                MPU_xTaskGetStaticBuffers
            #تعریف_کرو ux_Task_ترجیح_Get_From_ISR             MPU_ux_Task_ترجیح_Get_From_ISR
            #تعریف_کرو ux_Task_بنیاد_ترجیح_Get                MPU_ux_Task_بنیاد_ترجیح_Get
            #تعریف_کرو ux_Task_بنیاد_ترجیح_Get_From_ISR         MPU_ux_Task_بنیاد_ترجیح_Get_From_ISR
            #تعریف_کرو xTaskResumeFromISR                   MPU_xTaskResumeFromISR
            #تعریف_کرو x_Task_Get_Application_Task_لیبل_From_ISR    MPU_x_Task_Get_Application_Task_لیبل_From_ISR
            #تعریف_کرو xTaskGenericNotifyFromISR            MPU_xTaskGenericNotifyFromISR
            #تعریف_کرو vTaskGenericNotifyGiveFromISR        MPU_vTaskGenericNotifyGiveFromISR
        #اگر_ختم /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Map standard queue.h API functions to the MPU equivalents. */
        #تعریف_کرو x_قطار_Generic_Send            MPU_x_قطار_Generic_Send
        #تعریف_کرو x_قطار_Receive                MPU_x_قطار_Receive
        #تعریف_کرو x_قطار_Peek                   MPU_x_قطار_Peek
        #تعریف_کرو x_قطار_Semaphore_Take          MPU_x_قطار_Semaphore_Take
        #تعریف_کرو ux_قطار_Messages_انتظار       MPU_ux_قطار_Messages_انتظار
        #تعریف_کرو ux_قطار_Spaces_دستیاب       MPU_ux_قطار_Spaces_دستیاب
        #تعریف_کرو x_قطار_Get_Mutex_Holder         MPU_x_قطار_Get_Mutex_Holder
        #تعریف_کرو x_قطار_Take_Mutex_Recursive     MPU_x_قطار_Take_Mutex_Recursive
        #تعریف_کرو x_قطار_Give_Mutex_Recursive     MPU_x_قطار_Give_Mutex_Recursive
        #تعریف_کرو x_قطار_جوڑو_مقصد_سیٹ               MPU_x_قطار_جوڑو_مقصد_سیٹ
        #تعریف_کرو x_قطار_Select_From_سیٹ          MPU_x_قطار_Select_From_سیٹ

        #اگر ( config_قطار_REGISTRY_سائز > 0 )
            #تعریف_کرو v_قطار_جوڑو_مقصد_Registry      MPU_v_قطار_جوڑو_مقصد_Registry
            #تعریف_کرو v_قطار_Unregister_قطار    MPU_v_قطار_Unregister_قطار
            #تعریف_کرو pc_قطار_Get_Name           MPU_pc_قطار_Get_Name
        #اگر_ختم /* #if ( configQUEUE_REGISTRY_SIZE > 0 ) */

/* Privileged only wrappers for Queue APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
        #تعریف_کرو v_قطار_Delete                           MPU_v_قطار_Delete
        #تعریف_کرو x_قطار_Create_Mutex                      MPU_x_قطار_Create_Mutex
        #تعریف_کرو x_قطار_Create_Mutex_Static                MPU_x_قطار_Create_Mutex_Static
        #تعریف_کرو x_قطار_Create_Counting_Semaphore          MPU_x_قطار_Create_Counting_Semaphore
        #تعریف_کرو x_قطار_Create_Counting_Semaphore_Static    MPU_x_قطار_Create_Counting_Semaphore_Static
        #تعریف_کرو x_قطار_Generic_Create                    MPU_x_قطار_Generic_Create
        #تعریف_کرو x_قطار_Generic_Create_Static              MPU_x_قطار_Generic_Create_Static
        #تعریف_کرو x_قطار_Generic_Reset                     MPU_x_قطار_Generic_Reset
        #تعریف_کرو x_قطار_Create_سیٹ                        MPU_x_قطار_Create_سیٹ
        #تعریف_کرو x_قطار_Remove_From_سیٹ                    MPU_x_قطار_Remove_From_سیٹ

        #اگر ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #تعریف_کرو x_قطار_Generic_Get_Static_Buffers      MPU_x_قطار_Generic_Get_Static_Buffers
            #تعریف_کرو x_قطار_Generic_Send_From_ISR           MPU_x_قطار_Generic_Send_From_ISR
            #تعریف_کرو x_قطار_Give_From_ISR                  MPU_x_قطار_Give_From_ISR
            #تعریف_کرو x_قطار_Peek_From_ISR                  MPU_x_قطار_Peek_From_ISR
            #تعریف_کرو x_قطار_Receive_From_ISR               MPU_x_قطار_Receive_From_ISR
            #تعریف_کرو x_قطار_Is_قطار_خالی_خانے_From_ISR          MPU_x_قطار_Is_قطار_خالی_خانے_From_ISR
            #تعریف_کرو x_قطار_Is_قطار_بھرا_From_ISR           MPU_x_قطار_Is_قطار_بھرا_From_ISR
            #تعریف_کرو ux_قطار_Messages_انتظار_From_ISR      MPU_ux_قطار_Messages_انتظار_From_ISR
            #تعریف_کرو x_قطار_Get_Mutex_Holder_From_ISR        MPU_x_قطار_Get_Mutex_Holder_From_ISR
            #تعریف_کرو x_قطار_Select_From_سیٹ_From_ISR         MPU_x_قطار_Select_From_سیٹ_From_ISR
        #اگر_ختم /* if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Map standard timer.h API functions to the MPU equivalents. */
        #تعریف_کرو pvTimerGetTimerID                 MPU_pvTimerGetTimerID
        #تعریف_کرو v_Timer_سیٹ_Timer_ID                  MPU_v_Timer_سیٹ_Timer_ID
        #تعریف_کرو x_Timer_Is_Timer_فعال               MPU_x_Timer_Is_Timer_فعال
        #تعریف_کرو xTimerGetTimerDaemonTaskHandle    MPU_xTimerGetTimerDaemonTaskHandle
        #تعریف_کرو xTimerGenericCommandFromTask      MPU_xTimerGenericCommandFromTask
        #تعریف_کرو pcTimerGetName                    MPU_pcTimerGetName
        #تعریف_کرو v_Timer_سیٹ_Reload_منوال               MPU_v_Timer_سیٹ_Reload_منوال
        #تعریف_کرو ux_Timer_Get_Reload_منوال              MPU_ux_Timer_Get_Reload_منوال
        #تعریف_کرو xTimerGetPeriod                   MPU_xTimerGetPeriod
        #تعریف_کرو x_Timer_Get_Expiry_وقت               MPU_x_Timer_Get_Expiry_وقت

/* Privileged only wrappers for Timer APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
        #اگر ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #تعریف_کرو x_Timer_Get_Reload_منوال            MPU_x_Timer_Get_Reload_منوال
            #تعریف_کرو xTimerCreate                   MPU_xTimerCreate
            #تعریف_کرو xTimerCreateStatic             MPU_xTimerCreateStatic
            #تعریف_کرو x_Timer_Get_Static_بفر          MPU_x_Timer_Get_Static_بفر
            #تعریف_کرو xTimerGenericCommandFromISR    MPU_xTimerGenericCommandFromISR
        #اگر_ختم /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Map standard event_group.h API functions to the MPU equivalents. */
        #تعریف_کرو x_Event_گروہ_Wait_بٹ          MPU_x_Event_گروہ_Wait_بٹ
        #تعریف_کرو x_Event_گروہ_Clear_بٹ         MPU_x_Event_گروہ_Clear_بٹ
        #تعریف_کرو x_Event_گروہ_سیٹ_بٹ           MPU_x_Event_گروہ_سیٹ_بٹ
        #تعریف_کرو x_Event_گروہ_Sync              MPU_x_Event_گروہ_Sync

        #اگر ( ( configUSE_TRACE_FACILITY == 1 ) && ( configUSE_MPU_WRAPPERS_V1 == 0 ) )
            #تعریف_کرو ux_Event_گروہ_Get_نمبر    MPU_ux_Event_گروہ_Get_نمبر
            #تعریف_کرو v_Event_گروہ_سیٹ_نمبر     MPU_v_Event_گروہ_سیٹ_نمبر
        #اگر_ختم /* #if ( ( configUSE_TRACE_FACILITY == 1 ) && ( configUSE_MPU_WRAPPERS_V1 == 0 ) ) */

/* Privileged only wrappers for Event Group APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
        #تعریف_کرو x_Event_گروہ_Create                  MPU_x_Event_گروہ_Create
        #تعریف_کرو x_Event_گروہ_Create_Static            MPU_x_Event_گروہ_Create_Static
        #تعریف_کرو v_Event_گروہ_Delete                  MPU_v_Event_گروہ_Delete

        #اگر ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #تعریف_کرو x_Event_گروہ_Get_Static_بفر     MPU_x_Event_گروہ_Get_Static_بفر
            #تعریف_کرو x_Event_گروہ_Clear_بٹ_From_ISR    MPU_x_Event_گروہ_Clear_بٹ_From_ISR
            #تعریف_کرو x_Event_گروہ_سیٹ_بٹ_From_ISR      MPU_x_Event_گروہ_سیٹ_بٹ_From_ISR
            #تعریف_کرو x_Event_گروہ_Get_بٹ_From_ISR      MPU_x_Event_گروہ_Get_بٹ_From_ISR
        #اگر_ختم /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Map standard message/stream_buffer.h API functions to the MPU
 * equivalents. */
        #تعریف_کرو x_دھارا_بفر_Send                      MPU_x_دھارا_بفر_Send
        #تعریف_کرو x_دھارا_بفر_Receive                   MPU_x_دھارا_بفر_Receive
        #تعریف_کرو x_دھارا_بفر_Is_بھرا                    MPU_x_دھارا_بفر_Is_بھرا
        #تعریف_کرو x_دھارا_بفر_Is_خالی_خانے                   MPU_x_دھارا_بفر_Is_خالی_خانے
        #تعریف_کرو x_دھارا_بفر_Spaces_دستیاب           MPU_x_دھارا_بفر_Spaces_دستیاب
        #تعریف_کرو x_دھارا_بفر_بائٹیں_دستیاب            MPU_x_دھارا_بفر_بائٹیں_دستیاب
        #تعریف_کرو x_دھارا_بفر_سیٹ_Trigger_Level           MPU_x_دھارا_بفر_سیٹ_Trigger_Level
        #تعریف_کرو x_دھارا_بفر_اگلا_پیغام_لمبائی_بائٹیں    MPU_x_دھارا_بفر_اگلا_پیغام_لمبائی_بائٹیں

/* Privileged only wrappers for Stream Buffer APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */

        #تعریف_کرو x_دھارا_بفر_Generic_Create                  MPU_x_دھارا_بفر_Generic_Create
        #تعریف_کرو x_دھارا_بفر_Generic_Create_Static            MPU_x_دھارا_بفر_Generic_Create_Static
        #تعریف_کرو v_دھارا_بفر_Delete                         MPU_v_دھارا_بفر_Delete
        #تعریف_کرو x_دھارا_بفر_Reset                          MPU_x_دھارا_بفر_Reset

        #اگر ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #تعریف_کرو x_دھارا_بفر_Get_Static_Buffers           MPU_x_دھارا_بفر_Get_Static_Buffers
            #تعریف_کرو x_دھارا_بفر_Send_From_ISR                MPU_x_دھارا_بفر_Send_From_ISR
            #تعریف_کرو x_دھارا_بفر_Receive_From_ISR             MPU_x_دھارا_بفر_Receive_From_ISR
            #تعریف_کرو x_دھارا_بفر_Send_Completed_From_ISR       MPU_x_دھارا_بفر_Send_Completed_From_ISR
            #تعریف_کرو x_دھارا_بفر_Receive_Completed_From_ISR    MPU_x_دھارا_بفر_Receive_Completed_From_ISR
            #تعریف_کرو x_دھارا_بفر_Reset_From_ISR               MPU_x_دھارا_بفر_Reset_From_ISR
        #اگر_ختم /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

        #اگر ( ( configUSE_MPU_WRAPPERS_V1 == 0 ) && ( configENABLE_ACCESS_CONTROL_LIST == 1 ) )

            #تعریف_کرو v_Grant_Access_مقصد_Task( xTask, x_Task_مقصد_Grant_Access )                        v_Grant_Access_مقصد_Kernel_Object( ( xTask ), ( int32_t ) ( x_Task_مقصد_Grant_Access ) )
            #تعریف_کرو v_Revoke_Access_مقصد_Task( xTask, x_Task_مقصد_Revoke_Access )                      v_Revoke_Access_مقصد_Kernel_Object( ( xTask ), ( int32_t ) ( x_Task_مقصد_Revoke_Access ) )

            #تعریف_کرو v_Grant_Access_مقصد_Semaphore( xTask, x_Semaphore_مقصد_Grant_Access )              v_Grant_Access_مقصد_Kernel_Object( ( xTask ), ( int32_t ) ( x_Semaphore_مقصد_Grant_Access ) )
            #تعریف_کرو v_Revoke_Access_مقصد_Semaphore( xTask, x_Semaphore_مقصد_Revoke_Access )            v_Revoke_Access_مقصد_Kernel_Object( ( xTask ), ( int32_t ) ( x_Semaphore_مقصد_Revoke_Access ) )

            #تعریف_کرو v_Grant_Access_مقصد_قطار( xTask, x_قطار_مقصد_Grant_Access )                      v_Grant_Access_مقصد_Kernel_Object( ( xTask ), ( int32_t ) ( x_قطار_مقصد_Grant_Access ) )
            #تعریف_کرو v_Revoke_Access_مقصد_قطار( xTask, x_قطار_مقصد_Revoke_Access )                    v_Revoke_Access_مقصد_Kernel_Object( ( xTask ), ( int32_t ) ( x_قطار_مقصد_Revoke_Access ) )

            #تعریف_کرو v_Grant_Access_مقصد_قطار_سیٹ( xTask, x_قطار_سیٹ_مقصد_Grant_Access )                v_Grant_Access_مقصد_Kernel_Object( ( xTask ), ( int32_t ) ( x_قطار_سیٹ_مقصد_Grant_Access ) )
            #تعریف_کرو v_Revoke_Access_مقصد_قطار_سیٹ( xTask, x_قطار_سیٹ_مقصد_Revoke_Access )              v_Revoke_Access_مقصد_Kernel_Object( ( xTask ), ( int32_t ) ( x_قطار_سیٹ_مقصد_Revoke_Access ) )

            #تعریف_کرو v_Grant_Access_مقصد_Event_گروہ( xTask, x_Event_گروہ_مقصد_Grant_Access )            v_Grant_Access_مقصد_Kernel_Object( ( xTask ), ( int32_t ) ( x_Event_گروہ_مقصد_Grant_Access ) )
            #تعریف_کرو v_Revoke_Access_مقصد_Event_گروہ( xTask, x_Event_گروہ_مقصد_Revoke_Access )          v_Revoke_Access_مقصد_Kernel_Object( ( xTask ), ( int32_t ) ( x_Event_گروہ_مقصد_Revoke_Access ) )

            #تعریف_کرو v_Grant_Access_مقصد_دھارا_بفر( xTask, x_دھارا_بفر_مقصد_Grant_Access )        v_Grant_Access_مقصد_Kernel_Object( ( xTask ), ( int32_t ) ( x_دھارا_بفر_مقصد_Grant_Access ) )
            #تعریف_کرو v_Revoke_Access_مقصد_دھارا_بفر( xTask, x_دھارا_بفر_مقصد_Revoke_Access )      v_Revoke_Access_مقصد_Kernel_Object( ( xTask ), ( int32_t ) ( x_دھارا_بفر_مقصد_Revoke_Access ) )

            #تعریف_کرو v_Grant_Access_مقصد_پیغام_بفر( xTask, x_پیغام_بفر_مقصد_Grant_Access )      v_Grant_Access_مقصد_Kernel_Object( ( xTask ), ( int32_t ) ( x_پیغام_بفر_مقصد_Grant_Access ) )
            #تعریف_کرو v_Revoke_Access_مقصد_پیغام_بفر( xTask, x_پیغام_بفر_مقصد_Revoke_Access )    v_Revoke_Access_مقصد_Kernel_Object( ( xTask ), ( int32_t ) ( x_پیغام_بفر_مقصد_Revoke_Access ) )

            #تعریف_کرو v_Grant_Access_مقصد_Timer( xTask, x_Timer_مقصد_Grant_Access )                      v_Grant_Access_مقصد_Kernel_Object( ( xTask ), ( int32_t ) ( x_Timer_مقصد_Grant_Access ) )
            #تعریف_کرو v_Revoke_Access_مقصد_Timer( xTask, x_Timer_مقصد_Revoke_Access )                    v_Revoke_Access_مقصد_Kernel_Object( ( xTask ), ( int32_t ) ( x_Timer_مقصد_Revoke_Access ) )

        #اگر_ختم /* #if ( ( configUSE_MPU_WRAPPERS_V1 == 0 ) && ( configENABLE_ACCESS_CONTROL_LIST == 1 ) ) */

    #اگر_ختم /* MPU_WRAPPERS_INCLUDED_FROM_API_FILE */

    #تعریف_کرو PRIVILEGED_FUNCTION     __attribute__( ( section( "privileged_functions" ) ) )
    #تعریف_کرو PRIVILEGED_ڈیٹا         __attribute__( ( section( "privileged_data" ) ) )
    #تعریف_کرو FREERTOS_SYSTEM_CALL    __attribute__( ( section( "freertos_system_calls" ) ) )

#ورنہ /* portUSING_MPU_WRAPPERS */

    #تعریف_کرو PRIVILEGED_FUNCTION
    #تعریف_کرو PRIVILEGED_ڈیٹا
    #تعریف_کرو FREERTOS_SYSTEM_CALL

#اگر_ختم /* portUSING_MPU_WRAPPERS */


#اگر_ختم /* MPU_WRAPPERS_H */
