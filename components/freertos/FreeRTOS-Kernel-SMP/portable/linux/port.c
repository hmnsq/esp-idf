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

#समावेश <errno.h>
#समावेश <pthread.h>
#समावेश <signal.h>
#समावेश <stdio.h>
#समावेश <stdlib.h>
#समावेश <string.h>
#समावेश <sys/time.h>
#समावेश <sys/times.h>
#समावेश <time.h>
#समावेश <unistd.h>
#समावेश <assert.h>

/* Scheduler includes. */
#समावेश "esp_heap_caps.h"
#समावेश "FreeRTOS.h"
#समावेश "task.h"
#समावेश "esp_task.h"
#समावेश "timers.h"
#समावेश "utils/wait_for_event.h"
#समावेश "esp_log.h"
/*-----------------------------------------------------------*/

#घोषणा SIG_RESUME SIGUSR1

प्रकार काष्ठा THREAD
{
    pthread_t pthread;
    TaskFunction_t pxCode;
    व्योम *pvParams;
    BaseType_t xDying;
    काष्ठा event *ev;
} Thread_t;

/*
 * The additional per-thread data is stored at the beginning of the
 * task's stack.
 */
अटल अंतरभूत Thread_t *prvGetThreadFromTask(TaskHandle_t xTask)
{
    ढेर_Type_t *px_शीर्ष_Of_ढेर = *(ढेर_Type_t **)xTask;

    वापस (Thread_t *)(px_शीर्ष_Of_ढेर + 1);
}

/*-----------------------------------------------------------*/

अटल pthread_once_t hSigSetupThread = PTHREAD_ONCE_INIT;
अटल sigset_t xAllSignals;
अटल sigset_t xSchedulerOriginalSignalMask;
अटल pthread_t hMainThread = ( pthread_t )शून्य;

// These are saved as part of a thread's state in prvSwitchThread()
अटल अस्थिर BaseType_t uxCriticalNestingIDF = 0;    /* Track nesting calls for IDF style critical sections. FreeRTOS critical section nesting is maintained in the TCB. */
अटल अस्थिर UBaseType_t uxInterruptNesting = 0;     /* Tracks if we are currently in an interrupt. */
अटल अस्थिर BaseType_t uxInterruptLevel = 0;        /* Tracks the current level (i.e., interrupt mask) */
/*-----------------------------------------------------------*/

अटल BaseType_t xSchedulerEnd = pdFALSE;
/*-----------------------------------------------------------*/

अटल व्योम prvSetupSignalsAndSchedulerPolicy( व्योम );
अटल व्योम prvSetupTimerInterrupt( व्योम );
अटल व्योम *prvWaitForStart( व्योम * pvParams );
अटल व्योम prvSwitchThread( Thread_t * xThreadToResume,
                             Thread_t *xThreadToSuspend );
अटल व्योम prvSuspendSelf( Thread_t * thread);
अटल व्योम prvResumeThread( Thread_t * xThreadId );
अटल व्योम vPortSystemTickHandler( पूर्णांक sig );
अटल व्योम vPortStartFirstTask( व्योम );
/*-----------------------------------------------------------*/

अटल व्योम prvFatalError( स्थिर अक्षर *pcCall, पूर्णांक iErrno )
{
    ख_लिखो( मानक_त्रुटि, "%s: %s\n", pcCall, म_त्रुटि( iErrno ) );
    पात();
}

/*
 * See header file for description.
 */
ढेर_Type_t *px_Port_Initialise_ढेर( ढेर_Type_t *px_शीर्ष_Of_ढेर,
                                    ढेर_Type_t *px_End_Of_ढेर,
                                    TaskFunction_t pxCode,
                                    व्योम *pvParameters )
{
    Thread_t *thread;
    pthread_attr_t xThreadAttributes;
    माप_प्रकार ul_ढेर_आकार;
    पूर्णांक iRet;

    (व्योम)pthread_once( &hSigSetupThread, prvSetupSignalsAndSchedulerPolicy );

    /*
     * Store the additional thread data at the start of the stack.
     */
    thread = (Thread_t *)(px_शीर्ष_Of_ढेर + 1) - 1;
    px_शीर्ष_Of_ढेर = (ढेर_Type_t *)thread - 1;
    ul_ढेर_आकार = (px_शीर्ष_Of_ढेर + 1 - px_End_Of_ढेर) * माप(*px_शीर्ष_Of_ढेर);

    thread->pxCode = pxCode;
    thread->pvParams = pvParameters;
    thread->xDying = pdFALSE;

    pthread_attr_init( &xThreadAttributes );
    pthread_attr_setstack( &xThreadAttributes, px_End_Of_ढेर, ul_ढेर_आकार );

    thread->ev = event_create();

    BaseType_t पूर्व_intr_level = xPortSetInterruptMask();

    iRet = pthread_create( &thread->pthread, &xThreadAttributes,
                           prvWaitForStart, thread );
    अगर ( iRet )
    {
        prvFatalError( "pthread_create", iRet );
    }

    vPortClearInterruptMask( पूर्व_intr_level );

    वापस px_शीर्ष_Of_ढेर;
}
/*-----------------------------------------------------------*/

व्योम vPortStartFirstTask( व्योम )
{
    Thread_t *pxFirstThread = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    /* Start the first task. */
    prvResumeThread( pxFirstThread );
}
/*-----------------------------------------------------------*/

/*
 * See header file for description.
 */
BaseType_t xPortStartScheduler( व्योम )
{
    पूर्णांक iSignal;
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

    जबतक ( !xSchedulerEnd )
    {
        sigwait( &xSignals, &iSignal );
    }

    /* Cancel the Idle task and free its resources */
#अगर ( INCLUDE_xTaskGetIdleTaskHandle == 1 )
    vPortCancelThread( xTaskGetIdleTaskHandle() );
#पूर्ण_अगर

#अगर ( configUSE_TIMERS == 1 )
    /* Cancel the Timer task and free its resources */
    vPortCancelThread( xTimerGetTimerDaemonTaskHandle() );
#पूर्ण_अगर /* configUSE_TIMERS */

    /* Restore original signal mask. */
    (व्योम)pthread_sigmask( SIG_SETMASK, &xSchedulerOriginalSignalMask,  शून्य );

    वापस 0;
}
/*-----------------------------------------------------------*/

व्योम vPortEndScheduler( व्योम )
{
    काष्ठा itimerval itimer;
    काष्ठा sigaction sigtick;
    Thread_t *xCurrentThread;

    /* Stop the timer and ignore any pending SIGALRMs that would end
     * up running on the main thread when it is resumed. */
    itimer.it_value.tv_sec = 0;
    itimer.it_value.tv_usec = 0;

    itimer.it_interval.tv_sec = 0;
    itimer.it_interval.tv_usec = 0;
    (व्योम)setitimer( ITIMER_REAL, &itimer, शून्य );

    sigtick.sa_flags = 0;
    sigtick.sa_handler = संक_छोड़ो;
    sigemptyset( &sigtick.sa_mask );
    sigaction( SIGALRM, &sigtick, शून्य );

    /* Signal the scheduler to exit its loop. */
    xSchedulerEnd = pdTRUE;
    (व्योम)pthread_kill( hMainThread, SIG_RESUME );

    xCurrentThread = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );
    prvSuspendSelf(xCurrentThread);
}
/*-----------------------------------------------------------*/

अटल व्योम vPortDisableInterrupts( व्योम )
{
    pthread_sigmask( SIG_BLOCK, &xAllSignals, शून्य );
}
/*-----------------------------------------------------------*/

अटल व्योम vPortEnableInterrupts( व्योम )
{
    pthread_sigmask( SIG_UNBLOCK, &xAllSignals, शून्य );
}
/*-----------------------------------------------------------*/

व्योम vPortEnterCriticalIDF( व्योम )
{
    अगर ( uxCriticalNestingIDF == 0 && uxInterruptLevel == 0)
    {
        vPortDisableInterrupts();
    }
    uxCriticalNestingIDF++;
}
/*-----------------------------------------------------------*/

व्योम vPortExitCriticalIDF( व्योम )
{
    uxCriticalNestingIDF--;

    /* If we have reached 0 then re-enable the interrupts. */
    अगर( uxCriticalNestingIDF == 0 && uxInterruptLevel == 0)
    {
        vPortEnableInterrupts();
    }
}
/*-----------------------------------------------------------*/

व्योम vPortYieldFromISR( व्योम )
{
    Thread_t *xThreadToSuspend;
    Thread_t *xThreadToResume;

    xThreadToSuspend = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    vTaskSwitchContext();

    xThreadToResume = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    prvSwitchThread( xThreadToResume, xThreadToSuspend );
}
/*-----------------------------------------------------------*/

व्योम vPortYield( व्योम )
{
    BaseType_t पूर्व_intr_level = xPortSetInterruptMask();

    vPortYieldFromISR();

    vPortClearInterruptMask( पूर्व_intr_level );
}
/*-----------------------------------------------------------*/

/* In SMP code, the disable/enable interrupt macros are calling the set/get interrupt mask functions below.
   Hence, we need to call vPortDisableInterrupts() and vPortEnableInterrupts(), otherwise interrupts
   are never disabled/enabled. */

BaseType_t xPortSetInterruptMask( व्योम )
{
    अगर (uxInterruptLevel == 0 && uxCriticalNestingIDF == 0) {
        vPortDisableInterrupts();
    }
    BaseType_t पूर्व_intr_level = uxInterruptLevel;
    uxInterruptLevel++;
    वापस पूर्व_intr_level;
}
/*-----------------------------------------------------------*/

व्योम vPortClearInterruptMask( BaseType_t xMask )
{
    // Only re-enable interrupts if xMask is 0
    uxInterruptLevel = xMask;
    अगर (uxInterruptLevel == 0 && uxCriticalNestingIDF == 0) {
        vPortEnableInterrupts();
    }
}
/*-----------------------------------------------------------*/

अटल uint64_t prvGetTimeNs(व्योम)
{
    काष्ठा timespec t;

    clock_gettime(CLOCK_MONOTONIC, &t);

    वापस t.tv_sec * 1000000000ull + t.tv_nsec;
}

अटल uint64_t prvStartTimeNs;
/* commented as part of the code below in vPortSystemTickHandler,
 * to adjust timing according to full demo requirements */
/* static uint64_t prvTickCount; */

/*
 * Setup the systick timer to generate the tick interrupts at the required
 * frequency.
 */
व्योम prvSetupTimerInterrupt( व्योम )
{
    काष्ठा itimerval itimer;
    पूर्णांक iRet;

    /* Initialise the structure with the current timer information. */
    iRet = getitimer( ITIMER_REAL, &itimer );
    अगर ( iRet )
    {
        prvFatalError( "getitimer", त्रुटि_सं );
    }

    /* Set the interval between timer events. */
    itimer.it_interval.tv_sec = 0;
    itimer.it_interval.tv_usec = portTICK_RATE_MICROSECONDS;

    /* Set the current count-down. */
    itimer.it_value.tv_sec = 0;
    itimer.it_value.tv_usec = portTICK_RATE_MICROSECONDS;

    /* Set-up the timer interrupt. */
    iRet = setitimer( ITIMER_REAL, &itimer, शून्य );
    अगर ( iRet )
    {
        prvFatalError( "setitimer", त्रुटि_सं );
    }

    prvStartTimeNs = prvGetTimeNs();
}
/*-----------------------------------------------------------*/

अटल व्योम vPortSystemTickHandler( पूर्णांक sig )
{
    Thread_t *pxThreadToSuspend;
    Thread_t *pxThreadToResume;
    BaseType_t xSwitchRequired;
    /* uint64_t xExpectedTicks; */

    // Handling a timer signal, so we are currently in an interrupt.
    uxInterruptNesting++;

#अगर ( configUSE_PREEMPTION == 1 )
    pxThreadToSuspend = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );
#पूर्ण_अगर

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

#अगर ( configUSE_PREEMPTION == 1 )
    अगर (xSwitchRequired == pdTRUE) {
        /* Select Next Task. */
        vTaskSwitchContext();

        pxThreadToResume = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

        prvSwitchThread(pxThreadToResume, pxThreadToSuspend);
    }
#अन्यथा
    (व्योम)xSwitchRequired;
#पूर्ण_अगर

    // Returning from the timer signal handler, so we are exiting the interrupt.
    uxInterruptNesting--;
}
/*-----------------------------------------------------------*/

व्योम vPortThreadDying( व्योम *pxTaskToDelete, अस्थिर BaseType_t *pxPendYield )
{
    Thread_t *pxThread = prvGetThreadFromTask( pxTaskToDelete );

    pxThread->xDying = pdTRUE;
}

व्योम vPortCancelThread( व्योम *pxTaskToDelete )
{
    Thread_t *pxThreadToCancel = prvGetThreadFromTask( pxTaskToDelete );

    /*
     * The thread has already been suspended so it can be safely cancelled.
     */
    pthread_cancel( pxThreadToCancel->pthread );
    pthread_join( pxThreadToCancel->pthread, शून्य );
    event_delete( pxThreadToCancel->ev );
}
/*-----------------------------------------------------------*/

अटल व्योम *prvWaitForStart( व्योम * pvParams )
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

    वापस शून्य;
}
/*-----------------------------------------------------------*/

अटल व्योम prvSwitchThread( Thread_t *pxThreadToResume,
                             Thread_t *pxThreadToSuspend )
{
    BaseType_t uxSavedCriticalNestingIDF;
    BaseType_t uxSavedInterruptNesting;
    BaseType_t uxSavedInterruptLevel;

    अगर ( pxThreadToSuspend != pxThreadToResume )
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
        अगर ( pxThreadToSuspend->xDying )
        {
            pthread_exit( शून्य );
        }
        prvSuspendSelf( pxThreadToSuspend );

        uxCriticalNestingIDF = uxSavedCriticalNestingIDF;
        uxInterruptNesting = uxSavedInterruptNesting;
        uxInterruptLevel = uxSavedInterruptLevel;
    }
}
/*-----------------------------------------------------------*/

अटल व्योम prvSuspendSelf( Thread_t *thread )
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

अटल व्योम prvResumeThread( Thread_t *xThreadId )
{
    अगर ( pthread_self() != xThreadId->pthread )
    {
        event_signal(xThreadId->ev);
    }
}
/*-----------------------------------------------------------*/

अटल व्योम prvSetupSignalsAndSchedulerPolicy( व्योम )
{
    काष्ठा sigaction sigresume, sigtick;
    पूर्णांक iRet;

    hMainThread = pthread_self();

    /* Initialise common signal masks. */
    sigfillset( &xAllSignals );
    /* Don't block SIGINT so this can be used to break into GDB while
     * in a critical section. */
    sigdelset( &xAllSignals, संक_विघ्न );

    /*
     * Block all signals in this thread so all new threads
     * inherits this mask.
     *
     * When a thread is resumed for the first time, all signals
     * will be unblocked.
     */
    (व्योम)pthread_sigmask( SIG_SETMASK, &xAllSignals,
                           &xSchedulerOriginalSignalMask );

    /* SIG_RESUME is only used with sigwait() so doesn't need a
       handler. */
    sigresume.sa_flags = 0;
    sigresume.sa_handler = संक_छोड़ो;
    sigfillset( &sigresume.sa_mask );

    sigtick.sa_flags = 0;
    sigtick.sa_handler = vPortSystemTickHandler;
    sigfillset( &sigtick.sa_mask );

    iRet = sigaction( SIG_RESUME, &sigresume, शून्य );
    अगर ( iRet )
    {
        prvFatalError( "sigaction", त्रुटि_सं );
    }

    iRet = sigaction( SIGALRM, &sigtick, शून्य );
    अगर ( iRet )
    {
        prvFatalError( "sigaction", त्रुटि_सं );
    }
}
/*-----------------------------------------------------------*/

अचिन्हित दीर्घ ulPortGetRunTime( व्योम )
{
    काष्ठा tms xTimes;

    बार( &xTimes );

    वापस ( अचिन्हित दीर्घ ) xTimes.tms_utime;
}
/*-----------------------------------------------------------*/
bool portVALID_LIST_MEM(स्थिर व्योम *ptr)
{
    वापस true;
}

bool portVALID_TCB_MEM(स्थिर व्योम *ptr)
{
    वापस true;
}

bool port_VALID_ढेर_MEM(स्थिर व्योम *ptr)
{
    वापस true;
}
/*-----------------------------------------------------------*/

portMUX_TYPE port_xTaskLock = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE port_xISRLock = portMUX_INITIALIZER_UNLOCKED;

अटल स्थिर अक्षर *TAG = "port";

BaseType_t xPortCheckIfInISR(व्योम)
{
    वापस (uxInterruptNesting == 0) ? pdFALSE : pdTRUE;
}

व्योम app_main(व्योम);

अटल व्योम main_task(व्योम* args)
{
    app_main();
    vTaskDelete(शून्य);
}

पूर्णांक मुख्य(पूर्णांक argc, स्थिर अक्षर **argv)
{
    // This makes sure that stdio is flushed after each '\n' so that idf.py monitor
    // reads the program output on time.
    रखो_भबफ(मानक_निकास, शून्य, _IOLBF, 0);

    usleep(1000);

    BaseType_t res;

#अगर ( config_अंक_CORES > 1 )
    res = xTaskCreateAffinitySet(&main_task, "main",
                                 ESP_TASK_MAIN_ढेर, शून्य,
                                 ESP_TASK_MAIN_PRIO, ESP_TASK_MAIN_CORE, शून्य);
#अन्यथा
    res = xTaskCreate(&main_task, "main",
                      ESP_TASK_MAIN_ढेर, शून्य,
                      ESP_TASK_MAIN_PRIO, शून्य);
#पूर्ण_अगर

    निश्चित(res == pdTRUE);
    (व्योम)res;

    ESP_LOGI(TAG, "Starting SMP scheduler.");
    vTaskStartScheduler();

    // This line should never be reached
    निश्चित(false);
}

व्योम esp_vApplicationIdleHook(व्योम)
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

व्योम esp_vApplicationTickHook( व्योम ) { }

#अगर  (  configUSE_TICK_HOOK > 0 )
व्योम vApplicationTickHook( व्योम )
{
    esp_vApplicationTickHook();
}
#पूर्ण_अगर

#अगर ( configSUPPORT_STATIC_ALLOCATION == 1 )
/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
 * implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
 * used by the Idle task. */
व्योम vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                    ढेर_Type_t ** ppx_Idle_Task_ढेर_बफर,
                                    uint32_t * pul_Idle_Task_ढेर_आकार )
{
/* If the buffers to be provided to the Idle task are declared inside this
 * function then they must be declared static - otherwise they will be allocated on
 * the stack and so not exists after this function exits. */
    अटल StaticTask_t xIdleTaskTCB;
    अटल ढेर_Type_t ux_Idle_Task_ढेर[ config_MINIMAL_ढेर_आकार ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
     * state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppx_Idle_Task_ढेर_बफर = ux_Idle_Task_ढेर;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in bytes. */
    *pul_Idle_Task_ढेर_आकार = config_MINIMAL_ढेर_आकार;
}
#पूर्ण_अगर // configSUPPORT_STATIC_ALLOCATION == 1
/*-----------------------------------------------------------*/

#अगर ( (configSUPPORT_STATIC_ALLOCATION == 1) && (configUSE_TIMERS == 1))

/* When configSUPPORT_STATIC_ALLOCATION is set to 1 the application writer can
 * use a callback function to optionally provide the memory required by the idle
 * and timer tasks.  This is the stack that will be used by the timer task.  It is
 * declared here, as a global, so it can be checked by a test that is implemented
 * in a different file. */
ढेर_Type_t ux_Timer_Task_ढेर[ config_TIMER_TASK_ढेर_DEPTH ];


/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
 * application must provide an implementation of vApplicationGetTimerTaskMemory()
 * to provide the memory that is used by the Timer service task. */
व्योम vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                     ढेर_Type_t ** ppx_Timer_Task_ढेर_बफर,
                                     uint32_t * pul_Timer_Task_ढेर_आकार )
{
/* If the buffers to be provided to the Timer task are declared inside this
 * function then they must be declared static - otherwise they will be allocated on
 * the stack and so not exists after this function exits. */
    अटल StaticTask_t xTimerTaskTCB;

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
     * task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppx_Timer_Task_ढेर_बफर = ux_Timer_Task_ढेर;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in bytes. */
    *pul_Timer_Task_ढेर_आकार = config_TIMER_TASK_ढेर_DEPTH;
}
#पूर्ण_अगर // configSUPPORT_STATIC_ALLOCATION == 1 && (configUSE_TIMERS == 1)

व्योम vPortTakeLock( portMUX_TYPE *lock )
{
    spinlock_acquire( lock, portMUX_NO_TIMEOUT);
}

व्योम vPortReleaseLock( portMUX_TYPE *lock )
{
    spinlock_release( lock );
}

#घोषणा FREERTOS_SMP_MALLOC_CAPS    (MALLOC_CAP_INTERNAL|MALLOC_CAP_8BIT)

व्योम *pvPortMalloc( माप_प्रकार x_आकार )
{
    वापस heap_caps_malloc(x_आकार, FREERTOS_SMP_MALLOC_CAPS);
}

व्योम vPortFree( व्योम *pv )
{
    heap_caps_free(pv);
}

व्योम __attribute__((weak)) v_Application_ढेर_Overflow_Hook(TaskHandle_t xTask, अक्षर *pcTaskName)
{
#घोषणा ERR_STR1 "***ERROR*** A stack overflow in task "
#घोषणा ERR_STR2 " has been detected."
    स्थिर अक्षर *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    अक्षर buf[माप(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + माप(ERR_STR2) + 1 /* null char */] = {0};

    अक्षर *dest = buf;
    क्रम (पूर्णांक i = 0; i < माप(str) / माप(str[0]); i++) {
        dest = म_जोड़ो(dest, str[i]);
    }
	म_लिखो("%s\n", buf);
    पात();
}

// ------- Thread Local Storage Pointers Deletion Callbacks -------

#अगर ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
व्योम vPortTLSPointersDelCb( व्योम *pxTCB )
{
    /* Typecast pxTCB to StaticTask_t type to access TCB struct members.
     * pvDummy15 corresponds to pvThreadLocalStoragePointers member of the TCB.
     */
    StaticTask_t *tcb = ( StaticTask_t * )pxTCB;

    /* The TLSP deletion callbacks are stored at an offset of (configNUM_THREAD_LOCAL_STORAGE_POINTERS/2) */
    TlsDeleteCallbackFunction_t *pvThreadLocalStoragePointersDelCallback = ( TlsDeleteCallbackFunction_t * )( &( tcb->pvDummy15[ ( config_अंक_THREAD_LOCAL_STORAGE_POINTERS / 2 ) ] ) );

    /* We need to iterate over half the depth of the pvThreadLocalStoragePointers area
     * to access all TLS pointers and their respective TLS deletion callbacks.
     */
    क्रम ( पूर्णांक x = 0; x < ( config_अंक_THREAD_LOCAL_STORAGE_POINTERS / 2 ); x++ ) {
        अगर ( pvThreadLocalStoragePointersDelCallback[ x ] != शून्य ) {  //If del cb is set
            // We skip the check if the callback is executable as that is difficult to determine for different
            // platforms (compare xtensa and riscv code).
            pvThreadLocalStoragePointersDelCallback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#पूर्ण_अगर // CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS

व्योम vPortCleanUpTCB ( व्योम *pxTCB )
{
    #अगर ( CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK )
        /* Call the user defined task pre-deletion hook */
        बाह्य व्योम vTaskPreDeletionHook( व्योम * pxTCB );
        vTaskPreDeletionHook( pxTCB );
    #पूर्ण_अगर /* CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK */

#अगर ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
    /* Call TLS pointers deletion callbacks */
    vPortTLSPointersDelCb( pxTCB );
#पूर्ण_अगर /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

    vPortCancelThread(pxTCB);
}
