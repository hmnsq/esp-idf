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

#अगर_अघोषित MPU_SYSCALL_NUMBERS_H
#घोषणा MPU_SYSCALL_NUMBERS_H

/* Numbers assigned to various system calls. */
#घोषणा SYSTEM_CALL_xTaskGenericNotify                     0
#घोषणा SYSTEM_CALL_xTaskGenericNotifyWait                 1
#घोषणा SYSTEM_CALL_xTimerGenericCommandFromTask           2
#घोषणा SYSTEM_CALL_xEventGroupWaitBits                    3
#घोषणा SYSTEM_CALL_xTaskDelayUntil                        4
#घोषणा SYSTEM_CALL_xTaskAbortDelay                        5
#घोषणा SYSTEM_CALL_vTaskDelay                             6
#घोषणा SYSTEM_CALL_uxTaskPriorityGet                      7
#घोषणा SYSTEM_CALL_eTaskGetState                          8
#घोषणा SYSTEM_CALL_vTaskGetInfo                           9
#घोषणा SYSTEM_CALL_xTaskGetIdleTaskHandle                 10
#घोषणा SYSTEM_CALL_vTaskSuspend                           11
#घोषणा SYSTEM_CALL_vTaskResume                            12
#घोषणा SYSTEM_CALL_x_Task_Get_Tick_गिनती                      13
#घोषणा SYSTEM_CALL_ux_Task_Get_संख्या_Of_Tasks                 14
#घोषणा SYSTEM_CALL_ul_Task_Get_Run_Time_गणक                15
#घोषणा SYSTEM_CALL_ulTaskGetRunTimePercent                16
#घोषणा SYSTEM_CALL_ulTaskGetIdleRunTimePercent            17
#घोषणा SYSTEM_CALL_ul_Task_Get_Idle_Run_Time_गणक            18
#घोषणा SYSTEM_CALL_vTaskSetApplicationTaskTag             19
#घोषणा SYSTEM_CALL_xTaskGetApplicationTaskTag             20
#घोषणा SYSTEM_CALL_vTaskSetThreadLocalStoragePointer      21
#घोषणा SYSTEM_CALL_pvTaskGetThreadLocalStoragePointer     22
#घोषणा SYSTEM_CALL_uxTaskGetSystemState                   23
#घोषणा SYSTEM_CALL_ux_Task_Get_ढेर_ऊपरी_Water_Mark            24
#घोषणा SYSTEM_CALL_ux_Task_Get_ढेर_ऊपरी_Water_Mark2           25
#घोषणा SYSTEM_CALL_xTaskGetCurrentTaskHandle              26
#घोषणा SYSTEM_CALL_xTaskGetSchedulerState                 27
#घोषणा SYSTEM_CALL_vTaskSetTimeOutState                   28
#घोषणा SYSTEM_CALL_xTaskCheckForTimeOut                   29
#घोषणा SYSTEM_CALL_ulTaskGenericNotifyTake                30
#घोषणा SYSTEM_CALL_xTaskGenericNotifyStateClear           31
#घोषणा SYSTEM_CALL_ul_Task_Generic_Notify_मूल्य_Clear          32
#घोषणा SYSTEM_CALL_xQueueGenericSend                      33
#घोषणा SYSTEM_CALL_uxQueueMessagesWaiting                 34
#घोषणा SYSTEM_CALL_uxQueueSpacesAvailable                 35
#घोषणा SYSTEM_CALL_xQueueReceive                          36
#घोषणा SYSTEM_CALL_xQueuePeek                             37
#घोषणा SYSTEM_CALL_xQueueSemaphoreTake                    38
#घोषणा SYSTEM_CALL_xQueueGetMutexHolder                   39
#घोषणा SYSTEM_CALL_xQueueTakeMutexRecursive               40
#घोषणा SYSTEM_CALL_xQueueGiveMutexRecursive               41
#घोषणा SYSTEM_CALL_xQueueSelectFromSet                    42
#घोषणा SYSTEM_CALL_xQueueAddToSet                         43
#घोषणा SYSTEM_CALL_vQueueAddToRegistry                    44
#घोषणा SYSTEM_CALL_vQueueUnregisterQueue                  45
#घोषणा SYSTEM_CALL_pcQueueGetName                         46
#घोषणा SYSTEM_CALL_pvTimerGetTimerID                      47
#घोषणा SYSTEM_CALL_vTimerSetTimerID                       48
#घोषणा SYSTEM_CALL_xTimerIsTimerActive                    49
#घोषणा SYSTEM_CALL_xTimerGetTimerDaemonTaskHandle         50
#घोषणा SYSTEM_CALL_pcTimerGetName                         51
#घोषणा SYSTEM_CALL_vTimerSetReloadMode                    52
#घोषणा SYSTEM_CALL_xTimerGetReloadMode                    53
#घोषणा SYSTEM_CALL_uxTimerGetReloadMode                   54
#घोषणा SYSTEM_CALL_xTimerGetPeriod                        55
#घोषणा SYSTEM_CALL_xTimerGetExpiryTime                    56
#घोषणा SYSTEM_CALL_xEventGroupClearBits                   57
#घोषणा SYSTEM_CALL_xEventGroupSetBits                     58
#घोषणा SYSTEM_CALL_xEventGroupSync                        59
#घोषणा SYSTEM_CALL_ux_Event_Group_Get_संख्या                  60
#घोषणा SYSTEM_CALL_v_Event_Group_Set_संख्या                   61
#घोषणा SYSTEM_CALL_x_Stream_बफर_Send                      62
#घोषणा SYSTEM_CALL_x_Stream_बफर_Receive                   63
#घोषणा SYSTEM_CALL_x_Stream_बफर_Is_Full                    64
#घोषणा SYSTEM_CALL_x_Stream_बफर_Is_Empty                   65
#घोषणा SYSTEM_CALL_x_Stream_बफर_Spaces_Available           66
#घोषणा SYSTEM_CALL_x_Stream_बफर_Bytes_Available            67
#घोषणा SYSTEM_CALL_x_Stream_बफर_Set_Trigger_Level           68
#घोषणा SYSTEM_CALL_x_Stream_बफर_अग्र_Message_लंबाई_Bytes    69
#घोषणा अंक_SYSTEM_CALLS                                   70  /* Total number of system calls. */

#पूर्ण_अगर /* MPU_SYSCALL_NUMBERS_H */
