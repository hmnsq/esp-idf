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

ಅಸ್ಥಿರ ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಮುಗಿದ;
ಅಸ್ಥಿರ ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ error;

ಸ್ಥಿರ ಶೂನ್ಯ tskTestRand(ಶೂನ್ಯ *pvParameters)
{
    ಪೂರ್ಣಾಂಕ l;
    srand(0x1234);
    vTaskDelay((ಪೂರ್ಣಾಂಕ)pvParameters / ಪೋರ್ಟ್_TICK_PERIOD_MS);
    l = rand();
    ಬರೆ("Rand1: %d\n", l);
    ಆದರೆ (l != 869320854) {
        error++;
    }
    vTaskDelay((ಪೂರ್ಣಾಂಕ)pvParameters / ಪೋರ್ಟ್_TICK_PERIOD_MS);
    l = rand();
    ಬರೆ("Rand2: %d\n", l);
    ಆದರೆ (l != 1148737841) {
        error++;
    }
    ಮುಗಿದ++;
    vTaskDelete(ಖಾಲಿ);
}

// TODO: split this thing into separate orthogonal tests
TEST_CASE("Test for per-task non-reentrant tasks", "[freertos]")
{
    ಮುಗಿದ = 0;
    error = 0;
    ಸ್ಥಿರಾಂಕ uint32_t ರಾಶಿ_ಗಾತ್ರ_2 = 3072;
    x_Task_Create_Pinned_ಗಮ್ಯ_Core(tskTestRand, "tsk1", ರಾಶಿ_ಗಾತ್ರ_2, (ಶೂನ್ಯ *)100, 3, ಖಾಲಿ, 0);
    x_Task_Create_Pinned_ಗಮ್ಯ_Core(tskTestRand, "tsk2", ರಾಶಿ_ಗಾತ್ರ_2, (ಶೂನ್ಯ *)200, 3, ಖಾಲಿ, 0);
    x_Task_Create_Pinned_ಗಮ್ಯ_Core(tskTestRand, "tsk3", ರಾಶಿ_ಗಾತ್ರ_2, (ಶೂನ್ಯ *)300, 3, ಖಾಲಿ, CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES - 1);
    x_Task_Create_Pinned_ಗಮ್ಯ_Core(tskTestRand, "tsk4", ರಾಶಿ_ಗಾತ್ರ_2, (ಶೂನ್ಯ *)400, 3, ಖಾಲಿ, 0);
    ಆಗ (ಮುಗಿದ != 4) {
        vTaskDelay(1000 / ಪೋರ್ಟ್_TICK_PERIOD_MS);
    }
    TEST_ASSERT(error == 0);
}
