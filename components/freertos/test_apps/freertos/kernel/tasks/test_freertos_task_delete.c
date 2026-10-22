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
#అన్తర్భావ <stdio.h>
#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ "freertos/semphr.h"
#అన్తర్భావ "esp_heap_caps.h"

#అన్తర్భావ "unity.h"
#అన్తర్భావ "test_utils.h"

#అన్తర్భావ "esp_rom_sys.h"

#నిర్వచించు NO_యొక్క_TSKS  3
#నిర్వచించు DELAY_TICKS 2
/* Caps of all memory which is allocated from when a task is created */
#నిర్వచించు కుప్ప_CAPS   (MALLOC_CAP_అంతర్గత | MALLOC_CAP_8_బిట్టు)

#నిర్వచించు DELAY_US_ITERATIONS 1000

స్థిర శూన్య tsk_self_del(శూన్య *param)
{
    vTaskDelete(ఖాళీ);  //Deleting self means deleting currently running task
}

స్థిర శూన్య tsk_extern_del(శూన్య *param)
{
    vTaskDelay(పోర్టు_MAX_DELAY);  //Await external deletion
}

స్థిర శూన్య tsk_self_del_us_delay(శూన్య *param)
{
    uint32_t delay = (uint32_t)param;
    esp_rom_delay_us(delay);
    vTaskDelete(ఖాళీ);
}

TEST_CASE("FreeRTOS Delete Tasks", "[freertos]")
{
    /* -------------- Test vTaskDelete() on currently running tasks ----------------*/
    uint32_t before_లెక్క = ux_Task_Get_సంఖ్య_యొక్క_Tasks();
    uint32_t before_కుప్ప = కుప్ప_caps_get_free_కొలత(కుప్ప_CAPS);
    కొరకు (పూర్ణాంకం i = 0; i < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; i++) {
        కొరకు (పూర్ణాంకం j = 0; j < NO_యొక్క_TSKS; j++) {
            TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_గమ్యం_Core(tsk_self_del, "tsk_self", 1024, ఖాళీ, configMAX_PRIORITIES - 1, ఖాళీ, i));
        }
    }
    vTaskDelay(DELAY_TICKS);    //Minimal delay to see if Idle task cleans up all tasks awaiting deletion in a single tick
    TEST_ASSERT_EQUAL(before_లెక్క, ux_Task_Get_సంఖ్య_యొక్క_Tasks());
    TEST_ASSERT_EQUAL(before_కుప్ప, కుప్ప_caps_get_free_కొలత(కుప్ప_CAPS));

    /* ------------- Test vTaskDelete() on not currently running tasks ------------ */
    TaskHandle_t handles[NO_యొక్క_TSKS];
    before_కుప్ప = కుప్ప_caps_get_free_కొలత(కుప్ప_CAPS);
    //Create task pinned to the same core that will not run during task deletion
    కొరకు (పూర్ణాంకం j = 0 ; j < NO_యొక్క_TSKS; j++) {
        TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_గమ్యం_Core(tsk_extern_del, "tsk_extern", 4096, ఖాళీ, configMAX_PRIORITIES - 1, &handles[j], x_పోర్టు_Get_Core_ID()));
    }
    TEST_ASSERT_NOT_EQUAL(before_కుప్ప, కుప్ప_caps_get_free_కొలత(కుప్ప_CAPS));    //Check tasks have been created
    //Delete the tasks, memory should be freed immediately
    కొరకు (పూర్ణాంకం j = 0; j < NO_యొక్క_TSKS; j++) {
        vTaskDelete(handles[j]);
    }
    TEST_ASSERT_EQUAL(before_కుప్ప, కుప్ప_caps_get_free_కొలత(కుప్ప_CAPS));

    /* Test self deleting no affinity task is not removed by idle task of other core before context switch */
    కొరకు (పూర్ణాంకం i = 0; i < DELAY_US_ITERATIONS; i += 10) {
        vTaskDelay(1);                          //Sync to next tick interrupt
        x_Task_Create_Pinned_గమ్యం_Core(tsk_self_del_us_delay, "delay", 1024, (శూన్య *)i, UNITY_FREERTOS_ప్రాధాన్యత - 1, ఖాళీ, tskNO_AFFINITY);
        esp_rom_delay_us(10);                       //Busy wait to ensure no affinity task runs on opposite core
    }

}

రకనిర్వచనం నిర్మాణం {
    SemaphoreHandle_t sem;
    అస్థిర bool deleted; // Check the deleted task doesn't keep running after being deleted
} tsk_ఖండాలు_param_t;

/* Task blocks as often as possible
   (two or more of these can share the same semaphore and "juggle" it around)
*/
స్థిర శూన్య tsk_ఖండాలు_frequently(శూన్య *param)
{
    tsk_ఖండాలు_param_t *p = (tsk_ఖండాలు_param_t *)param;
    SemaphoreHandle_t sem = p->sem;
    srand(x_Task_Get_Tick_లెక్క() ^ (పూర్ణాంకం)xTaskGetCurrentTaskHandle());
    అయితే (1) {
        ప్రతిజ్ఞా(!p->deleted);
        esp_rom_delay_us(rand() % 10);
        ప్రతిజ్ఞా(!p->deleted);
        xSemaphoreTake(sem, పోర్టు_MAX_DELAY);
        ప్రతిజ్ఞా(!p->deleted);
        esp_rom_delay_us(rand() % 10);
        ప్రతిజ్ఞా(!p->deleted);
        xSemaphoreGive(sem);
    }
}

TEST_CASE("FreeRTOS Delete Blocked Tasks", "[freertos]")
{
    TaskHandle_t blocking_tasks[CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES + 1]; // one per CPU, plus one unpinned task
    tsk_ఖండాలు_param_t params[CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES + 1] = { 0 };

    గుర్తులేని before = కుప్ప_caps_get_free_కొలత(MALLOC_CAP_8_బిట్టు);
    రాయి("Free memory at start %u\n", before);

    /* Any bugs will depend on relative timing of destroying the tasks, so create & delete many times.

       Stop early if it looks like some resources have not been properly cleaned up.

       (1000 iterations takes about 9 seconds on ESP32 dual core)
     */
    గుర్తులేని iter = 0;
    కొరకు (iter = 0; iter < 1000; iter++) {
        // Create everything
        SemaphoreHandle_t sem = xSemaphoreCreateMutex();
        కొరకు (గుర్తులేని i = 0; i < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES + 1; i++) {
            params[i].deleted = false;
            params[i].sem = sem;

            TEST_ASSERT_EQUAL(pdTRUE,
                              x_Task_Create_Pinned_గమ్యం_Core(tsk_ఖండాలు_frequently, "tsk_block", 4096, &params[i],
                                                      UNITY_FREERTOS_ప్రాధాన్యత - 1, &blocking_tasks[i],
                                                      i < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES ? i : tskNO_AFFINITY));
        }

        vTaskDelay(5); // Let the tasks juggle the mutex for a bit

        కొరకు (గుర్తులేని i = 0; i < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES + 1; i++) {
            vTaskSuspend(blocking_tasks[i]);
            vTaskDelete(blocking_tasks[i]);
            params[i].deleted = true;
        }
        vTaskDelay(4); // Yield to the idle task for cleanup

        // As an experiment, until IDFCI-4988 is fixed (CI runner loses connection to the target)
        // let's occasionally print a message to the console to ensure the test is running
        // and keep the console active.
        ఒకవేళ (iter && iter % 100 == 0) {
            // In each iteration, we create CONFIG_FREERTOS_NUMBER_OF_CORES + 1 tasks
            రాయి("Deleted %u blocked tasks\n", iter * (CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES + 1));
        }

        vSemaphoreDelete(sem);

        // Check we haven't leaked resources yet
        TEST_ASSERT_GREATER_OR_EQUAL(before - 256, కుప్ప_caps_get_free_కొలత(MALLOC_CAP_8_బిట్టు));
    }
    రాయి("Deleted %u blocked tasks\n", iter * (CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES + 1));
}
