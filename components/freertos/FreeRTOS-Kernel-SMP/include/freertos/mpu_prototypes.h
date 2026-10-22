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

প্রকারলেখ কাঠামো xTaskGenericNotifyParams
{
    TaskHandle_t xTaskToNotify;
    UBaseType_t ux_সূচক_To_Notify;
    uint32_t ul_মান;
    eNotifyAction eAction;
    uint32_t * pul_Previous_Notification_মান;
} xTaskGenericNotifyParams_t;

প্রকারলেখ কাঠামো xTaskGenericNotifyWaitParams
{
    UBaseType_t ux_সূচক_To_Wait_On;
    uint32_t ulBitsToClearOnEntry;
    uint32_t ulBitsToClearOnExit;
    uint32_t * pul_Notification_মান;
    TickType_t xTicksToWait;
} xTaskGenericNotifyWaitParams_t;

প্রকারলেখ কাঠামো xTimerGenericCommandFromTaskParams
{
    TimerHandle_t xTimer;
    BaseType_t xCommandID;
    TickType_t x_Optional_মান;
    BaseType_t * pxHigherPriorityTaskWoken;
    TickType_t xTicksToWait;
} xTimerGenericCommandFromTaskParams_t;

প্রকারলেখ কাঠামো xEventGroupWaitBitsParams
{
    EventGroupHandle_t xEventGroup;
    EventBits_t uxBitsToWaitFor;
    BaseType_t xClearOnExit;
    BaseType_t xWaitForAllBits;
    TickType_t xTicksToWait;
} xEventGroupWaitBitsParams_t;

/* MPU versions of task.h API functions. */
শূন্য MPU_vTaskDelay( ধ্রুবক TickType_t xTicksToDelay ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskDelayUntil( TickType_t * ধ্রুবক pxPreviousWakeTime,
                                ধ্রুবক TickType_t xTimeIncrement ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskAbortDelay( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxTaskPriorityGet( ধ্রুবক TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
eTaskState MPU_eTaskGetState( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
শূন্য MPU_vTaskGetInfo( TaskHandle_t xTask,
                       TaskStatus_t * pxTaskStatus,
                       BaseType_t x_Get_Free_স্তূপ_Space,
                       eTaskState eState ) FREERTOS_SYSTEM_CALL;
শূন্য MPU_vTaskSuspend( TaskHandle_t xTaskToSuspend ) FREERTOS_SYSTEM_CALL;
শূন্য MPU_vTaskResume( TaskHandle_t xTaskToResume ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_x_Task_Get_Tick_গুনতি( শূন্য ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_Get_সংখ্যা_Of_Tasks( শূন্য ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_Get_স্তূপ_উপরের_Water_Mark( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
config_স্তূপ_DEPTH_TYPE MPU_ux_Task_Get_স্তূপ_উপরের_Water_Mark2( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
শূন্য MPU_vTaskSetApplicationTaskTag( TaskHandle_t xTask,
                                     TaskHookFunction_t pxHookFunction ) FREERTOS_SYSTEM_CALL;
TaskHookFunction_t MPU_xTaskGetApplicationTaskTag( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
শূন্য MPU_vTaskSetThreadLocalStoragePointer( TaskHandle_t xTaskToSet,
                                            BaseType_t x_সূচক,
                                            শূন্য * pv_মান ) FREERTOS_SYSTEM_CALL;
শূন্য * MPU_pvTaskGetThreadLocalStoragePointer( TaskHandle_t xTaskToQuery,
                                               BaseType_t x_সূচক ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetIdleTaskHandle( শূন্য ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxTaskGetSystemState( TaskStatus_t * ধ্রুবক pxTaskStatusArray,
                                      ধ্রুবক UBaseType_t ux_Array_আকার,
                                      config_RUN_TIME_গণক_TYPE * ধ্রুবক pul_যোগ_Run_Time ) FREERTOS_SYSTEM_CALL;
config_RUN_TIME_গণক_TYPE MPU_ul_Task_Get_Run_Time_গণক( ধ্রুবক TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
config_RUN_TIME_গণক_TYPE MPU_ulTaskGetRunTimePercent( ধ্রুবক TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
config_RUN_TIME_গণক_TYPE MPU_ul_Task_Get_Idle_Run_Time_গণক( শূন্য ) FREERTOS_SYSTEM_CALL;
config_RUN_TIME_গণক_TYPE MPU_ulTaskGetIdleRunTimePercent( শূন্য ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGenericNotify( TaskHandle_t xTaskToNotify,
                                   UBaseType_t ux_সূচক_To_Notify,
                                   uint32_t ul_মান,
                                   eNotifyAction eAction,
                                   uint32_t * pul_Previous_Notification_মান ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGenericNotifyEntry( ধ্রুবক xTaskGenericNotifyParams_t * pxParams ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGenericNotifyWait( UBaseType_t ux_সূচক_To_Wait_On,
                                       uint32_t ulBitsToClearOnEntry,
                                       uint32_t ulBitsToClearOnExit,
                                       uint32_t * pul_Notification_মান,
                                       TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGenericNotifyWaitEntry( ধ্রুবক xTaskGenericNotifyWaitParams_t * pxParams ) FREERTOS_SYSTEM_CALL;
uint32_t MPU_ulTaskGenericNotifyTake( UBaseType_t ux_সূচক_To_Wait_On,
                                      BaseType_t x_Clear_গুনতি_On_Exit,
                                      TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGenericNotifyStateClear( TaskHandle_t xTask,
                                             UBaseType_t ux_সূচক_To_Clear ) FREERTOS_SYSTEM_CALL;
uint32_t MPU_ul_Task_Generic_Notify_মান_Clear( TaskHandle_t xTask,
                                            UBaseType_t ux_সূচক_To_Clear,
                                            uint32_t ulBitsToClear ) FREERTOS_SYSTEM_CALL;
শূন্য MPU_vTaskSetTimeOutState( TimeOut_t * ধ্রুবক pxTimeOut ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskCheckForTimeOut( TimeOut_t * ধ্রুবক pxTimeOut,
                                     TickType_t * ধ্রুবক pxTicksToWait ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetCurrentTaskHandle( শূন্য ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGetSchedulerState( শূন্য ) FREERTOS_SYSTEM_CALL;

/* Privileged only wrappers for Task APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
BaseType_t MPU_xTaskCreate( TaskFunction_t pxTaskCode,
                            ধ্রুবক অক্ষর * ধ্রুবক pcName,
                            ধ্রুবক config_স্তূপ_DEPTH_TYPE ux_স্তূপ_Depth,
                            শূন্য * ধ্রুবক pvParameters,
                            UBaseType_t uxPriority,
                            TaskHandle_t * ধ্রুবক pxCreatedTask ) PRIVILEGED_FUNCTION;
TaskHandle_t MPU_xTaskCreateStatic( TaskFunction_t pxTaskCode,
                                    ধ্রুবক অক্ষর * ধ্রুবক pcName,
                                    ধ্রুবক config_স্তূপ_DEPTH_TYPE ux_স্তূপ_Depth,
                                    শূন্য * ধ্রুবক pvParameters,
                                    UBaseType_t uxPriority,
                                    স্তূপ_Type_t * ধ্রুবক pux_স্তূপ_বাফার,
                                    StaticTask_t * ধ্রুবক px_Task_বাফার ) PRIVILEGED_FUNCTION;
শূন্য MPU_vTaskDelete( TaskHandle_t xTaskToDelete ) PRIVILEGED_FUNCTION;
শূন্য MPU_vTaskPrioritySet( TaskHandle_t xTask,
                           UBaseType_t uxNewPriority ) PRIVILEGED_FUNCTION;
TaskHandle_t MPU_xTaskGetHandle( ধ্রুবক অক্ষর * pcNameToQuery ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xTaskCallApplicationTaskHook( TaskHandle_t xTask,
                                             শূন্য * pvParameter ) PRIVILEGED_FUNCTION;
অক্ষর * MPU_pcTaskGetName( TaskHandle_t xTaskToQuery ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xTaskCreateRestricted( ধ্রুবক TaskParameters_t * ধ্রুবক pxTaskDefinition,
                                      TaskHandle_t * pxCreatedTask ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xTaskCreateRestrictedStatic( ধ্রুবক TaskParameters_t * ধ্রুবক pxTaskDefinition,
                                            TaskHandle_t * pxCreatedTask ) PRIVILEGED_FUNCTION;
শূন্য MPU_vTaskAllocateMPURegions( TaskHandle_t xTaskToModify,
                                  ধ্রুবক MemoryRegion_t * ধ্রুবক xRegions ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xTaskGetStaticBuffers( TaskHandle_t xTask,
                                      স্তূপ_Type_t ** ppux_স্তূপ_বাফার,
                                      StaticTask_t ** ppx_Task_বাফার ) PRIVILEGED_FUNCTION;
UBaseType_t MPU_uxTaskPriorityGetFromISR( ধ্রুবক TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
UBaseType_t MPU_uxTaskBasePriorityGet( ধ্রুবক TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
UBaseType_t MPU_uxTaskBasePriorityGetFromISR( ধ্রুবক TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xTaskResumeFromISR( TaskHandle_t xTaskToResume ) PRIVILEGED_FUNCTION;
TaskHookFunction_t MPU_xTaskGetApplicationTaskTagFromISR( TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xTaskGenericNotifyFromISR( TaskHandle_t xTaskToNotify,
                                          UBaseType_t ux_সূচক_To_Notify,
                                          uint32_t ul_মান,
                                          eNotifyAction eAction,
                                          uint32_t * pul_Previous_Notification_মান,
                                          BaseType_t * pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;
শূন্য MPU_vTaskGenericNotifyGiveFromISR( TaskHandle_t xTaskToNotify,
                                        UBaseType_t ux_সূচক_To_Notify,
                                        BaseType_t * pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;

/* MPU versions of queue.h API functions. */
BaseType_t MPU_xQueueGenericSend( QueueHandle_t xQueue,
                                  ধ্রুবক শূন্য * ধ্রুবক pvItemToQueue,
                                  TickType_t xTicksToWait,
                                  ধ্রুবক BaseType_t xCopyPosition ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueReceive( QueueHandle_t xQueue,
                              শূন্য * ধ্রুবক pv_বাফার,
                              TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueuePeek( QueueHandle_t xQueue,
                           শূন্য * ধ্রুবক pv_বাফার,
                           TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueSemaphoreTake( QueueHandle_t xQueue,
                                    TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxQueueMessagesWaiting( ধ্রুবক QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxQueueSpacesAvailable( ধ্রুবক QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xQueueGetMutexHolder( QueueHandle_t xSemaphore ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueTakeMutexRecursive( QueueHandle_t xMutex,
                                         TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueGiveMutexRecursive( QueueHandle_t pxMutex ) FREERTOS_SYSTEM_CALL;
শূন্য MPU_vQueueAddToRegistry( QueueHandle_t xQueue,
                              ধ্রুবক অক্ষর * pcName ) FREERTOS_SYSTEM_CALL;
শূন্য MPU_vQueueUnregisterQueue( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
ধ্রুবক অক্ষর * MPU_pcQueueGetName( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueAddToSet( QueueSetMemberHandle_t xQueueOrSemaphore,
                               QueueSetHandle_t xQueueSet ) FREERTOS_SYSTEM_CALL;
QueueSetMemberHandle_t MPU_xQueueSelectFromSet( QueueSetHandle_t xQueueSet,
                                                ধ্রুবক TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
শূন্য MPU_v_Queue_Set_Queue_সংখ্যা( QueueHandle_t xQueue,
                               UBaseType_t ux_Queue_সংখ্যা ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Queue_Get_Queue_সংখ্যা( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
uint8_t MPU_ucQueueGetQueueType( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;

/* Privileged only wrappers for Queue APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
শূন্য MPU_vQueueDelete( QueueHandle_t xQueue ) PRIVILEGED_FUNCTION;
QueueHandle_t MPU_xQueueCreateMutex( ধ্রুবক uint8_t ucQueueType ) PRIVILEGED_FUNCTION;
QueueHandle_t MPU_xQueueCreateMutexStatic( ধ্রুবক uint8_t ucQueueType,
                                           StaticQueue_t * pxStaticQueue ) PRIVILEGED_FUNCTION;
QueueHandle_t MPU_xQueueCreateCountingSemaphore( ধ্রুবক UBaseType_t ux_Max_গুনতি,
                                                 ধ্রুবক UBaseType_t ux_Initial_গুনতি ) PRIVILEGED_FUNCTION;
QueueHandle_t MPU_xQueueCreateCountingSemaphoreStatic( ধ্রুবক UBaseType_t ux_Max_গুনতি,
                                                       ধ্রুবক UBaseType_t ux_Initial_গুনতি,
                                                       StaticQueue_t * pxStaticQueue ) PRIVILEGED_FUNCTION;
QueueHandle_t MPU_xQueueGenericCreate( ধ্রুবক UBaseType_t ux_Queue_দৈর্ঘ্য,
                                       ধ্রুবক UBaseType_t ux_Item_আকার,
                                       ধ্রুবক uint8_t ucQueueType ) PRIVILEGED_FUNCTION;
QueueHandle_t MPU_xQueueGenericCreateStatic( ধ্রুবক UBaseType_t ux_Queue_দৈর্ঘ্য,
                                             ধ্রুবক UBaseType_t ux_Item_আকার,
                                             uint8_t * pucQueueStorage,
                                             StaticQueue_t * pxStaticQueue,
                                             ধ্রুবক uint8_t ucQueueType ) PRIVILEGED_FUNCTION;
QueueSetHandle_t MPU_xQueueCreateSet( ধ্রুবক UBaseType_t ux_Event_Queue_দৈর্ঘ্য ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xQueueRemoveFromSet( QueueSetMemberHandle_t xQueueOrSemaphore,
                                    QueueSetHandle_t xQueueSet ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xQueueGenericReset( QueueHandle_t xQueue,
                                   BaseType_t xNewQueue ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xQueueGenericGetStaticBuffers( QueueHandle_t xQueue,
                                              uint8_t ** ppucQueueStorage,
                                              StaticQueue_t ** ppxStaticQueue ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xQueueGenericSendFromISR( QueueHandle_t xQueue,
                                         ধ্রুবক শূন্য * ধ্রুবক pvItemToQueue,
                                         BaseType_t * ধ্রুবক pxHigherPriorityTaskWoken,
                                         ধ্রুবক BaseType_t xCopyPosition ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xQueueGiveFromISR( QueueHandle_t xQueue,
                                  BaseType_t * ধ্রুবক pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xQueuePeekFromISR( QueueHandle_t xQueue,
                                  শূন্য * ধ্রুবক pv_বাফার ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xQueueReceiveFromISR( QueueHandle_t xQueue,
                                     শূন্য * ধ্রুবক pv_বাফার,
                                     BaseType_t * ধ্রুবক pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xQueueIsQueueEmptyFromISR( ধ্রুবক QueueHandle_t xQueue ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xQueueIsQueueFullFromISR( ধ্রুবক QueueHandle_t xQueue ) PRIVILEGED_FUNCTION;
UBaseType_t MPU_uxQueueMessagesWaitingFromISR( ধ্রুবক QueueHandle_t xQueue ) PRIVILEGED_FUNCTION;
TaskHandle_t MPU_xQueueGetMutexHolderFromISR( QueueHandle_t xSemaphore ) PRIVILEGED_FUNCTION;
QueueSetMemberHandle_t MPU_xQueueSelectFromSetFromISR( QueueSetHandle_t xQueueSet ) PRIVILEGED_FUNCTION;

/* MPU versions of timers.h API functions. */
শূন্য * MPU_pvTimerGetTimerID( ধ্রুবক TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
শূন্য MPU_vTimerSetTimerID( TimerHandle_t xTimer,
                           শূন্য * pvNewID ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerIsTimerActive( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTimerGetTimerDaemonTaskHandle( শূন্য ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerGenericCommandFromTask( TimerHandle_t xTimer,
                                             ধ্রুবক BaseType_t xCommandID,
                                             ধ্রুবক TickType_t x_Optional_মান,
                                             BaseType_t * ধ্রুবক pxHigherPriorityTaskWoken,
                                             ধ্রুবক TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerGenericCommandFromTaskEntry( ধ্রুবক xTimerGenericCommandFromTaskParams_t * pxParams ) FREERTOS_SYSTEM_CALL;
ধ্রুবক অক্ষর * MPU_pcTimerGetName( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
শূন্য MPU_vTimerSetReloadMode( TimerHandle_t xTimer,
                              ধ্রুবক BaseType_t uxAutoReload ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerGetReloadMode( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxTimerGetReloadMode( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_xTimerGetPeriod( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_xTimerGetExpiryTime( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;

/* Privileged only wrappers for Timer APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
TimerHandle_t MPU_xTimerCreate( ধ্রুবক অক্ষর * ধ্রুবক pcTimerName,
                                ধ্রুবক TickType_t xTimerPeriodInTicks,
                                ধ্রুবক UBaseType_t uxAutoReload,
                                শূন্য * ধ্রুবক pvTimerID,
                                TimerCallbackFunction_t pxCallbackFunction ) PRIVILEGED_FUNCTION;
TimerHandle_t MPU_xTimerCreateStatic( ধ্রুবক অক্ষর * ধ্রুবক pcTimerName,
                                      ধ্রুবক TickType_t xTimerPeriodInTicks,
                                      ধ্রুবক UBaseType_t uxAutoReload,
                                      শূন্য * ধ্রুবক pvTimerID,
                                      TimerCallbackFunction_t pxCallbackFunction,
                                      StaticTimer_t * px_Timer_বাফার ) PRIVILEGED_FUNCTION;
BaseType_t MPU_x_Timer_Get_Static_বাফার( TimerHandle_t xTimer,
                                      StaticTimer_t ** ppx_Timer_বাফার ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xTimerGenericCommandFromISR( TimerHandle_t xTimer,
                                            ধ্রুবক BaseType_t xCommandID,
                                            ধ্রুবক TickType_t x_Optional_মান,
                                            BaseType_t * ধ্রুবক pxHigherPriorityTaskWoken,
                                            ধ্রুবক TickType_t xTicksToWait ) PRIVILEGED_FUNCTION;

/* MPU versions of event_group.h API functions. */
EventBits_t MPU_xEventGroupWaitBits( EventGroupHandle_t xEventGroup,
                                     ধ্রুবক EventBits_t uxBitsToWaitFor,
                                     ধ্রুবক BaseType_t xClearOnExit,
                                     ধ্রুবক BaseType_t xWaitForAllBits,
                                     TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupWaitBitsEntry( ধ্রুবক xEventGroupWaitBitsParams_t * pxParams ) FREERTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupClearBits( EventGroupHandle_t xEventGroup,
                                      ধ্রুবক EventBits_t uxBitsToClear ) FREERTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupSetBits( EventGroupHandle_t xEventGroup,
                                    ধ্রুবক EventBits_t uxBitsToSet ) FREERTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupSync( EventGroupHandle_t xEventGroup,
                                 ধ্রুবক EventBits_t uxBitsToSet,
                                 ধ্রুবক EventBits_t uxBitsToWaitFor,
                                 TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
#যদি ( configUSE_TRACE_FACILITY == 1 )
    UBaseType_t MPU_ux_Event_Group_Get_সংখ্যা( শূন্য * xEventGroup ) FREERTOS_SYSTEM_CALL;
    শূন্য MPU_v_Event_Group_Set_সংখ্যা( শূন্য * xEventGroup,
                                   UBaseType_t ux_Event_Group_সংখ্যা ) FREERTOS_SYSTEM_CALL;
#endif /* ( configUSE_TRACE_FACILITY == 1 )*/

/* Privileged only wrappers for Event Group APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
EventGroupHandle_t MPU_xEventGroupCreate( শূন্য ) PRIVILEGED_FUNCTION;
EventGroupHandle_t MPU_xEventGroupCreateStatic( StaticEventGroup_t * px_Event_Group_বাফার ) PRIVILEGED_FUNCTION;
শূন্য MPU_vEventGroupDelete( EventGroupHandle_t xEventGroup ) PRIVILEGED_FUNCTION;
BaseType_t MPU_x_Event_Group_Get_Static_বাফার( EventGroupHandle_t xEventGroup,
                                           StaticEventGroup_t ** ppx_Event_Group_বাফার ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xEventGroupClearBitsFromISR( EventGroupHandle_t xEventGroup,
                                            ধ্রুবক EventBits_t uxBitsToClear ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xEventGroupSetBitsFromISR( EventGroupHandle_t xEventGroup,
                                          ধ্রুবক EventBits_t uxBitsToSet,
                                          BaseType_t * pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;
EventBits_t MPU_xEventGroupGetBitsFromISR( EventGroupHandle_t xEventGroup ) PRIVILEGED_FUNCTION;

/* MPU versions of message/stream_buffer.h API functions. */
size_t MPU_x_Stream_বাফার_Send( Stream_বাফার_Handle_t x_Stream_বাফার,
                              ধ্রুবক শূন্য * pv_Tx_তথ্য,
                              size_t x_তথ্য_দৈর্ঘ্য_Bytes,
                              TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_Stream_বাফার_Receive( Stream_বাফার_Handle_t x_Stream_বাফার,
                                 শূন্য * pv_Rx_তথ্য,
                                 size_t x_বাফার_দৈর্ঘ্য_Bytes,
                                 TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_x_Stream_বাফার_Is_Full( Stream_বাফার_Handle_t x_Stream_বাফার ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_x_Stream_বাফার_Is_Empty( Stream_বাফার_Handle_t x_Stream_বাফার ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_Stream_বাফার_Spaces_Available( Stream_বাফার_Handle_t x_Stream_বাফার ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_Stream_বাফার_Bytes_Available( Stream_বাফার_Handle_t x_Stream_বাফার ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_x_Stream_বাফার_Set_Trigger_Level( Stream_বাফার_Handle_t x_Stream_বাফার,
                                             size_t xTriggerLevel ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_Stream_বাফার_পরবর্তী_Message_দৈর্ঘ্য_Bytes( Stream_বাফার_Handle_t x_Stream_বাফার ) FREERTOS_SYSTEM_CALL;

/* Privileged only wrappers for Stream Buffer APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
Stream_বাফার_Handle_t MPU_x_Stream_বাফার_Generic_Create( size_t x_বাফার_আকার_Bytes,
                                                     size_t xTriggerLevelBytes,
                                                     BaseType_t x_Stream_বাফার_Type,
                                                     Stream_বাফার_Callback_Function_t pxSendCompletedCallback,
                                                     Stream_বাফার_Callback_Function_t pxReceiveCompletedCallback ) PRIVILEGED_FUNCTION;
Stream_বাফার_Handle_t MPU_x_Stream_বাফার_Generic_Create_Static( size_t x_বাফার_আকার_Bytes,
                                                           size_t xTriggerLevelBytes,
                                                           BaseType_t x_Stream_বাফার_Type,
                                                           uint8_t * ধ্রুবক puc_Stream_বাফার_Storage_Area,
                                                           Static_Stream_বাফার_t * ধ্রুবক px_Static_Stream_বাফার,
                                                           Stream_বাফার_Callback_Function_t pxSendCompletedCallback,
                                                           Stream_বাফার_Callback_Function_t pxReceiveCompletedCallback ) PRIVILEGED_FUNCTION;
শূন্য MPU_v_Stream_বাফার_Delete( Stream_বাফার_Handle_t x_Stream_বাফার ) PRIVILEGED_FUNCTION;
BaseType_t MPU_x_Stream_বাফার_Reset( Stream_বাফার_Handle_t x_Stream_বাফার ) PRIVILEGED_FUNCTION;
BaseType_t MPU_x_Stream_বাফার_Get_Static_Buffers( Stream_বাফার_Handle_t xStreamBuffers,
                                              uint8_t * ppuc_Stream_বাফার_Storage_Area,
                                              Static_Stream_বাফার_t * ppx_Static_Stream_বাফার ) PRIVILEGED_FUNCTION;
size_t MPU_x_Stream_বাফার_Send_From_ISR( Stream_বাফার_Handle_t x_Stream_বাফার,
                                     ধ্রুবক শূন্য * pv_Tx_তথ্য,
                                     size_t x_তথ্য_দৈর্ঘ্য_Bytes,
                                     BaseType_t * ধ্রুবক pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;
size_t MPU_x_Stream_বাফার_Receive_From_ISR( Stream_বাফার_Handle_t x_Stream_বাফার,
                                        শূন্য * pv_Rx_তথ্য,
                                        size_t x_বাফার_দৈর্ঘ্য_Bytes,
                                        BaseType_t * ধ্রুবক pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;
BaseType_t MPU_x_Stream_বাফার_Send_Completed_From_ISR( Stream_বাফার_Handle_t x_Stream_বাফার,
                                                  BaseType_t * pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;
BaseType_t MPU_x_Stream_বাফার_Receive_Completed_From_ISR( Stream_বাফার_Handle_t x_Stream_বাফার,
                                                     BaseType_t * pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;
BaseType_t MPU_x_Stream_বাফার_Reset_From_ISR( Stream_বাফার_Handle_t x_Stream_বাফার ) PRIVILEGED_FUNCTION;

#endif /* MPU_PROTOTYPES_H */
