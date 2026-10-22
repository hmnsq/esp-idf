/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Test for thread local storage support.
*/

#অন্তর্ভাব <string.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <esp_types.h>
#অন্তর্ভাব <inttypes.h>

#অন্তর্ভাব "freertos/FreeRTOS.h"
#অন্তর্ভাব "freertos/task.h"
#অন্তর্ভাব "unity.h"
#অন্তর্ভাব "test_utils.h"
#অন্তর্ভাব "sdkconfig.h"

#যদি defined(__XTENSA__)
#define GET_THREADPTR(tp_dest)  করো { asm অস্থির ("rur.threadptr %0":"=r"(tp_dest)); } যখন(0)
#নতুবা_যদি defined (__riscv)
#define GET_THREADPTR(tp_dest)  করো { নিবন্ধ uint32_t _tp asm("tp"); tp_dest = _tp; } যখন(0)
#endif

স্থির __thread পূর্ণসংখ্যা tl_test_var1;
স্থির __thread uint8_t tl_test_var2 = 55;
স্থির __thread uint16_t tl_test_var3 = 44;
স্থির __thread uint8_t tl_test_arr_var[10];
স্থির __thread কাঠামো test_tls_var {
    পূর্ণসংখ্যা f32;
    uint8_t f8;
    uint16_t f16;
    uint8_t farr[10];
} tl_test_struct_var;

স্থির শূন্য task_test_tls(শূন্য *arg)
{
    bool *running = (bool *)arg;
    uint32_t tp = (uint32_t) -1;
    পূর্ণসংখ্যা test_var1_old = 0;
    uint8_t test_var2_old = 0;
    uint16_t test_var3_old = 0;
    পূর্ণসংখ্যা f32_old = 0;
    uint8_t f8_old = 0;
    uint16_t f16_old = 0;
    srand((পূর্ণসংখ্যা) xTaskGetCurrentTaskHandle());
    পূর্ণসংখ্যা step = (rand() % 10) + 1;

    GET_THREADPTR(tp);
    জন্য (পূর্ণসংখ্যা i = 0; i < 5; i++) {
        printf("Task[%"PRIx32"]: var1 = %d var2 = 0x%"PRIx8" step=%d\n", tp, tl_test_var1, tl_test_var2, step);
        যদি (i == 0) {
            TEST_ASSERT_EQUAL(0, tl_test_var1);
            TEST_ASSERT_EQUAL(55, tl_test_var2);
            TEST_ASSERT_EQUAL(44, tl_test_var3);
            জন্য (পূর্ণসংখ্যা k = 0; k < আকার(tl_test_arr_var); k++) {
                TEST_ASSERT_EQUAL(0, tl_test_arr_var[k]);
            }
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f32);
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f8);
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f16);
            জন্য (পূর্ণসংখ্যা k = 0; k < আকার(tl_test_struct_var.farr); k++) {
                TEST_ASSERT_EQUAL(0, tl_test_struct_var.farr[k]);
            }
        } অন্যথা {
            TEST_ASSERT_EQUAL(test_var1_old + step, tl_test_var1);
            TEST_ASSERT_EQUAL(test_var2_old + step, tl_test_var2);
            TEST_ASSERT_EQUAL(test_var3_old + step, tl_test_var3);
            জন্য (পূর্ণসংখ্যা k = 0; k < আকার(tl_test_arr_var); k++) {
                TEST_ASSERT_EQUAL((i - 1) * step, tl_test_arr_var[k]);
            }
            TEST_ASSERT_EQUAL(f32_old + step, tl_test_struct_var.f32);
            TEST_ASSERT_EQUAL(f8_old + step, tl_test_struct_var.f8);
            TEST_ASSERT_EQUAL(f16_old + step, tl_test_struct_var.f16);
            জন্য (পূর্ণসংখ্যা k = 0; k < আকার(tl_test_struct_var.farr); k++) {
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
        memset(tl_test_arr_var, i * step, আকার(tl_test_arr_var));
        tl_test_struct_var.f32 += step;
        tl_test_struct_var.f8 += step;
        tl_test_struct_var.f16 += step;
        memset(tl_test_struct_var.farr, i * step, আকার(tl_test_struct_var.farr));
        vTaskDelay(10);
    }

    যদি (running) {
        *running = false;
        vTaskDelete(ফাঁকা);
    }
}

TEST_CASE("TLS test", "[freertos]")
{
    ধ্রুবক size_t স্তূপ_আকার_2 = 3072;
    স্তূপ_Type_t s_স্তূপ[স্তূপ_আকার_2]; /* with 8KB test task stack (default) this test still has ~3KB headroom */
    StaticTask_t s_task;
    bool running[2] = {true, true};
#যদি CONFIG_FREERTOS_UNICORE == 0
    পূর্ণসংখ্যা other_core = 1;
#অন্যথা
    পূর্ণসংখ্যা other_core = 0;
#endif

    xTaskCreatePinnedToCore((TaskFunction_t)&task_test_tls, "task_test_tls", স্তূপ_আকার_2, &running[0],
                            UNITY_FREERTOS_PRIORITY, ফাঁকা, 0);
    xTaskCreateStaticPinnedToCore((TaskFunction_t)&task_test_tls, "task_test_tls", স্তূপ_আকার_2, &running[1],
                                  UNITY_FREERTOS_PRIORITY, s_স্তূপ, &s_task, other_core);
    যখন (running[0] || running[1]) {
        vTaskDelay(10);
    }
    vTaskDelay(10); /* Make sure idle task can clean up s_task, before it goes out of scope */
}
