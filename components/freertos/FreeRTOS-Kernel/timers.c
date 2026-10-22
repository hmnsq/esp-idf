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
#अन्तर्भाव <stdlib.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#परिभाषय MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#अन्तर्भाव "FreeRTOS.h"
#अन्तर्भाव "task.h"
#अन्तर्भाव "queue.h"
#अन्तर्भाव "timers.h"
/* Include private IDF API additions for critical thread safety macros */
#अन्तर्भाव "esp_private/freertos_idf_additions_priv.h"

#यदि ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 0 )
    #दोषः configUSE_TIMERS must be समुच्चयः गन्तव्यम् 1 गन्तव्यम् make the xTimerPendFunctionCall() function उपलब्धम्.
#यदि_अन्तः

/* Lint e9021, e961 and e750 are suppressed as a MISRA exception justified
 * because the MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#अपरिभाषय MPU_WRAPPERS_INCLUDED_FROM_API_FILE /*lint !e9021 !e961 !e750. */


/* This entire source file will be skipped if the application is not configured
 * to include software timer functionality.  This #if is closed at the very bottom
 * of this file.  If you want to include software timer functionality then ensure
 * configUSE_TIMERS is set to 1 in FreeRTOSConfig.h. */
#यदि ( configUSE_TIMERS == 1 )

/* Misc definitions. */
    #परिभाषय tmrNO_DELAY                    ( ( TickType_t ) 0U )
    #परिभाषय tmr_MAX_TIME_BEFORE_अतिप्रवाहः    ( ( TickType_t ) -1 )

/* The name assigned to the timer service task. This can be overridden by
 * defining configTIMER_SERVICE_TASK_NAME in FreeRTOSConfig.h. */
    #यदि_अपरिभाषितम् configTIMER_SERVICE_TASK_NAME
        #परिभाषय configTIMER_SERVICE_TASK_NAME    "Tmr Svc"
    #यदि_अन्तः

    #यदि ( config_सङ्ख्या_सम्बन्धि_CORES > 1 )

/* The core affinity assigned to the timer service task on SMP systems.
 * This can be overridden by defining configTIMER_SERVICE_TASK_CORE_AFFINITY in FreeRTOSConfig.h. */
        #यदि_अपरिभाषितम् configTIMER_SERVICE_TASK_CORE_AFFINITY
            #परिभाषय configTIMER_SERVICE_TASK_CORE_AFFINITY    tskNO_AFFINITY
        #यदि_अन्तः

        #यदि ( !( ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x0 ) || ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x1 ) || ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x7FFFFFFF /* tskNO_AFFINITY */ ) ) )
            #दोषः Invalid Timer task affinity. Timer task core ID can only be 0x0, 0x1 or 0x7FFFFFFF (tskNO_AFFINITY).
        #यदि_अन्तः /* #if ( !( ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x0 ) || ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x01 ) || ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x7FFFFFFF ) ) ) */
    #यदि_अन्तः /* #if ( configNUMBER_OF_CORES > 1 ) */

/* Bit definitions used in the ucStatus member of a timer structure. */
    #परिभाषय tmr_स्थितिः_IS_सक्रिय                  ( ( uint8_t ) 0x01 )
    #परिभाषय tmr_स्थितिः_IS_STATICALLY_ALLOCATED    ( ( uint8_t ) 0x02 )
    #परिभाषय tmr_स्थितिः_IS_AUTORELOAD              ( ( uint8_t ) 0x04 )

/* The definition of the timers themselves. */
    प्रकारनिर्देश संरचना tmrTimerControl                  /* The old naming convention is used to prevent breaking kernel aware debuggers. */
    {
        ध्रुव वर्ण * pcTimerName;                   /*<< Text name.  This is not used by the kernel, it is included simply to make debugging easier. */ /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
        ListItem_t xTimerListItem;                  /*<< Standard linked list item as used by all kernel features for event management. */
        TickType_t x_Timer_Period_अन्तर्_Ticks;             /*<< How quickly and often the timer expires. */
        शून्य * pvTimerID;                           /*<< An ID to identify the timer.  This allows the timer to be identified when the same callback is used for multiple timers. */
        TimerCallbackFunction_t pxCallbackFunction; /*<< The function that will be called when the timer expires. */
        #यदि ( configUSE_TRACE_FACILITY == 1 )
            UBaseType_t ux_Timer_सङ्ख्या;              /*<< An ID assigned by trace tools such as FreeRTOS+Trace */
        #यदि_अन्तः
        uint8_t uc_स्थितिः;                           /*<< Holds bits to say if the timer was statically allocated or not, and if it is active or not. */
    } xTIMER;

/* The old xTIMER name is maintained above then typedefed to the new Timer_t
 * name below to enable the use of older kernel aware debuggers. */
    प्रकारनिर्देश xTIMER Timer_t;

/* The definition of messages that can be sent and received on the timer queue.
 * Two types of message can be queued - messages that manipulate a software timer,
 * and messages that request the execution of a non-timer related callback.  The
 * two message types are defined in two separate structures, xTimerParametersType
 * and xCallbackParametersType respectively. */
    प्रकारनिर्देश संरचना tmrTimerParameters
    {
        TickType_t x_सन्देशः_मानम्; /*<< An optional value used by a subset of commands, for example, when changing the period of a timer. */
        Timer_t * pxTimer;        /*<< The timer to which the command will be applied. */
    } TimerParameter_t;


    प्रकारनिर्देश संरचना tmrCallbackParameters
    {
        PendedFunction_t pxCallbackFunction; /* << The callback function to execute. */
        शून्य * pvParameter1;                 /* << The value that will be used as the callback functions first parameter. */
        uint32_t ulParameter2;               /* << The value that will be used as the callback functions second parameter. */
    } CallbackParameters_t;

/* The structure that contains the two message types, along with an identifier
 * that is used to determine which message type is valid. */
    प्रकारनिर्देश संरचना tmr_Timer_Queue_सन्देशः
    {
        आधारः_Type_t x_सन्देशः_ID; /*<< The command being sent to the timer service task. */
        सङ्घ
        {
            TimerParameter_t xTimerParameters;

            /* Don't include xCallbackParameters if it is not going to be used as
             * it makes the structure (and therefore the timer queue) larger. */
            #यदि ( INCLUDE_xTimerPendFunctionCall == 1 )
                CallbackParameters_t xCallbackParameters;
            #यदि_अन्तः /* INCLUDE_xTimerPendFunctionCall */
        } u;
    } Daemon_Task_सन्देशः_t;

/*lint -save -e956 A manual analysis and inspection has been used to determine
 * which static variables must be declared volatile. */

/* The list in which active timers are stored.  Timers are referenced in expire
 * time order, with the nearest expiry time at the front of the list.  Only the
 * timer service task is allowed to access these lists.
 * xActiveTimerList1 and xActiveTimerList2 could be at function scope but that
 * breaks some kernel aware debuggers, and debuggers that reply on removing the
 * static qualifier. */
    PRIVILEGED_दत्त स्थिर List_t x_सक्रिय_Timer_List1;
    PRIVILEGED_दत्त स्थिर List_t x_सक्रिय_Timer_List2;
    PRIVILEGED_दत्त स्थिर List_t * pxCurrentTimerList;
    PRIVILEGED_दत्त स्थिर List_t * px_अतिप्रवाहः_Timer_List;

/* A queue that is used to send commands to the timer service task. */
    PRIVILEGED_दत्त स्थिर QueueHandle_t xTimerQueue = रिक्त;
    PRIVILEGED_दत्त स्थिर TaskHandle_t xTimerTaskHandle = रिक्त;

/* Spinlock required in SMP when accessing the timers. For now we use a single lock
 * Todo: Each timer could possible have its own lock for increased granularity. */
    PRIVILEGED_दत्त स्थिर पत्तनम्_MUX_TYPE x_Timer_तालकम् = पत्तनम्_MUX_INITIALIZER_UNLOCKED;

/*lint -restore */

/*-----------------------------------------------------------*/

/*
 * Initialise the infrastructure used by the timer service task if it has not
 * been initialised already.
 */
    स्थिर शून्य prvCheckForValidListAndQueue( शून्य ) PRIVILEGED_FUNCTION;

/*
 * The timer service task (daemon).  Timer functionality is controlled by this
 * task.  Other tasks communicate with the timer service task using the
 * xTimerQueue queue.
 */
    स्थिर पत्तनम्_TASK_FUNCTION_PROTO( prvTimerTask, pvParameters ) PRIVILEGED_FUNCTION;

/*
 * Called by the timer service task to interpret and process a command it
 * received on the timer queue.
 */
    स्थिर शून्य prvProcessReceivedCommands( शून्य ) PRIVILEGED_FUNCTION;

/*
 * Insert the timer into either xActiveTimerList1, or xActiveTimerList2,
 * depending on if the expire time causes a timer counter overflow.
 */
    स्थिर आधारः_Type_t prv_निवेशय_Timer_अन्तर्_सक्रिय_List( Timer_t * ध्रुव pxTimer,
                                                  ध्रुव TickType_t x_अग्र_Expiry_Time,
                                                  ध्रुव TickType_t xTimeNow,
                                                  ध्रुव TickType_t xCommandTime ) PRIVILEGED_FUNCTION;

/*
 * Reload the specified auto-reload timer.  If the reloading is backlogged,
 * clear the backlog, calling the callback for each additional reload.  When
 * this function returns, the next expiry time is after xTimeNow.
 */
    स्थिर शून्य prvReloadTimer( Timer_t * ध्रुव pxTimer,
                                TickType_t xExpiredTime,
                                ध्रुव TickType_t xTimeNow ) PRIVILEGED_FUNCTION;

/*
 * An active timer has reached its expire time.  Reload the timer if it is an
 * auto-reload timer, then call its callback.
 */
    स्थिर शून्य prvProcessExpiredTimer( ध्रुव TickType_t x_अग्र_Expire_Time,
                                        ध्रुव TickType_t xTimeNow ) PRIVILEGED_FUNCTION;

/*
 * The tick count has overflowed.  Switch the timer lists after ensuring the
 * current timer list does not still reference some timers.
 */
    स्थिर शून्य prvSwitchTimerLists( शून्य ) PRIVILEGED_FUNCTION;

/*
 * Obtain the current tick count, setting *pxTimerListsWereSwitched to pdTRUE
 * if a tick count overflow occurred since prvSampleTimeNow() was last called.
 */
    स्थिर TickType_t prvSampleTimeNow( आधारः_Type_t * ध्रुव pxTimerListsWereSwitched ) PRIVILEGED_FUNCTION;

/*
 * If the timer list contains any active timers then return the expire time of
 * the timer that will expire first and set *pxListWasEmpty to false.  If the
 * timer list does not contain any timers then return 0 and set *pxListWasEmpty
 * to pdTRUE.
 */
    स्थिर TickType_t prv_Get_अग्र_Expire_Time( आधारः_Type_t * ध्रुव px_List_Was_रिक्तानि ) PRIVILEGED_FUNCTION;

/*
 * If a timer has expired, process it.  Otherwise, block the timer service task
 * until either a timer does expire or a command is received.
 */
    स्थिर शून्य prv_Process_Timer_Or_खण्डम्_Task( ध्रुव TickType_t x_अग्र_Expire_Time,
                                            आधारः_Type_t x_List_Was_रिक्तानि ) PRIVILEGED_FUNCTION;

/*
 * Called after a Timer_t structure has been allocated either statically or
 * dynamically to fill in the structure's members.
 */
    स्थिर शून्य prvInitialiseNewTimer( ध्रुव वर्ण * ध्रुव pcTimerName, /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                       ध्रुव TickType_t x_Timer_Period_अन्तर्_Ticks,
                                       ध्रुव आधारः_Type_t xAutoReload,
                                       शून्य * ध्रुव pvTimerID,
                                       TimerCallbackFunction_t pxCallbackFunction,
                                       Timer_t * pxNewTimer ) PRIVILEGED_FUNCTION;
/*-----------------------------------------------------------*/

    आधारः_Type_t xTimerCreateTimerTask( शून्य )
    {
        आधारः_Type_t xReturn = pdFAIL;

        /* This function is called when the scheduler is started if
         * configUSE_TIMERS is set to 1.  Check that the infrastructure used by the
         * timer service task has been created/initialised.  If timers have already
         * been created then the initialisation will already have been performed. */
        prvCheckForValidListAndQueue();

        यदि( xTimerQueue != रिक्त )
        {
            /* The following code block to set the timer task core affinity has been picked up from the FreeRTOS-Kernel
             * upstream repository. Commit# 631ae9e6e4828cf5c7d58bb7ac78a64fc309bcb7.
             * Necessary changes have been made for IDF-FreeRTOS compatibility. */
            #यदि ( config_सङ्ख्या_सम्बन्धि_CORES > 1 )
            {
                #यदि ( config_SUPPORT_STATIC_आवण्टनम् == 1 )
                {
                    StaticTask_t * pxTimerTaskTCBBuffer = रिक्त;
                    राशिः_Type_t * px_Timer_Task_राशिः_बफर = रिक्त;
                    uint32_t ul_Timer_Task_राशिः_आकार;

                    v_Application_Get_Timer_Task_स्मृतिः( &pxTimerTaskTCBBuffer, &px_Timer_Task_राशिः_बफर, &ul_Timer_Task_राशिः_आकार );
                    xTimerTaskHandle = x_Task_Create_Static_Pinned_गन्तव्यम्_Core( prvTimerTask,
                                                                      configTIMER_SERVICE_TASK_NAME,
                                                                      ul_Timer_Task_राशिः_आकार,
                                                                      रिक्त,
                                                                      ( ( UBaseType_t ) config_TIMER_TASK_प्राथमिकता ) | पत्तनम्_PRIVILEGE_BIT,
                                                                      px_Timer_Task_राशिः_बफर,
                                                                      pxTimerTaskTCBBuffer,
                                                                      configTIMER_SERVICE_TASK_CORE_AFFINITY );

                    यदि( xTimerTaskHandle != रिक्त )
                    {
                        xReturn = pdPASS;
                    }
                }
                #अन्यथा /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
                {
                    xReturn = x_Task_Create_Pinned_गन्तव्यम्_Core( prvTimerTask,
                                                       configTIMER_SERVICE_TASK_NAME,
                                                       config_TIMER_TASK_राशिः_गभीरता,
                                                       रिक्त,
                                                       ( ( UBaseType_t ) config_TIMER_TASK_प्राथमिकता ) | पत्तनम्_PRIVILEGE_BIT,
                                                       &xTimerTaskHandle,
                                                       configTIMER_SERVICE_TASK_CORE_AFFINITY );
                }
                #यदि_अन्तः /* configSUPPORT_STATIC_ALLOCATION */
            }
            #अन्यथा /* #if ( configNUMBER_OF_CORES > 1 ) */
            {
                #यदि ( config_SUPPORT_STATIC_आवण्टनम् == 1 )
                {
                    StaticTask_t * pxTimerTaskTCBBuffer = रिक्त;
                    राशिः_Type_t * px_Timer_Task_राशिः_बफर = रिक्त;
                    uint32_t ul_Timer_Task_राशिः_आकार;

                    v_Application_Get_Timer_Task_स्मृतिः( &pxTimerTaskTCBBuffer, &px_Timer_Task_राशिः_बफर, &ul_Timer_Task_राशिः_आकार );
                    xTimerTaskHandle = xTaskCreateStatic( prvTimerTask,
                                                          configTIMER_SERVICE_TASK_NAME,
                                                          ul_Timer_Task_राशिः_आकार,
                                                          रिक्त,
                                                          ( ( UBaseType_t ) config_TIMER_TASK_प्राथमिकता ) | पत्तनम्_PRIVILEGE_BIT,
                                                          px_Timer_Task_राशिः_बफर,
                                                          pxTimerTaskTCBBuffer );

                    यदि( xTimerTaskHandle != रिक्त )
                    {
                        xReturn = pdPASS;
                    }
                }
                #अन्यथा /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
                {
                    xReturn = xTaskCreate( prvTimerTask,
                                           configTIMER_SERVICE_TASK_NAME,
                                           config_TIMER_TASK_राशिः_गभीरता,
                                           रिक्त,
                                           ( ( UBaseType_t ) config_TIMER_TASK_प्राथमिकता ) | पत्तनम्_PRIVILEGE_BIT,
                                           &xTimerTaskHandle );
                }
                #यदि_अन्तः /* configSUPPORT_STATIC_ALLOCATION */
            }
            #यदि_अन्तः /* #if ( configNUMBER_OF_CORES > 1 ) */
        }
        अन्यथा
        {
            mtCOVERAGE_TEST_MARKER();
        }

        configASSERT( xReturn );
        प्रत्यागम xReturn;
    }
/*-----------------------------------------------------------*/

    #यदि ( config_SUPPORT_DYNAMIC_आवण्टनम् == 1 )

        TimerHandle_t xTimerCreate( ध्रुव वर्ण * ध्रुव pcTimerName, /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                    ध्रुव TickType_t x_Timer_Period_अन्तर्_Ticks,
                                    ध्रुव आधारः_Type_t xAutoReload,
                                    शून्य * ध्रुव pvTimerID,
                                    TimerCallbackFunction_t pxCallbackFunction )
        {
            Timer_t * pxNewTimer;

            pxNewTimer = ( Timer_t * ) pv_पत्तनम्_Malloc( परिमाण( Timer_t ) ); /*lint !e9087 !e9079 All values returned by pvPortMalloc() have at least the alignment required by the MCU's stack, and the first member of Timer_t is always a pointer to the timer's mame. */

            यदि( pxNewTimer != रिक्त )
            {
                /* Status is thus far zero as the timer is not created statically
                 * and has not been started.  The auto-reload bit may get set in
                 * prvInitialiseNewTimer. */
                pxNewTimer->uc_स्थितिः = 0x00;
                prvInitialiseNewTimer( pcTimerName, x_Timer_Period_अन्तर्_Ticks, xAutoReload, pvTimerID, pxCallbackFunction, pxNewTimer );
            }

            प्रत्यागम pxNewTimer;
        }

    #यदि_अन्तः /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

    #यदि ( config_SUPPORT_STATIC_आवण्टनम् == 1 )

        TimerHandle_t xTimerCreateStatic( ध्रुव वर्ण * ध्रुव pcTimerName, /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                          ध्रुव TickType_t x_Timer_Period_अन्तर्_Ticks,
                                          ध्रुव आधारः_Type_t xAutoReload,
                                          शून्य * ध्रुव pvTimerID,
                                          TimerCallbackFunction_t pxCallbackFunction,
                                          StaticTimer_t * px_Timer_बफर )
        {
            Timer_t * pxNewTimer;

            #यदि ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticTimer_t equals the size of the real timer
                 * structure. */
                चञ्चल size_t x_आकार = परिमाण( StaticTimer_t );
                configASSERT( x_आकार == परिमाण( Timer_t ) );
                ( शून्य ) x_आकार; /* Keeps lint quiet when configASSERT() is not defined. */
            }
            #यदि_अन्तः /* configASSERT_DEFINED */

            /* A pointer to a StaticTimer_t structure MUST be provided, use it. */
            configASSERT( px_Timer_बफर );
            pxNewTimer = ( Timer_t * ) px_Timer_बफर; /*lint !e740 !e9087 StaticTimer_t is a pointer to a Timer_t, so guaranteed to be aligned and sized correctly (checked by an assert()), so this is safe. */

            यदि( pxNewTimer != रिक्त )
            {
                /* Timers can be created statically or dynamically so note this
                 * timer was created statically in case it is later deleted.  The
                 * auto-reload bit may get set in prvInitialiseNewTimer(). */
                pxNewTimer->uc_स्थितिः = tmr_स्थितिः_IS_STATICALLY_ALLOCATED;

                prvInitialiseNewTimer( pcTimerName, x_Timer_Period_अन्तर्_Ticks, xAutoReload, pvTimerID, pxCallbackFunction, pxNewTimer );
            }

            प्रत्यागम pxNewTimer;
        }

    #यदि_अन्तः /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

    स्थिर शून्य prvInitialiseNewTimer( ध्रुव वर्ण * ध्रुव pcTimerName, /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                       ध्रुव TickType_t x_Timer_Period_अन्तर्_Ticks,
                                       ध्रुव आधारः_Type_t xAutoReload,
                                       शून्य * ध्रुव pvTimerID,
                                       TimerCallbackFunction_t pxCallbackFunction,
                                       Timer_t * pxNewTimer )
    {
        /* 0 is not a valid value for xTimerPeriodInTicks. */
        configASSERT( ( x_Timer_Period_अन्तर्_Ticks > 0 ) );

        /* Ensure the infrastructure used by the timer service task has been
         * created/initialised. */
        prvCheckForValidListAndQueue();

        /* Initialise the timer structure members using the function
         * parameters. */
        pxNewTimer->pcTimerName = pcTimerName;
        pxNewTimer->x_Timer_Period_अन्तर्_Ticks = x_Timer_Period_अन्तर्_Ticks;
        pxNewTimer->pvTimerID = pvTimerID;
        pxNewTimer->pxCallbackFunction = pxCallbackFunction;
        vListInitialiseItem( &( pxNewTimer->xTimerListItem ) );

        यदि( xAutoReload != pdFALSE )
        {
            pxNewTimer->uc_स्थितिः |= tmr_स्थितिः_IS_AUTORELOAD;
        }

        traceTIMER_CREATE( pxNewTimer );
    }
/*-----------------------------------------------------------*/

    आधारः_Type_t xTimerGenericCommand( TimerHandle_t xTimer,
                                     ध्रुव आधारः_Type_t xCommandID,
                                     ध्रुव TickType_t x_Optional_मानम्,
                                     आधारः_Type_t * ध्रुव px_Higher_प्राथमिकता_Task_Woken,
                                     ध्रुव TickType_t x_Ticks_गन्तव्यम्_Wait )
    {
        आधारः_Type_t xReturn = pdFAIL;
        Daemon_Task_सन्देशः_t x_सन्देशः;

        configASSERT( xTimer );

        /* Send a message to the timer service task to perform a particular action
         * on a particular timer definition. */
        यदि( xTimerQueue != रिक्त )
        {
            /* Send a command to the timer service task to start the xTimer timer. */
            x_सन्देशः.x_सन्देशः_ID = xCommandID;
            x_सन्देशः.u.xTimerParameters.x_सन्देशः_मानम् = x_Optional_मानम्;
            x_सन्देशः.u.xTimerParameters.pxTimer = xTimer;

            यदि( xCommandID < tmrFIRST_FROM_ISR_COMMAND )
            {
                यदि( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING )
                {
                    xReturn = x_Queue_Send_गन्तव्यम्_पृष्ठभागः( xTimerQueue, &x_सन्देशः, x_Ticks_गन्तव्यम्_Wait );
                }
                अन्यथा
                {
                    xReturn = x_Queue_Send_गन्तव्यम्_पृष्ठभागः( xTimerQueue, &x_सन्देशः, tmrNO_DELAY );
                }
            }
            अन्यथा
            {
                xReturn = x_Queue_Send_गन्तव्यम्_पृष्ठभागः_From_ISR( xTimerQueue, &x_सन्देशः, px_Higher_प्राथमिकता_Task_Woken );
            }

            traceTIMER_COMMAND_SEND( xTimer, xCommandID, x_Optional_मानम्, xReturn );
        }
        अन्यथा
        {
            mtCOVERAGE_TEST_MARKER();
        }

        प्रत्यागम xReturn;
    }
/*-----------------------------------------------------------*/

    TaskHandle_t xTimerGetTimerDaemonTaskHandle( शून्य )
    {
        /* If xTimerGetTimerDaemonTaskHandle() is called before the scheduler has been
         * started, then xTimerTaskHandle will be NULL. */
        configASSERT( ( xTimerTaskHandle != रिक्त ) );
        प्रत्यागम xTimerTaskHandle;
    }
/*-----------------------------------------------------------*/

    TickType_t xTimerGetPeriod( TimerHandle_t xTimer )
    {
        Timer_t * pxTimer = xTimer;

        configASSERT( xTimer );
        प्रत्यागम pxTimer->x_Timer_Period_अन्तर्_Ticks;
    }
/*-----------------------------------------------------------*/

    शून्य v_Timer_समुच्चयः_Reload_बहुलकम्( TimerHandle_t xTimer,
                              ध्रुव आधारः_Type_t xAutoReload )
    {
        Timer_t * pxTimer = xTimer;

        configASSERT( xTimer );
        taskENTER_CRITICAL( &x_Timer_तालकम् );
        {
            यदि( xAutoReload != pdFALSE )
            {
                pxTimer->uc_स्थितिः |= tmr_स्थितिः_IS_AUTORELOAD;
            }
            अन्यथा
            {
                pxTimer->uc_स्थितिः &= ( ( uint8_t ) ~tmr_स्थितिः_IS_AUTORELOAD );
            }
        }
        taskEXIT_CRITICAL( &x_Timer_तालकम् );
    }
/*-----------------------------------------------------------*/

    आधारः_Type_t x_Timer_Get_Reload_बहुलकम्( TimerHandle_t xTimer )
    {
        Timer_t * pxTimer = xTimer;
        आधारः_Type_t xReturn;

        configASSERT( xTimer );
        taskENTER_CRITICAL( &x_Timer_तालकम् );
        {
            यदि( ( pxTimer->uc_स्थितिः & tmr_स्थितिः_IS_AUTORELOAD ) == 0 )
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
        taskEXIT_CRITICAL( &x_Timer_तालकम् );

        प्रत्यागम xReturn;
    }

    UBaseType_t ux_Timer_Get_Reload_बहुलकम्( TimerHandle_t xTimer )
    {
        प्रत्यागम ( UBaseType_t ) x_Timer_Get_Reload_बहुलकम्( xTimer );
    }
/*-----------------------------------------------------------*/

    TickType_t xTimerGetExpiryTime( TimerHandle_t xTimer )
    {
        Timer_t * pxTimer = xTimer;
        TickType_t xReturn;

        configASSERT( xTimer );
        xReturn = list_GET_LIST_ITEM_मानम्( &( pxTimer->xTimerListItem ) );
        प्रत्यागम xReturn;
    }
/*-----------------------------------------------------------*/

    #यदि ( config_SUPPORT_STATIC_आवण्टनम् == 1 )
        आधारः_Type_t x_Timer_Get_Static_बफर( TimerHandle_t xTimer,
                                          StaticTimer_t ** ppx_Timer_बफर )
        {
            आधारः_Type_t xReturn;
            Timer_t * pxTimer = xTimer;

            configASSERT( ppx_Timer_बफर != रिक्त );

            यदि( ( pxTimer->uc_स्थितिः & tmr_स्थितिः_IS_STATICALLY_ALLOCATED ) != 0 )
            {
                *ppx_Timer_बफर = ( StaticTimer_t * ) pxTimer;
                xReturn = pdTRUE;
            }
            अन्यथा
            {
                xReturn = pdFALSE;
            }

            प्रत्यागम xReturn;
        }
    #यदि_अन्तः /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

    ध्रुव वर्ण * pcTimerGetName( TimerHandle_t xTimer ) /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
    {
        Timer_t * pxTimer = xTimer;

        configASSERT( xTimer );
        प्रत्यागम pxTimer->pcTimerName;
    }
/*-----------------------------------------------------------*/

    स्थिर शून्य prvReloadTimer( Timer_t * ध्रुव pxTimer,
                                TickType_t xExpiredTime,
                                ध्रुव TickType_t xTimeNow )
    {
        /* Insert the timer into the appropriate list for the next expiry time.
         * If the next expiry time has already passed, advance the expiry time,
         * call the callback function, and try again. */
        यावत्( prv_निवेशय_Timer_अन्तर्_सक्रिय_List( pxTimer, ( xExpiredTime + pxTimer->x_Timer_Period_अन्तर्_Ticks ), xTimeNow, xExpiredTime ) != pdFALSE )
        {
            /* Advance the expiry time. */
            xExpiredTime += pxTimer->x_Timer_Period_अन्तर्_Ticks;

            /* Call the timer callback. */
            traceTIMER_EXPIRED( pxTimer );
            pxTimer->pxCallbackFunction( ( TimerHandle_t ) pxTimer );
        }
    }
/*-----------------------------------------------------------*/

    स्थिर शून्य prvProcessExpiredTimer( ध्रुव TickType_t x_अग्र_Expire_Time,
                                        ध्रुव TickType_t xTimeNow )
    {
        Timer_t * ध्रुव pxTimer = ( Timer_t * ) list_GET_OWNER_सम्बन्धि_शीर्ष_प्रविष्टिः( pxCurrentTimerList ); /*lint !e9087 !e9079 void * is used as this macro is used with tasks and co-routines too.  Alignment is known to be fine as the type of the pointer stored and retrieved is the same. */

        /* Remove the timer from the list of active timers.  A check has already
         * been performed to ensure the list is not empty. */

        ( शून्य ) uxListRemove( &( pxTimer->xTimerListItem ) );

        /* If the timer is an auto-reload timer then calculate the next
         * expiry time and re-insert the timer in the list of active timers. */
        यदि( ( pxTimer->uc_स्थितिः & tmr_स्थितिः_IS_AUTORELOAD ) != 0 )
        {
            prvReloadTimer( pxTimer, x_अग्र_Expire_Time, xTimeNow );
        }
        अन्यथा
        {
            pxTimer->uc_स्थितिः &= ( ( uint8_t ) ~tmr_स्थितिः_IS_सक्रिय );
        }

        /* Call the timer callback. */
        traceTIMER_EXPIRED( pxTimer );
        pxTimer->pxCallbackFunction( ( TimerHandle_t ) pxTimer );
    }
/*-----------------------------------------------------------*/

    स्थिर पत्तनम्_TASK_FUNCTION( prvTimerTask, pvParameters )
    {
        TickType_t x_अग्र_Expire_Time;
        आधारः_Type_t x_List_Was_रिक्तानि;

        /* Just to avoid compiler warnings. */
        ( शून्य ) pvParameters;

        #यदि ( configUSE_DAEMON_TASK_STARTUP_HOOK == 1 )
        {
            बाह्य शून्य vApplicationDaemonTaskStartupHook( शून्य );

            /* Allow the application writer to execute some code in the context of
             * this task at the point the task starts executing.  This is useful if the
             * application includes initialisation code that would benefit from
             * executing after the scheduler has been started. */
            vApplicationDaemonTaskStartupHook();
        }
        #यदि_अन्तः /* configUSE_DAEMON_TASK_STARTUP_HOOK */

        प्रति( ; ; )
        {
            /* Query the timers list to see if it contains any timers, and if so,
             * obtain the time at which the next timer will expire. */
            x_अग्र_Expire_Time = prv_Get_अग्र_Expire_Time( &x_List_Was_रिक्तानि );

            /* If a timer has expired, process it.  Otherwise, block this task
             * until either a timer does expire, or a command is received. */
            prv_Process_Timer_Or_खण्डम्_Task( x_अग्र_Expire_Time, x_List_Was_रिक्तानि );

            /* Empty the command queue. */
            prvProcessReceivedCommands();
        }
    }
/*-----------------------------------------------------------*/

    स्थिर शून्य prv_Process_Timer_Or_खण्डम्_Task( ध्रुव TickType_t x_अग्र_Expire_Time,
                                            आधारः_Type_t x_List_Was_रिक्तानि )
    {
        TickType_t xTimeNow;
        आधारः_Type_t xTimerListsWereSwitched;

        prvENTER_CRITICAL_OR_SUSPEND_ALL( &x_Timer_तालकम् );
        {
            /* Obtain the time now to make an assessment as to whether the timer
             * has expired or not.  If obtaining the time causes the lists to switch
             * then don't process this timer as any timers that remained in the list
             * when the lists were switched will have been processed within the
             * prvSampleTimeNow() function. */
            xTimeNow = prvSampleTimeNow( &xTimerListsWereSwitched );

            यदि( xTimerListsWereSwitched == pdFALSE )
            {
                /* The tick count has not overflowed, has the timer expired? */
                यदि( ( x_List_Was_रिक्तानि == pdFALSE ) && ( x_अग्र_Expire_Time <= xTimeNow ) )
                {
                    ( शून्य ) prvEXIT_CRITICAL_OR_RESUME_ALL( &x_Timer_तालकम् );
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
                    यदि( x_List_Was_रिक्तानि != pdFALSE )
                    {
                        /* The current timer list is empty - is the overflow list
                         * also empty? */
                        x_List_Was_रिक्तानि = list_LIST_IS_रिक्तानि( px_अतिप्रवाहः_Timer_List );
                    }

                    v_Queue_Wait_For_सन्देशः_Restricted( xTimerQueue, ( x_अग्र_Expire_Time - xTimeNow ), x_List_Was_रिक्तानि );

                    यदि( prvEXIT_CRITICAL_OR_RESUME_ALL( &x_Timer_तालकम् ) == pdFALSE )
                    {
                        /* Yield to wait for either a command to arrive, or the
                         * block time to expire.  If a command arrived between the
                         * critical section being exited and this yield then the yield
                         * will not cause the task to block. */
                        पत्तनम्_YIELD_WITHIN_API();
                    }
                    अन्यथा
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
            }
            अन्यथा
            {
                ( शून्य ) prvEXIT_CRITICAL_OR_RESUME_ALL( &x_Timer_तालकम् );
            }
        }
    }
/*-----------------------------------------------------------*/

    स्थिर TickType_t prv_Get_अग्र_Expire_Time( आधारः_Type_t * ध्रुव px_List_Was_रिक्तानि )
    {
        TickType_t x_अग्र_Expire_Time;

        /* Timers are listed in expiry time order, with the head of the list
         * referencing the task that will expire first.  Obtain the time at which
         * the timer with the nearest expiry time will expire.  If there are no
         * active timers then just set the next expire time to 0.  That will cause
         * this task to unblock when the tick count overflows, at which point the
         * timer lists will be switched and the next expiry time can be
         * re-assessed.  */
        *px_List_Was_रिक्तानि = list_LIST_IS_रिक्तानि( pxCurrentTimerList );

        यदि( *px_List_Was_रिक्तानि == pdFALSE )
        {
            x_अग्र_Expire_Time = list_GET_ITEM_मानम्_सम्बन्धि_शीर्ष_प्रविष्टिः( pxCurrentTimerList );
        }
        अन्यथा
        {
            /* Ensure the task unblocks when the tick count rolls over. */
            x_अग्र_Expire_Time = ( TickType_t ) 0U;
        }

        प्रत्यागम x_अग्र_Expire_Time;
    }
/*-----------------------------------------------------------*/

    स्थिर TickType_t prvSampleTimeNow( आधारः_Type_t * ध्रुव pxTimerListsWereSwitched )
    {
        TickType_t xTimeNow;
        PRIVILEGED_दत्त स्थिर TickType_t x_चरमः_Time = ( TickType_t ) 0U; /*lint !e956 Variable is only accessible to one task. */

        xTimeNow = x_Task_Get_Tick_गणनम्();

        यदि( xTimeNow < x_चरमः_Time )
        {
            prvSwitchTimerLists();
            *pxTimerListsWereSwitched = pdTRUE;
        }
        अन्यथा
        {
            *pxTimerListsWereSwitched = pdFALSE;
        }

        x_चरमः_Time = xTimeNow;

        प्रत्यागम xTimeNow;
    }
/*-----------------------------------------------------------*/

    स्थिर आधारः_Type_t prv_निवेशय_Timer_अन्तर्_सक्रिय_List( Timer_t * ध्रुव pxTimer,
                                                  ध्रुव TickType_t x_अग्र_Expiry_Time,
                                                  ध्रुव TickType_t xTimeNow,
                                                  ध्रुव TickType_t xCommandTime )
    {
        आधारः_Type_t xProcessTimerNow = pdFALSE;

        list_समुच्चयः_LIST_ITEM_मानम्( &( pxTimer->xTimerListItem ), x_अग्र_Expiry_Time );
        list_समुच्चयः_LIST_ITEM_OWNER( &( pxTimer->xTimerListItem ), pxTimer );

        यदि( x_अग्र_Expiry_Time <= xTimeNow )
        {
            /* Has the expiry time elapsed between the command to start/reset a
             * timer was issued, and the time the command was processed? */
            यदि( ( ( TickType_t ) ( xTimeNow - xCommandTime ) ) >= pxTimer->x_Timer_Period_अन्तर्_Ticks ) /*lint !e961 MISRA exception as the casts are only redundant for some ports. */
            {
                /* The time between a command being issued and the command being
                 * processed actually exceeds the timers period.  */
                xProcessTimerNow = pdTRUE;
            }
            अन्यथा
            {
                v_List_निवेशय( px_अतिप्रवाहः_Timer_List, &( pxTimer->xTimerListItem ) );
            }
        }
        अन्यथा
        {
            यदि( ( xTimeNow < xCommandTime ) && ( x_अग्र_Expiry_Time >= xCommandTime ) )
            {
                /* If, since the command was issued, the tick count has overflowed
                 * but the expiry time has not, then the timer must have already passed
                 * its expiry time and should be processed immediately. */
                xProcessTimerNow = pdTRUE;
            }
            अन्यथा
            {
                v_List_निवेशय( pxCurrentTimerList, &( pxTimer->xTimerListItem ) );
            }
        }

        प्रत्यागम xProcessTimerNow;
    }
/*-----------------------------------------------------------*/

    स्थिर शून्य prvProcessReceivedCommands( शून्य )
    {
        Daemon_Task_सन्देशः_t x_सन्देशः;
        Timer_t * pxTimer;
        आधारः_Type_t xTimerListsWereSwitched;
        TickType_t xTimeNow;

        यावत्( xQueueReceive( xTimerQueue, &x_सन्देशः, tmrNO_DELAY ) != pdFAIL ) /*lint !e603 xMessage does not have to be initialised as it is passed out, not in, and it is not used unless xQueueReceive() returns pdTRUE. */
        {
            #यदि ( INCLUDE_xTimerPendFunctionCall == 1 )
            {
                /* Negative commands are pended function calls rather than timer
                 * commands. */
                यदि( x_सन्देशः.x_सन्देशः_ID < ( आधारः_Type_t ) 0 )
                {
                    ध्रुव CallbackParameters_t * ध्रुव pxCallback = &( x_सन्देशः.u.xCallbackParameters );

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
            #यदि_अन्तः /* INCLUDE_xTimerPendFunctionCall */

            /* Commands that are positive are timer commands rather than pended
             * function calls. */
            यदि( x_सन्देशः.x_सन्देशः_ID >= ( आधारः_Type_t ) 0 )
            {
                /* The messages uses the xTimerParameters member to work on a
                 * software timer. */
                pxTimer = x_सन्देशः.u.xTimerParameters.pxTimer;

                यदि( listIS_CONTAINED_WITHIN( रिक्त, &( pxTimer->xTimerListItem ) ) == pdFALSE ) /*lint !e961. The cast is only redundant when NULL is passed into the macro. */
                {
                    /* The timer is in a list, remove it. */
                    ( शून्य ) uxListRemove( &( pxTimer->xTimerListItem ) );
                }
                अन्यथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                traceTIMER_COMMAND_RECEIVED( pxTimer, x_सन्देशः.x_सन्देशः_ID, x_सन्देशः.u.xTimerParameters.x_सन्देशः_मानम् );

                /* In this case the xTimerListsWereSwitched parameter is not used, but
                 *  it must be present in the function call.  prvSampleTimeNow() must be
                 *  called after the message is received from xTimerQueue so there is no
                 *  possibility of a higher priority task adding a message to the message
                 *  queue with a time that is ahead of the timer daemon task (because it
                 *  pre-empted the timer daemon task after the xTimeNow value was set). */
                xTimeNow = prvSampleTimeNow( &xTimerListsWereSwitched );

                विकल्प( x_सन्देशः.x_सन्देशः_ID )
                {
                    पक्ष tmrCOMMAND_START:
                    पक्ष tmrCOMMAND_START_FROM_ISR:
                    पक्ष tmrCOMMAND_RESET:
                    पक्ष tmrCOMMAND_RESET_FROM_ISR:
                        /* Start or restart a timer. */
                        pxTimer->uc_स्थितिः |= tmr_स्थितिः_IS_सक्रिय;

                        यदि( prv_निवेशय_Timer_अन्तर्_सक्रिय_List( pxTimer, x_सन्देशः.u.xTimerParameters.x_सन्देशः_मानम् + pxTimer->x_Timer_Period_अन्तर्_Ticks, xTimeNow, x_सन्देशः.u.xTimerParameters.x_सन्देशः_मानम् ) != pdFALSE )
                        {
                            /* The timer expired before it was added to the active
                             * timer list.  Process it now. */
                            यदि( ( pxTimer->uc_स्थितिः & tmr_स्थितिः_IS_AUTORELOAD ) != 0 )
                            {
                                prvReloadTimer( pxTimer, x_सन्देशः.u.xTimerParameters.x_सन्देशः_मानम् + pxTimer->x_Timer_Period_अन्तर्_Ticks, xTimeNow );
                            }
                            अन्यथा
                            {
                                pxTimer->uc_स्थितिः &= ( ( uint8_t ) ~tmr_स्थितिः_IS_सक्रिय );
                            }

                            /* Call the timer callback. */
                            traceTIMER_EXPIRED( pxTimer );
                            pxTimer->pxCallbackFunction( ( TimerHandle_t ) pxTimer );
                        }
                        अन्यथा
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }

                        भङ्ग;

                    पक्ष tmrCOMMAND_STOP:
                    पक्ष tmrCOMMAND_STOP_FROM_ISR:
                        /* The timer has already been removed from the active list. */
                        pxTimer->uc_स्थितिः &= ( ( uint8_t ) ~tmr_स्थितिः_IS_सक्रिय );
                        भङ्ग;

                    पक्ष tmrCOMMAND_CHANGE_PERIOD:
                    पक्ष tmrCOMMAND_CHANGE_PERIOD_FROM_ISR:
                        pxTimer->uc_स्थितिः |= tmr_स्थितिः_IS_सक्रिय;
                        pxTimer->x_Timer_Period_अन्तर्_Ticks = x_सन्देशः.u.xTimerParameters.x_सन्देशः_मानम्;
                        configASSERT( ( pxTimer->x_Timer_Period_अन्तर्_Ticks > 0 ) );

                        /* The new period does not really have a reference, and can
                         * be longer or shorter than the old one.  The command time is
                         * therefore set to the current time, and as the period cannot
                         * be zero the next expiry time can only be in the future,
                         * meaning (unlike for the xTimerStart() case above) there is
                         * no fail case that needs to be handled here. */
                        ( शून्य ) prv_निवेशय_Timer_अन्तर्_सक्रिय_List( pxTimer, ( xTimeNow + pxTimer->x_Timer_Period_अन्तर्_Ticks ), xTimeNow, xTimeNow );
                        भङ्ग;

                    पक्ष tmrCOMMAND_DELETE:
                        #यदि ( config_SUPPORT_DYNAMIC_आवण्टनम् == 1 )
                        {
                            /* The timer has already been removed from the active list,
                             * just free up the memory if the memory was dynamically
                             * allocated. */
                            यदि( ( pxTimer->uc_स्थितिः & tmr_स्थितिः_IS_STATICALLY_ALLOCATED ) == ( uint8_t ) 0 )
                            {
                                v_पत्तनम्_Free( pxTimer );
                            }
                            अन्यथा
                            {
                                pxTimer->uc_स्थितिः &= ( ( uint8_t ) ~tmr_स्थितिः_IS_सक्रिय );
                            }
                        }
                        #अन्यथा /* if ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) */
                        {
                            /* If dynamic allocation is not enabled, the memory
                             * could not have been dynamically allocated. So there is
                             * no need to free the memory - just mark the timer as
                             * "not active". */
                            pxTimer->uc_स्थितिः &= ( ( uint8_t ) ~tmr_स्थितिः_IS_सक्रिय );
                        }
                        #यदि_अन्तः /* configSUPPORT_DYNAMIC_ALLOCATION */
                        भङ्ग;

                    स्वभाव:
                        /* Don't expect to get here. */
                        भङ्ग;
                }
            }
        }
    }
/*-----------------------------------------------------------*/

    स्थिर शून्य prvSwitchTimerLists( शून्य )
    {
        TickType_t x_अग्र_Expire_Time;
        List_t * px_अस्थायी;

        /* The tick count has overflowed.  The timer lists must be switched.
         * If there are any timers still referenced from the current timer list
         * then they must have expired and should be processed before the lists
         * are switched. */
        यावत्( list_LIST_IS_रिक्तानि( pxCurrentTimerList ) == pdFALSE )
        {
            x_अग्र_Expire_Time = list_GET_ITEM_मानम्_सम्बन्धि_शीर्ष_प्रविष्टिः( pxCurrentTimerList );

            /* Process the expired timer.  For auto-reload timers, be careful to
             * process only expirations that occur on the current list.  Further
             * expirations must wait until after the lists are switched. */
            prvProcessExpiredTimer( x_अग्र_Expire_Time, tmr_MAX_TIME_BEFORE_अतिप्रवाहः );
        }

        px_अस्थायी = pxCurrentTimerList;
        pxCurrentTimerList = px_अतिप्रवाहः_Timer_List;
        px_अतिप्रवाहः_Timer_List = px_अस्थायी;
    }
/*-----------------------------------------------------------*/

    स्थिर शून्य prvCheckForValidListAndQueue( शून्य )
    {
        /* Check that the list from which active timers are referenced, and the
         * queue used to communicate with the timer service, have been
         * initialised. */
        taskENTER_CRITICAL( &x_Timer_तालकम् );
        {
            यदि( xTimerQueue == रिक्त )
            {
                vListInitialise( &x_सक्रिय_Timer_List1 );
                vListInitialise( &x_सक्रिय_Timer_List2 );
                pxCurrentTimerList = &x_सक्रिय_Timer_List1;
                px_अतिप्रवाहः_Timer_List = &x_सक्रिय_Timer_List2;

                #यदि ( config_SUPPORT_STATIC_आवण्टनम् == 1 )
                {
                    /* The timer queue is allocated statically in case
                     * configSUPPORT_DYNAMIC_ALLOCATION is 0. */
                    PRIVILEGED_दत्त स्थिर StaticQueue_t xStaticTimerQueue;                                                                          /*lint !e956 Ok to declare in this manner to prevent additional conditional compilation guards in other locations. */
                    PRIVILEGED_दत्त स्थिर uint8_t ucStaticTimerQueueStorage[ ( size_t ) config_TIMER_QUEUE_दीर्घता * परिमाण( Daemon_Task_सन्देशः_t ) ]; /*lint !e956 Ok to declare in this manner to prevent additional conditional compilation guards in other locations. */

                    xTimerQueue = xQueueCreateStatic( ( UBaseType_t ) config_TIMER_QUEUE_दीर्घता, ( UBaseType_t ) परिमाण( Daemon_Task_सन्देशः_t ), &( ucStaticTimerQueueStorage[ 0 ] ), &xStaticTimerQueue );
                }
                #अन्यथा
                {
                    xTimerQueue = xQueueCreate( ( UBaseType_t ) config_TIMER_QUEUE_दीर्घता, परिमाण( Daemon_Task_सन्देशः_t ) );
                }
                #यदि_अन्तः /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */

                #यदि ( config_QUEUE_REGISTRY_आकार > 0 )
                {
                    यदि( xTimerQueue != रिक्त )
                    {
                        v_Queue_सङ्कलय_गन्तव्यम्_Registry( xTimerQueue, "TmrQ" );
                    }
                    अन्यथा
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #यदि_अन्तः /* configQUEUE_REGISTRY_SIZE */
            }
            अन्यथा
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        taskEXIT_CRITICAL( &x_Timer_तालकम् );
    }
/*-----------------------------------------------------------*/

    आधारः_Type_t x_Timer_Is_Timer_सक्रिय( TimerHandle_t xTimer )
    {
        आधारः_Type_t xReturn;
        Timer_t * pxTimer = xTimer;

        configASSERT( xTimer );

        /* Is the timer in the list of active timers? */
        taskENTER_CRITICAL( &x_Timer_तालकम् );
        {
            यदि( ( pxTimer->uc_स्थितिः & tmr_स्थितिः_IS_सक्रिय ) == 0 )
            {
                xReturn = pdFALSE;
            }
            अन्यथा
            {
                xReturn = pdTRUE;
            }
        }
        taskEXIT_CRITICAL( &x_Timer_तालकम् );

        प्रत्यागम xReturn;
    } /*lint !e818 Can't be pointer to const due to the typedef. */
/*-----------------------------------------------------------*/

    शून्य * pvTimerGetTimerID( ध्रुव TimerHandle_t xTimer )
    {
        Timer_t * ध्रुव pxTimer = xTimer;
        शून्य * pvReturn;

        configASSERT( xTimer );

        taskENTER_CRITICAL( &x_Timer_तालकम् );
        {
            pvReturn = pxTimer->pvTimerID;
        }
        taskEXIT_CRITICAL( &x_Timer_तालकम् );

        प्रत्यागम pvReturn;
    }
/*-----------------------------------------------------------*/

    शून्य v_Timer_समुच्चयः_Timer_ID( TimerHandle_t xTimer,
                           शून्य * pvNewID )
    {
        Timer_t * ध्रुव pxTimer = xTimer;

        configASSERT( xTimer );

        taskENTER_CRITICAL( &x_Timer_तालकम् );
        {
            pxTimer->pvTimerID = pvNewID;
        }
        taskEXIT_CRITICAL( &x_Timer_तालकम् );
    }
/*-----------------------------------------------------------*/

    #यदि ( INCLUDE_xTimerPendFunctionCall == 1 )

        आधारः_Type_t xTimerPendFunctionCallFromISR( PendedFunction_t x_Function_गन्तव्यम्_Pend,
                                                  शून्य * pvParameter1,
                                                  uint32_t ulParameter2,
                                                  आधारः_Type_t * px_Higher_प्राथमिकता_Task_Woken )
        {
            Daemon_Task_सन्देशः_t x_सन्देशः;
            आधारः_Type_t xReturn;

            /* Complete the message with the function parameters and post it to the
             * daemon task. */
            x_सन्देशः.x_सन्देशः_ID = tmrCOMMAND_EXECUTE_CALLBACK_FROM_ISR;
            x_सन्देशः.u.xCallbackParameters.pxCallbackFunction = x_Function_गन्तव्यम्_Pend;
            x_सन्देशः.u.xCallbackParameters.pvParameter1 = pvParameter1;
            x_सन्देशः.u.xCallbackParameters.ulParameter2 = ulParameter2;

            xReturn = xQueueSendFromISR( xTimerQueue, &x_सन्देशः, px_Higher_प्राथमिकता_Task_Woken );

            tracePEND_FUNC_CALL_FROM_ISR( x_Function_गन्तव्यम्_Pend, pvParameter1, ulParameter2, xReturn );

            प्रत्यागम xReturn;
        }

    #यदि_अन्तः /* INCLUDE_xTimerPendFunctionCall */
/*-----------------------------------------------------------*/

    #यदि ( INCLUDE_xTimerPendFunctionCall == 1 )

        आधारः_Type_t xTimerPendFunctionCall( PendedFunction_t x_Function_गन्तव्यम्_Pend,
                                           शून्य * pvParameter1,
                                           uint32_t ulParameter2,
                                           TickType_t x_Ticks_गन्तव्यम्_Wait )
        {
            Daemon_Task_सन्देशः_t x_सन्देशः;
            आधारः_Type_t xReturn;

            /* This function can only be called after a timer has been created or
             * after the scheduler has been started because, until then, the timer
             * queue does not exist. */
            configASSERT( xTimerQueue );

            /* Complete the message with the function parameters and post it to the
             * daemon task. */
            x_सन्देशः.x_सन्देशः_ID = tmrCOMMAND_EXECUTE_CALLBACK;
            x_सन्देशः.u.xCallbackParameters.pxCallbackFunction = x_Function_गन्तव्यम्_Pend;
            x_सन्देशः.u.xCallbackParameters.pvParameter1 = pvParameter1;
            x_सन्देशः.u.xCallbackParameters.ulParameter2 = ulParameter2;

            xReturn = x_Queue_Send_गन्तव्यम्_पृष्ठभागः( xTimerQueue, &x_सन्देशः, x_Ticks_गन्तव्यम्_Wait );

            tracePEND_FUNC_CALL( x_Function_गन्तव्यम्_Pend, pvParameter1, ulParameter2, xReturn );

            प्रत्यागम xReturn;
        }

    #यदि_अन्तः /* INCLUDE_xTimerPendFunctionCall */
/*-----------------------------------------------------------*/

    #यदि ( configUSE_TRACE_FACILITY == 1 )

        UBaseType_t ux_Timer_Get_Timer_सङ्ख्या( TimerHandle_t xTimer )
        {
            प्रत्यागम ( ( Timer_t * ) xTimer )->ux_Timer_सङ्ख्या;
        }

    #यदि_अन्तः /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

    #यदि ( configUSE_TRACE_FACILITY == 1 )

        शून्य v_Timer_समुच्चयः_Timer_सङ्ख्या( TimerHandle_t xTimer,
                                   UBaseType_t ux_Timer_सङ्ख्या )
        {
            ( ( Timer_t * ) xTimer )->ux_Timer_सङ्ख्या = ux_Timer_सङ्ख्या;
        }

    #यदि_अन्तः /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

/* This entire source file will be skipped if the application is not configured
 * to include software timer functionality.  If you want to include software timer
 * functionality then ensure configUSE_TIMERS is set to 1 in FreeRTOSConfig.h. */
#यदि_अन्तः /* configUSE_TIMERS == 1 */
