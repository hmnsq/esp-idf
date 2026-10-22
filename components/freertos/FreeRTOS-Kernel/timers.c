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

/* Standard includes. */
#ಅನ್ತರ್ಭಾವ <stdlib.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#ವ್ಯಾಖ್ಯಾನಿಸು MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#ಅನ್ತರ್ಭಾವ "FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "task.h"
#ಅನ್ತರ್ಭಾವ "queue.h"
#ಅನ್ತರ್ಭಾವ "timers.h"
/* Include private IDF API additions for critical thread safety macros */
#ಅನ್ತರ್ಭಾವ "esp_private/freertos_idf_additions_priv.h"

#ಆದರೆ ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 0 )
    #ದೋಷ configUSE_TIMERS must be ಗಣ ಗಮ್ಯ 1 ಗಮ್ಯ make the xTimerPendFunctionCall() function ಲಭ್ಯ.
#ಆದರೆ_ಅಂತ್ಯ

/* Lint e9021, e961 and e750 are suppressed as a MISRA exception justified
 * because the MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು MPU_WRAPPERS_INCLUDED_FROM_API_FILE /*lint !e9021 !e961 !e750. */


/* This entire source file will be skipped if the application is not configured
 * to include software timer functionality.  This #if is closed at the very bottom
 * of this file.  If you want to include software timer functionality then ensure
 * configUSE_TIMERS is set to 1 in FreeRTOSConfig.h. */
#ಆದರೆ ( configUSE_TIMERS == 1 )

/* Misc definitions. */
    #ವ್ಯಾಖ್ಯಾನಿಸು tmrNO_DELAY                    ( ( TickType_t ) 0U )
    #ವ್ಯಾಖ್ಯಾನಿಸು tmr_MAX_ಸಮಯ_BEFORE_ಅತಿಪ್ರವಾಹ    ( ( TickType_t ) -1 )

/* The name assigned to the timer service task. This can be overridden by
 * defining configTIMER_SERVICE_TASK_NAME in FreeRTOSConfig.h. */
    #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configTIMER_SERVICE_TASK_NAME
        #ವ್ಯಾಖ್ಯಾನಿಸು configTIMER_SERVICE_TASK_NAME    "Tmr Svc"
    #ಆದರೆ_ಅಂತ್ಯ

    #ಆದರೆ ( config_ಸಂಖ್ಯೆ_ದ_CORES > 1 )

/* The core affinity assigned to the timer service task on SMP systems.
 * This can be overridden by defining configTIMER_SERVICE_TASK_CORE_AFFINITY in FreeRTOSConfig.h. */
        #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ configTIMER_SERVICE_TASK_CORE_AFFINITY
            #ವ್ಯಾಖ್ಯಾನಿಸು configTIMER_SERVICE_TASK_CORE_AFFINITY    tskNO_AFFINITY
        #ಆದರೆ_ಅಂತ್ಯ

        #ಆದರೆ ( !( ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x0 ) || ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x1 ) || ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x7FFFFFFF /* tskNO_AFFINITY */ ) ) )
            #ದೋಷ Invalid Timer task affinity. Timer task core ID can only be 0x0, 0x1 or 0x7FFFFFFF (tskNO_AFFINITY).
        #ಆದರೆ_ಅಂತ್ಯ /* #if ( !( ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x0 ) || ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x01 ) || ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x7FFFFFFF ) ) ) */
    #ಆದರೆ_ಅಂತ್ಯ /* #if ( configNUMBER_OF_CORES > 1 ) */

/* Bit definitions used in the ucStatus member of a timer structure. */
    #ವ್ಯಾಖ್ಯಾನಿಸು tmr_ಸ್ಥಿತಿ_IS_ಸಕ್ರಿಯ                  ( ( uint8_t ) 0x01 )
    #ವ್ಯಾಖ್ಯಾನಿಸು tmr_ಸ್ಥಿತಿ_IS_STATICALLY_ALLOCATED    ( ( uint8_t ) 0x02 )
    #ವ್ಯಾಖ್ಯಾನಿಸು tmr_ಸ್ಥಿತಿ_IS_AUTORELOAD              ( ( uint8_t ) 0x04 )

/* The definition of the timers themselves. */
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ tmrTimerControl                  /* The old naming convention is used to prevent breaking kernel aware debuggers. */
    {
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * pcTimerName;                   /*<< Text name.  This is not used by the kernel, it is included simply to make debugging easier. */ /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
        List_ಅಂಶ_t x_Timer_List_ಅಂಶ;                  /*<< Standard linked list item as used by all kernel features for event management. */
        TickType_t x_Timer_Period_ಒಳ_Ticks;             /*<< How quickly and often the timer expires. */
        ಶೂನ್ಯ * pvTimerID;                           /*<< An ID to identify the timer.  This allows the timer to be identified when the same callback is used for multiple timers. */
        TimerCallbackFunction_t pxCallbackFunction; /*<< The function that will be called when the timer expires. */
        #ಆದರೆ ( configUSE_TRACE_FACILITY == 1 )
            UBaseType_t ux_Timer_ಸಂಖ್ಯೆ;              /*<< An ID assigned by trace tools such as FreeRTOS+Trace */
        #ಆದರೆ_ಅಂತ್ಯ
        uint8_t uc_ಸ್ಥಿತಿ;                           /*<< Holds bits to say if the timer was statically allocated or not, and if it is active or not. */
    } xTIMER;

/* The old xTIMER name is maintained above then typedefed to the new Timer_t
 * name below to enable the use of older kernel aware debuggers. */
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ xTIMER Timer_t;

/* The definition of messages that can be sent and received on the timer queue.
 * Two types of message can be queued - messages that manipulate a software timer,
 * and messages that request the execution of a non-timer related callback.  The
 * two message types are defined in two separate structures, xTimerParametersType
 * and xCallbackParametersType respectively. */
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ tmrTimerParameters
    {
        TickType_t x_ಸಂದೇಶ_ಮೌಲ್ಯ; /*<< An optional value used by a subset of commands, for example, when changing the period of a timer. */
        Timer_t * pxTimer;        /*<< The timer to which the command will be applied. */
    } TimerParameter_t;


    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ tmrCallbackParameters
    {
        PendedFunction_t pxCallbackFunction; /* << The callback function to execute. */
        ಶೂನ್ಯ * pvParameter1;                 /* << The value that will be used as the callback functions first parameter. */
        uint32_t ulParameter2;               /* << The value that will be used as the callback functions second parameter. */
    } CallbackParameters_t;

/* The structure that contains the two message types, along with an identifier
 * that is used to determine which message type is valid. */
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ tmr_Timer_ಸರತಿ_ಸಂದೇಶ
    {
        ತಳ_Type_t x_ಸಂದೇಶ_ID; /*<< The command being sent to the timer service task. */
        ಒಕ್ಕೂಟ
        {
            TimerParameter_t xTimerParameters;

            /* Don't include xCallbackParameters if it is not going to be used as
             * it makes the structure (and therefore the timer queue) larger. */
            #ಆದರೆ ( INCLUDE_xTimerPendFunctionCall == 1 )
                CallbackParameters_t xCallbackParameters;
            #ಆದರೆ_ಅಂತ್ಯ /* INCLUDE_xTimerPendFunctionCall */
        } u;
    } Daemon_Task_ಸಂದೇಶ_t;

/*lint -save -e956 A manual analysis and inspection has been used to determine
 * which static variables must be declared volatile. */

/* The list in which active timers are stored.  Timers are referenced in expire
 * time order, with the nearest expiry time at the front of the list.  Only the
 * timer service task is allowed to access these lists.
 * xActiveTimerList1 and xActiveTimerList2 could be at function scope but that
 * breaks some kernel aware debuggers, and debuggers that reply on removing the
 * static qualifier. */
    PRIVILEGED_ದತ್ತಾಂಶ ಸ್ಥಿರ List_t x_ಸಕ್ರಿಯ_Timer_List1;
    PRIVILEGED_ದತ್ತಾಂಶ ಸ್ಥಿರ List_t x_ಸಕ್ರಿಯ_Timer_List2;
    PRIVILEGED_ದತ್ತಾಂಶ ಸ್ಥಿರ List_t * pxCurrentTimerList;
    PRIVILEGED_ದತ್ತಾಂಶ ಸ್ಥಿರ List_t * px_ಅತಿಪ್ರವಾಹ_Timer_List;

/* A queue that is used to send commands to the timer service task. */
    PRIVILEGED_ದತ್ತಾಂಶ ಸ್ಥಿರ ಸರತಿ_Handle_t x_Timer_ಸರತಿ = ಖಾಲಿ;
    PRIVILEGED_ದತ್ತಾಂಶ ಸ್ಥಿರ TaskHandle_t xTimerTaskHandle = ಖಾಲಿ;

/* Spinlock required in SMP when accessing the timers. For now we use a single lock
 * Todo: Each timer could possible have its own lock for increased granularity. */
    PRIVILEGED_ದತ್ತಾಂಶ ಸ್ಥಿರ ಪೋರ್ಟ್_MUX_TYPE x_Timer_ಬೀಗ = ಪೋರ್ಟ್_MUX_INITIALIZER_UNLOCKED;

/*lint -restore */

/*-----------------------------------------------------------*/

/*
 * Initialise the infrastructure used by the timer service task if it has not
 * been initialised already.
 */
    ಸ್ಥಿರ ಶೂನ್ಯ prv_Check_For_Valid_List_And_ಸರತಿ( ಶೂನ್ಯ ) PRIVILEGED_FUNCTION;

/*
 * The timer service task (daemon).  Timer functionality is controlled by this
 * task.  Other tasks communicate with the timer service task using the
 * xTimerQueue queue.
 */
    ಸ್ಥಿರ ಪೋರ್ಟ್_TASK_FUNCTION_PROTO( prvTimerTask, pvParameters ) PRIVILEGED_FUNCTION;

/*
 * Called by the timer service task to interpret and process a command it
 * received on the timer queue.
 */
    ಸ್ಥಿರ ಶೂನ್ಯ prvProcessReceivedCommands( ಶೂನ್ಯ ) PRIVILEGED_FUNCTION;

/*
 * Insert the timer into either xActiveTimerList1, or xActiveTimerList2,
 * depending on if the expire time causes a timer counter overflow.
 */
    ಸ್ಥಿರ ತಳ_Type_t prv_ಸೇರಿಸು_Timer_ಒಳ_ಸಕ್ರಿಯ_List( Timer_t * ಸ್ಥಿರಾಂಕ pxTimer,
                                                  ಸ್ಥಿರಾಂಕ TickType_t x_ಮುಂದಿನ_Expiry_ಸಮಯ,
                                                  ಸ್ಥಿರಾಂಕ TickType_t x_ಸಮಯ_Now,
                                                  ಸ್ಥಿರಾಂಕ TickType_t x_Command_ಸಮಯ ) PRIVILEGED_FUNCTION;

/*
 * Reload the specified auto-reload timer.  If the reloading is backlogged,
 * clear the backlog, calling the callback for each additional reload.  When
 * this function returns, the next expiry time is after xTimeNow.
 */
    ಸ್ಥಿರ ಶೂನ್ಯ prvReloadTimer( Timer_t * ಸ್ಥಿರಾಂಕ pxTimer,
                                TickType_t x_Expired_ಸಮಯ,
                                ಸ್ಥಿರಾಂಕ TickType_t x_ಸಮಯ_Now ) PRIVILEGED_FUNCTION;

/*
 * An active timer has reached its expire time.  Reload the timer if it is an
 * auto-reload timer, then call its callback.
 */
    ಸ್ಥಿರ ಶೂನ್ಯ prvProcessExpiredTimer( ಸ್ಥಿರಾಂಕ TickType_t x_ಮುಂದಿನ_Expire_ಸಮಯ,
                                        ಸ್ಥಿರಾಂಕ TickType_t x_ಸಮಯ_Now ) PRIVILEGED_FUNCTION;

/*
 * The tick count has overflowed.  Switch the timer lists after ensuring the
 * current timer list does not still reference some timers.
 */
    ಸ್ಥಿರ ಶೂನ್ಯ prvSwitchTimerLists( ಶೂನ್ಯ ) PRIVILEGED_FUNCTION;

/*
 * Obtain the current tick count, setting *pxTimerListsWereSwitched to pdTRUE
 * if a tick count overflow occurred since prvSampleTimeNow() was last called.
 */
    ಸ್ಥಿರ TickType_t prv_Sample_ಸಮಯ_Now( ತಳ_Type_t * ಸ್ಥಿರಾಂಕ pxTimerListsWereSwitched ) PRIVILEGED_FUNCTION;

/*
 * If the timer list contains any active timers then return the expire time of
 * the timer that will expire first and set *pxListWasEmpty to false.  If the
 * timer list does not contain any timers then return 0 and set *pxListWasEmpty
 * to pdTRUE.
 */
    ಸ್ಥಿರ TickType_t prv_Get_ಮುಂದಿನ_Expire_ಸಮಯ( ತಳ_Type_t * ಸ್ಥಿರಾಂಕ px_List_Was_ಖಾಲಿಗಳು ) PRIVILEGED_FUNCTION;

/*
 * If a timer has expired, process it.  Otherwise, block the timer service task
 * until either a timer does expire or a command is received.
 */
    ಸ್ಥಿರ ಶೂನ್ಯ prv_Process_Timer_Or_ಖಂಡ_Task( ಸ್ಥಿರಾಂಕ TickType_t x_ಮುಂದಿನ_Expire_ಸಮಯ,
                                            ತಳ_Type_t x_List_Was_ಖಾಲಿಗಳು ) PRIVILEGED_FUNCTION;

/*
 * Called after a Timer_t structure has been allocated either statically or
 * dynamically to fill in the structure's members.
 */
    ಸ್ಥಿರ ಶೂನ್ಯ prvInitialiseNewTimer( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ pcTimerName, /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                       ಸ್ಥಿರಾಂಕ TickType_t x_Timer_Period_ಒಳ_Ticks,
                                       ಸ್ಥಿರಾಂಕ ತಳ_Type_t xAutoReload,
                                       ಶೂನ್ಯ * ಸ್ಥಿರಾಂಕ pvTimerID,
                                       TimerCallbackFunction_t pxCallbackFunction,
                                       Timer_t * pxNewTimer ) PRIVILEGED_FUNCTION;
/*-----------------------------------------------------------*/

    ತಳ_Type_t xTimerCreateTimerTask( ಶೂನ್ಯ )
    {
        ತಳ_Type_t xReturn = pdFAIL;

        /* This function is called when the scheduler is started if
         * configUSE_TIMERS is set to 1.  Check that the infrastructure used by the
         * timer service task has been created/initialised.  If timers have already
         * been created then the initialisation will already have been performed. */
        prv_Check_For_Valid_List_And_ಸರತಿ();

        ಆದರೆ( x_Timer_ಸರತಿ != ಖಾಲಿ )
        {
            /* The following code block to set the timer task core affinity has been picked up from the FreeRTOS-Kernel
             * upstream repository. Commit# 631ae9e6e4828cf5c7d58bb7ac78a64fc309bcb7.
             * Necessary changes have been made for IDF-FreeRTOS compatibility. */
            #ಆದರೆ ( config_ಸಂಖ್ಯೆ_ದ_CORES > 1 )
            {
                #ಆದರೆ ( config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 1 )
                {
                    StaticTask_t * pxTimerTaskTCBBuffer = ಖಾಲಿ;
                    ರಾಶಿ_Type_t * px_Timer_Task_ರಾಶಿ_ಬಫರ್ = ಖಾಲಿ;
                    uint32_t ul_Timer_Task_ರಾಶಿ_ಗಾತ್ರ;

                    v_Application_Get_Timer_Task_ಸ್ಮರಣೆ( &pxTimerTaskTCBBuffer, &px_Timer_Task_ರಾಶಿ_ಬಫರ್, &ul_Timer_Task_ರಾಶಿ_ಗಾತ್ರ );
                    xTimerTaskHandle = x_Task_Create_Static_Pinned_ಗಮ್ಯ_Core( prvTimerTask,
                                                                      configTIMER_SERVICE_TASK_NAME,
                                                                      ul_Timer_Task_ರಾಶಿ_ಗಾತ್ರ,
                                                                      ಖಾಲಿ,
                                                                      ( ( UBaseType_t ) config_TIMER_TASK_ಆದ್ಯತೆ ) | ಪೋರ್ಟ್_PRIVILEGE_ಬಿಟ್,
                                                                      px_Timer_Task_ರಾಶಿ_ಬಫರ್,
                                                                      pxTimerTaskTCBBuffer,
                                                                      configTIMER_SERVICE_TASK_CORE_AFFINITY );

                    ಆದರೆ( xTimerTaskHandle != ಖಾಲಿ )
                    {
                        xReturn = pdPASS;
                    }
                }
                #ಇಲ್ಲದಿದ್ದರೆ /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
                {
                    xReturn = x_Task_Create_Pinned_ಗಮ್ಯ_Core( prvTimerTask,
                                                       configTIMER_SERVICE_TASK_NAME,
                                                       config_TIMER_TASK_ರಾಶಿ_ಆಳ,
                                                       ಖಾಲಿ,
                                                       ( ( UBaseType_t ) config_TIMER_TASK_ಆದ್ಯತೆ ) | ಪೋರ್ಟ್_PRIVILEGE_ಬಿಟ್,
                                                       &xTimerTaskHandle,
                                                       configTIMER_SERVICE_TASK_CORE_AFFINITY );
                }
                #ಆದರೆ_ಅಂತ್ಯ /* configSUPPORT_STATIC_ALLOCATION */
            }
            #ಇಲ್ಲದಿದ್ದರೆ /* #if ( configNUMBER_OF_CORES > 1 ) */
            {
                #ಆದರೆ ( config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 1 )
                {
                    StaticTask_t * pxTimerTaskTCBBuffer = ಖಾಲಿ;
                    ರಾಶಿ_Type_t * px_Timer_Task_ರಾಶಿ_ಬಫರ್ = ಖಾಲಿ;
                    uint32_t ul_Timer_Task_ರಾಶಿ_ಗಾತ್ರ;

                    v_Application_Get_Timer_Task_ಸ್ಮರಣೆ( &pxTimerTaskTCBBuffer, &px_Timer_Task_ರಾಶಿ_ಬಫರ್, &ul_Timer_Task_ರಾಶಿ_ಗಾತ್ರ );
                    xTimerTaskHandle = xTaskCreateStatic( prvTimerTask,
                                                          configTIMER_SERVICE_TASK_NAME,
                                                          ul_Timer_Task_ರಾಶಿ_ಗಾತ್ರ,
                                                          ಖಾಲಿ,
                                                          ( ( UBaseType_t ) config_TIMER_TASK_ಆದ್ಯತೆ ) | ಪೋರ್ಟ್_PRIVILEGE_ಬಿಟ್,
                                                          px_Timer_Task_ರಾಶಿ_ಬಫರ್,
                                                          pxTimerTaskTCBBuffer );

                    ಆದರೆ( xTimerTaskHandle != ಖಾಲಿ )
                    {
                        xReturn = pdPASS;
                    }
                }
                #ಇಲ್ಲದಿದ್ದರೆ /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
                {
                    xReturn = xTaskCreate( prvTimerTask,
                                           configTIMER_SERVICE_TASK_NAME,
                                           config_TIMER_TASK_ರಾಶಿ_ಆಳ,
                                           ಖಾಲಿ,
                                           ( ( UBaseType_t ) config_TIMER_TASK_ಆದ್ಯತೆ ) | ಪೋರ್ಟ್_PRIVILEGE_ಬಿಟ್,
                                           &xTimerTaskHandle );
                }
                #ಆದರೆ_ಅಂತ್ಯ /* configSUPPORT_STATIC_ALLOCATION */
            }
            #ಆದರೆ_ಅಂತ್ಯ /* #if ( configNUMBER_OF_CORES > 1 ) */
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            mtCOVERAGE_TEST_MARKER();
        }

        configASSERT( xReturn );
        ಹಿಂತಿರುಗಿ xReturn;
    }
/*-----------------------------------------------------------*/

    #ಆದರೆ ( config_SUPPORT_DYNAMIC_ಹಂಚಿಕೆ == 1 )

        TimerHandle_t xTimerCreate( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ pcTimerName, /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                    ಸ್ಥಿರಾಂಕ TickType_t x_Timer_Period_ಒಳ_Ticks,
                                    ಸ್ಥಿರಾಂಕ ತಳ_Type_t xAutoReload,
                                    ಶೂನ್ಯ * ಸ್ಥಿರಾಂಕ pvTimerID,
                                    TimerCallbackFunction_t pxCallbackFunction )
        {
            Timer_t * pxNewTimer;

            pxNewTimer = ( Timer_t * ) pv_ಪೋರ್ಟ್_Malloc( ಗಾತ್ರ( Timer_t ) ); /*lint !e9087 !e9079 All values returned by pvPortMalloc() have at least the alignment required by the MCU's stack, and the first member of Timer_t is always a pointer to the timer's mame. */

            ಆದರೆ( pxNewTimer != ಖಾಲಿ )
            {
                /* Status is thus far zero as the timer is not created statically
                 * and has not been started.  The auto-reload bit may get set in
                 * prvInitialiseNewTimer. */
                pxNewTimer->uc_ಸ್ಥಿತಿ = 0x00;
                prvInitialiseNewTimer( pcTimerName, x_Timer_Period_ಒಳ_Ticks, xAutoReload, pvTimerID, pxCallbackFunction, pxNewTimer );
            }

            ಹಿಂತಿರುಗಿ pxNewTimer;
        }

    #ಆದರೆ_ಅಂತ್ಯ /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

    #ಆದರೆ ( config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 1 )

        TimerHandle_t xTimerCreateStatic( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ pcTimerName, /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                          ಸ್ಥಿರಾಂಕ TickType_t x_Timer_Period_ಒಳ_Ticks,
                                          ಸ್ಥಿರಾಂಕ ತಳ_Type_t xAutoReload,
                                          ಶೂನ್ಯ * ಸ್ಥಿರಾಂಕ pvTimerID,
                                          TimerCallbackFunction_t pxCallbackFunction,
                                          StaticTimer_t * px_Timer_ಬಫರ್ )
        {
            Timer_t * pxNewTimer;

            #ಆದರೆ ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticTimer_t equals the size of the real timer
                 * structure. */
                ಅಸ್ಥಿರ size_t x_ಗಾತ್ರ = ಗಾತ್ರ( StaticTimer_t );
                configASSERT( x_ಗಾತ್ರ == ಗಾತ್ರ( Timer_t ) );
                ( ಶೂನ್ಯ ) x_ಗಾತ್ರ; /* Keeps lint quiet when configASSERT() is not defined. */
            }
            #ಆದರೆ_ಅಂತ್ಯ /* configASSERT_DEFINED */

            /* A pointer to a StaticTimer_t structure MUST be provided, use it. */
            configASSERT( px_Timer_ಬಫರ್ );
            pxNewTimer = ( Timer_t * ) px_Timer_ಬಫರ್; /*lint !e740 !e9087 StaticTimer_t is a pointer to a Timer_t, so guaranteed to be aligned and sized correctly (checked by an assert()), so this is safe. */

            ಆದರೆ( pxNewTimer != ಖಾಲಿ )
            {
                /* Timers can be created statically or dynamically so note this
                 * timer was created statically in case it is later deleted.  The
                 * auto-reload bit may get set in prvInitialiseNewTimer(). */
                pxNewTimer->uc_ಸ್ಥಿತಿ = tmr_ಸ್ಥಿತಿ_IS_STATICALLY_ALLOCATED;

                prvInitialiseNewTimer( pcTimerName, x_Timer_Period_ಒಳ_Ticks, xAutoReload, pvTimerID, pxCallbackFunction, pxNewTimer );
            }

            ಹಿಂತಿರುಗಿ pxNewTimer;
        }

    #ಆದರೆ_ಅಂತ್ಯ /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

    ಸ್ಥಿರ ಶೂನ್ಯ prvInitialiseNewTimer( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ pcTimerName, /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                       ಸ್ಥಿರಾಂಕ TickType_t x_Timer_Period_ಒಳ_Ticks,
                                       ಸ್ಥಿರಾಂಕ ತಳ_Type_t xAutoReload,
                                       ಶೂನ್ಯ * ಸ್ಥಿರಾಂಕ pvTimerID,
                                       TimerCallbackFunction_t pxCallbackFunction,
                                       Timer_t * pxNewTimer )
    {
        /* 0 is not a valid value for xTimerPeriodInTicks. */
        configASSERT( ( x_Timer_Period_ಒಳ_Ticks > 0 ) );

        /* Ensure the infrastructure used by the timer service task has been
         * created/initialised. */
        prv_Check_For_Valid_List_And_ಸರತಿ();

        /* Initialise the timer structure members using the function
         * parameters. */
        pxNewTimer->pcTimerName = pcTimerName;
        pxNewTimer->x_Timer_Period_ಒಳ_Ticks = x_Timer_Period_ಒಳ_Ticks;
        pxNewTimer->pvTimerID = pvTimerID;
        pxNewTimer->pxCallbackFunction = pxCallbackFunction;
        v_List_Initialise_ಅಂಶ( &( pxNewTimer->x_Timer_List_ಅಂಶ ) );

        ಆದರೆ( xAutoReload != pdFALSE )
        {
            pxNewTimer->uc_ಸ್ಥಿತಿ |= tmr_ಸ್ಥಿತಿ_IS_AUTORELOAD;
        }

        traceTIMER_CREATE( pxNewTimer );
    }
/*-----------------------------------------------------------*/

    ತಳ_Type_t xTimerGenericCommand( TimerHandle_t xTimer,
                                     ಸ್ಥಿರಾಂಕ ತಳ_Type_t xCommandID,
                                     ಸ್ಥಿರಾಂಕ TickType_t x_Optional_ಮೌಲ್ಯ,
                                     ತಳ_Type_t * ಸ್ಥಿರಾಂಕ px_Higher_ಆದ್ಯತೆ_Task_Woken,
                                     ಸ್ಥಿರಾಂಕ TickType_t x_Ticks_ಗಮ್ಯ_Wait )
    {
        ತಳ_Type_t xReturn = pdFAIL;
        Daemon_Task_ಸಂದೇಶ_t x_ಸಂದೇಶ;

        configASSERT( xTimer );

        /* Send a message to the timer service task to perform a particular action
         * on a particular timer definition. */
        ಆದರೆ( x_Timer_ಸರತಿ != ಖಾಲಿ )
        {
            /* Send a command to the timer service task to start the xTimer timer. */
            x_ಸಂದೇಶ.x_ಸಂದೇಶ_ID = xCommandID;
            x_ಸಂದೇಶ.u.xTimerParameters.x_ಸಂದೇಶ_ಮೌಲ್ಯ = x_Optional_ಮೌಲ್ಯ;
            x_ಸಂದೇಶ.u.xTimerParameters.pxTimer = xTimer;

            ಆದರೆ( xCommandID < tmr_ಮೊದಲ_FROM_ISR_COMMAND )
            {
                ಆದರೆ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING )
                {
                    xReturn = x_ಸರತಿ_Send_ಗಮ್ಯ_ಹಿಂಭಾಗ( x_Timer_ಸರತಿ, &x_ಸಂದೇಶ, x_Ticks_ಗಮ್ಯ_Wait );
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    xReturn = x_ಸರತಿ_Send_ಗಮ್ಯ_ಹಿಂಭಾಗ( x_Timer_ಸರತಿ, &x_ಸಂದೇಶ, tmrNO_DELAY );
                }
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                xReturn = x_ಸರತಿ_Send_ಗಮ್ಯ_ಹಿಂಭಾಗ_From_ISR( x_Timer_ಸರತಿ, &x_ಸಂದೇಶ, px_Higher_ಆದ್ಯತೆ_Task_Woken );
            }

            traceTIMER_COMMAND_SEND( xTimer, xCommandID, x_Optional_ಮೌಲ್ಯ, xReturn );
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            mtCOVERAGE_TEST_MARKER();
        }

        ಹಿಂತಿರುಗಿ xReturn;
    }
/*-----------------------------------------------------------*/

    TaskHandle_t xTimerGetTimerDaemonTaskHandle( ಶೂನ್ಯ )
    {
        /* If xTimerGetTimerDaemonTaskHandle() is called before the scheduler has been
         * started, then xTimerTaskHandle will be NULL. */
        configASSERT( ( xTimerTaskHandle != ಖಾಲಿ ) );
        ಹಿಂತಿರುಗಿ xTimerTaskHandle;
    }
/*-----------------------------------------------------------*/

    TickType_t xTimerGetPeriod( TimerHandle_t xTimer )
    {
        Timer_t * pxTimer = xTimer;

        configASSERT( xTimer );
        ಹಿಂತಿರುಗಿ pxTimer->x_Timer_Period_ಒಳ_Ticks;
    }
/*-----------------------------------------------------------*/

    ಶೂನ್ಯ v_Timer_ಗಣ_Reload_ಬಹುಲಕ( TimerHandle_t xTimer,
                              ಸ್ಥಿರಾಂಕ ತಳ_Type_t xAutoReload )
    {
        Timer_t * pxTimer = xTimer;

        configASSERT( xTimer );
        taskENTER_CRITICAL( &x_Timer_ಬೀಗ );
        {
            ಆದರೆ( xAutoReload != pdFALSE )
            {
                pxTimer->uc_ಸ್ಥಿತಿ |= tmr_ಸ್ಥಿತಿ_IS_AUTORELOAD;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                pxTimer->uc_ಸ್ಥಿತಿ &= ( ( uint8_t ) ~tmr_ಸ್ಥಿತಿ_IS_AUTORELOAD );
            }
        }
        taskEXIT_CRITICAL( &x_Timer_ಬೀಗ );
    }
/*-----------------------------------------------------------*/

    ತಳ_Type_t x_Timer_Get_Reload_ಬಹುಲಕ( TimerHandle_t xTimer )
    {
        Timer_t * pxTimer = xTimer;
        ತಳ_Type_t xReturn;

        configASSERT( xTimer );
        taskENTER_CRITICAL( &x_Timer_ಬೀಗ );
        {
            ಆದರೆ( ( pxTimer->uc_ಸ್ಥಿತಿ & tmr_ಸ್ಥಿತಿ_IS_AUTORELOAD ) == 0 )
            {
                /* Not an auto-reload timer. */
                xReturn = pdFALSE;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                /* Is an auto-reload timer. */
                xReturn = pdTRUE;
            }
        }
        taskEXIT_CRITICAL( &x_Timer_ಬೀಗ );

        ಹಿಂತಿರುಗಿ xReturn;
    }

    UBaseType_t ux_Timer_Get_Reload_ಬಹುಲಕ( TimerHandle_t xTimer )
    {
        ಹಿಂತಿರುಗಿ ( UBaseType_t ) x_Timer_Get_Reload_ಬಹುಲಕ( xTimer );
    }
/*-----------------------------------------------------------*/

    TickType_t x_Timer_Get_Expiry_ಸಮಯ( TimerHandle_t xTimer )
    {
        Timer_t * pxTimer = xTimer;
        TickType_t xReturn;

        configASSERT( xTimer );
        xReturn = list_GET_LIST_ಅಂಶ_ಮೌಲ್ಯ( &( pxTimer->x_Timer_List_ಅಂಶ ) );
        ಹಿಂತಿರುಗಿ xReturn;
    }
/*-----------------------------------------------------------*/

    #ಆದರೆ ( config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 1 )
        ತಳ_Type_t x_Timer_Get_Static_ಬಫರ್( TimerHandle_t xTimer,
                                          StaticTimer_t ** ppx_Timer_ಬಫರ್ )
        {
            ತಳ_Type_t xReturn;
            Timer_t * pxTimer = xTimer;

            configASSERT( ppx_Timer_ಬಫರ್ != ಖಾಲಿ );

            ಆದರೆ( ( pxTimer->uc_ಸ್ಥಿತಿ & tmr_ಸ್ಥಿತಿ_IS_STATICALLY_ALLOCATED ) != 0 )
            {
                *ppx_Timer_ಬಫರ್ = ( StaticTimer_t * ) pxTimer;
                xReturn = pdTRUE;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                xReturn = pdFALSE;
            }

            ಹಿಂತಿರುಗಿ xReturn;
        }
    #ಆದರೆ_ಅಂತ್ಯ /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * pcTimerGetName( TimerHandle_t xTimer ) /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
    {
        Timer_t * pxTimer = xTimer;

        configASSERT( xTimer );
        ಹಿಂತಿರುಗಿ pxTimer->pcTimerName;
    }
/*-----------------------------------------------------------*/

    ಸ್ಥಿರ ಶೂನ್ಯ prvReloadTimer( Timer_t * ಸ್ಥಿರಾಂಕ pxTimer,
                                TickType_t x_Expired_ಸಮಯ,
                                ಸ್ಥಿರಾಂಕ TickType_t x_ಸಮಯ_Now )
    {
        /* Insert the timer into the appropriate list for the next expiry time.
         * If the next expiry time has already passed, advance the expiry time,
         * call the callback function, and try again. */
        ಆಗ( prv_ಸೇರಿಸು_Timer_ಒಳ_ಸಕ್ರಿಯ_List( pxTimer, ( x_Expired_ಸಮಯ + pxTimer->x_Timer_Period_ಒಳ_Ticks ), x_ಸಮಯ_Now, x_Expired_ಸಮಯ ) != pdFALSE )
        {
            /* Advance the expiry time. */
            x_Expired_ಸಮಯ += pxTimer->x_Timer_Period_ಒಳ_Ticks;

            /* Call the timer callback. */
            traceTIMER_EXPIRED( pxTimer );
            pxTimer->pxCallbackFunction( ( TimerHandle_t ) pxTimer );
        }
    }
/*-----------------------------------------------------------*/

    ಸ್ಥಿರ ಶೂನ್ಯ prvProcessExpiredTimer( ಸ್ಥಿರಾಂಕ TickType_t x_ಮುಂದಿನ_Expire_ಸಮಯ,
                                        ಸ್ಥಿರಾಂಕ TickType_t x_ಸಮಯ_Now )
    {
        Timer_t * ಸ್ಥಿರಾಂಕ pxTimer = ( Timer_t * ) list_GET_OWNER_ದ_ತಲೆ_ನಮೂದು( pxCurrentTimerList ); /*lint !e9087 !e9079 void * is used as this macro is used with tasks and co-routines too.  Alignment is known to be fine as the type of the pointer stored and retrieved is the same. */

        /* Remove the timer from the list of active timers.  A check has already
         * been performed to ensure the list is not empty. */

        ( ಶೂನ್ಯ ) uxListRemove( &( pxTimer->x_Timer_List_ಅಂಶ ) );

        /* If the timer is an auto-reload timer then calculate the next
         * expiry time and re-insert the timer in the list of active timers. */
        ಆದರೆ( ( pxTimer->uc_ಸ್ಥಿತಿ & tmr_ಸ್ಥಿತಿ_IS_AUTORELOAD ) != 0 )
        {
            prvReloadTimer( pxTimer, x_ಮುಂದಿನ_Expire_ಸಮಯ, x_ಸಮಯ_Now );
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            pxTimer->uc_ಸ್ಥಿತಿ &= ( ( uint8_t ) ~tmr_ಸ್ಥಿತಿ_IS_ಸಕ್ರಿಯ );
        }

        /* Call the timer callback. */
        traceTIMER_EXPIRED( pxTimer );
        pxTimer->pxCallbackFunction( ( TimerHandle_t ) pxTimer );
    }
/*-----------------------------------------------------------*/

    ಸ್ಥಿರ ಪೋರ್ಟ್_TASK_FUNCTION( prvTimerTask, pvParameters )
    {
        TickType_t x_ಮುಂದಿನ_Expire_ಸಮಯ;
        ತಳ_Type_t x_List_Was_ಖಾಲಿಗಳು;

        /* Just to avoid compiler warnings. */
        ( ಶೂನ್ಯ ) pvParameters;

        #ಆದರೆ ( configUSE_DAEMON_TASK_STARTUP_HOOK == 1 )
        {
            ಬಾಹ್ಯ ಶೂನ್ಯ vApplicationDaemonTaskStartupHook( ಶೂನ್ಯ );

            /* Allow the application writer to execute some code in the context of
             * this task at the point the task starts executing.  This is useful if the
             * application includes initialisation code that would benefit from
             * executing after the scheduler has been started. */
            vApplicationDaemonTaskStartupHook();
        }
        #ಆದರೆ_ಅಂತ್ಯ /* configUSE_DAEMON_TASK_STARTUP_HOOK */

        ಗಾಗಿ( ; ; )
        {
            /* Query the timers list to see if it contains any timers, and if so,
             * obtain the time at which the next timer will expire. */
            x_ಮುಂದಿನ_Expire_ಸಮಯ = prv_Get_ಮುಂದಿನ_Expire_ಸಮಯ( &x_List_Was_ಖಾಲಿಗಳು );

            /* If a timer has expired, process it.  Otherwise, block this task
             * until either a timer does expire, or a command is received. */
            prv_Process_Timer_Or_ಖಂಡ_Task( x_ಮುಂದಿನ_Expire_ಸಮಯ, x_List_Was_ಖಾಲಿಗಳು );

            /* Empty the command queue. */
            prvProcessReceivedCommands();
        }
    }
/*-----------------------------------------------------------*/

    ಸ್ಥಿರ ಶೂನ್ಯ prv_Process_Timer_Or_ಖಂಡ_Task( ಸ್ಥಿರಾಂಕ TickType_t x_ಮುಂದಿನ_Expire_ಸಮಯ,
                                            ತಳ_Type_t x_List_Was_ಖಾಲಿಗಳು )
    {
        TickType_t x_ಸಮಯ_Now;
        ತಳ_Type_t xTimerListsWereSwitched;

        prvENTER_CRITICAL_OR_SUSPEND_ALL( &x_Timer_ಬೀಗ );
        {
            /* Obtain the time now to make an assessment as to whether the timer
             * has expired or not.  If obtaining the time causes the lists to switch
             * then don't process this timer as any timers that remained in the list
             * when the lists were switched will have been processed within the
             * prvSampleTimeNow() function. */
            x_ಸಮಯ_Now = prv_Sample_ಸಮಯ_Now( &xTimerListsWereSwitched );

            ಆದರೆ( xTimerListsWereSwitched == pdFALSE )
            {
                /* The tick count has not overflowed, has the timer expired? */
                ಆದರೆ( ( x_List_Was_ಖಾಲಿಗಳು == pdFALSE ) && ( x_ಮುಂದಿನ_Expire_ಸಮಯ <= x_ಸಮಯ_Now ) )
                {
                    ( ಶೂನ್ಯ ) prvEXIT_CRITICAL_OR_RESUME_ALL( &x_Timer_ಬೀಗ );
                    prvProcessExpiredTimer( x_ಮುಂದಿನ_Expire_ಸಮಯ, x_ಸಮಯ_Now );
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    /* The tick count has not overflowed, and the next expire
                     * time has not been reached yet.  This task should therefore
                     * block to wait for the next expire time or a command to be
                     * received - whichever comes first.  The following line cannot
                     * be reached unless xNextExpireTime > xTimeNow, except in the
                     * case when the current timer list is empty. */
                    ಆದರೆ( x_List_Was_ಖಾಲಿಗಳು != pdFALSE )
                    {
                        /* The current timer list is empty - is the overflow list
                         * also empty? */
                        x_List_Was_ಖಾಲಿಗಳು = list_LIST_IS_ಖಾಲಿಗಳು( px_ಅತಿಪ್ರವಾಹ_Timer_List );
                    }

                    v_ಸರತಿ_Wait_For_ಸಂದೇಶ_Restricted( x_Timer_ಸರತಿ, ( x_ಮುಂದಿನ_Expire_ಸಮಯ - x_ಸಮಯ_Now ), x_List_Was_ಖಾಲಿಗಳು );

                    ಆದರೆ( prvEXIT_CRITICAL_OR_RESUME_ALL( &x_Timer_ಬೀಗ ) == pdFALSE )
                    {
                        /* Yield to wait for either a command to arrive, or the
                         * block time to expire.  If a command arrived between the
                         * critical section being exited and this yield then the yield
                         * will not cause the task to block. */
                        ಪೋರ್ಟ್_YIELD_WITHIN_API();
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ( ಶೂನ್ಯ ) prvEXIT_CRITICAL_OR_RESUME_ALL( &x_Timer_ಬೀಗ );
            }
        }
    }
/*-----------------------------------------------------------*/

    ಸ್ಥಿರ TickType_t prv_Get_ಮುಂದಿನ_Expire_ಸಮಯ( ತಳ_Type_t * ಸ್ಥಿರಾಂಕ px_List_Was_ಖಾಲಿಗಳು )
    {
        TickType_t x_ಮುಂದಿನ_Expire_ಸಮಯ;

        /* Timers are listed in expiry time order, with the head of the list
         * referencing the task that will expire first.  Obtain the time at which
         * the timer with the nearest expiry time will expire.  If there are no
         * active timers then just set the next expire time to 0.  That will cause
         * this task to unblock when the tick count overflows, at which point the
         * timer lists will be switched and the next expiry time can be
         * re-assessed.  */
        *px_List_Was_ಖಾಲಿಗಳು = list_LIST_IS_ಖಾಲಿಗಳು( pxCurrentTimerList );

        ಆದರೆ( *px_List_Was_ಖಾಲಿಗಳು == pdFALSE )
        {
            x_ಮುಂದಿನ_Expire_ಸಮಯ = list_GET_ಅಂಶ_ಮೌಲ್ಯ_ದ_ತಲೆ_ನಮೂದು( pxCurrentTimerList );
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            /* Ensure the task unblocks when the tick count rolls over. */
            x_ಮುಂದಿನ_Expire_ಸಮಯ = ( TickType_t ) 0U;
        }

        ಹಿಂತಿರುಗಿ x_ಮುಂದಿನ_Expire_ಸಮಯ;
    }
/*-----------------------------------------------------------*/

    ಸ್ಥಿರ TickType_t prv_Sample_ಸಮಯ_Now( ತಳ_Type_t * ಸ್ಥಿರಾಂಕ pxTimerListsWereSwitched )
    {
        TickType_t x_ಸಮಯ_Now;
        PRIVILEGED_ದತ್ತಾಂಶ ಸ್ಥಿರ TickType_t x_ಕೊನೆಯ_ಸಮಯ = ( TickType_t ) 0U; /*lint !e956 Variable is only accessible to one task. */

        x_ಸಮಯ_Now = x_Task_Get_Tick_ಎಣಿಕೆ();

        ಆದರೆ( x_ಸಮಯ_Now < x_ಕೊನೆಯ_ಸಮಯ )
        {
            prvSwitchTimerLists();
            *pxTimerListsWereSwitched = pdTRUE;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            *pxTimerListsWereSwitched = pdFALSE;
        }

        x_ಕೊನೆಯ_ಸಮಯ = x_ಸಮಯ_Now;

        ಹಿಂತಿರುಗಿ x_ಸಮಯ_Now;
    }
/*-----------------------------------------------------------*/

    ಸ್ಥಿರ ತಳ_Type_t prv_ಸೇರಿಸು_Timer_ಒಳ_ಸಕ್ರಿಯ_List( Timer_t * ಸ್ಥಿರಾಂಕ pxTimer,
                                                  ಸ್ಥಿರಾಂಕ TickType_t x_ಮುಂದಿನ_Expiry_ಸಮಯ,
                                                  ಸ್ಥಿರಾಂಕ TickType_t x_ಸಮಯ_Now,
                                                  ಸ್ಥಿರಾಂಕ TickType_t x_Command_ಸಮಯ )
    {
        ತಳ_Type_t xProcessTimerNow = pdFALSE;

        list_ಗಣ_LIST_ಅಂಶ_ಮೌಲ್ಯ( &( pxTimer->x_Timer_List_ಅಂಶ ), x_ಮುಂದಿನ_Expiry_ಸಮಯ );
        list_ಗಣ_LIST_ಅಂಶ_OWNER( &( pxTimer->x_Timer_List_ಅಂಶ ), pxTimer );

        ಆದರೆ( x_ಮುಂದಿನ_Expiry_ಸಮಯ <= x_ಸಮಯ_Now )
        {
            /* Has the expiry time elapsed between the command to start/reset a
             * timer was issued, and the time the command was processed? */
            ಆದರೆ( ( ( TickType_t ) ( x_ಸಮಯ_Now - x_Command_ಸಮಯ ) ) >= pxTimer->x_Timer_Period_ಒಳ_Ticks ) /*lint !e961 MISRA exception as the casts are only redundant for some ports. */
            {
                /* The time between a command being issued and the command being
                 * processed actually exceeds the timers period.  */
                xProcessTimerNow = pdTRUE;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                v_List_ಸೇರಿಸು( px_ಅತಿಪ್ರವಾಹ_Timer_List, &( pxTimer->x_Timer_List_ಅಂಶ ) );
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ಆದರೆ( ( x_ಸಮಯ_Now < x_Command_ಸಮಯ ) && ( x_ಮುಂದಿನ_Expiry_ಸಮಯ >= x_Command_ಸಮಯ ) )
            {
                /* If, since the command was issued, the tick count has overflowed
                 * but the expiry time has not, then the timer must have already passed
                 * its expiry time and should be processed immediately. */
                xProcessTimerNow = pdTRUE;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                v_List_ಸೇರಿಸು( pxCurrentTimerList, &( pxTimer->x_Timer_List_ಅಂಶ ) );
            }
        }

        ಹಿಂತಿರುಗಿ xProcessTimerNow;
    }
/*-----------------------------------------------------------*/

    ಸ್ಥಿರ ಶೂನ್ಯ prvProcessReceivedCommands( ಶೂನ್ಯ )
    {
        Daemon_Task_ಸಂದೇಶ_t x_ಸಂದೇಶ;
        Timer_t * pxTimer;
        ತಳ_Type_t xTimerListsWereSwitched;
        TickType_t x_ಸಮಯ_Now;

        ಆಗ( x_ಸರತಿ_Receive( x_Timer_ಸರತಿ, &x_ಸಂದೇಶ, tmrNO_DELAY ) != pdFAIL ) /*lint !e603 xMessage does not have to be initialised as it is passed out, not in, and it is not used unless xQueueReceive() returns pdTRUE. */
        {
            #ಆದರೆ ( INCLUDE_xTimerPendFunctionCall == 1 )
            {
                /* Negative commands are pended function calls rather than timer
                 * commands. */
                ಆದರೆ( x_ಸಂದೇಶ.x_ಸಂದೇಶ_ID < ( ತಳ_Type_t ) 0 )
                {
                    ಸ್ಥಿರಾಂಕ CallbackParameters_t * ಸ್ಥಿರಾಂಕ pxCallback = &( x_ಸಂದೇಶ.u.xCallbackParameters );

                    /* The timer uses the xCallbackParameters member to request a
                     * callback be executed.  Check the callback is not NULL. */
                    configASSERT( pxCallback );

                    /* Call the function. */
                    pxCallback->pxCallbackFunction( pxCallback->pvParameter1, pxCallback->ulParameter2 );
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            #ಆದರೆ_ಅಂತ್ಯ /* INCLUDE_xTimerPendFunctionCall */

            /* Commands that are positive are timer commands rather than pended
             * function calls. */
            ಆದರೆ( x_ಸಂದೇಶ.x_ಸಂದೇಶ_ID >= ( ತಳ_Type_t ) 0 )
            {
                /* The messages uses the xTimerParameters member to work on a
                 * software timer. */
                pxTimer = x_ಸಂದೇಶ.u.xTimerParameters.pxTimer;

                ಆದರೆ( listIS_CONTAINED_WITHIN( ಖಾಲಿ, &( pxTimer->x_Timer_List_ಅಂಶ ) ) == pdFALSE ) /*lint !e961. The cast is only redundant when NULL is passed into the macro. */
                {
                    /* The timer is in a list, remove it. */
                    ( ಶೂನ್ಯ ) uxListRemove( &( pxTimer->x_Timer_List_ಅಂಶ ) );
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                traceTIMER_COMMAND_RECEIVED( pxTimer, x_ಸಂದೇಶ.x_ಸಂದೇಶ_ID, x_ಸಂದೇಶ.u.xTimerParameters.x_ಸಂದೇಶ_ಮೌಲ್ಯ );

                /* In this case the xTimerListsWereSwitched parameter is not used, but
                 *  it must be present in the function call.  prvSampleTimeNow() must be
                 *  called after the message is received from xTimerQueue so there is no
                 *  possibility of a higher priority task adding a message to the message
                 *  queue with a time that is ahead of the timer daemon task (because it
                 *  pre-empted the timer daemon task after the xTimeNow value was set). */
                x_ಸಮಯ_Now = prv_Sample_ಸಮಯ_Now( &xTimerListsWereSwitched );

                ಆಯ್ಕೆ( x_ಸಂದೇಶ.x_ಸಂದೇಶ_ID )
                {
                    ಸ್ಥಿತಿ tmr_COMMAND_ಆರಂಭ:
                    ಸ್ಥಿತಿ tmr_COMMAND_ಆರಂಭ_FROM_ISR:
                    ಸ್ಥಿತಿ tmrCOMMAND_RESET:
                    ಸ್ಥಿತಿ tmrCOMMAND_RESET_FROM_ISR:
                        /* Start or restart a timer. */
                        pxTimer->uc_ಸ್ಥಿತಿ |= tmr_ಸ್ಥಿತಿ_IS_ಸಕ್ರಿಯ;

                        ಆದರೆ( prv_ಸೇರಿಸು_Timer_ಒಳ_ಸಕ್ರಿಯ_List( pxTimer, x_ಸಂದೇಶ.u.xTimerParameters.x_ಸಂದೇಶ_ಮೌಲ್ಯ + pxTimer->x_Timer_Period_ಒಳ_Ticks, x_ಸಮಯ_Now, x_ಸಂದೇಶ.u.xTimerParameters.x_ಸಂದೇಶ_ಮೌಲ್ಯ ) != pdFALSE )
                        {
                            /* The timer expired before it was added to the active
                             * timer list.  Process it now. */
                            ಆದರೆ( ( pxTimer->uc_ಸ್ಥಿತಿ & tmr_ಸ್ಥಿತಿ_IS_AUTORELOAD ) != 0 )
                            {
                                prvReloadTimer( pxTimer, x_ಸಂದೇಶ.u.xTimerParameters.x_ಸಂದೇಶ_ಮೌಲ್ಯ + pxTimer->x_Timer_Period_ಒಳ_Ticks, x_ಸಮಯ_Now );
                            }
                            ಇಲ್ಲದಿದ್ದರೆ
                            {
                                pxTimer->uc_ಸ್ಥಿತಿ &= ( ( uint8_t ) ~tmr_ಸ್ಥಿತಿ_IS_ಸಕ್ರಿಯ );
                            }

                            /* Call the timer callback. */
                            traceTIMER_EXPIRED( pxTimer );
                            pxTimer->pxCallbackFunction( ( TimerHandle_t ) pxTimer );
                        }
                        ಇಲ್ಲದಿದ್ದರೆ
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }

                        ನಿಲ್ಲಿಸು;

                    ಸ್ಥಿತಿ tmrCOMMAND_STOP:
                    ಸ್ಥಿತಿ tmrCOMMAND_STOP_FROM_ISR:
                        /* The timer has already been removed from the active list. */
                        pxTimer->uc_ಸ್ಥಿತಿ &= ( ( uint8_t ) ~tmr_ಸ್ಥಿತಿ_IS_ಸಕ್ರಿಯ );
                        ನಿಲ್ಲಿಸು;

                    ಸ್ಥಿತಿ tmrCOMMAND_CHANGE_PERIOD:
                    ಸ್ಥಿತಿ tmrCOMMAND_CHANGE_PERIOD_FROM_ISR:
                        pxTimer->uc_ಸ್ಥಿತಿ |= tmr_ಸ್ಥಿತಿ_IS_ಸಕ್ರಿಯ;
                        pxTimer->x_Timer_Period_ಒಳ_Ticks = x_ಸಂದೇಶ.u.xTimerParameters.x_ಸಂದೇಶ_ಮೌಲ್ಯ;
                        configASSERT( ( pxTimer->x_Timer_Period_ಒಳ_Ticks > 0 ) );

                        /* The new period does not really have a reference, and can
                         * be longer or shorter than the old one.  The command time is
                         * therefore set to the current time, and as the period cannot
                         * be zero the next expiry time can only be in the future,
                         * meaning (unlike for the xTimerStart() case above) there is
                         * no fail case that needs to be handled here. */
                        ( ಶೂನ್ಯ ) prv_ಸೇರಿಸು_Timer_ಒಳ_ಸಕ್ರಿಯ_List( pxTimer, ( x_ಸಮಯ_Now + pxTimer->x_Timer_Period_ಒಳ_Ticks ), x_ಸಮಯ_Now, x_ಸಮಯ_Now );
                        ನಿಲ್ಲಿಸು;

                    ಸ್ಥಿತಿ tmrCOMMAND_DELETE:
                        #ಆದರೆ ( config_SUPPORT_DYNAMIC_ಹಂಚಿಕೆ == 1 )
                        {
                            /* The timer has already been removed from the active list,
                             * just free up the memory if the memory was dynamically
                             * allocated. */
                            ಆದರೆ( ( pxTimer->uc_ಸ್ಥಿತಿ & tmr_ಸ್ಥಿತಿ_IS_STATICALLY_ALLOCATED ) == ( uint8_t ) 0 )
                            {
                                v_ಪೋರ್ಟ್_Free( pxTimer );
                            }
                            ಇಲ್ಲದಿದ್ದರೆ
                            {
                                pxTimer->uc_ಸ್ಥಿತಿ &= ( ( uint8_t ) ~tmr_ಸ್ಥಿತಿ_IS_ಸಕ್ರಿಯ );
                            }
                        }
                        #ಇಲ್ಲದಿದ್ದರೆ /* if ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) */
                        {
                            /* If dynamic allocation is not enabled, the memory
                             * could not have been dynamically allocated. So there is
                             * no need to free the memory - just mark the timer as
                             * "not active". */
                            pxTimer->uc_ಸ್ಥಿತಿ &= ( ( uint8_t ) ~tmr_ಸ್ಥಿತಿ_IS_ಸಕ್ರಿಯ );
                        }
                        #ಆದರೆ_ಅಂತ್ಯ /* configSUPPORT_DYNAMIC_ALLOCATION */
                        ನಿಲ್ಲಿಸು;

                    ಪೂರ್ವನಿಯೋಜಿತ:
                        /* Don't expect to get here. */
                        ನಿಲ್ಲಿಸು;
                }
            }
        }
    }
/*-----------------------------------------------------------*/

    ಸ್ಥಿರ ಶೂನ್ಯ prvSwitchTimerLists( ಶೂನ್ಯ )
    {
        TickType_t x_ಮುಂದಿನ_Expire_ಸಮಯ;
        List_t * px_ತಾತ್ಕಾಲಿಕ;

        /* The tick count has overflowed.  The timer lists must be switched.
         * If there are any timers still referenced from the current timer list
         * then they must have expired and should be processed before the lists
         * are switched. */
        ಆಗ( list_LIST_IS_ಖಾಲಿಗಳು( pxCurrentTimerList ) == pdFALSE )
        {
            x_ಮುಂದಿನ_Expire_ಸಮಯ = list_GET_ಅಂಶ_ಮೌಲ್ಯ_ದ_ತಲೆ_ನಮೂದು( pxCurrentTimerList );

            /* Process the expired timer.  For auto-reload timers, be careful to
             * process only expirations that occur on the current list.  Further
             * expirations must wait until after the lists are switched. */
            prvProcessExpiredTimer( x_ಮುಂದಿನ_Expire_ಸಮಯ, tmr_MAX_ಸಮಯ_BEFORE_ಅತಿಪ್ರವಾಹ );
        }

        px_ತಾತ್ಕಾಲಿಕ = pxCurrentTimerList;
        pxCurrentTimerList = px_ಅತಿಪ್ರವಾಹ_Timer_List;
        px_ಅತಿಪ್ರವಾಹ_Timer_List = px_ತಾತ್ಕಾಲಿಕ;
    }
/*-----------------------------------------------------------*/

    ಸ್ಥಿರ ಶೂನ್ಯ prv_Check_For_Valid_List_And_ಸರತಿ( ಶೂನ್ಯ )
    {
        /* Check that the list from which active timers are referenced, and the
         * queue used to communicate with the timer service, have been
         * initialised. */
        taskENTER_CRITICAL( &x_Timer_ಬೀಗ );
        {
            ಆದರೆ( x_Timer_ಸರತಿ == ಖಾಲಿ )
            {
                vListInitialise( &x_ಸಕ್ರಿಯ_Timer_List1 );
                vListInitialise( &x_ಸಕ್ರಿಯ_Timer_List2 );
                pxCurrentTimerList = &x_ಸಕ್ರಿಯ_Timer_List1;
                px_ಅತಿಪ್ರವಾಹ_Timer_List = &x_ಸಕ್ರಿಯ_Timer_List2;

                #ಆದರೆ ( config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 1 )
                {
                    /* The timer queue is allocated statically in case
                     * configSUPPORT_DYNAMIC_ALLOCATION is 0. */
                    PRIVILEGED_ದತ್ತಾಂಶ ಸ್ಥಿರ Static_ಸರತಿ_t x_Static_Timer_ಸರತಿ;                                                                          /*lint !e956 Ok to declare in this manner to prevent additional conditional compilation guards in other locations. */
                    PRIVILEGED_ದತ್ತಾಂಶ ಸ್ಥಿರ uint8_t uc_Static_Timer_ಸರತಿ_Storage[ ( size_t ) config_TIMER_ಸರತಿ_ಉದ್ದಳತೆ * ಗಾತ್ರ( Daemon_Task_ಸಂದೇಶ_t ) ]; /*lint !e956 Ok to declare in this manner to prevent additional conditional compilation guards in other locations. */

                    x_Timer_ಸರತಿ = x_ಸರತಿ_Create_Static( ( UBaseType_t ) config_TIMER_ಸರತಿ_ಉದ್ದಳತೆ, ( UBaseType_t ) ಗಾತ್ರ( Daemon_Task_ಸಂದೇಶ_t ), &( uc_Static_Timer_ಸರತಿ_Storage[ 0 ] ), &x_Static_Timer_ಸರತಿ );
                }
                #ಇಲ್ಲದಿದ್ದರೆ
                {
                    x_Timer_ಸರತಿ = x_ಸರತಿ_Create( ( UBaseType_t ) config_TIMER_ಸರತಿ_ಉದ್ದಳತೆ, ಗಾತ್ರ( Daemon_Task_ಸಂದೇಶ_t ) );
                }
                #ಆದರೆ_ಅಂತ್ಯ /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */

                #ಆದರೆ ( config_ಸರತಿ_REGISTRY_ಗಾತ್ರ > 0 )
                {
                    ಆದರೆ( x_Timer_ಸರತಿ != ಖಾಲಿ )
                    {
                        v_ಸರತಿ_ಕೂಡಿಸು_ಗಮ್ಯ_Registry( x_Timer_ಸರತಿ, "TmrQ" );
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #ಆದರೆ_ಅಂತ್ಯ /* configQUEUE_REGISTRY_SIZE */
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        taskEXIT_CRITICAL( &x_Timer_ಬೀಗ );
    }
/*-----------------------------------------------------------*/

    ತಳ_Type_t x_Timer_Is_Timer_ಸಕ್ರಿಯ( TimerHandle_t xTimer )
    {
        ತಳ_Type_t xReturn;
        Timer_t * pxTimer = xTimer;

        configASSERT( xTimer );

        /* Is the timer in the list of active timers? */
        taskENTER_CRITICAL( &x_Timer_ಬೀಗ );
        {
            ಆದರೆ( ( pxTimer->uc_ಸ್ಥಿತಿ & tmr_ಸ್ಥಿತಿ_IS_ಸಕ್ರಿಯ ) == 0 )
            {
                xReturn = pdFALSE;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                xReturn = pdTRUE;
            }
        }
        taskEXIT_CRITICAL( &x_Timer_ಬೀಗ );

        ಹಿಂತಿರುಗಿ xReturn;
    } /*lint !e818 Can't be pointer to const due to the typedef. */
/*-----------------------------------------------------------*/

    ಶೂನ್ಯ * pvTimerGetTimerID( ಸ್ಥಿರಾಂಕ TimerHandle_t xTimer )
    {
        Timer_t * ಸ್ಥಿರಾಂಕ pxTimer = xTimer;
        ಶೂನ್ಯ * pvReturn;

        configASSERT( xTimer );

        taskENTER_CRITICAL( &x_Timer_ಬೀಗ );
        {
            pvReturn = pxTimer->pvTimerID;
        }
        taskEXIT_CRITICAL( &x_Timer_ಬೀಗ );

        ಹಿಂತಿರುಗಿ pvReturn;
    }
/*-----------------------------------------------------------*/

    ಶೂನ್ಯ v_Timer_ಗಣ_Timer_ID( TimerHandle_t xTimer,
                           ಶೂನ್ಯ * pvNewID )
    {
        Timer_t * ಸ್ಥಿರಾಂಕ pxTimer = xTimer;

        configASSERT( xTimer );

        taskENTER_CRITICAL( &x_Timer_ಬೀಗ );
        {
            pxTimer->pvTimerID = pvNewID;
        }
        taskEXIT_CRITICAL( &x_Timer_ಬೀಗ );
    }
/*-----------------------------------------------------------*/

    #ಆದರೆ ( INCLUDE_xTimerPendFunctionCall == 1 )

        ತಳ_Type_t xTimerPendFunctionCallFromISR( PendedFunction_t x_Function_ಗಮ್ಯ_Pend,
                                                  ಶೂನ್ಯ * pvParameter1,
                                                  uint32_t ulParameter2,
                                                  ತಳ_Type_t * px_Higher_ಆದ್ಯತೆ_Task_Woken )
        {
            Daemon_Task_ಸಂದೇಶ_t x_ಸಂದೇಶ;
            ತಳ_Type_t xReturn;

            /* Complete the message with the function parameters and post it to the
             * daemon task. */
            x_ಸಂದೇಶ.x_ಸಂದೇಶ_ID = tmrCOMMAND_EXECUTE_CALLBACK_FROM_ISR;
            x_ಸಂದೇಶ.u.xCallbackParameters.pxCallbackFunction = x_Function_ಗಮ್ಯ_Pend;
            x_ಸಂದೇಶ.u.xCallbackParameters.pvParameter1 = pvParameter1;
            x_ಸಂದೇಶ.u.xCallbackParameters.ulParameter2 = ulParameter2;

            xReturn = x_ಸರತಿ_Send_From_ISR( x_Timer_ಸರತಿ, &x_ಸಂದೇಶ, px_Higher_ಆದ್ಯತೆ_Task_Woken );

            tracePEND_FUNC_CALL_FROM_ISR( x_Function_ಗಮ್ಯ_Pend, pvParameter1, ulParameter2, xReturn );

            ಹಿಂತಿರುಗಿ xReturn;
        }

    #ಆದರೆ_ಅಂತ್ಯ /* INCLUDE_xTimerPendFunctionCall */
/*-----------------------------------------------------------*/

    #ಆದರೆ ( INCLUDE_xTimerPendFunctionCall == 1 )

        ತಳ_Type_t xTimerPendFunctionCall( PendedFunction_t x_Function_ಗಮ್ಯ_Pend,
                                           ಶೂನ್ಯ * pvParameter1,
                                           uint32_t ulParameter2,
                                           TickType_t x_Ticks_ಗಮ್ಯ_Wait )
        {
            Daemon_Task_ಸಂದೇಶ_t x_ಸಂದೇಶ;
            ತಳ_Type_t xReturn;

            /* This function can only be called after a timer has been created or
             * after the scheduler has been started because, until then, the timer
             * queue does not exist. */
            configASSERT( x_Timer_ಸರತಿ );

            /* Complete the message with the function parameters and post it to the
             * daemon task. */
            x_ಸಂದೇಶ.x_ಸಂದೇಶ_ID = tmrCOMMAND_EXECUTE_CALLBACK;
            x_ಸಂದೇಶ.u.xCallbackParameters.pxCallbackFunction = x_Function_ಗಮ್ಯ_Pend;
            x_ಸಂದೇಶ.u.xCallbackParameters.pvParameter1 = pvParameter1;
            x_ಸಂದೇಶ.u.xCallbackParameters.ulParameter2 = ulParameter2;

            xReturn = x_ಸರತಿ_Send_ಗಮ್ಯ_ಹಿಂಭಾಗ( x_Timer_ಸರತಿ, &x_ಸಂದೇಶ, x_Ticks_ಗಮ್ಯ_Wait );

            tracePEND_FUNC_CALL( x_Function_ಗಮ್ಯ_Pend, pvParameter1, ulParameter2, xReturn );

            ಹಿಂತಿರುಗಿ xReturn;
        }

    #ಆದರೆ_ಅಂತ್ಯ /* INCLUDE_xTimerPendFunctionCall */
/*-----------------------------------------------------------*/

    #ಆದರೆ ( configUSE_TRACE_FACILITY == 1 )

        UBaseType_t ux_Timer_Get_Timer_ಸಂಖ್ಯೆ( TimerHandle_t xTimer )
        {
            ಹಿಂತಿರುಗಿ ( ( Timer_t * ) xTimer )->ux_Timer_ಸಂಖ್ಯೆ;
        }

    #ಆದರೆ_ಅಂತ್ಯ /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

    #ಆದರೆ ( configUSE_TRACE_FACILITY == 1 )

        ಶೂನ್ಯ v_Timer_ಗಣ_Timer_ಸಂಖ್ಯೆ( TimerHandle_t xTimer,
                                   UBaseType_t ux_Timer_ಸಂಖ್ಯೆ )
        {
            ( ( Timer_t * ) xTimer )->ux_Timer_ಸಂಖ್ಯೆ = ux_Timer_ಸಂಖ್ಯೆ;
        }

    #ಆದರೆ_ಅಂತ್ಯ /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

/* This entire source file will be skipped if the application is not configured
 * to include software timer functionality.  If you want to include software timer
 * functionality then ensure configUSE_TIMERS is set to 1 in FreeRTOSConfig.h. */
#ಆದರೆ_ಅಂತ್ಯ /* configUSE_TIMERS == 1 */
