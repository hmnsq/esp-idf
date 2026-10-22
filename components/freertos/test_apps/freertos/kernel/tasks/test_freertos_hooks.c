/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"

/*
Note: We disable this test when using the FreeRTOS SMP kernel as the port will already provide
a definition for vApplicationTickHook(). Thus this test cannot be run.
*/
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <freertos/semphr.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "unity.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "test_utils.h"

#ਜੇ !CONFIG_FREERTOS_SMP
/*
Test FreeRTOS idle hook. Only compiled in if FreeRTOS idle hooks are enabled.
*/
#ਜੇ CONFIG_FREERTOS_USE_IDLE_HOOK

ਸਥਿਰ ਅਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ idle_ਗਿਣਤੀ;

ਖਾਲੀ vApplicationIdleHook(ਖਾਲੀ)
{
    idle_ਗਿਣਤੀ++;
}

TEST_CASE("FreeRTOS idle hook", "[freertos]")
{
    idle_ਗਿਣਤੀ = 0;
    vTaskDelay(10);
    TEST_ASSERT_NOT_EQUAL(0, idle_ਗਿਣਤੀ); // The legacy idle hook should be called at least once
}

#ਜੇ_ਅੰਤ // CONFIG_FREERTOS_USE_IDLE_HOOK

/*
Test the FreeRTOS tick hook. Only compiled in if FreeRTOS tick hooks are enabled.
*/
#ਜੇ CONFIG_FREERTOS_USE_TICK_HOOK

ਸਥਿਰ ਅਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ tick_ਗਿਣਤੀ;

ਖਾਲੀ vApplicationTickHook(ਖਾਲੀ)
{
    tick_ਗਿਣਤੀ++;
}

TEST_CASE("FreeRTOS tick hook", "[freertos]")
{
    ਬਿਨਾਨਿਸ਼ਾਨੀ before = x_Task_Get_Tick_ਗਿਣਤੀ();
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ SLEEP_FOR = 20;
    tick_ਗਿਣਤੀ = before;
    vTaskDelay(SLEEP_FOR);
    TEST_ASSERT_UINT32_WITHIN_ਸੁਨੇਹਾ(3 * CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES, before + SLEEP_FOR * CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES, tick_ਗਿਣਤੀ,
                                      "The FreeRTOS tick hook should have been called approx 1 time per tick per CPU");
}

#ਜੇ_ਅੰਤ // CONFIG_FREERTOS_USE_TICK_HOOK
#ਜੇ_ਅੰਤ // !CONFIG_FREERTOS_SMP

#ਜੇ CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK

ਸਥਿਰ ਅਸਥਿਰ ਖਾਲੀ *deleted_tcb;

ਸਥਿਰ ਖਾਲੀ taskDeletesItself(ਖਾਲੀ *ignored)
{
    vTaskDelete(ਨਿਰਾ);
}

ਖਾਲੀ vTaskPreDeletionHook(ਖਾਲੀ *pxTCB)
{
    deleted_tcb = pxTCB;
}

TEST_CASE("static task cleanup hook is called based on config", "[freertos]")
{
    ਲਈ (ਪੂਰਨ i = 0; i < CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES; i++) {
        ਲਿਖੋ("Creating task CPU %d\n", i);
        TaskHandle_t new_task = ਨਿਰਾ;
        deleted_tcb = ਨਿਰਾ;
        x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core(taskDeletesItself, "delete", 2048, ਨਿਰਾ, UNITY_FREERTOS_ਤਰਜੀਹ, &new_task, i);
        vTaskDelay(5);
        TEST_ASSERT_EQUAL_PTR(deleted_tcb, new_task); // TCB & TaskHandle are the same in FreeRTOS
    }
}

#ਜੇ_ਅੰਤ // CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK
