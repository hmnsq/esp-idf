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

#ವ್ಯಾಖ್ಯಾನಿಸು NO_ದ_TSKS  3
#ವ್ಯಾಖ್ಯಾನಿಸು DELAY_TICKS 2
/* Caps of all memory which is allocated from when a task is created */
#ವ್ಯಾಖ್ಯಾನಿಸು ರಾಶಿ_CAPS   (MALLOC_CAP_ಆಂತರಿಕ | MALLOC_CAP_8_ಬಿಟ್)

#ವ್ಯಾಖ್ಯಾನಿಸು DELAY_US_ITERATIONS 1000

ಸ್ಥಿರ ಶೂನ್ಯ tsk_self_del(ಶೂನ್ಯ *param)
{
    vTaskDelete(ಖಾಲಿ);  //Deleting self means deleting currently running task
}

ಸ್ಥಿರ ಶೂನ್ಯ tsk_extern_del(ಶೂನ್ಯ *param)
{
    vTaskDelay(ಪೋರ್ಟ್_MAX_DELAY);  //Await external deletion
}

ಸ್ಥಿರ ಶೂನ್ಯ tsk_self_del_us_delay(ಶೂನ್ಯ *param)
{
    uint32_t delay = (uint32_t)param;
    esp_rom_delay_us(delay);
    vTaskDelete(ಖಾಲಿ);
}

TEST_CASE("FreeRTOS Delete Tasks", "[freertos]")
{
    /* -------------- Test vTaskDelete() on currently running tasks ----------------*/
    uint32_t before_ಎಣಿಕೆ = ux_Task_Get_ಸಂಖ್ಯೆ_ದ_Tasks();
    uint32_t before_ರಾಶಿ = ರಾಶಿ_caps_get_free_ಗಾತ್ರ(ರಾಶಿ_CAPS);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; i++) {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < NO_ದ_TSKS; j++) {
            TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_ಗಮ್ಯ_Core(tsk_self_del, "tsk_self", 1024, ಖಾಲಿ, configMAX_PRIORITIES - 1, ಖಾಲಿ, i));
        }
    }
    vTaskDelay(DELAY_TICKS);    //Minimal delay to see if Idle task cleans up all tasks awaiting deletion in a single tick
    TEST_ASSERT_EQUAL(before_ಎಣಿಕೆ, ux_Task_Get_ಸಂಖ್ಯೆ_ದ_Tasks());
    TEST_ASSERT_EQUAL(before_ರಾಶಿ, ರಾಶಿ_caps_get_free_ಗಾತ್ರ(ರಾಶಿ_CAPS));

    /* ------------- Test vTaskDelete() on not currently running tasks ------------ */
    TaskHandle_t handles[NO_ದ_TSKS];
    before_ರಾಶಿ = ರಾಶಿ_caps_get_free_ಗಾತ್ರ(ರಾಶಿ_CAPS);
    //Create task pinned to the same core that will not run during task deletion
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0 ; j < NO_ದ_TSKS; j++) {
        TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_ಗಮ್ಯ_Core(tsk_extern_del, "tsk_extern", 4096, ಖಾಲಿ, configMAX_PRIORITIES - 1, &handles[j], x_ಪೋರ್ಟ್_Get_Core_ID()));
    }
    TEST_ASSERT_NOT_EQUAL(before_ರಾಶಿ, ರಾಶಿ_caps_get_free_ಗಾತ್ರ(ರಾಶಿ_CAPS));    //Check tasks have been created
    //Delete the tasks, memory should be freed immediately
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < NO_ದ_TSKS; j++) {
        vTaskDelete(handles[j]);
    }
    TEST_ASSERT_EQUAL(before_ರಾಶಿ, ರಾಶಿ_caps_get_free_ಗಾತ್ರ(ರಾಶಿ_CAPS));

    /* Test self deleting no affinity task is not removed by idle task of other core before context switch */
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < DELAY_US_ITERATIONS; i += 10) {
        vTaskDelay(1);                          //Sync to next tick interrupt
        x_Task_Create_Pinned_ಗಮ್ಯ_Core(tsk_self_del_us_delay, "delay", 1024, (ಶೂನ್ಯ *)i, UNITY_FREERTOS_ಆದ್ಯತೆ - 1, ಖಾಲಿ, tskNO_AFFINITY);
        esp_rom_delay_us(10);                       //Busy wait to ensure no affinity task runs on opposite core
    }

}

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    SemaphoreHandle_t sem;
    ಅಸ್ಥಿರ bool deleted; // Check the deleted task doesn't keep running after being deleted
} tsk_ಖಂಡಗಳು_param_t;

/* Task blocks as often as possible
   (two or more of these can share the same semaphore and "juggle" it around)
*/
ಸ್ಥಿರ ಶೂನ್ಯ tsk_ಖಂಡಗಳು_frequently(ಶೂನ್ಯ *param)
{
    tsk_ಖಂಡಗಳು_param_t *p = (tsk_ಖಂಡಗಳು_param_t *)param;
    SemaphoreHandle_t sem = p->sem;
    srand(x_Task_Get_Tick_ಎಣಿಕೆ() ^ (ಪೂರ್ಣಾಂಕ)xTaskGetCurrentTaskHandle());
    ಆಗ (1) {
        ಪ್ರತಿಜ್ಞಾ(!p->deleted);
        esp_rom_delay_us(rand() % 10);
        ಪ್ರತಿಜ್ಞಾ(!p->deleted);
        xSemaphoreTake(sem, ಪೋರ್ಟ್_MAX_DELAY);
        ಪ್ರತಿಜ್ಞಾ(!p->deleted);
        esp_rom_delay_us(rand() % 10);
        ಪ್ರತಿಜ್ಞಾ(!p->deleted);
        xSemaphoreGive(sem);
    }
}

TEST_CASE("FreeRTOS Delete Blocked Tasks", "[freertos]")
{
    TaskHandle_t blocking_tasks[CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES + 1]; // one per CPU, plus one unpinned task
    tsk_ಖಂಡಗಳು_param_t params[CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES + 1] = { 0 };

    ಚಿಹ್ನೆರಹಿತ before = ರಾಶಿ_caps_get_free_ಗಾತ್ರ(MALLOC_CAP_8_ಬಿಟ್);
    ಬರೆ("Free memory at start %u\n", before);

    /* Any bugs will depend on relative timing of destroying the tasks, so create & delete many times.

       Stop early if it looks like some resources have not been properly cleaned up.

       (1000 iterations takes about 9 seconds on ESP32 dual core)
     */
    ಚಿಹ್ನೆರಹಿತ iter = 0;
    ಗಾಗಿ (iter = 0; iter < 1000; iter++) {
        // Create everything
        SemaphoreHandle_t sem = xSemaphoreCreateMutex();
        ಗಾಗಿ (ಚಿಹ್ನೆರಹಿತ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES + 1; i++) {
            params[i].deleted = false;
            params[i].sem = sem;

            TEST_ASSERT_EQUAL(pdTRUE,
                              x_Task_Create_Pinned_ಗಮ್ಯ_Core(tsk_ಖಂಡಗಳು_frequently, "tsk_block", 4096, &params[i],
                                                      UNITY_FREERTOS_ಆದ್ಯತೆ - 1, &blocking_tasks[i],
                                                      i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES ? i : tskNO_AFFINITY));
        }

        vTaskDelay(5); // Let the tasks juggle the mutex for a bit

        ಗಾಗಿ (ಚಿಹ್ನೆರಹಿತ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES + 1; i++) {
            vTaskSuspend(blocking_tasks[i]);
            vTaskDelete(blocking_tasks[i]);
            params[i].deleted = true;
        }
        vTaskDelay(4); // Yield to the idle task for cleanup

        // As an experiment, until IDFCI-4988 is fixed (CI runner loses connection to the target)
        // let's occasionally print a message to the console to ensure the test is running
        // and keep the console active.
        ಆದರೆ (iter && iter % 100 == 0) {
            // In each iteration, we create CONFIG_FREERTOS_NUMBER_OF_CORES + 1 tasks
            ಬರೆ("Deleted %u blocked tasks\n", iter * (CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES + 1));
        }

        vSemaphoreDelete(sem);

        // Check we haven't leaked resources yet
        TEST_ASSERT_GREATER_OR_EQUAL(before - 256, ರಾಶಿ_caps_get_free_ಗಾತ್ರ(MALLOC_CAP_8_ಬಿಟ್));
    }
    ಬರೆ("Deleted %u blocked tasks\n", iter * (CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES + 1));
}
