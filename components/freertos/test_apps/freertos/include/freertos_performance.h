/* SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#సూచన once

#అన్తర్భావ "sdkconfig.h"

#నిర్వచితం_కాకపోతే IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_చక్రాలు_చొప్పున_OP
#ఒకవేళ CONFIG_FREERTOS_పోర్టు_THREAD_సురక్షిత_CLAIM
#నిర్వచించు IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_చక్రాలు_చొప్పున_OP     230
#లేకపోతే
#నిర్వచించు IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_చక్రాలు_చొప్పున_OP     215
#అయితే_ముగింపు
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_చక్రాలు_చొప్పున_OP_PSRAM
#నిర్వచించు IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_చక్రాలు_చొప్పున_OP_PSRAM               300
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_చక్రాలు_చొప్పున_OP_UNICORE
#నిర్వచించు IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_చక్రాలు_చొప్పున_OP_UNICORE             130
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే IDF_PERFORMANCE_MAX_ISR_ENTER_చక్రాలు
#నిర్వచించు IDF_PERFORMANCE_MAX_ISR_ENTER_చక్రాలు                                    290
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే IDF_PERFORMANCE_MAX_ISR_EXIT_చక్రాలు
#నిర్వచించు IDF_PERFORMANCE_MAX_ISR_EXIT_చక్రాలు                                     565
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే IDF_PERFORMANCE_MAX_SCHEDULING_సమయం
#నిర్వచించు IDF_PERFORMANCE_MAX_SCHEDULING_సమయం                                     2000
#అయితే_ముగింపు

// Chip-Specific Data
#ఒకవేళ CONFIG_IDF_లక్ష్యం_ESP32_P4 || CONFIG_IDF_లక్ష్యం_ESP32_S31 || CONFIG_IDF_లక్ష్యం_ESP32_H4
/* Spinlock performance on esp32p4/esp32s31/esp32h4 is slower. */
#నిర్వచనం_రద్దు IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_చక్రాలు_చొప్పున_OP
#నిర్వచించు IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_చక్రాలు_చొప్పున_OP                     400
#నిర్వచనం_రద్దు IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_చక్రాలు_చొప్పున_OP_UNICORE
#నిర్వచించు IDF_PERFORMANCE_MAX_FREERTOS_SPINLOCK_చక్రాలు_చొప్పున_OP_UNICORE             150

/* Solicited yields (portYIELD() or taskYIELD()) take longer on esp32p4/esp32s31/esp32h4. TODO: IDF-2809 */
#నిర్వచనం_రద్దు IDF_PERFORMANCE_MAX_SCHEDULING_సమయం
#నిర్వచించు IDF_PERFORMANCE_MAX_SCHEDULING_సమయం                                     3200
#అయితే_ముగింపు
