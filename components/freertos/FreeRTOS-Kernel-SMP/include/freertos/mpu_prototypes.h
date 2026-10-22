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


#جيڪڏهن_وصف_نه_ٿيل MPU_PROTOTYPES_H
#وصف_ڪريو MPU_PROTOTYPES_H

تعريفنوع بنية xTaskGenericNotifyParams
{
    TaskHandle_t x_Task_منزل_Notify;
    UBaseType_t ux_اشاريو_منزل_Notify;
    uint32_t ul_قدر;
    eNotifyAction eAction;
    uint32_t * pul_اڳيون_Notification_قدر;
} xTaskGenericNotifyParams_t;

تعريفنوع بنية xTaskGenericNotifyWaitParams
{
    UBaseType_t ux_اشاريو_منزل_Wait_On;
    uint32_t ul_بٽ_منزل_Clear_On_داخلا;
    uint32_t ul_بٽ_منزل_Clear_On_Exit;
    uint32_t * pul_Notification_قدر;
    TickType_t x_Ticks_منزل_Wait;
} xTaskGenericNotifyWaitParams_t;

تعريفنوع بنية xTimerGenericCommandFromTaskParams
{
    TimerHandle_t xTimer;
    بنياد_Type_t xCommandID;
    TickType_t x_Optional_قدر;
    بنياد_Type_t * px_Higher_ترجيح_Task_Woken;
    TickType_t x_Ticks_منزل_Wait;
} xTimerGenericCommandFromTaskParams_t;

تعريفنوع بنية x_Event_گروهه_Wait_بٽ_Params
{
    Event_گروهه_Handle_t x_Event_گروهه;
    Event_بٽ_t ux_بٽ_منزل_Wait_For;
    بنياد_Type_t xClearOnExit;
    بنياد_Type_t x_Wait_For_All_بٽ;
    TickType_t x_Ticks_منزل_Wait;
} x_Event_گروهه_Wait_بٽ_Params_t;

/* MPU versions of task.h API functions. */
فراغ MPU_vTaskDelay( ثابت TickType_t x_Ticks_منزل_Delay ) FREERTOS_SYSTEM_CALL;
بنياد_Type_t MPU_xTaskDelayUntil( TickType_t * ثابت px_اڳيون_Wake_وقت,
                                ثابت TickType_t x_وقت_Increment ) FREERTOS_SYSTEM_CALL;
بنياد_Type_t MPU_xTaskAbortDelay( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_ترجيح_Get( ثابت TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
eTaskState MPU_eTaskGetState( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vTaskGetInfo( TaskHandle_t xTask,
                       Task_حالت_t * px_Task_حالت,
                       بنياد_Type_t x_Get_Free_ڍير_Space,
                       eTaskState eState ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vTaskSuspend( TaskHandle_t x_Task_منزل_Suspend ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vTaskResume( TaskHandle_t x_Task_منزل_Resume ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_x_Task_Get_Tick_ڳڻپ( فراغ ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_Get_عدد_جو_Tasks( فراغ ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_Get_ڍير_مٿاهون_Water_Mark( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
config_ڍير_اونهائي_TYPE MPU_ux_Task_Get_ڍير_مٿاهون_Water_Mark2( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_Task_سيٽ_Application_Task_ٽيگ( TaskHandle_t xTask,
                                     TaskHookFunction_t pxHookFunction ) FREERTOS_SYSTEM_CALL;
TaskHookFunction_t MPU_x_Task_Get_Application_Task_ٽيگ( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_Task_سيٽ_Thread_Local_Storage_اشارو( TaskHandle_t x_Task_منزل_سيٽ,
                                            بنياد_Type_t x_اشاريو,
                                            فراغ * pv_قدر ) FREERTOS_SYSTEM_CALL;
فراغ * MPU_pv_Task_Get_Thread_Local_Storage_اشارو( TaskHandle_t x_Task_منزل_Query,
                                               بنياد_Type_t x_اشاريو ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetIdleTaskHandle( فراغ ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxTaskGetSystemState( Task_حالت_t * ثابت px_Task_حالت_Array,
                                      ثابت UBaseType_t ux_Array_سائيز,
                                      config_لاڳيتو_وقت_COUNTER_TYPE * ثابت pul_ڪل_لاڳيتو_وقت ) FREERTOS_SYSTEM_CALL;
config_لاڳيتو_وقت_COUNTER_TYPE MPU_ul_Task_Get_لاڳيتو_وقت_Counter( ثابت TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
config_لاڳيتو_وقت_COUNTER_TYPE MPU_ul_Task_Get_لاڳيتو_وقت_Percent( ثابت TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
config_لاڳيتو_وقت_COUNTER_TYPE MPU_ul_Task_Get_Idle_لاڳيتو_وقت_Counter( فراغ ) FREERTOS_SYSTEM_CALL;
config_لاڳيتو_وقت_COUNTER_TYPE MPU_ul_Task_Get_Idle_لاڳيتو_وقت_Percent( فراغ ) FREERTOS_SYSTEM_CALL;
بنياد_Type_t MPU_xTaskGenericNotify( TaskHandle_t x_Task_منزل_Notify,
                                   UBaseType_t ux_اشاريو_منزل_Notify,
                                   uint32_t ul_قدر,
                                   eNotifyAction eAction,
                                   uint32_t * pul_اڳيون_Notification_قدر ) FREERTOS_SYSTEM_CALL;
بنياد_Type_t MPU_x_Task_Generic_Notify_داخلا( ثابت xTaskGenericNotifyParams_t * pxParams ) FREERTOS_SYSTEM_CALL;
بنياد_Type_t MPU_xTaskGenericNotifyWait( UBaseType_t ux_اشاريو_منزل_Wait_On,
                                       uint32_t ul_بٽ_منزل_Clear_On_داخلا,
                                       uint32_t ul_بٽ_منزل_Clear_On_Exit,
                                       uint32_t * pul_Notification_قدر,
                                       TickType_t x_Ticks_منزل_Wait ) FREERTOS_SYSTEM_CALL;
بنياد_Type_t MPU_x_Task_Generic_Notify_Wait_داخلا( ثابت xTaskGenericNotifyWaitParams_t * pxParams ) FREERTOS_SYSTEM_CALL;
uint32_t MPU_ulTaskGenericNotifyTake( UBaseType_t ux_اشاريو_منزل_Wait_On,
                                      بنياد_Type_t x_Clear_ڳڻپ_On_Exit,
                                      TickType_t x_Ticks_منزل_Wait ) FREERTOS_SYSTEM_CALL;
بنياد_Type_t MPU_xTaskGenericNotifyStateClear( TaskHandle_t xTask,
                                             UBaseType_t ux_اشاريو_منزل_Clear ) FREERTOS_SYSTEM_CALL;
uint32_t MPU_ul_Task_Generic_Notify_قدر_Clear( TaskHandle_t xTask,
                                            UBaseType_t ux_اشاريو_منزل_Clear,
                                            uint32_t ul_بٽ_منزل_Clear ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_Task_سيٽ_وقت_ٻاهريون_State( وقت_ٻاهريون_t * ثابت px_وقت_ٻاهريون ) FREERTOS_SYSTEM_CALL;
بنياد_Type_t MPU_x_Task_Check_For_وقت_ٻاهريون( وقت_ٻاهريون_t * ثابت px_وقت_ٻاهريون,
                                     TickType_t * ثابت px_Ticks_منزل_Wait ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetCurrentTaskHandle( فراغ ) FREERTOS_SYSTEM_CALL;
بنياد_Type_t MPU_xTaskGetSchedulerState( فراغ ) FREERTOS_SYSTEM_CALL;

/* Privileged only wrappers for Task APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
بنياد_Type_t MPU_xTaskCreate( TaskFunction_t px_Task_ڪوڊ,
                            ثابت حرف * ثابت pcName,
                            ثابت config_ڍير_اونهائي_TYPE ux_ڍير_اونهائي,
                            فراغ * ثابت pvParameters,
                            UBaseType_t ux_ترجيح,
                            TaskHandle_t * ثابت pxCreatedTask ) PRIVILEGED_FUNCTION;
TaskHandle_t MPU_xTaskCreateStatic( TaskFunction_t px_Task_ڪوڊ,
                                    ثابت حرف * ثابت pcName,
                                    ثابت config_ڍير_اونهائي_TYPE ux_ڍير_اونهائي,
                                    فراغ * ثابت pvParameters,
                                    UBaseType_t ux_ترجيح,
                                    ڍير_Type_t * ثابت pux_ڍير_بفر,
                                    StaticTask_t * ثابت px_Task_بفر ) PRIVILEGED_FUNCTION;
فراغ MPU_vTaskDelete( TaskHandle_t x_Task_منزل_Delete ) PRIVILEGED_FUNCTION;
فراغ MPU_v_Task_ترجيح_سيٽ( TaskHandle_t xTask,
                           UBaseType_t ux_New_ترجيح ) PRIVILEGED_FUNCTION;
TaskHandle_t MPU_xTaskGetHandle( ثابت حرف * pc_Name_منزل_Query ) PRIVILEGED_FUNCTION;
بنياد_Type_t MPU_xTaskCallApplicationTaskHook( TaskHandle_t xTask,
                                             فراغ * pvParameter ) PRIVILEGED_FUNCTION;
حرف * MPU_pcTaskGetName( TaskHandle_t x_Task_منزل_Query ) PRIVILEGED_FUNCTION;
بنياد_Type_t MPU_xTaskCreateRestricted( ثابت TaskParameters_t * ثابت pxTaskDefinition,
                                      TaskHandle_t * pxCreatedTask ) PRIVILEGED_FUNCTION;
بنياد_Type_t MPU_xTaskCreateRestrictedStatic( ثابت TaskParameters_t * ثابت pxTaskDefinition,
                                            TaskHandle_t * pxCreatedTask ) PRIVILEGED_FUNCTION;
فراغ MPU_vTaskAllocateMPURegions( TaskHandle_t x_Task_منزل_Modify,
                                  ثابت يادگيري_Region_t * ثابت xRegions ) PRIVILEGED_FUNCTION;
بنياد_Type_t MPU_xTaskGetStaticBuffers( TaskHandle_t xTask,
                                      ڍير_Type_t ** ppux_ڍير_بفر,
                                      StaticTask_t ** ppx_Task_بفر ) PRIVILEGED_FUNCTION;
UBaseType_t MPU_ux_Task_ترجيح_Get_From_ISR( ثابت TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
UBaseType_t MPU_ux_Task_بنياد_ترجيح_Get( ثابت TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
UBaseType_t MPU_ux_Task_بنياد_ترجيح_Get_From_ISR( ثابت TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
بنياد_Type_t MPU_xTaskResumeFromISR( TaskHandle_t x_Task_منزل_Resume ) PRIVILEGED_FUNCTION;
TaskHookFunction_t MPU_x_Task_Get_Application_Task_ٽيگ_From_ISR( TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
بنياد_Type_t MPU_xTaskGenericNotifyFromISR( TaskHandle_t x_Task_منزل_Notify,
                                          UBaseType_t ux_اشاريو_منزل_Notify,
                                          uint32_t ul_قدر,
                                          eNotifyAction eAction,
                                          uint32_t * pul_اڳيون_Notification_قدر,
                                          بنياد_Type_t * px_Higher_ترجيح_Task_Woken ) PRIVILEGED_FUNCTION;
فراغ MPU_vTaskGenericNotifyGiveFromISR( TaskHandle_t x_Task_منزل_Notify,
                                        UBaseType_t ux_اشاريو_منزل_Notify,
                                        بنياد_Type_t * px_Higher_ترجيح_Task_Woken ) PRIVILEGED_FUNCTION;

/* MPU versions of queue.h API functions. */
بنياد_Type_t MPU_x_قطار_Generic_Send( قطار_Handle_t x_قطار,
                                  ثابت فراغ * ثابت pv_شي_منزل_قطار,
                                  TickType_t x_Ticks_منزل_Wait,
                                  ثابت بنياد_Type_t xCopyPosition ) FREERTOS_SYSTEM_CALL;
بنياد_Type_t MPU_x_قطار_Receive( قطار_Handle_t x_قطار,
                              فراغ * ثابت pv_بفر,
                              TickType_t x_Ticks_منزل_Wait ) FREERTOS_SYSTEM_CALL;
بنياد_Type_t MPU_x_قطار_Peek( قطار_Handle_t x_قطار,
                           فراغ * ثابت pv_بفر,
                           TickType_t x_Ticks_منزل_Wait ) FREERTOS_SYSTEM_CALL;
بنياد_Type_t MPU_x_قطار_Semaphore_Take( قطار_Handle_t x_قطار,
                                    TickType_t x_Ticks_منزل_Wait ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_قطار_Messages_انتظار( ثابت قطار_Handle_t x_قطار ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_قطار_Spaces_موجود( ثابت قطار_Handle_t x_قطار ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_x_قطار_Get_Mutex_Holder( قطار_Handle_t xSemaphore ) FREERTOS_SYSTEM_CALL;
بنياد_Type_t MPU_x_قطار_Take_Mutex_Recursive( قطار_Handle_t xMutex,
                                         TickType_t x_Ticks_منزل_Wait ) FREERTOS_SYSTEM_CALL;
بنياد_Type_t MPU_x_قطار_Give_Mutex_Recursive( قطار_Handle_t pxMutex ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_قطار_جوڙيو_منزل_Registry( قطار_Handle_t x_قطار,
                              ثابت حرف * pcName ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_قطار_Unregister_قطار( قطار_Handle_t x_قطار ) FREERTOS_SYSTEM_CALL;
ثابت حرف * MPU_pc_قطار_Get_Name( قطار_Handle_t x_قطار ) FREERTOS_SYSTEM_CALL;
بنياد_Type_t MPU_x_قطار_جوڙيو_منزل_سيٽ( قطار_سيٽ_Member_Handle_t x_قطار_Or_Semaphore,
                               قطار_سيٽ_Handle_t x_قطار_سيٽ ) FREERTOS_SYSTEM_CALL;
قطار_سيٽ_Member_Handle_t MPU_x_قطار_Select_From_سيٽ( قطار_سيٽ_Handle_t x_قطار_سيٽ,
                                                ثابت TickType_t x_Ticks_منزل_Wait ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_قطار_سيٽ_قطار_عدد( قطار_Handle_t x_قطار,
                               UBaseType_t ux_قطار_عدد ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_قطار_Get_قطار_عدد( قطار_Handle_t x_قطار ) FREERTOS_SYSTEM_CALL;
uint8_t MPU_uc_قطار_Get_قطار_Type( قطار_Handle_t x_قطار ) FREERTOS_SYSTEM_CALL;

/* Privileged only wrappers for Queue APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
فراغ MPU_v_قطار_Delete( قطار_Handle_t x_قطار ) PRIVILEGED_FUNCTION;
قطار_Handle_t MPU_x_قطار_Create_Mutex( ثابت uint8_t uc_قطار_Type ) PRIVILEGED_FUNCTION;
قطار_Handle_t MPU_x_قطار_Create_Mutex_Static( ثابت uint8_t uc_قطار_Type,
                                           Static_قطار_t * px_Static_قطار ) PRIVILEGED_FUNCTION;
قطار_Handle_t MPU_x_قطار_Create_Counting_Semaphore( ثابت UBaseType_t ux_Max_ڳڻپ,
                                                 ثابت UBaseType_t ux_Initial_ڳڻپ ) PRIVILEGED_FUNCTION;
قطار_Handle_t MPU_x_قطار_Create_Counting_Semaphore_Static( ثابت UBaseType_t ux_Max_ڳڻپ,
                                                       ثابت UBaseType_t ux_Initial_ڳڻپ,
                                                       Static_قطار_t * px_Static_قطار ) PRIVILEGED_FUNCTION;
قطار_Handle_t MPU_x_قطار_Generic_Create( ثابت UBaseType_t ux_قطار_ڊيگهائي,
                                       ثابت UBaseType_t ux_شي_سائيز,
                                       ثابت uint8_t uc_قطار_Type ) PRIVILEGED_FUNCTION;
قطار_Handle_t MPU_x_قطار_Generic_Create_Static( ثابت UBaseType_t ux_قطار_ڊيگهائي,
                                             ثابت UBaseType_t ux_شي_سائيز,
                                             uint8_t * puc_قطار_Storage,
                                             Static_قطار_t * px_Static_قطار,
                                             ثابت uint8_t uc_قطار_Type ) PRIVILEGED_FUNCTION;
قطار_سيٽ_Handle_t MPU_x_قطار_Create_سيٽ( ثابت UBaseType_t ux_Event_قطار_ڊيگهائي ) PRIVILEGED_FUNCTION;
بنياد_Type_t MPU_x_قطار_Remove_From_سيٽ( قطار_سيٽ_Member_Handle_t x_قطار_Or_Semaphore,
                                    قطار_سيٽ_Handle_t x_قطار_سيٽ ) PRIVILEGED_FUNCTION;
بنياد_Type_t MPU_x_قطار_Generic_Reset( قطار_Handle_t x_قطار,
                                   بنياد_Type_t x_New_قطار ) PRIVILEGED_FUNCTION;
بنياد_Type_t MPU_x_قطار_Generic_Get_Static_Buffers( قطار_Handle_t x_قطار,
                                              uint8_t ** ppuc_قطار_Storage,
                                              Static_قطار_t ** ppx_Static_قطار ) PRIVILEGED_FUNCTION;
بنياد_Type_t MPU_x_قطار_Generic_Send_From_ISR( قطار_Handle_t x_قطار,
                                         ثابت فراغ * ثابت pv_شي_منزل_قطار,
                                         بنياد_Type_t * ثابت px_Higher_ترجيح_Task_Woken,
                                         ثابت بنياد_Type_t xCopyPosition ) PRIVILEGED_FUNCTION;
بنياد_Type_t MPU_x_قطار_Give_From_ISR( قطار_Handle_t x_قطار,
                                  بنياد_Type_t * ثابت px_Higher_ترجيح_Task_Woken ) PRIVILEGED_FUNCTION;
بنياد_Type_t MPU_x_قطار_Peek_From_ISR( قطار_Handle_t x_قطار,
                                  فراغ * ثابت pv_بفر ) PRIVILEGED_FUNCTION;
بنياد_Type_t MPU_x_قطار_Receive_From_ISR( قطار_Handle_t x_قطار,
                                     فراغ * ثابت pv_بفر,
                                     بنياد_Type_t * ثابت px_Higher_ترجيح_Task_Woken ) PRIVILEGED_FUNCTION;
بنياد_Type_t MPU_x_قطار_Is_قطار_خالي_جايون_From_ISR( ثابت قطار_Handle_t x_قطار ) PRIVILEGED_FUNCTION;
بنياد_Type_t MPU_x_قطار_Is_قطار_ڀريل_From_ISR( ثابت قطار_Handle_t x_قطار ) PRIVILEGED_FUNCTION;
UBaseType_t MPU_ux_قطار_Messages_انتظار_From_ISR( ثابت قطار_Handle_t x_قطار ) PRIVILEGED_FUNCTION;
TaskHandle_t MPU_x_قطار_Get_Mutex_Holder_From_ISR( قطار_Handle_t xSemaphore ) PRIVILEGED_FUNCTION;
قطار_سيٽ_Member_Handle_t MPU_x_قطار_Select_From_سيٽ_From_ISR( قطار_سيٽ_Handle_t x_قطار_سيٽ ) PRIVILEGED_FUNCTION;

/* MPU versions of timers.h API functions. */
فراغ * MPU_pvTimerGetTimerID( ثابت TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_Timer_سيٽ_Timer_ID( TimerHandle_t xTimer,
                           فراغ * pvNewID ) FREERTOS_SYSTEM_CALL;
بنياد_Type_t MPU_x_Timer_Is_Timer_سرگرم( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTimerGetTimerDaemonTaskHandle( فراغ ) FREERTOS_SYSTEM_CALL;
بنياد_Type_t MPU_xTimerGenericCommandFromTask( TimerHandle_t xTimer,
                                             ثابت بنياد_Type_t xCommandID,
                                             ثابت TickType_t x_Optional_قدر,
                                             بنياد_Type_t * ثابت px_Higher_ترجيح_Task_Woken,
                                             ثابت TickType_t x_Ticks_منزل_Wait ) FREERTOS_SYSTEM_CALL;
بنياد_Type_t MPU_x_Timer_Generic_Command_From_Task_داخلا( ثابت xTimerGenericCommandFromTaskParams_t * pxParams ) FREERTOS_SYSTEM_CALL;
ثابت حرف * MPU_pcTimerGetName( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_Timer_سيٽ_Reload_گهڻائي( TimerHandle_t xTimer,
                              ثابت بنياد_Type_t uxAutoReload ) FREERTOS_SYSTEM_CALL;
بنياد_Type_t MPU_x_Timer_Get_Reload_گهڻائي( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Timer_Get_Reload_گهڻائي( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_xTimerGetPeriod( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_x_Timer_Get_Expiry_وقت( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;

/* Privileged only wrappers for Timer APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
TimerHandle_t MPU_xTimerCreate( ثابت حرف * ثابت pcTimerName,
                                ثابت TickType_t x_Timer_Period_اندريون_Ticks,
                                ثابت UBaseType_t uxAutoReload,
                                فراغ * ثابت pvTimerID,
                                TimerCallbackFunction_t pxCallbackFunction ) PRIVILEGED_FUNCTION;
TimerHandle_t MPU_xTimerCreateStatic( ثابت حرف * ثابت pcTimerName,
                                      ثابت TickType_t x_Timer_Period_اندريون_Ticks,
                                      ثابت UBaseType_t uxAutoReload,
                                      فراغ * ثابت pvTimerID,
                                      TimerCallbackFunction_t pxCallbackFunction,
                                      StaticTimer_t * px_Timer_بفر ) PRIVILEGED_FUNCTION;
بنياد_Type_t MPU_x_Timer_Get_Static_بفر( TimerHandle_t xTimer,
                                      StaticTimer_t ** ppx_Timer_بفر ) PRIVILEGED_FUNCTION;
بنياد_Type_t MPU_xTimerGenericCommandFromISR( TimerHandle_t xTimer,
                                            ثابت بنياد_Type_t xCommandID,
                                            ثابت TickType_t x_Optional_قدر,
                                            بنياد_Type_t * ثابت px_Higher_ترجيح_Task_Woken,
                                            ثابت TickType_t x_Ticks_منزل_Wait ) PRIVILEGED_FUNCTION;

/* MPU versions of event_group.h API functions. */
Event_بٽ_t MPU_x_Event_گروهه_Wait_بٽ( Event_گروهه_Handle_t x_Event_گروهه,
                                     ثابت Event_بٽ_t ux_بٽ_منزل_Wait_For,
                                     ثابت بنياد_Type_t xClearOnExit,
                                     ثابت بنياد_Type_t x_Wait_For_All_بٽ,
                                     TickType_t x_Ticks_منزل_Wait ) FREERTOS_SYSTEM_CALL;
Event_بٽ_t MPU_x_Event_گروهه_Wait_بٽ_داخلا( ثابت x_Event_گروهه_Wait_بٽ_Params_t * pxParams ) FREERTOS_SYSTEM_CALL;
Event_بٽ_t MPU_x_Event_گروهه_Clear_بٽ( Event_گروهه_Handle_t x_Event_گروهه,
                                      ثابت Event_بٽ_t ux_بٽ_منزل_Clear ) FREERTOS_SYSTEM_CALL;
Event_بٽ_t MPU_x_Event_گروهه_سيٽ_بٽ( Event_گروهه_Handle_t x_Event_گروهه,
                                    ثابت Event_بٽ_t ux_بٽ_منزل_سيٽ ) FREERTOS_SYSTEM_CALL;
Event_بٽ_t MPU_x_Event_گروهه_Sync( Event_گروهه_Handle_t x_Event_گروهه,
                                 ثابت Event_بٽ_t ux_بٽ_منزل_سيٽ,
                                 ثابت Event_بٽ_t ux_بٽ_منزل_Wait_For,
                                 TickType_t x_Ticks_منزل_Wait ) FREERTOS_SYSTEM_CALL;
#إذا ( configUSE_TRACE_FACILITY == 1 )
    UBaseType_t MPU_ux_Event_گروهه_Get_عدد( فراغ * x_Event_گروهه ) FREERTOS_SYSTEM_CALL;
    فراغ MPU_v_Event_گروهه_سيٽ_عدد( فراغ * x_Event_گروهه,
                                   UBaseType_t ux_Event_گروهه_عدد ) FREERTOS_SYSTEM_CALL;
#جيڪڏهن_پورو /* ( configUSE_TRACE_FACILITY == 1 )*/

/* Privileged only wrappers for Event Group APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
Event_گروهه_Handle_t MPU_x_Event_گروهه_Create( فراغ ) PRIVILEGED_FUNCTION;
Event_گروهه_Handle_t MPU_x_Event_گروهه_Create_Static( Static_Event_گروهه_t * px_Event_گروهه_بفر ) PRIVILEGED_FUNCTION;
فراغ MPU_v_Event_گروهه_Delete( Event_گروهه_Handle_t x_Event_گروهه ) PRIVILEGED_FUNCTION;
بنياد_Type_t MPU_x_Event_گروهه_Get_Static_بفر( Event_گروهه_Handle_t x_Event_گروهه,
                                           Static_Event_گروهه_t ** ppx_Event_گروهه_بفر ) PRIVILEGED_FUNCTION;
بنياد_Type_t MPU_x_Event_گروهه_Clear_بٽ_From_ISR( Event_گروهه_Handle_t x_Event_گروهه,
                                            ثابت Event_بٽ_t ux_بٽ_منزل_Clear ) PRIVILEGED_FUNCTION;
بنياد_Type_t MPU_x_Event_گروهه_سيٽ_بٽ_From_ISR( Event_گروهه_Handle_t x_Event_گروهه,
                                          ثابت Event_بٽ_t ux_بٽ_منزل_سيٽ,
                                          بنياد_Type_t * px_Higher_ترجيح_Task_Woken ) PRIVILEGED_FUNCTION;
Event_بٽ_t MPU_x_Event_گروهه_Get_بٽ_From_ISR( Event_گروهه_Handle_t x_Event_گروهه ) PRIVILEGED_FUNCTION;

/* MPU versions of message/stream_buffer.h API functions. */
size_t MPU_x_وهڪرو_بفر_Send( وهڪرو_بفر_Handle_t x_وهڪرو_بفر,
                              ثابت فراغ * pv_Tx_معلومات,
                              size_t x_معلومات_ڊيگهائي_بائيٽون,
                              TickType_t x_Ticks_منزل_Wait ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_وهڪرو_بفر_Receive( وهڪرو_بفر_Handle_t x_وهڪرو_بفر,
                                 فراغ * pv_Rx_معلومات,
                                 size_t x_بفر_ڊيگهائي_بائيٽون,
                                 TickType_t x_Ticks_منزل_Wait ) FREERTOS_SYSTEM_CALL;
بنياد_Type_t MPU_x_وهڪرو_بفر_Is_ڀريل( وهڪرو_بفر_Handle_t x_وهڪرو_بفر ) FREERTOS_SYSTEM_CALL;
بنياد_Type_t MPU_x_وهڪرو_بفر_Is_خالي_جايون( وهڪرو_بفر_Handle_t x_وهڪرو_بفر ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_وهڪرو_بفر_Spaces_موجود( وهڪرو_بفر_Handle_t x_وهڪرو_بفر ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_وهڪرو_بفر_بائيٽون_موجود( وهڪرو_بفر_Handle_t x_وهڪرو_بفر ) FREERTOS_SYSTEM_CALL;
بنياد_Type_t MPU_x_وهڪرو_بفر_سيٽ_Trigger_Level( وهڪرو_بفر_Handle_t x_وهڪرو_بفر,
                                             size_t xTriggerLevel ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_وهڪرو_بفر_ايندڙ_پيغام_ڊيگهائي_بائيٽون( وهڪرو_بفر_Handle_t x_وهڪرو_بفر ) FREERTOS_SYSTEM_CALL;

/* Privileged only wrappers for Stream Buffer APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
وهڪرو_بفر_Handle_t MPU_x_وهڪرو_بفر_Generic_Create( size_t x_بفر_سائيز_بائيٽون,
                                                     size_t x_Trigger_Level_بائيٽون,
                                                     بنياد_Type_t x_وهڪرو_بفر_Type,
                                                     وهڪرو_بفر_Callback_Function_t pxSendCompletedCallback,
                                                     وهڪرو_بفر_Callback_Function_t pxReceiveCompletedCallback ) PRIVILEGED_FUNCTION;
وهڪرو_بفر_Handle_t MPU_x_وهڪرو_بفر_Generic_Create_Static( size_t x_بفر_سائيز_بائيٽون,
                                                           size_t x_Trigger_Level_بائيٽون,
                                                           بنياد_Type_t x_وهڪرو_بفر_Type,
                                                           uint8_t * ثابت puc_وهڪرو_بفر_Storage_Area,
                                                           Static_وهڪرو_بفر_t * ثابت px_Static_وهڪرو_بفر,
                                                           وهڪرو_بفر_Callback_Function_t pxSendCompletedCallback,
                                                           وهڪرو_بفر_Callback_Function_t pxReceiveCompletedCallback ) PRIVILEGED_FUNCTION;
فراغ MPU_v_وهڪرو_بفر_Delete( وهڪرو_بفر_Handle_t x_وهڪرو_بفر ) PRIVILEGED_FUNCTION;
بنياد_Type_t MPU_x_وهڪرو_بفر_Reset( وهڪرو_بفر_Handle_t x_وهڪرو_بفر ) PRIVILEGED_FUNCTION;
بنياد_Type_t MPU_x_وهڪرو_بفر_Get_Static_Buffers( وهڪرو_بفر_Handle_t x_وهڪرو_Buffers,
                                              uint8_t * ppuc_وهڪرو_بفر_Storage_Area,
                                              Static_وهڪرو_بفر_t * ppx_Static_وهڪرو_بفر ) PRIVILEGED_FUNCTION;
size_t MPU_x_وهڪرو_بفر_Send_From_ISR( وهڪرو_بفر_Handle_t x_وهڪرو_بفر,
                                     ثابت فراغ * pv_Tx_معلومات,
                                     size_t x_معلومات_ڊيگهائي_بائيٽون,
                                     بنياد_Type_t * ثابت px_Higher_ترجيح_Task_Woken ) PRIVILEGED_FUNCTION;
size_t MPU_x_وهڪرو_بفر_Receive_From_ISR( وهڪرو_بفر_Handle_t x_وهڪرو_بفر,
                                        فراغ * pv_Rx_معلومات,
                                        size_t x_بفر_ڊيگهائي_بائيٽون,
                                        بنياد_Type_t * ثابت px_Higher_ترجيح_Task_Woken ) PRIVILEGED_FUNCTION;
بنياد_Type_t MPU_x_وهڪرو_بفر_Send_Completed_From_ISR( وهڪرو_بفر_Handle_t x_وهڪرو_بفر,
                                                  بنياد_Type_t * px_Higher_ترجيح_Task_Woken ) PRIVILEGED_FUNCTION;
بنياد_Type_t MPU_x_وهڪرو_بفر_Receive_Completed_From_ISR( وهڪرو_بفر_Handle_t x_وهڪرو_بفر,
                                                     بنياد_Type_t * px_Higher_ترجيح_Task_Woken ) PRIVILEGED_FUNCTION;
بنياد_Type_t MPU_x_وهڪرو_بفر_Reset_From_ISR( وهڪرو_بفر_Handle_t x_وهڪرو_بفر ) PRIVILEGED_FUNCTION;

#جيڪڏهن_پورو /* MPU_PROTOTYPES_H */
