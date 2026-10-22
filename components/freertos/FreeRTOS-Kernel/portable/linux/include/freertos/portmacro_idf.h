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

#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h> // This is for malloc(), used by portmacro.h
#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ "esp_attr.h"
#ಅನ್ತರ್ಭಾವ "esp_task.h"
#ಅನ್ತರ್ಭಾವ "spinlock.h"

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
ಬಾಹ್ಯ "C" {
#ಆದರೆ_ಅಂತ್ಯ

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ spinlock_t ಪೋರ್ಟ್_MUX_TYPE;

/**< Spinlock initializer */
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_MUX_INITIALIZER_UNLOCKED {                      \
            .owner = ಪೋರ್ಟ್_MUX_FREE_VAL,                      \
            .ಎಣಿಕೆ_2 = 0,                                     \
        }
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_MUX_FREE_VAL                    SPINLOCK_FREE           /**< Spinlock is free. [refactor-todo] check if this is still required */

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Yield_From_ISR(ಶೂನ್ಯ);
ಶೂನ್ಯ v_ಪೋರ್ಟ್_Yield_Other_Core(ತಳ_Type_t coreid);

#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_MUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

/**
 * @brief Get the current core's ID
 *
 * @note dummy function for freertos simulator, always returns 0.
 @ return BaseType_t 0
 */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ತಳ_Type_t x_ಪೋರ್ಟ್_Get_Core_ID(ಶೂನ್ಯ)
{
    ಹಿಂತಿರುಗಿ (ತಳ_Type_t) 0;
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
bool x_ಪೋರ್ಟ್_Check_Valid_List_Mem(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's TCB
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a TCB
 * @return false Otherwise
 */
bool x_ಪೋರ್ಟ್_Check_Valid_TCBMem(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's stack
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a task stack
 * @return false Otherwise
 */
bool x_Portcheck_Valid_ರಾಶಿ_Mem(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *ptr);

#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_VALID_LIST_MEM(ptr)     x_ಪೋರ್ಟ್_Check_Valid_List_Mem(ptr)
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_VALID_TCB_MEM(ptr)      x_ಪೋರ್ಟ್_Check_Valid_TCBMem(ptr)
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_VALID_ರಾಶಿ_MEM(ptr)    x_Portcheck_Valid_ರಾಶಿ_Mem(ptr)

ತಳ_Type_t x_ಪೋರ್ಟ್_Check_If_ಒಳ_ISR(ಶೂನ್ಯ);

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
ಸ್ಥಿರ ಸಾಲೊಳಗೆ ತಳ_Type_t x_ಪೋರ್ಟ್_ಒಳ_Isr_Context(ಶೂನ್ಯ)
{
    //Just call the FreeRTOS port interface version
    ಹಿಂತಿರುಗಿ x_ಪೋರ್ಟ್_Check_If_ಒಳ_ISR();
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ v_ಪೋರ್ಟ್_Assert_If_ಒಳ_ISR(ಶೂನ್ಯ)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(x_ಪೋರ್ಟ್_ಒಳ_Isr_Context() == 0);
}

/**
 * @brief Assert if in ISR context
 */
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ASSERT_IF_ಒಳ_ISR() v_ಪೋರ್ಟ್_Assert_If_ಒಳ_ISR()

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
}
#ಆದರೆ_ಅಂತ್ಯ
