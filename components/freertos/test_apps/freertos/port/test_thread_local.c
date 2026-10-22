/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Test for thread local storage support.
*/

#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <esp_types.h>
#ଅନ୍ତର୍ଭାଵ <inttypes.h>

#ଅନ୍ତର୍ଭାଵ "freertos/FreeRTOS.h"
#ଅନ୍ତର୍ଭାଵ "freertos/task.h"
#ଅନ୍ତର୍ଭାଵ "unity.h"
#ଅନ୍ତର୍ଭାଵ "test_utils.h"
#ଅନ୍ତର୍ଭାଵ "sdkconfig.h"

#ଯଦି defined(__XTENSA__)
#define GET_THREADPTR(tp_dest)  କର { asm ଅସ୍ଥିର ("rur.threadptr %0":"=r"(tp_dest)); } ଯେତେବେଳେ(0)
#ନଚେତଯଦି defined (__riscv)
#define GET_THREADPTR(tp_dest)  କର { ପଞ୍ଜିକରଣ uint32_t _tp asm("tp"); tp_dest = _tp; } ଯେତେବେଳେ(0)
#endif

ସ୍ଥିର __thread ପୂର୍ଣ୍ଣସଂଖ୍ୟା tl_test_var1;
ସ୍ଥିର __thread uint8_t tl_test_var2 = 55;
ସ୍ଥିର __thread uint16_t tl_test_var3 = 44;
ସ୍ଥିର __thread uint8_t tl_test_arr_var[10];
ସ୍ଥିର __thread ସଂରଚନା test_tls_var {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା f32;
    uint8_t f8;
    uint16_t f16;
    uint8_t farr[10];
} tl_test_struct_var;

ସ୍ଥିର ଶୂନ୍ୟ task_test_tls(ଶୂନ୍ୟ *arg)
{
    bool *running = (bool *)arg;
    uint32_t tp = (uint32_t) -1;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା test_var1_old = 0;
    uint8_t test_var2_old = 0;
    uint16_t test_var3_old = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା f32_old = 0;
    uint8_t f8_old = 0;
    uint16_t f16_old = 0;
    srand((ପୂର୍ଣ୍ଣସଂଖ୍ୟା) xTaskGetCurrentTaskHandle());
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା step = (rand() % 10) + 1;

    GET_THREADPTR(tp);
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < 5; i++) {
        printf("Task[%"PRIx32"]: var1 = %d var2 = 0x%"PRIx8" step=%d\n", tp, tl_test_var1, tl_test_var2, step);
        ଯଦି (i == 0) {
            TEST_ASSERT_EQUAL(0, tl_test_var1);
            TEST_ASSERT_EQUAL(55, tl_test_var2);
            TEST_ASSERT_EQUAL(44, tl_test_var3);
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < ଆକାର(tl_test_arr_var); k++) {
                TEST_ASSERT_EQUAL(0, tl_test_arr_var[k]);
            }
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f32);
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f8);
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f16);
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < ଆକାର(tl_test_struct_var.farr); k++) {
                TEST_ASSERT_EQUAL(0, tl_test_struct_var.farr[k]);
            }
        } ଅନ୍ୟଥା {
            TEST_ASSERT_EQUAL(test_var1_old + step, tl_test_var1);
            TEST_ASSERT_EQUAL(test_var2_old + step, tl_test_var2);
            TEST_ASSERT_EQUAL(test_var3_old + step, tl_test_var3);
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < ଆକାର(tl_test_arr_var); k++) {
                TEST_ASSERT_EQUAL((i - 1) * step, tl_test_arr_var[k]);
            }
            TEST_ASSERT_EQUAL(f32_old + step, tl_test_struct_var.f32);
            TEST_ASSERT_EQUAL(f8_old + step, tl_test_struct_var.f8);
            TEST_ASSERT_EQUAL(f16_old + step, tl_test_struct_var.f16);
            ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < ଆକାର(tl_test_struct_var.farr); k++) {
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
        memset(tl_test_arr_var, i * step, ଆକାର(tl_test_arr_var));
        tl_test_struct_var.f32 += step;
        tl_test_struct_var.f8 += step;
        tl_test_struct_var.f16 += step;
        memset(tl_test_struct_var.farr, i * step, ଆକାର(tl_test_struct_var.farr));
        vTaskDelay(10);
    }

    ଯଦି (running) {
        *running = false;
        vTaskDelete(ଖାଲି);
    }
}

TEST_CASE("TLS test", "[freertos]")
{
    ଅଚଳ size_t ଗଦା_size = 3072;
    ଗଦା_Type_t s_ଗଦା[ଗଦା_size]; /* with 8KB test task stack (default) this test still has ~3KB headroom */
    StaticTask_t s_task;
    bool running[2] = {true, true};
#ଯଦି CONFIG_FREERTOS_UNICORE == 0
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା other_core = 1;
#ଅନ୍ୟଥା
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା other_core = 0;
#endif

    xTaskCreatePinnedToCore((TaskFunction_t)&task_test_tls, "task_test_tls", ଗଦା_size, &running[0],
                            UNITY_FREERTOS_PRIORITY, ଖାଲି, 0);
    xTaskCreateStaticPinnedToCore((TaskFunction_t)&task_test_tls, "task_test_tls", ଗଦା_size, &running[1],
                                  UNITY_FREERTOS_PRIORITY, s_ଗଦା, &s_task, other_core);
    ଯେତେବେଳେ (running[0] || running[1]) {
        vTaskDelay(10);
    }
    vTaskDelay(10); /* Make sure idle task can clean up s_task, before it goes out of scope */
}
