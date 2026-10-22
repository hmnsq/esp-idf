/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Test for thread local storage support.
*/

#ضمن <string.h>
#ضمن <stdlib.h>
#ضمن <esp_types.h>
#ضمن <inttypes.h>

#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"
#ضمن "unity.h"
#ضمن "test_utils.h"
#ضمن "sdkconfig.h"

#إذا defined(__XTENSA__)
#define GET_THREADPTR(tp_dest)  افعل { asm متطاير ("rur.threadptr %0":"=r"(tp_dest)); } طالما(0)
#وإلاإذا defined (__riscv)
#define GET_THREADPTR(tp_dest)  افعل { سجل uint32_t _tp asm("tp"); tp_dest = _tp; } طالما(0)
#endif

ساكن __thread صحيح tl_test_var1;
ساكن __thread uint8_t tl_test_var2 = 55;
ساكن __thread uint16_t tl_test_var3 = 44;
ساكن __thread uint8_t tl_test_arr_var[10];
ساكن __thread بنية test_tls_var {
    صحيح f32;
    uint8_t f8;
    uint16_t f16;
    uint8_t farr[10];
} tl_test_struct_var;

ساكن فراغ task_test_tls(فراغ *arg)
{
    bool *running = (bool *)arg;
    uint32_t tp = (uint32_t) -1;
    صحيح test_var1_old = 0;
    uint8_t test_var2_old = 0;
    uint16_t test_var3_old = 0;
    صحيح f32_old = 0;
    uint8_t f8_old = 0;
    uint16_t f16_old = 0;
    srand((صحيح) xTaskGetCurrentTaskHandle());
    صحيح step = (rand() % 10) + 1;

    GET_THREADPTR(tp);
    لكل (صحيح i = 0; i < 5; i++) {
        printf("Task[%"PRIx32"]: var1 = %d var2 = 0x%"PRIx8" step=%d\n", tp, tl_test_var1, tl_test_var2, step);
        إذا (i == 0) {
            TEST_ASSERT_EQUAL(0, tl_test_var1);
            TEST_ASSERT_EQUAL(55, tl_test_var2);
            TEST_ASSERT_EQUAL(44, tl_test_var3);
            لكل (صحيح k = 0; k < حجم(tl_test_arr_var); k++) {
                TEST_ASSERT_EQUAL(0, tl_test_arr_var[k]);
            }
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f32);
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f8);
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f16);
            لكل (صحيح k = 0; k < حجم(tl_test_struct_var.farr); k++) {
                TEST_ASSERT_EQUAL(0, tl_test_struct_var.farr[k]);
            }
        } وإلا {
            TEST_ASSERT_EQUAL(test_var1_old + step, tl_test_var1);
            TEST_ASSERT_EQUAL(test_var2_old + step, tl_test_var2);
            TEST_ASSERT_EQUAL(test_var3_old + step, tl_test_var3);
            لكل (صحيح k = 0; k < حجم(tl_test_arr_var); k++) {
                TEST_ASSERT_EQUAL((i - 1) * step, tl_test_arr_var[k]);
            }
            TEST_ASSERT_EQUAL(f32_old + step, tl_test_struct_var.f32);
            TEST_ASSERT_EQUAL(f8_old + step, tl_test_struct_var.f8);
            TEST_ASSERT_EQUAL(f16_old + step, tl_test_struct_var.f16);
            لكل (صحيح k = 0; k < حجم(tl_test_struct_var.farr); k++) {
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
        memset(tl_test_arr_var, i * step, حجم(tl_test_arr_var));
        tl_test_struct_var.f32 += step;
        tl_test_struct_var.f8 += step;
        tl_test_struct_var.f16 += step;
        memset(tl_test_struct_var.farr, i * step, حجم(tl_test_struct_var.farr));
        vTaskDelay(10);
    }

    إذا (running) {
        *running = false;
        vTaskDelete(عدم);
    }
}

TEST_CASE("TLS test", "[freertos]")
{
    ثابت size_t تيندنن_size = 3072;
    تيندنن_Type_t s_تيندنن[تيندنن_size]; /* with 8KB test task stack (default) this test still has ~3KB headroom */
    StaticTask_t s_task;
    bool running[2] = {true, true};
#إذا CONFIG_FREERTOS_UNICORE == 0
    صحيح other_core = 1;
#وإلا
    صحيح other_core = 0;
#endif

    xTaskCreatePinnedToCore((TaskFunction_t)&task_test_tls, "task_test_tls", تيندنن_size, &running[0],
                            UNITY_FREERTOS_PRIORITY, عدم, 0);
    xTaskCreateStaticPinnedToCore((TaskFunction_t)&task_test_tls, "task_test_tls", تيندنن_size, &running[1],
                                  UNITY_FREERTOS_PRIORITY, s_تيندنن, &s_task, other_core);
    طالما (running[0] || running[1]) {
        vTaskDelay(10);
    }
    vTaskDelay(10); /* Make sure idle task can clean up s_task, before it goes out of scope */
}
