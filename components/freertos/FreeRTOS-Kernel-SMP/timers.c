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

/* Standard includes. */
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#ਅਨ੍ਤਰ੍ਭਾਵ "FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "queue.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "timers.h"

#ਜੇ ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 0 )
    #ਗਲਤੀ configUSE_TIMERS must be ਸੈੱਟ ਮੰਜ਼ਿਲ 1 ਮੰਜ਼ਿਲ make the xTimerPendFunctionCall() function ਉਪਲਬਧ.
#ਜੇ_ਅੰਤ

/* The MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ MPU_WRAPPERS_INCLUDED_FROM_API_FILE


/* This entire source file will be skipped if the application is not configured
 * to include software timer functionality.  This #if is closed at the very bottom
 * of this file.  If you want to include software timer functionality then ensure
 * configUSE_TIMERS is set to 1 in FreeRTOSConfig.h. */
#ਜੇ ( configUSE_TIMERS == 1 )

/* Misc definitions. */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tmrNO_DELAY                    ( ( TickType_t ) 0U )
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tmr_MAX_ਸਮਾਂ_BEFORE_ਉਛਾਲ    ( ( TickType_t ) -1 )

/* The name assigned to the timer service task. This can be overridden by
 * defining configTIMER_SERVICE_TASK_NAME in FreeRTOSConfig.h. */
    #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configTIMER_SERVICE_TASK_NAME
        #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configTIMER_SERVICE_TASK_NAME    "Tmr Svc"
    #ਜੇ_ਅੰਤ

    #ਜੇ ( ( config_ਸੰਖਿਆ_ਦਾ_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )

/* The core affinity assigned to the timer service task on SMP systems.
 * This can be overridden by defining configTIMER_SERVICE_TASK_CORE_AFFINITY in FreeRTOSConfig.h. */
        #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ configTIMER_SERVICE_TASK_CORE_AFFINITY
            #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ configTIMER_SERVICE_TASK_CORE_AFFINITY    tskNO_AFFINITY
        #ਜੇ_ਅੰਤ
    #ਜੇ_ਅੰਤ /* #if ( ( configNUMBER_OF_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */

/* Bit definitions used in the ucStatus member of a timer structure. */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tmr_ਸਥਿਤੀ_IS_ਸਰਗਰਮ                  ( 0x01U )
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tmr_ਸਥਿਤੀ_IS_STATICALLY_ALLOCATED    ( 0x02U )
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ tmr_ਸਥਿਤੀ_IS_AUTORELOAD              ( 0x04U )

/* The definition of the timers themselves. */
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ tmrTimerControl                                               /* The old naming convention is used to prevent breaking kernel aware debuggers. */
    {
        ਅਚਲ ਅੱਖਰ * pcTimerName;                                                /**< Text name.  This is not used by the kernel, it is included simply to make debugging easier. */
        List_ਵਸਤੂ_t x_Timer_List_ਵਸਤੂ;                                               /**< Standard linked list item as used by all kernel features for event management. */
        TickType_t x_Timer_Period_ਵਿੱਚ_Ticks;                                          /**< How quickly and often the timer expires. */
        ਖਾਲੀ * pvTimerID;                                                        /**< An ID to identify the timer.  This allows the timer to be identified when the same callback is used for multiple timers. */
        ਪੋਰਟ_TIMER_CALLBACK_ATTRIBUTE TimerCallbackFunction_t pxCallbackFunction; /**< The function that will be called when the timer expires. */
        #ਜੇ ( configUSE_TRACE_FACILITY == 1 )
            UBaseType_t ux_Timer_ਸੰਖਿਆ;                                           /**< An ID assigned by trace tools such as FreeRTOS+Trace */
        #ਜੇ_ਅੰਤ
        uint8_t uc_ਸਥਿਤੀ;                                                        /**< Holds bits to say if the timer was statically allocated or not, and if it is active or not. */
    } xTIMER;

/* The old xTIMER name is maintained above then typedefed to the new Timer_t
 * name below to enable the use of older kernel aware debuggers. */
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ xTIMER Timer_t;

/* The definition of messages that can be sent and received on the timer queue.
 * Two types of message can be queued - messages that manipulate a software timer,
 * and messages that request the execution of a non-timer related callback.  The
 * two message types are defined in two separate structures, xTimerParametersType
 * and xCallbackParametersType respectively. */
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ tmrTimerParameters
    {
        TickType_t x_ਸੁਨੇਹਾ_ਮੁੱਲ; /**< An optional value used by a subset of commands, for example, when changing the period of a timer. */
        Timer_t * pxTimer;        /**< The timer to which the command will be applied. */
    } TimerParameter_t;


    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ tmrCallbackParameters
    {
        ਪੋਰਟ_TIMER_CALLBACK_ATTRIBUTE
        PendedFunction_t pxCallbackFunction; /* << The callback function to execute. */
        ਖਾਲੀ * pvParameter1;                 /* << The value that will be used as the callback functions first parameter. */
        uint32_t ulParameter2;               /* << The value that will be used as the callback functions second parameter. */
    } CallbackParameters_t;

/* The structure that contains the two message types, along with an identifier
 * that is used to determine which message type is valid. */
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ tmr_Timer_ਕਤਾਰ_ਸੁਨੇਹਾ
    {
        ਅਧਾਰ_Type_t x_ਸੁਨੇਹਾ_ID; /**< The command being sent to the timer service task. */
        ਸੰਘ
        {
            TimerParameter_t xTimerParameters;

            /* Don't include xCallbackParameters if it is not going to be used as
             * it makes the structure (and therefore the timer queue) larger. */
            #ਜੇ ( INCLUDE_xTimerPendFunctionCall == 1 )
                CallbackParameters_t xCallbackParameters;
            #ਜੇ_ਅੰਤ /* INCLUDE_xTimerPendFunctionCall */
        } u;
    } Daemon_Task_ਸੁਨੇਹਾ_t;

/* The list in which active timers are stored.  Timers are referenced in expire
 * time order, with the nearest expiry time at the front of the list.  Only the
 * timer service task is allowed to access these lists.
 * xActiveTimerList1 and xActiveTimerList2 could be at function scope but that
 * breaks some kernel aware debuggers, and debuggers that reply on removing the
 * static qualifier. */
    PRIVILEGED_ਅੰਕੜੇ ਸਥਿਰ List_t x_ਸਰਗਰਮ_Timer_List1;
    PRIVILEGED_ਅੰਕੜੇ ਸਥਿਰ List_t x_ਸਰਗਰਮ_Timer_List2;
    PRIVILEGED_ਅੰਕੜੇ ਸਥਿਰ List_t * pxCurrentTimerList;
    PRIVILEGED_ਅੰਕੜੇ ਸਥਿਰ List_t * px_ਉਛਾਲ_Timer_List;

/* A queue that is used to send commands to the timer service task. */
    PRIVILEGED_ਅੰਕੜੇ ਸਥਿਰ ਕਤਾਰ_Handle_t x_Timer_ਕਤਾਰ = ਨਿਰਾ;
    PRIVILEGED_ਅੰਕੜੇ ਸਥਿਰ TaskHandle_t xTimerTaskHandle = ਨਿਰਾ;

/*-----------------------------------------------------------*/

/*
 * Initialise the infrastructure used by the timer service task if it has not
 * been initialised already.
 */
    ਸਥਿਰ ਖਾਲੀ prv_Check_For_Valid_List_And_ਕਤਾਰ( ਖਾਲੀ ) PRIVILEGED_FUNCTION;

/*
 * The timer service task (daemon).  Timer functionality is controlled by this
 * task.  Other tasks communicate with the timer service task using the
 * xTimerQueue queue.
 */
    ਸਥਿਰ ਪੋਰਟ_TASK_FUNCTION_PROTO( prvTimerTask, pvParameters ) PRIVILEGED_FUNCTION;

/*
 * Called by the timer service task to interpret and process a command it
 * received on the timer queue.
 */
    ਸਥਿਰ ਖਾਲੀ prvProcessReceivedCommands( ਖਾਲੀ ) PRIVILEGED_FUNCTION;

/*
 * Insert the timer into either xActiveTimerList1, or xActiveTimerList2,
 * depending on if the expire time causes a timer counter overflow.
 */
    ਸਥਿਰ ਅਧਾਰ_Type_t prv_ਪਾਓ_Timer_ਵਿੱਚ_ਸਰਗਰਮ_List( Timer_t * ਅਚਲ pxTimer,
                                                  ਅਚਲ TickType_t x_ਅਗਲਾ_Expiry_ਸਮਾਂ,
                                                  ਅਚਲ TickType_t x_ਸਮਾਂ_Now,
                                                  ਅਚਲ TickType_t x_Command_ਸਮਾਂ ) PRIVILEGED_FUNCTION;

/*
 * Reload the specified auto-reload timer.  If the reloading is backlogged,
 * clear the backlog, calling the callback for each additional reload.  When
 * this function returns, the next expiry time is after xTimeNow.
 */
    ਸਥਿਰ ਖਾਲੀ prvReloadTimer( Timer_t * ਅਚਲ pxTimer,
                                TickType_t x_Expired_ਸਮਾਂ,
                                ਅਚਲ TickType_t x_ਸਮਾਂ_Now ) PRIVILEGED_FUNCTION;

/*
 * An active timer has reached its expire time.  Reload the timer if it is an
 * auto-reload timer, then call its callback.
 */
    ਸਥਿਰ ਖਾਲੀ prvProcessExpiredTimer( ਅਚਲ TickType_t x_ਅਗਲਾ_Expire_ਸਮਾਂ,
                                        ਅਚਲ TickType_t x_ਸਮਾਂ_Now ) PRIVILEGED_FUNCTION;

/*
 * The tick count has overflowed.  Switch the timer lists after ensuring the
 * current timer list does not still reference some timers.
 */
    ਸਥਿਰ ਖਾਲੀ prvSwitchTimerLists( ਖਾਲੀ ) PRIVILEGED_FUNCTION;

/*
 * Obtain the current tick count, setting *pxTimerListsWereSwitched to pdTRUE
 * if a tick count overflow occurred since prvSampleTimeNow() was last called.
 */
    ਸਥਿਰ TickType_t prv_Sample_ਸਮਾਂ_Now( ਅਧਾਰ_Type_t * ਅਚਲ pxTimerListsWereSwitched ) PRIVILEGED_FUNCTION;

/*
 * If the timer list contains any active timers then return the expire time of
 * the timer that will expire first and set *pxListWasEmpty to false.  If the
 * timer list does not contain any timers then return 0 and set *pxListWasEmpty
 * to pdTRUE.
 */
    ਸਥਿਰ TickType_t prv_Get_ਅਗਲਾ_Expire_ਸਮਾਂ( ਅਧਾਰ_Type_t * ਅਚਲ px_List_Was_ਖਾਲੀ_ਥਾਵਾਂ ) PRIVILEGED_FUNCTION;

/*
 * If a timer has expired, process it.  Otherwise, block the timer service task
 * until either a timer does expire or a command is received.
 */
    ਸਥਿਰ ਖਾਲੀ prv_Process_Timer_Or_ਖੰਡ_Task( ਅਚਲ TickType_t x_ਅਗਲਾ_Expire_ਸਮਾਂ,
                                            ਅਧਾਰ_Type_t x_List_Was_ਖਾਲੀ_ਥਾਵਾਂ ) PRIVILEGED_FUNCTION;

/*
 * Called after a Timer_t structure has been allocated either statically or
 * dynamically to fill in the structure's members.
 */
    ਸਥਿਰ ਖਾਲੀ prvInitialiseNewTimer( ਅਚਲ ਅੱਖਰ * ਅਚਲ pcTimerName,
                                       ਅਚਲ TickType_t x_Timer_Period_ਵਿੱਚ_Ticks,
                                       ਅਚਲ ਅਧਾਰ_Type_t xAutoReload,
                                       ਖਾਲੀ * ਅਚਲ pvTimerID,
                                       TimerCallbackFunction_t pxCallbackFunction,
                                       Timer_t * pxNewTimer ) PRIVILEGED_FUNCTION;
/*-----------------------------------------------------------*/

    ਅਧਾਰ_Type_t xTimerCreateTimerTask( ਖਾਲੀ )
    {
        ਅਧਾਰ_Type_t xReturn = pdFAIL;

        traceENTER_xTimerCreateTimerTask();

        /* This function is called when the scheduler is started if
         * configUSE_TIMERS is set to 1.  Check that the infrastructure used by the
         * timer service task has been created/initialised.  If timers have already
         * been created then the initialisation will already have been performed. */
        prv_Check_For_Valid_List_And_ਕਤਾਰ();

        ਜੇ( x_Timer_ਕਤਾਰ != ਨਿਰਾ )
        {
            #ਜੇ ( ( config_ਸੰਖਿਆ_ਦਾ_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
            {
                #ਜੇ ( config_SUPPORT_STATIC_ਵੰਡ == 1 )
                {
                    StaticTask_t * pxTimerTaskTCBBuffer = ਨਿਰਾ;
                    ਢੇਰ_Type_t * px_Timer_Task_ਢੇਰ_ਬਫਰ = ਨਿਰਾ;
                    config_ਢੇਰ_ਡੂੰਘਾਈ_TYPE ux_Timer_Task_ਢੇਰ_ਮਾਪ;

                    v_Application_Get_Timer_Task_ਸਮ੍ਰਿਤੀ( &pxTimerTaskTCBBuffer, &px_Timer_Task_ਢੇਰ_ਬਫਰ, &ux_Timer_Task_ਢੇਰ_ਮਾਪ );
                    xTimerTaskHandle = x_Task_Create_Static_Affinity_ਸੈੱਟ( prvTimerTask,
                                                                     configTIMER_SERVICE_TASK_NAME,
                                                                     ux_Timer_Task_ਢੇਰ_ਮਾਪ,
                                                                     ਨਿਰਾ,
                                                                     ( ( UBaseType_t ) config_TIMER_TASK_ਤਰਜੀਹ ) | ਪੋਰਟ_PRIVILEGE_ਦੋਆਧਾਰੀ_ਅੰਕ,
                                                                     px_Timer_Task_ਢੇਰ_ਬਫਰ,
                                                                     pxTimerTaskTCBBuffer,
                                                                     configTIMER_SERVICE_TASK_CORE_AFFINITY );

                    ਜੇ( xTimerTaskHandle != ਨਿਰਾ )
                    {
                        xReturn = pdPASS;
                    }
                }
                #ਨਹੀਂਤਾਂ /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
                {
                    xReturn = x_Task_Create_Affinity_ਸੈੱਟ( prvTimerTask,
                                                      configTIMER_SERVICE_TASK_NAME,
                                                      config_TIMER_TASK_ਢੇਰ_ਡੂੰਘਾਈ,
                                                      ਨਿਰਾ,
                                                      ( ( UBaseType_t ) config_TIMER_TASK_ਤਰਜੀਹ ) | ਪੋਰਟ_PRIVILEGE_ਦੋਆਧਾਰੀ_ਅੰਕ,
                                                      configTIMER_SERVICE_TASK_CORE_AFFINITY,
                                                      &xTimerTaskHandle );
                }
                #ਜੇ_ਅੰਤ /* configSUPPORT_STATIC_ALLOCATION */
            }
            #ਨਹੀਂਤਾਂ /* #if ( ( configNUMBER_OF_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            {
                #ਜੇ ( config_SUPPORT_STATIC_ਵੰਡ == 1 )
                {
                    StaticTask_t * pxTimerTaskTCBBuffer = ਨਿਰਾ;
                    ਢੇਰ_Type_t * px_Timer_Task_ਢੇਰ_ਬਫਰ = ਨਿਰਾ;
                    config_ਢੇਰ_ਡੂੰਘਾਈ_TYPE ux_Timer_Task_ਢੇਰ_ਮਾਪ;

                    v_Application_Get_Timer_Task_ਸਮ੍ਰਿਤੀ( &pxTimerTaskTCBBuffer, &px_Timer_Task_ਢੇਰ_ਬਫਰ, &ux_Timer_Task_ਢੇਰ_ਮਾਪ );
                    xTimerTaskHandle = xTaskCreateStatic( prvTimerTask,
                                                          configTIMER_SERVICE_TASK_NAME,
                                                          ux_Timer_Task_ਢੇਰ_ਮਾਪ,
                                                          ਨਿਰਾ,
                                                          ( ( UBaseType_t ) config_TIMER_TASK_ਤਰਜੀਹ ) | ਪੋਰਟ_PRIVILEGE_ਦੋਆਧਾਰੀ_ਅੰਕ,
                                                          px_Timer_Task_ਢੇਰ_ਬਫਰ,
                                                          pxTimerTaskTCBBuffer );

                    ਜੇ( xTimerTaskHandle != ਨਿਰਾ )
                    {
                        xReturn = pdPASS;
                    }
                }
                #ਨਹੀਂਤਾਂ /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
                {
                    xReturn = xTaskCreate( prvTimerTask,
                                           configTIMER_SERVICE_TASK_NAME,
                                           config_TIMER_TASK_ਢੇਰ_ਡੂੰਘਾਈ,
                                           ਨਿਰਾ,
                                           ( ( UBaseType_t ) config_TIMER_TASK_ਤਰਜੀਹ ) | ਪੋਰਟ_PRIVILEGE_ਦੋਆਧਾਰੀ_ਅੰਕ,
                                           &xTimerTaskHandle );
                }
                #ਜੇ_ਅੰਤ /* configSUPPORT_STATIC_ALLOCATION */
            }
            #ਜੇ_ਅੰਤ /* #if ( ( configNUMBER_OF_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
        }
        ਨਹੀਂਤਾਂ
        {
            mtCOVERAGE_TEST_MARKER();
        }

        configASSERT( xReturn );

        traceRETURN_xTimerCreateTimerTask( xReturn );

        ਵਾਪਸ xReturn;
    }
/*-----------------------------------------------------------*/

    #ਜੇ ( config_SUPPORT_DYNAMIC_ਵੰਡ == 1 )

        TimerHandle_t xTimerCreate( ਅਚਲ ਅੱਖਰ * ਅਚਲ pcTimerName,
                                    ਅਚਲ TickType_t x_Timer_Period_ਵਿੱਚ_Ticks,
                                    ਅਚਲ ਅਧਾਰ_Type_t xAutoReload,
                                    ਖਾਲੀ * ਅਚਲ pvTimerID,
                                    TimerCallbackFunction_t pxCallbackFunction )
        {
            Timer_t * pxNewTimer;

            traceENTER_xTimerCreate( pcTimerName, x_Timer_Period_ਵਿੱਚ_Ticks, xAutoReload, pvTimerID, pxCallbackFunction );

            /* MISRA Ref 11.5.1 [Malloc memory assignment] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
            /* coverity[misra_c_2012_rule_11_5_violation] */
            pxNewTimer = ( Timer_t * ) pv_ਪੋਰਟ_Malloc( ਆਕਾਰ( Timer_t ) );

            ਜੇ( pxNewTimer != ਨਿਰਾ )
            {
                /* Status is thus far zero as the timer is not created statically
                 * and has not been started.  The auto-reload bit may get set in
                 * prvInitialiseNewTimer. */
                pxNewTimer->uc_ਸਥਿਤੀ = 0x00;
                prvInitialiseNewTimer( pcTimerName, x_Timer_Period_ਵਿੱਚ_Ticks, xAutoReload, pvTimerID, pxCallbackFunction, pxNewTimer );
            }

            traceRETURN_xTimerCreate( pxNewTimer );

            ਵਾਪਸ pxNewTimer;
        }

    #ਜੇ_ਅੰਤ /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

    #ਜੇ ( config_SUPPORT_STATIC_ਵੰਡ == 1 )

        TimerHandle_t xTimerCreateStatic( ਅਚਲ ਅੱਖਰ * ਅਚਲ pcTimerName,
                                          ਅਚਲ TickType_t x_Timer_Period_ਵਿੱਚ_Ticks,
                                          ਅਚਲ ਅਧਾਰ_Type_t xAutoReload,
                                          ਖਾਲੀ * ਅਚਲ pvTimerID,
                                          TimerCallbackFunction_t pxCallbackFunction,
                                          StaticTimer_t * px_Timer_ਬਫਰ )
        {
            Timer_t * pxNewTimer;

            traceENTER_xTimerCreateStatic( pcTimerName, x_Timer_Period_ਵਿੱਚ_Ticks, xAutoReload, pvTimerID, pxCallbackFunction, px_Timer_ਬਫਰ );

            #ਜੇ ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticTimer_t equals the size of the real timer
                 * structure. */
                ਅਸਥਿਰ size_t x_ਮਾਪ = ਆਕਾਰ( StaticTimer_t );
                configASSERT( x_ਮਾਪ == ਆਕਾਰ( Timer_t ) );
                ( ਖਾਲੀ ) x_ਮਾਪ; /* Prevent unused variable warning when configASSERT() is not defined. */
            }
            #ਜੇ_ਅੰਤ /* configASSERT_DEFINED */

            /* A pointer to a StaticTimer_t structure MUST be provided, use it. */
            configASSERT( px_Timer_ਬਫਰ );
            /* MISRA Ref 11.3.1 [Misaligned access] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
            /* coverity[misra_c_2012_rule_11_3_violation] */
            pxNewTimer = ( Timer_t * ) px_Timer_ਬਫਰ;

            ਜੇ( pxNewTimer != ਨਿਰਾ )
            {
                /* Timers can be created statically or dynamically so note this
                 * timer was created statically in case it is later deleted.  The
                 * auto-reload bit may get set in prvInitialiseNewTimer(). */
                pxNewTimer->uc_ਸਥਿਤੀ = ( uint8_t ) tmr_ਸਥਿਤੀ_IS_STATICALLY_ALLOCATED;

                prvInitialiseNewTimer( pcTimerName, x_Timer_Period_ਵਿੱਚ_Ticks, xAutoReload, pvTimerID, pxCallbackFunction, pxNewTimer );
            }

            traceRETURN_xTimerCreateStatic( pxNewTimer );

            ਵਾਪਸ pxNewTimer;
        }

    #ਜੇ_ਅੰਤ /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

    ਸਥਿਰ ਖਾਲੀ prvInitialiseNewTimer( ਅਚਲ ਅੱਖਰ * ਅਚਲ pcTimerName,
                                       ਅਚਲ TickType_t x_Timer_Period_ਵਿੱਚ_Ticks,
                                       ਅਚਲ ਅਧਾਰ_Type_t xAutoReload,
                                       ਖਾਲੀ * ਅਚਲ pvTimerID,
                                       TimerCallbackFunction_t pxCallbackFunction,
                                       Timer_t * pxNewTimer )
    {
        /* 0 is not a valid value for xTimerPeriodInTicks. */
        configASSERT( ( x_Timer_Period_ਵਿੱਚ_Ticks > 0 ) );

        /* Ensure the infrastructure used by the timer service task has been
         * created/initialised. */
        prv_Check_For_Valid_List_And_ਕਤਾਰ();

        /* Initialise the timer structure members using the function
         * parameters. */
        pxNewTimer->pcTimerName = pcTimerName;
        pxNewTimer->x_Timer_Period_ਵਿੱਚ_Ticks = x_Timer_Period_ਵਿੱਚ_Ticks;
        pxNewTimer->pvTimerID = pvTimerID;
        pxNewTimer->pxCallbackFunction = pxCallbackFunction;
        v_List_Initialise_ਵਸਤੂ( &( pxNewTimer->x_Timer_List_ਵਸਤੂ ) );

        ਜੇ( xAutoReload != pdFALSE )
        {
            pxNewTimer->uc_ਸਥਿਤੀ |= ( uint8_t ) tmr_ਸਥਿਤੀ_IS_AUTORELOAD;
        }

        traceTIMER_CREATE( pxNewTimer );
    }
/*-----------------------------------------------------------*/

    ਅਧਾਰ_Type_t xTimerGenericCommandFromTask( TimerHandle_t xTimer,
                                             ਅਚਲ ਅਧਾਰ_Type_t xCommandID,
                                             ਅਚਲ TickType_t x_Optional_ਮੁੱਲ,
                                             ਅਧਾਰ_Type_t * ਅਚਲ px_Higher_ਤਰਜੀਹ_Task_Woken,
                                             ਅਚਲ TickType_t x_Ticks_ਮੰਜ਼ਿਲ_Wait )
    {
        ਅਧਾਰ_Type_t xReturn = pdFAIL;
        Daemon_Task_ਸੁਨੇਹਾ_t x_ਸੁਨੇਹਾ;

        ( ਖਾਲੀ ) px_Higher_ਤਰਜੀਹ_Task_Woken;

        traceENTER_xTimerGenericCommandFromTask( xTimer, xCommandID, x_Optional_ਮੁੱਲ, px_Higher_ਤਰਜੀਹ_Task_Woken, x_Ticks_ਮੰਜ਼ਿਲ_Wait );

        configASSERT( xTimer );

        /* Send a message to the timer service task to perform a particular action
         * on a particular timer definition. */
        ਜੇ( x_Timer_ਕਤਾਰ != ਨਿਰਾ )
        {
            /* Send a command to the timer service task to start the xTimer timer. */
            x_ਸੁਨੇਹਾ.x_ਸੁਨੇਹਾ_ID = xCommandID;
            x_ਸੁਨੇਹਾ.u.xTimerParameters.x_ਸੁਨੇਹਾ_ਮੁੱਲ = x_Optional_ਮੁੱਲ;
            x_ਸੁਨੇਹਾ.u.xTimerParameters.pxTimer = xTimer;

            configASSERT( xCommandID < tmr_ਪਹਿਲਾ_FROM_ISR_COMMAND );

            ਜੇ( xCommandID < tmr_ਪਹਿਲਾ_FROM_ISR_COMMAND )
            {
                ਜੇ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING )
                {
                    xReturn = x_ਕਤਾਰ_Send_ਮੰਜ਼ਿਲ_ਪਿੱਠ( x_Timer_ਕਤਾਰ, &x_ਸੁਨੇਹਾ, x_Ticks_ਮੰਜ਼ਿਲ_Wait );
                }
                ਨਹੀਂਤਾਂ
                {
                    xReturn = x_ਕਤਾਰ_Send_ਮੰਜ਼ਿਲ_ਪਿੱਠ( x_Timer_ਕਤਾਰ, &x_ਸੁਨੇਹਾ, tmrNO_DELAY );
                }
            }

            traceTIMER_COMMAND_SEND( xTimer, xCommandID, x_Optional_ਮੁੱਲ, xReturn );
        }
        ਨਹੀਂਤਾਂ
        {
            mtCOVERAGE_TEST_MARKER();
        }

        traceRETURN_xTimerGenericCommandFromTask( xReturn );

        ਵਾਪਸ xReturn;
    }
/*-----------------------------------------------------------*/

    ਅਧਾਰ_Type_t xTimerGenericCommandFromISR( TimerHandle_t xTimer,
                                            ਅਚਲ ਅਧਾਰ_Type_t xCommandID,
                                            ਅਚਲ TickType_t x_Optional_ਮੁੱਲ,
                                            ਅਧਾਰ_Type_t * ਅਚਲ px_Higher_ਤਰਜੀਹ_Task_Woken,
                                            ਅਚਲ TickType_t x_Ticks_ਮੰਜ਼ਿਲ_Wait )
    {
        ਅਧਾਰ_Type_t xReturn = pdFAIL;
        Daemon_Task_ਸੁਨੇਹਾ_t x_ਸੁਨੇਹਾ;

        ( ਖਾਲੀ ) x_Ticks_ਮੰਜ਼ਿਲ_Wait;

        traceENTER_xTimerGenericCommandFromISR( xTimer, xCommandID, x_Optional_ਮੁੱਲ, px_Higher_ਤਰਜੀਹ_Task_Woken, x_Ticks_ਮੰਜ਼ਿਲ_Wait );

        configASSERT( xTimer );

        /* Send a message to the timer service task to perform a particular action
         * on a particular timer definition. */
        ਜੇ( x_Timer_ਕਤਾਰ != ਨਿਰਾ )
        {
            /* Send a command to the timer service task to start the xTimer timer. */
            x_ਸੁਨੇਹਾ.x_ਸੁਨੇਹਾ_ID = xCommandID;
            x_ਸੁਨੇਹਾ.u.xTimerParameters.x_ਸੁਨੇਹਾ_ਮੁੱਲ = x_Optional_ਮੁੱਲ;
            x_ਸੁਨੇਹਾ.u.xTimerParameters.pxTimer = xTimer;

            configASSERT( xCommandID >= tmr_ਪਹਿਲਾ_FROM_ISR_COMMAND );

            ਜੇ( xCommandID >= tmr_ਪਹਿਲਾ_FROM_ISR_COMMAND )
            {
                xReturn = x_ਕਤਾਰ_Send_ਮੰਜ਼ਿਲ_ਪਿੱਠ_From_ISR( x_Timer_ਕਤਾਰ, &x_ਸੁਨੇਹਾ, px_Higher_ਤਰਜੀਹ_Task_Woken );
            }

            traceTIMER_COMMAND_SEND( xTimer, xCommandID, x_Optional_ਮੁੱਲ, xReturn );
        }
        ਨਹੀਂਤਾਂ
        {
            mtCOVERAGE_TEST_MARKER();
        }

        traceRETURN_xTimerGenericCommandFromISR( xReturn );

        ਵਾਪਸ xReturn;
    }
/*-----------------------------------------------------------*/

    TaskHandle_t xTimerGetTimerDaemonTaskHandle( ਖਾਲੀ )
    {
        traceENTER_xTimerGetTimerDaemonTaskHandle();

        /* If xTimerGetTimerDaemonTaskHandle() is called before the scheduler has been
         * started, then xTimerTaskHandle will be NULL. */
        configASSERT( ( xTimerTaskHandle != ਨਿਰਾ ) );

        traceRETURN_xTimerGetTimerDaemonTaskHandle( xTimerTaskHandle );

        ਵਾਪਸ xTimerTaskHandle;
    }
/*-----------------------------------------------------------*/

    TickType_t xTimerGetPeriod( TimerHandle_t xTimer )
    {
        Timer_t * pxTimer = xTimer;

        traceENTER_xTimerGetPeriod( xTimer );

        configASSERT( xTimer );

        traceRETURN_xTimerGetPeriod( pxTimer->x_Timer_Period_ਵਿੱਚ_Ticks );

        ਵਾਪਸ pxTimer->x_Timer_Period_ਵਿੱਚ_Ticks;
    }
/*-----------------------------------------------------------*/

    ਖਾਲੀ v_Timer_ਸੈੱਟ_Reload_ਬਹੁਲਕ( TimerHandle_t xTimer,
                              ਅਚਲ ਅਧਾਰ_Type_t xAutoReload )
    {
        Timer_t * pxTimer = xTimer;

        trace_ENTER_v_Timer_ਸੈੱਟ_Reload_ਬਹੁਲਕ( xTimer, xAutoReload );

        configASSERT( xTimer );
        taskENTER_CRITICAL();
        {
            ਜੇ( xAutoReload != pdFALSE )
            {
                pxTimer->uc_ਸਥਿਤੀ |= ( uint8_t ) tmr_ਸਥਿਤੀ_IS_AUTORELOAD;
            }
            ਨਹੀਂਤਾਂ
            {
                pxTimer->uc_ਸਥਿਤੀ &= ( ( uint8_t ) ~tmr_ਸਥਿਤੀ_IS_AUTORELOAD );
            }
        }
        taskEXIT_CRITICAL();

        trace_RETURN_v_Timer_ਸੈੱਟ_Reload_ਬਹੁਲਕ();
    }
/*-----------------------------------------------------------*/

    ਅਧਾਰ_Type_t x_Timer_Get_Reload_ਬਹੁਲਕ( TimerHandle_t xTimer )
    {
        Timer_t * pxTimer = xTimer;
        ਅਧਾਰ_Type_t xReturn;

        trace_ENTER_x_Timer_Get_Reload_ਬਹੁਲਕ( xTimer );

        configASSERT( xTimer );
        taskENTER_CRITICAL();
        {
            ਜੇ( ( pxTimer->uc_ਸਥਿਤੀ & tmr_ਸਥਿਤੀ_IS_AUTORELOAD ) == 0U )
            {
                /* Not an auto-reload timer. */
                xReturn = pdFALSE;
            }
            ਨਹੀਂਤਾਂ
            {
                /* Is an auto-reload timer. */
                xReturn = pdTRUE;
            }
        }
        taskEXIT_CRITICAL();

        trace_RETURN_x_Timer_Get_Reload_ਬਹੁਲਕ( xReturn );

        ਵਾਪਸ xReturn;
    }

    UBaseType_t ux_Timer_Get_Reload_ਬਹੁਲਕ( TimerHandle_t xTimer )
    {
        UBaseType_t uxReturn;

        trace_ENTER_ux_Timer_Get_Reload_ਬਹੁਲਕ( xTimer );

        uxReturn = ( UBaseType_t ) x_Timer_Get_Reload_ਬਹੁਲਕ( xTimer );

        trace_RETURN_ux_Timer_Get_Reload_ਬਹੁਲਕ( uxReturn );

        ਵਾਪਸ uxReturn;
    }
/*-----------------------------------------------------------*/

    TickType_t x_Timer_Get_Expiry_ਸਮਾਂ( TimerHandle_t xTimer )
    {
        Timer_t * pxTimer = xTimer;
        TickType_t xReturn;

        trace_ENTER_x_Timer_Get_Expiry_ਸਮਾਂ( xTimer );

        configASSERT( xTimer );
        xReturn = list_GET_LIST_ਵਸਤੂ_ਮੁੱਲ( &( pxTimer->x_Timer_List_ਵਸਤੂ ) );

        trace_RETURN_x_Timer_Get_Expiry_ਸਮਾਂ( xReturn );

        ਵਾਪਸ xReturn;
    }
/*-----------------------------------------------------------*/

    #ਜੇ ( config_SUPPORT_STATIC_ਵੰਡ == 1 )
        ਅਧਾਰ_Type_t x_Timer_Get_Static_ਬਫਰ( TimerHandle_t xTimer,
                                          StaticTimer_t ** ppx_Timer_ਬਫਰ )
        {
            ਅਧਾਰ_Type_t xReturn;
            Timer_t * pxTimer = xTimer;

            trace_ENTER_x_Timer_Get_Static_ਬਫਰ( xTimer, ppx_Timer_ਬਫਰ );

            configASSERT( ppx_Timer_ਬਫਰ != ਨਿਰਾ );

            ਜੇ( ( pxTimer->uc_ਸਥਿਤੀ & tmr_ਸਥਿਤੀ_IS_STATICALLY_ALLOCATED ) != 0U )
            {
                /* MISRA Ref 11.3.1 [Misaligned access] */
                /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
                /* coverity[misra_c_2012_rule_11_3_violation] */
                *ppx_Timer_ਬਫਰ = ( StaticTimer_t * ) pxTimer;
                xReturn = pdTRUE;
            }
            ਨਹੀਂਤਾਂ
            {
                xReturn = pdFALSE;
            }

            trace_RETURN_x_Timer_Get_Static_ਬਫਰ( xReturn );

            ਵਾਪਸ xReturn;
        }
    #ਜੇ_ਅੰਤ /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

    ਅਚਲ ਅੱਖਰ * pcTimerGetName( TimerHandle_t xTimer )
    {
        Timer_t * pxTimer = xTimer;

        traceENTER_pcTimerGetName( xTimer );

        configASSERT( xTimer );

        traceRETURN_pcTimerGetName( pxTimer->pcTimerName );

        ਵਾਪਸ pxTimer->pcTimerName;
    }
/*-----------------------------------------------------------*/

    ਸਥਿਰ ਖਾਲੀ prvReloadTimer( Timer_t * ਅਚਲ pxTimer,
                                TickType_t x_Expired_ਸਮਾਂ,
                                ਅਚਲ TickType_t x_ਸਮਾਂ_Now )
    {
        /* Insert the timer into the appropriate list for the next expiry time.
         * If the next expiry time has already passed, advance the expiry time,
         * call the callback function, and try again. */
        ਜਦੋਂ( prv_ਪਾਓ_Timer_ਵਿੱਚ_ਸਰਗਰਮ_List( pxTimer, ( x_Expired_ਸਮਾਂ + pxTimer->x_Timer_Period_ਵਿੱਚ_Ticks ), x_ਸਮਾਂ_Now, x_Expired_ਸਮਾਂ ) != pdFALSE )
        {
            /* Advance the expiry time. */
            x_Expired_ਸਮਾਂ += pxTimer->x_Timer_Period_ਵਿੱਚ_Ticks;

            /* Call the timer callback. */
            traceTIMER_EXPIRED( pxTimer );
            pxTimer->pxCallbackFunction( ( TimerHandle_t ) pxTimer );
        }
    }
/*-----------------------------------------------------------*/

    ਸਥਿਰ ਖਾਲੀ prvProcessExpiredTimer( ਅਚਲ TickType_t x_ਅਗਲਾ_Expire_ਸਮਾਂ,
                                        ਅਚਲ TickType_t x_ਸਮਾਂ_Now )
    {
        /* MISRA Ref 11.5.3 [Void pointer assignment] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
        /* coverity[misra_c_2012_rule_11_5_violation] */
        Timer_t * ਅਚਲ pxTimer = ( Timer_t * ) list_GET_OWNER_ਦਾ_ਸਿਰ_ਇੰਦਰਾਜ( pxCurrentTimerList );

        /* Remove the timer from the list of active timers.  A check has already
         * been performed to ensure the list is not empty. */

        ( ਖਾਲੀ ) uxListRemove( &( pxTimer->x_Timer_List_ਵਸਤੂ ) );

        /* If the timer is an auto-reload timer then calculate the next
         * expiry time and re-insert the timer in the list of active timers. */
        ਜੇ( ( pxTimer->uc_ਸਥਿਤੀ & tmr_ਸਥਿਤੀ_IS_AUTORELOAD ) != 0U )
        {
            prvReloadTimer( pxTimer, x_ਅਗਲਾ_Expire_ਸਮਾਂ, x_ਸਮਾਂ_Now );
        }
        ਨਹੀਂਤਾਂ
        {
            pxTimer->uc_ਸਥਿਤੀ &= ( ( uint8_t ) ~tmr_ਸਥਿਤੀ_IS_ਸਰਗਰਮ );
        }

        /* Call the timer callback. */
        traceTIMER_EXPIRED( pxTimer );
        pxTimer->pxCallbackFunction( ( TimerHandle_t ) pxTimer );
    }
/*-----------------------------------------------------------*/

    ਸਥਿਰ ਪੋਰਟ_TASK_FUNCTION( prvTimerTask, pvParameters )
    {
        TickType_t x_ਅਗਲਾ_Expire_ਸਮਾਂ;
        ਅਧਾਰ_Type_t x_List_Was_ਖਾਲੀ_ਥਾਵਾਂ;

        /* Just to avoid compiler warnings. */
        ( ਖਾਲੀ ) pvParameters;

        #ਜੇ ( configUSE_DAEMON_TASK_STARTUP_HOOK == 1 )
        {
            /* Allow the application writer to execute some code in the context of
             * this task at the point the task starts executing.  This is useful if the
             * application includes initialisation code that would benefit from
             * executing after the scheduler has been started. */
            vApplicationDaemonTaskStartupHook();
        }
        #ਜੇ_ਅੰਤ /* configUSE_DAEMON_TASK_STARTUP_HOOK */

        ਲਈ( ; configCONTROL_INFINITE_LOOP(); )
        {
            /* Query the timers list to see if it contains any timers, and if so,
             * obtain the time at which the next timer will expire. */
            x_ਅਗਲਾ_Expire_ਸਮਾਂ = prv_Get_ਅਗਲਾ_Expire_ਸਮਾਂ( &x_List_Was_ਖਾਲੀ_ਥਾਵਾਂ );

            /* If a timer has expired, process it.  Otherwise, block this task
             * until either a timer does expire, or a command is received. */
            prv_Process_Timer_Or_ਖੰਡ_Task( x_ਅਗਲਾ_Expire_ਸਮਾਂ, x_List_Was_ਖਾਲੀ_ਥਾਵਾਂ );

            /* Empty the command queue. */
            prvProcessReceivedCommands();
        }
    }
/*-----------------------------------------------------------*/

    ਸਥਿਰ ਖਾਲੀ prv_Process_Timer_Or_ਖੰਡ_Task( ਅਚਲ TickType_t x_ਅਗਲਾ_Expire_ਸਮਾਂ,
                                            ਅਧਾਰ_Type_t x_List_Was_ਖਾਲੀ_ਥਾਵਾਂ )
    {
        TickType_t x_ਸਮਾਂ_Now;
        ਅਧਾਰ_Type_t xTimerListsWereSwitched;

        vTaskSuspendAll();
        {
            /* Obtain the time now to make an assessment as to whether the timer
             * has expired or not.  If obtaining the time causes the lists to switch
             * then don't process this timer as any timers that remained in the list
             * when the lists were switched will have been processed within the
             * prvSampleTimeNow() function. */
            x_ਸਮਾਂ_Now = prv_Sample_ਸਮਾਂ_Now( &xTimerListsWereSwitched );

            ਜੇ( xTimerListsWereSwitched == pdFALSE )
            {
                /* The tick count has not overflowed, has the timer expired? */
                ਜੇ( ( x_List_Was_ਖਾਲੀ_ਥਾਵਾਂ == pdFALSE ) && ( x_ਅਗਲਾ_Expire_ਸਮਾਂ <= x_ਸਮਾਂ_Now ) )
                {
                    ( ਖਾਲੀ ) xTaskResumeAll();
                    prvProcessExpiredTimer( x_ਅਗਲਾ_Expire_ਸਮਾਂ, x_ਸਮਾਂ_Now );
                }
                ਨਹੀਂਤਾਂ
                {
                    /* The tick count has not overflowed, and the next expire
                     * time has not been reached yet.  This task should therefore
                     * block to wait for the next expire time or a command to be
                     * received - whichever comes first.  The following line cannot
                     * be reached unless xNextExpireTime > xTimeNow, except in the
                     * case when the current timer list is empty. */
                    ਜੇ( x_List_Was_ਖਾਲੀ_ਥਾਵਾਂ != pdFALSE )
                    {
                        /* The current timer list is empty - is the overflow list
                         * also empty? */
                        x_List_Was_ਖਾਲੀ_ਥਾਵਾਂ = list_LIST_IS_ਖਾਲੀ_ਥਾਵਾਂ( px_ਉਛਾਲ_Timer_List );
                    }

                    v_ਕਤਾਰ_Wait_For_ਸੁਨੇਹਾ_Restricted( x_Timer_ਕਤਾਰ, ( x_ਅਗਲਾ_Expire_ਸਮਾਂ - x_ਸਮਾਂ_Now ), x_List_Was_ਖਾਲੀ_ਥਾਵਾਂ );

                    ਜੇ( xTaskResumeAll() == pdFALSE )
                    {
                        /* Yield to wait for either a command to arrive, or the
                         * block time to expire.  If a command arrived between the
                         * critical section being exited and this yield then the yield
                         * will not cause the task to block. */
                        taskYIELD_WITHIN_API();
                    }
                    ਨਹੀਂਤਾਂ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
            }
            ਨਹੀਂਤਾਂ
            {
                ( ਖਾਲੀ ) xTaskResumeAll();
            }
        }
    }
/*-----------------------------------------------------------*/

    ਸਥਿਰ TickType_t prv_Get_ਅਗਲਾ_Expire_ਸਮਾਂ( ਅਧਾਰ_Type_t * ਅਚਲ px_List_Was_ਖਾਲੀ_ਥਾਵਾਂ )
    {
        TickType_t x_ਅਗਲਾ_Expire_ਸਮਾਂ;

        /* Timers are listed in expiry time order, with the head of the list
         * referencing the task that will expire first.  Obtain the time at which
         * the timer with the nearest expiry time will expire.  If there are no
         * active timers then just set the next expire time to 0.  That will cause
         * this task to unblock when the tick count overflows, at which point the
         * timer lists will be switched and the next expiry time can be
         * re-assessed.  */
        *px_List_Was_ਖਾਲੀ_ਥਾਵਾਂ = list_LIST_IS_ਖਾਲੀ_ਥਾਵਾਂ( pxCurrentTimerList );

        ਜੇ( *px_List_Was_ਖਾਲੀ_ਥਾਵਾਂ == pdFALSE )
        {
            x_ਅਗਲਾ_Expire_ਸਮਾਂ = list_GET_ਵਸਤੂ_ਮੁੱਲ_ਦਾ_ਸਿਰ_ਇੰਦਰਾਜ( pxCurrentTimerList );
        }
        ਨਹੀਂਤਾਂ
        {
            /* Ensure the task unblocks when the tick count rolls over. */
            x_ਅਗਲਾ_Expire_ਸਮਾਂ = ( TickType_t ) 0U;
        }

        ਵਾਪਸ x_ਅਗਲਾ_Expire_ਸਮਾਂ;
    }
/*-----------------------------------------------------------*/

    ਸਥਿਰ TickType_t prv_Sample_ਸਮਾਂ_Now( ਅਧਾਰ_Type_t * ਅਚਲ pxTimerListsWereSwitched )
    {
        TickType_t x_ਸਮਾਂ_Now;
        PRIVILEGED_ਅੰਕੜੇ ਸਥਿਰ TickType_t x_ਆਖਰੀ_ਸਮਾਂ = ( TickType_t ) 0U;

        x_ਸਮਾਂ_Now = x_Task_Get_Tick_ਗਿਣਤੀ();

        ਜੇ( x_ਸਮਾਂ_Now < x_ਆਖਰੀ_ਸਮਾਂ )
        {
            prvSwitchTimerLists();
            *pxTimerListsWereSwitched = pdTRUE;
        }
        ਨਹੀਂਤਾਂ
        {
            *pxTimerListsWereSwitched = pdFALSE;
        }

        x_ਆਖਰੀ_ਸਮਾਂ = x_ਸਮਾਂ_Now;

        ਵਾਪਸ x_ਸਮਾਂ_Now;
    }
/*-----------------------------------------------------------*/

    ਸਥਿਰ ਅਧਾਰ_Type_t prv_ਪਾਓ_Timer_ਵਿੱਚ_ਸਰਗਰਮ_List( Timer_t * ਅਚਲ pxTimer,
                                                  ਅਚਲ TickType_t x_ਅਗਲਾ_Expiry_ਸਮਾਂ,
                                                  ਅਚਲ TickType_t x_ਸਮਾਂ_Now,
                                                  ਅਚਲ TickType_t x_Command_ਸਮਾਂ )
    {
        ਅਧਾਰ_Type_t xProcessTimerNow = pdFALSE;

        list_ਸੈੱਟ_LIST_ਵਸਤੂ_ਮੁੱਲ( &( pxTimer->x_Timer_List_ਵਸਤੂ ), x_ਅਗਲਾ_Expiry_ਸਮਾਂ );
        list_ਸੈੱਟ_LIST_ਵਸਤੂ_OWNER( &( pxTimer->x_Timer_List_ਵਸਤੂ ), pxTimer );

        ਜੇ( x_ਅਗਲਾ_Expiry_ਸਮਾਂ <= x_ਸਮਾਂ_Now )
        {
            /* Has the expiry time elapsed between the command to start/reset a
             * timer was issued, and the time the command was processed? */
            ਜੇ( ( ( TickType_t ) ( x_ਸਮਾਂ_Now - x_Command_ਸਮਾਂ ) ) >= pxTimer->x_Timer_Period_ਵਿੱਚ_Ticks )
            {
                /* The time between a command being issued and the command being
                 * processed actually exceeds the timers period.  */
                xProcessTimerNow = pdTRUE;
            }
            ਨਹੀਂਤਾਂ
            {
                v_List_ਪਾਓ( px_ਉਛਾਲ_Timer_List, &( pxTimer->x_Timer_List_ਵਸਤੂ ) );
            }
        }
        ਨਹੀਂਤਾਂ
        {
            ਜੇ( ( x_ਸਮਾਂ_Now < x_Command_ਸਮਾਂ ) && ( x_ਅਗਲਾ_Expiry_ਸਮਾਂ >= x_Command_ਸਮਾਂ ) )
            {
                /* If, since the command was issued, the tick count has overflowed
                 * but the expiry time has not, then the timer must have already passed
                 * its expiry time and should be processed immediately. */
                xProcessTimerNow = pdTRUE;
            }
            ਨਹੀਂਤਾਂ
            {
                v_List_ਪਾਓ( pxCurrentTimerList, &( pxTimer->x_Timer_List_ਵਸਤੂ ) );
            }
        }

        ਵਾਪਸ xProcessTimerNow;
    }
/*-----------------------------------------------------------*/

    ਸਥਿਰ ਖਾਲੀ prvProcessReceivedCommands( ਖਾਲੀ )
    {
        Daemon_Task_ਸੁਨੇਹਾ_t x_ਸੁਨੇਹਾ = { 0 };
        Timer_t * pxTimer;
        ਅਧਾਰ_Type_t xTimerListsWereSwitched;
        TickType_t x_ਸਮਾਂ_Now;

        ਜਦੋਂ( x_ਕਤਾਰ_Receive( x_Timer_ਕਤਾਰ, &x_ਸੁਨੇਹਾ, tmrNO_DELAY ) != pdFAIL )
        {
            #ਜੇ ( INCLUDE_xTimerPendFunctionCall == 1 )
            {
                /* Negative commands are pended function calls rather than timer
                 * commands. */
                ਜੇ( x_ਸੁਨੇਹਾ.x_ਸੁਨੇਹਾ_ID < ( ਅਧਾਰ_Type_t ) 0 )
                {
                    ਅਚਲ CallbackParameters_t * ਅਚਲ pxCallback = &( x_ਸੁਨੇਹਾ.u.xCallbackParameters );

                    /* The timer uses the xCallbackParameters member to request a
                     * callback be executed.  Check the callback is not NULL. */
                    configASSERT( pxCallback );

                    /* Call the function. */
                    pxCallback->pxCallbackFunction( pxCallback->pvParameter1, pxCallback->ulParameter2 );
                }
                ਨਹੀਂਤਾਂ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            #ਜੇ_ਅੰਤ /* INCLUDE_xTimerPendFunctionCall */

            /* Commands that are positive are timer commands rather than pended
             * function calls. */
            ਜੇ( x_ਸੁਨੇਹਾ.x_ਸੁਨੇਹਾ_ID >= ( ਅਧਾਰ_Type_t ) 0 )
            {
                /* The messages uses the xTimerParameters member to work on a
                 * software timer. */
                pxTimer = x_ਸੁਨੇਹਾ.u.xTimerParameters.pxTimer;

                ਜੇ( listIS_CONTAINED_WITHIN( ਨਿਰਾ, &( pxTimer->x_Timer_List_ਵਸਤੂ ) ) == pdFALSE )
                {
                    /* The timer is in a list, remove it. */
                    ( ਖਾਲੀ ) uxListRemove( &( pxTimer->x_Timer_List_ਵਸਤੂ ) );
                }
                ਨਹੀਂਤਾਂ
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                traceTIMER_COMMAND_RECEIVED( pxTimer, x_ਸੁਨੇਹਾ.x_ਸੁਨੇਹਾ_ID, x_ਸੁਨੇਹਾ.u.xTimerParameters.x_ਸੁਨੇਹਾ_ਮੁੱਲ );

                /* In this case the xTimerListsWereSwitched parameter is not used, but
                 *  it must be present in the function call.  prvSampleTimeNow() must be
                 *  called after the message is received from xTimerQueue so there is no
                 *  possibility of a higher priority task adding a message to the message
                 *  queue with a time that is ahead of the timer daemon task (because it
                 *  pre-empted the timer daemon task after the xTimeNow value was set). */
                x_ਸਮਾਂ_Now = prv_Sample_ਸਮਾਂ_Now( &xTimerListsWereSwitched );

                ਚੋਣ( x_ਸੁਨੇਹਾ.x_ਸੁਨੇਹਾ_ID )
                {
                    ਹਾਲਤ tmr_COMMAND_ਸ਼ੁਰੂ:
                    ਹਾਲਤ tmr_COMMAND_ਸ਼ੁਰੂ_FROM_ISR:
                    ਹਾਲਤ tmrCOMMAND_RESET:
                    ਹਾਲਤ tmrCOMMAND_RESET_FROM_ISR:
                        /* Start or restart a timer. */
                        pxTimer->uc_ਸਥਿਤੀ |= ( uint8_t ) tmr_ਸਥਿਤੀ_IS_ਸਰਗਰਮ;

                        ਜੇ( prv_ਪਾਓ_Timer_ਵਿੱਚ_ਸਰਗਰਮ_List( pxTimer, x_ਸੁਨੇਹਾ.u.xTimerParameters.x_ਸੁਨੇਹਾ_ਮੁੱਲ + pxTimer->x_Timer_Period_ਵਿੱਚ_Ticks, x_ਸਮਾਂ_Now, x_ਸੁਨੇਹਾ.u.xTimerParameters.x_ਸੁਨੇਹਾ_ਮੁੱਲ ) != pdFALSE )
                        {
                            /* The timer expired before it was added to the active
                             * timer list.  Process it now. */
                            ਜੇ( ( pxTimer->uc_ਸਥਿਤੀ & tmr_ਸਥਿਤੀ_IS_AUTORELOAD ) != 0U )
                            {
                                prvReloadTimer( pxTimer, x_ਸੁਨੇਹਾ.u.xTimerParameters.x_ਸੁਨੇਹਾ_ਮੁੱਲ + pxTimer->x_Timer_Period_ਵਿੱਚ_Ticks, x_ਸਮਾਂ_Now );
                            }
                            ਨਹੀਂਤਾਂ
                            {
                                pxTimer->uc_ਸਥਿਤੀ &= ( ( uint8_t ) ~tmr_ਸਥਿਤੀ_IS_ਸਰਗਰਮ );
                            }

                            /* Call the timer callback. */
                            traceTIMER_EXPIRED( pxTimer );
                            pxTimer->pxCallbackFunction( ( TimerHandle_t ) pxTimer );
                        }
                        ਨਹੀਂਤਾਂ
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }

                        ਰੋਕੋ;

                    ਹਾਲਤ tmrCOMMAND_STOP:
                    ਹਾਲਤ tmrCOMMAND_STOP_FROM_ISR:
                        /* The timer has already been removed from the active list. */
                        pxTimer->uc_ਸਥਿਤੀ &= ( ( uint8_t ) ~tmr_ਸਥਿਤੀ_IS_ਸਰਗਰਮ );
                        ਰੋਕੋ;

                    ਹਾਲਤ tmrCOMMAND_CHANGE_PERIOD:
                    ਹਾਲਤ tmrCOMMAND_CHANGE_PERIOD_FROM_ISR:
                        pxTimer->uc_ਸਥਿਤੀ |= ( uint8_t ) tmr_ਸਥਿਤੀ_IS_ਸਰਗਰਮ;
                        pxTimer->x_Timer_Period_ਵਿੱਚ_Ticks = x_ਸੁਨੇਹਾ.u.xTimerParameters.x_ਸੁਨੇਹਾ_ਮੁੱਲ;
                        configASSERT( ( pxTimer->x_Timer_Period_ਵਿੱਚ_Ticks > 0 ) );

                        /* The new period does not really have a reference, and can
                         * be longer or shorter than the old one.  The command time is
                         * therefore set to the current time, and as the period cannot
                         * be zero the next expiry time can only be in the future,
                         * meaning (unlike for the xTimerStart() case above) there is
                         * no fail case that needs to be handled here. */
                        ( ਖਾਲੀ ) prv_ਪਾਓ_Timer_ਵਿੱਚ_ਸਰਗਰਮ_List( pxTimer, ( x_ਸਮਾਂ_Now + pxTimer->x_Timer_Period_ਵਿੱਚ_Ticks ), x_ਸਮਾਂ_Now, x_ਸਮਾਂ_Now );
                        ਰੋਕੋ;

                    ਹਾਲਤ tmrCOMMAND_DELETE:
                        #ਜੇ ( config_SUPPORT_DYNAMIC_ਵੰਡ == 1 )
                        {
                            /* The timer has already been removed from the active list,
                             * just free up the memory if the memory was dynamically
                             * allocated. */
                            ਜੇ( ( pxTimer->uc_ਸਥਿਤੀ & tmr_ਸਥਿਤੀ_IS_STATICALLY_ALLOCATED ) == ( uint8_t ) 0 )
                            {
                                v_ਪੋਰਟ_Free( pxTimer );
                            }
                            ਨਹੀਂਤਾਂ
                            {
                                pxTimer->uc_ਸਥਿਤੀ &= ( ( uint8_t ) ~tmr_ਸਥਿਤੀ_IS_ਸਰਗਰਮ );
                            }
                        }
                        #ਨਹੀਂਤਾਂ /* if ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) */
                        {
                            /* If dynamic allocation is not enabled, the memory
                             * could not have been dynamically allocated. So there is
                             * no need to free the memory - just mark the timer as
                             * "not active". */
                            pxTimer->uc_ਸਥਿਤੀ &= ( ( uint8_t ) ~tmr_ਸਥਿਤੀ_IS_ਸਰਗਰਮ );
                        }
                        #ਜੇ_ਅੰਤ /* configSUPPORT_DYNAMIC_ALLOCATION */
                        ਰੋਕੋ;

                    ਮੂਲ:
                        /* Don't expect to get here. */
                        ਰੋਕੋ;
                }
            }
        }
    }
/*-----------------------------------------------------------*/

    ਸਥਿਰ ਖਾਲੀ prvSwitchTimerLists( ਖਾਲੀ )
    {
        TickType_t x_ਅਗਲਾ_Expire_ਸਮਾਂ;
        List_t * px_ਆਰਜ਼ੀ;

        /* The tick count has overflowed.  The timer lists must be switched.
         * If there are any timers still referenced from the current timer list
         * then they must have expired and should be processed before the lists
         * are switched. */
        ਜਦੋਂ( list_LIST_IS_ਖਾਲੀ_ਥਾਵਾਂ( pxCurrentTimerList ) == pdFALSE )
        {
            x_ਅਗਲਾ_Expire_ਸਮਾਂ = list_GET_ਵਸਤੂ_ਮੁੱਲ_ਦਾ_ਸਿਰ_ਇੰਦਰਾਜ( pxCurrentTimerList );

            /* Process the expired timer.  For auto-reload timers, be careful to
             * process only expirations that occur on the current list.  Further
             * expirations must wait until after the lists are switched. */
            prvProcessExpiredTimer( x_ਅਗਲਾ_Expire_ਸਮਾਂ, tmr_MAX_ਸਮਾਂ_BEFORE_ਉਛਾਲ );
        }

        px_ਆਰਜ਼ੀ = pxCurrentTimerList;
        pxCurrentTimerList = px_ਉਛਾਲ_Timer_List;
        px_ਉਛਾਲ_Timer_List = px_ਆਰਜ਼ੀ;
    }
/*-----------------------------------------------------------*/

    ਸਥਿਰ ਖਾਲੀ prv_Check_For_Valid_List_And_ਕਤਾਰ( ਖਾਲੀ )
    {
        /* Check that the list from which active timers are referenced, and the
         * queue used to communicate with the timer service, have been
         * initialised. */
        taskENTER_CRITICAL();
        {
            ਜੇ( x_Timer_ਕਤਾਰ == ਨਿਰਾ )
            {
                vListInitialise( &x_ਸਰਗਰਮ_Timer_List1 );
                vListInitialise( &x_ਸਰਗਰਮ_Timer_List2 );
                pxCurrentTimerList = &x_ਸਰਗਰਮ_Timer_List1;
                px_ਉਛਾਲ_Timer_List = &x_ਸਰਗਰਮ_Timer_List2;

                #ਜੇ ( config_SUPPORT_STATIC_ਵੰਡ == 1 )
                {
                    /* The timer queue is allocated statically in case
                     * configSUPPORT_DYNAMIC_ALLOCATION is 0. */
                    PRIVILEGED_ਅੰਕੜੇ ਸਥਿਰ Static_ਕਤਾਰ_t x_Static_Timer_ਕਤਾਰ;
                    PRIVILEGED_ਅੰਕੜੇ ਸਥਿਰ uint8_t uc_Static_Timer_ਕਤਾਰ_Storage[ ( size_t ) config_TIMER_ਕਤਾਰ_ਲੰਮਾਈ * ਆਕਾਰ( Daemon_Task_ਸੁਨੇਹਾ_t ) ];

                    x_Timer_ਕਤਾਰ = x_ਕਤਾਰ_Create_Static( ( UBaseType_t ) config_TIMER_ਕਤਾਰ_ਲੰਮਾਈ, ( UBaseType_t ) ਆਕਾਰ( Daemon_Task_ਸੁਨੇਹਾ_t ), &( uc_Static_Timer_ਕਤਾਰ_Storage[ 0 ] ), &x_Static_Timer_ਕਤਾਰ );
                }
                #ਨਹੀਂਤਾਂ
                {
                    x_Timer_ਕਤਾਰ = x_ਕਤਾਰ_Create( ( UBaseType_t ) config_TIMER_ਕਤਾਰ_ਲੰਮਾਈ, ( UBaseType_t ) ਆਕਾਰ( Daemon_Task_ਸੁਨੇਹਾ_t ) );
                }
                #ਜੇ_ਅੰਤ /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */

                #ਜੇ ( config_ਕਤਾਰ_REGISTRY_ਮਾਪ > 0 )
                {
                    ਜੇ( x_Timer_ਕਤਾਰ != ਨਿਰਾ )
                    {
                        v_ਕਤਾਰ_ਜੋੜੋ_ਮੰਜ਼ਿਲ_Registry( x_Timer_ਕਤਾਰ, "TmrQ" );
                    }
                    ਨਹੀਂਤਾਂ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #ਜੇ_ਅੰਤ /* configQUEUE_REGISTRY_SIZE */
            }
            ਨਹੀਂਤਾਂ
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        taskEXIT_CRITICAL();
    }
/*-----------------------------------------------------------*/

    ਅਧਾਰ_Type_t x_Timer_Is_Timer_ਸਰਗਰਮ( TimerHandle_t xTimer )
    {
        ਅਧਾਰ_Type_t xReturn;
        Timer_t * pxTimer = xTimer;

        trace_ENTER_x_Timer_Is_Timer_ਸਰਗਰਮ( xTimer );

        configASSERT( xTimer );

        /* Is the timer in the list of active timers? */
        taskENTER_CRITICAL();
        {
            ਜੇ( ( pxTimer->uc_ਸਥਿਤੀ & tmr_ਸਥਿਤੀ_IS_ਸਰਗਰਮ ) == 0U )
            {
                xReturn = pdFALSE;
            }
            ਨਹੀਂਤਾਂ
            {
                xReturn = pdTRUE;
            }
        }
        taskEXIT_CRITICAL();

        trace_RETURN_x_Timer_Is_Timer_ਸਰਗਰਮ( xReturn );

        ਵਾਪਸ xReturn;
    }
/*-----------------------------------------------------------*/

    ਖਾਲੀ * pvTimerGetTimerID( ਅਚਲ TimerHandle_t xTimer )
    {
        Timer_t * ਅਚਲ pxTimer = xTimer;
        ਖਾਲੀ * pvReturn;

        traceENTER_pvTimerGetTimerID( xTimer );

        configASSERT( xTimer );

        taskENTER_CRITICAL();
        {
            pvReturn = pxTimer->pvTimerID;
        }
        taskEXIT_CRITICAL();

        traceRETURN_pvTimerGetTimerID( pvReturn );

        ਵਾਪਸ pvReturn;
    }
/*-----------------------------------------------------------*/

    ਖਾਲੀ v_Timer_ਸੈੱਟ_Timer_ID( TimerHandle_t xTimer,
                           ਖਾਲੀ * pvNewID )
    {
        Timer_t * ਅਚਲ pxTimer = xTimer;

        trace_ENTER_v_Timer_ਸੈੱਟ_Timer_ID( xTimer, pvNewID );

        configASSERT( xTimer );

        taskENTER_CRITICAL();
        {
            pxTimer->pvTimerID = pvNewID;
        }
        taskEXIT_CRITICAL();

        trace_RETURN_v_Timer_ਸੈੱਟ_Timer_ID();
    }
/*-----------------------------------------------------------*/

    #ਜੇ ( INCLUDE_xTimerPendFunctionCall == 1 )

        ਅਧਾਰ_Type_t xTimerPendFunctionCallFromISR( PendedFunction_t x_Function_ਮੰਜ਼ਿਲ_Pend,
                                                  ਖਾਲੀ * pvParameter1,
                                                  uint32_t ulParameter2,
                                                  ਅਧਾਰ_Type_t * px_Higher_ਤਰਜੀਹ_Task_Woken )
        {
            Daemon_Task_ਸੁਨੇਹਾ_t x_ਸੁਨੇਹਾ;
            ਅਧਾਰ_Type_t xReturn;

            traceENTER_xTimerPendFunctionCallFromISR( x_Function_ਮੰਜ਼ਿਲ_Pend, pvParameter1, ulParameter2, px_Higher_ਤਰਜੀਹ_Task_Woken );

            /* Complete the message with the function parameters and post it to the
             * daemon task. */
            x_ਸੁਨੇਹਾ.x_ਸੁਨੇਹਾ_ID = tmrCOMMAND_EXECUTE_CALLBACK_FROM_ISR;
            x_ਸੁਨੇਹਾ.u.xCallbackParameters.pxCallbackFunction = x_Function_ਮੰਜ਼ਿਲ_Pend;
            x_ਸੁਨੇਹਾ.u.xCallbackParameters.pvParameter1 = pvParameter1;
            x_ਸੁਨੇਹਾ.u.xCallbackParameters.ulParameter2 = ulParameter2;

            xReturn = x_ਕਤਾਰ_Send_From_ISR( x_Timer_ਕਤਾਰ, &x_ਸੁਨੇਹਾ, px_Higher_ਤਰਜੀਹ_Task_Woken );

            tracePEND_FUNC_CALL_FROM_ISR( x_Function_ਮੰਜ਼ਿਲ_Pend, pvParameter1, ulParameter2, xReturn );
            traceRETURN_xTimerPendFunctionCallFromISR( xReturn );

            ਵਾਪਸ xReturn;
        }

    #ਜੇ_ਅੰਤ /* INCLUDE_xTimerPendFunctionCall */
/*-----------------------------------------------------------*/

    #ਜੇ ( INCLUDE_xTimerPendFunctionCall == 1 )

        ਅਧਾਰ_Type_t xTimerPendFunctionCall( PendedFunction_t x_Function_ਮੰਜ਼ਿਲ_Pend,
                                           ਖਾਲੀ * pvParameter1,
                                           uint32_t ulParameter2,
                                           TickType_t x_Ticks_ਮੰਜ਼ਿਲ_Wait )
        {
            Daemon_Task_ਸੁਨੇਹਾ_t x_ਸੁਨੇਹਾ;
            ਅਧਾਰ_Type_t xReturn;

            traceENTER_xTimerPendFunctionCall( x_Function_ਮੰਜ਼ਿਲ_Pend, pvParameter1, ulParameter2, x_Ticks_ਮੰਜ਼ਿਲ_Wait );

            /* This function can only be called after a timer has been created or
             * after the scheduler has been started because, until then, the timer
             * queue does not exist. */
            configASSERT( x_Timer_ਕਤਾਰ );

            /* Complete the message with the function parameters and post it to the
             * daemon task. */
            x_ਸੁਨੇਹਾ.x_ਸੁਨੇਹਾ_ID = tmrCOMMAND_EXECUTE_CALLBACK;
            x_ਸੁਨੇਹਾ.u.xCallbackParameters.pxCallbackFunction = x_Function_ਮੰਜ਼ਿਲ_Pend;
            x_ਸੁਨੇਹਾ.u.xCallbackParameters.pvParameter1 = pvParameter1;
            x_ਸੁਨੇਹਾ.u.xCallbackParameters.ulParameter2 = ulParameter2;

            xReturn = x_ਕਤਾਰ_Send_ਮੰਜ਼ਿਲ_ਪਿੱਠ( x_Timer_ਕਤਾਰ, &x_ਸੁਨੇਹਾ, x_Ticks_ਮੰਜ਼ਿਲ_Wait );

            tracePEND_FUNC_CALL( x_Function_ਮੰਜ਼ਿਲ_Pend, pvParameter1, ulParameter2, xReturn );
            traceRETURN_xTimerPendFunctionCall( xReturn );

            ਵਾਪਸ xReturn;
        }

    #ਜੇ_ਅੰਤ /* INCLUDE_xTimerPendFunctionCall */
/*-----------------------------------------------------------*/

    #ਜੇ ( configUSE_TRACE_FACILITY == 1 )

        UBaseType_t ux_Timer_Get_Timer_ਸੰਖਿਆ( TimerHandle_t xTimer )
        {
            trace_ENTER_ux_Timer_Get_Timer_ਸੰਖਿਆ( xTimer );

            trace_RETURN_ux_Timer_Get_Timer_ਸੰਖਿਆ( ( ( Timer_t * ) xTimer )->ux_Timer_ਸੰਖਿਆ );

            ਵਾਪਸ ( ( Timer_t * ) xTimer )->ux_Timer_ਸੰਖਿਆ;
        }

    #ਜੇ_ਅੰਤ /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

    #ਜੇ ( configUSE_TRACE_FACILITY == 1 )

        ਖਾਲੀ v_Timer_ਸੈੱਟ_Timer_ਸੰਖਿਆ( TimerHandle_t xTimer,
                                   UBaseType_t ux_Timer_ਸੰਖਿਆ )
        {
            trace_ENTER_v_Timer_ਸੈੱਟ_Timer_ਸੰਖਿਆ( xTimer, ux_Timer_ਸੰਖਿਆ );

            ( ( Timer_t * ) xTimer )->ux_Timer_ਸੰਖਿਆ = ux_Timer_ਸੰਖਿਆ;

            trace_RETURN_v_Timer_ਸੈੱਟ_Timer_ਸੰਖਿਆ();
        }

    #ਜੇ_ਅੰਤ /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

/*
 * Reset the state in this file. This state is normally initialized at start up.
 * This function must be called by the application before restarting the
 * scheduler.
 */
    ਖਾਲੀ vTimerResetState( ਖਾਲੀ )
    {
        x_Timer_ਕਤਾਰ = ਨਿਰਾ;
        xTimerTaskHandle = ਨਿਰਾ;
    }
/*-----------------------------------------------------------*/

/* This entire source file will be skipped if the application is not configured
 * to include software timer functionality.  If you want to include software timer
 * functionality then ensure configUSE_TIMERS is set to 1 in FreeRTOSConfig.h. */
#ਜੇ_ਅੰਤ /* configUSE_TIMERS == 1 */
