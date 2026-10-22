/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ "FreeRTOS.h"
#అన్తర్భావ "semphr.h"
#అన్తర్భావ "freertos_test_utils.h"

#ఒకవేళ ( CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES > 1 )

రకనిర్వచనం నిర్మాణం {
    స్థిరరాశి TestFunction_t px_Test_సంకేతం;
    శూన్య * స్థిరరాశి pv_Test_సంకేతం_Arg;
    స్థిరరాశి SemaphoreHandle_t x_Task_పూర్తి_Sem;
} Test_వాదనలు_t;

స్థిర శూన్య test_func_task(శూన్య * pvParameters)
{
    Test_వాదనలు_t * px_Test_వాదనలు = (Test_వాదనలు_t *) pvParameters;
    /* Call the test function */
    px_Test_వాదనలు->px_Test_సంకేతం(px_Test_వాదనలు->pv_Test_సంకేతం_Arg);
    /* Indicate completion to the creation task and wait to be deleted. */
    xSemaphoreGive(px_Test_వాదనలు->x_Task_పూర్తి_Sem);
    vTaskSuspend(ఖాళీ);
}

శూన్య vTestOnAllCores(TestFunction_t px_Test_సంకేతం, శూన్య * pv_Test_సంకేతం_Arg, uint32_t ul_దొంతర_లోతు, UBaseType_t ux_ప్రాధాన్యత)
{
    SemaphoreHandle_t x_Task_పూర్తి_Sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES, 0);
    TaskHandle_t xTaskHandles[ CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES ];
    Test_వాదనలు_t x_Test_వాదనలు = {
        .px_Test_సంకేతం = px_Test_సంకేతం,
        .pv_Test_సంకేతం_Arg = pv_Test_సంకేతం_Arg,
        .x_Task_పూర్తి_Sem = x_Task_పూర్తి_Sem,
    };

    /* Create a separate task on each core to run the test function */
    కొరకు (పునాది_Type_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; xCoreID++) {
#ఒకవేళ ( CONFIG_FREERTOS_SMP == 1 )
        x_Task_Create_Affinity_సమితి(test_func_task,
                               "task",
                               ul_దొంతర_లోతు,
                               (శూన్య *) &x_Test_వాదనలు,
                               ux_ప్రాధాన్యత,
                               (UBaseType_t)(1 << xCoreID),
                               &(xTaskHandles[ xCoreID ]));
#లేకపోతే
        x_Task_Create_Pinned_గమ్యం_Core(test_func_task,
                                "task",
                                ul_దొంతర_లోతు,
                                (శూన్య *) &x_Test_వాదనలు,
                                ux_ప్రాధాన్యత,
                                &(xTaskHandles[ xCoreID ]),
                                xCoreID);
#అయితే_ముగింపు
    }

    /* Wait for each tasks to complete test */
    కొరకు (పునాది_Type_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; xCoreID++) {
        xSemaphoreTake(x_Task_పూర్తి_Sem, పోర్టు_MAX_DELAY);
    }

    /* Cleanup */
    కొరకు (పునాది_Type_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; xCoreID++) {
        vTaskDelete(xTaskHandles[ xCoreID ]);
    }
    vSemaphoreDelete(x_Task_పూర్తి_Sem);
    vTaskDelay(10); // Short delay to allow task memory to be freed
}

#అయితే_ముగింపు /* ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) */
