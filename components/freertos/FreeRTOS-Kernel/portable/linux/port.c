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

#تعریف_کرو FREERTOS_SIM_TICK_PERIOD_US (1000000 / CONFIG_FREERTOS_HZ)

تعريفنوع بنية thread {
    ثابت حرف *name;
    pthread_t pthread;
    TaskFunction_t px_کوڈ;
    فراغ *pvParams;
    bool is_dying;
    bool yield_needed;
    بنية event *ev;
} thread_t;

تعريفنوع بنية task_thread_گرہ {
    TaskHandle_t handle;
    thread_t *thread;
    SLIST_ENTRY(task_thread_گرہ) اگلا;
} task_thread_گرہ_t;


ساكن SLIST_HEAD(task_thread_گرہ_ll, task_thread_گرہ) s_task_thread_list = SLIST_HEAD_INITIALIZER(task_thread_گرہ);
ساكن pthread_mutex_t s_thread_map_mutex = PTHREAD_MUTEX_INITIALIZER;

ساكن pthread_mutex_t s_پورٹ_mutex;
ساكن pthread_t s_scheduler_thread;
ساكن bool s_scheduler_started = false;
ساكن صحيح s_ux_critical_nesting = 0;

/* TLS flag: true only when inside a real FreeRTOS task pthread */
ساكن __thread bool s_وچ_freertos_task = false;

bool linux_پورٹ_وچ_freertos_task(فراغ)
{
    أرجع s_وچ_freertos_task;
}

ساكن فراغ linux_پورٹ_initialize_mutexes(فراغ)
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&s_پورٹ_mutex, &attr);
    pthread_mutexattr_destroy(&attr);
}

ساكن فراغ linux_پورٹ_fatal_error(ثابت حرف *پیغام, صحيح err)
{
    fprintf(stderr, "%s: %s\n", پیغام, strerror(err));
    abort();
}

ساكن فراغ linux_پورٹ_رجسٹر_thread(TaskHandle_t handle, thread_t *thread)
{
    إذا (handle == عدم) {
        أرجع;
    }

    task_thread_گرہ_t *گرہ = حافظہ_دیو(حجم(task_thread_گرہ_t));
    إذا (!گرہ) {
        linux_پورٹ_fatal_error("Failed to allocate thread map node", -1);
    }

    گرہ->handle = handle;
    گرہ->thread = thread;

    pthread_mutex_lock(&s_thread_map_mutex);
    SLIST_INSERT_HEAD(&s_task_thread_list, گرہ, اگلا);
    pthread_mutex_unlock(&s_thread_map_mutex);
}

ساكن فراغ linux_پورٹ_unregister_thread(TaskHandle_t handle)
{
    إذا (handle == عدم) {
        أرجع;
    }

    pthread_mutex_lock(&s_thread_map_mutex);

    task_thread_گرہ_t *cur_گرہ = SLIST_FIRST(&s_task_thread_list);
    task_thread_گرہ_t *prev_گرہ = عدم;

    طالما (cur_گرہ) {
        إذا (cur_گرہ->handle == handle) {
            إذا (prev_گرہ) {
                prev_گرہ->اگلا.sle_اگلا = SLIST_NEXT(cur_گرہ, اگلا);
            } وإلا {
                SLIST_REMOVE_HEAD(&s_task_thread_list, اگلا);
            }

            حرر(cur_گرہ);
            pthread_mutex_unlock(&s_thread_map_mutex);
            أرجع;
        }

        prev_گرہ = cur_گرہ;
        cur_گرہ = SLIST_NEXT(cur_گرہ, اگلا);
    }

    pthread_mutex_unlock(&s_thread_map_mutex);
}

ساكن thread_t *linux_پورٹ_get_thread_from_handle(TaskHandle_t handle)
{
    إذا (handle == عدم) {
        أرجع عدم;
    }

    pthread_mutex_lock(&s_thread_map_mutex);
    task_thread_گرہ_t *گرہ = عدم;
    SLIST_FOREACH(گرہ, &s_task_thread_list, اگلا) {
        إذا (گرہ->handle == (TaskHandle_t)(*(ڈھیر_Type_t **)(handle))) {
            thread_t *t = گرہ->thread;
            pthread_mutex_unlock(&s_thread_map_mutex);
            أرجع t;
        }
    }
    pthread_mutex_unlock(&s_thread_map_mutex);
    أرجع عدم;
}

ساكن thread_t *linux_پورٹ_get_calling_thread(فراغ)
{
    pthread_t self = pthread_self();
    pthread_mutex_lock(&s_thread_map_mutex);
    task_thread_گرہ_t *گرہ = عدم;
    SLIST_FOREACH(گرہ, &s_task_thread_list, اگلا) {
        إذا (pthread_equal(گرہ->thread->pthread, self)) {
            thread_t *thread = گرہ->thread;
            pthread_mutex_unlock(&s_thread_map_mutex);
            أرجع thread;
        }
    }
    pthread_mutex_unlock(&s_thread_map_mutex);
    أرجع عدم;
}

pthread_t linux_پورٹ_get_scheduled_task_pthread(فراغ)
{
    thread_t *thread = linux_پورٹ_get_thread_from_handle(xTaskGetCurrentTaskHandle());
    أرجع thread ? thread->pthread : pthread_self();
}

ساكن فراغ *linux_پورٹ_task_runner(فراغ *arg)
{
    /* Allow this thread to be cancelled */
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, عدم);

    /* set the flag showing that this is a freertos task */
    s_وچ_freertos_task = true;

    /* setup the backtrace signal. ONLY triggered before abort so
     * it will not interfere with the simulation while its running */
    linux_پورٹ_تیاری_backtrace_signal();

    thread_t *thread = arg;

    /* Block until scheduler signals first time, then run the task body. */
    event_wait(thread->ev);
    thread->px_کوڈ(thread->pvParams);

    أرجع عدم;
}

ساكن فراغ linux_پورٹ_unblock_thread(thread_t *thread)
{
    event_signal(thread->ev);
}

ساكن فراغ linux_پورٹ_بلاک_thread(thread_t *thread)
{
    event_wait(thread->ev);
}

ساكن فراغ linux_پورٹ_increment_tick(فراغ)
{
    (فراغ)xTaskIncrementTick();
}

ساكن فراغ linux_پورٹ_switch_context(TaskHandle_t current_task_hdl)
{
    pthread_mutex_lock(&s_پورٹ_mutex);

    thread_t *current_thread = linux_پورٹ_get_thread_from_handle(current_task_hdl);

    /* get the task that should be scheduled next */
    vTaskSwitchContext();

    /* get the new task to schedule and the associated thread item */
    TaskHandle_t اگلا_task_hdl = xTaskGetCurrentTaskHandle();
    thread_t *اگلا_thread = linux_پورٹ_get_thread_from_handle(اگلا_task_hdl);

    /* unblock the newly scheduled task if it is different from
     * the one already scheduled */
    إذا (اگلا_thread) {
        /* Only unblock the thread if we are actually switching to a
         * different one. Signaling the already-running thread would
         * leave a stale event_triggered flag, causing its next
         * event_wait (e.g. in vPortYield) to return immediately
         * instead of blocking.
         *
         * Exception: on the very first switch, the task is still blocked
         * in its initial event_wait (linux_port_task_runner), so we must
         * signal it even though current_thread == next_thread. */
        إذا (اگلا_thread != current_thread || !s_scheduler_started) {
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
            linux_پورٹ_unblock_thread(اگلا_thread);
        }
    }

    إذا (!s_scheduler_started) {
        s_scheduler_started = true;
    }

    /* fill the name of the task in the thread item if not done already. */
    إذا (اگلا_thread && اگلا_thread->name == عدم) {
        /* fill the name of the thread now */
        اگلا_thread->name = pcTaskGetName(اگلا_task_hdl);
    }

    /* fill the name of the task in the thread item if not done already. */
    إذا (current_thread && current_thread->name == عدم) {
        /* fill the name of the thread now */
        current_thread->name = pcTaskGetName(current_task_hdl);
    }

    pthread_mutex_unlock(&s_پورٹ_mutex);
}

ساكن فراغ *linux_پورٹ_scheduler_runner(فراغ *arg)
{
    (فراغ)arg;

    طالما (1) {
        /* sleep for a period of 1 tick */
        usleep(FREERTOS_SIM_TICK_PERIOD_US);

        /* get the task that is currently scheduled */
        TaskHandle_t current_task_hdl = xTaskGetCurrentTaskHandle();

        /* Lock the port mutex. This will block while any task is in a
         * critical section, ensuring ticks don't preempt critical code. */
        pthread_mutex_lock(&s_پورٹ_mutex);

        /* increment the freertos tick */
        linux_پورٹ_increment_tick();

        /* schedule a new task, and schedule out the currently running one */
        linux_پورٹ_switch_context(current_task_hdl);

        pthread_mutex_unlock(&s_پورٹ_mutex);
    }
    أرجع عدم;
}

ڈھیر_Type_t *px_پورٹ_Initialise_ڈھیر(ڈھیر_Type_t *px_چوٹی_دا_ڈھیر,
                                   ڈھیر_Type_t *px_سرا_دا_ڈھیر,
                                   TaskFunction_t px_کوڈ,
                                   فراغ *pvParameters)
{
    pthread_attr_t thread_attr;
    size_t thread_ڈھیر_ناپ;

    /* Store the thread data at the start of the stack. */
    thread_ڈھیر_ناپ = (px_چوٹی_دا_ڈھیر - px_سرا_دا_ڈھیر) * حجم(*px_چوٹی_دا_ڈھیر);
    pthread_attr_init(&thread_attr);
    pthread_attr_setstack(&thread_attr, px_سرا_دا_ڈھیر, thread_ڈھیر_ناپ);

    thread_t *thread = حافظہ_دیو(حجم(thread_t));
    إذا (!thread) {
        linux_پورٹ_fatal_error("Failed to allocate thread metadata", -1);
    }
    thread->name = عدم; // this will be filled later when we know about the task name
    thread->px_کوڈ = px_کوڈ;
    thread->pvParams = pvParameters;
    thread->is_dying = false;
    thread->yield_needed = false;
    thread->ev = event_create();

    linux_پورٹ_رجسٹر_thread((TaskHandle_t)px_چوٹی_دا_ڈھیر, thread);

    /* create the thread associated with the task being created */
    ثابت صحيح ret = pthread_create(&thread->pthread, &thread_attr, linux_پورٹ_task_runner, thread);
    إذا (ret != 0) {
        linux_پورٹ_fatal_error("pthread_create", ret);
    }
    أرجع px_چوٹی_دا_ڈھیر;
}

ادھار_Type_t x_پورٹ_شروع_Scheduler(فراغ)
{
    /* set the port mutex to be recursive. Must be done before
     * vPortEnableInterrupts() which calls vPortExitCritical(). */
    linux_پورٹ_initialize_mutexes();

    /* enable interrupt that were disabled in vTaskStartScheduler */
    v_پورٹ_Enable_Interrupts();

    /* init the cooperative syscall layer (sets stdio non-blocking).
     * Provided by VFS component; weak no-op when VFS is not linked. */
    freertos_linux_coop_syscalls_init();

    /* Start scheduler thread */
    صحيح ret = pthread_create(&s_scheduler_thread, عدم, linux_پورٹ_scheduler_runner, عدم);
    إذا (ret != 0) {
       linux_پورٹ_fatal_error("pthread_create", ret);
    }

    /* Should never return */
    pthread_join(s_scheduler_thread, عدم);

    أرجع 0;
}

فراغ v_پورٹ_سرا_Scheduler(فراغ)
{
    باہر_نکلو(0);
}

فراغ v_پورٹ_Enter_Critical(فراغ)
{
    إذا (!s_scheduler_started) {
        أرجع;
    }

    pthread_mutex_lock(&s_پورٹ_mutex);

    /* Non-FreeRTOS thread or recursive enter: just bump the counter.
     * The mutex is already held (recursive lock succeeds for same thread). */
    إذا (!linux_پورٹ_وچ_freertos_task() || s_ux_critical_nesting > 0) {
        s_ux_critical_nesting++;
        أرجع;
    }

    /* First enter from a FreeRTOS task: ensure we're the scheduled task.
     * If not, release the mutex, block until the scheduler switches to us,
     * then re-acquire. */
    thread_t *calling_thread = linux_پورٹ_get_calling_thread();
    thread_t *scheduled_thread = linux_پورٹ_get_thread_from_handle(xTaskGetCurrentTaskHandle());

    طالما (calling_thread && !calling_thread->is_dying && calling_thread != scheduled_thread) {
        pthread_mutex_unlock(&s_پورٹ_mutex);
        linux_پورٹ_بلاک_thread(calling_thread);
        pthread_mutex_lock(&s_پورٹ_mutex);
        calling_thread = linux_پورٹ_get_calling_thread();
        scheduled_thread = linux_پورٹ_get_thread_from_handle(xTaskGetCurrentTaskHandle());
    }

    إذا (!calling_thread || calling_thread->is_dying) {
        linux_پورٹ_switch_context(xTaskGetCurrentTaskHandle());
        pthread_mutex_unlock(&s_پورٹ_mutex);
        أرجع;
    }

    s_ux_critical_nesting = 1;
}

فراغ v_پورٹ_Exit_Critical(فراغ)
{
    إذا (!s_scheduler_started || s_ux_critical_nesting == 0) {
        أرجع;
    }

    s_ux_critical_nesting--;

    /* Check for deferred yield on final exit from a FreeRTOS task */
    إذا (s_ux_critical_nesting == 0 && linux_پورٹ_وچ_freertos_task()) {
        thread_t *calling_thread = linux_پورٹ_get_calling_thread();
        إذا (calling_thread && calling_thread->yield_needed) {
            calling_thread->yield_needed = false;
            pthread_mutex_unlock(&s_پورٹ_mutex);
            v_پورٹ_Yield();
            أرجع;
        }
    }

    pthread_mutex_unlock(&s_پورٹ_mutex);
}

/* Handle the case where the calling pthread is not a registered FreeRTOS task.
 * If the calling thread has been deleted but is still the scheduled task,
 * perform a context switch. Otherwise just release the mutex and return.
 * Returns true if the caller should return early. */
ساكن bool linux_پورٹ_handle_deleted_task(TaskHandle_t scheduled_task_hdl,
                                           thread_t *calling_thread)
{
    إذا (calling_thread != عدم) {
        أرجع false;
    }

    thread_t *scheduled_thread = linux_پورٹ_get_thread_from_handle(scheduled_task_hdl);
    إذا (scheduled_thread == عدم) {
        linux_پورٹ_switch_context(scheduled_task_hdl);
    }
    pthread_mutex_unlock(&s_پورٹ_mutex);
    أرجع true;
}

فراغ v_پورٹ_Yield(فراغ)
{
    pthread_mutex_lock(&s_پورٹ_mutex);

    thread_t *calling_thread = linux_پورٹ_get_calling_thread();
    TaskHandle_t scheduled_task_hdl = xTaskGetCurrentTaskHandle();

    إذا (linux_پورٹ_handle_deleted_task(scheduled_task_hdl, calling_thread)) {
        أرجع;
    }

    /* If in a critical section, defer the yield until the section exits. */
    إذا (s_ux_critical_nesting != 0) {
        calling_thread->yield_needed = true;
        pthread_mutex_unlock(&s_پورٹ_mutex);
        أرجع;
    }

    /* Hand the CPU to the next ready task right now (mimics PendSV on real
     * hardware). */
    linux_پورٹ_switch_context(scheduled_task_hdl);
    pthread_mutex_unlock(&s_پورٹ_mutex);

    /* If the newly scheduled task is different from the calling thread,
     * block until the scheduler resumes this task. */
    TaskHandle_t اگلا_task_hdl = xTaskGetCurrentTaskHandle();
    thread_t *اگلا_thread = linux_پورٹ_get_thread_from_handle(اگلا_task_hdl);
    إذا (calling_thread != اگلا_thread) {
        linux_پورٹ_بلاک_thread(calling_thread);
    }
}

فراغ v_پورٹ_Yield_Within_Api(فراغ)
{
    v_پورٹ_Yield();
}

فراغ v_پورٹ_Suspend_Scheduler(فراغ)
{
    /* scheduled out task trying to suspend the scheduler should get blocked here */
    pthread_mutex_lock(&s_پورٹ_mutex);

   /* get the metadata of the pthread calling this function */
    thread_t *calling_thread = linux_پورٹ_get_calling_thread();

    /* get the thread metadata from the scheduled task */
    TaskHandle_t scheduled_task_hdl = xTaskGetCurrentTaskHandle();

    إذا (linux_پورٹ_handle_deleted_task(scheduled_task_hdl, calling_thread)) {
        أرجع;
    }

    thread_t *scheduled_thread = linux_پورٹ_get_thread_from_handle(scheduled_task_hdl);
    إذا (calling_thread != scheduled_thread) {
        pthread_mutex_unlock(&s_پورٹ_mutex);
        v_پورٹ_Yield();
        أرجع;
    }
    pthread_mutex_unlock(&s_پورٹ_mutex);
}

فراغ v_پورٹ_Disable_Interrupts(فراغ)
{
    v_پورٹ_Enter_Critical();
}

فراغ v_پورٹ_Enable_Interrupts(فراغ)
{
    v_پورٹ_Exit_Critical();
}

ادھار_Type_t x_پورٹ_سیٹ_رکاوٹ_نقاب(فراغ)
{
    v_پورٹ_Enter_Critical();
    أرجع pdTRUE;
}

فراغ v_پورٹ_Clear_رکاوٹ_نقاب(ادھار_Type_t x_نقاب)
{
    v_پورٹ_Exit_Critical();
}

فراغ v_پورٹ_Thread_Dying(فراغ *px_Task_منزل_Delete, متطاير ادھار_Type_t *pxPendYield)
{
    pthread_mutex_lock(&s_پورٹ_mutex);

    thread_t *thread = linux_پورٹ_get_thread_from_handle((TaskHandle_t)px_Task_منزل_Delete);
    إذا (thread == عدم) {
        pthread_mutex_unlock(&s_پورٹ_mutex);
        أرجع;
    }

    /* Mark the thread as dying, cancel the thread. the pthread
     * will be stopped on next cancellation point. Do not remove the
     * thread item from the list since it will be done in vPortCancelThread */
    thread->is_dying = true;
    pthread_cancel(thread->pthread);

    pthread_mutex_unlock(&s_پورٹ_mutex);
}

#إذا CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
ساكن فراغ v_پورٹ_TLSPointers_Del_Cb(فراغ *pxTCB)
{
    StaticTask_t *tcb = (StaticTask_t *)pxTCB;
    TlsDeleteCallbackFunction_t *pvDelCbs = (TlsDeleteCallbackFunction_t *)(&tcb->pvDummy15[config_عدد_THREAD_LOCAL_STORAGE_سوچکاں / 2]);

    لكل (صحيح x = 0; x < (config_عدد_THREAD_LOCAL_STORAGE_سوچکاں / 2); x++) {
        إذا (pvDelCbs[x] != عدم) {
            pvDelCbs[x](x, tcb->pvDummy15[x]);
        }
    }
}
#جے_انت /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

فراغ v_پورٹ_Cancel_Thread(فراغ *px_Task_منزل_Delete)
{
    pthread_mutex_lock(&s_پورٹ_mutex);

#إذا CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
    v_پورٹ_TLSPointers_Del_Cb(px_Task_منزل_Delete);
#جے_انت

    thread_t *thread = linux_پورٹ_get_thread_from_handle((TaskHandle_t)px_Task_منزل_Delete);
    إذا (!thread) {
        pthread_mutex_unlock(&s_پورٹ_mutex);
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
    linux_پورٹ_unregister_thread((TaskHandle_t)px_Task_منزل_Delete);

    /* Release the mutex before joining – the dying thread may need the
     * scheduler (which also takes s_port_mutex) to reach a cancellation
     * point. */
    pthread_mutex_unlock(&s_پورٹ_mutex);

    pthread_join(pt, عدم);
    event_delete(ev);
    حرر(thread);
}

فراغ v_پورٹ_سیٹ_ڈھیر_Watchpoint(فراغ *px_ڈھیر_شروع)
{
}

#إذا ( CONFIG_FREERTOS_GENERATE_لگاتار_ویلا_STATS )
config_لگاتار_ویلا_COUNTER_TYPE x_پورٹ_Get_لگاتار_ویلا_Counter_قدر( فراغ )
{
    بنية timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);

    أرجع ( config_لگاتار_ویلا_COUNTER_TYPE ) ( ( ( uint64_t ) ts.tv_sec * 1000000ULL ) +
                                            ( ( uint64_t ) ts.tv_nsec / 1000ULL ) );
}
#جے_انت /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
