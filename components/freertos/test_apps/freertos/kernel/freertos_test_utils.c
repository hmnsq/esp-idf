/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ଅନ୍ତର୍ଭାଵ "sdkconfig.h"
#ଅନ୍ତର୍ଭାଵ "FreeRTOS.h"
#ଅନ୍ତର୍ଭାଵ "semphr.h"
#ଅନ୍ତର୍ଭାଵ "freertos_test_utils.h"

#ଯଦି ( CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES > 1 )

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା {
    ଅଚଳ TestFunction_t pxTestCode;
    ଶୂନ୍ୟ * ଅଚଳ pvTestCodeArg;
    ଅଚଳ SemaphoreHandle_t xTaskDoneSem;
} TestArgs_t;

ସ୍ଥିର ଶୂନ୍ୟ test_func_task(ଶୂନ୍ୟ * pvParameters)
{
    TestArgs_t * pxTestArgs = (TestArgs_t *) pvParameters;
    /* Call the test function */
    pxTestArgs->pxTestCode(pxTestArgs->pvTestCodeArg);
    /* Indicate completion to the creation task and wait to be deleted. */
    xSemaphoreGive(pxTestArgs->xTaskDoneSem);
    vTaskSuspend(ଖାଲି);
}

ଶୂନ୍ୟ vTestOnAllCores(TestFunction_t pxTestCode, ଶୂନ୍ୟ * pvTestCodeArg, uint32_t ul_ଗଦା_Depth, UBaseType_t uxPriority)
{
    SemaphoreHandle_t xTaskDoneSem = xSemaphoreCreateCounting(CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES, 0);
    TaskHandle_t xTaskHandles[ CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES ];
    TestArgs_t xTestArgs = {
        .pxTestCode = pxTestCode,
        .pvTestCodeArg = pvTestCodeArg,
        .xTaskDoneSem = xTaskDoneSem,
    };

    /* Create a separate task on each core to run the test function */
    ପାଇଁ (BaseType_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES; xCoreID++) {
#ଯଦି ( CONFIG_FREERTOS_SMP == 1 )
        xTaskCreateAffinitySet(test_func_task,
                               "task",
                               ul_ଗଦା_Depth,
                               (ଶୂନ୍ୟ *) &xTestArgs,
                               uxPriority,
                               (UBaseType_t)(1 << xCoreID),
                               &(xTaskHandles[ xCoreID ]));
#ଅନ୍ୟଥା
        xTaskCreatePinnedToCore(test_func_task,
                                "task",
                                ul_ଗଦା_Depth,
                                (ଶୂନ୍ୟ *) &xTestArgs,
                                uxPriority,
                                &(xTaskHandles[ xCoreID ]),
                                xCoreID);
#endif
    }

    /* Wait for each tasks to complete test */
    ପାଇଁ (BaseType_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES; xCoreID++) {
        xSemaphoreTake(xTaskDoneSem, portMAX_DELAY);
    }

    /* Cleanup */
    ପାଇଁ (BaseType_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES; xCoreID++) {
        vTaskDelete(xTaskHandles[ xCoreID ]);
    }
    vSemaphoreDelete(xTaskDoneSem);
    vTaskDelay(10); // Short delay to allow task memory to be freed
}

#endif /* ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) */
