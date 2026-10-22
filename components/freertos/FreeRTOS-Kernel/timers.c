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
#અન્તર્ભાવ <stdlib.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#define MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#અન્તર્ભાવ "FreeRTOS.h"
#અન્તર્ભાવ "task.h"
#અન્તર્ભાવ "queue.h"
#અન્તર્ભાવ "timers.h"
/* Include private IDF API additions for critical thread safety macros */
#અન્તર્ભાવ "esp_private/freertos_idf_additions_priv.h"

#જો ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 0 )
    #error configUSE_TIMERS must be set to 1 to make the xTimerPendFunctionCall() function available.
#endif

/* Lint e9021, e961 and e750 are suppressed as a MISRA exception justified
 * because the MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#undef MPU_WRAPPERS_INCLUDED_FROM_API_FILE /*lint !e9021 !e961 !e750. */


/* This entire source file will be skipped if the application is not configured
 * to include software timer functionality.  This #if is closed at the very bottom
 * of this file.  If you want to include software timer functionality then ensure
 * configUSE_TIMERS is set to 1 in FreeRTOSConfig.h. */
#જો ( configUSE_TIMERS == 1 )

/* Misc definitions. */
    #define tmrNO_DELAY                    ( ( TickType_t ) 0U )
    #define tmrMAX_TIME_BEFORE_OVERFLOW    ( ( TickType_t ) -1 )

/* The name assigned to the timer service task. This can be overridden by
 * defining configTIMER_SERVICE_TASK_NAME in FreeRTOSConfig.h. */
    #ifndef configTIMER_SERVICE_TASK_NAME
        #define configTIMER_SERVICE_TASK_NAME    "Tmr Svc"
    #endif

    #જો ( config_સંખ્યા_OF_CORES > 1 )

/* The core affinity assigned to the timer service task on SMP systems.
 * This can be overridden by defining configTIMER_SERVICE_TASK_CORE_AFFINITY in FreeRTOSConfig.h. */
        #ifndef configTIMER_SERVICE_TASK_CORE_AFFINITY
            #define configTIMER_SERVICE_TASK_CORE_AFFINITY    tskNO_AFFINITY
        #endif

        #જો ( !( ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x0 ) || ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x1 ) || ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x7FFFFFFF /* tskNO_AFFINITY */ ) ) )
            #error Invalid Timer task affinity. Timer task core ID can only be 0x0, 0x1 or 0x7FFFFFFF (tskNO_AFFINITY).
        #endif /* #if ( !( ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x0 ) || ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x01 ) || ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x7FFFFFFF ) ) ) */
    #endif /* #if ( configNUMBER_OF_CORES > 1 ) */

/* Bit definitions used in the ucStatus member of a timer structure. */
    #define tmrSTATUS_IS_ACTIVE                  ( ( uint8_t ) 0x01 )
    #define tmrSTATUS_IS_STATICALLY_ALLOCATED    ( ( uint8_t ) 0x02 )
    #define tmrSTATUS_IS_AUTORELOAD              ( ( uint8_t ) 0x04 )

/* The definition of the timers themselves. */
    પ્રકારવ્યાખ્યા સંરચના tmrTimerControl                  /* The old naming convention is used to prevent breaking kernel aware debuggers. */
    {
        અચલ અક્ષર * pcTimerName;                   /*<< Text name.  This is not used by the kernel, it is included simply to make debugging easier. */ /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
        ListItem_t xTimerListItem;                  /*<< Standard linked list item as used by all kernel features for event management. */
        TickType_t xTimerPeriodInTicks;             /*<< How quickly and often the timer expires. */
        શૂન્ય * pvTimerID;                           /*<< An ID to identify the timer.  This allows the timer to be identified when the same callback is used for multiple timers. */
        TimerCallbackFunction_t pxCallbackFunction; /*<< The function that will be called when the timer expires. */
        #જો ( configUSE_TRACE_FACILITY == 1 )
            UBaseType_t ux_Timer_સંખ્યા;              /*<< An ID assigned by trace tools such as FreeRTOS+Trace */
        #endif
        uint8_t ucStatus;                           /*<< Holds bits to say if the timer was statically allocated or not, and if it is active or not. */
    } xTIMER;

/* The old xTIMER name is maintained above then typedefed to the new Timer_t
 * name below to enable the use of older kernel aware debuggers. */
    પ્રકારવ્યાખ્યા xTIMER Timer_t;

/* The definition of messages that can be sent and received on the timer queue.
 * Two types of message can be queued - messages that manipulate a software timer,
 * and messages that request the execution of a non-timer related callback.  The
 * two message types are defined in two separate structures, xTimerParametersType
 * and xCallbackParametersType respectively. */
    પ્રકારવ્યાખ્યા સંરચના tmrTimerParameters
    {
        TickType_t x_Message_કિંમત; /*<< An optional value used by a subset of commands, for example, when changing the period of a timer. */
        Timer_t * pxTimer;        /*<< The timer to which the command will be applied. */
    } TimerParameter_t;


    પ્રકારવ્યાખ્યા સંરચના tmrCallbackParameters
    {
        PendedFunction_t pxCallbackFunction; /* << The callback function to execute. */
        શૂન્ય * pvParameter1;                 /* << The value that will be used as the callback functions first parameter. */
        uint32_t ulParameter2;               /* << The value that will be used as the callback functions second parameter. */
    } CallbackParameters_t;

/* The structure that contains the two message types, along with an identifier
 * that is used to determine which message type is valid. */
    પ્રકારવ્યાખ્યા સંરચના tmrTimerQueueMessage
    {
        BaseType_t xMessageID; /*<< The command being sent to the timer service task. */
        સંઘ
        {
            TimerParameter_t xTimerParameters;

            /* Don't include xCallbackParameters if it is not going to be used as
             * it makes the structure (and therefore the timer queue) larger. */
            #જો ( INCLUDE_xTimerPendFunctionCall == 1 )
                CallbackParameters_t xCallbackParameters;
            #endif /* INCLUDE_xTimerPendFunctionCall */
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
    PRIVILEGED_DATA સ્થિર List_t xActiveTimerList1;
    PRIVILEGED_DATA સ્થિર List_t xActiveTimerList2;
    PRIVILEGED_DATA સ્થિર List_t * pxCurrentTimerList;
    PRIVILEGED_DATA સ્થિર List_t * pxOverflowTimerList;

/* A queue that is used to send commands to the timer service task. */
    PRIVILEGED_DATA સ્થિર QueueHandle_t xTimerQueue = ખાલી;
    PRIVILEGED_DATA સ્થિર TaskHandle_t xTimerTaskHandle = ખાલી;

/* Spinlock required in SMP when accessing the timers. For now we use a single lock
 * Todo: Each timer could possible have its own lock for increased granularity. */
    PRIVILEGED_DATA સ્થિર portMUX_TYPE xTimerLock = portMUX_INITIALIZER_UNLOCKED;

/*lint -restore */

/*-----------------------------------------------------------*/

/*
 * Initialise the infrastructure used by the timer service task if it has not
 * been initialised already.
 */
    સ્થિર શૂન્ય prvCheckForValidListAndQueue( શૂન્ય ) PRIVILEGED_FUNCTION;

/*
 * The timer service task (daemon).  Timer functionality is controlled by this
 * task.  Other tasks communicate with the timer service task using the
 * xTimerQueue queue.
 */
    સ્થિર portTASK_FUNCTION_PROTO( prvTimerTask, pvParameters ) PRIVILEGED_FUNCTION;

/*
 * Called by the timer service task to interpret and process a command it
 * received on the timer queue.
 */
    સ્થિર શૂન્ય prvProcessReceivedCommands( શૂન્ય ) PRIVILEGED_FUNCTION;

/*
 * Insert the timer into either xActiveTimerList1, or xActiveTimerList2,
 * depending on if the expire time causes a timer counter overflow.
 */
    સ્થિર BaseType_t prv_ઉમેરો_Timer_In_Active_List( Timer_t * અચલ pxTimer,
                                                  અચલ TickType_t xNextExpiryTime,
                                                  અચલ TickType_t xTimeNow,
                                                  અચલ TickType_t xCommandTime ) PRIVILEGED_FUNCTION;

/*
 * Reload the specified auto-reload timer.  If the reloading is backlogged,
 * clear the backlog, calling the callback for each additional reload.  When
 * this function returns, the next expiry time is after xTimeNow.
 */
    સ્થિર શૂન્ય prvReloadTimer( Timer_t * અચલ pxTimer,
                                TickType_t xExpiredTime,
                                અચલ TickType_t xTimeNow ) PRIVILEGED_FUNCTION;

/*
 * An active timer has reached its expire time.  Reload the timer if it is an
 * auto-reload timer, then call its callback.
 */
    સ્થિર શૂન્ય prvProcessExpiredTimer( અચલ TickType_t xNextExpireTime,
                                        અચલ TickType_t xTimeNow ) PRIVILEGED_FUNCTION;

/*
 * The tick count has overflowed.  Switch the timer lists after ensuring the
 * current timer list does not still reference some timers.
 */
    સ્થિર શૂન્ય prvSwitchTimerLists( શૂન્ય ) PRIVILEGED_FUNCTION;

/*
 * Obtain the current tick count, setting *pxTimerListsWereSwitched to pdTRUE
 * if a tick count overflow occurred since prvSampleTimeNow() was last called.
 */
    સ્થિર TickType_t prvSampleTimeNow( BaseType_t * અચલ pxTimerListsWereSwitched ) PRIVILEGED_FUNCTION;

/*
 * If the timer list contains any active timers then return the expire time of
 * the timer that will expire first and set *pxListWasEmpty to false.  If the
 * timer list does not contain any timers then return 0 and set *pxListWasEmpty
 * to pdTRUE.
 */
    સ્થિર TickType_t prvGetNextExpireTime( BaseType_t * અચલ pxListWasEmpty ) PRIVILEGED_FUNCTION;

/*
 * If a timer has expired, process it.  Otherwise, block the timer service task
 * until either a timer does expire or a command is received.
 */
    સ્થિર શૂન્ય prvProcessTimerOrBlockTask( અચલ TickType_t xNextExpireTime,
                                            BaseType_t xListWasEmpty ) PRIVILEGED_FUNCTION;

/*
 * Called after a Timer_t structure has been allocated either statically or
 * dynamically to fill in the structure's members.
 */
    સ્થિર શૂન્ય prvInitialiseNewTimer( અચલ અક્ષર * અચલ pcTimerName, /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                       અચલ TickType_t xTimerPeriodInTicks,
                                       અચલ BaseType_t xAutoReload,
                                       શૂન્ય * અચલ pvTimerID,
                                       TimerCallbackFunction_t pxCallbackFunction,
                                       Timer_t * pxNewTimer ) PRIVILEGED_FUNCTION;
/*-----------------------------------------------------------*/

    BaseType_t xTimerCreateTimerTask( શૂન્ય )
    {
        BaseType_t xReturn = pdFAIL;

        /* This function is called when the scheduler is started if
         * configUSE_TIMERS is set to 1.  Check that the infrastructure used by the
         * timer service task has been created/initialised.  If timers have already
         * been created then the initialisation will already have been performed. */
        prvCheckForValidListAndQueue();

        જો( xTimerQueue != ખાલી )
        {
            /* The following code block to set the timer task core affinity has been picked up from the FreeRTOS-Kernel
             * upstream repository. Commit# 631ae9e6e4828cf5c7d58bb7ac78a64fc309bcb7.
             * Necessary changes have been made for IDF-FreeRTOS compatibility. */
            #જો ( config_સંખ્યા_OF_CORES > 1 )
            {
                #જો ( configSUPPORT_STATIC_ALLOCATION == 1 )
                {
                    StaticTask_t * pxTimerTaskTCBBuffer = ખાલી;
                    ઢગલો_Type_t * px_Timer_Task_ઢગલો_Buffer = ખાલી;
                    uint32_t ul_Timer_Task_ઢગલો_Size;

                    vApplicationGetTimerTaskMemory( &pxTimerTaskTCBBuffer, &px_Timer_Task_ઢગલો_Buffer, &ul_Timer_Task_ઢગલો_Size );
                    xTimerTaskHandle = xTaskCreateStaticPinnedToCore( prvTimerTask,
                                                                      configTIMER_SERVICE_TASK_NAME,
                                                                      ul_Timer_Task_ઢગલો_Size,
                                                                      ખાલી,
                                                                      ( ( UBaseType_t ) configTIMER_TASK_PRIORITY ) | portPRIVILEGE_BIT,
                                                                      px_Timer_Task_ઢગલો_Buffer,
                                                                      pxTimerTaskTCBBuffer,
                                                                      configTIMER_SERVICE_TASK_CORE_AFFINITY );

                    જો( xTimerTaskHandle != ખાલી )
                    {
                        xReturn = pdPASS;
                    }
                }
                #નહીંતર /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
                {
                    xReturn = xTaskCreatePinnedToCore( prvTimerTask,
                                                       configTIMER_SERVICE_TASK_NAME,
                                                       config_TIMER_TASK_ઢગલો_DEPTH,
                                                       ખાલી,
                                                       ( ( UBaseType_t ) configTIMER_TASK_PRIORITY ) | portPRIVILEGE_BIT,
                                                       &xTimerTaskHandle,
                                                       configTIMER_SERVICE_TASK_CORE_AFFINITY );
                }
                #endif /* configSUPPORT_STATIC_ALLOCATION */
            }
            #નહીંતર /* #if ( configNUMBER_OF_CORES > 1 ) */
            {
                #જો ( configSUPPORT_STATIC_ALLOCATION == 1 )
                {
                    StaticTask_t * pxTimerTaskTCBBuffer = ખાલી;
                    ઢગલો_Type_t * px_Timer_Task_ઢગલો_Buffer = ખાલી;
                    uint32_t ul_Timer_Task_ઢગલો_Size;

                    vApplicationGetTimerTaskMemory( &pxTimerTaskTCBBuffer, &px_Timer_Task_ઢગલો_Buffer, &ul_Timer_Task_ઢગલો_Size );
                    xTimerTaskHandle = xTaskCreateStatic( prvTimerTask,
                                                          configTIMER_SERVICE_TASK_NAME,
                                                          ul_Timer_Task_ઢગલો_Size,
                                                          ખાલી,
                                                          ( ( UBaseType_t ) configTIMER_TASK_PRIORITY ) | portPRIVILEGE_BIT,
                                                          px_Timer_Task_ઢગલો_Buffer,
                                                          pxTimerTaskTCBBuffer );

                    જો( xTimerTaskHandle != ખાલી )
                    {
                        xReturn = pdPASS;
                    }
                }
                #નહીંતર /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
                {
                    xReturn = xTaskCreate( prvTimerTask,
                                           configTIMER_SERVICE_TASK_NAME,
                                           config_TIMER_TASK_ઢગલો_DEPTH,
                                           ખાલી,
                                           ( ( UBaseType_t ) configTIMER_TASK_PRIORITY ) | portPRIVILEGE_BIT,
                                           &xTimerTaskHandle );
                }
                #endif /* configSUPPORT_STATIC_ALLOCATION */
            }
            #endif /* #if ( configNUMBER_OF_CORES > 1 ) */
        }
        નહીંતર
        {
            mtCOVERAGE_TEST_MARKER();
        }

        configASSERT( xReturn );
        પાછા xReturn;
    }
/*-----------------------------------------------------------*/

    #જો ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )

        TimerHandle_t xTimerCreate( અચલ અક્ષર * અચલ pcTimerName, /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                    અચલ TickType_t xTimerPeriodInTicks,
                                    અચલ BaseType_t xAutoReload,
                                    શૂન્ય * અચલ pvTimerID,
                                    TimerCallbackFunction_t pxCallbackFunction )
        {
            Timer_t * pxNewTimer;

            pxNewTimer = ( Timer_t * ) pvPortMalloc( કદ( Timer_t ) ); /*lint !e9087 !e9079 All values returned by pvPortMalloc() have at least the alignment required by the MCU's stack, and the first member of Timer_t is always a pointer to the timer's mame. */

            જો( pxNewTimer != ખાલી )
            {
                /* Status is thus far zero as the timer is not created statically
                 * and has not been started.  The auto-reload bit may get set in
                 * prvInitialiseNewTimer. */
                pxNewTimer->ucStatus = 0x00;
                prvInitialiseNewTimer( pcTimerName, xTimerPeriodInTicks, xAutoReload, pvTimerID, pxCallbackFunction, pxNewTimer );
            }

            પાછા pxNewTimer;
        }

    #endif /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

    #જો ( configSUPPORT_STATIC_ALLOCATION == 1 )

        TimerHandle_t xTimerCreateStatic( અચલ અક્ષર * અચલ pcTimerName, /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                          અચલ TickType_t xTimerPeriodInTicks,
                                          અચલ BaseType_t xAutoReload,
                                          શૂન્ય * અચલ pvTimerID,
                                          TimerCallbackFunction_t pxCallbackFunction,
                                          StaticTimer_t * pxTimerBuffer )
        {
            Timer_t * pxNewTimer;

            #જો ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticTimer_t equals the size of the real timer
                 * structure. */
                અસ્થિર size_t xSize = કદ( StaticTimer_t );
                configASSERT( xSize == કદ( Timer_t ) );
                ( શૂન્ય ) xSize; /* Keeps lint quiet when configASSERT() is not defined. */
            }
            #endif /* configASSERT_DEFINED */

            /* A pointer to a StaticTimer_t structure MUST be provided, use it. */
            configASSERT( pxTimerBuffer );
            pxNewTimer = ( Timer_t * ) pxTimerBuffer; /*lint !e740 !e9087 StaticTimer_t is a pointer to a Timer_t, so guaranteed to be aligned and sized correctly (checked by an assert()), so this is safe. */

            જો( pxNewTimer != ખાલી )
            {
                /* Timers can be created statically or dynamically so note this
                 * timer was created statically in case it is later deleted.  The
                 * auto-reload bit may get set in prvInitialiseNewTimer(). */
                pxNewTimer->ucStatus = tmrSTATUS_IS_STATICALLY_ALLOCATED;

                prvInitialiseNewTimer( pcTimerName, xTimerPeriodInTicks, xAutoReload, pvTimerID, pxCallbackFunction, pxNewTimer );
            }

            પાછા pxNewTimer;
        }

    #endif /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

    સ્થિર શૂન્ય prvInitialiseNewTimer( અચલ અક્ષર * અચલ pcTimerName, /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                       અચલ TickType_t xTimerPeriodInTicks,
                                       અચલ BaseType_t xAutoReload,
                                       શૂન્ય * અચલ pvTimerID,
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

        જો( xAutoReload != pdFALSE )
        {
            pxNewTimer->ucStatus |= tmrSTATUS_IS_AUTORELOAD;
        }

        traceTIMER_CREATE( pxNewTimer );
    }
/*-----------------------------------------------------------*/

    BaseType_t xTimerGenericCommand( TimerHandle_t xTimer,
                                     અચલ BaseType_t xCommandID,
                                     અચલ TickType_t x_Optional_કિંમત,
                                     BaseType_t * અચલ pxHigherPriorityTaskWoken,
                                     અચલ TickType_t xTicksToWait )
    {
        BaseType_t xReturn = pdFAIL;
        DaemonTaskMessage_t xMessage;

        configASSERT( xTimer );

        /* Send a message to the timer service task to perform a particular action
         * on a particular timer definition. */
        જો( xTimerQueue != ખાલી )
        {
            /* Send a command to the timer service task to start the xTimer timer. */
            xMessage.xMessageID = xCommandID;
            xMessage.u.xTimerParameters.x_Message_કિંમત = x_Optional_કિંમત;
            xMessage.u.xTimerParameters.pxTimer = xTimer;

            જો( xCommandID < tmrFIRST_FROM_ISR_COMMAND )
            {
                જો( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING )
                {
                    xReturn = xQueueSendToBack( xTimerQueue, &xMessage, xTicksToWait );
                }
                નહીંતર
                {
                    xReturn = xQueueSendToBack( xTimerQueue, &xMessage, tmrNO_DELAY );
                }
            }
            નહીંતર
            {
                xReturn = xQueueSendToBackFromISR( xTimerQueue, &xMessage, pxHigherPriorityTaskWoken );
            }

            traceTIMER_COMMAND_SEND( xTimer, xCommandID, x_Optional_કિંમત, xReturn );
        }
        નહીંતર
        {
            mtCOVERAGE_TEST_MARKER();
        }

        પાછા xReturn;
    }
/*-----------------------------------------------------------*/

    TaskHandle_t xTimerGetTimerDaemonTaskHandle( શૂન્ય )
    {
        /* If xTimerGetTimerDaemonTaskHandle() is called before the scheduler has been
         * started, then xTimerTaskHandle will be NULL. */
        configASSERT( ( xTimerTaskHandle != ખાલી ) );
        પાછા xTimerTaskHandle;
    }
/*-----------------------------------------------------------*/

    TickType_t xTimerGetPeriod( TimerHandle_t xTimer )
    {
        Timer_t * pxTimer = xTimer;

        configASSERT( xTimer );
        પાછા pxTimer->xTimerPeriodInTicks;
    }
/*-----------------------------------------------------------*/

    શૂન્ય vTimerSetReloadMode( TimerHandle_t xTimer,
                              અચલ BaseType_t xAutoReload )
    {
        Timer_t * pxTimer = xTimer;

        configASSERT( xTimer );
        taskENTER_CRITICAL( &xTimerLock );
        {
            જો( xAutoReload != pdFALSE )
            {
                pxTimer->ucStatus |= tmrSTATUS_IS_AUTORELOAD;
            }
            નહીંતર
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
            જો( ( pxTimer->ucStatus & tmrSTATUS_IS_AUTORELOAD ) == 0 )
            {
                /* Not an auto-reload timer. */
                xReturn = pdFALSE;
            }
            નહીંતર
            {
                /* Is an auto-reload timer. */
                xReturn = pdTRUE;
            }
        }
        taskEXIT_CRITICAL( &xTimerLock );

        પાછા xReturn;
    }

    UBaseType_t uxTimerGetReloadMode( TimerHandle_t xTimer )
    {
        પાછા ( UBaseType_t ) xTimerGetReloadMode( xTimer );
    }
/*-----------------------------------------------------------*/

    TickType_t xTimerGetExpiryTime( TimerHandle_t xTimer )
    {
        Timer_t * pxTimer = xTimer;
        TickType_t xReturn;

        configASSERT( xTimer );
        xReturn = list_GET_LIST_ITEM_કિંમત( &( pxTimer->xTimerListItem ) );
        પાછા xReturn;
    }
/*-----------------------------------------------------------*/

    #જો ( configSUPPORT_STATIC_ALLOCATION == 1 )
        BaseType_t xTimerGetStaticBuffer( TimerHandle_t xTimer,
                                          StaticTimer_t ** ppxTimerBuffer )
        {
            BaseType_t xReturn;
            Timer_t * pxTimer = xTimer;

            configASSERT( ppxTimerBuffer != ખાલી );

            જો( ( pxTimer->ucStatus & tmrSTATUS_IS_STATICALLY_ALLOCATED ) != 0 )
            {
                *ppxTimerBuffer = ( StaticTimer_t * ) pxTimer;
                xReturn = pdTRUE;
            }
            નહીંતર
            {
                xReturn = pdFALSE;
            }

            પાછા xReturn;
        }
    #endif /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

    અચલ અક્ષર * pcTimerGetName( TimerHandle_t xTimer ) /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
    {
        Timer_t * pxTimer = xTimer;

        configASSERT( xTimer );
        પાછા pxTimer->pcTimerName;
    }
/*-----------------------------------------------------------*/

    સ્થિર શૂન્ય prvReloadTimer( Timer_t * અચલ pxTimer,
                                TickType_t xExpiredTime,
                                અચલ TickType_t xTimeNow )
    {
        /* Insert the timer into the appropriate list for the next expiry time.
         * If the next expiry time has already passed, advance the expiry time,
         * call the callback function, and try again. */
        જ્યારે( prv_ઉમેરો_Timer_In_Active_List( pxTimer, ( xExpiredTime + pxTimer->xTimerPeriodInTicks ), xTimeNow, xExpiredTime ) != pdFALSE )
        {
            /* Advance the expiry time. */
            xExpiredTime += pxTimer->xTimerPeriodInTicks;

            /* Call the timer callback. */
            traceTIMER_EXPIRED( pxTimer );
            pxTimer->pxCallbackFunction( ( TimerHandle_t ) pxTimer );
        }
    }
/*-----------------------------------------------------------*/

    સ્થિર શૂન્ય prvProcessExpiredTimer( અચલ TickType_t xNextExpireTime,
                                        અચલ TickType_t xTimeNow )
    {
        Timer_t * અચલ pxTimer = ( Timer_t * ) listGET_OWNER_OF_HEAD_ENTRY( pxCurrentTimerList ); /*lint !e9087 !e9079 void * is used as this macro is used with tasks and co-routines too.  Alignment is known to be fine as the type of the pointer stored and retrieved is the same. */

        /* Remove the timer from the list of active timers.  A check has already
         * been performed to ensure the list is not empty. */

        ( શૂન્ય ) uxListRemove( &( pxTimer->xTimerListItem ) );

        /* If the timer is an auto-reload timer then calculate the next
         * expiry time and re-insert the timer in the list of active timers. */
        જો( ( pxTimer->ucStatus & tmrSTATUS_IS_AUTORELOAD ) != 0 )
        {
            prvReloadTimer( pxTimer, xNextExpireTime, xTimeNow );
        }
        નહીંતર
        {
            pxTimer->ucStatus &= ( ( uint8_t ) ~tmrSTATUS_IS_ACTIVE );
        }

        /* Call the timer callback. */
        traceTIMER_EXPIRED( pxTimer );
        pxTimer->pxCallbackFunction( ( TimerHandle_t ) pxTimer );
    }
/*-----------------------------------------------------------*/

    સ્થિર portTASK_FUNCTION( prvTimerTask, pvParameters )
    {
        TickType_t xNextExpireTime;
        BaseType_t xListWasEmpty;

        /* Just to avoid compiler warnings. */
        ( શૂન્ય ) pvParameters;

        #જો ( configUSE_DAEMON_TASK_STARTUP_HOOK == 1 )
        {
            બાહ્ય શૂન્ય vApplicationDaemonTaskStartupHook( શૂન્ય );

            /* Allow the application writer to execute some code in the context of
             * this task at the point the task starts executing.  This is useful if the
             * application includes initialisation code that would benefit from
             * executing after the scheduler has been started. */
            vApplicationDaemonTaskStartupHook();
        }
        #endif /* configUSE_DAEMON_TASK_STARTUP_HOOK */

        માટે( ; ; )
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

    સ્થિર શૂન્ય prvProcessTimerOrBlockTask( અચલ TickType_t xNextExpireTime,
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

            જો( xTimerListsWereSwitched == pdFALSE )
            {
                /* The tick count has not overflowed, has the timer expired? */
                જો( ( xListWasEmpty == pdFALSE ) && ( xNextExpireTime <= xTimeNow ) )
                {
                    ( શૂન્ય ) prvEXIT_CRITICAL_OR_RESUME_ALL( &xTimerLock );
                    prvProcessExpiredTimer( xNextExpireTime, xTimeNow );
                }
                નહીંતર
                {
                    /* The tick count has not overflowed, and the next expire
                     * time has not been reached yet.  This task should therefore
                     * block to wait for the next expire time or a command to be
                     * received - whichever comes first.  The following line cannot
                     * be reached unless xNextExpireTime > xTimeNow, except in the
                     * case when the current timer list is empty. */
                    જો( xListWasEmpty != pdFALSE )
                    {
                        /* The current timer list is empty - is the overflow list
                         * also empty? */
                        xListWasEmpty = listLIST_IS_EMPTY( pxOverflowTimerList );
                    }

                    vQueueWaitForMessageRestricted( xTimerQueue, ( xNextExpireTime - xTimeNow ), xListWasEmpty );

                    જો( prvEXIT_CRITICAL_OR_RESUME_ALL( &xTimerLock ) == pdFALSE )
                    {
                        /* Yield to wait for either a command to arrive, or the
                         * block time to expire.  If a command arrived between the
                         * critical section being exited and this yield then the yield
                         * will not cause the task to block. */
                        portYIELD_WITHIN_API();
                    }
                    નહીંતર
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
            }
            નહીંતર
            {
                ( શૂન્ય ) prvEXIT_CRITICAL_OR_RESUME_ALL( &xTimerLock );
            }
        }
    }
/*-----------------------------------------------------------*/

    સ્થિર TickType_t prvGetNextExpireTime( BaseType_t * અચલ pxListWasEmpty )
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

        જો( *pxListWasEmpty == pdFALSE )
        {
            xNextExpireTime = list_GET_ITEM_કિંમત_OF_HEAD_ENTRY( pxCurrentTimerList );
        }
        નહીંતર
        {
            /* Ensure the task unblocks when the tick count rolls over. */
            xNextExpireTime = ( TickType_t ) 0U;
        }

        પાછા xNextExpireTime;
    }
/*-----------------------------------------------------------*/

    સ્થિર TickType_t prvSampleTimeNow( BaseType_t * અચલ pxTimerListsWereSwitched )
    {
        TickType_t xTimeNow;
        PRIVILEGED_DATA સ્થિર TickType_t xLastTime = ( TickType_t ) 0U; /*lint !e956 Variable is only accessible to one task. */

        xTimeNow = x_Task_Get_Tick_ગણતરી();

        જો( xTimeNow < xLastTime )
        {
            prvSwitchTimerLists();
            *pxTimerListsWereSwitched = pdTRUE;
        }
        નહીંતર
        {
            *pxTimerListsWereSwitched = pdFALSE;
        }

        xLastTime = xTimeNow;

        પાછા xTimeNow;
    }
/*-----------------------------------------------------------*/

    સ્થિર BaseType_t prv_ઉમેરો_Timer_In_Active_List( Timer_t * અચલ pxTimer,
                                                  અચલ TickType_t xNextExpiryTime,
                                                  અચલ TickType_t xTimeNow,
                                                  અચલ TickType_t xCommandTime )
    {
        BaseType_t xProcessTimerNow = pdFALSE;

        list_SET_LIST_ITEM_કિંમત( &( pxTimer->xTimerListItem ), xNextExpiryTime );
        listSET_LIST_ITEM_OWNER( &( pxTimer->xTimerListItem ), pxTimer );

        જો( xNextExpiryTime <= xTimeNow )
        {
            /* Has the expiry time elapsed between the command to start/reset a
             * timer was issued, and the time the command was processed? */
            જો( ( ( TickType_t ) ( xTimeNow - xCommandTime ) ) >= pxTimer->xTimerPeriodInTicks ) /*lint !e961 MISRA exception as the casts are only redundant for some ports. */
            {
                /* The time between a command being issued and the command being
                 * processed actually exceeds the timers period.  */
                xProcessTimerNow = pdTRUE;
            }
            નહીંતર
            {
                v_List_ઉમેરો( pxOverflowTimerList, &( pxTimer->xTimerListItem ) );
            }
        }
        નહીંતર
        {
            જો( ( xTimeNow < xCommandTime ) && ( xNextExpiryTime >= xCommandTime ) )
            {
                /* If, since the command was issued, the tick count has overflowed
                 * but the expiry time has not, then the timer must have already passed
                 * its expiry time and should be processed immediately. */
                xProcessTimerNow = pdTRUE;
            }
            નહીંતર
            {
                v_List_ઉમેરો( pxCurrentTimerList, &( pxTimer->xTimerListItem ) );
            }
        }

        પાછા xProcessTimerNow;
    }
/*-----------------------------------------------------------*/

    સ્થિર શૂન્ય prvProcessReceivedCommands( શૂન્ય )
    {
        DaemonTaskMessage_t xMessage;
        Timer_t * pxTimer;
        BaseType_t xTimerListsWereSwitched;
        TickType_t xTimeNow;

        જ્યારે( xQueueReceive( xTimerQueue, &xMessage, tmrNO_DELAY ) != pdFAIL ) /*lint !e603 xMessage does not have to be initialised as it is passed out, not in, and it is not used unless xQueueReceive() returns pdTRUE. */
        {
            #જો ( INCLUDE_xTimerPendFunctionCall == 1 )
            {
                /* Negative commands are pended function calls rather than timer
                 * commands. */
                જો( xMessage.xMessageID < ( BaseType_t ) 0 )
                {
                    અચલ CallbackParameters_t * અચલ pxCallback = &( xMessage.u.xCallbackParameters );

                    /* The timer uses the xCallbackParameters member to request a
                     * callback be executed.  Check the callback is not NULL. */
                    configASSERT( pxCallback );

                    /* Call the function. */
                    pxCallback->pxCallbackFunction( pxCallback->pvParameter1, pxCallback->ulParameter2 );
                }
                નહીંતર
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            #endif /* INCLUDE_xTimerPendFunctionCall */

            /* Commands that are positive are timer commands rather than pended
             * function calls. */
            જો( xMessage.xMessageID >= ( BaseType_t ) 0 )
            {
                /* The messages uses the xTimerParameters member to work on a
                 * software timer. */
                pxTimer = xMessage.u.xTimerParameters.pxTimer;

                જો( listIS_CONTAINED_WITHIN( ખાલી, &( pxTimer->xTimerListItem ) ) == pdFALSE ) /*lint !e961. The cast is only redundant when NULL is passed into the macro. */
                {
                    /* The timer is in a list, remove it. */
                    ( શૂન્ય ) uxListRemove( &( pxTimer->xTimerListItem ) );
                }
                નહીંતર
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                traceTIMER_COMMAND_RECEIVED( pxTimer, xMessage.xMessageID, xMessage.u.xTimerParameters.x_Message_કિંમત );

                /* In this case the xTimerListsWereSwitched parameter is not used, but
                 *  it must be present in the function call.  prvSampleTimeNow() must be
                 *  called after the message is received from xTimerQueue so there is no
                 *  possibility of a higher priority task adding a message to the message
                 *  queue with a time that is ahead of the timer daemon task (because it
                 *  pre-empted the timer daemon task after the xTimeNow value was set). */
                xTimeNow = prvSampleTimeNow( &xTimerListsWereSwitched );

                પસંદગી( xMessage.xMessageID )
                {
                    સ્થિતિ tmrCOMMAND_START:
                    સ્થિતિ tmrCOMMAND_START_FROM_ISR:
                    સ્થિતિ tmrCOMMAND_RESET:
                    સ્થિતિ tmrCOMMAND_RESET_FROM_ISR:
                        /* Start or restart a timer. */
                        pxTimer->ucStatus |= tmrSTATUS_IS_ACTIVE;

                        જો( prv_ઉમેરો_Timer_In_Active_List( pxTimer, xMessage.u.xTimerParameters.x_Message_કિંમત + pxTimer->xTimerPeriodInTicks, xTimeNow, xMessage.u.xTimerParameters.x_Message_કિંમત ) != pdFALSE )
                        {
                            /* The timer expired before it was added to the active
                             * timer list.  Process it now. */
                            જો( ( pxTimer->ucStatus & tmrSTATUS_IS_AUTORELOAD ) != 0 )
                            {
                                prvReloadTimer( pxTimer, xMessage.u.xTimerParameters.x_Message_કિંમત + pxTimer->xTimerPeriodInTicks, xTimeNow );
                            }
                            નહીંતર
                            {
                                pxTimer->ucStatus &= ( ( uint8_t ) ~tmrSTATUS_IS_ACTIVE );
                            }

                            /* Call the timer callback. */
                            traceTIMER_EXPIRED( pxTimer );
                            pxTimer->pxCallbackFunction( ( TimerHandle_t ) pxTimer );
                        }
                        નહીંતર
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }

                        તોડો;

                    સ્થિતિ tmrCOMMAND_STOP:
                    સ્થિતિ tmrCOMMAND_STOP_FROM_ISR:
                        /* The timer has already been removed from the active list. */
                        pxTimer->ucStatus &= ( ( uint8_t ) ~tmrSTATUS_IS_ACTIVE );
                        તોડો;

                    સ્થિતિ tmrCOMMAND_CHANGE_PERIOD:
                    સ્થિતિ tmrCOMMAND_CHANGE_PERIOD_FROM_ISR:
                        pxTimer->ucStatus |= tmrSTATUS_IS_ACTIVE;
                        pxTimer->xTimerPeriodInTicks = xMessage.u.xTimerParameters.x_Message_કિંમત;
                        configASSERT( ( pxTimer->xTimerPeriodInTicks > 0 ) );

                        /* The new period does not really have a reference, and can
                         * be longer or shorter than the old one.  The command time is
                         * therefore set to the current time, and as the period cannot
                         * be zero the next expiry time can only be in the future,
                         * meaning (unlike for the xTimerStart() case above) there is
                         * no fail case that needs to be handled here. */
                        ( શૂન્ય ) prv_ઉમેરો_Timer_In_Active_List( pxTimer, ( xTimeNow + pxTimer->xTimerPeriodInTicks ), xTimeNow, xTimeNow );
                        તોડો;

                    સ્થિતિ tmrCOMMAND_DELETE:
                        #જો ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
                        {
                            /* The timer has already been removed from the active list,
                             * just free up the memory if the memory was dynamically
                             * allocated. */
                            જો( ( pxTimer->ucStatus & tmrSTATUS_IS_STATICALLY_ALLOCATED ) == ( uint8_t ) 0 )
                            {
                                vPortFree( pxTimer );
                            }
                            નહીંતર
                            {
                                pxTimer->ucStatus &= ( ( uint8_t ) ~tmrSTATUS_IS_ACTIVE );
                            }
                        }
                        #નહીંતર /* if ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) */
                        {
                            /* If dynamic allocation is not enabled, the memory
                             * could not have been dynamically allocated. So there is
                             * no need to free the memory - just mark the timer as
                             * "not active". */
                            pxTimer->ucStatus &= ( ( uint8_t ) ~tmrSTATUS_IS_ACTIVE );
                        }
                        #endif /* configSUPPORT_DYNAMIC_ALLOCATION */
                        તોડો;

                    મૂળભૂત:
                        /* Don't expect to get here. */
                        તોડો;
                }
            }
        }
    }
/*-----------------------------------------------------------*/

    સ્થિર શૂન્ય prvSwitchTimerLists( શૂન્ય )
    {
        TickType_t xNextExpireTime;
        List_t * pxTemp;

        /* The tick count has overflowed.  The timer lists must be switched.
         * If there are any timers still referenced from the current timer list
         * then they must have expired and should be processed before the lists
         * are switched. */
        જ્યારે( listLIST_IS_EMPTY( pxCurrentTimerList ) == pdFALSE )
        {
            xNextExpireTime = list_GET_ITEM_કિંમત_OF_HEAD_ENTRY( pxCurrentTimerList );

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

    સ્થિર શૂન્ય prvCheckForValidListAndQueue( શૂન્ય )
    {
        /* Check that the list from which active timers are referenced, and the
         * queue used to communicate with the timer service, have been
         * initialised. */
        taskENTER_CRITICAL( &xTimerLock );
        {
            જો( xTimerQueue == ખાલી )
            {
                vListInitialise( &xActiveTimerList1 );
                vListInitialise( &xActiveTimerList2 );
                pxCurrentTimerList = &xActiveTimerList1;
                pxOverflowTimerList = &xActiveTimerList2;

                #જો ( configSUPPORT_STATIC_ALLOCATION == 1 )
                {
                    /* The timer queue is allocated statically in case
                     * configSUPPORT_DYNAMIC_ALLOCATION is 0. */
                    PRIVILEGED_DATA સ્થિર StaticQueue_t xStaticTimerQueue;                                                                          /*lint !e956 Ok to declare in this manner to prevent additional conditional compilation guards in other locations. */
                    PRIVILEGED_DATA સ્થિર uint8_t ucStaticTimerQueueStorage[ ( size_t ) configTIMER_QUEUE_LENGTH * કદ( DaemonTaskMessage_t ) ]; /*lint !e956 Ok to declare in this manner to prevent additional conditional compilation guards in other locations. */

                    xTimerQueue = xQueueCreateStatic( ( UBaseType_t ) configTIMER_QUEUE_LENGTH, ( UBaseType_t ) કદ( DaemonTaskMessage_t ), &( ucStaticTimerQueueStorage[ 0 ] ), &xStaticTimerQueue );
                }
                #નહીંતર
                {
                    xTimerQueue = xQueueCreate( ( UBaseType_t ) configTIMER_QUEUE_LENGTH, કદ( DaemonTaskMessage_t ) );
                }
                #endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */

                #જો ( configQUEUE_REGISTRY_SIZE > 0 )
                {
                    જો( xTimerQueue != ખાલી )
                    {
                        vQueueAddToRegistry( xTimerQueue, "TmrQ" );
                    }
                    નહીંતર
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #endif /* configQUEUE_REGISTRY_SIZE */
            }
            નહીંતર
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
            જો( ( pxTimer->ucStatus & tmrSTATUS_IS_ACTIVE ) == 0 )
            {
                xReturn = pdFALSE;
            }
            નહીંતર
            {
                xReturn = pdTRUE;
            }
        }
        taskEXIT_CRITICAL( &xTimerLock );

        પાછા xReturn;
    } /*lint !e818 Can't be pointer to const due to the typedef. */
/*-----------------------------------------------------------*/

    શૂન્ય * pvTimerGetTimerID( અચલ TimerHandle_t xTimer )
    {
        Timer_t * અચલ pxTimer = xTimer;
        શૂન્ય * pvReturn;

        configASSERT( xTimer );

        taskENTER_CRITICAL( &xTimerLock );
        {
            pvReturn = pxTimer->pvTimerID;
        }
        taskEXIT_CRITICAL( &xTimerLock );

        પાછા pvReturn;
    }
/*-----------------------------------------------------------*/

    શૂન્ય vTimerSetTimerID( TimerHandle_t xTimer,
                           શૂન્ય * pvNewID )
    {
        Timer_t * અચલ pxTimer = xTimer;

        configASSERT( xTimer );

        taskENTER_CRITICAL( &xTimerLock );
        {
            pxTimer->pvTimerID = pvNewID;
        }
        taskEXIT_CRITICAL( &xTimerLock );
    }
/*-----------------------------------------------------------*/

    #જો ( INCLUDE_xTimerPendFunctionCall == 1 )

        BaseType_t xTimerPendFunctionCallFromISR( PendedFunction_t xFunctionToPend,
                                                  શૂન્ય * pvParameter1,
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

            પાછા xReturn;
        }

    #endif /* INCLUDE_xTimerPendFunctionCall */
/*-----------------------------------------------------------*/

    #જો ( INCLUDE_xTimerPendFunctionCall == 1 )

        BaseType_t xTimerPendFunctionCall( PendedFunction_t xFunctionToPend,
                                           શૂન્ય * pvParameter1,
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

            પાછા xReturn;
        }

    #endif /* INCLUDE_xTimerPendFunctionCall */
/*-----------------------------------------------------------*/

    #જો ( configUSE_TRACE_FACILITY == 1 )

        UBaseType_t ux_Timer_Get_Timer_સંખ્યા( TimerHandle_t xTimer )
        {
            પાછા ( ( Timer_t * ) xTimer )->ux_Timer_સંખ્યા;
        }

    #endif /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

    #જો ( configUSE_TRACE_FACILITY == 1 )

        શૂન્ય v_Timer_Set_Timer_સંખ્યા( TimerHandle_t xTimer,
                                   UBaseType_t ux_Timer_સંખ્યા )
        {
            ( ( Timer_t * ) xTimer )->ux_Timer_સંખ્યા = ux_Timer_સંખ્યા;
        }

    #endif /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

/* This entire source file will be skipped if the application is not configured
 * to include software timer functionality.  If you want to include software timer
 * functionality then ensure configUSE_TIMERS is set to 1 in FreeRTOSConfig.h. */
#endif /* configUSE_TIMERS == 1 */
