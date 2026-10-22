/* SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_ਚੱਕਰਾਂ_ਪ੍ਰਤੀ_OP
#ਜੇ CONFIG_FREERTOS_ਪੋਰਟ_THREAD_ਸੁਰੱਖਿਅਤ_CLAIM
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_ਚੱਕਰਾਂ_ਪ੍ਰਤੀ_OP     230
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_ਚੱਕਰਾਂ_ਪ੍ਰਤੀ_OP     215
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_ਚੱਕਰਾਂ_ਪ੍ਰਤੀ_OP_PSRAM
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_ਚੱਕਰਾਂ_ਪ੍ਰਤੀ_OP_PSRAM               300
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_ਚੱਕਰਾਂ_ਪ੍ਰਤੀ_OP_UNICORE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_ਚੱਕਰਾਂ_ਪ੍ਰਤੀ_OP_UNICORE             130
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ IDF_PERFORMANCE_MAX_ISR_ENTER_ਚੱਕਰਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IDF_PERFORMANCE_MAX_ISR_ENTER_ਚੱਕਰਾਂ                                    290
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ IDF_PERFORMANCE_MAX_ISR_EXIT_ਚੱਕਰਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IDF_PERFORMANCE_MAX_ISR_EXIT_ਚੱਕਰਾਂ                                     565
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ IDF_PERFORMANCE_MAX_SCHEDULING_ਸਮਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IDF_PERFORMANCE_MAX_SCHEDULING_ਸਮਾਂ                                     2000
#ਜੇ_ਅੰਤ

// Chip-Specific Data
#ਜੇ CONFIG_IDF_ਨਿਸ਼ਾਨਾ_ESP32_P4 || CONFIG_IDF_ਨਿਸ਼ਾਨਾ_ESP32_S31 || CONFIG_IDF_ਨਿਸ਼ਾਨਾ_ESP32_H4
/* Spinlock performance on esp32p4/esp32s31/esp32h4 is slower. */
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_ਚੱਕਰਾਂ_ਪ੍ਰਤੀ_OP
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_ਚੱਕਰਾਂ_ਪ੍ਰਤੀ_OP                     400
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_ਚੱਕਰਾਂ_ਪ੍ਰਤੀ_OP_UNICORE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_ਚੱਕਰਾਂ_ਪ੍ਰਤੀ_OP_UNICORE             150

/* Solicited yields (portYIELD() or taskYIELD()) take longer on esp32p4/esp32s31/esp32h4. TODO: IDF-2809 */
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ IDF_PERFORMANCE_MAX_SCHEDULING_ਸਮਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IDF_PERFORMANCE_MAX_SCHEDULING_ਸਮਾਂ                                     3200
#ਜੇ_ਅੰਤ
