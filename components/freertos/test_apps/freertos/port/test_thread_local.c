/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Test for thread local storage support.
*/

#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <esp_types.h>
#അന്തര്ഭാവ <inttypes.h>

#അന്തര്ഭാവ "freertos/FreeRTOS.h"
#അന്തര്ഭാവ "freertos/task.h"
#അന്തര്ഭാവ "unity.h"
#അന്തര്ഭാവ "test_utils.h"
#അന്തര്ഭാവ "sdkconfig.h"

#എങ്കിൽ defined(__XTENSA__)
#define GET_THREADPTR(tp_dest)  ചെയ്യുക { asm അസ്ഥിര ("rur.threadptr %0":"=r"(tp_dest)); } ആകുമ്പോൾ(0)
#അല്ലെങ്കിലെങ്കിൽ defined (__riscv)
#define GET_THREADPTR(tp_dest)  ചെയ്യുക { രജിസ്റ്റർ uint32_t _tp asm("tp"); tp_dest = _tp; } ആകുമ്പോൾ(0)
#endif

സ്ഥിതം __thread പൂർണ്ണസംഖ്യ tl_test_var1;
സ്ഥിതം __thread uint8_t tl_test_var2 = 55;
സ്ഥിതം __thread uint16_t tl_test_var3 = 44;
സ്ഥിതം __thread uint8_t tl_test_arr_var[10];
സ്ഥിതം __thread ഘടന test_tls_var {
    പൂർണ്ണസംഖ്യ f32;
    uint8_t f8;
    uint16_t f16;
    uint8_t farr[10];
} tl_test_struct_var;

സ്ഥിതം ശൂന്യം task_test_tls(ശൂന്യം *arg)
{
    bool *running = (bool *)arg;
    uint32_t tp = (uint32_t) -1;
    പൂർണ്ണസംഖ്യ test_var1_old = 0;
    uint8_t test_var2_old = 0;
    uint16_t test_var3_old = 0;
    പൂർണ്ണസംഖ്യ f32_old = 0;
    uint8_t f8_old = 0;
    uint16_t f16_old = 0;
    srand((പൂർണ്ണസംഖ്യ) xTaskGetCurrentTaskHandle());
    പൂർണ്ണസംഖ്യ step = (rand() % 10) + 1;

    GET_THREADPTR(tp);
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < 5; i++) {
        printf("Task[%"PRIx32"]: var1 = %d var2 = 0x%"PRIx8" step=%d\n", tp, tl_test_var1, tl_test_var2, step);
        എങ്കിൽ (i == 0) {
            TEST_ASSERT_EQUAL(0, tl_test_var1);
            TEST_ASSERT_EQUAL(55, tl_test_var2);
            TEST_ASSERT_EQUAL(44, tl_test_var3);
            വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < വലുപ്പം(tl_test_arr_var); k++) {
                TEST_ASSERT_EQUAL(0, tl_test_arr_var[k]);
            }
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f32);
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f8);
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f16);
            വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < വലുപ്പം(tl_test_struct_var.farr); k++) {
                TEST_ASSERT_EQUAL(0, tl_test_struct_var.farr[k]);
            }
        } അല്ലെങ്കിൽ {
            TEST_ASSERT_EQUAL(test_var1_old + step, tl_test_var1);
            TEST_ASSERT_EQUAL(test_var2_old + step, tl_test_var2);
            TEST_ASSERT_EQUAL(test_var3_old + step, tl_test_var3);
            വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < വലുപ്പം(tl_test_arr_var); k++) {
                TEST_ASSERT_EQUAL((i - 1) * step, tl_test_arr_var[k]);
            }
            TEST_ASSERT_EQUAL(f32_old + step, tl_test_struct_var.f32);
            TEST_ASSERT_EQUAL(f8_old + step, tl_test_struct_var.f8);
            TEST_ASSERT_EQUAL(f16_old + step, tl_test_struct_var.f16);
            വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < വലുപ്പം(tl_test_struct_var.farr); k++) {
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
        memset(tl_test_arr_var, i * step, വലുപ്പം(tl_test_arr_var));
        tl_test_struct_var.f32 += step;
        tl_test_struct_var.f8 += step;
        tl_test_struct_var.f16 += step;
        memset(tl_test_struct_var.farr, i * step, വലുപ്പം(tl_test_struct_var.farr));
        vTaskDelay(10);
    }

    എങ്കിൽ (running) {
        *running = false;
        vTaskDelete(ശൂന്യ);
    }
}

TEST_CASE("TLS test", "[freertos]")
{
    സ്ഥിരം size_t അട്ടി_size = 3072;
    അട്ടി_Type_t s_അട്ടി[അട്ടി_size]; /* with 8KB test task stack (default) this test still has ~3KB headroom */
    StaticTask_t s_task;
    bool running[2] = {true, true};
#എങ്കിൽ CONFIG_FREERTOS_UNICORE == 0
    പൂർണ്ണസംഖ്യ other_core = 1;
#അല്ലെങ്കിൽ
    പൂർണ്ണസംഖ്യ other_core = 0;
#endif

    xTaskCreatePinnedToCore((TaskFunction_t)&task_test_tls, "task_test_tls", അട്ടി_size, &running[0],
                            UNITY_FREERTOS_PRIORITY, ശൂന്യ, 0);
    xTaskCreateStaticPinnedToCore((TaskFunction_t)&task_test_tls, "task_test_tls", അട്ടി_size, &running[1],
                                  UNITY_FREERTOS_PRIORITY, s_അട്ടി, &s_task, other_core);
    ആകുമ്പോൾ (running[0] || running[1]) {
        vTaskDelay(10);
    }
    vTaskDelay(10); /* Make sure idle task can clean up s_task, before it goes out of scope */
}
