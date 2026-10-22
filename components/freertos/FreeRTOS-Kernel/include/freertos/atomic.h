/*
 * FreeRTOS Kernel V10.5.1 (ESP-IDF SMP modified)
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-FileCopyrightText: 2021 Amazon.com, Inc. or its affiliates
 *
 * SPDX-License-Identifier: MIT
 *
 * SPDX-FileContributor: 2023 Espressif Systems (Shanghai) CO LTD
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
 */

#اگر_غیر_تعریف_شدہ ATOMIC_H
#تعریف_کرو ATOMIC_H

#اگر_غیر_تعریف_شدہ INC_FREERTOS_H
    #غلطی "include FreeRTOS.h must appear in source files before include atomic.h"
#اگر_ختم

/* Standard includes. */
#شامل <stdint.h>

/* *INDENT-OFF* */
#اگر_تعریف_شدہ __cplusplus
    خارجی "C" {
#اگر_ختم
/* *INDENT-ON* */

/*
 * Port specific definitions -- entering/exiting critical section.
 * Refer template -- ./lib/FreeRTOS/portable/Compiler/Arch/portmacro.h
 *
 * Every call to ATOMIC_EXIT_CRITICAL() must be closely paired with
 * ATOMIC_ENTER_CRITICAL().
 *
 */
#اگر defined( پورٹ_سیٹ_مداخلت_نقاب_FROM_ISR )

/* Nested interrupt scheme is supported in this port. */
    #تعریف_کرو ATOMIC_ENTER_CRITICAL() \
    UBaseType_t uxCriticalSectionType = پورٹ_سیٹ_مداخلت_نقاب_FROM_ISR()

    #تعریف_کرو ATOMIC_EXIT_CRITICAL() \
    پورٹ_CLEAR_مداخلت_نقاب_FROM_ISR( uxCriticalSectionType )

#ورنہ

/* Nested interrupt scheme is NOT supported in this port. */
    #تعریف_کرو ATOMIC_ENTER_CRITICAL()    پورٹ_ENTER_CRITICAL()
    #تعریف_کرو ATOMIC_EXIT_CRITICAL()     پورٹ_EXIT_CRITICAL()

#اگر_ختم /* portSET_INTERRUPT_MASK_FROM_ISR() */

/*
 * Port specific definition -- "always inline".
 * Inline is compiler specific, and may not always get inlined depending on your
 * optimization level.  Also, inline is considered as performance optimization
 * for atomic.  Thus, if portFORCE_INLINE is not provided by portmacro.h,
 * instead of resulting error, simply define it away.
 */
#اگر_غیر_تعریف_شدہ پورٹ_FORCE_INLINE
    #تعریف_کرو پورٹ_FORCE_INLINE
#اگر_ختم

#تعریف_کرو ATOMIC_COMPARE_AND_تبادلہ_SUCCESS    0x1U     /**< Compare and swap succeeded, swapped. */
#تعریف_کرو ATOMIC_COMPARE_AND_تبادلہ_FAILURE    0x0U     /**< Compare and swap failed, did not swap. */

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
ساکن پورٹ_FORCE_INLINE uint32_t Atomic_Compare_And_تبادلہ_u32( uint32_t متلون * pulDestination,
                                                            uint32_t ulExchange,
                                                            uint32_t ulComparand )
{
    uint32_t ul_Return_قدر;

    ATOMIC_ENTER_CRITICAL();
    {
        اگر( *pulDestination == ulComparand )
        {
            *pulDestination = ulExchange;
            ul_Return_قدر = ATOMIC_COMPARE_AND_تبادلہ_SUCCESS;
        }
        ورنہ
        {
            ul_Return_قدر = ATOMIC_COMPARE_AND_تبادلہ_FAILURE;
        }
    }
    ATOMIC_EXIT_CRITICAL();

    واپس ul_Return_قدر;
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
ساکن پورٹ_FORCE_INLINE خالی * Atomic_تبادلہ_اشارے_p32( خالی * متلون * ppvDestination,
                                                        خالی * pvExchange )
{
    خالی * p_Return_قدر;

    ATOMIC_ENTER_CRITICAL();
    {
        p_Return_قدر = *ppvDestination;
        *ppvDestination = pvExchange;
    }
    ATOMIC_EXIT_CRITICAL();

    واپس p_Return_قدر;
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
ساکن پورٹ_FORCE_INLINE uint32_t Atomic_Compare_And_تبادلہ_اشارے_p32( خالی * متلون * ppvDestination,
                                                                    خالی * pvExchange,
                                                                    خالی * pvComparand )
{
    uint32_t ul_Return_قدر = ATOMIC_COMPARE_AND_تبادلہ_FAILURE;

    ATOMIC_ENTER_CRITICAL();
    {
        اگر( *ppvDestination == pvComparand )
        {
            *ppvDestination = pvExchange;
            ul_Return_قدر = ATOMIC_COMPARE_AND_تبادلہ_SUCCESS;
        }
    }
    ATOMIC_EXIT_CRITICAL();

    واپس ul_Return_قدر;
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
ساکن پورٹ_FORCE_INLINE uint32_t Atomic_جوڑو_u32( uint32_t متلون * pulAddend,
                                                 uint32_t ul_گنتی )
{
    uint32_t ulCurrent;

    ATOMIC_ENTER_CRITICAL();
    {
        ulCurrent = *pulAddend;
        *pulAddend += ul_گنتی;
    }
    ATOMIC_EXIT_CRITICAL();

    واپس ulCurrent;
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
ساکن پورٹ_FORCE_INLINE uint32_t Atomic_تفریق_u32( uint32_t متلون * pulAddend,
                                                      uint32_t ul_گنتی )
{
    uint32_t ulCurrent;

    ATOMIC_ENTER_CRITICAL();
    {
        ulCurrent = *pulAddend;
        *pulAddend -= ul_گنتی;
    }
    ATOMIC_EXIT_CRITICAL();

    واپس ulCurrent;
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
ساکن پورٹ_FORCE_INLINE uint32_t Atomic_Increment_u32( uint32_t متلون * pulAddend )
{
    uint32_t ulCurrent;

    ATOMIC_ENTER_CRITICAL();
    {
        ulCurrent = *pulAddend;
        *pulAddend += 1;
    }
    ATOMIC_EXIT_CRITICAL();

    واپس ulCurrent;
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
ساکن پورٹ_FORCE_INLINE uint32_t Atomic_Decrement_u32( uint32_t متلون * pulAddend )
{
    uint32_t ulCurrent;

    ATOMIC_ENTER_CRITICAL();
    {
        ulCurrent = *pulAddend;
        *pulAddend -= 1;
    }
    ATOMIC_EXIT_CRITICAL();

    واپس ulCurrent;
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
ساکن پورٹ_FORCE_INLINE uint32_t Atomic_OR_u32( uint32_t متلون * pulDestination,
                                                uint32_t ul_قدر )
{
    uint32_t ulCurrent;

    ATOMIC_ENTER_CRITICAL();
    {
        ulCurrent = *pulDestination;
        *pulDestination |= ul_قدر;
    }
    ATOMIC_EXIT_CRITICAL();

    واپس ulCurrent;
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
ساکن پورٹ_FORCE_INLINE uint32_t Atomic_AND_u32( uint32_t متلون * pulDestination,
                                                 uint32_t ul_قدر )
{
    uint32_t ulCurrent;

    ATOMIC_ENTER_CRITICAL();
    {
        ulCurrent = *pulDestination;
        *pulDestination &= ul_قدر;
    }
    ATOMIC_EXIT_CRITICAL();

    واپس ulCurrent;
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
ساکن پورٹ_FORCE_INLINE uint32_t Atomic_NAND_u32( uint32_t متلون * pulDestination,
                                                  uint32_t ul_قدر )
{
    uint32_t ulCurrent;

    ATOMIC_ENTER_CRITICAL();
    {
        ulCurrent = *pulDestination;
        *pulDestination = ~( ulCurrent & ul_قدر );
    }
    ATOMIC_EXIT_CRITICAL();

    واپس ulCurrent;
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
ساکن پورٹ_FORCE_INLINE uint32_t Atomic_XOR_u32( uint32_t متلون * pulDestination,
                                                 uint32_t ul_قدر )
{
    uint32_t ulCurrent;

    ATOMIC_ENTER_CRITICAL();
    {
        ulCurrent = *pulDestination;
        *pulDestination ^= ul_قدر;
    }
    ATOMIC_EXIT_CRITICAL();

    واپس ulCurrent;
}

/* *INDENT-OFF* */
#اگر_تعریف_شدہ __cplusplus
    }
#اگر_ختم
/* *INDENT-ON* */

#اگر_ختم /* ATOMIC_H */
