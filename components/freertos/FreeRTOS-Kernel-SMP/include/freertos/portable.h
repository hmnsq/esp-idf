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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ PORTABLE_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PORTABLE_H

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
#ਅਨ੍ਤਰ੍ਭਾਵ "deprecated_definitions.h"

/* If portENTER_CRITICAL is not defined then including deprecated_definitions.h
 * did not result in a portmacro.h header file being included - and it should be
 * included here.  In this case the path to the correct portmacro.h header file
 * must be set in the compiler's include path. */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_ENTER_CRITICAL
    #ਅਨ੍ਤਰ੍ਭਾਵ "portmacro.h"
#ਜੇ_ਅੰਤ

#ਜੇ ਪੋਰਟ_BYTE_ALIGNMENT == 32
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_BYTE_ALIGNMENT_ਨਕਾਬ    ( 0x001f )
#ਜਾਂਜੇ ਪੋਰਟ_BYTE_ALIGNMENT == 16
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_BYTE_ALIGNMENT_ਨਕਾਬ    ( 0x000f )
#ਜਾਂਜੇ ਪੋਰਟ_BYTE_ALIGNMENT == 8
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_BYTE_ALIGNMENT_ਨਕਾਬ    ( 0x0007 )
#ਜਾਂਜੇ ਪੋਰਟ_BYTE_ALIGNMENT == 4
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_BYTE_ALIGNMENT_ਨਕਾਬ    ( 0x0003 )
#ਜਾਂਜੇ ਪੋਰਟ_BYTE_ALIGNMENT == 2
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_BYTE_ALIGNMENT_ਨਕਾਬ    ( 0x0001 )
#ਜਾਂਜੇ ਪੋਰਟ_BYTE_ALIGNMENT == 1
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_BYTE_ALIGNMENT_ਨਕਾਬ    ( 0x0000 )
#ਨਹੀਂਤਾਂ /* if portBYTE_ALIGNMENT == 32 */
    #ਗਲਤੀ "Invalid portBYTE_ALIGNMENT definition"
#ਜੇ_ਅੰਤ /* if portBYTE_ALIGNMENT == 32 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_USING_MPU_WRAPPERS
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_USING_MPU_WRAPPERS    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_ਅੰਕ_CONFIGURABLE_REGIONS
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ਅੰਕ_CONFIGURABLE_REGIONS    1
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_HAS_ਢੇਰ_ਉਛਾਲ_CHECKING
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_HAS_ਢੇਰ_ਉਛਾਲ_CHECKING    0
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_ARCH_NAME
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_ARCH_NAME    ਨਿਰਾ
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_ਢੇਰ_ਡੂੰਘਾਈ_TYPE
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_ਢੇਰ_ਡੂੰਘਾਈ_TYPE    ਢੇਰ_Type_t
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ config_ਢੇਰ_ਵੰਡ_FROM_SEPARATE_ਢੇਰ
    /* Defaults to 0 for backward compatibility. */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ config_ਢੇਰ_ਵੰਡ_FROM_SEPARATE_ਢੇਰ    0
#ਜੇ_ਅੰਤ

/* *INDENT-OFF* */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
    ਬਾਹਰੀ "C" {
#ਜੇ_ਅੰਤ
/* *INDENT-ON* */

#ਅਨ੍ਤਰ੍ਭਾਵ "mpu_wrappers.h"

/*
 * Setup the stack of a new task so it is ready to be placed under the
 * scheduler control.  The registers have to be placed on the stack in
 * the order that the port expects to find them.
 *
 */
#ਜੇ ( ਪੋਰਟ_USING_MPU_WRAPPERS == 1 )
    #ਜੇ ( ਪੋਰਟ_HAS_ਢੇਰ_ਉਛਾਲ_CHECKING == 1 )
        ਢੇਰ_Type_t * px_ਪੋਰਟ_Initialise_ਢੇਰ( ਢੇਰ_Type_t * px_ਸਿਖਰ_ਦਾ_ਢੇਰ,
                                             ਢੇਰ_Type_t * px_ਸਿਰਾ_ਦਾ_ਢੇਰ,
                                             TaskFunction_t px_ਕੋਡ,
                                             ਖਾਲੀ * pvParameters,
                                             ਅਧਾਰ_Type_t x_ਲੜੀਵਾਰ_Privileged,
                                             xMPU_SETTINGS * xMPUSettings ) PRIVILEGED_FUNCTION;
    #ਨਹੀਂਤਾਂ
        ਢੇਰ_Type_t * px_ਪੋਰਟ_Initialise_ਢੇਰ( ਢੇਰ_Type_t * px_ਸਿਖਰ_ਦਾ_ਢੇਰ,
                                             TaskFunction_t px_ਕੋਡ,
                                             ਖਾਲੀ * pvParameters,
                                             ਅਧਾਰ_Type_t x_ਲੜੀਵਾਰ_Privileged,
                                             xMPU_SETTINGS * xMPUSettings ) PRIVILEGED_FUNCTION;
    #ਜੇ_ਅੰਤ /* if ( portHAS_STACK_OVERFLOW_CHECKING == 1 ) */
#ਨਹੀਂਤਾਂ /* if ( portUSING_MPU_WRAPPERS == 1 ) */
    #ਜੇ ( ਪੋਰਟ_HAS_ਢੇਰ_ਉਛਾਲ_CHECKING == 1 )
        ਢੇਰ_Type_t * px_ਪੋਰਟ_Initialise_ਢੇਰ( ਢੇਰ_Type_t * px_ਸਿਖਰ_ਦਾ_ਢੇਰ,
                                             ਢੇਰ_Type_t * px_ਸਿਰਾ_ਦਾ_ਢੇਰ,
                                             TaskFunction_t px_ਕੋਡ,
                                             ਖਾਲੀ * pvParameters ) PRIVILEGED_FUNCTION;
    #ਨਹੀਂਤਾਂ
        ਢੇਰ_Type_t * px_ਪੋਰਟ_Initialise_ਢੇਰ( ਢੇਰ_Type_t * px_ਸਿਖਰ_ਦਾ_ਢੇਰ,
                                             TaskFunction_t px_ਕੋਡ,
                                             ਖਾਲੀ * pvParameters ) PRIVILEGED_FUNCTION;
    #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ /* if ( portUSING_MPU_WRAPPERS == 1 ) */

/* Used by heap_5.c to define the start address and size of each memory region
 * that together comprise the total FreeRTOS heap space. */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ਢੇਰ_Region
{
    uint8_t * puc_ਸ਼ੁਰੂ_ਪਤਾ;
    size_t x_ਮਾਪ_ਵਿੱਚ_ਬਾਈਟਾਂ;
} ਢੇਰ_Region_t;

/* Used to pass information about the heap out of vPortGetHeapStats(). */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ x_ਢੇਰ_Stats
{
    size_t x_ਉਪਲਬਧ_ਢੇਰ_Space_ਵਿੱਚ_ਬਾਈਟਾਂ;      /* The total heap size currently available - this is the sum of all the free blocks, not the largest block that can be allocated. */
    size_t x_ਮਾਪ_ਦਾ_ਸਭ_ਤੋਂ_ਵੱਡਾ_Free_ਖੰਡ_ਵਿੱਚ_ਬਾਈਟਾਂ;  /* The maximum size, in bytes, of all the free blocks within the heap at the time vPortGetHeapStats() is called. */
    size_t x_ਮਾਪ_ਦਾ_ਸਭ_ਤੋਂ_ਛੋਟਾ_Free_ਖੰਡ_ਵਿੱਚ_ਬਾਈਟਾਂ; /* The minimum size, in bytes, of all the free blocks within the heap at the time vPortGetHeapStats() is called. */
    size_t x_ਸੰਖਿਆ_ਦਾ_Free_ਖੰਡਾਂ;             /* The number of free memory blocks within the heap at the time vPortGetHeapStats() is called. */
    size_t x_Minimum_Ever_Free_ਬਾਈਟਾਂ_ਬਚਿਆ;  /* The minimum amount of total free memory (sum of all free blocks) there has been in the heap since the system booted. */
    size_t x_ਸੰਖਿਆ_ਦਾ_Successful_Allocations;  /* The number of calls to pvPortMalloc() that have returned a valid memory block. */
    size_t x_ਸੰਖਿਆ_ਦਾ_Successful_Frees;        /* The number of calls to vPortFree() that has successfully freed a block of memory. */
} ਢੇਰ_Stats_t;

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
ਖਾਲੀ v_ਪੋਰਟ_Define_ਢੇਰ_Regions( ਅਚਲ ਢੇਰ_Region_t * ਅਚਲ px_ਢੇਰ_Regions ) PRIVILEGED_FUNCTION;

/*
 * Returns a HeapStats_t structure filled with information about the current
 * heap state.
 */
ਖਾਲੀ v_ਪੋਰਟ_Get_ਢੇਰ_Stats( ਢੇਰ_Stats_t * px_ਢੇਰ_Stats );

/*
 * Map to the memory management routines required for the port.
 */
ਖਾਲੀ * pv_ਪੋਰਟ_Malloc( size_t x_Wanted_ਮਾਪ ) PRIVILEGED_FUNCTION;
ਖਾਲੀ * pv_ਪੋਰਟ_Calloc( size_t x_ਅੰਕ,
                     size_t x_ਮਾਪ ) PRIVILEGED_FUNCTION;
ਖਾਲੀ v_ਪੋਰਟ_Free( ਖਾਲੀ * pv ) PRIVILEGED_FUNCTION;
ਖਾਲੀ v_ਪੋਰਟ_Initialise_ਖੰਡਾਂ( ਖਾਲੀ ) PRIVILEGED_FUNCTION;
size_t x_ਪੋਰਟ_Get_Free_ਢੇਰ_ਮਾਪ( ਖਾਲੀ ) PRIVILEGED_FUNCTION;
size_t x_ਪੋਰਟ_Get_Minimum_Ever_Free_ਢੇਰ_ਮਾਪ( ਖਾਲੀ ) PRIVILEGED_FUNCTION;

#ਜੇ ( config_ਢੇਰ_ਵੰਡ_FROM_SEPARATE_ਢੇਰ == 1 )
    ਖਾਲੀ * pv_ਪੋਰਟ_Malloc_ਢੇਰ( size_t x_ਮਾਪ ) PRIVILEGED_FUNCTION;
    ਖਾਲੀ v_ਪੋਰਟ_Free_ਢੇਰ( ਖਾਲੀ * pv ) PRIVILEGED_FUNCTION;
#ਨਹੀਂਤਾਂ
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ pv_ਪੋਰਟ_Malloc_ਢੇਰ    pv_ਪੋਰਟ_Malloc
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ v_ਪੋਰਟ_Free_ਢੇਰ       v_ਪੋਰਟ_Free
#ਜੇ_ਅੰਤ

/*
 * This function resets the internal state of the heap module. It must be called
 * by the application before restarting the scheduler.
 */
ਖਾਲੀ v_ਪੋਰਟ_ਢੇਰ_Reset_State( ਖਾਲੀ ) PRIVILEGED_FUNCTION;

#ਜੇ ( configUSE_MALLOC_FAILED_HOOK == 1 )

/**
 * task.h
 * @code{c}
 * void vApplicationMallocFailedHook( void )
 * @endcode
 *
 * This hook function is called when allocation failed.
 */
    ਖਾਲੀ vApplicationMallocFailedHook( ਖਾਲੀ );
#ਜੇ_ਅੰਤ

/*
 * Setup the hardware ready for the scheduler to take control.  This generally
 * sets up a tick interrupt and sets timers for the correct tick frequency.
 */
ਅਧਾਰ_Type_t x_ਪੋਰਟ_ਸ਼ੁਰੂ_Scheduler( ਖਾਲੀ ) PRIVILEGED_FUNCTION;

/*
 * Undo any hardware/ISR setup that was performed by xPortStartScheduler() so
 * the hardware is left in its original condition after the scheduler stops
 * executing.
 */
ਖਾਲੀ v_ਪੋਰਟ_ਸਿਰਾ_Scheduler( ਖਾਲੀ ) PRIVILEGED_FUNCTION;

/*
 * The structures and methods of manipulating the MPU are contained within the
 * port layer.
 *
 * Fills the xMPUSettings structure with the memory region information
 * contained in xRegions.
 */
#ਜੇ ( ਪੋਰਟ_USING_MPU_WRAPPERS == 1 )
    ਬਣਤਰ x_ਸਮ੍ਰਿਤੀ_REGION;
    ਖਾਲੀ v_ਪੋਰਟ_Store_Task_MPUSettings( xMPU_SETTINGS * xMPUSettings,
                                    ਅਚਲ ਬਣਤਰ x_ਸਮ੍ਰਿਤੀ_REGION * ਅਚਲ xRegions,
                                    ਢੇਰ_Type_t * px_Bottom_ਦਾ_ਢੇਰ,
                                    config_ਢੇਰ_ਡੂੰਘਾਈ_TYPE ux_ਢੇਰ_ਡੂੰਘਾਈ ) PRIVILEGED_FUNCTION;
#ਜੇ_ਅੰਤ

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
#ਜੇ ( ਪੋਰਟ_USING_MPU_WRAPPERS == 1 )
    ਅਧਾਰ_Type_t x_ਪੋਰਟ_Is_Authorized_ਮੰਜ਼ਿਲ_Access_ਬਫਰ( ਅਚਲ ਖਾਲੀ * pv_ਬਫਰ,
                                                uint32_t ul_ਬਫਰ_ਲੰਮਾਈ,
                                                uint32_t ulAccessRequested ) PRIVILEGED_FUNCTION;
#ਜੇ_ਅੰਤ

/**
 * @brief Checks if the calling task is authorized to access the given kernel object.
 *
 * @param lInternalIndexOfKernelObject The index of the kernel object in the kernel
 *                                     object handle pool.
 *
 * @return pdTRUE if the calling task is authorized to access the kernel object,
 *         pdFALSE otherwise.
 */
#ਜੇ ( ( ਪੋਰਟ_USING_MPU_WRAPPERS == 1 ) && ( configUSE_MPU_WRAPPERS_V1 == 0 ) )

    ਅਧਾਰ_Type_t x_ਪੋਰਟ_Is_Authorized_ਮੰਜ਼ਿਲ_Access_Kernel_Object( int32_t l_ਅੰਦਰੂਨੀ_ਸੂਚਕਾਂਕ_ਦਾ_Kernel_Object ) PRIVILEGED_FUNCTION;

#ਜੇ_ਅੰਤ

/* *INDENT-OFF* */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
    }
#ਜੇ_ਅੰਤ
/* *INDENT-ON* */

#ਜੇ_ਅੰਤ /* PORTABLE_H */
