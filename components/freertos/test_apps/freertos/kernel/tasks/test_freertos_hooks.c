/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#అన్తర్భావ "sdkconfig.h"

/*
Note: We disable this test when using the FreeRTOS SMP kernel as the port will already provide
a definition for vApplicationTickHook(). Thus this test cannot be run.
*/
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ <freertos/semphr.h>
#అన్తర్భావ "unity.h"
#అన్తర్భావ "test_utils.h"

#ఒకవేళ !CONFIG_FREERTOS_SMP
/*
Test FreeRTOS idle hook. Only compiled in if FreeRTOS idle hooks are enabled.
*/
#ఒకవేళ CONFIG_FREERTOS_USE_IDLE_HOOK

స్థిర అస్థిర గుర్తులేని idle_లెక్క;

శూన్య vApplicationIdleHook(శూన్య)
{
    idle_లెక్క++;
}

TEST_CASE("FreeRTOS idle hook", "[freertos]")
{
    idle_లెక్క = 0;
    vTaskDelay(10);
    TEST_ASSERT_NOT_EQUAL(0, idle_లెక్క); // The legacy idle hook should be called at least once
}

#అయితే_ముగింపు // CONFIG_FREERTOS_USE_IDLE_HOOK

/*
Test the FreeRTOS tick hook. Only compiled in if FreeRTOS tick hooks are enabled.
*/
#ఒకవేళ CONFIG_FREERTOS_USE_TICK_HOOK

స్థిర అస్థిర గుర్తులేని tick_లెక్క;

శూన్య vApplicationTickHook(శూన్య)
{
    tick_లెక్క++;
}

TEST_CASE("FreeRTOS tick hook", "[freertos]")
{
    గుర్తులేని before = x_Task_Get_Tick_లెక్క();
    స్థిరరాశి గుర్తులేని SLEEP_FOR = 20;
    tick_లెక్క = before;
    vTaskDelay(SLEEP_FOR);
    TEST_ASSERT_UINT32_WITHIN_సందేశం(3 * CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES, before + SLEEP_FOR * CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES, tick_లెక్క,
                                      "The FreeRTOS tick hook should have been called approx 1 time per tick per CPU");
}

#అయితే_ముగింపు // CONFIG_FREERTOS_USE_TICK_HOOK
#అయితే_ముగింపు // !CONFIG_FREERTOS_SMP

#ఒకవేళ CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK

స్థిర అస్థిర శూన్య *deleted_tcb;

స్థిర శూన్య taskDeletesItself(శూన్య *ignored)
{
    vTaskDelete(ఖాళీ);
}

శూన్య vTaskPreDeletionHook(శూన్య *pxTCB)
{
    deleted_tcb = pxTCB;
}

TEST_CASE("static task cleanup hook is called based on config", "[freertos]")
{
    కొరకు (పూర్ణాంకం i = 0; i < CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES; i++) {
        రాయి("Creating task CPU %d\n", i);
        TaskHandle_t new_task = ఖాళీ;
        deleted_tcb = ఖాళీ;
        x_Task_Create_Pinned_గమ్యం_Core(taskDeletesItself, "delete", 2048, ఖాళీ, UNITY_FREERTOS_ప్రాధాన్యత, &new_task, i);
        vTaskDelay(5);
        TEST_ASSERT_EQUAL_PTR(deleted_tcb, new_task); // TCB & TaskHandle are the same in FreeRTOS
    }
}

#అయితే_ముగింపు // CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK
