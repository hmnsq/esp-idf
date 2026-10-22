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

#അന്തര്ഭാവ <stdint.h>
#അന്തര്ഭാവ <stdlib.h> // This is for malloc(), used by portmacro.h
#അന്തര്ഭാവ "sdkconfig.h"
#അന്തര്ഭാവ "esp_attr.h"
#അന്തര്ഭാവ "esp_task.h"
#അന്തര്ഭാവ "spinlock.h"

#ifdef __cplusplus
ബാഹ്യ "C" {
#endif

തരനിർവചനം spinlock_t portMUX_TYPE;

/**< Spinlock initializer */
#define portMUX_INITIALIZER_UNLOCKED {                      \
            .owner = portMUX_FREE_VAL,                      \
            .എണ്ണം = 0,                                     \
        }
#define portMUX_FREE_VAL                    SPINLOCK_FREE           /**< Spinlock is free. [refactor-todo] check if this is still required */

ശൂന്യം vPortYieldFromISR(ശൂന്യം);
ശൂന്യം vPortYieldOtherCore(BaseType_t coreid);

#define portMUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

/**
 * @brief Get the current core's ID
 *
 * @note dummy function for freertos simulator, always returns 0.
 @ return BaseType_t 0
 */
സ്ഥിതം വരിയിൽ BaseType_t xPortGetCoreID(ശൂന്യം)
{
    തിരികെ (BaseType_t) 0;
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
bool xPortCheckValidListMem(സ്ഥിരം ശൂന്യം *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's TCB
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a TCB
 * @return false Otherwise
 */
bool xPortCheckValidTCBMem(സ്ഥിരം ശൂന്യം *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's stack
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a task stack
 * @return false Otherwise
 */
bool x_Portcheck_Valid_അട്ടി_Mem(സ്ഥിരം ശൂന്യം *ptr);

#define portVALID_LIST_MEM(ptr)     xPortCheckValidListMem(ptr)
#define portVALID_TCB_MEM(ptr)      xPortCheckValidTCBMem(ptr)
#define port_VALID_അട്ടി_MEM(ptr)    x_Portcheck_Valid_അട്ടി_Mem(ptr)

BaseType_t xPortCheckIfInISR(ശൂന്യം);

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
സ്ഥിതം വരിയിൽ BaseType_t xPortInIsrContext(ശൂന്യം)
{
    //Just call the FreeRTOS port interface version
    തിരികെ xPortCheckIfInISR();
}

സ്ഥിതം വരിയിൽ ശൂന്യം vPortAssertIfInISR(ശൂന്യം)
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
