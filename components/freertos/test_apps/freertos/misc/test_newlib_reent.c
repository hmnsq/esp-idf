/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Test for multicore FreeRTOS. This test spins up threads, fiddles with queues etc.
*/

#અન્તર્ભાવ <esp_types.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ "sdkconfig.h"
#અન્તર્ભાવ "freertos/FreeRTOS.h"
#અન્તર્ભાવ "freertos/task.h"
#અન્તર્ભાવ "freertos/semphr.h"
#અન્તર્ભાવ "freertos/queue.h"
#અન્તર્ભાવ "unity.h"
#અન્તર્ભાવ "test_utils.h"

અસ્થિર સ્થિર પૂર્ણાંક done;
અસ્થિર સ્થિર પૂર્ણાંક error;

સ્થિર શૂન્ય tskTestRand(શૂન્ય *pvParameters)
{
    પૂર્ણાંક l;
    srand(0x1234);
    vTaskDelay((પૂર્ણાંક)pvParameters / portTICK_PERIOD_MS);
    l = rand();
    printf("Rand1: %d\n", l);
    જો (l != 869320854) {
        error++;
    }
    vTaskDelay((પૂર્ણાંક)pvParameters / portTICK_PERIOD_MS);
    l = rand();
    printf("Rand2: %d\n", l);
    જો (l != 1148737841) {
        error++;
    }
    done++;
    vTaskDelete(ખાલી);
}

// TODO: split this thing into separate orthogonal tests
TEST_CASE("Test for per-task non-reentrant tasks", "[freertos]")
{
    done = 0;
    error = 0;
    અચલ uint32_t ઢગલો_size = 3072;
    xTaskCreatePinnedToCore(tskTestRand, "tsk1", ઢગલો_size, (શૂન્ય *)100, 3, ખાલી, 0);
    xTaskCreatePinnedToCore(tskTestRand, "tsk2", ઢગલો_size, (શૂન્ય *)200, 3, ખાલી, 0);
    xTaskCreatePinnedToCore(tskTestRand, "tsk3", ઢગલો_size, (શૂન્ય *)300, 3, ખાલી, CONFIG_FREERTOS_સંખ્યા_OF_CORES - 1);
    xTaskCreatePinnedToCore(tskTestRand, "tsk4", ઢગલો_size, (શૂન્ય *)400, 3, ખાલી, 0);
    જ્યારે (done != 4) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    TEST_ASSERT(error == 0);
}
