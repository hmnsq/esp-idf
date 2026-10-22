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

#సూచన once

#అన్తర్భావ <stdint.h>
#అన్తర్భావ <stdlib.h> // This is for malloc(), used by portmacro.h
#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ "esp_attr.h"
#అన్తర్భావ "esp_task.h"
#అన్తర్భావ "spinlock.h"

#నిర్వచితమైతే __cplusplus
బాహ్య "C" {
#అయితే_ముగింపు

రకనిర్వచనం spinlock_t పోర్టు_MUX_TYPE;

/**< Spinlock initializer */
#నిర్వచించు పోర్టు_MUX_INITIALIZER_UNLOCKED {                      \
            .owner = పోర్టు_MUX_FREE_VAL,                      \
            .లెక్క = 0,                                     \
        }
#నిర్వచించు పోర్టు_MUX_FREE_VAL                    SPINLOCK_FREE           /**< Spinlock is free. [refactor-todo] check if this is still required */

శూన్య v_పోర్టు_Yield_From_ISR(శూన్య);
శూన్య v_పోర్టు_Yield_Other_Core(పునాది_Type_t coreid);

#నిర్వచించు పోర్టు_MUX_INITIALIZE(mux)             spinlock_initialize(mux)    /*< Initialize a spinlock to its unlocked state */

/**
 * @brief Get the current core's ID
 *
 * @note dummy function for freertos simulator, always returns 0.
 @ return BaseType_t 0
 */
స్థిర అంతఃవరుస పునాది_Type_t x_పోర్టు_Get_Core_ID(శూన్య)
{
    తిరిగి (పునాది_Type_t) 0;
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
bool x_పోర్టు_Check_Valid_List_Mem(స్థిరరాశి శూన్య *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's TCB
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a TCB
 * @return false Otherwise
 */
bool x_పోర్టు_Check_Valid_TCBMem(స్థిరరాశి శూన్య *ptr);

/**
 * @brief Checks if a given piece of memory can be used to store a task's stack
 *
 * - Defined in heap_idf.c
 *
 * @param ptr Pointer to memory
 * @return true Memory can be used to store a task stack
 * @return false Otherwise
 */
bool x_Portcheck_Valid_దొంతర_Mem(స్థిరరాశి శూన్య *ptr);

#నిర్వచించు పోర్టు_VALID_LIST_MEM(ptr)     x_పోర్టు_Check_Valid_List_Mem(ptr)
#నిర్వచించు పోర్టు_VALID_TCB_MEM(ptr)      x_పోర్టు_Check_Valid_TCBMem(ptr)
#నిర్వచించు పోర్టు_VALID_దొంతర_MEM(ptr)    x_Portcheck_Valid_దొంతర_Mem(ptr)

పునాది_Type_t x_పోర్టు_Check_If_లోపల_ISR(శూన్య);

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
స్థిర అంతఃవరుస పునాది_Type_t x_పోర్టు_లోపల_Isr_Context(శూన్య)
{
    //Just call the FreeRTOS port interface version
    తిరిగి x_పోర్టు_Check_If_లోపల_ISR();
}

స్థిర అంతఃవరుస శూన్య v_పోర్టు_Assert_If_లోపల_ISR(శూన్య)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(x_పోర్టు_లోపల_Isr_Context() == 0);
}

/**
 * @brief Assert if in ISR context
 */
#నిర్వచించు పోర్టు_ASSERT_IF_లోపల_ISR() v_పోర్టు_Assert_If_లోపల_ISR()

#నిర్వచితమైతే __cplusplus
}
#అయితే_ముగింపు
