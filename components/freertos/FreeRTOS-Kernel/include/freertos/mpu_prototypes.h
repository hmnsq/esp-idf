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


#ifndef MPU_PROTOTYPES_H
#define MPU_PROTOTYPES_H

/* MPU versions of task.h API functions. */
BaseType_t MPU_xTaskCreate( TaskFunction_t pxTaskCode,
                            स्थिरांक अक्षर * स्थिरांक pcName,
                            स्थिरांक uint16_t us_चळत_Depth,
                            शून्य * स्थिरांक pvParameters,
                            UBaseType_t uxPriority,
                            TaskHandle_t * स्थिरांक pxCreatedTask ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskCreateStatic( TaskFunction_t pxTaskCode,
                                    स्थिरांक अक्षर * स्थिरांक pcName,
                                    स्थिरांक uint32_t ul_चळत_Depth,
                                    शून्य * स्थिरांक pvParameters,
                                    UBaseType_t uxPriority,
                                    चळत_Type_t * स्थिरांक pux_चळत_Buffer,
                                    StaticTask_t * स्थिरांक pxTaskBuffer ) FREERTOS_SYSTEM_CALL;
शून्य MPU_vTaskDelete( TaskHandle_t xTaskToDelete ) FREERTOS_SYSTEM_CALL;
शून्य MPU_vTaskDelay( स्थिरांक TickType_t xTicksToDelay ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskDelayUntil( TickType_t * स्थिरांक pxPreviousWakeTime,
                                स्थिरांक TickType_t xTimeIncrement ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskAbortDelay( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxTaskPriorityGet( स्थिरांक TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
eTaskState MPU_eTaskGetState( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
शून्य MPU_vTaskGetInfo( TaskHandle_t xTask,
                       TaskStatus_t * pxTaskStatus,
                       BaseType_t x_Get_Free_चळत_Space,
                       eTaskState eState ) FREERTOS_SYSTEM_CALL;
शून्य MPU_vTaskPrioritySet( TaskHandle_t xTask,
                           UBaseType_t uxNewPriority ) FREERTOS_SYSTEM_CALL;
शून्य MPU_vTaskSuspend( TaskHandle_t xTaskToSuspend ) FREERTOS_SYSTEM_CALL;
शून्य MPU_vTaskResume( TaskHandle_t xTaskToResume ) FREERTOS_SYSTEM_CALL;
शून्य MPU_vTaskStartScheduler( शून्य ) FREERTOS_SYSTEM_CALL;
शून्य MPU_vTaskSuspendAll( शून्य ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskResumeAll( शून्य ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_x_Task_Get_Tick_मोजणी( शून्य ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_Get_संख्या_Of_Tasks( शून्य ) FREERTOS_SYSTEM_CALL;
अक्षर * MPU_pcTaskGetName( TaskHandle_t xTaskToQuery ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetHandle( स्थिरांक अक्षर * pcNameToQuery ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_Get_चळत_वरचा_Water_Mark( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
config_चळत_DEPTH_TYPE MPU_ux_Task_Get_चळत_वरचा_Water_Mark2( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
शून्य MPU_vTaskSetApplicationTaskTag( TaskHandle_t xTask,
                                     TaskHookFunction_t pxHookFunction ) FREERTOS_SYSTEM_CALL;
TaskHookFunction_t MPU_xTaskGetApplicationTaskTag( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
शून्य MPU_vTaskSetThreadLocalStoragePointer( TaskHandle_t xTaskToSet,
                                            BaseType_t xIndex,
                                            शून्य * pv_मूल्य ) FREERTOS_SYSTEM_CALL;
शून्य * MPU_pvTaskGetThreadLocalStoragePointer( TaskHandle_t xTaskToQuery,
                                               BaseType_t xIndex ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskCallApplicationTaskHook( TaskHandle_t xTask,
                                             शून्य * pvParameter ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetIdleTaskHandle( शून्य ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxTaskGetSystemState( TaskStatus_t * स्थिरांक pxTaskStatusArray,
                                      स्थिरांक UBaseType_t uxArraySize,
                                      configRUN_TIME_COUNTER_TYPE * स्थिरांक pulTotalRunTime ) FREERTOS_SYSTEM_CALL;
configRUN_TIME_COUNTER_TYPE MPU_ulTaskGetIdleRunTimeCounter( शून्य ) FREERTOS_SYSTEM_CALL;
configRUN_TIME_COUNTER_TYPE MPU_ulTaskGetIdleRunTimePercent( शून्य ) FREERTOS_SYSTEM_CALL;
शून्य MPU_vTaskList( अक्षर * pcWriteBuffer ) FREERTOS_SYSTEM_CALL;
शून्य MPU_vTaskGetRunTimeStats( अक्षर * pcWriteBuffer ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGenericNotify( TaskHandle_t xTaskToNotify,
                                   UBaseType_t uxIndexToNotify,
                                   uint32_t ul_मूल्य,
                                   eNotifyAction eAction,
                                   uint32_t * pul_Previous_Notification_मूल्य ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGenericNotifyWait( UBaseType_t uxIndexToWaitOn,
                                       uint32_t ulBitsToClearOnEntry,
                                       uint32_t ulBitsToClearOnExit,
                                       uint32_t * pul_Notification_मूल्य,
                                       TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
uint32_t MPU_ulTaskGenericNotifyTake( UBaseType_t uxIndexToWaitOn,
                                      BaseType_t x_Clear_मोजणी_On_Exit,
                                      TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGenericNotifyStateClear( TaskHandle_t xTask,
                                             UBaseType_t uxIndexToClear ) FREERTOS_SYSTEM_CALL;
uint32_t MPU_ul_Task_Generic_Notify_मूल्य_Clear( TaskHandle_t xTask,
                                            UBaseType_t uxIndexToClear,
                                            uint32_t ulBitsToClear ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskIncrementTick( शून्य ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetCurrentTaskHandle( शून्य ) FREERTOS_SYSTEM_CALL;
शून्य MPU_vTaskSetTimeOutState( TimeOut_t * स्थिरांक pxTimeOut ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskCheckForTimeOut( TimeOut_t * स्थिरांक pxTimeOut,
                                     TickType_t * स्थिरांक pxTicksToWait ) FREERTOS_SYSTEM_CALL;
शून्य MPU_vTaskMissedYield( शून्य ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGetSchedulerState( शून्य ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskCatchUpTicks( TickType_t xTicksToCatchUp ) FREERTOS_SYSTEM_CALL;

/* MPU versions of queue.h API functions. */
BaseType_t MPU_xQueueGenericSend( QueueHandle_t xQueue,
                                  स्थिरांक शून्य * स्थिरांक pvItemToQueue,
                                  TickType_t xTicksToWait,
                                  स्थिरांक BaseType_t xCopyPosition ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueReceive( QueueHandle_t xQueue,
                              शून्य * स्थिरांक pvBuffer,
                              TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueuePeek( QueueHandle_t xQueue,
                           शून्य * स्थिरांक pvBuffer,
                           TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueSemaphoreTake( QueueHandle_t xQueue,
                                    TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxQueueMessagesWaiting( स्थिरांक QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxQueueSpacesAvailable( स्थिरांक QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
शून्य MPU_vQueueDelete( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
QueueHandle_t MPU_xQueueCreateMutex( स्थिरांक uint8_t ucQueueType ) FREERTOS_SYSTEM_CALL;
QueueHandle_t MPU_xQueueCreateMutexStatic( स्थिरांक uint8_t ucQueueType,
                                           StaticQueue_t * pxStaticQueue ) FREERTOS_SYSTEM_CALL;
QueueHandle_t MPU_xQueueCreateCountingSemaphore( स्थिरांक UBaseType_t ux_Max_मोजणी,
                                                 स्थिरांक UBaseType_t ux_Initial_मोजणी ) FREERTOS_SYSTEM_CALL;
QueueHandle_t MPU_xQueueCreateCountingSemaphoreStatic( स्थिरांक UBaseType_t ux_Max_मोजणी,
                                                       स्थिरांक UBaseType_t ux_Initial_मोजणी,
                                                       StaticQueue_t * pxStaticQueue ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xQueueGetMutexHolder( QueueHandle_t xSemaphore ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueTakeMutexRecursive( QueueHandle_t xMutex,
                                         TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueGiveMutexRecursive( QueueHandle_t pxMutex ) FREERTOS_SYSTEM_CALL;
शून्य MPU_vQueueAddToRegistry( QueueHandle_t xQueue,
                              स्थिरांक अक्षर * pcName ) FREERTOS_SYSTEM_CALL;
शून्य MPU_vQueueUnregisterQueue( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
स्थिरांक अक्षर * MPU_pcQueueGetName( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
QueueHandle_t MPU_xQueueGenericCreate( स्थिरांक UBaseType_t uxQueueLength,
                                       स्थिरांक UBaseType_t uxItemSize,
                                       स्थिरांक uint8_t ucQueueType ) FREERTOS_SYSTEM_CALL;
QueueHandle_t MPU_xQueueGenericCreateStatic( स्थिरांक UBaseType_t uxQueueLength,
                                             स्थिरांक UBaseType_t uxItemSize,
                                             uint8_t * pucQueueStorage,
                                             StaticQueue_t * pxStaticQueue,
                                             स्थिरांक uint8_t ucQueueType ) FREERTOS_SYSTEM_CALL;
QueueSetHandle_t MPU_xQueueCreateSet( स्थिरांक UBaseType_t uxEventQueueLength ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueAddToSet( QueueSetMemberHandle_t xQueueOrSemaphore,
                               QueueSetHandle_t xQueueSet ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueRemoveFromSet( QueueSetMemberHandle_t xQueueOrSemaphore,
                                    QueueSetHandle_t xQueueSet ) FREERTOS_SYSTEM_CALL;
QueueSetMemberHandle_t MPU_xQueueSelectFromSet( QueueSetHandle_t xQueueSet,
                                                स्थिरांक TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueGenericReset( QueueHandle_t xQueue,
                                   BaseType_t xNewQueue ) FREERTOS_SYSTEM_CALL;
शून्य MPU_v_Queue_Set_Queue_संख्या( QueueHandle_t xQueue,
                               UBaseType_t ux_Queue_संख्या ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Queue_Get_Queue_संख्या( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
uint8_t MPU_ucQueueGetQueueType( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;

/* MPU versions of timers.h API functions. */
TimerHandle_t MPU_xTimerCreate( स्थिरांक अक्षर * स्थिरांक pcTimerName,
                                स्थिरांक TickType_t xTimerPeriodInTicks,
                                स्थिरांक UBaseType_t uxAutoReload,
                                शून्य * स्थिरांक pvTimerID,
                                TimerCallbackFunction_t pxCallbackFunction ) FREERTOS_SYSTEM_CALL;
TimerHandle_t MPU_xTimerCreateStatic( स्थिरांक अक्षर * स्थिरांक pcTimerName,
                                      स्थिरांक TickType_t xTimerPeriodInTicks,
                                      स्थिरांक UBaseType_t uxAutoReload,
                                      शून्य * स्थिरांक pvTimerID,
                                      TimerCallbackFunction_t pxCallbackFunction,
                                      StaticTimer_t * pxTimerBuffer ) FREERTOS_SYSTEM_CALL;
शून्य * MPU_pvTimerGetTimerID( स्थिरांक TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
शून्य MPU_vTimerSetTimerID( TimerHandle_t xTimer,
                           शून्य * pvNewID ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerIsTimerActive( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTimerGetTimerDaemonTaskHandle( शून्य ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerPendFunctionCall( PendedFunction_t xFunctionToPend,
                                       शून्य * pvParameter1,
                                       uint32_t ulParameter2,
                                       TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
स्थिरांक अक्षर * MPU_pcTimerGetName( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
शून्य MPU_vTimerSetReloadMode( TimerHandle_t xTimer,
                              स्थिरांक UBaseType_t uxAutoReload ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxTimerGetReloadMode( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_xTimerGetPeriod( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_xTimerGetExpiryTime( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerCreateTimerTask( शून्य ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerGenericCommand( TimerHandle_t xTimer,
                                     स्थिरांक BaseType_t xCommandID,
                                     स्थिरांक TickType_t x_Optional_मूल्य,
                                     BaseType_t * स्थिरांक pxHigherPriorityTaskWoken,
                                     स्थिरांक TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;

/* MPU versions of event_group.h API functions. */
EventGroupHandle_t MPU_xEventGroupCreate( शून्य ) FREERTOS_SYSTEM_CALL;
EventGroupHandle_t MPU_xEventGroupCreateStatic( StaticEventGroup_t * pxEventGroupBuffer ) FREERTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupWaitBits( EventGroupHandle_t xEventGroup,
                                     स्थिरांक EventBits_t uxBitsToWaitFor,
                                     स्थिरांक BaseType_t xClearOnExit,
                                     स्थिरांक BaseType_t xWaitForAllBits,
                                     TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupClearBits( EventGroupHandle_t xEventGroup,
                                      स्थिरांक EventBits_t uxBitsToClear ) FREERTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupSetBits( EventGroupHandle_t xEventGroup,
                                    स्थिरांक EventBits_t uxBitsToSet ) FREERTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupSync( EventGroupHandle_t xEventGroup,
                                 स्थिरांक EventBits_t uxBitsToSet,
                                 स्थिरांक EventBits_t uxBitsToWaitFor,
                                 TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
शून्य MPU_vEventGroupDelete( EventGroupHandle_t xEventGroup ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Event_Group_Get_संख्या( शून्य * xEventGroup ) FREERTOS_SYSTEM_CALL;

/* MPU versions of message/stream_buffer.h API functions. */
माप_प्रकार MPU_xStreamBufferSend( StreamBufferHandle_t xStreamBuffer,
                              स्थिरांक शून्य * pvTxData,
                              माप_प्रकार xDataLengthBytes,
                              TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
माप_प्रकार MPU_xStreamBufferReceive( StreamBufferHandle_t xStreamBuffer,
                                 शून्य * pvRxData,
                                 माप_प्रकार xBufferLengthBytes,
                                 TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
माप_प्रकार MPU_xStreamBufferNextMessageLengthBytes( StreamBufferHandle_t xStreamBuffer ) FREERTOS_SYSTEM_CALL;
शून्य MPU_vStreamBufferDelete( StreamBufferHandle_t xStreamBuffer ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xStreamBufferIsFull( StreamBufferHandle_t xStreamBuffer ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xStreamBufferIsEmpty( StreamBufferHandle_t xStreamBuffer ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xStreamBufferReset( StreamBufferHandle_t xStreamBuffer ) FREERTOS_SYSTEM_CALL;
माप_प्रकार MPU_xStreamBufferSpacesAvailable( StreamBufferHandle_t xStreamBuffer ) FREERTOS_SYSTEM_CALL;
माप_प्रकार MPU_xStreamBufferBytesAvailable( StreamBufferHandle_t xStreamBuffer ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xStreamBufferSetTriggerLevel( StreamBufferHandle_t xStreamBuffer,
                                             माप_प्रकार xTriggerLevel ) FREERTOS_SYSTEM_CALL;
StreamBufferHandle_t MPU_xStreamBufferGenericCreate( माप_प्रकार xBufferSizeBytes,
                                                     माप_प्रकार xTriggerLevelBytes,
                                                     BaseType_t xIsMessageBuffer,
                                                     StreamBufferCallbackFunction_t pxSendCompletedCallback,
                                                     StreamBufferCallbackFunction_t pxReceiveCompletedCallback ) FREERTOS_SYSTEM_CALL;
StreamBufferHandle_t MPU_xStreamBufferGenericCreateStatic( माप_प्रकार xBufferSizeBytes,
                                                           माप_प्रकार xTriggerLevelBytes,
                                                           BaseType_t xIsMessageBuffer,
                                                           uint8_t * स्थिरांक pucStreamBufferStorageArea,
                                                           StaticStreamBuffer_t * स्थिरांक pxStaticStreamBuffer,
                                                           StreamBufferCallbackFunction_t pxSendCompletedCallback,
                                                           StreamBufferCallbackFunction_t pxReceiveCompletedCallback ) FREERTOS_SYSTEM_CALL;



#endif /* MPU_PROTOTYPES_H */
