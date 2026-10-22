/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Test for multicore FreeRTOS. This test spins up threads, fiddles with queues etc.
*/

#समावेश <esp_types.h>
#समावेश <stdio.h>
#समावेश <stdlib.h>
#समावेश "sdkconfig.h"
#समावेश "freertos/FreeRTOS.h"
#समावेश "freertos/task.h"
#समावेश "freertos/semphr.h"
#समावेश "freertos/queue.h"
#समावेश "unity.h"
#समावेश "test_utils.h"

अस्थिर अटल पूर्णांक done;
अस्थिर अटल पूर्णांक error;

अटल व्योम tskTestRand(व्योम *pvParameters)
{
    पूर्णांक l;
    बेक्रम(0x1234);
    vTaskDelay((पूर्णांक)pvParameters / portTICK_PERIOD_MS);
    l = अक्रम();
    म_लिखो("Rand1: %d\n", l);
    अगर (l != 869320854) {
        error++;
    }
    vTaskDelay((पूर्णांक)pvParameters / portTICK_PERIOD_MS);
    l = अक्रम();
    म_लिखो("Rand2: %d\n", l);
    अगर (l != 1148737841) {
        error++;
    }
    done++;
    vTaskDelete(शून्य);
}

// TODO: split this thing into separate orthogonal tests
TEST_CASE("Test for per-task non-reentrant tasks", "[freertos]")
{
    done = 0;
    error = 0;
    स्थिर uint32_t ढेर_आकार_2 = 3072;
    xTaskCreatePinnedToCore(tskTestRand, "tsk1", ढेर_आकार_2, (व्योम *)100, 3, शून्य, 0);
    xTaskCreatePinnedToCore(tskTestRand, "tsk2", ढेर_आकार_2, (व्योम *)200, 3, शून्य, 0);
    xTaskCreatePinnedToCore(tskTestRand, "tsk3", ढेर_आकार_2, (व्योम *)300, 3, शून्य, CONFIG_FREERTOS_संख्या_OF_CORES - 1);
    xTaskCreatePinnedToCore(tskTestRand, "tsk4", ढेर_आकार_2, (व्योम *)400, 3, शून्य, 0);
    जबतक (done != 4) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    TEST_ASSERT(error == 0);
}
