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

#ضمن <errno.h>
#ضمن <pthread.h>
#ضمن <signal.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <string.h>
#ضمن <sys/time.h>
#ضمن <sys/times.h>
#ضمن <time.h>
#ضمن <unistd.h>
#ضمن <assert.h>

/* Scheduler includes. */
#ضمن "esp_heap_caps.h"
#ضمن "FreeRTOS.h"
#ضمن "task.h"
#ضمن "esp_task.h"
#ضمن "timers.h"
#ضمن "utils/wait_for_event.h"
#ضمن "esp_log.h"
/*-----------------------------------------------------------*/

#تعریف_کړه SIG_RESUME SIGUSR1

تعريفنوع بنية THREAD
{
    pthread_t pthread;
    TaskFunction_t px_کوډ;
    فراغ *pvParams;
    بنسټ_Type_t xDying;
    بنية event *ev;
} Thread_t;

/*
 * The additional per-thread data is stored at the beginning of the
 * task's stack.
 */
ساكن مضمن Thread_t *prvGetThreadFromTask(TaskHandle_t xTask)
{
    ډېری_Type_t *px_سر_د_ډېری = *(ډېری_Type_t **)xTask;

    أرجع (Thread_t *)(px_سر_د_ډېری + 1);
}

/*-----------------------------------------------------------*/

ساكن pthread_once_t h_Sig_تیاری_Thread = PTHREAD_ONCE_INIT;
ساكن sigset_t x_All_سیګنالونه;
ساكن sigset_t x_Scheduler_Original_Signal_ماسک;
ساكن pthread_t hMainThread = ( pthread_t )عدم;

// These are saved as part of a thread's state in prvSwitchThread()
ساكن متطاير بنسټ_Type_t uxCriticalNestingIDF = 0;    /* Track nesting calls for IDF style critical sections. FreeRTOS critical section nesting is maintained in the TCB. */
ساكن متطاير UBaseType_t ux_مداخله_Nesting = 0;     /* Tracks if we are currently in an interrupt. */
ساكن متطاير بنسټ_Type_t ux_مداخله_Level = 0;        /* Tracks the current level (i.e., interrupt mask) */
/*-----------------------------------------------------------*/

ساكن بنسټ_Type_t x_Scheduler_پای_ټکی = pdFALSE;
/*-----------------------------------------------------------*/

ساكن فراغ prv_تیاری_سیګنالونه_And_Scheduler_Policy( فراغ );
ساكن فراغ prv_تیاری_Timer_مداخله( فراغ );
ساكن فراغ *prv_Wait_For_پیل( فراغ * pvParams );
ساكن فراغ prvSwitchThread( Thread_t * x_Thread_منزل_Resume,
                             Thread_t *x_Thread_منزل_Suspend );
ساكن فراغ prvSuspendSelf( Thread_t * thread);
ساكن فراغ prvResumeThread( Thread_t * xThreadId );
ساكن فراغ v_پورټ_System_Tick_Handler( صحيح sig );
ساكن فراغ v_پورټ_پیل_لومړی_Task( فراغ );
/*-----------------------------------------------------------*/

ساكن فراغ prvFatalError( ثابت حرف *pcCall, صحيح iErrno )
{
    fprintf( stderr, "%s: %s\n", pcCall, strerror( iErrno ) );
    abort();
}

/*
 * See header file for description.
 */
ډېری_Type_t *px_پورټ_Initialise_ډېری( ډېری_Type_t *px_سر_د_ډېری,
                                    ډېری_Type_t *px_پای_ټکی_د_ډېری,
                                    TaskFunction_t px_کوډ,
                                    فراغ *pvParameters )
{
    Thread_t *thread;
    pthread_attr_t x_Thread_ځانګړتیاوې;
    size_t ul_ډېری_اندازه;
    صحيح iRet;

    (فراغ)pthread_once( &h_Sig_تیاری_Thread, prv_تیاری_سیګنالونه_And_Scheduler_Policy );

    /*
     * Store the additional thread data at the start of the stack.
     */
    thread = (Thread_t *)(px_سر_د_ډېری + 1) - 1;
    px_سر_د_ډېری = (ډېری_Type_t *)thread - 1;
    ul_ډېری_اندازه = (px_سر_د_ډېری + 1 - px_پای_ټکی_د_ډېری) * حجم(*px_سر_د_ډېری);

    thread->px_کوډ = px_کوډ;
    thread->pvParams = pvParameters;
    thread->xDying = pdFALSE;

    pthread_attr_init( &x_Thread_ځانګړتیاوې );
    pthread_attr_setstack( &x_Thread_ځانګړتیاوې, px_پای_ټکی_د_ډېری, ul_ډېری_اندازه );

    thread->ev = event_create();

    بنسټ_Type_t prev_intr_level = x_پورټ_سېټ_مداخله_ماسک();

    iRet = pthread_create( &thread->pthread, &x_Thread_ځانګړتیاوې,
                           prv_Wait_For_پیل, thread );
    إذا ( iRet )
    {
        prvFatalError( "pthread_create", iRet );
    }

    v_پورټ_Clear_مداخله_ماسک( prev_intr_level );

    أرجع px_سر_د_ډېری;
}
/*-----------------------------------------------------------*/

فراغ v_پورټ_پیل_لومړی_Task( فراغ )
{
    Thread_t *px_لومړی_Thread = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    /* Start the first task. */
    prvResumeThread( px_لومړی_Thread );
}
/*-----------------------------------------------------------*/

/*
 * See header file for description.
 */
بنسټ_Type_t x_پورټ_پیل_Scheduler( فراغ )
{
    صحيح iSignal;
    sigset_t x_سیګنالونه;

    hMainThread = pthread_self();

    /* Start the timer that generates the tick ISR(SIGALRM).
       Interrupts are disabled here already. */
    prv_تیاری_Timer_مداخله();

    /* Start the first task. */
    v_پورټ_پیل_لومړی_Task();

    /* Wait until signaled by vPortEndScheduler(). */
    sigemptyset( &x_سیګنالونه );
    sigaddset( &x_سیګنالونه, SIG_RESUME );

    طالما ( !x_Scheduler_پای_ټکی )
    {
        sigwait( &x_سیګنالونه, &iSignal );
    }

    /* Cancel the Idle task and free its resources */
#إذا ( INCLUDE_xTaskGetIdleTaskHandle == 1 )
    v_پورټ_Cancel_Thread( xTaskGetIdleTaskHandle() );
#د_که_پای

#إذا ( configUSE_TIMERS == 1 )
    /* Cancel the Timer task and free its resources */
    v_پورټ_Cancel_Thread( xTimerGetTimerDaemonTaskHandle() );
#د_که_پای /* configUSE_TIMERS */

    /* Restore original signal mask. */
    (فراغ)pthread_sigmask( SIG_SETMASK, &x_Scheduler_Original_Signal_ماسک,  عدم );

    أرجع 0;
}
/*-----------------------------------------------------------*/

فراغ v_پورټ_پای_ټکی_Scheduler( فراغ )
{
    بنية itimerval itimer;
    بنية sigaction sigtick;
    Thread_t *xCurrentThread;

    /* Stop the timer and ignore any pending SIGALRMs that would end
     * up running on the main thread when it is resumed. */
    itimer.it_value.tv_sec = 0;
    itimer.it_value.tv_usec = 0;

    itimer.it_interval.tv_sec = 0;
    itimer.it_interval.tv_usec = 0;
    (فراغ)setitimer( ITIMER_REAL, &itimer, عدم );

    sigtick.sa_flags = 0;
    sigtick.sa_handler = SIG_IGN;
    sigemptyset( &sigtick.sa_mask );
    sigaction( SIGALRM, &sigtick, عدم );

    /* Signal the scheduler to exit its loop. */
    x_Scheduler_پای_ټکی = pdTRUE;
    (فراغ)pthread_kill( hMainThread, SIG_RESUME );

    xCurrentThread = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );
    prvSuspendSelf(xCurrentThread);
}
/*-----------------------------------------------------------*/

ساكن فراغ v_پورټ_Disable_Interrupts( فراغ )
{
    pthread_sigmask( SIG_BLOCK, &x_All_سیګنالونه, عدم );
}
/*-----------------------------------------------------------*/

ساكن فراغ v_پورټ_Enable_Interrupts( فراغ )
{
    pthread_sigmask( SIG_UNBLOCK, &x_All_سیګنالونه, عدم );
}
/*-----------------------------------------------------------*/

فراغ v_پورټ_Enter_Critical_IDF( فراغ )
{
    إذا ( uxCriticalNestingIDF == 0 && ux_مداخله_Level == 0)
    {
        v_پورټ_Disable_Interrupts();
    }
    uxCriticalNestingIDF++;
}
/*-----------------------------------------------------------*/

فراغ v_پورټ_Exit_Critical_IDF( فراغ )
{
    uxCriticalNestingIDF--;

    /* If we have reached 0 then re-enable the interrupts. */
    إذا( uxCriticalNestingIDF == 0 && ux_مداخله_Level == 0)
    {
        v_پورټ_Enable_Interrupts();
    }
}
/*-----------------------------------------------------------*/

فراغ v_پورټ_Yield_From_ISR( فراغ )
{
    Thread_t *x_Thread_منزل_Suspend;
    Thread_t *x_Thread_منزل_Resume;

    x_Thread_منزل_Suspend = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    vTaskSwitchContext();

    x_Thread_منزل_Resume = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    prvSwitchThread( x_Thread_منزل_Resume, x_Thread_منزل_Suspend );
}
/*-----------------------------------------------------------*/

فراغ v_پورټ_Yield( فراغ )
{
    بنسټ_Type_t prev_intr_level = x_پورټ_سېټ_مداخله_ماسک();

    v_پورټ_Yield_From_ISR();

    v_پورټ_Clear_مداخله_ماسک( prev_intr_level );
}
/*-----------------------------------------------------------*/

/* In SMP code, the disable/enable interrupt macros are calling the set/get interrupt mask functions below.
   Hence, we need to call vPortDisableInterrupts() and vPortEnableInterrupts(), otherwise interrupts
   are never disabled/enabled. */

بنسټ_Type_t x_پورټ_سېټ_مداخله_ماسک( فراغ )
{
    إذا (ux_مداخله_Level == 0 && uxCriticalNestingIDF == 0) {
        v_پورټ_Disable_Interrupts();
    }
    بنسټ_Type_t prev_intr_level = ux_مداخله_Level;
    ux_مداخله_Level++;
    أرجع prev_intr_level;
}
/*-----------------------------------------------------------*/

فراغ v_پورټ_Clear_مداخله_ماسک( بنسټ_Type_t x_ماسک )
{
    // Only re-enable interrupts if xMask is 0
    ux_مداخله_Level = x_ماسک;
    إذا (ux_مداخله_Level == 0 && uxCriticalNestingIDF == 0) {
        v_پورټ_Enable_Interrupts();
    }
}
/*-----------------------------------------------------------*/

ساكن uint64_t prv_Get_وخت_Ns(فراغ)
{
    بنية timespec t;

    clock_gettime(CLOCK_MONOTONIC, &t);

    أرجع t.tv_sec * 1000000000ull + t.tv_nsec;
}

ساكن uint64_t prv_پیل_وخت_Ns;
/* commented as part of the code below in vPortSystemTickHandler,
 * to adjust timing according to full demo requirements */
/* static uint64_t prvTickCount; */

/*
 * Setup the systick timer to generate the tick interrupts at the required
 * frequency.
 */
فراغ prv_تیاری_Timer_مداخله( فراغ )
{
    بنية itimerval itimer;
    صحيح iRet;

    /* Initialise the structure with the current timer information. */
    iRet = getitimer( ITIMER_REAL, &itimer );
    إذا ( iRet )
    {
        prvFatalError( "getitimer", errno );
    }

    /* Set the interval between timer events. */
    itimer.it_interval.tv_sec = 0;
    itimer.it_interval.tv_usec = پورټ_TICK_شرح_MICROSECONDS;

    /* Set the current count-down. */
    itimer.it_value.tv_sec = 0;
    itimer.it_value.tv_usec = پورټ_TICK_شرح_MICROSECONDS;

    /* Set-up the timer interrupt. */
    iRet = setitimer( ITIMER_REAL, &itimer, عدم );
    إذا ( iRet )
    {
        prvFatalError( "setitimer", errno );
    }

    prv_پیل_وخت_Ns = prv_Get_وخت_Ns();
}
/*-----------------------------------------------------------*/

ساكن فراغ v_پورټ_System_Tick_Handler( صحيح sig )
{
    Thread_t *px_Thread_منزل_Suspend;
    Thread_t *px_Thread_منزل_Resume;
    بنسټ_Type_t xSwitchRequired;
    /* uint64_t xExpectedTicks; */

    // Handling a timer signal, so we are currently in an interrupt.
    ux_مداخله_Nesting++;

#إذا ( configUSE_PREEMPTION == 1 )
    px_Thread_منزل_Suspend = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );
#د_که_پای

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

#إذا ( configUSE_PREEMPTION == 1 )
    إذا (xSwitchRequired == pdTRUE) {
        /* Select Next Task. */
        vTaskSwitchContext();

        px_Thread_منزل_Resume = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

        prvSwitchThread(px_Thread_منزل_Resume, px_Thread_منزل_Suspend);
    }
#وإلا
    (فراغ)xSwitchRequired;
#د_که_پای

    // Returning from the timer signal handler, so we are exiting the interrupt.
    ux_مداخله_Nesting--;
}
/*-----------------------------------------------------------*/

فراغ v_پورټ_Thread_Dying( فراغ *px_Task_منزل_Delete, متطاير بنسټ_Type_t *pxPendYield )
{
    Thread_t *pxThread = prvGetThreadFromTask( px_Task_منزل_Delete );

    pxThread->xDying = pdTRUE;
}

فراغ v_پورټ_Cancel_Thread( فراغ *px_Task_منزل_Delete )
{
    Thread_t *px_Thread_منزل_Cancel = prvGetThreadFromTask( px_Task_منزل_Delete );

    /*
     * The thread has already been suspended so it can be safely cancelled.
     */
    pthread_cancel( px_Thread_منزل_Cancel->pthread );
    pthread_join( px_Thread_منزل_Cancel->pthread, عدم );
    event_delete( px_Thread_منزل_Cancel->ev );
}
/*-----------------------------------------------------------*/

ساكن فراغ *prv_Wait_For_پیل( فراغ * pvParams )
{
    Thread_t *pxThread = pvParams;

    prvSuspendSelf(pxThread);

    /* Resumed for the first time, thus this thread didn't previously call
     * prvSwitchThread(). So we need to initialise the state variables for this
     * thread. */
    uxCriticalNestingIDF = 0;
    ux_مداخله_Nesting = 0;
    ux_مداخله_Level = 0;
    v_پورټ_Enable_Interrupts();

    /* Call the task's entry point. */
    pxThread->px_کوډ( pxThread->pvParams );

    /* A function that implements a task must not exit or attempt to return to
    * its caller as there is nothing to return to. If a task wants to exit it
    * should instead call vTaskDelete( NULL ). Artificially force an assert()
    * to be triggered if configASSERT() is defined, so application writers can
        * catch the error. */
    configASSERT( pdFALSE );

    أرجع عدم;
}
/*-----------------------------------------------------------*/

ساكن فراغ prvSwitchThread( Thread_t *px_Thread_منزل_Resume,
                             Thread_t *px_Thread_منزل_Suspend )
{
    بنسټ_Type_t uxSavedCriticalNestingIDF;
    بنسټ_Type_t ux_Saved_مداخله_Nesting;
    بنسټ_Type_t ux_Saved_مداخله_Level;

    إذا ( px_Thread_منزل_Suspend != px_Thread_منزل_Resume )
    {
        /* It is possible for prvSwitchThread() to be called...
         * - while inside an ISR (i.e., via vPortSystemTickHandler() or vPortYieldFromISR())
         * - while interrupts are disabled or in a critical section (i.e., via vPortYield())
         *
         * So we need to save the various count variables as part of the thread's context.
         * They are restored when the pthread switches back. */
        uxSavedCriticalNestingIDF = uxCriticalNestingIDF;
        ux_Saved_مداخله_Nesting = ux_مداخله_Nesting;
        ux_Saved_مداخله_Level = ux_مداخله_Level;

        prvResumeThread( px_Thread_منزل_Resume );
        إذا ( px_Thread_منزل_Suspend->xDying )
        {
            pthread_exit( عدم );
        }
        prvSuspendSelf( px_Thread_منزل_Suspend );

        uxCriticalNestingIDF = uxSavedCriticalNestingIDF;
        ux_مداخله_Nesting = ux_Saved_مداخله_Nesting;
        ux_مداخله_Level = ux_Saved_مداخله_Level;
    }
}
/*-----------------------------------------------------------*/

ساكن فراغ prvSuspendSelf( Thread_t *thread )
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

ساكن فراغ prvResumeThread( Thread_t *xThreadId )
{
    إذا ( pthread_self() != xThreadId->pthread )
    {
        event_signal(xThreadId->ev);
    }
}
/*-----------------------------------------------------------*/

ساكن فراغ prv_تیاری_سیګنالونه_And_Scheduler_Policy( فراغ )
{
    بنية sigaction sigresume, sigtick;
    صحيح iRet;

    hMainThread = pthread_self();

    /* Initialise common signal masks. */
    sigfillset( &x_All_سیګنالونه );
    /* Don't block SIGINT so this can be used to break into GDB while
     * in a critical section. */
    sigdelset( &x_All_سیګنالونه, SIGINT );

    /*
     * Block all signals in this thread so all new threads
     * inherits this mask.
     *
     * When a thread is resumed for the first time, all signals
     * will be unblocked.
     */
    (فراغ)pthread_sigmask( SIG_SETMASK, &x_All_سیګنالونه,
                           &x_Scheduler_Original_Signal_ماسک );

    /* SIG_RESUME is only used with sigwait() so doesn't need a
       handler. */
    sigresume.sa_flags = 0;
    sigresume.sa_handler = SIG_IGN;
    sigfillset( &sigresume.sa_mask );

    sigtick.sa_flags = 0;
    sigtick.sa_handler = v_پورټ_System_Tick_Handler;
    sigfillset( &sigtick.sa_mask );

    iRet = sigaction( SIG_RESUME, &sigresume, عدم );
    إذا ( iRet )
    {
        prvFatalError( "sigaction", errno );
    }

    iRet = sigaction( SIGALRM, &sigtick, عدم );
    إذا ( iRet )
    {
        prvFatalError( "sigaction", errno );
    }
}
/*-----------------------------------------------------------*/

بینښه اوږد ul_پورټ_Get_پرلپسې_وخت( فراغ )
{
    بنية tms xTimes;

    times( &xTimes );

    أرجع ( بینښه اوږد ) xTimes.tms_utime;
}
/*-----------------------------------------------------------*/
bool پورټ_VALID_LIST_MEM(ثابت فراغ *ptr)
{
    أرجع true;
}

bool پورټ_VALID_TCB_MEM(ثابت فراغ *ptr)
{
    أرجع true;
}

bool پورټ_VALID_ډېری_MEM(ثابت فراغ *ptr)
{
    أرجع true;
}
/*-----------------------------------------------------------*/

پورټ_MUX_TYPE پورټ_x_Task_قلف = پورټ_MUX_INITIALIZER_UNLOCKED;
پورټ_MUX_TYPE پورټ_x_ISRLock = پورټ_MUX_INITIALIZER_UNLOCKED;

ساكن ثابت حرف *نښان = "port";

بنسټ_Type_t x_پورټ_Check_If_دننه_ISR(فراغ)
{
    أرجع (ux_مداخله_Nesting == 0) ? pdFALSE : pdTRUE;
}

فراغ app_main(فراغ);

ساكن فراغ main_task(فراغ* دلیلونه)
{
    app_main();
    vTaskDelete(عدم);
}

صحيح اصلي(صحيح argc, ثابت حرف **argv)
{
    // This makes sure that stdio is flushed after each '\n' so that idf.py monitor
    // reads the program output on time.
    setvbuf(stdout, عدم, _IOLBF, 0);

    usleep(1000);

    بنسټ_Type_t res;

#إذا ( config_عدد_CORES > 1 )
    res = x_Task_Create_Affinity_سېټ(&main_task, "main",
                                 ESP_TASK_MAIN_ډېری, عدم,
                                 ESP_TASK_MAIN_PRIO, ESP_TASK_MAIN_CORE, عدم);
#وإلا
    res = xTaskCreate(&main_task, "main",
                      ESP_TASK_MAIN_ډېری, عدم,
                      ESP_TASK_MAIN_PRIO, عدم);
#د_که_پای

    أكد(res == pdTRUE);
    (فراغ)res;

    ESP_LOGI(نښان, "Starting SMP scheduler.");
    v_Task_پیل_Scheduler();

    // This line should never be reached
    أكد(false);
}

فراغ esp_vApplicationIdleHook(فراغ)
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

فراغ esp_vApplicationTickHook( فراغ ) { }

#إذا  (  configUSE_TICK_HOOK > 0 )
فراغ vApplicationTickHook( فراغ )
{
    esp_vApplicationTickHook();
}
#د_که_پای

#إذا ( config_SUPPORT_STATIC_ویش == 1 )
/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
 * implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
 * used by the Idle task. */
فراغ v_Application_Get_Idle_Task_حافظه( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                    ډېری_Type_t ** ppx_Idle_Task_ډېری_بفر,
                                    uint32_t * pul_Idle_Task_ډېری_اندازه )
{
/* If the buffers to be provided to the Idle task are declared inside this
 * function then they must be declared static - otherwise they will be allocated on
 * the stack and so not exists after this function exits. */
    ساكن StaticTask_t xIdleTaskTCB;
    ساكن ډېری_Type_t ux_Idle_Task_ډېری[ config_اصغري_ډېری_اندازه ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
     * state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppx_Idle_Task_ډېری_بفر = ux_Idle_Task_ډېری;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in bytes. */
    *pul_Idle_Task_ډېری_اندازه = config_اصغري_ډېری_اندازه;
}
#د_که_پای // configSUPPORT_STATIC_ALLOCATION == 1
/*-----------------------------------------------------------*/

#إذا ( (config_SUPPORT_STATIC_ویش == 1) && (configUSE_TIMERS == 1))

/* When configSUPPORT_STATIC_ALLOCATION is set to 1 the application writer can
 * use a callback function to optionally provide the memory required by the idle
 * and timer tasks.  This is the stack that will be used by the timer task.  It is
 * declared here, as a global, so it can be checked by a test that is implemented
 * in a different file. */
ډېری_Type_t ux_Timer_Task_ډېری[ config_TIMER_TASK_ډېری_ژوروالی ];


/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
 * application must provide an implementation of vApplicationGetTimerTaskMemory()
 * to provide the memory that is used by the Timer service task. */
فراغ v_Application_Get_Timer_Task_حافظه( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                     ډېری_Type_t ** ppx_Timer_Task_ډېری_بفر,
                                     uint32_t * pul_Timer_Task_ډېری_اندازه )
{
/* If the buffers to be provided to the Timer task are declared inside this
 * function then they must be declared static - otherwise they will be allocated on
 * the stack and so not exists after this function exits. */
    ساكن StaticTask_t xTimerTaskTCB;

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
     * task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppx_Timer_Task_ډېری_بفر = ux_Timer_Task_ډېری;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in bytes. */
    *pul_Timer_Task_ډېری_اندازه = config_TIMER_TASK_ډېری_ژوروالی;
}
#د_که_پای // configSUPPORT_STATIC_ALLOCATION == 1 && (configUSE_TIMERS == 1)

فراغ v_پورټ_Take_قلف( پورټ_MUX_TYPE *قلف )
{
    spinlock_acquire( قلف, پورټ_MUX_NO_TIMEOUT);
}

فراغ v_پورټ_Release_قلف( پورټ_MUX_TYPE *قلف )
{
    spinlock_release( قلف );
}

#تعریف_کړه FREERTOS_SMP_MALLOC_CAPS    (MALLOC_CAP_دننی|MALLOC_CAP_8_بټ)

فراغ *pv_پورټ_Malloc( size_t x_اندازه )
{
    أرجع ډېرۍ_caps_malloc(x_اندازه, FREERTOS_SMP_MALLOC_CAPS);
}

فراغ v_پورټ_Free( فراغ *pv )
{
    ډېرۍ_caps_free(pv);
}

فراغ __attribute__((weak)) v_Application_ډېری_ډکېدنه_Hook(TaskHandle_t xTask, حرف *pcTaskName)
{
#تعریف_کړه ERR_STR1 "***ERROR*** A stack overflow in task "
#تعریف_کړه ERR_STR2 " has been detected."
    ثابت حرف *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    حرف buf[حجم(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + حجم(ERR_STR2) + 1 /* null char */] = {0};

    حرف *dest = buf;
    لكل (صحيح i = 0; i < حجم(str) / حجم(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
	چاپ_کړه("%s\n", buf);
    abort();
}

// ------- Thread Local Storage Pointers Deletion Callbacks -------

#إذا ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
فراغ v_پورټ_TLSPointers_Del_Cb( فراغ *pxTCB )
{
    /* Typecast pxTCB to StaticTask_t type to access TCB struct members.
     * pvDummy15 corresponds to pvThreadLocalStoragePointers member of the TCB.
     */
    StaticTask_t *tcb = ( StaticTask_t * )pxTCB;

    /* The TLSP deletion callbacks are stored at an offset of (configNUM_THREAD_LOCAL_STORAGE_POINTERS/2) */
    TlsDeleteCallbackFunction_t *pv_Thread_Local_Storage_اشارې_Del_Callback = ( TlsDeleteCallbackFunction_t * )( &( tcb->pvDummy15[ ( config_عدد_THREAD_LOCAL_STORAGE_اشارې / 2 ) ] ) );

    /* We need to iterate over half the depth of the pvThreadLocalStoragePointers area
     * to access all TLS pointers and their respective TLS deletion callbacks.
     */
    لكل ( صحيح x = 0; x < ( config_عدد_THREAD_LOCAL_STORAGE_اشارې / 2 ); x++ ) {
        إذا ( pv_Thread_Local_Storage_اشارې_Del_Callback[ x ] != عدم ) {  //If del cb is set
            // We skip the check if the callback is executable as that is difficult to determine for different
            // platforms (compare xtensa and riscv code).
            pv_Thread_Local_Storage_اشارې_Del_Callback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#د_که_پای // CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS

فراغ v_پورټ_Clean_پورته_TCB ( فراغ *pxTCB )
{
    #إذا ( CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK )
        /* Call the user defined task pre-deletion hook */
        خارجي فراغ vTaskPreDeletionHook( فراغ * pxTCB );
        vTaskPreDeletionHook( pxTCB );
    #د_که_پای /* CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK */

#إذا ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
    /* Call TLS pointers deletion callbacks */
    v_پورټ_TLSPointers_Del_Cb( pxTCB );
#د_که_پای /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

    v_پورټ_Cancel_Thread(pxTCB);
}
