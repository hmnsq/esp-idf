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


#ifndef MPU_PROTOTYPES_H
#define MPU_PROTOTYPES_H

வகைவரை கட்டமைப்பு xTaskGenericNotifyParams
{
    TaskHandle_t xTaskToNotify;
    UBaseType_t uxIndexToNotify;
    uint32_t ul_மதிப்பு;
    eNotifyAction eAction;
    uint32_t * pul_Previous_Notification_மதிப்பு;
} xTaskGenericNotifyParams_t;

வகைவரை கட்டமைப்பு xTaskGenericNotifyWaitParams
{
    UBaseType_t uxIndexToWaitOn;
    uint32_t ulBitsToClearOnEntry;
    uint32_t ulBitsToClearOnExit;
    uint32_t * pul_Notification_மதிப்பு;
    TickType_t xTicksToWait;
} xTaskGenericNotifyWaitParams_t;

வகைவரை கட்டமைப்பு xTimerGenericCommandFromTaskParams
{
    TimerHandle_t xTimer;
    BaseType_t xCommandID;
    TickType_t x_Optional_மதிப்பு;
    BaseType_t * pxHigherPriorityTaskWoken;
    TickType_t xTicksToWait;
} xTimerGenericCommandFromTaskParams_t;

வகைவரை கட்டமைப்பு xEventGroupWaitBitsParams
{
    EventGroupHandle_t xEventGroup;
    EventBits_t uxBitsToWaitFor;
    BaseType_t xClearOnExit;
    BaseType_t xWaitForAllBits;
    TickType_t xTicksToWait;
} xEventGroupWaitBitsParams_t;

/* MPU versions of task.h API functions. */
வெறுமை MPU_vTaskDelay( மாறிலி TickType_t xTicksToDelay ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskDelayUntil( TickType_t * மாறிலி pxPreviousWakeTime,
                                மாறிலி TickType_t xTimeIncrement ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskAbortDelay( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxTaskPriorityGet( மாறிலி TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
eTaskState MPU_eTaskGetState( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
வெறுமை MPU_vTaskGetInfo( TaskHandle_t xTask,
                       TaskStatus_t * pxTaskStatus,
                       BaseType_t x_Get_Free_அடுக்கு_Space,
                       eTaskState eState ) FREERTOS_SYSTEM_CALL;
வெறுமை MPU_vTaskSuspend( TaskHandle_t xTaskToSuspend ) FREERTOS_SYSTEM_CALL;
வெறுமை MPU_vTaskResume( TaskHandle_t xTaskToResume ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_x_Task_Get_Tick_மொத்தம்( வெறுமை ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_Get_எண்_Of_Tasks( வெறுமை ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_Get_அடுக்கு_உயர்வு_Water_Mark( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
config_அடுக்கு_DEPTH_TYPE MPU_ux_Task_Get_அடுக்கு_உயர்வு_Water_Mark2( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
வெறுமை MPU_vTaskSetApplicationTaskTag( TaskHandle_t xTask,
                                     TaskHookFunction_t pxHookFunction ) FREERTOS_SYSTEM_CALL;
TaskHookFunction_t MPU_xTaskGetApplicationTaskTag( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
வெறுமை MPU_vTaskSetThreadLocalStoragePointer( TaskHandle_t xTaskToSet,
                                            BaseType_t xIndex,
                                            வெறுமை * pv_மதிப்பு ) FREERTOS_SYSTEM_CALL;
வெறுமை * MPU_pvTaskGetThreadLocalStoragePointer( TaskHandle_t xTaskToQuery,
                                               BaseType_t xIndex ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetIdleTaskHandle( வெறுமை ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxTaskGetSystemState( TaskStatus_t * மாறிலி pxTaskStatusArray,
                                      மாறிலி UBaseType_t uxArraySize,
                                      configRUN_TIME_COUNTER_TYPE * மாறிலி pulTotalRunTime ) FREERTOS_SYSTEM_CALL;
configRUN_TIME_COUNTER_TYPE MPU_ulTaskGetRunTimeCounter( மாறிலி TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
configRUN_TIME_COUNTER_TYPE MPU_ulTaskGetRunTimePercent( மாறிலி TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
configRUN_TIME_COUNTER_TYPE MPU_ulTaskGetIdleRunTimeCounter( வெறுமை ) FREERTOS_SYSTEM_CALL;
configRUN_TIME_COUNTER_TYPE MPU_ulTaskGetIdleRunTimePercent( வெறுமை ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGenericNotify( TaskHandle_t xTaskToNotify,
                                   UBaseType_t uxIndexToNotify,
                                   uint32_t ul_மதிப்பு,
                                   eNotifyAction eAction,
                                   uint32_t * pul_Previous_Notification_மதிப்பு ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGenericNotifyEntry( மாறிலி xTaskGenericNotifyParams_t * pxParams ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGenericNotifyWait( UBaseType_t uxIndexToWaitOn,
                                       uint32_t ulBitsToClearOnEntry,
                                       uint32_t ulBitsToClearOnExit,
                                       uint32_t * pul_Notification_மதிப்பு,
                                       TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGenericNotifyWaitEntry( மாறிலி xTaskGenericNotifyWaitParams_t * pxParams ) FREERTOS_SYSTEM_CALL;
uint32_t MPU_ulTaskGenericNotifyTake( UBaseType_t uxIndexToWaitOn,
                                      BaseType_t x_Clear_மொத்தம்_On_Exit,
                                      TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGenericNotifyStateClear( TaskHandle_t xTask,
                                             UBaseType_t uxIndexToClear ) FREERTOS_SYSTEM_CALL;
uint32_t MPU_ul_Task_Generic_Notify_மதிப்பு_Clear( TaskHandle_t xTask,
                                            UBaseType_t uxIndexToClear,
                                            uint32_t ulBitsToClear ) FREERTOS_SYSTEM_CALL;
வெறுமை MPU_vTaskSetTimeOutState( TimeOut_t * மாறிலி pxTimeOut ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskCheckForTimeOut( TimeOut_t * மாறிலி pxTimeOut,
                                     TickType_t * மாறிலி pxTicksToWait ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetCurrentTaskHandle( வெறுமை ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGetSchedulerState( வெறுமை ) FREERTOS_SYSTEM_CALL;

/* Privileged only wrappers for Task APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
BaseType_t MPU_xTaskCreate( TaskFunction_t pxTaskCode,
                            மாறிலி எழுத்து * மாறிலி pcName,
                            மாறிலி config_அடுக்கு_DEPTH_TYPE ux_அடுக்கு_Depth,
                            வெறுமை * மாறிலி pvParameters,
                            UBaseType_t uxPriority,
                            TaskHandle_t * மாறிலி pxCreatedTask ) PRIVILEGED_FUNCTION;
TaskHandle_t MPU_xTaskCreateStatic( TaskFunction_t pxTaskCode,
                                    மாறிலி எழுத்து * மாறிலி pcName,
                                    மாறிலி config_அடுக்கு_DEPTH_TYPE ux_அடுக்கு_Depth,
                                    வெறுமை * மாறிலி pvParameters,
                                    UBaseType_t uxPriority,
                                    அடுக்கு_Type_t * மாறிலி pux_அடுக்கு_Buffer,
                                    StaticTask_t * மாறிலி pxTaskBuffer ) PRIVILEGED_FUNCTION;
வெறுமை MPU_vTaskDelete( TaskHandle_t xTaskToDelete ) PRIVILEGED_FUNCTION;
வெறுமை MPU_vTaskPrioritySet( TaskHandle_t xTask,
                           UBaseType_t uxNewPriority ) PRIVILEGED_FUNCTION;
TaskHandle_t MPU_xTaskGetHandle( மாறிலி எழுத்து * pcNameToQuery ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xTaskCallApplicationTaskHook( TaskHandle_t xTask,
                                             வெறுமை * pvParameter ) PRIVILEGED_FUNCTION;
எழுத்து * MPU_pcTaskGetName( TaskHandle_t xTaskToQuery ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xTaskCreateRestricted( மாறிலி TaskParameters_t * மாறிலி pxTaskDefinition,
                                      TaskHandle_t * pxCreatedTask ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xTaskCreateRestrictedStatic( மாறிலி TaskParameters_t * மாறிலி pxTaskDefinition,
                                            TaskHandle_t * pxCreatedTask ) PRIVILEGED_FUNCTION;
வெறுமை MPU_vTaskAllocateMPURegions( TaskHandle_t xTaskToModify,
                                  மாறிலி MemoryRegion_t * மாறிலி xRegions ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xTaskGetStaticBuffers( TaskHandle_t xTask,
                                      அடுக்கு_Type_t ** ppux_அடுக்கு_Buffer,
                                      StaticTask_t ** ppxTaskBuffer ) PRIVILEGED_FUNCTION;
UBaseType_t MPU_uxTaskPriorityGetFromISR( மாறிலி TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
UBaseType_t MPU_uxTaskBasePriorityGet( மாறிலி TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
UBaseType_t MPU_uxTaskBasePriorityGetFromISR( மாறிலி TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xTaskResumeFromISR( TaskHandle_t xTaskToResume ) PRIVILEGED_FUNCTION;
TaskHookFunction_t MPU_xTaskGetApplicationTaskTagFromISR( TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xTaskGenericNotifyFromISR( TaskHandle_t xTaskToNotify,
                                          UBaseType_t uxIndexToNotify,
                                          uint32_t ul_மதிப்பு,
                                          eNotifyAction eAction,
                                          uint32_t * pul_Previous_Notification_மதிப்பு,
                                          BaseType_t * pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;
வெறுமை MPU_vTaskGenericNotifyGiveFromISR( TaskHandle_t xTaskToNotify,
                                        UBaseType_t uxIndexToNotify,
                                        BaseType_t * pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;

/* MPU versions of queue.h API functions. */
BaseType_t MPU_xQueueGenericSend( QueueHandle_t xQueue,
                                  மாறிலி வெறுமை * மாறிலி pvItemToQueue,
                                  TickType_t xTicksToWait,
                                  மாறிலி BaseType_t xCopyPosition ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueReceive( QueueHandle_t xQueue,
                              வெறுமை * மாறிலி pvBuffer,
                              TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueuePeek( QueueHandle_t xQueue,
                           வெறுமை * மாறிலி pvBuffer,
                           TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueSemaphoreTake( QueueHandle_t xQueue,
                                    TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxQueueMessagesWaiting( மாறிலி QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxQueueSpacesAvailable( மாறிலி QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xQueueGetMutexHolder( QueueHandle_t xSemaphore ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueTakeMutexRecursive( QueueHandle_t xMutex,
                                         TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueGiveMutexRecursive( QueueHandle_t pxMutex ) FREERTOS_SYSTEM_CALL;
வெறுமை MPU_vQueueAddToRegistry( QueueHandle_t xQueue,
                              மாறிலி எழுத்து * pcName ) FREERTOS_SYSTEM_CALL;
வெறுமை MPU_vQueueUnregisterQueue( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
மாறிலி எழுத்து * MPU_pcQueueGetName( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueAddToSet( QueueSetMemberHandle_t xQueueOrSemaphore,
                               QueueSetHandle_t xQueueSet ) FREERTOS_SYSTEM_CALL;
QueueSetMemberHandle_t MPU_xQueueSelectFromSet( QueueSetHandle_t xQueueSet,
                                                மாறிலி TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
வெறுமை MPU_v_Queue_Set_Queue_எண்( QueueHandle_t xQueue,
                               UBaseType_t ux_Queue_எண் ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Queue_Get_Queue_எண்( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
uint8_t MPU_ucQueueGetQueueType( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;

/* Privileged only wrappers for Queue APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
வெறுமை MPU_vQueueDelete( QueueHandle_t xQueue ) PRIVILEGED_FUNCTION;
QueueHandle_t MPU_xQueueCreateMutex( மாறிலி uint8_t ucQueueType ) PRIVILEGED_FUNCTION;
QueueHandle_t MPU_xQueueCreateMutexStatic( மாறிலி uint8_t ucQueueType,
                                           StaticQueue_t * pxStaticQueue ) PRIVILEGED_FUNCTION;
QueueHandle_t MPU_xQueueCreateCountingSemaphore( மாறிலி UBaseType_t ux_Max_மொத்தம்,
                                                 மாறிலி UBaseType_t ux_Initial_மொத்தம் ) PRIVILEGED_FUNCTION;
QueueHandle_t MPU_xQueueCreateCountingSemaphoreStatic( மாறிலி UBaseType_t ux_Max_மொத்தம்,
                                                       மாறிலி UBaseType_t ux_Initial_மொத்தம்,
                                                       StaticQueue_t * pxStaticQueue ) PRIVILEGED_FUNCTION;
QueueHandle_t MPU_xQueueGenericCreate( மாறிலி UBaseType_t uxQueueLength,
                                       மாறிலி UBaseType_t uxItemSize,
                                       மாறிலி uint8_t ucQueueType ) PRIVILEGED_FUNCTION;
QueueHandle_t MPU_xQueueGenericCreateStatic( மாறிலி UBaseType_t uxQueueLength,
                                             மாறிலி UBaseType_t uxItemSize,
                                             uint8_t * pucQueueStorage,
                                             StaticQueue_t * pxStaticQueue,
                                             மாறிலி uint8_t ucQueueType ) PRIVILEGED_FUNCTION;
QueueSetHandle_t MPU_xQueueCreateSet( மாறிலி UBaseType_t uxEventQueueLength ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xQueueRemoveFromSet( QueueSetMemberHandle_t xQueueOrSemaphore,
                                    QueueSetHandle_t xQueueSet ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xQueueGenericReset( QueueHandle_t xQueue,
                                   BaseType_t xNewQueue ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xQueueGenericGetStaticBuffers( QueueHandle_t xQueue,
                                              uint8_t ** ppucQueueStorage,
                                              StaticQueue_t ** ppxStaticQueue ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xQueueGenericSendFromISR( QueueHandle_t xQueue,
                                         மாறிலி வெறுமை * மாறிலி pvItemToQueue,
                                         BaseType_t * மாறிலி pxHigherPriorityTaskWoken,
                                         மாறிலி BaseType_t xCopyPosition ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xQueueGiveFromISR( QueueHandle_t xQueue,
                                  BaseType_t * மாறிலி pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xQueuePeekFromISR( QueueHandle_t xQueue,
                                  வெறுமை * மாறிலி pvBuffer ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xQueueReceiveFromISR( QueueHandle_t xQueue,
                                     வெறுமை * மாறிலி pvBuffer,
                                     BaseType_t * மாறிலி pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xQueueIsQueueEmptyFromISR( மாறிலி QueueHandle_t xQueue ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xQueueIsQueueFullFromISR( மாறிலி QueueHandle_t xQueue ) PRIVILEGED_FUNCTION;
UBaseType_t MPU_uxQueueMessagesWaitingFromISR( மாறிலி QueueHandle_t xQueue ) PRIVILEGED_FUNCTION;
TaskHandle_t MPU_xQueueGetMutexHolderFromISR( QueueHandle_t xSemaphore ) PRIVILEGED_FUNCTION;
QueueSetMemberHandle_t MPU_xQueueSelectFromSetFromISR( QueueSetHandle_t xQueueSet ) PRIVILEGED_FUNCTION;

/* MPU versions of timers.h API functions. */
வெறுமை * MPU_pvTimerGetTimerID( மாறிலி TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
வெறுமை MPU_vTimerSetTimerID( TimerHandle_t xTimer,
                           வெறுமை * pvNewID ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerIsTimerActive( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTimerGetTimerDaemonTaskHandle( வெறுமை ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerGenericCommandFromTask( TimerHandle_t xTimer,
                                             மாறிலி BaseType_t xCommandID,
                                             மாறிலி TickType_t x_Optional_மதிப்பு,
                                             BaseType_t * மாறிலி pxHigherPriorityTaskWoken,
                                             மாறிலி TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerGenericCommandFromTaskEntry( மாறிலி xTimerGenericCommandFromTaskParams_t * pxParams ) FREERTOS_SYSTEM_CALL;
மாறிலி எழுத்து * MPU_pcTimerGetName( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
வெறுமை MPU_vTimerSetReloadMode( TimerHandle_t xTimer,
                              மாறிலி BaseType_t uxAutoReload ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerGetReloadMode( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxTimerGetReloadMode( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_xTimerGetPeriod( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_xTimerGetExpiryTime( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;

/* Privileged only wrappers for Timer APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
TimerHandle_t MPU_xTimerCreate( மாறிலி எழுத்து * மாறிலி pcTimerName,
                                மாறிலி TickType_t xTimerPeriodInTicks,
                                மாறிலி UBaseType_t uxAutoReload,
                                வெறுமை * மாறிலி pvTimerID,
                                TimerCallbackFunction_t pxCallbackFunction ) PRIVILEGED_FUNCTION;
TimerHandle_t MPU_xTimerCreateStatic( மாறிலி எழுத்து * மாறிலி pcTimerName,
                                      மாறிலி TickType_t xTimerPeriodInTicks,
                                      மாறிலி UBaseType_t uxAutoReload,
                                      வெறுமை * மாறிலி pvTimerID,
                                      TimerCallbackFunction_t pxCallbackFunction,
                                      StaticTimer_t * pxTimerBuffer ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xTimerGetStaticBuffer( TimerHandle_t xTimer,
                                      StaticTimer_t ** ppxTimerBuffer ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xTimerGenericCommandFromISR( TimerHandle_t xTimer,
                                            மாறிலி BaseType_t xCommandID,
                                            மாறிலி TickType_t x_Optional_மதிப்பு,
                                            BaseType_t * மாறிலி pxHigherPriorityTaskWoken,
                                            மாறிலி TickType_t xTicksToWait ) PRIVILEGED_FUNCTION;

/* MPU versions of event_group.h API functions. */
EventBits_t MPU_xEventGroupWaitBits( EventGroupHandle_t xEventGroup,
                                     மாறிலி EventBits_t uxBitsToWaitFor,
                                     மாறிலி BaseType_t xClearOnExit,
                                     மாறிலி BaseType_t xWaitForAllBits,
                                     TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupWaitBitsEntry( மாறிலி xEventGroupWaitBitsParams_t * pxParams ) FREERTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupClearBits( EventGroupHandle_t xEventGroup,
                                      மாறிலி EventBits_t uxBitsToClear ) FREERTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupSetBits( EventGroupHandle_t xEventGroup,
                                    மாறிலி EventBits_t uxBitsToSet ) FREERTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupSync( EventGroupHandle_t xEventGroup,
                                 மாறிலி EventBits_t uxBitsToSet,
                                 மாறிலி EventBits_t uxBitsToWaitFor,
                                 TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
#எனில் ( configUSE_TRACE_FACILITY == 1 )
    UBaseType_t MPU_ux_Event_Group_Get_எண்( வெறுமை * xEventGroup ) FREERTOS_SYSTEM_CALL;
    வெறுமை MPU_v_Event_Group_Set_எண்( வெறுமை * xEventGroup,
                                   UBaseType_t ux_Event_Group_எண் ) FREERTOS_SYSTEM_CALL;
#endif /* ( configUSE_TRACE_FACILITY == 1 )*/

/* Privileged only wrappers for Event Group APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
EventGroupHandle_t MPU_xEventGroupCreate( வெறுமை ) PRIVILEGED_FUNCTION;
EventGroupHandle_t MPU_xEventGroupCreateStatic( StaticEventGroup_t * pxEventGroupBuffer ) PRIVILEGED_FUNCTION;
வெறுமை MPU_vEventGroupDelete( EventGroupHandle_t xEventGroup ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xEventGroupGetStaticBuffer( EventGroupHandle_t xEventGroup,
                                           StaticEventGroup_t ** ppxEventGroupBuffer ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xEventGroupClearBitsFromISR( EventGroupHandle_t xEventGroup,
                                            மாறிலி EventBits_t uxBitsToClear ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xEventGroupSetBitsFromISR( EventGroupHandle_t xEventGroup,
                                          மாறிலி EventBits_t uxBitsToSet,
                                          BaseType_t * pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;
EventBits_t MPU_xEventGroupGetBitsFromISR( EventGroupHandle_t xEventGroup ) PRIVILEGED_FUNCTION;

/* MPU versions of message/stream_buffer.h API functions. */
size_t MPU_xStreamBufferSend( StreamBufferHandle_t xStreamBuffer,
                              மாறிலி வெறுமை * pvTxData,
                              size_t xDataLengthBytes,
                              TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
size_t MPU_xStreamBufferReceive( StreamBufferHandle_t xStreamBuffer,
                                 வெறுமை * pvRxData,
                                 size_t xBufferLengthBytes,
                                 TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xStreamBufferIsFull( StreamBufferHandle_t xStreamBuffer ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xStreamBufferIsEmpty( StreamBufferHandle_t xStreamBuffer ) FREERTOS_SYSTEM_CALL;
size_t MPU_xStreamBufferSpacesAvailable( StreamBufferHandle_t xStreamBuffer ) FREERTOS_SYSTEM_CALL;
size_t MPU_xStreamBufferBytesAvailable( StreamBufferHandle_t xStreamBuffer ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xStreamBufferSetTriggerLevel( StreamBufferHandle_t xStreamBuffer,
                                             size_t xTriggerLevel ) FREERTOS_SYSTEM_CALL;
size_t MPU_xStreamBufferNextMessageLengthBytes( StreamBufferHandle_t xStreamBuffer ) FREERTOS_SYSTEM_CALL;

/* Privileged only wrappers for Stream Buffer APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
StreamBufferHandle_t MPU_xStreamBufferGenericCreate( size_t xBufferSizeBytes,
                                                     size_t xTriggerLevelBytes,
                                                     BaseType_t xStreamBufferType,
                                                     StreamBufferCallbackFunction_t pxSendCompletedCallback,
                                                     StreamBufferCallbackFunction_t pxReceiveCompletedCallback ) PRIVILEGED_FUNCTION;
StreamBufferHandle_t MPU_xStreamBufferGenericCreateStatic( size_t xBufferSizeBytes,
                                                           size_t xTriggerLevelBytes,
                                                           BaseType_t xStreamBufferType,
                                                           uint8_t * மாறிலி pucStreamBufferStorageArea,
                                                           StaticStreamBuffer_t * மாறிலி pxStaticStreamBuffer,
                                                           StreamBufferCallbackFunction_t pxSendCompletedCallback,
                                                           StreamBufferCallbackFunction_t pxReceiveCompletedCallback ) PRIVILEGED_FUNCTION;
வெறுமை MPU_vStreamBufferDelete( StreamBufferHandle_t xStreamBuffer ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xStreamBufferReset( StreamBufferHandle_t xStreamBuffer ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xStreamBufferGetStaticBuffers( StreamBufferHandle_t xStreamBuffers,
                                              uint8_t * ppucStreamBufferStorageArea,
                                              StaticStreamBuffer_t * ppxStaticStreamBuffer ) PRIVILEGED_FUNCTION;
size_t MPU_xStreamBufferSendFromISR( StreamBufferHandle_t xStreamBuffer,
                                     மாறிலி வெறுமை * pvTxData,
                                     size_t xDataLengthBytes,
                                     BaseType_t * மாறிலி pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;
size_t MPU_xStreamBufferReceiveFromISR( StreamBufferHandle_t xStreamBuffer,
                                        வெறுமை * pvRxData,
                                        size_t xBufferLengthBytes,
                                        BaseType_t * மாறிலி pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xStreamBufferSendCompletedFromISR( StreamBufferHandle_t xStreamBuffer,
                                                  BaseType_t * pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xStreamBufferReceiveCompletedFromISR( StreamBufferHandle_t xStreamBuffer,
                                                     BaseType_t * pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xStreamBufferResetFromISR( StreamBufferHandle_t xStreamBuffer ) PRIVILEGED_FUNCTION;

#endif /* MPU_PROTOTYPES_H */
