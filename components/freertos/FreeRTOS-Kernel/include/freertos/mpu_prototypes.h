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


#که_نه_تعریف_شوی MPU_PROTOTYPES_H
#تعریف_کړه MPU_PROTOTYPES_H

/* MPU versions of task.h API functions. */
بنسټ_Type_t MPU_xTaskCreate( TaskFunction_t px_Task_کوډ,
                            ثابت حرف * ثابت pcName,
                            ثابت uint16_t us_ډېری_ژوروالی,
                            فراغ * ثابت pvParameters,
                            UBaseType_t ux_لومړیتوب,
                            TaskHandle_t * ثابت pxCreatedTask ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskCreateStatic( TaskFunction_t px_Task_کوډ,
                                    ثابت حرف * ثابت pcName,
                                    ثابت uint32_t ul_ډېری_ژوروالی,
                                    فراغ * ثابت pvParameters,
                                    UBaseType_t ux_لومړیتوب,
                                    ډېری_Type_t * ثابت pux_ډېری_بفر,
                                    StaticTask_t * ثابت px_Task_بفر ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vTaskDelete( TaskHandle_t x_Task_منزل_Delete ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vTaskDelay( ثابت TickType_t x_Ticks_منزل_Delay ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_xTaskDelayUntil( TickType_t * ثابت px_پخوانی_Wake_وخت,
                                ثابت TickType_t x_وخت_Increment ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_xTaskAbortDelay( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_لومړیتوب_Get( ثابت TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
eTaskState MPU_eTaskGetState( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vTaskGetInfo( TaskHandle_t xTask,
                       Task_حالت_t * px_Task_حالت,
                       بنسټ_Type_t x_Get_Free_ډېری_Space,
                       eTaskState eState ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_Task_لومړیتوب_سېټ( TaskHandle_t xTask,
                           UBaseType_t ux_New_لومړیتوب ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vTaskSuspend( TaskHandle_t x_Task_منزل_Suspend ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vTaskResume( TaskHandle_t x_Task_منزل_Resume ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_Task_پیل_Scheduler( فراغ ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vTaskSuspendAll( فراغ ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_xTaskResumeAll( فراغ ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_x_Task_Get_Tick_شمېر( فراغ ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_Get_شمېره_د_Tasks( فراغ ) FREERTOS_SYSTEM_CALL;
حرف * MPU_pcTaskGetName( TaskHandle_t x_Task_منزل_Query ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetHandle( ثابت حرف * pc_Name_منزل_Query ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_Get_ډېری_لوړ_Water_Mark( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
config_ډېری_ژوروالی_TYPE MPU_ux_Task_Get_ډېری_لوړ_Water_Mark2( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_Task_سېټ_Application_Task_نښان( TaskHandle_t xTask,
                                     TaskHookFunction_t pxHookFunction ) FREERTOS_SYSTEM_CALL;
TaskHookFunction_t MPU_x_Task_Get_Application_Task_نښان( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_Task_سېټ_Thread_Local_Storage_اشاره_ګر( TaskHandle_t x_Task_منزل_سېټ,
                                            بنسټ_Type_t x_شاخص,
                                            فراغ * pv_ارزښت ) FREERTOS_SYSTEM_CALL;
فراغ * MPU_pv_Task_Get_Thread_Local_Storage_اشاره_ګر( TaskHandle_t x_Task_منزل_Query,
                                               بنسټ_Type_t x_شاخص ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_xTaskCallApplicationTaskHook( TaskHandle_t xTask,
                                             فراغ * pvParameter ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetIdleTaskHandle( فراغ ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxTaskGetSystemState( Task_حالت_t * ثابت px_Task_حالت_Array,
                                      ثابت UBaseType_t ux_Array_اندازه,
                                      config_پرلپسې_وخت_COUNTER_TYPE * ثابت pul_ټول_پرلپسې_وخت ) FREERTOS_SYSTEM_CALL;
config_پرلپسې_وخت_COUNTER_TYPE MPU_ul_Task_Get_Idle_پرلپسې_وخت_Counter( فراغ ) FREERTOS_SYSTEM_CALL;
config_پرلپسې_وخت_COUNTER_TYPE MPU_ul_Task_Get_Idle_پرلپسې_وخت_Percent( فراغ ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vTaskList( حرف * pc_Write_بفر ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_Task_Get_پرلپسې_وخت_Stats( حرف * pc_Write_بفر ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_xTaskGenericNotify( TaskHandle_t x_Task_منزل_Notify,
                                   UBaseType_t ux_شاخص_منزل_Notify,
                                   uint32_t ul_ارزښت,
                                   eNotifyAction eAction,
                                   uint32_t * pul_پخوانی_Notification_ارزښت ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_xTaskGenericNotifyWait( UBaseType_t ux_شاخص_منزل_Wait_On,
                                       uint32_t ul_بټونه_منزل_Clear_On_ثبت,
                                       uint32_t ul_بټونه_منزل_Clear_On_Exit,
                                       uint32_t * pul_Notification_ارزښت,
                                       TickType_t x_Ticks_منزل_Wait ) FREERTOS_SYSTEM_CALL;
uint32_t MPU_ulTaskGenericNotifyTake( UBaseType_t ux_شاخص_منزل_Wait_On,
                                      بنسټ_Type_t x_Clear_شمېر_On_Exit,
                                      TickType_t x_Ticks_منزل_Wait ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_xTaskGenericNotifyStateClear( TaskHandle_t xTask,
                                             UBaseType_t ux_شاخص_منزل_Clear ) FREERTOS_SYSTEM_CALL;
uint32_t MPU_ul_Task_Generic_Notify_ارزښت_Clear( TaskHandle_t xTask,
                                            UBaseType_t ux_شاخص_منزل_Clear,
                                            uint32_t ul_بټونه_منزل_Clear ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_xTaskIncrementTick( فراغ ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetCurrentTaskHandle( فراغ ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_Task_سېټ_وخت_بهر_State( وخت_بهر_t * ثابت px_وخت_بهر ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_x_Task_Check_For_وخت_بهر( وخت_بهر_t * ثابت px_وخت_بهر,
                                     TickType_t * ثابت px_Ticks_منزل_Wait ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vTaskMissedYield( فراغ ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_xTaskGetSchedulerState( فراغ ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_x_Task_Catch_پورته_Ticks( TickType_t x_Ticks_منزل_Catch_پورته ) FREERTOS_SYSTEM_CALL;

/* MPU versions of queue.h API functions. */
بنسټ_Type_t MPU_x_کتار_Generic_Send( کتار_Handle_t x_کتار,
                                  ثابت فراغ * ثابت pv_توکی_منزل_کتار,
                                  TickType_t x_Ticks_منزل_Wait,
                                  ثابت بنسټ_Type_t xCopyPosition ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_x_کتار_Receive( کتار_Handle_t x_کتار,
                              فراغ * ثابت pv_بفر,
                              TickType_t x_Ticks_منزل_Wait ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_x_کتار_Peek( کتار_Handle_t x_کتار,
                           فراغ * ثابت pv_بفر,
                           TickType_t x_Ticks_منزل_Wait ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_x_کتار_Semaphore_Take( کتار_Handle_t x_کتار,
                                    TickType_t x_Ticks_منزل_Wait ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_کتار_Messages_انتظار( ثابت کتار_Handle_t x_کتار ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_کتار_Spaces_شته( ثابت کتار_Handle_t x_کتار ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_کتار_Delete( کتار_Handle_t x_کتار ) FREERTOS_SYSTEM_CALL;
کتار_Handle_t MPU_x_کتار_Create_Mutex( ثابت uint8_t uc_کتار_Type ) FREERTOS_SYSTEM_CALL;
کتار_Handle_t MPU_x_کتار_Create_Mutex_Static( ثابت uint8_t uc_کتار_Type,
                                           Static_کتار_t * px_Static_کتار ) FREERTOS_SYSTEM_CALL;
کتار_Handle_t MPU_x_کتار_Create_Counting_Semaphore( ثابت UBaseType_t ux_Max_شمېر,
                                                 ثابت UBaseType_t ux_Initial_شمېر ) FREERTOS_SYSTEM_CALL;
کتار_Handle_t MPU_x_کتار_Create_Counting_Semaphore_Static( ثابت UBaseType_t ux_Max_شمېر,
                                                       ثابت UBaseType_t ux_Initial_شمېر,
                                                       Static_کتار_t * px_Static_کتار ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_x_کتار_Get_Mutex_Holder( کتار_Handle_t xSemaphore ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_x_کتار_Take_Mutex_Recursive( کتار_Handle_t xMutex,
                                         TickType_t x_Ticks_منزل_Wait ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_x_کتار_Give_Mutex_Recursive( کتار_Handle_t pxMutex ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_کتار_جمع_کړه_منزل_Registry( کتار_Handle_t x_کتار,
                              ثابت حرف * pcName ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_کتار_Unregister_کتار( کتار_Handle_t x_کتار ) FREERTOS_SYSTEM_CALL;
ثابت حرف * MPU_pc_کتار_Get_Name( کتار_Handle_t x_کتار ) FREERTOS_SYSTEM_CALL;
کتار_Handle_t MPU_x_کتار_Generic_Create( ثابت UBaseType_t ux_کتار_اوږدتیا,
                                       ثابت UBaseType_t ux_توکی_اندازه,
                                       ثابت uint8_t uc_کتار_Type ) FREERTOS_SYSTEM_CALL;
کتار_Handle_t MPU_x_کتار_Generic_Create_Static( ثابت UBaseType_t ux_کتار_اوږدتیا,
                                             ثابت UBaseType_t ux_توکی_اندازه,
                                             uint8_t * puc_کتار_Storage,
                                             Static_کتار_t * px_Static_کتار,
                                             ثابت uint8_t uc_کتار_Type ) FREERTOS_SYSTEM_CALL;
کتار_سېټ_Handle_t MPU_x_کتار_Create_سېټ( ثابت UBaseType_t ux_Event_کتار_اوږدتیا ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_x_کتار_جمع_کړه_منزل_سېټ( کتار_سېټ_Member_Handle_t x_کتار_Or_Semaphore,
                               کتار_سېټ_Handle_t x_کتار_سېټ ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_x_کتار_Remove_From_سېټ( کتار_سېټ_Member_Handle_t x_کتار_Or_Semaphore,
                                    کتار_سېټ_Handle_t x_کتار_سېټ ) FREERTOS_SYSTEM_CALL;
کتار_سېټ_Member_Handle_t MPU_x_کتار_Select_From_سېټ( کتار_سېټ_Handle_t x_کتار_سېټ,
                                                ثابت TickType_t x_Ticks_منزل_Wait ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_x_کتار_Generic_Reset( کتار_Handle_t x_کتار,
                                   بنسټ_Type_t x_New_کتار ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_کتار_سېټ_کتار_شمېره( کتار_Handle_t x_کتار,
                               UBaseType_t ux_کتار_شمېره ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_کتار_Get_کتار_شمېره( کتار_Handle_t x_کتار ) FREERTOS_SYSTEM_CALL;
uint8_t MPU_uc_کتار_Get_کتار_Type( کتار_Handle_t x_کتار ) FREERTOS_SYSTEM_CALL;

/* MPU versions of timers.h API functions. */
TimerHandle_t MPU_xTimerCreate( ثابت حرف * ثابت pcTimerName,
                                ثابت TickType_t x_Timer_Period_دننه_Ticks,
                                ثابت UBaseType_t uxAutoReload,
                                فراغ * ثابت pvTimerID,
                                TimerCallbackFunction_t pxCallbackFunction ) FREERTOS_SYSTEM_CALL;
TimerHandle_t MPU_xTimerCreateStatic( ثابت حرف * ثابت pcTimerName,
                                      ثابت TickType_t x_Timer_Period_دننه_Ticks,
                                      ثابت UBaseType_t uxAutoReload,
                                      فراغ * ثابت pvTimerID,
                                      TimerCallbackFunction_t pxCallbackFunction,
                                      StaticTimer_t * px_Timer_بفر ) FREERTOS_SYSTEM_CALL;
فراغ * MPU_pvTimerGetTimerID( ثابت TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_Timer_سېټ_Timer_ID( TimerHandle_t xTimer,
                           فراغ * pvNewID ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_x_Timer_Is_Timer_فعال( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTimerGetTimerDaemonTaskHandle( فراغ ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_xTimerPendFunctionCall( PendedFunction_t x_Function_منزل_Pend,
                                       فراغ * pvParameter1,
                                       uint32_t ulParameter2,
                                       TickType_t x_Ticks_منزل_Wait ) FREERTOS_SYSTEM_CALL;
ثابت حرف * MPU_pcTimerGetName( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_Timer_سېټ_Reload_ډېرپېښ( TimerHandle_t xTimer,
                              ثابت UBaseType_t uxAutoReload ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Timer_Get_Reload_ډېرپېښ( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_xTimerGetPeriod( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_x_Timer_Get_Expiry_وخت( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_xTimerCreateTimerTask( فراغ ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_xTimerGenericCommand( TimerHandle_t xTimer,
                                     ثابت بنسټ_Type_t xCommandID,
                                     ثابت TickType_t x_Optional_ارزښت,
                                     بنسټ_Type_t * ثابت px_Higher_لومړیتوب_Task_Woken,
                                     ثابت TickType_t x_Ticks_منزل_Wait ) FREERTOS_SYSTEM_CALL;

/* MPU versions of event_group.h API functions. */
Event_ډله_Handle_t MPU_x_Event_ډله_Create( فراغ ) FREERTOS_SYSTEM_CALL;
Event_ډله_Handle_t MPU_x_Event_ډله_Create_Static( Static_Event_ډله_t * px_Event_ډله_بفر ) FREERTOS_SYSTEM_CALL;
Event_بټونه_t MPU_x_Event_ډله_Wait_بټونه( Event_ډله_Handle_t x_Event_ډله,
                                     ثابت Event_بټونه_t ux_بټونه_منزل_Wait_For,
                                     ثابت بنسټ_Type_t xClearOnExit,
                                     ثابت بنسټ_Type_t x_Wait_For_All_بټونه,
                                     TickType_t x_Ticks_منزل_Wait ) FREERTOS_SYSTEM_CALL;
Event_بټونه_t MPU_x_Event_ډله_Clear_بټونه( Event_ډله_Handle_t x_Event_ډله,
                                      ثابت Event_بټونه_t ux_بټونه_منزل_Clear ) FREERTOS_SYSTEM_CALL;
Event_بټونه_t MPU_x_Event_ډله_سېټ_بټونه( Event_ډله_Handle_t x_Event_ډله,
                                    ثابت Event_بټونه_t ux_بټونه_منزل_سېټ ) FREERTOS_SYSTEM_CALL;
Event_بټونه_t MPU_x_Event_ډله_Sync( Event_ډله_Handle_t x_Event_ډله,
                                 ثابت Event_بټونه_t ux_بټونه_منزل_سېټ,
                                 ثابت Event_بټونه_t ux_بټونه_منزل_Wait_For,
                                 TickType_t x_Ticks_منزل_Wait ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_Event_ډله_Delete( Event_ډله_Handle_t x_Event_ډله ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Event_ډله_Get_شمېره( فراغ * x_Event_ډله ) FREERTOS_SYSTEM_CALL;

/* MPU versions of message/stream_buffer.h API functions. */
size_t MPU_x_بهیر_بفر_Send( بهیر_بفر_Handle_t x_بهیر_بفر,
                              ثابت فراغ * pv_Tx_معلومات,
                              size_t x_معلومات_اوږدتیا_بایټونه,
                              TickType_t x_Ticks_منزل_Wait ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_بهیر_بفر_Receive( بهیر_بفر_Handle_t x_بهیر_بفر,
                                 فراغ * pv_Rx_معلومات,
                                 size_t x_بفر_اوږدتیا_بایټونه,
                                 TickType_t x_Ticks_منزل_Wait ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_بهیر_بفر_بل_پیغام_اوږدتیا_بایټونه( بهیر_بفر_Handle_t x_بهیر_بفر ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_بهیر_بفر_Delete( بهیر_بفر_Handle_t x_بهیر_بفر ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_x_بهیر_بفر_Is_ډک( بهیر_بفر_Handle_t x_بهیر_بفر ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_x_بهیر_بفر_Is_تشه( بهیر_بفر_Handle_t x_بهیر_بفر ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_x_بهیر_بفر_Reset( بهیر_بفر_Handle_t x_بهیر_بفر ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_بهیر_بفر_Spaces_شته( بهیر_بفر_Handle_t x_بهیر_بفر ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_بهیر_بفر_بایټونه_شته( بهیر_بفر_Handle_t x_بهیر_بفر ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_x_بهیر_بفر_سېټ_Trigger_Level( بهیر_بفر_Handle_t x_بهیر_بفر,
                                             size_t xTriggerLevel ) FREERTOS_SYSTEM_CALL;
بهیر_بفر_Handle_t MPU_x_بهیر_بفر_Generic_Create( size_t x_بفر_اندازه_بایټونه,
                                                     size_t x_Trigger_Level_بایټونه,
                                                     بنسټ_Type_t x_Is_پیغام_بفر,
                                                     بهیر_بفر_Callback_Function_t pxSendCompletedCallback,
                                                     بهیر_بفر_Callback_Function_t pxReceiveCompletedCallback ) FREERTOS_SYSTEM_CALL;
بهیر_بفر_Handle_t MPU_x_بهیر_بفر_Generic_Create_Static( size_t x_بفر_اندازه_بایټونه,
                                                           size_t x_Trigger_Level_بایټونه,
                                                           بنسټ_Type_t x_Is_پیغام_بفر,
                                                           uint8_t * ثابت puc_بهیر_بفر_Storage_Area,
                                                           Static_بهیر_بفر_t * ثابت px_Static_بهیر_بفر,
                                                           بهیر_بفر_Callback_Function_t pxSendCompletedCallback,
                                                           بهیر_بفر_Callback_Function_t pxReceiveCompletedCallback ) FREERTOS_SYSTEM_CALL;



#د_که_پای /* MPU_PROTOTYPES_H */
