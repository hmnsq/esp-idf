/*
 * SPDX-FileCopyrightText: 2020 Amazon.com, Inc. or its affiliates
 *
 * SPDX-License-Identifier: MIT
 */
/*
 * FreeRTOS Kernel V10.4.3
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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

/*-----------------------------------------------------------
 * Implementation of functions defined in portable.h for the Posix port.
 *
 * Each task has a pthread which eases use of standard debuggers
 * (allowing backtraces of tasks etc). Threads for tasks that are not
 * running are blocked in sigwait().
 *
 * Task switch is done by resuming the thread for the next task by
 * signaling the condition variable and then waiting on a condition variable
 * with the current thread.
 *
 * The timer interrupt uses SIGALRM and care is taken to ensure that
 * the signal handler runs only on the thread for the current task.
 *
 * Use of part of the standard C library requires care as some
 * functions can take pthread mutexes internally which can result in
 * deadlocks as the FreeRTOS kernel can switch tasks while they're
 * holding a pthread mutex.
 *
 * stdio (printf() and friends) should be called from a single task
 * only or serialized with a FreeRTOS primitive such as a binary
 * semaphore or mutex.
 *----------------------------------------------------------*/

#अन्तर्भाव <errno.h>
#अन्तर्भाव <pthread.h>
#अन्तर्भाव <signal.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <sys/time.h>
#अन्तर्भाव <sys/times.h>
#अन्तर्भाव <time.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <assert.h>

/* Scheduler includes. */
#अन्तर्भाव "esp_heap_caps.h"
#अन्तर्भाव "FreeRTOS.h"
#अन्तर्भाव "task.h"
#अन्तर्भाव "esp_task.h"
#अन्तर्भाव "timers.h"
#अन्तर्भाव "utils/wait_for_event.h"
#अन्तर्भाव "esp_log.h"
/*-----------------------------------------------------------*/

#define SIG_RESUME SIGUSR1

प्रकारनिर्देश संरचना THREAD
{
    pthread_t pthread;
    TaskFunction_t pxCode;
    शून्य *pvParams;
    BaseType_t xDying;
    संरचना event *ev;
} Thread_t;

/*
 * The additional per-thread data is stored at the beginning of the
 * task's stack.
 */
स्थिर अन्तःस्थ Thread_t *prvGetThreadFromTask(TaskHandle_t xTask)
{
    चळत_Type_t *px_शीर्ष_Of_चळत = *(चळत_Type_t **)xTask;

    प्रत्यागम (Thread_t *)(px_शीर्ष_Of_चळत + 1);
}

/*-----------------------------------------------------------*/

स्थिर pthread_once_t hSigSetupThread = PTHREAD_ONCE_INIT;
स्थिर sigset_t xAllSignals;
स्थिर sigset_t xSchedulerOriginalSignalMask;
स्थिर pthread_t hMainThread = ( pthread_t )रिक्त;

// These are saved as part of a thread's state in prvSwitchThread()
स्थिर चञ्चल BaseType_t uxCriticalNestingIDF = 0;    /* Track nesting calls for IDF style critical sections. FreeRTOS critical section nesting is maintained in the TCB. */
स्थिर चञ्चल UBaseType_t uxInterruptNesting = 0;     /* Tracks if we are currently in an interrupt. */
स्थिर चञ्चल BaseType_t uxInterruptLevel = 0;        /* Tracks the current level (i.e., interrupt mask) */
/*-----------------------------------------------------------*/

स्थिर BaseType_t xSchedulerEnd = pdFALSE;
/*-----------------------------------------------------------*/

स्थिर शून्य prvSetupSignalsAndSchedulerPolicy( शून्य );
स्थिर शून्य prvSetupTimerInterrupt( शून्य );
स्थिर शून्य *prvWaitForStart( शून्य * pvParams );
स्थिर शून्य prvSwitchThread( Thread_t * xThreadToResume,
                             Thread_t *xThreadToSuspend );
स्थिर शून्य prvSuspendSelf( Thread_t * thread);
स्थिर शून्य prvResumeThread( Thread_t * xThreadId );
स्थिर शून्य vPortSystemTickHandler( पूर्णाङ्क sig );
स्थिर शून्य vPortStartFirstTask( शून्य );
/*-----------------------------------------------------------*/

स्थिर शून्य prvFatalError( ध्रुव वर्ण *pcCall, पूर्णाङ्क iErrno )
{
    fprintf( stderr, "%s: %s\n", pcCall, strerror( iErrno ) );
    abort();
}

/*
 * See header file for description.
 */
चळत_Type_t *px_Port_Initialise_चळत( चळत_Type_t *px_शीर्ष_Of_चळत,
                                    चळत_Type_t *px_End_Of_चळत,
                                    TaskFunction_t pxCode,
                                    शून्य *pvParameters )
{
    Thread_t *thread;
    pthread_attr_t xThreadAttributes;
    size_t ul_चळत_Size;
    पूर्णाङ्क iRet;

    (शून्य)pthread_once( &hSigSetupThread, prvSetupSignalsAndSchedulerPolicy );

    /*
     * Store the additional thread data at the start of the stack.
     */
    thread = (Thread_t *)(px_शीर्ष_Of_चळत + 1) - 1;
    px_शीर्ष_Of_चळत = (चळत_Type_t *)thread - 1;
    ul_चळत_Size = (px_शीर्ष_Of_चळत + 1 - px_End_Of_चळत) * परिमाण(*px_शीर्ष_Of_चळत);

    thread->pxCode = pxCode;
    thread->pvParams = pvParameters;
    thread->xDying = pdFALSE;

    pthread_attr_init( &xThreadAttributes );
    pthread_attr_setstack( &xThreadAttributes, px_End_Of_चळत, ul_चळत_Size );

    thread->ev = event_create();

    BaseType_t prev_intr_level = xPortSetInterruptMask();

    iRet = pthread_create( &thread->pthread, &xThreadAttributes,
                           prvWaitForStart, thread );
    यदि ( iRet )
    {
        prvFatalError( "pthread_create", iRet );
    }

    vPortClearInterruptMask( prev_intr_level );

    प्रत्यागम px_शीर्ष_Of_चळत;
}
/*-----------------------------------------------------------*/

शून्य vPortStartFirstTask( शून्य )
{
    Thread_t *pxFirstThread = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    /* Start the first task. */
    prvResumeThread( pxFirstThread );
}
/*-----------------------------------------------------------*/

/*
 * See header file for description.
 */
BaseType_t xPortStartScheduler( शून्य )
{
    पूर्णाङ्क iSignal;
    sigset_t xSignals;

    hMainThread = pthread_self();

    /* Start the timer that generates the tick ISR(SIGALRM).
       Interrupts are disabled here already. */
    prvSetupTimerInterrupt();

    /* Start the first task. */
    vPortStartFirstTask();

    /* Wait until signaled by vPortEndScheduler(). */
    sigemptyset( &xSignals );
    sigaddset( &xSignals, SIG_RESUME );

    यावत् ( !xSchedulerEnd )
    {
        sigwait( &xSignals, &iSignal );
    }

    /* Cancel the Idle task and free its resources */
#यदि ( INCLUDE_xTaskGetIdleTaskHandle == 1 )
    vPortCancelThread( xTaskGetIdleTaskHandle() );
#endif

#यदि ( configUSE_TIMERS == 1 )
    /* Cancel the Timer task and free its resources */
    vPortCancelThread( xTimerGetTimerDaemonTaskHandle() );
#endif /* configUSE_TIMERS */

    /* Restore original signal mask. */
    (शून्य)pthread_sigmask( SIG_SETMASK, &xSchedulerOriginalSignalMask,  रिक्त );

    प्रत्यागम 0;
}
/*-----------------------------------------------------------*/

शून्य vPortEndScheduler( शून्य )
{
    संरचना itimerval itimer;
    संरचना sigaction sigtick;
    Thread_t *xCurrentThread;

    /* Stop the timer and ignore any pending SIGALRMs that would end
     * up running on the main thread when it is resumed. */
    itimer.it_value.tv_sec = 0;
    itimer.it_value.tv_usec = 0;

    itimer.it_interval.tv_sec = 0;
    itimer.it_interval.tv_usec = 0;
    (शून्य)setitimer( ITIMER_REAL, &itimer, रिक्त );

    sigtick.sa_flags = 0;
    sigtick.sa_handler = SIG_IGN;
    sigemptyset( &sigtick.sa_mask );
    sigaction( SIGALRM, &sigtick, रिक्त );

    /* Signal the scheduler to exit its loop. */
    xSchedulerEnd = pdTRUE;
    (शून्य)pthread_kill( hMainThread, SIG_RESUME );

    xCurrentThread = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );
    prvSuspendSelf(xCurrentThread);
}
/*-----------------------------------------------------------*/

स्थिर शून्य vPortDisableInterrupts( शून्य )
{
    pthread_sigmask( SIG_BLOCK, &xAllSignals, रिक्त );
}
/*-----------------------------------------------------------*/

स्थिर शून्य vPortEnableInterrupts( शून्य )
{
    pthread_sigmask( SIG_UNBLOCK, &xAllSignals, रिक्त );
}
/*-----------------------------------------------------------*/

शून्य vPortEnterCriticalIDF( शून्य )
{
    यदि ( uxCriticalNestingIDF == 0 && uxInterruptLevel == 0)
    {
        vPortDisableInterrupts();
    }
    uxCriticalNestingIDF++;
}
/*-----------------------------------------------------------*/

शून्य vPortExitCriticalIDF( शून्य )
{
    uxCriticalNestingIDF--;

    /* If we have reached 0 then re-enable the interrupts. */
    यदि( uxCriticalNestingIDF == 0 && uxInterruptLevel == 0)
    {
        vPortEnableInterrupts();
    }
}
/*-----------------------------------------------------------*/

शून्य vPortYieldFromISR( शून्य )
{
    Thread_t *xThreadToSuspend;
    Thread_t *xThreadToResume;

    xThreadToSuspend = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    vTaskSwitchContext();

    xThreadToResume = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    prvSwitchThread( xThreadToResume, xThreadToSuspend );
}
/*-----------------------------------------------------------*/

शून्य vPortYield( शून्य )
{
    BaseType_t prev_intr_level = xPortSetInterruptMask();

    vPortYieldFromISR();

    vPortClearInterruptMask( prev_intr_level );
}
/*-----------------------------------------------------------*/

/* In SMP code, the disable/enable interrupt macros are calling the set/get interrupt mask functions below.
   Hence, we need to call vPortDisableInterrupts() and vPortEnableInterrupts(), otherwise interrupts
   are never disabled/enabled. */

BaseType_t xPortSetInterruptMask( शून्य )
{
    यदि (uxInterruptLevel == 0 && uxCriticalNestingIDF == 0) {
        vPortDisableInterrupts();
    }
    BaseType_t prev_intr_level = uxInterruptLevel;
    uxInterruptLevel++;
    प्रत्यागम prev_intr_level;
}
/*-----------------------------------------------------------*/

शून्य vPortClearInterruptMask( BaseType_t xMask )
{
    // Only re-enable interrupts if xMask is 0
    uxInterruptLevel = xMask;
    यदि (uxInterruptLevel == 0 && uxCriticalNestingIDF == 0) {
        vPortEnableInterrupts();
    }
}
/*-----------------------------------------------------------*/

स्थिर uint64_t prvGetTimeNs(शून्य)
{
    संरचना timespec t;

    clock_gettime(CLOCK_MONOTONIC, &t);

    प्रत्यागम t.tv_sec * 1000000000ull + t.tv_nsec;
}

स्थिर uint64_t prvStartTimeNs;
/* commented as part of the code below in vPortSystemTickHandler,
 * to adjust timing according to full demo requirements */
/* static uint64_t prvTickCount; */

/*
 * Setup the systick timer to generate the tick interrupts at the required
 * frequency.
 */
शून्य prvSetupTimerInterrupt( शून्य )
{
    संरचना itimerval itimer;
    पूर्णाङ्क iRet;

    /* Initialise the structure with the current timer information. */
    iRet = getitimer( ITIMER_REAL, &itimer );
    यदि ( iRet )
    {
        prvFatalError( "getitimer", errno );
    }

    /* Set the interval between timer events. */
    itimer.it_interval.tv_sec = 0;
    itimer.it_interval.tv_usec = portTICK_RATE_MICROSECONDS;

    /* Set the current count-down. */
    itimer.it_value.tv_sec = 0;
    itimer.it_value.tv_usec = portTICK_RATE_MICROSECONDS;

    /* Set-up the timer interrupt. */
    iRet = setitimer( ITIMER_REAL, &itimer, रिक्त );
    यदि ( iRet )
    {
        prvFatalError( "setitimer", errno );
    }

    prvStartTimeNs = prvGetTimeNs();
}
/*-----------------------------------------------------------*/

स्थिर शून्य vPortSystemTickHandler( पूर्णाङ्क sig )
{
    Thread_t *pxThreadToSuspend;
    Thread_t *pxThreadToResume;
    BaseType_t xSwitchRequired;
    /* uint64_t xExpectedTicks; */

    // Handling a timer signal, so we are currently in an interrupt.
    uxInterruptNesting++;

#यदि ( configUSE_PREEMPTION == 1 )
    pxThreadToSuspend = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );
#endif

    /* Tick Increment, accounting for any lost signals or drift in
     * the timer. */
/*
 *      Comment code to adjust timing according to full demo requirements
 *      xExpectedTicks = (prvGetTimeNs() - prvStartTimeNs)
 *        / (portTICK_RATE_MICROSECONDS * 1000);
 * do { */
    xSwitchRequired = xTaskIncrementTick();
/*        prvTickCount++;
 *    } while (prvTickCount < xExpectedTicks);
*/

#यदि ( configUSE_PREEMPTION == 1 )
    यदि (xSwitchRequired == pdTRUE) {
        /* Select Next Task. */
        vTaskSwitchContext();

        pxThreadToResume = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

        prvSwitchThread(pxThreadToResume, pxThreadToSuspend);
    }
#अन्यथा
    (शून्य)xSwitchRequired;
#endif

    // Returning from the timer signal handler, so we are exiting the interrupt.
    uxInterruptNesting--;
}
/*-----------------------------------------------------------*/

शून्य vPortThreadDying( शून्य *pxTaskToDelete, चञ्चल BaseType_t *pxPendYield )
{
    Thread_t *pxThread = prvGetThreadFromTask( pxTaskToDelete );

    pxThread->xDying = pdTRUE;
}

शून्य vPortCancelThread( शून्य *pxTaskToDelete )
{
    Thread_t *pxThreadToCancel = prvGetThreadFromTask( pxTaskToDelete );

    /*
     * The thread has already been suspended so it can be safely cancelled.
     */
    pthread_cancel( pxThreadToCancel->pthread );
    pthread_join( pxThreadToCancel->pthread, रिक्त );
    event_delete( pxThreadToCancel->ev );
}
/*-----------------------------------------------------------*/

स्थिर शून्य *prvWaitForStart( शून्य * pvParams )
{
    Thread_t *pxThread = pvParams;

    prvSuspendSelf(pxThread);

    /* Resumed for the first time, thus this thread didn't previously call
     * prvSwitchThread(). So we need to initialise the state variables for this
     * thread. */
    uxCriticalNestingIDF = 0;
    uxInterruptNesting = 0;
    uxInterruptLevel = 0;
    vPortEnableInterrupts();

    /* Call the task's entry point. */
    pxThread->pxCode( pxThread->pvParams );

    /* A function that implements a task must not exit or attempt to return to
    * its caller as there is nothing to return to. If a task wants to exit it
    * should instead call vTaskDelete( NULL ). Artificially force an assert()
    * to be triggered if configASSERT() is defined, so application writers can
        * catch the error. */
    configASSERT( pdFALSE );

    प्रत्यागम रिक्त;
}
/*-----------------------------------------------------------*/

स्थिर शून्य prvSwitchThread( Thread_t *pxThreadToResume,
                             Thread_t *pxThreadToSuspend )
{
    BaseType_t uxSavedCriticalNestingIDF;
    BaseType_t uxSavedInterruptNesting;
    BaseType_t uxSavedInterruptLevel;

    यदि ( pxThreadToSuspend != pxThreadToResume )
    {
        /* It is possible for prvSwitchThread() to be called...
         * - while inside an ISR (i.e., via vPortSystemTickHandler() or vPortYieldFromISR())
         * - while interrupts are disabled or in a critical section (i.e., via vPortYield())
         *
         * So we need to save the various count variables as part of the thread's context.
         * They are restored when the pthread switches back. */
        uxSavedCriticalNestingIDF = uxCriticalNestingIDF;
        uxSavedInterruptNesting = uxInterruptNesting;
        uxSavedInterruptLevel = uxInterruptLevel;

        prvResumeThread( pxThreadToResume );
        यदि ( pxThreadToSuspend->xDying )
        {
            pthread_exit( रिक्त );
        }
        prvSuspendSelf( pxThreadToSuspend );

        uxCriticalNestingIDF = uxSavedCriticalNestingIDF;
        uxInterruptNesting = uxSavedInterruptNesting;
        uxInterruptLevel = uxSavedInterruptLevel;
    }
}
/*-----------------------------------------------------------*/

स्थिर शून्य prvSuspendSelf( Thread_t *thread )
{
    /*
     * Suspend this thread by waiting for a pthread_cond_signal event.
     *
     * A suspended thread must not handle signals (interrupts) so
     * all signals must be blocked by calling this from:
     *
     * - Inside a critical section (vPortEnterCritical() /
     *   vPortExitCritical()).
     *
     * - From a signal handler that has all signals masked.
     *
     * - A thread with all signals blocked with pthread_sigmask().
        */
    event_wait(thread->ev);
}

/*-----------------------------------------------------------*/

स्थिर शून्य prvResumeThread( Thread_t *xThreadId )
{
    यदि ( pthread_self() != xThreadId->pthread )
    {
        event_signal(xThreadId->ev);
    }
}
/*-----------------------------------------------------------*/

स्थिर शून्य prvSetupSignalsAndSchedulerPolicy( शून्य )
{
    संरचना sigaction sigresume, sigtick;
    पूर्णाङ्क iRet;

    hMainThread = pthread_self();

    /* Initialise common signal masks. */
    sigfillset( &xAllSignals );
    /* Don't block SIGINT so this can be used to break into GDB while
     * in a critical section. */
    sigdelset( &xAllSignals, SIGINT );

    /*
     * Block all signals in this thread so all new threads
     * inherits this mask.
     *
     * When a thread is resumed for the first time, all signals
     * will be unblocked.
     */
    (शून्य)pthread_sigmask( SIG_SETMASK, &xAllSignals,
                           &xSchedulerOriginalSignalMask );

    /* SIG_RESUME is only used with sigwait() so doesn't need a
       handler. */
    sigresume.sa_flags = 0;
    sigresume.sa_handler = SIG_IGN;
    sigfillset( &sigresume.sa_mask );

    sigtick.sa_flags = 0;
    sigtick.sa_handler = vPortSystemTickHandler;
    sigfillset( &sigtick.sa_mask );

    iRet = sigaction( SIG_RESUME, &sigresume, रिक्त );
    यदि ( iRet )
    {
        prvFatalError( "sigaction", errno );
    }

    iRet = sigaction( SIGALRM, &sigtick, रिक्त );
    यदि ( iRet )
    {
        prvFatalError( "sigaction", errno );
    }
}
/*-----------------------------------------------------------*/

unsigned long ulPortGetRunTime( शून्य )
{
    संरचना tms xTimes;

    times( &xTimes );

    प्रत्यागम ( unsigned long ) xTimes.tms_utime;
}
/*-----------------------------------------------------------*/
bool portVALID_LIST_MEM(ध्रुव शून्य *ptr)
{
    प्रत्यागम true;
}

bool portVALID_TCB_MEM(ध्रुव शून्य *ptr)
{
    प्रत्यागम true;
}

bool port_VALID_चळत_MEM(ध्रुव शून्य *ptr)
{
    प्रत्यागम true;
}
/*-----------------------------------------------------------*/

portMUX_TYPE port_xTaskLock = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE port_xISRLock = portMUX_INITIALIZER_UNLOCKED;

स्थिर ध्रुव वर्ण *TAG = "port";

BaseType_t xPortCheckIfInISR(शून्य)
{
    प्रत्यागम (uxInterruptNesting == 0) ? pdFALSE : pdTRUE;
}

शून्य app_main(शून्य);

स्थिर शून्य main_task(शून्य* args)
{
    app_main();
    vTaskDelete(रिक्त);
}

पूर्णाङ्क main(पूर्णाङ्क argc, ध्रुव वर्ण **argv)
{
    // This makes sure that stdio is flushed after each '\n' so that idf.py monitor
    // reads the program output on time.
    setvbuf(stdout, रिक्त, _IOLBF, 0);

    usleep(1000);

    BaseType_t res;

#यदि ( config_अंक_CORES > 1 )
    res = xTaskCreateAffinitySet(&main_task, "main",
                                 ESP_TASK_MAIN_चळत, रिक्त,
                                 ESP_TASK_MAIN_PRIO, ESP_TASK_MAIN_CORE, रिक्त);
#अन्यथा
    res = xTaskCreate(&main_task, "main",
                      ESP_TASK_MAIN_चळत, रिक्त,
                      ESP_TASK_MAIN_PRIO, रिक्त);
#endif

    प्रतिज्ञा(res == pdTRUE);
    (शून्य)res;

    ESP_LOGI(TAG, "Starting SMP scheduler.");
    vTaskStartScheduler();

    // This line should never be reached
    प्रतिज्ञा(false);
}

शून्य esp_vApplicationIdleHook(शून्य)
{
    /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
     * to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
     * task.  It is essential that code added to this hook function never attempts
     * to block in any way (for example, call xQueueReceive() with a block time
     * specified, or call vTaskDelay()).  If application tasks make use of the
     * vTaskDelete() API function to delete themselves then it is also important
     * that vApplicationIdleHook() is permitted to return to its calling function,
     * because it is the responsibility of the idle task to clean up memory
     * allocated by the kernel to any task that has since deleted itself. */


    usleep( 15000 );
}

शून्य esp_vApplicationTickHook( शून्य ) { }

#यदि  (  configUSE_TICK_HOOK > 0 )
शून्य vApplicationTickHook( शून्य )
{
    esp_vApplicationTickHook();
}
#endif

#यदि ( configSUPPORT_STATIC_ALLOCATION == 1 )
/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
 * implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
 * used by the Idle task. */
शून्य vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                    चळत_Type_t ** ppx_Idle_Task_चळत_Buffer,
                                    uint32_t * pul_Idle_Task_चळत_Size )
{
/* If the buffers to be provided to the Idle task are declared inside this
 * function then they must be declared static - otherwise they will be allocated on
 * the stack and so not exists after this function exits. */
    स्थिर StaticTask_t xIdleTaskTCB;
    स्थिर चळत_Type_t ux_Idle_Task_चळत[ config_MINIMAL_चळत_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
     * state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppx_Idle_Task_चळत_Buffer = ux_Idle_Task_चळत;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in bytes. */
    *pul_Idle_Task_चळत_Size = config_MINIMAL_चळत_SIZE;
}
#endif // configSUPPORT_STATIC_ALLOCATION == 1
/*-----------------------------------------------------------*/

#यदि ( (configSUPPORT_STATIC_ALLOCATION == 1) && (configUSE_TIMERS == 1))

/* When configSUPPORT_STATIC_ALLOCATION is set to 1 the application writer can
 * use a callback function to optionally provide the memory required by the idle
 * and timer tasks.  This is the stack that will be used by the timer task.  It is
 * declared here, as a global, so it can be checked by a test that is implemented
 * in a different file. */
चळत_Type_t ux_Timer_Task_चळत[ config_TIMER_TASK_चळत_DEPTH ];


/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
 * application must provide an implementation of vApplicationGetTimerTaskMemory()
 * to provide the memory that is used by the Timer service task. */
शून्य vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                     चळत_Type_t ** ppx_Timer_Task_चळत_Buffer,
                                     uint32_t * pul_Timer_Task_चळत_Size )
{
/* If the buffers to be provided to the Timer task are declared inside this
 * function then they must be declared static - otherwise they will be allocated on
 * the stack and so not exists after this function exits. */
    स्थिर StaticTask_t xTimerTaskTCB;

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
     * task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppx_Timer_Task_चळत_Buffer = ux_Timer_Task_चळत;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in bytes. */
    *pul_Timer_Task_चळत_Size = config_TIMER_TASK_चळत_DEPTH;
}
#endif // configSUPPORT_STATIC_ALLOCATION == 1 && (configUSE_TIMERS == 1)

शून्य vPortTakeLock( portMUX_TYPE *lock )
{
    spinlock_acquire( lock, portMUX_NO_TIMEOUT);
}

शून्य vPortReleaseLock( portMUX_TYPE *lock )
{
    spinlock_release( lock );
}

#define FREERTOS_SMP_MALLOC_CAPS    (MALLOC_CAP_INTERNAL|MALLOC_CAP_8BIT)

शून्य *pvPortMalloc( size_t xSize )
{
    प्रत्यागम heap_caps_malloc(xSize, FREERTOS_SMP_MALLOC_CAPS);
}

शून्य vPortFree( शून्य *pv )
{
    heap_caps_free(pv);
}

शून्य __attribute__((weak)) v_Application_चळत_Overflow_Hook(TaskHandle_t xTask, वर्ण *pcTaskName)
{
#define ERR_STR1 "***ERROR*** A stack overflow in task "
#define ERR_STR2 " has been detected."
    ध्रुव वर्ण *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    वर्ण buf[परिमाण(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + परिमाण(ERR_STR2) + 1 /* null char */] = {0};

    वर्ण *dest = buf;
    प्रति (पूर्णाङ्क i = 0; i < परिमाण(str) / परिमाण(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
	printf("%s\n", buf);
    abort();
}

// ------- Thread Local Storage Pointers Deletion Callbacks -------

#यदि ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
शून्य vPortTLSPointersDelCb( शून्य *pxTCB )
{
    /* Typecast pxTCB to StaticTask_t type to access TCB struct members.
     * pvDummy15 corresponds to pvThreadLocalStoragePointers member of the TCB.
     */
    StaticTask_t *tcb = ( StaticTask_t * )pxTCB;

    /* The TLSP deletion callbacks are stored at an offset of (configNUM_THREAD_LOCAL_STORAGE_POINTERS/2) */
    TlsDeleteCallbackFunction_t *pvThreadLocalStoragePointersDelCallback = ( TlsDeleteCallbackFunction_t * )( &( tcb->pvDummy15[ ( config_अंक_THREAD_LOCAL_STORAGE_POINTERS / 2 ) ] ) );

    /* We need to iterate over half the depth of the pvThreadLocalStoragePointers area
     * to access all TLS pointers and their respective TLS deletion callbacks.
     */
    प्रति ( पूर्णाङ्क x = 0; x < ( config_अंक_THREAD_LOCAL_STORAGE_POINTERS / 2 ); x++ ) {
        यदि ( pvThreadLocalStoragePointersDelCallback[ x ] != रिक्त ) {  //If del cb is set
            // We skip the check if the callback is executable as that is difficult to determine for different
            // platforms (compare xtensa and riscv code).
            pvThreadLocalStoragePointersDelCallback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#endif // CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS

शून्य vPortCleanUpTCB ( शून्य *pxTCB )
{
    #यदि ( CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK )
        /* Call the user defined task pre-deletion hook */
        बाह्य शून्य vTaskPreDeletionHook( शून्य * pxTCB );
        vTaskPreDeletionHook( pxTCB );
    #endif /* CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK */

#यदि ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
    /* Call TLS pointers deletion callbacks */
    vPortTLSPointersDelCb( pxTCB );
#endif /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

    vPortCancelThread(pxTCB);
}
