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

#यदि_अपरिभाषितम् MPU_SYSCALL_NUMBERS_H
#परिभाषय MPU_SYSCALL_NUMBERS_H

/* Numbers assigned to various system calls. */
#परिभाषय SYSTEM_CALL_xTaskGenericNotify                     0
#परिभाषय SYSTEM_CALL_xTaskGenericNotifyWait                 1
#परिभाषय SYSTEM_CALL_xTimerGenericCommandFromTask           2
#परिभाषय SYSTEM_CALL_x_Event_गणः_Wait_द्वयङ्काः                    3
#परिभाषय SYSTEM_CALL_xTaskDelayUntil                        4
#परिभाषय SYSTEM_CALL_xTaskAbortDelay                        5
#परिभाषय SYSTEM_CALL_vTaskDelay                             6
#परिभाषय SYSTEM_CALL_ux_Task_प्राथमिकता_Get                      7
#परिभाषय SYSTEM_CALL_eTaskGetState                          8
#परिभाषय SYSTEM_CALL_vTaskGetInfo                           9
#परिभाषय SYSTEM_CALL_xTaskGetIdleTaskHandle                 10
#परिभाषय SYSTEM_CALL_vTaskSuspend                           11
#परिभाषय SYSTEM_CALL_vTaskResume                            12
#परिभाषय SYSTEM_CALL_x_Task_Get_Tick_गणनम्                      13
#परिभाषय SYSTEM_CALL_ux_Task_Get_सङ्ख्या_सम्बन्धि_Tasks                 14
#परिभाषय SYSTEM_CALL_ul_Task_Get_सततम्_Time_गणक                15
#परिभाषय SYSTEM_CALL_ul_Task_Get_सततम्_Time_Percent                16
#परिभाषय SYSTEM_CALL_ul_Task_Get_Idle_सततम्_Time_Percent            17
#परिभाषय SYSTEM_CALL_ul_Task_Get_Idle_सततम्_Time_गणक            18
#परिभाषय SYSTEM_CALL_v_Task_समुच्चयः_Application_Task_चिह्नकम्             19
#परिभाषय SYSTEM_CALL_x_Task_Get_Application_Task_चिह्नकम्             20
#परिभाषय SYSTEM_CALL_v_Task_समुच्चयः_Thread_Local_Storage_Pointer      21
#परिभाषय SYSTEM_CALL_pvTaskGetThreadLocalStoragePointer     22
#परिभाषय SYSTEM_CALL_uxTaskGetSystemState                   23
#परिभाषय SYSTEM_CALL_ux_Task_Get_राशिः_उच्चः_Water_Mark            24
#परिभाषय SYSTEM_CALL_ux_Task_Get_राशिः_उच्चः_Water_Mark2           25
#परिभाषय SYSTEM_CALL_xTaskGetCurrentTaskHandle              26
#परिभाषय SYSTEM_CALL_xTaskGetSchedulerState                 27
#परिभाषय SYSTEM_CALL_v_Task_समुच्चयः_Time_बहिर्गमनम्_State                   28
#परिभाषय SYSTEM_CALL_x_Task_Check_For_Time_बहिर्गमनम्                   29
#परिभाषय SYSTEM_CALL_ulTaskGenericNotifyTake                30
#परिभाषय SYSTEM_CALL_xTaskGenericNotifyStateClear           31
#परिभाषय SYSTEM_CALL_ul_Task_Generic_Notify_मानम्_Clear          32
#परिभाषय SYSTEM_CALL_xQueueGenericSend                      33
#परिभाषय SYSTEM_CALL_ux_Queue_Messages_प्रतीक्षा                 34
#परिभाषय SYSTEM_CALL_ux_Queue_Spaces_उपलब्धम्                 35
#परिभाषय SYSTEM_CALL_xQueueReceive                          36
#परिभाषय SYSTEM_CALL_xQueuePeek                             37
#परिभाषय SYSTEM_CALL_xQueueSemaphoreTake                    38
#परिभाषय SYSTEM_CALL_xQueueGetMutexHolder                   39
#परिभाषय SYSTEM_CALL_xQueueTakeMutexRecursive               40
#परिभाषय SYSTEM_CALL_xQueueGiveMutexRecursive               41
#परिभाषय SYSTEM_CALL_x_Queue_Select_From_समुच्चयः                    42
#परिभाषय SYSTEM_CALL_x_Queue_सङ्कलय_गन्तव्यम्_समुच्चयः                         43
#परिभाषय SYSTEM_CALL_v_Queue_सङ्कलय_गन्तव्यम्_Registry                    44
#परिभाषय SYSTEM_CALL_vQueueUnregisterQueue                  45
#परिभाषय SYSTEM_CALL_pcQueueGetName                         46
#परिभाषय SYSTEM_CALL_pvTimerGetTimerID                      47
#परिभाषय SYSTEM_CALL_v_Timer_समुच्चयः_Timer_ID                       48
#परिभाषय SYSTEM_CALL_x_Timer_Is_Timer_सक्रिय                    49
#परिभाषय SYSTEM_CALL_xTimerGetTimerDaemonTaskHandle         50
#परिभाषय SYSTEM_CALL_pcTimerGetName                         51
#परिभाषय SYSTEM_CALL_v_Timer_समुच्चयः_Reload_बहुलकम्                    52
#परिभाषय SYSTEM_CALL_x_Timer_Get_Reload_बहुलकम्                    53
#परिभाषय SYSTEM_CALL_ux_Timer_Get_Reload_बहुलकम्                   54
#परिभाषय SYSTEM_CALL_xTimerGetPeriod                        55
#परिभाषय SYSTEM_CALL_xTimerGetExpiryTime                    56
#परिभाषय SYSTEM_CALL_x_Event_गणः_Clear_द्वयङ्काः                   57
#परिभाषय SYSTEM_CALL_x_Event_गणः_समुच्चयः_द्वयङ्काः                     58
#परिभाषय SYSTEM_CALL_x_Event_गणः_Sync                        59
#परिभाषय SYSTEM_CALL_ux_Event_गणः_Get_सङ्ख्या                  60
#परिभाषय SYSTEM_CALL_v_Event_गणः_समुच्चयः_सङ्ख्या                   61
#परिभाषय SYSTEM_CALL_x_प्रवाहः_बफर_Send                      62
#परिभाषय SYSTEM_CALL_x_प्रवाहः_बफर_Receive                   63
#परिभाषय SYSTEM_CALL_x_प्रवाहः_बफर_Is_पूर्णम्                    64
#परिभाषय SYSTEM_CALL_x_प्रवाहः_बफर_Is_रिक्तानि                   65
#परिभाषय SYSTEM_CALL_x_प्रवाहः_बफर_Spaces_उपलब्धम्           66
#परिभाषय SYSTEM_CALL_x_प्रवाहः_बफर_अष्टकानि_उपलब्धम्            67
#परिभाषय SYSTEM_CALL_x_प्रवाहः_बफर_समुच्चयः_Trigger_Level           68
#परिभाषय SYSTEM_CALL_x_प्रवाहः_बफर_अग्र_सन्देशः_दीर्घता_अष्टकानि    69
#परिभाषय अङ्कः_SYSTEM_CALLS                                   70  /* Total number of system calls. */

#यदि_अन्तः /* MPU_SYSCALL_NUMBERS_H */
