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

#جے_بنا_تعریف MPU_SYSCALL_NUMBERS_H
#تعریف_کرو MPU_SYSCALL_NUMBERS_H

/* Numbers assigned to various system calls. */
#تعریف_کرو SYSTEM_CALL_xTaskGenericNotify                     0
#تعریف_کرو SYSTEM_CALL_xTaskGenericNotifyWait                 1
#تعریف_کرو SYSTEM_CALL_xTimerGenericCommandFromTask           2
#تعریف_کرو SYSTEM_CALL_x_Event_گروہ_Wait_بٹ                    3
#تعریف_کرو SYSTEM_CALL_xTaskDelayUntil                        4
#تعریف_کرو SYSTEM_CALL_xTaskAbortDelay                        5
#تعریف_کرو SYSTEM_CALL_vTaskDelay                             6
#تعریف_کرو SYSTEM_CALL_ux_Task_ترجیح_Get                      7
#تعریف_کرو SYSTEM_CALL_eTaskGetState                          8
#تعریف_کرو SYSTEM_CALL_vTaskGetInfo                           9
#تعریف_کرو SYSTEM_CALL_xTaskGetIdleTaskHandle                 10
#تعریف_کرو SYSTEM_CALL_vTaskSuspend                           11
#تعریف_کرو SYSTEM_CALL_vTaskResume                            12
#تعریف_کرو SYSTEM_CALL_x_Task_Get_Tick_گنتی                      13
#تعریف_کرو SYSTEM_CALL_ux_Task_Get_نمبر_دا_Tasks                 14
#تعریف_کرو SYSTEM_CALL_ul_Task_Get_لگاتار_ویلا_Counter                15
#تعریف_کرو SYSTEM_CALL_ul_Task_Get_لگاتار_ویلا_Percent                16
#تعریف_کرو SYSTEM_CALL_ul_Task_Get_Idle_لگاتار_ویلا_Percent            17
#تعریف_کرو SYSTEM_CALL_ul_Task_Get_Idle_لگاتار_ویلا_Counter            18
#تعریف_کرو SYSTEM_CALL_v_Task_سیٹ_Application_Task_ٹیگ             19
#تعریف_کرو SYSTEM_CALL_x_Task_Get_Application_Task_ٹیگ             20
#تعریف_کرو SYSTEM_CALL_v_Task_سیٹ_Thread_Local_Storage_اشارہ_گر      21
#تعریف_کرو SYSTEM_CALL_pv_Task_Get_Thread_Local_Storage_اشارہ_گر     22
#تعریف_کرو SYSTEM_CALL_uxTaskGetSystemState                   23
#تعریف_کرو SYSTEM_CALL_ux_Task_Get_ڈھیر_بلند_Water_Mark            24
#تعریف_کرو SYSTEM_CALL_ux_Task_Get_ڈھیر_بلند_Water_Mark2           25
#تعریف_کرو SYSTEM_CALL_xTaskGetCurrentTaskHandle              26
#تعریف_کرو SYSTEM_CALL_xTaskGetSchedulerState                 27
#تعریف_کرو SYSTEM_CALL_v_Task_سیٹ_ویلا_باہرلا_State                   28
#تعریف_کرو SYSTEM_CALL_x_Task_Check_For_ویلا_باہرلا                   29
#تعریف_کرو SYSTEM_CALL_ulTaskGenericNotifyTake                30
#تعریف_کرو SYSTEM_CALL_xTaskGenericNotifyStateClear           31
#تعریف_کرو SYSTEM_CALL_ul_Task_Generic_Notify_قدر_Clear          32
#تعریف_کرو SYSTEM_CALL_x_قطار_Generic_Send                      33
#تعریف_کرو SYSTEM_CALL_ux_قطار_Messages_اڈیک                 34
#تعریف_کرو SYSTEM_CALL_ux_قطار_Spaces_دستیاب                 35
#تعریف_کرو SYSTEM_CALL_x_قطار_Receive                          36
#تعریف_کرو SYSTEM_CALL_x_قطار_Peek                             37
#تعریف_کرو SYSTEM_CALL_x_قطار_Semaphore_Take                    38
#تعریف_کرو SYSTEM_CALL_x_قطار_Get_Mutex_Holder                   39
#تعریف_کرو SYSTEM_CALL_x_قطار_Take_Mutex_Recursive               40
#تعریف_کرو SYSTEM_CALL_x_قطار_Give_Mutex_Recursive               41
#تعریف_کرو SYSTEM_CALL_x_قطار_Select_From_سیٹ                    42
#تعریف_کرو SYSTEM_CALL_x_قطار_جوڑو_منزل_سیٹ                         43
#تعریف_کرو SYSTEM_CALL_v_قطار_جوڑو_منزل_Registry                    44
#تعریف_کرو SYSTEM_CALL_v_قطار_Unregister_قطار                  45
#تعریف_کرو SYSTEM_CALL_pc_قطار_Get_Name                         46
#تعریف_کرو SYSTEM_CALL_pvTimerGetTimerID                      47
#تعریف_کرو SYSTEM_CALL_v_Timer_سیٹ_Timer_ID                       48
#تعریف_کرو SYSTEM_CALL_x_Timer_Is_Timer_سرگرم                    49
#تعریف_کرو SYSTEM_CALL_xTimerGetTimerDaemonTaskHandle         50
#تعریف_کرو SYSTEM_CALL_pcTimerGetName                         51
#تعریف_کرو SYSTEM_CALL_v_Timer_سیٹ_Reload_منوال                    52
#تعریف_کرو SYSTEM_CALL_x_Timer_Get_Reload_منوال                    53
#تعریف_کرو SYSTEM_CALL_ux_Timer_Get_Reload_منوال                   54
#تعریف_کرو SYSTEM_CALL_xTimerGetPeriod                        55
#تعریف_کرو SYSTEM_CALL_x_Timer_Get_Expiry_ویلا                    56
#تعریف_کرو SYSTEM_CALL_x_Event_گروہ_Clear_بٹ                   57
#تعریف_کرو SYSTEM_CALL_x_Event_گروہ_سیٹ_بٹ                     58
#تعریف_کرو SYSTEM_CALL_x_Event_گروہ_Sync                        59
#تعریف_کرو SYSTEM_CALL_ux_Event_گروہ_Get_نمبر                  60
#تعریف_کرو SYSTEM_CALL_v_Event_گروہ_سیٹ_نمبر                   61
#تعریف_کرو SYSTEM_CALL_x_دھارا_بفر_Send                      62
#تعریف_کرو SYSTEM_CALL_x_دھارا_بفر_Receive                   63
#تعریف_کرو SYSTEM_CALL_x_دھارا_بفر_Is_بھریا                    64
#تعریف_کرو SYSTEM_CALL_x_دھارا_بفر_Is_خالی_تھاواں                   65
#تعریف_کرو SYSTEM_CALL_x_دھارا_بفر_Spaces_دستیاب           66
#تعریف_کرو SYSTEM_CALL_x_دھارا_بفر_بائٹاں_دستیاب            67
#تعریف_کرو SYSTEM_CALL_x_دھارا_بفر_سیٹ_Trigger_Level           68
#تعریف_کرو SYSTEM_CALL_x_دھارا_بفر_اگلا_سنیہا_لمائی_بائٹاں    69
#تعریف_کرو عدد_SYSTEM_CALLS                                   70  /* Total number of system calls. */

#جے_انت /* MPU_SYSCALL_NUMBERS_H */
