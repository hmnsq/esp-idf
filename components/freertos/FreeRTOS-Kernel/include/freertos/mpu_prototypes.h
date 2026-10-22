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


#अगर_अघोषित MPU_PROTOTYPES_H
#घोषणा MPU_PROTOTYPES_H

/* MPU versions of task.h API functions. */
BaseType_t MPU_xTaskCreate( TaskFunction_t pxTaskCode,
                            स्थिर अक्षर * स्थिर pcName,
                            स्थिर uint16_t us_ढेर_Depth,
                            व्योम * स्थिर pvParameters,
                            UBaseType_t uxPriority,
                            TaskHandle_t * स्थिर pxCreatedTask ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskCreateStatic( TaskFunction_t pxTaskCode,
                                    स्थिर अक्षर * स्थिर pcName,
                                    स्थिर uint32_t ul_ढेर_Depth,
                                    व्योम * स्थिर pvParameters,
                                    UBaseType_t uxPriority,
                                    ढेर_Type_t * स्थिर pux_ढेर_बफर,
                                    StaticTask_t * स्थिर px_Task_बफर ) FREERTOS_SYSTEM_CALL;
व्योम MPU_vTaskDelete( TaskHandle_t xTaskToDelete ) FREERTOS_SYSTEM_CALL;
व्योम MPU_vTaskDelay( स्थिर TickType_t xTicksToDelay ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskDelayUntil( TickType_t * स्थिर pxPreviousWakeTime,
                                स्थिर TickType_t xTimeIncrement ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskAbortDelay( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxTaskPriorityGet( स्थिर TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
eTaskState MPU_eTaskGetState( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
व्योम MPU_vTaskGetInfo( TaskHandle_t xTask,
                       TaskStatus_t * pxTaskStatus,
                       BaseType_t x_Get_Free_ढेर_Space,
                       eTaskState eState ) FREERTOS_SYSTEM_CALL;
व्योम MPU_vTaskPrioritySet( TaskHandle_t xTask,
                           UBaseType_t uxNewPriority ) FREERTOS_SYSTEM_CALL;
व्योम MPU_vTaskSuspend( TaskHandle_t xTaskToSuspend ) FREERTOS_SYSTEM_CALL;
व्योम MPU_vTaskResume( TaskHandle_t xTaskToResume ) FREERTOS_SYSTEM_CALL;
व्योम MPU_vTaskStartScheduler( व्योम ) FREERTOS_SYSTEM_CALL;
व्योम MPU_vTaskSuspendAll( व्योम ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskResumeAll( व्योम ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_x_Task_Get_Tick_गिनती( व्योम ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_Get_संख्या_Of_Tasks( व्योम ) FREERTOS_SYSTEM_CALL;
अक्षर * MPU_pcTaskGetName( TaskHandle_t xTaskToQuery ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetHandle( स्थिर अक्षर * pcNameToQuery ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_Get_ढेर_ऊपरी_Water_Mark( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
config_ढेर_DEPTH_TYPE MPU_ux_Task_Get_ढेर_ऊपरी_Water_Mark2( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
व्योम MPU_vTaskSetApplicationTaskTag( TaskHandle_t xTask,
                                     TaskHookFunction_t pxHookFunction ) FREERTOS_SYSTEM_CALL;
TaskHookFunction_t MPU_xTaskGetApplicationTaskTag( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
व्योम MPU_vTaskSetThreadLocalStoragePointer( TaskHandle_t xTaskToSet,
                                            BaseType_t x_सूचकांक,
                                            व्योम * pv_मूल्य ) FREERTOS_SYSTEM_CALL;
व्योम * MPU_pvTaskGetThreadLocalStoragePointer( TaskHandle_t xTaskToQuery,
                                               BaseType_t x_सूचकांक ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskCallApplicationTaskHook( TaskHandle_t xTask,
                                             व्योम * pvParameter ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetIdleTaskHandle( व्योम ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxTaskGetSystemState( TaskStatus_t * स्थिर pxTaskStatusArray,
                                      स्थिर UBaseType_t ux_Array_आकार,
                                      config_RUN_TIME_गणक_TYPE * स्थिर pul_योग_Run_Time ) FREERTOS_SYSTEM_CALL;
config_RUN_TIME_गणक_TYPE MPU_ul_Task_Get_Idle_Run_Time_गणक( व्योम ) FREERTOS_SYSTEM_CALL;
config_RUN_TIME_गणक_TYPE MPU_ulTaskGetIdleRunTimePercent( व्योम ) FREERTOS_SYSTEM_CALL;
व्योम MPU_vTaskList( अक्षर * pc_Write_बफर ) FREERTOS_SYSTEM_CALL;
व्योम MPU_vTaskGetRunTimeStats( अक्षर * pc_Write_बफर ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGenericNotify( TaskHandle_t xTaskToNotify,
                                   UBaseType_t ux_सूचकांक_To_Notify,
                                   uint32_t ul_मूल्य,
                                   eNotifyAction eAction,
                                   uint32_t * pul_Previous_Notification_मूल्य ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGenericNotifyWait( UBaseType_t ux_सूचकांक_To_Wait_On,
                                       uint32_t ulBitsToClearOnEntry,
                                       uint32_t ulBitsToClearOnExit,
                                       uint32_t * pul_Notification_मूल्य,
                                       TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
uint32_t MPU_ulTaskGenericNotifyTake( UBaseType_t ux_सूचकांक_To_Wait_On,
                                      BaseType_t x_Clear_गिनती_On_Exit,
                                      TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGenericNotifyStateClear( TaskHandle_t xTask,
                                             UBaseType_t ux_सूचकांक_To_Clear ) FREERTOS_SYSTEM_CALL;
uint32_t MPU_ul_Task_Generic_Notify_मूल्य_Clear( TaskHandle_t xTask,
                                            UBaseType_t ux_सूचकांक_To_Clear,
                                            uint32_t ulBitsToClear ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskIncrementTick( व्योम ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetCurrentTaskHandle( व्योम ) FREERTOS_SYSTEM_CALL;
व्योम MPU_vTaskSetTimeOutState( TimeOut_t * स्थिर pxTimeOut ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskCheckForTimeOut( TimeOut_t * स्थिर pxTimeOut,
                                     TickType_t * स्थिर pxTicksToWait ) FREERTOS_SYSTEM_CALL;
व्योम MPU_vTaskMissedYield( व्योम ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGetSchedulerState( व्योम ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskCatchUpTicks( TickType_t xTicksToCatchUp ) FREERTOS_SYSTEM_CALL;

/* MPU versions of queue.h API functions. */
BaseType_t MPU_xQueueGenericSend( QueueHandle_t xQueue,
                                  स्थिर व्योम * स्थिर pvItemToQueue,
                                  TickType_t xTicksToWait,
                                  स्थिर BaseType_t xCopyPosition ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueReceive( QueueHandle_t xQueue,
                              व्योम * स्थिर pv_बफर,
                              TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueuePeek( QueueHandle_t xQueue,
                           व्योम * स्थिर pv_बफर,
                           TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueSemaphoreTake( QueueHandle_t xQueue,
                                    TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxQueueMessagesWaiting( स्थिर QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxQueueSpacesAvailable( स्थिर QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
व्योम MPU_vQueueDelete( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
QueueHandle_t MPU_xQueueCreateMutex( स्थिर uint8_t ucQueueType ) FREERTOS_SYSTEM_CALL;
QueueHandle_t MPU_xQueueCreateMutexStatic( स्थिर uint8_t ucQueueType,
                                           StaticQueue_t * pxStaticQueue ) FREERTOS_SYSTEM_CALL;
QueueHandle_t MPU_xQueueCreateCountingSemaphore( स्थिर UBaseType_t ux_Max_गिनती,
                                                 स्थिर UBaseType_t ux_Initial_गिनती ) FREERTOS_SYSTEM_CALL;
QueueHandle_t MPU_xQueueCreateCountingSemaphoreStatic( स्थिर UBaseType_t ux_Max_गिनती,
                                                       स्थिर UBaseType_t ux_Initial_गिनती,
                                                       StaticQueue_t * pxStaticQueue ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xQueueGetMutexHolder( QueueHandle_t xSemaphore ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueTakeMutexRecursive( QueueHandle_t xMutex,
                                         TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueGiveMutexRecursive( QueueHandle_t pxMutex ) FREERTOS_SYSTEM_CALL;
व्योम MPU_vQueueAddToRegistry( QueueHandle_t xQueue,
                              स्थिर अक्षर * pcName ) FREERTOS_SYSTEM_CALL;
व्योम MPU_vQueueUnregisterQueue( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
स्थिर अक्षर * MPU_pcQueueGetName( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
QueueHandle_t MPU_xQueueGenericCreate( स्थिर UBaseType_t ux_Queue_लंबाई,
                                       स्थिर UBaseType_t ux_Item_आकार,
                                       स्थिर uint8_t ucQueueType ) FREERTOS_SYSTEM_CALL;
QueueHandle_t MPU_xQueueGenericCreateStatic( स्थिर UBaseType_t ux_Queue_लंबाई,
                                             स्थिर UBaseType_t ux_Item_आकार,
                                             uint8_t * pucQueueStorage,
                                             StaticQueue_t * pxStaticQueue,
                                             स्थिर uint8_t ucQueueType ) FREERTOS_SYSTEM_CALL;
QueueSetHandle_t MPU_xQueueCreateSet( स्थिर UBaseType_t ux_Event_Queue_लंबाई ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueAddToSet( QueueSetMemberHandle_t xQueueOrSemaphore,
                               QueueSetHandle_t xQueueSet ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueRemoveFromSet( QueueSetMemberHandle_t xQueueOrSemaphore,
                                    QueueSetHandle_t xQueueSet ) FREERTOS_SYSTEM_CALL;
QueueSetMemberHandle_t MPU_xQueueSelectFromSet( QueueSetHandle_t xQueueSet,
                                                स्थिर TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueGenericReset( QueueHandle_t xQueue,
                                   BaseType_t xNewQueue ) FREERTOS_SYSTEM_CALL;
व्योम MPU_v_Queue_Set_Queue_संख्या( QueueHandle_t xQueue,
                               UBaseType_t ux_Queue_संख्या ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Queue_Get_Queue_संख्या( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
uint8_t MPU_ucQueueGetQueueType( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;

/* MPU versions of timers.h API functions. */
TimerHandle_t MPU_xTimerCreate( स्थिर अक्षर * स्थिर pcTimerName,
                                स्थिर TickType_t xTimerPeriodInTicks,
                                स्थिर UBaseType_t uxAutoReload,
                                व्योम * स्थिर pvTimerID,
                                TimerCallbackFunction_t pxCallbackFunction ) FREERTOS_SYSTEM_CALL;
TimerHandle_t MPU_xTimerCreateStatic( स्थिर अक्षर * स्थिर pcTimerName,
                                      स्थिर TickType_t xTimerPeriodInTicks,
                                      स्थिर UBaseType_t uxAutoReload,
                                      व्योम * स्थिर pvTimerID,
                                      TimerCallbackFunction_t pxCallbackFunction,
                                      StaticTimer_t * px_Timer_बफर ) FREERTOS_SYSTEM_CALL;
व्योम * MPU_pvTimerGetTimerID( स्थिर TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
व्योम MPU_vTimerSetTimerID( TimerHandle_t xTimer,
                           व्योम * pvNewID ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerIsTimerActive( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTimerGetTimerDaemonTaskHandle( व्योम ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerPendFunctionCall( PendedFunction_t xFunctionToPend,
                                       व्योम * pvParameter1,
                                       uint32_t ulParameter2,
                                       TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
स्थिर अक्षर * MPU_pcTimerGetName( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
व्योम MPU_vTimerSetReloadMode( TimerHandle_t xTimer,
                              स्थिर UBaseType_t uxAutoReload ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxTimerGetReloadMode( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_xTimerGetPeriod( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_xTimerGetExpiryTime( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerCreateTimerTask( व्योम ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerGenericCommand( TimerHandle_t xTimer,
                                     स्थिर BaseType_t xCommandID,
                                     स्थिर TickType_t x_Optional_मूल्य,
                                     BaseType_t * स्थिर pxHigherPriorityTaskWoken,
                                     स्थिर TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;

/* MPU versions of event_group.h API functions. */
EventGroupHandle_t MPU_xEventGroupCreate( व्योम ) FREERTOS_SYSTEM_CALL;
EventGroupHandle_t MPU_xEventGroupCreateStatic( StaticEventGroup_t * px_Event_Group_बफर ) FREERTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupWaitBits( EventGroupHandle_t xEventGroup,
                                     स्थिर EventBits_t uxBitsToWaitFor,
                                     स्थिर BaseType_t xClearOnExit,
                                     स्थिर BaseType_t xWaitForAllBits,
                                     TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupClearBits( EventGroupHandle_t xEventGroup,
                                      स्थिर EventBits_t uxBitsToClear ) FREERTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupSetBits( EventGroupHandle_t xEventGroup,
                                    स्थिर EventBits_t uxBitsToSet ) FREERTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupSync( EventGroupHandle_t xEventGroup,
                                 स्थिर EventBits_t uxBitsToSet,
                                 स्थिर EventBits_t uxBitsToWaitFor,
                                 TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
व्योम MPU_vEventGroupDelete( EventGroupHandle_t xEventGroup ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Event_Group_Get_संख्या( व्योम * xEventGroup ) FREERTOS_SYSTEM_CALL;

/* MPU versions of message/stream_buffer.h API functions. */
माप_प्रकार MPU_x_Stream_बफर_Send( Stream_बफर_Handle_t x_Stream_बफर,
                              स्थिर व्योम * pv_Tx_तथ्य,
                              माप_प्रकार x_तथ्य_लंबाई_Bytes,
                              TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
माप_प्रकार MPU_x_Stream_बफर_Receive( Stream_बफर_Handle_t x_Stream_बफर,
                                 व्योम * pv_Rx_तथ्य,
                                 माप_प्रकार x_बफर_लंबाई_Bytes,
                                 TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
माप_प्रकार MPU_x_Stream_बफर_अग्र_Message_लंबाई_Bytes( Stream_बफर_Handle_t x_Stream_बफर ) FREERTOS_SYSTEM_CALL;
व्योम MPU_v_Stream_बफर_Delete( Stream_बफर_Handle_t x_Stream_बफर ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_x_Stream_बफर_Is_Full( Stream_बफर_Handle_t x_Stream_बफर ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_x_Stream_बफर_Is_Empty( Stream_बफर_Handle_t x_Stream_बफर ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_x_Stream_बफर_Reset( Stream_बफर_Handle_t x_Stream_बफर ) FREERTOS_SYSTEM_CALL;
माप_प्रकार MPU_x_Stream_बफर_Spaces_Available( Stream_बफर_Handle_t x_Stream_बफर ) FREERTOS_SYSTEM_CALL;
माप_प्रकार MPU_x_Stream_बफर_Bytes_Available( Stream_बफर_Handle_t x_Stream_बफर ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_x_Stream_बफर_Set_Trigger_Level( Stream_बफर_Handle_t x_Stream_बफर,
                                             माप_प्रकार xTriggerLevel ) FREERTOS_SYSTEM_CALL;
Stream_बफर_Handle_t MPU_x_Stream_बफर_Generic_Create( माप_प्रकार x_बफर_आकार_Bytes,
                                                     माप_प्रकार xTriggerLevelBytes,
                                                     BaseType_t x_Is_Message_बफर,
                                                     Stream_बफर_Callback_Function_t pxSendCompletedCallback,
                                                     Stream_बफर_Callback_Function_t pxReceiveCompletedCallback ) FREERTOS_SYSTEM_CALL;
Stream_बफर_Handle_t MPU_x_Stream_बफर_Generic_Create_Static( माप_प्रकार x_बफर_आकार_Bytes,
                                                           माप_प्रकार xTriggerLevelBytes,
                                                           BaseType_t x_Is_Message_बफर,
                                                           uint8_t * स्थिर puc_Stream_बफर_Storage_Area,
                                                           Static_Stream_बफर_t * स्थिर px_Static_Stream_बफर,
                                                           Stream_बफर_Callback_Function_t pxSendCompletedCallback,
                                                           Stream_बफर_Callback_Function_t pxReceiveCompletedCallback ) FREERTOS_SYSTEM_CALL;



#पूर्ण_अगर /* MPU_PROTOTYPES_H */
