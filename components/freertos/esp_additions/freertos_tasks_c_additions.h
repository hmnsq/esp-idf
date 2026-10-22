/*
 * SPDX-FileCopyrightText: 2015-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#অন্তর্ভাব "sdkconfig.h"
#অন্তর্ভাব "esp_assert.h"
#অন্তর্ভাব "esp_heap_caps.h"
#অন্তর্ভাব "esp_compiler.h"
#অন্তর্ভাব "freertos/idf_additions.h"
#অন্তর্ভাব "freertos/freertos_debug.h"
#অন্তর্ভাব "esp_private/freertos_idf_additions_priv.h"

/**
 * This file will be included in `tasks.c` file, thus, it is treated as a source
 * file instead of a header file, and must NOT be included by any (other) file.
 * This file is used to add additional functions to `tasks.c`. See the
 * `esp_additions/include` directory of the headers that expose these `tasks.c`
 * additional API.
 */

/* ------------------------------------------------- Static Asserts ------------------------------------------------- */

/*
 * Both StaticTask_t and TCB_t structures are provided by FreeRTOS sources.
 * This is just an additional check of the consistency of these structures.
 */
_Static_assert( offsetof( StaticTask_t, pxDummy6 ) == offsetof( TCB_t, px_স্তূপ ) );
_Static_assert( offsetof( StaticTask_t, pxDummy8 ) == offsetof( TCB_t, px_End_Of_স্তূপ ) );
#যদি !CONFIG_IDF_TARGET_LINUX    // Disabled for linux builds due to differences in types
_Static_assert( tskNO_AFFINITY == ( BaseType_t ) CONFIG_FREERTOS_NO_AFFINITY, "CONFIG_FREERTOS_NO_AFFINITY must be the same as tskNO_AFFINITY" );
#endif

/* ------------------------------------------------- Kernel Control ------------------------------------------------- */

#যদি ( !CONFIG_FREERTOS_SMP && ( config_অঙ্ক_CORES > 1 ) )

/*
 * Wrapper function to take "xKerneLock"
 */
    শূন্য prvTakeKernelLock( শূন্য )
    {
        /* We call the tasks.c critical section macro to take xKernelLock */
        taskENTER_CRITICAL( &xKernelLock );
    }

#endif /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#যদি ( !CONFIG_FREERTOS_SMP && ( config_অঙ্ক_CORES > 1 ) )

/*
 * Wrapper function to release "xKerneLock"
 */
    শূন্য prvReleaseKernelLock( শূন্য )
    {
        /* We call the tasks.c critical section macro to release xKernelLock */
        taskEXIT_CRITICAL( &xKernelLock );
    }

#endif /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#যদি ( CONFIG_FREERTOS_SMP && ( config_অঙ্ক_CORES > 1 ) )

/*
 * Workaround for non-thread safe multi-core OS startup (see IDF-4524)
 */
    শূন্য prvStartSchedulerOtherCores( শূন্য )
    {
        /* This function is always called with interrupts disabled*/
        xSchedulerRunning = pdTRUE;
    }

#endif /* ( CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#যদি ( !CONFIG_FREERTOS_SMP && ( config_অঙ্ক_CORES > 1 ) )

    BaseType_t xTaskIncrementTickOtherCores( শূন্য )
    {
        /* Minor optimization. This function can never switch cores mid
         * execution */
        BaseType_t xCoreID = portGET_CORE_ID();
        BaseType_t xSwitchRequired = pdFALSE;

        /* This function should never be called by Core 0. */
        configASSERT( xCoreID != 0 );

        /* Called by the portable layer each time a tick interrupt occurs
         * on a core other than core 0. */
        traceTASK_INCREMENT_TICK( x_Tick_গুনতি );

        যদি( uxSchedulerSuspended[ xCoreID ] == ( UBaseType_t ) 0U )
        {
            /* We need take the kernel lock here as we are about to access
             * kernel data structures. */
            taskENTER_CRITICAL_ISR( &xKernelLock );

            /* Tasks of equal priority to the currently running task will share
             * processing time (time slice) if preemption is on, and the application
             * writer has not explicitly turned time slicing off. */
            #যদি ( ( configUSE_PREEMPTION == 1 ) && ( configUSE_TIME_SLICING == 1 ) )
            {
                যদি( list_CURRENT_LIST_দৈর্ঘ্য( &( pxReadyTasksLists[ pxCurrentTCBs[ xCoreID ]->uxPriority ] ) ) > ( UBaseType_t ) 1 )
                {
                    xSwitchRequired = pdTRUE;
                }
                অন্যথা
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            #endif /* ( ( configUSE_PREEMPTION == 1 ) && ( configUSE_TIME_SLICING == 1 ) ) */

            /* Release the previously taken kernel lock as we have finished
             * accessing the kernel data structures. */
            taskEXIT_CRITICAL_ISR( &xKernelLock );

            #যদি ( configUSE_PREEMPTION == 1 )
            {
                যদি( xYieldPending[ xCoreID ] != pdFALSE )
                {
                    xSwitchRequired = pdTRUE;
                }
                অন্যথা
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            #endif /* configUSE_PREEMPTION */
        }

        #যদি ( configUSE_TICK_HOOK == 1 )
        {
            vApplicationTickHook();
        }
        #endif

        ফেরত xSwitchRequired;
    }

#endif /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

/* -------------------------------------------------- Task Creation ------------------------------------------------- */

#যদি ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )

    BaseType_t xTaskCreatePinnedToCore( TaskFunction_t pxTaskCode,
                                        ধ্রুবক অক্ষর * ধ্রুবক pcName,
                                        ধ্রুবক uint32_t us_স্তূপ_Depth,
                                        শূন্য * ধ্রুবক pvParameters,
                                        UBaseType_t uxPriority,
                                        TaskHandle_t * ধ্রুবক pxCreatedTask,
                                        ধ্রুবক BaseType_t xCoreID )
    {
        BaseType_t xReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );

        #যদি CONFIG_FREERTOS_SMP
        {
            /* If using Amazon SMP FreeRTOS. This function is just a wrapper around
             * xTaskCreate() or xTaskCreateAffinitySet(). */
            #যদি ( ( config_অঙ্ক_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
            {
                /* Convert xCoreID into an affinity mask */
                UBaseType_t uxCoreAffinityMask;

                /* Bit shifting << xCoreID is only valid if we have less than
                 * 32 cores. */
                ESP_STATIC_ASSERT( config_অঙ্ক_CORES < 32 );

                যদি( xCoreID == tskNO_AFFINITY )
                {
                    uxCoreAffinityMask = tskNO_AFFINITY;
                }
                অন্যথা
                {
                    uxCoreAffinityMask = ( 1 << xCoreID );
                }

                xReturn = xTaskCreateAffinitySet( pxTaskCode, pcName, us_স্তূপ_Depth, pvParameters, uxPriority, uxCoreAffinityMask, pxCreatedTask );
            }
            #অন্যথা /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            {
                xReturn = xTaskCreate( pxTaskCode, pcName, us_স্তূপ_Depth, pvParameters, uxPriority, pxCreatedTask );
            }
            #endif /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
        }
        #অন্যথা /* CONFIG_FREERTOS_SMP */
        {
            TCB_t * pxNewTCB;

            /* If the stack grows down then allocate the stack then the TCB so the
             * stack does not grow into the TCB.  Likewise if the stack grows up
             * then allocate the TCB then the stack. */
            #যদি ( port_স্তূপ_GROWTH > 0 )
            {
                /* Allocate space for the TCB.  Where the memory comes from depends on
                 * the implementation of the port malloc function and whether or not static
                 * allocation is being used. */
                pxNewTCB = ( TCB_t * ) pvPortMalloc( আকার( TCB_t ) );

                যদি( pxNewTCB != ফাঁকা )
                {
                    memset( ( শূন্য * ) pxNewTCB, 0x00, আকার( TCB_t ) );

                    /* Allocate space for the stack used by the task being created.
                     * The base of the stack memory stored in the TCB so the task can
                     * be deleted later if required. */
                    pxNewTCB->px_স্তূপ = ( স্তূপ_Type_t * ) pv_Port_Malloc_স্তূপ( ( ( ( size_t ) us_স্তূপ_Depth ) * আকার( স্তূপ_Type_t ) ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */

                    যদি( pxNewTCB->px_স্তূপ == ফাঁকা )
                    {
                        /* Could not allocate the stack.  Delete the allocated TCB. */
                        vPortFree( pxNewTCB );
                        pxNewTCB = ফাঁকা;
                    }
                }
            }
            #অন্যথা /* portSTACK_GROWTH */
            {
                স্তূপ_Type_t * px_স্তূপ;

                /* Allocate space for the stack used by the task being created. */
                px_স্তূপ = pv_Port_Malloc_স্তূপ( ( ( ( size_t ) us_স্তূপ_Depth ) * আকার( স্তূপ_Type_t ) ) ); /*lint !e9079 All values returned by pvPortMalloc() have at least the alignment required by the MCU's stack and this allocation is the stack. */

                যদি( px_স্তূপ != ফাঁকা )
                {
                    /* Allocate space for the TCB. */
                    pxNewTCB = ( TCB_t * ) pvPortMalloc( আকার( TCB_t ) ); /*lint !e9087 !e9079 All values returned by pvPortMalloc() have at least the alignment required by the MCU's stack, and the first member of TCB_t is always a pointer to the task's stack. */

                    যদি( pxNewTCB != ফাঁকা )
                    {
                        memset( ( শূন্য * ) pxNewTCB, 0x00, আকার( TCB_t ) );

                        /* Store the stack location in the TCB. */
                        pxNewTCB->px_স্তূপ = px_স্তূপ;
                    }
                    অন্যথা
                    {
                        /* The stack cannot be used as the TCB was not created.  Free
                         * it again. */
                        v_Port_Free_স্তূপ( px_স্তূপ );
                    }
                }
                অন্যথা
                {
                    pxNewTCB = ফাঁকা;
                }
            }
            #endif /* portSTACK_GROWTH */

            যদি( pxNewTCB != ফাঁকা )
            {
                #যদি ( tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE != 0 ) /*lint !e9029 !e731 Macro has been consolidated for readability reasons. */
                {
                    /* Tasks can be created statically or dynamically, so note this
                     * task was created dynamically in case it is later deleted. */
                    pxNewTCB->ucStaticallyAllocated = tsk_DYNAMICALLY_ALLOCATED_স্তূপ_AND_TCB;
                }
                #endif /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

                prvInitialiseNewTask( pxTaskCode, pcName, ( uint32_t ) us_স্তূপ_Depth, pvParameters, uxPriority, pxCreatedTask, pxNewTCB, ফাঁকা, xCoreID );
                prvAddNewTaskToReadyList( pxNewTCB );
                xReturn = pdPASS;
            }
            অন্যথা
            {
                xReturn = errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY;
            }
        }
        #endif /* CONFIG_FREERTOS_SMP */

        ফেরত xReturn;
    }

#endif /* ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#যদি ( configSUPPORT_STATIC_ALLOCATION == 1 )

    TaskHandle_t xTaskCreateStaticPinnedToCore( TaskFunction_t pxTaskCode,
                                                ধ্রুবক অক্ষর * ধ্রুবক pcName,
                                                ধ্রুবক uint32_t ul_স্তূপ_Depth,
                                                শূন্য * ধ্রুবক pvParameters,
                                                UBaseType_t uxPriority,
                                                স্তূপ_Type_t * ধ্রুবক pux_স্তূপ_বাফার,
                                                StaticTask_t * ধ্রুবক px_Task_বাফার,
                                                ধ্রুবক BaseType_t xCoreID )
    {
        TaskHandle_t xReturn;

        configASSERT( port_VALID_স্তূপ_MEM( pux_স্তূপ_বাফার ) );
        configASSERT( portVALID_TCB_MEM( px_Task_বাফার ) );
        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );

        #যদি CONFIG_FREERTOS_SMP
        {
            /* If using Amazon SMP FreeRTOS. This function is just a wrapper around
             * xTaskCreateStatic() or xTaskCreateStaticAffinitySet(). */
            #যদি ( ( config_অঙ্ক_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
            {
                /* Convert xCoreID into an affinity mask */
                UBaseType_t uxCoreAffinityMask;

                /* Bit shifting << xCoreID is only valid if we have less than
                 * 32 cores. */
                ESP_STATIC_ASSERT( config_অঙ্ক_CORES < 32 );

                যদি( xCoreID == tskNO_AFFINITY )
                {
                    uxCoreAffinityMask = tskNO_AFFINITY;
                }
                অন্যথা
                {
                    uxCoreAffinityMask = ( 1 << xCoreID );
                }

                xReturn = xTaskCreateStaticAffinitySet( pxTaskCode, pcName, ul_স্তূপ_Depth, pvParameters, uxPriority, pux_স্তূপ_বাফার, px_Task_বাফার, uxCoreAffinityMask );
            }
            #অন্যথা /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            {
                xReturn = xTaskCreateStatic( pxTaskCode, pcName, ul_স্তূপ_Depth, pvParameters, uxPriority, pux_স্তূপ_বাফার, px_Task_বাফার );
            }
            #endif /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
        }
        #অন্যথা /* CONFIG_FREERTOS_SMP */
        {
            TCB_t * pxNewTCB;

            #যদি ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticTask_t equals the size of the real task
                 * structure. */
                অস্থির size_t x_আকার = আকার( StaticTask_t );
                configASSERT( x_আকার == আকার( TCB_t ) );
                ( শূন্য ) x_আকার; /* Prevent lint warning when configASSERT() is not used. */
            }
            #endif /* configASSERT_DEFINED */

            যদি( ( px_Task_বাফার != ফাঁকা ) && ( pux_স্তূপ_বাফার != ফাঁকা ) )
            {
                /* The memory used for the task's TCB and stack are passed into this
                 * function - use them. */
                pxNewTCB = ( TCB_t * ) px_Task_বাফার; /*lint !e740 !e9087 Unusual cast is ok as the structures are designed to have the same alignment, and the size is checked by an assert. */
                memset( ( শূন্য * ) pxNewTCB, 0x00, আকার( TCB_t ) );
                pxNewTCB->px_স্তূপ = ( স্তূপ_Type_t * ) pux_স্তূপ_বাফার;

                #যদি ( tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE != 0 ) /*lint !e731 !e9029 Macro has been consolidated for readability reasons. */
                {
                    /* Tasks can be created statically or dynamically, so note this
                     * task was created statically in case the task is later deleted. */
                    pxNewTCB->ucStaticallyAllocated = tsk_STATICALLY_ALLOCATED_স্তূপ_AND_TCB;
                }
                #endif /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

                prvInitialiseNewTask( pxTaskCode, pcName, ul_স্তূপ_Depth, pvParameters, uxPriority, &xReturn, pxNewTCB, ফাঁকা, xCoreID );
                prvAddNewTaskToReadyList( pxNewTCB );
            }
            অন্যথা
            {
                xReturn = ফাঁকা;
            }
        }
        #endif /* CONFIG_FREERTOS_SMP */

        ফেরত xReturn;
    }

#endif /* ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#যদি ( configUSE_TIMERS == 1 )

/*
 * In ESP-IDF, configUSE_TIMERS is always defined as 1 (i.e., not user configurable).
 * However, tasks.c: vTaskStartScheduler() will always call xTimerCreateTimerTask()
 * if ( configUSE_TIMERS == 1 ), thus causing the linker to link timers.c and
 * wasting some memory (due to the timer task being created)/
 *
 * If we provide a weak version of xTimerCreateTimerTask(), this version will be
 * compiled if the application does not call any other FreeRTOS timer functions.
 * Thus we can save some text/RAM as timers.c will not be linked and the timer
 * task never created.
 */
    BaseType_t __attribute__( ( weak ) ) xTimerCreateTimerTask( শূন্য )
    {
        ফেরত pdPASS;
    }

#endif /* configUSE_TIMERS */
/*----------------------------------------------------------*/

/* ------------------------------------------------- Task Utilities ------------------------------------------------- */

BaseType_t xTaskGetCoreID( TaskHandle_t xTask )
{
    BaseType_t xReturn;

    #যদি ( config_অঙ্ক_CORES > 1 )
    {
        #যদি CONFIG_FREERTOS_SMP
            UBaseType_t uxCoreAffinityMask;

            /* Get the core affinity mask and convert it to an ID */
            uxCoreAffinityMask = vTaskCoreAffinityGet( xTask );

            /* If the task is not pinned to a particular core, treat it as tskNO_AFFINITY */
            যদি( uxCoreAffinityMask & ( uxCoreAffinityMask - 1 ) ) /* If more than one bit set */
            {
                xReturn = tskNO_AFFINITY;
            }
            অন্যথা
            {
                পূর্ণসংখ্যা i_সূচক_Plus_One = __builtin_ffs( uxCoreAffinityMask );
                প্রতিজ্ঞা( i_সূচক_Plus_One >= 1 );
                xReturn = i_সূচক_Plus_One - 1;
            }
        #অন্যথা /* CONFIG_FREERTOS_SMP */
            TCB_t * pxTCB;

            pxTCB = prvGetTCBFromHandle( xTask );

            xReturn = pxTCB->xCoreID;
        #endif /* CONFIG_FREERTOS_SMP */
    }
    #অন্যথা /* configNUM_CORES > 1 */
    {
        /* Single-core. Just return a core ID of 0 */
        xReturn = 0;
    }
    #endif /* configNUM_CORES > 1 */

    ফেরত xReturn;
}
/*----------------------------------------------------------*/

#যদি ( ( !CONFIG_FREERTOS_SMP ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

    TaskHandle_t xTaskGetIdleTaskHandleForCore( BaseType_t xCoreID )
    {
        /* If xTaskGetIdleTaskHandle() is called before the scheduler has been
         * started, then xIdleTaskHandle will be NULL. */
        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );
        configASSERT( ( xIdleTaskHandle[ xCoreID ] != ফাঁকা ) );
        ফেরত xIdleTaskHandle[ xCoreID ];
    }

#endif /* ( ( !CONFIG_FREERTOS_SMP ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*----------------------------------------------------------*/

#যদি ( ( !CONFIG_FREERTOS_SMP ) && ( ( INCLUDE_xTaskGetCurrentTaskHandle == 1 ) || ( configUSE_MUTEXES == 1 ) ) )

    TaskHandle_t xTaskGetCurrentTaskHandleForCore( BaseType_t xCoreID )
    {
        TaskHandle_t xReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

        #যদি ( CONFIG_FREERTOS_SMP )
        {
            xReturn = xTaskGetCurrentTaskHandleCPU( ( UBaseType_t ) xCoreID );
        }
        #অন্যথা /* CONFIG_FREERTOS_SMP */
        {
            /* A critical section is not required as this function does not
             * guarantee that the TCB will still be valid when this function
             * returns. */
            xReturn = pxCurrentTCBs[ xCoreID ];
        }
        #endif /* CONFIG_FREERTOS_SMP */

        ফেরত xReturn;
    }

#endif /* ( ( !CONFIG_FREERTOS_SMP ) && ( ( INCLUDE_xTaskGetCurrentTaskHandle == 1 ) || ( configUSE_MUTEXES == 1 ) ) ) */
/*----------------------------------------------------------*/

#যদি ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

    config_RUN_TIME_গণক_TYPE ul_Task_Get_Idle_Run_Time_গণক_For_Core( BaseType_t xCoreID )
    {
        config_RUN_TIME_গণক_TYPE ul_Run_Time_গণক;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

        /* For SMP, we need to take the kernel lock here as we are about to
         * access kernel data structures. */
        prvENTER_CRITICAL_SMP_ONLY( &xKernelLock );
        {
            ul_Run_Time_গণক = xIdleTaskHandle[ xCoreID ]->ul_Run_Time_গণক;
        }
        /* Release the previously taken kernel lock. */
        prvEXIT_CRITICAL_SMP_ONLY( &xKernelLock );

        ফেরত ul_Run_Time_গণক;
    }

#endif /* ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*----------------------------------------------------------*/

#যদি ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

    config_RUN_TIME_গণক_TYPE ulTaskGetIdleRunTimePercentForCore( BaseType_t xCoreID )
    {
        config_RUN_TIME_গণক_TYPE ul_যোগ_Time, ulReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

        #ifdef port_ALT_GET_RUN_TIME_গণক_মান
            port_ALT_GET_RUN_TIME_গণক_মান( ul_যোগ_Time );
        #অন্যথা
            ul_যোগ_Time = port_GET_RUN_TIME_গণক_মান();
        #endif

        /* For percentage calculations. */
        ul_যোগ_Time /= ( config_RUN_TIME_গণক_TYPE ) 100;

        /* Avoid divide by zero errors. */
        যদি( ul_যোগ_Time > ( config_RUN_TIME_গণক_TYPE ) 0 )
        {
            /* For SMP, we need to take the kernel lock here as we are about
             * to access kernel data structures. */
            prvENTER_CRITICAL_SMP_ONLY( &xKernelLock );
            {
                ulReturn = xIdleTaskHandle[ xCoreID ]->ul_Run_Time_গণক / ul_যোগ_Time;
            }
            /* Release the previously taken kernel lock. */
            prvEXIT_CRITICAL_SMP_ONLY( &xKernelLock );
        }
        অন্যথা
        {
            ulReturn = 0;
        }

        ফেরত ulReturn;
    }

#endif /* ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*-----------------------------------------------------------*/

স্তূপ_Type_t * x_Task_Get_স্তূপ_Start( TaskHandle_t xTask )
{
    TCB_t * pxTCB;

    pxTCB = prvGetTCBFromHandle( xTask );
    ফেরত pxTCB->px_স্তূপ;
}
/*----------------------------------------------------------*/

uint8_t * px_Task_Get_স্তূপ_Start( TaskHandle_t xTask )
{
    ফেরত (uint8_t *)x_Task_Get_স্তূপ_Start( xTask );
}
/*----------------------------------------------------------*/

#যদি ( INCLUDE_vTaskPrioritySet == 1 )

    শূন্য prvTaskPriorityRaise( prvTaskSavedPriority_t * pxSavedPriority,
                               UBaseType_t uxNewPriority )
    {
        TCB_t * pxTCB;
        UBaseType_t uxPriorityUsedOnEntry;

        configASSERT( ( uxNewPriority < configMAX_PRIORITIES ) );

        /* Ensure the new priority is valid. */
        যদি( uxNewPriority >= ( UBaseType_t ) configMAX_PRIORITIES )
        {
            uxNewPriority = ( UBaseType_t ) configMAX_PRIORITIES - ( UBaseType_t ) 1U;
        }

        #যদি CONFIG_FREERTOS_SMP
            taskENTER_CRITICAL();
        #অন্যথা
            taskENTER_CRITICAL( &xKernelLock );
        #endif
        {
            pxTCB = prvGetTCBFromHandle( ফাঁকা );

            #যদি ( configUSE_MUTEXES == 1 )
            {
                pxSavedPriority->uxPriority = pxTCB->uxPriority;
                pxSavedPriority->uxBasePriority = pxTCB->uxBasePriority;

                /* If uxNewPriority < uxBasePriority, then there is nothing else to
                 * do, as uxBasePriority is always <= uxPriority. */
                যদি( uxNewPriority > pxTCB->uxBasePriority )
                {
                    pxTCB->uxBasePriority = uxNewPriority;

                    /* Remember the task's current priority before attempting to
                     * change it. If the task's current priority is changed, it must
                     * be done so before moving the task between task lists) in order
                     * for the taskRESET_READY_PRIORITY() macro to function correctly. */
                    uxPriorityUsedOnEntry = pxTCB->uxPriority;

                    যদি( uxNewPriority > pxTCB->uxPriority )
                    {
                        pxTCB->uxPriority = uxNewPriority;

                        /* Only reset the event list item value if the value is not
                         * being used for anything else. */
                        যদি( ( list_GET_LIST_ITEM_মান( &( pxTCB->xEventListItem ) ) & task_EVENT_LIST_ITEM_মান_IN_USE ) == 0UL )
                        {
                            list_SET_LIST_ITEM_মান( &( pxTCB->xEventListItem ), ( ( TickType_t ) configMAX_PRIORITIES - ( TickType_t ) uxNewPriority ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */
                        }

                        /* If the task is in the blocked or suspended list we need do
                         * nothing more than change its priority variable. However, if
                         * the task is in a ready list it needs to be removed and placed
                         * in the list appropriate to its new priority. */
                        যদি( listIS_CONTAINED_WITHIN( &( pxReadyTasksLists[ uxPriorityUsedOnEntry ] ), &( pxTCB->xStateListItem ) ) != pdFALSE )
                        {
                            /* The task is currently in its ready list - remove before
                             * adding it to its new ready list.  As we are in a critical
                             * section we can do this even if the scheduler is suspended. */
                            যদি( uxListRemove( &( pxTCB->xStateListItem ) ) == ( UBaseType_t ) 0 )
                            {
                                /* It is known that the task is in its ready list so
                                 * there is no need to check again and the port level
                                 * reset macro can be called directly. */
                                portRESET_READY_PRIORITY( uxPriorityUsedOnEntry, ux_শীর্ষ_Ready_Priority );
                            }

                            prvAddTaskToReadyList( pxTCB );
                        }
                    }
                }
            }
            #অন্যথা /* if ( configUSE_MUTEXES == 1 ) */
            {
                pxSavedPriority->uxPriority = pxTCB->uxPriority;

                যদি( uxNewPriority > pxTCB->uxPriority )
                {
                    vTaskPrioritySet( ফাঁকা, uxNewPriority );
                }
            }
            #endif /* if ( configUSE_MUTEXES == 1 ) */
        }
        #যদি CONFIG_FREERTOS_SMP
            taskEXIT_CRITICAL();
        #অন্যথা
            taskEXIT_CRITICAL( &xKernelLock );
        #endif
    }

#endif /* INCLUDE_vTaskPrioritySet == 1 */
/*----------------------------------------------------------*/

#যদি ( INCLUDE_vTaskPrioritySet == 1 )

    শূন্য prvTaskPriorityRestore( prvTaskSavedPriority_t * pxSavedPriority )
    {
        TCB_t * pxTCB;
        UBaseType_t uxNewPriority;
        UBaseType_t uxPriorityUsedOnEntry;
        UBaseType_t uxBasePriorityUsedOnEntry;
        BaseType_t xYieldRequired = pdFALSE;

        #যদি CONFIG_FREERTOS_SMP
            taskENTER_CRITICAL();
        #অন্যথা
            taskENTER_CRITICAL( &xKernelLock );
        #endif
        {
            pxTCB = prvGetTCBFromHandle( ফাঁকা );

            #যদি ( configUSE_MUTEXES == 1 )
            {
                /* If the saved uxBasePriority == the task's uxBasePriority, it means
                 * that prvTaskPriorityRaise() never raised the task's uxBasePriority.
                 * In that case, there is nothing else to do. */
                যদি( pxSavedPriority->uxBasePriority != pxTCB->uxBasePriority )
                {
                    uxBasePriorityUsedOnEntry = pxTCB->uxBasePriority;
                    pxTCB->uxBasePriority = pxSavedPriority->uxBasePriority;

                    /* Remember the task's current priority before attempting to
                     * change it. If the task's current priority is changed, it must
                     * be done so before moving the task between task lists in order
                     * for the taskRESET_READY_PRIORITY() macro to function correctly. */
                    uxPriorityUsedOnEntry = pxTCB->uxPriority;

                    /* Check if the task inherited a priority after prvTaskPriorityRaise().
                     * If this is the case, there is nothing else to do. The priority
                     * will be restored when the task disinherits its priority. */
                    যদি( pxTCB->uxPriority == uxBasePriorityUsedOnEntry )
                    {
                        যদি( pxTCB->uxMutexesHeld == 0 )
                        {
                            /* The task may have inherited a priority before prvTaskPriorityRaise()
                             * then disinherited a priority after prvTaskPriorityRaise().
                             * Thus we need set the uxPriority to the saved base priority
                             * so that the task's priority gets restored to the priority
                             * before any inheritance or raising. */
                            pxTCB->uxPriority = pxSavedPriority->uxBasePriority;
                        }
                        অন্যথা
                        {
                            /* The task may have inherited a priority before prvTaskPriorityRaise()
                             * was called. Thus, we need to restore uxPriority to the
                             * "saved uxPriority" so that the task still retains that
                             * inherited priority. */
                            pxTCB->uxPriority = pxSavedPriority->uxPriority;
                        }

                        uxNewPriority = pxTCB->uxPriority;

                        যদি( uxNewPriority < uxPriorityUsedOnEntry )
                        {
                            /* Setting the priority of the running task down means
                             * there may now be another task of higher priority that
                             * is ready to execute. */
                            xYieldRequired = pdTRUE;
                        }

                        /* Only reset the event list item value if the value is not
                         * being used for anything else. */
                        যদি( ( list_GET_LIST_ITEM_মান( &( pxTCB->xEventListItem ) ) & task_EVENT_LIST_ITEM_মান_IN_USE ) == 0UL )
                        {
                            list_SET_LIST_ITEM_মান( &( pxTCB->xEventListItem ), ( ( TickType_t ) configMAX_PRIORITIES - ( TickType_t ) uxNewPriority ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */
                        }

                        /* If the task is in the blocked or suspended list we need do
                         * nothing more than change its priority variable. However, if
                         * the task is in a ready list it needs to be removed and placed
                         * in the list appropriate to its new priority. */
                        যদি( listIS_CONTAINED_WITHIN( &( pxReadyTasksLists[ uxPriorityUsedOnEntry ] ), &( pxTCB->xStateListItem ) ) != pdFALSE )
                        {
                            /* The task is currently in its ready list - remove before
                             * adding it to its new ready list.  As we are in a critical
                             * section we can do this even if the scheduler is suspended. */
                            যদি( uxListRemove( &( pxTCB->xStateListItem ) ) == ( UBaseType_t ) 0 )
                            {
                                /* It is known that the task is in its ready list so
                                 * there is no need to check again and the port level
                                 * reset macro can be called directly. */
                                portRESET_READY_PRIORITY( uxPriorityUsedOnEntry, ux_শীর্ষ_Ready_Priority );
                            }

                            prvAddTaskToReadyList( pxTCB );
                        }

                        যদি( xYieldRequired != pdFALSE )
                        {
                            #যদি CONFIG_FREERTOS_SMP
                                taskYIELD_TASK_CORE_IF_USING_PREEMPTION( pxTCB );
                            #অন্যথা
                                taskYIELD_IF_USING_PREEMPTION();
                            #endif
                        }
                    }
                }
            }
            #অন্যথা /* if ( configUSE_MUTEXES == 1 ) */
            {
                vTaskPrioritySet( ফাঁকা, pxSavedPriority->uxPriority );
            }
            #endif /* if ( configUSE_MUTEXES == 1 ) */
        }
        #যদি CONFIG_FREERTOS_SMP
            taskEXIT_CRITICAL();
        #অন্যথা
            taskEXIT_CRITICAL( &xKernelLock );
        #endif
    }

#endif /* ( INCLUDE_vTaskPrioritySet == 1 ) */
/*----------------------------------------------------------*/

/* --------------------------------------------- TLSP Deletion Callbacks -------------------------------------------- */

#যদি CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS

    শূন্য vTaskSetThreadLocalStoragePointerAndDelCallback( TaskHandle_t xTaskToSet,
                                                          BaseType_t x_সূচক,
                                                          শূন্য * pv_মান,
                                                          TlsDeleteCallbackFunction_t pvDelCallback )
    {
        /* If TLSP deletion callbacks are enabled, then configNUM_THREAD_LOCAL_STORAGE_POINTERS
         * is doubled in size so that the latter half of the pvThreadLocalStoragePointers
         * stores the deletion callbacks. */
        যদি( x_সূচক < ( config_অঙ্ক_THREAD_LOCAL_STORAGE_POINTERS / 2 ) )
        {
            TCB_t * pxTCB;

            #যদি ( config_অঙ্ক_CORES > 1 )
            {
                /* For SMP, we need a critical section as another core could also
                 * update this task's TLSP at the same time. */
                #যদি CONFIG_FREERTOS_SMP
                {
                    taskENTER_CRITICAL();
                }
                #অন্যথা /* CONFIG_FREERTOS_SMP */
                {
                    taskENTER_CRITICAL( &xKernelLock );
                }
                #endif /* CONFIG_FREERTOS_SMP */
            }
            #endif /* configNUM_CORES > 1 */

            pxTCB = prvGetTCBFromHandle( xTaskToSet );
            /* Store the TLSP by indexing the first half of the array */
            pxTCB->pvThreadLocalStoragePointers[ x_সূচক ] = pv_মান;

            /* Store the TLSP deletion callback by indexing the second half
             * of the array. */
            pxTCB->pvThreadLocalStoragePointers[ ( x_সূচক + ( config_অঙ্ক_THREAD_LOCAL_STORAGE_POINTERS / 2 ) ) ] = ( শূন্য * ) pvDelCallback;

            #যদি ( config_অঙ্ক_CORES > 1 )
            {
                #যদি CONFIG_FREERTOS_SMP
                {
                    taskEXIT_CRITICAL();
                }
                #অন্যথা /* CONFIG_FREERTOS_SMP */
                {
                    taskEXIT_CRITICAL( &xKernelLock );
                }
                #endif /* CONFIG_FREERTOS_SMP */
            }
            #endif /* configNUM_CORES > 1 */
        }
    }

#endif /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */
/*----------------------------------------------------------*/

/* ----------------------------------------------------- Newlib ----------------------------------------------------- */

#যদি ( configUSE_NEWLIB_REENTRANT == 1 )

/**
 * @brief Get reentrancy structure of the current task
 *
 * - This function is required by newlib (when __DYNAMIC_REENT__ is enabled)
 * - It will return a pointer to the current task's reent struct
 * - If FreeRTOS is not running, it will return the global reent struct
 *
 * @return Pointer to a the (current taks's)/(global) reent struct
 */
    কাঠামো _reent * __getreent( শূন্য )
    {
        /* No lock needed because if this changes, we won't be running anymore. */
        TCB_t * pxCurTask = ( TCB_t * ) xTaskGetCurrentTaskHandle();
        কাঠামো _reent * ret;

        যদি( pxCurTask == ফাঁকা )
        {
            /* No task running. Return global struct. */
            ret = _GLOBAL_REENT;
        }
        অন্যথা
        {
            /* We have a currently executing task. Return its reentrant struct. */
            ret = &pxCurTask->xTLSBlock;
        }

        ফেরত ret;
    }

#endif /* configUSE_NEWLIB_REENTRANT == 1 */

/* -------------------------------------------------- Task Snapshot ------------------------------------------------- */

/**
 * @brief List of all task lists in FreeRTOS
 *
 * @note There are currently differing number of task list between SMP FreeRTOS and ESP-IDF FreeRTOS
 */
স্থির List_t * non_ready_task_lists[] =
{
    #ifdef CONFIG_FREERTOS_SMP
        &xPendingReadyList,
    #অন্যথা /* CONFIG_FREERTOS_SMP */
        &xPendingReadyList[ 0 ],
        #ifndef CONFIG_FREERTOS_UNICORE
            &xPendingReadyList[ 1 ],
        #endif /* CONFIG_FREERTOS_UNICORE */
    #endif /* CONFIG_FREERTOS_SMP */
    &xDelayedTaskList1,
    &xDelayedTaskList2,
    #যদি ( INCLUDE_vTaskDelete == 1 )
        &xTasksWaitingTermination,
    #endif
    #যদি ( INCLUDE_vTaskSuspend == 1 )
        &xSuspendedTaskList,
    #endif
};
/*----------------------------------------------------------*/

/**
 * @brief Get the task list from state lists by index
 *
 * - This function returns the task list based on the specified index.
 * - The index is relative to the below order of the task state lists
 *      - Ready lists (highest to lowers priority)
 *      - Pending ready list(s)
 *      - Delayed list 1
 *      - Delayed list 2
 *      - Waiting termination list
 *      - Suspended list
 *
 * @param uxListIndex The index of the desired task list.
 * @return A pointer to the task list at the specified index.
 *         Returns NULL if the index is out of bounds or list is corrupted.
 */
স্থির List_t * px_Get_Task_List_By_সূচক( UBaseType_t ux_List_সূচক )
{
    List_t * pxTaskList;
    ধ্রুবক size_t xNonReadyTaskListsCnt = ( আকার( non_ready_task_lists ) / আকার( List_t * ) );

    যদি( ux_List_সূচক < configMAX_PRIORITIES )
    {
        pxTaskList = &pxReadyTasksLists[ configMAX_PRIORITIES - 1 - ux_List_সূচক ];
    }
    অন্যথা যদি( ux_List_সূচক < configMAX_PRIORITIES + xNonReadyTaskListsCnt )
    {
        pxTaskList = non_ready_task_lists[ ux_List_সূচক - configMAX_PRIORITIES ];
    }
    অন্যথা
    {
        pxTaskList = ফাঁকা;
    }

    /* sanity check */
    যদি( pxTaskList )
    {
        যদি( !portVALID_LIST_MEM( pxTaskList ) )
        {
            pxTaskList = ফাঁকা;
        }
    }

    ফেরত pxTaskList;
}
/*----------------------------------------------------------*/

/**
 * @brief Get the total count of task lists.
 *
 * The count includes both the ready task lists (based on priority) and non-ready task lists.
 *
 * @return The total count of task lists.
 *
 */
স্থির অন্তঃরেখ UBaseType_t px_Get_Task_List_গুনতি( শূন্য )
{
    ফেরত configMAX_PRIORITIES + ( আকার( non_ready_task_lists ) / আকার( List_t * ) );
}
/*----------------------------------------------------------*/

/**
 * @brief Get the next task using the task iterator.
 *
 * This function retrieves the next task in the traversal sequence.
 *
 * @param xIterator Pointer to the task iterator structure.
 *
 * @return Index of the current task list. Returns -1 if all tasks have been traversed.
 *
 * @note The task iterator keeps track of the current state during task traversal,
 *       including the index of the current task list and the pointer of the next task list item.
 *       When all tasks have been traversed, this function returns -1.
 *       If a broken or corrupted task is encountered, the task handle is set to NULL.
 */
পূর্ণসংখ্যা x_Task_Get_পরবর্তী( TaskIterator_t * xIterator )
{
    যদি( !xIterator )
    {
        ফেরত -1;
    }

    ListItem_t * px_পরবর্তী_List_Item = xIterator->px_পরবর্তী_List_Item;
    UBaseType_t uxCurListIdx = xIterator->ux_Current_List_সূচক;
    UBaseType_t uxMaxListIdx = px_Get_Task_List_গুনতি();

    জন্য( ; uxCurListIdx < uxMaxListIdx; ++uxCurListIdx )
    {
        List_t * pxCurrentTaskList = px_Get_Task_List_By_সূচক( uxCurListIdx );

        যদি( !pxCurrentTaskList || ( list_CURRENT_LIST_দৈর্ঘ্য( pxCurrentTaskList ) == 0 ) )
        {
            চালু;
        }

        ধ্রুবক ListItem_t * pxCurrListItem = listGET_END_MARKER( pxCurrentTaskList );

        যদি( !px_পরবর্তী_List_Item )
        {
            /* We are here if the traversal starts from the beginning or when we finish traversing
             * for one of the state lists
             */
            px_পরবর্তী_List_Item = list_GET_পরবর্তী( pxCurrListItem );
        }

        যদি( !portVALID_LIST_MEM( px_পরবর্তী_List_Item ) )
        {
            /* Nothing to do with the corrupted list item. We will skip to the next task state list.
             * pxNextListItem should be NULL at the beginning of each task list.
             */
            px_পরবর্তী_List_Item = ফাঁকা;
            চালু;
        }

        TCB_t * pxTCB = ( TCB_t * ) listGET_LIST_ITEM_OWNER( px_পরবর্তী_List_Item );

        যদি( !portVALID_TCB_MEM( pxTCB ) )
        {
            pxTCB = ফাঁকা;
        }

        xIterator->pxTaskHandle = pxTCB;
        xIterator->ux_Current_List_সূচক = uxCurListIdx;

        যদি( pxCurrListItem->pxPrevious == px_পরবর্তী_List_Item )
        {
            /* If this is the last item of the current state list */
            xIterator->ux_Current_List_সূচক++;
            xIterator->px_পরবর্তী_List_Item = ফাঁকা;
        }
        অন্যথা
        {
            xIterator->px_পরবর্তী_List_Item = list_GET_পরবর্তী( px_পরবর্তী_List_Item );
        }

        ফেরত uxCurListIdx;
    }

    ফেরত -1; /* end of the task list */
}
/*----------------------------------------------------------*/

BaseType_t vTaskGetSnapshot( TaskHandle_t pxTask,
                             TaskSnapshot_t * pxTaskSnapshot )
{
    ESP_STATIC_ANALYZER_CHECK(!pxTask, pdFALSE);

    যদি( ( portVALID_TCB_MEM( pxTask ) == false ) || ( pxTaskSnapshot == ফাঁকা ) )
    {
        ফেরত pdFALSE;
    }

    TCB_t * pxTCB = ( TCB_t * ) pxTask;
    pxTaskSnapshot->pxTCB = pxTCB;
    pxTaskSnapshot->px_শীর্ষ_Of_স্তূপ = ( স্তূপ_Type_t * ) pxTCB->px_শীর্ষ_Of_স্তূপ;
    pxTaskSnapshot->px_End_Of_স্তূপ = ( স্তূপ_Type_t * ) pxTCB->px_End_Of_স্তূপ;
    ফেরত pdTRUE;
}
/*----------------------------------------------------------*/

UBaseType_t uxTaskGetSnapshotAll( TaskSnapshot_t * ধ্রুবক pxTaskSnapshotArray,
                                  ধ্রুবক UBaseType_t ux_Array_দৈর্ঘ্য,
                                  UBaseType_t * ধ্রুবক pxTCBSize )
{
    UBaseType_t ux_Array_অঙ্ক_Filled = 0;

    /* Traverse all of the tasks lists */
    TaskIterator_t xTaskIter = { 0 }; /* Point to the first task list */

    যখন( x_Task_Get_পরবর্তী( &xTaskIter ) != -1 && ux_Array_অঙ্ক_Filled < ux_Array_দৈর্ঘ্য )
    {
        vTaskGetSnapshot( xTaskIter.pxTaskHandle, &pxTaskSnapshotArray[ ux_Array_অঙ্ক_Filled ] );
        ux_Array_অঙ্ক_Filled++;
    }

    যদি( pxTCBSize != ফাঁকা )
    {
        *pxTCBSize = আকার( TCB_t );
    }

    ফেরত ux_Array_অঙ্ক_Filled;
}
/*----------------------------------------------------------*/

/* ----------------------------------------------------- Misc ----------------------------------------------------- */

শূন্য * pvTaskGetCurrentTCBForCore( BaseType_t xCoreID )
{
    শূন্য * pvRet;

    configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

    #যদি CONFIG_FREERTOS_SMP
        /* SMP FreeRTOS defines pxCurrentTCB as a macro function call */
        pvRet = ( শূন্য * ) pxCurrentTCB;
    #অন্যথা /* CONFIG_FREERTOS_SMP */
        pvRet = ( শূন্য * ) pxCurrentTCBs[ xCoreID ];
    #endif /* CONFIG_FREERTOS_SMP */
    ফেরত pvRet;
}

/* ----------------------------------------------------- OpenOCD ---------------------------------------------------- */

#যদি CONFIG_FREERTOS_DEBUG_OCDAWARE

/**
 * Debug param indexes. DO NOT change the order. OpenOCD uses the same indexes
 * Entries in FreeRTOS_openocd_params must match the order of these indexes
 */
    গণনা
    {
        ESP_FREERTOS_DEBUG_TABLE_আকার = 0,
        ESP_FREERTOS_DEBUG_TABLE_VERSION,
        ESP_FREERTOS_DEBUG_KERNEL_VER_MAJOR,
        ESP_FREERTOS_DEBUG_KERNEL_VER_MINOR,
        ESP_FREERTOS_DEBUG_KERNEL_VER_BUILD,
        ESP_FREERTOS_DEBUG_UX_শীর্ষ_USED_PIORITY,
        ESP_FREERTOS_DEBUG_PX_শীর্ষ_OF_স্তূপ,
        ESP_FREERTOS_DEBUG_PC_TASK_NAME,
        ESP_FREERTOS_DEBUG_LIST_আকার,
        ESP_FREERTOS_DEBUG_LIST_অঙ্ক_বস্তুসমূহ,
        ESP_FREERTOS_DEBUG_LIST_END,
        ESP_FREERTOS_DEBUG_LIST_END_পূর্ব,
        ESP_FREERTOS_DEBUG_LIST_ITEM_পূর্ব,
        ESP_FREERTOS_DEBUG_LIST_ITEM_OWNER,
        ESP_FREERTOS_DEBUG_TASK_গুনতি_WIDTH,
        ESP_FREERTOS_DEBUG_PTR_WIDTH,
        /* New entries must be inserted here */
        ESP_FREERTOS_DEBUG_TABLE_END,
    };

    ধ্রুবক DRAM_ATTR uint8_t FreeRTOS_openocd_params[ ESP_FREERTOS_DEBUG_TABLE_END ] =
    {
        ESP_FREERTOS_DEBUG_TABLE_END, /* table size */
        1,                            /* table version */
        tskKERNEL_VERSION_MAJOR,
        tskKERNEL_VERSION_MINOR,
        tskKERNEL_VERSION_BUILD,
        configMAX_PRIORITIES - 1,                   /* uxTopUsedPriority */
        offsetof( TCB_t, px_শীর্ষ_Of_স্তূপ ),            /* thread_stack_offset; */
        offsetof( TCB_t, pcTaskName ),              /* thread_name_offset; */
        আকার( List_t ),                           /* list_width */
        offsetof( List_t, ux_সংখ্যা_Of_বস্তুসমূহ ),        /* list_item_num */
        offsetof( List_t, xListEnd ),               /* list_end_offset */
        offsetof( List_t, xListEnd.pxPrevious ),    /* list_next_offset */
        offsetof( ListItem_t, pxPrevious ),         /* list_elem_next_offset */
        offsetof( ListItem_t, pvOwner ),            /* list_elem_content_offset */
        আকার( UBaseType_t ),                      /* task_count_width */
        আকার( শূন্য * )                            /* ptr_width */
    };

#endif /* CONFIG_FREERTOS_DEBUG_OCDAWARE */
/*----------------------------------------------------------*/

/* ----------------------------------------------------- PSRAM ---------------------------------------------------- */

#যদি CONFIG_SPIRAM

    #যদি CONFIG_FREERTOS_SMP
        BaseType_t prvTaskCreateDynamicAffinitySetWithCaps( TaskFunction_t pxTaskCode,
                                                            ধ্রুবক অক্ষর * ধ্রুবক pcName,
                                                            ধ্রুবক config_স্তূপ_DEPTH_TYPE us_স্তূপ_Depth,
                                                            শূন্য * ধ্রুবক pvParameters,
                                                            UBaseType_t uxPriority,
                                                            UBaseType_t uxCoreAffinityMask,
                                                            UBaseType_t ux_স্তূপ_Memory_Caps,
                                                            TaskHandle_t * ধ্রুবক pxCreatedTask )
    #অন্যথা /* CONFIG_FREERTOS_SMP */
        BaseType_t prvTaskCreateDynamicPinnedToCoreWithCaps( TaskFunction_t pxTaskCode,
                                                             ধ্রুবক অক্ষর * ধ্রুবক pcName,
                                                             ধ্রুবক config_স্তূপ_DEPTH_TYPE us_স্তূপ_Depth,
                                                             শূন্য * ধ্রুবক pvParameters,
                                                             UBaseType_t uxPriority,
                                                             ধ্রুবক BaseType_t xCoreID,
                                                             UBaseType_t ux_স্তূপ_Memory_Caps,
                                                             TaskHandle_t * ধ্রুবক pxCreatedTask )
    #endif /* CONFIG_FREERTOS_SMP */
    {
        TCB_t * pxNewTCB;
        BaseType_t xReturn;

        স্তূপ_Type_t * px_স্তূপ;

        configASSERT( ux_স্তূপ_Memory_Caps & ( MALLOC_CAP_8BIT ) );
        configASSERT( ( ux_স্তূপ_Memory_Caps & MALLOC_CAP_SPIRAM ) ||
                      ( ux_স্তূপ_Memory_Caps & MALLOC_CAP_INTERNAL ) );
        #যদি ( !CONFIG_FREERTOS_SMP )
        {
            configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );
        }
        #endif /* !CONFIG_FREERTOS_SMP */

        /* Allocate space for the stack used by the task being created. */
        px_স্তূপ = heap_caps_malloc( ( ( ( size_t ) us_স্তূপ_Depth ) * আকার( স্তূপ_Type_t ) ), ux_স্তূপ_Memory_Caps );

        যদি( px_স্তূপ != ফাঁকা )
        {
            /* Allocate space for the TCB. */
            pxNewTCB = ( TCB_t * ) pvPortMalloc( আকার( TCB_t ) );

            যদি( pxNewTCB != ফাঁকা )
            {
                memset( ( শূন্য * ) pxNewTCB, 0x00, আকার( TCB_t ) );

                /* Store the stack location in the TCB. */
                pxNewTCB->px_স্তূপ = px_স্তূপ;
            }
            অন্যথা
            {
                /* The stack cannot be used as the TCB has not been created. Free it. */
                heap_caps_free( px_স্তূপ );
            }
        }
        অন্যথা
        {
            pxNewTCB = ফাঁকা;
        }

        যদি( pxNewTCB != ফাঁকা )
        {
            #যদি ( tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE != 0 )
            {
                /* Tasks can be created statically or dynamically, so note this
                 * task was created dynamically in case it is later deleted. */
                pxNewTCB->ucStaticallyAllocated = tsk_DYNAMICALLY_ALLOCATED_স্তূপ_AND_TCB;
            }
            #endif /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

            #যদি CONFIG_FREERTOS_SMP
            {
                prvInitialiseNewTask( pxTaskCode, pcName, ( uint32_t ) us_স্তূপ_Depth, pvParameters, uxPriority, pxCreatedTask, pxNewTCB, ফাঁকা );
                #যদি ( ( config_অঙ্ক_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
                {
                    /* Set the task's affinity before scheduling it */
                    pxNewTCB->uxCoreAffinityMask = uxCoreAffinityMask;
                }
                #endif /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            }
            #অন্যথা /* CONFIG_FREERTOS_SMP */
            {
                prvInitialiseNewTask( pxTaskCode, pcName, ( uint32_t ) us_স্তূপ_Depth, pvParameters, uxPriority, pxCreatedTask, pxNewTCB, ফাঁকা, xCoreID );
            }
            #endif /* CONFIG_FREERTOS_SMP */

            prvAddNewTaskToReadyList( pxNewTCB );
            xReturn = pdPASS;
        }
        অন্যথা
        {
            xReturn = errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY;
        }

        ফেরত xReturn;
    }

#endif /* CONFIG_SPIRAM */
/*----------------------------------------------------------*/
