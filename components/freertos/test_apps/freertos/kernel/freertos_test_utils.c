/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "semphr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos_test_utils.h"

#ਜੇ ( CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES > 1 )

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ {
    ਅਚਲ TestFunction_t px_Test_ਕੋਡ;
    ਖਾਲੀ * ਅਚਲ pv_Test_ਕੋਡ_Arg;
    ਅਚਲ SemaphoreHandle_t x_Task_ਪੂਰਾ_Sem;
} Test_ਦਲੀਲਾਂ_t;

ਸਥਿਰ ਖਾਲੀ test_func_task(ਖਾਲੀ * pvParameters)
{
    Test_ਦਲੀਲਾਂ_t * px_Test_ਦਲੀਲਾਂ = (Test_ਦਲੀਲਾਂ_t *) pvParameters;
    /* Call the test function */
    px_Test_ਦਲੀਲਾਂ->px_Test_ਕੋਡ(px_Test_ਦਲੀਲਾਂ->pv_Test_ਕੋਡ_Arg);
    /* Indicate completion to the creation task and wait to be deleted. */
    xSemaphoreGive(px_Test_ਦਲੀਲਾਂ->x_Task_ਪੂਰਾ_Sem);
    vTaskSuspend(ਨਿਰਾ);
}

ਖਾਲੀ vTestOnAllCores(TestFunction_t px_Test_ਕੋਡ, ਖਾਲੀ * pv_Test_ਕੋਡ_Arg, uint32_t ul_ਢੇਰ_ਡੂੰਘਾਈ, UBaseType_t ux_ਤਰਜੀਹ)
{
    SemaphoreHandle_t x_Task_ਪੂਰਾ_Sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES, 0);
    TaskHandle_t xTaskHandles[ CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES ];
    Test_ਦਲੀਲਾਂ_t x_Test_ਦਲੀਲਾਂ = {
        .px_Test_ਕੋਡ = px_Test_ਕੋਡ,
        .pv_Test_ਕੋਡ_Arg = pv_Test_ਕੋਡ_Arg,
        .x_Task_ਪੂਰਾ_Sem = x_Task_ਪੂਰਾ_Sem,
    };

    /* Create a separate task on each core to run the test function */
    ਲਈ (ਅਧਾਰ_Type_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; xCoreID++) {
#ਜੇ ( CONFIG_FREERTOS_SMP == 1 )
        x_Task_Create_Affinity_ਸੈੱਟ(test_func_task,
                               "task",
                               ul_ਢੇਰ_ਡੂੰਘਾਈ,
                               (ਖਾਲੀ *) &x_Test_ਦਲੀਲਾਂ,
                               ux_ਤਰਜੀਹ,
                               (UBaseType_t)(1 << xCoreID),
                               &(xTaskHandles[ xCoreID ]));
#ਨਹੀਂਤਾਂ
        x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(test_func_task,
                                "task",
                                ul_ਢੇਰ_ਡੂੰਘਾਈ,
                                (ਖਾਲੀ *) &x_Test_ਦਲੀਲਾਂ,
                                ux_ਤਰਜੀਹ,
                                &(xTaskHandles[ xCoreID ]),
                                xCoreID);
#ਜੇ_ਅੰਤ
    }

    /* Wait for each tasks to complete test */
    ਲਈ (ਅਧਾਰ_Type_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; xCoreID++) {
        xSemaphoreTake(x_Task_ਪੂਰਾ_Sem, ਪੋਰਟ_MAX_DELAY);
    }

    /* Cleanup */
    ਲਈ (ਅਧਾਰ_Type_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; xCoreID++) {
        vTaskDelete(xTaskHandles[ xCoreID ]);
    }
    vSemaphoreDelete(x_Task_ਪੂਰਾ_Sem);
    vTaskDelay(10); // Short delay to allow task memory to be freed
}

#ਜੇ_ਅੰਤ /* ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) */
