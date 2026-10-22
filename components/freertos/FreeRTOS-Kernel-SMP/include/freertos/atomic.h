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

/**
 * @file atomic.h
 * @brief FreeRTOS atomic operation support.
 *
 * This file implements atomic functions by disabling interrupts globally.
 * Implementations with architecture specific atomic instructions can be
 * provided under each compiler directory.
 *
 * The atomic interface can be used in FreeRTOS tasks on all FreeRTOS ports. It
 * can also be used in Interrupt Service Routines (ISRs) on FreeRTOS ports that
 * support nested interrupts (i.e. portHAS_NESTED_INTERRUPTS is set to 1). The
 * atomic interface must not be used in ISRs on FreeRTOS ports that do not
 * support nested interrupts (i.e. portHAS_NESTED_INTERRUPTS is set to 0)
 * because ISRs on these ports cannot be interrupted and therefore, do not need
 * atomics in ISRs.
 */

#اگر_تعریف_نشده ATOMIC_H
#تعریف_کن ATOMIC_H

#اگر_تعریف_نشده INC_FREERTOS_H
    #خطا "include FreeRTOS.h must appear in source files before include atomic.h"
#پایان_اگر

/* Standard includes. */
#ضمن <stdint.h>

/* *INDENT-OFF* */
#اگر_تعریف_شده __cplusplus
    خارجي "C" {
#پایان_اگر
/* *INDENT-ON* */

/*
 * Port specific definitions -- entering/exiting critical section.
 * Refer template -- ./lib/FreeRTOS/portable/Compiler/Arch/portmacro.h
 *
 * Every call to ATOMIC_EXIT_CRITICAL() must be closely paired with
 * ATOMIC_ENTER_CRITICAL().
 *
 */
#إذا ( پورت_HAS_NESTED_INTERRUPTS == 1 )

/* Nested interrupt scheme is supported in this port. */
    #تعریف_کن ATOMIC_ENTER_CRITICAL() \
    UBaseType_t uxCriticalSectionType = پورت_مجموعه_وقفه_ماسک_FROM_ISR()

    #تعریف_کن ATOMIC_EXIT_CRITICAL() \
    پورت_CLEAR_وقفه_ماسک_FROM_ISR( uxCriticalSectionType )

#وإلا

/* Nested interrupt scheme is NOT supported in this port. */
    #تعریف_کن ATOMIC_ENTER_CRITICAL()    پورت_ENTER_CRITICAL()
    #تعریف_کن ATOMIC_EXIT_CRITICAL()     پورت_EXIT_CRITICAL()

#پایان_اگر /* portSET_INTERRUPT_MASK_FROM_ISR() */

/*
 * Port specific definition -- "always inline".
 * Inline is compiler specific, and may not always get inlined depending on your
 * optimization level.  Also, inline is considered as performance optimization
 * for atomic.  Thus, if portFORCE_INLINE is not provided by portmacro.h,
 * instead of resulting error, simply define it away.
 */
#اگر_تعریف_نشده پورت_FORCE_INLINE
    #تعریف_کن پورت_FORCE_INLINE
#پایان_اگر

#تعریف_کن ATOMIC_COMPARE_AND_جابجا_SUCCESS    0x1U     /**< Compare and swap succeeded, swapped. */
#تعریف_کن ATOMIC_COMPARE_AND_جابجا_FAILURE    0x0U     /**< Compare and swap failed, did not swap. */

/*----------------------------- Swap && CAS ------------------------------*/

/**
 * Atomic compare-and-swap
 *
 * @brief Performs an atomic compare-and-swap operation on the specified values.
 *
 * @param[in, out] pulDestination  Pointer to memory location from where value is
 *                               to be loaded and checked.
 * @param[in] ulExchange         If condition meets, write this value to memory.
 * @param[in] ulComparand        Swap condition.
 *
 * @return Unsigned integer of value 1 or 0. 1 for swapped, 0 for not swapped.
 *
 * @note This function only swaps *pulDestination with ulExchange, if previous
 *       *pulDestination value equals ulComparand.
 */
ساكن پورت_FORCE_INLINE uint32_t Atomic_Compare_And_جابجا_u32( uint32_t متطاير * pulDestination,
                                                            uint32_t ulExchange,
                                                            uint32_t ulComparand )
{
    uint32_t ul_Return_مقدار;

    ATOMIC_ENTER_CRITICAL();
    {
        إذا( *pulDestination == ulComparand )
        {
            *pulDestination = ulExchange;
            ul_Return_مقدار = ATOMIC_COMPARE_AND_جابجا_SUCCESS;
        }
        وإلا
        {
            ul_Return_مقدار = ATOMIC_COMPARE_AND_جابجا_FAILURE;
        }
    }
    ATOMIC_EXIT_CRITICAL();

    أرجع ul_Return_مقدار;
}
/*-----------------------------------------------------------*/

/**
 * Atomic swap (pointers)
 *
 * @brief Atomically sets the address pointed to by *ppvDestination to the value
 *        of *pvExchange.
 *
 * @param[in, out] ppvDestination  Pointer to memory location from where a pointer
 *                                 value is to be loaded and written back to.
 * @param[in] pvExchange           Pointer value to be written to *ppvDestination.
 *
 * @return The initial value of *ppvDestination.
 */
ساكن پورت_FORCE_INLINE فراغ * Atomic_جابجا_اشارهگرها_p32( فراغ * متطاير * ppvDestination,
                                                        فراغ * pvExchange )
{
    فراغ * p_Return_مقدار;

    ATOMIC_ENTER_CRITICAL();
    {
        p_Return_مقدار = *ppvDestination;
        *ppvDestination = pvExchange;
    }
    ATOMIC_EXIT_CRITICAL();

    أرجع p_Return_مقدار;
}
/*-----------------------------------------------------------*/

/**
 * Atomic compare-and-swap (pointers)
 *
 * @brief Performs an atomic compare-and-swap operation on the specified pointer
 *        values.
 *
 * @param[in, out] ppvDestination  Pointer to memory location from where a pointer
 *                                 value is to be loaded and checked.
 * @param[in] pvExchange           If condition meets, write this value to memory.
 * @param[in] pvComparand          Swap condition.
 *
 * @return Unsigned integer of value 1 or 0. 1 for swapped, 0 for not swapped.
 *
 * @note This function only swaps *ppvDestination with pvExchange, if previous
 *       *ppvDestination value equals pvComparand.
 */
ساكن پورت_FORCE_INLINE uint32_t Atomic_Compare_And_جابجا_اشارهگرها_p32( فراغ * متطاير * ppvDestination,
                                                                    فراغ * pvExchange,
                                                                    فراغ * pvComparand )
{
    uint32_t ul_Return_مقدار = ATOMIC_COMPARE_AND_جابجا_FAILURE;

    ATOMIC_ENTER_CRITICAL();
    {
        إذا( *ppvDestination == pvComparand )
        {
            *ppvDestination = pvExchange;
            ul_Return_مقدار = ATOMIC_COMPARE_AND_جابجا_SUCCESS;
        }
    }
    ATOMIC_EXIT_CRITICAL();

    أرجع ul_Return_مقدار;
}


/*----------------------------- Arithmetic ------------------------------*/

/**
 * Atomic add
 *
 * @brief Atomically adds count to the value of the specified pointer points to.
 *
 * @param[in,out] pulAddend  Pointer to memory location from where value is to be
 *                         loaded and written back to.
 * @param[in] ulCount      Value to be added to *pulAddend.
 *
 * @return previous *pulAddend value.
 */
ساكن پورت_FORCE_INLINE uint32_t Atomic_جمع_کن_u32( uint32_t متطاير * pulAddend,
                                                 uint32_t ul_شمار )
{
    uint32_t ulCurrent;

    ATOMIC_ENTER_CRITICAL();
    {
        ulCurrent = *pulAddend;
        *pulAddend += ul_شمار;
    }
    ATOMIC_EXIT_CRITICAL();

    أرجع ulCurrent;
}
/*-----------------------------------------------------------*/

/**
 * Atomic subtract
 *
 * @brief Atomically subtracts count from the value of the specified pointer
 *        pointers to.
 *
 * @param[in,out] pulAddend  Pointer to memory location from where value is to be
 *                         loaded and written back to.
 * @param[in] ulCount      Value to be subtract from *pulAddend.
 *
 * @return previous *pulAddend value.
 */
ساكن پورت_FORCE_INLINE uint32_t Atomic_تفریق_u32( uint32_t متطاير * pulAddend,
                                                      uint32_t ul_شمار )
{
    uint32_t ulCurrent;

    ATOMIC_ENTER_CRITICAL();
    {
        ulCurrent = *pulAddend;
        *pulAddend -= ul_شمار;
    }
    ATOMIC_EXIT_CRITICAL();

    أرجع ulCurrent;
}
/*-----------------------------------------------------------*/

/**
 * Atomic increment
 *
 * @brief Atomically increments the value of the specified pointer points to.
 *
 * @param[in,out] pulAddend  Pointer to memory location from where value is to be
 *                         loaded and written back to.
 *
 * @return *pulAddend value before increment.
 */
ساكن پورت_FORCE_INLINE uint32_t Atomic_Increment_u32( uint32_t متطاير * pulAddend )
{
    uint32_t ulCurrent;

    ATOMIC_ENTER_CRITICAL();
    {
        ulCurrent = *pulAddend;
        *pulAddend += 1;
    }
    ATOMIC_EXIT_CRITICAL();

    أرجع ulCurrent;
}
/*-----------------------------------------------------------*/

/**
 * Atomic decrement
 *
 * @brief Atomically decrements the value of the specified pointer points to
 *
 * @param[in,out] pulAddend  Pointer to memory location from where value is to be
 *                         loaded and written back to.
 *
 * @return *pulAddend value before decrement.
 */
ساكن پورت_FORCE_INLINE uint32_t Atomic_Decrement_u32( uint32_t متطاير * pulAddend )
{
    uint32_t ulCurrent;

    ATOMIC_ENTER_CRITICAL();
    {
        ulCurrent = *pulAddend;
        *pulAddend -= 1;
    }
    ATOMIC_EXIT_CRITICAL();

    أرجع ulCurrent;
}

/*----------------------------- Bitwise Logical ------------------------------*/

/**
 * Atomic OR
 *
 * @brief Performs an atomic OR operation on the specified values.
 *
 * @param [in, out] pulDestination  Pointer to memory location from where value is
 *                                to be loaded and written back to.
 * @param [in] ulValue            Value to be ORed with *pulDestination.
 *
 * @return The original value of *pulDestination.
 */
ساكن پورت_FORCE_INLINE uint32_t Atomic_OR_u32( uint32_t متطاير * pulDestination,
                                                uint32_t ul_مقدار )
{
    uint32_t ulCurrent;

    ATOMIC_ENTER_CRITICAL();
    {
        ulCurrent = *pulDestination;
        *pulDestination |= ul_مقدار;
    }
    ATOMIC_EXIT_CRITICAL();

    أرجع ulCurrent;
}
/*-----------------------------------------------------------*/

/**
 * Atomic AND
 *
 * @brief Performs an atomic AND operation on the specified values.
 *
 * @param [in, out] pulDestination  Pointer to memory location from where value is
 *                                to be loaded and written back to.
 * @param [in] ulValue            Value to be ANDed with *pulDestination.
 *
 * @return The original value of *pulDestination.
 */
ساكن پورت_FORCE_INLINE uint32_t Atomic_AND_u32( uint32_t متطاير * pulDestination,
                                                 uint32_t ul_مقدار )
{
    uint32_t ulCurrent;

    ATOMIC_ENTER_CRITICAL();
    {
        ulCurrent = *pulDestination;
        *pulDestination &= ul_مقدار;
    }
    ATOMIC_EXIT_CRITICAL();

    أرجع ulCurrent;
}
/*-----------------------------------------------------------*/

/**
 * Atomic NAND
 *
 * @brief Performs an atomic NAND operation on the specified values.
 *
 * @param [in, out] pulDestination  Pointer to memory location from where value is
 *                                to be loaded and written back to.
 * @param [in] ulValue            Value to be NANDed with *pulDestination.
 *
 * @return The original value of *pulDestination.
 */
ساكن پورت_FORCE_INLINE uint32_t Atomic_NAND_u32( uint32_t متطاير * pulDestination,
                                                  uint32_t ul_مقدار )
{
    uint32_t ulCurrent;

    ATOMIC_ENTER_CRITICAL();
    {
        ulCurrent = *pulDestination;
        *pulDestination = ~( ulCurrent & ul_مقدار );
    }
    ATOMIC_EXIT_CRITICAL();

    أرجع ulCurrent;
}
/*-----------------------------------------------------------*/

/**
 * Atomic XOR
 *
 * @brief Performs an atomic XOR operation on the specified values.
 *
 * @param [in, out] pulDestination  Pointer to memory location from where value is
 *                                to be loaded and written back to.
 * @param [in] ulValue            Value to be XORed with *pulDestination.
 *
 * @return The original value of *pulDestination.
 */
ساكن پورت_FORCE_INLINE uint32_t Atomic_XOR_u32( uint32_t متطاير * pulDestination,
                                                 uint32_t ul_مقدار )
{
    uint32_t ulCurrent;

    ATOMIC_ENTER_CRITICAL();
    {
        ulCurrent = *pulDestination;
        *pulDestination ^= ul_مقدار;
    }
    ATOMIC_EXIT_CRITICAL();

    أرجع ulCurrent;
}

/* *INDENT-OFF* */
#اگر_تعریف_شده __cplusplus
    }
#پایان_اگر
/* *INDENT-ON* */

#پایان_اگر /* ATOMIC_H */
