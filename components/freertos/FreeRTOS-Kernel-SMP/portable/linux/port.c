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

#অন্তর্ভাৱ <errno.h>
#অন্তর্ভাৱ <pthread.h>
#অন্তর্ভাৱ <signal.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <stdlib.h>
#অন্তর্ভাৱ <string.h>
#অন্তর্ভাৱ <sys/time.h>
#অন্তর্ভাৱ <sys/times.h>
#অন্তর্ভাৱ <time.h>
#অন্তর্ভাৱ <unistd.h>
#অন্তর্ভাৱ <assert.h>

/* Scheduler includes. */
#অন্তর্ভাৱ "esp_heap_caps.h"
#অন্তর্ভাৱ "FreeRTOS.h"
#অন্তর্ভাৱ "task.h"
#অন্তর্ভাৱ "esp_task.h"
#অন্তর্ভাৱ "timers.h"
#অন্তর্ভাৱ "utils/wait_for_event.h"
#অন্তর্ভাৱ "esp_log.h"
/*-----------------------------------------------------------*/

#define SIG_RESUME SIGUSR1

প্রকারনির্দেশ সংরচনা THREAD
{
    pthread_t pthread;
    TaskFunction_t pxCode;
    শূন্য *pvParams;
    BaseType_t xDying;
    সংরচনা event *ev;
} Thread_t;

/*
 * The additional per-thread data is stored at the beginning of the
 * task's stack.
 */
স্থির অন্তঃস্থ Thread_t *prvGetThreadFromTask(TaskHandle_t xTask)
{
    স্তূপ_Type_t *px_শীৰ্ষ_Of_স্তূপ = *(স্তূপ_Type_t **)xTask;

    ঘূৰাই (Thread_t *)(px_শীৰ্ষ_Of_স্তূপ + 1);
}

/*-----------------------------------------------------------*/

স্থির pthread_once_t hSigSetupThread = PTHREAD_ONCE_INIT;
স্থির sigset_t xAllSignals;
স্থির sigset_t xSchedulerOriginalSignalMask;
স্থির pthread_t hMainThread = ( pthread_t )রিক্ত;

// These are saved as part of a thread's state in prvSwitchThread()
স্থির চঞ্চল BaseType_t uxCriticalNestingIDF = 0;    /* Track nesting calls for IDF style critical sections. FreeRTOS critical section nesting is maintained in the TCB. */
স্থির চঞ্চল UBaseType_t uxInterruptNesting = 0;     /* Tracks if we are currently in an interrupt. */
স্থির চঞ্চল BaseType_t uxInterruptLevel = 0;        /* Tracks the current level (i.e., interrupt mask) */
/*-----------------------------------------------------------*/

স্থির BaseType_t xSchedulerEnd = pdFALSE;
/*-----------------------------------------------------------*/

স্থির শূন্য prvSetupSignalsAndSchedulerPolicy( শূন্য );
স্থির শূন্য prvSetupTimerInterrupt( শূন্য );
স্থির শূন্য *prvWaitForStart( শূন্য * pvParams );
স্থির শূন্য prvSwitchThread( Thread_t * xThreadToResume,
                             Thread_t *xThreadToSuspend );
স্থির শূন্য prvSuspendSelf( Thread_t * thread);
স্থির শূন্য prvResumeThread( Thread_t * xThreadId );
স্থির শূন্য vPortSystemTickHandler( পূর্ণাঙ্ক sig );
স্থির শূন্য vPortStartFirstTask( শূন্য );
/*-----------------------------------------------------------*/

স্থির শূন্য prvFatalError( ধ্রুৱ ৱর্ণ *pcCall, পূর্ণাঙ্ক iErrno )
{
    fprintf( stderr, "%s: %s\n", pcCall, strerror( iErrno ) );
    abort();
}

/*
 * See header file for description.
 */
স্তূপ_Type_t *px_Port_Initialise_স্তূপ( স্তূপ_Type_t *px_শীৰ্ষ_Of_স্তূপ,
                                    স্তূপ_Type_t *px_End_Of_স্তূপ,
                                    TaskFunction_t pxCode,
                                    শূন্য *pvParameters )
{
    Thread_t *thread;
    pthread_attr_t xThreadAttributes;
    size_t ul_স্তূপ_Size;
    পূর্ণাঙ্ক iRet;

    (শূন্য)pthread_once( &hSigSetupThread, prvSetupSignalsAndSchedulerPolicy );

    /*
     * Store the additional thread data at the start of the stack.
     */
    thread = (Thread_t *)(px_শীৰ্ষ_Of_স্তূপ + 1) - 1;
    px_শীৰ্ষ_Of_স্তূপ = (স্তূপ_Type_t *)thread - 1;
    ul_স্তূপ_Size = (px_শীৰ্ষ_Of_স্তূপ + 1 - px_End_Of_স্তূপ) * পরিমাণ(*px_শীৰ্ষ_Of_স্তূপ);

    thread->pxCode = pxCode;
    thread->pvParams = pvParameters;
    thread->xDying = pdFALSE;

    pthread_attr_init( &xThreadAttributes );
    pthread_attr_setstack( &xThreadAttributes, px_End_Of_স্তূপ, ul_স্তূপ_Size );

    thread->ev = event_create();

    BaseType_t prev_intr_level = xPortSetInterruptMask();

    iRet = pthread_create( &thread->pthread, &xThreadAttributes,
                           prvWaitForStart, thread );
    যদি ( iRet )
    {
        prvFatalError( "pthread_create", iRet );
    }

    vPortClearInterruptMask( prev_intr_level );

    ঘূৰাই px_শীৰ্ষ_Of_স্তূপ;
}
/*-----------------------------------------------------------*/

শূন্য vPortStartFirstTask( শূন্য )
{
    Thread_t *pxFirstThread = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    /* Start the first task. */
    prvResumeThread( pxFirstThread );
}
/*-----------------------------------------------------------*/

/*
 * See header file for description.
 */
BaseType_t xPortStartScheduler( শূন্য )
{
    পূর্ণাঙ্ক iSignal;
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

    যেতিয়ালৈ ( !xSchedulerEnd )
    {
        sigwait( &xSignals, &iSignal );
    }

    /* Cancel the Idle task and free its resources */
#যদি ( INCLUDE_xTaskGetIdleTaskHandle == 1 )
    vPortCancelThread( xTaskGetIdleTaskHandle() );
#endif

#যদি ( configUSE_TIMERS == 1 )
    /* Cancel the Timer task and free its resources */
    vPortCancelThread( xTimerGetTimerDaemonTaskHandle() );
#endif /* configUSE_TIMERS */

    /* Restore original signal mask. */
    (শূন্য)pthread_sigmask( SIG_SETMASK, &xSchedulerOriginalSignalMask,  রিক্ত );

    ঘূৰাই 0;
}
/*-----------------------------------------------------------*/

শূন্য vPortEndScheduler( শূন্য )
{
    সংরচনা itimerval itimer;
    সংরচনা sigaction sigtick;
    Thread_t *xCurrentThread;

    /* Stop the timer and ignore any pending SIGALRMs that would end
     * up running on the main thread when it is resumed. */
    itimer.it_value.tv_sec = 0;
    itimer.it_value.tv_usec = 0;

    itimer.it_interval.tv_sec = 0;
    itimer.it_interval.tv_usec = 0;
    (শূন্য)setitimer( ITIMER_REAL, &itimer, রিক্ত );

    sigtick.sa_flags = 0;
    sigtick.sa_handler = SIG_IGN;
    sigemptyset( &sigtick.sa_mask );
    sigaction( SIGALRM, &sigtick, রিক্ত );

    /* Signal the scheduler to exit its loop. */
    xSchedulerEnd = pdTRUE;
    (শূন্য)pthread_kill( hMainThread, SIG_RESUME );

    xCurrentThread = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );
    prvSuspendSelf(xCurrentThread);
}
/*-----------------------------------------------------------*/

স্থির শূন্য vPortDisableInterrupts( শূন্য )
{
    pthread_sigmask( SIG_BLOCK, &xAllSignals, রিক্ত );
}
/*-----------------------------------------------------------*/

স্থির শূন্য vPortEnableInterrupts( শূন্য )
{
    pthread_sigmask( SIG_UNBLOCK, &xAllSignals, রিক্ত );
}
/*-----------------------------------------------------------*/

শূন্য vPortEnterCriticalIDF( শূন্য )
{
    যদি ( uxCriticalNestingIDF == 0 && uxInterruptLevel == 0)
    {
        vPortDisableInterrupts();
    }
    uxCriticalNestingIDF++;
}
/*-----------------------------------------------------------*/

শূন্য vPortExitCriticalIDF( শূন্য )
{
    uxCriticalNestingIDF--;

    /* If we have reached 0 then re-enable the interrupts. */
    যদি( uxCriticalNestingIDF == 0 && uxInterruptLevel == 0)
    {
        vPortEnableInterrupts();
    }
}
/*-----------------------------------------------------------*/

শূন্য vPortYieldFromISR( শূন্য )
{
    Thread_t *xThreadToSuspend;
    Thread_t *xThreadToResume;

    xThreadToSuspend = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    vTaskSwitchContext();

    xThreadToResume = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    prvSwitchThread( xThreadToResume, xThreadToSuspend );
}
/*-----------------------------------------------------------*/

শূন্য vPortYield( শূন্য )
{
    BaseType_t prev_intr_level = xPortSetInterruptMask();

    vPortYieldFromISR();

    vPortClearInterruptMask( prev_intr_level );
}
/*-----------------------------------------------------------*/

/* In SMP code, the disable/enable interrupt macros are calling the set/get interrupt mask functions below.
   Hence, we need to call vPortDisableInterrupts() and vPortEnableInterrupts(), otherwise interrupts
   are never disabled/enabled. */

BaseType_t xPortSetInterruptMask( শূন্য )
{
    যদি (uxInterruptLevel == 0 && uxCriticalNestingIDF == 0) {
        vPortDisableInterrupts();
    }
    BaseType_t prev_intr_level = uxInterruptLevel;
    uxInterruptLevel++;
    ঘূৰাই prev_intr_level;
}
/*-----------------------------------------------------------*/

শূন্য vPortClearInterruptMask( BaseType_t xMask )
{
    // Only re-enable interrupts if xMask is 0
    uxInterruptLevel = xMask;
    যদি (uxInterruptLevel == 0 && uxCriticalNestingIDF == 0) {
        vPortEnableInterrupts();
    }
}
/*-----------------------------------------------------------*/

স্থির uint64_t prvGetTimeNs(শূন্য)
{
    সংরচনা timespec t;

    clock_gettime(CLOCK_MONOTONIC, &t);

    ঘূৰাই t.tv_sec * 1000000000ull + t.tv_nsec;
}

স্থির uint64_t prvStartTimeNs;
/* commented as part of the code below in vPortSystemTickHandler,
 * to adjust timing according to full demo requirements */
/* static uint64_t prvTickCount; */

/*
 * Setup the systick timer to generate the tick interrupts at the required
 * frequency.
 */
শূন্য prvSetupTimerInterrupt( শূন্য )
{
    সংরচনা itimerval itimer;
    পূর্ণাঙ্ক iRet;

    /* Initialise the structure with the current timer information. */
    iRet = getitimer( ITIMER_REAL, &itimer );
    যদি ( iRet )
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
    iRet = setitimer( ITIMER_REAL, &itimer, রিক্ত );
    যদি ( iRet )
    {
        prvFatalError( "setitimer", errno );
    }

    prvStartTimeNs = prvGetTimeNs();
}
/*-----------------------------------------------------------*/

স্থির শূন্য vPortSystemTickHandler( পূর্ণাঙ্ক sig )
{
    Thread_t *pxThreadToSuspend;
    Thread_t *pxThreadToResume;
    BaseType_t xSwitchRequired;
    /* uint64_t xExpectedTicks; */

    // Handling a timer signal, so we are currently in an interrupt.
    uxInterruptNesting++;

#যদি ( configUSE_PREEMPTION == 1 )
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

#যদি ( configUSE_PREEMPTION == 1 )
    যদি (xSwitchRequired == pdTRUE) {
        /* Select Next Task. */
        vTaskSwitchContext();

        pxThreadToResume = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

        prvSwitchThread(pxThreadToResume, pxThreadToSuspend);
    }
#অন্যথা
    (শূন্য)xSwitchRequired;
#endif

    // Returning from the timer signal handler, so we are exiting the interrupt.
    uxInterruptNesting--;
}
/*-----------------------------------------------------------*/

শূন্য vPortThreadDying( শূন্য *pxTaskToDelete, চঞ্চল BaseType_t *pxPendYield )
{
    Thread_t *pxThread = prvGetThreadFromTask( pxTaskToDelete );

    pxThread->xDying = pdTRUE;
}

শূন্য vPortCancelThread( শূন্য *pxTaskToDelete )
{
    Thread_t *pxThreadToCancel = prvGetThreadFromTask( pxTaskToDelete );

    /*
     * The thread has already been suspended so it can be safely cancelled.
     */
    pthread_cancel( pxThreadToCancel->pthread );
    pthread_join( pxThreadToCancel->pthread, রিক্ত );
    event_delete( pxThreadToCancel->ev );
}
/*-----------------------------------------------------------*/

স্থির শূন্য *prvWaitForStart( শূন্য * pvParams )
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

    ঘূৰাই রিক্ত;
}
/*-----------------------------------------------------------*/

স্থির শূন্য prvSwitchThread( Thread_t *pxThreadToResume,
                             Thread_t *pxThreadToSuspend )
{
    BaseType_t uxSavedCriticalNestingIDF;
    BaseType_t uxSavedInterruptNesting;
    BaseType_t uxSavedInterruptLevel;

    যদি ( pxThreadToSuspend != pxThreadToResume )
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
        যদি ( pxThreadToSuspend->xDying )
        {
            pthread_exit( রিক্ত );
        }
        prvSuspendSelf( pxThreadToSuspend );

        uxCriticalNestingIDF = uxSavedCriticalNestingIDF;
        uxInterruptNesting = uxSavedInterruptNesting;
        uxInterruptLevel = uxSavedInterruptLevel;
    }
}
/*-----------------------------------------------------------*/

স্থির শূন্য prvSuspendSelf( Thread_t *thread )
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

স্থির শূন্য prvResumeThread( Thread_t *xThreadId )
{
    যদি ( pthread_self() != xThreadId->pthread )
    {
        event_signal(xThreadId->ev);
    }
}
/*-----------------------------------------------------------*/

স্থির শূন্য prvSetupSignalsAndSchedulerPolicy( শূন্য )
{
    সংরচনা sigaction sigresume, sigtick;
    পূর্ণাঙ্ক iRet;

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
    (শূন্য)pthread_sigmask( SIG_SETMASK, &xAllSignals,
                           &xSchedulerOriginalSignalMask );

    /* SIG_RESUME is only used with sigwait() so doesn't need a
       handler. */
    sigresume.sa_flags = 0;
    sigresume.sa_handler = SIG_IGN;
    sigfillset( &sigresume.sa_mask );

    sigtick.sa_flags = 0;
    sigtick.sa_handler = vPortSystemTickHandler;
    sigfillset( &sigtick.sa_mask );

    iRet = sigaction( SIG_RESUME, &sigresume, রিক্ত );
    যদি ( iRet )
    {
        prvFatalError( "sigaction", errno );
    }

    iRet = sigaction( SIGALRM, &sigtick, রিক্ত );
    যদি ( iRet )
    {
        prvFatalError( "sigaction", errno );
    }
}
/*-----------------------------------------------------------*/

unsigned long ulPortGetRunTime( শূন্য )
{
    সংরচনা tms xTimes;

    times( &xTimes );

    ঘূৰাই ( unsigned long ) xTimes.tms_utime;
}
/*-----------------------------------------------------------*/
bool portVALID_LIST_MEM(ধ্রুৱ শূন্য *ptr)
{
    ঘূৰাই true;
}

bool portVALID_TCB_MEM(ধ্রুৱ শূন্য *ptr)
{
    ঘূৰাই true;
}

bool port_VALID_স্তূপ_MEM(ধ্রুৱ শূন্য *ptr)
{
    ঘূৰাই true;
}
/*-----------------------------------------------------------*/

portMUX_TYPE port_xTaskLock = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE port_xISRLock = portMUX_INITIALIZER_UNLOCKED;

স্থির ধ্রুৱ ৱর্ণ *TAG = "port";

BaseType_t xPortCheckIfInISR(শূন্য)
{
    ঘূৰাই (uxInterruptNesting == 0) ? pdFALSE : pdTRUE;
}

শূন্য app_main(শূন্য);

স্থির শূন্য main_task(শূন্য* args)
{
    app_main();
    vTaskDelete(রিক্ত);
}

পূর্ণাঙ্ক main(পূর্ণাঙ্ক argc, ধ্রুৱ ৱর্ণ **argv)
{
    // This makes sure that stdio is flushed after each '\n' so that idf.py monitor
    // reads the program output on time.
    setvbuf(stdout, রিক্ত, _IOLBF, 0);

    usleep(1000);

    BaseType_t res;

#যদি ( config_অঙ্ক_CORES > 1 )
    res = xTaskCreateAffinitySet(&main_task, "main",
                                 ESP_TASK_MAIN_স্তূপ, রিক্ত,
                                 ESP_TASK_MAIN_PRIO, ESP_TASK_MAIN_CORE, রিক্ত);
#অন্যথা
    res = xTaskCreate(&main_task, "main",
                      ESP_TASK_MAIN_স্তূপ, রিক্ত,
                      ESP_TASK_MAIN_PRIO, রিক্ত);
#endif

    প্রতিজ্ঞা(res == pdTRUE);
    (শূন্য)res;

    ESP_LOGI(TAG, "Starting SMP scheduler.");
    vTaskStartScheduler();

    // This line should never be reached
    প্রতিজ্ঞা(false);
}

শূন্য esp_vApplicationIdleHook(শূন্য)
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

শূন্য esp_vApplicationTickHook( শূন্য ) { }

#যদি  (  configUSE_TICK_HOOK > 0 )
শূন্য vApplicationTickHook( শূন্য )
{
    esp_vApplicationTickHook();
}
#endif

#যদি ( configSUPPORT_STATIC_ALLOCATION == 1 )
/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
 * implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
 * used by the Idle task. */
শূন্য vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                    স্তূপ_Type_t ** ppx_Idle_Task_স্তূপ_Buffer,
                                    uint32_t * pul_Idle_Task_স্তূপ_Size )
{
/* If the buffers to be provided to the Idle task are declared inside this
 * function then they must be declared static - otherwise they will be allocated on
 * the stack and so not exists after this function exits. */
    স্থির StaticTask_t xIdleTaskTCB;
    স্থির স্তূপ_Type_t ux_Idle_Task_স্তূপ[ config_MINIMAL_স্তূপ_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
     * state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppx_Idle_Task_স্তূপ_Buffer = ux_Idle_Task_স্তূপ;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in bytes. */
    *pul_Idle_Task_স্তূপ_Size = config_MINIMAL_স্তূপ_SIZE;
}
#endif // configSUPPORT_STATIC_ALLOCATION == 1
/*-----------------------------------------------------------*/

#যদি ( (configSUPPORT_STATIC_ALLOCATION == 1) && (configUSE_TIMERS == 1))

/* When configSUPPORT_STATIC_ALLOCATION is set to 1 the application writer can
 * use a callback function to optionally provide the memory required by the idle
 * and timer tasks.  This is the stack that will be used by the timer task.  It is
 * declared here, as a global, so it can be checked by a test that is implemented
 * in a different file. */
স্তূপ_Type_t ux_Timer_Task_স্তূপ[ config_TIMER_TASK_স্তূপ_DEPTH ];


/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
 * application must provide an implementation of vApplicationGetTimerTaskMemory()
 * to provide the memory that is used by the Timer service task. */
শূন্য vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                     স্তূপ_Type_t ** ppx_Timer_Task_স্তূপ_Buffer,
                                     uint32_t * pul_Timer_Task_স্তূপ_Size )
{
/* If the buffers to be provided to the Timer task are declared inside this
 * function then they must be declared static - otherwise they will be allocated on
 * the stack and so not exists after this function exits. */
    স্থির StaticTask_t xTimerTaskTCB;

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
     * task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppx_Timer_Task_স্তূপ_Buffer = ux_Timer_Task_স্তূপ;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in bytes. */
    *pul_Timer_Task_স্তূপ_Size = config_TIMER_TASK_স্তূপ_DEPTH;
}
#endif // configSUPPORT_STATIC_ALLOCATION == 1 && (configUSE_TIMERS == 1)

শূন্য vPortTakeLock( portMUX_TYPE *lock )
{
    spinlock_acquire( lock, portMUX_NO_TIMEOUT);
}

শূন্য vPortReleaseLock( portMUX_TYPE *lock )
{
    spinlock_release( lock );
}

#define FREERTOS_SMP_MALLOC_CAPS    (MALLOC_CAP_INTERNAL|MALLOC_CAP_8BIT)

শূন্য *pvPortMalloc( size_t xSize )
{
    ঘূৰাই heap_caps_malloc(xSize, FREERTOS_SMP_MALLOC_CAPS);
}

শূন্য vPortFree( শূন্য *pv )
{
    heap_caps_free(pv);
}

শূন্য __attribute__((weak)) v_Application_স্তূপ_Overflow_Hook(TaskHandle_t xTask, ৱর্ণ *pcTaskName)
{
#define ERR_STR1 "***ERROR*** A stack overflow in task "
#define ERR_STR2 " has been detected."
    ধ্রুৱ ৱর্ণ *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    ৱর্ণ buf[পরিমাণ(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + পরিমাণ(ERR_STR2) + 1 /* null char */] = {0};

    ৱর্ণ *dest = buf;
    বাবে (পূর্ণাঙ্ক i = 0; i < পরিমাণ(str) / পরিমাণ(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
	printf("%s\n", buf);
    abort();
}

// ------- Thread Local Storage Pointers Deletion Callbacks -------

#যদি ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
শূন্য vPortTLSPointersDelCb( শূন্য *pxTCB )
{
    /* Typecast pxTCB to StaticTask_t type to access TCB struct members.
     * pvDummy15 corresponds to pvThreadLocalStoragePointers member of the TCB.
     */
    StaticTask_t *tcb = ( StaticTask_t * )pxTCB;

    /* The TLSP deletion callbacks are stored at an offset of (configNUM_THREAD_LOCAL_STORAGE_POINTERS/2) */
    TlsDeleteCallbackFunction_t *pvThreadLocalStoragePointersDelCallback = ( TlsDeleteCallbackFunction_t * )( &( tcb->pvDummy15[ ( config_অঙ্ক_THREAD_LOCAL_STORAGE_POINTERS / 2 ) ] ) );

    /* We need to iterate over half the depth of the pvThreadLocalStoragePointers area
     * to access all TLS pointers and their respective TLS deletion callbacks.
     */
    বাবে ( পূর্ণাঙ্ক x = 0; x < ( config_অঙ্ক_THREAD_LOCAL_STORAGE_POINTERS / 2 ); x++ ) {
        যদি ( pvThreadLocalStoragePointersDelCallback[ x ] != রিক্ত ) {  //If del cb is set
            // We skip the check if the callback is executable as that is difficult to determine for different
            // platforms (compare xtensa and riscv code).
            pvThreadLocalStoragePointersDelCallback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#endif // CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS

শূন্য vPortCleanUpTCB ( শূন্য *pxTCB )
{
    #যদি ( CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK )
        /* Call the user defined task pre-deletion hook */
        বাহ্য শূন্য vTaskPreDeletionHook( শূন্য * pxTCB );
        vTaskPreDeletionHook( pxTCB );
    #endif /* CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK */

#যদি ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
    /* Call TLS pointers deletion callbacks */
    vPortTLSPointersDelCb( pxTCB );
#endif /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

    vPortCancelThread(pxTCB);
}
