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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ PORTABLE_H
#ವ್ಯಾಖ್ಯಾನಿಸು PORTABLE_H

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
#ಅನ್ತರ್ಭಾವ "deprecated_definitions.h"

/* If portENTER_CRITICAL is not defined then including deprecated_definitions.h
 * did not result in a portmacro.h header file being included - and it should be
 * included here.  In this case the path to the correct portmacro.h header file
 * must be set in the compiler's include path. */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_ENTER_CRITICAL
    #ಅನ್ತರ್ಭಾವ "portmacro.h"
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ ಪೋರ್ಟ್_BYTE_ALIGNMENT == 32
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_BYTE_ALIGNMENT_ಮುಖವಾಡ    ( 0x001f )
#ಅಥವಾಆದರೆ ಪೋರ್ಟ್_BYTE_ALIGNMENT == 16
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_BYTE_ALIGNMENT_ಮುಖವಾಡ    ( 0x000f )
#ಅಥವಾಆದರೆ ಪೋರ್ಟ್_BYTE_ALIGNMENT == 8
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_BYTE_ALIGNMENT_ಮುಖವಾಡ    ( 0x0007 )
#ಅಥವಾಆದರೆ ಪೋರ್ಟ್_BYTE_ALIGNMENT == 4
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_BYTE_ALIGNMENT_ಮುಖವಾಡ    ( 0x0003 )
#ಅಥವಾಆದರೆ ಪೋರ್ಟ್_BYTE_ALIGNMENT == 2
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_BYTE_ALIGNMENT_ಮುಖವಾಡ    ( 0x0001 )
#ಅಥವಾಆದರೆ ಪೋರ್ಟ್_BYTE_ALIGNMENT == 1
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_BYTE_ALIGNMENT_ಮುಖವಾಡ    ( 0x0000 )
#ಇಲ್ಲದಿದ್ದರೆ /* if portBYTE_ALIGNMENT == 32 */
    #ದೋಷ "Invalid portBYTE_ALIGNMENT definition"
#ಆದರೆ_ಅಂತ್ಯ /* if portBYTE_ALIGNMENT == 32 */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_USING_MPU_WRAPPERS
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_USING_MPU_WRAPPERS    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_ಅಂಕೆ_CONFIGURABLE_REGIONS
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ಅಂಕೆ_CONFIGURABLE_REGIONS    1
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_HAS_ರಾಶಿ_ಅತಿಪ್ರವಾಹ_CHECKING
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_HAS_ರಾಶಿ_ಅತಿಪ್ರವಾಹ_CHECKING    0
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_ARCH_NAME
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ARCH_NAME    ಖಾಲಿ
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_ರಾಶಿ_ಆಳ_TYPE
    #ವ್ಯಾಖ್ಯಾನಿಸು config_ರಾಶಿ_ಆಳ_TYPE    ರಾಶಿ_Type_t
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ config_ರಾಶಿ_ಹಂಚಿಕೆ_FROM_SEPARATE_ರಾಶಿ
    /* Defaults to 0 for backward compatibility. */
    #ವ್ಯಾಖ್ಯಾನಿಸು config_ರಾಶಿ_ಹಂಚಿಕೆ_FROM_SEPARATE_ರಾಶಿ    0
#ಆದರೆ_ಅಂತ್ಯ

/* *INDENT-OFF* */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
    ಬಾಹ್ಯ "C" {
#ಆದರೆ_ಅಂತ್ಯ
/* *INDENT-ON* */

#ಅನ್ತರ್ಭಾವ "mpu_wrappers.h"

/*
 * Setup the stack of a new task so it is ready to be placed under the
 * scheduler control.  The registers have to be placed on the stack in
 * the order that the port expects to find them.
 *
 */
#ಆದರೆ ( ಪೋರ್ಟ್_USING_MPU_WRAPPERS == 1 )
    #ಆದರೆ ( ಪೋರ್ಟ್_HAS_ರಾಶಿ_ಅತಿಪ್ರವಾಹ_CHECKING == 1 )
        ರಾಶಿ_Type_t * px_ಪೋರ್ಟ್_Initialise_ರಾಶಿ( ರಾಶಿ_Type_t * px_ತುದಿ_ದ_ರಾಶಿ,
                                             ರಾಶಿ_Type_t * px_ಕೊನೆ_ದ_ರಾಶಿ,
                                             TaskFunction_t px_ಸಂಕೇತ,
                                             ಶೂನ್ಯ * pvParameters,
                                             ತಳ_Type_t x_ಸತತ_Privileged,
                                             xMPU_SETTINGS * xMPUSettings ) PRIVILEGED_FUNCTION;
    #ಇಲ್ಲದಿದ್ದರೆ
        ರಾಶಿ_Type_t * px_ಪೋರ್ಟ್_Initialise_ರಾಶಿ( ರಾಶಿ_Type_t * px_ತುದಿ_ದ_ರಾಶಿ,
                                             TaskFunction_t px_ಸಂಕೇತ,
                                             ಶೂನ್ಯ * pvParameters,
                                             ತಳ_Type_t x_ಸತತ_Privileged,
                                             xMPU_SETTINGS * xMPUSettings ) PRIVILEGED_FUNCTION;
    #ಆದರೆ_ಅಂತ್ಯ /* if ( portHAS_STACK_OVERFLOW_CHECKING == 1 ) */
#ಇಲ್ಲದಿದ್ದರೆ /* if ( portUSING_MPU_WRAPPERS == 1 ) */
    #ಆದರೆ ( ಪೋರ್ಟ್_HAS_ರಾಶಿ_ಅತಿಪ್ರವಾಹ_CHECKING == 1 )
        ರಾಶಿ_Type_t * px_ಪೋರ್ಟ್_Initialise_ರಾಶಿ( ರಾಶಿ_Type_t * px_ತುದಿ_ದ_ರಾಶಿ,
                                             ರಾಶಿ_Type_t * px_ಕೊನೆ_ದ_ರಾಶಿ,
                                             TaskFunction_t px_ಸಂಕೇತ,
                                             ಶೂನ್ಯ * pvParameters ) PRIVILEGED_FUNCTION;
    #ಇಲ್ಲದಿದ್ದರೆ
        ರಾಶಿ_Type_t * px_ಪೋರ್ಟ್_Initialise_ರಾಶಿ( ರಾಶಿ_Type_t * px_ತುದಿ_ದ_ರಾಶಿ,
                                             TaskFunction_t px_ಸಂಕೇತ,
                                             ಶೂನ್ಯ * pvParameters ) PRIVILEGED_FUNCTION;
    #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ /* if ( portUSING_MPU_WRAPPERS == 1 ) */

/* Used by heap_5.c to define the start address and size of each memory region
 * that together comprise the total FreeRTOS heap space. */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ರಾಶಿ_Region
{
    uint8_t * puc_ಆರಂಭ_ವಿಳಾಸ;
    size_t x_ಗಾತ್ರ_ಒಳ_ಬೈಟುಗಳು;
} ರಾಶಿ_Region_t;

/* Used to pass information about the heap out of vPortGetHeapStats(). */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ x_ರಾಶಿ_Stats
{
    size_t x_ಲಭ್ಯ_ರಾಶಿ_Space_ಒಳ_ಬೈಟುಗಳು;      /* The total heap size currently available - this is the sum of all the free blocks, not the largest block that can be allocated. */
    size_t x_ಗಾತ್ರ_ದ_ಅತಿದೊಡ್ಡ_Free_ಖಂಡ_ಒಳ_ಬೈಟುಗಳು;  /* The maximum size, in bytes, of all the free blocks within the heap at the time vPortGetHeapStats() is called. */
    size_t x_ಗಾತ್ರ_ದ_ಅತಿಚಿಕ್ಕ_Free_ಖಂಡ_ಒಳ_ಬೈಟುಗಳು; /* The minimum size, in bytes, of all the free blocks within the heap at the time vPortGetHeapStats() is called. */
    size_t x_ಸಂಖ್ಯೆ_ದ_Free_ಖಂಡಗಳು;             /* The number of free memory blocks within the heap at the time vPortGetHeapStats() is called. */
    size_t x_Minimum_Ever_Free_ಬೈಟುಗಳು_ಉಳಿದ;  /* The minimum amount of total free memory (sum of all free blocks) there has been in the heap since the system booted. */
    size_t x_ಸಂಖ್ಯೆ_ದ_Successful_Allocations;  /* The number of calls to pvPortMalloc() that have returned a valid memory block. */
    size_t x_ಸಂಖ್ಯೆ_ದ_Successful_Frees;        /* The number of calls to vPortFree() that has successfully freed a block of memory. */
} ರಾಶಿ_Stats_t;

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
ಶೂನ್ಯ v_ಪೋರ್ಟ್_Define_ರಾಶಿ_Regions( ಸ್ಥಿರಾಂಕ ರಾಶಿ_Region_t * ಸ್ಥಿರಾಂಕ px_ರಾಶಿ_Regions ) PRIVILEGED_FUNCTION;

/*
 * Returns a HeapStats_t structure filled with information about the current
 * heap state.
 */
ಶೂನ್ಯ v_ಪೋರ್ಟ್_Get_ರಾಶಿ_Stats( ರಾಶಿ_Stats_t * px_ರಾಶಿ_Stats );

/*
 * Map to the memory management routines required for the port.
 */
ಶೂನ್ಯ * pv_ಪೋರ್ಟ್_Malloc( size_t x_Wanted_ಗಾತ್ರ ) PRIVILEGED_FUNCTION;
ಶೂನ್ಯ * pv_ಪೋರ್ಟ್_Calloc( size_t x_ಅಂಕೆ,
                     size_t x_ಗಾತ್ರ ) PRIVILEGED_FUNCTION;
ಶೂನ್ಯ v_ಪೋರ್ಟ್_Free( ಶೂನ್ಯ * pv ) PRIVILEGED_FUNCTION;
ಶೂನ್ಯ v_ಪೋರ್ಟ್_Initialise_ಖಂಡಗಳು( ಶೂನ್ಯ ) PRIVILEGED_FUNCTION;
size_t x_ಪೋರ್ಟ್_Get_Free_ರಾಶಿ_ಗಾತ್ರ( ಶೂನ್ಯ ) PRIVILEGED_FUNCTION;
size_t x_ಪೋರ್ಟ್_Get_Minimum_Ever_Free_ರಾಶಿ_ಗಾತ್ರ( ಶೂನ್ಯ ) PRIVILEGED_FUNCTION;

#ಆದರೆ ( config_ರಾಶಿ_ಹಂಚಿಕೆ_FROM_SEPARATE_ರಾಶಿ == 1 )
    ಶೂನ್ಯ * pv_ಪೋರ್ಟ್_Malloc_ರಾಶಿ( size_t x_ಗಾತ್ರ ) PRIVILEGED_FUNCTION;
    ಶೂನ್ಯ v_ಪೋರ್ಟ್_Free_ರಾಶಿ( ಶೂನ್ಯ * pv ) PRIVILEGED_FUNCTION;
#ಇಲ್ಲದಿದ್ದರೆ
    #ವ್ಯಾಖ್ಯಾನಿಸು pv_ಪೋರ್ಟ್_Malloc_ರಾಶಿ    pv_ಪೋರ್ಟ್_Malloc
    #ವ್ಯಾಖ್ಯಾನಿಸು v_ಪೋರ್ಟ್_Free_ರಾಶಿ       v_ಪೋರ್ಟ್_Free
#ಆದರೆ_ಅಂತ್ಯ

/*
 * This function resets the internal state of the heap module. It must be called
 * by the application before restarting the scheduler.
 */
ಶೂನ್ಯ v_ಪೋರ್ಟ್_ರಾಶಿ_Reset_State( ಶೂನ್ಯ ) PRIVILEGED_FUNCTION;

#ಆದರೆ ( configUSE_MALLOC_FAILED_HOOK == 1 )

/**
 * task.h
 * @code{c}
 * void vApplicationMallocFailedHook( void )
 * @endcode
 *
 * This hook function is called when allocation failed.
 */
    ಶೂನ್ಯ vApplicationMallocFailedHook( ಶೂನ್ಯ );
#ಆದರೆ_ಅಂತ್ಯ

/*
 * Setup the hardware ready for the scheduler to take control.  This generally
 * sets up a tick interrupt and sets timers for the correct tick frequency.
 */
ತಳ_Type_t x_ಪೋರ್ಟ್_ಆರಂಭ_Scheduler( ಶೂನ್ಯ ) PRIVILEGED_FUNCTION;

/*
 * Undo any hardware/ISR setup that was performed by xPortStartScheduler() so
 * the hardware is left in its original condition after the scheduler stops
 * executing.
 */
ಶೂನ್ಯ v_ಪೋರ್ಟ್_ಕೊನೆ_Scheduler( ಶೂನ್ಯ ) PRIVILEGED_FUNCTION;

/*
 * The structures and methods of manipulating the MPU are contained within the
 * port layer.
 *
 * Fills the xMPUSettings structure with the memory region information
 * contained in xRegions.
 */
#ಆದರೆ ( ಪೋರ್ಟ್_USING_MPU_WRAPPERS == 1 )
    ರಚನೆ x_ಸ್ಮರಣೆ_REGION;
    ಶೂನ್ಯ v_ಪೋರ್ಟ್_Store_Task_MPUSettings( xMPU_SETTINGS * xMPUSettings,
                                    ಸ್ಥಿರಾಂಕ ರಚನೆ x_ಸ್ಮರಣೆ_REGION * ಸ್ಥಿರಾಂಕ xRegions,
                                    ರಾಶಿ_Type_t * px_Bottom_ದ_ರಾಶಿ,
                                    config_ರಾಶಿ_ಆಳ_TYPE ux_ರಾಶಿ_ಆಳ ) PRIVILEGED_FUNCTION;
#ಆದರೆ_ಅಂತ್ಯ

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
#ಆದರೆ ( ಪೋರ್ಟ್_USING_MPU_WRAPPERS == 1 )
    ತಳ_Type_t x_ಪೋರ್ಟ್_Is_Authorized_ಗಮ್ಯ_Access_ಬಫರ್( ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ * pv_ಬಫರ್,
                                                uint32_t ul_ಬಫರ್_ಉದ್ದಳತೆ,
                                                uint32_t ulAccessRequested ) PRIVILEGED_FUNCTION;
#ಆದರೆ_ಅಂತ್ಯ

/**
 * @brief Checks if the calling task is authorized to access the given kernel object.
 *
 * @param lInternalIndexOfKernelObject The index of the kernel object in the kernel
 *                                     object handle pool.
 *
 * @return pdTRUE if the calling task is authorized to access the kernel object,
 *         pdFALSE otherwise.
 */
#ಆದರೆ ( ( ಪೋರ್ಟ್_USING_MPU_WRAPPERS == 1 ) && ( configUSE_MPU_WRAPPERS_V1 == 0 ) )

    ತಳ_Type_t x_ಪೋರ್ಟ್_Is_Authorized_ಗಮ್ಯ_Access_Kernel_Object( int32_t l_ಆಂತರಿಕ_ಸೂಚ್ಯಂಕ_ದ_Kernel_Object ) PRIVILEGED_FUNCTION;

#ಆದರೆ_ಅಂತ್ಯ

/* *INDENT-OFF* */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
    }
#ಆದರೆ_ಅಂತ್ಯ
/* *INDENT-ON* */

#ಆದರೆ_ಅಂತ್ಯ /* PORTABLE_H */
