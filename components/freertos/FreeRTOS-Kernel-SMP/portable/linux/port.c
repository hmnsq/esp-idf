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

#شامل <errno.h>
#شامل <pthread.h>
#شامل <signal.h>
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <string.h>
#شامل <sys/time.h>
#شامل <sys/times.h>
#شامل <time.h>
#شامل <unistd.h>
#شامل <assert.h>

/* Scheduler includes. */
#شامل "esp_heap_caps.h"
#شامل "FreeRTOS.h"
#شامل "task.h"
#شامل "esp_task.h"
#شامل "timers.h"
#شامل "utils/wait_for_event.h"
#شامل "esp_log.h"
/*-----------------------------------------------------------*/

#تعریف_کرو SIG_RESUME SIGUSR1

قسمتعریف ساخت THREAD
{
    pthread_t pthread;
    TaskFunction_t px_رمز;
    خالی *pvParams;
    بنیاد_Type_t xDying;
    ساخت event *ev;
} Thread_t;

/*
 * The additional per-thread data is stored at the beginning of the
 * task's stack.
 */
ساکن اندرونی Thread_t *prvGetThreadFromTask(TaskHandle_t xTask)
{
    ڈھیر_Type_t *px_چوٹی_کا_ڈھیر = *(ڈھیر_Type_t **)xTask;

    واپس (Thread_t *)(px_چوٹی_کا_ڈھیر + 1);
}

/*-----------------------------------------------------------*/

ساکن pthread_once_t h_Sig_تیاری_Thread = PTHREAD_ONCE_INIT;
ساکن sigset_t x_All_اشارات;
ساکن sigset_t x_Scheduler_Original_Signal_نقاب;
ساکن pthread_t hMainThread = ( pthread_t )تہی;

// These are saved as part of a thread's state in prvSwitchThread()
ساکن متلون بنیاد_Type_t uxCriticalNestingIDF = 0;    /* Track nesting calls for IDF style critical sections. FreeRTOS critical section nesting is maintained in the TCB. */
ساکن متلون UBaseType_t ux_مداخلت_Nesting = 0;     /* Tracks if we are currently in an interrupt. */
ساکن متلون بنیاد_Type_t ux_مداخلت_Level = 0;        /* Tracks the current level (i.e., interrupt mask) */
/*-----------------------------------------------------------*/

ساکن بنیاد_Type_t x_Scheduler_انتہا = pdFALSE;
/*-----------------------------------------------------------*/

ساکن خالی prv_تیاری_اشارات_And_Scheduler_Policy( خالی );
ساکن خالی prv_تیاری_Timer_مداخلت( خالی );
ساکن خالی *prv_Wait_For_آغاز( خالی * pvParams );
ساکن خالی prvSwitchThread( Thread_t * x_Thread_مقصد_Resume,
                             Thread_t *x_Thread_مقصد_Suspend );
ساکن خالی prvSuspendSelf( Thread_t * thread);
ساکن خالی prvResumeThread( Thread_t * xThreadId );
ساکن خالی v_پورٹ_System_Tick_Handler( صحیح sig );
ساکن خالی v_پورٹ_آغاز_پہلا_Task( خالی );
/*-----------------------------------------------------------*/

ساکن خالی prvFatalError( ثابت حرف *pcCall, صحیح iErrno )
{
    fprintf( stderr, "%s: %s\n", pcCall, strerror( iErrno ) );
    abort();
}

/*
 * See header file for description.
 */
ڈھیر_Type_t *px_پورٹ_Initialise_ڈھیر( ڈھیر_Type_t *px_چوٹی_کا_ڈھیر,
                                    ڈھیر_Type_t *px_انتہا_کا_ڈھیر,
                                    TaskFunction_t px_رمز,
                                    خالی *pvParameters )
{
    Thread_t *thread;
    pthread_attr_t x_Thread_اوصاف;
    size_t ul_ڈھیر_سائز;
    صحیح iRet;

    (خالی)pthread_once( &h_Sig_تیاری_Thread, prv_تیاری_اشارات_And_Scheduler_Policy );

    /*
     * Store the additional thread data at the start of the stack.
     */
    thread = (Thread_t *)(px_چوٹی_کا_ڈھیر + 1) - 1;
    px_چوٹی_کا_ڈھیر = (ڈھیر_Type_t *)thread - 1;
    ul_ڈھیر_سائز = (px_چوٹی_کا_ڈھیر + 1 - px_انتہا_کا_ڈھیر) * حجم(*px_چوٹی_کا_ڈھیر);

    thread->px_رمز = px_رمز;
    thread->pvParams = pvParameters;
    thread->xDying = pdFALSE;

    pthread_attr_init( &x_Thread_اوصاف );
    pthread_attr_setstack( &x_Thread_اوصاف, px_انتہا_کا_ڈھیر, ul_ڈھیر_سائز );

    thread->ev = event_create();

    بنیاد_Type_t پچھلا_intr_level = x_پورٹ_سیٹ_مداخلت_نقاب();

    iRet = pthread_create( &thread->pthread, &x_Thread_اوصاف,
                           prv_Wait_For_آغاز, thread );
    اگر ( iRet )
    {
        prvFatalError( "pthread_create", iRet );
    }

    v_پورٹ_Clear_مداخلت_نقاب( پچھلا_intr_level );

    واپس px_چوٹی_کا_ڈھیر;
}
/*-----------------------------------------------------------*/

خالی v_پورٹ_آغاز_پہلا_Task( خالی )
{
    Thread_t *px_پہلا_Thread = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    /* Start the first task. */
    prvResumeThread( px_پہلا_Thread );
}
/*-----------------------------------------------------------*/

/*
 * See header file for description.
 */
بنیاد_Type_t x_پورٹ_آغاز_Scheduler( خالی )
{
    صحیح iSignal;
    sigset_t x_اشارات;

    hMainThread = pthread_self();

    /* Start the timer that generates the tick ISR(SIGALRM).
       Interrupts are disabled here already. */
    prv_تیاری_Timer_مداخلت();

    /* Start the first task. */
    v_پورٹ_آغاز_پہلا_Task();

    /* Wait until signaled by vPortEndScheduler(). */
    sigemptyset( &x_اشارات );
    sigaddset( &x_اشارات, SIG_RESUME );

    جب ( !x_Scheduler_انتہا )
    {
        sigwait( &x_اشارات, &iSignal );
    }

    /* Cancel the Idle task and free its resources */
#اگر ( INCLUDE_xTaskGetIdleTaskHandle == 1 )
    v_پورٹ_Cancel_Thread( xTaskGetIdleTaskHandle() );
#اگر_ختم

#اگر ( configUSE_TIMERS == 1 )
    /* Cancel the Timer task and free its resources */
    v_پورٹ_Cancel_Thread( xTimerGetTimerDaemonTaskHandle() );
#اگر_ختم /* configUSE_TIMERS */

    /* Restore original signal mask. */
    (خالی)pthread_sigmask( SIG_SETMASK, &x_Scheduler_Original_Signal_نقاب,  تہی );

    واپس 0;
}
/*-----------------------------------------------------------*/

خالی v_پورٹ_انتہا_Scheduler( خالی )
{
    ساخت itimerval itimer;
    ساخت sigaction sigtick;
    Thread_t *xCurrentThread;

    /* Stop the timer and ignore any pending SIGALRMs that would end
     * up running on the main thread when it is resumed. */
    itimer.it_value.tv_sec = 0;
    itimer.it_value.tv_usec = 0;

    itimer.it_interval.tv_sec = 0;
    itimer.it_interval.tv_usec = 0;
    (خالی)setitimer( ITIMER_REAL, &itimer, تہی );

    sigtick.sa_flags = 0;
    sigtick.sa_handler = SIG_IGN;
    sigemptyset( &sigtick.sa_mask );
    sigaction( SIGALRM, &sigtick, تہی );

    /* Signal the scheduler to exit its loop. */
    x_Scheduler_انتہا = pdTRUE;
    (خالی)pthread_kill( hMainThread, SIG_RESUME );

    xCurrentThread = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );
    prvSuspendSelf(xCurrentThread);
}
/*-----------------------------------------------------------*/

ساکن خالی v_پورٹ_Disable_Interrupts( خالی )
{
    pthread_sigmask( SIG_BLOCK, &x_All_اشارات, تہی );
}
/*-----------------------------------------------------------*/

ساکن خالی v_پورٹ_Enable_Interrupts( خالی )
{
    pthread_sigmask( SIG_UNBLOCK, &x_All_اشارات, تہی );
}
/*-----------------------------------------------------------*/

خالی v_پورٹ_Enter_Critical_IDF( خالی )
{
    اگر ( uxCriticalNestingIDF == 0 && ux_مداخلت_Level == 0)
    {
        v_پورٹ_Disable_Interrupts();
    }
    uxCriticalNestingIDF++;
}
/*-----------------------------------------------------------*/

خالی v_پورٹ_Exit_Critical_IDF( خالی )
{
    uxCriticalNestingIDF--;

    /* If we have reached 0 then re-enable the interrupts. */
    اگر( uxCriticalNestingIDF == 0 && ux_مداخلت_Level == 0)
    {
        v_پورٹ_Enable_Interrupts();
    }
}
/*-----------------------------------------------------------*/

خالی v_پورٹ_Yield_From_ISR( خالی )
{
    Thread_t *x_Thread_مقصد_Suspend;
    Thread_t *x_Thread_مقصد_Resume;

    x_Thread_مقصد_Suspend = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    vTaskSwitchContext();

    x_Thread_مقصد_Resume = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

    prvSwitchThread( x_Thread_مقصد_Resume, x_Thread_مقصد_Suspend );
}
/*-----------------------------------------------------------*/

خالی v_پورٹ_Yield( خالی )
{
    بنیاد_Type_t پچھلا_intr_level = x_پورٹ_سیٹ_مداخلت_نقاب();

    v_پورٹ_Yield_From_ISR();

    v_پورٹ_Clear_مداخلت_نقاب( پچھلا_intr_level );
}
/*-----------------------------------------------------------*/

/* In SMP code, the disable/enable interrupt macros are calling the set/get interrupt mask functions below.
   Hence, we need to call vPortDisableInterrupts() and vPortEnableInterrupts(), otherwise interrupts
   are never disabled/enabled. */

بنیاد_Type_t x_پورٹ_سیٹ_مداخلت_نقاب( خالی )
{
    اگر (ux_مداخلت_Level == 0 && uxCriticalNestingIDF == 0) {
        v_پورٹ_Disable_Interrupts();
    }
    بنیاد_Type_t پچھلا_intr_level = ux_مداخلت_Level;
    ux_مداخلت_Level++;
    واپس پچھلا_intr_level;
}
/*-----------------------------------------------------------*/

خالی v_پورٹ_Clear_مداخلت_نقاب( بنیاد_Type_t x_نقاب )
{
    // Only re-enable interrupts if xMask is 0
    ux_مداخلت_Level = x_نقاب;
    اگر (ux_مداخلت_Level == 0 && uxCriticalNestingIDF == 0) {
        v_پورٹ_Enable_Interrupts();
    }
}
/*-----------------------------------------------------------*/

ساکن uint64_t prv_Get_وقت_Ns(خالی)
{
    ساخت timespec t;

    clock_gettime(CLOCK_MONOTONIC, &t);

    واپس t.tv_sec * 1000000000ull + t.tv_nsec;
}

ساکن uint64_t prv_آغاز_وقت_Ns;
/* commented as part of the code below in vPortSystemTickHandler,
 * to adjust timing according to full demo requirements */
/* static uint64_t prvTickCount; */

/*
 * Setup the systick timer to generate the tick interrupts at the required
 * frequency.
 */
خالی prv_تیاری_Timer_مداخلت( خالی )
{
    ساخت itimerval itimer;
    صحیح iRet;

    /* Initialise the structure with the current timer information. */
    iRet = getitimer( ITIMER_REAL, &itimer );
    اگر ( iRet )
    {
        prvFatalError( "getitimer", errno );
    }

    /* Set the interval between timer events. */
    itimer.it_interval.tv_sec = 0;
    itimer.it_interval.tv_usec = پورٹ_TICK_شرح_MICROSECONDS;

    /* Set the current count-down. */
    itimer.it_value.tv_sec = 0;
    itimer.it_value.tv_usec = پورٹ_TICK_شرح_MICROSECONDS;

    /* Set-up the timer interrupt. */
    iRet = setitimer( ITIMER_REAL, &itimer, تہی );
    اگر ( iRet )
    {
        prvFatalError( "setitimer", errno );
    }

    prv_آغاز_وقت_Ns = prv_Get_وقت_Ns();
}
/*-----------------------------------------------------------*/

ساکن خالی v_پورٹ_System_Tick_Handler( صحیح sig )
{
    Thread_t *px_Thread_مقصد_Suspend;
    Thread_t *px_Thread_مقصد_Resume;
    بنیاد_Type_t xSwitchRequired;
    /* uint64_t xExpectedTicks; */

    // Handling a timer signal, so we are currently in an interrupt.
    ux_مداخلت_Nesting++;

#اگر ( configUSE_PREEMPTION == 1 )
    px_Thread_مقصد_Suspend = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );
#اگر_ختم

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

#اگر ( configUSE_PREEMPTION == 1 )
    اگر (xSwitchRequired == pdTRUE) {
        /* Select Next Task. */
        vTaskSwitchContext();

        px_Thread_مقصد_Resume = prvGetThreadFromTask( xTaskGetCurrentTaskHandle() );

        prvSwitchThread(px_Thread_مقصد_Resume, px_Thread_مقصد_Suspend);
    }
#ورنہ
    (خالی)xSwitchRequired;
#اگر_ختم

    // Returning from the timer signal handler, so we are exiting the interrupt.
    ux_مداخلت_Nesting--;
}
/*-----------------------------------------------------------*/

خالی v_پورٹ_Thread_Dying( خالی *px_Task_مقصد_Delete, متلون بنیاد_Type_t *pxPendYield )
{
    Thread_t *pxThread = prvGetThreadFromTask( px_Task_مقصد_Delete );

    pxThread->xDying = pdTRUE;
}

خالی v_پورٹ_Cancel_Thread( خالی *px_Task_مقصد_Delete )
{
    Thread_t *px_Thread_مقصد_Cancel = prvGetThreadFromTask( px_Task_مقصد_Delete );

    /*
     * The thread has already been suspended so it can be safely cancelled.
     */
    pthread_cancel( px_Thread_مقصد_Cancel->pthread );
    pthread_join( px_Thread_مقصد_Cancel->pthread, تہی );
    event_delete( px_Thread_مقصد_Cancel->ev );
}
/*-----------------------------------------------------------*/

ساکن خالی *prv_Wait_For_آغاز( خالی * pvParams )
{
    Thread_t *pxThread = pvParams;

    prvSuspendSelf(pxThread);

    /* Resumed for the first time, thus this thread didn't previously call
     * prvSwitchThread(). So we need to initialise the state variables for this
     * thread. */
    uxCriticalNestingIDF = 0;
    ux_مداخلت_Nesting = 0;
    ux_مداخلت_Level = 0;
    v_پورٹ_Enable_Interrupts();

    /* Call the task's entry point. */
    pxThread->px_رمز( pxThread->pvParams );

    /* A function that implements a task must not exit or attempt to return to
    * its caller as there is nothing to return to. If a task wants to exit it
    * should instead call vTaskDelete( NULL ). Artificially force an assert()
    * to be triggered if configASSERT() is defined, so application writers can
        * catch the error. */
    configASSERT( pdFALSE );

    واپس تہی;
}
/*-----------------------------------------------------------*/

ساکن خالی prvSwitchThread( Thread_t *px_Thread_مقصد_Resume,
                             Thread_t *px_Thread_مقصد_Suspend )
{
    بنیاد_Type_t uxSavedCriticalNestingIDF;
    بنیاد_Type_t ux_Saved_مداخلت_Nesting;
    بنیاد_Type_t ux_Saved_مداخلت_Level;

    اگر ( px_Thread_مقصد_Suspend != px_Thread_مقصد_Resume )
    {
        /* It is possible for prvSwitchThread() to be called...
         * - while inside an ISR (i.e., via vPortSystemTickHandler() or vPortYieldFromISR())
         * - while interrupts are disabled or in a critical section (i.e., via vPortYield())
         *
         * So we need to save the various count variables as part of the thread's context.
         * They are restored when the pthread switches back. */
        uxSavedCriticalNestingIDF = uxCriticalNestingIDF;
        ux_Saved_مداخلت_Nesting = ux_مداخلت_Nesting;
        ux_Saved_مداخلت_Level = ux_مداخلت_Level;

        prvResumeThread( px_Thread_مقصد_Resume );
        اگر ( px_Thread_مقصد_Suspend->xDying )
        {
            pthread_exit( تہی );
        }
        prvSuspendSelf( px_Thread_مقصد_Suspend );

        uxCriticalNestingIDF = uxSavedCriticalNestingIDF;
        ux_مداخلت_Nesting = ux_Saved_مداخلت_Nesting;
        ux_مداخلت_Level = ux_Saved_مداخلت_Level;
    }
}
/*-----------------------------------------------------------*/

ساکن خالی prvSuspendSelf( Thread_t *thread )
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

ساکن خالی prvResumeThread( Thread_t *xThreadId )
{
    اگر ( pthread_self() != xThreadId->pthread )
    {
        event_signal(xThreadId->ev);
    }
}
/*-----------------------------------------------------------*/

ساکن خالی prv_تیاری_اشارات_And_Scheduler_Policy( خالی )
{
    ساخت sigaction sigresume, sigtick;
    صحیح iRet;

    hMainThread = pthread_self();

    /* Initialise common signal masks. */
    sigfillset( &x_All_اشارات );
    /* Don't block SIGINT so this can be used to break into GDB while
     * in a critical section. */
    sigdelset( &x_All_اشارات, SIGINT );

    /*
     * Block all signals in this thread so all new threads
     * inherits this mask.
     *
     * When a thread is resumed for the first time, all signals
     * will be unblocked.
     */
    (خالی)pthread_sigmask( SIG_SETMASK, &x_All_اشارات,
                           &x_Scheduler_Original_Signal_نقاب );

    /* SIG_RESUME is only used with sigwait() so doesn't need a
       handler. */
    sigresume.sa_flags = 0;
    sigresume.sa_handler = SIG_IGN;
    sigfillset( &sigresume.sa_mask );

    sigtick.sa_flags = 0;
    sigtick.sa_handler = v_پورٹ_System_Tick_Handler;
    sigfillset( &sigtick.sa_mask );

    iRet = sigaction( SIG_RESUME, &sigresume, تہی );
    اگر ( iRet )
    {
        prvFatalError( "sigaction", errno );
    }

    iRet = sigaction( SIGALRM, &sigtick, تہی );
    اگر ( iRet )
    {
        prvFatalError( "sigaction", errno );
    }
}
/*-----------------------------------------------------------*/

غیرعلامتی طویل ul_پورٹ_Get_تسلسل_وقت( خالی )
{
    ساخت tms xTimes;

    times( &xTimes );

    واپس ( غیرعلامتی طویل ) xTimes.tms_utime;
}
/*-----------------------------------------------------------*/
bool پورٹ_VALID_LIST_MEM(ثابت خالی *ptr)
{
    واپس true;
}

bool پورٹ_VALID_TCB_MEM(ثابت خالی *ptr)
{
    واپس true;
}

bool پورٹ_VALID_ڈھیر_MEM(ثابت خالی *ptr)
{
    واپس true;
}
/*-----------------------------------------------------------*/

پورٹ_MUX_TYPE پورٹ_x_Task_تالا = پورٹ_MUX_INITIALIZER_UNLOCKED;
پورٹ_MUX_TYPE پورٹ_x_ISRLock = پورٹ_MUX_INITIALIZER_UNLOCKED;

ساکن ثابت حرف *لیبل = "port";

بنیاد_Type_t x_پورٹ_Check_If_اندر_ISR(خالی)
{
    واپس (ux_مداخلت_Nesting == 0) ? pdFALSE : pdTRUE;
}

خالی app_main(خالی);

ساکن خالی main_task(خالی* دلائل)
{
    app_main();
    vTaskDelete(تہی);
}

صحیح مین(صحیح argc, ثابت حرف **argv)
{
    // This makes sure that stdio is flushed after each '\n' so that idf.py monitor
    // reads the program output on time.
    setvbuf(stdout, تہی, _IOLBF, 0);

    usleep(1000);

    بنیاد_Type_t res;

#اگر ( config_عدد_CORES > 1 )
    res = x_Task_Create_Affinity_سیٹ(&main_task, "main",
                                 ESP_TASK_MAIN_ڈھیر, تہی,
                                 ESP_TASK_MAIN_PRIO, ESP_TASK_MAIN_CORE, تہی);
#ورنہ
    res = xTaskCreate(&main_task, "main",
                      ESP_TASK_MAIN_ڈھیر, تہی,
                      ESP_TASK_MAIN_PRIO, تہی);
#اگر_ختم

    أكد(res == pdTRUE);
    (خالی)res;

    ESP_LOGI(لیبل, "Starting SMP scheduler.");
    v_Task_آغاز_Scheduler();

    // This line should never be reached
    أكد(false);
}

خالی esp_vApplicationIdleHook(خالی)
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

خالی esp_vApplicationTickHook( خالی ) { }

#اگر  (  configUSE_TICK_HOOK > 0 )
خالی vApplicationTickHook( خالی )
{
    esp_vApplicationTickHook();
}
#اگر_ختم

#اگر ( config_SUPPORT_STATIC_تخصیص == 1 )
/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
 * implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
 * used by the Idle task. */
خالی v_Application_Get_Idle_Task_حافظہ( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                    ڈھیر_Type_t ** ppx_Idle_Task_ڈھیر_بفر,
                                    uint32_t * pul_Idle_Task_ڈھیر_سائز )
{
/* If the buffers to be provided to the Idle task are declared inside this
 * function then they must be declared static - otherwise they will be allocated on
 * the stack and so not exists after this function exits. */
    ساکن StaticTask_t xIdleTaskTCB;
    ساکن ڈھیر_Type_t ux_Idle_Task_ڈھیر[ config_اقل_ڈھیر_سائز ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
     * state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppx_Idle_Task_ڈھیر_بفر = ux_Idle_Task_ڈھیر;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in bytes. */
    *pul_Idle_Task_ڈھیر_سائز = config_اقل_ڈھیر_سائز;
}
#اگر_ختم // configSUPPORT_STATIC_ALLOCATION == 1
/*-----------------------------------------------------------*/

#اگر ( (config_SUPPORT_STATIC_تخصیص == 1) && (configUSE_TIMERS == 1))

/* When configSUPPORT_STATIC_ALLOCATION is set to 1 the application writer can
 * use a callback function to optionally provide the memory required by the idle
 * and timer tasks.  This is the stack that will be used by the timer task.  It is
 * declared here, as a global, so it can be checked by a test that is implemented
 * in a different file. */
ڈھیر_Type_t ux_Timer_Task_ڈھیر[ config_TIMER_TASK_ڈھیر_گہرائی ];


/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
 * application must provide an implementation of vApplicationGetTimerTaskMemory()
 * to provide the memory that is used by the Timer service task. */
خالی v_Application_Get_Timer_Task_حافظہ( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                     ڈھیر_Type_t ** ppx_Timer_Task_ڈھیر_بفر,
                                     uint32_t * pul_Timer_Task_ڈھیر_سائز )
{
/* If the buffers to be provided to the Timer task are declared inside this
 * function then they must be declared static - otherwise they will be allocated on
 * the stack and so not exists after this function exits. */
    ساکن StaticTask_t xTimerTaskTCB;

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
     * task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppx_Timer_Task_ڈھیر_بفر = ux_Timer_Task_ڈھیر;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in bytes. */
    *pul_Timer_Task_ڈھیر_سائز = config_TIMER_TASK_ڈھیر_گہرائی;
}
#اگر_ختم // configSUPPORT_STATIC_ALLOCATION == 1 && (configUSE_TIMERS == 1)

خالی v_پورٹ_Take_تالا( پورٹ_MUX_TYPE *تالا_2 )
{
    spinlock_acquire( تالا_2, پورٹ_MUX_NO_TIMEOUT);
}

خالی v_پورٹ_Release_تالا( پورٹ_MUX_TYPE *تالا_2 )
{
    spinlock_release( تالا_2 );
}

#تعریف_کرو FREERTOS_SMP_MALLOC_CAPS    (MALLOC_CAP_اندرونی|MALLOC_CAP_8_ثنائی_ہندسہ)

خالی *pv_پورٹ_Malloc( size_t x_سائز )
{
    واپس ڈھیر_caps_malloc(x_سائز, FREERTOS_SMP_MALLOC_CAPS);
}

خالی v_پورٹ_Free( خالی *pv )
{
    ڈھیر_caps_free(pv);
}

خالی __attribute__((weak)) v_Application_ڈھیر_حد_سے_تجاوز_Hook(TaskHandle_t xTask, حرف *pcTaskName)
{
#تعریف_کرو ERR_STR1 "***ERROR*** A stack overflow in task "
#تعریف_کرو ERR_STR2 " has been detected."
    ثابت حرف *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    حرف buf[حجم(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + حجم(ERR_STR2) + 1 /* null char */] = {0};

    حرف *dest = buf;
    برائے (صحیح i = 0; i < حجم(str) / حجم(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
	لکھو("%s\n", buf);
    abort();
}

// ------- Thread Local Storage Pointers Deletion Callbacks -------

#اگر ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
خالی v_پورٹ_TLSPointers_Del_Cb( خالی *pxTCB )
{
    /* Typecast pxTCB to StaticTask_t type to access TCB struct members.
     * pvDummy15 corresponds to pvThreadLocalStoragePointers member of the TCB.
     */
    StaticTask_t *tcb = ( StaticTask_t * )pxTCB;

    /* The TLSP deletion callbacks are stored at an offset of (configNUM_THREAD_LOCAL_STORAGE_POINTERS/2) */
    TlsDeleteCallbackFunction_t *pv_Thread_Local_Storage_اشارے_Del_Callback = ( TlsDeleteCallbackFunction_t * )( &( tcb->pvDummy15[ ( config_عدد_THREAD_LOCAL_STORAGE_اشارے / 2 ) ] ) );

    /* We need to iterate over half the depth of the pvThreadLocalStoragePointers area
     * to access all TLS pointers and their respective TLS deletion callbacks.
     */
    برائے ( صحیح x = 0; x < ( config_عدد_THREAD_LOCAL_STORAGE_اشارے / 2 ); x++ ) {
        اگر ( pv_Thread_Local_Storage_اشارے_Del_Callback[ x ] != تہی ) {  //If del cb is set
            // We skip the check if the callback is executable as that is difficult to determine for different
            // platforms (compare xtensa and riscv code).
            pv_Thread_Local_Storage_اشارے_Del_Callback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#اگر_ختم // CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS

خالی v_پورٹ_Clean_اوپر_TCB ( خالی *pxTCB )
{
    #اگر ( CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK )
        /* Call the user defined task pre-deletion hook */
        خارجی خالی vTaskPreDeletionHook( خالی * pxTCB );
        vTaskPreDeletionHook( pxTCB );
    #اگر_ختم /* CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK */

#اگر ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
    /* Call TLS pointers deletion callbacks */
    v_پورٹ_TLSPointers_Del_Cb( pxTCB );
#اگر_ختم /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

    v_پورٹ_Cancel_Thread(pxTCB);
}
