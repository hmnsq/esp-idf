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


#अगर_अघोषित MPU_PROTOTYPES_H
#घोषणा MPU_PROTOTYPES_H

प्रकार काष्ठा xTaskGenericNotifyParams
{
    TaskHandle_t xTaskToNotify;
    UBaseType_t ux_सूचकांक_To_Notify;
    uint32_t ul_मूल्य;
    eNotifyAction eAction;
    uint32_t * pul_Previous_Notification_मूल्य;
} xTaskGenericNotifyParams_t;

प्रकार काष्ठा xTaskGenericNotifyWaitParams
{
    UBaseType_t ux_सूचकांक_To_Wait_On;
    uint32_t ulBitsToClearOnEntry;
    uint32_t ulBitsToClearOnExit;
    uint32_t * pul_Notification_मूल्य;
    TickType_t xTicksToWait;
} xTaskGenericNotifyWaitParams_t;

प्रकार काष्ठा xTimerGenericCommandFromTaskParams
{
    TimerHandle_t xTimer;
    BaseType_t xCommandID;
    TickType_t x_Optional_मूल्य;
    BaseType_t * pxHigherPriorityTaskWoken;
    TickType_t xTicksToWait;
} xTimerGenericCommandFromTaskParams_t;

प्रकार काष्ठा xEventGroupWaitBitsParams
{
    EventGroupHandle_t xEventGroup;
    EventBits_t uxBitsToWaitFor;
    BaseType_t xClearOnExit;
    BaseType_t xWaitForAllBits;
    TickType_t xTicksToWait;
} xEventGroupWaitBitsParams_t;

/* MPU versions of task.h API functions. */
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
व्योम MPU_vTaskSuspend( TaskHandle_t xTaskToSuspend ) FREERTOS_SYSTEM_CALL;
व्योम MPU_vTaskResume( TaskHandle_t xTaskToResume ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_x_Task_Get_Tick_गिनती( व्योम ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_Get_संख्या_Of_Tasks( व्योम ) FREERTOS_SYSTEM_CALL;
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
TaskHandle_t MPU_xTaskGetIdleTaskHandle( व्योम ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxTaskGetSystemState( TaskStatus_t * स्थिर pxTaskStatusArray,
                                      स्थिर UBaseType_t ux_Array_आकार,
                                      config_RUN_TIME_गणक_TYPE * स्थिर pul_योग_Run_Time ) FREERTOS_SYSTEM_CALL;
config_RUN_TIME_गणक_TYPE MPU_ul_Task_Get_Run_Time_गणक( स्थिर TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
config_RUN_TIME_गणक_TYPE MPU_ulTaskGetRunTimePercent( स्थिर TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
config_RUN_TIME_गणक_TYPE MPU_ul_Task_Get_Idle_Run_Time_गणक( व्योम ) FREERTOS_SYSTEM_CALL;
config_RUN_TIME_गणक_TYPE MPU_ulTaskGetIdleRunTimePercent( व्योम ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGenericNotify( TaskHandle_t xTaskToNotify,
                                   UBaseType_t ux_सूचकांक_To_Notify,
                                   uint32_t ul_मूल्य,
                                   eNotifyAction eAction,
                                   uint32_t * pul_Previous_Notification_मूल्य ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGenericNotifyEntry( स्थिर xTaskGenericNotifyParams_t * pxParams ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGenericNotifyWait( UBaseType_t ux_सूचकांक_To_Wait_On,
                                       uint32_t ulBitsToClearOnEntry,
                                       uint32_t ulBitsToClearOnExit,
                                       uint32_t * pul_Notification_मूल्य,
                                       TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGenericNotifyWaitEntry( स्थिर xTaskGenericNotifyWaitParams_t * pxParams ) FREERTOS_SYSTEM_CALL;
uint32_t MPU_ulTaskGenericNotifyTake( UBaseType_t ux_सूचकांक_To_Wait_On,
                                      BaseType_t x_Clear_गिनती_On_Exit,
                                      TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGenericNotifyStateClear( TaskHandle_t xTask,
                                             UBaseType_t ux_सूचकांक_To_Clear ) FREERTOS_SYSTEM_CALL;
uint32_t MPU_ul_Task_Generic_Notify_मूल्य_Clear( TaskHandle_t xTask,
                                            UBaseType_t ux_सूचकांक_To_Clear,
                                            uint32_t ulBitsToClear ) FREERTOS_SYSTEM_CALL;
व्योम MPU_vTaskSetTimeOutState( TimeOut_t * स्थिर pxTimeOut ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskCheckForTimeOut( TimeOut_t * स्थिर pxTimeOut,
                                     TickType_t * स्थिर pxTicksToWait ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetCurrentTaskHandle( व्योम ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTaskGetSchedulerState( व्योम ) FREERTOS_SYSTEM_CALL;

/* Privileged only wrappers for Task APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
BaseType_t MPU_xTaskCreate( TaskFunction_t pxTaskCode,
                            स्थिर अक्षर * स्थिर pcName,
                            स्थिर config_ढेर_DEPTH_TYPE ux_ढेर_Depth,
                            व्योम * स्थिर pvParameters,
                            UBaseType_t uxPriority,
                            TaskHandle_t * स्थिर pxCreatedTask ) PRIVILEGED_FUNCTION;
TaskHandle_t MPU_xTaskCreateStatic( TaskFunction_t pxTaskCode,
                                    स्थिर अक्षर * स्थिर pcName,
                                    स्थिर config_ढेर_DEPTH_TYPE ux_ढेर_Depth,
                                    व्योम * स्थिर pvParameters,
                                    UBaseType_t uxPriority,
                                    ढेर_Type_t * स्थिर pux_ढेर_बफर,
                                    StaticTask_t * स्थिर px_Task_बफर ) PRIVILEGED_FUNCTION;
व्योम MPU_vTaskDelete( TaskHandle_t xTaskToDelete ) PRIVILEGED_FUNCTION;
व्योम MPU_vTaskPrioritySet( TaskHandle_t xTask,
                           UBaseType_t uxNewPriority ) PRIVILEGED_FUNCTION;
TaskHandle_t MPU_xTaskGetHandle( स्थिर अक्षर * pcNameToQuery ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xTaskCallApplicationTaskHook( TaskHandle_t xTask,
                                             व्योम * pvParameter ) PRIVILEGED_FUNCTION;
अक्षर * MPU_pcTaskGetName( TaskHandle_t xTaskToQuery ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xTaskCreateRestricted( स्थिर TaskParameters_t * स्थिर pxTaskDefinition,
                                      TaskHandle_t * pxCreatedTask ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xTaskCreateRestrictedStatic( स्थिर TaskParameters_t * स्थिर pxTaskDefinition,
                                            TaskHandle_t * pxCreatedTask ) PRIVILEGED_FUNCTION;
व्योम MPU_vTaskAllocateMPURegions( TaskHandle_t xTaskToModify,
                                  स्थिर MemoryRegion_t * स्थिर xRegions ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xTaskGetStaticBuffers( TaskHandle_t xTask,
                                      ढेर_Type_t ** ppux_ढेर_बफर,
                                      StaticTask_t ** ppx_Task_बफर ) PRIVILEGED_FUNCTION;
UBaseType_t MPU_uxTaskPriorityGetFromISR( स्थिर TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
UBaseType_t MPU_uxTaskBasePriorityGet( स्थिर TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
UBaseType_t MPU_uxTaskBasePriorityGetFromISR( स्थिर TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xTaskResumeFromISR( TaskHandle_t xTaskToResume ) PRIVILEGED_FUNCTION;
TaskHookFunction_t MPU_xTaskGetApplicationTaskTagFromISR( TaskHandle_t xTask ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xTaskGenericNotifyFromISR( TaskHandle_t xTaskToNotify,
                                          UBaseType_t ux_सूचकांक_To_Notify,
                                          uint32_t ul_मूल्य,
                                          eNotifyAction eAction,
                                          uint32_t * pul_Previous_Notification_मूल्य,
                                          BaseType_t * pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;
व्योम MPU_vTaskGenericNotifyGiveFromISR( TaskHandle_t xTaskToNotify,
                                        UBaseType_t ux_सूचकांक_To_Notify,
                                        BaseType_t * pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;

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
TaskHandle_t MPU_xQueueGetMutexHolder( QueueHandle_t xSemaphore ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueTakeMutexRecursive( QueueHandle_t xMutex,
                                         TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueGiveMutexRecursive( QueueHandle_t pxMutex ) FREERTOS_SYSTEM_CALL;
व्योम MPU_vQueueAddToRegistry( QueueHandle_t xQueue,
                              स्थिर अक्षर * pcName ) FREERTOS_SYSTEM_CALL;
व्योम MPU_vQueueUnregisterQueue( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
स्थिर अक्षर * MPU_pcQueueGetName( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xQueueAddToSet( QueueSetMemberHandle_t xQueueOrSemaphore,
                               QueueSetHandle_t xQueueSet ) FREERTOS_SYSTEM_CALL;
QueueSetMemberHandle_t MPU_xQueueSelectFromSet( QueueSetHandle_t xQueueSet,
                                                स्थिर TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
व्योम MPU_v_Queue_Set_Queue_संख्या( QueueHandle_t xQueue,
                               UBaseType_t ux_Queue_संख्या ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Queue_Get_Queue_संख्या( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;
uint8_t MPU_ucQueueGetQueueType( QueueHandle_t xQueue ) FREERTOS_SYSTEM_CALL;

/* Privileged only wrappers for Queue APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
व्योम MPU_vQueueDelete( QueueHandle_t xQueue ) PRIVILEGED_FUNCTION;
QueueHandle_t MPU_xQueueCreateMutex( स्थिर uint8_t ucQueueType ) PRIVILEGED_FUNCTION;
QueueHandle_t MPU_xQueueCreateMutexStatic( स्थिर uint8_t ucQueueType,
                                           StaticQueue_t * pxStaticQueue ) PRIVILEGED_FUNCTION;
QueueHandle_t MPU_xQueueCreateCountingSemaphore( स्थिर UBaseType_t ux_Max_गिनती,
                                                 स्थिर UBaseType_t ux_Initial_गिनती ) PRIVILEGED_FUNCTION;
QueueHandle_t MPU_xQueueCreateCountingSemaphoreStatic( स्थिर UBaseType_t ux_Max_गिनती,
                                                       स्थिर UBaseType_t ux_Initial_गिनती,
                                                       StaticQueue_t * pxStaticQueue ) PRIVILEGED_FUNCTION;
QueueHandle_t MPU_xQueueGenericCreate( स्थिर UBaseType_t ux_Queue_लंबाई,
                                       स्थिर UBaseType_t ux_Item_आकार,
                                       स्थिर uint8_t ucQueueType ) PRIVILEGED_FUNCTION;
QueueHandle_t MPU_xQueueGenericCreateStatic( स्थिर UBaseType_t ux_Queue_लंबाई,
                                             स्थिर UBaseType_t ux_Item_आकार,
                                             uint8_t * pucQueueStorage,
                                             StaticQueue_t * pxStaticQueue,
                                             स्थिर uint8_t ucQueueType ) PRIVILEGED_FUNCTION;
QueueSetHandle_t MPU_xQueueCreateSet( स्थिर UBaseType_t ux_Event_Queue_लंबाई ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xQueueRemoveFromSet( QueueSetMemberHandle_t xQueueOrSemaphore,
                                    QueueSetHandle_t xQueueSet ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xQueueGenericReset( QueueHandle_t xQueue,
                                   BaseType_t xNewQueue ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xQueueGenericGetStaticBuffers( QueueHandle_t xQueue,
                                              uint8_t ** ppucQueueStorage,
                                              StaticQueue_t ** ppxStaticQueue ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xQueueGenericSendFromISR( QueueHandle_t xQueue,
                                         स्थिर व्योम * स्थिर pvItemToQueue,
                                         BaseType_t * स्थिर pxHigherPriorityTaskWoken,
                                         स्थिर BaseType_t xCopyPosition ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xQueueGiveFromISR( QueueHandle_t xQueue,
                                  BaseType_t * स्थिर pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xQueuePeekFromISR( QueueHandle_t xQueue,
                                  व्योम * स्थिर pv_बफर ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xQueueReceiveFromISR( QueueHandle_t xQueue,
                                     व्योम * स्थिर pv_बफर,
                                     BaseType_t * स्थिर pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xQueueIsQueueEmptyFromISR( स्थिर QueueHandle_t xQueue ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xQueueIsQueueFullFromISR( स्थिर QueueHandle_t xQueue ) PRIVILEGED_FUNCTION;
UBaseType_t MPU_uxQueueMessagesWaitingFromISR( स्थिर QueueHandle_t xQueue ) PRIVILEGED_FUNCTION;
TaskHandle_t MPU_xQueueGetMutexHolderFromISR( QueueHandle_t xSemaphore ) PRIVILEGED_FUNCTION;
QueueSetMemberHandle_t MPU_xQueueSelectFromSetFromISR( QueueSetHandle_t xQueueSet ) PRIVILEGED_FUNCTION;

/* MPU versions of timers.h API functions. */
व्योम * MPU_pvTimerGetTimerID( स्थिर TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
व्योम MPU_vTimerSetTimerID( TimerHandle_t xTimer,
                           व्योम * pvNewID ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerIsTimerActive( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTimerGetTimerDaemonTaskHandle( व्योम ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerGenericCommandFromTask( TimerHandle_t xTimer,
                                             स्थिर BaseType_t xCommandID,
                                             स्थिर TickType_t x_Optional_मूल्य,
                                             BaseType_t * स्थिर pxHigherPriorityTaskWoken,
                                             स्थिर TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerGenericCommandFromTaskEntry( स्थिर xTimerGenericCommandFromTaskParams_t * pxParams ) FREERTOS_SYSTEM_CALL;
स्थिर अक्षर * MPU_pcTimerGetName( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
व्योम MPU_vTimerSetReloadMode( TimerHandle_t xTimer,
                              स्थिर BaseType_t uxAutoReload ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_xTimerGetReloadMode( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxTimerGetReloadMode( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_xTimerGetPeriod( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_xTimerGetExpiryTime( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;

/* Privileged only wrappers for Timer APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
TimerHandle_t MPU_xTimerCreate( स्थिर अक्षर * स्थिर pcTimerName,
                                स्थिर TickType_t xTimerPeriodInTicks,
                                स्थिर UBaseType_t uxAutoReload,
                                व्योम * स्थिर pvTimerID,
                                TimerCallbackFunction_t pxCallbackFunction ) PRIVILEGED_FUNCTION;
TimerHandle_t MPU_xTimerCreateStatic( स्थिर अक्षर * स्थिर pcTimerName,
                                      स्थिर TickType_t xTimerPeriodInTicks,
                                      स्थिर UBaseType_t uxAutoReload,
                                      व्योम * स्थिर pvTimerID,
                                      TimerCallbackFunction_t pxCallbackFunction,
                                      StaticTimer_t * px_Timer_बफर ) PRIVILEGED_FUNCTION;
BaseType_t MPU_x_Timer_Get_Static_बफर( TimerHandle_t xTimer,
                                      StaticTimer_t ** ppx_Timer_बफर ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xTimerGenericCommandFromISR( TimerHandle_t xTimer,
                                            स्थिर BaseType_t xCommandID,
                                            स्थिर TickType_t x_Optional_मूल्य,
                                            BaseType_t * स्थिर pxHigherPriorityTaskWoken,
                                            स्थिर TickType_t xTicksToWait ) PRIVILEGED_FUNCTION;

/* MPU versions of event_group.h API functions. */
EventBits_t MPU_xEventGroupWaitBits( EventGroupHandle_t xEventGroup,
                                     स्थिर EventBits_t uxBitsToWaitFor,
                                     स्थिर BaseType_t xClearOnExit,
                                     स्थिर BaseType_t xWaitForAllBits,
                                     TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupWaitBitsEntry( स्थिर xEventGroupWaitBitsParams_t * pxParams ) FREERTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupClearBits( EventGroupHandle_t xEventGroup,
                                      स्थिर EventBits_t uxBitsToClear ) FREERTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupSetBits( EventGroupHandle_t xEventGroup,
                                    स्थिर EventBits_t uxBitsToSet ) FREERTOS_SYSTEM_CALL;
EventBits_t MPU_xEventGroupSync( EventGroupHandle_t xEventGroup,
                                 स्थिर EventBits_t uxBitsToSet,
                                 स्थिर EventBits_t uxBitsToWaitFor,
                                 TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
#अगर ( configUSE_TRACE_FACILITY == 1 )
    UBaseType_t MPU_ux_Event_Group_Get_संख्या( व्योम * xEventGroup ) FREERTOS_SYSTEM_CALL;
    व्योम MPU_v_Event_Group_Set_संख्या( व्योम * xEventGroup,
                                   UBaseType_t ux_Event_Group_संख्या ) FREERTOS_SYSTEM_CALL;
#पूर्ण_अगर /* ( configUSE_TRACE_FACILITY == 1 )*/

/* Privileged only wrappers for Event Group APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
EventGroupHandle_t MPU_xEventGroupCreate( व्योम ) PRIVILEGED_FUNCTION;
EventGroupHandle_t MPU_xEventGroupCreateStatic( StaticEventGroup_t * px_Event_Group_बफर ) PRIVILEGED_FUNCTION;
व्योम MPU_vEventGroupDelete( EventGroupHandle_t xEventGroup ) PRIVILEGED_FUNCTION;
BaseType_t MPU_x_Event_Group_Get_Static_बफर( EventGroupHandle_t xEventGroup,
                                           StaticEventGroup_t ** ppx_Event_Group_बफर ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xEventGroupClearBitsFromISR( EventGroupHandle_t xEventGroup,
                                            स्थिर EventBits_t uxBitsToClear ) PRIVILEGED_FUNCTION;
BaseType_t MPU_xEventGroupSetBitsFromISR( EventGroupHandle_t xEventGroup,
                                          स्थिर EventBits_t uxBitsToSet,
                                          BaseType_t * pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;
EventBits_t MPU_xEventGroupGetBitsFromISR( EventGroupHandle_t xEventGroup ) PRIVILEGED_FUNCTION;

/* MPU versions of message/stream_buffer.h API functions. */
माप_प्रकार MPU_x_Stream_बफर_Send( Stream_बफर_Handle_t x_Stream_बफर,
                              स्थिर व्योम * pv_Tx_तथ्य,
                              माप_प्रकार x_तथ्य_लंबाई_Bytes,
                              TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
माप_प्रकार MPU_x_Stream_बफर_Receive( Stream_बफर_Handle_t x_Stream_बफर,
                                 व्योम * pv_Rx_तथ्य,
                                 माप_प्रकार x_बफर_लंबाई_Bytes,
                                 TickType_t xTicksToWait ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_x_Stream_बफर_Is_Full( Stream_बफर_Handle_t x_Stream_बफर ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_x_Stream_बफर_Is_Empty( Stream_बफर_Handle_t x_Stream_बफर ) FREERTOS_SYSTEM_CALL;
माप_प्रकार MPU_x_Stream_बफर_Spaces_Available( Stream_बफर_Handle_t x_Stream_बफर ) FREERTOS_SYSTEM_CALL;
माप_प्रकार MPU_x_Stream_बफर_Bytes_Available( Stream_बफर_Handle_t x_Stream_बफर ) FREERTOS_SYSTEM_CALL;
BaseType_t MPU_x_Stream_बफर_Set_Trigger_Level( Stream_बफर_Handle_t x_Stream_बफर,
                                             माप_प्रकार xTriggerLevel ) FREERTOS_SYSTEM_CALL;
माप_प्रकार MPU_x_Stream_बफर_अग्र_Message_लंबाई_Bytes( Stream_बफर_Handle_t x_Stream_बफर ) FREERTOS_SYSTEM_CALL;

/* Privileged only wrappers for Stream Buffer APIs. These are needed so that
 * the application can use opaque handles maintained in mpu_wrappers.c
 * with all the APIs. */
Stream_बफर_Handle_t MPU_x_Stream_बफर_Generic_Create( माप_प्रकार x_बफर_आकार_Bytes,
                                                     माप_प्रकार xTriggerLevelBytes,
                                                     BaseType_t x_Stream_बफर_Type,
                                                     Stream_बफर_Callback_Function_t pxSendCompletedCallback,
                                                     Stream_बफर_Callback_Function_t pxReceiveCompletedCallback ) PRIVILEGED_FUNCTION;
Stream_बफर_Handle_t MPU_x_Stream_बफर_Generic_Create_Static( माप_प्रकार x_बफर_आकार_Bytes,
                                                           माप_प्रकार xTriggerLevelBytes,
                                                           BaseType_t x_Stream_बफर_Type,
                                                           uint8_t * स्थिर puc_Stream_बफर_Storage_Area,
                                                           Static_Stream_बफर_t * स्थिर px_Static_Stream_बफर,
                                                           Stream_बफर_Callback_Function_t pxSendCompletedCallback,
                                                           Stream_बफर_Callback_Function_t pxReceiveCompletedCallback ) PRIVILEGED_FUNCTION;
व्योम MPU_v_Stream_बफर_Delete( Stream_बफर_Handle_t x_Stream_बफर ) PRIVILEGED_FUNCTION;
BaseType_t MPU_x_Stream_बफर_Reset( Stream_बफर_Handle_t x_Stream_बफर ) PRIVILEGED_FUNCTION;
BaseType_t MPU_x_Stream_बफर_Get_Static_Buffers( Stream_बफर_Handle_t xStreamBuffers,
                                              uint8_t * ppuc_Stream_बफर_Storage_Area,
                                              Static_Stream_बफर_t * ppx_Static_Stream_बफर ) PRIVILEGED_FUNCTION;
माप_प्रकार MPU_x_Stream_बफर_Send_From_ISR( Stream_बफर_Handle_t x_Stream_बफर,
                                     स्थिर व्योम * pv_Tx_तथ्य,
                                     माप_प्रकार x_तथ्य_लंबाई_Bytes,
                                     BaseType_t * स्थिर pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;
माप_प्रकार MPU_x_Stream_बफर_Receive_From_ISR( Stream_बफर_Handle_t x_Stream_बफर,
                                        व्योम * pv_Rx_तथ्य,
                                        माप_प्रकार x_बफर_लंबाई_Bytes,
                                        BaseType_t * स्थिर pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;
BaseType_t MPU_x_Stream_बफर_Send_Completed_From_ISR( Stream_बफर_Handle_t x_Stream_बफर,
                                                  BaseType_t * pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;
BaseType_t MPU_x_Stream_बफर_Receive_Completed_From_ISR( Stream_बफर_Handle_t x_Stream_बफर,
                                                     BaseType_t * pxHigherPriorityTaskWoken ) PRIVILEGED_FUNCTION;
BaseType_t MPU_x_Stream_बफर_Reset_From_ISR( Stream_बफर_Handle_t x_Stream_बफर ) PRIVILEGED_FUNCTION;

#पूर्ण_अगर /* MPU_PROTOTYPES_H */
