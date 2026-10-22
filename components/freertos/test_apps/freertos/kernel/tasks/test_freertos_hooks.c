/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#অন্তর্ভাব "sdkconfig.h"

/*
Note: We disable this test when using the FreeRTOS SMP kernel as the port will already provide
a definition for vApplicationTickHook(). Thus this test cannot be run.
*/
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব "freertos/FreeRTOS.h"
#অন্তর্ভাব "freertos/task.h"
#অন্তর্ভাব <freertos/semphr.h>
#অন্তর্ভাব "unity.h"
#অন্তর্ভাব "test_utils.h"

#যদি !CONFIG_FREERTOS_SMP
/*
Test FreeRTOS idle hook. Only compiled in if FreeRTOS idle hooks are enabled.
*/
#যদি CONFIG_FREERTOS_USE_IDLE_HOOK

স্থির চঞ্চল unsigned idle_গুনতি;

শূন্য vApplicationIdleHook(শূন্য)
{
    idle_গুনতি++;
}

TEST_CASE("FreeRTOS idle hook", "[freertos]")
{
    idle_গুনতি = 0;
    vTaskDelay(10);
    TEST_ASSERT_NOT_EQUAL(0, idle_গুনতি); // The legacy idle hook should be called at least once
}

#endif // CONFIG_FREERTOS_USE_IDLE_HOOK

/*
Test the FreeRTOS tick hook. Only compiled in if FreeRTOS tick hooks are enabled.
*/
#যদি CONFIG_FREERTOS_USE_TICK_HOOK

স্থির চঞ্চল unsigned tick_গুনতি;

শূন্য vApplicationTickHook(শূন্য)
{
    tick_গুনতি++;
}

TEST_CASE("FreeRTOS tick hook", "[freertos]")
{
    unsigned before = x_Task_Get_Tick_গুনতি();
    ধ্রুব unsigned SLEEP_FOR = 20;
    tick_গুনতি = before;
    vTaskDelay(SLEEP_FOR);
    TEST_ASSERT_UINT32_WITHIN_MESSAGE(3 * CONFIG_FREERTOS_সংখ্যা_OF_CORES, before + SLEEP_FOR * CONFIG_FREERTOS_সংখ্যা_OF_CORES, tick_গুনতি,
                                      "The FreeRTOS tick hook should have been called approx 1 time per tick per CPU");
}

#endif // CONFIG_FREERTOS_USE_TICK_HOOK
#endif // !CONFIG_FREERTOS_SMP

#যদি CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK

স্থির চঞ্চল শূন্য *deleted_tcb;

স্থির শূন্য taskDeletesItself(শূন্য *ignored)
{
    vTaskDelete(রিক্ত);
}

শূন্য vTaskPreDeletionHook(শূন্য *pxTCB)
{
    deleted_tcb = pxTCB;
}

TEST_CASE("static task cleanup hook is called based on config", "[freertos]")
{
    প্রতি (পূর্ণাঙ্ক i = 0; i < CONFIG_FREERTOS_সংখ্যা_OF_CORES; i++) {
        printf("Creating task CPU %d\n", i);
        TaskHandle_t new_task = রিক্ত;
        deleted_tcb = রিক্ত;
        xTaskCreatePinnedToCore(taskDeletesItself, "delete", 2048, রিক্ত, UNITY_FREERTOS_PRIORITY, &new_task, i);
        vTaskDelay(5);
        TEST_ASSERT_EQUAL_PTR(deleted_tcb, new_task); // TCB & TaskHandle are the same in FreeRTOS
    }
}

#endif // CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK
