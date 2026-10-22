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

#ifndef PORTABLE_H
#define PORTABLE_H

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
#ضمن "deprecated_definitions.h"

/* If portENTER_CRITICAL is not defined then including deprecated_definitions.h
 * did not result in a portmacro.h header file being included - and it should be
 * included here.  In this case the path to the correct portmacro.h header file
 * must be set in the compiler's include path. */
#ifndef portENTER_CRITICAL
    #ضمن "portmacro.h"
#endif

#إذا portBYTE_ALIGNMENT == 32
    #define portBYTE_ALIGNMENT_MASK    ( 0x001f )
#وإلاإذا portBYTE_ALIGNMENT == 16
    #define portBYTE_ALIGNMENT_MASK    ( 0x000f )
#وإلاإذا portBYTE_ALIGNMENT == 8
    #define portBYTE_ALIGNMENT_MASK    ( 0x0007 )
#وإلاإذا portBYTE_ALIGNMENT == 4
    #define portBYTE_ALIGNMENT_MASK    ( 0x0003 )
#وإلاإذا portBYTE_ALIGNMENT == 2
    #define portBYTE_ALIGNMENT_MASK    ( 0x0001 )
#وإلاإذا portBYTE_ALIGNMENT == 1
    #define portBYTE_ALIGNMENT_MASK    ( 0x0000 )
#وإلا /* if portBYTE_ALIGNMENT == 32 */
    #error "Invalid portBYTE_ALIGNMENT definition"
#endif /* if portBYTE_ALIGNMENT == 32 */

#ifndef portUSING_MPU_WRAPPERS
    #define portUSING_MPU_WRAPPERS    0
#endif

#ifndef port_لمب_CONFIGURABLE_REGIONS
    #define port_لمب_CONFIGURABLE_REGIONS    1
#endif

#ifndef port_HAS_تر_OVERFLOW_CHECKING
    #define port_HAS_تر_OVERFLOW_CHECKING    0
#endif

#ifndef portARCH_NAME
    #define portARCH_NAME    عدم
#endif

#ifndef config_تر_DEPTH_TYPE
    #define config_تر_DEPTH_TYPE    تر_Type_t
#endif

#ifndef config_تر_ALLOCATION_FROM_SEPARATE_HEAP
    /* Defaults to 0 for backward compatibility. */
    #define config_تر_ALLOCATION_FROM_SEPARATE_HEAP    0
#endif

/* *INDENT-OFF* */
#ifdef __cplusplus
    خارجي "C" {
#endif
/* *INDENT-ON* */

#ضمن "mpu_wrappers.h"

/*
 * Setup the stack of a new task so it is ready to be placed under the
 * scheduler control.  The registers have to be placed on the stack in
 * the order that the port expects to find them.
 *
 */
#إذا ( portUSING_MPU_WRAPPERS == 1 )
    #إذا ( port_HAS_تر_OVERFLOW_CHECKING == 1 )
        تر_Type_t * px_Port_Initialise_تر( تر_Type_t * px_كولي_Of_تر,
                                             تر_Type_t * px_End_Of_تر,
                                             TaskFunction_t pxCode,
                                             فراغ * pvParameters,
                                             BaseType_t xRunPrivileged,
                                             xMPU_SETTINGS * xMPUSettings ) PRIVILEGED_FUNCTION;
    #وإلا
        تر_Type_t * px_Port_Initialise_تر( تر_Type_t * px_كولي_Of_تر,
                                             TaskFunction_t pxCode,
                                             فراغ * pvParameters,
                                             BaseType_t xRunPrivileged,
                                             xMPU_SETTINGS * xMPUSettings ) PRIVILEGED_FUNCTION;
    #endif /* if ( portHAS_STACK_OVERFLOW_CHECKING == 1 ) */
#وإلا /* if ( portUSING_MPU_WRAPPERS == 1 ) */
    #إذا ( port_HAS_تر_OVERFLOW_CHECKING == 1 )
        تر_Type_t * px_Port_Initialise_تر( تر_Type_t * px_كولي_Of_تر,
                                             تر_Type_t * px_End_Of_تر,
                                             TaskFunction_t pxCode,
                                             فراغ * pvParameters ) PRIVILEGED_FUNCTION;
    #وإلا
        تر_Type_t * px_Port_Initialise_تر( تر_Type_t * px_كولي_Of_تر,
                                             TaskFunction_t pxCode,
                                             فراغ * pvParameters ) PRIVILEGED_FUNCTION;
    #endif
#endif /* if ( portUSING_MPU_WRAPPERS == 1 ) */

/* Used by heap_5.c to define the start address and size of each memory region
 * that together comprise the total FreeRTOS heap space. */
تعريفنوع بنية HeapRegion
{
    uint8_t * pucStartAddress;
    size_t xSizeInBytes;
} HeapRegion_t;

/* Used to pass information about the heap out of vPortGetHeapStats(). */
تعريفنوع بنية xHeapStats
{
    size_t xAvailableHeapSpaceInBytes;      /* The total heap size currently available - this is the sum of all the free blocks, not the largest block that can be allocated. */
    size_t xSizeOfLargestFreeBlockInBytes;  /* The maximum size, in bytes, of all the free blocks within the heap at the time vPortGetHeapStats() is called. */
    size_t xSizeOfSmallestFreeBlockInBytes; /* The minimum size, in bytes, of all the free blocks within the heap at the time vPortGetHeapStats() is called. */
    size_t x_لمب_Of_Free_Blocks;             /* The number of free memory blocks within the heap at the time vPortGetHeapStats() is called. */
    size_t xMinimumEverFreeBytesRemaining;  /* The minimum amount of total free memory (sum of all free blocks) there has been in the heap since the system booted. */
    size_t x_لمب_Of_Successful_Allocations;  /* The number of calls to pvPortMalloc() that have returned a valid memory block. */
    size_t x_لمب_Of_Successful_Frees;        /* The number of calls to vPortFree() that has successfully freed a block of memory. */
} HeapStats_t;

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
فراغ vPortDefineHeapRegions( ثابت HeapRegion_t * ثابت pxHeapRegions ) PRIVILEGED_FUNCTION;

/*
 * Returns a HeapStats_t structure filled with information about the current
 * heap state.
 */
فراغ vPortGetHeapStats( HeapStats_t * pxHeapStats );

/*
 * Map to the memory management routines required for the port.
 */
فراغ * pvPortMalloc( size_t xWantedSize ) PRIVILEGED_FUNCTION;
فراغ * pvPortCalloc( size_t x_لمب,
                     size_t xSize ) PRIVILEGED_FUNCTION;
فراغ vPortFree( فراغ * pv ) PRIVILEGED_FUNCTION;
فراغ vPortInitialiseBlocks( فراغ ) PRIVILEGED_FUNCTION;
size_t xPortGetFreeHeapSize( فراغ ) PRIVILEGED_FUNCTION;
size_t xPortGetMinimumEverFreeHeapSize( فراغ ) PRIVILEGED_FUNCTION;

#إذا ( config_تر_ALLOCATION_FROM_SEPARATE_HEAP == 1 )
    فراغ * pv_Port_Malloc_تر( size_t xSize ) PRIVILEGED_FUNCTION;
    فراغ v_Port_Free_تر( فراغ * pv ) PRIVILEGED_FUNCTION;
#وإلا
    #define pv_Port_Malloc_تر    pvPortMalloc
    #define v_Port_Free_تر       vPortFree
#endif

/*
 * This function resets the internal state of the heap module. It must be called
 * by the application before restarting the scheduler.
 */
فراغ vPortHeapResetState( فراغ ) PRIVILEGED_FUNCTION;

#إذا ( configUSE_MALLOC_FAILED_HOOK == 1 )

/**
 * task.h
 * @code{c}
 * void vApplicationMallocFailedHook( void )
 * @endcode
 *
 * This hook function is called when allocation failed.
 */
    فراغ vApplicationMallocFailedHook( فراغ );
#endif

/*
 * Setup the hardware ready for the scheduler to take control.  This generally
 * sets up a tick interrupt and sets timers for the correct tick frequency.
 */
BaseType_t xPortStartScheduler( فراغ ) PRIVILEGED_FUNCTION;

/*
 * Undo any hardware/ISR setup that was performed by xPortStartScheduler() so
 * the hardware is left in its original condition after the scheduler stops
 * executing.
 */
فراغ vPortEndScheduler( فراغ ) PRIVILEGED_FUNCTION;

/*
 * The structures and methods of manipulating the MPU are contained within the
 * port layer.
 *
 * Fills the xMPUSettings structure with the memory region information
 * contained in xRegions.
 */
#إذا ( portUSING_MPU_WRAPPERS == 1 )
    بنية xMEMORY_REGION;
    فراغ vPortStoreTaskMPUSettings( xMPU_SETTINGS * xMPUSettings,
                                    ثابت بنية xMEMORY_REGION * ثابت xRegions,
                                    تر_Type_t * px_Bottom_Of_تر,
                                    config_تر_DEPTH_TYPE ux_تر_Depth ) PRIVILEGED_FUNCTION;
#endif

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
#إذا ( portUSING_MPU_WRAPPERS == 1 )
    BaseType_t xPortIsAuthorizedToAccessBuffer( ثابت فراغ * pvBuffer,
                                                uint32_t ulBufferLength,
                                                uint32_t ulAccessRequested ) PRIVILEGED_FUNCTION;
#endif

/**
 * @brief Checks if the calling task is authorized to access the given kernel object.
 *
 * @param lInternalIndexOfKernelObject The index of the kernel object in the kernel
 *                                     object handle pool.
 *
 * @return pdTRUE if the calling task is authorized to access the kernel object,
 *         pdFALSE otherwise.
 */
#إذا ( ( portUSING_MPU_WRAPPERS == 1 ) && ( configUSE_MPU_WRAPPERS_V1 == 0 ) )

    BaseType_t xPortIsAuthorizedToAccessKernelObject( int32_t lInternalIndexOfKernelObject ) PRIVILEGED_FUNCTION;

#endif

/* *INDENT-OFF* */
#ifdef __cplusplus
    }
#endif
/* *INDENT-ON* */

#endif /* PORTABLE_H */
