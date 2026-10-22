/*
 * SPDX-FileCopyrightText: 2021-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن <string.h>
#ضمن "FreeRTOS.h"
#ضمن "task.h"
#ضمن "esp_system.h"
#ضمن "esp_memory_utils.h"
#ضمن "sdkconfig.h"

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
#إذا ( config_SUPPORT_STATIC_ویش == 1 )
فراغ v_Application_Get_Idle_Task_حافظه(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   ډېری_Type_t **ppx_Idle_Task_ډېری_بفر,
                                   uint32_t *pul_Idle_Task_ډېری_اندازه)
{
    StaticTask_t *px_TCBBuffer_لنډمهاله;
    ډېری_Type_t *px_ډېری_بفر_لنډمهاله;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#إذا (پورټ_ډېری_GROWTH > 0)
    {
        px_TCBBuffer_لنډمهاله = pv_پورټ_Malloc(حجم(StaticTask_t));
        px_ډېری_بفر_لنډمهاله = pv_پورټ_Malloc(config_اصغري_ډېری_اندازه);
    }
#وإلا /* portSTACK_GROWTH */
    {
        px_ډېری_بفر_لنډمهاله = pv_پورټ_Malloc(config_اصغري_ډېری_اندازه);
        px_TCBBuffer_لنډمهاله = pv_پورټ_Malloc(حجم(StaticTask_t));
    }
#د_که_پای /* portSTACK_GROWTH */

    أكد(px_TCBBuffer_لنډمهاله != عدم);
    أكد(px_ډېری_بفر_لنډمهاله != عدم);
    //Write back pointers
    *ppxIdleTaskTCBBuffer = px_TCBBuffer_لنډمهاله;
    *ppx_Idle_Task_ډېری_بفر = px_ډېری_بفر_لنډمهاله;
    *pul_Idle_Task_ډېری_اندازه = config_اصغري_ډېری_اندازه;
}

#إذا ( ( CONFIG_FREERTOS_SMP ) && ( config_شمېره_د_CORES > 1 ) )
فراغ v_Application_Get_Passive_Idle_Task_حافظه(StaticTask_t ** ppxIdleTaskTCBBuffer,
                                          ډېری_Type_t ** ppx_Idle_Task_ډېری_بفر,
                                          uint32_t * pul_Idle_Task_ډېری_اندازه,
                                          بنسټ_Type_t x_Passive_Idle_Task_شاخص)
{
    v_Application_Get_Idle_Task_حافظه(ppxIdleTaskTCBBuffer, ppx_Idle_Task_ډېری_بفر, pul_Idle_Task_ډېری_اندازه);
}
#د_که_پای /* ( ( CONFIG_FREERTOS_SMP ) && ( configNUMBER_OF_CORES > 1 ) ) */

#إذا configUSE_TIMERS

فراغ v_Application_Get_Timer_Task_حافظه(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    ډېری_Type_t **ppx_Timer_Task_ډېری_بفر,
                                    uint32_t *pul_Timer_Task_ډېری_اندازه)
{
    StaticTask_t *px_TCBBuffer_لنډمهاله;
    ډېری_Type_t *px_ډېری_بفر_لنډمهاله;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#إذا (پورټ_ډېری_GROWTH > 0)
    {
        px_TCBBuffer_لنډمهاله = pv_پورټ_Malloc(حجم(StaticTask_t));
        px_ډېری_بفر_لنډمهاله = pv_پورټ_Malloc(config_TIMER_TASK_ډېری_ژوروالی);
    }
#وإلا /* portSTACK_GROWTH */
    {
        px_ډېری_بفر_لنډمهاله = pv_پورټ_Malloc(config_TIMER_TASK_ډېری_ژوروالی);
        px_TCBBuffer_لنډمهاله = pv_پورټ_Malloc(حجم(StaticTask_t));
    }
#د_که_پای /* portSTACK_GROWTH */

    أكد(px_TCBBuffer_لنډمهاله != عدم);
    أكد(px_ډېری_بفر_لنډمهاله != عدم);
    //Write back pointers
    *ppxTimerTaskTCBBuffer = px_TCBBuffer_لنډمهاله;
    *ppx_Timer_Task_ډېری_بفر = px_ډېری_بفر_لنډمهاله;
    *pul_Timer_Task_ډېری_اندازه = config_TIMER_TASK_ډېری_ژوروالی;
}
#د_که_پای //configUSE_TIMERS

#د_که_پای // configSUPPORT_STATIC_ALLOCATION == 1
