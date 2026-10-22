/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Test for thread local storage support.
*/

#অন্তর্ভাৱ <string.h>
#অন্তর্ভাৱ <stdlib.h>
#অন্তর্ভাৱ <esp_types.h>
#অন্তর্ভাৱ <inttypes.h>

#অন্তর্ভাৱ "freertos/FreeRTOS.h"
#অন্তর্ভাৱ "freertos/task.h"
#অন্তর্ভাৱ "unity.h"
#অন্তর্ভাৱ "test_utils.h"
#অন্তর্ভাৱ "sdkconfig.h"

#যদি defined(__XTENSA__)
#define GET_THREADPTR(tp_dest)  কৰা { asm চঞ্চল ("rur.threadptr %0":"=r"(tp_dest)); } যেতিয়ালৈ(0)
#নোচেদ্যদি defined (__riscv)
#define GET_THREADPTR(tp_dest)  কৰা { পঞ্জিকা uint32_t _tp asm("tp"); tp_dest = _tp; } যেতিয়ালৈ(0)
#endif

স্থির __thread পূর্ণাঙ্ক tl_test_var1;
স্থির __thread uint8_t tl_test_var2 = 55;
স্থির __thread uint16_t tl_test_var3 = 44;
স্থির __thread uint8_t tl_test_arr_var[10];
স্থির __thread সংরচনা test_tls_var {
    পূর্ণাঙ্ক f32;
    uint8_t f8;
    uint16_t f16;
    uint8_t farr[10];
} tl_test_struct_var;

স্থির শূন্য task_test_tls(শূন্য *arg)
{
    bool *running = (bool *)arg;
    uint32_t tp = (uint32_t) -1;
    পূর্ণাঙ্ক test_var1_old = 0;
    uint8_t test_var2_old = 0;
    uint16_t test_var3_old = 0;
    পূর্ণাঙ্ক f32_old = 0;
    uint8_t f8_old = 0;
    uint16_t f16_old = 0;
    srand((পূর্ণাঙ্ক) xTaskGetCurrentTaskHandle());
    পূর্ণাঙ্ক step = (rand() % 10) + 1;

    GET_THREADPTR(tp);
    বাবে (পূর্ণাঙ্ক i = 0; i < 5; i++) {
        printf("Task[%"PRIx32"]: var1 = %d var2 = 0x%"PRIx8" step=%d\n", tp, tl_test_var1, tl_test_var2, step);
        যদি (i == 0) {
            TEST_ASSERT_EQUAL(0, tl_test_var1);
            TEST_ASSERT_EQUAL(55, tl_test_var2);
            TEST_ASSERT_EQUAL(44, tl_test_var3);
            বাবে (পূর্ণাঙ্ক k = 0; k < পরিমাণ(tl_test_arr_var); k++) {
                TEST_ASSERT_EQUAL(0, tl_test_arr_var[k]);
            }
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f32);
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f8);
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f16);
            বাবে (পূর্ণাঙ্ক k = 0; k < পরিমাণ(tl_test_struct_var.farr); k++) {
                TEST_ASSERT_EQUAL(0, tl_test_struct_var.farr[k]);
            }
        } অন্যথা {
            TEST_ASSERT_EQUAL(test_var1_old + step, tl_test_var1);
            TEST_ASSERT_EQUAL(test_var2_old + step, tl_test_var2);
            TEST_ASSERT_EQUAL(test_var3_old + step, tl_test_var3);
            বাবে (পূর্ণাঙ্ক k = 0; k < পরিমাণ(tl_test_arr_var); k++) {
                TEST_ASSERT_EQUAL((i - 1) * step, tl_test_arr_var[k]);
            }
            TEST_ASSERT_EQUAL(f32_old + step, tl_test_struct_var.f32);
            TEST_ASSERT_EQUAL(f8_old + step, tl_test_struct_var.f8);
            TEST_ASSERT_EQUAL(f16_old + step, tl_test_struct_var.f16);
            বাবে (পূর্ণাঙ্ক k = 0; k < পরিমাণ(tl_test_struct_var.farr); k++) {
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
        memset(tl_test_arr_var, i * step, পরিমাণ(tl_test_arr_var));
        tl_test_struct_var.f32 += step;
        tl_test_struct_var.f8 += step;
        tl_test_struct_var.f16 += step;
        memset(tl_test_struct_var.farr, i * step, পরিমাণ(tl_test_struct_var.farr));
        vTaskDelay(10);
    }

    যদি (running) {
        *running = false;
        vTaskDelete(রিক্ত);
    }
}

TEST_CASE("TLS test", "[freertos]")
{
    ধ্রুৱ size_t স্তূপ_size = 3072;
    স্তূপ_Type_t s_স্তূপ[স্তূপ_size]; /* with 8KB test task stack (default) this test still has ~3KB headroom */
    StaticTask_t s_task;
    bool running[2] = {true, true};
#যদি CONFIG_FREERTOS_UNICORE == 0
    পূর্ণাঙ্ক other_core = 1;
#অন্যথা
    পূর্ণাঙ্ক other_core = 0;
#endif

    xTaskCreatePinnedToCore((TaskFunction_t)&task_test_tls, "task_test_tls", স্তূপ_size, &running[0],
                            UNITY_FREERTOS_PRIORITY, রিক্ত, 0);
    xTaskCreateStaticPinnedToCore((TaskFunction_t)&task_test_tls, "task_test_tls", স্তূপ_size, &running[1],
                                  UNITY_FREERTOS_PRIORITY, s_স্তূপ, &s_task, other_core);
    যেতিয়ালৈ (running[0] || running[1]) {
        vTaskDelay(10);
    }
    vTaskDelay(10); /* Make sure idle task can clean up s_task, before it goes out of scope */
}
