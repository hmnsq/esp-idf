/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#आशय once

/* Linux POSIX simulator specific configuration. This file is included in the common FreeRTOSConfig.h. */

#समावेश "sdkconfig.h"

/* ------------------------------------------------- FreeRTOS Config ---------------------------------------------------
 * - All Vanilla FreeRTOS configuration goes into this section
 * - Match upstream POSIX simulator example FreeRTOSConfig.h where possible. See following link for more details.
 *   https://github.com/FreeRTOS/FreeRTOS/blob/main/FreeRTOS/Demo/Posix_GCC/FreeRTOSConfig.h
 * ------------------------------------------------------------------------------------------------------------------ */

/* ------------------ Scheduler Related -------------------- */

#घोषणा configUSE_PORT_OPTIMISED_TASK_SELECTION    0

/* The stack allocated by FreeRTOS will be passed to a pthread.
 * pthread has a minimal stack size which currently is 16KB.
 * The rest is for additional structures of the POSIX/Linux port.
 * This is a magic number since PTHREAD_STACK_MIN seems to not be a constant. */
#घोषणा config_MINIMAL_ढेर_आकार                   ( ( ढेर_Type_t ) ( 0x4000 + 40 ) / माप( ढेर_Type_t ) )
/* Currently not used in Linux POSIX simulator */
#घोषणा configMAX_API_CALL_INTERRUPT_PRIORITY      0

/* ---------------- Amazon SMP FreeRTOS -------------------- */

#अगर CONFIG_FREERTOS_SMP
    #घोषणा configUSE_PASSIVE_IDLE_HOOK    0             /* Not implemented yet, TODO IDF-6654 */
#पूर्ण_अगर

/* ----------------------- System -------------------------- */

/* On the Linux simulator, we use the system-provided libc */
#घोषणा configUSE_NEWLIB_REENTRANT                   0

#घोषणा configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H    1

/* ----------------------- Memory  ------------------------- */

#घोषणा config_योग_HEAP_आकार    ( ( माप_प्रकार ) ( 65 * 1024 ) )

/* ------------------- Run-time Stats ---------------------- */

#घोषणा configUSE_TRACE_FACILITY    1

/* -------------------- API Includes ----------------------- */

/* Todo: Reconcile INCLUDE_option differences (IDF-8186) */
#घोषणा INCLUDE_vTaskDelayUntil                 1
#घोषणा INCLUDE_ux_Task_Get_ढेर_ऊपरी_Water_Mark2    0

/* ------------------------------------------------ ESP-IDF Additions --------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */
