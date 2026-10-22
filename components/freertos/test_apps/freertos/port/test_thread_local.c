/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Test for thread local storage support.
*/

#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <esp_types.h>
#અન્તર્ભાવ <inttypes.h>

#અન્તર્ભાવ "freertos/FreeRTOS.h"
#અન્તર્ભાવ "freertos/task.h"
#અન્તર્ભાવ "unity.h"
#અન્તર્ભાવ "test_utils.h"
#અન્તર્ભાવ "sdkconfig.h"

#જો defined(__XTENSA__)
#define GET_THREADPTR(tp_dest)  કરો { asm અસ્થિર ("rur.threadptr %0":"=r"(tp_dest)); } જ્યારે(0)
#અથવાજો defined (__riscv)
#define GET_THREADPTR(tp_dest)  કરો { નોંધણી uint32_t _tp asm("tp"); tp_dest = _tp; } જ્યારે(0)
#endif

સ્થિર __thread પૂર્ણાંક tl_test_var1;
સ્થિર __thread uint8_t tl_test_var2 = 55;
સ્થિર __thread uint16_t tl_test_var3 = 44;
સ્થિર __thread uint8_t tl_test_arr_var[10];
સ્થિર __thread સંરચના test_tls_var {
    પૂર્ણાંક f32;
    uint8_t f8;
    uint16_t f16;
    uint8_t farr[10];
} tl_test_struct_var;

સ્થિર શૂન્ય task_test_tls(શૂન્ય *arg)
{
    bool *running = (bool *)arg;
    uint32_t tp = (uint32_t) -1;
    પૂર્ણાંક test_var1_old = 0;
    uint8_t test_var2_old = 0;
    uint16_t test_var3_old = 0;
    પૂર્ણાંક f32_old = 0;
    uint8_t f8_old = 0;
    uint16_t f16_old = 0;
    srand((પૂર્ણાંક) xTaskGetCurrentTaskHandle());
    પૂર્ણાંક step = (rand() % 10) + 1;

    GET_THREADPTR(tp);
    માટે (પૂર્ણાંક i = 0; i < 5; i++) {
        printf("Task[%"PRIx32"]: var1 = %d var2 = 0x%"PRIx8" step=%d\n", tp, tl_test_var1, tl_test_var2, step);
        જો (i == 0) {
            TEST_ASSERT_EQUAL(0, tl_test_var1);
            TEST_ASSERT_EQUAL(55, tl_test_var2);
            TEST_ASSERT_EQUAL(44, tl_test_var3);
            માટે (પૂર્ણાંક k = 0; k < કદ(tl_test_arr_var); k++) {
                TEST_ASSERT_EQUAL(0, tl_test_arr_var[k]);
            }
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f32);
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f8);
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f16);
            માટે (પૂર્ણાંક k = 0; k < કદ(tl_test_struct_var.farr); k++) {
                TEST_ASSERT_EQUAL(0, tl_test_struct_var.farr[k]);
            }
        } નહીંતર {
            TEST_ASSERT_EQUAL(test_var1_old + step, tl_test_var1);
            TEST_ASSERT_EQUAL(test_var2_old + step, tl_test_var2);
            TEST_ASSERT_EQUAL(test_var3_old + step, tl_test_var3);
            માટે (પૂર્ણાંક k = 0; k < કદ(tl_test_arr_var); k++) {
                TEST_ASSERT_EQUAL((i - 1) * step, tl_test_arr_var[k]);
            }
            TEST_ASSERT_EQUAL(f32_old + step, tl_test_struct_var.f32);
            TEST_ASSERT_EQUAL(f8_old + step, tl_test_struct_var.f8);
            TEST_ASSERT_EQUAL(f16_old + step, tl_test_struct_var.f16);
            માટે (પૂર્ણાંક k = 0; k < કદ(tl_test_struct_var.farr); k++) {
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
        memset(tl_test_arr_var, i * step, કદ(tl_test_arr_var));
        tl_test_struct_var.f32 += step;
        tl_test_struct_var.f8 += step;
        tl_test_struct_var.f16 += step;
        memset(tl_test_struct_var.farr, i * step, કદ(tl_test_struct_var.farr));
        vTaskDelay(10);
    }

    જો (running) {
        *running = false;
        vTaskDelete(ખાલી);
    }
}

TEST_CASE("TLS test", "[freertos]")
{
    અચલ size_t ઢગલો_size = 3072;
    ઢગલો_Type_t s_ઢગલો[ઢગલો_size]; /* with 8KB test task stack (default) this test still has ~3KB headroom */
    StaticTask_t s_task;
    bool running[2] = {true, true};
#જો CONFIG_FREERTOS_UNICORE == 0
    પૂર્ણાંક other_core = 1;
#નહીંતર
    પૂર્ણાંક other_core = 0;
#endif

    xTaskCreatePinnedToCore((TaskFunction_t)&task_test_tls, "task_test_tls", ઢગલો_size, &running[0],
                            UNITY_FREERTOS_PRIORITY, ખાલી, 0);
    xTaskCreateStaticPinnedToCore((TaskFunction_t)&task_test_tls, "task_test_tls", ઢગલો_size, &running[1],
                                  UNITY_FREERTOS_PRIORITY, s_ઢગલો, &s_task, other_core);
    જ્યારે (running[0] || running[1]) {
        vTaskDelay(10);
    }
    vTaskDelay(10); /* Make sure idle task can clean up s_task, before it goes out of scope */
}
