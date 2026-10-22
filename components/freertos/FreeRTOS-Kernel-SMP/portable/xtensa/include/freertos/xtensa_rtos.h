/*
 * SPDX-FileCopyrightText: 2015-2019 Cadence Design Systems, Inc.
 *
 * SPDX-License-Identifier: MIT
 *
 * SPDX-FileContributor: 2016-2022 Espressif Systems (Shanghai) CO LTD
 */
/*
 * Copyright (c) 2015-2019 Cadence Design Systems, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * RTOS-SPECIFIC INFORMATION FOR XTENSA RTOS ASSEMBLER SOURCES
 *                       (FreeRTOS Port)
 *
 * This header is the primary glue between generic Xtensa RTOS support
 * sources and a specific RTOS port for Xtensa.  It contains definitions
 * and macros for use primarily by Xtensa assembly coded source files.
 *
 * Macros in this header map callouts from generic Xtensa files to specific
 * RTOS functions. It may also be included in C source files.
 *
 * Xtensa RTOS ports support all RTOS-compatible configurations of the Xtensa
 * architecture, using the Xtensa hardware abstraction layer (HAL) to deal
 * with configuration specifics.
 *
 * Should be included by all Xtensa generic and RTOS port-specific sources.
 */

#अगर_अघोषित XTENSA_RTOS_H
#घोषणा XTENSA_RTOS_H

#अगर_घोषित __ASSEMBLER__
#समावेश    <xtensa/coreasm.h>
#अन्यथा
#समावेश    <xtensa/config/core.h>
#पूर्ण_अगर

#समावेश    <xtensa/corebits.h>
#समावेश    <xtensa/config/system.h>
#समावेश    "sdkconfig.h"

/*
Include any RTOS specific definitions that are needed by this header.
*/
#समावेश    "freertos/FreeRTOSConfig.h"

/*
Convert FreeRTOSConfig definitions to XTENSA definitions.
However these can still be overridden from the command line.
*/

#अगर_अघोषित XT_SIMULATOR
  #अगर configXT_SIMULATOR
    #घोषणा XT_SIMULATOR             1  /* Simulator mode */
  #पूर्ण_अगर
#पूर्ण_अगर

#अगर_अघोषित XT_BOARD
  #अगर configXT_BOARD
    #घोषणा XT_BOARD                 1  /* Board mode */
  #पूर्ण_अगर
#पूर्ण_अगर

#अगर_अघोषित XT_TIMER_सूचकांक
  #अगर defined config_XT_TIMER_सूचकांक
    #घोषणा XT_TIMER_सूचकांक           config_XT_TIMER_सूचकांक  /* Index of hardware timer to be used */
  #पूर्ण_अगर
#पूर्ण_अगर

#अगर_अघोषित XT_INTEXC_HOOKS
  #अगर configXT_INTEXC_HOOKS
    #घोषणा XT_INTEXC_HOOKS          1  /* Enables exception hooks */
  #पूर्ण_अगर
#पूर्ण_अगर

#अगर (!XT_SIMULATOR) && (!XT_BOARD)
  #त्रुटि Either XT_SIMULATOR or XT_BOARD must be defined.
#पूर्ण_अगर


/*
Name of RTOS (for messages).
*/
#घोषणा XT_RTOS_NAME    FreeRTOS

/*
Check some Xtensa configuration requirements and report error if not met.
Error messages can be customize to the RTOS port.
*/

#अगर !XCHAL_HAVE_XEA2
#त्रुटि "FreeRTOS/Xtensa requires XEA2 (exception architecture 2)."
#पूर्ण_अगर


/*******************************************************************************

RTOS CALLOUT MACROS MAPPED TO RTOS PORT-SPECIFIC FUNCTIONS.

Define callout macros used in generic Xtensa code to interact with the RTOS.
The macros are simply the function names for use in calls from assembler code.
Some of these functions may call back to generic functions in xtensa_context.h .

*******************************************************************************/

/*
Inform RTOS of entry into an interrupt handler that will affect it.
Allows RTOS to manage switch to any system stack and count nesting level.
Called after minimal context has been saved, with interrupts disabled.
RTOS port can call0 _xt_context_save to save the rest of the context.
May only be called from assembly code by the 'call0' instruction.
*/
// void XT_RTOS_INT_ENTER(void)
#घोषणा XT_RTOS_INT_ENTER   _frxt_int_enter

/*
Inform RTOS of completion of an interrupt handler, and give control to
RTOS to perform thread/task scheduling, switch back from any system stack
and restore the context, and return to the exit dispatcher saved in the
stack frame at XT_STK_EXIT. RTOS port can call0 _xt_context_restore
to save the context saved in XT_RTOS_INT_ENTER via _xt_context_save,
leaving only a minimal part of the context to be restored by the exit
dispatcher. This function does not return to the place it was called from.
May only be called from assembly code by the 'call0' instruction.
*/
// void XT_RTOS_INT_EXIT(void)
#घोषणा XT_RTOS_INT_EXIT    _frxt_int_exit

/*
Inform RTOS of the occurrence of a tick timer interrupt.
If RTOS has no tick timer, leave XT_RTOS_TIMER_INT undefined.
May be coded in or called from C or assembly, per ABI conventions.
RTOS may optionally define XT_TICK_PER_SEC in its own way (eg. macro).
*/
// void XT_RTOS_TIMER_INT(void)
#अगर_घोषित CONFIG_FREERTOS_SYSTICK_USES_CCOUNT
#घोषणा XT_RTOS_TIMER_INT   _frxt_timer_int
#पूर्ण_अगर
#घोषणा XT_TICK_PER_SEC     configTICK_RATE_HZ

/*
Return in a15 the base address of the co-processor state save area for the
thread that triggered a co-processor exception, or 0 if no thread was running.
The state save area is structured as defined in xtensa_context.h and has size
XT_CP_SIZE. Co-processor instructions should only be used in thread code, never
in interrupt handlers or the RTOS kernel. May only be called from assembly code
and by the 'call0' instruction. A result of 0 indicates an unrecoverable error.
The implementation may use only a2-4, a15 (all other regs must be preserved).
*/
// void* XT_RTOS_CP_STATE(void)
#घोषणा XT_RTOS_CP_STATE    _frxt_task_coproc_state

/*
RTOS provided hook function that is called on every coprocessor exception. May
only be called from assembly code and by the 'call0' instruction.
The implementation may use only a2-4, a15 (all other regs must be preserved).
*/
// void XT_RTOS_CP_EXC_HOOK(void)
#अगर XCHAL_CP_अंक > 0
#घोषणा XT_RTOS_CP_EXC_HOOK   _frxt_coproc_exc_hook
#पूर्ण_अगर

/*******************************************************************************

HOOKS TO DYNAMICALLY INSTALL INTERRUPT AND EXCEPTION HANDLERS PER LEVEL.

This Xtensa RTOS port provides hooks for dynamically installing exception
and interrupt handlers to facilitate automated testing where each test
case can install its own handler for user exceptions and each interrupt
priority (level). This consists of an array of function pointers indexed
by interrupt priority, with index 0 being the user exception handler hook.
Each entry in the array is initially 0, and may be replaced by a function
pointer of type XT_INTEXC_HOOK. A handler may be uninstalled by installing 0.

The handler for low and medium priority obeys ABI conventions so may be coded
in C. For the exception handler, the cause is the contents of the EXCCAUSE
reg, and the result is -1 if handled, else the cause (still needs handling).
For interrupt handlers, the cause is a mask of pending enabled interrupts at
that level, and the result is the same mask with the bits for the handled
interrupts cleared (those not cleared still need handling). This allows a test
case to either pre-handle or override the default handling for the exception
or interrupt level (see xtensa_vectors.S).

High priority handlers (including NMI) must be coded in assembly, are always
called by 'call0' regardless of ABI, must preserve all registers except a0,
and must not use or modify the interrupted stack. The hook argument 'cause'
is not passed and the result is ignored, so as not to burden the caller with
saving and restoring a2 (it assumes only one interrupt per level - see the
discussion in high priority interrupts in xtensa_vectors.S). The handler
therefore should be coded to prototype 'void h(void)' even though it plugs
into an array of handlers of prototype 'unsigned h(unsigned)'.

To enable interrupt/exception hooks, compile the RTOS with '-DXT_INTEXC_HOOKS'.

*******************************************************************************/

#घोषणा XT_INTEXC_HOOK_अंक  (1 + XCHAL_अंक_INTLEVELS + XCHAL_HAVE_NMI)

#अगर_अघोषित __ASSEMBLER__
प्रकार अचिन्हित (*XT_INTEXC_HOOK)(अचिन्हित cause);
बाह्य  अस्थिर XT_INTEXC_HOOK _xt_intexc_hooks[XT_INTEXC_HOOK_अंक];
#पूर्ण_अगर


/*******************************************************************************

CONVENIENCE INCLUSIONS.

Ensures RTOS specific files need only include this one Xtensa-generic header.
These headers are included last so they can use the RTOS definitions above.

*******************************************************************************/

#समावेश    "xtensa_context.h"

#अगर_घोषित XT_RTOS_TIMER_INT
#समावेश    "xtensa_timer.h"
#पूर्ण_अगर


/*******************************************************************************

Xtensa Port Version.

*******************************************************************************/

#घोषणा XTENSA_PORT_VERSION             1.7
#घोषणा XTENSA_PORT_VERSION_STRING      "1.7"

#पूर्ण_अगर /* XTENSA_RTOS_H */
