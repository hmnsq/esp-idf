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

#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <pthread.h>
#ಅನ್ತರ್ಭಾವ <signal.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <sys/time.h>
#ಅನ್ತರ್ಭಾವ <sys/times.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <assert.h>

/* Scheduler includes. */
#ಅನ್ತರ್ಭಾವ "esp_heap_caps.h"
#ಅನ್ತರ್ಭಾವ "FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "task.h"
#ಅನ್ತರ್ಭಾವ "esp_task.h"
#ಅನ್ತರ್ಭಾವ "timers.h"
#ಅನ್ತರ್ಭಾವ "utils/wait_for_event.h"
#ಅನ್ತರ್ಭಾವ "esp_log.h"
/*-----------------------------------------------------------*/

#define SIG_RESUME SIGUSR1

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ THREAD
{
    pthread_t pthread;
    TaskFunction_t pxCode;
    ಶೂನ್ಯ *pvParams;
    BaseType_t xDying;
    ಸಂರಚನಾ event *ev;
} Thread_t;

/*
 * The additional per-thread data is stored at the beginning of the
 * task's stack.
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ Thread_t *prvGetThreadFromTask(TaskHandle_t xTask)
{
    ರಾಶಿ_Type_t *px_ತುದಿ_Of_ರಾಶಿ = *(ರಾಶಿ_Type_t **)xTask;

    ಪ್ರತ್ಯಾಗಮ (Thread_t *)(px_ತುದಿ_Of_ರಾಶಿ + 1);
}

/*-----------------------------------------------------------*/

ಸ್ಥಿರ pthread_once_t hSigSetupThread = PTHREAD_ONCE_INIT;
ಸ್ಥಿರ sigset_t xAllSignals;
ಸ್ಥಿರ sigset_t xSchedulerOriginalSignalMask;
ಸ್ಥಿರ pthread_t hMainThread = ( pthread_t )ರಿಕ್ತ;

// These are saved as part of a thread's state in prvSwitchThread()
ಸ್ಥಿರ ಚಞ್ಚಲ BaseType_t uxCriticalNestingIDF = 0;    /* Track nesting calls for IDF style critical sections. FreeRTOS critical section nesting is maintained in the TCB. */
ಸ್ಥಿರ ಚಞ್ಚಲ UBaseType_t uxInterruptNesting = 0;     /* Tracks if we are currently in an interrupt. */
ಸ್ಥಿರ ಚಞ್ಚಲ BaseType_t uxInterruptLevel = 0;        /* Tracks the current level (i.e., interrupt mask) */
/*-----------------------------------------------------------*/

ಸ್ಥಿರ BaseType_t xSchedulerEnd = pdFALSE;
/*-----------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ prvSetupSignalsAndSchedulerPolicy( ಶೂನ್ಯ );
ಸ್ಥಿರ ಶೂನ್ಯ prvSetupTimerInterrupt( ಶೂನ್ಯ );
ಸ್ಥಿರ ಶೂನ್ಯ *prvWaitForStart( ಶೂನ್ಯ * pvParams );
ಸ್ಥಿರ ಶೂನ್ಯ prvSwitchThread( Thread_t * xThreadToResume,
                             Thread_t *xThreadToSuspend );
ಸ್ಥಿರ ಶೂನ್ಯ prvSuspendSelf( Thread_t * thread);
ಸ್ಥಿರ ಶೂನ್ಯ prvResumeThread( Thread_t * xThreadId );
ಸ್ಥಿರ ಶೂನ್ಯ vPortSystemTickHandler( ಪೂರ್ಣಾಙ್ಕ sig );
ಸ್ಥಿರ ಶೂನ್ಯ vPortStartFirstTask( ಶೂನ್ಯ );
/*-----------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ prvFatalError( ಧ್ರುವ ವರ್ಣ *pcCall, ಪೂರ್ಣಾಙ್ಕ iErrno )
{
    fprintf( stderr, "%s: %s\n", pcCall, strerror( iErrno ) );
    abort();
}

/*
 * See header file for description.
 */
ರಾಶಿ_Type_t *px_Port_Initialise_ರಾಶಿ( ರಾಶಿ_Type_t *px_ತುದಿ_Of_ರಾಶಿ,
                                    ರಾಶಿ_Type_t *px_End_Of_ರಾಶಿ,
                                    TaskFunction_t pxCode,
                                    ಶೂನ್ಯ *pvParameters )
{
    Thread_t *thread;
    pthread_attr_t xThreadAttributes;
    size_t ul_ರಾಶಿ_Size;
    ಪೂರ್ಣಾಙ್ಕ iRet;

    (ಶೂನ್ಯ)pthread_once( &hSigSetupThread, prvSetupSignalsAndSchedulerPolicy );

    /*
     * Store the additional thread data at the start of the stack.
     */
    thread = (Thread_t *)(px_ತುದಿ_Of_ರಾಶಿ + 1) - 1;
    px_ತುದಿ_Of_ರಾಶಿ = (ರಾಶಿ_Type_t *)thread - 1;
    ul_ರಾಶಿ_Size = (px_ತುದಿ_Of_ರಾಶಿ + 1 - px_End_Of_ರಾಶಿ) * ಪರಿಮಾಣ(*px_ತುದಿ_Of_ರಾಶಿ);

    thread->pxCode = pxCode;
    thread->pvParams = pvParameters;
    thread->xDying = pdFALSE;

    pthread_attr_init( &xThreadAttributes );
    pthread_attr_setstack( &xThreadAttributes, px_End_Of_ರಾಶಿ, ul_ರಾಶಿ_Size );

    thread->ev = event_create();

    BaseType_t prev_intr_level = xPortSetInterruptMask();

    iRet = pthread_create( &thread->pthread, &xThreadAttributes,
                           prvWaitForStart, thread );
    ಯದಿ ( iRet )
    {
        prvFatalError( "pthread_create", iRet );
    }

    vPortClearInterruptMask( prev_intr_level );

    ಪ್ರತ್ಯಾಗಮ px_ತುದಿ_Of_ರಾಶಿ;
}
/*-----------------------------------------------------------*/

ಶೂನ್ಯ vPortStartFirstTask( ಶೂನ್ಯ )
{
    Thread_t *pxFirstThread = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    /* Start the first task. */
    prvResumeThread( pxFirstThread );
}
/*-----------------------------------------------------------*/

/*
 * See header file for description.
 */
BaseType_t xPortStartScheduler( ಶೂನ್ಯ )
{
    ಪೂರ್ಣಾಙ್ಕ iSignal;
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

    ಯಾವತ್ ( !xSchedulerEnd )
    {
        sigwait( &xSignals, &iSignal );
    }

    /* Cancel the Idle task and free its resources */
#ಯದಿ ( INCLUDE_xTaskGetIdleTaskHandle == 1 )
    vPortCancelThread( xTaskGetIdleTaskHandle() );
#endif

#ಯದಿ ( configUSE_TIMERS == 1 )
    /* Cancel the Timer task and free its resources */
    vPortCancelThread( xTimerGetTimerDaemonTaskHandle() );
#endif /* configUSE_TIMERS */

    /* Restore original signal mask. */
    (ಶೂನ್ಯ)pthread_sigmask( SIG_SETMASK, &xSchedulerOriginalSignalMask,  ರಿಕ್ತ );

    ಪ್ರತ್ಯಾಗಮ 0;
}
/*-----------------------------------------------------------*/

ಶೂನ್ಯ vPortEndScheduler( ಶೂನ್ಯ )
{
    ಸಂರಚನಾ itimerval itimer;
    ಸಂರಚನಾ sigaction sigtick;
    Thread_t *xCurrentThread;

    /* Stop the timer and ignore any pending SIGALRMs that would end
     * up running on the main thread when it is resumed. */
    itimer.it_value.tv_sec = 0;
    itimer.it_value.tv_usec = 0;

    itimer.it_interval.tv_sec = 0;
    itimer.it_interval.tv_usec = 0;
    (ಶೂನ್ಯ)setitimer( ITIMER_REAL, &itimer, ರಿಕ್ತ );

    sigtick.sa_flags = 0;
    sigtick.sa_handler = SIG_IGN;
    sigemptyset( &sigtick.sa_mask );
    sigaction( SIGALRM, &sigtick, ರಿಕ್ತ );

    /* Signal the scheduler to exit its loop. */
    xSchedulerEnd = pdTRUE;
    (ಶೂನ್ಯ)pthread_kill( hMainThread, SIG_RESUME );

    xCurrentThread = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );
    prvSuspendSelf(xCurrentThread);
}
/*-----------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ vPortDisableInterrupts( ಶೂನ್ಯ )
{
    pthread_sigmask( SIG_BLOCK, &xAllSignals, ರಿಕ್ತ );
}
/*-----------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ vPortEnableInterrupts( ಶೂನ್ಯ )
{
    pthread_sigmask( SIG_UNBLOCK, &xAllSignals, ರಿಕ್ತ );
}
/*-----------------------------------------------------------*/

ಶೂನ್ಯ vPortEnterCriticalIDF( ಶೂನ್ಯ )
{
    ಯದಿ ( uxCriticalNestingIDF == 0 && uxInterruptLevel == 0)
    {
        vPortDisableInterrupts();
    }
    uxCriticalNestingIDF++;
}
/*-----------------------------------------------------------*/

ಶೂನ್ಯ vPortExitCriticalIDF( ಶೂನ್ಯ )
{
    uxCriticalNestingIDF--;

    /* If we have reached 0 then re-enable the interrupts. */
    ಯದಿ( uxCriticalNestingIDF == 0 && uxInterruptLevel == 0)
    {
        vPortEnableInterrupts();
    }
}
/*-----------------------------------------------------------*/

ಶೂನ್ಯ vPortYieldFromISR( ಶೂನ್ಯ )
{
    Thread_t *xThreadToSuspend;
    Thread_t *xThreadToResume;

    xThreadToSuspend = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    vTaskSwitchContext();

    xThreadToResume = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    prvSwitchThread( xThreadToResume, xThreadToSuspend );
}
/*-----------------------------------------------------------*/

ಶೂನ್ಯ vPortYield( ಶೂನ್ಯ )
{
    BaseType_t prev_intr_level = xPortSetInterruptMask();

    vPortYieldFromISR();

    vPortClearInterruptMask( prev_intr_level );
}
/*-----------------------------------------------------------*/

/* In SMP code, the disable/enable interrupt macros are calling the set/get interrupt mask functions below.
   Hence, we need to call vPortDisableInterrupts() and vPortEnableInterrupts(), otherwise interrupts
   are never disabled/enabled. */

BaseType_t xPortSetInterruptMask( ಶೂನ್ಯ )
{
    ಯದಿ (uxInterruptLevel == 0 && uxCriticalNestingIDF == 0) {
        vPortDisableInterrupts();
    }
    BaseType_t prev_intr_level = uxInterruptLevel;
    uxInterruptLevel++;
    ಪ್ರತ್ಯಾಗಮ prev_intr_level;
}
/*-----------------------------------------------------------*/

ಶೂನ್ಯ vPortClearInterruptMask( BaseType_t xMask )
{
    // Only re-enable interrupts if xMask is 0
    uxInterruptLevel = xMask;
    ಯದಿ (uxInterruptLevel == 0 && uxCriticalNestingIDF == 0) {
        vPortEnableInterrupts();
    }
}
/*-----------------------------------------------------------*/

ಸ್ಥಿರ uint64_t prvGetTimeNs(ಶೂನ್ಯ)
{
    ಸಂರಚನಾ timespec t;

    clock_gettime(CLOCK_MONOTONIC, &t);

    ಪ್ರತ್ಯಾಗಮ t.tv_sec * 1000000000ull + t.tv_nsec;
}

ಸ್ಥಿರ uint64_t prvStartTimeNs;
/* commented as part of the code below in vPortSystemTickHandler,
 * to adjust timing according to full demo requirements */
/* static uint64_t prvTickCount; */

/*
 * Setup the systick timer to generate the tick interrupts at the required
 * frequency.
 */
ಶೂನ್ಯ prvSetupTimerInterrupt( ಶೂನ್ಯ )
{
    ಸಂರಚನಾ itimerval itimer;
    ಪೂರ್ಣಾಙ್ಕ iRet;

    /* Initialise the structure with the current timer information. */
    iRet = getitimer( ITIMER_REAL, &itimer );
    ಯದಿ ( iRet )
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
    iRet = setitimer( ITIMER_REAL, &itimer, ರಿಕ್ತ );
    ಯದಿ ( iRet )
    {
        prvFatalError( "setitimer", errno );
    }

    prvStartTimeNs = prvGetTimeNs();
}
/*-----------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ vPortSystemTickHandler( ಪೂರ್ಣಾಙ್ಕ sig )
{
    Thread_t *pxThreadToSuspend;
    Thread_t *pxThreadToResume;
    BaseType_t xSwitchRequired;
    /* uint64_t xExpectedTicks; */

    // Handling a timer signal, so we are currently in an interrupt.
    uxInterruptNesting++;

#ಯದಿ ( configUSE_PREEMPTION == 1 )
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

#ಯದಿ ( configUSE_PREEMPTION == 1 )
    ಯದಿ (xSwitchRequired == pdTRUE) {
        /* Select Next Task. */
        vTaskSwitchContext();

        pxThreadToResume = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

        prvSwitchThread(pxThreadToResume, pxThreadToSuspend);
    }
#ಅನ್ಯಥಾ
    (ಶೂನ್ಯ)xSwitchRequired;
#endif

    // Returning from the timer signal handler, so we are exiting the interrupt.
    uxInterruptNesting--;
}
/*-----------------------------------------------------------*/

ಶೂನ್ಯ vPortThreadDying( ಶೂನ್ಯ *pxTaskToDelete, ಚಞ್ಚಲ BaseType_t *pxPendYield )
{
    Thread_t *pxThread = prvGetThreadFromTask( pxTaskToDelete );

    pxThread->xDying = pdTRUE;
}

ಶೂನ್ಯ vPortCancelThread( ಶೂನ್ಯ *pxTaskToDelete )
{
    Thread_t *pxThreadToCancel = prvGetThreadFromTask( pxTaskToDelete );

    /*
     * The thread has already been suspended so it can be safely cancelled.
     */
    pthread_cancel( pxThreadToCancel->pthread );
    pthread_join( pxThreadToCancel->pthread, ರಿಕ್ತ );
    event_delete( pxThreadToCancel->ev );
}
/*-----------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ *prvWaitForStart( ಶೂನ್ಯ * pvParams )
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

    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}
/*-----------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ prvSwitchThread( Thread_t *pxThreadToResume,
                             Thread_t *pxThreadToSuspend )
{
    BaseType_t uxSavedCriticalNestingIDF;
    BaseType_t uxSavedInterruptNesting;
    BaseType_t uxSavedInterruptLevel;

    ಯದಿ ( pxThreadToSuspend != pxThreadToResume )
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
        ಯದಿ ( pxThreadToSuspend->xDying )
        {
            pthread_exit( ರಿಕ್ತ );
        }
        prvSuspendSelf( pxThreadToSuspend );

        uxCriticalNestingIDF = uxSavedCriticalNestingIDF;
        uxInterruptNesting = uxSavedInterruptNesting;
        uxInterruptLevel = uxSavedInterruptLevel;
    }
}
/*-----------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ prvSuspendSelf( Thread_t *thread )
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

ಸ್ಥಿರ ಶೂನ್ಯ prvResumeThread( Thread_t *xThreadId )
{
    ಯದಿ ( pthread_self() != xThreadId->pthread )
    {
        event_signal(xThreadId->ev);
    }
}
/*-----------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ prvSetupSignalsAndSchedulerPolicy( ಶೂನ್ಯ )
{
    ಸಂರಚನಾ sigaction sigresume, sigtick;
    ಪೂರ್ಣಾಙ್ಕ iRet;

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
    (ಶೂನ್ಯ)pthread_sigmask( SIG_SETMASK, &xAllSignals,
                           &xSchedulerOriginalSignalMask );

    /* SIG_RESUME is only used with sigwait() so doesn't need a
       handler. */
    sigresume.sa_flags = 0;
    sigresume.sa_handler = SIG_IGN;
    sigfillset( &sigresume.sa_mask );

    sigtick.sa_flags = 0;
    sigtick.sa_handler = vPortSystemTickHandler;
    sigfillset( &sigtick.sa_mask );

    iRet = sigaction( SIG_RESUME, &sigresume, ರಿಕ್ತ );
    ಯದಿ ( iRet )
    {
        prvFatalError( "sigaction", errno );
    }

    iRet = sigaction( SIGALRM, &sigtick, ರಿಕ್ತ );
    ಯದಿ ( iRet )
    {
        prvFatalError( "sigaction", errno );
    }
}
/*-----------------------------------------------------------*/

unsigned long ulPortGetRunTime( ಶೂನ್ಯ )
{
    ಸಂರಚನಾ tms xTimes;

    times( &xTimes );

    ಪ್ರತ್ಯಾಗಮ ( unsigned long ) xTimes.tms_utime;
}
/*-----------------------------------------------------------*/
bool portVALID_LIST_MEM(ಧ್ರುವ ಶೂನ್ಯ *ptr)
{
    ಪ್ರತ್ಯಾಗಮ true;
}

bool portVALID_TCB_MEM(ಧ್ರುವ ಶೂನ್ಯ *ptr)
{
    ಪ್ರತ್ಯಾಗಮ true;
}

bool port_VALID_ರಾಶಿ_MEM(ಧ್ರುವ ಶೂನ್ಯ *ptr)
{
    ಪ್ರತ್ಯಾಗಮ true;
}
/*-----------------------------------------------------------*/

portMUX_TYPE port_xTaskLock = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE port_xISRLock = portMUX_INITIALIZER_UNLOCKED;

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *TAG = "port";

BaseType_t xPortCheckIfInISR(ಶೂನ್ಯ)
{
    ಪ್ರತ್ಯಾಗಮ (uxInterruptNesting == 0) ? pdFALSE : pdTRUE;
}

ಶೂನ್ಯ app_main(ಶೂನ್ಯ);

ಸ್ಥಿರ ಶೂನ್ಯ main_task(ಶೂನ್ಯ* args)
{
    app_main();
    vTaskDelete(ರಿಕ್ತ);
}

ಪೂರ್ಣಾಙ್ಕ main(ಪೂರ್ಣಾಙ್ಕ argc, ಧ್ರುವ ವರ್ಣ **argv)
{
    // This makes sure that stdio is flushed after each '\n' so that idf.py monitor
    // reads the program output on time.
    setvbuf(stdout, ರಿಕ್ತ, _IOLBF, 0);

    usleep(1000);

    BaseType_t res;

#ಯದಿ ( config_ಅಂಕೆ_CORES > 1 )
    res = xTaskCreateAffinitySet(&main_task, "main",
                                 ESP_TASK_MAIN_ರಾಶಿ, ರಿಕ್ತ,
                                 ESP_TASK_MAIN_PRIO, ESP_TASK_MAIN_CORE, ರಿಕ್ತ);
#ಅನ್ಯಥಾ
    res = xTaskCreate(&main_task, "main",
                      ESP_TASK_MAIN_ರಾಶಿ, ರಿಕ್ತ,
                      ESP_TASK_MAIN_PRIO, ರಿಕ್ತ);
#endif

    ಪ್ರತಿಜ್ಞಾ(res == pdTRUE);
    (ಶೂನ್ಯ)res;

    ESP_LOGI(TAG, "Starting SMP scheduler.");
    vTaskStartScheduler();

    // This line should never be reached
    ಪ್ರತಿಜ್ಞಾ(false);
}

ಶೂನ್ಯ esp_vApplicationIdleHook(ಶೂನ್ಯ)
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

ಶೂನ್ಯ esp_vApplicationTickHook( ಶೂನ್ಯ ) { }

#ಯದಿ  (  configUSE_TICK_HOOK > 0 )
ಶೂನ್ಯ vApplicationTickHook( ಶೂನ್ಯ )
{
    esp_vApplicationTickHook();
}
#endif

#ಯದಿ ( configSUPPORT_STATIC_ALLOCATION == 1 )
/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
 * implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
 * used by the Idle task. */
ಶೂನ್ಯ vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                    ರಾಶಿ_Type_t ** ppx_Idle_Task_ರಾಶಿ_Buffer,
                                    uint32_t * pul_Idle_Task_ರಾಶಿ_Size )
{
/* If the buffers to be provided to the Idle task are declared inside this
 * function then they must be declared static - otherwise they will be allocated on
 * the stack and so not exists after this function exits. */
    ಸ್ಥಿರ StaticTask_t xIdleTaskTCB;
    ಸ್ಥಿರ ರಾಶಿ_Type_t ux_Idle_Task_ರಾಶಿ[ config_MINIMAL_ರಾಶಿ_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
     * state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppx_Idle_Task_ರಾಶಿ_Buffer = ux_Idle_Task_ರಾಶಿ;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in bytes. */
    *pul_Idle_Task_ರಾಶಿ_Size = config_MINIMAL_ರಾಶಿ_SIZE;
}
#endif // configSUPPORT_STATIC_ALLOCATION == 1
/*-----------------------------------------------------------*/

#ಯದಿ ( (configSUPPORT_STATIC_ALLOCATION == 1) && (configUSE_TIMERS == 1))

/* When configSUPPORT_STATIC_ALLOCATION is set to 1 the application writer can
 * use a callback function to optionally provide the memory required by the idle
 * and timer tasks.  This is the stack that will be used by the timer task.  It is
 * declared here, as a global, so it can be checked by a test that is implemented
 * in a different file. */
ರಾಶಿ_Type_t ux_Timer_Task_ರಾಶಿ[ config_TIMER_TASK_ರಾಶಿ_DEPTH ];


/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
 * application must provide an implementation of vApplicationGetTimerTaskMemory()
 * to provide the memory that is used by the Timer service task. */
ಶೂನ್ಯ vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                     ರಾಶಿ_Type_t ** ppx_Timer_Task_ರಾಶಿ_Buffer,
                                     uint32_t * pul_Timer_Task_ರಾಶಿ_Size )
{
/* If the buffers to be provided to the Timer task are declared inside this
 * function then they must be declared static - otherwise they will be allocated on
 * the stack and so not exists after this function exits. */
    ಸ್ಥಿರ StaticTask_t xTimerTaskTCB;

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
     * task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppx_Timer_Task_ರಾಶಿ_Buffer = ux_Timer_Task_ರಾಶಿ;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in bytes. */
    *pul_Timer_Task_ರಾಶಿ_Size = config_TIMER_TASK_ರಾಶಿ_DEPTH;
}
#endif // configSUPPORT_STATIC_ALLOCATION == 1 && (configUSE_TIMERS == 1)

ಶೂನ್ಯ vPortTakeLock( portMUX_TYPE *lock )
{
    spinlock_acquire( lock, portMUX_NO_TIMEOUT);
}

ಶೂನ್ಯ vPortReleaseLock( portMUX_TYPE *lock )
{
    spinlock_release( lock );
}

#define FREERTOS_SMP_MALLOC_CAPS    (MALLOC_CAP_INTERNAL|MALLOC_CAP_8BIT)

ಶೂನ್ಯ *pvPortMalloc( size_t xSize )
{
    ಪ್ರತ್ಯಾಗಮ heap_caps_malloc(xSize, FREERTOS_SMP_MALLOC_CAPS);
}

ಶೂನ್ಯ vPortFree( ಶೂನ್ಯ *pv )
{
    heap_caps_free(pv);
}

ಶೂನ್ಯ __attribute__((weak)) v_Application_ರಾಶಿ_Overflow_Hook(TaskHandle_t xTask, ವರ್ಣ *pcTaskName)
{
#define ERR_STR1 "***ERROR*** A stack overflow in task "
#define ERR_STR2 " has been detected."
    ಧ್ರುವ ವರ್ಣ *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    ವರ್ಣ buf[ಪರಿಮಾಣ(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + ಪರಿಮಾಣ(ERR_STR2) + 1 /* null char */] = {0};

    ವರ್ಣ *dest = buf;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಪರಿಮಾಣ(str) / ಪರಿಮಾಣ(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
	printf("%s\n", buf);
    abort();
}

// ------- Thread Local Storage Pointers Deletion Callbacks -------

#ಯದಿ ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
ಶೂನ್ಯ vPortTLSPointersDelCb( ಶೂನ್ಯ *pxTCB )
{
    /* Typecast pxTCB to StaticTask_t type to access TCB struct members.
     * pvDummy15 corresponds to pvThreadLocalStoragePointers member of the TCB.
     */
    StaticTask_t *tcb = ( StaticTask_t * )pxTCB;

    /* The TLSP deletion callbacks are stored at an offset of (configNUM_THREAD_LOCAL_STORAGE_POINTERS/2) */
    TlsDeleteCallbackFunction_t *pvThreadLocalStoragePointersDelCallback = ( TlsDeleteCallbackFunction_t * )( &( tcb->pvDummy15[ ( config_ಅಂಕೆ_THREAD_LOCAL_STORAGE_POINTERS / 2 ) ] ) );

    /* We need to iterate over half the depth of the pvThreadLocalStoragePointers area
     * to access all TLS pointers and their respective TLS deletion callbacks.
     */
    ಪ್ರತಿ ( ಪೂರ್ಣಾಙ್ಕ x = 0; x < ( config_ಅಂಕೆ_THREAD_LOCAL_STORAGE_POINTERS / 2 ); x++ ) {
        ಯದಿ ( pvThreadLocalStoragePointersDelCallback[ x ] != ರಿಕ್ತ ) {  //If del cb is set
            // We skip the check if the callback is executable as that is difficult to determine for different
            // platforms (compare xtensa and riscv code).
            pvThreadLocalStoragePointersDelCallback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#endif // CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS

ಶೂನ್ಯ vPortCleanUpTCB ( ಶೂನ್ಯ *pxTCB )
{
    #ಯದಿ ( CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK )
        /* Call the user defined task pre-deletion hook */
        ಬಾಹ್ಯ ಶೂನ್ಯ vTaskPreDeletionHook( ಶೂನ್ಯ * pxTCB );
        vTaskPreDeletionHook( pxTCB );
    #endif /* CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK */

#ಯದಿ ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
    /* Call TLS pointers deletion callbacks */
    vPortTLSPointersDelCb( pxTCB );
#endif /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

    vPortCancelThread(pxTCB);
}
