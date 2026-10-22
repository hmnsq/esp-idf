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

#அந்தர்பாவ <errno.h>
#அந்தர்பாவ <pthread.h>
#அந்தர்பாவ <signal.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <sys/time.h>
#அந்தர்பாவ <sys/times.h>
#அந்தர்பாவ <time.h>
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <assert.h>

/* Scheduler includes. */
#அந்தர்பாவ "esp_heap_caps.h"
#அந்தர்பாவ "FreeRTOS.h"
#அந்தர்பாவ "task.h"
#அந்தர்பாவ "esp_task.h"
#அந்தர்பாவ "timers.h"
#அந்தர்பாவ "utils/wait_for_event.h"
#அந்தர்பாவ "esp_log.h"
/*-----------------------------------------------------------*/

#define SIG_RESUME SIGUSR1

வகைவரை கட்டமைப்பு THREAD
{
    pthread_t pthread;
    TaskFunction_t pxCode;
    வெறுமை *pvParams;
    BaseType_t xDying;
    கட்டமைப்பு event *ev;
} Thread_t;

/*
 * The additional per-thread data is stored at the beginning of the
 * task's stack.
 */
நிலையான உள்வரி Thread_t *prvGetThreadFromTask(TaskHandle_t xTask)
{
    அடுக்கு_Type_t *px_உச்சி_Of_அடுக்கு = *(அடுக்கு_Type_t **)xTask;

    திரும்பு (Thread_t *)(px_உச்சி_Of_அடுக்கு + 1);
}

/*-----------------------------------------------------------*/

நிலையான pthread_once_t hSigSetupThread = PTHREAD_ONCE_INIT;
நிலையான sigset_t xAllSignals;
நிலையான sigset_t xSchedulerOriginalSignalMask;
நிலையான pthread_t hMainThread = ( pthread_t )வெற்று;

// These are saved as part of a thread's state in prvSwitchThread()
நிலையான நிலையற்ற BaseType_t uxCriticalNestingIDF = 0;    /* Track nesting calls for IDF style critical sections. FreeRTOS critical section nesting is maintained in the TCB. */
நிலையான நிலையற்ற UBaseType_t uxInterruptNesting = 0;     /* Tracks if we are currently in an interrupt. */
நிலையான நிலையற்ற BaseType_t uxInterruptLevel = 0;        /* Tracks the current level (i.e., interrupt mask) */
/*-----------------------------------------------------------*/

நிலையான BaseType_t xSchedulerEnd = pdFALSE;
/*-----------------------------------------------------------*/

நிலையான வெறுமை prvSetupSignalsAndSchedulerPolicy( வெறுமை );
நிலையான வெறுமை prvSetupTimerInterrupt( வெறுமை );
நிலையான வெறுமை *prvWaitForStart( வெறுமை * pvParams );
நிலையான வெறுமை prvSwitchThread( Thread_t * xThreadToResume,
                             Thread_t *xThreadToSuspend );
நிலையான வெறுமை prvSuspendSelf( Thread_t * thread);
நிலையான வெறுமை prvResumeThread( Thread_t * xThreadId );
நிலையான வெறுமை vPortSystemTickHandler( முழு sig );
நிலையான வெறுமை vPortStartFirstTask( வெறுமை );
/*-----------------------------------------------------------*/

நிலையான வெறுமை prvFatalError( மாறிலி எழுத்து *pcCall, முழு iErrno )
{
    fprintf( stderr, "%s: %s\n", pcCall, strerror( iErrno ) );
    abort();
}

/*
 * See header file for description.
 */
அடுக்கு_Type_t *px_Port_Initialise_அடுக்கு( அடுக்கு_Type_t *px_உச்சி_Of_அடுக்கு,
                                    அடுக்கு_Type_t *px_End_Of_அடுக்கு,
                                    TaskFunction_t pxCode,
                                    வெறுமை *pvParameters )
{
    Thread_t *thread;
    pthread_attr_t xThreadAttributes;
    size_t ul_அடுக்கு_Size;
    முழு iRet;

    (வெறுமை)pthread_once( &hSigSetupThread, prvSetupSignalsAndSchedulerPolicy );

    /*
     * Store the additional thread data at the start of the stack.
     */
    thread = (Thread_t *)(px_உச்சி_Of_அடுக்கு + 1) - 1;
    px_உச்சி_Of_அடுக்கு = (அடுக்கு_Type_t *)thread - 1;
    ul_அடுக்கு_Size = (px_உச்சி_Of_அடுக்கு + 1 - px_End_Of_அடுக்கு) * அளவு(*px_உச்சி_Of_அடுக்கு);

    thread->pxCode = pxCode;
    thread->pvParams = pvParameters;
    thread->xDying = pdFALSE;

    pthread_attr_init( &xThreadAttributes );
    pthread_attr_setstack( &xThreadAttributes, px_End_Of_அடுக்கு, ul_அடுக்கு_Size );

    thread->ev = event_create();

    BaseType_t prev_intr_level = xPortSetInterruptMask();

    iRet = pthread_create( &thread->pthread, &xThreadAttributes,
                           prvWaitForStart, thread );
    எனில் ( iRet )
    {
        prvFatalError( "pthread_create", iRet );
    }

    vPortClearInterruptMask( prev_intr_level );

    திரும்பு px_உச்சி_Of_அடுக்கு;
}
/*-----------------------------------------------------------*/

வெறுமை vPortStartFirstTask( வெறுமை )
{
    Thread_t *pxFirstThread = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    /* Start the first task. */
    prvResumeThread( pxFirstThread );
}
/*-----------------------------------------------------------*/

/*
 * See header file for description.
 */
BaseType_t xPortStartScheduler( வெறுமை )
{
    முழு iSignal;
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

    போது ( !xSchedulerEnd )
    {
        sigwait( &xSignals, &iSignal );
    }

    /* Cancel the Idle task and free its resources */
#எனில் ( INCLUDE_xTaskGetIdleTaskHandle == 1 )
    vPortCancelThread( xTaskGetIdleTaskHandle() );
#endif

#எனில் ( configUSE_TIMERS == 1 )
    /* Cancel the Timer task and free its resources */
    vPortCancelThread( xTimerGetTimerDaemonTaskHandle() );
#endif /* configUSE_TIMERS */

    /* Restore original signal mask. */
    (வெறுமை)pthread_sigmask( SIG_SETMASK, &xSchedulerOriginalSignalMask,  வெற்று );

    திரும்பு 0;
}
/*-----------------------------------------------------------*/

வெறுமை vPortEndScheduler( வெறுமை )
{
    கட்டமைப்பு itimerval itimer;
    கட்டமைப்பு sigaction sigtick;
    Thread_t *xCurrentThread;

    /* Stop the timer and ignore any pending SIGALRMs that would end
     * up running on the main thread when it is resumed. */
    itimer.it_value.tv_sec = 0;
    itimer.it_value.tv_usec = 0;

    itimer.it_interval.tv_sec = 0;
    itimer.it_interval.tv_usec = 0;
    (வெறுமை)setitimer( ITIMER_REAL, &itimer, வெற்று );

    sigtick.sa_flags = 0;
    sigtick.sa_handler = SIG_IGN;
    sigemptyset( &sigtick.sa_mask );
    sigaction( SIGALRM, &sigtick, வெற்று );

    /* Signal the scheduler to exit its loop. */
    xSchedulerEnd = pdTRUE;
    (வெறுமை)pthread_kill( hMainThread, SIG_RESUME );

    xCurrentThread = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );
    prvSuspendSelf(xCurrentThread);
}
/*-----------------------------------------------------------*/

நிலையான வெறுமை vPortDisableInterrupts( வெறுமை )
{
    pthread_sigmask( SIG_BLOCK, &xAllSignals, வெற்று );
}
/*-----------------------------------------------------------*/

நிலையான வெறுமை vPortEnableInterrupts( வெறுமை )
{
    pthread_sigmask( SIG_UNBLOCK, &xAllSignals, வெற்று );
}
/*-----------------------------------------------------------*/

வெறுமை vPortEnterCriticalIDF( வெறுமை )
{
    எனில் ( uxCriticalNestingIDF == 0 && uxInterruptLevel == 0)
    {
        vPortDisableInterrupts();
    }
    uxCriticalNestingIDF++;
}
/*-----------------------------------------------------------*/

வெறுமை vPortExitCriticalIDF( வெறுமை )
{
    uxCriticalNestingIDF--;

    /* If we have reached 0 then re-enable the interrupts. */
    எனில்( uxCriticalNestingIDF == 0 && uxInterruptLevel == 0)
    {
        vPortEnableInterrupts();
    }
}
/*-----------------------------------------------------------*/

வெறுமை vPortYieldFromISR( வெறுமை )
{
    Thread_t *xThreadToSuspend;
    Thread_t *xThreadToResume;

    xThreadToSuspend = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    vTaskSwitchContext();

    xThreadToResume = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    prvSwitchThread( xThreadToResume, xThreadToSuspend );
}
/*-----------------------------------------------------------*/

வெறுமை vPortYield( வெறுமை )
{
    BaseType_t prev_intr_level = xPortSetInterruptMask();

    vPortYieldFromISR();

    vPortClearInterruptMask( prev_intr_level );
}
/*-----------------------------------------------------------*/

/* In SMP code, the disable/enable interrupt macros are calling the set/get interrupt mask functions below.
   Hence, we need to call vPortDisableInterrupts() and vPortEnableInterrupts(), otherwise interrupts
   are never disabled/enabled. */

BaseType_t xPortSetInterruptMask( வெறுமை )
{
    எனில் (uxInterruptLevel == 0 && uxCriticalNestingIDF == 0) {
        vPortDisableInterrupts();
    }
    BaseType_t prev_intr_level = uxInterruptLevel;
    uxInterruptLevel++;
    திரும்பு prev_intr_level;
}
/*-----------------------------------------------------------*/

வெறுமை vPortClearInterruptMask( BaseType_t xMask )
{
    // Only re-enable interrupts if xMask is 0
    uxInterruptLevel = xMask;
    எனில் (uxInterruptLevel == 0 && uxCriticalNestingIDF == 0) {
        vPortEnableInterrupts();
    }
}
/*-----------------------------------------------------------*/

நிலையான uint64_t prvGetTimeNs(வெறுமை)
{
    கட்டமைப்பு timespec t;

    clock_gettime(CLOCK_MONOTONIC, &t);

    திரும்பு t.tv_sec * 1000000000ull + t.tv_nsec;
}

நிலையான uint64_t prvStartTimeNs;
/* commented as part of the code below in vPortSystemTickHandler,
 * to adjust timing according to full demo requirements */
/* static uint64_t prvTickCount; */

/*
 * Setup the systick timer to generate the tick interrupts at the required
 * frequency.
 */
வெறுமை prvSetupTimerInterrupt( வெறுமை )
{
    கட்டமைப்பு itimerval itimer;
    முழு iRet;

    /* Initialise the structure with the current timer information. */
    iRet = getitimer( ITIMER_REAL, &itimer );
    எனில் ( iRet )
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
    iRet = setitimer( ITIMER_REAL, &itimer, வெற்று );
    எனில் ( iRet )
    {
        prvFatalError( "setitimer", errno );
    }

    prvStartTimeNs = prvGetTimeNs();
}
/*-----------------------------------------------------------*/

நிலையான வெறுமை vPortSystemTickHandler( முழு sig )
{
    Thread_t *pxThreadToSuspend;
    Thread_t *pxThreadToResume;
    BaseType_t xSwitchRequired;
    /* uint64_t xExpectedTicks; */

    // Handling a timer signal, so we are currently in an interrupt.
    uxInterruptNesting++;

#எனில் ( configUSE_PREEMPTION == 1 )
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

#எனில் ( configUSE_PREEMPTION == 1 )
    எனில் (xSwitchRequired == pdTRUE) {
        /* Select Next Task. */
        vTaskSwitchContext();

        pxThreadToResume = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

        prvSwitchThread(pxThreadToResume, pxThreadToSuspend);
    }
#இல்லையெனில்
    (வெறுமை)xSwitchRequired;
#endif

    // Returning from the timer signal handler, so we are exiting the interrupt.
    uxInterruptNesting--;
}
/*-----------------------------------------------------------*/

வெறுமை vPortThreadDying( வெறுமை *pxTaskToDelete, நிலையற்ற BaseType_t *pxPendYield )
{
    Thread_t *pxThread = prvGetThreadFromTask( pxTaskToDelete );

    pxThread->xDying = pdTRUE;
}

வெறுமை vPortCancelThread( வெறுமை *pxTaskToDelete )
{
    Thread_t *pxThreadToCancel = prvGetThreadFromTask( pxTaskToDelete );

    /*
     * The thread has already been suspended so it can be safely cancelled.
     */
    pthread_cancel( pxThreadToCancel->pthread );
    pthread_join( pxThreadToCancel->pthread, வெற்று );
    event_delete( pxThreadToCancel->ev );
}
/*-----------------------------------------------------------*/

நிலையான வெறுமை *prvWaitForStart( வெறுமை * pvParams )
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

    திரும்பு வெற்று;
}
/*-----------------------------------------------------------*/

நிலையான வெறுமை prvSwitchThread( Thread_t *pxThreadToResume,
                             Thread_t *pxThreadToSuspend )
{
    BaseType_t uxSavedCriticalNestingIDF;
    BaseType_t uxSavedInterruptNesting;
    BaseType_t uxSavedInterruptLevel;

    எனில் ( pxThreadToSuspend != pxThreadToResume )
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
        எனில் ( pxThreadToSuspend->xDying )
        {
            pthread_exit( வெற்று );
        }
        prvSuspendSelf( pxThreadToSuspend );

        uxCriticalNestingIDF = uxSavedCriticalNestingIDF;
        uxInterruptNesting = uxSavedInterruptNesting;
        uxInterruptLevel = uxSavedInterruptLevel;
    }
}
/*-----------------------------------------------------------*/

நிலையான வெறுமை prvSuspendSelf( Thread_t *thread )
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

நிலையான வெறுமை prvResumeThread( Thread_t *xThreadId )
{
    எனில் ( pthread_self() != xThreadId->pthread )
    {
        event_signal(xThreadId->ev);
    }
}
/*-----------------------------------------------------------*/

நிலையான வெறுமை prvSetupSignalsAndSchedulerPolicy( வெறுமை )
{
    கட்டமைப்பு sigaction sigresume, sigtick;
    முழு iRet;

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
    (வெறுமை)pthread_sigmask( SIG_SETMASK, &xAllSignals,
                           &xSchedulerOriginalSignalMask );

    /* SIG_RESUME is only used with sigwait() so doesn't need a
       handler. */
    sigresume.sa_flags = 0;
    sigresume.sa_handler = SIG_IGN;
    sigfillset( &sigresume.sa_mask );

    sigtick.sa_flags = 0;
    sigtick.sa_handler = vPortSystemTickHandler;
    sigfillset( &sigtick.sa_mask );

    iRet = sigaction( SIG_RESUME, &sigresume, வெற்று );
    எனில் ( iRet )
    {
        prvFatalError( "sigaction", errno );
    }

    iRet = sigaction( SIGALRM, &sigtick, வெற்று );
    எனில் ( iRet )
    {
        prvFatalError( "sigaction", errno );
    }
}
/*-----------------------------------------------------------*/

unsigned long ulPortGetRunTime( வெறுமை )
{
    கட்டமைப்பு tms xTimes;

    times( &xTimes );

    திரும்பு ( unsigned long ) xTimes.tms_utime;
}
/*-----------------------------------------------------------*/
bool portVALID_LIST_MEM(மாறிலி வெறுமை *ptr)
{
    திரும்பு true;
}

bool portVALID_TCB_MEM(மாறிலி வெறுமை *ptr)
{
    திரும்பு true;
}

bool port_VALID_அடுக்கு_MEM(மாறிலி வெறுமை *ptr)
{
    திரும்பு true;
}
/*-----------------------------------------------------------*/

portMUX_TYPE port_xTaskLock = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE port_xISRLock = portMUX_INITIALIZER_UNLOCKED;

நிலையான மாறிலி எழுத்து *TAG = "port";

BaseType_t xPortCheckIfInISR(வெறுமை)
{
    திரும்பு (uxInterruptNesting == 0) ? pdFALSE : pdTRUE;
}

வெறுமை app_main(வெறுமை);

நிலையான வெறுமை main_task(வெறுமை* args)
{
    app_main();
    vTaskDelete(வெற்று);
}

முழு முதன்மை(முழு argc, மாறிலி எழுத்து **argv)
{
    // This makes sure that stdio is flushed after each '\n' so that idf.py monitor
    // reads the program output on time.
    setvbuf(stdout, வெற்று, _IOLBF, 0);

    usleep(1000);

    BaseType_t res;

#எனில் ( config_இலக்கம்_CORES > 1 )
    res = xTaskCreateAffinitySet(&main_task, "main",
                                 ESP_TASK_MAIN_அடுக்கு, வெற்று,
                                 ESP_TASK_MAIN_PRIO, ESP_TASK_MAIN_CORE, வெற்று);
#இல்லையெனில்
    res = xTaskCreate(&main_task, "main",
                      ESP_TASK_MAIN_அடுக்கு, வெற்று,
                      ESP_TASK_MAIN_PRIO, வெற்று);
#endif

    ப்ரதிஜ்ஞா(res == pdTRUE);
    (வெறுமை)res;

    ESP_LOGI(TAG, "Starting SMP scheduler.");
    vTaskStartScheduler();

    // This line should never be reached
    ப்ரதிஜ்ஞா(false);
}

வெறுமை esp_vApplicationIdleHook(வெறுமை)
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

வெறுமை esp_vApplicationTickHook( வெறுமை ) { }

#எனில்  (  configUSE_TICK_HOOK > 0 )
வெறுமை vApplicationTickHook( வெறுமை )
{
    esp_vApplicationTickHook();
}
#endif

#எனில் ( configSUPPORT_STATIC_ALLOCATION == 1 )
/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
 * implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
 * used by the Idle task. */
வெறுமை vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                    அடுக்கு_Type_t ** ppx_Idle_Task_அடுக்கு_Buffer,
                                    uint32_t * pul_Idle_Task_அடுக்கு_Size )
{
/* If the buffers to be provided to the Idle task are declared inside this
 * function then they must be declared static - otherwise they will be allocated on
 * the stack and so not exists after this function exits. */
    நிலையான StaticTask_t xIdleTaskTCB;
    நிலையான அடுக்கு_Type_t ux_Idle_Task_அடுக்கு[ config_MINIMAL_அடுக்கு_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
     * state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppx_Idle_Task_அடுக்கு_Buffer = ux_Idle_Task_அடுக்கு;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in bytes. */
    *pul_Idle_Task_அடுக்கு_Size = config_MINIMAL_அடுக்கு_SIZE;
}
#endif // configSUPPORT_STATIC_ALLOCATION == 1
/*-----------------------------------------------------------*/

#எனில் ( (configSUPPORT_STATIC_ALLOCATION == 1) && (configUSE_TIMERS == 1))

/* When configSUPPORT_STATIC_ALLOCATION is set to 1 the application writer can
 * use a callback function to optionally provide the memory required by the idle
 * and timer tasks.  This is the stack that will be used by the timer task.  It is
 * declared here, as a global, so it can be checked by a test that is implemented
 * in a different file. */
அடுக்கு_Type_t ux_Timer_Task_அடுக்கு[ config_TIMER_TASK_அடுக்கு_DEPTH ];


/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
 * application must provide an implementation of vApplicationGetTimerTaskMemory()
 * to provide the memory that is used by the Timer service task. */
வெறுமை vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                     அடுக்கு_Type_t ** ppx_Timer_Task_அடுக்கு_Buffer,
                                     uint32_t * pul_Timer_Task_அடுக்கு_Size )
{
/* If the buffers to be provided to the Timer task are declared inside this
 * function then they must be declared static - otherwise they will be allocated on
 * the stack and so not exists after this function exits. */
    நிலையான StaticTask_t xTimerTaskTCB;

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
     * task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppx_Timer_Task_அடுக்கு_Buffer = ux_Timer_Task_அடுக்கு;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in bytes. */
    *pul_Timer_Task_அடுக்கு_Size = config_TIMER_TASK_அடுக்கு_DEPTH;
}
#endif // configSUPPORT_STATIC_ALLOCATION == 1 && (configUSE_TIMERS == 1)

வெறுமை vPortTakeLock( portMUX_TYPE *lock )
{
    spinlock_acquire( lock, portMUX_NO_TIMEOUT);
}

வெறுமை vPortReleaseLock( portMUX_TYPE *lock )
{
    spinlock_release( lock );
}

#define FREERTOS_SMP_MALLOC_CAPS    (MALLOC_CAP_INTERNAL|MALLOC_CAP_8BIT)

வெறுமை *pvPortMalloc( size_t xSize )
{
    திரும்பு heap_caps_malloc(xSize, FREERTOS_SMP_MALLOC_CAPS);
}

வெறுமை vPortFree( வெறுமை *pv )
{
    heap_caps_free(pv);
}

வெறுமை __attribute__((weak)) v_Application_அடுக்கு_Overflow_Hook(TaskHandle_t xTask, எழுத்து *pcTaskName)
{
#define ERR_STR1 "***ERROR*** A stack overflow in task "
#define ERR_STR2 " has been detected."
    மாறிலி எழுத்து *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    எழுத்து buf[அளவு(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + அளவு(ERR_STR2) + 1 /* null char */] = {0};

    எழுத்து *dest = buf;
    க்கு (முழு i = 0; i < அளவு(str) / அளவு(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
	printf("%s\n", buf);
    abort();
}

// ------- Thread Local Storage Pointers Deletion Callbacks -------

#எனில் ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
வெறுமை vPortTLSPointersDelCb( வெறுமை *pxTCB )
{
    /* Typecast pxTCB to StaticTask_t type to access TCB struct members.
     * pvDummy15 corresponds to pvThreadLocalStoragePointers member of the TCB.
     */
    StaticTask_t *tcb = ( StaticTask_t * )pxTCB;

    /* The TLSP deletion callbacks are stored at an offset of (configNUM_THREAD_LOCAL_STORAGE_POINTERS/2) */
    TlsDeleteCallbackFunction_t *pvThreadLocalStoragePointersDelCallback = ( TlsDeleteCallbackFunction_t * )( &( tcb->pvDummy15[ ( config_இலக்கம்_THREAD_LOCAL_STORAGE_POINTERS / 2 ) ] ) );

    /* We need to iterate over half the depth of the pvThreadLocalStoragePointers area
     * to access all TLS pointers and their respective TLS deletion callbacks.
     */
    க்கு ( முழு x = 0; x < ( config_இலக்கம்_THREAD_LOCAL_STORAGE_POINTERS / 2 ); x++ ) {
        எனில் ( pvThreadLocalStoragePointersDelCallback[ x ] != வெற்று ) {  //If del cb is set
            // We skip the check if the callback is executable as that is difficult to determine for different
            // platforms (compare xtensa and riscv code).
            pvThreadLocalStoragePointersDelCallback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#endif // CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS

வெறுமை vPortCleanUpTCB ( வெறுமை *pxTCB )
{
    #எனில் ( CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK )
        /* Call the user defined task pre-deletion hook */
        வெளி வெறுமை vTaskPreDeletionHook( வெறுமை * pxTCB );
        vTaskPreDeletionHook( pxTCB );
    #endif /* CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK */

#எனில் ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
    /* Call TLS pointers deletion callbacks */
    vPortTLSPointersDelCb( pxTCB );
#endif /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

    vPortCancelThread(pxTCB);
}
