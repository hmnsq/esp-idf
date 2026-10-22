/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#شامل "sdkconfig.h"
#شامل "FreeRTOS.h"
#شامل "task.h"
#شامل "semphr.h"
#شامل "unity.h"
#شامل "freertos_test_utils.h"
#شامل "portTestMacro.h"

/* ------------------------------------------------------------------------------------------------------------------ */
#اگر SOC_GPTIMER_SUPPORTED

/*
Test vTaskDelay

Purpose:
    - Test that vTaskDelay is accurate
Procedure:
    - The test contains TEST_VTASKDELAY_ITERATIONS number of iterations. For each iteration...
        - vTaskDelay(1) to align to next tick boundary
        - Store current tick count and current time (using ref clock)
        - vTaskDelay for TEST_VTASKDELAY_TICKS
        - Get post delay tick count and ref clock time
    - For single core, run the test directly from the UnityTask
    - For SMP, run the test once on each core (using vTestOnAllCores())
Expected:
    - The elapsed ticks should be TEST_VTASKDELAY_TICKS, with TEST_VTASKDELAY_DELTA_TICKS error allowed (in case the
      delay and ref clock functions last long enough to cross a tick boundary).
    - The elapsed time should be equivalent to TEST_VTASKDELAY_TICKS tick periods, with TEST_VTASKDELAY_TICKS tick
      period of error allowed (in case ref clock functions last longer that a tick period).
*/

#اگر ( INCLUDE_vTaskDelay == 1 )

#تعریف_کرو TEST_VTASKDELAY_TICKS           5   // Number of ticks to delay in test
#تعریف_کرو TEST_VTASKDELAY_ITERATIONS      5   // Number of iterations in test
#اگر CONFIG_FREERTOS_SMP
#تعریف_کرو TEST_VTASKDELAY_DELTA_TICKS     1   // Number of ticks worth of delta allowed
#ورنہ
#تعریف_کرو TEST_VTASKDELAY_DELTA_TICKS     2   // Number of ticks worth of delta allowed. We allow 2 ticks in IDF FreeRTOS as each core's tick interrupt could be out of phase
#اگر_ختم

ساکن خالی test_vTaskDelay(خالی *arg)
{
    برائے (صحیح i = 0; i < TEST_VTASKDELAY_ITERATIONS; i++) {
        TickType_t tick_آغاز, tick_انتہا;
        پورٹ_TEST_REF_CLOCK_TYPE ref_clock_آغاز, ref_clock_انتہا;

        /* Delay until the next tick boundary */
        vTaskDelay(1);

        /* Get the current tick count and ref clock time */
        tick_آغاز = x_Task_Get_Tick_گنتی();
        ref_clock_آغاز = پورٹ_TEST_REF_CLOCK_GET_وقت();

        vTaskDelay(TEST_VTASKDELAY_TICKS);

        /* Get the post delay tick count and ref clock time */
        tick_انتہا = x_Task_Get_Tick_گنتی();
        ref_clock_انتہا = پورٹ_TEST_REF_CLOCK_GET_وقت();

        /* Check that elapsed ticks and ref clock is accurate. We allow TEST_VTASKDELAY_DELTA_TICKS of error in case
         * vTaskDelay() or portTEST_REF_CLOCK_GET_TIME() last long enough to cross a tick boundary */
#اگر ( config_USE_16_ثنائی_ہندسہ_TICKS == 1 )
        TEST_ASSERT_UINT16_WITHIN(TEST_VTASKDELAY_DELTA_TICKS, TEST_VTASKDELAY_TICKS, tick_انتہا - tick_آغاز);
#ورنہ
        TEST_ASSERT_UINT32_WITHIN(TEST_VTASKDELAY_DELTA_TICKS, TEST_VTASKDELAY_TICKS, tick_انتہا - tick_آغاز);
#اگر_ختم
        TEST_ASSERT_UINT32_WITHIN(پورٹ_TEST_TICKS_مقصد_REF_CLOCK(TEST_VTASKDELAY_DELTA_TICKS),
                                  پورٹ_TEST_TICKS_مقصد_REF_CLOCK(TEST_VTASKDELAY_TICKS),
                                  ref_clock_انتہا - ref_clock_آغاز);
    }
}

TEST_CASE("Tasks: Test vTaskDelay", "[freertos]")
{
    پورٹ_TEST_REF_CLOCK_INIT();

#اگر ( CONFIG_FREERTOS_نمبر_کا_CORES > 1 )
    vTestOnAllCores(test_vTaskDelay, تہی, config_TEST_DEFAULT_ڈھیر_سائز, config_TEST_UNITY_TASK_ترجیح - 1);
#ورنہ
    /* Test vTaskDelay directly on the current core */
    test_vTaskDelay(تہی);
#اگر_ختم

    پورٹ_TEST_REF_CLOCK_DEINIT();
}

#اگر_ختم /* ( INCLUDE_vTaskDelay == 1 ) */

/* ------------------------------------------------------------------------------------------------------------------ */

/*
Test vTaskDelayUntil

Purpose:
    - Test that vTaskDelayUntil is accurate
Procedure:
    - The test contains TEST_VTASKDELAYUNTIL_ITERATIONS number of iterations. For each iteration...
        - vTaskDelay(1) to align to next tick boundary
        - Store current tick count and current time (using ref clock)
        - Call vTaskDelayUntil() for TEST_VTASKDELAYUNTIL_TICKS, using the stored tick count as the previous wake time
        - Get post delay tick count and ref clock time
    - For single core, run the test directly from the UnityTask
    - For SMP, run the test once on each core (using vTestOnAllCores())
Expected:
    - The elapsed ticks should be TEST_VTASKDELAYUNTIL_TICKS, with TEST_VTASKDELAYUNTIL_DELTA_TICKS tick of error
      allowed (in case the delay and ref clock functions last long enough to cross a tick boundary).
    - The elapsed time should be equivalent to TEST_VTASKDELAYUNTIL_TICKS tick periods, with
      TEST_VTASKDELAYUNTIL_DELTA_TICKS tick period of error allowed (in case ref clock functions last longer that a tick
      period).
*/

#اگر ( INCLUDE_xTaskDelayUntil == 1 )

#تعریف_کرو TEST_VTASKDELAYUNTIL_TICKS              5   // Number of ticks to delay in test
#تعریف_کرو TEST_VTASKDELAYUNTIL_ITERATIONS         5   // Number of iterations in test
#اگر CONFIG_FREERTOS_SMP
#تعریف_کرو TEST_VTASKDELAYUNTIL_DELTA_TICKS        1   // Number of ticks worth of delta allowed
#ورنہ
#تعریف_کرو TEST_VTASKDELAYUNTIL_DELTA_TICKS        2   // Number of ticks worth of delta allowed. We allow 2 ticks in IDF FreeRTOS as each core's tick interrupt could be out of phase
#اگر_ختم

ساکن خالی test_vTaskDelayUntil(خالی *arg)
{
    برائے (صحیح i = 0; i < TEST_VTASKDELAYUNTIL_ITERATIONS; i++) {
        TickType_t tick_آغاز, tick_انتہا, آخری_wake_tick;
        پورٹ_TEST_REF_CLOCK_TYPE ref_clock_آغاز, ref_clock_انتہا;

        /* Delay until the next tick boundary */
        vTaskDelay(1);

        /* Get the current tick count and ref clock time */
        tick_آغاز = x_Task_Get_Tick_گنتی();
        آخری_wake_tick = tick_آغاز;
        ref_clock_آغاز = پورٹ_TEST_REF_CLOCK_GET_وقت();

        vTaskDelayUntil(&آخری_wake_tick, TEST_VTASKDELAYUNTIL_TICKS);

        /* Get the post delay tick count and ref clock time */
        tick_انتہا = x_Task_Get_Tick_گنتی();
        ref_clock_انتہا = پورٹ_TEST_REF_CLOCK_GET_وقت();

        /* Check that elapsed ticks and ref clock is accurate. We allow TEST_VTASKDELAYUNTIL_DELTA_TICKS of error in
         * case vTaskDelayUntil() or portTEST_REF_CLOCK_GET_TIME() last long enough to cross a tick boundary */
#اگر ( config_USE_16_ثنائی_ہندسہ_TICKS == 1 )
        TEST_ASSERT_UINT16_WITHIN(TEST_VTASKDELAYUNTIL_DELTA_TICKS, TEST_VTASKDELAYUNTIL_TICKS, tick_انتہا - tick_آغاز);
        TEST_ASSERT_UINT16_WITHIN(TEST_VTASKDELAYUNTIL_DELTA_TICKS, tick_انتہا, آخری_wake_tick);
#ورنہ
        TEST_ASSERT_UINT32_WITHIN(TEST_VTASKDELAYUNTIL_DELTA_TICKS, TEST_VTASKDELAYUNTIL_TICKS, tick_انتہا - tick_آغاز);
        TEST_ASSERT_UINT32_WITHIN(TEST_VTASKDELAYUNTIL_DELTA_TICKS, tick_انتہا, آخری_wake_tick);
#اگر_ختم

        /* Check that the elapsed ref clock time is accurate. We allow TEST_VTASKDELAYUNTIL_DELTA_TICKS time worth of
         * error to account for the execution time of the ref clock functions. */
        TEST_ASSERT_UINT32_WITHIN(پورٹ_TEST_TICKS_مقصد_REF_CLOCK(TEST_VTASKDELAYUNTIL_DELTA_TICKS),
                                  پورٹ_TEST_TICKS_مقصد_REF_CLOCK(TEST_VTASKDELAYUNTIL_TICKS),
                                  ref_clock_انتہا - ref_clock_آغاز);
    }
}

TEST_CASE("Tasks: Test vTaskDelayUntil", "[freertos]")
{
    پورٹ_TEST_REF_CLOCK_INIT();

#اگر ( CONFIG_FREERTOS_نمبر_کا_CORES > 1 )
    vTestOnAllCores(test_vTaskDelayUntil, تہی, config_TEST_DEFAULT_ڈھیر_سائز, config_TEST_UNITY_TASK_ترجیح - 1);
#ورنہ
    /* Test vTaskDelay directly on the current core */
    test_vTaskDelayUntil(تہی);
#اگر_ختم

    پورٹ_TEST_REF_CLOCK_DEINIT();
}

#اگر_ختم /* ( INCLUDE_xTaskDelayUntil == 1 ) */
#اگر_ختم //SOC_GPTIMER_SUPPORTED
