/* SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#رهنمود once

#ضمن "sdkconfig.h"

#اگر_تعریف_نشده IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_چرخهها_در_هر_OP
#إذا CONFIG_FREERTOS_پورت_THREAD_امن_CLAIM
#تعریف_کن IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_چرخهها_در_هر_OP     230
#وإلا
#تعریف_کن IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_چرخهها_در_هر_OP     215
#پایان_اگر
#پایان_اگر
#اگر_تعریف_نشده IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_چرخهها_در_هر_OP_PSRAM
#تعریف_کن IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_چرخهها_در_هر_OP_PSRAM               300
#پایان_اگر
#اگر_تعریف_نشده IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_چرخهها_در_هر_OP_UNICORE
#تعریف_کن IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_چرخهها_در_هر_OP_UNICORE             130
#پایان_اگر

#اگر_تعریف_نشده IDF_PERFORMANCE_MAX_ISR_ENTER_چرخهها
#تعریف_کن IDF_PERFORMANCE_MAX_ISR_ENTER_چرخهها                                    290
#پایان_اگر
#اگر_تعریف_نشده IDF_PERFORMANCE_MAX_ISR_EXIT_چرخهها
#تعریف_کن IDF_PERFORMANCE_MAX_ISR_EXIT_چرخهها                                     565
#پایان_اگر

#اگر_تعریف_نشده IDF_PERFORMANCE_MAX_SCHEDULING_زمان
#تعریف_کن IDF_PERFORMANCE_MAX_SCHEDULING_زمان                                     2000
#پایان_اگر

// Chip-Specific Data
#إذا CONFIG_IDF_هدف_ESP32_P4 || CONFIG_IDF_هدف_ESP32_S31 || CONFIG_IDF_هدف_ESP32_H4
/* Spinlock performance on esp32p4/esp32s31/esp32h4 is slower. */
#لغو_تعریف IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_چرخهها_در_هر_OP
#تعریف_کن IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_چرخهها_در_هر_OP                     400
#لغو_تعریف IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_چرخهها_در_هر_OP_UNICORE
#تعریف_کن IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_چرخهها_در_هر_OP_UNICORE             150

/* Solicited yields (portYIELD() or taskYIELD()) take longer on esp32p4/esp32s31/esp32h4. TODO: IDF-2809 */
#لغو_تعریف IDF_PERFORMANCE_MAX_SCHEDULING_زمان
#تعریف_کن IDF_PERFORMANCE_MAX_SCHEDULING_زمان                                     3200
#پایان_اگر
