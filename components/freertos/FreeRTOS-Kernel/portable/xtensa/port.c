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

#شامل "sdkconfig.h"
#شامل <stdbool.h>
#شامل <stdint.h>
#شامل <stdlib.h>
#شامل <string.h>
#شامل <xtensa/config/core.h>
#شامل <xtensa_context.h>
#شامل "soc/soc_caps.h"
#شامل "esp_attr.h"
#شامل "esp_compiler.h"
#شامل "esp_private/crosscore_int.h"
#شامل "esp_private/esp_int_wdt.h"
#شامل "esp_system.h"
#شامل "esp_log.h"
#شامل "FreeRTOS.h"        /* This pulls in portmacro.h */
#شامل "task.h"            /* Required for TaskHandle_t, tskNO_AFFINITY, and vTaskStartScheduler */
#شامل "port_systick.h"
#شامل "esp_cpu.h"
#شامل <xtensa/hal.h>             /* required for xthal_get_ccount() */
#اگر CONFIG_FREERTOS_تسلسل_وقت_STATS_USING_ESP_TIMER
#شامل "esp_timer.h"
#اگر_ختم
#شامل "esp_memory_utils.h"
#شامل "esp_macros.h"

_Static_assert(پورٹ_BYTE_ALIGNMENT == 16, "portBYTE_ALIGNMENT must be set to 16");

/* ---------------------------------------------------- Variables ------------------------------------------------------
 * - Various variables used to maintain the FreeRTOS port's state. Used from both port.c and various .S files
 * - Constant offsets are used by assembly to jump to particular TCB members or a stack area (such as the CPSA). We use
 *   C constants instead of preprocessor macros due to assembly lacking "offsetof()".
 * ------------------------------------------------------------------------------------------------------------------ */

#اگر XCHAL_CP_عدد > 0
/* Offsets used to navigate to a task's CPSA on the stack */
ثابت DRAM_ATTR uint32_t انحراف_px_انتہا_کا_ڈھیر = offsetof(StaticTask_t, pxDummy8);
ثابت DRAM_ATTR uint32_t انحراف_cpsa = XT_CP_سائز;  /* Offset to start of the CPSA area on the stack. See uxInitialiseStackCPSA(). */
#اگر config_عدد_CORES > 1
/* Offset to TCB_t.xCoreID member. Used to pin unpinned tasks that use the FPU. */
ثابت DRAM_ATTR uint32_t انحراف_x_Core_ID = offsetof(StaticTask_t, xDummyCoreID);
#اگر_ختم /* configNUM_CORES > 1 */
#اگر_ختم /* XCHAL_CP_NUM > 0 */

متلون غیرعلامتی پورٹ_x_Scheduler_Running[پورٹ_عدد_PROCESSORS] = {0}; // Indicates whether scheduler is running on a per-core basis
غیرعلامتی پورٹ_مداخلت_Nesting[پورٹ_عدد_PROCESSORS] = {0};  // Interrupt nesting level. Increased/decreased in portasm.c, _frxt_int_enter/_frxt_int_exit
بنیاد_Type_t پورٹ_ux_Critical_Nesting[پورٹ_عدد_PROCESSORS] = {0};
#اگر CONFIG_FREERTOS_پورٹ_THREAD_محفوظ_CLAIM
متلون bool پورٹ_x_Thread_محفوظ_Claimed = false;
#اگر_ختم
بنیاد_Type_t پورٹ_ux_Old_مداخلت_State[پورٹ_عدد_PROCESSORS] = {0};
متلون غیرعلامتی پورٹ_ux_Core_Startup_مکمل[پورٹ_عدد_PROCESSORS] = {0};  // Indicates whether the core has completed its startup sequence

/*
*******************************************************************************
* Interrupt stack. The size of the interrupt stack is determined by the config
* parameter "configISR_STACK_SIZE" in FreeRTOSConfig.h
*******************************************************************************
*/
متلون ڈھیر_Type_t DRAM_ATTR __attribute__((aligned(16))) پورٹ_Int_ڈھیر[پورٹ_عدد_PROCESSORS][config_ISR_ڈھیر_سائز];
/* One flag for each individual CPU. */
متلون uint32_t پورٹ_switch_flag[پورٹ_عدد_PROCESSORS];

/* ------------------------------------------------ FreeRTOS Portable --------------------------------------------------
 * - Provides implementation for functions required by FreeRTOS
 * - Declared in portable.h
 * ------------------------------------------------------------------------------------------------------------------ */

// ----------------- Scheduler Start/End -------------------

/* Defined in xtensa_context.S */
خارجی خالی _xt_coproc_init(خالی);

بنیاد_Type_t x_پورٹ_آغاز_Scheduler( خالی )
{
    پورٹ_DISABLE_INTERRUPTS();
    // Interrupts are disabled at this point and stack contains PS with enabled interrupts when task context is restored

#اگر XCHAL_CP_عدد > 0
    /* Initialize co-processor management for tasks. Leave CPENABLE alone. */
    _xt_coproc_init();
#اگر_ختم

    /* Setup the hardware to generate the tick. */
    v_پورٹ_تیاری_Timer();

    /* Initialize all kernel state tracking variables */
    بنیاد_Type_t coreID = x_پورٹ_Get_Core_ID();
    پورٹ_x_Scheduler_Running[coreID] = 1;
    پورٹ_ux_Core_Startup_مکمل[coreID] = 0;

    // Windows contain references to the startup stack which will be reclaimed by the main task
    // Spill the windows to create a clean environment to ensure we do not carry over any such references
    // to invalid SPs which will cause problems if main_task does a windowoverflow to them
    xthal_کھڑکی_spill();

    // Cannot be directly called from C; never returns
    __asm__ متلون ("call0    _frxt_dispatch\n");

    /* Should not get here. */
    واپس pdTRUE;
}

خالی v_پورٹ_انتہا_Scheduler( خالی )
{
    /* It is unlikely that the Xtensa port will get stopped.  If required simply
    disable the tick interrupt here. */
    abort();
}

// ------------------------ Stack --------------------------

// User exception dispatcher when exiting
خالی _xt_user_exit(خالی);

#اگر CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
// Wrapper to allow task functions to return (increases stack overhead by 16 bytes)
ساکن خالی v_پورٹ_Task_Wrapper(TaskFunction_t px_رمز, خالی *pvParameters)
{
    px_رمز(pvParameters);
    //FreeRTOS tasks should not return. Log the task name and abort.
    حرف *pcTaskName = pcTaskGetName(تہی);
    ESP_LOGE("FreeRTOS", "FreeRTOS Task \"%s\" should not return, Aborting now!", pcTaskName);
    abort();
}
#اگر_ختم



#اگر XCHAL_CP_عدد > 0
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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_ڈھیر_CPSA(UBaseType_t ux_ڈھیر_اشارہ_گر)
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
    ux_ڈھیر_اشارہ_گر = ESP_ALIGN_نیچے_کو(ux_ڈھیر_اشارہ_گر - XT_CP_سائز, 16);
    // Initialize the coprocessor context switching flags.
    uint32_t *p = (uint32_t *)ux_ڈھیر_اشارہ_گر;
    p[0] = 0;   // Clear XT_CPENABLE and XT_CPSTORED
    p[1] = 0;   // Clear XT_CP_CS_ST
    // XT_CP_ASA points to the aligned start of the individual CP save areas (i.e., start of CP0 SA)
    p[2] = (uint32_t)ESP_ALIGN_اوپر((uint32_t)ux_ڈھیر_اشارہ_گر + 12, XCHAL_کل_SA_ALIGN);
    واپس ux_ڈھیر_اشارہ_گر;
}
#اگر_ختم /* XCHAL_CP_NUM > 0 */

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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_ڈھیر_TLS(UBaseType_t ux_ڈھیر_اشارہ_گر, uint32_t *ret_threadptr_reg_init)
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
    خارجی صحیح _tls_section_alignment;
    خارجی حرف thread_local_ڈیٹا_آغاز, thread_local_ڈیٹا_انتہا;
    خارجی حرف thread_local_bss_آغاز, thread_local_bss_انتہا;
    ثابت uint32_t tls_ڈیٹا_سائز = (uint32_t)&thread_local_ڈیٹا_انتہا - (uint32_t)&thread_local_ڈیٹا_آغاز;
    ثابت uint32_t tls_bss_سائز = (uint32_t)&thread_local_bss_انتہا - (uint32_t)&thread_local_bss_آغاز;
    ثابت uint32_t tls_area_سائز = ESP_ALIGN_اوپر(tls_ڈیٹا_سائز + tls_bss_سائز, 16);
    // TODO: check that TLS area fits the stack

    // Allocate space for the TLS area on the stack. The area must be allocated at a 16-byte aligned address
    ux_ڈھیر_اشارہ_گر = ESP_ALIGN_نیچے_کو(ux_ڈھیر_اشارہ_گر - (UBaseType_t)tls_area_سائز, 16);
    // Initialize the TLS data with the initialization values of each TLS variable
    حافظہ_نقل((خالی *)ux_ڈھیر_اشارہ_گر, &thread_local_ڈیٹا_آغاز, tls_ڈیٹا_سائز);
    // Initialize the TLS bss with zeroes
    memset((خالی *)(ux_ڈھیر_اشارہ_گر + tls_ڈیٹا_سائز), 0, tls_bss_سائز);

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
    #تعریف_کرو TCB_سائز 8
    ثابت uint32_t بنیاد = ESP_ALIGN_اوپر(TCB_سائز, tls_section_align);
    *ret_threadptr_reg_init = (uint32_t)ux_ڈھیر_اشارہ_گر - بنیاد;

    واپس ux_ڈھیر_اشارہ_گر;
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
FORCE_INLINE_ATTR UBaseType_t ux_Initialise_ڈھیر_چوکھٹا(UBaseType_t ux_ڈھیر_اشارہ_گر, TaskFunction_t px_رمز, خالی *pvParameters, uint32_t threadptr_reg_init)
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
    UBaseType_t ux_ڈھیر_اشارہ_گر_پچھلا = ux_ڈھیر_اشارہ_گر;
    ux_ڈھیر_اشارہ_گر = ESP_ALIGN_نیچے_کو(ux_ڈھیر_اشارہ_گر - XT_STK_FRMSZ, 16);

    // Clear the entire interrupt stack frame
    memset((خالی *)ux_ڈھیر_اشارہ_گر, 0, (size_t)(ux_ڈھیر_اشارہ_گر_پچھلا - ux_ڈھیر_اشارہ_گر));

    Xt_Exc_چوکھٹا *چوکھٹا = (Xt_Exc_چوکھٹا *)ux_ڈھیر_اشارہ_گر;

    /*
    Initialize common registers
    */
    چوکھٹا->a0 = 0;                                          // Set the return address to 0 terminate GDB backtrace
    چوکھٹا->a1 = ux_ڈھیر_اشارہ_گر + XT_STK_FRMSZ;              // Saved stack pointer should point to physical top of stack frame
    چوکھٹا->باہر_نکلو = (UBaseType_t) _xt_user_exit;              // User exception exit dispatcher

    /*
    Initialize the task's entry point. This will differ depending on
    - Whether the task's entry point is the wrapper function or pxCode
    - Whether Windowed ABI is used (for windowed, we mimic the task entry point being call4'd )
    */
    #اگر CONFIG_FREERTOS_TASK_FUNCTION_WRAPPER
        چوکھٹا->pc = (UBaseType_t) v_پورٹ_Task_Wrapper;         // Task entry point is the wrapper function
        #اگر_تعریف_شدہ __XTENSA_CALL0_ABI__
            چوکھٹا->a2 = (UBaseType_t) px_رمز;               // Wrapper function's argument 0 (which is the task function)
            چوکھٹا->a3 = (UBaseType_t) pvParameters;         // Wrapper function's argument 1 (which is the task function's argument)
        #ورنہ // __XTENSA_CALL0_ABI__
            چوکھٹا->a6 = (UBaseType_t) px_رمز;               // Wrapper function's argument 0 (which is the task function), passed as if we call4'd
            چوکھٹا->a7 = (UBaseType_t) pvParameters;         // Wrapper function's argument 1 (which is the task function's argument), passed as if we call4'd
        #اگر_ختم // __XTENSA_CALL0_ABI__
    #ورنہ
        چوکھٹا->pc = (UBaseType_t) px_رمز;                   // Task entry point is the provided task function
        #اگر_تعریف_شدہ __XTENSA_CALL0_ABI__
            چوکھٹا->a2 = (UBaseType_t) pvParameters;         // Task function's argument
        #ورنہ // __XTENSA_CALL0_ABI__
            چوکھٹا->a6 = (UBaseType_t) pvParameters;         // Task function's argument, passed as if we call4'd
        #اگر_ختم // __XTENSA_CALL0_ABI__
    #اگر_ختم

    /*
    Set initial PS to int level 0, EXCM disabled ('rfe' will enable), user mode.
    For windowed ABI also set WOE and CALLINC (pretend task was 'call4'd)
    */
    #اگر_تعریف_شدہ __XTENSA_CALL0_ABI__
        چوکھٹا->ps = PS_UM | PS_EXCM;
    #ورنہ // __XTENSA_CALL0_ABI__
        چوکھٹا->ps = PS_UM | PS_EXCM | PS_WOE | PS_CALLINC(1);
    #اگر_ختم // __XTENSA_CALL0_ABI__

    #اگر_تعریف_شدہ XT_USE_SWPRI
        // Set the initial virtual priority mask value to all 1's.
        چوکھٹا->vpri = 0xFFFFFFFF;
    #اگر_ختم

    // Initialize the threadptr register in the extra save area of the stack frame
    uint32_t *threadptr_reg = (uint32_t *)(ux_ڈھیر_اشارہ_گر + XT_STK_EXTRA);
    *threadptr_reg = threadptr_reg_init;

    واپس ux_ڈھیر_اشارہ_گر;
}

#اگر ( پورٹ_HAS_ڈھیر_حد_سے_تجاوز_CHECKING == 1 )
ڈھیر_Type_t * px_پورٹ_Initialise_ڈھیر( ڈھیر_Type_t * px_چوٹی_کا_ڈھیر,
                                     ڈھیر_Type_t * px_انتہا_کا_ڈھیر,
                                     TaskFunction_t px_رمز,
                                     خالی * pvParameters )
#ورنہ
ڈھیر_Type_t * px_پورٹ_Initialise_ڈھیر( ڈھیر_Type_t * px_چوٹی_کا_ڈھیر,
                                     TaskFunction_t px_رمز,
                                     خالی * pvParameters )
#اگر_ختم
{
#اگر_تعریف_شدہ __clang_analyzer__
    // Teach clang-tidy that pxTopOfStack cannot be a pointer to const
    متلون ڈھیر_Type_t * px_عارضی = px_چوٹی_کا_ڈھیر;
    px_چوٹی_کا_ڈھیر = px_عارضی;
#اگر_ختم /*__clang_analyzer__ */
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
    UBaseType_t ux_ڈھیر_اشارہ_گر = (UBaseType_t)px_چوٹی_کا_ڈھیر;
    // Make sure the incoming stack pointer is aligned on 16
    configASSERT((ux_ڈھیر_اشارہ_گر & پورٹ_BYTE_ALIGNMENT_نقاب) == 0);
#اگر XCHAL_CP_عدد > 0
    // Initialize the coprocessor save area. THIS MUST BE THE FIRST AREA due to access from _frxt_task_coproc_state()
    ux_ڈھیر_اشارہ_گر = ux_Initialise_ڈھیر_CPSA(ux_ڈھیر_اشارہ_گر);
    // Each allocated section on the stack must have a size aligned on 16
    configASSERT((ux_ڈھیر_اشارہ_گر & پورٹ_BYTE_ALIGNMENT_نقاب) == 0);
#اگر_ختم /* XCHAL_CP_NUM > 0 */

    // Initialize the GCC TLS area
    uint32_t threadptr_reg_init;
    ux_ڈھیر_اشارہ_گر = ux_Initialise_ڈھیر_TLS(ux_ڈھیر_اشارہ_گر, &threadptr_reg_init);
    configASSERT((ux_ڈھیر_اشارہ_گر & پورٹ_BYTE_ALIGNMENT_نقاب) == 0);

    // Initialize the starting interrupt stack frame
    ux_ڈھیر_اشارہ_گر = ux_Initialise_ڈھیر_چوکھٹا(ux_ڈھیر_اشارہ_گر, px_رمز, pvParameters, threadptr_reg_init);
    configASSERT((ux_ڈھیر_اشارہ_گر & پورٹ_BYTE_ALIGNMENT_نقاب) == 0);
    // Return the task's current stack pointer address which should point to the starting interrupt stack frame
    واپس (ڈھیر_Type_t *)ux_ڈھیر_اشارہ_گر;
}



/* ---------------------------------------------- Port Implementations -------------------------------------------------
 *
 * ------------------------------------------------------------------------------------------------------------------ */

// --------------------- Interrupts ------------------------

بنیاد_Type_t x_پورٹ_اندر_Isr_Context(خالی)
{
    غیرعلامتی صحیح irq_حالت;
    بنیاد_Type_t ret;
    irq_حالت = پورٹ_سیٹ_مداخلت_نقاب_FROM_ISR();
    ret = (پورٹ_مداخلت_Nesting[x_پورٹ_Get_Core_ID()] != 0);
    پورٹ_CLEAR_مداخلت_نقاب_FROM_ISR(irq_حالت);
    واپس ret;
}

خالی v_پورٹ_Assert_If_اندر_ISR(خالی)
{
    /* Assert if the interrupt nesting count is > 0 */
    configASSERT(x_پورٹ_اندر_Isr_Context() == 0);
}

بنیاد_Type_t x_پورٹ_Interrupted_From_ISRContext(خالی)
{
    واپس (پورٹ_مداخلت_Nesting[x_پورٹ_Get_Core_ID()] != 0);
}

// ------------------ Critical Sections --------------------

#اگر CONFIG_FREERTOS_پورٹ_THREAD_محفوظ_CLAIM
خالی x_پورٹ_Thread_محفوظ_Claim(خالی)
{
    configASSERT(!x_پورٹ_Can_Yield());
    configASSERT(!پورٹ_x_Thread_محفوظ_Claimed);
    پورٹ_x_Thread_محفوظ_Claimed = true;
}

خالی x_پورٹ_Thread_محفوظ_Disclaim(خالی)
{
    configASSERT(!x_پورٹ_Can_Yield());
    configASSERT(پورٹ_x_Thread_محفوظ_Claimed);
    پورٹ_x_Thread_محفوظ_Claimed = false;
}
#اگر_ختم /* CONFIG_FREERTOS_PORT_THREAD_SAFE_CLAIM */

بنیاد_Type_t __attribute__((optimize("-O3"))) x_پورٹ_Enter_Critical_Timeout(پورٹ_MUX_TYPE *mux, بنیاد_Type_t timeout)
{
#اگر CONFIG_FREERTOS_پورٹ_THREAD_محفوظ_CLAIM
    اگر (unlikely(پورٹ_x_Thread_محفوظ_Claimed)) {
        واپس pdPASS;
    }
#اگر_ختم
    /* Interrupts may already be disabled (if this function is called in nested
     * manner). However, there's no atomic operation that will allow us to check,
     * thus we have to disable interrupts again anyways.
     *
     * However, if this is call is NOT nested (i.e., the first call to enter a
     * critical section), we will save the previous interrupt level so that the
     * saved level can be restored on the last call to exit the critical.
     */
    بنیاد_Type_t x_Old_مداخلت_Level = پورٹ_سیٹ_مداخلت_نقاب_FROM_ISR();
    /* Interrupts are masked, so the core id is stable. Read the core id register once
     * (its value is already the spinlock owner id on Xtensa) and reuse it, so
     * spinlock_acquire does not read the core id register a second time. */
    uint32_t coreOwnerId = spinlock_owner_id();
    بنیاد_Type_t coreID = spinlock_core_id_from_owner_id(coreOwnerId);
    اگر (!spinlock_acquire_impl(mux, timeout, coreOwnerId)) {
        //Timed out attempting to get spinlock. Restore previous interrupt level and return
        پورٹ_CLEAR_مداخلت_نقاب_FROM_ISR(x_Old_مداخلت_Level);
        واپس pdFAIL;
    }
    //Spinlock acquired. Increment the critical nesting count.
    بنیاد_Type_t newNesting = پورٹ_ux_Critical_Nesting[coreID] + 1;
    پورٹ_ux_Critical_Nesting[coreID] = newNesting;
    //If this is the first entry to a critical section. Save the old interrupt level.
    اگر ( newNesting == 1 ) {
        پورٹ_ux_Old_مداخلت_State[coreID] = x_Old_مداخلت_Level;
    }
    واپس pdPASS;
}

خالی __attribute__((optimize("-O3"))) v_پورٹ_Exit_Critical(پورٹ_MUX_TYPE *mux)
{
#اگر CONFIG_FREERTOS_پورٹ_THREAD_محفوظ_CLAIM
    اگر (unlikely(پورٹ_x_Thread_محفوظ_Claimed)) {
        واپس;
    }
#اگر_ختم
    /* This function may be called in a nested manner. Therefore, we only need
     * to re-enable interrupts if this is the last call to exit the critical. We
     * can use the nesting count to determine whether this is the last exit call.
     */
    /* Interrupts remain disabled for the whole critical section, so the core id is
     * stable and the spinlock does not need to mask them again. Read the core id
     * register once (its value is the spinlock owner id on Xtensa) and reuse it. */
    uint32_t coreOwnerId = spinlock_owner_id();
    بنیاد_Type_t coreID = spinlock_core_id_from_owner_id(coreOwnerId);
    spinlock_release_impl(mux, coreOwnerId);
    بنیاد_Type_t nesting = پورٹ_ux_Critical_Nesting[coreID];

    /* Critical section nesting count must never be negative */
    configASSERT( nesting > 0 );

    اگر (nesting > 0) {
        nesting--;
        پورٹ_ux_Critical_Nesting[coreID] = nesting;

        //This is the last exit call, restore the saved interrupt level
        اگر ( nesting == 0 ) {
            پورٹ_CLEAR_مداخلت_نقاب_FROM_ISR(پورٹ_ux_Old_مداخلت_State[coreID]);
        }
    }
}

بنیاد_Type_t x_پورٹ_Enter_Critical_Timeout_Compliance(پورٹ_MUX_TYPE *mux, بنیاد_Type_t timeout)
{
    بنیاد_Type_t ret;
    اگر (!x_پورٹ_اندر_Isr_Context()) {
        ret = x_پورٹ_Enter_Critical_Timeout(mux, timeout);
    } ورنہ {
        esp_rom_printf("port*_CRITICAL called from ISR context. Aborting!\n");
        abort();
        ret = pdFAIL;
    }
    واپس ret;
}

خالی v_پورٹ_Exit_Critical_Compliance(پورٹ_MUX_TYPE *mux)
{
    اگر (!x_پورٹ_اندر_Isr_Context()) {
        v_پورٹ_Exit_Critical(mux);
    } ورنہ {
        esp_rom_printf("port*_CRITICAL called from ISR context. Aborting!\n");
        abort();
    }
}

// ---------------------- Yielding -------------------------

خالی v_پورٹ_Yield_Other_Core( بنیاد_Type_t coreid )
{
    esp_crosscore_int_send_yield( coreid );
}

// ------------------- Hook Functions ----------------------

خالی  __attribute__((weak)) v_Application_ڈھیر_حد_سے_تجاوز_Hook( TaskHandle_t xTask, حرف *pcTaskName )
{
#تعریف_کرو ERR_STR1 "***ERROR*** A stack overflow in task "
#تعریف_کرو ERR_STR2 " has been detected."
    ثابت حرف *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    حرف buf[حجم(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + حجم(ERR_STR2) + 1 /* null char */] = { 0 };

    حرف *dest = buf;
    برائے (size_t i = 0 ; i < حجم(str) / حجم(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
    esp_system_abort(buf);
}

// ----------------------- System --------------------------

uint32_t x_پورٹ_Get_Tick_شرح_Hz(خالی)
{
    واپس (uint32_t)config_TICK_شرح_HZ;
}


#تعریف_کرو ڈھیر_WATCH_AREA_سائز 32
#تعریف_کرو ڈھیر_WATCH_نقطہ_نمبر (SOC_CPU_WATCHPOINTS_عدد - 1)

خالی v_پورٹ_سیٹ_ڈھیر_Watchpoint( خالی *px_ڈھیر_آغاز )
{
    //Set watchpoint 1 to watch the last 32 bytes of the stack.
    //Unfortunately, the Xtensa watchpoints can't set a watchpoint on a random [base - base+n] region because
    //the size works by masking off the lowest address bits. For that reason, we futz a bit and watch the lowest 32
    //bytes of the stack we can actually watch. In general, this can cause the watchpoint to be triggered at most
    //28 bytes early. The value 32 is chosen because it's larger than the stack canary, which in FreeRTOS is 20 bytes.
    //This way, we make sure we trigger before/when the stack canary is corrupted, not after.
    صحیح addr = (صحیح)px_ڈھیر_آغاز;
    addr = (addr + 31) & (~31);
    esp_cpu_سیٹ_watchpoint(ڈھیر_WATCH_نقطہ_نمبر, (حرف *)addr, 32, ESP_CPU_WATCHPOINT_STORE);
}

// --------------------- TCB Cleanup -----------------------

#اگر ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
ساکن خالی v_پورٹ_TLSPointers_Del_Cb( خالی *pxTCB )
{
    /* Typecast pxTCB to StaticTask_t type to access TCB struct members.
     * pvDummy15 corresponds to pvThreadLocalStoragePointers member of the TCB.
     */
    StaticTask_t *tcb = ( StaticTask_t * )pxTCB;

    /* The TLSP deletion callbacks are stored at an offset of (configNUM_THREAD_LOCAL_STORAGE_POINTERS/2) */
    TlsDeleteCallbackFunction_t *pv_Thread_Local_Storage_اشارے_Del_Callback = ( TlsDeleteCallbackFunction_t * )( &( tcb->pvDummy15[ ( config_عدد_THREAD_LOCAL_STORAGE_اشارے / 2 ) ] ) );

    /* We need to iterate over half the depth of the pvThreadLocalStoragePointers area
     * to access all TLS pointers and their respective TLS deletion callbacks.
     */
    برائے ( صحیح x = 0; x < ( config_عدد_THREAD_LOCAL_STORAGE_اشارے / 2 ); x++ ) {
        اگر ( pv_Thread_Local_Storage_اشارے_Del_Callback[ x ] != تہی ) {  //If del cb is set
            /* In case the TLSP deletion callback has been overwritten by a TLS pointer, gracefully abort. */
            اگر ( !esp_ptr_executable( pv_Thread_Local_Storage_اشارے_Del_Callback[ x ] ) ) {
                // We call EARLY log here as currently portCLEAN_UP_TCB() is called in a critical section
                ESP_EARLY_LOGE("FreeRTOS", "Fatal error: TLSP deletion callback at index %d overwritten with non-excutable pointer %p", x, pv_Thread_Local_Storage_اشارے_Del_Callback[ x ]);
                abort();
            }

            pv_Thread_Local_Storage_اشارے_Del_Callback[ x ]( x, tcb->pvDummy15[ x ] );   //Call del cb
        }
    }
}
#اگر_ختم /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

#اگر ( XCHAL_CP_عدد > 0 )
ساکن خالی v_پورٹ_Clean_اوپر_Coproc_Area(خالی *pvTCB)
{
    UBaseType_t uxCoprocArea;
    بنیاد_Type_t x_ہدف_Core_ID;

    /* Get a pointer to the task's coprocessor save area */
    uxCoprocArea = ( UBaseType_t ) ( ( ( StaticTask_t * ) pvTCB )->pxDummy8 );  /* Get TCB_t.pxEndOfStack */
    uxCoprocArea = ESP_ALIGN_نیچے_کو(uxCoprocArea - XT_CP_سائز, 16);

    #اگر ( config_نمبر_کا_CORES > 1 )
        /* Get xTargetCoreID from the TCB.xCoreID */
        x_ہدف_Core_ID = ( ( StaticTask_t * ) pvTCB )->xDummyCoreID;
    #ورنہ /* configNUMBER_OF_CORES > 1 */
        x_ہدف_Core_ID = 0;
    #اگر_ختم /* configNUMBER_OF_CORES > 1 */

    /* If task has live floating point registers somewhere, release them */
    خالی _xt_coproc_release(متلون خالی *coproc_sa_بنیاد, بنیاد_Type_t x_ہدف_Core_ID);
    _xt_coproc_release( (خالی *)uxCoprocArea, x_ہدف_Core_ID );
}
#اگر_ختم /* XCHAL_CP_NUM > 0 */

خالی v_پورٹ_TCBPre_Delete_Hook( خالی *pxTCB )
{
    #اگر ( CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK )
        /* Call the user defined task pre-deletion hook */
        خارجی خالی vTaskPreDeletionHook( خالی * pxTCB );
        vTaskPreDeletionHook( pxTCB );
    #اگر_ختم /* CONFIG_FREERTOS_TASK_PRE_DELETION_HOOK */

    #اگر ( CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS )
        /* Call TLS pointers deletion callbacks */
        v_پورٹ_TLSPointers_Del_Cb( pxTCB );
    #اگر_ختم /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

    #اگر ( XCHAL_CP_عدد > 0 )
        /* Cleanup coproc save area */
        v_پورٹ_Clean_اوپر_Coproc_Area( pxTCB );
    #اگر_ختم /* XCHAL_CP_NUM > 0 */
}

/* ------------------------------------------------ Run Time Stats ------------------------------------------------- */

#اگر ( CONFIG_FREERTOS_GENERATE_تسلسل_وقت_STATS )

config_تسلسل_وقت_شمار_TYPE x_پورٹ_Get_تسلسل_وقت_شمار_قدر( خالی )
{
#اگر_تعریف_شدہ CONFIG_FREERTOS_تسلسل_وقت_STATS_USING_ESP_TIMER
    واپس (config_تسلسل_وقت_شمار_TYPE) esp_timer_get_وقت();
#ورنہ // Uses CCOUNT
    واپس (config_تسلسل_وقت_شمار_TYPE) xthal_get_ccount();
#اگر_ختم // CONFIG_FREERTOS_RUN_TIME_STATS_USING_ESP_TIMER
}

#اگر_ختم /* CONFIG_FREERTOS_GENERATE_RUN_TIME_STATS */
