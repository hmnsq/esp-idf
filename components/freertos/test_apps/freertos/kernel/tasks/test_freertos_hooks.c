/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#അന്തര്ഭാവ "sdkconfig.h"

/*
Note: We disable this test when using the FreeRTOS SMP kernel as the port will already provide
a definition for vApplicationTickHook(). Thus this test cannot be run.
*/
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ "freertos/FreeRTOS.h"
#അന്തര്ഭാവ "freertos/task.h"
#അന്തര്ഭാവ <freertos/semphr.h>
#അന്തര്ഭാവ "unity.h"
#അന്തര്ഭാവ "test_utils.h"

#എങ്കിൽ !CONFIG_FREERTOS_SMP
/*
Test FreeRTOS idle hook. Only compiled in if FreeRTOS idle hooks are enabled.
*/
#എങ്കിൽ CONFIG_FREERTOS_USE_IDLE_HOOK

സ്ഥിതം അസ്ഥിര unsigned idle_എണ്ണം;

ശൂന്യം vApplicationIdleHook(ശൂന്യം)
{
    idle_എണ്ണം++;
}

TEST_CASE("FreeRTOS idle hook", "[freertos]")
{
    idle_എണ്ണം = 0;
    vTaskDelay(10);
    TEST_ASSERT_NOT_EQUAL(0, idle_എണ്ണം); // The legacy idle hook should be called at least once
}

#endif // CONFIG_FREERTOS_USE_IDLE_HOOK

/*
Test the FreeRTOS tick hook. Only compiled in if FreeRTOS tick hooks are enabled.
*/
#എങ്കിൽ CONFIG_FREERTOS_USE_TICK_HOOK

സ്ഥിതം അസ്ഥിര unsigned tick_എണ്ണം;

ശൂന്യം vApplicationTickHook(ശൂന്യം)
{
    tick_എണ്ണം++;
}

TEST_CASE("FreeRTOS tick hook", "[freertos]")
{
    unsigned before = x_Task_Get_Tick_എണ്ണം();
    സ്ഥിരം unsigned SLEEP_FOR = 20;
    tick_എണ്ണം = before;
    vTaskDelay(SLEEP_FOR);
    TEST_ASSERT_UINT32_WITHIN_MESSAGE(3 * CONFIG_FREERTOS_സംഖ്യ_OF_CORES, before + SLEEP_FOR * CONFIG_FREERTOS_സംഖ്യ_OF_CORES, tick_എണ്ണം,
                                      "The FreeRTOS tick hook should have been called approx 1 time per tick per CPU");
}

#endif // CONFIG_FREERTOS_USE_TICK_HOOK
#endif // !CONFIG_FREERTOS_SMP

#എങ്കിൽ CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK

സ്ഥിതം അസ്ഥിര ശൂന്യം *deleted_tcb;

സ്ഥിതം ശൂന്യം taskDeletesItself(ശൂന്യം *ignored)
{
    vTaskDelete(ശൂന്യ);
}

ശൂന്യം vTaskPreDeletionHook(ശൂന്യം *pxTCB)
{
    deleted_tcb = pxTCB;
}

TEST_CASE("static task cleanup hook is called based on config", "[freertos]")
{
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < CONFIG_FREERTOS_സംഖ്യ_OF_CORES; i++) {
        printf("Creating task CPU %d\n", i);
        TaskHandle_t new_task = ശൂന്യ;
        deleted_tcb = ശൂന്യ;
        xTaskCreatePinnedToCore(taskDeletesItself, "delete", 2048, ശൂന്യ, UNITY_FREERTOS_PRIORITY, &new_task, i);
        vTaskDelay(5);
        TEST_ASSERT_EQUAL_PTR(deleted_tcb, new_task); // TCB & TaskHandle are the same in FreeRTOS
    }
}

#endif // CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK
