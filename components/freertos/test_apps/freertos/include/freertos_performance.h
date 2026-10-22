/* SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#निर्देशः once

#अन्तर्भाव "sdkconfig.h"

#यदि_अपरिभाषितम् IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_आवृत्तयः_प्रत्येकम्_OP
#यदि CONFIG_FREERTOS_पत्तनम्_THREAD_SAFE_CLAIM
#परिभाषय IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_आवृत्तयः_प्रत्येकम्_OP     230
#अन्यथा
#परिभाषय IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_आवृत्तयः_प्रत्येकम्_OP     215
#यदि_अन्तः
#यदि_अन्तः
#यदि_अपरिभाषितम् IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_आवृत्तयः_प्रत्येकम्_OP_PSRAM
#परिभाषय IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_आवृत्तयः_प्रत्येकम्_OP_PSRAM               300
#यदि_अन्तः
#यदि_अपरिभाषितम् IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_आवृत्तयः_प्रत्येकम्_OP_UNICORE
#परिभाषय IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_आवृत्तयः_प्रत्येकम्_OP_UNICORE             130
#यदि_अन्तः

#यदि_अपरिभाषितम् IDF_PERFORMANCE_MAX_ISR_ENTER_आवृत्तयः
#परिभाषय IDF_PERFORMANCE_MAX_ISR_ENTER_आवृत्तयः                                    290
#यदि_अन्तः
#यदि_अपरिभाषितम् IDF_PERFORMANCE_MAX_ISR_EXIT_आवृत्तयः
#परिभाषय IDF_PERFORMANCE_MAX_ISR_EXIT_आवृत्तयः                                     565
#यदि_अन्तः

#यदि_अपरिभाषितम् IDF_PERFORMANCE_MAX_SCHEDULING_TIME
#परिभाषय IDF_PERFORMANCE_MAX_SCHEDULING_TIME                                     2000
#यदि_अन्तः

// Chip-Specific Data
#यदि CONFIG_IDF_TARGET_ESP32P4 || CONFIG_IDF_TARGET_ESP32S31 || CONFIG_IDF_TARGET_ESP32H4
/* Spinlock performance on esp32p4/esp32s31/esp32h4 is slower. */
#अपरिभाषय IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_आवृत्तयः_प्रत्येकम्_OP
#परिभाषय IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_आवृत्तयः_प्रत्येकम्_OP                     400
#अपरिभाषय IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_आवृत्तयः_प्रत्येकम्_OP_UNICORE
#परिभाषय IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_आवृत्तयः_प्रत्येकम्_OP_UNICORE             150

/* Solicited yields (portYIELD() or taskYIELD()) take longer on esp32p4/esp32s31/esp32h4. TODO: IDF-2809 */
#अपरिभाषय IDF_PERFORMANCE_MAX_SCHEDULING_TIME
#परिभाषय IDF_PERFORMANCE_MAX_SCHEDULING_TIME                                     3200
#यदि_अन्तः
