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
#إذا ( configSUPPORT_STATIC_ALLOCATION == 1 )
فراغ vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   کەڵەکە_Type_t **ppx_Idle_Task_کەڵەکە_Buffer,
                                   uint32_t *pul_Idle_Task_کەڵەکە_Size)
{
    StaticTask_t *pxTCBBufferTemp;
    کەڵەکە_Type_t *px_کەڵەکە_Buffer_Temp;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#إذا (port_کەڵەکە_GROWTH > 0)
    {
        pxTCBBufferTemp = pvPortMalloc(حجم(StaticTask_t));
        px_کەڵەکە_Buffer_Temp = pvPortMalloc(config_MINIMAL_کەڵەکە_SIZE);
    }
#وإلا /* portSTACK_GROWTH */
    {
        px_کەڵەکە_Buffer_Temp = pvPortMalloc(config_MINIMAL_کەڵەکە_SIZE);
        pxTCBBufferTemp = pvPortMalloc(حجم(StaticTask_t));
    }
#endif /* portSTACK_GROWTH */

    أكد(pxTCBBufferTemp != عدم);
    أكد(px_کەڵەکە_Buffer_Temp != عدم);
    //Write back pointers
    *ppxIdleTaskTCBBuffer = pxTCBBufferTemp;
    *ppx_Idle_Task_کەڵەکە_Buffer = px_کەڵەکە_Buffer_Temp;
    *pul_Idle_Task_کەڵەکە_Size = config_MINIMAL_کەڵەکە_SIZE;
}

#إذا ( ( CONFIG_FREERTOS_SMP ) && ( config_ژمار_OF_CORES > 1 ) )
فراغ vApplicationGetPassiveIdleTaskMemory(StaticTask_t ** ppxIdleTaskTCBBuffer,
                                          کەڵەکە_Type_t ** ppx_Idle_Task_کەڵەکە_Buffer,
                                          uint32_t * pul_Idle_Task_کەڵەکە_Size,
                                          BaseType_t xPassiveIdleTaskIndex)
{
    vApplicationGetIdleTaskMemory(ppxIdleTaskTCBBuffer, ppx_Idle_Task_کەڵەکە_Buffer, pul_Idle_Task_کەڵەکە_Size);
}
#endif /* ( ( CONFIG_FREERTOS_SMP ) && ( configNUMBER_OF_CORES > 1 ) ) */

#إذا configUSE_TIMERS

فراغ vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    کەڵەکە_Type_t **ppx_Timer_Task_کەڵەکە_Buffer,
                                    uint32_t *pul_Timer_Task_کەڵەکە_Size)
{
    StaticTask_t *pxTCBBufferTemp;
    کەڵەکە_Type_t *px_کەڵەکە_Buffer_Temp;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#إذا (port_کەڵەکە_GROWTH > 0)
    {
        pxTCBBufferTemp = pvPortMalloc(حجم(StaticTask_t));
        px_کەڵەکە_Buffer_Temp = pvPortMalloc(config_TIMER_TASK_کەڵەکە_DEPTH);
    }
#وإلا /* portSTACK_GROWTH */
    {
        px_کەڵەکە_Buffer_Temp = pvPortMalloc(config_TIMER_TASK_کەڵەکە_DEPTH);
        pxTCBBufferTemp = pvPortMalloc(حجم(StaticTask_t));
    }
#endif /* portSTACK_GROWTH */

    أكد(pxTCBBufferTemp != عدم);
    أكد(px_کەڵەکە_Buffer_Temp != عدم);
    //Write back pointers
    *ppxTimerTaskTCBBuffer = pxTCBBufferTemp;
    *ppx_Timer_Task_کەڵەکە_Buffer = px_کەڵەکە_Buffer_Temp;
    *pul_Timer_Task_کەڵەکە_Size = config_TIMER_TASK_کەڵەکە_DEPTH;
}
#endif //configUSE_TIMERS

#endif // configSUPPORT_STATIC_ALLOCATION == 1
