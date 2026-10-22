/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Test for multicore FreeRTOS. This test spins up threads, fiddles with queues etc.
*/

#ਅਨ੍ਤਰ੍ਭਾਵ <esp_types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/semphr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/queue.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unity.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "test_utils.h"

ਅਸਥਿਰ ਸਥਿਰ ਪੂਰਨ ਪੂਰਾ;
ਅਸਥਿਰ ਸਥਿਰ ਪੂਰਨ error;

ਸਥਿਰ ਖਾਲੀ tskTestRand(ਖਾਲੀ *pvParameters)
{
    ਪੂਰਨ l;
    srand(0x1234);
    vTaskDelay((ਪੂਰਨ)pvParameters / ਪੋਰਟ_TICK_PERIOD_MS);
    l = rand();
    ਲਿਖੋ("Rand1: %d\n", l);
    ਜੇ (l != 869320854) {
        error++;
    }
    vTaskDelay((ਪੂਰਨ)pvParameters / ਪੋਰਟ_TICK_PERIOD_MS);
    l = rand();
    ਲਿਖੋ("Rand2: %d\n", l);
    ਜੇ (l != 1148737841) {
        error++;
    }
    ਪੂਰਾ++;
    vTaskDelete(ਨਿਰਾ);
}

// TODO: split this thing into separate orthogonal tests
TEST_CASE("Test for per-task non-reentrant tasks", "[freertos]")
{
    ਪੂਰਾ = 0;
    error = 0;
    ਅਚਲ uint32_t ਢੇਰ_ਮਾਪ_2 = 3072;
    x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(tskTestRand, "tsk1", ਢੇਰ_ਮਾਪ_2, (ਖਾਲੀ *)100, 3, ਨਿਰਾ, 0);
    x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(tskTestRand, "tsk2", ਢੇਰ_ਮਾਪ_2, (ਖਾਲੀ *)200, 3, ਨਿਰਾ, 0);
    x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(tskTestRand, "tsk3", ਢੇਰ_ਮਾਪ_2, (ਖਾਲੀ *)300, 3, ਨਿਰਾ, CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES - 1);
    x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(tskTestRand, "tsk4", ਢੇਰ_ਮਾਪ_2, (ਖਾਲੀ *)400, 3, ਨਿਰਾ, 0);
    ਜਦੋਂ (ਪੂਰਾ != 4) {
        vTaskDelay(1000 / ਪੋਰਟ_TICK_PERIOD_MS);
    }
    TEST_ASSERT(error == 0);
}
