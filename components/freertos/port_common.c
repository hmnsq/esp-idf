/*
 * SPDX-FileCopyrightText: 2021-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#شامل <string.h>
#شامل "FreeRTOS.h"
#شامل "task.h"
#شامل "esp_system.h"
#شامل "esp_memory_utils.h"
#شامل "sdkconfig.h"

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
#اگر ( config_SUPPORT_STATIC_تخصیص == 1 )
خالی v_Application_Get_Idle_Task_حافظہ(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   ڈھیر_Type_t **ppx_Idle_Task_ڈھیر_بفر,
                                   uint32_t *pul_Idle_Task_ڈھیر_سائز)
{
    StaticTask_t *px_TCBBuffer_عارضی;
    ڈھیر_Type_t *px_ڈھیر_بفر_عارضی;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#اگر (پورٹ_ڈھیر_GROWTH > 0)
    {
        px_TCBBuffer_عارضی = pv_پورٹ_Malloc(حجم(StaticTask_t));
        px_ڈھیر_بفر_عارضی = pv_پورٹ_Malloc(config_اقل_ڈھیر_سائز);
    }
#ورنہ /* portSTACK_GROWTH */
    {
        px_ڈھیر_بفر_عارضی = pv_پورٹ_Malloc(config_اقل_ڈھیر_سائز);
        px_TCBBuffer_عارضی = pv_پورٹ_Malloc(حجم(StaticTask_t));
    }
#اگر_ختم /* portSTACK_GROWTH */

    أكد(px_TCBBuffer_عارضی != تہی);
    أكد(px_ڈھیر_بفر_عارضی != تہی);
    //Write back pointers
    *ppxIdleTaskTCBBuffer = px_TCBBuffer_عارضی;
    *ppx_Idle_Task_ڈھیر_بفر = px_ڈھیر_بفر_عارضی;
    *pul_Idle_Task_ڈھیر_سائز = config_اقل_ڈھیر_سائز;
}

#اگر ( ( CONFIG_FREERTOS_SMP ) && ( config_نمبر_کا_CORES > 1 ) )
خالی v_Application_Get_Passive_Idle_Task_حافظہ(StaticTask_t ** ppxIdleTaskTCBBuffer,
                                          ڈھیر_Type_t ** ppx_Idle_Task_ڈھیر_بفر,
                                          uint32_t * pul_Idle_Task_ڈھیر_سائز,
                                          بنیاد_Type_t x_Passive_Idle_Task_اشاریہ)
{
    v_Application_Get_Idle_Task_حافظہ(ppxIdleTaskTCBBuffer, ppx_Idle_Task_ڈھیر_بفر, pul_Idle_Task_ڈھیر_سائز);
}
#اگر_ختم /* ( ( CONFIG_FREERTOS_SMP ) && ( configNUMBER_OF_CORES > 1 ) ) */

#اگر configUSE_TIMERS

خالی v_Application_Get_Timer_Task_حافظہ(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    ڈھیر_Type_t **ppx_Timer_Task_ڈھیر_بفر,
                                    uint32_t *pul_Timer_Task_ڈھیر_سائز)
{
    StaticTask_t *px_TCBBuffer_عارضی;
    ڈھیر_Type_t *px_ڈھیر_بفر_عارضی;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#اگر (پورٹ_ڈھیر_GROWTH > 0)
    {
        px_TCBBuffer_عارضی = pv_پورٹ_Malloc(حجم(StaticTask_t));
        px_ڈھیر_بفر_عارضی = pv_پورٹ_Malloc(config_TIMER_TASK_ڈھیر_گہرائی);
    }
#ورنہ /* portSTACK_GROWTH */
    {
        px_ڈھیر_بفر_عارضی = pv_پورٹ_Malloc(config_TIMER_TASK_ڈھیر_گہرائی);
        px_TCBBuffer_عارضی = pv_پورٹ_Malloc(حجم(StaticTask_t));
    }
#اگر_ختم /* portSTACK_GROWTH */

    أكد(px_TCBBuffer_عارضی != تہی);
    أكد(px_ڈھیر_بفر_عارضی != تہی);
    //Write back pointers
    *ppxTimerTaskTCBBuffer = px_TCBBuffer_عارضی;
    *ppx_Timer_Task_ڈھیر_بفر = px_ڈھیر_بفر_عارضی;
    *pul_Timer_Task_ڈھیر_سائز = config_TIMER_TASK_ڈھیر_گہرائی;
}
#اگر_ختم //configUSE_TIMERS

#اگر_ختم // configSUPPORT_STATIC_ALLOCATION == 1
