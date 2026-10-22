/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Test for multicore FreeRTOS. This test spins up threads, fiddles with queues etc.
*/

#ضمن <esp_types.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن "sdkconfig.h"
#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن "freertos/semphr.h"
#ضمن "freertos/queue.h"
#ضمن "unity.h"
#ضمن "test_utils.h"

متطاير ساكن صحيح done;
متطاير ساكن صحيح error;

ساكن فراغ tskTestRand(فراغ *pvParameters)
{
    صحيح l;
    srand(0x1234);
    vTaskDelay((صحيح)pvParameters / portTICK_PERIOD_MS);
    l = rand();
    printf("Rand1: %d\n", l);
    ܐܢ (l != 869320854) {
        error++;
    }
    vTaskDelay((صحيح)pvParameters / portTICK_PERIOD_MS);
    l = rand();
    printf("Rand2: %d\n", l);
    ܐܢ (l != 1148737841) {
        error++;
    }
    done++;
    vTaskDelete(عدم);
}

// TODO: split this thing into separate orthogonal tests
TEST_CASE("Test for per-task non-reentrant tasks", "[freertos]")
{
    done = 0;
    error = 0;
    ثابت uint32_t stack_size = 3072;
    xTaskCreatePinnedToCore(tskTestRand, "tsk1", stack_size, (فراغ *)100, 3, عدم, 0);
    xTaskCreatePinnedToCore(tskTestRand, "tsk2", stack_size, (فراغ *)200, 3, عدم, 0);
    xTaskCreatePinnedToCore(tskTestRand, "tsk3", stack_size, (فراغ *)300, 3, عدم, CONFIG_FREERTOS_NUMBER_OF_CORES - 1);
    xTaskCreatePinnedToCore(tskTestRand, "tsk4", stack_size, (فراغ *)400, 3, عدم, 0);
    ܟܡܐ (done != 4) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    TEST_ASSERT(error == 0);
}
