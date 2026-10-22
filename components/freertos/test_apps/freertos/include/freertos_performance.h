/* SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ہدایت once

#شامل "sdkconfig.h"

#اگر_غیر_تعریف_شدہ IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_ادوار_فی_OP
#اگر CONFIG_FREERTOS_پورٹ_THREAD_محفوظ_CLAIM
#تعریف_کرو IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_ادوار_فی_OP     230
#ورنہ
#تعریف_کرو IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_ادوار_فی_OP     215
#اگر_ختم
#اگر_ختم
#اگر_غیر_تعریف_شدہ IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_ادوار_فی_OP_PSRAM
#تعریف_کرو IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_ادوار_فی_OP_PSRAM               300
#اگر_ختم
#اگر_غیر_تعریف_شدہ IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_ادوار_فی_OP_UNICORE
#تعریف_کرو IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_ادوار_فی_OP_UNICORE             130
#اگر_ختم

#اگر_غیر_تعریف_شدہ IDF_PERFORMANCE_MAX_ISR_ENTER_ادوار
#تعریف_کرو IDF_PERFORMANCE_MAX_ISR_ENTER_ادوار                                    290
#اگر_ختم
#اگر_غیر_تعریف_شدہ IDF_PERFORMANCE_MAX_ISR_EXIT_ادوار
#تعریف_کرو IDF_PERFORMANCE_MAX_ISR_EXIT_ادوار                                     565
#اگر_ختم

#اگر_غیر_تعریف_شدہ IDF_PERFORMANCE_MAX_SCHEDULING_وقت
#تعریف_کرو IDF_PERFORMANCE_MAX_SCHEDULING_وقت                                     2000
#اگر_ختم

// Chip-Specific Data
#اگر CONFIG_IDF_ہدف_ESP32_P4 || CONFIG_IDF_ہدف_ESP32_S31 || CONFIG_IDF_ہدف_ESP32_H4
/* Spinlock performance on esp32p4/esp32s31/esp32h4 is slower. */
#تعریف_منسوخ IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_ادوار_فی_OP
#تعریف_کرو IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_ادوار_فی_OP                     400
#تعریف_منسوخ IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_ادوار_فی_OP_UNICORE
#تعریف_کرو IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_ادوار_فی_OP_UNICORE             150

/* Solicited yields (portYIELD() or taskYIELD()) take longer on esp32p4/esp32s31/esp32h4. TODO: IDF-2809 */
#تعریف_منسوخ IDF_PERFORMANCE_MAX_SCHEDULING_وقت
#تعریف_کرو IDF_PERFORMANCE_MAX_SCHEDULING_وقت                                     3200
#اگر_ختم
