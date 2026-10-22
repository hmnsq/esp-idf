/*
 * FreeRTOS Kernel V11.1.0
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * SPDX-FileCopyrightText: 2021 Amazon.com, Inc. or its affiliates
 *
 * SPDX-License-Identifier: MIT
 *
 * SPDX-FileContributor: 2023-2024 Espressif Systems (Shanghai) CO LTD
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

/*-----------------------------------------------------------
* Portable layer API.  Each function must be defined for each port.
*----------------------------------------------------------*/

#اگر_غیر_تعریف_شدہ PORTABLE_H
#تعریف_کرو PORTABLE_H

/* Each FreeRTOS port has a unique portmacro.h header file.  Originally a
 * pre-processor definition was used to ensure the pre-processor found the correct
 * portmacro.h file for the port being used.  That scheme was deprecated in favour
 * of setting the compiler's include path such that it found the correct
 * portmacro.h file - removing the need for the constant and allowing the
 * portmacro.h file to be located anywhere in relation to the port being used.
 * Purely for reasons of backward compatibility the old method is still valid, but
 * to make it clear that new projects should not use it, support for the port
 * specific constants has been moved into the deprecated_definitions.h header
 * file. */
#شامل "deprecated_definitions.h"

/* If portENTER_CRITICAL is not defined then including deprecated_definitions.h
 * did not result in a portmacro.h header file being included - and it should be
 * included here.  In this case the path to the correct portmacro.h header file
 * must be set in the compiler's include path. */
#اگر_غیر_تعریف_شدہ پورٹ_ENTER_CRITICAL
    #شامل "portmacro.h"
#اگر_ختم

#اگر پورٹ_BYTE_ALIGNMENT == 32
    #تعریف_کرو پورٹ_BYTE_ALIGNMENT_نقاب    ( 0x001f )
#وگرنہ_اگر پورٹ_BYTE_ALIGNMENT == 16
    #تعریف_کرو پورٹ_BYTE_ALIGNMENT_نقاب    ( 0x000f )
#وگرنہ_اگر پورٹ_BYTE_ALIGNMENT == 8
    #تعریف_کرو پورٹ_BYTE_ALIGNMENT_نقاب    ( 0x0007 )
#وگرنہ_اگر پورٹ_BYTE_ALIGNMENT == 4
    #تعریف_کرو پورٹ_BYTE_ALIGNMENT_نقاب    ( 0x0003 )
#وگرنہ_اگر پورٹ_BYTE_ALIGNMENT == 2
    #تعریف_کرو پورٹ_BYTE_ALIGNMENT_نقاب    ( 0x0001 )
#وگرنہ_اگر پورٹ_BYTE_ALIGNMENT == 1
    #تعریف_کرو پورٹ_BYTE_ALIGNMENT_نقاب    ( 0x0000 )
#ورنہ /* if portBYTE_ALIGNMENT == 32 */
    #غلطی "Invalid portBYTE_ALIGNMENT definition"
#اگر_ختم /* if portBYTE_ALIGNMENT == 32 */

#اگر_غیر_تعریف_شدہ پورٹ_USING_MPU_WRAPPERS
    #تعریف_کرو پورٹ_USING_MPU_WRAPPERS    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ پورٹ_عدد_CONFIGURABLE_REGIONS
    #تعریف_کرو پورٹ_عدد_CONFIGURABLE_REGIONS    1
#اگر_ختم

#اگر_غیر_تعریف_شدہ پورٹ_HAS_ڈھیر_حد_سے_تجاوز_CHECKING
    #تعریف_کرو پورٹ_HAS_ڈھیر_حد_سے_تجاوز_CHECKING    0
#اگر_ختم

#اگر_غیر_تعریف_شدہ پورٹ_ARCH_NAME
    #تعریف_کرو پورٹ_ARCH_NAME    تہی
#اگر_ختم

#اگر_غیر_تعریف_شدہ config_ڈھیر_گہرائی_TYPE
    #تعریف_کرو config_ڈھیر_گہرائی_TYPE    ڈھیر_Type_t
#اگر_ختم

#اگر_غیر_تعریف_شدہ config_ڈھیر_تخصیص_FROM_SEPARATE_ڈھیر
    /* Defaults to 0 for backward compatibility. */
    #تعریف_کرو config_ڈھیر_تخصیص_FROM_SEPARATE_ڈھیر    0
#اگر_ختم

/* *INDENT-OFF* */
#اگر_تعریف_شدہ __cplusplus
    خارجی "C" {
#اگر_ختم
/* *INDENT-ON* */

#شامل "mpu_wrappers.h"

/*
 * Setup the stack of a new task so it is ready to be placed under the
 * scheduler control.  The registers have to be placed on the stack in
 * the order that the port expects to find them.
 *
 */
#اگر ( پورٹ_USING_MPU_WRAPPERS == 1 )
    #اگر ( پورٹ_HAS_ڈھیر_حد_سے_تجاوز_CHECKING == 1 )
        ڈھیر_Type_t * px_پورٹ_Initialise_ڈھیر( ڈھیر_Type_t * px_چوٹی_کا_ڈھیر,
                                             ڈھیر_Type_t * px_انتہا_کا_ڈھیر,
                                             TaskFunction_t px_رمز,
                                             خالی * pvParameters,
                                             بنیاد_Type_t x_تسلسل_Privileged,
                                             xMPU_SETTINGS * xMPUSettings ) PRIVILEGED_FUNCTION;
    #ورنہ
        ڈھیر_Type_t * px_پورٹ_Initialise_ڈھیر( ڈھیر_Type_t * px_چوٹی_کا_ڈھیر,
                                             TaskFunction_t px_رمز,
                                             خالی * pvParameters,
                                             بنیاد_Type_t x_تسلسل_Privileged,
                                             xMPU_SETTINGS * xMPUSettings ) PRIVILEGED_FUNCTION;
    #اگر_ختم /* if ( portHAS_STACK_OVERFLOW_CHECKING == 1 ) */
#ورنہ /* if ( portUSING_MPU_WRAPPERS == 1 ) */
    #اگر ( پورٹ_HAS_ڈھیر_حد_سے_تجاوز_CHECKING == 1 )
        ڈھیر_Type_t * px_پورٹ_Initialise_ڈھیر( ڈھیر_Type_t * px_چوٹی_کا_ڈھیر,
                                             ڈھیر_Type_t * px_انتہا_کا_ڈھیر,
                                             TaskFunction_t px_رمز,
                                             خالی * pvParameters ) PRIVILEGED_FUNCTION;
    #ورنہ
        ڈھیر_Type_t * px_پورٹ_Initialise_ڈھیر( ڈھیر_Type_t * px_چوٹی_کا_ڈھیر,
                                             TaskFunction_t px_رمز,
                                             خالی * pvParameters ) PRIVILEGED_FUNCTION;
    #اگر_ختم
#اگر_ختم /* if ( portUSING_MPU_WRAPPERS == 1 ) */

/* Used by heap_5.c to define the start address and size of each memory region
 * that together comprise the total FreeRTOS heap space. */
قسمتعریف ساخت ڈھیر_Region
{
    uint8_t * puc_آغاز_پتہ;
    size_t x_سائز_اندر_بائٹیں;
} ڈھیر_Region_t;

/* Used to pass information about the heap out of vPortGetHeapStats(). */
قسمتعریف ساخت x_ڈھیر_Stats
{
    size_t x_دستیاب_ڈھیر_Space_اندر_بائٹیں;      /* The total heap size currently available - this is the sum of all the free blocks, not the largest block that can be allocated. */
    size_t x_سائز_کا_سب_سے_بڑا_Free_بلاک_اندر_بائٹیں;  /* The maximum size, in bytes, of all the free blocks within the heap at the time vPortGetHeapStats() is called. */
    size_t x_سائز_کا_سب_سے_چھوٹا_Free_بلاک_اندر_بائٹیں; /* The minimum size, in bytes, of all the free blocks within the heap at the time vPortGetHeapStats() is called. */
    size_t x_نمبر_کا_Free_بلاکیں;             /* The number of free memory blocks within the heap at the time vPortGetHeapStats() is called. */
    size_t x_Minimum_Ever_Free_بائٹیں_بقایا;  /* The minimum amount of total free memory (sum of all free blocks) there has been in the heap since the system booted. */
    size_t x_نمبر_کا_Successful_Allocations;  /* The number of calls to pvPortMalloc() that have returned a valid memory block. */
    size_t x_نمبر_کا_Successful_Frees;        /* The number of calls to vPortFree() that has successfully freed a block of memory. */
} ڈھیر_Stats_t;

/*
 * Used to define multiple heap regions for use by heap_5.c.  This function
 * must be called before any calls to pvPortMalloc() - not creating a task,
 * queue, semaphore, mutex, software timer, event group, etc. will result in
 * pvPortMalloc being called.
 *
 * pxHeapRegions passes in an array of HeapRegion_t structures - each of which
 * defines a region of memory that can be used as the heap.  The array is
 * terminated by a HeapRegions_t structure that has a size of 0.  The region
 * with the lowest start address must appear first in the array.
 */
خالی v_پورٹ_Define_ڈھیر_Regions( ثابت ڈھیر_Region_t * ثابت px_ڈھیر_Regions ) PRIVILEGED_FUNCTION;

/*
 * Returns a HeapStats_t structure filled with information about the current
 * heap state.
 */
خالی v_پورٹ_Get_ڈھیر_Stats( ڈھیر_Stats_t * px_ڈھیر_Stats );

/*
 * Map to the memory management routines required for the port.
 */
خالی * pv_پورٹ_Malloc( size_t x_Wanted_سائز ) PRIVILEGED_FUNCTION;
خالی * pv_پورٹ_Calloc( size_t x_عدد,
                     size_t x_سائز ) PRIVILEGED_FUNCTION;
خالی v_پورٹ_Free( خالی * pv ) PRIVILEGED_FUNCTION;
خالی v_پورٹ_Initialise_بلاکیں( خالی ) PRIVILEGED_FUNCTION;
size_t x_پورٹ_Get_Free_ڈھیر_سائز( خالی ) PRIVILEGED_FUNCTION;
size_t x_پورٹ_Get_Minimum_Ever_Free_ڈھیر_سائز( خالی ) PRIVILEGED_FUNCTION;

#اگر ( config_ڈھیر_تخصیص_FROM_SEPARATE_ڈھیر == 1 )
    خالی * pv_پورٹ_Malloc_ڈھیر( size_t x_سائز ) PRIVILEGED_FUNCTION;
    خالی v_پورٹ_Free_ڈھیر( خالی * pv ) PRIVILEGED_FUNCTION;
#ورنہ
    #تعریف_کرو pv_پورٹ_Malloc_ڈھیر    pv_پورٹ_Malloc
    #تعریف_کرو v_پورٹ_Free_ڈھیر       v_پورٹ_Free
#اگر_ختم

/*
 * This function resets the internal state of the heap module. It must be called
 * by the application before restarting the scheduler.
 */
خالی v_پورٹ_ڈھیر_Reset_State( خالی ) PRIVILEGED_FUNCTION;

#اگر ( configUSE_MALLOC_FAILED_HOOK == 1 )

/**
 * task.h
 * @code{c}
 * void vApplicationMallocFailedHook( void )
 * @endcode
 *
 * This hook function is called when allocation failed.
 */
    خالی vApplicationMallocFailedHook( خالی );
#اگر_ختم

/*
 * Setup the hardware ready for the scheduler to take control.  This generally
 * sets up a tick interrupt and sets timers for the correct tick frequency.
 */
بنیاد_Type_t x_پورٹ_آغاز_Scheduler( خالی ) PRIVILEGED_FUNCTION;

/*
 * Undo any hardware/ISR setup that was performed by xPortStartScheduler() so
 * the hardware is left in its original condition after the scheduler stops
 * executing.
 */
خالی v_پورٹ_انتہا_Scheduler( خالی ) PRIVILEGED_FUNCTION;

/*
 * The structures and methods of manipulating the MPU are contained within the
 * port layer.
 *
 * Fills the xMPUSettings structure with the memory region information
 * contained in xRegions.
 */
#اگر ( پورٹ_USING_MPU_WRAPPERS == 1 )
    ساخت x_حافظہ_REGION;
    خالی v_پورٹ_Store_Task_MPUSettings( xMPU_SETTINGS * xMPUSettings,
                                    ثابت ساخت x_حافظہ_REGION * ثابت xRegions,
                                    ڈھیر_Type_t * px_Bottom_کا_ڈھیر,
                                    config_ڈھیر_گہرائی_TYPE ux_ڈھیر_گہرائی ) PRIVILEGED_FUNCTION;
#اگر_ختم

/**
 * @brief Checks if the calling task is authorized to access the given buffer.
 *
 * @param pvBuffer The buffer which the calling task wants to access.
 * @param ulBufferLength The length of the pvBuffer.
 * @param ulAccessRequested The permissions that the calling task wants.
 *
 * @return pdTRUE if the calling task is authorized to access the buffer,
 *         pdFALSE otherwise.
 */
#اگر ( پورٹ_USING_MPU_WRAPPERS == 1 )
    بنیاد_Type_t x_پورٹ_Is_Authorized_مقصد_Access_بفر( ثابت خالی * pv_بفر,
                                                uint32_t ul_بفر_لمبائی,
                                                uint32_t ulAccessRequested ) PRIVILEGED_FUNCTION;
#اگر_ختم

/**
 * @brief Checks if the calling task is authorized to access the given kernel object.
 *
 * @param lInternalIndexOfKernelObject The index of the kernel object in the kernel
 *                                     object handle pool.
 *
 * @return pdTRUE if the calling task is authorized to access the kernel object,
 *         pdFALSE otherwise.
 */
#اگر ( ( پورٹ_USING_MPU_WRAPPERS == 1 ) && ( configUSE_MPU_WRAPPERS_V1 == 0 ) )

    بنیاد_Type_t x_پورٹ_Is_Authorized_مقصد_Access_Kernel_Object( int32_t l_اندرونی_اشاریہ_کا_Kernel_Object ) PRIVILEGED_FUNCTION;

#اگر_ختم

/* *INDENT-OFF* */
#اگر_تعریف_شدہ __cplusplus
    }
#اگر_ختم
/* *INDENT-ON* */

#اگر_ختم /* PORTABLE_H */
