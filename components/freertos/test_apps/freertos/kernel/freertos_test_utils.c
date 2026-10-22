/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#அந்தர்பாவ "sdkconfig.h"
#அந்தர்பாவ "FreeRTOS.h"
#அந்தர்பாவ "semphr.h"
#அந்தர்பாவ "freertos_test_utils.h"

#எனில் ( CONFIG_FREERTOS_எண்_OF_CORES > 1 )

வகைவரை கட்டமைப்பு {
    மாறிலி TestFunction_t pxTestCode;
    வெறுமை * மாறிலி pvTestCodeArg;
    மாறிலி SemaphoreHandle_t xTaskDoneSem;
} TestArgs_t;

நிலையான வெறுமை test_func_task(வெறுமை * pvParameters)
{
    TestArgs_t * pxTestArgs = (TestArgs_t *) pvParameters;
    /* Call the test function */
    pxTestArgs->pxTestCode(pxTestArgs->pvTestCodeArg);
    /* Indicate completion to the creation task and wait to be deleted. */
    xSemaphoreGive(pxTestArgs->xTaskDoneSem);
    vTaskSuspend(வெற்று);
}

வெறுமை vTestOnAllCores(TestFunction_t pxTestCode, வெறுமை * pvTestCodeArg, uint32_t ul_அடுக்கு_Depth, UBaseType_t uxPriority)
{
    SemaphoreHandle_t xTaskDoneSem = xSemaphoreCreateCounting(CONFIG_FREERTOS_எண்_OF_CORES, 0);
    TaskHandle_t xTaskHandles[ CONFIG_FREERTOS_எண்_OF_CORES ];
    TestArgs_t xTestArgs = {
        .pxTestCode = pxTestCode,
        .pvTestCodeArg = pvTestCodeArg,
        .xTaskDoneSem = xTaskDoneSem,
    };

    /* Create a separate task on each core to run the test function */
    க்கு (BaseType_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_எண்_OF_CORES; xCoreID++) {
#எனில் ( CONFIG_FREERTOS_SMP == 1 )
        xTaskCreateAffinitySet(test_func_task,
                               "task",
                               ul_அடுக்கு_Depth,
                               (வெறுமை *) &xTestArgs,
                               uxPriority,
                               (UBaseType_t)(1 << xCoreID),
                               &(xTaskHandles[ xCoreID ]));
#இல்லையெனில்
        xTaskCreatePinnedToCore(test_func_task,
                                "task",
                                ul_அடுக்கு_Depth,
                                (வெறுமை *) &xTestArgs,
                                uxPriority,
                                &(xTaskHandles[ xCoreID ]),
                                xCoreID);
#endif
    }

    /* Wait for each tasks to complete test */
    க்கு (BaseType_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_எண்_OF_CORES; xCoreID++) {
        xSemaphoreTake(xTaskDoneSem, portMAX_DELAY);
    }

    /* Cleanup */
    க்கு (BaseType_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_எண்_OF_CORES; xCoreID++) {
        vTaskDelete(xTaskHandles[ xCoreID ]);
    }
    vSemaphoreDelete(xTaskDoneSem);
    vTaskDelay(10); // Short delay to allow task memory to be freed
}

#endif /* ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) */
