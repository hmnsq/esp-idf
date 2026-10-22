/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ "FreeRTOS.h"
#అన్తర్భావ "task.h"
#అన్తర్భావ "semphr.h"
#అన్తర్భావ "unity.h"
#అన్తర్భావ "freertos_test_utils.h"
#అన్తర్భావ "portTestMacro.h"

/* ------------------------------------------------------------------------------------------------------------------ */
#ఒకవేళ SOC_GPTIMER_SUPPORTED

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

#ఒకవేళ ( INCLUDE_vTaskDelay == 1 )

#నిర్వచించు TEST_VTASKDELAY_TICKS           5   // Number of ticks to delay in test
#నిర్వచించు TEST_VTASKDELAY_ITERATIONS      5   // Number of iterations in test
#ఒకవేళ CONFIG_FREERTOS_SMP
#నిర్వచించు TEST_VTASKDELAY_DELTA_TICKS     1   // Number of ticks worth of delta allowed
#లేకపోతే
#నిర్వచించు TEST_VTASKDELAY_DELTA_TICKS     2   // Number of ticks worth of delta allowed. We allow 2 ticks in IDF FreeRTOS as each core's tick interrupt could be out of phase
#అయితే_ముగింపు

స్థిర శూన్య test_vTaskDelay(శూన్య *arg)
{
    కొరకు (పూర్ణాంకం i = 0; i < TEST_VTASKDELAY_ITERATIONS; i++) {
        TickType_t tick_ప్రారంభం, tick_చివర;
        పోర్టు_TEST_REF_CLOCK_TYPE ref_clock_ప్రారంభం, ref_clock_చివర;

        /* Delay until the next tick boundary */
        vTaskDelay(1);

        /* Get the current tick count and ref clock time */
        tick_ప్రారంభం = x_Task_Get_Tick_లెక్క();
        ref_clock_ప్రారంభం = పోర్టు_TEST_REF_CLOCK_GET_సమయం();

        vTaskDelay(TEST_VTASKDELAY_TICKS);

        /* Get the post delay tick count and ref clock time */
        tick_చివర = x_Task_Get_Tick_లెక్క();
        ref_clock_చివర = పోర్టు_TEST_REF_CLOCK_GET_సమయం();

        /* Check that elapsed ticks and ref clock is accurate. We allow TEST_VTASKDELAY_DELTA_TICKS of error in case
         * vTaskDelay() or portTEST_REF_CLOCK_GET_TIME() last long enough to cross a tick boundary */
#ఒకవేళ ( config_USE_16_బిట్టు_TICKS == 1 )
        TEST_ASSERT_UINT16_WITHIN(TEST_VTASKDELAY_DELTA_TICKS, TEST_VTASKDELAY_TICKS, tick_చివర - tick_ప్రారంభం);
#లేకపోతే
        TEST_ASSERT_UINT32_WITHIN(TEST_VTASKDELAY_DELTA_TICKS, TEST_VTASKDELAY_TICKS, tick_చివర - tick_ప్రారంభం);
#అయితే_ముగింపు
        TEST_ASSERT_UINT32_WITHIN(పోర్టు_TEST_TICKS_గమ్యం_REF_CLOCK(TEST_VTASKDELAY_DELTA_TICKS),
                                  పోర్టు_TEST_TICKS_గమ్యం_REF_CLOCK(TEST_VTASKDELAY_TICKS),
                                  ref_clock_చివర - ref_clock_ప్రారంభం);
    }
}

TEST_CASE("Tasks: Test vTaskDelay", "[freertos]")
{
    పోర్టు_TEST_REF_CLOCK_INIT();

#ఒకవేళ ( CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES > 1 )
    vTestOnAllCores(test_vTaskDelay, ఖాళీ, config_TEST_DEFAULT_దొంతర_కొలత, config_TEST_UNITY_TASK_ప్రాధాన్యత - 1);
#లేకపోతే
    /* Test vTaskDelay directly on the current core */
    test_vTaskDelay(ఖాళీ);
#అయితే_ముగింపు

    పోర్టు_TEST_REF_CLOCK_DEINIT();
}

#అయితే_ముగింపు /* ( INCLUDE_vTaskDelay == 1 ) */

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

#ఒకవేళ ( INCLUDE_xTaskDelayUntil == 1 )

#నిర్వచించు TEST_VTASKDELAYUNTIL_TICKS              5   // Number of ticks to delay in test
#నిర్వచించు TEST_VTASKDELAYUNTIL_ITERATIONS         5   // Number of iterations in test
#ఒకవేళ CONFIG_FREERTOS_SMP
#నిర్వచించు TEST_VTASKDELAYUNTIL_DELTA_TICKS        1   // Number of ticks worth of delta allowed
#లేకపోతే
#నిర్వచించు TEST_VTASKDELAYUNTIL_DELTA_TICKS        2   // Number of ticks worth of delta allowed. We allow 2 ticks in IDF FreeRTOS as each core's tick interrupt could be out of phase
#అయితే_ముగింపు

స్థిర శూన్య test_vTaskDelayUntil(శూన్య *arg)
{
    కొరకు (పూర్ణాంకం i = 0; i < TEST_VTASKDELAYUNTIL_ITERATIONS; i++) {
        TickType_t tick_ప్రారంభం, tick_చివర, చివరి_wake_tick;
        పోర్టు_TEST_REF_CLOCK_TYPE ref_clock_ప్రారంభం, ref_clock_చివర;

        /* Delay until the next tick boundary */
        vTaskDelay(1);

        /* Get the current tick count and ref clock time */
        tick_ప్రారంభం = x_Task_Get_Tick_లెక్క();
        చివరి_wake_tick = tick_ప్రారంభం;
        ref_clock_ప్రారంభం = పోర్టు_TEST_REF_CLOCK_GET_సమయం();

        vTaskDelayUntil(&చివరి_wake_tick, TEST_VTASKDELAYUNTIL_TICKS);

        /* Get the post delay tick count and ref clock time */
        tick_చివర = x_Task_Get_Tick_లెక్క();
        ref_clock_చివర = పోర్టు_TEST_REF_CLOCK_GET_సమయం();

        /* Check that elapsed ticks and ref clock is accurate. We allow TEST_VTASKDELAYUNTIL_DELTA_TICKS of error in
         * case vTaskDelayUntil() or portTEST_REF_CLOCK_GET_TIME() last long enough to cross a tick boundary */
#ఒకవేళ ( config_USE_16_బిట్టు_TICKS == 1 )
        TEST_ASSERT_UINT16_WITHIN(TEST_VTASKDELAYUNTIL_DELTA_TICKS, TEST_VTASKDELAYUNTIL_TICKS, tick_చివర - tick_ప్రారంభం);
        TEST_ASSERT_UINT16_WITHIN(TEST_VTASKDELAYUNTIL_DELTA_TICKS, tick_చివర, చివరి_wake_tick);
#లేకపోతే
        TEST_ASSERT_UINT32_WITHIN(TEST_VTASKDELAYUNTIL_DELTA_TICKS, TEST_VTASKDELAYUNTIL_TICKS, tick_చివర - tick_ప్రారంభం);
        TEST_ASSERT_UINT32_WITHIN(TEST_VTASKDELAYUNTIL_DELTA_TICKS, tick_చివర, చివరి_wake_tick);
#అయితే_ముగింపు

        /* Check that the elapsed ref clock time is accurate. We allow TEST_VTASKDELAYUNTIL_DELTA_TICKS time worth of
         * error to account for the execution time of the ref clock functions. */
        TEST_ASSERT_UINT32_WITHIN(పోర్టు_TEST_TICKS_గమ్యం_REF_CLOCK(TEST_VTASKDELAYUNTIL_DELTA_TICKS),
                                  పోర్టు_TEST_TICKS_గమ్యం_REF_CLOCK(TEST_VTASKDELAYUNTIL_TICKS),
                                  ref_clock_చివర - ref_clock_ప్రారంభం);
    }
}

TEST_CASE("Tasks: Test vTaskDelayUntil", "[freertos]")
{
    పోర్టు_TEST_REF_CLOCK_INIT();

#ఒకవేళ ( CONFIG_FREERTOS_సంఖ్య_యొక్క_CORES > 1 )
    vTestOnAllCores(test_vTaskDelayUntil, ఖాళీ, config_TEST_DEFAULT_దొంతర_కొలత, config_TEST_UNITY_TASK_ప్రాధాన్యత - 1);
#లేకపోతే
    /* Test vTaskDelay directly on the current core */
    test_vTaskDelayUntil(ఖాళీ);
#అయితే_ముగింపు

    పోర్టు_TEST_REF_CLOCK_DEINIT();
}

#అయితే_ముగింపు /* ( INCLUDE_xTaskDelayUntil == 1 ) */
#అయితే_ముగింపు //SOC_GPTIMER_SUPPORTED
