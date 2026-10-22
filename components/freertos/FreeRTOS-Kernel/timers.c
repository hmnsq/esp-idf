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
#ضمن <stdlib.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#تعریف_کرو MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#ضمن "FreeRTOS.h"
#ضمن "task.h"
#ضمن "queue.h"
#ضمن "timers.h"
/* Include private IDF API additions for critical thread safety macros */
#ضمن "esp_private/freertos_idf_additions_priv.h"

#إذا ( INCLUDE_xTimerPendFunctionCall == 1 ) && ( configUSE_TIMERS == 0 )
    #غلطی configUSE_TIMERS must be سیٹ منزل 1 منزل make the xTimerPendFunctionCall() function دستیاب.
#جے_انت

/* Lint e9021, e961 and e750 are suppressed as a MISRA exception justified
 * because the MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
 * for the header files above, but not in this file, in order to generate the
 * correct privileged Vs unprivileged linkage and placement. */
#تعریف_رد MPU_WRAPPERS_INCLUDED_FROM_API_FILE /*lint !e9021 !e961 !e750. */


/* This entire source file will be skipped if the application is not configured
 * to include software timer functionality.  This #if is closed at the very bottom
 * of this file.  If you want to include software timer functionality then ensure
 * configUSE_TIMERS is set to 1 in FreeRTOSConfig.h. */
#إذا ( configUSE_TIMERS == 1 )

/* Misc definitions. */
    #تعریف_کرو tmrNO_DELAY                    ( ( TickType_t ) 0U )
    #تعریف_کرو tmr_MAX_ویلا_BEFORE_حد_توں_ودھ    ( ( TickType_t ) -1 )

/* The name assigned to the timer service task. This can be overridden by
 * defining configTIMER_SERVICE_TASK_NAME in FreeRTOSConfig.h. */
    #جے_بنا_تعریف configTIMER_SERVICE_TASK_NAME
        #تعریف_کرو configTIMER_SERVICE_TASK_NAME    "Tmr Svc"
    #جے_انت

    #إذا ( config_نمبر_دا_CORES > 1 )

/* The core affinity assigned to the timer service task on SMP systems.
 * This can be overridden by defining configTIMER_SERVICE_TASK_CORE_AFFINITY in FreeRTOSConfig.h. */
        #جے_بنا_تعریف configTIMER_SERVICE_TASK_CORE_AFFINITY
            #تعریف_کرو configTIMER_SERVICE_TASK_CORE_AFFINITY    tskNO_AFFINITY
        #جے_انت

        #إذا ( !( ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x0 ) || ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x1 ) || ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x7FFFFFFF /* tskNO_AFFINITY */ ) ) )
            #غلطی Invalid Timer task affinity. Timer task core ID can only be 0x0, 0x1 or 0x7FFFFFFF (tskNO_AFFINITY).
        #جے_انت /* #if ( !( ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x0 ) || ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x01 ) || ( configTIMER_SERVICE_TASK_CORE_AFFINITY == 0x7FFFFFFF ) ) ) */
    #جے_انت /* #if ( configNUMBER_OF_CORES > 1 ) */

/* Bit definitions used in the ucStatus member of a timer structure. */
    #تعریف_کرو tmr_حالت_IS_سرگرم                  ( ( uint8_t ) 0x01 )
    #تعریف_کرو tmr_حالت_IS_STATICALLY_ALLOCATED    ( ( uint8_t ) 0x02 )
    #تعریف_کرو tmr_حالت_IS_AUTORELOAD              ( ( uint8_t ) 0x04 )

/* The definition of the timers themselves. */
    تعريفنوع بنية tmrTimerControl                  /* The old naming convention is used to prevent breaking kernel aware debuggers. */
    {
        ثابت حرف * pcTimerName;                   /*<< Text name.  This is not used by the kernel, it is included simply to make debugging easier. */ /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
        List_شے_t x_Timer_List_شے;                  /*<< Standard linked list item as used by all kernel features for event management. */
        TickType_t x_Timer_Period_وچ_Ticks;             /*<< How quickly and often the timer expires. */
        فراغ * pvTimerID;                           /*<< An ID to identify the timer.  This allows the timer to be identified when the same callback is used for multiple timers. */
        TimerCallbackFunction_t pxCallbackFunction; /*<< The function that will be called when the timer expires. */
        #إذا ( configUSE_TRACE_FACILITY == 1 )
            UBaseType_t ux_Timer_نمبر;              /*<< An ID assigned by trace tools such as FreeRTOS+Trace */
        #جے_انت
        uint8_t uc_حالت;                           /*<< Holds bits to say if the timer was statically allocated or not, and if it is active or not. */
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
        TickType_t x_سنیہا_قدر; /*<< An optional value used by a subset of commands, for example, when changing the period of a timer. */
        Timer_t * pxTimer;        /*<< The timer to which the command will be applied. */
    } TimerParameter_t;


    تعريفنوع بنية tmrCallbackParameters
    {
        PendedFunction_t pxCallbackFunction; /* << The callback function to execute. */
        فراغ * pvParameter1;                 /* << The value that will be used as the callback functions first parameter. */
        uint32_t ulParameter2;               /* << The value that will be used as the callback functions second parameter. */
    } CallbackParameters_t;

/* The structure that contains the two message types, along with an identifier
 * that is used to determine which message type is valid. */
    تعريفنوع بنية tmr_Timer_قطار_سنیہا
    {
        ادھار_Type_t x_سنیہا_ID; /*<< The command being sent to the timer service task. */
        اتحاد
        {
            TimerParameter_t xTimerParameters;

            /* Don't include xCallbackParameters if it is not going to be used as
             * it makes the structure (and therefore the timer queue) larger. */
            #إذا ( INCLUDE_xTimerPendFunctionCall == 1 )
                CallbackParameters_t xCallbackParameters;
            #جے_انت /* INCLUDE_xTimerPendFunctionCall */
        } u;
    } Daemon_Task_سنیہا_t;

/*lint -save -e956 A manual analysis and inspection has been used to determine
 * which static variables must be declared volatile. */

/* The list in which active timers are stored.  Timers are referenced in expire
 * time order, with the nearest expiry time at the front of the list.  Only the
 * timer service task is allowed to access these lists.
 * xActiveTimerList1 and xActiveTimerList2 could be at function scope but that
 * breaks some kernel aware debuggers, and debuggers that reply on removing the
 * static qualifier. */
    PRIVILEGED_انکڑے ساكن List_t x_سرگرم_Timer_List1;
    PRIVILEGED_انکڑے ساكن List_t x_سرگرم_Timer_List2;
    PRIVILEGED_انکڑے ساكن List_t * pxCurrentTimerList;
    PRIVILEGED_انکڑے ساكن List_t * px_حد_توں_ودھ_Timer_List;

/* A queue that is used to send commands to the timer service task. */
    PRIVILEGED_انکڑے ساكن قطار_Handle_t x_Timer_قطار = عدم;
    PRIVILEGED_انکڑے ساكن TaskHandle_t xTimerTaskHandle = عدم;

/* Spinlock required in SMP when accessing the timers. For now we use a single lock
 * Todo: Each timer could possible have its own lock for increased granularity. */
    PRIVILEGED_انکڑے ساكن پورٹ_MUX_TYPE x_Timer_جندرا = پورٹ_MUX_INITIALIZER_UNLOCKED;

/*lint -restore */

/*-----------------------------------------------------------*/

/*
 * Initialise the infrastructure used by the timer service task if it has not
 * been initialised already.
 */
    ساكن فراغ prv_Check_For_Valid_List_And_قطار( فراغ ) PRIVILEGED_FUNCTION;

/*
 * The timer service task (daemon).  Timer functionality is controlled by this
 * task.  Other tasks communicate with the timer service task using the
 * xTimerQueue queue.
 */
    ساكن پورٹ_TASK_FUNCTION_PROTO( prvTimerTask, pvParameters ) PRIVILEGED_FUNCTION;

/*
 * Called by the timer service task to interpret and process a command it
 * received on the timer queue.
 */
    ساكن فراغ prvProcessReceivedCommands( فراغ ) PRIVILEGED_FUNCTION;

/*
 * Insert the timer into either xActiveTimerList1, or xActiveTimerList2,
 * depending on if the expire time causes a timer counter overflow.
 */
    ساكن ادھار_Type_t prv_داخل_کرو_Timer_وچ_سرگرم_List( Timer_t * ثابت pxTimer,
                                                  ثابت TickType_t x_اگلا_Expiry_ویلا,
                                                  ثابت TickType_t x_ویلا_Now,
                                                  ثابت TickType_t x_Command_ویلا ) PRIVILEGED_FUNCTION;

/*
 * Reload the specified auto-reload timer.  If the reloading is backlogged,
 * clear the backlog, calling the callback for each additional reload.  When
 * this function returns, the next expiry time is after xTimeNow.
 */
    ساكن فراغ prvReloadTimer( Timer_t * ثابت pxTimer,
                                TickType_t x_Expired_ویلا,
                                ثابت TickType_t x_ویلا_Now ) PRIVILEGED_FUNCTION;

/*
 * An active timer has reached its expire time.  Reload the timer if it is an
 * auto-reload timer, then call its callback.
 */
    ساكن فراغ prvProcessExpiredTimer( ثابت TickType_t x_اگلا_Expire_ویلا,
                                        ثابت TickType_t x_ویلا_Now ) PRIVILEGED_FUNCTION;

/*
 * The tick count has overflowed.  Switch the timer lists after ensuring the
 * current timer list does not still reference some timers.
 */
    ساكن فراغ prvSwitchTimerLists( فراغ ) PRIVILEGED_FUNCTION;

/*
 * Obtain the current tick count, setting *pxTimerListsWereSwitched to pdTRUE
 * if a tick count overflow occurred since prvSampleTimeNow() was last called.
 */
    ساكن TickType_t prv_Sample_ویلا_Now( ادھار_Type_t * ثابت pxTimerListsWereSwitched ) PRIVILEGED_FUNCTION;

/*
 * If the timer list contains any active timers then return the expire time of
 * the timer that will expire first and set *pxListWasEmpty to false.  If the
 * timer list does not contain any timers then return 0 and set *pxListWasEmpty
 * to pdTRUE.
 */
    ساكن TickType_t prv_Get_اگلا_Expire_ویلا( ادھار_Type_t * ثابت px_List_Was_خالی_تھاواں ) PRIVILEGED_FUNCTION;

/*
 * If a timer has expired, process it.  Otherwise, block the timer service task
 * until either a timer does expire or a command is received.
 */
    ساكن فراغ prv_Process_Timer_Or_بلاک_Task( ثابت TickType_t x_اگلا_Expire_ویلا,
                                            ادھار_Type_t x_List_Was_خالی_تھاواں ) PRIVILEGED_FUNCTION;

/*
 * Called after a Timer_t structure has been allocated either statically or
 * dynamically to fill in the structure's members.
 */
    ساكن فراغ prvInitialiseNewTimer( ثابت حرف * ثابت pcTimerName, /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                       ثابت TickType_t x_Timer_Period_وچ_Ticks,
                                       ثابت ادھار_Type_t xAutoReload,
                                       فراغ * ثابت pvTimerID,
                                       TimerCallbackFunction_t pxCallbackFunction,
                                       Timer_t * pxNewTimer ) PRIVILEGED_FUNCTION;
/*-----------------------------------------------------------*/

    ادھار_Type_t xTimerCreateTimerTask( فراغ )
    {
        ادھار_Type_t xReturn = pdFAIL;

        /* This function is called when the scheduler is started if
         * configUSE_TIMERS is set to 1.  Check that the infrastructure used by the
         * timer service task has been created/initialised.  If timers have already
         * been created then the initialisation will already have been performed. */
        prv_Check_For_Valid_List_And_قطار();

        إذا( x_Timer_قطار != عدم )
        {
            /* The following code block to set the timer task core affinity has been picked up from the FreeRTOS-Kernel
             * upstream repository. Commit# 631ae9e6e4828cf5c7d58bb7ac78a64fc309bcb7.
             * Necessary changes have been made for IDF-FreeRTOS compatibility. */
            #إذا ( config_نمبر_دا_CORES > 1 )
            {
                #إذا ( config_SUPPORT_STATIC_ونڈ == 1 )
                {
                    StaticTask_t * pxTimerTaskTCBBuffer = عدم;
                    ڈھیر_Type_t * px_Timer_Task_ڈھیر_بفر = عدم;
                    uint32_t ul_Timer_Task_ڈھیر_ناپ;

                    v_Application_Get_Timer_Task_حافظہ( &pxTimerTaskTCBBuffer, &px_Timer_Task_ڈھیر_بفر, &ul_Timer_Task_ڈھیر_ناپ );
                    xTimerTaskHandle = x_Task_Create_Static_Pinned_منزل_Core( prvTimerTask,
                                                                      configTIMER_SERVICE_TASK_NAME,
                                                                      ul_Timer_Task_ڈھیر_ناپ,
                                                                      عدم,
                                                                      ( ( UBaseType_t ) config_TIMER_TASK_ترجیح ) | پورٹ_PRIVILEGE_ثنائی_ہندسہ,
                                                                      px_Timer_Task_ڈھیر_بفر,
                                                                      pxTimerTaskTCBBuffer,
                                                                      configTIMER_SERVICE_TASK_CORE_AFFINITY );

                    إذا( xTimerTaskHandle != عدم )
                    {
                        xReturn = pdPASS;
                    }
                }
                #وإلا /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
                {
                    xReturn = x_Task_Create_Pinned_منزل_Core( prvTimerTask,
                                                       configTIMER_SERVICE_TASK_NAME,
                                                       config_TIMER_TASK_ڈھیر_ڈونگھائی,
                                                       عدم,
                                                       ( ( UBaseType_t ) config_TIMER_TASK_ترجیح ) | پورٹ_PRIVILEGE_ثنائی_ہندسہ,
                                                       &xTimerTaskHandle,
                                                       configTIMER_SERVICE_TASK_CORE_AFFINITY );
                }
                #جے_انت /* configSUPPORT_STATIC_ALLOCATION */
            }
            #وإلا /* #if ( configNUMBER_OF_CORES > 1 ) */
            {
                #إذا ( config_SUPPORT_STATIC_ونڈ == 1 )
                {
                    StaticTask_t * pxTimerTaskTCBBuffer = عدم;
                    ڈھیر_Type_t * px_Timer_Task_ڈھیر_بفر = عدم;
                    uint32_t ul_Timer_Task_ڈھیر_ناپ;

                    v_Application_Get_Timer_Task_حافظہ( &pxTimerTaskTCBBuffer, &px_Timer_Task_ڈھیر_بفر, &ul_Timer_Task_ڈھیر_ناپ );
                    xTimerTaskHandle = xTaskCreateStatic( prvTimerTask,
                                                          configTIMER_SERVICE_TASK_NAME,
                                                          ul_Timer_Task_ڈھیر_ناپ,
                                                          عدم,
                                                          ( ( UBaseType_t ) config_TIMER_TASK_ترجیح ) | پورٹ_PRIVILEGE_ثنائی_ہندسہ,
                                                          px_Timer_Task_ڈھیر_بفر,
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
                                           config_TIMER_TASK_ڈھیر_ڈونگھائی,
                                           عدم,
                                           ( ( UBaseType_t ) config_TIMER_TASK_ترجیح ) | پورٹ_PRIVILEGE_ثنائی_ہندسہ,
                                           &xTimerTaskHandle );
                }
                #جے_انت /* configSUPPORT_STATIC_ALLOCATION */
            }
            #جے_انت /* #if ( configNUMBER_OF_CORES > 1 ) */
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }

        configASSERT( xReturn );
        أرجع xReturn;
    }
/*-----------------------------------------------------------*/

    #إذا ( config_SUPPORT_DYNAMIC_ونڈ == 1 )

        TimerHandle_t xTimerCreate( ثابت حرف * ثابت pcTimerName, /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                    ثابت TickType_t x_Timer_Period_وچ_Ticks,
                                    ثابت ادھار_Type_t xAutoReload,
                                    فراغ * ثابت pvTimerID,
                                    TimerCallbackFunction_t pxCallbackFunction )
        {
            Timer_t * pxNewTimer;

            pxNewTimer = ( Timer_t * ) pv_پورٹ_Malloc( حجم( Timer_t ) ); /*lint !e9087 !e9079 All values returned by pvPortMalloc() have at least the alignment required by the MCU's stack, and the first member of Timer_t is always a pointer to the timer's mame. */

            إذا( pxNewTimer != عدم )
            {
                /* Status is thus far zero as the timer is not created statically
                 * and has not been started.  The auto-reload bit may get set in
                 * prvInitialiseNewTimer. */
                pxNewTimer->uc_حالت = 0x00;
                prvInitialiseNewTimer( pcTimerName, x_Timer_Period_وچ_Ticks, xAutoReload, pvTimerID, pxCallbackFunction, pxNewTimer );
            }

            أرجع pxNewTimer;
        }

    #جے_انت /* configSUPPORT_DYNAMIC_ALLOCATION */
/*-----------------------------------------------------------*/

    #إذا ( config_SUPPORT_STATIC_ونڈ == 1 )

        TimerHandle_t xTimerCreateStatic( ثابت حرف * ثابت pcTimerName, /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                          ثابت TickType_t x_Timer_Period_وچ_Ticks,
                                          ثابت ادھار_Type_t xAutoReload,
                                          فراغ * ثابت pvTimerID,
                                          TimerCallbackFunction_t pxCallbackFunction,
                                          StaticTimer_t * px_Timer_بفر )
        {
            Timer_t * pxNewTimer;

            #إذا ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticTimer_t equals the size of the real timer
                 * structure. */
                متطاير size_t x_ناپ = حجم( StaticTimer_t );
                configASSERT( x_ناپ == حجم( Timer_t ) );
                ( فراغ ) x_ناپ; /* Keeps lint quiet when configASSERT() is not defined. */
            }
            #جے_انت /* configASSERT_DEFINED */

            /* A pointer to a StaticTimer_t structure MUST be provided, use it. */
            configASSERT( px_Timer_بفر );
            pxNewTimer = ( Timer_t * ) px_Timer_بفر; /*lint !e740 !e9087 StaticTimer_t is a pointer to a Timer_t, so guaranteed to be aligned and sized correctly (checked by an assert()), so this is safe. */

            إذا( pxNewTimer != عدم )
            {
                /* Timers can be created statically or dynamically so note this
                 * timer was created statically in case it is later deleted.  The
                 * auto-reload bit may get set in prvInitialiseNewTimer(). */
                pxNewTimer->uc_حالت = tmr_حالت_IS_STATICALLY_ALLOCATED;

                prvInitialiseNewTimer( pcTimerName, x_Timer_Period_وچ_Ticks, xAutoReload, pvTimerID, pxCallbackFunction, pxNewTimer );
            }

            أرجع pxNewTimer;
        }

    #جے_انت /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

    ساكن فراغ prvInitialiseNewTimer( ثابت حرف * ثابت pcTimerName, /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
                                       ثابت TickType_t x_Timer_Period_وچ_Ticks,
                                       ثابت ادھار_Type_t xAutoReload,
                                       فراغ * ثابت pvTimerID,
                                       TimerCallbackFunction_t pxCallbackFunction,
                                       Timer_t * pxNewTimer )
    {
        /* 0 is not a valid value for xTimerPeriodInTicks. */
        configASSERT( ( x_Timer_Period_وچ_Ticks > 0 ) );

        /* Ensure the infrastructure used by the timer service task has been
         * created/initialised. */
        prv_Check_For_Valid_List_And_قطار();

        /* Initialise the timer structure members using the function
         * parameters. */
        pxNewTimer->pcTimerName = pcTimerName;
        pxNewTimer->x_Timer_Period_وچ_Ticks = x_Timer_Period_وچ_Ticks;
        pxNewTimer->pvTimerID = pvTimerID;
        pxNewTimer->pxCallbackFunction = pxCallbackFunction;
        v_List_Initialise_شے( &( pxNewTimer->x_Timer_List_شے ) );

        إذا( xAutoReload != pdFALSE )
        {
            pxNewTimer->uc_حالت |= tmr_حالت_IS_AUTORELOAD;
        }

        traceTIMER_CREATE( pxNewTimer );
    }
/*-----------------------------------------------------------*/

    ادھار_Type_t xTimerGenericCommand( TimerHandle_t xTimer,
                                     ثابت ادھار_Type_t xCommandID,
                                     ثابت TickType_t x_Optional_قدر,
                                     ادھار_Type_t * ثابت px_Higher_ترجیح_Task_Woken,
                                     ثابت TickType_t x_Ticks_منزل_Wait )
    {
        ادھار_Type_t xReturn = pdFAIL;
        Daemon_Task_سنیہا_t x_سنیہا;

        configASSERT( xTimer );

        /* Send a message to the timer service task to perform a particular action
         * on a particular timer definition. */
        إذا( x_Timer_قطار != عدم )
        {
            /* Send a command to the timer service task to start the xTimer timer. */
            x_سنیہا.x_سنیہا_ID = xCommandID;
            x_سنیہا.u.xTimerParameters.x_سنیہا_قدر = x_Optional_قدر;
            x_سنیہا.u.xTimerParameters.pxTimer = xTimer;

            إذا( xCommandID < tmr_پہلا_FROM_ISR_COMMAND )
            {
                إذا( xTaskGetSchedulerState() == taskSCHEDULER_RUNNING )
                {
                    xReturn = x_قطار_Send_منزل_پٹھ( x_Timer_قطار, &x_سنیہا, x_Ticks_منزل_Wait );
                }
                وإلا
                {
                    xReturn = x_قطار_Send_منزل_پٹھ( x_Timer_قطار, &x_سنیہا, tmrNO_DELAY );
                }
            }
            وإلا
            {
                xReturn = x_قطار_Send_منزل_پٹھ_From_ISR( x_Timer_قطار, &x_سنیہا, px_Higher_ترجیح_Task_Woken );
            }

            traceTIMER_COMMAND_SEND( xTimer, xCommandID, x_Optional_قدر, xReturn );
        }
        وإلا
        {
            mtCOVERAGE_TEST_MARKER();
        }

        أرجع xReturn;
    }
/*-----------------------------------------------------------*/

    TaskHandle_t xTimerGetTimerDaemonTaskHandle( فراغ )
    {
        /* If xTimerGetTimerDaemonTaskHandle() is called before the scheduler has been
         * started, then xTimerTaskHandle will be NULL. */
        configASSERT( ( xTimerTaskHandle != عدم ) );
        أرجع xTimerTaskHandle;
    }
/*-----------------------------------------------------------*/

    TickType_t xTimerGetPeriod( TimerHandle_t xTimer )
    {
        Timer_t * pxTimer = xTimer;

        configASSERT( xTimer );
        أرجع pxTimer->x_Timer_Period_وچ_Ticks;
    }
/*-----------------------------------------------------------*/

    فراغ v_Timer_سیٹ_Reload_منوال( TimerHandle_t xTimer,
                              ثابت ادھار_Type_t xAutoReload )
    {
        Timer_t * pxTimer = xTimer;

        configASSERT( xTimer );
        taskENTER_CRITICAL( &x_Timer_جندرا );
        {
            إذا( xAutoReload != pdFALSE )
            {
                pxTimer->uc_حالت |= tmr_حالت_IS_AUTORELOAD;
            }
            وإلا
            {
                pxTimer->uc_حالت &= ( ( uint8_t ) ~tmr_حالت_IS_AUTORELOAD );
            }
        }
        taskEXIT_CRITICAL( &x_Timer_جندرا );
    }
/*-----------------------------------------------------------*/

    ادھار_Type_t x_Timer_Get_Reload_منوال( TimerHandle_t xTimer )
    {
        Timer_t * pxTimer = xTimer;
        ادھار_Type_t xReturn;

        configASSERT( xTimer );
        taskENTER_CRITICAL( &x_Timer_جندرا );
        {
            إذا( ( pxTimer->uc_حالت & tmr_حالت_IS_AUTORELOAD ) == 0 )
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
        taskEXIT_CRITICAL( &x_Timer_جندرا );

        أرجع xReturn;
    }

    UBaseType_t ux_Timer_Get_Reload_منوال( TimerHandle_t xTimer )
    {
        أرجع ( UBaseType_t ) x_Timer_Get_Reload_منوال( xTimer );
    }
/*-----------------------------------------------------------*/

    TickType_t x_Timer_Get_Expiry_ویلا( TimerHandle_t xTimer )
    {
        Timer_t * pxTimer = xTimer;
        TickType_t xReturn;

        configASSERT( xTimer );
        xReturn = list_GET_LIST_شے_قدر( &( pxTimer->x_Timer_List_شے ) );
        أرجع xReturn;
    }
/*-----------------------------------------------------------*/

    #إذا ( config_SUPPORT_STATIC_ونڈ == 1 )
        ادھار_Type_t x_Timer_Get_Static_بفر( TimerHandle_t xTimer,
                                          StaticTimer_t ** ppx_Timer_بفر )
        {
            ادھار_Type_t xReturn;
            Timer_t * pxTimer = xTimer;

            configASSERT( ppx_Timer_بفر != عدم );

            إذا( ( pxTimer->uc_حالت & tmr_حالت_IS_STATICALLY_ALLOCATED ) != 0 )
            {
                *ppx_Timer_بفر = ( StaticTimer_t * ) pxTimer;
                xReturn = pdTRUE;
            }
            وإلا
            {
                xReturn = pdFALSE;
            }

            أرجع xReturn;
        }
    #جے_انت /* configSUPPORT_STATIC_ALLOCATION */
/*-----------------------------------------------------------*/

    ثابت حرف * pcTimerGetName( TimerHandle_t xTimer ) /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
    {
        Timer_t * pxTimer = xTimer;

        configASSERT( xTimer );
        أرجع pxTimer->pcTimerName;
    }
/*-----------------------------------------------------------*/

    ساكن فراغ prvReloadTimer( Timer_t * ثابت pxTimer,
                                TickType_t x_Expired_ویلا,
                                ثابت TickType_t x_ویلا_Now )
    {
        /* Insert the timer into the appropriate list for the next expiry time.
         * If the next expiry time has already passed, advance the expiry time,
         * call the callback function, and try again. */
        طالما( prv_داخل_کرو_Timer_وچ_سرگرم_List( pxTimer, ( x_Expired_ویلا + pxTimer->x_Timer_Period_وچ_Ticks ), x_ویلا_Now, x_Expired_ویلا ) != pdFALSE )
        {
            /* Advance the expiry time. */
            x_Expired_ویلا += pxTimer->x_Timer_Period_وچ_Ticks;

            /* Call the timer callback. */
            traceTIMER_EXPIRED( pxTimer );
            pxTimer->pxCallbackFunction( ( TimerHandle_t ) pxTimer );
        }
    }
/*-----------------------------------------------------------*/

    ساكن فراغ prvProcessExpiredTimer( ثابت TickType_t x_اگلا_Expire_ویلا,
                                        ثابت TickType_t x_ویلا_Now )
    {
        Timer_t * ثابت pxTimer = ( Timer_t * ) list_GET_OWNER_دا_سر_اندراج( pxCurrentTimerList ); /*lint !e9087 !e9079 void * is used as this macro is used with tasks and co-routines too.  Alignment is known to be fine as the type of the pointer stored and retrieved is the same. */

        /* Remove the timer from the list of active timers.  A check has already
         * been performed to ensure the list is not empty. */

        ( فراغ ) uxListRemove( &( pxTimer->x_Timer_List_شے ) );

        /* If the timer is an auto-reload timer then calculate the next
         * expiry time and re-insert the timer in the list of active timers. */
        إذا( ( pxTimer->uc_حالت & tmr_حالت_IS_AUTORELOAD ) != 0 )
        {
            prvReloadTimer( pxTimer, x_اگلا_Expire_ویلا, x_ویلا_Now );
        }
        وإلا
        {
            pxTimer->uc_حالت &= ( ( uint8_t ) ~tmr_حالت_IS_سرگرم );
        }

        /* Call the timer callback. */
        traceTIMER_EXPIRED( pxTimer );
        pxTimer->pxCallbackFunction( ( TimerHandle_t ) pxTimer );
    }
/*-----------------------------------------------------------*/

    ساكن پورٹ_TASK_FUNCTION( prvTimerTask, pvParameters )
    {
        TickType_t x_اگلا_Expire_ویلا;
        ادھار_Type_t x_List_Was_خالی_تھاواں;

        /* Just to avoid compiler warnings. */
        ( فراغ ) pvParameters;

        #إذا ( configUSE_DAEMON_TASK_STARTUP_HOOK == 1 )
        {
            خارجي فراغ vApplicationDaemonTaskStartupHook( فراغ );

            /* Allow the application writer to execute some code in the context of
             * this task at the point the task starts executing.  This is useful if the
             * application includes initialisation code that would benefit from
             * executing after the scheduler has been started. */
            vApplicationDaemonTaskStartupHook();
        }
        #جے_انت /* configUSE_DAEMON_TASK_STARTUP_HOOK */

        لكل( ; ; )
        {
            /* Query the timers list to see if it contains any timers, and if so,
             * obtain the time at which the next timer will expire. */
            x_اگلا_Expire_ویلا = prv_Get_اگلا_Expire_ویلا( &x_List_Was_خالی_تھاواں );

            /* If a timer has expired, process it.  Otherwise, block this task
             * until either a timer does expire, or a command is received. */
            prv_Process_Timer_Or_بلاک_Task( x_اگلا_Expire_ویلا, x_List_Was_خالی_تھاواں );

            /* Empty the command queue. */
            prvProcessReceivedCommands();
        }
    }
/*-----------------------------------------------------------*/

    ساكن فراغ prv_Process_Timer_Or_بلاک_Task( ثابت TickType_t x_اگلا_Expire_ویلا,
                                            ادھار_Type_t x_List_Was_خالی_تھاواں )
    {
        TickType_t x_ویلا_Now;
        ادھار_Type_t xTimerListsWereSwitched;

        prvENTER_CRITICAL_OR_SUSPEND_ALL( &x_Timer_جندرا );
        {
            /* Obtain the time now to make an assessment as to whether the timer
             * has expired or not.  If obtaining the time causes the lists to switch
             * then don't process this timer as any timers that remained in the list
             * when the lists were switched will have been processed within the
             * prvSampleTimeNow() function. */
            x_ویلا_Now = prv_Sample_ویلا_Now( &xTimerListsWereSwitched );

            إذا( xTimerListsWereSwitched == pdFALSE )
            {
                /* The tick count has not overflowed, has the timer expired? */
                إذا( ( x_List_Was_خالی_تھاواں == pdFALSE ) && ( x_اگلا_Expire_ویلا <= x_ویلا_Now ) )
                {
                    ( فراغ ) prvEXIT_CRITICAL_OR_RESUME_ALL( &x_Timer_جندرا );
                    prvProcessExpiredTimer( x_اگلا_Expire_ویلا, x_ویلا_Now );
                }
                وإلا
                {
                    /* The tick count has not overflowed, and the next expire
                     * time has not been reached yet.  This task should therefore
                     * block to wait for the next expire time or a command to be
                     * received - whichever comes first.  The following line cannot
                     * be reached unless xNextExpireTime > xTimeNow, except in the
                     * case when the current timer list is empty. */
                    إذا( x_List_Was_خالی_تھاواں != pdFALSE )
                    {
                        /* The current timer list is empty - is the overflow list
                         * also empty? */
                        x_List_Was_خالی_تھاواں = list_LIST_IS_خالی_تھاواں( px_حد_توں_ودھ_Timer_List );
                    }

                    v_قطار_Wait_For_سنیہا_Restricted( x_Timer_قطار, ( x_اگلا_Expire_ویلا - x_ویلا_Now ), x_List_Was_خالی_تھاواں );

                    إذا( prvEXIT_CRITICAL_OR_RESUME_ALL( &x_Timer_جندرا ) == pdFALSE )
                    {
                        /* Yield to wait for either a command to arrive, or the
                         * block time to expire.  If a command arrived between the
                         * critical section being exited and this yield then the yield
                         * will not cause the task to block. */
                        پورٹ_YIELD_WITHIN_API();
                    }
                    وإلا
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
            }
            وإلا
            {
                ( فراغ ) prvEXIT_CRITICAL_OR_RESUME_ALL( &x_Timer_جندرا );
            }
        }
    }
/*-----------------------------------------------------------*/

    ساكن TickType_t prv_Get_اگلا_Expire_ویلا( ادھار_Type_t * ثابت px_List_Was_خالی_تھاواں )
    {
        TickType_t x_اگلا_Expire_ویلا;

        /* Timers are listed in expiry time order, with the head of the list
         * referencing the task that will expire first.  Obtain the time at which
         * the timer with the nearest expiry time will expire.  If there are no
         * active timers then just set the next expire time to 0.  That will cause
         * this task to unblock when the tick count overflows, at which point the
         * timer lists will be switched and the next expiry time can be
         * re-assessed.  */
        *px_List_Was_خالی_تھاواں = list_LIST_IS_خالی_تھاواں( pxCurrentTimerList );

        إذا( *px_List_Was_خالی_تھاواں == pdFALSE )
        {
            x_اگلا_Expire_ویلا = list_GET_شے_قدر_دا_سر_اندراج( pxCurrentTimerList );
        }
        وإلا
        {
            /* Ensure the task unblocks when the tick count rolls over. */
            x_اگلا_Expire_ویلا = ( TickType_t ) 0U;
        }

        أرجع x_اگلا_Expire_ویلا;
    }
/*-----------------------------------------------------------*/

    ساكن TickType_t prv_Sample_ویلا_Now( ادھار_Type_t * ثابت pxTimerListsWereSwitched )
    {
        TickType_t x_ویلا_Now;
        PRIVILEGED_انکڑے ساكن TickType_t x_آخری_ویلا = ( TickType_t ) 0U; /*lint !e956 Variable is only accessible to one task. */

        x_ویلا_Now = x_Task_Get_Tick_گنتی();

        إذا( x_ویلا_Now < x_آخری_ویلا )
        {
            prvSwitchTimerLists();
            *pxTimerListsWereSwitched = pdTRUE;
        }
        وإلا
        {
            *pxTimerListsWereSwitched = pdFALSE;
        }

        x_آخری_ویلا = x_ویلا_Now;

        أرجع x_ویلا_Now;
    }
/*-----------------------------------------------------------*/

    ساكن ادھار_Type_t prv_داخل_کرو_Timer_وچ_سرگرم_List( Timer_t * ثابت pxTimer,
                                                  ثابت TickType_t x_اگلا_Expiry_ویلا,
                                                  ثابت TickType_t x_ویلا_Now,
                                                  ثابت TickType_t x_Command_ویلا )
    {
        ادھار_Type_t xProcessTimerNow = pdFALSE;

        list_سیٹ_LIST_شے_قدر( &( pxTimer->x_Timer_List_شے ), x_اگلا_Expiry_ویلا );
        list_سیٹ_LIST_شے_OWNER( &( pxTimer->x_Timer_List_شے ), pxTimer );

        إذا( x_اگلا_Expiry_ویلا <= x_ویلا_Now )
        {
            /* Has the expiry time elapsed between the command to start/reset a
             * timer was issued, and the time the command was processed? */
            إذا( ( ( TickType_t ) ( x_ویلا_Now - x_Command_ویلا ) ) >= pxTimer->x_Timer_Period_وچ_Ticks ) /*lint !e961 MISRA exception as the casts are only redundant for some ports. */
            {
                /* The time between a command being issued and the command being
                 * processed actually exceeds the timers period.  */
                xProcessTimerNow = pdTRUE;
            }
            وإلا
            {
                v_List_داخل_کرو( px_حد_توں_ودھ_Timer_List, &( pxTimer->x_Timer_List_شے ) );
            }
        }
        وإلا
        {
            إذا( ( x_ویلا_Now < x_Command_ویلا ) && ( x_اگلا_Expiry_ویلا >= x_Command_ویلا ) )
            {
                /* If, since the command was issued, the tick count has overflowed
                 * but the expiry time has not, then the timer must have already passed
                 * its expiry time and should be processed immediately. */
                xProcessTimerNow = pdTRUE;
            }
            وإلا
            {
                v_List_داخل_کرو( pxCurrentTimerList, &( pxTimer->x_Timer_List_شے ) );
            }
        }

        أرجع xProcessTimerNow;
    }
/*-----------------------------------------------------------*/

    ساكن فراغ prvProcessReceivedCommands( فراغ )
    {
        Daemon_Task_سنیہا_t x_سنیہا;
        Timer_t * pxTimer;
        ادھار_Type_t xTimerListsWereSwitched;
        TickType_t x_ویلا_Now;

        طالما( x_قطار_Receive( x_Timer_قطار, &x_سنیہا, tmrNO_DELAY ) != pdFAIL ) /*lint !e603 xMessage does not have to be initialised as it is passed out, not in, and it is not used unless xQueueReceive() returns pdTRUE. */
        {
            #إذا ( INCLUDE_xTimerPendFunctionCall == 1 )
            {
                /* Negative commands are pended function calls rather than timer
                 * commands. */
                إذا( x_سنیہا.x_سنیہا_ID < ( ادھار_Type_t ) 0 )
                {
                    ثابت CallbackParameters_t * ثابت pxCallback = &( x_سنیہا.u.xCallbackParameters );

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
            #جے_انت /* INCLUDE_xTimerPendFunctionCall */

            /* Commands that are positive are timer commands rather than pended
             * function calls. */
            إذا( x_سنیہا.x_سنیہا_ID >= ( ادھار_Type_t ) 0 )
            {
                /* The messages uses the xTimerParameters member to work on a
                 * software timer. */
                pxTimer = x_سنیہا.u.xTimerParameters.pxTimer;

                إذا( listIS_CONTAINED_WITHIN( عدم, &( pxTimer->x_Timer_List_شے ) ) == pdFALSE ) /*lint !e961. The cast is only redundant when NULL is passed into the macro. */
                {
                    /* The timer is in a list, remove it. */
                    ( فراغ ) uxListRemove( &( pxTimer->x_Timer_List_شے ) );
                }
                وإلا
                {
                    mtCOVERAGE_TEST_MARKER();
                }

                traceTIMER_COMMAND_RECEIVED( pxTimer, x_سنیہا.x_سنیہا_ID, x_سنیہا.u.xTimerParameters.x_سنیہا_قدر );

                /* In this case the xTimerListsWereSwitched parameter is not used, but
                 *  it must be present in the function call.  prvSampleTimeNow() must be
                 *  called after the message is received from xTimerQueue so there is no
                 *  possibility of a higher priority task adding a message to the message
                 *  queue with a time that is ahead of the timer daemon task (because it
                 *  pre-empted the timer daemon task after the xTimeNow value was set). */
                x_ویلا_Now = prv_Sample_ویلا_Now( &xTimerListsWereSwitched );

                بدل( x_سنیہا.x_سنیہا_ID )
                {
                    حالة tmr_COMMAND_شروع:
                    حالة tmr_COMMAND_شروع_FROM_ISR:
                    حالة tmrCOMMAND_RESET:
                    حالة tmrCOMMAND_RESET_FROM_ISR:
                        /* Start or restart a timer. */
                        pxTimer->uc_حالت |= tmr_حالت_IS_سرگرم;

                        إذا( prv_داخل_کرو_Timer_وچ_سرگرم_List( pxTimer, x_سنیہا.u.xTimerParameters.x_سنیہا_قدر + pxTimer->x_Timer_Period_وچ_Ticks, x_ویلا_Now, x_سنیہا.u.xTimerParameters.x_سنیہا_قدر ) != pdFALSE )
                        {
                            /* The timer expired before it was added to the active
                             * timer list.  Process it now. */
                            إذا( ( pxTimer->uc_حالت & tmr_حالت_IS_AUTORELOAD ) != 0 )
                            {
                                prvReloadTimer( pxTimer, x_سنیہا.u.xTimerParameters.x_سنیہا_قدر + pxTimer->x_Timer_Period_وچ_Ticks, x_ویلا_Now );
                            }
                            وإلا
                            {
                                pxTimer->uc_حالت &= ( ( uint8_t ) ~tmr_حالت_IS_سرگرم );
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
                        pxTimer->uc_حالت &= ( ( uint8_t ) ~tmr_حالت_IS_سرگرم );
                        اكسر;

                    حالة tmrCOMMAND_CHANGE_PERIOD:
                    حالة tmrCOMMAND_CHANGE_PERIOD_FROM_ISR:
                        pxTimer->uc_حالت |= tmr_حالت_IS_سرگرم;
                        pxTimer->x_Timer_Period_وچ_Ticks = x_سنیہا.u.xTimerParameters.x_سنیہا_قدر;
                        configASSERT( ( pxTimer->x_Timer_Period_وچ_Ticks > 0 ) );

                        /* The new period does not really have a reference, and can
                         * be longer or shorter than the old one.  The command time is
                         * therefore set to the current time, and as the period cannot
                         * be zero the next expiry time can only be in the future,
                         * meaning (unlike for the xTimerStart() case above) there is
                         * no fail case that needs to be handled here. */
                        ( فراغ ) prv_داخل_کرو_Timer_وچ_سرگرم_List( pxTimer, ( x_ویلا_Now + pxTimer->x_Timer_Period_وچ_Ticks ), x_ویلا_Now, x_ویلا_Now );
                        اكسر;

                    حالة tmrCOMMAND_DELETE:
                        #إذا ( config_SUPPORT_DYNAMIC_ونڈ == 1 )
                        {
                            /* The timer has already been removed from the active list,
                             * just free up the memory if the memory was dynamically
                             * allocated. */
                            إذا( ( pxTimer->uc_حالت & tmr_حالت_IS_STATICALLY_ALLOCATED ) == ( uint8_t ) 0 )
                            {
                                v_پورٹ_Free( pxTimer );
                            }
                            وإلا
                            {
                                pxTimer->uc_حالت &= ( ( uint8_t ) ~tmr_حالت_IS_سرگرم );
                            }
                        }
                        #وإلا /* if ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) */
                        {
                            /* If dynamic allocation is not enabled, the memory
                             * could not have been dynamically allocated. So there is
                             * no need to free the memory - just mark the timer as
                             * "not active". */
                            pxTimer->uc_حالت &= ( ( uint8_t ) ~tmr_حالت_IS_سرگرم );
                        }
                        #جے_انت /* configSUPPORT_DYNAMIC_ALLOCATION */
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
        TickType_t x_اگلا_Expire_ویلا;
        List_t * px_عارضی;

        /* The tick count has overflowed.  The timer lists must be switched.
         * If there are any timers still referenced from the current timer list
         * then they must have expired and should be processed before the lists
         * are switched. */
        طالما( list_LIST_IS_خالی_تھاواں( pxCurrentTimerList ) == pdFALSE )
        {
            x_اگلا_Expire_ویلا = list_GET_شے_قدر_دا_سر_اندراج( pxCurrentTimerList );

            /* Process the expired timer.  For auto-reload timers, be careful to
             * process only expirations that occur on the current list.  Further
             * expirations must wait until after the lists are switched. */
            prvProcessExpiredTimer( x_اگلا_Expire_ویلا, tmr_MAX_ویلا_BEFORE_حد_توں_ودھ );
        }

        px_عارضی = pxCurrentTimerList;
        pxCurrentTimerList = px_حد_توں_ودھ_Timer_List;
        px_حد_توں_ودھ_Timer_List = px_عارضی;
    }
/*-----------------------------------------------------------*/

    ساكن فراغ prv_Check_For_Valid_List_And_قطار( فراغ )
    {
        /* Check that the list from which active timers are referenced, and the
         * queue used to communicate with the timer service, have been
         * initialised. */
        taskENTER_CRITICAL( &x_Timer_جندرا );
        {
            إذا( x_Timer_قطار == عدم )
            {
                vListInitialise( &x_سرگرم_Timer_List1 );
                vListInitialise( &x_سرگرم_Timer_List2 );
                pxCurrentTimerList = &x_سرگرم_Timer_List1;
                px_حد_توں_ودھ_Timer_List = &x_سرگرم_Timer_List2;

                #إذا ( config_SUPPORT_STATIC_ونڈ == 1 )
                {
                    /* The timer queue is allocated statically in case
                     * configSUPPORT_DYNAMIC_ALLOCATION is 0. */
                    PRIVILEGED_انکڑے ساكن Static_قطار_t x_Static_Timer_قطار;                                                                          /*lint !e956 Ok to declare in this manner to prevent additional conditional compilation guards in other locations. */
                    PRIVILEGED_انکڑے ساكن uint8_t uc_Static_Timer_قطار_Storage[ ( size_t ) config_TIMER_قطار_لمائی * حجم( Daemon_Task_سنیہا_t ) ]; /*lint !e956 Ok to declare in this manner to prevent additional conditional compilation guards in other locations. */

                    x_Timer_قطار = x_قطار_Create_Static( ( UBaseType_t ) config_TIMER_قطار_لمائی, ( UBaseType_t ) حجم( Daemon_Task_سنیہا_t ), &( uc_Static_Timer_قطار_Storage[ 0 ] ), &x_Static_Timer_قطار );
                }
                #وإلا
                {
                    x_Timer_قطار = x_قطار_Create( ( UBaseType_t ) config_TIMER_قطار_لمائی, حجم( Daemon_Task_سنیہا_t ) );
                }
                #جے_انت /* if ( configSUPPORT_STATIC_ALLOCATION == 1 ) */

                #إذا ( config_قطار_REGISTRY_ناپ > 0 )
                {
                    إذا( x_Timer_قطار != عدم )
                    {
                        v_قطار_جوڑو_منزل_Registry( x_Timer_قطار, "TmrQ" );
                    }
                    وإلا
                    {
                        mtCOVERAGE_TEST_MARKER();
                    }
                }
                #جے_انت /* configQUEUE_REGISTRY_SIZE */
            }
            وإلا
            {
                mtCOVERAGE_TEST_MARKER();
            }
        }
        taskEXIT_CRITICAL( &x_Timer_جندرا );
    }
/*-----------------------------------------------------------*/

    ادھار_Type_t x_Timer_Is_Timer_سرگرم( TimerHandle_t xTimer )
    {
        ادھار_Type_t xReturn;
        Timer_t * pxTimer = xTimer;

        configASSERT( xTimer );

        /* Is the timer in the list of active timers? */
        taskENTER_CRITICAL( &x_Timer_جندرا );
        {
            إذا( ( pxTimer->uc_حالت & tmr_حالت_IS_سرگرم ) == 0 )
            {
                xReturn = pdFALSE;
            }
            وإلا
            {
                xReturn = pdTRUE;
            }
        }
        taskEXIT_CRITICAL( &x_Timer_جندرا );

        أرجع xReturn;
    } /*lint !e818 Can't be pointer to const due to the typedef. */
/*-----------------------------------------------------------*/

    فراغ * pvTimerGetTimerID( ثابت TimerHandle_t xTimer )
    {
        Timer_t * ثابت pxTimer = xTimer;
        فراغ * pvReturn;

        configASSERT( xTimer );

        taskENTER_CRITICAL( &x_Timer_جندرا );
        {
            pvReturn = pxTimer->pvTimerID;
        }
        taskEXIT_CRITICAL( &x_Timer_جندرا );

        أرجع pvReturn;
    }
/*-----------------------------------------------------------*/

    فراغ v_Timer_سیٹ_Timer_ID( TimerHandle_t xTimer,
                           فراغ * pvNewID )
    {
        Timer_t * ثابت pxTimer = xTimer;

        configASSERT( xTimer );

        taskENTER_CRITICAL( &x_Timer_جندرا );
        {
            pxTimer->pvTimerID = pvNewID;
        }
        taskEXIT_CRITICAL( &x_Timer_جندرا );
    }
/*-----------------------------------------------------------*/

    #إذا ( INCLUDE_xTimerPendFunctionCall == 1 )

        ادھار_Type_t xTimerPendFunctionCallFromISR( PendedFunction_t x_Function_منزل_Pend,
                                                  فراغ * pvParameter1,
                                                  uint32_t ulParameter2,
                                                  ادھار_Type_t * px_Higher_ترجیح_Task_Woken )
        {
            Daemon_Task_سنیہا_t x_سنیہا;
            ادھار_Type_t xReturn;

            /* Complete the message with the function parameters and post it to the
             * daemon task. */
            x_سنیہا.x_سنیہا_ID = tmrCOMMAND_EXECUTE_CALLBACK_FROM_ISR;
            x_سنیہا.u.xCallbackParameters.pxCallbackFunction = x_Function_منزل_Pend;
            x_سنیہا.u.xCallbackParameters.pvParameter1 = pvParameter1;
            x_سنیہا.u.xCallbackParameters.ulParameter2 = ulParameter2;

            xReturn = x_قطار_Send_From_ISR( x_Timer_قطار, &x_سنیہا, px_Higher_ترجیح_Task_Woken );

            tracePEND_FUNC_CALL_FROM_ISR( x_Function_منزل_Pend, pvParameter1, ulParameter2, xReturn );

            أرجع xReturn;
        }

    #جے_انت /* INCLUDE_xTimerPendFunctionCall */
/*-----------------------------------------------------------*/

    #إذا ( INCLUDE_xTimerPendFunctionCall == 1 )

        ادھار_Type_t xTimerPendFunctionCall( PendedFunction_t x_Function_منزل_Pend,
                                           فراغ * pvParameter1,
                                           uint32_t ulParameter2,
                                           TickType_t x_Ticks_منزل_Wait )
        {
            Daemon_Task_سنیہا_t x_سنیہا;
            ادھار_Type_t xReturn;

            /* This function can only be called after a timer has been created or
             * after the scheduler has been started because, until then, the timer
             * queue does not exist. */
            configASSERT( x_Timer_قطار );

            /* Complete the message with the function parameters and post it to the
             * daemon task. */
            x_سنیہا.x_سنیہا_ID = tmrCOMMAND_EXECUTE_CALLBACK;
            x_سنیہا.u.xCallbackParameters.pxCallbackFunction = x_Function_منزل_Pend;
            x_سنیہا.u.xCallbackParameters.pvParameter1 = pvParameter1;
            x_سنیہا.u.xCallbackParameters.ulParameter2 = ulParameter2;

            xReturn = x_قطار_Send_منزل_پٹھ( x_Timer_قطار, &x_سنیہا, x_Ticks_منزل_Wait );

            tracePEND_FUNC_CALL( x_Function_منزل_Pend, pvParameter1, ulParameter2, xReturn );

            أرجع xReturn;
        }

    #جے_انت /* INCLUDE_xTimerPendFunctionCall */
/*-----------------------------------------------------------*/

    #إذا ( configUSE_TRACE_FACILITY == 1 )

        UBaseType_t ux_Timer_Get_Timer_نمبر( TimerHandle_t xTimer )
        {
            أرجع ( ( Timer_t * ) xTimer )->ux_Timer_نمبر;
        }

    #جے_انت /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

    #إذا ( configUSE_TRACE_FACILITY == 1 )

        فراغ v_Timer_سیٹ_Timer_نمبر( TimerHandle_t xTimer,
                                   UBaseType_t ux_Timer_نمبر )
        {
            ( ( Timer_t * ) xTimer )->ux_Timer_نمبر = ux_Timer_نمبر;
        }

    #جے_انت /* configUSE_TRACE_FACILITY */
/*-----------------------------------------------------------*/

/* This entire source file will be skipped if the application is not configured
 * to include software timer functionality.  If you want to include software timer
 * functionality then ensure configUSE_TIMERS is set to 1 in FreeRTOSConfig.h. */
#جے_انت /* configUSE_TIMERS == 1 */
