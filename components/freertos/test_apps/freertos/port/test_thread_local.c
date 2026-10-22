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

#ಆದರೆ defined(__XTENSA__)
#ವ್ಯಾಖ್ಯಾನಿಸು GET_THREADPTR(tp_dest)  ಮಾಡು { asm ಅಸ್ಥಿರ ("rur.threadptr %0":"=r"(tp_dest)); } ಆಗ(0)
#ಅಥವಾಆದರೆ defined (__riscv)
#ವ್ಯಾಖ್ಯಾನಿಸು GET_THREADPTR(tp_dest)  ಮಾಡು { ನೋಂದಣಿ uint32_t _tp asm("tp"); tp_dest = _tp; } ಆಗ(0)
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ __thread ಪೂರ್ಣಾಂಕ tl_test_var1;
ಸ್ಥಿರ __thread uint8_t tl_test_var2 = 55;
ಸ್ಥಿರ __thread uint16_t tl_test_var3 = 44;
ಸ್ಥಿರ __thread uint8_t tl_test_arr_var[10];
ಸ್ಥಿರ __thread ರಚನೆ test_tls_var {
    ಪೂರ್ಣಾಂಕ f32;
    uint8_t f8;
    uint16_t f16;
    uint8_t farr[10];
} tl_test_struct_var;

ಸ್ಥಿರ ಶೂನ್ಯ task_test_tls(ಶೂನ್ಯ *arg)
{
    bool *running = (bool *)arg;
    uint32_t tp = (uint32_t) -1;
    ಪೂರ್ಣಾಂಕ test_var1_old = 0;
    uint8_t test_var2_old = 0;
    uint16_t test_var3_old = 0;
    ಪೂರ್ಣಾಂಕ f32_old = 0;
    uint8_t f8_old = 0;
    uint16_t f16_old = 0;
    srand((ಪೂರ್ಣಾಂಕ) xTaskGetCurrentTaskHandle());
    ಪೂರ್ಣಾಂಕ ಹೆಜ್ಜೆ = (rand() % 10) + 1;

    GET_THREADPTR(tp);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 5; i++) {
        ಬರೆ("Task[%"PRIx32"]: var1 = %d var2 = 0x%"PRIx8" step=%d\n", tp, tl_test_var1, tl_test_var2, ಹೆಜ್ಜೆ);
        ಆದರೆ (i == 0) {
            TEST_ASSERT_EQUAL(0, tl_test_var1);
            TEST_ASSERT_EQUAL(55, tl_test_var2);
            TEST_ASSERT_EQUAL(44, tl_test_var3);
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < ಗಾತ್ರ(tl_test_arr_var); k++) {
                TEST_ASSERT_EQUAL(0, tl_test_arr_var[k]);
            }
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f32);
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f8);
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f16);
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < ಗಾತ್ರ(tl_test_struct_var.farr); k++) {
                TEST_ASSERT_EQUAL(0, tl_test_struct_var.farr[k]);
            }
        } ಇಲ್ಲದಿದ್ದರೆ {
            TEST_ASSERT_EQUAL(test_var1_old + ಹೆಜ್ಜೆ, tl_test_var1);
            TEST_ASSERT_EQUAL(test_var2_old + ಹೆಜ್ಜೆ, tl_test_var2);
            TEST_ASSERT_EQUAL(test_var3_old + ಹೆಜ್ಜೆ, tl_test_var3);
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < ಗಾತ್ರ(tl_test_arr_var); k++) {
                TEST_ASSERT_EQUAL((i - 1) * ಹೆಜ್ಜೆ, tl_test_arr_var[k]);
            }
            TEST_ASSERT_EQUAL(f32_old + ಹೆಜ್ಜೆ, tl_test_struct_var.f32);
            TEST_ASSERT_EQUAL(f8_old + ಹೆಜ್ಜೆ, tl_test_struct_var.f8);
            TEST_ASSERT_EQUAL(f16_old + ಹೆಜ್ಜೆ, tl_test_struct_var.f16);
            ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < ಗಾತ್ರ(tl_test_struct_var.farr); k++) {
                TEST_ASSERT_EQUAL((i - 1) * ಹೆಜ್ಜೆ, tl_test_struct_var.farr[k]);
            }
        }

        test_var1_old = tl_test_var1;
        test_var2_old = tl_test_var2;
        test_var3_old = tl_test_var3;
        f32_old = tl_test_struct_var.f32;
        f8_old = tl_test_struct_var.f8;
        f16_old = tl_test_struct_var.f16;
        tl_test_var1 += ಹೆಜ್ಜೆ;
        tl_test_var2 += ಹೆಜ್ಜೆ;
        tl_test_var3 += ಹೆಜ್ಜೆ;
        memset(tl_test_arr_var, i * ಹೆಜ್ಜೆ, ಗಾತ್ರ(tl_test_arr_var));
        tl_test_struct_var.f32 += ಹೆಜ್ಜೆ;
        tl_test_struct_var.f8 += ಹೆಜ್ಜೆ;
        tl_test_struct_var.f16 += ಹೆಜ್ಜೆ;
        memset(tl_test_struct_var.farr, i * ಹೆಜ್ಜೆ, ಗಾತ್ರ(tl_test_struct_var.farr));
        vTaskDelay(10);
    }

    ಆದರೆ (running) {
        *running = false;
        vTaskDelete(ಖಾಲಿ);
    }
}

TEST_CASE("TLS test", "[freertos]")
{
    ಸ್ಥಿರಾಂಕ size_t ರಾಶಿ_ಗಾತ್ರ_2 = 3072;
    ರಾಶಿ_Type_t s_ರಾಶಿ[ರಾಶಿ_ಗಾತ್ರ_2]; /* with 8KB test task stack (default) this test still has ~3KB headroom */
    StaticTask_t s_task;
    bool running[2] = {true, true};
#ಆದರೆ CONFIG_FREERTOS_UNICORE == 0
    ಪೂರ್ಣಾಂಕ other_core = 1;
#ಇಲ್ಲದಿದ್ದರೆ
    ಪೂರ್ಣಾಂಕ other_core = 0;
#ಆದರೆ_ಅಂತ್ಯ

    x_Task_Create_Pinned_ಗಮ್ಯ_Core((TaskFunction_t)&task_test_tls, "task_test_tls", ರಾಶಿ_ಗಾತ್ರ_2, &running[0],
                            UNITY_FREERTOS_ಆದ್ಯತೆ, ಖಾಲಿ, 0);
    x_Task_Create_Static_Pinned_ಗಮ್ಯ_Core((TaskFunction_t)&task_test_tls, "task_test_tls", ರಾಶಿ_ಗಾತ್ರ_2, &running[1],
                                  UNITY_FREERTOS_ಆದ್ಯತೆ, s_ರಾಶಿ, &s_task, other_core);
    ಆಗ (running[0] || running[1]) {
        vTaskDelay(10);
    }
    vTaskDelay(10); /* Make sure idle task can clean up s_task, before it goes out of scope */
}
