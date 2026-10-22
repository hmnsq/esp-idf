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


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ MPU_PROTOTYPES_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MPU_PROTOTYPES_H

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ xTaskGenericNotifyParams
{
    TaskHandle_t x_Task_ਮੰਜ਼ਿਲ_Notify;
    UBaseType_t ux_ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_Notify;
    uint32_t ul_ਮੁੱਲ;
    eNotifyAction eAction;
    uint32_t * pul_ਪਿਛਲਾ_Notification_ਮੁੱਲ;
} xTaskGenericNotifyParams_t;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ xTaskGenericNotifyWaitParams
{
    UBaseType_t ux_ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_Wait_On;
    uint32_t ul_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear_On_ਇੰਦਰਾਜ;
    uint32_t ul_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear_On_Exit;
    uint32_t * pul_Notification_ਮੁੱਲ;
    TickType_t x_Ticks_ਮੰਜ਼ਿਲ_Wait;
} xTaskGenericNotifyWaitParams_t;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ xTimerGenericCommandFromTaskParams
{
    TimerHandle_t xTimer;
    ਅਧਾਰ_Type_t xCommandID;
    TickType_t x_Optional_ਮੁੱਲ;
    ਅਧਾਰ_Type_t * px_Higher_ਤਰਜੀਹ_Task_Woken;
    TickType_t x_Ticks_ਮੰਜ਼ਿਲ_Wait;
} xTimerGenericCommandFromTaskParams_t;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ x_Event_ਸਮੂਹ_Wait_ਬਿੱਟ_Params
{
    Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ;
    Event_ਬਿੱਟ_t ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For;
    ਅਧਾਰ_Type_t xClearOnExit;
    ਅਧਾਰ_Type_t x_Wait_For_All_ਬਿੱਟ;
    TickType_t x_Ticks_ਮੰਜ਼ਿਲ_Wait;
} x_Event_ਸਮੂਹ_Wait_ਬਿੱਟ_Params_t;

/* MPU versions of task.h API functions. */
ਖਾਲੀ MPU_vTaskDelay( ਅਚਲ TickType_t x_Ticks_ਮੰਜ਼ਿਲ_Delay ) FREERTOS_SYSTEM_CALL;
ਅਧਾਰ_Type_t MPU_xTaskDelayUntil( TickType_t * ਅਚਲ px_ਪਿਛਲਾ_Wake_ਸਮਾਂ,
                                ਅਚਲ TickType_t x_ਸਮਾਂ_Increment ) FREERTOS_SYSTEM_CALL;
ਅਧਾਰ_Type_t MPU_xTaskAbortDelay( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_ਤਰਜੀਹ_Get( ਅਚਲ TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
eTaskState MPU_eTaskGetState( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
ਖਾਲੀ MPU_vTaskGetInfo( TaskHandle_t xTask,
                       Task_ਸਥਿਤੀ_t * px_Task_ਸਥਿਤੀ,
                       ਅਧਾਰ_Type_t x_Get_Free_ਢੇਰ_Space,
                       eTaskState eState ) FREERTOS_SYSTEM_CALL;
ਖਾਲੀ MPU_vTaskSuspend( TaskHandle_t x_Task_ਮੰਜ਼ਿਲ_Suspend ) FREERTOS_SYSTEM_CALL;
ਖਾਲੀ MPU_vTaskResume( TaskHandle_t x_Task_ਮੰਜ਼ਿਲ_Resume ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_x_Task_Get_Tick_ਗਿਣਤੀ( ਖਾਲੀ ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_Get_ਸੰਖਿਆ_ਦਾ_Tasks( ਖਾਲੀ ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_Get_ਢੇਰ_ਉੱਚਾ_Water_Mark( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
config_ਢੇਰ_ਡੂੰਘਾਈ_TYPE MPU_ux_Task_Get_ਢੇਰ_ਉੱਚਾ_Water_Mark2( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
ਖਾਲੀ MPU_v_Task_ਸੈੱਟ_Application_Task_ਟੈਗ( TaskHandle_t xTask,
                                     TaskHookFunction_t pxHookFunction ) FREERTOS_SYSTEM_CALL;
TaskHookFunction_t MPU_x_Task_Get_Application_Task_ਟੈਗ( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
ਖਾਲੀ MPU_v_Task_ਸੈੱਟ_Thread_Local_Storage_ਸੂਚਕ( TaskHandle_t x_Task_ਮੰਜ਼ਿਲ_ਸੈੱਟ,
                                            ਅਧਾਰ_Type_t x_ਸੂਚਕਾਂਕ,
                                            ਖਾਲੀ * pv_ਮੁੱਲ ) FREERTOS_SYSTEM_CALL;
ਖਾਲੀ * MPU_pv_Task_Get_Thread_Local_Storage_ਸੂਚਕ( TaskHandle_t x_Task_ਮੰਜ਼ਿਲ_Query,
                                               ਅਧਾਰ_Type_t x_ਸੂਚਕਾਂਕ ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetIdleTaskHandle( ਖਾਲੀ ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxTaskGetSystemState( Task_ਸਥਿਤੀ_t * ਅਚਲ px_Task_ਸਥਿਤੀ_Array,
                                      ਅਚਲ UBaseType_t ux_Array_ਮਾਪ,
                                      config_ਲੜੀਵਾਰ_ਸਮਾਂ_COUNTER_TYPE * ਅਚਲ pul_ਕੁੱਲ_ਲੜੀਵਾਰ_ਸਮਾਂ ) FREERTOS_SYSTEM_CALL;
config_ਲੜੀਵਾਰ_ਸਮਾਂ_COUNTER_TYPE MPU_ul_Task_Get_ਲੜੀਵਾਰ_ਸਮਾਂ_Counter( ਅਚਲ TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
config_ਲੜੀਵਾਰ_ਸਮਾਂ_COUNTER_TYPE MPU_ul_Task_Get_ਲੜੀਵਾਰ_ਸਮਾਂ_Percent( ਅਚਲ TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
config_ਲੜੀਵਾਰ_ਸਮਾਂ_COUNTER_TYPE MPU_ul_Task_Get_Idle_ਲੜੀਵਾਰ_ਸਮਾਂ_Counter( ਖਾਲੀ ) FREERTOS_SYSTEM_CALL;
config_ਲੜੀਵਾਰ_ਸਮਾਂ_COUNTER_TYPE MPU_ul_Task_Get_Idle_ਲੜੀਵਾਰ_ਸਮਾਂ_Percent( ਖਾਲੀ ) FREERTOS_SYSTEM_CALL;
ਅਧਾਰ_Type_t MPU_xTaskGenericNotify( TaskHandle_t x_Task_ਮੰਜ਼ਿਲ_Notify,
                                   UBaseType_t ux_ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_Notify,
                                   uint32_t ul_ਮੁੱਲ,
                                   eNotifyAction eAction,
                                   uint32_t * pul_ਪਿਛਲਾ_Notification_ਮੁੱਲ ) FREERTOS_SYSTEM_CALL;
ਅਧਾਰ_Type_t MPU_x_Task_Generic_Notify_ਇੰਦਰਾਜ( ਅਚਲ xTaskGenericNotifyParams_t * pxParams ) FREERTOS_SYSTEM_CALL;
ਅਧਾਰ_Type_t MPU_xTaskGenericNotifyWait( UBaseType_t ux_ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_Wait_On,
                                       uint32_t ul_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear_On_ਇੰਦਰਾਜ,
                                       uint32_t ul_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear_On_Exit,
                                       uint32_t * pul_Notification_ਮੁੱਲ,
                                       TickType_t x_Ticks_ਮੰਜ਼ਿਲ_Wait ) FREERTOS_SYSTEM_CALL;
ਅਧਾਰ_Type_t MPU_x_Task_Generic_Notify_Wait_ਇੰਦਰਾਜ( ਅਚਲ xTaskGenericNotifyWaitParams_t * pxParams ) FREERTOS_SYSTEM_CALL;
uint32_t MPU_ulTaskGenericNotifyTake( UBaseType_t ux_ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_Wait_On,
                                      ਅਧਾਰ_Type_t x_Clear_ਗਿਣਤੀ_On_Exit,
                                      TickType_t x_Ticks_ਮੰਜ਼ਿਲ_Wait ) FREERTOS_SYSTEM_CALL;
ਅਧਾਰ_Type_t MPU_xTaskGenericNotifyStateClear( TaskHandle_t xTask,
                                             UBaseType_t ux_ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_Clear ) FREERTOS_SYSTEM_CALL;
uint32_t MPU_ul_Task_Generic_Notify_ਮੁੱਲ_Clear( TaskHandle_t xTask,
                                            UBaseType_t ux_ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_Clear,
                                            uint32_t ul_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear ) FREERTOS_SYSTEM_CALL;
ਖਾਲੀ MPU_v_Task_ਸੈੱਟ_ਸਮਾਂ_ਬਾਹਰਲਾ_State( ਸਮਾਂ_ਬਾਹਰਲਾ_t * ਅਚਲ px_ਸਮਾਂ_ਬਾਹਰਲਾ ) FREERTOS_SYSTEM_CALL;
ਅਧਾਰ_Type_t MPU_x_Task_Check_For_ਸਮਾਂ_ਬਾਹਰਲਾ( ਸਮਾਂ_ਬਾਹਰਲਾ_t * ਅਚਲ px_ਸਮਾਂ_ਬਾਹਰਲਾ,
                                     TickType_t * ਅਚਲ px_Ticks_ਮੰਜ਼ਿਲ_Wait ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetCurrentTaskHandle( ਖਾਲੀ ) FREERTOS_SYSTEM_CALL;
ਅਧਾਰ_Type_t MPU_xTaskGetSchedulerState( ਖਾਲੀ ) FREERTOS_SYSTEM_CALL;

/* Privileged only wrappers for Task APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
ਅਧਾਰ_Type_t MPU_xTaskCreate( TaskFunction_t px_Task_ਕੋਡ,
                            ਅਚਲ ਅੱਖਰ * ਅਚਲ pcName,
                            ਅਚਲ config_ਢੇਰ_ਡੂੰਘਾਈ_TYPE ux_ਢੇਰ_ਡੂੰਘਾਈ,
                            ਖਾਲੀ * ਅਚਲ pvParameters,
                            UBaseType_t ux_ਤਰਜੀਹ,
                            TaskHandle_t * ਅਚਲ pxCreatedTask ) PRIVILEGED_FUNCTION;
TaskHandle_t MPU_xTaskCreateStatic( TaskFunction_t px_Task_ਕੋਡ,
                                    ਅਚਲ ਅੱਖਰ * ਅਚਲ pcName,
                                    ਅਚਲ config_ਢੇਰ_ਡੂੰਘਾਈ_TYPE ux_ਢੇਰ_ਡੂੰਘਾਈ,
                                    ਖਾਲੀ * ਅਚਲ pvParameters,
                                    UBaseType_t ux_ਤਰਜੀਹ,
                                    ਢੇਰ_Type_t * ਅਚਲ pux_ਢੇਰ_ਬਫਰ,
                                    StaticTask_t * ਅਚਲ px_Task_ਬਫਰ ) PRIVILEGED_FUNCTION;
ਖਾਲੀ MPU_vTaskDelete( TaskHandle_t x_Task_ਮੰਜ਼ਿਲ_Delete ) PRIVILEGED_FUNCTION;
ਖਾਲੀ MPU_v_Task_ਤਰਜੀਹ_ਸੈੱਟ( TaskHandle_t xTask,
                           UBaseType_t ux_New_ਤਰਜੀਹ ) PRIVILEGED_FUNCTION;
TaskHandle_t MPU_xTaskGetHandle( ਅਚਲ ਅੱਖਰ * pc_Name_ਮੰਜ਼ਿਲ_Query ) PRIVILEGED_FUNCTION;
ਅਧਾਰ_Type_t MPU_xTaskCallApplicationTaskHook( TaskHandle_t xTask,
                                             ਖਾਲੀ * pvParameter ) PRIVILEGED_FUNCTION;
ਅੱਖਰ * MPU_pcTaskGetName( TaskHandle_t x_Task_ਮੰਜ਼ਿਲ_Query ) PRIVILEGED_FUNCTION;
ਅਧਾਰ_Type_t MPU_xTaskCreateRestricted( ਅਚਲ TaskParameters_t * ਅਚਲ pxTaskDefinition,
                                      TaskHandle_t * pxCreatedTask ) PRIVILEGED_FUNCTION;
ਅਧਾਰ_Type_t MPU_xTaskCreateRestrictedStatic( ਅਚਲ TaskParameters_t * ਅਚਲ pxTaskDefinition,
                                            TaskHandle_t * pxCreatedTask ) PRIVILEGED_FUNCTION;
ਖਾਲੀ MPU_vTaskAllocateMPURegions( TaskHandle_t x_Task_ਮੰਜ਼ਿਲ_Modify,
                                  ਅਚਲ ਸਮ੍ਰਿਤੀ_Region_t * ਅਚਲ xRegions ) PRIVILEGED_FUNCTION;
ਅਧਾਰ_Type_t MPU_xTaskGetStaticBuffers( TaskHandle_t xTask,
                                      ਢੇਰ_Type_t ** ppux_ਢੇਰ_ਬਫਰ,
                                      StaticTask_t ** ppx_Task_ਬਫਰ ) PRIVILEGED_FUNCTION;
UBaseType_t MPU_ux_Task_ਤਰਜੀਹ_Get_From_ISR( ਅਚਲ TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
UBaseType_t MPU_ux_Task_ਅਧਾਰ_ਤਰਜੀਹ_Get( ਅਚਲ TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
UBaseType_t MPU_ux_Task_ਅਧਾਰ_ਤਰਜੀਹ_Get_From_ISR( ਅਚਲ TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
ਅਧਾਰ_Type_t MPU_xTaskResumeFromISR( TaskHandle_t x_Task_ਮੰਜ਼ਿਲ_Resume ) PRIVILEGED_FUNCTION;
TaskHookFunction_t MPU_x_Task_Get_Application_Task_ਟੈਗ_From_ISR( TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
ਅਧਾਰ_Type_t MPU_xTaskGenericNotifyFromISR( TaskHandle_t x_Task_ਮੰਜ਼ਿਲ_Notify,
                                          UBaseType_t ux_ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_Notify,
                                          uint32_t ul_ਮੁੱਲ,
                                          eNotifyAction eAction,
                                          uint32_t * pul_ਪਿਛਲਾ_Notification_ਮੁੱਲ,
                                          ਅਧਾਰ_Type_t * px_Higher_ਤਰਜੀਹ_Task_Woken ) PRIVILEGED_FUNCTION;
ਖਾਲੀ MPU_vTaskGenericNotifyGiveFromISR( TaskHandle_t x_Task_ਮੰਜ਼ਿਲ_Notify,
                                        UBaseType_t ux_ਸੂਚਕਾਂਕ_ਮੰਜ਼ਿਲ_Notify,
                                        ਅਧਾਰ_Type_t * px_Higher_ਤਰਜੀਹ_Task_Woken ) PRIVILEGED_FUNCTION;

/* MPU versions of queue.h API functions. */
ਅਧਾਰ_Type_t MPU_x_ਕਤਾਰ_Generic_Send( ਕਤਾਰ_Handle_t x_ਕਤਾਰ,
                                  ਅਚਲ ਖਾਲੀ * ਅਚਲ pv_ਵਸਤੂ_ਮੰਜ਼ਿਲ_ਕਤਾਰ,
                                  TickType_t x_Ticks_ਮੰਜ਼ਿਲ_Wait,
                                  ਅਚਲ ਅਧਾਰ_Type_t xCopyPosition ) FREERTOS_SYSTEM_CALL;
ਅਧਾਰ_Type_t MPU_x_ਕਤਾਰ_Receive( ਕਤਾਰ_Handle_t x_ਕਤਾਰ,
                              ਖਾਲੀ * ਅਚਲ pv_ਬਫਰ,
                              TickType_t x_Ticks_ਮੰਜ਼ਿਲ_Wait ) FREERTOS_SYSTEM_CALL;
ਅਧਾਰ_Type_t MPU_x_ਕਤਾਰ_Peek( ਕਤਾਰ_Handle_t x_ਕਤਾਰ,
                           ਖਾਲੀ * ਅਚਲ pv_ਬਫਰ,
                           TickType_t x_Ticks_ਮੰਜ਼ਿਲ_Wait ) FREERTOS_SYSTEM_CALL;
ਅਧਾਰ_Type_t MPU_x_ਕਤਾਰ_Semaphore_Take( ਕਤਾਰ_Handle_t x_ਕਤਾਰ,
                                    TickType_t x_Ticks_ਮੰਜ਼ਿਲ_Wait ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_ਕਤਾਰ_Messages_ਉਡੀਕ( ਅਚਲ ਕਤਾਰ_Handle_t x_ਕਤਾਰ ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_ਕਤਾਰ_Spaces_ਉਪਲਬਧ( ਅਚਲ ਕਤਾਰ_Handle_t x_ਕਤਾਰ ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_x_ਕਤਾਰ_Get_Mutex_Holder( ਕਤਾਰ_Handle_t xSemaphore ) FREERTOS_SYSTEM_CALL;
ਅਧਾਰ_Type_t MPU_x_ਕਤਾਰ_Take_Mutex_Recursive( ਕਤਾਰ_Handle_t xMutex,
                                         TickType_t x_Ticks_ਮੰਜ਼ਿਲ_Wait ) FREERTOS_SYSTEM_CALL;
ਅਧਾਰ_Type_t MPU_x_ਕਤਾਰ_Give_Mutex_Recursive( ਕਤਾਰ_Handle_t pxMutex ) FREERTOS_SYSTEM_CALL;
ਖਾਲੀ MPU_v_ਕਤਾਰ_ਜੋੜੋ_ਮੰਜ਼ਿਲ_Registry( ਕਤਾਰ_Handle_t x_ਕਤਾਰ,
                              ਅਚਲ ਅੱਖਰ * pcName ) FREERTOS_SYSTEM_CALL;
ਖਾਲੀ MPU_v_ਕਤਾਰ_Unregister_ਕਤਾਰ( ਕਤਾਰ_Handle_t x_ਕਤਾਰ ) FREERTOS_SYSTEM_CALL;
ਅਚਲ ਅੱਖਰ * MPU_pc_ਕਤਾਰ_Get_Name( ਕਤਾਰ_Handle_t x_ਕਤਾਰ ) FREERTOS_SYSTEM_CALL;
ਅਧਾਰ_Type_t MPU_x_ਕਤਾਰ_ਜੋੜੋ_ਮੰਜ਼ਿਲ_ਸੈੱਟ( ਕਤਾਰ_ਸੈੱਟ_Member_Handle_t x_ਕਤਾਰ_Or_Semaphore,
                               ਕਤਾਰ_ਸੈੱਟ_Handle_t x_ਕਤਾਰ_ਸੈੱਟ ) FREERTOS_SYSTEM_CALL;
ਕਤਾਰ_ਸੈੱਟ_Member_Handle_t MPU_x_ਕਤਾਰ_Select_From_ਸੈੱਟ( ਕਤਾਰ_ਸੈੱਟ_Handle_t x_ਕਤਾਰ_ਸੈੱਟ,
                                                ਅਚਲ TickType_t x_Ticks_ਮੰਜ਼ਿਲ_Wait ) FREERTOS_SYSTEM_CALL;
ਖਾਲੀ MPU_v_ਕਤਾਰ_ਸੈੱਟ_ਕਤਾਰ_ਸੰਖਿਆ( ਕਤਾਰ_Handle_t x_ਕਤਾਰ,
                               UBaseType_t ux_ਕਤਾਰ_ਸੰਖਿਆ ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_ਕਤਾਰ_Get_ਕਤਾਰ_ਸੰਖਿਆ( ਕਤਾਰ_Handle_t x_ਕਤਾਰ ) FREERTOS_SYSTEM_CALL;
uint8_t MPU_uc_ਕਤਾਰ_Get_ਕਤਾਰ_Type( ਕਤਾਰ_Handle_t x_ਕਤਾਰ ) FREERTOS_SYSTEM_CALL;

/* Privileged only wrappers for Queue APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
ਖਾਲੀ MPU_v_ਕਤਾਰ_Delete( ਕਤਾਰ_Handle_t x_ਕਤਾਰ ) PRIVILEGED_FUNCTION;
ਕਤਾਰ_Handle_t MPU_x_ਕਤਾਰ_Create_Mutex( ਅਚਲ uint8_t uc_ਕਤਾਰ_Type ) PRIVILEGED_FUNCTION;
ਕਤਾਰ_Handle_t MPU_x_ਕਤਾਰ_Create_Mutex_Static( ਅਚਲ uint8_t uc_ਕਤਾਰ_Type,
                                           Static_ਕਤਾਰ_t * px_Static_ਕਤਾਰ ) PRIVILEGED_FUNCTION;
ਕਤਾਰ_Handle_t MPU_x_ਕਤਾਰ_Create_Counting_Semaphore( ਅਚਲ UBaseType_t ux_Max_ਗਿਣਤੀ,
                                                 ਅਚਲ UBaseType_t ux_Initial_ਗਿਣਤੀ ) PRIVILEGED_FUNCTION;
ਕਤਾਰ_Handle_t MPU_x_ਕਤਾਰ_Create_Counting_Semaphore_Static( ਅਚਲ UBaseType_t ux_Max_ਗਿਣਤੀ,
                                                       ਅਚਲ UBaseType_t ux_Initial_ਗਿਣਤੀ,
                                                       Static_ਕਤਾਰ_t * px_Static_ਕਤਾਰ ) PRIVILEGED_FUNCTION;
ਕਤਾਰ_Handle_t MPU_x_ਕਤਾਰ_Generic_Create( ਅਚਲ UBaseType_t ux_ਕਤਾਰ_ਲੰਮਾਈ,
                                       ਅਚਲ UBaseType_t ux_ਵਸਤੂ_ਮਾਪ,
                                       ਅਚਲ uint8_t uc_ਕਤਾਰ_Type ) PRIVILEGED_FUNCTION;
ਕਤਾਰ_Handle_t MPU_x_ਕਤਾਰ_Generic_Create_Static( ਅਚਲ UBaseType_t ux_ਕਤਾਰ_ਲੰਮਾਈ,
                                             ਅਚਲ UBaseType_t ux_ਵਸਤੂ_ਮਾਪ,
                                             uint8_t * puc_ਕਤਾਰ_Storage,
                                             Static_ਕਤਾਰ_t * px_Static_ਕਤਾਰ,
                                             ਅਚਲ uint8_t uc_ਕਤਾਰ_Type ) PRIVILEGED_FUNCTION;
ਕਤਾਰ_ਸੈੱਟ_Handle_t MPU_x_ਕਤਾਰ_Create_ਸੈੱਟ( ਅਚਲ UBaseType_t ux_Event_ਕਤਾਰ_ਲੰਮਾਈ ) PRIVILEGED_FUNCTION;
ਅਧਾਰ_Type_t MPU_x_ਕਤਾਰ_Remove_From_ਸੈੱਟ( ਕਤਾਰ_ਸੈੱਟ_Member_Handle_t x_ਕਤਾਰ_Or_Semaphore,
                                    ਕਤਾਰ_ਸੈੱਟ_Handle_t x_ਕਤਾਰ_ਸੈੱਟ ) PRIVILEGED_FUNCTION;
ਅਧਾਰ_Type_t MPU_x_ਕਤਾਰ_Generic_Reset( ਕਤਾਰ_Handle_t x_ਕਤਾਰ,
                                   ਅਧਾਰ_Type_t x_New_ਕਤਾਰ ) PRIVILEGED_FUNCTION;
ਅਧਾਰ_Type_t MPU_x_ਕਤਾਰ_Generic_Get_Static_Buffers( ਕਤਾਰ_Handle_t x_ਕਤਾਰ,
                                              uint8_t ** ppuc_ਕਤਾਰ_Storage,
                                              Static_ਕਤਾਰ_t ** ppx_Static_ਕਤਾਰ ) PRIVILEGED_FUNCTION;
ਅਧਾਰ_Type_t MPU_x_ਕਤਾਰ_Generic_Send_From_ISR( ਕਤਾਰ_Handle_t x_ਕਤਾਰ,
                                         ਅਚਲ ਖਾਲੀ * ਅਚਲ pv_ਵਸਤੂ_ਮੰਜ਼ਿਲ_ਕਤਾਰ,
                                         ਅਧਾਰ_Type_t * ਅਚਲ px_Higher_ਤਰਜੀਹ_Task_Woken,
                                         ਅਚਲ ਅਧਾਰ_Type_t xCopyPosition ) PRIVILEGED_FUNCTION;
ਅਧਾਰ_Type_t MPU_x_ਕਤਾਰ_Give_From_ISR( ਕਤਾਰ_Handle_t x_ਕਤਾਰ,
                                  ਅਧਾਰ_Type_t * ਅਚਲ px_Higher_ਤਰਜੀਹ_Task_Woken ) PRIVILEGED_FUNCTION;
ਅਧਾਰ_Type_t MPU_x_ਕਤਾਰ_Peek_From_ISR( ਕਤਾਰ_Handle_t x_ਕਤਾਰ,
                                  ਖਾਲੀ * ਅਚਲ pv_ਬਫਰ ) PRIVILEGED_FUNCTION;
ਅਧਾਰ_Type_t MPU_x_ਕਤਾਰ_Receive_From_ISR( ਕਤਾਰ_Handle_t x_ਕਤਾਰ,
                                     ਖਾਲੀ * ਅਚਲ pv_ਬਫਰ,
                                     ਅਧਾਰ_Type_t * ਅਚਲ px_Higher_ਤਰਜੀਹ_Task_Woken ) PRIVILEGED_FUNCTION;
ਅਧਾਰ_Type_t MPU_x_ਕਤਾਰ_Is_ਕਤਾਰ_ਖਾਲੀ_ਥਾਵਾਂ_From_ISR( ਅਚਲ ਕਤਾਰ_Handle_t x_ਕਤਾਰ ) PRIVILEGED_FUNCTION;
ਅਧਾਰ_Type_t MPU_x_ਕਤਾਰ_Is_ਕਤਾਰ_ਭਰਿਆ_From_ISR( ਅਚਲ ਕਤਾਰ_Handle_t x_ਕਤਾਰ ) PRIVILEGED_FUNCTION;
UBaseType_t MPU_ux_ਕਤਾਰ_Messages_ਉਡੀਕ_From_ISR( ਅਚਲ ਕਤਾਰ_Handle_t x_ਕਤਾਰ ) PRIVILEGED_FUNCTION;
TaskHandle_t MPU_x_ਕਤਾਰ_Get_Mutex_Holder_From_ISR( ਕਤਾਰ_Handle_t xSemaphore ) PRIVILEGED_FUNCTION;
ਕਤਾਰ_ਸੈੱਟ_Member_Handle_t MPU_x_ਕਤਾਰ_Select_From_ਸੈੱਟ_From_ISR( ਕਤਾਰ_ਸੈੱਟ_Handle_t x_ਕਤਾਰ_ਸੈੱਟ ) PRIVILEGED_FUNCTION;

/* MPU versions of timers.h API functions. */
ਖਾਲੀ * MPU_pvTimerGetTimerID( ਅਚਲ TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
ਖਾਲੀ MPU_v_Timer_ਸੈੱਟ_Timer_ID( TimerHandle_t xTimer,
                           ਖਾਲੀ * pvNewID ) FREERTOS_SYSTEM_CALL;
ਅਧਾਰ_Type_t MPU_x_Timer_Is_Timer_ਸਰਗਰਮ( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTimerGetTimerDaemonTaskHandle( ਖਾਲੀ ) FREERTOS_SYSTEM_CALL;
ਅਧਾਰ_Type_t MPU_xTimerGenericCommandFromTask( TimerHandle_t xTimer,
                                             ਅਚਲ ਅਧਾਰ_Type_t xCommandID,
                                             ਅਚਲ TickType_t x_Optional_ਮੁੱਲ,
                                             ਅਧਾਰ_Type_t * ਅਚਲ px_Higher_ਤਰਜੀਹ_Task_Woken,
                                             ਅਚਲ TickType_t x_Ticks_ਮੰਜ਼ਿਲ_Wait ) FREERTOS_SYSTEM_CALL;
ਅਧਾਰ_Type_t MPU_x_Timer_Generic_Command_From_Task_ਇੰਦਰਾਜ( ਅਚਲ xTimerGenericCommandFromTaskParams_t * pxParams ) FREERTOS_SYSTEM_CALL;
ਅਚਲ ਅੱਖਰ * MPU_pcTimerGetName( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
ਖਾਲੀ MPU_v_Timer_ਸੈੱਟ_Reload_ਬਹੁਲਕ( TimerHandle_t xTimer,
                              ਅਚਲ ਅਧਾਰ_Type_t uxAutoReload ) FREERTOS_SYSTEM_CALL;
ਅਧਾਰ_Type_t MPU_x_Timer_Get_Reload_ਬਹੁਲਕ( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Timer_Get_Reload_ਬਹੁਲਕ( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_xTimerGetPeriod( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_x_Timer_Get_Expiry_ਸਮਾਂ( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;

/* Privileged only wrappers for Timer APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
TimerHandle_t MPU_xTimerCreate( ਅਚਲ ਅੱਖਰ * ਅਚਲ pcTimerName,
                                ਅਚਲ TickType_t x_Timer_Period_ਵਿੱਚ_Ticks,
                                ਅਚਲ UBaseType_t uxAutoReload,
                                ਖਾਲੀ * ਅਚਲ pvTimerID,
                                TimerCallbackFunction_t pxCallbackFunction ) PRIVILEGED_FUNCTION;
TimerHandle_t MPU_xTimerCreateStatic( ਅਚਲ ਅੱਖਰ * ਅਚਲ pcTimerName,
                                      ਅਚਲ TickType_t x_Timer_Period_ਵਿੱਚ_Ticks,
                                      ਅਚਲ UBaseType_t uxAutoReload,
                                      ਖਾਲੀ * ਅਚਲ pvTimerID,
                                      TimerCallbackFunction_t pxCallbackFunction,
                                      StaticTimer_t * px_Timer_ਬਫਰ ) PRIVILEGED_FUNCTION;
ਅਧਾਰ_Type_t MPU_x_Timer_Get_Static_ਬਫਰ( TimerHandle_t xTimer,
                                      StaticTimer_t ** ppx_Timer_ਬਫਰ ) PRIVILEGED_FUNCTION;
ਅਧਾਰ_Type_t MPU_xTimerGenericCommandFromISR( TimerHandle_t xTimer,
                                            ਅਚਲ ਅਧਾਰ_Type_t xCommandID,
                                            ਅਚਲ TickType_t x_Optional_ਮੁੱਲ,
                                            ਅਧਾਰ_Type_t * ਅਚਲ px_Higher_ਤਰਜੀਹ_Task_Woken,
                                            ਅਚਲ TickType_t x_Ticks_ਮੰਜ਼ਿਲ_Wait ) PRIVILEGED_FUNCTION;

/* MPU versions of event_group.h API functions. */
Event_ਬਿੱਟ_t MPU_x_Event_ਸਮੂਹ_Wait_ਬਿੱਟ( Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ,
                                     ਅਚਲ Event_ਬਿੱਟ_t ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For,
                                     ਅਚਲ ਅਧਾਰ_Type_t xClearOnExit,
                                     ਅਚਲ ਅਧਾਰ_Type_t x_Wait_For_All_ਬਿੱਟ,
                                     TickType_t x_Ticks_ਮੰਜ਼ਿਲ_Wait ) FREERTOS_SYSTEM_CALL;
Event_ਬਿੱਟ_t MPU_x_Event_ਸਮੂਹ_Wait_ਬਿੱਟ_ਇੰਦਰਾਜ( ਅਚਲ x_Event_ਸਮੂਹ_Wait_ਬਿੱਟ_Params_t * pxParams ) FREERTOS_SYSTEM_CALL;
Event_ਬਿੱਟ_t MPU_x_Event_ਸਮੂਹ_Clear_ਬਿੱਟ( Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ,
                                      ਅਚਲ Event_ਬਿੱਟ_t ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear ) FREERTOS_SYSTEM_CALL;
Event_ਬਿੱਟ_t MPU_x_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ( Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ,
                                    ਅਚਲ Event_ਬਿੱਟ_t ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ ) FREERTOS_SYSTEM_CALL;
Event_ਬਿੱਟ_t MPU_x_Event_ਸਮੂਹ_Sync( Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ,
                                 ਅਚਲ Event_ਬਿੱਟ_t ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ,
                                 ਅਚਲ Event_ਬਿੱਟ_t ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Wait_For,
                                 TickType_t x_Ticks_ਮੰਜ਼ਿਲ_Wait ) FREERTOS_SYSTEM_CALL;
#ਜੇ ( configUSE_TRACE_FACILITY == 1 )
    UBaseType_t MPU_ux_Event_ਸਮੂਹ_Get_ਸੰਖਿਆ( ਖਾਲੀ * x_Event_ਸਮੂਹ ) FREERTOS_SYSTEM_CALL;
    ਖਾਲੀ MPU_v_Event_ਸਮੂਹ_ਸੈੱਟ_ਸੰਖਿਆ( ਖਾਲੀ * x_Event_ਸਮੂਹ,
                                   UBaseType_t ux_Event_ਸਮੂਹ_ਸੰਖਿਆ ) FREERTOS_SYSTEM_CALL;
#ਜੇ_ਅੰਤ /* ( configUSE_TRACE_FACILITY == 1 )*/

/* Privileged only wrappers for Event Group APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
Event_ਸਮੂਹ_Handle_t MPU_x_Event_ਸਮੂਹ_Create( ਖਾਲੀ ) PRIVILEGED_FUNCTION;
Event_ਸਮੂਹ_Handle_t MPU_x_Event_ਸਮੂਹ_Create_Static( Static_Event_ਸਮੂਹ_t * px_Event_ਸਮੂਹ_ਬਫਰ ) PRIVILEGED_FUNCTION;
ਖਾਲੀ MPU_v_Event_ਸਮੂਹ_Delete( Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ ) PRIVILEGED_FUNCTION;
ਅਧਾਰ_Type_t MPU_x_Event_ਸਮੂਹ_Get_Static_ਬਫਰ( Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ,
                                           Static_Event_ਸਮੂਹ_t ** ppx_Event_ਸਮੂਹ_ਬਫਰ ) PRIVILEGED_FUNCTION;
ਅਧਾਰ_Type_t MPU_x_Event_ਸਮੂਹ_Clear_ਬਿੱਟ_From_ISR( Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ,
                                            ਅਚਲ Event_ਬਿੱਟ_t ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_Clear ) PRIVILEGED_FUNCTION;
ਅਧਾਰ_Type_t MPU_x_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ_From_ISR( Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ,
                                          ਅਚਲ Event_ਬਿੱਟ_t ux_ਬਿੱਟ_ਮੰਜ਼ਿਲ_ਸੈੱਟ,
                                          ਅਧਾਰ_Type_t * px_Higher_ਤਰਜੀਹ_Task_Woken ) PRIVILEGED_FUNCTION;
Event_ਬਿੱਟ_t MPU_x_Event_ਸਮੂਹ_Get_ਬਿੱਟ_From_ISR( Event_ਸਮੂਹ_Handle_t x_Event_ਸਮੂਹ ) PRIVILEGED_FUNCTION;

/* MPU versions of message/stream_buffer.h API functions. */
size_t MPU_x_ਧਾਰਾ_ਬਫਰ_Send( ਧਾਰਾ_ਬਫਰ_Handle_t x_ਧਾਰਾ_ਬਫਰ,
                              ਅਚਲ ਖਾਲੀ * pv_Tx_ਅੰਕੜੇ,
                              size_t x_ਅੰਕੜੇ_ਲੰਮਾਈ_ਬਾਈਟਾਂ,
                              TickType_t x_Ticks_ਮੰਜ਼ਿਲ_Wait ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_ਧਾਰਾ_ਬਫਰ_Receive( ਧਾਰਾ_ਬਫਰ_Handle_t x_ਧਾਰਾ_ਬਫਰ,
                                 ਖਾਲੀ * pv_Rx_ਅੰਕੜੇ,
                                 size_t x_ਬਫਰ_ਲੰਮਾਈ_ਬਾਈਟਾਂ,
                                 TickType_t x_Ticks_ਮੰਜ਼ਿਲ_Wait ) FREERTOS_SYSTEM_CALL;
ਅਧਾਰ_Type_t MPU_x_ਧਾਰਾ_ਬਫਰ_Is_ਭਰਿਆ( ਧਾਰਾ_ਬਫਰ_Handle_t x_ਧਾਰਾ_ਬਫਰ ) FREERTOS_SYSTEM_CALL;
ਅਧਾਰ_Type_t MPU_x_ਧਾਰਾ_ਬਫਰ_Is_ਖਾਲੀ_ਥਾਵਾਂ( ਧਾਰਾ_ਬਫਰ_Handle_t x_ਧਾਰਾ_ਬਫਰ ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_ਧਾਰਾ_ਬਫਰ_Spaces_ਉਪਲਬਧ( ਧਾਰਾ_ਬਫਰ_Handle_t x_ਧਾਰਾ_ਬਫਰ ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_ਧਾਰਾ_ਬਫਰ_ਬਾਈਟਾਂ_ਉਪਲਬਧ( ਧਾਰਾ_ਬਫਰ_Handle_t x_ਧਾਰਾ_ਬਫਰ ) FREERTOS_SYSTEM_CALL;
ਅਧਾਰ_Type_t MPU_x_ਧਾਰਾ_ਬਫਰ_ਸੈੱਟ_Trigger_Level( ਧਾਰਾ_ਬਫਰ_Handle_t x_ਧਾਰਾ_ਬਫਰ,
                                             size_t xTriggerLevel ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_ਧਾਰਾ_ਬਫਰ_ਅਗਲਾ_ਸੁਨੇਹਾ_ਲੰਮਾਈ_ਬਾਈਟਾਂ( ਧਾਰਾ_ਬਫਰ_Handle_t x_ਧਾਰਾ_ਬਫਰ ) FREERTOS_SYSTEM_CALL;

/* Privileged only wrappers for Stream Buffer APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
ਧਾਰਾ_ਬਫਰ_Handle_t MPU_x_ਧਾਰਾ_ਬਫਰ_Generic_Create( size_t x_ਬਫਰ_ਮਾਪ_ਬਾਈਟਾਂ,
                                                     size_t x_Trigger_Level_ਬਾਈਟਾਂ,
                                                     ਅਧਾਰ_Type_t x_ਧਾਰਾ_ਬਫਰ_Type,
                                                     ਧਾਰਾ_ਬਫਰ_Callback_Function_t pxSendCompletedCallback,
                                                     ਧਾਰਾ_ਬਫਰ_Callback_Function_t pxReceiveCompletedCallback ) PRIVILEGED_FUNCTION;
ਧਾਰਾ_ਬਫਰ_Handle_t MPU_x_ਧਾਰਾ_ਬਫਰ_Generic_Create_Static( size_t x_ਬਫਰ_ਮਾਪ_ਬਾਈਟਾਂ,
                                                           size_t x_Trigger_Level_ਬਾਈਟਾਂ,
                                                           ਅਧਾਰ_Type_t x_ਧਾਰਾ_ਬਫਰ_Type,
                                                           uint8_t * ਅਚਲ puc_ਧਾਰਾ_ਬਫਰ_Storage_Area,
                                                           Static_ਧਾਰਾ_ਬਫਰ_t * ਅਚਲ px_Static_ਧਾਰਾ_ਬਫਰ,
                                                           ਧਾਰਾ_ਬਫਰ_Callback_Function_t pxSendCompletedCallback,
                                                           ਧਾਰਾ_ਬਫਰ_Callback_Function_t pxReceiveCompletedCallback ) PRIVILEGED_FUNCTION;
ਖਾਲੀ MPU_v_ਧਾਰਾ_ਬਫਰ_Delete( ਧਾਰਾ_ਬਫਰ_Handle_t x_ਧਾਰਾ_ਬਫਰ ) PRIVILEGED_FUNCTION;
ਅਧਾਰ_Type_t MPU_x_ਧਾਰਾ_ਬਫਰ_Reset( ਧਾਰਾ_ਬਫਰ_Handle_t x_ਧਾਰਾ_ਬਫਰ ) PRIVILEGED_FUNCTION;
ਅਧਾਰ_Type_t MPU_x_ਧਾਰਾ_ਬਫਰ_Get_Static_Buffers( ਧਾਰਾ_ਬਫਰ_Handle_t x_ਧਾਰਾ_Buffers,
                                              uint8_t * ppuc_ਧਾਰਾ_ਬਫਰ_Storage_Area,
                                              Static_ਧਾਰਾ_ਬਫਰ_t * ppx_Static_ਧਾਰਾ_ਬਫਰ ) PRIVILEGED_FUNCTION;
size_t MPU_x_ਧਾਰਾ_ਬਫਰ_Send_From_ISR( ਧਾਰਾ_ਬਫਰ_Handle_t x_ਧਾਰਾ_ਬਫਰ,
                                     ਅਚਲ ਖਾਲੀ * pv_Tx_ਅੰਕੜੇ,
                                     size_t x_ਅੰਕੜੇ_ਲੰਮਾਈ_ਬਾਈਟਾਂ,
                                     ਅਧਾਰ_Type_t * ਅਚਲ px_Higher_ਤਰਜੀਹ_Task_Woken ) PRIVILEGED_FUNCTION;
size_t MPU_x_ਧਾਰਾ_ਬਫਰ_Receive_From_ISR( ਧਾਰਾ_ਬਫਰ_Handle_t x_ਧਾਰਾ_ਬਫਰ,
                                        ਖਾਲੀ * pv_Rx_ਅੰਕੜੇ,
                                        size_t x_ਬਫਰ_ਲੰਮਾਈ_ਬਾਈਟਾਂ,
                                        ਅਧਾਰ_Type_t * ਅਚਲ px_Higher_ਤਰਜੀਹ_Task_Woken ) PRIVILEGED_FUNCTION;
ਅਧਾਰ_Type_t MPU_x_ਧਾਰਾ_ਬਫਰ_Send_Completed_From_ISR( ਧਾਰਾ_ਬਫਰ_Handle_t x_ਧਾਰਾ_ਬਫਰ,
                                                  ਅਧਾਰ_Type_t * px_Higher_ਤਰਜੀਹ_Task_Woken ) PRIVILEGED_FUNCTION;
ਅਧਾਰ_Type_t MPU_x_ਧਾਰਾ_ਬਫਰ_Receive_Completed_From_ISR( ਧਾਰਾ_ਬਫਰ_Handle_t x_ਧਾਰਾ_ਬਫਰ,
                                                     ਅਧਾਰ_Type_t * px_Higher_ਤਰਜੀਹ_Task_Woken ) PRIVILEGED_FUNCTION;
ਅਧਾਰ_Type_t MPU_x_ਧਾਰਾ_ਬਫਰ_Reset_From_ISR( ਧਾਰਾ_ਬਫਰ_Handle_t x_ਧਾਰਾ_ਬਫਰ ) PRIVILEGED_FUNCTION;

#ਜੇ_ਅੰਤ /* MPU_PROTOTYPES_H */
