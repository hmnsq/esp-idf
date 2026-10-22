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
#అన్తర్భావ <stdlib.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#నిర్వచించు MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#అన్తర్భావ "FreeRTOS.h"
#అన్తర్భావ "task.h"
#అన్తర్భావ "queue.h"
#అన్తర్భావ "timers.h"
/* Include private IDF API additions for critical thread safety macros */
#అన్తర్భావ "esp_private/freertos_idf_additions_priv.h"

#ఒకవేళ ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 0 )
    #దోషం configUSE_TIMERS must be సమితి గమ్యం 1 గమ్యం make the xTimerPendFunctionCall() function అందుబాటు.
#అయితే_ముగింపు

/* Lint e9021, e961 and e750 are suppressed as a MISRA exception justified
 * because the MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#నిర్వచనం_రద్దు MPU_WRAPPERS_INCLUDED_FROM_API_FILE /*lint !e9021 !e961 !e750. */


/* This entire source file will be skipped if the application is not configured
 * to include software timer functionality.  This #if is closed at the very bottom
 * of this file.  If you want to include software timer functionality then ensure
 * configUSE_TIMERS is set to 1 in FreeRTOSConfig.h. */
#ఒకవేళ ( configUSE_TIMERS == 1 )

/* Misc definitions. */
    #నిర్వచించు tmrNO_DELAY                    ( ( TickType_t ) 0U )
    #నిర్వచించు tmr_MAX_సమయం_BEFORE_అధికప్రవాహం    ( ( TickType_t ) -1 )

/* The name assigned to the timer service task. This can be overridden by
 * defining configTIMER_SERVICE_TASK_NAME in FreeRTOSConfig.h. */
    #నిర్వచితం_కాకపోతే configTIMER_SERVICE_TASK_NAME
        #నిర్వచించు configTIMER_SERVICE_TASK_NAME    "Tmr Svc"
    #అయితే_ముగింపు

    #ఒకవేళ ( config_సంఖ్య_యొక్క_CORES > 1 )

/* The core affinity assigned to the timer service task on SMP systems.
 * This can be overridden by defining configTIMER_SERVICE_TASK_CORE_AFFINITY in FreeRTOSConfig.h. */
        #నిర్వచితం_కాకపోతే configTIMER_SERVICE_TASK_CORE_AFFINITY
            #నిర్వచించు configTIMER_SERVICE_TASK_CORE_AFFINITY    tskNO_AFFINITY
        #అయితే_ముగింపు

        #ఒకవేళ ( !( ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x0 ) || ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x1 ) || ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x7FFFFFFF /* tskNO_AFFINITY */ ) ) )
            #దోషం Invalid Timer task affinity. Timer task core ID can only be 0x0, 0x1 or 0x7FFFFFFF (tskNO_AFFINITY).
        #అయితే_ముగింపు /* #if ( !( ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x0 ) || ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x01 ) || ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x7FFFFFFF ) ) ) */
    #అయితే_ముగింపు /* #if ( configNUMBER_OF_CORES > 1 ) */

/* Bit definitions used in the ucStatus member of a timer structure. */
    #నిర్వచించు tmr_స్థితి_IS_చురుకైన                  ( ( uint8_t ) 0x01 )
    #నిర్వచించు tmr_స్థితి_IS_STATICALLY_ALLOCATED    ( ( uint8_t ) 0x02 )
    #నిర్వచించు tmr_స్థితి_IS_AUTORELOAD              ( ( uint8_t ) 0x04 )

/* The definition of the timers themselves. */
    రకనిర్వచనం నిర్మాణం tmrTimerControl                  /* The old naming convention is used to prevent breaking kernel aware debuggers. */
    {
        స్థిరరాశి అక్షరం * pcTimerName;                   /*<< Text name.  This is not used by the kernel, it is included simply to make debugging easier. */ /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
        List_అంశం_t x_Timer_List_అంశం;                  /*<< Standard linked list item as used by all kernel features for event management. */
        TickType_t x_Timer_Period_లోపల_Ticks;             /*<< How quickly and often the timer expires. */
        శూన్య * pvTimerID;                           /*<< An ID to identify the timer.  This allows the timer to be identified when the same callback is used for multiple timers. */
        TimerCallbackFunction_t pxCallbackFunction; /*<< The function that will be called when the timer expires. */
        #ఒకవేళ ( configUSE_TRACE_FACILITY == 1 )
            UBaseType_t ux_Timer_సంఖ్య;              /*<< An ID assigned by trace tools such as FreeRTOS+Trace */
        #అయితే_ముగింపు
        uint8_t uc_స్థితి;                           /*<< Holds bits to say if the timer was statically allocated or not, and if it is active or not. */
    } xTIMER;

/* The old xTIMER name is maintained above then typedefed to the new Timer_t
 * name below to enable the use of older kernel aware debuggers. */
    రకనిర్వచనం xTIMER Timer_t;

/* The definition of messages that can be sent and received on the timer queue.
 * Two types of message can be queued - messages that manipulate a software timer,
 * and messages that request the execution of a non-timer related callback.  The
 * two message types are defined in two separate structures, xTimerParametersType
 * and xCallbackParametersType respectively. */
    రకనిర్వచనం నిర్మాణం tmrTimerParameters
    {
        TickType_t x_సందేశం_విలువ; /*<< An optional value used by a subset of commands, for example, when changing the period of a timer. */
        Timer_t * pxTimer;        /*<< The timer to which the command will be applied. */
    } TimerParameter_t;


    రకనిర్వచనం నిర్మాణం tmrCallbackParameters
    {
        PendedFunction_t pxCallbackFunction; /* << The callback function to execute. */
        శూన్య * pvParameter1;                 /* << The value that will be used as the callback functions first parameter. */
        uint32_t ulParameter2;               /* << The value that will be used as the callback functions second parameter. */
    } CallbackParameters_t;

/* The structure that contains the two message types, along with an identifier
 * that is used to determine which message type is valid. */
    రకనిర్వచనం నిర్మాణం tmr_Timer_వరుస_సందేశం
    {
        పునాది_Type_t x_సందేశం_ID; /*<< The command being sent to the timer service task. */
        సంఘం
        {
            TimerParameter_t xTimerParameters;

            /* Don't include xCallbackParameters if it is not going to be used as
             * it makes the structure (and therefore the timer queue) larger. */
            #ఒకవేళ ( INCLUDE_xTimerPendFunctionCall == 1 )
                CallbackParameters_t xCallbackParameters;
            #అయితే_ముగింపు /* INCLUDE_xTimerPendFunctionCall */
        } u;
    } Daemon_Task_సందేశం_t;

/*lint -save -e956 A manual analysis and inspection has been used to determine
 * which static variables must be declared volatile. */

/* The list in which active timers are stored.  Timers are referenced in expire
 * time order, with the nearest expiry time at the front of the list.  Only the
 * timer service task is allowed to access these lists.
 * xActiveTimerList1 and xActiveTimerList2 could be at function scope but that
 * breaks some kernel aware debuggers, and debuggers that reply on removing the
 * static qualifier. */
    PRIVILEGED_దత్తాంశం స్థిర List_t x_చురుకైన_Timer_List1;
    PRIVILEGED_దత్తాంశం స్థిర List_t x_చురుకైన_Timer_List2;
    PRIVILEGED_దత్తాంశం స్థిర List_t * pxCurrentTimerList;
    PRIVILEGED_దత్తాంశం స్థిర List_t * px_అధికప్రవాహం_Timer_List;

/* A queue that is used to send commands to the timer service task. */
    PRIVILEGED_దత్తాంశం స్థిర వరుస_Handle_t x_Timer_వరుస = ఖాళీ;
    PRIVILEGED_దత్తాంశం స్థిర TaskHandle_t xTimerTaskHandle = ఖాళీ;

/* Spinlock required in SMP when accessing the timers. For now we use a single lock
 * Todo: Each timer could possible have its own lock for increased granularity. */
    PRIVILEGED_దత్తాంశం స్థిర పోర్టు_MUX_TYPE x_Timer_తాళం = పోర్టు_MUX_INITIALIZER_UNLOCKED;

/*lint -restore */

/*-----------------------------------------------------------*/

/*
 * Initialise the infrastructure used by the timer service task if it has not
 * been initialised already.
 */
    స్థిర శూన్య prv_Check_For_Valid_List_And_వరుస( శూన్య ) PRIVILEGED_FUNCTION;

/*
 * The timer service task (daemon).  Timer functionality is controlled by this
 * task.  Other tasks communicate with the timer service task using the
 * xTimerQueue queue.
 */
    స్థిర పోర్టు_TASK_FUNCTION_PROTO( prvTimerTask, pvParameters ) PRIVILEGED_FUNCTION;

/*
 * Called by the timer service task to interpret and process a command it
 * received on the timer queue.
 */
    స్థిర శూన్య prvProcessReceivedCommands( శూన్య ) PRIVILEGED_FUNCTION;

/*
 * Insert the timer into either xActiveTimerList1, or xActiveTimerList2,
 * depending on if the expire time causes a timer counter overflow.
 */
    స్థిర పునాది_Type_t prv_చొప్పించు_Timer_లోపల_చురుకైన_List( Timer_t * స్థిరరాశి pxTimer,
                                                  స్థిరరాశి TickType_t x_తదుపరి_Expiry_సమయం,
                                                  స్థిరరాశి TickType_t x_సమయం_Now,
                                                  స్థిరరాశి TickType_t x_Command_సమయం ) PRIVILEGED_FUNCTION;

/*
 * Reload the specified auto-reload timer.  If the reloading is backlogged,
 * clear the backlog, calling the callback for each additional reload.  When
 * this function returns, the next expiry time is after xTimeNow.
 */
    స్థిర శూన్య prvReloadTimer( Timer_t * స్థిరరాశి pxTimer,
                                TickType_t x_Expired_సమయం,
                                స్థిరరాశి TickType_t x_సమయం_Now ) PRIVILEGED_FUNCTION;

/*
 * An active timer has reached its expire time.  Reload the timer if it is an
 * auto-reload timer, then call its callback.
 */
    స్థిర శూన్య prvProcessExpiredTimer( స్థిరరాశి TickType_t x_తదుపరి_Expire_సమయం,
                                        స్థిరరాశి TickType_t x_సమయం_Now ) PRIVILEGED_FUNCTION;

/*
 * The tick count has overflowed.  Switch the timer lists after ensuring the
 * current timer list does not still reference some timers.
 */
    స్థిర శూన్య prvSwitchTimerLists( శూన్య ) PRIVILEGED_FUNCTION;

/*
 * Obtain the current tick count, setting *pxTimerListsWereSwitched to pdTRUE
 * if a tick count overflow occurred since prvSampleTimeNow() was last called.
 */
    స్థిర TickType_t prv_Sample_సమయం_Now( పునాది_Type_t * స్థిరరాశి pxTimerListsWereSwitched ) PRIVILEGED_FUNCTION;

/*
 * If the timer list contains any active timers then return the expire time of
 * the timer that will expire first and set *pxListWasEmpty to false.  If the
 * timer list does not contain any timers then return 0 and set *pxListWasEmpty
 * to pdTRUE.
 */
    స్థిర TickType_t prv_Get_తదుపరి_Expire_సమయం( పునాది_Type_t * స్థిరరాశి px_List_Was_ఖాళీలు ) PRIVILEGED_FUNCTION;

/*
 * If a timer has expired, process it.  Otherwise, block the timer service task
 * until either a timer does expire or a command is received.
 */
    స్థిర శూన్య prv_Process_Timer_Or_ఖండం_Task( స్థిరరాశి TickType_t x_తదుపరి_Expire_సమయం,
                                            పునాది_Type_t x_List_Was_ఖాళీలు ) PRIVILEGED_FUNCTION;

/*
 * Called after a Timer_t structure has been allocated either statically or
 * dynamically to fill in the structure's members.
 */
    స్థిర శూన్య prvInitialiseNewTimer( స్థిరరాశి అక్షరం * స్థిరరాశి pcTimerName, /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                       స్థిరరాశి TickType_t x_Timer_Period_లోపల_Ticks,
                                       స్థిరరాశి పునాది_Type_t xAutoReload,
                                       శూన్య * స్థిరరాశి pvTimerID,
                                       TimerCallbackFunction_t pxCallbackFunction,
                                       Timer_t * pxNewTimer ) PRIVILEGED_FUNCTION;
/*-----------------------------------------------------------*/

    పునాది_Type_t xTimerCreateTimerTask( శూన్య )
    {
        పునాది_Type_t xReturn = pdFAIL;

        /* This function is called when the scheduler is started if
         * configUSE_TIMERS is set to 1.  Check that the infrastructure used by the
         * timer service task has been created/initialised.  If timers have already
         * been created then the initialisation will already have been performed. */
        prv_Check_For_Valid_List_And_వరుస();

        ఒకవేళ( x_Timer_వరుస != ఖాళీ )
        {
            /* The following code block to set the timer task core affinity has been picked up from the FreeRTOS-Kernel
             * upstream repository. Commit# 631ae9e6e4828cf5c7d58bb7ac78a64fc309bcb7.
             * Necessary changes have been made for IDF-FreeRTOS compatibility. */
            #ఒకవేళ ( config_సంఖ్య_యొక్క_CORES > 1 )
            {
                #ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )
                {
                    StaticTask_t * pxTimerTaskTCBBuffer = ఖాళీ;
                    దొంతర_Type_t * px_Timer_Task_దొంతర_బఫరు = ఖాళీ;
                    uint32_t ul_Timer_Task_దొంతర_కొలత;

                    v_Application_Get_Timer_Task_జ్ఞాపకం( &pxTimerTaskTCBBuffer, &px_Timer_Task_దొంతర_బఫరు, &ul_Timer_Task_దొంతర_కొలత );
                    xTimerTaskHandle = x_Task_Create_Static_Pinned_గమ్యం_Core( prvTimerTask,
                                                                      configTIMER_SERVICE_TASK_NAME,
                                                                      ul_Timer_Task_దొంతర_కొలత,
                                                                      ఖాళీ,
                                                                      ( ( UBaseType_t ) config_TIMER_TASK_ప్రాధాన్యత ) | పోర్టు_PRIVILEGE_బిట్టు,
                                                                      px_Timer_Task_దొంతర_బఫరు,
                                                                      pxTimerTaskTCBBuffer,
                                                                      configTIMER_SERVICE_TASK_CORE_AFFINITY );

                    ఒకవేళ( xTimerTaskHandle != ఖాళీ )
                    {
                        xReturn = pdPASS;
                    }
                }
                #లేకపోతే /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
                {
                    xReturn = x_Task_Create_Pinned_గమ్యం_Core( prvTimerTask,
                                                       configTIMER_SERVICE_TASK_NAME,
                                                       config_TIMER_TASK_దొంతర_లోతు,
                                                       ఖాళీ,
                                                       ( ( UBaseType_t ) config_TIMER_TASK_ప్రాధాన్యత ) | పోర్టు_PRIVILEGE_బిట్టు,
                                                       &xTimerTaskHandle,
                                                       configTIMER_SERVICE_TASK_CORE_AFFINITY );
                }
                #అయితే_ముగింపు /* configSUPPORT_STATIC_ALLOCATION */
            }
            #లేకపోతే /* #if ( configNUMBER_OF_CORES > 1 ) */
            {
                #ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )
                {
                    StaticTask_t * pxTimerTaskTCBBuffer = ఖాళీ;
                    దొంతర_Type_t * px_Timer_Task_దొంతర_బఫరు = ఖాళీ;
                    uint32_t ul_Timer_Task_దొంతర_కొలత;

                    v_Application_Get_Timer_Task_జ్ఞాపకం( &pxTimerTaskTCBBuffer, &px_Timer_Task_దొంతర_బఫరు, &ul_Timer_Task_దొంతర_కొలత );
                    xTimerTaskHandle = xTaskCreateStatic( prvTimerTask,
                                                          configTIMER_SERVICE_TASK_NAME,
                                                          ul_Timer_Task_దొంతర_కొలత,
                                                          ఖాళీ,
                                                          ( ( UBaseType_t ) config_TIMER_TASK_ప్రాధాన్యత ) | పోర్టు_PRIVILEGE_బిట్టు,
                                                          px_Timer_Task_దొంతర_బఫరు,
                                                          pxTimerTaskTCBBuffer );

                    ఒకవేళ( xTimerTaskHandle != ఖాళీ )
                    {
                        xReturn = pdPASS;
                    }
                }
                #లేకపోతే /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
                {
                    xReturn = xTaskCreate( prvTimerTask,
                                           configTIMER_SERVICE_TASK_NAME,
                                           config_TIMER_TASK_దొంతర_లోతు,
                                           ఖాళీ,
                                           ( ( UBaseType_t ) config_TIMER_TASK_ప్రాధాన్యత ) | పోర్టు_PRIVILEGE_బిట్టు,
                                           &xTimerTaskHandle );
                }
                #అయితే_ముగింపు /* configSUPPORT_STATIC_ALLOCATION */
            }
            #అయితే_ముగింపు /* #if ( configNUMBER_OF_CORES > 1 ) */
        }
        లేకపోతే
        {
            mtCOVERAGE_TEST_MARKER();
        }

        configASSERT( xReturn );
        తిరిగి xReturn;
    }
/*-----------------------------------------------------------*/

    #ఒకవేళ ( config_SUPPORT_DYNAMIC_కేటాయింపు == 1 )

        TimerHandle_t xTimerCreate( స్థిరరాశి అక్షరం * స్థిరరాశి pcTimerName, /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                    స్థిరరాశి TickType_t x_Timer_Period_లోపల_Ticks,
                                    స్థిరరాశి పునాది_Type_t xAutoReload,
                                    శూన్య * స్థిరరాశి pvTimerID,
                                    TimerCallbackFunction_t pxCallbackFunction )
        {
            Timer_t * pxNewTimer;

            pxNewTimer = ( Timer_t * ) pv_పోర్టు_Malloc( పరిమాణం( Timer_t ) ); /*lint !e9087 !e9079 All values returned by pvPortMalloc() have at least the alignment required by the MCU's stack, and the first member of Timer_t is always a pointer to the timer's mame. */

            ఒకవేళ( pxNewTimer != ఖాళీ )
            {
                /* Status is thus far zero as the timer is not created statically
                 * and has not been started.  The auto-reload bit may get set in
                 * prvInitialiseNewTimer. */
                pxNewTimer->uc_స్థితి = 0x00;
                prvInitialiseNewTimer( pcTimerName, x_Timer_Period_లోపల_Ticks, xAutoReload, pvTimerID, pxCallbackFunction, pxNewTimer );
            }

            తిరిగి pxNewTimer;
        }

    #అయితే_ముగింపు /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

    #ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )

        TimerHandle_t xTimerCreateStatic( స్థిరరాశి అక్షరం * స్థిరరాశి pcTimerName, /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                          స్థిరరాశి TickType_t x_Timer_Period_లోపల_Ticks,
                                          స్థిరరాశి పునాది_Type_t xAutoReload,
                                          శూన్య * స్థిరరాశి pvTimerID,
                                          TimerCallbackFunction_t pxCallbackFunction,
                                          StaticTimer_t * px_Timer_బఫరు )
        {
            Timer_t * pxNewTimer;

            #ఒకవేళ ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticTimer_t equals the size of the real timer
                 * structure. */
                అస్థిర size_t x_కొలత = పరిమాణం( StaticTimer_t );
                configASSERT( x_కొలత == పరిమాణం( Timer_t ) );
                ( శూన్య ) x_కొలత; /* Keeps lint quiet when configASSERT() is not defined. */
            }
            #అయితే_ముగింపు /* configASSERT_DEFINED */

            /* A pointer to a StaticTimer_t structure MUST be provided, use it. */
            configASSERT( px_Timer_బఫరు );
            pxNewTimer = ( Timer_t * ) px_Timer_బఫరు; /*lint !e740 !e9087 StaticTimer_t is a pointer to a Timer_t, so guaranteed to be aligned and sized correctly (checked by an assert()), so this is safe. */

            ఒకవేళ( pxNewTimer != ఖాళీ )
            {
                /* Timers can be created statically or dynamically so note this
                 * timer was created statically in case it is later deleted.  The
                 * auto-reload bit may get set in prvInitialiseNewTimer(). */
                pxNewTimer->uc_స్థితి = tmr_స్థితి_IS_STATICALLY_ALLOCATED;

                prvInitialiseNewTimer( pcTimerName, x_Timer_Period_లోపల_Ticks, xAutoReload, pvTimerID, pxCallbackFunction, pxNewTimer );
            }

            తిరిగి pxNewTimer;
        }

    #అయితే_ముగింపు /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

    స్థిర శూన్య prvInitialiseNewTimer( స్థిరరాశి అక్షరం * స్థిరరాశి pcTimerName, /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                       స్థిరరాశి TickType_t x_Timer_Period_లోపల_Ticks,
                                       స్థిరరాశి పునాది_Type_t xAutoReload,
                                       శూన్య * స్థిరరాశి pvTimerID,
                                       TimerCallbackFunction_t pxCallbackFunction,
                                       Timer_t * pxNewTimer )
    {
        /* 0 is not a valid value for xTimerPeriodInTicks. */
        configASSERT( ( x_Timer_Period_లోపల_Ticks > 0 ) );

        /* Ensure the infrastructure used by the timer service task has been
         * created/initialised. */
        prv_Check_For_Valid_List_And_వరుస();

        /* Initialise the timer structure members using the function
         * parameters. */
        pxNewTimer->pcTimerName = pcTimerName;
        pxNewTimer->x_Timer_Period_లోపల_Ticks = x_Timer_Period_లోపల_Ticks;
        pxNewTimer->pvTimerID = pvTimerID;
        pxNewTimer->pxCallbackFunction = pxCallbackFunction;
        v_List_Initialise_అంశం( &( pxNewTimer->x_Timer_List_అంశం ) );

        ఒకవేళ( xAutoReload != pdFALSE )
        {
            pxNewTimer->uc_స్థితి |= tmr_స్థితి_IS_AUTORELOAD;
        }

        traceTIMER_CREATE( pxNewTimer );
    }
/*-----------------------------------------------------------*/

    పునాది_Type_t xTimerGenericCommand( TimerHandle_t xTimer,
                                     స్థిరరాశి పునాది_Type_t xCommandID,
                                     స్థిరరాశి TickType_t x_Optional_విలువ,
                                     పునాది_Type_t * స్థిరరాశి px_Higher_ప్రాధాన్యత_Task_Woken,
                                     స్థిరరాశి TickType_t x_Ticks_గమ్యం_Wait )
    {
        పునాది_Type_t xReturn = pdFAIL;
        Daemon_Task_సందేశం_t x_సందేశం;

        configASSERT( xTimer );

        /* Send a message to the timer service task to perform a particular action
         * on a particular timer definition. */
        ఒకవేళ( x_Timer_వరుస != ఖాళీ )
        {
            /* Send a command to the timer service task to start the xTimer timer. */
            x_సందేశం.x_సందేశం_ID = xCommandID;
            x_సందేశం.u.xTimerParameters.x_సందేశం_విలువ = x_Optional_విలువ;
            x_సందేశం.u.xTimerParameters.pxTimer = xTimer;

            ఒకవేళ( xCommandID < tmr_మొదటి_FROM_ISR_COMMAND )
            {
                ఒకవేళ( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING )
                {
                    xReturn = x_వరుస_Send_గమ్యం_వెనుక( x_Timer_వరుస, &x_సందేశం, x_Ticks_గమ్యం_Wait );
                }
                లేకపోతే
                {
                    xReturn = x_వరుస_Send_గమ్యం_వెనుక( x_Timer_వరుస, &x_సందేశం, tmrNO_DELAY );
                }
            }
            లేకపోతే
            {
                xReturn = x_వరుస_Send_గమ్యం_వెనుక_From_ISR( x_Timer_వరుస, &x_సందేశం, px_Higher_ప్రాధాన్యత_Task_Woken );
            }

            traceTIMER_COMMAND_SEND( xTimer, xCommandID, x_Optional_విలువ, xReturn );
        }
        లేకపోతే
        {
            mtCOVERAGE_TEST_MARKER();
        }

        తిరిగి xReturn;
    }
/*-----------------------------------------------------------*/

    TaskHandle_t xTimerGetTimerDaemonTaskHandle( శూన్య )
    {
        /* If xTimerGetTimerDaemonTaskHandle() is called before the scheduler has been
         * started, then xTimerTaskHandle will be NULL. */
        configASSERT( ( xTimerTaskHandle != ఖాళీ ) );
        తిరిగి xTimerTaskHandle;
    }
/*-----------------------------------------------------------*/

    TickType_t xTimerGetPeriod( TimerHandle_t xTimer )
    {
        Timer_t * pxTimer = xTimer;

        configASSERT( xTimer );
        తిరిగి pxTimer->x_Timer_Period_లోపల_Ticks;
    }
/*-----------------------------------------------------------*/

    శూన్య v_Timer_సమితి_Reload_బహుళకం( TimerHandle_t xTimer,
                              స్థిరరాశి పునాది_Type_t xAutoReload )
    {
        Timer_t * pxTimer = xTimer;

        configASSERT( xTimer );
        taskENTER_CRITICAL( &x_Timer_తాళం );
        {
            ఒకవేళ( xAutoReload != pdFALSE )
            {
                pxTimer->uc_స్థితి |= tmr_స్థితి_IS_AUTORELOAD;
            }
            లేకపోతే
            {
                pxTimer->uc_స్థితి &= ( ( uint8_t ) ~tmr_స్థితి_IS_AUTORELOAD );
            }
        }
        taskEXIT_CRITICAL( &x_Timer_తాళం );
    }
/*-----------------------------------------------------------*/

    పునాది_Type_t x_Timer_Get_Reload_బహుళకం( TimerHandle_t xTimer )
    {
        Timer_t * pxTimer = xTimer;
        పునాది_Type_t xReturn;

        configASSERT( xTimer );
        taskENTER_CRITICAL( &x_Timer_తాళం );
        {
            ఒకవేళ( ( pxTimer->uc_స్థితి & tmr_స్థితి_IS_AUTORELOAD ) == 0 )
            {
                /* Not an auto-reload timer. */
                xReturn = pdFALSE;
            }
            లేకపోతే
            {
                /* Is an auto-reload timer. */
                xReturn = pdTRUE;
            }
        }
        taskEXIT_CRITICAL( &x_Timer_తాళం );

        తిరిగి xReturn;
    }

    UBaseType_t ux_Timer_Get_Reload_బహుళకం( TimerHandle_t xTimer )
    {
        తిరిగి ( UBaseType_t ) x_Timer_Get_Reload_బహుళకం( xTimer );
    }
/*-----------------------------------------------------------*/

    TickType_t x_Timer_Get_Expiry_సమయం( TimerHandle_t xTimer )
    {
        Timer_t * pxTimer = xTimer;
        TickType_t xReturn;

        configASSERT( xTimer );
        xReturn = list_GET_LIST_అంశం_విలువ( &( pxTimer->x_Timer_List_అంశం ) );
        తిరిగి xReturn;
    }
/*-----------------------------------------------------------*/

    #ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )
        పునాది_Type_t x_Timer_Get_Static_బఫరు( TimerHandle_t xTimer,
                                          StaticTimer_t ** ppx_Timer_బఫరు )
        {
            పునాది_Type_t xReturn;
            Timer_t * pxTimer = xTimer;

            configASSERT( ppx_Timer_బఫరు != ఖాళీ );

            ఒకవేళ( ( pxTimer->uc_స్థితి & tmr_స్థితి_IS_STATICALLY_ALLOCATED ) != 0 )
            {
                *ppx_Timer_బఫరు = ( StaticTimer_t * ) pxTimer;
                xReturn = pdTRUE;
            }
            లేకపోతే
            {
                xReturn = pdFALSE;
            }

            తిరిగి xReturn;
        }
    #అయితే_ముగింపు /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

    స్థిరరాశి అక్షరం * pcTimerGetName( TimerHandle_t xTimer ) /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
    {
        Timer_t * pxTimer = xTimer;

        configASSERT( xTimer );
        తిరిగి pxTimer->pcTimerName;
    }
/*-----------------------------------------------------------*/

    స్థిర శూన్య prvReloadTimer( Timer_t * స్థిరరాశి pxTimer,
                                TickType_t x_Expired_సమయం,
                                స్థిరరాశి TickType_t x_సమయం_Now )
    {
        /* Insert the timer into the appropriate list for the next expiry time.
         * If the next expiry time has already passed, advance the expiry time,
         * call the callback function, and try again. */
        అయితే( prv_చొప్పించు_Timer_లోపల_చురుకైన_List( pxTimer, ( x_Expired_సమయం + pxTimer->x_Timer_Period_లోపల_Ticks ), x_సమయం_Now, x_Expired_సమయం ) != pdFALSE )
        {
            /* Advance the expiry time. */
            x_Expired_సమయం += pxTimer->x_Timer_Period_లోపల_Ticks;

            /* Call the timer callback. */
            traceTIMER_EXPIRED( pxTimer );
            pxTimer->pxCallbackFunction( ( TimerHandle_t ) pxTimer );
        }
    }
/*-----------------------------------------------------------*/

    స్థిర శూన్య prvProcessExpiredTimer( స్థిరరాశి TickType_t x_తదుపరి_Expire_సమయం,
                                        స్థిరరాశి TickType_t x_సమయం_Now )
    {
        Timer_t * స్థిరరాశి pxTimer = ( Timer_t * ) list_GET_OWNER_యొక్క_తల_నమోదు( pxCurrentTimerList ); /*lint !e9087 !e9079 void * is used as this macro is used with tasks and co-routines too.  Alignment is known to be fine as the type of the pointer stored and retrieved is the same. */

        /* Remove the timer from the list of active timers.  A check has already
         * been performed to ensure the list is not empty. */

        ( శూన్య ) uxListRemove( &( pxTimer->x_Timer_List_అంశం ) );

        /* If the timer is an auto-reload timer then calculate the next
         * expiry time and re-insert the timer in the list of active timers. */
        ఒకవేళ( ( pxTimer->uc_స్థితి & tmr_స్థితి_IS_AUTORELOAD ) != 0 )
        {
            prvReloadTimer( pxTimer, x_తదుపరి_Expire_సమయం, x_సమయం_Now );
        }
        లేకపోతే
        {
            pxTimer->uc_స్థితి &= ( ( uint8_t ) ~tmr_స్థితి_IS_చురుకైన );
        }

        /* Call the timer callback. */
        traceTIMER_EXPIRED( pxTimer );
        pxTimer->pxCallbackFunction( ( TimerHandle_t ) pxTimer );
    }
/*-----------------------------------------------------------*/

    స్థిర పోర్టు_TASK_FUNCTION( prvTimerTask, pvParameters )
    {
        TickType_t x_తదుపరి_Expire_సమయం;
        పునాది_Type_t x_List_Was_ఖాళీలు;

        /* Just to avoid compiler warnings. */
        ( శూన్య ) pvParameters;

        #ఒకవేళ ( configUSE_DAEMON_TASK_STARTUP_HOOK == 1 )
        {
            బాహ్య శూన్య vApplicationDaemonTaskStartupHook( శూన్య );

            /* Allow the application writer to execute some code in the context of
             * this task at the point the task starts executing.  This is useful if the
             * application includes initialisation code that would benefit from
             * executing after the scheduler has been started. */
            vApplicationDaemonTaskStartupHook();
        }
        #అయితే_ముగింపు /* configUSE_DAEMON_TASK_STARTUP_HOOK */

        కొరకు( ; ; )
        {
            /* Query the timers list to see if it contains any timers, and if so,
             * obtain the time at which the next timer will expire. */
            x_తదుపరి_Expire_సమయం = prv_Get_తదుపరి_Expire_సమయం( &x_List_Was_ఖాళీలు );

            /* If a timer has expired, process it.  Otherwise, block this task
             * until either a timer does expire, or a command is received. */
            prv_Process_Timer_Or_ఖండం_Task( x_తదుపరి_Expire_సమయం, x_List_Was_ఖాళీలు );

            /* Empty the command queue. */
            prvProcessReceivedCommands();
        }
    }
/*-----------------------------------------------------------*/

    స్థిర శూన్య prv_Process_Timer_Or_ఖండం_Task( స్థిరరాశి TickType_t x_తదుపరి_Expire_సమయం,
                                            పునాది_Type_t x_List_Was_ఖాళీలు )
    {
        TickType_t x_సమయం_Now;
        పునాది_Type_t xTimerListsWereSwitched;

        prvENTER_CRITICAL_OR_SUSPEND_ALL( &x_Timer_తాళం );
        {
            /* Obtain the time now to make an assessment as to whether the timer
             * has expired or not.  If obtaining the time causes the lists to switch
             * then don't process this timer as any timers that remained in the list
             * when the lists were switched will have been processed within the
             * prvSampleTimeNow() function. */
            x_సమయం_Now = prv_Sample_సమయం_Now( &xTimerListsWereSwitched );

            ఒకవేళ( xTimerListsWereSwitched == pdFALSE )
            {
                /* The tick count has not overflowed, has the timer expired? */
                ఒకవేళ( ( x_List_Was_ఖాళీలు == pdFALSE ) && ( x_తదుపరి_Expire_సమయం <= x_సమయం_Now ) )
                {
                    ( శూన్య ) prvEXIT_CRITICAL_OR_RESUME_ALL( &x_Timer_తాళం );
                    prvProcessExpiredTimer( x_తదుపరి_Expire_సమయం, x_సమయం_Now );
                }
                లేకపోతే
                {
                    /* The tick count has not overflowed, and the next expire
                     * time has not been reached yet.  This task should therefore
                     * block to wait for the next expire time or a command to be
                     * received - whichever comes first.  The following line cannot
                     * be reached unless xNextExpireTime > xTimeNow, except in the
                     * case when the current timer list is empty. */
                    ఒకవేళ( x_List_Was_ఖాళీలు != pdFALSE )
                    {
                        /* The current timer list is empty - is the overflow list
                         * also empty? */
                        x_List_Was_ఖాళీలు = list_LIST_IS_ఖాళీలు( px_అధికప్రవాహం_Timer_List );
                    }

                    v_వరుస_Wait_For_సందేశం_Restricted( x_Timer_వరుస, ( x_తదుపరి_Expire_సమయం - x_సమయం_Now ), x_List_Was_ఖాళీలు );

                    ఒకవేళ( prvEXIT_CRITICAL_OR_RESUME_ALL( &x_Timer_తాళం ) == pdFALSE )
                    {
                        /* Yield to wait for either a command to arrive, or the
                         * block time to expire.  If a command arrived between the
                         * critical section being exited and this yield then the yield
                         * will not cause the task to block. */
                        పోర్టు_YIELD_WITHIN_API();
                    }
                    లేకపోతే
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
            }
            లేకపోతే
            {
                ( శూన్య ) prvEXIT_CRITICAL_OR_RESUME_ALL( &x_Timer_తాళం );
            }
        }
    }
/*-----------------------------------------------------------*/

    స్థిర TickType_t prv_Get_తదుపరి_Expire_సమయం( పునాది_Type_t * స్థిరరాశి px_List_Was_ఖాళీలు )
    {
        TickType_t x_తదుపరి_Expire_సమయం;

        /* Timers are listed in expiry time order, with the head of the list
         * referencing the task that will expire first.  Obtain the time at which
         * the timer with the nearest expiry time will expire.  If there are no
         * active timers then just set the next expire time to 0.  That will cause
         * this task to unblock when the tick count overflows, at which point the
         * timer lists will be switched and the next expiry time can be
         * re-assessed.  */
        *px_List_Was_ఖాళీలు = list_LIST_IS_ఖాళీలు( pxCurrentTimerList );

        ఒకవేళ( *px_List_Was_ఖాళీలు == pdFALSE )
        {
            x_తదుపరి_Expire_సమయం = list_GET_అంశం_విలువ_యొక్క_తల_నమోదు( pxCurrentTimerList );
        }
        లేకపోతే
        {
            /* Ensure the task unblocks when the tick count rolls over. */
            x_తదుపరి_Expire_సమయం = ( TickType_t ) 0U;
        }

        తిరిగి x_తదుపరి_Expire_సమయం;
    }
/*-----------------------------------------------------------*/

    స్థిర TickType_t prv_Sample_సమయం_Now( పునాది_Type_t * స్థిరరాశి pxTimerListsWereSwitched )
    {
        TickType_t x_సమయం_Now;
        PRIVILEGED_దత్తాంశం స్థిర TickType_t x_చివరి_సమయం = ( TickType_t ) 0U; /*lint !e956 Variable is only accessible to one task. */

        x_సమయం_Now = x_Task_Get_Tick_లెక్క();

        ఒకవేళ( x_సమయం_Now < x_చివరి_సమయం )
        {
            prvSwitchTimerLists();
            *pxTimerListsWereSwitched = pdTRUE;
        }
        లేకపోతే
        {
            *pxTimerListsWereSwitched = pdFALSE;
        }

        x_చివరి_సమయం = x_సమయం_Now;

        తిరిగి x_సమయం_Now;
    }
/*-----------------------------------------------------------*/

    స్థిర పునాది_Type_t prv_చొప్పించు_Timer_లోపల_చురుకైన_List( Timer_t * స్థిరరాశి pxTimer,
                                                  స్థిరరాశి TickType_t x_తదుపరి_Expiry_సమయం,
                                                  స్థిరరాశి TickType_t x_సమయం_Now,
                                                  స్థిరరాశి TickType_t x_Command_సమయం )
    {
        పునాది_Type_t xProcessTimerNow = pdFALSE;

        list_సమితి_LIST_అంశం_విలువ( &( pxTimer->x_Timer_List_అంశం ), x_తదుపరి_Expiry_సమయం );
        list_సమితి_LIST_అంశం_OWNER( &( pxTimer->x_Timer_List_అంశం ), pxTimer );

        ఒకవేళ( x_తదుపరి_Expiry_సమయం <= x_సమయం_Now )
        {
            /* Has the expiry time elapsed between the command to start/reset a
             * timer was issued, and the time the command was processed? */
            ఒకవేళ( ( ( TickType_t ) ( x_సమయం_Now - x_Command_సమయం ) ) >= pxTimer->x_Timer_Period_లోపల_Ticks ) /*lint !e961 MISRA exception as the casts are only redundant for some ports. */
            {
                /* The time between a command being issued and the command being
                 * processed actually exceeds the timers period.  */
                xProcessTimerNow = pdTRUE;
            }
            లేకపోతే
            {
                v_List_చొప్పించు( px_అధికప్రవాహం_Timer_List, &( pxTimer->x_Timer_List_అంశం ) );
            }
        }
        లేకపోతే
        {
            ఒకవేళ( ( x_సమయం_Now < x_Command_సమయం ) && ( x_తదుపరి_Expiry_సమయం >= x_Command_సమయం ) )
            {
                /* If, since the command was issued, the tick count has overflowed
                 * but the expiry time has not, then the timer must have already passed
                 * its expiry time and should be processed immediately. */
                xProcessTimerNow = pdTRUE;
            }
            లేకపోతే
            {
                v_List_చొప్పించు( pxCurrentTimerList, &( pxTimer->x_Timer_List_అంశం ) );
            }
        }

        తిరిగి xProcessTimerNow;
    }
/*-----------------------------------------------------------*/

    స్థిర శూన్య prvProcessReceivedCommands( శూన్య )
    {
        Daemon_Task_సందేశం_t x_సందేశం;
        Timer_t * pxTimer;
        పునాది_Type_t xTimerListsWereSwitched;
        TickType_t x_సమయం_Now;

        అయితే( x_వరుస_Receive( x_Timer_వరుస, &x_సందేశం, tmrNO_DELAY ) != pdFAIL ) /*lint !e603 xMessage does not have to be initialised as it is passed out, not in, and it is not used unless xQueueReceive() returns pdTRUE. */
        {
            #ఒకవేళ ( INCLUDE_xTimerPendFunctionCall == 1 )
            {
                /* Negative commands are pended function calls rather than timer
                 * commands. */
                ఒకవేళ( x_సందేశం.x_సందేశం_ID < ( పునాది_Type_t ) 0 )
                {
                    స్థిరరాశి CallbackParameters_t * స్థిరరాశి pxCallback = &( x_సందేశం.u.xCallbackParameters );

                    /* The timer uses the xCallbackParameters member to request a
                     * callback be executed.  Check the callback is not NULL. */
                    configASSERT( pxCallback );

                    /* Call the function. */
                    pxCallback->pxCallbackFunction( pxCallback->pvParameter1, pxCallback->ulParameter2 );
                }
                లేకపోతే
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            #అయితే_ముగింపు /* INCLUDE_xTimerPendFunctionCall */

            /* Commands that are positive are timer commands rather than pended
             * function calls. */
            ఒకవేళ( x_సందేశం.x_సందేశం_ID >= ( పునాది_Type_t ) 0 )
            {
                /* The messages uses the xTimerParameters member to work on a
                 * software timer. */
                pxTimer = x_సందేశం.u.xTimerParameters.pxTimer;

                ఒకవేళ( listIS_CONTAINED_WITHIN( ఖాళీ, &( pxTimer->x_Timer_List_అంశం ) ) == pdFALSE ) /*lint !e961. The cast is only redundant when NULL is passed into the macro. */
                {
                    /* The timer is in a list, remove it. */
                    ( శూన్య ) uxListRemove( &( pxTimer->x_Timer_List_అంశం ) );
                }
                లేకపోతే
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                traceTIMER_COMMAND_RECEIVED( pxTimer, x_సందేశం.x_సందేశం_ID, x_సందేశం.u.xTimerParameters.x_సందేశం_విలువ );

                /* In this case the xTimerListsWereSwitched parameter is not used, but
                 *  it must be present in the function call.  prvSampleTimeNow() must be
                 *  called after the message is received from xTimerQueue so there is no
                 *  possibility of a higher priority task adding a message to the message
                 *  queue with a time that is ahead of the timer daemon task (because it
                 *  pre-empted the timer daemon task after the xTimeNow value was set). */
                x_సమయం_Now = prv_Sample_సమయం_Now( &xTimerListsWereSwitched );

                ఎంపిక( x_సందేశం.x_సందేశం_ID )
                {
                    స్థితి tmr_COMMAND_ప్రారంభం:
                    స్థితి tmr_COMMAND_ప్రారంభం_FROM_ISR:
                    స్థితి tmrCOMMAND_RESET:
                    స్థితి tmrCOMMAND_RESET_FROM_ISR:
                        /* Start or restart a timer. */
                        pxTimer->uc_స్థితి |= tmr_స్థితి_IS_చురుకైన;

                        ఒకవేళ( prv_చొప్పించు_Timer_లోపల_చురుకైన_List( pxTimer, x_సందేశం.u.xTimerParameters.x_సందేశం_విలువ + pxTimer->x_Timer_Period_లోపల_Ticks, x_సమయం_Now, x_సందేశం.u.xTimerParameters.x_సందేశం_విలువ ) != pdFALSE )
                        {
                            /* The timer expired before it was added to the active
                             * timer list.  Process it now. */
                            ఒకవేళ( ( pxTimer->uc_స్థితి & tmr_స్థితి_IS_AUTORELOAD ) != 0 )
                            {
                                prvReloadTimer( pxTimer, x_సందేశం.u.xTimerParameters.x_సందేశం_విలువ + pxTimer->x_Timer_Period_లోపల_Ticks, x_సమయం_Now );
                            }
                            లేకపోతే
                            {
                                pxTimer->uc_స్థితి &= ( ( uint8_t ) ~tmr_స్థితి_IS_చురుకైన );
                            }

                            /* Call the timer callback. */
                            traceTIMER_EXPIRED( pxTimer );
                            pxTimer->pxCallbackFunction( ( TimerHandle_t ) pxTimer );
                        }
                        లేకపోతే
                        {
                            mtCOVERAGE_TEST_MARKER();
                        }

                        ఆపు;

                    స్థితి tmrCOMMAND_STOP:
                    స్థితి tmrCOMMAND_STOP_FROM_ISR:
                        /* The timer has already been removed from the active list. */
                        pxTimer->uc_స్థితి &= ( ( uint8_t ) ~tmr_స్థితి_IS_చురుకైన );
                        ఆపు;

                    స్థితి tmrCOMMAND_CHANGE_PERIOD:
                    స్థితి tmrCOMMAND_CHANGE_PERIOD_FROM_ISR:
                        pxTimer->uc_స్థితి |= tmr_స్థితి_IS_చురుకైన;
                        pxTimer->x_Timer_Period_లోపల_Ticks = x_సందేశం.u.xTimerParameters.x_సందేశం_విలువ;
                        configASSERT( ( pxTimer->x_Timer_Period_లోపల_Ticks > 0 ) );

                        /* The new period does not really have a reference, and can
                         * be longer or shorter than the old one.  The command time is
                         * therefore set to the current time, and as the period cannot
                         * be zero the next expiry time can only be in the future,
                         * meaning (unlike for the xTimerStart() case above) there is
                         * no fail case that needs to be handled here. */
                        ( శూన్య ) prv_చొప్పించు_Timer_లోపల_చురుకైన_List( pxTimer, ( x_సమయం_Now + pxTimer->x_Timer_Period_లోపల_Ticks ), x_సమయం_Now, x_సమయం_Now );
                        ఆపు;

                    స్థితి tmrCOMMAND_DELETE:
                        #ఒకవేళ ( config_SUPPORT_DYNAMIC_కేటాయింపు == 1 )
                        {
                            /* The timer has already been removed from the active list,
                             * just free up the memory if the memory was dynamically
                             * allocated. */
                            ఒకవేళ( ( pxTimer->uc_స్థితి & tmr_స్థితి_IS_STATICALLY_ALLOCATED ) == ( uint8_t ) 0 )
                            {
                                v_పోర్టు_Free( pxTimer );
                            }
                            లేకపోతే
                            {
                                pxTimer->uc_స్థితి &= ( ( uint8_t ) ~tmr_స్థితి_IS_చురుకైన );
                            }
                        }
                        #లేకపోతే /* if ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) */
                        {
                            /* If dynamic allocation is not enabled, the memory
                             * could not have been dynamically allocated. So there is
                             * no need to free the memory - just mark the timer as
                             * "not active". */
                            pxTimer->uc_స్థితి &= ( ( uint8_t ) ~tmr_స్థితి_IS_చురుకైన );
                        }
                        #అయితే_ముగింపు /* configSUPPORT_DYNAMIC_ALLOCATION */
                        ఆపు;

                    అప్రమేయ:
                        /* Don't expect to get here. */
                        ఆపు;
                }
            }
        }
    }
/*-----------------------------------------------------------*/

    స్థిర శూన్య prvSwitchTimerLists( శూన్య )
    {
        TickType_t x_తదుపరి_Expire_సమయం;
        List_t * px_తాత్కాలిక;

        /* The tick count has overflowed.  The timer lists must be switched.
         * If there are any timers still referenced from the current timer list
         * then they must have expired and should be processed before the lists
         * are switched. */
        అయితే( list_LIST_IS_ఖాళీలు( pxCurrentTimerList ) == pdFALSE )
        {
            x_తదుపరి_Expire_సమయం = list_GET_అంశం_విలువ_యొక్క_తల_నమోదు( pxCurrentTimerList );

            /* Process the expired timer.  For auto-reload timers, be careful to
             * process only expirations that occur on the current list.  Further
             * expirations must wait until after the lists are switched. */
            prvProcessExpiredTimer( x_తదుపరి_Expire_సమయం, tmr_MAX_సమయం_BEFORE_అధికప్రవాహం );
        }

        px_తాత్కాలిక = pxCurrentTimerList;
        pxCurrentTimerList = px_అధికప్రవాహం_Timer_List;
        px_అధికప్రవాహం_Timer_List = px_తాత్కాలిక;
    }
/*-----------------------------------------------------------*/

    స్థిర శూన్య prv_Check_For_Valid_List_And_వరుస( శూన్య )
    {
        /* Check that the list from which active timers are referenced, and the
         * queue used to communicate with the timer service, have been
         * initialised. */
        taskENTER_CRITICAL( &x_Timer_తాళం );
        {
            ఒకవేళ( x_Timer_వరుస == ఖాళీ )
            {
                vListInitialise( &x_చురుకైన_Timer_List1 );
                vListInitialise( &x_చురుకైన_Timer_List2 );
                pxCurrentTimerList = &x_చురుకైన_Timer_List1;
                px_అధికప్రవాహం_Timer_List = &x_చురుకైన_Timer_List2;

                #ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )
                {
                    /* The timer queue is allocated statically in case
                     * configSUPPORT_DYNAMIC_ALLOCATION is 0. */
                    PRIVILEGED_దత్తాంశం స్థిర Static_వరుస_t x_Static_Timer_వరుస;                                                                          /*lint !e956 Ok to declare in this manner to prevent additional conditional compilation guards in other locations. */
                    PRIVILEGED_దత్తాంశం స్థిర uint8_t uc_Static_Timer_వరుస_Storage[ ( size_t ) config_TIMER_వరుస_నిడివి * పరిమాణం( Daemon_Task_సందేశం_t ) ]; /*lint !e956 Ok to declare in this manner to prevent additional conditional compilation guards in other locations. */

                    x_Timer_వరుస = x_వరుస_Create_Static( ( UBaseType_t ) config_TIMER_వరుస_నిడివి, ( UBaseType_t ) పరిమాణం( Daemon_Task_సందేశం_t ), &( uc_Static_Timer_వరుస_Storage[ 0 ] ), &x_Static_Timer_వరుస );
                }
                #లేకపోతే
                {
                    x_Timer_వరుస = x_వరుస_Create( ( UBaseType_t ) config_TIMER_వరుస_నిడివి, పరిమాణం( Daemon_Task_సందేశం_t ) );
                }
                #అయితే_ముగింపు /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */

                #ఒకవేళ ( config_వరుస_REGISTRY_కొలత > 0 )
                {
                    ఒకవేళ( x_Timer_వరుస != ఖాళీ )
                    {
                        v_వరుస_కూడు_గమ్యం_Registry( x_Timer_వరుస, "TmrQ" );
                    }
                    లేకపోతే
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #అయితే_ముగింపు /* configQUEUE_REGISTRY_SIZE */
            }
            లేకపోతే
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        taskEXIT_CRITICAL( &x_Timer_తాళం );
    }
/*-----------------------------------------------------------*/

    పునాది_Type_t x_Timer_Is_Timer_చురుకైన( TimerHandle_t xTimer )
    {
        పునాది_Type_t xReturn;
        Timer_t * pxTimer = xTimer;

        configASSERT( xTimer );

        /* Is the timer in the list of active timers? */
        taskENTER_CRITICAL( &x_Timer_తాళం );
        {
            ఒకవేళ( ( pxTimer->uc_స్థితి & tmr_స్థితి_IS_చురుకైన ) == 0 )
            {
                xReturn = pdFALSE;
            }
            లేకపోతే
            {
                xReturn = pdTRUE;
            }
        }
        taskEXIT_CRITICAL( &x_Timer_తాళం );

        తిరిగి xReturn;
    } /*lint !e818 Can't be pointer to const due to the typedef. */
/*-----------------------------------------------------------*/

    శూన్య * pvTimerGetTimerID( స్థిరరాశి TimerHandle_t xTimer )
    {
        Timer_t * స్థిరరాశి pxTimer = xTimer;
        శూన్య * pvReturn;

        configASSERT( xTimer );

        taskENTER_CRITICAL( &x_Timer_తాళం );
        {
            pvReturn = pxTimer->pvTimerID;
        }
        taskEXIT_CRITICAL( &x_Timer_తాళం );

        తిరిగి pvReturn;
    }
/*-----------------------------------------------------------*/

    శూన్య v_Timer_సమితి_Timer_ID( TimerHandle_t xTimer,
                           శూన్య * pvNewID )
    {
        Timer_t * స్థిరరాశి pxTimer = xTimer;

        configASSERT( xTimer );

        taskENTER_CRITICAL( &x_Timer_తాళం );
        {
            pxTimer->pvTimerID = pvNewID;
        }
        taskEXIT_CRITICAL( &x_Timer_తాళం );
    }
/*-----------------------------------------------------------*/

    #ఒకవేళ ( INCLUDE_xTimerPendFunctionCall == 1 )

        పునాది_Type_t xTimerPendFunctionCallFromISR( PendedFunction_t x_Function_గమ్యం_Pend,
                                                  శూన్య * pvParameter1,
                                                  uint32_t ulParameter2,
                                                  పునాది_Type_t * px_Higher_ప్రాధాన్యత_Task_Woken )
        {
            Daemon_Task_సందేశం_t x_సందేశం;
            పునాది_Type_t xReturn;

            /* Complete the message with the function parameters and post it to the
             * daemon task. */
            x_సందేశం.x_సందేశం_ID = tmrCOMMAND_EXECUTE_CALLBACK_FROM_ISR;
            x_సందేశం.u.xCallbackParameters.pxCallbackFunction = x_Function_గమ్యం_Pend;
            x_సందేశం.u.xCallbackParameters.pvParameter1 = pvParameter1;
            x_సందేశం.u.xCallbackParameters.ulParameter2 = ulParameter2;

            xReturn = x_వరుస_Send_From_ISR( x_Timer_వరుస, &x_సందేశం, px_Higher_ప్రాధాన్యత_Task_Woken );

            tracePEND_FUNC_CALL_FROM_ISR( x_Function_గమ్యం_Pend, pvParameter1, ulParameter2, xReturn );

            తిరిగి xReturn;
        }

    #అయితే_ముగింపు /* INCLUDE_xTimerPendFunctionCall */
/*-----------------------------------------------------------*/

    #ఒకవేళ ( INCLUDE_xTimerPendFunctionCall == 1 )

        పునాది_Type_t xTimerPendFunctionCall( PendedFunction_t x_Function_గమ్యం_Pend,
                                           శూన్య * pvParameter1,
                                           uint32_t ulParameter2,
                                           TickType_t x_Ticks_గమ్యం_Wait )
        {
            Daemon_Task_సందేశం_t x_సందేశం;
            పునాది_Type_t xReturn;

            /* This function can only be called after a timer has been created or
             * after the scheduler has been started because, until then, the timer
             * queue does not exist. */
            configASSERT( x_Timer_వరుస );

            /* Complete the message with the function parameters and post it to the
             * daemon task. */
            x_సందేశం.x_సందేశం_ID = tmrCOMMAND_EXECUTE_CALLBACK;
            x_సందేశం.u.xCallbackParameters.pxCallbackFunction = x_Function_గమ్యం_Pend;
            x_సందేశం.u.xCallbackParameters.pvParameter1 = pvParameter1;
            x_సందేశం.u.xCallbackParameters.ulParameter2 = ulParameter2;

            xReturn = x_వరుస_Send_గమ్యం_వెనుక( x_Timer_వరుస, &x_సందేశం, x_Ticks_గమ్యం_Wait );

            tracePEND_FUNC_CALL( x_Function_గమ్యం_Pend, pvParameter1, ulParameter2, xReturn );

            తిరిగి xReturn;
        }

    #అయితే_ముగింపు /* INCLUDE_xTimerPendFunctionCall */
/*-----------------------------------------------------------*/

    #ఒకవేళ ( configUSE_TRACE_FACILITY == 1 )

        UBaseType_t ux_Timer_Get_Timer_సంఖ్య( TimerHandle_t xTimer )
        {
            తిరిగి ( ( Timer_t * ) xTimer )->ux_Timer_సంఖ్య;
        }

    #అయితే_ముగింపు /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

    #ఒకవేళ ( configUSE_TRACE_FACILITY == 1 )

        శూన్య v_Timer_సమితి_Timer_సంఖ్య( TimerHandle_t xTimer,
                                   UBaseType_t ux_Timer_సంఖ్య )
        {
            ( ( Timer_t * ) xTimer )->ux_Timer_సంఖ్య = ux_Timer_సంఖ్య;
        }

    #అయితే_ముగింపు /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

/* This entire source file will be skipped if the application is not configured
 * to include software timer functionality.  If you want to include software timer
 * functionality then ensure configUSE_TIMERS is set to 1 in FreeRTOSConfig.h. */
#అయితే_ముగింపు /* configUSE_TIMERS == 1 */
