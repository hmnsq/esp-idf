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

#ہدایت once

#شامل <stdint.h>
#شامل <stdlib.h> // This is for malloc(), used by portmacro.h
#شامل "sdkconfig.h"
#شامل "esp_attr.h"
#شامل "esp_task.h"
#شامل "spinlock.h"

#اگر_تعریف_شدہ __cplusplus
خارجی "C" {
#اگر_ختم

قسمتعریف spinlock_t پورٹ_MUX_TYPE;

/**< Spinlock initializer */
#تعریف_کرو پورٹ_MUX_INITIALIZER_UNLOCKED {                      \
            .owner = پورٹ_MUX_FREE_VAL,                      \
            .گنتی = 0,                                     \
        }
#تعریف_کرو پورٹ_MUX_FREE_VAL                    SPINLOCK_FREE           /**< Spinlock is free. [refactor-todo] check if this is still required */

خالی v_پورٹ_Yield_From_ISR(خالی);
خالی v_پورٹ_Yield_Other_Core(بنیاد_Type_t coreid);

#تعریف_کرو پورٹ_MUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

/**
 * @brief Get the current core's ID
 *
 * @note dummy function for freertos simulator, always returns 0.
 @ return BaseType_t 0
 */
ساکن اندرونی بنیاد_Type_t x_پورٹ_Get_Core_ID(خالی)
{
    واپس (بنیاد_Type_t) 0;
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
bool x_پورٹ_Check_Valid_List_Mem(ثابت خالی *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's TCB
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a TCB
 * @return false Otherwise
 */
bool x_پورٹ_Check_Valid_TCBMem(ثابت خالی *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's stack
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a task stack
 * @return false Otherwise
 */
bool x_Portcheck_Valid_ڈھیر_Mem(ثابت خالی *ptr);

#تعریف_کرو پورٹ_VALID_LIST_MEM(ptr)     x_پورٹ_Check_Valid_List_Mem(ptr)
#تعریف_کرو پورٹ_VALID_TCB_MEM(ptr)      x_پورٹ_Check_Valid_TCBMem(ptr)
#تعریف_کرو پورٹ_VALID_ڈھیر_MEM(ptr)    x_Portcheck_Valid_ڈھیر_Mem(ptr)

بنیاد_Type_t x_پورٹ_Check_If_اندر_ISR(خالی);

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
ساکن اندرونی بنیاد_Type_t x_پورٹ_اندر_Isr_Context(خالی)
{
    //Just call the FreeRTOS port interface version
    واپس x_پورٹ_Check_If_اندر_ISR();
}

ساکن اندرونی خالی v_پورٹ_Assert_If_اندر_ISR(خالی)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(x_پورٹ_اندر_Isr_Context() == 0);
}

/**
 * @brief Assert if in ISR context
 */
#تعریف_کرو پورٹ_ASSERT_IF_اندر_ISR() v_پورٹ_Assert_If_اندر_ISR()

#اگر_تعریف_شدہ __cplusplus
}
#اگر_ختم
