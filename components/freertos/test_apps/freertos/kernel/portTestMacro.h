/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ "test_utils.h"

#ವ್ಯಾಖ್ಯಾನಿಸು config_TEST_DEFAULT_ರಾಶಿ_ಗಾತ್ರ               4096
#ವ್ಯಾಖ್ಯಾನಿಸು config_TEST_UNITY_TASK_ಆದ್ಯತೆ              UNITY_FREERTOS_ಆದ್ಯತೆ

#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TEST_REF_CLOCK_TYPE                     uint32_t
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TEST_REF_CLOCK_INIT()                   ref_clock_init()
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TEST_REF_CLOCK_DEINIT()                 ref_clock_deinit()
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TEST_REF_CLOCK_GET_ಸಮಯ()               ((uint32_t) ref_clock_get())
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_TEST_TICKS_ಗಮ್ಯ_REF_CLOCK(ticks)          ((ticks) * (1000000/config_TICK_ದರ_HZ))
