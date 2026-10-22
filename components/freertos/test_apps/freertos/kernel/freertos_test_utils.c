/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#অন্তর্ভাব "sdkconfig.h"
#অন্তর্ভাব "FreeRTOS.h"
#অন্তর্ভাব "semphr.h"
#অন্তর্ভাব "freertos_test_utils.h"

#যদি ( CONFIG_FREERTOS_সংখ্যা_OF_CORES > 1 )

প্রকারলেখ কাঠামো {
    ধ্রুবক TestFunction_t pxTestCode;
    শূন্য * ধ্রুবক pvTestCodeArg;
    ধ্রুবক SemaphoreHandle_t xTaskDoneSem;
} TestArgs_t;

স্থির শূন্য test_func_task(শূন্য * pvParameters)
{
    TestArgs_t * pxTestArgs = (TestArgs_t *) pvParameters;
    /* Call the test function */
    pxTestArgs->pxTestCode(pxTestArgs->pvTestCodeArg);
    /* Indicate completion to the creation task and wait to be deleted. */
    xSemaphoreGive(pxTestArgs->xTaskDoneSem);
    vTaskSuspend(ফাঁকা);
}

শূন্য vTestOnAllCores(TestFunction_t pxTestCode, শূন্য * pvTestCodeArg, uint32_t ul_স্তূপ_Depth, UBaseType_t uxPriority)
{
    SemaphoreHandle_t xTaskDoneSem = xSemaphoreCreateCounting(CONFIG_FREERTOS_সংখ্যা_OF_CORES, 0);
    TaskHandle_t xTaskHandles[ CONFIG_FREERTOS_সংখ্যা_OF_CORES ];
    TestArgs_t xTestArgs = {
        .pxTestCode = pxTestCode,
        .pvTestCodeArg = pvTestCodeArg,
        .xTaskDoneSem = xTaskDoneSem,
    };

    /* Create a separate task on each core to run the test function */
    জন্য (BaseType_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_সংখ্যা_OF_CORES; xCoreID++) {
#যদি ( CONFIG_FREERTOS_SMP == 1 )
        xTaskCreateAffinitySet(test_func_task,
                               "task",
                               ul_স্তূপ_Depth,
                               (শূন্য *) &xTestArgs,
                               uxPriority,
                               (UBaseType_t)(1 << xCoreID),
                               &(xTaskHandles[ xCoreID ]));
#অন্যথা
        xTaskCreatePinnedToCore(test_func_task,
                                "task",
                                ul_স্তূপ_Depth,
                                (শূন্য *) &xTestArgs,
                                uxPriority,
                                &(xTaskHandles[ xCoreID ]),
                                xCoreID);
#endif
    }

    /* Wait for each tasks to complete test */
    জন্য (BaseType_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_সংখ্যা_OF_CORES; xCoreID++) {
        xSemaphoreTake(xTaskDoneSem, portMAX_DELAY);
    }

    /* Cleanup */
    জন্য (BaseType_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_সংখ্যা_OF_CORES; xCoreID++) {
        vTaskDelete(xTaskHandles[ xCoreID ]);
    }
    vSemaphoreDelete(xTaskDoneSem);
    vTaskDelay(10); // Short delay to allow task memory to be freed
}

#endif /* ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) */
