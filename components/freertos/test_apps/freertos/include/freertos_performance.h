/* SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ہدایت once

#ضمن "sdkconfig.h"

#جے_بنا_تعریف IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_چکراں_فی_OP
#إذا CONFIG_FREERTOS_پورٹ_THREAD_محفوظ_CLAIM
#تعریف_کرو IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_چکراں_فی_OP     230
#وإلا
#تعریف_کرو IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_چکراں_فی_OP     215
#جے_انت
#جے_انت
#جے_بنا_تعریف IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_چکراں_فی_OP_PSRAM
#تعریف_کرو IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_چکراں_فی_OP_PSRAM               300
#جے_انت
#جے_بنا_تعریف IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_چکراں_فی_OP_UNICORE
#تعریف_کرو IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_چکراں_فی_OP_UNICORE             130
#جے_انت

#جے_بنا_تعریف IDF_PERFORMANCE_MAX_ISR_ENTER_چکراں
#تعریف_کرو IDF_PERFORMANCE_MAX_ISR_ENTER_چکراں                                    290
#جے_انت
#جے_بنا_تعریف IDF_PERFORMANCE_MAX_ISR_EXIT_چکراں
#تعریف_کرو IDF_PERFORMANCE_MAX_ISR_EXIT_چکراں                                     565
#جے_انت

#جے_بنا_تعریف IDF_PERFORMANCE_MAX_SCHEDULING_ویلا
#تعریف_کرو IDF_PERFORMANCE_MAX_SCHEDULING_ویلا                                     2000
#جے_انت

// Chip-Specific Data
#إذا CONFIG_IDF_نشانہ_ESP32_P4 || CONFIG_IDF_نشانہ_ESP32_S31 || CONFIG_IDF_نشانہ_ESP32_H4
/* Spinlock performance on esp32p4/esp32s31/esp32h4 is slower. */
#تعریف_رد IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_چکراں_فی_OP
#تعریف_کرو IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_چکراں_فی_OP                     400
#تعریف_رد IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_چکراں_فی_OP_UNICORE
#تعریف_کرو IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_چکراں_فی_OP_UNICORE             150

/* Solicited yields (portYIELD() or taskYIELD()) take longer on esp32p4/esp32s31/esp32h4. TODO: IDF-2809 */
#تعریف_رد IDF_PERFORMANCE_MAX_SCHEDULING_ویلا
#تعریف_کرو IDF_PERFORMANCE_MAX_SCHEDULING_ویلا                                     3200
#جے_انت
