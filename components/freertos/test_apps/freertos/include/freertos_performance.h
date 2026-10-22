/* SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#هدايت once

#ضمن "sdkconfig.h"

#جيڪڏهن_وصف_نه_ٿيل IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_چڪرن_في_OP
#إذا CONFIG_FREERTOS_پورٽ_THREAD_محفوظ_CLAIM
#وصف_ڪريو IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_چڪرن_في_OP     230
#وإلا
#وصف_ڪريو IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_چڪرن_في_OP     215
#جيڪڏهن_پورو
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_چڪرن_في_OP_PSRAM
#وصف_ڪريو IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_چڪرن_في_OP_PSRAM               300
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_چڪرن_في_OP_UNICORE
#وصف_ڪريو IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_چڪرن_في_OP_UNICORE             130
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل IDF_PERFORMANCE_MAX_ISR_ENTER_چڪرن
#وصف_ڪريو IDF_PERFORMANCE_MAX_ISR_ENTER_چڪرن                                    290
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل IDF_PERFORMANCE_MAX_ISR_EXIT_چڪرن
#وصف_ڪريو IDF_PERFORMANCE_MAX_ISR_EXIT_چڪرن                                     565
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل IDF_PERFORMANCE_MAX_SCHEDULING_وقت
#وصف_ڪريو IDF_PERFORMANCE_MAX_SCHEDULING_وقت                                     2000
#جيڪڏهن_پورو

// Chip-Specific Data
#إذا CONFIG_IDF_هدف_ESP32_P4 || CONFIG_IDF_هدف_ESP32_S31 || CONFIG_IDF_هدف_ESP32_H4
/* Spinlock performance on esp32p4/esp32s31/esp32h4 is slower. */
#وصف_رد IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_چڪرن_في_OP
#وصف_ڪريو IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_چڪرن_في_OP                     400
#وصف_رد IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_چڪرن_في_OP_UNICORE
#وصف_ڪريو IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_چڪرن_في_OP_UNICORE             150

/* Solicited yields (portYIELD() or taskYIELD()) take longer on esp32p4/esp32s31/esp32h4. TODO: IDF-2809 */
#وصف_رد IDF_PERFORMANCE_MAX_SCHEDULING_وقت
#وصف_ڪريو IDF_PERFORMANCE_MAX_SCHEDULING_وقت                                     3200
#جيڪڏهن_پورو
