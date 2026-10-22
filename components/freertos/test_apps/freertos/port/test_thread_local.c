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

#जइ defined(__XTENSA__)
#define GET_THREADPTR(tp_dest)  करेहि { asm चञ्चल ("rur.threadptr %0":"=r"(tp_dest)); } जाव(0)
#नोचेद्यदि defined (__riscv)
#define GET_THREADPTR(tp_dest)  करेहि { पञ्जिका uint32_t _tp asm("tp"); tp_dest = _tp; } जाव(0)
#endif

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
    srand((पूर्णाङ्क) xTaskGetCurrentTaskHandle());
    पूर्णाङ्क step = (rand() % 10) + 1;

    GET_THREADPTR(tp);
    प्रति (पूर्णाङ्क i = 0; i < 5; i++) {
        printf("Task[%"PRIx32"]: var1 = %d var2 = 0x%"PRIx8" step=%d\n", tp, tl_test_var1, tl_test_var2, step);
        जइ (i == 0) {
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
        } अण्णहा {
            TEST_ASSERT_EQUAL(test_var1_old + step, tl_test_var1);
            TEST_ASSERT_EQUAL(test_var2_old + step, tl_test_var2);
            TEST_ASSERT_EQUAL(test_var3_old + step, tl_test_var3);
            प्रति (पूर्णाङ्क k = 0; k < परिमाण(tl_test_arr_var); k++) {
                TEST_ASSERT_EQUAL((i - 1) * step, tl_test_arr_var[k]);
            }
            TEST_ASSERT_EQUAL(f32_old + step, tl_test_struct_var.f32);
            TEST_ASSERT_EQUAL(f8_old + step, tl_test_struct_var.f8);
            TEST_ASSERT_EQUAL(f16_old + step, tl_test_struct_var.f16);
            प्रति (पूर्णाङ्क k = 0; k < परिमाण(tl_test_struct_var.farr); k++) {
                TEST_ASSERT_EQUAL((i - 1) * step, tl_test_struct_var.farr[k]);
            }
        }

        test_var1_old = tl_test_var1;
        test_var2_old = tl_test_var2;
        test_var3_old = tl_test_var3;
        f32_old = tl_test_struct_var.f32;
        f8_old = tl_test_struct_var.f8;
        f16_old = tl_test_struct_var.f16;
        tl_test_var1 += step;
        tl_test_var2 += step;
        tl_test_var3 += step;
        memset(tl_test_arr_var, i * step, परिमाण(tl_test_arr_var));
        tl_test_struct_var.f32 += step;
        tl_test_struct_var.f8 += step;
        tl_test_struct_var.f16 += step;
        memset(tl_test_struct_var.farr, i * step, परिमाण(tl_test_struct_var.farr));
        vTaskDelay(10);
    }

    जइ (running) {
        *running = false;
        vTaskDelete(रिक्त);
    }
}

TEST_CASE("TLS test", "[freertos]")
{
    ध्रुव size_t राशिः_size = 3072;
    राशिः_Type_t s_राशिः[राशिः_size]; /* with 8KB test task stack (default) this test still has ~3KB headroom */
    StaticTask_t s_task;
    bool running[2] = {true, true};
#जइ CONFIG_FREERTOS_UNICORE == 0
    पूर्णाङ्क other_core = 1;
#अण्णहा
    पूर्णाङ्क other_core = 0;
#endif

    xTaskCreatePinnedToCore((TaskFunction_t)&task_test_tls, "task_test_tls", राशिः_size, &running[0],
                            UNITY_FREERTOS_PRIORITY, रिक्त, 0);
    xTaskCreateStaticPinnedToCore((TaskFunction_t)&task_test_tls, "task_test_tls", राशिः_size, &running[1],
                                  UNITY_FREERTOS_PRIORITY, s_राशिः, &s_task, other_core);
    जाव (running[0] || running[1]) {
        vTaskDelay(10);
    }
    vTaskDelay(10); /* Make sure idle task can clean up s_task, before it goes out of scope */
}
