/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن "sdkconfig.h"
#ضمن "FreeRTOS.h"
#ضمن "semphr.h"
#ضمن "freertos_test_utils.h"

#إذا ( CONFIG_FREERTOS_لمب_OF_CORES > 1 )

تعريفنوع بنية {
    ثابت TestFunction_t pxTestCode;
    فراغ * ثابت pvTestCodeArg;
    ثابت SemaphoreHandle_t xTaskDoneSem;
} Test_حجوجي_t;

ساكن فراغ test_func_task(فراغ * pvParameters)
{
    Test_حجوجي_t * px_Test_حجوجي = (Test_حجوجي_t *) pvParameters;
    /* Call the test function */
    px_Test_حجوجي->pxTestCode(px_Test_حجوجي->pvTestCodeArg);
    /* Indicate completion to the creation task and wait to be deleted. */
    xSemaphoreGive(px_Test_حجوجي->xTaskDoneSem);
    vTaskSuspend(عدم);
}

فراغ vTestOnAllCores(TestFunction_t pxTestCode, فراغ * pvTestCodeArg, uint32_t ul_تر_Depth, UBaseType_t uxPriority)
{
    SemaphoreHandle_t xTaskDoneSem = xSemaphoreCreateCounting(CONFIG_FREERTOS_لمب_OF_CORES, 0);
    TaskHandle_t xTaskHandles[ CONFIG_FREERTOS_لمب_OF_CORES ];
    Test_حجوجي_t x_Test_حجوجي = {
        .pxTestCode = pxTestCode,
        .pvTestCodeArg = pvTestCodeArg,
        .xTaskDoneSem = xTaskDoneSem,
    };

    /* Create a separate task on each core to run the test function */
    لكل (BaseType_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_لمب_OF_CORES; xCoreID++) {
#إذا ( CONFIG_FREERTOS_SMP == 1 )
        xTaskCreateAffinitySet(test_func_task,
                               "task",
                               ul_تر_Depth,
                               (فراغ *) &x_Test_حجوجي,
                               uxPriority,
                               (UBaseType_t)(1 << xCoreID),
                               &(xTaskHandles[ xCoreID ]));
#وإلا
        xTaskCreatePinnedToCore(test_func_task,
                                "task",
                                ul_تر_Depth,
                                (فراغ *) &x_Test_حجوجي,
                                uxPriority,
                                &(xTaskHandles[ xCoreID ]),
                                xCoreID);
#endif
    }

    /* Wait for each tasks to complete test */
    لكل (BaseType_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_لمب_OF_CORES; xCoreID++) {
        xSemaphoreTake(xTaskDoneSem, portMAX_DELAY);
    }

    /* Cleanup */
    لكل (BaseType_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_لمب_OF_CORES; xCoreID++) {
        vTaskDelete(xTaskHandles[ xCoreID ]);
    }
    vSemaphoreDelete(xTaskDoneSem);
    vTaskDelay(10); // Short delay to allow task memory to be freed
}

#endif /* ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) */
