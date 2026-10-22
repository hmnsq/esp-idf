/*
 * SPDX-FileCopyrightText: 2022-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/semphr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "driver/gptimer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_rom_sys.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unity.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "test_utils.h"

/*
Scheduler suspension behavior differs significantly in SMP FreeRTOS, thus none of these tests apply to SMP FreeRTOS
*/
#ਜੇ !CONFIG_FREERTOS_SMP

/*
GP timer is used to trigger an interrupt. Test cases will register an interrupt callback called from the timer's
interrupt callback. The functions below simply the interrupt registration/trigger/deregistration process.
*/

#ਜੇ SOC_GPTIMER_SUPPORTED

ਸਥਿਰ gptimer_handle_t gptimer = ਨਿਰਾ;
ਸਥਿਰ bool (*registered_intr_callback)(ਖਾਲੀ *) = ਨਿਰਾ;

ਸਥਿਰ bool on_timer_alarm_cb(gptimer_handle_t timer, ਅਚਲ gptimer_alarm_event_ਅੰਕੜੇ_t *edata, ਖਾਲੀ *user_ctx)
{
    bool yield;
    ਜੇ (registered_intr_callback) {
        yield = registered_intr_callback(user_ctx);
    } ਨਹੀਂਤਾਂ {
        yield = false;
    }
    ਵਾਪਸ yield;
}

ਸਥਿਰ ਖਾਲੀ ਰਜਿਸਟਰ_intr_cb(bool (*callback)(ਖਾਲੀ *), ਖਾਲੀ *callback_arg)
{
    gptimer_handle_t gptimer_ਆਰਜ਼ੀ;

    // Initialize a GP timer used to trigger an interrupt
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_ਗਿਣਤੀ_ਉੱਪਰ,
        .resolution_hz = 1000000, // 1MHz, 1 tick=1us
    };
    TEST_ESP_OK(gptimer_new_timer(&timer_config, &gptimer_ਆਰਜ਼ੀ));

    // Configure an alarm (of 1ms) and callback for the timer
    gptimer_alarm_config_t alarm_config = {
        .reload_ਗਿਣਤੀ = 0,
        .alarm_ਗਿਣਤੀ = 1000, // alarm period 1ms
        .flags.auto_reload_on_alarm = true,
    };
    gptimer_event_callbacks_t cbs = {
        .on_alarm = on_timer_alarm_cb,
    };
    TEST_ESP_OK(gptimer_ਰਜਿਸਟਰ_event_callbacks(gptimer_ਆਰਜ਼ੀ, &cbs, callback_arg));
    TEST_ESP_OK(gptimer_enable(gptimer_ਆਰਜ਼ੀ));
    TEST_ESP_OK(gptimer_ਸੈੱਟ_alarm_action(gptimer_ਆਰਜ਼ੀ, &alarm_config));

    gptimer = gptimer_ਆਰਜ਼ੀ;
    registered_intr_callback = callback;
}

ਸਥਿਰ ਖਾਲੀ trigger_intr_cb(ਖਾਲੀ)
{
    // Interrupt should be triggered in 1ms
    TEST_ESP_OK(gptimer_ਸ਼ੁਰੂ(gptimer));
}

ਸਥਿਰ ਖਾਲੀ deregister_intr_cb(ਖਾਲੀ)
{
    gptimer_handle_t gptimer_ਆਰਜ਼ੀ = gptimer;
    gptimer = ਨਿਰਾ;
    registered_intr_callback = ਨਿਰਾ;

    TEST_ESP_OK(gptimer_stop(gptimer_ਆਰਜ਼ੀ));
    TEST_ESP_OK(gptimer_disable(gptimer_ਆਰਜ਼ੀ));
    TEST_ESP_OK(gptimer_del_timer(gptimer_ਆਰਜ਼ੀ));
}

#ਜੇ_ਅੰਤ //SOC_GPTIMER_SUPPORTED

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

#ਜੇ !CONFIG_FREERTOS_UNICORE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TEST_BASIC_BUSY_DELAY_US    10000

ਸਥਿਰ ਅਸਥਿਰ ਪੂਰਨ taskA_sync;
ਸਥਿਰ ਅਸਥਿਰ ਪੂਰਨ taskB_sync;

ਸਥਿਰ ਖਾਲੀ test_multicore_taskA(ਖਾਲੀ *arg)
{
    // Wait to be started
    ulTaskNotifyTake(pdTRUE, ਪੋਰਟ_MAX_DELAY);

    // Check scheduler on core A is enabled
    TEST_ASSERT_EQUAL(taskSCHEDULER_RUNNING, xTaskGetSchedulerState());
    taskA_sync++;
    ਜਦੋਂ (taskB_sync != 1) {
        ;   // Wait for task B to complete its check
    }

    // Suspend the scheduler on core A
    vTaskSuspendAll();
    // Check scheduler is suspended on core A
    TEST_ASSERT_EQUAL(taskSCHEDULER_SUSPENDED, xTaskGetSchedulerState());
    taskA_sync++;
    ਜਦੋਂ (taskB_sync != 2) {
        ;   // Wait for task B to complete its check
    }

    // Busy spin for a while to simulate work done while scheduler is suspended
    esp_rom_delay_us(TEST_BASIC_BUSY_DELAY_US);

    // Check scheduler is still suspended on core A
    TEST_ASSERT_EQUAL(taskSCHEDULER_SUSPENDED, xTaskGetSchedulerState());
    taskA_sync++;
    ਜਦੋਂ (taskB_sync != 3) {
        ;   // Wait for task B to complete its check
    }

    // Resume the scheduler on core A
    xTaskResumeAll();
    // Check that scheduler has resumed resumed on core A
    TEST_ASSERT_EQUAL(taskSCHEDULER_RUNNING, xTaskGetSchedulerState());
    taskA_sync++;
    ਜਦੋਂ (taskB_sync != 4) {
        ;   // Wait for task B to complete its check
    }

    // Indicate done and wait to be deleted
    SemaphoreHandle_t ਪੂਰਾ_sem = (SemaphoreHandle_t)arg;
    xSemaphoreGive(ਪੂਰਾ_sem);
    vTaskSuspend(ਨਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ test_multicore_taskB(ਖਾਲੀ *arg)
{
    // Wait to be started
    ulTaskNotifyTake(pdTRUE, ਪੋਰਟ_MAX_DELAY);

    ਲਈ (ਪੂਰਨ i = 1; i <= 4; i++) {
        // Wait for suspended trigger from task A
        ਜਦੋਂ (taskA_sync != i) {
            ;
        }
        // Check that scheduler is still running on core B
        TEST_ASSERT_EQUAL(taskSCHEDULER_RUNNING, xTaskGetSchedulerState());
        taskB_sync++;
    }

    // Indicate done and wait to be deleted
    SemaphoreHandle_t ਪੂਰਾ_sem = (SemaphoreHandle_t)arg;
    xSemaphoreGive(ਪੂਰਾ_sem);
    vTaskSuspend(ਨਿਰਾ);
}

TEST_CASE("Test vTaskSuspendAll() and xTaskResumeAll() multicore", "[freertos]")
{
    SemaphoreHandle_t ਪੂਰਾ_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES, 0);
    TEST_ASSERT_NOT_EQUAL(ਨਿਰਾ, ਪੂਰਾ_sem);

    ਲਈ (ਪੂਰਨ i = 0; i < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; i++) {
        // Create tasks on core A and core B
        TaskHandle_t taskA_hdl;
        TaskHandle_t taskB_hdl;
        TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(test_multicore_taskA, "taskA", 2048, (ਖਾਲੀ *)ਪੂਰਾ_sem, UNITY_FREERTOS_ਤਰਜੀਹ - 1, &taskA_hdl, i));
        TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(test_multicore_taskB, "taskB", 2048, (ਖਾਲੀ *)ਪੂਰਾ_sem, UNITY_FREERTOS_ਤਰਜੀਹ - 1, &taskB_hdl, !i));
        // Start the tasks and wait for their completion
        taskA_sync = 0;
        taskB_sync = 0;
        xTaskNotifyGive(taskA_hdl);
        xTaskNotifyGive(taskB_hdl);
        ਲਈ (ਪੂਰਨ j = 0; j < 2; j++) {
            xSemaphoreTake(ਪੂਰਾ_sem, ਪੋਰਟ_MAX_DELAY);
        }

        // Suspend the test tasks in case they haven't suspended themselves yet
        vTaskSuspend(taskA_hdl);
        vTaskSuspend(taskB_hdl);
        vTaskDelay(10);

        // Cleanup the tasks
        vTaskDelete(taskA_hdl);
        vTaskDelete(taskB_hdl);
    }

    vSemaphoreDelete(ਪੂਰਾ_sem);
    // Add a short delay to allow the idle task to free any remaining task memory
    vTaskDelay(10);
}
#ਜੇ_ਅੰਤ  // !CONFIG_FREERTOS_UNICORE

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

#ਜੇ !CONFIG_FREERTOS_UNICORE

#ਜੇ SOC_GPTIMER_SUPPORTED

ਸਥਿਰ ਅਸਥਿਰ ਪੂਰਨ test_unblk_sync;
ਸਥਿਰ SemaphoreHandle_t test_unblk_ਪੂਰਾ_sem;

ਸਥਿਰ bool test_unblk_coreA_isr(ਖਾਲੀ *arg)
{
    TaskHandle_t a2_task_hdl = (TaskHandle_t)arg;
    ਅਧਾਰ_Type_t task_woken = pdFALSE;
    // Unblock task b2
    vTaskNotifyGiveFromISR(a2_task_hdl, &task_woken);
    ਵਾਪਸ (task_woken == pdTRUE);
}

ਸਥਿਰ ਖਾਲੀ test_unblk_a2_task(ਖਾਲੀ *arg)
{
    ਅਸਥਿਰ ਪੂਰਨ *a2_task_ਲੜੀਵਾਰ_ਗਿਣਤੀ = (ਅਸਥਿਰ ਪੂਰਨ *)arg;
    // Wait to be unblocked by A1
    ulTaskNotifyTake(pdTRUE, ਪੋਰਟ_MAX_DELAY);
    (*a2_task_ਲੜੀਵਾਰ_ਗਿਣਤੀ)++;
    // Wait to be unblocked by Core A ISR
    ulTaskNotifyTake(pdTRUE, ਪੋਰਟ_MAX_DELAY);
    (*a2_task_ਲੜੀਵਾਰ_ਗਿਣਤੀ)++;
    // Wait to be deleted
    vTaskSuspend(ਨਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ test_unblk_a1_task(ਖਾਲੀ *arg)
{
    ਅਸਥਿਰ ਪੂਰਨ a2_task_ਲੜੀਵਾਰ_ਗਿਣਤੀ = 0;

    // Create task A2 to block on this core (i.e., core A)
    TaskHandle_t a2_task_hdl;
    TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(test_unblk_a2_task, "A2", 8192, (ਖਾਲੀ *)&a2_task_ਲੜੀਵਾਰ_ਗਿਣਤੀ, UNITY_FREERTOS_ਤਰਜੀਹ + 2, &a2_task_hdl, x_ਪੋਰਟ_Get_Core_ID()));
    // Install an interrupt on core A
    ਰਜਿਸਟਰ_intr_cb(test_unblk_coreA_isr, (ਖਾਲੀ *)a2_task_hdl);

    // Wait to be started by the main task
    ulTaskNotifyTake(pdTRUE, ਪੋਰਟ_MAX_DELAY);

    // Start B1
    xTaskNotifyGive((TaskHandle_t)arg);
    ਜਦੋਂ (test_unblk_sync != 1) {
        ; // Wait for confirmation from B1 that scheduler has been suspended on Core B
    }
    // Verify that the scheduler is still enabled on core A
    TEST_ASSERT_EQUAL(taskSCHEDULER_RUNNING, xTaskGetSchedulerState());

    // Unblock A2, it should preempt immediately due to its higher priority
    xTaskNotifyGive(a2_task_hdl);
    // Verify that task A2 has run
    TEST_ASSERT_EQUAL(1, a2_task_ਲੜੀਵਾਰ_ਗਿਣਤੀ);

    // Trigger an ISR on this core A to unblock task A2. A2 should preempt immediately due to its higher priority
    trigger_intr_cb();
    esp_rom_delay_us(2000); // Short busy delay to ensure interrupt has triggered
    // Verify that task A2 has run
    TEST_ASSERT_EQUAL(2, a2_task_ਲੜੀਵਾਰ_ਗਿਣਤੀ);

    // Trigger B1 to resume scheduling on core B
    test_unblk_sync = 2;
    ਜਦੋਂ (test_unblk_sync != 3) {
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
    xSemaphoreGive(test_unblk_ਪੂਰਾ_sem);
    vTaskSuspend(ਨਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ test_unblk_b1_task(ਖਾਲੀ *arg)
{
    // Wait to be started by A1
    ulTaskNotifyTake(pdTRUE, ਪੋਰਟ_MAX_DELAY);

    // Check scheduler is running on core B
    TEST_ASSERT_EQUAL(taskSCHEDULER_RUNNING, xTaskGetSchedulerState());
    // Suspend the scheduler on core B
    vTaskSuspendAll();
    TEST_ASSERT_EQUAL(taskSCHEDULER_SUSPENDED, xTaskGetSchedulerState());

    // Indicate to A1 that core B scheduler has been suspended
    test_unblk_sync = 1;

    ਜਦੋਂ (test_unblk_sync != 2) {
        ;   // Wait for trigger from A1
    }
    // Resume the scheduler on core B
    xTaskResumeAll();
    TEST_ASSERT_EQUAL(taskSCHEDULER_RUNNING, xTaskGetSchedulerState());
    // Indicate to A1 that core B scheduler has been resumed
    test_unblk_sync = 3;

    // Indicate done and wait to be deleted
    xSemaphoreGive(test_unblk_ਪੂਰਾ_sem);
    vTaskSuspend(ਨਿਰਾ);
}

TEST_CASE("Test vTaskSuspendAll allows scheduling on other cores", "[freertos]")
{
    test_unblk_ਪੂਰਾ_sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES, 0);
    TEST_ASSERT_NOT_EQUAL(ਨਿਰਾ, test_unblk_ਪੂਰਾ_sem);

    ਲਈ (ਪੂਰਨ i = 0; i < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; i++) {
        test_unblk_sync = 0;
        // Create a tasks
        TaskHandle_t a1_task_hdl;
        TaskHandle_t b1_task_hdl;
        TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(test_unblk_b1_task, "B1", 8192, ਨਿਰਾ, UNITY_FREERTOS_ਤਰਜੀਹ + 1, &b1_task_hdl, !i));
        TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(test_unblk_a1_task, "A1", 8192, (ਖਾਲੀ *)b1_task_hdl, UNITY_FREERTOS_ਤਰਜੀਹ + 1, &a1_task_hdl, i));
        // Start A1 to and wait for both tasks to complete
        xTaskNotifyGive(a1_task_hdl);
        ਲਈ (ਪੂਰਨ j = 0; j < 2; j++) {
            xSemaphoreTake(test_unblk_ਪੂਰਾ_sem, ਪੋਰਟ_MAX_DELAY);
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

    vSemaphoreDelete(test_unblk_ਪੂਰਾ_sem);
    // Add a short delay to allow the idle task to free any remaining task memory
    vTaskDelay(10);
}

#ਜੇ_ਅੰਤ //SOC_GPTIMER_SUPPORTED

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
ਸਥਿਰ SemaphoreHandle_t test_unpinned_sem;
ਅਸਥਿਰ uint8_t unpinned_task_running = 0;

ਸਥਿਰ ਖਾਲੀ test_unpinned_task(ਖਾਲੀ* arg)
{
    ਅਧਾਰ_Type_t *core_A = (ਅਧਾਰ_Type_t *)arg;

    // Verify that the task is running on core B
    TEST_ASSERT_EQUAL(!(*core_A), x_ਪੋਰਟ_Get_Core_ID());

    // Wait to be unblocked by the pinned task
    ulTaskNotifyTake(pdTRUE, ਪੋਰਟ_MAX_DELAY);

    // Verify that the task is running on core A once it is unblocked
    TEST_ASSERT_EQUAL((*core_A), x_ਪੋਰਟ_Get_Core_ID());
    unpinned_task_running = 1;

    // Mark test completion
    xSemaphoreGive(test_unpinned_sem);

    // Cleanup
    vTaskSuspend(ਨਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ test_pinned_task(ਖਾਲੀ* arg)
{
    TaskHandle_t unpinned_task_hdl = (TaskHandle_t)arg;

    // Wait to be notified by the main task
    ulTaskNotifyTake(pdTRUE, ਪੋਰਟ_MAX_DELAY);

    // Check scheduler is running on core B
    TEST_ASSERT_EQUAL(taskSCHEDULER_RUNNING, xTaskGetSchedulerState());

    // Suspend the scheduler on core B
    vTaskSuspendAll();

    // Check scheduler is suspended on core B
    TEST_ASSERT_EQUAL(taskSCHEDULER_SUSPENDED, xTaskGetSchedulerState());

    // Unblock the unpinned task
    // It should run on core A which is not in suspended state
    // We use the FromISR() call to create an ISR scenario and to force the unblocked task to be placed in the ready list
    ਅਧਾਰ_Type_t yield = 0;
    vTaskNotifyGiveFromISR(unpinned_task_hdl, &yield);

    // Busy wait to ensure that the unpinned task is scheduled
    ਜਦੋਂ (unpinned_task_running == 0)
        ;

    // Resume scheduler on core B
    xTaskResumeAll();

    // Mark test completion
    xSemaphoreGive(test_unpinned_sem);

    // Cleanup
    vTaskSuspend(ਨਿਰਾ);
}

TEST_CASE("Test vTaskSuspendAll doesn't block unpinned tasks from being scheduled on other cores", "[freertos]")
{
    TaskHandle_t pinned_task_hdl;
    TaskHandle_t unpinned_task_hdl;
    ਅਧਾਰ_Type_t core_A = x_ਪੋਰਟ_Get_Core_ID();
    test_unpinned_sem = xSemaphoreCreateCounting(2, 0);

    // Suspend the scheduler on core A to avoid the unpinned task from being spawned on core A
    vTaskSuspendAll();

    // Create unpinned task. This should spin up on core B as we have suspended the scheduler on core A
    TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(test_unpinned_task, "unpinned_task", 8192, &core_A, UNITY_FREERTOS_ਤਰਜੀਹ + 1, &unpinned_task_hdl, tskNO_AFFINITY));

    // Create task pinned to core B
    TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(test_pinned_task, "pinned_task", 8192, (ਖਾਲੀ *)unpinned_task_hdl, UNITY_FREERTOS_ਤਰਜੀਹ + 1, &pinned_task_hdl, !core_A));

    // Resume the scheduler on core A
    xTaskResumeAll();

    // Unblock the pinned task
    xTaskNotifyGive(pinned_task_hdl);

    // Wait for test completion
    ਲਈ (ਪੂਰਨ i = 0; i < 2; i++) {
        xSemaphoreTake(test_unpinned_sem, ਪੋਰਟ_MAX_DELAY);
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
#ਜੇ_ਅੰਤ  // !CONFIG_FREERTOS_UNICORE

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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TEST_PENDED_ਅੰਕ_BLOCKED_TASKS   4

#ਜੇ SOC_GPTIMER_SUPPORTED

ਸਥਿਰ bool test_pended_isr(ਖਾਲੀ *arg)
{
    TaskHandle_t *blkd_tsks = (TaskHandle_t *)arg;
    ਅਧਾਰ_Type_t task_woken = pdFALSE;
    // Unblock the blocked tasks
    ਲਈ (ਪੂਰਨ i = 0; i < TEST_PENDED_ਅੰਕ_BLOCKED_TASKS; i++) {
        vTaskNotifyGiveFromISR(blkd_tsks[i], &task_woken);
    }

    ਵਾਪਸ (task_woken == pdTRUE);
}

ਸਥਿਰ ਖਾਲੀ test_pended_blkd_task(ਖਾਲੀ *arg)
{
    ਅਸਥਿਰ bool *has_ਲੜੀਵਾਰ = (bool *)arg;
    // Wait to be unblocked
    ulTaskNotifyTake(pdTRUE, ਪੋਰਟ_MAX_DELAY);
    // Indicate the task been unblocked and has run
    *has_ਲੜੀਵਾਰ = true;
    // Wait to be deleted
    vTaskSuspend(ਨਿਰਾ);
}

ਸਥਿਰ ਖਾਲੀ test_pended_running_task(ਖਾਲੀ *arg)
{
    TaskHandle_t main_task_hdl = (TaskHandle_t)arg;
    TaskHandle_t blkd_tsks[TEST_PENDED_ਅੰਕ_BLOCKED_TASKS];
    ਅਸਥਿਰ bool has_ਲੜੀਵਾਰ[TEST_PENDED_ਅੰਕ_BLOCKED_TASKS];

    // Created blocked tasks pinned to each core
    ਲਈ (ਪੂਰਨ i = 0; i < TEST_PENDED_ਅੰਕ_BLOCKED_TASKS; i++) {
        has_ਲੜੀਵਾਰ[i] = false;
        TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(test_pended_blkd_task, "blkd", 4096, (ਖਾਲੀ *)&has_ਲੜੀਵਾਰ[i], UNITY_FREERTOS_ਤਰਜੀਹ + 2, &blkd_tsks[i], i % CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES));
    }
    vTaskDelay(10);

    // Install an interrupt on the current core core
    ਰਜਿਸਟਰ_intr_cb(test_pended_isr, (ਖਾਲੀ *)blkd_tsks);

    // Checked that all tasks are blocked and have no run yet
    ਲਈ (ਪੂਰਨ i = 0; i < TEST_PENDED_ਅੰਕ_BLOCKED_TASKS; i++) {
        TEST_ASSERT_EQUAL(eBlocked, eTaskGetState(blkd_tsks[i])); // Should be eSuspended due to portMAX_DELAY
        TEST_ASSERT_EQUAL(false, has_ਲੜੀਵਾਰ[i]);
    }

    // Suspend the scheduler on the current core
    vTaskSuspendAll();

    // Trigger the interrupt to unblocked the blocked tasks
    trigger_intr_cb();
    esp_rom_delay_us(2000); // Short busy delay to ensure interrupt has triggered

    // Check that tasks which have affinity to the current core are blocked and have not run as the scheduler is suspended.
    // While tasks which do not have affinity to the current core are unblocked.
    ਲਈ (ਪੂਰਨ i = 0; i < TEST_PENDED_ਅੰਕ_BLOCKED_TASKS; i++) {
        // Note: We use eBlocked instead of eReady due to a bug in eTaskGetState(). See (IDF-5543)
        ਜੇ (xTaskGetCoreID(blkd_tsks[i]) == x_ਪੋਰਟ_Get_Core_ID()) {
            TEST_ASSERT_EQUAL(eBlocked, eTaskGetState(blkd_tsks[i]));
            TEST_ASSERT_EQUAL(false, has_ਲੜੀਵਾਰ[i]);
        } ਨਹੀਂਤਾਂ {
            TEST_ASSERT_NOT_EQUAL(eBlocked, eTaskGetState(blkd_tsks[i]));
        }
    }

    // Resume the scheduler on the current core to schedule the unblocked tasks
    xTaskResumeAll();
    esp_rom_delay_us(10000); // Busy delay to ensure each task has enough time to run

    // Check that all tasks have run
    ਲਈ (ਪੂਰਨ i = 0; i < TEST_PENDED_ਅੰਕ_BLOCKED_TASKS; i++) {
        TEST_ASSERT_EQUAL(true, has_ਲੜੀਵਾਰ[i]);
    }

    // Clean up the interrupt and tasks
    deregister_intr_cb();
    ਲਈ (ਪੂਰਨ i = 0; i < TEST_PENDED_ਅੰਕ_BLOCKED_TASKS; i++) {
        vTaskDelete(blkd_tsks[i]);
    }

    // Notify completion and wait for deletion
    xTaskNotifyGive(main_task_hdl);
    vTaskSuspend(ਨਿਰਾ);
}

TEST_CASE("Test xTaskResumeAll resumes pended tasks", "[freertos]")
{
    // Run the test on each core
    ਲਈ (ਪੂਰਨ i = 0; i < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; i++) {
        TaskHandle_t susp_tsk_hdl;
        TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(test_pended_running_task, "susp", 2048, (ਖਾਲੀ *)xTaskGetCurrentTaskHandle(), UNITY_FREERTOS_ਤਰਜੀਹ + 1, &susp_tsk_hdl, i));
        // Wait for to be notified to test completion
        ulTaskNotifyTake(pdTRUE, ਪੋਰਟ_MAX_DELAY);
        // Suspend the test task in case it hasn't suspended itself yet
        vTaskSuspend(susp_tsk_hdl);
        vTaskDelay(10);
        vTaskDelete(susp_tsk_hdl);
    }
    // Add a short delay to allow the idle task to free any remaining task memory
    vTaskDelay(10);
}

#ਜੇ_ਅੰਤ //SOC_GPTIMER_SUPPORTED

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

#ਜੇ !CONFIG_FREERTOS_UNICORE
TaskHandle_t blkd_tasks[TEST_PENDED_ਅੰਕ_BLOCKED_TASKS];
SemaphoreHandle_t ਪੂਰਾ_sem;

ਸਥਿਰ ਖਾਲੀ test_susp_task(ਖਾਲੀ *arg)
{
    bool *has_ਲੜੀਵਾਰ = (bool *)arg;

    // Suspend the scheduler on this core
    vTaskSuspendAll();

    ਲਈ (ਪੂਰਨ i = 0; i < TEST_PENDED_ਅੰਕ_BLOCKED_TASKS; i++) {
        ਜੇ ((i % CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES) == x_ਪੋਰਟ_Get_Core_ID()) {
            // Unblock the blocked tasks pinned to this core.
            // We use the FromISR() call to create an ISR scenario and to force the unblocked task to be placed
            // on the pending ready list
            ਅਧਾਰ_Type_t yield = pdFALSE;
            vTaskNotifyGiveFromISR(blkd_tasks[i], &yield);

            // The unblocked task must still be blocked and must not have run
            TEST_ASSERT_EQUAL(eBlocked, eTaskGetState(blkd_tasks[i]));
            TEST_ASSERT_EQUAL(false, has_ਲੜੀਵਾਰ[i]);
        }
    }

    // Resume the scheduler on this core
    xTaskResumeAll();

    // Signal test completion
    xSemaphoreGive(ਪੂਰਾ_sem);

    // Wait for cleanup
    vTaskSuspend(ਨਿਰਾ);
}

#ਜੇ SOC_GPTIMER_SUPPORTED
TEST_CASE("Test xTaskSuspendAll on all cores pends all tasks and xTaskResumeAll on all cores resumes all tasks", "[freertos]")
{
    ਅਸਥਿਰ bool has_ਲੜੀਵਾਰ[TEST_PENDED_ਅੰਕ_BLOCKED_TASKS];
    ਪੂਰਾ_sem = x_Semaphore_Create_ਦੁਆਧਾਰੀ();

    // Creat blocked tasks pinned to each core
    ਲਈ (ਪੂਰਨ i = 0; i < TEST_PENDED_ਅੰਕ_BLOCKED_TASKS; i++) {
        has_ਲੜੀਵਾਰ[i] = false;
        TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(test_pended_blkd_task, "blkd", 4096, (ਖਾਲੀ *)&has_ਲੜੀਵਾਰ[i], UNITY_FREERTOS_ਤਰਜੀਹ + 2, &blkd_tasks[i], i % CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES));
    }
    vTaskDelay(10);

    // Create pinned task on the other core which will suspend its scheduler
    TaskHandle_t susp_task;
    TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(test_susp_task, "susp_task", 2048, (ਖਾਲੀ *)has_ਲੜੀਵਾਰ, UNITY_FREERTOS_ਤਰਜੀਹ, &susp_task, !x_ਪੋਰਟ_Get_Core_ID()));

    // Suspend the scheduler on this core
    vTaskSuspendAll();

    ਲਈ (ਪੂਰਨ i = 0; i < TEST_PENDED_ਅੰਕ_BLOCKED_TASKS; i++) {
        ਜੇ ((i % CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES) == x_ਪੋਰਟ_Get_Core_ID()) {
            // Unblock the blocked tasks pinned to this core.
            // We use the FromISR() call to create an ISR scenario and to force the unblocked task to be placed
            // on the pending ready list
            ਅਧਾਰ_Type_t yield = pdFALSE;
            vTaskNotifyGiveFromISR(blkd_tasks[i], &yield);

            // The unblocked task must still be blocked and must not have run
            TEST_ASSERT_EQUAL(eBlocked, eTaskGetState(blkd_tasks[i]));
            TEST_ASSERT_EQUAL(false, has_ਲੜੀਵਾਰ[i]);
        }
    }

    // Resume scheduler on this core
    xTaskResumeAll();

    // Wait for test completion
    xSemaphoreTake(ਪੂਰਾ_sem, ਪੋਰਟ_MAX_DELAY);

    // Verify that all blocked tasks have resumed and run when the schdulers are resumed on both cores
    ਲਈ (ਪੂਰਨ i = 0; i < TEST_PENDED_ਅੰਕ_BLOCKED_TASKS; i++) {
        TEST_ASSERT_NOT_EQUAL(eBlocked, eTaskGetState(blkd_tasks[i]));
        TEST_ASSERT_EQUAL(true, has_ਲੜੀਵਾਰ[i]);
    }

    // Cleanup
    ਲਈ (ਪੂਰਨ i = 0; i < TEST_PENDED_ਅੰਕ_BLOCKED_TASKS; i++) {
        vTaskSuspend(blkd_tasks[i]);
        vTaskDelay(10);
        vTaskDelete(blkd_tasks[i]);
    }
    vTaskSuspend(susp_task);
    vTaskDelay(10);
    vTaskDelete(susp_task);
    vSemaphoreDelete(ਪੂਰਾ_sem);
}
#ਜੇ_ਅੰਤ //SOC_GPTIMER_SUPPORTED
#ਜੇ_ਅੰਤ  // !CONFIG_FREERTOS_UNICORE

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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TEST_BLOCKED_TASK_DELAY_MS   100

ਅਸਥਿਰ ਸਥਿਰ bool has_ਲੜੀਵਾਰ = false;
SemaphoreHandle_t ਪੂਰਾ_sem1;

ਖਾਲੀ test_blocked_task(ਖਾਲੀ *arg)
{
    // Wait to be started
    ulTaskNotifyTake(pdTRUE, ਪੋਰਟ_MAX_DELAY);

    // Mark has_run as false to begin the test
    has_ਲੜੀਵਾਰ = false;

    // Got to blocked state
    vTaskDelay(TEST_BLOCKED_TASK_DELAY_MS / ਪੋਰਟ_TICK_PERIOD_MS);

    // Mark when this task runs
    has_ਲੜੀਵਾਰ = true;

    // Cleanup
    vTaskDelete(ਨਿਰਾ);
}

ਖਾਲੀ test_suspend_task(ਖਾਲੀ *arg)
{
    TaskHandle_t blkd_task = (TaskHandle_t)arg;

    // Wait to be started
    ulTaskNotifyTake(pdTRUE, ਪੋਰਟ_MAX_DELAY);

    // Start the task which would block
    xTaskNotifyGive(blkd_task);

    // Verify the state of the blocked task is eBlocked
    TEST_ASSERT_EQUAL(eBlocked, eTaskGetState(blkd_task));

    // Suspend the scheduler on this core
    vTaskSuspendAll();

    // Busy spin for a time which ensures that the blocked task's delay expires
    esp_rom_delay_us(TEST_BLOCKED_TASK_DELAY_MS * 1000 * 2);

    // Verify that the blocked task has not been scheduled
    TEST_ASSERT_EQUAL(false, has_ਲੜੀਵਾਰ);

    // Resume the scheduler
    xTaskResumeAll();

    // Let the blocked task to be scheduled
    vTaskDelay(10);

    // Verify that state of the blocked task is not eBlocked
    TEST_ASSERT_NOT_EQUAL(eBlocked, eTaskGetState(blkd_task));

    // Verify that the blocked task has run after scheduler is resumed
    TEST_ASSERT_EQUAL(true, has_ਲੜੀਵਾਰ);

    // Signal test completion
    xSemaphoreGive(ਪੂਰਾ_sem1);

    // Cleanup
    vTaskDelete(ਨਿਰਾ);
}

TEST_CASE("Test vTaskSuspendAll pinned task scheduling", "[freertos]")
{
    ਲਈ (ਪੂਰਨ x = 0; x < ਪੋਰਟ_ਅੰਕ_PROCESSORS; x++) {
        TaskHandle_t susp_task;
        TaskHandle_t blkd_task;
        ਪੂਰਾ_sem1 = x_Semaphore_Create_ਦੁਆਧਾਰੀ();
        TEST_ASSERT_NOT_EQUAL(ਨਿਰਾ, ਪੂਰਾ_sem1);

        // Create pinned task on core x which will block
        // Ensure that this has a higher priority than the suspension task so that it immediately runs when the scheduler resumes
        TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(test_blocked_task, "blkd", 4096, ਨਿਰਾ, UNITY_FREERTOS_ਤਰਜੀਹ + 1, &blkd_task, x));

        // Create pinned task on core x which will suspend its scheduler
        TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(test_suspend_task, "susp", 4096, (ਖਾਲੀ *)blkd_task, UNITY_FREERTOS_ਤਰਜੀਹ, &susp_task, x));

        // Start the scheduler suspension task
        xTaskNotifyGive(susp_task);

        // Wait for test completion
        xSemaphoreTake(ਪੂਰਾ_sem1, ਪੋਰਟ_MAX_DELAY);

        // Cleanup
        vSemaphoreDelete(ਪੂਰਾ_sem1);

        // Add a short delay to allow the idle task to free any remaining task memory
        vTaskDelay(10);
    }
}

#ਜੇ_ਅੰਤ // !CONFIG_FREERTOS_SMP
