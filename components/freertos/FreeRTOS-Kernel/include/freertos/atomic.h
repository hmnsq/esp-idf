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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ATOMIC_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ATOMIC_H

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ INC_FREERTOS_H
    #ਗਲਤੀ "include FreeRTOS.h must appear in source files before include atomic.h"
#ਜੇ_ਅੰਤ

/* Standard includes. */
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>

/* *INDENT-OFF* */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
    ਬਾਹਰੀ "C" {
#ਜੇ_ਅੰਤ
/* *INDENT-ON* */

/*
 * Port specific definitions -- entering/exiting critical section.
 * Refer template -- ./lib/FreeRTOS/portable/Compiler/Arch/portmacro.h
 *
 * Every call to ATOMIC_EXIT_CRITICAL() must be closely paired with
 * ATOMIC_ENTER_CRITICAL().
 *
 */
#ਜੇ defined( ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR )

/* Nested interrupt scheme is supported in this port. */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ATOMIC_ENTER_CRITICAL() \
    UBaseType_t uxCriticalSectionType = ਪੋਰਟ_ਸੈੱਟ_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR()

    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ATOMIC_EXIT_CRITICAL() \
    ਪੋਰਟ_CLEAR_ਰੁਕਾਵਟ_ਨਕਾਬ_FROM_ISR( uxCriticalSectionType )

#ਨਹੀਂਤਾਂ

/* Nested interrupt scheme is NOT supported in this port. */
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ATOMIC_ENTER_CRITICAL()    ਪੋਰਟ_ENTER_CRITICAL()
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ATOMIC_EXIT_CRITICAL()     ਪੋਰਟ_EXIT_CRITICAL()

#ਜੇ_ਅੰਤ /* portSET_INTERRUPT_MASK_FROM_ISR() */

/*
 * Port specific definition -- "always inline".
 * Inline is compiler specific, and may not always get inlined depending on your
 * optimization level.  Also, inline is considered as performance optimization
 * for atomic.  Thus, if portFORCE_INLINE is not provided by portmacro.h,
 * instead of resulting error, simply define it away.
 */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_FORCE_INLINE
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪੋਰਟ_FORCE_INLINE
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ATOMIC_COMPARE_AND_ਅਦਲਾ_ਬਦਲੀ_SUCCESS    0x1U     /**< Compare and swap succeeded, swapped. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ATOMIC_COMPARE_AND_ਅਦਲਾ_ਬਦਲੀ_FAILURE    0x0U     /**< Compare and swap failed, did not swap. */

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
ਸਥਿਰ ਪੋਰਟ_FORCE_INLINE uint32_t Atomic_Compare_And_ਅਦਲਾ_ਬਦਲੀ_u32( uint32_t ਅਸਥਿਰ * pulDestination,
                                                            uint32_t ulExchange,
                                                            uint32_t ulComparand )
{
    uint32_t ul_Return_ਮੁੱਲ;

    ATOMIC_ENTER_CRITICAL();
    {
        ਜੇ( *pulDestination == ulComparand )
        {
            *pulDestination = ulExchange;
            ul_Return_ਮੁੱਲ = ATOMIC_COMPARE_AND_ਅਦਲਾ_ਬਦਲੀ_SUCCESS;
        }
        ਨਹੀਂਤਾਂ
        {
            ul_Return_ਮੁੱਲ = ATOMIC_COMPARE_AND_ਅਦਲਾ_ਬਦਲੀ_FAILURE;
        }
    }
    ATOMIC_EXIT_CRITICAL();

    ਵਾਪਸ ul_Return_ਮੁੱਲ;
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
ਸਥਿਰ ਪੋਰਟ_FORCE_INLINE ਖਾਲੀ * Atomic_ਅਦਲਾ_ਬਦਲੀ_ਸੂਚਕ_ਸਾਰੇ_p32( ਖਾਲੀ * ਅਸਥਿਰ * ppvDestination,
                                                        ਖਾਲੀ * pvExchange )
{
    ਖਾਲੀ * p_Return_ਮੁੱਲ;

    ATOMIC_ENTER_CRITICAL();
    {
        p_Return_ਮੁੱਲ = *ppvDestination;
        *ppvDestination = pvExchange;
    }
    ATOMIC_EXIT_CRITICAL();

    ਵਾਪਸ p_Return_ਮੁੱਲ;
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
ਸਥਿਰ ਪੋਰਟ_FORCE_INLINE uint32_t Atomic_Compare_And_ਅਦਲਾ_ਬਦਲੀ_ਸੂਚਕ_ਸਾਰੇ_p32( ਖਾਲੀ * ਅਸਥਿਰ * ppvDestination,
                                                                    ਖਾਲੀ * pvExchange,
                                                                    ਖਾਲੀ * pvComparand )
{
    uint32_t ul_Return_ਮੁੱਲ = ATOMIC_COMPARE_AND_ਅਦਲਾ_ਬਦਲੀ_FAILURE;

    ATOMIC_ENTER_CRITICAL();
    {
        ਜੇ( *ppvDestination == pvComparand )
        {
            *ppvDestination = pvExchange;
            ul_Return_ਮੁੱਲ = ATOMIC_COMPARE_AND_ਅਦਲਾ_ਬਦਲੀ_SUCCESS;
        }
    }
    ATOMIC_EXIT_CRITICAL();

    ਵਾਪਸ ul_Return_ਮੁੱਲ;
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
ਸਥਿਰ ਪੋਰਟ_FORCE_INLINE uint32_t Atomic_ਜੋੜੋ_u32( uint32_t ਅਸਥਿਰ * pulAddend,
                                                 uint32_t ul_ਗਿਣਤੀ )
{
    uint32_t ulCurrent;

    ATOMIC_ENTER_CRITICAL();
    {
        ulCurrent = *pulAddend;
        *pulAddend += ul_ਗਿਣਤੀ;
    }
    ATOMIC_EXIT_CRITICAL();

    ਵਾਪਸ ulCurrent;
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
ਸਥਿਰ ਪੋਰਟ_FORCE_INLINE uint32_t Atomic_ਘਟਾਓ_u32( uint32_t ਅਸਥਿਰ * pulAddend,
                                                      uint32_t ul_ਗਿਣਤੀ )
{
    uint32_t ulCurrent;

    ATOMIC_ENTER_CRITICAL();
    {
        ulCurrent = *pulAddend;
        *pulAddend -= ul_ਗਿਣਤੀ;
    }
    ATOMIC_EXIT_CRITICAL();

    ਵਾਪਸ ulCurrent;
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
ਸਥਿਰ ਪੋਰਟ_FORCE_INLINE uint32_t Atomic_Increment_u32( uint32_t ਅਸਥਿਰ * pulAddend )
{
    uint32_t ulCurrent;

    ATOMIC_ENTER_CRITICAL();
    {
        ulCurrent = *pulAddend;
        *pulAddend += 1;
    }
    ATOMIC_EXIT_CRITICAL();

    ਵਾਪਸ ulCurrent;
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
ਸਥਿਰ ਪੋਰਟ_FORCE_INLINE uint32_t Atomic_Decrement_u32( uint32_t ਅਸਥਿਰ * pulAddend )
{
    uint32_t ulCurrent;

    ATOMIC_ENTER_CRITICAL();
    {
        ulCurrent = *pulAddend;
        *pulAddend -= 1;
    }
    ATOMIC_EXIT_CRITICAL();

    ਵਾਪਸ ulCurrent;
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
ਸਥਿਰ ਪੋਰਟ_FORCE_INLINE uint32_t Atomic_OR_u32( uint32_t ਅਸਥਿਰ * pulDestination,
                                                uint32_t ul_ਮੁੱਲ )
{
    uint32_t ulCurrent;

    ATOMIC_ENTER_CRITICAL();
    {
        ulCurrent = *pulDestination;
        *pulDestination |= ul_ਮੁੱਲ;
    }
    ATOMIC_EXIT_CRITICAL();

    ਵਾਪਸ ulCurrent;
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
ਸਥਿਰ ਪੋਰਟ_FORCE_INLINE uint32_t Atomic_AND_u32( uint32_t ਅਸਥਿਰ * pulDestination,
                                                 uint32_t ul_ਮੁੱਲ )
{
    uint32_t ulCurrent;

    ATOMIC_ENTER_CRITICAL();
    {
        ulCurrent = *pulDestination;
        *pulDestination &= ul_ਮੁੱਲ;
    }
    ATOMIC_EXIT_CRITICAL();

    ਵਾਪਸ ulCurrent;
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
ਸਥਿਰ ਪੋਰਟ_FORCE_INLINE uint32_t Atomic_NAND_u32( uint32_t ਅਸਥਿਰ * pulDestination,
                                                  uint32_t ul_ਮੁੱਲ )
{
    uint32_t ulCurrent;

    ATOMIC_ENTER_CRITICAL();
    {
        ulCurrent = *pulDestination;
        *pulDestination = ~( ulCurrent & ul_ਮੁੱਲ );
    }
    ATOMIC_EXIT_CRITICAL();

    ਵਾਪਸ ulCurrent;
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
ਸਥਿਰ ਪੋਰਟ_FORCE_INLINE uint32_t Atomic_XOR_u32( uint32_t ਅਸਥਿਰ * pulDestination,
                                                 uint32_t ul_ਮੁੱਲ )
{
    uint32_t ulCurrent;

    ATOMIC_ENTER_CRITICAL();
    {
        ulCurrent = *pulDestination;
        *pulDestination ^= ul_ਮੁੱਲ;
    }
    ATOMIC_EXIT_CRITICAL();

    ਵਾਪਸ ulCurrent;
}

/* *INDENT-OFF* */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
    }
#ਜੇ_ਅੰਤ
/* *INDENT-ON* */

#ਜੇ_ਅੰਤ /* ATOMIC_H */
