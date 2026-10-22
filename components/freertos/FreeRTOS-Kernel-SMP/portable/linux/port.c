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

#ଅନ୍ତର୍ଭାଵ <errno.h>
#ଅନ୍ତର୍ଭାଵ <pthread.h>
#ଅନ୍ତର୍ଭାଵ <signal.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <sys/time.h>
#ଅନ୍ତର୍ଭାଵ <sys/times.h>
#ଅନ୍ତର୍ଭାଵ <time.h>
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#ଅନ୍ତର୍ଭାଵ <assert.h>

/* Scheduler includes. */
#ଅନ୍ତର୍ଭାଵ "esp_heap_caps.h"
#ଅନ୍ତର୍ଭାଵ "FreeRTOS.h"
#ଅନ୍ତର୍ଭାଵ "task.h"
#ଅନ୍ତର୍ଭାଵ "esp_task.h"
#ଅନ୍ତର୍ଭାଵ "timers.h"
#ଅନ୍ତର୍ଭାଵ "utils/wait_for_event.h"
#ଅନ୍ତର୍ଭାଵ "esp_log.h"
/*-----------------------------------------------------------*/

#define SIG_RESUME SIGUSR1

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା THREAD
{
    pthread_t pthread;
    TaskFunction_t pxCode;
    ଶୂନ୍ୟ *pvParams;
    BaseType_t xDying;
    ସଂରଚନା event *ev;
} Thread_t;

/*
 * The additional per-thread data is stored at the beginning of the
 * task's stack.
 */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ Thread_t *prvGetThreadFromTask(TaskHandle_t xTask)
{
    ଗଦା_Type_t *px_ଶୀର୍ଷ_Of_ଗଦା = *(ଗଦା_Type_t **)xTask;

    ଫେରନ୍ତୁ (Thread_t *)(px_ଶୀର୍ଷ_Of_ଗଦା + 1);
}

/*-----------------------------------------------------------*/

ସ୍ଥିର pthread_once_t hSigSetupThread = PTHREAD_ONCE_INIT;
ସ୍ଥିର sigset_t xAllSignals;
ସ୍ଥିର sigset_t xSchedulerOriginalSignalMask;
ସ୍ଥିର pthread_t hMainThread = ( pthread_t )ଖାଲି;

// These are saved as part of a thread's state in prvSwitchThread()
ସ୍ଥିର ଅସ୍ଥିର BaseType_t uxCriticalNestingIDF = 0;    /* Track nesting calls for IDF style critical sections. FreeRTOS critical section nesting is maintained in the TCB. */
ସ୍ଥିର ଅସ୍ଥିର UBaseType_t uxInterruptNesting = 0;     /* Tracks if we are currently in an interrupt. */
ସ୍ଥିର ଅସ୍ଥିର BaseType_t uxInterruptLevel = 0;        /* Tracks the current level (i.e., interrupt mask) */
/*-----------------------------------------------------------*/

ସ୍ଥିର BaseType_t xSchedulerEnd = pdFALSE;
/*-----------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ prvSetupSignalsAndSchedulerPolicy( ଶୂନ୍ୟ );
ସ୍ଥିର ଶୂନ୍ୟ prvSetupTimerInterrupt( ଶୂନ୍ୟ );
ସ୍ଥିର ଶୂନ୍ୟ *prvWaitForStart( ଶୂନ୍ୟ * pvParams );
ସ୍ଥିର ଶୂନ୍ୟ prvSwitchThread( Thread_t * xThreadToResume,
                             Thread_t *xThreadToSuspend );
ସ୍ଥିର ଶୂନ୍ୟ prvSuspendSelf( Thread_t * thread);
ସ୍ଥିର ଶୂନ୍ୟ prvResumeThread( Thread_t * xThreadId );
ସ୍ଥିର ଶୂନ୍ୟ vPortSystemTickHandler( ପୂର୍ଣ୍ଣସଂଖ୍ୟା sig );
ସ୍ଥିର ଶୂନ୍ୟ vPortStartFirstTask( ଶୂନ୍ୟ );
/*-----------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ prvFatalError( ଅଚଳ ଅକ୍ଷର *pcCall, ପୂର୍ଣ୍ଣସଂଖ୍ୟା iErrno )
{
    fprintf( stderr, "%s: %s\n", pcCall, strerror( iErrno ) );
    abort();
}

/*
 * See header file for description.
 */
ଗଦା_Type_t *px_Port_Initialise_ଗଦା( ଗଦା_Type_t *px_ଶୀର୍ଷ_Of_ଗଦା,
                                    ଗଦା_Type_t *px_End_Of_ଗଦା,
                                    TaskFunction_t pxCode,
                                    ଶୂନ୍ୟ *pvParameters )
{
    Thread_t *thread;
    pthread_attr_t xThreadAttributes;
    size_t ul_ଗଦା_Size;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା iRet;

    (ଶୂନ୍ୟ)pthread_once( &hSigSetupThread, prvSetupSignalsAndSchedulerPolicy );

    /*
     * Store the additional thread data at the start of the stack.
     */
    thread = (Thread_t *)(px_ଶୀର୍ଷ_Of_ଗଦା + 1) - 1;
    px_ଶୀର୍ଷ_Of_ଗଦା = (ଗଦା_Type_t *)thread - 1;
    ul_ଗଦା_Size = (px_ଶୀର୍ଷ_Of_ଗଦା + 1 - px_End_Of_ଗଦା) * ଆକାର(*px_ଶୀର୍ଷ_Of_ଗଦା);

    thread->pxCode = pxCode;
    thread->pvParams = pvParameters;
    thread->xDying = pdFALSE;

    pthread_attr_init( &xThreadAttributes );
    pthread_attr_setstack( &xThreadAttributes, px_End_Of_ଗଦା, ul_ଗଦା_Size );

    thread->ev = event_create();

    BaseType_t prev_intr_level = xPortSetInterruptMask();

    iRet = pthread_create( &thread->pthread, &xThreadAttributes,
                           prvWaitForStart, thread );
    ଯଦି ( iRet )
    {
        prvFatalError( "pthread_create", iRet );
    }

    vPortClearInterruptMask( prev_intr_level );

    ଫେରନ୍ତୁ px_ଶୀର୍ଷ_Of_ଗଦା;
}
/*-----------------------------------------------------------*/

ଶୂନ୍ୟ vPortStartFirstTask( ଶୂନ୍ୟ )
{
    Thread_t *pxFirstThread = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    /* Start the first task. */
    prvResumeThread( pxFirstThread );
}
/*-----------------------------------------------------------*/

/*
 * See header file for description.
 */
BaseType_t xPortStartScheduler( ଶୂନ୍ୟ )
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା iSignal;
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

    ଯେତେବେଳେ ( !xSchedulerEnd )
    {
        sigwait( &xSignals, &iSignal );
    }

    /* Cancel the Idle task and free its resources */
#ଯଦି ( INCLUDE_xTaskGetIdleTaskHandle == 1 )
    vPortCancelThread( xTaskGetIdleTaskHandle() );
#endif

#ଯଦି ( configUSE_TIMERS == 1 )
    /* Cancel the Timer task and free its resources */
    vPortCancelThread( xTimerGetTimerDaemonTaskHandle() );
#endif /* configUSE_TIMERS */

    /* Restore original signal mask. */
    (ଶୂନ୍ୟ)pthread_sigmask( SIG_SETMASK, &xSchedulerOriginalSignalMask,  ଖାଲି );

    ଫେରନ୍ତୁ 0;
}
/*-----------------------------------------------------------*/

ଶୂନ୍ୟ vPortEndScheduler( ଶୂନ୍ୟ )
{
    ସଂରଚନା itimerval itimer;
    ସଂରଚନା sigaction sigtick;
    Thread_t *xCurrentThread;

    /* Stop the timer and ignore any pending SIGALRMs that would end
     * up running on the main thread when it is resumed. */
    itimer.it_value.tv_sec = 0;
    itimer.it_value.tv_usec = 0;

    itimer.it_interval.tv_sec = 0;
    itimer.it_interval.tv_usec = 0;
    (ଶୂନ୍ୟ)setitimer( ITIMER_REAL, &itimer, ଖାଲି );

    sigtick.sa_flags = 0;
    sigtick.sa_handler = SIG_IGN;
    sigemptyset( &sigtick.sa_mask );
    sigaction( SIGALRM, &sigtick, ଖାଲି );

    /* Signal the scheduler to exit its loop. */
    xSchedulerEnd = pdTRUE;
    (ଶୂନ୍ୟ)pthread_kill( hMainThread, SIG_RESUME );

    xCurrentThread = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );
    prvSuspendSelf(xCurrentThread);
}
/*-----------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ vPortDisableInterrupts( ଶୂନ୍ୟ )
{
    pthread_sigmask( SIG_BLOCK, &xAllSignals, ଖାଲି );
}
/*-----------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ vPortEnableInterrupts( ଶୂନ୍ୟ )
{
    pthread_sigmask( SIG_UNBLOCK, &xAllSignals, ଖାଲି );
}
/*-----------------------------------------------------------*/

ଶୂନ୍ୟ vPortEnterCriticalIDF( ଶୂନ୍ୟ )
{
    ଯଦି ( uxCriticalNestingIDF == 0 && uxInterruptLevel == 0)
    {
        vPortDisableInterrupts();
    }
    uxCriticalNestingIDF++;
}
/*-----------------------------------------------------------*/

ଶୂନ୍ୟ vPortExitCriticalIDF( ଶୂନ୍ୟ )
{
    uxCriticalNestingIDF--;

    /* If we have reached 0 then re-enable the interrupts. */
    ଯଦି( uxCriticalNestingIDF == 0 && uxInterruptLevel == 0)
    {
        vPortEnableInterrupts();
    }
}
/*-----------------------------------------------------------*/

ଶୂନ୍ୟ vPortYieldFromISR( ଶୂନ୍ୟ )
{
    Thread_t *xThreadToSuspend;
    Thread_t *xThreadToResume;

    xThreadToSuspend = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    vTaskSwitchContext();

    xThreadToResume = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    prvSwitchThread( xThreadToResume, xThreadToSuspend );
}
/*-----------------------------------------------------------*/

ଶୂନ୍ୟ vPortYield( ଶୂନ୍ୟ )
{
    BaseType_t prev_intr_level = xPortSetInterruptMask();

    vPortYieldFromISR();

    vPortClearInterruptMask( prev_intr_level );
}
/*-----------------------------------------------------------*/

/* In SMP code, the disable/enable interrupt macros are calling the set/get interrupt mask functions below.
   Hence, we need to call vPortDisableInterrupts() and vPortEnableInterrupts(), otherwise interrupts
   are never disabled/enabled. */

BaseType_t xPortSetInterruptMask( ଶୂନ୍ୟ )
{
    ଯଦି (uxInterruptLevel == 0 && uxCriticalNestingIDF == 0) {
        vPortDisableInterrupts();
    }
    BaseType_t prev_intr_level = uxInterruptLevel;
    uxInterruptLevel++;
    ଫେରନ୍ତୁ prev_intr_level;
}
/*-----------------------------------------------------------*/

ଶୂନ୍ୟ vPortClearInterruptMask( BaseType_t xMask )
{
    // Only re-enable interrupts if xMask is 0
    uxInterruptLevel = xMask;
    ଯଦି (uxInterruptLevel == 0 && uxCriticalNestingIDF == 0) {
        vPortEnableInterrupts();
    }
}
/*-----------------------------------------------------------*/

ସ୍ଥିର uint64_t prvGetTimeNs(ଶୂନ୍ୟ)
{
    ସଂରଚନା timespec t;

    clock_gettime(CLOCK_MONOTONIC, &t);

    ଫେରନ୍ତୁ t.tv_sec * 1000000000ull + t.tv_nsec;
}

ସ୍ଥିର uint64_t prvStartTimeNs;
/* commented as part of the code below in vPortSystemTickHandler,
 * to adjust timing according to full demo requirements */
/* static uint64_t prvTickCount; */

/*
 * Setup the systick timer to generate the tick interrupts at the required
 * frequency.
 */
ଶୂନ୍ୟ prvSetupTimerInterrupt( ଶୂନ୍ୟ )
{
    ସଂରଚନା itimerval itimer;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା iRet;

    /* Initialise the structure with the current timer information. */
    iRet = getitimer( ITIMER_REAL, &itimer );
    ଯଦି ( iRet )
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
    iRet = setitimer( ITIMER_REAL, &itimer, ଖାଲି );
    ଯଦି ( iRet )
    {
        prvFatalError( "setitimer", errno );
    }

    prvStartTimeNs = prvGetTimeNs();
}
/*-----------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ vPortSystemTickHandler( ପୂର୍ଣ୍ଣସଂଖ୍ୟା sig )
{
    Thread_t *pxThreadToSuspend;
    Thread_t *pxThreadToResume;
    BaseType_t xSwitchRequired;
    /* uint64_t xExpectedTicks; */

    // Handling a timer signal, so we are currently in an interrupt.
    uxInterruptNesting++;

#ଯଦି ( configUSE_PREEMPTION == 1 )
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

#ଯଦି ( configUSE_PREEMPTION == 1 )
    ଯଦି (xSwitchRequired == pdTRUE) {
        /* Select Next Task. */
        vTaskSwitchContext();

        pxThreadToResume = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

        prvSwitchThread(pxThreadToResume, pxThreadToSuspend);
    }
#ଅନ୍ୟଥା
    (ଶୂନ୍ୟ)xSwitchRequired;
#endif

    // Returning from the timer signal handler, so we are exiting the interrupt.
    uxInterruptNesting--;
}
/*-----------------------------------------------------------*/

ଶୂନ୍ୟ vPortThreadDying( ଶୂନ୍ୟ *pxTaskToDelete, ଅସ୍ଥିର BaseType_t *pxPendYield )
{
    Thread_t *pxThread = prvGetThreadFromTask( pxTaskToDelete );

    pxThread->xDying = pdTRUE;
}

ଶୂନ୍ୟ vPortCancelThread( ଶୂନ୍ୟ *pxTaskToDelete )
{
    Thread_t *pxThreadToCancel = prvGetThreadFromTask( pxTaskToDelete );

    /*
     * The thread has already been suspended so it can be safely cancelled.
     */
    pthread_cancel( pxThreadToCancel->pthread );
    pthread_join( pxThreadToCancel->pthread, ଖାଲି );
    event_delete( pxThreadToCancel->ev );
}
/*-----------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ *prvWaitForStart( ଶୂନ୍ୟ * pvParams )
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

    ଫେରନ୍ତୁ ଖାଲି;
}
/*-----------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ prvSwitchThread( Thread_t *pxThreadToResume,
                             Thread_t *pxThreadToSuspend )
{
    BaseType_t uxSavedCriticalNestingIDF;
    BaseType_t uxSavedInterruptNesting;
    BaseType_t uxSavedInterruptLevel;

    ଯଦି ( pxThreadToSuspend != pxThreadToResume )
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
        ଯଦି ( pxThreadToSuspend->xDying )
        {
            pthread_exit( ଖାଲି );
        }
        prvSuspendSelf( pxThreadToSuspend );

        uxCriticalNestingIDF = uxSavedCriticalNestingIDF;
        uxInterruptNesting = uxSavedInterruptNesting;
        uxInterruptLevel = uxSavedInterruptLevel;
    }
}
/*-----------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ prvSuspendSelf( Thread_t *thread )
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

ସ୍ଥିର ଶୂନ୍ୟ prvResumeThread( Thread_t *xThreadId )
{
    ଯଦି ( pthread_self() != xThreadId->pthread )
    {
        event_signal(xThreadId->ev);
    }
}
/*-----------------------------------------------------------*/

ସ୍ଥିର ଶୂନ୍ୟ prvSetupSignalsAndSchedulerPolicy( ଶୂନ୍ୟ )
{
    ସଂରଚନା sigaction sigresume, sigtick;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା iRet;

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
    (ଶୂନ୍ୟ)pthread_sigmask( SIG_SETMASK, &xAllSignals,
                           &xSchedulerOriginalSignalMask );

    /* SIG_RESUME is only used with sigwait() so doesn't need a
       handler. */
    sigresume.sa_flags = 0;
    sigresume.sa_handler = SIG_IGN;
    sigfillset( &sigresume.sa_mask );

    sigtick.sa_flags = 0;
    sigtick.sa_handler = vPortSystemTickHandler;
    sigfillset( &sigtick.sa_mask );

    iRet = sigaction( SIG_RESUME, &sigresume, ଖାଲି );
    ଯଦି ( iRet )
    {
        prvFatalError( "sigaction", errno );
    }

    iRet = sigaction( SIGALRM, &sigtick, ଖାଲି );
    ଯଦି ( iRet )
    {
        prvFatalError( "sigaction", errno );
    }
}
/*-----------------------------------------------------------*/

unsigned long ulPortGetRunTime( ଶୂନ୍ୟ )
{
    ସଂରଚନା tms xTimes;

    times( &xTimes );

    ଫେରନ୍ତୁ ( unsigned long ) xTimes.tms_utime;
}
/*-----------------------------------------------------------*/
bool portVALID_LIST_MEM(ଅଚଳ ଶୂନ୍ୟ *ptr)
{
    ଫେରନ୍ତୁ true;
}

bool portVALID_TCB_MEM(ଅଚଳ ଶୂନ୍ୟ *ptr)
{
    ଫେରନ୍ତୁ true;
}

bool port_VALID_ଗଦା_MEM(ଅଚଳ ଶୂନ୍ୟ *ptr)
{
    ଫେରନ୍ତୁ true;
}
/*-----------------------------------------------------------*/

portMUX_TYPE port_xTaskLock = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE port_xISRLock = portMUX_INITIALIZER_UNLOCKED;

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *TAG = "port";

BaseType_t xPortCheckIfInISR(ଶୂନ୍ୟ)
{
    ଫେରନ୍ତୁ (uxInterruptNesting == 0) ? pdFALSE : pdTRUE;
}

ଶୂନ୍ୟ app_main(ଶୂନ୍ୟ);

ସ୍ଥିର ଶୂନ୍ୟ main_task(ଶୂନ୍ୟ* args)
{
    app_main();
    vTaskDelete(ଖାଲି);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୁଖ୍ୟ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅଚଳ ଅକ୍ଷର **argv)
{
    // This makes sure that stdio is flushed after each '\n' so that idf.py monitor
    // reads the program output on time.
    setvbuf(stdout, ଖାଲି, _IOLBF, 0);

    usleep(1000);

    BaseType_t res;

#ଯଦି ( config_ଅଙ୍କ_CORES > 1 )
    res = xTaskCreateAffinitySet(&main_task, "main",
                                 ESP_TASK_MAIN_ଗଦା, ଖାଲି,
                                 ESP_TASK_MAIN_PRIO, ESP_TASK_MAIN_CORE, ଖାଲି);
#ଅନ୍ୟଥା
    res = xTaskCreate(&main_task, "main",
                      ESP_TASK_MAIN_ଗଦା, ଖାଲି,
                      ESP_TASK_MAIN_PRIO, ଖାଲି);
#endif

    ପ୍ରତିଜ୍ଞା(res == pdTRUE);
    (ଶୂନ୍ୟ)res;

    ESP_LOGI(TAG, "Starting SMP scheduler.");
    vTaskStartScheduler();

    // This line should never be reached
    ପ୍ରତିଜ୍ଞା(false);
}

ଶୂନ୍ୟ esp_vApplicationIdleHook(ଶୂନ୍ୟ)
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

ଶୂନ୍ୟ esp_vApplicationTickHook( ଶୂନ୍ୟ ) { }

#ଯଦି  (  configUSE_TICK_HOOK > 0 )
ଶୂନ୍ୟ vApplicationTickHook( ଶୂନ୍ୟ )
{
    esp_vApplicationTickHook();
}
#endif

#ଯଦି ( configSUPPORT_STATIC_ALLOCATION == 1 )
/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
 * implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
 * used by the Idle task. */
ଶୂନ୍ୟ vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                    ଗଦା_Type_t ** ppx_Idle_Task_ଗଦା_Buffer,
                                    uint32_t * pul_Idle_Task_ଗଦା_Size )
{
/* If the buffers to be provided to the Idle task are declared inside this
 * function then they must be declared static - otherwise they will be allocated on
 * the stack and so not exists after this function exits. */
    ସ୍ଥିର StaticTask_t xIdleTaskTCB;
    ସ୍ଥିର ଗଦା_Type_t ux_Idle_Task_ଗଦା[ config_MINIMAL_ଗଦା_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
     * state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppx_Idle_Task_ଗଦା_Buffer = ux_Idle_Task_ଗଦା;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in bytes. */
    *pul_Idle_Task_ଗଦା_Size = config_MINIMAL_ଗଦା_SIZE;
}
#endif // configSUPPORT_STATIC_ALLOCATION == 1
/*-----------------------------------------------------------*/

#ଯଦି ( (configSUPPORT_STATIC_ALLOCATION == 1) && (configUSE_TIMERS == 1))

/* When configSUPPORT_STATIC_ALLOCATION is set to 1 the application writer can
 * use a callback function to optionally provide the memory required by the idle
 * and timer tasks.  This is the stack that will be used by the timer task.  It is
 * declared here, as a global, so it can be checked by a test that is implemented
 * in a different file. */
ଗଦା_Type_t ux_Timer_Task_ଗଦା[ config_TIMER_TASK_ଗଦା_DEPTH ];


/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
 * application must provide an implementation of vApplicationGetTimerTaskMemory()
 * to provide the memory that is used by the Timer service task. */
ଶୂନ୍ୟ vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                     ଗଦା_Type_t ** ppx_Timer_Task_ଗଦା_Buffer,
                                     uint32_t * pul_Timer_Task_ଗଦା_Size )
{
/* If the buffers to be provided to the Timer task are declared inside this
 * function then they must be declared static - otherwise they will be allocated on
 * the stack and so not exists after this function exits. */
    ସ୍ଥିର StaticTask_t xTimerTaskTCB;

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
     * task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppx_Timer_Task_ଗଦା_Buffer = ux_Timer_Task_ଗଦା;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in bytes. */
    *pul_Timer_Task_ଗଦା_Size = config_TIMER_TASK_ଗଦା_DEPTH;
}
#endif // configSUPPORT_STATIC_ALLOCATION == 1 && (configUSE_TIMERS == 1)

ଶୂନ୍ୟ vPortTakeLock( portMUX_TYPE *lock )
{
    spinlock_acquire( lock, portMUX_NO_TIMEOUT);
}

ଶୂନ୍ୟ vPortReleaseLock( portMUX_TYPE *lock )
{
    spinlock_release( lock );
}

#define FREERTOS_SMP_MALLOC_CAPS    (MALLOC_CAP_INTERNAL|MALLOC_CAP_8BIT)

ଶୂନ୍ୟ *pvPortMalloc( size_t xSize )
{
    ଫେରନ୍ତୁ heap_caps_malloc(xSize, FREERTOS_SMP_MALLOC_CAPS);
}

ଶୂନ୍ୟ vPortFree( ଶୂନ୍ୟ *pv )
{
    heap_caps_free(pv);
}

ଶୂନ୍ୟ __attribute__((weak)) v_Application_ଗଦା_Overflow_Hook(TaskHandle_t xTask, ଅକ୍ଷର *pcTaskName)
{
#define ERR_STR1 "***ERROR*** A stack overflow in task "
#define ERR_STR2 " has been detected."
    ଅଚଳ ଅକ୍ଷର *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    ଅକ୍ଷର buf[ଆକାର(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + ଆକାର(ERR_STR2) + 1 /* null char */] = {0};

    ଅକ୍ଷର *dest = buf;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଆକାର(str) / ଆକାର(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
	printf("%s\n", buf);
    abort();
}

// ------- Thread Local Storage Pointers Deletion Callbacks -------

#ଯଦି ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
ଶୂନ୍ୟ vPortTLSPointersDelCb( ଶୂନ୍ୟ *pxTCB )
{
    /* Typecast pxTCB to StaticTask_t type to access TCB struct members.
     * pvDummy15 corresponds to pvThreadLocalStoragePointers member of the TCB.
     */
    StaticTask_t *tcb = ( StaticTask_t * )pxTCB;

    /* The TLSP deletion callbacks are stored at an offset of (configNUM_THREAD_LOCAL_STORAGE_POINTERS/2) */
    TlsDeleteCallbackFunction_t *pvThreadLocalStoragePointersDelCallback = ( TlsDeleteCallbackFunction_t * )( &( tcb->pvDummy15[ ( config_ଅଙ୍କ_THREAD_LOCAL_STORAGE_POINTERS / 2 ) ] ) );

    /* We need to iterate over half the depth of the pvThreadLocalStoragePointers area
     * to access all TLS pointers and their respective TLS deletion callbacks.
     */
    ପାଇଁ ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0; x < ( config_ଅଙ୍କ_THREAD_LOCAL_STORAGE_POINTERS / 2 ); x++ ) {
        ଯଦି ( pvThreadLocalStoragePointersDelCallback[ x ] != ଖାଲି ) {  //If del cb is set
            // We skip the check if the callback is executable as that is difficult to determine for different
            // platforms (compare xtensa and riscv code).
            pvThreadLocalStoragePointersDelCallback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#endif // CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS

ଶୂନ୍ୟ vPortCleanUpTCB ( ଶୂନ୍ୟ *pxTCB )
{
    #ଯଦି ( CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK )
        /* Call the user defined task pre-deletion hook */
        ବାହ୍ୟ ଶୂନ୍ୟ vTaskPreDeletionHook( ଶୂନ୍ୟ * pxTCB );
        vTaskPreDeletionHook( pxTCB );
    #endif /* CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK */

#ଯଦି ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
    /* Call TLS pointers deletion callbacks */
    vPortTLSPointersDelCb( pxTCB );
#endif /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

    vPortCancelThread(pxTCB);
}
