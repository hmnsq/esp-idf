/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Test for multicore FreeRTOS. This test spins up threads, fiddles with queues etc.
*/

#അന്തര്ഭാവ <esp_types.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ "sdkconfig.h"
#അന്തര്ഭാവ "freertos/FreeRTOS.h"
#അന്തര്ഭാവ "freertos/task.h"
#അന്തര്ഭാവ "freertos/semphr.h"
#അന്തര്ഭാവ "freertos/queue.h"
#അന്തര്ഭാവ "unity.h"
#അന്തര്ഭാവ "test_utils.h"

അസ്ഥിര സ്ഥിതം പൂർണ്ണസംഖ്യ done;
അസ്ഥിര സ്ഥിതം പൂർണ്ണസംഖ്യ error;

സ്ഥിതം ശൂന്യം tskTestRand(ശൂന്യം *pvParameters)
{
    പൂർണ്ണസംഖ്യ l;
    srand(0x1234);
    vTaskDelay((പൂർണ്ണസംഖ്യ)pvParameters / portTICK_PERIOD_MS);
    l = rand();
    printf("Rand1: %d\n", l);
    എങ്കിൽ (l != 869320854) {
        error++;
    }
    vTaskDelay((പൂർണ്ണസംഖ്യ)pvParameters / portTICK_PERIOD_MS);
    l = rand();
    printf("Rand2: %d\n", l);
    എങ്കിൽ (l != 1148737841) {
        error++;
    }
    done++;
    vTaskDelete(ശൂന്യ);
}

// TODO: split this thing into separate orthogonal tests
TEST_CASE("Test for per-task non-reentrant tasks", "[freertos]")
{
    done = 0;
    error = 0;
    സ്ഥിരം uint32_t അട്ടി_size = 3072;
    xTaskCreatePinnedToCore(tskTestRand, "tsk1", അട്ടി_size, (ശൂന്യം *)100, 3, ശൂന്യ, 0);
    xTaskCreatePinnedToCore(tskTestRand, "tsk2", അട്ടി_size, (ശൂന്യം *)200, 3, ശൂന്യ, 0);
    xTaskCreatePinnedToCore(tskTestRand, "tsk3", അട്ടി_size, (ശൂന്യം *)300, 3, ശൂന്യ, CONFIG_FREERTOS_സംഖ്യ_OF_CORES - 1);
    xTaskCreatePinnedToCore(tskTestRand, "tsk4", അട്ടി_size, (ശൂന്യം *)400, 3, ശൂന്യ, 0);
    ആകുമ്പോൾ (done != 4) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    TEST_ASSERT(error == 0);
}
