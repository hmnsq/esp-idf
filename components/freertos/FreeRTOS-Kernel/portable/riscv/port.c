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

#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ <stdbool.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ "soc/soc_caps.h"
#అన్తర్భావ "soc/periph_defs.h"
#అన్తర్భావ "soc/system_reg.h"
#అన్తర్భావ "soc/interrupt_reg.h"
#అన్తర్భావ "hal/systimer_hal.h"
#అన్తర్భావ "hal/systimer_ll.h"
#అన్తర్భావ "riscv/rvruntime-frames.h"
#అన్తర్భావ "riscv/rv_utils.h"
#అన్తర్భావ "riscv/interrupt.h"
#అన్తర్భావ "esp_private/crosscore_int.h"
#అన్తర్భావ "hal/crosscore_int_ll.h"
#అన్తర్భావ "esp_attr.h"
#అన్తర్భావ "esp_compiler.h"
#అన్తర్భావ "esp_system.h"
#అన్తర్భావ "esp_intr_alloc.h"
#అన్తర్భావ "esp_log.h"
#అన్తర్భావ "FreeRTOS.h"       /* This pulls in portmacro.h */
#అన్తర్భావ "task.h"
#ఒకవేళ CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST
#అన్తర్భావ "freertos/idf_additions.h"
#అయితే_ముగింపు
#అన్తర్భావ "portmacro.h"
#అన్తర్భావ "port_systick.h"
#అన్తర్భావ "esp_memory_utils.h"
#అన్తర్భావ "esp_macros.h"
#ఒకవేళ CONFIG_FREERTOS_నిరంతరం_సమయం_STATS_USING_ESP_TIMER
#అన్తర్భావ "esp_timer.h"
#అయితే_ముగింపు

#ఒకవేళ SOC_CPU_HAS_HWLOOP
#అన్తర్భావ "riscv/csr.h"
#అన్తర్భావ "riscv/csr_hwlp.h"
#అయితే_ముగింపు

#ఒకవేళ ( SOC_CPU_COPROC_అంకె > 0 )

#అన్తర్భావ "esp_private/panic_internal.h"

/* Since `portFORCE_INLINE` is not defined in `portmacro.h`, we must define it here since it is
 * used by `atomic.h`. */
#నిర్వచించు పోర్టు_FORCE_INLINE    అంతఃవరుస
#అన్తర్భావ "freertos/atomic.h"

#అయితే_ముగింపు // ( SOC_CPU_COPROC_NUM > 0 )


_Static_assert(పోర్టు_BYTE_ALIGNMENT == 16, "portBYTE_ALIGNMENT must be set to 16");
#ఒకవేళ CONFIG_ESP_SYSTEM_HW_దొంతర_GUARD
/**
 * offsetof() can not be used in asm code. Then we need make sure that
 * PORT_OFFSET_PX_STACK and PORT_OFFSET_PX_END_OF_STACK have expected values.
 * Macro used in the portasm.S instead of variables to save at least 4 instruction calls
 * which accessing DRAM memory. This optimization saves CPU time in the interrupt handling.
 */

_Static_assert(offsetof( StaticTask_t, pxDummy6 ) == పోర్టు_స్థానభ్రంశం_PX_దొంతర);
_Static_assert(offsetof( StaticTask_t, pxDummy8 ) == పోర్టు_స్థానభ్రంశం_PX_చివర_యొక్క_దొంతర);
#అయితే_ముగింపు // CONFIG_ESP_SYSTEM_HW_STACK_GUARD

/* ---------------------------------------------------- Variables ------------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */

అస్థిర UBaseType_t పోర్టు_x_Scheduler_Running[పోర్టు_అంకె_PROCESSORS] = {0}; // Indicates whether scheduler is running on a per-core basis
అస్థిర UBaseType_t పోర్టు_ux_అంతరాయం_Nesting[పోర్టు_అంకె_PROCESSORS] = {0};  // Interrupt nesting level. Increased/decreased in portasm.c
అస్థిర UBaseType_t పోర్టు_ux_Critical_Nesting[పోర్టు_అంకె_PROCESSORS] = {0};
#ఒకవేళ CONFIG_FREERTOS_పోర్టు_THREAD_సురక్షిత_CLAIM
అస్థిర bool పోర్టు_x_Thread_సురక్షిత_Claimed = false;
#అయితే_ముగింపు
అస్థిర UBaseType_t పోర్టు_ux_Old_అంతరాయం_State[పోర్టు_అంకె_PROCESSORS] = {0};
అస్థిర UBaseType_t x_పోర్టు_Switch_Flag[పోర్టు_అంకె_PROCESSORS] = {0};
అస్థిర UBaseType_t పోర్టు_ux_Core_Startup_పూర్తి[పోర్టు_అంకె_PROCESSORS] = {0};  // Indicates whether the core has completed its startup sequence

#ఒకవేళ ( SOC_CPU_COPROC_అంకె > 0 )

/* Current owner of the coprocessors for each core */
StaticTask_t* పోర్టు_ux_Coproc_Owner[పోర్టు_అంకె_PROCESSORS][SOC_CPU_COPROC_అంకె];

#అయితే_ముగింపు /* SOC_CPU_COPROC_NUM > 0 */

/*
*******************************************************************************
* Interrupt stack. The size of the interrupt stack is determined by the config
* parameter "configISR_STACK_SIZE" in FreeRTOSConfig.h
*******************************************************************************
*/
__attribute__((aligned(16))) దొంతర_Type_t x_Isr_దొంతర[పోర్టు_అంకె_PROCESSORS][config_ISR_దొంతర_కొలత];
దొంతర_Type_t *x_Isr_దొంతర_శిఖరం[పోర్టు_అంకె_PROCESSORS] = {0};

#ఒకవేళ CONFIG_ESP_SYSTEM_HW_దొంతర_GUARD
దొంతర_Type_t *x_Isr_దొంతర_Bottom[పోర్టు_అంకె_PROCESSORS] = {0};
#అయితే_ముగింపు

/* ------------------------------------------------ FreeRTOS Portable --------------------------------------------------
 * - Provides implementation for functions required by FreeRTOS
 * - Declared in portable.h
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------- Scheduler Start/End -------------------

పునాది_Type_t x_పోర్టు_ప్రారంభం_Scheduler(శూన్య)
{
#ఒకవేళ ( SOC_CPU_COPROC_అంకె > 0 )

#ఒకవేళ SOC_CPU_HAS_FPU
    /* Disable FPU so that the first task to use it will trigger an exception */
    rv_utils_disable_fpu();
#అయితే_ముగింపు /* SOC_CPU_HAS_FPU */

#ఒకవేళ SOC_CPU_HAS_PIE
    /* Similarly, disable PIE */
    rv_utils_disable_pie();
#అయితే_ముగింపు /* SOC_CPU_HAS_PIE */

#ఒకవేళ SOC_CPU_HAS_DSP
    rv_utils_disable_dsp();
#అయితే_ముగింపు /* SOC_CPU_HAS_DSP */

#ఒకవేళ SOC_CPU_HAS_HWLOOP
    /* Initialize the Hardware loop feature */
    RV_WRITE_CSR(CSR_HWLP_STATE_REG, HWLP_INITIAL_STATE);
#అయితే_ముగింపు /* SOC_CPU_HAS_HWLOOP */
#అయితే_ముగింపు /* ( SOC_CPU_COPROC_NUM > 0 ) */

    /* Initialize all kernel state tracking variables */
    పునాది_Type_t coreID = x_పోర్టు_Get_Core_ID();
    పోర్టు_ux_అంతరాయం_Nesting[coreID] = 0;
    పోర్టు_ux_Critical_Nesting[coreID] = 0;
#ఒకవేళ CONFIG_FREERTOS_పోర్టు_THREAD_సురక్షిత_CLAIM
    పోర్టు_x_Thread_సురక్షిత_Claimed = false;
#అయితే_ముగింపు
    పోర్టు_x_Scheduler_Running[coreID] = 0;
    పోర్టు_ux_Core_Startup_పూర్తి[coreID] = 0;

    /* Initialize ISR Stack(s) */
    కొరకు (పూర్ణాంకం i = 0; i < పోర్టు_అంకె_PROCESSORS; i++) {
        x_Isr_దొంతర_శిఖరం[i] = &x_Isr_దొంతర[i][0] + (config_ISR_దొంతర_కొలత & (~((పోర్టు_సూచిక_కొలత_TYPE)పోర్టు_BYTE_ALIGNMENT_ముసుగు)));
#ఒకవేళ CONFIG_ESP_SYSTEM_HW_దొంతర_GUARD
        x_Isr_దొంతర_Bottom[i] = &x_Isr_దొంతర[i][0];
#అయితే_ముగింపు
    }

    /* Setup the hardware to generate the tick. */
    v_పోర్టు_అమరిక_Timer();

    rv_utils_intr_global_enable();
    esprv_int_సమితి_పరిమితి(RVHAL_INTR_ENABLE_THRESH); /* set global interrupt masking level */

    v_పోర్టు_Yield();

    /* Should not get here */
    తిరిగి pdTRUE;
}

శూన్య v_పోర్టు_చివర_Scheduler(శూన్య)
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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_దొంతర_TLS(UBaseType_t ux_దొంతర_సూచిక, uint32_t *ret_threadptr_reg_init)
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
    బాహ్య అక్షరం thread_local_దత్తాంశం_ప్రారంభం, thread_local_దత్తాంశం_చివర;
    బాహ్య అక్షరం thread_local_bss_ప్రారంభం, thread_local_bss_చివర;
    స్థిరరాశి uint32_t tls_దత్తాంశం_కొలత = (uint32_t)&thread_local_దత్తాంశం_చివర - (uint32_t)&thread_local_దత్తాంశం_ప్రారంభం;
    స్థిరరాశి uint32_t tls_bss_కొలత = (uint32_t)&thread_local_bss_చివర - (uint32_t)&thread_local_bss_ప్రారంభం;
    స్థిరరాశి uint32_t tls_area_కొలత = ESP_ALIGN_పైకి(tls_దత్తాంశం_కొలత + tls_bss_కొలత, 16);
    // TODO: check that TLS area fits the stack

    // Allocate space for the TLS area on the stack. The area must be aligned to 16-bytes
    ux_దొంతర_సూచిక = ESP_ALIGN_కిందికి(ux_దొంతర_సూచిక - (UBaseType_t)tls_area_కొలత, 16);
    // Initialize the TLS data with the initialization values of each TLS variable
    జ్ఞాపకం_నకలు((శూన్య *)ux_దొంతర_సూచిక, &thread_local_దత్తాంశం_ప్రారంభం, tls_దత్తాంశం_కొలత);
    // Initialize the TLS bss with zeroes
    memset((శూన్య *)(ux_దొంతర_సూచిక + tls_దత్తాంశం_కొలత), 0, tls_bss_కొలత);

    // Save tls start address
    *ret_threadptr_reg_init = (uint32_t)ux_దొంతర_సూచిక;
    తిరిగి ux_దొంతర_సూచిక;
}

#ఒకవేళ CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
స్థిర శూన్య v_పోర్టు_Task_Wrapper(TaskFunction_t px_సంకేతం, శూన్య *pvParameters)
{
    __asm__ అస్థిర(".cfi_undefined ra");  // tell to debugger that it's outermost (initial) frame
    బాహ్య శూన్య __attribute__((noreturn)) panic_abort(స్థిరరాశి అక్షరం *details);
    స్థిర అక్షరం DRAM_ATTR సందేశం[80] = "FreeRTOS: FreeRTOS Task \"\0";
    px_సంకేతం(pvParameters);
    /* FreeRTOS tasks should not return. Log the task name and abort. */
    /* We cannot use s(n)printf because it is in flash */
    strcat(సందేశం, pcTaskGetName(ఖాళీ));
    strcat(సందేశం, "\" should not return, Aborting now!");
    panic_abort(సందేశం);
}
#అయితే_ముగింపు // CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER


#ఒకవేళ ( SOC_CPU_COPROC_అంకె > 0 )

/**
 * @brief Retrieve or allocate coprocessors save area from the given pxTopOfStack address.
 *
 * @param pxTopOfStack End of the stack address. This represents the highest address of a Task's stack.
 */
FORCE_INLINE_ATTR RvCoprocSaveArea* px_Retrieve_Coproc_Save_Area_From_దొంతర_సూచిక(UBaseType_t px_శిఖరం_యొక్క_దొంతర)
{
    తిరిగి (RvCoprocSaveArea*) ESP_ALIGN_కిందికి(px_శిఖరం_యొక్క_దొంతర - పరిమాణం(RvCoprocSaveArea), 16);
}

/**
 * @brief Allocate and initialize the coprocessors save area on the stack
 *
 * @param[in] uxStackPointer Current stack pointer address
 *
 * @return Stack pointer that points to allocated and initialized the coprocessor save area
 */
FORCE_INLINE_ATTR UBaseType_t uxInitialiseCoprocSaveArea(UBaseType_t ux_దొంతర_సూచిక)
{
    RvCoprocSaveArea* sa = px_Retrieve_Coproc_Save_Area_From_దొంతర_సూచిక(ux_దొంతర_సూచిక);
    memset(sa, 0, పరిమాణం(RvCoprocSaveArea));
    తిరిగి (UBaseType_t) sa;
}


స్థిర శూన్య v_పోర్టు_Clean_పైకి_Coproc_Area(శూన్య *pvTCB)
{
    StaticTask_t* task = (StaticTask_t*) pvTCB;

    /* Get a pointer to the task's coprocessor save area */
    స్థిరరాశి UBaseType_t bottomstack = (UBaseType_t) task->pxDummy8;
    RvCoprocSaveArea* sa = px_Retrieve_Coproc_Save_Area_From_దొంతర_సూచిక(bottomstack);

    /* If the Task ever saved the original stack pointer, restore it before returning */
    ఒకవేళ (sa->sa_allocator != 0) {
        /* Restore the original lowest address of the stack in the TCB */
        task->pxDummy6 = sa->sa_tcbstack;

        /* Get the core the task is pinned on */
        #ఒకవేళ ( config_అంకె_CORES > 1 )
            స్థిరరాశి పునాది_Type_t coreID = task->xDummyCoreID;
            /* If the task is not pinned on any core, it didn't use any coprocessor than need to be freed (FPU or PIE).
             * If it used the HWLP coprocessor, it has nothing to clear since there is no "owner" for it. */
            ఒకవేళ (coreID == tskNO_AFFINITY) {
                తిరిగి;
            }
        #లేకపోతే /* configNUM_CORES > 1 */
            స్థిరరాశి పునాది_Type_t coreID = 0;
        #అయితే_ముగింపు /* configNUM_CORES > 1 */

        కొరకు (పూర్ణాంకం i = 0; i < SOC_CPU_COPROC_అంకె; i++) {
            StaticTask_t** owner = &పోర్టు_ux_Coproc_Owner[coreID][i];
            /* If the owner is `task`, replace it with NULL atomically */
            Atomic_Compare_And_మార్పిడి_సూచికలు_p32((శూన్య**) owner, ఖాళీ, task);
        }
    }
}
#అయితే_ముగింపు /* SOC_CPU_COPROC_NUM > 0 */


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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_దొంతర_చట్రం(UBaseType_t ux_దొంతర_సూచిక, TaskFunction_t px_సంకేతం, శూన్య *pvParameters, uint32_t threadptr_reg_init)
{
    /*
    Allocate space for the task's starting interrupt stack frame.
    - The stack frame must be allocated to a 16-byte aligned address.
    - We use RV_STK_FRMSZ as it rounds up the total size to a multiple of 16.
    */
    ux_దొంతర_సూచిక = ESP_ALIGN_కిందికి(ux_దొంతర_సూచిక - RV_STK_FRMSZ, 16);

    // Clear the entire interrupt stack frame
    Rv_Exc_చట్రం *చట్రం = (Rv_Exc_చట్రం *)ux_దొంతర_సూచిక;
    memset(చట్రం, 0, పరిమాణం(Rv_Exc_చట్రం));

    /* Initialize the stack frame. */
    బాహ్య uint32_t __global_pointer$;
    #ఒకవేళ CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
        చట్రం->mepc = (UBaseType_t)v_పోర్టు_Task_Wrapper;
        చట్రం->a0 = (UBaseType_t)px_సంకేతం;
        చట్రం->a1 = (UBaseType_t)pvParameters;
    #లేకపోతే
        చట్రం->mepc = (UBaseType_t)px_సంకేతం;
        చట్రం->a0 = (UBaseType_t)pvParameters;
    #అయితే_ముగింపు // CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
    చట్రం->gp = (UBaseType_t)&__global_pointer$;
    చట్రం->tp = (UBaseType_t)threadptr_reg_init;

    తిరిగి ux_దొంతర_సూచిక;
}

దొంతర_Type_t *px_పోర్టు_Initialise_దొంతర(దొంతర_Type_t *px_శిఖరం_యొక్క_దొంతర, TaskFunction_t px_సంకేతం, శూన్య *pvParameters)
{
#నిర్వచితమైతే __clang_analyzer__
    // Teach clang-tidy that pxTopOfStack cannot be a pointer to const
    అస్థిర దొంతర_Type_t * px_తాత్కాలిక = px_శిఖరం_యొక్క_దొంతర;
    px_శిఖరం_యొక్క_దొంతర = px_తాత్కాలిక;
#అయితే_ముగింపు /*__clang_analyzer__ */
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

    UBaseType_t ux_దొంతర_సూచిక = (UBaseType_t)px_శిఖరం_యొక్క_దొంతర;
    configASSERT((ux_దొంతర_సూచిక & పోర్టు_BYTE_ALIGNMENT_ముసుగు) == 0);

#ఒకవేళ ( SOC_CPU_COPROC_అంకె > 0 )
    // Initialize the coprocessors save area
    ux_దొంతర_సూచిక = uxInitialiseCoprocSaveArea(ux_దొంతర_సూచిక);
    configASSERT((ux_దొంతర_సూచిక & పోర్టు_BYTE_ALIGNMENT_ముసుగు) == 0);
#అయితే_ముగింపు // SOC_CPU_COPROC_NUM > 0

    // Initialize GCC TLS area
    uint32_t threadptr_reg_init;
    ux_దొంతర_సూచిక = ux_Initialise_దొంతర_TLS(ux_దొంతర_సూచిక, &threadptr_reg_init);
    configASSERT((ux_దొంతర_సూచిక & పోర్టు_BYTE_ALIGNMENT_ముసుగు) == 0);

    // Initialize the starting interrupt stack frame
    ux_దొంతర_సూచిక = ux_Initialise_దొంతర_చట్రం(ux_దొంతర_సూచిక, px_సంకేతం, pvParameters, threadptr_reg_init);
    configASSERT((ux_దొంతర_సూచిక & పోర్టు_BYTE_ALIGNMENT_ముసుగు) == 0);

    // Return the task's current stack pointer address which should point to the starting interrupt stack frame
    తిరిగి (దొంతర_Type_t *)ux_దొంతర_సూచిక;
}



/* ---------------------------------------------- Port Implementations -------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

పునాది_Type_t x_పోర్టు_లోపల_Isr_Context(శూన్య)
{
#ఒకవేళ (config_అంకె_CORES > 1)
    గుర్తులేని పూర్ణాంకం irq_స్థితి;
    పునాది_Type_t ret;

    /* Disable interrupts to fetch the coreID atomically */
    irq_స్థితి = పోర్టు_సమితి_అంతరాయం_ముసుగు_FROM_ISR();

    /* Return the interrupt nesting counter for this core */
    ret = పోర్టు_ux_అంతరాయం_Nesting[x_పోర్టు_Get_Core_ID()];

    /* Restore interrupts */
    పోర్టు_CLEAR_అంతరాయం_ముసుగు_FROM_ISR(irq_స్థితి);

    తిరిగి ret;
#లేకపోతే
    /* Optimize the call for single-core targets */
    తిరిగి పోర్టు_ux_అంతరాయం_Nesting[0];
#అయితే_ముగింపు /* (configNUM_CORES > 1) */
}

శూన్య v_పోర్టు_Assert_If_లోపల_ISR(శూన్య)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(x_పోర్టు_లోపల_Isr_Context() == 0);
}

పునాది_Type_t x_పోర్టు_Interrupted_From_ISRContext(శూన్య)
{
    /* Return the interrupt nesting counter for this core */
    తిరిగి పోర్టు_ux_అంతరాయం_Nesting[x_పోర్టు_Get_Core_ID()];
}

UBaseType_t x_పోర్టు_సమితి_అంతరాయం_ముసుగు_From_ISR(శూన్య)
{
    UBaseType_t prev_int_level = 0, int_level = 0;
#ఒకవేళ !SOC_INT_CLIC_SUPPORTED
    int_level = RVHAL_EXCM_LEVEL;
#లేకపోతే
    int_level = RVHAL_EXCM_LEVEL_CLIC;
#అయితే_ముగింపు

    prev_int_level = rv_utils_సమితి_intlevel_regval(int_level);
    తిరిగి prev_int_level;
}

శూన్య v_పోర్టు_Clear_అంతరాయం_ముసుగు_From_ISR(UBaseType_t prev_int_level)
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
    asm అస్థిర ( "nop" );
    asm అస్థిర ( "nop" );
    asm అస్థిర ( "nop" );
}

// ------------------ Critical Sections --------------------

#ఒకవేళ (config_అంకె_CORES > 1)
పునాది_Type_t __attribute__((optimize("-O3"))) x_పోర్టు_Enter_Critical_Timeout(పోర్టు_MUX_TYPE *mux, పునాది_Type_t timeout)
{
#ఒకవేళ CONFIG_FREERTOS_పోర్టు_THREAD_సురక్షిత_CLAIM
    ఒకవేళ (unlikely(పోర్టు_x_Thread_సురక్షిత_Claimed)) {
        తిరిగి pdPASS;
    }
#అయితే_ముగింపు
    /* Interrupts may already be disabled (if this function is called in nested
     * manner). However, there's no atomic operation that will allow us to check,
     * thus we have to disable interrupts again anyways.
     *
     * However, if this is call is NOT nested (i.e., the first call to enter a
     * critical section), we will save the previous interrupt level so that the
     * saved level can be restored on the last call to exit the critical.
     */
    పునాది_Type_t x_Old_అంతరాయం_Level = పోర్టు_సమితి_అంతరాయం_ముసుగు_FROM_ISR();
    /* Interrupts are masked, so the core id is stable. Read it once and reuse it for
     * both the spinlock owner id and the per-core critical nesting state. Interrupts
     * stay masked for the whole critical section (to the same level the spinlock would
     * use), so the spinlock does not need to disable them again. */
    పునాది_Type_t coreID = x_పోర్టు_Get_Core_ID();
    ఒకవేళ (!spinlock_acquire_impl(mux, timeout, spinlock_owner_id_for_core(coreID))) {
        //Timed out attempting to get spinlock. Restore previous interrupt level and return
        పోర్టు_CLEAR_అంతరాయం_ముసుగు_FROM_ISR(x_Old_అంతరాయం_Level);
        తిరిగి pdFAIL;
    }
    //Spinlock acquired. Increment the critical nesting count.
    పునాది_Type_t newNesting = పోర్టు_ux_Critical_Nesting[coreID] + 1;
    పోర్టు_ux_Critical_Nesting[coreID] = newNesting;
    //If this is the first entry to a critical section. Save the old interrupt level.
    ఒకవేళ ( newNesting == 1 ) {
        పోర్టు_ux_Old_అంతరాయం_State[coreID] = x_Old_అంతరాయం_Level;
    }
    తిరిగి pdPASS;
}

శూన్య __attribute__((optimize("-O3"))) v_పోర్టు_Exit_Critical_Multi_Core(పోర్టు_MUX_TYPE *mux)
{
#ఒకవేళ CONFIG_FREERTOS_పోర్టు_THREAD_సురక్షిత_CLAIM
    ఒకవేళ (unlikely(పోర్టు_x_Thread_సురక్షిత_Claimed)) {
        తిరిగి;
    }
#అయితే_ముగింపు
    /* This function may be called in a nested manner. Therefore, we only need
     * to re-enable interrupts if this is the last call to exit the critical. We
     * can use the nesting count to determine whether this is the last exit call.
     */
    /* Interrupts remain disabled for the whole critical section, so the core id is
     * stable and the spinlock does not need to mask them again. Read the core id
     * once and reuse it for both the release owner id and the per-core nesting state. */
    పునాది_Type_t coreID = x_పోర్టు_Get_Core_ID();
    spinlock_release_impl(mux, spinlock_owner_id_for_core(coreID));
    పునాది_Type_t nesting = పోర్టు_ux_Critical_Nesting[coreID];

    /* Critical section nesting count must never be negative */
    configASSERT( nesting > 0 );

    ఒకవేళ (nesting > 0) {
        nesting--;
        పోర్టు_ux_Critical_Nesting[coreID] = nesting;

        //This is the last exit call, restore the saved interrupt level
        ఒకవేళ ( nesting == 0 ) {
            పోర్టు_CLEAR_అంతరాయం_ముసుగు_FROM_ISR(పోర్టు_ux_Old_అంతరాయం_State[coreID]);
        }
    }
}


పునాది_Type_t x_పోర్టు_Enter_Critical_Timeout_Compliance(పోర్టు_MUX_TYPE *mux, పునాది_Type_t timeout)
{
    పునాది_Type_t ret;
    ఒకవేళ (!x_పోర్టు_లోపల_Isr_Context()) {
        ret = x_పోర్టు_Enter_Critical_Timeout(mux, timeout);
    } లేకపోతే {
        esp_rom_printf("port*_CRITICAL called from ISR context. Aborting!\n");
        abort();
        ret = pdFAIL;
    }
    తిరిగి ret;
}

శూన్య v_పోర్టు_Exit_Critical_Compliance(పోర్టు_MUX_TYPE *mux)
{
    ఒకవేళ (!x_పోర్టు_లోపల_Isr_Context()) {
        v_పోర్టు_Exit_Critical_Multi_Core(mux);
    } లేకపోతే {
        esp_rom_printf("port*_CRITICAL called from ISR context. Aborting!\n");
        abort();
    }
}
#అయితే_ముగింపు /* (configNUM_CORES > 1) */

#ఒకవేళ CONFIG_FREERTOS_పోర్టు_THREAD_సురక్షిత_CLAIM
శూన్య x_పోర్టు_Thread_సురక్షిత_Claim(శూన్య)
{
    configASSERT(!x_పోర్టు_Can_Yield());
    configASSERT(!పోర్టు_x_Thread_సురక్షిత_Claimed);
    పోర్టు_x_Thread_సురక్షిత_Claimed = true;
}

శూన్య x_పోర్టు_Thread_సురక్షిత_Disclaim(శూన్య)
{
    configASSERT(!x_పోర్టు_Can_Yield());
    configASSERT(పోర్టు_x_Thread_సురక్షిత_Claimed);
    పోర్టు_x_Thread_సురక్షిత_Claimed = false;
}
#అయితే_ముగింపు /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

శూన్య v_పోర్టు_Enter_Critical(శూన్య)
{
#ఒకవేళ (config_అంకె_CORES > 1)
        esp_rom_printf("vPortEnterCritical(void) is not supported on multi-core targets. Please use vPortEnterCriticalMultiCore(portMUX_TYPE *mux) instead.\n");
        abort();
#అయితే_ముగింపు /* (configNUM_CORES > 1) */
#ఒకవేళ CONFIG_FREERTOS_పోర్టు_THREAD_సురక్షిత_CLAIM
    ఒకవేళ (unlikely(పోర్టు_x_Thread_సురక్షిత_Claimed)) {
        తిరిగి;
    }
#అయితే_ముగింపు
    పునాది_Type_t state = పోర్టు_సమితి_అంతరాయం_ముసుగు_FROM_ISR();
    పోర్టు_ux_Critical_Nesting[0]++;

    ఒకవేళ (పోర్టు_ux_Critical_Nesting[0] == 1) {
        పోర్టు_ux_Old_అంతరాయం_State[0] = state;
    }
}

శూన్య v_పోర్టు_Exit_Critical(శూన్య)
{
#ఒకవేళ (config_అంకె_CORES > 1)
        esp_rom_printf("vPortExitCritical(void) is not supported on multi-core targets. Please use vPortExitCriticalMultiCore(portMUX_TYPE *mux) instead.\n");
        abort();
#అయితే_ముగింపు /* (configNUM_CORES > 1) */
#ఒకవేళ CONFIG_FREERTOS_పోర్టు_THREAD_సురక్షిత_CLAIM
    ఒకవేళ (unlikely(పోర్టు_x_Thread_సురక్షిత_Claimed)) {
        తిరిగి;
    }
#అయితే_ముగింపు

    /* Critical section nesting count must never be negative */
    configASSERT( పోర్టు_ux_Critical_Nesting[0] > 0 );

    ఒకవేళ (పోర్టు_ux_Critical_Nesting[0] > 0) {
        పోర్టు_ux_Critical_Nesting[0]--;

        ఒకవేళ (పోర్టు_ux_Critical_Nesting[0] == 0) {
            పోర్టు_CLEAR_అంతరాయం_ముసుగు_FROM_ISR(పోర్టు_ux_Old_అంతరాయం_State[0]);
        }
    }
}

// ---------------------- Yielding -------------------------

శూన్య v_పోర్టు_Yield(శూన్య)
{
    పునాది_Type_t coreID = x_పోర్టు_Get_Core_ID();

    ఒకవేళ (పోర్టు_ux_అంతరాయం_Nesting[coreID]) {
        v_పోర్టు_Yield_From_ISR();
    } లేకపోతే {
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
        అయితే (పోర్టు_x_Scheduler_Running[coreID] && పోర్టు_ux_Critical_Nesting[coreID] == 0 && crosscore_int_ll_get_state(coreID) != 0) {}
    }
}

శూన్య v_పోర్టు_Yield_From_ISR( శూన్య )
{
    trace_ISR_EXIT_గమ్యం_SCHEDULER();
    పునాది_Type_t coreID = x_పోర్టు_Get_Core_ID();
    పోర్టు_x_Scheduler_Running[coreID] = 1;
    x_పోర్టు_Switch_Flag[coreID] = 1;
}

శూన్య v_పోర్టు_Yield_Other_Core(పునాది_Type_t coreid)
{
    esp_crosscore_int_send_yield(coreid);
}

// ------------------- Hook Functions ----------------------

శూన్య __attribute__((weak)) v_Application_దొంతర_అధికప్రవాహం_Hook(TaskHandle_t xTask, అక్షరం *pcTaskName)
{
#నిర్వచించు ERR_STR1 "***ERROR*** A stack overflow in task "
#నిర్వచించు ERR_STR2 " has been detected."
    స్థిరరాశి అక్షరం *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    అక్షరం buf[పరిమాణం(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + పరిమాణం(ERR_STR2) + 1 /* null char */] = {0};

    అక్షరం *dest = buf;
    కొరకు (పూర్ణాంకం i = 0; i < పరిమాణం(str) / పరిమాణం(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
    esp_system_abort(buf);
}

// ----------------------- System --------------------------

uint32_t x_పోర్టు_Get_Tick_రేటు_Hz(శూన్య)
{
    తిరిగి (uint32_t)config_TICK_రేటు_HZ;
}

#నిర్వచించు దొంతర_WATCH_AREA_కొలత 32
#నిర్వచించు దొంతర_WATCH_బిందువు_సంఖ్య (SOC_CPU_WATCHPOINTS_అంకె - 1)

శూన్య v_పోర్టు_సమితి_దొంతర_Watchpoint(శూన్య *px_దొంతర_ప్రారంభం)
{
    uint32_t addr = (uint32_t)px_దొంతర_ప్రారంభం;
    addr = (addr + (దొంతర_WATCH_AREA_కొలత - 1)) & (~(దొంతర_WATCH_AREA_కొలత - 1));
    esp_cpu_సమితి_watchpoint(దొంతర_WATCH_బిందువు_సంఖ్య, (అక్షరం *)addr, దొంతర_WATCH_AREA_కొలత, ESP_CPU_WATCHPOINT_STORE);
}

// --------------------- TCB Cleanup -----------------------

#ఒకవేళ ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
స్థిర శూన్య v_పోర్టు_TLSPointers_Del_Cb( శూన్య *pxTCB )
{
    /* Typecast pxTCB to StaticTask_t type to access TCB struct members.
     * pvDummy15 corresponds to pvThreadLocalStoragePointers member of the TCB.
     */
    StaticTask_t *tcb = ( StaticTask_t * )pxTCB;

    /* The TLSP deletion callbacks are stored at an offset of (configNUM_THREAD_LOCAL_STORAGE_POINTERS/2) */
    TlsDeleteCallbackFunction_t *pv_Thread_Local_Storage_సూచికలు_Del_Callback = ( TlsDeleteCallbackFunction_t * )( &( tcb->pvDummy15[ ( config_అంకె_THREAD_LOCAL_STORAGE_సూచికలు / 2 ) ] ) );

    /* We need to iterate over half the depth of the pvThreadLocalStoragePointers area
     * to access all TLS pointers and their respective TLS deletion callbacks.
     */
    కొరకు ( పూర్ణాంకం x = 0; x < ( config_అంకె_THREAD_LOCAL_STORAGE_సూచికలు / 2 ); x++ ) {
        ఒకవేళ ( pv_Thread_Local_Storage_సూచికలు_Del_Callback[ x ] != ఖాళీ ) {  //If del cb is set
            /* In case the TLSP deletion callback has been overwritten by a TLS pointer, gracefully abort. */
            ఒకవేళ ( !esp_ptr_executable( pv_Thread_Local_Storage_సూచికలు_Del_Callback[ x ] ) ) {
                ESP_EARLY_LOGE("FreeRTOS", "Fatal error: TLSP deletion callback at index %d overwritten with non-excutable pointer %p", x, pv_Thread_Local_Storage_సూచికలు_Del_Callback[ x ]);
                abort();
            }

            pv_Thread_Local_Storage_సూచికలు_Del_Callback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#అయితే_ముగింపు /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

శూన్య v_పోర్టు_TCBPre_Delete_Hook( శూన్య *pxTCB )
{
    #ఒకవేళ ( CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK )
        /* Call the user defined task pre-deletion hook */
        బాహ్య శూన్య vTaskPreDeletionHook( శూన్య * pxTCB );
        vTaskPreDeletionHook( pxTCB );
    #అయితే_ముగింపు /* CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK */

    #ఒకవేళ ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
        /* Call TLS pointers deletion callbacks */
        v_పోర్టు_TLSPointers_Del_Cb( pxTCB );
    #అయితే_ముగింపు /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

    #ఒకవేళ ( CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST )
        ఒకవేళ (pxTCB != ఖాళీ) {
            v_Task_సమితి_Pie_Blacklisted((TaskHandle_t)pxTCB, pdFALSE);
        }
    #అయితే_ముగింపు /* CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST */

    #ఒకవేళ ( SOC_CPU_COPROC_అంకె > 0 )
        /* Cleanup coproc save area */
        v_పోర్టు_Clean_పైకి_Coproc_Area( pxTCB );
    #అయితే_ముగింపు /* SOC_CPU_COPROC_NUM > 0 */
}


#ఒకవేళ ( SOC_CPU_COPROC_అంకె > 0 )

// ----------------------- Coprocessors --------------------------

/**
 * @brief Pin the given task to the given core
 *
 * This function is called when a task uses a coprocessor. Since the coprocessors registers
 * are saved lazily, as soon as a task starts using one, it must always be scheduled on the core
 * it is currently executing on.
 */
#ఒకవేళ ( config_అంకె_CORES > 1 )
శూన్య v_పోర్టు_Task_Pin_గమ్యం_Core(StaticTask_t* task, పూర్ణాంకం coreid)
{
    task->xDummyCoreID = coreid;
}
#అయితే_ముగింపు /* configNUM_CORES > 1 */


/**
 * @brief Function to call to simulate an `abort()` occurring in a different context than the one it's called from.
 */
బాహ్య శూన్య xt_unhandled_exception(శూన్య *చట్రం);

/**
 * @brief Get coprocessor save area out of the given task. If the coprocessor area is not created,
 *        it shall be allocated.
 *
 * @param task Task to get the coprocessor save area of
 * @param allocate When true, memory will be allocated for the coprocessor if it hasn't been allocated yet.
 *                 When false, the coprocessor memory will be left as NULL if not allocated.
 * @param coproc Coprocessor number to allocate memory for
 */
RvCoprocSaveArea* px_పోర్టు_Get_Coproc_Area(StaticTask_t* task, bool allocate, పూర్ణాంకం coproc)
{
    ప్రతిజ్ఞా(coproc < SOC_CPU_COPROC_అంకె);

    స్థిరరాశి UBaseType_t bottomstack = (UBaseType_t) task->pxDummy8;
    RvCoprocSaveArea* sa = px_Retrieve_Coproc_Save_Area_From_దొంతర_సూచిక(bottomstack);
    /* Check if the allocator is NULL. Since we don't have a way to get the end of the stack
     * during its initialization, we have to do this here */
    ఒకవేళ (sa->sa_allocator == 0) {
        /* Since the lowest stack address shall not be used as `sp` anymore, we will modify it */
        sa->sa_tcbstack = task->pxDummy6;
        sa->sa_allocator = (UBaseType_t) task->pxDummy6;
    }

    /* Check if coprocessor area is allocated */
    ఒకవేళ (allocate && sa->sa_coprocs[coproc] == ఖాళీ) {
        స్థిరరాశి uint32_t coproc_sa_sizes[] = {
            RV_COPROC0_కొలత, RV_COPROC1_కొలత, RV_COPROC2_కొలత
        };
        స్థిరరాశి uint32_t coproc_sa_align[] = {
            RV_COPROC0_ALIGN, RV_COPROC1_ALIGN, RV_COPROC2_ALIGN
        };
        /* The allocator points to a usable part of the stack, use it for the coprocessor.
         * Align it up to the coprocessor save area requirement */
        sa->sa_allocator = (sa->sa_allocator + coproc_sa_align[coproc] - 1) & ~(coproc_sa_align[coproc] - 1);
        sa->sa_coprocs[coproc] = (శూన్య*) (sa->sa_allocator);
        sa->sa_allocator += coproc_sa_sizes[coproc];
        /* Update the lowest address of the stack to prevent FreeRTOS performing overflow/watermark checks on the coprocessors contexts */
        task->pxDummy6 = (శూన్య*) (sa->sa_allocator);
        /* Make sure the Task stack pointer is not pointing to the coprocessor context area, in other words, make
         * sure we don't have a stack overflow */
        శూన్య* task_sp = task->pxDummy1;
        ఒకవేళ (task_sp <= task->pxDummy6) {
            /* In theory we need to call vApplicationStackOverflowHook to trigger the stack overflow callback,
             * but in practice, since we are already in an exception handler, this won't work, so let's manually
             * trigger an exception with the previous coprocessor owner's TCB */
            g_panic_abort = true;
            g_panic_abort_details = (అక్షరం *) "ERROR: Stack overflow while saving coprocessor context!\n";
            xt_unhandled_exception(task_sp);
        }
    }
    తిరిగి sa;
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
RvCoprocSaveArea* px_పోర్టు_Update_Coproc_Owner(పూర్ణాంకం coreid, పూర్ణాంకం coproc, StaticTask_t* owner)
{
    RvCoprocSaveArea* sa = ఖాళీ;
    /* Address of coprocessor owner */
    StaticTask_t** owner_addr = &పోర్టు_ux_Coproc_Owner[ coreid ][ coproc ];
    /* Atomically exchange former owner with the new one */
    StaticTask_t* former = Atomic_మార్పిడి_సూచికలు_p32((శూన్య**) owner_addr, owner);
    /* Get the save area of former owner. small optimization here, if the former owner is the new owner,
     * return -1. This will simplify the assembly code while making it faster. */
    ఒకవేళ (former == owner) {
        sa = (శూన్య*) -1;
    } లేకపోతే ఒకవేళ (former != ఖాళీ) {
        /* Allocate coprocessor memory if not available yet */
        sa = px_పోర్టు_Get_Coproc_Area(former, true, coproc);
    }
    తిరిగి sa;
}


/**
 * @brief Aborts execution when a coprocessor was used in an ISR context
 */
శూన్య v_పోర్టు_Coproc_వాడిన_లోపల_ISR(శూన్య* చట్రం)
{
    /* Since this function is called from an exception handler, the interrupts are disabled,
     * as such, it is not possible to trigger another exception as would `abort` do.
     * Simulate an abort without actually triggering an exception. */
    g_panic_abort = true;
    g_panic_abort_details = (అక్షరం *) "ERROR: Coprocessors must not be used in ISRs!\n";
    xt_unhandled_exception(చట్రం);
}

#ఒకవేళ CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST
/**
 * @brief Abort if the given task is blacklisted from using the PIE coprocessor.
 *
 * Called from the PIE illegal-instruction path, so this must not trigger another
 * exception via abort().
 */
శూన్య v_పోర్టు_Abort_If_Task_Pie_Blacklisted(StaticTask_t *task, శూన్య *చట్రం)
{
    ఒకవేళ (task != ఖాళీ && xTaskGetPieBlacklisted((TaskHandle_t)task) != pdFALSE) {
        g_panic_abort = true;
        g_panic_abort_details = (అక్షరం *) "ERROR: Blacklisted task executed a PIE instruction!\n";
        xt_unhandled_exception(చట్రం);
    }
}
#అయితే_ముగింపు /* CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST */

#ఒకవేళ CONFIG_IDF_లక్ష్యం_ESP32_S31
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
శూన్య* v_పోర్టు_Task_వాడిన_PIEOn_CPU0(StaticTask_t* task, శూన్య* చట్రం)
{
#ఒకవేళ CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST
    v_పోర్టు_Abort_If_Task_Pie_Blacklisted(task, చట్రం);
#అయితే_ముగింపు
#ఒకవేళ CONFIG_FREERTOS_UNICORE
    g_panic_abort = true;
    g_panic_abort_details = (అక్షరం *) "ERROR: PIE coprocessor is not supported in unicore configuration!\n";
    xt_unhandled_exception(చట్రం);
    తిరిగి ఖాళీ;
#లేకపోతే
    శూన్య* context_గమ్యం_save = ఖాళీ;
    /* Make sure we are not in an interrupt context nor in a critical section */
    ఒకవేళ (x_పోర్టు_లోపల_Isr_Context()) {
        /* We are in an interrupt context, abort */
        v_పోర్టు_Coproc_వాడిన_లోపల_ISR(చట్రం);
    }
    /* Since we count on crosscore interrupt to reschedule the current task, we must not be in a
     * critical section. In other words, we must be able to yield */
    ఒకవేళ (!x_పోర్టు_Can_Yield()) {
        g_panic_abort = true;
        g_panic_abort_details = (అక్షరం *) "ERROR: PIE coprocessor must not be used in critical sections!\n";
        xt_unhandled_exception(చట్రం);
    }

    /* Check if the current task is the owner of the FPU on the current core. No need to make the following two instructions
     * atomic since we are in an exception context, we can't be interrupted by an interrupt. */
    ఒకవేళ (పోర్టు_ux_Coproc_Owner[0][FPU_COPROC_IDX] == task) {
        /* Task is the owner of the FPU on the current core, set the new owner to NULL and return the save area to fill */
        RvCoprocSaveArea* sa = px_పోర్టు_Update_Coproc_Owner(0, FPU_COPROC_IDX, ఖాళీ);
        /* `sa` is not NULL here for sure */
        context_గమ్యం_save = sa->sa_coprocs[FPU_COPROC_IDX];
    }
    /* Migrate the task to core 1. NOTE: This will override any existing pinning of the task */
    v_పోర్టు_Task_Pin_గమ్యం_Core(task, 1);
    /* Raised an error if the scheduler is NOT running on core 0 */
    ఒకవేళ (!పోర్టు_x_Scheduler_Running[0]) {
        /* Scheduler is not running on core 0, raise an error */
        g_panic_abort = true;
        g_panic_abort_details = (అక్షరం *) "ERROR: Scheduler is not running on core 0, task must migrate to core 1!\n";
        xt_unhandled_exception(చట్రం);
    }
    /* Send a cross-core interrupt on the current core, it won't be triggered until we return from the exception handler */
    esp_crosscore_int_send_yield(0);
    తిరిగి context_గమ్యం_save;
#అయితే_ముగింపు /* CONFIG_FREERTOS_UNICORE */
}

#అయితే_ముగింపు /* CONFIG_IDF_TARGET_ESP32S31 */


#అయితే_ముగింపు /* SOC_CPU_COPROC_NUM > 0 */

/* ------------------------------------------------ Run Time Stats ------------------------------------------------- */

#ఒకవేళ ( CONFIG_FREERTOS_GENERATE_నిరంతరం_సమయం_STATS )

config_నిరంతరం_సమయం_COUNTER_TYPE x_పోర్టు_Get_నిరంతరం_సమయం_Counter_విలువ( శూన్య )
{
#నిర్వచితమైతే CONFIG_FREERTOS_నిరంతరం_సమయం_STATS_USING_ESP_TIMER
    తిరిగి (config_నిరంతరం_సమయం_COUNTER_TYPE) esp_timer_get_సమయం();
#లేకపోతే
    తిరిగి 0;
#అయితే_ముగింపు // CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
}

#అయితే_ముగింపు /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */

/* ---------------------------------------------- Misc Implementations -------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */
#ఒకవేళ (SOC_CPU_COPROC_అంకె > 0) && SOC_CPU_HAS_FPU && SOC_PM_FPU_RETENTION_BY_SW
పునాది_Type_t x_పోర్టు_FPUContext_Is_Dirty(పునాది_Type_t core_id)
{
    తిరిగి (పునాది_Type_t)(పోర్టు_ux_Coproc_Owner[core_id][FPU_COPROC_IDX] != ఖాళీ);
}
#అయితే_ముగింపు /* (SOC_CPU_COPROC_NUM > 0) && SOC_CPU_HAS_FPU && SOC_PM_FPU_RETENTION_BY_SW */
