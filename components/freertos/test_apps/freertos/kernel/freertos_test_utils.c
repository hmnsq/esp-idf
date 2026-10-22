/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن "sdkconfig.h"
#ضمن "FreeRTOS.h"
#ضمن "semphr.h"
#ضمن "freertos_test_utils.h"

#إذا ( CONFIG_FREERTOS_عدد_جو_CORES > 1 )

تعريفنوع بنية {
    ثابت TestFunction_t px_Test_ڪوڊ;
    فراغ * ثابت pv_Test_ڪوڊ_Arg;
    ثابت SemaphoreHandle_t x_Task_پورو_Sem;
} Test_دليلون_t;

ساكن فراغ test_func_task(فراغ * pvParameters)
{
    Test_دليلون_t * px_Test_دليلون = (Test_دليلون_t *) pvParameters;
    /* Call the test function */
    px_Test_دليلون->px_Test_ڪوڊ(px_Test_دليلون->pv_Test_ڪوڊ_Arg);
    /* Indicate completion to the creation task and wait to be deleted. */
    xSemaphoreGive(px_Test_دليلون->x_Task_پورو_Sem);
    vTaskSuspend(عدم);
}

فراغ vTestOnAllCores(TestFunction_t px_Test_ڪوڊ, فراغ * pv_Test_ڪوڊ_Arg, uint32_t ul_ڍير_اونهائي, UBaseType_t ux_ترجيح)
{
    SemaphoreHandle_t x_Task_پورو_Sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_عدد_جو_CORES, 0);
    TaskHandle_t xTaskHandles[ CONFIG_FREERTOS_عدد_جو_CORES ];
    Test_دليلون_t x_Test_دليلون = {
        .px_Test_ڪوڊ = px_Test_ڪوڊ,
        .pv_Test_ڪوڊ_Arg = pv_Test_ڪوڊ_Arg,
        .x_Task_پورو_Sem = x_Task_پورو_Sem,
    };

    /* Create a separate task on each core to run the test function */
    لكل (بنياد_Type_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_عدد_جو_CORES; xCoreID++) {
#إذا ( CONFIG_FREERTOS_SMP == 1 )
        x_Task_Create_Affinity_سيٽ(test_func_task,
                               "task",
                               ul_ڍير_اونهائي,
                               (فراغ *) &x_Test_دليلون,
                               ux_ترجيح,
                               (UBaseType_t)(1 << xCoreID),
                               &(xTaskHandles[ xCoreID ]));
#وإلا
        x_Task_Create_Pinned_منزل_Core(test_func_task,
                                "task",
                                ul_ڍير_اونهائي,
                                (فراغ *) &x_Test_دليلون,
                                ux_ترجيح,
                                &(xTaskHandles[ xCoreID ]),
                                xCoreID);
#جيڪڏهن_پورو
    }

    /* Wait for each tasks to complete test */
    لكل (بنياد_Type_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_عدد_جو_CORES; xCoreID++) {
        xSemaphoreTake(x_Task_پورو_Sem, پورٽ_MAX_DELAY);
    }

    /* Cleanup */
    لكل (بنياد_Type_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_عدد_جو_CORES; xCoreID++) {
        vTaskDelete(xTaskHandles[ xCoreID ]);
    }
    vSemaphoreDelete(x_Task_پورو_Sem);
    vTaskDelay(10); // Short delay to allow task memory to be freed
}

#جيڪڏهن_پورو /* ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) */
