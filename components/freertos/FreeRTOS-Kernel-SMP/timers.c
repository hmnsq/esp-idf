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
#شامل <stdlib.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#تعریف_کرو MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#شامل "FreeRTOS.h"
#شامل "task.h"
#شامل "queue.h"
#شامل "timers.h"

#اگر ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 0 )
    #غلطی configUSE_TIMERS must be سیٹ مقصد 1 مقصد make the xTimerPendFunctionCall() function دستیاب.
#اگر_ختم

/* The MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#تعریف_منسوخ MPU_WRAPPERS_INCLUDED_FROM_API_FILE


/* This entire source file will be skipped if the application is not configured
 * to include software timer functionality.  This #if is closed at the very bottom
 * of this file.  If you want to include software timer functionality then ensure
 * configUSE_TIMERS is set to 1 in FreeRTOSConfig.h. */
#اگر ( configUSE_TIMERS == 1 )

/* Misc definitions. */
    #تعریف_کرو tmrNO_DELAY                    ( ( TickType_t ) 0U )
    #تعریف_کرو tmr_MAX_وقت_BEFORE_حد_سے_تجاوز    ( ( TickType_t ) -1 )

/* The name assigned to the timer service task. This can be overridden by
 * defining configTIMER_SERVICE_TASK_NAME in FreeRTOSConfig.h. */
    #اگر_غیر_تعریف_شدہ configTIMER_SERVICE_TASK_NAME
        #تعریف_کرو configTIMER_SERVICE_TASK_NAME    "Tmr Svc"
    #اگر_ختم

    #اگر ( ( config_نمبر_کا_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )

/* The core affinity assigned to the timer service task on SMP systems.
 * This can be overridden by defining configTIMER_SERVICE_TASK_CORE_AFFINITY in FreeRTOSConfig.h. */
        #اگر_غیر_تعریف_شدہ configTIMER_SERVICE_TASK_CORE_AFFINITY
            #تعریف_کرو configTIMER_SERVICE_TASK_CORE_AFFINITY    tskNO_AFFINITY
        #اگر_ختم
    #اگر_ختم /* #if ( ( configNUMBER_OF_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */

/* Bit definitions used in the ucStatus member of a timer structure. */
    #تعریف_کرو tmr_حالت_IS_فعال                  ( 0x01U )
    #تعریف_کرو tmr_حالت_IS_STATICALLY_ALLOCATED    ( 0x02U )
    #تعریف_کرو tmr_حالت_IS_AUTORELOAD              ( 0x04U )

/* The definition of the timers themselves. */
    قسمتعریف ساخت tmrTimerControl                                               /* The old naming convention is used to prevent breaking kernel aware debuggers. */
    {
        ثابت حرف * pcTimerName;                                                /**< Text name.  This is not used by the kernel, it is included simply to make debugging easier. */
        List_شے_t x_Timer_List_شے;                                               /**< Standard linked list item as used by all kernel features for event management. */
        TickType_t x_Timer_Period_اندر_Ticks;                                          /**< How quickly and often the timer expires. */
        خالی * pvTimerID;                                                        /**< An ID to identify the timer.  This allows the timer to be identified when the same callback is used for multiple timers. */
        پورٹ_TIMER_CALLBACK_ATTRIBUTE TimerCallbackFunction_t pxCallbackFunction; /**< The function that will be called when the timer expires. */
        #اگر ( configUSE_TRACE_FACILITY == 1 )
            UBaseType_t ux_Timer_نمبر;                                           /**< An ID assigned by trace tools such as FreeRTOS+Trace */
        #اگر_ختم
        uint8_t uc_حالت;                                                        /**< Holds bits to say if the timer was statically allocated or not, and if it is active or not. */
    } xTIMER;

/* The old xTIMER name is maintained above then typedefed to the new Timer_t
 * name below to enable the use of older kernel aware debuggers. */
    قسمتعریف xTIMER Timer_t;

/* The definition of messages that can be sent and received on the timer queue.
 * Two types of message can be queued - messages that manipulate a software timer,
 * and messages that request the execution of a non-timer related callback.  The
 * two message types are defined in two separate structures, xTimerParametersType
 * and xCallbackParametersType respectively. */
    قسمتعریف ساخت tmrTimerParameters
    {
        TickType_t x_پیغام_قدر; /**< An optional value used by a subset of commands, for example, when changing the period of a timer. */
        Timer_t * pxTimer;        /**< The timer to which the command will be applied. */
    } TimerParameter_t;


    قسمتعریف ساخت tmrCallbackParameters
    {
        پورٹ_TIMER_CALLBACK_ATTRIBUTE
        PendedFunction_t pxCallbackFunction; /* << The callback function to execute. */
        خالی * pvParameter1;                 /* << The value that will be used as the callback functions first parameter. */
        uint32_t ulParameter2;               /* << The value that will be used as the callback functions second parameter. */
    } CallbackParameters_t;

/* The structure that contains the two message types, along with an identifier
 * that is used to determine which message type is valid. */
    قسمتعریف ساخت tmr_Timer_قطار_پیغام
    {
        بنیاد_Type_t x_پیغام_ID; /**< The command being sent to the timer service task. */
        اتحاد
        {
            TimerParameter_t xTimerParameters;

            /* Don't include xCallbackParameters if it is not going to be used as
             * it makes the structure (and therefore the timer queue) larger. */
            #اگر ( INCLUDE_xTimerPendFunctionCall == 1 )
                CallbackParameters_t xCallbackParameters;
            #اگر_ختم /* INCLUDE_xTimerPendFunctionCall */
        } u;
    } Daemon_Task_پیغام_t;

/* The list in which active timers are stored.  Timers are referenced in expire
 * time order, with the nearest expiry time at the front of the list.  Only the
 * timer service task is allowed to access these lists.
 * xActiveTimerList1 and xActiveTimerList2 could be at function scope but that
 * breaks some kernel aware debuggers, and debuggers that reply on removing the
 * static qualifier. */
    PRIVILEGED_ڈیٹا ساکن List_t x_فعال_Timer_List1;
    PRIVILEGED_ڈیٹا ساکن List_t x_فعال_Timer_List2;
    PRIVILEGED_ڈیٹا ساکن List_t * pxCurrentTimerList;
    PRIVILEGED_ڈیٹا ساکن List_t * px_حد_سے_تجاوز_Timer_List;

/* A queue that is used to send commands to the timer service task. */
    PRIVILEGED_ڈیٹا ساکن قطار_Handle_t x_Timer_قطار = تہی;
    PRIVILEGED_ڈیٹا ساکن TaskHandle_t xTimerTaskHandle = تہی;

/*-----------------------------------------------------------*/

/*
 * Initialise the infrastructure used by the timer service task if it has not
 * been initialised already.
 */
    ساکن خالی prv_Check_For_Valid_List_And_قطار( خالی ) PRIVILEGED_FUNCTION;

/*
 * The timer service task (daemon).  Timer functionality is controlled by this
 * task.  Other tasks communicate with the timer service task using the
 * xTimerQueue queue.
 */
    ساکن پورٹ_TASK_FUNCTION_PROTO( prvTimerTask, pvParameters ) PRIVILEGED_FUNCTION;

/*
 * Called by the timer service task to interpret and process a command it
 * received on the timer queue.
 */
    ساکن خالی prvProcessReceivedCommands( خالی ) PRIVILEGED_FUNCTION;

/*
 * Insert the timer into either xActiveTimerList1, or xActiveTimerList2,
 * depending on if the expire time causes a timer counter overflow.
 */
    ساکن بنیاد_Type_t prv_داخل_کرو_Timer_اندر_فعال_List( Timer_t * ثابت pxTimer,
                                                  ثابت TickType_t x_اگلا_Expiry_وقت,
                                                  ثابت TickType_t x_وقت_Now,
                                                  ثابت TickType_t x_Command_وقت ) PRIVILEGED_FUNCTION;

/*
 * Reload the specified auto-reload timer.  If the reloading is backlogged,
 * clear the backlog, calling the callback for each additional reload.  When
 * this function returns, the next expiry time is after xTimeNow.
 */
    ساکن خالی prvReloadTimer( Timer_t * ثابت pxTimer,
                                TickType_t x_Expired_وقت,
                                ثابت TickType_t x_وقت_Now ) PRIVILEGED_FUNCTION;

/*
 * An active timer has reached its expire time.  Reload the timer if it is an
 * auto-reload timer, then call its callback.
 */
    ساکن خالی prvProcessExpiredTimer( ثابت TickType_t x_اگلا_Expire_وقت,
                                        ثابت TickType_t x_وقت_Now ) PRIVILEGED_FUNCTION;

/*
 * The tick count has overflowed.  Switch the timer lists after ensuring the
 * current timer list does not still reference some timers.
 */
    ساکن خالی prvSwitchTimerLists( خالی ) PRIVILEGED_FUNCTION;

/*
 * Obtain the current tick count, setting *pxTimerListsWereSwitched to pdTRUE
 * if a tick count overflow occurred since prvSampleTimeNow() was last called.
 */
    ساکن TickType_t prv_Sample_وقت_Now( بنیاد_Type_t * ثابت pxTimerListsWereSwitched ) PRIVILEGED_FUNCTION;

/*
 * If the timer list contains any active timers then return the expire time of
 * the timer that will expire first and set *pxListWasEmpty to false.  If the
 * timer list does not contain any timers then return 0 and set *pxListWasEmpty
 * to pdTRUE.
 */
    ساکن TickType_t prv_Get_اگلا_Expire_وقت( بنیاد_Type_t * ثابت px_List_Was_خالی_خانے ) PRIVILEGED_FUNCTION;

/*
 * If a timer has expired, process it.  Otherwise, block the timer service task
 * until either a timer does expire or a command is received.
 */
    ساکن خالی prv_Process_Timer_Or_بلاک_Task( ثابت TickType_t x_اگلا_Expire_وقت,
                                            بنیاد_Type_t x_List_Was_خالی_خانے ) PRIVILEGED_FUNCTION;

/*
 * Called after a Timer_t structure has been allocated either statically or
 * dynamically to fill in the structure's members.
 */
    ساکن خالی prvInitialiseNewTimer( ثابت حرف * ثابت pcTimerName,
                                       ثابت TickType_t x_Timer_Period_اندر_Ticks,
                                       ثابت بنیاد_Type_t xAutoReload,
                                       خالی * ثابت pvTimerID,
                                       TimerCallbackFunction_t pxCallbackFunction,
                                       Timer_t * pxNewTimer ) PRIVILEGED_FUNCTION;
/*-----------------------------------------------------------*/

    بنیاد_Type_t xTimerCreateTimerTask( خالی )
    {
        بنیاد_Type_t xReturn = pdFAIL;

        traceENTER_xTimerCreateTimerTask();

        /* This function is called when the scheduler is started if
         * configUSE_TIMERS is set to 1.  Check that the infrastructure used by the
         * timer service task has been created/initialised.  If timers have already
         * been created then the initialisation will already have been performed. */
        prv_Check_For_Valid_List_And_قطار();

        اگر( x_Timer_قطار != تہی )
        {
            #اگر ( ( config_نمبر_کا_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
            {
                #اگر ( config_SUPPORT_STATIC_تخصیص == 1 )
                {
                    StaticTask_t * pxTimerTaskTCBBuffer = تہی;
                    ڈھیر_Type_t * px_Timer_Task_ڈھیر_بفر = تہی;
                    config_ڈھیر_گہرائی_TYPE ux_Timer_Task_ڈھیر_سائز;

                    v_Application_Get_Timer_Task_حافظہ( &pxTimerTaskTCBBuffer, &px_Timer_Task_ڈھیر_بفر, &ux_Timer_Task_ڈھیر_سائز );
                    xTimerTaskHandle = x_Task_Create_Static_Affinity_سیٹ( prvTimerTask,
                                                                     configTIMER_SERVICE_TASK_NAME,
                                                                     ux_Timer_Task_ڈھیر_سائز,
                                                                     تہی,
                                                                     ( ( UBaseType_t ) config_TIMER_TASK_ترجیح ) | پورٹ_PRIVILEGE_ثنائی_ہندسہ,
                                                                     px_Timer_Task_ڈھیر_بفر,
                                                                     pxTimerTaskTCBBuffer,
                                                                     configTIMER_SERVICE_TASK_CORE_AFFINITY );

                    اگر( xTimerTaskHandle != تہی )
                    {
                        xReturn = pdPASS;
                    }
                }
                #ورنہ /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
                {
                    xReturn = x_Task_Create_Affinity_سیٹ( prvTimerTask,
                                                      configTIMER_SERVICE_TASK_NAME,
                                                      config_TIMER_TASK_ڈھیر_گہرائی,
                                                      تہی,
                                                      ( ( UBaseType_t ) config_TIMER_TASK_ترجیح ) | پورٹ_PRIVILEGE_ثنائی_ہندسہ,
                                                      configTIMER_SERVICE_TASK_CORE_AFFINITY,
                                                      &xTimerTaskHandle );
                }
                #اگر_ختم /* configSUPPORT_STATIC_ALLOCATION */
            }
            #ورنہ /* #if ( ( configNUMBER_OF_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            {
                #اگر ( config_SUPPORT_STATIC_تخصیص == 1 )
                {
                    StaticTask_t * pxTimerTaskTCBBuffer = تہی;
                    ڈھیر_Type_t * px_Timer_Task_ڈھیر_بفر = تہی;
                    config_ڈھیر_گہرائی_TYPE ux_Timer_Task_ڈھیر_سائز;

                    v_Application_Get_Timer_Task_حافظہ( &pxTimerTaskTCBBuffer, &px_Timer_Task_ڈھیر_بفر, &ux_Timer_Task_ڈھیر_سائز );
                    xTimerTaskHandle = xTaskCreateStatic( prvTimerTask,
                                                          configTIMER_SERVICE_TASK_NAME,
                                                          ux_Timer_Task_ڈھیر_سائز,
                                                          تہی,
                                                          ( ( UBaseType_t ) config_TIMER_TASK_ترجیح ) | پورٹ_PRIVILEGE_ثنائی_ہندسہ,
                                                          px_Timer_Task_ڈھیر_بفر,
                                                          pxTimerTaskTCBBuffer );

                    اگر( xTimerTaskHandle != تہی )
                    {
                        xReturn = pdPASS;
                    }
                }
                #ورنہ /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
                {
                    xReturn = xTaskCreate( prvTimerTask,
                                           configTIMER_SERVICE_TASK_NAME,
                                           config_TIMER_TASK_ڈھیر_گہرائی,
                                           تہی,
                                           ( ( UBaseType_t ) config_TIMER_TASK_ترجیح ) | پورٹ_PRIVILEGE_ثنائی_ہندسہ,
                                           &xTimerTaskHandle );
                }
                #اگر_ختم /* configSUPPORT_STATIC_ALLOCATION */
            }
            #اگر_ختم /* #if ( ( configNUMBER_OF_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
        }
        ورنہ
        {
            mtCOVERAGE_TEST_MARKER();
        }

        configASSERT( xReturn );

        traceRETURN_xTimerCreateTimerTask( xReturn );

        واپس xReturn;
    }
/*-----------------------------------------------------------*/

    #اگر ( config_SUPPORT_DYNAMIC_تخصیص == 1 )

        TimerHandle_t xTimerCreate( ثابت حرف * ثابت pcTimerName,
                                    ثابت TickType_t x_Timer_Period_اندر_Ticks,
                                    ثابت بنیاد_Type_t xAutoReload,
                                    خالی * ثابت pvTimerID,
                                    TimerCallbackFunction_t pxCallbackFunction )
        {
            Timer_t * pxNewTimer;

            traceENTER_xTimerCreate( pcTimerName, x_Timer_Period_اندر_Ticks, xAutoReload, pvTimerID, pxCallbackFunction );

            /* MISRA Ref 11.5.1 [Malloc memory assignment] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
            /* coverity[misra_c_2012_rule_11_5_violation] */
            pxNewTimer = ( Timer_t * ) pv_پورٹ_Malloc( حجم( Timer_t ) );

            اگر( pxNewTimer != تہی )
            {
                /* Status is thus far zero as the timer is not created statically
                 * and has not been started.  The auto-reload bit may get set in
                 * prvInitialiseNewTimer. */
                pxNewTimer->uc_حالت = 0x00;
                prvInitialiseNewTimer( pcTimerName, x_Timer_Period_اندر_Ticks, xAutoReload, pvTimerID, pxCallbackFunction, pxNewTimer );
            }

            traceRETURN_xTimerCreate( pxNewTimer );

            واپس pxNewTimer;
        }

    #اگر_ختم /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

    #اگر ( config_SUPPORT_STATIC_تخصیص == 1 )

        TimerHandle_t xTimerCreateStatic( ثابت حرف * ثابت pcTimerName,
                                          ثابت TickType_t x_Timer_Period_اندر_Ticks,
                                          ثابت بنیاد_Type_t xAutoReload,
                                          خالی * ثابت pvTimerID,
                                          TimerCallbackFunction_t pxCallbackFunction,
                                          StaticTimer_t * px_Timer_بفر )
        {
            Timer_t * pxNewTimer;

            traceENTER_xTimerCreateStatic( pcTimerName, x_Timer_Period_اندر_Ticks, xAutoReload, pvTimerID, pxCallbackFunction, px_Timer_بفر );

            #اگر ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticTimer_t equals the size of the real timer
                 * structure. */
                متلون size_t x_سائز = حجم( StaticTimer_t );
                configASSERT( x_سائز == حجم( Timer_t ) );
                ( خالی ) x_سائز; /* Prevent unused variable warning when configASSERT() is not defined. */
            }
            #اگر_ختم /* configASSERT_DEFINED */

            /* A pointer to a StaticTimer_t structure MUST be provided, use it. */
            configASSERT( px_Timer_بفر );
            /* MISRA Ref 11.3.1 [Misaligned access] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
            /* coverity[misra_c_2012_rule_11_3_violation] */
            pxNewTimer = ( Timer_t * ) px_Timer_بفر;

            اگر( pxNewTimer != تہی )
            {
                /* Timers can be created statically or dynamically so note this
                 * timer was created statically in case it is later deleted.  The
                 * auto-reload bit may get set in prvInitialiseNewTimer(). */
                pxNewTimer->uc_حالت = ( uint8_t ) tmr_حالت_IS_STATICALLY_ALLOCATED;

                prvInitialiseNewTimer( pcTimerName, x_Timer_Period_اندر_Ticks, xAutoReload, pvTimerID, pxCallbackFunction, pxNewTimer );
            }

            traceRETURN_xTimerCreateStatic( pxNewTimer );

            واپس pxNewTimer;
        }

    #اگر_ختم /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

    ساکن خالی prvInitialiseNewTimer( ثابت حرف * ثابت pcTimerName,
                                       ثابت TickType_t x_Timer_Period_اندر_Ticks,
                                       ثابت بنیاد_Type_t xAutoReload,
                                       خالی * ثابت pvTimerID,
                                       TimerCallbackFunction_t pxCallbackFunction,
                                       Timer_t * pxNewTimer )
    {
        /* 0 is not a valid value for xTimerPeriodInTicks. */
        configASSERT( ( x_Timer_Period_اندر_Ticks > 0 ) );

        /* Ensure the infrastructure used by the timer service task has been
         * created/initialised. */
        prv_Check_For_Valid_List_And_قطار();

        /* Initialise the timer structure members using the function
         * parameters. */
        pxNewTimer->pcTimerName = pcTimerName;
        pxNewTimer->x_Timer_Period_اندر_Ticks = x_Timer_Period_اندر_Ticks;
        pxNewTimer->pvTimerID = pvTimerID;
        pxNewTimer->pxCallbackFunction = pxCallbackFunction;
        v_List_Initialise_شے( &( pxNewTimer->x_Timer_List_شے ) );

        اگر( xAutoReload != pdFALSE )
        {
            pxNewTimer->uc_حالت |= ( uint8_t ) tmr_حالت_IS_AUTORELOAD;
        }

        traceTIMER_CREATE( pxNewTimer );
    }
/*-----------------------------------------------------------*/

    بنیاد_Type_t xTimerGenericCommandFromTask( TimerHandle_t xTimer,
                                             ثابت بنیاد_Type_t xCommandID,
                                             ثابت TickType_t x_Optional_قدر,
                                             بنیاد_Type_t * ثابت px_Higher_ترجیح_Task_Woken,
                                             ثابت TickType_t x_Ticks_مقصد_Wait )
    {
        بنیاد_Type_t xReturn = pdFAIL;
        Daemon_Task_پیغام_t x_پیغام;

        ( خالی ) px_Higher_ترجیح_Task_Woken;

        traceENTER_xTimerGenericCommandFromTask( xTimer, xCommandID, x_Optional_قدر, px_Higher_ترجیح_Task_Woken, x_Ticks_مقصد_Wait );

        configASSERT( xTimer );

        /* Send a message to the timer service task to perform a particular action
         * on a particular timer definition. */
        اگر( x_Timer_قطار != تہی )
        {
            /* Send a command to the timer service task to start the xTimer timer. */
            x_پیغام.x_پیغام_ID = xCommandID;
            x_پیغام.u.xTimerParameters.x_پیغام_قدر = x_Optional_قدر;
            x_پیغام.u.xTimerParameters.pxTimer = xTimer;

            configASSERT( xCommandID < tmr_پہلا_FROM_ISR_COMMAND );

            اگر( xCommandID < tmr_پہلا_FROM_ISR_COMMAND )
            {
                اگر( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING )
                {
                    xReturn = x_قطار_Send_مقصد_عقب( x_Timer_قطار, &x_پیغام, x_Ticks_مقصد_Wait );
                }
                ورنہ
                {
                    xReturn = x_قطار_Send_مقصد_عقب( x_Timer_قطار, &x_پیغام, tmrNO_DELAY );
                }
            }

            traceTIMER_COMMAND_SEND( xTimer, xCommandID, x_Optional_قدر, xReturn );
        }
        ورنہ
        {
            mtCOVERAGE_TEST_MARKER();
        }

        traceRETURN_xTimerGenericCommandFromTask( xReturn );

        واپس xReturn;
    }
/*-----------------------------------------------------------*/

    بنیاد_Type_t xTimerGenericCommandFromISR( TimerHandle_t xTimer,
                                            ثابت بنیاد_Type_t xCommandID,
                                            ثابت TickType_t x_Optional_قدر,
                                            بنیاد_Type_t * ثابت px_Higher_ترجیح_Task_Woken,
                                            ثابت TickType_t x_Ticks_مقصد_Wait )
    {
        بنیاد_Type_t xReturn = pdFAIL;
        Daemon_Task_پیغام_t x_پیغام;

        ( خالی ) x_Ticks_مقصد_Wait;

        traceENTER_xTimerGenericCommandFromISR( xTimer, xCommandID, x_Optional_قدر, px_Higher_ترجیح_Task_Woken, x_Ticks_مقصد_Wait );

        configASSERT( xTimer );

        /* Send a message to the timer service task to perform a particular action
         * on a particular timer definition. */
        اگر( x_Timer_قطار != تہی )
        {
            /* Send a command to the timer service task to start the xTimer timer. */
            x_پیغام.x_پیغام_ID = xCommandID;
            x_پیغام.u.xTimerParameters.x_پیغام_قدر = x_Optional_قدر;
            x_پیغام.u.xTimerParameters.pxTimer = xTimer;

            configASSERT( xCommandID >= tmr_پہلا_FROM_ISR_COMMAND );

            اگر( xCommandID >= tmr_پہلا_FROM_ISR_COMMAND )
            {
                xReturn = x_قطار_Send_مقصد_عقب_From_ISR( x_Timer_قطار, &x_پیغام, px_Higher_ترجیح_Task_Woken );
            }

            traceTIMER_COMMAND_SEND( xTimer, xCommandID, x_Optional_قدر, xReturn );
        }
        ورنہ
        {
            mtCOVERAGE_TEST_MARKER();
        }

        traceRETURN_xTimerGenericCommandFromISR( xReturn );

        واپس xReturn;
    }
/*-----------------------------------------------------------*/

    TaskHandle_t xTimerGetTimerDaemonTaskHandle( خالی )
    {
        traceENTER_xTimerGetTimerDaemonTaskHandle();

        /* If xTimerGetTimerDaemonTaskHandle() is called before the scheduler has been
         * started, then xTimerTaskHandle will be NULL. */
        configASSERT( ( xTimerTaskHandle != تہی ) );

        traceRETURN_xTimerGetTimerDaemonTaskHandle( xTimerTaskHandle );

        واپس xTimerTaskHandle;
    }
/*-----------------------------------------------------------*/

    TickType_t xTimerGetPeriod( TimerHandle_t xTimer )
    {
        Timer_t * pxTimer = xTimer;

        traceENTER_xTimerGetPeriod( xTimer );

        configASSERT( xTimer );

        traceRETURN_xTimerGetPeriod( pxTimer->x_Timer_Period_اندر_Ticks );

        واپس pxTimer->x_Timer_Period_اندر_Ticks;
    }
/*-----------------------------------------------------------*/

    خالی v_Timer_سیٹ_Reload_منوال( TimerHandle_t xTimer,
                              ثابت بنیاد_Type_t xAutoReload )
    {
        Timer_t * pxTimer = xTimer;

        trace_ENTER_v_Timer_سیٹ_Reload_منوال( xTimer, xAutoReload );

        configASSERT( xTimer );
        taskENTER_CRITICAL();
        {
            اگر( xAutoReload != pdFALSE )
            {
                pxTimer->uc_حالت |= ( uint8_t ) tmr_حالت_IS_AUTORELOAD;
            }
            ورنہ
            {
                pxTimer->uc_حالت &= ( ( uint8_t ) ~tmr_حالت_IS_AUTORELOAD );
            }
        }
        taskEXIT_CRITICAL();

        trace_RETURN_v_Timer_سیٹ_Reload_منوال();
    }
/*-----------------------------------------------------------*/

    بنیاد_Type_t x_Timer_Get_Reload_منوال( TimerHandle_t xTimer )
    {
        Timer_t * pxTimer = xTimer;
        بنیاد_Type_t xReturn;

        trace_ENTER_x_Timer_Get_Reload_منوال( xTimer );

        configASSERT( xTimer );
        taskENTER_CRITICAL();
        {
            اگر( ( pxTimer->uc_حالت & tmr_حالت_IS_AUTORELOAD ) == 0U )
            {
                /* Not an auto-reload timer. */
                xReturn = pdFALSE;
            }
            ورنہ
            {
                /* Is an auto-reload timer. */
                xReturn = pdTRUE;
            }
        }
        taskEXIT_CRITICAL();

        trace_RETURN_x_Timer_Get_Reload_منوال( xReturn );

        واپس xReturn;
    }

    UBaseType_t ux_Timer_Get_Reload_منوال( TimerHandle_t xTimer )
    {
        UBaseType_t uxReturn;

        trace_ENTER_ux_Timer_Get_Reload_منوال( xTimer );

        uxReturn = ( UBaseType_t ) x_Timer_Get_Reload_منوال( xTimer );

        trace_RETURN_ux_Timer_Get_Reload_منوال( uxReturn );

        واپس uxReturn;
    }
/*-----------------------------------------------------------*/

    TickType_t x_Timer_Get_Expiry_وقت( TimerHandle_t xTimer )
    {
        Timer_t * pxTimer = xTimer;
        TickType_t xReturn;

        trace_ENTER_x_Timer_Get_Expiry_وقت( xTimer );

        configASSERT( xTimer );
        xReturn = list_GET_LIST_شے_قدر( &( pxTimer->x_Timer_List_شے ) );

        trace_RETURN_x_Timer_Get_Expiry_وقت( xReturn );

        واپس xReturn;
    }
/*-----------------------------------------------------------*/

    #اگر ( config_SUPPORT_STATIC_تخصیص == 1 )
        بنیاد_Type_t x_Timer_Get_Static_بفر( TimerHandle_t xTimer,
                                          StaticTimer_t ** ppx_Timer_بفر )
        {
            بنیاد_Type_t xReturn;
            Timer_t * pxTimer = xTimer;

            trace_ENTER_x_Timer_Get_Static_بفر( xTimer, ppx_Timer_بفر );

            configASSERT( ppx_Timer_بفر != تہی );

            اگر( ( pxTimer->uc_حالت & tmr_حالت_IS_STATICALLY_ALLOCATED ) != 0U )
            {
                /* MISRA Ref 11.3.1 [Misaligned access] */
                /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
                /* coverity[misra_c_2012_rule_11_3_violation] */
                *ppx_Timer_بفر = ( StaticTimer_t * ) pxTimer;
                xReturn = pdTRUE;
            }
            ورنہ
            {
                xReturn = pdFALSE;
            }

            trace_RETURN_x_Timer_Get_Static_بفر( xReturn );

            واپس xReturn;
        }
    #اگر_ختم /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

    ثابت حرف * pcTimerGetName( TimerHandle_t xTimer )
    {
        Timer_t * pxTimer = xTimer;

        traceENTER_pcTimerGetName( xTimer );

        configASSERT( xTimer );

        traceRETURN_pcTimerGetName( pxTimer->pcTimerName );

        واپس pxTimer->pcTimerName;
    }
/*-----------------------------------------------------------*/

    ساکن خالی prvReloadTimer( Timer_t * ثابت pxTimer,
                                TickType_t x_Expired_وقت,
                                ثابت TickType_t x_وقت_Now )
    {
        /* Insert the timer into the appropriate list for the next expiry time.
         * If the next expiry time has already passed, advance the expiry time,
         * call the callback function, and try again. */
        جب( prv_داخل_کرو_Timer_اندر_فعال_List( pxTimer, ( x_Expired_وقت + pxTimer->x_Timer_Period_اندر_Ticks ), x_وقت_Now, x_Expired_وقت ) != pdFALSE )
        {
            /* Advance the expiry time. */
            x_Expired_وقت += pxTimer->x_Timer_Period_اندر_Ticks;

            /* Call the timer callback. */
            traceTIMER_EXPIRED( pxTimer );
            pxTimer->pxCallbackFunction( ( TimerHandle_t ) pxTimer );
        }
    }
/*-----------------------------------------------------------*/

    ساکن خالی prvProcessExpiredTimer( ثابت TickType_t x_اگلا_Expire_وقت,
                                        ثابت TickType_t x_وقت_Now )
    {
        /* MISRA Ref 11.5.3 [Void pointer assignment] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
        /* coverity[misra_c_2012_rule_11_5_violation] */
        Timer_t * ثابت pxTimer = ( Timer_t * ) list_GET_OWNER_کا_سر_اندراج( pxCurrentTimerList );

        /* Remove the timer from the list of active timers.  A check has already
         * been performed to ensure the list is not empty. */

        ( خالی ) uxListRemove( &( pxTimer->x_Timer_List_شے ) );

        /* If the timer is an auto-reload timer then calculate the next
         * expiry time and re-insert the timer in the list of active timers. */
        اگر( ( pxTimer->uc_حالت & tmr_حالت_IS_AUTORELOAD ) != 0U )
        {
            prvReloadTimer( pxTimer, x_اگلا_Expire_وقت, x_وقت_Now );
        }
        ورنہ
        {
            pxTimer->uc_حالت &= ( ( uint8_t ) ~tmr_حالت_IS_فعال );
        }

        /* Call the timer callback. */
        traceTIMER_EXPIRED( pxTimer );
        pxTimer->pxCallbackFunction( ( TimerHandle_t ) pxTimer );
    }
/*-----------------------------------------------------------*/

    ساکن پورٹ_TASK_FUNCTION( prvTimerTask, pvParameters )
    {
        TickType_t x_اگلا_Expire_وقت;
        بنیاد_Type_t x_List_Was_خالی_خانے;

        /* Just to avoid compiler warnings. */
        ( خالی ) pvParameters;

        #اگر ( configUSE_DAEMON_TASK_STARTUP_HOOK == 1 )
        {
            /* Allow the application writer to execute some code in the context of
             * this task at the point the task starts executing.  This is useful if the
             * application includes initialisation code that would benefit from
             * executing after the scheduler has been started. */
            vApplicationDaemonTaskStartupHook();
        }
        #اگر_ختم /* configUSE_DAEMON_TASK_STARTUP_HOOK */

        برائے( ; configCONTROL_INFINITE_LOOP(); )
        {
            /* Query the timers list to see if it contains any timers, and if so,
             * obtain the time at which the next timer will expire. */
            x_اگلا_Expire_وقت = prv_Get_اگلا_Expire_وقت( &x_List_Was_خالی_خانے );

            /* If a timer has expired, process it.  Otherwise, block this task
             * until either a timer does expire, or a command is received. */
            prv_Process_Timer_Or_بلاک_Task( x_اگلا_Expire_وقت, x_List_Was_خالی_خانے );

            /* Empty the command queue. */
            prvProcessReceivedCommands();
        }
    }
/*-----------------------------------------------------------*/

    ساکن خالی prv_Process_Timer_Or_بلاک_Task( ثابت TickType_t x_اگلا_Expire_وقت,
                                            بنیاد_Type_t x_List_Was_خالی_خانے )
    {
        TickType_t x_وقت_Now;
        بنیاد_Type_t xTimerListsWereSwitched;

        vTaskSuspendAll();
        {
            /* Obtain the time now to make an assessment as to whether the timer
             * has expired or not.  If obtaining the time causes the lists to switch
             * then don't process this timer as any timers that remained in the list
             * when the lists were switched will have been processed within the
             * prvSampleTimeNow() function. */
            x_وقت_Now = prv_Sample_وقت_Now( &xTimerListsWereSwitched );

            اگر( xTimerListsWereSwitched == pdFALSE )
            {
                /* The tick count has not overflowed, has the timer expired? */
                اگر( ( x_List_Was_خالی_خانے == pdFALSE ) && ( x_اگلا_Expire_وقت <= x_وقت_Now ) )
                {
                    ( خالی ) xTaskResumeAll();
                    prvProcessExpiredTimer( x_اگلا_Expire_وقت, x_وقت_Now );
                }
                ورنہ
                {
                    /* The tick count has not overflowed, and the next expire
                     * time has not been reached yet.  This task should therefore
                     * block to wait for the next expire time or a command to be
                     * received - whichever comes first.  The following line cannot
                     * be reached unless xNextExpireTime > xTimeNow, except in the
                     * case when the current timer list is empty. */
                    اگر( x_List_Was_خالی_خانے != pdFALSE )
                    {
                        /* The current timer list is empty - is the overflow list
                         * also empty? */
                        x_List_Was_خالی_خانے = list_LIST_IS_خالی_خانے( px_حد_سے_تجاوز_Timer_List );
                    }

                    v_قطار_Wait_For_پیغام_Restricted( x_Timer_قطار, ( x_اگلا_Expire_وقت - x_وقت_Now ), x_List_Was_خالی_خانے );

                    اگر( xTaskResumeAll() == pdFALSE )
                    {
                        /* Yield to wait for either a command to arrive, or the
                         * block time to expire.  If a command arrived between the
                         * critical section being exited and this yield then the yield
                         * will not cause the task to block. */
                        taskYIELD_WITHIN_API();
                    }
                    ورنہ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
            }
            ورنہ
            {
                ( خالی ) xTaskResumeAll();
            }
        }
    }
/*-----------------------------------------------------------*/

    ساکن TickType_t prv_Get_اگلا_Expire_وقت( بنیاد_Type_t * ثابت px_List_Was_خالی_خانے )
    {
        TickType_t x_اگلا_Expire_وقت;

        /* Timers are listed in expiry time order, with the head of the list
         * referencing the task that will expire first.  Obtain the time at which
         * the timer with the nearest expiry time will expire.  If there are no
         * active timers then just set the next expire time to 0.  That will cause
         * this task to unblock when the tick count overflows, at which point the
         * timer lists will be switched and the next expiry time can be
         * re-assessed.  */
        *px_List_Was_خالی_خانے = list_LIST_IS_خالی_خانے( pxCurrentTimerList );

        اگر( *px_List_Was_خالی_خانے == pdFALSE )
        {
            x_اگلا_Expire_وقت = list_GET_شے_قدر_کا_سر_اندراج( pxCurrentTimerList );
        }
        ورنہ
        {
            /* Ensure the task unblocks when the tick count rolls over. */
            x_اگلا_Expire_وقت = ( TickType_t ) 0U;
        }

        واپس x_اگلا_Expire_وقت;
    }
/*-----------------------------------------------------------*/

    ساکن TickType_t prv_Sample_وقت_Now( بنیاد_Type_t * ثابت pxTimerListsWereSwitched )
    {
        TickType_t x_وقت_Now;
        PRIVILEGED_ڈیٹا ساکن TickType_t x_آخری_وقت = ( TickType_t ) 0U;

        x_وقت_Now = x_Task_Get_Tick_گنتی();

        اگر( x_وقت_Now < x_آخری_وقت )
        {
            prvSwitchTimerLists();
            *pxTimerListsWereSwitched = pdTRUE;
        }
        ورنہ
        {
            *pxTimerListsWereSwitched = pdFALSE;
        }

        x_آخری_وقت = x_وقت_Now;

        واپس x_وقت_Now;
    }
/*-----------------------------------------------------------*/

    ساکن بنیاد_Type_t prv_داخل_کرو_Timer_اندر_فعال_List( Timer_t * ثابت pxTimer,
                                                  ثابت TickType_t x_اگلا_Expiry_وقت,
                                                  ثابت TickType_t x_وقت_Now,
                                                  ثابت TickType_t x_Command_وقت )
    {
        بنیاد_Type_t xProcessTimerNow = pdFALSE;

        list_سیٹ_LIST_شے_قدر( &( pxTimer->x_Timer_List_شے ), x_اگلا_Expiry_وقت );
        list_سیٹ_LIST_شے_OWNER( &( pxTimer->x_Timer_List_شے ), pxTimer );

        اگر( x_اگلا_Expiry_وقت <= x_وقت_Now )
        {
            /* Has the expiry time elapsed between the command to start/reset a
             * timer was issued, and the time the command was processed? */
            اگر( ( ( TickType_t ) ( x_وقت_Now - x_Command_وقت ) ) >= pxTimer->x_Timer_Period_اندر_Ticks )
            {
                /* The time between a command being issued and the command being
                 * processed actually exceeds the timers period.  */
                xProcessTimerNow = pdTRUE;
            }
            ورنہ
            {
                v_List_داخل_کرو( px_حد_سے_تجاوز_Timer_List, &( pxTimer->x_Timer_List_شے ) );
            }
        }
        ورنہ
        {
            اگر( ( x_وقت_Now < x_Command_وقت ) && ( x_اگلا_Expiry_وقت >= x_Command_وقت ) )
            {
                /* If, since the command was issued, the tick count has overflowed
                 * but the expiry time has not, then the timer must have already passed
                 * its expiry time and should be processed immediately. */
                xProcessTimerNow = pdTRUE;
            }
            ورنہ
            {
                v_List_داخل_کرو( pxCurrentTimerList, &( pxTimer->x_Timer_List_شے ) );
            }
        }

        واپس xProcessTimerNow;
    }
/*-----------------------------------------------------------*/

    ساکن خالی prvProcessReceivedCommands( خالی )
    {
        Daemon_Task_پیغام_t x_پیغام = { 0 };
        Timer_t * pxTimer;
        بنیاد_Type_t xTimerListsWereSwitched;
        TickType_t x_وقت_Now;

        جب( x_قطار_Receive( x_Timer_قطار, &x_پیغام, tmrNO_DELAY ) != pdFAIL )
        {
            #اگر ( INCLUDE_xTimerPendFunctionCall == 1 )
            {
                /* Negative commands are pended function calls rather than timer
                 * commands. */
                اگر( x_پیغام.x_پیغام_ID < ( بنیاد_Type_t ) 0 )
                {
                    ثابت CallbackParameters_t * ثابت pxCallback = &( x_پیغام.u.xCallbackParameters );

                    /* The timer uses the xCallbackParameters member to request a
                     * callback be executed.  Check the callback is not NULL. */
                    configASSERT( pxCallback );

                    /* Call the function. */
                    pxCallback->pxCallbackFunction( pxCallback->pvParameter1, pxCallback->ulParameter2 );
                }
                ورنہ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            #اگر_ختم /* INCLUDE_xTimerPendFunctionCall */

            /* Commands that are positive are timer commands rather than pended
             * function calls. */
            اگر( x_پیغام.x_پیغام_ID >= ( بنیاد_Type_t ) 0 )
            {
                /* The messages uses the xTimerParameters member to work on a
                 * software timer. */
                pxTimer = x_پیغام.u.xTimerParameters.pxTimer;

                اگر( listIS_CONTAINED_WITHIN( تہی, &( pxTimer->x_Timer_List_شے ) ) == pdFALSE )
                {
                    /* The timer is in a list, remove it. */
                    ( خالی ) uxListRemove( &( pxTimer->x_Timer_List_شے ) );
                }
                ورنہ
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                traceTIMER_COMMAND_RECEIVED( pxTimer, x_پیغام.x_پیغام_ID, x_پیغام.u.xTimerParameters.x_پیغام_قدر );

                /* In this case the xTimerListsWereSwitched parameter is not used, but
                 *  it must be present in the function call.  prvSampleTimeNow() must be
                 *  called after the message is received from xTimerQueue so there is no
                 *  possibility of a higher priority task adding a message to the message
                 *  queue with a time that is ahead of the timer daemon task (because it
                 *  pre-empted the timer daemon task after the xTimeNow value was set). */
                x_وقت_Now = prv_Sample_وقت_Now( &xTimerListsWereSwitched );

                انتخاب( x_پیغام.x_پیغام_ID )
                {
                    حالت tmr_COMMAND_آغاز:
                    حالت tmr_COMMAND_آغاز_FROM_ISR:
                    حالت tmrCOMMAND_RESET:
                    حالت tmrCOMMAND_RESET_FROM_ISR:
                        /* Start or restart a timer. */
                        pxTimer->uc_حالت |= ( uint8_t ) tmr_حالت_IS_فعال;

                        اگر( prv_داخل_کرو_Timer_اندر_فعال_List( pxTimer, x_پیغام.u.xTimerParameters.x_پیغام_قدر + pxTimer->x_Timer_Period_اندر_Ticks, x_وقت_Now, x_پیغام.u.xTimerParameters.x_پیغام_قدر ) != pdFALSE )
                        {
                            /* The timer expired before it was added to the active
                             * timer list.  Process it now. */
                            اگر( ( pxTimer->uc_حالت & tmr_حالت_IS_AUTORELOAD ) != 0U )
                            {
                                prvReloadTimer( pxTimer, x_پیغام.u.xTimerParameters.x_پیغام_قدر + pxTimer->x_Timer_Period_اندر_Ticks, x_وقت_Now );
                            }
                            ورنہ
                            {
                                pxTimer->uc_حالت &= ( ( uint8_t ) ~tmr_حالت_IS_فعال );
                            }

                            /* Call the timer callback. */
                            traceTIMER_EXPIRED( pxTimer );
                            pxTimer->pxCallbackFunction( ( TimerHandle_t ) pxTimer );
                        }
                        ورنہ
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }

                        توڑو;

                    حالت tmrCOMMAND_STOP:
                    حالت tmrCOMMAND_STOP_FROM_ISR:
                        /* The timer has already been removed from the active list. */
                        pxTimer->uc_حالت &= ( ( uint8_t ) ~tmr_حالت_IS_فعال );
                        توڑو;

                    حالت tmrCOMMAND_CHANGE_PERIOD:
                    حالت tmrCOMMAND_CHANGE_PERIOD_FROM_ISR:
                        pxTimer->uc_حالت |= ( uint8_t ) tmr_حالت_IS_فعال;
                        pxTimer->x_Timer_Period_اندر_Ticks = x_پیغام.u.xTimerParameters.x_پیغام_قدر;
                        configASSERT( ( pxTimer->x_Timer_Period_اندر_Ticks > 0 ) );

                        /* The new period does not really have a reference, and can
                         * be longer or shorter than the old one.  The command time is
                         * therefore set to the current time, and as the period cannot
                         * be zero the next expiry time can only be in the future,
                         * meaning (unlike for the xTimerStart() case above) there is
                         * no fail case that needs to be handled here. */
                        ( خالی ) prv_داخل_کرو_Timer_اندر_فعال_List( pxTimer, ( x_وقت_Now + pxTimer->x_Timer_Period_اندر_Ticks ), x_وقت_Now, x_وقت_Now );
                        توڑو;

                    حالت tmrCOMMAND_DELETE:
                        #اگر ( config_SUPPORT_DYNAMIC_تخصیص == 1 )
                        {
                            /* The timer has already been removed from the active list,
                             * just free up the memory if the memory was dynamically
                             * allocated. */
                            اگر( ( pxTimer->uc_حالت & tmr_حالت_IS_STATICALLY_ALLOCATED ) == ( uint8_t ) 0 )
                            {
                                v_پورٹ_Free( pxTimer );
                            }
                            ورنہ
                            {
                                pxTimer->uc_حالت &= ( ( uint8_t ) ~tmr_حالت_IS_فعال );
                            }
                        }
                        #ورنہ /* if ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) */
                        {
                            /* If dynamic allocation is not enabled, the memory
                             * could not have been dynamically allocated. So there is
                             * no need to free the memory - just mark the timer as
                             * "not active". */
                            pxTimer->uc_حالت &= ( ( uint8_t ) ~tmr_حالت_IS_فعال );
                        }
                        #اگر_ختم /* configSUPPORT_DYNAMIC_ALLOCATION */
                        توڑو;

                    طےشدہ:
                        /* Don't expect to get here. */
                        توڑو;
                }
            }
        }
    }
/*-----------------------------------------------------------*/

    ساکن خالی prvSwitchTimerLists( خالی )
    {
        TickType_t x_اگلا_Expire_وقت;
        List_t * px_عارضی;

        /* The tick count has overflowed.  The timer lists must be switched.
         * If there are any timers still referenced from the current timer list
         * then they must have expired and should be processed before the lists
         * are switched. */
        جب( list_LIST_IS_خالی_خانے( pxCurrentTimerList ) == pdFALSE )
        {
            x_اگلا_Expire_وقت = list_GET_شے_قدر_کا_سر_اندراج( pxCurrentTimerList );

            /* Process the expired timer.  For auto-reload timers, be careful to
             * process only expirations that occur on the current list.  Further
             * expirations must wait until after the lists are switched. */
            prvProcessExpiredTimer( x_اگلا_Expire_وقت, tmr_MAX_وقت_BEFORE_حد_سے_تجاوز );
        }

        px_عارضی = pxCurrentTimerList;
        pxCurrentTimerList = px_حد_سے_تجاوز_Timer_List;
        px_حد_سے_تجاوز_Timer_List = px_عارضی;
    }
/*-----------------------------------------------------------*/

    ساکن خالی prv_Check_For_Valid_List_And_قطار( خالی )
    {
        /* Check that the list from which active timers are referenced, and the
         * queue used to communicate with the timer service, have been
         * initialised. */
        taskENTER_CRITICAL();
        {
            اگر( x_Timer_قطار == تہی )
            {
                vListInitialise( &x_فعال_Timer_List1 );
                vListInitialise( &x_فعال_Timer_List2 );
                pxCurrentTimerList = &x_فعال_Timer_List1;
                px_حد_سے_تجاوز_Timer_List = &x_فعال_Timer_List2;

                #اگر ( config_SUPPORT_STATIC_تخصیص == 1 )
                {
                    /* The timer queue is allocated statically in case
                     * configSUPPORT_DYNAMIC_ALLOCATION is 0. */
                    PRIVILEGED_ڈیٹا ساکن Static_قطار_t x_Static_Timer_قطار;
                    PRIVILEGED_ڈیٹا ساکن uint8_t uc_Static_Timer_قطار_Storage[ ( size_t ) config_TIMER_قطار_لمبائی * حجم( Daemon_Task_پیغام_t ) ];

                    x_Timer_قطار = x_قطار_Create_Static( ( UBaseType_t ) config_TIMER_قطار_لمبائی, ( UBaseType_t ) حجم( Daemon_Task_پیغام_t ), &( uc_Static_Timer_قطار_Storage[ 0 ] ), &x_Static_Timer_قطار );
                }
                #ورنہ
                {
                    x_Timer_قطار = x_قطار_Create( ( UBaseType_t ) config_TIMER_قطار_لمبائی, ( UBaseType_t ) حجم( Daemon_Task_پیغام_t ) );
                }
                #اگر_ختم /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */

                #اگر ( config_قطار_REGISTRY_سائز > 0 )
                {
                    اگر( x_Timer_قطار != تہی )
                    {
                        v_قطار_جوڑو_مقصد_Registry( x_Timer_قطار, "TmrQ" );
                    }
                    ورنہ
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #اگر_ختم /* configQUEUE_REGISTRY_SIZE */
            }
            ورنہ
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        taskEXIT_CRITICAL();
    }
/*-----------------------------------------------------------*/

    بنیاد_Type_t x_Timer_Is_Timer_فعال( TimerHandle_t xTimer )
    {
        بنیاد_Type_t xReturn;
        Timer_t * pxTimer = xTimer;

        trace_ENTER_x_Timer_Is_Timer_فعال( xTimer );

        configASSERT( xTimer );

        /* Is the timer in the list of active timers? */
        taskENTER_CRITICAL();
        {
            اگر( ( pxTimer->uc_حالت & tmr_حالت_IS_فعال ) == 0U )
            {
                xReturn = pdFALSE;
            }
            ورنہ
            {
                xReturn = pdTRUE;
            }
        }
        taskEXIT_CRITICAL();

        trace_RETURN_x_Timer_Is_Timer_فعال( xReturn );

        واپس xReturn;
    }
/*-----------------------------------------------------------*/

    خالی * pvTimerGetTimerID( ثابت TimerHandle_t xTimer )
    {
        Timer_t * ثابت pxTimer = xTimer;
        خالی * pvReturn;

        traceENTER_pvTimerGetTimerID( xTimer );

        configASSERT( xTimer );

        taskENTER_CRITICAL();
        {
            pvReturn = pxTimer->pvTimerID;
        }
        taskEXIT_CRITICAL();

        traceRETURN_pvTimerGetTimerID( pvReturn );

        واپس pvReturn;
    }
/*-----------------------------------------------------------*/

    خالی v_Timer_سیٹ_Timer_ID( TimerHandle_t xTimer,
                           خالی * pvNewID )
    {
        Timer_t * ثابت pxTimer = xTimer;

        trace_ENTER_v_Timer_سیٹ_Timer_ID( xTimer, pvNewID );

        configASSERT( xTimer );

        taskENTER_CRITICAL();
        {
            pxTimer->pvTimerID = pvNewID;
        }
        taskEXIT_CRITICAL();

        trace_RETURN_v_Timer_سیٹ_Timer_ID();
    }
/*-----------------------------------------------------------*/

    #اگر ( INCLUDE_xTimerPendFunctionCall == 1 )

        بنیاد_Type_t xTimerPendFunctionCallFromISR( PendedFunction_t x_Function_مقصد_Pend,
                                                  خالی * pvParameter1,
                                                  uint32_t ulParameter2,
                                                  بنیاد_Type_t * px_Higher_ترجیح_Task_Woken )
        {
            Daemon_Task_پیغام_t x_پیغام;
            بنیاد_Type_t xReturn;

            traceENTER_xTimerPendFunctionCallFromISR( x_Function_مقصد_Pend, pvParameter1, ulParameter2, px_Higher_ترجیح_Task_Woken );

            /* Complete the message with the function parameters and post it to the
             * daemon task. */
            x_پیغام.x_پیغام_ID = tmrCOMMAND_EXECUTE_CALLBACK_FROM_ISR;
            x_پیغام.u.xCallbackParameters.pxCallbackFunction = x_Function_مقصد_Pend;
            x_پیغام.u.xCallbackParameters.pvParameter1 = pvParameter1;
            x_پیغام.u.xCallbackParameters.ulParameter2 = ulParameter2;

            xReturn = x_قطار_Send_From_ISR( x_Timer_قطار, &x_پیغام, px_Higher_ترجیح_Task_Woken );

            tracePEND_FUNC_CALL_FROM_ISR( x_Function_مقصد_Pend, pvParameter1, ulParameter2, xReturn );
            traceRETURN_xTimerPendFunctionCallFromISR( xReturn );

            واپس xReturn;
        }

    #اگر_ختم /* INCLUDE_xTimerPendFunctionCall */
/*-----------------------------------------------------------*/

    #اگر ( INCLUDE_xTimerPendFunctionCall == 1 )

        بنیاد_Type_t xTimerPendFunctionCall( PendedFunction_t x_Function_مقصد_Pend,
                                           خالی * pvParameter1,
                                           uint32_t ulParameter2,
                                           TickType_t x_Ticks_مقصد_Wait )
        {
            Daemon_Task_پیغام_t x_پیغام;
            بنیاد_Type_t xReturn;

            traceENTER_xTimerPendFunctionCall( x_Function_مقصد_Pend, pvParameter1, ulParameter2, x_Ticks_مقصد_Wait );

            /* This function can only be called after a timer has been created or
             * after the scheduler has been started because, until then, the timer
             * queue does not exist. */
            configASSERT( x_Timer_قطار );

            /* Complete the message with the function parameters and post it to the
             * daemon task. */
            x_پیغام.x_پیغام_ID = tmrCOMMAND_EXECUTE_CALLBACK;
            x_پیغام.u.xCallbackParameters.pxCallbackFunction = x_Function_مقصد_Pend;
            x_پیغام.u.xCallbackParameters.pvParameter1 = pvParameter1;
            x_پیغام.u.xCallbackParameters.ulParameter2 = ulParameter2;

            xReturn = x_قطار_Send_مقصد_عقب( x_Timer_قطار, &x_پیغام, x_Ticks_مقصد_Wait );

            tracePEND_FUNC_CALL( x_Function_مقصد_Pend, pvParameter1, ulParameter2, xReturn );
            traceRETURN_xTimerPendFunctionCall( xReturn );

            واپس xReturn;
        }

    #اگر_ختم /* INCLUDE_xTimerPendFunctionCall */
/*-----------------------------------------------------------*/

    #اگر ( configUSE_TRACE_FACILITY == 1 )

        UBaseType_t ux_Timer_Get_Timer_نمبر( TimerHandle_t xTimer )
        {
            trace_ENTER_ux_Timer_Get_Timer_نمبر( xTimer );

            trace_RETURN_ux_Timer_Get_Timer_نمبر( ( ( Timer_t * ) xTimer )->ux_Timer_نمبر );

            واپس ( ( Timer_t * ) xTimer )->ux_Timer_نمبر;
        }

    #اگر_ختم /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

    #اگر ( configUSE_TRACE_FACILITY == 1 )

        خالی v_Timer_سیٹ_Timer_نمبر( TimerHandle_t xTimer,
                                   UBaseType_t ux_Timer_نمبر )
        {
            trace_ENTER_v_Timer_سیٹ_Timer_نمبر( xTimer, ux_Timer_نمبر );

            ( ( Timer_t * ) xTimer )->ux_Timer_نمبر = ux_Timer_نمبر;

            trace_RETURN_v_Timer_سیٹ_Timer_نمبر();
        }

    #اگر_ختم /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

/*
 * Reset the state in this file. This state is normally initialized at start up.
 * This function must be called by the application before restarting the
 * scheduler.
 */
    خالی vTimerResetState( خالی )
    {
        x_Timer_قطار = تہی;
        xTimerTaskHandle = تہی;
    }
/*-----------------------------------------------------------*/

/* This entire source file will be skipped if the application is not configured
 * to include software timer functionality.  If you want to include software timer
 * functionality then ensure configUSE_TIMERS is set to 1 in FreeRTOSConfig.h. */
#اگر_ختم /* configUSE_TIMERS == 1 */
