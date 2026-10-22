/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#अन्तर्भाव "sdkconfig.h"
#अन्तर्भाव "FreeRTOS.h"
#अन्तर्भाव "task.h"
#अन्तर्भाव "semphr.h"
#अन्तर्भाव "unity.h"
#अन्तर्भाव "freertos_test_utils.h"
#अन्तर्भाव "portTestMacro.h"

/* ------------------------------------------------------------------------------------------------------------------ */
#यदि SOC_GPTIMER_SUPPORTED

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

#यदि ( INCLUDE_vTaskDelay == 1 )

#परिभाषय TEST_VTASKDELAY_TICKS           5   // Number of ticks to delay in test
#परिभाषय TEST_VTASKDELAY_ITERATIONS      5   // Number of iterations in test
#यदि CONFIG_FREERTOS_SMP
#परिभाषय TEST_VTASKDELAY_DELTA_TICKS     1   // Number of ticks worth of delta allowed
#अन्यथा
#परिभाषय TEST_VTASKDELAY_DELTA_TICKS     2   // Number of ticks worth of delta allowed. We allow 2 ticks in IDF FreeRTOS as each core's tick interrupt could be out of phase
#यदि_अन्तः

स्थिर शून्य test_vTaskDelay(शून्य *arg)
{
    प्रति (पूर्णाङ्क i = 0; i < TEST_VTASKDELAY_ITERATIONS; i++) {
        TickType_t tick_start, tick_अन्तः;
        पत्तनम्_TEST_REF_CLOCK_TYPE ref_clock_start, ref_clock_अन्तः;

        /* Delay until the next tick boundary */
        vTaskDelay(1);

        /* Get the current tick count and ref clock time */
        tick_start = x_Task_Get_Tick_गणनम्();
        ref_clock_start = पत्तनम्_TEST_REF_CLOCK_GET_TIME();

        vTaskDelay(TEST_VTASKDELAY_TICKS);

        /* Get the post delay tick count and ref clock time */
        tick_अन्तः = x_Task_Get_Tick_गणनम्();
        ref_clock_अन्तः = पत्तनम्_TEST_REF_CLOCK_GET_TIME();

        /* Check that elapsed ticks and ref clock is accurate. We allow TEST_VTASKDELAY_DELTA_TICKS of error in case
         * vTaskDelay() or portTEST_REF_CLOCK_GET_TIME() last long enough to cross a tick boundary */
#यदि ( configUSE_16_BIT_TICKS == 1 )
        TEST_ASSERT_UINT16_WITHIN(TEST_VTASKDELAY_DELTA_TICKS, TEST_VTASKDELAY_TICKS, tick_अन्तः - tick_start);
#अन्यथा
        TEST_ASSERT_UINT32_WITHIN(TEST_VTASKDELAY_DELTA_TICKS, TEST_VTASKDELAY_TICKS, tick_अन्तः - tick_start);
#यदि_अन्तः
        TEST_ASSERT_UINT32_WITHIN(पत्तनम्_TEST_TICKS_गन्तव्यम्_REF_CLOCK(TEST_VTASKDELAY_DELTA_TICKS),
                                  पत्तनम्_TEST_TICKS_गन्तव्यम्_REF_CLOCK(TEST_VTASKDELAY_TICKS),
                                  ref_clock_अन्तः - ref_clock_start);
    }
}

TEST_CASE("Tasks: Test vTaskDelay", "[freertos]")
{
    पत्तनम्_TEST_REF_CLOCK_INIT();

#यदि ( CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES > 1 )
    vTestOnAllCores(test_vTaskDelay, रिक्त, config_TEST_DEFAULT_राशिः_आकार, config_TEST_UNITY_TASK_प्राथमिकता - 1);
#अन्यथा
    /* Test vTaskDelay directly on the current core */
    test_vTaskDelay(रिक्त);
#यदि_अन्तः

    पत्तनम्_TEST_REF_CLOCK_DEINIT();
}

#यदि_अन्तः /* ( INCLUDE_vTaskDelay == 1 ) */

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

#यदि ( INCLUDE_xTaskDelayUntil == 1 )

#परिभाषय TEST_VTASKDELAYUNTIL_TICKS              5   // Number of ticks to delay in test
#परिभाषय TEST_VTASKDELAYUNTIL_ITERATIONS         5   // Number of iterations in test
#यदि CONFIG_FREERTOS_SMP
#परिभाषय TEST_VTASKDELAYUNTIL_DELTA_TICKS        1   // Number of ticks worth of delta allowed
#अन्यथा
#परिभाषय TEST_VTASKDELAYUNTIL_DELTA_TICKS        2   // Number of ticks worth of delta allowed. We allow 2 ticks in IDF FreeRTOS as each core's tick interrupt could be out of phase
#यदि_अन्तः

स्थिर शून्य test_vTaskDelayUntil(शून्य *arg)
{
    प्रति (पूर्णाङ्क i = 0; i < TEST_VTASKDELAYUNTIL_ITERATIONS; i++) {
        TickType_t tick_start, tick_अन्तः, चरमः_wake_tick;
        पत्तनम्_TEST_REF_CLOCK_TYPE ref_clock_start, ref_clock_अन्तः;

        /* Delay until the next tick boundary */
        vTaskDelay(1);

        /* Get the current tick count and ref clock time */
        tick_start = x_Task_Get_Tick_गणनम्();
        चरमः_wake_tick = tick_start;
        ref_clock_start = पत्तनम्_TEST_REF_CLOCK_GET_TIME();

        vTaskDelayUntil(&चरमः_wake_tick, TEST_VTASKDELAYUNTIL_TICKS);

        /* Get the post delay tick count and ref clock time */
        tick_अन्तः = x_Task_Get_Tick_गणनम्();
        ref_clock_अन्तः = पत्तनम्_TEST_REF_CLOCK_GET_TIME();

        /* Check that elapsed ticks and ref clock is accurate. We allow TEST_VTASKDELAYUNTIL_DELTA_TICKS of error in
         * case vTaskDelayUntil() or portTEST_REF_CLOCK_GET_TIME() last long enough to cross a tick boundary */
#यदि ( configUSE_16_BIT_TICKS == 1 )
        TEST_ASSERT_UINT16_WITHIN(TEST_VTASKDELAYUNTIL_DELTA_TICKS, TEST_VTASKDELAYUNTIL_TICKS, tick_अन्तः - tick_start);
        TEST_ASSERT_UINT16_WITHIN(TEST_VTASKDELAYUNTIL_DELTA_TICKS, tick_अन्तः, चरमः_wake_tick);
#अन्यथा
        TEST_ASSERT_UINT32_WITHIN(TEST_VTASKDELAYUNTIL_DELTA_TICKS, TEST_VTASKDELAYUNTIL_TICKS, tick_अन्तः - tick_start);
        TEST_ASSERT_UINT32_WITHIN(TEST_VTASKDELAYUNTIL_DELTA_TICKS, tick_अन्तः, चरमः_wake_tick);
#यदि_अन्तः

        /* Check that the elapsed ref clock time is accurate. We allow TEST_VTASKDELAYUNTIL_DELTA_TICKS time worth of
         * error to account for the execution time of the ref clock functions. */
        TEST_ASSERT_UINT32_WITHIN(पत्तनम्_TEST_TICKS_गन्तव्यम्_REF_CLOCK(TEST_VTASKDELAYUNTIL_DELTA_TICKS),
                                  पत्तनम्_TEST_TICKS_गन्तव्यम्_REF_CLOCK(TEST_VTASKDELAYUNTIL_TICKS),
                                  ref_clock_अन्तः - ref_clock_start);
    }
}

TEST_CASE("Tasks: Test vTaskDelayUntil", "[freertos]")
{
    पत्तनम्_TEST_REF_CLOCK_INIT();

#यदि ( CONFIG_FREERTOS_सङ्ख्या_सम्बन्धि_CORES > 1 )
    vTestOnAllCores(test_vTaskDelayUntil, रिक्त, config_TEST_DEFAULT_राशिः_आकार, config_TEST_UNITY_TASK_प्राथमिकता - 1);
#अन्यथा
    /* Test vTaskDelay directly on the current core */
    test_vTaskDelayUntil(रिक्त);
#यदि_अन्तः

    पत्तनम्_TEST_REF_CLOCK_DEINIT();
}

#यदि_अन्तः /* ( INCLUDE_xTaskDelayUntil == 1 ) */
#यदि_अन्तः //SOC_GPTIMER_SUPPORTED
