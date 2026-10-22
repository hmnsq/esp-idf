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
                            സ്ഥിരം അക്ഷരം * സ്ഥിരം pcName,
                            സ്ഥിരം uint16_t us_അട്ടി_Depth,
                            ശൂന്യം * സ്ഥിരം pvParameters,
                            UBaseType_t uxPriority,
                            TaskHandle_t * സ്ഥിരം pxCreatedTask ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskCreateStatic( TaskFunction_t pxTaskCode,
                                    സ്ഥിരം അക്ഷരം * സ്ഥിരം pcName,
                                    സ്ഥിരം uint32_t ul_അട്ടി_Depth,
                                    ശൂന്യം * സ്ഥിരം pvParameters,
                                    UBaseType_t uxPriority,
                                    അട്ടി_Type_t * സ്ഥിരം pux_അട്ടി_Buffer,
                                    StaticTask_t * സ്ഥിരം pxTaskBuffer ) FREERTOS_SYSTEM_CALL;
ശൂന്യം MPU_vTaskDelete( TaskHandle_t xTaskToDelete ) FREERTOS_SYSTEM_CALL;
ശൂന്യം MPU_vTaskDelay( സ്ഥിരം TickType_t xTicksToDelay ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskDelayUntil( TickType_t * സ്ഥിരം pxPreviousWakeTime,
                                സ്ഥിരം TickType_t xTimeIncrement ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskAbortDelay( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxTaskPriorityGet( സ്ഥിരം TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
eTaskState MPU_eTaskGetState( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
ശൂന്യം MPU_vTaskGetInfo( TaskHandle_t xTask,
                       TaskStatus_t * pxTaskStatus,
                       BaseType_t x_Get_Free_അട്ടി_Space,
                       eTaskState eState ) FREERTOS_SYSTEM_CALL;
ശൂന്യം MPU_vTaskPrioritySet( TaskHandle_t xTask,
                           UBaseType_t uxNewPriority ) FREERTOS_SYSTEM_CALL;
ശൂന്യം MPU_vTaskSuspend( TaskHandle_t xTaskToSuspend ) FREERTOS_SYSTEM_CALL;
ശൂന്യം MPU_vTaskResume( TaskHandle_t xTaskToResume ) FREERTOS_SYSTEM_CALL;
ശൂന്യം MPU_vTaskStartScheduler( ശൂന്യം ) FREERTOS_SYSTEM_CALL;
ശൂന്യം MPU_vTaskSuspendAll( ശൂന്യം ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskResumeAll( ശൂന്യം ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_x_Task_Get_Tick_എണ്ണം( ശൂന്യം ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_Get_സംഖ്യ_Of_Tasks( ശൂന്യം ) FREERTOS_SYSTEM_CALL;
അക്ഷരം * MPU_pcTaskGetName( TaskHandle_t xTaskToQuery ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetHandle( സ്ഥിരം അക്ഷരം * pcNameToQuery ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_Get_അട്ടി_മുകളിലെ_Water_Mark( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
config_അട്ടി_DEPTH_TYPE MPU_ux_Task_Get_അട്ടി_മുകളിലെ_Water_Mark2( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
ശൂന്യം MPU_vTaskSetApplicationTaskTag( TaskHandle_t xTask,
                                     TaskHookFunction_t pxHookFunction ) FREERTOS_SYSTEM_CALL;
TaskHookFunction_t MPU_xTaskGetApplicationTaskTag( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
ശൂന്യം MPU_vTaskSetThreadLocalStoragePointer( TaskHandle_t xTaskToSet,
                                            BaseType_t xIndex,
                                            ശൂന്യം * pv_മൂല്യം ) FREERTOS_SYSTEM_CALL;
ശൂന്യം * MPU_pvTaskGetThreadLocalStoragePointer( TaskHandle_t xTaskToQuery,
                                               BaseType_t xIndex ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskCallApplicationTaskHook( TaskHandle_t xTask,
                                             ശൂന്യം * pvParameter ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetIdleTaskHandle( ശൂന്യം ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxTaskGetSystemState( TaskStatus_t * സ്ഥിരം pxTaskStatusArray,
                                      സ്ഥിരം UBaseType_t uxArraySize,
                                      configRUN_TIME_COUNTER_TYPE * സ്ഥിരം pulTotalRunTime ) FREERTOS_SYSTEM_CALL;
configRUN_TIME_COUNTER_TYPE MPU_ulTaskGetIdleRunTimeCounter( ശൂന്യം ) FREERTOS_SYSTEM_CALL;
configRUN_TIME_COUNTER_TYPE MPU_ulTaskGetIdleRunTimePercent( ശൂന്യം ) FREERTOS_SYSTEM_CALL;
ശൂന്യം MPU_vTaskList( അക്ഷരം * pcWriteBuffer ) FREERTOS_SYSTEM_CALL;
ശൂന്യം MPU_vTaskGetRunTimeStats( അക്ഷരം * pcWriteBuffer ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGenericNotify( TaskHandle_t xTaskToNotify,
                                   UBaseType_t uxIndexToNotify,
                                   uint32_t ul_മൂല്യം,
                                   eNotifyAction eAction,
                                   uint32_t * pul_Previous_Notification_മൂല്യം ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGenericNotifyWait( UBaseType_t uxIndexToWaitOn,
                                       uint32_t ulBitsToClearOnEntry,
                                       uint32_t ulBitsToClearOnExit,
                                       uint32_t * pul_Notification_മൂല്യം,
                                       TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
uint32_t MPU_ulTaskGenericNotifyTake( UBaseType_t uxIndexToWaitOn,
                                      BaseType_t x_Clear_എണ്ണം_On_Exit,
                                      TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGenericNotifyStateClear( TaskHandle_t xTask,
                                             UBaseType_t uxIndexToClear ) FREERTOS_SYSTEM_CALL;
uint32_t MPU_ul_Task_Generic_Notify_മൂല്യം_Clear( TaskHandle_t xTask,
                                            UBaseType_t uxIndexToClear,
                                            uint32_t ulBitsToClear ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskIncrementTick( ശൂന്യം ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetCurrentTaskHandle( ശൂന്യം ) FREERTOS_SYSTEM_CALL;
ശൂന്യം MPU_vTaskSetTimeOutState( TimeOut_t * സ്ഥിരം pxTimeOut ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskCheckForTimeOut( TimeOut_t * സ്ഥിരം pxTimeOut,
                                     TickType_t * സ്ഥിരം pxTicksToWait ) FREERTOS_SYSTEM_CALL;
ശൂന്യം MPU_vTaskMissedYield( ശൂന്യം ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGetSchedulerState( ശൂന്യം ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskCatchUpTicks( TickType_t xTicksToCatchUp ) FREERTOS_SYSTEM_CALL;

/* MPU versions of queue.h API functions. */
BaseType_t MPU_xQueueGenericSend( QueueHandle_t xQueue,
                                  സ്ഥിരം ശൂന്യം * സ്ഥിരം pvItemToQueue,
                                  TickType_t xTicksToWait,
                                  സ്ഥിരം BaseType_t xCopyPosition ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueReceive( QueueHandle_t xQueue,
                              ശൂന്യം * സ്ഥിരം pvBuffer,
                              TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueuePeek( QueueHandle_t xQueue,
                           ശൂന്യം * സ്ഥിരം pvBuffer,
                           TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueSemaphoreTake( QueueHandle_t xQueue,
                                    TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxQueueMessagesWaiting( സ്ഥിരം QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxQueueSpacesAvailable( സ്ഥിരം QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
ശൂന്യം MPU_vQueueDelete( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
QueueHandle_t MPU_xQueueCreateMutex( സ്ഥിരം uint8_t ucQueueType ) FREERTOS_SYSTEM_CALL;
QueueHandle_t MPU_xQueueCreateMutexStatic( സ്ഥിരം uint8_t ucQueueType,
                                           StaticQueue_t * pxStaticQueue ) FREERTOS_SYSTEM_CALL;
QueueHandle_t MPU_xQueueCreateCountingSemaphore( സ്ഥിരം UBaseType_t ux_Max_എണ്ണം,
                                                 സ്ഥിരം UBaseType_t ux_Initial_എണ്ണം ) FREERTOS_SYSTEM_CALL;
QueueHandle_t MPU_xQueueCreateCountingSemaphoreStatic( സ്ഥിരം UBaseType_t ux_Max_എണ്ണം,
                                                       സ്ഥിരം UBaseType_t ux_Initial_എണ്ണം,
                                                       StaticQueue_t * pxStaticQueue ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xQueueGetMutexHolder( QueueHandle_t xSemaphore ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueTakeMutexRecursive( QueueHandle_t xMutex,
                                         TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueGiveMutexRecursive( QueueHandle_t pxMutex ) FREERTOS_SYSTEM_CALL;
ശൂന്യം MPU_vQueueAddToRegistry( QueueHandle_t xQueue,
                              സ്ഥിരം അക്ഷരം * pcName ) FREERTOS_SYSTEM_CALL;
ശൂന്യം MPU_vQueueUnregisterQueue( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
സ്ഥിരം അക്ഷരം * MPU_pcQueueGetName( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
QueueHandle_t MPU_xQueueGenericCreate( സ്ഥിരം UBaseType_t uxQueueLength,
                                       സ്ഥിരം UBaseType_t uxItemSize,
                                       സ്ഥിരം uint8_t ucQueueType ) FREERTOS_SYSTEM_CALL;
QueueHandle_t MPU_xQueueGenericCreateStatic( സ്ഥിരം UBaseType_t uxQueueLength,
                                             സ്ഥിരം UBaseType_t uxItemSize,
                                             uint8_t * pucQueueStorage,
                                             StaticQueue_t * pxStaticQueue,
                                             സ്ഥിരം uint8_t ucQueueType ) FREERTOS_SYSTEM_CALL;
QueueSetHandle_t MPU_xQueueCreateSet( സ്ഥിരം UBaseType_t uxEventQueueLength ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueAddToSet( QueueSetMemberHandle_t xQueueOrSemaphore,
                               QueueSetHandle_t xQueueSet ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueRemoveFromSet( QueueSetMemberHandle_t xQueueOrSemaphore,
                                    QueueSetHandle_t xQueueSet ) FREERTOS_SYSTEM_CALL;
QueueSetMemberHandle_t MPU_xQueueSelectFromSet( QueueSetHandle_t xQueueSet,
                                                സ്ഥിരം TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueGenericReset( QueueHandle_t xQueue,
                                   BaseType_t xNewQueue ) FREERTOS_SYSTEM_CALL;
ശൂന്യം MPU_v_Queue_Set_Queue_സംഖ്യ( QueueHandle_t xQueue,
                               UBaseType_t ux_Queue_സംഖ്യ ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Queue_Get_Queue_സംഖ്യ( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
uint8_t MPU_ucQueueGetQueueType( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;

/* MPU versions of timers.h API functions. */
TimerHandle_t MPU_xTimerCreate( സ്ഥിരം അക്ഷരം * സ്ഥിരം pcTimerName,
                                സ്ഥിരം TickType_t xTimerPeriodInTicks,
                                സ്ഥിരം UBaseType_t uxAutoReload,
                                ശൂന്യം * സ്ഥിരം pvTimerID,
                                TimerCallbackFunction_t pxCallbackFunction ) FREERTOS_SYSTEM_CALL;
TimerHandle_t MPU_xTimerCreateStatic( സ്ഥിരം അക്ഷരം * സ്ഥിരം pcTimerName,
                                      സ്ഥിരം TickType_t xTimerPeriodInTicks,
                                      സ്ഥിരം UBaseType_t uxAutoReload,
                                      ശൂന്യം * സ്ഥിരം pvTimerID,
                                      TimerCallbackFunction_t pxCallbackFunction,
                                      StaticTimer_t * pxTimerBuffer ) FREERTOS_SYSTEM_CALL;
ശൂന്യം * MPU_pvTimerGetTimerID( സ്ഥിരം TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
ശൂന്യം MPU_vTimerSetTimerID( TimerHandle_t xTimer,
                           ശൂന്യം * pvNewID ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerIsTimerActive( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTimerGetTimerDaemonTaskHandle( ശൂന്യം ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerPendFunctionCall( PendedFunction_t xFunctionToPend,
                                       ശൂന്യം * pvParameter1,
                                       uint32_t ulParameter2,
                                       TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
സ്ഥിരം അക്ഷരം * MPU_pcTimerGetName( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
ശൂന്യം MPU_vTimerSetReloadMode( TimerHandle_t xTimer,
                              സ്ഥിരം UBaseType_t uxAutoReload ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxTimerGetReloadMode( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_xTimerGetPeriod( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_xTimerGetExpiryTime( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerCreateTimerTask( ശൂന്യം ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerGenericCommand( TimerHandle_t xTimer,
                                     സ്ഥിരം BaseType_t xCommandID,
                                     സ്ഥിരം TickType_t x_Optional_മൂല്യം,
                                     BaseType_t * സ്ഥിരം pxHigherPriorityTaskWoken,
                                     സ്ഥിരം TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;

/* MPU versions of event_group.h API functions. */
EventGroupHandle_t MPU_xEventGroupCreate( ശൂന്യം ) FREERTOS_SYSTEM_CALL;
EventGroupHandle_t MPU_xEventGroupCreateStatic( StaticEventGroup_t * pxEventGroupBuffer ) FREERTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupWaitBits( EventGroupHandle_t xEventGroup,
                                     സ്ഥിരം EventBits_t uxBitsToWaitFor,
                                     സ്ഥിരം BaseType_t xClearOnExit,
                                     സ്ഥിരം BaseType_t xWaitForAllBits,
                                     TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupClearBits( EventGroupHandle_t xEventGroup,
                                      സ്ഥിരം EventBits_t uxBitsToClear ) FREERTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupSetBits( EventGroupHandle_t xEventGroup,
                                    സ്ഥിരം EventBits_t uxBitsToSet ) FREERTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupSync( EventGroupHandle_t xEventGroup,
                                 സ്ഥിരം EventBits_t uxBitsToSet,
                                 സ്ഥിരം EventBits_t uxBitsToWaitFor,
                                 TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
ശൂന്യം MPU_vEventGroupDelete( EventGroupHandle_t xEventGroup ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Event_Group_Get_സംഖ്യ( ശൂന്യം * xEventGroup ) FREERTOS_SYSTEM_CALL;

/* MPU versions of message/stream_buffer.h API functions. */
size_t MPU_xStreamBufferSend( StreamBufferHandle_t xStreamBuffer,
                              സ്ഥിരം ശൂന്യം * pvTxData,
                              size_t xDataLengthBytes,
                              TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
size_t MPU_xStreamBufferReceive( StreamBufferHandle_t xStreamBuffer,
                                 ശൂന്യം * pvRxData,
                                 size_t xBufferLengthBytes,
                                 TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
size_t MPU_xStreamBufferNextMessageLengthBytes( StreamBufferHandle_t xStreamBuffer ) FREERTOS_SYSTEM_CALL;
ശൂന്യം MPU_vStreamBufferDelete( StreamBufferHandle_t xStreamBuffer ) FREERTOS_SYSTEM_CALL;
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
                                                           uint8_t * സ്ഥിരം pucStreamBufferStorageArea,
                                                           StaticStreamBuffer_t * സ്ഥിരം pxStaticStreamBuffer,
                                                           StreamBufferCallbackFunction_t pxSendCompletedCallback,
                                                           StreamBufferCallbackFunction_t pxReceiveCompletedCallback ) FREERTOS_SYSTEM_CALL;



#endif /* MPU_PROTOTYPES_H */
