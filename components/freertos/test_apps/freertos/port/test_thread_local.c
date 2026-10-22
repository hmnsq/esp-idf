/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 Test for thread local storage support.
*/

#समावेश <string.h>
#समावेश <stdlib.h>
#समावेश <esp_types.h>
#समावेश <inttypes.h>

#समावेश "freertos/FreeRTOS.h"
#समावेश "freertos/task.h"
#समावेश "unity.h"
#समावेश "test_utils.h"
#समावेश "sdkconfig.h"

#अगर defined(__XTENSA__)
#घोषणा GET_THREADPTR(tp_dest)  करो { यंत्र अस्थिर ("rur.threadptr %0":"=r"(tp_dest)); } जबतक(0)
#या_अगर defined (__riscv)
#घोषणा GET_THREADPTR(tp_dest)  करो { रेजिस्टर uint32_t _tp यंत्र("tp"); tp_dest = _tp; } जबतक(0)
#पूर्ण_अगर

अटल __thread पूर्णांक tl_test_var1;
अटल __thread uint8_t tl_test_var2 = 55;
अटल __thread uint16_t tl_test_var3 = 44;
अटल __thread uint8_t tl_test_arr_var[10];
अटल __thread काष्ठा test_tls_var {
    पूर्णांक f32;
    uint8_t f8;
    uint16_t f16;
    uint8_t farr[10];
} tl_test_struct_var;

अटल व्योम task_test_tls(व्योम *arg)
{
    bool *running = (bool *)arg;
    uint32_t tp = (uint32_t) -1;
    पूर्णांक test_var1_old = 0;
    uint8_t test_var2_old = 0;
    uint16_t test_var3_old = 0;
    पूर्णांक f32_old = 0;
    uint8_t f8_old = 0;
    uint16_t f16_old = 0;
    बेक्रम((पूर्णांक) xTaskGetCurrentTaskHandle());
    पूर्णांक step = (अक्रम() % 10) + 1;

    GET_THREADPTR(tp);
    क्रम (पूर्णांक i = 0; i < 5; i++) {
        म_लिखो("Task[%"PRIx32"]: var1 = %d var2 = 0x%"PRIx8" step=%d\n", tp, tl_test_var1, tl_test_var2, step);
        अगर (i == 0) {
            TEST_ASSERT_EQUAL(0, tl_test_var1);
            TEST_ASSERT_EQUAL(55, tl_test_var2);
            TEST_ASSERT_EQUAL(44, tl_test_var3);
            क्रम (पूर्णांक k = 0; k < माप(tl_test_arr_var); k++) {
                TEST_ASSERT_EQUAL(0, tl_test_arr_var[k]);
            }
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f32);
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f8);
            TEST_ASSERT_EQUAL(0, tl_test_struct_var.f16);
            क्रम (पूर्णांक k = 0; k < माप(tl_test_struct_var.farr); k++) {
                TEST_ASSERT_EQUAL(0, tl_test_struct_var.farr[k]);
            }
        } अन्यथा {
            TEST_ASSERT_EQUAL(test_var1_old + step, tl_test_var1);
            TEST_ASSERT_EQUAL(test_var2_old + step, tl_test_var2);
            TEST_ASSERT_EQUAL(test_var3_old + step, tl_test_var3);
            क्रम (पूर्णांक k = 0; k < माप(tl_test_arr_var); k++) {
                TEST_ASSERT_EQUAL((i - 1) * step, tl_test_arr_var[k]);
            }
            TEST_ASSERT_EQUAL(f32_old + step, tl_test_struct_var.f32);
            TEST_ASSERT_EQUAL(f8_old + step, tl_test_struct_var.f8);
            TEST_ASSERT_EQUAL(f16_old + step, tl_test_struct_var.f16);
            क्रम (पूर्णांक k = 0; k < माप(tl_test_struct_var.farr); k++) {
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
        स_रखो(tl_test_arr_var, i * step, माप(tl_test_arr_var));
        tl_test_struct_var.f32 += step;
        tl_test_struct_var.f8 += step;
        tl_test_struct_var.f16 += step;
        स_रखो(tl_test_struct_var.farr, i * step, माप(tl_test_struct_var.farr));
        vTaskDelay(10);
    }

    अगर (running) {
        *running = false;
        vTaskDelete(शून्य);
    }
}

TEST_CASE("TLS test", "[freertos]")
{
    स्थिर माप_प्रकार ढेर_आकार_2 = 3072;
    ढेर_Type_t s_ढेर[ढेर_आकार_2]; /* with 8KB test task stack (default) this test still has ~3KB headroom */
    StaticTask_t s_task;
    bool running[2] = {true, true};
#अगर CONFIG_FREERTOS_UNICORE == 0
    पूर्णांक other_core = 1;
#अन्यथा
    पूर्णांक other_core = 0;
#पूर्ण_अगर

    xTaskCreatePinnedToCore((TaskFunction_t)&task_test_tls, "task_test_tls", ढेर_आकार_2, &running[0],
                            UNITY_FREERTOS_PRIORITY, शून्य, 0);
    xTaskCreateStaticPinnedToCore((TaskFunction_t)&task_test_tls, "task_test_tls", ढेर_आकार_2, &running[1],
                                  UNITY_FREERTOS_PRIORITY, s_ढेर, &s_task, other_core);
    जबतक (running[0] || running[1]) {
        vTaskDelay(10);
    }
    vTaskDelay(10); /* Make sure idle task can clean up s_task, before it goes out of scope */
}
