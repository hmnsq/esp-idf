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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ MPU_SYSCALL_NUMBERS_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MPU_SYSCALL_NUMBERS_H

/* Numbers assigned to various system calls. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_xTaskGenericNotify                     0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_xTaskGenericNotifyWait                 1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_xTimerGenericCommandFromTask           2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_x_Event_ਸਮੂਹ_Wait_ਬਿੱਟ                    3
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_xTaskDelayUntil                        4
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_xTaskAbortDelay                        5
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_vTaskDelay                             6
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_ux_Task_ਤਰਜੀਹ_Get                      7
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_eTaskGetState                          8
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_vTaskGetInfo                           9
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_xTaskGetIdleTaskHandle                 10
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_vTaskSuspend                           11
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_vTaskResume                            12
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_x_Task_Get_Tick_ਗਿਣਤੀ                      13
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_ux_Task_Get_ਸੰਖਿਆ_ਦਾ_Tasks                 14
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_ul_Task_Get_ਲੜੀਵਾਰ_ਸਮਾਂ_Counter                15
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_ul_Task_Get_ਲੜੀਵਾਰ_ਸਮਾਂ_Percent                16
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_ul_Task_Get_Idle_ਲੜੀਵਾਰ_ਸਮਾਂ_Percent            17
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_ul_Task_Get_Idle_ਲੜੀਵਾਰ_ਸਮਾਂ_Counter            18
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_v_Task_ਸੈੱਟ_Application_Task_ਟੈਗ             19
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_x_Task_Get_Application_Task_ਟੈਗ             20
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_v_Task_ਸੈੱਟ_Thread_Local_Storage_ਸੂਚਕ      21
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_pv_Task_Get_Thread_Local_Storage_ਸੂਚਕ     22
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_uxTaskGetSystemState                   23
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_ux_Task_Get_ਢੇਰ_ਉੱਚਾ_Water_Mark            24
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_ux_Task_Get_ਢੇਰ_ਉੱਚਾ_Water_Mark2           25
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_xTaskGetCurrentTaskHandle              26
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_xTaskGetSchedulerState                 27
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_v_Task_ਸੈੱਟ_ਸਮਾਂ_ਬਾਹਰਲਾ_State                   28
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_x_Task_Check_For_ਸਮਾਂ_ਬਾਹਰਲਾ                   29
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_ulTaskGenericNotifyTake                30
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_xTaskGenericNotifyStateClear           31
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_ul_Task_Generic_Notify_ਮੁੱਲ_Clear          32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_x_ਕਤਾਰ_Generic_Send                      33
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_ux_ਕਤਾਰ_Messages_ਉਡੀਕ                 34
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_ux_ਕਤਾਰ_Spaces_ਉਪਲਬਧ                 35
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_x_ਕਤਾਰ_Receive                          36
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_x_ਕਤਾਰ_Peek                             37
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_x_ਕਤਾਰ_Semaphore_Take                    38
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_x_ਕਤਾਰ_Get_Mutex_Holder                   39
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_x_ਕਤਾਰ_Take_Mutex_Recursive               40
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_x_ਕਤਾਰ_Give_Mutex_Recursive               41
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_x_ਕਤਾਰ_Select_From_ਸੈੱਟ                    42
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_x_ਕਤਾਰ_ਜੋੜੋ_ਮੰਜ਼ਿਲ_ਸੈੱਟ                         43
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_v_ਕਤਾਰ_ਜੋੜੋ_ਮੰਜ਼ਿਲ_Registry                    44
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_v_ਕਤਾਰ_Unregister_ਕਤਾਰ                  45
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_pc_ਕਤਾਰ_Get_Name                         46
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_pvTimerGetTimerID                      47
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_v_Timer_ਸੈੱਟ_Timer_ID                       48
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_x_Timer_Is_Timer_ਸਰਗਰਮ                    49
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_xTimerGetTimerDaemonTaskHandle         50
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_pcTimerGetName                         51
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_v_Timer_ਸੈੱਟ_Reload_ਬਹੁਲਕ                    52
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_x_Timer_Get_Reload_ਬਹੁਲਕ                    53
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_ux_Timer_Get_Reload_ਬਹੁਲਕ                   54
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_xTimerGetPeriod                        55
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_x_Timer_Get_Expiry_ਸਮਾਂ                    56
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_x_Event_ਸਮੂਹ_Clear_ਬਿੱਟ                   57
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_x_Event_ਸਮੂਹ_ਸੈੱਟ_ਬਿੱਟ                     58
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_x_Event_ਸਮੂਹ_Sync                        59
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_ux_Event_ਸਮੂਹ_Get_ਸੰਖਿਆ                  60
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_v_Event_ਸਮੂਹ_ਸੈੱਟ_ਸੰਖਿਆ                   61
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_x_ਧਾਰਾ_ਬਫਰ_Send                      62
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_x_ਧਾਰਾ_ਬਫਰ_Receive                   63
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_x_ਧਾਰਾ_ਬਫਰ_Is_ਭਰਿਆ                    64
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_x_ਧਾਰਾ_ਬਫਰ_Is_ਖਾਲੀ_ਥਾਵਾਂ                   65
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_x_ਧਾਰਾ_ਬਫਰ_Spaces_ਉਪਲਬਧ           66
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_x_ਧਾਰਾ_ਬਫਰ_ਬਾਈਟਾਂ_ਉਪਲਬਧ            67
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_x_ਧਾਰਾ_ਬਫਰ_ਸੈੱਟ_Trigger_Level           68
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SYSTEM_CALL_x_ਧਾਰਾ_ਬਫਰ_ਅਗਲਾ_ਸੁਨੇਹਾ_ਲੰਮਾਈ_ਬਾਈਟਾਂ    69
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਅੰਕ_SYSTEM_CALLS                                   70  /* Total number of system calls. */

#ਜੇ_ਅੰਤ /* MPU_SYSCALL_NUMBERS_H */
