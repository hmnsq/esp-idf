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

#ವ್ಯಾಖ್ಯಾನಿಸು SIG_RESUME SIGUSR1

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ THREAD
{
    pthread_t pthread;
    TaskFunction_t px_ಸಂಕೇತ;
    ಶೂನ್ಯ *pvParams;
    ತಳ_Type_t xDying;
    ರಚನೆ event *ev;
} Thread_t;

/*
 * The additional per-thread data is stored at the beginning of the
 * task's stack.
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ Thread_t *prvGetThreadFromTask(TaskHandle_t xTask)
{
    ರಾಶಿ_Type_t *px_ತುದಿ_ದ_ರಾಶಿ = *(ರಾಶಿ_Type_t **)xTask;

    ಹಿಂತಿರುಗಿ (Thread_t *)(px_ತುದಿ_ದ_ರಾಶಿ + 1);
}

/*-----------------------------------------------------------*/

ಸ್ಥಿರ pthread_once_t h_Sig_ಸಿದ್ಧತೆ_Thread = PTHREAD_ONCE_INIT;
ಸ್ಥಿರ sigset_t x_All_ಸಂಕೇತಗಳು;
ಸ್ಥಿರ sigset_t x_Scheduler_Original_Signal_ಮುಖವಾಡ;
ಸ್ಥಿರ pthread_t hMainThread = ( pthread_t )ಖಾಲಿ;

// These are saved as part of a thread's state in prvSwitchThread()
ಸ್ಥಿರ ಅಸ್ಥಿರ ತಳ_Type_t uxCriticalNestingIDF = 0;    /* Track nesting calls for IDF style critical sections. FreeRTOS critical section nesting is maintained in the TCB. */
ಸ್ಥಿರ ಅಸ್ಥಿರ UBaseType_t ux_ಅಡಚಣೆ_Nesting = 0;     /* Tracks if we are currently in an interrupt. */
ಸ್ಥಿರ ಅಸ್ಥಿರ ತಳ_Type_t ux_ಅಡಚಣೆ_Level = 0;        /* Tracks the current level (i.e., interrupt mask) */
/*-----------------------------------------------------------*/

ಸ್ಥಿರ ತಳ_Type_t x_Scheduler_ಕೊನೆ = pdFALSE;
/*-----------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ prv_ಸಿದ್ಧತೆ_ಸಂಕೇತಗಳು_And_Scheduler_Policy( ಶೂನ್ಯ );
ಸ್ಥಿರ ಶೂನ್ಯ prv_ಸಿದ್ಧತೆ_Timer_ಅಡಚಣೆ( ಶೂನ್ಯ );
ಸ್ಥಿರ ಶೂನ್ಯ *prv_Wait_For_ಆರಂಭ( ಶೂನ್ಯ * pvParams );
ಸ್ಥಿರ ಶೂನ್ಯ prvSwitchThread( Thread_t * x_Thread_ಗಮ್ಯ_Resume,
                             Thread_t *x_Thread_ಗಮ್ಯ_Suspend );
ಸ್ಥಿರ ಶೂನ್ಯ prvSuspendSelf( Thread_t * thread);
ಸ್ಥಿರ ಶೂನ್ಯ prvResumeThread( Thread_t * xThreadId );
ಸ್ಥಿರ ಶೂನ್ಯ v_ಪೋರ್ಟ್_System_Tick_Handler( ಪೂರ್ಣಾಂಕ sig );
ಸ್ಥಿರ ಶೂನ್ಯ v_ಪೋರ್ಟ್_ಆರಂಭ_ಮೊದಲ_Task( ಶೂನ್ಯ );
/*-----------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ prvFatalError( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pcCall, ಪೂರ್ಣಾಂಕ iErrno )
{
    fprintf( stderr, "%s: %s\n", pcCall, strerror( iErrno ) );
    abort();
}

/*
 * See header file for description.
 */
ರಾಶಿ_Type_t *px_ಪೋರ್ಟ್_Initialise_ರಾಶಿ( ರಾಶಿ_Type_t *px_ತುದಿ_ದ_ರಾಶಿ,
                                    ರಾಶಿ_Type_t *px_ಕೊನೆ_ದ_ರಾಶಿ,
                                    TaskFunction_t px_ಸಂಕೇತ,
                                    ಶೂನ್ಯ *pvParameters )
{
    Thread_t *thread;
    pthread_attr_t x_Thread_ಗುಣಲಕ್ಷಣಗಳು;
    size_t ul_ರಾಶಿ_ಗಾತ್ರ;
    ಪೂರ್ಣಾಂಕ iRet;

    (ಶೂನ್ಯ)pthread_once( &h_Sig_ಸಿದ್ಧತೆ_Thread, prv_ಸಿದ್ಧತೆ_ಸಂಕೇತಗಳು_And_Scheduler_Policy );

    /*
     * Store the additional thread data at the start of the stack.
     */
    thread = (Thread_t *)(px_ತುದಿ_ದ_ರಾಶಿ + 1) - 1;
    px_ತುದಿ_ದ_ರಾಶಿ = (ರಾಶಿ_Type_t *)thread - 1;
    ul_ರಾಶಿ_ಗಾತ್ರ = (px_ತುದಿ_ದ_ರಾಶಿ + 1 - px_ಕೊನೆ_ದ_ರಾಶಿ) * ಗಾತ್ರ(*px_ತುದಿ_ದ_ರಾಶಿ);

    thread->px_ಸಂಕೇತ = px_ಸಂಕೇತ;
    thread->pvParams = pvParameters;
    thread->xDying = pdFALSE;

    pthread_attr_init( &x_Thread_ಗುಣಲಕ್ಷಣಗಳು );
    pthread_attr_setstack( &x_Thread_ಗುಣಲಕ್ಷಣಗಳು, px_ಕೊನೆ_ದ_ರಾಶಿ, ul_ರಾಶಿ_ಗಾತ್ರ );

    thread->ev = event_create();

    ತಳ_Type_t prev_intr_level = x_ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ();

    iRet = pthread_create( &thread->pthread, &x_Thread_ಗುಣಲಕ್ಷಣಗಳು,
                           prv_Wait_For_ಆರಂಭ, thread );
    ಆದರೆ ( iRet )
    {
        prvFatalError( "pthread_create", iRet );
    }

    v_ಪೋರ್ಟ್_Clear_ಅಡಚಣೆ_ಮುಖವಾಡ( prev_intr_level );

    ಹಿಂತಿರುಗಿ px_ತುದಿ_ದ_ರಾಶಿ;
}
/*-----------------------------------------------------------*/

ಶೂನ್ಯ v_ಪೋರ್ಟ್_ಆರಂಭ_ಮೊದಲ_Task( ಶೂನ್ಯ )
{
    Thread_t *px_ಮೊದಲ_Thread = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    /* Start the first task. */
    prvResumeThread( px_ಮೊದಲ_Thread );
}
/*-----------------------------------------------------------*/

/*
 * See header file for description.
 */
ತಳ_Type_t x_ಪೋರ್ಟ್_ಆರಂಭ_Scheduler( ಶೂನ್ಯ )
{
    ಪೂರ್ಣಾಂಕ iSignal;
    sigset_t x_ಸಂಕೇತಗಳು;

    hMainThread = pthread_self();

    /* Start the timer that generates the tick ISR(SIGALRM).
       Interrupts are disabled here already. */
    prv_ಸಿದ್ಧತೆ_Timer_ಅಡಚಣೆ();

    /* Start the first task. */
    v_ಪೋರ್ಟ್_ಆರಂಭ_ಮೊದಲ_Task();

    /* Wait until signaled by vPortEndScheduler(). */
    sigemptyset( &x_ಸಂಕೇತಗಳು );
    sigaddset( &x_ಸಂಕೇತಗಳು, SIG_RESUME );

    ಆಗ ( !x_Scheduler_ಕೊನೆ )
    {
        sigwait( &x_ಸಂಕೇತಗಳು, &iSignal );
    }

    /* Cancel the Idle task and free its resources */
#ಆದರೆ ( INCLUDE_xTaskGetIdleTaskHandle == 1 )
    v_ಪೋರ್ಟ್_Cancel_Thread( xTaskGetIdleTaskHandle() );
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ ( configUSE_TIMERS == 1 )
    /* Cancel the Timer task and free its resources */
    v_ಪೋರ್ಟ್_Cancel_Thread( xTimerGetTimerDaemonTaskHandle() );
#ಆದರೆ_ಅಂತ್ಯ /* configUSE_TIMERS */

    /* Restore original signal mask. */
    (ಶೂನ್ಯ)pthread_sigmask( SIG_SETMASK, &x_Scheduler_Original_Signal_ಮುಖವಾಡ,  ಖಾಲಿ );

    ಹಿಂತಿರುಗಿ 0;
}
/*-----------------------------------------------------------*/

ಶೂನ್ಯ v_ಪೋರ್ಟ್_ಕೊನೆ_Scheduler( ಶೂನ್ಯ )
{
    ರಚನೆ itimerval itimer;
    ರಚನೆ sigaction sigtick;
    Thread_t *xCurrentThread;

    /* Stop the timer and ignore any pending SIGALRMs that would end
     * up running on the main thread when it is resumed. */
    itimer.it_value.tv_sec = 0;
    itimer.it_value.tv_usec = 0;

    itimer.it_interval.tv_sec = 0;
    itimer.it_interval.tv_usec = 0;
    (ಶೂನ್ಯ)setitimer( ITIMER_REAL, &itimer, ಖಾಲಿ );

    sigtick.sa_flags = 0;
    sigtick.sa_handler = SIG_IGN;
    sigemptyset( &sigtick.sa_mask );
    sigaction( SIGALRM, &sigtick, ಖಾಲಿ );

    /* Signal the scheduler to exit its loop. */
    x_Scheduler_ಕೊನೆ = pdTRUE;
    (ಶೂನ್ಯ)pthread_kill( hMainThread, SIG_RESUME );

    xCurrentThread = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );
    prvSuspendSelf(xCurrentThread);
}
/*-----------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ v_ಪೋರ್ಟ್_Disable_Interrupts( ಶೂನ್ಯ )
{
    pthread_sigmask( SIG_BLOCK, &x_All_ಸಂಕೇತಗಳು, ಖಾಲಿ );
}
/*-----------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ v_ಪೋರ್ಟ್_Enable_Interrupts( ಶೂನ್ಯ )
{
    pthread_sigmask( SIG_UNBLOCK, &x_All_ಸಂಕೇತಗಳು, ಖಾಲಿ );
}
/*-----------------------------------------------------------*/

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Enter_Critical_IDF( ಶೂನ್ಯ )
{
    ಆದರೆ ( uxCriticalNestingIDF == 0 && ux_ಅಡಚಣೆ_Level == 0)
    {
        v_ಪೋರ್ಟ್_Disable_Interrupts();
    }
    uxCriticalNestingIDF++;
}
/*-----------------------------------------------------------*/

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Exit_Critical_IDF( ಶೂನ್ಯ )
{
    uxCriticalNestingIDF--;

    /* If we have reached 0 then re-enable the interrupts. */
    ಆದರೆ( uxCriticalNestingIDF == 0 && ux_ಅಡಚಣೆ_Level == 0)
    {
        v_ಪೋರ್ಟ್_Enable_Interrupts();
    }
}
/*-----------------------------------------------------------*/

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Yield_From_ISR( ಶೂನ್ಯ )
{
    Thread_t *x_Thread_ಗಮ್ಯ_Suspend;
    Thread_t *x_Thread_ಗಮ್ಯ_Resume;

    x_Thread_ಗಮ್ಯ_Suspend = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    vTaskSwitchContext();

    x_Thread_ಗಮ್ಯ_Resume = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    prvSwitchThread( x_Thread_ಗಮ್ಯ_Resume, x_Thread_ಗಮ್ಯ_Suspend );
}
/*-----------------------------------------------------------*/

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Yield( ಶೂನ್ಯ )
{
    ತಳ_Type_t prev_intr_level = x_ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ();

    v_ಪೋರ್ಟ್_Yield_From_ISR();

    v_ಪೋರ್ಟ್_Clear_ಅಡಚಣೆ_ಮುಖವಾಡ( prev_intr_level );
}
/*-----------------------------------------------------------*/

/* In SMP code, the disable/enable interrupt macros are calling the set/get interrupt mask functions below.
   Hence, we need to call vPortDisableInterrupts() and vPortEnableInterrupts(), otherwise interrupts
   are never disabled/enabled. */

ತಳ_Type_t x_ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ( ಶೂನ್ಯ )
{
    ಆದರೆ (ux_ಅಡಚಣೆ_Level == 0 && uxCriticalNestingIDF == 0) {
        v_ಪೋರ್ಟ್_Disable_Interrupts();
    }
    ತಳ_Type_t prev_intr_level = ux_ಅಡಚಣೆ_Level;
    ux_ಅಡಚಣೆ_Level++;
    ಹಿಂತಿರುಗಿ prev_intr_level;
}
/*-----------------------------------------------------------*/

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Clear_ಅಡಚಣೆ_ಮುಖವಾಡ( ತಳ_Type_t x_ಮುಖವಾಡ )
{
    // Only re-enable interrupts if xMask is 0
    ux_ಅಡಚಣೆ_Level = x_ಮುಖವಾಡ;
    ಆದರೆ (ux_ಅಡಚಣೆ_Level == 0 && uxCriticalNestingIDF == 0) {
        v_ಪೋರ್ಟ್_Enable_Interrupts();
    }
}
/*-----------------------------------------------------------*/

ಸ್ಥಿರ uint64_t prv_Get_ಸಮಯ_Ns(ಶೂನ್ಯ)
{
    ರಚನೆ timespec t;

    clock_gettime(CLOCK_MONOTONIC, &t);

    ಹಿಂತಿರುಗಿ t.tv_sec * 1000000000ull + t.tv_nsec;
}

ಸ್ಥಿರ uint64_t prv_ಆರಂಭ_ಸಮಯ_Ns;
/* commented as part of the code below in vPortSystemTickHandler,
 * to adjust timing according to full demo requirements */
/* static uint64_t prvTickCount; */

/*
 * Setup the systick timer to generate the tick interrupts at the required
 * frequency.
 */
ಶೂನ್ಯ prv_ಸಿದ್ಧತೆ_Timer_ಅಡಚಣೆ( ಶೂನ್ಯ )
{
    ರಚನೆ itimerval itimer;
    ಪೂರ್ಣಾಂಕ iRet;

    /* Initialise the structure with the current timer information. */
    iRet = getitimer( ITIMER_REAL, &itimer );
    ಆದರೆ ( iRet )
    {
        prvFatalError( "getitimer", errno );
    }

    /* Set the interval between timer events. */
    itimer.it_interval.tv_sec = 0;
    itimer.it_interval.tv_usec = ಪೋರ್ಟ್_TICK_ದರ_MICROSECONDS;

    /* Set the current count-down. */
    itimer.it_value.tv_sec = 0;
    itimer.it_value.tv_usec = ಪೋರ್ಟ್_TICK_ದರ_MICROSECONDS;

    /* Set-up the timer interrupt. */
    iRet = setitimer( ITIMER_REAL, &itimer, ಖಾಲಿ );
    ಆದರೆ ( iRet )
    {
        prvFatalError( "setitimer", errno );
    }

    prv_ಆರಂಭ_ಸಮಯ_Ns = prv_Get_ಸಮಯ_Ns();
}
/*-----------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ v_ಪೋರ್ಟ್_System_Tick_Handler( ಪೂರ್ಣಾಂಕ sig )
{
    Thread_t *px_Thread_ಗಮ್ಯ_Suspend;
    Thread_t *px_Thread_ಗಮ್ಯ_Resume;
    ತಳ_Type_t xSwitchRequired;
    /* uint64_t xExpectedTicks; */

    // Handling a timer signal, so we are currently in an interrupt.
    ux_ಅಡಚಣೆ_Nesting++;

#ಆದರೆ ( configUSE_PREEMPTION == 1 )
    px_Thread_ಗಮ್ಯ_Suspend = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );
#ಆದರೆ_ಅಂತ್ಯ

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

#ಆದರೆ ( configUSE_PREEMPTION == 1 )
    ಆದರೆ (xSwitchRequired == pdTRUE) {
        /* Select Next Task. */
        vTaskSwitchContext();

        px_Thread_ಗಮ್ಯ_Resume = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

        prvSwitchThread(px_Thread_ಗಮ್ಯ_Resume, px_Thread_ಗಮ್ಯ_Suspend);
    }
#ಇಲ್ಲದಿದ್ದರೆ
    (ಶೂನ್ಯ)xSwitchRequired;
#ಆದರೆ_ಅಂತ್ಯ

    // Returning from the timer signal handler, so we are exiting the interrupt.
    ux_ಅಡಚಣೆ_Nesting--;
}
/*-----------------------------------------------------------*/

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Thread_Dying( ಶೂನ್ಯ *px_Task_ಗಮ್ಯ_Delete, ಅಸ್ಥಿರ ತಳ_Type_t *pxPendYield )
{
    Thread_t *pxThread = prvGetThreadFromTask( px_Task_ಗಮ್ಯ_Delete );

    pxThread->xDying = pdTRUE;
}

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Cancel_Thread( ಶೂನ್ಯ *px_Task_ಗಮ್ಯ_Delete )
{
    Thread_t *px_Thread_ಗಮ್ಯ_Cancel = prvGetThreadFromTask( px_Task_ಗಮ್ಯ_Delete );

    /*
     * The thread has already been suspended so it can be safely cancelled.
     */
    pthread_cancel( px_Thread_ಗಮ್ಯ_Cancel->pthread );
    pthread_join( px_Thread_ಗಮ್ಯ_Cancel->pthread, ಖಾಲಿ );
    event_delete( px_Thread_ಗಮ್ಯ_Cancel->ev );
}
/*-----------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ *prv_Wait_For_ಆರಂಭ( ಶೂನ್ಯ * pvParams )
{
    Thread_t *pxThread = pvParams;

    prvSuspendSelf(pxThread);

    /* Resumed for the first time, thus this thread didn't previously call
     * prvSwitchThread(). So we need to initialise the state variables for this
     * thread. */
    uxCriticalNestingIDF = 0;
    ux_ಅಡಚಣೆ_Nesting = 0;
    ux_ಅಡಚಣೆ_Level = 0;
    v_ಪೋರ್ಟ್_Enable_Interrupts();

    /* Call the task's entry point. */
    pxThread->px_ಸಂಕೇತ( pxThread->pvParams );

    /* A function that implements a task must not exit or attempt to return to
    * its caller as there is nothing to return to. If a task wants to exit it
    * should instead call vTaskDelete( NULL ). Artificially force an assert()
    * to be triggered if configASSERT() is defined, so application writers can
        * catch the error. */
    configASSERT( pdFALSE );

    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}
/*-----------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ prvSwitchThread( Thread_t *px_Thread_ಗಮ್ಯ_Resume,
                             Thread_t *px_Thread_ಗಮ್ಯ_Suspend )
{
    ತಳ_Type_t uxSavedCriticalNestingIDF;
    ತಳ_Type_t ux_Saved_ಅಡಚಣೆ_Nesting;
    ತಳ_Type_t ux_Saved_ಅಡಚಣೆ_Level;

    ಆದರೆ ( px_Thread_ಗಮ್ಯ_Suspend != px_Thread_ಗಮ್ಯ_Resume )
    {
        /* It is possible for prvSwitchThread() to be called...
         * - while inside an ISR (i.e., via vPortSystemTickHandler() or vPortYieldFromISR())
         * - while interrupts are disabled or in a critical section (i.e., via vPortYield())
         *
         * So we need to save the various count variables as part of the thread's context.
         * They are restored when the pthread switches back. */
        uxSavedCriticalNestingIDF = uxCriticalNestingIDF;
        ux_Saved_ಅಡಚಣೆ_Nesting = ux_ಅಡಚಣೆ_Nesting;
        ux_Saved_ಅಡಚಣೆ_Level = ux_ಅಡಚಣೆ_Level;

        prvResumeThread( px_Thread_ಗಮ್ಯ_Resume );
        ಆದರೆ ( px_Thread_ಗಮ್ಯ_Suspend->xDying )
        {
            pthread_exit( ಖಾಲಿ );
        }
        prvSuspendSelf( px_Thread_ಗಮ್ಯ_Suspend );

        uxCriticalNestingIDF = uxSavedCriticalNestingIDF;
        ux_ಅಡಚಣೆ_Nesting = ux_Saved_ಅಡಚಣೆ_Nesting;
        ux_ಅಡಚಣೆ_Level = ux_Saved_ಅಡಚಣೆ_Level;
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
    ಆದರೆ ( pthread_self() != xThreadId->pthread )
    {
        event_signal(xThreadId->ev);
    }
}
/*-----------------------------------------------------------*/

ಸ್ಥಿರ ಶೂನ್ಯ prv_ಸಿದ್ಧತೆ_ಸಂಕೇತಗಳು_And_Scheduler_Policy( ಶೂನ್ಯ )
{
    ರಚನೆ sigaction sigresume, sigtick;
    ಪೂರ್ಣಾಂಕ iRet;

    hMainThread = pthread_self();

    /* Initialise common signal masks. */
    sigfillset( &x_All_ಸಂಕೇತಗಳು );
    /* Don't block SIGINT so this can be used to break into GDB while
     * in a critical section. */
    sigdelset( &x_All_ಸಂಕೇತಗಳು, SIGINT );

    /*
     * Block all signals in this thread so all new threads
     * inherits this mask.
     *
     * When a thread is resumed for the first time, all signals
     * will be unblocked.
     */
    (ಶೂನ್ಯ)pthread_sigmask( SIG_SETMASK, &x_All_ಸಂಕೇತಗಳು,
                           &x_Scheduler_Original_Signal_ಮುಖವಾಡ );

    /* SIG_RESUME is only used with sigwait() so doesn't need a
       handler. */
    sigresume.sa_flags = 0;
    sigresume.sa_handler = SIG_IGN;
    sigfillset( &sigresume.sa_mask );

    sigtick.sa_flags = 0;
    sigtick.sa_handler = v_ಪೋರ್ಟ್_System_Tick_Handler;
    sigfillset( &sigtick.sa_mask );

    iRet = sigaction( SIG_RESUME, &sigresume, ಖಾಲಿ );
    ಆದರೆ ( iRet )
    {
        prvFatalError( "sigaction", errno );
    }

    iRet = sigaction( SIGALRM, &sigtick, ಖಾಲಿ );
    ಆದರೆ ( iRet )
    {
        prvFatalError( "sigaction", errno );
    }
}
/*-----------------------------------------------------------*/

ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ul_ಪೋರ್ಟ್_Get_ಸತತ_ಸಮಯ( ಶೂನ್ಯ )
{
    ರಚನೆ tms xTimes;

    times( &xTimes );

    ಹಿಂತಿರುಗಿ ( ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ) xTimes.tms_utime;
}
/*-----------------------------------------------------------*/
bool ಪೋರ್ಟ್_VALID_LIST_MEM(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ptr)
{
    ಹಿಂತಿರುಗಿ true;
}

bool ಪೋರ್ಟ್_VALID_TCB_MEM(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ptr)
{
    ಹಿಂತಿರುಗಿ true;
}

bool ಪೋರ್ಟ್_VALID_ರಾಶಿ_MEM(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ptr)
{
    ಹಿಂತಿರುಗಿ true;
}
/*-----------------------------------------------------------*/

ಪೋರ್ಟ್_MUX_TYPE ಪೋರ್ಟ್_x_Task_ಬೀಗ = ಪೋರ್ಟ್_MUX_INITIALIZER_UNLOCKED;
ಪೋರ್ಟ್_MUX_TYPE ಪೋರ್ಟ್_x_ISRLock = ಪೋರ್ಟ್_MUX_INITIALIZER_UNLOCKED;

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಟ್ಯಾಗ್ = "port";

ತಳ_Type_t x_ಪೋರ್ಟ್_Check_If_ಒಳ_ISR(ಶೂನ್ಯ)
{
    ಹಿಂತಿರುಗಿ (ux_ಅಡಚಣೆ_Nesting == 0) ? pdFALSE : pdTRUE;
}

ಶೂನ್ಯ app_main(ಶೂನ್ಯ);

ಸ್ಥಿರ ಶೂನ್ಯ main_task(ಶೂನ್ಯ* ವಾದಗಳು)
{
    app_main();
    vTaskDelete(ಖಾಲಿ);
}

ಪೂರ್ಣಾಂಕ ಮುಖ್ಯ(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **argv)
{
    // This makes sure that stdio is flushed after each '\n' so that idf.py monitor
    // reads the program output on time.
    setvbuf(stdout, ಖಾಲಿ, _IOLBF, 0);

    usleep(1000);

    ತಳ_Type_t res;

#ಆದರೆ ( config_ಅಂಕೆ_CORES > 1 )
    res = x_Task_Create_Affinity_ಗಣ(&main_task, "main",
                                 ESP_TASK_MAIN_ರಾಶಿ, ಖಾಲಿ,
                                 ESP_TASK_MAIN_PRIO, ESP_TASK_MAIN_CORE, ಖಾಲಿ);
#ಇಲ್ಲದಿದ್ದರೆ
    res = xTaskCreate(&main_task, "main",
                      ESP_TASK_MAIN_ರಾಶಿ, ಖಾಲಿ,
                      ESP_TASK_MAIN_PRIO, ಖಾಲಿ);
#ಆದರೆ_ಅಂತ್ಯ

    ಪ್ರತಿಜ್ಞಾ(res == pdTRUE);
    (ಶೂನ್ಯ)res;

    ESP_LOGI(ಟ್ಯಾಗ್, "Starting SMP scheduler.");
    v_Task_ಆರಂಭ_Scheduler();

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

#ಆದರೆ  (  configUSE_TICK_HOOK > 0 )
ಶೂನ್ಯ vApplicationTickHook( ಶೂನ್ಯ )
{
    esp_vApplicationTickHook();
}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ ( config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 1 )
/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
 * implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
 * used by the Idle task. */
ಶೂನ್ಯ v_Application_Get_Idle_Task_ಸ್ಮರಣೆ( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                    ರಾಶಿ_Type_t ** ppx_Idle_Task_ರಾಶಿ_ಬಫರ್,
                                    uint32_t * pul_Idle_Task_ರಾಶಿ_ಗಾತ್ರ )
{
/* If the buffers to be provided to the Idle task are declared inside this
 * function then they must be declared static - otherwise they will be allocated on
 * the stack and so not exists after this function exits. */
    ಸ್ಥಿರ StaticTask_t xIdleTaskTCB;
    ಸ್ಥಿರ ರಾಶಿ_Type_t ux_Idle_Task_ರಾಶಿ[ config_ಕನಿಷ್ಠತಮ_ರಾಶಿ_ಗಾತ್ರ ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
     * state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppx_Idle_Task_ರಾಶಿ_ಬಫರ್ = ux_Idle_Task_ರಾಶಿ;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in bytes. */
    *pul_Idle_Task_ರಾಶಿ_ಗಾತ್ರ = config_ಕನಿಷ್ಠತಮ_ರಾಶಿ_ಗಾತ್ರ;
}
#ಆದರೆ_ಅಂತ್ಯ // configSUPPORT_STATIC_ALLOCATION == 1
/*-----------------------------------------------------------*/

#ಆದರೆ ( (config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 1) && (configUSE_TIMERS == 1))

/* When configSUPPORT_STATIC_ALLOCATION is set to 1 the application writer can
 * use a callback function to optionally provide the memory required by the idle
 * and timer tasks.  This is the stack that will be used by the timer task.  It is
 * declared here, as a global, so it can be checked by a test that is implemented
 * in a different file. */
ರಾಶಿ_Type_t ux_Timer_Task_ರಾಶಿ[ config_TIMER_TASK_ರಾಶಿ_ಆಳ ];


/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
 * application must provide an implementation of vApplicationGetTimerTaskMemory()
 * to provide the memory that is used by the Timer service task. */
ಶೂನ್ಯ v_Application_Get_Timer_Task_ಸ್ಮರಣೆ( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                     ರಾಶಿ_Type_t ** ppx_Timer_Task_ರಾಶಿ_ಬಫರ್,
                                     uint32_t * pul_Timer_Task_ರಾಶಿ_ಗಾತ್ರ )
{
/* If the buffers to be provided to the Timer task are declared inside this
 * function then they must be declared static - otherwise they will be allocated on
 * the stack and so not exists after this function exits. */
    ಸ್ಥಿರ StaticTask_t xTimerTaskTCB;

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
     * task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppx_Timer_Task_ರಾಶಿ_ಬಫರ್ = ux_Timer_Task_ರಾಶಿ;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in bytes. */
    *pul_Timer_Task_ರಾಶಿ_ಗಾತ್ರ = config_TIMER_TASK_ರಾಶಿ_ಆಳ;
}
#ಆದರೆ_ಅಂತ್ಯ // configSUPPORT_STATIC_ALLOCATION == 1 && (configUSE_TIMERS == 1)

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Take_ಬೀಗ( ಪೋರ್ಟ್_MUX_TYPE *ಬೀಗ_2 )
{
    spinlock_acquire( ಬೀಗ_2, ಪೋರ್ಟ್_MUX_NO_TIMEOUT);
}

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Release_ಬೀಗ( ಪೋರ್ಟ್_MUX_TYPE *ಬೀಗ_2 )
{
    spinlock_release( ಬೀಗ_2 );
}

#ವ್ಯಾಖ್ಯಾನಿಸು FREERTOS_SMP_MALLOC_CAPS    (MALLOC_CAP_ಆಂತರಿಕ|MALLOC_CAP_8_ಬಿಟ್)

ಶೂನ್ಯ *pv_ಪೋರ್ಟ್_Malloc( size_t x_ಗಾತ್ರ )
{
    ಹಿಂತಿರುಗಿ ರಾಶಿ_caps_malloc(x_ಗಾತ್ರ, FREERTOS_SMP_MALLOC_CAPS);
}

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Free( ಶೂನ್ಯ *pv )
{
    ರಾಶಿ_caps_free(pv);
}

ಶೂನ್ಯ __attribute__((weak)) v_Application_ರಾಶಿ_ಅತಿಪ್ರವಾಹ_Hook(TaskHandle_t xTask, ಅಕ್ಷರ *pcTaskName)
{
#ವ್ಯಾಖ್ಯಾನಿಸು ERR_STR1 "***ERROR*** A stack overflow in task "
#ವ್ಯಾಖ್ಯಾನಿಸು ERR_STR2 " has been detected."
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    ಅಕ್ಷರ buf[ಗಾತ್ರ(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + ಗಾತ್ರ(ERR_STR2) + 1 /* null char */] = {0};

    ಅಕ್ಷರ *dest = buf;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಗಾತ್ರ(str) / ಗಾತ್ರ(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
	ಬರೆ("%s\n", buf);
    abort();
}

// ------- Thread Local Storage Pointers Deletion Callbacks -------

#ಆದರೆ ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
ಶೂನ್ಯ v_ಪೋರ್ಟ್_TLSPointers_Del_Cb( ಶೂನ್ಯ *pxTCB )
{
    /* Typecast pxTCB to StaticTask_t type to access TCB struct members.
     * pvDummy15 corresponds to pvThreadLocalStoragePointers member of the TCB.
     */
    StaticTask_t *tcb = ( StaticTask_t * )pxTCB;

    /* The TLSP deletion callbacks are stored at an offset of (configNUM_THREAD_LOCAL_STORAGE_POINTERS/2) */
    TlsDeleteCallbackFunction_t *pv_Thread_Local_Storage_ಸೂಚಕಗಳು_Del_Callback = ( TlsDeleteCallbackFunction_t * )( &( tcb->pvDummy15[ ( config_ಅಂಕೆ_THREAD_LOCAL_STORAGE_ಸೂಚಕಗಳು / 2 ) ] ) );

    /* We need to iterate over half the depth of the pvThreadLocalStoragePointers area
     * to access all TLS pointers and their respective TLS deletion callbacks.
     */
    ಗಾಗಿ ( ಪೂರ್ಣಾಂಕ x = 0; x < ( config_ಅಂಕೆ_THREAD_LOCAL_STORAGE_ಸೂಚಕಗಳು / 2 ); x++ ) {
        ಆದರೆ ( pv_Thread_Local_Storage_ಸೂಚಕಗಳು_Del_Callback[ x ] != ಖಾಲಿ ) {  //If del cb is set
            // We skip the check if the callback is executable as that is difficult to determine for different
            // platforms (compare xtensa and riscv code).
            pv_Thread_Local_Storage_ಸೂಚಕಗಳು_Del_Callback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#ಆದರೆ_ಅಂತ್ಯ // CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Clean_ಮೇಲೆ_TCB ( ಶೂನ್ಯ *pxTCB )
{
    #ಆದರೆ ( CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK )
        /* Call the user defined task pre-deletion hook */
        ಬಾಹ್ಯ ಶೂನ್ಯ vTaskPreDeletionHook( ಶೂನ್ಯ * pxTCB );
        vTaskPreDeletionHook( pxTCB );
    #ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK */

#ಆದರೆ ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
    /* Call TLS pointers deletion callbacks */
    v_ಪೋರ್ಟ್_TLSPointers_Del_Cb( pxTCB );
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

    v_ಪೋರ್ಟ್_Cancel_Thread(pxTCB);
}
