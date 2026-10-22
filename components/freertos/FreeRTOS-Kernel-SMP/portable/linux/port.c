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

#అన్తర్భావ <errno.h>
#అన్తర్భావ <pthread.h>
#అన్తర్భావ <signal.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <sys/time.h>
#అన్తర్భావ <sys/times.h>
#అన్తర్భావ <time.h>
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <assert.h>

/* Scheduler includes. */
#అన్తర్భావ "esp_heap_caps.h"
#అన్తర్భావ "FreeRTOS.h"
#అన్తర్భావ "task.h"
#అన్తర్భావ "esp_task.h"
#అన్తర్భావ "timers.h"
#అన్తర్భావ "utils/wait_for_event.h"
#అన్తర్భావ "esp_log.h"
/*-----------------------------------------------------------*/

#నిర్వచించు SIG_RESUME SIGUSR1

రకనిర్వచనం నిర్మాణం THREAD
{
    pthread_t pthread;
    TaskFunction_t px_సంకేతం;
    శూన్య *pvParams;
    పునాది_Type_t xDying;
    నిర్మాణం event *ev;
} Thread_t;

/*
 * The additional per-thread data is stored at the beginning of the
 * task's stack.
 */
స్థిర అంతఃవరుస Thread_t *prvGetThreadFromTask(TaskHandle_t xTask)
{
    దొంతర_Type_t *px_శిఖరం_యొక్క_దొంతర = *(దొంతర_Type_t **)xTask;

    తిరిగి (Thread_t *)(px_శిఖరం_యొక్క_దొంతర + 1);
}

/*-----------------------------------------------------------*/

స్థిర pthread_once_t h_Sig_అమరిక_Thread = PTHREAD_ONCE_INIT;
స్థిర sigset_t x_All_సంకేతాలు;
స్థిర sigset_t x_Scheduler_Original_Signal_ముసుగు;
స్థిర pthread_t hMainThread = ( pthread_t )ఖాళీ;

// These are saved as part of a thread's state in prvSwitchThread()
స్థిర అస్థిర పునాది_Type_t uxCriticalNestingIDF = 0;    /* Track nesting calls for IDF style critical sections. FreeRTOS critical section nesting is maintained in the TCB. */
స్థిర అస్థిర UBaseType_t ux_అంతరాయం_Nesting = 0;     /* Tracks if we are currently in an interrupt. */
స్థిర అస్థిర పునాది_Type_t ux_అంతరాయం_Level = 0;        /* Tracks the current level (i.e., interrupt mask) */
/*-----------------------------------------------------------*/

స్థిర పునాది_Type_t x_Scheduler_చివర = pdFALSE;
/*-----------------------------------------------------------*/

స్థిర శూన్య prv_అమరిక_సంకేతాలు_And_Scheduler_Policy( శూన్య );
స్థిర శూన్య prv_అమరిక_Timer_అంతరాయం( శూన్య );
స్థిర శూన్య *prv_Wait_For_ప్రారంభం( శూన్య * pvParams );
స్థిర శూన్య prvSwitchThread( Thread_t * x_Thread_గమ్యం_Resume,
                             Thread_t *x_Thread_గమ్యం_Suspend );
స్థిర శూన్య prvSuspendSelf( Thread_t * thread);
స్థిర శూన్య prvResumeThread( Thread_t * xThreadId );
స్థిర శూన్య v_పోర్టు_System_Tick_Handler( పూర్ణాంకం sig );
స్థిర శూన్య v_పోర్టు_ప్రారంభం_మొదటి_Task( శూన్య );
/*-----------------------------------------------------------*/

స్థిర శూన్య prvFatalError( స్థిరరాశి అక్షరం *pcCall, పూర్ణాంకం iErrno )
{
    fprintf( stderr, "%s: %s\n", pcCall, strerror( iErrno ) );
    abort();
}

/*
 * See header file for description.
 */
దొంతర_Type_t *px_పోర్టు_Initialise_దొంతర( దొంతర_Type_t *px_శిఖరం_యొక్క_దొంతర,
                                    దొంతర_Type_t *px_చివర_యొక్క_దొంతర,
                                    TaskFunction_t px_సంకేతం,
                                    శూన్య *pvParameters )
{
    Thread_t *thread;
    pthread_attr_t x_Thread_లక్షణాలు;
    size_t ul_దొంతర_కొలత;
    పూర్ణాంకం iRet;

    (శూన్య)pthread_once( &h_Sig_అమరిక_Thread, prv_అమరిక_సంకేతాలు_And_Scheduler_Policy );

    /*
     * Store the additional thread data at the start of the stack.
     */
    thread = (Thread_t *)(px_శిఖరం_యొక్క_దొంతర + 1) - 1;
    px_శిఖరం_యొక్క_దొంతర = (దొంతర_Type_t *)thread - 1;
    ul_దొంతర_కొలత = (px_శిఖరం_యొక్క_దొంతర + 1 - px_చివర_యొక్క_దొంతర) * పరిమాణం(*px_శిఖరం_యొక్క_దొంతర);

    thread->px_సంకేతం = px_సంకేతం;
    thread->pvParams = pvParameters;
    thread->xDying = pdFALSE;

    pthread_attr_init( &x_Thread_లక్షణాలు );
    pthread_attr_setstack( &x_Thread_లక్షణాలు, px_చివర_యొక్క_దొంతర, ul_దొంతర_కొలత );

    thread->ev = event_create();

    పునాది_Type_t prev_intr_level = x_పోర్టు_సమితి_అంతరాయం_ముసుగు();

    iRet = pthread_create( &thread->pthread, &x_Thread_లక్షణాలు,
                           prv_Wait_For_ప్రారంభం, thread );
    ఒకవేళ ( iRet )
    {
        prvFatalError( "pthread_create", iRet );
    }

    v_పోర్టు_Clear_అంతరాయం_ముసుగు( prev_intr_level );

    తిరిగి px_శిఖరం_యొక్క_దొంతర;
}
/*-----------------------------------------------------------*/

శూన్య v_పోర్టు_ప్రారంభం_మొదటి_Task( శూన్య )
{
    Thread_t *px_మొదటి_Thread = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    /* Start the first task. */
    prvResumeThread( px_మొదటి_Thread );
}
/*-----------------------------------------------------------*/

/*
 * See header file for description.
 */
పునాది_Type_t x_పోర్టు_ప్రారంభం_Scheduler( శూన్య )
{
    పూర్ణాంకం iSignal;
    sigset_t x_సంకేతాలు;

    hMainThread = pthread_self();

    /* Start the timer that generates the tick ISR(SIGALRM).
       Interrupts are disabled here already. */
    prv_అమరిక_Timer_అంతరాయం();

    /* Start the first task. */
    v_పోర్టు_ప్రారంభం_మొదటి_Task();

    /* Wait until signaled by vPortEndScheduler(). */
    sigemptyset( &x_సంకేతాలు );
    sigaddset( &x_సంకేతాలు, SIG_RESUME );

    అయితే ( !x_Scheduler_చివర )
    {
        sigwait( &x_సంకేతాలు, &iSignal );
    }

    /* Cancel the Idle task and free its resources */
#ఒకవేళ ( INCLUDE_xTaskGetIdleTaskHandle == 1 )
    v_పోర్టు_Cancel_Thread( xTaskGetIdleTaskHandle() );
#అయితే_ముగింపు

#ఒకవేళ ( configUSE_TIMERS == 1 )
    /* Cancel the Timer task and free its resources */
    v_పోర్టు_Cancel_Thread( xTimerGetTimerDaemonTaskHandle() );
#అయితే_ముగింపు /* configUSE_TIMERS */

    /* Restore original signal mask. */
    (శూన్య)pthread_sigmask( SIG_SETMASK, &x_Scheduler_Original_Signal_ముసుగు,  ఖాళీ );

    తిరిగి 0;
}
/*-----------------------------------------------------------*/

శూన్య v_పోర్టు_చివర_Scheduler( శూన్య )
{
    నిర్మాణం itimerval itimer;
    నిర్మాణం sigaction sigtick;
    Thread_t *xCurrentThread;

    /* Stop the timer and ignore any pending SIGALRMs that would end
     * up running on the main thread when it is resumed. */
    itimer.it_value.tv_sec = 0;
    itimer.it_value.tv_usec = 0;

    itimer.it_interval.tv_sec = 0;
    itimer.it_interval.tv_usec = 0;
    (శూన్య)setitimer( ITIMER_REAL, &itimer, ఖాళీ );

    sigtick.sa_flags = 0;
    sigtick.sa_handler = SIG_IGN;
    sigemptyset( &sigtick.sa_mask );
    sigaction( SIGALRM, &sigtick, ఖాళీ );

    /* Signal the scheduler to exit its loop. */
    x_Scheduler_చివర = pdTRUE;
    (శూన్య)pthread_kill( hMainThread, SIG_RESUME );

    xCurrentThread = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );
    prvSuspendSelf(xCurrentThread);
}
/*-----------------------------------------------------------*/

స్థిర శూన్య v_పోర్టు_Disable_Interrupts( శూన్య )
{
    pthread_sigmask( SIG_BLOCK, &x_All_సంకేతాలు, ఖాళీ );
}
/*-----------------------------------------------------------*/

స్థిర శూన్య v_పోర్టు_Enable_Interrupts( శూన్య )
{
    pthread_sigmask( SIG_UNBLOCK, &x_All_సంకేతాలు, ఖాళీ );
}
/*-----------------------------------------------------------*/

శూన్య v_పోర్టు_Enter_Critical_IDF( శూన్య )
{
    ఒకవేళ ( uxCriticalNestingIDF == 0 && ux_అంతరాయం_Level == 0)
    {
        v_పోర్టు_Disable_Interrupts();
    }
    uxCriticalNestingIDF++;
}
/*-----------------------------------------------------------*/

శూన్య v_పోర్టు_Exit_Critical_IDF( శూన్య )
{
    uxCriticalNestingIDF--;

    /* If we have reached 0 then re-enable the interrupts. */
    ఒకవేళ( uxCriticalNestingIDF == 0 && ux_అంతరాయం_Level == 0)
    {
        v_పోర్టు_Enable_Interrupts();
    }
}
/*-----------------------------------------------------------*/

శూన్య v_పోర్టు_Yield_From_ISR( శూన్య )
{
    Thread_t *x_Thread_గమ్యం_Suspend;
    Thread_t *x_Thread_గమ్యం_Resume;

    x_Thread_గమ్యం_Suspend = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    vTaskSwitchContext();

    x_Thread_గమ్యం_Resume = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    prvSwitchThread( x_Thread_గమ్యం_Resume, x_Thread_గమ్యం_Suspend );
}
/*-----------------------------------------------------------*/

శూన్య v_పోర్టు_Yield( శూన్య )
{
    పునాది_Type_t prev_intr_level = x_పోర్టు_సమితి_అంతరాయం_ముసుగు();

    v_పోర్టు_Yield_From_ISR();

    v_పోర్టు_Clear_అంతరాయం_ముసుగు( prev_intr_level );
}
/*-----------------------------------------------------------*/

/* In SMP code, the disable/enable interrupt macros are calling the set/get interrupt mask functions below.
   Hence, we need to call vPortDisableInterrupts() and vPortEnableInterrupts(), otherwise interrupts
   are never disabled/enabled. */

పునాది_Type_t x_పోర్టు_సమితి_అంతరాయం_ముసుగు( శూన్య )
{
    ఒకవేళ (ux_అంతరాయం_Level == 0 && uxCriticalNestingIDF == 0) {
        v_పోర్టు_Disable_Interrupts();
    }
    పునాది_Type_t prev_intr_level = ux_అంతరాయం_Level;
    ux_అంతరాయం_Level++;
    తిరిగి prev_intr_level;
}
/*-----------------------------------------------------------*/

శూన్య v_పోర్టు_Clear_అంతరాయం_ముసుగు( పునాది_Type_t x_ముసుగు )
{
    // Only re-enable interrupts if xMask is 0
    ux_అంతరాయం_Level = x_ముసుగు;
    ఒకవేళ (ux_అంతరాయం_Level == 0 && uxCriticalNestingIDF == 0) {
        v_పోర్టు_Enable_Interrupts();
    }
}
/*-----------------------------------------------------------*/

స్థిర uint64_t prv_Get_సమయం_Ns(శూన్య)
{
    నిర్మాణం timespec t;

    clock_gettime(CLOCK_MONOTONIC, &t);

    తిరిగి t.tv_sec * 1000000000ull + t.tv_nsec;
}

స్థిర uint64_t prv_ప్రారంభం_సమయం_Ns;
/* commented as part of the code below in vPortSystemTickHandler,
 * to adjust timing according to full demo requirements */
/* static uint64_t prvTickCount; */

/*
 * Setup the systick timer to generate the tick interrupts at the required
 * frequency.
 */
శూన్య prv_అమరిక_Timer_అంతరాయం( శూన్య )
{
    నిర్మాణం itimerval itimer;
    పూర్ణాంకం iRet;

    /* Initialise the structure with the current timer information. */
    iRet = getitimer( ITIMER_REAL, &itimer );
    ఒకవేళ ( iRet )
    {
        prvFatalError( "getitimer", errno );
    }

    /* Set the interval between timer events. */
    itimer.it_interval.tv_sec = 0;
    itimer.it_interval.tv_usec = పోర్టు_TICK_రేటు_MICROSECONDS;

    /* Set the current count-down. */
    itimer.it_value.tv_sec = 0;
    itimer.it_value.tv_usec = పోర్టు_TICK_రేటు_MICROSECONDS;

    /* Set-up the timer interrupt. */
    iRet = setitimer( ITIMER_REAL, &itimer, ఖాళీ );
    ఒకవేళ ( iRet )
    {
        prvFatalError( "setitimer", errno );
    }

    prv_ప్రారంభం_సమయం_Ns = prv_Get_సమయం_Ns();
}
/*-----------------------------------------------------------*/

స్థిర శూన్య v_పోర్టు_System_Tick_Handler( పూర్ణాంకం sig )
{
    Thread_t *px_Thread_గమ్యం_Suspend;
    Thread_t *px_Thread_గమ్యం_Resume;
    పునాది_Type_t xSwitchRequired;
    /* uint64_t xExpectedTicks; */

    // Handling a timer signal, so we are currently in an interrupt.
    ux_అంతరాయం_Nesting++;

#ఒకవేళ ( configUSE_PREEMPTION == 1 )
    px_Thread_గమ్యం_Suspend = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );
#అయితే_ముగింపు

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

#ఒకవేళ ( configUSE_PREEMPTION == 1 )
    ఒకవేళ (xSwitchRequired == pdTRUE) {
        /* Select Next Task. */
        vTaskSwitchContext();

        px_Thread_గమ్యం_Resume = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

        prvSwitchThread(px_Thread_గమ్యం_Resume, px_Thread_గమ్యం_Suspend);
    }
#లేకపోతే
    (శూన్య)xSwitchRequired;
#అయితే_ముగింపు

    // Returning from the timer signal handler, so we are exiting the interrupt.
    ux_అంతరాయం_Nesting--;
}
/*-----------------------------------------------------------*/

శూన్య v_పోర్టు_Thread_Dying( శూన్య *px_Task_గమ్యం_Delete, అస్థిర పునాది_Type_t *pxPendYield )
{
    Thread_t *pxThread = prvGetThreadFromTask( px_Task_గమ్యం_Delete );

    pxThread->xDying = pdTRUE;
}

శూన్య v_పోర్టు_Cancel_Thread( శూన్య *px_Task_గమ్యం_Delete )
{
    Thread_t *px_Thread_గమ్యం_Cancel = prvGetThreadFromTask( px_Task_గమ్యం_Delete );

    /*
     * The thread has already been suspended so it can be safely cancelled.
     */
    pthread_cancel( px_Thread_గమ్యం_Cancel->pthread );
    pthread_join( px_Thread_గమ్యం_Cancel->pthread, ఖాళీ );
    event_delete( px_Thread_గమ్యం_Cancel->ev );
}
/*-----------------------------------------------------------*/

స్థిర శూన్య *prv_Wait_For_ప్రారంభం( శూన్య * pvParams )
{
    Thread_t *pxThread = pvParams;

    prvSuspendSelf(pxThread);

    /* Resumed for the first time, thus this thread didn't previously call
     * prvSwitchThread(). So we need to initialise the state variables for this
     * thread. */
    uxCriticalNestingIDF = 0;
    ux_అంతరాయం_Nesting = 0;
    ux_అంతరాయం_Level = 0;
    v_పోర్టు_Enable_Interrupts();

    /* Call the task's entry point. */
    pxThread->px_సంకేతం( pxThread->pvParams );

    /* A function that implements a task must not exit or attempt to return to
    * its caller as there is nothing to return to. If a task wants to exit it
    * should instead call vTaskDelete( NULL ). Artificially force an assert()
    * to be triggered if configASSERT() is defined, so application writers can
        * catch the error. */
    configASSERT( pdFALSE );

    తిరిగి ఖాళీ;
}
/*-----------------------------------------------------------*/

స్థిర శూన్య prvSwitchThread( Thread_t *px_Thread_గమ్యం_Resume,
                             Thread_t *px_Thread_గమ్యం_Suspend )
{
    పునాది_Type_t uxSavedCriticalNestingIDF;
    పునాది_Type_t ux_Saved_అంతరాయం_Nesting;
    పునాది_Type_t ux_Saved_అంతరాయం_Level;

    ఒకవేళ ( px_Thread_గమ్యం_Suspend != px_Thread_గమ్యం_Resume )
    {
        /* It is possible for prvSwitchThread() to be called...
         * - while inside an ISR (i.e., via vPortSystemTickHandler() or vPortYieldFromISR())
         * - while interrupts are disabled or in a critical section (i.e., via vPortYield())
         *
         * So we need to save the various count variables as part of the thread's context.
         * They are restored when the pthread switches back. */
        uxSavedCriticalNestingIDF = uxCriticalNestingIDF;
        ux_Saved_అంతరాయం_Nesting = ux_అంతరాయం_Nesting;
        ux_Saved_అంతరాయం_Level = ux_అంతరాయం_Level;

        prvResumeThread( px_Thread_గమ్యం_Resume );
        ఒకవేళ ( px_Thread_గమ్యం_Suspend->xDying )
        {
            pthread_exit( ఖాళీ );
        }
        prvSuspendSelf( px_Thread_గమ్యం_Suspend );

        uxCriticalNestingIDF = uxSavedCriticalNestingIDF;
        ux_అంతరాయం_Nesting = ux_Saved_అంతరాయం_Nesting;
        ux_అంతరాయం_Level = ux_Saved_అంతరాయం_Level;
    }
}
/*-----------------------------------------------------------*/

స్థిర శూన్య prvSuspendSelf( Thread_t *thread )
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

స్థిర శూన్య prvResumeThread( Thread_t *xThreadId )
{
    ఒకవేళ ( pthread_self() != xThreadId->pthread )
    {
        event_signal(xThreadId->ev);
    }
}
/*-----------------------------------------------------------*/

స్థిర శూన్య prv_అమరిక_సంకేతాలు_And_Scheduler_Policy( శూన్య )
{
    నిర్మాణం sigaction sigresume, sigtick;
    పూర్ణాంకం iRet;

    hMainThread = pthread_self();

    /* Initialise common signal masks. */
    sigfillset( &x_All_సంకేతాలు );
    /* Don't block SIGINT so this can be used to break into GDB while
     * in a critical section. */
    sigdelset( &x_All_సంకేతాలు, SIGINT );

    /*
     * Block all signals in this thread so all new threads
     * inherits this mask.
     *
     * When a thread is resumed for the first time, all signals
     * will be unblocked.
     */
    (శూన్య)pthread_sigmask( SIG_SETMASK, &x_All_సంకేతాలు,
                           &x_Scheduler_Original_Signal_ముసుగు );

    /* SIG_RESUME is only used with sigwait() so doesn't need a
       handler. */
    sigresume.sa_flags = 0;
    sigresume.sa_handler = SIG_IGN;
    sigfillset( &sigresume.sa_mask );

    sigtick.sa_flags = 0;
    sigtick.sa_handler = v_పోర్టు_System_Tick_Handler;
    sigfillset( &sigtick.sa_mask );

    iRet = sigaction( SIG_RESUME, &sigresume, ఖాళీ );
    ఒకవేళ ( iRet )
    {
        prvFatalError( "sigaction", errno );
    }

    iRet = sigaction( SIGALRM, &sigtick, ఖాళీ );
    ఒకవేళ ( iRet )
    {
        prvFatalError( "sigaction", errno );
    }
}
/*-----------------------------------------------------------*/

గుర్తులేని దీర్ఘ ul_పోర్టు_Get_నిరంతరం_సమయం( శూన్య )
{
    నిర్మాణం tms xTimes;

    times( &xTimes );

    తిరిగి ( గుర్తులేని దీర్ఘ ) xTimes.tms_utime;
}
/*-----------------------------------------------------------*/
bool పోర్టు_VALID_LIST_MEM(స్థిరరాశి శూన్య *ptr)
{
    తిరిగి true;
}

bool పోర్టు_VALID_TCB_MEM(స్థిరరాశి శూన్య *ptr)
{
    తిరిగి true;
}

bool పోర్టు_VALID_దొంతర_MEM(స్థిరరాశి శూన్య *ptr)
{
    తిరిగి true;
}
/*-----------------------------------------------------------*/

పోర్టు_MUX_TYPE పోర్టు_x_Task_తాళం = పోర్టు_MUX_INITIALIZER_UNLOCKED;
పోర్టు_MUX_TYPE పోర్టు_x_ISRLock = పోర్టు_MUX_INITIALIZER_UNLOCKED;

స్థిర స్థిరరాశి అక్షరం *ట్యాగ్ = "port";

పునాది_Type_t x_పోర్టు_Check_If_లోపల_ISR(శూన్య)
{
    తిరిగి (ux_అంతరాయం_Nesting == 0) ? pdFALSE : pdTRUE;
}

శూన్య app_main(శూన్య);

స్థిర శూన్య main_task(శూన్య* వాదనలు)
{
    app_main();
    vTaskDelete(ఖాళీ);
}

పూర్ణాంకం ప్రధాన(పూర్ణాంకం argc, స్థిరరాశి అక్షరం **argv)
{
    // This makes sure that stdio is flushed after each '\n' so that idf.py monitor
    // reads the program output on time.
    setvbuf(stdout, ఖాళీ, _IOLBF, 0);

    usleep(1000);

    పునాది_Type_t res;

#ఒకవేళ ( config_అంకె_CORES > 1 )
    res = x_Task_Create_Affinity_సమితి(&main_task, "main",
                                 ESP_TASK_MAIN_దొంతర, ఖాళీ,
                                 ESP_TASK_MAIN_PRIO, ESP_TASK_MAIN_CORE, ఖాళీ);
#లేకపోతే
    res = xTaskCreate(&main_task, "main",
                      ESP_TASK_MAIN_దొంతర, ఖాళీ,
                      ESP_TASK_MAIN_PRIO, ఖాళీ);
#అయితే_ముగింపు

    ప్రతిజ్ఞా(res == pdTRUE);
    (శూన్య)res;

    ESP_LOGI(ట్యాగ్, "Starting SMP scheduler.");
    v_Task_ప్రారంభం_Scheduler();

    // This line should never be reached
    ప్రతిజ్ఞా(false);
}

శూన్య esp_vApplicationIdleHook(శూన్య)
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

శూన్య esp_vApplicationTickHook( శూన్య ) { }

#ఒకవేళ  (  configUSE_TICK_HOOK > 0 )
శూన్య vApplicationTickHook( శూన్య )
{
    esp_vApplicationTickHook();
}
#అయితే_ముగింపు

#ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )
/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
 * implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
 * used by the Idle task. */
శూన్య v_Application_Get_Idle_Task_జ్ఞాపకం( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                    దొంతర_Type_t ** ppx_Idle_Task_దొంతర_బఫరు,
                                    uint32_t * pul_Idle_Task_దొంతర_కొలత )
{
/* If the buffers to be provided to the Idle task are declared inside this
 * function then they must be declared static - otherwise they will be allocated on
 * the stack and so not exists after this function exits. */
    స్థిర StaticTask_t xIdleTaskTCB;
    స్థిర దొంతర_Type_t ux_Idle_Task_దొంతర[ config_కనీస_దొంతర_కొలత ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
     * state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppx_Idle_Task_దొంతర_బఫరు = ux_Idle_Task_దొంతర;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in bytes. */
    *pul_Idle_Task_దొంతర_కొలత = config_కనీస_దొంతర_కొలత;
}
#అయితే_ముగింపు // configSUPPORT_STATIC_ALLOCATION == 1
/*-----------------------------------------------------------*/

#ఒకవేళ ( (config_SUPPORT_STATIC_కేటాయింపు == 1) && (configUSE_TIMERS == 1))

/* When configSUPPORT_STATIC_ALLOCATION is set to 1 the application writer can
 * use a callback function to optionally provide the memory required by the idle
 * and timer tasks.  This is the stack that will be used by the timer task.  It is
 * declared here, as a global, so it can be checked by a test that is implemented
 * in a different file. */
దొంతర_Type_t ux_Timer_Task_దొంతర[ config_TIMER_TASK_దొంతర_లోతు ];


/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
 * application must provide an implementation of vApplicationGetTimerTaskMemory()
 * to provide the memory that is used by the Timer service task. */
శూన్య v_Application_Get_Timer_Task_జ్ఞాపకం( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                     దొంతర_Type_t ** ppx_Timer_Task_దొంతర_బఫరు,
                                     uint32_t * pul_Timer_Task_దొంతర_కొలత )
{
/* If the buffers to be provided to the Timer task are declared inside this
 * function then they must be declared static - otherwise they will be allocated on
 * the stack and so not exists after this function exits. */
    స్థిర StaticTask_t xTimerTaskTCB;

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
     * task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppx_Timer_Task_దొంతర_బఫరు = ux_Timer_Task_దొంతర;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in bytes. */
    *pul_Timer_Task_దొంతర_కొలత = config_TIMER_TASK_దొంతర_లోతు;
}
#అయితే_ముగింపు // configSUPPORT_STATIC_ALLOCATION == 1 && (configUSE_TIMERS == 1)

శూన్య v_పోర్టు_Take_తాళం( పోర్టు_MUX_TYPE *తాళం_2 )
{
    spinlock_acquire( తాళం_2, పోర్టు_MUX_NO_TIMEOUT);
}

శూన్య v_పోర్టు_Release_తాళం( పోర్టు_MUX_TYPE *తాళం_2 )
{
    spinlock_release( తాళం_2 );
}

#నిర్వచించు FREERTOS_SMP_MALLOC_CAPS    (MALLOC_CAP_అంతర్గత|MALLOC_CAP_8_బిట్టు)

శూన్య *pv_పోర్టు_Malloc( size_t x_కొలత )
{
    తిరిగి కుప్ప_caps_malloc(x_కొలత, FREERTOS_SMP_MALLOC_CAPS);
}

శూన్య v_పోర్టు_Free( శూన్య *pv )
{
    కుప్ప_caps_free(pv);
}

శూన్య __attribute__((weak)) v_Application_దొంతర_అధికప్రవాహం_Hook(TaskHandle_t xTask, అక్షరం *pcTaskName)
{
#నిర్వచించు ERR_STR1 "***ERROR*** A stack overflow in task "
#నిర్వచించు ERR_STR2 " has been detected."
    స్థిరరాశి అక్షరం *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    అక్షరం buf[పరిమాణం(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + పరిమాణం(ERR_STR2) + 1 /* null char */] = {0};

    అక్షరం *dest = buf;
    కొరకు (పూర్ణాంకం i = 0; i < పరిమాణం(str) / పరిమాణం(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
	రాయి("%s\n", buf);
    abort();
}

// ------- Thread Local Storage Pointers Deletion Callbacks -------

#ఒకవేళ ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
శూన్య v_పోర్టు_TLSPointers_Del_Cb( శూన్య *pxTCB )
{
    /* Typecast pxTCB to StaticTask_t type to access TCB struct members.
     * pvDummy15 corresponds to pvThreadLocalStoragePointers member of the TCB.
     */
    StaticTask_t *tcb = ( StaticTask_t * )pxTCB;

    /* The TLSP deletion callbacks are stored at an offset of (configNUM_THREAD_LOCAL_STORAGE_POINTERS/2) */
    TlsDeleteCallbackFunction_t *pv_Thread_Local_Storage_సూచికలు_Del_Callback = ( TlsDeleteCallbackFunction_t * )( &( tcb->pvDummy15[ ( config_అంకె_THREAD_LOCAL_STORAGE_సూచికలు / 2 ) ] ) );

    /* We need to iterate over half the depth of the pvThreadLocalStoragePointers area
     * to access all TLS pointers and their respective TLS deletion callbacks.
     */
    కొరకు ( పూర్ణాంకం x = 0; x < ( config_అంకె_THREAD_LOCAL_STORAGE_సూచికలు / 2 ); x++ ) {
        ఒకవేళ ( pv_Thread_Local_Storage_సూచికలు_Del_Callback[ x ] != ఖాళీ ) {  //If del cb is set
            // We skip the check if the callback is executable as that is difficult to determine for different
            // platforms (compare xtensa and riscv code).
            pv_Thread_Local_Storage_సూచికలు_Del_Callback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#అయితే_ముగింపు // CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS

శూన్య v_పోర్టు_Clean_పైకి_TCB ( శూన్య *pxTCB )
{
    #ఒకవేళ ( CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK )
        /* Call the user defined task pre-deletion hook */
        బాహ్య శూన్య vTaskPreDeletionHook( శూన్య * pxTCB );
        vTaskPreDeletionHook( pxTCB );
    #అయితే_ముగింపు /* CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK */

#ఒకవేళ ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
    /* Call TLS pointers deletion callbacks */
    v_పోర్టు_TLSPointers_Del_Cb( pxTCB );
#అయితే_ముగింపు /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

    v_పోర్టు_Cancel_Thread(pxTCB);
}
