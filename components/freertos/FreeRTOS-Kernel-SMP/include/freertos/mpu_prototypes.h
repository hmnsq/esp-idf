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

/*
 * When the MPU is used the standard (non MPU) API functions are mapped to
 * equivalents that start "MPU_", the prototypes for which are defined in this
 * header files.  This will cause the application code to call the MPU_ version
 * which wraps the non-MPU version with privilege promoting then demoting code,
 * so the kernel code always runs will full privileges.
 */


#که_نه_تعریف_شوی MPU_PROTOTYPES_H
#تعریف_کړه MPU_PROTOTYPES_H

تعريفنوع بنية xTaskGenericNotifyParams
{
    TaskHandle_t x_Task_منزل_Notify;
    UBaseType_t ux_شاخص_منزل_Notify;
    uint32_t ul_ارزښت;
    eNotifyAction eAction;
    uint32_t * pul_پخوانی_Notification_ارزښت;
} xTaskGenericNotifyParams_t;

تعريفنوع بنية xTaskGenericNotifyWaitParams
{
    UBaseType_t ux_شاخص_منزل_Wait_On;
    uint32_t ul_بټونه_منزل_Clear_On_ثبت;
    uint32_t ul_بټونه_منزل_Clear_On_Exit;
    uint32_t * pul_Notification_ارزښت;
    TickType_t x_Ticks_منزل_Wait;
} xTaskGenericNotifyWaitParams_t;

تعريفنوع بنية xTimerGenericCommandFromTaskParams
{
    TimerHandle_t xTimer;
    بنسټ_Type_t xCommandID;
    TickType_t x_Optional_ارزښت;
    بنسټ_Type_t * px_Higher_لومړیتوب_Task_Woken;
    TickType_t x_Ticks_منزل_Wait;
} xTimerGenericCommandFromTaskParams_t;

تعريفنوع بنية x_Event_ډله_Wait_بټونه_Params
{
    Event_ډله_Handle_t x_Event_ډله;
    Event_بټونه_t ux_بټونه_منزل_Wait_For;
    بنسټ_Type_t xClearOnExit;
    بنسټ_Type_t x_Wait_For_All_بټونه;
    TickType_t x_Ticks_منزل_Wait;
} x_Event_ډله_Wait_بټونه_Params_t;

/* MPU versions of task.h API functions. */
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
فراغ MPU_vTaskSuspend( TaskHandle_t x_Task_منزل_Suspend ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vTaskResume( TaskHandle_t x_Task_منزل_Resume ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_x_Task_Get_Tick_شمېر( فراغ ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_Get_شمېره_د_Tasks( فراغ ) FREERTOS_SYSTEM_CALL;
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
TaskHandle_t MPU_xTaskGetIdleTaskHandle( فراغ ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxTaskGetSystemState( Task_حالت_t * ثابت px_Task_حالت_Array,
                                      ثابت UBaseType_t ux_Array_اندازه,
                                      config_پرلپسې_وخت_COUNTER_TYPE * ثابت pul_ټول_پرلپسې_وخت ) FREERTOS_SYSTEM_CALL;
config_پرلپسې_وخت_COUNTER_TYPE MPU_ul_Task_Get_پرلپسې_وخت_Counter( ثابت TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
config_پرلپسې_وخت_COUNTER_TYPE MPU_ul_Task_Get_پرلپسې_وخت_Percent( ثابت TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
config_پرلپسې_وخت_COUNTER_TYPE MPU_ul_Task_Get_Idle_پرلپسې_وخت_Counter( فراغ ) FREERTOS_SYSTEM_CALL;
config_پرلپسې_وخت_COUNTER_TYPE MPU_ul_Task_Get_Idle_پرلپسې_وخت_Percent( فراغ ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_xTaskGenericNotify( TaskHandle_t x_Task_منزل_Notify,
                                   UBaseType_t ux_شاخص_منزل_Notify,
                                   uint32_t ul_ارزښت,
                                   eNotifyAction eAction,
                                   uint32_t * pul_پخوانی_Notification_ارزښت ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_x_Task_Generic_Notify_ثبت( ثابت xTaskGenericNotifyParams_t * pxParams ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_xTaskGenericNotifyWait( UBaseType_t ux_شاخص_منزل_Wait_On,
                                       uint32_t ul_بټونه_منزل_Clear_On_ثبت,
                                       uint32_t ul_بټونه_منزل_Clear_On_Exit,
                                       uint32_t * pul_Notification_ارزښت,
                                       TickType_t x_Ticks_منزل_Wait ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_x_Task_Generic_Notify_Wait_ثبت( ثابت xTaskGenericNotifyWaitParams_t * pxParams ) FREERTOS_SYSTEM_CALL;
uint32_t MPU_ulTaskGenericNotifyTake( UBaseType_t ux_شاخص_منزل_Wait_On,
                                      بنسټ_Type_t x_Clear_شمېر_On_Exit,
                                      TickType_t x_Ticks_منزل_Wait ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_xTaskGenericNotifyStateClear( TaskHandle_t xTask,
                                             UBaseType_t ux_شاخص_منزل_Clear ) FREERTOS_SYSTEM_CALL;
uint32_t MPU_ul_Task_Generic_Notify_ارزښت_Clear( TaskHandle_t xTask,
                                            UBaseType_t ux_شاخص_منزل_Clear,
                                            uint32_t ul_بټونه_منزل_Clear ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_Task_سېټ_وخت_بهر_State( وخت_بهر_t * ثابت px_وخت_بهر ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_x_Task_Check_For_وخت_بهر( وخت_بهر_t * ثابت px_وخت_بهر,
                                     TickType_t * ثابت px_Ticks_منزل_Wait ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetCurrentTaskHandle( فراغ ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_xTaskGetSchedulerState( فراغ ) FREERTOS_SYSTEM_CALL;

/* Privileged only wrappers for Task APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
بنسټ_Type_t MPU_xTaskCreate( TaskFunction_t px_Task_کوډ,
                            ثابت حرف * ثابت pcName,
                            ثابت config_ډېری_ژوروالی_TYPE ux_ډېری_ژوروالی,
                            فراغ * ثابت pvParameters,
                            UBaseType_t ux_لومړیتوب,
                            TaskHandle_t * ثابت pxCreatedTask ) PRIVILEGED_FUNCTION;
TaskHandle_t MPU_xTaskCreateStatic( TaskFunction_t px_Task_کوډ,
                                    ثابت حرف * ثابت pcName,
                                    ثابت config_ډېری_ژوروالی_TYPE ux_ډېری_ژوروالی,
                                    فراغ * ثابت pvParameters,
                                    UBaseType_t ux_لومړیتوب,
                                    ډېری_Type_t * ثابت pux_ډېری_بفر,
                                    StaticTask_t * ثابت px_Task_بفر ) PRIVILEGED_FUNCTION;
فراغ MPU_vTaskDelete( TaskHandle_t x_Task_منزل_Delete ) PRIVILEGED_FUNCTION;
فراغ MPU_v_Task_لومړیتوب_سېټ( TaskHandle_t xTask,
                           UBaseType_t ux_New_لومړیتوب ) PRIVILEGED_FUNCTION;
TaskHandle_t MPU_xTaskGetHandle( ثابت حرف * pc_Name_منزل_Query ) PRIVILEGED_FUNCTION;
بنسټ_Type_t MPU_xTaskCallApplicationTaskHook( TaskHandle_t xTask,
                                             فراغ * pvParameter ) PRIVILEGED_FUNCTION;
حرف * MPU_pcTaskGetName( TaskHandle_t x_Task_منزل_Query ) PRIVILEGED_FUNCTION;
بنسټ_Type_t MPU_xTaskCreateRestricted( ثابت TaskParameters_t * ثابت pxTaskDefinition,
                                      TaskHandle_t * pxCreatedTask ) PRIVILEGED_FUNCTION;
بنسټ_Type_t MPU_xTaskCreateRestrictedStatic( ثابت TaskParameters_t * ثابت pxTaskDefinition,
                                            TaskHandle_t * pxCreatedTask ) PRIVILEGED_FUNCTION;
فراغ MPU_vTaskAllocateMPURegions( TaskHandle_t x_Task_منزل_Modify,
                                  ثابت حافظه_Region_t * ثابت xRegions ) PRIVILEGED_FUNCTION;
بنسټ_Type_t MPU_xTaskGetStaticBuffers( TaskHandle_t xTask,
                                      ډېری_Type_t ** ppux_ډېری_بفر,
                                      StaticTask_t ** ppx_Task_بفر ) PRIVILEGED_FUNCTION;
UBaseType_t MPU_ux_Task_لومړیتوب_Get_From_ISR( ثابت TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
UBaseType_t MPU_ux_Task_بنسټ_لومړیتوب_Get( ثابت TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
UBaseType_t MPU_ux_Task_بنسټ_لومړیتوب_Get_From_ISR( ثابت TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
بنسټ_Type_t MPU_xTaskResumeFromISR( TaskHandle_t x_Task_منزل_Resume ) PRIVILEGED_FUNCTION;
TaskHookFunction_t MPU_x_Task_Get_Application_Task_نښان_From_ISR( TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
بنسټ_Type_t MPU_xTaskGenericNotifyFromISR( TaskHandle_t x_Task_منزل_Notify,
                                          UBaseType_t ux_شاخص_منزل_Notify,
                                          uint32_t ul_ارزښت,
                                          eNotifyAction eAction,
                                          uint32_t * pul_پخوانی_Notification_ارزښت,
                                          بنسټ_Type_t * px_Higher_لومړیتوب_Task_Woken ) PRIVILEGED_FUNCTION;
فراغ MPU_vTaskGenericNotifyGiveFromISR( TaskHandle_t x_Task_منزل_Notify,
                                        UBaseType_t ux_شاخص_منزل_Notify,
                                        بنسټ_Type_t * px_Higher_لومړیتوب_Task_Woken ) PRIVILEGED_FUNCTION;

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
TaskHandle_t MPU_x_کتار_Get_Mutex_Holder( کتار_Handle_t xSemaphore ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_x_کتار_Take_Mutex_Recursive( کتار_Handle_t xMutex,
                                         TickType_t x_Ticks_منزل_Wait ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_x_کتار_Give_Mutex_Recursive( کتار_Handle_t pxMutex ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_کتار_جمع_کړه_منزل_Registry( کتار_Handle_t x_کتار,
                              ثابت حرف * pcName ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_کتار_Unregister_کتار( کتار_Handle_t x_کتار ) FREERTOS_SYSTEM_CALL;
ثابت حرف * MPU_pc_کتار_Get_Name( کتار_Handle_t x_کتار ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_x_کتار_جمع_کړه_منزل_سېټ( کتار_سېټ_Member_Handle_t x_کتار_Or_Semaphore,
                               کتار_سېټ_Handle_t x_کتار_سېټ ) FREERTOS_SYSTEM_CALL;
کتار_سېټ_Member_Handle_t MPU_x_کتار_Select_From_سېټ( کتار_سېټ_Handle_t x_کتار_سېټ,
                                                ثابت TickType_t x_Ticks_منزل_Wait ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_کتار_سېټ_کتار_شمېره( کتار_Handle_t x_کتار,
                               UBaseType_t ux_کتار_شمېره ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_کتار_Get_کتار_شمېره( کتار_Handle_t x_کتار ) FREERTOS_SYSTEM_CALL;
uint8_t MPU_uc_کتار_Get_کتار_Type( کتار_Handle_t x_کتار ) FREERTOS_SYSTEM_CALL;

/* Privileged only wrappers for Queue APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
فراغ MPU_v_کتار_Delete( کتار_Handle_t x_کتار ) PRIVILEGED_FUNCTION;
کتار_Handle_t MPU_x_کتار_Create_Mutex( ثابت uint8_t uc_کتار_Type ) PRIVILEGED_FUNCTION;
کتار_Handle_t MPU_x_کتار_Create_Mutex_Static( ثابت uint8_t uc_کتار_Type,
                                           Static_کتار_t * px_Static_کتار ) PRIVILEGED_FUNCTION;
کتار_Handle_t MPU_x_کتار_Create_Counting_Semaphore( ثابت UBaseType_t ux_Max_شمېر,
                                                 ثابت UBaseType_t ux_Initial_شمېر ) PRIVILEGED_FUNCTION;
کتار_Handle_t MPU_x_کتار_Create_Counting_Semaphore_Static( ثابت UBaseType_t ux_Max_شمېر,
                                                       ثابت UBaseType_t ux_Initial_شمېر,
                                                       Static_کتار_t * px_Static_کتار ) PRIVILEGED_FUNCTION;
کتار_Handle_t MPU_x_کتار_Generic_Create( ثابت UBaseType_t ux_کتار_اوږدتیا,
                                       ثابت UBaseType_t ux_توکی_اندازه,
                                       ثابت uint8_t uc_کتار_Type ) PRIVILEGED_FUNCTION;
کتار_Handle_t MPU_x_کتار_Generic_Create_Static( ثابت UBaseType_t ux_کتار_اوږدتیا,
                                             ثابت UBaseType_t ux_توکی_اندازه,
                                             uint8_t * puc_کتار_Storage,
                                             Static_کتار_t * px_Static_کتار,
                                             ثابت uint8_t uc_کتار_Type ) PRIVILEGED_FUNCTION;
کتار_سېټ_Handle_t MPU_x_کتار_Create_سېټ( ثابت UBaseType_t ux_Event_کتار_اوږدتیا ) PRIVILEGED_FUNCTION;
بنسټ_Type_t MPU_x_کتار_Remove_From_سېټ( کتار_سېټ_Member_Handle_t x_کتار_Or_Semaphore,
                                    کتار_سېټ_Handle_t x_کتار_سېټ ) PRIVILEGED_FUNCTION;
بنسټ_Type_t MPU_x_کتار_Generic_Reset( کتار_Handle_t x_کتار,
                                   بنسټ_Type_t x_New_کتار ) PRIVILEGED_FUNCTION;
بنسټ_Type_t MPU_x_کتار_Generic_Get_Static_Buffers( کتار_Handle_t x_کتار,
                                              uint8_t ** ppuc_کتار_Storage,
                                              Static_کتار_t ** ppx_Static_کتار ) PRIVILEGED_FUNCTION;
بنسټ_Type_t MPU_x_کتار_Generic_Send_From_ISR( کتار_Handle_t x_کتار,
                                         ثابت فراغ * ثابت pv_توکی_منزل_کتار,
                                         بنسټ_Type_t * ثابت px_Higher_لومړیتوب_Task_Woken,
                                         ثابت بنسټ_Type_t xCopyPosition ) PRIVILEGED_FUNCTION;
بنسټ_Type_t MPU_x_کتار_Give_From_ISR( کتار_Handle_t x_کتار,
                                  بنسټ_Type_t * ثابت px_Higher_لومړیتوب_Task_Woken ) PRIVILEGED_FUNCTION;
بنسټ_Type_t MPU_x_کتار_Peek_From_ISR( کتار_Handle_t x_کتار,
                                  فراغ * ثابت pv_بفر ) PRIVILEGED_FUNCTION;
بنسټ_Type_t MPU_x_کتار_Receive_From_ISR( کتار_Handle_t x_کتار,
                                     فراغ * ثابت pv_بفر,
                                     بنسټ_Type_t * ثابت px_Higher_لومړیتوب_Task_Woken ) PRIVILEGED_FUNCTION;
بنسټ_Type_t MPU_x_کتار_Is_کتار_تشه_From_ISR( ثابت کتار_Handle_t x_کتار ) PRIVILEGED_FUNCTION;
بنسټ_Type_t MPU_x_کتار_Is_کتار_ډک_From_ISR( ثابت کتار_Handle_t x_کتار ) PRIVILEGED_FUNCTION;
UBaseType_t MPU_ux_کتار_Messages_انتظار_From_ISR( ثابت کتار_Handle_t x_کتار ) PRIVILEGED_FUNCTION;
TaskHandle_t MPU_x_کتار_Get_Mutex_Holder_From_ISR( کتار_Handle_t xSemaphore ) PRIVILEGED_FUNCTION;
کتار_سېټ_Member_Handle_t MPU_x_کتار_Select_From_سېټ_From_ISR( کتار_سېټ_Handle_t x_کتار_سېټ ) PRIVILEGED_FUNCTION;

/* MPU versions of timers.h API functions. */
فراغ * MPU_pvTimerGetTimerID( ثابت TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_Timer_سېټ_Timer_ID( TimerHandle_t xTimer,
                           فراغ * pvNewID ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_x_Timer_Is_Timer_فعال( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTimerGetTimerDaemonTaskHandle( فراغ ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_xTimerGenericCommandFromTask( TimerHandle_t xTimer,
                                             ثابت بنسټ_Type_t xCommandID,
                                             ثابت TickType_t x_Optional_ارزښت,
                                             بنسټ_Type_t * ثابت px_Higher_لومړیتوب_Task_Woken,
                                             ثابت TickType_t x_Ticks_منزل_Wait ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_x_Timer_Generic_Command_From_Task_ثبت( ثابت xTimerGenericCommandFromTaskParams_t * pxParams ) FREERTOS_SYSTEM_CALL;
ثابت حرف * MPU_pcTimerGetName( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_Timer_سېټ_Reload_ډېرپېښ( TimerHandle_t xTimer,
                              ثابت بنسټ_Type_t uxAutoReload ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_x_Timer_Get_Reload_ډېرپېښ( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Timer_Get_Reload_ډېرپېښ( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_xTimerGetPeriod( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_x_Timer_Get_Expiry_وخت( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;

/* Privileged only wrappers for Timer APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
TimerHandle_t MPU_xTimerCreate( ثابت حرف * ثابت pcTimerName,
                                ثابت TickType_t x_Timer_Period_دننه_Ticks,
                                ثابت UBaseType_t uxAutoReload,
                                فراغ * ثابت pvTimerID,
                                TimerCallbackFunction_t pxCallbackFunction ) PRIVILEGED_FUNCTION;
TimerHandle_t MPU_xTimerCreateStatic( ثابت حرف * ثابت pcTimerName,
                                      ثابت TickType_t x_Timer_Period_دننه_Ticks,
                                      ثابت UBaseType_t uxAutoReload,
                                      فراغ * ثابت pvTimerID,
                                      TimerCallbackFunction_t pxCallbackFunction,
                                      StaticTimer_t * px_Timer_بفر ) PRIVILEGED_FUNCTION;
بنسټ_Type_t MPU_x_Timer_Get_Static_بفر( TimerHandle_t xTimer,
                                      StaticTimer_t ** ppx_Timer_بفر ) PRIVILEGED_FUNCTION;
بنسټ_Type_t MPU_xTimerGenericCommandFromISR( TimerHandle_t xTimer,
                                            ثابت بنسټ_Type_t xCommandID,
                                            ثابت TickType_t x_Optional_ارزښت,
                                            بنسټ_Type_t * ثابت px_Higher_لومړیتوب_Task_Woken,
                                            ثابت TickType_t x_Ticks_منزل_Wait ) PRIVILEGED_FUNCTION;

/* MPU versions of event_group.h API functions. */
Event_بټونه_t MPU_x_Event_ډله_Wait_بټونه( Event_ډله_Handle_t x_Event_ډله,
                                     ثابت Event_بټونه_t ux_بټونه_منزل_Wait_For,
                                     ثابت بنسټ_Type_t xClearOnExit,
                                     ثابت بنسټ_Type_t x_Wait_For_All_بټونه,
                                     TickType_t x_Ticks_منزل_Wait ) FREERTOS_SYSTEM_CALL;
Event_بټونه_t MPU_x_Event_ډله_Wait_بټونه_ثبت( ثابت x_Event_ډله_Wait_بټونه_Params_t * pxParams ) FREERTOS_SYSTEM_CALL;
Event_بټونه_t MPU_x_Event_ډله_Clear_بټونه( Event_ډله_Handle_t x_Event_ډله,
                                      ثابت Event_بټونه_t ux_بټونه_منزل_Clear ) FREERTOS_SYSTEM_CALL;
Event_بټونه_t MPU_x_Event_ډله_سېټ_بټونه( Event_ډله_Handle_t x_Event_ډله,
                                    ثابت Event_بټونه_t ux_بټونه_منزل_سېټ ) FREERTOS_SYSTEM_CALL;
Event_بټونه_t MPU_x_Event_ډله_Sync( Event_ډله_Handle_t x_Event_ډله,
                                 ثابت Event_بټونه_t ux_بټونه_منزل_سېټ,
                                 ثابت Event_بټونه_t ux_بټونه_منزل_Wait_For,
                                 TickType_t x_Ticks_منزل_Wait ) FREERTOS_SYSTEM_CALL;
#إذا ( configUSE_TRACE_FACILITY == 1 )
    UBaseType_t MPU_ux_Event_ډله_Get_شمېره( فراغ * x_Event_ډله ) FREERTOS_SYSTEM_CALL;
    فراغ MPU_v_Event_ډله_سېټ_شمېره( فراغ * x_Event_ډله,
                                   UBaseType_t ux_Event_ډله_شمېره ) FREERTOS_SYSTEM_CALL;
#د_که_پای /* ( configUSE_TRACE_FACILITY == 1 )*/

/* Privileged only wrappers for Event Group APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
Event_ډله_Handle_t MPU_x_Event_ډله_Create( فراغ ) PRIVILEGED_FUNCTION;
Event_ډله_Handle_t MPU_x_Event_ډله_Create_Static( Static_Event_ډله_t * px_Event_ډله_بفر ) PRIVILEGED_FUNCTION;
فراغ MPU_v_Event_ډله_Delete( Event_ډله_Handle_t x_Event_ډله ) PRIVILEGED_FUNCTION;
بنسټ_Type_t MPU_x_Event_ډله_Get_Static_بفر( Event_ډله_Handle_t x_Event_ډله,
                                           Static_Event_ډله_t ** ppx_Event_ډله_بفر ) PRIVILEGED_FUNCTION;
بنسټ_Type_t MPU_x_Event_ډله_Clear_بټونه_From_ISR( Event_ډله_Handle_t x_Event_ډله,
                                            ثابت Event_بټونه_t ux_بټونه_منزل_Clear ) PRIVILEGED_FUNCTION;
بنسټ_Type_t MPU_x_Event_ډله_سېټ_بټونه_From_ISR( Event_ډله_Handle_t x_Event_ډله,
                                          ثابت Event_بټونه_t ux_بټونه_منزل_سېټ,
                                          بنسټ_Type_t * px_Higher_لومړیتوب_Task_Woken ) PRIVILEGED_FUNCTION;
Event_بټونه_t MPU_x_Event_ډله_Get_بټونه_From_ISR( Event_ډله_Handle_t x_Event_ډله ) PRIVILEGED_FUNCTION;

/* MPU versions of message/stream_buffer.h API functions. */
size_t MPU_x_بهیر_بفر_Send( بهیر_بفر_Handle_t x_بهیر_بفر,
                              ثابت فراغ * pv_Tx_معلومات,
                              size_t x_معلومات_اوږدتیا_بایټونه,
                              TickType_t x_Ticks_منزل_Wait ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_بهیر_بفر_Receive( بهیر_بفر_Handle_t x_بهیر_بفر,
                                 فراغ * pv_Rx_معلومات,
                                 size_t x_بفر_اوږدتیا_بایټونه,
                                 TickType_t x_Ticks_منزل_Wait ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_x_بهیر_بفر_Is_ډک( بهیر_بفر_Handle_t x_بهیر_بفر ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_x_بهیر_بفر_Is_تشه( بهیر_بفر_Handle_t x_بهیر_بفر ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_بهیر_بفر_Spaces_شته( بهیر_بفر_Handle_t x_بهیر_بفر ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_بهیر_بفر_بایټونه_شته( بهیر_بفر_Handle_t x_بهیر_بفر ) FREERTOS_SYSTEM_CALL;
بنسټ_Type_t MPU_x_بهیر_بفر_سېټ_Trigger_Level( بهیر_بفر_Handle_t x_بهیر_بفر,
                                             size_t xTriggerLevel ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_بهیر_بفر_بل_پیغام_اوږدتیا_بایټونه( بهیر_بفر_Handle_t x_بهیر_بفر ) FREERTOS_SYSTEM_CALL;

/* Privileged only wrappers for Stream Buffer APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
بهیر_بفر_Handle_t MPU_x_بهیر_بفر_Generic_Create( size_t x_بفر_اندازه_بایټونه,
                                                     size_t x_Trigger_Level_بایټونه,
                                                     بنسټ_Type_t x_بهیر_بفر_Type,
                                                     بهیر_بفر_Callback_Function_t pxSendCompletedCallback,
                                                     بهیر_بفر_Callback_Function_t pxReceiveCompletedCallback ) PRIVILEGED_FUNCTION;
بهیر_بفر_Handle_t MPU_x_بهیر_بفر_Generic_Create_Static( size_t x_بفر_اندازه_بایټونه,
                                                           size_t x_Trigger_Level_بایټونه,
                                                           بنسټ_Type_t x_بهیر_بفر_Type,
                                                           uint8_t * ثابت puc_بهیر_بفر_Storage_Area,
                                                           Static_بهیر_بفر_t * ثابت px_Static_بهیر_بفر,
                                                           بهیر_بفر_Callback_Function_t pxSendCompletedCallback,
                                                           بهیر_بفر_Callback_Function_t pxReceiveCompletedCallback ) PRIVILEGED_FUNCTION;
فراغ MPU_v_بهیر_بفر_Delete( بهیر_بفر_Handle_t x_بهیر_بفر ) PRIVILEGED_FUNCTION;
بنسټ_Type_t MPU_x_بهیر_بفر_Reset( بهیر_بفر_Handle_t x_بهیر_بفر ) PRIVILEGED_FUNCTION;
بنسټ_Type_t MPU_x_بهیر_بفر_Get_Static_Buffers( بهیر_بفر_Handle_t x_بهیر_Buffers,
                                              uint8_t * ppuc_بهیر_بفر_Storage_Area,
                                              Static_بهیر_بفر_t * ppx_Static_بهیر_بفر ) PRIVILEGED_FUNCTION;
size_t MPU_x_بهیر_بفر_Send_From_ISR( بهیر_بفر_Handle_t x_بهیر_بفر,
                                     ثابت فراغ * pv_Tx_معلومات,
                                     size_t x_معلومات_اوږدتیا_بایټونه,
                                     بنسټ_Type_t * ثابت px_Higher_لومړیتوب_Task_Woken ) PRIVILEGED_FUNCTION;
size_t MPU_x_بهیر_بفر_Receive_From_ISR( بهیر_بفر_Handle_t x_بهیر_بفر,
                                        فراغ * pv_Rx_معلومات,
                                        size_t x_بفر_اوږدتیا_بایټونه,
                                        بنسټ_Type_t * ثابت px_Higher_لومړیتوب_Task_Woken ) PRIVILEGED_FUNCTION;
بنسټ_Type_t MPU_x_بهیر_بفر_Send_Completed_From_ISR( بهیر_بفر_Handle_t x_بهیر_بفر,
                                                  بنسټ_Type_t * px_Higher_لومړیتوب_Task_Woken ) PRIVILEGED_FUNCTION;
بنسټ_Type_t MPU_x_بهیر_بفر_Receive_Completed_From_ISR( بهیر_بفر_Handle_t x_بهیر_بفر,
                                                     بنسټ_Type_t * px_Higher_لومړیتوب_Task_Woken ) PRIVILEGED_FUNCTION;
بنسټ_Type_t MPU_x_بهیر_بفر_Reset_From_ISR( بهیر_بفر_Handle_t x_بهیر_بفر ) PRIVILEGED_FUNCTION;

#د_که_پای /* MPU_PROTOTYPES_H */
