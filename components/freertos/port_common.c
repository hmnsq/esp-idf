/*
 * SPDX-FileCopyrightText: 2021-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#অন্তর্ভাব <string.h>
#অন্তর্ভাব "FreeRTOS.h"
#অন্তর্ভাব "task.h"
#অন্তর্ভাব "esp_system.h"
#অন্তর্ভাব "esp_memory_utils.h"
#অন্তর্ভাব "sdkconfig.h"

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
#যদি ( configSUPPORT_STATIC_ALLOCATION == 1 )
শূন্য vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   স্তূপ_Type_t **ppx_Idle_Task_স্তূপ_বাফার,
                                   uint32_t *pul_Idle_Task_স্তূপ_আকার)
{
    StaticTask_t *px_TCBBuffer_অস্থায়ী;
    স্তূপ_Type_t *px_স্তূপ_বাফার_অস্থায়ী;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#যদি (port_স্তূপ_GROWTH > 0)
    {
        px_TCBBuffer_অস্থায়ী = pvPortMalloc(আকার(StaticTask_t));
        px_স্তূপ_বাফার_অস্থায়ী = pvPortMalloc(config_MINIMAL_স্তূপ_আকার);
    }
#অন্যথা /* portSTACK_GROWTH */
    {
        px_স্তূপ_বাফার_অস্থায়ী = pvPortMalloc(config_MINIMAL_স্তূপ_আকার);
        px_TCBBuffer_অস্থায়ী = pvPortMalloc(আকার(StaticTask_t));
    }
#endif /* portSTACK_GROWTH */

    প্রতিজ্ঞা(px_TCBBuffer_অস্থায়ী != ফাঁকা);
    প্রতিজ্ঞা(px_স্তূপ_বাফার_অস্থায়ী != ফাঁকা);
    //Write back pointers
    *ppxIdleTaskTCBBuffer = px_TCBBuffer_অস্থায়ী;
    *ppx_Idle_Task_স্তূপ_বাফার = px_স্তূপ_বাফার_অস্থায়ী;
    *pul_Idle_Task_স্তূপ_আকার = config_MINIMAL_স্তূপ_আকার;
}

#যদি ( ( CONFIG_FREERTOS_SMP ) && ( config_সংখ্যা_OF_CORES > 1 ) )
শূন্য vApplicationGetPassiveIdleTaskMemory(StaticTask_t ** ppxIdleTaskTCBBuffer,
                                          স্তূপ_Type_t ** ppx_Idle_Task_স্তূপ_বাফার,
                                          uint32_t * pul_Idle_Task_স্তূপ_আকার,
                                          BaseType_t x_Passive_Idle_Task_সূচক)
{
    vApplicationGetIdleTaskMemory(ppxIdleTaskTCBBuffer, ppx_Idle_Task_স্তূপ_বাফার, pul_Idle_Task_স্তূপ_আকার);
}
#endif /* ( ( CONFIG_FREERTOS_SMP ) && ( configNUMBER_OF_CORES > 1 ) ) */

#যদি configUSE_TIMERS

শূন্য vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    স্তূপ_Type_t **ppx_Timer_Task_স্তূপ_বাফার,
                                    uint32_t *pul_Timer_Task_স্তূপ_আকার)
{
    StaticTask_t *px_TCBBuffer_অস্থায়ী;
    স্তূপ_Type_t *px_স্তূপ_বাফার_অস্থায়ী;

    /* Allocate TCB and stack buffer from the FreeRTOS heap
     *
     * If the stack grows down then allocate the stack then the TCB so the stack
     * does not grow into the TCB.  Likewise if the stack grows up then allocate
     * the TCB then the stack. */
#যদি (port_স্তূপ_GROWTH > 0)
    {
        px_TCBBuffer_অস্থায়ী = pvPortMalloc(আকার(StaticTask_t));
        px_স্তূপ_বাফার_অস্থায়ী = pvPortMalloc(config_TIMER_TASK_স্তূপ_DEPTH);
    }
#অন্যথা /* portSTACK_GROWTH */
    {
        px_স্তূপ_বাফার_অস্থায়ী = pvPortMalloc(config_TIMER_TASK_স্তূপ_DEPTH);
        px_TCBBuffer_অস্থায়ী = pvPortMalloc(আকার(StaticTask_t));
    }
#endif /* portSTACK_GROWTH */

    প্রতিজ্ঞা(px_TCBBuffer_অস্থায়ী != ফাঁকা);
    প্রতিজ্ঞা(px_স্তূপ_বাফার_অস্থায়ী != ফাঁকা);
    //Write back pointers
    *ppxTimerTaskTCBBuffer = px_TCBBuffer_অস্থায়ী;
    *ppx_Timer_Task_স্তূপ_বাফার = px_স্তূপ_বাফার_অস্থায়ী;
    *pul_Timer_Task_স্তূপ_আকার = config_TIMER_TASK_স্তূপ_DEPTH;
}
#endif //configUSE_TIMERS

#endif // configSUPPORT_STATIC_ALLOCATION == 1
