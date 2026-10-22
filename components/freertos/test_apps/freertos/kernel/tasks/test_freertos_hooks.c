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

ساكن متطاير بنانشان idle_ڳڻپ;

فراغ vApplicationIdleHook(فراغ)
{
    idle_ڳڻپ++;
}

TEST_CASE("FreeRTOS idle hook", "[freertos]")
{
    idle_ڳڻپ = 0;
    vTaskDelay(10);
    TEST_ASSERT_NOT_EQUAL(0, idle_ڳڻپ); // The legacy idle hook should be called at least once
}

#جيڪڏهن_پورو // CONFIG_FREERTOS_USE_IDLE_HOOK

/*
Test the FreeRTOS tick hook. Only compiled in if FreeRTOS tick hooks are enabled.
*/
#إذا CONFIG_FREERTOS_USE_TICK_HOOK

ساكن متطاير بنانشان tick_ڳڻپ;

فراغ vApplicationTickHook(فراغ)
{
    tick_ڳڻپ++;
}

TEST_CASE("FreeRTOS tick hook", "[freertos]")
{
    بنانشان before = x_Task_Get_Tick_ڳڻپ();
    ثابت بنانشان SLEEP_FOR = 20;
    tick_ڳڻپ = before;
    vTaskDelay(SLEEP_FOR);
    TEST_ASSERT_UINT32_WITHIN_پيغام(3 * CONFIG_FREERTOS_عدد_جو_CORES, before + SLEEP_FOR * CONFIG_FREERTOS_عدد_جو_CORES, tick_ڳڻپ,
                                      "The FreeRTOS tick hook should have been called approx 1 time per tick per CPU");
}

#جيڪڏهن_پورو // CONFIG_FREERTOS_USE_TICK_HOOK
#جيڪڏهن_پورو // !CONFIG_FREERTOS_SMP

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
    لكل (صحيح i = 0; i < CONFIG_FREERTOS_عدد_جو_CORES; i++) {
        لکو("Creating task CPU %d\n", i);
        TaskHandle_t new_task = عدم;
        deleted_tcb = عدم;
        x_Task_Create_Pinned_منزل_Core(taskDeletesItself, "delete", 2048, عدم, UNITY_FREERTOS_ترجيح, &new_task, i);
        vTaskDelay(5);
        TEST_ASSERT_EQUAL_PTR(deleted_tcb, new_task); // TCB & TaskHandle are the same in FreeRTOS
    }
}

#جيڪڏهن_پورو // CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK
