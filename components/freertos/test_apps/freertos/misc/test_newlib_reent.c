/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Test for multicore FreeRTOS. This test spins up threads, fiddles with queues etc.
*/

#অন্তর্ভাব <esp_types.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব "sdkconfig.h"
#অন্তর্ভাব "freertos/FreeRTOS.h"
#অন্তর্ভাব "freertos/task.h"
#অন্তর্ভাব "freertos/semphr.h"
#অন্তর্ভাব "freertos/queue.h"
#অন্তর্ভাব "unity.h"
#অন্তর্ভাব "test_utils.h"

চঞ্চল স্থির পূর্ণাঙ্ক done;
চঞ্চল স্থির পূর্ণাঙ্ক error;

স্থির শূন্য tskTestRand(শূন্য *pvParameters)
{
    পূর্ণাঙ্ক l;
    srand(0x1234);
    vTaskDelay((পূর্ণাঙ্ক)pvParameters / portTICK_PERIOD_MS);
    l = rand();
    printf("Rand1: %d\n", l);
    যদি (l != 869320854) {
        error++;
    }
    vTaskDelay((পূর্ণাঙ্ক)pvParameters / portTICK_PERIOD_MS);
    l = rand();
    printf("Rand2: %d\n", l);
    যদি (l != 1148737841) {
        error++;
    }
    done++;
    vTaskDelete(রিক্ত);
}

// TODO: split this thing into separate orthogonal tests
TEST_CASE("Test for per-task non-reentrant tasks", "[freertos]")
{
    done = 0;
    error = 0;
    ধ্রুব uint32_t স্তূপ_size = 3072;
    xTaskCreatePinnedToCore(tskTestRand, "tsk1", স্তূপ_size, (শূন্য *)100, 3, রিক্ত, 0);
    xTaskCreatePinnedToCore(tskTestRand, "tsk2", স্তূপ_size, (শূন্য *)200, 3, রিক্ত, 0);
    xTaskCreatePinnedToCore(tskTestRand, "tsk3", স্তূপ_size, (শূন্য *)300, 3, রিক্ত, CONFIG_FREERTOS_সংখ্যা_OF_CORES - 1);
    xTaskCreatePinnedToCore(tskTestRand, "tsk4", স্তূপ_size, (শূন্য *)400, 3, রিক্ত, 0);
    যাবত্ (done != 4) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    TEST_ASSERT(error == 0);
}
