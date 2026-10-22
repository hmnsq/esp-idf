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
#समावेश <stdlib.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#घोषणा MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#समावेश "FreeRTOS.h"
#समावेश "task.h"
#समावेश "queue.h"
#समावेश "timers.h"
/* Include private IDF API additions for critical thread safety macros */
#समावेश "esp_private/freertos_idf_additions_priv.h"

#अगर ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 0 )
    #त्रुटि configUSE_TIMERS must be set to 1 to make the xTimerPendFunctionCall() function available.
#पूर्ण_अगर

/* Lint e9021, e961 and e750 are suppressed as a MISRA exception justified
 * because the MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#अघोषित MPU_WRAPPERS_INCLUDED_FROM_API_FILE /*lint !e9021 !e961 !e750. */


/* This entire source file will be skipped if the application is not configured
 * to include software timer functionality.  This #if is closed at the very bottom
 * of this file.  If you want to include software timer functionality then ensure
 * configUSE_TIMERS is set to 1 in FreeRTOSConfig.h. */
#अगर ( configUSE_TIMERS == 1 )

/* Misc definitions. */
    #घोषणा tmrNO_DELAY                    ( ( TickType_t ) 0U )
    #घोषणा tmrMAX_TIME_BEFORE_OVERFLOW    ( ( TickType_t ) -1 )

/* The name assigned to the timer service task. This can be overridden by
 * defining configTIMER_SERVICE_TASK_NAME in FreeRTOSConfig.h. */
    #अगर_अघोषित configTIMER_SERVICE_TASK_NAME
        #घोषणा configTIMER_SERVICE_TASK_NAME    "Tmr Svc"
    #पूर्ण_अगर

    #अगर ( config_संख्या_OF_CORES > 1 )

/* The core affinity assigned to the timer service task on SMP systems.
 * This can be overridden by defining configTIMER_SERVICE_TASK_CORE_AFFINITY in FreeRTOSConfig.h. */
        #अगर_अघोषित configTIMER_SERVICE_TASK_CORE_AFFINITY
            #घोषणा configTIMER_SERVICE_TASK_CORE_AFFINITY    tskNO_AFFINITY
        #पूर्ण_अगर

        #अगर ( !( ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x0 ) || ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x1 ) || ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x7FFFFFFF /* tskNO_AFFINITY */ ) ) )
            #त्रुटि Invalid Timer task affinity. Timer task core ID can only be 0x0, 0x1 or 0x7FFFFFFF (tskNO_AFFINITY).
        #पूर्ण_अगर /* #if ( !( ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x0 ) || ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x01 ) || ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x7FFFFFFF ) ) ) */
    #पूर्ण_अगर /* #if ( configNUMBER_OF_CORES > 1 ) */

/* Bit definitions used in the ucStatus member of a timer structure. */
    #घोषणा tmrSTATUS_IS_ACTIVE                  ( ( uint8_t ) 0x01 )
    #घोषणा tmrSTATUS_IS_STATICALLY_ALLOCATED    ( ( uint8_t ) 0x02 )
    #घोषणा tmrSTATUS_IS_AUTORELOAD              ( ( uint8_t ) 0x04 )

/* The definition of the timers themselves. */
    प्रकार काष्ठा tmrTimerControl                  /* The old naming convention is used to prevent breaking kernel aware debuggers. */
    {
        स्थिर अक्षर * pcTimerName;                   /*<< Text name.  This is not used by the kernel, it is included simply to make debugging easier. */ /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
        ListItem_t xTimerListItem;                  /*<< Standard linked list item as used by all kernel features for event management. */
        TickType_t xTimerPeriodInTicks;             /*<< How quickly and often the timer expires. */
        व्योम * pvTimerID;                           /*<< An ID to identify the timer.  This allows the timer to be identified when the same callback is used for multiple timers. */
        TimerCallbackFunction_t pxCallbackFunction; /*<< The function that will be called when the timer expires. */
        #अगर ( configUSE_TRACE_FACILITY == 1 )
            UBaseType_t ux_Timer_संख्या;              /*<< An ID assigned by trace tools such as FreeRTOS+Trace */
        #पूर्ण_अगर
        uint8_t ucStatus;                           /*<< Holds bits to say if the timer was statically allocated or not, and if it is active or not. */
    } xTIMER;

/* The old xTIMER name is maintained above then typedefed to the new Timer_t
 * name below to enable the use of older kernel aware debuggers. */
    प्रकार xTIMER Timer_t;

/* The definition of messages that can be sent and received on the timer queue.
 * Two types of message can be queued - messages that manipulate a software timer,
 * and messages that request the execution of a non-timer related callback.  The
 * two message types are defined in two separate structures, xTimerParametersType
 * and xCallbackParametersType respectively. */
    प्रकार काष्ठा tmrTimerParameters
    {
        TickType_t x_Message_मूल्य; /*<< An optional value used by a subset of commands, for example, when changing the period of a timer. */
        Timer_t * pxTimer;        /*<< The timer to which the command will be applied. */
    } TimerParameter_t;


    प्रकार काष्ठा tmrCallbackParameters
    {
        PendedFunction_t pxCallbackFunction; /* << The callback function to execute. */
        व्योम * pvParameter1;                 /* << The value that will be used as the callback functions first parameter. */
        uint32_t ulParameter2;               /* << The value that will be used as the callback functions second parameter. */
    } CallbackParameters_t;

/* The structure that contains the two message types, along with an identifier
 * that is used to determine which message type is valid. */
    प्रकार काष्ठा tmrTimerQueueMessage
    {
        BaseType_t xMessageID; /*<< The command being sent to the timer service task. */
        जोड़
        {
            TimerParameter_t xTimerParameters;

            /* Don't include xCallbackParameters if it is not going to be used as
             * it makes the structure (and therefore the timer queue) larger. */
            #अगर ( INCLUDE_xTimerPendFunctionCall == 1 )
                CallbackParameters_t xCallbackParameters;
            #पूर्ण_अगर /* INCLUDE_xTimerPendFunctionCall */
        } u;
    } DaemonTaskMessage_t;

/*lint -save -e956 A manual analysis and inspection has been used to determine
 * which static variables must be declared volatile. */

/* The list in which active timers are stored.  Timers are referenced in expire
 * time order, with the nearest expiry time at the front of the list.  Only the
 * timer service task is allowed to access these lists.
 * xActiveTimerList1 and xActiveTimerList2 could be at function scope but that
 * breaks some kernel aware debuggers, and debuggers that reply on removing the
 * static qualifier. */
    PRIVILEGED_तथ्य अटल List_t xActiveTimerList1;
    PRIVILEGED_तथ्य अटल List_t xActiveTimerList2;
    PRIVILEGED_तथ्य अटल List_t * pxCurrentTimerList;
    PRIVILEGED_तथ्य अटल List_t * pxOverflowTimerList;

/* A queue that is used to send commands to the timer service task. */
    PRIVILEGED_तथ्य अटल QueueHandle_t xTimerQueue = शून्य;
    PRIVILEGED_तथ्य अटल TaskHandle_t xTimerTaskHandle = शून्य;

/* Spinlock required in SMP when accessing the timers. For now we use a single lock
 * Todo: Each timer could possible have its own lock for increased granularity. */
    PRIVILEGED_तथ्य अटल portMUX_TYPE xTimerLock = portMUX_INITIALIZER_UNLOCKED;

/*lint -restore */

/*-----------------------------------------------------------*/

/*
 * Initialise the infrastructure used by the timer service task if it has not
 * been initialised already.
 */
    अटल व्योम prvCheckForValidListAndQueue( व्योम ) PRIVILEGED_FUNCTION;

/*
 * The timer service task (daemon).  Timer functionality is controlled by this
 * task.  Other tasks communicate with the timer service task using the
 * xTimerQueue queue.
 */
    अटल portTASK_FUNCTION_PROTO( prvTimerTask, pvParameters ) PRIVILEGED_FUNCTION;

/*
 * Called by the timer service task to interpret and process a command it
 * received on the timer queue.
 */
    अटल व्योम prvProcessReceivedCommands( व्योम ) PRIVILEGED_FUNCTION;

/*
 * Insert the timer into either xActiveTimerList1, or xActiveTimerList2,
 * depending on if the expire time causes a timer counter overflow.
 */
    अटल BaseType_t prv_जोड़ो_Timer_In_Active_List( Timer_t * स्थिर pxTimer,
                                                  स्थिर TickType_t x_अग्र_Expiry_Time,
                                                  स्थिर TickType_t xTimeNow,
                                                  स्थिर TickType_t xCommandTime ) PRIVILEGED_FUNCTION;

/*
 * Reload the specified auto-reload timer.  If the reloading is backlogged,
 * clear the backlog, calling the callback for each additional reload.  When
 * this function returns, the next expiry time is after xTimeNow.
 */
    अटल व्योम prvReloadTimer( Timer_t * स्थिर pxTimer,
                                TickType_t xExpiredTime,
                                स्थिर TickType_t xTimeNow ) PRIVILEGED_FUNCTION;

/*
 * An active timer has reached its expire time.  Reload the timer if it is an
 * auto-reload timer, then call its callback.
 */
    अटल व्योम prvProcessExpiredTimer( स्थिर TickType_t x_अग्र_Expire_Time,
                                        स्थिर TickType_t xTimeNow ) PRIVILEGED_FUNCTION;

/*
 * The tick count has overflowed.  Switch the timer lists after ensuring the
 * current timer list does not still reference some timers.
 */
    अटल व्योम prvSwitchTimerLists( व्योम ) PRIVILEGED_FUNCTION;

/*
 * Obtain the current tick count, setting *pxTimerListsWereSwitched to pdTRUE
 * if a tick count overflow occurred since prvSampleTimeNow() was last called.
 */
    अटल TickType_t prvSampleTimeNow( BaseType_t * स्थिर pxTimerListsWereSwitched ) PRIVILEGED_FUNCTION;

/*
 * If the timer list contains any active timers then return the expire time of
 * the timer that will expire first and set *pxListWasEmpty to false.  If the
 * timer list does not contain any timers then return 0 and set *pxListWasEmpty
 * to pdTRUE.
 */
    अटल TickType_t prv_Get_अग्र_Expire_Time( BaseType_t * स्थिर pxListWasEmpty ) PRIVILEGED_FUNCTION;

/*
 * If a timer has expired, process it.  Otherwise, block the timer service task
 * until either a timer does expire or a command is received.
 */
    अटल व्योम prvProcessTimerOrBlockTask( स्थिर TickType_t x_अग्र_Expire_Time,
                                            BaseType_t xListWasEmpty ) PRIVILEGED_FUNCTION;

/*
 * Called after a Timer_t structure has been allocated either statically or
 * dynamically to fill in the structure's members.
 */
    अटल व्योम prvInitialiseNewTimer( स्थिर अक्षर * स्थिर pcTimerName, /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                       स्थिर TickType_t xTimerPeriodInTicks,
                                       स्थिर BaseType_t xAutoReload,
                                       व्योम * स्थिर pvTimerID,
                                       TimerCallbackFunction_t pxCallbackFunction,
                                       Timer_t * pxNewTimer ) PRIVILEGED_FUNCTION;
/*-----------------------------------------------------------*/

    BaseType_t xTimerCreateTimerTask( व्योम )
    {
        BaseType_t xReturn = pdFAIL;

        /* This function is called when the scheduler is started if
         * configUSE_TIMERS is set to 1.  Check that the infrastructure used by the
         * timer service task has been created/initialised.  If timers have already
         * been created then the initialisation will already have been performed. */
        prvCheckForValidListAndQueue();

        अगर( xTimerQueue != शून्य )
        {
            /* The following code block to set the timer task core affinity has been picked up from the FreeRTOS-Kernel
             * upstream repository. Commit# 631ae9e6e4828cf5c7d58bb7ac78a64fc309bcb7.
             * Necessary changes have been made for IDF-FreeRTOS compatibility. */
            #अगर ( config_संख्या_OF_CORES > 1 )
            {
                #अगर ( configSUPPORT_STATIC_ALLOCATION == 1 )
                {
                    StaticTask_t * pxTimerTaskTCBBuffer = शून्य;
                    ढेर_Type_t * px_Timer_Task_ढेर_बफर = शून्य;
                    uint32_t ul_Timer_Task_ढेर_आकार;

                    vApplicationGetTimerTaskMemory( &pxTimerTaskTCBBuffer, &px_Timer_Task_ढेर_बफर, &ul_Timer_Task_ढेर_आकार );
                    xTimerTaskHandle = xTaskCreateStaticPinnedToCore( prvTimerTask,
                                                                      configTIMER_SERVICE_TASK_NAME,
                                                                      ul_Timer_Task_ढेर_आकार,
                                                                      शून्य,
                                                                      ( ( UBaseType_t ) configTIMER_TASK_PRIORITY ) | portPRIVILEGE_BIT,
                                                                      px_Timer_Task_ढेर_बफर,
                                                                      pxTimerTaskTCBBuffer,
                                                                      configTIMER_SERVICE_TASK_CORE_AFFINITY );

                    अगर( xTimerTaskHandle != शून्य )
                    {
                        xReturn = pdPASS;
                    }
                }
                #अन्यथा /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
                {
                    xReturn = xTaskCreatePinnedToCore( prvTimerTask,
                                                       configTIMER_SERVICE_TASK_NAME,
                                                       config_TIMER_TASK_ढेर_DEPTH,
                                                       शून्य,
                                                       ( ( UBaseType_t ) configTIMER_TASK_PRIORITY ) | portPRIVILEGE_BIT,
                                                       &xTimerTaskHandle,
                                                       configTIMER_SERVICE_TASK_CORE_AFFINITY );
                }
                #पूर्ण_अगर /* configSUPPORT_STATIC_ALLOCATION */
            }
            #अन्यथा /* #if ( configNUMBER_OF_CORES > 1 ) */
            {
                #अगर ( configSUPPORT_STATIC_ALLOCATION == 1 )
                {
                    StaticTask_t * pxTimerTaskTCBBuffer = शून्य;
                    ढेर_Type_t * px_Timer_Task_ढेर_बफर = शून्य;
                    uint32_t ul_Timer_Task_ढेर_आकार;

                    vApplicationGetTimerTaskMemory( &pxTimerTaskTCBBuffer, &px_Timer_Task_ढेर_बफर, &ul_Timer_Task_ढेर_आकार );
                    xTimerTaskHandle = xTaskCreateStatic( prvTimerTask,
                                                          configTIMER_SERVICE_TASK_NAME,
                                                          ul_Timer_Task_ढेर_आकार,
                                                          शून्य,
                                                          ( ( UBaseType_t ) configTIMER_TASK_PRIORITY ) | portPRIVILEGE_BIT,
                                                          px_Timer_Task_ढेर_बफर,
                                                          pxTimerTaskTCBBuffer );

                    अगर( xTimerTaskHandle != शून्य )
                    {
                        xReturn = pdPASS;
                    }
                }
                #अन्यथा /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
                {
                    xReturn = xTaskCreate( prvTimerTask,
                                           configTIMER_SERVICE_TASK_NAME,
                                           config_TIMER_TASK_ढेर_DEPTH,
                                           शून्य,
                                           ( ( UBaseType_t ) configTIMER_TASK_PRIORITY ) | portPRIVILEGE_BIT,
                                           &xTimerTaskHandle );
                }
                #पूर्ण_अगर /* configSUPPORT_STATIC_ALLOCATION */
            }
            #पूर्ण_अगर /* #if ( configNUMBER_OF_CORES > 1 ) */
        }
        अन्यथा
        {
            mtCOVERAGE_TEST_MARKER();
        }

        configASSERT( xReturn );
        वापस xReturn;
    }
/*-----------------------------------------------------------*/

    #अगर ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )

        TimerHandle_t xTimerCreate( स्थिर अक्षर * स्थिर pcTimerName, /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                    स्थिर TickType_t xTimerPeriodInTicks,
                                    स्थिर BaseType_t xAutoReload,
                                    व्योम * स्थिर pvTimerID,
                                    TimerCallbackFunction_t pxCallbackFunction )
        {
            Timer_t * pxNewTimer;

            pxNewTimer = ( Timer_t * ) pvPortMalloc( माप( Timer_t ) ); /*lint !e9087 !e9079 All values returned by pvPortMalloc() have at least the alignment required by the MCU's stack, and the first member of Timer_t is always a pointer to the timer's mame. */

            अगर( pxNewTimer != शून्य )
            {
                /* Status is thus far zero as the timer is not created statically
                 * and has not been started.  The auto-reload bit may get set in
                 * prvInitialiseNewTimer. */
                pxNewTimer->ucStatus = 0x00;
                prvInitialiseNewTimer( pcTimerName, xTimerPeriodInTicks, xAutoReload, pvTimerID, pxCallbackFunction, pxNewTimer );
            }

            वापस pxNewTimer;
        }

    #पूर्ण_अगर /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

    #अगर ( configSUPPORT_STATIC_ALLOCATION == 1 )

        TimerHandle_t xTimerCreateStatic( स्थिर अक्षर * स्थिर pcTimerName, /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                          स्थिर TickType_t xTimerPeriodInTicks,
                                          स्थिर BaseType_t xAutoReload,
                                          व्योम * स्थिर pvTimerID,
                                          TimerCallbackFunction_t pxCallbackFunction,
                                          StaticTimer_t * px_Timer_बफर )
        {
            Timer_t * pxNewTimer;

            #अगर ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticTimer_t equals the size of the real timer
                 * structure. */
                अस्थिर माप_प्रकार x_आकार = माप( StaticTimer_t );
                configASSERT( x_आकार == माप( Timer_t ) );
                ( व्योम ) x_आकार; /* Keeps lint quiet when configASSERT() is not defined. */
            }
            #पूर्ण_अगर /* configASSERT_DEFINED */

            /* A pointer to a StaticTimer_t structure MUST be provided, use it. */
            configASSERT( px_Timer_बफर );
            pxNewTimer = ( Timer_t * ) px_Timer_बफर; /*lint !e740 !e9087 StaticTimer_t is a pointer to a Timer_t, so guaranteed to be aligned and sized correctly (checked by an assert()), so this is safe. */

            अगर( pxNewTimer != शून्य )
            {
                /* Timers can be created statically or dynamically so note this
                 * timer was created statically in case it is later deleted.  The
                 * auto-reload bit may get set in prvInitialiseNewTimer(). */
                pxNewTimer->ucStatus = tmrSTATUS_IS_STATICALLY_ALLOCATED;

                prvInitialiseNewTimer( pcTimerName, xTimerPeriodInTicks, xAutoReload, pvTimerID, pxCallbackFunction, pxNewTimer );
            }

            वापस pxNewTimer;
        }

    #पूर्ण_अगर /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

    अटल व्योम prvInitialiseNewTimer( स्थिर अक्षर * स्थिर pcTimerName, /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                       स्थिर TickType_t xTimerPeriodInTicks,
                                       स्थिर BaseType_t xAutoReload,
                                       व्योम * स्थिर pvTimerID,
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

        अगर( xAutoReload != pdFALSE )
        {
            pxNewTimer->ucStatus |= tmrSTATUS_IS_AUTORELOAD;
        }

        traceTIMER_CREATE( pxNewTimer );
    }
/*-----------------------------------------------------------*/

    BaseType_t xTimerGenericCommand( TimerHandle_t xTimer,
                                     स्थिर BaseType_t xCommandID,
                                     स्थिर TickType_t x_Optional_मूल्य,
                                     BaseType_t * स्थिर pxHigherPriorityTaskWoken,
                                     स्थिर TickType_t xTicksToWait )
    {
        BaseType_t xReturn = pdFAIL;
        DaemonTaskMessage_t xMessage;

        configASSERT( xTimer );

        /* Send a message to the timer service task to perform a particular action
         * on a particular timer definition. */
        अगर( xTimerQueue != शून्य )
        {
            /* Send a command to the timer service task to start the xTimer timer. */
            xMessage.xMessageID = xCommandID;
            xMessage.u.xTimerParameters.x_Message_मूल्य = x_Optional_मूल्य;
            xMessage.u.xTimerParameters.pxTimer = xTimer;

            अगर( xCommandID < tmrFIRST_FROM_ISR_COMMAND )
            {
                अगर( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING )
                {
                    xReturn = xQueueSendToBack( xTimerQueue, &xMessage, xTicksToWait );
                }
                अन्यथा
                {
                    xReturn = xQueueSendToBack( xTimerQueue, &xMessage, tmrNO_DELAY );
                }
            }
            अन्यथा
            {
                xReturn = xQueueSendToBackFromISR( xTimerQueue, &xMessage, pxHigherPriorityTaskWoken );
            }

            traceTIMER_COMMAND_SEND( xTimer, xCommandID, x_Optional_मूल्य, xReturn );
        }
        अन्यथा
        {
            mtCOVERAGE_TEST_MARKER();
        }

        वापस xReturn;
    }
/*-----------------------------------------------------------*/

    TaskHandle_t xTimerGetTimerDaemonTaskHandle( व्योम )
    {
        /* If xTimerGetTimerDaemonTaskHandle() is called before the scheduler has been
         * started, then xTimerTaskHandle will be NULL. */
        configASSERT( ( xTimerTaskHandle != शून्य ) );
        वापस xTimerTaskHandle;
    }
/*-----------------------------------------------------------*/

    TickType_t xTimerGetPeriod( TimerHandle_t xTimer )
    {
        Timer_t * pxTimer = xTimer;

        configASSERT( xTimer );
        वापस pxTimer->xTimerPeriodInTicks;
    }
/*-----------------------------------------------------------*/

    व्योम vTimerSetReloadMode( TimerHandle_t xTimer,
                              स्थिर BaseType_t xAutoReload )
    {
        Timer_t * pxTimer = xTimer;

        configASSERT( xTimer );
        taskENTER_CRITICAL( &xTimerLock );
        {
            अगर( xAutoReload != pdFALSE )
            {
                pxTimer->ucStatus |= tmrSTATUS_IS_AUTORELOAD;
            }
            अन्यथा
            {
                pxTimer->ucStatus &= ( ( uint8_t ) ~tmrSTATUS_IS_AUTORELOAD );
            }
        }
        taskEXIT_CRITICAL( &xTimerLock );
    }
/*-----------------------------------------------------------*/

    BaseType_t xTimerGetReloadMode( TimerHandle_t xTimer )
    {
        Timer_t * pxTimer = xTimer;
        BaseType_t xReturn;

        configASSERT( xTimer );
        taskENTER_CRITICAL( &xTimerLock );
        {
            अगर( ( pxTimer->ucStatus & tmrSTATUS_IS_AUTORELOAD ) == 0 )
            {
                /* Not an auto-reload timer. */
                xReturn = pdFALSE;
            }
            अन्यथा
            {
                /* Is an auto-reload timer. */
                xReturn = pdTRUE;
            }
        }
        taskEXIT_CRITICAL( &xTimerLock );

        वापस xReturn;
    }

    UBaseType_t uxTimerGetReloadMode( TimerHandle_t xTimer )
    {
        वापस ( UBaseType_t ) xTimerGetReloadMode( xTimer );
    }
/*-----------------------------------------------------------*/

    TickType_t xTimerGetExpiryTime( TimerHandle_t xTimer )
    {
        Timer_t * pxTimer = xTimer;
        TickType_t xReturn;

        configASSERT( xTimer );
        xReturn = list_GET_LIST_ITEM_मूल्य( &( pxTimer->xTimerListItem ) );
        वापस xReturn;
    }
/*-----------------------------------------------------------*/

    #अगर ( configSUPPORT_STATIC_ALLOCATION == 1 )
        BaseType_t x_Timer_Get_Static_बफर( TimerHandle_t xTimer,
                                          StaticTimer_t ** ppx_Timer_बफर )
        {
            BaseType_t xReturn;
            Timer_t * pxTimer = xTimer;

            configASSERT( ppx_Timer_बफर != शून्य );

            अगर( ( pxTimer->ucStatus & tmrSTATUS_IS_STATICALLY_ALLOCATED ) != 0 )
            {
                *ppx_Timer_बफर = ( StaticTimer_t * ) pxTimer;
                xReturn = pdTRUE;
            }
            अन्यथा
            {
                xReturn = pdFALSE;
            }

            वापस xReturn;
        }
    #पूर्ण_अगर /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

    स्थिर अक्षर * pcTimerGetName( TimerHandle_t xTimer ) /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
    {
        Timer_t * pxTimer = xTimer;

        configASSERT( xTimer );
        वापस pxTimer->pcTimerName;
    }
/*-----------------------------------------------------------*/

    अटल व्योम prvReloadTimer( Timer_t * स्थिर pxTimer,
                                TickType_t xExpiredTime,
                                स्थिर TickType_t xTimeNow )
    {
        /* Insert the timer into the appropriate list for the next expiry time.
         * If the next expiry time has already passed, advance the expiry time,
         * call the callback function, and try again. */
        जबतक( prv_जोड़ो_Timer_In_Active_List( pxTimer, ( xExpiredTime + pxTimer->xTimerPeriodInTicks ), xTimeNow, xExpiredTime ) != pdFALSE )
        {
            /* Advance the expiry time. */
            xExpiredTime += pxTimer->xTimerPeriodInTicks;

            /* Call the timer callback. */
            traceTIMER_EXPIRED( pxTimer );
            pxTimer->pxCallbackFunction( ( TimerHandle_t ) pxTimer );
        }
    }
/*-----------------------------------------------------------*/

    अटल व्योम prvProcessExpiredTimer( स्थिर TickType_t x_अग्र_Expire_Time,
                                        स्थिर TickType_t xTimeNow )
    {
        Timer_t * स्थिर pxTimer = ( Timer_t * ) list_GET_OWNER_OF_शीर्ष_ENTRY( pxCurrentTimerList ); /*lint !e9087 !e9079 void * is used as this macro is used with tasks and co-routines too.  Alignment is known to be fine as the type of the pointer stored and retrieved is the same. */

        /* Remove the timer from the list of active timers.  A check has already
         * been performed to ensure the list is not empty. */

        ( व्योम ) uxListRemove( &( pxTimer->xTimerListItem ) );

        /* If the timer is an auto-reload timer then calculate the next
         * expiry time and re-insert the timer in the list of active timers. */
        अगर( ( pxTimer->ucStatus & tmrSTATUS_IS_AUTORELOAD ) != 0 )
        {
            prvReloadTimer( pxTimer, x_अग्र_Expire_Time, xTimeNow );
        }
        अन्यथा
        {
            pxTimer->ucStatus &= ( ( uint8_t ) ~tmrSTATUS_IS_ACTIVE );
        }

        /* Call the timer callback. */
        traceTIMER_EXPIRED( pxTimer );
        pxTimer->pxCallbackFunction( ( TimerHandle_t ) pxTimer );
    }
/*-----------------------------------------------------------*/

    अटल portTASK_FUNCTION( prvTimerTask, pvParameters )
    {
        TickType_t x_अग्र_Expire_Time;
        BaseType_t xListWasEmpty;

        /* Just to avoid compiler warnings. */
        ( व्योम ) pvParameters;

        #अगर ( configUSE_DAEMON_TASK_STARTUP_HOOK == 1 )
        {
            बाह्य व्योम vApplicationDaemonTaskStartupHook( व्योम );

            /* Allow the application writer to execute some code in the context of
             * this task at the point the task starts executing.  This is useful if the
             * application includes initialisation code that would benefit from
             * executing after the scheduler has been started. */
            vApplicationDaemonTaskStartupHook();
        }
        #पूर्ण_अगर /* configUSE_DAEMON_TASK_STARTUP_HOOK */

        क्रम( ; ; )
        {
            /* Query the timers list to see if it contains any timers, and if so,
             * obtain the time at which the next timer will expire. */
            x_अग्र_Expire_Time = prv_Get_अग्र_Expire_Time( &xListWasEmpty );

            /* If a timer has expired, process it.  Otherwise, block this task
             * until either a timer does expire, or a command is received. */
            prvProcessTimerOrBlockTask( x_अग्र_Expire_Time, xListWasEmpty );

            /* Empty the command queue. */
            prvProcessReceivedCommands();
        }
    }
/*-----------------------------------------------------------*/

    अटल व्योम prvProcessTimerOrBlockTask( स्थिर TickType_t x_अग्र_Expire_Time,
                                            BaseType_t xListWasEmpty )
    {
        TickType_t xTimeNow;
        BaseType_t xTimerListsWereSwitched;

        prvENTER_CRITICAL_OR_SUSPEND_ALL( &xTimerLock );
        {
            /* Obtain the time now to make an assessment as to whether the timer
             * has expired or not.  If obtaining the time causes the lists to switch
             * then don't process this timer as any timers that remained in the list
             * when the lists were switched will have been processed within the
             * prvSampleTimeNow() function. */
            xTimeNow = prvSampleTimeNow( &xTimerListsWereSwitched );

            अगर( xTimerListsWereSwitched == pdFALSE )
            {
                /* The tick count has not overflowed, has the timer expired? */
                अगर( ( xListWasEmpty == pdFALSE ) && ( x_अग्र_Expire_Time <= xTimeNow ) )
                {
                    ( व्योम ) prvEXIT_CRITICAL_OR_RESUME_ALL( &xTimerLock );
                    prvProcessExpiredTimer( x_अग्र_Expire_Time, xTimeNow );
                }
                अन्यथा
                {
                    /* The tick count has not overflowed, and the next expire
                     * time has not been reached yet.  This task should therefore
                     * block to wait for the next expire time or a command to be
                     * received - whichever comes first.  The following line cannot
                     * be reached unless xNextExpireTime > xTimeNow, except in the
                     * case when the current timer list is empty. */
                    अगर( xListWasEmpty != pdFALSE )
                    {
                        /* The current timer list is empty - is the overflow list
                         * also empty? */
                        xListWasEmpty = listLIST_IS_EMPTY( pxOverflowTimerList );
                    }

                    vQueueWaitForMessageRestricted( xTimerQueue, ( x_अग्र_Expire_Time - xTimeNow ), xListWasEmpty );

                    अगर( prvEXIT_CRITICAL_OR_RESUME_ALL( &xTimerLock ) == pdFALSE )
                    {
                        /* Yield to wait for either a command to arrive, or the
                         * block time to expire.  If a command arrived between the
                         * critical section being exited and this yield then the yield
                         * will not cause the task to block. */
                        portYIELD_WITHIN_API();
                    }
                    अन्यथा
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
            }
            अन्यथा
            {
                ( व्योम ) prvEXIT_CRITICAL_OR_RESUME_ALL( &xTimerLock );
            }
        }
    }
/*-----------------------------------------------------------*/

    अटल TickType_t prv_Get_अग्र_Expire_Time( BaseType_t * स्थिर pxListWasEmpty )
    {
        TickType_t x_अग्र_Expire_Time;

        /* Timers are listed in expiry time order, with the head of the list
         * referencing the task that will expire first.  Obtain the time at which
         * the timer with the nearest expiry time will expire.  If there are no
         * active timers then just set the next expire time to 0.  That will cause
         * this task to unblock when the tick count overflows, at which point the
         * timer lists will be switched and the next expiry time can be
         * re-assessed.  */
        *pxListWasEmpty = listLIST_IS_EMPTY( pxCurrentTimerList );

        अगर( *pxListWasEmpty == pdFALSE )
        {
            x_अग्र_Expire_Time = list_GET_ITEM_मूल्य_OF_शीर्ष_ENTRY( pxCurrentTimerList );
        }
        अन्यथा
        {
            /* Ensure the task unblocks when the tick count rolls over. */
            x_अग्र_Expire_Time = ( TickType_t ) 0U;
        }

        वापस x_अग्र_Expire_Time;
    }
/*-----------------------------------------------------------*/

    अटल TickType_t prvSampleTimeNow( BaseType_t * स्थिर pxTimerListsWereSwitched )
    {
        TickType_t xTimeNow;
        PRIVILEGED_तथ्य अटल TickType_t xLastTime = ( TickType_t ) 0U; /*lint !e956 Variable is only accessible to one task. */

        xTimeNow = x_Task_Get_Tick_गिनती();

        अगर( xTimeNow < xLastTime )
        {
            prvSwitchTimerLists();
            *pxTimerListsWereSwitched = pdTRUE;
        }
        अन्यथा
        {
            *pxTimerListsWereSwitched = pdFALSE;
        }

        xLastTime = xTimeNow;

        वापस xTimeNow;
    }
/*-----------------------------------------------------------*/

    अटल BaseType_t prv_जोड़ो_Timer_In_Active_List( Timer_t * स्थिर pxTimer,
                                                  स्थिर TickType_t x_अग्र_Expiry_Time,
                                                  स्थिर TickType_t xTimeNow,
                                                  स्थिर TickType_t xCommandTime )
    {
        BaseType_t xProcessTimerNow = pdFALSE;

        list_SET_LIST_ITEM_मूल्य( &( pxTimer->xTimerListItem ), x_अग्र_Expiry_Time );
        listSET_LIST_ITEM_OWNER( &( pxTimer->xTimerListItem ), pxTimer );

        अगर( x_अग्र_Expiry_Time <= xTimeNow )
        {
            /* Has the expiry time elapsed between the command to start/reset a
             * timer was issued, and the time the command was processed? */
            अगर( ( ( TickType_t ) ( xTimeNow - xCommandTime ) ) >= pxTimer->xTimerPeriodInTicks ) /*lint !e961 MISRA exception as the casts are only redundant for some ports. */
            {
                /* The time between a command being issued and the command being
                 * processed actually exceeds the timers period.  */
                xProcessTimerNow = pdTRUE;
            }
            अन्यथा
            {
                v_List_जोड़ो( pxOverflowTimerList, &( pxTimer->xTimerListItem ) );
            }
        }
        अन्यथा
        {
            अगर( ( xTimeNow < xCommandTime ) && ( x_अग्र_Expiry_Time >= xCommandTime ) )
            {
                /* If, since the command was issued, the tick count has overflowed
                 * but the expiry time has not, then the timer must have already passed
                 * its expiry time and should be processed immediately. */
                xProcessTimerNow = pdTRUE;
            }
            अन्यथा
            {
                v_List_जोड़ो( pxCurrentTimerList, &( pxTimer->xTimerListItem ) );
            }
        }

        वापस xProcessTimerNow;
    }
/*-----------------------------------------------------------*/

    अटल व्योम prvProcessReceivedCommands( व्योम )
    {
        DaemonTaskMessage_t xMessage;
        Timer_t * pxTimer;
        BaseType_t xTimerListsWereSwitched;
        TickType_t xTimeNow;

        जबतक( xQueueReceive( xTimerQueue, &xMessage, tmrNO_DELAY ) != pdFAIL ) /*lint !e603 xMessage does not have to be initialised as it is passed out, not in, and it is not used unless xQueueReceive() returns pdTRUE. */
        {
            #अगर ( INCLUDE_xTimerPendFunctionCall == 1 )
            {
                /* Negative commands are pended function calls rather than timer
                 * commands. */
                अगर( xMessage.xMessageID < ( BaseType_t ) 0 )
                {
                    स्थिर CallbackParameters_t * स्थिर pxCallback = &( xMessage.u.xCallbackParameters );

                    /* The timer uses the xCallbackParameters member to request a
                     * callback be executed.  Check the callback is not NULL. */
                    configASSERT( pxCallback );

                    /* Call the function. */
                    pxCallback->pxCallbackFunction( pxCallback->pvParameter1, pxCallback->ulParameter2 );
                }
                अन्यथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            #पूर्ण_अगर /* INCLUDE_xTimerPendFunctionCall */

            /* Commands that are positive are timer commands rather than pended
             * function calls. */
            अगर( xMessage.xMessageID >= ( BaseType_t ) 0 )
            {
                /* The messages uses the xTimerParameters member to work on a
                 * software timer. */
                pxTimer = xMessage.u.xTimerParameters.pxTimer;

                अगर( listIS_CONTAINED_WITHIN( शून्य, &( pxTimer->xTimerListItem ) ) == pdFALSE ) /*lint !e961. The cast is only redundant when NULL is passed into the macro. */
                {
                    /* The timer is in a list, remove it. */
                    ( व्योम ) uxListRemove( &( pxTimer->xTimerListItem ) );
                }
                अन्यथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                traceTIMER_COMMAND_RECEIVED( pxTimer, xMessage.xMessageID, xMessage.u.xTimerParameters.x_Message_मूल्य );

                /* In this case the xTimerListsWereSwitched parameter is not used, but
                 *  it must be present in the function call.  prvSampleTimeNow() must be
                 *  called after the message is received from xTimerQueue so there is no
                 *  possibility of a higher priority task adding a message to the message
                 *  queue with a time that is ahead of the timer daemon task (because it
                 *  pre-empted the timer daemon task after the xTimeNow value was set). */
                xTimeNow = prvSampleTimeNow( &xTimerListsWereSwitched );

                चयन( xMessage.xMessageID )
                {
                    हाल tmrCOMMAND_START:
                    हाल tmrCOMMAND_START_FROM_ISR:
                    हाल tmrCOMMAND_RESET:
                    हाल tmrCOMMAND_RESET_FROM_ISR:
                        /* Start or restart a timer. */
                        pxTimer->ucStatus |= tmrSTATUS_IS_ACTIVE;

                        अगर( prv_जोड़ो_Timer_In_Active_List( pxTimer, xMessage.u.xTimerParameters.x_Message_मूल्य + pxTimer->xTimerPeriodInTicks, xTimeNow, xMessage.u.xTimerParameters.x_Message_मूल्य ) != pdFALSE )
                        {
                            /* The timer expired before it was added to the active
                             * timer list.  Process it now. */
                            अगर( ( pxTimer->ucStatus & tmrSTATUS_IS_AUTORELOAD ) != 0 )
                            {
                                prvReloadTimer( pxTimer, xMessage.u.xTimerParameters.x_Message_मूल्य + pxTimer->xTimerPeriodInTicks, xTimeNow );
                            }
                            अन्यथा
                            {
                                pxTimer->ucStatus &= ( ( uint8_t ) ~tmrSTATUS_IS_ACTIVE );
                            }

                            /* Call the timer callback. */
                            traceTIMER_EXPIRED( pxTimer );
                            pxTimer->pxCallbackFunction( ( TimerHandle_t ) pxTimer );
                        }
                        अन्यथा
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }

                        अवरोध;

                    हाल tmrCOMMAND_STOP:
                    हाल tmrCOMMAND_STOP_FROM_ISR:
                        /* The timer has already been removed from the active list. */
                        pxTimer->ucStatus &= ( ( uint8_t ) ~tmrSTATUS_IS_ACTIVE );
                        अवरोध;

                    हाल tmrCOMMAND_CHANGE_PERIOD:
                    हाल tmrCOMMAND_CHANGE_PERIOD_FROM_ISR:
                        pxTimer->ucStatus |= tmrSTATUS_IS_ACTIVE;
                        pxTimer->xTimerPeriodInTicks = xMessage.u.xTimerParameters.x_Message_मूल्य;
                        configASSERT( ( pxTimer->xTimerPeriodInTicks > 0 ) );

                        /* The new period does not really have a reference, and can
                         * be longer or shorter than the old one.  The command time is
                         * therefore set to the current time, and as the period cannot
                         * be zero the next expiry time can only be in the future,
                         * meaning (unlike for the xTimerStart() case above) there is
                         * no fail case that needs to be handled here. */
                        ( व्योम ) prv_जोड़ो_Timer_In_Active_List( pxTimer, ( xTimeNow + pxTimer->xTimerPeriodInTicks ), xTimeNow, xTimeNow );
                        अवरोध;

                    हाल tmrCOMMAND_DELETE:
                        #अगर ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
                        {
                            /* The timer has already been removed from the active list,
                             * just free up the memory if the memory was dynamically
                             * allocated. */
                            अगर( ( pxTimer->ucStatus & tmrSTATUS_IS_STATICALLY_ALLOCATED ) == ( uint8_t ) 0 )
                            {
                                vPortFree( pxTimer );
                            }
                            अन्यथा
                            {
                                pxTimer->ucStatus &= ( ( uint8_t ) ~tmrSTATUS_IS_ACTIVE );
                            }
                        }
                        #अन्यथा /* if ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) */
                        {
                            /* If dynamic allocation is not enabled, the memory
                             * could not have been dynamically allocated. So there is
                             * no need to free the memory - just mark the timer as
                             * "not active". */
                            pxTimer->ucStatus &= ( ( uint8_t ) ~tmrSTATUS_IS_ACTIVE );
                        }
                        #पूर्ण_अगर /* configSUPPORT_DYNAMIC_ALLOCATION */
                        अवरोध;

                    शेष:
                        /* Don't expect to get here. */
                        अवरोध;
                }
            }
        }
    }
/*-----------------------------------------------------------*/

    अटल व्योम prvSwitchTimerLists( व्योम )
    {
        TickType_t x_अग्र_Expire_Time;
        List_t * px_अस्थायी;

        /* The tick count has overflowed.  The timer lists must be switched.
         * If there are any timers still referenced from the current timer list
         * then they must have expired and should be processed before the lists
         * are switched. */
        जबतक( listLIST_IS_EMPTY( pxCurrentTimerList ) == pdFALSE )
        {
            x_अग्र_Expire_Time = list_GET_ITEM_मूल्य_OF_शीर्ष_ENTRY( pxCurrentTimerList );

            /* Process the expired timer.  For auto-reload timers, be careful to
             * process only expirations that occur on the current list.  Further
             * expirations must wait until after the lists are switched. */
            prvProcessExpiredTimer( x_अग्र_Expire_Time, tmrMAX_TIME_BEFORE_OVERFLOW );
        }

        px_अस्थायी = pxCurrentTimerList;
        pxCurrentTimerList = pxOverflowTimerList;
        pxOverflowTimerList = px_अस्थायी;
    }
/*-----------------------------------------------------------*/

    अटल व्योम prvCheckForValidListAndQueue( व्योम )
    {
        /* Check that the list from which active timers are referenced, and the
         * queue used to communicate with the timer service, have been
         * initialised. */
        taskENTER_CRITICAL( &xTimerLock );
        {
            अगर( xTimerQueue == शून्य )
            {
                vListInitialise( &xActiveTimerList1 );
                vListInitialise( &xActiveTimerList2 );
                pxCurrentTimerList = &xActiveTimerList1;
                pxOverflowTimerList = &xActiveTimerList2;

                #अगर ( configSUPPORT_STATIC_ALLOCATION == 1 )
                {
                    /* The timer queue is allocated statically in case
                     * configSUPPORT_DYNAMIC_ALLOCATION is 0. */
                    PRIVILEGED_तथ्य अटल StaticQueue_t xStaticTimerQueue;                                                                          /*lint !e956 Ok to declare in this manner to prevent additional conditional compilation guards in other locations. */
                    PRIVILEGED_तथ्य अटल uint8_t ucStaticTimerQueueStorage[ ( माप_प्रकार ) config_TIMER_QUEUE_लंबाई * माप( DaemonTaskMessage_t ) ]; /*lint !e956 Ok to declare in this manner to prevent additional conditional compilation guards in other locations. */

                    xTimerQueue = xQueueCreateStatic( ( UBaseType_t ) config_TIMER_QUEUE_लंबाई, ( UBaseType_t ) माप( DaemonTaskMessage_t ), &( ucStaticTimerQueueStorage[ 0 ] ), &xStaticTimerQueue );
                }
                #अन्यथा
                {
                    xTimerQueue = xQueueCreate( ( UBaseType_t ) config_TIMER_QUEUE_लंबाई, माप( DaemonTaskMessage_t ) );
                }
                #पूर्ण_अगर /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */

                #अगर ( config_QUEUE_REGISTRY_आकार > 0 )
                {
                    अगर( xTimerQueue != शून्य )
                    {
                        vQueueAddToRegistry( xTimerQueue, "TmrQ" );
                    }
                    अन्यथा
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #पूर्ण_अगर /* configQUEUE_REGISTRY_SIZE */
            }
            अन्यथा
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        taskEXIT_CRITICAL( &xTimerLock );
    }
/*-----------------------------------------------------------*/

    BaseType_t xTimerIsTimerActive( TimerHandle_t xTimer )
    {
        BaseType_t xReturn;
        Timer_t * pxTimer = xTimer;

        configASSERT( xTimer );

        /* Is the timer in the list of active timers? */
        taskENTER_CRITICAL( &xTimerLock );
        {
            अगर( ( pxTimer->ucStatus & tmrSTATUS_IS_ACTIVE ) == 0 )
            {
                xReturn = pdFALSE;
            }
            अन्यथा
            {
                xReturn = pdTRUE;
            }
        }
        taskEXIT_CRITICAL( &xTimerLock );

        वापस xReturn;
    } /*lint !e818 Can't be pointer to const due to the typedef. */
/*-----------------------------------------------------------*/

    व्योम * pvTimerGetTimerID( स्थिर TimerHandle_t xTimer )
    {
        Timer_t * स्थिर pxTimer = xTimer;
        व्योम * pvReturn;

        configASSERT( xTimer );

        taskENTER_CRITICAL( &xTimerLock );
        {
            pvReturn = pxTimer->pvTimerID;
        }
        taskEXIT_CRITICAL( &xTimerLock );

        वापस pvReturn;
    }
/*-----------------------------------------------------------*/

    व्योम vTimerSetTimerID( TimerHandle_t xTimer,
                           व्योम * pvNewID )
    {
        Timer_t * स्थिर pxTimer = xTimer;

        configASSERT( xTimer );

        taskENTER_CRITICAL( &xTimerLock );
        {
            pxTimer->pvTimerID = pvNewID;
        }
        taskEXIT_CRITICAL( &xTimerLock );
    }
/*-----------------------------------------------------------*/

    #अगर ( INCLUDE_xTimerPendFunctionCall == 1 )

        BaseType_t xTimerPendFunctionCallFromISR( PendedFunction_t xFunctionToPend,
                                                  व्योम * pvParameter1,
                                                  uint32_t ulParameter2,
                                                  BaseType_t * pxHigherPriorityTaskWoken )
        {
            DaemonTaskMessage_t xMessage;
            BaseType_t xReturn;

            /* Complete the message with the function parameters and post it to the
             * daemon task. */
            xMessage.xMessageID = tmrCOMMAND_EXECUTE_CALLBACK_FROM_ISR;
            xMessage.u.xCallbackParameters.pxCallbackFunction = xFunctionToPend;
            xMessage.u.xCallbackParameters.pvParameter1 = pvParameter1;
            xMessage.u.xCallbackParameters.ulParameter2 = ulParameter2;

            xReturn = xQueueSendFromISR( xTimerQueue, &xMessage, pxHigherPriorityTaskWoken );

            tracePEND_FUNC_CALL_FROM_ISR( xFunctionToPend, pvParameter1, ulParameter2, xReturn );

            वापस xReturn;
        }

    #पूर्ण_अगर /* INCLUDE_xTimerPendFunctionCall */
/*-----------------------------------------------------------*/

    #अगर ( INCLUDE_xTimerPendFunctionCall == 1 )

        BaseType_t xTimerPendFunctionCall( PendedFunction_t xFunctionToPend,
                                           व्योम * pvParameter1,
                                           uint32_t ulParameter2,
                                           TickType_t xTicksToWait )
        {
            DaemonTaskMessage_t xMessage;
            BaseType_t xReturn;

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

            वापस xReturn;
        }

    #पूर्ण_अगर /* INCLUDE_xTimerPendFunctionCall */
/*-----------------------------------------------------------*/

    #अगर ( configUSE_TRACE_FACILITY == 1 )

        UBaseType_t ux_Timer_Get_Timer_संख्या( TimerHandle_t xTimer )
        {
            वापस ( ( Timer_t * ) xTimer )->ux_Timer_संख्या;
        }

    #पूर्ण_अगर /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

    #अगर ( configUSE_TRACE_FACILITY == 1 )

        व्योम v_Timer_Set_Timer_संख्या( TimerHandle_t xTimer,
                                   UBaseType_t ux_Timer_संख्या )
        {
            ( ( Timer_t * ) xTimer )->ux_Timer_संख्या = ux_Timer_संख्या;
        }

    #पूर्ण_अगर /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

/* This entire source file will be skipped if the application is not configured
 * to include software timer functionality.  If you want to include software timer
 * functionality then ensure configUSE_TIMERS is set to 1 in FreeRTOSConfig.h. */
#पूर्ण_अगर /* configUSE_TIMERS == 1 */
