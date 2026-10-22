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

/*
 * When the MPU is used the standard (non MPU) API functions are mapped to
 * equivalents that start "MPU_", the prototypes for which are defined in this
 * header files.  This will cause the application code to call the MPU_ version
 * which wraps the non-MPU version with privilege promoting then demoting code,
 * so the kernel code always runs will full privileges.
 */


#నిర్వచితం_కాకపోతే MPU_PROTOTYPES_H
#నిర్వచించు MPU_PROTOTYPES_H

/* MPU versions of task.h API functions. */
పునాది_Type_t MPU_xTaskCreate( TaskFunction_t px_Task_సంకేతం,
                            స్థిరరాశి అక్షరం * స్థిరరాశి pcName,
                            స్థిరరాశి uint16_t us_దొంతర_లోతు,
                            శూన్య * స్థిరరాశి pvParameters,
                            UBaseType_t ux_ప్రాధాన్యత,
                            TaskHandle_t * స్థిరరాశి pxCreatedTask ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskCreateStatic( TaskFunction_t px_Task_సంకేతం,
                                    స్థిరరాశి అక్షరం * స్థిరరాశి pcName,
                                    స్థిరరాశి uint32_t ul_దొంతర_లోతు,
                                    శూన్య * స్థిరరాశి pvParameters,
                                    UBaseType_t ux_ప్రాధాన్యత,
                                    దొంతర_Type_t * స్థిరరాశి pux_దొంతర_బఫరు,
                                    StaticTask_t * స్థిరరాశి px_Task_బఫరు ) FREERTOS_SYSTEM_CALL;
శూన్య MPU_vTaskDelete( TaskHandle_t x_Task_గమ్యం_Delete ) FREERTOS_SYSTEM_CALL;
శూన్య MPU_vTaskDelay( స్థిరరాశి TickType_t x_Ticks_గమ్యం_Delay ) FREERTOS_SYSTEM_CALL;
పునాది_Type_t MPU_xTaskDelayUntil( TickType_t * స్థిరరాశి px_మునుపటి_Wake_సమయం,
                                స్థిరరాశి TickType_t x_సమయం_Increment ) FREERTOS_SYSTEM_CALL;
పునాది_Type_t MPU_xTaskAbortDelay( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_ప్రాధాన్యత_Get( స్థిరరాశి TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
eTaskState MPU_eTaskGetState( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
శూన్య MPU_vTaskGetInfo( TaskHandle_t xTask,
                       Task_స్థితి_t * px_Task_స్థితి,
                       పునాది_Type_t x_Get_Free_దొంతర_Space,
                       eTaskState eState ) FREERTOS_SYSTEM_CALL;
శూన్య MPU_v_Task_ప్రాధాన్యత_సమితి( TaskHandle_t xTask,
                           UBaseType_t ux_New_ప్రాధాన్యత ) FREERTOS_SYSTEM_CALL;
శూన్య MPU_vTaskSuspend( TaskHandle_t x_Task_గమ్యం_Suspend ) FREERTOS_SYSTEM_CALL;
శూన్య MPU_vTaskResume( TaskHandle_t x_Task_గమ్యం_Resume ) FREERTOS_SYSTEM_CALL;
శూన్య MPU_v_Task_ప్రారంభం_Scheduler( శూన్య ) FREERTOS_SYSTEM_CALL;
శూన్య MPU_vTaskSuspendAll( శూన్య ) FREERTOS_SYSTEM_CALL;
పునాది_Type_t MPU_xTaskResumeAll( శూన్య ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_x_Task_Get_Tick_లెక్క( శూన్య ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_Get_సంఖ్య_యొక్క_Tasks( శూన్య ) FREERTOS_SYSTEM_CALL;
అక్షరం * MPU_pcTaskGetName( TaskHandle_t x_Task_గమ్యం_Query ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetHandle( స్థిరరాశి అక్షరం * pc_Name_గమ్యం_Query ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_Get_దొంతర_ఎగువ_Water_Mark( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
config_దొంతర_లోతు_TYPE MPU_ux_Task_Get_దొంతర_ఎగువ_Water_Mark2( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
శూన్య MPU_v_Task_సమితి_Application_Task_ట్యాగ్( TaskHandle_t xTask,
                                     TaskHookFunction_t pxHookFunction ) FREERTOS_SYSTEM_CALL;
TaskHookFunction_t MPU_x_Task_Get_Application_Task_ట్యాగ్( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
శూన్య MPU_v_Task_సమితి_Thread_Local_Storage_సూచిక( TaskHandle_t x_Task_గమ్యం_సమితి,
                                            పునాది_Type_t x_సూచీ,
                                            శూన్య * pv_విలువ ) FREERTOS_SYSTEM_CALL;
శూన్య * MPU_pv_Task_Get_Thread_Local_Storage_సూచిక( TaskHandle_t x_Task_గమ్యం_Query,
                                               పునాది_Type_t x_సూచీ ) FREERTOS_SYSTEM_CALL;
పునాది_Type_t MPU_xTaskCallApplicationTaskHook( TaskHandle_t xTask,
                                             శూన్య * pvParameter ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetIdleTaskHandle( శూన్య ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxTaskGetSystemState( Task_స్థితి_t * స్థిరరాశి px_Task_స్థితి_Array,
                                      స్థిరరాశి UBaseType_t ux_Array_కొలత,
                                      config_నిరంతరం_సమయం_COUNTER_TYPE * స్థిరరాశి pul_మొత్తం_నిరంతరం_సమయం ) FREERTOS_SYSTEM_CALL;
config_నిరంతరం_సమయం_COUNTER_TYPE MPU_ul_Task_Get_Idle_నిరంతరం_సమయం_Counter( శూన్య ) FREERTOS_SYSTEM_CALL;
config_నిరంతరం_సమయం_COUNTER_TYPE MPU_ul_Task_Get_Idle_నిరంతరం_సమయం_Percent( శూన్య ) FREERTOS_SYSTEM_CALL;
శూన్య MPU_vTaskList( అక్షరం * pc_Write_బఫరు ) FREERTOS_SYSTEM_CALL;
శూన్య MPU_v_Task_Get_నిరంతరం_సమయం_Stats( అక్షరం * pc_Write_బఫరు ) FREERTOS_SYSTEM_CALL;
పునాది_Type_t MPU_xTaskGenericNotify( TaskHandle_t x_Task_గమ్యం_Notify,
                                   UBaseType_t ux_సూచీ_గమ్యం_Notify,
                                   uint32_t ul_విలువ,
                                   eNotifyAction eAction,
                                   uint32_t * pul_మునుపటి_Notification_విలువ ) FREERTOS_SYSTEM_CALL;
పునాది_Type_t MPU_xTaskGenericNotifyWait( UBaseType_t ux_సూచీ_గమ్యం_Wait_On,
                                       uint32_t ul_బిట్లు_గమ్యం_Clear_On_నమోదు,
                                       uint32_t ul_బిట్లు_గమ్యం_Clear_On_Exit,
                                       uint32_t * pul_Notification_విలువ,
                                       TickType_t x_Ticks_గమ్యం_Wait ) FREERTOS_SYSTEM_CALL;
uint32_t MPU_ulTaskGenericNotifyTake( UBaseType_t ux_సూచీ_గమ్యం_Wait_On,
                                      పునాది_Type_t x_Clear_లెక్క_On_Exit,
                                      TickType_t x_Ticks_గమ్యం_Wait ) FREERTOS_SYSTEM_CALL;
పునాది_Type_t MPU_xTaskGenericNotifyStateClear( TaskHandle_t xTask,
                                             UBaseType_t ux_సూచీ_గమ్యం_Clear ) FREERTOS_SYSTEM_CALL;
uint32_t MPU_ul_Task_Generic_Notify_విలువ_Clear( TaskHandle_t xTask,
                                            UBaseType_t ux_సూచీ_గమ్యం_Clear,
                                            uint32_t ul_బిట్లు_గమ్యం_Clear ) FREERTOS_SYSTEM_CALL;
పునాది_Type_t MPU_xTaskIncrementTick( శూన్య ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetCurrentTaskHandle( శూన్య ) FREERTOS_SYSTEM_CALL;
శూన్య MPU_v_Task_సమితి_సమయం_బహిర్గతం_State( సమయం_బహిర్గతం_t * స్థిరరాశి px_సమయం_బహిర్గతం ) FREERTOS_SYSTEM_CALL;
పునాది_Type_t MPU_x_Task_Check_For_సమయం_బహిర్గతం( సమయం_బహిర్గతం_t * స్థిరరాశి px_సమయం_బహిర్గతం,
                                     TickType_t * స్థిరరాశి px_Ticks_గమ్యం_Wait ) FREERTOS_SYSTEM_CALL;
శూన్య MPU_vTaskMissedYield( శూన్య ) FREERTOS_SYSTEM_CALL;
పునాది_Type_t MPU_xTaskGetSchedulerState( శూన్య ) FREERTOS_SYSTEM_CALL;
పునాది_Type_t MPU_x_Task_Catch_పైకి_Ticks( TickType_t x_Ticks_గమ్యం_Catch_పైకి ) FREERTOS_SYSTEM_CALL;

/* MPU versions of queue.h API functions. */
పునాది_Type_t MPU_x_వరుస_Generic_Send( వరుస_Handle_t x_వరుస,
                                  స్థిరరాశి శూన్య * స్థిరరాశి pv_అంశం_గమ్యం_వరుస,
                                  TickType_t x_Ticks_గమ్యం_Wait,
                                  స్థిరరాశి పునాది_Type_t xCopyPosition ) FREERTOS_SYSTEM_CALL;
పునాది_Type_t MPU_x_వరుస_Receive( వరుస_Handle_t x_వరుస,
                              శూన్య * స్థిరరాశి pv_బఫరు,
                              TickType_t x_Ticks_గమ్యం_Wait ) FREERTOS_SYSTEM_CALL;
పునాది_Type_t MPU_x_వరుస_Peek( వరుస_Handle_t x_వరుస,
                           శూన్య * స్థిరరాశి pv_బఫరు,
                           TickType_t x_Ticks_గమ్యం_Wait ) FREERTOS_SYSTEM_CALL;
పునాది_Type_t MPU_x_వరుస_Semaphore_Take( వరుస_Handle_t x_వరుస,
                                    TickType_t x_Ticks_గమ్యం_Wait ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_వరుస_Messages_నిరీక్షణ( స్థిరరాశి వరుస_Handle_t x_వరుస ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_వరుస_Spaces_అందుబాటు( స్థిరరాశి వరుస_Handle_t x_వరుస ) FREERTOS_SYSTEM_CALL;
శూన్య MPU_v_వరుస_Delete( వరుస_Handle_t x_వరుస ) FREERTOS_SYSTEM_CALL;
వరుస_Handle_t MPU_x_వరుస_Create_Mutex( స్థిరరాశి uint8_t uc_వరుస_Type ) FREERTOS_SYSTEM_CALL;
వరుస_Handle_t MPU_x_వరుస_Create_Mutex_Static( స్థిరరాశి uint8_t uc_వరుస_Type,
                                           Static_వరుస_t * px_Static_వరుస ) FREERTOS_SYSTEM_CALL;
వరుస_Handle_t MPU_x_వరుస_Create_Counting_Semaphore( స్థిరరాశి UBaseType_t ux_Max_లెక్క,
                                                 స్థిరరాశి UBaseType_t ux_Initial_లెక్క ) FREERTOS_SYSTEM_CALL;
వరుస_Handle_t MPU_x_వరుస_Create_Counting_Semaphore_Static( స్థిరరాశి UBaseType_t ux_Max_లెక్క,
                                                       స్థిరరాశి UBaseType_t ux_Initial_లెక్క,
                                                       Static_వరుస_t * px_Static_వరుస ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_x_వరుస_Get_Mutex_Holder( వరుస_Handle_t xSemaphore ) FREERTOS_SYSTEM_CALL;
పునాది_Type_t MPU_x_వరుస_Take_Mutex_Recursive( వరుస_Handle_t xMutex,
                                         TickType_t x_Ticks_గమ్యం_Wait ) FREERTOS_SYSTEM_CALL;
పునాది_Type_t MPU_x_వరుస_Give_Mutex_Recursive( వరుస_Handle_t pxMutex ) FREERTOS_SYSTEM_CALL;
శూన్య MPU_v_వరుస_కూడు_గమ్యం_Registry( వరుస_Handle_t x_వరుస,
                              స్థిరరాశి అక్షరం * pcName ) FREERTOS_SYSTEM_CALL;
శూన్య MPU_v_వరుస_Unregister_వరుస( వరుస_Handle_t x_వరుస ) FREERTOS_SYSTEM_CALL;
స్థిరరాశి అక్షరం * MPU_pc_వరుస_Get_Name( వరుస_Handle_t x_వరుస ) FREERTOS_SYSTEM_CALL;
వరుస_Handle_t MPU_x_వరుస_Generic_Create( స్థిరరాశి UBaseType_t ux_వరుస_నిడివి,
                                       స్థిరరాశి UBaseType_t ux_అంశం_కొలత,
                                       స్థిరరాశి uint8_t uc_వరుస_Type ) FREERTOS_SYSTEM_CALL;
వరుస_Handle_t MPU_x_వరుస_Generic_Create_Static( స్థిరరాశి UBaseType_t ux_వరుస_నిడివి,
                                             స్థిరరాశి UBaseType_t ux_అంశం_కొలత,
                                             uint8_t * puc_వరుస_Storage,
                                             Static_వరుస_t * px_Static_వరుస,
                                             స్థిరరాశి uint8_t uc_వరుస_Type ) FREERTOS_SYSTEM_CALL;
వరుస_సమితి_Handle_t MPU_x_వరుస_Create_సమితి( స్థిరరాశి UBaseType_t ux_Event_వరుస_నిడివి ) FREERTOS_SYSTEM_CALL;
పునాది_Type_t MPU_x_వరుస_కూడు_గమ్యం_సమితి( వరుస_సమితి_Member_Handle_t x_వరుస_Or_Semaphore,
                               వరుస_సమితి_Handle_t x_వరుస_సమితి ) FREERTOS_SYSTEM_CALL;
పునాది_Type_t MPU_x_వరుస_Remove_From_సమితి( వరుస_సమితి_Member_Handle_t x_వరుస_Or_Semaphore,
                                    వరుస_సమితి_Handle_t x_వరుస_సమితి ) FREERTOS_SYSTEM_CALL;
వరుస_సమితి_Member_Handle_t MPU_x_వరుస_Select_From_సమితి( వరుస_సమితి_Handle_t x_వరుస_సమితి,
                                                స్థిరరాశి TickType_t x_Ticks_గమ్యం_Wait ) FREERTOS_SYSTEM_CALL;
పునాది_Type_t MPU_x_వరుస_Generic_Reset( వరుస_Handle_t x_వరుస,
                                   పునాది_Type_t x_New_వరుస ) FREERTOS_SYSTEM_CALL;
శూన్య MPU_v_వరుస_సమితి_వరుస_సంఖ్య( వరుస_Handle_t x_వరుస,
                               UBaseType_t ux_వరుస_సంఖ్య ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_వరుస_Get_వరుస_సంఖ్య( వరుస_Handle_t x_వరుస ) FREERTOS_SYSTEM_CALL;
uint8_t MPU_uc_వరుస_Get_వరుస_Type( వరుస_Handle_t x_వరుస ) FREERTOS_SYSTEM_CALL;

/* MPU versions of timers.h API functions. */
TimerHandle_t MPU_xTimerCreate( స్థిరరాశి అక్షరం * స్థిరరాశి pcTimerName,
                                స్థిరరాశి TickType_t x_Timer_Period_లోపల_Ticks,
                                స్థిరరాశి UBaseType_t uxAutoReload,
                                శూన్య * స్థిరరాశి pvTimerID,
                                TimerCallbackFunction_t pxCallbackFunction ) FREERTOS_SYSTEM_CALL;
TimerHandle_t MPU_xTimerCreateStatic( స్థిరరాశి అక్షరం * స్థిరరాశి pcTimerName,
                                      స్థిరరాశి TickType_t x_Timer_Period_లోపల_Ticks,
                                      స్థిరరాశి UBaseType_t uxAutoReload,
                                      శూన్య * స్థిరరాశి pvTimerID,
                                      TimerCallbackFunction_t pxCallbackFunction,
                                      StaticTimer_t * px_Timer_బఫరు ) FREERTOS_SYSTEM_CALL;
శూన్య * MPU_pvTimerGetTimerID( స్థిరరాశి TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
శూన్య MPU_v_Timer_సమితి_Timer_ID( TimerHandle_t xTimer,
                           శూన్య * pvNewID ) FREERTOS_SYSTEM_CALL;
పునాది_Type_t MPU_x_Timer_Is_Timer_చురుకైన( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTimerGetTimerDaemonTaskHandle( శూన్య ) FREERTOS_SYSTEM_CALL;
పునాది_Type_t MPU_xTimerPendFunctionCall( PendedFunction_t x_Function_గమ్యం_Pend,
                                       శూన్య * pvParameter1,
                                       uint32_t ulParameter2,
                                       TickType_t x_Ticks_గమ్యం_Wait ) FREERTOS_SYSTEM_CALL;
స్థిరరాశి అక్షరం * MPU_pcTimerGetName( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
శూన్య MPU_v_Timer_సమితి_Reload_బహుళకం( TimerHandle_t xTimer,
                              స్థిరరాశి UBaseType_t uxAutoReload ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Timer_Get_Reload_బహుళకం( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_xTimerGetPeriod( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_x_Timer_Get_Expiry_సమయం( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
పునాది_Type_t MPU_xTimerCreateTimerTask( శూన్య ) FREERTOS_SYSTEM_CALL;
పునాది_Type_t MPU_xTimerGenericCommand( TimerHandle_t xTimer,
                                     స్థిరరాశి పునాది_Type_t xCommandID,
                                     స్థిరరాశి TickType_t x_Optional_విలువ,
                                     పునాది_Type_t * స్థిరరాశి px_Higher_ప్రాధాన్యత_Task_Woken,
                                     స్థిరరాశి TickType_t x_Ticks_గమ్యం_Wait ) FREERTOS_SYSTEM_CALL;

/* MPU versions of event_group.h API functions. */
Event_సమూహం_Handle_t MPU_x_Event_సమూహం_Create( శూన్య ) FREERTOS_SYSTEM_CALL;
Event_సమూహం_Handle_t MPU_x_Event_సమూహం_Create_Static( Static_Event_సమూహం_t * px_Event_సమూహం_బఫరు ) FREERTOS_SYSTEM_CALL;
Event_బిట్లు_t MPU_x_Event_సమూహం_Wait_బిట్లు( Event_సమూహం_Handle_t x_Event_సమూహం,
                                     స్థిరరాశి Event_బిట్లు_t ux_బిట్లు_గమ్యం_Wait_For,
                                     స్థిరరాశి పునాది_Type_t xClearOnExit,
                                     స్థిరరాశి పునాది_Type_t x_Wait_For_All_బిట్లు,
                                     TickType_t x_Ticks_గమ్యం_Wait ) FREERTOS_SYSTEM_CALL;
Event_బిట్లు_t MPU_x_Event_సమూహం_Clear_బిట్లు( Event_సమూహం_Handle_t x_Event_సమూహం,
                                      స్థిరరాశి Event_బిట్లు_t ux_బిట్లు_గమ్యం_Clear ) FREERTOS_SYSTEM_CALL;
Event_బిట్లు_t MPU_x_Event_సమూహం_సమితి_బిట్లు( Event_సమూహం_Handle_t x_Event_సమూహం,
                                    స్థిరరాశి Event_బిట్లు_t ux_బిట్లు_గమ్యం_సమితి ) FREERTOS_SYSTEM_CALL;
Event_బిట్లు_t MPU_x_Event_సమూహం_Sync( Event_సమూహం_Handle_t x_Event_సమూహం,
                                 స్థిరరాశి Event_బిట్లు_t ux_బిట్లు_గమ్యం_సమితి,
                                 స్థిరరాశి Event_బిట్లు_t ux_బిట్లు_గమ్యం_Wait_For,
                                 TickType_t x_Ticks_గమ్యం_Wait ) FREERTOS_SYSTEM_CALL;
శూన్య MPU_v_Event_సమూహం_Delete( Event_సమూహం_Handle_t x_Event_సమూహం ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Event_సమూహం_Get_సంఖ్య( శూన్య * x_Event_సమూహం ) FREERTOS_SYSTEM_CALL;

/* MPU versions of message/stream_buffer.h API functions. */
size_t MPU_x_ప్రవాహం_బఫరు_Send( ప్రవాహం_బఫరు_Handle_t x_ప్రవాహం_బఫరు,
                              స్థిరరాశి శూన్య * pv_Tx_దత్తాంశం,
                              size_t x_దత్తాంశం_నిడివి_బైట్లు,
                              TickType_t x_Ticks_గమ్యం_Wait ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_ప్రవాహం_బఫరు_Receive( ప్రవాహం_బఫరు_Handle_t x_ప్రవాహం_బఫరు,
                                 శూన్య * pv_Rx_దత్తాంశం,
                                 size_t x_బఫరు_నిడివి_బైట్లు,
                                 TickType_t x_Ticks_గమ్యం_Wait ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_ప్రవాహం_బఫరు_తదుపరి_సందేశం_నిడివి_బైట్లు( ప్రవాహం_బఫరు_Handle_t x_ప్రవాహం_బఫరు ) FREERTOS_SYSTEM_CALL;
శూన్య MPU_v_ప్రవాహం_బఫరు_Delete( ప్రవాహం_బఫరు_Handle_t x_ప్రవాహం_బఫరు ) FREERTOS_SYSTEM_CALL;
పునాది_Type_t MPU_x_ప్రవాహం_బఫరు_Is_నిండిన( ప్రవాహం_బఫరు_Handle_t x_ప్రవాహం_బఫరు ) FREERTOS_SYSTEM_CALL;
పునాది_Type_t MPU_x_ప్రవాహం_బఫరు_Is_ఖాళీలు( ప్రవాహం_బఫరు_Handle_t x_ప్రవాహం_బఫరు ) FREERTOS_SYSTEM_CALL;
పునాది_Type_t MPU_x_ప్రవాహం_బఫరు_Reset( ప్రవాహం_బఫరు_Handle_t x_ప్రవాహం_బఫరు ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_ప్రవాహం_బఫరు_Spaces_అందుబాటు( ప్రవాహం_బఫరు_Handle_t x_ప్రవాహం_బఫరు ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_ప్రవాహం_బఫరు_బైట్లు_అందుబాటు( ప్రవాహం_బఫరు_Handle_t x_ప్రవాహం_బఫరు ) FREERTOS_SYSTEM_CALL;
పునాది_Type_t MPU_x_ప్రవాహం_బఫరు_సమితి_Trigger_Level( ప్రవాహం_బఫరు_Handle_t x_ప్రవాహం_బఫరు,
                                             size_t xTriggerLevel ) FREERTOS_SYSTEM_CALL;
ప్రవాహం_బఫరు_Handle_t MPU_x_ప్రవాహం_బఫరు_Generic_Create( size_t x_బఫరు_కొలత_బైట్లు,
                                                     size_t x_Trigger_Level_బైట్లు,
                                                     పునాది_Type_t x_Is_సందేశం_బఫరు,
                                                     ప్రవాహం_బఫరు_Callback_Function_t pxSendCompletedCallback,
                                                     ప్రవాహం_బఫరు_Callback_Function_t pxReceiveCompletedCallback ) FREERTOS_SYSTEM_CALL;
ప్రవాహం_బఫరు_Handle_t MPU_x_ప్రవాహం_బఫరు_Generic_Create_Static( size_t x_బఫరు_కొలత_బైట్లు,
                                                           size_t x_Trigger_Level_బైట్లు,
                                                           పునాది_Type_t x_Is_సందేశం_బఫరు,
                                                           uint8_t * స్థిరరాశి puc_ప్రవాహం_బఫరు_Storage_Area,
                                                           Static_ప్రవాహం_బఫరు_t * స్థిరరాశి px_Static_ప్రవాహం_బఫరు,
                                                           ప్రవాహం_బఫరు_Callback_Function_t pxSendCompletedCallback,
                                                           ప్రవాహం_బఫరు_Callback_Function_t pxReceiveCompletedCallback ) FREERTOS_SYSTEM_CALL;



#అయితే_ముగింపు /* MPU_PROTOTYPES_H */
