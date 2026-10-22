/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Test for thread local storage support.
*/

#شامل <string.h>
#شامل <stdlib.h>
#شامل <esp_types.h>
#شامل <inttypes.h>

#شامل "freertos/FreeRTOS.h"
#شامل "freertos/task.h"
#شامل "unity.h"
#شامل "test_utils.h"
#شامل "sdkconfig.h"

#اگر defined(__XTENSA__)
#تعریف_کرو GET_THREADPTR(tp_dest)  کرو { asm متلون ("rur.threadptr %0":"=r"(tp_dest)); } جب(0)
#وگرنہ_اگر defined (__riscv)
#تعریف_کرو GET_THREADPTR(tp_dest)  کرو { اندراج uint32_t _tp asm("tp"); tp_dest = _tp; } جب(0)
#اگر_ختم

ساکن __thread صحیح tl_test_var1;
ساکن __thread uint8_t tl_test_var2 = 55;
ساکن __thread uint16_t tl_test_var3 = 44;
ساکن __thread uint8_t tl_test_arr_var[10];
ساکن __thread ساخت test_tls_var {
    صحیح f32;
    uint8_t f8;
    uint16_t f16;
    uint8_t farr[10];
} tl_test_struct_var;

ساکن خالی task_test_tls(خالی *arg)
{
    bool *running = (bool *)arg;
    uint32_t tp = (uint32_t) -1;
    صحیح test_var1_old = 0;
    uint8_t test_var2_old = 0;
    uint16_t test_var3_old = 0;
    صحیح f32_old = 0;
    uint8_t f8_old = 0;
    uint16_t f16_old = 0;
    srand((صحیح) xTaskGetCurrentTaskHandle());
    صحیح قدم = (rand() % 10) + 1;

    GET_THREADPTR(tp);
    برائے (صحیح i = 0; i < 5; i++) {
        لکھو("Task[%"PRIx32"]: var1 = %d var2 = 0x%"PRIx8" step=%d\n", tp, tl_test_var1, tl_test_var2, قدم);
        اگر (i == 0) {
            TEST_ASSERT_EQUAL(0, tl_test_var1);
            TEST_ASSERT_EQUAL(55, tl_test_var2);
            TEST_ASSERT_EQUAL(44, tl_test_var3);
            برائے (صحیح k = 0; k < حجم(tl_test_arr_var); k++) {
                TEST_ASSERT_EQUAL(0, tl_test_arr_var[k]);
            }
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f32);
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f8);
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f16);
            برائے (صحیح k = 0; k < حجم(tl_test_struct_var.farr); k++) {
                TEST_ASSERT_EQUAL(0, tl_test_struct_var.farr[k]);
            }
        } ورنہ {
            TEST_ASSERT_EQUAL(test_var1_old + قدم, tl_test_var1);
            TEST_ASSERT_EQUAL(test_var2_old + قدم, tl_test_var2);
            TEST_ASSERT_EQUAL(test_var3_old + قدم, tl_test_var3);
            برائے (صحیح k = 0; k < حجم(tl_test_arr_var); k++) {
                TEST_ASSERT_EQUAL((i - 1) * قدم, tl_test_arr_var[k]);
            }
            TEST_ASSERT_EQUAL(f32_old + قدم, tl_test_struct_var.f32);
            TEST_ASSERT_EQUAL(f8_old + قدم, tl_test_struct_var.f8);
            TEST_ASSERT_EQUAL(f16_old + قدم, tl_test_struct_var.f16);
            برائے (صحیح k = 0; k < حجم(tl_test_struct_var.farr); k++) {
                TEST_ASSERT_EQUAL((i - 1) * قدم, tl_test_struct_var.farr[k]);
            }
        }

        test_var1_old = tl_test_var1;
        test_var2_old = tl_test_var2;
        test_var3_old = tl_test_var3;
        f32_old = tl_test_struct_var.f32;
        f8_old = tl_test_struct_var.f8;
        f16_old = tl_test_struct_var.f16;
        tl_test_var1 += قدم;
        tl_test_var2 += قدم;
        tl_test_var3 += قدم;
        memset(tl_test_arr_var, i * قدم, حجم(tl_test_arr_var));
        tl_test_struct_var.f32 += قدم;
        tl_test_struct_var.f8 += قدم;
        tl_test_struct_var.f16 += قدم;
        memset(tl_test_struct_var.farr, i * قدم, حجم(tl_test_struct_var.farr));
        vTaskDelay(10);
    }

    اگر (running) {
        *running = false;
        vTaskDelete(تہی);
    }
}

TEST_CASE("TLS test", "[freertos]")
{
    ثابت size_t ڈھیر_سائز_2 = 3072;
    ڈھیر_Type_t s_ڈھیر[ڈھیر_سائز_2]; /* with 8KB test task stack (default) this test still has ~3KB headroom */
    StaticTask_t s_task;
    bool running[2] = {true, true};
#اگر CONFIG_FREERTOS_UNICORE == 0
    صحیح other_core = 1;
#ورنہ
    صحیح other_core = 0;
#اگر_ختم

    x_Task_Create_Pinned_مقصد_Core((TaskFunction_t)&task_test_tls, "task_test_tls", ڈھیر_سائز_2, &running[0],
                            UNITY_FREERTOS_ترجیح, تہی, 0);
    x_Task_Create_Static_Pinned_مقصد_Core((TaskFunction_t)&task_test_tls, "task_test_tls", ڈھیر_سائز_2, &running[1],
                                  UNITY_FREERTOS_ترجیح, s_ڈھیر, &s_task, other_core);
    جب (running[0] || running[1]) {
        vTaskDelay(10);
    }
    vTaskDelay(10); /* Make sure idle task can clean up s_task, before it goes out of scope */
}
