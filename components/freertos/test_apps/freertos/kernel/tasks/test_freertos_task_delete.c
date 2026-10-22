/*
 * SPDX-FileCopyrightText: 2023-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * Test backported deletion behavior by creating tasks of various affinities and
 * check if the task memory is freed immediately under the correct conditions.
 *
 * The behavior of vTaskDelete() results in the immediate freeing of task memory
 * and the immediate execution of deletion callbacks for tasks which are not
 * running, provided they are not pinned to the other core (due to FPU cleanup
 * requirements).
 *
 * If the condition is not met, freeing of task memory and execution of
 * deletion callbacks will still be carried out by the Idle Task.
 */
#شامل <stdio.h>
#شامل "sdkconfig.h"
#شامل "freertos/FreeRTOS.h"
#شامل "freertos/task.h"
#شامل "freertos/semphr.h"
#شامل "esp_heap_caps.h"

#شامل "unity.h"
#شامل "test_utils.h"

#شامل "esp_rom_sys.h"

#تعریف_کرو NO_کا_TSKS  3
#تعریف_کرو DELAY_TICKS 2
/* Caps of all memory which is allocated from when a task is created */
#تعریف_کرو ڈھیر_CAPS   (MALLOC_CAP_اندرونی | MALLOC_CAP_8_ثنائی_ہندسہ)

#تعریف_کرو DELAY_US_ITERATIONS 1000

ساکن خالی tsk_self_del(خالی *param)
{
    vTaskDelete(تہی);  //Deleting self means deleting currently running task
}

ساکن خالی tsk_extern_del(خالی *param)
{
    vTaskDelay(پورٹ_MAX_DELAY);  //Await external deletion
}

ساکن خالی tsk_self_del_us_delay(خالی *param)
{
    uint32_t delay = (uint32_t)param;
    esp_rom_delay_us(delay);
    vTaskDelete(تہی);
}

TEST_CASE("FreeRTOS Delete Tasks", "[freertos]")
{
    /* -------------- Test vTaskDelete() on currently running tasks ----------------*/
    uint32_t before_گنتی = ux_Task_Get_نمبر_کا_Tasks();
    uint32_t before_ڈھیر = ڈھیر_caps_get_free_سائز(ڈھیر_CAPS);
    برائے (صحیح i = 0; i < CONFIG_FREERTOS_نمبر_کا_CORES; i++) {
        برائے (صحیح j = 0; j < NO_کا_TSKS; j++) {
            TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_مقصد_Core(tsk_self_del, "tsk_self", 1024, تہی, configMAX_PRIORITIES - 1, تہی, i));
        }
    }
    vTaskDelay(DELAY_TICKS);    //Minimal delay to see if Idle task cleans up all tasks awaiting deletion in a single tick
    TEST_ASSERT_EQUAL(before_گنتی, ux_Task_Get_نمبر_کا_Tasks());
    TEST_ASSERT_EQUAL(before_ڈھیر, ڈھیر_caps_get_free_سائز(ڈھیر_CAPS));

    /* ------------- Test vTaskDelete() on not currently running tasks ------------ */
    TaskHandle_t handles[NO_کا_TSKS];
    before_ڈھیر = ڈھیر_caps_get_free_سائز(ڈھیر_CAPS);
    //Create task pinned to the same core that will not run during task deletion
    برائے (صحیح j = 0 ; j < NO_کا_TSKS; j++) {
        TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_مقصد_Core(tsk_extern_del, "tsk_extern", 4096, تہی, configMAX_PRIORITIES - 1, &handles[j], x_پورٹ_Get_Core_ID()));
    }
    TEST_ASSERT_NOT_EQUAL(before_ڈھیر, ڈھیر_caps_get_free_سائز(ڈھیر_CAPS));    //Check tasks have been created
    //Delete the tasks, memory should be freed immediately
    برائے (صحیح j = 0; j < NO_کا_TSKS; j++) {
        vTaskDelete(handles[j]);
    }
    TEST_ASSERT_EQUAL(before_ڈھیر, ڈھیر_caps_get_free_سائز(ڈھیر_CAPS));

    /* Test self deleting no affinity task is not removed by idle task of other core before context switch */
    برائے (صحیح i = 0; i < DELAY_US_ITERATIONS; i += 10) {
        vTaskDelay(1);                          //Sync to next tick interrupt
        x_Task_Create_Pinned_مقصد_Core(tsk_self_del_us_delay, "delay", 1024, (خالی *)i, UNITY_FREERTOS_ترجیح - 1, تہی, tskNO_AFFINITY);
        esp_rom_delay_us(10);                       //Busy wait to ensure no affinity task runs on opposite core
    }

}

قسمتعریف ساخت {
    SemaphoreHandle_t sem;
    متلون bool deleted; // Check the deleted task doesn't keep running after being deleted
} tsk_بلاکیں_param_t;

/* Task blocks as often as possible
   (two or more of these can share the same semaphore and "juggle" it around)
*/
ساکن خالی tsk_بلاکیں_frequently(خالی *param)
{
    tsk_بلاکیں_param_t *p = (tsk_بلاکیں_param_t *)param;
    SemaphoreHandle_t sem = p->sem;
    srand(x_Task_Get_Tick_گنتی() ^ (صحیح)xTaskGetCurrentTaskHandle());
    جب (1) {
        أكد(!p->deleted);
        esp_rom_delay_us(rand() % 10);
        أكد(!p->deleted);
        xSemaphoreTake(sem, پورٹ_MAX_DELAY);
        أكد(!p->deleted);
        esp_rom_delay_us(rand() % 10);
        أكد(!p->deleted);
        xSemaphoreGive(sem);
    }
}

TEST_CASE("FreeRTOS Delete Blocked Tasks", "[freertos]")
{
    TaskHandle_t blocking_tasks[CONFIG_FREERTOS_نمبر_کا_CORES + 1]; // one per CPU, plus one unpinned task
    tsk_بلاکیں_param_t params[CONFIG_FREERTOS_نمبر_کا_CORES + 1] = { 0 };

    غیرعلامتی before = ڈھیر_caps_get_free_سائز(MALLOC_CAP_8_ثنائی_ہندسہ);
    لکھو("Free memory at start %u\n", before);

    /* Any bugs will depend on relative timing of destroying the tasks, so create & delete many times.

       Stop early if it looks like some resources have not been properly cleaned up.

       (1000 iterations takes about 9 seconds on ESP32 dual core)
     */
    غیرعلامتی iter = 0;
    برائے (iter = 0; iter < 1000; iter++) {
        // Create everything
        SemaphoreHandle_t sem = xSemaphoreCreateMutex();
        برائے (غیرعلامتی i = 0; i < CONFIG_FREERTOS_نمبر_کا_CORES + 1; i++) {
            params[i].deleted = false;
            params[i].sem = sem;

            TEST_ASSERT_EQUAL(pdTRUE,
                              x_Task_Create_Pinned_مقصد_Core(tsk_بلاکیں_frequently, "tsk_block", 4096, &params[i],
                                                      UNITY_FREERTOS_ترجیح - 1, &blocking_tasks[i],
                                                      i < CONFIG_FREERTOS_نمبر_کا_CORES ? i : tskNO_AFFINITY));
        }

        vTaskDelay(5); // Let the tasks juggle the mutex for a bit

        برائے (غیرعلامتی i = 0; i < CONFIG_FREERTOS_نمبر_کا_CORES + 1; i++) {
            vTaskSuspend(blocking_tasks[i]);
            vTaskDelete(blocking_tasks[i]);
            params[i].deleted = true;
        }
        vTaskDelay(4); // Yield to the idle task for cleanup

        // As an experiment, until IDFCI-4988 is fixed (CI runner loses connection to the target)
        // let's occasionally print a message to the console to ensure the test is running
        // and keep the console active.
        اگر (iter && iter % 100 == 0) {
            // In each iteration, we create CONFIG_FREERTOS_NUMBER_OF_CORES + 1 tasks
            لکھو("Deleted %u blocked tasks\n", iter * (CONFIG_FREERTOS_نمبر_کا_CORES + 1));
        }

        vSemaphoreDelete(sem);

        // Check we haven't leaked resources yet
        TEST_ASSERT_GREATER_OR_EQUAL(before - 256, ڈھیر_caps_get_free_سائز(MALLOC_CAP_8_ثنائی_ہندسہ));
    }
    لکھو("Deleted %u blocked tasks\n", iter * (CONFIG_FREERTOS_نمبر_کا_CORES + 1));
}
