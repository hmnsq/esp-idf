/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Test for thread local storage support.
*/

#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <esp_types.h>
#ಅನ್ತರ್ಭಾವ <inttypes.h>

#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"
#ಅನ್ತರ್ಭಾವ "unity.h"
#ಅನ್ತರ್ಭಾವ "test_utils.h"
#ಅನ್ತರ್ಭಾವ "sdkconfig.h"

#ಯದಿ defined(__XTENSA__)
#define GET_THREADPTR(tp_dest)  ಕುರು { asm ಚಞ್ಚಲ ("rur.threadptr %0":"=r"(tp_dest)); } ಯಾವತ್(0)
#ನೋಚೇದ್ಯದಿ defined (__riscv)
#define GET_THREADPTR(tp_dest)  ಕುರು { ಪಞ್ಜಿಕಾ uint32_t _tp asm("tp"); tp_dest = _tp; } ಯಾವತ್(0)
#endif

ಸ್ಥಿರ __thread ಪೂರ್ಣಾಙ್ಕ tl_test_var1;
ಸ್ಥಿರ __thread uint8_t tl_test_var2 = 55;
ಸ್ಥಿರ __thread uint16_t tl_test_var3 = 44;
ಸ್ಥಿರ __thread uint8_t tl_test_arr_var[10];
ಸ್ಥಿರ __thread ಸಂರಚನಾ test_tls_var {
    ಪೂರ್ಣಾಙ್ಕ f32;
    uint8_t f8;
    uint16_t f16;
    uint8_t farr[10];
} tl_test_struct_var;

ಸ್ಥಿರ ಶೂನ್ಯ task_test_tls(ಶೂನ್ಯ *arg)
{
    bool *running = (bool *)arg;
    uint32_t tp = (uint32_t) -1;
    ಪೂರ್ಣಾಙ್ಕ test_var1_old = 0;
    uint8_t test_var2_old = 0;
    uint16_t test_var3_old = 0;
    ಪೂರ್ಣಾಙ್ಕ f32_old = 0;
    uint8_t f8_old = 0;
    uint16_t f16_old = 0;
    srand((ಪೂರ್ಣಾಙ್ಕ) xTaskGetCurrentTaskHandle());
    ಪೂರ್ಣಾಙ್ಕ step = (rand() % 10) + 1;

    GET_THREADPTR(tp);
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < 5; i++) {
        printf("Task[%"PRIx32"]: var1 = %d var2 = 0x%"PRIx8" step=%d\n", tp, tl_test_var1, tl_test_var2, step);
        ಯದಿ (i == 0) {
            TEST_ASSERT_EQUAL(0, tl_test_var1);
            TEST_ASSERT_EQUAL(55, tl_test_var2);
            TEST_ASSERT_EQUAL(44, tl_test_var3);
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < ಪರಿಮಾಣ(tl_test_arr_var); k++) {
                TEST_ASSERT_EQUAL(0, tl_test_arr_var[k]);
            }
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f32);
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f8);
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f16);
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < ಪರಿಮಾಣ(tl_test_struct_var.farr); k++) {
                TEST_ASSERT_EQUAL(0, tl_test_struct_var.farr[k]);
            }
        } ಅನ್ಯಥಾ {
            TEST_ASSERT_EQUAL(test_var1_old + step, tl_test_var1);
            TEST_ASSERT_EQUAL(test_var2_old + step, tl_test_var2);
            TEST_ASSERT_EQUAL(test_var3_old + step, tl_test_var3);
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < ಪರಿಮಾಣ(tl_test_arr_var); k++) {
                TEST_ASSERT_EQUAL((i - 1) * step, tl_test_arr_var[k]);
            }
            TEST_ASSERT_EQUAL(f32_old + step, tl_test_struct_var.f32);
            TEST_ASSERT_EQUAL(f8_old + step, tl_test_struct_var.f8);
            TEST_ASSERT_EQUAL(f16_old + step, tl_test_struct_var.f16);
            ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < ಪರಿಮಾಣ(tl_test_struct_var.farr); k++) {
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
        memset(tl_test_arr_var, i * step, ಪರಿಮಾಣ(tl_test_arr_var));
        tl_test_struct_var.f32 += step;
        tl_test_struct_var.f8 += step;
        tl_test_struct_var.f16 += step;
        memset(tl_test_struct_var.farr, i * step, ಪರಿಮಾಣ(tl_test_struct_var.farr));
        vTaskDelay(10);
    }

    ಯದಿ (running) {
        *running = false;
        vTaskDelete(ರಿಕ್ತ);
    }
}

TEST_CASE("TLS test", "[freertos]")
{
    ಧ್ರುವ size_t ರಾಶಿ_size = 3072;
    ರಾಶಿ_Type_t s_ರಾಶಿ[ರಾಶಿ_size]; /* with 8KB test task stack (default) this test still has ~3KB headroom */
    StaticTask_t s_task;
    bool running[2] = {true, true};
#ಯದಿ CONFIG_FREERTOS_UNICORE == 0
    ಪೂರ್ಣಾಙ್ಕ other_core = 1;
#ಅನ್ಯಥಾ
    ಪೂರ್ಣಾಙ್ಕ other_core = 0;
#endif

    xTaskCreatePinnedToCore((TaskFunction_t)&task_test_tls, "task_test_tls", ರಾಶಿ_size, &running[0],
                            UNITY_FREERTOS_PRIORITY, ರಿಕ್ತ, 0);
    xTaskCreateStaticPinnedToCore((TaskFunction_t)&task_test_tls, "task_test_tls", ರಾಶಿ_size, &running[1],
                                  UNITY_FREERTOS_PRIORITY, s_ರಾಶಿ, &s_task, other_core);
    ಯಾವತ್ (running[0] || running[1]) {
        vTaskDelay(10);
    }
    vTaskDelay(10); /* Make sure idle task can clean up s_task, before it goes out of scope */
}
