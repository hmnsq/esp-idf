/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Test for thread local storage support.
*/

#अन्तर्भाव <string.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <esp_types.h>
#अन्तर्भाव <inttypes.h>

#अन्तर्भाव "freertos/FreeRTOS.h"
#अन्तर्भाव "freertos/task.h"
#अन्तर्भाव "unity.h"
#अन्तर्भाव "test_utils.h"
#अन्तर्भाव "sdkconfig.h"

#यदि defined(__XTENSA__)
#परिभाषय GET_THREADPTR(tp_dest)  कुरु { asm चञ्चल ("rur.threadptr %0":"=r"(tp_dest)); } यावत्(0)
#नोचेद्यदि defined (__riscv)
#परिभाषय GET_THREADPTR(tp_dest)  कुरु { पञ्जिका uint32_t _tp asm("tp"); tp_dest = _tp; } यावत्(0)
#यदि_अन्तः

स्थिर __thread पूर्णाङ्क tl_test_var1;
स्थिर __thread uint8_t tl_test_var2 = 55;
स्थिर __thread uint16_t tl_test_var3 = 44;
स्थिर __thread uint8_t tl_test_arr_var[10];
स्थिर __thread संरचना test_tls_var {
    पूर्णाङ्क f32;
    uint8_t f8;
    uint16_t f16;
    uint8_t farr[10];
} tl_test_struct_var;

स्थिर शून्य task_test_tls(शून्य *arg)
{
    bool *running = (bool *)arg;
    uint32_t tp = (uint32_t) -1;
    पूर्णाङ्क test_var1_old = 0;
    uint8_t test_var2_old = 0;
    uint16_t test_var3_old = 0;
    पूर्णाङ्क f32_old = 0;
    uint8_t f8_old = 0;
    uint16_t f16_old = 0;
    बीजं_रोपय((पूर्णाङ्क) xTaskGetCurrentTaskHandle());
    पूर्णाङ्क चरणः = (यादृच्छिकम्() % 10) + 1;

    GET_THREADPTR(tp);
    प्रति (पूर्णाङ्क i = 0; i < 5; i++) {
        प्रारूप_लिख("Task[%"PRIx32"]: var1 = %d var2 = 0x%"PRIx8" step=%d\n", tp, tl_test_var1, tl_test_var2, चरणः);
        यदि (i == 0) {
            TEST_ASSERT_EQUAL(0, tl_test_var1);
            TEST_ASSERT_EQUAL(55, tl_test_var2);
            TEST_ASSERT_EQUAL(44, tl_test_var3);
            प्रति (पूर्णाङ्क k = 0; k < परिमाण(tl_test_arr_var); k++) {
                TEST_ASSERT_EQUAL(0, tl_test_arr_var[k]);
            }
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f32);
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f8);
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f16);
            प्रति (पूर्णाङ्क k = 0; k < परिमाण(tl_test_struct_var.farr); k++) {
                TEST_ASSERT_EQUAL(0, tl_test_struct_var.farr[k]);
            }
        } अन्यथा {
            TEST_ASSERT_EQUAL(test_var1_old + चरणः, tl_test_var1);
            TEST_ASSERT_EQUAL(test_var2_old + चरणः, tl_test_var2);
            TEST_ASSERT_EQUAL(test_var3_old + चरणः, tl_test_var3);
            प्रति (पूर्णाङ्क k = 0; k < परिमाण(tl_test_arr_var); k++) {
                TEST_ASSERT_EQUAL((i - 1) * चरणः, tl_test_arr_var[k]);
            }
            TEST_ASSERT_EQUAL(f32_old + चरणः, tl_test_struct_var.f32);
            TEST_ASSERT_EQUAL(f8_old + चरणः, tl_test_struct_var.f8);
            TEST_ASSERT_EQUAL(f16_old + चरणः, tl_test_struct_var.f16);
            प्रति (पूर्णाङ्क k = 0; k < परिमाण(tl_test_struct_var.farr); k++) {
                TEST_ASSERT_EQUAL((i - 1) * चरणः, tl_test_struct_var.farr[k]);
            }
        }

        test_var1_old = tl_test_var1;
        test_var2_old = tl_test_var2;
        test_var3_old = tl_test_var3;
        f32_old = tl_test_struct_var.f32;
        f8_old = tl_test_struct_var.f8;
        f16_old = tl_test_struct_var.f16;
        tl_test_var1 += चरणः;
        tl_test_var2 += चरणः;
        tl_test_var3 += चरणः;
        स्मृतिं_पूरय(tl_test_arr_var, i * चरणः, परिमाण(tl_test_arr_var));
        tl_test_struct_var.f32 += चरणः;
        tl_test_struct_var.f8 += चरणः;
        tl_test_struct_var.f16 += चरणः;
        स्मृतिं_पूरय(tl_test_struct_var.farr, i * चरणः, परिमाण(tl_test_struct_var.farr));
        vTaskDelay(10);
    }

    यदि (running) {
        *running = false;
        vTaskDelete(रिक्त);
    }
}

TEST_CASE("TLS test", "[freertos]")
{
    ध्रुव size_t राशिः_आकार_2 = 3072;
    राशिः_Type_t s_राशिः[राशिः_आकार_2]; /* with 8KB test task stack (default) this test still has ~3KB headroom */
    StaticTask_t s_task;
    bool running[2] = {true, true};
#यदि CONFIG_FREERTOS_UNICORE == 0
    पूर्णाङ्क other_core = 1;
#अन्यथा
    पूर्णाङ्क other_core = 0;
#यदि_अन्तः

    x_Task_Create_Pinned_गन्तव्यम्_Core((TaskFunction_t)&task_test_tls, "task_test_tls", राशिः_आकार_2, &running[0],
                            UNITY_FREERTOS_प्राथमिकता, रिक्त, 0);
    x_Task_Create_Static_Pinned_गन्तव्यम्_Core((TaskFunction_t)&task_test_tls, "task_test_tls", राशिः_आकार_2, &running[1],
                                  UNITY_FREERTOS_प्राथमिकता, s_राशिः, &s_task, other_core);
    यावत् (running[0] || running[1]) {
        vTaskDelay(10);
    }
    vTaskDelay(10); /* Make sure idle task can clean up s_task, before it goes out of scope */
}
