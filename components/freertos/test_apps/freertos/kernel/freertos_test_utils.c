/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#अन्तर्भाव "sdkconfig.h"
#अन्तर्भाव "FreeRTOS.h"
#अन्तर्भाव "semphr.h"
#अन्तर्भाव "freertos_test_utils.h"

#यदि ( CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES > 1 )

प्रकारनिर्देश संरचना {
    ध्रुव TestFunction_t px_Test_संहिता;
    शून्य * ध्रुव pv_Test_संहिता_Arg;
    ध्रुव SemaphoreHandle_t x_Task_समाप्तम्_Sem;
} Test_तर्काः_t;

स्थिर शून्य test_func_task(शून्य * pvParameters)
{
    Test_तर्काः_t * px_Test_तर्काः = (Test_तर्काः_t *) pvParameters;
    /* Call the test function */
    px_Test_तर्काः->px_Test_संहिता(px_Test_तर्काः->pv_Test_संहिता_Arg);
    /* Indicate completion to the creation task and wait to be deleted. */
    xSemaphoreGive(px_Test_तर्काः->x_Task_समाप्तम्_Sem);
    vTaskSuspend(रिक्त);
}

शून्य vTestOnAllCores(TestFunction_t px_Test_संहिता, शून्य * pv_Test_संहिता_Arg, uint32_t ul_राशिः_गभीरता, UBaseType_t ux_प्राथमिकता)
{
    SemaphoreHandle_t x_Task_समाप्तम्_Sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES, 0);
    TaskHandle_t xTaskHandles[ CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES ];
    Test_तर्काः_t x_Test_तर्काः = {
        .px_Test_संहिता = px_Test_संहिता,
        .pv_Test_संहिता_Arg = pv_Test_संहिता_Arg,
        .x_Task_समाप्तम्_Sem = x_Task_समाप्तम्_Sem,
    };

    /* Create a separate task on each core to run the test function */
    प्रति (आधारः_Type_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES; xCoreID++) {
#यदि ( CONFIG_FREERTOS_SMP == 1 )
        x_Task_Create_Affinity_समुच्चयः(test_func_task,
                               "task",
                               ul_राशिः_गभीरता,
                               (शून्य *) &x_Test_तर्काः,
                               ux_प्राथमिकता,
                               (UBaseType_t)(1 << xCoreID),
                               &(xTaskHandles[ xCoreID ]));
#अन्यथा
        x_Task_Create_Pinned_गन्तव्यम्_Core(test_func_task,
                                "task",
                                ul_राशिः_गभीरता,
                                (शून्य *) &x_Test_तर्काः,
                                ux_प्राथमिकता,
                                &(xTaskHandles[ xCoreID ]),
                                xCoreID);
#यदि_अन्तः
    }

    /* Wait for each tasks to complete test */
    प्रति (आधारः_Type_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES; xCoreID++) {
        xSemaphoreTake(x_Task_समाप्तम्_Sem, पत्तनम्_MAX_DELAY);
    }

    /* Cleanup */
    प्रति (आधारः_Type_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES; xCoreID++) {
        vTaskDelete(xTaskHandles[ xCoreID ]);
    }
    vSemaphoreDelete(x_Task_समाप्तम्_Sem);
    vTaskDelay(10); // Short delay to allow task memory to be freed
}

#यदि_अन्तः /* ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) */
