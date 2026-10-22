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
#ضمن <stdio.h>
#ضمن "sdkconfig.h"
#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن "freertos/semphr.h"
#ضمن "esp_heap_caps.h"

#ضمن "unity.h"
#ضمن "test_utils.h"

#ضمن "esp_rom_sys.h"

#define NO_OF_TSKS  3
#define DELAY_TICKS 2
/* Caps of all memory which is allocated from when a task is created */
#define HEAP_CAPS   (MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT)

#define DELAY_US_ITERATIONS 1000

ساكن فراغ tsk_self_del(فراغ *param)
{
    vTaskDelete(عدم);  //Deleting self means deleting currently running task
}

ساكن فراغ tsk_extern_del(فراغ *param)
{
    vTaskDelay(portMAX_DELAY);  //Await external deletion
}

ساكن فراغ tsk_self_del_us_delay(فراغ *param)
{
    uint32_t delay = (uint32_t)param;
    esp_rom_delay_us(delay);
    vTaskDelete(عدم);
}

TEST_CASE("FreeRTOS Delete Tasks", "[freertos]")
{
    /* -------------- Test vTaskDelete() on currently running tasks ----------------*/
    uint32_t before_count = uxTaskGetNumberOfTasks();
    uint32_t before_heap = heap_caps_get_free_size(HEAP_CAPS);
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_NUMBER_OF_CORES; i++) {
        لكل (صحيح j = 0; j < NO_OF_TSKS; j++) {
            TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(tsk_self_del, "tsk_self", 1024, عدم, configMAX_PRIORITIES - 1, عدم, i));
        }
    }
    vTaskDelay(DELAY_TICKS);    //Minimal delay to see if Idle task cleans up all tasks awaiting deletion in a single tick
    TEST_ASSERT_EQUAL(before_count, uxTaskGetNumberOfTasks());
    TEST_ASSERT_EQUAL(before_heap, heap_caps_get_free_size(HEAP_CAPS));

    /* ------------- Test vTaskDelete() on not currently running tasks ------------ */
    TaskHandle_t handles[NO_OF_TSKS];
    before_heap = heap_caps_get_free_size(HEAP_CAPS);
    //Create task pinned to the same core that will not run during task deletion
    لكل (صحيح j = 0 ; j < NO_OF_TSKS; j++) {
        TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(tsk_extern_del, "tsk_extern", 4096, عدم, configMAX_PRIORITIES - 1, &handles[j], xPortGetCoreID()));
    }
    TEST_ASSERT_NOT_EQUAL(before_heap, heap_caps_get_free_size(HEAP_CAPS));    //Check tasks have been created
    //Delete the tasks, memory should be freed immediately
    لكل (صحيح j = 0; j < NO_OF_TSKS; j++) {
        vTaskDelete(handles[j]);
    }
    TEST_ASSERT_EQUAL(before_heap, heap_caps_get_free_size(HEAP_CAPS));

    /* Test self deleting no affinity task is not removed by idle task of other core before context switch */
    لكل (صحيح i = 0; i < DELAY_US_ITERATIONS; i += 10) {
        vTaskDelay(1);                          //Sync to next tick interrupt
        xTaskCreatePinnedToCore(tsk_self_del_us_delay, "delay", 1024, (فراغ *)i, UNITY_FREERTOS_PRIORITY - 1, عدم, tskNO_AFFINITY);
        esp_rom_delay_us(10);                       //Busy wait to ensure no affinity task runs on opposite core
    }

}

تعريفنوع بنية {
    SemaphoreHandle_t sem;
    متطاير bool deleted; // Check the deleted task doesn't keep running after being deleted
} tsk_blocks_param_t;

/* Task blocks as often as possible
   (two or more of these can share the same semaphore and "juggle" it around)
*/
ساكن فراغ tsk_blocks_frequently(فراغ *param)
{
    tsk_blocks_param_t *p = (tsk_blocks_param_t *)param;
    SemaphoreHandle_t sem = p->sem;
    srand(xTaskGetTickCount() ^ (صحيح)xTaskGetCurrentTaskHandle());
    ܟܡܐ (1) {
        أكد(!p->deleted);
        esp_rom_delay_us(rand() % 10);
        أكد(!p->deleted);
        xSemaphoreTake(sem, portMAX_DELAY);
        أكد(!p->deleted);
        esp_rom_delay_us(rand() % 10);
        أكد(!p->deleted);
        xSemaphoreGive(sem);
    }
}

TEST_CASE("FreeRTOS Delete Blocked Tasks", "[freertos]")
{
    TaskHandle_t blocking_tasks[CONFIG_FREERTOS_NUMBER_OF_CORES + 1]; // one per CPU, plus one unpinned task
    tsk_blocks_param_t params[CONFIG_FREERTOS_NUMBER_OF_CORES + 1] = { 0 };

    unsigned before = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    printf("Free memory at start %u\n", before);

    /* Any bugs will depend on relative timing of destroying the tasks, so create & delete many times.

       Stop early if it looks like some resources have not been properly cleaned up.

       (1000 iterations takes about 9 seconds on ESP32 dual core)
     */
    unsigned iter = 0;
    لكل (iter = 0; iter < 1000; iter++) {
        // Create everything
        SemaphoreHandle_t sem = xSemaphoreCreateMutex();
        لكل (unsigned i = 0; i < CONFIG_FREERTOS_NUMBER_OF_CORES + 1; i++) {
            params[i].deleted = false;
            params[i].sem = sem;

            TEST_ASSERT_EQUAL(pdTRUE,
                              xTaskCreatePinnedToCore(tsk_blocks_frequently, "tsk_block", 4096, &params[i],
                                                      UNITY_FREERTOS_PRIORITY - 1, &blocking_tasks[i],
                                                      i < CONFIG_FREERTOS_NUMBER_OF_CORES ? i : tskNO_AFFINITY));
        }

        vTaskDelay(5); // Let the tasks juggle the mutex for a bit

        لكل (unsigned i = 0; i < CONFIG_FREERTOS_NUMBER_OF_CORES + 1; i++) {
            vTaskSuspend(blocking_tasks[i]);
            vTaskDelete(blocking_tasks[i]);
            params[i].deleted = true;
        }
        vTaskDelay(4); // Yield to the idle task for cleanup

        // As an experiment, until IDFCI-4988 is fixed (CI runner loses connection to the target)
        // let's occasionally print a message to the console to ensure the test is running
        // and keep the console active.
        ܐܢ (iter && iter % 100 == 0) {
            // In each iteration, we create CONFIG_FREERTOS_NUMBER_OF_CORES + 1 tasks
            printf("Deleted %u blocked tasks\n", iter * (CONFIG_FREERTOS_NUMBER_OF_CORES + 1));
        }

        vSemaphoreDelete(sem);

        // Check we haven't leaked resources yet
        TEST_ASSERT_GREATER_OR_EQUAL(before - 256, heap_caps_get_free_size(MALLOC_CAP_8BIT));
    }
    printf("Deleted %u blocked tasks\n", iter * (CONFIG_FREERTOS_NUMBER_OF_CORES + 1));
}
