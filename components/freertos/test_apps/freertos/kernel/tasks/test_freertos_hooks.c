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

स्थिर चञ्चल अचिह्नित idle_गणनम्;

शून्य vApplicationIdleHook(शून्य)
{
    idle_गणनम्++;
}

TEST_CASE("FreeRTOS idle hook", "[freertos]")
{
    idle_गणनम् = 0;
    vTaskDelay(10);
    TEST_ASSERT_NOT_EQUAL(0, idle_गणनम्); // The legacy idle hook should be called at least once
}

#यदि_अन्तः // CONFIG_FREERTOS_USE_IDLE_HOOK

/*
Test the FreeRTOS tick hook. Only compiled in if FreeRTOS tick hooks are enabled.
*/
#यदि CONFIG_FREERTOS_USE_TICK_HOOK

स्थिर चञ्चल अचिह्नित tick_गणनम्;

शून्य vApplicationTickHook(शून्य)
{
    tick_गणनम्++;
}

TEST_CASE("FreeRTOS tick hook", "[freertos]")
{
    अचिह्नित before = x_Task_Get_Tick_गणनम्();
    ध्रुव अचिह्नित SLEEP_FOR = 20;
    tick_गणनम् = before;
    vTaskDelay(SLEEP_FOR);
    TEST_ASSERT_UINT32_WITHIN_सन्देशः(3 * CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES, before + SLEEP_FOR * CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES, tick_गणनम्,
                                      "The FreeRTOS tick hook should have been called approx 1 time per tick per CPU");
}

#यदि_अन्तः // CONFIG_FREERTOS_USE_TICK_HOOK
#यदि_अन्तः // !CONFIG_FREERTOS_SMP

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
    प्रति (पूर्णाङ्क i = 0; i < CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES; i++) {
        प्रारूप_लिख("Creating task CPU %d\n", i);
        TaskHandle_t new_task = रिक्त;
        deleted_tcb = रिक्त;
        x_Task_Create_Pinned_गन्तव्यम्_Core(taskDeletesItself, "delete", 2048, रिक्त, UNITY_FREERTOS_प्राथमिकता, &new_task, i);
        vTaskDelay(5);
        TEST_ASSERT_EQUAL_PTR(deleted_tcb, new_task); // TCB & TaskHandle are the same in FreeRTOS
    }
}

#यदि_अन्तः // CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK
