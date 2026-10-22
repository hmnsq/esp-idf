/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Test for multicore FreeRTOS. This test spins up threads, fiddles with queues etc.
*/

#ಅನ್ತರ್ಭಾವ <esp_types.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ "freertos/semphr.h"
#ಅನ್ತರ್ಭಾವ "freertos/queue.h"
#ಅನ್ತರ್ಭಾವ "unity.h"
#ಅನ್ತರ್ಭಾವ "test_utils.h"

ಚಞ್ಚಲ ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ done;
ಚಞ್ಚಲ ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ error;

ಸ್ಥಿರ ಶೂನ್ಯ tskTestRand(ಶೂನ್ಯ *pvParameters)
{
    ಪೂರ್ಣಾಙ್ಕ l;
    srand(0x1234);
    vTaskDelay((ಪೂರ್ಣಾಙ್ಕ)pvParameters / portTICK_PERIOD_MS);
    l = rand();
    printf("Rand1: %d\n", l);
    ಯದಿ (l != 869320854) {
        error++;
    }
    vTaskDelay((ಪೂರ್ಣಾಙ್ಕ)pvParameters / portTICK_PERIOD_MS);
    l = rand();
    printf("Rand2: %d\n", l);
    ಯದಿ (l != 1148737841) {
        error++;
    }
    done++;
    vTaskDelete(ರಿಕ್ತ);
}

// TODO: split this thing into separate orthogonal tests
TEST_CASE("Test for per-task non-reentrant tasks", "[freertos]")
{
    done = 0;
    error = 0;
    ಧ್ರುವ uint32_t ರಾಶಿ_size = 3072;
    xTaskCreatePinnedToCore(tskTestRand, "tsk1", ರಾಶಿ_size, (ಶೂನ್ಯ *)100, 3, ರಿಕ್ತ, 0);
    xTaskCreatePinnedToCore(tskTestRand, "tsk2", ರಾಶಿ_size, (ಶೂನ್ಯ *)200, 3, ರಿಕ್ತ, 0);
    xTaskCreatePinnedToCore(tskTestRand, "tsk3", ರಾಶಿ_size, (ಶೂನ್ಯ *)300, 3, ರಿಕ್ತ, CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES - 1);
    xTaskCreatePinnedToCore(tskTestRand, "tsk4", ರಾಶಿ_size, (ಶೂನ್ಯ *)400, 3, ರಿಕ್ತ, 0);
    ಯಾವತ್ (done != 4) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    TEST_ASSERT(error == 0);
}
