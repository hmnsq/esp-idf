/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن "sdkconfig.h"

/*
Note: We disable this test when using the FreeRTOS SMP kernel as the port will already provide
a definition for vApplicationTickHook(). Thus this test cannot be run.
*/
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن <freertos/semphr.h>
#ضمن "unity.h"
#ضمن "test_utils.h"

#إذا !CONFIG_FREERTOS_SMP
/*
Test FreeRTOS idle hook. Only compiled in if FreeRTOS idle hooks are enabled.
*/
#إذا CONFIG_FREERTOS_USE_IDLE_HOOK

ساكن متطاير unsigned idle_تعداد;

فراغ vApplicationIdleHook(فراغ)
{
    idle_تعداد++;
}

TEST_CASE("FreeRTOS idle hook", "[freertos]")
{
    idle_تعداد = 0;
    vTaskDelay(10);
    TEST_ASSERT_NOT_EQUAL(0, idle_تعداد); // The legacy idle hook should be called at least once
}

#endif // CONFIG_FREERTOS_USE_IDLE_HOOK

/*
Test the FreeRTOS tick hook. Only compiled in if FreeRTOS tick hooks are enabled.
*/
#إذا CONFIG_FREERTOS_USE_TICK_HOOK

ساكن متطاير unsigned tick_تعداد;

فراغ vApplicationTickHook(فراغ)
{
    tick_تعداد++;
}

TEST_CASE("FreeRTOS tick hook", "[freertos]")
{
    unsigned before = x_Task_Get_Tick_تعداد();
    ثابت unsigned SLEEP_FOR = 20;
    tick_تعداد = before;
    vTaskDelay(SLEEP_FOR);
    TEST_ASSERT_UINT32_WITHIN_MESSAGE(3 * CONFIG_FREERTOS_العدد_OF_CORES, before + SLEEP_FOR * CONFIG_FREERTOS_العدد_OF_CORES, tick_تعداد,
                                      "The FreeRTOS tick hook should have been called approx 1 time per tick per CPU");
}

#endif // CONFIG_FREERTOS_USE_TICK_HOOK
#endif // !CONFIG_FREERTOS_SMP

#إذا CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK

ساكن متطاير فراغ *deleted_tcb;

ساكن فراغ taskDeletesItself(فراغ *ignored)
{
    vTaskDelete(عدم);
}

فراغ vTaskPreDeletionHook(فراغ *pxTCB)
{
    deleted_tcb = pxTCB;
}

TEST_CASE("static task cleanup hook is called based on config", "[freertos]")
{
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_العدد_OF_CORES; i++) {
        printf("Creating task CPU %d\n", i);
        TaskHandle_t new_task = عدم;
        deleted_tcb = عدم;
        xTaskCreatePinnedToCore(taskDeletesItself, "delete", 2048, عدم, UNITY_FREERTOS_PRIORITY, &new_task, i);
        vTaskDelay(5);
        TEST_ASSERT_EQUAL_PTR(deleted_tcb, new_task); // TCB & TaskHandle are the same in FreeRTOS
    }
}

#endif // CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK
