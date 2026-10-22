/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#समावेश "sdkconfig.h"
#समावेश "FreeRTOS.h"
#समावेश "task.h"
#समावेश "semphr.h"
#समावेश "unity.h"
#समावेश "freertos_test_utils.h"
#समावेश "portTestMacro.h"

/* ------------------------------------------------------------------------------------------------------------------ */
#अगर SOC_GPTIMER_SUPPORTED

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

#अगर ( INCLUDE_vTaskDelay == 1 )

#घोषणा TEST_VTASKDELAY_TICKS           5   // Number of ticks to delay in test
#घोषणा TEST_VTASKDELAY_ITERATIONS      5   // Number of iterations in test
#अगर CONFIG_FREERTOS_SMP
#घोषणा TEST_VTASKDELAY_DELTA_TICKS     1   // Number of ticks worth of delta allowed
#अन्यथा
#घोषणा TEST_VTASKDELAY_DELTA_TICKS     2   // Number of ticks worth of delta allowed. We allow 2 ticks in IDF FreeRTOS as each core's tick interrupt could be out of phase
#पूर्ण_अगर

अटल व्योम test_vTaskDelay(व्योम *arg)
{
    क्रम (पूर्णांक i = 0; i < TEST_VTASKDELAY_ITERATIONS; i++) {
        TickType_t tick_start, tick_end;
        portTEST_REF_CLOCK_TYPE ref_clock_start, ref_clock_end;

        /* Delay until the next tick boundary */
        vTaskDelay(1);

        /* Get the current tick count and ref clock time */
        tick_start = x_Task_Get_Tick_गिनती();
        ref_clock_start = portTEST_REF_CLOCK_GET_TIME();

        vTaskDelay(TEST_VTASKDELAY_TICKS);

        /* Get the post delay tick count and ref clock time */
        tick_end = x_Task_Get_Tick_गिनती();
        ref_clock_end = portTEST_REF_CLOCK_GET_TIME();

        /* Check that elapsed ticks and ref clock is accurate. We allow TEST_VTASKDELAY_DELTA_TICKS of error in case
         * vTaskDelay() or portTEST_REF_CLOCK_GET_TIME() last long enough to cross a tick boundary */
#अगर ( configUSE_16_BIT_TICKS == 1 )
        TEST_ASSERT_UINT16_WITHIN(TEST_VTASKDELAY_DELTA_TICKS, TEST_VTASKDELAY_TICKS, tick_end - tick_start);
#अन्यथा
        TEST_ASSERT_UINT32_WITHIN(TEST_VTASKDELAY_DELTA_TICKS, TEST_VTASKDELAY_TICKS, tick_end - tick_start);
#पूर्ण_अगर
        TEST_ASSERT_UINT32_WITHIN(portTEST_TICKS_TO_REF_CLOCK(TEST_VTASKDELAY_DELTA_TICKS),
                                  portTEST_TICKS_TO_REF_CLOCK(TEST_VTASKDELAY_TICKS),
                                  ref_clock_end - ref_clock_start);
    }
}

TEST_CASE("Tasks: Test vTaskDelay", "[freertos]")
{
    portTEST_REF_CLOCK_INIT();

#अगर ( CONFIG_FREERTOS_संख्या_OF_CORES > 1 )
    vTestOnAllCores(test_vTaskDelay, शून्य, config_TEST_DEFAULT_ढेर_आकार, configTEST_UNITY_TASK_PRIORITY - 1);
#अन्यथा
    /* Test vTaskDelay directly on the current core */
    test_vTaskDelay(शून्य);
#पूर्ण_अगर

    portTEST_REF_CLOCK_DEINIT();
}

#पूर्ण_अगर /* ( INCLUDE_vTaskDelay == 1 ) */

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

#अगर ( INCLUDE_xTaskDelayUntil == 1 )

#घोषणा TEST_VTASKDELAYUNTIL_TICKS              5   // Number of ticks to delay in test
#घोषणा TEST_VTASKDELAYUNTIL_ITERATIONS         5   // Number of iterations in test
#अगर CONFIG_FREERTOS_SMP
#घोषणा TEST_VTASKDELAYUNTIL_DELTA_TICKS        1   // Number of ticks worth of delta allowed
#अन्यथा
#घोषणा TEST_VTASKDELAYUNTIL_DELTA_TICKS        2   // Number of ticks worth of delta allowed. We allow 2 ticks in IDF FreeRTOS as each core's tick interrupt could be out of phase
#पूर्ण_अगर

अटल व्योम test_vTaskDelayUntil(व्योम *arg)
{
    क्रम (पूर्णांक i = 0; i < TEST_VTASKDELAYUNTIL_ITERATIONS; i++) {
        TickType_t tick_start, tick_end, last_wake_tick;
        portTEST_REF_CLOCK_TYPE ref_clock_start, ref_clock_end;

        /* Delay until the next tick boundary */
        vTaskDelay(1);

        /* Get the current tick count and ref clock time */
        tick_start = x_Task_Get_Tick_गिनती();
        last_wake_tick = tick_start;
        ref_clock_start = portTEST_REF_CLOCK_GET_TIME();

        vTaskDelayUntil(&last_wake_tick, TEST_VTASKDELAYUNTIL_TICKS);

        /* Get the post delay tick count and ref clock time */
        tick_end = x_Task_Get_Tick_गिनती();
        ref_clock_end = portTEST_REF_CLOCK_GET_TIME();

        /* Check that elapsed ticks and ref clock is accurate. We allow TEST_VTASKDELAYUNTIL_DELTA_TICKS of error in
         * case vTaskDelayUntil() or portTEST_REF_CLOCK_GET_TIME() last long enough to cross a tick boundary */
#अगर ( configUSE_16_BIT_TICKS == 1 )
        TEST_ASSERT_UINT16_WITHIN(TEST_VTASKDELAYUNTIL_DELTA_TICKS, TEST_VTASKDELAYUNTIL_TICKS, tick_end - tick_start);
        TEST_ASSERT_UINT16_WITHIN(TEST_VTASKDELAYUNTIL_DELTA_TICKS, tick_end, last_wake_tick);
#अन्यथा
        TEST_ASSERT_UINT32_WITHIN(TEST_VTASKDELAYUNTIL_DELTA_TICKS, TEST_VTASKDELAYUNTIL_TICKS, tick_end - tick_start);
        TEST_ASSERT_UINT32_WITHIN(TEST_VTASKDELAYUNTIL_DELTA_TICKS, tick_end, last_wake_tick);
#पूर्ण_अगर

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

#अगर ( CONFIG_FREERTOS_संख्या_OF_CORES > 1 )
    vTestOnAllCores(test_vTaskDelayUntil, शून्य, config_TEST_DEFAULT_ढेर_आकार, configTEST_UNITY_TASK_PRIORITY - 1);
#अन्यथा
    /* Test vTaskDelay directly on the current core */
    test_vTaskDelayUntil(शून्य);
#पूर्ण_अगर

    portTEST_REF_CLOCK_DEINIT();
}

#पूर्ण_अगर /* ( INCLUDE_xTaskDelayUntil == 1 ) */
#पूर्ण_अगर //SOC_GPTIMER_SUPPORTED
