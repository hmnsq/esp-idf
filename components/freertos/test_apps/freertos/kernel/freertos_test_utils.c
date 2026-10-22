/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#अन्तर्भाव "sdkconfig.h"
#अन्तर्भाव "FreeRTOS.h"
#अन्तर्भाव "semphr.h"
#अन्तर्भाव "freertos_test_utils.h"

#यदि ( CONFIG_FREERTOS_संख्या_OF_CORES > 1 )

प्रकारनिर्देश संरचना {
    ध्रुव TestFunction_t pxTestCode;
    शून्य * ध्रुव pvTestCodeArg;
    ध्रुव SemaphoreHandle_t xTaskDoneSem;
} TestArgs_t;

स्थिर शून्य test_func_task(शून्य * pvParameters)
{
    TestArgs_t * pxTestArgs = (TestArgs_t *) pvParameters;
    /* Call the test function */
    pxTestArgs->pxTestCode(pxTestArgs->pvTestCodeArg);
    /* Indicate completion to the creation task and wait to be deleted. */
    xSemaphoreGive(pxTestArgs->xTaskDoneSem);
    vTaskSuspend(रिक्त);
}

शून्य vTestOnAllCores(TestFunction_t pxTestCode, शून्य * pvTestCodeArg, uint32_t ul_चळत_Depth, UBaseType_t uxPriority)
{
    SemaphoreHandle_t xTaskDoneSem = xSemaphoreCreateCounting(CONFIG_FREERTOS_संख्या_OF_CORES, 0);
    TaskHandle_t xTaskHandles[ CONFIG_FREERTOS_संख्या_OF_CORES ];
    TestArgs_t xTestArgs = {
        .pxTestCode = pxTestCode,
        .pvTestCodeArg = pvTestCodeArg,
        .xTaskDoneSem = xTaskDoneSem,
    };

    /* Create a separate task on each core to run the test function */
    प्रति (BaseType_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_संख्या_OF_CORES; xCoreID++) {
#यदि ( CONFIG_FREERTOS_SMP == 1 )
        xTaskCreateAffinitySet(test_func_task,
                               "task",
                               ul_चळत_Depth,
                               (शून्य *) &xTestArgs,
                               uxPriority,
                               (UBaseType_t)(1 << xCoreID),
                               &(xTaskHandles[ xCoreID ]));
#अन्यथा
        xTaskCreatePinnedToCore(test_func_task,
                                "task",
                                ul_चळत_Depth,
                                (शून्य *) &xTestArgs,
                                uxPriority,
                                &(xTaskHandles[ xCoreID ]),
                                xCoreID);
#endif
    }

    /* Wait for each tasks to complete test */
    प्रति (BaseType_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_संख्या_OF_CORES; xCoreID++) {
        xSemaphoreTake(xTaskDoneSem, portMAX_DELAY);
    }

    /* Cleanup */
    प्रति (BaseType_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_संख्या_OF_CORES; xCoreID++) {
        vTaskDelete(xTaskHandles[ xCoreID ]);
    }
    vSemaphoreDelete(xTaskDoneSem);
    vTaskDelay(10); // Short delay to allow task memory to be freed
}

#endif /* ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) */
