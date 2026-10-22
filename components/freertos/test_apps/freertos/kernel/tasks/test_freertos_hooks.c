/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#અન્તર્ભાવ "sdkconfig.h"

/*
Note: We disable this test when using the FreeRTOS SMP kernel as the port will already provide
a definition for vApplicationTickHook(). Thus this test cannot be run.
*/
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ "freertos/FreeRTOS.h"
#અન્તર્ભાવ "freertos/task.h"
#અન્તર્ભાવ <freertos/semphr.h>
#અન્તર્ભાવ "unity.h"
#અન્તર્ભાવ "test_utils.h"

#જો !CONFIG_FREERTOS_SMP
/*
Test FreeRTOS idle hook. Only compiled in if FreeRTOS idle hooks are enabled.
*/
#જો CONFIG_FREERTOS_USE_IDLE_HOOK

સ્થિર અસ્થિર unsigned idle_ગણતરી;

શૂન્ય vApplicationIdleHook(શૂન્ય)
{
    idle_ગણતરી++;
}

TEST_CASE("FreeRTOS idle hook", "[freertos]")
{
    idle_ગણતરી = 0;
    vTaskDelay(10);
    TEST_ASSERT_NOT_EQUAL(0, idle_ગણતરી); // The legacy idle hook should be called at least once
}

#endif // CONFIG_FREERTOS_USE_IDLE_HOOK

/*
Test the FreeRTOS tick hook. Only compiled in if FreeRTOS tick hooks are enabled.
*/
#જો CONFIG_FREERTOS_USE_TICK_HOOK

સ્થિર અસ્થિર unsigned tick_ગણતરી;

શૂન્ય vApplicationTickHook(શૂન્ય)
{
    tick_ગણતરી++;
}

TEST_CASE("FreeRTOS tick hook", "[freertos]")
{
    unsigned before = x_Task_Get_Tick_ગણતરી();
    અચલ unsigned SLEEP_FOR = 20;
    tick_ગણતરી = before;
    vTaskDelay(SLEEP_FOR);
    TEST_ASSERT_UINT32_WITHIN_MESSAGE(3 * CONFIG_FREERTOS_સંખ્યા_OF_CORES, before + SLEEP_FOR * CONFIG_FREERTOS_સંખ્યા_OF_CORES, tick_ગણતરી,
                                      "The FreeRTOS tick hook should have been called approx 1 time per tick per CPU");
}

#endif // CONFIG_FREERTOS_USE_TICK_HOOK
#endif // !CONFIG_FREERTOS_SMP

#જો CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK

સ્થિર અસ્થિર શૂન્ય *deleted_tcb;

સ્થિર શૂન્ય taskDeletesItself(શૂન્ય *ignored)
{
    vTaskDelete(ખાલી);
}

શૂન્ય vTaskPreDeletionHook(શૂન્ય *pxTCB)
{
    deleted_tcb = pxTCB;
}

TEST_CASE("static task cleanup hook is called based on config", "[freertos]")
{
    માટે (પૂર્ણાંક i = 0; i < CONFIG_FREERTOS_સંખ્યા_OF_CORES; i++) {
        printf("Creating task CPU %d\n", i);
        TaskHandle_t new_task = ખાલી;
        deleted_tcb = ખાલી;
        xTaskCreatePinnedToCore(taskDeletesItself, "delete", 2048, ખાલી, UNITY_FREERTOS_PRIORITY, &new_task, i);
        vTaskDelay(5);
        TEST_ASSERT_EQUAL_PTR(deleted_tcb, new_task); // TCB & TaskHandle are the same in FreeRTOS
    }
}

#endif // CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK
