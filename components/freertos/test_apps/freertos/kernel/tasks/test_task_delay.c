/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن "sdkconfig.h"
#ضمن "FreeRTOS.h"
#ضمن "task.h"
#ضمن "semphr.h"
#ضمن "unity.h"
#ضمن "freertos_test_utils.h"
#ضمن "portTestMacro.h"

/* ------------------------------------------------------------------------------------------------------------------ */
#إذا SOC_GPTIMER_SUPPORTED

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

#إذا ( INCLUDE_vTaskDelay == 1 )

#تعریف_کن TEST_VTASKDELAY_TICKS           5   // Number of ticks to delay in test
#تعریف_کن TEST_VTASKDELAY_ITERATIONS      5   // Number of iterations in test
#إذا CONFIG_FREERTOS_SMP
#تعریف_کن TEST_VTASKDELAY_DELTA_TICKS     1   // Number of ticks worth of delta allowed
#وإلا
#تعریف_کن TEST_VTASKDELAY_DELTA_TICKS     2   // Number of ticks worth of delta allowed. We allow 2 ticks in IDF FreeRTOS as each core's tick interrupt could be out of phase
#پایان_اگر

ساكن فراغ test_vTaskDelay(فراغ *arg)
{
    لكل (صحيح i = 0; i < TEST_VTASKDELAY_ITERATIONS; i++) {
        TickType_t tick_شروع, tick_انتها;
        پورت_TEST_REF_CLOCK_TYPE ref_clock_شروع, ref_clock_انتها;

        /* Delay until the next tick boundary */
        vTaskDelay(1);

        /* Get the current tick count and ref clock time */
        tick_شروع = x_Task_Get_Tick_شمار();
        ref_clock_شروع = پورت_TEST_REF_CLOCK_GET_زمان();

        vTaskDelay(TEST_VTASKDELAY_TICKS);

        /* Get the post delay tick count and ref clock time */
        tick_انتها = x_Task_Get_Tick_شمار();
        ref_clock_انتها = پورت_TEST_REF_CLOCK_GET_زمان();

        /* Check that elapsed ticks and ref clock is accurate. We allow TEST_VTASKDELAY_DELTA_TICKS of error in case
         * vTaskDelay() or portTEST_REF_CLOCK_GET_TIME() last long enough to cross a tick boundary */
#إذا ( config_USE_16_بیت_TICKS == 1 )
        TEST_ASSERT_UINT16_WITHIN(TEST_VTASKDELAY_DELTA_TICKS, TEST_VTASKDELAY_TICKS, tick_انتها - tick_شروع);
#وإلا
        TEST_ASSERT_UINT32_WITHIN(TEST_VTASKDELAY_DELTA_TICKS, TEST_VTASKDELAY_TICKS, tick_انتها - tick_شروع);
#پایان_اگر
        TEST_ASSERT_UINT32_WITHIN(پورت_TEST_TICKS_مقصد_REF_CLOCK(TEST_VTASKDELAY_DELTA_TICKS),
                                  پورت_TEST_TICKS_مقصد_REF_CLOCK(TEST_VTASKDELAY_TICKS),
                                  ref_clock_انتها - ref_clock_شروع);
    }
}

TEST_CASE("Tasks: Test vTaskDelay", "[freertos]")
{
    پورت_TEST_REF_CLOCK_INIT();

#إذا ( CONFIG_FREERTOS_عدد_از_CORES > 1 )
    vTestOnAllCores(test_vTaskDelay, عدم, config_TEST_DEFAULT_پشته_اندازه, config_TEST_UNITY_TASK_اولویت - 1);
#وإلا
    /* Test vTaskDelay directly on the current core */
    test_vTaskDelay(عدم);
#پایان_اگر

    پورت_TEST_REF_CLOCK_DEINIT();
}

#پایان_اگر /* ( INCLUDE_vTaskDelay == 1 ) */

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

#إذا ( INCLUDE_xTaskDelayUntil == 1 )

#تعریف_کن TEST_VTASKDELAYUNTIL_TICKS              5   // Number of ticks to delay in test
#تعریف_کن TEST_VTASKDELAYUNTIL_ITERATIONS         5   // Number of iterations in test
#إذا CONFIG_FREERTOS_SMP
#تعریف_کن TEST_VTASKDELAYUNTIL_DELTA_TICKS        1   // Number of ticks worth of delta allowed
#وإلا
#تعریف_کن TEST_VTASKDELAYUNTIL_DELTA_TICKS        2   // Number of ticks worth of delta allowed. We allow 2 ticks in IDF FreeRTOS as each core's tick interrupt could be out of phase
#پایان_اگر

ساكن فراغ test_vTaskDelayUntil(فراغ *arg)
{
    لكل (صحيح i = 0; i < TEST_VTASKDELAYUNTIL_ITERATIONS; i++) {
        TickType_t tick_شروع, tick_انتها, آخرین_wake_tick;
        پورت_TEST_REF_CLOCK_TYPE ref_clock_شروع, ref_clock_انتها;

        /* Delay until the next tick boundary */
        vTaskDelay(1);

        /* Get the current tick count and ref clock time */
        tick_شروع = x_Task_Get_Tick_شمار();
        آخرین_wake_tick = tick_شروع;
        ref_clock_شروع = پورت_TEST_REF_CLOCK_GET_زمان();

        vTaskDelayUntil(&آخرین_wake_tick, TEST_VTASKDELAYUNTIL_TICKS);

        /* Get the post delay tick count and ref clock time */
        tick_انتها = x_Task_Get_Tick_شمار();
        ref_clock_انتها = پورت_TEST_REF_CLOCK_GET_زمان();

        /* Check that elapsed ticks and ref clock is accurate. We allow TEST_VTASKDELAYUNTIL_DELTA_TICKS of error in
         * case vTaskDelayUntil() or portTEST_REF_CLOCK_GET_TIME() last long enough to cross a tick boundary */
#إذا ( config_USE_16_بیت_TICKS == 1 )
        TEST_ASSERT_UINT16_WITHIN(TEST_VTASKDELAYUNTIL_DELTA_TICKS, TEST_VTASKDELAYUNTIL_TICKS, tick_انتها - tick_شروع);
        TEST_ASSERT_UINT16_WITHIN(TEST_VTASKDELAYUNTIL_DELTA_TICKS, tick_انتها, آخرین_wake_tick);
#وإلا
        TEST_ASSERT_UINT32_WITHIN(TEST_VTASKDELAYUNTIL_DELTA_TICKS, TEST_VTASKDELAYUNTIL_TICKS, tick_انتها - tick_شروع);
        TEST_ASSERT_UINT32_WITHIN(TEST_VTASKDELAYUNTIL_DELTA_TICKS, tick_انتها, آخرین_wake_tick);
#پایان_اگر

        /* Check that the elapsed ref clock time is accurate. We allow TEST_VTASKDELAYUNTIL_DELTA_TICKS time worth of
         * error to account for the execution time of the ref clock functions. */
        TEST_ASSERT_UINT32_WITHIN(پورت_TEST_TICKS_مقصد_REF_CLOCK(TEST_VTASKDELAYUNTIL_DELTA_TICKS),
                                  پورت_TEST_TICKS_مقصد_REF_CLOCK(TEST_VTASKDELAYUNTIL_TICKS),
                                  ref_clock_انتها - ref_clock_شروع);
    }
}

TEST_CASE("Tasks: Test vTaskDelayUntil", "[freertos]")
{
    پورت_TEST_REF_CLOCK_INIT();

#إذا ( CONFIG_FREERTOS_عدد_از_CORES > 1 )
    vTestOnAllCores(test_vTaskDelayUntil, عدم, config_TEST_DEFAULT_پشته_اندازه, config_TEST_UNITY_TASK_اولویت - 1);
#وإلا
    /* Test vTaskDelay directly on the current core */
    test_vTaskDelayUntil(عدم);
#پایان_اگر

    پورت_TEST_REF_CLOCK_DEINIT();
}

#پایان_اگر /* ( INCLUDE_xTaskDelayUntil == 1 ) */
#پایان_اگر //SOC_GPTIMER_SUPPORTED
