/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Test for multicore FreeRTOS. This test spins up threads, fiddles with queues etc.
*/

#అన్తర్భావ <esp_types.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ "freertos/semphr.h"
#అన్తర్భావ "freertos/queue.h"
#అన్తర్భావ "unity.h"
#అన్తర్భావ "test_utils.h"

అస్థిర స్థిర పూర్ణాంకం పూర్తి;
అస్థిర స్థిర పూర్ణాంకం error;

స్థిర శూన్య tskTestRand(శూన్య *pvParameters)
{
    పూర్ణాంకం l;
    srand(0x1234);
    vTaskDelay((పూర్ణాంకం)pvParameters / పోర్టు_TICK_PERIOD_MS);
    l = rand();
    రాయి("Rand1: %d\n", l);
    ఒకవేళ (l != 869320854) {
        error++;
    }
    vTaskDelay((పూర్ణాంకం)pvParameters / పోర్టు_TICK_PERIOD_MS);
    l = rand();
    రాయి("Rand2: %d\n", l);
    ఒకవేళ (l != 1148737841) {
        error++;
    }
    పూర్తి++;
    vTaskDelete(ఖాళీ);
}

// TODO: split this thing into separate orthogonal tests
TEST_CASE("Test for per-task non-reentrant tasks", "[freertos]")
{
    పూర్తి = 0;
    error = 0;
    స్థిరరాశి uint32_t దొంతర_కొలత_2 = 3072;
    x_Task_Create_Pinned_గమ్యం_Core(tskTestRand, "tsk1", దొంతర_కొలత_2, (శూన్య *)100, 3, ఖాళీ, 0);
    x_Task_Create_Pinned_గమ్యం_Core(tskTestRand, "tsk2", దొంతర_కొలత_2, (శూన్య *)200, 3, ఖాళీ, 0);
    x_Task_Create_Pinned_గమ్యం_Core(tskTestRand, "tsk3", దొంతర_కొలత_2, (శూన్య *)300, 3, ఖాళీ, CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES - 1);
    x_Task_Create_Pinned_గమ్యం_Core(tskTestRand, "tsk4", దొంతర_కొలత_2, (శూన్య *)400, 3, ఖాళీ, 0);
    అయితే (పూర్తి != 4) {
        vTaskDelay(1000 / పోర్టు_TICK_PERIOD_MS);
    }
    TEST_ASSERT(error == 0);
}
