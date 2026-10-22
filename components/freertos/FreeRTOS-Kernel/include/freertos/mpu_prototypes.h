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


#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ MPU_PROTOTYPES_H
#ವ್ಯಾಖ್ಯಾನಿಸು MPU_PROTOTYPES_H

/* MPU versions of task.h API functions. */
ತಳ_Type_t MPU_xTaskCreate( TaskFunction_t px_Task_ಸಂಕೇತ,
                            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ pcName,
                            ಸ್ಥಿರಾಂಕ uint16_t us_ರಾಶಿ_ಆಳ,
                            ಶೂನ್ಯ * ಸ್ಥಿರಾಂಕ pvParameters,
                            UBaseType_t ux_ಆದ್ಯತೆ,
                            TaskHandle_t * ಸ್ಥಿರಾಂಕ pxCreatedTask ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskCreateStatic( TaskFunction_t px_Task_ಸಂಕೇತ,
                                    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ pcName,
                                    ಸ್ಥಿರಾಂಕ uint32_t ul_ರಾಶಿ_ಆಳ,
                                    ಶೂನ್ಯ * ಸ್ಥಿರಾಂಕ pvParameters,
                                    UBaseType_t ux_ಆದ್ಯತೆ,
                                    ರಾಶಿ_Type_t * ಸ್ಥಿರಾಂಕ pux_ರಾಶಿ_ಬಫರ್,
                                    StaticTask_t * ಸ್ಥಿರಾಂಕ px_Task_ಬಫರ್ ) FREERTOS_SYSTEM_CALL;
ಶೂನ್ಯ MPU_vTaskDelete( TaskHandle_t x_Task_ಗಮ್ಯ_Delete ) FREERTOS_SYSTEM_CALL;
ಶೂನ್ಯ MPU_vTaskDelay( ಸ್ಥಿರಾಂಕ TickType_t x_Ticks_ಗಮ್ಯ_Delay ) FREERTOS_SYSTEM_CALL;
ತಳ_Type_t MPU_xTaskDelayUntil( TickType_t * ಸ್ಥಿರಾಂಕ px_ಹಿಂದಿನ_Wake_ಸಮಯ,
                                ಸ್ಥಿರಾಂಕ TickType_t x_ಸಮಯ_Increment ) FREERTOS_SYSTEM_CALL;
ತಳ_Type_t MPU_xTaskAbortDelay( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_ಆದ್ಯತೆ_Get( ಸ್ಥಿರಾಂಕ TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
eTaskState MPU_eTaskGetState( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
ಶೂನ್ಯ MPU_vTaskGetInfo( TaskHandle_t xTask,
                       Task_ಸ್ಥಿತಿ_t * px_Task_ಸ್ಥಿತಿ,
                       ತಳ_Type_t x_Get_Free_ರಾಶಿ_Space,
                       eTaskState eState ) FREERTOS_SYSTEM_CALL;
ಶೂನ್ಯ MPU_v_Task_ಆದ್ಯತೆ_ಗಣ( TaskHandle_t xTask,
                           UBaseType_t ux_New_ಆದ್ಯತೆ ) FREERTOS_SYSTEM_CALL;
ಶೂನ್ಯ MPU_vTaskSuspend( TaskHandle_t x_Task_ಗಮ್ಯ_Suspend ) FREERTOS_SYSTEM_CALL;
ಶೂನ್ಯ MPU_vTaskResume( TaskHandle_t x_Task_ಗಮ್ಯ_Resume ) FREERTOS_SYSTEM_CALL;
ಶೂನ್ಯ MPU_v_Task_ಆರಂಭ_Scheduler( ಶೂನ್ಯ ) FREERTOS_SYSTEM_CALL;
ಶೂನ್ಯ MPU_vTaskSuspendAll( ಶೂನ್ಯ ) FREERTOS_SYSTEM_CALL;
ತಳ_Type_t MPU_xTaskResumeAll( ಶೂನ್ಯ ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_x_Task_Get_Tick_ಎಣಿಕೆ( ಶೂನ್ಯ ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_Get_ಸಂಖ್ಯೆ_ದ_Tasks( ಶೂನ್ಯ ) FREERTOS_SYSTEM_CALL;
ಅಕ್ಷರ * MPU_pcTaskGetName( TaskHandle_t x_Task_ಗಮ್ಯ_Query ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetHandle( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * pc_Name_ಗಮ್ಯ_Query ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Task_Get_ರಾಶಿ_ಮೇಲಿನ_Water_Mark( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
config_ರಾಶಿ_ಆಳ_TYPE MPU_ux_Task_Get_ರಾಶಿ_ಮೇಲಿನ_Water_Mark2( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
ಶೂನ್ಯ MPU_v_Task_ಗಣ_Application_Task_ಟ್ಯಾಗ್( TaskHandle_t xTask,
                                     TaskHookFunction_t pxHookFunction ) FREERTOS_SYSTEM_CALL;
TaskHookFunction_t MPU_x_Task_Get_Application_Task_ಟ್ಯಾಗ್( TaskHandle_t xTask ) FREERTOS_SYSTEM_CALL;
ಶೂನ್ಯ MPU_v_Task_ಗಣ_Thread_Local_Storage_ಸೂಚಕ( TaskHandle_t x_Task_ಗಮ್ಯ_ಗಣ,
                                            ತಳ_Type_t x_ಸೂಚ್ಯಂಕ,
                                            ಶೂನ್ಯ * pv_ಮೌಲ್ಯ ) FREERTOS_SYSTEM_CALL;
ಶೂನ್ಯ * MPU_pv_Task_Get_Thread_Local_Storage_ಸೂಚಕ( TaskHandle_t x_Task_ಗಮ್ಯ_Query,
                                               ತಳ_Type_t x_ಸೂಚ್ಯಂಕ ) FREERTOS_SYSTEM_CALL;
ತಳ_Type_t MPU_xTaskCallApplicationTaskHook( TaskHandle_t xTask,
                                             ಶೂನ್ಯ * pvParameter ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetIdleTaskHandle( ಶೂನ್ಯ ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_uxTaskGetSystemState( Task_ಸ್ಥಿತಿ_t * ಸ್ಥಿರಾಂಕ px_Task_ಸ್ಥಿತಿ_Array,
                                      ಸ್ಥಿರಾಂಕ UBaseType_t ux_Array_ಗಾತ್ರ,
                                      config_ಸತತ_ಸಮಯ_COUNTER_TYPE * ಸ್ಥಿರಾಂಕ pul_ಒಟ್ಟು_ಸತತ_ಸಮಯ ) FREERTOS_SYSTEM_CALL;
config_ಸತತ_ಸಮಯ_COUNTER_TYPE MPU_ul_Task_Get_Idle_ಸತತ_ಸಮಯ_Counter( ಶೂನ್ಯ ) FREERTOS_SYSTEM_CALL;
config_ಸತತ_ಸಮಯ_COUNTER_TYPE MPU_ul_Task_Get_Idle_ಸತತ_ಸಮಯ_Percent( ಶೂನ್ಯ ) FREERTOS_SYSTEM_CALL;
ಶೂನ್ಯ MPU_vTaskList( ಅಕ್ಷರ * pc_Write_ಬಫರ್ ) FREERTOS_SYSTEM_CALL;
ಶೂನ್ಯ MPU_v_Task_Get_ಸತತ_ಸಮಯ_Stats( ಅಕ್ಷರ * pc_Write_ಬಫರ್ ) FREERTOS_SYSTEM_CALL;
ತಳ_Type_t MPU_xTaskGenericNotify( TaskHandle_t x_Task_ಗಮ್ಯ_Notify,
                                   UBaseType_t ux_ಸೂಚ್ಯಂಕ_ಗಮ್ಯ_Notify,
                                   uint32_t ul_ಮೌಲ್ಯ,
                                   eNotifyAction eAction,
                                   uint32_t * pul_ಹಿಂದಿನ_Notification_ಮೌಲ್ಯ ) FREERTOS_SYSTEM_CALL;
ತಳ_Type_t MPU_xTaskGenericNotifyWait( UBaseType_t ux_ಸೂಚ್ಯಂಕ_ಗಮ್ಯ_Wait_On,
                                       uint32_t ul_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Clear_On_ನಮೂದು,
                                       uint32_t ul_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Clear_On_Exit,
                                       uint32_t * pul_Notification_ಮೌಲ್ಯ,
                                       TickType_t x_Ticks_ಗಮ್ಯ_Wait ) FREERTOS_SYSTEM_CALL;
uint32_t MPU_ulTaskGenericNotifyTake( UBaseType_t ux_ಸೂಚ್ಯಂಕ_ಗಮ್ಯ_Wait_On,
                                      ತಳ_Type_t x_Clear_ಎಣಿಕೆ_On_Exit,
                                      TickType_t x_Ticks_ಗಮ್ಯ_Wait ) FREERTOS_SYSTEM_CALL;
ತಳ_Type_t MPU_xTaskGenericNotifyStateClear( TaskHandle_t xTask,
                                             UBaseType_t ux_ಸೂಚ್ಯಂಕ_ಗಮ್ಯ_Clear ) FREERTOS_SYSTEM_CALL;
uint32_t MPU_ul_Task_Generic_Notify_ಮೌಲ್ಯ_Clear( TaskHandle_t xTask,
                                            UBaseType_t ux_ಸೂಚ್ಯಂಕ_ಗಮ್ಯ_Clear,
                                            uint32_t ul_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Clear ) FREERTOS_SYSTEM_CALL;
ತಳ_Type_t MPU_xTaskIncrementTick( ಶೂನ್ಯ ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTaskGetCurrentTaskHandle( ಶೂನ್ಯ ) FREERTOS_SYSTEM_CALL;
ಶೂನ್ಯ MPU_v_Task_ಗಣ_ಸಮಯ_ಹೊರಹೋಗು_State( ಸಮಯ_ಹೊರಹೋಗು_t * ಸ್ಥಿರಾಂಕ px_ಸಮಯ_ಹೊರಹೋಗು ) FREERTOS_SYSTEM_CALL;
ತಳ_Type_t MPU_x_Task_Check_For_ಸಮಯ_ಹೊರಹೋಗು( ಸಮಯ_ಹೊರಹೋಗು_t * ಸ್ಥಿರಾಂಕ px_ಸಮಯ_ಹೊರಹೋಗು,
                                     TickType_t * ಸ್ಥಿರಾಂಕ px_Ticks_ಗಮ್ಯ_Wait ) FREERTOS_SYSTEM_CALL;
ಶೂನ್ಯ MPU_vTaskMissedYield( ಶೂನ್ಯ ) FREERTOS_SYSTEM_CALL;
ತಳ_Type_t MPU_xTaskGetSchedulerState( ಶೂನ್ಯ ) FREERTOS_SYSTEM_CALL;
ತಳ_Type_t MPU_x_Task_Catch_ಮೇಲೆ_Ticks( TickType_t x_Ticks_ಗಮ್ಯ_Catch_ಮೇಲೆ ) FREERTOS_SYSTEM_CALL;

/* MPU versions of queue.h API functions. */
ತಳ_Type_t MPU_x_ಸರತಿ_Generic_Send( ಸರತಿ_Handle_t x_ಸರತಿ,
                                  ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ * ಸ್ಥಿರಾಂಕ pv_ಅಂಶ_ಗಮ್ಯ_ಸರತಿ,
                                  TickType_t x_Ticks_ಗಮ್ಯ_Wait,
                                  ಸ್ಥಿರಾಂಕ ತಳ_Type_t xCopyPosition ) FREERTOS_SYSTEM_CALL;
ತಳ_Type_t MPU_x_ಸರತಿ_Receive( ಸರತಿ_Handle_t x_ಸರತಿ,
                              ಶೂನ್ಯ * ಸ್ಥಿರಾಂಕ pv_ಬಫರ್,
                              TickType_t x_Ticks_ಗಮ್ಯ_Wait ) FREERTOS_SYSTEM_CALL;
ತಳ_Type_t MPU_x_ಸರತಿ_Peek( ಸರತಿ_Handle_t x_ಸರತಿ,
                           ಶೂನ್ಯ * ಸ್ಥಿರಾಂಕ pv_ಬಫರ್,
                           TickType_t x_Ticks_ಗಮ್ಯ_Wait ) FREERTOS_SYSTEM_CALL;
ತಳ_Type_t MPU_x_ಸರತಿ_Semaphore_Take( ಸರತಿ_Handle_t x_ಸರತಿ,
                                    TickType_t x_Ticks_ಗಮ್ಯ_Wait ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_ಸರತಿ_Messages_ಕಾಯುವಿಕೆ( ಸ್ಥಿರಾಂಕ ಸರತಿ_Handle_t x_ಸರತಿ ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_ಸರತಿ_Spaces_ಲಭ್ಯ( ಸ್ಥಿರಾಂಕ ಸರತಿ_Handle_t x_ಸರತಿ ) FREERTOS_SYSTEM_CALL;
ಶೂನ್ಯ MPU_v_ಸರತಿ_Delete( ಸರತಿ_Handle_t x_ಸರತಿ ) FREERTOS_SYSTEM_CALL;
ಸರತಿ_Handle_t MPU_x_ಸರತಿ_Create_Mutex( ಸ್ಥಿರಾಂಕ uint8_t uc_ಸರತಿ_Type ) FREERTOS_SYSTEM_CALL;
ಸರತಿ_Handle_t MPU_x_ಸರತಿ_Create_Mutex_Static( ಸ್ಥಿರಾಂಕ uint8_t uc_ಸರತಿ_Type,
                                           Static_ಸರತಿ_t * px_Static_ಸರತಿ ) FREERTOS_SYSTEM_CALL;
ಸರತಿ_Handle_t MPU_x_ಸರತಿ_Create_Counting_Semaphore( ಸ್ಥಿರಾಂಕ UBaseType_t ux_Max_ಎಣಿಕೆ,
                                                 ಸ್ಥಿರಾಂಕ UBaseType_t ux_Initial_ಎಣಿಕೆ ) FREERTOS_SYSTEM_CALL;
ಸರತಿ_Handle_t MPU_x_ಸರತಿ_Create_Counting_Semaphore_Static( ಸ್ಥಿರಾಂಕ UBaseType_t ux_Max_ಎಣಿಕೆ,
                                                       ಸ್ಥಿರಾಂಕ UBaseType_t ux_Initial_ಎಣಿಕೆ,
                                                       Static_ಸರತಿ_t * px_Static_ಸರತಿ ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_x_ಸರತಿ_Get_Mutex_Holder( ಸರತಿ_Handle_t xSemaphore ) FREERTOS_SYSTEM_CALL;
ತಳ_Type_t MPU_x_ಸರತಿ_Take_Mutex_Recursive( ಸರತಿ_Handle_t xMutex,
                                         TickType_t x_Ticks_ಗಮ್ಯ_Wait ) FREERTOS_SYSTEM_CALL;
ತಳ_Type_t MPU_x_ಸರತಿ_Give_Mutex_Recursive( ಸರತಿ_Handle_t pxMutex ) FREERTOS_SYSTEM_CALL;
ಶೂನ್ಯ MPU_v_ಸರತಿ_ಕೂಡಿಸು_ಗಮ್ಯ_Registry( ಸರತಿ_Handle_t x_ಸರತಿ,
                              ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * pcName ) FREERTOS_SYSTEM_CALL;
ಶೂನ್ಯ MPU_v_ಸರತಿ_Unregister_ಸರತಿ( ಸರತಿ_Handle_t x_ಸರತಿ ) FREERTOS_SYSTEM_CALL;
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * MPU_pc_ಸರತಿ_Get_Name( ಸರತಿ_Handle_t x_ಸರತಿ ) FREERTOS_SYSTEM_CALL;
ಸರತಿ_Handle_t MPU_x_ಸರತಿ_Generic_Create( ಸ್ಥಿರಾಂಕ UBaseType_t ux_ಸರತಿ_ಉದ್ದಳತೆ,
                                       ಸ್ಥಿರಾಂಕ UBaseType_t ux_ಅಂಶ_ಗಾತ್ರ,
                                       ಸ್ಥಿರಾಂಕ uint8_t uc_ಸರತಿ_Type ) FREERTOS_SYSTEM_CALL;
ಸರತಿ_Handle_t MPU_x_ಸರತಿ_Generic_Create_Static( ಸ್ಥಿರಾಂಕ UBaseType_t ux_ಸರತಿ_ಉದ್ದಳತೆ,
                                             ಸ್ಥಿರಾಂಕ UBaseType_t ux_ಅಂಶ_ಗಾತ್ರ,
                                             uint8_t * puc_ಸರತಿ_Storage,
                                             Static_ಸರತಿ_t * px_Static_ಸರತಿ,
                                             ಸ್ಥಿರಾಂಕ uint8_t uc_ಸರತಿ_Type ) FREERTOS_SYSTEM_CALL;
ಸರತಿ_ಗಣ_Handle_t MPU_x_ಸರತಿ_Create_ಗಣ( ಸ್ಥಿರಾಂಕ UBaseType_t ux_Event_ಸರತಿ_ಉದ್ದಳತೆ ) FREERTOS_SYSTEM_CALL;
ತಳ_Type_t MPU_x_ಸರತಿ_ಕೂಡಿಸು_ಗಮ್ಯ_ಗಣ( ಸರತಿ_ಗಣ_Member_Handle_t x_ಸರತಿ_Or_Semaphore,
                               ಸರತಿ_ಗಣ_Handle_t x_ಸರತಿ_ಗಣ ) FREERTOS_SYSTEM_CALL;
ತಳ_Type_t MPU_x_ಸರತಿ_Remove_From_ಗಣ( ಸರತಿ_ಗಣ_Member_Handle_t x_ಸರತಿ_Or_Semaphore,
                                    ಸರತಿ_ಗಣ_Handle_t x_ಸರತಿ_ಗಣ ) FREERTOS_SYSTEM_CALL;
ಸರತಿ_ಗಣ_Member_Handle_t MPU_x_ಸರತಿ_Select_From_ಗಣ( ಸರತಿ_ಗಣ_Handle_t x_ಸರತಿ_ಗಣ,
                                                ಸ್ಥಿರಾಂಕ TickType_t x_Ticks_ಗಮ್ಯ_Wait ) FREERTOS_SYSTEM_CALL;
ತಳ_Type_t MPU_x_ಸರತಿ_Generic_Reset( ಸರತಿ_Handle_t x_ಸರತಿ,
                                   ತಳ_Type_t x_New_ಸರತಿ ) FREERTOS_SYSTEM_CALL;
ಶೂನ್ಯ MPU_v_ಸರತಿ_ಗಣ_ಸರತಿ_ಸಂಖ್ಯೆ( ಸರತಿ_Handle_t x_ಸರತಿ,
                               UBaseType_t ux_ಸರತಿ_ಸಂಖ್ಯೆ ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_ಸರತಿ_Get_ಸರತಿ_ಸಂಖ್ಯೆ( ಸರತಿ_Handle_t x_ಸರತಿ ) FREERTOS_SYSTEM_CALL;
uint8_t MPU_uc_ಸರತಿ_Get_ಸರತಿ_Type( ಸರತಿ_Handle_t x_ಸರತಿ ) FREERTOS_SYSTEM_CALL;

/* MPU versions of timers.h API functions. */
TimerHandle_t MPU_xTimerCreate( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ pcTimerName,
                                ಸ್ಥಿರಾಂಕ TickType_t x_Timer_Period_ಒಳ_Ticks,
                                ಸ್ಥಿರಾಂಕ UBaseType_t uxAutoReload,
                                ಶೂನ್ಯ * ಸ್ಥಿರಾಂಕ pvTimerID,
                                TimerCallbackFunction_t pxCallbackFunction ) FREERTOS_SYSTEM_CALL;
TimerHandle_t MPU_xTimerCreateStatic( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ pcTimerName,
                                      ಸ್ಥಿರಾಂಕ TickType_t x_Timer_Period_ಒಳ_Ticks,
                                      ಸ್ಥಿರಾಂಕ UBaseType_t uxAutoReload,
                                      ಶೂನ್ಯ * ಸ್ಥಿರಾಂಕ pvTimerID,
                                      TimerCallbackFunction_t pxCallbackFunction,
                                      StaticTimer_t * px_Timer_ಬಫರ್ ) FREERTOS_SYSTEM_CALL;
ಶೂನ್ಯ * MPU_pvTimerGetTimerID( ಸ್ಥಿರಾಂಕ TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
ಶೂನ್ಯ MPU_v_Timer_ಗಣ_Timer_ID( TimerHandle_t xTimer,
                           ಶೂನ್ಯ * pvNewID ) FREERTOS_SYSTEM_CALL;
ತಳ_Type_t MPU_x_Timer_Is_Timer_ಸಕ್ರಿಯ( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TaskHandle_t MPU_xTimerGetTimerDaemonTaskHandle( ಶೂನ್ಯ ) FREERTOS_SYSTEM_CALL;
ತಳ_Type_t MPU_xTimerPendFunctionCall( PendedFunction_t x_Function_ಗಮ್ಯ_Pend,
                                       ಶೂನ್ಯ * pvParameter1,
                                       uint32_t ulParameter2,
                                       TickType_t x_Ticks_ಗಮ್ಯ_Wait ) FREERTOS_SYSTEM_CALL;
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * MPU_pcTimerGetName( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
ಶೂನ್ಯ MPU_v_Timer_ಗಣ_Reload_ಬಹುಲಕ( TimerHandle_t xTimer,
                              ಸ್ಥಿರಾಂಕ UBaseType_t uxAutoReload ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Timer_Get_Reload_ಬಹುಲಕ( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_xTimerGetPeriod( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
TickType_t MPU_x_Timer_Get_Expiry_ಸಮಯ( TimerHandle_t xTimer ) FREERTOS_SYSTEM_CALL;
ತಳ_Type_t MPU_xTimerCreateTimerTask( ಶೂನ್ಯ ) FREERTOS_SYSTEM_CALL;
ತಳ_Type_t MPU_xTimerGenericCommand( TimerHandle_t xTimer,
                                     ಸ್ಥಿರಾಂಕ ತಳ_Type_t xCommandID,
                                     ಸ್ಥಿರಾಂಕ TickType_t x_Optional_ಮೌಲ್ಯ,
                                     ತಳ_Type_t * ಸ್ಥಿರಾಂಕ px_Higher_ಆದ್ಯತೆ_Task_Woken,
                                     ಸ್ಥಿರಾಂಕ TickType_t x_Ticks_ಗಮ್ಯ_Wait ) FREERTOS_SYSTEM_CALL;

/* MPU versions of event_group.h API functions. */
Event_ಗುಂಪು_Handle_t MPU_x_Event_ಗುಂಪು_Create( ಶೂನ್ಯ ) FREERTOS_SYSTEM_CALL;
Event_ಗುಂಪು_Handle_t MPU_x_Event_ಗುಂಪು_Create_Static( Static_Event_ಗುಂಪು_t * px_Event_ಗುಂಪು_ಬಫರ್ ) FREERTOS_SYSTEM_CALL;
Event_ಬಿಟ್ಟುಗಳು_t MPU_x_Event_ಗುಂಪು_Wait_ಬಿಟ್ಟುಗಳು( Event_ಗುಂಪು_Handle_t x_Event_ಗುಂಪು,
                                     ಸ್ಥಿರಾಂಕ Event_ಬಿಟ್ಟುಗಳು_t ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Wait_For,
                                     ಸ್ಥಿರಾಂಕ ತಳ_Type_t xClearOnExit,
                                     ಸ್ಥಿರಾಂಕ ತಳ_Type_t x_Wait_For_All_ಬಿಟ್ಟುಗಳು,
                                     TickType_t x_Ticks_ಗಮ್ಯ_Wait ) FREERTOS_SYSTEM_CALL;
Event_ಬಿಟ್ಟುಗಳು_t MPU_x_Event_ಗುಂಪು_Clear_ಬಿಟ್ಟುಗಳು( Event_ಗುಂಪು_Handle_t x_Event_ಗುಂಪು,
                                      ಸ್ಥಿರಾಂಕ Event_ಬಿಟ್ಟುಗಳು_t ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Clear ) FREERTOS_SYSTEM_CALL;
Event_ಬಿಟ್ಟುಗಳು_t MPU_x_Event_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು( Event_ಗುಂಪು_Handle_t x_Event_ಗುಂಪು,
                                    ಸ್ಥಿರಾಂಕ Event_ಬಿಟ್ಟುಗಳು_t ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_ಗಣ ) FREERTOS_SYSTEM_CALL;
Event_ಬಿಟ್ಟುಗಳು_t MPU_x_Event_ಗುಂಪು_Sync( Event_ಗುಂಪು_Handle_t x_Event_ಗುಂಪು,
                                 ಸ್ಥಿರಾಂಕ Event_ಬಿಟ್ಟುಗಳು_t ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_ಗಣ,
                                 ಸ್ಥಿರಾಂಕ Event_ಬಿಟ್ಟುಗಳು_t ux_ಬಿಟ್ಟುಗಳು_ಗಮ್ಯ_Wait_For,
                                 TickType_t x_Ticks_ಗಮ್ಯ_Wait ) FREERTOS_SYSTEM_CALL;
ಶೂನ್ಯ MPU_v_Event_ಗುಂಪು_Delete( Event_ಗುಂಪು_Handle_t x_Event_ಗುಂಪು ) FREERTOS_SYSTEM_CALL;
UBaseType_t MPU_ux_Event_ಗುಂಪು_Get_ಸಂಖ್ಯೆ( ಶೂನ್ಯ * x_Event_ಗುಂಪು ) FREERTOS_SYSTEM_CALL;

/* MPU versions of message/stream_buffer.h API functions. */
size_t MPU_x_ಹರಿವು_ಬಫರ್_Send( ಹರಿವು_ಬಫರ್_Handle_t x_ಹರಿವು_ಬಫರ್,
                              ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ * pv_Tx_ದತ್ತಾಂಶ,
                              size_t x_ದತ್ತಾಂಶ_ಉದ್ದಳತೆ_ಬೈಟುಗಳು,
                              TickType_t x_Ticks_ಗಮ್ಯ_Wait ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_ಹರಿವು_ಬಫರ್_Receive( ಹರಿವು_ಬಫರ್_Handle_t x_ಹರಿವು_ಬಫರ್,
                                 ಶೂನ್ಯ * pv_Rx_ದತ್ತಾಂಶ,
                                 size_t x_ಬಫರ್_ಉದ್ದಳತೆ_ಬೈಟುಗಳು,
                                 TickType_t x_Ticks_ಗಮ್ಯ_Wait ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_ಹರಿವು_ಬಫರ್_ಮುಂದಿನ_ಸಂದೇಶ_ಉದ್ದಳತೆ_ಬೈಟುಗಳು( ಹರಿವು_ಬಫರ್_Handle_t x_ಹರಿವು_ಬಫರ್ ) FREERTOS_SYSTEM_CALL;
ಶೂನ್ಯ MPU_v_ಹರಿವು_ಬಫರ್_Delete( ಹರಿವು_ಬಫರ್_Handle_t x_ಹರಿವು_ಬಫರ್ ) FREERTOS_SYSTEM_CALL;
ತಳ_Type_t MPU_x_ಹರಿವು_ಬಫರ್_Is_ತುಂಬಿದ( ಹರಿವು_ಬಫರ್_Handle_t x_ಹರಿವು_ಬಫರ್ ) FREERTOS_SYSTEM_CALL;
ತಳ_Type_t MPU_x_ಹರಿವು_ಬಫರ್_Is_ಖಾಲಿಗಳು( ಹರಿವು_ಬಫರ್_Handle_t x_ಹರಿವು_ಬಫರ್ ) FREERTOS_SYSTEM_CALL;
ತಳ_Type_t MPU_x_ಹರಿವು_ಬಫರ್_Reset( ಹರಿವು_ಬಫರ್_Handle_t x_ಹರಿವು_ಬಫರ್ ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_ಹರಿವು_ಬಫರ್_Spaces_ಲಭ್ಯ( ಹರಿವು_ಬಫರ್_Handle_t x_ಹರಿವು_ಬಫರ್ ) FREERTOS_SYSTEM_CALL;
size_t MPU_x_ಹರಿವು_ಬಫರ್_ಬೈಟುಗಳು_ಲಭ್ಯ( ಹರಿವು_ಬಫರ್_Handle_t x_ಹರಿವು_ಬಫರ್ ) FREERTOS_SYSTEM_CALL;
ತಳ_Type_t MPU_x_ಹರಿವು_ಬಫರ್_ಗಣ_Trigger_Level( ಹರಿವು_ಬಫರ್_Handle_t x_ಹರಿವು_ಬಫರ್,
                                             size_t xTriggerLevel ) FREERTOS_SYSTEM_CALL;
ಹರಿವು_ಬಫರ್_Handle_t MPU_x_ಹರಿವು_ಬಫರ್_Generic_Create( size_t x_ಬಫರ್_ಗಾತ್ರ_ಬೈಟುಗಳು,
                                                     size_t x_Trigger_Level_ಬೈಟುಗಳು,
                                                     ತಳ_Type_t x_Is_ಸಂದೇಶ_ಬಫರ್,
                                                     ಹರಿವು_ಬಫರ್_Callback_Function_t pxSendCompletedCallback,
                                                     ಹರಿವು_ಬಫರ್_Callback_Function_t pxReceiveCompletedCallback ) FREERTOS_SYSTEM_CALL;
ಹರಿವು_ಬಫರ್_Handle_t MPU_x_ಹರಿವು_ಬಫರ್_Generic_Create_Static( size_t x_ಬಫರ್_ಗಾತ್ರ_ಬೈಟುಗಳು,
                                                           size_t x_Trigger_Level_ಬೈಟುಗಳು,
                                                           ತಳ_Type_t x_Is_ಸಂದೇಶ_ಬಫರ್,
                                                           uint8_t * ಸ್ಥಿರಾಂಕ puc_ಹರಿವು_ಬಫರ್_Storage_Area,
                                                           Static_ಹರಿವು_ಬಫರ್_t * ಸ್ಥಿರಾಂಕ px_Static_ಹರಿವು_ಬಫರ್,
                                                           ಹರಿವು_ಬಫರ್_Callback_Function_t pxSendCompletedCallback,
                                                           ಹರಿವು_ಬಫರ್_Callback_Function_t pxReceiveCompletedCallback ) FREERTOS_SYSTEM_CALL;



#ಆದರೆ_ಅಂತ್ಯ /* MPU_PROTOTYPES_H */
