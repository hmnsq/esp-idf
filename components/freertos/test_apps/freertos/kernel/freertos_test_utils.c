/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن "sdkconfig.h"
#ضمن "FreeRTOS.h"
#ضمن "semphr.h"
#ضمن "freertos_test_utils.h"

#إذا ( CONFIG_FREERTOS_شمېره_د_CORES > 1 )

تعريفنوع بنية {
    ثابت TestFunction_t px_Test_کوډ;
    فراغ * ثابت pv_Test_کوډ_Arg;
    ثابت SemaphoreHandle_t x_Task_بشپړ_Sem;
} Test_دلیلونه_t;

ساكن فراغ test_func_task(فراغ * pvParameters)
{
    Test_دلیلونه_t * px_Test_دلیلونه = (Test_دلیلونه_t *) pvParameters;
    /* Call the test function */
    px_Test_دلیلونه->px_Test_کوډ(px_Test_دلیلونه->pv_Test_کوډ_Arg);
    /* Indicate completion to the creation task and wait to be deleted. */
    xSemaphoreGive(px_Test_دلیلونه->x_Task_بشپړ_Sem);
    vTaskSuspend(عدم);
}

فراغ vTestOnAllCores(TestFunction_t px_Test_کوډ, فراغ * pv_Test_کوډ_Arg, uint32_t ul_ډېری_ژوروالی, UBaseType_t ux_لومړیتوب)
{
    SemaphoreHandle_t x_Task_بشپړ_Sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_شمېره_د_CORES, 0);
    TaskHandle_t xTaskHandles[ CONFIG_FREERTOS_شمېره_د_CORES ];
    Test_دلیلونه_t x_Test_دلیلونه = {
        .px_Test_کوډ = px_Test_کوډ,
        .pv_Test_کوډ_Arg = pv_Test_کوډ_Arg,
        .x_Task_بشپړ_Sem = x_Task_بشپړ_Sem,
    };

    /* Create a separate task on each core to run the test function */
    لكل (بنسټ_Type_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_شمېره_د_CORES; xCoreID++) {
#إذا ( CONFIG_FREERTOS_SMP == 1 )
        x_Task_Create_Affinity_سېټ(test_func_task,
                               "task",
                               ul_ډېری_ژوروالی,
                               (فراغ *) &x_Test_دلیلونه,
                               ux_لومړیتوب,
                               (UBaseType_t)(1 << xCoreID),
                               &(xTaskHandles[ xCoreID ]));
#وإلا
        x_Task_Create_Pinned_منزل_Core(test_func_task,
                                "task",
                                ul_ډېری_ژوروالی,
                                (فراغ *) &x_Test_دلیلونه,
                                ux_لومړیتوب,
                                &(xTaskHandles[ xCoreID ]),
                                xCoreID);
#د_که_پای
    }

    /* Wait for each tasks to complete test */
    لكل (بنسټ_Type_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_شمېره_د_CORES; xCoreID++) {
        xSemaphoreTake(x_Task_بشپړ_Sem, پورټ_MAX_DELAY);
    }

    /* Cleanup */
    لكل (بنسټ_Type_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_شمېره_د_CORES; xCoreID++) {
        vTaskDelete(xTaskHandles[ xCoreID ]);
    }
    vSemaphoreDelete(x_Task_بشپړ_Sem);
    vTaskDelay(10); // Short delay to allow task memory to be freed
}

#د_که_پای /* ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) */
