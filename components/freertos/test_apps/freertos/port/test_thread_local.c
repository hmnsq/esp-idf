/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Test for thread local storage support.
*/

#అన్తర్భావ <string.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <esp_types.h>
#అన్తర్భావ <inttypes.h>

#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ "unity.h"
#అన్తర్భావ "test_utils.h"
#అన్తర్భావ "sdkconfig.h"

#ఒకవేళ defined(__XTENSA__)
#నిర్వచించు GET_THREADPTR(tp_dest)  చేయి { asm అస్థిర ("rur.threadptr %0":"=r"(tp_dest)); } అయితే(0)
#లేదావేళ defined (__riscv)
#నిర్వచించు GET_THREADPTR(tp_dest)  చేయి { నమోదు uint32_t _tp asm("tp"); tp_dest = _tp; } అయితే(0)
#అయితే_ముగింపు

స్థిర __thread పూర్ణాంకం tl_test_var1;
స్థిర __thread uint8_t tl_test_var2 = 55;
స్థిర __thread uint16_t tl_test_var3 = 44;
స్థిర __thread uint8_t tl_test_arr_var[10];
స్థిర __thread నిర్మాణం test_tls_var {
    పూర్ణాంకం f32;
    uint8_t f8;
    uint16_t f16;
    uint8_t farr[10];
} tl_test_struct_var;

స్థిర శూన్య task_test_tls(శూన్య *arg)
{
    bool *running = (bool *)arg;
    uint32_t tp = (uint32_t) -1;
    పూర్ణాంకం test_var1_old = 0;
    uint8_t test_var2_old = 0;
    uint16_t test_var3_old = 0;
    పూర్ణాంకం f32_old = 0;
    uint8_t f8_old = 0;
    uint16_t f16_old = 0;
    srand((పూర్ణాంకం) xTaskGetCurrentTaskHandle());
    పూర్ణాంకం అడుగు = (rand() % 10) + 1;

    GET_THREADPTR(tp);
    కొరకు (పూర్ణాంకం i = 0; i < 5; i++) {
        రాయి("Task[%"PRIx32"]: var1 = %d var2 = 0x%"PRIx8" step=%d\n", tp, tl_test_var1, tl_test_var2, అడుగు);
        ఒకవేళ (i == 0) {
            TEST_ASSERT_EQUAL(0, tl_test_var1);
            TEST_ASSERT_EQUAL(55, tl_test_var2);
            TEST_ASSERT_EQUAL(44, tl_test_var3);
            కొరకు (పూర్ణాంకం k = 0; k < పరిమాణం(tl_test_arr_var); k++) {
                TEST_ASSERT_EQUAL(0, tl_test_arr_var[k]);
            }
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f32);
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f8);
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f16);
            కొరకు (పూర్ణాంకం k = 0; k < పరిమాణం(tl_test_struct_var.farr); k++) {
                TEST_ASSERT_EQUAL(0, tl_test_struct_var.farr[k]);
            }
        } లేకపోతే {
            TEST_ASSERT_EQUAL(test_var1_old + అడుగు, tl_test_var1);
            TEST_ASSERT_EQUAL(test_var2_old + అడుగు, tl_test_var2);
            TEST_ASSERT_EQUAL(test_var3_old + అడుగు, tl_test_var3);
            కొరకు (పూర్ణాంకం k = 0; k < పరిమాణం(tl_test_arr_var); k++) {
                TEST_ASSERT_EQUAL((i - 1) * అడుగు, tl_test_arr_var[k]);
            }
            TEST_ASSERT_EQUAL(f32_old + అడుగు, tl_test_struct_var.f32);
            TEST_ASSERT_EQUAL(f8_old + అడుగు, tl_test_struct_var.f8);
            TEST_ASSERT_EQUAL(f16_old + అడుగు, tl_test_struct_var.f16);
            కొరకు (పూర్ణాంకం k = 0; k < పరిమాణం(tl_test_struct_var.farr); k++) {
                TEST_ASSERT_EQUAL((i - 1) * అడుగు, tl_test_struct_var.farr[k]);
            }
        }

        test_var1_old = tl_test_var1;
        test_var2_old = tl_test_var2;
        test_var3_old = tl_test_var3;
        f32_old = tl_test_struct_var.f32;
        f8_old = tl_test_struct_var.f8;
        f16_old = tl_test_struct_var.f16;
        tl_test_var1 += అడుగు;
        tl_test_var2 += అడుగు;
        tl_test_var3 += అడుగు;
        memset(tl_test_arr_var, i * అడుగు, పరిమాణం(tl_test_arr_var));
        tl_test_struct_var.f32 += అడుగు;
        tl_test_struct_var.f8 += అడుగు;
        tl_test_struct_var.f16 += అడుగు;
        memset(tl_test_struct_var.farr, i * అడుగు, పరిమాణం(tl_test_struct_var.farr));
        vTaskDelay(10);
    }

    ఒకవేళ (running) {
        *running = false;
        vTaskDelete(ఖాళీ);
    }
}

TEST_CASE("TLS test", "[freertos]")
{
    స్థిరరాశి size_t దొంతర_కొలత_2 = 3072;
    దొంతర_Type_t s_దొంతర[దొంతర_కొలత_2]; /* with 8KB test task stack (default) this test still has ~3KB headroom */
    StaticTask_t s_task;
    bool running[2] = {true, true};
#ఒకవేళ CONFIG_FREERTOS_UNICORE == 0
    పూర్ణాంకం other_core = 1;
#లేకపోతే
    పూర్ణాంకం other_core = 0;
#అయితే_ముగింపు

    x_Task_Create_Pinned_గమ్యం_Core((TaskFunction_t)&task_test_tls, "task_test_tls", దొంతర_కొలత_2, &running[0],
                            UNITY_FREERTOS_ప్రాధాన్యత, ఖాళీ, 0);
    x_Task_Create_Static_Pinned_గమ్యం_Core((TaskFunction_t)&task_test_tls, "task_test_tls", దొంతర_కొలత_2, &running[1],
                                  UNITY_FREERTOS_ప్రాధాన్యత, s_దొంతర, &s_task, other_core);
    అయితే (running[0] || running[1]) {
        vTaskDelay(10);
    }
    vTaskDelay(10); /* Make sure idle task can clean up s_task, before it goes out of scope */
}
