/*
 * FreeRTOS Kernel V10.5.1 (ESP-IDF SMP modified)
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-FileCopyrightText: 2021 Amazon.com, Inc. or its affiliates
 *
 * SPDX-License-Identifier: MIT
 *
 * SPDX-FileContributor: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/*-----------------------------------------------------------------------
 * Implementation of functions defined in portable.h for the RISC-V port.
 *----------------------------------------------------------------------*/

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ "soc/soc_caps.h"
#ಅನ್ತರ್ಭಾವ "soc/periph_defs.h"
#ಅನ್ತರ್ಭಾವ "soc/system_reg.h"
#ಅನ್ತರ್ಭಾವ "soc/interrupt_reg.h"
#ಅನ್ತರ್ಭಾವ "hal/systimer_hal.h"
#ಅನ್ತರ್ಭಾವ "hal/systimer_ll.h"
#ಅನ್ತರ್ಭಾವ "riscv/rvruntime-frames.h"
#ಅನ್ತರ್ಭಾವ "riscv/rv_utils.h"
#ಅನ್ತರ್ಭಾವ "riscv/interrupt.h"
#ಅನ್ತರ್ಭಾವ "esp_private/crosscore_int.h"
#ಅನ್ತರ್ಭಾವ "hal/crosscore_int_ll.h"
#ಅನ್ತರ್ಭಾವ "esp_attr.h"
#ಅನ್ತರ್ಭಾವ "esp_compiler.h"
#ಅನ್ತರ್ಭಾವ "esp_system.h"
#ಅನ್ತರ್ಭಾವ "esp_intr_alloc.h"
#ಅನ್ತರ್ಭಾವ "esp_log.h"
#ಅನ್ತರ್ಭಾವ "FreeRTOS.h"       /* This pulls in portmacro.h */
#ಅನ್ತರ್ಭಾವ "task.h"
#ಆದರೆ CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST
#ಅನ್ತರ್ಭಾವ "freertos/idf_additions.h"
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ "portmacro.h"
#ಅನ್ತರ್ಭಾವ "port_systick.h"
#ಅನ್ತರ್ಭಾವ "esp_memory_utils.h"
#ಅನ್ತರ್ಭಾವ "esp_macros.h"
#ಆದರೆ CONFIG_FREERTOS_ಸತತ_ಸಮಯ_STATS_USING_ESP_TIMER
#ಅನ್ತರ್ಭಾವ "esp_timer.h"
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ SOC_CPU_HAS_HWLOOP
#ಅನ್ತರ್ಭಾವ "riscv/csr.h"
#ಅನ್ತರ್ಭಾವ "riscv/csr_hwlp.h"
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ ( SOC_CPU_COPROC_ಅಂಕೆ > 0 )

#ಅನ್ತರ್ಭಾವ "esp_private/panic_internal.h"

/* Since `portFORCE_INLINE` is not defined in `portmacro.h`, we must define it here since it is
 * used by `atomic.h`. */
#ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_FORCE_INLINE    ಸಾಲೊಳಗೆ
#ಅನ್ತರ್ಭಾವ "freertos/atomic.h"

#ಆದರೆ_ಅಂತ್ಯ // ( SOC_CPU_COPROC_NUM > 0 )


_Static_assert(ಪೋರ್ಟ್_BYTE_ALIGNMENT == 16, "portBYTE_ALIGNMENT must be set to 16");
#ಆದರೆ CONFIG_ESP_SYSTEM_HW_ರಾಶಿ_GUARD
/**
 * offsetof() can not be used in asm code. Then we need make sure that
 * PORT_OFFSET_PX_STACK and PORT_OFFSET_PX_END_OF_STACK have expected values.
 * Macro used in the portasm.S instead of variables to save at least 4 instruction calls
 * which accessing DRAM memory. This optimization saves CPU time in the interrupt handling.
 */

_Static_assert(offsetof( StaticTask_t, pxDummy6 ) == ಪೋರ್ಟ್_ಸ್ಥಾನಾಂತರ_PX_ರಾಶಿ);
_Static_assert(offsetof( StaticTask_t, pxDummy8 ) == ಪೋರ್ಟ್_ಸ್ಥಾನಾಂತರ_PX_ಕೊನೆ_ದ_ರಾಶಿ);
#ಆದರೆ_ಅಂತ್ಯ // CONFIG_ESP_SYSTEM_HW_STACK_GUARD

/* ---------------------------------------------------- Variables ------------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */

ಅಸ್ಥಿರ UBaseType_t ಪೋರ್ಟ್_x_Scheduler_Running[ಪೋರ್ಟ್_ಅಂಕೆ_PROCESSORS] = {0}; // Indicates whether scheduler is running on a per-core basis
ಅಸ್ಥಿರ UBaseType_t ಪೋರ್ಟ್_ux_ಅಡಚಣೆ_Nesting[ಪೋರ್ಟ್_ಅಂಕೆ_PROCESSORS] = {0};  // Interrupt nesting level. Increased/decreased in portasm.c
ಅಸ್ಥಿರ UBaseType_t ಪೋರ್ಟ್_ux_Critical_Nesting[ಪೋರ್ಟ್_ಅಂಕೆ_PROCESSORS] = {0};
#ಆದರೆ CONFIG_FREERTOS_ಪೋರ್ಟ್_THREAD_ಸುರಕ್ಷಿತ_CLAIM
ಅಸ್ಥಿರ bool ಪೋರ್ಟ್_x_Thread_ಸುರಕ್ಷಿತ_Claimed = false;
#ಆದರೆ_ಅಂತ್ಯ
ಅಸ್ಥಿರ UBaseType_t ಪೋರ್ಟ್_ux_Old_ಅಡಚಣೆ_State[ಪೋರ್ಟ್_ಅಂಕೆ_PROCESSORS] = {0};
ಅಸ್ಥಿರ UBaseType_t x_ಪೋರ್ಟ್_Switch_Flag[ಪೋರ್ಟ್_ಅಂಕೆ_PROCESSORS] = {0};
ಅಸ್ಥಿರ UBaseType_t ಪೋರ್ಟ್_ux_Core_Startup_ಮುಗಿದ[ಪೋರ್ಟ್_ಅಂಕೆ_PROCESSORS] = {0};  // Indicates whether the core has completed its startup sequence

#ಆದರೆ ( SOC_CPU_COPROC_ಅಂಕೆ > 0 )

/* Current owner of the coprocessors for each core */
StaticTask_t* ಪೋರ್ಟ್_ux_Coproc_Owner[ಪೋರ್ಟ್_ಅಂಕೆ_PROCESSORS][SOC_CPU_COPROC_ಅಂಕೆ];

#ಆದರೆ_ಅಂತ್ಯ /* SOC_CPU_COPROC_NUM > 0 */

/*
*******************************************************************************
* Interrupt stack. The size of the interrupt stack is determined by the config
* parameter "configISR_STACK_SIZE" in FreeRTOSConfig.h
*******************************************************************************
*/
__attribute__((aligned(16))) ರಾಶಿ_Type_t x_Isr_ರಾಶಿ[ಪೋರ್ಟ್_ಅಂಕೆ_PROCESSORS][config_ISR_ರಾಶಿ_ಗಾತ್ರ];
ರಾಶಿ_Type_t *x_Isr_ರಾಶಿ_ತುದಿ[ಪೋರ್ಟ್_ಅಂಕೆ_PROCESSORS] = {0};

#ಆದರೆ CONFIG_ESP_SYSTEM_HW_ರಾಶಿ_GUARD
ರಾಶಿ_Type_t *x_Isr_ರಾಶಿ_Bottom[ಪೋರ್ಟ್_ಅಂಕೆ_PROCESSORS] = {0};
#ಆದರೆ_ಅಂತ್ಯ

/* ------------------------------------------------ FreeRTOS Portable --------------------------------------------------
 * - Provides implementation for functions required by FreeRTOS
 * - Declared in portable.h
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------- Scheduler Start/End -------------------

ತಳ_Type_t x_ಪೋರ್ಟ್_ಆರಂಭ_Scheduler(ಶೂನ್ಯ)
{
#ಆದರೆ ( SOC_CPU_COPROC_ಅಂಕೆ > 0 )

#ಆದರೆ SOC_CPU_HAS_FPU
    /* Disable FPU so that the first task to use it will trigger an exception */
    rv_utils_disable_fpu();
#ಆದರೆ_ಅಂತ್ಯ /* SOC_CPU_HAS_FPU */

#ಆದರೆ SOC_CPU_HAS_PIE
    /* Similarly, disable PIE */
    rv_utils_disable_pie();
#ಆದರೆ_ಅಂತ್ಯ /* SOC_CPU_HAS_PIE */

#ಆದರೆ SOC_CPU_HAS_DSP
    rv_utils_disable_dsp();
#ಆದರೆ_ಅಂತ್ಯ /* SOC_CPU_HAS_DSP */

#ಆದರೆ SOC_CPU_HAS_HWLOOP
    /* Initialize the Hardware loop feature */
    RV_WRITE_CSR(CSR_HWLP_STATE_REG, HWLP_INITIAL_STATE);
#ಆದರೆ_ಅಂತ್ಯ /* SOC_CPU_HAS_HWLOOP */
#ಆದರೆ_ಅಂತ್ಯ /* ( SOC_CPU_COPROC_NUM > 0 ) */

    /* Initialize all kernel state tracking variables */
    ತಳ_Type_t coreID = x_ಪೋರ್ಟ್_Get_Core_ID();
    ಪೋರ್ಟ್_ux_ಅಡಚಣೆ_Nesting[coreID] = 0;
    ಪೋರ್ಟ್_ux_Critical_Nesting[coreID] = 0;
#ಆದರೆ CONFIG_FREERTOS_ಪೋರ್ಟ್_THREAD_ಸುರಕ್ಷಿತ_CLAIM
    ಪೋರ್ಟ್_x_Thread_ಸುರಕ್ಷಿತ_Claimed = false;
#ಆದರೆ_ಅಂತ್ಯ
    ಪೋರ್ಟ್_x_Scheduler_Running[coreID] = 0;
    ಪೋರ್ಟ್_ux_Core_Startup_ಮುಗಿದ[coreID] = 0;

    /* Initialize ISR Stack(s) */
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಪೋರ್ಟ್_ಅಂಕೆ_PROCESSORS; i++) {
        x_Isr_ರಾಶಿ_ತುದಿ[i] = &x_Isr_ರಾಶಿ[i][0] + (config_ISR_ರಾಶಿ_ಗಾತ್ರ & (~((ಪೋರ್ಟ್_ಸೂಚಕ_ಗಾತ್ರ_TYPE)ಪೋರ್ಟ್_BYTE_ALIGNMENT_ಮುಖವಾಡ)));
#ಆದರೆ CONFIG_ESP_SYSTEM_HW_ರಾಶಿ_GUARD
        x_Isr_ರಾಶಿ_Bottom[i] = &x_Isr_ರಾಶಿ[i][0];
#ಆದರೆ_ಅಂತ್ಯ
    }

    /* Setup the hardware to generate the tick. */
    v_ಪೋರ್ಟ್_ಸಿದ್ಧತೆ_Timer();

    rv_utils_intr_global_enable();
    esprv_int_ಗಣ_ಮಿತಿ(RVHAL_INTR_ENABLE_THRESH); /* set global interrupt masking level */

    v_ಪೋರ್ಟ್_Yield();

    /* Should not get here */
    ಹಿಂತಿರುಗಿ pdTRUE;
}

ಶೂನ್ಯ v_ಪೋರ್ಟ್_ಕೊನೆ_Scheduler(ಶೂನ್ಯ)
{
    /* very unlikely this function will be called, so just trap here */
    abort();
}

// ------------------------ Stack --------------------------



/**
 * @brief Allocate and initialize GCC TLS area
 *
 * This function allocates and initializes the area on the stack used to store GCC TLS (Thread Local Storage) variables.
 * - The area's size is derived from the TLS section's linker variables, and rounded up to a multiple of 16 bytes
 * - The allocated area is aligned to a 16-byte aligned address
 * - The TLS variables in the area are then initialized
 *
 * Each task access the TLS variables using the THREADPTR register plus an offset to obtain the address of the variable.
 * The value for the THREADPTR register is also calculated by this function, and that value should be use to initialize
 * the THREADPTR register.
 *
 * @param[in] uxStackPointer Current stack pointer address
 * @param[out] ret_threadptr_reg_init Calculated THREADPTR register initialization value
 * @return Stack pointer that points to the TLS area
 */
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_ರಾಶಿ_TLS(UBaseType_t ux_ರಾಶಿ_ಸೂಚಕ, uint32_t *ret_threadptr_reg_init)
{
    /*
    LOW ADDRESS
            |---------------------------|   Linker Symbols
            | Section                   |   --------------
            | .flash.tdata              |
         0x0|---------------------------| <- _thread_local_data_start  ^
            | .flash.tdata              |                              |
            | int var_1 = 1;            |                              |
            |                           | <- _thread_local_data_end    |
            |                           | <- _thread_local_bss_start   | tls_area_size
            |                           |                              |
            | .flash.tbss (NOLOAD)      |                              |
            | int var_2;                |                              |
            |---------------------------| <- _thread_local_bss_end     V
            | Other data                |
            | ...                       |
            |---------------------------|
    HIGH ADDRESS
    */
    // Calculate TLS area size and round up to multiple of 16 bytes.
    ಬಾಹ್ಯ ಅಕ್ಷರ thread_local_ದತ್ತಾಂಶ_ಆರಂಭ, thread_local_ದತ್ತಾಂಶ_ಕೊನೆ;
    ಬಾಹ್ಯ ಅಕ್ಷರ thread_local_bss_ಆರಂಭ, thread_local_bss_ಕೊನೆ;
    ಸ್ಥಿರಾಂಕ uint32_t tls_ದತ್ತಾಂಶ_ಗಾತ್ರ = (uint32_t)&thread_local_ದತ್ತಾಂಶ_ಕೊನೆ - (uint32_t)&thread_local_ದತ್ತಾಂಶ_ಆರಂಭ;
    ಸ್ಥಿರಾಂಕ uint32_t tls_bss_ಗಾತ್ರ = (uint32_t)&thread_local_bss_ಕೊನೆ - (uint32_t)&thread_local_bss_ಆರಂಭ;
    ಸ್ಥಿರಾಂಕ uint32_t tls_area_ಗಾತ್ರ = ESP_ALIGN_ಮೇಲೆ(tls_ದತ್ತಾಂಶ_ಗಾತ್ರ + tls_bss_ಗಾತ್ರ, 16);
    // TODO: check that TLS area fits the stack

    // Allocate space for the TLS area on the stack. The area must be aligned to 16-bytes
    ux_ರಾಶಿ_ಸೂಚಕ = ESP_ALIGN_ಕೆಳಕ್ಕೆ(ux_ರಾಶಿ_ಸೂಚಕ - (UBaseType_t)tls_area_ಗಾತ್ರ, 16);
    // Initialize the TLS data with the initialization values of each TLS variable
    ಸ್ಮರಣೆ_ನಕಲು((ಶೂನ್ಯ *)ux_ರಾಶಿ_ಸೂಚಕ, &thread_local_ದತ್ತಾಂಶ_ಆರಂಭ, tls_ದತ್ತಾಂಶ_ಗಾತ್ರ);
    // Initialize the TLS bss with zeroes
    memset((ಶೂನ್ಯ *)(ux_ರಾಶಿ_ಸೂಚಕ + tls_ದತ್ತಾಂಶ_ಗಾತ್ರ), 0, tls_bss_ಗಾತ್ರ);

    // Save tls start address
    *ret_threadptr_reg_init = (uint32_t)ux_ರಾಶಿ_ಸೂಚಕ;
    ಹಿಂತಿರುಗಿ ux_ರಾಶಿ_ಸೂಚಕ;
}

#ಆದರೆ CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
ಸ್ಥಿರ ಶೂನ್ಯ v_ಪೋರ್ಟ್_Task_Wrapper(TaskFunction_t px_ಸಂಕೇತ, ಶೂನ್ಯ *pvParameters)
{
    __asm__ ಅಸ್ಥಿರ(".cfi_undefined ra");  // tell to debugger that it's outermost (initial) frame
    ಬಾಹ್ಯ ಶೂನ್ಯ __attribute__((noreturn)) panic_abort(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *details);
    ಸ್ಥಿರ ಅಕ್ಷರ DRAM_ATTR ಸಂದೇಶ[80] = "FreeRTOS: FreeRTOS Task \"\0";
    px_ಸಂಕೇತ(pvParameters);
    /* FreeRTOS tasks should not return. Log the task name and abort. */
    /* We cannot use s(n)printf because it is in flash */
    strcat(ಸಂದೇಶ, pcTaskGetName(ಖಾಲಿ));
    strcat(ಸಂದೇಶ, "\" should not return, Aborting now!");
    panic_abort(ಸಂದೇಶ);
}
#ಆದರೆ_ಅಂತ್ಯ // CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER


#ಆದರೆ ( SOC_CPU_COPROC_ಅಂಕೆ > 0 )

/**
 * @brief Retrieve or allocate coprocessors save area from the given pxTopOfStack address.
 *
 * @param pxTopOfStack End of the stack address. This represents the highest address of a Task's stack.
 */
FORCE_INLINE_ATTR RvCoprocSaveArea* px_Retrieve_Coproc_Save_Area_From_ರಾಶಿ_ಸೂಚಕ(UBaseType_t px_ತುದಿ_ದ_ರಾಶಿ)
{
    ಹಿಂತಿರುಗಿ (RvCoprocSaveArea*) ESP_ALIGN_ಕೆಳಕ್ಕೆ(px_ತುದಿ_ದ_ರಾಶಿ - ಗಾತ್ರ(RvCoprocSaveArea), 16);
}

/**
 * @brief Allocate and initialize the coprocessors save area on the stack
 *
 * @param[in] uxStackPointer Current stack pointer address
 *
 * @return Stack pointer that points to allocated and initialized the coprocessor save area
 */
FORCE_INLINE_ATTR UBaseType_t uxInitialiseCoprocSaveArea(UBaseType_t ux_ರಾಶಿ_ಸೂಚಕ)
{
    RvCoprocSaveArea* sa = px_Retrieve_Coproc_Save_Area_From_ರಾಶಿ_ಸೂಚಕ(ux_ರಾಶಿ_ಸೂಚಕ);
    memset(sa, 0, ಗಾತ್ರ(RvCoprocSaveArea));
    ಹಿಂತಿರುಗಿ (UBaseType_t) sa;
}


ಸ್ಥಿರ ಶೂನ್ಯ v_ಪೋರ್ಟ್_Clean_ಮೇಲೆ_Coproc_Area(ಶೂನ್ಯ *pvTCB)
{
    StaticTask_t* task = (StaticTask_t*) pvTCB;

    /* Get a pointer to the task's coprocessor save area */
    ಸ್ಥಿರಾಂಕ UBaseType_t bottomstack = (UBaseType_t) task->pxDummy8;
    RvCoprocSaveArea* sa = px_Retrieve_Coproc_Save_Area_From_ರಾಶಿ_ಸೂಚಕ(bottomstack);

    /* If the Task ever saved the original stack pointer, restore it before returning */
    ಆದರೆ (sa->sa_allocator != 0) {
        /* Restore the original lowest address of the stack in the TCB */
        task->pxDummy6 = sa->sa_tcbstack;

        /* Get the core the task is pinned on */
        #ಆದರೆ ( config_ಅಂಕೆ_CORES > 1 )
            ಸ್ಥಿರಾಂಕ ತಳ_Type_t coreID = task->xDummyCoreID;
            /* If the task is not pinned on any core, it didn't use any coprocessor than need to be freed (FPU or PIE).
             * If it used the HWLP coprocessor, it has nothing to clear since there is no "owner" for it. */
            ಆದರೆ (coreID == tskNO_AFFINITY) {
                ಹಿಂತಿರುಗಿ;
            }
        #ಇಲ್ಲದಿದ್ದರೆ /* configNUM_CORES > 1 */
            ಸ್ಥಿರಾಂಕ ತಳ_Type_t coreID = 0;
        #ಆದರೆ_ಅಂತ್ಯ /* configNUM_CORES > 1 */

        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < SOC_CPU_COPROC_ಅಂಕೆ; i++) {
            StaticTask_t** owner = &ಪೋರ್ಟ್_ux_Coproc_Owner[coreID][i];
            /* If the owner is `task`, replace it with NULL atomically */
            Atomic_Compare_And_ಅದಲುಬದಲು_ಸೂಚಕಗಳು_p32((ಶೂನ್ಯ**) owner, ಖಾಲಿ, task);
        }
    }
}
#ಆದರೆ_ಅಂತ್ಯ /* SOC_CPU_COPROC_NUM > 0 */


/**
 * @brief Initialize the task's starting interrupt stack frame
 *
 * This function initializes the task's starting interrupt stack frame. The dispatcher will use this stack frame in a
 * context restore routine. Therefore, the starting stack frame must be initialized as if the task was interrupted right
 * before its first instruction is called.
 *
 * - The stack frame is allocated to a 16-byte aligned address
 *
 * @param[in] uxStackPointer Current stack pointer address
 * @param[in] pxCode Task function
 * @param[in] pvParameters Task function's parameter
 * @param[in] threadptr_reg_init THREADPTR register initialization value
 * @return Stack pointer that points to the stack frame
 */
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_ರಾಶಿ_ಚೌಕಟ್ಟು(UBaseType_t ux_ರಾಶಿ_ಸೂಚಕ, TaskFunction_t px_ಸಂಕೇತ, ಶೂನ್ಯ *pvParameters, uint32_t threadptr_reg_init)
{
    /*
    Allocate space for the task's starting interrupt stack frame.
    - The stack frame must be allocated to a 16-byte aligned address.
    - We use RV_STK_FRMSZ as it rounds up the total size to a multiple of 16.
    */
    ux_ರಾಶಿ_ಸೂಚಕ = ESP_ALIGN_ಕೆಳಕ್ಕೆ(ux_ರಾಶಿ_ಸೂಚಕ - RV_STK_FRMSZ, 16);

    // Clear the entire interrupt stack frame
    Rv_Exc_ಚೌಕಟ್ಟು *ಚೌಕಟ್ಟು = (Rv_Exc_ಚೌಕಟ್ಟು *)ux_ರಾಶಿ_ಸೂಚಕ;
    memset(ಚೌಕಟ್ಟು, 0, ಗಾತ್ರ(Rv_Exc_ಚೌಕಟ್ಟು));

    /* Initialize the stack frame. */
    ಬಾಹ್ಯ uint32_t __global_pointer$;
    #ಆದರೆ CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
        ಚೌಕಟ್ಟು->mepc = (UBaseType_t)v_ಪೋರ್ಟ್_Task_Wrapper;
        ಚೌಕಟ್ಟು->a0 = (UBaseType_t)px_ಸಂಕೇತ;
        ಚೌಕಟ್ಟು->a1 = (UBaseType_t)pvParameters;
    #ಇಲ್ಲದಿದ್ದರೆ
        ಚೌಕಟ್ಟು->mepc = (UBaseType_t)px_ಸಂಕೇತ;
        ಚೌಕಟ್ಟು->a0 = (UBaseType_t)pvParameters;
    #ಆದರೆ_ಅಂತ್ಯ // CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
    ಚೌಕಟ್ಟು->gp = (UBaseType_t)&__global_pointer$;
    ಚೌಕಟ್ಟು->tp = (UBaseType_t)threadptr_reg_init;

    ಹಿಂತಿರುಗಿ ux_ರಾಶಿ_ಸೂಚಕ;
}

ರಾಶಿ_Type_t *px_ಪೋರ್ಟ್_Initialise_ರಾಶಿ(ರಾಶಿ_Type_t *px_ತುದಿ_ದ_ರಾಶಿ, TaskFunction_t px_ಸಂಕೇತ, ಶೂನ್ಯ *pvParameters)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __clang_analyzer__
    // Teach clang-tidy that pxTopOfStack cannot be a pointer to const
    ಅಸ್ಥಿರ ರಾಶಿ_Type_t * px_ತಾತ್ಕಾಲಿಕ = px_ತುದಿ_ದ_ರಾಶಿ;
    px_ತುದಿ_ದ_ರಾಶಿ = px_ತಾತ್ಕಾಲಿಕ;
#ಆದರೆ_ಅಂತ್ಯ /*__clang_analyzer__ */
    /*
    HIGH ADDRESS
    |---------------------------| <- pxTopOfStack on entry
    | TLS Variables             |
    | ------------------------- | <- Start of usable stack
    | Starting stack frame      |
    | ------------------------- | <- pxTopOfStack on return (which is the tasks current SP)
    |             |             |
    |             |             |
    |             V             |
    ----------------------------- <- Bottom of stack
    LOW ADDRESS

    - All stack areas are aligned to 16 byte boundary
    - We use UBaseType_t for all of stack area initialization functions for more convenient pointer arithmetic

    In the case of targets that have coprocessors, the stack is presented as follows:
    HIGH ADDRESS
    |---------------------------| <- pxTopOfStack on entry
    | Coproc. Save Area         | <- RvCoprocSaveArea
    | ------------------------- |
    | TLS Variables             |
    | ------------------------- | <- Start of usable stack
    | Starting stack frame      |
    | ------------------------- | <- pxTopOfStack on return (which is the tasks current SP)
    |             |             |
    |             |             |
    |             V             |
    |---------------------------|
    | Coproc. m Saved Context   | <- Coprocessor context save area after allocation
    |---------------------------|
    | Coproc. n Saved Context   | <- Another coprocessor context save area after allocation
    ----------------------------- <- Bottom of stack
    LOW ADDRESS

    Where m != n, n < SOC_CPU_COPROC_NUM, m < SOC_CPU_COPROC_NUM

    */

    UBaseType_t ux_ರಾಶಿ_ಸೂಚಕ = (UBaseType_t)px_ತುದಿ_ದ_ರಾಶಿ;
    configASSERT((ux_ರಾಶಿ_ಸೂಚಕ & ಪೋರ್ಟ್_BYTE_ALIGNMENT_ಮುಖವಾಡ) == 0);

#ಆದರೆ ( SOC_CPU_COPROC_ಅಂಕೆ > 0 )
    // Initialize the coprocessors save area
    ux_ರಾಶಿ_ಸೂಚಕ = uxInitialiseCoprocSaveArea(ux_ರಾಶಿ_ಸೂಚಕ);
    configASSERT((ux_ರಾಶಿ_ಸೂಚಕ & ಪೋರ್ಟ್_BYTE_ALIGNMENT_ಮುಖವಾಡ) == 0);
#ಆದರೆ_ಅಂತ್ಯ // SOC_CPU_COPROC_NUM > 0

    // Initialize GCC TLS area
    uint32_t threadptr_reg_init;
    ux_ರಾಶಿ_ಸೂಚಕ = ux_Initialise_ರಾಶಿ_TLS(ux_ರಾಶಿ_ಸೂಚಕ, &threadptr_reg_init);
    configASSERT((ux_ರಾಶಿ_ಸೂಚಕ & ಪೋರ್ಟ್_BYTE_ALIGNMENT_ಮುಖವಾಡ) == 0);

    // Initialize the starting interrupt stack frame
    ux_ರಾಶಿ_ಸೂಚಕ = ux_Initialise_ರಾಶಿ_ಚೌಕಟ್ಟು(ux_ರಾಶಿ_ಸೂಚಕ, px_ಸಂಕೇತ, pvParameters, threadptr_reg_init);
    configASSERT((ux_ರಾಶಿ_ಸೂಚಕ & ಪೋರ್ಟ್_BYTE_ALIGNMENT_ಮುಖವಾಡ) == 0);

    // Return the task's current stack pointer address which should point to the starting interrupt stack frame
    ಹಿಂತಿರುಗಿ (ರಾಶಿ_Type_t *)ux_ರಾಶಿ_ಸೂಚಕ;
}



/* ---------------------------------------------- Port Implementations -------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

ತಳ_Type_t x_ಪೋರ್ಟ್_ಒಳ_Isr_Context(ಶೂನ್ಯ)
{
#ಆದರೆ (config_ಅಂಕೆ_CORES > 1)
    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ irq_ಸ್ಥಿತಿ;
    ತಳ_Type_t ret;

    /* Disable interrupts to fetch the coreID atomically */
    irq_ಸ್ಥಿತಿ = ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR();

    /* Return the interrupt nesting counter for this core */
    ret = ಪೋರ್ಟ್_ux_ಅಡಚಣೆ_Nesting[x_ಪೋರ್ಟ್_Get_Core_ID()];

    /* Restore interrupts */
    ಪೋರ್ಟ್_CLEAR_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR(irq_ಸ್ಥಿತಿ);

    ಹಿಂತಿರುಗಿ ret;
#ಇಲ್ಲದಿದ್ದರೆ
    /* Optimize the call for single-core targets */
    ಹಿಂತಿರುಗಿ ಪೋರ್ಟ್_ux_ಅಡಚಣೆ_Nesting[0];
#ಆದರೆ_ಅಂತ್ಯ /* (configNUM_CORES > 1) */
}

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Assert_If_ಒಳ_ISR(ಶೂನ್ಯ)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(x_ಪೋರ್ಟ್_ಒಳ_Isr_Context() == 0);
}

ತಳ_Type_t x_ಪೋರ್ಟ್_Interrupted_From_ISRContext(ಶೂನ್ಯ)
{
    /* Return the interrupt nesting counter for this core */
    ಹಿಂತಿರುಗಿ ಪೋರ್ಟ್_ux_ಅಡಚಣೆ_Nesting[x_ಪೋರ್ಟ್_Get_Core_ID()];
}

UBaseType_t x_ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ_From_ISR(ಶೂನ್ಯ)
{
    UBaseType_t prev_int_level = 0, int_level = 0;
#ಆದರೆ !SOC_INT_CLIC_SUPPORTED
    int_level = RVHAL_EXCM_LEVEL;
#ಇಲ್ಲದಿದ್ದರೆ
    int_level = RVHAL_EXCM_LEVEL_CLIC;
#ಆದರೆ_ಅಂತ್ಯ

    prev_int_level = rv_utils_ಗಣ_intlevel_regval(int_level);
    ಹಿಂತಿರುಗಿ prev_int_level;
}

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Clear_ಅಡಚಣೆ_ಮುಖವಾಡ_From_ISR(UBaseType_t prev_int_level)
{
    rv_utils_restore_intlevel_regval(prev_int_level);
    /**
     * The delay between the moment we unmask the interrupt threshold register
     * and the moment the potential requested interrupt is triggered is not
     * null: up to three machine cycles/instructions can be executed.
     *
     * When compilation size optimization is enabled, this function and its
     * callers returning void will have NO epilogue, thus the instruction
     * following these calls will be executed.
     *
     * If the requested interrupt is a context switch to a higher priority
     * task then the one currently running, we MUST NOT execute any instruction
     * before the interrupt effectively happens.
     * In order to prevent this, force this routine to have a 3-instruction
     * delay before exiting.
     */
    asm ಅಸ್ಥಿರ ( "nop" );
    asm ಅಸ್ಥಿರ ( "nop" );
    asm ಅಸ್ಥಿರ ( "nop" );
}

// ------------------ Critical Sections --------------------

#ಆದರೆ (config_ಅಂಕೆ_CORES > 1)
ತಳ_Type_t __attribute__((optimize("-O3"))) x_ಪೋರ್ಟ್_Enter_Critical_Timeout(ಪೋರ್ಟ್_MUX_TYPE *mux, ತಳ_Type_t timeout)
{
#ಆದರೆ CONFIG_FREERTOS_ಪೋರ್ಟ್_THREAD_ಸುರಕ್ಷಿತ_CLAIM
    ಆದರೆ (unlikely(ಪೋರ್ಟ್_x_Thread_ಸುರಕ್ಷಿತ_Claimed)) {
        ಹಿಂತಿರುಗಿ pdPASS;
    }
#ಆದರೆ_ಅಂತ್ಯ
    /* Interrupts may already be disabled (if this function is called in nested
     * manner). However, there's no atomic operation that will allow us to check,
     * thus we have to disable interrupts again anyways.
     *
     * However, if this is call is NOT nested (i.e., the first call to enter a
     * critical section), we will save the previous interrupt level so that the
     * saved level can be restored on the last call to exit the critical.
     */
    ತಳ_Type_t x_Old_ಅಡಚಣೆ_Level = ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR();
    /* Interrupts are masked, so the core id is stable. Read it once and reuse it for
     * both the spinlock owner id and the per-core critical nesting state. Interrupts
     * stay masked for the whole critical section (to the same level the spinlock would
     * use), so the spinlock does not need to disable them again. */
    ತಳ_Type_t coreID = x_ಪೋರ್ಟ್_Get_Core_ID();
    ಆದರೆ (!spinlock_acquire_impl(mux, timeout, spinlock_owner_id_for_core(coreID))) {
        //Timed out attempting to get spinlock. Restore previous interrupt level and return
        ಪೋರ್ಟ್_CLEAR_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR(x_Old_ಅಡಚಣೆ_Level);
        ಹಿಂತಿರುಗಿ pdFAIL;
    }
    //Spinlock acquired. Increment the critical nesting count.
    ತಳ_Type_t newNesting = ಪೋರ್ಟ್_ux_Critical_Nesting[coreID] + 1;
    ಪೋರ್ಟ್_ux_Critical_Nesting[coreID] = newNesting;
    //If this is the first entry to a critical section. Save the old interrupt level.
    ಆದರೆ ( newNesting == 1 ) {
        ಪೋರ್ಟ್_ux_Old_ಅಡಚಣೆ_State[coreID] = x_Old_ಅಡಚಣೆ_Level;
    }
    ಹಿಂತಿರುಗಿ pdPASS;
}

ಶೂನ್ಯ __attribute__((optimize("-O3"))) v_ಪೋರ್ಟ್_Exit_Critical_Multi_Core(ಪೋರ್ಟ್_MUX_TYPE *mux)
{
#ಆದರೆ CONFIG_FREERTOS_ಪೋರ್ಟ್_THREAD_ಸುರಕ್ಷಿತ_CLAIM
    ಆದರೆ (unlikely(ಪೋರ್ಟ್_x_Thread_ಸುರಕ್ಷಿತ_Claimed)) {
        ಹಿಂತಿರುಗಿ;
    }
#ಆದರೆ_ಅಂತ್ಯ
    /* This function may be called in a nested manner. Therefore, we only need
     * to re-enable interrupts if this is the last call to exit the critical. We
     * can use the nesting count to determine whether this is the last exit call.
     */
    /* Interrupts remain disabled for the whole critical section, so the core id is
     * stable and the spinlock does not need to mask them again. Read the core id
     * once and reuse it for both the release owner id and the per-core nesting state. */
    ತಳ_Type_t coreID = x_ಪೋರ್ಟ್_Get_Core_ID();
    spinlock_release_impl(mux, spinlock_owner_id_for_core(coreID));
    ತಳ_Type_t nesting = ಪೋರ್ಟ್_ux_Critical_Nesting[coreID];

    /* Critical section nesting count must never be negative */
    configASSERT( nesting > 0 );

    ಆದರೆ (nesting > 0) {
        nesting--;
        ಪೋರ್ಟ್_ux_Critical_Nesting[coreID] = nesting;

        //This is the last exit call, restore the saved interrupt level
        ಆದರೆ ( nesting == 0 ) {
            ಪೋರ್ಟ್_CLEAR_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR(ಪೋರ್ಟ್_ux_Old_ಅಡಚಣೆ_State[coreID]);
        }
    }
}


ತಳ_Type_t x_ಪೋರ್ಟ್_Enter_Critical_Timeout_Compliance(ಪೋರ್ಟ್_MUX_TYPE *mux, ತಳ_Type_t timeout)
{
    ತಳ_Type_t ret;
    ಆದರೆ (!x_ಪೋರ್ಟ್_ಒಳ_Isr_Context()) {
        ret = x_ಪೋರ್ಟ್_Enter_Critical_Timeout(mux, timeout);
    } ಇಲ್ಲದಿದ್ದರೆ {
        esp_rom_printf("port*_CRITICAL called from ISR context. Aborting!\n");
        abort();
        ret = pdFAIL;
    }
    ಹಿಂತಿರುಗಿ ret;
}

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Exit_Critical_Compliance(ಪೋರ್ಟ್_MUX_TYPE *mux)
{
    ಆದರೆ (!x_ಪೋರ್ಟ್_ಒಳ_Isr_Context()) {
        v_ಪೋರ್ಟ್_Exit_Critical_Multi_Core(mux);
    } ಇಲ್ಲದಿದ್ದರೆ {
        esp_rom_printf("port*_CRITICAL called from ISR context. Aborting!\n");
        abort();
    }
}
#ಆದರೆ_ಅಂತ್ಯ /* (configNUM_CORES > 1) */

#ಆದರೆ CONFIG_FREERTOS_ಪೋರ್ಟ್_THREAD_ಸುರಕ್ಷಿತ_CLAIM
ಶೂನ್ಯ x_ಪೋರ್ಟ್_Thread_ಸುರಕ್ಷಿತ_Claim(ಶೂನ್ಯ)
{
    configASSERT(!x_ಪೋರ್ಟ್_Can_Yield());
    configASSERT(!ಪೋರ್ಟ್_x_Thread_ಸುರಕ್ಷಿತ_Claimed);
    ಪೋರ್ಟ್_x_Thread_ಸುರಕ್ಷಿತ_Claimed = true;
}

ಶೂನ್ಯ x_ಪೋರ್ಟ್_Thread_ಸುರಕ್ಷಿತ_Disclaim(ಶೂನ್ಯ)
{
    configASSERT(!x_ಪೋರ್ಟ್_Can_Yield());
    configASSERT(ಪೋರ್ಟ್_x_Thread_ಸುರಕ್ಷಿತ_Claimed);
    ಪೋರ್ಟ್_x_Thread_ಸುರಕ್ಷಿತ_Claimed = false;
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Enter_Critical(ಶೂನ್ಯ)
{
#ಆದರೆ (config_ಅಂಕೆ_CORES > 1)
        esp_rom_printf("vPortEnterCritical(void) is not supported on multi-core targets. Please use vPortEnterCriticalMultiCore(portMUX_TYPE *mux) instead.\n");
        abort();
#ಆದರೆ_ಅಂತ್ಯ /* (configNUM_CORES > 1) */
#ಆದರೆ CONFIG_FREERTOS_ಪೋರ್ಟ್_THREAD_ಸುರಕ್ಷಿತ_CLAIM
    ಆದರೆ (unlikely(ಪೋರ್ಟ್_x_Thread_ಸುರಕ್ಷಿತ_Claimed)) {
        ಹಿಂತಿರುಗಿ;
    }
#ಆದರೆ_ಅಂತ್ಯ
    ತಳ_Type_t state = ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR();
    ಪೋರ್ಟ್_ux_Critical_Nesting[0]++;

    ಆದರೆ (ಪೋರ್ಟ್_ux_Critical_Nesting[0] == 1) {
        ಪೋರ್ಟ್_ux_Old_ಅಡಚಣೆ_State[0] = state;
    }
}

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Exit_Critical(ಶೂನ್ಯ)
{
#ಆದರೆ (config_ಅಂಕೆ_CORES > 1)
        esp_rom_printf("vPortExitCritical(void) is not supported on multi-core targets. Please use vPortExitCriticalMultiCore(portMUX_TYPE *mux) instead.\n");
        abort();
#ಆದರೆ_ಅಂತ್ಯ /* (configNUM_CORES > 1) */
#ಆದರೆ CONFIG_FREERTOS_ಪೋರ್ಟ್_THREAD_ಸುರಕ್ಷಿತ_CLAIM
    ಆದರೆ (unlikely(ಪೋರ್ಟ್_x_Thread_ಸುರಕ್ಷಿತ_Claimed)) {
        ಹಿಂತಿರುಗಿ;
    }
#ಆದರೆ_ಅಂತ್ಯ

    /* Critical section nesting count must never be negative */
    configASSERT( ಪೋರ್ಟ್_ux_Critical_Nesting[0] > 0 );

    ಆದರೆ (ಪೋರ್ಟ್_ux_Critical_Nesting[0] > 0) {
        ಪೋರ್ಟ್_ux_Critical_Nesting[0]--;

        ಆದರೆ (ಪೋರ್ಟ್_ux_Critical_Nesting[0] == 0) {
            ಪೋರ್ಟ್_CLEAR_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR(ಪೋರ್ಟ್_ux_Old_ಅಡಚಣೆ_State[0]);
        }
    }
}

// ---------------------- Yielding -------------------------

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Yield(ಶೂನ್ಯ)
{
    ತಳ_Type_t coreID = x_ಪೋರ್ಟ್_Get_Core_ID();

    ಆದರೆ (ಪೋರ್ಟ್_ux_ಅಡಚಣೆ_Nesting[coreID]) {
        v_ಪೋರ್ಟ್_Yield_From_ISR();
    } ಇಲ್ಲದಿದ್ದರೆ {
        esp_crosscore_int_send_yield(coreID);
        /* There are 3-4 instructions of latency between triggering the software
           interrupt and the CPU interrupt happening. Make sure it happened before
           we return, otherwise vTaskDelay() may return and execute 1-2
           instructions before the delay actually happens.

           (We could use the WFI instruction here, but there is a chance that
           the interrupt will happen while evaluating the other two conditions
           for an instant yield, and if that happens then the WFI would be
           waiting for the next interrupt to occur...)
        */
        ಆಗ (ಪೋರ್ಟ್_x_Scheduler_Running[coreID] && ಪೋರ್ಟ್_ux_Critical_Nesting[coreID] == 0 && crosscore_int_ll_get_state(coreID) != 0) {}
    }
}

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Yield_From_ISR( ಶೂನ್ಯ )
{
    trace_ISR_EXIT_ಗಮ್ಯ_SCHEDULER();
    ತಳ_Type_t coreID = x_ಪೋರ್ಟ್_Get_Core_ID();
    ಪೋರ್ಟ್_x_Scheduler_Running[coreID] = 1;
    x_ಪೋರ್ಟ್_Switch_Flag[coreID] = 1;
}

ಶೂನ್ಯ v_ಪೋರ್ಟ್_Yield_Other_Core(ತಳ_Type_t coreid)
{
    esp_crosscore_int_send_yield(coreid);
}

// ------------------- Hook Functions ----------------------

ಶೂನ್ಯ __attribute__((weak)) v_Application_ರಾಶಿ_ಅತಿಪ್ರವಾಹ_Hook(TaskHandle_t xTask, ಅಕ್ಷರ *pcTaskName)
{
#ವ್ಯಾಖ್ಯಾನಿಸು ERR_STR1 "***ERROR*** A stack overflow in task "
#ವ್ಯಾಖ್ಯಾನಿಸು ERR_STR2 " has been detected."
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    ಅಕ್ಷರ buf[ಗಾತ್ರ(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + ಗಾತ್ರ(ERR_STR2) + 1 /* null char */] = {0};

    ಅಕ್ಷರ *dest = buf;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಗಾತ್ರ(str) / ಗಾತ್ರ(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
    esp_system_abort(buf);
}

// ----------------------- System --------------------------

uint32_t x_ಪೋರ್ಟ್_Get_Tick_ದರ_Hz(ಶೂನ್ಯ)
{
    ಹಿಂತಿರುಗಿ (uint32_t)config_TICK_ದರ_HZ;
}

#ವ್ಯಾಖ್ಯಾನಿಸು ರಾಶಿ_WATCH_AREA_ಗಾತ್ರ 32
#ವ್ಯಾಖ್ಯಾನಿಸು ರಾಶಿ_WATCH_ಬಿಂದು_ಸಂಖ್ಯೆ (SOC_CPU_WATCHPOINTS_ಅಂಕೆ - 1)

ಶೂನ್ಯ v_ಪೋರ್ಟ್_ಗಣ_ರಾಶಿ_Watchpoint(ಶೂನ್ಯ *px_ರಾಶಿ_ಆರಂಭ)
{
    uint32_t addr = (uint32_t)px_ರಾಶಿ_ಆರಂಭ;
    addr = (addr + (ರಾಶಿ_WATCH_AREA_ಗಾತ್ರ - 1)) & (~(ರಾಶಿ_WATCH_AREA_ಗಾತ್ರ - 1));
    esp_cpu_ಗಣ_watchpoint(ರಾಶಿ_WATCH_ಬಿಂದು_ಸಂಖ್ಯೆ, (ಅಕ್ಷರ *)addr, ರಾಶಿ_WATCH_AREA_ಗಾತ್ರ, ESP_CPU_WATCHPOINT_STORE);
}

// --------------------- TCB Cleanup -----------------------

#ಆದರೆ ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
ಸ್ಥಿರ ಶೂನ್ಯ v_ಪೋರ್ಟ್_TLSPointers_Del_Cb( ಶೂನ್ಯ *pxTCB )
{
    /* Typecast pxTCB to StaticTask_t type to access TCB struct members.
     * pvDummy15 corresponds to pvThreadLocalStoragePointers member of the TCB.
     */
    StaticTask_t *tcb = ( StaticTask_t * )pxTCB;

    /* The TLSP deletion callbacks are stored at an offset of (configNUM_THREAD_LOCAL_STORAGE_POINTERS/2) */
    TlsDeleteCallbackFunction_t *pv_Thread_Local_Storage_ಸೂಚಕಗಳು_Del_Callback = ( TlsDeleteCallbackFunction_t * )( &( tcb->pvDummy15[ ( config_ಅಂಕೆ_THREAD_LOCAL_STORAGE_ಸೂಚಕಗಳು / 2 ) ] ) );

    /* We need to iterate over half the depth of the pvThreadLocalStoragePointers area
     * to access all TLS pointers and their respective TLS deletion callbacks.
     */
    ಗಾಗಿ ( ಪೂರ್ಣಾಂಕ x = 0; x < ( config_ಅಂಕೆ_THREAD_LOCAL_STORAGE_ಸೂಚಕಗಳು / 2 ); x++ ) {
        ಆದರೆ ( pv_Thread_Local_Storage_ಸೂಚಕಗಳು_Del_Callback[ x ] != ಖಾಲಿ ) {  //If del cb is set
            /* In case the TLSP deletion callback has been overwritten by a TLS pointer, gracefully abort. */
            ಆದರೆ ( !esp_ptr_executable( pv_Thread_Local_Storage_ಸೂಚಕಗಳು_Del_Callback[ x ] ) ) {
                ESP_EARLY_LOGE("FreeRTOS", "Fatal error: TLSP deletion callback at index %d overwritten with non-excutable pointer %p", x, pv_Thread_Local_Storage_ಸೂಚಕಗಳು_Del_Callback[ x ]);
                abort();
            }

            pv_Thread_Local_Storage_ಸೂಚಕಗಳು_Del_Callback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

ಶೂನ್ಯ v_ಪೋರ್ಟ್_TCBPre_Delete_Hook( ಶೂನ್ಯ *pxTCB )
{
    #ಆದರೆ ( CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK )
        /* Call the user defined task pre-deletion hook */
        ಬಾಹ್ಯ ಶೂನ್ಯ vTaskPreDeletionHook( ಶೂನ್ಯ * pxTCB );
        vTaskPreDeletionHook( pxTCB );
    #ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK */

    #ಆದರೆ ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
        /* Call TLS pointers deletion callbacks */
        v_ಪೋರ್ಟ್_TLSPointers_Del_Cb( pxTCB );
    #ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

    #ಆದರೆ ( CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST )
        ಆದರೆ (pxTCB != ಖಾಲಿ) {
            v_Task_ಗಣ_Pie_Blacklisted((TaskHandle_t)pxTCB, pdFALSE);
        }
    #ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST */

    #ಆದರೆ ( SOC_CPU_COPROC_ಅಂಕೆ > 0 )
        /* Cleanup coproc save area */
        v_ಪೋರ್ಟ್_Clean_ಮೇಲೆ_Coproc_Area( pxTCB );
    #ಆದರೆ_ಅಂತ್ಯ /* SOC_CPU_COPROC_NUM > 0 */
}


#ಆದರೆ ( SOC_CPU_COPROC_ಅಂಕೆ > 0 )

// ----------------------- Coprocessors --------------------------

/**
 * @brief Pin the given task to the given core
 *
 * This function is called when a task uses a coprocessor. Since the coprocessors registers
 * are saved lazily, as soon as a task starts using one, it must always be scheduled on the core
 * it is currently executing on.
 */
#ಆದರೆ ( config_ಅಂಕೆ_CORES > 1 )
ಶೂನ್ಯ v_ಪೋರ್ಟ್_Task_Pin_ಗಮ್ಯ_Core(StaticTask_t* task, ಪೂರ್ಣಾಂಕ coreid)
{
    task->xDummyCoreID = coreid;
}
#ಆದರೆ_ಅಂತ್ಯ /* configNUM_CORES > 1 */


/**
 * @brief Function to call to simulate an `abort()` occurring in a different context than the one it's called from.
 */
ಬಾಹ್ಯ ಶೂನ್ಯ xt_unhandled_exception(ಶೂನ್ಯ *ಚೌಕಟ್ಟು);

/**
 * @brief Get coprocessor save area out of the given task. If the coprocessor area is not created,
 *        it shall be allocated.
 *
 * @param task Task to get the coprocessor save area of
 * @param allocate When true, memory will be allocated for the coprocessor if it hasn't been allocated yet.
 *                 When false, the coprocessor memory will be left as NULL if not allocated.
 * @param coproc Coprocessor number to allocate memory for
 */
RvCoprocSaveArea* px_ಪೋರ್ಟ್_Get_Coproc_Area(StaticTask_t* task, bool allocate, ಪೂರ್ಣಾಂಕ coproc)
{
    ಪ್ರತಿಜ್ಞಾ(coproc < SOC_CPU_COPROC_ಅಂಕೆ);

    ಸ್ಥಿರಾಂಕ UBaseType_t bottomstack = (UBaseType_t) task->pxDummy8;
    RvCoprocSaveArea* sa = px_Retrieve_Coproc_Save_Area_From_ರಾಶಿ_ಸೂಚಕ(bottomstack);
    /* Check if the allocator is NULL. Since we don't have a way to get the end of the stack
     * during its initialization, we have to do this here */
    ಆದರೆ (sa->sa_allocator == 0) {
        /* Since the lowest stack address shall not be used as `sp` anymore, we will modify it */
        sa->sa_tcbstack = task->pxDummy6;
        sa->sa_allocator = (UBaseType_t) task->pxDummy6;
    }

    /* Check if coprocessor area is allocated */
    ಆದರೆ (allocate && sa->sa_coprocs[coproc] == ಖಾಲಿ) {
        ಸ್ಥಿರಾಂಕ uint32_t coproc_sa_sizes[] = {
            RV_COPROC0_ಗಾತ್ರ, RV_COPROC1_ಗಾತ್ರ, RV_COPROC2_ಗಾತ್ರ
        };
        ಸ್ಥಿರಾಂಕ uint32_t coproc_sa_align[] = {
            RV_COPROC0_ALIGN, RV_COPROC1_ALIGN, RV_COPROC2_ALIGN
        };
        /* The allocator points to a usable part of the stack, use it for the coprocessor.
         * Align it up to the coprocessor save area requirement */
        sa->sa_allocator = (sa->sa_allocator + coproc_sa_align[coproc] - 1) & ~(coproc_sa_align[coproc] - 1);
        sa->sa_coprocs[coproc] = (ಶೂನ್ಯ*) (sa->sa_allocator);
        sa->sa_allocator += coproc_sa_sizes[coproc];
        /* Update the lowest address of the stack to prevent FreeRTOS performing overflow/watermark checks on the coprocessors contexts */
        task->pxDummy6 = (ಶೂನ್ಯ*) (sa->sa_allocator);
        /* Make sure the Task stack pointer is not pointing to the coprocessor context area, in other words, make
         * sure we don't have a stack overflow */
        ಶೂನ್ಯ* task_sp = task->pxDummy1;
        ಆದರೆ (task_sp <= task->pxDummy6) {
            /* In theory we need to call vApplicationStackOverflowHook to trigger the stack overflow callback,
             * but in practice, since we are already in an exception handler, this won't work, so let's manually
             * trigger an exception with the previous coprocessor owner's TCB */
            g_panic_abort = true;
            g_panic_abort_details = (ಅಕ್ಷರ *) "ERROR: Stack overflow while saving coprocessor context!\n";
            xt_unhandled_exception(task_sp);
        }
    }
    ಹಿಂತಿರುಗಿ sa;
}


/**
 * @brief Update given coprocessor owner and get the address of former owner's save area.
 *
 * This function is called when the current running task has poked a coprocessor's register which
 * was used by a previous task. We have to save the coprocessor context (registers) inside the
 * current owner's save area and change the ownership. The coprocessor will be marked as used in
 * the new owner's coprocessor save area.
 *
 * @param coreid    Current core
 * @param coproc    Coprocessor to save context of
 * @param owner     New owner of the coprocessor. Can be NULL to clear the owner.
 *
 * @returns Coprocessor former owner's save area, can be NULL if there was no owner yet, can be -1 if
 *          the former owner is the same as the new owner.
 */
RvCoprocSaveArea* px_ಪೋರ್ಟ್_Update_Coproc_Owner(ಪೂರ್ಣಾಂಕ coreid, ಪೂರ್ಣಾಂಕ coproc, StaticTask_t* owner)
{
    RvCoprocSaveArea* sa = ಖಾಲಿ;
    /* Address of coprocessor owner */
    StaticTask_t** owner_addr = &ಪೋರ್ಟ್_ux_Coproc_Owner[ coreid ][ coproc ];
    /* Atomically exchange former owner with the new one */
    StaticTask_t* former = Atomic_ಅದಲುಬದಲು_ಸೂಚಕಗಳು_p32((ಶೂನ್ಯ**) owner_addr, owner);
    /* Get the save area of former owner. small optimization here, if the former owner is the new owner,
     * return -1. This will simplify the assembly code while making it faster. */
    ಆದರೆ (former == owner) {
        sa = (ಶೂನ್ಯ*) -1;
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (former != ಖಾಲಿ) {
        /* Allocate coprocessor memory if not available yet */
        sa = px_ಪೋರ್ಟ್_Get_Coproc_Area(former, true, coproc);
    }
    ಹಿಂತಿರುಗಿ sa;
}


/**
 * @brief Aborts execution when a coprocessor was used in an ISR context
 */
ಶೂನ್ಯ v_ಪೋರ್ಟ್_Coproc_ಬಳಸಿದ_ಒಳ_ISR(ಶೂನ್ಯ* ಚೌಕಟ್ಟು)
{
    /* Since this function is called from an exception handler, the interrupts are disabled,
     * as such, it is not possible to trigger another exception as would `abort` do.
     * Simulate an abort without actually triggering an exception. */
    g_panic_abort = true;
    g_panic_abort_details = (ಅಕ್ಷರ *) "ERROR: Coprocessors must not be used in ISRs!\n";
    xt_unhandled_exception(ಚೌಕಟ್ಟು);
}

#ಆದರೆ CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST
/**
 * @brief Abort if the given task is blacklisted from using the PIE coprocessor.
 *
 * Called from the PIE illegal-instruction path, so this must not trigger another
 * exception via abort().
 */
ಶೂನ್ಯ v_ಪೋರ್ಟ್_Abort_If_Task_Pie_Blacklisted(StaticTask_t *task, ಶೂನ್ಯ *ಚೌಕಟ್ಟು)
{
    ಆದರೆ (task != ಖಾಲಿ && xTaskGetPieBlacklisted((TaskHandle_t)task) != pdFALSE) {
        g_panic_abort = true;
        g_panic_abort_details = (ಅಕ್ಷರ *) "ERROR: Blacklisted task executed a PIE instruction!\n";
        xt_unhandled_exception(ಚೌಕಟ್ಟು);
    }
}
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST */

#ಆದರೆ CONFIG_IDF_ಗುರಿ_ESP32_S31
/* On the ESP32-S31, the PIE is only available on core 1, so we need to perform a few checks when core 0 uses a PIE instruction.
 * the functions here will help us with that. */

/**
 * @brief Called when a task uses a PIE instruction on core 0.
 *
 * @param task Task that used the PIE instruction
 * @param frame Frame of the PIE instruction
 *
 * @returns The context to save with the current FPU context when the current task was the FPU owner,
 *          NULL if the task is not the owner of the FPU on the current core.
 */
ಶೂನ್ಯ* v_ಪೋರ್ಟ್_Task_ಬಳಸಿದ_PIEOn_CPU0(StaticTask_t* task, ಶೂನ್ಯ* ಚೌಕಟ್ಟು)
{
#ಆದರೆ CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST
    v_ಪೋರ್ಟ್_Abort_If_Task_Pie_Blacklisted(task, ಚೌಕಟ್ಟು);
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ CONFIG_FREERTOS_UNICORE
    g_panic_abort = true;
    g_panic_abort_details = (ಅಕ್ಷರ *) "ERROR: PIE coprocessor is not supported in unicore configuration!\n";
    xt_unhandled_exception(ಚೌಕಟ್ಟು);
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
#ಇಲ್ಲದಿದ್ದರೆ
    ಶೂನ್ಯ* context_ಗಮ್ಯ_save = ಖಾಲಿ;
    /* Make sure we are not in an interrupt context nor in a critical section */
    ಆದರೆ (x_ಪೋರ್ಟ್_ಒಳ_Isr_Context()) {
        /* We are in an interrupt context, abort */
        v_ಪೋರ್ಟ್_Coproc_ಬಳಸಿದ_ಒಳ_ISR(ಚೌಕಟ್ಟು);
    }
    /* Since we count on crosscore interrupt to reschedule the current task, we must not be in a
     * critical section. In other words, we must be able to yield */
    ಆದರೆ (!x_ಪೋರ್ಟ್_Can_Yield()) {
        g_panic_abort = true;
        g_panic_abort_details = (ಅಕ್ಷರ *) "ERROR: PIE coprocessor must not be used in critical sections!\n";
        xt_unhandled_exception(ಚೌಕಟ್ಟು);
    }

    /* Check if the current task is the owner of the FPU on the current core. No need to make the following two instructions
     * atomic since we are in an exception context, we can't be interrupted by an interrupt. */
    ಆದರೆ (ಪೋರ್ಟ್_ux_Coproc_Owner[0][FPU_COPROC_IDX] == task) {
        /* Task is the owner of the FPU on the current core, set the new owner to NULL and return the save area to fill */
        RvCoprocSaveArea* sa = px_ಪೋರ್ಟ್_Update_Coproc_Owner(0, FPU_COPROC_IDX, ಖಾಲಿ);
        /* `sa` is not NULL here for sure */
        context_ಗಮ್ಯ_save = sa->sa_coprocs[FPU_COPROC_IDX];
    }
    /* Migrate the task to core 1. NOTE: This will override any existing pinning of the task */
    v_ಪೋರ್ಟ್_Task_Pin_ಗಮ್ಯ_Core(task, 1);
    /* Raised an error if the scheduler is NOT running on core 0 */
    ಆದರೆ (!ಪೋರ್ಟ್_x_Scheduler_Running[0]) {
        /* Scheduler is not running on core 0, raise an error */
        g_panic_abort = true;
        g_panic_abort_details = (ಅಕ್ಷರ *) "ERROR: Scheduler is not running on core 0, task must migrate to core 1!\n";
        xt_unhandled_exception(ಚೌಕಟ್ಟು);
    }
    /* Send a cross-core interrupt on the current core, it won't be triggered until we return from the exception handler */
    esp_crosscore_int_send_yield(0);
    ಹಿಂತಿರುಗಿ context_ಗಮ್ಯ_save;
#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_UNICORE */
}

#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_IDF_TARGET_ESP32S31 */


#ಆದರೆ_ಅಂತ್ಯ /* SOC_CPU_COPROC_NUM > 0 */

/* ------------------------------------------------ Run Time Stats ------------------------------------------------- */

#ಆದರೆ ( CONFIG_FREERTOS_GENERATE_ಸತತ_ಸಮಯ_STATS )

config_ಸತತ_ಸಮಯ_COUNTER_TYPE x_ಪೋರ್ಟ್_Get_ಸತತ_ಸಮಯ_Counter_ಮೌಲ್ಯ( ಶೂನ್ಯ )
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FREERTOS_ಸತತ_ಸಮಯ_STATS_USING_ESP_TIMER
    ಹಿಂತಿರುಗಿ (config_ಸತತ_ಸಮಯ_COUNTER_TYPE) esp_timer_get_ಸಮಯ();
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ 0;
#ಆದರೆ_ಅಂತ್ಯ // CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
}

#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */

/* ---------------------------------------------- Misc Implementations -------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */
#ಆದರೆ (SOC_CPU_COPROC_ಅಂಕೆ > 0) && SOC_CPU_HAS_FPU && SOC_PM_FPU_RETENTION_BY_SW
ತಳ_Type_t x_ಪೋರ್ಟ್_FPUContext_Is_Dirty(ತಳ_Type_t core_id)
{
    ಹಿಂತಿರುಗಿ (ತಳ_Type_t)(ಪೋರ್ಟ್_ux_Coproc_Owner[core_id][FPU_COPROC_IDX] != ಖಾಲಿ);
}
#ಆದರೆ_ಅಂತ್ಯ /* (SOC_CPU_COPROC_NUM > 0) && SOC_CPU_HAS_FPU && SOC_PM_FPU_RETENTION_BY_SW */
