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
                            ଅଚଳ ଅକ୍ଷର * ଅଚଳ pcName,
                            ଅଚଳ uint16_t us_ଗଦା_Depth,
                            ଶୂନ୍ୟ * ଅଚଳ pvParameters,
                            UBaseType_t uxPriority,
                            TaskHandle_t * ଅଚଳ pxCreatedTask ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskCreateStatic( TaskFunction_t pxTaskCode,
                                    ଅଚଳ ଅକ୍ଷର * ଅଚଳ pcName,
                                    ଅଚଳ uint32_t ul_ଗଦା_Depth,
                                    ଶୂନ୍ୟ * ଅଚଳ pvParameters,
                                    UBaseType_t uxPriority,
                                    ଗଦା_Type_t * ଅଚଳ pux_ଗଦା_Buffer,
                                    StaticTask_t * ଅଚଳ pxTaskBuffer ) FREERTOS_SYSTEM_CALL;
ଶୂନ୍ୟ MPU_vTaskDelete( TaskHandle_t xTaskToDelete ) FREERTOS_SYSTEM_CALL;
ଶୂନ୍ୟ MPU_vTaskDelay( ଅଚଳ TickType_t xTicksToDelay ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskDelayUntil( TickType_t * ଅଚଳ pxPreviousWakeTime,
                                ଅଚଳ TickType_t xTimeIncrement ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskAbortDelay( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxTaskPriorityGet( ଅଚଳ TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
eTaskState MPU_eTaskGetState( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
ଶୂନ୍ୟ MPU_vTaskGetInfo( TaskHandle_t xTask,
                       TaskStatus_t * pxTaskStatus,
                       BaseType_t x_Get_Free_ଗଦା_Space,
                       eTaskState eState ) FREERTOS_SYSTEM_CALL;
ଶୂନ୍ୟ MPU_vTaskPrioritySet( TaskHandle_t xTask,
                           UBaseType_t uxNewPriority ) FREERTOS_SYSTEM_CALL;
ଶୂନ୍ୟ MPU_vTaskSuspend( TaskHandle_t xTaskToSuspend ) FREERTOS_SYSTEM_CALL;
ଶୂନ୍ୟ MPU_vTaskResume( TaskHandle_t xTaskToResume ) FREERTOS_SYSTEM_CALL;
ଶୂନ୍ୟ MPU_vTaskStartScheduler( ଶୂନ୍ୟ ) FREERTOS_SYSTEM_CALL;
ଶୂନ୍ୟ MPU_vTaskSuspendAll( ଶୂନ୍ୟ ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskResumeAll( ଶୂନ୍ୟ ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_x_Task_Get_Tick_ଗଣନା( ଶୂନ୍ୟ ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_Get_ସଂଖ୍ୟା_Of_Tasks( ଶୂନ୍ୟ ) FREERTOS_SYSTEM_CALL;
ଅକ୍ଷର * MPU_pcTaskGetName( TaskHandle_t xTaskToQuery ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetHandle( ଅଚଳ ଅକ୍ଷର * pcNameToQuery ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_Get_ଗଦା_ଉପର_Water_Mark( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
config_ଗଦା_DEPTH_TYPE MPU_ux_Task_Get_ଗଦା_ଉପର_Water_Mark2( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
ଶୂନ୍ୟ MPU_vTaskSetApplicationTaskTag( TaskHandle_t xTask,
                                     TaskHookFunction_t pxHookFunction ) FREERTOS_SYSTEM_CALL;
TaskHookFunction_t MPU_xTaskGetApplicationTaskTag( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
ଶୂନ୍ୟ MPU_vTaskSetThreadLocalStoragePointer( TaskHandle_t xTaskToSet,
                                            BaseType_t xIndex,
                                            ଶୂନ୍ୟ * pv_ମୂଲ୍ୟ ) FREERTOS_SYSTEM_CALL;
ଶୂନ୍ୟ * MPU_pvTaskGetThreadLocalStoragePointer( TaskHandle_t xTaskToQuery,
                                               BaseType_t xIndex ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskCallApplicationTaskHook( TaskHandle_t xTask,
                                             ଶୂନ୍ୟ * pvParameter ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetIdleTaskHandle( ଶୂନ୍ୟ ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxTaskGetSystemState( TaskStatus_t * ଅଚଳ pxTaskStatusArray,
                                      ଅଚଳ UBaseType_t uxArraySize,
                                      configRUN_TIME_COUNTER_TYPE * ଅଚଳ pulTotalRunTime ) FREERTOS_SYSTEM_CALL;
configRUN_TIME_COUNTER_TYPE MPU_ulTaskGetIdleRunTimeCounter( ଶୂନ୍ୟ ) FREERTOS_SYSTEM_CALL;
configRUN_TIME_COUNTER_TYPE MPU_ulTaskGetIdleRunTimePercent( ଶୂନ୍ୟ ) FREERTOS_SYSTEM_CALL;
ଶୂନ୍ୟ MPU_vTaskList( ଅକ୍ଷର * pcWriteBuffer ) FREERTOS_SYSTEM_CALL;
ଶୂନ୍ୟ MPU_vTaskGetRunTimeStats( ଅକ୍ଷର * pcWriteBuffer ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGenericNotify( TaskHandle_t xTaskToNotify,
                                   UBaseType_t uxIndexToNotify,
                                   uint32_t ul_ମୂଲ୍ୟ,
                                   eNotifyAction eAction,
                                   uint32_t * pul_Previous_Notification_ମୂଲ୍ୟ ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGenericNotifyWait( UBaseType_t uxIndexToWaitOn,
                                       uint32_t ulBitsToClearOnEntry,
                                       uint32_t ulBitsToClearOnExit,
                                       uint32_t * pul_Notification_ମୂଲ୍ୟ,
                                       TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
uint32_t MPU_ulTaskGenericNotifyTake( UBaseType_t uxIndexToWaitOn,
                                      BaseType_t x_Clear_ଗଣନା_On_Exit,
                                      TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGenericNotifyStateClear( TaskHandle_t xTask,
                                             UBaseType_t uxIndexToClear ) FREERTOS_SYSTEM_CALL;
uint32_t MPU_ul_Task_Generic_Notify_ମୂଲ୍ୟ_Clear( TaskHandle_t xTask,
                                            UBaseType_t uxIndexToClear,
                                            uint32_t ulBitsToClear ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskIncrementTick( ଶୂନ୍ୟ ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetCurrentTaskHandle( ଶୂନ୍ୟ ) FREERTOS_SYSTEM_CALL;
ଶୂନ୍ୟ MPU_vTaskSetTimeOutState( TimeOut_t * ଅଚଳ pxTimeOut ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskCheckForTimeOut( TimeOut_t * ଅଚଳ pxTimeOut,
                                     TickType_t * ଅଚଳ pxTicksToWait ) FREERTOS_SYSTEM_CALL;
ଶୂନ୍ୟ MPU_vTaskMissedYield( ଶୂନ୍ୟ ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGetSchedulerState( ଶୂନ୍ୟ ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskCatchUpTicks( TickType_t xTicksToCatchUp ) FREERTOS_SYSTEM_CALL;

/* MPU versions of queue.h API functions. */
BaseType_t MPU_xQueueGenericSend( QueueHandle_t xQueue,
                                  ଅଚଳ ଶୂନ୍ୟ * ଅଚଳ pvItemToQueue,
                                  TickType_t xTicksToWait,
                                  ଅଚଳ BaseType_t xCopyPosition ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueReceive( QueueHandle_t xQueue,
                              ଶୂନ୍ୟ * ଅଚଳ pvBuffer,
                              TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueuePeek( QueueHandle_t xQueue,
                           ଶୂନ୍ୟ * ଅଚଳ pvBuffer,
                           TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueSemaphoreTake( QueueHandle_t xQueue,
                                    TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxQueueMessagesWaiting( ଅଚଳ QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxQueueSpacesAvailable( ଅଚଳ QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
ଶୂନ୍ୟ MPU_vQueueDelete( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
QueueHandle_t MPU_xQueueCreateMutex( ଅଚଳ uint8_t ucQueueType ) FREERTOS_SYSTEM_CALL;
QueueHandle_t MPU_xQueueCreateMutexStatic( ଅଚଳ uint8_t ucQueueType,
                                           StaticQueue_t * pxStaticQueue ) FREERTOS_SYSTEM_CALL;
QueueHandle_t MPU_xQueueCreateCountingSemaphore( ଅଚଳ UBaseType_t ux_Max_ଗଣନା,
                                                 ଅଚଳ UBaseType_t ux_Initial_ଗଣନା ) FREERTOS_SYSTEM_CALL;
QueueHandle_t MPU_xQueueCreateCountingSemaphoreStatic( ଅଚଳ UBaseType_t ux_Max_ଗଣନା,
                                                       ଅଚଳ UBaseType_t ux_Initial_ଗଣନା,
                                                       StaticQueue_t * pxStaticQueue ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xQueueGetMutexHolder( QueueHandle_t xSemaphore ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueTakeMutexRecursive( QueueHandle_t xMutex,
                                         TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueGiveMutexRecursive( QueueHandle_t pxMutex ) FREERTOS_SYSTEM_CALL;
ଶୂନ୍ୟ MPU_vQueueAddToRegistry( QueueHandle_t xQueue,
                              ଅଚଳ ଅକ୍ଷର * pcName ) FREERTOS_SYSTEM_CALL;
ଶୂନ୍ୟ MPU_vQueueUnregisterQueue( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
ଅଚଳ ଅକ୍ଷର * MPU_pcQueueGetName( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
QueueHandle_t MPU_xQueueGenericCreate( ଅଚଳ UBaseType_t uxQueueLength,
                                       ଅଚଳ UBaseType_t uxItemSize,
                                       ଅଚଳ uint8_t ucQueueType ) FREERTOS_SYSTEM_CALL;
QueueHandle_t MPU_xQueueGenericCreateStatic( ଅଚଳ UBaseType_t uxQueueLength,
                                             ଅଚଳ UBaseType_t uxItemSize,
                                             uint8_t * pucQueueStorage,
                                             StaticQueue_t * pxStaticQueue,
                                             ଅଚଳ uint8_t ucQueueType ) FREERTOS_SYSTEM_CALL;
QueueSetHandle_t MPU_xQueueCreateSet( ଅଚଳ UBaseType_t uxEventQueueLength ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueAddToSet( QueueSetMemberHandle_t xQueueOrSemaphore,
                               QueueSetHandle_t xQueueSet ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueRemoveFromSet( QueueSetMemberHandle_t xQueueOrSemaphore,
                                    QueueSetHandle_t xQueueSet ) FREERTOS_SYSTEM_CALL;
QueueSetMemberHandle_t MPU_xQueueSelectFromSet( QueueSetHandle_t xQueueSet,
                                                ଅଚଳ TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueGenericReset( QueueHandle_t xQueue,
                                   BaseType_t xNewQueue ) FREERTOS_SYSTEM_CALL;
ଶୂନ୍ୟ MPU_v_Queue_Set_Queue_ସଂଖ୍ୟା( QueueHandle_t xQueue,
                               UBaseType_t ux_Queue_ସଂଖ୍ୟା ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Queue_Get_Queue_ସଂଖ୍ୟା( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
uint8_t MPU_ucQueueGetQueueType( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;

/* MPU versions of timers.h API functions. */
TimerHandle_t MPU_xTimerCreate( ଅଚଳ ଅକ୍ଷର * ଅଚଳ pcTimerName,
                                ଅଚଳ TickType_t xTimerPeriodInTicks,
                                ଅଚଳ UBaseType_t uxAutoReload,
                                ଶୂନ୍ୟ * ଅଚଳ pvTimerID,
                                TimerCallbackFunction_t pxCallbackFunction ) FREERTOS_SYSTEM_CALL;
TimerHandle_t MPU_xTimerCreateStatic( ଅଚଳ ଅକ୍ଷର * ଅଚଳ pcTimerName,
                                      ଅଚଳ TickType_t xTimerPeriodInTicks,
                                      ଅଚଳ UBaseType_t uxAutoReload,
                                      ଶୂନ୍ୟ * ଅଚଳ pvTimerID,
                                      TimerCallbackFunction_t pxCallbackFunction,
                                      StaticTimer_t * pxTimerBuffer ) FREERTOS_SYSTEM_CALL;
ଶୂନ୍ୟ * MPU_pvTimerGetTimerID( ଅଚଳ TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
ଶୂନ୍ୟ MPU_vTimerSetTimerID( TimerHandle_t xTimer,
                           ଶୂନ୍ୟ * pvNewID ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerIsTimerActive( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTimerGetTimerDaemonTaskHandle( ଶୂନ୍ୟ ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerPendFunctionCall( PendedFunction_t xFunctionToPend,
                                       ଶୂନ୍ୟ * pvParameter1,
                                       uint32_t ulParameter2,
                                       TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
ଅଚଳ ଅକ୍ଷର * MPU_pcTimerGetName( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
ଶୂନ୍ୟ MPU_vTimerSetReloadMode( TimerHandle_t xTimer,
                              ଅଚଳ UBaseType_t uxAutoReload ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxTimerGetReloadMode( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_xTimerGetPeriod( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_xTimerGetExpiryTime( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerCreateTimerTask( ଶୂନ୍ୟ ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerGenericCommand( TimerHandle_t xTimer,
                                     ଅଚଳ BaseType_t xCommandID,
                                     ଅଚଳ TickType_t x_Optional_ମୂଲ୍ୟ,
                                     BaseType_t * ଅଚଳ pxHigherPriorityTaskWoken,
                                     ଅଚଳ TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;

/* MPU versions of event_group.h API functions. */
EventGroupHandle_t MPU_xEventGroupCreate( ଶୂନ୍ୟ ) FREERTOS_SYSTEM_CALL;
EventGroupHandle_t MPU_xEventGroupCreateStatic( StaticEventGroup_t * pxEventGroupBuffer ) FREERTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupWaitBits( EventGroupHandle_t xEventGroup,
                                     ଅଚଳ EventBits_t uxBitsToWaitFor,
                                     ଅଚଳ BaseType_t xClearOnExit,
                                     ଅଚଳ BaseType_t xWaitForAllBits,
                                     TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupClearBits( EventGroupHandle_t xEventGroup,
                                      ଅଚଳ EventBits_t uxBitsToClear ) FREERTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupSetBits( EventGroupHandle_t xEventGroup,
                                    ଅଚଳ EventBits_t uxBitsToSet ) FREERTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupSync( EventGroupHandle_t xEventGroup,
                                 ଅଚଳ EventBits_t uxBitsToSet,
                                 ଅଚଳ EventBits_t uxBitsToWaitFor,
                                 TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
ଶୂନ୍ୟ MPU_vEventGroupDelete( EventGroupHandle_t xEventGroup ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Event_Group_Get_ସଂଖ୍ୟା( ଶୂନ୍ୟ * xEventGroup ) FREERTOS_SYSTEM_CALL;

/* MPU versions of message/stream_buffer.h API functions. */
size_t MPU_xStreamBufferSend( StreamBufferHandle_t xStreamBuffer,
                              ଅଚଳ ଶୂନ୍ୟ * pvTxData,
                              size_t xDataLengthBytes,
                              TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
size_t MPU_xStreamBufferReceive( StreamBufferHandle_t xStreamBuffer,
                                 ଶୂନ୍ୟ * pvRxData,
                                 size_t xBufferLengthBytes,
                                 TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
size_t MPU_xStreamBufferNextMessageLengthBytes( StreamBufferHandle_t xStreamBuffer ) FREERTOS_SYSTEM_CALL;
ଶୂନ୍ୟ MPU_vStreamBufferDelete( StreamBufferHandle_t xStreamBuffer ) FREERTOS_SYSTEM_CALL;
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
                                                           uint8_t * ଅଚଳ pucStreamBufferStorageArea,
                                                           StaticStreamBuffer_t * ଅଚଳ pxStaticStreamBuffer,
                                                           StreamBufferCallbackFunction_t pxSendCompletedCallback,
                                                           StreamBufferCallbackFunction_t pxReceiveCompletedCallback ) FREERTOS_SYSTEM_CALL;



#endif /* MPU_PROTOTYPES_H */
