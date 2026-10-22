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

ساکن متلون غیرعلامتی idle_گنتی;

خالی vApplicationIdleHook(خالی)
{
    idle_گنتی++;
}

TEST_CASE("FreeRTOS idle hook", "[freertos]")
{
    idle_گنتی = 0;
    vTaskDelay(10);
    TEST_ASSERT_NOT_EQUAL(0, idle_گنتی); // The legacy idle hook should be called at least once
}

#اگر_ختم // CONFIG_FREERTOS_USE_IDLE_HOOK

/*
Test the FreeRTOS tick hook. Only compiled in if FreeRTOS tick hooks are enabled.
*/
#اگر CONFIG_FREERTOS_USE_TICK_HOOK

ساکن متلون غیرعلامتی tick_گنتی;

خالی vApplicationTickHook(خالی)
{
    tick_گنتی++;
}

TEST_CASE("FreeRTOS tick hook", "[freertos]")
{
    غیرعلامتی before = x_Task_Get_Tick_گنتی();
    ثابت غیرعلامتی SLEEP_FOR = 20;
    tick_گنتی = before;
    vTaskDelay(SLEEP_FOR);
    TEST_ASSERT_UINT32_WITHIN_پیغام(3 * CONFIG_FREERTOS_نمبر_کا_CORES, before + SLEEP_FOR * CONFIG_FREERTOS_نمبر_کا_CORES, tick_گنتی,
                                      "The FreeRTOS tick hook should have been called approx 1 time per tick per CPU");
}

#اگر_ختم // CONFIG_FREERTOS_USE_TICK_HOOK
#اگر_ختم // !CONFIG_FREERTOS_SMP

#اگر CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK

ساکن متلون خالی *deleted_tcb;

ساکن خالی taskDeletesItself(خالی *ignored)
{
    vTaskDelete(تہی);
}

خالی vTaskPreDeletionHook(خالی *pxTCB)
{
    deleted_tcb = pxTCB;
}

TEST_CASE("static task cleanup hook is called based on config", "[freertos]")
{
    برائے (صحیح i = 0; i < CONFIG_FREERTOS_نمبر_کا_CORES; i++) {
        لکھو("Creating task CPU %d\n", i);
        TaskHandle_t new_task = تہی;
        deleted_tcb = تہی;
        x_Task_Create_Pinned_مقصد_Core(taskDeletesItself, "delete", 2048, تہی, UNITY_FREERTOS_ترجیح, &new_task, i);
        vTaskDelay(5);
        TEST_ASSERT_EQUAL_PTR(deleted_tcb, new_task); // TCB & TaskHandle are the same in FreeRTOS
    }
}

#اگر_ختم // CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK
