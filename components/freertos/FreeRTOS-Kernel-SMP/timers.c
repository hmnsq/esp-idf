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
#ଅନ୍ତର୍ଭାଵ <stdlib.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#define MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#ଅନ୍ତର୍ଭାଵ "FreeRTOS.h"
#ଅନ୍ତର୍ଭାଵ "task.h"
#ଅନ୍ତର୍ଭାଵ "queue.h"
#ଅନ୍ତର୍ଭାଵ "timers.h"

#ଯଦି ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 0 )
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
#ଯଦି ( configUSE_TIMERS == 1 )

/* Misc definitions. */
    #define tmrNO_DELAY                    ( ( TickType_t ) 0U )
    #define tmrMAX_TIME_BEFORE_OVERFLOW    ( ( TickType_t ) -1 )

/* The name assigned to the timer service task. This can be overridden by
 * defining configTIMER_SERVICE_TASK_NAME in FreeRTOSConfig.h. */
    #ifndef configTIMER_SERVICE_TASK_NAME
        #define configTIMER_SERVICE_TASK_NAME    "Tmr Svc"
    #endif

    #ଯଦି ( ( config_ସଂଖ୍ୟା_OF_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )

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
    ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା tmrTimerControl                                               /* The old naming convention is used to prevent breaking kernel aware debuggers. */
    {
        ଅଚଳ ଅକ୍ଷର * pcTimerName;                                                /**< Text name.  This is not used by the kernel, it is included simply to make debugging easier. */
        ListItem_t xTimerListItem;                                               /**< Standard linked list item as used by all kernel features for event management. */
        TickType_t xTimerPeriodInTicks;                                          /**< How quickly and often the timer expires. */
        ଶୂନ୍ୟ * pvTimerID;                                                        /**< An ID to identify the timer.  This allows the timer to be identified when the same callback is used for multiple timers. */
        portTIMER_CALLBACK_ATTRIBUTE TimerCallbackFunction_t pxCallbackFunction; /**< The function that will be called when the timer expires. */
        #ଯଦି ( configUSE_TRACE_FACILITY == 1 )
            UBaseType_t ux_Timer_ସଂଖ୍ୟା;                                           /**< An ID assigned by trace tools such as FreeRTOS+Trace */
        #endif
        uint8_t ucStatus;                                                        /**< Holds bits to say if the timer was statically allocated or not, and if it is active or not. */
    } xTIMER;

/* The old xTIMER name is maintained above then typedefed to the new Timer_t
 * name below to enable the use of older kernel aware debuggers. */
    ପ୍ରକାରବ୍ୟାଖ୍ୟା xTIMER Timer_t;

/* The definition of messages that can be sent and received on the timer queue.
 * Two types of message can be queued - messages that manipulate a software timer,
 * and messages that request the execution of a non-timer related callback.  The
 * two message types are defined in two separate structures, xTimerParametersType
 * and xCallbackParametersType respectively. */
    ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା tmrTimerParameters
    {
        TickType_t x_Message_ମୂଲ୍ୟ; /**< An optional value used by a subset of commands, for example, when changing the period of a timer. */
        Timer_t * pxTimer;        /**< The timer to which the command will be applied. */
    } TimerParameter_t;


    ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା tmrCallbackParameters
    {
        portTIMER_CALLBACK_ATTRIBUTE
        PendedFunction_t pxCallbackFunction; /* << The callback function to execute. */
        ଶୂନ୍ୟ * pvParameter1;                 /* << The value that will be used as the callback functions first parameter. */
        uint32_t ulParameter2;               /* << The value that will be used as the callback functions second parameter. */
    } CallbackParameters_t;

/* The structure that contains the two message types, along with an identifier
 * that is used to determine which message type is valid. */
    ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା tmrTimerQueueMessage
    {
        BaseType_t xMessageID; /**< The command being sent to the timer service task. */
        ସଂଘ
        {
            TimerParameter_t xTimerParameters;

            /* Don't include xCallbackParameters if it is not going to be used as
             * it makes the structure (and therefore the timer queue) larger. */
            #ଯଦି ( INCLUDE_xTimerPendFunctionCall == 1 )
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
    PRIVILEGED_DATA ସ୍ଥିର List_t xActiveTimerList1;
    PRIVILEGED_DATA ସ୍ଥିର List_t xActiveTimerList2;
    PRIVILEGED_DATA ସ୍ଥିର List_t * pxCurrentTimerList;
    PRIVILEGED_DATA ସ୍ଥିର List_t * pxOverflowTimerList;

/* A queue that is used to send commands to the timer service task. */
    PRIVILEGED_DATA ସ୍ଥିର QueueHandle_t xTimerQueue = ଖାଲି;
    PRIVILEGED_DATA ସ୍ଥିର TaskHandle_t xTimerTaskHandle = ଖାଲି;

/*-----------------------------------------------------------*/

/*
 * Initialise the infrastructure used by the timer service task if it has not
 * been initialised already.
 */
    ସ୍ଥିର ଶୂନ୍ୟ prvCheckForValidListAndQueue( ଶୂନ୍ୟ ) PRIVILEGED_FUNCTION;

/*
 * The timer service task (daemon).  Timer functionality is controlled by this
 * task.  Other tasks communicate with the timer service task using the
 * xTimerQueue queue.
 */
    ସ୍ଥିର portTASK_FUNCTION_PROTO( prvTimerTask, pvParameters ) PRIVILEGED_FUNCTION;

/*
 * Called by the timer service task to interpret and process a command it
 * received on the timer queue.
 */
    ସ୍ଥିର ଶୂନ୍ୟ prvProcessReceivedCommands( ଶୂନ୍ୟ ) PRIVILEGED_FUNCTION;

/*
 * Insert the timer into either xActiveTimerList1, or xActiveTimerList2,
 * depending on if the expire time causes a timer counter overflow.
 */
    ସ୍ଥିର BaseType_t prv_ଭର୍ତ୍ତି_Timer_In_Active_List( Timer_t * ଅଚଳ pxTimer,
                                                  ଅଚଳ TickType_t xNextExpiryTime,
                                                  ଅଚଳ TickType_t xTimeNow,
                                                  ଅଚଳ TickType_t xCommandTime ) PRIVILEGED_FUNCTION;

/*
 * Reload the specified auto-reload timer.  If the reloading is backlogged,
 * clear the backlog, calling the callback for each additional reload.  When
 * this function returns, the next expiry time is after xTimeNow.
 */
    ସ୍ଥିର ଶୂନ୍ୟ prvReloadTimer( Timer_t * ଅଚଳ pxTimer,
                                TickType_t xExpiredTime,
                                ଅଚଳ TickType_t xTimeNow ) PRIVILEGED_FUNCTION;

/*
 * An active timer has reached its expire time.  Reload the timer if it is an
 * auto-reload timer, then call its callback.
 */
    ସ୍ଥିର ଶୂନ୍ୟ prvProcessExpiredTimer( ଅଚଳ TickType_t xNextExpireTime,
                                        ଅଚଳ TickType_t xTimeNow ) PRIVILEGED_FUNCTION;

/*
 * The tick count has overflowed.  Switch the timer lists after ensuring the
 * current timer list does not still reference some timers.
 */
    ସ୍ଥିର ଶୂନ୍ୟ prvSwitchTimerLists( ଶୂନ୍ୟ ) PRIVILEGED_FUNCTION;

/*
 * Obtain the current tick count, setting *pxTimerListsWereSwitched to pdTRUE
 * if a tick count overflow occurred since prvSampleTimeNow() was last called.
 */
    ସ୍ଥିର TickType_t prvSampleTimeNow( BaseType_t * ଅଚଳ pxTimerListsWereSwitched ) PRIVILEGED_FUNCTION;

/*
 * If the timer list contains any active timers then return the expire time of
 * the timer that will expire first and set *pxListWasEmpty to false.  If the
 * timer list does not contain any timers then return 0 and set *pxListWasEmpty
 * to pdTRUE.
 */
    ସ୍ଥିର TickType_t prvGetNextExpireTime( BaseType_t * ଅଚଳ pxListWasEmpty ) PRIVILEGED_FUNCTION;

/*
 * If a timer has expired, process it.  Otherwise, block the timer service task
 * until either a timer does expire or a command is received.
 */
    ସ୍ଥିର ଶୂନ୍ୟ prvProcessTimerOrBlockTask( ଅଚଳ TickType_t xNextExpireTime,
                                            BaseType_t xListWasEmpty ) PRIVILEGED_FUNCTION;

/*
 * Called after a Timer_t structure has been allocated either statically or
 * dynamically to fill in the structure's members.
 */
    ସ୍ଥିର ଶୂନ୍ୟ prvInitialiseNewTimer( ଅଚଳ ଅକ୍ଷର * ଅଚଳ pcTimerName,
                                       ଅଚଳ TickType_t xTimerPeriodInTicks,
                                       ଅଚଳ BaseType_t xAutoReload,
                                       ଶୂନ୍ୟ * ଅଚଳ pvTimerID,
                                       TimerCallbackFunction_t pxCallbackFunction,
                                       Timer_t * pxNewTimer ) PRIVILEGED_FUNCTION;
/*-----------------------------------------------------------*/

    BaseType_t xTimerCreateTimerTask( ଶୂନ୍ୟ )
    {
        BaseType_t xReturn = pdFAIL;

        traceENTER_xTimerCreateTimerTask();

        /* This function is called when the scheduler is started if
         * configUSE_TIMERS is set to 1.  Check that the infrastructure used by the
         * timer service task has been created/initialised.  If timers have already
         * been created then the initialisation will already have been performed. */
        prvCheckForValidListAndQueue();

        ଯଦି( xTimerQueue != ଖାଲି )
        {
            #ଯଦି ( ( config_ସଂଖ୍ୟା_OF_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
            {
                #ଯଦି ( configSUPPORT_STATIC_ALLOCATION == 1 )
                {
                    StaticTask_t * pxTimerTaskTCBBuffer = ଖାଲି;
                    ଗଦା_Type_t * px_Timer_Task_ଗଦା_Buffer = ଖାଲି;
                    config_ଗଦା_DEPTH_TYPE ux_Timer_Task_ଗଦା_Size;

                    vApplicationGetTimerTaskMemory( &pxTimerTaskTCBBuffer, &px_Timer_Task_ଗଦା_Buffer, &ux_Timer_Task_ଗଦା_Size );
                    xTimerTaskHandle = xTaskCreateStaticAffinitySet( prvTimerTask,
                                                                     configTIMER_SERVICE_TASK_NAME,
                                                                     ux_Timer_Task_ଗଦା_Size,
                                                                     ଖାଲି,
                                                                     ( ( UBaseType_t ) configTIMER_TASK_PRIORITY ) | portPRIVILEGE_BIT,
                                                                     px_Timer_Task_ଗଦା_Buffer,
                                                                     pxTimerTaskTCBBuffer,
                                                                     configTIMER_SERVICE_TASK_CORE_AFFINITY );

                    ଯଦି( xTimerTaskHandle != ଖାଲି )
                    {
                        xReturn = pdPASS;
                    }
                }
                #ଅନ୍ୟଥା /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
                {
                    xReturn = xTaskCreateAffinitySet( prvTimerTask,
                                                      configTIMER_SERVICE_TASK_NAME,
                                                      config_TIMER_TASK_ଗଦା_DEPTH,
                                                      ଖାଲି,
                                                      ( ( UBaseType_t ) configTIMER_TASK_PRIORITY ) | portPRIVILEGE_BIT,
                                                      configTIMER_SERVICE_TASK_CORE_AFFINITY,
                                                      &xTimerTaskHandle );
                }
                #endif /* configSUPPORT_STATIC_ALLOCATION */
            }
            #ଅନ୍ୟଥା /* #if ( ( configNUMBER_OF_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            {
                #ଯଦି ( configSUPPORT_STATIC_ALLOCATION == 1 )
                {
                    StaticTask_t * pxTimerTaskTCBBuffer = ଖାଲି;
                    ଗଦା_Type_t * px_Timer_Task_ଗଦା_Buffer = ଖାଲି;
                    config_ଗଦା_DEPTH_TYPE ux_Timer_Task_ଗଦା_Size;

                    vApplicationGetTimerTaskMemory( &pxTimerTaskTCBBuffer, &px_Timer_Task_ଗଦା_Buffer, &ux_Timer_Task_ଗଦା_Size );
                    xTimerTaskHandle = xTaskCreateStatic( prvTimerTask,
                                                          configTIMER_SERVICE_TASK_NAME,
                                                          ux_Timer_Task_ଗଦା_Size,
                                                          ଖାଲି,
                                                          ( ( UBaseType_t ) configTIMER_TASK_PRIORITY ) | portPRIVILEGE_BIT,
                                                          px_Timer_Task_ଗଦା_Buffer,
                                                          pxTimerTaskTCBBuffer );

                    ଯଦି( xTimerTaskHandle != ଖାଲି )
                    {
                        xReturn = pdPASS;
                    }
                }
                #ଅନ୍ୟଥା /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
                {
                    xReturn = xTaskCreate( prvTimerTask,
                                           configTIMER_SERVICE_TASK_NAME,
                                           config_TIMER_TASK_ଗଦା_DEPTH,
                                           ଖାଲି,
                                           ( ( UBaseType_t ) configTIMER_TASK_PRIORITY ) | portPRIVILEGE_BIT,
                                           &xTimerTaskHandle );
                }
                #endif /* configSUPPORT_STATIC_ALLOCATION */
            }
            #endif /* #if ( ( configNUMBER_OF_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
        }
        ଅନ୍ୟଥା
        {
            mtCOVERAGE_TEST_MARKER();
        }

        configASSERT( xReturn );

        traceRETURN_xTimerCreateTimerTask( xReturn );

        ଫେରନ୍ତୁ xReturn;
    }
/*-----------------------------------------------------------*/

    #ଯଦି ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )

        TimerHandle_t xTimerCreate( ଅଚଳ ଅକ୍ଷର * ଅଚଳ pcTimerName,
                                    ଅଚଳ TickType_t xTimerPeriodInTicks,
                                    ଅଚଳ BaseType_t xAutoReload,
                                    ଶୂନ୍ୟ * ଅଚଳ pvTimerID,
                                    TimerCallbackFunction_t pxCallbackFunction )
        {
            Timer_t * pxNewTimer;

            traceENTER_xTimerCreate( pcTimerName, xTimerPeriodInTicks, xAutoReload, pvTimerID, pxCallbackFunction );

            /* MISRA Ref 11.5.1 [Malloc memory assignment] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
            /* coverity[misra_c_2012_rule_11_5_violation] */
            pxNewTimer = ( Timer_t * ) pvPortMalloc( ଆକାର( Timer_t ) );

            ଯଦି( pxNewTimer != ଖାଲି )
            {
                /* Status is thus far zero as the timer is not created statically
                 * and has not been started.  The auto-reload bit may get set in
                 * prvInitialiseNewTimer. */
                pxNewTimer->ucStatus = 0x00;
                prvInitialiseNewTimer( pcTimerName, xTimerPeriodInTicks, xAutoReload, pvTimerID, pxCallbackFunction, pxNewTimer );
            }

            traceRETURN_xTimerCreate( pxNewTimer );

            ଫେରନ୍ତୁ pxNewTimer;
        }

    #endif /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

    #ଯଦି ( configSUPPORT_STATIC_ALLOCATION == 1 )

        TimerHandle_t xTimerCreateStatic( ଅଚଳ ଅକ୍ଷର * ଅଚଳ pcTimerName,
                                          ଅଚଳ TickType_t xTimerPeriodInTicks,
                                          ଅଚଳ BaseType_t xAutoReload,
                                          ଶୂନ୍ୟ * ଅଚଳ pvTimerID,
                                          TimerCallbackFunction_t pxCallbackFunction,
                                          StaticTimer_t * pxTimerBuffer )
        {
            Timer_t * pxNewTimer;

            traceENTER_xTimerCreateStatic( pcTimerName, xTimerPeriodInTicks, xAutoReload, pvTimerID, pxCallbackFunction, pxTimerBuffer );

            #ଯଦି ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticTimer_t equals the size of the real timer
                 * structure. */
                ଅସ୍ଥିର size_t xSize = ଆକାର( StaticTimer_t );
                configASSERT( xSize == ଆକାର( Timer_t ) );
                ( ଶୂନ୍ୟ ) xSize; /* Prevent unused variable warning when configASSERT() is not defined. */
            }
            #endif /* configASSERT_DEFINED */

            /* A pointer to a StaticTimer_t structure MUST be provided, use it. */
            configASSERT( pxTimerBuffer );
            /* MISRA Ref 11.3.1 [Misaligned access] */
            /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
            /* coverity[misra_c_2012_rule_11_3_violation] */
            pxNewTimer = ( Timer_t * ) pxTimerBuffer;

            ଯଦି( pxNewTimer != ଖାଲି )
            {
                /* Timers can be created statically or dynamically so note this
                 * timer was created statically in case it is later deleted.  The
                 * auto-reload bit may get set in prvInitialiseNewTimer(). */
                pxNewTimer->ucStatus = ( uint8_t ) tmrSTATUS_IS_STATICALLY_ALLOCATED;

                prvInitialiseNewTimer( pcTimerName, xTimerPeriodInTicks, xAutoReload, pvTimerID, pxCallbackFunction, pxNewTimer );
            }

            traceRETURN_xTimerCreateStatic( pxNewTimer );

            ଫେରନ୍ତୁ pxNewTimer;
        }

    #endif /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

    ସ୍ଥିର ଶୂନ୍ୟ prvInitialiseNewTimer( ଅଚଳ ଅକ୍ଷର * ଅଚଳ pcTimerName,
                                       ଅଚଳ TickType_t xTimerPeriodInTicks,
                                       ଅଚଳ BaseType_t xAutoReload,
                                       ଶୂନ୍ୟ * ଅଚଳ pvTimerID,
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

        ଯଦି( xAutoReload != pdFALSE )
        {
            pxNewTimer->ucStatus |= ( uint8_t ) tmrSTATUS_IS_AUTORELOAD;
        }

        traceTIMER_CREATE( pxNewTimer );
    }
/*-----------------------------------------------------------*/

    BaseType_t xTimerGenericCommandFromTask( TimerHandle_t xTimer,
                                             ଅଚଳ BaseType_t xCommandID,
                                             ଅଚଳ TickType_t x_Optional_ମୂଲ୍ୟ,
                                             BaseType_t * ଅଚଳ pxHigherPriorityTaskWoken,
                                             ଅଚଳ TickType_t xTicksToWait )
    {
        BaseType_t xReturn = pdFAIL;
        DaemonTaskMessage_t xMessage;

        ( ଶୂନ୍ୟ ) pxHigherPriorityTaskWoken;

        traceENTER_xTimerGenericCommandFromTask( xTimer, xCommandID, x_Optional_ମୂଲ୍ୟ, pxHigherPriorityTaskWoken, xTicksToWait );

        configASSERT( xTimer );

        /* Send a message to the timer service task to perform a particular action
         * on a particular timer definition. */
        ଯଦି( xTimerQueue != ଖାଲି )
        {
            /* Send a command to the timer service task to start the xTimer timer. */
            xMessage.xMessageID = xCommandID;
            xMessage.u.xTimerParameters.x_Message_ମୂଲ୍ୟ = x_Optional_ମୂଲ୍ୟ;
            xMessage.u.xTimerParameters.pxTimer = xTimer;

            configASSERT( xCommandID < tmrFIRST_FROM_ISR_COMMAND );

            ଯଦି( xCommandID < tmrFIRST_FROM_ISR_COMMAND )
            {
                ଯଦି( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING )
                {
                    xReturn = xQueueSendToBack( xTimerQueue, &xMessage, xTicksToWait );
                }
                ଅନ୍ୟଥା
                {
                    xReturn = xQueueSendToBack( xTimerQueue, &xMessage, tmrNO_DELAY );
                }
            }

            traceTIMER_COMMAND_SEND( xTimer, xCommandID, x_Optional_ମୂଲ୍ୟ, xReturn );
        }
        ଅନ୍ୟଥା
        {
            mtCOVERAGE_TEST_MARKER();
        }

        traceRETURN_xTimerGenericCommandFromTask( xReturn );

        ଫେରନ୍ତୁ xReturn;
    }
/*-----------------------------------------------------------*/

    BaseType_t xTimerGenericCommandFromISR( TimerHandle_t xTimer,
                                            ଅଚଳ BaseType_t xCommandID,
                                            ଅଚଳ TickType_t x_Optional_ମୂଲ୍ୟ,
                                            BaseType_t * ଅଚଳ pxHigherPriorityTaskWoken,
                                            ଅଚଳ TickType_t xTicksToWait )
    {
        BaseType_t xReturn = pdFAIL;
        DaemonTaskMessage_t xMessage;

        ( ଶୂନ୍ୟ ) xTicksToWait;

        traceENTER_xTimerGenericCommandFromISR( xTimer, xCommandID, x_Optional_ମୂଲ୍ୟ, pxHigherPriorityTaskWoken, xTicksToWait );

        configASSERT( xTimer );

        /* Send a message to the timer service task to perform a particular action
         * on a particular timer definition. */
        ଯଦି( xTimerQueue != ଖାଲି )
        {
            /* Send a command to the timer service task to start the xTimer timer. */
            xMessage.xMessageID = xCommandID;
            xMessage.u.xTimerParameters.x_Message_ମୂଲ୍ୟ = x_Optional_ମୂଲ୍ୟ;
            xMessage.u.xTimerParameters.pxTimer = xTimer;

            configASSERT( xCommandID >= tmrFIRST_FROM_ISR_COMMAND );

            ଯଦି( xCommandID >= tmrFIRST_FROM_ISR_COMMAND )
            {
                xReturn = xQueueSendToBackFromISR( xTimerQueue, &xMessage, pxHigherPriorityTaskWoken );
            }

            traceTIMER_COMMAND_SEND( xTimer, xCommandID, x_Optional_ମୂଲ୍ୟ, xReturn );
        }
        ଅନ୍ୟଥା
        {
            mtCOVERAGE_TEST_MARKER();
        }

        traceRETURN_xTimerGenericCommandFromISR( xReturn );

        ଫେରନ୍ତୁ xReturn;
    }
/*-----------------------------------------------------------*/

    TaskHandle_t xTimerGetTimerDaemonTaskHandle( ଶୂନ୍ୟ )
    {
        traceENTER_xTimerGetTimerDaemonTaskHandle();

        /* If xTimerGetTimerDaemonTaskHandle() is called before the scheduler has been
         * started, then xTimerTaskHandle will be NULL. */
        configASSERT( ( xTimerTaskHandle != ଖାଲି ) );

        traceRETURN_xTimerGetTimerDaemonTaskHandle( xTimerTaskHandle );

        ଫେରନ୍ତୁ xTimerTaskHandle;
    }
/*-----------------------------------------------------------*/

    TickType_t xTimerGetPeriod( TimerHandle_t xTimer )
    {
        Timer_t * pxTimer = xTimer;

        traceENTER_xTimerGetPeriod( xTimer );

        configASSERT( xTimer );

        traceRETURN_xTimerGetPeriod( pxTimer->xTimerPeriodInTicks );

        ଫେରନ୍ତୁ pxTimer->xTimerPeriodInTicks;
    }
/*-----------------------------------------------------------*/

    ଶୂନ୍ୟ vTimerSetReloadMode( TimerHandle_t xTimer,
                              ଅଚଳ BaseType_t xAutoReload )
    {
        Timer_t * pxTimer = xTimer;

        traceENTER_vTimerSetReloadMode( xTimer, xAutoReload );

        configASSERT( xTimer );
        taskENTER_CRITICAL();
        {
            ଯଦି( xAutoReload != pdFALSE )
            {
                pxTimer->ucStatus |= ( uint8_t ) tmrSTATUS_IS_AUTORELOAD;
            }
            ଅନ୍ୟଥା
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
            ଯଦି( ( pxTimer->ucStatus & tmrSTATUS_IS_AUTORELOAD ) == 0U )
            {
                /* Not an auto-reload timer. */
                xReturn = pdFALSE;
            }
            ଅନ୍ୟଥା
            {
                /* Is an auto-reload timer. */
                xReturn = pdTRUE;
            }
        }
        taskEXIT_CRITICAL();

        traceRETURN_xTimerGetReloadMode( xReturn );

        ଫେରନ୍ତୁ xReturn;
    }

    UBaseType_t uxTimerGetReloadMode( TimerHandle_t xTimer )
    {
        UBaseType_t uxReturn;

        traceENTER_uxTimerGetReloadMode( xTimer );

        uxReturn = ( UBaseType_t ) xTimerGetReloadMode( xTimer );

        traceRETURN_uxTimerGetReloadMode( uxReturn );

        ଫେରନ୍ତୁ uxReturn;
    }
/*-----------------------------------------------------------*/

    TickType_t xTimerGetExpiryTime( TimerHandle_t xTimer )
    {
        Timer_t * pxTimer = xTimer;
        TickType_t xReturn;

        traceENTER_xTimerGetExpiryTime( xTimer );

        configASSERT( xTimer );
        xReturn = list_GET_LIST_ITEM_ମୂଲ୍ୟ( &( pxTimer->xTimerListItem ) );

        traceRETURN_xTimerGetExpiryTime( xReturn );

        ଫେରନ୍ତୁ xReturn;
    }
/*-----------------------------------------------------------*/

    #ଯଦି ( configSUPPORT_STATIC_ALLOCATION == 1 )
        BaseType_t xTimerGetStaticBuffer( TimerHandle_t xTimer,
                                          StaticTimer_t ** ppxTimerBuffer )
        {
            BaseType_t xReturn;
            Timer_t * pxTimer = xTimer;

            traceENTER_xTimerGetStaticBuffer( xTimer, ppxTimerBuffer );

            configASSERT( ppxTimerBuffer != ଖାଲି );

            ଯଦି( ( pxTimer->ucStatus & tmrSTATUS_IS_STATICALLY_ALLOCATED ) != 0U )
            {
                /* MISRA Ref 11.3.1 [Misaligned access] */
                /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-113 */
                /* coverity[misra_c_2012_rule_11_3_violation] */
                *ppxTimerBuffer = ( StaticTimer_t * ) pxTimer;
                xReturn = pdTRUE;
            }
            ଅନ୍ୟଥା
            {
                xReturn = pdFALSE;
            }

            traceRETURN_xTimerGetStaticBuffer( xReturn );

            ଫେରନ୍ତୁ xReturn;
        }
    #endif /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

    ଅଚଳ ଅକ୍ଷର * pcTimerGetName( TimerHandle_t xTimer )
    {
        Timer_t * pxTimer = xTimer;

        traceENTER_pcTimerGetName( xTimer );

        configASSERT( xTimer );

        traceRETURN_pcTimerGetName( pxTimer->pcTimerName );

        ଫେରନ୍ତୁ pxTimer->pcTimerName;
    }
/*-----------------------------------------------------------*/

    ସ୍ଥିର ଶୂନ୍ୟ prvReloadTimer( Timer_t * ଅଚଳ pxTimer,
                                TickType_t xExpiredTime,
                                ଅଚଳ TickType_t xTimeNow )
    {
        /* Insert the timer into the appropriate list for the next expiry time.
         * If the next expiry time has already passed, advance the expiry time,
         * call the callback function, and try again. */
        ଯେତେବେଳେ( prv_ଭର୍ତ୍ତି_Timer_In_Active_List( pxTimer, ( xExpiredTime + pxTimer->xTimerPeriodInTicks ), xTimeNow, xExpiredTime ) != pdFALSE )
        {
            /* Advance the expiry time. */
            xExpiredTime += pxTimer->xTimerPeriodInTicks;

            /* Call the timer callback. */
            traceTIMER_EXPIRED( pxTimer );
            pxTimer->pxCallbackFunction( ( TimerHandle_t ) pxTimer );
        }
    }
/*-----------------------------------------------------------*/

    ସ୍ଥିର ଶୂନ୍ୟ prvProcessExpiredTimer( ଅଚଳ TickType_t xNextExpireTime,
                                        ଅଚଳ TickType_t xTimeNow )
    {
        /* MISRA Ref 11.5.3 [Void pointer assignment] */
        /* More details at: https://github.com/FreeRTOS/FreeRTOS-Kernel/blob/main/MISRA.md#rule-115 */
        /* coverity[misra_c_2012_rule_11_5_violation] */
        Timer_t * ଅଚଳ pxTimer = ( Timer_t * ) listGET_OWNER_OF_HEAD_ENTRY( pxCurrentTimerList );

        /* Remove the timer from the list of active timers.  A check has already
         * been performed to ensure the list is not empty. */

        ( ଶୂନ୍ୟ ) uxListRemove( &( pxTimer->xTimerListItem ) );

        /* If the timer is an auto-reload timer then calculate the next
         * expiry time and re-insert the timer in the list of active timers. */
        ଯଦି( ( pxTimer->ucStatus & tmrSTATUS_IS_AUTORELOAD ) != 0U )
        {
            prvReloadTimer( pxTimer, xNextExpireTime, xTimeNow );
        }
        ଅନ୍ୟଥା
        {
            pxTimer->ucStatus &= ( ( uint8_t ) ~tmrSTATUS_IS_ACTIVE );
        }

        /* Call the timer callback. */
        traceTIMER_EXPIRED( pxTimer );
        pxTimer->pxCallbackFunction( ( TimerHandle_t ) pxTimer );
    }
/*-----------------------------------------------------------*/

    ସ୍ଥିର portTASK_FUNCTION( prvTimerTask, pvParameters )
    {
        TickType_t xNextExpireTime;
        BaseType_t xListWasEmpty;

        /* Just to avoid compiler warnings. */
        ( ଶୂନ୍ୟ ) pvParameters;

        #ଯଦି ( configUSE_DAEMON_TASK_STARTUP_HOOK == 1 )
        {
            /* Allow the application writer to execute some code in the context of
             * this task at the point the task starts executing.  This is useful if the
             * application includes initialisation code that would benefit from
             * executing after the scheduler has been started. */
            vApplicationDaemonTaskStartupHook();
        }
        #endif /* configUSE_DAEMON_TASK_STARTUP_HOOK */

        ପାଇଁ( ; configCONTROL_INFINITE_LOOP(); )
        {
            /* Query the timers list to see if it contains any timers, and if so,
             * obtain the time at which the next timer will expire. */
            xNextExpireTime = prvGetNextExpireTime( &xListWasEmpty );

            /* If a timer has expired, process it.  Otherwise, block this task
             * until either a timer does expire, or a command is received. */
            prvProcessTimerOrBlockTask( xNextExpireTime, xListWasEmpty );

            /* Empty the command queue. */
            prvProcessReceivedCommands();
        }
    }
/*-----------------------------------------------------------*/

    ସ୍ଥିର ଶୂନ୍ୟ prvProcessTimerOrBlockTask( ଅଚଳ TickType_t xNextExpireTime,
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

            ଯଦି( xTimerListsWereSwitched == pdFALSE )
            {
                /* The tick count has not overflowed, has the timer expired? */
                ଯଦି( ( xListWasEmpty == pdFALSE ) && ( xNextExpireTime <= xTimeNow ) )
                {
                    ( ଶୂନ୍ୟ ) xTaskResumeAll();
                    prvProcessExpiredTimer( xNextExpireTime, xTimeNow );
                }
                ଅନ୍ୟଥା
                {
                    /* The tick count has not overflowed, and the next expire
                     * time has not been reached yet.  This task should therefore
                     * block to wait for the next expire time or a command to be
                     * received - whichever comes first.  The following line cannot
                     * be reached unless xNextExpireTime > xTimeNow, except in the
                     * case when the current timer list is empty. */
                    ଯଦି( xListWasEmpty != pdFALSE )
                    {
                        /* The current timer list is empty - is the overflow list
                         * also empty? */
                        xListWasEmpty = listLIST_IS_EMPTY( pxOverflowTimerList );
                    }

                    vQueueWaitForMessageRestricted( xTimerQueue, ( xNextExpireTime - xTimeNow ), xListWasEmpty );

                    ଯଦି( xTaskResumeAll() == pdFALSE )
                    {
                        /* Yield to wait for either a command to arrive, or the
                         * block time to expire.  If a command arrived between the
                         * critical section being exited and this yield then the yield
                         * will not cause the task to block. */
                        taskYIELD_WITHIN_API();
                    }
                    ଅନ୍ୟଥା
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
            }
            ଅନ୍ୟଥା
            {
                ( ଶୂନ୍ୟ ) xTaskResumeAll();
            }
        }
    }
/*-----------------------------------------------------------*/

    ସ୍ଥିର TickType_t prvGetNextExpireTime( BaseType_t * ଅଚଳ pxListWasEmpty )
    {
        TickType_t xNextExpireTime;

        /* Timers are listed in expiry time order, with the head of the list
         * referencing the task that will expire first.  Obtain the time at which
         * the timer with the nearest expiry time will expire.  If there are no
         * active timers then just set the next expire time to 0.  That will cause
         * this task to unblock when the tick count overflows, at which point the
         * timer lists will be switched and the next expiry time can be
         * re-assessed.  */
        *pxListWasEmpty = listLIST_IS_EMPTY( pxCurrentTimerList );

        ଯଦି( *pxListWasEmpty == pdFALSE )
        {
            xNextExpireTime = list_GET_ITEM_ମୂଲ୍ୟ_OF_HEAD_ENTRY( pxCurrentTimerList );
        }
        ଅନ୍ୟଥା
        {
            /* Ensure the task unblocks when the tick count rolls over. */
            xNextExpireTime = ( TickType_t ) 0U;
        }

        ଫେରନ୍ତୁ xNextExpireTime;
    }
/*-----------------------------------------------------------*/

    ସ୍ଥିର TickType_t prvSampleTimeNow( BaseType_t * ଅଚଳ pxTimerListsWereSwitched )
    {
        TickType_t xTimeNow;
        PRIVILEGED_DATA ସ୍ଥିର TickType_t xLastTime = ( TickType_t ) 0U;

        xTimeNow = x_Task_Get_Tick_ଗଣନା();

        ଯଦି( xTimeNow < xLastTime )
        {
            prvSwitchTimerLists();
            *pxTimerListsWereSwitched = pdTRUE;
        }
        ଅନ୍ୟଥା
        {
            *pxTimerListsWereSwitched = pdFALSE;
        }

        xLastTime = xTimeNow;

        ଫେରନ୍ତୁ xTimeNow;
    }
/*-----------------------------------------------------------*/

    ସ୍ଥିର BaseType_t prv_ଭର୍ତ୍ତି_Timer_In_Active_List( Timer_t * ଅଚଳ pxTimer,
                                                  ଅଚଳ TickType_t xNextExpiryTime,
                                                  ଅଚଳ TickType_t xTimeNow,
                                                  ଅଚଳ TickType_t xCommandTime )
    {
        BaseType_t xProcessTimerNow = pdFALSE;

        list_SET_LIST_ITEM_ମୂଲ୍ୟ( &( pxTimer->xTimerListItem ), xNextExpiryTime );
        listSET_LIST_ITEM_OWNER( &( pxTimer->xTimerListItem ), pxTimer );

        ଯଦି( xNextExpiryTime <= xTimeNow )
        {
            /* Has the expiry time elapsed between the command to start/reset a
             * timer was issued, and the time the command was processed? */
            ଯଦି( ( ( TickType_t ) ( xTimeNow - xCommandTime ) ) >= pxTimer->xTimerPeriodInTicks )
            {
                /* The time between a command being issued and the command being
                 * processed actually exceeds the timers period.  */
                xProcessTimerNow = pdTRUE;
            }
            ଅନ୍ୟଥା
            {
                v_List_ଭର୍ତ୍ତି( pxOverflowTimerList, &( pxTimer->xTimerListItem ) );
            }
        }
        ଅନ୍ୟଥା
        {
            ଯଦି( ( xTimeNow < xCommandTime ) && ( xNextExpiryTime >= xCommandTime ) )
            {
                /* If, since the command was issued, the tick count has overflowed
                 * but the expiry time has not, then the timer must have already passed
                 * its expiry time and should be processed immediately. */
                xProcessTimerNow = pdTRUE;
            }
            ଅନ୍ୟଥା
            {
                v_List_ଭର୍ତ୍ତି( pxCurrentTimerList, &( pxTimer->xTimerListItem ) );
            }
        }

        ଫେରନ୍ତୁ xProcessTimerNow;
    }
/*-----------------------------------------------------------*/

    ସ୍ଥିର ଶୂନ୍ୟ prvProcessReceivedCommands( ଶୂନ୍ୟ )
    {
        DaemonTaskMessage_t xMessage = { 0 };
        Timer_t * pxTimer;
        BaseType_t xTimerListsWereSwitched;
        TickType_t xTimeNow;

        ଯେତେବେଳେ( xQueueReceive( xTimerQueue, &xMessage, tmrNO_DELAY ) != pdFAIL )
        {
            #ଯଦି ( INCLUDE_xTimerPendFunctionCall == 1 )
            {
                /* Negative commands are pended function calls rather than timer
                 * commands. */
                ଯଦି( xMessage.xMessageID < ( BaseType_t ) 0 )
                {
                    ଅଚଳ CallbackParameters_t * ଅଚଳ pxCallback = &( xMessage.u.xCallbackParameters );

                    /* The timer uses the xCallbackParameters member to request a
                     * callback be executed.  Check the callback is not NULL. */
                    configASSERT( pxCallback );

                    /* Call the function. */
                    pxCallback->pxCallbackFunction( pxCallback->pvParameter1, pxCallback->ulParameter2 );
                }
                ଅନ୍ୟଥା
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            #endif /* INCLUDE_xTimerPendFunctionCall */

            /* Commands that are positive are timer commands rather than pended
             * function calls. */
            ଯଦି( xMessage.xMessageID >= ( BaseType_t ) 0 )
            {
                /* The messages uses the xTimerParameters member to work on a
                 * software timer. */
                pxTimer = xMessage.u.xTimerParameters.pxTimer;

                ଯଦି( listIS_CONTAINED_WITHIN( ଖାଲି, &( pxTimer->xTimerListItem ) ) == pdFALSE )
                {
                    /* The timer is in a list, remove it. */
                    ( ଶୂନ୍ୟ ) uxListRemove( &( pxTimer->xTimerListItem ) );
                }
                ଅନ୍ୟଥା
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                traceTIMER_COMMAND_RECEIVED( pxTimer, xMessage.xMessageID, xMessage.u.xTimerParameters.x_Message_ମୂଲ୍ୟ );

                /* In this case the xTimerListsWereSwitched parameter is not used, but
                 *  it must be present in the function call.  prvSampleTimeNow() must be
                 *  called after the message is received from xTimerQueue so there is no
                 *  possibility of a higher priority task adding a message to the message
                 *  queue with a time that is ahead of the timer daemon task (because it
                 *  pre-empted the timer daemon task after the xTimeNow value was set). */
                xTimeNow = prvSampleTimeNow( &xTimerListsWereSwitched );

                ବାଛ( xMessage.xMessageID )
                {
                    ସ୍ଥିତି tmrCOMMAND_START:
                    ସ୍ଥିତି tmrCOMMAND_START_FROM_ISR:
                    ସ୍ଥିତି tmrCOMMAND_RESET:
                    ସ୍ଥିତି tmrCOMMAND_RESET_FROM_ISR:
                        /* Start or restart a timer. */
                        pxTimer->ucStatus |= ( uint8_t ) tmrSTATUS_IS_ACTIVE;

                        ଯଦି( prv_ଭର୍ତ୍ତି_Timer_In_Active_List( pxTimer, xMessage.u.xTimerParameters.x_Message_ମୂଲ୍ୟ + pxTimer->xTimerPeriodInTicks, xTimeNow, xMessage.u.xTimerParameters.x_Message_ମୂଲ୍ୟ ) != pdFALSE )
                        {
                            /* The timer expired before it was added to the active
                             * timer list.  Process it now. */
                            ଯଦି( ( pxTimer->ucStatus & tmrSTATUS_IS_AUTORELOAD ) != 0U )
                            {
                                prvReloadTimer( pxTimer, xMessage.u.xTimerParameters.x_Message_ମୂଲ୍ୟ + pxTimer->xTimerPeriodInTicks, xTimeNow );
                            }
                            ଅନ୍ୟଥା
                            {
                                pxTimer->ucStatus &= ( ( uint8_t ) ~tmrSTATUS_IS_ACTIVE );
                            }

                            /* Call the timer callback. */
                            traceTIMER_EXPIRED( pxTimer );
                            pxTimer->pxCallbackFunction( ( TimerHandle_t ) pxTimer );
                        }
                        ଅନ୍ୟଥା
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }

                        ଭାଙ୍ଗ;

                    ସ୍ଥିତି tmrCOMMAND_STOP:
                    ସ୍ଥିତି tmrCOMMAND_STOP_FROM_ISR:
                        /* The timer has already been removed from the active list. */
                        pxTimer->ucStatus &= ( ( uint8_t ) ~tmrSTATUS_IS_ACTIVE );
                        ଭାଙ୍ଗ;

                    ସ୍ଥିତି tmrCOMMAND_CHANGE_PERIOD:
                    ସ୍ଥିତି tmrCOMMAND_CHANGE_PERIOD_FROM_ISR:
                        pxTimer->ucStatus |= ( uint8_t ) tmrSTATUS_IS_ACTIVE;
                        pxTimer->xTimerPeriodInTicks = xMessage.u.xTimerParameters.x_Message_ମୂଲ୍ୟ;
                        configASSERT( ( pxTimer->xTimerPeriodInTicks > 0 ) );

                        /* The new period does not really have a reference, and can
                         * be longer or shorter than the old one.  The command time is
                         * therefore set to the current time, and as the period cannot
                         * be zero the next expiry time can only be in the future,
                         * meaning (unlike for the xTimerStart() case above) there is
                         * no fail case that needs to be handled here. */
                        ( ଶୂନ୍ୟ ) prv_ଭର୍ତ୍ତି_Timer_In_Active_List( pxTimer, ( xTimeNow + pxTimer->xTimerPeriodInTicks ), xTimeNow, xTimeNow );
                        ଭାଙ୍ଗ;

                    ସ୍ଥିତି tmrCOMMAND_DELETE:
                        #ଯଦି ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
                        {
                            /* The timer has already been removed from the active list,
                             * just free up the memory if the memory was dynamically
                             * allocated. */
                            ଯଦି( ( pxTimer->ucStatus & tmrSTATUS_IS_STATICALLY_ALLOCATED ) == ( uint8_t ) 0 )
                            {
                                vPortFree( pxTimer );
                            }
                            ଅନ୍ୟଥା
                            {
                                pxTimer->ucStatus &= ( ( uint8_t ) ~tmrSTATUS_IS_ACTIVE );
                            }
                        }
                        #ଅନ୍ୟଥା /* if ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) */
                        {
                            /* If dynamic allocation is not enabled, the memory
                             * could not have been dynamically allocated. So there is
                             * no need to free the memory - just mark the timer as
                             * "not active". */
                            pxTimer->ucStatus &= ( ( uint8_t ) ~tmrSTATUS_IS_ACTIVE );
                        }
                        #endif /* configSUPPORT_DYNAMIC_ALLOCATION */
                        ଭାଙ୍ଗ;

                    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
                        /* Don't expect to get here. */
                        ଭାଙ୍ଗ;
                }
            }
        }
    }
/*-----------------------------------------------------------*/

    ସ୍ଥିର ଶୂନ୍ୟ prvSwitchTimerLists( ଶୂନ୍ୟ )
    {
        TickType_t xNextExpireTime;
        List_t * pxTemp;

        /* The tick count has overflowed.  The timer lists must be switched.
         * If there are any timers still referenced from the current timer list
         * then they must have expired and should be processed before the lists
         * are switched. */
        ଯେତେବେଳେ( listLIST_IS_EMPTY( pxCurrentTimerList ) == pdFALSE )
        {
            xNextExpireTime = list_GET_ITEM_ମୂଲ୍ୟ_OF_HEAD_ENTRY( pxCurrentTimerList );

            /* Process the expired timer.  For auto-reload timers, be careful to
             * process only expirations that occur on the current list.  Further
             * expirations must wait until after the lists are switched. */
            prvProcessExpiredTimer( xNextExpireTime, tmrMAX_TIME_BEFORE_OVERFLOW );
        }

        pxTemp = pxCurrentTimerList;
        pxCurrentTimerList = pxOverflowTimerList;
        pxOverflowTimerList = pxTemp;
    }
/*-----------------------------------------------------------*/

    ସ୍ଥିର ଶୂନ୍ୟ prvCheckForValidListAndQueue( ଶୂନ୍ୟ )
    {
        /* Check that the list from which active timers are referenced, and the
         * queue used to communicate with the timer service, have been
         * initialised. */
        taskENTER_CRITICAL();
        {
            ଯଦି( xTimerQueue == ଖାଲି )
            {
                vListInitialise( &xActiveTimerList1 );
                vListInitialise( &xActiveTimerList2 );
                pxCurrentTimerList = &xActiveTimerList1;
                pxOverflowTimerList = &xActiveTimerList2;

                #ଯଦି ( configSUPPORT_STATIC_ALLOCATION == 1 )
                {
                    /* The timer queue is allocated statically in case
                     * configSUPPORT_DYNAMIC_ALLOCATION is 0. */
                    PRIVILEGED_DATA ସ୍ଥିର StaticQueue_t xStaticTimerQueue;
                    PRIVILEGED_DATA ସ୍ଥିର uint8_t ucStaticTimerQueueStorage[ ( size_t ) configTIMER_QUEUE_LENGTH * ଆକାର( DaemonTaskMessage_t ) ];

                    xTimerQueue = xQueueCreateStatic( ( UBaseType_t ) configTIMER_QUEUE_LENGTH, ( UBaseType_t ) ଆକାର( DaemonTaskMessage_t ), &( ucStaticTimerQueueStorage[ 0 ] ), &xStaticTimerQueue );
                }
                #ଅନ୍ୟଥା
                {
                    xTimerQueue = xQueueCreate( ( UBaseType_t ) configTIMER_QUEUE_LENGTH, ( UBaseType_t ) ଆକାର( DaemonTaskMessage_t ) );
                }
                #endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */

                #ଯଦି ( configQUEUE_REGISTRY_SIZE > 0 )
                {
                    ଯଦି( xTimerQueue != ଖାଲି )
                    {
                        vQueueAddToRegistry( xTimerQueue, "TmrQ" );
                    }
                    ଅନ୍ୟଥା
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #endif /* configQUEUE_REGISTRY_SIZE */
            }
            ଅନ୍ୟଥା
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
            ଯଦି( ( pxTimer->ucStatus & tmrSTATUS_IS_ACTIVE ) == 0U )
            {
                xReturn = pdFALSE;
            }
            ଅନ୍ୟଥା
            {
                xReturn = pdTRUE;
            }
        }
        taskEXIT_CRITICAL();

        traceRETURN_xTimerIsTimerActive( xReturn );

        ଫେରନ୍ତୁ xReturn;
    }
/*-----------------------------------------------------------*/

    ଶୂନ୍ୟ * pvTimerGetTimerID( ଅଚଳ TimerHandle_t xTimer )
    {
        Timer_t * ଅଚଳ pxTimer = xTimer;
        ଶୂନ୍ୟ * pvReturn;

        traceENTER_pvTimerGetTimerID( xTimer );

        configASSERT( xTimer );

        taskENTER_CRITICAL();
        {
            pvReturn = pxTimer->pvTimerID;
        }
        taskEXIT_CRITICAL();

        traceRETURN_pvTimerGetTimerID( pvReturn );

        ଫେରନ୍ତୁ pvReturn;
    }
/*-----------------------------------------------------------*/

    ଶୂନ୍ୟ vTimerSetTimerID( TimerHandle_t xTimer,
                           ଶୂନ୍ୟ * pvNewID )
    {
        Timer_t * ଅଚଳ pxTimer = xTimer;

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

    #ଯଦି ( INCLUDE_xTimerPendFunctionCall == 1 )

        BaseType_t xTimerPendFunctionCallFromISR( PendedFunction_t xFunctionToPend,
                                                  ଶୂନ୍ୟ * pvParameter1,
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

            ଫେରନ୍ତୁ xReturn;
        }

    #endif /* INCLUDE_xTimerPendFunctionCall */
/*-----------------------------------------------------------*/

    #ଯଦି ( INCLUDE_xTimerPendFunctionCall == 1 )

        BaseType_t xTimerPendFunctionCall( PendedFunction_t xFunctionToPend,
                                           ଶୂନ୍ୟ * pvParameter1,
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

            ଫେରନ୍ତୁ xReturn;
        }

    #endif /* INCLUDE_xTimerPendFunctionCall */
/*-----------------------------------------------------------*/

    #ଯଦି ( configUSE_TRACE_FACILITY == 1 )

        UBaseType_t ux_Timer_Get_Timer_ସଂଖ୍ୟା( TimerHandle_t xTimer )
        {
            trace_ENTER_ux_Timer_Get_Timer_ସଂଖ୍ୟା( xTimer );

            trace_RETURN_ux_Timer_Get_Timer_ସଂଖ୍ୟା( ( ( Timer_t * ) xTimer )->ux_Timer_ସଂଖ୍ୟା );

            ଫେରନ୍ତୁ ( ( Timer_t * ) xTimer )->ux_Timer_ସଂଖ୍ୟା;
        }

    #endif /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

    #ଯଦି ( configUSE_TRACE_FACILITY == 1 )

        ଶୂନ୍ୟ v_Timer_Set_Timer_ସଂଖ୍ୟା( TimerHandle_t xTimer,
                                   UBaseType_t ux_Timer_ସଂଖ୍ୟା )
        {
            trace_ENTER_v_Timer_Set_Timer_ସଂଖ୍ୟା( xTimer, ux_Timer_ସଂଖ୍ୟା );

            ( ( Timer_t * ) xTimer )->ux_Timer_ସଂଖ୍ୟା = ux_Timer_ସଂଖ୍ୟା;

            trace_RETURN_v_Timer_Set_Timer_ସଂଖ୍ୟା();
        }

    #endif /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

/*
 * Reset the state in this file. This state is normally initialized at start up.
 * This function must be called by the application before restarting the
 * scheduler.
 */
    ଶୂନ୍ୟ vTimerResetState( ଶୂନ୍ୟ )
    {
        xTimerQueue = ଖାଲି;
        xTimerTaskHandle = ଖାଲି;
    }
/*-----------------------------------------------------------*/

/* This entire source file will be skipped if the application is not configured
 * to include software timer functionality.  If you want to include software timer
 * functionality then ensure configUSE_TIMERS is set to 1 in FreeRTOSConfig.h. */
#endif /* configUSE_TIMERS == 1 */
