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


#اگر_تعریف_نشده MPU_PROTOTYPES_H
#تعریف_کن MPU_PROTOTYPES_H

/* MPU versions of task.h API functions. */
پایه_Type_t MPU_xTaskCreate( TaskFunction_t px_Task_کد,
                            ثابت حرف * ثابت pcName,
                            ثابت uint16_t us_پشته_عمق,
                            فراغ * ثابت pvParameters,
                            UBaseType_t ux_اولویت,
                            TaskHandle_t * ثابت pxCreatedTask ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskCreateStatic( TaskFunction_t px_Task_کد,
                                    ثابت حرف * ثابت pcName,
                                    ثابت uint32_t ul_پشته_عمق,
                                    فراغ * ثابت pvParameters,
                                    UBaseType_t ux_اولویت,
                                    پشته_Type_t * ثابت pux_پشته_میانگیر,
                                    StaticTask_t * ثابت px_Task_میانگیر ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vTaskDelete( TaskHandle_t x_Task_مقصد_Delete ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vTaskDelay( ثابت TickType_t x_Ticks_مقصد_Delay ) FREERTOS_SYSTEM_CALL;
پایه_Type_t MPU_xTaskDelayUntil( TickType_t * ثابت px_قبلی_Wake_زمان,
                                ثابت TickType_t x_زمان_Increment ) FREERTOS_SYSTEM_CALL;
پایه_Type_t MPU_xTaskAbortDelay( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_اولویت_Get( ثابت TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
eTaskState MPU_eTaskGetState( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vTaskGetInfo( TaskHandle_t xTask,
                       Task_وضعیت_t * px_Task_وضعیت,
                       پایه_Type_t x_Get_Free_پشته_Space,
                       eTaskState eState ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_Task_اولویت_مجموعه( TaskHandle_t xTask,
                           UBaseType_t ux_New_اولویت ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vTaskSuspend( TaskHandle_t x_Task_مقصد_Suspend ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vTaskResume( TaskHandle_t x_Task_مقصد_Resume ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_Task_شروع_Scheduler( فراغ ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vTaskSuspendAll( فراغ ) FREERTOS_SYSTEM_CALL;
پایه_Type_t MPU_xTaskResumeAll( فراغ ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_x_Task_Get_Tick_شمار( فراغ ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_Get_عدد_از_Tasks( فراغ ) FREERTOS_SYSTEM_CALL;
حرف * MPU_pcTaskGetName( TaskHandle_t x_Task_مقصد_Query ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetHandle( ثابت حرف * pc_Name_مقصد_Query ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_Get_پشته_بالا_Water_Mark( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
config_پشته_عمق_TYPE MPU_ux_Task_Get_پشته_بالا_Water_Mark2( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_Task_مجموعه_Application_Task_برچسب( TaskHandle_t xTask,
                                     TaskHookFunction_t pxHookFunction ) FREERTOS_SYSTEM_CALL;
TaskHookFunction_t MPU_x_Task_Get_Application_Task_برچسب( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_Task_مجموعه_Thread_Local_Storage_اشاره_گر( TaskHandle_t x_Task_مقصد_مجموعه,
                                            پایه_Type_t x_شاخص,
                                            فراغ * pv_مقدار ) FREERTOS_SYSTEM_CALL;
فراغ * MPU_pv_Task_Get_Thread_Local_Storage_اشاره_گر( TaskHandle_t x_Task_مقصد_Query,
                                               پایه_Type_t x_شاخص ) FREERTOS_SYSTEM_CALL;
پایه_Type_t MPU_xTaskCallApplicationTaskHook( TaskHandle_t xTask,
                                             فراغ * pvParameter ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetIdleTaskHandle( فراغ ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxTaskGetSystemState( Task_وضعیت_t * ثابت px_Task_وضعیت_Array,
                                      ثابت UBaseType_t ux_Array_اندازه,
                                      config_پیاپی_زمان_COUNTER_TYPE * ثابت pul_مجموع_پیاپی_زمان ) FREERTOS_SYSTEM_CALL;
config_پیاپی_زمان_COUNTER_TYPE MPU_ul_Task_Get_Idle_پیاپی_زمان_Counter( فراغ ) FREERTOS_SYSTEM_CALL;
config_پیاپی_زمان_COUNTER_TYPE MPU_ul_Task_Get_Idle_پیاپی_زمان_Percent( فراغ ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vTaskList( حرف * pc_Write_میانگیر ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_Task_Get_پیاپی_زمان_Stats( حرف * pc_Write_میانگیر ) FREERTOS_SYSTEM_CALL;
پایه_Type_t MPU_xTaskGenericNotify( TaskHandle_t x_Task_مقصد_Notify,
                                   UBaseType_t ux_شاخص_مقصد_Notify,
                                   uint32_t ul_مقدار,
                                   eNotifyAction eAction,
                                   uint32_t * pul_قبلی_Notification_مقدار ) FREERTOS_SYSTEM_CALL;
پایه_Type_t MPU_xTaskGenericNotifyWait( UBaseType_t ux_شاخص_مقصد_Wait_On,
                                       uint32_t ul_بیتها_مقصد_Clear_On_مدخل,
                                       uint32_t ul_بیتها_مقصد_Clear_On_Exit,
                                       uint32_t * pul_Notification_مقدار,
                                       TickType_t x_Ticks_مقصد_Wait ) FREERTOS_SYSTEM_CALL;
uint32_t MPU_ulTaskGenericNotifyTake( UBaseType_t ux_شاخص_مقصد_Wait_On,
                                      پایه_Type_t x_Clear_شمار_On_Exit,
                                      TickType_t x_Ticks_مقصد_Wait ) FREERTOS_SYSTEM_CALL;
پایه_Type_t MPU_xTaskGenericNotifyStateClear( TaskHandle_t xTask,
                                             UBaseType_t ux_شاخص_مقصد_Clear ) FREERTOS_SYSTEM_CALL;
uint32_t MPU_ul_Task_Generic_Notify_مقدار_Clear( TaskHandle_t xTask,
                                            UBaseType_t ux_شاخص_مقصد_Clear,
                                            uint32_t ul_بیتها_مقصد_Clear ) FREERTOS_SYSTEM_CALL;
پایه_Type_t MPU_xTaskIncrementTick( فراغ ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetCurrentTaskHandle( فراغ ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_Task_مجموعه_زمان_بیرون_State( زمان_بیرون_t * ثابت px_زمان_بیرون ) FREERTOS_SYSTEM_CALL;
پایه_Type_t MPU_x_Task_Check_For_زمان_بیرون( زمان_بیرون_t * ثابت px_زمان_بیرون,
                                     TickType_t * ثابت px_Ticks_مقصد_Wait ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vTaskMissedYield( فراغ ) FREERTOS_SYSTEM_CALL;
پایه_Type_t MPU_xTaskGetSchedulerState( فراغ ) FREERTOS_SYSTEM_CALL;
پایه_Type_t MPU_x_Task_Catch_به_بالا_Ticks( TickType_t x_Ticks_مقصد_Catch_به_بالا ) FREERTOS_SYSTEM_CALL;

/* MPU versions of queue.h API functions. */
پایه_Type_t MPU_x_صف_Generic_Send( صف_Handle_t x_صف,
                                  ثابت فراغ * ثابت pv_قلم_مقصد_صف,
                                  TickType_t x_Ticks_مقصد_Wait,
                                  ثابت پایه_Type_t xCopyPosition ) FREERTOS_SYSTEM_CALL;
پایه_Type_t MPU_x_صف_Receive( صف_Handle_t x_صف,
                              فراغ * ثابت pv_میانگیر,
                              TickType_t x_Ticks_مقصد_Wait ) FREERTOS_SYSTEM_CALL;
پایه_Type_t MPU_x_صف_Peek( صف_Handle_t x_صف,
                           فراغ * ثابت pv_میانگیر,
                           TickType_t x_Ticks_مقصد_Wait ) FREERTOS_SYSTEM_CALL;
پایه_Type_t MPU_x_صف_Semaphore_Take( صف_Handle_t x_صف,
                                    TickType_t x_Ticks_مقصد_Wait ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_صف_Messages_انتظار( ثابت صف_Handle_t x_صف ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_صف_Spaces_موجود( ثابت صف_Handle_t x_صف ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_صف_Delete( صف_Handle_t x_صف ) FREERTOS_SYSTEM_CALL;
صف_Handle_t MPU_x_صف_Create_Mutex( ثابت uint8_t uc_صف_Type ) FREERTOS_SYSTEM_CALL;
صف_Handle_t MPU_x_صف_Create_Mutex_Static( ثابت uint8_t uc_صف_Type,
                                           Static_صف_t * px_Static_صف ) FREERTOS_SYSTEM_CALL;
صف_Handle_t MPU_x_صف_Create_Counting_Semaphore( ثابت UBaseType_t ux_Max_شمار,
                                                 ثابت UBaseType_t ux_Initial_شمار ) FREERTOS_SYSTEM_CALL;
صف_Handle_t MPU_x_صف_Create_Counting_Semaphore_Static( ثابت UBaseType_t ux_Max_شمار,
                                                       ثابت UBaseType_t ux_Initial_شمار,
                                                       Static_صف_t * px_Static_صف ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_x_صف_Get_Mutex_Holder( صف_Handle_t xSemaphore ) FREERTOS_SYSTEM_CALL;
پایه_Type_t MPU_x_صف_Take_Mutex_Recursive( صف_Handle_t xMutex,
                                         TickType_t x_Ticks_مقصد_Wait ) FREERTOS_SYSTEM_CALL;
پایه_Type_t MPU_x_صف_Give_Mutex_Recursive( صف_Handle_t pxMutex ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_صف_جمع_کن_مقصد_Registry( صف_Handle_t x_صف,
                              ثابت حرف * pcName ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_صف_Unregister_صف( صف_Handle_t x_صف ) FREERTOS_SYSTEM_CALL;
ثابت حرف * MPU_pc_صف_Get_Name( صف_Handle_t x_صف ) FREERTOS_SYSTEM_CALL;
صف_Handle_t MPU_x_صف_Generic_Create( ثابت UBaseType_t ux_صف_درازی,
                                       ثابت UBaseType_t ux_قلم_اندازه,
                                       ثابت uint8_t uc_صف_Type ) FREERTOS_SYSTEM_CALL;
صف_Handle_t MPU_x_صف_Generic_Create_Static( ثابت UBaseType_t ux_صف_درازی,
                                             ثابت UBaseType_t ux_قلم_اندازه,
                                             uint8_t * puc_صف_Storage,
                                             Static_صف_t * px_Static_صف,
                                             ثابت uint8_t uc_صف_Type ) FREERTOS_SYSTEM_CALL;
صف_مجموعه_Handle_t MPU_x_صف_Create_مجموعه( ثابت UBaseType_t ux_Event_صف_درازی ) FREERTOS_SYSTEM_CALL;
پایه_Type_t MPU_x_صف_جمع_کن_مقصد_مجموعه( صف_مجموعه_Member_Handle_t x_صف_Or_Semaphore,
                               صف_مجموعه_Handle_t x_صف_مجموعه ) FREERTOS_SYSTEM_CALL;
پایه_Type_t MPU_x_صف_Remove_From_مجموعه( صف_مجموعه_Member_Handle_t x_صف_Or_Semaphore,
                                    صف_مجموعه_Handle_t x_صف_مجموعه ) FREERTOS_SYSTEM_CALL;
صف_مجموعه_Member_Handle_t MPU_x_صف_Select_From_مجموعه( صف_مجموعه_Handle_t x_صف_مجموعه,
                                                ثابت TickType_t x_Ticks_مقصد_Wait ) FREERTOS_SYSTEM_CALL;
پایه_Type_t MPU_x_صف_Generic_Reset( صف_Handle_t x_صف,
                                   پایه_Type_t x_New_صف ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_صف_مجموعه_صف_عدد( صف_Handle_t x_صف,
                               UBaseType_t ux_صف_عدد ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_صف_Get_صف_عدد( صف_Handle_t x_صف ) FREERTOS_SYSTEM_CALL;
uint8_t MPU_uc_صف_Get_صف_Type( صف_Handle_t x_صف ) FREERTOS_SYSTEM_CALL;

/* MPU versions of timers.h API functions. */
TimerHandle_t MPU_xTimerCreate( ثابت حرف * ثابت pcTimerName,
                                ثابت TickType_t x_Timer_Period_درون_Ticks,
                                ثابت UBaseType_t uxAutoReload,
                                فراغ * ثابت pvTimerID,
                                TimerCallbackFunction_t pxCallbackFunction ) FREERTOS_SYSTEM_CALL;
TimerHandle_t MPU_xTimerCreateStatic( ثابت حرف * ثابت pcTimerName,
                                      ثابت TickType_t x_Timer_Period_درون_Ticks,
                                      ثابت UBaseType_t uxAutoReload,
                                      فراغ * ثابت pvTimerID,
                                      TimerCallbackFunction_t pxCallbackFunction,
                                      StaticTimer_t * px_Timer_میانگیر ) FREERTOS_SYSTEM_CALL;
فراغ * MPU_pvTimerGetTimerID( ثابت TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_Timer_مجموعه_Timer_ID( TimerHandle_t xTimer,
                           فراغ * pvNewID ) FREERTOS_SYSTEM_CALL;
پایه_Type_t MPU_x_Timer_Is_Timer_فعال( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTimerGetTimerDaemonTaskHandle( فراغ ) FREERTOS_SYSTEM_CALL;
پایه_Type_t MPU_xTimerPendFunctionCall( PendedFunction_t x_Function_مقصد_Pend,
                                       فراغ * pvParameter1,
                                       uint32_t ulParameter2,
                                       TickType_t x_Ticks_مقصد_Wait ) FREERTOS_SYSTEM_CALL;
ثابت حرف * MPU_pcTimerGetName( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_Timer_مجموعه_Reload_نما( TimerHandle_t xTimer,
                              ثابت UBaseType_t uxAutoReload ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Timer_Get_Reload_نما( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_xTimerGetPeriod( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_x_Timer_Get_Expiry_زمان( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
پایه_Type_t MPU_xTimerCreateTimerTask( فراغ ) FREERTOS_SYSTEM_CALL;
پایه_Type_t MPU_xTimerGenericCommand( TimerHandle_t xTimer,
                                     ثابت پایه_Type_t xCommandID,
                                     ثابت TickType_t x_Optional_مقدار,
                                     پایه_Type_t * ثابت px_Higher_اولویت_Task_Woken,
                                     ثابت TickType_t x_Ticks_مقصد_Wait ) FREERTOS_SYSTEM_CALL;

/* MPU versions of event_group.h API functions. */
Event_گروه_Handle_t MPU_x_Event_گروه_Create( فراغ ) FREERTOS_SYSTEM_CALL;
Event_گروه_Handle_t MPU_x_Event_گروه_Create_Static( Static_Event_گروه_t * px_Event_گروه_میانگیر ) FREERTOS_SYSTEM_CALL;
Event_بیتها_t MPU_x_Event_گروه_Wait_بیتها( Event_گروه_Handle_t x_Event_گروه,
                                     ثابت Event_بیتها_t ux_بیتها_مقصد_Wait_For,
                                     ثابت پایه_Type_t xClearOnExit,
                                     ثابت پایه_Type_t x_Wait_For_All_بیتها,
                                     TickType_t x_Ticks_مقصد_Wait ) FREERTOS_SYSTEM_CALL;
Event_بیتها_t MPU_x_Event_گروه_Clear_بیتها( Event_گروه_Handle_t x_Event_گروه,
                                      ثابت Event_بیتها_t ux_بیتها_مقصد_Clear ) FREERTOS_SYSTEM_CALL;
Event_بیتها_t MPU_x_Event_گروه_مجموعه_بیتها( Event_گروه_Handle_t x_Event_گروه,
                                    ثابت Event_بیتها_t ux_بیتها_مقصد_مجموعه ) FREERTOS_SYSTEM_CALL;
Event_بیتها_t MPU_x_Event_گروه_Sync( Event_گروه_Handle_t x_Event_گروه,
                                 ثابت Event_بیتها_t ux_بیتها_مقصد_مجموعه,
                                 ثابت Event_بیتها_t ux_بیتها_مقصد_Wait_For,
                                 TickType_t x_Ticks_مقصد_Wait ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_Event_گروه_Delete( Event_گروه_Handle_t x_Event_گروه ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Event_گروه_Get_عدد( فراغ * x_Event_گروه ) FREERTOS_SYSTEM_CALL;

/* MPU versions of message/stream_buffer.h API functions. */
size_t MPU_x_جریان_میانگیر_Send( جریان_میانگیر_Handle_t x_جریان_میانگیر,
                              ثابت فراغ * pv_Tx_داده,
                              size_t x_داده_درازی_بایتها,
                              TickType_t x_Ticks_مقصد_Wait ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_جریان_میانگیر_Receive( جریان_میانگیر_Handle_t x_جریان_میانگیر,
                                 فراغ * pv_Rx_داده,
                                 size_t x_میانگیر_درازی_بایتها,
                                 TickType_t x_Ticks_مقصد_Wait ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_جریان_میانگیر_بعدی_پیام_درازی_بایتها( جریان_میانگیر_Handle_t x_جریان_میانگیر ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_جریان_میانگیر_Delete( جریان_میانگیر_Handle_t x_جریان_میانگیر ) FREERTOS_SYSTEM_CALL;
پایه_Type_t MPU_x_جریان_میانگیر_Is_پر( جریان_میانگیر_Handle_t x_جریان_میانگیر ) FREERTOS_SYSTEM_CALL;
پایه_Type_t MPU_x_جریان_میانگیر_Is_خالی( جریان_میانگیر_Handle_t x_جریان_میانگیر ) FREERTOS_SYSTEM_CALL;
پایه_Type_t MPU_x_جریان_میانگیر_Reset( جریان_میانگیر_Handle_t x_جریان_میانگیر ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_جریان_میانگیر_Spaces_موجود( جریان_میانگیر_Handle_t x_جریان_میانگیر ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_جریان_میانگیر_بایتها_موجود( جریان_میانگیر_Handle_t x_جریان_میانگیر ) FREERTOS_SYSTEM_CALL;
پایه_Type_t MPU_x_جریان_میانگیر_مجموعه_Trigger_Level( جریان_میانگیر_Handle_t x_جریان_میانگیر,
                                             size_t xTriggerLevel ) FREERTOS_SYSTEM_CALL;
جریان_میانگیر_Handle_t MPU_x_جریان_میانگیر_Generic_Create( size_t x_میانگیر_اندازه_بایتها,
                                                     size_t x_Trigger_Level_بایتها,
                                                     پایه_Type_t x_Is_پیام_میانگیر,
                                                     جریان_میانگیر_Callback_Function_t pxSendCompletedCallback,
                                                     جریان_میانگیر_Callback_Function_t pxReceiveCompletedCallback ) FREERTOS_SYSTEM_CALL;
جریان_میانگیر_Handle_t MPU_x_جریان_میانگیر_Generic_Create_Static( size_t x_میانگیر_اندازه_بایتها,
                                                           size_t x_Trigger_Level_بایتها,
                                                           پایه_Type_t x_Is_پیام_میانگیر,
                                                           uint8_t * ثابت puc_جریان_میانگیر_Storage_Area,
                                                           Static_جریان_میانگیر_t * ثابت px_Static_جریان_میانگیر,
                                                           جریان_میانگیر_Callback_Function_t pxSendCompletedCallback,
                                                           جریان_میانگیر_Callback_Function_t pxReceiveCompletedCallback ) FREERTOS_SYSTEM_CALL;



#پایان_اگر /* MPU_PROTOTYPES_H */
