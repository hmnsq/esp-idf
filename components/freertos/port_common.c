/*
 * SPDX-FileCopyrightText: 2021-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "FreeRTOS.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "task.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_memory_utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"

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
#ਜੇ ( config_SUPPORT_STATIC_ਵੰਡ == 1 )
ਖਾਲੀ v_Application_Get_Idle_Task_ਸਮ੍ਰਿਤੀ(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   ਢੇਰ_Type_t **ppx_Idle_Task_ਢੇਰ_ਬਫਰ,
                                   uint32_t *pul_Idle_Task_ਢੇਰ_ਮਾਪ)
{
    StaticTask_t *px_TCBBuffer_ਆਰਜ਼ੀ;
    ਢੇਰ_Type_t *px_ਢੇਰ_ਬਫਰ_ਆਰਜ਼ੀ;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#ਜੇ (ਪੋਰਟ_ਢੇਰ_GROWTH > 0)
    {
        px_TCBBuffer_ਆਰਜ਼ੀ = pv_ਪੋਰਟ_Malloc(ਆਕਾਰ(StaticTask_t));
        px_ਢੇਰ_ਬਫਰ_ਆਰਜ਼ੀ = pv_ਪੋਰਟ_Malloc(config_ਨਿਊਨਤਮ_ਢੇਰ_ਮਾਪ);
    }
#ਨਹੀਂਤਾਂ /* portSTACK_GROWTH */
    {
        px_ਢੇਰ_ਬਫਰ_ਆਰਜ਼ੀ = pv_ਪੋਰਟ_Malloc(config_ਨਿਊਨਤਮ_ਢੇਰ_ਮਾਪ);
        px_TCBBuffer_ਆਰਜ਼ੀ = pv_ਪੋਰਟ_Malloc(ਆਕਾਰ(StaticTask_t));
    }
#ਜੇ_ਅੰਤ /* portSTACK_GROWTH */

    ਪ੍ਰਤਿਜ੍ਞਾ(px_TCBBuffer_ਆਰਜ਼ੀ != ਨਿਰਾ);
    ਪ੍ਰਤਿਜ੍ਞਾ(px_ਢੇਰ_ਬਫਰ_ਆਰਜ਼ੀ != ਨਿਰਾ);
    //Write back pointers
    *ppxIdleTaskTCBBuffer = px_TCBBuffer_ਆਰਜ਼ੀ;
    *ppx_Idle_Task_ਢੇਰ_ਬਫਰ = px_ਢੇਰ_ਬਫਰ_ਆਰਜ਼ੀ;
    *pul_Idle_Task_ਢੇਰ_ਮਾਪ = config_ਨਿਊਨਤਮ_ਢੇਰ_ਮਾਪ;
}

#ਜੇ ( ( CONFIG_FREERTOS_SMP ) && ( config_ਸੰਖਿਆ_ਦਾ_CORES > 1 ) )
ਖਾਲੀ v_Application_Get_Passive_Idle_Task_ਸਮ੍ਰਿਤੀ(StaticTask_t ** ppxIdleTaskTCBBuffer,
                                          ਢੇਰ_Type_t ** ppx_Idle_Task_ਢੇਰ_ਬਫਰ,
                                          uint32_t * pul_Idle_Task_ਢੇਰ_ਮਾਪ,
                                          ਅਧਾਰ_Type_t x_Passive_Idle_Task_ਸੂਚਕਾਂਕ)
{
    v_Application_Get_Idle_Task_ਸਮ੍ਰਿਤੀ(ppxIdleTaskTCBBuffer, ppx_Idle_Task_ਢੇਰ_ਬਫਰ, pul_Idle_Task_ਢੇਰ_ਮਾਪ);
}
#ਜੇ_ਅੰਤ /* ( ( CONFIG_FREERTOS_SMP ) && ( configNUMBER_OF_CORES > 1 ) ) */

#ਜੇ configUSE_TIMERS

ਖਾਲੀ v_Application_Get_Timer_Task_ਸਮ੍ਰਿਤੀ(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    ਢੇਰ_Type_t **ppx_Timer_Task_ਢੇਰ_ਬਫਰ,
                                    uint32_t *pul_Timer_Task_ਢੇਰ_ਮਾਪ)
{
    StaticTask_t *px_TCBBuffer_ਆਰਜ਼ੀ;
    ਢੇਰ_Type_t *px_ਢੇਰ_ਬਫਰ_ਆਰਜ਼ੀ;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#ਜੇ (ਪੋਰਟ_ਢੇਰ_GROWTH > 0)
    {
        px_TCBBuffer_ਆਰਜ਼ੀ = pv_ਪੋਰਟ_Malloc(ਆਕਾਰ(StaticTask_t));
        px_ਢੇਰ_ਬਫਰ_ਆਰਜ਼ੀ = pv_ਪੋਰਟ_Malloc(config_TIMER_TASK_ਢੇਰ_ਡੂੰਘਾਈ);
    }
#ਨਹੀਂਤਾਂ /* portSTACK_GROWTH */
    {
        px_ਢੇਰ_ਬਫਰ_ਆਰਜ਼ੀ = pv_ਪੋਰਟ_Malloc(config_TIMER_TASK_ਢੇਰ_ਡੂੰਘਾਈ);
        px_TCBBuffer_ਆਰਜ਼ੀ = pv_ਪੋਰਟ_Malloc(ਆਕਾਰ(StaticTask_t));
    }
#ਜੇ_ਅੰਤ /* portSTACK_GROWTH */

    ਪ੍ਰਤਿਜ੍ਞਾ(px_TCBBuffer_ਆਰਜ਼ੀ != ਨਿਰਾ);
    ਪ੍ਰਤਿਜ੍ਞਾ(px_ਢੇਰ_ਬਫਰ_ਆਰਜ਼ੀ != ਨਿਰਾ);
    //Write back pointers
    *ppxTimerTaskTCBBuffer = px_TCBBuffer_ਆਰਜ਼ੀ;
    *ppx_Timer_Task_ਢੇਰ_ਬਫਰ = px_ਢੇਰ_ਬਫਰ_ਆਰਜ਼ੀ;
    *pul_Timer_Task_ਢੇਰ_ਮਾਪ = config_TIMER_TASK_ਢੇਰ_ਡੂੰਘਾਈ;
}
#ਜੇ_ਅੰਤ //configUSE_TIMERS

#ਜੇ_ਅੰਤ // configSUPPORT_STATIC_ALLOCATION == 1
