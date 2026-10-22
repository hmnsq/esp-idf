/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#अन्तर्भाव "sdkconfig.h"

/*
Note: We disable this test when using the FreeRTOS SMP kernel as the port will already provide
a definition for vApplicationTickHook(). Thus this test cannot be run.
*/
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव "freertos/FreeRTOS.h"
#अन्तर्भाव "freertos/task.h"
#अन्तर्भाव <freertos/semphr.h>
#अन्तर्भाव "unity.h"
#अन्तर्भाव "test_utils.h"

#यदि !CONFIG_FREERTOS_SMP
/*
Test FreeRTOS idle hook. Only compiled in if FreeRTOS idle hooks are enabled.
*/
#यदि CONFIG_FREERTOS_USE_IDLE_HOOK

स्थिर चञ्चल unsigned idle_गिनती;

शून्य vApplicationIdleHook(शून्य)
{
    idle_गिनती++;
}

TEST_CASE("FreeRTOS idle hook", "[freertos]")
{
    idle_गिनती = 0;
    vTaskDelay(10);
    TEST_ASSERT_NOT_EQUAL(0, idle_गिनती); // The legacy idle hook should be called at least once
}

#endif // CONFIG_FREERTOS_USE_IDLE_HOOK

/*
Test the FreeRTOS tick hook. Only compiled in if FreeRTOS tick hooks are enabled.
*/
#यदि CONFIG_FREERTOS_USE_TICK_HOOK

स्थिर चञ्चल unsigned tick_गिनती;

शून्य vApplicationTickHook(शून्य)
{
    tick_गिनती++;
}

TEST_CASE("FreeRTOS tick hook", "[freertos]")
{
    unsigned before = x_Task_Get_Tick_गिनती();
    ध्रुव unsigned SLEEP_FOR = 20;
    tick_गिनती = before;
    vTaskDelay(SLEEP_FOR);
    TEST_ASSERT_UINT32_WITHIN_MESSAGE(3 * CONFIG_FREERTOS_संख्या_OF_CORES, before + SLEEP_FOR * CONFIG_FREERTOS_संख्या_OF_CORES, tick_गिनती,
                                      "The FreeRTOS tick hook should have been called approx 1 time per tick per CPU");
}

#endif // CONFIG_FREERTOS_USE_TICK_HOOK
#endif // !CONFIG_FREERTOS_SMP

#यदि CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK

स्थिर चञ्चल शून्य *deleted_tcb;

स्थिर शून्य taskDeletesItself(शून्य *ignored)
{
    vTaskDelete(रिक्त);
}

शून्य vTaskPreDeletionHook(शून्य *pxTCB)
{
    deleted_tcb = pxTCB;
}

TEST_CASE("static task cleanup hook is called based on config", "[freertos]")
{
    प्रति (पूर्णाङ्क i = 0; i < CONFIG_FREERTOS_संख्या_OF_CORES; i++) {
        printf("Creating task CPU %d\n", i);
        TaskHandle_t new_task = रिक्त;
        deleted_tcb = रिक्त;
        xTaskCreatePinnedToCore(taskDeletesItself, "delete", 2048, रिक्त, UNITY_FREERTOS_PRIORITY, &new_task, i);
        vTaskDelay(5);
        TEST_ASSERT_EQUAL_PTR(deleted_tcb, new_task); // TCB & TaskHandle are the same in FreeRTOS
    }
}

#endif // CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK
