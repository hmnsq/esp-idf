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


#यदि_अपरिभाषितम् MPU_PROTOTYPES_H
#परिभाषय MPU_PROTOTYPES_H

प्रकारनिर्देश संरचना xTaskGenericNotifyParams
{
    TaskHandle_t x_Task_गन्तव्यम्_Notify;
    UBaseType_t ux_सूचकांक_गन्तव्यम्_Notify;
    uint32_t ul_मानम्;
    eNotifyAction eAction;
    uint32_t * pul_Previous_Notification_मानम्;
} xTaskGenericNotifyParams_t;

प्रकारनिर्देश संरचना xTaskGenericNotifyWaitParams
{
    UBaseType_t ux_सूचकांक_गन्तव्यम्_Wait_On;
    uint32_t ul_द्वयङ्काः_गन्तव्यम्_Clear_On_प्रविष्टिः;
    uint32_t ul_द्वयङ्काः_गन्तव्यम्_Clear_On_Exit;
    uint32_t * pul_Notification_मानम्;
    TickType_t x_Ticks_गन्तव्यम्_Wait;
} xTaskGenericNotifyWaitParams_t;

प्रकारनिर्देश संरचना xTimerGenericCommandFromTaskParams
{
    TimerHandle_t xTimer;
    आधारः_Type_t xCommandID;
    TickType_t x_Optional_मानम्;
    आधारः_Type_t * px_Higher_प्राथमिकता_Task_Woken;
    TickType_t x_Ticks_गन्तव्यम्_Wait;
} xTimerGenericCommandFromTaskParams_t;

प्रकारनिर्देश संरचना x_Event_गणः_Wait_द्वयङ्काः_Params
{
    Event_गणः_Handle_t x_Event_गणः;
    Event_द्वयङ्काः_t ux_द्वयङ्काः_गन्तव्यम्_Wait_For;
    आधारः_Type_t xClearOnExit;
    आधारः_Type_t x_Wait_For_All_द्वयङ्काः;
    TickType_t x_Ticks_गन्तव्यम्_Wait;
} x_Event_गणः_Wait_द्वयङ्काः_Params_t;

/* MPU versions of task.h API functions. */
शून्य MPU_vTaskDelay( ध्रुव TickType_t x_Ticks_गन्तव्यम्_Delay ) FREERTOS_SYSTEM_CALL;
आधारः_Type_t MPU_xTaskDelayUntil( TickType_t * ध्रुव pxPreviousWakeTime,
                                ध्रुव TickType_t xTimeIncrement ) FREERTOS_SYSTEM_CALL;
आधारः_Type_t MPU_xTaskAbortDelay( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_प्राथमिकता_Get( ध्रुव TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
eTaskState MPU_eTaskGetState( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
शून्य MPU_vTaskGetInfo( TaskHandle_t xTask,
                       Task_स्थितिः_t * px_Task_स्थितिः,
                       आधारः_Type_t x_Get_Free_राशिः_Space,
                       eTaskState eState ) FREERTOS_SYSTEM_CALL;
शून्य MPU_vTaskSuspend( TaskHandle_t x_Task_गन्तव्यम्_Suspend ) FREERTOS_SYSTEM_CALL;
शून्य MPU_vTaskResume( TaskHandle_t x_Task_गन्तव्यम्_Resume ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_x_Task_Get_Tick_गणनम्( शून्य ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_Get_सङ्ख्या_सम्बन्धि_Tasks( शून्य ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_Get_राशिः_उच्चः_Water_Mark( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
config_राशिः_गभीरता_TYPE MPU_ux_Task_Get_राशिः_उच्चः_Water_Mark2( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
शून्य MPU_v_Task_समुच्चयः_Application_Task_चिह्नकम्( TaskHandle_t xTask,
                                     TaskHookFunction_t pxHookFunction ) FREERTOS_SYSTEM_CALL;
TaskHookFunction_t MPU_x_Task_Get_Application_Task_चिह्नकम्( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
शून्य MPU_v_Task_समुच्चयः_Thread_Local_Storage_Pointer( TaskHandle_t x_Task_गन्तव्यम्_समुच्चयः,
                                            आधारः_Type_t x_सूचकांक,
                                            शून्य * pv_मानम् ) FREERTOS_SYSTEM_CALL;
शून्य * MPU_pvTaskGetThreadLocalStoragePointer( TaskHandle_t x_Task_गन्तव्यम्_Query,
                                               आधारः_Type_t x_सूचकांक ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetIdleTaskHandle( शून्य ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxTaskGetSystemState( Task_स्थितिः_t * ध्रुव px_Task_स्थितिः_Array,
                                      ध्रुव UBaseType_t ux_Array_आकार,
                                      config_सततम्_TIME_गणक_TYPE * ध्रुव pul_योग_सततम्_Time ) FREERTOS_SYSTEM_CALL;
config_सततम्_TIME_गणक_TYPE MPU_ul_Task_Get_सततम्_Time_गणक( ध्रुव TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
config_सततम्_TIME_गणक_TYPE MPU_ul_Task_Get_सततम्_Time_Percent( ध्रुव TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
config_सततम्_TIME_गणक_TYPE MPU_ul_Task_Get_Idle_सततम्_Time_गणक( शून्य ) FREERTOS_SYSTEM_CALL;
config_सततम्_TIME_गणक_TYPE MPU_ul_Task_Get_Idle_सततम्_Time_Percent( शून्य ) FREERTOS_SYSTEM_CALL;
आधारः_Type_t MPU_xTaskGenericNotify( TaskHandle_t x_Task_गन्तव्यम्_Notify,
                                   UBaseType_t ux_सूचकांक_गन्तव्यम्_Notify,
                                   uint32_t ul_मानम्,
                                   eNotifyAction eAction,
                                   uint32_t * pul_Previous_Notification_मानम् ) FREERTOS_SYSTEM_CALL;
आधारः_Type_t MPU_x_Task_Generic_Notify_प्रविष्टिः( ध्रुव xTaskGenericNotifyParams_t * pxParams ) FREERTOS_SYSTEM_CALL;
आधारः_Type_t MPU_xTaskGenericNotifyWait( UBaseType_t ux_सूचकांक_गन्तव्यम्_Wait_On,
                                       uint32_t ul_द्वयङ्काः_गन्तव्यम्_Clear_On_प्रविष्टिः,
                                       uint32_t ul_द्वयङ्काः_गन्तव्यम्_Clear_On_Exit,
                                       uint32_t * pul_Notification_मानम्,
                                       TickType_t x_Ticks_गन्तव्यम्_Wait ) FREERTOS_SYSTEM_CALL;
आधारः_Type_t MPU_x_Task_Generic_Notify_Wait_प्रविष्टिः( ध्रुव xTaskGenericNotifyWaitParams_t * pxParams ) FREERTOS_SYSTEM_CALL;
uint32_t MPU_ulTaskGenericNotifyTake( UBaseType_t ux_सूचकांक_गन्तव्यम्_Wait_On,
                                      आधारः_Type_t x_Clear_गणनम्_On_Exit,
                                      TickType_t x_Ticks_गन्तव्यम्_Wait ) FREERTOS_SYSTEM_CALL;
आधारः_Type_t MPU_xTaskGenericNotifyStateClear( TaskHandle_t xTask,
                                             UBaseType_t ux_सूचकांक_गन्तव्यम्_Clear ) FREERTOS_SYSTEM_CALL;
uint32_t MPU_ul_Task_Generic_Notify_मानम्_Clear( TaskHandle_t xTask,
                                            UBaseType_t ux_सूचकांक_गन्तव्यम्_Clear,
                                            uint32_t ul_द्वयङ्काः_गन्तव्यम्_Clear ) FREERTOS_SYSTEM_CALL;
शून्य MPU_v_Task_समुच्चयः_Time_बहिर्गमनम्_State( Time_बहिर्गमनम्_t * ध्रुव px_Time_बहिर्गमनम् ) FREERTOS_SYSTEM_CALL;
आधारः_Type_t MPU_x_Task_Check_For_Time_बहिर्गमनम्( Time_बहिर्गमनम्_t * ध्रुव px_Time_बहिर्गमनम्,
                                     TickType_t * ध्रुव px_Ticks_गन्तव्यम्_Wait ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetCurrentTaskHandle( शून्य ) FREERTOS_SYSTEM_CALL;
आधारः_Type_t MPU_xTaskGetSchedulerState( शून्य ) FREERTOS_SYSTEM_CALL;

/* Privileged only wrappers for Task APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
आधारः_Type_t MPU_xTaskCreate( TaskFunction_t px_Task_संहिता,
                            ध्रुव वर्ण * ध्रुव pcName,
                            ध्रुव config_राशिः_गभीरता_TYPE ux_राशिः_गभीरता,
                            शून्य * ध्रुव pvParameters,
                            UBaseType_t ux_प्राथमिकता,
                            TaskHandle_t * ध्रुव pxCreatedTask ) PRIVILEGED_FUNCTION;
TaskHandle_t MPU_xTaskCreateStatic( TaskFunction_t px_Task_संहिता,
                                    ध्रुव वर्ण * ध्रुव pcName,
                                    ध्रुव config_राशिः_गभीरता_TYPE ux_राशिः_गभीरता,
                                    शून्य * ध्रुव pvParameters,
                                    UBaseType_t ux_प्राथमिकता,
                                    राशिः_Type_t * ध्रुव pux_राशिः_बफर,
                                    StaticTask_t * ध्रुव px_Task_बफर ) PRIVILEGED_FUNCTION;
शून्य MPU_vTaskDelete( TaskHandle_t x_Task_गन्तव्यम्_Delete ) PRIVILEGED_FUNCTION;
शून्य MPU_v_Task_प्राथमिकता_समुच्चयः( TaskHandle_t xTask,
                           UBaseType_t ux_New_प्राथमिकता ) PRIVILEGED_FUNCTION;
TaskHandle_t MPU_xTaskGetHandle( ध्रुव वर्ण * pc_Name_गन्तव्यम्_Query ) PRIVILEGED_FUNCTION;
आधारः_Type_t MPU_xTaskCallApplicationTaskHook( TaskHandle_t xTask,
                                             शून्य * pvParameter ) PRIVILEGED_FUNCTION;
वर्ण * MPU_pcTaskGetName( TaskHandle_t x_Task_गन्तव्यम्_Query ) PRIVILEGED_FUNCTION;
आधारः_Type_t MPU_xTaskCreateRestricted( ध्रुव TaskParameters_t * ध्रुव pxTaskDefinition,
                                      TaskHandle_t * pxCreatedTask ) PRIVILEGED_FUNCTION;
आधारः_Type_t MPU_xTaskCreateRestrictedStatic( ध्रुव TaskParameters_t * ध्रुव pxTaskDefinition,
                                            TaskHandle_t * pxCreatedTask ) PRIVILEGED_FUNCTION;
शून्य MPU_vTaskAllocateMPURegions( TaskHandle_t x_Task_गन्तव्यम्_Modify,
                                  ध्रुव स्मृतिः_Region_t * ध्रुव xRegions ) PRIVILEGED_FUNCTION;
आधारः_Type_t MPU_xTaskGetStaticBuffers( TaskHandle_t xTask,
                                      राशिः_Type_t ** ppux_राशिः_बफर,
                                      StaticTask_t ** ppx_Task_बफर ) PRIVILEGED_FUNCTION;
UBaseType_t MPU_ux_Task_प्राथमिकता_Get_From_ISR( ध्रुव TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
UBaseType_t MPU_ux_Task_आधारः_प्राथमिकता_Get( ध्रुव TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
UBaseType_t MPU_ux_Task_आधारः_प्राथमिकता_Get_From_ISR( ध्रुव TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
आधारः_Type_t MPU_xTaskResumeFromISR( TaskHandle_t x_Task_गन्तव्यम्_Resume ) PRIVILEGED_FUNCTION;
TaskHookFunction_t MPU_x_Task_Get_Application_Task_चिह्नकम्_From_ISR( TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
आधारः_Type_t MPU_xTaskGenericNotifyFromISR( TaskHandle_t x_Task_गन्तव्यम्_Notify,
                                          UBaseType_t ux_सूचकांक_गन्तव्यम्_Notify,
                                          uint32_t ul_मानम्,
                                          eNotifyAction eAction,
                                          uint32_t * pul_Previous_Notification_मानम्,
                                          आधारः_Type_t * px_Higher_प्राथमिकता_Task_Woken ) PRIVILEGED_FUNCTION;
शून्य MPU_vTaskGenericNotifyGiveFromISR( TaskHandle_t x_Task_गन्तव्यम्_Notify,
                                        UBaseType_t ux_सूचकांक_गन्तव्यम्_Notify,
                                        आधारः_Type_t * px_Higher_प्राथमिकता_Task_Woken ) PRIVILEGED_FUNCTION;

/* MPU versions of queue.h API functions. */
आधारः_Type_t MPU_xQueueGenericSend( QueueHandle_t xQueue,
                                  ध्रुव शून्य * ध्रुव pv_Item_गन्तव्यम्_Queue,
                                  TickType_t x_Ticks_गन्तव्यम्_Wait,
                                  ध्रुव आधारः_Type_t xCopyPosition ) FREERTOS_SYSTEM_CALL;
आधारः_Type_t MPU_xQueueReceive( QueueHandle_t xQueue,
                              शून्य * ध्रुव pv_बफर,
                              TickType_t x_Ticks_गन्तव्यम्_Wait ) FREERTOS_SYSTEM_CALL;
आधारः_Type_t MPU_xQueuePeek( QueueHandle_t xQueue,
                           शून्य * ध्रुव pv_बफर,
                           TickType_t x_Ticks_गन्तव्यम्_Wait ) FREERTOS_SYSTEM_CALL;
आधारः_Type_t MPU_xQueueSemaphoreTake( QueueHandle_t xQueue,
                                    TickType_t x_Ticks_गन्तव्यम्_Wait ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Queue_Messages_प्रतीक्षा( ध्रुव QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Queue_Spaces_उपलब्धम्( ध्रुव QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xQueueGetMutexHolder( QueueHandle_t xSemaphore ) FREERTOS_SYSTEM_CALL;
आधारः_Type_t MPU_xQueueTakeMutexRecursive( QueueHandle_t xMutex,
                                         TickType_t x_Ticks_गन्तव्यम्_Wait ) FREERTOS_SYSTEM_CALL;
आधारः_Type_t MPU_xQueueGiveMutexRecursive( QueueHandle_t pxMutex ) FREERTOS_SYSTEM_CALL;
शून्य MPU_v_Queue_सङ्कलय_गन्तव्यम्_Registry( QueueHandle_t xQueue,
                              ध्रुव वर्ण * pcName ) FREERTOS_SYSTEM_CALL;
शून्य MPU_vQueueUnregisterQueue( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
ध्रुव वर्ण * MPU_pcQueueGetName( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
आधारः_Type_t MPU_x_Queue_सङ्कलय_गन्तव्यम्_समुच्चयः( Queue_समुच्चयः_Member_Handle_t xQueueOrSemaphore,
                               Queue_समुच्चयः_Handle_t x_Queue_समुच्चयः ) FREERTOS_SYSTEM_CALL;
Queue_समुच्चयः_Member_Handle_t MPU_x_Queue_Select_From_समुच्चयः( Queue_समुच्चयः_Handle_t x_Queue_समुच्चयः,
                                                ध्रुव TickType_t x_Ticks_गन्तव्यम्_Wait ) FREERTOS_SYSTEM_CALL;
शून्य MPU_v_Queue_समुच्चयः_Queue_सङ्ख्या( QueueHandle_t xQueue,
                               UBaseType_t ux_Queue_सङ्ख्या ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Queue_Get_Queue_सङ्ख्या( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
uint8_t MPU_ucQueueGetQueueType( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;

/* Privileged only wrappers for Queue APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
शून्य MPU_vQueueDelete( QueueHandle_t xQueue ) PRIVILEGED_FUNCTION;
QueueHandle_t MPU_xQueueCreateMutex( ध्रुव uint8_t ucQueueType ) PRIVILEGED_FUNCTION;
QueueHandle_t MPU_xQueueCreateMutexStatic( ध्रुव uint8_t ucQueueType,
                                           StaticQueue_t * pxStaticQueue ) PRIVILEGED_FUNCTION;
QueueHandle_t MPU_xQueueCreateCountingSemaphore( ध्रुव UBaseType_t ux_Max_गणनम्,
                                                 ध्रुव UBaseType_t ux_Initial_गणनम् ) PRIVILEGED_FUNCTION;
QueueHandle_t MPU_xQueueCreateCountingSemaphoreStatic( ध्रुव UBaseType_t ux_Max_गणनम्,
                                                       ध्रुव UBaseType_t ux_Initial_गणनम्,
                                                       StaticQueue_t * pxStaticQueue ) PRIVILEGED_FUNCTION;
QueueHandle_t MPU_xQueueGenericCreate( ध्रुव UBaseType_t ux_Queue_दीर्घता,
                                       ध्रुव UBaseType_t ux_Item_आकार,
                                       ध्रुव uint8_t ucQueueType ) PRIVILEGED_FUNCTION;
QueueHandle_t MPU_xQueueGenericCreateStatic( ध्रुव UBaseType_t ux_Queue_दीर्घता,
                                             ध्रुव UBaseType_t ux_Item_आकार,
                                             uint8_t * pucQueueStorage,
                                             StaticQueue_t * pxStaticQueue,
                                             ध्रुव uint8_t ucQueueType ) PRIVILEGED_FUNCTION;
Queue_समुच्चयः_Handle_t MPU_x_Queue_Create_समुच्चयः( ध्रुव UBaseType_t ux_Event_Queue_दीर्घता ) PRIVILEGED_FUNCTION;
आधारः_Type_t MPU_x_Queue_Remove_From_समुच्चयः( Queue_समुच्चयः_Member_Handle_t xQueueOrSemaphore,
                                    Queue_समुच्चयः_Handle_t x_Queue_समुच्चयः ) PRIVILEGED_FUNCTION;
आधारः_Type_t MPU_xQueueGenericReset( QueueHandle_t xQueue,
                                   आधारः_Type_t xNewQueue ) PRIVILEGED_FUNCTION;
आधारः_Type_t MPU_xQueueGenericGetStaticBuffers( QueueHandle_t xQueue,
                                              uint8_t ** ppucQueueStorage,
                                              StaticQueue_t ** ppxStaticQueue ) PRIVILEGED_FUNCTION;
आधारः_Type_t MPU_xQueueGenericSendFromISR( QueueHandle_t xQueue,
                                         ध्रुव शून्य * ध्रुव pv_Item_गन्तव्यम्_Queue,
                                         आधारः_Type_t * ध्रुव px_Higher_प्राथमिकता_Task_Woken,
                                         ध्रुव आधारः_Type_t xCopyPosition ) PRIVILEGED_FUNCTION;
आधारः_Type_t MPU_xQueueGiveFromISR( QueueHandle_t xQueue,
                                  आधारः_Type_t * ध्रुव px_Higher_प्राथमिकता_Task_Woken ) PRIVILEGED_FUNCTION;
आधारः_Type_t MPU_xQueuePeekFromISR( QueueHandle_t xQueue,
                                  शून्य * ध्रुव pv_बफर ) PRIVILEGED_FUNCTION;
आधारः_Type_t MPU_xQueueReceiveFromISR( QueueHandle_t xQueue,
                                     शून्य * ध्रुव pv_बफर,
                                     आधारः_Type_t * ध्रुव px_Higher_प्राथमिकता_Task_Woken ) PRIVILEGED_FUNCTION;
आधारः_Type_t MPU_x_Queue_Is_Queue_रिक्तानि_From_ISR( ध्रुव QueueHandle_t xQueue ) PRIVILEGED_FUNCTION;
आधारः_Type_t MPU_x_Queue_Is_Queue_पूर्णम्_From_ISR( ध्रुव QueueHandle_t xQueue ) PRIVILEGED_FUNCTION;
UBaseType_t MPU_ux_Queue_Messages_प्रतीक्षा_From_ISR( ध्रुव QueueHandle_t xQueue ) PRIVILEGED_FUNCTION;
TaskHandle_t MPU_xQueueGetMutexHolderFromISR( QueueHandle_t xSemaphore ) PRIVILEGED_FUNCTION;
Queue_समुच्चयः_Member_Handle_t MPU_x_Queue_Select_From_समुच्चयः_From_ISR( Queue_समुच्चयः_Handle_t x_Queue_समुच्चयः ) PRIVILEGED_FUNCTION;

/* MPU versions of timers.h API functions. */
शून्य * MPU_pvTimerGetTimerID( ध्रुव TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
शून्य MPU_v_Timer_समुच्चयः_Timer_ID( TimerHandle_t xTimer,
                           शून्य * pvNewID ) FREERTOS_SYSTEM_CALL;
आधारः_Type_t MPU_x_Timer_Is_Timer_सक्रिय( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTimerGetTimerDaemonTaskHandle( शून्य ) FREERTOS_SYSTEM_CALL;
आधारः_Type_t MPU_xTimerGenericCommandFromTask( TimerHandle_t xTimer,
                                             ध्रुव आधारः_Type_t xCommandID,
                                             ध्रुव TickType_t x_Optional_मानम्,
                                             आधारः_Type_t * ध्रुव px_Higher_प्राथमिकता_Task_Woken,
                                             ध्रुव TickType_t x_Ticks_गन्तव्यम्_Wait ) FREERTOS_SYSTEM_CALL;
आधारः_Type_t MPU_x_Timer_Generic_Command_From_Task_प्रविष्टिः( ध्रुव xTimerGenericCommandFromTaskParams_t * pxParams ) FREERTOS_SYSTEM_CALL;
ध्रुव वर्ण * MPU_pcTimerGetName( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
शून्य MPU_v_Timer_समुच्चयः_Reload_बहुलकम्( TimerHandle_t xTimer,
                              ध्रुव आधारः_Type_t uxAutoReload ) FREERTOS_SYSTEM_CALL;
आधारः_Type_t MPU_x_Timer_Get_Reload_बहुलकम्( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Timer_Get_Reload_बहुलकम्( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_xTimerGetPeriod( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_xTimerGetExpiryTime( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;

/* Privileged only wrappers for Timer APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
TimerHandle_t MPU_xTimerCreate( ध्रुव वर्ण * ध्रुव pcTimerName,
                                ध्रुव TickType_t x_Timer_Period_अन्तर्_Ticks,
                                ध्रुव UBaseType_t uxAutoReload,
                                शून्य * ध्रुव pvTimerID,
                                TimerCallbackFunction_t pxCallbackFunction ) PRIVILEGED_FUNCTION;
TimerHandle_t MPU_xTimerCreateStatic( ध्रुव वर्ण * ध्रुव pcTimerName,
                                      ध्रुव TickType_t x_Timer_Period_अन्तर्_Ticks,
                                      ध्रुव UBaseType_t uxAutoReload,
                                      शून्य * ध्रुव pvTimerID,
                                      TimerCallbackFunction_t pxCallbackFunction,
                                      StaticTimer_t * px_Timer_बफर ) PRIVILEGED_FUNCTION;
आधारः_Type_t MPU_x_Timer_Get_Static_बफर( TimerHandle_t xTimer,
                                      StaticTimer_t ** ppx_Timer_बफर ) PRIVILEGED_FUNCTION;
आधारः_Type_t MPU_xTimerGenericCommandFromISR( TimerHandle_t xTimer,
                                            ध्रुव आधारः_Type_t xCommandID,
                                            ध्रुव TickType_t x_Optional_मानम्,
                                            आधारः_Type_t * ध्रुव px_Higher_प्राथमिकता_Task_Woken,
                                            ध्रुव TickType_t x_Ticks_गन्तव्यम्_Wait ) PRIVILEGED_FUNCTION;

/* MPU versions of event_group.h API functions. */
Event_द्वयङ्काः_t MPU_x_Event_गणः_Wait_द्वयङ्काः( Event_गणः_Handle_t x_Event_गणः,
                                     ध्रुव Event_द्वयङ्काः_t ux_द्वयङ्काः_गन्तव्यम्_Wait_For,
                                     ध्रुव आधारः_Type_t xClearOnExit,
                                     ध्रुव आधारः_Type_t x_Wait_For_All_द्वयङ्काः,
                                     TickType_t x_Ticks_गन्तव्यम्_Wait ) FREERTOS_SYSTEM_CALL;
Event_द्वयङ्काः_t MPU_x_Event_गणः_Wait_द्वयङ्काः_प्रविष्टिः( ध्रुव x_Event_गणः_Wait_द्वयङ्काः_Params_t * pxParams ) FREERTOS_SYSTEM_CALL;
Event_द्वयङ्काः_t MPU_x_Event_गणः_Clear_द्वयङ्काः( Event_गणः_Handle_t x_Event_गणः,
                                      ध्रुव Event_द्वयङ्काः_t ux_द्वयङ्काः_गन्तव्यम्_Clear ) FREERTOS_SYSTEM_CALL;
Event_द्वयङ्काः_t MPU_x_Event_गणः_समुच्चयः_द्वयङ्काः( Event_गणः_Handle_t x_Event_गणः,
                                    ध्रुव Event_द्वयङ्काः_t ux_द्वयङ्काः_गन्तव्यम्_समुच्चयः ) FREERTOS_SYSTEM_CALL;
Event_द्वयङ्काः_t MPU_x_Event_गणः_Sync( Event_गणः_Handle_t x_Event_गणः,
                                 ध्रुव Event_द्वयङ्काः_t ux_द्वयङ्काः_गन्तव्यम्_समुच्चयः,
                                 ध्रुव Event_द्वयङ्काः_t ux_द्वयङ्काः_गन्तव्यम्_Wait_For,
                                 TickType_t x_Ticks_गन्तव्यम्_Wait ) FREERTOS_SYSTEM_CALL;
#यदि ( configUSE_TRACE_FACILITY == 1 )
    UBaseType_t MPU_ux_Event_गणः_Get_सङ्ख्या( शून्य * x_Event_गणः ) FREERTOS_SYSTEM_CALL;
    शून्य MPU_v_Event_गणः_समुच्चयः_सङ्ख्या( शून्य * x_Event_गणः,
                                   UBaseType_t ux_Event_गणः_सङ्ख्या ) FREERTOS_SYSTEM_CALL;
#यदि_अन्तः /* ( configUSE_TRACE_FACILITY == 1 )*/

/* Privileged only wrappers for Event Group APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
Event_गणः_Handle_t MPU_x_Event_गणः_Create( शून्य ) PRIVILEGED_FUNCTION;
Event_गणः_Handle_t MPU_x_Event_गणः_Create_Static( Static_Event_गणः_t * px_Event_गणः_बफर ) PRIVILEGED_FUNCTION;
शून्य MPU_v_Event_गणः_Delete( Event_गणः_Handle_t x_Event_गणः ) PRIVILEGED_FUNCTION;
आधारः_Type_t MPU_x_Event_गणः_Get_Static_बफर( Event_गणः_Handle_t x_Event_गणः,
                                           Static_Event_गणः_t ** ppx_Event_गणः_बफर ) PRIVILEGED_FUNCTION;
आधारः_Type_t MPU_x_Event_गणः_Clear_द्वयङ्काः_From_ISR( Event_गणः_Handle_t x_Event_गणः,
                                            ध्रुव Event_द्वयङ्काः_t ux_द्वयङ्काः_गन्तव्यम्_Clear ) PRIVILEGED_FUNCTION;
आधारः_Type_t MPU_x_Event_गणः_समुच्चयः_द्वयङ्काः_From_ISR( Event_गणः_Handle_t x_Event_गणः,
                                          ध्रुव Event_द्वयङ्काः_t ux_द्वयङ्काः_गन्तव्यम्_समुच्चयः,
                                          आधारः_Type_t * px_Higher_प्राथमिकता_Task_Woken ) PRIVILEGED_FUNCTION;
Event_द्वयङ्काः_t MPU_x_Event_गणः_Get_द्वयङ्काः_From_ISR( Event_गणः_Handle_t x_Event_गणः ) PRIVILEGED_FUNCTION;

/* MPU versions of message/stream_buffer.h API functions. */
size_t MPU_x_प्रवाहः_बफर_Send( प्रवाहः_बफर_Handle_t x_प्रवाहः_बफर,
                              ध्रुव शून्य * pv_Tx_दत्त,
                              size_t x_दत्त_दीर्घता_अष्टकानि,
                              TickType_t x_Ticks_गन्तव्यम्_Wait ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_प्रवाहः_बफर_Receive( प्रवाहः_बफर_Handle_t x_प्रवाहः_बफर,
                                 शून्य * pv_Rx_दत्त,
                                 size_t x_बफर_दीर्घता_अष्टकानि,
                                 TickType_t x_Ticks_गन्तव्यम्_Wait ) FREERTOS_SYSTEM_CALL;
आधारः_Type_t MPU_x_प्रवाहः_बफर_Is_पूर्णम्( प्रवाहः_बफर_Handle_t x_प्रवाहः_बफर ) FREERTOS_SYSTEM_CALL;
आधारः_Type_t MPU_x_प्रवाहः_बफर_Is_रिक्तानि( प्रवाहः_बफर_Handle_t x_प्रवाहः_बफर ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_प्रवाहः_बफर_Spaces_उपलब्धम्( प्रवाहः_बफर_Handle_t x_प्रवाहः_बफर ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_प्रवाहः_बफर_अष्टकानि_उपलब्धम्( प्रवाहः_बफर_Handle_t x_प्रवाहः_बफर ) FREERTOS_SYSTEM_CALL;
आधारः_Type_t MPU_x_प्रवाहः_बफर_समुच्चयः_Trigger_Level( प्रवाहः_बफर_Handle_t x_प्रवाहः_बफर,
                                             size_t xTriggerLevel ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_प्रवाहः_बफर_अग्र_सन्देशः_दीर्घता_अष्टकानि( प्रवाहः_बफर_Handle_t x_प्रवाहः_बफर ) FREERTOS_SYSTEM_CALL;

/* Privileged only wrappers for Stream Buffer APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
प्रवाहः_बफर_Handle_t MPU_x_प्रवाहः_बफर_Generic_Create( size_t x_बफर_आकार_अष्टकानि,
                                                     size_t x_Trigger_Level_अष्टकानि,
                                                     आधारः_Type_t x_प्रवाहः_बफर_Type,
                                                     प्रवाहः_बफर_Callback_Function_t pxSendCompletedCallback,
                                                     प्रवाहः_बफर_Callback_Function_t pxReceiveCompletedCallback ) PRIVILEGED_FUNCTION;
प्रवाहः_बफर_Handle_t MPU_x_प्रवाहः_बफर_Generic_Create_Static( size_t x_बफर_आकार_अष्टकानि,
                                                           size_t x_Trigger_Level_अष्टकानि,
                                                           आधारः_Type_t x_प्रवाहः_बफर_Type,
                                                           uint8_t * ध्रुव puc_प्रवाहः_बफर_Storage_Area,
                                                           Static_प्रवाहः_बफर_t * ध्रुव px_Static_प्रवाहः_बफर,
                                                           प्रवाहः_बफर_Callback_Function_t pxSendCompletedCallback,
                                                           प्रवाहः_बफर_Callback_Function_t pxReceiveCompletedCallback ) PRIVILEGED_FUNCTION;
शून्य MPU_v_प्रवाहः_बफर_Delete( प्रवाहः_बफर_Handle_t x_प्रवाहः_बफर ) PRIVILEGED_FUNCTION;
आधारः_Type_t MPU_x_प्रवाहः_बफर_Reset( प्रवाहः_बफर_Handle_t x_प्रवाहः_बफर ) PRIVILEGED_FUNCTION;
आधारः_Type_t MPU_x_प्रवाहः_बफर_Get_Static_Buffers( प्रवाहः_बफर_Handle_t x_प्रवाहः_Buffers,
                                              uint8_t * ppuc_प्रवाहः_बफर_Storage_Area,
                                              Static_प्रवाहः_बफर_t * ppx_Static_प्रवाहः_बफर ) PRIVILEGED_FUNCTION;
size_t MPU_x_प्रवाहः_बफर_Send_From_ISR( प्रवाहः_बफर_Handle_t x_प्रवाहः_बफर,
                                     ध्रुव शून्य * pv_Tx_दत्त,
                                     size_t x_दत्त_दीर्घता_अष्टकानि,
                                     आधारः_Type_t * ध्रुव px_Higher_प्राथमिकता_Task_Woken ) PRIVILEGED_FUNCTION;
size_t MPU_x_प्रवाहः_बफर_Receive_From_ISR( प्रवाहः_बफर_Handle_t x_प्रवाहः_बफर,
                                        शून्य * pv_Rx_दत्त,
                                        size_t x_बफर_दीर्घता_अष्टकानि,
                                        आधारः_Type_t * ध्रुव px_Higher_प्राथमिकता_Task_Woken ) PRIVILEGED_FUNCTION;
आधारः_Type_t MPU_x_प्रवाहः_बफर_Send_Completed_From_ISR( प्रवाहः_बफर_Handle_t x_प्रवाहः_बफर,
                                                  आधारः_Type_t * px_Higher_प्राथमिकता_Task_Woken ) PRIVILEGED_FUNCTION;
आधारः_Type_t MPU_x_प्रवाहः_बफर_Receive_Completed_From_ISR( प्रवाहः_बफर_Handle_t x_प्रवाहः_बफर,
                                                     आधारः_Type_t * px_Higher_प्राथमिकता_Task_Woken ) PRIVILEGED_FUNCTION;
आधारः_Type_t MPU_x_प्रवाहः_बफर_Reset_From_ISR( प्रवाहः_बफर_Handle_t x_प्रवाहः_बफर ) PRIVILEGED_FUNCTION;

#यदि_अन्तः /* MPU_PROTOTYPES_H */
