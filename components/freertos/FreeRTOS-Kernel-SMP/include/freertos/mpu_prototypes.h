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

प्रकारनिर्देश संरचना xTaskGenericNotifyParams
{
    TaskHandle_t xTaskToNotify;
    UBaseType_t uxIndexToNotify;
    uint32_t ul_मान;
    eNotifyAction eAction;
    uint32_t * pul_Previous_Notification_मान;
} xTaskGenericNotifyParams_t;

प्रकारनिर्देश संरचना xTaskGenericNotifyWaitParams
{
    UBaseType_t uxIndexToWaitOn;
    uint32_t ulBitsToClearOnEntry;
    uint32_t ulBitsToClearOnExit;
    uint32_t * pul_Notification_मान;
    TickType_t xTicksToWait;
} xTaskGenericNotifyWaitParams_t;

प्रकारनिर्देश संरचना xTimerGenericCommandFromTaskParams
{
    TimerHandle_t xTimer;
    BaseType_t xCommandID;
    TickType_t x_Optional_मान;
    BaseType_t * pxHigherPriorityTaskWoken;
    TickType_t xTicksToWait;
} xTimerGenericCommandFromTaskParams_t;

प्रकारनिर्देश संरचना xEventGroupWaitBitsParams
{
    EventGroupHandle_t xEventGroup;
    EventBits_t uxBitsToWaitFor;
    BaseType_t xClearOnExit;
    BaseType_t xWaitForAllBits;
    TickType_t xTicksToWait;
} xEventGroupWaitBitsParams_t;

/* MPU versions of task.h API functions. */
शून्य MPU_vTaskDelay( अचर TickType_t xTicksToDelay ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskDelayUntil( TickType_t * अचर pxPreviousWakeTime,
                                अचर TickType_t xTimeIncrement ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskAbortDelay( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxTaskPriorityGet( अचर TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
eTaskState MPU_eTaskGetState( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
शून्य MPU_vTaskGetInfo( TaskHandle_t xTask,
                       TaskStatus_t * pxTaskStatus,
                       BaseType_t x_Get_Free_थाक_Space,
                       eTaskState eState ) FREERTOS_SYSTEM_CALL;
शून्य MPU_vTaskSuspend( TaskHandle_t xTaskToSuspend ) FREERTOS_SYSTEM_CALL;
शून्य MPU_vTaskResume( TaskHandle_t xTaskToResume ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_x_Task_Get_Tick_गन्ती( शून्य ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_Get_सङ्ख्या_Of_Tasks( शून्य ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_Get_थाक_माथिल्लो_Water_Mark( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
config_थाक_DEPTH_TYPE MPU_ux_Task_Get_थाक_माथिल्लो_Water_Mark2( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
शून्य MPU_vTaskSetApplicationTaskTag( TaskHandle_t xTask,
                                     TaskHookFunction_t pxHookFunction ) FREERTOS_SYSTEM_CALL;
TaskHookFunction_t MPU_xTaskGetApplicationTaskTag( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
शून्य MPU_vTaskSetThreadLocalStoragePointer( TaskHandle_t xTaskToSet,
                                            BaseType_t xIndex,
                                            शून्य * pv_मान ) FREERTOS_SYSTEM_CALL;
शून्य * MPU_pvTaskGetThreadLocalStoragePointer( TaskHandle_t xTaskToQuery,
                                               BaseType_t xIndex ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetIdleTaskHandle( शून्य ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxTaskGetSystemState( TaskStatus_t * अचर pxTaskStatusArray,
                                      अचर UBaseType_t uxArraySize,
                                      configRUN_TIME_COUNTER_TYPE * अचर pulTotalRunTime ) FREERTOS_SYSTEM_CALL;
configRUN_TIME_COUNTER_TYPE MPU_ulTaskGetRunTimeCounter( अचर TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
configRUN_TIME_COUNTER_TYPE MPU_ulTaskGetRunTimePercent( अचर TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
configRUN_TIME_COUNTER_TYPE MPU_ulTaskGetIdleRunTimeCounter( शून्य ) FREERTOS_SYSTEM_CALL;
configRUN_TIME_COUNTER_TYPE MPU_ulTaskGetIdleRunTimePercent( शून्य ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGenericNotify( TaskHandle_t xTaskToNotify,
                                   UBaseType_t uxIndexToNotify,
                                   uint32_t ul_मान,
                                   eNotifyAction eAction,
                                   uint32_t * pul_Previous_Notification_मान ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGenericNotifyEntry( अचर xTaskGenericNotifyParams_t * pxParams ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGenericNotifyWait( UBaseType_t uxIndexToWaitOn,
                                       uint32_t ulBitsToClearOnEntry,
                                       uint32_t ulBitsToClearOnExit,
                                       uint32_t * pul_Notification_मान,
                                       TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGenericNotifyWaitEntry( अचर xTaskGenericNotifyWaitParams_t * pxParams ) FREERTOS_SYSTEM_CALL;
uint32_t MPU_ulTaskGenericNotifyTake( UBaseType_t uxIndexToWaitOn,
                                      BaseType_t x_Clear_गन्ती_On_Exit,
                                      TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGenericNotifyStateClear( TaskHandle_t xTask,
                                             UBaseType_t uxIndexToClear ) FREERTOS_SYSTEM_CALL;
uint32_t MPU_ul_Task_Generic_Notify_मान_Clear( TaskHandle_t xTask,
                                            UBaseType_t uxIndexToClear,
                                            uint32_t ulBitsToClear ) FREERTOS_SYSTEM_CALL;
शून्य MPU_vTaskSetTimeOutState( TimeOut_t * अचर pxTimeOut ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskCheckForTimeOut( TimeOut_t * अचर pxTimeOut,
                                     TickType_t * अचर pxTicksToWait ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetCurrentTaskHandle( शून्य ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGetSchedulerState( शून्य ) FREERTOS_SYSTEM_CALL;

/* Privileged only wrappers for Task APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
BaseType_t MPU_xTaskCreate( TaskFunction_t pxTaskCode,
                            अचर वर्ण * अचर pcName,
                            अचर config_थाक_DEPTH_TYPE ux_थाक_Depth,
                            शून्य * अचर pvParameters,
                            UBaseType_t uxPriority,
                            TaskHandle_t * अचर pxCreatedTask ) PRIVILEGED_FUNCTION;
TaskHandle_t MPU_xTaskCreateStatic( TaskFunction_t pxTaskCode,
                                    अचर वर्ण * अचर pcName,
                                    अचर config_थाक_DEPTH_TYPE ux_थाक_Depth,
                                    शून्य * अचर pvParameters,
                                    UBaseType_t uxPriority,
                                    थाक_Type_t * अचर pux_थाक_Buffer,
                                    StaticTask_t * अचर pxTaskBuffer ) PRIVILEGED_FUNCTION;
शून्य MPU_vTaskDelete( TaskHandle_t xTaskToDelete ) PRIVILEGED_FUNCTION;
शून्य MPU_vTaskPrioritySet( TaskHandle_t xTask,
                           UBaseType_t uxNewPriority ) PRIVILEGED_FUNCTION;
TaskHandle_t MPU_xTaskGetHandle( अचर वर्ण * pcNameToQuery ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xTaskCallApplicationTaskHook( TaskHandle_t xTask,
                                             शून्य * pvParameter ) PRIVILEGED_FUNCTION;
वर्ण * MPU_pcTaskGetName( TaskHandle_t xTaskToQuery ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xTaskCreateRestricted( अचर TaskParameters_t * अचर pxTaskDefinition,
                                      TaskHandle_t * pxCreatedTask ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xTaskCreateRestrictedStatic( अचर TaskParameters_t * अचर pxTaskDefinition,
                                            TaskHandle_t * pxCreatedTask ) PRIVILEGED_FUNCTION;
शून्य MPU_vTaskAllocateMPURegions( TaskHandle_t xTaskToModify,
                                  अचर MemoryRegion_t * अचर xRegions ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xTaskGetStaticBuffers( TaskHandle_t xTask,
                                      थाक_Type_t ** ppux_थाक_Buffer,
                                      StaticTask_t ** ppxTaskBuffer ) PRIVILEGED_FUNCTION;
UBaseType_t MPU_uxTaskPriorityGetFromISR( अचर TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
UBaseType_t MPU_uxTaskBasePriorityGet( अचर TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
UBaseType_t MPU_uxTaskBasePriorityGetFromISR( अचर TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xTaskResumeFromISR( TaskHandle_t xTaskToResume ) PRIVILEGED_FUNCTION;
TaskHookFunction_t MPU_xTaskGetApplicationTaskTagFromISR( TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xTaskGenericNotifyFromISR( TaskHandle_t xTaskToNotify,
                                          UBaseType_t uxIndexToNotify,
                                          uint32_t ul_मान,
                                          eNotifyAction eAction,
                                          uint32_t * pul_Previous_Notification_मान,
                                          BaseType_t * pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;
शून्य MPU_vTaskGenericNotifyGiveFromISR( TaskHandle_t xTaskToNotify,
                                        UBaseType_t uxIndexToNotify,
                                        BaseType_t * pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;

/* MPU versions of queue.h API functions. */
BaseType_t MPU_xQueueGenericSend( QueueHandle_t xQueue,
                                  अचर शून्य * अचर pvItemToQueue,
                                  TickType_t xTicksToWait,
                                  अचर BaseType_t xCopyPosition ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueReceive( QueueHandle_t xQueue,
                              शून्य * अचर pvBuffer,
                              TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueuePeek( QueueHandle_t xQueue,
                           शून्य * अचर pvBuffer,
                           TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueSemaphoreTake( QueueHandle_t xQueue,
                                    TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxQueueMessagesWaiting( अचर QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxQueueSpacesAvailable( अचर QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xQueueGetMutexHolder( QueueHandle_t xSemaphore ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueTakeMutexRecursive( QueueHandle_t xMutex,
                                         TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueGiveMutexRecursive( QueueHandle_t pxMutex ) FREERTOS_SYSTEM_CALL;
शून्य MPU_vQueueAddToRegistry( QueueHandle_t xQueue,
                              अचर वर्ण * pcName ) FREERTOS_SYSTEM_CALL;
शून्य MPU_vQueueUnregisterQueue( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
अचर वर्ण * MPU_pcQueueGetName( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueAddToSet( QueueSetMemberHandle_t xQueueOrSemaphore,
                               QueueSetHandle_t xQueueSet ) FREERTOS_SYSTEM_CALL;
QueueSetMemberHandle_t MPU_xQueueSelectFromSet( QueueSetHandle_t xQueueSet,
                                                अचर TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
शून्य MPU_v_Queue_Set_Queue_सङ्ख्या( QueueHandle_t xQueue,
                               UBaseType_t ux_Queue_सङ्ख्या ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Queue_Get_Queue_सङ्ख्या( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
uint8_t MPU_ucQueueGetQueueType( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;

/* Privileged only wrappers for Queue APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
शून्य MPU_vQueueDelete( QueueHandle_t xQueue ) PRIVILEGED_FUNCTION;
QueueHandle_t MPU_xQueueCreateMutex( अचर uint8_t ucQueueType ) PRIVILEGED_FUNCTION;
QueueHandle_t MPU_xQueueCreateMutexStatic( अचर uint8_t ucQueueType,
                                           StaticQueue_t * pxStaticQueue ) PRIVILEGED_FUNCTION;
QueueHandle_t MPU_xQueueCreateCountingSemaphore( अचर UBaseType_t ux_Max_गन्ती,
                                                 अचर UBaseType_t ux_Initial_गन्ती ) PRIVILEGED_FUNCTION;
QueueHandle_t MPU_xQueueCreateCountingSemaphoreStatic( अचर UBaseType_t ux_Max_गन्ती,
                                                       अचर UBaseType_t ux_Initial_गन्ती,
                                                       StaticQueue_t * pxStaticQueue ) PRIVILEGED_FUNCTION;
QueueHandle_t MPU_xQueueGenericCreate( अचर UBaseType_t uxQueueLength,
                                       अचर UBaseType_t uxItemSize,
                                       अचर uint8_t ucQueueType ) PRIVILEGED_FUNCTION;
QueueHandle_t MPU_xQueueGenericCreateStatic( अचर UBaseType_t uxQueueLength,
                                             अचर UBaseType_t uxItemSize,
                                             uint8_t * pucQueueStorage,
                                             StaticQueue_t * pxStaticQueue,
                                             अचर uint8_t ucQueueType ) PRIVILEGED_FUNCTION;
QueueSetHandle_t MPU_xQueueCreateSet( अचर UBaseType_t uxEventQueueLength ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xQueueRemoveFromSet( QueueSetMemberHandle_t xQueueOrSemaphore,
                                    QueueSetHandle_t xQueueSet ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xQueueGenericReset( QueueHandle_t xQueue,
                                   BaseType_t xNewQueue ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xQueueGenericGetStaticBuffers( QueueHandle_t xQueue,
                                              uint8_t ** ppucQueueStorage,
                                              StaticQueue_t ** ppxStaticQueue ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xQueueGenericSendFromISR( QueueHandle_t xQueue,
                                         अचर शून्य * अचर pvItemToQueue,
                                         BaseType_t * अचर pxHigherPriorityTaskWoken,
                                         अचर BaseType_t xCopyPosition ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xQueueGiveFromISR( QueueHandle_t xQueue,
                                  BaseType_t * अचर pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xQueuePeekFromISR( QueueHandle_t xQueue,
                                  शून्य * अचर pvBuffer ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xQueueReceiveFromISR( QueueHandle_t xQueue,
                                     शून्य * अचर pvBuffer,
                                     BaseType_t * अचर pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xQueueIsQueueEmptyFromISR( अचर QueueHandle_t xQueue ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xQueueIsQueueFullFromISR( अचर QueueHandle_t xQueue ) PRIVILEGED_FUNCTION;
UBaseType_t MPU_uxQueueMessagesWaitingFromISR( अचर QueueHandle_t xQueue ) PRIVILEGED_FUNCTION;
TaskHandle_t MPU_xQueueGetMutexHolderFromISR( QueueHandle_t xSemaphore ) PRIVILEGED_FUNCTION;
QueueSetMemberHandle_t MPU_xQueueSelectFromSetFromISR( QueueSetHandle_t xQueueSet ) PRIVILEGED_FUNCTION;

/* MPU versions of timers.h API functions. */
शून्य * MPU_pvTimerGetTimerID( अचर TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
शून्य MPU_vTimerSetTimerID( TimerHandle_t xTimer,
                           शून्य * pvNewID ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerIsTimerActive( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTimerGetTimerDaemonTaskHandle( शून्य ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerGenericCommandFromTask( TimerHandle_t xTimer,
                                             अचर BaseType_t xCommandID,
                                             अचर TickType_t x_Optional_मान,
                                             BaseType_t * अचर pxHigherPriorityTaskWoken,
                                             अचर TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerGenericCommandFromTaskEntry( अचर xTimerGenericCommandFromTaskParams_t * pxParams ) FREERTOS_SYSTEM_CALL;
अचर वर्ण * MPU_pcTimerGetName( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
शून्य MPU_vTimerSetReloadMode( TimerHandle_t xTimer,
                              अचर BaseType_t uxAutoReload ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerGetReloadMode( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxTimerGetReloadMode( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_xTimerGetPeriod( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_xTimerGetExpiryTime( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;

/* Privileged only wrappers for Timer APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
TimerHandle_t MPU_xTimerCreate( अचर वर्ण * अचर pcTimerName,
                                अचर TickType_t xTimerPeriodInTicks,
                                अचर UBaseType_t uxAutoReload,
                                शून्य * अचर pvTimerID,
                                TimerCallbackFunction_t pxCallbackFunction ) PRIVILEGED_FUNCTION;
TimerHandle_t MPU_xTimerCreateStatic( अचर वर्ण * अचर pcTimerName,
                                      अचर TickType_t xTimerPeriodInTicks,
                                      अचर UBaseType_t uxAutoReload,
                                      शून्य * अचर pvTimerID,
                                      TimerCallbackFunction_t pxCallbackFunction,
                                      StaticTimer_t * pxTimerBuffer ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xTimerGetStaticBuffer( TimerHandle_t xTimer,
                                      StaticTimer_t ** ppxTimerBuffer ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xTimerGenericCommandFromISR( TimerHandle_t xTimer,
                                            अचर BaseType_t xCommandID,
                                            अचर TickType_t x_Optional_मान,
                                            BaseType_t * अचर pxHigherPriorityTaskWoken,
                                            अचर TickType_t xTicksToWait ) PRIVILEGED_FUNCTION;

/* MPU versions of event_group.h API functions. */
EventBits_t MPU_xEventGroupWaitBits( EventGroupHandle_t xEventGroup,
                                     अचर EventBits_t uxBitsToWaitFor,
                                     अचर BaseType_t xClearOnExit,
                                     अचर BaseType_t xWaitForAllBits,
                                     TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupWaitBitsEntry( अचर xEventGroupWaitBitsParams_t * pxParams ) FREERTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupClearBits( EventGroupHandle_t xEventGroup,
                                      अचर EventBits_t uxBitsToClear ) FREERTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupSetBits( EventGroupHandle_t xEventGroup,
                                    अचर EventBits_t uxBitsToSet ) FREERTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupSync( EventGroupHandle_t xEventGroup,
                                 अचर EventBits_t uxBitsToSet,
                                 अचर EventBits_t uxBitsToWaitFor,
                                 TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
#यदि ( configUSE_TRACE_FACILITY == 1 )
    UBaseType_t MPU_ux_Event_Group_Get_सङ्ख्या( शून्य * xEventGroup ) FREERTOS_SYSTEM_CALL;
    शून्य MPU_v_Event_Group_Set_सङ्ख्या( शून्य * xEventGroup,
                                   UBaseType_t ux_Event_Group_सङ्ख्या ) FREERTOS_SYSTEM_CALL;
#endif /* ( configUSE_TRACE_FACILITY == 1 )*/

/* Privileged only wrappers for Event Group APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
EventGroupHandle_t MPU_xEventGroupCreate( शून्य ) PRIVILEGED_FUNCTION;
EventGroupHandle_t MPU_xEventGroupCreateStatic( StaticEventGroup_t * pxEventGroupBuffer ) PRIVILEGED_FUNCTION;
शून्य MPU_vEventGroupDelete( EventGroupHandle_t xEventGroup ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xEventGroupGetStaticBuffer( EventGroupHandle_t xEventGroup,
                                           StaticEventGroup_t ** ppxEventGroupBuffer ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xEventGroupClearBitsFromISR( EventGroupHandle_t xEventGroup,
                                            अचर EventBits_t uxBitsToClear ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xEventGroupSetBitsFromISR( EventGroupHandle_t xEventGroup,
                                          अचर EventBits_t uxBitsToSet,
                                          BaseType_t * pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;
EventBits_t MPU_xEventGroupGetBitsFromISR( EventGroupHandle_t xEventGroup ) PRIVILEGED_FUNCTION;

/* MPU versions of message/stream_buffer.h API functions. */
माप_प्रकार MPU_xStreamBufferSend( StreamBufferHandle_t xStreamBuffer,
                              अचर शून्य * pvTxData,
                              माप_प्रकार xDataLengthBytes,
                              TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
माप_प्रकार MPU_xStreamBufferReceive( StreamBufferHandle_t xStreamBuffer,
                                 शून्य * pvRxData,
                                 माप_प्रकार xBufferLengthBytes,
                                 TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xStreamBufferIsFull( StreamBufferHandle_t xStreamBuffer ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xStreamBufferIsEmpty( StreamBufferHandle_t xStreamBuffer ) FREERTOS_SYSTEM_CALL;
माप_प्रकार MPU_xStreamBufferSpacesAvailable( StreamBufferHandle_t xStreamBuffer ) FREERTOS_SYSTEM_CALL;
माप_प्रकार MPU_xStreamBufferBytesAvailable( StreamBufferHandle_t xStreamBuffer ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xStreamBufferSetTriggerLevel( StreamBufferHandle_t xStreamBuffer,
                                             माप_प्रकार xTriggerLevel ) FREERTOS_SYSTEM_CALL;
माप_प्रकार MPU_xStreamBufferNextMessageLengthBytes( StreamBufferHandle_t xStreamBuffer ) FREERTOS_SYSTEM_CALL;

/* Privileged only wrappers for Stream Buffer APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
StreamBufferHandle_t MPU_xStreamBufferGenericCreate( माप_प्रकार xBufferSizeBytes,
                                                     माप_प्रकार xTriggerLevelBytes,
                                                     BaseType_t xStreamBufferType,
                                                     StreamBufferCallbackFunction_t pxSendCompletedCallback,
                                                     StreamBufferCallbackFunction_t pxReceiveCompletedCallback ) PRIVILEGED_FUNCTION;
StreamBufferHandle_t MPU_xStreamBufferGenericCreateStatic( माप_प्रकार xBufferSizeBytes,
                                                           माप_प्रकार xTriggerLevelBytes,
                                                           BaseType_t xStreamBufferType,
                                                           uint8_t * अचर pucStreamBufferStorageArea,
                                                           StaticStreamBuffer_t * अचर pxStaticStreamBuffer,
                                                           StreamBufferCallbackFunction_t pxSendCompletedCallback,
                                                           StreamBufferCallbackFunction_t pxReceiveCompletedCallback ) PRIVILEGED_FUNCTION;
शून्य MPU_vStreamBufferDelete( StreamBufferHandle_t xStreamBuffer ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xStreamBufferReset( StreamBufferHandle_t xStreamBuffer ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xStreamBufferGetStaticBuffers( StreamBufferHandle_t xStreamBuffers,
                                              uint8_t * ppucStreamBufferStorageArea,
                                              StaticStreamBuffer_t * ppxStaticStreamBuffer ) PRIVILEGED_FUNCTION;
माप_प्रकार MPU_xStreamBufferSendFromISR( StreamBufferHandle_t xStreamBuffer,
                                     अचर शून्य * pvTxData,
                                     माप_प्रकार xDataLengthBytes,
                                     BaseType_t * अचर pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;
माप_प्रकार MPU_xStreamBufferReceiveFromISR( StreamBufferHandle_t xStreamBuffer,
                                        शून्य * pvRxData,
                                        माप_प्रकार xBufferLengthBytes,
                                        BaseType_t * अचर pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xStreamBufferSendCompletedFromISR( StreamBufferHandle_t xStreamBuffer,
                                                  BaseType_t * pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xStreamBufferReceiveCompletedFromISR( StreamBufferHandle_t xStreamBuffer,
                                                     BaseType_t * pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xStreamBufferResetFromISR( StreamBufferHandle_t xStreamBuffer ) PRIVILEGED_FUNCTION;

#endif /* MPU_PROTOTYPES_H */
