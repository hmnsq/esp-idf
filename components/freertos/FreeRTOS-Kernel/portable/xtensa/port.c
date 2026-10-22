/*
 * FreeRTOS Kernel V10.5.1
 * Copyright (C) 2015-2019 Cadence Design Systems, Inc.
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-FileCopyrightText: 2015-2019 Cadence Design Systems, Inc
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

#ضمن "sdkconfig.h"
#ضمن <stdbool.h>
#ضمن <stdint.h>
#ضمن <stdlib.h>
#ضمن <string.h>
#ضمن <xtensa/config/core.h>
#ضمن <xtensa_context.h>
#ضمن "soc/soc_caps.h"
#ضمن "esp_attr.h"
#ضمن "esp_compiler.h"
#ضمن "esp_private/crosscore_int.h"
#ضمن "esp_private/esp_int_wdt.h"
#ضمن "esp_system.h"
#ضمن "esp_log.h"
#ضمن "FreeRTOS.h"        /* This pulls in portmacro.h */
#ضمن "task.h"            /* Required for TaskHandle_t, tskNO_AFFINITY, and vTaskStartScheduler */
#ضمن "port_systick.h"
#ضمن "esp_cpu.h"
#ضمن <xtensa/hal.h>             /* required for xthal_get_ccount() */
#إذا CONFIG_FREERTOS_لاڳيتو_وقت_STATS_USING_ESP_TIMER
#ضمن "esp_timer.h"
#جيڪڏهن_پورو
#ضمن "esp_memory_utils.h"
#ضمن "esp_macros.h"

_Static_assert(پورٽ_BYTE_ALIGNMENT == 16, "portBYTE_ALIGNMENT must be set to 16");

/* ---------------------------------------------------- Variables ------------------------------------------------------
 * - Various variables used to maintain the FreeRTOS port's state. Used from both port.c and various .S files
 * - Constant offsets are used by assembly to jump to particular TCB members or a stack area (such as the CPSA). We use
 *   C constants instead of preprocessor macros due to assembly lacking "offsetof()".
 * ------------------------------------------------------------------------------------------------------------------ */

#إذا XCHAL_CP_انگ > 0
/* Offsets used to navigate to a task's CPSA on the stack */
ثابت DRAM_ATTR uint32_t وٿي_px_پڇاڙي_جو_ڍير = offsetof(StaticTask_t, pxDummy8);
ثابت DRAM_ATTR uint32_t وٿي_cpsa = XT_CP_سائيز;  /* Offset to start of the CPSA area on the stack. See uxInitialiseStackCPSA(). */
#إذا config_انگ_CORES > 1
/* Offset to TCB_t.xCoreID member. Used to pin unpinned tasks that use the FPU. */
ثابت DRAM_ATTR uint32_t وٿي_x_Core_ID = offsetof(StaticTask_t, xDummyCoreID);
#جيڪڏهن_پورو /* configNUM_CORES > 1 */
#جيڪڏهن_پورو /* XCHAL_CP_NUM > 0 */

متطاير بنانشان پورٽ_x_Scheduler_Running[پورٽ_انگ_PROCESSORS] = {0}; // Indicates whether scheduler is running on a per-core basis
بنانشان پورٽ_مداخلت_Nesting[پورٽ_انگ_PROCESSORS] = {0};  // Interrupt nesting level. Increased/decreased in portasm.c, _frxt_int_enter/_frxt_int_exit
بنياد_Type_t پورٽ_ux_Critical_Nesting[پورٽ_انگ_PROCESSORS] = {0};
#إذا CONFIG_FREERTOS_پورٽ_THREAD_محفوظ_CLAIM
متطاير bool پورٽ_x_Thread_محفوظ_Claimed = false;
#جيڪڏهن_پورو
بنياد_Type_t پورٽ_ux_Old_مداخلت_State[پورٽ_انگ_PROCESSORS] = {0};
متطاير بنانشان پورٽ_ux_Core_Startup_پورو[پورٽ_انگ_PROCESSORS] = {0};  // Indicates whether the core has completed its startup sequence

/*
*******************************************************************************
* Interrupt stack. The size of the interrupt stack is determined by the config
* parameter "configISR_STACK_SIZE" in FreeRTOSConfig.h
*******************************************************************************
*/
متطاير ڍير_Type_t DRAM_ATTR __attribute__((aligned(16))) پورٽ_Int_ڍير[پورٽ_انگ_PROCESSORS][config_ISR_ڍير_سائيز];
/* One flag for each individual CPU. */
متطاير uint32_t پورٽ_switch_flag[پورٽ_انگ_PROCESSORS];

/* ------------------------------------------------ FreeRTOS Portable --------------------------------------------------
 * - Provides implementation for functions required by FreeRTOS
 * - Declared in portable.h
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------- Scheduler Start/End -------------------

/* Defined in xtensa_context.S */
خارجي فراغ _xt_coproc_init(فراغ);

بنياد_Type_t x_پورٽ_شروعات_Scheduler( فراغ )
{
    پورٽ_DISABLE_INTERRUPTS();
    // Interrupts are disabled at this point and stack contains PS with enabled interrupts when task context is restored

#إذا XCHAL_CP_انگ > 0
    /* Initialize co-processor management for tasks. Leave CPENABLE alone. */
    _xt_coproc_init();
#جيڪڏهن_پورو

    /* Setup the hardware to generate the tick. */
    v_پورٽ_تياري_Timer();

    /* Initialize all kernel state tracking variables */
    بنياد_Type_t coreID = x_پورٽ_Get_Core_ID();
    پورٽ_x_Scheduler_Running[coreID] = 1;
    پورٽ_ux_Core_Startup_پورو[coreID] = 0;

    // Windows contain references to the startup stack which will be reclaimed by the main task
    // Spill the windows to create a clean environment to ensure we do not carry over any such references
    // to invalid SPs which will cause problems if main_task does a windowoverflow to them
    xthal_دري_spill();

    // Cannot be directly called from C; never returns
    __asm__ متطاير ("call0    _frxt_dispatch\n");

    /* Should not get here. */
    أرجع pdTRUE;
}

فراغ v_پورٽ_پڇاڙي_Scheduler( فراغ )
{
    /* It is unlikely that the Xtensa port will get stopped.  If required simply
    disable the tick interrupt here. */
    abort();
}

// ------------------------ Stack --------------------------

// User exception dispatcher when exiting
فراغ _xt_user_exit(فراغ);

#إذا CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
// Wrapper to allow task functions to return (increases stack overhead by 16 bytes)
ساكن فراغ v_پورٽ_Task_Wrapper(TaskFunction_t px_ڪوڊ, فراغ *pvParameters)
{
    px_ڪوڊ(pvParameters);
    //FreeRTOS tasks should not return. Log the task name and abort.
    حرف *pcTaskName = pcTaskGetName(عدم);
    ESP_LOGE("FreeRTOS", "FreeRTOS Task \"%s\" should not return, Aborting now!", pcTaskName);
    abort();
}
#جيڪڏهن_پورو



#إذا XCHAL_CP_انگ > 0
/**
 * @brief Allocate and initialize coprocessor save area on the stack
 *
 * This function allocates the coprocessor save area on the stack (sized XT_CP_SIZE) which includes...
 *  - Individual save areas for each coprocessor (size XT_CPx_SA, inclusive of each area's alignment)
 *  - Coprocessor context switching flags (e.g., XT_CPENABLE, XT_CPSTORED, XT_CP_CS_ST, XT_CP_ASA).
 *
 * The coprocessor save area is aligned to a 16-byte boundary.
 * The coprocessor context switching flags are then initialized
 *
 * @param[in] uxStackPointer Current stack pointer address
 * @return Stack pointer that points to allocated and initialized the coprocessor save area
 */
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_ڍير_CPSA(UBaseType_t ux_ڍير_اشارو)
{
    /*
    HIGH ADDRESS
    |-------------------|      XT_CP_SIZE
    | CPn SA            |           ^
    | ...               |           |
    | CP0 SA            |           |
    | ----------------- |           |       ---- XCHAL_TOTAL_SA_ALIGN aligned
    |-------------------|           |   12 bytes
    | XT_CP_ASA         |           |       ^
    | XT_CP_CS_ST       |           |       |
    | XT_CPSTORED       |           |       |
    | XT_CPENABLE       |           |       |
    |-------------------| ---------------------- 16 byte aligned
    LOW ADDRESS
    */

    // Allocate overall coprocessor save area, aligned down to 16 byte boundary
    ux_ڍير_اشارو = ESP_ALIGN_هيٺ_ڏانهن(ux_ڍير_اشارو - XT_CP_سائيز, 16);
    // Initialize the coprocessor context switching flags.
    uint32_t *p = (uint32_t *)ux_ڍير_اشارو;
    p[0] = 0;   // Clear XT_CPENABLE and XT_CPSTORED
    p[1] = 0;   // Clear XT_CP_CS_ST
    // XT_CP_ASA points to the aligned start of the individual CP save areas (i.e., start of CP0 SA)
    p[2] = (uint32_t)ESP_ALIGN_مٿي((uint32_t)ux_ڍير_اشارو + 12, XCHAL_ڪل_SA_ALIGN);
    أرجع ux_ڍير_اشارو;
}
#جيڪڏهن_پورو /* XCHAL_CP_NUM > 0 */

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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_ڍير_TLS(UBaseType_t ux_ڍير_اشارو, uint32_t *ret_threadptr_reg_init)
{
    /*
    TLS layout at link-time, where 0xNNN is the offset that the linker calculates to a particular TLS variable.

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
    // Calculate the TLS area's size (rounded up to multiple of 16 bytes).
    خارجي صحيح _tls_section_alignment;
    خارجي حرف thread_local_معلومات_شروعات, thread_local_معلومات_پڇاڙي;
    خارجي حرف thread_local_bss_شروعات, thread_local_bss_پڇاڙي;
    ثابت uint32_t tls_معلومات_سائيز = (uint32_t)&thread_local_معلومات_پڇاڙي - (uint32_t)&thread_local_معلومات_شروعات;
    ثابت uint32_t tls_bss_سائيز = (uint32_t)&thread_local_bss_پڇاڙي - (uint32_t)&thread_local_bss_شروعات;
    ثابت uint32_t tls_area_سائيز = ESP_ALIGN_مٿي(tls_معلومات_سائيز + tls_bss_سائيز, 16);
    // TODO: check that TLS area fits the stack

    // Allocate space for the TLS area on the stack. The area must be allocated at a 16-byte aligned address
    ux_ڍير_اشارو = ESP_ALIGN_هيٺ_ڏانهن(ux_ڍير_اشارو - (UBaseType_t)tls_area_سائيز, 16);
    // Initialize the TLS data with the initialization values of each TLS variable
    يادگيري_نقل((فراغ *)ux_ڍير_اشارو, &thread_local_معلومات_شروعات, tls_معلومات_سائيز);
    // Initialize the TLS bss with zeroes
    memset((فراغ *)(ux_ڍير_اشارو + tls_معلومات_سائيز), 0, tls_bss_سائيز);

    /*
    Calculate the THREADPTR register's initialization value based on the link-time offset and the TLS area allocated on
    the stack.

    HIGH ADDRESS
            |---------------------------|
            | .tdata (*)                |
          ^ | int example;              |
          | |                           |
          | | .tbss (*)                 |
          | |---------------------------| <- uxStackPointer (start of TLS area)
    0xNNN | |                           | ^
          | |                           | |
          |             ...               | (_thread_local_start - _flash_rodata_start) + align_up(TCB_SIZE, tls_section_alignment)
          | |                           | |
          | |                           | V
          V |                           | <- threadptr register's value

    LOW ADDRESS

    Note: Xtensa is slightly different compared to the RISC-V port as there is an implicit aligned TCB_SIZE added to
    the offset. (search for 'tpoff' in elf32-xtensa.c in BFD):
        - "offset = address - tls_section_vma + align_up(TCB_SIZE, tls_section_alignment)"
        - TCB_SIZE is hardcoded to 8
    */
    ثابت uint32_t tls_section_align = (uint32_t)&_tls_section_alignment;  // ALIGN value of .flash.tdata section
    #وصف_ڪريو TCB_سائيز 8
    ثابت uint32_t بنياد = ESP_ALIGN_مٿي(TCB_سائيز, tls_section_align);
    *ret_threadptr_reg_init = (uint32_t)ux_ڍير_اشارو - بنياد;

    أرجع ux_ڍير_اشارو;
}

/**
 * @brief Initialize the task's starting interrupt stack frame
 *
 * This function initializes the task's starting interrupt stack frame. The dispatcher will use this stack frame in a
 * context restore routine. Therefore, the starting stack frame must be initialized as if the task was interrupted right
 * before its first instruction is called.
 *
 * - The stack frame is allocated to a 16-byte aligned address
 * - The THREADPTR register is saved in the extra storage area of the stack frame. This is also initialized
 *
 * @param[in] uxStackPointer Current stack pointer address
 * @param[in] pxCode Task function
 * @param[in] pvParameters Task function's parameter
 * @param[in] threadptr_reg_init THREADPTR register initialization value
 * @return Stack pointer that points to the stack frame
 */
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_ڍير_چوکٺو(UBaseType_t ux_ڍير_اشارو, TaskFunction_t px_ڪوڊ, فراغ *pvParameters, uint32_t threadptr_reg_init)
{
    /*
    HIGH ADDRESS
    |---------------------------|       ^ XT_STK_FRMSZ
    |                           |       |
    | Stack Frame Extra Storage |       |
    |                           |       |
    | ------------------------- |       |   ^ XT_STK_EXTRA
    |                           |       |   |
    | Intr/Exc Stack Frame      |       |   |
    |                           |       V   V
    | ------------------------- | ---------------------- 16 byte aligned
    LOW ADDRESS
    */

    /*
    Allocate space for the task's starting interrupt stack frame.
    - The stack frame must be allocated to a 16-byte aligned address.
    - We use XT_STK_FRMSZ (instead of sizeof(XtExcFrame)) as it...
        - includes the size of the extra storage area
        - includes the size for a base save area before the stack frame
        - rounds up the total size to a multiple of 16
    */
    UBaseType_t ux_ڍير_اشارو_اڳيون = ux_ڍير_اشارو;
    ux_ڍير_اشارو = ESP_ALIGN_هيٺ_ڏانهن(ux_ڍير_اشارو - XT_STK_FRMSZ, 16);

    // Clear the entire interrupt stack frame
    memset((فراغ *)ux_ڍير_اشارو, 0, (size_t)(ux_ڍير_اشارو_اڳيون - ux_ڍير_اشارو));

    Xt_Exc_چوکٺو *چوکٺو = (Xt_Exc_چوکٺو *)ux_ڍير_اشارو;

    /*
    Initialize common registers
    */
    چوکٺو->a0 = 0;                                          // Set the return address to 0 terminate GDB backtrace
    چوکٺو->a1 = ux_ڍير_اشارو + XT_STK_FRMSZ;              // Saved stack pointer should point to physical top of stack frame
    چوکٺو->ٻاهر_نڪرو = (UBaseType_t) _xt_user_exit;              // User exception exit dispatcher

    /*
    Initialize the task's entry point. This will differ depending on
    - Whether the task's entry point is the wrapper function or pxCode
    - Whether Windowed ABI is used (for windowed, we mimic the task entry point being call4'd )
    */
    #إذا CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
        چوکٺو->pc = (UBaseType_t) v_پورٽ_Task_Wrapper;         // Task entry point is the wrapper function
        #جيڪڏهن_وصف_ٿيل __XTENSA_CALL0_ABI__
            چوکٺو->a2 = (UBaseType_t) px_ڪوڊ;               // Wrapper function's argument 0 (which is the task function)
            چوکٺو->a3 = (UBaseType_t) pvParameters;         // Wrapper function's argument 1 (which is the task function's argument)
        #وإلا // __XTENSA_CALL0_ABI__
            چوکٺو->a6 = (UBaseType_t) px_ڪوڊ;               // Wrapper function's argument 0 (which is the task function), passed as if we call4'd
            چوکٺو->a7 = (UBaseType_t) pvParameters;         // Wrapper function's argument 1 (which is the task function's argument), passed as if we call4'd
        #جيڪڏهن_پورو // __XTENSA_CALL0_ABI__
    #وإلا
        چوکٺو->pc = (UBaseType_t) px_ڪوڊ;                   // Task entry point is the provided task function
        #جيڪڏهن_وصف_ٿيل __XTENSA_CALL0_ABI__
            چوکٺو->a2 = (UBaseType_t) pvParameters;         // Task function's argument
        #وإلا // __XTENSA_CALL0_ABI__
            چوکٺو->a6 = (UBaseType_t) pvParameters;         // Task function's argument, passed as if we call4'd
        #جيڪڏهن_پورو // __XTENSA_CALL0_ABI__
    #جيڪڏهن_پورو

    /*
    Set initial PS to int level 0, EXCM disabled ('rfe' will enable), user mode.
    For windowed ABI also set WOE and CALLINC (pretend task was 'call4'd)
    */
    #جيڪڏهن_وصف_ٿيل __XTENSA_CALL0_ABI__
        چوکٺو->ps = PS_UM | PS_EXCM;
    #وإلا // __XTENSA_CALL0_ABI__
        چوکٺو->ps = PS_UM | PS_EXCM | PS_WOE | PS_CALLINC(1);
    #جيڪڏهن_پورو // __XTENSA_CALL0_ABI__

    #جيڪڏهن_وصف_ٿيل XT_USE_SWPRI
        // Set the initial virtual priority mask value to all 1's.
        چوکٺو->vpri = 0xFFFFFFFF;
    #جيڪڏهن_پورو

    // Initialize the threadptr register in the extra save area of the stack frame
    uint32_t *threadptr_reg = (uint32_t *)(ux_ڍير_اشارو + XT_STK_EXTRA);
    *threadptr_reg = threadptr_reg_init;

    أرجع ux_ڍير_اشارو;
}

#إذا ( پورٽ_HAS_ڍير_حد_کان_مٿي_CHECKING == 1 )
ڍير_Type_t * px_پورٽ_Initialise_ڍير( ڍير_Type_t * px_چوٽي_جو_ڍير,
                                     ڍير_Type_t * px_پڇاڙي_جو_ڍير,
                                     TaskFunction_t px_ڪوڊ,
                                     فراغ * pvParameters )
#وإلا
ڍير_Type_t * px_پورٽ_Initialise_ڍير( ڍير_Type_t * px_چوٽي_جو_ڍير,
                                     TaskFunction_t px_ڪوڊ,
                                     فراغ * pvParameters )
#جيڪڏهن_پورو
{
#جيڪڏهن_وصف_ٿيل __clang_analyzer__
    // Teach clang-tidy that pxTopOfStack cannot be a pointer to const
    متطاير ڍير_Type_t * px_عارضي = px_چوٽي_جو_ڍير;
    px_چوٽي_جو_ڍير = px_عارضي;
#جيڪڏهن_پورو /*__clang_analyzer__ */
    /*
    HIGH ADDRESS
    |---------------------------| <- pxTopOfStack on entry
    | Coproc Save Area          | (CPSA MUST BE FIRST)
    | ------------------------- |
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
    */
    UBaseType_t ux_ڍير_اشارو = (UBaseType_t)px_چوٽي_جو_ڍير;
    // Make sure the incoming stack pointer is aligned on 16
    configASSERT((ux_ڍير_اشارو & پورٽ_BYTE_ALIGNMENT_نقاب) == 0);
#إذا XCHAL_CP_انگ > 0
    // Initialize the coprocessor save area. THIS MUST BE THE FIRST AREA due to access from _frxt_task_coproc_state()
    ux_ڍير_اشارو = ux_Initialise_ڍير_CPSA(ux_ڍير_اشارو);
    // Each allocated section on the stack must have a size aligned on 16
    configASSERT((ux_ڍير_اشارو & پورٽ_BYTE_ALIGNMENT_نقاب) == 0);
#جيڪڏهن_پورو /* XCHAL_CP_NUM > 0 */

    // Initialize the GCC TLS area
    uint32_t threadptr_reg_init;
    ux_ڍير_اشارو = ux_Initialise_ڍير_TLS(ux_ڍير_اشارو, &threadptr_reg_init);
    configASSERT((ux_ڍير_اشارو & پورٽ_BYTE_ALIGNMENT_نقاب) == 0);

    // Initialize the starting interrupt stack frame
    ux_ڍير_اشارو = ux_Initialise_ڍير_چوکٺو(ux_ڍير_اشارو, px_ڪوڊ, pvParameters, threadptr_reg_init);
    configASSERT((ux_ڍير_اشارو & پورٽ_BYTE_ALIGNMENT_نقاب) == 0);
    // Return the task's current stack pointer address which should point to the starting interrupt stack frame
    أرجع (ڍير_Type_t *)ux_ڍير_اشارو;
}



/* ---------------------------------------------- Port Implementations -------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

بنياد_Type_t x_پورٽ_اندريون_Isr_Context(فراغ)
{
    بنانشان صحيح irq_حالت;
    بنياد_Type_t ret;
    irq_حالت = پورٽ_سيٽ_مداخلت_نقاب_FROM_ISR();
    ret = (پورٽ_مداخلت_Nesting[x_پورٽ_Get_Core_ID()] != 0);
    پورٽ_CLEAR_مداخلت_نقاب_FROM_ISR(irq_حالت);
    أرجع ret;
}

فراغ v_پورٽ_Assert_If_اندريون_ISR(فراغ)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(x_پورٽ_اندريون_Isr_Context() == 0);
}

بنياد_Type_t x_پورٽ_Interrupted_From_ISRContext(فراغ)
{
    أرجع (پورٽ_مداخلت_Nesting[x_پورٽ_Get_Core_ID()] != 0);
}

// ------------------ Critical Sections --------------------

#إذا CONFIG_FREERTOS_پورٽ_THREAD_محفوظ_CLAIM
فراغ x_پورٽ_Thread_محفوظ_Claim(فراغ)
{
    configASSERT(!x_پورٽ_Can_Yield());
    configASSERT(!پورٽ_x_Thread_محفوظ_Claimed);
    پورٽ_x_Thread_محفوظ_Claimed = true;
}

فراغ x_پورٽ_Thread_محفوظ_Disclaim(فراغ)
{
    configASSERT(!x_پورٽ_Can_Yield());
    configASSERT(پورٽ_x_Thread_محفوظ_Claimed);
    پورٽ_x_Thread_محفوظ_Claimed = false;
}
#جيڪڏهن_پورو /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

بنياد_Type_t __attribute__((optimize("-O3"))) x_پورٽ_Enter_Critical_Timeout(پورٽ_MUX_TYPE *mux, بنياد_Type_t timeout)
{
#إذا CONFIG_FREERTOS_پورٽ_THREAD_محفوظ_CLAIM
    إذا (unlikely(پورٽ_x_Thread_محفوظ_Claimed)) {
        أرجع pdPASS;
    }
#جيڪڏهن_پورو
    /* Interrupts may already be disabled (if this function is called in nested
     * manner). However, there's no atomic operation that will allow us to check,
     * thus we have to disable interrupts again anyways.
     *
     * However, if this is call is NOT nested (i.e., the first call to enter a
     * critical section), we will save the previous interrupt level so that the
     * saved level can be restored on the last call to exit the critical.
     */
    بنياد_Type_t x_Old_مداخلت_Level = پورٽ_سيٽ_مداخلت_نقاب_FROM_ISR();
    /* Interrupts are masked, so the core id is stable. Read the core id register once
     * (its value is already the spinlock owner id on Xtensa) and reuse it, so
     * spinlock_acquire does not read the core id register a second time. */
    uint32_t coreOwnerId = spinlock_owner_id();
    بنياد_Type_t coreID = spinlock_core_id_from_owner_id(coreOwnerId);
    إذا (!spinlock_acquire_impl(mux, timeout, coreOwnerId)) {
        //Timed out attempting to get spinlock. Restore previous interrupt level and return
        پورٽ_CLEAR_مداخلت_نقاب_FROM_ISR(x_Old_مداخلت_Level);
        أرجع pdFAIL;
    }
    //Spinlock acquired. Increment the critical nesting count.
    بنياد_Type_t newNesting = پورٽ_ux_Critical_Nesting[coreID] + 1;
    پورٽ_ux_Critical_Nesting[coreID] = newNesting;
    //If this is the first entry to a critical section. Save the old interrupt level.
    إذا ( newNesting == 1 ) {
        پورٽ_ux_Old_مداخلت_State[coreID] = x_Old_مداخلت_Level;
    }
    أرجع pdPASS;
}

فراغ __attribute__((optimize("-O3"))) v_پورٽ_Exit_Critical(پورٽ_MUX_TYPE *mux)
{
#إذا CONFIG_FREERTOS_پورٽ_THREAD_محفوظ_CLAIM
    إذا (unlikely(پورٽ_x_Thread_محفوظ_Claimed)) {
        أرجع;
    }
#جيڪڏهن_پورو
    /* This function may be called in a nested manner. Therefore, we only need
     * to re-enable interrupts if this is the last call to exit the critical. We
     * can use the nesting count to determine whether this is the last exit call.
     */
    /* Interrupts remain disabled for the whole critical section, so the core id is
     * stable and the spinlock does not need to mask them again. Read the core id
     * register once (its value is the spinlock owner id on Xtensa) and reuse it. */
    uint32_t coreOwnerId = spinlock_owner_id();
    بنياد_Type_t coreID = spinlock_core_id_from_owner_id(coreOwnerId);
    spinlock_release_impl(mux, coreOwnerId);
    بنياد_Type_t nesting = پورٽ_ux_Critical_Nesting[coreID];

    /* Critical section nesting count must never be negative */
    configASSERT( nesting > 0 );

    إذا (nesting > 0) {
        nesting--;
        پورٽ_ux_Critical_Nesting[coreID] = nesting;

        //This is the last exit call, restore the saved interrupt level
        إذا ( nesting == 0 ) {
            پورٽ_CLEAR_مداخلت_نقاب_FROM_ISR(پورٽ_ux_Old_مداخلت_State[coreID]);
        }
    }
}

بنياد_Type_t x_پورٽ_Enter_Critical_Timeout_Compliance(پورٽ_MUX_TYPE *mux, بنياد_Type_t timeout)
{
    بنياد_Type_t ret;
    إذا (!x_پورٽ_اندريون_Isr_Context()) {
        ret = x_پورٽ_Enter_Critical_Timeout(mux, timeout);
    } وإلا {
        esp_rom_printf("port*_CRITICAL called from ISR context. Aborting!\n");
        abort();
        ret = pdFAIL;
    }
    أرجع ret;
}

فراغ v_پورٽ_Exit_Critical_Compliance(پورٽ_MUX_TYPE *mux)
{
    إذا (!x_پورٽ_اندريون_Isr_Context()) {
        v_پورٽ_Exit_Critical(mux);
    } وإلا {
        esp_rom_printf("port*_CRITICAL called from ISR context. Aborting!\n");
        abort();
    }
}

// ---------------------- Yielding -------------------------

فراغ v_پورٽ_Yield_Other_Core( بنياد_Type_t coreid )
{
    esp_crosscore_int_send_yield( coreid );
}

// ------------------- Hook Functions ----------------------

فراغ  __attribute__((weak)) v_Application_ڍير_حد_کان_مٿي_Hook( TaskHandle_t xTask, حرف *pcTaskName )
{
#وصف_ڪريو ERR_STR1 "***ERROR*** A stack overflow in task "
#وصف_ڪريو ERR_STR2 " has been detected."
    ثابت حرف *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    حرف buf[حجم(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + حجم(ERR_STR2) + 1 /* null char */] = { 0 };

    حرف *dest = buf;
    لكل (size_t i = 0 ; i < حجم(str) / حجم(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
    esp_system_abort(buf);
}

// ----------------------- System --------------------------

uint32_t x_پورٽ_Get_Tick_شرح_Hz(فراغ)
{
    أرجع (uint32_t)config_TICK_شرح_HZ;
}


#وصف_ڪريو ڍير_WATCH_AREA_سائيز 32
#وصف_ڪريو ڍير_WATCH_نقطو_عدد (SOC_CPU_WATCHPOINTS_انگ - 1)

فراغ v_پورٽ_سيٽ_ڍير_Watchpoint( فراغ *px_ڍير_شروعات )
{
    //Set watchpoint 1 to watch the last 32 bytes of the stack.
    //Unfortunately, the Xtensa watchpoints can't set a watchpoint on a random [base - base+n] region because
    //the size works by masking off the lowest address bits. For that reason, we futz a bit and watch the lowest 32
    //bytes of the stack we can actually watch. In general, this can cause the watchpoint to be triggered at most
    //28 bytes early. The value 32 is chosen because it's larger than the stack canary, which in FreeRTOS is 20 bytes.
    //This way, we make sure we trigger before/when the stack canary is corrupted, not after.
    صحيح addr = (صحيح)px_ڍير_شروعات;
    addr = (addr + 31) & (~31);
    esp_cpu_سيٽ_watchpoint(ڍير_WATCH_نقطو_عدد, (حرف *)addr, 32, ESP_CPU_WATCHPOINT_STORE);
}

// --------------------- TCB Cleanup -----------------------

#إذا ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
ساكن فراغ v_پورٽ_TLSPointers_Del_Cb( فراغ *pxTCB )
{
    /* Typecast pxTCB to StaticTask_t type to access TCB struct members.
     * pvDummy15 corresponds to pvThreadLocalStoragePointers member of the TCB.
     */
    StaticTask_t *tcb = ( StaticTask_t * )pxTCB;

    /* The TLSP deletion callbacks are stored at an offset of (configNUM_THREAD_LOCAL_STORAGE_POINTERS/2) */
    TlsDeleteCallbackFunction_t *pv_Thread_Local_Storage_اشارا_Del_Callback = ( TlsDeleteCallbackFunction_t * )( &( tcb->pvDummy15[ ( config_انگ_THREAD_LOCAL_STORAGE_اشارا / 2 ) ] ) );

    /* We need to iterate over half the depth of the pvThreadLocalStoragePointers area
     * to access all TLS pointers and their respective TLS deletion callbacks.
     */
    لكل ( صحيح x = 0; x < ( config_انگ_THREAD_LOCAL_STORAGE_اشارا / 2 ); x++ ) {
        إذا ( pv_Thread_Local_Storage_اشارا_Del_Callback[ x ] != عدم ) {  //If del cb is set
            /* In case the TLSP deletion callback has been overwritten by a TLS pointer, gracefully abort. */
            إذا ( !esp_ptr_executable( pv_Thread_Local_Storage_اشارا_Del_Callback[ x ] ) ) {
                // We call EARLY log here as currently portCLEAN_UP_TCB() is called in a critical section
                ESP_EARLY_LOGE("FreeRTOS", "Fatal error: TLSP deletion callback at index %d overwritten with non-excutable pointer %p", x, pv_Thread_Local_Storage_اشارا_Del_Callback[ x ]);
                abort();
            }

            pv_Thread_Local_Storage_اشارا_Del_Callback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#جيڪڏهن_پورو /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

#إذا ( XCHAL_CP_انگ > 0 )
ساكن فراغ v_پورٽ_Clean_مٿي_Coproc_Area(فراغ *pvTCB)
{
    UBaseType_t uxCoprocArea;
    بنياد_Type_t x_هدف_Core_ID;

    /* Get a pointer to the task's coprocessor save area */
    uxCoprocArea = ( UBaseType_t ) ( ( ( StaticTask_t * ) pvTCB )->pxDummy8 );  /* Get TCB_t.pxEndOfStack */
    uxCoprocArea = ESP_ALIGN_هيٺ_ڏانهن(uxCoprocArea - XT_CP_سائيز, 16);

    #إذا ( config_عدد_جو_CORES > 1 )
        /* Get xTargetCoreID from the TCB.xCoreID */
        x_هدف_Core_ID = ( ( StaticTask_t * ) pvTCB )->xDummyCoreID;
    #وإلا /* configNUMBER_OF_CORES > 1 */
        x_هدف_Core_ID = 0;
    #جيڪڏهن_پورو /* configNUMBER_OF_CORES > 1 */

    /* If task has live floating point registers somewhere, release them */
    فراغ _xt_coproc_release(متطاير فراغ *coproc_sa_بنياد, بنياد_Type_t x_هدف_Core_ID);
    _xt_coproc_release( (فراغ *)uxCoprocArea, x_هدف_Core_ID );
}
#جيڪڏهن_پورو /* XCHAL_CP_NUM > 0 */

فراغ v_پورٽ_TCBPre_Delete_Hook( فراغ *pxTCB )
{
    #إذا ( CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK )
        /* Call the user defined task pre-deletion hook */
        خارجي فراغ vTaskPreDeletionHook( فراغ * pxTCB );
        vTaskPreDeletionHook( pxTCB );
    #جيڪڏهن_پورو /* CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK */

    #إذا ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
        /* Call TLS pointers deletion callbacks */
        v_پورٽ_TLSPointers_Del_Cb( pxTCB );
    #جيڪڏهن_پورو /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

    #إذا ( XCHAL_CP_انگ > 0 )
        /* Cleanup coproc save area */
        v_پورٽ_Clean_مٿي_Coproc_Area( pxTCB );
    #جيڪڏهن_پورو /* XCHAL_CP_NUM > 0 */
}

/* ------------------------------------------------ Run Time Stats ------------------------------------------------- */

#إذا ( CONFIG_FREERTOS_GENERATE_لاڳيتو_وقت_STATS )

config_لاڳيتو_وقت_COUNTER_TYPE x_پورٽ_Get_لاڳيتو_وقت_Counter_قدر( فراغ )
{
#جيڪڏهن_وصف_ٿيل CONFIG_FREERTOS_لاڳيتو_وقت_STATS_USING_ESP_TIMER
    أرجع (config_لاڳيتو_وقت_COUNTER_TYPE) esp_timer_get_وقت();
#وإلا // Uses CCOUNT
    أرجع (config_لاڳيتو_وقت_COUNTER_TYPE) xthal_get_ccount();
#جيڪڏهن_پورو // CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
}

#جيڪڏهن_پورو /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
