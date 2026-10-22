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

#هدايت once

#ضمن <stdint.h>
#ضمن <stdlib.h> // This is for malloc(), used by portmacro.h
#ضمن "sdkconfig.h"
#ضمن "esp_attr.h"
#ضمن "esp_task.h"
#ضمن "spinlock.h"

#جيڪڏهن_وصف_ٿيل __cplusplus
خارجي "C" {
#جيڪڏهن_پورو

تعريفنوع spinlock_t پورٽ_MUX_TYPE;

/**< Spinlock initializer */
#وصف_ڪريو پورٽ_MUX_INITIALIZER_UNLOCKED {                      \
            .owner = پورٽ_MUX_FREE_VAL,                      \
            .ڳڻپ = 0,                                     \
        }
#وصف_ڪريو پورٽ_MUX_FREE_VAL                    SPINLOCK_FREE           /**< Spinlock is free. [refactor-todo] check if this is still required */

فراغ v_پورٽ_Yield_From_ISR(فراغ);
فراغ v_پورٽ_Yield_Other_Core(بنياد_Type_t coreid);

#وصف_ڪريو پورٽ_MUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

/**
 * @brief Get the current core's ID
 *
 * @note dummy function for freertos simulator, always returns 0.
 @ return BaseType_t 0
 */
ساكن مضمن بنياد_Type_t x_پورٽ_Get_Core_ID(فراغ)
{
    أرجع (بنياد_Type_t) 0;
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
bool x_پورٽ_Check_Valid_List_Mem(ثابت فراغ *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's TCB
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a TCB
 * @return false Otherwise
 */
bool x_پورٽ_Check_Valid_TCBMem(ثابت فراغ *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's stack
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a task stack
 * @return false Otherwise
 */
bool x_Portcheck_Valid_ڍير_Mem(ثابت فراغ *ptr);

#وصف_ڪريو پورٽ_VALID_LIST_MEM(ptr)     x_پورٽ_Check_Valid_List_Mem(ptr)
#وصف_ڪريو پورٽ_VALID_TCB_MEM(ptr)      x_پورٽ_Check_Valid_TCBMem(ptr)
#وصف_ڪريو پورٽ_VALID_ڍير_MEM(ptr)    x_Portcheck_Valid_ڍير_Mem(ptr)

بنياد_Type_t x_پورٽ_Check_If_اندريون_ISR(فراغ);

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
ساكن مضمن بنياد_Type_t x_پورٽ_اندريون_Isr_Context(فراغ)
{
    //Just call the FreeRTOS port interface version
    أرجع x_پورٽ_Check_If_اندريون_ISR();
}

ساكن مضمن فراغ v_پورٽ_Assert_If_اندريون_ISR(فراغ)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(x_پورٽ_اندريون_Isr_Context() == 0);
}

/**
 * @brief Assert if in ISR context
 */
#وصف_ڪريو پورٽ_ASSERT_IF_اندريون_ISR() v_پورٽ_Assert_If_اندريون_ISR()

#جيڪڏهن_وصف_ٿيل __cplusplus
}
#جيڪڏهن_پورو
