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

#define FREERTOS_SIM_TICK_PERIOD_US (1000000 / CONFIG_FREERTOS_HZ)

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ thread {
    ಧ್ರುವ ವರ್ಣ *name;
    pthread_t pthread;
    TaskFunction_t pxCode;
    ಶೂನ್ಯ *pvParams;
    bool is_dying;
    bool yield_needed;
    ಸಂರಚನಾ event *ev;
} thread_t;

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ task_thread_ಗಂಟು {
    TaskHandle_t handle;
    thread_t *thread;
    SLIST_ENTRY(task_thread_ಗಂಟು) next;
} task_thread_ಗಂಟು_t;


ಸ್ಥಿರ SLIST_HEAD(task_thread_ಗಂಟು_ll, task_thread_ಗಂಟು) s_task_thread_list = SLIST_HEAD_INITIALIZER(task_thread_ಗಂಟು);
ಸ್ಥಿರ pthread_mutex_t s_thread_map_mutex = PTHREAD_MUTEX_INITIALIZER;

ಸ್ಥಿರ pthread_mutex_t s_port_mutex;
ಸ್ಥಿರ pthread_t s_scheduler_thread;
ಸ್ಥಿರ bool s_scheduler_started = false;
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ s_ux_critical_nesting = 0;

/* TLS flag: true only when inside a real FreeRTOS task pthread */
ಸ್ಥಿರ __thread bool s_in_freertos_task = false;

bool linux_port_in_freertos_task(ಶೂನ್ಯ)
{
    ಪ್ರತ್ಯಾಗಮ s_in_freertos_task;
}

ಸ್ಥಿರ ಶೂನ್ಯ linux_port_initialize_mutexes(ಶೂನ್ಯ)
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&s_port_mutex, &attr);
    pthread_mutexattr_destroy(&attr);
}

ಸ್ಥಿರ ಶೂನ್ಯ linux_port_fatal_error(ಧ್ರುವ ವರ್ಣ *ಸಂದೇಶ, ಪೂರ್ಣಾಙ್ಕ err)
{
    fprintf(stderr, "%s: %s\n", ಸಂದೇಶ, strerror(err));
    abort();
}

ಸ್ಥಿರ ಶೂನ್ಯ linux_port_register_thread(TaskHandle_t handle, thread_t *thread)
{
    ಯದಿ (handle == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ;
    }

    task_thread_ಗಂಟು_t *ಗಂಟು = malloc(ಪರಿಮಾಣ(task_thread_ಗಂಟು_t));
    ಯದಿ (!ಗಂಟು) {
        linux_port_fatal_error("Failed to allocate thread map node", -1);
    }

    ಗಂಟು->handle = handle;
    ಗಂಟು->thread = thread;

    pthread_mutex_lock(&s_thread_map_mutex);
    SLIST_INSERT_HEAD(&s_task_thread_list, ಗಂಟು, next);
    pthread_mutex_unlock(&s_thread_map_mutex);
}

ಸ್ಥಿರ ಶೂನ್ಯ linux_port_unregister_thread(TaskHandle_t handle)
{
    ಯದಿ (handle == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ;
    }

    pthread_mutex_lock(&s_thread_map_mutex);

    task_thread_ಗಂಟು_t *cur_ಗಂಟು = SLIST_FIRST(&s_task_thread_list);
    task_thread_ಗಂಟು_t *prev_ಗಂಟು = ರಿಕ್ತ;

    ಯಾವತ್ (cur_ಗಂಟು) {
        ಯದಿ (cur_ಗಂಟು->handle == handle) {
            ಯದಿ (prev_ಗಂಟು) {
                prev_ಗಂಟು->next.sle_next = SLIST_NEXT(cur_ಗಂಟು, next);
            } ಅನ್ಯಥಾ {
                SLIST_REMOVE_HEAD(&s_task_thread_list, next);
            }

            ಮುಕ್ತ(cur_ಗಂಟು);
            pthread_mutex_unlock(&s_thread_map_mutex);
            ಪ್ರತ್ಯಾಗಮ;
        }

        prev_ಗಂಟು = cur_ಗಂಟು;
        cur_ಗಂಟು = SLIST_NEXT(cur_ಗಂಟು, next);
    }

    pthread_mutex_unlock(&s_thread_map_mutex);
}

ಸ್ಥಿರ thread_t *linux_port_get_thread_from_handle(TaskHandle_t handle)
{
    ಯದಿ (handle == ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    pthread_mutex_lock(&s_thread_map_mutex);
    task_thread_ಗಂಟು_t *ಗಂಟು = ರಿಕ್ತ;
    SLIST_FOREACH(ಗಂಟು, &s_task_thread_list, next) {
        ಯದಿ (ಗಂಟು->handle == (TaskHandle_t)(*(ರಾಶಿ_Type_t **)(handle))) {
            thread_t *t = ಗಂಟು->thread;
            pthread_mutex_unlock(&s_thread_map_mutex);
            ಪ್ರತ್ಯಾಗಮ t;
        }
    }
    pthread_mutex_unlock(&s_thread_map_mutex);
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸ್ಥಿರ thread_t *linux_port_get_calling_thread(ಶೂನ್ಯ)
{
    pthread_t self = pthread_self();
    pthread_mutex_lock(&s_thread_map_mutex);
    task_thread_ಗಂಟು_t *ಗಂಟು = ರಿಕ್ತ;
    SLIST_FOREACH(ಗಂಟು, &s_task_thread_list, next) {
        ಯದಿ (pthread_equal(ಗಂಟು->thread->pthread, self)) {
            thread_t *thread = ಗಂಟು->thread;
            pthread_mutex_unlock(&s_thread_map_mutex);
            ಪ್ರತ್ಯಾಗಮ thread;
        }
    }
    pthread_mutex_unlock(&s_thread_map_mutex);
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

pthread_t linux_port_get_scheduled_task_pthread(ಶೂನ್ಯ)
{
    thread_t *thread = linux_port_get_thread_from_handle(xTaskGetCurrentTaskHandle());
    ಪ್ರತ್ಯಾಗಮ thread ? thread->pthread : pthread_self();
}

ಸ್ಥಿರ ಶೂನ್ಯ *linux_port_task_runner(ಶೂನ್ಯ *arg)
{
    /* Allow this thread to be cancelled */
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, ರಿಕ್ತ);

    /* set the flag showing that this is a freertos task */
    s_in_freertos_task = true;

    /* setup the backtrace signal. ONLY triggered before abort so
     * it will not interfere with the simulation while its running */
    linux_port_setup_backtrace_signal();

    thread_t *thread = arg;

    /* Block until scheduler signals first time, then run the task body. */
    event_wait(thread->ev);
    thread->pxCode(thread->pvParams);

    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ಸ್ಥಿರ ಶೂನ್ಯ linux_port_unblock_thread(thread_t *thread)
{
    event_signal(thread->ev);
}

ಸ್ಥಿರ ಶೂನ್ಯ linux_port_block_thread(thread_t *thread)
{
    event_wait(thread->ev);
}

ಸ್ಥಿರ ಶೂನ್ಯ linux_port_increment_tick(ಶೂನ್ಯ)
{
    (ಶೂನ್ಯ)xTaskIncrementTick();
}

ಸ್ಥಿರ ಶೂನ್ಯ linux_port_switch_context(TaskHandle_t current_task_hdl)
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
    ಯದಿ (next_thread) {
        /* Only unblock the thread if we are actually switching to a
         * different one. Signaling the already-running thread would
         * leave a stale event_triggered flag, causing its next
         * event_wait (e.g. in vPortYield) to return immediately
         * instead of blocking.
         *
         * Exception: on the very first switch, the task is still blocked
         * in its initial event_wait (linux_port_task_runner), so we must
         * signal it even though current_thread == next_thread. */
        ಯದಿ (next_thread != current_thread || !s_scheduler_started) {
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
            ಯದಿ (current_thread) {
                event_clear(current_thread->ev);
            }
            linux_port_unblock_thread(next_thread);
        }
    }

    ಯದಿ (!s_scheduler_started) {
        s_scheduler_started = true;
    }

    /* fill the name of the task in the thread item if not done already. */
    ಯದಿ (next_thread && next_thread->name == ರಿಕ್ತ) {
        /* fill the name of the thread now */
        next_thread->name = pcTaskGetName(next_task_hdl);
    }

    /* fill the name of the task in the thread item if not done already. */
    ಯದಿ (current_thread && current_thread->name == ರಿಕ್ತ) {
        /* fill the name of the thread now */
        current_thread->name = pcTaskGetName(current_task_hdl);
    }

    pthread_mutex_unlock(&s_port_mutex);
}

ಸ್ಥಿರ ಶೂನ್ಯ *linux_port_scheduler_runner(ಶೂನ್ಯ *arg)
{
    (ಶೂನ್ಯ)arg;

    ಯಾವತ್ (1) {
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
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

ರಾಶಿ_Type_t *px_Port_Initialise_ರಾಶಿ(ರಾಶಿ_Type_t *px_ತುದಿ_Of_ರಾಶಿ,
                                   ರಾಶಿ_Type_t *px_End_Of_ರಾಶಿ,
                                   TaskFunction_t pxCode,
                                   ಶೂನ್ಯ *pvParameters)
{
    pthread_attr_t thread_attr;
    size_t thread_ರಾಶಿ_size;

    /* Store the thread data at the start of the stack. */
    thread_ರಾಶಿ_size = (px_ತುದಿ_Of_ರಾಶಿ - px_End_Of_ರಾಶಿ) * ಪರಿಮಾಣ(*px_ತುದಿ_Of_ರಾಶಿ);
    pthread_attr_init(&thread_attr);
    pthread_attr_setstack(&thread_attr, px_End_Of_ರಾಶಿ, thread_ರಾಶಿ_size);

    thread_t *thread = malloc(ಪರಿಮಾಣ(thread_t));
    ಯದಿ (!thread) {
        linux_port_fatal_error("Failed to allocate thread metadata", -1);
    }
    thread->name = ರಿಕ್ತ; // this will be filled later when we know about the task name
    thread->pxCode = pxCode;
    thread->pvParams = pvParameters;
    thread->is_dying = false;
    thread->yield_needed = false;
    thread->ev = event_create();

    linux_port_register_thread((TaskHandle_t)px_ತುದಿ_Of_ರಾಶಿ, thread);

    /* create the thread associated with the task being created */
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ ret = pthread_create(&thread->pthread, &thread_attr, linux_port_task_runner, thread);
    ಯದಿ (ret != 0) {
        linux_port_fatal_error("pthread_create", ret);
    }
    ಪ್ರತ್ಯಾಗಮ px_ತುದಿ_Of_ರಾಶಿ;
}

BaseType_t xPortStartScheduler(ಶೂನ್ಯ)
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
    ಪೂರ್ಣಾಙ್ಕ ret = pthread_create(&s_scheduler_thread, ರಿಕ್ತ, linux_port_scheduler_runner, ರಿಕ್ತ);
    ಯದಿ (ret != 0) {
       linux_port_fatal_error("pthread_create", ret);
    }

    /* Should never return */
    pthread_join(s_scheduler_thread, ರಿಕ್ತ);

    ಪ್ರತ್ಯಾಗಮ 0;
}

ಶೂನ್ಯ vPortEndScheduler(ಶೂನ್ಯ)
{
    exit(0);
}

ಶೂನ್ಯ vPortEnterCritical(ಶೂನ್ಯ)
{
    ಯದಿ (!s_scheduler_started) {
        ಪ್ರತ್ಯಾಗಮ;
    }

    pthread_mutex_lock(&s_port_mutex);

    /* Non-FreeRTOS thread or recursive enter: just bump the counter.
     * The mutex is already held (recursive lock succeeds for same thread). */
    ಯದಿ (!linux_port_in_freertos_task() || s_ux_critical_nesting > 0) {
        s_ux_critical_nesting++;
        ಪ್ರತ್ಯಾಗಮ;
    }

    /* First enter from a FreeRTOS task: ensure we're the scheduled task.
     * If not, release the mutex, block until the scheduler switches to us,
     * then re-acquire. */
    thread_t *calling_thread = linux_port_get_calling_thread();
    thread_t *scheduled_thread = linux_port_get_thread_from_handle(xTaskGetCurrentTaskHandle());

    ಯಾವತ್ (calling_thread && !calling_thread->is_dying && calling_thread != scheduled_thread) {
        pthread_mutex_unlock(&s_port_mutex);
        linux_port_block_thread(calling_thread);
        pthread_mutex_lock(&s_port_mutex);
        calling_thread = linux_port_get_calling_thread();
        scheduled_thread = linux_port_get_thread_from_handle(xTaskGetCurrentTaskHandle());
    }

    ಯದಿ (!calling_thread || calling_thread->is_dying) {
        linux_port_switch_context(xTaskGetCurrentTaskHandle());
        pthread_mutex_unlock(&s_port_mutex);
        ಪ್ರತ್ಯಾಗಮ;
    }

    s_ux_critical_nesting = 1;
}

ಶೂನ್ಯ vPortExitCritical(ಶೂನ್ಯ)
{
    ಯದಿ (!s_scheduler_started || s_ux_critical_nesting == 0) {
        ಪ್ರತ್ಯಾಗಮ;
    }

    s_ux_critical_nesting--;

    /* Check for deferred yield on final exit from a FreeRTOS task */
    ಯದಿ (s_ux_critical_nesting == 0 && linux_port_in_freertos_task()) {
        thread_t *calling_thread = linux_port_get_calling_thread();
        ಯದಿ (calling_thread && calling_thread->yield_needed) {
            calling_thread->yield_needed = false;
            pthread_mutex_unlock(&s_port_mutex);
            vPortYield();
            ಪ್ರತ್ಯಾಗಮ;
        }
    }

    pthread_mutex_unlock(&s_port_mutex);
}

/* Handle the case where the calling pthread is not a registered FreeRTOS task.
 * If the calling thread has been deleted but is still the scheduled task,
 * perform a context switch. Otherwise just release the mutex and return.
 * Returns true if the caller should return early. */
ಸ್ಥಿರ bool linux_port_handle_deleted_task(TaskHandle_t scheduled_task_hdl,
                                           thread_t *calling_thread)
{
    ಯದಿ (calling_thread != ರಿಕ್ತ) {
        ಪ್ರತ್ಯಾಗಮ false;
    }

    thread_t *scheduled_thread = linux_port_get_thread_from_handle(scheduled_task_hdl);
    ಯದಿ (scheduled_thread == ರಿಕ್ತ) {
        linux_port_switch_context(scheduled_task_hdl);
    }
    pthread_mutex_unlock(&s_port_mutex);
    ಪ್ರತ್ಯಾಗಮ true;
}

ಶೂನ್ಯ vPortYield(ಶೂನ್ಯ)
{
    pthread_mutex_lock(&s_port_mutex);

    thread_t *calling_thread = linux_port_get_calling_thread();
    TaskHandle_t scheduled_task_hdl = xTaskGetCurrentTaskHandle();

    ಯದಿ (linux_port_handle_deleted_task(scheduled_task_hdl, calling_thread)) {
        ಪ್ರತ್ಯಾಗಮ;
    }

    /* If in a critical section, defer the yield until the section exits. */
    ಯದಿ (s_ux_critical_nesting != 0) {
        calling_thread->yield_needed = true;
        pthread_mutex_unlock(&s_port_mutex);
        ಪ್ರತ್ಯಾಗಮ;
    }

    /* Hand the CPU to the next ready task right now (mimics PendSV on real
     * hardware). */
    linux_port_switch_context(scheduled_task_hdl);
    pthread_mutex_unlock(&s_port_mutex);

    /* If the newly scheduled task is different from the calling thread,
     * block until the scheduler resumes this task. */
    TaskHandle_t next_task_hdl = xTaskGetCurrentTaskHandle();
    thread_t *next_thread = linux_port_get_thread_from_handle(next_task_hdl);
    ಯದಿ (calling_thread != next_thread) {
        linux_port_block_thread(calling_thread);
    }
}

ಶೂನ್ಯ vPortYieldWithinApi(ಶೂನ್ಯ)
{
    vPortYield();
}

ಶೂನ್ಯ vPortSuspendScheduler(ಶೂನ್ಯ)
{
    /* scheduled out task trying to suspend the scheduler should get blocked here */
    pthread_mutex_lock(&s_port_mutex);

   /* get the metadata of the pthread calling this function */
    thread_t *calling_thread = linux_port_get_calling_thread();

    /* get the thread metadata from the scheduled task */
    TaskHandle_t scheduled_task_hdl = xTaskGetCurrentTaskHandle();

    ಯದಿ (linux_port_handle_deleted_task(scheduled_task_hdl, calling_thread)) {
        ಪ್ರತ್ಯಾಗಮ;
    }

    thread_t *scheduled_thread = linux_port_get_thread_from_handle(scheduled_task_hdl);
    ಯದಿ (calling_thread != scheduled_thread) {
        pthread_mutex_unlock(&s_port_mutex);
        vPortYield();
        ಪ್ರತ್ಯಾಗಮ;
    }
    pthread_mutex_unlock(&s_port_mutex);
}

ಶೂನ್ಯ vPortDisableInterrupts(ಶೂನ್ಯ)
{
    vPortEnterCritical();
}

ಶೂನ್ಯ vPortEnableInterrupts(ಶೂನ್ಯ)
{
    vPortExitCritical();
}

BaseType_t xPortSetInterruptMask(ಶೂನ್ಯ)
{
    vPortEnterCritical();
    ಪ್ರತ್ಯಾಗಮ pdTRUE;
}

ಶೂನ್ಯ vPortClearInterruptMask(BaseType_t xMask)
{
    vPortExitCritical();
}

ಶೂನ್ಯ vPortThreadDying(ಶೂನ್ಯ *pxTaskToDelete, ಚಞ್ಚಲ BaseType_t *pxPendYield)
{
    pthread_mutex_lock(&s_port_mutex);

    thread_t *thread = linux_port_get_thread_from_handle((TaskHandle_t)pxTaskToDelete);
    ಯದಿ (thread == ರಿಕ್ತ) {
        pthread_mutex_unlock(&s_port_mutex);
        ಪ್ರತ್ಯಾಗಮ;
    }

    /* Mark the thread as dying, cancel the thread. the pthread
     * will be stopped on next cancellation point. Do not remove the
     * thread item from the list since it will be done in vPortCancelThread */
    thread->is_dying = true;
    pthread_cancel(thread->pthread);

    pthread_mutex_unlock(&s_port_mutex);
}

#ಯದಿ CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
ಸ್ಥಿರ ಶೂನ್ಯ vPortTLSPointersDelCb(ಶೂನ್ಯ *pxTCB)
{
    StaticTask_t *tcb = (StaticTask_t *)pxTCB;
    TlsDeleteCallbackFunction_t *pvDelCbs = (TlsDeleteCallbackFunction_t *)(&tcb->pvDummy15[config_ಅಂಕೆ_THREAD_LOCAL_STORAGE_POINTERS / 2]);

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ x = 0; x < (config_ಅಂಕೆ_THREAD_LOCAL_STORAGE_POINTERS / 2); x++) {
        ಯದಿ (pvDelCbs[x] != ರಿಕ್ತ) {
            pvDelCbs[x](x, tcb->pvDummy15[x]);
        }
    }
}
#endif /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

ಶೂನ್ಯ vPortCancelThread(ಶೂನ್ಯ *pxTaskToDelete)
{
    pthread_mutex_lock(&s_port_mutex);

#ಯದಿ CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
    vPortTLSPointersDelCb(pxTaskToDelete);
#endif

    thread_t *thread = linux_port_get_thread_from_handle((TaskHandle_t)pxTaskToDelete);
    ಯದಿ (!thread) {
        pthread_mutex_unlock(&s_port_mutex);
        ಪ್ರತ್ಯಾಗಮ;
    }

    ಯದಿ (thread->is_dying) {
        /* vPortThreadDying already called */
    } ಅನ್ಯಥಾ {
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

    pthread_join(pt, ರಿಕ್ತ);
    event_delete(ev);
    ಮುಕ್ತ(thread);
}

ಶೂನ್ಯ v_Port_Set_ರಾಶಿ_Watchpoint(ಶೂನ್ಯ *px_ರಾಶಿ_Start)
{
}

#ಯದಿ ( CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS )
configRUN_TIME_COUNTER_TYPE x_Port_Get_Run_Time_Counter_ಮೌಲ್ಯ( ಶೂನ್ಯ )
{
    ಸಂರಚನಾ timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);

    ಪ್ರತ್ಯಾಗಮ ( configRUN_TIME_COUNTER_TYPE ) ( ( ( uint64_t ) ts.tv_sec * 1000000ULL ) +
                                            ( ( uint64_t ) ts.tv_nsec / 1000ULL ) );
}
#endif /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
