/*
 * SPDX-FileCopyrightText: 2025-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <stdint.h>
#ضمن <unistd.h>
#ضمن <pthread.h>
#ضمن <sys/queue.h>
#ضمن <time.h>
#ضمن "string.h"
#ضمن "FreeRTOS.h"
#ضمن "task.h"
#ضمن "utils/wait_for_event.h"
#ضمن "esp_private/freertos_linux_coop_syscalls.h"
#ضمن "utils/linux_port_utils.h"

#وصف_ڪريو FREERTOS_SIM_TICK_PERIOD_US (1000000 / CONFIG_FREERTOS_HZ)

تعريفنوع بنية thread {
    ثابت حرف *name;
    pthread_t pthread;
    TaskFunction_t px_ڪوڊ;
    فراغ *pvParams;
    bool is_dying;
    bool yield_needed;
    بنية event *ev;
} thread_t;

تعريفنوع بنية task_thread_ڳنڍ {
    TaskHandle_t handle;
    thread_t *thread;
    SLIST_ENTRY(task_thread_ڳنڍ) ايندڙ;
} task_thread_ڳنڍ_t;


ساكن SLIST_HEAD(task_thread_ڳنڍ_ll, task_thread_ڳنڍ) s_task_thread_list = SLIST_HEAD_INITIALIZER(task_thread_ڳنڍ);
ساكن pthread_mutex_t s_thread_map_mutex = PTHREAD_MUTEX_INITIALIZER;

ساكن pthread_mutex_t s_پورٽ_mutex;
ساكن pthread_t s_scheduler_thread;
ساكن bool s_scheduler_started = false;
ساكن صحيح s_ux_critical_nesting = 0;

/* TLS flag: true only when inside a real FreeRTOS task pthread */
ساكن __thread bool s_اندريون_freertos_task = false;

bool linux_پورٽ_اندريون_freertos_task(فراغ)
{
    أرجع s_اندريون_freertos_task;
}

ساكن فراغ linux_پورٽ_initialize_mutexes(فراغ)
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&s_پورٽ_mutex, &attr);
    pthread_mutexattr_destroy(&attr);
}

ساكن فراغ linux_پورٽ_fatal_error(ثابت حرف *نياپو, صحيح err)
{
    fprintf(stderr, "%s: %s\n", نياپو, strerror(err));
    abort();
}

ساكن فراغ linux_پورٽ_رجسٽر_thread(TaskHandle_t handle, thread_t *thread)
{
    إذا (handle == عدم) {
        أرجع;
    }

    task_thread_ڳنڍ_t *ڳنڍ = يادگيري_ڏيو(حجم(task_thread_ڳنڍ_t));
    إذا (!ڳنڍ) {
        linux_پورٽ_fatal_error("Failed to allocate thread map node", -1);
    }

    ڳنڍ->handle = handle;
    ڳنڍ->thread = thread;

    pthread_mutex_lock(&s_thread_map_mutex);
    SLIST_INSERT_HEAD(&s_task_thread_list, ڳنڍ, ايندڙ);
    pthread_mutex_unlock(&s_thread_map_mutex);
}

ساكن فراغ linux_پورٽ_unregister_thread(TaskHandle_t handle)
{
    إذا (handle == عدم) {
        أرجع;
    }

    pthread_mutex_lock(&s_thread_map_mutex);

    task_thread_ڳنڍ_t *cur_ڳنڍ = SLIST_FIRST(&s_task_thread_list);
    task_thread_ڳنڍ_t *prev_ڳنڍ = عدم;

    طالما (cur_ڳنڍ) {
        إذا (cur_ڳنڍ->handle == handle) {
            إذا (prev_ڳنڍ) {
                prev_ڳنڍ->ايندڙ.sle_ايندڙ = SLIST_NEXT(cur_ڳنڍ, ايندڙ);
            } وإلا {
                SLIST_REMOVE_HEAD(&s_task_thread_list, ايندڙ);
            }

            حرر(cur_ڳنڍ);
            pthread_mutex_unlock(&s_thread_map_mutex);
            أرجع;
        }

        prev_ڳنڍ = cur_ڳنڍ;
        cur_ڳنڍ = SLIST_NEXT(cur_ڳنڍ, ايندڙ);
    }

    pthread_mutex_unlock(&s_thread_map_mutex);
}

ساكن thread_t *linux_پورٽ_get_thread_from_handle(TaskHandle_t handle)
{
    إذا (handle == عدم) {
        أرجع عدم;
    }

    pthread_mutex_lock(&s_thread_map_mutex);
    task_thread_ڳنڍ_t *ڳنڍ = عدم;
    SLIST_FOREACH(ڳنڍ, &s_task_thread_list, ايندڙ) {
        إذا (ڳنڍ->handle == (TaskHandle_t)(*(ڍير_Type_t **)(handle))) {
            thread_t *t = ڳنڍ->thread;
            pthread_mutex_unlock(&s_thread_map_mutex);
            أرجع t;
        }
    }
    pthread_mutex_unlock(&s_thread_map_mutex);
    أرجع عدم;
}

ساكن thread_t *linux_پورٽ_get_calling_thread(فراغ)
{
    pthread_t self = pthread_self();
    pthread_mutex_lock(&s_thread_map_mutex);
    task_thread_ڳنڍ_t *ڳنڍ = عدم;
    SLIST_FOREACH(ڳنڍ, &s_task_thread_list, ايندڙ) {
        إذا (pthread_equal(ڳنڍ->thread->pthread, self)) {
            thread_t *thread = ڳنڍ->thread;
            pthread_mutex_unlock(&s_thread_map_mutex);
            أرجع thread;
        }
    }
    pthread_mutex_unlock(&s_thread_map_mutex);
    أرجع عدم;
}

pthread_t linux_پورٽ_get_scheduled_task_pthread(فراغ)
{
    thread_t *thread = linux_پورٽ_get_thread_from_handle(xTaskGetCurrentTaskHandle());
    أرجع thread ? thread->pthread : pthread_self();
}

ساكن فراغ *linux_پورٽ_task_runner(فراغ *arg)
{
    /* Allow this thread to be cancelled */
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, عدم);

    /* set the flag showing that this is a freertos task */
    s_اندريون_freertos_task = true;

    /* setup the backtrace signal. ONLY triggered before abort so
     * it will not interfere with the simulation while its running */
    linux_پورٽ_تياري_backtrace_signal();

    thread_t *thread = arg;

    /* Block until scheduler signals first time, then run the task body. */
    event_wait(thread->ev);
    thread->px_ڪوڊ(thread->pvParams);

    أرجع عدم;
}

ساكن فراغ linux_پورٽ_unblock_thread(thread_t *thread)
{
    event_signal(thread->ev);
}

ساكن فراغ linux_پورٽ_بلاڪ_thread(thread_t *thread)
{
    event_wait(thread->ev);
}

ساكن فراغ linux_پورٽ_increment_tick(فراغ)
{
    (فراغ)xTaskIncrementTick();
}

ساكن فراغ linux_پورٽ_switch_context(TaskHandle_t current_task_hdl)
{
    pthread_mutex_lock(&s_پورٽ_mutex);

    thread_t *current_thread = linux_پورٽ_get_thread_from_handle(current_task_hdl);

    /* get the task that should be scheduled next */
    vTaskSwitchContext();

    /* get the new task to schedule and the associated thread item */
    TaskHandle_t ايندڙ_task_hdl = xTaskGetCurrentTaskHandle();
    thread_t *ايندڙ_thread = linux_پورٽ_get_thread_from_handle(ايندڙ_task_hdl);

    /* unblock the newly scheduled task if it is different from
     * the one already scheduled */
    إذا (ايندڙ_thread) {
        /* Only unblock the thread if we are actually switching to a
         * different one. Signaling the already-running thread would
         * leave a stale event_triggered flag, causing its next
         * event_wait (e.g. in vPortYield) to return immediately
         * instead of blocking.
         *
         * Exception: on the very first switch, the task is still blocked
         * in its initial event_wait (linux_port_task_runner), so we must
         * signal it even though current_thread == next_thread. */
        إذا (ايندڙ_thread != current_thread || !s_scheduler_started) {
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
            إذا (current_thread) {
                event_clear(current_thread->ev);
            }
            linux_پورٽ_unblock_thread(ايندڙ_thread);
        }
    }

    إذا (!s_scheduler_started) {
        s_scheduler_started = true;
    }

    /* fill the name of the task in the thread item if not done already. */
    إذا (ايندڙ_thread && ايندڙ_thread->name == عدم) {
        /* fill the name of the thread now */
        ايندڙ_thread->name = pcTaskGetName(ايندڙ_task_hdl);
    }

    /* fill the name of the task in the thread item if not done already. */
    إذا (current_thread && current_thread->name == عدم) {
        /* fill the name of the thread now */
        current_thread->name = pcTaskGetName(current_task_hdl);
    }

    pthread_mutex_unlock(&s_پورٽ_mutex);
}

ساكن فراغ *linux_پورٽ_scheduler_runner(فراغ *arg)
{
    (فراغ)arg;

    طالما (1) {
        /* sleep for a period of 1 tick */
        usleep(FREERTOS_SIM_TICK_PERIOD_US);

        /* get the task that is currently scheduled */
        TaskHandle_t current_task_hdl = xTaskGetCurrentTaskHandle();

        /* Lock the port mutex. This will block while any task is in a
         * critical section, ensuring ticks don't preempt critical code. */
        pthread_mutex_lock(&s_پورٽ_mutex);

        /* increment the freertos tick */
        linux_پورٽ_increment_tick();

        /* schedule a new task, and schedule out the currently running one */
        linux_پورٽ_switch_context(current_task_hdl);

        pthread_mutex_unlock(&s_پورٽ_mutex);
    }
    أرجع عدم;
}

ڍير_Type_t *px_پورٽ_Initialise_ڍير(ڍير_Type_t *px_چوٽي_جو_ڍير,
                                   ڍير_Type_t *px_پڇاڙي_جو_ڍير,
                                   TaskFunction_t px_ڪوڊ,
                                   فراغ *pvParameters)
{
    pthread_attr_t thread_attr;
    size_t thread_ڍير_سائيز;

    /* Store the thread data at the start of the stack. */
    thread_ڍير_سائيز = (px_چوٽي_جو_ڍير - px_پڇاڙي_جو_ڍير) * حجم(*px_چوٽي_جو_ڍير);
    pthread_attr_init(&thread_attr);
    pthread_attr_setstack(&thread_attr, px_پڇاڙي_جو_ڍير, thread_ڍير_سائيز);

    thread_t *thread = يادگيري_ڏيو(حجم(thread_t));
    إذا (!thread) {
        linux_پورٽ_fatal_error("Failed to allocate thread metadata", -1);
    }
    thread->name = عدم; // this will be filled later when we know about the task name
    thread->px_ڪوڊ = px_ڪوڊ;
    thread->pvParams = pvParameters;
    thread->is_dying = false;
    thread->yield_needed = false;
    thread->ev = event_create();

    linux_پورٽ_رجسٽر_thread((TaskHandle_t)px_چوٽي_جو_ڍير, thread);

    /* create the thread associated with the task being created */
    ثابت صحيح ret = pthread_create(&thread->pthread, &thread_attr, linux_پورٽ_task_runner, thread);
    إذا (ret != 0) {
        linux_پورٽ_fatal_error("pthread_create", ret);
    }
    أرجع px_چوٽي_جو_ڍير;
}

بنياد_Type_t x_پورٽ_شروعات_Scheduler(فراغ)
{
    /* set the port mutex to be recursive. Must be done before
     * vPortEnableInterrupts() which calls vPortExitCritical(). */
    linux_پورٽ_initialize_mutexes();

    /* enable interrupt that were disabled in vTaskStartScheduler */
    v_پورٽ_Enable_Interrupts();

    /* init the cooperative syscall layer (sets stdio non-blocking).
     * Provided by VFS component; weak no-op when VFS is not linked. */
    freertos_linux_coop_syscalls_init();

    /* Start scheduler thread */
    صحيح ret = pthread_create(&s_scheduler_thread, عدم, linux_پورٽ_scheduler_runner, عدم);
    إذا (ret != 0) {
       linux_پورٽ_fatal_error("pthread_create", ret);
    }

    /* Should never return */
    pthread_join(s_scheduler_thread, عدم);

    أرجع 0;
}

فراغ v_پورٽ_پڇاڙي_Scheduler(فراغ)
{
    ٻاهر_نڪرو(0);
}

فراغ v_پورٽ_Enter_Critical(فراغ)
{
    إذا (!s_scheduler_started) {
        أرجع;
    }

    pthread_mutex_lock(&s_پورٽ_mutex);

    /* Non-FreeRTOS thread or recursive enter: just bump the counter.
     * The mutex is already held (recursive lock succeeds for same thread). */
    إذا (!linux_پورٽ_اندريون_freertos_task() || s_ux_critical_nesting > 0) {
        s_ux_critical_nesting++;
        أرجع;
    }

    /* First enter from a FreeRTOS task: ensure we're the scheduled task.
     * If not, release the mutex, block until the scheduler switches to us,
     * then re-acquire. */
    thread_t *calling_thread = linux_پورٽ_get_calling_thread();
    thread_t *scheduled_thread = linux_پورٽ_get_thread_from_handle(xTaskGetCurrentTaskHandle());

    طالما (calling_thread && !calling_thread->is_dying && calling_thread != scheduled_thread) {
        pthread_mutex_unlock(&s_پورٽ_mutex);
        linux_پورٽ_بلاڪ_thread(calling_thread);
        pthread_mutex_lock(&s_پورٽ_mutex);
        calling_thread = linux_پورٽ_get_calling_thread();
        scheduled_thread = linux_پورٽ_get_thread_from_handle(xTaskGetCurrentTaskHandle());
    }

    إذا (!calling_thread || calling_thread->is_dying) {
        linux_پورٽ_switch_context(xTaskGetCurrentTaskHandle());
        pthread_mutex_unlock(&s_پورٽ_mutex);
        أرجع;
    }

    s_ux_critical_nesting = 1;
}

فراغ v_پورٽ_Exit_Critical(فراغ)
{
    إذا (!s_scheduler_started || s_ux_critical_nesting == 0) {
        أرجع;
    }

    s_ux_critical_nesting--;

    /* Check for deferred yield on final exit from a FreeRTOS task */
    إذا (s_ux_critical_nesting == 0 && linux_پورٽ_اندريون_freertos_task()) {
        thread_t *calling_thread = linux_پورٽ_get_calling_thread();
        إذا (calling_thread && calling_thread->yield_needed) {
            calling_thread->yield_needed = false;
            pthread_mutex_unlock(&s_پورٽ_mutex);
            v_پورٽ_Yield();
            أرجع;
        }
    }

    pthread_mutex_unlock(&s_پورٽ_mutex);
}

/* Handle the case where the calling pthread is not a registered FreeRTOS task.
 * If the calling thread has been deleted but is still the scheduled task,
 * perform a context switch. Otherwise just release the mutex and return.
 * Returns true if the caller should return early. */
ساكن bool linux_پورٽ_handle_deleted_task(TaskHandle_t scheduled_task_hdl,
                                           thread_t *calling_thread)
{
    إذا (calling_thread != عدم) {
        أرجع false;
    }

    thread_t *scheduled_thread = linux_پورٽ_get_thread_from_handle(scheduled_task_hdl);
    إذا (scheduled_thread == عدم) {
        linux_پورٽ_switch_context(scheduled_task_hdl);
    }
    pthread_mutex_unlock(&s_پورٽ_mutex);
    أرجع true;
}

فراغ v_پورٽ_Yield(فراغ)
{
    pthread_mutex_lock(&s_پورٽ_mutex);

    thread_t *calling_thread = linux_پورٽ_get_calling_thread();
    TaskHandle_t scheduled_task_hdl = xTaskGetCurrentTaskHandle();

    إذا (linux_پورٽ_handle_deleted_task(scheduled_task_hdl, calling_thread)) {
        أرجع;
    }

    /* If in a critical section, defer the yield until the section exits. */
    إذا (s_ux_critical_nesting != 0) {
        calling_thread->yield_needed = true;
        pthread_mutex_unlock(&s_پورٽ_mutex);
        أرجع;
    }

    /* Hand the CPU to the next ready task right now (mimics PendSV on real
     * hardware). */
    linux_پورٽ_switch_context(scheduled_task_hdl);
    pthread_mutex_unlock(&s_پورٽ_mutex);

    /* If the newly scheduled task is different from the calling thread,
     * block until the scheduler resumes this task. */
    TaskHandle_t ايندڙ_task_hdl = xTaskGetCurrentTaskHandle();
    thread_t *ايندڙ_thread = linux_پورٽ_get_thread_from_handle(ايندڙ_task_hdl);
    إذا (calling_thread != ايندڙ_thread) {
        linux_پورٽ_بلاڪ_thread(calling_thread);
    }
}

فراغ v_پورٽ_Yield_Within_Api(فراغ)
{
    v_پورٽ_Yield();
}

فراغ v_پورٽ_Suspend_Scheduler(فراغ)
{
    /* scheduled out task trying to suspend the scheduler should get blocked here */
    pthread_mutex_lock(&s_پورٽ_mutex);

   /* get the metadata of the pthread calling this function */
    thread_t *calling_thread = linux_پورٽ_get_calling_thread();

    /* get the thread metadata from the scheduled task */
    TaskHandle_t scheduled_task_hdl = xTaskGetCurrentTaskHandle();

    إذا (linux_پورٽ_handle_deleted_task(scheduled_task_hdl, calling_thread)) {
        أرجع;
    }

    thread_t *scheduled_thread = linux_پورٽ_get_thread_from_handle(scheduled_task_hdl);
    إذا (calling_thread != scheduled_thread) {
        pthread_mutex_unlock(&s_پورٽ_mutex);
        v_پورٽ_Yield();
        أرجع;
    }
    pthread_mutex_unlock(&s_پورٽ_mutex);
}

فراغ v_پورٽ_Disable_Interrupts(فراغ)
{
    v_پورٽ_Enter_Critical();
}

فراغ v_پورٽ_Enable_Interrupts(فراغ)
{
    v_پورٽ_Exit_Critical();
}

بنياد_Type_t x_پورٽ_سيٽ_مداخلت_نقاب(فراغ)
{
    v_پورٽ_Enter_Critical();
    أرجع pdTRUE;
}

فراغ v_پورٽ_Clear_مداخلت_نقاب(بنياد_Type_t x_نقاب)
{
    v_پورٽ_Exit_Critical();
}

فراغ v_پورٽ_Thread_Dying(فراغ *px_Task_منزل_Delete, متطاير بنياد_Type_t *pxPendYield)
{
    pthread_mutex_lock(&s_پورٽ_mutex);

    thread_t *thread = linux_پورٽ_get_thread_from_handle((TaskHandle_t)px_Task_منزل_Delete);
    إذا (thread == عدم) {
        pthread_mutex_unlock(&s_پورٽ_mutex);
        أرجع;
    }

    /* Mark the thread as dying, cancel the thread. the pthread
     * will be stopped on next cancellation point. Do not remove the
     * thread item from the list since it will be done in vPortCancelThread */
    thread->is_dying = true;
    pthread_cancel(thread->pthread);

    pthread_mutex_unlock(&s_پورٽ_mutex);
}

#إذا CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
ساكن فراغ v_پورٽ_TLSPointers_Del_Cb(فراغ *pxTCB)
{
    StaticTask_t *tcb = (StaticTask_t *)pxTCB;
    TlsDeleteCallbackFunction_t *pvDelCbs = (TlsDeleteCallbackFunction_t *)(&tcb->pvDummy15[config_انگ_THREAD_LOCAL_STORAGE_اشارا / 2]);

    لكل (صحيح x = 0; x < (config_انگ_THREAD_LOCAL_STORAGE_اشارا / 2); x++) {
        إذا (pvDelCbs[x] != عدم) {
            pvDelCbs[x](x, tcb->pvDummy15[x]);
        }
    }
}
#جيڪڏهن_پورو /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

فراغ v_پورٽ_Cancel_Thread(فراغ *px_Task_منزل_Delete)
{
    pthread_mutex_lock(&s_پورٽ_mutex);

#إذا CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
    v_پورٽ_TLSPointers_Del_Cb(px_Task_منزل_Delete);
#جيڪڏهن_پورو

    thread_t *thread = linux_پورٽ_get_thread_from_handle((TaskHandle_t)px_Task_منزل_Delete);
    إذا (!thread) {
        pthread_mutex_unlock(&s_پورٽ_mutex);
        أرجع;
    }

    إذا (thread->is_dying) {
        /* vPortThreadDying already called */
    } وإلا {
        thread->is_dying = true;
        pthread_cancel(thread->pthread);
    }

    /* Save fields and unregister while holding the lock. */
    pthread_t pt = thread->pthread;
    event_t *ev = thread->ev;
    linux_پورٽ_unregister_thread((TaskHandle_t)px_Task_منزل_Delete);

    /* Release the mutex before joining – the dying thread may need the
     * scheduler (which also takes s_port_mutex) to reach a cancellation
     * point. */
    pthread_mutex_unlock(&s_پورٽ_mutex);

    pthread_join(pt, عدم);
    event_delete(ev);
    حرر(thread);
}

فراغ v_پورٽ_سيٽ_ڍير_Watchpoint(فراغ *px_ڍير_شروعات)
{
}

#إذا ( CONFIG_FREERTOS_GENERATE_لاڳيتو_وقت_STATS )
config_لاڳيتو_وقت_COUNTER_TYPE x_پورٽ_Get_لاڳيتو_وقت_Counter_قدر( فراغ )
{
    بنية timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);

    أرجع ( config_لاڳيتو_وقت_COUNTER_TYPE ) ( ( ( uint64_t ) ts.tv_sec * 1000000ULL ) +
                                            ( ( uint64_t ) ts.tv_nsec / 1000ULL ) );
}
#جيڪڏهن_پورو /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
