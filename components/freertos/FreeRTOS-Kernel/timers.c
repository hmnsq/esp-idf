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
#அந்தர்பாவ <stdlib.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#define MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#அந்தர்பாவ "FreeRTOS.h"
#அந்தர்பாவ "task.h"
#அந்தர்பாவ "queue.h"
#அந்தர்பாவ "timers.h"
/* Include private IDF API additions for critical thread safety macros */
#அந்தர்பாவ "esp_private/freertos_idf_additions_priv.h"

#எனில் ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 0 )
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
#எனில் ( configUSE_TIMERS == 1 )

/* Misc definitions. */
    #define tmrNO_DELAY                    ( ( TickType_t ) 0U )
    #define tmrMAX_TIME_BEFORE_OVERFLOW    ( ( TickType_t ) -1 )

/* The name assigned to the timer service task. This can be overridden by
 * defining configTIMER_SERVICE_TASK_NAME in FreeRTOSConfig.h. */
    #ifndef configTIMER_SERVICE_TASK_NAME
        #define configTIMER_SERVICE_TASK_NAME    "Tmr Svc"
    #endif

    #எனில் ( config_எண்_OF_CORES > 1 )

/* The core affinity assigned to the timer service task on SMP systems.
 * This can be overridden by defining configTIMER_SERVICE_TASK_CORE_AFFINITY in FreeRTOSConfig.h. */
        #ifndef configTIMER_SERVICE_TASK_CORE_AFFINITY
            #define configTIMER_SERVICE_TASK_CORE_AFFINITY    tskNO_AFFINITY
        #endif

        #எனில் ( !( ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x0 ) || ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x1 ) || ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x7FFFFFFF /* tskNO_AFFINITY */ ) ) )
            #error Invalid Timer task affinity. Timer task core ID can only be 0x0, 0x1 or 0x7FFFFFFF (tskNO_AFFINITY).
        #endif /* #if ( !( ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x0 ) || ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x01 ) || ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x7FFFFFFF ) ) ) */
    #endif /* #if ( configNUMBER_OF_CORES > 1 ) */

/* Bit definitions used in the ucStatus member of a timer structure. */
    #define tmrSTATUS_IS_ACTIVE                  ( ( uint8_t ) 0x01 )
    #define tmrSTATUS_IS_STATICALLY_ALLOCATED    ( ( uint8_t ) 0x02 )
    #define tmrSTATUS_IS_AUTORELOAD              ( ( uint8_t ) 0x04 )

/* The definition of the timers themselves. */
    வகைவரை கட்டமைப்பு tmrTimerControl                  /* The old naming convention is used to prevent breaking kernel aware debuggers. */
    {
        மாறிலி எழுத்து * pcTimerName;                   /*<< Text name.  This is not used by the kernel, it is included simply to make debugging easier. */ /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
        ListItem_t xTimerListItem;                  /*<< Standard linked list item as used by all kernel features for event management. */
        TickType_t xTimerPeriodInTicks;             /*<< How quickly and often the timer expires. */
        வெறுமை * pvTimerID;                           /*<< An ID to identify the timer.  This allows the timer to be identified when the same callback is used for multiple timers. */
        TimerCallbackFunction_t pxCallbackFunction; /*<< The function that will be called when the timer expires. */
        #எனில் ( configUSE_TRACE_FACILITY == 1 )
            UBaseType_t ux_Timer_எண்;              /*<< An ID assigned by trace tools such as FreeRTOS+Trace */
        #endif
        uint8_t ucStatus;                           /*<< Holds bits to say if the timer was statically allocated or not, and if it is active or not. */
    } xTIMER;

/* The old xTIMER name is maintained above then typedefed to the new Timer_t
 * name below to enable the use of older kernel aware debuggers. */
    வகைவரை xTIMER Timer_t;

/* The definition of messages that can be sent and received on the timer queue.
 * Two types of message can be queued - messages that manipulate a software timer,
 * and messages that request the execution of a non-timer related callback.  The
 * two message types are defined in two separate structures, xTimerParametersType
 * and xCallbackParametersType respectively. */
    வகைவரை கட்டமைப்பு tmrTimerParameters
    {
        TickType_t x_Message_மதிப்பு; /*<< An optional value used by a subset of commands, for example, when changing the period of a timer. */
        Timer_t * pxTimer;        /*<< The timer to which the command will be applied. */
    } TimerParameter_t;


    வகைவரை கட்டமைப்பு tmrCallbackParameters
    {
        PendedFunction_t pxCallbackFunction; /* << The callback function to execute. */
        வெறுமை * pvParameter1;                 /* << The value that will be used as the callback functions first parameter. */
        uint32_t ulParameter2;               /* << The value that will be used as the callback functions second parameter. */
    } CallbackParameters_t;

/* The structure that contains the two message types, along with an identifier
 * that is used to determine which message type is valid. */
    வகைவரை கட்டமைப்பு tmrTimerQueueMessage
    {
        BaseType_t xMessageID; /*<< The command being sent to the timer service task. */
        ஒன்றிப்பு
        {
            TimerParameter_t xTimerParameters;

            /* Don't include xCallbackParameters if it is not going to be used as
             * it makes the structure (and therefore the timer queue) larger. */
            #எனில் ( INCLUDE_xTimerPendFunctionCall == 1 )
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
    PRIVILEGED_DATA நிலையான List_t xActiveTimerList1;
    PRIVILEGED_DATA நிலையான List_t xActiveTimerList2;
    PRIVILEGED_DATA நிலையான List_t * pxCurrentTimerList;
    PRIVILEGED_DATA நிலையான List_t * pxOverflowTimerList;

/* A queue that is used to send commands to the timer service task. */
    PRIVILEGED_DATA நிலையான QueueHandle_t xTimerQueue = வெற்று;
    PRIVILEGED_DATA நிலையான TaskHandle_t xTimerTaskHandle = வெற்று;

/* Spinlock required in SMP when accessing the timers. For now we use a single lock
 * Todo: Each timer could possible have its own lock for increased granularity. */
    PRIVILEGED_DATA நிலையான portMUX_TYPE xTimerLock = portMUX_INITIALIZER_UNLOCKED;

/*lint -restore */

/*-----------------------------------------------------------*/

/*
 * Initialise the infrastructure used by the timer service task if it has not
 * been initialised already.
 */
    நிலையான வெறுமை prvCheckForValidListAndQueue( வெறுமை ) PRIVILEGED_FUNCTION;

/*
 * The timer service task (daemon).  Timer functionality is controlled by this
 * task.  Other tasks communicate with the timer service task using the
 * xTimerQueue queue.
 */
    நிலையான portTASK_FUNCTION_PROTO( prvTimerTask, pvParameters ) PRIVILEGED_FUNCTION;

/*
 * Called by the timer service task to interpret and process a command it
 * received on the timer queue.
 */
    நிலையான வெறுமை prvProcessReceivedCommands( வெறுமை ) PRIVILEGED_FUNCTION;

/*
 * Insert the timer into either xActiveTimerList1, or xActiveTimerList2,
 * depending on if the expire time causes a timer counter overflow.
 */
    நிலையான BaseType_t prv_செருகு_Timer_In_Active_List( Timer_t * மாறிலி pxTimer,
                                                  மாறிலி TickType_t xNextExpiryTime,
                                                  மாறிலி TickType_t xTimeNow,
                                                  மாறிலி TickType_t xCommandTime ) PRIVILEGED_FUNCTION;

/*
 * Reload the specified auto-reload timer.  If the reloading is backlogged,
 * clear the backlog, calling the callback for each additional reload.  When
 * this function returns, the next expiry time is after xTimeNow.
 */
    நிலையான வெறுமை prvReloadTimer( Timer_t * மாறிலி pxTimer,
                                TickType_t xExpiredTime,
                                மாறிலி TickType_t xTimeNow ) PRIVILEGED_FUNCTION;

/*
 * An active timer has reached its expire time.  Reload the timer if it is an
 * auto-reload timer, then call its callback.
 */
    நிலையான வெறுமை prvProcessExpiredTimer( மாறிலி TickType_t xNextExpireTime,
                                        மாறிலி TickType_t xTimeNow ) PRIVILEGED_FUNCTION;

/*
 * The tick count has overflowed.  Switch the timer lists after ensuring the
 * current timer list does not still reference some timers.
 */
    நிலையான வெறுமை prvSwitchTimerLists( வெறுமை ) PRIVILEGED_FUNCTION;

/*
 * Obtain the current tick count, setting *pxTimerListsWereSwitched to pdTRUE
 * if a tick count overflow occurred since prvSampleTimeNow() was last called.
 */
    நிலையான TickType_t prvSampleTimeNow( BaseType_t * மாறிலி pxTimerListsWereSwitched ) PRIVILEGED_FUNCTION;

/*
 * If the timer list contains any active timers then return the expire time of
 * the timer that will expire first and set *pxListWasEmpty to false.  If the
 * timer list does not contain any timers then return 0 and set *pxListWasEmpty
 * to pdTRUE.
 */
    நிலையான TickType_t prvGetNextExpireTime( BaseType_t * மாறிலி pxListWasEmpty ) PRIVILEGED_FUNCTION;

/*
 * If a timer has expired, process it.  Otherwise, block the timer service task
 * until either a timer does expire or a command is received.
 */
    நிலையான வெறுமை prvProcessTimerOrBlockTask( மாறிலி TickType_t xNextExpireTime,
                                            BaseType_t xListWasEmpty ) PRIVILEGED_FUNCTION;

/*
 * Called after a Timer_t structure has been allocated either statically or
 * dynamically to fill in the structure's members.
 */
    நிலையான வெறுமை prvInitialiseNewTimer( மாறிலி எழுத்து * மாறிலி pcTimerName, /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                       மாறிலி TickType_t xTimerPeriodInTicks,
                                       மாறிலி BaseType_t xAutoReload,
                                       வெறுமை * மாறிலி pvTimerID,
                                       TimerCallbackFunction_t pxCallbackFunction,
                                       Timer_t * pxNewTimer ) PRIVILEGED_FUNCTION;
/*-----------------------------------------------------------*/

    BaseType_t xTimerCreateTimerTask( வெறுமை )
    {
        BaseType_t xReturn = pdFAIL;

        /* This function is called when the scheduler is started if
         * configUSE_TIMERS is set to 1.  Check that the infrastructure used by the
         * timer service task has been created/initialised.  If timers have already
         * been created then the initialisation will already have been performed. */
        prvCheckForValidListAndQueue();

        எனில்( xTimerQueue != வெற்று )
        {
            /* The following code block to set the timer task core affinity has been picked up from the FreeRTOS-Kernel
             * upstream repository. Commit# 631ae9e6e4828cf5c7d58bb7ac78a64fc309bcb7.
             * Necessary changes have been made for IDF-FreeRTOS compatibility. */
            #எனில் ( config_எண்_OF_CORES > 1 )
            {
                #எனில் ( configSUPPORT_STATIC_ALLOCATION == 1 )
                {
                    StaticTask_t * pxTimerTaskTCBBuffer = வெற்று;
                    அடுக்கு_Type_t * px_Timer_Task_அடுக்கு_Buffer = வெற்று;
                    uint32_t ul_Timer_Task_அடுக்கு_Size;

                    vApplicationGetTimerTaskMemory( &pxTimerTaskTCBBuffer, &px_Timer_Task_அடுக்கு_Buffer, &ul_Timer_Task_அடுக்கு_Size );
                    xTimerTaskHandle = xTaskCreateStaticPinnedToCore( prvTimerTask,
                                                                      configTIMER_SERVICE_TASK_NAME,
                                                                      ul_Timer_Task_அடுக்கு_Size,
                                                                      வெற்று,
                                                                      ( ( UBaseType_t ) configTIMER_TASK_PRIORITY ) | portPRIVILEGE_BIT,
                                                                      px_Timer_Task_அடுக்கு_Buffer,
                                                                      pxTimerTaskTCBBuffer,
                                                                      configTIMER_SERVICE_TASK_CORE_AFFINITY );

                    எனில்( xTimerTaskHandle != வெற்று )
                    {
                        xReturn = pdPASS;
                    }
                }
                #இல்லையெனில் /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
                {
                    xReturn = xTaskCreatePinnedToCore( prvTimerTask,
                                                       configTIMER_SERVICE_TASK_NAME,
                                                       config_TIMER_TASK_அடுக்கு_DEPTH,
                                                       வெற்று,
                                                       ( ( UBaseType_t ) configTIMER_TASK_PRIORITY ) | portPRIVILEGE_BIT,
                                                       &xTimerTaskHandle,
                                                       configTIMER_SERVICE_TASK_CORE_AFFINITY );
                }
                #endif /* configSUPPORT_STATIC_ALLOCATION */
            }
            #இல்லையெனில் /* #if ( configNUMBER_OF_CORES > 1 ) */
            {
                #எனில் ( configSUPPORT_STATIC_ALLOCATION == 1 )
                {
                    StaticTask_t * pxTimerTaskTCBBuffer = வெற்று;
                    அடுக்கு_Type_t * px_Timer_Task_அடுக்கு_Buffer = வெற்று;
                    uint32_t ul_Timer_Task_அடுக்கு_Size;

                    vApplicationGetTimerTaskMemory( &pxTimerTaskTCBBuffer, &px_Timer_Task_அடுக்கு_Buffer, &ul_Timer_Task_அடுக்கு_Size );
                    xTimerTaskHandle = xTaskCreateStatic( prvTimerTask,
                                                          configTIMER_SERVICE_TASK_NAME,
                                                          ul_Timer_Task_அடுக்கு_Size,
                                                          வெற்று,
                                                          ( ( UBaseType_t ) configTIMER_TASK_PRIORITY ) | portPRIVILEGE_BIT,
                                                          px_Timer_Task_அடுக்கு_Buffer,
                                                          pxTimerTaskTCBBuffer );

                    எனில்( xTimerTaskHandle != வெற்று )
                    {
                        xReturn = pdPASS;
                    }
                }
                #இல்லையெனில் /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
                {
                    xReturn = xTaskCreate( prvTimerTask,
                                           configTIMER_SERVICE_TASK_NAME,
                                           config_TIMER_TASK_அடுக்கு_DEPTH,
                                           வெற்று,
                                           ( ( UBaseType_t ) configTIMER_TASK_PRIORITY ) | portPRIVILEGE_BIT,
                                           &xTimerTaskHandle );
                }
                #endif /* configSUPPORT_STATIC_ALLOCATION */
            }
            #endif /* #if ( configNUMBER_OF_CORES > 1 ) */
        }
        இல்லையெனில்
        {
            mtCOVERAGE_TEST_MARKER();
        }

        configASSERT( xReturn );
        திரும்பு xReturn;
    }
/*-----------------------------------------------------------*/

    #எனில் ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )

        TimerHandle_t xTimerCreate( மாறிலி எழுத்து * மாறிலி pcTimerName, /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                    மாறிலி TickType_t xTimerPeriodInTicks,
                                    மாறிலி BaseType_t xAutoReload,
                                    வெறுமை * மாறிலி pvTimerID,
                                    TimerCallbackFunction_t pxCallbackFunction )
        {
            Timer_t * pxNewTimer;

            pxNewTimer = ( Timer_t * ) pvPortMalloc( அளவு( Timer_t ) ); /*lint !e9087 !e9079 All values returned by pvPortMalloc() have at least the alignment required by the MCU's stack, and the first member of Timer_t is always a pointer to the timer's mame. */

            எனில்( pxNewTimer != வெற்று )
            {
                /* Status is thus far zero as the timer is not created statically
                 * and has not been started.  The auto-reload bit may get set in
                 * prvInitialiseNewTimer. */
                pxNewTimer->ucStatus = 0x00;
                prvInitialiseNewTimer( pcTimerName, xTimerPeriodInTicks, xAutoReload, pvTimerID, pxCallbackFunction, pxNewTimer );
            }

            திரும்பு pxNewTimer;
        }

    #endif /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

    #எனில் ( configSUPPORT_STATIC_ALLOCATION == 1 )

        TimerHandle_t xTimerCreateStatic( மாறிலி எழுத்து * மாறிலி pcTimerName, /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                          மாறிலி TickType_t xTimerPeriodInTicks,
                                          மாறிலி BaseType_t xAutoReload,
                                          வெறுமை * மாறிலி pvTimerID,
                                          TimerCallbackFunction_t pxCallbackFunction,
                                          StaticTimer_t * pxTimerBuffer )
        {
            Timer_t * pxNewTimer;

            #எனில் ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticTimer_t equals the size of the real timer
                 * structure. */
                நிலையற்ற size_t xSize = அளவு( StaticTimer_t );
                configASSERT( xSize == அளவு( Timer_t ) );
                ( வெறுமை ) xSize; /* Keeps lint quiet when configASSERT() is not defined. */
            }
            #endif /* configASSERT_DEFINED */

            /* A pointer to a StaticTimer_t structure MUST be provided, use it. */
            configASSERT( pxTimerBuffer );
            pxNewTimer = ( Timer_t * ) pxTimerBuffer; /*lint !e740 !e9087 StaticTimer_t is a pointer to a Timer_t, so guaranteed to be aligned and sized correctly (checked by an assert()), so this is safe. */

            எனில்( pxNewTimer != வெற்று )
            {
                /* Timers can be created statically or dynamically so note this
                 * timer was created statically in case it is later deleted.  The
                 * auto-reload bit may get set in prvInitialiseNewTimer(). */
                pxNewTimer->ucStatus = tmrSTATUS_IS_STATICALLY_ALLOCATED;

                prvInitialiseNewTimer( pcTimerName, xTimerPeriodInTicks, xAutoReload, pvTimerID, pxCallbackFunction, pxNewTimer );
            }

            திரும்பு pxNewTimer;
        }

    #endif /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

    நிலையான வெறுமை prvInitialiseNewTimer( மாறிலி எழுத்து * மாறிலி pcTimerName, /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                       மாறிலி TickType_t xTimerPeriodInTicks,
                                       மாறிலி BaseType_t xAutoReload,
                                       வெறுமை * மாறிலி pvTimerID,
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

        எனில்( xAutoReload != pdFALSE )
        {
            pxNewTimer->ucStatus |= tmrSTATUS_IS_AUTORELOAD;
        }

        traceTIMER_CREATE( pxNewTimer );
    }
/*-----------------------------------------------------------*/

    BaseType_t xTimerGenericCommand( TimerHandle_t xTimer,
                                     மாறிலி BaseType_t xCommandID,
                                     மாறிலி TickType_t x_Optional_மதிப்பு,
                                     BaseType_t * மாறிலி pxHigherPriorityTaskWoken,
                                     மாறிலி TickType_t xTicksToWait )
    {
        BaseType_t xReturn = pdFAIL;
        DaemonTaskMessage_t xMessage;

        configASSERT( xTimer );

        /* Send a message to the timer service task to perform a particular action
         * on a particular timer definition. */
        எனில்( xTimerQueue != வெற்று )
        {
            /* Send a command to the timer service task to start the xTimer timer. */
            xMessage.xMessageID = xCommandID;
            xMessage.u.xTimerParameters.x_Message_மதிப்பு = x_Optional_மதிப்பு;
            xMessage.u.xTimerParameters.pxTimer = xTimer;

            எனில்( xCommandID < tmrFIRST_FROM_ISR_COMMAND )
            {
                எனில்( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING )
                {
                    xReturn = xQueueSendToBack( xTimerQueue, &xMessage, xTicksToWait );
                }
                இல்லையெனில்
                {
                    xReturn = xQueueSendToBack( xTimerQueue, &xMessage, tmrNO_DELAY );
                }
            }
            இல்லையெனில்
            {
                xReturn = xQueueSendToBackFromISR( xTimerQueue, &xMessage, pxHigherPriorityTaskWoken );
            }

            traceTIMER_COMMAND_SEND( xTimer, xCommandID, x_Optional_மதிப்பு, xReturn );
        }
        இல்லையெனில்
        {
            mtCOVERAGE_TEST_MARKER();
        }

        திரும்பு xReturn;
    }
/*-----------------------------------------------------------*/

    TaskHandle_t xTimerGetTimerDaemonTaskHandle( வெறுமை )
    {
        /* If xTimerGetTimerDaemonTaskHandle() is called before the scheduler has been
         * started, then xTimerTaskHandle will be NULL. */
        configASSERT( ( xTimerTaskHandle != வெற்று ) );
        திரும்பு xTimerTaskHandle;
    }
/*-----------------------------------------------------------*/

    TickType_t xTimerGetPeriod( TimerHandle_t xTimer )
    {
        Timer_t * pxTimer = xTimer;

        configASSERT( xTimer );
        திரும்பு pxTimer->xTimerPeriodInTicks;
    }
/*-----------------------------------------------------------*/

    வெறுமை vTimerSetReloadMode( TimerHandle_t xTimer,
                              மாறிலி BaseType_t xAutoReload )
    {
        Timer_t * pxTimer = xTimer;

        configASSERT( xTimer );
        taskENTER_CRITICAL( &xTimerLock );
        {
            எனில்( xAutoReload != pdFALSE )
            {
                pxTimer->ucStatus |= tmrSTATUS_IS_AUTORELOAD;
            }
            இல்லையெனில்
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
            எனில்( ( pxTimer->ucStatus & tmrSTATUS_IS_AUTORELOAD ) == 0 )
            {
                /* Not an auto-reload timer. */
                xReturn = pdFALSE;
            }
            இல்லையெனில்
            {
                /* Is an auto-reload timer. */
                xReturn = pdTRUE;
            }
        }
        taskEXIT_CRITICAL( &xTimerLock );

        திரும்பு xReturn;
    }

    UBaseType_t uxTimerGetReloadMode( TimerHandle_t xTimer )
    {
        திரும்பு ( UBaseType_t ) xTimerGetReloadMode( xTimer );
    }
/*-----------------------------------------------------------*/

    TickType_t xTimerGetExpiryTime( TimerHandle_t xTimer )
    {
        Timer_t * pxTimer = xTimer;
        TickType_t xReturn;

        configASSERT( xTimer );
        xReturn = list_GET_LIST_ITEM_மதிப்பு( &( pxTimer->xTimerListItem ) );
        திரும்பு xReturn;
    }
/*-----------------------------------------------------------*/

    #எனில் ( configSUPPORT_STATIC_ALLOCATION == 1 )
        BaseType_t xTimerGetStaticBuffer( TimerHandle_t xTimer,
                                          StaticTimer_t ** ppxTimerBuffer )
        {
            BaseType_t xReturn;
            Timer_t * pxTimer = xTimer;

            configASSERT( ppxTimerBuffer != வெற்று );

            எனில்( ( pxTimer->ucStatus & tmrSTATUS_IS_STATICALLY_ALLOCATED ) != 0 )
            {
                *ppxTimerBuffer = ( StaticTimer_t * ) pxTimer;
                xReturn = pdTRUE;
            }
            இல்லையெனில்
            {
                xReturn = pdFALSE;
            }

            திரும்பு xReturn;
        }
    #endif /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

    மாறிலி எழுத்து * pcTimerGetName( TimerHandle_t xTimer ) /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
    {
        Timer_t * pxTimer = xTimer;

        configASSERT( xTimer );
        திரும்பு pxTimer->pcTimerName;
    }
/*-----------------------------------------------------------*/

    நிலையான வெறுமை prvReloadTimer( Timer_t * மாறிலி pxTimer,
                                TickType_t xExpiredTime,
                                மாறிலி TickType_t xTimeNow )
    {
        /* Insert the timer into the appropriate list for the next expiry time.
         * If the next expiry time has already passed, advance the expiry time,
         * call the callback function, and try again. */
        போது( prv_செருகு_Timer_In_Active_List( pxTimer, ( xExpiredTime + pxTimer->xTimerPeriodInTicks ), xTimeNow, xExpiredTime ) != pdFALSE )
        {
            /* Advance the expiry time. */
            xExpiredTime += pxTimer->xTimerPeriodInTicks;

            /* Call the timer callback. */
            traceTIMER_EXPIRED( pxTimer );
            pxTimer->pxCallbackFunction( ( TimerHandle_t ) pxTimer );
        }
    }
/*-----------------------------------------------------------*/

    நிலையான வெறுமை prvProcessExpiredTimer( மாறிலி TickType_t xNextExpireTime,
                                        மாறிலி TickType_t xTimeNow )
    {
        Timer_t * மாறிலி pxTimer = ( Timer_t * ) listGET_OWNER_OF_HEAD_ENTRY( pxCurrentTimerList ); /*lint !e9087 !e9079 void * is used as this macro is used with tasks and co-routines too.  Alignment is known to be fine as the type of the pointer stored and retrieved is the same. */

        /* Remove the timer from the list of active timers.  A check has already
         * been performed to ensure the list is not empty. */

        ( வெறுமை ) uxListRemove( &( pxTimer->xTimerListItem ) );

        /* If the timer is an auto-reload timer then calculate the next
         * expiry time and re-insert the timer in the list of active timers. */
        எனில்( ( pxTimer->ucStatus & tmrSTATUS_IS_AUTORELOAD ) != 0 )
        {
            prvReloadTimer( pxTimer, xNextExpireTime, xTimeNow );
        }
        இல்லையெனில்
        {
            pxTimer->ucStatus &= ( ( uint8_t ) ~tmrSTATUS_IS_ACTIVE );
        }

        /* Call the timer callback. */
        traceTIMER_EXPIRED( pxTimer );
        pxTimer->pxCallbackFunction( ( TimerHandle_t ) pxTimer );
    }
/*-----------------------------------------------------------*/

    நிலையான portTASK_FUNCTION( prvTimerTask, pvParameters )
    {
        TickType_t xNextExpireTime;
        BaseType_t xListWasEmpty;

        /* Just to avoid compiler warnings. */
        ( வெறுமை ) pvParameters;

        #எனில் ( configUSE_DAEMON_TASK_STARTUP_HOOK == 1 )
        {
            வெளி வெறுமை vApplicationDaemonTaskStartupHook( வெறுமை );

            /* Allow the application writer to execute some code in the context of
             * this task at the point the task starts executing.  This is useful if the
             * application includes initialisation code that would benefit from
             * executing after the scheduler has been started. */
            vApplicationDaemonTaskStartupHook();
        }
        #endif /* configUSE_DAEMON_TASK_STARTUP_HOOK */

        க்கு( ; ; )
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

    நிலையான வெறுமை prvProcessTimerOrBlockTask( மாறிலி TickType_t xNextExpireTime,
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

            எனில்( xTimerListsWereSwitched == pdFALSE )
            {
                /* The tick count has not overflowed, has the timer expired? */
                எனில்( ( xListWasEmpty == pdFALSE ) && ( xNextExpireTime <= xTimeNow ) )
                {
                    ( வெறுமை ) prvEXIT_CRITICAL_OR_RESUME_ALL( &xTimerLock );
                    prvProcessExpiredTimer( xNextExpireTime, xTimeNow );
                }
                இல்லையெனில்
                {
                    /* The tick count has not overflowed, and the next expire
                     * time has not been reached yet.  This task should therefore
                     * block to wait for the next expire time or a command to be
                     * received - whichever comes first.  The following line cannot
                     * be reached unless xNextExpireTime > xTimeNow, except in the
                     * case when the current timer list is empty. */
                    எனில்( xListWasEmpty != pdFALSE )
                    {
                        /* The current timer list is empty - is the overflow list
                         * also empty? */
                        xListWasEmpty = listLIST_IS_EMPTY( pxOverflowTimerList );
                    }

                    vQueueWaitForMessageRestricted( xTimerQueue, ( xNextExpireTime - xTimeNow ), xListWasEmpty );

                    எனில்( prvEXIT_CRITICAL_OR_RESUME_ALL( &xTimerLock ) == pdFALSE )
                    {
                        /* Yield to wait for either a command to arrive, or the
                         * block time to expire.  If a command arrived between the
                         * critical section being exited and this yield then the yield
                         * will not cause the task to block. */
                        portYIELD_WITHIN_API();
                    }
                    இல்லையெனில்
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
            }
            இல்லையெனில்
            {
                ( வெறுமை ) prvEXIT_CRITICAL_OR_RESUME_ALL( &xTimerLock );
            }
        }
    }
/*-----------------------------------------------------------*/

    நிலையான TickType_t prvGetNextExpireTime( BaseType_t * மாறிலி pxListWasEmpty )
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

        எனில்( *pxListWasEmpty == pdFALSE )
        {
            xNextExpireTime = list_GET_ITEM_மதிப்பு_OF_HEAD_ENTRY( pxCurrentTimerList );
        }
        இல்லையெனில்
        {
            /* Ensure the task unblocks when the tick count rolls over. */
            xNextExpireTime = ( TickType_t ) 0U;
        }

        திரும்பு xNextExpireTime;
    }
/*-----------------------------------------------------------*/

    நிலையான TickType_t prvSampleTimeNow( BaseType_t * மாறிலி pxTimerListsWereSwitched )
    {
        TickType_t xTimeNow;
        PRIVILEGED_DATA நிலையான TickType_t xLastTime = ( TickType_t ) 0U; /*lint !e956 Variable is only accessible to one task. */

        xTimeNow = x_Task_Get_Tick_மொத்தம்();

        எனில்( xTimeNow < xLastTime )
        {
            prvSwitchTimerLists();
            *pxTimerListsWereSwitched = pdTRUE;
        }
        இல்லையெனில்
        {
            *pxTimerListsWereSwitched = pdFALSE;
        }

        xLastTime = xTimeNow;

        திரும்பு xTimeNow;
    }
/*-----------------------------------------------------------*/

    நிலையான BaseType_t prv_செருகு_Timer_In_Active_List( Timer_t * மாறிலி pxTimer,
                                                  மாறிலி TickType_t xNextExpiryTime,
                                                  மாறிலி TickType_t xTimeNow,
                                                  மாறிலி TickType_t xCommandTime )
    {
        BaseType_t xProcessTimerNow = pdFALSE;

        list_SET_LIST_ITEM_மதிப்பு( &( pxTimer->xTimerListItem ), xNextExpiryTime );
        listSET_LIST_ITEM_OWNER( &( pxTimer->xTimerListItem ), pxTimer );

        எனில்( xNextExpiryTime <= xTimeNow )
        {
            /* Has the expiry time elapsed between the command to start/reset a
             * timer was issued, and the time the command was processed? */
            எனில்( ( ( TickType_t ) ( xTimeNow - xCommandTime ) ) >= pxTimer->xTimerPeriodInTicks ) /*lint !e961 MISRA exception as the casts are only redundant for some ports. */
            {
                /* The time between a command being issued and the command being
                 * processed actually exceeds the timers period.  */
                xProcessTimerNow = pdTRUE;
            }
            இல்லையெனில்
            {
                v_List_செருகு( pxOverflowTimerList, &( pxTimer->xTimerListItem ) );
            }
        }
        இல்லையெனில்
        {
            எனில்( ( xTimeNow < xCommandTime ) && ( xNextExpiryTime >= xCommandTime ) )
            {
                /* If, since the command was issued, the tick count has overflowed
                 * but the expiry time has not, then the timer must have already passed
                 * its expiry time and should be processed immediately. */
                xProcessTimerNow = pdTRUE;
            }
            இல்லையெனில்
            {
                v_List_செருகு( pxCurrentTimerList, &( pxTimer->xTimerListItem ) );
            }
        }

        திரும்பு xProcessTimerNow;
    }
/*-----------------------------------------------------------*/

    நிலையான வெறுமை prvProcessReceivedCommands( வெறுமை )
    {
        DaemonTaskMessage_t xMessage;
        Timer_t * pxTimer;
        BaseType_t xTimerListsWereSwitched;
        TickType_t xTimeNow;

        போது( xQueueReceive( xTimerQueue, &xMessage, tmrNO_DELAY ) != pdFAIL ) /*lint !e603 xMessage does not have to be initialised as it is passed out, not in, and it is not used unless xQueueReceive() returns pdTRUE. */
        {
            #எனில் ( INCLUDE_xTimerPendFunctionCall == 1 )
            {
                /* Negative commands are pended function calls rather than timer
                 * commands. */
                எனில்( xMessage.xMessageID < ( BaseType_t ) 0 )
                {
                    மாறிலி CallbackParameters_t * மாறிலி pxCallback = &( xMessage.u.xCallbackParameters );

                    /* The timer uses the xCallbackParameters member to request a
                     * callback be executed.  Check the callback is not NULL. */
                    configASSERT( pxCallback );

                    /* Call the function. */
                    pxCallback->pxCallbackFunction( pxCallback->pvParameter1, pxCallback->ulParameter2 );
                }
                இல்லையெனில்
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            #endif /* INCLUDE_xTimerPendFunctionCall */

            /* Commands that are positive are timer commands rather than pended
             * function calls. */
            எனில்( xMessage.xMessageID >= ( BaseType_t ) 0 )
            {
                /* The messages uses the xTimerParameters member to work on a
                 * software timer. */
                pxTimer = xMessage.u.xTimerParameters.pxTimer;

                எனில்( listIS_CONTAINED_WITHIN( வெற்று, &( pxTimer->xTimerListItem ) ) == pdFALSE ) /*lint !e961. The cast is only redundant when NULL is passed into the macro. */
                {
                    /* The timer is in a list, remove it. */
                    ( வெறுமை ) uxListRemove( &( pxTimer->xTimerListItem ) );
                }
                இல்லையெனில்
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                traceTIMER_COMMAND_RECEIVED( pxTimer, xMessage.xMessageID, xMessage.u.xTimerParameters.x_Message_மதிப்பு );

                /* In this case the xTimerListsWereSwitched parameter is not used, but
                 *  it must be present in the function call.  prvSampleTimeNow() must be
                 *  called after the message is received from xTimerQueue so there is no
                 *  possibility of a higher priority task adding a message to the message
                 *  queue with a time that is ahead of the timer daemon task (because it
                 *  pre-empted the timer daemon task after the xTimeNow value was set). */
                xTimeNow = prvSampleTimeNow( &xTimerListsWereSwitched );

                தேர்வு( xMessage.xMessageID )
                {
                    நிலை tmrCOMMAND_START:
                    நிலை tmrCOMMAND_START_FROM_ISR:
                    நிலை tmrCOMMAND_RESET:
                    நிலை tmrCOMMAND_RESET_FROM_ISR:
                        /* Start or restart a timer. */
                        pxTimer->ucStatus |= tmrSTATUS_IS_ACTIVE;

                        எனில்( prv_செருகு_Timer_In_Active_List( pxTimer, xMessage.u.xTimerParameters.x_Message_மதிப்பு + pxTimer->xTimerPeriodInTicks, xTimeNow, xMessage.u.xTimerParameters.x_Message_மதிப்பு ) != pdFALSE )
                        {
                            /* The timer expired before it was added to the active
                             * timer list.  Process it now. */
                            எனில்( ( pxTimer->ucStatus & tmrSTATUS_IS_AUTORELOAD ) != 0 )
                            {
                                prvReloadTimer( pxTimer, xMessage.u.xTimerParameters.x_Message_மதிப்பு + pxTimer->xTimerPeriodInTicks, xTimeNow );
                            }
                            இல்லையெனில்
                            {
                                pxTimer->ucStatus &= ( ( uint8_t ) ~tmrSTATUS_IS_ACTIVE );
                            }

                            /* Call the timer callback. */
                            traceTIMER_EXPIRED( pxTimer );
                            pxTimer->pxCallbackFunction( ( TimerHandle_t ) pxTimer );
                        }
                        இல்லையெனில்
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }

                        நிறுத்து;

                    நிலை tmrCOMMAND_STOP:
                    நிலை tmrCOMMAND_STOP_FROM_ISR:
                        /* The timer has already been removed from the active list. */
                        pxTimer->ucStatus &= ( ( uint8_t ) ~tmrSTATUS_IS_ACTIVE );
                        நிறுத்து;

                    நிலை tmrCOMMAND_CHANGE_PERIOD:
                    நிலை tmrCOMMAND_CHANGE_PERIOD_FROM_ISR:
                        pxTimer->ucStatus |= tmrSTATUS_IS_ACTIVE;
                        pxTimer->xTimerPeriodInTicks = xMessage.u.xTimerParameters.x_Message_மதிப்பு;
                        configASSERT( ( pxTimer->xTimerPeriodInTicks > 0 ) );

                        /* The new period does not really have a reference, and can
                         * be longer or shorter than the old one.  The command time is
                         * therefore set to the current time, and as the period cannot
                         * be zero the next expiry time can only be in the future,
                         * meaning (unlike for the xTimerStart() case above) there is
                         * no fail case that needs to be handled here. */
                        ( வெறுமை ) prv_செருகு_Timer_In_Active_List( pxTimer, ( xTimeNow + pxTimer->xTimerPeriodInTicks ), xTimeNow, xTimeNow );
                        நிறுத்து;

                    நிலை tmrCOMMAND_DELETE:
                        #எனில் ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )
                        {
                            /* The timer has already been removed from the active list,
                             * just free up the memory if the memory was dynamically
                             * allocated. */
                            எனில்( ( pxTimer->ucStatus & tmrSTATUS_IS_STATICALLY_ALLOCATED ) == ( uint8_t ) 0 )
                            {
                                vPortFree( pxTimer );
                            }
                            இல்லையெனில்
                            {
                                pxTimer->ucStatus &= ( ( uint8_t ) ~tmrSTATUS_IS_ACTIVE );
                            }
                        }
                        #இல்லையெனில் /* if ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) */
                        {
                            /* If dynamic allocation is not enabled, the memory
                             * could not have been dynamically allocated. So there is
                             * no need to free the memory - just mark the timer as
                             * "not active". */
                            pxTimer->ucStatus &= ( ( uint8_t ) ~tmrSTATUS_IS_ACTIVE );
                        }
                        #endif /* configSUPPORT_DYNAMIC_ALLOCATION */
                        நிறுத்து;

                    இயல்பு:
                        /* Don't expect to get here. */
                        நிறுத்து;
                }
            }
        }
    }
/*-----------------------------------------------------------*/

    நிலையான வெறுமை prvSwitchTimerLists( வெறுமை )
    {
        TickType_t xNextExpireTime;
        List_t * pxTemp;

        /* The tick count has overflowed.  The timer lists must be switched.
         * If there are any timers still referenced from the current timer list
         * then they must have expired and should be processed before the lists
         * are switched. */
        போது( listLIST_IS_EMPTY( pxCurrentTimerList ) == pdFALSE )
        {
            xNextExpireTime = list_GET_ITEM_மதிப்பு_OF_HEAD_ENTRY( pxCurrentTimerList );

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

    நிலையான வெறுமை prvCheckForValidListAndQueue( வெறுமை )
    {
        /* Check that the list from which active timers are referenced, and the
         * queue used to communicate with the timer service, have been
         * initialised. */
        taskENTER_CRITICAL( &xTimerLock );
        {
            எனில்( xTimerQueue == வெற்று )
            {
                vListInitialise( &xActiveTimerList1 );
                vListInitialise( &xActiveTimerList2 );
                pxCurrentTimerList = &xActiveTimerList1;
                pxOverflowTimerList = &xActiveTimerList2;

                #எனில் ( configSUPPORT_STATIC_ALLOCATION == 1 )
                {
                    /* The timer queue is allocated statically in case
                     * configSUPPORT_DYNAMIC_ALLOCATION is 0. */
                    PRIVILEGED_DATA நிலையான StaticQueue_t xStaticTimerQueue;                                                                          /*lint !e956 Ok to declare in this manner to prevent additional conditional compilation guards in other locations. */
                    PRIVILEGED_DATA நிலையான uint8_t ucStaticTimerQueueStorage[ ( size_t ) configTIMER_QUEUE_LENGTH * அளவு( DaemonTaskMessage_t ) ]; /*lint !e956 Ok to declare in this manner to prevent additional conditional compilation guards in other locations. */

                    xTimerQueue = xQueueCreateStatic( ( UBaseType_t ) configTIMER_QUEUE_LENGTH, ( UBaseType_t ) அளவு( DaemonTaskMessage_t ), &( ucStaticTimerQueueStorage[ 0 ] ), &xStaticTimerQueue );
                }
                #இல்லையெனில்
                {
                    xTimerQueue = xQueueCreate( ( UBaseType_t ) configTIMER_QUEUE_LENGTH, அளவு( DaemonTaskMessage_t ) );
                }
                #endif /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */

                #எனில் ( configQUEUE_REGISTRY_SIZE > 0 )
                {
                    எனில்( xTimerQueue != வெற்று )
                    {
                        vQueueAddToRegistry( xTimerQueue, "TmrQ" );
                    }
                    இல்லையெனில்
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #endif /* configQUEUE_REGISTRY_SIZE */
            }
            இல்லையெனில்
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
            எனில்( ( pxTimer->ucStatus & tmrSTATUS_IS_ACTIVE ) == 0 )
            {
                xReturn = pdFALSE;
            }
            இல்லையெனில்
            {
                xReturn = pdTRUE;
            }
        }
        taskEXIT_CRITICAL( &xTimerLock );

        திரும்பு xReturn;
    } /*lint !e818 Can't be pointer to const due to the typedef. */
/*-----------------------------------------------------------*/

    வெறுமை * pvTimerGetTimerID( மாறிலி TimerHandle_t xTimer )
    {
        Timer_t * மாறிலி pxTimer = xTimer;
        வெறுமை * pvReturn;

        configASSERT( xTimer );

        taskENTER_CRITICAL( &xTimerLock );
        {
            pvReturn = pxTimer->pvTimerID;
        }
        taskEXIT_CRITICAL( &xTimerLock );

        திரும்பு pvReturn;
    }
/*-----------------------------------------------------------*/

    வெறுமை vTimerSetTimerID( TimerHandle_t xTimer,
                           வெறுமை * pvNewID )
    {
        Timer_t * மாறிலி pxTimer = xTimer;

        configASSERT( xTimer );

        taskENTER_CRITICAL( &xTimerLock );
        {
            pxTimer->pvTimerID = pvNewID;
        }
        taskEXIT_CRITICAL( &xTimerLock );
    }
/*-----------------------------------------------------------*/

    #எனில் ( INCLUDE_xTimerPendFunctionCall == 1 )

        BaseType_t xTimerPendFunctionCallFromISR( PendedFunction_t xFunctionToPend,
                                                  வெறுமை * pvParameter1,
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

            திரும்பு xReturn;
        }

    #endif /* INCLUDE_xTimerPendFunctionCall */
/*-----------------------------------------------------------*/

    #எனில் ( INCLUDE_xTimerPendFunctionCall == 1 )

        BaseType_t xTimerPendFunctionCall( PendedFunction_t xFunctionToPend,
                                           வெறுமை * pvParameter1,
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

            திரும்பு xReturn;
        }

    #endif /* INCLUDE_xTimerPendFunctionCall */
/*-----------------------------------------------------------*/

    #எனில் ( configUSE_TRACE_FACILITY == 1 )

        UBaseType_t ux_Timer_Get_Timer_எண்( TimerHandle_t xTimer )
        {
            திரும்பு ( ( Timer_t * ) xTimer )->ux_Timer_எண்;
        }

    #endif /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

    #எனில் ( configUSE_TRACE_FACILITY == 1 )

        வெறுமை v_Timer_Set_Timer_எண்( TimerHandle_t xTimer,
                                   UBaseType_t ux_Timer_எண் )
        {
            ( ( Timer_t * ) xTimer )->ux_Timer_எண் = ux_Timer_எண்;
        }

    #endif /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

/* This entire source file will be skipped if the application is not configured
 * to include software timer functionality.  If you want to include software timer
 * functionality then ensure configUSE_TIMERS is set to 1 in FreeRTOSConfig.h. */
#endif /* configUSE_TIMERS == 1 */
