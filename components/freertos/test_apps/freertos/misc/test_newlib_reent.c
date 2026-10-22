/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Test for multicore FreeRTOS. This test spins up threads, fiddles with queues etc.
*/

#شامل <esp_types.h>
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل "sdkconfig.h"
#شامل "freertos/FreeRTOS.h"
#شامل "freertos/task.h"
#شامل "freertos/semphr.h"
#شامل "freertos/queue.h"
#شامل "unity.h"
#شامل "test_utils.h"

فرار ایستا صحیح done;
فرار ایستا صحیح error;

ایستا تهی tskTestRand(تهی *pvParameters)
{
    صحیح l;
    srand(0x1234);
    vTaskDelay((صحیح)pvParameters / portTICK_PERIOD_MS);
    l = rand();
    printf("Rand1: %d\n", l);
    اگر (l != 869320854) {
        error++;
    }
    vTaskDelay((صحیح)pvParameters / portTICK_PERIOD_MS);
    l = rand();
    printf("Rand2: %d\n", l);
    اگر (l != 1148737841) {
        error++;
    }
    done++;
    vTaskDelete(پوچ);
}

// TODO: split this thing into separate orthogonal tests
TEST_CASE("Test for per-task non-reentrant tasks", "[freertos]")
{
    done = 0;
    error = 0;
    ثابت uint32_t پشته_size = 3072;
    xTaskCreatePinnedToCore(tskTestRand, "tsk1", پشته_size, (تهی *)100, 3, پوچ, 0);
    xTaskCreatePinnedToCore(tskTestRand, "tsk2", پشته_size, (تهی *)200, 3, پوچ, 0);
    xTaskCreatePinnedToCore(tskTestRand, "tsk3", پشته_size, (تهی *)300, 3, پوچ, CONFIG_FREERTOS_عدد_OF_CORES - 1);
    xTaskCreatePinnedToCore(tskTestRand, "tsk4", پشته_size, (تهی *)400, 3, پوچ, 0);
    وقتی (done != 4) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    TEST_ASSERT(error == 0);
}
