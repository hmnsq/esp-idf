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

#अन्तर्भाव <stdint.h>
#अन्तर्भाव <stdlib.h> // This is for malloc(), used by portmacro.h
#अन्तर्भाव "sdkconfig.h"
#अन्तर्भाव "esp_attr.h"
#अन्तर्भाव "esp_task.h"
#अन्तर्भाव "spinlock.h"

#ifdef __cplusplus
बाह्य "C" {
#endif

प्रकारनिर्देश spinlock_t portMUX_TYPE;

/**< Spinlock initializer */
#define portMUX_INITIALIZER_UNLOCKED {                      \
            .owner = portMUX_FREE_VAL,                      \
            .मोजणी = 0,                                     \
        }
#define portMUX_FREE_VAL                    SPINLOCK_FREE           /**< Spinlock is free. [refactor-todo] check if this is still required */

शून्य vPortYieldFromISR(शून्य);
शून्य vPortYieldOtherCore(BaseType_t coreid);

#define portMUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

/**
 * @brief Get the current core's ID
 *
 * @note dummy function for freertos simulator, always returns 0.
 @ return BaseType_t 0
 */
स्थिर अन्तःस्थ BaseType_t xPortGetCoreID(शून्य)
{
    प्रत्यागम (BaseType_t) 0;
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
bool xPortCheckValidListMem(ध्रुव शून्य *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's TCB
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a TCB
 * @return false Otherwise
 */
bool xPortCheckValidTCBMem(ध्रुव शून्य *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's stack
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a task stack
 * @return false Otherwise
 */
bool x_Portcheck_Valid_चळत_Mem(ध्रुव शून्य *ptr);

#define portVALID_LIST_MEM(ptr)     xPortCheckValidListMem(ptr)
#define portVALID_TCB_MEM(ptr)      xPortCheckValidTCBMem(ptr)
#define port_VALID_चळत_MEM(ptr)    x_Portcheck_Valid_चळत_Mem(ptr)

BaseType_t xPortCheckIfInISR(शून्य);

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
स्थिर अन्तःस्थ BaseType_t xPortInIsrContext(शून्य)
{
    //Just call the FreeRTOS port interface version
    प्रत्यागम xPortCheckIfInISR();
}

स्थिर अन्तःस्थ शून्य vPortAssertIfInISR(शून्य)
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
