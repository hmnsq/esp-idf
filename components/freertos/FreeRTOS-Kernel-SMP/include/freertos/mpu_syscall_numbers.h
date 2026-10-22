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

#که_نه_تعریف_شوی MPU_SYSCALL_NUMBERS_H
#تعریف_کړه MPU_SYSCALL_NUMBERS_H

/* Numbers assigned to various system calls. */
#تعریف_کړه SYSTEM_CALL_xTaskGenericNotify                     0
#تعریف_کړه SYSTEM_CALL_xTaskGenericNotifyWait                 1
#تعریف_کړه SYSTEM_CALL_xTimerGenericCommandFromTask           2
#تعریف_کړه SYSTEM_CALL_x_Event_ډله_Wait_بټونه                    3
#تعریف_کړه SYSTEM_CALL_xTaskDelayUntil                        4
#تعریف_کړه SYSTEM_CALL_xTaskAbortDelay                        5
#تعریف_کړه SYSTEM_CALL_vTaskDelay                             6
#تعریف_کړه SYSTEM_CALL_ux_Task_لومړیتوب_Get                      7
#تعریف_کړه SYSTEM_CALL_eTaskGetState                          8
#تعریف_کړه SYSTEM_CALL_vTaskGetInfo                           9
#تعریف_کړه SYSTEM_CALL_xTaskGetIdleTaskHandle                 10
#تعریف_کړه SYSTEM_CALL_vTaskSuspend                           11
#تعریف_کړه SYSTEM_CALL_vTaskResume                            12
#تعریف_کړه SYSTEM_CALL_x_Task_Get_Tick_شمېر                      13
#تعریف_کړه SYSTEM_CALL_ux_Task_Get_شمېره_د_Tasks                 14
#تعریف_کړه SYSTEM_CALL_ul_Task_Get_پرلپسې_وخت_Counter                15
#تعریف_کړه SYSTEM_CALL_ul_Task_Get_پرلپسې_وخت_Percent                16
#تعریف_کړه SYSTEM_CALL_ul_Task_Get_Idle_پرلپسې_وخت_Percent            17
#تعریف_کړه SYSTEM_CALL_ul_Task_Get_Idle_پرلپسې_وخت_Counter            18
#تعریف_کړه SYSTEM_CALL_v_Task_سېټ_Application_Task_نښان             19
#تعریف_کړه SYSTEM_CALL_x_Task_Get_Application_Task_نښان             20
#تعریف_کړه SYSTEM_CALL_v_Task_سېټ_Thread_Local_Storage_اشاره_ګر      21
#تعریف_کړه SYSTEM_CALL_pv_Task_Get_Thread_Local_Storage_اشاره_ګر     22
#تعریف_کړه SYSTEM_CALL_uxTaskGetSystemState                   23
#تعریف_کړه SYSTEM_CALL_ux_Task_Get_ډېری_لوړ_Water_Mark            24
#تعریف_کړه SYSTEM_CALL_ux_Task_Get_ډېری_لوړ_Water_Mark2           25
#تعریف_کړه SYSTEM_CALL_xTaskGetCurrentTaskHandle              26
#تعریف_کړه SYSTEM_CALL_xTaskGetSchedulerState                 27
#تعریف_کړه SYSTEM_CALL_v_Task_سېټ_وخت_بهر_State                   28
#تعریف_کړه SYSTEM_CALL_x_Task_Check_For_وخت_بهر                   29
#تعریف_کړه SYSTEM_CALL_ulTaskGenericNotifyTake                30
#تعریف_کړه SYSTEM_CALL_xTaskGenericNotifyStateClear           31
#تعریف_کړه SYSTEM_CALL_ul_Task_Generic_Notify_ارزښت_Clear          32
#تعریف_کړه SYSTEM_CALL_x_کتار_Generic_Send                      33
#تعریف_کړه SYSTEM_CALL_ux_کتار_Messages_انتظار                 34
#تعریف_کړه SYSTEM_CALL_ux_کتار_Spaces_شته                 35
#تعریف_کړه SYSTEM_CALL_x_کتار_Receive                          36
#تعریف_کړه SYSTEM_CALL_x_کتار_Peek                             37
#تعریف_کړه SYSTEM_CALL_x_کتار_Semaphore_Take                    38
#تعریف_کړه SYSTEM_CALL_x_کتار_Get_Mutex_Holder                   39
#تعریف_کړه SYSTEM_CALL_x_کتار_Take_Mutex_Recursive               40
#تعریف_کړه SYSTEM_CALL_x_کتار_Give_Mutex_Recursive               41
#تعریف_کړه SYSTEM_CALL_x_کتار_Select_From_سېټ                    42
#تعریف_کړه SYSTEM_CALL_x_کتار_جمع_کړه_منزل_سېټ                         43
#تعریف_کړه SYSTEM_CALL_v_کتار_جمع_کړه_منزل_Registry                    44
#تعریف_کړه SYSTEM_CALL_v_کتار_Unregister_کتار                  45
#تعریف_کړه SYSTEM_CALL_pc_کتار_Get_Name                         46
#تعریف_کړه SYSTEM_CALL_pvTimerGetTimerID                      47
#تعریف_کړه SYSTEM_CALL_v_Timer_سېټ_Timer_ID                       48
#تعریف_کړه SYSTEM_CALL_x_Timer_Is_Timer_فعال                    49
#تعریف_کړه SYSTEM_CALL_xTimerGetTimerDaemonTaskHandle         50
#تعریف_کړه SYSTEM_CALL_pcTimerGetName                         51
#تعریف_کړه SYSTEM_CALL_v_Timer_سېټ_Reload_ډېرپېښ                    52
#تعریف_کړه SYSTEM_CALL_x_Timer_Get_Reload_ډېرپېښ                    53
#تعریف_کړه SYSTEM_CALL_ux_Timer_Get_Reload_ډېرپېښ                   54
#تعریف_کړه SYSTEM_CALL_xTimerGetPeriod                        55
#تعریف_کړه SYSTEM_CALL_x_Timer_Get_Expiry_وخت                    56
#تعریف_کړه SYSTEM_CALL_x_Event_ډله_Clear_بټونه                   57
#تعریف_کړه SYSTEM_CALL_x_Event_ډله_سېټ_بټونه                     58
#تعریف_کړه SYSTEM_CALL_x_Event_ډله_Sync                        59
#تعریف_کړه SYSTEM_CALL_ux_Event_ډله_Get_شمېره                  60
#تعریف_کړه SYSTEM_CALL_v_Event_ډله_سېټ_شمېره                   61
#تعریف_کړه SYSTEM_CALL_x_بهیر_بفر_Send                      62
#تعریف_کړه SYSTEM_CALL_x_بهیر_بفر_Receive                   63
#تعریف_کړه SYSTEM_CALL_x_بهیر_بفر_Is_ډک                    64
#تعریف_کړه SYSTEM_CALL_x_بهیر_بفر_Is_تشه                   65
#تعریف_کړه SYSTEM_CALL_x_بهیر_بفر_Spaces_شته           66
#تعریف_کړه SYSTEM_CALL_x_بهیر_بفر_بایټونه_شته            67
#تعریف_کړه SYSTEM_CALL_x_بهیر_بفر_سېټ_Trigger_Level           68
#تعریف_کړه SYSTEM_CALL_x_بهیر_بفر_بل_پیغام_اوږدتیا_بایټونه    69
#تعریف_کړه عدد_SYSTEM_CALLS                                   70  /* Total number of system calls. */

#د_که_پای /* MPU_SYSCALL_NUMBERS_H */
