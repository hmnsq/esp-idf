/*
 * SPDX-FileCopyrightText: 2025-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <pthread.h>
#ಅನ್ತರ್ಭಾವ <sys/queue.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಅನ್ತರ್ಭಾವ "string.h"
#ಅನ್ತರ್ಭಾವ "FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "task.h"
#ಅನ್ತರ್ಭಾವ "utils/wait_for_event.h"
#ಅನ್ತರ್ಭಾವ "esp_private/freertos_linux_coop_syscalls.h"
#ಅನ್ತರ್ಭಾವ "utils/linux_port_utils.h"

#ವ್ಯಾಖ್ಯಾನಿಸು FREERTOS_SIM_TICK_PERIOD_US (1000000 / CONFIG_FREERTOS_HZ)

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ thread {
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
    pthread_t pthread;
    TaskFunction_t px_ಸಂಕೇತ;
    ಶೂನ್ಯ *pvParams;
    bool is_dying;
    bool yield_needed;
    ರಚನೆ event *ev;
} thread_t;

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ task_thread_ಗಂಟು {
    TaskHandle_t handle;
    thread_t *thread;
    SLIST_ENTRY(task_thread_ಗಂಟು) ಮುಂದಿನ;
} task_thread_ಗಂಟು_t;


ಸ್ಥಿರ SLIST_HEAD(task_thread_ಗಂಟು_ll, task_thread_ಗಂಟು) s_task_thread_list = SLIST_HEAD_INITIALIZER(task_thread_ಗಂಟು);
ಸ್ಥಿರ pthread_mutex_t s_thread_map_mutex = PTHREAD_MUTEX_INITIALIZER;

ಸ್ಥಿರ pthread_mutex_t s_ಪೋರ್ಟ್_mutex;
ಸ್ಥಿರ pthread_t s_scheduler_thread;
ಸ್ಥಿರ bool s_scheduler_started = false;
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ s_ux_critical_nesting = 0;

/* TLS flag: true only when inside a real FreeRTOS task pthread */
ಸ್ಥಿರ __thread bool s_ಒಳ_freertos_task = false;

bool linux_ಪೋರ್ಟ್_ಒಳ_freertos_task(ಶೂನ್ಯ)
{
    ಹಿಂತಿರುಗಿ s_ಒಳ_freertos_task;
}

ಸ್ಥಿರ ಶೂನ್ಯ linux_ಪೋರ್ಟ್_initialize_mutexes(ಶೂನ್ಯ)
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&s_ಪೋರ್ಟ್_mutex, &attr);
    pthread_mutexattr_destroy(&attr);
}

ಸ್ಥಿರ ಶೂನ್ಯ linux_ಪೋರ್ಟ್_fatal_error(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ, ಪೂರ್ಣಾಂಕ err)
{
    fprintf(stderr, "%s: %s\n", ಸಂದೇಶ, strerror(err));
    abort();
}

ಸ್ಥಿರ ಶೂನ್ಯ linux_ಪೋರ್ಟ್_ರಿಜಿಸ್ಟರ್_thread(TaskHandle_t handle, thread_t *thread)
{
    ಆದರೆ (handle == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ;
    }

    task_thread_ಗಂಟು_t *ಗಂಟು = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(task_thread_ಗಂಟು_t));
    ಆದರೆ (!ಗಂಟು) {
        linux_ಪೋರ್ಟ್_fatal_error("Failed to allocate thread map node", -1);
    }

    ಗಂಟು->handle = handle;
    ಗಂಟು->thread = thread;

    pthread_mutex_lock(&s_thread_map_mutex);
    SLIST_INSERT_HEAD(&s_task_thread_list, ಗಂಟು, ಮುಂದಿನ);
    pthread_mutex_unlock(&s_thread_map_mutex);
}

ಸ್ಥಿರ ಶೂನ್ಯ linux_ಪೋರ್ಟ್_unregister_thread(TaskHandle_t handle)
{
    ಆದರೆ (handle == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ;
    }

    pthread_mutex_lock(&s_thread_map_mutex);

    task_thread_ಗಂಟು_t *cur_ಗಂಟು = SLIST_FIRST(&s_task_thread_list);
    task_thread_ಗಂಟು_t *prev_ಗಂಟು = ಖಾಲಿ;

    ಆಗ (cur_ಗಂಟು) {
        ಆದರೆ (cur_ಗಂಟು->handle == handle) {
            ಆದರೆ (prev_ಗಂಟು) {
                prev_ಗಂಟು->ಮುಂದಿನ.sle_ಮುಂದಿನ = SLIST_NEXT(cur_ಗಂಟು, ಮುಂದಿನ);
            } ಇಲ್ಲದಿದ್ದರೆ {
                SLIST_REMOVE_HEAD(&s_task_thread_list, ಮುಂದಿನ);
            }

            ಬಿಡುಗಡೆ(cur_ಗಂಟು);
            pthread_mutex_unlock(&s_thread_map_mutex);
            ಹಿಂತಿರುಗಿ;
        }

        prev_ಗಂಟು = cur_ಗಂಟು;
        cur_ಗಂಟು = SLIST_NEXT(cur_ಗಂಟು, ಮುಂದಿನ);
    }

    pthread_mutex_unlock(&s_thread_map_mutex);
}

ಸ್ಥಿರ thread_t *linux_ಪೋರ್ಟ್_get_thread_from_handle(TaskHandle_t handle)
{
    ಆದರೆ (handle == ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    pthread_mutex_lock(&s_thread_map_mutex);
    task_thread_ಗಂಟು_t *ಗಂಟು = ಖಾಲಿ;
    SLIST_FOREACH(ಗಂಟು, &s_task_thread_list, ಮುಂದಿನ) {
        ಆದರೆ (ಗಂಟು->handle == (TaskHandle_t)(*(ರಾಶಿ_Type_t **)(handle))) {
            thread_t *t = ಗಂಟು->thread;
            pthread_mutex_unlock(&s_thread_map_mutex);
            ಹಿಂತಿರುಗಿ t;
        }
    }
    pthread_mutex_unlock(&s_thread_map_mutex);
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ thread_t *linux_ಪೋರ್ಟ್_get_calling_thread(ಶೂನ್ಯ)
{
    pthread_t self = pthread_self();
    pthread_mutex_lock(&s_thread_map_mutex);
    task_thread_ಗಂಟು_t *ಗಂಟು = ಖಾಲಿ;
    SLIST_FOREACH(ಗಂಟು, &s_task_thread_list, ಮುಂದಿನ) {
        ಆದರೆ (pthread_equal(ಗಂಟು->thread->pthread, self)) {
            thread_t *thread = ಗಂಟು->thread;
            pthread_mutex_unlock(&s_thread_map_mutex);
            ಹಿಂತಿರುಗಿ thread;
        }
    }
    pthread_mutex_unlock(&s_thread_map_mutex);
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

pthread_t linux_ಪೋರ್ಟ್_get_scheduled_task_pthread(ಶೂನ್ಯ)
{
    thread_t *thread = linux_ಪೋರ್ಟ್_get_thread_from_handle(xTaskGetCurrentTaskHandle());
    ಹಿಂತಿರುಗಿ thread ? thread->pthread : pthread_self();
}

ಸ್ಥಿರ ಶೂನ್ಯ *linux_ಪೋರ್ಟ್_task_runner(ಶೂನ್ಯ *arg)
{
    /* Allow this thread to be cancelled */
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, ಖಾಲಿ);

    /* set the flag showing that this is a freertos task */
    s_ಒಳ_freertos_task = true;

    /* setup the backtrace signal. ONLY triggered before abort so
     * it will not interfere with the simulation while its running */
    linux_ಪೋರ್ಟ್_ಸಿದ್ಧತೆ_backtrace_signal();

    thread_t *thread = arg;

    /* Block until scheduler signals first time, then run the task body. */
    event_wait(thread->ev);
    thread->px_ಸಂಕೇತ(thread->pvParams);

    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ ಶೂನ್ಯ linux_ಪೋರ್ಟ್_unblock_thread(thread_t *thread)
{
    event_signal(thread->ev);
}

ಸ್ಥಿರ ಶೂನ್ಯ linux_ಪೋರ್ಟ್_ಖಂಡ_thread(thread_t *thread)
{
    event_wait(thread->ev);
}

ಸ್ಥಿರ ಶೂನ್ಯ linux_ಪೋರ್ಟ್_increment_tick(ಶೂನ್ಯ)
{
    (ಶೂನ್ಯ)xTaskIncrementTick();
}

ಸ್ಥಿರ ಶೂನ್ಯ linux_ಪೋರ್ಟ್_switch_context(TaskHandle_t current_task_hdl)
{
    pthread_mutex_lock(&s_ಪೋರ್ಟ್_mutex);

    thread_t *current_thread = linux_ಪೋರ್ಟ್_get_thread_from_handle(current_task_hdl);

    /* get the task that should be scheduled next */
    vTaskSwitchContext();

    /* get the new task to schedule and the associated thread item */
    TaskHandle_t ಮುಂದಿನ_task_hdl = xTaskGetCurrentTaskHandle();
    thread_t *ಮುಂದಿನ_thread = linux_ಪೋರ್ಟ್_get_thread_from_handle(ಮುಂದಿನ_task_hdl);

    /* unblock the newly scheduled task if it is different from
     * the one already scheduled */
    ಆದರೆ (ಮುಂದಿನ_thread) {
        /* Only unblock the thread if we are actually switching to a
         * different one. Signaling the already-running thread would
         * leave a stale event_triggered flag, causing its next
         * event_wait (e.g. in vPortYield) to return immediately
         * instead of blocking.
         *
         * Exception: on the very first switch, the task is still blocked
         * in its initial event_wait (linux_port_task_runner), so we must
         * signal it even though current_thread == next_thread. */
        ಆದರೆ (ಮುಂದಿನ_thread != current_thread || !s_scheduler_started) {
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
            ಆದರೆ (current_thread) {
                event_clear(current_thread->ev);
            }
            linux_ಪೋರ್ಟ್_unblock_thread(ಮುಂದಿನ_thread);
        }
    }

    ಆದರೆ (!s_scheduler_started) {
        s_scheduler_started = true;
    }

    /* fill the name of the task in the thread item if not done already. */
    ಆದರೆ (ಮುಂದಿನ_thread && ಮುಂದಿನ_thread->name == ಖಾಲಿ) {
        /* fill the name of the thread now */
        ಮುಂದಿನ_thread->name = pcTaskGetName(ಮುಂದಿನ_task_hdl);
    }

    /* fill the name of the task in the thread item if not done already. */
    ಆದರೆ (current_thread && current_thread->name == ಖಾಲಿ) {
        /* fill the name of the thread now */
        current_thread->name = pcTaskGetName(current_task_hdl);
    }

    pthread_mutex_unlock(&s_ಪೋರ್ಟ್_mutex);
}

ಸ್ಥಿರ ಶೂನ್ಯ *linux_ಪೋರ್ಟ್_scheduler_runner(ಶೂನ್ಯ *arg)
{
    (ಶೂನ್ಯ)arg;

    ಆಗ (1) {
        /* sleep for a period of 1 tick */
        usleep(FREERTOS_SIM_TICK_PERIOD_US);

        /* get the task that is currently scheduled */
        TaskHandle_t current_task_hdl = xTaskGetCurrentTaskHandle();

        /* Lock the port mutex. This will block while any task is in a
         * critical section, ensuring ticks don't preempt critical code. */
        pthread_mutex_lock(&s_ಪೋರ್ಟ್_mutex);

        /* increment the freertos tick */
        linux_ಪೋರ್ಟ್_increment_tick();

        /* schedule a new task, and schedule out the currently running one */
        linux_ಪೋರ್ಟ್_switch_context(current_task_hdl);

        pthread_mutex_unlock(&s_ಪೋರ್ಟ್_mutex);
    }
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ರಾಶಿ_Type_t *px_ಪೋರ್ಟ್_Initialise_ರಾಶಿ(ರಾಶಿ_Type_t *px_ತುದಿ_ದ_ರಾಶಿ,
                                   ರಾಶಿ_Type_t *px_ಕೊನೆ_ದ_ರಾಶಿ,
                                   TaskFunction_t px_ಸಂಕೇತ,
                                   ಶೂನ್ಯ *pvParameters)
{
    pthread_attr_t thread_attr;
    size_t thread_ರಾಶಿ_ಗಾತ್ರ;

    /* Store the thread data at the start of the stack. */
    thread_ರಾಶಿ_ಗಾತ್ರ = (px_ತುದಿ_ದ_ರಾಶಿ - px_ಕೊನೆ_ದ_ರಾಶಿ) * ಗಾತ್ರ(*px_ತುದಿ_ದ_ರಾಶಿ);
    pthread_attr_init(&thread_attr);
    pthread_attr_setstack(&thread_attr, px_ಕೊನೆ_ದ_ರಾಶಿ, thread_ರಾಶಿ_ಗಾತ್ರ);

    thread_t *thread = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ(thread_t));
    ಆದರೆ (!thread) {
        linux_ಪೋರ್ಟ್_fatal_error("Failed to allocate thread metadata", -1);
    }
    thread->name = ಖಾಲಿ; // this will be filled later when we know about the task name
    thread->px_ಸಂಕೇತ = px_ಸಂಕೇತ;
    thread->pvParams = pvParameters;
    thread->is_dying = false;
    thread->yield_needed = false;
    thread->ev = event_create();

    linux_ಪೋರ್ಟ್_ರಿಜಿಸ್ಟರ್_thread((TaskHandle_t)px_ತುದಿ_ದ_ರಾಶಿ, thread);

    /* create the thread associated with the task being created */
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ ret = pthread_create(&thread->pthread, &thread_attr, linux_ಪೋರ್ಟ್_task_runner, thread);
    ಆದರೆ (ret != 0) {
        linux_ಪೋರ್ಟ್_fatal_error("pthread_create", ret);
    }
    ಹಿಂತಿರುಗಿ px_ತುದಿ_ದ_ರಾಶಿ;
}

ತಳ_Type_t x_ಪೋರ್ಟ್_ಆರಂಭ_Scheduler(ಶೂನ್ಯ)
{
    /* set the port mutex to be recursive. Must be done before
     * vPortEnableInterrupts() which calls vPortExitCritical(). */
    linux_ಪೋರ್ಟ್_initialize_mutexes();

    /* enable interrupt that were disabled in vTaskStartScheduler */
    v_ಪೋರ್ಟ್_Enable_Interrupts();

    /* init the cooperative syscall layer (sets stdio non-blocking).
     * Provided by VFS component; weak no-op when VFS is not linked. */
    freertos_linux_coop_syscalls_init();

    /* Start scheduler thread */
    ಪೂರ್ಣಾಂಕ ret = pthread_create(&s_scheduler_thread, ಖಾಲಿ, linux_ಪೋರ್ಟ್_scheduler_runner, ಖಾಲಿ);
    ಆದರೆ (ret != 0) {
       linux_ಪೋರ್ಟ್_fatal_error("pthread_create", ret);
    }

    /* Should never return */
    pthread_join(s_scheduler_thread, ಖಾಲಿ);

    ಹಿಂತಿರುಗಿ 0;
}

ಶೂನ್ಯ v_ಪೋರ್ಟ್_ಕೊನೆ_Scheduler(ಶೂನ್ಯ)
{
    ನಿರ್ಗಮಿಸು(0);
}

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Enter_Critical(ಶೂನ್ಯ)
{
    ಆದರೆ (!s_scheduler_started) {
        ಹಿಂತಿರುಗಿ;
    }

    pthread_mutex_lock(&s_ಪೋರ್ಟ್_mutex);

    /* Non-FreeRTOS thread or recursive enter: just bump the counter.
     * The mutex is already held (recursive lock succeeds for same thread). */
    ಆದರೆ (!linux_ಪೋರ್ಟ್_ಒಳ_freertos_task() || s_ux_critical_nesting > 0) {
        s_ux_critical_nesting++;
        ಹಿಂತಿರುಗಿ;
    }

    /* First enter from a FreeRTOS task: ensure we're the scheduled task.
     * If not, release the mutex, block until the scheduler switches to us,
     * then re-acquire. */
    thread_t *calling_thread = linux_ಪೋರ್ಟ್_get_calling_thread();
    thread_t *scheduled_thread = linux_ಪೋರ್ಟ್_get_thread_from_handle(xTaskGetCurrentTaskHandle());

    ಆಗ (calling_thread && !calling_thread->is_dying && calling_thread != scheduled_thread) {
        pthread_mutex_unlock(&s_ಪೋರ್ಟ್_mutex);
        linux_ಪೋರ್ಟ್_ಖಂಡ_thread(calling_thread);
        pthread_mutex_lock(&s_ಪೋರ್ಟ್_mutex);
        calling_thread = linux_ಪೋರ್ಟ್_get_calling_thread();
        scheduled_thread = linux_ಪೋರ್ಟ್_get_thread_from_handle(xTaskGetCurrentTaskHandle());
    }

    ಆದರೆ (!calling_thread || calling_thread->is_dying) {
        linux_ಪೋರ್ಟ್_switch_context(xTaskGetCurrentTaskHandle());
        pthread_mutex_unlock(&s_ಪೋರ್ಟ್_mutex);
        ಹಿಂತಿರುಗಿ;
    }

    s_ux_critical_nesting = 1;
}

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Exit_Critical(ಶೂನ್ಯ)
{
    ಆದರೆ (!s_scheduler_started || s_ux_critical_nesting == 0) {
        ಹಿಂತಿರುಗಿ;
    }

    s_ux_critical_nesting--;

    /* Check for deferred yield on final exit from a FreeRTOS task */
    ಆದರೆ (s_ux_critical_nesting == 0 && linux_ಪೋರ್ಟ್_ಒಳ_freertos_task()) {
        thread_t *calling_thread = linux_ಪೋರ್ಟ್_get_calling_thread();
        ಆದರೆ (calling_thread && calling_thread->yield_needed) {
            calling_thread->yield_needed = false;
            pthread_mutex_unlock(&s_ಪೋರ್ಟ್_mutex);
            v_ಪೋರ್ಟ್_Yield();
            ಹಿಂತಿರುಗಿ;
        }
    }

    pthread_mutex_unlock(&s_ಪೋರ್ಟ್_mutex);
}

/* Handle the case where the calling pthread is not a registered FreeRTOS task.
 * If the calling thread has been deleted but is still the scheduled task,
 * perform a context switch. Otherwise just release the mutex and return.
 * Returns true if the caller should return early. */
ಸ್ಥಿರ bool linux_ಪೋರ್ಟ್_handle_deleted_task(TaskHandle_t scheduled_task_hdl,
                                           thread_t *calling_thread)
{
    ಆದರೆ (calling_thread != ಖಾಲಿ) {
        ಹಿಂತಿರುಗಿ false;
    }

    thread_t *scheduled_thread = linux_ಪೋರ್ಟ್_get_thread_from_handle(scheduled_task_hdl);
    ಆದರೆ (scheduled_thread == ಖಾಲಿ) {
        linux_ಪೋರ್ಟ್_switch_context(scheduled_task_hdl);
    }
    pthread_mutex_unlock(&s_ಪೋರ್ಟ್_mutex);
    ಹಿಂತಿರುಗಿ true;
}

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Yield(ಶೂನ್ಯ)
{
    pthread_mutex_lock(&s_ಪೋರ್ಟ್_mutex);

    thread_t *calling_thread = linux_ಪೋರ್ಟ್_get_calling_thread();
    TaskHandle_t scheduled_task_hdl = xTaskGetCurrentTaskHandle();

    ಆದರೆ (linux_ಪೋರ್ಟ್_handle_deleted_task(scheduled_task_hdl, calling_thread)) {
        ಹಿಂತಿರುಗಿ;
    }

    /* If in a critical section, defer the yield until the section exits. */
    ಆದರೆ (s_ux_critical_nesting != 0) {
        calling_thread->yield_needed = true;
        pthread_mutex_unlock(&s_ಪೋರ್ಟ್_mutex);
        ಹಿಂತಿರುಗಿ;
    }

    /* Hand the CPU to the next ready task right now (mimics PendSV on real
     * hardware). */
    linux_ಪೋರ್ಟ್_switch_context(scheduled_task_hdl);
    pthread_mutex_unlock(&s_ಪೋರ್ಟ್_mutex);

    /* If the newly scheduled task is different from the calling thread,
     * block until the scheduler resumes this task. */
    TaskHandle_t ಮುಂದಿನ_task_hdl = xTaskGetCurrentTaskHandle();
    thread_t *ಮುಂದಿನ_thread = linux_ಪೋರ್ಟ್_get_thread_from_handle(ಮುಂದಿನ_task_hdl);
    ಆದರೆ (calling_thread != ಮುಂದಿನ_thread) {
        linux_ಪೋರ್ಟ್_ಖಂಡ_thread(calling_thread);
    }
}

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Yield_Within_Api(ಶೂನ್ಯ)
{
    v_ಪೋರ್ಟ್_Yield();
}

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Suspend_Scheduler(ಶೂನ್ಯ)
{
    /* scheduled out task trying to suspend the scheduler should get blocked here */
    pthread_mutex_lock(&s_ಪೋರ್ಟ್_mutex);

   /* get the metadata of the pthread calling this function */
    thread_t *calling_thread = linux_ಪೋರ್ಟ್_get_calling_thread();

    /* get the thread metadata from the scheduled task */
    TaskHandle_t scheduled_task_hdl = xTaskGetCurrentTaskHandle();

    ಆದರೆ (linux_ಪೋರ್ಟ್_handle_deleted_task(scheduled_task_hdl, calling_thread)) {
        ಹಿಂತಿರುಗಿ;
    }

    thread_t *scheduled_thread = linux_ಪೋರ್ಟ್_get_thread_from_handle(scheduled_task_hdl);
    ಆದರೆ (calling_thread != scheduled_thread) {
        pthread_mutex_unlock(&s_ಪೋರ್ಟ್_mutex);
        v_ಪೋರ್ಟ್_Yield();
        ಹಿಂತಿರುಗಿ;
    }
    pthread_mutex_unlock(&s_ಪೋರ್ಟ್_mutex);
}

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Disable_Interrupts(ಶೂನ್ಯ)
{
    v_ಪೋರ್ಟ್_Enter_Critical();
}

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Enable_Interrupts(ಶೂನ್ಯ)
{
    v_ಪೋರ್ಟ್_Exit_Critical();
}

ತಳ_Type_t x_ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ(ಶೂನ್ಯ)
{
    v_ಪೋರ್ಟ್_Enter_Critical();
    ಹಿಂತಿರುಗಿ pdTRUE;
}

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Clear_ಅಡಚಣೆ_ಮುಖವಾಡ(ತಳ_Type_t x_ಮುಖವಾಡ)
{
    v_ಪೋರ್ಟ್_Exit_Critical();
}

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Thread_Dying(ಶೂನ್ಯ *px_Task_ಗಮ್ಯ_Delete, ಅಸ್ಥಿರ ತಳ_Type_t *pxPendYield)
{
    pthread_mutex_lock(&s_ಪೋರ್ಟ್_mutex);

    thread_t *thread = linux_ಪೋರ್ಟ್_get_thread_from_handle((TaskHandle_t)px_Task_ಗಮ್ಯ_Delete);
    ಆದರೆ (thread == ಖಾಲಿ) {
        pthread_mutex_unlock(&s_ಪೋರ್ಟ್_mutex);
        ಹಿಂತಿರುಗಿ;
    }

    /* Mark the thread as dying, cancel the thread. the pthread
     * will be stopped on next cancellation point. Do not remove the
     * thread item from the list since it will be done in vPortCancelThread */
    thread->is_dying = true;
    pthread_cancel(thread->pthread);

    pthread_mutex_unlock(&s_ಪೋರ್ಟ್_mutex);
}

#ಆದರೆ CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
ಸ್ಥಿರ ಶೂನ್ಯ v_ಪೋರ್ಟ್_TLSPointers_Del_Cb(ಶೂನ್ಯ *pxTCB)
{
    StaticTask_t *tcb = (StaticTask_t *)pxTCB;
    TlsDeleteCallbackFunction_t *pvDelCbs = (TlsDeleteCallbackFunction_t *)(&tcb->pvDummy15[config_ಅಂಕೆ_THREAD_LOCAL_STORAGE_ಸೂಚಕಗಳು / 2]);

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ x = 0; x < (config_ಅಂಕೆ_THREAD_LOCAL_STORAGE_ಸೂಚಕಗಳು / 2); x++) {
        ಆದರೆ (pvDelCbs[x] != ಖಾಲಿ) {
            pvDelCbs[x](x, tcb->pvDummy15[x]);
        }
    }
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Cancel_Thread(ಶೂನ್ಯ *px_Task_ಗಮ್ಯ_Delete)
{
    pthread_mutex_lock(&s_ಪೋರ್ಟ್_mutex);

#ಆದರೆ CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
    v_ಪೋರ್ಟ್_TLSPointers_Del_Cb(px_Task_ಗಮ್ಯ_Delete);
#ಆದರೆ_ಅಂತ್ಯ

    thread_t *thread = linux_ಪೋರ್ಟ್_get_thread_from_handle((TaskHandle_t)px_Task_ಗಮ್ಯ_Delete);
    ಆದರೆ (!thread) {
        pthread_mutex_unlock(&s_ಪೋರ್ಟ್_mutex);
        ಹಿಂತಿರುಗಿ;
    }

    ಆದರೆ (thread->is_dying) {
        /* vPortThreadDying already called */
    } ಇಲ್ಲದಿದ್ದರೆ {
        thread->is_dying = true;
        pthread_cancel(thread->pthread);
    }

    /* Save fields and unregister while holding the lock. */
    pthread_t pt = thread->pthread;
    event_t *ev = thread->ev;
    linux_ಪೋರ್ಟ್_unregister_thread((TaskHandle_t)px_Task_ಗಮ್ಯ_Delete);

    /* Release the mutex before joining – the dying thread may need the
     * scheduler (which also takes s_port_mutex) to reach a cancellation
     * point. */
    pthread_mutex_unlock(&s_ಪೋರ್ಟ್_mutex);

    pthread_join(pt, ಖಾಲಿ);
    event_delete(ev);
    ಬಿಡುಗಡೆ(thread);
}

ಶೂನ್ಯ v_ಪೋರ್ಟ್_ಗಣ_ರಾಶಿ_Watchpoint(ಶೂನ್ಯ *px_ರಾಶಿ_ಆರಂಭ)
{
}

#ಆದರೆ ( CONFIG_FREERTOS_GENERATE_ಸತತ_ಸಮಯ_STATS )
config_ಸತತ_ಸಮಯ_COUNTER_TYPE x_ಪೋರ್ಟ್_Get_ಸತತ_ಸಮಯ_Counter_ಮೌಲ್ಯ( ಶೂನ್ಯ )
{
    ರಚನೆ timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);

    ಹಿಂತಿರುಗಿ ( config_ಸತತ_ಸಮಯ_COUNTER_TYPE ) ( ( ( uint64_t ) ts.tv_sec * 1000000ULL ) +
                                            ( ( uint64_t ) ts.tv_nsec / 1000ULL ) );
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
