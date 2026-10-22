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
#ܐܢ ( configSUPPORT_STATIC_ALLOCATION == 1 )
فراغ vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
    StaticTask_t *pxTCBBufferTemp;
    StackType_t *pxStackBufferTemp;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#ܐܢ (portSTACK_GROWTH > 0)
    {
        pxTCBBufferTemp = pvPortMalloc(حجم(StaticTask_t));
        pxStackBufferTemp = pvPortMalloc(configMINIMAL_STACK_SIZE);
    }
#ܐܚܪܢܐ /* portSTACK_GROWTH */
    {
        pxStackBufferTemp = pvPortMalloc(configMINIMAL_STACK_SIZE);
        pxTCBBufferTemp = pvPortMalloc(حجم(StaticTask_t));
    }
#endif /* portSTACK_GROWTH */

    أكد(pxTCBBufferTemp != عدم);
    أكد(pxStackBufferTemp != عدم);
    //Write back pointers
    *ppxIdleTaskTCBBuffer = pxTCBBufferTemp;
    *ppxIdleTaskStackBuffer = pxStackBufferTemp;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

#ܐܢ ( ( CONFIG_FREERTOS_SMP ) && ( configNUMBER_OF_CORES > 1 ) )
فراغ vApplicationGetPassiveIdleTaskMemory(StaticTask_t ** ppxIdleTaskTCBBuffer,
                                          StackType_t ** ppxIdleTaskStackBuffer,
                                          uint32_t * pulIdleTaskStackSize,
                                          BaseType_t xPassiveIdleTaskIndex)
{
    vApplicationGetIdleTaskMemory(ppxIdleTaskTCBBuffer, ppxIdleTaskStackBuffer, pulIdleTaskStackSize);
}
#endif /* ( ( CONFIG_FREERTOS_SMP ) && ( configNUMBER_OF_CORES > 1 ) ) */

#ܐܢ configUSE_TIMERS

فراغ vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize)
{
    StaticTask_t *pxTCBBufferTemp;
    StackType_t *pxStackBufferTemp;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#ܐܢ (portSTACK_GROWTH > 0)
    {
        pxTCBBufferTemp = pvPortMalloc(حجم(StaticTask_t));
        pxStackBufferTemp = pvPortMalloc(configTIMER_TASK_STACK_DEPTH);
    }
#ܐܚܪܢܐ /* portSTACK_GROWTH */
    {
        pxStackBufferTemp = pvPortMalloc(configTIMER_TASK_STACK_DEPTH);
        pxTCBBufferTemp = pvPortMalloc(حجم(StaticTask_t));
    }
#endif /* portSTACK_GROWTH */

    أكد(pxTCBBufferTemp != عدم);
    أكد(pxStackBufferTemp != عدم);
    //Write back pointers
    *ppxTimerTaskTCBBuffer = pxTCBBufferTemp;
    *ppxTimerTaskStackBuffer = pxStackBufferTemp;
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
#endif //configUSE_TIMERS

#endif // configSUPPORT_STATIC_ALLOCATION == 1
