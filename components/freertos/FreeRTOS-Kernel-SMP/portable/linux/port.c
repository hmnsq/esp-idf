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

#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <pthread.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <signal.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/times.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <assert.h>

/* Scheduler includes. */
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_heap_caps.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "timers.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils/wait_for_event.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_log.h"
/*-----------------------------------------------------------*/

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SIG_RESUME SIGUSR1

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ THREAD
{
    pthread_t pthread;
    TaskFunction_t px_ਕੋਡ;
    ਖਾਲੀ *pvParams;
    ਅਧਾਰ_Type_t xDying;
    ਬਣਤਰ event *ev;
} Thread_t;

/*
 * The additional per-thread data is stored at the beginning of the
 * task's stack.
 */
ਸਥਿਰ ਸਤਰਅੰਦਰ Thread_t *prvGetThreadFromTask(TaskHandle_t xTask)
{
    ਢੇਰ_Type_t *px_ਸਿਖਰ_ਦਾ_ਢੇਰ = *(ਢੇਰ_Type_t **)xTask;

    ਵਾਪਸ (Thread_t *)(px_ਸਿਖਰ_ਦਾ_ਢੇਰ + 1);
}

/*-----------------------------------------------------------*/

ਸਥਿਰ pthread_once_t h_Sig_ਤਿਆਰੀ_Thread = PTHREAD_ONCE_INIT;
ਸਥਿਰ sigset_t x_All_ਸੰਕੇਤ;
ਸਥਿਰ sigset_t x_Scheduler_Original_Signal_ਨਕਾਬ;
ਸਥਿਰ pthread_t hMainThread = ( pthread_t )ਨਿਰਾ;

// These are saved as part of a thread's state in prvSwitchThread()
ਸਥਿਰ ਅਸਥਿਰ ਅਧਾਰ_Type_t uxCriticalNestingIDF = 0;    /* Track nesting calls for IDF style critical sections. FreeRTOS critical section nesting is maintained in the TCB. */
ਸਥਿਰ ਅਸਥਿਰ UBaseType_t ux_ਰੁਕਾਵਟ_Nesting = 0;     /* Tracks if we are currently in an interrupt. */
ਸਥਿਰ ਅਸਥਿਰ ਅਧਾਰ_Type_t ux_ਰੁਕਾਵਟ_Level = 0;        /* Tracks the current level (i.e., interrupt mask) */
/*-----------------------------------------------------------*/

ਸਥਿਰ ਅਧਾਰ_Type_t x_Scheduler_ਸਿਰਾ = pdFALSE;
/*-----------------------------------------------------------*/

ਸਥਿਰ ਖਾਲੀ prv_ਤਿਆਰੀ_ਸੰਕੇਤ_And_Scheduler_Policy( ਖਾਲੀ );
ਸਥਿਰ ਖਾਲੀ prv_ਤਿਆਰੀ_Timer_ਰੁਕਾਵਟ( ਖਾਲੀ );
ਸਥਿਰ ਖਾਲੀ *prv_Wait_For_ਸ਼ੁਰੂ( ਖਾਲੀ * pvParams );
ਸਥਿਰ ਖਾਲੀ prvSwitchThread( Thread_t * x_Thread_ਮੰਜ਼ਿਲ_Resume,
                             Thread_t *x_Thread_ਮੰਜ਼ਿਲ_Suspend );
ਸਥਿਰ ਖਾਲੀ prvSuspendSelf( Thread_t * thread);
ਸਥਿਰ ਖਾਲੀ prvResumeThread( Thread_t * xThreadId );
ਸਥਿਰ ਖਾਲੀ v_ਪੋਰਟ_System_Tick_Handler( ਪੂਰਨ sig );
ਸਥਿਰ ਖਾਲੀ v_ਪੋਰਟ_ਸ਼ੁਰੂ_ਪਹਿਲਾ_Task( ਖਾਲੀ );
/*-----------------------------------------------------------*/

ਸਥਿਰ ਖਾਲੀ prvFatalError( ਅਚਲ ਅੱਖਰ *pcCall, ਪੂਰਨ iErrno )
{
    fprintf( stderr, "%s: %s\n", pcCall, strerror( iErrno ) );
    abort();
}

/*
 * See header file for description.
 */
ਢੇਰ_Type_t *px_ਪੋਰਟ_Initialise_ਢੇਰ( ਢੇਰ_Type_t *px_ਸਿਖਰ_ਦਾ_ਢੇਰ,
                                    ਢੇਰ_Type_t *px_ਸਿਰਾ_ਦਾ_ਢੇਰ,
                                    TaskFunction_t px_ਕੋਡ,
                                    ਖਾਲੀ *pvParameters )
{
    Thread_t *thread;
    pthread_attr_t x_Thread_ਗੁਣ;
    size_t ul_ਢੇਰ_ਮਾਪ;
    ਪੂਰਨ iRet;

    (ਖਾਲੀ)pthread_once( &h_Sig_ਤਿਆਰੀ_Thread, prv_ਤਿਆਰੀ_ਸੰਕੇਤ_And_Scheduler_Policy );

    /*
     * Store the additional thread data at the start of the stack.
     */
    thread = (Thread_t *)(px_ਸਿਖਰ_ਦਾ_ਢੇਰ + 1) - 1;
    px_ਸਿਖਰ_ਦਾ_ਢੇਰ = (ਢੇਰ_Type_t *)thread - 1;
    ul_ਢੇਰ_ਮਾਪ = (px_ਸਿਖਰ_ਦਾ_ਢੇਰ + 1 - px_ਸਿਰਾ_ਦਾ_ਢੇਰ) * ਆਕਾਰ(*px_ਸਿਖਰ_ਦਾ_ਢੇਰ);

    thread->px_ਕੋਡ = px_ਕੋਡ;
    thread->pvParams = pvParameters;
    thread->xDying = pdFALSE;

    pthread_attr_init( &x_Thread_ਗੁਣ );
    pthread_attr_setstack( &x_Thread_ਗੁਣ, px_ਸਿਰਾ_ਦਾ_ਢੇਰ, ul_ਢੇਰ_ਮਾਪ );

    thread->ev = event_create();

    ਅਧਾਰ_Type_t prev_intr_level = x_ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ();

    iRet = pthread_create( &thread->pthread, &x_Thread_ਗੁਣ,
                           prv_Wait_For_ਸ਼ੁਰੂ, thread );
    ਜੇ ( iRet )
    {
        prvFatalError( "pthread_create", iRet );
    }

    v_ਪੋਰਟ_Clear_ਰੁਕਾਵਟ_ਨਕਾਬ( prev_intr_level );

    ਵਾਪਸ px_ਸਿਖਰ_ਦਾ_ਢੇਰ;
}
/*-----------------------------------------------------------*/

ਖਾਲੀ v_ਪੋਰਟ_ਸ਼ੁਰੂ_ਪਹਿਲਾ_Task( ਖਾਲੀ )
{
    Thread_t *px_ਪਹਿਲਾ_Thread = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    /* Start the first task. */
    prvResumeThread( px_ਪਹਿਲਾ_Thread );
}
/*-----------------------------------------------------------*/

/*
 * See header file for description.
 */
ਅਧਾਰ_Type_t x_ਪੋਰਟ_ਸ਼ੁਰੂ_Scheduler( ਖਾਲੀ )
{
    ਪੂਰਨ iSignal;
    sigset_t x_ਸੰਕੇਤ;

    hMainThread = pthread_self();

    /* Start the timer that generates the tick ISR(SIGALRM).
       Interrupts are disabled here already. */
    prv_ਤਿਆਰੀ_Timer_ਰੁਕਾਵਟ();

    /* Start the first task. */
    v_ਪੋਰਟ_ਸ਼ੁਰੂ_ਪਹਿਲਾ_Task();

    /* Wait until signaled by vPortEndScheduler(). */
    sigemptyset( &x_ਸੰਕੇਤ );
    sigaddset( &x_ਸੰਕੇਤ, SIG_RESUME );

    ਜਦੋਂ ( !x_Scheduler_ਸਿਰਾ )
    {
        sigwait( &x_ਸੰਕੇਤ, &iSignal );
    }

    /* Cancel the Idle task and free its resources */
#ਜੇ ( INCLUDE_xTaskGetIdleTaskHandle == 1 )
    v_ਪੋਰਟ_Cancel_Thread( xTaskGetIdleTaskHandle() );
#ਜੇ_ਅੰਤ

#ਜੇ ( configUSE_TIMERS == 1 )
    /* Cancel the Timer task and free its resources */
    v_ਪੋਰਟ_Cancel_Thread( xTimerGetTimerDaemonTaskHandle() );
#ਜੇ_ਅੰਤ /* configUSE_TIMERS */

    /* Restore original signal mask. */
    (ਖਾਲੀ)pthread_sigmask( SIG_SETMASK, &x_Scheduler_Original_Signal_ਨਕਾਬ,  ਨਿਰਾ );

    ਵਾਪਸ 0;
}
/*-----------------------------------------------------------*/

ਖਾਲੀ v_ਪੋਰਟ_ਸਿਰਾ_Scheduler( ਖਾਲੀ )
{
    ਬਣਤਰ itimerval itimer;
    ਬਣਤਰ sigaction sigtick;
    Thread_t *xCurrentThread;

    /* Stop the timer and ignore any pending SIGALRMs that would end
     * up running on the main thread when it is resumed. */
    itimer.it_value.tv_sec = 0;
    itimer.it_value.tv_usec = 0;

    itimer.it_interval.tv_sec = 0;
    itimer.it_interval.tv_usec = 0;
    (ਖਾਲੀ)setitimer( ITIMER_REAL, &itimer, ਨਿਰਾ );

    sigtick.sa_flags = 0;
    sigtick.sa_handler = SIG_IGN;
    sigemptyset( &sigtick.sa_mask );
    sigaction( SIGALRM, &sigtick, ਨਿਰਾ );

    /* Signal the scheduler to exit its loop. */
    x_Scheduler_ਸਿਰਾ = pdTRUE;
    (ਖਾਲੀ)pthread_kill( hMainThread, SIG_RESUME );

    xCurrentThread = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );
    prvSuspendSelf(xCurrentThread);
}
/*-----------------------------------------------------------*/

ਸਥਿਰ ਖਾਲੀ v_ਪੋਰਟ_Disable_Interrupts( ਖਾਲੀ )
{
    pthread_sigmask( SIG_BLOCK, &x_All_ਸੰਕੇਤ, ਨਿਰਾ );
}
/*-----------------------------------------------------------*/

ਸਥਿਰ ਖਾਲੀ v_ਪੋਰਟ_Enable_Interrupts( ਖਾਲੀ )
{
    pthread_sigmask( SIG_UNBLOCK, &x_All_ਸੰਕੇਤ, ਨਿਰਾ );
}
/*-----------------------------------------------------------*/

ਖਾਲੀ v_ਪੋਰਟ_Enter_Critical_IDF( ਖਾਲੀ )
{
    ਜੇ ( uxCriticalNestingIDF == 0 && ux_ਰੁਕਾਵਟ_Level == 0)
    {
        v_ਪੋਰਟ_Disable_Interrupts();
    }
    uxCriticalNestingIDF++;
}
/*-----------------------------------------------------------*/

ਖਾਲੀ v_ਪੋਰਟ_Exit_Critical_IDF( ਖਾਲੀ )
{
    uxCriticalNestingIDF--;

    /* If we have reached 0 then re-enable the interrupts. */
    ਜੇ( uxCriticalNestingIDF == 0 && ux_ਰੁਕਾਵਟ_Level == 0)
    {
        v_ਪੋਰਟ_Enable_Interrupts();
    }
}
/*-----------------------------------------------------------*/

ਖਾਲੀ v_ਪੋਰਟ_Yield_From_ISR( ਖਾਲੀ )
{
    Thread_t *x_Thread_ਮੰਜ਼ਿਲ_Suspend;
    Thread_t *x_Thread_ਮੰਜ਼ਿਲ_Resume;

    x_Thread_ਮੰਜ਼ਿਲ_Suspend = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    vTaskSwitchContext();

    x_Thread_ਮੰਜ਼ਿਲ_Resume = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    prvSwitchThread( x_Thread_ਮੰਜ਼ਿਲ_Resume, x_Thread_ਮੰਜ਼ਿਲ_Suspend );
}
/*-----------------------------------------------------------*/

ਖਾਲੀ v_ਪੋਰਟ_Yield( ਖਾਲੀ )
{
    ਅਧਾਰ_Type_t prev_intr_level = x_ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ();

    v_ਪੋਰਟ_Yield_From_ISR();

    v_ਪੋਰਟ_Clear_ਰੁਕਾਵਟ_ਨਕਾਬ( prev_intr_level );
}
/*-----------------------------------------------------------*/

/* In SMP code, the disable/enable interrupt macros are calling the set/get interrupt mask functions below.
   Hence, we need to call vPortDisableInterrupts() and vPortEnableInterrupts(), otherwise interrupts
   are never disabled/enabled. */

ਅਧਾਰ_Type_t x_ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ( ਖਾਲੀ )
{
    ਜੇ (ux_ਰੁਕਾਵਟ_Level == 0 && uxCriticalNestingIDF == 0) {
        v_ਪੋਰਟ_Disable_Interrupts();
    }
    ਅਧਾਰ_Type_t prev_intr_level = ux_ਰੁਕਾਵਟ_Level;
    ux_ਰੁਕਾਵਟ_Level++;
    ਵਾਪਸ prev_intr_level;
}
/*-----------------------------------------------------------*/

ਖਾਲੀ v_ਪੋਰਟ_Clear_ਰੁਕਾਵਟ_ਨਕਾਬ( ਅਧਾਰ_Type_t x_ਨਕਾਬ )
{
    // Only re-enable interrupts if xMask is 0
    ux_ਰੁਕਾਵਟ_Level = x_ਨਕਾਬ;
    ਜੇ (ux_ਰੁਕਾਵਟ_Level == 0 && uxCriticalNestingIDF == 0) {
        v_ਪੋਰਟ_Enable_Interrupts();
    }
}
/*-----------------------------------------------------------*/

ਸਥਿਰ uint64_t prv_Get_ਸਮਾਂ_Ns(ਖਾਲੀ)
{
    ਬਣਤਰ timespec t;

    clock_gettime(CLOCK_MONOTONIC, &t);

    ਵਾਪਸ t.tv_sec * 1000000000ull + t.tv_nsec;
}

ਸਥਿਰ uint64_t prv_ਸ਼ੁਰੂ_ਸਮਾਂ_Ns;
/* commented as part of the code below in vPortSystemTickHandler,
 * to adjust timing according to full demo requirements */
/* static uint64_t prvTickCount; */

/*
 * Setup the systick timer to generate the tick interrupts at the required
 * frequency.
 */
ਖਾਲੀ prv_ਤਿਆਰੀ_Timer_ਰੁਕਾਵਟ( ਖਾਲੀ )
{
    ਬਣਤਰ itimerval itimer;
    ਪੂਰਨ iRet;

    /* Initialise the structure with the current timer information. */
    iRet = getitimer( ITIMER_REAL, &itimer );
    ਜੇ ( iRet )
    {
        prvFatalError( "getitimer", errno );
    }

    /* Set the interval between timer events. */
    itimer.it_interval.tv_sec = 0;
    itimer.it_interval.tv_usec = ਪੋਰਟ_TICK_ਦਰ_MICROSECONDS;

    /* Set the current count-down. */
    itimer.it_value.tv_sec = 0;
    itimer.it_value.tv_usec = ਪੋਰਟ_TICK_ਦਰ_MICROSECONDS;

    /* Set-up the timer interrupt. */
    iRet = setitimer( ITIMER_REAL, &itimer, ਨਿਰਾ );
    ਜੇ ( iRet )
    {
        prvFatalError( "setitimer", errno );
    }

    prv_ਸ਼ੁਰੂ_ਸਮਾਂ_Ns = prv_Get_ਸਮਾਂ_Ns();
}
/*-----------------------------------------------------------*/

ਸਥਿਰ ਖਾਲੀ v_ਪੋਰਟ_System_Tick_Handler( ਪੂਰਨ sig )
{
    Thread_t *px_Thread_ਮੰਜ਼ਿਲ_Suspend;
    Thread_t *px_Thread_ਮੰਜ਼ਿਲ_Resume;
    ਅਧਾਰ_Type_t xSwitchRequired;
    /* uint64_t xExpectedTicks; */

    // Handling a timer signal, so we are currently in an interrupt.
    ux_ਰੁਕਾਵਟ_Nesting++;

#ਜੇ ( configUSE_PREEMPTION == 1 )
    px_Thread_ਮੰਜ਼ਿਲ_Suspend = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );
#ਜੇ_ਅੰਤ

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

#ਜੇ ( configUSE_PREEMPTION == 1 )
    ਜੇ (xSwitchRequired == pdTRUE) {
        /* Select Next Task. */
        vTaskSwitchContext();

        px_Thread_ਮੰਜ਼ਿਲ_Resume = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

        prvSwitchThread(px_Thread_ਮੰਜ਼ਿਲ_Resume, px_Thread_ਮੰਜ਼ਿਲ_Suspend);
    }
#ਨਹੀਂਤਾਂ
    (ਖਾਲੀ)xSwitchRequired;
#ਜੇ_ਅੰਤ

    // Returning from the timer signal handler, so we are exiting the interrupt.
    ux_ਰੁਕਾਵਟ_Nesting--;
}
/*-----------------------------------------------------------*/

ਖਾਲੀ v_ਪੋਰਟ_Thread_Dying( ਖਾਲੀ *px_Task_ਮੰਜ਼ਿਲ_Delete, ਅਸਥਿਰ ਅਧਾਰ_Type_t *pxPendYield )
{
    Thread_t *pxThread = prvGetThreadFromTask( px_Task_ਮੰਜ਼ਿਲ_Delete );

    pxThread->xDying = pdTRUE;
}

ਖਾਲੀ v_ਪੋਰਟ_Cancel_Thread( ਖਾਲੀ *px_Task_ਮੰਜ਼ਿਲ_Delete )
{
    Thread_t *px_Thread_ਮੰਜ਼ਿਲ_Cancel = prvGetThreadFromTask( px_Task_ਮੰਜ਼ਿਲ_Delete );

    /*
     * The thread has already been suspended so it can be safely cancelled.
     */
    pthread_cancel( px_Thread_ਮੰਜ਼ਿਲ_Cancel->pthread );
    pthread_join( px_Thread_ਮੰਜ਼ਿਲ_Cancel->pthread, ਨਿਰਾ );
    event_delete( px_Thread_ਮੰਜ਼ਿਲ_Cancel->ev );
}
/*-----------------------------------------------------------*/

ਸਥਿਰ ਖਾਲੀ *prv_Wait_For_ਸ਼ੁਰੂ( ਖਾਲੀ * pvParams )
{
    Thread_t *pxThread = pvParams;

    prvSuspendSelf(pxThread);

    /* Resumed for the first time, thus this thread didn't previously call
     * prvSwitchThread(). So we need to initialise the state variables for this
     * thread. */
    uxCriticalNestingIDF = 0;
    ux_ਰੁਕਾਵਟ_Nesting = 0;
    ux_ਰੁਕਾਵਟ_Level = 0;
    v_ਪੋਰਟ_Enable_Interrupts();

    /* Call the task's entry point. */
    pxThread->px_ਕੋਡ( pxThread->pvParams );

    /* A function that implements a task must not exit or attempt to return to
    * its caller as there is nothing to return to. If a task wants to exit it
    * should instead call vTaskDelete( NULL ). Artificially force an assert()
    * to be triggered if configASSERT() is defined, so application writers can
        * catch the error. */
    configASSERT( pdFALSE );

    ਵਾਪਸ ਨਿਰਾ;
}
/*-----------------------------------------------------------*/

ਸਥਿਰ ਖਾਲੀ prvSwitchThread( Thread_t *px_Thread_ਮੰਜ਼ਿਲ_Resume,
                             Thread_t *px_Thread_ਮੰਜ਼ਿਲ_Suspend )
{
    ਅਧਾਰ_Type_t uxSavedCriticalNestingIDF;
    ਅਧਾਰ_Type_t ux_Saved_ਰੁਕਾਵਟ_Nesting;
    ਅਧਾਰ_Type_t ux_Saved_ਰੁਕਾਵਟ_Level;

    ਜੇ ( px_Thread_ਮੰਜ਼ਿਲ_Suspend != px_Thread_ਮੰਜ਼ਿਲ_Resume )
    {
        /* It is possible for prvSwitchThread() to be called...
         * - while inside an ISR (i.e., via vPortSystemTickHandler() or vPortYieldFromISR())
         * - while interrupts are disabled or in a critical section (i.e., via vPortYield())
         *
         * So we need to save the various count variables as part of the thread's context.
         * They are restored when the pthread switches back. */
        uxSavedCriticalNestingIDF = uxCriticalNestingIDF;
        ux_Saved_ਰੁਕਾਵਟ_Nesting = ux_ਰੁਕਾਵਟ_Nesting;
        ux_Saved_ਰੁਕਾਵਟ_Level = ux_ਰੁਕਾਵਟ_Level;

        prvResumeThread( px_Thread_ਮੰਜ਼ਿਲ_Resume );
        ਜੇ ( px_Thread_ਮੰਜ਼ਿਲ_Suspend->xDying )
        {
            pthread_exit( ਨਿਰਾ );
        }
        prvSuspendSelf( px_Thread_ਮੰਜ਼ਿਲ_Suspend );

        uxCriticalNestingIDF = uxSavedCriticalNestingIDF;
        ux_ਰੁਕਾਵਟ_Nesting = ux_Saved_ਰੁਕਾਵਟ_Nesting;
        ux_ਰੁਕਾਵਟ_Level = ux_Saved_ਰੁਕਾਵਟ_Level;
    }
}
/*-----------------------------------------------------------*/

ਸਥਿਰ ਖਾਲੀ prvSuspendSelf( Thread_t *thread )
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

ਸਥਿਰ ਖਾਲੀ prvResumeThread( Thread_t *xThreadId )
{
    ਜੇ ( pthread_self() != xThreadId->pthread )
    {
        event_signal(xThreadId->ev);
    }
}
/*-----------------------------------------------------------*/

ਸਥਿਰ ਖਾਲੀ prv_ਤਿਆਰੀ_ਸੰਕੇਤ_And_Scheduler_Policy( ਖਾਲੀ )
{
    ਬਣਤਰ sigaction sigresume, sigtick;
    ਪੂਰਨ iRet;

    hMainThread = pthread_self();

    /* Initialise common signal masks. */
    sigfillset( &x_All_ਸੰਕੇਤ );
    /* Don't block SIGINT so this can be used to break into GDB while
     * in a critical section. */
    sigdelset( &x_All_ਸੰਕੇਤ, SIGINT );

    /*
     * Block all signals in this thread so all new threads
     * inherits this mask.
     *
     * When a thread is resumed for the first time, all signals
     * will be unblocked.
     */
    (ਖਾਲੀ)pthread_sigmask( SIG_SETMASK, &x_All_ਸੰਕੇਤ,
                           &x_Scheduler_Original_Signal_ਨਕਾਬ );

    /* SIG_RESUME is only used with sigwait() so doesn't need a
       handler. */
    sigresume.sa_flags = 0;
    sigresume.sa_handler = SIG_IGN;
    sigfillset( &sigresume.sa_mask );

    sigtick.sa_flags = 0;
    sigtick.sa_handler = v_ਪੋਰਟ_System_Tick_Handler;
    sigfillset( &sigtick.sa_mask );

    iRet = sigaction( SIG_RESUME, &sigresume, ਨਿਰਾ );
    ਜੇ ( iRet )
    {
        prvFatalError( "sigaction", errno );
    }

    iRet = sigaction( SIGALRM, &sigtick, ਨਿਰਾ );
    ਜੇ ( iRet )
    {
        prvFatalError( "sigaction", errno );
    }
}
/*-----------------------------------------------------------*/

ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ul_ਪੋਰਟ_Get_ਲੜੀਵਾਰ_ਸਮਾਂ( ਖਾਲੀ )
{
    ਬਣਤਰ tms xTimes;

    times( &xTimes );

    ਵਾਪਸ ( ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ) xTimes.tms_utime;
}
/*-----------------------------------------------------------*/
bool ਪੋਰਟ_VALID_LIST_MEM(ਅਚਲ ਖਾਲੀ *ptr)
{
    ਵਾਪਸ true;
}

bool ਪੋਰਟ_VALID_TCB_MEM(ਅਚਲ ਖਾਲੀ *ptr)
{
    ਵਾਪਸ true;
}

bool ਪੋਰਟ_VALID_ਢੇਰ_MEM(ਅਚਲ ਖਾਲੀ *ptr)
{
    ਵਾਪਸ true;
}
/*-----------------------------------------------------------*/

ਪੋਰਟ_MUX_TYPE ਪੋਰਟ_x_Task_ਤਾਲਾ = ਪੋਰਟ_MUX_INITIALIZER_UNLOCKED;
ਪੋਰਟ_MUX_TYPE ਪੋਰਟ_x_ISRLock = ਪੋਰਟ_MUX_INITIALIZER_UNLOCKED;

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *ਟੈਗ = "port";

ਅਧਾਰ_Type_t x_ਪੋਰਟ_Check_If_ਵਿੱਚ_ISR(ਖਾਲੀ)
{
    ਵਾਪਸ (ux_ਰੁਕਾਵਟ_Nesting == 0) ? pdFALSE : pdTRUE;
}

ਖਾਲੀ app_main(ਖਾਲੀ);

ਸਥਿਰ ਖਾਲੀ main_task(ਖਾਲੀ* ਦਲੀਲਾਂ)
{
    app_main();
    vTaskDelete(ਨਿਰਾ);
}

ਪੂਰਨ ਮੁੱਖ(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ **argv)
{
    // This makes sure that stdio is flushed after each '\n' so that idf.py monitor
    // reads the program output on time.
    setvbuf(stdout, ਨਿਰਾ, _IOLBF, 0);

    usleep(1000);

    ਅਧਾਰ_Type_t res;

#ਜੇ ( config_ਅੰਕ_CORES > 1 )
    res = x_Task_Create_Affinity_ਸੈੱਟ(&main_task, "main",
                                 ESP_TASK_MAIN_ਢੇਰ, ਨਿਰਾ,
                                 ESP_TASK_MAIN_PRIO, ESP_TASK_MAIN_CORE, ਨਿਰਾ);
#ਨਹੀਂਤਾਂ
    res = xTaskCreate(&main_task, "main",
                      ESP_TASK_MAIN_ਢੇਰ, ਨਿਰਾ,
                      ESP_TASK_MAIN_PRIO, ਨਿਰਾ);
#ਜੇ_ਅੰਤ

    ਪ੍ਰਤਿਜ੍ਞਾ(res == pdTRUE);
    (ਖਾਲੀ)res;

    ESP_LOGI(ਟੈਗ, "Starting SMP scheduler.");
    v_Task_ਸ਼ੁਰੂ_Scheduler();

    // This line should never be reached
    ਪ੍ਰਤਿਜ੍ਞਾ(false);
}

ਖਾਲੀ esp_vApplicationIdleHook(ਖਾਲੀ)
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

ਖਾਲੀ esp_vApplicationTickHook( ਖਾਲੀ ) { }

#ਜੇ  (  configUSE_TICK_HOOK > 0 )
ਖਾਲੀ vApplicationTickHook( ਖਾਲੀ )
{
    esp_vApplicationTickHook();
}
#ਜੇ_ਅੰਤ

#ਜੇ ( config_SUPPORT_STATIC_ਵੰਡ == 1 )
/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
 * implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
 * used by the Idle task. */
ਖਾਲੀ v_Application_Get_Idle_Task_ਸਮ੍ਰਿਤੀ( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                    ਢੇਰ_Type_t ** ppx_Idle_Task_ਢੇਰ_ਬਫਰ,
                                    uint32_t * pul_Idle_Task_ਢੇਰ_ਮਾਪ )
{
/* If the buffers to be provided to the Idle task are declared inside this
 * function then they must be declared static - otherwise they will be allocated on
 * the stack and so not exists after this function exits. */
    ਸਥਿਰ StaticTask_t xIdleTaskTCB;
    ਸਥਿਰ ਢੇਰ_Type_t ux_Idle_Task_ਢੇਰ[ config_ਨਿਊਨਤਮ_ਢੇਰ_ਮਾਪ ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
     * state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppx_Idle_Task_ਢੇਰ_ਬਫਰ = ux_Idle_Task_ਢੇਰ;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in bytes. */
    *pul_Idle_Task_ਢੇਰ_ਮਾਪ = config_ਨਿਊਨਤਮ_ਢੇਰ_ਮਾਪ;
}
#ਜੇ_ਅੰਤ // configSUPPORT_STATIC_ALLOCATION == 1
/*-----------------------------------------------------------*/

#ਜੇ ( (config_SUPPORT_STATIC_ਵੰਡ == 1) && (configUSE_TIMERS == 1))

/* When configSUPPORT_STATIC_ALLOCATION is set to 1 the application writer can
 * use a callback function to optionally provide the memory required by the idle
 * and timer tasks.  This is the stack that will be used by the timer task.  It is
 * declared here, as a global, so it can be checked by a test that is implemented
 * in a different file. */
ਢੇਰ_Type_t ux_Timer_Task_ਢੇਰ[ config_TIMER_TASK_ਢੇਰ_ਡੂੰਘਾਈ ];


/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
 * application must provide an implementation of vApplicationGetTimerTaskMemory()
 * to provide the memory that is used by the Timer service task. */
ਖਾਲੀ v_Application_Get_Timer_Task_ਸਮ੍ਰਿਤੀ( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                     ਢੇਰ_Type_t ** ppx_Timer_Task_ਢੇਰ_ਬਫਰ,
                                     uint32_t * pul_Timer_Task_ਢੇਰ_ਮਾਪ )
{
/* If the buffers to be provided to the Timer task are declared inside this
 * function then they must be declared static - otherwise they will be allocated on
 * the stack and so not exists after this function exits. */
    ਸਥਿਰ StaticTask_t xTimerTaskTCB;

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
     * task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppx_Timer_Task_ਢੇਰ_ਬਫਰ = ux_Timer_Task_ਢੇਰ;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in bytes. */
    *pul_Timer_Task_ਢੇਰ_ਮਾਪ = config_TIMER_TASK_ਢੇਰ_ਡੂੰਘਾਈ;
}
#ਜੇ_ਅੰਤ // configSUPPORT_STATIC_ALLOCATION == 1 && (configUSE_TIMERS == 1)

ਖਾਲੀ v_ਪੋਰਟ_Take_ਤਾਲਾ( ਪੋਰਟ_MUX_TYPE *ਤਾਲਾ )
{
    spinlock_acquire( ਤਾਲਾ, ਪੋਰਟ_MUX_NO_TIMEOUT);
}

ਖਾਲੀ v_ਪੋਰਟ_Release_ਤਾਲਾ( ਪੋਰਟ_MUX_TYPE *ਤਾਲਾ )
{
    spinlock_release( ਤਾਲਾ );
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FREERTOS_SMP_MALLOC_CAPS    (MALLOC_CAP_ਅੰਦਰੂਨੀ|MALLOC_CAP_8_ਦੋਆਧਾਰੀ_ਅੰਕ)

ਖਾਲੀ *pv_ਪੋਰਟ_Malloc( size_t x_ਮਾਪ )
{
    ਵਾਪਸ ਢੇਰ_caps_malloc(x_ਮਾਪ, FREERTOS_SMP_MALLOC_CAPS);
}

ਖਾਲੀ v_ਪੋਰਟ_Free( ਖਾਲੀ *pv )
{
    ਢੇਰ_caps_free(pv);
}

ਖਾਲੀ __attribute__((weak)) v_Application_ਢੇਰ_ਉਛਾਲ_Hook(TaskHandle_t xTask, ਅੱਖਰ *pcTaskName)
{
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ERR_STR1 "***ERROR*** A stack overflow in task "
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ERR_STR2 " has been detected."
    ਅਚਲ ਅੱਖਰ *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    ਅੱਖਰ buf[ਆਕਾਰ(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + ਆਕਾਰ(ERR_STR2) + 1 /* null char */] = {0};

    ਅੱਖਰ *dest = buf;
    ਲਈ (ਪੂਰਨ i = 0; i < ਆਕਾਰ(str) / ਆਕਾਰ(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
	ਲਿਖੋ("%s\n", buf);
    abort();
}

// ------- Thread Local Storage Pointers Deletion Callbacks -------

#ਜੇ ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
ਖਾਲੀ v_ਪੋਰਟ_TLSPointers_Del_Cb( ਖਾਲੀ *pxTCB )
{
    /* Typecast pxTCB to StaticTask_t type to access TCB struct members.
     * pvDummy15 corresponds to pvThreadLocalStoragePointers member of the TCB.
     */
    StaticTask_t *tcb = ( StaticTask_t * )pxTCB;

    /* The TLSP deletion callbacks are stored at an offset of (configNUM_THREAD_LOCAL_STORAGE_POINTERS/2) */
    TlsDeleteCallbackFunction_t *pv_Thread_Local_Storage_ਸੂਚਕ_ਸਾਰੇ_Del_Callback = ( TlsDeleteCallbackFunction_t * )( &( tcb->pvDummy15[ ( config_ਅੰਕ_THREAD_LOCAL_STORAGE_ਸੂਚਕ_ਸਾਰੇ / 2 ) ] ) );

    /* We need to iterate over half the depth of the pvThreadLocalStoragePointers area
     * to access all TLS pointers and their respective TLS deletion callbacks.
     */
    ਲਈ ( ਪੂਰਨ x = 0; x < ( config_ਅੰਕ_THREAD_LOCAL_STORAGE_ਸੂਚਕ_ਸਾਰੇ / 2 ); x++ ) {
        ਜੇ ( pv_Thread_Local_Storage_ਸੂਚਕ_ਸਾਰੇ_Del_Callback[ x ] != ਨਿਰਾ ) {  //If del cb is set
            // We skip the check if the callback is executable as that is difficult to determine for different
            // platforms (compare xtensa and riscv code).
            pv_Thread_Local_Storage_ਸੂਚਕ_ਸਾਰੇ_Del_Callback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#ਜੇ_ਅੰਤ // CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS

ਖਾਲੀ v_ਪੋਰਟ_Clean_ਉੱਪਰ_TCB ( ਖਾਲੀ *pxTCB )
{
    #ਜੇ ( CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK )
        /* Call the user defined task pre-deletion hook */
        ਬਾਹਰੀ ਖਾਲੀ vTaskPreDeletionHook( ਖਾਲੀ * pxTCB );
        vTaskPreDeletionHook( pxTCB );
    #ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK */

#ਜੇ ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
    /* Call TLS pointers deletion callbacks */
    v_ਪੋਰਟ_TLSPointers_Del_Cb( pxTCB );
#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

    v_ਪੋਰਟ_Cancel_Thread(pxTCB);
}
