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

अस्थिर स्थिर पूर्णांक done;
अस्थिर स्थिर पूर्णांक error;

स्थिर शून्य tskTestRand(शून्य *pvParameters)
{
    पूर्णांक l;
    बेक्रम(0x1234);
    vTaskDelay((पूर्णांक)pvParameters / portTICK_PERIOD_MS);
    l = अक्रम();
    म_लिखो("Rand1: %d\n", l);
    जर (l != 869320854) {
        error++;
    }
    vTaskDelay((पूर्णांक)pvParameters / portTICK_PERIOD_MS);
    l = अक्रम();
    म_लिखो("Rand2: %d\n", l);
    जर (l != 1148737841) {
        error++;
    }
    done++;
    vTaskDelete(रिक्त);
}

// TODO: split this thing into separate orthogonal tests
TEST_CASE("Test for per-task non-reentrant tasks", "[freertos]")
{
    done = 0;
    error = 0;
    स्थिरांक uint32_t चळत_size = 3072;
    xTaskCreatePinnedToCore(tskTestRand, "tsk1", चळत_size, (शून्य *)100, 3, रिक्त, 0);
    xTaskCreatePinnedToCore(tskTestRand, "tsk2", चळत_size, (शून्य *)200, 3, रिक्त, 0);
    xTaskCreatePinnedToCore(tskTestRand, "tsk3", चळत_size, (शून्य *)300, 3, रिक्त, CONFIG_FREERTOS_संख्या_OF_CORES - 1);
    xTaskCreatePinnedToCore(tskTestRand, "tsk4", चळत_size, (शून्य *)400, 3, रिक्त, 0);
    पर्यंत (done != 4) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    TEST_ASSERT(error == 0);
}
