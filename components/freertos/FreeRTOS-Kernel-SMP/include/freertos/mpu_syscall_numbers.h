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

#جيڪڏهن_وصف_نه_ٿيل MPU_SYSCALL_NUMBERS_H
#وصف_ڪريو MPU_SYSCALL_NUMBERS_H

/* Numbers assigned to various system calls. */
#وصف_ڪريو SYSTEM_CALL_xTaskGenericNotify                     0
#وصف_ڪريو SYSTEM_CALL_xTaskGenericNotifyWait                 1
#وصف_ڪريو SYSTEM_CALL_xTimerGenericCommandFromTask           2
#وصف_ڪريو SYSTEM_CALL_x_Event_گروهه_Wait_بٽ                    3
#وصف_ڪريو SYSTEM_CALL_xTaskDelayUntil                        4
#وصف_ڪريو SYSTEM_CALL_xTaskAbortDelay                        5
#وصف_ڪريو SYSTEM_CALL_vTaskDelay                             6
#وصف_ڪريو SYSTEM_CALL_ux_Task_ترجيح_Get                      7
#وصف_ڪريو SYSTEM_CALL_eTaskGetState                          8
#وصف_ڪريو SYSTEM_CALL_vTaskGetInfo                           9
#وصف_ڪريو SYSTEM_CALL_xTaskGetIdleTaskHandle                 10
#وصف_ڪريو SYSTEM_CALL_vTaskSuspend                           11
#وصف_ڪريو SYSTEM_CALL_vTaskResume                            12
#وصف_ڪريو SYSTEM_CALL_x_Task_Get_Tick_ڳڻپ                      13
#وصف_ڪريو SYSTEM_CALL_ux_Task_Get_عدد_جو_Tasks                 14
#وصف_ڪريو SYSTEM_CALL_ul_Task_Get_لاڳيتو_وقت_Counter                15
#وصف_ڪريو SYSTEM_CALL_ul_Task_Get_لاڳيتو_وقت_Percent                16
#وصف_ڪريو SYSTEM_CALL_ul_Task_Get_Idle_لاڳيتو_وقت_Percent            17
#وصف_ڪريو SYSTEM_CALL_ul_Task_Get_Idle_لاڳيتو_وقت_Counter            18
#وصف_ڪريو SYSTEM_CALL_v_Task_سيٽ_Application_Task_ٽيگ             19
#وصف_ڪريو SYSTEM_CALL_x_Task_Get_Application_Task_ٽيگ             20
#وصف_ڪريو SYSTEM_CALL_v_Task_سيٽ_Thread_Local_Storage_اشارو      21
#وصف_ڪريو SYSTEM_CALL_pv_Task_Get_Thread_Local_Storage_اشارو     22
#وصف_ڪريو SYSTEM_CALL_uxTaskGetSystemState                   23
#وصف_ڪريو SYSTEM_CALL_ux_Task_Get_ڍير_مٿاهون_Water_Mark            24
#وصف_ڪريو SYSTEM_CALL_ux_Task_Get_ڍير_مٿاهون_Water_Mark2           25
#وصف_ڪريو SYSTEM_CALL_xTaskGetCurrentTaskHandle              26
#وصف_ڪريو SYSTEM_CALL_xTaskGetSchedulerState                 27
#وصف_ڪريو SYSTEM_CALL_v_Task_سيٽ_وقت_ٻاهريون_State                   28
#وصف_ڪريو SYSTEM_CALL_x_Task_Check_For_وقت_ٻاهريون                   29
#وصف_ڪريو SYSTEM_CALL_ulTaskGenericNotifyTake                30
#وصف_ڪريو SYSTEM_CALL_xTaskGenericNotifyStateClear           31
#وصف_ڪريو SYSTEM_CALL_ul_Task_Generic_Notify_قدر_Clear          32
#وصف_ڪريو SYSTEM_CALL_x_قطار_Generic_Send                      33
#وصف_ڪريو SYSTEM_CALL_ux_قطار_Messages_انتظار                 34
#وصف_ڪريو SYSTEM_CALL_ux_قطار_Spaces_موجود                 35
#وصف_ڪريو SYSTEM_CALL_x_قطار_Receive                          36
#وصف_ڪريو SYSTEM_CALL_x_قطار_Peek                             37
#وصف_ڪريو SYSTEM_CALL_x_قطار_Semaphore_Take                    38
#وصف_ڪريو SYSTEM_CALL_x_قطار_Get_Mutex_Holder                   39
#وصف_ڪريو SYSTEM_CALL_x_قطار_Take_Mutex_Recursive               40
#وصف_ڪريو SYSTEM_CALL_x_قطار_Give_Mutex_Recursive               41
#وصف_ڪريو SYSTEM_CALL_x_قطار_Select_From_سيٽ                    42
#وصف_ڪريو SYSTEM_CALL_x_قطار_جوڙيو_منزل_سيٽ                         43
#وصف_ڪريو SYSTEM_CALL_v_قطار_جوڙيو_منزل_Registry                    44
#وصف_ڪريو SYSTEM_CALL_v_قطار_Unregister_قطار                  45
#وصف_ڪريو SYSTEM_CALL_pc_قطار_Get_Name                         46
#وصف_ڪريو SYSTEM_CALL_pvTimerGetTimerID                      47
#وصف_ڪريو SYSTEM_CALL_v_Timer_سيٽ_Timer_ID                       48
#وصف_ڪريو SYSTEM_CALL_x_Timer_Is_Timer_سرگرم                    49
#وصف_ڪريو SYSTEM_CALL_xTimerGetTimerDaemonTaskHandle         50
#وصف_ڪريو SYSTEM_CALL_pcTimerGetName                         51
#وصف_ڪريو SYSTEM_CALL_v_Timer_سيٽ_Reload_گهڻائي                    52
#وصف_ڪريو SYSTEM_CALL_x_Timer_Get_Reload_گهڻائي                    53
#وصف_ڪريو SYSTEM_CALL_ux_Timer_Get_Reload_گهڻائي                   54
#وصف_ڪريو SYSTEM_CALL_xTimerGetPeriod                        55
#وصف_ڪريو SYSTEM_CALL_x_Timer_Get_Expiry_وقت                    56
#وصف_ڪريو SYSTEM_CALL_x_Event_گروهه_Clear_بٽ                   57
#وصف_ڪريو SYSTEM_CALL_x_Event_گروهه_سيٽ_بٽ                     58
#وصف_ڪريو SYSTEM_CALL_x_Event_گروهه_Sync                        59
#وصف_ڪريو SYSTEM_CALL_ux_Event_گروهه_Get_عدد                  60
#وصف_ڪريو SYSTEM_CALL_v_Event_گروهه_سيٽ_عدد                   61
#وصف_ڪريو SYSTEM_CALL_x_وهڪرو_بفر_Send                      62
#وصف_ڪريو SYSTEM_CALL_x_وهڪرو_بفر_Receive                   63
#وصف_ڪريو SYSTEM_CALL_x_وهڪرو_بفر_Is_ڀريل                    64
#وصف_ڪريو SYSTEM_CALL_x_وهڪرو_بفر_Is_خالي_جايون                   65
#وصف_ڪريو SYSTEM_CALL_x_وهڪرو_بفر_Spaces_موجود           66
#وصف_ڪريو SYSTEM_CALL_x_وهڪرو_بفر_بائيٽون_موجود            67
#وصف_ڪريو SYSTEM_CALL_x_وهڪرو_بفر_سيٽ_Trigger_Level           68
#وصف_ڪريو SYSTEM_CALL_x_وهڪرو_بفر_ايندڙ_پيغام_ڊيگهائي_بائيٽون    69
#وصف_ڪريو انگ_SYSTEM_CALLS                                   70  /* Total number of system calls. */

#جيڪڏهن_پورو /* MPU_SYSCALL_NUMBERS_H */
