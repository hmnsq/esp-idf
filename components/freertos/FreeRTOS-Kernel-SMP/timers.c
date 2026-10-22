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
#অন্তর্ভাব <stdlib.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#define MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#অন্তর্ভাব "FreeRTOS.h"
#অন্তর্ভাব "task.h"
#অন্তর্ভাব "queue.h"
#অন্তর্ভাব "timers.h"

#যদি ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 0 )
    #error configUSE_TIMERS must be set to 1 to make the xTimerPendFunctionCall() function available.
#endif

/* The MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#undef MPU_WRAPPERS_INCLUDED_FROM_API_FILE


/* This entire source file will be skipped if the application is not configured
 * to include software timer functionality.  This #if is closed at the very bottom
 * of this file.  If you want to include software timer functionality then ensure
 * configUSE_TIMERS is set to 1 in FreeRTOSConfig.h. */
#যদি ( configUSE_TIMERS == 1 )

/* Misc definitions. */
    #define tmrNO_DELAY                    ( ( TickType_t ) 0U )
    #define tmrMAX_TIME_BEFORE_OVERFLOW    ( ( TickType_t ) -1 )

/* The name assigned to the timer service task. This can be overridden by
 * defining configTIMER_SERVICE_TASK_NAME in FreeRTOSConfig.h. */
    #ifndef configTIMER_SERVICE_TASK_NAME
        #define configTIMER_SERVICE_TASK_NAME    "Tmr Svc"
    #endif

    #যদি ( ( config_সংখ্যা_OF_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )

/* The core affinity assigned to the timer service task on SMP systems.
 * This can be overridden by defining configTIMER_SERVICE_TASK_CORE_AFFINITY in FreeRTOSConfig.h. */
        #ifndef configTIMER_SERVICE_TASK_CORE_AFFINITY
            #define configTIMER_SERVICE_TASK_CORE_AFFINITY    tskNO_AFFINITY
        #endif
    #endif /* #if ( ( configNUMBER_OF_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */

/* Bit definitions used in the ucStatus member of a timer structure. */
    #define tmrSTATUS_IS_ACTIVE                  ( 0x01U )
    #define tmrSTATUS_IS_STATICALLY_ALLOCATED    ( 0x02U )
    #define tmrSTATUS_IS_AUTORELOAD              ( 0x04U )

/* The definition of the timers themselves. */
    প্রকারলেখ কাঠামো tmrTimerControl                                               /* The old naming convention is used to prevent breaking kernel aware debuggers. */
    {
        ধ্রুবক অক্ষর * pcTimerName;                                                /**< Text name.  This is not used by the kernel, it is included simply to make debugging easier. */
        ListItem_t xTimerListItem;                                               /**< Standard linked list item as used by all kernel features for event management. */
        TickType_t xTimerPeriodInTicks;                                          /**< How quickly and often the timer expires. */
        শূন্য * pvTimerID;                                                        /**< An ID to identify the timer.  This allows the timer to be identified when the same callback is used for multiple timers. */
        portTIMER_CALLBACK_ATTRIBUTE TimerCallbackFunction_t pxCallbackFunction; /**< The function that will be called when the timer expires. */
        #যদি ( configUSE_TRACE_FACILITY == 1 )
            UBaseType_t ux_Timer_সংখ্যা;                                           /**< An ID assigned by trace tools such as FreeRTOS+Trace */
        #endif
        uint8_t ucStatus;                                                        /**< Holds bits to say if the timer was statically allocated or not, and if it is active or not. */
    } xTIMER;

/* The old xTIMER name is maintained above then typedefed to the new Timer_t
 * name below to enable the use of older kernel aware debuggers. */
    প্রকারলেখ xTIMER Timer_t;

/* The definition of messages that can be sent and received on the timer queue.
 * Two types of message can be queued - messages that manipulate a software timer,
 * and messages that request the execution of a non-timer related callback.  The
 * two message types are defined in two separate structures, xTimerParametersType
 * and xCallbackParametersType respectively. */
    প্রকারলেখ কাঠামো tmrTimerParameters
    {
        TickType_t x_Message_মান; /**< An optional value used by a subset of commands, for example, when changing the period of a timer. */
        Timer_t * pxTimer;        /**< The timer to which the command will be applied. */
    } TimerParameter_t;


    প্রকারলেখ কাঠামো tmrCallbackParameters
    {
        portTIMER_CALLBACK_ATTRIBUTE
        PendedFunction_t pxCallbackFunction; /* << The callback function to execute. */
        শূন্য * pvParameter1;                 /* << The value that will be used as the callback functions first parameter. */
        uint32_t ulParameter2;               /* << The value that will be used as the callback functions second parameter. */
    } CallbackParameters_t;

/* The structure that contains the two message types, along with an identifier
 * that is used to determine which message type is valid. */
    প্রকারলেখ কাঠামো tmrTimerQueueMessage
    {
        BaseType_t xMessageID; /**< The command being sent to the timer service task. */
        সংঘ
        {
            TimerParameter_t xTimerParameters;

            /* Don't include xCallbackParameters if it is not going to be used as
             * it makes the structure (and therefore the timer queue) larger. */
            #যদি ( INCLUDE_xTimerPendFunctionCall == 1 )
                CallbackParameters_t xCallbackParameters;
            #endif /* INCLUDE_xTimerPendFunctionCall */
        } u;
    } DaemonTaskMessage_t;

/* The list in which active timers are stored.  Timers are referenced in expire
 * time order, with the nearest expiry time at the front of the list.  Only the
 * timer service task is allowed to access these lists.
 * xActiveTimerList1 and xActiveTimerList2 could be at function scope but that
 * breaks some kernel aware debuggers, and debuggers that reply on removing the
 * static qualifier. */
    PRIVILEGED_তথ্য স্থির List_t xActiveTimerList1;
    PRIVILEGED_তথ্য স্থির List_t xActiveTimerList2;
    PRIVILEGED_তথ্য স্থির List_t * pxCurrentTimerList;
    PRIVILEGED_তথ্য স্থির List_t * pxOverflowTimerList;

/* A queue that is used to send commands to the timer service task. */
    PRIVILEGED_তথ্য স্থির QueueHandle_t xTimerQueue = ফাঁকা;
    PRIVILEGED_তথ্য স্থির TaskHandle_t xTimerTaskHandle = ফাঁকা;

/*-----------------------------------------------------------*/

/*
 * Initialise the infrastructure used by the timer service task if it has not
 * been initialised already.
 */
    স্থির শূন্য prvCheckForValidListAndQueue( শূন্য ) PRIVILEGED_FUNCTION;

/*
 * The timer service task (daemon).  Timer functionality is controlled by this
 * task.  Other tasks communicate with the timer service task using the
 * xTimerQueue queue.
 */
    স্থির portTASK_FUNCTION_PROTO( prvTimerTask, pvParameters ) PRIVILEGED_FUNCTION;

/*
 * Called by the timer service task to interpret and process a command it
 * received on the timer queue.
 */
    স্থির শূন্য prvProcessReceivedCommands( শূন্য ) PRIVILEGED_FUNCTION;

/*
 * Insert the timer into either xActiveTimerList1, or xActiveTimerList2,
 * depending on if the expire time causes a timer counter overflow.
 */
    স্থির BaseType_t prv_ঢোকাও_Timer_In_Active_List( Timer_t * ধ্রুবক pxTimer,
                                                  ধ্রুবক TickType_t x_পরবর্তী_Expiry_Time,
                                                  ধ্রুবক TickType_t xTimeNow,
                                                  ধ্রুবক TickType_t xCommandTime ) PRIVILEGED_FUNCTION;

/*
 * Reload the specified auto-reload timer.  If the reloading is backlogged,
 * clear the backlog, calling the callback for each additional reload.  When
 * this function returns, the next expiry time is after xTimeNow.
 */
    স্থির শূন্য prvReloadTimer( Timer_t * ধ্রুবক pxTimer,
                                TickType_t xExpiredTime,
                                ধ্রুবক TickType_t xTimeNow ) PRIVILEGED_FUNCTION;

/*
 * An active timer has reached its expire time.  Reload the timer if it is an
 * auto-reload timer, then call its callback.
 */
    স্থির শূন্য prvProcessExpiredTimer( ধ্রুবক TickType_t x_পরবর্তী_Expire_Time,
                                        ধ্রুবক TickType_t xTimeNow ) PRIVILEGED_FUNCTION;

/*
 * The tick count has overflowed.  Switch the timer lists after ensuring the
 * current timer list does not still reference some timers.
 */
    স্থির শূন্য prvSwitchTimerLists( শূন্য ) PRIVILEGED_FUNCTION;

/*
 * Obtain the current tick count, setting *pxTimerListsWereSwitched to pdTRUE
 * if a tick count overflow occurred since prvSampleTimeNow() was last called.
 */
    স্থির TickType_t prvSampleTimeNow( BaseType_t * ধ্রুবক pxTimerListsWereSwitched ) PRIVILEGED_FUNCTION;

/*
 * If the timer list contains any active timers then return the expire time of
 * the timer that will expire first and set *pxListWasEmpty to false.  If the
 * timer list does not contain any timers then return 0 and set *pxListWasEmpty
 * to pdTRUE.
 */
    স্থির TickType_t prv_Get_পরবর্তী_Expire_Time( BaseType_t * ধ্রুবক pxListWasEmpty ) PRIVILEGED_FUNCTION;

/*
 * If a timer has expired, process it.  Otherwise, block the timer service task
 * until either a timer does expire or a command is received.
 */
    স্থির শূন্য prvProcessTimerOrBlockTask( ধ্রুবক TickType_t x_পরবর্তী_Expire_Time,
                                            BaseType_t xListWasEmpty ) PRIVILEGED_FUNCTION;

/*
 * Called after a Timer_t structure has been allocated either statically or
 * dynamically to fill in the structure's members.
 */
    স্থির শূন্য prvInitialiseNewTimer( ধ্রুবক অক্ষর * ধ্রুবক pcTimerName,
                                       ধ্রুবক TickType_t xTimerPeriodInTicks,
                                       ধ্রুবক BaseType_t xAutoReload,
                                       শূন্য * ধ্রুবক pvTimerID,
                                       TimerCallbackFunction_t pxCallbackFunction,
                                       Timer_t * pxNewTimer ) PRIVILEGED_FUNCTION;
/*-----------------------------------------------------------*/

    BaseType_t xTimerCreateTimerTask( শূন্য )
    {
        BaseType_t xReturn = pdFAIL;

        traceENTER_xTimerCreateTimerTask();

        /* This function is called when the scheduler is started if
         * configUSE_TIMERS is set to 1.  Check that the infrastructure used by the
         * timer service task has been created/initialised.  If timers have already
         * been created then the initialisation will already have been performed. */
        prvCheckForValidListAndQueue();

        যদি( xTimerQueue != ফাঁকা )
        {
            #যদি ( ( config_সংখ্যা_OF_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
            {
                #যদি ( configSUPPORT_STATIC_ALLOCATION == 1 )
                {
                    StaticTask_t * pxTimerTaskTCBBuffer = ফাঁকা;
                    স্তূপ_Type_t * px_Timer_Task_স্তূপ_বাফার = ফাঁকা;
                    config_স্তূপ_DEPTH_TYPE ux_Timer_Task_স্তূপ_আকার;

                    vApplicationGetTimerTaskMemory( &pxTimerTaskTCBBuffer, &px_Timer_Task_স্তূপ_বাফার, &ux_Timer_Task_স্তূপ_আকার );
                    xTimerTaskHandle = xTaskCreateStaticAffinitySet( prvTimerTask,
                                                                     configTIMER_SERVICE_TASK_NAME,
                                                                     ux_Timer_Task_স্তূপ_আকার,
                                                                     ফাঁকা,
                                                                     ( ( UBaseType_t ) configTIMER_TASK_PRIORITY ) | portPRIVILEGE_BIT,
                                                                     px_Timer_Task_স্তূপ_বাফার,
                                                                     pxTimerTaskTCBBuffer,
                                                                     configTIMER_SERVICE_TASK_CORE_AFFINITY );

                    যদি( xTimerTaskHandle != ফাঁকা )
                    {
                        xReturn = pdPASS;
                    }
                }
                #অন্যথা /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
                {
                    xReturn = xTaskCreateAffinitySet( prvTimerTask,
                                                      configTIMER_SERVICE_TASK_NAME,
                                                      config_TIMER_TASK_স্তূপ_DEPTH,
                                                      ফাঁকা,
                                                      ( ( UBaseType_t ) configTIMER_TASK_PRIORITY ) | portPRIVILEGE_BIT,
                                                      configTIMER_SERVICE_TASK_CORE_AFFINITY,
                                                      &xTimerTaskHandle );
                }
                #endif /* configSUPPORT_STATIC_ALLOCATION */
            }
            #অন্যথা /* #if ( ( configNUMBER_OF_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            {
                #যদি ( configSUPPORT_STATIC_ALLOCATION == 1 )
                {
                    StaticTask_t * pxTimerTaskTCBBuffer = ফাঁকা;
                    স্তূপ_Type_t * px_Timer_Task_স্তূপ_বাফার = ফাঁকা;
                    config_স্তূপ_DEPTH_TYPE ux_Timer_Task_স্তূপ_আকার;

                    vApplicationGetTimerTaskMemory( &pxTimerTaskTCBBuffer, &px_Timer_Task_স্তূপ_বাফার, &ux_Timer_Task_স্তূপ_আকার );
                    xTimerTaskHandle = xTaskCreateStatic( prvTimerTask,
                                                          configTIMER_SERVICE_TASK_NAME,
                                                          ux_Timer_Task_স্তূপ_আকার,
                                                          ফাঁকা,
                                                          ( ( UBaseType_t ) configTIMER_TASK_PRIORITY ) | portPRIVILEGE_BIT,
                                                          px_Timer_Task_স্তূপ_বাফার,
                                                          pxTimerTaskTCBBuffer );

                    যদি( xTimerTaskHandle != ফাঁকা )
                    {
                        xReturn = pdPASS;
                    }
                }
                #অন্যথা /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
                {
                    xReturn = xTaskCreate( prvTimerTask,
                                           configTIMER_SERVICE_TASK_NAME,
                                           config_TIMER_TASK_স্তূপ_DEPTH,
                                           ফাঁকা,
                                           ( ( UBaseType_t ) configTIMER_TASK_PRIORITY ) | portPRIVILEGE_BIT,
                                           &xTimerTaskHandle );
                }
                #endif /* configSUPPORT_STATIC_ALLOCATION */
            }
            #endif /* #if ( ( configNUMBER_OF_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
        }
        অন্যথা
        {
            mtCOVERAGE_TEST_MARKER();
        }

        configASSERT( xReturn );

        traceRETURN_xTimerCreateTimerTask( xReturn );

        ফেরত xReturn;
    }
/*-----------------------------------------------------------*/

    #যদি ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )

        TimerHandle_t xTimerCreate( ধ্রুবক অক্ষর * ধ্রুবক pcTimerName,
                                    ধ্রুবক TickType_t xTimerPeriodInTicks,
                                    ধ্রুবক BaseType_t xAutoReload,
                                    শূন্য * ধ্রুবক pvTimerID,
                                    TimerCallbackFunction_t pxCallbackFunction )
        {
            Timer_t * pxNewTimer;

            traceENTER_xTimerCreate( pcTimerName, xTimerPeriodInTicks, xAutoReload, pvTimerID, pxCallbackFunction );

            /* MISRA Ref 11.5.1 [Malloc memory assignment] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
            /* coverity[misra_c_2012_rule_11_5_violation] */
            pxNewTimer = ( Timer_t * ) pvPortMalloc( আকার( Timer_t ) );

            যদি( pxNewTimer != ফাঁকা )
            {
                /* Status is thus far zero as the timer is not created statically
                 * and has not been started.  The auto-reload bit may get set in
                 * prvInitialiseNewTimer. */
                pxNewTimer->ucStatus = 0x00;
                prvInitialiseNewTimer( pcTimerName, xTimerPeriodInTicks, xAutoReload, pvTimerID, pxCallbackFunction, pxNewTimer );
            }

            traceRETURN_xTimerCreate( pxNewTimer );

            ফেরত pxNewTimer;
        }

    #endif /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

    #যদি ( configSUPPORT_STATIC_ALLOCATION == 1 )

        TimerHandle_t xTimerCreateStatic( ধ্রুবক অক্ষর * ধ্রুবক pcTimerName,
                                          ধ্রুবক TickType_t xTimerPeriodInTicks,
                                          ধ্রুবক BaseType_t xAutoReload,
                                          শূন্য * ধ্রুবক pvTimerID,
                                          TimerCallbackFunction_t pxCallbackFunction,
                                          StaticTimer_t * px_Timer_বাফার )
        {
            Timer_t * pxNewTimer;

            traceENTER_xTimerCreateStatic( pcTimerName, xTimerPeriodInTicks, xAutoReload, pvTimerID, pxCallbackFunction, px_Timer_বাফার );

            #যদি ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticTimer_t equals the size of the real timer
                 * structure. */
                অস্থির size_t x_আকার = আকার( StaticTimer_t );
                configASSERT( x_আকার == আকার( Timer_t ) );
                ( শূন্য ) x_আকার; /* Prevent unused variable warning when configASSERT() is not defined. */
            }
            #endif /* configASSERT_DEFINED */

            /* A pointer to a StaticTimer_t structure MUST be provided, use it. */
            configASSERT( px_Timer_বাফার );
            /* MISRA Ref 11.3.1 [Misaligned access] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
            /* coverity[misra_c_2012_rule_11_3_violation] */
            pxNewTimer = ( Timer_t * ) px_Timer_বাফার;

            যদি( pxNewTimer != ফাঁকা )
            {
                /* Timers can be created statically or dynamically so note this
                 * timer was created statically in case it is later deleted.  The
                 * auto-reload bit may get set in prvInitialiseNewTimer(). */
                pxNewTimer->ucStatus = ( uint8_t ) tmrSTATUS_IS_STATICALLY_ALLOCATED;

                prvInitialiseNewTimer( pcTimerName, xTimerPeriodInTicks, xAutoReload, pvTimerID, pxCallbackFunction, pxNewTimer );
            }

            traceRETURN_xTimerCreateStatic( pxNewTimer );

            ফেরত pxNewTimer;
        }

    #endif /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

    স্থির শূন্য prvInitialiseNewTimer( ধ্রুবক অক্ষর * ধ্রুবক pcTimerName,
                                       ধ্রুবক TickType_t xTimerPeriodInTicks,
                                       ধ্রুবক BaseType_t xAutoReload,
                                       শূন্য * ধ্রুবক pvTimerID,
                                       TimerCallbackFunction_t pxCallbackFunction,
                                       Timer_t * pxNewTimer )
    {
        /* 0 is not a valid value for xTimerPeriodInTicks. */
        configASSERT( ( xTimerPeriodInTicks > 0 ) );

        /* Ensure the infrastructure used by the timer service task has been
         * created/initialised. */
        prvCheckForValidListAndQueue();

        /* Initialise the timer structure members using the function
         * parameters. */
        pxNewTimer->pcTimerName = pcTimerName;
        pxNewTimer->xTimerPeriodInTicks = xTimerPeriodInTicks;
        pxNewTimer->pvTimerID = pvTimerID;
        pxNewTimer->pxCallbackFunction = pxCallbackFunction;
        vListInitialiseItem( &( pxNewTimer->xTimerListItem ) );

        যদি( xAutoReload != pdFALSE )
        {
            pxNewTimer->ucStatus |= ( uint8_t ) tmrSTATUS_IS_AUTORELOAD;
        }

        traceTIMER_CREATE( pxNewTimer );
    }
/*-----------------------------------------------------------*/

    BaseType_t xTimerGenericCommandFromTask( TimerHandle_t xTimer,
                                             ধ্রুবক BaseType_t xCommandID,
                                             ধ্রুবক TickType_t x_Optional_মান,
                                             BaseType_t * ধ্রুবক pxHigherPriorityTaskWoken,
                                             ধ্রুবক TickType_t xTicksToWait )
    {
        BaseType_t xReturn = pdFAIL;
        DaemonTaskMessage_t xMessage;

        ( শূন্য ) pxHigherPriorityTaskWoken;

        traceENTER_xTimerGenericCommandFromTask( xTimer, xCommandID, x_Optional_মান, pxHigherPriorityTaskWoken, xTicksToWait );

        configASSERT( xTimer );

        /* Send a message to the timer service task to perform a particular action
         * on a particular timer definition. */
        যদি( xTimerQueue != ফাঁকা )
        {
            /* Send a command to the timer service task to start the xTimer timer. */
            xMessage.xMessageID = xCommandID;
            xMessage.u.xTimerParameters.x_Message_মান = x_Optional_মান;
            xMessage.u.xTimerParameters.pxTimer = xTimer;

            configASSERT( xCommandID < tmrFIRST_FROM_ISR_COMMAND );

            যদি( xCommandID < tmrFIRST_FROM_ISR_COMMAND )
            {
                যদি( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING )
                {
                    xReturn = xQueueSendToBack( xTimerQueue, &xMessage, xTicksToWait );
                }
                অন্যথা
                {
                    xReturn = xQueueSendToBack( xTimerQueue, &xMessage, tmrNO_DELAY );
                }
            }

            traceTIMER_COMMAND_SEND( xTimer, xCommandID, x_Optional_মান, xReturn );
        }
        অন্যথা
        {
            mtCOVERAGE_TEST_MARKER();
        }

        traceRETURN_xTimerGenericCommandFromTask( xReturn );

        ফেরত xReturn;
    }
/*-----------------------------------------------------------*/

    BaseType_t xTimerGenericCommandFromISR( TimerHandle_t xTimer,
                                            ধ্রুবক BaseType_t xCommandID,
                                            ধ্রুবক TickType_t x_Optional_মান,
                                            BaseType_t * ধ্রুবক pxHigherPriorityTaskWoken,
                                            ধ্রুবক TickType_t xTicksToWait )
    {
        BaseType_t xReturn = pdFAIL;
        DaemonTaskMessage_t xMessage;

        ( শূন্য ) xTicksToWait;

        traceENTER_xTimerGenericCommandFromISR( xTimer, xCommandID, x_Optional_মান, pxHigherPriorityTaskWoken, xTicksToWait );

        configASSERT( xTimer );

        /* Send a message to the timer service task to perform a particular action
         * on a particular timer definition. */
        যদি( xTimerQueue != ফাঁকা )
        {
            /* Send a command to the timer service task to start the xTimer timer. */
            xMessage.xMessageID = xCommandID;
            xMessage.u.xTimerParameters.x_Message_মান = x_Optional_মান;
            xMessage.u.xTimerParameters.pxTimer = xTimer;

            configASSERT( xCommandID >= tmrFIRST_FROM_ISR_COMMAND );

            যদি( xCommandID >= tmrFIRST_FROM_ISR_COMMAND )
            {
                xReturn = xQueueSendToBackFromISR( xTimerQueue, &xMessage, pxHigherPriorityTaskWoken );
            }

            traceTIMER_COMMAND_SEND( xTimer, xCommandID, x_Optional_মান, xReturn );
        }
        অন্যথা
        {
            mtCOVERAGE_TEST_MARKER();
        }

        traceRETURN_xTimerGenericCommandFromISR( xReturn );

        ফেরত xReturn;
    }
/*-----------------------------------------------------------*/

    TaskHandle_t xTimerGetTimerDaemonTaskHandle( শূন্য )
    {
        traceENTER_xTimerGetTimerDaemonTaskHandle();

        /* If xTimerGetTimerDaemonTaskHandle() is called before the scheduler has been
         * started, then xTimerTaskHandle will be NULL. */
        configASSERT( ( xTimerTaskHandle != ফাঁকা ) );

        traceRETURN_xTimerGetTimerDaemonTaskHandle( xTimerTaskHandle );

        ফেরত xTimerTaskHandle;
    }
/*-----------------------------------------------------------*/

    TickType_t xTimerGetPeriod( TimerHandle_t xTimer )
    {
        Timer_t * pxTimer = xTimer;

        traceENTER_xTimerGetPeriod( xTimer );

        configASSERT( xTimer );

        traceRETURN_xTimerGetPeriod( pxTimer->xTimerPeriodInTicks );

        ফেরত pxTimer->xTimerPeriodInTicks;
    }
/*-----------------------------------------------------------*/

    শূন্য vTimerSetReloadMode( TimerHandle_t xTimer,
                              ধ্রুবক BaseType_t xAutoReload )
    {
        Timer_t * pxTimer = xTimer;

        traceENTER_vTimerSetReloadMode( xTimer, xAutoReload );

        configASSERT( xTimer );
        taskENTER_CRITICAL();
        {
            যদি( xAutoReload != pdFALSE )
            {
                pxTimer->ucStatus |= ( uint8_t ) tmrSTATUS_IS_AUTORELOAD;
            }
            অন্যথা
            {
                pxTimer->ucStatus &= ( ( uint8_t ) ~tmrSTATUS_IS_AUTORELOAD );
            }
        }
        taskEXIT_CRITICAL();

        traceRETURN_vTimerSetReloadMode();
    }
/*-----------------------------------------------------------*/

    BaseType_t xTimerGetReloadMode( TimerHandle_t xTimer )
    {
        Timer_t * pxTimer = xTimer;
        BaseType_t xReturn;

        traceENTER_xTimerGetReloadMode( xTimer );

        configASSERT( xTimer );
        taskENTER_CRITICAL();
        {
            যদি( ( pxTimer->ucStatus & tmrSTATUS_IS_AUTORELOAD ) == 0U )
            {
                /* Not an auto-reload timer. */
                xReturn = pdFALSE;
            }
            অন্যথা
            {
                /* Is an auto-reload timer. */
                xReturn = pdTRUE;
            }
        }
        taskEXIT_CRITICAL();

        traceRETURN_xTimerGetReloadMode( xReturn );

        ফেরত xReturn;
    }

    UBaseType_t uxTimerGetReloadMode( TimerHandle_t xTimer )
    {
        UBaseType_t uxReturn;

        traceENTER_uxTimerGetReloadMode( xTimer );

        uxReturn = ( UBaseType_t ) xTimerGetReloadMode( xTimer );

        traceRETURN_uxTimerGetReloadMode( uxReturn );

        ফেরত uxReturn;
    }
/*-----------------------------------------------------------*/

    TickType_t xTimerGetExpiryTime( TimerHandle_t xTimer )
    {
        Timer_t * pxTimer = xTimer;
        TickType_t xReturn;

        traceENTER_xTimerGetExpiryTime( xTimer );

        configASSERT( xTimer );
        xReturn = list_GET_LIST_ITEM_মান( &( pxTimer->xTimerListItem ) );

        traceRETURN_xTimerGetExpiryTime( xReturn );

        ফেরত xReturn;
    }
/*-----------------------------------------------------------*/

    #যদি ( configSUPPORT_STATIC_ALLOCATION == 1 )
        BaseType_t x_Timer_Get_Static_বাফার( TimerHandle_t xTimer,
                                          StaticTimer_t ** ppx_Timer_বাফার )
        {
            BaseType_t xReturn;
            Timer_t * pxTimer = xTimer;

            trace_ENTER_x_Timer_Get_Static_বাফার( xTimer, ppx_Timer_বাফার );

            configASSERT( ppx_Timer_বাফার != ফাঁকা );

            যদি( ( pxTimer->ucStatus & tmrSTATUS_IS_STATICALLY_ALLOCATED ) != 0U )
            {
                /* MISRA Ref 11.3.1 [Misaligned access] */
                /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
                /* coverity[misra_c_2012_rule_11_3_violation] */
                *ppx_Timer_বাফার = ( StaticTimer_t * ) pxTimer;
                xReturn = pdTRUE;
            }
            অন্যথা
            {
                xReturn = pdFALSE;
            }

            trace_RETURN_x_Timer_Get_Static_বাফার( xReturn );

            ফেরত xReturn;
        }
    #endif /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

    ধ্রুবক অক্ষর * pcTimerGetName( TimerHandle_t xTimer )
    {
        Timer_t * pxTimer = xTimer;

        traceENTER_pcTimerGetName( xTimer );

        configASSERT( xTimer );

        traceRETURN_pcTimerGetName( pxTimer->pcTimerName );

        ফেরত pxTimer->pcTimerName;
    }
/*-----------------------------------------------------------*/

    স্থির শূন্য prvReloadTimer( Timer_t * ধ্রুবক pxTimer,
                                TickType_t xExpiredTime,
                                ধ্রুবক TickType_t xTimeNow )
    {
        /* Insert the timer into the appropriate list for the next expiry time.
         * If the next expiry time has already passed, advance the expiry time,
         * call the callback function, and try again. */
        যখন( prv_ঢোকাও_Timer_In_Active_List( pxTimer, ( xExpiredTime + pxTimer->xTimerPeriodInTicks ), xTimeNow, xExpiredTime ) != pdFALSE )
        {
            /* Advance the expiry time. */
            xExpiredTime += pxTimer->xTimerPeriodInTicks;

            /* Call the timer callback. */
            traceTIMER_EXPIRED( pxTimer );
            pxTimer->pxCallbackFunction( ( TimerHandle_t ) pxTimer );
        }
    }
/*-----------------------------------------------------------*/

    স্থির শূন্য prvProcessExpiredTimer( ধ্রুবক TickType_t x_পরবর্তী_Expire_Time,
                                        ধ্রুবক TickType_t xTimeNow )
    {
        /* MISRA Ref 11.5.3 [Void pointer assignment] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
        /* coverity[misra_c_2012_rule_11_5_violation] */
        Timer_t * ধ্রুবক pxTimer = ( Timer_t * ) list_GET_OWNER_OF_শীর্ষ_ENTRY( pxCurrentTimerList );

        /* Remove the timer from the list of active timers.  A check has already
         * been performed to ensure the list is not empty. */

        ( শূন্য ) uxListRemove( &( pxTimer->xTimerListItem ) );

        /* If the timer is an auto-reload timer then calculate the next
         * expiry time and re-insert the timer in the list of active timers. */
        যদি( ( pxTimer->ucStatus & tmrSTATUS_IS_AUTORELOAD ) != 0U )
        {
            prvReloadTimer( pxTimer, x_পরবর্তী_Expire_Time, xTimeNow );
        }
        অন্যথা
        {
            pxTimer->ucStatus &= ( ( uint8_t ) ~tmrSTATUS_IS_ACTIVE );
        }

        /* Call the timer callback. */
        traceTIMER_EXPIRED( pxTimer );
        pxTimer->pxCallbackFunction( ( TimerHandle_t ) pxTimer );
    }
/*-----------------------------------------------------------*/

    স্থির portTASK_FUNCTION( prvTimerTask, pvParameters )
    {
        TickType_t x_পরবর্তী_Expire_Time;
        BaseType_t xListWasEmpty;

        /* Just to avoid compiler warnings. */
        ( শূন্য ) pvParameters;

        #যদি ( configUSE_DAEMON_TASK_STARTUP_HOOK == 1 )
        {
            /* Allow the application writer to execute some code in the context of
             * this task at the point the task starts executing.  This is useful if the
             * application includes initialisation code that would benefit from
             * executing after the scheduler has been started. */
            vApplicationDaemonTaskStartupHook();
        }
        #endif /* configUSE_DAEMON_TASK_STARTUP_HOOK */

        জন্য( ; configCONTROL_INFINITE_LOOP(); )
        {
            /* Query the timers list to see if it contains any timers, and if so,
             * obtain the time at which the next timer will expire. */
            x_পরবর্তী_Expire_Time = prv_Get_পরবর্তী_Expire_Time( &xListWasEmpty );

            /* If a timer has expired, process it.  Otherwise, block this task
             * until either a timer does expire, or a command is received. */
            prvProcessTimerOrBlockTask( x_পরবর্তী_Expire_Time, xListWasEmpty );

            /* Empty the command queue. */
            prvProcessReceivedCommands();
        }
    }
/*-----------------------------------------------------------*/

    স্থির শূন্য prvProcessTimerOrBlockTask( ধ্রুবক TickType_t x_পরবর্তী_Expire_Time,
                                            BaseType_t xListWasEmpty )
    {
        TickType_t xTimeNow;
        BaseType_t xTimerListsWereSwitched;

        vTaskSuspendAll();
        {
            /* Obtain the time now to make an assessment as to whether the timer
             * has expired or not.  If obtaining the time causes the lists to switch
             * then don't process this timer as any timers that remained in the list
             * when the lists were switched will have been processed within the
             * prvSampleTimeNow() function. */
            xTimeNow = prvSampleTimeNow( &xTimerListsWereSwitched );

            যদি( xTimerListsWereSwitched == pdFALSE )
            {
                /* The tick count has not overflowed, has the timer expired? */
                যদি( ( xListWasEmpty == pdFALSE ) && ( x_পরবর্তী_Expire_Time <= xTimeNow ) )
                {
                    ( শূন্য ) xTaskResumeAll();
                    prvProcessExpiredTimer( x_পরবর্তী_Expire_Time, xTimeNow );
                }
                অন্যথা
                {
                    /* The tick count has not overflowed, and the next expire
                     * time has not been reached yet.  This task should therefore
                     * block to wait for the next expire time or a command to be
                     * received - whichever comes first.  The following line cannot
                     * be reached unless xNextExpireTime > xTimeNow, except in the
                     * case when the current timer list is empty. */
                    যদি( xListWasEmpty != pdFALSE )
                    {
                        /* The current timer list is empty - is the overflow list
                         * also empty? */
                        xListWasEmpty = listLIST_IS_EMPTY( pxOverflowTimerList );
                    }

                    vQueueWaitForMessageRestricted( xTimerQueue, ( x_পরবর্তী_Expire_Time - xTimeNow ), xListWasEmpty );

                    যদি( xTaskResumeAll() == pdFALSE )
                    {
                        /* Yield to wait for either a command to arrive, or the
                         * block time to expire.  If a command arrived between the
                         * critical section being exited and this yield then the yield
                         * will not cause the task to block. */
                        taskYIELD_WITHIN_API();
                    }
                    অন্যথা
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
            }
            অন্যথা
            {
                ( শূন্য ) xTaskResumeAll();
            }
        }
    }
/*-----------------------------------------------------------*/

    স্থির TickType_t prv_Get_পরবর্তী_Expire_Time( BaseType_t * ধ্রুবক pxListWasEmpty )
    {
        TickType_t x_পরবর্তী_Expire_Time;

        /* Timers are listed in expiry time order, with the head of the list
         * referencing the task that will expire first.  Obtain the time at which
         * the timer with the nearest expiry time will expire.  If there are no
         * active timers then just set the next expire time to 0.  That will cause
         * this task to unblock when the tick count overflows, at which point the
         * timer lists will be switched and the next expiry time can be
         * re-assessed.  */
        *pxListWasEmpty = listLIST_IS_EMPTY( pxCurrentTimerList );

        যদি( *pxListWasEmpty == pdFALSE )
        {
            x_পরবর্তী_Expire_Time = list_GET_ITEM_মান_OF_শীর্ষ_ENTRY( pxCurrentTimerList );
        }
        অন্যথা
        {
            /* Ensure the task unblocks when the tick count rolls over. */
            x_পরবর্তী_Expire_Time = ( TickType_t ) 0U;
        }

        ফেরত x_পরবর্তী_Expire_Time;
    }
/*-----------------------------------------------------------*/

    স্থির TickType_t prvSampleTimeNow( BaseType_t * ধ্রুবক pxTimerListsWereSwitched )
    {
        TickType_t xTimeNow;
        PRIVILEGED_তথ্য স্থির TickType_t xLastTime = ( TickType_t ) 0U;

        xTimeNow = x_Task_Get_Tick_গুনতি();

        যদি( xTimeNow < xLastTime )
        {
            prvSwitchTimerLists();
            *pxTimerListsWereSwitched = pdTRUE;
        }
        অন্যথা
        {
            *pxTimerListsWereSwitched = pdFALSE;
        }

        xLastTime = xTimeNow;

        ফেরত xTimeNow;
    }
/*-----------------------------------------------------------*/

    স্থির BaseType_t prv_ঢোকাও_Timer_In_Active_List( Timer_t * ধ্রুবক pxTimer,
                                                  ধ্রুবক TickType_t x_পরবর্তী_Expiry_Time,
                                                  ধ্রুবক TickType_t xTimeNow,
                                                  ধ্রুবক TickType_t xCommandTime )
    {
        BaseType_t xProcessTimerNow = pdFALSE;

        list_SET_LIST_ITEM_মান( &( pxTimer->xTimerListItem ), x_পরবর্তী_Expiry_Time );
        listSET_LIST_ITEM_OWNER( &( pxTimer->xTimerListItem ), pxTimer );

        যদি( x_পরবর্তী_Expiry_Time <= xTimeNow )
        {
            /* Has the expiry time elapsed between the command to start/reset a
             * timer was issued, and the time the command was processed? */
            যদি( ( ( TickType_t ) ( xTimeNow - xCommandTime ) ) >= pxTimer->xTimerPeriodInTicks )
            {
                /* The time between a command being issued and the command being
                 * processed actually exceeds the timers period.  */
                xProcessTimerNow = pdTRUE;
            }
            অন্যথা
            {
                v_List_ঢোকাও( pxOverflowTimerList, &( pxTimer->xTimerListItem ) );
            }
        }
        অন্যথা
        {
            যদি( ( xTimeNow < xCommandTime ) && ( x_পরবর্তী_Expiry_Time >= xCommandTime ) )
            {
                /* If, since the command was issued, the tick count has overflowed
                 * but the expiry time has not, then the timer must have already passed
                 * its expiry time and should be processed immediately. */
                xProcessTimerNow = pdTRUE;
            }
            অন্যথা
            {
                v_List_ঢোকাও( pxCurrentTimerList, &( pxTimer->xTimerListItem ) );
            }
        }

        ফেরত xProcessTimerNow;
    }
/*-----------------------------------------------------------*/

    স্থির শূন্য prvProcessReceivedCommands( শূন্য )
    {
        DaemonTaskMessage_t xMessage = { 0 };
        Timer_t * pxTimer;
        BaseType_t xTimerListsWereSwitched;
        TickType_t xTimeNow;

        যখন( xQueueReceive( xTimerQueue, &xMessage, tmrNO_DELAY ) != pdFAIL )
        {
            #যদি ( INCLUDE_xTimerPendFunctionCall == 1 )
            {
                /* Negative commands are pended function calls rather than timer
                 * commands. */
                যদি( xMessage.xMessageID < ( BaseType_t ) 0 )
                {
                    ধ্রুবক CallbackParameters_t * ধ্রুবক pxCallback = &( xMessage.u.xCallbackParameters );

                    /* The timer uses the xCallbackParameters member to request a
                     * callback be executed.  Check the callback is not NULL. */
                    configASSERT( pxCallback );

                    /* Call the function. */
                    pxCallback->pxCallbackFunction( pxCallback->pvParameter1, pxCallback->ulParameter2 );
                }
                অন্যথা
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            #endif /* INCLUDE_xTimerPendFunctionCall */

            /* Commands that are positive are timer commands rather than pended
             * function calls. */
            যদি( xMessage.xMessageID >= ( BaseType_t ) 0 )
            {
                /* The messages uses the xTimerParameters member to work on a
                 * software timer. */
                pxTimer = xMessage.u.xTimerParameters.pxTimer;

                যদি( listIS_CONTAINED_WITHIN( ফাঁকা, &( pxTimer->xTimerListItem ) ) == pdFALSE )
                {
                    /* The timer is in a list, remove it. */
                    ( শূন্য ) uxListRemove( &( pxTimer->xTimerListItem ) );
                }
                অন্যথা
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                traceTIMER_COMMAND_RECEIVED( pxTimer, xMessage.xMessageID, xMessage.u.xTimerParameters.x_Message_মান );

                /* In this case the xTimerListsWereSwitched parameter is not used, but
                 *  it must be present in the function call.  prvSampleTimeNow() must be
                 *  called after the message is received from xTimerQueue so there is no
                 *  possibility of a higher priority task adding a message to the message
                 *  queue with a time that is ahead of the timer daemon task (because it
                 *  pre-empted the timer daemon task after the xTimeNow value was set). */
                xTimeNow = prvSampleTimeNow( &xTimerListsWereSwitched );

                নির্বাচন( xMessage.xMessageID )
                {
                    ক্ষেত্র tmrCOMMAND_START:
                    ক্ষেত্র tmrCOMMAND_START_FROM_ISR:
                    ক্ষেত্র tmrCOMMAND_RESET:
                    ক্ষেত্র tmrCOMMAND_RESET_FROM_ISR:
                        /* Start or restart a timer. */
                        pxTimer->ucStatus |= ( uint8_t ) tmrSTATUS_IS_ACTIVE;

                        যদি( prv_ঢোকাও_Timer_In_Active_List( pxTimer, xMessage.u.xTimerParameters.x_Message_মান + pxTimer->xTimerPeriodInTicks, xTimeNow, xMessage.u.xTimerParameters.x_Message_মান ) != pdFALSE )
                        {
                            /* The timer expired before it was added to the active
                             * timer list.  Process it now. */
                            যদি( ( pxTimer->ucStatus & tmrSTATUS_IS_AUTORELOAD ) != 0U )
                            {
                                prvReloadTimer( pxTimer, xMessage.u.xTimerParameters.x_Message_মান + pxTimer->xTimerPeriodInTicks, xTimeNow );
                            }
                            অন্যথা
                            {
                                pxTimer->ucStatus &= ( ( uint8_t ) ~tmrSTATUS_IS_ACTIVE );
                            }

                            /* Call the timer callback. */
                            traceTIMER_EXPIRED( pxTimer );
                            pxTimer->pxCallbackFunction( ( TimerHandle_t ) pxTimer );
                        }
                        অন্যথা
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }

                        বিরতি;

                    ক্ষেত্র tmrCOMMAND_STOP:
                    ক্ষেত্র tmrCOMMAND_STOP_FROM_ISR:
                        /* The timer has already been removed from the active list. */
                        pxTimer->ucStatus &= ( ( uint8_t ) ~tmrSTATUS_IS_ACTIVE );
                        বিরতি;

                    ক্ষেত্র tmrCOMMAND_CHANGE_PERIOD:
                    ক্ষেত্র tmrCOMMAND_CHANGE_PERIOD_FROM_ISR:
                        pxTimer->ucStatus |= ( uint8_t ) tmrSTATUS_IS_ACTIVE;
                        pxTimer->xTimerPeriodInTicks = xMessage.u.xTimerParameters.x_Message_মান;
                        configASSERT( ( pxTimer->xTimerPeriodInTicks > 0 ) );

                        /* The new period does not really have a reference, and can
                         * be longer or shorter than the old one.  The command time is
                         * therefore set to the current time, and as the period cannot
                         * be zero the next expiry time can only be in the future,
                         * meaning (unlike for the xTimerStart() case above) there is
                         * no fail case that needs to be handled here. */
                        ( শূন্য ) prv_ঢোকাও_Timer_In_Active_List( pxTimer, ( xTimeNow + pxTimer->xTimerPeriodInTicks ), xTimeNow, xTimeNow );
                        বিরতি;

                    ক্ষেত্র tmrCOMMAND_DELETE:
                        #যদি ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
                        {
                            /* The timer has already been removed from the active list,
                             * just free up the memory if the memory was dynamically
                             * allocated. */
                            যদি( ( pxTimer->ucStatus & tmrSTATUS_IS_STATICALLY_ALLOCATED ) == ( uint8_t ) 0 )
                            {
                                vPortFree( pxTimer );
                            }
                            অন্যথা
                            {
                                pxTimer->ucStatus &= ( ( uint8_t ) ~tmrSTATUS_IS_ACTIVE );
                            }
                        }
                        #অন্যথা /* if ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) */
                        {
                            /* If dynamic allocation is not enabled, the memory
                             * could not have been dynamically allocated. So there is
                             * no need to free the memory - just mark the timer as
                             * "not active". */
                            pxTimer->ucStatus &= ( ( uint8_t ) ~tmrSTATUS_IS_ACTIVE );
                        }
                        #endif /* configSUPPORT_DYNAMIC_ALLOCATION */
                        বিরতি;

                    পূর্বনির্ধারিত:
                        /* Don't expect to get here. */
                        বিরতি;
                }
            }
        }
    }
/*-----------------------------------------------------------*/

    স্থির শূন্য prvSwitchTimerLists( শূন্য )
    {
        TickType_t x_পরবর্তী_Expire_Time;
        List_t * px_অস্থায়ী;

        /* The tick count has overflowed.  The timer lists must be switched.
         * If there are any timers still referenced from the current timer list
         * then they must have expired and should be processed before the lists
         * are switched. */
        যখন( listLIST_IS_EMPTY( pxCurrentTimerList ) == pdFALSE )
        {
            x_পরবর্তী_Expire_Time = list_GET_ITEM_মান_OF_শীর্ষ_ENTRY( pxCurrentTimerList );

            /* Process the expired timer.  For auto-reload timers, be careful to
             * process only expirations that occur on the current list.  Further
             * expirations must wait until after the lists are switched. */
            prvProcessExpiredTimer( x_পরবর্তী_Expire_Time, tmrMAX_TIME_BEFORE_OVERFLOW );
        }

        px_অস্থায়ী = pxCurrentTimerList;
        pxCurrentTimerList = pxOverflowTimerList;
        pxOverflowTimerList = px_অস্থায়ী;
    }
/*-----------------------------------------------------------*/

    স্থির শূন্য prvCheckForValidListAndQueue( শূন্য )
    {
        /* Check that the list from which active timers are referenced, and the
         * queue used to communicate with the timer service, have been
         * initialised. */
        taskENTER_CRITICAL();
        {
            যদি( xTimerQueue == ফাঁকা )
            {
                vListInitialise( &xActiveTimerList1 );
                vListInitialise( &xActiveTimerList2 );
                pxCurrentTimerList = &xActiveTimerList1;
                pxOverflowTimerList = &xActiveTimerList2;

                #যদি ( configSUPPORT_STATIC_ALLOCATION == 1 )
                {
                    /* The timer queue is allocated statically in case
                     * configSUPPORT_DYNAMIC_ALLOCATION is 0. */
                    PRIVILEGED_তথ্য স্থির StaticQueue_t xStaticTimerQueue;
                    PRIVILEGED_তথ্য স্থির uint8_t ucStaticTimerQueueStorage[ ( size_t ) config_TIMER_QUEUE_দৈর্ঘ্য * আকার( DaemonTaskMessage_t ) ];

                    xTimerQueue = xQueueCreateStatic( ( UBaseType_t ) config_TIMER_QUEUE_দৈর্ঘ্য, ( UBaseType_t ) আকার( DaemonTaskMessage_t ), &( ucStaticTimerQueueStorage[ 0 ] ), &xStaticTimerQueue );
                }
                #অন্যথা
                {
                    xTimerQueue = xQueueCreate( ( UBaseType_t ) config_TIMER_QUEUE_দৈর্ঘ্য, ( UBaseType_t ) আকার( DaemonTaskMessage_t ) );
                }
                #endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */

                #যদি ( config_QUEUE_REGISTRY_আকার > 0 )
                {
                    যদি( xTimerQueue != ফাঁকা )
                    {
                        vQueueAddToRegistry( xTimerQueue, "TmrQ" );
                    }
                    অন্যথা
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #endif /* configQUEUE_REGISTRY_SIZE */
            }
            অন্যথা
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        taskEXIT_CRITICAL();
    }
/*-----------------------------------------------------------*/

    BaseType_t xTimerIsTimerActive( TimerHandle_t xTimer )
    {
        BaseType_t xReturn;
        Timer_t * pxTimer = xTimer;

        traceENTER_xTimerIsTimerActive( xTimer );

        configASSERT( xTimer );

        /* Is the timer in the list of active timers? */
        taskENTER_CRITICAL();
        {
            যদি( ( pxTimer->ucStatus & tmrSTATUS_IS_ACTIVE ) == 0U )
            {
                xReturn = pdFALSE;
            }
            অন্যথা
            {
                xReturn = pdTRUE;
            }
        }
        taskEXIT_CRITICAL();

        traceRETURN_xTimerIsTimerActive( xReturn );

        ফেরত xReturn;
    }
/*-----------------------------------------------------------*/

    শূন্য * pvTimerGetTimerID( ধ্রুবক TimerHandle_t xTimer )
    {
        Timer_t * ধ্রুবক pxTimer = xTimer;
        শূন্য * pvReturn;

        traceENTER_pvTimerGetTimerID( xTimer );

        configASSERT( xTimer );

        taskENTER_CRITICAL();
        {
            pvReturn = pxTimer->pvTimerID;
        }
        taskEXIT_CRITICAL();

        traceRETURN_pvTimerGetTimerID( pvReturn );

        ফেরত pvReturn;
    }
/*-----------------------------------------------------------*/

    শূন্য vTimerSetTimerID( TimerHandle_t xTimer,
                           শূন্য * pvNewID )
    {
        Timer_t * ধ্রুবক pxTimer = xTimer;

        traceENTER_vTimerSetTimerID( xTimer, pvNewID );

        configASSERT( xTimer );

        taskENTER_CRITICAL();
        {
            pxTimer->pvTimerID = pvNewID;
        }
        taskEXIT_CRITICAL();

        traceRETURN_vTimerSetTimerID();
    }
/*-----------------------------------------------------------*/

    #যদি ( INCLUDE_xTimerPendFunctionCall == 1 )

        BaseType_t xTimerPendFunctionCallFromISR( PendedFunction_t xFunctionToPend,
                                                  শূন্য * pvParameter1,
                                                  uint32_t ulParameter2,
                                                  BaseType_t * pxHigherPriorityTaskWoken )
        {
            DaemonTaskMessage_t xMessage;
            BaseType_t xReturn;

            traceENTER_xTimerPendFunctionCallFromISR( xFunctionToPend, pvParameter1, ulParameter2, pxHigherPriorityTaskWoken );

            /* Complete the message with the function parameters and post it to the
             * daemon task. */
            xMessage.xMessageID = tmrCOMMAND_EXECUTE_CALLBACK_FROM_ISR;
            xMessage.u.xCallbackParameters.pxCallbackFunction = xFunctionToPend;
            xMessage.u.xCallbackParameters.pvParameter1 = pvParameter1;
            xMessage.u.xCallbackParameters.ulParameter2 = ulParameter2;

            xReturn = xQueueSendFromISR( xTimerQueue, &xMessage, pxHigherPriorityTaskWoken );

            tracePEND_FUNC_CALL_FROM_ISR( xFunctionToPend, pvParameter1, ulParameter2, xReturn );
            traceRETURN_xTimerPendFunctionCallFromISR( xReturn );

            ফেরত xReturn;
        }

    #endif /* INCLUDE_xTimerPendFunctionCall */
/*-----------------------------------------------------------*/

    #যদি ( INCLUDE_xTimerPendFunctionCall == 1 )

        BaseType_t xTimerPendFunctionCall( PendedFunction_t xFunctionToPend,
                                           শূন্য * pvParameter1,
                                           uint32_t ulParameter2,
                                           TickType_t xTicksToWait )
        {
            DaemonTaskMessage_t xMessage;
            BaseType_t xReturn;

            traceENTER_xTimerPendFunctionCall( xFunctionToPend, pvParameter1, ulParameter2, xTicksToWait );

            /* This function can only be called after a timer has been created or
             * after the scheduler has been started because, until then, the timer
             * queue does not exist. */
            configASSERT( xTimerQueue );

            /* Complete the message with the function parameters and post it to the
             * daemon task. */
            xMessage.xMessageID = tmrCOMMAND_EXECUTE_CALLBACK;
            xMessage.u.xCallbackParameters.pxCallbackFunction = xFunctionToPend;
            xMessage.u.xCallbackParameters.pvParameter1 = pvParameter1;
            xMessage.u.xCallbackParameters.ulParameter2 = ulParameter2;

            xReturn = xQueueSendToBack( xTimerQueue, &xMessage, xTicksToWait );

            tracePEND_FUNC_CALL( xFunctionToPend, pvParameter1, ulParameter2, xReturn );
            traceRETURN_xTimerPendFunctionCall( xReturn );

            ফেরত xReturn;
        }

    #endif /* INCLUDE_xTimerPendFunctionCall */
/*-----------------------------------------------------------*/

    #যদি ( configUSE_TRACE_FACILITY == 1 )

        UBaseType_t ux_Timer_Get_Timer_সংখ্যা( TimerHandle_t xTimer )
        {
            trace_ENTER_ux_Timer_Get_Timer_সংখ্যা( xTimer );

            trace_RETURN_ux_Timer_Get_Timer_সংখ্যা( ( ( Timer_t * ) xTimer )->ux_Timer_সংখ্যা );

            ফেরত ( ( Timer_t * ) xTimer )->ux_Timer_সংখ্যা;
        }

    #endif /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

    #যদি ( configUSE_TRACE_FACILITY == 1 )

        শূন্য v_Timer_Set_Timer_সংখ্যা( TimerHandle_t xTimer,
                                   UBaseType_t ux_Timer_সংখ্যা )
        {
            trace_ENTER_v_Timer_Set_Timer_সংখ্যা( xTimer, ux_Timer_সংখ্যা );

            ( ( Timer_t * ) xTimer )->ux_Timer_সংখ্যা = ux_Timer_সংখ্যা;

            trace_RETURN_v_Timer_Set_Timer_সংখ্যা();
        }

    #endif /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

/*
 * Reset the state in this file. This state is normally initialized at start up.
 * This function must be called by the application before restarting the
 * scheduler.
 */
    শূন্য vTimerResetState( শূন্য )
    {
        xTimerQueue = ফাঁকা;
        xTimerTaskHandle = ফাঁকা;
    }
/*-----------------------------------------------------------*/

/* This entire source file will be skipped if the application is not configured
 * to include software timer functionality.  If you want to include software timer
 * functionality then ensure configUSE_TIMERS is set to 1 in FreeRTOSConfig.h. */
#endif /* configUSE_TIMERS == 1 */
