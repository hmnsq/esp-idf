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

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdbool.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "soc/soc_caps.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "soc/periph_defs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "soc/system_reg.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "soc/interrupt_reg.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hal/systimer_hal.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hal/systimer_ll.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "riscv/rvruntime-frames.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "riscv/rv_utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "riscv/interrupt.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_private/crosscore_int.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hal/crosscore_int_ll.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_attr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_compiler.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_intr_alloc.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_log.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "FreeRTOS.h"       /* This pulls in portmacro.h */
#ਅਨ੍ਤਰ੍ਭਾਵ "task.h"
#ਜੇ CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/idf_additions.h"
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ "portmacro.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "port_systick.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_memory_utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_macros.h"
#ਜੇ CONFIG_FREERTOS_ਲੜੀਵਾਰ_ਸਮਾਂ_STATS_USING_ESP_TIMER
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_timer.h"
#ਜੇ_ਅੰਤ

#ਜੇ SOC_CPU_HAS_HWLOOP
#ਅਨ੍ਤਰ੍ਭਾਵ "riscv/csr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "riscv/csr_hwlp.h"
#ਜੇ_ਅੰਤ

#ਜੇ ( SOC_CPU_COPROC_ਅੰਕ > 0 )

#ਅਨ੍ਤਰ੍ਭਾਵ "esp_private/panic_internal.h"

/* Since `portFORCE_INLINE` is not defined in `portmacro.h`, we must define it here since it is
 * used by `atomic.h`. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_FORCE_INLINE    ਸਤਰਅੰਦਰ
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/atomic.h"

#ਜੇ_ਅੰਤ // ( SOC_CPU_COPROC_NUM > 0 )


_Static_assert(ਪੋਰਟ_BYTE_ALIGNMENT == 16, "portBYTE_ALIGNMENT must be set to 16");
#ਜੇ CONFIG_ESP_SYSTEM_HW_ਢੇਰ_GUARD
/**
 * offsetof() can not be used in asm code. Then we need make sure that
 * PORT_OFFSET_PX_STACK and PORT_OFFSET_PX_END_OF_STACK have expected values.
 * Macro used in the portasm.S instead of variables to save at least 4 instruction calls
 * which accessing DRAM memory. This optimization saves CPU time in the interrupt handling.
 */

_Static_assert(offsetof( StaticTask_t, pxDummy6 ) == ਪੋਰਟ_ਵਿੱਥ_PX_ਢੇਰ);
_Static_assert(offsetof( StaticTask_t, pxDummy8 ) == ਪੋਰਟ_ਵਿੱਥ_PX_ਸਿਰਾ_ਦਾ_ਢੇਰ);
#ਜੇ_ਅੰਤ // CONFIG_ESP_SYSTEM_HW_STACK_GUARD

/* ---------------------------------------------------- Variables ------------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */

ਅਸਥਿਰ UBaseType_t ਪੋਰਟ_x_Scheduler_Running[ਪੋਰਟ_ਅੰਕ_PROCESSORS] = {0}; // Indicates whether scheduler is running on a per-core basis
ਅਸਥਿਰ UBaseType_t ਪੋਰਟ_ux_ਰੁਕਾਵਟ_Nesting[ਪੋਰਟ_ਅੰਕ_PROCESSORS] = {0};  // Interrupt nesting level. Increased/decreased in portasm.c
ਅਸਥਿਰ UBaseType_t ਪੋਰਟ_ux_Critical_Nesting[ਪੋਰਟ_ਅੰਕ_PROCESSORS] = {0};
#ਜੇ CONFIG_FREERTOS_ਪੋਰਟ_THREAD_ਸੁਰੱਖਿਅਤ_CLAIM
ਅਸਥਿਰ bool ਪੋਰਟ_x_Thread_ਸੁਰੱਖਿਅਤ_Claimed = false;
#ਜੇ_ਅੰਤ
ਅਸਥਿਰ UBaseType_t ਪੋਰਟ_ux_Old_ਰੁਕਾਵਟ_State[ਪੋਰਟ_ਅੰਕ_PROCESSORS] = {0};
ਅਸਥਿਰ UBaseType_t x_ਪੋਰਟ_Switch_Flag[ਪੋਰਟ_ਅੰਕ_PROCESSORS] = {0};
ਅਸਥਿਰ UBaseType_t ਪੋਰਟ_ux_Core_Startup_ਪੂਰਾ[ਪੋਰਟ_ਅੰਕ_PROCESSORS] = {0};  // Indicates whether the core has completed its startup sequence

#ਜੇ ( SOC_CPU_COPROC_ਅੰਕ > 0 )

/* Current owner of the coprocessors for each core */
StaticTask_t* ਪੋਰਟ_ux_Coproc_Owner[ਪੋਰਟ_ਅੰਕ_PROCESSORS][SOC_CPU_COPROC_ਅੰਕ];

#ਜੇ_ਅੰਤ /* SOC_CPU_COPROC_NUM > 0 */

/*
*******************************************************************************
* Interrupt stack. The size of the interrupt stack is determined by the config
* parameter "configISR_STACK_SIZE" in FreeRTOSConfig.h
*******************************************************************************
*/
__attribute__((aligned(16))) ਢੇਰ_Type_t x_Isr_ਢੇਰ[ਪੋਰਟ_ਅੰਕ_PROCESSORS][config_ISR_ਢੇਰ_ਮਾਪ];
ਢੇਰ_Type_t *x_Isr_ਢੇਰ_ਸਿਖਰ[ਪੋਰਟ_ਅੰਕ_PROCESSORS] = {0};

#ਜੇ CONFIG_ESP_SYSTEM_HW_ਢੇਰ_GUARD
ਢੇਰ_Type_t *x_Isr_ਢੇਰ_Bottom[ਪੋਰਟ_ਅੰਕ_PROCESSORS] = {0};
#ਜੇ_ਅੰਤ

/* ------------------------------------------------ FreeRTOS Portable --------------------------------------------------
 * - Provides implementation for functions required by FreeRTOS
 * - Declared in portable.h
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------- Scheduler Start/End -------------------

ਅਧਾਰ_Type_t x_ਪੋਰਟ_ਸ਼ੁਰੂ_Scheduler(ਖਾਲੀ)
{
#ਜੇ ( SOC_CPU_COPROC_ਅੰਕ > 0 )

#ਜੇ SOC_CPU_HAS_FPU
    /* Disable FPU so that the first task to use it will trigger an exception */
    rv_utils_disable_fpu();
#ਜੇ_ਅੰਤ /* SOC_CPU_HAS_FPU */

#ਜੇ SOC_CPU_HAS_PIE
    /* Similarly, disable PIE */
    rv_utils_disable_pie();
#ਜੇ_ਅੰਤ /* SOC_CPU_HAS_PIE */

#ਜੇ SOC_CPU_HAS_DSP
    rv_utils_disable_dsp();
#ਜੇ_ਅੰਤ /* SOC_CPU_HAS_DSP */

#ਜੇ SOC_CPU_HAS_HWLOOP
    /* Initialize the Hardware loop feature */
    RV_WRITE_CSR(CSR_HWLP_STATE_REG, HWLP_INITIAL_STATE);
#ਜੇ_ਅੰਤ /* SOC_CPU_HAS_HWLOOP */
#ਜੇ_ਅੰਤ /* ( SOC_CPU_COPROC_NUM > 0 ) */

    /* Initialize all kernel state tracking variables */
    ਅਧਾਰ_Type_t coreID = x_ਪੋਰਟ_Get_Core_ID();
    ਪੋਰਟ_ux_ਰੁਕਾਵਟ_Nesting[coreID] = 0;
    ਪੋਰਟ_ux_Critical_Nesting[coreID] = 0;
#ਜੇ CONFIG_FREERTOS_ਪੋਰਟ_THREAD_ਸੁਰੱਖਿਅਤ_CLAIM
    ਪੋਰਟ_x_Thread_ਸੁਰੱਖਿਅਤ_Claimed = false;
#ਜੇ_ਅੰਤ
    ਪੋਰਟ_x_Scheduler_Running[coreID] = 0;
    ਪੋਰਟ_ux_Core_Startup_ਪੂਰਾ[coreID] = 0;

    /* Initialize ISR Stack(s) */
    ਲਈ (ਪੂਰਨ i = 0; i < ਪੋਰਟ_ਅੰਕ_PROCESSORS; i++) {
        x_Isr_ਢੇਰ_ਸਿਖਰ[i] = &x_Isr_ਢੇਰ[i][0] + (config_ISR_ਢੇਰ_ਮਾਪ & (~((ਪੋਰਟ_ਸੂਚਕ_ਮਾਪ_TYPE)ਪੋਰਟ_BYTE_ALIGNMENT_ਨਕਾਬ)));
#ਜੇ CONFIG_ESP_SYSTEM_HW_ਢੇਰ_GUARD
        x_Isr_ਢੇਰ_Bottom[i] = &x_Isr_ਢੇਰ[i][0];
#ਜੇ_ਅੰਤ
    }

    /* Setup the hardware to generate the tick. */
    v_ਪੋਰਟ_ਤਿਆਰੀ_Timer();

    rv_utils_intr_global_enable();
    esprv_int_ਸੈੱਟ_ਹੱਦ(RVHAL_INTR_ENABLE_THRESH); /* set global interrupt masking level */

    v_ਪੋਰਟ_Yield();

    /* Should not get here */
    ਵਾਪਸ pdTRUE;
}

ਖਾਲੀ v_ਪੋਰਟ_ਸਿਰਾ_Scheduler(ਖਾਲੀ)
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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_ਢੇਰ_TLS(UBaseType_t ux_ਢੇਰ_ਸੂਚਕ, uint32_t *ret_threadptr_reg_init)
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
    ਬਾਹਰੀ ਅੱਖਰ thread_local_ਅੰਕੜੇ_ਸ਼ੁਰੂ, thread_local_ਅੰਕੜੇ_ਸਿਰਾ;
    ਬਾਹਰੀ ਅੱਖਰ thread_local_bss_ਸ਼ੁਰੂ, thread_local_bss_ਸਿਰਾ;
    ਅਚਲ uint32_t tls_ਅੰਕੜੇ_ਮਾਪ = (uint32_t)&thread_local_ਅੰਕੜੇ_ਸਿਰਾ - (uint32_t)&thread_local_ਅੰਕੜੇ_ਸ਼ੁਰੂ;
    ਅਚਲ uint32_t tls_bss_ਮਾਪ = (uint32_t)&thread_local_bss_ਸਿਰਾ - (uint32_t)&thread_local_bss_ਸ਼ੁਰੂ;
    ਅਚਲ uint32_t tls_area_ਮਾਪ = ESP_ALIGN_ਉੱਪਰ(tls_ਅੰਕੜੇ_ਮਾਪ + tls_bss_ਮਾਪ, 16);
    // TODO: check that TLS area fits the stack

    // Allocate space for the TLS area on the stack. The area must be aligned to 16-bytes
    ux_ਢੇਰ_ਸੂਚਕ = ESP_ALIGN_ਥੱਲੇ(ux_ਢੇਰ_ਸੂਚਕ - (UBaseType_t)tls_area_ਮਾਪ, 16);
    // Initialize the TLS data with the initialization values of each TLS variable
    ਸਮ੍ਰਿਤੀ_ਨਕਲ((ਖਾਲੀ *)ux_ਢੇਰ_ਸੂਚਕ, &thread_local_ਅੰਕੜੇ_ਸ਼ੁਰੂ, tls_ਅੰਕੜੇ_ਮਾਪ);
    // Initialize the TLS bss with zeroes
    memset((ਖਾਲੀ *)(ux_ਢੇਰ_ਸੂਚਕ + tls_ਅੰਕੜੇ_ਮਾਪ), 0, tls_bss_ਮਾਪ);

    // Save tls start address
    *ret_threadptr_reg_init = (uint32_t)ux_ਢੇਰ_ਸੂਚਕ;
    ਵਾਪਸ ux_ਢੇਰ_ਸੂਚਕ;
}

#ਜੇ CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
ਸਥਿਰ ਖਾਲੀ v_ਪੋਰਟ_Task_Wrapper(TaskFunction_t px_ਕੋਡ, ਖਾਲੀ *pvParameters)
{
    __asm__ ਅਸਥਿਰ(".cfi_undefined ra");  // tell to debugger that it's outermost (initial) frame
    ਬਾਹਰੀ ਖਾਲੀ __attribute__((noreturn)) panic_abort(ਅਚਲ ਅੱਖਰ *details);
    ਸਥਿਰ ਅੱਖਰ DRAM_ATTR ਸੁਨੇਹਾ[80] = "FreeRTOS: FreeRTOS Task \"\0";
    px_ਕੋਡ(pvParameters);
    /* FreeRTOS tasks should not return. Log the task name and abort. */
    /* We cannot use s(n)printf because it is in flash */
    strcat(ਸੁਨੇਹਾ, pcTaskGetName(ਨਿਰਾ));
    strcat(ਸੁਨੇਹਾ, "\" should not return, Aborting now!");
    panic_abort(ਸੁਨੇਹਾ);
}
#ਜੇ_ਅੰਤ // CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER


#ਜੇ ( SOC_CPU_COPROC_ਅੰਕ > 0 )

/**
 * @brief Retrieve or allocate coprocessors save area from the given pxTopOfStack address.
 *
 * @param pxTopOfStack End of the stack address. This represents the highest address of a Task's stack.
 */
FORCE_INLINE_ATTR RvCoprocSaveArea* px_Retrieve_Coproc_Save_Area_From_ਢੇਰ_ਸੂਚਕ(UBaseType_t px_ਸਿਖਰ_ਦਾ_ਢੇਰ)
{
    ਵਾਪਸ (RvCoprocSaveArea*) ESP_ALIGN_ਥੱਲੇ(px_ਸਿਖਰ_ਦਾ_ਢੇਰ - ਆਕਾਰ(RvCoprocSaveArea), 16);
}

/**
 * @brief Allocate and initialize the coprocessors save area on the stack
 *
 * @param[in] uxStackPointer Current stack pointer address
 *
 * @return Stack pointer that points to allocated and initialized the coprocessor save area
 */
FORCE_INLINE_ATTR UBaseType_t uxInitialiseCoprocSaveArea(UBaseType_t ux_ਢੇਰ_ਸੂਚਕ)
{
    RvCoprocSaveArea* sa = px_Retrieve_Coproc_Save_Area_From_ਢੇਰ_ਸੂਚਕ(ux_ਢੇਰ_ਸੂਚਕ);
    memset(sa, 0, ਆਕਾਰ(RvCoprocSaveArea));
    ਵਾਪਸ (UBaseType_t) sa;
}


ਸਥਿਰ ਖਾਲੀ v_ਪੋਰਟ_Clean_ਉੱਪਰ_Coproc_Area(ਖਾਲੀ *pvTCB)
{
    StaticTask_t* task = (StaticTask_t*) pvTCB;

    /* Get a pointer to the task's coprocessor save area */
    ਅਚਲ UBaseType_t bottomstack = (UBaseType_t) task->pxDummy8;
    RvCoprocSaveArea* sa = px_Retrieve_Coproc_Save_Area_From_ਢੇਰ_ਸੂਚਕ(bottomstack);

    /* If the Task ever saved the original stack pointer, restore it before returning */
    ਜੇ (sa->sa_allocator != 0) {
        /* Restore the original lowest address of the stack in the TCB */
        task->pxDummy6 = sa->sa_tcbstack;

        /* Get the core the task is pinned on */
        #ਜੇ ( config_ਅੰਕ_CORES > 1 )
            ਅਚਲ ਅਧਾਰ_Type_t coreID = task->xDummyCoreID;
            /* If the task is not pinned on any core, it didn't use any coprocessor than need to be freed (FPU or PIE).
             * If it used the HWLP coprocessor, it has nothing to clear since there is no "owner" for it. */
            ਜੇ (coreID == tskNO_AFFINITY) {
                ਵਾਪਸ;
            }
        #ਨਹੀਂਤਾਂ /* configNUM_CORES > 1 */
            ਅਚਲ ਅਧਾਰ_Type_t coreID = 0;
        #ਜੇ_ਅੰਤ /* configNUM_CORES > 1 */

        ਲਈ (ਪੂਰਨ i = 0; i < SOC_CPU_COPROC_ਅੰਕ; i++) {
            StaticTask_t** owner = &ਪੋਰਟ_ux_Coproc_Owner[coreID][i];
            /* If the owner is `task`, replace it with NULL atomically */
            Atomic_Compare_And_ਅਦਲਾ_ਬਦਲੀ_ਸੂਚਕ_ਸਾਰੇ_p32((ਖਾਲੀ**) owner, ਨਿਰਾ, task);
        }
    }
}
#ਜੇ_ਅੰਤ /* SOC_CPU_COPROC_NUM > 0 */


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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_ਢੇਰ_ਚੌਖਟਾ(UBaseType_t ux_ਢੇਰ_ਸੂਚਕ, TaskFunction_t px_ਕੋਡ, ਖਾਲੀ *pvParameters, uint32_t threadptr_reg_init)
{
    /*
    Allocate space for the task's starting interrupt stack frame.
    - The stack frame must be allocated to a 16-byte aligned address.
    - We use RV_STK_FRMSZ as it rounds up the total size to a multiple of 16.
    */
    ux_ਢੇਰ_ਸੂਚਕ = ESP_ALIGN_ਥੱਲੇ(ux_ਢੇਰ_ਸੂਚਕ - RV_STK_FRMSZ, 16);

    // Clear the entire interrupt stack frame
    Rv_Exc_ਚੌਖਟਾ *ਚੌਖਟਾ = (Rv_Exc_ਚੌਖਟਾ *)ux_ਢੇਰ_ਸੂਚਕ;
    memset(ਚੌਖਟਾ, 0, ਆਕਾਰ(Rv_Exc_ਚੌਖਟਾ));

    /* Initialize the stack frame. */
    ਬਾਹਰੀ uint32_t __global_pointer$;
    #ਜੇ CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
        ਚੌਖਟਾ->mepc = (UBaseType_t)v_ਪੋਰਟ_Task_Wrapper;
        ਚੌਖਟਾ->a0 = (UBaseType_t)px_ਕੋਡ;
        ਚੌਖਟਾ->a1 = (UBaseType_t)pvParameters;
    #ਨਹੀਂਤਾਂ
        ਚੌਖਟਾ->mepc = (UBaseType_t)px_ਕੋਡ;
        ਚੌਖਟਾ->a0 = (UBaseType_t)pvParameters;
    #ਜੇ_ਅੰਤ // CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
    ਚੌਖਟਾ->gp = (UBaseType_t)&__global_pointer$;
    ਚੌਖਟਾ->tp = (UBaseType_t)threadptr_reg_init;

    ਵਾਪਸ ux_ਢੇਰ_ਸੂਚਕ;
}

ਢੇਰ_Type_t *px_ਪੋਰਟ_Initialise_ਢੇਰ(ਢੇਰ_Type_t *px_ਸਿਖਰ_ਦਾ_ਢੇਰ, TaskFunction_t px_ਕੋਡ, ਖਾਲੀ *pvParameters)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __clang_analyzer__
    // Teach clang-tidy that pxTopOfStack cannot be a pointer to const
    ਅਸਥਿਰ ਢੇਰ_Type_t * px_ਆਰਜ਼ੀ = px_ਸਿਖਰ_ਦਾ_ਢੇਰ;
    px_ਸਿਖਰ_ਦਾ_ਢੇਰ = px_ਆਰਜ਼ੀ;
#ਜੇ_ਅੰਤ /*__clang_analyzer__ */
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

    UBaseType_t ux_ਢੇਰ_ਸੂਚਕ = (UBaseType_t)px_ਸਿਖਰ_ਦਾ_ਢੇਰ;
    configASSERT((ux_ਢੇਰ_ਸੂਚਕ & ਪੋਰਟ_BYTE_ALIGNMENT_ਨਕਾਬ) == 0);

#ਜੇ ( SOC_CPU_COPROC_ਅੰਕ > 0 )
    // Initialize the coprocessors save area
    ux_ਢੇਰ_ਸੂਚਕ = uxInitialiseCoprocSaveArea(ux_ਢੇਰ_ਸੂਚਕ);
    configASSERT((ux_ਢੇਰ_ਸੂਚਕ & ਪੋਰਟ_BYTE_ALIGNMENT_ਨਕਾਬ) == 0);
#ਜੇ_ਅੰਤ // SOC_CPU_COPROC_NUM > 0

    // Initialize GCC TLS area
    uint32_t threadptr_reg_init;
    ux_ਢੇਰ_ਸੂਚਕ = ux_Initialise_ਢੇਰ_TLS(ux_ਢੇਰ_ਸੂਚਕ, &threadptr_reg_init);
    configASSERT((ux_ਢੇਰ_ਸੂਚਕ & ਪੋਰਟ_BYTE_ALIGNMENT_ਨਕਾਬ) == 0);

    // Initialize the starting interrupt stack frame
    ux_ਢੇਰ_ਸੂਚਕ = ux_Initialise_ਢੇਰ_ਚੌਖਟਾ(ux_ਢੇਰ_ਸੂਚਕ, px_ਕੋਡ, pvParameters, threadptr_reg_init);
    configASSERT((ux_ਢੇਰ_ਸੂਚਕ & ਪੋਰਟ_BYTE_ALIGNMENT_ਨਕਾਬ) == 0);

    // Return the task's current stack pointer address which should point to the starting interrupt stack frame
    ਵਾਪਸ (ਢੇਰ_Type_t *)ux_ਢੇਰ_ਸੂਚਕ;
}



/* ---------------------------------------------- Port Implementations -------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

ਅਧਾਰ_Type_t x_ਪੋਰਟ_ਵਿੱਚ_Isr_Context(ਖਾਲੀ)
{
#ਜੇ (config_ਅੰਕ_CORES > 1)
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ irq_ਸਥਿਤੀ;
    ਅਧਾਰ_Type_t ret;

    /* Disable interrupts to fetch the coreID atomically */
    irq_ਸਥਿਤੀ = ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR();

    /* Return the interrupt nesting counter for this core */
    ret = ਪੋਰਟ_ux_ਰੁਕਾਵਟ_Nesting[x_ਪੋਰਟ_Get_Core_ID()];

    /* Restore interrupts */
    ਪੋਰਟ_CLEAR_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR(irq_ਸਥਿਤੀ);

    ਵਾਪਸ ret;
#ਨਹੀਂਤਾਂ
    /* Optimize the call for single-core targets */
    ਵਾਪਸ ਪੋਰਟ_ux_ਰੁਕਾਵਟ_Nesting[0];
#ਜੇ_ਅੰਤ /* (configNUM_CORES > 1) */
}

ਖਾਲੀ v_ਪੋਰਟ_Assert_If_ਵਿੱਚ_ISR(ਖਾਲੀ)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(x_ਪੋਰਟ_ਵਿੱਚ_Isr_Context() == 0);
}

ਅਧਾਰ_Type_t x_ਪੋਰਟ_Interrupted_From_ISRContext(ਖਾਲੀ)
{
    /* Return the interrupt nesting counter for this core */
    ਵਾਪਸ ਪੋਰਟ_ux_ਰੁਕਾਵਟ_Nesting[x_ਪੋਰਟ_Get_Core_ID()];
}

UBaseType_t x_ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ_From_ISR(ਖਾਲੀ)
{
    UBaseType_t prev_int_level = 0, int_level = 0;
#ਜੇ !SOC_INT_CLIC_SUPPORTED
    int_level = RVHAL_EXCM_LEVEL;
#ਨਹੀਂਤਾਂ
    int_level = RVHAL_EXCM_LEVEL_CLIC;
#ਜੇ_ਅੰਤ

    prev_int_level = rv_utils_ਸੈੱਟ_intlevel_regval(int_level);
    ਵਾਪਸ prev_int_level;
}

ਖਾਲੀ v_ਪੋਰਟ_Clear_ਰੁਕਾਵਟ_ਨਕਾਬ_From_ISR(UBaseType_t prev_int_level)
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
    asm ਅਸਥਿਰ ( "nop" );
    asm ਅਸਥਿਰ ( "nop" );
    asm ਅਸਥਿਰ ( "nop" );
}

// ------------------ Critical Sections --------------------

#ਜੇ (config_ਅੰਕ_CORES > 1)
ਅਧਾਰ_Type_t __attribute__((optimize("-O3"))) x_ਪੋਰਟ_Enter_Critical_Timeout(ਪੋਰਟ_MUX_TYPE *mux, ਅਧਾਰ_Type_t timeout)
{
#ਜੇ CONFIG_FREERTOS_ਪੋਰਟ_THREAD_ਸੁਰੱਖਿਅਤ_CLAIM
    ਜੇ (unlikely(ਪੋਰਟ_x_Thread_ਸੁਰੱਖਿਅਤ_Claimed)) {
        ਵਾਪਸ pdPASS;
    }
#ਜੇ_ਅੰਤ
    /* Interrupts may already be disabled (if this function is called in nested
     * manner). However, there's no atomic operation that will allow us to check,
     * thus we have to disable interrupts again anyways.
     *
     * However, if this is call is NOT nested (i.e., the first call to enter a
     * critical section), we will save the previous interrupt level so that the
     * saved level can be restored on the last call to exit the critical.
     */
    ਅਧਾਰ_Type_t x_Old_ਰੁਕਾਵਟ_Level = ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR();
    /* Interrupts are masked, so the core id is stable. Read it once and reuse it for
     * both the spinlock owner id and the per-core critical nesting state. Interrupts
     * stay masked for the whole critical section (to the same level the spinlock would
     * use), so the spinlock does not need to disable them again. */
    ਅਧਾਰ_Type_t coreID = x_ਪੋਰਟ_Get_Core_ID();
    ਜੇ (!spinlock_acquire_impl(mux, timeout, spinlock_owner_id_for_core(coreID))) {
        //Timed out attempting to get spinlock. Restore previous interrupt level and return
        ਪੋਰਟ_CLEAR_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR(x_Old_ਰੁਕਾਵਟ_Level);
        ਵਾਪਸ pdFAIL;
    }
    //Spinlock acquired. Increment the critical nesting count.
    ਅਧਾਰ_Type_t newNesting = ਪੋਰਟ_ux_Critical_Nesting[coreID] + 1;
    ਪੋਰਟ_ux_Critical_Nesting[coreID] = newNesting;
    //If this is the first entry to a critical section. Save the old interrupt level.
    ਜੇ ( newNesting == 1 ) {
        ਪੋਰਟ_ux_Old_ਰੁਕਾਵਟ_State[coreID] = x_Old_ਰੁਕਾਵਟ_Level;
    }
    ਵਾਪਸ pdPASS;
}

ਖਾਲੀ __attribute__((optimize("-O3"))) v_ਪੋਰਟ_Exit_Critical_Multi_Core(ਪੋਰਟ_MUX_TYPE *mux)
{
#ਜੇ CONFIG_FREERTOS_ਪੋਰਟ_THREAD_ਸੁਰੱਖਿਅਤ_CLAIM
    ਜੇ (unlikely(ਪੋਰਟ_x_Thread_ਸੁਰੱਖਿਅਤ_Claimed)) {
        ਵਾਪਸ;
    }
#ਜੇ_ਅੰਤ
    /* This function may be called in a nested manner. Therefore, we only need
     * to re-enable interrupts if this is the last call to exit the critical. We
     * can use the nesting count to determine whether this is the last exit call.
     */
    /* Interrupts remain disabled for the whole critical section, so the core id is
     * stable and the spinlock does not need to mask them again. Read the core id
     * once and reuse it for both the release owner id and the per-core nesting state. */
    ਅਧਾਰ_Type_t coreID = x_ਪੋਰਟ_Get_Core_ID();
    spinlock_release_impl(mux, spinlock_owner_id_for_core(coreID));
    ਅਧਾਰ_Type_t nesting = ਪੋਰਟ_ux_Critical_Nesting[coreID];

    /* Critical section nesting count must never be negative */
    configASSERT( nesting > 0 );

    ਜੇ (nesting > 0) {
        nesting--;
        ਪੋਰਟ_ux_Critical_Nesting[coreID] = nesting;

        //This is the last exit call, restore the saved interrupt level
        ਜੇ ( nesting == 0 ) {
            ਪੋਰਟ_CLEAR_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR(ਪੋਰਟ_ux_Old_ਰੁਕਾਵਟ_State[coreID]);
        }
    }
}


ਅਧਾਰ_Type_t x_ਪੋਰਟ_Enter_Critical_Timeout_Compliance(ਪੋਰਟ_MUX_TYPE *mux, ਅਧਾਰ_Type_t timeout)
{
    ਅਧਾਰ_Type_t ret;
    ਜੇ (!x_ਪੋਰਟ_ਵਿੱਚ_Isr_Context()) {
        ret = x_ਪੋਰਟ_Enter_Critical_Timeout(mux, timeout);
    } ਨਹੀਂਤਾਂ {
        esp_rom_printf("port*_CRITICAL called from ISR context. Aborting!\n");
        abort();
        ret = pdFAIL;
    }
    ਵਾਪਸ ret;
}

ਖਾਲੀ v_ਪੋਰਟ_Exit_Critical_Compliance(ਪੋਰਟ_MUX_TYPE *mux)
{
    ਜੇ (!x_ਪੋਰਟ_ਵਿੱਚ_Isr_Context()) {
        v_ਪੋਰਟ_Exit_Critical_Multi_Core(mux);
    } ਨਹੀਂਤਾਂ {
        esp_rom_printf("port*_CRITICAL called from ISR context. Aborting!\n");
        abort();
    }
}
#ਜੇ_ਅੰਤ /* (configNUM_CORES > 1) */

#ਜੇ CONFIG_FREERTOS_ਪੋਰਟ_THREAD_ਸੁਰੱਖਿਅਤ_CLAIM
ਖਾਲੀ x_ਪੋਰਟ_Thread_ਸੁਰੱਖਿਅਤ_Claim(ਖਾਲੀ)
{
    configASSERT(!x_ਪੋਰਟ_Can_Yield());
    configASSERT(!ਪੋਰਟ_x_Thread_ਸੁਰੱਖਿਅਤ_Claimed);
    ਪੋਰਟ_x_Thread_ਸੁਰੱਖਿਅਤ_Claimed = true;
}

ਖਾਲੀ x_ਪੋਰਟ_Thread_ਸੁਰੱਖਿਅਤ_Disclaim(ਖਾਲੀ)
{
    configASSERT(!x_ਪੋਰਟ_Can_Yield());
    configASSERT(ਪੋਰਟ_x_Thread_ਸੁਰੱਖਿਅਤ_Claimed);
    ਪੋਰਟ_x_Thread_ਸੁਰੱਖਿਅਤ_Claimed = false;
}
#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

ਖਾਲੀ v_ਪੋਰਟ_Enter_Critical(ਖਾਲੀ)
{
#ਜੇ (config_ਅੰਕ_CORES > 1)
        esp_rom_printf("vPortEnterCritical(void) is not supported on multi-core targets. Please use vPortEnterCriticalMultiCore(portMUX_TYPE *mux) instead.\n");
        abort();
#ਜੇ_ਅੰਤ /* (configNUM_CORES > 1) */
#ਜੇ CONFIG_FREERTOS_ਪੋਰਟ_THREAD_ਸੁਰੱਖਿਅਤ_CLAIM
    ਜੇ (unlikely(ਪੋਰਟ_x_Thread_ਸੁਰੱਖਿਅਤ_Claimed)) {
        ਵਾਪਸ;
    }
#ਜੇ_ਅੰਤ
    ਅਧਾਰ_Type_t state = ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR();
    ਪੋਰਟ_ux_Critical_Nesting[0]++;

    ਜੇ (ਪੋਰਟ_ux_Critical_Nesting[0] == 1) {
        ਪੋਰਟ_ux_Old_ਰੁਕਾਵਟ_State[0] = state;
    }
}

ਖਾਲੀ v_ਪੋਰਟ_Exit_Critical(ਖਾਲੀ)
{
#ਜੇ (config_ਅੰਕ_CORES > 1)
        esp_rom_printf("vPortExitCritical(void) is not supported on multi-core targets. Please use vPortExitCriticalMultiCore(portMUX_TYPE *mux) instead.\n");
        abort();
#ਜੇ_ਅੰਤ /* (configNUM_CORES > 1) */
#ਜੇ CONFIG_FREERTOS_ਪੋਰਟ_THREAD_ਸੁਰੱਖਿਅਤ_CLAIM
    ਜੇ (unlikely(ਪੋਰਟ_x_Thread_ਸੁਰੱਖਿਅਤ_Claimed)) {
        ਵਾਪਸ;
    }
#ਜੇ_ਅੰਤ

    /* Critical section nesting count must never be negative */
    configASSERT( ਪੋਰਟ_ux_Critical_Nesting[0] > 0 );

    ਜੇ (ਪੋਰਟ_ux_Critical_Nesting[0] > 0) {
        ਪੋਰਟ_ux_Critical_Nesting[0]--;

        ਜੇ (ਪੋਰਟ_ux_Critical_Nesting[0] == 0) {
            ਪੋਰਟ_CLEAR_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR(ਪੋਰਟ_ux_Old_ਰੁਕਾਵਟ_State[0]);
        }
    }
}

// ---------------------- Yielding -------------------------

ਖਾਲੀ v_ਪੋਰਟ_Yield(ਖਾਲੀ)
{
    ਅਧਾਰ_Type_t coreID = x_ਪੋਰਟ_Get_Core_ID();

    ਜੇ (ਪੋਰਟ_ux_ਰੁਕਾਵਟ_Nesting[coreID]) {
        v_ਪੋਰਟ_Yield_From_ISR();
    } ਨਹੀਂਤਾਂ {
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
        ਜਦੋਂ (ਪੋਰਟ_x_Scheduler_Running[coreID] && ਪੋਰਟ_ux_Critical_Nesting[coreID] == 0 && crosscore_int_ll_get_state(coreID) != 0) {}
    }
}

ਖਾਲੀ v_ਪੋਰਟ_Yield_From_ISR( ਖਾਲੀ )
{
    trace_ISR_EXIT_ਮੰਜ਼ਿਲ_SCHEDULER();
    ਅਧਾਰ_Type_t coreID = x_ਪੋਰਟ_Get_Core_ID();
    ਪੋਰਟ_x_Scheduler_Running[coreID] = 1;
    x_ਪੋਰਟ_Switch_Flag[coreID] = 1;
}

ਖਾਲੀ v_ਪੋਰਟ_Yield_Other_Core(ਅਧਾਰ_Type_t coreid)
{
    esp_crosscore_int_send_yield(coreid);
}

// ------------------- Hook Functions ----------------------

ਖਾਲੀ __attribute__((weak)) v_Application_ਢੇਰ_ਉਛਾਲ_Hook(TaskHandle_t xTask, ਅੱਖਰ *pcTaskName)
{
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ERR_STR1 "***ERROR*** A stack overflow in task "
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ERR_STR2 " has been detected."
    ਅਚਲ ਅੱਖਰ *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    ਅੱਖਰ buf[ਆਕਾਰ(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + ਆਕਾਰ(ERR_STR2) + 1 /* null char */] = {0};

    ਅੱਖਰ *dest = buf;
    ਲਈ (ਪੂਰਨ i = 0; i < ਆਕਾਰ(str) / ਆਕਾਰ(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
    esp_system_abort(buf);
}

// ----------------------- System --------------------------

uint32_t x_ਪੋਰਟ_Get_Tick_ਦਰ_Hz(ਖਾਲੀ)
{
    ਵਾਪਸ (uint32_t)config_TICK_ਦਰ_HZ;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਢੇਰ_WATCH_AREA_ਮਾਪ 32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਢੇਰ_WATCH_ਬਿੰਦੂ_ਸੰਖਿਆ (SOC_CPU_WATCHPOINTS_ਅੰਕ - 1)

ਖਾਲੀ v_ਪੋਰਟ_ਸੈੱਟ_ਢੇਰ_Watchpoint(ਖਾਲੀ *px_ਢੇਰ_ਸ਼ੁਰੂ)
{
    uint32_t addr = (uint32_t)px_ਢੇਰ_ਸ਼ੁਰੂ;
    addr = (addr + (ਢੇਰ_WATCH_AREA_ਮਾਪ - 1)) & (~(ਢੇਰ_WATCH_AREA_ਮਾਪ - 1));
    esp_cpu_ਸੈੱਟ_watchpoint(ਢੇਰ_WATCH_ਬਿੰਦੂ_ਸੰਖਿਆ, (ਅੱਖਰ *)addr, ਢੇਰ_WATCH_AREA_ਮਾਪ, ESP_CPU_WATCHPOINT_STORE);
}

// --------------------- TCB Cleanup -----------------------

#ਜੇ ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
ਸਥਿਰ ਖਾਲੀ v_ਪੋਰਟ_TLSPointers_Del_Cb( ਖਾਲੀ *pxTCB )
{
    /* Typecast pxTCB to StaticTask_t type to access TCB struct members.
     * pvDummy15 corresponds to pvThreadLocalStoragePointers member of the TCB.
     */
    StaticTask_t *tcb = ( StaticTask_t * )pxTCB;

    /* The TLSP deletion callbacks are stored at an offset of (configNUM_THREAD_LOCAL_STORAGE_POINTERS/2) */
    TlsDeleteCallbackFunction_t *pv_Thread_Local_Storage_ਸੂਚਕ_ਸਾਰੇ_Del_Callback = ( TlsDeleteCallbackFunction_t * )( &( tcb->pvDummy15[ ( config_ਅੰਕ_THREAD_LOCAL_STORAGE_ਸੂਚਕ_ਸਾਰੇ / 2 ) ] ) );

    /* We need to iterate over half the depth of the pvThreadLocalStoragePointers area
     * to access all TLS pointers and their respective TLS deletion callbacks.
     */
    ਲਈ ( ਪੂਰਨ x = 0; x < ( config_ਅੰਕ_THREAD_LOCAL_STORAGE_ਸੂਚਕ_ਸਾਰੇ / 2 ); x++ ) {
        ਜੇ ( pv_Thread_Local_Storage_ਸੂਚਕ_ਸਾਰੇ_Del_Callback[ x ] != ਨਿਰਾ ) {  //If del cb is set
            /* In case the TLSP deletion callback has been overwritten by a TLS pointer, gracefully abort. */
            ਜੇ ( !esp_ptr_executable( pv_Thread_Local_Storage_ਸੂਚਕ_ਸਾਰੇ_Del_Callback[ x ] ) ) {
                ESP_EARLY_LOGE("FreeRTOS", "Fatal error: TLSP deletion callback at index %d overwritten with non-excutable pointer %p", x, pv_Thread_Local_Storage_ਸੂਚਕ_ਸਾਰੇ_Del_Callback[ x ]);
                abort();
            }

            pv_Thread_Local_Storage_ਸੂਚਕ_ਸਾਰੇ_Del_Callback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

ਖਾਲੀ v_ਪੋਰਟ_TCBPre_Delete_Hook( ਖਾਲੀ *pxTCB )
{
    #ਜੇ ( CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK )
        /* Call the user defined task pre-deletion hook */
        ਬਾਹਰੀ ਖਾਲੀ vTaskPreDeletionHook( ਖਾਲੀ * pxTCB );
        vTaskPreDeletionHook( pxTCB );
    #ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK */

    #ਜੇ ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
        /* Call TLS pointers deletion callbacks */
        v_ਪੋਰਟ_TLSPointers_Del_Cb( pxTCB );
    #ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

    #ਜੇ ( CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST )
        ਜੇ (pxTCB != ਨਿਰਾ) {
            v_Task_ਸੈੱਟ_Pie_Blacklisted((TaskHandle_t)pxTCB, pdFALSE);
        }
    #ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST */

    #ਜੇ ( SOC_CPU_COPROC_ਅੰਕ > 0 )
        /* Cleanup coproc save area */
        v_ਪੋਰਟ_Clean_ਉੱਪਰ_Coproc_Area( pxTCB );
    #ਜੇ_ਅੰਤ /* SOC_CPU_COPROC_NUM > 0 */
}


#ਜੇ ( SOC_CPU_COPROC_ਅੰਕ > 0 )

// ----------------------- Coprocessors --------------------------

/**
 * @brief Pin the given task to the given core
 *
 * This function is called when a task uses a coprocessor. Since the coprocessors registers
 * are saved lazily, as soon as a task starts using one, it must always be scheduled on the core
 * it is currently executing on.
 */
#ਜੇ ( config_ਅੰਕ_CORES > 1 )
ਖਾਲੀ v_ਪੋਰਟ_Task_Pin_ਮੰਜ਼ਿਲ_Core(StaticTask_t* task, ਪੂਰਨ coreid)
{
    task->xDummyCoreID = coreid;
}
#ਜੇ_ਅੰਤ /* configNUM_CORES > 1 */


/**
 * @brief Function to call to simulate an `abort()` occurring in a different context than the one it's called from.
 */
ਬਾਹਰੀ ਖਾਲੀ xt_unhandled_exception(ਖਾਲੀ *ਚੌਖਟਾ);

/**
 * @brief Get coprocessor save area out of the given task. If the coprocessor area is not created,
 *        it shall be allocated.
 *
 * @param task Task to get the coprocessor save area of
 * @param allocate When true, memory will be allocated for the coprocessor if it hasn't been allocated yet.
 *                 When false, the coprocessor memory will be left as NULL if not allocated.
 * @param coproc Coprocessor number to allocate memory for
 */
RvCoprocSaveArea* px_ਪੋਰਟ_Get_Coproc_Area(StaticTask_t* task, bool allocate, ਪੂਰਨ coproc)
{
    ਪ੍ਰਤਿਜ੍ਞਾ(coproc < SOC_CPU_COPROC_ਅੰਕ);

    ਅਚਲ UBaseType_t bottomstack = (UBaseType_t) task->pxDummy8;
    RvCoprocSaveArea* sa = px_Retrieve_Coproc_Save_Area_From_ਢੇਰ_ਸੂਚਕ(bottomstack);
    /* Check if the allocator is NULL. Since we don't have a way to get the end of the stack
     * during its initialization, we have to do this here */
    ਜੇ (sa->sa_allocator == 0) {
        /* Since the lowest stack address shall not be used as `sp` anymore, we will modify it */
        sa->sa_tcbstack = task->pxDummy6;
        sa->sa_allocator = (UBaseType_t) task->pxDummy6;
    }

    /* Check if coprocessor area is allocated */
    ਜੇ (allocate && sa->sa_coprocs[coproc] == ਨਿਰਾ) {
        ਅਚਲ uint32_t coproc_sa_sizes[] = {
            RV_COPROC0_ਮਾਪ, RV_COPROC1_ਮਾਪ, RV_COPROC2_ਮਾਪ
        };
        ਅਚਲ uint32_t coproc_sa_align[] = {
            RV_COPROC0_ALIGN, RV_COPROC1_ALIGN, RV_COPROC2_ALIGN
        };
        /* The allocator points to a usable part of the stack, use it for the coprocessor.
         * Align it up to the coprocessor save area requirement */
        sa->sa_allocator = (sa->sa_allocator + coproc_sa_align[coproc] - 1) & ~(coproc_sa_align[coproc] - 1);
        sa->sa_coprocs[coproc] = (ਖਾਲੀ*) (sa->sa_allocator);
        sa->sa_allocator += coproc_sa_sizes[coproc];
        /* Update the lowest address of the stack to prevent FreeRTOS performing overflow/watermark checks on the coprocessors contexts */
        task->pxDummy6 = (ਖਾਲੀ*) (sa->sa_allocator);
        /* Make sure the Task stack pointer is not pointing to the coprocessor context area, in other words, make
         * sure we don't have a stack overflow */
        ਖਾਲੀ* task_sp = task->pxDummy1;
        ਜੇ (task_sp <= task->pxDummy6) {
            /* In theory we need to call vApplicationStackOverflowHook to trigger the stack overflow callback,
             * but in practice, since we are already in an exception handler, this won't work, so let's manually
             * trigger an exception with the previous coprocessor owner's TCB */
            g_panic_abort = true;
            g_panic_abort_details = (ਅੱਖਰ *) "ERROR: Stack overflow while saving coprocessor context!\n";
            xt_unhandled_exception(task_sp);
        }
    }
    ਵਾਪਸ sa;
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
RvCoprocSaveArea* px_ਪੋਰਟ_Update_Coproc_Owner(ਪੂਰਨ coreid, ਪੂਰਨ coproc, StaticTask_t* owner)
{
    RvCoprocSaveArea* sa = ਨਿਰਾ;
    /* Address of coprocessor owner */
    StaticTask_t** owner_addr = &ਪੋਰਟ_ux_Coproc_Owner[ coreid ][ coproc ];
    /* Atomically exchange former owner with the new one */
    StaticTask_t* former = Atomic_ਅਦਲਾ_ਬਦਲੀ_ਸੂਚਕ_ਸਾਰੇ_p32((ਖਾਲੀ**) owner_addr, owner);
    /* Get the save area of former owner. small optimization here, if the former owner is the new owner,
     * return -1. This will simplify the assembly code while making it faster. */
    ਜੇ (former == owner) {
        sa = (ਖਾਲੀ*) -1;
    } ਨਹੀਂਤਾਂ ਜੇ (former != ਨਿਰਾ) {
        /* Allocate coprocessor memory if not available yet */
        sa = px_ਪੋਰਟ_Get_Coproc_Area(former, true, coproc);
    }
    ਵਾਪਸ sa;
}


/**
 * @brief Aborts execution when a coprocessor was used in an ISR context
 */
ਖਾਲੀ v_ਪੋਰਟ_Coproc_ਵਰਤਿਆ_ਵਿੱਚ_ISR(ਖਾਲੀ* ਚੌਖਟਾ)
{
    /* Since this function is called from an exception handler, the interrupts are disabled,
     * as such, it is not possible to trigger another exception as would `abort` do.
     * Simulate an abort without actually triggering an exception. */
    g_panic_abort = true;
    g_panic_abort_details = (ਅੱਖਰ *) "ERROR: Coprocessors must not be used in ISRs!\n";
    xt_unhandled_exception(ਚੌਖਟਾ);
}

#ਜੇ CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST
/**
 * @brief Abort if the given task is blacklisted from using the PIE coprocessor.
 *
 * Called from the PIE illegal-instruction path, so this must not trigger another
 * exception via abort().
 */
ਖਾਲੀ v_ਪੋਰਟ_Abort_If_Task_Pie_Blacklisted(StaticTask_t *task, ਖਾਲੀ *ਚੌਖਟਾ)
{
    ਜੇ (task != ਨਿਰਾ && xTaskGetPieBlacklisted((TaskHandle_t)task) != pdFALSE) {
        g_panic_abort = true;
        g_panic_abort_details = (ਅੱਖਰ *) "ERROR: Blacklisted task executed a PIE instruction!\n";
        xt_unhandled_exception(ਚੌਖਟਾ);
    }
}
#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST */

#ਜੇ CONFIG_IDF_ਨਿਸ਼ਾਨਾ_ESP32_S31
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
ਖਾਲੀ* v_ਪੋਰਟ_Task_ਵਰਤਿਆ_PIEOn_CPU0(StaticTask_t* task, ਖਾਲੀ* ਚੌਖਟਾ)
{
#ਜੇ CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST
    v_ਪੋਰਟ_Abort_If_Task_Pie_Blacklisted(task, ਚੌਖਟਾ);
#ਜੇ_ਅੰਤ
#ਜੇ CONFIG_FREERTOS_UNICORE
    g_panic_abort = true;
    g_panic_abort_details = (ਅੱਖਰ *) "ERROR: PIE coprocessor is not supported in unicore configuration!\n";
    xt_unhandled_exception(ਚੌਖਟਾ);
    ਵਾਪਸ ਨਿਰਾ;
#ਨਹੀਂਤਾਂ
    ਖਾਲੀ* context_ਮੰਜ਼ਿਲ_save = ਨਿਰਾ;
    /* Make sure we are not in an interrupt context nor in a critical section */
    ਜੇ (x_ਪੋਰਟ_ਵਿੱਚ_Isr_Context()) {
        /* We are in an interrupt context, abort */
        v_ਪੋਰਟ_Coproc_ਵਰਤਿਆ_ਵਿੱਚ_ISR(ਚੌਖਟਾ);
    }
    /* Since we count on crosscore interrupt to reschedule the current task, we must not be in a
     * critical section. In other words, we must be able to yield */
    ਜੇ (!x_ਪੋਰਟ_Can_Yield()) {
        g_panic_abort = true;
        g_panic_abort_details = (ਅੱਖਰ *) "ERROR: PIE coprocessor must not be used in critical sections!\n";
        xt_unhandled_exception(ਚੌਖਟਾ);
    }

    /* Check if the current task is the owner of the FPU on the current core. No need to make the following two instructions
     * atomic since we are in an exception context, we can't be interrupted by an interrupt. */
    ਜੇ (ਪੋਰਟ_ux_Coproc_Owner[0][FPU_COPROC_IDX] == task) {
        /* Task is the owner of the FPU on the current core, set the new owner to NULL and return the save area to fill */
        RvCoprocSaveArea* sa = px_ਪੋਰਟ_Update_Coproc_Owner(0, FPU_COPROC_IDX, ਨਿਰਾ);
        /* `sa` is not NULL here for sure */
        context_ਮੰਜ਼ਿਲ_save = sa->sa_coprocs[FPU_COPROC_IDX];
    }
    /* Migrate the task to core 1. NOTE: This will override any existing pinning of the task */
    v_ਪੋਰਟ_Task_Pin_ਮੰਜ਼ਿਲ_Core(task, 1);
    /* Raised an error if the scheduler is NOT running on core 0 */
    ਜੇ (!ਪੋਰਟ_x_Scheduler_Running[0]) {
        /* Scheduler is not running on core 0, raise an error */
        g_panic_abort = true;
        g_panic_abort_details = (ਅੱਖਰ *) "ERROR: Scheduler is not running on core 0, task must migrate to core 1!\n";
        xt_unhandled_exception(ਚੌਖਟਾ);
    }
    /* Send a cross-core interrupt on the current core, it won't be triggered until we return from the exception handler */
    esp_crosscore_int_send_yield(0);
    ਵਾਪਸ context_ਮੰਜ਼ਿਲ_save;
#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_UNICORE */
}

#ਜੇ_ਅੰਤ /* CONFIG_IDF_TARGET_ESP32S31 */


#ਜੇ_ਅੰਤ /* SOC_CPU_COPROC_NUM > 0 */

/* ------------------------------------------------ Run Time Stats ------------------------------------------------- */

#ਜੇ ( CONFIG_FREERTOS_GENERATE_ਲੜੀਵਾਰ_ਸਮਾਂ_STATS )

config_ਲੜੀਵਾਰ_ਸਮਾਂ_COUNTER_TYPE x_ਪੋਰਟ_Get_ਲੜੀਵਾਰ_ਸਮਾਂ_Counter_ਮੁੱਲ( ਖਾਲੀ )
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FREERTOS_ਲੜੀਵਾਰ_ਸਮਾਂ_STATS_USING_ESP_TIMER
    ਵਾਪਸ (config_ਲੜੀਵਾਰ_ਸਮਾਂ_COUNTER_TYPE) esp_timer_get_ਸਮਾਂ();
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ 0;
#ਜੇ_ਅੰਤ // CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
}

#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */

/* ---------------------------------------------- Misc Implementations -------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */
#ਜੇ (SOC_CPU_COPROC_ਅੰਕ > 0) && SOC_CPU_HAS_FPU && SOC_PM_FPU_RETENTION_BY_SW
ਅਧਾਰ_Type_t x_ਪੋਰਟ_FPUContext_Is_Dirty(ਅਧਾਰ_Type_t core_id)
{
    ਵਾਪਸ (ਅਧਾਰ_Type_t)(ਪੋਰਟ_ux_Coproc_Owner[core_id][FPU_COPROC_IDX] != ਨਿਰਾ);
}
#ਜੇ_ਅੰਤ /* (SOC_CPU_COPROC_NUM > 0) && SOC_CPU_HAS_FPU && SOC_PM_FPU_RETENTION_BY_SW */
