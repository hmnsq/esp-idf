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

#شامل "sdkconfig.h"
#شامل <stdbool.h>
#شامل <string.h>
#شامل "soc/soc_caps.h"
#شامل "soc/periph_defs.h"
#شامل "soc/system_reg.h"
#شامل "soc/interrupt_reg.h"
#شامل "hal/systimer_hal.h"
#شامل "hal/systimer_ll.h"
#شامل "riscv/rvruntime-frames.h"
#شامل "riscv/rv_utils.h"
#شامل "riscv/interrupt.h"
#شامل "esp_private/crosscore_int.h"
#شامل "hal/crosscore_int_ll.h"
#شامل "esp_attr.h"
#شامل "esp_compiler.h"
#شامل "esp_system.h"
#شامل "esp_intr_alloc.h"
#شامل "esp_log.h"
#شامل "FreeRTOS.h"       /* This pulls in portmacro.h */
#شامل "task.h"
#اگر CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST
#شامل "freertos/idf_additions.h"
#endif
#شامل "portmacro.h"
#شامل "port_systick.h"
#شامل "esp_memory_utils.h"
#شامل "esp_macros.h"
#اگر CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
#شامل "esp_timer.h"
#endif

#اگر SOC_CPU_HAS_HWLOOP
#شامل "riscv/csr.h"
#شامل "riscv/csr_hwlp.h"
#endif

#اگر ( SOC_CPU_COPROC_رقم > 0 )

#شامل "esp_private/panic_internal.h"

/* Since `portFORCE_INLINE` is not defined in `portmacro.h`, we must define it here since it is
 * used by `atomic.h`. */
#define portFORCE_INLINE    درونخطی
#شامل "freertos/atomic.h"

#endif // ( SOC_CPU_COPROC_NUM > 0 )


_Static_assert(portBYTE_ALIGNMENT == 16, "portBYTE_ALIGNMENT must be set to 16");
#اگر CONFIG_ESP_SYSTEM_HW_پشته_GUARD
/**
 * offsetof() can not be used in asm code. Then we need make sure that
 * PORT_OFFSET_PX_STACK and PORT_OFFSET_PX_END_OF_STACK have expected values.
 * Macro used in the portasm.S instead of variables to save at least 4 instruction calls
 * which accessing DRAM memory. This optimization saves CPU time in the interrupt handling.
 */

_Static_assert(offsetof( StaticTask_t, pxDummy6 ) == PORT_OFFSET_PX_پشته);
_Static_assert(offsetof( StaticTask_t, pxDummy8 ) == PORT_OFFSET_PX_END_OF_پشته);
#endif // CONFIG_ESP_SYSTEM_HW_STACK_GUARD

/* ---------------------------------------------------- Variables ------------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */

فرار UBaseType_t port_xSchedulerRunning[port_رقم_PROCESSORS] = {0}; // Indicates whether scheduler is running on a per-core basis
فرار UBaseType_t port_uxInterruptNesting[port_رقم_PROCESSORS] = {0};  // Interrupt nesting level. Increased/decreased in portasm.c
فرار UBaseType_t port_uxCriticalNesting[port_رقم_PROCESSORS] = {0};
#اگر CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
فرار bool port_xThreadSafeClaimed = false;
#endif
فرار UBaseType_t port_uxOldInterruptState[port_رقم_PROCESSORS] = {0};
فرار UBaseType_t xPortSwitchFlag[port_رقم_PROCESSORS] = {0};
فرار UBaseType_t port_uxCoreStartupDone[port_رقم_PROCESSORS] = {0};  // Indicates whether the core has completed its startup sequence

#اگر ( SOC_CPU_COPROC_رقم > 0 )

/* Current owner of the coprocessors for each core */
StaticTask_t* port_uxCoprocOwner[port_رقم_PROCESSORS][SOC_CPU_COPROC_رقم];

#endif /* SOC_CPU_COPROC_NUM > 0 */

/*
*******************************************************************************
* Interrupt stack. The size of the interrupt stack is determined by the config
* parameter "configISR_STACK_SIZE" in FreeRTOSConfig.h
*******************************************************************************
*/
__attribute__((aligned(16))) پشته_Type_t x_Isr_پشته[port_رقم_PROCESSORS][config_ISR_پشته_SIZE];
پشته_Type_t *x_Isr_پشته_بالاترین[port_رقم_PROCESSORS] = {0};

#اگر CONFIG_ESP_SYSTEM_HW_پشته_GUARD
پشته_Type_t *x_Isr_پشته_Bottom[port_رقم_PROCESSORS] = {0};
#endif

/* ------------------------------------------------ FreeRTOS Portable --------------------------------------------------
 * - Provides implementation for functions required by FreeRTOS
 * - Declared in portable.h
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------- Scheduler Start/End -------------------

BaseType_t xPortStartScheduler(تهی)
{
#اگر ( SOC_CPU_COPROC_رقم > 0 )

#اگر SOC_CPU_HAS_FPU
    /* Disable FPU so that the first task to use it will trigger an exception */
    rv_utils_disable_fpu();
#endif /* SOC_CPU_HAS_FPU */

#اگر SOC_CPU_HAS_PIE
    /* Similarly, disable PIE */
    rv_utils_disable_pie();
#endif /* SOC_CPU_HAS_PIE */

#اگر SOC_CPU_HAS_DSP
    rv_utils_disable_dsp();
#endif /* SOC_CPU_HAS_DSP */

#اگر SOC_CPU_HAS_HWLOOP
    /* Initialize the Hardware loop feature */
    RV_WRITE_CSR(CSR_HWLP_STATE_REG, HWLP_INITIAL_STATE);
#endif /* SOC_CPU_HAS_HWLOOP */
#endif /* ( SOC_CPU_COPROC_NUM > 0 ) */

    /* Initialize all kernel state tracking variables */
    BaseType_t coreID = xPortGetCoreID();
    port_uxInterruptNesting[coreID] = 0;
    port_uxCriticalNesting[coreID] = 0;
#اگر CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
    port_xThreadSafeClaimed = false;
#endif
    port_xSchedulerRunning[coreID] = 0;
    port_uxCoreStartupDone[coreID] = 0;

    /* Initialize ISR Stack(s) */
    برای (صحیح i = 0; i < port_رقم_PROCESSORS; i++) {
        x_Isr_پشته_بالاترین[i] = &x_Isr_پشته[i][0] + (config_ISR_پشته_SIZE & (~((portPOINTER_SIZE_TYPE)portBYTE_ALIGNMENT_MASK)));
#اگر CONFIG_ESP_SYSTEM_HW_پشته_GUARD
        x_Isr_پشته_Bottom[i] = &x_Isr_پشته[i][0];
#endif
    }

    /* Setup the hardware to generate the tick. */
    vPortSetupTimer();

    rv_utils_intr_global_enable();
    esprv_int_set_threshold(RVHAL_INTR_ENABLE_THRESH); /* set global interrupt masking level */

    vPortYield();

    /* Should not get here */
    بازگشت pdTRUE;
}

تهی vPortEndScheduler(تهی)
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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_پشته_TLS(UBaseType_t ux_پشته_Pointer, uint32_t *ret_threadptr_reg_init)
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
    بیرونی نویسه _thread_local_data_start, _thread_local_data_end;
    بیرونی نویسه _thread_local_bss_start, _thread_local_bss_end;
    ثابت uint32_t tls_data_size = (uint32_t)&_thread_local_data_end - (uint32_t)&_thread_local_data_start;
    ثابت uint32_t tls_bss_size = (uint32_t)&_thread_local_bss_end - (uint32_t)&_thread_local_bss_start;
    ثابت uint32_t tls_area_size = ESP_ALIGN_UP(tls_data_size + tls_bss_size, 16);
    // TODO: check that TLS area fits the stack

    // Allocate space for the TLS area on the stack. The area must be aligned to 16-bytes
    ux_پشته_Pointer = ESP_ALIGN_DOWN(ux_پشته_Pointer - (UBaseType_t)tls_area_size, 16);
    // Initialize the TLS data with the initialization values of each TLS variable
    memcpy((تهی *)ux_پشته_Pointer, &_thread_local_data_start, tls_data_size);
    // Initialize the TLS bss with zeroes
    memset((تهی *)(ux_پشته_Pointer + tls_data_size), 0, tls_bss_size);

    // Save tls start address
    *ret_threadptr_reg_init = (uint32_t)ux_پشته_Pointer;
    بازگشت ux_پشته_Pointer;
}

#اگر CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
ایستا تهی vPortTaskWrapper(TaskFunction_t pxCode, تهی *pvParameters)
{
    __asm__ فرار(".cfi_undefined ra");  // tell to debugger that it's outermost (initial) frame
    بیرونی تهی __attribute__((noreturn)) panic_abort(ثابت نویسه *details);
    ایستا نویسه DRAM_ATTR پیام[80] = "FreeRTOS: FreeRTOS Task \"\0";
    pxCode(pvParameters);
    /* FreeRTOS tasks should not return. Log the task name and abort. */
    /* We cannot use s(n)printf because it is in flash */
    strcat(پیام, pcTaskGetName(پوچ));
    strcat(پیام, "\" should not return, Aborting now!");
    panic_abort(پیام);
}
#endif // CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER


#اگر ( SOC_CPU_COPROC_رقم > 0 )

/**
 * @brief Retrieve or allocate coprocessors save area from the given pxTopOfStack address.
 *
 * @param pxTopOfStack End of the stack address. This represents the highest address of a Task's stack.
 */
FORCE_INLINE_ATTR RvCoprocSaveArea* px_Retrieve_Coproc_Save_Area_From_پشته_Pointer(UBaseType_t px_بالاترین_Of_پشته)
{
    بازگشت (RvCoprocSaveArea*) ESP_ALIGN_DOWN(px_بالاترین_Of_پشته - اندازه(RvCoprocSaveArea), 16);
}

/**
 * @brief Allocate and initialize the coprocessors save area on the stack
 *
 * @param[in] uxStackPointer Current stack pointer address
 *
 * @return Stack pointer that points to allocated and initialized the coprocessor save area
 */
FORCE_INLINE_ATTR UBaseType_t uxInitialiseCoprocSaveArea(UBaseType_t ux_پشته_Pointer)
{
    RvCoprocSaveArea* sa = px_Retrieve_Coproc_Save_Area_From_پشته_Pointer(ux_پشته_Pointer);
    memset(sa, 0, اندازه(RvCoprocSaveArea));
    بازگشت (UBaseType_t) sa;
}


ایستا تهی vPortCleanUpCoprocArea(تهی *pvTCB)
{
    StaticTask_t* task = (StaticTask_t*) pvTCB;

    /* Get a pointer to the task's coprocessor save area */
    ثابت UBaseType_t bottomstack = (UBaseType_t) task->pxDummy8;
    RvCoprocSaveArea* sa = px_Retrieve_Coproc_Save_Area_From_پشته_Pointer(bottomstack);

    /* If the Task ever saved the original stack pointer, restore it before returning */
    اگر (sa->sa_allocator != 0) {
        /* Restore the original lowest address of the stack in the TCB */
        task->pxDummy6 = sa->sa_tcbstack;

        /* Get the core the task is pinned on */
        #اگر ( config_رقم_CORES > 1 )
            ثابت BaseType_t coreID = task->xDummyCoreID;
            /* If the task is not pinned on any core, it didn't use any coprocessor than need to be freed (FPU or PIE).
             * If it used the HWLP coprocessor, it has nothing to clear since there is no "owner" for it. */
            اگر (coreID == tskNO_AFFINITY) {
                بازگشت;
            }
        #وگرنه /* configNUM_CORES > 1 */
            ثابت BaseType_t coreID = 0;
        #endif /* configNUM_CORES > 1 */

        برای (صحیح i = 0; i < SOC_CPU_COPROC_رقم; i++) {
            StaticTask_t** owner = &port_uxCoprocOwner[coreID][i];
            /* If the owner is `task`, replace it with NULL atomically */
            Atomic_CompareAndSwapPointers_p32((تهی**) owner, پوچ, task);
        }
    }
}
#endif /* SOC_CPU_COPROC_NUM > 0 */


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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_پشته_Frame(UBaseType_t ux_پشته_Pointer, TaskFunction_t pxCode, تهی *pvParameters, uint32_t threadptr_reg_init)
{
    /*
    Allocate space for the task's starting interrupt stack frame.
    - The stack frame must be allocated to a 16-byte aligned address.
    - We use RV_STK_FRMSZ as it rounds up the total size to a multiple of 16.
    */
    ux_پشته_Pointer = ESP_ALIGN_DOWN(ux_پشته_Pointer - RV_STK_FRMSZ, 16);

    // Clear the entire interrupt stack frame
    RvExcFrame *frame = (RvExcFrame *)ux_پشته_Pointer;
    memset(frame, 0, اندازه(RvExcFrame));

    /* Initialize the stack frame. */
    بیرونی uint32_t __global_pointer$;
    #اگر CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
        frame->mepc = (UBaseType_t)vPortTaskWrapper;
        frame->a0 = (UBaseType_t)pxCode;
        frame->a1 = (UBaseType_t)pvParameters;
    #وگرنه
        frame->mepc = (UBaseType_t)pxCode;
        frame->a0 = (UBaseType_t)pvParameters;
    #endif // CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
    frame->gp = (UBaseType_t)&__global_pointer$;
    frame->tp = (UBaseType_t)threadptr_reg_init;

    بازگشت ux_پشته_Pointer;
}

پشته_Type_t *px_Port_Initialise_پشته(پشته_Type_t *px_بالاترین_Of_پشته, TaskFunction_t pxCode, تهی *pvParameters)
{
#ifdef __clang_analyzer__
    // Teach clang-tidy that pxTopOfStack cannot be a pointer to const
    فرار پشته_Type_t * pxTemp = px_بالاترین_Of_پشته;
    px_بالاترین_Of_پشته = pxTemp;
#endif /*__clang_analyzer__ */
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

    UBaseType_t ux_پشته_Pointer = (UBaseType_t)px_بالاترین_Of_پشته;
    configASSERT((ux_پشته_Pointer & portBYTE_ALIGNMENT_MASK) == 0);

#اگر ( SOC_CPU_COPROC_رقم > 0 )
    // Initialize the coprocessors save area
    ux_پشته_Pointer = uxInitialiseCoprocSaveArea(ux_پشته_Pointer);
    configASSERT((ux_پشته_Pointer & portBYTE_ALIGNMENT_MASK) == 0);
#endif // SOC_CPU_COPROC_NUM > 0

    // Initialize GCC TLS area
    uint32_t threadptr_reg_init;
    ux_پشته_Pointer = ux_Initialise_پشته_TLS(ux_پشته_Pointer, &threadptr_reg_init);
    configASSERT((ux_پشته_Pointer & portBYTE_ALIGNMENT_MASK) == 0);

    // Initialize the starting interrupt stack frame
    ux_پشته_Pointer = ux_Initialise_پشته_Frame(ux_پشته_Pointer, pxCode, pvParameters, threadptr_reg_init);
    configASSERT((ux_پشته_Pointer & portBYTE_ALIGNMENT_MASK) == 0);

    // Return the task's current stack pointer address which should point to the starting interrupt stack frame
    بازگشت (پشته_Type_t *)ux_پشته_Pointer;
}



/* ---------------------------------------------- Port Implementations -------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

BaseType_t xPortInIsrContext(تهی)
{
#اگر (config_رقم_CORES > 1)
    unsigned صحیح irqStatus;
    BaseType_t ret;

    /* Disable interrupts to fetch the coreID atomically */
    irqStatus = portSET_INTERRUPT_MASK_FROM_ISR();

    /* Return the interrupt nesting counter for this core */
    ret = port_uxInterruptNesting[xPortGetCoreID()];

    /* Restore interrupts */
    portCLEAR_INTERRUPT_MASK_FROM_ISR(irqStatus);

    بازگشت ret;
#وگرنه
    /* Optimize the call for single-core targets */
    بازگشت port_uxInterruptNesting[0];
#endif /* (configNUM_CORES > 1) */
}

تهی vPortAssertIfInISR(تهی)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(xPortInIsrContext() == 0);
}

BaseType_t xPortInterruptedFromISRContext(تهی)
{
    /* Return the interrupt nesting counter for this core */
    بازگشت port_uxInterruptNesting[xPortGetCoreID()];
}

UBaseType_t xPortSetInterruptMaskFromISR(تهی)
{
    UBaseType_t prev_int_level = 0, int_level = 0;
#اگر !SOC_INT_CLIC_SUPPORTED
    int_level = RVHAL_EXCM_LEVEL;
#وگرنه
    int_level = RVHAL_EXCM_LEVEL_CLIC;
#endif

    prev_int_level = rv_utils_set_intlevel_regval(int_level);
    بازگشت prev_int_level;
}

تهی vPortClearInterruptMaskFromISR(UBaseType_t prev_int_level)
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
    asm فرار ( "nop" );
    asm فرار ( "nop" );
    asm فرار ( "nop" );
}

// ------------------ Critical Sections --------------------

#اگر (config_رقم_CORES > 1)
BaseType_t __attribute__((optimize("-O3"))) xPortEnterCriticalTimeout(portMUX_TYPE *mux, BaseType_t timeout)
{
#اگر CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
    اگر (unlikely(port_xThreadSafeClaimed)) {
        بازگشت pdPASS;
    }
#endif
    /* Interrupts may already be disabled (if this function is called in nested
     * manner). However, there's no atomic operation that will allow us to check,
     * thus we have to disable interrupts again anyways.
     *
     * However, if this is call is NOT nested (i.e., the first call to enter a
     * critical section), we will save the previous interrupt level so that the
     * saved level can be restored on the last call to exit the critical.
     */
    BaseType_t xOldInterruptLevel = portSET_INTERRUPT_MASK_FROM_ISR();
    /* Interrupts are masked, so the core id is stable. Read it once and reuse it for
     * both the spinlock owner id and the per-core critical nesting state. Interrupts
     * stay masked for the whole critical section (to the same level the spinlock would
     * use), so the spinlock does not need to disable them again. */
    BaseType_t coreID = xPortGetCoreID();
    اگر (!spinlock_acquire_impl(mux, timeout, spinlock_owner_id_for_core(coreID))) {
        //Timed out attempting to get spinlock. Restore previous interrupt level and return
        portCLEAR_INTERRUPT_MASK_FROM_ISR(xOldInterruptLevel);
        بازگشت pdFAIL;
    }
    //Spinlock acquired. Increment the critical nesting count.
    BaseType_t newNesting = port_uxCriticalNesting[coreID] + 1;
    port_uxCriticalNesting[coreID] = newNesting;
    //If this is the first entry to a critical section. Save the old interrupt level.
    اگر ( newNesting == 1 ) {
        port_uxOldInterruptState[coreID] = xOldInterruptLevel;
    }
    بازگشت pdPASS;
}

تهی __attribute__((optimize("-O3"))) vPortExitCriticalMultiCore(portMUX_TYPE *mux)
{
#اگر CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
    اگر (unlikely(port_xThreadSafeClaimed)) {
        بازگشت;
    }
#endif
    /* This function may be called in a nested manner. Therefore, we only need
     * to re-enable interrupts if this is the last call to exit the critical. We
     * can use the nesting count to determine whether this is the last exit call.
     */
    /* Interrupts remain disabled for the whole critical section, so the core id is
     * stable and the spinlock does not need to mask them again. Read the core id
     * once and reuse it for both the release owner id and the per-core nesting state. */
    BaseType_t coreID = xPortGetCoreID();
    spinlock_release_impl(mux, spinlock_owner_id_for_core(coreID));
    BaseType_t nesting = port_uxCriticalNesting[coreID];

    /* Critical section nesting count must never be negative */
    configASSERT( nesting > 0 );

    اگر (nesting > 0) {
        nesting--;
        port_uxCriticalNesting[coreID] = nesting;

        //This is the last exit call, restore the saved interrupt level
        اگر ( nesting == 0 ) {
            portCLEAR_INTERRUPT_MASK_FROM_ISR(port_uxOldInterruptState[coreID]);
        }
    }
}


BaseType_t xPortEnterCriticalTimeoutCompliance(portMUX_TYPE *mux, BaseType_t timeout)
{
    BaseType_t ret;
    اگر (!xPortInIsrContext()) {
        ret = xPortEnterCriticalTimeout(mux, timeout);
    } وگرنه {
        esp_rom_printf("port*_CRITICAL called from ISR context. Aborting!\n");
        abort();
        ret = pdFAIL;
    }
    بازگشت ret;
}

تهی vPortExitCriticalCompliance(portMUX_TYPE *mux)
{
    اگر (!xPortInIsrContext()) {
        vPortExitCriticalMultiCore(mux);
    } وگرنه {
        esp_rom_printf("port*_CRITICAL called from ISR context. Aborting!\n");
        abort();
    }
}
#endif /* (configNUM_CORES > 1) */

#اگر CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
تهی xPortThreadSafeClaim(تهی)
{
    configASSERT(!xPortCanYield());
    configASSERT(!port_xThreadSafeClaimed);
    port_xThreadSafeClaimed = true;
}

تهی xPortThreadSafeDisclaim(تهی)
{
    configASSERT(!xPortCanYield());
    configASSERT(port_xThreadSafeClaimed);
    port_xThreadSafeClaimed = false;
}
#endif /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

تهی vPortEnterCritical(تهی)
{
#اگر (config_رقم_CORES > 1)
        esp_rom_printf("vPortEnterCritical(void) is not supported on multi-core targets. Please use vPortEnterCriticalMultiCore(portMUX_TYPE *mux) instead.\n");
        abort();
#endif /* (configNUM_CORES > 1) */
#اگر CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
    اگر (unlikely(port_xThreadSafeClaimed)) {
        بازگشت;
    }
#endif
    BaseType_t state = portSET_INTERRUPT_MASK_FROM_ISR();
    port_uxCriticalNesting[0]++;

    اگر (port_uxCriticalNesting[0] == 1) {
        port_uxOldInterruptState[0] = state;
    }
}

تهی vPortExitCritical(تهی)
{
#اگر (config_رقم_CORES > 1)
        esp_rom_printf("vPortExitCritical(void) is not supported on multi-core targets. Please use vPortExitCriticalMultiCore(portMUX_TYPE *mux) instead.\n");
        abort();
#endif /* (configNUM_CORES > 1) */
#اگر CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM
    اگر (unlikely(port_xThreadSafeClaimed)) {
        بازگشت;
    }
#endif

    /* Critical section nesting count must never be negative */
    configASSERT( port_uxCriticalNesting[0] > 0 );

    اگر (port_uxCriticalNesting[0] > 0) {
        port_uxCriticalNesting[0]--;

        اگر (port_uxCriticalNesting[0] == 0) {
            portCLEAR_INTERRUPT_MASK_FROM_ISR(port_uxOldInterruptState[0]);
        }
    }
}

// ---------------------- Yielding -------------------------

تهی vPortYield(تهی)
{
    BaseType_t coreID = xPortGetCoreID();

    اگر (port_uxInterruptNesting[coreID]) {
        vPortYieldFromISR();
    } وگرنه {
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
        وقتی (port_xSchedulerRunning[coreID] && port_uxCriticalNesting[coreID] == 0 && crosscore_int_ll_get_state(coreID) != 0) {}
    }
}

تهی vPortYieldFromISR( تهی )
{
    traceISR_EXIT_TO_SCHEDULER();
    BaseType_t coreID = xPortGetCoreID();
    port_xSchedulerRunning[coreID] = 1;
    xPortSwitchFlag[coreID] = 1;
}

تهی vPortYieldOtherCore(BaseType_t coreid)
{
    esp_crosscore_int_send_yield(coreid);
}

// ------------------- Hook Functions ----------------------

تهی __attribute__((weak)) v_Application_پشته_Overflow_Hook(TaskHandle_t xTask, نویسه *pcTaskName)
{
#define ERR_STR1 "***ERROR*** A stack overflow in task "
#define ERR_STR2 " has been detected."
    ثابت نویسه *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    نویسه buf[اندازه(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + اندازه(ERR_STR2) + 1 /* null char */] = {0};

    نویسه *dest = buf;
    برای (صحیح i = 0; i < اندازه(str) / اندازه(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
    esp_system_abort(buf);
}

// ----------------------- System --------------------------

uint32_t xPortGetTickRateHz(تهی)
{
    بازگشت (uint32_t)configTICK_RATE_HZ;
}

#define پشته_WATCH_AREA_SIZE 32
#define پشته_WATCH_POINT_عدد (SOC_CPU_WATCHPOINTS_رقم - 1)

تهی v_Port_Set_پشته_Watchpoint(تهی *px_پشته_Start)
{
    uint32_t addr = (uint32_t)px_پشته_Start;
    addr = (addr + (پشته_WATCH_AREA_SIZE - 1)) & (~(پشته_WATCH_AREA_SIZE - 1));
    esp_cpu_set_watchpoint(پشته_WATCH_POINT_عدد, (نویسه *)addr, پشته_WATCH_AREA_SIZE, ESP_CPU_WATCHPOINT_STORE);
}

// --------------------- TCB Cleanup -----------------------

#اگر ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
ایستا تهی vPortTLSPointersDelCb( تهی *pxTCB )
{
    /* Typecast pxTCB to StaticTask_t type to access TCB struct members.
     * pvDummy15 corresponds to pvThreadLocalStoragePointers member of the TCB.
     */
    StaticTask_t *tcb = ( StaticTask_t * )pxTCB;

    /* The TLSP deletion callbacks are stored at an offset of (configNUM_THREAD_LOCAL_STORAGE_POINTERS/2) */
    TlsDeleteCallbackFunction_t *pvThreadLocalStoragePointersDelCallback = ( TlsDeleteCallbackFunction_t * )( &( tcb->pvDummy15[ ( config_رقم_THREAD_LOCAL_STORAGE_POINTERS / 2 ) ] ) );

    /* We need to iterate over half the depth of the pvThreadLocalStoragePointers area
     * to access all TLS pointers and their respective TLS deletion callbacks.
     */
    برای ( صحیح x = 0; x < ( config_رقم_THREAD_LOCAL_STORAGE_POINTERS / 2 ); x++ ) {
        اگر ( pvThreadLocalStoragePointersDelCallback[ x ] != پوچ ) {  //If del cb is set
            /* In case the TLSP deletion callback has been overwritten by a TLS pointer, gracefully abort. */
            اگر ( !esp_ptr_executable( pvThreadLocalStoragePointersDelCallback[ x ] ) ) {
                ESP_EARLY_LOGE("FreeRTOS", "Fatal error: TLSP deletion callback at index %d overwritten with non-excutable pointer %p", x, pvThreadLocalStoragePointersDelCallback[ x ]);
                abort();
            }

            pvThreadLocalStoragePointersDelCallback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#endif /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

تهی vPortTCBPreDeleteHook( تهی *pxTCB )
{
    #اگر ( CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK )
        /* Call the user defined task pre-deletion hook */
        بیرونی تهی vTaskPreDeletionHook( تهی * pxTCB );
        vTaskPreDeletionHook( pxTCB );
    #endif /* CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK */

    #اگر ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
        /* Call TLS pointers deletion callbacks */
        vPortTLSPointersDelCb( pxTCB );
    #endif /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

    #اگر ( CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST )
        اگر (pxTCB != پوچ) {
            vTaskSetPieBlacklisted((TaskHandle_t)pxTCB, pdFALSE);
        }
    #endif /* CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST */

    #اگر ( SOC_CPU_COPROC_رقم > 0 )
        /* Cleanup coproc save area */
        vPortCleanUpCoprocArea( pxTCB );
    #endif /* SOC_CPU_COPROC_NUM > 0 */
}


#اگر ( SOC_CPU_COPROC_رقم > 0 )

// ----------------------- Coprocessors --------------------------

/**
 * @brief Pin the given task to the given core
 *
 * This function is called when a task uses a coprocessor. Since the coprocessors registers
 * are saved lazily, as soon as a task starts using one, it must always be scheduled on the core
 * it is currently executing on.
 */
#اگر ( config_رقم_CORES > 1 )
تهی vPortTaskPinToCore(StaticTask_t* task, صحیح coreid)
{
    task->xDummyCoreID = coreid;
}
#endif /* configNUM_CORES > 1 */


/**
 * @brief Function to call to simulate an `abort()` occurring in a different context than the one it's called from.
 */
بیرونی تهی xt_unhandled_exception(تهی *frame);

/**
 * @brief Get coprocessor save area out of the given task. If the coprocessor area is not created,
 *        it shall be allocated.
 *
 * @param task Task to get the coprocessor save area of
 * @param allocate When true, memory will be allocated for the coprocessor if it hasn't been allocated yet.
 *                 When false, the coprocessor memory will be left as NULL if not allocated.
 * @param coproc Coprocessor number to allocate memory for
 */
RvCoprocSaveArea* pxPortGetCoprocArea(StaticTask_t* task, bool allocate, صحیح coproc)
{
    أكد(coproc < SOC_CPU_COPROC_رقم);

    ثابت UBaseType_t bottomstack = (UBaseType_t) task->pxDummy8;
    RvCoprocSaveArea* sa = px_Retrieve_Coproc_Save_Area_From_پشته_Pointer(bottomstack);
    /* Check if the allocator is NULL. Since we don't have a way to get the end of the stack
     * during its initialization, we have to do this here */
    اگر (sa->sa_allocator == 0) {
        /* Since the lowest stack address shall not be used as `sp` anymore, we will modify it */
        sa->sa_tcbstack = task->pxDummy6;
        sa->sa_allocator = (UBaseType_t) task->pxDummy6;
    }

    /* Check if coprocessor area is allocated */
    اگر (allocate && sa->sa_coprocs[coproc] == پوچ) {
        ثابت uint32_t coproc_sa_sizes[] = {
            RV_COPROC0_SIZE, RV_COPROC1_SIZE, RV_COPROC2_SIZE
        };
        ثابت uint32_t coproc_sa_align[] = {
            RV_COPROC0_ALIGN, RV_COPROC1_ALIGN, RV_COPROC2_ALIGN
        };
        /* The allocator points to a usable part of the stack, use it for the coprocessor.
         * Align it up to the coprocessor save area requirement */
        sa->sa_allocator = (sa->sa_allocator + coproc_sa_align[coproc] - 1) & ~(coproc_sa_align[coproc] - 1);
        sa->sa_coprocs[coproc] = (تهی*) (sa->sa_allocator);
        sa->sa_allocator += coproc_sa_sizes[coproc];
        /* Update the lowest address of the stack to prevent FreeRTOS performing overflow/watermark checks on the coprocessors contexts */
        task->pxDummy6 = (تهی*) (sa->sa_allocator);
        /* Make sure the Task stack pointer is not pointing to the coprocessor context area, in other words, make
         * sure we don't have a stack overflow */
        تهی* task_sp = task->pxDummy1;
        اگر (task_sp <= task->pxDummy6) {
            /* In theory we need to call vApplicationStackOverflowHook to trigger the stack overflow callback,
             * but in practice, since we are already in an exception handler, this won't work, so let's manually
             * trigger an exception with the previous coprocessor owner's TCB */
            g_panic_abort = true;
            g_panic_abort_details = (نویسه *) "ERROR: Stack overflow while saving coprocessor context!\n";
            xt_unhandled_exception(task_sp);
        }
    }
    بازگشت sa;
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
RvCoprocSaveArea* pxPortUpdateCoprocOwner(صحیح coreid, صحیح coproc, StaticTask_t* owner)
{
    RvCoprocSaveArea* sa = پوچ;
    /* Address of coprocessor owner */
    StaticTask_t** owner_addr = &port_uxCoprocOwner[ coreid ][ coproc ];
    /* Atomically exchange former owner with the new one */
    StaticTask_t* former = Atomic_SwapPointers_p32((تهی**) owner_addr, owner);
    /* Get the save area of former owner. small optimization here, if the former owner is the new owner,
     * return -1. This will simplify the assembly code while making it faster. */
    اگر (former == owner) {
        sa = (تهی*) -1;
    } وگرنه اگر (former != پوچ) {
        /* Allocate coprocessor memory if not available yet */
        sa = pxPortGetCoprocArea(former, true, coproc);
    }
    بازگشت sa;
}


/**
 * @brief Aborts execution when a coprocessor was used in an ISR context
 */
تهی vPortCoprocUsedInISR(تهی* frame)
{
    /* Since this function is called from an exception handler, the interrupts are disabled,
     * as such, it is not possible to trigger another exception as would `abort` do.
     * Simulate an abort without actually triggering an exception. */
    g_panic_abort = true;
    g_panic_abort_details = (نویسه *) "ERROR: Coprocessors must not be used in ISRs!\n";
    xt_unhandled_exception(frame);
}

#اگر CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST
/**
 * @brief Abort if the given task is blacklisted from using the PIE coprocessor.
 *
 * Called from the PIE illegal-instruction path, so this must not trigger another
 * exception via abort().
 */
تهی vPortAbortIfTaskPieBlacklisted(StaticTask_t *task, تهی *frame)
{
    اگر (task != پوچ && xTaskGetPieBlacklisted((TaskHandle_t)task) != pdFALSE) {
        g_panic_abort = true;
        g_panic_abort_details = (نویسه *) "ERROR: Blacklisted task executed a PIE instruction!\n";
        xt_unhandled_exception(frame);
    }
}
#endif /* CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST */

#اگر CONFIG_IDF_TARGET_ESP32S31
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
تهی* vPortTaskUsedPIEOnCPU0(StaticTask_t* task, تهی* frame)
{
#اگر CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST
    vPortAbortIfTaskPieBlacklisted(task, frame);
#endif
#اگر CONFIG_FREERTOS_UNICORE
    g_panic_abort = true;
    g_panic_abort_details = (نویسه *) "ERROR: PIE coprocessor is not supported in unicore configuration!\n";
    xt_unhandled_exception(frame);
    بازگشت پوچ;
#وگرنه
    تهی* context_to_save = پوچ;
    /* Make sure we are not in an interrupt context nor in a critical section */
    اگر (xPortInIsrContext()) {
        /* We are in an interrupt context, abort */
        vPortCoprocUsedInISR(frame);
    }
    /* Since we count on crosscore interrupt to reschedule the current task, we must not be in a
     * critical section. In other words, we must be able to yield */
    اگر (!xPortCanYield()) {
        g_panic_abort = true;
        g_panic_abort_details = (نویسه *) "ERROR: PIE coprocessor must not be used in critical sections!\n";
        xt_unhandled_exception(frame);
    }

    /* Check if the current task is the owner of the FPU on the current core. No need to make the following two instructions
     * atomic since we are in an exception context, we can't be interrupted by an interrupt. */
    اگر (port_uxCoprocOwner[0][FPU_COPROC_IDX] == task) {
        /* Task is the owner of the FPU on the current core, set the new owner to NULL and return the save area to fill */
        RvCoprocSaveArea* sa = pxPortUpdateCoprocOwner(0, FPU_COPROC_IDX, پوچ);
        /* `sa` is not NULL here for sure */
        context_to_save = sa->sa_coprocs[FPU_COPROC_IDX];
    }
    /* Migrate the task to core 1. NOTE: This will override any existing pinning of the task */
    vPortTaskPinToCore(task, 1);
    /* Raised an error if the scheduler is NOT running on core 0 */
    اگر (!port_xSchedulerRunning[0]) {
        /* Scheduler is not running on core 0, raise an error */
        g_panic_abort = true;
        g_panic_abort_details = (نویسه *) "ERROR: Scheduler is not running on core 0, task must migrate to core 1!\n";
        xt_unhandled_exception(frame);
    }
    /* Send a cross-core interrupt on the current core, it won't be triggered until we return from the exception handler */
    esp_crosscore_int_send_yield(0);
    بازگشت context_to_save;
#endif /* CONFIG_FREERTOS_UNICORE */
}

#endif /* CONFIG_IDF_TARGET_ESP32S31 */


#endif /* SOC_CPU_COPROC_NUM > 0 */

/* ------------------------------------------------ Run Time Stats ------------------------------------------------- */

#اگر ( CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS )

configRUN_TIME_COUNTER_TYPE x_Port_Get_Run_Time_Counter_مقدار( تهی )
{
#ifdef CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
    بازگشت (configRUN_TIME_COUNTER_TYPE) esp_timer_get_time();
#وگرنه
    بازگشت 0;
#endif // CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
}

#endif /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */

/* ---------------------------------------------- Misc Implementations -------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */
#اگر (SOC_CPU_COPROC_رقم > 0) && SOC_CPU_HAS_FPU && SOC_PM_FPU_RETENTION_BY_SW
BaseType_t xPortFPUContextIsDirty(BaseType_t core_id)
{
    بازگشت (BaseType_t)(port_uxCoprocOwner[core_id][FPU_COPROC_IDX] != پوچ);
}
#endif /* (SOC_CPU_COPROC_NUM > 0) && SOC_CPU_HAS_FPU && SOC_PM_FPU_RETENTION_BY_SW */
