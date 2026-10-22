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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ MPU_WRAPPERS_H
#ವ್ಯಾಖ್ಯಾನಿಸು MPU_WRAPPERS_H

/* This file redefines API functions to be called through a wrapper macro, but
 * only for ports that are using the MPU. */
#ಆದರೆ ( ಪೋರ್ಟ್_USING_MPU_WRAPPERS == 1 )

/* MPU_WRAPPERS_INCLUDED_FROM_API_FILE will be defined when this file is
 * included from queue.c or task.c to prevent it from having an effect within
 * those files. */
    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ MPU_WRAPPERS_INCLUDED_FROM_API_FILE

/*
 * Map standard (non MPU) API functions to equivalents that start
 * "MPU_".  This will cause the application code to call the MPU_
 * version, which wraps the non-MPU version with privilege promoting
 * then demoting code, so the kernel code always runs will full
 * privileges.
 */

/* Map standard task.h API functions to the MPU equivalents. */
        #ವ್ಯಾಖ್ಯಾನಿಸು xTaskCreate                            MPU_xTaskCreate
        #ವ್ಯಾಖ್ಯಾನಿಸು xTaskCreateStatic                      MPU_xTaskCreateStatic
        #ವ್ಯಾಖ್ಯಾನಿಸು vTaskDelete                            MPU_vTaskDelete
        #ವ್ಯಾಖ್ಯಾನಿಸು vTaskDelay                             MPU_vTaskDelay
        #ವ್ಯಾಖ್ಯಾನಿಸು xTaskDelayUntil                        MPU_xTaskDelayUntil
        #ವ್ಯಾಖ್ಯಾನಿಸು xTaskAbortDelay                        MPU_xTaskAbortDelay
        #ವ್ಯಾಖ್ಯಾನಿಸು ux_Task_ಆದ್ಯತೆ_Get                      MPU_ux_Task_ಆದ್ಯತೆ_Get
        #ವ್ಯಾಖ್ಯಾನಿಸು eTaskGetState                          MPU_eTaskGetState
        #ವ್ಯಾಖ್ಯಾನಿಸು vTaskGetInfo                           MPU_vTaskGetInfo
        #ವ್ಯಾಖ್ಯಾನಿಸು v_Task_ಆದ್ಯತೆ_ಗಣ                       MPU_v_Task_ಆದ್ಯತೆ_ಗಣ
        #ವ್ಯಾಖ್ಯಾನಿಸು vTaskSuspend                           MPU_vTaskSuspend
        #ವ್ಯಾಖ್ಯಾನಿಸು vTaskResume                            MPU_vTaskResume
        #ವ್ಯಾಖ್ಯಾನಿಸು vTaskSuspendAll                        MPU_vTaskSuspendAll
        #ವ್ಯಾಖ್ಯಾನಿಸು xTaskResumeAll                         MPU_xTaskResumeAll
        #ವ್ಯಾಖ್ಯಾನಿಸು x_Task_Get_Tick_ಎಣಿಕೆ                      MPU_x_Task_Get_Tick_ಎಣಿಕೆ
        #ವ್ಯಾಖ್ಯಾನಿಸು ux_Task_Get_ಸಂಖ್ಯೆ_ದ_Tasks                 MPU_ux_Task_Get_ಸಂಖ್ಯೆ_ದ_Tasks
        #ವ್ಯಾಖ್ಯಾನಿಸು pcTaskGetName                          MPU_pcTaskGetName
        #ವ್ಯಾಖ್ಯಾನಿಸು xTaskGetHandle                         MPU_xTaskGetHandle
        #ವ್ಯಾಖ್ಯಾನಿಸು ux_Task_Get_ರಾಶಿ_ಮೇಲಿನ_Water_Mark            MPU_ux_Task_Get_ರಾಶಿ_ಮೇಲಿನ_Water_Mark
        #ವ್ಯಾಖ್ಯಾನಿಸು ux_Task_Get_ರಾಶಿ_ಮೇಲಿನ_Water_Mark2           MPU_ux_Task_Get_ರಾಶಿ_ಮೇಲಿನ_Water_Mark2
        #ವ್ಯಾಖ್ಯಾನಿಸು v_Task_ಗಣ_Application_Task_ಟ್ಯಾಗ್             MPU_v_Task_ಗಣ_Application_Task_ಟ್ಯಾಗ್
        #ವ್ಯಾಖ್ಯಾನಿಸು x_Task_Get_Application_Task_ಟ್ಯಾಗ್             MPU_x_Task_Get_Application_Task_ಟ್ಯಾಗ್
        #ವ್ಯಾಖ್ಯಾನಿಸು v_Task_ಗಣ_Thread_Local_Storage_ಸೂಚಕ      MPU_v_Task_ಗಣ_Thread_Local_Storage_ಸೂಚಕ
        #ವ್ಯಾಖ್ಯಾನಿಸು pv_Task_Get_Thread_Local_Storage_ಸೂಚಕ     MPU_pv_Task_Get_Thread_Local_Storage_ಸೂಚಕ
        #ವ್ಯಾಖ್ಯಾನಿಸು xTaskCallApplicationTaskHook           MPU_xTaskCallApplicationTaskHook
        #ವ್ಯಾಖ್ಯಾನಿಸು xTaskGetIdleTaskHandle                 MPU_xTaskGetIdleTaskHandle
        #ವ್ಯಾಖ್ಯಾನಿಸು uxTaskGetSystemState                   MPU_uxTaskGetSystemState
        #ವ್ಯಾಖ್ಯಾನಿಸು vTaskList                              MPU_vTaskList
        #ವ್ಯಾಖ್ಯಾನಿಸು v_Task_Get_ಸತತ_ಸಮಯ_Stats                   MPU_v_Task_Get_ಸತತ_ಸಮಯ_Stats
        #ವ್ಯಾಖ್ಯಾನಿಸು ul_Task_Get_Idle_ಸತತ_ಸಮಯ_Counter            MPU_ul_Task_Get_Idle_ಸತತ_ಸಮಯ_Counter
        #ವ್ಯಾಖ್ಯಾನಿಸು ul_Task_Get_Idle_ಸತತ_ಸಮಯ_Percent            MPU_ul_Task_Get_Idle_ಸತತ_ಸಮಯ_Percent
        #ವ್ಯಾಖ್ಯಾನಿಸು xTaskGenericNotify                     MPU_xTaskGenericNotify
        #ವ್ಯಾಖ್ಯಾನಿಸು xTaskGenericNotifyWait                 MPU_xTaskGenericNotifyWait
        #ವ್ಯಾಖ್ಯಾನಿಸು ulTaskGenericNotifyTake                MPU_ulTaskGenericNotifyTake
        #ವ್ಯಾಖ್ಯಾನಿಸು xTaskGenericNotifyStateClear           MPU_xTaskGenericNotifyStateClear
        #ವ್ಯಾಖ್ಯಾನಿಸು ul_Task_Generic_Notify_ಮೌಲ್ಯ_Clear          MPU_ul_Task_Generic_Notify_ಮೌಲ್ಯ_Clear
        #ವ್ಯಾಖ್ಯಾನಿಸು x_Task_Catch_ಮೇಲೆ_Ticks                      MPU_x_Task_Catch_ಮೇಲೆ_Ticks

        #ವ್ಯಾಖ್ಯಾನಿಸು xTaskGetCurrentTaskHandle              MPU_xTaskGetCurrentTaskHandle
        #ವ್ಯಾಖ್ಯಾನಿಸು v_Task_ಗಣ_ಸಮಯ_ಹೊರಹೋಗು_State                   MPU_v_Task_ಗಣ_ಸಮಯ_ಹೊರಹೋಗು_State
        #ವ್ಯಾಖ್ಯಾನಿಸು x_Task_Check_For_ಸಮಯ_ಹೊರಹೋಗು                   MPU_x_Task_Check_For_ಸಮಯ_ಹೊರಹೋಗು
        #ವ್ಯಾಖ್ಯಾನಿಸು xTaskGetSchedulerState                 MPU_xTaskGetSchedulerState

/* Map standard queue.h API functions to the MPU equivalents. */
        #ವ್ಯಾಖ್ಯಾನಿಸು x_ಸರತಿ_Generic_Send                      MPU_x_ಸರತಿ_Generic_Send
        #ವ್ಯಾಖ್ಯಾನಿಸು x_ಸರತಿ_Receive                          MPU_x_ಸರತಿ_Receive
        #ವ್ಯಾಖ್ಯಾನಿಸು x_ಸರತಿ_Peek                             MPU_x_ಸರತಿ_Peek
        #ವ್ಯಾಖ್ಯಾನಿಸು x_ಸರತಿ_Semaphore_Take                    MPU_x_ಸರತಿ_Semaphore_Take
        #ವ್ಯಾಖ್ಯಾನಿಸು ux_ಸರತಿ_Messages_ಕಾಯುವಿಕೆ                 MPU_ux_ಸರತಿ_Messages_ಕಾಯುವಿಕೆ
        #ವ್ಯಾಖ್ಯಾನಿಸು ux_ಸರತಿ_Spaces_ಲಭ್ಯ                 MPU_ux_ಸರತಿ_Spaces_ಲಭ್ಯ
        #ವ್ಯಾಖ್ಯಾನಿಸು v_ಸರತಿ_Delete                           MPU_v_ಸರತಿ_Delete
        #ವ್ಯಾಖ್ಯಾನಿಸು x_ಸರತಿ_Create_Mutex                      MPU_x_ಸರತಿ_Create_Mutex
        #ವ್ಯಾಖ್ಯಾನಿಸು x_ಸರತಿ_Create_Mutex_Static                MPU_x_ಸರತಿ_Create_Mutex_Static
        #ವ್ಯಾಖ್ಯಾನಿಸು x_ಸರತಿ_Create_Counting_Semaphore          MPU_x_ಸರತಿ_Create_Counting_Semaphore
        #ವ್ಯಾಖ್ಯಾನಿಸು x_ಸರತಿ_Create_Counting_Semaphore_Static    MPU_x_ಸರತಿ_Create_Counting_Semaphore_Static
        #ವ್ಯಾಖ್ಯಾನಿಸು x_ಸರತಿ_Get_Mutex_Holder                   MPU_x_ಸರತಿ_Get_Mutex_Holder
        #ವ್ಯಾಖ್ಯಾನಿಸು x_ಸರತಿ_Take_Mutex_Recursive               MPU_x_ಸರತಿ_Take_Mutex_Recursive
        #ವ್ಯಾಖ್ಯಾನಿಸು x_ಸರತಿ_Give_Mutex_Recursive               MPU_x_ಸರತಿ_Give_Mutex_Recursive
        #ವ್ಯಾಖ್ಯಾನಿಸು x_ಸರತಿ_Generic_Create                    MPU_x_ಸರತಿ_Generic_Create
        #ವ್ಯಾಖ್ಯಾನಿಸು x_ಸರತಿ_Generic_Create_Static              MPU_x_ಸರತಿ_Generic_Create_Static
        #ವ್ಯಾಖ್ಯಾನಿಸು x_ಸರತಿ_Create_ಗಣ                        MPU_x_ಸರತಿ_Create_ಗಣ
        #ವ್ಯಾಖ್ಯಾನಿಸು x_ಸರತಿ_ಕೂಡಿಸು_ಗಮ್ಯ_ಗಣ                         MPU_x_ಸರತಿ_ಕೂಡಿಸು_ಗಮ್ಯ_ಗಣ
        #ವ್ಯಾಖ್ಯಾನಿಸು x_ಸರತಿ_Remove_From_ಗಣ                    MPU_x_ಸರತಿ_Remove_From_ಗಣ
        #ವ್ಯಾಖ್ಯಾನಿಸು x_ಸರತಿ_Select_From_ಗಣ                    MPU_x_ಸರತಿ_Select_From_ಗಣ
        #ವ್ಯಾಖ್ಯಾನಿಸು x_ಸರತಿ_Generic_Reset                     MPU_x_ಸರತಿ_Generic_Reset

        #ಆದರೆ ( config_ಸರತಿ_REGISTRY_ಗಾತ್ರ > 0 )
            #ವ್ಯಾಖ್ಯಾನಿಸು v_ಸರತಿ_ಕೂಡಿಸು_ಗಮ್ಯ_Registry                MPU_v_ಸರತಿ_ಕೂಡಿಸು_ಗಮ್ಯ_Registry
            #ವ್ಯಾಖ್ಯಾನಿಸು v_ಸರತಿ_Unregister_ಸರತಿ              MPU_v_ಸರತಿ_Unregister_ಸರತಿ
            #ವ್ಯಾಖ್ಯಾನಿಸು pc_ಸರತಿ_Get_Name                     MPU_pc_ಸರತಿ_Get_Name
        #ಆದರೆ_ಅಂತ್ಯ

/* Map standard timer.h API functions to the MPU equivalents. */
        #ವ್ಯಾಖ್ಯಾನಿಸು pvTimerGetTimerID                      MPU_pvTimerGetTimerID
        #ವ್ಯಾಖ್ಯಾನಿಸು v_Timer_ಗಣ_Timer_ID                       MPU_v_Timer_ಗಣ_Timer_ID
        #ವ್ಯಾಖ್ಯಾನಿಸು x_Timer_Is_Timer_ಸಕ್ರಿಯ                    MPU_x_Timer_Is_Timer_ಸಕ್ರಿಯ
        #ವ್ಯಾಖ್ಯಾನಿಸು xTimerGetTimerDaemonTaskHandle         MPU_xTimerGetTimerDaemonTaskHandle
        #ವ್ಯಾಖ್ಯಾನಿಸು pcTimerGetName                         MPU_pcTimerGetName
        #ವ್ಯಾಖ್ಯಾನಿಸು v_Timer_ಗಣ_Reload_ಬಹುಲಕ                    MPU_v_Timer_ಗಣ_Reload_ಬಹುಲಕ
        #ವ್ಯಾಖ್ಯಾನಿಸು ux_Timer_Get_Reload_ಬಹುಲಕ                   MPU_ux_Timer_Get_Reload_ಬಹುಲಕ
        #ವ್ಯಾಖ್ಯಾನಿಸು xTimerGetPeriod                        MPU_xTimerGetPeriod
        #ವ್ಯಾಖ್ಯಾನಿಸು x_Timer_Get_Expiry_ಸಮಯ                    MPU_x_Timer_Get_Expiry_ಸಮಯ
        #ವ್ಯಾಖ್ಯಾನಿಸು xTimerGenericCommand                   MPU_xTimerGenericCommand

/* Map standard event_group.h API functions to the MPU equivalents. */
        #ವ್ಯಾಖ್ಯಾನಿಸು x_Event_ಗುಂಪು_Create                      MPU_x_Event_ಗುಂಪು_Create
        #ವ್ಯಾಖ್ಯಾನಿಸು x_Event_ಗುಂಪು_Create_Static                MPU_x_Event_ಗುಂಪು_Create_Static
        #ವ್ಯಾಖ್ಯಾನಿಸು x_Event_ಗುಂಪು_Wait_ಬಿಟ್ಟುಗಳು                    MPU_x_Event_ಗುಂಪು_Wait_ಬಿಟ್ಟುಗಳು
        #ವ್ಯಾಖ್ಯಾನಿಸು x_Event_ಗುಂಪು_Clear_ಬಿಟ್ಟುಗಳು                   MPU_x_Event_ಗುಂಪು_Clear_ಬಿಟ್ಟುಗಳು
        #ವ್ಯಾಖ್ಯಾನಿಸು x_Event_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು                     MPU_x_Event_ಗುಂಪು_ಗಣ_ಬಿಟ್ಟುಗಳು
        #ವ್ಯಾಖ್ಯಾನಿಸು x_Event_ಗುಂಪು_Sync                        MPU_x_Event_ಗುಂಪು_Sync
        #ವ್ಯಾಖ್ಯಾನಿಸು v_Event_ಗುಂಪು_Delete                      MPU_v_Event_ಗುಂಪು_Delete

/* Map standard message/stream_buffer.h API functions to the MPU
 * equivalents. */
        #ವ್ಯಾಖ್ಯಾನಿಸು x_ಹರಿವು_ಬಫರ್_Send                      MPU_x_ಹರಿವು_ಬಫರ್_Send
        #ವ್ಯಾಖ್ಯಾನಿಸು x_ಹರಿವು_ಬಫರ್_Receive                   MPU_x_ಹರಿವು_ಬಫರ್_Receive
        #ವ್ಯಾಖ್ಯಾನಿಸು x_ಹರಿವು_ಬಫರ್_ಮುಂದಿನ_ಸಂದೇಶ_ಉದ್ದಳತೆ_ಬೈಟುಗಳು    MPU_x_ಹರಿವು_ಬಫರ್_ಮುಂದಿನ_ಸಂದೇಶ_ಉದ್ದಳತೆ_ಬೈಟುಗಳು
        #ವ್ಯಾಖ್ಯಾನಿಸು v_ಹರಿವು_ಬಫರ್_Delete                    MPU_v_ಹರಿವು_ಬಫರ್_Delete
        #ವ್ಯಾಖ್ಯಾನಿಸು x_ಹರಿವು_ಬಫರ್_Is_ತುಂಬಿದ                    MPU_x_ಹರಿವು_ಬಫರ್_Is_ತುಂಬಿದ
        #ವ್ಯಾಖ್ಯಾನಿಸು x_ಹರಿವು_ಬಫರ್_Is_ಖಾಲಿಗಳು                   MPU_x_ಹರಿವು_ಬಫರ್_Is_ಖಾಲಿಗಳು
        #ವ್ಯಾಖ್ಯಾನಿಸು x_ಹರಿವು_ಬಫರ್_Reset                     MPU_x_ಹರಿವು_ಬಫರ್_Reset
        #ವ್ಯಾಖ್ಯಾನಿಸು x_ಹರಿವು_ಬಫರ್_Spaces_ಲಭ್ಯ           MPU_x_ಹರಿವು_ಬಫರ್_Spaces_ಲಭ್ಯ
        #ವ್ಯಾಖ್ಯಾನಿಸು x_ಹರಿವು_ಬಫರ್_ಬೈಟುಗಳು_ಲಭ್ಯ            MPU_x_ಹರಿವು_ಬಫರ್_ಬೈಟುಗಳು_ಲಭ್ಯ
        #ವ್ಯಾಖ್ಯಾನಿಸು x_ಹರಿವು_ಬಫರ್_ಗಣ_Trigger_Level           MPU_x_ಹರಿವು_ಬಫರ್_ಗಣ_Trigger_Level
        #ವ್ಯಾಖ್ಯಾನಿಸು x_ಹರಿವು_ಬಫರ್_Generic_Create             MPU_x_ಹರಿವು_ಬಫರ್_Generic_Create
        #ವ್ಯಾಖ್ಯಾನಿಸು x_ಹರಿವು_ಬಫರ್_Generic_Create_Static       MPU_x_ಹರಿವು_ಬಫರ್_Generic_Create_Static


/* Remove the privileged function macro, but keep the PRIVILEGED_DATA
 * macro so applications can place data in privileged access sections
 * (useful when using statically allocated objects). */
        #ವ್ಯಾಖ್ಯಾನಿಸು PRIVILEGED_FUNCTION
        #ವ್ಯಾಖ್ಯಾನಿಸು PRIVILEGED_ದತ್ತಾಂಶ    __attribute__( ( section( "privileged_data" ) ) )
        #ವ್ಯಾಖ್ಯಾನಿಸು FREERTOS_SYSTEM_CALL

    #ಇಲ್ಲದಿದ್ದರೆ /* MPU_WRAPPERS_INCLUDED_FROM_API_FILE */

/* Ensure API functions go in the privileged execution section. */
        #ವ್ಯಾಖ್ಯಾನಿಸು PRIVILEGED_FUNCTION     __attribute__( ( section( "privileged_functions" ) ) )
        #ವ್ಯಾಖ್ಯಾನಿಸು PRIVILEGED_ದತ್ತಾಂಶ         __attribute__( ( section( "privileged_data" ) ) )
        #ವ್ಯಾಖ್ಯಾನಿಸು FREERTOS_SYSTEM_CALL    __attribute__( ( section( "freertos_system_calls" ) ) )

    #ಆದರೆ_ಅಂತ್ಯ /* MPU_WRAPPERS_INCLUDED_FROM_API_FILE */

#ಇಲ್ಲದಿದ್ದರೆ /* portUSING_MPU_WRAPPERS */

    #ವ್ಯಾಖ್ಯಾನಿಸು PRIVILEGED_FUNCTION
    #ವ್ಯಾಖ್ಯಾನಿಸು PRIVILEGED_ದತ್ತಾಂಶ
    #ವ್ಯಾಖ್ಯಾನಿಸು FREERTOS_SYSTEM_CALL

#ಆದರೆ_ಅಂತ್ಯ /* portUSING_MPU_WRAPPERS */


#ಆದರೆ_ಅಂತ್ಯ /* MPU_WRAPPERS_H */
