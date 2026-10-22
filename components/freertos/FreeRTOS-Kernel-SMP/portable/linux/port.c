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

#അന്തര്ഭാവ <errno.h>
#അന്തര്ഭാവ <pthread.h>
#അന്തര്ഭാവ <signal.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <sys/time.h>
#അന്തര്ഭാവ <sys/times.h>
#അന്തര്ഭാവ <time.h>
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <assert.h>

/* Scheduler includes. */
#അന്തര്ഭാവ "esp_heap_caps.h"
#അന്തര്ഭാവ "FreeRTOS.h"
#അന്തര്ഭാവ "task.h"
#അന്തര്ഭാവ "esp_task.h"
#അന്തര്ഭാവ "timers.h"
#അന്തര്ഭാവ "utils/wait_for_event.h"
#അന്തര്ഭാവ "esp_log.h"
/*-----------------------------------------------------------*/

#define SIG_RESUME SIGUSR1

തരനിർവചനം ഘടന THREAD
{
    pthread_t pthread;
    TaskFunction_t pxCode;
    ശൂന്യം *pvParams;
    BaseType_t xDying;
    ഘടന event *ev;
} Thread_t;

/*
 * The additional per-thread data is stored at the beginning of the
 * task's stack.
 */
സ്ഥിതം വരിയിൽ Thread_t *prvGetThreadFromTask(TaskHandle_t xTask)
{
    അട്ടി_Type_t *px_മുകൾ_Of_അട്ടി = *(അട്ടി_Type_t **)xTask;

    തിരികെ (Thread_t *)(px_മുകൾ_Of_അട്ടി + 1);
}

/*-----------------------------------------------------------*/

സ്ഥിതം pthread_once_t hSigSetupThread = PTHREAD_ONCE_INIT;
സ്ഥിതം sigset_t xAllSignals;
സ്ഥിതം sigset_t xSchedulerOriginalSignalMask;
സ്ഥിതം pthread_t hMainThread = ( pthread_t )ശൂന്യ;

// These are saved as part of a thread's state in prvSwitchThread()
സ്ഥിതം അസ്ഥിര BaseType_t uxCriticalNestingIDF = 0;    /* Track nesting calls for IDF style critical sections. FreeRTOS critical section nesting is maintained in the TCB. */
സ്ഥിതം അസ്ഥിര UBaseType_t uxInterruptNesting = 0;     /* Tracks if we are currently in an interrupt. */
സ്ഥിതം അസ്ഥിര BaseType_t uxInterruptLevel = 0;        /* Tracks the current level (i.e., interrupt mask) */
/*-----------------------------------------------------------*/

സ്ഥിതം BaseType_t xSchedulerEnd = pdFALSE;
/*-----------------------------------------------------------*/

സ്ഥിതം ശൂന്യം prvSetupSignalsAndSchedulerPolicy( ശൂന്യം );
സ്ഥിതം ശൂന്യം prvSetupTimerInterrupt( ശൂന്യം );
സ്ഥിതം ശൂന്യം *prvWaitForStart( ശൂന്യം * pvParams );
സ്ഥിതം ശൂന്യം prvSwitchThread( Thread_t * xThreadToResume,
                             Thread_t *xThreadToSuspend );
സ്ഥിതം ശൂന്യം prvSuspendSelf( Thread_t * thread);
സ്ഥിതം ശൂന്യം prvResumeThread( Thread_t * xThreadId );
സ്ഥിതം ശൂന്യം vPortSystemTickHandler( പൂർണ്ണസംഖ്യ sig );
സ്ഥിതം ശൂന്യം vPortStartFirstTask( ശൂന്യം );
/*-----------------------------------------------------------*/

സ്ഥിതം ശൂന്യം prvFatalError( സ്ഥിരം അക്ഷരം *pcCall, പൂർണ്ണസംഖ്യ iErrno )
{
    fprintf( stderr, "%s: %s\n", pcCall, strerror( iErrno ) );
    abort();
}

/*
 * See header file for description.
 */
അട്ടി_Type_t *px_Port_Initialise_അട്ടി( അട്ടി_Type_t *px_മുകൾ_Of_അട്ടി,
                                    അട്ടി_Type_t *px_End_Of_അട്ടി,
                                    TaskFunction_t pxCode,
                                    ശൂന്യം *pvParameters )
{
    Thread_t *thread;
    pthread_attr_t xThreadAttributes;
    size_t ul_അട്ടി_Size;
    പൂർണ്ണസംഖ്യ iRet;

    (ശൂന്യം)pthread_once( &hSigSetupThread, prvSetupSignalsAndSchedulerPolicy );

    /*
     * Store the additional thread data at the start of the stack.
     */
    thread = (Thread_t *)(px_മുകൾ_Of_അട്ടി + 1) - 1;
    px_മുകൾ_Of_അട്ടി = (അട്ടി_Type_t *)thread - 1;
    ul_അട്ടി_Size = (px_മുകൾ_Of_അട്ടി + 1 - px_End_Of_അട്ടി) * വലുപ്പം(*px_മുകൾ_Of_അട്ടി);

    thread->pxCode = pxCode;
    thread->pvParams = pvParameters;
    thread->xDying = pdFALSE;

    pthread_attr_init( &xThreadAttributes );
    pthread_attr_setstack( &xThreadAttributes, px_End_Of_അട്ടി, ul_അട്ടി_Size );

    thread->ev = event_create();

    BaseType_t prev_intr_level = xPortSetInterruptMask();

    iRet = pthread_create( &thread->pthread, &xThreadAttributes,
                           prvWaitForStart, thread );
    എങ്കിൽ ( iRet )
    {
        prvFatalError( "pthread_create", iRet );
    }

    vPortClearInterruptMask( prev_intr_level );

    തിരികെ px_മുകൾ_Of_അട്ടി;
}
/*-----------------------------------------------------------*/

ശൂന്യം vPortStartFirstTask( ശൂന്യം )
{
    Thread_t *pxFirstThread = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    /* Start the first task. */
    prvResumeThread( pxFirstThread );
}
/*-----------------------------------------------------------*/

/*
 * See header file for description.
 */
BaseType_t xPortStartScheduler( ശൂന്യം )
{
    പൂർണ്ണസംഖ്യ iSignal;
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

    ആകുമ്പോൾ ( !xSchedulerEnd )
    {
        sigwait( &xSignals, &iSignal );
    }

    /* Cancel the Idle task and free its resources */
#എങ്കിൽ ( INCLUDE_xTaskGetIdleTaskHandle == 1 )
    vPortCancelThread( xTaskGetIdleTaskHandle() );
#endif

#എങ്കിൽ ( configUSE_TIMERS == 1 )
    /* Cancel the Timer task and free its resources */
    vPortCancelThread( xTimerGetTimerDaemonTaskHandle() );
#endif /* configUSE_TIMERS */

    /* Restore original signal mask. */
    (ശൂന്യം)pthread_sigmask( SIG_SETMASK, &xSchedulerOriginalSignalMask,  ശൂന്യ );

    തിരികെ 0;
}
/*-----------------------------------------------------------*/

ശൂന്യം vPortEndScheduler( ശൂന്യം )
{
    ഘടന itimerval itimer;
    ഘടന sigaction sigtick;
    Thread_t *xCurrentThread;

    /* Stop the timer and ignore any pending SIGALRMs that would end
     * up running on the main thread when it is resumed. */
    itimer.it_value.tv_sec = 0;
    itimer.it_value.tv_usec = 0;

    itimer.it_interval.tv_sec = 0;
    itimer.it_interval.tv_usec = 0;
    (ശൂന്യം)setitimer( ITIMER_REAL, &itimer, ശൂന്യ );

    sigtick.sa_flags = 0;
    sigtick.sa_handler = SIG_IGN;
    sigemptyset( &sigtick.sa_mask );
    sigaction( SIGALRM, &sigtick, ശൂന്യ );

    /* Signal the scheduler to exit its loop. */
    xSchedulerEnd = pdTRUE;
    (ശൂന്യം)pthread_kill( hMainThread, SIG_RESUME );

    xCurrentThread = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );
    prvSuspendSelf(xCurrentThread);
}
/*-----------------------------------------------------------*/

സ്ഥിതം ശൂന്യം vPortDisableInterrupts( ശൂന്യം )
{
    pthread_sigmask( SIG_BLOCK, &xAllSignals, ശൂന്യ );
}
/*-----------------------------------------------------------*/

സ്ഥിതം ശൂന്യം vPortEnableInterrupts( ശൂന്യം )
{
    pthread_sigmask( SIG_UNBLOCK, &xAllSignals, ശൂന്യ );
}
/*-----------------------------------------------------------*/

ശൂന്യം vPortEnterCriticalIDF( ശൂന്യം )
{
    എങ്കിൽ ( uxCriticalNestingIDF == 0 && uxInterruptLevel == 0)
    {
        vPortDisableInterrupts();
    }
    uxCriticalNestingIDF++;
}
/*-----------------------------------------------------------*/

ശൂന്യം vPortExitCriticalIDF( ശൂന്യം )
{
    uxCriticalNestingIDF--;

    /* If we have reached 0 then re-enable the interrupts. */
    എങ്കിൽ( uxCriticalNestingIDF == 0 && uxInterruptLevel == 0)
    {
        vPortEnableInterrupts();
    }
}
/*-----------------------------------------------------------*/

ശൂന്യം vPortYieldFromISR( ശൂന്യം )
{
    Thread_t *xThreadToSuspend;
    Thread_t *xThreadToResume;

    xThreadToSuspend = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    vTaskSwitchContext();

    xThreadToResume = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    prvSwitchThread( xThreadToResume, xThreadToSuspend );
}
/*-----------------------------------------------------------*/

ശൂന്യം vPortYield( ശൂന്യം )
{
    BaseType_t prev_intr_level = xPortSetInterruptMask();

    vPortYieldFromISR();

    vPortClearInterruptMask( prev_intr_level );
}
/*-----------------------------------------------------------*/

/* In SMP code, the disable/enable interrupt macros are calling the set/get interrupt mask functions below.
   Hence, we need to call vPortDisableInterrupts() and vPortEnableInterrupts(), otherwise interrupts
   are never disabled/enabled. */

BaseType_t xPortSetInterruptMask( ശൂന്യം )
{
    എങ്കിൽ (uxInterruptLevel == 0 && uxCriticalNestingIDF == 0) {
        vPortDisableInterrupts();
    }
    BaseType_t prev_intr_level = uxInterruptLevel;
    uxInterruptLevel++;
    തിരികെ prev_intr_level;
}
/*-----------------------------------------------------------*/

ശൂന്യം vPortClearInterruptMask( BaseType_t xMask )
{
    // Only re-enable interrupts if xMask is 0
    uxInterruptLevel = xMask;
    എങ്കിൽ (uxInterruptLevel == 0 && uxCriticalNestingIDF == 0) {
        vPortEnableInterrupts();
    }
}
/*-----------------------------------------------------------*/

സ്ഥിതം uint64_t prvGetTimeNs(ശൂന്യം)
{
    ഘടന timespec t;

    clock_gettime(CLOCK_MONOTONIC, &t);

    തിരികെ t.tv_sec * 1000000000ull + t.tv_nsec;
}

സ്ഥിതം uint64_t prvStartTimeNs;
/* commented as part of the code below in vPortSystemTickHandler,
 * to adjust timing according to full demo requirements */
/* static uint64_t prvTickCount; */

/*
 * Setup the systick timer to generate the tick interrupts at the required
 * frequency.
 */
ശൂന്യം prvSetupTimerInterrupt( ശൂന്യം )
{
    ഘടന itimerval itimer;
    പൂർണ്ണസംഖ്യ iRet;

    /* Initialise the structure with the current timer information. */
    iRet = getitimer( ITIMER_REAL, &itimer );
    എങ്കിൽ ( iRet )
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
    iRet = setitimer( ITIMER_REAL, &itimer, ശൂന്യ );
    എങ്കിൽ ( iRet )
    {
        prvFatalError( "setitimer", errno );
    }

    prvStartTimeNs = prvGetTimeNs();
}
/*-----------------------------------------------------------*/

സ്ഥിതം ശൂന്യം vPortSystemTickHandler( പൂർണ്ണസംഖ്യ sig )
{
    Thread_t *pxThreadToSuspend;
    Thread_t *pxThreadToResume;
    BaseType_t xSwitchRequired;
    /* uint64_t xExpectedTicks; */

    // Handling a timer signal, so we are currently in an interrupt.
    uxInterruptNesting++;

#എങ്കിൽ ( configUSE_PREEMPTION == 1 )
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

#എങ്കിൽ ( configUSE_PREEMPTION == 1 )
    എങ്കിൽ (xSwitchRequired == pdTRUE) {
        /* Select Next Task. */
        vTaskSwitchContext();

        pxThreadToResume = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

        prvSwitchThread(pxThreadToResume, pxThreadToSuspend);
    }
#അല്ലെങ്കിൽ
    (ശൂന്യം)xSwitchRequired;
#endif

    // Returning from the timer signal handler, so we are exiting the interrupt.
    uxInterruptNesting--;
}
/*-----------------------------------------------------------*/

ശൂന്യം vPortThreadDying( ശൂന്യം *pxTaskToDelete, അസ്ഥിര BaseType_t *pxPendYield )
{
    Thread_t *pxThread = prvGetThreadFromTask( pxTaskToDelete );

    pxThread->xDying = pdTRUE;
}

ശൂന്യം vPortCancelThread( ശൂന്യം *pxTaskToDelete )
{
    Thread_t *pxThreadToCancel = prvGetThreadFromTask( pxTaskToDelete );

    /*
     * The thread has already been suspended so it can be safely cancelled.
     */
    pthread_cancel( pxThreadToCancel->pthread );
    pthread_join( pxThreadToCancel->pthread, ശൂന്യ );
    event_delete( pxThreadToCancel->ev );
}
/*-----------------------------------------------------------*/

സ്ഥിതം ശൂന്യം *prvWaitForStart( ശൂന്യം * pvParams )
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

    തിരികെ ശൂന്യ;
}
/*-----------------------------------------------------------*/

സ്ഥിതം ശൂന്യം prvSwitchThread( Thread_t *pxThreadToResume,
                             Thread_t *pxThreadToSuspend )
{
    BaseType_t uxSavedCriticalNestingIDF;
    BaseType_t uxSavedInterruptNesting;
    BaseType_t uxSavedInterruptLevel;

    എങ്കിൽ ( pxThreadToSuspend != pxThreadToResume )
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
        എങ്കിൽ ( pxThreadToSuspend->xDying )
        {
            pthread_exit( ശൂന്യ );
        }
        prvSuspendSelf( pxThreadToSuspend );

        uxCriticalNestingIDF = uxSavedCriticalNestingIDF;
        uxInterruptNesting = uxSavedInterruptNesting;
        uxInterruptLevel = uxSavedInterruptLevel;
    }
}
/*-----------------------------------------------------------*/

സ്ഥിതം ശൂന്യം prvSuspendSelf( Thread_t *thread )
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

സ്ഥിതം ശൂന്യം prvResumeThread( Thread_t *xThreadId )
{
    എങ്കിൽ ( pthread_self() != xThreadId->pthread )
    {
        event_signal(xThreadId->ev);
    }
}
/*-----------------------------------------------------------*/

സ്ഥിതം ശൂന്യം prvSetupSignalsAndSchedulerPolicy( ശൂന്യം )
{
    ഘടന sigaction sigresume, sigtick;
    പൂർണ്ണസംഖ്യ iRet;

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
    (ശൂന്യം)pthread_sigmask( SIG_SETMASK, &xAllSignals,
                           &xSchedulerOriginalSignalMask );

    /* SIG_RESUME is only used with sigwait() so doesn't need a
       handler. */
    sigresume.sa_flags = 0;
    sigresume.sa_handler = SIG_IGN;
    sigfillset( &sigresume.sa_mask );

    sigtick.sa_flags = 0;
    sigtick.sa_handler = vPortSystemTickHandler;
    sigfillset( &sigtick.sa_mask );

    iRet = sigaction( SIG_RESUME, &sigresume, ശൂന്യ );
    എങ്കിൽ ( iRet )
    {
        prvFatalError( "sigaction", errno );
    }

    iRet = sigaction( SIGALRM, &sigtick, ശൂന്യ );
    എങ്കിൽ ( iRet )
    {
        prvFatalError( "sigaction", errno );
    }
}
/*-----------------------------------------------------------*/

unsigned long ulPortGetRunTime( ശൂന്യം )
{
    ഘടന tms xTimes;

    times( &xTimes );

    തിരികെ ( unsigned long ) xTimes.tms_utime;
}
/*-----------------------------------------------------------*/
bool portVALID_LIST_MEM(സ്ഥിരം ശൂന്യം *ptr)
{
    തിരികെ true;
}

bool portVALID_TCB_MEM(സ്ഥിരം ശൂന്യം *ptr)
{
    തിരികെ true;
}

bool port_VALID_അട്ടി_MEM(സ്ഥിരം ശൂന്യം *ptr)
{
    തിരികെ true;
}
/*-----------------------------------------------------------*/

portMUX_TYPE port_xTaskLock = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE port_xISRLock = portMUX_INITIALIZER_UNLOCKED;

സ്ഥിതം സ്ഥിരം അക്ഷരം *TAG = "port";

BaseType_t xPortCheckIfInISR(ശൂന്യം)
{
    തിരികെ (uxInterruptNesting == 0) ? pdFALSE : pdTRUE;
}

ശൂന്യം app_main(ശൂന്യം);

സ്ഥിതം ശൂന്യം main_task(ശൂന്യം* args)
{
    app_main();
    vTaskDelete(ശൂന്യ);
}

പൂർണ്ണസംഖ്യ പ്രധാന(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം **argv)
{
    // This makes sure that stdio is flushed after each '\n' so that idf.py monitor
    // reads the program output on time.
    setvbuf(stdout, ശൂന്യ, _IOLBF, 0);

    usleep(1000);

    BaseType_t res;

#എങ്കിൽ ( config_അക്കം_CORES > 1 )
    res = xTaskCreateAffinitySet(&main_task, "main",
                                 ESP_TASK_MAIN_അട്ടി, ശൂന്യ,
                                 ESP_TASK_MAIN_PRIO, ESP_TASK_MAIN_CORE, ശൂന്യ);
#അല്ലെങ്കിൽ
    res = xTaskCreate(&main_task, "main",
                      ESP_TASK_MAIN_അട്ടി, ശൂന്യ,
                      ESP_TASK_MAIN_PRIO, ശൂന്യ);
#endif

    പ്രതിജ്ഞാ(res == pdTRUE);
    (ശൂന്യം)res;

    ESP_LOGI(TAG, "Starting SMP scheduler.");
    vTaskStartScheduler();

    // This line should never be reached
    പ്രതിജ്ഞാ(false);
}

ശൂന്യം esp_vApplicationIdleHook(ശൂന്യം)
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

ശൂന്യം esp_vApplicationTickHook( ശൂന്യം ) { }

#എങ്കിൽ  (  configUSE_TICK_HOOK > 0 )
ശൂന്യം vApplicationTickHook( ശൂന്യം )
{
    esp_vApplicationTickHook();
}
#endif

#എങ്കിൽ ( configSUPPORT_STATIC_ALLOCATION == 1 )
/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
 * implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
 * used by the Idle task. */
ശൂന്യം vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                    അട്ടി_Type_t ** ppx_Idle_Task_അട്ടി_Buffer,
                                    uint32_t * pul_Idle_Task_അട്ടി_Size )
{
/* If the buffers to be provided to the Idle task are declared inside this
 * function then they must be declared static - otherwise they will be allocated on
 * the stack and so not exists after this function exits. */
    സ്ഥിതം StaticTask_t xIdleTaskTCB;
    സ്ഥിതം അട്ടി_Type_t ux_Idle_Task_അട്ടി[ config_MINIMAL_അട്ടി_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
     * state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppx_Idle_Task_അട്ടി_Buffer = ux_Idle_Task_അട്ടി;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in bytes. */
    *pul_Idle_Task_അട്ടി_Size = config_MINIMAL_അട്ടി_SIZE;
}
#endif // configSUPPORT_STATIC_ALLOCATION == 1
/*-----------------------------------------------------------*/

#എങ്കിൽ ( (configSUPPORT_STATIC_ALLOCATION == 1) && (configUSE_TIMERS == 1))

/* When configSUPPORT_STATIC_ALLOCATION is set to 1 the application writer can
 * use a callback function to optionally provide the memory required by the idle
 * and timer tasks.  This is the stack that will be used by the timer task.  It is
 * declared here, as a global, so it can be checked by a test that is implemented
 * in a different file. */
അട്ടി_Type_t ux_Timer_Task_അട്ടി[ config_TIMER_TASK_അട്ടി_DEPTH ];


/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
 * application must provide an implementation of vApplicationGetTimerTaskMemory()
 * to provide the memory that is used by the Timer service task. */
ശൂന്യം vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                     അട്ടി_Type_t ** ppx_Timer_Task_അട്ടി_Buffer,
                                     uint32_t * pul_Timer_Task_അട്ടി_Size )
{
/* If the buffers to be provided to the Timer task are declared inside this
 * function then they must be declared static - otherwise they will be allocated on
 * the stack and so not exists after this function exits. */
    സ്ഥിതം StaticTask_t xTimerTaskTCB;

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
     * task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppx_Timer_Task_അട്ടി_Buffer = ux_Timer_Task_അട്ടി;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in bytes. */
    *pul_Timer_Task_അട്ടി_Size = config_TIMER_TASK_അട്ടി_DEPTH;
}
#endif // configSUPPORT_STATIC_ALLOCATION == 1 && (configUSE_TIMERS == 1)

ശൂന്യം vPortTakeLock( portMUX_TYPE *lock )
{
    spinlock_acquire( lock, portMUX_NO_TIMEOUT);
}

ശൂന്യം vPortReleaseLock( portMUX_TYPE *lock )
{
    spinlock_release( lock );
}

#define FREERTOS_SMP_MALLOC_CAPS    (MALLOC_CAP_INTERNAL|MALLOC_CAP_8BIT)

ശൂന്യം *pvPortMalloc( size_t xSize )
{
    തിരികെ heap_caps_malloc(xSize, FREERTOS_SMP_MALLOC_CAPS);
}

ശൂന്യം vPortFree( ശൂന്യം *pv )
{
    heap_caps_free(pv);
}

ശൂന്യം __attribute__((weak)) v_Application_അട്ടി_Overflow_Hook(TaskHandle_t xTask, അക്ഷരം *pcTaskName)
{
#define ERR_STR1 "***ERROR*** A stack overflow in task "
#define ERR_STR2 " has been detected."
    സ്ഥിരം അക്ഷരം *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    അക്ഷരം buf[വലുപ്പം(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + വലുപ്പം(ERR_STR2) + 1 /* null char */] = {0};

    അക്ഷരം *dest = buf;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < വലുപ്പം(str) / വലുപ്പം(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
	printf("%s\n", buf);
    abort();
}

// ------- Thread Local Storage Pointers Deletion Callbacks -------

#എങ്കിൽ ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
ശൂന്യം vPortTLSPointersDelCb( ശൂന്യം *pxTCB )
{
    /* Typecast pxTCB to StaticTask_t type to access TCB struct members.
     * pvDummy15 corresponds to pvThreadLocalStoragePointers member of the TCB.
     */
    StaticTask_t *tcb = ( StaticTask_t * )pxTCB;

    /* The TLSP deletion callbacks are stored at an offset of (configNUM_THREAD_LOCAL_STORAGE_POINTERS/2) */
    TlsDeleteCallbackFunction_t *pvThreadLocalStoragePointersDelCallback = ( TlsDeleteCallbackFunction_t * )( &( tcb->pvDummy15[ ( config_അക്കം_THREAD_LOCAL_STORAGE_POINTERS / 2 ) ] ) );

    /* We need to iterate over half the depth of the pvThreadLocalStoragePointers area
     * to access all TLS pointers and their respective TLS deletion callbacks.
     */
    വേണ്ടി ( പൂർണ്ണസംഖ്യ x = 0; x < ( config_അക്കം_THREAD_LOCAL_STORAGE_POINTERS / 2 ); x++ ) {
        എങ്കിൽ ( pvThreadLocalStoragePointersDelCallback[ x ] != ശൂന്യ ) {  //If del cb is set
            // We skip the check if the callback is executable as that is difficult to determine for different
            // platforms (compare xtensa and riscv code).
            pvThreadLocalStoragePointersDelCallback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#endif // CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS

ശൂന്യം vPortCleanUpTCB ( ശൂന്യം *pxTCB )
{
    #എങ്കിൽ ( CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK )
        /* Call the user defined task pre-deletion hook */
        ബാഹ്യ ശൂന്യം vTaskPreDeletionHook( ശൂന്യം * pxTCB );
        vTaskPreDeletionHook( pxTCB );
    #endif /* CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK */

#എങ്കിൽ ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
    /* Call TLS pointers deletion callbacks */
    vPortTLSPointersDelCb( pxTCB );
#endif /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

    vPortCancelThread(pxTCB);
}
