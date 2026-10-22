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

#निर्देशः once

#अन्तर्भाव <stdint.h>
#अन्तर्भाव <stdlib.h> // This is for malloc(), used by portmacro.h
#अन्तर्भाव "sdkconfig.h"
#अन्तर्भाव "esp_attr.h"
#अन्तर्भाव "esp_task.h"
#अन्तर्भाव "spinlock.h"

#यदि_परिभाषितम् __cplusplus
बाह्य "C" {
#यदि_अन्तः

प्रकारनिर्देश spinlock_t पत्तनम्_MUX_TYPE;

/**< Spinlock initializer */
#परिभाषय पत्तनम्_MUX_INITIALIZER_UNLOCKED {                      \
            .owner = पत्तनम्_MUX_FREE_VAL,                      \
            .गणनम् = 0,                                     \
        }
#परिभाषय पत्तनम्_MUX_FREE_VAL                    SPINLOCK_FREE           /**< Spinlock is free. [refactor-todo] check if this is still required */

शून्य v_पत्तनम्_Yield_From_ISR(शून्य);
शून्य v_पत्तनम्_Yield_Other_Core(आधारः_Type_t coreid);

#परिभाषय पत्तनम्_MUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

/**
 * @brief Get the current core's ID
 *
 * @note dummy function for freertos simulator, always returns 0.
 @ return BaseType_t 0
 */
स्थिर अन्तःस्थ आधारः_Type_t x_पत्तनम्_Get_Core_ID(शून्य)
{
    प्रत्यागम (आधारः_Type_t) 0;
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
bool x_पत्तनम्_Check_Valid_List_Mem(ध्रुव शून्य *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's TCB
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a TCB
 * @return false Otherwise
 */
bool x_पत्तनम्_Check_Valid_TCBMem(ध्रुव शून्य *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's stack
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a task stack
 * @return false Otherwise
 */
bool x_Portcheck_Valid_राशिः_Mem(ध्रुव शून्य *ptr);

#परिभाषय पत्तनम्_VALID_LIST_MEM(ptr)     x_पत्तनम्_Check_Valid_List_Mem(ptr)
#परिभाषय पत्तनम्_VALID_TCB_MEM(ptr)      x_पत्तनम्_Check_Valid_TCBMem(ptr)
#परिभाषय पत्तनम्_VALID_राशिः_MEM(ptr)    x_Portcheck_Valid_राशिः_Mem(ptr)

आधारः_Type_t x_पत्तनम्_Check_If_अन्तर्_ISR(शून्य);

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
स्थिर अन्तःस्थ आधारः_Type_t x_पत्तनम्_अन्तर्_Isr_Context(शून्य)
{
    //Just call the FreeRTOS port interface version
    प्रत्यागम x_पत्तनम्_Check_If_अन्तर्_ISR();
}

स्थिर अन्तःस्थ शून्य v_पत्तनम्_Assert_If_अन्तर्_ISR(शून्य)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(x_पत्तनम्_अन्तर्_Isr_Context() == 0);
}

/**
 * @brief Assert if in ISR context
 */
#परिभाषय पत्तनम्_ASSERT_IF_अन्तर्_ISR() v_पत्तनम्_Assert_If_अन्तर्_ISR()

#यदि_परिभाषितम् __cplusplus
}
#यदि_अन्तः
