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
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ "freertos/semphr.h"
#ಅನ್ತರ್ಭಾವ "esp_heap_caps.h"

#ಅನ್ತರ್ಭಾವ "unity.h"
#ಅನ್ತರ್ಭಾವ "test_utils.h"

#ಅನ್ತರ್ಭಾವ "esp_rom_sys.h"

#define NO_OF_TSKS  3
#define DELAY_TICKS 2
/* Caps of all memory which is allocated from when a task is created */
#define HEAP_CAPS   (MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT)

#define DELAY_US_ITERATIONS 1000

ಸ್ಥಿರ ಶೂನ್ಯ tsk_self_del(ಶೂನ್ಯ *param)
{
    vTaskDelete(ರಿಕ್ತ);  //Deleting self means deleting currently running task
}

ಸ್ಥಿರ ಶೂನ್ಯ tsk_extern_del(ಶೂನ್ಯ *param)
{
    vTaskDelay(portMAX_DELAY);  //Await external deletion
}

ಸ್ಥಿರ ಶೂನ್ಯ tsk_self_del_us_delay(ಶೂನ್ಯ *param)
{
    uint32_t delay = (uint32_t)param;
    esp_rom_delay_us(delay);
    vTaskDelete(ರಿಕ್ತ);
}

TEST_CASE("FreeRTOS Delete Tasks", "[freertos]")
{
    /* -------------- Test vTaskDelete() on currently running tasks ----------------*/
    uint32_t before_ಎಣಿಕೆ = ux_Task_Get_ಸಂಖ್ಯೆ_Of_Tasks();
    uint32_t before_heap = heap_caps_get_free_size(HEAP_CAPS);
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES; i++) {
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < NO_OF_TSKS; j++) {
            TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(tsk_self_del, "tsk_self", 1024, ರಿಕ್ತ, configMAX_PRIORITIES - 1, ರಿಕ್ತ, i));
        }
    }
    vTaskDelay(DELAY_TICKS);    //Minimal delay to see if Idle task cleans up all tasks awaiting deletion in a single tick
    TEST_ASSERT_EQUAL(before_ಎಣಿಕೆ, ux_Task_Get_ಸಂಖ್ಯೆ_Of_Tasks());
    TEST_ASSERT_EQUAL(before_heap, heap_caps_get_free_size(HEAP_CAPS));

    /* ------------- Test vTaskDelete() on not currently running tasks ------------ */
    TaskHandle_t handles[NO_OF_TSKS];
    before_heap = heap_caps_get_free_size(HEAP_CAPS);
    //Create task pinned to the same core that will not run during task deletion
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0 ; j < NO_OF_TSKS; j++) {
        TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(tsk_extern_del, "tsk_extern", 4096, ರಿಕ್ತ, configMAX_PRIORITIES - 1, &handles[j], xPortGetCoreID()));
    }
    TEST_ASSERT_NOT_EQUAL(before_heap, heap_caps_get_free_size(HEAP_CAPS));    //Check tasks have been created
    //Delete the tasks, memory should be freed immediately
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ j = 0; j < NO_OF_TSKS; j++) {
        vTaskDelete(handles[j]);
    }
    TEST_ASSERT_EQUAL(before_heap, heap_caps_get_free_size(HEAP_CAPS));

    /* Test self deleting no affinity task is not removed by idle task of other core before context switch */
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < DELAY_US_ITERATIONS; i += 10) {
        vTaskDelay(1);                          //Sync to next tick interrupt
        xTaskCreatePinnedToCore(tsk_self_del_us_delay, "delay", 1024, (ಶೂನ್ಯ *)i, UNITY_FREERTOS_PRIORITY - 1, ರಿಕ್ತ, tskNO_AFFINITY);
        esp_rom_delay_us(10);                       //Busy wait to ensure no affinity task runs on opposite core
    }

}

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    SemaphoreHandle_t sem;
    ಚಞ್ಚಲ bool deleted; // Check the deleted task doesn't keep running after being deleted
} tsk_blocks_param_t;

/* Task blocks as often as possible
   (two or more of these can share the same semaphore and "juggle" it around)
*/
ಸ್ಥಿರ ಶೂನ್ಯ tsk_blocks_frequently(ಶೂನ್ಯ *param)
{
    tsk_blocks_param_t *p = (tsk_blocks_param_t *)param;
    SemaphoreHandle_t sem = p->sem;
    srand(x_Task_Get_Tick_ಎಣಿಕೆ() ^ (ಪೂರ್ಣಾಙ್ಕ)xTaskGetCurrentTaskHandle());
    ಯಾವತ್ (1) {
        ಪ್ರತಿಜ್ಞಾ(!p->deleted);
        esp_rom_delay_us(rand() % 10);
        ಪ್ರತಿಜ್ಞಾ(!p->deleted);
        xSemaphoreTake(sem, portMAX_DELAY);
        ಪ್ರತಿಜ್ಞಾ(!p->deleted);
        esp_rom_delay_us(rand() % 10);
        ಪ್ರತಿಜ್ಞಾ(!p->deleted);
        xSemaphoreGive(sem);
    }
}

TEST_CASE("FreeRTOS Delete Blocked Tasks", "[freertos]")
{
    TaskHandle_t blocking_tasks[CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES + 1]; // one per CPU, plus one unpinned task
    tsk_blocks_param_t params[CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES + 1] = { 0 };

    unsigned before = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    printf("Free memory at start %u\n", before);

    /* Any bugs will depend on relative timing of destroying the tasks, so create & delete many times.

       Stop early if it looks like some resources have not been properly cleaned up.

       (1000 iterations takes about 9 seconds on ESP32 dual core)
     */
    unsigned iter = 0;
    ಪ್ರತಿ (iter = 0; iter < 1000; iter++) {
        // Create everything
        SemaphoreHandle_t sem = xSemaphoreCreateMutex();
        ಪ್ರತಿ (unsigned i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES + 1; i++) {
            params[i].deleted = false;
            params[i].sem = sem;

            TEST_ASSERT_EQUAL(pdTRUE,
                              xTaskCreatePinnedToCore(tsk_blocks_frequently, "tsk_block", 4096, &params[i],
                                                      UNITY_FREERTOS_PRIORITY - 1, &blocking_tasks[i],
                                                      i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES ? i : tskNO_AFFINITY));
        }

        vTaskDelay(5); // Let the tasks juggle the mutex for a bit

        ಪ್ರತಿ (unsigned i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES + 1; i++) {
            vTaskSuspend(blocking_tasks[i]);
            vTaskDelete(blocking_tasks[i]);
            params[i].deleted = true;
        }
        vTaskDelay(4); // Yield to the idle task for cleanup

        // As an experiment, until IDFCI-4988 is fixed (CI runner loses connection to the target)
        // let's occasionally print a message to the console to ensure the test is running
        // and keep the console active.
        ಯದಿ (iter && iter % 100 == 0) {
            // In each iteration, we create CONFIG_FREERTOS_NUMBER_OF_CORES + 1 tasks
            printf("Deleted %u blocked tasks\n", iter * (CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES + 1));
        }

        vSemaphoreDelete(sem);

        // Check we haven't leaked resources yet
        TEST_ASSERT_GREATER_OR_EQUAL(before - 256, heap_caps_get_free_size(MALLOC_CAP_8BIT));
    }
    printf("Deleted %u blocked tasks\n", iter * (CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES + 1));
}
