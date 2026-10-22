/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#شامل "sdkconfig.h"
#شامل "FreeRTOS.h"
#شامل "semphr.h"
#شامل "freertos_test_utils.h"

#اگر ( CONFIG_FREERTOS_عدد_OF_CORES > 1 )

تعریفنوع ساختار {
    ثابت TestFunction_t pxTestCode;
    تهی * ثابت pvTestCodeArg;
    ثابت SemaphoreHandle_t xTaskDoneSem;
} TestArgs_t;

ایستا تهی test_func_task(تهی * pvParameters)
{
    TestArgs_t * pxTestArgs = (TestArgs_t *) pvParameters;
    /* Call the test function */
    pxTestArgs->pxTestCode(pxTestArgs->pvTestCodeArg);
    /* Indicate completion to the creation task and wait to be deleted. */
    xSemaphoreGive(pxTestArgs->xTaskDoneSem);
    vTaskSuspend(پوچ);
}

تهی vTestOnAllCores(TestFunction_t pxTestCode, تهی * pvTestCodeArg, uint32_t ul_پشته_Depth, UBaseType_t uxPriority)
{
    SemaphoreHandle_t xTaskDoneSem = xSemaphoreCreateCounting(CONFIG_FREERTOS_عدد_OF_CORES, 0);
    TaskHandle_t xTaskHandles[ CONFIG_FREERTOS_عدد_OF_CORES ];
    TestArgs_t xTestArgs = {
        .pxTestCode = pxTestCode,
        .pvTestCodeArg = pvTestCodeArg,
        .xTaskDoneSem = xTaskDoneSem,
    };

    /* Create a separate task on each core to run the test function */
    برای (BaseType_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_عدد_OF_CORES; xCoreID++) {
#اگر ( CONFIG_FREERTOS_SMP == 1 )
        xTaskCreateAffinitySet(test_func_task,
                               "task",
                               ul_پشته_Depth,
                               (تهی *) &xTestArgs,
                               uxPriority,
                               (UBaseType_t)(1 << xCoreID),
                               &(xTaskHandles[ xCoreID ]));
#وگرنه
        xTaskCreatePinnedToCore(test_func_task,
                                "task",
                                ul_پشته_Depth,
                                (تهی *) &xTestArgs,
                                uxPriority,
                                &(xTaskHandles[ xCoreID ]),
                                xCoreID);
#endif
    }

    /* Wait for each tasks to complete test */
    برای (BaseType_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_عدد_OF_CORES; xCoreID++) {
        xSemaphoreTake(xTaskDoneSem, portMAX_DELAY);
    }

    /* Cleanup */
    برای (BaseType_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_عدد_OF_CORES; xCoreID++) {
        vTaskDelete(xTaskHandles[ xCoreID ]);
    }
    vSemaphoreDelete(xTaskDoneSem);
    vTaskDelay(10); // Short delay to allow task memory to be freed
}

#endif /* ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) */
