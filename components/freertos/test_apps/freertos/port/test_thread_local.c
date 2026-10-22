/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Test for thread local storage support.
*/

#அந்தர்பாவ <string.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <esp_types.h>
#அந்தர்பாவ <inttypes.h>

#அந்தர்பாவ "freertos/FreeRTOS.h"
#அந்தர்பாவ "freertos/task.h"
#அந்தர்பாவ "unity.h"
#அந்தர்பாவ "test_utils.h"
#அந்தர்பாவ "sdkconfig.h"

#எனில் defined(__XTENSA__)
#define GET_THREADPTR(tp_dest)  செய் { asm நிலையற்ற ("rur.threadptr %0":"=r"(tp_dest)); } போது(0)
#அல்லதெனில் defined (__riscv)
#define GET_THREADPTR(tp_dest)  செய் { பதிவு uint32_t _tp asm("tp"); tp_dest = _tp; } போது(0)
#endif

நிலையான __thread முழு tl_test_var1;
நிலையான __thread uint8_t tl_test_var2 = 55;
நிலையான __thread uint16_t tl_test_var3 = 44;
நிலையான __thread uint8_t tl_test_arr_var[10];
நிலையான __thread கட்டமைப்பு test_tls_var {
    முழு f32;
    uint8_t f8;
    uint16_t f16;
    uint8_t farr[10];
} tl_test_struct_var;

நிலையான வெறுமை task_test_tls(வெறுமை *arg)
{
    bool *running = (bool *)arg;
    uint32_t tp = (uint32_t) -1;
    முழு test_var1_old = 0;
    uint8_t test_var2_old = 0;
    uint16_t test_var3_old = 0;
    முழு f32_old = 0;
    uint8_t f8_old = 0;
    uint16_t f16_old = 0;
    srand((முழு) xTaskGetCurrentTaskHandle());
    முழு step = (rand() % 10) + 1;

    GET_THREADPTR(tp);
    க்கு (முழு i = 0; i < 5; i++) {
        printf("Task[%"PRIx32"]: var1 = %d var2 = 0x%"PRIx8" step=%d\n", tp, tl_test_var1, tl_test_var2, step);
        எனில் (i == 0) {
            TEST_ASSERT_EQUAL(0, tl_test_var1);
            TEST_ASSERT_EQUAL(55, tl_test_var2);
            TEST_ASSERT_EQUAL(44, tl_test_var3);
            க்கு (முழு k = 0; k < அளவு(tl_test_arr_var); k++) {
                TEST_ASSERT_EQUAL(0, tl_test_arr_var[k]);
            }
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f32);
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f8);
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f16);
            க்கு (முழு k = 0; k < அளவு(tl_test_struct_var.farr); k++) {
                TEST_ASSERT_EQUAL(0, tl_test_struct_var.farr[k]);
            }
        } இல்லையெனில் {
            TEST_ASSERT_EQUAL(test_var1_old + step, tl_test_var1);
            TEST_ASSERT_EQUAL(test_var2_old + step, tl_test_var2);
            TEST_ASSERT_EQUAL(test_var3_old + step, tl_test_var3);
            க்கு (முழு k = 0; k < அளவு(tl_test_arr_var); k++) {
                TEST_ASSERT_EQUAL((i - 1) * step, tl_test_arr_var[k]);
            }
            TEST_ASSERT_EQUAL(f32_old + step, tl_test_struct_var.f32);
            TEST_ASSERT_EQUAL(f8_old + step, tl_test_struct_var.f8);
            TEST_ASSERT_EQUAL(f16_old + step, tl_test_struct_var.f16);
            க்கு (முழு k = 0; k < அளவு(tl_test_struct_var.farr); k++) {
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
        memset(tl_test_arr_var, i * step, அளவு(tl_test_arr_var));
        tl_test_struct_var.f32 += step;
        tl_test_struct_var.f8 += step;
        tl_test_struct_var.f16 += step;
        memset(tl_test_struct_var.farr, i * step, அளவு(tl_test_struct_var.farr));
        vTaskDelay(10);
    }

    எனில் (running) {
        *running = false;
        vTaskDelete(வெற்று);
    }
}

TEST_CASE("TLS test", "[freertos]")
{
    மாறிலி size_t அடுக்கு_size = 3072;
    அடுக்கு_Type_t s_அடுக்கு[அடுக்கு_size]; /* with 8KB test task stack (default) this test still has ~3KB headroom */
    StaticTask_t s_task;
    bool running[2] = {true, true};
#எனில் CONFIG_FREERTOS_UNICORE == 0
    முழு other_core = 1;
#இல்லையெனில்
    முழு other_core = 0;
#endif

    xTaskCreatePinnedToCore((TaskFunction_t)&task_test_tls, "task_test_tls", அடுக்கு_size, &running[0],
                            UNITY_FREERTOS_PRIORITY, வெற்று, 0);
    xTaskCreateStaticPinnedToCore((TaskFunction_t)&task_test_tls, "task_test_tls", அடுக்கு_size, &running[1],
                                  UNITY_FREERTOS_PRIORITY, s_அடுக்கு, &s_task, other_core);
    போது (running[0] || running[1]) {
        vTaskDelay(10);
    }
    vTaskDelay(10); /* Make sure idle task can clean up s_task, before it goes out of scope */
}
