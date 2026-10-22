/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "semphr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "unity.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos_test_utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "portTestMacro.h"

/* ------------------------------------------------------------------------------------------------------------------ */
#ਜੇ SOC_GPTIMER_SUPPORTED

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

#ਜੇ ( INCLUDE_vTaskDelay == 1 )

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TEST_VTASKDELAY_TICKS           5   // Number of ticks to delay in test
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TEST_VTASKDELAY_ITERATIONS      5   // Number of iterations in test
#ਜੇ CONFIG_FREERTOS_SMP
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TEST_VTASKDELAY_DELTA_TICKS     1   // Number of ticks worth of delta allowed
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TEST_VTASKDELAY_DELTA_TICKS     2   // Number of ticks worth of delta allowed. We allow 2 ticks in IDF FreeRTOS as each core's tick interrupt could be out of phase
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਖਾਲੀ test_vTaskDelay(ਖਾਲੀ *arg)
{
    ਲਈ (ਪੂਰਨ i = 0; i < TEST_VTASKDELAY_ITERATIONS; i++) {
        TickType_t tick_ਸ਼ੁਰੂ, tick_ਸਿਰਾ;
        ਪੋਰਟ_TEST_REF_CLOCK_TYPE ref_clock_ਸ਼ੁਰੂ, ref_clock_ਸਿਰਾ;

        /* Delay until the next tick boundary */
        vTaskDelay(1);

        /* Get the current tick count and ref clock time */
        tick_ਸ਼ੁਰੂ = x_Task_Get_Tick_ਗਿਣਤੀ();
        ref_clock_ਸ਼ੁਰੂ = ਪੋਰਟ_TEST_REF_CLOCK_GET_ਸਮਾਂ();

        vTaskDelay(TEST_VTASKDELAY_TICKS);

        /* Get the post delay tick count and ref clock time */
        tick_ਸਿਰਾ = x_Task_Get_Tick_ਗਿਣਤੀ();
        ref_clock_ਸਿਰਾ = ਪੋਰਟ_TEST_REF_CLOCK_GET_ਸਮਾਂ();

        /* Check that elapsed ticks and ref clock is accurate. We allow TEST_VTASKDELAY_DELTA_TICKS of error in case
         * vTaskDelay() or portTEST_REF_CLOCK_GET_TIME() last long enough to cross a tick boundary */
#ਜੇ ( config_USE_16_ਦੋਆਧਾਰੀ_ਅੰਕ_TICKS == 1 )
        TEST_ASSERT_UINT16_WITHIN(TEST_VTASKDELAY_DELTA_TICKS, TEST_VTASKDELAY_TICKS, tick_ਸਿਰਾ - tick_ਸ਼ੁਰੂ);
#ਨਹੀਂਤਾਂ
        TEST_ASSERT_UINT32_WITHIN(TEST_VTASKDELAY_DELTA_TICKS, TEST_VTASKDELAY_TICKS, tick_ਸਿਰਾ - tick_ਸ਼ੁਰੂ);
#ਜੇ_ਅੰਤ
        TEST_ASSERT_UINT32_WITHIN(ਪੋਰਟ_TEST_TICKS_ਮੰਜ਼ਿਲ_REF_CLOCK(TEST_VTASKDELAY_DELTA_TICKS),
                                  ਪੋਰਟ_TEST_TICKS_ਮੰਜ਼ਿਲ_REF_CLOCK(TEST_VTASKDELAY_TICKS),
                                  ref_clock_ਸਿਰਾ - ref_clock_ਸ਼ੁਰੂ);
    }
}

TEST_CASE("Tasks: Test vTaskDelay", "[freertos]")
{
    ਪੋਰਟ_TEST_REF_CLOCK_INIT();

#ਜੇ ( CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES > 1 )
    vTestOnAllCores(test_vTaskDelay, ਨਿਰਾ, config_TEST_DEFAULT_ਢੇਰ_ਮਾਪ, config_TEST_UNITY_TASK_ਤਰਜੀਹ - 1);
#ਨਹੀਂਤਾਂ
    /* Test vTaskDelay directly on the current core */
    test_vTaskDelay(ਨਿਰਾ);
#ਜੇ_ਅੰਤ

    ਪੋਰਟ_TEST_REF_CLOCK_DEINIT();
}

#ਜੇ_ਅੰਤ /* ( INCLUDE_vTaskDelay == 1 ) */

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

#ਜੇ ( INCLUDE_xTaskDelayUntil == 1 )

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TEST_VTASKDELAYUNTIL_TICKS              5   // Number of ticks to delay in test
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TEST_VTASKDELAYUNTIL_ITERATIONS         5   // Number of iterations in test
#ਜੇ CONFIG_FREERTOS_SMP
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TEST_VTASKDELAYUNTIL_DELTA_TICKS        1   // Number of ticks worth of delta allowed
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TEST_VTASKDELAYUNTIL_DELTA_TICKS        2   // Number of ticks worth of delta allowed. We allow 2 ticks in IDF FreeRTOS as each core's tick interrupt could be out of phase
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਖਾਲੀ test_vTaskDelayUntil(ਖਾਲੀ *arg)
{
    ਲਈ (ਪੂਰਨ i = 0; i < TEST_VTASKDELAYUNTIL_ITERATIONS; i++) {
        TickType_t tick_ਸ਼ੁਰੂ, tick_ਸਿਰਾ, ਆਖਰੀ_wake_tick;
        ਪੋਰਟ_TEST_REF_CLOCK_TYPE ref_clock_ਸ਼ੁਰੂ, ref_clock_ਸਿਰਾ;

        /* Delay until the next tick boundary */
        vTaskDelay(1);

        /* Get the current tick count and ref clock time */
        tick_ਸ਼ੁਰੂ = x_Task_Get_Tick_ਗਿਣਤੀ();
        ਆਖਰੀ_wake_tick = tick_ਸ਼ੁਰੂ;
        ref_clock_ਸ਼ੁਰੂ = ਪੋਰਟ_TEST_REF_CLOCK_GET_ਸਮਾਂ();

        vTaskDelayUntil(&ਆਖਰੀ_wake_tick, TEST_VTASKDELAYUNTIL_TICKS);

        /* Get the post delay tick count and ref clock time */
        tick_ਸਿਰਾ = x_Task_Get_Tick_ਗਿਣਤੀ();
        ref_clock_ਸਿਰਾ = ਪੋਰਟ_TEST_REF_CLOCK_GET_ਸਮਾਂ();

        /* Check that elapsed ticks and ref clock is accurate. We allow TEST_VTASKDELAYUNTIL_DELTA_TICKS of error in
         * case vTaskDelayUntil() or portTEST_REF_CLOCK_GET_TIME() last long enough to cross a tick boundary */
#ਜੇ ( config_USE_16_ਦੋਆਧਾਰੀ_ਅੰਕ_TICKS == 1 )
        TEST_ASSERT_UINT16_WITHIN(TEST_VTASKDELAYUNTIL_DELTA_TICKS, TEST_VTASKDELAYUNTIL_TICKS, tick_ਸਿਰਾ - tick_ਸ਼ੁਰੂ);
        TEST_ASSERT_UINT16_WITHIN(TEST_VTASKDELAYUNTIL_DELTA_TICKS, tick_ਸਿਰਾ, ਆਖਰੀ_wake_tick);
#ਨਹੀਂਤਾਂ
        TEST_ASSERT_UINT32_WITHIN(TEST_VTASKDELAYUNTIL_DELTA_TICKS, TEST_VTASKDELAYUNTIL_TICKS, tick_ਸਿਰਾ - tick_ਸ਼ੁਰੂ);
        TEST_ASSERT_UINT32_WITHIN(TEST_VTASKDELAYUNTIL_DELTA_TICKS, tick_ਸਿਰਾ, ਆਖਰੀ_wake_tick);
#ਜੇ_ਅੰਤ

        /* Check that the elapsed ref clock time is accurate. We allow TEST_VTASKDELAYUNTIL_DELTA_TICKS time worth of
         * error to account for the execution time of the ref clock functions. */
        TEST_ASSERT_UINT32_WITHIN(ਪੋਰਟ_TEST_TICKS_ਮੰਜ਼ਿਲ_REF_CLOCK(TEST_VTASKDELAYUNTIL_DELTA_TICKS),
                                  ਪੋਰਟ_TEST_TICKS_ਮੰਜ਼ਿਲ_REF_CLOCK(TEST_VTASKDELAYUNTIL_TICKS),
                                  ref_clock_ਸਿਰਾ - ref_clock_ਸ਼ੁਰੂ);
    }
}

TEST_CASE("Tasks: Test vTaskDelayUntil", "[freertos]")
{
    ਪੋਰਟ_TEST_REF_CLOCK_INIT();

#ਜੇ ( CONFIG_FREERTOS_ਸੰਖਿਆ_ਦਾ_CORES > 1 )
    vTestOnAllCores(test_vTaskDelayUntil, ਨਿਰਾ, config_TEST_DEFAULT_ਢੇਰ_ਮਾਪ, config_TEST_UNITY_TASK_ਤਰਜੀਹ - 1);
#ਨਹੀਂਤਾਂ
    /* Test vTaskDelay directly on the current core */
    test_vTaskDelayUntil(ਨਿਰਾ);
#ਜੇ_ਅੰਤ

    ਪੋਰਟ_TEST_REF_CLOCK_DEINIT();
}

#ਜੇ_ਅੰਤ /* ( INCLUDE_xTaskDelayUntil == 1 ) */
#ਜੇ_ਅੰਤ //SOC_GPTIMER_SUPPORTED
