/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن "sdkconfig.h"
#ضمن "FreeRTOS.h"
#ضمن "semphr.h"
#ضمن "freertos_test_utils.h"

#إذا ( CONFIG_FREERTOS_عدد_از_CORES > 1 )

تعريفنوع بنية {
    ثابت TestFunction_t px_Test_کد;
    فراغ * ثابت pv_Test_کد_Arg;
    ثابت SemaphoreHandle_t x_Task_انجامشده_Sem;
} Test_آرگومانها_t;

ساكن فراغ test_func_task(فراغ * pvParameters)
{
    Test_آرگومانها_t * px_Test_آرگومانها = (Test_آرگومانها_t *) pvParameters;
    /* Call the test function */
    px_Test_آرگومانها->px_Test_کد(px_Test_آرگومانها->pv_Test_کد_Arg);
    /* Indicate completion to the creation task and wait to be deleted. */
    xSemaphoreGive(px_Test_آرگومانها->x_Task_انجامشده_Sem);
    vTaskSuspend(عدم);
}

فراغ vTestOnAllCores(TestFunction_t px_Test_کد, فراغ * pv_Test_کد_Arg, uint32_t ul_پشته_عمق, UBaseType_t ux_اولویت)
{
    SemaphoreHandle_t x_Task_انجامشده_Sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_عدد_از_CORES, 0);
    TaskHandle_t xTaskHandles[ CONFIG_FREERTOS_عدد_از_CORES ];
    Test_آرگومانها_t x_Test_آرگومانها = {
        .px_Test_کد = px_Test_کد,
        .pv_Test_کد_Arg = pv_Test_کد_Arg,
        .x_Task_انجامشده_Sem = x_Task_انجامشده_Sem,
    };

    /* Create a separate task on each core to run the test function */
    لكل (پایه_Type_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_عدد_از_CORES; xCoreID++) {
#إذا ( CONFIG_FREERTOS_SMP == 1 )
        x_Task_Create_Affinity_مجموعه(test_func_task,
                               "task",
                               ul_پشته_عمق,
                               (فراغ *) &x_Test_آرگومانها,
                               ux_اولویت,
                               (UBaseType_t)(1 << xCoreID),
                               &(xTaskHandles[ xCoreID ]));
#وإلا
        x_Task_Create_Pinned_مقصد_Core(test_func_task,
                                "task",
                                ul_پشته_عمق,
                                (فراغ *) &x_Test_آرگومانها,
                                ux_اولویت,
                                &(xTaskHandles[ xCoreID ]),
                                xCoreID);
#پایان_اگر
    }

    /* Wait for each tasks to complete test */
    لكل (پایه_Type_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_عدد_از_CORES; xCoreID++) {
        xSemaphoreTake(x_Task_انجامشده_Sem, پورت_MAX_DELAY);
    }

    /* Cleanup */
    لكل (پایه_Type_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_عدد_از_CORES; xCoreID++) {
        vTaskDelete(xTaskHandles[ xCoreID ]);
    }
    vSemaphoreDelete(x_Task_انجامشده_Sem);
    vTaskDelay(10); // Short delay to allow task memory to be freed
}

#پایان_اگر /* ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) */
