/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "sdkconfig.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "freertos_test_utils.h"

#if ( CONFIG_FREERTOS_नुमेरो_OF_CORES > 1 )

typedef struct {
    const TestFunction_t pxTestCode;
    void * const pvTestCodeArg;
    const SemaphoreHandle_t xTaskDoneSem;
} Test_मगा_कातविरान_t;

static void test_func_task(void * pvParameters)
{
    Test_मगा_कातविरान_t * px_Test_मगा_कातविरान = (Test_मगा_कातविरान_t *) pvParameters;
    /* Call the test function */
    px_Test_मगा_कातविरान->pxTestCode(px_Test_मगा_कातविरान->pvTestCodeArg);
    /* Indicate completion to the creation task and wait to be deleted. */
    xSemaphoreGive(px_Test_मगा_कातविरान->xTaskDoneSem);
    vTaskSuspend(NULL);
}

void vTestOnAllCores(TestFunction_t pxTestCode, void * pvTestCodeArg, uint32_t ul_सालानसान_Depth, UBaseType_t uxPriority)
{
    SemaphoreHandle_t xTaskDoneSem = xSemaphoreCreateCounting(CONFIG_FREERTOS_नुमेरो_OF_CORES, 0);
    TaskHandle_t xTaskHandles[ CONFIG_FREERTOS_नुमेरो_OF_CORES ];
    Test_मगा_कातविरान_t x_Test_मगा_कातविरान = {
        .pxTestCode = pxTestCode,
        .pvTestCodeArg = pvTestCodeArg,
        .xTaskDoneSem = xTaskDoneSem,
    };

    /* Create a separate task on each core to run the test function */
    for (BaseType_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_नुमेरो_OF_CORES; xCoreID++) {
#if ( CONFIG_FREERTOS_SMP == 1 )
        xTaskCreateAffinitySet(test_func_task,
                               "task",
                               ul_सालानसान_Depth,
                               (void *) &x_Test_मगा_कातविरान,
                               uxPriority,
                               (UBaseType_t)(1 << xCoreID),
                               &(xTaskHandles[ xCoreID ]));
#else
        xTaskCreatePinnedToCore(test_func_task,
                                "task",
                                ul_सालानसान_Depth,
                                (void *) &x_Test_मगा_कातविरान,
                                uxPriority,
                                &(xTaskHandles[ xCoreID ]),
                                xCoreID);
#endif
    }

    /* Wait for each tasks to complete test */
    for (BaseType_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_नुमेरो_OF_CORES; xCoreID++) {
        xSemaphoreTake(xTaskDoneSem, portMAX_DELAY);
    }

    /* Cleanup */
    for (BaseType_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_नुमेरो_OF_CORES; xCoreID++) {
        vTaskDelete(xTaskHandles[ xCoreID ]);
    }
    vSemaphoreDelete(xTaskDoneSem);
    vTaskDelay(10); // Short delay to allow task memory to be freed
}

#endif /* ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) */
