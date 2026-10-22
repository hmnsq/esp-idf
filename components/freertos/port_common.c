/*
 * SPDX-FileCopyrightText: 2021-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "esp_system.h"
#include "esp_memory_utils.h"
#include "sdkconfig.h"

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
#if ( configSUPPORT_STATIC_ALLOCATION == 1 )
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   सालानसान_Type_t **ppx_Idle_Task_सालानसान_Buffer,
                                   uint32_t *pul_Idle_Task_सालानसान_Size)
{
    StaticTask_t *pxTCBBufferTemp;
    सालानसान_Type_t *px_सालानसान_Buffer_Temp;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#if (port_सालानसान_GROWTH > 0)
    {
        pxTCBBufferTemp = pvPortMalloc(sizeof(StaticTask_t));
        px_सालानसान_Buffer_Temp = pvPortMalloc(config_MINIMAL_सालानसान_SIZE);
    }
#else /* portSTACK_GROWTH */
    {
        px_सालानसान_Buffer_Temp = pvPortMalloc(config_MINIMAL_सालानसान_SIZE);
        pxTCBBufferTemp = pvPortMalloc(sizeof(StaticTask_t));
    }
#endif /* portSTACK_GROWTH */

    assert(pxTCBBufferTemp != NULL);
    assert(px_सालानसान_Buffer_Temp != NULL);
    //Write back pointers
    *ppxIdleTaskTCBBuffer = pxTCBBufferTemp;
    *ppx_Idle_Task_सालानसान_Buffer = px_सालानसान_Buffer_Temp;
    *pul_Idle_Task_सालानसान_Size = config_MINIMAL_सालानसान_SIZE;
}

#if ( ( CONFIG_FREERTOS_SMP ) && ( config_नुमेरो_OF_CORES > 1 ) )
void vApplicationGetPassiveIdleTaskMemory(StaticTask_t ** ppxIdleTaskTCBBuffer,
                                          सालानसान_Type_t ** ppx_Idle_Task_सालानसान_Buffer,
                                          uint32_t * pul_Idle_Task_सालानसान_Size,
                                          BaseType_t xPassiveIdleTaskIndex)
{
    vApplicationGetIdleTaskMemory(ppxIdleTaskTCBBuffer, ppx_Idle_Task_सालानसान_Buffer, pul_Idle_Task_सालानसान_Size);
}
#endif /* ( ( CONFIG_FREERTOS_SMP ) && ( configNUMBER_OF_CORES > 1 ) ) */

#if configUSE_TIMERS

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    सालानसान_Type_t **ppx_Timer_Task_सालानसान_Buffer,
                                    uint32_t *pul_Timer_Task_सालानसान_Size)
{
    StaticTask_t *pxTCBBufferTemp;
    सालानसान_Type_t *px_सालानसान_Buffer_Temp;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#if (port_सालानसान_GROWTH > 0)
    {
        pxTCBBufferTemp = pvPortMalloc(sizeof(StaticTask_t));
        px_सालानसान_Buffer_Temp = pvPortMalloc(config_TIMER_TASK_सालानसान_DEPTH);
    }
#else /* portSTACK_GROWTH */
    {
        px_सालानसान_Buffer_Temp = pvPortMalloc(config_TIMER_TASK_सालानसान_DEPTH);
        pxTCBBufferTemp = pvPortMalloc(sizeof(StaticTask_t));
    }
#endif /* portSTACK_GROWTH */

    assert(pxTCBBufferTemp != NULL);
    assert(px_सालानसान_Buffer_Temp != NULL);
    //Write back pointers
    *ppxTimerTaskTCBBuffer = pxTCBBufferTemp;
    *ppx_Timer_Task_सालानसान_Buffer = px_सालानसान_Buffer_Temp;
    *pul_Timer_Task_सालानसान_Size = config_TIMER_TASK_सालानसान_DEPTH;
}
#endif //configUSE_TIMERS

#endif // configSUPPORT_STATIC_ALLOCATION == 1
