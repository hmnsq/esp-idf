/*
 * SPDX-FileCopyrightText: 2021-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#அந்தர்பாவ <string.h>
#அந்தர்பாவ "FreeRTOS.h"
#அந்தர்பாவ "task.h"
#அந்தர்பாவ "esp_system.h"
#அந்தர்பாவ "esp_memory_utils.h"
#அந்தர்பாவ "sdkconfig.h"

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
#எனில் ( configSUPPORT_STATIC_ALLOCATION == 1 )
வெறுமை vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   அடுக்கு_Type_t **ppx_Idle_Task_அடுக்கு_Buffer,
                                   uint32_t *pul_Idle_Task_அடுக்கு_Size)
{
    StaticTask_t *pxTCBBufferTemp;
    அடுக்கு_Type_t *px_அடுக்கு_Buffer_Temp;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#எனில் (port_அடுக்கு_GROWTH > 0)
    {
        pxTCBBufferTemp = pvPortMalloc(அளவு(StaticTask_t));
        px_அடுக்கு_Buffer_Temp = pvPortMalloc(config_MINIMAL_அடுக்கு_SIZE);
    }
#இல்லையெனில் /* portSTACK_GROWTH */
    {
        px_அடுக்கு_Buffer_Temp = pvPortMalloc(config_MINIMAL_அடுக்கு_SIZE);
        pxTCBBufferTemp = pvPortMalloc(அளவு(StaticTask_t));
    }
#endif /* portSTACK_GROWTH */

    ப்ரதிஜ்ஞா(pxTCBBufferTemp != வெற்று);
    ப்ரதிஜ்ஞா(px_அடுக்கு_Buffer_Temp != வெற்று);
    //Write back pointers
    *ppxIdleTaskTCBBuffer = pxTCBBufferTemp;
    *ppx_Idle_Task_அடுக்கு_Buffer = px_அடுக்கு_Buffer_Temp;
    *pul_Idle_Task_அடுக்கு_Size = config_MINIMAL_அடுக்கு_SIZE;
}

#எனில் ( ( CONFIG_FREERTOS_SMP ) && ( config_எண்_OF_CORES > 1 ) )
வெறுமை vApplicationGetPassiveIdleTaskMemory(StaticTask_t ** ppxIdleTaskTCBBuffer,
                                          அடுக்கு_Type_t ** ppx_Idle_Task_அடுக்கு_Buffer,
                                          uint32_t * pul_Idle_Task_அடுக்கு_Size,
                                          BaseType_t xPassiveIdleTaskIndex)
{
    vApplicationGetIdleTaskMemory(ppxIdleTaskTCBBuffer, ppx_Idle_Task_அடுக்கு_Buffer, pul_Idle_Task_அடுக்கு_Size);
}
#endif /* ( ( CONFIG_FREERTOS_SMP ) && ( configNUMBER_OF_CORES > 1 ) ) */

#எனில் configUSE_TIMERS

வெறுமை vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    அடுக்கு_Type_t **ppx_Timer_Task_அடுக்கு_Buffer,
                                    uint32_t *pul_Timer_Task_அடுக்கு_Size)
{
    StaticTask_t *pxTCBBufferTemp;
    அடுக்கு_Type_t *px_அடுக்கு_Buffer_Temp;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#எனில் (port_அடுக்கு_GROWTH > 0)
    {
        pxTCBBufferTemp = pvPortMalloc(அளவு(StaticTask_t));
        px_அடுக்கு_Buffer_Temp = pvPortMalloc(config_TIMER_TASK_அடுக்கு_DEPTH);
    }
#இல்லையெனில் /* portSTACK_GROWTH */
    {
        px_அடுக்கு_Buffer_Temp = pvPortMalloc(config_TIMER_TASK_அடுக்கு_DEPTH);
        pxTCBBufferTemp = pvPortMalloc(அளவு(StaticTask_t));
    }
#endif /* portSTACK_GROWTH */

    ப்ரதிஜ்ஞா(pxTCBBufferTemp != வெற்று);
    ப்ரதிஜ்ஞா(px_அடுக்கு_Buffer_Temp != வெற்று);
    //Write back pointers
    *ppxTimerTaskTCBBuffer = pxTCBBufferTemp;
    *ppx_Timer_Task_அடுக்கு_Buffer = px_அடுக்கு_Buffer_Temp;
    *pul_Timer_Task_அடுக்கு_Size = config_TIMER_TASK_அடுக்கு_DEPTH;
}
#endif //configUSE_TIMERS

#endif // configSUPPORT_STATIC_ALLOCATION == 1
