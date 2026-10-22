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

#جيڪڏهن_وصف_نه_ٿيل MPU_WRAPPERS_H
#وصف_ڪريو MPU_WRAPPERS_H

/* This file redefines API functions to be called through a wrapper macro, but
 * only for ports that are using the MPU. */
#إذا ( پورٽ_USING_MPU_WRAPPERS == 1 )

/* MPU_WRAPPERS_INCLUDED_FROM_API_FILE will be defined when this file is
 * included from queue.c or task.c to prevent it from having an effect within
 * those files. */
    #جيڪڏهن_وصف_نه_ٿيل MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/*
 * Map standard (non MPU) API functions to equivalents that start
 * "MPU_".  This will cause the application code to call the MPU_
 * version, which wraps the non-MPU version with privilege promoting
 * then demoting code, so the kernel code always runs will full
 * privileges.
 */

/* Map standard task.h API functions to the MPU equivalents. */
        #وصف_ڪريو vTaskDelay                            MPU_vTaskDelay
        #وصف_ڪريو xTaskDelayUntil                       MPU_xTaskDelayUntil
        #وصف_ڪريو xTaskAbortDelay                       MPU_xTaskAbortDelay
        #وصف_ڪريو ux_Task_ترجيح_Get                     MPU_ux_Task_ترجيح_Get
        #وصف_ڪريو eTaskGetState                         MPU_eTaskGetState
        #وصف_ڪريو vTaskGetInfo                          MPU_vTaskGetInfo
        #وصف_ڪريو vTaskSuspend                          MPU_vTaskSuspend
        #وصف_ڪريو vTaskResume                           MPU_vTaskResume
        #وصف_ڪريو x_Task_Get_Tick_ڳڻپ                     MPU_x_Task_Get_Tick_ڳڻپ
        #وصف_ڪريو ux_Task_Get_عدد_جو_Tasks                MPU_ux_Task_Get_عدد_جو_Tasks
        #وصف_ڪريو ux_Task_Get_ڍير_مٿاهون_Water_Mark           MPU_ux_Task_Get_ڍير_مٿاهون_Water_Mark
        #وصف_ڪريو ux_Task_Get_ڍير_مٿاهون_Water_Mark2          MPU_ux_Task_Get_ڍير_مٿاهون_Water_Mark2
        #وصف_ڪريو v_Task_سيٽ_Application_Task_ٽيگ            MPU_v_Task_سيٽ_Application_Task_ٽيگ
        #وصف_ڪريو x_Task_Get_Application_Task_ٽيگ            MPU_x_Task_Get_Application_Task_ٽيگ
        #وصف_ڪريو v_Task_سيٽ_Thread_Local_Storage_اشارو     MPU_v_Task_سيٽ_Thread_Local_Storage_اشارو
        #وصف_ڪريو pv_Task_Get_Thread_Local_Storage_اشارو    MPU_pv_Task_Get_Thread_Local_Storage_اشارو
        #وصف_ڪريو xTaskGetIdleTaskHandle                MPU_xTaskGetIdleTaskHandle
        #وصف_ڪريو uxTaskGetSystemState                  MPU_uxTaskGetSystemState
        #وصف_ڪريو ul_Task_Get_Idle_لاڳيتو_وقت_Counter           MPU_ul_Task_Get_Idle_لاڳيتو_وقت_Counter
        #وصف_ڪريو ul_Task_Get_Idle_لاڳيتو_وقت_Percent           MPU_ul_Task_Get_Idle_لاڳيتو_وقت_Percent
        #وصف_ڪريو xTaskGenericNotify                    MPU_xTaskGenericNotify
        #وصف_ڪريو xTaskGenericNotifyWait                MPU_xTaskGenericNotifyWait
        #وصف_ڪريو ulTaskGenericNotifyTake               MPU_ulTaskGenericNotifyTake
        #وصف_ڪريو xTaskGenericNotifyStateClear          MPU_xTaskGenericNotifyStateClear
        #وصف_ڪريو ul_Task_Generic_Notify_قدر_Clear         MPU_ul_Task_Generic_Notify_قدر_Clear
        #وصف_ڪريو v_Task_سيٽ_وقت_ٻاهريون_State                  MPU_v_Task_سيٽ_وقت_ٻاهريون_State
        #وصف_ڪريو x_Task_Check_For_وقت_ٻاهريون                  MPU_x_Task_Check_For_وقت_ٻاهريون
        #وصف_ڪريو xTaskGetCurrentTaskHandle             MPU_xTaskGetCurrentTaskHandle
        #وصف_ڪريو xTaskGetSchedulerState                MPU_xTaskGetSchedulerState

        #إذا ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #وصف_ڪريو ul_Task_Get_لاڳيتو_وقت_Counter           MPU_ul_Task_Get_لاڳيتو_وقت_Counter
            #وصف_ڪريو ul_Task_Get_لاڳيتو_وقت_Percent           MPU_ul_Task_Get_لاڳيتو_وقت_Percent
        #جيڪڏهن_پورو /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Privileged only wrappers for Task APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
        #وصف_ڪريو xTaskCreate                              MPU_xTaskCreate
        #وصف_ڪريو xTaskCreateStatic                        MPU_xTaskCreateStatic
        #وصف_ڪريو vTaskDelete                              MPU_vTaskDelete
        #وصف_ڪريو v_Task_ترجيح_سيٽ                         MPU_v_Task_ترجيح_سيٽ
        #وصف_ڪريو xTaskGetHandle                           MPU_xTaskGetHandle
        #وصف_ڪريو xTaskCallApplicationTaskHook             MPU_xTaskCallApplicationTaskHook

        #إذا ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #وصف_ڪريو pcTaskGetName                        MPU_pcTaskGetName
            #وصف_ڪريو xTaskCreateRestricted                MPU_xTaskCreateRestricted
            #وصف_ڪريو xTaskCreateRestrictedStatic          MPU_xTaskCreateRestrictedStatic
            #وصف_ڪريو vTaskAllocateMPURegions              MPU_vTaskAllocateMPURegions
            #وصف_ڪريو xTaskGetStaticBuffers                MPU_xTaskGetStaticBuffers
            #وصف_ڪريو ux_Task_ترجيح_Get_From_ISR             MPU_ux_Task_ترجيح_Get_From_ISR
            #وصف_ڪريو ux_Task_بنياد_ترجيح_Get                MPU_ux_Task_بنياد_ترجيح_Get
            #وصف_ڪريو ux_Task_بنياد_ترجيح_Get_From_ISR         MPU_ux_Task_بنياد_ترجيح_Get_From_ISR
            #وصف_ڪريو xTaskResumeFromISR                   MPU_xTaskResumeFromISR
            #وصف_ڪريو x_Task_Get_Application_Task_ٽيگ_From_ISR    MPU_x_Task_Get_Application_Task_ٽيگ_From_ISR
            #وصف_ڪريو xTaskGenericNotifyFromISR            MPU_xTaskGenericNotifyFromISR
            #وصف_ڪريو vTaskGenericNotifyGiveFromISR        MPU_vTaskGenericNotifyGiveFromISR
        #جيڪڏهن_پورو /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Map standard queue.h API functions to the MPU equivalents. */
        #وصف_ڪريو x_قطار_Generic_Send            MPU_x_قطار_Generic_Send
        #وصف_ڪريو x_قطار_Receive                MPU_x_قطار_Receive
        #وصف_ڪريو x_قطار_Peek                   MPU_x_قطار_Peek
        #وصف_ڪريو x_قطار_Semaphore_Take          MPU_x_قطار_Semaphore_Take
        #وصف_ڪريو ux_قطار_Messages_انتظار       MPU_ux_قطار_Messages_انتظار
        #وصف_ڪريو ux_قطار_Spaces_موجود       MPU_ux_قطار_Spaces_موجود
        #وصف_ڪريو x_قطار_Get_Mutex_Holder         MPU_x_قطار_Get_Mutex_Holder
        #وصف_ڪريو x_قطار_Take_Mutex_Recursive     MPU_x_قطار_Take_Mutex_Recursive
        #وصف_ڪريو x_قطار_Give_Mutex_Recursive     MPU_x_قطار_Give_Mutex_Recursive
        #وصف_ڪريو x_قطار_جوڙيو_منزل_سيٽ               MPU_x_قطار_جوڙيو_منزل_سيٽ
        #وصف_ڪريو x_قطار_Select_From_سيٽ          MPU_x_قطار_Select_From_سيٽ

        #إذا ( config_قطار_REGISTRY_سائيز > 0 )
            #وصف_ڪريو v_قطار_جوڙيو_منزل_Registry      MPU_v_قطار_جوڙيو_منزل_Registry
            #وصف_ڪريو v_قطار_Unregister_قطار    MPU_v_قطار_Unregister_قطار
            #وصف_ڪريو pc_قطار_Get_Name           MPU_pc_قطار_Get_Name
        #جيڪڏهن_پورو /* #if ( configQUEUE_REGISTRY_SIZE > 0 ) */

/* Privileged only wrappers for Queue APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
        #وصف_ڪريو v_قطار_Delete                           MPU_v_قطار_Delete
        #وصف_ڪريو x_قطار_Create_Mutex                      MPU_x_قطار_Create_Mutex
        #وصف_ڪريو x_قطار_Create_Mutex_Static                MPU_x_قطار_Create_Mutex_Static
        #وصف_ڪريو x_قطار_Create_Counting_Semaphore          MPU_x_قطار_Create_Counting_Semaphore
        #وصف_ڪريو x_قطار_Create_Counting_Semaphore_Static    MPU_x_قطار_Create_Counting_Semaphore_Static
        #وصف_ڪريو x_قطار_Generic_Create                    MPU_x_قطار_Generic_Create
        #وصف_ڪريو x_قطار_Generic_Create_Static              MPU_x_قطار_Generic_Create_Static
        #وصف_ڪريو x_قطار_Generic_Reset                     MPU_x_قطار_Generic_Reset
        #وصف_ڪريو x_قطار_Create_سيٽ                        MPU_x_قطار_Create_سيٽ
        #وصف_ڪريو x_قطار_Remove_From_سيٽ                    MPU_x_قطار_Remove_From_سيٽ

        #إذا ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #وصف_ڪريو x_قطار_Generic_Get_Static_Buffers      MPU_x_قطار_Generic_Get_Static_Buffers
            #وصف_ڪريو x_قطار_Generic_Send_From_ISR           MPU_x_قطار_Generic_Send_From_ISR
            #وصف_ڪريو x_قطار_Give_From_ISR                  MPU_x_قطار_Give_From_ISR
            #وصف_ڪريو x_قطار_Peek_From_ISR                  MPU_x_قطار_Peek_From_ISR
            #وصف_ڪريو x_قطار_Receive_From_ISR               MPU_x_قطار_Receive_From_ISR
            #وصف_ڪريو x_قطار_Is_قطار_خالي_جايون_From_ISR          MPU_x_قطار_Is_قطار_خالي_جايون_From_ISR
            #وصف_ڪريو x_قطار_Is_قطار_ڀريل_From_ISR           MPU_x_قطار_Is_قطار_ڀريل_From_ISR
            #وصف_ڪريو ux_قطار_Messages_انتظار_From_ISR      MPU_ux_قطار_Messages_انتظار_From_ISR
            #وصف_ڪريو x_قطار_Get_Mutex_Holder_From_ISR        MPU_x_قطار_Get_Mutex_Holder_From_ISR
            #وصف_ڪريو x_قطار_Select_From_سيٽ_From_ISR         MPU_x_قطار_Select_From_سيٽ_From_ISR
        #جيڪڏهن_پورو /* if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Map standard timer.h API functions to the MPU equivalents. */
        #وصف_ڪريو pvTimerGetTimerID                 MPU_pvTimerGetTimerID
        #وصف_ڪريو v_Timer_سيٽ_Timer_ID                  MPU_v_Timer_سيٽ_Timer_ID
        #وصف_ڪريو x_Timer_Is_Timer_سرگرم               MPU_x_Timer_Is_Timer_سرگرم
        #وصف_ڪريو xTimerGetTimerDaemonTaskHandle    MPU_xTimerGetTimerDaemonTaskHandle
        #وصف_ڪريو xTimerGenericCommandFromTask      MPU_xTimerGenericCommandFromTask
        #وصف_ڪريو pcTimerGetName                    MPU_pcTimerGetName
        #وصف_ڪريو v_Timer_سيٽ_Reload_گهڻائي               MPU_v_Timer_سيٽ_Reload_گهڻائي
        #وصف_ڪريو ux_Timer_Get_Reload_گهڻائي              MPU_ux_Timer_Get_Reload_گهڻائي
        #وصف_ڪريو xTimerGetPeriod                   MPU_xTimerGetPeriod
        #وصف_ڪريو x_Timer_Get_Expiry_وقت               MPU_x_Timer_Get_Expiry_وقت

/* Privileged only wrappers for Timer APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
        #إذا ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #وصف_ڪريو x_Timer_Get_Reload_گهڻائي            MPU_x_Timer_Get_Reload_گهڻائي
            #وصف_ڪريو xTimerCreate                   MPU_xTimerCreate
            #وصف_ڪريو xTimerCreateStatic             MPU_xTimerCreateStatic
            #وصف_ڪريو x_Timer_Get_Static_بفر          MPU_x_Timer_Get_Static_بفر
            #وصف_ڪريو xTimerGenericCommandFromISR    MPU_xTimerGenericCommandFromISR
        #جيڪڏهن_پورو /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Map standard event_group.h API functions to the MPU equivalents. */
        #وصف_ڪريو x_Event_گروهه_Wait_بٽ          MPU_x_Event_گروهه_Wait_بٽ
        #وصف_ڪريو x_Event_گروهه_Clear_بٽ         MPU_x_Event_گروهه_Clear_بٽ
        #وصف_ڪريو x_Event_گروهه_سيٽ_بٽ           MPU_x_Event_گروهه_سيٽ_بٽ
        #وصف_ڪريو x_Event_گروهه_Sync              MPU_x_Event_گروهه_Sync

        #إذا ( ( configUSE_TRACE_FACILITY == 1 ) && ( configUSE_MPU_WRAPPERS_V1 == 0 ) )
            #وصف_ڪريو ux_Event_گروهه_Get_عدد    MPU_ux_Event_گروهه_Get_عدد
            #وصف_ڪريو v_Event_گروهه_سيٽ_عدد     MPU_v_Event_گروهه_سيٽ_عدد
        #جيڪڏهن_پورو /* #if ( ( configUSE_TRACE_FACILITY == 1 ) && ( configUSE_MPU_WRAPPERS_V1 == 0 ) ) */

/* Privileged only wrappers for Event Group APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
        #وصف_ڪريو x_Event_گروهه_Create                  MPU_x_Event_گروهه_Create
        #وصف_ڪريو x_Event_گروهه_Create_Static            MPU_x_Event_گروهه_Create_Static
        #وصف_ڪريو v_Event_گروهه_Delete                  MPU_v_Event_گروهه_Delete

        #إذا ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #وصف_ڪريو x_Event_گروهه_Get_Static_بفر     MPU_x_Event_گروهه_Get_Static_بفر
            #وصف_ڪريو x_Event_گروهه_Clear_بٽ_From_ISR    MPU_x_Event_گروهه_Clear_بٽ_From_ISR
            #وصف_ڪريو x_Event_گروهه_سيٽ_بٽ_From_ISR      MPU_x_Event_گروهه_سيٽ_بٽ_From_ISR
            #وصف_ڪريو x_Event_گروهه_Get_بٽ_From_ISR      MPU_x_Event_گروهه_Get_بٽ_From_ISR
        #جيڪڏهن_پورو /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Map standard message/stream_buffer.h API functions to the MPU
 * equivalents. */
        #وصف_ڪريو x_وهڪرو_بفر_Send                      MPU_x_وهڪرو_بفر_Send
        #وصف_ڪريو x_وهڪرو_بفر_Receive                   MPU_x_وهڪرو_بفر_Receive
        #وصف_ڪريو x_وهڪرو_بفر_Is_ڀريل                    MPU_x_وهڪرو_بفر_Is_ڀريل
        #وصف_ڪريو x_وهڪرو_بفر_Is_خالي_جايون                   MPU_x_وهڪرو_بفر_Is_خالي_جايون
        #وصف_ڪريو x_وهڪرو_بفر_Spaces_موجود           MPU_x_وهڪرو_بفر_Spaces_موجود
        #وصف_ڪريو x_وهڪرو_بفر_بائيٽون_موجود            MPU_x_وهڪرو_بفر_بائيٽون_موجود
        #وصف_ڪريو x_وهڪرو_بفر_سيٽ_Trigger_Level           MPU_x_وهڪرو_بفر_سيٽ_Trigger_Level
        #وصف_ڪريو x_وهڪرو_بفر_ايندڙ_پيغام_ڊيگهائي_بائيٽون    MPU_x_وهڪرو_بفر_ايندڙ_پيغام_ڊيگهائي_بائيٽون

/* Privileged only wrappers for Stream Buffer APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */

        #وصف_ڪريو x_وهڪرو_بفر_Generic_Create                  MPU_x_وهڪرو_بفر_Generic_Create
        #وصف_ڪريو x_وهڪرو_بفر_Generic_Create_Static            MPU_x_وهڪرو_بفر_Generic_Create_Static
        #وصف_ڪريو v_وهڪرو_بفر_Delete                         MPU_v_وهڪرو_بفر_Delete
        #وصف_ڪريو x_وهڪرو_بفر_Reset                          MPU_x_وهڪرو_بفر_Reset

        #إذا ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #وصف_ڪريو x_وهڪرو_بفر_Get_Static_Buffers           MPU_x_وهڪرو_بفر_Get_Static_Buffers
            #وصف_ڪريو x_وهڪرو_بفر_Send_From_ISR                MPU_x_وهڪرو_بفر_Send_From_ISR
            #وصف_ڪريو x_وهڪرو_بفر_Receive_From_ISR             MPU_x_وهڪرو_بفر_Receive_From_ISR
            #وصف_ڪريو x_وهڪرو_بفر_Send_Completed_From_ISR       MPU_x_وهڪرو_بفر_Send_Completed_From_ISR
            #وصف_ڪريو x_وهڪرو_بفر_Receive_Completed_From_ISR    MPU_x_وهڪرو_بفر_Receive_Completed_From_ISR
            #وصف_ڪريو x_وهڪرو_بفر_Reset_From_ISR               MPU_x_وهڪرو_بفر_Reset_From_ISR
        #جيڪڏهن_پورو /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

        #إذا ( ( configUSE_MPU_WRAPPERS_V1 == 0 ) && ( configENABLE_ACCESS_CONTROL_LIST == 1 ) )

            #وصف_ڪريو v_Grant_Access_منزل_Task( xTask, x_Task_منزل_Grant_Access )                        v_Grant_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_Task_منزل_Grant_Access ) )
            #وصف_ڪريو v_Revoke_Access_منزل_Task( xTask, x_Task_منزل_Revoke_Access )                      v_Revoke_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_Task_منزل_Revoke_Access ) )

            #وصف_ڪريو v_Grant_Access_منزل_Semaphore( xTask, x_Semaphore_منزل_Grant_Access )              v_Grant_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_Semaphore_منزل_Grant_Access ) )
            #وصف_ڪريو v_Revoke_Access_منزل_Semaphore( xTask, x_Semaphore_منزل_Revoke_Access )            v_Revoke_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_Semaphore_منزل_Revoke_Access ) )

            #وصف_ڪريو v_Grant_Access_منزل_قطار( xTask, x_قطار_منزل_Grant_Access )                      v_Grant_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_قطار_منزل_Grant_Access ) )
            #وصف_ڪريو v_Revoke_Access_منزل_قطار( xTask, x_قطار_منزل_Revoke_Access )                    v_Revoke_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_قطار_منزل_Revoke_Access ) )

            #وصف_ڪريو v_Grant_Access_منزل_قطار_سيٽ( xTask, x_قطار_سيٽ_منزل_Grant_Access )                v_Grant_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_قطار_سيٽ_منزل_Grant_Access ) )
            #وصف_ڪريو v_Revoke_Access_منزل_قطار_سيٽ( xTask, x_قطار_سيٽ_منزل_Revoke_Access )              v_Revoke_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_قطار_سيٽ_منزل_Revoke_Access ) )

            #وصف_ڪريو v_Grant_Access_منزل_Event_گروهه( xTask, x_Event_گروهه_منزل_Grant_Access )            v_Grant_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_Event_گروهه_منزل_Grant_Access ) )
            #وصف_ڪريو v_Revoke_Access_منزل_Event_گروهه( xTask, x_Event_گروهه_منزل_Revoke_Access )          v_Revoke_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_Event_گروهه_منزل_Revoke_Access ) )

            #وصف_ڪريو v_Grant_Access_منزل_وهڪرو_بفر( xTask, x_وهڪرو_بفر_منزل_Grant_Access )        v_Grant_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_وهڪرو_بفر_منزل_Grant_Access ) )
            #وصف_ڪريو v_Revoke_Access_منزل_وهڪرو_بفر( xTask, x_وهڪرو_بفر_منزل_Revoke_Access )      v_Revoke_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_وهڪرو_بفر_منزل_Revoke_Access ) )

            #وصف_ڪريو v_Grant_Access_منزل_پيغام_بفر( xTask, x_پيغام_بفر_منزل_Grant_Access )      v_Grant_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_پيغام_بفر_منزل_Grant_Access ) )
            #وصف_ڪريو v_Revoke_Access_منزل_پيغام_بفر( xTask, x_پيغام_بفر_منزل_Revoke_Access )    v_Revoke_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_پيغام_بفر_منزل_Revoke_Access ) )

            #وصف_ڪريو v_Grant_Access_منزل_Timer( xTask, x_Timer_منزل_Grant_Access )                      v_Grant_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_Timer_منزل_Grant_Access ) )
            #وصف_ڪريو v_Revoke_Access_منزل_Timer( xTask, x_Timer_منزل_Revoke_Access )                    v_Revoke_Access_منزل_Kernel_Object( ( xTask ), ( int32_t ) ( x_Timer_منزل_Revoke_Access ) )

        #جيڪڏهن_پورو /* #if ( ( configUSE_MPU_WRAPPERS_V1 == 0 ) && ( configENABLE_ACCESS_CONTROL_LIST == 1 ) ) */

    #جيڪڏهن_پورو /* MPU_WRAPPERS_INCLUDED_FROM_API_FILE */

    #وصف_ڪريو PRIVILEGED_FUNCTION     __attribute__( ( section( "privileged_functions" ) ) )
    #وصف_ڪريو PRIVILEGED_معلومات         __attribute__( ( section( "privileged_data" ) ) )
    #وصف_ڪريو FREERTOS_SYSTEM_CALL    __attribute__( ( section( "freertos_system_calls" ) ) )

#وإلا /* portUSING_MPU_WRAPPERS */

    #وصف_ڪريو PRIVILEGED_FUNCTION
    #وصف_ڪريو PRIVILEGED_معلومات
    #وصف_ڪريو FREERTOS_SYSTEM_CALL

#جيڪڏهن_پورو /* portUSING_MPU_WRAPPERS */


#جيڪڏهن_پورو /* MPU_WRAPPERS_H */
