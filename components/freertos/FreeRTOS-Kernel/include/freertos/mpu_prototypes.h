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


#اگر_غیر_تعریف_شدہ MPU_PROTOTYPES_H
#تعریف_کرو MPU_PROTOTYPES_H

/* MPU versions of task.h API functions. */
بنیاد_Type_t MPU_xTaskCreate( TaskFunction_t px_Task_رمز,
                            ثابت حرف * ثابت pcName,
                            ثابت uint16_t us_ڈھیر_گہرائی,
                            خالی * ثابت pvParameters,
                            UBaseType_t ux_ترجیح,
                            TaskHandle_t * ثابت pxCreatedTask ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskCreateStatic( TaskFunction_t px_Task_رمز,
                                    ثابت حرف * ثابت pcName,
                                    ثابت uint32_t ul_ڈھیر_گہرائی,
                                    خالی * ثابت pvParameters,
                                    UBaseType_t ux_ترجیح,
                                    ڈھیر_Type_t * ثابت pux_ڈھیر_بفر,
                                    StaticTask_t * ثابت px_Task_بفر ) FREERTOS_SYSTEM_CALL;
خالی MPU_vTaskDelete( TaskHandle_t x_Task_مقصد_Delete ) FREERTOS_SYSTEM_CALL;
خالی MPU_vTaskDelay( ثابت TickType_t x_Ticks_مقصد_Delay ) FREERTOS_SYSTEM_CALL;
بنیاد_Type_t MPU_xTaskDelayUntil( TickType_t * ثابت px_پچھلا_Wake_وقت,
                                ثابت TickType_t x_وقت_Increment ) FREERTOS_SYSTEM_CALL;
بنیاد_Type_t MPU_xTaskAbortDelay( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_ترجیح_Get( ثابت TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
eTaskState MPU_eTaskGetState( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
خالی MPU_vTaskGetInfo( TaskHandle_t xTask,
                       Task_حالت_t * px_Task_حالت,
                       بنیاد_Type_t x_Get_Free_ڈھیر_Space,
                       eTaskState eState ) FREERTOS_SYSTEM_CALL;
خالی MPU_v_Task_ترجیح_سیٹ( TaskHandle_t xTask,
                           UBaseType_t ux_New_ترجیح ) FREERTOS_SYSTEM_CALL;
خالی MPU_vTaskSuspend( TaskHandle_t x_Task_مقصد_Suspend ) FREERTOS_SYSTEM_CALL;
خالی MPU_vTaskResume( TaskHandle_t x_Task_مقصد_Resume ) FREERTOS_SYSTEM_CALL;
خالی MPU_v_Task_آغاز_Scheduler( خالی ) FREERTOS_SYSTEM_CALL;
خالی MPU_vTaskSuspendAll( خالی ) FREERTOS_SYSTEM_CALL;
بنیاد_Type_t MPU_xTaskResumeAll( خالی ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_x_Task_Get_Tick_گنتی( خالی ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_Get_نمبر_کا_Tasks( خالی ) FREERTOS_SYSTEM_CALL;
حرف * MPU_pcTaskGetName( TaskHandle_t x_Task_مقصد_Query ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetHandle( ثابت حرف * pc_Name_مقصد_Query ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_Get_ڈھیر_بلند_Water_Mark( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
config_ڈھیر_گہرائی_TYPE MPU_ux_Task_Get_ڈھیر_بلند_Water_Mark2( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
خالی MPU_v_Task_سیٹ_Application_Task_لیبل( TaskHandle_t xTask,
                                     TaskHookFunction_t pxHookFunction ) FREERTOS_SYSTEM_CALL;
TaskHookFunction_t MPU_x_Task_Get_Application_Task_لیبل( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
خالی MPU_v_Task_سیٹ_Thread_Local_Storage_اشارہ_گر( TaskHandle_t x_Task_مقصد_سیٹ,
                                            بنیاد_Type_t x_اشاریہ,
                                            خالی * pv_قدر ) FREERTOS_SYSTEM_CALL;
خالی * MPU_pv_Task_Get_Thread_Local_Storage_اشارہ_گر( TaskHandle_t x_Task_مقصد_Query,
                                               بنیاد_Type_t x_اشاریہ ) FREERTOS_SYSTEM_CALL;
بنیاد_Type_t MPU_xTaskCallApplicationTaskHook( TaskHandle_t xTask,
                                             خالی * pvParameter ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetIdleTaskHandle( خالی ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxTaskGetSystemState( Task_حالت_t * ثابت px_Task_حالت_Array,
                                      ثابت UBaseType_t ux_Array_سائز,
                                      config_تسلسل_وقت_شمار_TYPE * ثابت pul_کل_تسلسل_وقت ) FREERTOS_SYSTEM_CALL;
config_تسلسل_وقت_شمار_TYPE MPU_ul_Task_Get_Idle_تسلسل_وقت_شمار( خالی ) FREERTOS_SYSTEM_CALL;
config_تسلسل_وقت_شمار_TYPE MPU_ul_Task_Get_Idle_تسلسل_وقت_Percent( خالی ) FREERTOS_SYSTEM_CALL;
خالی MPU_vTaskList( حرف * pc_Write_بفر ) FREERTOS_SYSTEM_CALL;
خالی MPU_v_Task_Get_تسلسل_وقت_Stats( حرف * pc_Write_بفر ) FREERTOS_SYSTEM_CALL;
بنیاد_Type_t MPU_xTaskGenericNotify( TaskHandle_t x_Task_مقصد_Notify,
                                   UBaseType_t ux_اشاریہ_مقصد_Notify,
                                   uint32_t ul_قدر,
                                   eNotifyAction eAction,
                                   uint32_t * pul_پچھلا_Notification_قدر ) FREERTOS_SYSTEM_CALL;
بنیاد_Type_t MPU_xTaskGenericNotifyWait( UBaseType_t ux_اشاریہ_مقصد_Wait_On,
                                       uint32_t ul_بٹ_مقصد_Clear_On_اندراج,
                                       uint32_t ul_بٹ_مقصد_Clear_On_Exit,
                                       uint32_t * pul_Notification_قدر,
                                       TickType_t x_Ticks_مقصد_Wait ) FREERTOS_SYSTEM_CALL;
uint32_t MPU_ulTaskGenericNotifyTake( UBaseType_t ux_اشاریہ_مقصد_Wait_On,
                                      بنیاد_Type_t x_Clear_گنتی_On_Exit,
                                      TickType_t x_Ticks_مقصد_Wait ) FREERTOS_SYSTEM_CALL;
بنیاد_Type_t MPU_xTaskGenericNotifyStateClear( TaskHandle_t xTask,
                                             UBaseType_t ux_اشاریہ_مقصد_Clear ) FREERTOS_SYSTEM_CALL;
uint32_t MPU_ul_Task_Generic_Notify_قدر_Clear( TaskHandle_t xTask,
                                            UBaseType_t ux_اشاریہ_مقصد_Clear,
                                            uint32_t ul_بٹ_مقصد_Clear ) FREERTOS_SYSTEM_CALL;
بنیاد_Type_t MPU_xTaskIncrementTick( خالی ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetCurrentTaskHandle( خالی ) FREERTOS_SYSTEM_CALL;
خالی MPU_v_Task_سیٹ_وقت_برآمد_State( وقت_برآمد_t * ثابت px_وقت_برآمد ) FREERTOS_SYSTEM_CALL;
بنیاد_Type_t MPU_x_Task_Check_For_وقت_برآمد( وقت_برآمد_t * ثابت px_وقت_برآمد,
                                     TickType_t * ثابت px_Ticks_مقصد_Wait ) FREERTOS_SYSTEM_CALL;
خالی MPU_vTaskMissedYield( خالی ) FREERTOS_SYSTEM_CALL;
بنیاد_Type_t MPU_xTaskGetSchedulerState( خالی ) FREERTOS_SYSTEM_CALL;
بنیاد_Type_t MPU_x_Task_Catch_اوپر_Ticks( TickType_t x_Ticks_مقصد_Catch_اوپر ) FREERTOS_SYSTEM_CALL;

/* MPU versions of queue.h API functions. */
بنیاد_Type_t MPU_x_قطار_Generic_Send( قطار_Handle_t x_قطار,
                                  ثابت خالی * ثابت pv_شے_مقصد_قطار,
                                  TickType_t x_Ticks_مقصد_Wait,
                                  ثابت بنیاد_Type_t xCopyPosition ) FREERTOS_SYSTEM_CALL;
بنیاد_Type_t MPU_x_قطار_Receive( قطار_Handle_t x_قطار,
                              خالی * ثابت pv_بفر,
                              TickType_t x_Ticks_مقصد_Wait ) FREERTOS_SYSTEM_CALL;
بنیاد_Type_t MPU_x_قطار_Peek( قطار_Handle_t x_قطار,
                           خالی * ثابت pv_بفر,
                           TickType_t x_Ticks_مقصد_Wait ) FREERTOS_SYSTEM_CALL;
بنیاد_Type_t MPU_x_قطار_Semaphore_Take( قطار_Handle_t x_قطار,
                                    TickType_t x_Ticks_مقصد_Wait ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_قطار_Messages_انتظار( ثابت قطار_Handle_t x_قطار ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_قطار_Spaces_دستیاب( ثابت قطار_Handle_t x_قطار ) FREERTOS_SYSTEM_CALL;
خالی MPU_v_قطار_Delete( قطار_Handle_t x_قطار ) FREERTOS_SYSTEM_CALL;
قطار_Handle_t MPU_x_قطار_Create_Mutex( ثابت uint8_t uc_قطار_Type ) FREERTOS_SYSTEM_CALL;
قطار_Handle_t MPU_x_قطار_Create_Mutex_Static( ثابت uint8_t uc_قطار_Type,
                                           Static_قطار_t * px_Static_قطار ) FREERTOS_SYSTEM_CALL;
قطار_Handle_t MPU_x_قطار_Create_Counting_Semaphore( ثابت UBaseType_t ux_Max_گنتی,
                                                 ثابت UBaseType_t ux_Initial_گنتی ) FREERTOS_SYSTEM_CALL;
قطار_Handle_t MPU_x_قطار_Create_Counting_Semaphore_Static( ثابت UBaseType_t ux_Max_گنتی,
                                                       ثابت UBaseType_t ux_Initial_گنتی,
                                                       Static_قطار_t * px_Static_قطار ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_x_قطار_Get_Mutex_Holder( قطار_Handle_t xSemaphore ) FREERTOS_SYSTEM_CALL;
بنیاد_Type_t MPU_x_قطار_Take_Mutex_Recursive( قطار_Handle_t xMutex,
                                         TickType_t x_Ticks_مقصد_Wait ) FREERTOS_SYSTEM_CALL;
بنیاد_Type_t MPU_x_قطار_Give_Mutex_Recursive( قطار_Handle_t pxMutex ) FREERTOS_SYSTEM_CALL;
خالی MPU_v_قطار_جوڑو_مقصد_Registry( قطار_Handle_t x_قطار,
                              ثابت حرف * pcName ) FREERTOS_SYSTEM_CALL;
خالی MPU_v_قطار_Unregister_قطار( قطار_Handle_t x_قطار ) FREERTOS_SYSTEM_CALL;
ثابت حرف * MPU_pc_قطار_Get_Name( قطار_Handle_t x_قطار ) FREERTOS_SYSTEM_CALL;
قطار_Handle_t MPU_x_قطار_Generic_Create( ثابت UBaseType_t ux_قطار_لمبائی,
                                       ثابت UBaseType_t ux_شے_سائز,
                                       ثابت uint8_t uc_قطار_Type ) FREERTOS_SYSTEM_CALL;
قطار_Handle_t MPU_x_قطار_Generic_Create_Static( ثابت UBaseType_t ux_قطار_لمبائی,
                                             ثابت UBaseType_t ux_شے_سائز,
                                             uint8_t * puc_قطار_Storage,
                                             Static_قطار_t * px_Static_قطار,
                                             ثابت uint8_t uc_قطار_Type ) FREERTOS_SYSTEM_CALL;
قطار_سیٹ_Handle_t MPU_x_قطار_Create_سیٹ( ثابت UBaseType_t ux_Event_قطار_لمبائی ) FREERTOS_SYSTEM_CALL;
بنیاد_Type_t MPU_x_قطار_جوڑو_مقصد_سیٹ( قطار_سیٹ_Member_Handle_t x_قطار_Or_Semaphore,
                               قطار_سیٹ_Handle_t x_قطار_سیٹ ) FREERTOS_SYSTEM_CALL;
بنیاد_Type_t MPU_x_قطار_Remove_From_سیٹ( قطار_سیٹ_Member_Handle_t x_قطار_Or_Semaphore,
                                    قطار_سیٹ_Handle_t x_قطار_سیٹ ) FREERTOS_SYSTEM_CALL;
قطار_سیٹ_Member_Handle_t MPU_x_قطار_Select_From_سیٹ( قطار_سیٹ_Handle_t x_قطار_سیٹ,
                                                ثابت TickType_t x_Ticks_مقصد_Wait ) FREERTOS_SYSTEM_CALL;
بنیاد_Type_t MPU_x_قطار_Generic_Reset( قطار_Handle_t x_قطار,
                                   بنیاد_Type_t x_New_قطار ) FREERTOS_SYSTEM_CALL;
خالی MPU_v_قطار_سیٹ_قطار_نمبر( قطار_Handle_t x_قطار,
                               UBaseType_t ux_قطار_نمبر ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_قطار_Get_قطار_نمبر( قطار_Handle_t x_قطار ) FREERTOS_SYSTEM_CALL;
uint8_t MPU_uc_قطار_Get_قطار_Type( قطار_Handle_t x_قطار ) FREERTOS_SYSTEM_CALL;

/* MPU versions of timers.h API functions. */
TimerHandle_t MPU_xTimerCreate( ثابت حرف * ثابت pcTimerName,
                                ثابت TickType_t x_Timer_Period_اندر_Ticks,
                                ثابت UBaseType_t uxAutoReload,
                                خالی * ثابت pvTimerID,
                                TimerCallbackFunction_t pxCallbackFunction ) FREERTOS_SYSTEM_CALL;
TimerHandle_t MPU_xTimerCreateStatic( ثابت حرف * ثابت pcTimerName,
                                      ثابت TickType_t x_Timer_Period_اندر_Ticks,
                                      ثابت UBaseType_t uxAutoReload,
                                      خالی * ثابت pvTimerID,
                                      TimerCallbackFunction_t pxCallbackFunction,
                                      StaticTimer_t * px_Timer_بفر ) FREERTOS_SYSTEM_CALL;
خالی * MPU_pvTimerGetTimerID( ثابت TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
خالی MPU_v_Timer_سیٹ_Timer_ID( TimerHandle_t xTimer,
                           خالی * pvNewID ) FREERTOS_SYSTEM_CALL;
بنیاد_Type_t MPU_x_Timer_Is_Timer_فعال( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTimerGetTimerDaemonTaskHandle( خالی ) FREERTOS_SYSTEM_CALL;
بنیاد_Type_t MPU_xTimerPendFunctionCall( PendedFunction_t x_Function_مقصد_Pend,
                                       خالی * pvParameter1,
                                       uint32_t ulParameter2,
                                       TickType_t x_Ticks_مقصد_Wait ) FREERTOS_SYSTEM_CALL;
ثابت حرف * MPU_pcTimerGetName( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
خالی MPU_v_Timer_سیٹ_Reload_منوال( TimerHandle_t xTimer,
                              ثابت UBaseType_t uxAutoReload ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Timer_Get_Reload_منوال( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_xTimerGetPeriod( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_x_Timer_Get_Expiry_وقت( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
بنیاد_Type_t MPU_xTimerCreateTimerTask( خالی ) FREERTOS_SYSTEM_CALL;
بنیاد_Type_t MPU_xTimerGenericCommand( TimerHandle_t xTimer,
                                     ثابت بنیاد_Type_t xCommandID,
                                     ثابت TickType_t x_Optional_قدر,
                                     بنیاد_Type_t * ثابت px_Higher_ترجیح_Task_Woken,
                                     ثابت TickType_t x_Ticks_مقصد_Wait ) FREERTOS_SYSTEM_CALL;

/* MPU versions of event_group.h API functions. */
Event_گروہ_Handle_t MPU_x_Event_گروہ_Create( خالی ) FREERTOS_SYSTEM_CALL;
Event_گروہ_Handle_t MPU_x_Event_گروہ_Create_Static( Static_Event_گروہ_t * px_Event_گروہ_بفر ) FREERTOS_SYSTEM_CALL;
Event_بٹ_t MPU_x_Event_گروہ_Wait_بٹ( Event_گروہ_Handle_t x_Event_گروہ,
                                     ثابت Event_بٹ_t ux_بٹ_مقصد_Wait_For,
                                     ثابت بنیاد_Type_t xClearOnExit,
                                     ثابت بنیاد_Type_t x_Wait_For_All_بٹ,
                                     TickType_t x_Ticks_مقصد_Wait ) FREERTOS_SYSTEM_CALL;
Event_بٹ_t MPU_x_Event_گروہ_Clear_بٹ( Event_گروہ_Handle_t x_Event_گروہ,
                                      ثابت Event_بٹ_t ux_بٹ_مقصد_Clear ) FREERTOS_SYSTEM_CALL;
Event_بٹ_t MPU_x_Event_گروہ_سیٹ_بٹ( Event_گروہ_Handle_t x_Event_گروہ,
                                    ثابت Event_بٹ_t ux_بٹ_مقصد_سیٹ ) FREERTOS_SYSTEM_CALL;
Event_بٹ_t MPU_x_Event_گروہ_Sync( Event_گروہ_Handle_t x_Event_گروہ,
                                 ثابت Event_بٹ_t ux_بٹ_مقصد_سیٹ,
                                 ثابت Event_بٹ_t ux_بٹ_مقصد_Wait_For,
                                 TickType_t x_Ticks_مقصد_Wait ) FREERTOS_SYSTEM_CALL;
خالی MPU_v_Event_گروہ_Delete( Event_گروہ_Handle_t x_Event_گروہ ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Event_گروہ_Get_نمبر( خالی * x_Event_گروہ ) FREERTOS_SYSTEM_CALL;

/* MPU versions of message/stream_buffer.h API functions. */
size_t MPU_x_دھارا_بفر_Send( دھارا_بفر_Handle_t x_دھارا_بفر,
                              ثابت خالی * pv_Tx_ڈیٹا,
                              size_t x_ڈیٹا_لمبائی_بائٹیں,
                              TickType_t x_Ticks_مقصد_Wait ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_دھارا_بفر_Receive( دھارا_بفر_Handle_t x_دھارا_بفر,
                                 خالی * pv_Rx_ڈیٹا,
                                 size_t x_بفر_لمبائی_بائٹیں,
                                 TickType_t x_Ticks_مقصد_Wait ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_دھارا_بفر_اگلا_پیغام_لمبائی_بائٹیں( دھارا_بفر_Handle_t x_دھارا_بفر ) FREERTOS_SYSTEM_CALL;
خالی MPU_v_دھارا_بفر_Delete( دھارا_بفر_Handle_t x_دھارا_بفر ) FREERTOS_SYSTEM_CALL;
بنیاد_Type_t MPU_x_دھارا_بفر_Is_بھرا( دھارا_بفر_Handle_t x_دھارا_بفر ) FREERTOS_SYSTEM_CALL;
بنیاد_Type_t MPU_x_دھارا_بفر_Is_خالی_خانے( دھارا_بفر_Handle_t x_دھارا_بفر ) FREERTOS_SYSTEM_CALL;
بنیاد_Type_t MPU_x_دھارا_بفر_Reset( دھارا_بفر_Handle_t x_دھارا_بفر ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_دھارا_بفر_Spaces_دستیاب( دھارا_بفر_Handle_t x_دھارا_بفر ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_دھارا_بفر_بائٹیں_دستیاب( دھارا_بفر_Handle_t x_دھارا_بفر ) FREERTOS_SYSTEM_CALL;
بنیاد_Type_t MPU_x_دھارا_بفر_سیٹ_Trigger_Level( دھارا_بفر_Handle_t x_دھارا_بفر,
                                             size_t xTriggerLevel ) FREERTOS_SYSTEM_CALL;
دھارا_بفر_Handle_t MPU_x_دھارا_بفر_Generic_Create( size_t x_بفر_سائز_بائٹیں,
                                                     size_t x_Trigger_Level_بائٹیں,
                                                     بنیاد_Type_t x_Is_پیغام_بفر,
                                                     دھارا_بفر_Callback_Function_t pxSendCompletedCallback,
                                                     دھارا_بفر_Callback_Function_t pxReceiveCompletedCallback ) FREERTOS_SYSTEM_CALL;
دھارا_بفر_Handle_t MPU_x_دھارا_بفر_Generic_Create_Static( size_t x_بفر_سائز_بائٹیں,
                                                           size_t x_Trigger_Level_بائٹیں,
                                                           بنیاد_Type_t x_Is_پیغام_بفر,
                                                           uint8_t * ثابت puc_دھارا_بفر_Storage_Area,
                                                           Static_دھارا_بفر_t * ثابت px_Static_دھارا_بفر,
                                                           دھارا_بفر_Callback_Function_t pxSendCompletedCallback,
                                                           دھارا_بفر_Callback_Function_t pxReceiveCompletedCallback ) FREERTOS_SYSTEM_CALL;



#اگر_ختم /* MPU_PROTOTYPES_H */
