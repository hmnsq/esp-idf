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
#إذا ( config_SUPPORT_STATIC_ورهاست == 1 )
فراغ v_Application_Get_Idle_Task_يادگيري(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   ڍير_Type_t **ppx_Idle_Task_ڍير_بفر,
                                   uint32_t *pul_Idle_Task_ڍير_سائيز)
{
    StaticTask_t *px_TCBBuffer_عارضي;
    ڍير_Type_t *px_ڍير_بفر_عارضي;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#إذا (پورٽ_ڍير_GROWTH > 0)
    {
        px_TCBBuffer_عارضي = pv_پورٽ_Malloc(حجم(StaticTask_t));
        px_ڍير_بفر_عارضي = pv_پورٽ_Malloc(config_گهٽتر_ڍير_سائيز);
    }
#وإلا /* portSTACK_GROWTH */
    {
        px_ڍير_بفر_عارضي = pv_پورٽ_Malloc(config_گهٽتر_ڍير_سائيز);
        px_TCBBuffer_عارضي = pv_پورٽ_Malloc(حجم(StaticTask_t));
    }
#جيڪڏهن_پورو /* portSTACK_GROWTH */

    أكد(px_TCBBuffer_عارضي != عدم);
    أكد(px_ڍير_بفر_عارضي != عدم);
    //Write back pointers
    *ppxIdleTaskTCBBuffer = px_TCBBuffer_عارضي;
    *ppx_Idle_Task_ڍير_بفر = px_ڍير_بفر_عارضي;
    *pul_Idle_Task_ڍير_سائيز = config_گهٽتر_ڍير_سائيز;
}

#إذا ( ( CONFIG_FREERTOS_SMP ) && ( config_عدد_جو_CORES > 1 ) )
فراغ v_Application_Get_Passive_Idle_Task_يادگيري(StaticTask_t ** ppxIdleTaskTCBBuffer,
                                          ڍير_Type_t ** ppx_Idle_Task_ڍير_بفر,
                                          uint32_t * pul_Idle_Task_ڍير_سائيز,
                                          بنياد_Type_t x_Passive_Idle_Task_اشاريو)
{
    v_Application_Get_Idle_Task_يادگيري(ppxIdleTaskTCBBuffer, ppx_Idle_Task_ڍير_بفر, pul_Idle_Task_ڍير_سائيز);
}
#جيڪڏهن_پورو /* ( ( CONFIG_FREERTOS_SMP ) && ( configNUMBER_OF_CORES > 1 ) ) */

#إذا configUSE_TIMERS

فراغ v_Application_Get_Timer_Task_يادگيري(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    ڍير_Type_t **ppx_Timer_Task_ڍير_بفر,
                                    uint32_t *pul_Timer_Task_ڍير_سائيز)
{
    StaticTask_t *px_TCBBuffer_عارضي;
    ڍير_Type_t *px_ڍير_بفر_عارضي;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#إذا (پورٽ_ڍير_GROWTH > 0)
    {
        px_TCBBuffer_عارضي = pv_پورٽ_Malloc(حجم(StaticTask_t));
        px_ڍير_بفر_عارضي = pv_پورٽ_Malloc(config_TIMER_TASK_ڍير_اونهائي);
    }
#وإلا /* portSTACK_GROWTH */
    {
        px_ڍير_بفر_عارضي = pv_پورٽ_Malloc(config_TIMER_TASK_ڍير_اونهائي);
        px_TCBBuffer_عارضي = pv_پورٽ_Malloc(حجم(StaticTask_t));
    }
#جيڪڏهن_پورو /* portSTACK_GROWTH */

    أكد(px_TCBBuffer_عارضي != عدم);
    أكد(px_ڍير_بفر_عارضي != عدم);
    //Write back pointers
    *ppxTimerTaskTCBBuffer = px_TCBBuffer_عارضي;
    *ppx_Timer_Task_ڍير_بفر = px_ڍير_بفر_عارضي;
    *pul_Timer_Task_ڍير_سائيز = config_TIMER_TASK_ڍير_اونهائي;
}
#جيڪڏهن_پورو //configUSE_TIMERS

#جيڪڏهن_پورو // configSUPPORT_STATIC_ALLOCATION == 1
