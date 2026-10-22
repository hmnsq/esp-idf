/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#அந்தர்பாவ "sdkconfig.h"

/*
Note: We disable this test when using the FreeRTOS SMP kernel as the port will already provide
a definition for vApplicationTickHook(). Thus this test cannot be run.
*/
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ "freertos/FreeRTOS.h"
#அந்தர்பாவ "freertos/task.h"
#அந்தர்பாவ <freertos/semphr.h>
#அந்தர்பாவ "unity.h"
#அந்தர்பாவ "test_utils.h"

#எனில் !CONFIG_FREERTOS_SMP
/*
Test FreeRTOS idle hook. Only compiled in if FreeRTOS idle hooks are enabled.
*/
#எனில் CONFIG_FREERTOS_USE_IDLE_HOOK

நிலையான நிலையற்ற unsigned idle_மொத்தம்;

வெறுமை vApplicationIdleHook(வெறுமை)
{
    idle_மொத்தம்++;
}

TEST_CASE("FreeRTOS idle hook", "[freertos]")
{
    idle_மொத்தம் = 0;
    vTaskDelay(10);
    TEST_ASSERT_NOT_EQUAL(0, idle_மொத்தம்); // The legacy idle hook should be called at least once
}

#endif // CONFIG_FREERTOS_USE_IDLE_HOOK

/*
Test the FreeRTOS tick hook. Only compiled in if FreeRTOS tick hooks are enabled.
*/
#எனில் CONFIG_FREERTOS_USE_TICK_HOOK

நிலையான நிலையற்ற unsigned tick_மொத்தம்;

வெறுமை vApplicationTickHook(வெறுமை)
{
    tick_மொத்தம்++;
}

TEST_CASE("FreeRTOS tick hook", "[freertos]")
{
    unsigned before = x_Task_Get_Tick_மொத்தம்();
    மாறிலி unsigned SLEEP_FOR = 20;
    tick_மொத்தம் = before;
    vTaskDelay(SLEEP_FOR);
    TEST_ASSERT_UINT32_WITHIN_MESSAGE(3 * CONFIG_FREERTOS_எண்_OF_CORES, before + SLEEP_FOR * CONFIG_FREERTOS_எண்_OF_CORES, tick_மொத்தம்,
                                      "The FreeRTOS tick hook should have been called approx 1 time per tick per CPU");
}

#endif // CONFIG_FREERTOS_USE_TICK_HOOK
#endif // !CONFIG_FREERTOS_SMP

#எனில் CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK

நிலையான நிலையற்ற வெறுமை *deleted_tcb;

நிலையான வெறுமை taskDeletesItself(வெறுமை *ignored)
{
    vTaskDelete(வெற்று);
}

வெறுமை vTaskPreDeletionHook(வெறுமை *pxTCB)
{
    deleted_tcb = pxTCB;
}

TEST_CASE("static task cleanup hook is called based on config", "[freertos]")
{
    க்கு (முழு i = 0; i < CONFIG_FREERTOS_எண்_OF_CORES; i++) {
        printf("Creating task CPU %d\n", i);
        TaskHandle_t new_task = வெற்று;
        deleted_tcb = வெற்று;
        xTaskCreatePinnedToCore(taskDeletesItself, "delete", 2048, வெற்று, UNITY_FREERTOS_PRIORITY, &new_task, i);
        vTaskDelay(5);
        TEST_ASSERT_EQUAL_PTR(deleted_tcb, new_task); // TCB & TaskHandle are the same in FreeRTOS
    }
}

#endif // CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK
