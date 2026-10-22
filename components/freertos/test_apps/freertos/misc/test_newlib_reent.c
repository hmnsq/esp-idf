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

متطاير ساكن صحيح پورو;
متطاير ساكن صحيح error;

ساكن فراغ tskTestRand(فراغ *pvParameters)
{
    صحيح l;
    srand(0x1234);
    vTaskDelay((صحيح)pvParameters / پورٽ_TICK_PERIOD_MS);
    l = rand();
    لکو("Rand1: %d\n", l);
    إذا (l != 869320854) {
        error++;
    }
    vTaskDelay((صحيح)pvParameters / پورٽ_TICK_PERIOD_MS);
    l = rand();
    لکو("Rand2: %d\n", l);
    إذا (l != 1148737841) {
        error++;
    }
    پورو++;
    vTaskDelete(عدم);
}

// TODO: split this thing into separate orthogonal tests
TEST_CASE("Test for per-task non-reentrant tasks", "[freertos]")
{
    پورو = 0;
    error = 0;
    ثابت uint32_t ڍير_سائيز_2 = 3072;
    x_Task_Create_Pinned_منزل_Core(tskTestRand, "tsk1", ڍير_سائيز_2, (فراغ *)100, 3, عدم, 0);
    x_Task_Create_Pinned_منزل_Core(tskTestRand, "tsk2", ڍير_سائيز_2, (فراغ *)200, 3, عدم, 0);
    x_Task_Create_Pinned_منزل_Core(tskTestRand, "tsk3", ڍير_سائيز_2, (فراغ *)300, 3, عدم, CONFIG_FREERTOS_عدد_جو_CORES - 1);
    x_Task_Create_Pinned_منزل_Core(tskTestRand, "tsk4", ڍير_سائيز_2, (فراغ *)400, 3, عدم, 0);
    طالما (پورو != 4) {
        vTaskDelay(1000 / پورٽ_TICK_PERIOD_MS);
    }
    TEST_ASSERT(error == 0);
}
