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
#अन्तर्भाव <stdio.h>
#अन्तर्भाव "sdkconfig.h"
#अन्तर्भाव "freertos/FreeRTOS.h"
#अन्तर्भाव "freertos/task.h"
#अन्तर्भाव "freertos/semphr.h"
#अन्तर्भाव "esp_heap_caps.h"

#अन्तर्भाव "unity.h"
#अन्तर्भाव "test_utils.h"

#अन्तर्भाव "esp_rom_sys.h"

#परिभाषय NO_सम्बन्धि_TSKS  3
#परिभाषय DELAY_TICKS 2
/* Caps of all memory which is allocated from when a task is created */
#परिभाषय HEAP_CAPS   (MALLOC_CAP_आन्तरिकम् | MALLOC_CAP_8BIT)

#परिभाषय DELAY_US_ITERATIONS 1000

स्थिर शून्य tsk_self_del(शून्य *param)
{
    vTaskDelete(रिक्त);  //Deleting self means deleting currently running task
}

स्थिर शून्य tsk_extern_del(शून्य *param)
{
    vTaskDelay(पत्तनम्_MAX_DELAY);  //Await external deletion
}

स्थिर शून्य tsk_self_del_us_delay(शून्य *param)
{
    uint32_t delay = (uint32_t)param;
    esp_rom_delay_us(delay);
    vTaskDelete(रिक्त);
}

TEST_CASE("FreeRTOS Delete Tasks", "[freertos]")
{
    /* -------------- Test vTaskDelete() on currently running tasks ----------------*/
    uint32_t before_गणनम् = ux_Task_Get_सङ्ख्या_सम्बन्धि_Tasks();
    uint32_t before_heap = heap_caps_get_free_आकार(HEAP_CAPS);
    प्रति (पूर्णाङ्क i = 0; i < CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES; i++) {
        प्रति (पूर्णाङ्क j = 0; j < NO_सम्बन्धि_TSKS; j++) {
            TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_गन्तव्यम्_Core(tsk_self_del, "tsk_self", 1024, रिक्त, configMAX_PRIORITIES - 1, रिक्त, i));
        }
    }
    vTaskDelay(DELAY_TICKS);    //Minimal delay to see if Idle task cleans up all tasks awaiting deletion in a single tick
    TEST_ASSERT_EQUAL(before_गणनम्, ux_Task_Get_सङ्ख्या_सम्बन्धि_Tasks());
    TEST_ASSERT_EQUAL(before_heap, heap_caps_get_free_आकार(HEAP_CAPS));

    /* ------------- Test vTaskDelete() on not currently running tasks ------------ */
    TaskHandle_t handles[NO_सम्बन्धि_TSKS];
    before_heap = heap_caps_get_free_आकार(HEAP_CAPS);
    //Create task pinned to the same core that will not run during task deletion
    प्रति (पूर्णाङ्क j = 0 ; j < NO_सम्बन्धि_TSKS; j++) {
        TEST_ASSERT_EQUAL(pdTRUE, x_Task_Create_Pinned_गन्तव्यम्_Core(tsk_extern_del, "tsk_extern", 4096, रिक्त, configMAX_PRIORITIES - 1, &handles[j], x_पत्तनम्_Get_Core_ID()));
    }
    TEST_ASSERT_NOT_EQUAL(before_heap, heap_caps_get_free_आकार(HEAP_CAPS));    //Check tasks have been created
    //Delete the tasks, memory should be freed immediately
    प्रति (पूर्णाङ्क j = 0; j < NO_सम्बन्धि_TSKS; j++) {
        vTaskDelete(handles[j]);
    }
    TEST_ASSERT_EQUAL(before_heap, heap_caps_get_free_आकार(HEAP_CAPS));

    /* Test self deleting no affinity task is not removed by idle task of other core before context switch */
    प्रति (पूर्णाङ्क i = 0; i < DELAY_US_ITERATIONS; i += 10) {
        vTaskDelay(1);                          //Sync to next tick interrupt
        x_Task_Create_Pinned_गन्तव्यम्_Core(tsk_self_del_us_delay, "delay", 1024, (शून्य *)i, UNITY_FREERTOS_प्राथमिकता - 1, रिक्त, tskNO_AFFINITY);
        esp_rom_delay_us(10);                       //Busy wait to ensure no affinity task runs on opposite core
    }

}

प्रकारनिर्देश संरचना {
    SemaphoreHandle_t sem;
    चञ्चल bool deleted; // Check the deleted task doesn't keep running after being deleted
} tsk_खण्डानि_param_t;

/* Task blocks as often as possible
   (two or more of these can share the same semaphore and "juggle" it around)
*/
स्थिर शून्य tsk_खण्डानि_frequently(शून्य *param)
{
    tsk_खण्डानि_param_t *p = (tsk_खण्डानि_param_t *)param;
    SemaphoreHandle_t sem = p->sem;
    बीजं_रोपय(x_Task_Get_Tick_गणनम्() ^ (पूर्णाङ्क)xTaskGetCurrentTaskHandle());
    यावत् (1) {
        प्रतिज्ञा(!p->deleted);
        esp_rom_delay_us(यादृच्छिकम्() % 10);
        प्रतिज्ञा(!p->deleted);
        xSemaphoreTake(sem, पत्तनम्_MAX_DELAY);
        प्रतिज्ञा(!p->deleted);
        esp_rom_delay_us(यादृच्छिकम्() % 10);
        प्रतिज्ञा(!p->deleted);
        xSemaphoreGive(sem);
    }
}

TEST_CASE("FreeRTOS Delete Blocked Tasks", "[freertos]")
{
    TaskHandle_t blocking_tasks[CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES + 1]; // one per CPU, plus one unpinned task
    tsk_खण्डानि_param_t params[CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES + 1] = { 0 };

    अचिह्नित before = heap_caps_get_free_आकार(MALLOC_CAP_8BIT);
    प्रारूप_लिख("Free memory at start %u\n", before);

    /* Any bugs will depend on relative timing of destroying the tasks, so create & delete many times.

       Stop early if it looks like some resources have not been properly cleaned up.

       (1000 iterations takes about 9 seconds on ESP32 dual core)
     */
    अचिह्नित iter = 0;
    प्रति (iter = 0; iter < 1000; iter++) {
        // Create everything
        SemaphoreHandle_t sem = xSemaphoreCreateMutex();
        प्रति (अचिह्नित i = 0; i < CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES + 1; i++) {
            params[i].deleted = false;
            params[i].sem = sem;

            TEST_ASSERT_EQUAL(pdTRUE,
                              x_Task_Create_Pinned_गन्तव्यम्_Core(tsk_खण्डानि_frequently, "tsk_block", 4096, &params[i],
                                                      UNITY_FREERTOS_प्राथमिकता - 1, &blocking_tasks[i],
                                                      i < CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES ? i : tskNO_AFFINITY));
        }

        vTaskDelay(5); // Let the tasks juggle the mutex for a bit

        प्रति (अचिह्नित i = 0; i < CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES + 1; i++) {
            vTaskSuspend(blocking_tasks[i]);
            vTaskDelete(blocking_tasks[i]);
            params[i].deleted = true;
        }
        vTaskDelay(4); // Yield to the idle task for cleanup

        // As an experiment, until IDFCI-4988 is fixed (CI runner loses connection to the target)
        // let's occasionally print a message to the console to ensure the test is running
        // and keep the console active.
        यदि (iter && iter % 100 == 0) {
            // In each iteration, we create CONFIG_FREERTOS_NUMBER_OF_CORES + 1 tasks
            प्रारूप_लिख("Deleted %u blocked tasks\n", iter * (CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES + 1));
        }

        vSemaphoreDelete(sem);

        // Check we haven't leaked resources yet
        TEST_ASSERT_GREATER_OR_EQUAL(before - 256, heap_caps_get_free_आकार(MALLOC_CAP_8BIT));
    }
    प्रारूप_लिख("Deleted %u blocked tasks\n", iter * (CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES + 1));
}
