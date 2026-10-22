/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ "FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "semphr.h"
#ಅನ್ತರ್ಭಾವ "freertos_test_utils.h"

#ಯದಿ ( CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES > 1 )

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ {
    ಧ್ರುವ TestFunction_t pxTestCode;
    ಶೂನ್ಯ * ಧ್ರುವ pvTestCodeArg;
    ಧ್ರುವ SemaphoreHandle_t xTaskDoneSem;
} TestArgs_t;

ಸ್ಥಿರ ಶೂನ್ಯ test_func_task(ಶೂನ್ಯ * pvParameters)
{
    TestArgs_t * pxTestArgs = (TestArgs_t *) pvParameters;
    /* Call the test function */
    pxTestArgs->pxTestCode(pxTestArgs->pvTestCodeArg);
    /* Indicate completion to the creation task and wait to be deleted. */
    xSemaphoreGive(pxTestArgs->xTaskDoneSem);
    vTaskSuspend(ರಿಕ್ತ);
}

ಶೂನ್ಯ vTestOnAllCores(TestFunction_t pxTestCode, ಶೂನ್ಯ * pvTestCodeArg, uint32_t ul_ರಾಶಿ_Depth, UBaseType_t uxPriority)
{
    SemaphoreHandle_t xTaskDoneSem = xSemaphoreCreateCounting(CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES, 0);
    TaskHandle_t xTaskHandles[ CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES ];
    TestArgs_t xTestArgs = {
        .pxTestCode = pxTestCode,
        .pvTestCodeArg = pvTestCodeArg,
        .xTaskDoneSem = xTaskDoneSem,
    };

    /* Create a separate task on each core to run the test function */
    ಪ್ರತಿ (BaseType_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES; xCoreID++) {
#ಯದಿ ( CONFIG_FREERTOS_SMP == 1 )
        xTaskCreateAffinitySet(test_func_task,
                               "task",
                               ul_ರಾಶಿ_Depth,
                               (ಶೂನ್ಯ *) &xTestArgs,
                               uxPriority,
                               (UBaseType_t)(1 << xCoreID),
                               &(xTaskHandles[ xCoreID ]));
#ಅನ್ಯಥಾ
        xTaskCreatePinnedToCore(test_func_task,
                                "task",
                                ul_ರಾಶಿ_Depth,
                                (ಶೂನ್ಯ *) &xTestArgs,
                                uxPriority,
                                &(xTaskHandles[ xCoreID ]),
                                xCoreID);
#endif
    }

    /* Wait for each tasks to complete test */
    ಪ್ರತಿ (BaseType_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES; xCoreID++) {
        xSemaphoreTake(xTaskDoneSem, portMAX_DELAY);
    }

    /* Cleanup */
    ಪ್ರತಿ (BaseType_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES; xCoreID++) {
        vTaskDelete(xTaskHandles[ xCoreID ]);
    }
    vSemaphoreDelete(xTaskDoneSem);
    vTaskDelay(10); // Short delay to allow task memory to be freed
}

#endif /* ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) */
