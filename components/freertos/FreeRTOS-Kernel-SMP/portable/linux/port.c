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

#અન્તર્ભાવ <errno.h>
#અન્તર્ભાવ <pthread.h>
#અન્તર્ભાવ <signal.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <sys/time.h>
#અન્તર્ભાવ <sys/times.h>
#અન્તર્ભાવ <time.h>
#અન્તર્ભાવ <unistd.h>
#અન્તર્ભાવ <assert.h>

/* Scheduler includes. */
#અન્તર્ભાવ "esp_heap_caps.h"
#અન્તર્ભાવ "FreeRTOS.h"
#અન્તર્ભાવ "task.h"
#અન્તર્ભાવ "esp_task.h"
#અન્તર્ભાવ "timers.h"
#અન્તર્ભાવ "utils/wait_for_event.h"
#અન્તર્ભાવ "esp_log.h"
/*-----------------------------------------------------------*/

#define SIG_RESUME SIGUSR1

પ્રકારવ્યાખ્યા સંરચના THREAD
{
    pthread_t pthread;
    TaskFunction_t pxCode;
    શૂન્ય *pvParams;
    BaseType_t xDying;
    સંરચના event *ev;
} Thread_t;

/*
 * The additional per-thread data is stored at the beginning of the
 * task's stack.
 */
સ્થિર અંતઃરેખ Thread_t *prvGetThreadFromTask(TaskHandle_t xTask)
{
    ઢગલો_Type_t *px_ટોચ_Of_ઢગલો = *(ઢગલો_Type_t **)xTask;

    પાછા (Thread_t *)(px_ટોચ_Of_ઢગલો + 1);
}

/*-----------------------------------------------------------*/

સ્થિર pthread_once_t hSigSetupThread = PTHREAD_ONCE_INIT;
સ્થિર sigset_t xAllSignals;
સ્થિર sigset_t xSchedulerOriginalSignalMask;
સ્થિર pthread_t hMainThread = ( pthread_t )ખાલી;

// These are saved as part of a thread's state in prvSwitchThread()
સ્થિર અસ્થિર BaseType_t uxCriticalNestingIDF = 0;    /* Track nesting calls for IDF style critical sections. FreeRTOS critical section nesting is maintained in the TCB. */
સ્થિર અસ્થિર UBaseType_t uxInterruptNesting = 0;     /* Tracks if we are currently in an interrupt. */
સ્થિર અસ્થિર BaseType_t uxInterruptLevel = 0;        /* Tracks the current level (i.e., interrupt mask) */
/*-----------------------------------------------------------*/

સ્થિર BaseType_t xSchedulerEnd = pdFALSE;
/*-----------------------------------------------------------*/

સ્થિર શૂન્ય prvSetupSignalsAndSchedulerPolicy( શૂન્ય );
સ્થિર શૂન્ય prvSetupTimerInterrupt( શૂન્ય );
સ્થિર શૂન્ય *prvWaitForStart( શૂન્ય * pvParams );
સ્થિર શૂન્ય prvSwitchThread( Thread_t * xThreadToResume,
                             Thread_t *xThreadToSuspend );
સ્થિર શૂન્ય prvSuspendSelf( Thread_t * thread);
સ્થિર શૂન્ય prvResumeThread( Thread_t * xThreadId );
સ્થિર શૂન્ય vPortSystemTickHandler( પૂર્ણાંક sig );
સ્થિર શૂન્ય vPortStartFirstTask( શૂન્ય );
/*-----------------------------------------------------------*/

સ્થિર શૂન્ય prvFatalError( અચલ અક્ષર *pcCall, પૂર્ણાંક iErrno )
{
    fprintf( stderr, "%s: %s\n", pcCall, strerror( iErrno ) );
    abort();
}

/*
 * See header file for description.
 */
ઢગલો_Type_t *px_Port_Initialise_ઢગલો( ઢગલો_Type_t *px_ટોચ_Of_ઢગલો,
                                    ઢગલો_Type_t *px_End_Of_ઢગલો,
                                    TaskFunction_t pxCode,
                                    શૂન્ય *pvParameters )
{
    Thread_t *thread;
    pthread_attr_t xThreadAttributes;
    size_t ul_ઢગલો_Size;
    પૂર્ણાંક iRet;

    (શૂન્ય)pthread_once( &hSigSetupThread, prvSetupSignalsAndSchedulerPolicy );

    /*
     * Store the additional thread data at the start of the stack.
     */
    thread = (Thread_t *)(px_ટોચ_Of_ઢગલો + 1) - 1;
    px_ટોચ_Of_ઢગલો = (ઢગલો_Type_t *)thread - 1;
    ul_ઢગલો_Size = (px_ટોચ_Of_ઢગલો + 1 - px_End_Of_ઢગલો) * કદ(*px_ટોચ_Of_ઢગલો);

    thread->pxCode = pxCode;
    thread->pvParams = pvParameters;
    thread->xDying = pdFALSE;

    pthread_attr_init( &xThreadAttributes );
    pthread_attr_setstack( &xThreadAttributes, px_End_Of_ઢગલો, ul_ઢગલો_Size );

    thread->ev = event_create();

    BaseType_t prev_intr_level = xPortSetInterruptMask();

    iRet = pthread_create( &thread->pthread, &xThreadAttributes,
                           prvWaitForStart, thread );
    જો ( iRet )
    {
        prvFatalError( "pthread_create", iRet );
    }

    vPortClearInterruptMask( prev_intr_level );

    પાછા px_ટોચ_Of_ઢગલો;
}
/*-----------------------------------------------------------*/

શૂન્ય vPortStartFirstTask( શૂન્ય )
{
    Thread_t *pxFirstThread = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    /* Start the first task. */
    prvResumeThread( pxFirstThread );
}
/*-----------------------------------------------------------*/

/*
 * See header file for description.
 */
BaseType_t xPortStartScheduler( શૂન્ય )
{
    પૂર્ણાંક iSignal;
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

    જ્યારે ( !xSchedulerEnd )
    {
        sigwait( &xSignals, &iSignal );
    }

    /* Cancel the Idle task and free its resources */
#જો ( INCLUDE_xTaskGetIdleTaskHandle == 1 )
    vPortCancelThread( xTaskGetIdleTaskHandle() );
#endif

#જો ( configUSE_TIMERS == 1 )
    /* Cancel the Timer task and free its resources */
    vPortCancelThread( xTimerGetTimerDaemonTaskHandle() );
#endif /* configUSE_TIMERS */

    /* Restore original signal mask. */
    (શૂન્ય)pthread_sigmask( SIG_SETMASK, &xSchedulerOriginalSignalMask,  ખાલી );

    પાછા 0;
}
/*-----------------------------------------------------------*/

શૂન્ય vPortEndScheduler( શૂન્ય )
{
    સંરચના itimerval itimer;
    સંરચના sigaction sigtick;
    Thread_t *xCurrentThread;

    /* Stop the timer and ignore any pending SIGALRMs that would end
     * up running on the main thread when it is resumed. */
    itimer.it_value.tv_sec = 0;
    itimer.it_value.tv_usec = 0;

    itimer.it_interval.tv_sec = 0;
    itimer.it_interval.tv_usec = 0;
    (શૂન્ય)setitimer( ITIMER_REAL, &itimer, ખાલી );

    sigtick.sa_flags = 0;
    sigtick.sa_handler = SIG_IGN;
    sigemptyset( &sigtick.sa_mask );
    sigaction( SIGALRM, &sigtick, ખાલી );

    /* Signal the scheduler to exit its loop. */
    xSchedulerEnd = pdTRUE;
    (શૂન્ય)pthread_kill( hMainThread, SIG_RESUME );

    xCurrentThread = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );
    prvSuspendSelf(xCurrentThread);
}
/*-----------------------------------------------------------*/

સ્થિર શૂન્ય vPortDisableInterrupts( શૂન્ય )
{
    pthread_sigmask( SIG_BLOCK, &xAllSignals, ખાલી );
}
/*-----------------------------------------------------------*/

સ્થિર શૂન્ય vPortEnableInterrupts( શૂન્ય )
{
    pthread_sigmask( SIG_UNBLOCK, &xAllSignals, ખાલી );
}
/*-----------------------------------------------------------*/

શૂન્ય vPortEnterCriticalIDF( શૂન્ય )
{
    જો ( uxCriticalNestingIDF == 0 && uxInterruptLevel == 0)
    {
        vPortDisableInterrupts();
    }
    uxCriticalNestingIDF++;
}
/*-----------------------------------------------------------*/

શૂન્ય vPortExitCriticalIDF( શૂન્ય )
{
    uxCriticalNestingIDF--;

    /* If we have reached 0 then re-enable the interrupts. */
    જો( uxCriticalNestingIDF == 0 && uxInterruptLevel == 0)
    {
        vPortEnableInterrupts();
    }
}
/*-----------------------------------------------------------*/

શૂન્ય vPortYieldFromISR( શૂન્ય )
{
    Thread_t *xThreadToSuspend;
    Thread_t *xThreadToResume;

    xThreadToSuspend = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    vTaskSwitchContext();

    xThreadToResume = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    prvSwitchThread( xThreadToResume, xThreadToSuspend );
}
/*-----------------------------------------------------------*/

શૂન્ય vPortYield( શૂન્ય )
{
    BaseType_t prev_intr_level = xPortSetInterruptMask();

    vPortYieldFromISR();

    vPortClearInterruptMask( prev_intr_level );
}
/*-----------------------------------------------------------*/

/* In SMP code, the disable/enable interrupt macros are calling the set/get interrupt mask functions below.
   Hence, we need to call vPortDisableInterrupts() and vPortEnableInterrupts(), otherwise interrupts
   are never disabled/enabled. */

BaseType_t xPortSetInterruptMask( શૂન્ય )
{
    જો (uxInterruptLevel == 0 && uxCriticalNestingIDF == 0) {
        vPortDisableInterrupts();
    }
    BaseType_t prev_intr_level = uxInterruptLevel;
    uxInterruptLevel++;
    પાછા prev_intr_level;
}
/*-----------------------------------------------------------*/

શૂન્ય vPortClearInterruptMask( BaseType_t xMask )
{
    // Only re-enable interrupts if xMask is 0
    uxInterruptLevel = xMask;
    જો (uxInterruptLevel == 0 && uxCriticalNestingIDF == 0) {
        vPortEnableInterrupts();
    }
}
/*-----------------------------------------------------------*/

સ્થિર uint64_t prvGetTimeNs(શૂન્ય)
{
    સંરચના timespec t;

    clock_gettime(CLOCK_MONOTONIC, &t);

    પાછા t.tv_sec * 1000000000ull + t.tv_nsec;
}

સ્થિર uint64_t prvStartTimeNs;
/* commented as part of the code below in vPortSystemTickHandler,
 * to adjust timing according to full demo requirements */
/* static uint64_t prvTickCount; */

/*
 * Setup the systick timer to generate the tick interrupts at the required
 * frequency.
 */
શૂન્ય prvSetupTimerInterrupt( શૂન્ય )
{
    સંરચના itimerval itimer;
    પૂર્ણાંક iRet;

    /* Initialise the structure with the current timer information. */
    iRet = getitimer( ITIMER_REAL, &itimer );
    જો ( iRet )
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
    iRet = setitimer( ITIMER_REAL, &itimer, ખાલી );
    જો ( iRet )
    {
        prvFatalError( "setitimer", errno );
    }

    prvStartTimeNs = prvGetTimeNs();
}
/*-----------------------------------------------------------*/

સ્થિર શૂન્ય vPortSystemTickHandler( પૂર્ણાંક sig )
{
    Thread_t *pxThreadToSuspend;
    Thread_t *pxThreadToResume;
    BaseType_t xSwitchRequired;
    /* uint64_t xExpectedTicks; */

    // Handling a timer signal, so we are currently in an interrupt.
    uxInterruptNesting++;

#જો ( configUSE_PREEMPTION == 1 )
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

#જો ( configUSE_PREEMPTION == 1 )
    જો (xSwitchRequired == pdTRUE) {
        /* Select Next Task. */
        vTaskSwitchContext();

        pxThreadToResume = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

        prvSwitchThread(pxThreadToResume, pxThreadToSuspend);
    }
#નહીંતર
    (શૂન્ય)xSwitchRequired;
#endif

    // Returning from the timer signal handler, so we are exiting the interrupt.
    uxInterruptNesting--;
}
/*-----------------------------------------------------------*/

શૂન્ય vPortThreadDying( શૂન્ય *pxTaskToDelete, અસ્થિર BaseType_t *pxPendYield )
{
    Thread_t *pxThread = prvGetThreadFromTask( pxTaskToDelete );

    pxThread->xDying = pdTRUE;
}

શૂન્ય vPortCancelThread( શૂન્ય *pxTaskToDelete )
{
    Thread_t *pxThreadToCancel = prvGetThreadFromTask( pxTaskToDelete );

    /*
     * The thread has already been suspended so it can be safely cancelled.
     */
    pthread_cancel( pxThreadToCancel->pthread );
    pthread_join( pxThreadToCancel->pthread, ખાલી );
    event_delete( pxThreadToCancel->ev );
}
/*-----------------------------------------------------------*/

સ્થિર શૂન્ય *prvWaitForStart( શૂન્ય * pvParams )
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

    પાછા ખાલી;
}
/*-----------------------------------------------------------*/

સ્થિર શૂન્ય prvSwitchThread( Thread_t *pxThreadToResume,
                             Thread_t *pxThreadToSuspend )
{
    BaseType_t uxSavedCriticalNestingIDF;
    BaseType_t uxSavedInterruptNesting;
    BaseType_t uxSavedInterruptLevel;

    જો ( pxThreadToSuspend != pxThreadToResume )
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
        જો ( pxThreadToSuspend->xDying )
        {
            pthread_exit( ખાલી );
        }
        prvSuspendSelf( pxThreadToSuspend );

        uxCriticalNestingIDF = uxSavedCriticalNestingIDF;
        uxInterruptNesting = uxSavedInterruptNesting;
        uxInterruptLevel = uxSavedInterruptLevel;
    }
}
/*-----------------------------------------------------------*/

સ્થિર શૂન્ય prvSuspendSelf( Thread_t *thread )
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

સ્થિર શૂન્ય prvResumeThread( Thread_t *xThreadId )
{
    જો ( pthread_self() != xThreadId->pthread )
    {
        event_signal(xThreadId->ev);
    }
}
/*-----------------------------------------------------------*/

સ્થિર શૂન્ય prvSetupSignalsAndSchedulerPolicy( શૂન્ય )
{
    સંરચના sigaction sigresume, sigtick;
    પૂર્ણાંક iRet;

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
    (શૂન્ય)pthread_sigmask( SIG_SETMASK, &xAllSignals,
                           &xSchedulerOriginalSignalMask );

    /* SIG_RESUME is only used with sigwait() so doesn't need a
       handler. */
    sigresume.sa_flags = 0;
    sigresume.sa_handler = SIG_IGN;
    sigfillset( &sigresume.sa_mask );

    sigtick.sa_flags = 0;
    sigtick.sa_handler = vPortSystemTickHandler;
    sigfillset( &sigtick.sa_mask );

    iRet = sigaction( SIG_RESUME, &sigresume, ખાલી );
    જો ( iRet )
    {
        prvFatalError( "sigaction", errno );
    }

    iRet = sigaction( SIGALRM, &sigtick, ખાલી );
    જો ( iRet )
    {
        prvFatalError( "sigaction", errno );
    }
}
/*-----------------------------------------------------------*/

unsigned long ulPortGetRunTime( શૂન્ય )
{
    સંરચના tms xTimes;

    times( &xTimes );

    પાછા ( unsigned long ) xTimes.tms_utime;
}
/*-----------------------------------------------------------*/
bool portVALID_LIST_MEM(અચલ શૂન્ય *ptr)
{
    પાછા true;
}

bool portVALID_TCB_MEM(અચલ શૂન્ય *ptr)
{
    પાછા true;
}

bool port_VALID_ઢગલો_MEM(અચલ શૂન્ય *ptr)
{
    પાછા true;
}
/*-----------------------------------------------------------*/

portMUX_TYPE port_xTaskLock = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE port_xISRLock = portMUX_INITIALIZER_UNLOCKED;

સ્થિર અચલ અક્ષર *TAG = "port";

BaseType_t xPortCheckIfInISR(શૂન્ય)
{
    પાછા (uxInterruptNesting == 0) ? pdFALSE : pdTRUE;
}

શૂન્ય app_main(શૂન્ય);

સ્થિર શૂન્ય main_task(શૂન્ય* args)
{
    app_main();
    vTaskDelete(ખાલી);
}

પૂર્ણાંક મુખ્ય(પૂર્ણાંક argc, અચલ અક્ષર **argv)
{
    // This makes sure that stdio is flushed after each '\n' so that idf.py monitor
    // reads the program output on time.
    setvbuf(stdout, ખાલી, _IOLBF, 0);

    usleep(1000);

    BaseType_t res;

#જો ( config_અંક_CORES > 1 )
    res = xTaskCreateAffinitySet(&main_task, "main",
                                 ESP_TASK_MAIN_ઢગલો, ખાલી,
                                 ESP_TASK_MAIN_PRIO, ESP_TASK_MAIN_CORE, ખાલી);
#નહીંતર
    res = xTaskCreate(&main_task, "main",
                      ESP_TASK_MAIN_ઢગલો, ખાલી,
                      ESP_TASK_MAIN_PRIO, ખાલી);
#endif

    પ્રતિજ્ઞા(res == pdTRUE);
    (શૂન્ય)res;

    ESP_LOGI(TAG, "Starting SMP scheduler.");
    vTaskStartScheduler();

    // This line should never be reached
    પ્રતિજ્ઞા(false);
}

શૂન્ય esp_vApplicationIdleHook(શૂન્ય)
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

શૂન્ય esp_vApplicationTickHook( શૂન્ય ) { }

#જો  (  configUSE_TICK_HOOK > 0 )
શૂન્ય vApplicationTickHook( શૂન્ય )
{
    esp_vApplicationTickHook();
}
#endif

#જો ( configSUPPORT_STATIC_ALLOCATION == 1 )
/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
 * implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
 * used by the Idle task. */
શૂન્ય vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                    ઢગલો_Type_t ** ppx_Idle_Task_ઢગલો_Buffer,
                                    uint32_t * pul_Idle_Task_ઢગલો_Size )
{
/* If the buffers to be provided to the Idle task are declared inside this
 * function then they must be declared static - otherwise they will be allocated on
 * the stack and so not exists after this function exits. */
    સ્થિર StaticTask_t xIdleTaskTCB;
    સ્થિર ઢગલો_Type_t ux_Idle_Task_ઢગલો[ config_MINIMAL_ઢગલો_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
     * state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppx_Idle_Task_ઢગલો_Buffer = ux_Idle_Task_ઢગલો;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in bytes. */
    *pul_Idle_Task_ઢગલો_Size = config_MINIMAL_ઢગલો_SIZE;
}
#endif // configSUPPORT_STATIC_ALLOCATION == 1
/*-----------------------------------------------------------*/

#જો ( (configSUPPORT_STATIC_ALLOCATION == 1) && (configUSE_TIMERS == 1))

/* When configSUPPORT_STATIC_ALLOCATION is set to 1 the application writer can
 * use a callback function to optionally provide the memory required by the idle
 * and timer tasks.  This is the stack that will be used by the timer task.  It is
 * declared here, as a global, so it can be checked by a test that is implemented
 * in a different file. */
ઢગલો_Type_t ux_Timer_Task_ઢગલો[ config_TIMER_TASK_ઢગલો_DEPTH ];


/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
 * application must provide an implementation of vApplicationGetTimerTaskMemory()
 * to provide the memory that is used by the Timer service task. */
શૂન્ય vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                     ઢગલો_Type_t ** ppx_Timer_Task_ઢગલો_Buffer,
                                     uint32_t * pul_Timer_Task_ઢગલો_Size )
{
/* If the buffers to be provided to the Timer task are declared inside this
 * function then they must be declared static - otherwise they will be allocated on
 * the stack and so not exists after this function exits. */
    સ્થિર StaticTask_t xTimerTaskTCB;

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
     * task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppx_Timer_Task_ઢગલો_Buffer = ux_Timer_Task_ઢગલો;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in bytes. */
    *pul_Timer_Task_ઢગલો_Size = config_TIMER_TASK_ઢગલો_DEPTH;
}
#endif // configSUPPORT_STATIC_ALLOCATION == 1 && (configUSE_TIMERS == 1)

શૂન્ય vPortTakeLock( portMUX_TYPE *lock )
{
    spinlock_acquire( lock, portMUX_NO_TIMEOUT);
}

શૂન્ય vPortReleaseLock( portMUX_TYPE *lock )
{
    spinlock_release( lock );
}

#define FREERTOS_SMP_MALLOC_CAPS    (MALLOC_CAP_INTERNAL|MALLOC_CAP_8BIT)

શૂન્ય *pvPortMalloc( size_t xSize )
{
    પાછા heap_caps_malloc(xSize, FREERTOS_SMP_MALLOC_CAPS);
}

શૂન્ય vPortFree( શૂન્ય *pv )
{
    heap_caps_free(pv);
}

શૂન્ય __attribute__((weak)) v_Application_ઢગલો_Overflow_Hook(TaskHandle_t xTask, અક્ષર *pcTaskName)
{
#define ERR_STR1 "***ERROR*** A stack overflow in task "
#define ERR_STR2 " has been detected."
    અચલ અક્ષર *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    અક્ષર buf[કદ(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + કદ(ERR_STR2) + 1 /* null char */] = {0};

    અક્ષર *dest = buf;
    માટે (પૂર્ણાંક i = 0; i < કદ(str) / કદ(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
	printf("%s\n", buf);
    abort();
}

// ------- Thread Local Storage Pointers Deletion Callbacks -------

#જો ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
શૂન્ય vPortTLSPointersDelCb( શૂન્ય *pxTCB )
{
    /* Typecast pxTCB to StaticTask_t type to access TCB struct members.
     * pvDummy15 corresponds to pvThreadLocalStoragePointers member of the TCB.
     */
    StaticTask_t *tcb = ( StaticTask_t * )pxTCB;

    /* The TLSP deletion callbacks are stored at an offset of (configNUM_THREAD_LOCAL_STORAGE_POINTERS/2) */
    TlsDeleteCallbackFunction_t *pvThreadLocalStoragePointersDelCallback = ( TlsDeleteCallbackFunction_t * )( &( tcb->pvDummy15[ ( config_અંક_THREAD_LOCAL_STORAGE_POINTERS / 2 ) ] ) );

    /* We need to iterate over half the depth of the pvThreadLocalStoragePointers area
     * to access all TLS pointers and their respective TLS deletion callbacks.
     */
    માટે ( પૂર્ણાંક x = 0; x < ( config_અંક_THREAD_LOCAL_STORAGE_POINTERS / 2 ); x++ ) {
        જો ( pvThreadLocalStoragePointersDelCallback[ x ] != ખાલી ) {  //If del cb is set
            // We skip the check if the callback is executable as that is difficult to determine for different
            // platforms (compare xtensa and riscv code).
            pvThreadLocalStoragePointersDelCallback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#endif // CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS

શૂન્ય vPortCleanUpTCB ( શૂન્ય *pxTCB )
{
    #જો ( CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK )
        /* Call the user defined task pre-deletion hook */
        બાહ્ય શૂન્ય vTaskPreDeletionHook( શૂન્ય * pxTCB );
        vTaskPreDeletionHook( pxTCB );
    #endif /* CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK */

#જો ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
    /* Call TLS pointers deletion callbacks */
    vPortTLSPointersDelCb( pxTCB );
#endif /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

    vPortCancelThread(pxTCB);
}
