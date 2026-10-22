/*
 * SPDX-FileCopyrightText: 2025-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <stdint.h>
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <pthread.h>
#అన్తర్భావ <sys/queue.h>
#అన్తర్భావ <time.h>
#అన్తర్భావ "string.h"
#అన్తర్భావ "FreeRTOS.h"
#అన్తర్భావ "task.h"
#అన్తర్భావ "utils/wait_for_event.h"
#అన్తర్భావ "esp_private/freertos_linux_coop_syscalls.h"
#అన్తర్భావ "utils/linux_port_utils.h"

#నిర్వచించు FREERTOS_SIM_TICK_PERIOD_US (1000000 / CONFIG_FREERTOS_HZ)

రకనిర్వచనం నిర్మాణం thread {
    స్థిరరాశి అక్షరం *name;
    pthread_t pthread;
    TaskFunction_t px_సంకేతం;
    శూన్య *pvParams;
    bool is_dying;
    bool yield_needed;
    నిర్మాణం event *ev;
} thread_t;

రకనిర్వచనం నిర్మాణం task_thread_కణుపు {
    TaskHandle_t handle;
    thread_t *thread;
    SLIST_ENTRY(task_thread_కణుపు) తదుపరి;
} task_thread_కణుపు_t;


స్థిర SLIST_HEAD(task_thread_కణుపు_ll, task_thread_కణుపు) s_task_thread_list = SLIST_HEAD_INITIALIZER(task_thread_కణుపు);
స్థిర pthread_mutex_t s_thread_map_mutex = PTHREAD_MUTEX_INITIALIZER;

స్థిర pthread_mutex_t s_పోర్టు_mutex;
స్థిర pthread_t s_scheduler_thread;
స్థిర bool s_scheduler_started = false;
స్థిర పూర్ణాంకం s_ux_critical_nesting = 0;

/* TLS flag: true only when inside a real FreeRTOS task pthread */
స్థిర __thread bool s_లోపల_freertos_task = false;

bool linux_పోర్టు_లోపల_freertos_task(శూన్య)
{
    తిరిగి s_లోపల_freertos_task;
}

స్థిర శూన్య linux_పోర్టు_initialize_mutexes(శూన్య)
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&s_పోర్టు_mutex, &attr);
    pthread_mutexattr_destroy(&attr);
}

స్థిర శూన్య linux_పోర్టు_fatal_error(స్థిరరాశి అక్షరం *సందేశం, పూర్ణాంకం err)
{
    fprintf(stderr, "%s: %s\n", సందేశం, strerror(err));
    abort();
}

స్థిర శూన్య linux_పోర్టు_రిజిస్టరు_thread(TaskHandle_t handle, thread_t *thread)
{
    ఒకవేళ (handle == ఖాళీ) {
        తిరిగి;
    }

    task_thread_కణుపు_t *కణుపు = జ్ఞాపకం_ఇవ్వు(పరిమాణం(task_thread_కణుపు_t));
    ఒకవేళ (!కణుపు) {
        linux_పోర్టు_fatal_error("Failed to allocate thread map node", -1);
    }

    కణుపు->handle = handle;
    కణుపు->thread = thread;

    pthread_mutex_lock(&s_thread_map_mutex);
    SLIST_INSERT_HEAD(&s_task_thread_list, కణుపు, తదుపరి);
    pthread_mutex_unlock(&s_thread_map_mutex);
}

స్థిర శూన్య linux_పోర్టు_unregister_thread(TaskHandle_t handle)
{
    ఒకవేళ (handle == ఖాళీ) {
        తిరిగి;
    }

    pthread_mutex_lock(&s_thread_map_mutex);

    task_thread_కణుపు_t *cur_కణుపు = SLIST_FIRST(&s_task_thread_list);
    task_thread_కణుపు_t *prev_కణుపు = ఖాళీ;

    అయితే (cur_కణుపు) {
        ఒకవేళ (cur_కణుపు->handle == handle) {
            ఒకవేళ (prev_కణుపు) {
                prev_కణుపు->తదుపరి.sle_తదుపరి = SLIST_NEXT(cur_కణుపు, తదుపరి);
            } లేకపోతే {
                SLIST_REMOVE_HEAD(&s_task_thread_list, తదుపరి);
            }

            విడుదల(cur_కణుపు);
            pthread_mutex_unlock(&s_thread_map_mutex);
            తిరిగి;
        }

        prev_కణుపు = cur_కణుపు;
        cur_కణుపు = SLIST_NEXT(cur_కణుపు, తదుపరి);
    }

    pthread_mutex_unlock(&s_thread_map_mutex);
}

స్థిర thread_t *linux_పోర్టు_get_thread_from_handle(TaskHandle_t handle)
{
    ఒకవేళ (handle == ఖాళీ) {
        తిరిగి ఖాళీ;
    }

    pthread_mutex_lock(&s_thread_map_mutex);
    task_thread_కణుపు_t *కణుపు = ఖాళీ;
    SLIST_FOREACH(కణుపు, &s_task_thread_list, తదుపరి) {
        ఒకవేళ (కణుపు->handle == (TaskHandle_t)(*(దొంతర_Type_t **)(handle))) {
            thread_t *t = కణుపు->thread;
            pthread_mutex_unlock(&s_thread_map_mutex);
            తిరిగి t;
        }
    }
    pthread_mutex_unlock(&s_thread_map_mutex);
    తిరిగి ఖాళీ;
}

స్థిర thread_t *linux_పోర్టు_get_calling_thread(శూన్య)
{
    pthread_t self = pthread_self();
    pthread_mutex_lock(&s_thread_map_mutex);
    task_thread_కణుపు_t *కణుపు = ఖాళీ;
    SLIST_FOREACH(కణుపు, &s_task_thread_list, తదుపరి) {
        ఒకవేళ (pthread_equal(కణుపు->thread->pthread, self)) {
            thread_t *thread = కణుపు->thread;
            pthread_mutex_unlock(&s_thread_map_mutex);
            తిరిగి thread;
        }
    }
    pthread_mutex_unlock(&s_thread_map_mutex);
    తిరిగి ఖాళీ;
}

pthread_t linux_పోర్టు_get_scheduled_task_pthread(శూన్య)
{
    thread_t *thread = linux_పోర్టు_get_thread_from_handle(xTaskGetCurrentTaskHandle());
    తిరిగి thread ? thread->pthread : pthread_self();
}

స్థిర శూన్య *linux_పోర్టు_task_runner(శూన్య *arg)
{
    /* Allow this thread to be cancelled */
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, ఖాళీ);

    /* set the flag showing that this is a freertos task */
    s_లోపల_freertos_task = true;

    /* setup the backtrace signal. ONLY triggered before abort so
     * it will not interfere with the simulation while its running */
    linux_పోర్టు_అమరిక_backtrace_signal();

    thread_t *thread = arg;

    /* Block until scheduler signals first time, then run the task body. */
    event_wait(thread->ev);
    thread->px_సంకేతం(thread->pvParams);

    తిరిగి ఖాళీ;
}

స్థిర శూన్య linux_పోర్టు_unblock_thread(thread_t *thread)
{
    event_signal(thread->ev);
}

స్థిర శూన్య linux_పోర్టు_ఖండం_thread(thread_t *thread)
{
    event_wait(thread->ev);
}

స్థిర శూన్య linux_పోర్టు_increment_tick(శూన్య)
{
    (శూన్య)xTaskIncrementTick();
}

స్థిర శూన్య linux_పోర్టు_switch_context(TaskHandle_t current_task_hdl)
{
    pthread_mutex_lock(&s_పోర్టు_mutex);

    thread_t *current_thread = linux_పోర్టు_get_thread_from_handle(current_task_hdl);

    /* get the task that should be scheduled next */
    vTaskSwitchContext();

    /* get the new task to schedule and the associated thread item */
    TaskHandle_t తదుపరి_task_hdl = xTaskGetCurrentTaskHandle();
    thread_t *తదుపరి_thread = linux_పోర్టు_get_thread_from_handle(తదుపరి_task_hdl);

    /* unblock the newly scheduled task if it is different from
     * the one already scheduled */
    ఒకవేళ (తదుపరి_thread) {
        /* Only unblock the thread if we are actually switching to a
         * different one. Signaling the already-running thread would
         * leave a stale event_triggered flag, causing its next
         * event_wait (e.g. in vPortYield) to return immediately
         * instead of blocking.
         *
         * Exception: on the very first switch, the task is still blocked
         * in its initial event_wait (linux_port_task_runner), so we must
         * signal it even though current_thread == next_thread. */
        ఒకవేళ (తదుపరి_thread != current_thread || !s_scheduler_started) {
            /* Discard any stale wake on the task being switched out.
             *
             * Because tasks are pthreads that cannot be forcibly paused,
             * the port can only "unschedule" a task by choosing a different
             * next task; the outgoing task keeps running until it next
             * blocks itself in event_wait (e.g. via vPortYield). If, while
             * it was unscheduled, the scheduler already switched back into
             * it (event_signal in a later switch) without the task ever
             * having parked in event_wait, that signal is never consumed
             * and event_triggered stays latched. Its next voluntary block
             * would then return immediately instead of blocking (e.g.
             * vTaskDelay(100ms) returning in 0ms).
             *
             * Clearing here, under s_port_mutex and atomically with the
             * scheduling decision, drops that latched-but-unconsumed wake.
             * It is safe: a legitimate wake can only be delivered later,
             * when the scheduler next selects this task as next_thread.
             *
             * Reached only for a genuine switch to a different task:
             * entering this block with next_thread == current_thread
             * happens solely on the first switch (!s_scheduler_started),
             * which the current_thread NULL-check below also tolerates.
             * The NULL-check additionally guards the deleted-task path
             * where the outgoing task has no thread mapping. */
            ఒకవేళ (current_thread) {
                event_clear(current_thread->ev);
            }
            linux_పోర్టు_unblock_thread(తదుపరి_thread);
        }
    }

    ఒకవేళ (!s_scheduler_started) {
        s_scheduler_started = true;
    }

    /* fill the name of the task in the thread item if not done already. */
    ఒకవేళ (తదుపరి_thread && తదుపరి_thread->name == ఖాళీ) {
        /* fill the name of the thread now */
        తదుపరి_thread->name = pcTaskGetName(తదుపరి_task_hdl);
    }

    /* fill the name of the task in the thread item if not done already. */
    ఒకవేళ (current_thread && current_thread->name == ఖాళీ) {
        /* fill the name of the thread now */
        current_thread->name = pcTaskGetName(current_task_hdl);
    }

    pthread_mutex_unlock(&s_పోర్టు_mutex);
}

స్థిర శూన్య *linux_పోర్టు_scheduler_runner(శూన్య *arg)
{
    (శూన్య)arg;

    అయితే (1) {
        /* sleep for a period of 1 tick */
        usleep(FREERTOS_SIM_TICK_PERIOD_US);

        /* get the task that is currently scheduled */
        TaskHandle_t current_task_hdl = xTaskGetCurrentTaskHandle();

        /* Lock the port mutex. This will block while any task is in a
         * critical section, ensuring ticks don't preempt critical code. */
        pthread_mutex_lock(&s_పోర్టు_mutex);

        /* increment the freertos tick */
        linux_పోర్టు_increment_tick();

        /* schedule a new task, and schedule out the currently running one */
        linux_పోర్టు_switch_context(current_task_hdl);

        pthread_mutex_unlock(&s_పోర్టు_mutex);
    }
    తిరిగి ఖాళీ;
}

దొంతర_Type_t *px_పోర్టు_Initialise_దొంతర(దొంతర_Type_t *px_శిఖరం_యొక్క_దొంతర,
                                   దొంతర_Type_t *px_చివర_యొక్క_దొంతర,
                                   TaskFunction_t px_సంకేతం,
                                   శూన్య *pvParameters)
{
    pthread_attr_t thread_attr;
    size_t thread_దొంతర_కొలత;

    /* Store the thread data at the start of the stack. */
    thread_దొంతర_కొలత = (px_శిఖరం_యొక్క_దొంతర - px_చివర_యొక్క_దొంతర) * పరిమాణం(*px_శిఖరం_యొక్క_దొంతర);
    pthread_attr_init(&thread_attr);
    pthread_attr_setstack(&thread_attr, px_చివర_యొక్క_దొంతర, thread_దొంతర_కొలత);

    thread_t *thread = జ్ఞాపకం_ఇవ్వు(పరిమాణం(thread_t));
    ఒకవేళ (!thread) {
        linux_పోర్టు_fatal_error("Failed to allocate thread metadata", -1);
    }
    thread->name = ఖాళీ; // this will be filled later when we know about the task name
    thread->px_సంకేతం = px_సంకేతం;
    thread->pvParams = pvParameters;
    thread->is_dying = false;
    thread->yield_needed = false;
    thread->ev = event_create();

    linux_పోర్టు_రిజిస్టరు_thread((TaskHandle_t)px_శిఖరం_యొక్క_దొంతర, thread);

    /* create the thread associated with the task being created */
    స్థిరరాశి పూర్ణాంకం ret = pthread_create(&thread->pthread, &thread_attr, linux_పోర్టు_task_runner, thread);
    ఒకవేళ (ret != 0) {
        linux_పోర్టు_fatal_error("pthread_create", ret);
    }
    తిరిగి px_శిఖరం_యొక్క_దొంతర;
}

పునాది_Type_t x_పోర్టు_ప్రారంభం_Scheduler(శూన్య)
{
    /* set the port mutex to be recursive. Must be done before
     * vPortEnableInterrupts() which calls vPortExitCritical(). */
    linux_పోర్టు_initialize_mutexes();

    /* enable interrupt that were disabled in vTaskStartScheduler */
    v_పోర్టు_Enable_Interrupts();

    /* init the cooperative syscall layer (sets stdio non-blocking).
     * Provided by VFS component; weak no-op when VFS is not linked. */
    freertos_linux_coop_syscalls_init();

    /* Start scheduler thread */
    పూర్ణాంకం ret = pthread_create(&s_scheduler_thread, ఖాళీ, linux_పోర్టు_scheduler_runner, ఖాళీ);
    ఒకవేళ (ret != 0) {
       linux_పోర్టు_fatal_error("pthread_create", ret);
    }

    /* Should never return */
    pthread_join(s_scheduler_thread, ఖాళీ);

    తిరిగి 0;
}

శూన్య v_పోర్టు_చివర_Scheduler(శూన్య)
{
    నిష్క్రమించు(0);
}

శూన్య v_పోర్టు_Enter_Critical(శూన్య)
{
    ఒకవేళ (!s_scheduler_started) {
        తిరిగి;
    }

    pthread_mutex_lock(&s_పోర్టు_mutex);

    /* Non-FreeRTOS thread or recursive enter: just bump the counter.
     * The mutex is already held (recursive lock succeeds for same thread). */
    ఒకవేళ (!linux_పోర్టు_లోపల_freertos_task() || s_ux_critical_nesting > 0) {
        s_ux_critical_nesting++;
        తిరిగి;
    }

    /* First enter from a FreeRTOS task: ensure we're the scheduled task.
     * If not, release the mutex, block until the scheduler switches to us,
     * then re-acquire. */
    thread_t *calling_thread = linux_పోర్టు_get_calling_thread();
    thread_t *scheduled_thread = linux_పోర్టు_get_thread_from_handle(xTaskGetCurrentTaskHandle());

    అయితే (calling_thread && !calling_thread->is_dying && calling_thread != scheduled_thread) {
        pthread_mutex_unlock(&s_పోర్టు_mutex);
        linux_పోర్టు_ఖండం_thread(calling_thread);
        pthread_mutex_lock(&s_పోర్టు_mutex);
        calling_thread = linux_పోర్టు_get_calling_thread();
        scheduled_thread = linux_పోర్టు_get_thread_from_handle(xTaskGetCurrentTaskHandle());
    }

    ఒకవేళ (!calling_thread || calling_thread->is_dying) {
        linux_పోర్టు_switch_context(xTaskGetCurrentTaskHandle());
        pthread_mutex_unlock(&s_పోర్టు_mutex);
        తిరిగి;
    }

    s_ux_critical_nesting = 1;
}

శూన్య v_పోర్టు_Exit_Critical(శూన్య)
{
    ఒకవేళ (!s_scheduler_started || s_ux_critical_nesting == 0) {
        తిరిగి;
    }

    s_ux_critical_nesting--;

    /* Check for deferred yield on final exit from a FreeRTOS task */
    ఒకవేళ (s_ux_critical_nesting == 0 && linux_పోర్టు_లోపల_freertos_task()) {
        thread_t *calling_thread = linux_పోర్టు_get_calling_thread();
        ఒకవేళ (calling_thread && calling_thread->yield_needed) {
            calling_thread->yield_needed = false;
            pthread_mutex_unlock(&s_పోర్టు_mutex);
            v_పోర్టు_Yield();
            తిరిగి;
        }
    }

    pthread_mutex_unlock(&s_పోర్టు_mutex);
}

/* Handle the case where the calling pthread is not a registered FreeRTOS task.
 * If the calling thread has been deleted but is still the scheduled task,
 * perform a context switch. Otherwise just release the mutex and return.
 * Returns true if the caller should return early. */
స్థిర bool linux_పోర్టు_handle_deleted_task(TaskHandle_t scheduled_task_hdl,
                                           thread_t *calling_thread)
{
    ఒకవేళ (calling_thread != ఖాళీ) {
        తిరిగి false;
    }

    thread_t *scheduled_thread = linux_పోర్టు_get_thread_from_handle(scheduled_task_hdl);
    ఒకవేళ (scheduled_thread == ఖాళీ) {
        linux_పోర్టు_switch_context(scheduled_task_hdl);
    }
    pthread_mutex_unlock(&s_పోర్టు_mutex);
    తిరిగి true;
}

శూన్య v_పోర్టు_Yield(శూన్య)
{
    pthread_mutex_lock(&s_పోర్టు_mutex);

    thread_t *calling_thread = linux_పోర్టు_get_calling_thread();
    TaskHandle_t scheduled_task_hdl = xTaskGetCurrentTaskHandle();

    ఒకవేళ (linux_పోర్టు_handle_deleted_task(scheduled_task_hdl, calling_thread)) {
        తిరిగి;
    }

    /* If in a critical section, defer the yield until the section exits. */
    ఒకవేళ (s_ux_critical_nesting != 0) {
        calling_thread->yield_needed = true;
        pthread_mutex_unlock(&s_పోర్టు_mutex);
        తిరిగి;
    }

    /* Hand the CPU to the next ready task right now (mimics PendSV on real
     * hardware). */
    linux_పోర్టు_switch_context(scheduled_task_hdl);
    pthread_mutex_unlock(&s_పోర్టు_mutex);

    /* If the newly scheduled task is different from the calling thread,
     * block until the scheduler resumes this task. */
    TaskHandle_t తదుపరి_task_hdl = xTaskGetCurrentTaskHandle();
    thread_t *తదుపరి_thread = linux_పోర్టు_get_thread_from_handle(తదుపరి_task_hdl);
    ఒకవేళ (calling_thread != తదుపరి_thread) {
        linux_పోర్టు_ఖండం_thread(calling_thread);
    }
}

శూన్య v_పోర్టు_Yield_Within_Api(శూన్య)
{
    v_పోర్టు_Yield();
}

శూన్య v_పోర్టు_Suspend_Scheduler(శూన్య)
{
    /* scheduled out task trying to suspend the scheduler should get blocked here */
    pthread_mutex_lock(&s_పోర్టు_mutex);

   /* get the metadata of the pthread calling this function */
    thread_t *calling_thread = linux_పోర్టు_get_calling_thread();

    /* get the thread metadata from the scheduled task */
    TaskHandle_t scheduled_task_hdl = xTaskGetCurrentTaskHandle();

    ఒకవేళ (linux_పోర్టు_handle_deleted_task(scheduled_task_hdl, calling_thread)) {
        తిరిగి;
    }

    thread_t *scheduled_thread = linux_పోర్టు_get_thread_from_handle(scheduled_task_hdl);
    ఒకవేళ (calling_thread != scheduled_thread) {
        pthread_mutex_unlock(&s_పోర్టు_mutex);
        v_పోర్టు_Yield();
        తిరిగి;
    }
    pthread_mutex_unlock(&s_పోర్టు_mutex);
}

శూన్య v_పోర్టు_Disable_Interrupts(శూన్య)
{
    v_పోర్టు_Enter_Critical();
}

శూన్య v_పోర్టు_Enable_Interrupts(శూన్య)
{
    v_పోర్టు_Exit_Critical();
}

పునాది_Type_t x_పోర్టు_సమితి_అంతరాయం_ముసుగు(శూన్య)
{
    v_పోర్టు_Enter_Critical();
    తిరిగి pdTRUE;
}

శూన్య v_పోర్టు_Clear_అంతరాయం_ముసుగు(పునాది_Type_t x_ముసుగు)
{
    v_పోర్టు_Exit_Critical();
}

శూన్య v_పోర్టు_Thread_Dying(శూన్య *px_Task_గమ్యం_Delete, అస్థిర పునాది_Type_t *pxPendYield)
{
    pthread_mutex_lock(&s_పోర్టు_mutex);

    thread_t *thread = linux_పోర్టు_get_thread_from_handle((TaskHandle_t)px_Task_గమ్యం_Delete);
    ఒకవేళ (thread == ఖాళీ) {
        pthread_mutex_unlock(&s_పోర్టు_mutex);
        తిరిగి;
    }

    /* Mark the thread as dying, cancel the thread. the pthread
     * will be stopped on next cancellation point. Do not remove the
     * thread item from the list since it will be done in vPortCancelThread */
    thread->is_dying = true;
    pthread_cancel(thread->pthread);

    pthread_mutex_unlock(&s_పోర్టు_mutex);
}

#ఒకవేళ CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
స్థిర శూన్య v_పోర్టు_TLSPointers_Del_Cb(శూన్య *pxTCB)
{
    StaticTask_t *tcb = (StaticTask_t *)pxTCB;
    TlsDeleteCallbackFunction_t *pvDelCbs = (TlsDeleteCallbackFunction_t *)(&tcb->pvDummy15[config_అంకె_THREAD_LOCAL_STORAGE_సూచికలు / 2]);

    కొరకు (పూర్ణాంకం x = 0; x < (config_అంకె_THREAD_LOCAL_STORAGE_సూచికలు / 2); x++) {
        ఒకవేళ (pvDelCbs[x] != ఖాళీ) {
            pvDelCbs[x](x, tcb->pvDummy15[x]);
        }
    }
}
#అయితే_ముగింపు /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

శూన్య v_పోర్టు_Cancel_Thread(శూన్య *px_Task_గమ్యం_Delete)
{
    pthread_mutex_lock(&s_పోర్టు_mutex);

#ఒకవేళ CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
    v_పోర్టు_TLSPointers_Del_Cb(px_Task_గమ్యం_Delete);
#అయితే_ముగింపు

    thread_t *thread = linux_పోర్టు_get_thread_from_handle((TaskHandle_t)px_Task_గమ్యం_Delete);
    ఒకవేళ (!thread) {
        pthread_mutex_unlock(&s_పోర్టు_mutex);
        తిరిగి;
    }

    ఒకవేళ (thread->is_dying) {
        /* vPortThreadDying already called */
    } లేకపోతే {
        thread->is_dying = true;
        pthread_cancel(thread->pthread);
    }

    /* Save fields and unregister while holding the lock. */
    pthread_t pt = thread->pthread;
    event_t *ev = thread->ev;
    linux_పోర్టు_unregister_thread((TaskHandle_t)px_Task_గమ్యం_Delete);

    /* Release the mutex before joining – the dying thread may need the
     * scheduler (which also takes s_port_mutex) to reach a cancellation
     * point. */
    pthread_mutex_unlock(&s_పోర్టు_mutex);

    pthread_join(pt, ఖాళీ);
    event_delete(ev);
    విడుదల(thread);
}

శూన్య v_పోర్టు_సమితి_దొంతర_Watchpoint(శూన్య *px_దొంతర_ప్రారంభం)
{
}

#ఒకవేళ ( CONFIG_FREERTOS_GENERATE_నిరంతరం_సమయం_STATS )
config_నిరంతరం_సమయం_COUNTER_TYPE x_పోర్టు_Get_నిరంతరం_సమయం_Counter_విలువ( శూన్య )
{
    నిర్మాణం timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);

    తిరిగి ( config_నిరంతరం_సమయం_COUNTER_TYPE ) ( ( ( uint64_t ) ts.tv_sec * 1000000ULL ) +
                                            ( ( uint64_t ) ts.tv_nsec / 1000ULL ) );
}
#అయితే_ముగింపు /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
