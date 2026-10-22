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

ساكن متطاير بینښه idle_شمېر;

فراغ vApplicationIdleHook(فراغ)
{
    idle_شمېر++;
}

TEST_CASE("FreeRTOS idle hook", "[freertos]")
{
    idle_شمېر = 0;
    vTaskDelay(10);
    TEST_ASSERT_NOT_EQUAL(0, idle_شمېر); // The legacy idle hook should be called at least once
}

#د_که_پای // CONFIG_FREERTOS_USE_IDLE_HOOK

/*
Test the FreeRTOS tick hook. Only compiled in if FreeRTOS tick hooks are enabled.
*/
#إذا CONFIG_FREERTOS_USE_TICK_HOOK

ساكن متطاير بینښه tick_شمېر;

فراغ vApplicationTickHook(فراغ)
{
    tick_شمېر++;
}

TEST_CASE("FreeRTOS tick hook", "[freertos]")
{
    بینښه before = x_Task_Get_Tick_شمېر();
    ثابت بینښه SLEEP_FOR = 20;
    tick_شمېر = before;
    vTaskDelay(SLEEP_FOR);
    TEST_ASSERT_UINT32_WITHIN_پیغام(3 * CONFIG_FREERTOS_شمېره_د_CORES, before + SLEEP_FOR * CONFIG_FREERTOS_شمېره_د_CORES, tick_شمېر,
                                      "The FreeRTOS tick hook should have been called approx 1 time per tick per CPU");
}

#د_که_پای // CONFIG_FREERTOS_USE_TICK_HOOK
#د_که_پای // !CONFIG_FREERTOS_SMP

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
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_شمېره_د_CORES; i++) {
        چاپ_کړه("Creating task CPU %d\n", i);
        TaskHandle_t new_task = عدم;
        deleted_tcb = عدم;
        x_Task_Create_Pinned_منزل_Core(taskDeletesItself, "delete", 2048, عدم, UNITY_FREERTOS_لومړیتوب, &new_task, i);
        vTaskDelay(5);
        TEST_ASSERT_EQUAL_PTR(deleted_tcb, new_task); // TCB & TaskHandle are the same in FreeRTOS
    }
}

#د_که_پای // CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK
