/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Test for multicore FreeRTOS. This test spins up threads, fiddles with queues etc.
*/

#ଅନ୍ତର୍ଭାଵ <esp_types.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ "sdkconfig.h"
#ଅନ୍ତର୍ଭାଵ "freertos/FreeRTOS.h"
#ଅନ୍ତର୍ଭାଵ "freertos/task.h"
#ଅନ୍ତର୍ଭାଵ "freertos/semphr.h"
#ଅନ୍ତର୍ଭାଵ "freertos/queue.h"
#ଅନ୍ତର୍ଭାଵ "unity.h"
#ଅନ୍ତର୍ଭାଵ "test_utils.h"

ଅସ୍ଥିର ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା done;
ଅସ୍ଥିର ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା error;

ସ୍ଥିର ଶୂନ୍ୟ tskTestRand(ଶୂନ୍ୟ *pvParameters)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା l;
    srand(0x1234);
    vTaskDelay((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)pvParameters / portTICK_PERIOD_MS);
    l = rand();
    printf("Rand1: %d\n", l);
    ଯଦି (l != 869320854) {
        error++;
    }
    vTaskDelay((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)pvParameters / portTICK_PERIOD_MS);
    l = rand();
    printf("Rand2: %d\n", l);
    ଯଦି (l != 1148737841) {
        error++;
    }
    done++;
    vTaskDelete(ଖାଲି);
}

// TODO: split this thing into separate orthogonal tests
TEST_CASE("Test for per-task non-reentrant tasks", "[freertos]")
{
    done = 0;
    error = 0;
    ଅଚଳ uint32_t ଗଦା_size = 3072;
    xTaskCreatePinnedToCore(tskTestRand, "tsk1", ଗଦା_size, (ଶୂନ୍ୟ *)100, 3, ଖାଲି, 0);
    xTaskCreatePinnedToCore(tskTestRand, "tsk2", ଗଦା_size, (ଶୂନ୍ୟ *)200, 3, ଖାଲି, 0);
    xTaskCreatePinnedToCore(tskTestRand, "tsk3", ଗଦା_size, (ଶୂନ୍ୟ *)300, 3, ଖାଲି, CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES - 1);
    xTaskCreatePinnedToCore(tskTestRand, "tsk4", ଗଦା_size, (ଶୂନ୍ୟ *)400, 3, ଖାଲି, 0);
    ଯେତେବେଳେ (done != 4) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    TEST_ASSERT(error == 0);
}
