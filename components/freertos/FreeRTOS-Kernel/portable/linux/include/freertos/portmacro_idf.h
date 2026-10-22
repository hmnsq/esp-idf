/*
 * SPDX-FileCopyrightText: 2022-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * This is the "IDF-part" of the POSIX portmacro.
 * We need additional definitions for code in ESP-IDF which is kept here to separate the original
 * FreeRTOS POSIX port code from the additional IDF POSIX port code.
 */

#pragma once

#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h> // This is for malloc(), used by portmacro.h
#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ "esp_attr.h"
#ಅನ್ತರ್ಭಾವ "esp_task.h"
#ಅನ್ತರ್ಭಾವ "spinlock.h"

#ifdef __cplusplus
ಬಾಹ್ಯ "C" {
#endif

ಪ್ರಕಾರನಿರ್ದೇಶ spinlock_t portMUX_TYPE;

/**< Spinlock initializer */
#define portMUX_INITIALIZER_UNLOCKED {                      \
            .owner = portMUX_FREE_VAL,                      \
            .ಎಣಿಕೆ = 0,                                     \
        }
#define portMUX_FREE_VAL                    SPINLOCK_FREE           /**< Spinlock is free. [refactor-todo] check if this is still required */

ಶೂನ್ಯ vPortYieldFromISR(ಶೂನ್ಯ);
ಶೂನ್ಯ vPortYieldOtherCore(BaseType_t coreid);

#define portMUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

/**
 * @brief Get the current core's ID
 *
 * @note dummy function for freertos simulator, always returns 0.
 @ return BaseType_t 0
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ BaseType_t xPortGetCoreID(ಶೂನ್ಯ)
{
    ಪ್ರತ್ಯಾಗಮ (BaseType_t) 0;
}

/**
 * @brief Checks if a given piece of memory can be used to store a FreeRTOS list
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a List
 * @return false Otherwise
 */
bool xPortCheckValidListMem(ಧ್ರುವ ಶೂನ್ಯ *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's TCB
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a TCB
 * @return false Otherwise
 */
bool xPortCheckValidTCBMem(ಧ್ರುವ ಶೂನ್ಯ *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's stack
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a task stack
 * @return false Otherwise
 */
bool x_Portcheck_Valid_ರಾಶಿ_Mem(ಧ್ರುವ ಶೂನ್ಯ *ptr);

#define portVALID_LIST_MEM(ptr)     xPortCheckValidListMem(ptr)
#define portVALID_TCB_MEM(ptr)      xPortCheckValidTCBMem(ptr)
#define port_VALID_ರಾಶಿ_MEM(ptr)    x_Portcheck_Valid_ರಾಶಿ_Mem(ptr)

BaseType_t xPortCheckIfInISR(ಶೂನ್ಯ);

/**
 * @brief Checks if the current core is in an ISR context
 *
 * - ISR context consist of Low/Mid priority ISR, or time tick ISR
 * - High priority ISRs aren't detected here, but they normally cannot call C code, so that should not be an issue anyway.
 *
 * @note [refactor-todo] Check if this should be inlined
 * @return
 *  - pdTRUE if in ISR
 *  - pdFALSE otherwise
 */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ BaseType_t xPortInIsrContext(ಶೂನ್ಯ)
{
    //Just call the FreeRTOS port interface version
    ಪ್ರತ್ಯಾಗಮ xPortCheckIfInISR();
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ vPortAssertIfInISR(ಶೂನ್ಯ)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(xPortInIsrContext() == 0);
}

/**
 * @brief Assert if in ISR context
 */
#define portASSERT_IF_IN_ISR() vPortAssertIfInISR()

#ifdef __cplusplus
}
#endif
