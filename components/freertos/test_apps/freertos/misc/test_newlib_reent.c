/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Test for multicore FreeRTOS. This test spins up threads, fiddles with queues etc.
*/

#அந்தர்பாவ <esp_types.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ "sdkconfig.h"
#அந்தர்பாவ "freertos/FreeRTOS.h"
#அந்தர்பாவ "freertos/task.h"
#அந்தர்பாவ "freertos/semphr.h"
#அந்தர்பாவ "freertos/queue.h"
#அந்தர்பாவ "unity.h"
#அந்தர்பாவ "test_utils.h"

நிலையற்ற நிலையான முழு done;
நிலையற்ற நிலையான முழு error;

நிலையான வெறுமை tskTestRand(வெறுமை *pvParameters)
{
    முழு l;
    srand(0x1234);
    vTaskDelay((முழு)pvParameters / portTICK_PERIOD_MS);
    l = rand();
    printf("Rand1: %d\n", l);
    எனில் (l != 869320854) {
        error++;
    }
    vTaskDelay((முழு)pvParameters / portTICK_PERIOD_MS);
    l = rand();
    printf("Rand2: %d\n", l);
    எனில் (l != 1148737841) {
        error++;
    }
    done++;
    vTaskDelete(வெற்று);
}

// TODO: split this thing into separate orthogonal tests
TEST_CASE("Test for per-task non-reentrant tasks", "[freertos]")
{
    done = 0;
    error = 0;
    மாறிலி uint32_t அடுக்கு_size = 3072;
    xTaskCreatePinnedToCore(tskTestRand, "tsk1", அடுக்கு_size, (வெறுமை *)100, 3, வெற்று, 0);
    xTaskCreatePinnedToCore(tskTestRand, "tsk2", அடுக்கு_size, (வெறுமை *)200, 3, வெற்று, 0);
    xTaskCreatePinnedToCore(tskTestRand, "tsk3", அடுக்கு_size, (வெறுமை *)300, 3, வெற்று, CONFIG_FREERTOS_எண்_OF_CORES - 1);
    xTaskCreatePinnedToCore(tskTestRand, "tsk4", அடுக்கு_size, (வெறுமை *)400, 3, வெற்று, 0);
    போது (done != 4) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    TEST_ASSERT(error == 0);
}
