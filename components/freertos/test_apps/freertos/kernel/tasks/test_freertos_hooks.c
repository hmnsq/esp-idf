/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#شامل "sdkconfig.h"

/*
Note: We disable this test when using the FreeRTOS SMP kernel as the port will already provide
a definition for vApplicationTickHook(). Thus this test cannot be run.
*/
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل "freertos/FreeRTOS.h"
#شامل "freertos/task.h"
#شامل <freertos/semphr.h>
#شامل "unity.h"
#شامل "test_utils.h"

#اگر !CONFIG_FREERTOS_SMP
/*
Test FreeRTOS idle hook. Only compiled in if FreeRTOS idle hooks are enabled.
*/
#اگر CONFIG_FREERTOS_USE_IDLE_HOOK

ایستا فرار unsigned idle_شمار;

تهی vApplicationIdleHook(تهی)
{
    idle_شمار++;
}

TEST_CASE("FreeRTOS idle hook", "[freertos]")
{
    idle_شمار = 0;
    vTaskDelay(10);
    TEST_ASSERT_NOT_EQUAL(0, idle_شمار); // The legacy idle hook should be called at least once
}

#endif // CONFIG_FREERTOS_USE_IDLE_HOOK

/*
Test the FreeRTOS tick hook. Only compiled in if FreeRTOS tick hooks are enabled.
*/
#اگر CONFIG_FREERTOS_USE_TICK_HOOK

ایستا فرار unsigned tick_شمار;

تهی vApplicationTickHook(تهی)
{
    tick_شمار++;
}

TEST_CASE("FreeRTOS tick hook", "[freertos]")
{
    unsigned before = x_Task_Get_Tick_شمار();
    ثابت unsigned SLEEP_FOR = 20;
    tick_شمار = before;
    vTaskDelay(SLEEP_FOR);
    TEST_ASSERT_UINT32_WITHIN_MESSAGE(3 * CONFIG_FREERTOS_عدد_OF_CORES, before + SLEEP_FOR * CONFIG_FREERTOS_عدد_OF_CORES, tick_شمار,
                                      "The FreeRTOS tick hook should have been called approx 1 time per tick per CPU");
}

#endif // CONFIG_FREERTOS_USE_TICK_HOOK
#endif // !CONFIG_FREERTOS_SMP

#اگر CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK

ایستا فرار تهی *deleted_tcb;

ایستا تهی taskDeletesItself(تهی *ignored)
{
    vTaskDelete(پوچ);
}

تهی vTaskPreDeletionHook(تهی *pxTCB)
{
    deleted_tcb = pxTCB;
}

TEST_CASE("static task cleanup hook is called based on config", "[freertos]")
{
    برای (صحیح i = 0; i < CONFIG_FREERTOS_عدد_OF_CORES; i++) {
        printf("Creating task CPU %d\n", i);
        TaskHandle_t new_task = پوچ;
        deleted_tcb = پوچ;
        xTaskCreatePinnedToCore(taskDeletesItself, "delete", 2048, پوچ, UNITY_FREERTOS_PRIORITY, &new_task, i);
        vTaskDelay(5);
        TEST_ASSERT_EQUAL_PTR(deleted_tcb, new_task); // TCB & TaskHandle are the same in FreeRTOS
    }
}

#endif // CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK
