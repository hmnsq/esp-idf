/*
 * SPDX-FileCopyrightText: 2022-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن "sdkconfig.h"
#ضمن <stdbool.h>
#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن "freertos/semphr.h"
#ضمن "driver/gptimer.h"
#ضمن "esp_rom_sys.h"
#ضمن "unity.h"
#ضمن "test_utils.h"

/*
Scheduler suspension behavior differs significantly in SMP FreeRTOS, thus none of these tests apply to SMP FreeRTOS
*/
#إذا !CONFIG_FREERTOS_SMP

/*
GP timer is used to trigger an interrupt. Test cases will register an interrupt callback called from the timer's
interrupt callback. The functions below simply the interrupt registration/trigger/deregistration process.
*/

#إذا SOC_GPTIMER_SUPPORTED

ساكن gptimer_handle_t gptimer = عدم;
ساكن bool (*registered_intr_callback)(فراغ *) = عدم;

ساكن bool on_timer_alarm_cb(gptimer_handle_t timer, ثابت gptimer_alarm_event_معلومات_t *edata, فراغ *user_ctx)
{
    bool yield;
    إذا (registered_intr_callback) {
        yield = registered_intr_callback(user_ctx);
    } وإلا {
        yield = false;
    }
    أرجع yield;
}

ساكن فراغ ثبتوونکی_intr_cb(bool (*callback)(فراغ *), فراغ *callback_arg)
{
    gptimer_handle_t gptimer_لنډمهاله;

    // Initialize a GP timer used to trigger an interrupt
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_شمېر_پورته,
        .resolution_hz = 1000000, // 1MHz, 1 tick=1us
    };
    TEST_ESP_OK(gptimer_new_timer(&timer_config, &gptimer_لنډمهاله));

    // Configure an alarm (of 1ms) and callback for the timer
    gptimer_alarm_config_t alarm_config = {
        .reload_شمېر = 0,
        .alarm_شمېر = 1000, // alarm period 1ms
        .flags.auto_reload_on_alarm = true,
    };
    gptimer_event_callbacks_t cbs = {
        .on_alarm = on_timer_alarm_cb,
    };
    TEST_ESP_OK(gptimer_ثبتوونکی_event_callbacks(gptimer_لنډمهاله, &cbs, callback_arg));
    TEST_ESP_OK(gptimer_enable(gptimer_لنډمهاله));
    TEST_ESP_OK(gptimer_سېټ_alarm_action(gptimer_لنډمهاله, &alarm_config));

    gptimer = gptimer_لنډمهاله;
    registered_intr_callback = callback;
}

ساكن فراغ trigger_intr_cb(فراغ)
{
    // Interrupt should be triggered in 1ms
    TEST_ESP_OK(gptimer_پیل(gptimer));
}

ساكن فراغ deregister_intr_cb(فراغ)
{
    gptimer_handle_t gptimer_لنډمهاله = gptimer;
    gptimer = عدم;
    registered_intr_callback = عدم;

    TEST_ESP_OK(gptimer_stop(gptimer_لنډمهاله));
    TEST_ESP_OK(gptimer_disable(gptimer_لنډمهاله));
    TEST_ESP_OK(gptimer_del_timer(gptimer_لنډمهاله));
}

#د_که_پای //SOC_GPTIMER_SUPPORTED

/* ---------------------------------------------------------------------------------------------------------------------
Test vTaskSuspendAll() and xTaskResumeAll() basic

Purpose:
    - Test that vTaskSuspendAll() will suspends the scheduler for the calling core
    - Test that xTaskResumeAll() will resumes scheduling for the calling core

Procedure:
    - Call vTaskSuspendAll() to suspend the scheduler
    - Call xTaskResumeAll() to resume the scheduler

Expected:
    - xTaskGetSchedulerState() should return the correct state
--------------------------------------------------------------------------------------------------------------------- */

TEST_CASE("Test vTaskSuspendAll and xTaskResumeAll basic", "[freertos]")
{
    // Check scheduler is running on the current core
    TEST_ASSERT_EQUAL(taskSCHEDULER_RUNNING, xTaskGetSchedulerState());
    vTaskSuspendAll();
    TEST_ASSERT_EQUAL(taskSCHEDULER_SUSPENDED, xTaskGetSchedulerState());
    xTaskResumeAll();
    TEST_ASSERT_EQUAL(taskSCHEDULER_RUNNING, xTaskGetSchedulerState());
}

/* ---------------------------------------------------------------------------------------------------------------------
Test vTaskSuspendAll() and xTaskResumeAll() multicore

Only runs on !CONFIG_FREERTOS_UNICORE

Purpose:
    - Test that vTaskSuspendAll() will only suspends scheduling only for the calling core
    - Test that xTaskResumeAll() will only resume scheduling for the calling core

Procedure:
    Each core gets tested in the role of core A
    - Create a taskA pinned to one core (e.g., core A) that will disable the scheduler
    - Created a "taskB" to another core (e.g., core B) that will not disable the scheduler
    - taskA calls vTaskSuspendAll() to suspend the scheduler on core A
    - taskA calls xTaskResumeAll() to resume the scheduler on core A

Expected:
    - vTaskSuspendAll() should only disable the scheduler for the suspended core A
    - xTaskResumeAll() should resume the scheduler for the suspended core A
    - Scheduler on core B should remain enabled
--------------------------------------------------------------------------------------------------------------------- */

#إذا !CONFIG_FREERTOS_UNICORE
#تعریف_کړه TEST_BASIC_BUSY_DELAY_US    10000

ساكن متطاير صحيح taskA_sync;
ساكن متطاير صحيح taskB_sync;

ساكن فراغ test_multicore_taskA(فراغ *arg)
{
    // Wait to be started
    ulTaskNotifyTake(pdTRUE, پورټ_MAX_DELAY);

    // Check scheduler on core A is enabled
    TEST_ASSERT_EQUAL(taskSCHEDULER_RUNNING, xTaskGetSchedulerState());
    taskA_sync++;
    طالما (taskB_sync != 1) {
        ;   // Wait for task B to complete its check
    }

    // Suspend the scheduler on core A
    vTaskSuspendAll();
    // Check scheduler is suspended on core A
    TEST_ASSERT_EQUAL(taskSCHEDULER_SUSPENDED, xTaskGetSchedulerState());
    taskA_sync++;
    طالما (taskB_sync != 2) {
        ;   // Wait for task B to complete its check
    }

    // Busy spin for a while to simulate work done while scheduler is suspended
    esp_rom_delay_us(TEST_BASIC_BUSY_DELAY_US);

    // Check scheduler is still suspended on core A
    TEST_ASSERT_EQUAL(taskSCHEDULER_SUSPENDED, xTaskGetSchedulerState());
    taskA_sync++;
    طالما (taskB_sync != 3) {
        ;   // Wait for task B to complete its check
    }

    // Resume the scheduler on core A
    xTaskResumeAll();
    // Check that scheduler has resumed resumed on core A
    TEST_ASSERT_EQUAL(taskSCHEDULER_RUNNING, xTaskGetSchedulerState());
    taskA_sync++;
    طالما (taskB_sync != 4) {
        ;   // Wait for task B to complete its check
    }

    // Indicate done and wait to be deleted
    SemaphoreHandle_t بشپړ_sem = (SemaphoreHandle_t)arg;
    xSemaphoreGive(بشپړ_sem);
    vTaskSuspend(عدم);
}

ساكن فراغ test_multicore_taskB(فراغ *arg)
{
    // Wait to be started
    ulTaskNotifyTake(pdTRUE, پورټ_MAX_DELAY);

    لكل (صحيح i = 1; i <= 4; i++) {
        // Wait for suspended trigger from task A
        طالما (taskA_sync != i) {
            ;
        }
        // Check that scheduler is still running on core B
        TEST_ASSERT_EQUAL(taskSCHEDULER_RUNNING, xTaskGetSchedulerState());
        taskB_sync++;
    }

    // Indicate done and wait to be deleted
    SemaphoreHandle_t بشپړ_sem = (SemaphoreHandle_t)arg;
    xSemaphoreGive(بشپړ_sem);
    vTaskSuspend(عدم);
}

TEST_CASE("Test vTaskSuspendAll() and xTaskResumeAll() multicore", "[freertos]")
{
    SemaphoreHandle_t بشپړ_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_شمېره_د_CORES, 0);
    TEST_ASSERT_NOT_EQUAL(عدم, بشپړ_sem);

    لكل (صحيح i = 0; i < CONFIG_FREERTOS_شمېره_د_CORES; i++) {
        // Create tasks on core A and core B
        TaskHandle_t taskA_hdl;
        TaskHandle_t taskB_hdl;
        TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_منزل_Core(test_multicore_taskA, "taskA", 2048, (فراغ *)بشپړ_sem, UNITY_FREERTOS_لومړیتوب - 1, &taskA_hdl, i));
        TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_منزل_Core(test_multicore_taskB, "taskB", 2048, (فراغ *)بشپړ_sem, UNITY_FREERTOS_لومړیتوب - 1, &taskB_hdl, !i));
        // Start the tasks and wait for their completion
        taskA_sync = 0;
        taskB_sync = 0;
        xTaskNotifyGive(taskA_hdl);
        xTaskNotifyGive(taskB_hdl);
        لكل (صحيح j = 0; j < 2; j++) {
            xSemaphoreTake(بشپړ_sem, پورټ_MAX_DELAY);
        }

        // Suspend the test tasks in case they haven't suspended themselves yet
        vTaskSuspend(taskA_hdl);
        vTaskSuspend(taskB_hdl);
        vTaskDelay(10);

        // Cleanup the tasks
        vTaskDelete(taskA_hdl);
        vTaskDelete(taskB_hdl);
    }

    vSemaphoreDelete(بشپړ_sem);
    // Add a short delay to allow the idle task to free any remaining task memory
    vTaskDelay(10);
}
#د_که_پای  // !CONFIG_FREERTOS_UNICORE

/* ---------------------------------------------------------------------------------------------------------------------
Test vTaskSuspendAll allows scheduling on other cores

Only runs on !CONFIG_FREERTOS_UNICORE

Purpose:
    - Test that disabling a scheduler on one core (e.g., core B) does not disable scheduling on the other core (e.g., core A)
    - While the scheduler on core B is disabled, test that...
        - A task on Core A can be unblocked by another task also on core A
        - A task on Core A can be unblocked by an interrupt on core A

Procedure:
    Each core gets tested in the role of core A
        - Create task B1 pinned to core B that will suspend scheduling on core B
        - Create task A2 pinned to core A that will test unblocking on core A
        - Create task A1 pinned to core A that will unblock task A2
        - Register an interrupt on core A that will unblock task A2
        - Have A2 block
        - Have B1 disable scheduling on core B. A1 checks that scheduling is still enabled on core A
        - Have A1 unblock A2
        - Have the core A ISR unblock A2
        - Cleanup the tasks

Expected:
    When B1 disables scheduling on core B...
        - Scheduling on core A should still be enabled
        - A2 should be unblocked by A1 and run without issue
        - A2 should be unblocked by core A ISR and run without issue
--------------------------------------------------------------------------------------------------------------------- */

#إذا !CONFIG_FREERTOS_UNICORE

#إذا SOC_GPTIMER_SUPPORTED

ساكن متطاير صحيح test_unblk_sync;
ساكن SemaphoreHandle_t test_unblk_بشپړ_sem;

ساكن bool test_unblk_coreA_isr(فراغ *arg)
{
    TaskHandle_t a2_task_hdl = (TaskHandle_t)arg;
    بنسټ_Type_t task_woken = pdFALSE;
    // Unblock task b2
    vTaskNotifyGiveFromISR(a2_task_hdl, &task_woken);
    أرجع (task_woken == pdTRUE);
}

ساكن فراغ test_unblk_a2_task(فراغ *arg)
{
    متطاير صحيح *a2_task_پرلپسې_شمېر = (متطاير صحيح *)arg;
    // Wait to be unblocked by A1
    ulTaskNotifyTake(pdTRUE, پورټ_MAX_DELAY);
    (*a2_task_پرلپسې_شمېر)++;
    // Wait to be unblocked by Core A ISR
    ulTaskNotifyTake(pdTRUE, پورټ_MAX_DELAY);
    (*a2_task_پرلپسې_شمېر)++;
    // Wait to be deleted
    vTaskSuspend(عدم);
}

ساكن فراغ test_unblk_a1_task(فراغ *arg)
{
    متطاير صحيح a2_task_پرلپسې_شمېر = 0;

    // Create task A2 to block on this core (i.e., core A)
    TaskHandle_t a2_task_hdl;
    TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_منزل_Core(test_unblk_a2_task, "A2", 8192, (فراغ *)&a2_task_پرلپسې_شمېر, UNITY_FREERTOS_لومړیتوب + 2, &a2_task_hdl, x_پورټ_Get_Core_ID()));
    // Install an interrupt on core A
    ثبتوونکی_intr_cb(test_unblk_coreA_isr, (فراغ *)a2_task_hdl);

    // Wait to be started by the main task
    ulTaskNotifyTake(pdTRUE, پورټ_MAX_DELAY);

    // Start B1
    xTaskNotifyGive((TaskHandle_t)arg);
    طالما (test_unblk_sync != 1) {
        ; // Wait for confirmation from B1 that scheduler has been suspended on Core B
    }
    // Verify that the scheduler is still enabled on core A
    TEST_ASSERT_EQUAL(taskSCHEDULER_RUNNING, xTaskGetSchedulerState());

    // Unblock A2, it should preempt immediately due to its higher priority
    xTaskNotifyGive(a2_task_hdl);
    // Verify that task A2 has run
    TEST_ASSERT_EQUAL(1, a2_task_پرلپسې_شمېر);

    // Trigger an ISR on this core A to unblock task A2. A2 should preempt immediately due to its higher priority
    trigger_intr_cb();
    esp_rom_delay_us(2000); // Short busy delay to ensure interrupt has triggered
    // Verify that task A2 has run
    TEST_ASSERT_EQUAL(2, a2_task_پرلپسې_شمېر);

    // Trigger B1 to resume scheduling on core B
    test_unblk_sync = 2;
    طالما (test_unblk_sync != 3) {
        ; // Wait for confirmation from B1 that scheduler has been resumed
    }
    // Verify that the scheduler is still enabled on core A
    TEST_ASSERT_EQUAL(taskSCHEDULER_RUNNING, xTaskGetSchedulerState());

    // Cleanup A2 and interrupt
    deregister_intr_cb();
    vTaskSuspend(a2_task_hdl);
    vTaskDelay(10);
    vTaskDelete(a2_task_hdl);

    // Indicate done and wait to be deleted
    xSemaphoreGive(test_unblk_بشپړ_sem);
    vTaskSuspend(عدم);
}

ساكن فراغ test_unblk_b1_task(فراغ *arg)
{
    // Wait to be started by A1
    ulTaskNotifyTake(pdTRUE, پورټ_MAX_DELAY);

    // Check scheduler is running on core B
    TEST_ASSERT_EQUAL(taskSCHEDULER_RUNNING, xTaskGetSchedulerState());
    // Suspend the scheduler on core B
    vTaskSuspendAll();
    TEST_ASSERT_EQUAL(taskSCHEDULER_SUSPENDED, xTaskGetSchedulerState());

    // Indicate to A1 that core B scheduler has been suspended
    test_unblk_sync = 1;

    طالما (test_unblk_sync != 2) {
        ;   // Wait for trigger from A1
    }
    // Resume the scheduler on core B
    xTaskResumeAll();
    TEST_ASSERT_EQUAL(taskSCHEDULER_RUNNING, xTaskGetSchedulerState());
    // Indicate to A1 that core B scheduler has been resumed
    test_unblk_sync = 3;

    // Indicate done and wait to be deleted
    xSemaphoreGive(test_unblk_بشپړ_sem);
    vTaskSuspend(عدم);
}

TEST_CASE("Test vTaskSuspendAll allows scheduling on other cores", "[freertos]")
{
    test_unblk_بشپړ_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_شمېره_د_CORES, 0);
    TEST_ASSERT_NOT_EQUAL(عدم, test_unblk_بشپړ_sem);

    لكل (صحيح i = 0; i < CONFIG_FREERTOS_شمېره_د_CORES; i++) {
        test_unblk_sync = 0;
        // Create a tasks
        TaskHandle_t a1_task_hdl;
        TaskHandle_t b1_task_hdl;
        TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_منزل_Core(test_unblk_b1_task, "B1", 8192, عدم, UNITY_FREERTOS_لومړیتوب + 1, &b1_task_hdl, !i));
        TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_منزل_Core(test_unblk_a1_task, "A1", 8192, (فراغ *)b1_task_hdl, UNITY_FREERTOS_لومړیتوب + 1, &a1_task_hdl, i));
        // Start A1 to and wait for both tasks to complete
        xTaskNotifyGive(a1_task_hdl);
        لكل (صحيح j = 0; j < 2; j++) {
            xSemaphoreTake(test_unblk_بشپړ_sem, پورټ_MAX_DELAY);
        }
        // Suspend the test tasks in case they haven't suspended themselves yet
        vTaskSuspend(a1_task_hdl);
        vTaskSuspend(b1_task_hdl);
        vTaskDelay(10);

        // Cleanup tasks
        vTaskDelete(a1_task_hdl);
        vTaskDelete(b1_task_hdl);
        vTaskDelay(10);
    }

    vSemaphoreDelete(test_unblk_بشپړ_sem);
    // Add a short delay to allow the idle task to free any remaining task memory
    vTaskDelay(10);
}

#د_که_پای //SOC_GPTIMER_SUPPORTED

/* ---------------------------------------------------------------------------------------------------------------------
Test vTaskSuspendAll doesn't block unpinned tasks from being scheduled on other cores

Only runs on !CONFIG_FREERTOS_UNICORE

Purpose:
    - Test that disabling a scheduler on one core (e.g., core B) does not block unpinned tasks running on core B to be scheduled on core A

Procedure:
        - Suspend the scheduler on core A so that the unpinned task spawns on core B
        - Create a pinned task pinned to core B
        - Pinned task is blocked on start
        - Unpinned task verifies that it spawns on core B and then blocks
        - Resume the scheduler on core A
        - Unblock the pinned task
        - The pinned task suspends the scheduler on core B
        - The pinned task unblocks the unpinned task
        - Verify that the unpinned task can move to core A which has the scheduler running
        - Resume scheduler on core B
        - Cleanup the tasks

Expected:
    When the pinned task disables scheduling on core B...
        - The unpinned task, which was originally running on core B, moves to core A once it is unblocked
--------------------------------------------------------------------------------------------------------------------- */
ساكن SemaphoreHandle_t test_unpinned_sem;
متطاير uint8_t unpinned_task_running = 0;

ساكن فراغ test_unpinned_task(فراغ* arg)
{
    بنسټ_Type_t *core_A = (بنسټ_Type_t *)arg;

    // Verify that the task is running on core B
    TEST_ASSERT_EQUAL(!(*core_A), x_پورټ_Get_Core_ID());

    // Wait to be unblocked by the pinned task
    ulTaskNotifyTake(pdTRUE, پورټ_MAX_DELAY);

    // Verify that the task is running on core A once it is unblocked
    TEST_ASSERT_EQUAL((*core_A), x_پورټ_Get_Core_ID());
    unpinned_task_running = 1;

    // Mark test completion
    xSemaphoreGive(test_unpinned_sem);

    // Cleanup
    vTaskSuspend(عدم);
}

ساكن فراغ test_pinned_task(فراغ* arg)
{
    TaskHandle_t unpinned_task_hdl = (TaskHandle_t)arg;

    // Wait to be notified by the main task
    ulTaskNotifyTake(pdTRUE, پورټ_MAX_DELAY);

    // Check scheduler is running on core B
    TEST_ASSERT_EQUAL(taskSCHEDULER_RUNNING, xTaskGetSchedulerState());

    // Suspend the scheduler on core B
    vTaskSuspendAll();

    // Check scheduler is suspended on core B
    TEST_ASSERT_EQUAL(taskSCHEDULER_SUSPENDED, xTaskGetSchedulerState());

    // Unblock the unpinned task
    // It should run on core A which is not in suspended state
    // We use the FromISR() call to create an ISR scenario and to force the unblocked task to be placed in the ready list
    بنسټ_Type_t yield = 0;
    vTaskNotifyGiveFromISR(unpinned_task_hdl, &yield);

    // Busy wait to ensure that the unpinned task is scheduled
    طالما (unpinned_task_running == 0)
        ;

    // Resume scheduler on core B
    xTaskResumeAll();

    // Mark test completion
    xSemaphoreGive(test_unpinned_sem);

    // Cleanup
    vTaskSuspend(عدم);
}

TEST_CASE("Test vTaskSuspendAll doesn't block unpinned tasks from being scheduled on other cores", "[freertos]")
{
    TaskHandle_t pinned_task_hdl;
    TaskHandle_t unpinned_task_hdl;
    بنسټ_Type_t core_A = x_پورټ_Get_Core_ID();
    test_unpinned_sem = xSemaphoreCreateCounting(2, 0);

    // Suspend the scheduler on core A to avoid the unpinned task from being spawned on core A
    vTaskSuspendAll();

    // Create unpinned task. This should spin up on core B as we have suspended the scheduler on core A
    TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_منزل_Core(test_unpinned_task, "unpinned_task", 8192, &core_A, UNITY_FREERTOS_لومړیتوب + 1, &unpinned_task_hdl, tskNO_AFFINITY));

    // Create task pinned to core B
    TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_منزل_Core(test_pinned_task, "pinned_task", 8192, (فراغ *)unpinned_task_hdl, UNITY_FREERTOS_لومړیتوب + 1, &pinned_task_hdl, !core_A));

    // Resume the scheduler on core A
    xTaskResumeAll();

    // Unblock the pinned task
    xTaskNotifyGive(pinned_task_hdl);

    // Wait for test completion
    لكل (صحيح i = 0; i < 2; i++) {
        xSemaphoreTake(test_unpinned_sem, پورټ_MAX_DELAY);
    }
    // Suspend the test tasks in case they haven't suspended themselves yet
    vTaskSuspend(pinned_task_hdl);
    vTaskSuspend(unpinned_task_hdl);
    vTaskDelay(10);

    // Cleanup
    vTaskDelete(pinned_task_hdl);
    vTaskDelete(unpinned_task_hdl);
    vSemaphoreDelete(test_unpinned_sem);
}
#د_که_پای  // !CONFIG_FREERTOS_UNICORE

/* ---------------------------------------------------------------------------------------------------------------------
Test xTaskResumeAll() resumes pended tasks on the current core

Purpose:
    - When the scheduler is suspended on a particular core, test that tasks unblocked by an ISR on that core will place
      those tasks on the core's pending ready list if the tasks are pinned to the core with the suspended scheduler.
      Tasks which have affinity to the other core must be resumed.
    - When the scheduler is resumed on a particular core, test that the tasks on core's pending ready list will be
      scheduled.

Procedure:
    Test for each core
        - Create some blocking tasks on the same core
        - Register an interrupt on the same core to unblock those tasks
        - Suspend the scheduler on the core
        - Trigger the interrupt to unblock those tasks
        - Resume the scheduler
        - Cleanup

Expected:
    - When the ISR unblocks the blocked tasks, the task's state should be ready
    - When the scheduler is resumed, the tasks should be scheduled and run without issue.
--------------------------------------------------------------------------------------------------------------------- */

#تعریف_کړه TEST_PENDED_عدد_BLOCKED_TASKS   4

#إذا SOC_GPTIMER_SUPPORTED

ساكن bool test_pended_isr(فراغ *arg)
{
    TaskHandle_t *blkd_tsks = (TaskHandle_t *)arg;
    بنسټ_Type_t task_woken = pdFALSE;
    // Unblock the blocked tasks
    لكل (صحيح i = 0; i < TEST_PENDED_عدد_BLOCKED_TASKS; i++) {
        vTaskNotifyGiveFromISR(blkd_tsks[i], &task_woken);
    }

    أرجع (task_woken == pdTRUE);
}

ساكن فراغ test_pended_blkd_task(فراغ *arg)
{
    متطاير bool *has_پرلپسې = (bool *)arg;
    // Wait to be unblocked
    ulTaskNotifyTake(pdTRUE, پورټ_MAX_DELAY);
    // Indicate the task been unblocked and has run
    *has_پرلپسې = true;
    // Wait to be deleted
    vTaskSuspend(عدم);
}

ساكن فراغ test_pended_running_task(فراغ *arg)
{
    TaskHandle_t main_task_hdl = (TaskHandle_t)arg;
    TaskHandle_t blkd_tsks[TEST_PENDED_عدد_BLOCKED_TASKS];
    متطاير bool has_پرلپسې[TEST_PENDED_عدد_BLOCKED_TASKS];

    // Created blocked tasks pinned to each core
    لكل (صحيح i = 0; i < TEST_PENDED_عدد_BLOCKED_TASKS; i++) {
        has_پرلپسې[i] = false;
        TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_منزل_Core(test_pended_blkd_task, "blkd", 4096, (فراغ *)&has_پرلپسې[i], UNITY_FREERTOS_لومړیتوب + 2, &blkd_tsks[i], i % CONFIG_FREERTOS_شمېره_د_CORES));
    }
    vTaskDelay(10);

    // Install an interrupt on the current core core
    ثبتوونکی_intr_cb(test_pended_isr, (فراغ *)blkd_tsks);

    // Checked that all tasks are blocked and have no run yet
    لكل (صحيح i = 0; i < TEST_PENDED_عدد_BLOCKED_TASKS; i++) {
        TEST_ASSERT_EQUAL(eBlocked, eTaskGetState(blkd_tsks[i])); // Should be eSuspended due to portMAX_DELAY
        TEST_ASSERT_EQUAL(false, has_پرلپسې[i]);
    }

    // Suspend the scheduler on the current core
    vTaskSuspendAll();

    // Trigger the interrupt to unblocked the blocked tasks
    trigger_intr_cb();
    esp_rom_delay_us(2000); // Short busy delay to ensure interrupt has triggered

    // Check that tasks which have affinity to the current core are blocked and have not run as the scheduler is suspended.
    // While tasks which do not have affinity to the current core are unblocked.
    لكل (صحيح i = 0; i < TEST_PENDED_عدد_BLOCKED_TASKS; i++) {
        // Note: We use eBlocked instead of eReady due to a bug in eTaskGetState(). See (IDF-5543)
        إذا (xTaskGetCoreID(blkd_tsks[i]) == x_پورټ_Get_Core_ID()) {
            TEST_ASSERT_EQUAL(eBlocked, eTaskGetState(blkd_tsks[i]));
            TEST_ASSERT_EQUAL(false, has_پرلپسې[i]);
        } وإلا {
            TEST_ASSERT_NOT_EQUAL(eBlocked, eTaskGetState(blkd_tsks[i]));
        }
    }

    // Resume the scheduler on the current core to schedule the unblocked tasks
    xTaskResumeAll();
    esp_rom_delay_us(10000); // Busy delay to ensure each task has enough time to run

    // Check that all tasks have run
    لكل (صحيح i = 0; i < TEST_PENDED_عدد_BLOCKED_TASKS; i++) {
        TEST_ASSERT_EQUAL(true, has_پرلپسې[i]);
    }

    // Clean up the interrupt and tasks
    deregister_intr_cb();
    لكل (صحيح i = 0; i < TEST_PENDED_عدد_BLOCKED_TASKS; i++) {
        vTaskDelete(blkd_tsks[i]);
    }

    // Notify completion and wait for deletion
    xTaskNotifyGive(main_task_hdl);
    vTaskSuspend(عدم);
}

TEST_CASE("Test xTaskResumeAll resumes pended tasks", "[freertos]")
{
    // Run the test on each core
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_شمېره_د_CORES; i++) {
        TaskHandle_t susp_tsk_hdl;
        TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_منزل_Core(test_pended_running_task, "susp", 2048, (فراغ *)xTaskGetCurrentTaskHandle(), UNITY_FREERTOS_لومړیتوب + 1, &susp_tsk_hdl, i));
        // Wait for to be notified to test completion
        ulTaskNotifyTake(pdTRUE, پورټ_MAX_DELAY);
        // Suspend the test task in case it hasn't suspended itself yet
        vTaskSuspend(susp_tsk_hdl);
        vTaskDelay(10);
        vTaskDelete(susp_tsk_hdl);
    }
    // Add a short delay to allow the idle task to free any remaining task memory
    vTaskDelay(10);
}

#د_که_پای //SOC_GPTIMER_SUPPORTED

/* ---------------------------------------------------------------------------------------------------------------------
Test xTaskSuspendAll on both cores pends all tasks and xTaskResumeAll on both cores resumes all tasks

Purpose:
    - When the scheduler is suspended on both cores, test that tasks unblocked by an ISR on a core would place the
      those tasks on the core's pending ready list.
    - When the scheduler is resumed on both cores, test that each core will schedule the
      tasks from their respective pending ready lists.

Procedure:
        - Create some blocking tasks pinned on both cores
        - Create a task which suspends the scheduler on the other core
        - Suspend the scheduler respectively on both cores
        - Unblock pinned tasks on both cores once the scheduler is suspended
        - Test that unblocked tasks are not scheduled
        - Resume the scheduler respectively on both cores
        - Test that unblocked tasks are now scheduled
        - Cleanup

Expected:
    - When the ISR unblocks the blocked tasks, the task's state should be blocked
    - When the scheduler is resumed, the tasks should be scheduled and run without issue
--------------------------------------------------------------------------------------------------------------------- */

#إذا !CONFIG_FREERTOS_UNICORE
TaskHandle_t blkd_tasks[TEST_PENDED_عدد_BLOCKED_TASKS];
SemaphoreHandle_t بشپړ_sem;

ساكن فراغ test_susp_task(فراغ *arg)
{
    bool *has_پرلپسې = (bool *)arg;

    // Suspend the scheduler on this core
    vTaskSuspendAll();

    لكل (صحيح i = 0; i < TEST_PENDED_عدد_BLOCKED_TASKS; i++) {
        إذا ((i % CONFIG_FREERTOS_شمېره_د_CORES) == x_پورټ_Get_Core_ID()) {
            // Unblock the blocked tasks pinned to this core.
            // We use the FromISR() call to create an ISR scenario and to force the unblocked task to be placed
            // on the pending ready list
            بنسټ_Type_t yield = pdFALSE;
            vTaskNotifyGiveFromISR(blkd_tasks[i], &yield);

            // The unblocked task must still be blocked and must not have run
            TEST_ASSERT_EQUAL(eBlocked, eTaskGetState(blkd_tasks[i]));
            TEST_ASSERT_EQUAL(false, has_پرلپسې[i]);
        }
    }

    // Resume the scheduler on this core
    xTaskResumeAll();

    // Signal test completion
    xSemaphoreGive(بشپړ_sem);

    // Wait for cleanup
    vTaskSuspend(عدم);
}

#إذا SOC_GPTIMER_SUPPORTED
TEST_CASE("Test xTaskSuspendAll on all cores pends all tasks and xTaskResumeAll on all cores resumes all tasks", "[freertos]")
{
    متطاير bool has_پرلپسې[TEST_PENDED_عدد_BLOCKED_TASKS];
    بشپړ_sem = x_Semaphore_Create_دوګونی();

    // Creat blocked tasks pinned to each core
    لكل (صحيح i = 0; i < TEST_PENDED_عدد_BLOCKED_TASKS; i++) {
        has_پرلپسې[i] = false;
        TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_منزل_Core(test_pended_blkd_task, "blkd", 4096, (فراغ *)&has_پرلپسې[i], UNITY_FREERTOS_لومړیتوب + 2, &blkd_tasks[i], i % CONFIG_FREERTOS_شمېره_د_CORES));
    }
    vTaskDelay(10);

    // Create pinned task on the other core which will suspend its scheduler
    TaskHandle_t susp_task;
    TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_منزل_Core(test_susp_task, "susp_task", 2048, (فراغ *)has_پرلپسې, UNITY_FREERTOS_لومړیتوب, &susp_task, !x_پورټ_Get_Core_ID()));

    // Suspend the scheduler on this core
    vTaskSuspendAll();

    لكل (صحيح i = 0; i < TEST_PENDED_عدد_BLOCKED_TASKS; i++) {
        إذا ((i % CONFIG_FREERTOS_شمېره_د_CORES) == x_پورټ_Get_Core_ID()) {
            // Unblock the blocked tasks pinned to this core.
            // We use the FromISR() call to create an ISR scenario and to force the unblocked task to be placed
            // on the pending ready list
            بنسټ_Type_t yield = pdFALSE;
            vTaskNotifyGiveFromISR(blkd_tasks[i], &yield);

            // The unblocked task must still be blocked and must not have run
            TEST_ASSERT_EQUAL(eBlocked, eTaskGetState(blkd_tasks[i]));
            TEST_ASSERT_EQUAL(false, has_پرلپسې[i]);
        }
    }

    // Resume scheduler on this core
    xTaskResumeAll();

    // Wait for test completion
    xSemaphoreTake(بشپړ_sem, پورټ_MAX_DELAY);

    // Verify that all blocked tasks have resumed and run when the schdulers are resumed on both cores
    لكل (صحيح i = 0; i < TEST_PENDED_عدد_BLOCKED_TASKS; i++) {
        TEST_ASSERT_NOT_EQUAL(eBlocked, eTaskGetState(blkd_tasks[i]));
        TEST_ASSERT_EQUAL(true, has_پرلپسې[i]);
    }

    // Cleanup
    لكل (صحيح i = 0; i < TEST_PENDED_عدد_BLOCKED_TASKS; i++) {
        vTaskSuspend(blkd_tasks[i]);
        vTaskDelay(10);
        vTaskDelete(blkd_tasks[i]);
    }
    vTaskSuspend(susp_task);
    vTaskDelay(10);
    vTaskDelete(susp_task);
    vSemaphoreDelete(بشپړ_sem);
}
#د_که_پای //SOC_GPTIMER_SUPPORTED
#د_که_پای  // !CONFIG_FREERTOS_UNICORE

/* ---------------------------------------------------------------------------------------------------------------------
Test vTaskSuspendAll pinned task scheduling

Purpose:
    - Test that when we disable the scheduler on core X, core X does not schedule any unblocked tasks pinned to it until
      scheduling is resumed.
    - While the scheduler on a core X is suspended, test that...
        - A task pinned to core X is not scheduled even if its unblock time has been met
        - The task is scheduled as soon as the scheduler on the core is resumed

Procedure:
    Each core gets tested in the role of core X
        - Create task A1 pinned to core X that will suspend scheduling on core X
        - Create task A2 pinned to core X that will test unblocking on core X
        - Put task A2 in blocked state with a finite delay
        - Suspend the scheduler on core X from task A1
        - Make sure that the delay time on task A2 expires
        - Resume scheduler on core X from task A1
        - Cleanup the tasks

Expected:
        - When A1 disables scheduling, A2 should not be scheduled even after expiry of its delay time
        - When A1 resumes scheduling, A2 should be scheduled
--------------------------------------------------------------------------------------------------------------------- */

#تعریف_کړه TEST_BLOCKED_TASK_DELAY_MS   100

متطاير ساكن bool has_پرلپسې = false;
SemaphoreHandle_t بشپړ_sem1;

فراغ test_blocked_task(فراغ *arg)
{
    // Wait to be started
    ulTaskNotifyTake(pdTRUE, پورټ_MAX_DELAY);

    // Mark has_run as false to begin the test
    has_پرلپسې = false;

    // Got to blocked state
    vTaskDelay(TEST_BLOCKED_TASK_DELAY_MS / پورټ_TICK_PERIOD_MS);

    // Mark when this task runs
    has_پرلپسې = true;

    // Cleanup
    vTaskDelete(عدم);
}

فراغ test_suspend_task(فراغ *arg)
{
    TaskHandle_t blkd_task = (TaskHandle_t)arg;

    // Wait to be started
    ulTaskNotifyTake(pdTRUE, پورټ_MAX_DELAY);

    // Start the task which would block
    xTaskNotifyGive(blkd_task);

    // Verify the state of the blocked task is eBlocked
    TEST_ASSERT_EQUAL(eBlocked, eTaskGetState(blkd_task));

    // Suspend the scheduler on this core
    vTaskSuspendAll();

    // Busy spin for a time which ensures that the blocked task's delay expires
    esp_rom_delay_us(TEST_BLOCKED_TASK_DELAY_MS * 1000 * 2);

    // Verify that the blocked task has not been scheduled
    TEST_ASSERT_EQUAL(false, has_پرلپسې);

    // Resume the scheduler
    xTaskResumeAll();

    // Let the blocked task to be scheduled
    vTaskDelay(10);

    // Verify that state of the blocked task is not eBlocked
    TEST_ASSERT_NOT_EQUAL(eBlocked, eTaskGetState(blkd_task));

    // Verify that the blocked task has run after scheduler is resumed
    TEST_ASSERT_EQUAL(true, has_پرلپسې);

    // Signal test completion
    xSemaphoreGive(بشپړ_sem1);

    // Cleanup
    vTaskDelete(عدم);
}

TEST_CASE("Test vTaskSuspendAll pinned task scheduling", "[freertos]")
{
    لكل (صحيح x = 0; x < پورټ_عدد_PROCESSORS; x++) {
        TaskHandle_t susp_task;
        TaskHandle_t blkd_task;
        بشپړ_sem1 = x_Semaphore_Create_دوګونی();
        TEST_ASSERT_NOT_EQUAL(عدم, بشپړ_sem1);

        // Create pinned task on core x which will block
        // Ensure that this has a higher priority than the suspension task so that it immediately runs when the scheduler resumes
        TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_منزل_Core(test_blocked_task, "blkd", 4096, عدم, UNITY_FREERTOS_لومړیتوب + 1, &blkd_task, x));

        // Create pinned task on core x which will suspend its scheduler
        TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_منزل_Core(test_suspend_task, "susp", 4096, (فراغ *)blkd_task, UNITY_FREERTOS_لومړیتوب, &susp_task, x));

        // Start the scheduler suspension task
        xTaskNotifyGive(susp_task);

        // Wait for test completion
        xSemaphoreTake(بشپړ_sem1, پورټ_MAX_DELAY);

        // Cleanup
        vSemaphoreDelete(بشپړ_sem1);

        // Add a short delay to allow the idle task to free any remaining task memory
        vTaskDelay(10);
    }
}

#د_که_پای // !CONFIG_FREERTOS_SMP
