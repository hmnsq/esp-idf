/*
 * SPDX-FileCopyrightText: 2025-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <stdint.h>
#شامل <unistd.h>
#شامل <pthread.h>
#شامل <sys/queue.h>
#شامل <time.h>
#شامل "string.h"
#شامل "FreeRTOS.h"
#شامل "task.h"
#شامل "utils/wait_for_event.h"
#شامل "esp_private/freertos_linux_coop_syscalls.h"
#شامل "utils/linux_port_utils.h"

#تعریف_کرو FREERTOS_SIM_TICK_PERIOD_US (1000000 / CONFIG_FREERTOS_HZ)

قسمتعریف ساخت thread {
    ثابت حرف *name;
    pthread_t pthread;
    TaskFunction_t px_رمز;
    خالی *pvParams;
    bool is_dying;
    bool yield_needed;
    ساخت event *ev;
} thread_t;

قسمتعریف ساخت task_thread_گرہ {
    TaskHandle_t handle;
    thread_t *thread;
    SLIST_ENTRY(task_thread_گرہ) اگلا;
} task_thread_گرہ_t;


ساکن SLIST_HEAD(task_thread_گرہ_ll, task_thread_گرہ) s_task_thread_list = SLIST_HEAD_INITIALIZER(task_thread_گرہ);
ساکن pthread_mutex_t s_thread_map_mutex = PTHREAD_MUTEX_INITIALIZER;

ساکن pthread_mutex_t s_پورٹ_mutex;
ساکن pthread_t s_scheduler_thread;
ساکن bool s_scheduler_started = false;
ساکن صحیح s_ux_critical_nesting = 0;

/* TLS flag: true only when inside a real FreeRTOS task pthread */
ساکن __thread bool s_اندر_freertos_task = false;

bool linux_پورٹ_اندر_freertos_task(خالی)
{
    واپس s_اندر_freertos_task;
}

ساکن خالی linux_پورٹ_initialize_mutexes(خالی)
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&s_پورٹ_mutex, &attr);
    pthread_mutexattr_destroy(&attr);
}

ساکن خالی linux_پورٹ_fatal_error(ثابت حرف *پیغام, صحیح err)
{
    fprintf(stderr, "%s: %s\n", پیغام, strerror(err));
    abort();
}

ساکن خالی linux_پورٹ_رجسٹر_thread(TaskHandle_t handle, thread_t *thread)
{
    اگر (handle == تہی) {
        واپس;
    }

    task_thread_گرہ_t *گرہ = حافظہ_دو(حجم(task_thread_گرہ_t));
    اگر (!گرہ) {
        linux_پورٹ_fatal_error("Failed to allocate thread map node", -1);
    }

    گرہ->handle = handle;
    گرہ->thread = thread;

    pthread_mutex_lock(&s_thread_map_mutex);
    SLIST_INSERT_HEAD(&s_task_thread_list, گرہ, اگلا);
    pthread_mutex_unlock(&s_thread_map_mutex);
}

ساکن خالی linux_پورٹ_unregister_thread(TaskHandle_t handle)
{
    اگر (handle == تہی) {
        واپس;
    }

    pthread_mutex_lock(&s_thread_map_mutex);

    task_thread_گرہ_t *cur_گرہ = SLIST_FIRST(&s_task_thread_list);
    task_thread_گرہ_t *پچھلا_گرہ = تہی;

    جب (cur_گرہ) {
        اگر (cur_گرہ->handle == handle) {
            اگر (پچھلا_گرہ) {
                پچھلا_گرہ->اگلا.sle_اگلا = SLIST_NEXT(cur_گرہ, اگلا);
            } ورنہ {
                SLIST_REMOVE_HEAD(&s_task_thread_list, اگلا);
            }

            آزاد(cur_گرہ);
            pthread_mutex_unlock(&s_thread_map_mutex);
            واپس;
        }

        پچھلا_گرہ = cur_گرہ;
        cur_گرہ = SLIST_NEXT(cur_گرہ, اگلا);
    }

    pthread_mutex_unlock(&s_thread_map_mutex);
}

ساکن thread_t *linux_پورٹ_get_thread_from_handle(TaskHandle_t handle)
{
    اگر (handle == تہی) {
        واپس تہی;
    }

    pthread_mutex_lock(&s_thread_map_mutex);
    task_thread_گرہ_t *گرہ = تہی;
    SLIST_FOREACH(گرہ, &s_task_thread_list, اگلا) {
        اگر (گرہ->handle == (TaskHandle_t)(*(ڈھیر_Type_t **)(handle))) {
            thread_t *t = گرہ->thread;
            pthread_mutex_unlock(&s_thread_map_mutex);
            واپس t;
        }
    }
    pthread_mutex_unlock(&s_thread_map_mutex);
    واپس تہی;
}

ساکن thread_t *linux_پورٹ_get_calling_thread(خالی)
{
    pthread_t self = pthread_self();
    pthread_mutex_lock(&s_thread_map_mutex);
    task_thread_گرہ_t *گرہ = تہی;
    SLIST_FOREACH(گرہ, &s_task_thread_list, اگلا) {
        اگر (pthread_equal(گرہ->thread->pthread, self)) {
            thread_t *thread = گرہ->thread;
            pthread_mutex_unlock(&s_thread_map_mutex);
            واپس thread;
        }
    }
    pthread_mutex_unlock(&s_thread_map_mutex);
    واپس تہی;
}

pthread_t linux_پورٹ_get_scheduled_task_pthread(خالی)
{
    thread_t *thread = linux_پورٹ_get_thread_from_handle(xTaskGetCurrentTaskHandle());
    واپس thread ? thread->pthread : pthread_self();
}

ساکن خالی *linux_پورٹ_task_runner(خالی *arg)
{
    /* Allow this thread to be cancelled */
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, تہی);

    /* set the flag showing that this is a freertos task */
    s_اندر_freertos_task = true;

    /* setup the backtrace signal. ONLY triggered before abort so
     * it will not interfere with the simulation while its running */
    linux_پورٹ_تیاری_backtrace_signal();

    thread_t *thread = arg;

    /* Block until scheduler signals first time, then run the task body. */
    event_wait(thread->ev);
    thread->px_رمز(thread->pvParams);

    واپس تہی;
}

ساکن خالی linux_پورٹ_unblock_thread(thread_t *thread)
{
    event_signal(thread->ev);
}

ساکن خالی linux_پورٹ_بلاک_thread(thread_t *thread)
{
    event_wait(thread->ev);
}

ساکن خالی linux_پورٹ_increment_tick(خالی)
{
    (خالی)xTaskIncrementTick();
}

ساکن خالی linux_پورٹ_switch_context(TaskHandle_t current_task_hdl)
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
    اگر (اگلا_thread) {
        /* Only unblock the thread if we are actually switching to a
         * different one. Signaling the already-running thread would
         * leave a stale event_triggered flag, causing its next
         * event_wait (e.g. in vPortYield) to return immediately
         * instead of blocking.
         *
         * Exception: on the very first switch, the task is still blocked
         * in its initial event_wait (linux_port_task_runner), so we must
         * signal it even though current_thread == next_thread. */
        اگر (اگلا_thread != current_thread || !s_scheduler_started) {
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
            اگر (current_thread) {
                event_clear(current_thread->ev);
            }
            linux_پورٹ_unblock_thread(اگلا_thread);
        }
    }

    اگر (!s_scheduler_started) {
        s_scheduler_started = true;
    }

    /* fill the name of the task in the thread item if not done already. */
    اگر (اگلا_thread && اگلا_thread->name == تہی) {
        /* fill the name of the thread now */
        اگلا_thread->name = pcTaskGetName(اگلا_task_hdl);
    }

    /* fill the name of the task in the thread item if not done already. */
    اگر (current_thread && current_thread->name == تہی) {
        /* fill the name of the thread now */
        current_thread->name = pcTaskGetName(current_task_hdl);
    }

    pthread_mutex_unlock(&s_پورٹ_mutex);
}

ساکن خالی *linux_پورٹ_scheduler_runner(خالی *arg)
{
    (خالی)arg;

    جب (1) {
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
    واپس تہی;
}

ڈھیر_Type_t *px_پورٹ_Initialise_ڈھیر(ڈھیر_Type_t *px_چوٹی_کا_ڈھیر,
                                   ڈھیر_Type_t *px_انتہا_کا_ڈھیر,
                                   TaskFunction_t px_رمز,
                                   خالی *pvParameters)
{
    pthread_attr_t thread_attr;
    size_t thread_ڈھیر_سائز;

    /* Store the thread data at the start of the stack. */
    thread_ڈھیر_سائز = (px_چوٹی_کا_ڈھیر - px_انتہا_کا_ڈھیر) * حجم(*px_چوٹی_کا_ڈھیر);
    pthread_attr_init(&thread_attr);
    pthread_attr_setstack(&thread_attr, px_انتہا_کا_ڈھیر, thread_ڈھیر_سائز);

    thread_t *thread = حافظہ_دو(حجم(thread_t));
    اگر (!thread) {
        linux_پورٹ_fatal_error("Failed to allocate thread metadata", -1);
    }
    thread->name = تہی; // this will be filled later when we know about the task name
    thread->px_رمز = px_رمز;
    thread->pvParams = pvParameters;
    thread->is_dying = false;
    thread->yield_needed = false;
    thread->ev = event_create();

    linux_پورٹ_رجسٹر_thread((TaskHandle_t)px_چوٹی_کا_ڈھیر, thread);

    /* create the thread associated with the task being created */
    ثابت صحیح ret = pthread_create(&thread->pthread, &thread_attr, linux_پورٹ_task_runner, thread);
    اگر (ret != 0) {
        linux_پورٹ_fatal_error("pthread_create", ret);
    }
    واپس px_چوٹی_کا_ڈھیر;
}

بنیاد_Type_t x_پورٹ_آغاز_Scheduler(خالی)
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
    صحیح ret = pthread_create(&s_scheduler_thread, تہی, linux_پورٹ_scheduler_runner, تہی);
    اگر (ret != 0) {
       linux_پورٹ_fatal_error("pthread_create", ret);
    }

    /* Should never return */
    pthread_join(s_scheduler_thread, تہی);

    واپس 0;
}

خالی v_پورٹ_انتہا_Scheduler(خالی)
{
    باہر_نکلو(0);
}

خالی v_پورٹ_Enter_Critical(خالی)
{
    اگر (!s_scheduler_started) {
        واپس;
    }

    pthread_mutex_lock(&s_پورٹ_mutex);

    /* Non-FreeRTOS thread or recursive enter: just bump the counter.
     * The mutex is already held (recursive lock succeeds for same thread). */
    اگر (!linux_پورٹ_اندر_freertos_task() || s_ux_critical_nesting > 0) {
        s_ux_critical_nesting++;
        واپس;
    }

    /* First enter from a FreeRTOS task: ensure we're the scheduled task.
     * If not, release the mutex, block until the scheduler switches to us,
     * then re-acquire. */
    thread_t *calling_thread = linux_پورٹ_get_calling_thread();
    thread_t *scheduled_thread = linux_پورٹ_get_thread_from_handle(xTaskGetCurrentTaskHandle());

    جب (calling_thread && !calling_thread->is_dying && calling_thread != scheduled_thread) {
        pthread_mutex_unlock(&s_پورٹ_mutex);
        linux_پورٹ_بلاک_thread(calling_thread);
        pthread_mutex_lock(&s_پورٹ_mutex);
        calling_thread = linux_پورٹ_get_calling_thread();
        scheduled_thread = linux_پورٹ_get_thread_from_handle(xTaskGetCurrentTaskHandle());
    }

    اگر (!calling_thread || calling_thread->is_dying) {
        linux_پورٹ_switch_context(xTaskGetCurrentTaskHandle());
        pthread_mutex_unlock(&s_پورٹ_mutex);
        واپس;
    }

    s_ux_critical_nesting = 1;
}

خالی v_پورٹ_Exit_Critical(خالی)
{
    اگر (!s_scheduler_started || s_ux_critical_nesting == 0) {
        واپس;
    }

    s_ux_critical_nesting--;

    /* Check for deferred yield on final exit from a FreeRTOS task */
    اگر (s_ux_critical_nesting == 0 && linux_پورٹ_اندر_freertos_task()) {
        thread_t *calling_thread = linux_پورٹ_get_calling_thread();
        اگر (calling_thread && calling_thread->yield_needed) {
            calling_thread->yield_needed = false;
            pthread_mutex_unlock(&s_پورٹ_mutex);
            v_پورٹ_Yield();
            واپس;
        }
    }

    pthread_mutex_unlock(&s_پورٹ_mutex);
}

/* Handle the case where the calling pthread is not a registered FreeRTOS task.
 * If the calling thread has been deleted but is still the scheduled task,
 * perform a context switch. Otherwise just release the mutex and return.
 * Returns true if the caller should return early. */
ساکن bool linux_پورٹ_handle_deleted_task(TaskHandle_t scheduled_task_hdl,
                                           thread_t *calling_thread)
{
    اگر (calling_thread != تہی) {
        واپس false;
    }

    thread_t *scheduled_thread = linux_پورٹ_get_thread_from_handle(scheduled_task_hdl);
    اگر (scheduled_thread == تہی) {
        linux_پورٹ_switch_context(scheduled_task_hdl);
    }
    pthread_mutex_unlock(&s_پورٹ_mutex);
    واپس true;
}

خالی v_پورٹ_Yield(خالی)
{
    pthread_mutex_lock(&s_پورٹ_mutex);

    thread_t *calling_thread = linux_پورٹ_get_calling_thread();
    TaskHandle_t scheduled_task_hdl = xTaskGetCurrentTaskHandle();

    اگر (linux_پورٹ_handle_deleted_task(scheduled_task_hdl, calling_thread)) {
        واپس;
    }

    /* If in a critical section, defer the yield until the section exits. */
    اگر (s_ux_critical_nesting != 0) {
        calling_thread->yield_needed = true;
        pthread_mutex_unlock(&s_پورٹ_mutex);
        واپس;
    }

    /* Hand the CPU to the next ready task right now (mimics PendSV on real
     * hardware). */
    linux_پورٹ_switch_context(scheduled_task_hdl);
    pthread_mutex_unlock(&s_پورٹ_mutex);

    /* If the newly scheduled task is different from the calling thread,
     * block until the scheduler resumes this task. */
    TaskHandle_t اگلا_task_hdl = xTaskGetCurrentTaskHandle();
    thread_t *اگلا_thread = linux_پورٹ_get_thread_from_handle(اگلا_task_hdl);
    اگر (calling_thread != اگلا_thread) {
        linux_پورٹ_بلاک_thread(calling_thread);
    }
}

خالی v_پورٹ_Yield_Within_Api(خالی)
{
    v_پورٹ_Yield();
}

خالی v_پورٹ_Suspend_Scheduler(خالی)
{
    /* scheduled out task trying to suspend the scheduler should get blocked here */
    pthread_mutex_lock(&s_پورٹ_mutex);

   /* get the metadata of the pthread calling this function */
    thread_t *calling_thread = linux_پورٹ_get_calling_thread();

    /* get the thread metadata from the scheduled task */
    TaskHandle_t scheduled_task_hdl = xTaskGetCurrentTaskHandle();

    اگر (linux_پورٹ_handle_deleted_task(scheduled_task_hdl, calling_thread)) {
        واپس;
    }

    thread_t *scheduled_thread = linux_پورٹ_get_thread_from_handle(scheduled_task_hdl);
    اگر (calling_thread != scheduled_thread) {
        pthread_mutex_unlock(&s_پورٹ_mutex);
        v_پورٹ_Yield();
        واپس;
    }
    pthread_mutex_unlock(&s_پورٹ_mutex);
}

خالی v_پورٹ_Disable_Interrupts(خالی)
{
    v_پورٹ_Enter_Critical();
}

خالی v_پورٹ_Enable_Interrupts(خالی)
{
    v_پورٹ_Exit_Critical();
}

بنیاد_Type_t x_پورٹ_سیٹ_مداخلت_نقاب(خالی)
{
    v_پورٹ_Enter_Critical();
    واپس pdTRUE;
}

خالی v_پورٹ_Clear_مداخلت_نقاب(بنیاد_Type_t x_نقاب)
{
    v_پورٹ_Exit_Critical();
}

خالی v_پورٹ_Thread_Dying(خالی *px_Task_مقصد_Delete, متلون بنیاد_Type_t *pxPendYield)
{
    pthread_mutex_lock(&s_پورٹ_mutex);

    thread_t *thread = linux_پورٹ_get_thread_from_handle((TaskHandle_t)px_Task_مقصد_Delete);
    اگر (thread == تہی) {
        pthread_mutex_unlock(&s_پورٹ_mutex);
        واپس;
    }

    /* Mark the thread as dying, cancel the thread. the pthread
     * will be stopped on next cancellation point. Do not remove the
     * thread item from the list since it will be done in vPortCancelThread */
    thread->is_dying = true;
    pthread_cancel(thread->pthread);

    pthread_mutex_unlock(&s_پورٹ_mutex);
}

#اگر CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
ساکن خالی v_پورٹ_TLSPointers_Del_Cb(خالی *pxTCB)
{
    StaticTask_t *tcb = (StaticTask_t *)pxTCB;
    TlsDeleteCallbackFunction_t *pvDelCbs = (TlsDeleteCallbackFunction_t *)(&tcb->pvDummy15[config_عدد_THREAD_LOCAL_STORAGE_اشارے / 2]);

    برائے (صحیح x = 0; x < (config_عدد_THREAD_LOCAL_STORAGE_اشارے / 2); x++) {
        اگر (pvDelCbs[x] != تہی) {
            pvDelCbs[x](x, tcb->pvDummy15[x]);
        }
    }
}
#اگر_ختم /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

خالی v_پورٹ_Cancel_Thread(خالی *px_Task_مقصد_Delete)
{
    pthread_mutex_lock(&s_پورٹ_mutex);

#اگر CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
    v_پورٹ_TLSPointers_Del_Cb(px_Task_مقصد_Delete);
#اگر_ختم

    thread_t *thread = linux_پورٹ_get_thread_from_handle((TaskHandle_t)px_Task_مقصد_Delete);
    اگر (!thread) {
        pthread_mutex_unlock(&s_پورٹ_mutex);
        واپس;
    }

    اگر (thread->is_dying) {
        /* vPortThreadDying already called */
    } ورنہ {
        thread->is_dying = true;
        pthread_cancel(thread->pthread);
    }

    /* Save fields and unregister while holding the lock. */
    pthread_t pt = thread->pthread;
    event_t *ev = thread->ev;
    linux_پورٹ_unregister_thread((TaskHandle_t)px_Task_مقصد_Delete);

    /* Release the mutex before joining – the dying thread may need the
     * scheduler (which also takes s_port_mutex) to reach a cancellation
     * point. */
    pthread_mutex_unlock(&s_پورٹ_mutex);

    pthread_join(pt, تہی);
    event_delete(ev);
    آزاد(thread);
}

خالی v_پورٹ_سیٹ_ڈھیر_Watchpoint(خالی *px_ڈھیر_آغاز)
{
}

#اگر ( CONFIG_FREERTOS_GENERATE_تسلسل_وقت_STATS )
config_تسلسل_وقت_شمار_TYPE x_پورٹ_Get_تسلسل_وقت_شمار_قدر( خالی )
{
    ساخت timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);

    واپس ( config_تسلسل_وقت_شمار_TYPE ) ( ( ( uint64_t ) ts.tv_sec * 1000000ULL ) +
                                            ( ( uint64_t ) ts.tv_nsec / 1000ULL ) );
}
#اگر_ختم /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
