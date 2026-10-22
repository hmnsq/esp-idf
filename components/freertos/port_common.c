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
#إذا ( config_SUPPORT_STATIC_ونڈ == 1 )
فراغ v_Application_Get_Idle_Task_حافظہ(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   ڈھیر_Type_t **ppx_Idle_Task_ڈھیر_بفر,
                                   uint32_t *pul_Idle_Task_ڈھیر_ناپ)
{
    StaticTask_t *px_TCBBuffer_عارضی;
    ڈھیر_Type_t *px_ڈھیر_بفر_عارضی;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#إذا (پورٹ_ڈھیر_GROWTH > 0)
    {
        px_TCBBuffer_عارضی = pv_پورٹ_Malloc(حجم(StaticTask_t));
        px_ڈھیر_بفر_عارضی = pv_پورٹ_Malloc(config_اقل_ڈھیر_ناپ);
    }
#وإلا /* portSTACK_GROWTH */
    {
        px_ڈھیر_بفر_عارضی = pv_پورٹ_Malloc(config_اقل_ڈھیر_ناپ);
        px_TCBBuffer_عارضی = pv_پورٹ_Malloc(حجم(StaticTask_t));
    }
#جے_انت /* portSTACK_GROWTH */

    أكد(px_TCBBuffer_عارضی != عدم);
    أكد(px_ڈھیر_بفر_عارضی != عدم);
    //Write back pointers
    *ppxIdleTaskTCBBuffer = px_TCBBuffer_عارضی;
    *ppx_Idle_Task_ڈھیر_بفر = px_ڈھیر_بفر_عارضی;
    *pul_Idle_Task_ڈھیر_ناپ = config_اقل_ڈھیر_ناپ;
}

#إذا ( ( CONFIG_FREERTOS_SMP ) && ( config_نمبر_دا_CORES > 1 ) )
فراغ v_Application_Get_Passive_Idle_Task_حافظہ(StaticTask_t ** ppxIdleTaskTCBBuffer,
                                          ڈھیر_Type_t ** ppx_Idle_Task_ڈھیر_بفر,
                                          uint32_t * pul_Idle_Task_ڈھیر_ناپ,
                                          ادھار_Type_t x_Passive_Idle_Task_اشاریہ)
{
    v_Application_Get_Idle_Task_حافظہ(ppxIdleTaskTCBBuffer, ppx_Idle_Task_ڈھیر_بفر, pul_Idle_Task_ڈھیر_ناپ);
}
#جے_انت /* ( ( CONFIG_FREERTOS_SMP ) && ( configNUMBER_OF_CORES > 1 ) ) */

#إذا configUSE_TIMERS

فراغ v_Application_Get_Timer_Task_حافظہ(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    ڈھیر_Type_t **ppx_Timer_Task_ڈھیر_بفر,
                                    uint32_t *pul_Timer_Task_ڈھیر_ناپ)
{
    StaticTask_t *px_TCBBuffer_عارضی;
    ڈھیر_Type_t *px_ڈھیر_بفر_عارضی;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#إذا (پورٹ_ڈھیر_GROWTH > 0)
    {
        px_TCBBuffer_عارضی = pv_پورٹ_Malloc(حجم(StaticTask_t));
        px_ڈھیر_بفر_عارضی = pv_پورٹ_Malloc(config_TIMER_TASK_ڈھیر_ڈونگھائی);
    }
#وإلا /* portSTACK_GROWTH */
    {
        px_ڈھیر_بفر_عارضی = pv_پورٹ_Malloc(config_TIMER_TASK_ڈھیر_ڈونگھائی);
        px_TCBBuffer_عارضی = pv_پورٹ_Malloc(حجم(StaticTask_t));
    }
#جے_انت /* portSTACK_GROWTH */

    أكد(px_TCBBuffer_عارضی != عدم);
    أكد(px_ڈھیر_بفر_عارضی != عدم);
    //Write back pointers
    *ppxTimerTaskTCBBuffer = px_TCBBuffer_عارضی;
    *ppx_Timer_Task_ڈھیر_بفر = px_ڈھیر_بفر_عارضی;
    *pul_Timer_Task_ڈھیر_ناپ = config_TIMER_TASK_ڈھیر_ڈونگھائی;
}
#جے_انت //configUSE_TIMERS

#جے_انت // configSUPPORT_STATIC_ALLOCATION == 1
