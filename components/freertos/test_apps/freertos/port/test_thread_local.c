/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Test for thread local storage support.
*/

#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <esp_types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <inttypes.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unity.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "test_utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"

#ਜੇ defined(__XTENSA__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GET_THREADPTR(tp_dest)  ਕਰੋ { asm ਅਸਥਿਰ ("rur.threadptr %0":"=r"(tp_dest)); } ਜਦੋਂ(0)
#ਜਾਂਜੇ defined (__riscv)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GET_THREADPTR(tp_dest)  ਕਰੋ { ਰਜਿਸਟਰ uint32_t _tp asm("tp"); tp_dest = _tp; } ਜਦੋਂ(0)
#ਜੇ_ਅੰਤ

ਸਥਿਰ __thread ਪੂਰਨ tl_test_var1;
ਸਥਿਰ __thread uint8_t tl_test_var2 = 55;
ਸਥਿਰ __thread uint16_t tl_test_var3 = 44;
ਸਥਿਰ __thread uint8_t tl_test_arr_var[10];
ਸਥਿਰ __thread ਬਣਤਰ test_tls_var {
    ਪੂਰਨ f32;
    uint8_t f8;
    uint16_t f16;
    uint8_t farr[10];
} tl_test_struct_var;

ਸਥਿਰ ਖਾਲੀ task_test_tls(ਖਾਲੀ *arg)
{
    bool *running = (bool *)arg;
    uint32_t tp = (uint32_t) -1;
    ਪੂਰਨ test_var1_old = 0;
    uint8_t test_var2_old = 0;
    uint16_t test_var3_old = 0;
    ਪੂਰਨ f32_old = 0;
    uint8_t f8_old = 0;
    uint16_t f16_old = 0;
    srand((ਪੂਰਨ) xTaskGetCurrentTaskHandle());
    ਪੂਰਨ ਕਦਮ = (rand() % 10) + 1;

    GET_THREADPTR(tp);
    ਲਈ (ਪੂਰਨ i = 0; i < 5; i++) {
        ਲਿਖੋ("Task[%"PRIx32"]: var1 = %d var2 = 0x%"PRIx8" step=%d\n", tp, tl_test_var1, tl_test_var2, ਕਦਮ);
        ਜੇ (i == 0) {
            TEST_ASSERT_EQUAL(0, tl_test_var1);
            TEST_ASSERT_EQUAL(55, tl_test_var2);
            TEST_ASSERT_EQUAL(44, tl_test_var3);
            ਲਈ (ਪੂਰਨ k = 0; k < ਆਕਾਰ(tl_test_arr_var); k++) {
                TEST_ASSERT_EQUAL(0, tl_test_arr_var[k]);
            }
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f32);
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f8);
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f16);
            ਲਈ (ਪੂਰਨ k = 0; k < ਆਕਾਰ(tl_test_struct_var.farr); k++) {
                TEST_ASSERT_EQUAL(0, tl_test_struct_var.farr[k]);
            }
        } ਨਹੀਂਤਾਂ {
            TEST_ASSERT_EQUAL(test_var1_old + ਕਦਮ, tl_test_var1);
            TEST_ASSERT_EQUAL(test_var2_old + ਕਦਮ, tl_test_var2);
            TEST_ASSERT_EQUAL(test_var3_old + ਕਦਮ, tl_test_var3);
            ਲਈ (ਪੂਰਨ k = 0; k < ਆਕਾਰ(tl_test_arr_var); k++) {
                TEST_ASSERT_EQUAL((i - 1) * ਕਦਮ, tl_test_arr_var[k]);
            }
            TEST_ASSERT_EQUAL(f32_old + ਕਦਮ, tl_test_struct_var.f32);
            TEST_ASSERT_EQUAL(f8_old + ਕਦਮ, tl_test_struct_var.f8);
            TEST_ASSERT_EQUAL(f16_old + ਕਦਮ, tl_test_struct_var.f16);
            ਲਈ (ਪੂਰਨ k = 0; k < ਆਕਾਰ(tl_test_struct_var.farr); k++) {
                TEST_ASSERT_EQUAL((i - 1) * ਕਦਮ, tl_test_struct_var.farr[k]);
            }
        }

        test_var1_old = tl_test_var1;
        test_var2_old = tl_test_var2;
        test_var3_old = tl_test_var3;
        f32_old = tl_test_struct_var.f32;
        f8_old = tl_test_struct_var.f8;
        f16_old = tl_test_struct_var.f16;
        tl_test_var1 += ਕਦਮ;
        tl_test_var2 += ਕਦਮ;
        tl_test_var3 += ਕਦਮ;
        memset(tl_test_arr_var, i * ਕਦਮ, ਆਕਾਰ(tl_test_arr_var));
        tl_test_struct_var.f32 += ਕਦਮ;
        tl_test_struct_var.f8 += ਕਦਮ;
        tl_test_struct_var.f16 += ਕਦਮ;
        memset(tl_test_struct_var.farr, i * ਕਦਮ, ਆਕਾਰ(tl_test_struct_var.farr));
        vTaskDelay(10);
    }

    ਜੇ (running) {
        *running = false;
        vTaskDelete(ਨਿਰਾ);
    }
}

TEST_CASE("TLS test", "[freertos]")
{
    ਅਚਲ size_t ਢੇਰ_ਮਾਪ_2 = 3072;
    ਢੇਰ_Type_t s_ਢੇਰ[ਢੇਰ_ਮਾਪ_2]; /* with 8KB test task stack (default) this test still has ~3KB headroom */
    StaticTask_t s_task;
    bool running[2] = {true, true};
#ਜੇ CONFIG_FREERTOS_UNICORE == 0
    ਪੂਰਨ other_core = 1;
#ਨਹੀਂਤਾਂ
    ਪੂਰਨ other_core = 0;
#ਜੇ_ਅੰਤ

    x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core((TaskFunction_t)&task_test_tls, "task_test_tls", ਢੇਰ_ਮਾਪ_2, &running[0],
                            UNITY_FREERTOS_ਤਰਜੀਹ, ਨਿਰਾ, 0);
    x_Task_Create_Static_Pinned_ਮੰਜ਼ਿਲ_Core((TaskFunction_t)&task_test_tls, "task_test_tls", ਢੇਰ_ਮਾਪ_2, &running[1],
                                  UNITY_FREERTOS_ਤਰਜੀਹ, s_ਢੇਰ, &s_task, other_core);
    ਜਦੋਂ (running[0] || running[1]) {
        vTaskDelay(10);
    }
    vTaskDelay(10); /* Make sure idle task can clean up s_task, before it goes out of scope */
}
