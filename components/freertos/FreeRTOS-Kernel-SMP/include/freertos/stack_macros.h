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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ರಾಶಿ_MACROS_H
#ವ್ಯಾಖ್ಯಾನಿಸು ರಾಶಿ_MACROS_H

/*
 * Call the stack overflow hook function if the stack of the task being swapped
 * out is currently overflowed, or looks like it might have overflowed in the
 * past.
 *
 * Setting configCHECK_FOR_STACK_OVERFLOW to 1 will cause the macro to check
 * the current stack state only - comparing the current top of stack value to
 * the stack limit.  Setting configCHECK_FOR_STACK_OVERFLOW to greater than 1
 * will also cause the last few stack bytes to be checked to ensure the value
 * to which the bytes were set when the task was created have not been
 * overwritten.  Note this second test does not guarantee that an overflowed
 * stack will always be recognised.
 */

/*-----------------------------------------------------------*/

/*
 * portSTACK_LIMIT_PADDING is a number of extra words to consider to be in
 * use on the stack.
 */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಪೋರ್ಟ್_ರಾಶಿ_ಎಲ್ಲೆ_PADDING
    #ವ್ಯಾಖ್ಯಾನಿಸು ಪೋರ್ಟ್_ರಾಶಿ_ಎಲ್ಲೆ_PADDING    0
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ ( ( config_CHECK_FOR_ರಾಶಿ_ಅತಿಪ್ರವಾಹ == 1 ) && ( ಪೋರ್ಟ್_ರಾಶಿ_GROWTH < 0 ) )

/* Only the current stack state is to be checked. */
    #ವ್ಯಾಖ್ಯಾನಿಸು task_CHECK_FOR_ರಾಶಿ_ಅತಿಪ್ರವಾಹ()                                                      \
    ಮಾಡು {                                                                                        \
        /* Is the currently saved stack pointer within the stack limit? */                      \
        ಆದರೆ( pxCurrentTCB->px_ತುದಿ_ದ_ರಾಶಿ <= pxCurrentTCB->px_ರಾಶಿ + ಪೋರ್ಟ್_ರಾಶಿ_ಎಲ್ಲೆ_PADDING )     \
        {                                                                                       \
            ಅಕ್ಷರ * pc_ಅತಿಪ್ರವಾಹ_Task_Name = pxCurrentTCB->pcTaskName;                               \
            v_Application_ರಾಶಿ_ಅತಿಪ್ರವಾಹ_Hook( ( TaskHandle_t ) pxCurrentTCB, pc_ಅತಿಪ್ರವಾಹ_Task_Name ); \
        }                                                                                       \
    } ಆಗ( 0 )

#ಆದರೆ_ಅಂತ್ಯ /* configCHECK_FOR_STACK_OVERFLOW == 1 */
/*-----------------------------------------------------------*/

#ಆದರೆ ( ( config_CHECK_FOR_ರಾಶಿ_ಅತಿಪ್ರವಾಹ == 1 ) && ( ಪೋರ್ಟ್_ರಾಶಿ_GROWTH > 0 ) )

/* Only the current stack state is to be checked. */
    #ವ್ಯಾಖ್ಯಾನಿಸು task_CHECK_FOR_ರಾಶಿ_ಅತಿಪ್ರವಾಹ()                                                       \
    ಮಾಡು {                                                                                         \
                                                                                                 \
        /* Is the currently saved stack pointer within the stack limit? */                       \
        ಆದರೆ( pxCurrentTCB->px_ತುದಿ_ದ_ರಾಶಿ >= pxCurrentTCB->px_ಕೊನೆ_ದ_ರಾಶಿ - ಪೋರ್ಟ್_ರಾಶಿ_ಎಲ್ಲೆ_PADDING ) \
        {                                                                                        \
            ಅಕ್ಷರ * pc_ಅತಿಪ್ರವಾಹ_Task_Name = pxCurrentTCB->pcTaskName;                                \
            v_Application_ರಾಶಿ_ಅತಿಪ್ರವಾಹ_Hook( ( TaskHandle_t ) pxCurrentTCB, pc_ಅತಿಪ್ರವಾಹ_Task_Name );  \
        }                                                                                        \
    } ಆಗ( 0 )

#ಆದರೆ_ಅಂತ್ಯ /* configCHECK_FOR_STACK_OVERFLOW == 1 */
/*-----------------------------------------------------------*/

#ಆದರೆ ( ( config_CHECK_FOR_ರಾಶಿ_ಅತಿಪ್ರವಾಹ > 1 ) && ( ಪೋರ್ಟ್_ರಾಶಿ_GROWTH < 0 ) )

    #ವ್ಯಾಖ್ಯಾನಿಸು task_CHECK_FOR_ರಾಶಿ_ಅತಿಪ್ರವಾಹ()                                                      \
    ಮಾಡು {                                                                                        \
        ಸ್ಥಿರಾಂಕ uint32_t * ಸ್ಥಿರಾಂಕ pul_ರಾಶಿ = ( uint32_t * ) pxCurrentTCB->px_ರಾಶಿ;                 \
        ಸ್ಥಿರಾಂಕ uint32_t ul_Check_ಮೌಲ್ಯ = ( uint32_t ) 0xa5a5a5a5U;                                 \
                                                                                                \
        ಆದರೆ( ( pul_ರಾಶಿ[ 0 ] != ul_Check_ಮೌಲ್ಯ ) ||                                                \
            ( pul_ರಾಶಿ[ 1 ] != ul_Check_ಮೌಲ್ಯ ) ||                                                \
            ( pul_ರಾಶಿ[ 2 ] != ul_Check_ಮೌಲ್ಯ ) ||                                                \
            ( pul_ರಾಶಿ[ 3 ] != ul_Check_ಮೌಲ್ಯ ) )                                                 \
        {                                                                                       \
            ಅಕ್ಷರ * pc_ಅತಿಪ್ರವಾಹ_Task_Name = pxCurrentTCB->pcTaskName;                               \
            v_Application_ರಾಶಿ_ಅತಿಪ್ರವಾಹ_Hook( ( TaskHandle_t ) pxCurrentTCB, pc_ಅತಿಪ್ರವಾಹ_Task_Name ); \
        }                                                                                       \
    } ಆಗ( 0 )

#ಆದರೆ_ಅಂತ್ಯ /* #if( configCHECK_FOR_STACK_OVERFLOW > 1 ) */
/*-----------------------------------------------------------*/

#ಆದರೆ ( ( config_CHECK_FOR_ರಾಶಿ_ಅತಿಪ್ರವಾಹ > 1 ) && ( ಪೋರ್ಟ್_ರಾಶಿ_GROWTH > 0 ) )

    #ವ್ಯಾಖ್ಯಾನಿಸು task_CHECK_FOR_ರಾಶಿ_ಅತಿಪ್ರವಾಹ()                                                                                                \
    ಮಾಡು {                                                                                                                                  \
        int8_t * pc_ಕೊನೆ_ದ_ರಾಶಿ = ( int8_t * ) pxCurrentTCB->px_ಕೊನೆ_ದ_ರಾಶಿ;                                                                  \
        ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ uint8_t uc_Expected_ರಾಶಿ_ಬೈಟುಗಳು[] = { tsk_ರಾಶಿ_FILL_BYTE, tsk_ರಾಶಿ_FILL_BYTE, tsk_ರಾಶಿ_FILL_BYTE, tsk_ರಾಶಿ_FILL_BYTE,   \
                                                        tsk_ರಾಶಿ_FILL_BYTE, tsk_ರಾಶಿ_FILL_BYTE, tsk_ರಾಶಿ_FILL_BYTE, tsk_ರಾಶಿ_FILL_BYTE,   \
                                                        tsk_ರಾಶಿ_FILL_BYTE, tsk_ರಾಶಿ_FILL_BYTE, tsk_ರಾಶಿ_FILL_BYTE, tsk_ರಾಶಿ_FILL_BYTE,   \
                                                        tsk_ರಾಶಿ_FILL_BYTE, tsk_ರಾಶಿ_FILL_BYTE, tsk_ರಾಶಿ_FILL_BYTE, tsk_ರಾಶಿ_FILL_BYTE,   \
                                                        tsk_ರಾಶಿ_FILL_BYTE, tsk_ರಾಶಿ_FILL_BYTE, tsk_ರಾಶಿ_FILL_BYTE, tsk_ರಾಶಿ_FILL_BYTE }; \
                                                                                                                                          \
                                                                                                                                          \
        pc_ಕೊನೆ_ದ_ರಾಶಿ -= ಗಾತ್ರ( uc_Expected_ರಾಶಿ_ಬೈಟುಗಳು );                                                                                   \
                                                                                                                                          \
        /* Has the extremity of the task stack ever been written over? */                                                                 \
        ಆದರೆ( memcmp( ( ಶೂನ್ಯ * ) pc_ಕೊನೆ_ದ_ರಾಶಿ, ( ಶೂನ್ಯ * ) uc_Expected_ರಾಶಿ_ಬೈಟುಗಳು, ಗಾತ್ರ( uc_Expected_ರಾಶಿ_ಬೈಟುಗಳು ) ) != 0 )                     \
        {                                                                                                                                 \
            ಅಕ್ಷರ * pc_ಅತಿಪ್ರವಾಹ_Task_Name = pxCurrentTCB->pcTaskName;                                                                         \
            v_Application_ರಾಶಿ_ಅತಿಪ್ರವಾಹ_Hook( ( TaskHandle_t ) pxCurrentTCB, pc_ಅತಿಪ್ರವಾಹ_Task_Name );                                           \
        }                                                                                                                                 \
    } ಆಗ( 0 )

#ಆದರೆ_ಅಂತ್ಯ /* #if( configCHECK_FOR_STACK_OVERFLOW > 1 ) */
/*-----------------------------------------------------------*/

/* Remove stack overflow macro if not being used. */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ task_CHECK_FOR_ರಾಶಿ_ಅತಿಪ್ರವಾಹ
    #ವ್ಯಾಖ್ಯಾನಿಸು task_CHECK_FOR_ರಾಶಿ_ಅತಿಪ್ರವಾಹ()
#ಆದರೆ_ಅಂತ್ಯ



#ಆದರೆ_ಅಂತ್ಯ /* STACK_MACROS_H */
