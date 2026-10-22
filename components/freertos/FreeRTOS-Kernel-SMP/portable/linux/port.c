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

#परिभाषय SIG_RESUME SIGUSR1

प्रकारनिर्देश संरचना THREAD
{
    pthread_t pthread;
    TaskFunction_t px_संहिता;
    शून्य *pvParams;
    आधारः_Type_t xDying;
    संरचना event *ev;
} Thread_t;

/*
 * The additional per-thread data is stored at the beginning of the
 * task's stack.
 */
स्थिर अन्तःस्थ Thread_t *prvGetThreadFromTask(TaskHandle_t xTask)
{
    राशिः_Type_t *px_शिखरम्_सम्बन्धि_राशिः = *(राशिः_Type_t **)xTask;

    प्रत्यागम (Thread_t *)(px_शिखरम्_सम्बन्धि_राशिः + 1);
}

/*-----------------------------------------------------------*/

स्थिर pthread_once_t h_Sig_सज्जता_Thread = PTHREAD_ONCE_INIT;
स्थिर sigset_t x_All_सङ्केताः;
स्थिर sigset_t x_Scheduler_Original_Signal_आवरणम्;
स्थिर pthread_t hMainThread = ( pthread_t )रिक्त;

// These are saved as part of a thread's state in prvSwitchThread()
स्थिर चञ्चल आधारः_Type_t uxCriticalNestingIDF = 0;    /* Track nesting calls for IDF style critical sections. FreeRTOS critical section nesting is maintained in the TCB. */
स्थिर चञ्चल UBaseType_t ux_व्यवधानम्_Nesting = 0;     /* Tracks if we are currently in an interrupt. */
स्थिर चञ्चल आधारः_Type_t ux_व्यवधानम्_Level = 0;        /* Tracks the current level (i.e., interrupt mask) */
/*-----------------------------------------------------------*/

स्थिर आधारः_Type_t x_Scheduler_अन्तः = pdFALSE;
/*-----------------------------------------------------------*/

स्थिर शून्य prv_सज्जता_सङ्केताः_And_Scheduler_Policy( शून्य );
स्थिर शून्य prv_सज्जता_Timer_व्यवधानम्( शून्य );
स्थिर शून्य *prvWaitForStart( शून्य * pvParams );
स्थिर शून्य prvSwitchThread( Thread_t * x_Thread_गन्तव्यम्_Resume,
                             Thread_t *x_Thread_गन्तव्यम्_Suspend );
स्थिर शून्य prvSuspendSelf( Thread_t * thread);
स्थिर शून्य prvResumeThread( Thread_t * xThreadId );
स्थिर शून्य v_पत्तनम्_System_Tick_Handler( पूर्णाङ्क sig );
स्थिर शून्य v_पत्तनम्_Start_First_Task( शून्य );
/*-----------------------------------------------------------*/

स्थिर शून्य prvFatalError( ध्रुव वर्ण *pcCall, पूर्णाङ्क iErrno )
{
    fprintf( stderr, "%s: %s\n", pcCall, strerror( iErrno ) );
    abort();
}

/*
 * See header file for description.
 */
राशिः_Type_t *px_पत्तनम्_Initialise_राशिः( राशिः_Type_t *px_शिखरम्_सम्बन्धि_राशिः,
                                    राशिः_Type_t *px_अन्तः_सम्बन्धि_राशिः,
                                    TaskFunction_t px_संहिता,
                                    शून्य *pvParameters )
{
    Thread_t *thread;
    pthread_attr_t x_Thread_गुणाः;
    size_t ul_राशिः_आकार;
    पूर्णाङ्क iRet;

    (शून्य)pthread_once( &h_Sig_सज्जता_Thread, prv_सज्जता_सङ्केताः_And_Scheduler_Policy );

    /*
     * Store the additional thread data at the start of the stack.
     */
    thread = (Thread_t *)(px_शिखरम्_सम्बन्धि_राशिः + 1) - 1;
    px_शिखरम्_सम्बन्धि_राशिः = (राशिः_Type_t *)thread - 1;
    ul_राशिः_आकार = (px_शिखरम्_सम्बन्धि_राशिः + 1 - px_अन्तः_सम्बन्धि_राशिः) * परिमाण(*px_शिखरम्_सम्बन्धि_राशिः);

    thread->px_संहिता = px_संहिता;
    thread->pvParams = pvParameters;
    thread->xDying = pdFALSE;

    pthread_attr_init( &x_Thread_गुणाः );
    pthread_attr_setstack( &x_Thread_गुणाः, px_अन्तः_सम्बन्धि_राशिः, ul_राशिः_आकार );

    thread->ev = event_create();

    आधारः_Type_t पूर्व_intr_level = x_पत्तनम्_समुच्चयः_व्यवधानम्_आवरणम्();

    iRet = pthread_create( &thread->pthread, &x_Thread_गुणाः,
                           prvWaitForStart, thread );
    यदि ( iRet )
    {
        prvFatalError( "pthread_create", iRet );
    }

    v_पत्तनम्_Clear_व्यवधानम्_आवरणम्( पूर्व_intr_level );

    प्रत्यागम px_शिखरम्_सम्बन्धि_राशिः;
}
/*-----------------------------------------------------------*/

शून्य v_पत्तनम्_Start_First_Task( शून्य )
{
    Thread_t *pxFirstThread = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    /* Start the first task. */
    prvResumeThread( pxFirstThread );
}
/*-----------------------------------------------------------*/

/*
 * See header file for description.
 */
आधारः_Type_t x_पत्तनम्_Start_Scheduler( शून्य )
{
    पूर्णाङ्क iSignal;
    sigset_t x_सङ्केताः;

    hMainThread = pthread_self();

    /* Start the timer that generates the tick ISR(SIGALRM).
       Interrupts are disabled here already. */
    prv_सज्जता_Timer_व्यवधानम्();

    /* Start the first task. */
    v_पत्तनम्_Start_First_Task();

    /* Wait until signaled by vPortEndScheduler(). */
    sigemptyset( &x_सङ्केताः );
    sigaddset( &x_सङ्केताः, SIG_RESUME );

    यावत् ( !x_Scheduler_अन्तः )
    {
        sigwait( &x_सङ्केताः, &iSignal );
    }

    /* Cancel the Idle task and free its resources */
#यदि ( INCLUDE_xTaskGetIdleTaskHandle == 1 )
    v_पत्तनम्_Cancel_Thread( xTaskGetIdleTaskHandle() );
#यदि_अन्तः

#यदि ( configUSE_TIMERS == 1 )
    /* Cancel the Timer task and free its resources */
    v_पत्तनम्_Cancel_Thread( xTimerGetTimerDaemonTaskHandle() );
#यदि_अन्तः /* configUSE_TIMERS */

    /* Restore original signal mask. */
    (शून्य)pthread_sigmask( SIG_SETMASK, &x_Scheduler_Original_Signal_आवरणम्,  रिक्त );

    प्रत्यागम 0;
}
/*-----------------------------------------------------------*/

शून्य v_पत्तनम्_अन्तः_Scheduler( शून्य )
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
    x_Scheduler_अन्तः = pdTRUE;
    (शून्य)pthread_kill( hMainThread, SIG_RESUME );

    xCurrentThread = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );
    prvSuspendSelf(xCurrentThread);
}
/*-----------------------------------------------------------*/

स्थिर शून्य v_पत्तनम्_Disable_Interrupts( शून्य )
{
    pthread_sigmask( SIG_BLOCK, &x_All_सङ्केताः, रिक्त );
}
/*-----------------------------------------------------------*/

स्थिर शून्य v_पत्तनम्_Enable_Interrupts( शून्य )
{
    pthread_sigmask( SIG_UNBLOCK, &x_All_सङ्केताः, रिक्त );
}
/*-----------------------------------------------------------*/

शून्य v_पत्तनम्_Enter_Critical_IDF( शून्य )
{
    यदि ( uxCriticalNestingIDF == 0 && ux_व्यवधानम्_Level == 0)
    {
        v_पत्तनम्_Disable_Interrupts();
    }
    uxCriticalNestingIDF++;
}
/*-----------------------------------------------------------*/

शून्य v_पत्तनम्_Exit_Critical_IDF( शून्य )
{
    uxCriticalNestingIDF--;

    /* If we have reached 0 then re-enable the interrupts. */
    यदि( uxCriticalNestingIDF == 0 && ux_व्यवधानम्_Level == 0)
    {
        v_पत्तनम्_Enable_Interrupts();
    }
}
/*-----------------------------------------------------------*/

शून्य v_पत्तनम्_Yield_From_ISR( शून्य )
{
    Thread_t *x_Thread_गन्तव्यम्_Suspend;
    Thread_t *x_Thread_गन्तव्यम्_Resume;

    x_Thread_गन्तव्यम्_Suspend = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    vTaskSwitchContext();

    x_Thread_गन्तव्यम्_Resume = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    prvSwitchThread( x_Thread_गन्तव्यम्_Resume, x_Thread_गन्तव्यम्_Suspend );
}
/*-----------------------------------------------------------*/

शून्य v_पत्तनम्_Yield( शून्य )
{
    आधारः_Type_t पूर्व_intr_level = x_पत्तनम्_समुच्चयः_व्यवधानम्_आवरणम्();

    v_पत्तनम्_Yield_From_ISR();

    v_पत्तनम्_Clear_व्यवधानम्_आवरणम्( पूर्व_intr_level );
}
/*-----------------------------------------------------------*/

/* In SMP code, the disable/enable interrupt macros are calling the set/get interrupt mask functions below.
   Hence, we need to call vPortDisableInterrupts() and vPortEnableInterrupts(), otherwise interrupts
   are never disabled/enabled. */

आधारः_Type_t x_पत्तनम्_समुच्चयः_व्यवधानम्_आवरणम्( शून्य )
{
    यदि (ux_व्यवधानम्_Level == 0 && uxCriticalNestingIDF == 0) {
        v_पत्तनम्_Disable_Interrupts();
    }
    आधारः_Type_t पूर्व_intr_level = ux_व्यवधानम्_Level;
    ux_व्यवधानम्_Level++;
    प्रत्यागम पूर्व_intr_level;
}
/*-----------------------------------------------------------*/

शून्य v_पत्तनम्_Clear_व्यवधानम्_आवरणम्( आधारः_Type_t x_आवरणम् )
{
    // Only re-enable interrupts if xMask is 0
    ux_व्यवधानम्_Level = x_आवरणम्;
    यदि (ux_व्यवधानम्_Level == 0 && uxCriticalNestingIDF == 0) {
        v_पत्तनम्_Enable_Interrupts();
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
शून्य prv_सज्जता_Timer_व्यवधानम्( शून्य )
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
    itimer.it_interval.tv_usec = पत्तनम्_TICK_दरः_MICROSECONDS;

    /* Set the current count-down. */
    itimer.it_value.tv_sec = 0;
    itimer.it_value.tv_usec = पत्तनम्_TICK_दरः_MICROSECONDS;

    /* Set-up the timer interrupt. */
    iRet = setitimer( ITIMER_REAL, &itimer, रिक्त );
    यदि ( iRet )
    {
        prvFatalError( "setitimer", errno );
    }

    prvStartTimeNs = prvGetTimeNs();
}
/*-----------------------------------------------------------*/

स्थिर शून्य v_पत्तनम्_System_Tick_Handler( पूर्णाङ्क sig )
{
    Thread_t *px_Thread_गन्तव्यम्_Suspend;
    Thread_t *px_Thread_गन्तव्यम्_Resume;
    आधारः_Type_t xSwitchRequired;
    /* uint64_t xExpectedTicks; */

    // Handling a timer signal, so we are currently in an interrupt.
    ux_व्यवधानम्_Nesting++;

#यदि ( configUSE_PREEMPTION == 1 )
    px_Thread_गन्तव्यम्_Suspend = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );
#यदि_अन्तः

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

        px_Thread_गन्तव्यम्_Resume = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

        prvSwitchThread(px_Thread_गन्तव्यम्_Resume, px_Thread_गन्तव्यम्_Suspend);
    }
#अन्यथा
    (शून्य)xSwitchRequired;
#यदि_अन्तः

    // Returning from the timer signal handler, so we are exiting the interrupt.
    ux_व्यवधानम्_Nesting--;
}
/*-----------------------------------------------------------*/

शून्य v_पत्तनम्_Thread_Dying( शून्य *px_Task_गन्तव्यम्_Delete, चञ्चल आधारः_Type_t *pxPendYield )
{
    Thread_t *pxThread = prvGetThreadFromTask( px_Task_गन्तव्यम्_Delete );

    pxThread->xDying = pdTRUE;
}

शून्य v_पत्तनम्_Cancel_Thread( शून्य *px_Task_गन्तव्यम्_Delete )
{
    Thread_t *px_Thread_गन्तव्यम्_Cancel = prvGetThreadFromTask( px_Task_गन्तव्यम्_Delete );

    /*
     * The thread has already been suspended so it can be safely cancelled.
     */
    pthread_cancel( px_Thread_गन्तव्यम्_Cancel->pthread );
    pthread_join( px_Thread_गन्तव्यम्_Cancel->pthread, रिक्त );
    event_delete( px_Thread_गन्तव्यम्_Cancel->ev );
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
    ux_व्यवधानम्_Nesting = 0;
    ux_व्यवधानम्_Level = 0;
    v_पत्तनम्_Enable_Interrupts();

    /* Call the task's entry point. */
    pxThread->px_संहिता( pxThread->pvParams );

    /* A function that implements a task must not exit or attempt to return to
    * its caller as there is nothing to return to. If a task wants to exit it
    * should instead call vTaskDelete( NULL ). Artificially force an assert()
    * to be triggered if configASSERT() is defined, so application writers can
        * catch the error. */
    configASSERT( pdFALSE );

    प्रत्यागम रिक्त;
}
/*-----------------------------------------------------------*/

स्थिर शून्य prvSwitchThread( Thread_t *px_Thread_गन्तव्यम्_Resume,
                             Thread_t *px_Thread_गन्तव्यम्_Suspend )
{
    आधारः_Type_t uxSavedCriticalNestingIDF;
    आधारः_Type_t ux_Saved_व्यवधानम्_Nesting;
    आधारः_Type_t ux_Saved_व्यवधानम्_Level;

    यदि ( px_Thread_गन्तव्यम्_Suspend != px_Thread_गन्तव्यम्_Resume )
    {
        /* It is possible for prvSwitchThread() to be called...
         * - while inside an ISR (i.e., via vPortSystemTickHandler() or vPortYieldFromISR())
         * - while interrupts are disabled or in a critical section (i.e., via vPortYield())
         *
         * So we need to save the various count variables as part of the thread's context.
         * They are restored when the pthread switches back. */
        uxSavedCriticalNestingIDF = uxCriticalNestingIDF;
        ux_Saved_व्यवधानम्_Nesting = ux_व्यवधानम्_Nesting;
        ux_Saved_व्यवधानम्_Level = ux_व्यवधानम्_Level;

        prvResumeThread( px_Thread_गन्तव्यम्_Resume );
        यदि ( px_Thread_गन्तव्यम्_Suspend->xDying )
        {
            pthread_exit( रिक्त );
        }
        prvSuspendSelf( px_Thread_गन्तव्यम्_Suspend );

        uxCriticalNestingIDF = uxSavedCriticalNestingIDF;
        ux_व्यवधानम्_Nesting = ux_Saved_व्यवधानम्_Nesting;
        ux_व्यवधानम्_Level = ux_Saved_व्यवधानम्_Level;
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

स्थिर शून्य prv_सज्जता_सङ्केताः_And_Scheduler_Policy( शून्य )
{
    संरचना sigaction sigresume, sigtick;
    पूर्णाङ्क iRet;

    hMainThread = pthread_self();

    /* Initialise common signal masks. */
    sigfillset( &x_All_सङ्केताः );
    /* Don't block SIGINT so this can be used to break into GDB while
     * in a critical section. */
    sigdelset( &x_All_सङ्केताः, SIGINT );

    /*
     * Block all signals in this thread so all new threads
     * inherits this mask.
     *
     * When a thread is resumed for the first time, all signals
     * will be unblocked.
     */
    (शून्य)pthread_sigmask( SIG_SETMASK, &x_All_सङ्केताः,
                           &x_Scheduler_Original_Signal_आवरणम् );

    /* SIG_RESUME is only used with sigwait() so doesn't need a
       handler. */
    sigresume.sa_flags = 0;
    sigresume.sa_handler = SIG_IGN;
    sigfillset( &sigresume.sa_mask );

    sigtick.sa_flags = 0;
    sigtick.sa_handler = v_पत्तनम्_System_Tick_Handler;
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

अचिह्नित दीर्घ ul_पत्तनम्_Get_सततम्_Time( शून्य )
{
    संरचना tms xTimes;

    times( &xTimes );

    प्रत्यागम ( अचिह्नित दीर्घ ) xTimes.tms_utime;
}
/*-----------------------------------------------------------*/
bool पत्तनम्_VALID_LIST_MEM(ध्रुव शून्य *ptr)
{
    प्रत्यागम true;
}

bool पत्तनम्_VALID_TCB_MEM(ध्रुव शून्य *ptr)
{
    प्रत्यागम true;
}

bool पत्तनम्_VALID_राशिः_MEM(ध्रुव शून्य *ptr)
{
    प्रत्यागम true;
}
/*-----------------------------------------------------------*/

पत्तनम्_MUX_TYPE पत्तनम्_x_Task_तालकम् = पत्तनम्_MUX_INITIALIZER_UNLOCKED;
पत्तनम्_MUX_TYPE पत्तनम्_x_ISRLock = पत्तनम्_MUX_INITIALIZER_UNLOCKED;

स्थिर ध्रुव वर्ण *चिह्नकम् = "port";

आधारः_Type_t x_पत्तनम्_Check_If_अन्तर्_ISR(शून्य)
{
    प्रत्यागम (ux_व्यवधानम्_Nesting == 0) ? pdFALSE : pdTRUE;
}

शून्य app_main(शून्य);

स्थिर शून्य main_task(शून्य* तर्काः)
{
    app_main();
    vTaskDelete(रिक्त);
}

पूर्णाङ्क मुख्यम्(पूर्णाङ्क argc, ध्रुव वर्ण **argv)
{
    // This makes sure that stdio is flushed after each '\n' so that idf.py monitor
    // reads the program output on time.
    setvbuf(stdout, रिक्त, _IOLBF, 0);

    usleep(1000);

    आधारः_Type_t res;

#यदि ( config_अङ्कः_CORES > 1 )
    res = x_Task_Create_Affinity_समुच्चयः(&main_task, "main",
                                 ESP_TASK_MAIN_राशिः, रिक्त,
                                 ESP_TASK_MAIN_PRIO, ESP_TASK_MAIN_CORE, रिक्त);
#अन्यथा
    res = xTaskCreate(&main_task, "main",
                      ESP_TASK_MAIN_राशिः, रिक्त,
                      ESP_TASK_MAIN_PRIO, रिक्त);
#यदि_अन्तः

    प्रतिज्ञा(res == pdTRUE);
    (शून्य)res;

    ESP_LOGI(चिह्नकम्, "Starting SMP scheduler.");
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
#यदि_अन्तः

#यदि ( config_SUPPORT_STATIC_आवण्टनम् == 1 )
/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
 * implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
 * used by the Idle task. */
शून्य v_Application_Get_Idle_Task_स्मृतिः( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                    राशिः_Type_t ** ppx_Idle_Task_राशिः_बफर,
                                    uint32_t * pul_Idle_Task_राशिः_आकार )
{
/* If the buffers to be provided to the Idle task are declared inside this
 * function then they must be declared static - otherwise they will be allocated on
 * the stack and so not exists after this function exits. */
    स्थिर StaticTask_t xIdleTaskTCB;
    स्थिर राशिः_Type_t ux_Idle_Task_राशिः[ config_अल्पिष्ठम्_राशिः_आकार ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
     * state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppx_Idle_Task_राशिः_बफर = ux_Idle_Task_राशिः;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in bytes. */
    *pul_Idle_Task_राशिः_आकार = config_अल्पिष्ठम्_राशिः_आकार;
}
#यदि_अन्तः // configSUPPORT_STATIC_ALLOCATION == 1
/*-----------------------------------------------------------*/

#यदि ( (config_SUPPORT_STATIC_आवण्टनम् == 1) && (configUSE_TIMERS == 1))

/* When configSUPPORT_STATIC_ALLOCATION is set to 1 the application writer can
 * use a callback function to optionally provide the memory required by the idle
 * and timer tasks.  This is the stack that will be used by the timer task.  It is
 * declared here, as a global, so it can be checked by a test that is implemented
 * in a different file. */
राशिः_Type_t ux_Timer_Task_राशिः[ config_TIMER_TASK_राशिः_गभीरता ];


/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
 * application must provide an implementation of vApplicationGetTimerTaskMemory()
 * to provide the memory that is used by the Timer service task. */
शून्य v_Application_Get_Timer_Task_स्मृतिः( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                     राशिः_Type_t ** ppx_Timer_Task_राशिः_बफर,
                                     uint32_t * pul_Timer_Task_राशिः_आकार )
{
/* If the buffers to be provided to the Timer task are declared inside this
 * function then they must be declared static - otherwise they will be allocated on
 * the stack and so not exists after this function exits. */
    स्थिर StaticTask_t xTimerTaskTCB;

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
     * task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppx_Timer_Task_राशिः_बफर = ux_Timer_Task_राशिः;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in bytes. */
    *pul_Timer_Task_राशिः_आकार = config_TIMER_TASK_राशिः_गभीरता;
}
#यदि_अन्तः // configSUPPORT_STATIC_ALLOCATION == 1 && (configUSE_TIMERS == 1)

शून्य v_पत्तनम्_Take_तालकम्( पत्तनम्_MUX_TYPE *तालकम् )
{
    spinlock_acquire( तालकम्, पत्तनम्_MUX_NO_TIMEOUT);
}

शून्य v_पत्तनम्_Release_तालकम्( पत्तनम्_MUX_TYPE *तालकम् )
{
    spinlock_release( तालकम् );
}

#परिभाषय FREERTOS_SMP_MALLOC_CAPS    (MALLOC_CAP_आन्तरिकम्|MALLOC_CAP_8BIT)

शून्य *pv_पत्तनम्_Malloc( size_t x_आकार )
{
    प्रत्यागम heap_caps_malloc(x_आकार, FREERTOS_SMP_MALLOC_CAPS);
}

शून्य v_पत्तनम्_Free( शून्य *pv )
{
    heap_caps_free(pv);
}

शून्य __attribute__((weak)) v_Application_राशिः_अतिप्रवाहः_Hook(TaskHandle_t xTask, वर्ण *pcTaskName)
{
#परिभाषय ERR_STR1 "***ERROR*** A stack overflow in task "
#परिभाषय ERR_STR2 " has been detected."
    ध्रुव वर्ण *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    वर्ण buf[परिमाण(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + परिमाण(ERR_STR2) + 1 /* null char */] = {0};

    वर्ण *dest = buf;
    प्रति (पूर्णाङ्क i = 0; i < परिमाण(str) / परिमाण(str[0]); i++) {
        dest = वाक्यं_योजय(dest, str[i]);
    }
	प्रारूप_लिख("%s\n", buf);
    abort();
}

// ------- Thread Local Storage Pointers Deletion Callbacks -------

#यदि ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
शून्य v_पत्तनम्_TLSPointers_Del_Cb( शून्य *pxTCB )
{
    /* Typecast pxTCB to StaticTask_t type to access TCB struct members.
     * pvDummy15 corresponds to pvThreadLocalStoragePointers member of the TCB.
     */
    StaticTask_t *tcb = ( StaticTask_t * )pxTCB;

    /* The TLSP deletion callbacks are stored at an offset of (configNUM_THREAD_LOCAL_STORAGE_POINTERS/2) */
    TlsDeleteCallbackFunction_t *pv_Thread_Local_Storage_सूचकाः_Del_Callback = ( TlsDeleteCallbackFunction_t * )( &( tcb->pvDummy15[ ( config_अङ्कः_THREAD_LOCAL_STORAGE_सूचकाः / 2 ) ] ) );

    /* We need to iterate over half the depth of the pvThreadLocalStoragePointers area
     * to access all TLS pointers and their respective TLS deletion callbacks.
     */
    प्रति ( पूर्णाङ्क x = 0; x < ( config_अङ्कः_THREAD_LOCAL_STORAGE_सूचकाः / 2 ); x++ ) {
        यदि ( pv_Thread_Local_Storage_सूचकाः_Del_Callback[ x ] != रिक्त ) {  //If del cb is set
            // We skip the check if the callback is executable as that is difficult to determine for different
            // platforms (compare xtensa and riscv code).
            pv_Thread_Local_Storage_सूचकाः_Del_Callback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#यदि_अन्तः // CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS

शून्य v_पत्तनम्_Clean_ऊर्ध्वम्_TCB ( शून्य *pxTCB )
{
    #यदि ( CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK )
        /* Call the user defined task pre-deletion hook */
        बाह्य शून्य vTaskPreDeletionHook( शून्य * pxTCB );
        vTaskPreDeletionHook( pxTCB );
    #यदि_अन्तः /* CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK */

#यदि ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
    /* Call TLS pointers deletion callbacks */
    v_पत्तनम्_TLSPointers_Del_Cb( pxTCB );
#यदि_अन्तः /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

    v_पत्तनम्_Cancel_Thread(pxTCB);
}
