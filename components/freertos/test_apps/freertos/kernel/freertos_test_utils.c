/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#અન્તર્ભાવ "sdkconfig.h"
#અન્તર્ભાવ "FreeRTOS.h"
#અન્તર્ભાવ "semphr.h"
#અન્તર્ભાવ "freertos_test_utils.h"

#જો ( CONFIG_FREERTOS_સંખ્યા_OF_CORES > 1 )

પ્રકારવ્યાખ્યા સંરચના {
    અચલ TestFunction_t pxTestCode;
    શૂન્ય * અચલ pvTestCodeArg;
    અચલ SemaphoreHandle_t xTaskDoneSem;
} TestArgs_t;

સ્થિર શૂન્ય test_func_task(શૂન્ય * pvParameters)
{
    TestArgs_t * pxTestArgs = (TestArgs_t *) pvParameters;
    /* Call the test function */
    pxTestArgs->pxTestCode(pxTestArgs->pvTestCodeArg);
    /* Indicate completion to the creation task and wait to be deleted. */
    xSemaphoreGive(pxTestArgs->xTaskDoneSem);
    vTaskSuspend(ખાલી);
}

શૂન્ય vTestOnAllCores(TestFunction_t pxTestCode, શૂન્ય * pvTestCodeArg, uint32_t ul_ઢગલો_Depth, UBaseType_t uxPriority)
{
    SemaphoreHandle_t xTaskDoneSem = xSemaphoreCreateCounting(CONFIG_FREERTOS_સંખ્યા_OF_CORES, 0);
    TaskHandle_t xTaskHandles[ CONFIG_FREERTOS_સંખ્યા_OF_CORES ];
    TestArgs_t xTestArgs = {
        .pxTestCode = pxTestCode,
        .pvTestCodeArg = pvTestCodeArg,
        .xTaskDoneSem = xTaskDoneSem,
    };

    /* Create a separate task on each core to run the test function */
    માટે (BaseType_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_સંખ્યા_OF_CORES; xCoreID++) {
#જો ( CONFIG_FREERTOS_SMP == 1 )
        xTaskCreateAffinitySet(test_func_task,
                               "task",
                               ul_ઢગલો_Depth,
                               (શૂન્ય *) &xTestArgs,
                               uxPriority,
                               (UBaseType_t)(1 << xCoreID),
                               &(xTaskHandles[ xCoreID ]));
#નહીંતર
        xTaskCreatePinnedToCore(test_func_task,
                                "task",
                                ul_ઢગલો_Depth,
                                (શૂન્ય *) &xTestArgs,
                                uxPriority,
                                &(xTaskHandles[ xCoreID ]),
                                xCoreID);
#endif
    }

    /* Wait for each tasks to complete test */
    માટે (BaseType_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_સંખ્યા_OF_CORES; xCoreID++) {
        xSemaphoreTake(xTaskDoneSem, portMAX_DELAY);
    }

    /* Cleanup */
    માટે (BaseType_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_સંખ્યા_OF_CORES; xCoreID++) {
        vTaskDelete(xTaskHandles[ xCoreID ]);
    }
    vSemaphoreDelete(xTaskDoneSem);
    vTaskDelay(10); // Short delay to allow task memory to be freed
}

#endif /* ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) */
