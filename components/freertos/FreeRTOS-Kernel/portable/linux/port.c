/*
 * SPDX-FileCopyrightText: 2025-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <pthread.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/queue.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "string.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils/wait_for_event.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_private/freertos_linux_coop_syscalls.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils/linux_port_utils.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ FREERTOS_SIM_TICK_PERIOD_US (1000000 / CONFIG_FREERTOS_HZ)

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ thread {
    ਅਚਲ ਅੱਖਰ *name;
    pthread_t pthread;
    TaskFunction_t px_ਕੋਡ;
    ਖਾਲੀ *pvParams;
    bool is_dying;
    bool yield_needed;
    ਬਣਤਰ event *ev;
} thread_t;

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ task_thread_ਗੰਢ {
    TaskHandle_t handle;
    thread_t *thread;
    SLIST_ENTRY(task_thread_ਗੰਢ) ਅਗਲਾ;
} task_thread_ਗੰਢ_t;


ਸਥਿਰ SLIST_HEAD(task_thread_ਗੰਢ_ll, task_thread_ਗੰਢ) s_task_thread_list = SLIST_HEAD_INITIALIZER(task_thread_ਗੰਢ);
ਸਥਿਰ pthread_mutex_t s_thread_map_mutex = PTHREAD_MUTEX_INITIALIZER;

ਸਥਿਰ pthread_mutex_t s_ਪੋਰਟ_mutex;
ਸਥਿਰ pthread_t s_scheduler_thread;
ਸਥਿਰ bool s_scheduler_started = false;
ਸਥਿਰ ਪੂਰਨ s_ux_critical_nesting = 0;

/* TLS flag: true only when inside a real FreeRTOS task pthread */
ਸਥਿਰ __thread bool s_ਵਿੱਚ_freertos_task = false;

bool linux_ਪੋਰਟ_ਵਿੱਚ_freertos_task(ਖਾਲੀ)
{
    ਵਾਪਸ s_ਵਿੱਚ_freertos_task;
}

ਸਥਿਰ ਖਾਲੀ linux_ਪੋਰਟ_initialize_mutexes(ਖਾਲੀ)
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&s_ਪੋਰਟ_mutex, &attr);
    pthread_mutexattr_destroy(&attr);
}

ਸਥਿਰ ਖਾਲੀ linux_ਪੋਰਟ_fatal_error(ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ, ਪੂਰਨ err)
{
    fprintf(stderr, "%s: %s\n", ਸੁਨੇਹਾ, strerror(err));
    abort();
}

ਸਥਿਰ ਖਾਲੀ linux_ਪੋਰਟ_ਰਜਿਸਟਰ_thread(TaskHandle_t handle, thread_t *thread)
{
    ਜੇ (handle == ਨਿਰਾ) {
        ਵਾਪਸ;
    }

    task_thread_ਗੰਢ_t *ਗੰਢ = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(task_thread_ਗੰਢ_t));
    ਜੇ (!ਗੰਢ) {
        linux_ਪੋਰਟ_fatal_error("Failed to allocate thread map node", -1);
    }

    ਗੰਢ->handle = handle;
    ਗੰਢ->thread = thread;

    pthread_mutex_lock(&s_thread_map_mutex);
    SLIST_INSERT_HEAD(&s_task_thread_list, ਗੰਢ, ਅਗਲਾ);
    pthread_mutex_unlock(&s_thread_map_mutex);
}

ਸਥਿਰ ਖਾਲੀ linux_ਪੋਰਟ_unregister_thread(TaskHandle_t handle)
{
    ਜੇ (handle == ਨਿਰਾ) {
        ਵਾਪਸ;
    }

    pthread_mutex_lock(&s_thread_map_mutex);

    task_thread_ਗੰਢ_t *cur_ਗੰਢ = SLIST_FIRST(&s_task_thread_list);
    task_thread_ਗੰਢ_t *prev_ਗੰਢ = ਨਿਰਾ;

    ਜਦੋਂ (cur_ਗੰਢ) {
        ਜੇ (cur_ਗੰਢ->handle == handle) {
            ਜੇ (prev_ਗੰਢ) {
                prev_ਗੰਢ->ਅਗਲਾ.sle_ਅਗਲਾ = SLIST_NEXT(cur_ਗੰਢ, ਅਗਲਾ);
            } ਨਹੀਂਤਾਂ {
                SLIST_REMOVE_HEAD(&s_task_thread_list, ਅਗਲਾ);
            }

            ਮੁਕਤ(cur_ਗੰਢ);
            pthread_mutex_unlock(&s_thread_map_mutex);
            ਵਾਪਸ;
        }

        prev_ਗੰਢ = cur_ਗੰਢ;
        cur_ਗੰਢ = SLIST_NEXT(cur_ਗੰਢ, ਅਗਲਾ);
    }

    pthread_mutex_unlock(&s_thread_map_mutex);
}

ਸਥਿਰ thread_t *linux_ਪੋਰਟ_get_thread_from_handle(TaskHandle_t handle)
{
    ਜੇ (handle == ਨਿਰਾ) {
        ਵਾਪਸ ਨਿਰਾ;
    }

    pthread_mutex_lock(&s_thread_map_mutex);
    task_thread_ਗੰਢ_t *ਗੰਢ = ਨਿਰਾ;
    SLIST_FOREACH(ਗੰਢ, &s_task_thread_list, ਅਗਲਾ) {
        ਜੇ (ਗੰਢ->handle == (TaskHandle_t)(*(ਢੇਰ_Type_t **)(handle))) {
            thread_t *t = ਗੰਢ->thread;
            pthread_mutex_unlock(&s_thread_map_mutex);
            ਵਾਪਸ t;
        }
    }
    pthread_mutex_unlock(&s_thread_map_mutex);
    ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ thread_t *linux_ਪੋਰਟ_get_calling_thread(ਖਾਲੀ)
{
    pthread_t self = pthread_self();
    pthread_mutex_lock(&s_thread_map_mutex);
    task_thread_ਗੰਢ_t *ਗੰਢ = ਨਿਰਾ;
    SLIST_FOREACH(ਗੰਢ, &s_task_thread_list, ਅਗਲਾ) {
        ਜੇ (pthread_equal(ਗੰਢ->thread->pthread, self)) {
            thread_t *thread = ਗੰਢ->thread;
            pthread_mutex_unlock(&s_thread_map_mutex);
            ਵਾਪਸ thread;
        }
    }
    pthread_mutex_unlock(&s_thread_map_mutex);
    ਵਾਪਸ ਨਿਰਾ;
}

pthread_t linux_ਪੋਰਟ_get_scheduled_task_pthread(ਖਾਲੀ)
{
    thread_t *thread = linux_ਪੋਰਟ_get_thread_from_handle(xTaskGetCurrentTaskHandle());
    ਵਾਪਸ thread ? thread->pthread : pthread_self();
}

ਸਥਿਰ ਖਾਲੀ *linux_ਪੋਰਟ_task_runner(ਖਾਲੀ *arg)
{
    /* Allow this thread to be cancelled */
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, ਨਿਰਾ);

    /* set the flag showing that this is a freertos task */
    s_ਵਿੱਚ_freertos_task = true;

    /* setup the backtrace signal. ONLY triggered before abort so
     * it will not interfere with the simulation while its running */
    linux_ਪੋਰਟ_ਤਿਆਰੀ_backtrace_signal();

    thread_t *thread = arg;

    /* Block until scheduler signals first time, then run the task body. */
    event_wait(thread->ev);
    thread->px_ਕੋਡ(thread->pvParams);

    ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ ਖਾਲੀ linux_ਪੋਰਟ_unblock_thread(thread_t *thread)
{
    event_signal(thread->ev);
}

ਸਥਿਰ ਖਾਲੀ linux_ਪੋਰਟ_ਖੰਡ_thread(thread_t *thread)
{
    event_wait(thread->ev);
}

ਸਥਿਰ ਖਾਲੀ linux_ਪੋਰਟ_increment_tick(ਖਾਲੀ)
{
    (ਖਾਲੀ)xTaskIncrementTick();
}

ਸਥਿਰ ਖਾਲੀ linux_ਪੋਰਟ_switch_context(TaskHandle_t current_task_hdl)
{
    pthread_mutex_lock(&s_ਪੋਰਟ_mutex);

    thread_t *current_thread = linux_ਪੋਰਟ_get_thread_from_handle(current_task_hdl);

    /* get the task that should be scheduled next */
    vTaskSwitchContext();

    /* get the new task to schedule and the associated thread item */
    TaskHandle_t ਅਗਲਾ_task_hdl = xTaskGetCurrentTaskHandle();
    thread_t *ਅਗਲਾ_thread = linux_ਪੋਰਟ_get_thread_from_handle(ਅਗਲਾ_task_hdl);

    /* unblock the newly scheduled task if it is different from
     * the one already scheduled */
    ਜੇ (ਅਗਲਾ_thread) {
        /* Only unblock the thread if we are actually switching to a
         * different one. Signaling the already-running thread would
         * leave a stale event_triggered flag, causing its next
         * event_wait (e.g. in vPortYield) to return immediately
         * instead of blocking.
         *
         * Exception: on the very first switch, the task is still blocked
         * in its initial event_wait (linux_port_task_runner), so we must
         * signal it even though current_thread == next_thread. */
        ਜੇ (ਅਗਲਾ_thread != current_thread || !s_scheduler_started) {
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
            ਜੇ (current_thread) {
                event_clear(current_thread->ev);
            }
            linux_ਪੋਰਟ_unblock_thread(ਅਗਲਾ_thread);
        }
    }

    ਜੇ (!s_scheduler_started) {
        s_scheduler_started = true;
    }

    /* fill the name of the task in the thread item if not done already. */
    ਜੇ (ਅਗਲਾ_thread && ਅਗਲਾ_thread->name == ਨਿਰਾ) {
        /* fill the name of the thread now */
        ਅਗਲਾ_thread->name = pcTaskGetName(ਅਗਲਾ_task_hdl);
    }

    /* fill the name of the task in the thread item if not done already. */
    ਜੇ (current_thread && current_thread->name == ਨਿਰਾ) {
        /* fill the name of the thread now */
        current_thread->name = pcTaskGetName(current_task_hdl);
    }

    pthread_mutex_unlock(&s_ਪੋਰਟ_mutex);
}

ਸਥਿਰ ਖਾਲੀ *linux_ਪੋਰਟ_scheduler_runner(ਖਾਲੀ *arg)
{
    (ਖਾਲੀ)arg;

    ਜਦੋਂ (1) {
        /* sleep for a period of 1 tick */
        usleep(FREERTOS_SIM_TICK_PERIOD_US);

        /* get the task that is currently scheduled */
        TaskHandle_t current_task_hdl = xTaskGetCurrentTaskHandle();

        /* Lock the port mutex. This will block while any task is in a
         * critical section, ensuring ticks don't preempt critical code. */
        pthread_mutex_lock(&s_ਪੋਰਟ_mutex);

        /* increment the freertos tick */
        linux_ਪੋਰਟ_increment_tick();

        /* schedule a new task, and schedule out the currently running one */
        linux_ਪੋਰਟ_switch_context(current_task_hdl);

        pthread_mutex_unlock(&s_ਪੋਰਟ_mutex);
    }
    ਵਾਪਸ ਨਿਰਾ;
}

ਢੇਰ_Type_t *px_ਪੋਰਟ_Initialise_ਢੇਰ(ਢੇਰ_Type_t *px_ਸਿਖਰ_ਦਾ_ਢੇਰ,
                                   ਢੇਰ_Type_t *px_ਸਿਰਾ_ਦਾ_ਢੇਰ,
                                   TaskFunction_t px_ਕੋਡ,
                                   ਖਾਲੀ *pvParameters)
{
    pthread_attr_t thread_attr;
    size_t thread_ਢੇਰ_ਮਾਪ;

    /* Store the thread data at the start of the stack. */
    thread_ਢੇਰ_ਮਾਪ = (px_ਸਿਖਰ_ਦਾ_ਢੇਰ - px_ਸਿਰਾ_ਦਾ_ਢੇਰ) * ਆਕਾਰ(*px_ਸਿਖਰ_ਦਾ_ਢੇਰ);
    pthread_attr_init(&thread_attr);
    pthread_attr_setstack(&thread_attr, px_ਸਿਰਾ_ਦਾ_ਢੇਰ, thread_ਢੇਰ_ਮਾਪ);

    thread_t *thread = ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਆਕਾਰ(thread_t));
    ਜੇ (!thread) {
        linux_ਪੋਰਟ_fatal_error("Failed to allocate thread metadata", -1);
    }
    thread->name = ਨਿਰਾ; // this will be filled later when we know about the task name
    thread->px_ਕੋਡ = px_ਕੋਡ;
    thread->pvParams = pvParameters;
    thread->is_dying = false;
    thread->yield_needed = false;
    thread->ev = event_create();

    linux_ਪੋਰਟ_ਰਜਿਸਟਰ_thread((TaskHandle_t)px_ਸਿਖਰ_ਦਾ_ਢੇਰ, thread);

    /* create the thread associated with the task being created */
    ਅਚਲ ਪੂਰਨ ret = pthread_create(&thread->pthread, &thread_attr, linux_ਪੋਰਟ_task_runner, thread);
    ਜੇ (ret != 0) {
        linux_ਪੋਰਟ_fatal_error("pthread_create", ret);
    }
    ਵਾਪਸ px_ਸਿਖਰ_ਦਾ_ਢੇਰ;
}

ਅਧਾਰ_Type_t x_ਪੋਰਟ_ਸ਼ੁਰੂ_Scheduler(ਖਾਲੀ)
{
    /* set the port mutex to be recursive. Must be done before
     * vPortEnableInterrupts() which calls vPortExitCritical(). */
    linux_ਪੋਰਟ_initialize_mutexes();

    /* enable interrupt that were disabled in vTaskStartScheduler */
    v_ਪੋਰਟ_Enable_Interrupts();

    /* init the cooperative syscall layer (sets stdio non-blocking).
     * Provided by VFS component; weak no-op when VFS is not linked. */
    freertos_linux_coop_syscalls_init();

    /* Start scheduler thread */
    ਪੂਰਨ ret = pthread_create(&s_scheduler_thread, ਨਿਰਾ, linux_ਪੋਰਟ_scheduler_runner, ਨਿਰਾ);
    ਜੇ (ret != 0) {
       linux_ਪੋਰਟ_fatal_error("pthread_create", ret);
    }

    /* Should never return */
    pthread_join(s_scheduler_thread, ਨਿਰਾ);

    ਵਾਪਸ 0;
}

ਖਾਲੀ v_ਪੋਰਟ_ਸਿਰਾ_Scheduler(ਖਾਲੀ)
{
    ਬਾਹਰ_ਨਿਕਲੋ(0);
}

ਖਾਲੀ v_ਪੋਰਟ_Enter_Critical(ਖਾਲੀ)
{
    ਜੇ (!s_scheduler_started) {
        ਵਾਪਸ;
    }

    pthread_mutex_lock(&s_ਪੋਰਟ_mutex);

    /* Non-FreeRTOS thread or recursive enter: just bump the counter.
     * The mutex is already held (recursive lock succeeds for same thread). */
    ਜੇ (!linux_ਪੋਰਟ_ਵਿੱਚ_freertos_task() || s_ux_critical_nesting > 0) {
        s_ux_critical_nesting++;
        ਵਾਪਸ;
    }

    /* First enter from a FreeRTOS task: ensure we're the scheduled task.
     * If not, release the mutex, block until the scheduler switches to us,
     * then re-acquire. */
    thread_t *calling_thread = linux_ਪੋਰਟ_get_calling_thread();
    thread_t *scheduled_thread = linux_ਪੋਰਟ_get_thread_from_handle(xTaskGetCurrentTaskHandle());

    ਜਦੋਂ (calling_thread && !calling_thread->is_dying && calling_thread != scheduled_thread) {
        pthread_mutex_unlock(&s_ਪੋਰਟ_mutex);
        linux_ਪੋਰਟ_ਖੰਡ_thread(calling_thread);
        pthread_mutex_lock(&s_ਪੋਰਟ_mutex);
        calling_thread = linux_ਪੋਰਟ_get_calling_thread();
        scheduled_thread = linux_ਪੋਰਟ_get_thread_from_handle(xTaskGetCurrentTaskHandle());
    }

    ਜੇ (!calling_thread || calling_thread->is_dying) {
        linux_ਪੋਰਟ_switch_context(xTaskGetCurrentTaskHandle());
        pthread_mutex_unlock(&s_ਪੋਰਟ_mutex);
        ਵਾਪਸ;
    }

    s_ux_critical_nesting = 1;
}

ਖਾਲੀ v_ਪੋਰਟ_Exit_Critical(ਖਾਲੀ)
{
    ਜੇ (!s_scheduler_started || s_ux_critical_nesting == 0) {
        ਵਾਪਸ;
    }

    s_ux_critical_nesting--;

    /* Check for deferred yield on final exit from a FreeRTOS task */
    ਜੇ (s_ux_critical_nesting == 0 && linux_ਪੋਰਟ_ਵਿੱਚ_freertos_task()) {
        thread_t *calling_thread = linux_ਪੋਰਟ_get_calling_thread();
        ਜੇ (calling_thread && calling_thread->yield_needed) {
            calling_thread->yield_needed = false;
            pthread_mutex_unlock(&s_ਪੋਰਟ_mutex);
            v_ਪੋਰਟ_Yield();
            ਵਾਪਸ;
        }
    }

    pthread_mutex_unlock(&s_ਪੋਰਟ_mutex);
}

/* Handle the case where the calling pthread is not a registered FreeRTOS task.
 * If the calling thread has been deleted but is still the scheduled task,
 * perform a context switch. Otherwise just release the mutex and return.
 * Returns true if the caller should return early. */
ਸਥਿਰ bool linux_ਪੋਰਟ_handle_deleted_task(TaskHandle_t scheduled_task_hdl,
                                           thread_t *calling_thread)
{
    ਜੇ (calling_thread != ਨਿਰਾ) {
        ਵਾਪਸ false;
    }

    thread_t *scheduled_thread = linux_ਪੋਰਟ_get_thread_from_handle(scheduled_task_hdl);
    ਜੇ (scheduled_thread == ਨਿਰਾ) {
        linux_ਪੋਰਟ_switch_context(scheduled_task_hdl);
    }
    pthread_mutex_unlock(&s_ਪੋਰਟ_mutex);
    ਵਾਪਸ true;
}

ਖਾਲੀ v_ਪੋਰਟ_Yield(ਖਾਲੀ)
{
    pthread_mutex_lock(&s_ਪੋਰਟ_mutex);

    thread_t *calling_thread = linux_ਪੋਰਟ_get_calling_thread();
    TaskHandle_t scheduled_task_hdl = xTaskGetCurrentTaskHandle();

    ਜੇ (linux_ਪੋਰਟ_handle_deleted_task(scheduled_task_hdl, calling_thread)) {
        ਵਾਪਸ;
    }

    /* If in a critical section, defer the yield until the section exits. */
    ਜੇ (s_ux_critical_nesting != 0) {
        calling_thread->yield_needed = true;
        pthread_mutex_unlock(&s_ਪੋਰਟ_mutex);
        ਵਾਪਸ;
    }

    /* Hand the CPU to the next ready task right now (mimics PendSV on real
     * hardware). */
    linux_ਪੋਰਟ_switch_context(scheduled_task_hdl);
    pthread_mutex_unlock(&s_ਪੋਰਟ_mutex);

    /* If the newly scheduled task is different from the calling thread,
     * block until the scheduler resumes this task. */
    TaskHandle_t ਅਗਲਾ_task_hdl = xTaskGetCurrentTaskHandle();
    thread_t *ਅਗਲਾ_thread = linux_ਪੋਰਟ_get_thread_from_handle(ਅਗਲਾ_task_hdl);
    ਜੇ (calling_thread != ਅਗਲਾ_thread) {
        linux_ਪੋਰਟ_ਖੰਡ_thread(calling_thread);
    }
}

ਖਾਲੀ v_ਪੋਰਟ_Yield_Within_Api(ਖਾਲੀ)
{
    v_ਪੋਰਟ_Yield();
}

ਖਾਲੀ v_ਪੋਰਟ_Suspend_Scheduler(ਖਾਲੀ)
{
    /* scheduled out task trying to suspend the scheduler should get blocked here */
    pthread_mutex_lock(&s_ਪੋਰਟ_mutex);

   /* get the metadata of the pthread calling this function */
    thread_t *calling_thread = linux_ਪੋਰਟ_get_calling_thread();

    /* get the thread metadata from the scheduled task */
    TaskHandle_t scheduled_task_hdl = xTaskGetCurrentTaskHandle();

    ਜੇ (linux_ਪੋਰਟ_handle_deleted_task(scheduled_task_hdl, calling_thread)) {
        ਵਾਪਸ;
    }

    thread_t *scheduled_thread = linux_ਪੋਰਟ_get_thread_from_handle(scheduled_task_hdl);
    ਜੇ (calling_thread != scheduled_thread) {
        pthread_mutex_unlock(&s_ਪੋਰਟ_mutex);
        v_ਪੋਰਟ_Yield();
        ਵਾਪਸ;
    }
    pthread_mutex_unlock(&s_ਪੋਰਟ_mutex);
}

ਖਾਲੀ v_ਪੋਰਟ_Disable_Interrupts(ਖਾਲੀ)
{
    v_ਪੋਰਟ_Enter_Critical();
}

ਖਾਲੀ v_ਪੋਰਟ_Enable_Interrupts(ਖਾਲੀ)
{
    v_ਪੋਰਟ_Exit_Critical();
}

ਅਧਾਰ_Type_t x_ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ(ਖਾਲੀ)
{
    v_ਪੋਰਟ_Enter_Critical();
    ਵਾਪਸ pdTRUE;
}

ਖਾਲੀ v_ਪੋਰਟ_Clear_ਰੁਕਾਵਟ_ਨਕਾਬ(ਅਧਾਰ_Type_t x_ਨਕਾਬ)
{
    v_ਪੋਰਟ_Exit_Critical();
}

ਖਾਲੀ v_ਪੋਰਟ_Thread_Dying(ਖਾਲੀ *px_Task_ਮੰਜ਼ਿਲ_Delete, ਅਸਥਿਰ ਅਧਾਰ_Type_t *pxPendYield)
{
    pthread_mutex_lock(&s_ਪੋਰਟ_mutex);

    thread_t *thread = linux_ਪੋਰਟ_get_thread_from_handle((TaskHandle_t)px_Task_ਮੰਜ਼ਿਲ_Delete);
    ਜੇ (thread == ਨਿਰਾ) {
        pthread_mutex_unlock(&s_ਪੋਰਟ_mutex);
        ਵਾਪਸ;
    }

    /* Mark the thread as dying, cancel the thread. the pthread
     * will be stopped on next cancellation point. Do not remove the
     * thread item from the list since it will be done in vPortCancelThread */
    thread->is_dying = true;
    pthread_cancel(thread->pthread);

    pthread_mutex_unlock(&s_ਪੋਰਟ_mutex);
}

#ਜੇ CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
ਸਥਿਰ ਖਾਲੀ v_ਪੋਰਟ_TLSPointers_Del_Cb(ਖਾਲੀ *pxTCB)
{
    StaticTask_t *tcb = (StaticTask_t *)pxTCB;
    TlsDeleteCallbackFunction_t *pvDelCbs = (TlsDeleteCallbackFunction_t *)(&tcb->pvDummy15[config_ਅੰਕ_THREAD_LOCAL_STORAGE_ਸੂਚਕ_ਸਾਰੇ / 2]);

    ਲਈ (ਪੂਰਨ x = 0; x < (config_ਅੰਕ_THREAD_LOCAL_STORAGE_ਸੂਚਕ_ਸਾਰੇ / 2); x++) {
        ਜੇ (pvDelCbs[x] != ਨਿਰਾ) {
            pvDelCbs[x](x, tcb->pvDummy15[x]);
        }
    }
}
#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

ਖਾਲੀ v_ਪੋਰਟ_Cancel_Thread(ਖਾਲੀ *px_Task_ਮੰਜ਼ਿਲ_Delete)
{
    pthread_mutex_lock(&s_ਪੋਰਟ_mutex);

#ਜੇ CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
    v_ਪੋਰਟ_TLSPointers_Del_Cb(px_Task_ਮੰਜ਼ਿਲ_Delete);
#ਜੇ_ਅੰਤ

    thread_t *thread = linux_ਪੋਰਟ_get_thread_from_handle((TaskHandle_t)px_Task_ਮੰਜ਼ਿਲ_Delete);
    ਜੇ (!thread) {
        pthread_mutex_unlock(&s_ਪੋਰਟ_mutex);
        ਵਾਪਸ;
    }

    ਜੇ (thread->is_dying) {
        /* vPortThreadDying already called */
    } ਨਹੀਂਤਾਂ {
        thread->is_dying = true;
        pthread_cancel(thread->pthread);
    }

    /* Save fields and unregister while holding the lock. */
    pthread_t pt = thread->pthread;
    event_t *ev = thread->ev;
    linux_ਪੋਰਟ_unregister_thread((TaskHandle_t)px_Task_ਮੰਜ਼ਿਲ_Delete);

    /* Release the mutex before joining – the dying thread may need the
     * scheduler (which also takes s_port_mutex) to reach a cancellation
     * point. */
    pthread_mutex_unlock(&s_ਪੋਰਟ_mutex);

    pthread_join(pt, ਨਿਰਾ);
    event_delete(ev);
    ਮੁਕਤ(thread);
}

ਖਾਲੀ v_ਪੋਰਟ_ਸੈੱਟ_ਢੇਰ_Watchpoint(ਖਾਲੀ *px_ਢੇਰ_ਸ਼ੁਰੂ)
{
}

#ਜੇ ( CONFIG_FREERTOS_GENERATE_ਲੜੀਵਾਰ_ਸਮਾਂ_STATS )
config_ਲੜੀਵਾਰ_ਸਮਾਂ_COUNTER_TYPE x_ਪੋਰਟ_Get_ਲੜੀਵਾਰ_ਸਮਾਂ_Counter_ਮੁੱਲ( ਖਾਲੀ )
{
    ਬਣਤਰ timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);

    ਵਾਪਸ ( config_ਲੜੀਵਾਰ_ਸਮਾਂ_COUNTER_TYPE ) ( ( ( uint64_t ) ts.tv_sec * 1000000ULL ) +
                                            ( ( uint64_t ) ts.tv_nsec / 1000ULL ) );
}
#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
