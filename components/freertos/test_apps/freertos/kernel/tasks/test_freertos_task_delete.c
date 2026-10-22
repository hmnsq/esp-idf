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
#समावेश <stdio.h>
#समावेश "sdkconfig.h"
#समावेश "freertos/FreeRTOS.h"
#समावेश "freertos/task.h"
#समावेश "freertos/semphr.h"
#समावेश "esp_heap_caps.h"

#समावेश "unity.h"
#समावेश "test_utils.h"

#समावेश "esp_rom_sys.h"

#घोषणा NO_OF_TSKS  3
#घोषणा DELAY_TICKS 2
/* Caps of all memory which is allocated from when a task is created */
#घोषणा HEAP_CAPS   (MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT)

#घोषणा DELAY_US_ITERATIONS 1000

अटल व्योम tsk_self_del(व्योम *param)
{
    vTaskDelete(शून्य);  //Deleting self means deleting currently running task
}

अटल व्योम tsk_extern_del(व्योम *param)
{
    vTaskDelay(portMAX_DELAY);  //Await external deletion
}

अटल व्योम tsk_self_del_us_delay(व्योम *param)
{
    uint32_t delay = (uint32_t)param;
    esp_rom_delay_us(delay);
    vTaskDelete(शून्य);
}

TEST_CASE("FreeRTOS Delete Tasks", "[freertos]")
{
    /* -------------- Test vTaskDelete() on currently running tasks ----------------*/
    uint32_t before_गिनती = ux_Task_Get_संख्या_Of_Tasks();
    uint32_t before_heap = heap_caps_get_free_आकार(HEAP_CAPS);
    क्रम (पूर्णांक i = 0; i < CONFIG_FREERTOS_संख्या_OF_CORES; i++) {
        क्रम (पूर्णांक j = 0; j < NO_OF_TSKS; j++) {
            TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(tsk_self_del, "tsk_self", 1024, शून्य, configMAX_PRIORITIES - 1, शून्य, i));
        }
    }
    vTaskDelay(DELAY_TICKS);    //Minimal delay to see if Idle task cleans up all tasks awaiting deletion in a single tick
    TEST_ASSERT_EQUAL(before_गिनती, ux_Task_Get_संख्या_Of_Tasks());
    TEST_ASSERT_EQUAL(before_heap, heap_caps_get_free_आकार(HEAP_CAPS));

    /* ------------- Test vTaskDelete() on not currently running tasks ------------ */
    TaskHandle_t handles[NO_OF_TSKS];
    before_heap = heap_caps_get_free_आकार(HEAP_CAPS);
    //Create task pinned to the same core that will not run during task deletion
    क्रम (पूर्णांक j = 0 ; j < NO_OF_TSKS; j++) {
        TEST_ASSERT_EQUAL(pdTRUE, xTaskCreatePinnedToCore(tsk_extern_del, "tsk_extern", 4096, शून्य, configMAX_PRIORITIES - 1, &handles[j], xPortGetCoreID()));
    }
    TEST_ASSERT_NOT_EQUAL(before_heap, heap_caps_get_free_आकार(HEAP_CAPS));    //Check tasks have been created
    //Delete the tasks, memory should be freed immediately
    क्रम (पूर्णांक j = 0; j < NO_OF_TSKS; j++) {
        vTaskDelete(handles[j]);
    }
    TEST_ASSERT_EQUAL(before_heap, heap_caps_get_free_आकार(HEAP_CAPS));

    /* Test self deleting no affinity task is not removed by idle task of other core before context switch */
    क्रम (पूर्णांक i = 0; i < DELAY_US_ITERATIONS; i += 10) {
        vTaskDelay(1);                          //Sync to next tick interrupt
        xTaskCreatePinnedToCore(tsk_self_del_us_delay, "delay", 1024, (व्योम *)i, UNITY_FREERTOS_PRIORITY - 1, शून्य, tskNO_AFFINITY);
        esp_rom_delay_us(10);                       //Busy wait to ensure no affinity task runs on opposite core
    }

}

प्रकार काष्ठा {
    SemaphoreHandle_t sem;
    अस्थिर bool deleted; // Check the deleted task doesn't keep running after being deleted
} tsk_blocks_param_t;

/* Task blocks as often as possible
   (two or more of these can share the same semaphore and "juggle" it around)
*/
अटल व्योम tsk_blocks_frequently(व्योम *param)
{
    tsk_blocks_param_t *p = (tsk_blocks_param_t *)param;
    SemaphoreHandle_t sem = p->sem;
    बेक्रम(x_Task_Get_Tick_गिनती() ^ (पूर्णांक)xTaskGetCurrentTaskHandle());
    जबतक (1) {
        निश्चित(!p->deleted);
        esp_rom_delay_us(अक्रम() % 10);
        निश्चित(!p->deleted);
        xSemaphoreTake(sem, portMAX_DELAY);
        निश्चित(!p->deleted);
        esp_rom_delay_us(अक्रम() % 10);
        निश्चित(!p->deleted);
        xSemaphoreGive(sem);
    }
}

TEST_CASE("FreeRTOS Delete Blocked Tasks", "[freertos]")
{
    TaskHandle_t blocking_tasks[CONFIG_FREERTOS_संख्या_OF_CORES + 1]; // one per CPU, plus one unpinned task
    tsk_blocks_param_t params[CONFIG_FREERTOS_संख्या_OF_CORES + 1] = { 0 };

    अचिन्हित before = heap_caps_get_free_आकार(MALLOC_CAP_8BIT);
    म_लिखो("Free memory at start %u\n", before);

    /* Any bugs will depend on relative timing of destroying the tasks, so create & delete many times.

       Stop early if it looks like some resources have not been properly cleaned up.

       (1000 iterations takes about 9 seconds on ESP32 dual core)
     */
    अचिन्हित iter = 0;
    क्रम (iter = 0; iter < 1000; iter++) {
        // Create everything
        SemaphoreHandle_t sem = xSemaphoreCreateMutex();
        क्रम (अचिन्हित i = 0; i < CONFIG_FREERTOS_संख्या_OF_CORES + 1; i++) {
            params[i].deleted = false;
            params[i].sem = sem;

            TEST_ASSERT_EQUAL(pdTRUE,
                              xTaskCreatePinnedToCore(tsk_blocks_frequently, "tsk_block", 4096, &params[i],
                                                      UNITY_FREERTOS_PRIORITY - 1, &blocking_tasks[i],
                                                      i < CONFIG_FREERTOS_संख्या_OF_CORES ? i : tskNO_AFFINITY));
        }

        vTaskDelay(5); // Let the tasks juggle the mutex for a bit

        क्रम (अचिन्हित i = 0; i < CONFIG_FREERTOS_संख्या_OF_CORES + 1; i++) {
            vTaskSuspend(blocking_tasks[i]);
            vTaskDelete(blocking_tasks[i]);
            params[i].deleted = true;
        }
        vTaskDelay(4); // Yield to the idle task for cleanup

        // As an experiment, until IDFCI-4988 is fixed (CI runner loses connection to the target)
        // let's occasionally print a message to the console to ensure the test is running
        // and keep the console active.
        अगर (iter && iter % 100 == 0) {
            // In each iteration, we create CONFIG_FREERTOS_NUMBER_OF_CORES + 1 tasks
            म_लिखो("Deleted %u blocked tasks\n", iter * (CONFIG_FREERTOS_संख्या_OF_CORES + 1));
        }

        vSemaphoreDelete(sem);

        // Check we haven't leaked resources yet
        TEST_ASSERT_GREATER_OR_EQUAL(before - 256, heap_caps_get_free_आकार(MALLOC_CAP_8BIT));
    }
    म_लिखो("Deleted %u blocked tasks\n", iter * (CONFIG_FREERTOS_संख्या_OF_CORES + 1));
}
