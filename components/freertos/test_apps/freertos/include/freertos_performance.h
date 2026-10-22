/* SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_ಚಕ್ರಗಳು_ಪ್ರತಿ_OP
#ಆದರೆ CONFIG_FREERTOS_ಪೋರ್ಟ್_THREAD_ಸುರಕ್ಷಿತ_CLAIM
#ವ್ಯಾಖ್ಯಾನಿಸು IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_ಚಕ್ರಗಳು_ಪ್ರತಿ_OP     230
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_ಚಕ್ರಗಳು_ಪ್ರತಿ_OP     215
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_ಚಕ್ರಗಳು_ಪ್ರತಿ_OP_PSRAM
#ವ್ಯಾಖ್ಯಾನಿಸು IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_ಚಕ್ರಗಳು_ಪ್ರತಿ_OP_PSRAM               300
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_ಚಕ್ರಗಳು_ಪ್ರತಿ_OP_UNICORE
#ವ್ಯಾಖ್ಯಾನಿಸು IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_ಚಕ್ರಗಳು_ಪ್ರತಿ_OP_UNICORE             130
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ IDF_PERFORMANCE_MAX_ISR_ENTER_ಚಕ್ರಗಳು
#ವ್ಯಾಖ್ಯಾನಿಸು IDF_PERFORMANCE_MAX_ISR_ENTER_ಚಕ್ರಗಳು                                    290
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ IDF_PERFORMANCE_MAX_ISR_EXIT_ಚಕ್ರಗಳು
#ವ್ಯಾಖ್ಯಾನಿಸು IDF_PERFORMANCE_MAX_ISR_EXIT_ಚಕ್ರಗಳು                                     565
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ IDF_PERFORMANCE_MAX_SCHEDULING_ಸಮಯ
#ವ್ಯಾಖ್ಯಾನಿಸು IDF_PERFORMANCE_MAX_SCHEDULING_ಸಮಯ                                     2000
#ಆದರೆ_ಅಂತ್ಯ

// Chip-Specific Data
#ಆದರೆ CONFIG_IDF_ಗುರಿ_ESP32_P4 || CONFIG_IDF_ಗುರಿ_ESP32_S31 || CONFIG_IDF_ಗುರಿ_ESP32_H4
/* Spinlock performance on esp32p4/esp32s31/esp32h4 is slower. */
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_ಚಕ್ರಗಳು_ಪ್ರತಿ_OP
#ವ್ಯಾಖ್ಯಾನಿಸು IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_ಚಕ್ರಗಳು_ಪ್ರತಿ_OP                     400
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_ಚಕ್ರಗಳು_ಪ್ರತಿ_OP_UNICORE
#ವ್ಯಾಖ್ಯಾನಿಸು IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_ಚಕ್ರಗಳು_ಪ್ರತಿ_OP_UNICORE             150

/* Solicited yields (portYIELD() or taskYIELD()) take longer on esp32p4/esp32s31/esp32h4. TODO: IDF-2809 */
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು IDF_PERFORMANCE_MAX_SCHEDULING_ಸಮಯ
#ವ್ಯಾಖ್ಯಾನಿಸು IDF_PERFORMANCE_MAX_SCHEDULING_ಸಮಯ                                     3200
#ಆದರೆ_ಅಂತ್ಯ
