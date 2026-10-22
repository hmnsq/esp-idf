/*
 * SPDX-FileCopyrightText: 2025-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#समावेश <stdio.h>
#समावेश <stdlib.h>
#समावेश <stdint.h>
#समावेश <unistd.h>
#समावेश <pthread.h>
#समावेश <sys/queue.h>
#समावेश <time.h>
#समावेश "string.h"
#समावेश "FreeRTOS.h"
#समावेश "task.h"
#समावेश "utils/wait_for_event.h"
#समावेश "esp_private/freertos_linux_coop_syscalls.h"
#समावेश "utils/linux_port_utils.h"

#घोषणा FREERTOS_SIM_TICK_PERIOD_US (1000000 / CONFIG_FREERTOS_HZ)

प्रकार काष्ठा thread {
    स्थिर अक्षर *name;
    pthread_t pthread;
    TaskFunction_t pxCode;
    व्योम *pvParams;
    bool is_dying;
    bool yield_needed;
    काष्ठा event *ev;
} thread_t;

प्रकार काष्ठा task_thread_गाँठ {
    TaskHandle_t handle;
    thread_t *thread;
    SLIST_ENTRY(task_thread_गाँठ) अग्र;
} task_thread_गाँठ_t;


अटल SLIST_HEAD(task_thread_गाँठ_ll, task_thread_गाँठ) s_task_thread_list = SLIST_HEAD_INITIALIZER(task_thread_गाँठ);
अटल pthread_mutex_t s_thread_map_mutex = PTHREAD_MUTEX_INITIALIZER;

अटल pthread_mutex_t s_port_mutex;
अटल pthread_t s_scheduler_thread;
अटल bool s_scheduler_started = false;
अटल पूर्णांक s_ux_critical_nesting = 0;

/* TLS flag: true only when inside a real FreeRTOS task pthread */
अटल __thread bool s_in_freertos_task = false;

bool linux_port_in_freertos_task(व्योम)
{
    वापस s_in_freertos_task;
}

अटल व्योम linux_port_initialize_mutexes(व्योम)
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&s_port_mutex, &attr);
    pthread_mutexattr_destroy(&attr);
}

अटल व्योम linux_port_fatal_error(स्थिर अक्षर *संदेश, पूर्णांक err)
{
    ख_लिखो(मानक_त्रुटि, "%s: %s\n", संदेश, म_त्रुटि(err));
    पात();
}

अटल व्योम linux_port_register_thread(TaskHandle_t handle, thread_t *thread)
{
    अगर (handle == शून्य) {
        वापस;
    }

    task_thread_गाँठ_t *गाँठ = दो_स्मृति(माप(task_thread_गाँठ_t));
    अगर (!गाँठ) {
        linux_port_fatal_error("Failed to allocate thread map node", -1);
    }

    गाँठ->handle = handle;
    गाँठ->thread = thread;

    pthread_mutex_lock(&s_thread_map_mutex);
    SLIST_INSERT_HEAD(&s_task_thread_list, गाँठ, अग्र);
    pthread_mutex_unlock(&s_thread_map_mutex);
}

अटल व्योम linux_port_unregister_thread(TaskHandle_t handle)
{
    अगर (handle == शून्य) {
        वापस;
    }

    pthread_mutex_lock(&s_thread_map_mutex);

    task_thread_गाँठ_t *cur_गाँठ = SLIST_FIRST(&s_task_thread_list);
    task_thread_गाँठ_t *पूर्व_गाँठ = शून्य;

    जबतक (cur_गाँठ) {
        अगर (cur_गाँठ->handle == handle) {
            अगर (पूर्व_गाँठ) {
                पूर्व_गाँठ->अग्र.sle_अग्र = SLIST_NEXT(cur_गाँठ, अग्र);
            } अन्यथा {
                SLIST_REMOVE_HEAD(&s_task_thread_list, अग्र);
            }

            मुक्त(cur_गाँठ);
            pthread_mutex_unlock(&s_thread_map_mutex);
            वापस;
        }

        पूर्व_गाँठ = cur_गाँठ;
        cur_गाँठ = SLIST_NEXT(cur_गाँठ, अग्र);
    }

    pthread_mutex_unlock(&s_thread_map_mutex);
}

अटल thread_t *linux_port_get_thread_from_handle(TaskHandle_t handle)
{
    अगर (handle == शून्य) {
        वापस शून्य;
    }

    pthread_mutex_lock(&s_thread_map_mutex);
    task_thread_गाँठ_t *गाँठ = शून्य;
    SLIST_FOREACH(गाँठ, &s_task_thread_list, अग्र) {
        अगर (गाँठ->handle == (TaskHandle_t)(*(ढेर_Type_t **)(handle))) {
            thread_t *t = गाँठ->thread;
            pthread_mutex_unlock(&s_thread_map_mutex);
            वापस t;
        }
    }
    pthread_mutex_unlock(&s_thread_map_mutex);
    वापस शून्य;
}

अटल thread_t *linux_port_get_calling_thread(व्योम)
{
    pthread_t self = pthread_self();
    pthread_mutex_lock(&s_thread_map_mutex);
    task_thread_गाँठ_t *गाँठ = शून्य;
    SLIST_FOREACH(गाँठ, &s_task_thread_list, अग्र) {
        अगर (pthread_equal(गाँठ->thread->pthread, self)) {
            thread_t *thread = गाँठ->thread;
            pthread_mutex_unlock(&s_thread_map_mutex);
            वापस thread;
        }
    }
    pthread_mutex_unlock(&s_thread_map_mutex);
    वापस शून्य;
}

pthread_t linux_port_get_scheduled_task_pthread(व्योम)
{
    thread_t *thread = linux_port_get_thread_from_handle(xTaskGetCurrentTaskHandle());
    वापस thread ? thread->pthread : pthread_self();
}

अटल व्योम *linux_port_task_runner(व्योम *arg)
{
    /* Allow this thread to be cancelled */
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, शून्य);

    /* set the flag showing that this is a freertos task */
    s_in_freertos_task = true;

    /* setup the backtrace signal. ONLY triggered before abort so
     * it will not interfere with the simulation while its running */
    linux_port_setup_backtrace_signal();

    thread_t *thread = arg;

    /* Block until scheduler signals first time, then run the task body. */
    event_wait(thread->ev);
    thread->pxCode(thread->pvParams);

    वापस शून्य;
}

अटल व्योम linux_port_unblock_thread(thread_t *thread)
{
    event_signal(thread->ev);
}

अटल व्योम linux_port_block_thread(thread_t *thread)
{
    event_wait(thread->ev);
}

अटल व्योम linux_port_increment_tick(व्योम)
{
    (व्योम)xTaskIncrementTick();
}

अटल व्योम linux_port_switch_context(TaskHandle_t current_task_hdl)
{
    pthread_mutex_lock(&s_port_mutex);

    thread_t *current_thread = linux_port_get_thread_from_handle(current_task_hdl);

    /* get the task that should be scheduled next */
    vTaskSwitchContext();

    /* get the new task to schedule and the associated thread item */
    TaskHandle_t अग्र_task_hdl = xTaskGetCurrentTaskHandle();
    thread_t *अग्र_thread = linux_port_get_thread_from_handle(अग्र_task_hdl);

    /* unblock the newly scheduled task if it is different from
     * the one already scheduled */
    अगर (अग्र_thread) {
        /* Only unblock the thread if we are actually switching to a
         * different one. Signaling the already-running thread would
         * leave a stale event_triggered flag, causing its next
         * event_wait (e.g. in vPortYield) to return immediately
         * instead of blocking.
         *
         * Exception: on the very first switch, the task is still blocked
         * in its initial event_wait (linux_port_task_runner), so we must
         * signal it even though current_thread == next_thread. */
        अगर (अग्र_thread != current_thread || !s_scheduler_started) {
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
            अगर (current_thread) {
                event_clear(current_thread->ev);
            }
            linux_port_unblock_thread(अग्र_thread);
        }
    }

    अगर (!s_scheduler_started) {
        s_scheduler_started = true;
    }

    /* fill the name of the task in the thread item if not done already. */
    अगर (अग्र_thread && अग्र_thread->name == शून्य) {
        /* fill the name of the thread now */
        अग्र_thread->name = pcTaskGetName(अग्र_task_hdl);
    }

    /* fill the name of the task in the thread item if not done already. */
    अगर (current_thread && current_thread->name == शून्य) {
        /* fill the name of the thread now */
        current_thread->name = pcTaskGetName(current_task_hdl);
    }

    pthread_mutex_unlock(&s_port_mutex);
}

अटल व्योम *linux_port_scheduler_runner(व्योम *arg)
{
    (व्योम)arg;

    जबतक (1) {
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
    वापस शून्य;
}

ढेर_Type_t *px_Port_Initialise_ढेर(ढेर_Type_t *px_शीर्ष_Of_ढेर,
                                   ढेर_Type_t *px_End_Of_ढेर,
                                   TaskFunction_t pxCode,
                                   व्योम *pvParameters)
{
    pthread_attr_t thread_attr;
    माप_प्रकार thread_ढेर_आकार;

    /* Store the thread data at the start of the stack. */
    thread_ढेर_आकार = (px_शीर्ष_Of_ढेर - px_End_Of_ढेर) * माप(*px_शीर्ष_Of_ढेर);
    pthread_attr_init(&thread_attr);
    pthread_attr_setstack(&thread_attr, px_End_Of_ढेर, thread_ढेर_आकार);

    thread_t *thread = दो_स्मृति(माप(thread_t));
    अगर (!thread) {
        linux_port_fatal_error("Failed to allocate thread metadata", -1);
    }
    thread->name = शून्य; // this will be filled later when we know about the task name
    thread->pxCode = pxCode;
    thread->pvParams = pvParameters;
    thread->is_dying = false;
    thread->yield_needed = false;
    thread->ev = event_create();

    linux_port_register_thread((TaskHandle_t)px_शीर्ष_Of_ढेर, thread);

    /* create the thread associated with the task being created */
    स्थिर पूर्णांक ret = pthread_create(&thread->pthread, &thread_attr, linux_port_task_runner, thread);
    अगर (ret != 0) {
        linux_port_fatal_error("pthread_create", ret);
    }
    वापस px_शीर्ष_Of_ढेर;
}

BaseType_t xPortStartScheduler(व्योम)
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
    पूर्णांक ret = pthread_create(&s_scheduler_thread, शून्य, linux_port_scheduler_runner, शून्य);
    अगर (ret != 0) {
       linux_port_fatal_error("pthread_create", ret);
    }

    /* Should never return */
    pthread_join(s_scheduler_thread, शून्य);

    वापस 0;
}

व्योम vPortEndScheduler(व्योम)
{
    निकास(0);
}

व्योम vPortEnterCritical(व्योम)
{
    अगर (!s_scheduler_started) {
        वापस;
    }

    pthread_mutex_lock(&s_port_mutex);

    /* Non-FreeRTOS thread or recursive enter: just bump the counter.
     * The mutex is already held (recursive lock succeeds for same thread). */
    अगर (!linux_port_in_freertos_task() || s_ux_critical_nesting > 0) {
        s_ux_critical_nesting++;
        वापस;
    }

    /* First enter from a FreeRTOS task: ensure we're the scheduled task.
     * If not, release the mutex, block until the scheduler switches to us,
     * then re-acquire. */
    thread_t *calling_thread = linux_port_get_calling_thread();
    thread_t *scheduled_thread = linux_port_get_thread_from_handle(xTaskGetCurrentTaskHandle());

    जबतक (calling_thread && !calling_thread->is_dying && calling_thread != scheduled_thread) {
        pthread_mutex_unlock(&s_port_mutex);
        linux_port_block_thread(calling_thread);
        pthread_mutex_lock(&s_port_mutex);
        calling_thread = linux_port_get_calling_thread();
        scheduled_thread = linux_port_get_thread_from_handle(xTaskGetCurrentTaskHandle());
    }

    अगर (!calling_thread || calling_thread->is_dying) {
        linux_port_switch_context(xTaskGetCurrentTaskHandle());
        pthread_mutex_unlock(&s_port_mutex);
        वापस;
    }

    s_ux_critical_nesting = 1;
}

व्योम vPortExitCritical(व्योम)
{
    अगर (!s_scheduler_started || s_ux_critical_nesting == 0) {
        वापस;
    }

    s_ux_critical_nesting--;

    /* Check for deferred yield on final exit from a FreeRTOS task */
    अगर (s_ux_critical_nesting == 0 && linux_port_in_freertos_task()) {
        thread_t *calling_thread = linux_port_get_calling_thread();
        अगर (calling_thread && calling_thread->yield_needed) {
            calling_thread->yield_needed = false;
            pthread_mutex_unlock(&s_port_mutex);
            vPortYield();
            वापस;
        }
    }

    pthread_mutex_unlock(&s_port_mutex);
}

/* Handle the case where the calling pthread is not a registered FreeRTOS task.
 * If the calling thread has been deleted but is still the scheduled task,
 * perform a context switch. Otherwise just release the mutex and return.
 * Returns true if the caller should return early. */
अटल bool linux_port_handle_deleted_task(TaskHandle_t scheduled_task_hdl,
                                           thread_t *calling_thread)
{
    अगर (calling_thread != शून्य) {
        वापस false;
    }

    thread_t *scheduled_thread = linux_port_get_thread_from_handle(scheduled_task_hdl);
    अगर (scheduled_thread == शून्य) {
        linux_port_switch_context(scheduled_task_hdl);
    }
    pthread_mutex_unlock(&s_port_mutex);
    वापस true;
}

व्योम vPortYield(व्योम)
{
    pthread_mutex_lock(&s_port_mutex);

    thread_t *calling_thread = linux_port_get_calling_thread();
    TaskHandle_t scheduled_task_hdl = xTaskGetCurrentTaskHandle();

    अगर (linux_port_handle_deleted_task(scheduled_task_hdl, calling_thread)) {
        वापस;
    }

    /* If in a critical section, defer the yield until the section exits. */
    अगर (s_ux_critical_nesting != 0) {
        calling_thread->yield_needed = true;
        pthread_mutex_unlock(&s_port_mutex);
        वापस;
    }

    /* Hand the CPU to the next ready task right now (mimics PendSV on real
     * hardware). */
    linux_port_switch_context(scheduled_task_hdl);
    pthread_mutex_unlock(&s_port_mutex);

    /* If the newly scheduled task is different from the calling thread,
     * block until the scheduler resumes this task. */
    TaskHandle_t अग्र_task_hdl = xTaskGetCurrentTaskHandle();
    thread_t *अग्र_thread = linux_port_get_thread_from_handle(अग्र_task_hdl);
    अगर (calling_thread != अग्र_thread) {
        linux_port_block_thread(calling_thread);
    }
}

व्योम vPortYieldWithinApi(व्योम)
{
    vPortYield();
}

व्योम vPortSuspendScheduler(व्योम)
{
    /* scheduled out task trying to suspend the scheduler should get blocked here */
    pthread_mutex_lock(&s_port_mutex);

   /* get the metadata of the pthread calling this function */
    thread_t *calling_thread = linux_port_get_calling_thread();

    /* get the thread metadata from the scheduled task */
    TaskHandle_t scheduled_task_hdl = xTaskGetCurrentTaskHandle();

    अगर (linux_port_handle_deleted_task(scheduled_task_hdl, calling_thread)) {
        वापस;
    }

    thread_t *scheduled_thread = linux_port_get_thread_from_handle(scheduled_task_hdl);
    अगर (calling_thread != scheduled_thread) {
        pthread_mutex_unlock(&s_port_mutex);
        vPortYield();
        वापस;
    }
    pthread_mutex_unlock(&s_port_mutex);
}

व्योम vPortDisableInterrupts(व्योम)
{
    vPortEnterCritical();
}

व्योम vPortEnableInterrupts(व्योम)
{
    vPortExitCritical();
}

BaseType_t xPortSetInterruptMask(व्योम)
{
    vPortEnterCritical();
    वापस pdTRUE;
}

व्योम vPortClearInterruptMask(BaseType_t xMask)
{
    vPortExitCritical();
}

व्योम vPortThreadDying(व्योम *pxTaskToDelete, अस्थिर BaseType_t *pxPendYield)
{
    pthread_mutex_lock(&s_port_mutex);

    thread_t *thread = linux_port_get_thread_from_handle((TaskHandle_t)pxTaskToDelete);
    अगर (thread == शून्य) {
        pthread_mutex_unlock(&s_port_mutex);
        वापस;
    }

    /* Mark the thread as dying, cancel the thread. the pthread
     * will be stopped on next cancellation point. Do not remove the
     * thread item from the list since it will be done in vPortCancelThread */
    thread->is_dying = true;
    pthread_cancel(thread->pthread);

    pthread_mutex_unlock(&s_port_mutex);
}

#अगर CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
अटल व्योम vPortTLSPointersDelCb(व्योम *pxTCB)
{
    StaticTask_t *tcb = (StaticTask_t *)pxTCB;
    TlsDeleteCallbackFunction_t *pvDelCbs = (TlsDeleteCallbackFunction_t *)(&tcb->pvDummy15[config_अंक_THREAD_LOCAL_STORAGE_POINTERS / 2]);

    क्रम (पूर्णांक x = 0; x < (config_अंक_THREAD_LOCAL_STORAGE_POINTERS / 2); x++) {
        अगर (pvDelCbs[x] != शून्य) {
            pvDelCbs[x](x, tcb->pvDummy15[x]);
        }
    }
}
#पूर्ण_अगर /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

व्योम vPortCancelThread(व्योम *pxTaskToDelete)
{
    pthread_mutex_lock(&s_port_mutex);

#अगर CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS
    vPortTLSPointersDelCb(pxTaskToDelete);
#पूर्ण_अगर

    thread_t *thread = linux_port_get_thread_from_handle((TaskHandle_t)pxTaskToDelete);
    अगर (!thread) {
        pthread_mutex_unlock(&s_port_mutex);
        वापस;
    }

    अगर (thread->is_dying) {
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

    pthread_join(pt, शून्य);
    event_delete(ev);
    मुक्त(thread);
}

व्योम v_Port_Set_ढेर_Watchpoint(व्योम *px_ढेर_Start)
{
}

#अगर ( CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS )
config_RUN_TIME_गणक_TYPE x_Port_Get_Run_Time_गणक_मूल्य( व्योम )
{
    काष्ठा timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);

    वापस ( config_RUN_TIME_गणक_TYPE ) ( ( ( uint64_t ) ts.tv_sec * 1000000ULL ) +
                                            ( ( uint64_t ) ts.tv_nsec / 1000ULL ) );
}
#पूर्ण_अगर /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
