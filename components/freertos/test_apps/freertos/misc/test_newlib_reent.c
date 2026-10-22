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

متلون ساکن صحیح مکمل;
متلون ساکن صحیح error;

ساکن خالی tskTestRand(خالی *pvParameters)
{
    صحیح l;
    srand(0x1234);
    vTaskDelay((صحیح)pvParameters / پورٹ_TICK_PERIOD_MS);
    l = rand();
    لکھو("Rand1: %d\n", l);
    اگر (l != 869320854) {
        error++;
    }
    vTaskDelay((صحیح)pvParameters / پورٹ_TICK_PERIOD_MS);
    l = rand();
    لکھو("Rand2: %d\n", l);
    اگر (l != 1148737841) {
        error++;
    }
    مکمل++;
    vTaskDelete(تہی);
}

// TODO: split this thing into separate orthogonal tests
TEST_CASE("Test for per-task non-reentrant tasks", "[freertos]")
{
    مکمل = 0;
    error = 0;
    ثابت uint32_t ڈھیر_سائز_2 = 3072;
    x_Task_Create_Pinned_مقصد_Core(tskTestRand, "tsk1", ڈھیر_سائز_2, (خالی *)100, 3, تہی, 0);
    x_Task_Create_Pinned_مقصد_Core(tskTestRand, "tsk2", ڈھیر_سائز_2, (خالی *)200, 3, تہی, 0);
    x_Task_Create_Pinned_مقصد_Core(tskTestRand, "tsk3", ڈھیر_سائز_2, (خالی *)300, 3, تہی, CONFIG_FREERTOS_نمبر_کا_CORES - 1);
    x_Task_Create_Pinned_مقصد_Core(tskTestRand, "tsk4", ڈھیر_سائز_2, (خالی *)400, 3, تہی, 0);
    جب (مکمل != 4) {
        vTaskDelay(1000 / پورٹ_TICK_PERIOD_MS);
    }
    TEST_ASSERT(error == 0);
}
