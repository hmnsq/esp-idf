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

#आशय once

#समावेश <stdint.h>
#समावेश <stdlib.h> // This is for malloc(), used by portmacro.h
#समावेश "sdkconfig.h"
#समावेश "esp_attr.h"
#समावेश "esp_task.h"
#समावेश "spinlock.h"

#अगर_घोषित __cplusplus
बाह्य "C" {
#पूर्ण_अगर

प्रकार spinlock_t portMUX_TYPE;

/**< Spinlock initializer */
#घोषणा portMUX_INITIALIZER_UNLOCKED {                      \
            .owner = portMUX_FREE_VAL,                      \
            .गिनती = 0,                                     \
        }
#घोषणा portMUX_FREE_VAL                    SPINLOCK_FREE           /**< Spinlock is free. [refactor-todo] check if this is still required */

व्योम vPortYieldFromISR(व्योम);
व्योम vPortYieldOtherCore(BaseType_t coreid);

#घोषणा portMUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

/**
 * @brief Get the current core's ID
 *
 * @note dummy function for freertos simulator, always returns 0.
 @ return BaseType_t 0
 */
अटल अंतरभूत BaseType_t xPortGetCoreID(व्योम)
{
    वापस (BaseType_t) 0;
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
bool xPortCheckValidListMem(स्थिर व्योम *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's TCB
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a TCB
 * @return false Otherwise
 */
bool xPortCheckValidTCBMem(स्थिर व्योम *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's stack
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a task stack
 * @return false Otherwise
 */
bool x_Portcheck_Valid_ढेर_Mem(स्थिर व्योम *ptr);

#घोषणा portVALID_LIST_MEM(ptr)     xPortCheckValidListMem(ptr)
#घोषणा portVALID_TCB_MEM(ptr)      xPortCheckValidTCBMem(ptr)
#घोषणा port_VALID_ढेर_MEM(ptr)    x_Portcheck_Valid_ढेर_Mem(ptr)

BaseType_t xPortCheckIfInISR(व्योम);

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
अटल अंतरभूत BaseType_t xPortInIsrContext(व्योम)
{
    //Just call the FreeRTOS port interface version
    वापस xPortCheckIfInISR();
}

अटल अंतरभूत व्योम vPortAssertIfInISR(व्योम)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(xPortInIsrContext() == 0);
}

/**
 * @brief Assert if in ISR context
 */
#घोषणा portASSERT_IF_IN_ISR() vPortAssertIfInISR()

#अगर_घोषित __cplusplus
}
#पूर्ण_अगर
