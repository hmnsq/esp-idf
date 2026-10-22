/*
 * SPDX-FileCopyrightText: 2021-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ "FreeRTOS.h"
#અન્તર્ભાવ "task.h"
#અન્તર્ભાવ "esp_system.h"
#અન્તર્ભાવ "esp_memory_utils.h"
#અન્તર્ભાવ "sdkconfig.h"

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
#જો ( configSUPPORT_STATIC_ALLOCATION == 1 )
શૂન્ય vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   ઢગલો_Type_t **ppx_Idle_Task_ઢગલો_Buffer,
                                   uint32_t *pul_Idle_Task_ઢગલો_Size)
{
    StaticTask_t *pxTCBBufferTemp;
    ઢગલો_Type_t *px_ઢગલો_Buffer_Temp;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#જો (port_ઢગલો_GROWTH > 0)
    {
        pxTCBBufferTemp = pvPortMalloc(કદ(StaticTask_t));
        px_ઢગલો_Buffer_Temp = pvPortMalloc(config_MINIMAL_ઢગલો_SIZE);
    }
#નહીંતર /* portSTACK_GROWTH */
    {
        px_ઢગલો_Buffer_Temp = pvPortMalloc(config_MINIMAL_ઢગલો_SIZE);
        pxTCBBufferTemp = pvPortMalloc(કદ(StaticTask_t));
    }
#endif /* portSTACK_GROWTH */

    પ્રતિજ્ઞા(pxTCBBufferTemp != ખાલી);
    પ્રતિજ્ઞા(px_ઢગલો_Buffer_Temp != ખાલી);
    //Write back pointers
    *ppxIdleTaskTCBBuffer = pxTCBBufferTemp;
    *ppx_Idle_Task_ઢગલો_Buffer = px_ઢગલો_Buffer_Temp;
    *pul_Idle_Task_ઢગલો_Size = config_MINIMAL_ઢગલો_SIZE;
}

#જો ( ( CONFIG_FREERTOS_SMP ) && ( config_સંખ્યા_OF_CORES > 1 ) )
શૂન્ય vApplicationGetPassiveIdleTaskMemory(StaticTask_t ** ppxIdleTaskTCBBuffer,
                                          ઢગલો_Type_t ** ppx_Idle_Task_ઢગલો_Buffer,
                                          uint32_t * pul_Idle_Task_ઢગલો_Size,
                                          BaseType_t xPassiveIdleTaskIndex)
{
    vApplicationGetIdleTaskMemory(ppxIdleTaskTCBBuffer, ppx_Idle_Task_ઢગલો_Buffer, pul_Idle_Task_ઢગલો_Size);
}
#endif /* ( ( CONFIG_FREERTOS_SMP ) && ( configNUMBER_OF_CORES > 1 ) ) */

#જો configUSE_TIMERS

શૂન્ય vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    ઢગલો_Type_t **ppx_Timer_Task_ઢગલો_Buffer,
                                    uint32_t *pul_Timer_Task_ઢગલો_Size)
{
    StaticTask_t *pxTCBBufferTemp;
    ઢગલો_Type_t *px_ઢગલો_Buffer_Temp;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#જો (port_ઢગલો_GROWTH > 0)
    {
        pxTCBBufferTemp = pvPortMalloc(કદ(StaticTask_t));
        px_ઢગલો_Buffer_Temp = pvPortMalloc(config_TIMER_TASK_ઢગલો_DEPTH);
    }
#નહીંતર /* portSTACK_GROWTH */
    {
        px_ઢગલો_Buffer_Temp = pvPortMalloc(config_TIMER_TASK_ઢગલો_DEPTH);
        pxTCBBufferTemp = pvPortMalloc(કદ(StaticTask_t));
    }
#endif /* portSTACK_GROWTH */

    પ્રતિજ્ઞા(pxTCBBufferTemp != ખાલી);
    પ્રતિજ્ઞા(px_ઢગલો_Buffer_Temp != ખાલી);
    //Write back pointers
    *ppxTimerTaskTCBBuffer = pxTCBBufferTemp;
    *ppx_Timer_Task_ઢગલો_Buffer = px_ઢગલો_Buffer_Temp;
    *pul_Timer_Task_ઢગલો_Size = config_TIMER_TASK_ઢગલો_DEPTH;
}
#endif //configUSE_TIMERS

#endif // configSUPPORT_STATIC_ALLOCATION == 1
