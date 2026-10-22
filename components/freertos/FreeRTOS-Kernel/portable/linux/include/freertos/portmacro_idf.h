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

#رهنمود once

#ضمن <stdint.h>
#ضمن <stdlib.h> // This is for malloc(), used by portmacro.h
#ضمن "sdkconfig.h"
#ضمن "esp_attr.h"
#ضمن "esp_task.h"
#ضمن "spinlock.h"

#اگر_تعریف_شده __cplusplus
خارجي "C" {
#پایان_اگر

تعريفنوع spinlock_t پورت_MUX_TYPE;

/**< Spinlock initializer */
#تعریف_کن پورت_MUX_INITIALIZER_UNLOCKED {                      \
            .owner = پورت_MUX_FREE_VAL,                      \
            .شمار = 0,                                     \
        }
#تعریف_کن پورت_MUX_FREE_VAL                    SPINLOCK_FREE           /**< Spinlock is free. [refactor-todo] check if this is still required */

فراغ v_پورت_Yield_From_ISR(فراغ);
فراغ v_پورت_Yield_Other_Core(پایه_Type_t coreid);

#تعریف_کن پورت_MUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

/**
 * @brief Get the current core's ID
 *
 * @note dummy function for freertos simulator, always returns 0.
 @ return BaseType_t 0
 */
ساكن مضمن پایه_Type_t x_پورت_Get_Core_ID(فراغ)
{
    أرجع (پایه_Type_t) 0;
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
bool x_پورت_Check_Valid_List_Mem(ثابت فراغ *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's TCB
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a TCB
 * @return false Otherwise
 */
bool x_پورت_Check_Valid_TCBMem(ثابت فراغ *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's stack
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a task stack
 * @return false Otherwise
 */
bool x_Portcheck_Valid_پشته_Mem(ثابت فراغ *ptr);

#تعریف_کن پورت_VALID_LIST_MEM(ptr)     x_پورت_Check_Valid_List_Mem(ptr)
#تعریف_کن پورت_VALID_TCB_MEM(ptr)      x_پورت_Check_Valid_TCBMem(ptr)
#تعریف_کن پورت_VALID_پشته_MEM(ptr)    x_Portcheck_Valid_پشته_Mem(ptr)

پایه_Type_t x_پورت_Check_If_درون_ISR(فراغ);

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
ساكن مضمن پایه_Type_t x_پورت_درون_Isr_Context(فراغ)
{
    //Just call the FreeRTOS port interface version
    أرجع x_پورت_Check_If_درون_ISR();
}

ساكن مضمن فراغ v_پورت_Assert_If_درون_ISR(فراغ)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(x_پورت_درون_Isr_Context() == 0);
}

/**
 * @brief Assert if in ISR context
 */
#تعریف_کن پورت_ASSERT_IF_درون_ISR() v_پورت_Assert_If_درون_ISR()

#اگر_تعریف_شده __cplusplus
}
#پایان_اگر
