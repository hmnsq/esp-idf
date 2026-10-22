/*
 * SPDX-FileCopyrightText: 2021-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ "FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "task.h"
#ಅನ್ತರ್ಭಾವ "esp_system.h"
#ಅನ್ತರ್ಭಾವ "esp_memory_utils.h"
#ಅನ್ತರ್ಭಾವ "sdkconfig.h"

/* ----------------------------------------- Port Implementations (Common)  --------------------------------------------
 * - Common FreeRTOS port function implementations
 * - These functions are common to all FreeRTOS ports (i.e., on all architectures and all FreeRTOS implementations).
 * ------------------------------------------------------------------------------------------------------------------ */

// ------------- FreeRTOS Static Allocation ----------------

/*
These function are required by FreeRTOS when configSUPPORT_STATIC_ALLOCATION is
enabled and is used by FreeRTOS to obtain memory for its IDLE/Timer tasks.

We simply allocate the IDLE/Timer tasks memory from the FreeRTOS heap.
*/
#ಆದರೆ ( config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 1 )
ಶೂನ್ಯ v_Application_Get_Idle_Task_ಸ್ಮರಣೆ(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   ರಾಶಿ_Type_t **ppx_Idle_Task_ರಾಶಿ_ಬಫರ್,
                                   uint32_t *pul_Idle_Task_ರಾಶಿ_ಗಾತ್ರ)
{
    StaticTask_t *px_TCBBuffer_ತಾತ್ಕಾಲಿಕ;
    ರಾಶಿ_Type_t *px_ರಾಶಿ_ಬಫರ್_ತಾತ್ಕಾಲಿಕ;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#ಆದರೆ (ಪೋರ್ಟ್_ರಾಶಿ_GROWTH > 0)
    {
        px_TCBBuffer_ತಾತ್ಕಾಲಿಕ = pv_ಪೋರ್ಟ್_Malloc(ಗಾತ್ರ(StaticTask_t));
        px_ರಾಶಿ_ಬಫರ್_ತಾತ್ಕಾಲಿಕ = pv_ಪೋರ್ಟ್_Malloc(config_ಕನಿಷ್ಠತಮ_ರಾಶಿ_ಗಾತ್ರ);
    }
#ಇಲ್ಲದಿದ್ದರೆ /* portSTACK_GROWTH */
    {
        px_ರಾಶಿ_ಬಫರ್_ತಾತ್ಕಾಲಿಕ = pv_ಪೋರ್ಟ್_Malloc(config_ಕನಿಷ್ಠತಮ_ರಾಶಿ_ಗಾತ್ರ);
        px_TCBBuffer_ತಾತ್ಕಾಲಿಕ = pv_ಪೋರ್ಟ್_Malloc(ಗಾತ್ರ(StaticTask_t));
    }
#ಆದರೆ_ಅಂತ್ಯ /* portSTACK_GROWTH */

    ಪ್ರತಿಜ್ಞಾ(px_TCBBuffer_ತಾತ್ಕಾಲಿಕ != ಖಾಲಿ);
    ಪ್ರತಿಜ್ಞಾ(px_ರಾಶಿ_ಬಫರ್_ತಾತ್ಕಾಲಿಕ != ಖಾಲಿ);
    //Write back pointers
    *ppxIdleTaskTCBBuffer = px_TCBBuffer_ತಾತ್ಕಾಲಿಕ;
    *ppx_Idle_Task_ರಾಶಿ_ಬಫರ್ = px_ರಾಶಿ_ಬಫರ್_ತಾತ್ಕಾಲಿಕ;
    *pul_Idle_Task_ರಾಶಿ_ಗಾತ್ರ = config_ಕನಿಷ್ಠತಮ_ರಾಶಿ_ಗಾತ್ರ;
}

#ಆದರೆ ( ( CONFIG_FREERTOS_SMP ) && ( config_ಸಂಖ್ಯೆ_ದ_CORES > 1 ) )
ಶೂನ್ಯ v_Application_Get_Passive_Idle_Task_ಸ್ಮರಣೆ(StaticTask_t ** ppxIdleTaskTCBBuffer,
                                          ರಾಶಿ_Type_t ** ppx_Idle_Task_ರಾಶಿ_ಬಫರ್,
                                          uint32_t * pul_Idle_Task_ರಾಶಿ_ಗಾತ್ರ,
                                          ತಳ_Type_t x_Passive_Idle_Task_ಸೂಚ್ಯಂಕ)
{
    v_Application_Get_Idle_Task_ಸ್ಮರಣೆ(ppxIdleTaskTCBBuffer, ppx_Idle_Task_ರಾಶಿ_ಬಫರ್, pul_Idle_Task_ರಾಶಿ_ಗಾತ್ರ);
}
#ಆದರೆ_ಅಂತ್ಯ /* ( ( CONFIG_FREERTOS_SMP ) && ( configNUMBER_OF_CORES > 1 ) ) */

#ಆದರೆ configUSE_TIMERS

ಶೂನ್ಯ v_Application_Get_Timer_Task_ಸ್ಮರಣೆ(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    ರಾಶಿ_Type_t **ppx_Timer_Task_ರಾಶಿ_ಬಫರ್,
                                    uint32_t *pul_Timer_Task_ರಾಶಿ_ಗಾತ್ರ)
{
    StaticTask_t *px_TCBBuffer_ತಾತ್ಕಾಲಿಕ;
    ರಾಶಿ_Type_t *px_ರಾಶಿ_ಬಫರ್_ತಾತ್ಕಾಲಿಕ;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#ಆದರೆ (ಪೋರ್ಟ್_ರಾಶಿ_GROWTH > 0)
    {
        px_TCBBuffer_ತಾತ್ಕಾಲಿಕ = pv_ಪೋರ್ಟ್_Malloc(ಗಾತ್ರ(StaticTask_t));
        px_ರಾಶಿ_ಬಫರ್_ತಾತ್ಕಾಲಿಕ = pv_ಪೋರ್ಟ್_Malloc(config_TIMER_TASK_ರಾಶಿ_ಆಳ);
    }
#ಇಲ್ಲದಿದ್ದರೆ /* portSTACK_GROWTH */
    {
        px_ರಾಶಿ_ಬಫರ್_ತಾತ್ಕಾಲಿಕ = pv_ಪೋರ್ಟ್_Malloc(config_TIMER_TASK_ರಾಶಿ_ಆಳ);
        px_TCBBuffer_ತಾತ್ಕಾಲಿಕ = pv_ಪೋರ್ಟ್_Malloc(ಗಾತ್ರ(StaticTask_t));
    }
#ಆದರೆ_ಅಂತ್ಯ /* portSTACK_GROWTH */

    ಪ್ರತಿಜ್ಞಾ(px_TCBBuffer_ತಾತ್ಕಾಲಿಕ != ಖಾಲಿ);
    ಪ್ರತಿಜ್ಞಾ(px_ರಾಶಿ_ಬಫರ್_ತಾತ್ಕಾಲಿಕ != ಖಾಲಿ);
    //Write back pointers
    *ppxTimerTaskTCBBuffer = px_TCBBuffer_ತಾತ್ಕಾಲಿಕ;
    *ppx_Timer_Task_ರಾಶಿ_ಬಫರ್ = px_ರಾಶಿ_ಬಫರ್_ತಾತ್ಕಾಲಿಕ;
    *pul_Timer_Task_ರಾಶಿ_ಗಾತ್ರ = config_TIMER_TASK_ರಾಶಿ_ಆಳ;
}
#ಆದರೆ_ಅಂತ್ಯ //configUSE_TIMERS

#ಆದರೆ_ಅಂತ್ಯ // configSUPPORT_STATIC_ALLOCATION == 1
