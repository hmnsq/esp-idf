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

#ಆದರೆ !CONFIG_FREERTOS_SMP
/*
Test FreeRTOS idle hook. Only compiled in if FreeRTOS idle hooks are enabled.
*/
#ಆದರೆ CONFIG_FREERTOS_USE_IDLE_HOOK

ಸ್ಥಿರ ಅಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ idle_ಎಣಿಕೆ;

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

#ಆದರೆ_ಅಂತ್ಯ // CONFIG_FREERTOS_USE_IDLE_HOOK

/*
Test the FreeRTOS tick hook. Only compiled in if FreeRTOS tick hooks are enabled.
*/
#ಆದರೆ CONFIG_FREERTOS_USE_TICK_HOOK

ಸ್ಥಿರ ಅಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ tick_ಎಣಿಕೆ;

ಶೂನ್ಯ vApplicationTickHook(ಶೂನ್ಯ)
{
    tick_ಎಣಿಕೆ++;
}

TEST_CASE("FreeRTOS tick hook", "[freertos]")
{
    ಚಿಹ್ನೆರಹಿತ before = x_Task_Get_Tick_ಎಣಿಕೆ();
    ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ SLEEP_FOR = 20;
    tick_ಎಣಿಕೆ = before;
    vTaskDelay(SLEEP_FOR);
    TEST_ASSERT_UINT32_WITHIN_ಸಂದೇಶ(3 * CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES, before + SLEEP_FOR * CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES, tick_ಎಣಿಕೆ,
                                      "The FreeRTOS tick hook should have been called approx 1 time per tick per CPU");
}

#ಆದರೆ_ಅಂತ್ಯ // CONFIG_FREERTOS_USE_TICK_HOOK
#ಆದರೆ_ಅಂತ್ಯ // !CONFIG_FREERTOS_SMP

#ಆದರೆ CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK

ಸ್ಥಿರ ಅಸ್ಥಿರ ಶೂನ್ಯ *deleted_tcb;

ಸ್ಥಿರ ಶೂನ್ಯ taskDeletesItself(ಶೂನ್ಯ *ignored)
{
    vTaskDelete(ಖಾಲಿ);
}

ಶೂನ್ಯ vTaskPreDeletionHook(ಶೂನ್ಯ *pxTCB)
{
    deleted_tcb = pxTCB;
}

TEST_CASE("static task cleanup hook is called based on config", "[freertos]")
{
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES; i++) {
        ಬರೆ("Creating task CPU %d\n", i);
        TaskHandle_t new_task = ಖಾಲಿ;
        deleted_tcb = ಖಾಲಿ;
        x_Task_Create_Pinned_ಗಮ್ಯ_Core(taskDeletesItself, "delete", 2048, ಖಾಲಿ, UNITY_FREERTOS_ಆದ್ಯತೆ, &new_task, i);
        vTaskDelay(5);
        TEST_ASSERT_EQUAL_PTR(deleted_tcb, new_task); // TCB & TaskHandle are the same in FreeRTOS
    }
}

#ಆದರೆ_ಅಂತ್ಯ // CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK
