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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ MPU_SYSCALL_NUMBERS_H
#ವ್ಯಾಖ್ಯಾನಿಸು MPU_SYSCALL_NUMBERS_H

/* Numbers assigned to various system calls. */
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_xTaskGenericNotify                     0
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_xTaskGenericNotifyWait                 1
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_xTimerGenericCommandFromTask           2
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_x_Event_ಗುಂಪು_Wait_ಬಿಟ್ಟುಗಳು                    3
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_xTaskDelayUntil                        4
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_xTaskAbortDelay                        5
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_vTaskDelay                             6
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_ux_Task_ಆದ್ಯತೆ_Get                      7
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_eTaskGetState                          8
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_vTaskGetInfo                           9
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_xTaskGetIdleTaskHandle                 10
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_vTaskSuspend                           11
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_vTaskResume                            12
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_x_Task_Get_Tick_ಎಣಿಕೆ                      13
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_ux_Task_Get_ಸಂಖ್ಯೆ_ದ_Tasks                 14
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_ul_Task_Get_ಸತತ_ಸಮಯ_Counter                15
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_ul_Task_Get_ಸತತ_ಸಮಯ_Percent                16
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_ul_Task_Get_Idle_ಸತತ_ಸಮಯ_Percent            17
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_ul_Task_Get_Idle_ಸತತ_ಸಮಯ_Counter            18
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_v_Task_ಗಣ_Application_Task_ಟ್ಯಾಗ್             19
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_x_Task_Get_Application_Task_ಟ್ಯಾಗ್             20
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_v_Task_ಗಣ_Thread_Local_Storage_ಸೂಚಕ      21
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_pv_Task_Get_Thread_Local_Storage_ಸೂಚಕ     22
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_uxTaskGetSystemState                   23
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_ux_Task_Get_ರಾಶಿ_ಮೇಲಿನ_Water_Mark            24
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_ux_Task_Get_ರಾಶಿ_ಮೇಲಿನ_Water_Mark2           25
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_xTaskGetCurrentTaskHandle              26
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_xTaskGetSchedulerState                 27
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_v_Task_ಗಣ_ಸಮಯ_ಹೊರಹೋಗು_State                   28
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_x_Task_Check_For_ಸಮಯ_ಹೊರಹೋಗು                   29
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_ulTaskGenericNotifyTake                30
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_xTaskGenericNotifyStateClear           31
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_ul_Task_Generic_Notify_ಮೌಲ್ಯ_Clear          32
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_x_ಸರತಿ_Generic_Send                      33
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_ux_ಸರತಿ_Messages_ಕಾಯುವಿಕೆ                 34
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_ux_ಸರತಿ_Spaces_ಲಭ್ಯ                 35
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_x_ಸರತಿ_Receive                          36
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_x_ಸರತಿ_Peek                             37
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_x_ಸರತಿ_Semaphore_Take                    38
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_x_ಸರತಿ_Get_Mutex_Holder                   39
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_x_ಸರತಿ_Take_Mutex_Recursive               40
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_x_ಸರತಿ_Give_Mutex_Recursive               41
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_x_ಸರತಿ_Select_From_ಗಣ                    42
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_x_ಸರತಿ_ಕೂಡಿಸು_ಗಮ್ಯ_ಗಣ                         43
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_v_ಸರತಿ_ಕೂಡಿಸು_ಗಮ್ಯ_Registry                    44
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_v_ಸರತಿ_Unregister_ಸರತಿ                  45
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_pc_ಸರತಿ_Get_Name                         46
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_pvTimerGetTimerID                      47
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_v_Timer_ಗಣ_Timer_ID                       48
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_x_Timer_Is_Timer_ಸಕ್ರಿಯ                    49
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_xTimerGetTimerDaemonTaskHandle         50
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_pcTimerGetName                         51
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_v_Timer_ಗಣ_Reload_ಬಹುಲಕ                    52
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_x_Timer_Get_Reload_ಬಹುಲಕ                    53
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_ux_Timer_Get_Reload_ಬಹುಲಕ                   54
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_xTimerGetPeriod                        55
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_x_Timer_Get_Expiry_ಸಮಯ                    56
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_x_Event_ಗುಂಪು_Clear_ಬಿಟ್ಟುಗಳು                   57
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_x_Event_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು                     58
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_x_Event_ಗುಂಪು_Sync                        59
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_ux_Event_ಗುಂಪು_Get_ಸಂಖ್ಯೆ                  60
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_v_Event_ಗುಂಪು_ಗಣ_ಸಂಖ್ಯೆ                   61
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_x_ಹರಿವು_ಬಫರ್_Send                      62
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_x_ಹರಿವು_ಬಫರ್_Receive                   63
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_x_ಹರಿವು_ಬಫರ್_Is_ತುಂಬಿದ                    64
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_x_ಹರಿವು_ಬಫರ್_Is_ಖಾಲಿಗಳು                   65
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_x_ಹರಿವು_ಬಫರ್_Spaces_ಲಭ್ಯ           66
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_x_ಹರಿವು_ಬಫರ್_ಬೈಟುಗಳು_ಲಭ್ಯ            67
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_x_ಹರಿವು_ಬಫರ್_ಗಣ_Trigger_Level           68
#ವ್ಯಾಖ್ಯಾನಿಸು SYSTEM_CALL_x_ಹರಿವು_ಬಫರ್_ಮುಂದಿನ_ಸಂದೇಶ_ಉದ್ದಳತೆ_ಬೈಟುಗಳು    69
#ವ್ಯಾಖ್ಯಾನಿಸು ಅಂಕೆ_SYSTEM_CALLS                                   70  /* Total number of system calls. */

#ಆದರೆ_ಅಂತ್ಯ /* MPU_SYSCALL_NUMBERS_H */
