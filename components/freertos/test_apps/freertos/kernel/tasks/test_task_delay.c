/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ "FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "task.h"
#ಅನ್ತರ್ಭಾವ "semphr.h"
#ಅನ್ತರ್ಭಾವ "unity.h"
#ಅನ್ತರ್ಭಾವ "freertos_test_utils.h"
#ಅನ್ತರ್ಭಾವ "portTestMacro.h"

/* ------------------------------------------------------------------------------------------------------------------ */
#ಆದರೆ SOC_GPTIMER_SUPPORTED

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

#ಆದರೆ ( INCLUDE_vTaskDelay == 1 )

#ವ್ಯಾಖ್ಯಾನಿಸು TEST_VTASKDELAY_TICKS           5   // Number of ticks to delay in test
#ವ್ಯಾಖ್ಯಾನಿಸು TEST_VTASKDELAY_ITERATIONS      5   // Number of iterations in test
#ಆದರೆ CONFIG_FREERTOS_SMP
#ವ್ಯಾಖ್ಯಾನಿಸು TEST_VTASKDELAY_DELTA_TICKS     1   // Number of ticks worth of delta allowed
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು TEST_VTASKDELAY_DELTA_TICKS     2   // Number of ticks worth of delta allowed. We allow 2 ticks in IDF FreeRTOS as each core's tick interrupt could be out of phase
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಶೂನ್ಯ test_vTaskDelay(ಶೂನ್ಯ *arg)
{
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < TEST_VTASKDELAY_ITERATIONS; i++) {
        TickType_t tick_ಆರಂಭ, tick_ಕೊನೆ;
        ಪೋರ್ಟ್_TEST_REF_CLOCK_TYPE ref_clock_ಆರಂಭ, ref_clock_ಕೊನೆ;

        /* Delay until the next tick boundary */
        vTaskDelay(1);

        /* Get the current tick count and ref clock time */
        tick_ಆರಂಭ = x_Task_Get_Tick_ಎಣಿಕೆ();
        ref_clock_ಆರಂಭ = ಪೋರ್ಟ್_TEST_REF_CLOCK_GET_ಸಮಯ();

        vTaskDelay(TEST_VTASKDELAY_TICKS);

        /* Get the post delay tick count and ref clock time */
        tick_ಕೊನೆ = x_Task_Get_Tick_ಎಣಿಕೆ();
        ref_clock_ಕೊನೆ = ಪೋರ್ಟ್_TEST_REF_CLOCK_GET_ಸಮಯ();

        /* Check that elapsed ticks and ref clock is accurate. We allow TEST_VTASKDELAY_DELTA_TICKS of error in case
         * vTaskDelay() or portTEST_REF_CLOCK_GET_TIME() last long enough to cross a tick boundary */
#ಆದರೆ ( config_USE_16_ಬಿಟ್_TICKS == 1 )
        TEST_ASSERT_UINT16_WITHIN(TEST_VTASKDELAY_DELTA_TICKS, TEST_VTASKDELAY_TICKS, tick_ಕೊನೆ - tick_ಆರಂಭ);
#ಇಲ್ಲದಿದ್ದರೆ
        TEST_ASSERT_UINT32_WITHIN(TEST_VTASKDELAY_DELTA_TICKS, TEST_VTASKDELAY_TICKS, tick_ಕೊನೆ - tick_ಆರಂಭ);
#ಆದರೆ_ಅಂತ್ಯ
        TEST_ASSERT_UINT32_WITHIN(ಪೋರ್ಟ್_TEST_TICKS_ಗಮ್ಯ_REF_CLOCK(TEST_VTASKDELAY_DELTA_TICKS),
                                  ಪೋರ್ಟ್_TEST_TICKS_ಗಮ್ಯ_REF_CLOCK(TEST_VTASKDELAY_TICKS),
                                  ref_clock_ಕೊನೆ - ref_clock_ಆರಂಭ);
    }
}

TEST_CASE("Tasks: Test vTaskDelay", "[freertos]")
{
    ಪೋರ್ಟ್_TEST_REF_CLOCK_INIT();

#ಆದರೆ ( CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES > 1 )
    vTestOnAllCores(test_vTaskDelay, ಖಾಲಿ, config_TEST_DEFAULT_ರಾಶಿ_ಗಾತ್ರ, config_TEST_UNITY_TASK_ಆದ್ಯತೆ - 1);
#ಇಲ್ಲದಿದ್ದರೆ
    /* Test vTaskDelay directly on the current core */
    test_vTaskDelay(ಖಾಲಿ);
#ಆದರೆ_ಅಂತ್ಯ

    ಪೋರ್ಟ್_TEST_REF_CLOCK_DEINIT();
}

#ಆದರೆ_ಅಂತ್ಯ /* ( INCLUDE_vTaskDelay == 1 ) */

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

#ಆದರೆ ( INCLUDE_xTaskDelayUntil == 1 )

#ವ್ಯಾಖ್ಯಾನಿಸು TEST_VTASKDELAYUNTIL_TICKS              5   // Number of ticks to delay in test
#ವ್ಯಾಖ್ಯಾನಿಸು TEST_VTASKDELAYUNTIL_ITERATIONS         5   // Number of iterations in test
#ಆದರೆ CONFIG_FREERTOS_SMP
#ವ್ಯಾಖ್ಯಾನಿಸು TEST_VTASKDELAYUNTIL_DELTA_TICKS        1   // Number of ticks worth of delta allowed
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು TEST_VTASKDELAYUNTIL_DELTA_TICKS        2   // Number of ticks worth of delta allowed. We allow 2 ticks in IDF FreeRTOS as each core's tick interrupt could be out of phase
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಶೂನ್ಯ test_vTaskDelayUntil(ಶೂನ್ಯ *arg)
{
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < TEST_VTASKDELAYUNTIL_ITERATIONS; i++) {
        TickType_t tick_ಆರಂಭ, tick_ಕೊನೆ, ಕೊನೆಯ_wake_tick;
        ಪೋರ್ಟ್_TEST_REF_CLOCK_TYPE ref_clock_ಆರಂಭ, ref_clock_ಕೊನೆ;

        /* Delay until the next tick boundary */
        vTaskDelay(1);

        /* Get the current tick count and ref clock time */
        tick_ಆರಂಭ = x_Task_Get_Tick_ಎಣಿಕೆ();
        ಕೊನೆಯ_wake_tick = tick_ಆರಂಭ;
        ref_clock_ಆರಂಭ = ಪೋರ್ಟ್_TEST_REF_CLOCK_GET_ಸಮಯ();

        vTaskDelayUntil(&ಕೊನೆಯ_wake_tick, TEST_VTASKDELAYUNTIL_TICKS);

        /* Get the post delay tick count and ref clock time */
        tick_ಕೊನೆ = x_Task_Get_Tick_ಎಣಿಕೆ();
        ref_clock_ಕೊನೆ = ಪೋರ್ಟ್_TEST_REF_CLOCK_GET_ಸಮಯ();

        /* Check that elapsed ticks and ref clock is accurate. We allow TEST_VTASKDELAYUNTIL_DELTA_TICKS of error in
         * case vTaskDelayUntil() or portTEST_REF_CLOCK_GET_TIME() last long enough to cross a tick boundary */
#ಆದರೆ ( config_USE_16_ಬಿಟ್_TICKS == 1 )
        TEST_ASSERT_UINT16_WITHIN(TEST_VTASKDELAYUNTIL_DELTA_TICKS, TEST_VTASKDELAYUNTIL_TICKS, tick_ಕೊನೆ - tick_ಆರಂಭ);
        TEST_ASSERT_UINT16_WITHIN(TEST_VTASKDELAYUNTIL_DELTA_TICKS, tick_ಕೊನೆ, ಕೊನೆಯ_wake_tick);
#ಇಲ್ಲದಿದ್ದರೆ
        TEST_ASSERT_UINT32_WITHIN(TEST_VTASKDELAYUNTIL_DELTA_TICKS, TEST_VTASKDELAYUNTIL_TICKS, tick_ಕೊನೆ - tick_ಆರಂಭ);
        TEST_ASSERT_UINT32_WITHIN(TEST_VTASKDELAYUNTIL_DELTA_TICKS, tick_ಕೊನೆ, ಕೊನೆಯ_wake_tick);
#ಆದರೆ_ಅಂತ್ಯ

        /* Check that the elapsed ref clock time is accurate. We allow TEST_VTASKDELAYUNTIL_DELTA_TICKS time worth of
         * error to account for the execution time of the ref clock functions. */
        TEST_ASSERT_UINT32_WITHIN(ಪೋರ್ಟ್_TEST_TICKS_ಗಮ್ಯ_REF_CLOCK(TEST_VTASKDELAYUNTIL_DELTA_TICKS),
                                  ಪೋರ್ಟ್_TEST_TICKS_ಗಮ್ಯ_REF_CLOCK(TEST_VTASKDELAYUNTIL_TICKS),
                                  ref_clock_ಕೊನೆ - ref_clock_ಆರಂಭ);
    }
}

TEST_CASE("Tasks: Test vTaskDelayUntil", "[freertos]")
{
    ಪೋರ್ಟ್_TEST_REF_CLOCK_INIT();

#ಆದರೆ ( CONFIG_FREERTOS_ಸಂಖ್ಯೆ_ದ_CORES > 1 )
    vTestOnAllCores(test_vTaskDelayUntil, ಖಾಲಿ, config_TEST_DEFAULT_ರಾಶಿ_ಗಾತ್ರ, config_TEST_UNITY_TASK_ಆದ್ಯತೆ - 1);
#ಇಲ್ಲದಿದ್ದರೆ
    /* Test vTaskDelay directly on the current core */
    test_vTaskDelayUntil(ಖಾಲಿ);
#ಆದರೆ_ಅಂತ್ಯ

    ಪೋರ್ಟ್_TEST_REF_CLOCK_DEINIT();
}

#ಆದರೆ_ಅಂತ್ಯ /* ( INCLUDE_xTaskDelayUntil == 1 ) */
#ಆದರೆ_ಅಂತ್ಯ //SOC_GPTIMER_SUPPORTED
