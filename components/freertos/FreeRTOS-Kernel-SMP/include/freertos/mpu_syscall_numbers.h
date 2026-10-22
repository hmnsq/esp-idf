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

#اگر_تعریف_نشده MPU_SYSCALL_NUMBERS_H
#تعریف_کن MPU_SYSCALL_NUMBERS_H

/* Numbers assigned to various system calls. */
#تعریف_کن SYSTEM_CALL_xTaskGenericNotify                     0
#تعریف_کن SYSTEM_CALL_xTaskGenericNotifyWait                 1
#تعریف_کن SYSTEM_CALL_xTimerGenericCommandFromTask           2
#تعریف_کن SYSTEM_CALL_x_Event_گروه_Wait_بیتها                    3
#تعریف_کن SYSTEM_CALL_xTaskDelayUntil                        4
#تعریف_کن SYSTEM_CALL_xTaskAbortDelay                        5
#تعریف_کن SYSTEM_CALL_vTaskDelay                             6
#تعریف_کن SYSTEM_CALL_ux_Task_اولویت_Get                      7
#تعریف_کن SYSTEM_CALL_eTaskGetState                          8
#تعریف_کن SYSTEM_CALL_vTaskGetInfo                           9
#تعریف_کن SYSTEM_CALL_xTaskGetIdleTaskHandle                 10
#تعریف_کن SYSTEM_CALL_vTaskSuspend                           11
#تعریف_کن SYSTEM_CALL_vTaskResume                            12
#تعریف_کن SYSTEM_CALL_x_Task_Get_Tick_شمار                      13
#تعریف_کن SYSTEM_CALL_ux_Task_Get_عدد_از_Tasks                 14
#تعریف_کن SYSTEM_CALL_ul_Task_Get_پیاپی_زمان_Counter                15
#تعریف_کن SYSTEM_CALL_ul_Task_Get_پیاپی_زمان_Percent                16
#تعریف_کن SYSTEM_CALL_ul_Task_Get_Idle_پیاپی_زمان_Percent            17
#تعریف_کن SYSTEM_CALL_ul_Task_Get_Idle_پیاپی_زمان_Counter            18
#تعریف_کن SYSTEM_CALL_v_Task_مجموعه_Application_Task_برچسب             19
#تعریف_کن SYSTEM_CALL_x_Task_Get_Application_Task_برچسب             20
#تعریف_کن SYSTEM_CALL_v_Task_مجموعه_Thread_Local_Storage_اشاره_گر      21
#تعریف_کن SYSTEM_CALL_pv_Task_Get_Thread_Local_Storage_اشاره_گر     22
#تعریف_کن SYSTEM_CALL_uxTaskGetSystemState                   23
#تعریف_کن SYSTEM_CALL_ux_Task_Get_پشته_بالا_Water_Mark            24
#تعریف_کن SYSTEM_CALL_ux_Task_Get_پشته_بالا_Water_Mark2           25
#تعریف_کن SYSTEM_CALL_xTaskGetCurrentTaskHandle              26
#تعریف_کن SYSTEM_CALL_xTaskGetSchedulerState                 27
#تعریف_کن SYSTEM_CALL_v_Task_مجموعه_زمان_بیرون_State                   28
#تعریف_کن SYSTEM_CALL_x_Task_Check_For_زمان_بیرون                   29
#تعریف_کن SYSTEM_CALL_ulTaskGenericNotifyTake                30
#تعریف_کن SYSTEM_CALL_xTaskGenericNotifyStateClear           31
#تعریف_کن SYSTEM_CALL_ul_Task_Generic_Notify_مقدار_Clear          32
#تعریف_کن SYSTEM_CALL_x_صف_Generic_Send                      33
#تعریف_کن SYSTEM_CALL_ux_صف_Messages_انتظار                 34
#تعریف_کن SYSTEM_CALL_ux_صف_Spaces_موجود                 35
#تعریف_کن SYSTEM_CALL_x_صف_Receive                          36
#تعریف_کن SYSTEM_CALL_x_صف_Peek                             37
#تعریف_کن SYSTEM_CALL_x_صف_Semaphore_Take                    38
#تعریف_کن SYSTEM_CALL_x_صف_Get_Mutex_Holder                   39
#تعریف_کن SYSTEM_CALL_x_صف_Take_Mutex_Recursive               40
#تعریف_کن SYSTEM_CALL_x_صف_Give_Mutex_Recursive               41
#تعریف_کن SYSTEM_CALL_x_صف_Select_From_مجموعه                    42
#تعریف_کن SYSTEM_CALL_x_صف_جمع_کن_مقصد_مجموعه                         43
#تعریف_کن SYSTEM_CALL_v_صف_جمع_کن_مقصد_Registry                    44
#تعریف_کن SYSTEM_CALL_v_صف_Unregister_صف                  45
#تعریف_کن SYSTEM_CALL_pc_صف_Get_Name                         46
#تعریف_کن SYSTEM_CALL_pvTimerGetTimerID                      47
#تعریف_کن SYSTEM_CALL_v_Timer_مجموعه_Timer_ID                       48
#تعریف_کن SYSTEM_CALL_x_Timer_Is_Timer_فعال                    49
#تعریف_کن SYSTEM_CALL_xTimerGetTimerDaemonTaskHandle         50
#تعریف_کن SYSTEM_CALL_pcTimerGetName                         51
#تعریف_کن SYSTEM_CALL_v_Timer_مجموعه_Reload_نما                    52
#تعریف_کن SYSTEM_CALL_x_Timer_Get_Reload_نما                    53
#تعریف_کن SYSTEM_CALL_ux_Timer_Get_Reload_نما                   54
#تعریف_کن SYSTEM_CALL_xTimerGetPeriod                        55
#تعریف_کن SYSTEM_CALL_x_Timer_Get_Expiry_زمان                    56
#تعریف_کن SYSTEM_CALL_x_Event_گروه_Clear_بیتها                   57
#تعریف_کن SYSTEM_CALL_x_Event_گروه_مجموعه_بیتها                     58
#تعریف_کن SYSTEM_CALL_x_Event_گروه_Sync                        59
#تعریف_کن SYSTEM_CALL_ux_Event_گروه_Get_عدد                  60
#تعریف_کن SYSTEM_CALL_v_Event_گروه_مجموعه_عدد                   61
#تعریف_کن SYSTEM_CALL_x_جریان_میانگیر_Send                      62
#تعریف_کن SYSTEM_CALL_x_جریان_میانگیر_Receive                   63
#تعریف_کن SYSTEM_CALL_x_جریان_میانگیر_Is_پر                    64
#تعریف_کن SYSTEM_CALL_x_جریان_میانگیر_Is_خالی                   65
#تعریف_کن SYSTEM_CALL_x_جریان_میانگیر_Spaces_موجود           66
#تعریف_کن SYSTEM_CALL_x_جریان_میانگیر_بایتها_موجود            67
#تعریف_کن SYSTEM_CALL_x_جریان_میانگیر_مجموعه_Trigger_Level           68
#تعریف_کن SYSTEM_CALL_x_جریان_میانگیر_بعدی_پیام_درازی_بایتها    69
#تعریف_کن رقم_SYSTEM_CALLS                                   70  /* Total number of system calls. */

#پایان_اگر /* MPU_SYSCALL_NUMBERS_H */
