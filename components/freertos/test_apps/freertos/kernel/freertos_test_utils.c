/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#समावेश "sdkconfig.h"
#समावेश "FreeRTOS.h"
#समावेश "semphr.h"
#समावेश "freertos_test_utils.h"

#अगर ( CONFIG_FREERTOS_संख्या_OF_CORES > 1 )

प्रकार काष्ठा {
    स्थिर TestFunction_t pxTestCode;
    व्योम * स्थिर pvTestCodeArg;
    स्थिर SemaphoreHandle_t xTaskDoneSem;
} TestArgs_t;

अटल व्योम test_func_task(व्योम * pvParameters)
{
    TestArgs_t * pxTestArgs = (TestArgs_t *) pvParameters;
    /* Call the test function */
    pxTestArgs->pxTestCode(pxTestArgs->pvTestCodeArg);
    /* Indicate completion to the creation task and wait to be deleted. */
    xSemaphoreGive(pxTestArgs->xTaskDoneSem);
    vTaskSuspend(शून्य);
}

व्योम vTestOnAllCores(TestFunction_t pxTestCode, व्योम * pvTestCodeArg, uint32_t ul_ढेर_Depth, UBaseType_t uxPriority)
{
    SemaphoreHandle_t xTaskDoneSem = xSemaphoreCreateCounting(CONFIG_FREERTOS_संख्या_OF_CORES, 0);
    TaskHandle_t xTaskHandles[ CONFIG_FREERTOS_संख्या_OF_CORES ];
    TestArgs_t xTestArgs = {
        .pxTestCode = pxTestCode,
        .pvTestCodeArg = pvTestCodeArg,
        .xTaskDoneSem = xTaskDoneSem,
    };

    /* Create a separate task on each core to run the test function */
    क्रम (BaseType_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_संख्या_OF_CORES; xCoreID++) {
#अगर ( CONFIG_FREERTOS_SMP == 1 )
        xTaskCreateAffinitySet(test_func_task,
                               "task",
                               ul_ढेर_Depth,
                               (व्योम *) &xTestArgs,
                               uxPriority,
                               (UBaseType_t)(1 << xCoreID),
                               &(xTaskHandles[ xCoreID ]));
#अन्यथा
        xTaskCreatePinnedToCore(test_func_task,
                                "task",
                                ul_ढेर_Depth,
                                (व्योम *) &xTestArgs,
                                uxPriority,
                                &(xTaskHandles[ xCoreID ]),
                                xCoreID);
#पूर्ण_अगर
    }

    /* Wait for each tasks to complete test */
    क्रम (BaseType_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_संख्या_OF_CORES; xCoreID++) {
        xSemaphoreTake(xTaskDoneSem, portMAX_DELAY);
    }

    /* Cleanup */
    क्रम (BaseType_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_संख्या_OF_CORES; xCoreID++) {
        vTaskDelete(xTaskHandles[ xCoreID ]);
    }
    vSemaphoreDelete(xTaskDoneSem);
    vTaskDelay(10); // Short delay to allow task memory to be freed
}

#पूर्ण_अगर /* ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) */
