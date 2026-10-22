/*
 * SPDX-FileCopyrightText: 2021-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ "FreeRTOS.h"
#ଅନ୍ତର୍ଭାଵ "task.h"
#ଅନ୍ତର୍ଭାଵ "esp_system.h"
#ଅନ୍ତର୍ଭାଵ "esp_memory_utils.h"
#ଅନ୍ତର୍ଭାଵ "sdkconfig.h"

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
#ଯଦି ( configSUPPORT_STATIC_ALLOCATION == 1 )
ଶୂନ୍ୟ vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   ଗଦା_Type_t **ppx_Idle_Task_ଗଦା_Buffer,
                                   uint32_t *pul_Idle_Task_ଗଦା_Size)
{
    StaticTask_t *pxTCBBufferTemp;
    ଗଦା_Type_t *px_ଗଦା_Buffer_Temp;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#ଯଦି (port_ଗଦା_GROWTH > 0)
    {
        pxTCBBufferTemp = pvPortMalloc(ଆକାର(StaticTask_t));
        px_ଗଦା_Buffer_Temp = pvPortMalloc(config_MINIMAL_ଗଦା_SIZE);
    }
#ଅନ୍ୟଥା /* portSTACK_GROWTH */
    {
        px_ଗଦା_Buffer_Temp = pvPortMalloc(config_MINIMAL_ଗଦା_SIZE);
        pxTCBBufferTemp = pvPortMalloc(ଆକାର(StaticTask_t));
    }
#endif /* portSTACK_GROWTH */

    ପ୍ରତିଜ୍ଞା(pxTCBBufferTemp != ଖାଲି);
    ପ୍ରତିଜ୍ଞା(px_ଗଦା_Buffer_Temp != ଖାଲି);
    //Write back pointers
    *ppxIdleTaskTCBBuffer = pxTCBBufferTemp;
    *ppx_Idle_Task_ଗଦା_Buffer = px_ଗଦା_Buffer_Temp;
    *pul_Idle_Task_ଗଦା_Size = config_MINIMAL_ଗଦା_SIZE;
}

#ଯଦି ( ( CONFIG_FREERTOS_SMP ) && ( config_ସଂଖ୍ୟା_OF_CORES > 1 ) )
ଶୂନ୍ୟ vApplicationGetPassiveIdleTaskMemory(StaticTask_t ** ppxIdleTaskTCBBuffer,
                                          ଗଦା_Type_t ** ppx_Idle_Task_ଗଦା_Buffer,
                                          uint32_t * pul_Idle_Task_ଗଦା_Size,
                                          BaseType_t xPassiveIdleTaskIndex)
{
    vApplicationGetIdleTaskMemory(ppxIdleTaskTCBBuffer, ppx_Idle_Task_ଗଦା_Buffer, pul_Idle_Task_ଗଦା_Size);
}
#endif /* ( ( CONFIG_FREERTOS_SMP ) && ( configNUMBER_OF_CORES > 1 ) ) */

#ଯଦି configUSE_TIMERS

ଶୂନ୍ୟ vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    ଗଦା_Type_t **ppx_Timer_Task_ଗଦା_Buffer,
                                    uint32_t *pul_Timer_Task_ଗଦା_Size)
{
    StaticTask_t *pxTCBBufferTemp;
    ଗଦା_Type_t *px_ଗଦା_Buffer_Temp;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#ଯଦି (port_ଗଦା_GROWTH > 0)
    {
        pxTCBBufferTemp = pvPortMalloc(ଆକାର(StaticTask_t));
        px_ଗଦା_Buffer_Temp = pvPortMalloc(config_TIMER_TASK_ଗଦା_DEPTH);
    }
#ଅନ୍ୟଥା /* portSTACK_GROWTH */
    {
        px_ଗଦା_Buffer_Temp = pvPortMalloc(config_TIMER_TASK_ଗଦା_DEPTH);
        pxTCBBufferTemp = pvPortMalloc(ଆକାର(StaticTask_t));
    }
#endif /* portSTACK_GROWTH */

    ପ୍ରତିଜ୍ଞା(pxTCBBufferTemp != ଖାଲି);
    ପ୍ରତିଜ୍ଞା(px_ଗଦା_Buffer_Temp != ଖାଲି);
    //Write back pointers
    *ppxTimerTaskTCBBuffer = pxTCBBufferTemp;
    *ppx_Timer_Task_ଗଦା_Buffer = px_ଗଦା_Buffer_Temp;
    *pul_Timer_Task_ଗଦା_Size = config_TIMER_TASK_ଗଦା_DEPTH;
}
#endif //configUSE_TIMERS

#endif // configSUPPORT_STATIC_ALLOCATION == 1
