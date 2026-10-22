/*
 * SPDX-FileCopyrightText: 2021-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ "FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "task.h"
#ಅನ್ತರ್ಭಾವ "esp_system.h"
#ಅನ್ತರ್ಭಾವ "esp_memory_utils.h"
#ಅನ್ತರ್ಭಾವ "sdkconfig.h"

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
#ಯದಿ ( configSUPPORT_STATIC_ALLOCATION == 1 )
ಶೂನ್ಯ vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   ರಾಶಿ_Type_t **ppx_Idle_Task_ರಾಶಿ_Buffer,
                                   uint32_t *pul_Idle_Task_ರಾಶಿ_Size)
{
    StaticTask_t *pxTCBBufferTemp;
    ರಾಶಿ_Type_t *px_ರಾಶಿ_Buffer_Temp;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#ಯದಿ (port_ರಾಶಿ_GROWTH > 0)
    {
        pxTCBBufferTemp = pvPortMalloc(ಪರಿಮಾಣ(StaticTask_t));
        px_ರಾಶಿ_Buffer_Temp = pvPortMalloc(config_MINIMAL_ರಾಶಿ_SIZE);
    }
#ಅನ್ಯಥಾ /* portSTACK_GROWTH */
    {
        px_ರಾಶಿ_Buffer_Temp = pvPortMalloc(config_MINIMAL_ರಾಶಿ_SIZE);
        pxTCBBufferTemp = pvPortMalloc(ಪರಿಮಾಣ(StaticTask_t));
    }
#endif /* portSTACK_GROWTH */

    ಪ್ರತಿಜ್ಞಾ(pxTCBBufferTemp != ರಿಕ್ತ);
    ಪ್ರತಿಜ್ಞಾ(px_ರಾಶಿ_Buffer_Temp != ರಿಕ್ತ);
    //Write back pointers
    *ppxIdleTaskTCBBuffer = pxTCBBufferTemp;
    *ppx_Idle_Task_ರಾಶಿ_Buffer = px_ರಾಶಿ_Buffer_Temp;
    *pul_Idle_Task_ರಾಶಿ_Size = config_MINIMAL_ರಾಶಿ_SIZE;
}

#ಯದಿ ( ( CONFIG_FREERTOS_SMP ) && ( config_ಸಂಖ್ಯೆ_OF_CORES > 1 ) )
ಶೂನ್ಯ vApplicationGetPassiveIdleTaskMemory(StaticTask_t ** ppxIdleTaskTCBBuffer,
                                          ರಾಶಿ_Type_t ** ppx_Idle_Task_ರಾಶಿ_Buffer,
                                          uint32_t * pul_Idle_Task_ರಾಶಿ_Size,
                                          BaseType_t xPassiveIdleTaskIndex)
{
    vApplicationGetIdleTaskMemory(ppxIdleTaskTCBBuffer, ppx_Idle_Task_ರಾಶಿ_Buffer, pul_Idle_Task_ರಾಶಿ_Size);
}
#endif /* ( ( CONFIG_FREERTOS_SMP ) && ( configNUMBER_OF_CORES > 1 ) ) */

#ಯದಿ configUSE_TIMERS

ಶೂನ್ಯ vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    ರಾಶಿ_Type_t **ppx_Timer_Task_ರಾಶಿ_Buffer,
                                    uint32_t *pul_Timer_Task_ರಾಶಿ_Size)
{
    StaticTask_t *pxTCBBufferTemp;
    ರಾಶಿ_Type_t *px_ರಾಶಿ_Buffer_Temp;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#ಯದಿ (port_ರಾಶಿ_GROWTH > 0)
    {
        pxTCBBufferTemp = pvPortMalloc(ಪರಿಮಾಣ(StaticTask_t));
        px_ರಾಶಿ_Buffer_Temp = pvPortMalloc(config_TIMER_TASK_ರಾಶಿ_DEPTH);
    }
#ಅನ್ಯಥಾ /* portSTACK_GROWTH */
    {
        px_ರಾಶಿ_Buffer_Temp = pvPortMalloc(config_TIMER_TASK_ರಾಶಿ_DEPTH);
        pxTCBBufferTemp = pvPortMalloc(ಪರಿಮಾಣ(StaticTask_t));
    }
#endif /* portSTACK_GROWTH */

    ಪ್ರತಿಜ್ಞಾ(pxTCBBufferTemp != ರಿಕ್ತ);
    ಪ್ರತಿಜ್ಞಾ(px_ರಾಶಿ_Buffer_Temp != ರಿಕ್ತ);
    //Write back pointers
    *ppxTimerTaskTCBBuffer = pxTCBBufferTemp;
    *ppx_Timer_Task_ರಾಶಿ_Buffer = px_ರಾಶಿ_Buffer_Temp;
    *pul_Timer_Task_ರಾಶಿ_Size = config_TIMER_TASK_ರಾಶಿ_DEPTH;
}
#endif //configUSE_TIMERS

#endif // configSUPPORT_STATIC_ALLOCATION == 1
