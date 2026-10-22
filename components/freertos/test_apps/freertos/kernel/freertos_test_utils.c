/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن "sdkconfig.h"
#ضمن "FreeRTOS.h"
#ضمن "semphr.h"
#ضمن "freertos_test_utils.h"

#إذا ( CONFIG_FREERTOS_نمبر_دا_CORES > 1 )

تعريفنوع بنية {
    ثابت TestFunction_t px_Test_کوڈ;
    فراغ * ثابت pv_Test_کوڈ_Arg;
    ثابت SemaphoreHandle_t x_Task_پورا_Sem;
} Test_دلیلاں_t;

ساكن فراغ test_func_task(فراغ * pvParameters)
{
    Test_دلیلاں_t * px_Test_دلیلاں = (Test_دلیلاں_t *) pvParameters;
    /* Call the test function */
    px_Test_دلیلاں->px_Test_کوڈ(px_Test_دلیلاں->pv_Test_کوڈ_Arg);
    /* Indicate completion to the creation task and wait to be deleted. */
    xSemaphoreGive(px_Test_دلیلاں->x_Task_پورا_Sem);
    vTaskSuspend(عدم);
}

فراغ vTestOnAllCores(TestFunction_t px_Test_کوڈ, فراغ * pv_Test_کوڈ_Arg, uint32_t ul_ڈھیر_ڈونگھائی, UBaseType_t ux_ترجیح)
{
    SemaphoreHandle_t x_Task_پورا_Sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_نمبر_دا_CORES, 0);
    TaskHandle_t xTaskHandles[ CONFIG_FREERTOS_نمبر_دا_CORES ];
    Test_دلیلاں_t x_Test_دلیلاں = {
        .px_Test_کوڈ = px_Test_کوڈ,
        .pv_Test_کوڈ_Arg = pv_Test_کوڈ_Arg,
        .x_Task_پورا_Sem = x_Task_پورا_Sem,
    };

    /* Create a separate task on each core to run the test function */
    لكل (ادھار_Type_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_نمبر_دا_CORES; xCoreID++) {
#إذا ( CONFIG_FREERTOS_SMP == 1 )
        x_Task_Create_Affinity_سیٹ(test_func_task,
                               "task",
                               ul_ڈھیر_ڈونگھائی,
                               (فراغ *) &x_Test_دلیلاں,
                               ux_ترجیح,
                               (UBaseType_t)(1 << xCoreID),
                               &(xTaskHandles[ xCoreID ]));
#وإلا
        x_Task_Create_Pinned_منزل_Core(test_func_task,
                                "task",
                                ul_ڈھیر_ڈونگھائی,
                                (فراغ *) &x_Test_دلیلاں,
                                ux_ترجیح,
                                &(xTaskHandles[ xCoreID ]),
                                xCoreID);
#جے_انت
    }

    /* Wait for each tasks to complete test */
    لكل (ادھار_Type_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_نمبر_دا_CORES; xCoreID++) {
        xSemaphoreTake(x_Task_پورا_Sem, پورٹ_MAX_DELAY);
    }

    /* Cleanup */
    لكل (ادھار_Type_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_نمبر_دا_CORES; xCoreID++) {
        vTaskDelete(xTaskHandles[ xCoreID ]);
    }
    vSemaphoreDelete(x_Task_پورا_Sem);
    vTaskDelay(10); // Short delay to allow task memory to be freed
}

#جے_انت /* ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) */
