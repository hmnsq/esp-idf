/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#شامل "sdkconfig.h"
#شامل "FreeRTOS.h"
#شامل "semphr.h"
#شامل "freertos_test_utils.h"

#اگر ( CONFIG_FREERTOS_نمبر_کا_CORES > 1 )

قسمتعریف ساخت {
    ثابت TestFunction_t px_Test_رمز;
    خالی * ثابت pv_Test_رمز_Arg;
    ثابت SemaphoreHandle_t x_Task_مکمل_Sem;
} Test_دلائل_t;

ساکن خالی test_func_task(خالی * pvParameters)
{
    Test_دلائل_t * px_Test_دلائل = (Test_دلائل_t *) pvParameters;
    /* Call the test function */
    px_Test_دلائل->px_Test_رمز(px_Test_دلائل->pv_Test_رمز_Arg);
    /* Indicate completion to the creation task and wait to be deleted. */
    xSemaphoreGive(px_Test_دلائل->x_Task_مکمل_Sem);
    vTaskSuspend(تہی);
}

خالی vTestOnAllCores(TestFunction_t px_Test_رمز, خالی * pv_Test_رمز_Arg, uint32_t ul_ڈھیر_گہرائی, UBaseType_t ux_ترجیح)
{
    SemaphoreHandle_t x_Task_مکمل_Sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_نمبر_کا_CORES, 0);
    TaskHandle_t xTaskHandles[ CONFIG_FREERTOS_نمبر_کا_CORES ];
    Test_دلائل_t x_Test_دلائل = {
        .px_Test_رمز = px_Test_رمز,
        .pv_Test_رمز_Arg = pv_Test_رمز_Arg,
        .x_Task_مکمل_Sem = x_Task_مکمل_Sem,
    };

    /* Create a separate task on each core to run the test function */
    برائے (بنیاد_Type_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_نمبر_کا_CORES; xCoreID++) {
#اگر ( CONFIG_FREERTOS_SMP == 1 )
        x_Task_Create_Affinity_سیٹ(test_func_task,
                               "task",
                               ul_ڈھیر_گہرائی,
                               (خالی *) &x_Test_دلائل,
                               ux_ترجیح,
                               (UBaseType_t)(1 << xCoreID),
                               &(xTaskHandles[ xCoreID ]));
#ورنہ
        x_Task_Create_Pinned_مقصد_Core(test_func_task,
                                "task",
                                ul_ڈھیر_گہرائی,
                                (خالی *) &x_Test_دلائل,
                                ux_ترجیح,
                                &(xTaskHandles[ xCoreID ]),
                                xCoreID);
#اگر_ختم
    }

    /* Wait for each tasks to complete test */
    برائے (بنیاد_Type_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_نمبر_کا_CORES; xCoreID++) {
        xSemaphoreTake(x_Task_مکمل_Sem, پورٹ_MAX_DELAY);
    }

    /* Cleanup */
    برائے (بنیاد_Type_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_نمبر_کا_CORES; xCoreID++) {
        vTaskDelete(xTaskHandles[ xCoreID ]);
    }
    vSemaphoreDelete(x_Task_مکمل_Sem);
    vTaskDelay(10); // Short delay to allow task memory to be freed
}

#اگر_ختم /* ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) */
