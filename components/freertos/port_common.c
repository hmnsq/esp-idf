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
#إذا ( config_SUPPORT_STATIC_تخصیص == 1 )
فراغ v_Application_Get_Idle_Task_حافظه(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   پشته_Type_t **ppx_Idle_Task_پشته_میانگیر,
                                   uint32_t *pul_Idle_Task_پشته_اندازه)
{
    StaticTask_t *px_TCBBuffer_موقت;
    پشته_Type_t *px_پشته_میانگیر_موقت;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#إذا (پورت_پشته_GROWTH > 0)
    {
        px_TCBBuffer_موقت = pv_پورت_Malloc(حجم(StaticTask_t));
        px_پشته_میانگیر_موقت = pv_پورت_Malloc(config_کمینه_پشته_اندازه);
    }
#وإلا /* portSTACK_GROWTH */
    {
        px_پشته_میانگیر_موقت = pv_پورت_Malloc(config_کمینه_پشته_اندازه);
        px_TCBBuffer_موقت = pv_پورت_Malloc(حجم(StaticTask_t));
    }
#پایان_اگر /* portSTACK_GROWTH */

    أكد(px_TCBBuffer_موقت != عدم);
    أكد(px_پشته_میانگیر_موقت != عدم);
    //Write back pointers
    *ppxIdleTaskTCBBuffer = px_TCBBuffer_موقت;
    *ppx_Idle_Task_پشته_میانگیر = px_پشته_میانگیر_موقت;
    *pul_Idle_Task_پشته_اندازه = config_کمینه_پشته_اندازه;
}

#إذا ( ( CONFIG_FREERTOS_SMP ) && ( config_عدد_از_CORES > 1 ) )
فراغ v_Application_Get_Passive_Idle_Task_حافظه(StaticTask_t ** ppxIdleTaskTCBBuffer,
                                          پشته_Type_t ** ppx_Idle_Task_پشته_میانگیر,
                                          uint32_t * pul_Idle_Task_پشته_اندازه,
                                          پایه_Type_t x_Passive_Idle_Task_شاخص)
{
    v_Application_Get_Idle_Task_حافظه(ppxIdleTaskTCBBuffer, ppx_Idle_Task_پشته_میانگیر, pul_Idle_Task_پشته_اندازه);
}
#پایان_اگر /* ( ( CONFIG_FREERTOS_SMP ) && ( configNUMBER_OF_CORES > 1 ) ) */

#إذا configUSE_TIMERS

فراغ v_Application_Get_Timer_Task_حافظه(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    پشته_Type_t **ppx_Timer_Task_پشته_میانگیر,
                                    uint32_t *pul_Timer_Task_پشته_اندازه)
{
    StaticTask_t *px_TCBBuffer_موقت;
    پشته_Type_t *px_پشته_میانگیر_موقت;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#إذا (پورت_پشته_GROWTH > 0)
    {
        px_TCBBuffer_موقت = pv_پورت_Malloc(حجم(StaticTask_t));
        px_پشته_میانگیر_موقت = pv_پورت_Malloc(config_TIMER_TASK_پشته_عمق);
    }
#وإلا /* portSTACK_GROWTH */
    {
        px_پشته_میانگیر_موقت = pv_پورت_Malloc(config_TIMER_TASK_پشته_عمق);
        px_TCBBuffer_موقت = pv_پورت_Malloc(حجم(StaticTask_t));
    }
#پایان_اگر /* portSTACK_GROWTH */

    أكد(px_TCBBuffer_موقت != عدم);
    أكد(px_پشته_میانگیر_موقت != عدم);
    //Write back pointers
    *ppxTimerTaskTCBBuffer = px_TCBBuffer_موقت;
    *ppx_Timer_Task_پشته_میانگیر = px_پشته_میانگیر_موقت;
    *pul_Timer_Task_پشته_اندازه = config_TIMER_TASK_پشته_عمق;
}
#پایان_اگر //configUSE_TIMERS

#پایان_اگر // configSUPPORT_STATIC_ALLOCATION == 1
