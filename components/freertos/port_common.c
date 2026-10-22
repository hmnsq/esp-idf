/*
 * SPDX-FileCopyrightText: 2021-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ "FreeRTOS.h"
#അന്തര്ഭാവ "task.h"
#അന്തര്ഭാവ "esp_system.h"
#അന്തര്ഭാവ "esp_memory_utils.h"
#അന്തര്ഭാവ "sdkconfig.h"

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
#എങ്കിൽ ( configSUPPORT_STATIC_ALLOCATION == 1 )
ശൂന്യം vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   അട്ടി_Type_t **ppx_Idle_Task_അട്ടി_Buffer,
                                   uint32_t *pul_Idle_Task_അട്ടി_Size)
{
    StaticTask_t *pxTCBBufferTemp;
    അട്ടി_Type_t *px_അട്ടി_Buffer_Temp;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#എങ്കിൽ (port_അട്ടി_GROWTH > 0)
    {
        pxTCBBufferTemp = pvPortMalloc(വലുപ്പം(StaticTask_t));
        px_അട്ടി_Buffer_Temp = pvPortMalloc(config_MINIMAL_അട്ടി_SIZE);
    }
#അല്ലെങ്കിൽ /* portSTACK_GROWTH */
    {
        px_അട്ടി_Buffer_Temp = pvPortMalloc(config_MINIMAL_അട്ടി_SIZE);
        pxTCBBufferTemp = pvPortMalloc(വലുപ്പം(StaticTask_t));
    }
#endif /* portSTACK_GROWTH */

    പ്രതിജ്ഞാ(pxTCBBufferTemp != ശൂന്യ);
    പ്രതിജ്ഞാ(px_അട്ടി_Buffer_Temp != ശൂന്യ);
    //Write back pointers
    *ppxIdleTaskTCBBuffer = pxTCBBufferTemp;
    *ppx_Idle_Task_അട്ടി_Buffer = px_അട്ടി_Buffer_Temp;
    *pul_Idle_Task_അട്ടി_Size = config_MINIMAL_അട്ടി_SIZE;
}

#എങ്കിൽ ( ( CONFIG_FREERTOS_SMP ) && ( config_സംഖ്യ_OF_CORES > 1 ) )
ശൂന്യം vApplicationGetPassiveIdleTaskMemory(StaticTask_t ** ppxIdleTaskTCBBuffer,
                                          അട്ടി_Type_t ** ppx_Idle_Task_അട്ടി_Buffer,
                                          uint32_t * pul_Idle_Task_അട്ടി_Size,
                                          BaseType_t xPassiveIdleTaskIndex)
{
    vApplicationGetIdleTaskMemory(ppxIdleTaskTCBBuffer, ppx_Idle_Task_അട്ടി_Buffer, pul_Idle_Task_അട്ടി_Size);
}
#endif /* ( ( CONFIG_FREERTOS_SMP ) && ( configNUMBER_OF_CORES > 1 ) ) */

#എങ്കിൽ configUSE_TIMERS

ശൂന്യം vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    അട്ടി_Type_t **ppx_Timer_Task_അട്ടി_Buffer,
                                    uint32_t *pul_Timer_Task_അട്ടി_Size)
{
    StaticTask_t *pxTCBBufferTemp;
    അട്ടി_Type_t *px_അട്ടി_Buffer_Temp;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#എങ്കിൽ (port_അട്ടി_GROWTH > 0)
    {
        pxTCBBufferTemp = pvPortMalloc(വലുപ്പം(StaticTask_t));
        px_അട്ടി_Buffer_Temp = pvPortMalloc(config_TIMER_TASK_അട്ടി_DEPTH);
    }
#അല്ലെങ്കിൽ /* portSTACK_GROWTH */
    {
        px_അട്ടി_Buffer_Temp = pvPortMalloc(config_TIMER_TASK_അട്ടി_DEPTH);
        pxTCBBufferTemp = pvPortMalloc(വലുപ്പം(StaticTask_t));
    }
#endif /* portSTACK_GROWTH */

    പ്രതിജ്ഞാ(pxTCBBufferTemp != ശൂന്യ);
    പ്രതിജ്ഞാ(px_അട്ടി_Buffer_Temp != ശൂന്യ);
    //Write back pointers
    *ppxTimerTaskTCBBuffer = pxTCBBufferTemp;
    *ppx_Timer_Task_അട്ടി_Buffer = px_അട്ടി_Buffer_Temp;
    *pul_Timer_Task_അട്ടി_Size = config_TIMER_TASK_അട്ടി_DEPTH;
}
#endif //configUSE_TIMERS

#endif // configSUPPORT_STATIC_ALLOCATION == 1
