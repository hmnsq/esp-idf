/* SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#لارښوونه once

#ضمن "sdkconfig.h"

#که_نه_تعریف_شوی IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_دورې_فی_OP
#إذا CONFIG_FREERTOS_پورټ_THREAD_خوندي_CLAIM
#تعریف_کړه IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_دورې_فی_OP     230
#وإلا
#تعریف_کړه IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_دورې_فی_OP     215
#د_که_پای
#د_که_پای
#که_نه_تعریف_شوی IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_دورې_فی_OP_PSRAM
#تعریف_کړه IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_دورې_فی_OP_PSRAM               300
#د_که_پای
#که_نه_تعریف_شوی IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_دورې_فی_OP_UNICORE
#تعریف_کړه IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_دورې_فی_OP_UNICORE             130
#د_که_پای

#که_نه_تعریف_شوی IDF_PERFORMANCE_MAX_ISR_ENTER_دورې
#تعریف_کړه IDF_PERFORMANCE_MAX_ISR_ENTER_دورې                                    290
#د_که_پای
#که_نه_تعریف_شوی IDF_PERFORMANCE_MAX_ISR_EXIT_دورې
#تعریف_کړه IDF_PERFORMANCE_MAX_ISR_EXIT_دورې                                     565
#د_که_پای

#که_نه_تعریف_شوی IDF_PERFORMANCE_MAX_SCHEDULING_وخت
#تعریف_کړه IDF_PERFORMANCE_MAX_SCHEDULING_وخت                                     2000
#د_که_پای

// Chip-Specific Data
#إذا CONFIG_IDF_موخه_ESP32_P4 || CONFIG_IDF_موخه_ESP32_S31 || CONFIG_IDF_موخه_ESP32_H4
/* Spinlock performance on esp32p4/esp32s31/esp32h4 is slower. */
#تعریف_لغوه IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_دورې_فی_OP
#تعریف_کړه IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_دورې_فی_OP                     400
#تعریف_لغوه IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_دورې_فی_OP_UNICORE
#تعریف_کړه IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_دورې_فی_OP_UNICORE             150

/* Solicited yields (portYIELD() or taskYIELD()) take longer on esp32p4/esp32s31/esp32h4. TODO: IDF-2809 */
#تعریف_لغوه IDF_PERFORMANCE_MAX_SCHEDULING_وخت
#تعریف_کړه IDF_PERFORMANCE_MAX_SCHEDULING_وخت                                     3200
#د_که_پای
