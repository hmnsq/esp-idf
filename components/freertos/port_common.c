/*
 * SPDX-FileCopyrightText: 2021-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#अन्तर्भाव <string.h>
#अन्तर्भाव "FreeRTOS.h"
#अन्तर्भाव "task.h"
#अन्तर्भाव "esp_system.h"
#अन्तर्भाव "esp_memory_utils.h"
#अन्तर्भाव "sdkconfig.h"

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
#सचे ( configSUPPORT_STATIC_ALLOCATION == 1 )
शून्य vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   राशिः_Type_t **ppx_Idle_Task_राशिः_Buffer,
                                   uint32_t *pul_Idle_Task_राशिः_Size)
{
    StaticTask_t *pxTCBBufferTemp;
    राशिः_Type_t *px_राशिः_Buffer_Temp;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#सचे (port_राशिः_GROWTH > 0)
    {
        pxTCBBufferTemp = pvPortMalloc(परिमाण(StaticTask_t));
        px_राशिः_Buffer_Temp = pvPortMalloc(config_MINIMAL_राशिः_SIZE);
    }
#अञ्ञथा /* portSTACK_GROWTH */
    {
        px_राशिः_Buffer_Temp = pvPortMalloc(config_MINIMAL_राशिः_SIZE);
        pxTCBBufferTemp = pvPortMalloc(परिमाण(StaticTask_t));
    }
#endif /* portSTACK_GROWTH */

    प्रतिज्ञा(pxTCBBufferTemp != रिक्त);
    प्रतिज्ञा(px_राशिः_Buffer_Temp != रिक्त);
    //Write back pointers
    *ppxIdleTaskTCBBuffer = pxTCBBufferTemp;
    *ppx_Idle_Task_राशिः_Buffer = px_राशिः_Buffer_Temp;
    *pul_Idle_Task_राशिः_Size = config_MINIMAL_राशिः_SIZE;
}

#सचे ( ( CONFIG_FREERTOS_SMP ) && ( config_सङ्ख्या_OF_CORES > 1 ) )
शून्य vApplicationGetPassiveIdleTaskMemory(StaticTask_t ** ppxIdleTaskTCBBuffer,
                                          राशिः_Type_t ** ppx_Idle_Task_राशिः_Buffer,
                                          uint32_t * pul_Idle_Task_राशिः_Size,
                                          BaseType_t xPassiveIdleTaskIndex)
{
    vApplicationGetIdleTaskMemory(ppxIdleTaskTCBBuffer, ppx_Idle_Task_राशिः_Buffer, pul_Idle_Task_राशिः_Size);
}
#endif /* ( ( CONFIG_FREERTOS_SMP ) && ( configNUMBER_OF_CORES > 1 ) ) */

#सचे configUSE_TIMERS

शून्य vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    राशिः_Type_t **ppx_Timer_Task_राशिः_Buffer,
                                    uint32_t *pul_Timer_Task_राशिः_Size)
{
    StaticTask_t *pxTCBBufferTemp;
    राशिः_Type_t *px_राशिः_Buffer_Temp;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#सचे (port_राशिः_GROWTH > 0)
    {
        pxTCBBufferTemp = pvPortMalloc(परिमाण(StaticTask_t));
        px_राशिः_Buffer_Temp = pvPortMalloc(config_TIMER_TASK_राशिः_DEPTH);
    }
#अञ्ञथा /* portSTACK_GROWTH */
    {
        px_राशिः_Buffer_Temp = pvPortMalloc(config_TIMER_TASK_राशिः_DEPTH);
        pxTCBBufferTemp = pvPortMalloc(परिमाण(StaticTask_t));
    }
#endif /* portSTACK_GROWTH */

    प्रतिज्ञा(pxTCBBufferTemp != रिक्त);
    प्रतिज्ञा(px_राशिः_Buffer_Temp != रिक्त);
    //Write back pointers
    *ppxTimerTaskTCBBuffer = pxTCBBufferTemp;
    *ppx_Timer_Task_राशिः_Buffer = px_राशिः_Buffer_Temp;
    *pul_Timer_Task_राशिः_Size = config_TIMER_TASK_राशिः_DEPTH;
}
#endif //configUSE_TIMERS

#endif // configSUPPORT_STATIC_ALLOCATION == 1
