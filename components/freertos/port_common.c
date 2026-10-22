/*
 * SPDX-FileCopyrightText: 2021-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#समावेश <string.h>
#समावेश "FreeRTOS.h"
#समावेश "task.h"
#समावेश "esp_system.h"
#समावेश "esp_memory_utils.h"
#समावेश "sdkconfig.h"

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
#अगर ( configSUPPORT_STATIC_ALLOCATION == 1 )
व्योम vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   ढेर_Type_t **ppx_Idle_Task_ढेर_बफर,
                                   uint32_t *pul_Idle_Task_ढेर_आकार)
{
    StaticTask_t *px_TCBBuffer_अस्थायी;
    ढेर_Type_t *px_ढेर_बफर_अस्थायी;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#अगर (port_ढेर_GROWTH > 0)
    {
        px_TCBBuffer_अस्थायी = pvPortMalloc(माप(StaticTask_t));
        px_ढेर_बफर_अस्थायी = pvPortMalloc(config_MINIMAL_ढेर_आकार);
    }
#अन्यथा /* portSTACK_GROWTH */
    {
        px_ढेर_बफर_अस्थायी = pvPortMalloc(config_MINIMAL_ढेर_आकार);
        px_TCBBuffer_अस्थायी = pvPortMalloc(माप(StaticTask_t));
    }
#पूर्ण_अगर /* portSTACK_GROWTH */

    निश्चित(px_TCBBuffer_अस्थायी != शून्य);
    निश्चित(px_ढेर_बफर_अस्थायी != शून्य);
    //Write back pointers
    *ppxIdleTaskTCBBuffer = px_TCBBuffer_अस्थायी;
    *ppx_Idle_Task_ढेर_बफर = px_ढेर_बफर_अस्थायी;
    *pul_Idle_Task_ढेर_आकार = config_MINIMAL_ढेर_आकार;
}

#अगर ( ( CONFIG_FREERTOS_SMP ) && ( config_संख्या_OF_CORES > 1 ) )
व्योम vApplicationGetPassiveIdleTaskMemory(StaticTask_t ** ppxIdleTaskTCBBuffer,
                                          ढेर_Type_t ** ppx_Idle_Task_ढेर_बफर,
                                          uint32_t * pul_Idle_Task_ढेर_आकार,
                                          BaseType_t x_Passive_Idle_Task_सूचकांक)
{
    vApplicationGetIdleTaskMemory(ppxIdleTaskTCBBuffer, ppx_Idle_Task_ढेर_बफर, pul_Idle_Task_ढेर_आकार);
}
#पूर्ण_अगर /* ( ( CONFIG_FREERTOS_SMP ) && ( configNUMBER_OF_CORES > 1 ) ) */

#अगर configUSE_TIMERS

व्योम vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    ढेर_Type_t **ppx_Timer_Task_ढेर_बफर,
                                    uint32_t *pul_Timer_Task_ढेर_आकार)
{
    StaticTask_t *px_TCBBuffer_अस्थायी;
    ढेर_Type_t *px_ढेर_बफर_अस्थायी;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#अगर (port_ढेर_GROWTH > 0)
    {
        px_TCBBuffer_अस्थायी = pvPortMalloc(माप(StaticTask_t));
        px_ढेर_बफर_अस्थायी = pvPortMalloc(config_TIMER_TASK_ढेर_DEPTH);
    }
#अन्यथा /* portSTACK_GROWTH */
    {
        px_ढेर_बफर_अस्थायी = pvPortMalloc(config_TIMER_TASK_ढेर_DEPTH);
        px_TCBBuffer_अस्थायी = pvPortMalloc(माप(StaticTask_t));
    }
#पूर्ण_अगर /* portSTACK_GROWTH */

    निश्चित(px_TCBBuffer_अस्थायी != शून्य);
    निश्चित(px_ढेर_बफर_अस्थायी != शून्य);
    //Write back pointers
    *ppxTimerTaskTCBBuffer = px_TCBBuffer_अस्थायी;
    *ppx_Timer_Task_ढेर_बफर = px_ढेर_बफर_अस्थायी;
    *pul_Timer_Task_ढेर_आकार = config_TIMER_TASK_ढेर_DEPTH;
}
#पूर्ण_अगर //configUSE_TIMERS

#पूर्ण_अगर // configSUPPORT_STATIC_ALLOCATION == 1
