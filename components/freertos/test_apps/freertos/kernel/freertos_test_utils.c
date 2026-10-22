/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#അന്തര്ഭാവ "sdkconfig.h"
#അന്തര്ഭാവ "FreeRTOS.h"
#അന്തര്ഭാവ "semphr.h"
#അന്തര്ഭാവ "freertos_test_utils.h"

#എങ്കിൽ ( CONFIG_FREERTOS_സംഖ്യ_OF_CORES > 1 )

തരനിർവചനം ഘടന {
    സ്ഥിരം TestFunction_t pxTestCode;
    ശൂന്യം * സ്ഥിരം pvTestCodeArg;
    സ്ഥിരം SemaphoreHandle_t xTaskDoneSem;
} TestArgs_t;

സ്ഥിതം ശൂന്യം test_func_task(ശൂന്യം * pvParameters)
{
    TestArgs_t * pxTestArgs = (TestArgs_t *) pvParameters;
    /* Call the test function */
    pxTestArgs->pxTestCode(pxTestArgs->pvTestCodeArg);
    /* Indicate completion to the creation task and wait to be deleted. */
    xSemaphoreGive(pxTestArgs->xTaskDoneSem);
    vTaskSuspend(ശൂന്യ);
}

ശൂന്യം vTestOnAllCores(TestFunction_t pxTestCode, ശൂന്യം * pvTestCodeArg, uint32_t ul_അട്ടി_Depth, UBaseType_t uxPriority)
{
    SemaphoreHandle_t xTaskDoneSem = xSemaphoreCreateCounting(CONFIG_FREERTOS_സംഖ്യ_OF_CORES, 0);
    TaskHandle_t xTaskHandles[ CONFIG_FREERTOS_സംഖ്യ_OF_CORES ];
    TestArgs_t xTestArgs = {
        .pxTestCode = pxTestCode,
        .pvTestCodeArg = pvTestCodeArg,
        .xTaskDoneSem = xTaskDoneSem,
    };

    /* Create a separate task on each core to run the test function */
    വേണ്ടി (BaseType_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; xCoreID++) {
#എങ്കിൽ ( CONFIG_FREERTOS_SMP == 1 )
        xTaskCreateAffinitySet(test_func_task,
                               "task",
                               ul_അട്ടി_Depth,
                               (ശൂന്യം *) &xTestArgs,
                               uxPriority,
                               (UBaseType_t)(1 << xCoreID),
                               &(xTaskHandles[ xCoreID ]));
#അല്ലെങ്കിൽ
        xTaskCreatePinnedToCore(test_func_task,
                                "task",
                                ul_അട്ടി_Depth,
                                (ശൂന്യം *) &xTestArgs,
                                uxPriority,
                                &(xTaskHandles[ xCoreID ]),
                                xCoreID);
#endif
    }

    /* Wait for each tasks to complete test */
    വേണ്ടി (BaseType_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; xCoreID++) {
        xSemaphoreTake(xTaskDoneSem, portMAX_DELAY);
    }

    /* Cleanup */
    വേണ്ടി (BaseType_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; xCoreID++) {
        vTaskDelete(xTaskHandles[ xCoreID ]);
    }
    vSemaphoreDelete(xTaskDoneSem);
    vTaskDelay(10); // Short delay to allow task memory to be freed
}

#endif /* ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) */
