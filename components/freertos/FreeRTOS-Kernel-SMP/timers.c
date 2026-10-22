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
#ضمن <stdlib.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#تعریف_کړه MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#ضمن "FreeRTOS.h"
#ضمن "task.h"
#ضمن "queue.h"
#ضمن "timers.h"

#إذا ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 0 )
    #تېروتنه configUSE_TIMERS must be سېټ منزل 1 منزل make the xTimerPendFunctionCall() function شته.
#د_که_پای

/* The MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#تعریف_لغوه MPU_WRAPPERS_INCLUDED_FROM_API_FILE


/* This entire source file will be skipped if the application is not configured
 * to include software timer functionality.  This #if is closed at the very bottom
 * of this file.  If you want to include software timer functionality then ensure
 * configUSE_TIMERS is set to 1 in FreeRTOSConfig.h. */
#إذا ( configUSE_TIMERS == 1 )

/* Misc definitions. */
    #تعریف_کړه tmrNO_DELAY                    ( ( TickType_t ) 0U )
    #تعریف_کړه tmr_MAX_وخت_BEFORE_ډکېدنه    ( ( TickType_t ) -1 )

/* The name assigned to the timer service task. This can be overridden by
 * defining configTIMER_SERVICE_TASK_NAME in FreeRTOSConfig.h. */
    #که_نه_تعریف_شوی configTIMER_SERVICE_TASK_NAME
        #تعریف_کړه configTIMER_SERVICE_TASK_NAME    "Tmr Svc"
    #د_که_پای

    #إذا ( ( config_شمېره_د_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )

/* The core affinity assigned to the timer service task on SMP systems.
 * This can be overridden by defining configTIMER_SERVICE_TASK_CORE_AFFINITY in FreeRTOSConfig.h. */
        #که_نه_تعریف_شوی configTIMER_SERVICE_TASK_CORE_AFFINITY
            #تعریف_کړه configTIMER_SERVICE_TASK_CORE_AFFINITY    tskNO_AFFINITY
        #د_که_پای
    #د_که_پای /* #if ( ( configNUMBER_OF_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */

/* Bit definitions used in the ucStatus member of a timer structure. */
    #تعریف_کړه tmr_حالت_IS_فعال                  ( 0x01U )
    #تعریف_کړه tmr_حالت_IS_STATICALLY_ALLOCATED    ( 0x02U )
    #تعریف_کړه tmr_حالت_IS_AUTORELOAD              ( 0x04U )

/* The definition of the timers themselves. */
    تعريفنوع بنية tmrTimerControl                                               /* The old naming convention is used to prevent breaking kernel aware debuggers. */
    {
        ثابت حرف * pcTimerName;                                                /**< Text name.  This is not used by the kernel, it is included simply to make debugging easier. */
        List_توکی_t x_Timer_List_توکی;                                               /**< Standard linked list item as used by all kernel features for event management. */
        TickType_t x_Timer_Period_دننه_Ticks;                                          /**< How quickly and often the timer expires. */
        فراغ * pvTimerID;                                                        /**< An ID to identify the timer.  This allows the timer to be identified when the same callback is used for multiple timers. */
        پورټ_TIMER_CALLBACK_ATTRIBUTE TimerCallbackFunction_t pxCallbackFunction; /**< The function that will be called when the timer expires. */
        #إذا ( configUSE_TRACE_FACILITY == 1 )
            UBaseType_t ux_Timer_شمېره;                                           /**< An ID assigned by trace tools such as FreeRTOS+Trace */
        #د_که_پای
        uint8_t uc_حالت;                                                        /**< Holds bits to say if the timer was statically allocated or not, and if it is active or not. */
    } xTIMER;

/* The old xTIMER name is maintained above then typedefed to the new Timer_t
 * name below to enable the use of older kernel aware debuggers. */
    تعريفنوع xTIMER Timer_t;

/* The definition of messages that can be sent and received on the timer queue.
 * Two types of message can be queued - messages that manipulate a software timer,
 * and messages that request the execution of a non-timer related callback.  The
 * two message types are defined in two separate structures, xTimerParametersType
 * and xCallbackParametersType respectively. */
    تعريفنوع بنية tmrTimerParameters
    {
        TickType_t x_پیغام_ارزښت; /**< An optional value used by a subset of commands, for example, when changing the period of a timer. */
        Timer_t * pxTimer;        /**< The timer to which the command will be applied. */
    } TimerParameter_t;


    تعريفنوع بنية tmrCallbackParameters
    {
        پورټ_TIMER_CALLBACK_ATTRIBUTE
        PendedFunction_t pxCallbackFunction; /* << The callback function to execute. */
        فراغ * pvParameter1;                 /* << The value that will be used as the callback functions first parameter. */
        uint32_t ulParameter2;               /* << The value that will be used as the callback functions second parameter. */
    } CallbackParameters_t;

/* The structure that contains the two message types, along with an identifier
 * that is used to determine which message type is valid. */
    تعريفنوع بنية tmr_Timer_کتار_پیغام
    {
        بنسټ_Type_t x_پیغام_ID; /**< The command being sent to the timer service task. */
        اتحاد
        {
            TimerParameter_t xTimerParameters;

            /* Don't include xCallbackParameters if it is not going to be used as
             * it makes the structure (and therefore the timer queue) larger. */
            #إذا ( INCLUDE_xTimerPendFunctionCall == 1 )
                CallbackParameters_t xCallbackParameters;
            #د_که_پای /* INCLUDE_xTimerPendFunctionCall */
        } u;
    } Daemon_Task_پیغام_t;

/* The list in which active timers are stored.  Timers are referenced in expire
 * time order, with the nearest expiry time at the front of the list.  Only the
 * timer service task is allowed to access these lists.
 * xActiveTimerList1 and xActiveTimerList2 could be at function scope but that
 * breaks some kernel aware debuggers, and debuggers that reply on removing the
 * static qualifier. */
    PRIVILEGED_معلومات ساكن List_t x_فعال_Timer_List1;
    PRIVILEGED_معلومات ساكن List_t x_فعال_Timer_List2;
    PRIVILEGED_معلومات ساكن List_t * pxCurrentTimerList;
    PRIVILEGED_معلومات ساكن List_t * px_ډکېدنه_Timer_List;

/* A queue that is used to send commands to the timer service task. */
    PRIVILEGED_معلومات ساكن کتار_Handle_t x_Timer_کتار = عدم;
    PRIVILEGED_معلومات ساكن TaskHandle_t xTimerTaskHandle = عدم;

/*-----------------------------------------------------------*/

/*
 * Initialise the infrastructure used by the timer service task if it has not
 * been initialised already.
 */
    ساكن فراغ prv_Check_For_Valid_List_And_کتار( فراغ ) PRIVILEGED_FUNCTION;

/*
 * The timer service task (daemon).  Timer functionality is controlled by this
 * task.  Other tasks communicate with the timer service task using the
 * xTimerQueue queue.
 */
    ساكن پورټ_TASK_FUNCTION_PROTO( prvTimerTask, pvParameters ) PRIVILEGED_FUNCTION;

/*
 * Called by the timer service task to interpret and process a command it
 * received on the timer queue.
 */
    ساكن فراغ prvProcessReceivedCommands( فراغ ) PRIVILEGED_FUNCTION;

/*
 * Insert the timer into either xActiveTimerList1, or xActiveTimerList2,
 * depending on if the expire time causes a timer counter overflow.
 */
    ساكن بنسټ_Type_t prv_ورننباسه_Timer_دننه_فعال_List( Timer_t * ثابت pxTimer,
                                                  ثابت TickType_t x_بل_Expiry_وخت,
                                                  ثابت TickType_t x_وخت_Now,
                                                  ثابت TickType_t x_Command_وخت ) PRIVILEGED_FUNCTION;

/*
 * Reload the specified auto-reload timer.  If the reloading is backlogged,
 * clear the backlog, calling the callback for each additional reload.  When
 * this function returns, the next expiry time is after xTimeNow.
 */
    ساكن فراغ prvReloadTimer( Timer_t * ثابت pxTimer,
                                TickType_t x_Expired_وخت,
                                ثابت TickType_t x_وخت_Now ) PRIVILEGED_FUNCTION;

/*
 * An active timer has reached its expire time.  Reload the timer if it is an
 * auto-reload timer, then call its callback.
 */
    ساكن فراغ prvProcessExpiredTimer( ثابت TickType_t x_بل_Expire_وخت,
                                        ثابت TickType_t x_وخت_Now ) PRIVILEGED_FUNCTION;

/*
 * The tick count has overflowed.  Switch the timer lists after ensuring the
 * current timer list does not still reference some timers.
 */
    ساكن فراغ prvSwitchTimerLists( فراغ ) PRIVILEGED_FUNCTION;

/*
 * Obtain the current tick count, setting *pxTimerListsWereSwitched to pdTRUE
 * if a tick count overflow occurred since prvSampleTimeNow() was last called.
 */
    ساكن TickType_t prv_Sample_وخت_Now( بنسټ_Type_t * ثابت pxTimerListsWereSwitched ) PRIVILEGED_FUNCTION;

/*
 * If the timer list contains any active timers then return the expire time of
 * the timer that will expire first and set *pxListWasEmpty to false.  If the
 * timer list does not contain any timers then return 0 and set *pxListWasEmpty
 * to pdTRUE.
 */
    ساكن TickType_t prv_Get_بل_Expire_وخت( بنسټ_Type_t * ثابت px_List_Was_تشه ) PRIVILEGED_FUNCTION;

/*
 * If a timer has expired, process it.  Otherwise, block the timer service task
 * until either a timer does expire or a command is received.
 */
    ساكن فراغ prv_Process_Timer_Or_بلاک_Task( ثابت TickType_t x_بل_Expire_وخت,
                                            بنسټ_Type_t x_List_Was_تشه ) PRIVILEGED_FUNCTION;

/*
 * Called after a Timer_t structure has been allocated either statically or
 * dynamically to fill in the structure's members.
 */
    ساكن فراغ prvInitialiseNewTimer( ثابت حرف * ثابت pcTimerName,
                                       ثابت TickType_t x_Timer_Period_دننه_Ticks,
                                       ثابت بنسټ_Type_t xAutoReload,
                                       فراغ * ثابت pvTimerID,
                                       TimerCallbackFunction_t pxCallbackFunction,
                                       Timer_t * pxNewTimer ) PRIVILEGED_FUNCTION;
/*-----------------------------------------------------------*/

    بنسټ_Type_t xTimerCreateTimerTask( فراغ )
    {
        بنسټ_Type_t xReturn = pdFAIL;

        traceENTER_xTimerCreateTimerTask();

        /* This function is called when the scheduler is started if
         * configUSE_TIMERS is set to 1.  Check that the infrastructure used by the
         * timer service task has been created/initialised.  If timers have already
         * been created then the initialisation will already have been performed. */
        prv_Check_For_Valid_List_And_کتار();

        إذا( x_Timer_کتار != عدم )
        {
            #إذا ( ( config_شمېره_د_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
            {
                #إذا ( config_SUPPORT_STATIC_ویش == 1 )
                {
                    StaticTask_t * pxTimerTaskTCBBuffer = عدم;
                    ډېری_Type_t * px_Timer_Task_ډېری_بفر = عدم;
                    config_ډېری_ژوروالی_TYPE ux_Timer_Task_ډېری_اندازه;

                    v_Application_Get_Timer_Task_حافظه( &pxTimerTaskTCBBuffer, &px_Timer_Task_ډېری_بفر, &ux_Timer_Task_ډېری_اندازه );
                    xTimerTaskHandle = x_Task_Create_Static_Affinity_سېټ( prvTimerTask,
                                                                     configTIMER_SERVICE_TASK_NAME,
                                                                     ux_Timer_Task_ډېری_اندازه,
                                                                     عدم,
                                                                     ( ( UBaseType_t ) config_TIMER_TASK_لومړیتوب ) | پورټ_PRIVILEGE_بټ,
                                                                     px_Timer_Task_ډېری_بفر,
                                                                     pxTimerTaskTCBBuffer,
                                                                     configTIMER_SERVICE_TASK_CORE_AFFINITY );

                    إذا( xTimerTaskHandle != عدم )
                    {
                        xReturn = pdPASS;
                    }
                }
                #وإلا /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
                {
                    xReturn = x_Task_Create_Affinity_سېټ( prvTimerTask,
                                                      configTIMER_SERVICE_TASK_NAME,
                                                      config_TIMER_TASK_ډېری_ژوروالی,
                                                      عدم,
                                                      ( ( UBaseType_t ) config_TIMER_TASK_لومړیتوب ) | پورټ_PRIVILEGE_بټ,
                                                      configTIMER_SERVICE_TASK_CORE_AFFINITY,
                                                      &xTimerTaskHandle );
                }
                #د_که_پای /* configSUPPORT_STATIC_ALLOCATION */
            }
            #وإلا /* #if ( ( configNUMBER_OF_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            {
                #إذا ( config_SUPPORT_STATIC_ویش == 1 )
                {
                    StaticTask_t * pxTimerTaskTCBBuffer = عدم;
                    ډېری_Type_t * px_Timer_Task_ډېری_بفر = عدم;
                    config_ډېری_ژوروالی_TYPE ux_Timer_Task_ډېری_اندازه;

                    v_Application_Get_Timer_Task_حافظه( &pxTimerTaskTCBBuffer, &px_Timer_Task_ډېری_بفر, &ux_Timer_Task_ډېری_اندازه );
                    xTimerTaskHandle = xTaskCreateStatic( prvTimerTask,
                                                          configTIMER_SERVICE_TASK_NAME,
                                                          ux_Timer_Task_ډېری_اندازه,
                                                          عدم,
                                                          ( ( UBaseType_t ) config_TIMER_TASK_لومړیتوب ) | پورټ_PRIVILEGE_بټ,
                                                          px_Timer_Task_ډېری_بفر,
                                                          pxTimerTaskTCBBuffer );

                    إذا( xTimerTaskHandle != عدم )
                    {
                        xReturn = pdPASS;
                    }
                }
                #وإلا /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
                {
                    xReturn = xTaskCreate( prvTimerTask,
                                           configTIMER_SERVICE_TASK_NAME,
                                           config_TIMER_TASK_ډېری_ژوروالی,
                                           عدم,
                                           ( ( UBaseType_t ) config_TIMER_TASK_لومړیتوب ) | پورټ_PRIVILEGE_بټ,
                                           &xTimerTaskHandle );
                }
                #د_که_پای /* configSUPPORT_STATIC_ALLOCATION */
            }
            #د_که_پای /* #if ( ( configNUMBER_OF_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }

        configASSERT( xReturn );

        traceRETURN_xTimerCreateTimerTask( xReturn );

        أرجع xReturn;
    }
/*-----------------------------------------------------------*/

    #إذا ( config_SUPPORT_DYNAMIC_ویش == 1 )

        TimerHandle_t xTimerCreate( ثابت حرف * ثابت pcTimerName,
                                    ثابت TickType_t x_Timer_Period_دننه_Ticks,
                                    ثابت بنسټ_Type_t xAutoReload,
                                    فراغ * ثابت pvTimerID,
                                    TimerCallbackFunction_t pxCallbackFunction )
        {
            Timer_t * pxNewTimer;

            traceENTER_xTimerCreate( pcTimerName, x_Timer_Period_دننه_Ticks, xAutoReload, pvTimerID, pxCallbackFunction );

            /* MISRA Ref 11.5.1 [Malloc memory assignment] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
            /* coverity[misra_c_2012_rule_11_5_violation] */
            pxNewTimer = ( Timer_t * ) pv_پورټ_Malloc( حجم( Timer_t ) );

            إذا( pxNewTimer != عدم )
            {
                /* Status is thus far zero as the timer is not created statically
                 * and has not been started.  The auto-reload bit may get set in
                 * prvInitialiseNewTimer. */
                pxNewTimer->uc_حالت = 0x00;
                prvInitialiseNewTimer( pcTimerName, x_Timer_Period_دننه_Ticks, xAutoReload, pvTimerID, pxCallbackFunction, pxNewTimer );
            }

            traceRETURN_xTimerCreate( pxNewTimer );

            أرجع pxNewTimer;
        }

    #د_که_پای /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

    #إذا ( config_SUPPORT_STATIC_ویش == 1 )

        TimerHandle_t xTimerCreateStatic( ثابت حرف * ثابت pcTimerName,
                                          ثابت TickType_t x_Timer_Period_دننه_Ticks,
                                          ثابت بنسټ_Type_t xAutoReload,
                                          فراغ * ثابت pvTimerID,
                                          TimerCallbackFunction_t pxCallbackFunction,
                                          StaticTimer_t * px_Timer_بفر )
        {
            Timer_t * pxNewTimer;

            traceENTER_xTimerCreateStatic( pcTimerName, x_Timer_Period_دننه_Ticks, xAutoReload, pvTimerID, pxCallbackFunction, px_Timer_بفر );

            #إذا ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticTimer_t equals the size of the real timer
                 * structure. */
                متطاير size_t x_اندازه = حجم( StaticTimer_t );
                configASSERT( x_اندازه == حجم( Timer_t ) );
                ( فراغ ) x_اندازه; /* Prevent unused variable warning when configASSERT() is not defined. */
            }
            #د_که_پای /* configASSERT_DEFINED */

            /* A pointer to a StaticTimer_t structure MUST be provided, use it. */
            configASSERT( px_Timer_بفر );
            /* MISRA Ref 11.3.1 [Misaligned access] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
            /* coverity[misra_c_2012_rule_11_3_violation] */
            pxNewTimer = ( Timer_t * ) px_Timer_بفر;

            إذا( pxNewTimer != عدم )
            {
                /* Timers can be created statically or dynamically so note this
                 * timer was created statically in case it is later deleted.  The
                 * auto-reload bit may get set in prvInitialiseNewTimer(). */
                pxNewTimer->uc_حالت = ( uint8_t ) tmr_حالت_IS_STATICALLY_ALLOCATED;

                prvInitialiseNewTimer( pcTimerName, x_Timer_Period_دننه_Ticks, xAutoReload, pvTimerID, pxCallbackFunction, pxNewTimer );
            }

            traceRETURN_xTimerCreateStatic( pxNewTimer );

            أرجع pxNewTimer;
        }

    #د_که_پای /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

    ساكن فراغ prvInitialiseNewTimer( ثابت حرف * ثابت pcTimerName,
                                       ثابت TickType_t x_Timer_Period_دننه_Ticks,
                                       ثابت بنسټ_Type_t xAutoReload,
                                       فراغ * ثابت pvTimerID,
                                       TimerCallbackFunction_t pxCallbackFunction,
                                       Timer_t * pxNewTimer )
    {
        /* 0 is not a valid value for xTimerPeriodInTicks. */
        configASSERT( ( x_Timer_Period_دننه_Ticks > 0 ) );

        /* Ensure the infrastructure used by the timer service task has been
         * created/initialised. */
        prv_Check_For_Valid_List_And_کتار();

        /* Initialise the timer structure members using the function
         * parameters. */
        pxNewTimer->pcTimerName = pcTimerName;
        pxNewTimer->x_Timer_Period_دننه_Ticks = x_Timer_Period_دننه_Ticks;
        pxNewTimer->pvTimerID = pvTimerID;
        pxNewTimer->pxCallbackFunction = pxCallbackFunction;
        v_List_Initialise_توکی( &( pxNewTimer->x_Timer_List_توکی ) );

        إذا( xAutoReload != pdFALSE )
        {
            pxNewTimer->uc_حالت |= ( uint8_t ) tmr_حالت_IS_AUTORELOAD;
        }

        traceTIMER_CREATE( pxNewTimer );
    }
/*-----------------------------------------------------------*/

    بنسټ_Type_t xTimerGenericCommandFromTask( TimerHandle_t xTimer,
                                             ثابت بنسټ_Type_t xCommandID,
                                             ثابت TickType_t x_Optional_ارزښت,
                                             بنسټ_Type_t * ثابت px_Higher_لومړیتوب_Task_Woken,
                                             ثابت TickType_t x_Ticks_منزل_Wait )
    {
        بنسټ_Type_t xReturn = pdFAIL;
        Daemon_Task_پیغام_t x_پیغام;

        ( فراغ ) px_Higher_لومړیتوب_Task_Woken;

        traceENTER_xTimerGenericCommandFromTask( xTimer, xCommandID, x_Optional_ارزښت, px_Higher_لومړیتوب_Task_Woken, x_Ticks_منزل_Wait );

        configASSERT( xTimer );

        /* Send a message to the timer service task to perform a particular action
         * on a particular timer definition. */
        إذا( x_Timer_کتار != عدم )
        {
            /* Send a command to the timer service task to start the xTimer timer. */
            x_پیغام.x_پیغام_ID = xCommandID;
            x_پیغام.u.xTimerParameters.x_پیغام_ارزښت = x_Optional_ارزښت;
            x_پیغام.u.xTimerParameters.pxTimer = xTimer;

            configASSERT( xCommandID < tmr_لومړی_FROM_ISR_COMMAND );

            إذا( xCommandID < tmr_لومړی_FROM_ISR_COMMAND )
            {
                إذا( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING )
                {
                    xReturn = x_کتار_Send_منزل_شا( x_Timer_کتار, &x_پیغام, x_Ticks_منزل_Wait );
                }
                وإلا
                {
                    xReturn = x_کتار_Send_منزل_شا( x_Timer_کتار, &x_پیغام, tmrNO_DELAY );
                }
            }

            traceTIMER_COMMAND_SEND( xTimer, xCommandID, x_Optional_ارزښت, xReturn );
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }

        traceRETURN_xTimerGenericCommandFromTask( xReturn );

        أرجع xReturn;
    }
/*-----------------------------------------------------------*/

    بنسټ_Type_t xTimerGenericCommandFromISR( TimerHandle_t xTimer,
                                            ثابت بنسټ_Type_t xCommandID,
                                            ثابت TickType_t x_Optional_ارزښت,
                                            بنسټ_Type_t * ثابت px_Higher_لومړیتوب_Task_Woken,
                                            ثابت TickType_t x_Ticks_منزل_Wait )
    {
        بنسټ_Type_t xReturn = pdFAIL;
        Daemon_Task_پیغام_t x_پیغام;

        ( فراغ ) x_Ticks_منزل_Wait;

        traceENTER_xTimerGenericCommandFromISR( xTimer, xCommandID, x_Optional_ارزښت, px_Higher_لومړیتوب_Task_Woken, x_Ticks_منزل_Wait );

        configASSERT( xTimer );

        /* Send a message to the timer service task to perform a particular action
         * on a particular timer definition. */
        إذا( x_Timer_کتار != عدم )
        {
            /* Send a command to the timer service task to start the xTimer timer. */
            x_پیغام.x_پیغام_ID = xCommandID;
            x_پیغام.u.xTimerParameters.x_پیغام_ارزښت = x_Optional_ارزښت;
            x_پیغام.u.xTimerParameters.pxTimer = xTimer;

            configASSERT( xCommandID >= tmr_لومړی_FROM_ISR_COMMAND );

            إذا( xCommandID >= tmr_لومړی_FROM_ISR_COMMAND )
            {
                xReturn = x_کتار_Send_منزل_شا_From_ISR( x_Timer_کتار, &x_پیغام, px_Higher_لومړیتوب_Task_Woken );
            }

            traceTIMER_COMMAND_SEND( xTimer, xCommandID, x_Optional_ارزښت, xReturn );
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }

        traceRETURN_xTimerGenericCommandFromISR( xReturn );

        أرجع xReturn;
    }
/*-----------------------------------------------------------*/

    TaskHandle_t xTimerGetTimerDaemonTaskHandle( فراغ )
    {
        traceENTER_xTimerGetTimerDaemonTaskHandle();

        /* If xTimerGetTimerDaemonTaskHandle() is called before the scheduler has been
         * started, then xTimerTaskHandle will be NULL. */
        configASSERT( ( xTimerTaskHandle != عدم ) );

        traceRETURN_xTimerGetTimerDaemonTaskHandle( xTimerTaskHandle );

        أرجع xTimerTaskHandle;
    }
/*-----------------------------------------------------------*/

    TickType_t xTimerGetPeriod( TimerHandle_t xTimer )
    {
        Timer_t * pxTimer = xTimer;

        traceENTER_xTimerGetPeriod( xTimer );

        configASSERT( xTimer );

        traceRETURN_xTimerGetPeriod( pxTimer->x_Timer_Period_دننه_Ticks );

        أرجع pxTimer->x_Timer_Period_دننه_Ticks;
    }
/*-----------------------------------------------------------*/

    فراغ v_Timer_سېټ_Reload_ډېرپېښ( TimerHandle_t xTimer,
                              ثابت بنسټ_Type_t xAutoReload )
    {
        Timer_t * pxTimer = xTimer;

        trace_ENTER_v_Timer_سېټ_Reload_ډېرپېښ( xTimer, xAutoReload );

        configASSERT( xTimer );
        taskENTER_CRITICAL();
        {
            إذا( xAutoReload != pdFALSE )
            {
                pxTimer->uc_حالت |= ( uint8_t ) tmr_حالت_IS_AUTORELOAD;
            }
            وإلا
            {
                pxTimer->uc_حالت &= ( ( uint8_t ) ~tmr_حالت_IS_AUTORELOAD );
            }
        }
        taskEXIT_CRITICAL();

        trace_RETURN_v_Timer_سېټ_Reload_ډېرپېښ();
    }
/*-----------------------------------------------------------*/

    بنسټ_Type_t x_Timer_Get_Reload_ډېرپېښ( TimerHandle_t xTimer )
    {
        Timer_t * pxTimer = xTimer;
        بنسټ_Type_t xReturn;

        trace_ENTER_x_Timer_Get_Reload_ډېرپېښ( xTimer );

        configASSERT( xTimer );
        taskENTER_CRITICAL();
        {
            إذا( ( pxTimer->uc_حالت & tmr_حالت_IS_AUTORELOAD ) == 0U )
            {
                /* Not an auto-reload timer. */
                xReturn = pdFALSE;
            }
            وإلا
            {
                /* Is an auto-reload timer. */
                xReturn = pdTRUE;
            }
        }
        taskEXIT_CRITICAL();

        trace_RETURN_x_Timer_Get_Reload_ډېرپېښ( xReturn );

        أرجع xReturn;
    }

    UBaseType_t ux_Timer_Get_Reload_ډېرپېښ( TimerHandle_t xTimer )
    {
        UBaseType_t uxReturn;

        trace_ENTER_ux_Timer_Get_Reload_ډېرپېښ( xTimer );

        uxReturn = ( UBaseType_t ) x_Timer_Get_Reload_ډېرپېښ( xTimer );

        trace_RETURN_ux_Timer_Get_Reload_ډېرپېښ( uxReturn );

        أرجع uxReturn;
    }
/*-----------------------------------------------------------*/

    TickType_t x_Timer_Get_Expiry_وخت( TimerHandle_t xTimer )
    {
        Timer_t * pxTimer = xTimer;
        TickType_t xReturn;

        trace_ENTER_x_Timer_Get_Expiry_وخت( xTimer );

        configASSERT( xTimer );
        xReturn = list_GET_LIST_توکی_ارزښت( &( pxTimer->x_Timer_List_توکی ) );

        trace_RETURN_x_Timer_Get_Expiry_وخت( xReturn );

        أرجع xReturn;
    }
/*-----------------------------------------------------------*/

    #إذا ( config_SUPPORT_STATIC_ویش == 1 )
        بنسټ_Type_t x_Timer_Get_Static_بفر( TimerHandle_t xTimer,
                                          StaticTimer_t ** ppx_Timer_بفر )
        {
            بنسټ_Type_t xReturn;
            Timer_t * pxTimer = xTimer;

            trace_ENTER_x_Timer_Get_Static_بفر( xTimer, ppx_Timer_بفر );

            configASSERT( ppx_Timer_بفر != عدم );

            إذا( ( pxTimer->uc_حالت & tmr_حالت_IS_STATICALLY_ALLOCATED ) != 0U )
            {
                /* MISRA Ref 11.3.1 [Misaligned access] */
                /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
                /* coverity[misra_c_2012_rule_11_3_violation] */
                *ppx_Timer_بفر = ( StaticTimer_t * ) pxTimer;
                xReturn = pdTRUE;
            }
            وإلا
            {
                xReturn = pdFALSE;
            }

            trace_RETURN_x_Timer_Get_Static_بفر( xReturn );

            أرجع xReturn;
        }
    #د_که_پای /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

    ثابت حرف * pcTimerGetName( TimerHandle_t xTimer )
    {
        Timer_t * pxTimer = xTimer;

        traceENTER_pcTimerGetName( xTimer );

        configASSERT( xTimer );

        traceRETURN_pcTimerGetName( pxTimer->pcTimerName );

        أرجع pxTimer->pcTimerName;
    }
/*-----------------------------------------------------------*/

    ساكن فراغ prvReloadTimer( Timer_t * ثابت pxTimer,
                                TickType_t x_Expired_وخت,
                                ثابت TickType_t x_وخت_Now )
    {
        /* Insert the timer into the appropriate list for the next expiry time.
         * If the next expiry time has already passed, advance the expiry time,
         * call the callback function, and try again. */
        طالما( prv_ورننباسه_Timer_دننه_فعال_List( pxTimer, ( x_Expired_وخت + pxTimer->x_Timer_Period_دننه_Ticks ), x_وخت_Now, x_Expired_وخت ) != pdFALSE )
        {
            /* Advance the expiry time. */
            x_Expired_وخت += pxTimer->x_Timer_Period_دننه_Ticks;

            /* Call the timer callback. */
            traceTIMER_EXPIRED( pxTimer );
            pxTimer->pxCallbackFunction( ( TimerHandle_t ) pxTimer );
        }
    }
/*-----------------------------------------------------------*/

    ساكن فراغ prvProcessExpiredTimer( ثابت TickType_t x_بل_Expire_وخت,
                                        ثابت TickType_t x_وخت_Now )
    {
        /* MISRA Ref 11.5.3 [Void pointer assignment] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
        /* coverity[misra_c_2012_rule_11_5_violation] */
        Timer_t * ثابت pxTimer = ( Timer_t * ) list_GET_OWNER_د_سر_ثبت( pxCurrentTimerList );

        /* Remove the timer from the list of active timers.  A check has already
         * been performed to ensure the list is not empty. */

        ( فراغ ) uxListRemove( &( pxTimer->x_Timer_List_توکی ) );

        /* If the timer is an auto-reload timer then calculate the next
         * expiry time and re-insert the timer in the list of active timers. */
        إذا( ( pxTimer->uc_حالت & tmr_حالت_IS_AUTORELOAD ) != 0U )
        {
            prvReloadTimer( pxTimer, x_بل_Expire_وخت, x_وخت_Now );
        }
        وإلا
        {
            pxTimer->uc_حالت &= ( ( uint8_t ) ~tmr_حالت_IS_فعال );
        }

        /* Call the timer callback. */
        traceTIMER_EXPIRED( pxTimer );
        pxTimer->pxCallbackFunction( ( TimerHandle_t ) pxTimer );
    }
/*-----------------------------------------------------------*/

    ساكن پورټ_TASK_FUNCTION( prvTimerTask, pvParameters )
    {
        TickType_t x_بل_Expire_وخت;
        بنسټ_Type_t x_List_Was_تشه;

        /* Just to avoid compiler warnings. */
        ( فراغ ) pvParameters;

        #إذا ( configUSE_DAEMON_TASK_STARTUP_HOOK == 1 )
        {
            /* Allow the application writer to execute some code in the context of
             * this task at the point the task starts executing.  This is useful if the
             * application includes initialisation code that would benefit from
             * executing after the scheduler has been started. */
            vApplicationDaemonTaskStartupHook();
        }
        #د_که_پای /* configUSE_DAEMON_TASK_STARTUP_HOOK */

        لكل( ; configCONTROL_INFINITE_LOOP(); )
        {
            /* Query the timers list to see if it contains any timers, and if so,
             * obtain the time at which the next timer will expire. */
            x_بل_Expire_وخت = prv_Get_بل_Expire_وخت( &x_List_Was_تشه );

            /* If a timer has expired, process it.  Otherwise, block this task
             * until either a timer does expire, or a command is received. */
            prv_Process_Timer_Or_بلاک_Task( x_بل_Expire_وخت, x_List_Was_تشه );

            /* Empty the command queue. */
            prvProcessReceivedCommands();
        }
    }
/*-----------------------------------------------------------*/

    ساكن فراغ prv_Process_Timer_Or_بلاک_Task( ثابت TickType_t x_بل_Expire_وخت,
                                            بنسټ_Type_t x_List_Was_تشه )
    {
        TickType_t x_وخت_Now;
        بنسټ_Type_t xTimerListsWereSwitched;

        vTaskSuspendAll();
        {
            /* Obtain the time now to make an assessment as to whether the timer
             * has expired or not.  If obtaining the time causes the lists to switch
             * then don't process this timer as any timers that remained in the list
             * when the lists were switched will have been processed within the
             * prvSampleTimeNow() function. */
            x_وخت_Now = prv_Sample_وخت_Now( &xTimerListsWereSwitched );

            إذا( xTimerListsWereSwitched == pdFALSE )
            {
                /* The tick count has not overflowed, has the timer expired? */
                إذا( ( x_List_Was_تشه == pdFALSE ) && ( x_بل_Expire_وخت <= x_وخت_Now ) )
                {
                    ( فراغ ) xTaskResumeAll();
                    prvProcessExpiredTimer( x_بل_Expire_وخت, x_وخت_Now );
                }
                وإلا
                {
                    /* The tick count has not overflowed, and the next expire
                     * time has not been reached yet.  This task should therefore
                     * block to wait for the next expire time or a command to be
                     * received - whichever comes first.  The following line cannot
                     * be reached unless xNextExpireTime > xTimeNow, except in the
                     * case when the current timer list is empty. */
                    إذا( x_List_Was_تشه != pdFALSE )
                    {
                        /* The current timer list is empty - is the overflow list
                         * also empty? */
                        x_List_Was_تشه = list_LIST_IS_تشه( px_ډکېدنه_Timer_List );
                    }

                    v_کتار_Wait_For_پیغام_Restricted( x_Timer_کتار, ( x_بل_Expire_وخت - x_وخت_Now ), x_List_Was_تشه );

                    إذا( xTaskResumeAll() == pdFALSE )
                    {
                        /* Yield to wait for either a command to arrive, or the
                         * block time to expire.  If a command arrived between the
                         * critical section being exited and this yield then the yield
                         * will not cause the task to block. */
                        taskYIELD_WITHIN_API();
                    }
                    وإلا
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
            }
            وإلا
            {
                ( فراغ ) xTaskResumeAll();
            }
        }
    }
/*-----------------------------------------------------------*/

    ساكن TickType_t prv_Get_بل_Expire_وخت( بنسټ_Type_t * ثابت px_List_Was_تشه )
    {
        TickType_t x_بل_Expire_وخت;

        /* Timers are listed in expiry time order, with the head of the list
         * referencing the task that will expire first.  Obtain the time at which
         * the timer with the nearest expiry time will expire.  If there are no
         * active timers then just set the next expire time to 0.  That will cause
         * this task to unblock when the tick count overflows, at which point the
         * timer lists will be switched and the next expiry time can be
         * re-assessed.  */
        *px_List_Was_تشه = list_LIST_IS_تشه( pxCurrentTimerList );

        إذا( *px_List_Was_تشه == pdFALSE )
        {
            x_بل_Expire_وخت = list_GET_توکی_ارزښت_د_سر_ثبت( pxCurrentTimerList );
        }
        وإلا
        {
            /* Ensure the task unblocks when the tick count rolls over. */
            x_بل_Expire_وخت = ( TickType_t ) 0U;
        }

        أرجع x_بل_Expire_وخت;
    }
/*-----------------------------------------------------------*/

    ساكن TickType_t prv_Sample_وخت_Now( بنسټ_Type_t * ثابت pxTimerListsWereSwitched )
    {
        TickType_t x_وخت_Now;
        PRIVILEGED_معلومات ساكن TickType_t x_وروستی_وخت = ( TickType_t ) 0U;

        x_وخت_Now = x_Task_Get_Tick_شمېر();

        إذا( x_وخت_Now < x_وروستی_وخت )
        {
            prvSwitchTimerLists();
            *pxTimerListsWereSwitched = pdTRUE;
        }
        وإلا
        {
            *pxTimerListsWereSwitched = pdFALSE;
        }

        x_وروستی_وخت = x_وخت_Now;

        أرجع x_وخت_Now;
    }
/*-----------------------------------------------------------*/

    ساكن بنسټ_Type_t prv_ورننباسه_Timer_دننه_فعال_List( Timer_t * ثابت pxTimer,
                                                  ثابت TickType_t x_بل_Expiry_وخت,
                                                  ثابت TickType_t x_وخت_Now,
                                                  ثابت TickType_t x_Command_وخت )
    {
        بنسټ_Type_t xProcessTimerNow = pdFALSE;

        list_سېټ_LIST_توکی_ارزښت( &( pxTimer->x_Timer_List_توکی ), x_بل_Expiry_وخت );
        list_سېټ_LIST_توکی_OWNER( &( pxTimer->x_Timer_List_توکی ), pxTimer );

        إذا( x_بل_Expiry_وخت <= x_وخت_Now )
        {
            /* Has the expiry time elapsed between the command to start/reset a
             * timer was issued, and the time the command was processed? */
            إذا( ( ( TickType_t ) ( x_وخت_Now - x_Command_وخت ) ) >= pxTimer->x_Timer_Period_دننه_Ticks )
            {
                /* The time between a command being issued and the command being
                 * processed actually exceeds the timers period.  */
                xProcessTimerNow = pdTRUE;
            }
            وإلا
            {
                v_List_ورننباسه( px_ډکېدنه_Timer_List, &( pxTimer->x_Timer_List_توکی ) );
            }
        }
        وإلا
        {
            إذا( ( x_وخت_Now < x_Command_وخت ) && ( x_بل_Expiry_وخت >= x_Command_وخت ) )
            {
                /* If, since the command was issued, the tick count has overflowed
                 * but the expiry time has not, then the timer must have already passed
                 * its expiry time and should be processed immediately. */
                xProcessTimerNow = pdTRUE;
            }
            وإلا
            {
                v_List_ورننباسه( pxCurrentTimerList, &( pxTimer->x_Timer_List_توکی ) );
            }
        }

        أرجع xProcessTimerNow;
    }
/*-----------------------------------------------------------*/

    ساكن فراغ prvProcessReceivedCommands( فراغ )
    {
        Daemon_Task_پیغام_t x_پیغام = { 0 };
        Timer_t * pxTimer;
        بنسټ_Type_t xTimerListsWereSwitched;
        TickType_t x_وخت_Now;

        طالما( x_کتار_Receive( x_Timer_کتار, &x_پیغام, tmrNO_DELAY ) != pdFAIL )
        {
            #إذا ( INCLUDE_xTimerPendFunctionCall == 1 )
            {
                /* Negative commands are pended function calls rather than timer
                 * commands. */
                إذا( x_پیغام.x_پیغام_ID < ( بنسټ_Type_t ) 0 )
                {
                    ثابت CallbackParameters_t * ثابت pxCallback = &( x_پیغام.u.xCallbackParameters );

                    /* The timer uses the xCallbackParameters member to request a
                     * callback be executed.  Check the callback is not NULL. */
                    configASSERT( pxCallback );

                    /* Call the function. */
                    pxCallback->pxCallbackFunction( pxCallback->pvParameter1, pxCallback->ulParameter2 );
                }
                وإلا
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            #د_که_پای /* INCLUDE_xTimerPendFunctionCall */

            /* Commands that are positive are timer commands rather than pended
             * function calls. */
            إذا( x_پیغام.x_پیغام_ID >= ( بنسټ_Type_t ) 0 )
            {
                /* The messages uses the xTimerParameters member to work on a
                 * software timer. */
                pxTimer = x_پیغام.u.xTimerParameters.pxTimer;

                إذا( listIS_CONTAINED_WITHIN( عدم, &( pxTimer->x_Timer_List_توکی ) ) == pdFALSE )
                {
                    /* The timer is in a list, remove it. */
                    ( فراغ ) uxListRemove( &( pxTimer->x_Timer_List_توکی ) );
                }
                وإلا
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                traceTIMER_COMMAND_RECEIVED( pxTimer, x_پیغام.x_پیغام_ID, x_پیغام.u.xTimerParameters.x_پیغام_ارزښت );

                /* In this case the xTimerListsWereSwitched parameter is not used, but
                 *  it must be present in the function call.  prvSampleTimeNow() must be
                 *  called after the message is received from xTimerQueue so there is no
                 *  possibility of a higher priority task adding a message to the message
                 *  queue with a time that is ahead of the timer daemon task (because it
                 *  pre-empted the timer daemon task after the xTimeNow value was set). */
                x_وخت_Now = prv_Sample_وخت_Now( &xTimerListsWereSwitched );

                بدل( x_پیغام.x_پیغام_ID )
                {
                    حالة tmr_COMMAND_پیل:
                    حالة tmr_COMMAND_پیل_FROM_ISR:
                    حالة tmrCOMMAND_RESET:
                    حالة tmrCOMMAND_RESET_FROM_ISR:
                        /* Start or restart a timer. */
                        pxTimer->uc_حالت |= ( uint8_t ) tmr_حالت_IS_فعال;

                        إذا( prv_ورننباسه_Timer_دننه_فعال_List( pxTimer, x_پیغام.u.xTimerParameters.x_پیغام_ارزښت + pxTimer->x_Timer_Period_دننه_Ticks, x_وخت_Now, x_پیغام.u.xTimerParameters.x_پیغام_ارزښت ) != pdFALSE )
                        {
                            /* The timer expired before it was added to the active
                             * timer list.  Process it now. */
                            إذا( ( pxTimer->uc_حالت & tmr_حالت_IS_AUTORELOAD ) != 0U )
                            {
                                prvReloadTimer( pxTimer, x_پیغام.u.xTimerParameters.x_پیغام_ارزښت + pxTimer->x_Timer_Period_دننه_Ticks, x_وخت_Now );
                            }
                            وإلا
                            {
                                pxTimer->uc_حالت &= ( ( uint8_t ) ~tmr_حالت_IS_فعال );
                            }

                            /* Call the timer callback. */
                            traceTIMER_EXPIRED( pxTimer );
                            pxTimer->pxCallbackFunction( ( TimerHandle_t ) pxTimer );
                        }
                        وإلا
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }

                        اكسر;

                    حالة tmrCOMMAND_STOP:
                    حالة tmrCOMMAND_STOP_FROM_ISR:
                        /* The timer has already been removed from the active list. */
                        pxTimer->uc_حالت &= ( ( uint8_t ) ~tmr_حالت_IS_فعال );
                        اكسر;

                    حالة tmrCOMMAND_CHANGE_PERIOD:
                    حالة tmrCOMMAND_CHANGE_PERIOD_FROM_ISR:
                        pxTimer->uc_حالت |= ( uint8_t ) tmr_حالت_IS_فعال;
                        pxTimer->x_Timer_Period_دننه_Ticks = x_پیغام.u.xTimerParameters.x_پیغام_ارزښت;
                        configASSERT( ( pxTimer->x_Timer_Period_دننه_Ticks > 0 ) );

                        /* The new period does not really have a reference, and can
                         * be longer or shorter than the old one.  The command time is
                         * therefore set to the current time, and as the period cannot
                         * be zero the next expiry time can only be in the future,
                         * meaning (unlike for the xTimerStart() case above) there is
                         * no fail case that needs to be handled here. */
                        ( فراغ ) prv_ورننباسه_Timer_دننه_فعال_List( pxTimer, ( x_وخت_Now + pxTimer->x_Timer_Period_دننه_Ticks ), x_وخت_Now, x_وخت_Now );
                        اكسر;

                    حالة tmrCOMMAND_DELETE:
                        #إذا ( config_SUPPORT_DYNAMIC_ویش == 1 )
                        {
                            /* The timer has already been removed from the active list,
                             * just free up the memory if the memory was dynamically
                             * allocated. */
                            إذا( ( pxTimer->uc_حالت & tmr_حالت_IS_STATICALLY_ALLOCATED ) == ( uint8_t ) 0 )
                            {
                                v_پورټ_Free( pxTimer );
                            }
                            وإلا
                            {
                                pxTimer->uc_حالت &= ( ( uint8_t ) ~tmr_حالت_IS_فعال );
                            }
                        }
                        #وإلا /* if ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) */
                        {
                            /* If dynamic allocation is not enabled, the memory
                             * could not have been dynamically allocated. So there is
                             * no need to free the memory - just mark the timer as
                             * "not active". */
                            pxTimer->uc_حالت &= ( ( uint8_t ) ~tmr_حالت_IS_فعال );
                        }
                        #د_که_پای /* configSUPPORT_DYNAMIC_ALLOCATION */
                        اكسر;

                    افتراضي:
                        /* Don't expect to get here. */
                        اكسر;
                }
            }
        }
    }
/*-----------------------------------------------------------*/

    ساكن فراغ prvSwitchTimerLists( فراغ )
    {
        TickType_t x_بل_Expire_وخت;
        List_t * px_لنډمهاله;

        /* The tick count has overflowed.  The timer lists must be switched.
         * If there are any timers still referenced from the current timer list
         * then they must have expired and should be processed before the lists
         * are switched. */
        طالما( list_LIST_IS_تشه( pxCurrentTimerList ) == pdFALSE )
        {
            x_بل_Expire_وخت = list_GET_توکی_ارزښت_د_سر_ثبت( pxCurrentTimerList );

            /* Process the expired timer.  For auto-reload timers, be careful to
             * process only expirations that occur on the current list.  Further
             * expirations must wait until after the lists are switched. */
            prvProcessExpiredTimer( x_بل_Expire_وخت, tmr_MAX_وخت_BEFORE_ډکېدنه );
        }

        px_لنډمهاله = pxCurrentTimerList;
        pxCurrentTimerList = px_ډکېدنه_Timer_List;
        px_ډکېدنه_Timer_List = px_لنډمهاله;
    }
/*-----------------------------------------------------------*/

    ساكن فراغ prv_Check_For_Valid_List_And_کتار( فراغ )
    {
        /* Check that the list from which active timers are referenced, and the
         * queue used to communicate with the timer service, have been
         * initialised. */
        taskENTER_CRITICAL();
        {
            إذا( x_Timer_کتار == عدم )
            {
                vListInitialise( &x_فعال_Timer_List1 );
                vListInitialise( &x_فعال_Timer_List2 );
                pxCurrentTimerList = &x_فعال_Timer_List1;
                px_ډکېدنه_Timer_List = &x_فعال_Timer_List2;

                #إذا ( config_SUPPORT_STATIC_ویش == 1 )
                {
                    /* The timer queue is allocated statically in case
                     * configSUPPORT_DYNAMIC_ALLOCATION is 0. */
                    PRIVILEGED_معلومات ساكن Static_کتار_t x_Static_Timer_کتار;
                    PRIVILEGED_معلومات ساكن uint8_t uc_Static_Timer_کتار_Storage[ ( size_t ) config_TIMER_کتار_اوږدتیا * حجم( Daemon_Task_پیغام_t ) ];

                    x_Timer_کتار = x_کتار_Create_Static( ( UBaseType_t ) config_TIMER_کتار_اوږدتیا, ( UBaseType_t ) حجم( Daemon_Task_پیغام_t ), &( uc_Static_Timer_کتار_Storage[ 0 ] ), &x_Static_Timer_کتار );
                }
                #وإلا
                {
                    x_Timer_کتار = x_کتار_Create( ( UBaseType_t ) config_TIMER_کتار_اوږدتیا, ( UBaseType_t ) حجم( Daemon_Task_پیغام_t ) );
                }
                #د_که_پای /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */

                #إذا ( config_کتار_REGISTRY_اندازه > 0 )
                {
                    إذا( x_Timer_کتار != عدم )
                    {
                        v_کتار_جمع_کړه_منزل_Registry( x_Timer_کتار, "TmrQ" );
                    }
                    وإلا
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #د_که_پای /* configQUEUE_REGISTRY_SIZE */
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        taskEXIT_CRITICAL();
    }
/*-----------------------------------------------------------*/

    بنسټ_Type_t x_Timer_Is_Timer_فعال( TimerHandle_t xTimer )
    {
        بنسټ_Type_t xReturn;
        Timer_t * pxTimer = xTimer;

        trace_ENTER_x_Timer_Is_Timer_فعال( xTimer );

        configASSERT( xTimer );

        /* Is the timer in the list of active timers? */
        taskENTER_CRITICAL();
        {
            إذا( ( pxTimer->uc_حالت & tmr_حالت_IS_فعال ) == 0U )
            {
                xReturn = pdFALSE;
            }
            وإلا
            {
                xReturn = pdTRUE;
            }
        }
        taskEXIT_CRITICAL();

        trace_RETURN_x_Timer_Is_Timer_فعال( xReturn );

        أرجع xReturn;
    }
/*-----------------------------------------------------------*/

    فراغ * pvTimerGetTimerID( ثابت TimerHandle_t xTimer )
    {
        Timer_t * ثابت pxTimer = xTimer;
        فراغ * pvReturn;

        traceENTER_pvTimerGetTimerID( xTimer );

        configASSERT( xTimer );

        taskENTER_CRITICAL();
        {
            pvReturn = pxTimer->pvTimerID;
        }
        taskEXIT_CRITICAL();

        traceRETURN_pvTimerGetTimerID( pvReturn );

        أرجع pvReturn;
    }
/*-----------------------------------------------------------*/

    فراغ v_Timer_سېټ_Timer_ID( TimerHandle_t xTimer,
                           فراغ * pvNewID )
    {
        Timer_t * ثابت pxTimer = xTimer;

        trace_ENTER_v_Timer_سېټ_Timer_ID( xTimer, pvNewID );

        configASSERT( xTimer );

        taskENTER_CRITICAL();
        {
            pxTimer->pvTimerID = pvNewID;
        }
        taskEXIT_CRITICAL();

        trace_RETURN_v_Timer_سېټ_Timer_ID();
    }
/*-----------------------------------------------------------*/

    #إذا ( INCLUDE_xTimerPendFunctionCall == 1 )

        بنسټ_Type_t xTimerPendFunctionCallFromISR( PendedFunction_t x_Function_منزل_Pend,
                                                  فراغ * pvParameter1,
                                                  uint32_t ulParameter2,
                                                  بنسټ_Type_t * px_Higher_لومړیتوب_Task_Woken )
        {
            Daemon_Task_پیغام_t x_پیغام;
            بنسټ_Type_t xReturn;

            traceENTER_xTimerPendFunctionCallFromISR( x_Function_منزل_Pend, pvParameter1, ulParameter2, px_Higher_لومړیتوب_Task_Woken );

            /* Complete the message with the function parameters and post it to the
             * daemon task. */
            x_پیغام.x_پیغام_ID = tmrCOMMAND_EXECUTE_CALLBACK_FROM_ISR;
            x_پیغام.u.xCallbackParameters.pxCallbackFunction = x_Function_منزل_Pend;
            x_پیغام.u.xCallbackParameters.pvParameter1 = pvParameter1;
            x_پیغام.u.xCallbackParameters.ulParameter2 = ulParameter2;

            xReturn = x_کتار_Send_From_ISR( x_Timer_کتار, &x_پیغام, px_Higher_لومړیتوب_Task_Woken );

            tracePEND_FUNC_CALL_FROM_ISR( x_Function_منزل_Pend, pvParameter1, ulParameter2, xReturn );
            traceRETURN_xTimerPendFunctionCallFromISR( xReturn );

            أرجع xReturn;
        }

    #د_که_پای /* INCLUDE_xTimerPendFunctionCall */
/*-----------------------------------------------------------*/

    #إذا ( INCLUDE_xTimerPendFunctionCall == 1 )

        بنسټ_Type_t xTimerPendFunctionCall( PendedFunction_t x_Function_منزل_Pend,
                                           فراغ * pvParameter1,
                                           uint32_t ulParameter2,
                                           TickType_t x_Ticks_منزل_Wait )
        {
            Daemon_Task_پیغام_t x_پیغام;
            بنسټ_Type_t xReturn;

            traceENTER_xTimerPendFunctionCall( x_Function_منزل_Pend, pvParameter1, ulParameter2, x_Ticks_منزل_Wait );

            /* This function can only be called after a timer has been created or
             * after the scheduler has been started because, until then, the timer
             * queue does not exist. */
            configASSERT( x_Timer_کتار );

            /* Complete the message with the function parameters and post it to the
             * daemon task. */
            x_پیغام.x_پیغام_ID = tmrCOMMAND_EXECUTE_CALLBACK;
            x_پیغام.u.xCallbackParameters.pxCallbackFunction = x_Function_منزل_Pend;
            x_پیغام.u.xCallbackParameters.pvParameter1 = pvParameter1;
            x_پیغام.u.xCallbackParameters.ulParameter2 = ulParameter2;

            xReturn = x_کتار_Send_منزل_شا( x_Timer_کتار, &x_پیغام, x_Ticks_منزل_Wait );

            tracePEND_FUNC_CALL( x_Function_منزل_Pend, pvParameter1, ulParameter2, xReturn );
            traceRETURN_xTimerPendFunctionCall( xReturn );

            أرجع xReturn;
        }

    #د_که_پای /* INCLUDE_xTimerPendFunctionCall */
/*-----------------------------------------------------------*/

    #إذا ( configUSE_TRACE_FACILITY == 1 )

        UBaseType_t ux_Timer_Get_Timer_شمېره( TimerHandle_t xTimer )
        {
            trace_ENTER_ux_Timer_Get_Timer_شمېره( xTimer );

            trace_RETURN_ux_Timer_Get_Timer_شمېره( ( ( Timer_t * ) xTimer )->ux_Timer_شمېره );

            أرجع ( ( Timer_t * ) xTimer )->ux_Timer_شمېره;
        }

    #د_که_پای /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

    #إذا ( configUSE_TRACE_FACILITY == 1 )

        فراغ v_Timer_سېټ_Timer_شمېره( TimerHandle_t xTimer,
                                   UBaseType_t ux_Timer_شمېره )
        {
            trace_ENTER_v_Timer_سېټ_Timer_شمېره( xTimer, ux_Timer_شمېره );

            ( ( Timer_t * ) xTimer )->ux_Timer_شمېره = ux_Timer_شمېره;

            trace_RETURN_v_Timer_سېټ_Timer_شمېره();
        }

    #د_که_پای /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

/*
 * Reset the state in this file. This state is normally initialized at start up.
 * This function must be called by the application before restarting the
 * scheduler.
 */
    فراغ vTimerResetState( فراغ )
    {
        x_Timer_کتار = عدم;
        xTimerTaskHandle = عدم;
    }
/*-----------------------------------------------------------*/

/* This entire source file will be skipped if the application is not configured
 * to include software timer functionality.  If you want to include software timer
 * functionality then ensure configUSE_TIMERS is set to 1 in FreeRTOSConfig.h. */
#د_که_پای /* configUSE_TIMERS == 1 */
