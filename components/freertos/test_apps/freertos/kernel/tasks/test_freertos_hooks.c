/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#समावेश "sdkconfig.h"

/*
Note: We disable this test when using the FreeRTOS SMP kernel as the port will already provide
a definition for vApplicationTickHook(). Thus this test cannot be run.
*/
#समावेश <stdio.h>
#समावेश <stdlib.h>
#समावेश "freertos/FreeRTOS.h"
#समावेश "freertos/task.h"
#समावेश <freertos/semphr.h>
#समावेश "unity.h"
#समावेश "test_utils.h"

#अगर !CONFIG_FREERTOS_SMP
/*
Test FreeRTOS idle hook. Only compiled in if FreeRTOS idle hooks are enabled.
*/
#अगर CONFIG_FREERTOS_USE_IDLE_HOOK

अटल अस्थिर अचिन्हित idle_गिनती;

व्योम vApplicationIdleHook(व्योम)
{
    idle_गिनती++;
}

TEST_CASE("FreeRTOS idle hook", "[freertos]")
{
    idle_गिनती = 0;
    vTaskDelay(10);
    TEST_ASSERT_NOT_EQUAL(0, idle_गिनती); // The legacy idle hook should be called at least once
}

#पूर्ण_अगर // CONFIG_FREERTOS_USE_IDLE_HOOK

/*
Test the FreeRTOS tick hook. Only compiled in if FreeRTOS tick hooks are enabled.
*/
#अगर CONFIG_FREERTOS_USE_TICK_HOOK

अटल अस्थिर अचिन्हित tick_गिनती;

व्योम vApplicationTickHook(व्योम)
{
    tick_गिनती++;
}

TEST_CASE("FreeRTOS tick hook", "[freertos]")
{
    अचिन्हित before = x_Task_Get_Tick_गिनती();
    स्थिर अचिन्हित SLEEP_FOR = 20;
    tick_गिनती = before;
    vTaskDelay(SLEEP_FOR);
    TEST_ASSERT_UINT32_WITHIN_MESSAGE(3 * CONFIG_FREERTOS_संख्या_OF_CORES, before + SLEEP_FOR * CONFIG_FREERTOS_संख्या_OF_CORES, tick_गिनती,
                                      "The FreeRTOS tick hook should have been called approx 1 time per tick per CPU");
}

#पूर्ण_अगर // CONFIG_FREERTOS_USE_TICK_HOOK
#पूर्ण_अगर // !CONFIG_FREERTOS_SMP

#अगर CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK

अटल अस्थिर व्योम *deleted_tcb;

अटल व्योम taskDeletesItself(व्योम *ignored)
{
    vTaskDelete(शून्य);
}

व्योम vTaskPreDeletionHook(व्योम *pxTCB)
{
    deleted_tcb = pxTCB;
}

TEST_CASE("static task cleanup hook is called based on config", "[freertos]")
{
    क्रम (पूर्णांक i = 0; i < CONFIG_FREERTOS_संख्या_OF_CORES; i++) {
        म_लिखो("Creating task CPU %d\n", i);
        TaskHandle_t new_task = शून्य;
        deleted_tcb = शून्य;
        xTaskCreatePinnedToCore(taskDeletesItself, "delete", 2048, शून्य, UNITY_FREERTOS_PRIORITY, &new_task, i);
        vTaskDelay(5);
        TEST_ASSERT_EQUAL_PTR(deleted_tcb, new_task); // TCB & TaskHandle are the same in FreeRTOS
    }
}

#पूर्ण_अगर // CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK
