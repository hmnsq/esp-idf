/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Test for multicore FreeRTOS. This test spins up threads, fiddles with queues etc.
*/

#अन्तर्भाव <esp_types.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव "sdkconfig.h"
#अन्तर्भाव "freertos/FreeRTOS.h"
#अन्तर्भाव "freertos/task.h"
#अन्तर्भाव "freertos/semphr.h"
#अन्तर्भाव "freertos/queue.h"
#अन्तर्भाव "unity.h"
#अन्तर्भाव "test_utils.h"

चञ्चल स्थिर पूर्णाङ्क समाप्तम्;
चञ्चल स्थिर पूर्णाङ्क error;

स्थिर शून्य tskTestRand(शून्य *pvParameters)
{
    पूर्णाङ्क l;
    बीजं_रोपय(0x1234);
    vTaskDelay((पूर्णाङ्क)pvParameters / पत्तनम्_TICK_PERIOD_MS);
    l = यादृच्छिकम्();
    प्रारूप_लिख("Rand1: %d\n", l);
    यदि (l != 869320854) {
        error++;
    }
    vTaskDelay((पूर्णाङ्क)pvParameters / पत्तनम्_TICK_PERIOD_MS);
    l = यादृच्छिकम्();
    प्रारूप_लिख("Rand2: %d\n", l);
    यदि (l != 1148737841) {
        error++;
    }
    समाप्तम्++;
    vTaskDelete(रिक्त);
}

// TODO: split this thing into separate orthogonal tests
TEST_CASE("Test for per-task non-reentrant tasks", "[freertos]")
{
    समाप्तम् = 0;
    error = 0;
    ध्रुव uint32_t राशिः_आकार_2 = 3072;
    x_Task_Create_Pinned_गन्तव्यम्_Core(tskTestRand, "tsk1", राशिः_आकार_2, (शून्य *)100, 3, रिक्त, 0);
    x_Task_Create_Pinned_गन्तव्यम्_Core(tskTestRand, "tsk2", राशिः_आकार_2, (शून्य *)200, 3, रिक्त, 0);
    x_Task_Create_Pinned_गन्तव्यम्_Core(tskTestRand, "tsk3", राशिः_आकार_2, (शून्य *)300, 3, रिक्त, CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES - 1);
    x_Task_Create_Pinned_गन्तव्यम्_Core(tskTestRand, "tsk4", राशिः_आकार_2, (शून्य *)400, 3, रिक्त, 0);
    यावत् (समाप्तम् != 4) {
        vTaskDelay(1000 / पत्तनम्_TICK_PERIOD_MS);
    }
    TEST_ASSERT(error == 0);
}
