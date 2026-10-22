/*
 * SPDX-FileCopyrightText: 2025-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <stdlib.h>
#অন্তর্ভাৱ <stdint.h>
#অন্তর্ভাৱ <unistd.h>
#অন্তর্ভাৱ <pthread.h>
#অন্তর্ভাৱ <sys/queue.h>
#অন্তর্ভাৱ <time.h>
#অন্তর্ভাৱ "string.h"
#অন্তর্ভাৱ "FreeRTOS.h"
#অন্তর্ভাৱ "task.h"
#অন্তর্ভাৱ "utils/wait_for_event.h"
#অন্তর্ভাৱ "esp_private/freertos_linux_coop_syscalls.h"
#অন্তর্ভাৱ "utils/linux_port_utils.h"

#define FREERTOS_SIM_TICK_PERIOD_US (1000000 / CONFIG_FREERTOS_HZ)

প্রকারনির্দেশ সংরচনা thread {
    ধ্রুৱ ৱর্ণ *name;
    pthread_t pthread;
    TaskFunction_t pxCode;
    শূন্য *pvParams;
    bool is_dying;
    bool yield_needed;
    সংরচনা event *ev;
} thread_t;

প্রকারনির্দেশ সংরচনা task_thread_গ্ৰন্থি {
    TaskHandle_t handle;
    thread_t *thread;
    SLIST_ENTRY(task_thread_গ্ৰন্থি) next;
} task_thread_গ্ৰন্থি_t;


স্থির SLIST_HEAD(task_thread_গ্ৰন্থি_ll, task_thread_গ্ৰন্থি) s_task_thread_list = SLIST_HEAD_INITIALIZER(task_thread_গ্ৰন্থি);
স্থির pthread_mutex_t s_thread_map_mutex = PTHREAD_MUTEX_INITIALIZER;

স্থির pthread_mutex_t s_port_mutex;
স্থির pthread_t s_scheduler_thread;
স্থির bool s_scheduler_started = false;
স্থির পূর্ণাঙ্ক s_ux_critical_nesting = 0;

/* TLS flag: true only when inside a real FreeRTOS task pthread */
স্থির __thread bool s_in_freertos_task = false;

bool linux_port_in_freertos_task(শূন্য)
{
    ঘূৰাই s_in_freertos_task;
}

স্থির শূন্য linux_port_initialize_mutexes(শূন্য)
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&s_port_mutex, &attr);
    pthread_mutexattr_destroy(&attr);
}

স্থির শূন্য linux_port_fatal_error(ধ্রুৱ ৱর্ণ *বাৰ্তা, পূর্ণাঙ্ক err)
{
    fprintf(stderr, "%s: %s\n", বাৰ্তা, strerror(err));
    abort();
}

স্থির শূন্য linux_port_register_thread(TaskHandle_t handle, thread_t *thread)
{
    যদি (handle == রিক্ত) {
        ঘূৰাই;
    }

    task_thread_গ্ৰন্থি_t *গ্ৰন্থি = malloc(পরিমাণ(task_thread_গ্ৰন্থি_t));
    যদি (!গ্ৰন্থি) {
        linux_port_fatal_error("Failed to allocate thread map node", -1);
    }

    গ্ৰন্থি->handle = handle;
    গ্ৰন্থি->thread = thread;

    pthread_mutex_lock(&s_thread_map_mutex);
    SLIST_INSERT_HEAD(&s_task_thread_list, গ্ৰন্থি, next);
    pthread_mutex_unlock(&s_thread_map_mutex);
}

স্থির শূন্য linux_port_unregister_thread(TaskHandle_t handle)
{
    যদি (handle == রিক্ত) {
        ঘূৰাই;
    }

    pthread_mutex_lock(&s_thread_map_mutex);

    task_thread_গ্ৰন্থি_t *cur_গ্ৰন্থি = SLIST_FIRST(&s_task_thread_list);
    task_thread_গ্ৰন্থি_t *prev_গ্ৰন্থি = রিক্ত;

    যেতিয়ালৈ (cur_গ্ৰন্থি) {
        যদি (cur_গ্ৰন্থি->handle == handle) {
            যদি (prev_গ্ৰন্থি) {
                prev_গ্ৰন্থি->next.sle_next = SLIST_NEXT(cur_গ্ৰন্থি, next);
            } অন্যথা {
                SLIST_REMOVE_HEAD(&s_task_thread_list, next);
            }

            মুক্ত(cur_গ্ৰন্থি);
            pthread_mutex_unlock(&s_thread_map_mutex);
            ঘূৰাই;
        }

        prev_গ্ৰন্থি = cur_গ্ৰন্থি;
        cur_গ্ৰন্থি = SLIST_NEXT(cur_গ্ৰন্থি, next);
    }

    pthread_mutex_unlock(&s_thread_map_mutex);
}

স্থির thread_t *linux_port_get_thread_from_handle(TaskHandle_t handle)
{
    যদি (handle == রিক্ত) {
        ঘূৰাই রিক্ত;
    }

    pthread_mutex_lock(&s_thread_map_mutex);
    task_thread_গ্ৰন্থি_t *গ্ৰন্থি = রিক্ত;
    SLIST_FOREACH(গ্ৰন্থি, &s_task_thread_list, next) {
        যদি (গ্ৰন্থি->handle == (TaskHandle_t)(*(স্তূপ_Type_t **)(handle))) {
            thread_t *t = গ্ৰন্থি->thread;
            pthread_mutex_unlock(&s_thread_map_mutex);
            ঘূৰাই t;
        }
    }
    pthread_mutex_unlock(&s_thread_map_mutex);
    ঘূৰাই রিক্ত;
}

স্থির thread_t *linux_port_get_calling_thread(শূন্য)
{
    pthread_t self = pthread_self();
    pthread_mutex_lock(&s_thread_map_mutex);
    task_thread_গ্ৰন্থি_t *গ্ৰন্থি = রিক্ত;
    SLIST_FOREACH(গ্ৰন্থি, &s_task_thread_list, next) {
        যদি (pthread_equal(গ্ৰন্থি->thread->pthread, self)) {
            thread_t *thread = গ্ৰন্থি->thread;
            pthread_mutex_unlock(&s_thread_map_mutex);
            ঘূৰাই thread;
        }
    }
    pthread_mutex_unlock(&s_thread_map_mutex);
    ঘূৰাই রিক্ত;
}

pthread_t linux_port_get_scheduled_task_pthread(শূন্য)
{
    thread_t *thread = linux_port_get_thread_from_handle(xTaskGetCurrentTaskHandle());
    ঘূৰাই thread ? thread->pthread : pthread_self();
}

স্থির শূন্য *linux_port_task_runner(শূন্য *arg)
{
    /* Allow this thread to be cancelled */
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, রিক্ত);

    /* set the flag showing that this is a freertos task */
    s_in_freertos_task = true;

    /* setup the backtrace signal. ONLY triggered before abort so
     * it will not interfere with the simulation while its running */
    linux_port_setup_backtrace_signal();

    thread_t *thread = arg;

    /* Block until scheduler signals first time, then run the task body. */
    event_wait(thread->ev);
    thread->pxCode(thread->pvParams);

    ঘূৰাই রিক্ত;
}

স্থির শূন্য linux_port_unblock_thread(thread_t *thread)
{
    event_signal(thread->ev);
}

স্থির শূন্য linux_port_block_thread(thread_t *thread)
{
    event_wait(thread->ev);
}

স্থির শূন্য linux_port_increment_tick(শূন্য)
{
    (শূন্য)xTaskIncrementTick();
}

স্থির শূন্য linux_port_switch_context(TaskHandle_t current_task_hdl)
{
    pthread_mutex_lock(&s_port_mutex);

    thread_t *current_thread = linux_port_get_thread_from_handle(current_task_hdl);

    /* get the task that should be scheduled next */
    vTaskSwitchContext();

    /* get the new task to schedule and the associated thread item */
    TaskHandle_t next_task_hdl = xTaskGetCurrentTaskHandle();
    thread_t *next_thread = linux_port_get_thread_from_handle(next_task_hdl);

    /* unblock the newly scheduled task if it is different from
     * the one already scheduled */
    যদি (next_thread) {
        /* Only unblock the thread if we are actually switching to a
         * different one. Signaling the already-running thread would
         * leave a stale event_triggered flag, causing its next
         * event_wait (e.g. in vPortYield) to return immediately
         * instead of blocking.
         *
         * Exception: on the very first switch, the task is still blocked
         * in its initial event_wait (linux_port_task_runner), so we must
         * signal it even though current_thread == next_thread. */
        যদি (next_thread != current_thread || !s_scheduler_started) {
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
            যদি (current_thread) {
                event_clear(current_thread->ev);
            }
            linux_port_unblock_thread(next_thread);
        }
    }

    যদি (!s_scheduler_started) {
        s_scheduler_started = true;
    }

    /* fill the name of the task in the thread item if not done already. */
    যদি (next_thread && next_thread->name == রিক্ত) {
        /* fill the name of the thread now */
        next_thread->name = pcTaskGetName(next_task_hdl);
    }

    /* fill the name of the task in the thread item if not done already. */
    যদি (current_thread && current_thread->name == রিক্ত) {
        /* fill the name of the thread now */
        current_thread->name = pcTaskGetName(current_task_hdl);
    }

    pthread_mutex_unlock(&s_port_mutex);
}

স্থির শূন্য *linux_port_scheduler_runner(শূন্য *arg)
{
    (শূন্য)arg;

    যেতিয়ালৈ (1) {
        /* sleep for a period of 1 tick */
        usleep(FREERTOS_SIM_TICK_PERIOD_US);

        /* get the task that is currently scheduled */
        TaskHandle_t current_task_hdl = xTaskGetCurrentTaskHandle();

        /* Lock the port mutex. This will block while any task is in a
         * critical section, ensuring ticks don't preempt critical code. */
        pthread_mutex_lock(&s_port_mutex);

        /* increment the freertos tick */
        linux_port_increment_tick();

        /* schedule a new task, and schedule out the currently running one */
        linux_port_switch_context(current_task_hdl);

        pthread_mutex_unlock(&s_port_mutex);
    }
    ঘূৰাই রিক্ত;
}

স্তূপ_Type_t *px_Port_Initialise_স্তূপ(স্তূপ_Type_t *px_শীৰ্ষ_Of_স্তূপ,
                                   স্তূপ_Type_t *px_End_Of_স্তূপ,
                                   TaskFunction_t pxCode,
                                   শূন্য *pvParameters)
{
    pthread_attr_t thread_attr;
    size_t thread_স্তূপ_size;

    /* Store the thread data at the start of the stack. */
    thread_স্তূপ_size = (px_শীৰ্ষ_Of_স্তূপ - px_End_Of_স্তূপ) * পরিমাণ(*px_শীৰ্ষ_Of_স্তূপ);
    pthread_attr_init(&thread_attr);
    pthread_attr_setstack(&thread_attr, px_End_Of_স্তূপ, thread_স্তূপ_size);

    thread_t *thread = malloc(পরিমাণ(thread_t));
    যদি (!thread) {
        linux_port_fatal_error("Failed to allocate thread metadata", -1);
    }
    thread->name = রিক্ত; // this will be filled later when we know about the task name
    thread->pxCode = pxCode;
    thread->pvParams = pvParameters;
    thread->is_dying = false;
    thread->yield_needed = false;
    thread->ev = event_create();

    linux_port_register_thread((TaskHandle_t)px_শীৰ্ষ_Of_স্তূপ, thread);

    /* create the thread associated with the task being created */
    ধ্রুৱ পূর্ণাঙ্ক ret = pthread_create(&thread->pthread, &thread_attr, linux_port_task_runner, thread);
    যদি (ret != 0) {
        linux_port_fatal_error("pthread_create", ret);
    }
    ঘূৰাই px_শীৰ্ষ_Of_স্তূপ;
}

BaseType_t xPortStartScheduler(শূন্য)
{
    /* set the port mutex to be recursive. Must be done before
     * vPortEnableInterrupts() which calls vPortExitCritical(). */
    linux_port_initialize_mutexes();

    /* enable interrupt that were disabled in vTaskStartScheduler */
    vPortEnableInterrupts();

    /* init the cooperative syscall layer (sets stdio non-blocking).
     * Provided by VFS component; weak no-op when VFS is not linked. */
    freertos_linux_coop_syscalls_init();

    /* Start scheduler thread */
    পূর্ণাঙ্ক ret = pthread_create(&s_scheduler_thread, রিক্ত, linux_port_scheduler_runner, রিক্ত);
    যদি (ret != 0) {
       linux_port_fatal_error("pthread_create", ret);
    }

    /* Should never return */
    pthread_join(s_scheduler_thread, রিক্ত);

    ঘূৰাই 0;
}

শূন্য vPortEndScheduler(শূন্য)
{
    exit(0);
}

শূন্য vPortEnterCritical(শূন্য)
{
    যদি (!s_scheduler_started) {
        ঘূৰাই;
    }

    pthread_mutex_lock(&s_port_mutex);

    /* Non-FreeRTOS thread or recursive enter: just bump the counter.
     * The mutex is already held (recursive lock succeeds for same thread). */
    যদি (!linux_port_in_freertos_task() || s_ux_critical_nesting > 0) {
        s_ux_critical_nesting++;
        ঘূৰাই;
    }

    /* First enter from a FreeRTOS task: ensure we're the scheduled task.
     * If not, release the mutex, block until the scheduler switches to us,
     * then re-acquire. */
    thread_t *calling_thread = linux_port_get_calling_thread();
    thread_t *scheduled_thread = linux_port_get_thread_from_handle(xTaskGetCurrentTaskHandle());

    যেতিয়ালৈ (calling_thread && !calling_thread->is_dying && calling_thread != scheduled_thread) {
        pthread_mutex_unlock(&s_port_mutex);
        linux_port_block_thread(calling_thread);
        pthread_mutex_lock(&s_port_mutex);
        calling_thread = linux_port_get_calling_thread();
        scheduled_thread = linux_port_get_thread_from_handle(xTaskGetCurrentTaskHandle());
    }

    যদি (!calling_thread || calling_thread->is_dying) {
        linux_port_switch_context(xTaskGetCurrentTaskHandle());
        pthread_mutex_unlock(&s_port_mutex);
        ঘূৰাই;
    }

    s_ux_critical_nesting = 1;
}

শূন্য vPortExitCritical(শূন্য)
{
    যদি (!s_scheduler_started || s_ux_critical_nesting == 0) {
        ঘূৰাই;
    }

    s_ux_critical_nesting--;

    /* Check for deferred yield on final exit from a FreeRTOS task */
    যদি (s_ux_critical_nesting == 0 && linux_port_in_freertos_task()) {
        thread_t *calling_thread = linux_port_get_calling_thread();
        যদি (calling_thread && calling_thread->yield_needed) {
            calling_thread->yield_needed = false;
            pthread_mutex_unlock(&s_port_mutex);
            vPortYield();
            ঘূৰাই;
        }
    }

    pthread_mutex_unlock(&s_port_mutex);
}

/* Handle the case where the calling pthread is not a registered FreeRTOS task.
 * If the calling thread has been deleted but is still the scheduled task,
 * perform a context switch. Otherwise just release the mutex and return.
 * Returns true if the caller should return early. */
স্থির bool linux_port_handle_deleted_task(TaskHandle_t scheduled_task_hdl,
                                           thread_t *calling_thread)
{
    যদি (calling_thread != রিক্ত) {
        ঘূৰাই false;
    }

    thread_t *scheduled_thread = linux_port_get_thread_from_handle(scheduled_task_hdl);
    যদি (scheduled_thread == রিক্ত) {
        linux_port_switch_context(scheduled_task_hdl);
    }
    pthread_mutex_unlock(&s_port_mutex);
    ঘূৰাই true;
}

শূন্য vPortYield(শূন্য)
{
    pthread_mutex_lock(&s_port_mutex);

    thread_t *calling_thread = linux_port_get_calling_thread();
    TaskHandle_t scheduled_task_hdl = xTaskGetCurrentTaskHandle();

    যদি (linux_port_handle_deleted_task(scheduled_task_hdl, calling_thread)) {
        ঘূৰাই;
    }

    /* If in a critical section, defer the yield until the section exits. */
    যদি (s_ux_critical_nesting != 0) {
        calling_thread->yield_needed = true;
        pthread_mutex_unlock(&s_port_mutex);
        ঘূৰাই;
    }

    /* Hand the CPU to the next ready task right now (mimics PendSV on real
     * hardware). */
    linux_port_switch_context(scheduled_task_hdl);
    pthread_mutex_unlock(&s_port_mutex);

    /* If the newly scheduled task is different from the calling thread,
     * block until the scheduler resumes this task. */
    TaskHandle_t next_task_hdl = xTaskGetCurrentTaskHandle();
    thread_t *next_thread = linux_port_get_thread_from_handle(next_task_hdl);
    যদি (calling_thread != next_thread) {
        linux_port_block_thread(calling_thread);
    }
}

শূন্য vPortYieldWithinApi(শূন্য)
{
    vPortYield();
}

শূন্য vPortSuspendScheduler(শূন্য)
{
    /* scheduled out task trying to suspend the scheduler should get blocked here */
    pthread_mutex_lock(&s_port_mutex);

   /* get the metadata of the pthread calling this function */
    thread_t *calling_thread = linux_port_get_calling_thread();

    /* get the thread metadata from the scheduled task */
    TaskHandle_t scheduled_task_hdl = xTaskGetCurrentTaskHandle();

    যদি (linux_port_handle_deleted_task(scheduled_task_hdl, calling_thread)) {
        ঘূৰাই;
    }

    thread_t *scheduled_thread = linux_port_get_thread_from_handle(scheduled_task_hdl);
    যদি (calling_thread != scheduled_thread) {
        pthread_mutex_unlock(&s_port_mutex);
        vPortYield();
        ঘূৰাই;
    }
    pthread_mutex_unlock(&s_port_mutex);
}

শূন্য vPortDisableInterrupts(শূন্য)
{
    vPortEnterCritical();
}

শূন্য vPortEnableInterrupts(শূন্য)
{
    vPortExitCritical();
}

BaseType_t xPortSetInterruptMask(শূন্য)
{
    vPortEnterCritical();
    ঘূৰাই pdTRUE;
}

শূন্য vPortClearInterruptMask(BaseType_t xMask)
{
    vPortExitCritical();
}

শূন্য vPortThreadDying(শূন্য *pxTaskToDelete, চঞ্চল BaseType_t *pxPendYield)
{
    pthread_mutex_lock(&s_port_mutex);

    thread_t *thread = linux_port_get_thread_from_handle((TaskHandle_t)pxTaskToDelete);
    যদি (thread == রিক্ত) {
        pthread_mutex_unlock(&s_port_mutex);
        ঘূৰাই;
    }

    /* Mark the thread as dying, cancel the thread. the pthread
     * will be stopped on next cancellation point. Do not remove the
     * thread item from the list since it will be done in vPortCancelThread */
    thread->is_dying = true;
    pthread_cancel(thread->pthread);

    pthread_mutex_unlock(&s_port_mutex);
}

#যদি CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
স্থির শূন্য vPortTLSPointersDelCb(শূন্য *pxTCB)
{
    StaticTask_t *tcb = (StaticTask_t *)pxTCB;
    TlsDeleteCallbackFunction_t *pvDelCbs = (TlsDeleteCallbackFunction_t *)(&tcb->pvDummy15[config_অঙ্ক_THREAD_LOCAL_STORAGE_POINTERS / 2]);

    বাবে (পূর্ণাঙ্ক x = 0; x < (config_অঙ্ক_THREAD_LOCAL_STORAGE_POINTERS / 2); x++) {
        যদি (pvDelCbs[x] != রিক্ত) {
            pvDelCbs[x](x, tcb->pvDummy15[x]);
        }
    }
}
#endif /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

শূন্য vPortCancelThread(শূন্য *pxTaskToDelete)
{
    pthread_mutex_lock(&s_port_mutex);

#যদি CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
    vPortTLSPointersDelCb(pxTaskToDelete);
#endif

    thread_t *thread = linux_port_get_thread_from_handle((TaskHandle_t)pxTaskToDelete);
    যদি (!thread) {
        pthread_mutex_unlock(&s_port_mutex);
        ঘূৰাই;
    }

    যদি (thread->is_dying) {
        /* vPortThreadDying already called */
    } অন্যথা {
        thread->is_dying = true;
        pthread_cancel(thread->pthread);
    }

    /* Save fields and unregister while holding the lock. */
    pthread_t pt = thread->pthread;
    event_t *ev = thread->ev;
    linux_port_unregister_thread((TaskHandle_t)pxTaskToDelete);

    /* Release the mutex before joining – the dying thread may need the
     * scheduler (which also takes s_port_mutex) to reach a cancellation
     * point. */
    pthread_mutex_unlock(&s_port_mutex);

    pthread_join(pt, রিক্ত);
    event_delete(ev);
    মুক্ত(thread);
}

শূন্য v_Port_Set_স্তূপ_Watchpoint(শূন্য *px_স্তূপ_Start)
{
}

#যদি ( CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS )
configRUN_TIME_COUNTER_TYPE x_Port_Get_Run_Time_Counter_মান( শূন্য )
{
    সংরচনা timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);

    ঘূৰাই ( configRUN_TIME_COUNTER_TYPE ) ( ( ( uint64_t ) ts.tv_sec * 1000000ULL ) +
                                            ( ( uint64_t ) ts.tv_nsec / 1000ULL ) );
}
#endif /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
