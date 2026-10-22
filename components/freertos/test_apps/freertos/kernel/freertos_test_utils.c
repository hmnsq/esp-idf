/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ "FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "semphr.h"
#ಅನ್ತರ್ಭಾವ "freertos_test_utils.h"

#ಆದರೆ ( CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES > 1 )

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ {
    ಸ್ಥಿರಾಂಕ TestFunction_t px_Test_ಸಂಕೇತ;
    ಶೂನ್ಯ * ಸ್ಥಿರಾಂಕ pv_Test_ಸಂಕೇತ_Arg;
    ಸ್ಥಿರಾಂಕ SemaphoreHandle_t x_Task_ಮುಗಿದ_Sem;
} Test_ವಾದಗಳು_t;

ಸ್ಥಿರ ಶೂನ್ಯ test_func_task(ಶೂನ್ಯ * pvParameters)
{
    Test_ವಾದಗಳು_t * px_Test_ವಾದಗಳು = (Test_ವಾದಗಳು_t *) pvParameters;
    /* Call the test function */
    px_Test_ವಾದಗಳು->px_Test_ಸಂಕೇತ(px_Test_ವಾದಗಳು->pv_Test_ಸಂಕೇತ_Arg);
    /* Indicate completion to the creation task and wait to be deleted. */
    xSemaphoreGive(px_Test_ವಾದಗಳು->x_Task_ಮುಗಿದ_Sem);
    vTaskSuspend(ಖಾಲಿ);
}

ಶೂನ್ಯ vTestOnAllCores(TestFunction_t px_Test_ಸಂಕೇತ, ಶೂನ್ಯ * pv_Test_ಸಂಕೇತ_Arg, uint32_t ul_ರಾಶಿ_ಆಳ, UBaseType_t ux_ಆದ್ಯತೆ)
{
    SemaphoreHandle_t x_Task_ಮುಗಿದ_Sem = xSemaphoreCreateCounting(CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES, 0);
    TaskHandle_t xTaskHandles[ CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES ];
    Test_ವಾದಗಳು_t x_Test_ವಾದಗಳು = {
        .px_Test_ಸಂಕೇತ = px_Test_ಸಂಕೇತ,
        .pv_Test_ಸಂಕೇತ_Arg = pv_Test_ಸಂಕೇತ_Arg,
        .x_Task_ಮುಗಿದ_Sem = x_Task_ಮುಗಿದ_Sem,
    };

    /* Create a separate task on each core to run the test function */
    ಗಾಗಿ (ತಳ_Type_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; xCoreID++) {
#ಆದರೆ ( CONFIG_FREERTOS_SMP == 1 )
        x_Task_Create_Affinity_ಗಣ(test_func_task,
                               "task",
                               ul_ರಾಶಿ_ಆಳ,
                               (ಶೂನ್ಯ *) &x_Test_ವಾದಗಳು,
                               ux_ಆದ್ಯತೆ,
                               (UBaseType_t)(1 << xCoreID),
                               &(xTaskHandles[ xCoreID ]));
#ಇಲ್ಲದಿದ್ದರೆ
        x_Task_Create_Pinned_ಗಮ್ಯ_Core(test_func_task,
                                "task",
                                ul_ರಾಶಿ_ಆಳ,
                                (ಶೂನ್ಯ *) &x_Test_ವಾದಗಳು,
                                ux_ಆದ್ಯತೆ,
                                &(xTaskHandles[ xCoreID ]),
                                xCoreID);
#ಆದರೆ_ಅಂತ್ಯ
    }

    /* Wait for each tasks to complete test */
    ಗಾಗಿ (ತಳ_Type_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; xCoreID++) {
        xSemaphoreTake(x_Task_ಮುಗಿದ_Sem, ಪೋರ್ಟ್_MAX_DELAY);
    }

    /* Cleanup */
    ಗಾಗಿ (ತಳ_Type_t xCoreID = 0; xCoreID < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; xCoreID++) {
        vTaskDelete(xTaskHandles[ xCoreID ]);
    }
    vSemaphoreDelete(x_Task_ಮುಗಿದ_Sem);
    vTaskDelay(10); // Short delay to allow task memory to be freed
}

#ಆದರೆ_ಅಂತ್ಯ /* ( CONFIG_FREERTOS_NUMBER_OF_CORES > 1 ) */
