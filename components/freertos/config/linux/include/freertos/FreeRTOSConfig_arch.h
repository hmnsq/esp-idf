/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#సూచన once

/* Linux POSIX simulator specific configuration. This file is included in the common FreeRTOSConfig.h. */

#అన్తర్భావ "sdkconfig.h"

/* ------------------------------------------------- FreeRTOS Config ---------------------------------------------------
 * - All Vanilla FreeRTOS configuration goes into this section
 * - Match upstream POSIX simulator example FreeRTOSConfig.h where possible. See following link for more details.
 *   https://github.com/FreeRTOS/FreeRTOS/blob/main/FreeRTOS/Demo/Posix_GCC/FreeRTOSConfig.h
 * ------------------------------------------------------------------------------------------------------------------ */

/* ------------------ Scheduler Related -------------------- */

#నిర్వచించు config_USE_పోర్టు_OPTIMISED_TASK_SELECTION    0

/* The stack allocated by FreeRTOS will be passed to a pthread.
 * pthread has a minimal stack size which currently is 16KB.
 * The rest is for additional structures of the POSIX/Linux port.
 * This is a magic number since PTHREAD_STACK_MIN seems to not be a constant. */
#నిర్వచించు config_కనీస_దొంతర_కొలత                   ( ( దొంతర_Type_t ) ( 0x4000 + 40 ) / పరిమాణం( దొంతర_Type_t ) )
/* Currently not used in Linux POSIX simulator */
#నిర్వచించు config_MAX_API_CALL_అంతరాయం_ప్రాధాన్యత      0

/* ---------------- Amazon SMP FreeRTOS -------------------- */

#ఒకవేళ CONFIG_FREERTOS_SMP
    #నిర్వచించు configUSE_PASSIVE_IDLE_HOOK    0             /* Not implemented yet, TODO IDF-6654 */
#అయితే_ముగింపు

/* ----------------------- System -------------------------- */

/* On the Linux simulator, we use the system-provided libc */
#నిర్వచించు configUSE_NEWLIB_REENTRANT                   0

#నిర్వచించు configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H    1

/* ----------------------- Memory  ------------------------- */

#నిర్వచించు config_మొత్తం_కుప్ప_కొలత    ( ( size_t ) ( 65 * 1024 ) )

/* ------------------- Run-time Stats ---------------------- */

#నిర్వచించు configUSE_TRACE_FACILITY    1

/* -------------------- API Includes ----------------------- */

/* Todo: Reconcile INCLUDE_option differences (IDF-8186) */
#నిర్వచించు INCLUDE_vTaskDelayUntil                 1
#నిర్వచించు INCLUDE_ux_Task_Get_దొంతర_ఎగువ_Water_Mark2    0

/* ------------------------------------------------ ESP-IDF Additions --------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */
