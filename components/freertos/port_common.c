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
#اگر ( configSUPPORT_STATIC_ALLOCATION == 1 )
تهی vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   پشته_Type_t **ppx_Idle_Task_پشته_Buffer,
                                   uint32_t *pul_Idle_Task_پشته_Size)
{
    StaticTask_t *pxTCBBufferTemp;
    پشته_Type_t *px_پشته_Buffer_Temp;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#اگر (port_پشته_GROWTH > 0)
    {
        pxTCBBufferTemp = pvPortMalloc(اندازه(StaticTask_t));
        px_پشته_Buffer_Temp = pvPortMalloc(config_MINIMAL_پشته_SIZE);
    }
#وگرنه /* portSTACK_GROWTH */
    {
        px_پشته_Buffer_Temp = pvPortMalloc(config_MINIMAL_پشته_SIZE);
        pxTCBBufferTemp = pvPortMalloc(اندازه(StaticTask_t));
    }
#endif /* portSTACK_GROWTH */

    أكد(pxTCBBufferTemp != پوچ);
    أكد(px_پشته_Buffer_Temp != پوچ);
    //Write back pointers
    *ppxIdleTaskTCBBuffer = pxTCBBufferTemp;
    *ppx_Idle_Task_پشته_Buffer = px_پشته_Buffer_Temp;
    *pul_Idle_Task_پشته_Size = config_MINIMAL_پشته_SIZE;
}

#اگر ( ( CONFIG_FREERTOS_SMP ) && ( config_عدد_OF_CORES > 1 ) )
تهی vApplicationGetPassiveIdleTaskMemory(StaticTask_t ** ppxIdleTaskTCBBuffer,
                                          پشته_Type_t ** ppx_Idle_Task_پشته_Buffer,
                                          uint32_t * pul_Idle_Task_پشته_Size,
                                          BaseType_t xPassiveIdleTaskIndex)
{
    vApplicationGetIdleTaskMemory(ppxIdleTaskTCBBuffer, ppx_Idle_Task_پشته_Buffer, pul_Idle_Task_پشته_Size);
}
#endif /* ( ( CONFIG_FREERTOS_SMP ) && ( configNUMBER_OF_CORES > 1 ) ) */

#اگر configUSE_TIMERS

تهی vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    پشته_Type_t **ppx_Timer_Task_پشته_Buffer,
                                    uint32_t *pul_Timer_Task_پشته_Size)
{
    StaticTask_t *pxTCBBufferTemp;
    پشته_Type_t *px_پشته_Buffer_Temp;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#اگر (port_پشته_GROWTH > 0)
    {
        pxTCBBufferTemp = pvPortMalloc(اندازه(StaticTask_t));
        px_پشته_Buffer_Temp = pvPortMalloc(config_TIMER_TASK_پشته_DEPTH);
    }
#وگرنه /* portSTACK_GROWTH */
    {
        px_پشته_Buffer_Temp = pvPortMalloc(config_TIMER_TASK_پشته_DEPTH);
        pxTCBBufferTemp = pvPortMalloc(اندازه(StaticTask_t));
    }
#endif /* portSTACK_GROWTH */

    أكد(pxTCBBufferTemp != پوچ);
    أكد(px_پشته_Buffer_Temp != پوچ);
    //Write back pointers
    *ppxTimerTaskTCBBuffer = pxTCBBufferTemp;
    *ppx_Timer_Task_پشته_Buffer = px_پشته_Buffer_Temp;
    *pul_Timer_Task_پشته_Size = config_TIMER_TASK_پشته_DEPTH;
}
#endif //configUSE_TIMERS

#endif // configSUPPORT_STATIC_ALLOCATION == 1
