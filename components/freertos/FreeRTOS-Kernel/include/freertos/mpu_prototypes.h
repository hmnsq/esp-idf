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
                            ثابت حرف * ثابت pcName,
                            ثابت uint16_t us_تر_Depth,
                            فراغ * ثابت pvParameters,
                            UBaseType_t uxPriority,
                            TaskHandle_t * ثابت pxCreatedTask ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskCreateStatic( TaskFunction_t pxTaskCode,
                                    ثابت حرف * ثابت pcName,
                                    ثابت uint32_t ul_تر_Depth,
                                    فراغ * ثابت pvParameters,
                                    UBaseType_t uxPriority,
                                    تر_Type_t * ثابت pux_تر_Buffer,
                                    StaticTask_t * ثابت pxTaskBuffer ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vTaskDelete( TaskHandle_t xTaskToDelete ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vTaskDelay( ثابت TickType_t xTicksToDelay ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskDelayUntil( TickType_t * ثابت pxPreviousWakeTime,
                                ثابت TickType_t xTimeIncrement ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskAbortDelay( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxTaskPriorityGet( ثابت TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
eTaskState MPU_eTaskGetState( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vTaskGetInfo( TaskHandle_t xTask,
                       TaskStatus_t * pxTaskStatus,
                       BaseType_t x_Get_Free_تر_Space,
                       eTaskState eState ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vTaskPrioritySet( TaskHandle_t xTask,
                           UBaseType_t uxNewPriority ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vTaskSuspend( TaskHandle_t xTaskToSuspend ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vTaskResume( TaskHandle_t xTaskToResume ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vTaskStartScheduler( فراغ ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vTaskSuspendAll( فراغ ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskResumeAll( فراغ ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_x_Task_Get_Tick_كدي( فراغ ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_Get_لمب_Of_Tasks( فراغ ) FREERTOS_SYSTEM_CALL;
حرف * MPU_pcTaskGetName( TaskHandle_t xTaskToQuery ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetHandle( ثابت حرف * pcNameToQuery ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_Get_تر_سم_Water_Mark( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
config_تر_DEPTH_TYPE MPU_ux_Task_Get_تر_سم_Water_Mark2( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vTaskSetApplicationTaskTag( TaskHandle_t xTask,
                                     TaskHookFunction_t pxHookFunction ) FREERTOS_SYSTEM_CALL;
TaskHookFunction_t MPU_xTaskGetApplicationTaskTag( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vTaskSetThreadLocalStoragePointer( TaskHandle_t xTaskToSet,
                                            BaseType_t xIndex,
                                            فراغ * pv_درج ) FREERTOS_SYSTEM_CALL;
فراغ * MPU_pvTaskGetThreadLocalStoragePointer( TaskHandle_t xTaskToQuery,
                                               BaseType_t xIndex ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskCallApplicationTaskHook( TaskHandle_t xTask,
                                             فراغ * pvParameter ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetIdleTaskHandle( فراغ ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxTaskGetSystemState( TaskStatus_t * ثابت pxTaskStatusArray,
                                      ثابت UBaseType_t uxArraySize,
                                      configRUN_TIME_COUNTER_TYPE * ثابت pulTotalRunTime ) FREERTOS_SYSTEM_CALL;
configRUN_TIME_COUNTER_TYPE MPU_ulTaskGetIdleRunTimeCounter( فراغ ) FREERTOS_SYSTEM_CALL;
configRUN_TIME_COUNTER_TYPE MPU_ulTaskGetIdleRunTimePercent( فراغ ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vTaskList( حرف * pcWriteBuffer ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vTaskGetRunTimeStats( حرف * pcWriteBuffer ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGenericNotify( TaskHandle_t xTaskToNotify,
                                   UBaseType_t uxIndexToNotify,
                                   uint32_t ul_درج,
                                   eNotifyAction eAction,
                                   uint32_t * pul_Previous_Notification_درج ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGenericNotifyWait( UBaseType_t uxIndexToWaitOn,
                                       uint32_t ulBitsToClearOnEntry,
                                       uint32_t ulBitsToClearOnExit,
                                       uint32_t * pul_Notification_درج,
                                       TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
uint32_t MPU_ulTaskGenericNotifyTake( UBaseType_t uxIndexToWaitOn,
                                      BaseType_t x_Clear_كدي_On_Exit,
                                      TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGenericNotifyStateClear( TaskHandle_t xTask,
                                             UBaseType_t uxIndexToClear ) FREERTOS_SYSTEM_CALL;
uint32_t MPU_ul_Task_Generic_Notify_درج_Clear( TaskHandle_t xTask,
                                            UBaseType_t uxIndexToClear,
                                            uint32_t ulBitsToClear ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskIncrementTick( فراغ ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetCurrentTaskHandle( فراغ ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vTaskSetTimeOutState( TimeOut_t * ثابت pxTimeOut ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskCheckForTimeOut( TimeOut_t * ثابت pxTimeOut,
                                     TickType_t * ثابت pxTicksToWait ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vTaskMissedYield( فراغ ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGetSchedulerState( فراغ ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskCatchUpTicks( TickType_t xTicksToCatchUp ) FREERTOS_SYSTEM_CALL;

/* MPU versions of queue.h API functions. */
BaseType_t MPU_xQueueGenericSend( QueueHandle_t xQueue,
                                  ثابت فراغ * ثابت pvItemToQueue,
                                  TickType_t xTicksToWait,
                                  ثابت BaseType_t xCopyPosition ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueReceive( QueueHandle_t xQueue,
                              فراغ * ثابت pvBuffer,
                              TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueuePeek( QueueHandle_t xQueue,
                           فراغ * ثابت pvBuffer,
                           TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueSemaphoreTake( QueueHandle_t xQueue,
                                    TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxQueueMessagesWaiting( ثابت QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxQueueSpacesAvailable( ثابت QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vQueueDelete( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
QueueHandle_t MPU_xQueueCreateMutex( ثابت uint8_t ucQueueType ) FREERTOS_SYSTEM_CALL;
QueueHandle_t MPU_xQueueCreateMutexStatic( ثابت uint8_t ucQueueType,
                                           StaticQueue_t * pxStaticQueue ) FREERTOS_SYSTEM_CALL;
QueueHandle_t MPU_xQueueCreateCountingSemaphore( ثابت UBaseType_t ux_Max_كدي,
                                                 ثابت UBaseType_t ux_Initial_كدي ) FREERTOS_SYSTEM_CALL;
QueueHandle_t MPU_xQueueCreateCountingSemaphoreStatic( ثابت UBaseType_t ux_Max_كدي,
                                                       ثابت UBaseType_t ux_Initial_كدي,
                                                       StaticQueue_t * pxStaticQueue ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xQueueGetMutexHolder( QueueHandle_t xSemaphore ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueTakeMutexRecursive( QueueHandle_t xMutex,
                                         TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueGiveMutexRecursive( QueueHandle_t pxMutex ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vQueueAddToRegistry( QueueHandle_t xQueue,
                              ثابت حرف * pcName ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vQueueUnregisterQueue( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
ثابت حرف * MPU_pcQueueGetName( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
QueueHandle_t MPU_xQueueGenericCreate( ثابت UBaseType_t uxQueueLength,
                                       ثابت UBaseType_t uxItemSize,
                                       ثابت uint8_t ucQueueType ) FREERTOS_SYSTEM_CALL;
QueueHandle_t MPU_xQueueGenericCreateStatic( ثابت UBaseType_t uxQueueLength,
                                             ثابت UBaseType_t uxItemSize,
                                             uint8_t * pucQueueStorage,
                                             StaticQueue_t * pxStaticQueue,
                                             ثابت uint8_t ucQueueType ) FREERTOS_SYSTEM_CALL;
QueueSetHandle_t MPU_xQueueCreateSet( ثابت UBaseType_t uxEventQueueLength ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueAddToSet( QueueSetMemberHandle_t xQueueOrSemaphore,
                               QueueSetHandle_t xQueueSet ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueRemoveFromSet( QueueSetMemberHandle_t xQueueOrSemaphore,
                                    QueueSetHandle_t xQueueSet ) FREERTOS_SYSTEM_CALL;
QueueSetMemberHandle_t MPU_xQueueSelectFromSet( QueueSetHandle_t xQueueSet,
                                                ثابت TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueGenericReset( QueueHandle_t xQueue,
                                   BaseType_t xNewQueue ) FREERTOS_SYSTEM_CALL;
فراغ MPU_v_Queue_Set_Queue_لمب( QueueHandle_t xQueue,
                               UBaseType_t ux_Queue_لمب ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Queue_Get_Queue_لمب( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
uint8_t MPU_ucQueueGetQueueType( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;

/* MPU versions of timers.h API functions. */
TimerHandle_t MPU_xTimerCreate( ثابت حرف * ثابت pcTimerName,
                                ثابت TickType_t xTimerPeriodInTicks,
                                ثابت UBaseType_t uxAutoReload,
                                فراغ * ثابت pvTimerID,
                                TimerCallbackFunction_t pxCallbackFunction ) FREERTOS_SYSTEM_CALL;
TimerHandle_t MPU_xTimerCreateStatic( ثابت حرف * ثابت pcTimerName,
                                      ثابت TickType_t xTimerPeriodInTicks,
                                      ثابت UBaseType_t uxAutoReload,
                                      فراغ * ثابت pvTimerID,
                                      TimerCallbackFunction_t pxCallbackFunction,
                                      StaticTimer_t * pxTimerBuffer ) FREERTOS_SYSTEM_CALL;
فراغ * MPU_pvTimerGetTimerID( ثابت TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vTimerSetTimerID( TimerHandle_t xTimer,
                           فراغ * pvNewID ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerIsTimerActive( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTimerGetTimerDaemonTaskHandle( فراغ ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerPendFunctionCall( PendedFunction_t xFunctionToPend,
                                       فراغ * pvParameter1,
                                       uint32_t ulParameter2,
                                       TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
ثابت حرف * MPU_pcTimerGetName( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vTimerSetReloadMode( TimerHandle_t xTimer,
                              ثابت UBaseType_t uxAutoReload ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxTimerGetReloadMode( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_xTimerGetPeriod( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_xTimerGetExpiryTime( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerCreateTimerTask( فراغ ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerGenericCommand( TimerHandle_t xTimer,
                                     ثابت BaseType_t xCommandID,
                                     ثابت TickType_t x_Optional_درج,
                                     BaseType_t * ثابت pxHigherPriorityTaskWoken,
                                     ثابت TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;

/* MPU versions of event_group.h API functions. */
EventGroupHandle_t MPU_xEventGroupCreate( فراغ ) FREERTOS_SYSTEM_CALL;
EventGroupHandle_t MPU_xEventGroupCreateStatic( StaticEventGroup_t * pxEventGroupBuffer ) FREERTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupWaitBits( EventGroupHandle_t xEventGroup,
                                     ثابت EventBits_t uxBitsToWaitFor,
                                     ثابت BaseType_t xClearOnExit,
                                     ثابت BaseType_t xWaitForAllBits,
                                     TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupClearBits( EventGroupHandle_t xEventGroup,
                                      ثابت EventBits_t uxBitsToClear ) FREERTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupSetBits( EventGroupHandle_t xEventGroup,
                                    ثابت EventBits_t uxBitsToSet ) FREERTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupSync( EventGroupHandle_t xEventGroup,
                                 ثابت EventBits_t uxBitsToSet,
                                 ثابت EventBits_t uxBitsToWaitFor,
                                 TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vEventGroupDelete( EventGroupHandle_t xEventGroup ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Event_Group_Get_لمب( فراغ * xEventGroup ) FREERTOS_SYSTEM_CALL;

/* MPU versions of message/stream_buffer.h API functions. */
size_t MPU_xStreamBufferSend( StreamBufferHandle_t xStreamBuffer,
                              ثابت فراغ * pvTxData,
                              size_t xDataLengthBytes,
                              TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
size_t MPU_xStreamBufferReceive( StreamBufferHandle_t xStreamBuffer,
                                 فراغ * pvRxData,
                                 size_t xBufferLengthBytes,
                                 TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
size_t MPU_xStreamBufferNextMessageLengthBytes( StreamBufferHandle_t xStreamBuffer ) FREERTOS_SYSTEM_CALL;
فراغ MPU_vStreamBufferDelete( StreamBufferHandle_t xStreamBuffer ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xStreamBufferIsFull( StreamBufferHandle_t xStreamBuffer ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xStreamBufferIsEmpty( StreamBufferHandle_t xStreamBuffer ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xStreamBufferReset( StreamBufferHandle_t xStreamBuffer ) FREERTOS_SYSTEM_CALL;
size_t MPU_xStreamBufferSpacesAvailable( StreamBufferHandle_t xStreamBuffer ) FREERTOS_SYSTEM_CALL;
size_t MPU_xStreamBufferBytesAvailable( StreamBufferHandle_t xStreamBuffer ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xStreamBufferSetTriggerLevel( StreamBufferHandle_t xStreamBuffer,
                                             size_t xTriggerLevel ) FREERTOS_SYSTEM_CALL;
StreamBufferHandle_t MPU_xStreamBufferGenericCreate( size_t xBufferSizeBytes,
                                                     size_t xTriggerLevelBytes,
                                                     BaseType_t xIsMessageBuffer,
                                                     StreamBufferCallbackFunction_t pxSendCompletedCallback,
                                                     StreamBufferCallbackFunction_t pxReceiveCompletedCallback ) FREERTOS_SYSTEM_CALL;
StreamBufferHandle_t MPU_xStreamBufferGenericCreateStatic( size_t xBufferSizeBytes,
                                                           size_t xTriggerLevelBytes,
                                                           BaseType_t xIsMessageBuffer,
                                                           uint8_t * ثابت pucStreamBufferStorageArea,
                                                           StaticStreamBuffer_t * ثابت pxStaticStreamBuffer,
                                                           StreamBufferCallbackFunction_t pxSendCompletedCallback,
                                                           StreamBufferCallbackFunction_t pxReceiveCompletedCallback ) FREERTOS_SYSTEM_CALL;



#endif /* MPU_PROTOTYPES_H */
