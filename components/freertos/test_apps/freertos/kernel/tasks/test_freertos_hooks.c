/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ଅନ୍ତର୍ଭାଵ "sdkconfig.h"

/*
Note: We disable this test when using the FreeRTOS SMP kernel as the port will already provide
a definition for vApplicationTickHook(). Thus this test cannot be run.
*/
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ "freertos/FreeRTOS.h"
#ଅନ୍ତର୍ଭାଵ "freertos/task.h"
#ଅନ୍ତର୍ଭାଵ <freertos/semphr.h>
#ଅନ୍ତର୍ଭାଵ "unity.h"
#ଅନ୍ତର୍ଭାଵ "test_utils.h"

#ଯଦି !CONFIG_FREERTOS_SMP
/*
Test FreeRTOS idle hook. Only compiled in if FreeRTOS idle hooks are enabled.
*/
#ଯଦି CONFIG_FREERTOS_USE_IDLE_HOOK

ସ୍ଥିର ଅସ୍ଥିର unsigned idle_ଗଣନା;

ଶୂନ୍ୟ vApplicationIdleHook(ଶୂନ୍ୟ)
{
    idle_ଗଣନା++;
}

TEST_CASE("FreeRTOS idle hook", "[freertos]")
{
    idle_ଗଣନା = 0;
    vTaskDelay(10);
    TEST_ASSERT_NOT_EQUAL(0, idle_ଗଣନା); // The legacy idle hook should be called at least once
}

#endif // CONFIG_FREERTOS_USE_IDLE_HOOK

/*
Test the FreeRTOS tick hook. Only compiled in if FreeRTOS tick hooks are enabled.
*/
#ଯଦି CONFIG_FREERTOS_USE_TICK_HOOK

ସ୍ଥିର ଅସ୍ଥିର unsigned tick_ଗଣନା;

ଶୂନ୍ୟ vApplicationTickHook(ଶୂନ୍ୟ)
{
    tick_ଗଣନା++;
}

TEST_CASE("FreeRTOS tick hook", "[freertos]")
{
    unsigned before = x_Task_Get_Tick_ଗଣନା();
    ଅଚଳ unsigned SLEEP_FOR = 20;
    tick_ଗଣନା = before;
    vTaskDelay(SLEEP_FOR);
    TEST_ASSERT_UINT32_WITHIN_MESSAGE(3 * CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES, before + SLEEP_FOR * CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES, tick_ଗଣନା,
                                      "The FreeRTOS tick hook should have been called approx 1 time per tick per CPU");
}

#endif // CONFIG_FREERTOS_USE_TICK_HOOK
#endif // !CONFIG_FREERTOS_SMP

#ଯଦି CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK

ସ୍ଥିର ଅସ୍ଥିର ଶୂନ୍ୟ *deleted_tcb;

ସ୍ଥିର ଶୂନ୍ୟ taskDeletesItself(ଶୂନ୍ୟ *ignored)
{
    vTaskDelete(ଖାଲି);
}

ଶୂନ୍ୟ vTaskPreDeletionHook(ଶୂନ୍ୟ *pxTCB)
{
    deleted_tcb = pxTCB;
}

TEST_CASE("static task cleanup hook is called based on config", "[freertos]")
{
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES; i++) {
        printf("Creating task CPU %d\n", i);
        TaskHandle_t new_task = ଖାଲି;
        deleted_tcb = ଖାଲି;
        xTaskCreatePinnedToCore(taskDeletesItself, "delete", 2048, ଖାଲି, UNITY_FREERTOS_PRIORITY, &new_task, i);
        vTaskDelay(5);
        TEST_ASSERT_EQUAL_PTR(deleted_tcb, new_task); // TCB & TaskHandle are the same in FreeRTOS
    }
}

#endif // CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK
