/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"

/*
Note: We disable this test when using the FreeRTOS SMP kernel as the port will already provide
a definition for vApplicationTickHook(). Thus this test cannot be run.
*/
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ <freertos/semphr.h>
#ಅನ್ತರ್ಭಾವ "unity.h"
#ಅನ್ತರ್ಭಾವ "test_utils.h"

#ಯದಿ !CONFIG_FREERTOS_SMP
/*
Test FreeRTOS idle hook. Only compiled in if FreeRTOS idle hooks are enabled.
*/
#ಯದಿ CONFIG_FREERTOS_USE_IDLE_HOOK

ಸ್ಥಿರ ಚಞ್ಚಲ unsigned idle_ಎಣಿಕೆ;

ಶೂನ್ಯ vApplicationIdleHook(ಶೂನ್ಯ)
{
    idle_ಎಣಿಕೆ++;
}

TEST_CASE("FreeRTOS idle hook", "[freertos]")
{
    idle_ಎಣಿಕೆ = 0;
    vTaskDelay(10);
    TEST_ASSERT_NOT_EQUAL(0, idle_ಎಣಿಕೆ); // The legacy idle hook should be called at least once
}

#endif // CONFIG_FREERTOS_USE_IDLE_HOOK

/*
Test the FreeRTOS tick hook. Only compiled in if FreeRTOS tick hooks are enabled.
*/
#ಯದಿ CONFIG_FREERTOS_USE_TICK_HOOK

ಸ್ಥಿರ ಚಞ್ಚಲ unsigned tick_ಎಣಿಕೆ;

ಶೂನ್ಯ vApplicationTickHook(ಶೂನ್ಯ)
{
    tick_ಎಣಿಕೆ++;
}

TEST_CASE("FreeRTOS tick hook", "[freertos]")
{
    unsigned before = x_Task_Get_Tick_ಎಣಿಕೆ();
    ಧ್ರುವ unsigned SLEEP_FOR = 20;
    tick_ಎಣಿಕೆ = before;
    vTaskDelay(SLEEP_FOR);
    TEST_ASSERT_UINT32_WITHIN_MESSAGE(3 * CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES, before + SLEEP_FOR * CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES, tick_ಎಣಿಕೆ,
                                      "The FreeRTOS tick hook should have been called approx 1 time per tick per CPU");
}

#endif // CONFIG_FREERTOS_USE_TICK_HOOK
#endif // !CONFIG_FREERTOS_SMP

#ಯದಿ CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK

ಸ್ಥಿರ ಚಞ್ಚಲ ಶೂನ್ಯ *deleted_tcb;

ಸ್ಥಿರ ಶೂನ್ಯ taskDeletesItself(ಶೂನ್ಯ *ignored)
{
    vTaskDelete(ರಿಕ್ತ);
}

ಶೂನ್ಯ vTaskPreDeletionHook(ಶೂನ್ಯ *pxTCB)
{
    deleted_tcb = pxTCB;
}

TEST_CASE("static task cleanup hook is called based on config", "[freertos]")
{
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_OF_CORES; i++) {
        printf("Creating task CPU %d\n", i);
        TaskHandle_t new_task = ರಿಕ್ತ;
        deleted_tcb = ರಿಕ್ತ;
        xTaskCreatePinnedToCore(taskDeletesItself, "delete", 2048, ರಿಕ್ತ, UNITY_FREERTOS_PRIORITY, &new_task, i);
        vTaskDelay(5);
        TEST_ASSERT_EQUAL_PTR(deleted_tcb, new_task); // TCB & TaskHandle are the same in FreeRTOS
    }
}

#endif // CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK
