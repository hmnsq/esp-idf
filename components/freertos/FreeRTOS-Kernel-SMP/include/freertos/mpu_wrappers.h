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

#جے_بنا_تعریف MPU_WRAPPERS_H
#تعریف_کرو MPU_WRAPPERS_H

/* This file redefines API functions to be called through a wrapper macro, but
 * only for ports that are using the MPU. */
#إذا ( پورٹ_USING_MPU_WRAPPERS == 1 )

/* MPU_WRAPPERS_INCLUDED_FROM_API_FILE will be defined when this file is
 * included from queue.c or task.c to prevent it from having an effect within
 * those files. */
    #جے_بنا_تعریف MPU_WRAPPERS_INCLUDED_FROM_API_FILE

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
        #تعریف_کرو ux_Task_Get_نمبر_دا_Tasks                MPU_ux_Task_Get_نمبر_دا_Tasks
        #تعریف_کرو ux_Task_Get_ڈھیر_بلند_Water_Mark           MPU_ux_Task_Get_ڈھیر_بلند_Water_Mark
        #تعریف_کرو ux_Task_Get_ڈھیر_بلند_Water_Mark2          MPU_ux_Task_Get_ڈھیر_بلند_Water_Mark2
        #تعریف_کرو v_Task_سیٹ_Application_Task_ٹیگ            MPU_v_Task_سیٹ_Application_Task_ٹیگ
        #تعریف_کرو x_Task_Get_Application_Task_ٹیگ            MPU_x_Task_Get_Application_Task_ٹیگ
        #تعریف_کرو v_Task_سیٹ_Thread_Local_Storage_اشارہ_گر     MPU_v_Task_سیٹ_Thread_Local_Storage_اشارہ_گر
        #تعریف_کرو pv_Task_Get_Thread_Local_Storage_اشارہ_گر    MPU_pv_Task_Get_Thread_Local_Storage_اشارہ_گر
        #تعریف_کرو xTaskGetIdleTaskHandle                MPU_xTaskGetIdleTaskHandle
        #تعریف_کرو uxTaskGetSystemState                  MPU_uxTaskGetSystemState
        #تعریف_کرو ul_Task_Get_Idle_لگاتار_ویلا_Counter           MPU_ul_Task_Get_Idle_لگاتار_ویلا_Counter
        #تعریف_کرو ul_Task_Get_Idle_لگاتار_ویلا_Percent           MPU_ul_Task_Get_Idle_لگاتار_ویلا_Percent
        #تعریف_کرو xTaskGenericNotify                    MPU_xTaskGenericNotify
        #تعریف_کرو xTaskGenericNotifyWait                MPU_xTaskGenericNotifyWait
        #تعریف_کرو ulTaskGenericNotifyTake               MPU_ulTaskGenericNotifyTake
        #تعریف_کرو xTaskGenericNotifyStateClear          MPU_xTaskGenericNotifyStateClear
        #تعریف_کرو ul_Task_Generic_Notify_قدر_Clear         MPU_ul_Task_Generic_Notify_قدر_Clear
        #تعریف_کرو v_Task_سیٹ_ویلا_باہرلا_State                  MPU_v_Task_سیٹ_ویلا_باہرلا_State
        #تعریف_کرو x_Task_Check_For_ویلا_باہرلا                  MPU_x_Task_Check_For_ویلا_باہرلا
        #تعریف_کرو xTaskGetCurrentTaskHandle             MPU_xTaskGetCurrentTaskHandle
        #تعریف_کرو xTaskGetSchedulerState                MPU_xTaskGetSchedulerState

        #إذا ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #تعریف_کرو ul_Task_Get_لگاتار_ویلا_Counter           MPU_ul_Task_Get_لگاتار_ویلا_Counter
            #تعریف_کرو ul_Task_Get_لگاتار_ویلا_Percent           MPU_ul_Task_Get_لگاتار_ویلا_Percent
        #جے_انت /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Privileged only wrappers for Task APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
        #تعریف_کرو xTaskCreate                              MPU_xTaskCreate
        #تعریف_کرو xTaskCreateStatic                        MPU_xTaskCreateStatic
        #تعریف_کرو vTaskDelete                              MPU_vTaskDelete
        #تعریف_کرو v_Task_ترجیح_سیٹ                         MPU_v_Task_ترجیح_سیٹ
        #تعریف_کرو xTaskGetHandle                           MPU_xTaskGetHandle
        #تعریف_کرو xTaskCallApplicationTaskHook             MPU_xTaskCallApplicationTaskHook

        #إذا ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #تعریف_کرو pcTaskGetName                        MPU_pcTaskGetName
            #تعریف_کرو xTaskCreateRestricted                MPU_xTaskCreateRestricted
            #تعریف_کرو xTaskCreateRestrictedStatic          MPU_xTaskCreateRestrictedStatic
            #تعریف_کرو vTaskAllocateMPURegions              MPU_vTaskAllocateMPURegions
            #تعریف_کرو xTaskGetStaticBuffers                MPU_xTaskGetStaticBuffers
            #تعریف_کرو ux_Task_ترجیح_Get_From_ISR             MPU_ux_Task_ترجیح_Get_From_ISR
            #تعریف_کرو ux_Task_ادھار_ترجیح_Get                MPU_ux_Task_ادھار_ترجیح_Get
            #تعریف_کرو ux_Task_ادھار_ترجیح_Get_From_ISR         MPU_ux_Task_ادھار_ترجیح_Get_From_ISR
            #تعریف_کرو xTaskResumeFromISR                   MPU_xTaskResumeFromISR
            #تعریف_کرو x_Task_Get_Application_Task_ٹیگ_From_ISR    MPU_x_Task_Get_Application_Task_ٹیگ_From_ISR
            #تعریف_کرو xTaskGenericNotifyFromISR            MPU_xTaskGenericNotifyFromISR
            #تعریف_کرو vTaskGenericNotifyGiveFromISR        MPU_vTaskGenericNotifyGiveFromISR
        #جے_انت /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Map standard queue.h API functions to the MPU equivalents. */
        #تعریف_کرو x_قطار_Generic_Send            MPU_x_قطار_Generic_Send
        #تعریف_کرو x_قطار_Receive                MPU_x_قطار_Receive
        #تعریف_کرو x_قطار_Peek                   MPU_x_قطار_Peek
        #تعریف_کرو x_قطار_Semaphore_Take          MPU_x_قطار_Semaphore_Take
        #تعریف_کرو ux_قطار_Messages_اڈیک       MPU_ux_قطار_Messages_اڈیک
        #تعریف_کرو ux_قطار_Spaces_دستیاب       MPU_ux_قطار_Spaces_دستیاب
        #تعریف_کرو x_قطار_Get_Mutex_Holder         MPU_x_قطار_Get_Mutex_Holder
        #تعریف_کرو x_قطار_Take_Mutex_Recursive     MPU_x_قطار_Take_Mutex_Recursive
        #تعریف_کرو x_قطار_Give_Mutex_Recursive     MPU_x_قطار_Give_Mutex_Recursive
        #تعریف_کرو x_قطار_جوڑو_منزل_سیٹ               MPU_x_قطار_جوڑو_منزل_سیٹ
        #تعریف_کرو x_قطار_Select_From_سیٹ          MPU_x_قطار_Select_From_سیٹ

        #إذا ( config_قطار_REGISTRY_ناپ > 0 )
            #تعریف_کرو v_قطار_جوڑو_منزل_Registry      MPU_v_قطار_جوڑو_منزل_Registry
            #تعریف_کرو v_قطار_Unregister_قطار    MPU_v_قطار_Unregister_قطار
            #تعریف_کرو pc_قطار_Get_Name           MPU_pc_قطار_Get_Name
        #جے_انت /* #if ( configQUEUE_REGISTRY_SIZE > 0 ) */

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

        #إذا ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #تعریف_کرو x_قطار_Generic_Get_Static_Buffers      MPU_x_قطار_Generic_Get_Static_Buffers
            #تعریف_کرو x_قطار_Generic_Send_From_ISR           MPU_x_قطار_Generic_Send_From_ISR
            #تعریف_کرو x_قطار_Give_From_ISR                  MPU_x_قطار_Give_From_ISR
            #تعریف_کرو x_قطار_Peek_From_ISR                  MPU_x_قطار_Peek_From_ISR
            #تعریف_کرو x_قطار_Receive_From_ISR               MPU_x_قطار_Receive_From_ISR
            #تعریف_کرو x_قطار_Is_قطار_خالی_تھاواں_From_ISR          MPU_x_قطار_Is_قطار_خالی_تھاواں_From_ISR
            #تعریف_کرو x_قطار_Is_قطار_بھریا_From_ISR           MPU_x_قطار_Is_قطار_بھریا_From_ISR
            #تعریف_کرو ux_قطار_Messages_اڈیک_From_ISR      MPU_ux_قطار_Messages_اڈیک_From_ISR
            #تعریف_کرو x_قطار_Get_Mutex_Holder_From_ISR        MPU_x_قطار_Get_Mutex_Holder_From_ISR
            #تعریف_کرو x_قطار_Select_From_سیٹ_From_ISR         MPU_x_قطار_Select_From_سیٹ_From_ISR
        #جے_انت /* if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Map standard timer.h API functions to the MPU equivalents. */
        #تعریف_کرو pvTimerGetTimerID                 MPU_pvTimerGetTimerID
        #تعریف_کرو v_Timer_سیٹ_Timer_ID                  MPU_v_Timer_سیٹ_Timer_ID
        #تعریف_کرو x_Timer_Is_Timer_سرگرم               MPU_x_Timer_Is_Timer_سرگرم
        #تعریف_کرو xTimerGetTimerDaemonTaskHandle    MPU_xTimerGetTimerDaemonTaskHandle
        #تعریف_کرو xTimerGenericCommandFromTask      MPU_xTimerGenericCommandFromTask
        #تعریف_کرو pcTimerGetName                    MPU_pcTimerGetName
        #تعریف_کرو v_Timer_سیٹ_Reload_منوال               MPU_v_Timer_سیٹ_Reload_منوال
        #تعریف_کرو ux_Timer_Get_Reload_منوال              MPU_ux_Timer_Get_Reload_منوال
        #تعریف_کرو xTimerGetPeriod                   MPU_xTimerGetPeriod
        #تعریف_کرو x_Timer_Get_Expiry_ویلا               MPU_x_Timer_Get_Expiry_ویلا

/* Privileged only wrappers for Timer APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
        #إذا ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #تعریف_کرو x_Timer_Get_Reload_منوال            MPU_x_Timer_Get_Reload_منوال
            #تعریف_کرو xTimerCreate                   MPU_xTimerCreate
            #تعریف_کرو xTimerCreateStatic             MPU_xTimerCreateStatic
            #تعریف_کرو x_Timer_Get_Static_بفر          MPU_x_Timer_Get_Static_بفر
            #تعریف_کرو xTimerGenericCommandFromISR    MPU_xTimerGenericCommandFromISR
        #جے_انت /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Map standard event_group.h API functions to the MPU equivalents. */
        #تعریف_کرو x_Event_گروہ_Wait_بٹ          MPU_x_Event_گروہ_Wait_بٹ
        #تعریف_کرو x_Event_گروہ_Clear_بٹ         MPU_x_Event_گروہ_Clear_بٹ
        #تعریف_کرو x_Event_گروہ_سیٹ_بٹ           MPU_x_Event_گروہ_سیٹ_بٹ
        #تعریف_کرو x_Event_گروہ_Sync              MPU_x_Event_گروہ_Sync

        #إذا ( ( configUSE_TRACE_FACILITY == 1 ) && ( configUSE_MPU_WRAPPERS_V1 == 0 ) )
            #تعریف_کرو ux_Event_گروہ_Get_نمبر    MPU_ux_Event_گروہ_Get_نمبر
            #تعریف_کرو v_Event_گروہ_سیٹ_نمبر     MPU_v_Event_گروہ_سیٹ_نمبر
        #جے_انت /* #if ( ( configUSE_TRACE_FACILITY == 1 ) && ( configUSE_MPU_WRAPPERS_V1 == 0 ) ) */

/* Privileged only wrappers for Event Group APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
        #تعریف_کرو x_Event_گروہ_Create                  MPU_x_Event_گروہ_Create
        #تعریف_کرو x_Event_گروہ_Create_Static            MPU_x_Event_گروہ_Create_Static
        #تعریف_کرو v_Event_گروہ_Delete                  MPU_v_Event_گروہ_Delete

        #إذا ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #تعریف_کرو x_Event_گروہ_Get_Static_بفر     MPU_x_Event_گروہ_Get_Static_بفر
            #تعریف_کرو x_Event_گروہ_Clear_بٹ_From_ISR    MPU_x_Event_گروہ_Clear_بٹ_From_ISR
            #تعریف_کرو x_Event_گروہ_سیٹ_بٹ_From_ISR      MPU_x_Event_گروہ_سیٹ_بٹ_From_ISR
            #تعریف_کرو x_Event_گروہ_Get_بٹ_From_ISR      MPU_x_Event_گروہ_Get_بٹ_From_ISR
        #جے_انت /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Map standard message/stream_buffer.h API functions to the MPU
 * equivalents. */
        #تعریف_کرو x_دھارا_بفر_Send                      MPU_x_دھارا_بفر_Send
        #تعریف_کرو x_دھارا_بفر_Receive                   MPU_x_دھارا_بفر_Receive
        #تعریف_کرو x_دھارا_بفر_Is_بھریا                    MPU_x_دھارا_بفر_Is_بھریا
        #تعریف_کرو x_دھارا_بفر_Is_خالی_تھاواں                   MPU_x_دھارا_بفر_Is_خالی_تھاواں
        #تعریف_کرو x_دھارا_بفر_Spaces_دستیاب           MPU_x_دھارا_بفر_Spaces_دستیاب
        #تعریف_کرو x_دھارا_بفر_بائٹاں_دستیاب            MPU_x_دھارا_بفر_بائٹاں_دستیاب
        #تعریف_کرو x_دھارا_بفر_سیٹ_Trigger_Level           MPU_x_دھارا_بفر_سیٹ_Trigger_Level
        #تعریف_کرو x_دھارا_بفر_اگلا_سنیہا_لمائی_بائٹاں    MPU_x_دھارا_بفر_اگلا_سنیہا_لمائی_بائٹاں

/* Privileged only wrappers for Stream Buffer APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */

        #تعریف_کرو x_دھارا_بفر_Generic_Create                  MPU_x_دھارا_بفر_Generic_Create
        #تعریف_کرو x_دھارا_بفر_Generic_Create_Static            MPU_x_دھارا_بفر_Generic_Create_Static
        #تعریف_کرو v_دھارا_بفر_Delete                         MPU_v_دھارا_بفر_Delete
        #تعریف_کرو x_دھارا_بفر_Reset                          MPU_x_دھارا_بفر_Reset

        #إذا ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #تعریف_کرو x_دھارا_بفر_Get_Static_Buffers           MPU_x_دھارا_بفر_Get_Static_Buffers
            #تعریف_کرو x_دھارا_بفر_Send_From_ISR                MPU_x_دھارا_بفر_Send_From_ISR
            #تعریف_کرو x_دھارا_بفر_Receive_From_ISR             MPU_x_دھارا_بفر_Receive_From_ISR
            #تعریف_کرو x_دھارا_بفر_Send_Completed_From_ISR       MPU_x_دھارا_بفر_Send_Completed_From_ISR
            #تعریف_کرو x_دھارا_بفر_Receive_Completed_From_ISR    MPU_x_دھارا_بفر_Receive_Completed_From_ISR
            #تعریف_کرو x_دھارا_بفر_Reset_From_ISR               MPU_x_دھارا_بفر_Reset_From_ISR
        #جے_انت /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

        #إذا ( ( configUSE_MPU_WRAPPERS_V1 == 0 ) && ( configENABLE_ACCESS_CONTROL_LIST == 1 ) )

            #تعریف_کرو v_Grant_Access_منزل_Task( xTask, x_Task_منزل_Grant_Access )                        v_Grant_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_Task_منزل_Grant_Access ) )
            #تعریف_کرو v_Revoke_Access_منزل_Task( xTask, x_Task_منزل_Revoke_Access )                      v_Revoke_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_Task_منزل_Revoke_Access ) )

            #تعریف_کرو v_Grant_Access_منزل_Semaphore( xTask, x_Semaphore_منزل_Grant_Access )              v_Grant_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_Semaphore_منزل_Grant_Access ) )
            #تعریف_کرو v_Revoke_Access_منزل_Semaphore( xTask, x_Semaphore_منزل_Revoke_Access )            v_Revoke_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_Semaphore_منزل_Revoke_Access ) )

            #تعریف_کرو v_Grant_Access_منزل_قطار( xTask, x_قطار_منزل_Grant_Access )                      v_Grant_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_قطار_منزل_Grant_Access ) )
            #تعریف_کرو v_Revoke_Access_منزل_قطار( xTask, x_قطار_منزل_Revoke_Access )                    v_Revoke_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_قطار_منزل_Revoke_Access ) )

            #تعریف_کرو v_Grant_Access_منزل_قطار_سیٹ( xTask, x_قطار_سیٹ_منزل_Grant_Access )                v_Grant_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_قطار_سیٹ_منزل_Grant_Access ) )
            #تعریف_کرو v_Revoke_Access_منزل_قطار_سیٹ( xTask, x_قطار_سیٹ_منزل_Revoke_Access )              v_Revoke_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_قطار_سیٹ_منزل_Revoke_Access ) )

            #تعریف_کرو v_Grant_Access_منزل_Event_گروہ( xTask, x_Event_گروہ_منزل_Grant_Access )            v_Grant_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_Event_گروہ_منزل_Grant_Access ) )
            #تعریف_کرو v_Revoke_Access_منزل_Event_گروہ( xTask, x_Event_گروہ_منزل_Revoke_Access )          v_Revoke_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_Event_گروہ_منزل_Revoke_Access ) )

            #تعریف_کرو v_Grant_Access_منزل_دھارا_بفر( xTask, x_دھارا_بفر_منزل_Grant_Access )        v_Grant_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_دھارا_بفر_منزل_Grant_Access ) )
            #تعریف_کرو v_Revoke_Access_منزل_دھارا_بفر( xTask, x_دھارا_بفر_منزل_Revoke_Access )      v_Revoke_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_دھارا_بفر_منزل_Revoke_Access ) )

            #تعریف_کرو v_Grant_Access_منزل_سنیہا_بفر( xTask, x_سنیہا_بفر_منزل_Grant_Access )      v_Grant_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_سنیہا_بفر_منزل_Grant_Access ) )
            #تعریف_کرو v_Revoke_Access_منزل_سنیہا_بفر( xTask, x_سنیہا_بفر_منزل_Revoke_Access )    v_Revoke_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_سنیہا_بفر_منزل_Revoke_Access ) )

            #تعریف_کرو v_Grant_Access_منزل_Timer( xTask, x_Timer_منزل_Grant_Access )                      v_Grant_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_Timer_منزل_Grant_Access ) )
            #تعریف_کرو v_Revoke_Access_منزل_Timer( xTask, x_Timer_منزل_Revoke_Access )                    v_Revoke_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_Timer_منزل_Revoke_Access ) )

        #جے_انت /* #if ( ( configUSE_MPU_WRAPPERS_V1 == 0 ) && ( configENABLE_ACCESS_CONTROL_LIST == 1 ) ) */

    #جے_انت /* MPU_WRAPPERS_INCLUDED_FROM_API_FILE */

    #تعریف_کرو PRIVILEGED_FUNCTION     __attribute__( ( section( "privileged_functions" ) ) )
    #تعریف_کرو PRIVILEGED_انکڑے         __attribute__( ( section( "privileged_data" ) ) )
    #تعریف_کرو FREERTOS_SYSTEM_CALL    __attribute__( ( section( "freertos_system_calls" ) ) )

#وإلا /* portUSING_MPU_WRAPPERS */

    #تعریف_کرو PRIVILEGED_FUNCTION
    #تعریف_کرو PRIVILEGED_انکڑے
    #تعریف_کرو FREERTOS_SYSTEM_CALL

#جے_انت /* portUSING_MPU_WRAPPERS */


#جے_انت /* MPU_WRAPPERS_H */
