/*
 * SPDX-FileCopyrightText: 2025-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <stdint.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <pthread.h>
#अन्तर्भाव <sys/queue.h>
#अन्तर्भाव <time.h>
#अन्तर्भाव "string.h"
#अन्तर्भाव "FreeRTOS.h"
#अन्तर्भाव "task.h"
#अन्तर्भाव "utils/wait_for_event.h"
#अन्तर्भाव "esp_private/freertos_linux_coop_syscalls.h"
#अन्तर्भाव "utils/linux_port_utils.h"

#define FREERTOS_SIM_TICK_PERIOD_US (1000000 / CONFIG_FREERTOS_HZ)

प्रकारव्याख्या संरचना thread {
    स्थिरांक अक्षर *name;
    pthread_t pthread;
    TaskFunction_t pxCode;
    शून्य *pvParams;
    bool is_dying;
    bool yield_needed;
    संरचना event *ev;
} thread_t;

प्रकारव्याख्या संरचना task_thread_गाठ {
    TaskHandle_t handle;
    thread_t *thread;
    SLIST_ENTRY(task_thread_गाठ) next;
} task_thread_गाठ_t;


स्थिर SLIST_HEAD(task_thread_गाठ_ll, task_thread_गाठ) s_task_thread_list = SLIST_HEAD_INITIALIZER(task_thread_गाठ);
स्थिर pthread_mutex_t s_thread_map_mutex = PTHREAD_MUTEX_INITIALIZER;

स्थिर pthread_mutex_t s_port_mutex;
स्थिर pthread_t s_scheduler_thread;
स्थिर bool s_scheduler_started = false;
स्थिर पूर्णांक s_ux_critical_nesting = 0;

/* TLS flag: true only when inside a real FreeRTOS task pthread */
स्थिर __thread bool s_in_freertos_task = false;

bool linux_port_in_freertos_task(शून्य)
{
    परत s_in_freertos_task;
}

स्थिर शून्य linux_port_initialize_mutexes(शून्य)
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&s_port_mutex, &attr);
    pthread_mutexattr_destroy(&attr);
}

स्थिर शून्य linux_port_fatal_error(स्थिरांक अक्षर *संदेश, पूर्णांक err)
{
    ख_लिखो(मानक_त्रुटि, "%s: %s\n", संदेश, म_त्रुटि(err));
    पात();
}

स्थिर शून्य linux_port_register_thread(TaskHandle_t handle, thread_t *thread)
{
    जर (handle == रिक्त) {
        परत;
    }

    task_thread_गाठ_t *गाठ = दो_स्मृति(आकार(task_thread_गाठ_t));
    जर (!गाठ) {
        linux_port_fatal_error("Failed to allocate thread map node", -1);
    }

    गाठ->handle = handle;
    गाठ->thread = thread;

    pthread_mutex_lock(&s_thread_map_mutex);
    SLIST_INSERT_HEAD(&s_task_thread_list, गाठ, next);
    pthread_mutex_unlock(&s_thread_map_mutex);
}

स्थिर शून्य linux_port_unregister_thread(TaskHandle_t handle)
{
    जर (handle == रिक्त) {
        परत;
    }

    pthread_mutex_lock(&s_thread_map_mutex);

    task_thread_गाठ_t *cur_गाठ = SLIST_FIRST(&s_task_thread_list);
    task_thread_गाठ_t *prev_गाठ = रिक्त;

    पर्यंत (cur_गाठ) {
        जर (cur_गाठ->handle == handle) {
            जर (prev_गाठ) {
                prev_गाठ->next.sle_next = SLIST_NEXT(cur_गाठ, next);
            } अन्यथा {
                SLIST_REMOVE_HEAD(&s_task_thread_list, next);
            }

            मुक्त(cur_गाठ);
            pthread_mutex_unlock(&s_thread_map_mutex);
            परत;
        }

        prev_गाठ = cur_गाठ;
        cur_गाठ = SLIST_NEXT(cur_गाठ, next);
    }

    pthread_mutex_unlock(&s_thread_map_mutex);
}

स्थिर thread_t *linux_port_get_thread_from_handle(TaskHandle_t handle)
{
    जर (handle == रिक्त) {
        परत रिक्त;
    }

    pthread_mutex_lock(&s_thread_map_mutex);
    task_thread_गाठ_t *गाठ = रिक्त;
    SLIST_FOREACH(गाठ, &s_task_thread_list, next) {
        जर (गाठ->handle == (TaskHandle_t)(*(चळत_Type_t **)(handle))) {
            thread_t *t = गाठ->thread;
            pthread_mutex_unlock(&s_thread_map_mutex);
            परत t;
        }
    }
    pthread_mutex_unlock(&s_thread_map_mutex);
    परत रिक्त;
}

स्थिर thread_t *linux_port_get_calling_thread(शून्य)
{
    pthread_t self = pthread_self();
    pthread_mutex_lock(&s_thread_map_mutex);
    task_thread_गाठ_t *गाठ = रिक्त;
    SLIST_FOREACH(गाठ, &s_task_thread_list, next) {
        जर (pthread_equal(गाठ->thread->pthread, self)) {
            thread_t *thread = गाठ->thread;
            pthread_mutex_unlock(&s_thread_map_mutex);
            परत thread;
        }
    }
    pthread_mutex_unlock(&s_thread_map_mutex);
    परत रिक्त;
}

pthread_t linux_port_get_scheduled_task_pthread(शून्य)
{
    thread_t *thread = linux_port_get_thread_from_handle(xTaskGetCurrentTaskHandle());
    परत thread ? thread->pthread : pthread_self();
}

स्थिर शून्य *linux_port_task_runner(शून्य *arg)
{
    /* Allow this thread to be cancelled */
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, रिक्त);

    /* set the flag showing that this is a freertos task */
    s_in_freertos_task = true;

    /* setup the backtrace signal. ONLY triggered before abort so
     * it will not interfere with the simulation while its running */
    linux_port_setup_backtrace_signal();

    thread_t *thread = arg;

    /* Block until scheduler signals first time, then run the task body. */
    event_wait(thread->ev);
    thread->pxCode(thread->pvParams);

    परत रिक्त;
}

स्थिर शून्य linux_port_unblock_thread(thread_t *thread)
{
    event_signal(thread->ev);
}

स्थिर शून्य linux_port_block_thread(thread_t *thread)
{
    event_wait(thread->ev);
}

स्थिर शून्य linux_port_increment_tick(शून्य)
{
    (शून्य)xTaskIncrementTick();
}

स्थिर शून्य linux_port_switch_context(TaskHandle_t current_task_hdl)
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
    जर (next_thread) {
        /* Only unblock the thread if we are actually switching to a
         * different one. Signaling the already-running thread would
         * leave a stale event_triggered flag, causing its next
         * event_wait (e.g. in vPortYield) to return immediately
         * instead of blocking.
         *
         * Exception: on the very first switch, the task is still blocked
         * in its initial event_wait (linux_port_task_runner), so we must
         * signal it even though current_thread == next_thread. */
        जर (next_thread != current_thread || !s_scheduler_started) {
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
            जर (current_thread) {
                event_clear(current_thread->ev);
            }
            linux_port_unblock_thread(next_thread);
        }
    }

    जर (!s_scheduler_started) {
        s_scheduler_started = true;
    }

    /* fill the name of the task in the thread item if not done already. */
    जर (next_thread && next_thread->name == रिक्त) {
        /* fill the name of the thread now */
        next_thread->name = pcTaskGetName(next_task_hdl);
    }

    /* fill the name of the task in the thread item if not done already. */
    जर (current_thread && current_thread->name == रिक्त) {
        /* fill the name of the thread now */
        current_thread->name = pcTaskGetName(current_task_hdl);
    }

    pthread_mutex_unlock(&s_port_mutex);
}

स्थिर शून्य *linux_port_scheduler_runner(शून्य *arg)
{
    (शून्य)arg;

    पर्यंत (1) {
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
    परत रिक्त;
}

चळत_Type_t *px_Port_Initialise_चळत(चळत_Type_t *px_शीर्ष_Of_चळत,
                                   चळत_Type_t *px_End_Of_चळत,
                                   TaskFunction_t pxCode,
                                   शून्य *pvParameters)
{
    pthread_attr_t thread_attr;
    माप_प्रकार thread_चळत_size;

    /* Store the thread data at the start of the stack. */
    thread_चळत_size = (px_शीर्ष_Of_चळत - px_End_Of_चळत) * आकार(*px_शीर्ष_Of_चळत);
    pthread_attr_init(&thread_attr);
    pthread_attr_setstack(&thread_attr, px_End_Of_चळत, thread_चळत_size);

    thread_t *thread = दो_स्मृति(आकार(thread_t));
    जर (!thread) {
        linux_port_fatal_error("Failed to allocate thread metadata", -1);
    }
    thread->name = रिक्त; // this will be filled later when we know about the task name
    thread->pxCode = pxCode;
    thread->pvParams = pvParameters;
    thread->is_dying = false;
    thread->yield_needed = false;
    thread->ev = event_create();

    linux_port_register_thread((TaskHandle_t)px_शीर्ष_Of_चळत, thread);

    /* create the thread associated with the task being created */
    स्थिरांक पूर्णांक ret = pthread_create(&thread->pthread, &thread_attr, linux_port_task_runner, thread);
    जर (ret != 0) {
        linux_port_fatal_error("pthread_create", ret);
    }
    परत px_शीर्ष_Of_चळत;
}

BaseType_t xPortStartScheduler(शून्य)
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
    पूर्णांक ret = pthread_create(&s_scheduler_thread, रिक्त, linux_port_scheduler_runner, रिक्त);
    जर (ret != 0) {
       linux_port_fatal_error("pthread_create", ret);
    }

    /* Should never return */
    pthread_join(s_scheduler_thread, रिक्त);

    परत 0;
}

शून्य vPortEndScheduler(शून्य)
{
    निकास(0);
}

शून्य vPortEnterCritical(शून्य)
{
    जर (!s_scheduler_started) {
        परत;
    }

    pthread_mutex_lock(&s_port_mutex);

    /* Non-FreeRTOS thread or recursive enter: just bump the counter.
     * The mutex is already held (recursive lock succeeds for same thread). */
    जर (!linux_port_in_freertos_task() || s_ux_critical_nesting > 0) {
        s_ux_critical_nesting++;
        परत;
    }

    /* First enter from a FreeRTOS task: ensure we're the scheduled task.
     * If not, release the mutex, block until the scheduler switches to us,
     * then re-acquire. */
    thread_t *calling_thread = linux_port_get_calling_thread();
    thread_t *scheduled_thread = linux_port_get_thread_from_handle(xTaskGetCurrentTaskHandle());

    पर्यंत (calling_thread && !calling_thread->is_dying && calling_thread != scheduled_thread) {
        pthread_mutex_unlock(&s_port_mutex);
        linux_port_block_thread(calling_thread);
        pthread_mutex_lock(&s_port_mutex);
        calling_thread = linux_port_get_calling_thread();
        scheduled_thread = linux_port_get_thread_from_handle(xTaskGetCurrentTaskHandle());
    }

    जर (!calling_thread || calling_thread->is_dying) {
        linux_port_switch_context(xTaskGetCurrentTaskHandle());
        pthread_mutex_unlock(&s_port_mutex);
        परत;
    }

    s_ux_critical_nesting = 1;
}

शून्य vPortExitCritical(शून्य)
{
    जर (!s_scheduler_started || s_ux_critical_nesting == 0) {
        परत;
    }

    s_ux_critical_nesting--;

    /* Check for deferred yield on final exit from a FreeRTOS task */
    जर (s_ux_critical_nesting == 0 && linux_port_in_freertos_task()) {
        thread_t *calling_thread = linux_port_get_calling_thread();
        जर (calling_thread && calling_thread->yield_needed) {
            calling_thread->yield_needed = false;
            pthread_mutex_unlock(&s_port_mutex);
            vPortYield();
            परत;
        }
    }

    pthread_mutex_unlock(&s_port_mutex);
}

/* Handle the case where the calling pthread is not a registered FreeRTOS task.
 * If the calling thread has been deleted but is still the scheduled task,
 * perform a context switch. Otherwise just release the mutex and return.
 * Returns true if the caller should return early. */
स्थिर bool linux_port_handle_deleted_task(TaskHandle_t scheduled_task_hdl,
                                           thread_t *calling_thread)
{
    जर (calling_thread != रिक्त) {
        परत false;
    }

    thread_t *scheduled_thread = linux_port_get_thread_from_handle(scheduled_task_hdl);
    जर (scheduled_thread == रिक्त) {
        linux_port_switch_context(scheduled_task_hdl);
    }
    pthread_mutex_unlock(&s_port_mutex);
    परत true;
}

शून्य vPortYield(शून्य)
{
    pthread_mutex_lock(&s_port_mutex);

    thread_t *calling_thread = linux_port_get_calling_thread();
    TaskHandle_t scheduled_task_hdl = xTaskGetCurrentTaskHandle();

    जर (linux_port_handle_deleted_task(scheduled_task_hdl, calling_thread)) {
        परत;
    }

    /* If in a critical section, defer the yield until the section exits. */
    जर (s_ux_critical_nesting != 0) {
        calling_thread->yield_needed = true;
        pthread_mutex_unlock(&s_port_mutex);
        परत;
    }

    /* Hand the CPU to the next ready task right now (mimics PendSV on real
     * hardware). */
    linux_port_switch_context(scheduled_task_hdl);
    pthread_mutex_unlock(&s_port_mutex);

    /* If the newly scheduled task is different from the calling thread,
     * block until the scheduler resumes this task. */
    TaskHandle_t next_task_hdl = xTaskGetCurrentTaskHandle();
    thread_t *next_thread = linux_port_get_thread_from_handle(next_task_hdl);
    जर (calling_thread != next_thread) {
        linux_port_block_thread(calling_thread);
    }
}

शून्य vPortYieldWithinApi(शून्य)
{
    vPortYield();
}

शून्य vPortSuspendScheduler(शून्य)
{
    /* scheduled out task trying to suspend the scheduler should get blocked here */
    pthread_mutex_lock(&s_port_mutex);

   /* get the metadata of the pthread calling this function */
    thread_t *calling_thread = linux_port_get_calling_thread();

    /* get the thread metadata from the scheduled task */
    TaskHandle_t scheduled_task_hdl = xTaskGetCurrentTaskHandle();

    जर (linux_port_handle_deleted_task(scheduled_task_hdl, calling_thread)) {
        परत;
    }

    thread_t *scheduled_thread = linux_port_get_thread_from_handle(scheduled_task_hdl);
    जर (calling_thread != scheduled_thread) {
        pthread_mutex_unlock(&s_port_mutex);
        vPortYield();
        परत;
    }
    pthread_mutex_unlock(&s_port_mutex);
}

शून्य vPortDisableInterrupts(शून्य)
{
    vPortEnterCritical();
}

शून्य vPortEnableInterrupts(शून्य)
{
    vPortExitCritical();
}

BaseType_t xPortSetInterruptMask(शून्य)
{
    vPortEnterCritical();
    परत pdTRUE;
}

शून्य vPortClearInterruptMask(BaseType_t xMask)
{
    vPortExitCritical();
}

शून्य vPortThreadDying(शून्य *pxTaskToDelete, अस्थिर BaseType_t *pxPendYield)
{
    pthread_mutex_lock(&s_port_mutex);

    thread_t *thread = linux_port_get_thread_from_handle((TaskHandle_t)pxTaskToDelete);
    जर (thread == रिक्त) {
        pthread_mutex_unlock(&s_port_mutex);
        परत;
    }

    /* Mark the thread as dying, cancel the thread. the pthread
     * will be stopped on next cancellation point. Do not remove the
     * thread item from the list since it will be done in vPortCancelThread */
    thread->is_dying = true;
    pthread_cancel(thread->pthread);

    pthread_mutex_unlock(&s_port_mutex);
}

#जर CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
स्थिर शून्य vPortTLSPointersDelCb(शून्य *pxTCB)
{
    StaticTask_t *tcb = (StaticTask_t *)pxTCB;
    TlsDeleteCallbackFunction_t *pvDelCbs = (TlsDeleteCallbackFunction_t *)(&tcb->pvDummy15[config_अंक_THREAD_LOCAL_STORAGE_POINTERS / 2]);

    साठी (पूर्णांक x = 0; x < (config_अंक_THREAD_LOCAL_STORAGE_POINTERS / 2); x++) {
        जर (pvDelCbs[x] != रिक्त) {
            pvDelCbs[x](x, tcb->pvDummy15[x]);
        }
    }
}
#endif /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

शून्य vPortCancelThread(शून्य *pxTaskToDelete)
{
    pthread_mutex_lock(&s_port_mutex);

#जर CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
    vPortTLSPointersDelCb(pxTaskToDelete);
#endif

    thread_t *thread = linux_port_get_thread_from_handle((TaskHandle_t)pxTaskToDelete);
    जर (!thread) {
        pthread_mutex_unlock(&s_port_mutex);
        परत;
    }

    जर (thread->is_dying) {
        /* vPortThreadDying already called */
    } अन्यथा {
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

    pthread_join(pt, रिक्त);
    event_delete(ev);
    मुक्त(thread);
}

शून्य v_Port_Set_चळत_Watchpoint(शून्य *px_चळत_Start)
{
}

#जर ( CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS )
configRUN_TIME_COUNTER_TYPE x_Port_Get_Run_Time_Counter_मूल्य( शून्य )
{
    संरचना timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);

    परत ( configRUN_TIME_COUNTER_TYPE ) ( ( ( uint64_t ) ts.tv_sec * 1000000ULL ) +
                                            ( ( uint64_t ) ts.tv_nsec / 1000ULL ) );
}
#endif /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
