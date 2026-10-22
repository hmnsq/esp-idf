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

#నిర్వచితం_కాకపోతే MPU_WRAPPERS_H
#నిర్వచించు MPU_WRAPPERS_H

/* This file redefines API functions to be called through a wrapper macro, but
 * only for ports that are using the MPU. */
#ఒకవేళ ( పోర్టు_USING_MPU_WRAPPERS == 1 )

/* MPU_WRAPPERS_INCLUDED_FROM_API_FILE will be defined when this file is
 * included from queue.c or task.c to prevent it from having an effect within
 * those files. */
    #నిర్వచితం_కాకపోతే MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/*
 * Map standard (non MPU) API functions to equivalents that start
 * "MPU_".  This will cause the application code to call the MPU_
 * version, which wraps the non-MPU version with privilege promoting
 * then demoting code, so the kernel code always runs will full
 * privileges.
 */

/* Map standard task.h API functions to the MPU equivalents. */
        #నిర్వచించు vTaskDelay                            MPU_vTaskDelay
        #నిర్వచించు xTaskDelayUntil                       MPU_xTaskDelayUntil
        #నిర్వచించు xTaskAbortDelay                       MPU_xTaskAbortDelay
        #నిర్వచించు ux_Task_ప్రాధాన్యత_Get                     MPU_ux_Task_ప్రాధాన్యత_Get
        #నిర్వచించు eTaskGetState                         MPU_eTaskGetState
        #నిర్వచించు vTaskGetInfo                          MPU_vTaskGetInfo
        #నిర్వచించు vTaskSuspend                          MPU_vTaskSuspend
        #నిర్వచించు vTaskResume                           MPU_vTaskResume
        #నిర్వచించు x_Task_Get_Tick_లెక్క                     MPU_x_Task_Get_Tick_లెక్క
        #నిర్వచించు ux_Task_Get_సంఖ్య_యొక్క_Tasks                MPU_ux_Task_Get_సంఖ్య_యొక్క_Tasks
        #నిర్వచించు ux_Task_Get_దొంతర_ఎగువ_Water_Mark           MPU_ux_Task_Get_దొంతర_ఎగువ_Water_Mark
        #నిర్వచించు ux_Task_Get_దొంతర_ఎగువ_Water_Mark2          MPU_ux_Task_Get_దొంతర_ఎగువ_Water_Mark2
        #నిర్వచించు v_Task_సమితి_Application_Task_ట్యాగ్            MPU_v_Task_సమితి_Application_Task_ట్యాగ్
        #నిర్వచించు x_Task_Get_Application_Task_ట్యాగ్            MPU_x_Task_Get_Application_Task_ట్యాగ్
        #నిర్వచించు v_Task_సమితి_Thread_Local_Storage_సూచిక     MPU_v_Task_సమితి_Thread_Local_Storage_సూచిక
        #నిర్వచించు pv_Task_Get_Thread_Local_Storage_సూచిక    MPU_pv_Task_Get_Thread_Local_Storage_సూచిక
        #నిర్వచించు xTaskGetIdleTaskHandle                MPU_xTaskGetIdleTaskHandle
        #నిర్వచించు uxTaskGetSystemState                  MPU_uxTaskGetSystemState
        #నిర్వచించు ul_Task_Get_Idle_నిరంతరం_సమయం_Counter           MPU_ul_Task_Get_Idle_నిరంతరం_సమయం_Counter
        #నిర్వచించు ul_Task_Get_Idle_నిరంతరం_సమయం_Percent           MPU_ul_Task_Get_Idle_నిరంతరం_సమయం_Percent
        #నిర్వచించు xTaskGenericNotify                    MPU_xTaskGenericNotify
        #నిర్వచించు xTaskGenericNotifyWait                MPU_xTaskGenericNotifyWait
        #నిర్వచించు ulTaskGenericNotifyTake               MPU_ulTaskGenericNotifyTake
        #నిర్వచించు xTaskGenericNotifyStateClear          MPU_xTaskGenericNotifyStateClear
        #నిర్వచించు ul_Task_Generic_Notify_విలువ_Clear         MPU_ul_Task_Generic_Notify_విలువ_Clear
        #నిర్వచించు v_Task_సమితి_సమయం_బహిర్గతం_State                  MPU_v_Task_సమితి_సమయం_బహిర్గతం_State
        #నిర్వచించు x_Task_Check_For_సమయం_బహిర్గతం                  MPU_x_Task_Check_For_సమయం_బహిర్గతం
        #నిర్వచించు xTaskGetCurrentTaskHandle             MPU_xTaskGetCurrentTaskHandle
        #నిర్వచించు xTaskGetSchedulerState                MPU_xTaskGetSchedulerState

        #ఒకవేళ ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #నిర్వచించు ul_Task_Get_నిరంతరం_సమయం_Counter           MPU_ul_Task_Get_నిరంతరం_సమయం_Counter
            #నిర్వచించు ul_Task_Get_నిరంతరం_సమయం_Percent           MPU_ul_Task_Get_నిరంతరం_సమయం_Percent
        #అయితే_ముగింపు /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Privileged only wrappers for Task APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
        #నిర్వచించు xTaskCreate                              MPU_xTaskCreate
        #నిర్వచించు xTaskCreateStatic                        MPU_xTaskCreateStatic
        #నిర్వచించు vTaskDelete                              MPU_vTaskDelete
        #నిర్వచించు v_Task_ప్రాధాన్యత_సమితి                         MPU_v_Task_ప్రాధాన్యత_సమితి
        #నిర్వచించు xTaskGetHandle                           MPU_xTaskGetHandle
        #నిర్వచించు xTaskCallApplicationTaskHook             MPU_xTaskCallApplicationTaskHook

        #ఒకవేళ ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #నిర్వచించు pcTaskGetName                        MPU_pcTaskGetName
            #నిర్వచించు xTaskCreateRestricted                MPU_xTaskCreateRestricted
            #నిర్వచించు xTaskCreateRestrictedStatic          MPU_xTaskCreateRestrictedStatic
            #నిర్వచించు vTaskAllocateMPURegions              MPU_vTaskAllocateMPURegions
            #నిర్వచించు xTaskGetStaticBuffers                MPU_xTaskGetStaticBuffers
            #నిర్వచించు ux_Task_ప్రాధాన్యత_Get_From_ISR             MPU_ux_Task_ప్రాధాన్యత_Get_From_ISR
            #నిర్వచించు ux_Task_పునాది_ప్రాధాన్యత_Get                MPU_ux_Task_పునాది_ప్రాధాన్యత_Get
            #నిర్వచించు ux_Task_పునాది_ప్రాధాన్యత_Get_From_ISR         MPU_ux_Task_పునాది_ప్రాధాన్యత_Get_From_ISR
            #నిర్వచించు xTaskResumeFromISR                   MPU_xTaskResumeFromISR
            #నిర్వచించు x_Task_Get_Application_Task_ట్యాగ్_From_ISR    MPU_x_Task_Get_Application_Task_ట్యాగ్_From_ISR
            #నిర్వచించు xTaskGenericNotifyFromISR            MPU_xTaskGenericNotifyFromISR
            #నిర్వచించు vTaskGenericNotifyGiveFromISR        MPU_vTaskGenericNotifyGiveFromISR
        #అయితే_ముగింపు /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Map standard queue.h API functions to the MPU equivalents. */
        #నిర్వచించు x_వరుస_Generic_Send            MPU_x_వరుస_Generic_Send
        #నిర్వచించు x_వరుస_Receive                MPU_x_వరుస_Receive
        #నిర్వచించు x_వరుస_Peek                   MPU_x_వరుస_Peek
        #నిర్వచించు x_వరుస_Semaphore_Take          MPU_x_వరుస_Semaphore_Take
        #నిర్వచించు ux_వరుస_Messages_నిరీక్షణ       MPU_ux_వరుస_Messages_నిరీక్షణ
        #నిర్వచించు ux_వరుస_Spaces_అందుబాటు       MPU_ux_వరుస_Spaces_అందుబాటు
        #నిర్వచించు x_వరుస_Get_Mutex_Holder         MPU_x_వరుస_Get_Mutex_Holder
        #నిర్వచించు x_వరుస_Take_Mutex_Recursive     MPU_x_వరుస_Take_Mutex_Recursive
        #నిర్వచించు x_వరుస_Give_Mutex_Recursive     MPU_x_వరుస_Give_Mutex_Recursive
        #నిర్వచించు x_వరుస_కూడు_గమ్యం_సమితి               MPU_x_వరుస_కూడు_గమ్యం_సమితి
        #నిర్వచించు x_వరుస_Select_From_సమితి          MPU_x_వరుస_Select_From_సమితి

        #ఒకవేళ ( config_వరుస_REGISTRY_కొలత > 0 )
            #నిర్వచించు v_వరుస_కూడు_గమ్యం_Registry      MPU_v_వరుస_కూడు_గమ్యం_Registry
            #నిర్వచించు v_వరుస_Unregister_వరుస    MPU_v_వరుస_Unregister_వరుస
            #నిర్వచించు pc_వరుస_Get_Name           MPU_pc_వరుస_Get_Name
        #అయితే_ముగింపు /* #if ( configQUEUE_REGISTRY_SIZE > 0 ) */

/* Privileged only wrappers for Queue APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
        #నిర్వచించు v_వరుస_Delete                           MPU_v_వరుస_Delete
        #నిర్వచించు x_వరుస_Create_Mutex                      MPU_x_వరుస_Create_Mutex
        #నిర్వచించు x_వరుస_Create_Mutex_Static                MPU_x_వరుస_Create_Mutex_Static
        #నిర్వచించు x_వరుస_Create_Counting_Semaphore          MPU_x_వరుస_Create_Counting_Semaphore
        #నిర్వచించు x_వరుస_Create_Counting_Semaphore_Static    MPU_x_వరుస_Create_Counting_Semaphore_Static
        #నిర్వచించు x_వరుస_Generic_Create                    MPU_x_వరుస_Generic_Create
        #నిర్వచించు x_వరుస_Generic_Create_Static              MPU_x_వరుస_Generic_Create_Static
        #నిర్వచించు x_వరుస_Generic_Reset                     MPU_x_వరుస_Generic_Reset
        #నిర్వచించు x_వరుస_Create_సమితి                        MPU_x_వరుస_Create_సమితి
        #నిర్వచించు x_వరుస_Remove_From_సమితి                    MPU_x_వరుస_Remove_From_సమితి

        #ఒకవేళ ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #నిర్వచించు x_వరుస_Generic_Get_Static_Buffers      MPU_x_వరుస_Generic_Get_Static_Buffers
            #నిర్వచించు x_వరుస_Generic_Send_From_ISR           MPU_x_వరుస_Generic_Send_From_ISR
            #నిర్వచించు x_వరుస_Give_From_ISR                  MPU_x_వరుస_Give_From_ISR
            #నిర్వచించు x_వరుస_Peek_From_ISR                  MPU_x_వరుస_Peek_From_ISR
            #నిర్వచించు x_వరుస_Receive_From_ISR               MPU_x_వరుస_Receive_From_ISR
            #నిర్వచించు x_వరుస_Is_వరుస_ఖాళీలు_From_ISR          MPU_x_వరుస_Is_వరుస_ఖాళీలు_From_ISR
            #నిర్వచించు x_వరుస_Is_వరుస_నిండిన_From_ISR           MPU_x_వరుస_Is_వరుస_నిండిన_From_ISR
            #నిర్వచించు ux_వరుస_Messages_నిరీక్షణ_From_ISR      MPU_ux_వరుస_Messages_నిరీక్షణ_From_ISR
            #నిర్వచించు x_వరుస_Get_Mutex_Holder_From_ISR        MPU_x_వరుస_Get_Mutex_Holder_From_ISR
            #నిర్వచించు x_వరుస_Select_From_సమితి_From_ISR         MPU_x_వరుస_Select_From_సమితి_From_ISR
        #అయితే_ముగింపు /* if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Map standard timer.h API functions to the MPU equivalents. */
        #నిర్వచించు pvTimerGetTimerID                 MPU_pvTimerGetTimerID
        #నిర్వచించు v_Timer_సమితి_Timer_ID                  MPU_v_Timer_సమితి_Timer_ID
        #నిర్వచించు x_Timer_Is_Timer_చురుకైన               MPU_x_Timer_Is_Timer_చురుకైన
        #నిర్వచించు xTimerGetTimerDaemonTaskHandle    MPU_xTimerGetTimerDaemonTaskHandle
        #నిర్వచించు xTimerGenericCommandFromTask      MPU_xTimerGenericCommandFromTask
        #నిర్వచించు pcTimerGetName                    MPU_pcTimerGetName
        #నిర్వచించు v_Timer_సమితి_Reload_బహుళకం               MPU_v_Timer_సమితి_Reload_బహుళకం
        #నిర్వచించు ux_Timer_Get_Reload_బహుళకం              MPU_ux_Timer_Get_Reload_బహుళకం
        #నిర్వచించు xTimerGetPeriod                   MPU_xTimerGetPeriod
        #నిర్వచించు x_Timer_Get_Expiry_సమయం               MPU_x_Timer_Get_Expiry_సమయం

/* Privileged only wrappers for Timer APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
        #ఒకవేళ ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #నిర్వచించు x_Timer_Get_Reload_బహుళకం            MPU_x_Timer_Get_Reload_బహుళకం
            #నిర్వచించు xTimerCreate                   MPU_xTimerCreate
            #నిర్వచించు xTimerCreateStatic             MPU_xTimerCreateStatic
            #నిర్వచించు x_Timer_Get_Static_బఫరు          MPU_x_Timer_Get_Static_బఫరు
            #నిర్వచించు xTimerGenericCommandFromISR    MPU_xTimerGenericCommandFromISR
        #అయితే_ముగింపు /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Map standard event_group.h API functions to the MPU equivalents. */
        #నిర్వచించు x_Event_సమూహం_Wait_బిట్లు          MPU_x_Event_సమూహం_Wait_బిట్లు
        #నిర్వచించు x_Event_సమూహం_Clear_బిట్లు         MPU_x_Event_సమూహం_Clear_బిట్లు
        #నిర్వచించు x_Event_సమూహం_సమితి_బిట్లు           MPU_x_Event_సమూహం_సమితి_బిట్లు
        #నిర్వచించు x_Event_సమూహం_Sync              MPU_x_Event_సమూహం_Sync

        #ఒకవేళ ( ( configUSE_TRACE_FACILITY == 1 ) && ( configUSE_MPU_WRAPPERS_V1 == 0 ) )
            #నిర్వచించు ux_Event_సమూహం_Get_సంఖ్య    MPU_ux_Event_సమూహం_Get_సంఖ్య
            #నిర్వచించు v_Event_సమూహం_సమితి_సంఖ్య     MPU_v_Event_సమూహం_సమితి_సంఖ్య
        #అయితే_ముగింపు /* #if ( ( configUSE_TRACE_FACILITY == 1 ) && ( configUSE_MPU_WRAPPERS_V1 == 0 ) ) */

/* Privileged only wrappers for Event Group APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
        #నిర్వచించు x_Event_సమూహం_Create                  MPU_x_Event_సమూహం_Create
        #నిర్వచించు x_Event_సమూహం_Create_Static            MPU_x_Event_సమూహం_Create_Static
        #నిర్వచించు v_Event_సమూహం_Delete                  MPU_v_Event_సమూహం_Delete

        #ఒకవేళ ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #నిర్వచించు x_Event_సమూహం_Get_Static_బఫరు     MPU_x_Event_సమూహం_Get_Static_బఫరు
            #నిర్వచించు x_Event_సమూహం_Clear_బిట్లు_From_ISR    MPU_x_Event_సమూహం_Clear_బిట్లు_From_ISR
            #నిర్వచించు x_Event_సమూహం_సమితి_బిట్లు_From_ISR      MPU_x_Event_సమూహం_సమితి_బిట్లు_From_ISR
            #నిర్వచించు x_Event_సమూహం_Get_బిట్లు_From_ISR      MPU_x_Event_సమూహం_Get_బిట్లు_From_ISR
        #అయితే_ముగింపు /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

/* Map standard message/stream_buffer.h API functions to the MPU
 * equivalents. */
        #నిర్వచించు x_ప్రవాహం_బఫరు_Send                      MPU_x_ప్రవాహం_బఫరు_Send
        #నిర్వచించు x_ప్రవాహం_బఫరు_Receive                   MPU_x_ప్రవాహం_బఫరు_Receive
        #నిర్వచించు x_ప్రవాహం_బఫరు_Is_నిండిన                    MPU_x_ప్రవాహం_బఫరు_Is_నిండిన
        #నిర్వచించు x_ప్రవాహం_బఫరు_Is_ఖాళీలు                   MPU_x_ప్రవాహం_బఫరు_Is_ఖాళీలు
        #నిర్వచించు x_ప్రవాహం_బఫరు_Spaces_అందుబాటు           MPU_x_ప్రవాహం_బఫరు_Spaces_అందుబాటు
        #నిర్వచించు x_ప్రవాహం_బఫరు_బైట్లు_అందుబాటు            MPU_x_ప్రవాహం_బఫరు_బైట్లు_అందుబాటు
        #నిర్వచించు x_ప్రవాహం_బఫరు_సమితి_Trigger_Level           MPU_x_ప్రవాహం_బఫరు_సమితి_Trigger_Level
        #నిర్వచించు x_ప్రవాహం_బఫరు_తదుపరి_సందేశం_నిడివి_బైట్లు    MPU_x_ప్రవాహం_బఫరు_తదుపరి_సందేశం_నిడివి_బైట్లు

/* Privileged only wrappers for Stream Buffer APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */

        #నిర్వచించు x_ప్రవాహం_బఫరు_Generic_Create                  MPU_x_ప్రవాహం_బఫరు_Generic_Create
        #నిర్వచించు x_ప్రవాహం_బఫరు_Generic_Create_Static            MPU_x_ప్రవాహం_బఫరు_Generic_Create_Static
        #నిర్వచించు v_ప్రవాహం_బఫరు_Delete                         MPU_v_ప్రవాహం_బఫరు_Delete
        #నిర్వచించు x_ప్రవాహం_బఫరు_Reset                          MPU_x_ప్రవాహం_బఫరు_Reset

        #ఒకవేళ ( configUSE_MPU_WRAPPERS_V1 == 0 )
            #నిర్వచించు x_ప్రవాహం_బఫరు_Get_Static_Buffers           MPU_x_ప్రవాహం_బఫరు_Get_Static_Buffers
            #నిర్వచించు x_ప్రవాహం_బఫరు_Send_From_ISR                MPU_x_ప్రవాహం_బఫరు_Send_From_ISR
            #నిర్వచించు x_ప్రవాహం_బఫరు_Receive_From_ISR             MPU_x_ప్రవాహం_బఫరు_Receive_From_ISR
            #నిర్వచించు x_ప్రవాహం_బఫరు_Send_Completed_From_ISR       MPU_x_ప్రవాహం_బఫరు_Send_Completed_From_ISR
            #నిర్వచించు x_ప్రవాహం_బఫరు_Receive_Completed_From_ISR    MPU_x_ప్రవాహం_బఫరు_Receive_Completed_From_ISR
            #నిర్వచించు x_ప్రవాహం_బఫరు_Reset_From_ISR               MPU_x_ప్రవాహం_బఫరు_Reset_From_ISR
        #అయితే_ముగింపు /* #if ( configUSE_MPU_WRAPPERS_V1 == 0 ) */

        #ఒకవేళ ( ( configUSE_MPU_WRAPPERS_V1 == 0 ) && ( configENABLE_ACCESS_CONTROL_LIST == 1 ) )

            #నిర్వచించు v_Grant_Access_గమ్యం_Task( xTask, x_Task_గమ్యం_Grant_Access )                        v_Grant_Access_గమ్యం_Kernel_Object( ( xTask ), ( int32_t ) ( x_Task_గమ్యం_Grant_Access ) )
            #నిర్వచించు v_Revoke_Access_గమ్యం_Task( xTask, x_Task_గమ్యం_Revoke_Access )                      v_Revoke_Access_గమ్యం_Kernel_Object( ( xTask ), ( int32_t ) ( x_Task_గమ్యం_Revoke_Access ) )

            #నిర్వచించు v_Grant_Access_గమ్యం_Semaphore( xTask, x_Semaphore_గమ్యం_Grant_Access )              v_Grant_Access_గమ్యం_Kernel_Object( ( xTask ), ( int32_t ) ( x_Semaphore_గమ్యం_Grant_Access ) )
            #నిర్వచించు v_Revoke_Access_గమ్యం_Semaphore( xTask, x_Semaphore_గమ్యం_Revoke_Access )            v_Revoke_Access_గమ్యం_Kernel_Object( ( xTask ), ( int32_t ) ( x_Semaphore_గమ్యం_Revoke_Access ) )

            #నిర్వచించు v_Grant_Access_గమ్యం_వరుస( xTask, x_వరుస_గమ్యం_Grant_Access )                      v_Grant_Access_గమ్యం_Kernel_Object( ( xTask ), ( int32_t ) ( x_వరుస_గమ్యం_Grant_Access ) )
            #నిర్వచించు v_Revoke_Access_గమ్యం_వరుస( xTask, x_వరుస_గమ్యం_Revoke_Access )                    v_Revoke_Access_గమ్యం_Kernel_Object( ( xTask ), ( int32_t ) ( x_వరుస_గమ్యం_Revoke_Access ) )

            #నిర్వచించు v_Grant_Access_గమ్యం_వరుస_సమితి( xTask, x_వరుస_సమితి_గమ్యం_Grant_Access )                v_Grant_Access_గమ్యం_Kernel_Object( ( xTask ), ( int32_t ) ( x_వరుస_సమితి_గమ్యం_Grant_Access ) )
            #నిర్వచించు v_Revoke_Access_గమ్యం_వరుస_సమితి( xTask, x_వరుస_సమితి_గమ్యం_Revoke_Access )              v_Revoke_Access_గమ్యం_Kernel_Object( ( xTask ), ( int32_t ) ( x_వరుస_సమితి_గమ్యం_Revoke_Access ) )

            #నిర్వచించు v_Grant_Access_గమ్యం_Event_సమూహం( xTask, x_Event_సమూహం_గమ్యం_Grant_Access )            v_Grant_Access_గమ్యం_Kernel_Object( ( xTask ), ( int32_t ) ( x_Event_సమూహం_గమ్యం_Grant_Access ) )
            #నిర్వచించు v_Revoke_Access_గమ్యం_Event_సమూహం( xTask, x_Event_సమూహం_గమ్యం_Revoke_Access )          v_Revoke_Access_గమ్యం_Kernel_Object( ( xTask ), ( int32_t ) ( x_Event_సమూహం_గమ్యం_Revoke_Access ) )

            #నిర్వచించు v_Grant_Access_గమ్యం_ప్రవాహం_బఫరు( xTask, x_ప్రవాహం_బఫరు_గమ్యం_Grant_Access )        v_Grant_Access_గమ్యం_Kernel_Object( ( xTask ), ( int32_t ) ( x_ప్రవాహం_బఫరు_గమ్యం_Grant_Access ) )
            #నిర్వచించు v_Revoke_Access_గమ్యం_ప్రవాహం_బఫరు( xTask, x_ప్రవాహం_బఫరు_గమ్యం_Revoke_Access )      v_Revoke_Access_గమ్యం_Kernel_Object( ( xTask ), ( int32_t ) ( x_ప్రవాహం_బఫరు_గమ్యం_Revoke_Access ) )

            #నిర్వచించు v_Grant_Access_గమ్యం_సందేశం_బఫరు( xTask, x_సందేశం_బఫరు_గమ్యం_Grant_Access )      v_Grant_Access_గమ్యం_Kernel_Object( ( xTask ), ( int32_t ) ( x_సందేశం_బఫరు_గమ్యం_Grant_Access ) )
            #నిర్వచించు v_Revoke_Access_గమ్యం_సందేశం_బఫరు( xTask, x_సందేశం_బఫరు_గమ్యం_Revoke_Access )    v_Revoke_Access_గమ్యం_Kernel_Object( ( xTask ), ( int32_t ) ( x_సందేశం_బఫరు_గమ్యం_Revoke_Access ) )

            #నిర్వచించు v_Grant_Access_గమ్యం_Timer( xTask, x_Timer_గమ్యం_Grant_Access )                      v_Grant_Access_గమ్యం_Kernel_Object( ( xTask ), ( int32_t ) ( x_Timer_గమ్యం_Grant_Access ) )
            #నిర్వచించు v_Revoke_Access_గమ్యం_Timer( xTask, x_Timer_గమ్యం_Revoke_Access )                    v_Revoke_Access_గమ్యం_Kernel_Object( ( xTask ), ( int32_t ) ( x_Timer_గమ్యం_Revoke_Access ) )

        #అయితే_ముగింపు /* #if ( ( configUSE_MPU_WRAPPERS_V1 == 0 ) && ( configENABLE_ACCESS_CONTROL_LIST == 1 ) ) */

    #అయితే_ముగింపు /* MPU_WRAPPERS_INCLUDED_FROM_API_FILE */

    #నిర్వచించు PRIVILEGED_FUNCTION     __attribute__( ( section( "privileged_functions" ) ) )
    #నిర్వచించు PRIVILEGED_దత్తాంశం         __attribute__( ( section( "privileged_data" ) ) )
    #నిర్వచించు FREERTOS_SYSTEM_CALL    __attribute__( ( section( "freertos_system_calls" ) ) )

#లేకపోతే /* portUSING_MPU_WRAPPERS */

    #నిర్వచించు PRIVILEGED_FUNCTION
    #నిర్వచించు PRIVILEGED_దత్తాంశం
    #నిర్వచించు FREERTOS_SYSTEM_CALL

#అయితే_ముగింపు /* portUSING_MPU_WRAPPERS */


#అయితే_ముగింపు /* MPU_WRAPPERS_H */
