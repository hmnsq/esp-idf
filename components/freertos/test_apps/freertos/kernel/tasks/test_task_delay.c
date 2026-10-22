/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ଅନ୍ତର୍ଭାଵ "sdkconfig.h"
#ଅନ୍ତର୍ଭାଵ "FreeRTOS.h"
#ଅନ୍ତର୍ଭାଵ "task.h"
#ଅନ୍ତର୍ଭାଵ "semphr.h"
#ଅନ୍ତର୍ଭାଵ "unity.h"
#ଅନ୍ତର୍ଭାଵ "freertos_test_utils.h"
#ଅନ୍ତର୍ଭାଵ "portTestMacro.h"

/* ------------------------------------------------------------------------------------------------------------------ */
#ଯଦି SOC_GPTIMER_SUPPORTED

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

#ଯଦି ( INCLUDE_vTaskDelay == 1 )

#define TEST_VTASKDELAY_TICKS           5   // Number of ticks to delay in test
#define TEST_VTASKDELAY_ITERATIONS      5   // Number of iterations in test
#ଯଦି CONFIG_FREERTOS_SMP
#define TEST_VTASKDELAY_DELTA_TICKS     1   // Number of ticks worth of delta allowed
#ଅନ୍ୟଥା
#define TEST_VTASKDELAY_DELTA_TICKS     2   // Number of ticks worth of delta allowed. We allow 2 ticks in IDF FreeRTOS as each core's tick interrupt could be out of phase
#endif

ସ୍ଥିର ଶୂନ୍ୟ test_vTaskDelay(ଶୂନ୍ୟ *arg)
{
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < TEST_VTASKDELAY_ITERATIONS; i++) {
        TickType_t tick_start, tick_end;
        portTEST_REF_CLOCK_TYPE ref_clock_start, ref_clock_end;

        /* Delay until the next tick boundary */
        vTaskDelay(1);

        /* Get the current tick count and ref clock time */
        tick_start = x_Task_Get_Tick_ଗଣନା();
        ref_clock_start = portTEST_REF_CLOCK_GET_TIME();

        vTaskDelay(TEST_VTASKDELAY_TICKS);

        /* Get the post delay tick count and ref clock time */
        tick_end = x_Task_Get_Tick_ଗଣନା();
        ref_clock_end = portTEST_REF_CLOCK_GET_TIME();

        /* Check that elapsed ticks and ref clock is accurate. We allow TEST_VTASKDELAY_DELTA_TICKS of error in case
         * vTaskDelay() or portTEST_REF_CLOCK_GET_TIME() last long enough to cross a tick boundary */
#ଯଦି ( configUSE_16_BIT_TICKS == 1 )
        TEST_ASSERT_UINT16_WITHIN(TEST_VTASKDELAY_DELTA_TICKS, TEST_VTASKDELAY_TICKS, tick_end - tick_start);
#ଅନ୍ୟଥା
        TEST_ASSERT_UINT32_WITHIN(TEST_VTASKDELAY_DELTA_TICKS, TEST_VTASKDELAY_TICKS, tick_end - tick_start);
#endif
        TEST_ASSERT_UINT32_WITHIN(portTEST_TICKS_TO_REF_CLOCK(TEST_VTASKDELAY_DELTA_TICKS),
                                  portTEST_TICKS_TO_REF_CLOCK(TEST_VTASKDELAY_TICKS),
                                  ref_clock_end - ref_clock_start);
    }
}

TEST_CASE("Tasks: Test vTaskDelay", "[freertos]")
{
    portTEST_REF_CLOCK_INIT();

#ଯଦି ( CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES > 1 )
    vTestOnAllCores(test_vTaskDelay, ଖାଲି, config_TEST_DEFAULT_ଗଦା_SIZE, configTEST_UNITY_TASK_PRIORITY - 1);
#ଅନ୍ୟଥା
    /* Test vTaskDelay directly on the current core */
    test_vTaskDelay(ଖାଲି);
#endif

    portTEST_REF_CLOCK_DEINIT();
}

#endif /* ( INCLUDE_vTaskDelay == 1 ) */

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

#ଯଦି ( INCLUDE_xTaskDelayUntil == 1 )

#define TEST_VTASKDELAYUNTIL_TICKS              5   // Number of ticks to delay in test
#define TEST_VTASKDELAYUNTIL_ITERATIONS         5   // Number of iterations in test
#ଯଦି CONFIG_FREERTOS_SMP
#define TEST_VTASKDELAYUNTIL_DELTA_TICKS        1   // Number of ticks worth of delta allowed
#ଅନ୍ୟଥା
#define TEST_VTASKDELAYUNTIL_DELTA_TICKS        2   // Number of ticks worth of delta allowed. We allow 2 ticks in IDF FreeRTOS as each core's tick interrupt could be out of phase
#endif

ସ୍ଥିର ଶୂନ୍ୟ test_vTaskDelayUntil(ଶୂନ୍ୟ *arg)
{
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < TEST_VTASKDELAYUNTIL_ITERATIONS; i++) {
        TickType_t tick_start, tick_end, last_wake_tick;
        portTEST_REF_CLOCK_TYPE ref_clock_start, ref_clock_end;

        /* Delay until the next tick boundary */
        vTaskDelay(1);

        /* Get the current tick count and ref clock time */
        tick_start = x_Task_Get_Tick_ଗଣନା();
        last_wake_tick = tick_start;
        ref_clock_start = portTEST_REF_CLOCK_GET_TIME();

        vTaskDelayUntil(&last_wake_tick, TEST_VTASKDELAYUNTIL_TICKS);

        /* Get the post delay tick count and ref clock time */
        tick_end = x_Task_Get_Tick_ଗଣନା();
        ref_clock_end = portTEST_REF_CLOCK_GET_TIME();

        /* Check that elapsed ticks and ref clock is accurate. We allow TEST_VTASKDELAYUNTIL_DELTA_TICKS of error in
         * case vTaskDelayUntil() or portTEST_REF_CLOCK_GET_TIME() last long enough to cross a tick boundary */
#ଯଦି ( configUSE_16_BIT_TICKS == 1 )
        TEST_ASSERT_UINT16_WITHIN(TEST_VTASKDELAYUNTIL_DELTA_TICKS, TEST_VTASKDELAYUNTIL_TICKS, tick_end - tick_start);
        TEST_ASSERT_UINT16_WITHIN(TEST_VTASKDELAYUNTIL_DELTA_TICKS, tick_end, last_wake_tick);
#ଅନ୍ୟଥା
        TEST_ASSERT_UINT32_WITHIN(TEST_VTASKDELAYUNTIL_DELTA_TICKS, TEST_VTASKDELAYUNTIL_TICKS, tick_end - tick_start);
        TEST_ASSERT_UINT32_WITHIN(TEST_VTASKDELAYUNTIL_DELTA_TICKS, tick_end, last_wake_tick);
#endif

        /* Check that the elapsed ref clock time is accurate. We allow TEST_VTASKDELAYUNTIL_DELTA_TICKS time worth of
         * error to account for the execution time of the ref clock functions. */
        TEST_ASSERT_UINT32_WITHIN(portTEST_TICKS_TO_REF_CLOCK(TEST_VTASKDELAYUNTIL_DELTA_TICKS),
                                  portTEST_TICKS_TO_REF_CLOCK(TEST_VTASKDELAYUNTIL_TICKS),
                                  ref_clock_end - ref_clock_start);
    }
}

TEST_CASE("Tasks: Test vTaskDelayUntil", "[freertos]")
{
    portTEST_REF_CLOCK_INIT();

#ଯଦି ( CONFIG_FREERTOS_ସଂଖ୍ୟା_OF_CORES > 1 )
    vTestOnAllCores(test_vTaskDelayUntil, ଖାଲି, config_TEST_DEFAULT_ଗଦା_SIZE, configTEST_UNITY_TASK_PRIORITY - 1);
#ଅନ୍ୟଥା
    /* Test vTaskDelay directly on the current core */
    test_vTaskDelayUntil(ଖାଲି);
#endif

    portTEST_REF_CLOCK_DEINIT();
}

#endif /* ( INCLUDE_xTaskDelayUntil == 1 ) */
#endif //SOC_GPTIMER_SUPPORTED
