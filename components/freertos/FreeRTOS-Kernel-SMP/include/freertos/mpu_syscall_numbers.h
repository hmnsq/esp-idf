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

#నిర్వచితం_కాకపోతే MPU_SYSCALL_NUMBERS_H
#నిర్వచించు MPU_SYSCALL_NUMBERS_H

/* Numbers assigned to various system calls. */
#నిర్వచించు SYSTEM_CALL_xTaskGenericNotify                     0
#నిర్వచించు SYSTEM_CALL_xTaskGenericNotifyWait                 1
#నిర్వచించు SYSTEM_CALL_xTimerGenericCommandFromTask           2
#నిర్వచించు SYSTEM_CALL_x_Event_సమూహం_Wait_బిట్లు                    3
#నిర్వచించు SYSTEM_CALL_xTaskDelayUntil                        4
#నిర్వచించు SYSTEM_CALL_xTaskAbortDelay                        5
#నిర్వచించు SYSTEM_CALL_vTaskDelay                             6
#నిర్వచించు SYSTEM_CALL_ux_Task_ప్రాధాన్యత_Get                      7
#నిర్వచించు SYSTEM_CALL_eTaskGetState                          8
#నిర్వచించు SYSTEM_CALL_vTaskGetInfo                           9
#నిర్వచించు SYSTEM_CALL_xTaskGetIdleTaskHandle                 10
#నిర్వచించు SYSTEM_CALL_vTaskSuspend                           11
#నిర్వచించు SYSTEM_CALL_vTaskResume                            12
#నిర్వచించు SYSTEM_CALL_x_Task_Get_Tick_లెక్క                      13
#నిర్వచించు SYSTEM_CALL_ux_Task_Get_సంఖ్య_యొక్క_Tasks                 14
#నిర్వచించు SYSTEM_CALL_ul_Task_Get_నిరంతరం_సమయం_Counter                15
#నిర్వచించు SYSTEM_CALL_ul_Task_Get_నిరంతరం_సమయం_Percent                16
#నిర్వచించు SYSTEM_CALL_ul_Task_Get_Idle_నిరంతరం_సమయం_Percent            17
#నిర్వచించు SYSTEM_CALL_ul_Task_Get_Idle_నిరంతరం_సమయం_Counter            18
#నిర్వచించు SYSTEM_CALL_v_Task_సమితి_Application_Task_ట్యాగ్             19
#నిర్వచించు SYSTEM_CALL_x_Task_Get_Application_Task_ట్యాగ్             20
#నిర్వచించు SYSTEM_CALL_v_Task_సమితి_Thread_Local_Storage_సూచిక      21
#నిర్వచించు SYSTEM_CALL_pv_Task_Get_Thread_Local_Storage_సూచిక     22
#నిర్వచించు SYSTEM_CALL_uxTaskGetSystemState                   23
#నిర్వచించు SYSTEM_CALL_ux_Task_Get_దొంతర_ఎగువ_Water_Mark            24
#నిర్వచించు SYSTEM_CALL_ux_Task_Get_దొంతర_ఎగువ_Water_Mark2           25
#నిర్వచించు SYSTEM_CALL_xTaskGetCurrentTaskHandle              26
#నిర్వచించు SYSTEM_CALL_xTaskGetSchedulerState                 27
#నిర్వచించు SYSTEM_CALL_v_Task_సమితి_సమయం_బహిర్గతం_State                   28
#నిర్వచించు SYSTEM_CALL_x_Task_Check_For_సమయం_బహిర్గతం                   29
#నిర్వచించు SYSTEM_CALL_ulTaskGenericNotifyTake                30
#నిర్వచించు SYSTEM_CALL_xTaskGenericNotifyStateClear           31
#నిర్వచించు SYSTEM_CALL_ul_Task_Generic_Notify_విలువ_Clear          32
#నిర్వచించు SYSTEM_CALL_x_వరుస_Generic_Send                      33
#నిర్వచించు SYSTEM_CALL_ux_వరుస_Messages_నిరీక్షణ                 34
#నిర్వచించు SYSTEM_CALL_ux_వరుస_Spaces_అందుబాటు                 35
#నిర్వచించు SYSTEM_CALL_x_వరుస_Receive                          36
#నిర్వచించు SYSTEM_CALL_x_వరుస_Peek                             37
#నిర్వచించు SYSTEM_CALL_x_వరుస_Semaphore_Take                    38
#నిర్వచించు SYSTEM_CALL_x_వరుస_Get_Mutex_Holder                   39
#నిర్వచించు SYSTEM_CALL_x_వరుస_Take_Mutex_Recursive               40
#నిర్వచించు SYSTEM_CALL_x_వరుస_Give_Mutex_Recursive               41
#నిర్వచించు SYSTEM_CALL_x_వరుస_Select_From_సమితి                    42
#నిర్వచించు SYSTEM_CALL_x_వరుస_కూడు_గమ్యం_సమితి                         43
#నిర్వచించు SYSTEM_CALL_v_వరుస_కూడు_గమ్యం_Registry                    44
#నిర్వచించు SYSTEM_CALL_v_వరుస_Unregister_వరుస                  45
#నిర్వచించు SYSTEM_CALL_pc_వరుస_Get_Name                         46
#నిర్వచించు SYSTEM_CALL_pvTimerGetTimerID                      47
#నిర్వచించు SYSTEM_CALL_v_Timer_సమితి_Timer_ID                       48
#నిర్వచించు SYSTEM_CALL_x_Timer_Is_Timer_చురుకైన                    49
#నిర్వచించు SYSTEM_CALL_xTimerGetTimerDaemonTaskHandle         50
#నిర్వచించు SYSTEM_CALL_pcTimerGetName                         51
#నిర్వచించు SYSTEM_CALL_v_Timer_సమితి_Reload_బహుళకం                    52
#నిర్వచించు SYSTEM_CALL_x_Timer_Get_Reload_బహుళకం                    53
#నిర్వచించు SYSTEM_CALL_ux_Timer_Get_Reload_బహుళకం                   54
#నిర్వచించు SYSTEM_CALL_xTimerGetPeriod                        55
#నిర్వచించు SYSTEM_CALL_x_Timer_Get_Expiry_సమయం                    56
#నిర్వచించు SYSTEM_CALL_x_Event_సమూహం_Clear_బిట్లు                   57
#నిర్వచించు SYSTEM_CALL_x_Event_సమూహం_సమితి_బిట్లు                     58
#నిర్వచించు SYSTEM_CALL_x_Event_సమూహం_Sync                        59
#నిర్వచించు SYSTEM_CALL_ux_Event_సమూహం_Get_సంఖ్య                  60
#నిర్వచించు SYSTEM_CALL_v_Event_సమూహం_సమితి_సంఖ్య                   61
#నిర్వచించు SYSTEM_CALL_x_ప్రవాహం_బఫరు_Send                      62
#నిర్వచించు SYSTEM_CALL_x_ప్రవాహం_బఫరు_Receive                   63
#నిర్వచించు SYSTEM_CALL_x_ప్రవాహం_బఫరు_Is_నిండిన                    64
#నిర్వచించు SYSTEM_CALL_x_ప్రవాహం_బఫరు_Is_ఖాళీలు                   65
#నిర్వచించు SYSTEM_CALL_x_ప్రవాహం_బఫరు_Spaces_అందుబాటు           66
#నిర్వచించు SYSTEM_CALL_x_ప్రవాహం_బఫరు_బైట్లు_అందుబాటు            67
#నిర్వచించు SYSTEM_CALL_x_ప్రవాహం_బఫరు_సమితి_Trigger_Level           68
#నిర్వచించు SYSTEM_CALL_x_ప్రవాహం_బఫరు_తదుపరి_సందేశం_నిడివి_బైట్లు    69
#నిర్వచించు అంకె_SYSTEM_CALLS                                   70  /* Total number of system calls. */

#అయితే_ముగింపు /* MPU_SYSCALL_NUMBERS_H */
