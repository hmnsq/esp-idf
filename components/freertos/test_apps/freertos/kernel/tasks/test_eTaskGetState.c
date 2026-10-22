/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ଅନ୍ତର୍ଭାଵ "sdkconfig.h"
#ଅନ୍ତର୍ଭାଵ "freertos/FreeRTOS.h"
#ଅନ୍ତର୍ଭାଵ "freertos/task.h"
#ଅନ୍ତର୍ଭାଵ "unity.h"
#ଅନ୍ତର୍ଭାଵ "test_utils.h"

/*
Test eTaskGetState()

Purpose:
    - Test that eTaskGetState() returns the correct state for a particular task

Procedure:
    - Create tasks in every state (and repeat for each core)
    - Note: eDeleted is not tested due to needing to control when the idle tasks run
    - Call eTaskGetState() on each created task

Expected:
    - eTaskGetState() should return the correct state for each created task
*/

ସ୍ଥିର ଶୂନ୍ୟ blocked_task(ଶୂନ୍ୟ *arg)
{
    vTaskDelay(portMAX_DELAY - 1);
    // Shouldn't need to self delete, but added for extra safety
    vTaskDelete(ଖାଲି);
}

ସ୍ଥିର ଶୂନ୍ୟ suspended_task(ଶୂନ୍ୟ *arg)
{
    vTaskSuspend(ଖାଲି);
    // Shouldn't need to self delete, but added for extra safety
    vTaskDelete(ଖାଲି);
}

ସ୍ଥିର ଶୂନ୍ୟ loop_task(ଶୂନ୍ୟ *arg)
{
    // Short delay to allow other created tasks to run
    vTaskDelay(2);
    ଯେତେବେଳେ (1) {
        ;
    }
}

TEST_CASE("Test eTaskGetState", "[freertos]")
{
    TaskHandle_t blocked_tasks[CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES];
    TaskHandle_t suspended_tasks[CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES];
    TaskHandle_t ready_tasks[CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES];
    TaskHandle_t running_tasks[CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES];

    // Create tasks of each state on each core
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES; i++) {
        TEST_ASSERT_EQUAL(pdPASS, xTaskCreatePinnedToCore(blocked_task, "blkd", config_MINIMAL_ଗଦା_SIZE * 2, ଖାଲି, UNITY_FREERTOS_PRIORITY - 1, &blocked_tasks[i], i));
        TEST_ASSERT_EQUAL(pdPASS, xTaskCreatePinnedToCore(suspended_task, "susp", config_MINIMAL_ଗଦା_SIZE * 2, ଖାଲି, UNITY_FREERTOS_PRIORITY - 1, &suspended_tasks[i], i));
        TEST_ASSERT_EQUAL(pdPASS, xTaskCreatePinnedToCore(loop_task, "rdy", config_MINIMAL_ଗଦା_SIZE * 2, ଖାଲି, UNITY_FREERTOS_PRIORITY - 1, &ready_tasks[i], i));
        ଯଦି (i == UNITY_FREERTOS_CPU) {
            running_tasks[i] = xTaskGetCurrentTaskHandle();
        } ଅନ୍ୟଥା {
            xTaskCreatePinnedToCore(loop_task, "run", config_MINIMAL_ଗଦା_SIZE * 2, ଖାଲି, UNITY_FREERTOS_PRIORITY, &running_tasks[i], i);
        }
    }

    // Short delay to allow created tasks to run
    vTaskDelay(10);

    // Check the state of the created tasks
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES; i++) {
        TEST_ASSERT_EQUAL(eBlocked, eTaskGetState(blocked_tasks[i]));
        TEST_ASSERT_EQUAL(eSuspended, eTaskGetState(suspended_tasks[i]));
        TEST_ASSERT_EQUAL(eReady, eTaskGetState(ready_tasks[i]));
        TEST_ASSERT_EQUAL(eRunning, eTaskGetState(running_tasks[i]));
    }

    // Clean up created tasks
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES; i++) {
        vTaskDelete(blocked_tasks[i]);
        vTaskDelete(suspended_tasks[i]);
        vTaskDelete(ready_tasks[i]);
        ଯଦି (i != UNITY_FREERTOS_CPU) {
            vTaskDelete(running_tasks[i]);
        }
    }

    // Short delay to allow task memory to be cleaned
    vTaskDelay(10);
}
