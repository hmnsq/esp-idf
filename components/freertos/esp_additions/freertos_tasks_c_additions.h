/*
 * SPDX-FileCopyrightText: 2015-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#അന്തര്ഭാവ "sdkconfig.h"
#അന്തര്ഭാവ "esp_assert.h"
#അന്തര്ഭാവ "esp_heap_caps.h"
#അന്തര്ഭാവ "esp_compiler.h"
#അന്തര്ഭാവ "freertos/idf_additions.h"
#അന്തര്ഭാവ "freertos/freertos_debug.h"
#അന്തര്ഭാവ "esp_private/freertos_idf_additions_priv.h"

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
_Static_assert( offsetof( StaticTask_t, pxDummy6 ) == offsetof( TCB_t, px_അട്ടി ) );
_Static_assert( offsetof( StaticTask_t, pxDummy8 ) == offsetof( TCB_t, px_End_Of_അട്ടി ) );
#എങ്കിൽ !CONFIG_IDF_TARGET_LINUX    // Disabled for linux builds due to differences in types
_Static_assert( tskNO_AFFINITY == ( BaseType_t ) CONFIG_FREERTOS_NO_AFFINITY, "CONFIG_FREERTOS_NO_AFFINITY must be the same as tskNO_AFFINITY" );
#endif

/* ------------------------------------------------- Kernel Control ------------------------------------------------- */

#എങ്കിൽ ( !CONFIG_FREERTOS_SMP && ( config_അക്കം_CORES > 1 ) )

/*
 * Wrapper function to take "xKerneLock"
 */
    ശൂന്യം prvTakeKernelLock( ശൂന്യം )
    {
        /* We call the tasks.c critical section macro to take xKernelLock */
        taskENTER_CRITICAL( &xKernelLock );
    }

#endif /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#എങ്കിൽ ( !CONFIG_FREERTOS_SMP && ( config_അക്കം_CORES > 1 ) )

/*
 * Wrapper function to release "xKerneLock"
 */
    ശൂന്യം prvReleaseKernelLock( ശൂന്യം )
    {
        /* We call the tasks.c critical section macro to release xKernelLock */
        taskEXIT_CRITICAL( &xKernelLock );
    }

#endif /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#എങ്കിൽ ( CONFIG_FREERTOS_SMP && ( config_അക്കം_CORES > 1 ) )

/*
 * Workaround for non-thread safe multi-core OS startup (see IDF-4524)
 */
    ശൂന്യം prvStartSchedulerOtherCores( ശൂന്യം )
    {
        /* This function is always called with interrupts disabled*/
        xSchedulerRunning = pdTRUE;
    }

#endif /* ( CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#എങ്കിൽ ( !CONFIG_FREERTOS_SMP && ( config_അക്കം_CORES > 1 ) )

    BaseType_t xTaskIncrementTickOtherCores( ശൂന്യം )
    {
        /* Minor optimization. This function can never switch cores mid
         * execution */
        BaseType_t xCoreID = portGET_CORE_ID();
        BaseType_t xSwitchRequired = pdFALSE;

        /* This function should never be called by Core 0. */
        configASSERT( xCoreID != 0 );

        /* Called by the portable layer each time a tick interrupt occurs
         * on a core other than core 0. */
        traceTASK_INCREMENT_TICK( x_Tick_എണ്ണം );

        എങ്കിൽ( uxSchedulerSuspended[ xCoreID ] == ( UBaseType_t ) 0U )
        {
            /* We need take the kernel lock here as we are about to access
             * kernel data structures. */
            taskENTER_CRITICAL_ISR( &xKernelLock );

            /* Tasks of equal priority to the currently running task will share
             * processing time (time slice) if preemption is on, and the application
             * writer has not explicitly turned time slicing off. */
            #എങ്കിൽ ( ( configUSE_PREEMPTION == 1 ) && ( configUSE_TIME_SLICING == 1 ) )
            {
                എങ്കിൽ( listCURRENT_LIST_LENGTH( &( pxReadyTasksLists[ pxCurrentTCBs[ xCoreID ]->uxPriority ] ) ) > ( UBaseType_t ) 1 )
                {
                    xSwitchRequired = pdTRUE;
                }
                അല്ലെങ്കിൽ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            #endif /* ( ( configUSE_PREEMPTION == 1 ) && ( configUSE_TIME_SLICING == 1 ) ) */

            /* Release the previously taken kernel lock as we have finished
             * accessing the kernel data structures. */
            taskEXIT_CRITICAL_ISR( &xKernelLock );

            #എങ്കിൽ ( configUSE_PREEMPTION == 1 )
            {
                എങ്കിൽ( xYieldPending[ xCoreID ] != pdFALSE )
                {
                    xSwitchRequired = pdTRUE;
                }
                അല്ലെങ്കിൽ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            #endif /* configUSE_PREEMPTION */
        }

        #എങ്കിൽ ( configUSE_TICK_HOOK == 1 )
        {
            vApplicationTickHook();
        }
        #endif

        തിരികെ xSwitchRequired;
    }

#endif /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

/* -------------------------------------------------- Task Creation ------------------------------------------------- */

#എങ്കിൽ ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )

    BaseType_t xTaskCreatePinnedToCore( TaskFunction_t pxTaskCode,
                                        സ്ഥിരം അക്ഷരം * സ്ഥിരം pcName,
                                        സ്ഥിരം uint32_t us_അട്ടി_Depth,
                                        ശൂന്യം * സ്ഥിരം pvParameters,
                                        UBaseType_t uxPriority,
                                        TaskHandle_t * സ്ഥിരം pxCreatedTask,
                                        സ്ഥിരം BaseType_t xCoreID )
    {
        BaseType_t xReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );

        #എങ്കിൽ CONFIG_FREERTOS_SMP
        {
            /* If using Amazon SMP FreeRTOS. This function is just a wrapper around
             * xTaskCreate() or xTaskCreateAffinitySet(). */
            #എങ്കിൽ ( ( config_അക്കം_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
            {
                /* Convert xCoreID into an affinity mask */
                UBaseType_t uxCoreAffinityMask;

                /* Bit shifting << xCoreID is only valid if we have less than
                 * 32 cores. */
                ESP_STATIC_ASSERT( config_അക്കം_CORES < 32 );

                എങ്കിൽ( xCoreID == tskNO_AFFINITY )
                {
                    uxCoreAffinityMask = tskNO_AFFINITY;
                }
                അല്ലെങ്കിൽ
                {
                    uxCoreAffinityMask = ( 1 << xCoreID );
                }

                xReturn = xTaskCreateAffinitySet( pxTaskCode, pcName, us_അട്ടി_Depth, pvParameters, uxPriority, uxCoreAffinityMask, pxCreatedTask );
            }
            #അല്ലെങ്കിൽ /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            {
                xReturn = xTaskCreate( pxTaskCode, pcName, us_അട്ടി_Depth, pvParameters, uxPriority, pxCreatedTask );
            }
            #endif /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
        }
        #അല്ലെങ്കിൽ /* CONFIG_FREERTOS_SMP */
        {
            TCB_t * pxNewTCB;

            /* If the stack grows down then allocate the stack then the TCB so the
             * stack does not grow into the TCB.  Likewise if the stack grows up
             * then allocate the TCB then the stack. */
            #എങ്കിൽ ( port_അട്ടി_GROWTH > 0 )
            {
                /* Allocate space for the TCB.  Where the memory comes from depends on
                 * the implementation of the port malloc function and whether or not static
                 * allocation is being used. */
                pxNewTCB = ( TCB_t * ) pvPortMalloc( വലുപ്പം( TCB_t ) );

                എങ്കിൽ( pxNewTCB != ശൂന്യ )
                {
                    memset( ( ശൂന്യം * ) pxNewTCB, 0x00, വലുപ്പം( TCB_t ) );

                    /* Allocate space for the stack used by the task being created.
                     * The base of the stack memory stored in the TCB so the task can
                     * be deleted later if required. */
                    pxNewTCB->px_അട്ടി = ( അട്ടി_Type_t * ) pv_Port_Malloc_അട്ടി( ( ( ( size_t ) us_അട്ടി_Depth ) * വലുപ്പം( അട്ടി_Type_t ) ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */

                    എങ്കിൽ( pxNewTCB->px_അട്ടി == ശൂന്യ )
                    {
                        /* Could not allocate the stack.  Delete the allocated TCB. */
                        vPortFree( pxNewTCB );
                        pxNewTCB = ശൂന്യ;
                    }
                }
            }
            #അല്ലെങ്കിൽ /* portSTACK_GROWTH */
            {
                അട്ടി_Type_t * px_അട്ടി;

                /* Allocate space for the stack used by the task being created. */
                px_അട്ടി = pv_Port_Malloc_അട്ടി( ( ( ( size_t ) us_അട്ടി_Depth ) * വലുപ്പം( അട്ടി_Type_t ) ) ); /*lint !e9079 All values returned by pvPortMalloc() have at least the alignment required by the MCU's stack and this allocation is the stack. */

                എങ്കിൽ( px_അട്ടി != ശൂന്യ )
                {
                    /* Allocate space for the TCB. */
                    pxNewTCB = ( TCB_t * ) pvPortMalloc( വലുപ്പം( TCB_t ) ); /*lint !e9087 !e9079 All values returned by pvPortMalloc() have at least the alignment required by the MCU's stack, and the first member of TCB_t is always a pointer to the task's stack. */

                    എങ്കിൽ( pxNewTCB != ശൂന്യ )
                    {
                        memset( ( ശൂന്യം * ) pxNewTCB, 0x00, വലുപ്പം( TCB_t ) );

                        /* Store the stack location in the TCB. */
                        pxNewTCB->px_അട്ടി = px_അട്ടി;
                    }
                    അല്ലെങ്കിൽ
                    {
                        /* The stack cannot be used as the TCB was not created.  Free
                         * it again. */
                        v_Port_Free_അട്ടി( px_അട്ടി );
                    }
                }
                അല്ലെങ്കിൽ
                {
                    pxNewTCB = ശൂന്യ;
                }
            }
            #endif /* portSTACK_GROWTH */

            എങ്കിൽ( pxNewTCB != ശൂന്യ )
            {
                #എങ്കിൽ ( tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE != 0 ) /*lint !e9029 !e731 Macro has been consolidated for readability reasons. */
                {
                    /* Tasks can be created statically or dynamically, so note this
                     * task was created dynamically in case it is later deleted. */
                    pxNewTCB->ucStaticallyAllocated = tsk_DYNAMICALLY_ALLOCATED_അട്ടി_AND_TCB;
                }
                #endif /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

                prvInitialiseNewTask( pxTaskCode, pcName, ( uint32_t ) us_അട്ടി_Depth, pvParameters, uxPriority, pxCreatedTask, pxNewTCB, ശൂന്യ, xCoreID );
                prvAddNewTaskToReadyList( pxNewTCB );
                xReturn = pdPASS;
            }
            അല്ലെങ്കിൽ
            {
                xReturn = errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY;
            }
        }
        #endif /* CONFIG_FREERTOS_SMP */

        തിരികെ xReturn;
    }

#endif /* ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#എങ്കിൽ ( configSUPPORT_STATIC_ALLOCATION == 1 )

    TaskHandle_t xTaskCreateStaticPinnedToCore( TaskFunction_t pxTaskCode,
                                                സ്ഥിരം അക്ഷരം * സ്ഥിരം pcName,
                                                സ്ഥിരം uint32_t ul_അട്ടി_Depth,
                                                ശൂന്യം * സ്ഥിരം pvParameters,
                                                UBaseType_t uxPriority,
                                                അട്ടി_Type_t * സ്ഥിരം pux_അട്ടി_Buffer,
                                                StaticTask_t * സ്ഥിരം pxTaskBuffer,
                                                സ്ഥിരം BaseType_t xCoreID )
    {
        TaskHandle_t xReturn;

        configASSERT( port_VALID_അട്ടി_MEM( pux_അട്ടി_Buffer ) );
        configASSERT( portVALID_TCB_MEM( pxTaskBuffer ) );
        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );

        #എങ്കിൽ CONFIG_FREERTOS_SMP
        {
            /* If using Amazon SMP FreeRTOS. This function is just a wrapper around
             * xTaskCreateStatic() or xTaskCreateStaticAffinitySet(). */
            #എങ്കിൽ ( ( config_അക്കം_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
            {
                /* Convert xCoreID into an affinity mask */
                UBaseType_t uxCoreAffinityMask;

                /* Bit shifting << xCoreID is only valid if we have less than
                 * 32 cores. */
                ESP_STATIC_ASSERT( config_അക്കം_CORES < 32 );

                എങ്കിൽ( xCoreID == tskNO_AFFINITY )
                {
                    uxCoreAffinityMask = tskNO_AFFINITY;
                }
                അല്ലെങ്കിൽ
                {
                    uxCoreAffinityMask = ( 1 << xCoreID );
                }

                xReturn = xTaskCreateStaticAffinitySet( pxTaskCode, pcName, ul_അട്ടി_Depth, pvParameters, uxPriority, pux_അട്ടി_Buffer, pxTaskBuffer, uxCoreAffinityMask );
            }
            #അല്ലെങ്കിൽ /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            {
                xReturn = xTaskCreateStatic( pxTaskCode, pcName, ul_അട്ടി_Depth, pvParameters, uxPriority, pux_അട്ടി_Buffer, pxTaskBuffer );
            }
            #endif /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
        }
        #അല്ലെങ്കിൽ /* CONFIG_FREERTOS_SMP */
        {
            TCB_t * pxNewTCB;

            #എങ്കിൽ ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticTask_t equals the size of the real task
                 * structure. */
                അസ്ഥിര size_t xSize = വലുപ്പം( StaticTask_t );
                configASSERT( xSize == വലുപ്പം( TCB_t ) );
                ( ശൂന്യം ) xSize; /* Prevent lint warning when configASSERT() is not used. */
            }
            #endif /* configASSERT_DEFINED */

            എങ്കിൽ( ( pxTaskBuffer != ശൂന്യ ) && ( pux_അട്ടി_Buffer != ശൂന്യ ) )
            {
                /* The memory used for the task's TCB and stack are passed into this
                 * function - use them. */
                pxNewTCB = ( TCB_t * ) pxTaskBuffer; /*lint !e740 !e9087 Unusual cast is ok as the structures are designed to have the same alignment, and the size is checked by an assert. */
                memset( ( ശൂന്യം * ) pxNewTCB, 0x00, വലുപ്പം( TCB_t ) );
                pxNewTCB->px_അട്ടി = ( അട്ടി_Type_t * ) pux_അട്ടി_Buffer;

                #എങ്കിൽ ( tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE != 0 ) /*lint !e731 !e9029 Macro has been consolidated for readability reasons. */
                {
                    /* Tasks can be created statically or dynamically, so note this
                     * task was created statically in case the task is later deleted. */
                    pxNewTCB->ucStaticallyAllocated = tsk_STATICALLY_ALLOCATED_അട്ടി_AND_TCB;
                }
                #endif /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

                prvInitialiseNewTask( pxTaskCode, pcName, ul_അട്ടി_Depth, pvParameters, uxPriority, &xReturn, pxNewTCB, ശൂന്യ, xCoreID );
                prvAddNewTaskToReadyList( pxNewTCB );
            }
            അല്ലെങ്കിൽ
            {
                xReturn = ശൂന്യ;
            }
        }
        #endif /* CONFIG_FREERTOS_SMP */

        തിരികെ xReturn;
    }

#endif /* ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#എങ്കിൽ ( configUSE_TIMERS == 1 )

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
    BaseType_t __attribute__( ( weak ) ) xTimerCreateTimerTask( ശൂന്യം )
    {
        തിരികെ pdPASS;
    }

#endif /* configUSE_TIMERS */
/*----------------------------------------------------------*/

/* ------------------------------------------------- Task Utilities ------------------------------------------------- */

BaseType_t xTaskGetCoreID( TaskHandle_t xTask )
{
    BaseType_t xReturn;

    #എങ്കിൽ ( config_അക്കം_CORES > 1 )
    {
        #എങ്കിൽ CONFIG_FREERTOS_SMP
            UBaseType_t uxCoreAffinityMask;

            /* Get the core affinity mask and convert it to an ID */
            uxCoreAffinityMask = vTaskCoreAffinityGet( xTask );

            /* If the task is not pinned to a particular core, treat it as tskNO_AFFINITY */
            എങ്കിൽ( uxCoreAffinityMask & ( uxCoreAffinityMask - 1 ) ) /* If more than one bit set */
            {
                xReturn = tskNO_AFFINITY;
            }
            അല്ലെങ്കിൽ
            {
                പൂർണ്ണസംഖ്യ iIndexPlusOne = __builtin_ffs( uxCoreAffinityMask );
                പ്രതിജ്ഞാ( iIndexPlusOne >= 1 );
                xReturn = iIndexPlusOne - 1;
            }
        #അല്ലെങ്കിൽ /* CONFIG_FREERTOS_SMP */
            TCB_t * pxTCB;

            pxTCB = prvGetTCBFromHandle( xTask );

            xReturn = pxTCB->xCoreID;
        #endif /* CONFIG_FREERTOS_SMP */
    }
    #അല്ലെങ്കിൽ /* configNUM_CORES > 1 */
    {
        /* Single-core. Just return a core ID of 0 */
        xReturn = 0;
    }
    #endif /* configNUM_CORES > 1 */

    തിരികെ xReturn;
}
/*----------------------------------------------------------*/

#എങ്കിൽ ( ( !CONFIG_FREERTOS_SMP ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

    TaskHandle_t xTaskGetIdleTaskHandleForCore( BaseType_t xCoreID )
    {
        /* If xTaskGetIdleTaskHandle() is called before the scheduler has been
         * started, then xIdleTaskHandle will be NULL. */
        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );
        configASSERT( ( xIdleTaskHandle[ xCoreID ] != ശൂന്യ ) );
        തിരികെ xIdleTaskHandle[ xCoreID ];
    }

#endif /* ( ( !CONFIG_FREERTOS_SMP ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*----------------------------------------------------------*/

#എങ്കിൽ ( ( !CONFIG_FREERTOS_SMP ) && ( ( INCLUDE_xTaskGetCurrentTaskHandle == 1 ) || ( configUSE_MUTEXES == 1 ) ) )

    TaskHandle_t xTaskGetCurrentTaskHandleForCore( BaseType_t xCoreID )
    {
        TaskHandle_t xReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

        #എങ്കിൽ ( CONFIG_FREERTOS_SMP )
        {
            xReturn = xTaskGetCurrentTaskHandleCPU( ( UBaseType_t ) xCoreID );
        }
        #അല്ലെങ്കിൽ /* CONFIG_FREERTOS_SMP */
        {
            /* A critical section is not required as this function does not
             * guarantee that the TCB will still be valid when this function
             * returns. */
            xReturn = pxCurrentTCBs[ xCoreID ];
        }
        #endif /* CONFIG_FREERTOS_SMP */

        തിരികെ xReturn;
    }

#endif /* ( ( !CONFIG_FREERTOS_SMP ) && ( ( INCLUDE_xTaskGetCurrentTaskHandle == 1 ) || ( configUSE_MUTEXES == 1 ) ) ) */
/*----------------------------------------------------------*/

#എങ്കിൽ ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

    configRUN_TIME_COUNTER_TYPE ulTaskGetIdleRunTimeCounterForCore( BaseType_t xCoreID )
    {
        configRUN_TIME_COUNTER_TYPE ulRunTimeCounter;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

        /* For SMP, we need to take the kernel lock here as we are about to
         * access kernel data structures. */
        prvENTER_CRITICAL_SMP_ONLY( &xKernelLock );
        {
            ulRunTimeCounter = xIdleTaskHandle[ xCoreID ]->ulRunTimeCounter;
        }
        /* Release the previously taken kernel lock. */
        prvEXIT_CRITICAL_SMP_ONLY( &xKernelLock );

        തിരികെ ulRunTimeCounter;
    }

#endif /* ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*----------------------------------------------------------*/

#എങ്കിൽ ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

    configRUN_TIME_COUNTER_TYPE ulTaskGetIdleRunTimePercentForCore( BaseType_t xCoreID )
    {
        configRUN_TIME_COUNTER_TYPE ulTotalTime, ulReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

        #ifdef port_ALT_GET_RUN_TIME_COUNTER_മൂല്യം
            port_ALT_GET_RUN_TIME_COUNTER_മൂല്യം( ulTotalTime );
        #അല്ലെങ്കിൽ
            ulTotalTime = port_GET_RUN_TIME_COUNTER_മൂല്യം();
        #endif

        /* For percentage calculations. */
        ulTotalTime /= ( configRUN_TIME_COUNTER_TYPE ) 100;

        /* Avoid divide by zero errors. */
        എങ്കിൽ( ulTotalTime > ( configRUN_TIME_COUNTER_TYPE ) 0 )
        {
            /* For SMP, we need to take the kernel lock here as we are about
             * to access kernel data structures. */
            prvENTER_CRITICAL_SMP_ONLY( &xKernelLock );
            {
                ulReturn = xIdleTaskHandle[ xCoreID ]->ulRunTimeCounter / ulTotalTime;
            }
            /* Release the previously taken kernel lock. */
            prvEXIT_CRITICAL_SMP_ONLY( &xKernelLock );
        }
        അല്ലെങ്കിൽ
        {
            ulReturn = 0;
        }

        തിരികെ ulReturn;
    }

#endif /* ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*-----------------------------------------------------------*/

അട്ടി_Type_t * x_Task_Get_അട്ടി_Start( TaskHandle_t xTask )
{
    TCB_t * pxTCB;

    pxTCB = prvGetTCBFromHandle( xTask );
    തിരികെ pxTCB->px_അട്ടി;
}
/*----------------------------------------------------------*/

uint8_t * px_Task_Get_അട്ടി_Start( TaskHandle_t xTask )
{
    തിരികെ (uint8_t *)x_Task_Get_അട്ടി_Start( xTask );
}
/*----------------------------------------------------------*/

#എങ്കിൽ ( INCLUDE_vTaskPrioritySet == 1 )

    ശൂന്യം prvTaskPriorityRaise( prvTaskSavedPriority_t * pxSavedPriority,
                               UBaseType_t uxNewPriority )
    {
        TCB_t * pxTCB;
        UBaseType_t uxPriorityUsedOnEntry;

        configASSERT( ( uxNewPriority < configMAX_PRIORITIES ) );

        /* Ensure the new priority is valid. */
        എങ്കിൽ( uxNewPriority >= ( UBaseType_t ) configMAX_PRIORITIES )
        {
            uxNewPriority = ( UBaseType_t ) configMAX_PRIORITIES - ( UBaseType_t ) 1U;
        }

        #എങ്കിൽ CONFIG_FREERTOS_SMP
            taskENTER_CRITICAL();
        #അല്ലെങ്കിൽ
            taskENTER_CRITICAL( &xKernelLock );
        #endif
        {
            pxTCB = prvGetTCBFromHandle( ശൂന്യ );

            #എങ്കിൽ ( configUSE_MUTEXES == 1 )
            {
                pxSavedPriority->uxPriority = pxTCB->uxPriority;
                pxSavedPriority->uxBasePriority = pxTCB->uxBasePriority;

                /* If uxNewPriority < uxBasePriority, then there is nothing else to
                 * do, as uxBasePriority is always <= uxPriority. */
                എങ്കിൽ( uxNewPriority > pxTCB->uxBasePriority )
                {
                    pxTCB->uxBasePriority = uxNewPriority;

                    /* Remember the task's current priority before attempting to
                     * change it. If the task's current priority is changed, it must
                     * be done so before moving the task between task lists) in order
                     * for the taskRESET_READY_PRIORITY() macro to function correctly. */
                    uxPriorityUsedOnEntry = pxTCB->uxPriority;

                    എങ്കിൽ( uxNewPriority > pxTCB->uxPriority )
                    {
                        pxTCB->uxPriority = uxNewPriority;

                        /* Only reset the event list item value if the value is not
                         * being used for anything else. */
                        എങ്കിൽ( ( list_GET_LIST_ITEM_മൂല്യം( &( pxTCB->xEventListItem ) ) & task_EVENT_LIST_ITEM_മൂല്യം_IN_USE ) == 0UL )
                        {
                            list_SET_LIST_ITEM_മൂല്യം( &( pxTCB->xEventListItem ), ( ( TickType_t ) configMAX_PRIORITIES - ( TickType_t ) uxNewPriority ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */
                        }

                        /* If the task is in the blocked or suspended list we need do
                         * nothing more than change its priority variable. However, if
                         * the task is in a ready list it needs to be removed and placed
                         * in the list appropriate to its new priority. */
                        എങ്കിൽ( listIS_CONTAINED_WITHIN( &( pxReadyTasksLists[ uxPriorityUsedOnEntry ] ), &( pxTCB->xStateListItem ) ) != pdFALSE )
                        {
                            /* The task is currently in its ready list - remove before
                             * adding it to its new ready list.  As we are in a critical
                             * section we can do this even if the scheduler is suspended. */
                            എങ്കിൽ( uxListRemove( &( pxTCB->xStateListItem ) ) == ( UBaseType_t ) 0 )
                            {
                                /* It is known that the task is in its ready list so
                                 * there is no need to check again and the port level
                                 * reset macro can be called directly. */
                                portRESET_READY_PRIORITY( uxPriorityUsedOnEntry, ux_മുകൾ_Ready_Priority );
                            }

                            prvAddTaskToReadyList( pxTCB );
                        }
                    }
                }
            }
            #അല്ലെങ്കിൽ /* if ( configUSE_MUTEXES == 1 ) */
            {
                pxSavedPriority->uxPriority = pxTCB->uxPriority;

                എങ്കിൽ( uxNewPriority > pxTCB->uxPriority )
                {
                    vTaskPrioritySet( ശൂന്യ, uxNewPriority );
                }
            }
            #endif /* if ( configUSE_MUTEXES == 1 ) */
        }
        #എങ്കിൽ CONFIG_FREERTOS_SMP
            taskEXIT_CRITICAL();
        #അല്ലെങ്കിൽ
            taskEXIT_CRITICAL( &xKernelLock );
        #endif
    }

#endif /* INCLUDE_vTaskPrioritySet == 1 */
/*----------------------------------------------------------*/

#എങ്കിൽ ( INCLUDE_vTaskPrioritySet == 1 )

    ശൂന്യം prvTaskPriorityRestore( prvTaskSavedPriority_t * pxSavedPriority )
    {
        TCB_t * pxTCB;
        UBaseType_t uxNewPriority;
        UBaseType_t uxPriorityUsedOnEntry;
        UBaseType_t uxBasePriorityUsedOnEntry;
        BaseType_t xYieldRequired = pdFALSE;

        #എങ്കിൽ CONFIG_FREERTOS_SMP
            taskENTER_CRITICAL();
        #അല്ലെങ്കിൽ
            taskENTER_CRITICAL( &xKernelLock );
        #endif
        {
            pxTCB = prvGetTCBFromHandle( ശൂന്യ );

            #എങ്കിൽ ( configUSE_MUTEXES == 1 )
            {
                /* If the saved uxBasePriority == the task's uxBasePriority, it means
                 * that prvTaskPriorityRaise() never raised the task's uxBasePriority.
                 * In that case, there is nothing else to do. */
                എങ്കിൽ( pxSavedPriority->uxBasePriority != pxTCB->uxBasePriority )
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
                    എങ്കിൽ( pxTCB->uxPriority == uxBasePriorityUsedOnEntry )
                    {
                        എങ്കിൽ( pxTCB->uxMutexesHeld == 0 )
                        {
                            /* The task may have inherited a priority before prvTaskPriorityRaise()
                             * then disinherited a priority after prvTaskPriorityRaise().
                             * Thus we need set the uxPriority to the saved base priority
                             * so that the task's priority gets restored to the priority
                             * before any inheritance or raising. */
                            pxTCB->uxPriority = pxSavedPriority->uxBasePriority;
                        }
                        അല്ലെങ്കിൽ
                        {
                            /* The task may have inherited a priority before prvTaskPriorityRaise()
                             * was called. Thus, we need to restore uxPriority to the
                             * "saved uxPriority" so that the task still retains that
                             * inherited priority. */
                            pxTCB->uxPriority = pxSavedPriority->uxPriority;
                        }

                        uxNewPriority = pxTCB->uxPriority;

                        എങ്കിൽ( uxNewPriority < uxPriorityUsedOnEntry )
                        {
                            /* Setting the priority of the running task down means
                             * there may now be another task of higher priority that
                             * is ready to execute. */
                            xYieldRequired = pdTRUE;
                        }

                        /* Only reset the event list item value if the value is not
                         * being used for anything else. */
                        എങ്കിൽ( ( list_GET_LIST_ITEM_മൂല്യം( &( pxTCB->xEventListItem ) ) & task_EVENT_LIST_ITEM_മൂല്യം_IN_USE ) == 0UL )
                        {
                            list_SET_LIST_ITEM_മൂല്യം( &( pxTCB->xEventListItem ), ( ( TickType_t ) configMAX_PRIORITIES - ( TickType_t ) uxNewPriority ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */
                        }

                        /* If the task is in the blocked or suspended list we need do
                         * nothing more than change its priority variable. However, if
                         * the task is in a ready list it needs to be removed and placed
                         * in the list appropriate to its new priority. */
                        എങ്കിൽ( listIS_CONTAINED_WITHIN( &( pxReadyTasksLists[ uxPriorityUsedOnEntry ] ), &( pxTCB->xStateListItem ) ) != pdFALSE )
                        {
                            /* The task is currently in its ready list - remove before
                             * adding it to its new ready list.  As we are in a critical
                             * section we can do this even if the scheduler is suspended. */
                            എങ്കിൽ( uxListRemove( &( pxTCB->xStateListItem ) ) == ( UBaseType_t ) 0 )
                            {
                                /* It is known that the task is in its ready list so
                                 * there is no need to check again and the port level
                                 * reset macro can be called directly. */
                                portRESET_READY_PRIORITY( uxPriorityUsedOnEntry, ux_മുകൾ_Ready_Priority );
                            }

                            prvAddTaskToReadyList( pxTCB );
                        }

                        എങ്കിൽ( xYieldRequired != pdFALSE )
                        {
                            #എങ്കിൽ CONFIG_FREERTOS_SMP
                                taskYIELD_TASK_CORE_IF_USING_PREEMPTION( pxTCB );
                            #അല്ലെങ്കിൽ
                                taskYIELD_IF_USING_PREEMPTION();
                            #endif
                        }
                    }
                }
            }
            #അല്ലെങ്കിൽ /* if ( configUSE_MUTEXES == 1 ) */
            {
                vTaskPrioritySet( ശൂന്യ, pxSavedPriority->uxPriority );
            }
            #endif /* if ( configUSE_MUTEXES == 1 ) */
        }
        #എങ്കിൽ CONFIG_FREERTOS_SMP
            taskEXIT_CRITICAL();
        #അല്ലെങ്കിൽ
            taskEXIT_CRITICAL( &xKernelLock );
        #endif
    }

#endif /* ( INCLUDE_vTaskPrioritySet == 1 ) */
/*----------------------------------------------------------*/

/* --------------------------------------------- TLSP Deletion Callbacks -------------------------------------------- */

#എങ്കിൽ CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS

    ശൂന്യം vTaskSetThreadLocalStoragePointerAndDelCallback( TaskHandle_t xTaskToSet,
                                                          BaseType_t xIndex,
                                                          ശൂന്യം * pv_മൂല്യം,
                                                          TlsDeleteCallbackFunction_t pvDelCallback )
    {
        /* If TLSP deletion callbacks are enabled, then configNUM_THREAD_LOCAL_STORAGE_POINTERS
         * is doubled in size so that the latter half of the pvThreadLocalStoragePointers
         * stores the deletion callbacks. */
        എങ്കിൽ( xIndex < ( config_അക്കം_THREAD_LOCAL_STORAGE_POINTERS / 2 ) )
        {
            TCB_t * pxTCB;

            #എങ്കിൽ ( config_അക്കം_CORES > 1 )
            {
                /* For SMP, we need a critical section as another core could also
                 * update this task's TLSP at the same time. */
                #എങ്കിൽ CONFIG_FREERTOS_SMP
                {
                    taskENTER_CRITICAL();
                }
                #അല്ലെങ്കിൽ /* CONFIG_FREERTOS_SMP */
                {
                    taskENTER_CRITICAL( &xKernelLock );
                }
                #endif /* CONFIG_FREERTOS_SMP */
            }
            #endif /* configNUM_CORES > 1 */

            pxTCB = prvGetTCBFromHandle( xTaskToSet );
            /* Store the TLSP by indexing the first half of the array */
            pxTCB->pvThreadLocalStoragePointers[ xIndex ] = pv_മൂല്യം;

            /* Store the TLSP deletion callback by indexing the second half
             * of the array. */
            pxTCB->pvThreadLocalStoragePointers[ ( xIndex + ( config_അക്കം_THREAD_LOCAL_STORAGE_POINTERS / 2 ) ) ] = ( ശൂന്യം * ) pvDelCallback;

            #എങ്കിൽ ( config_അക്കം_CORES > 1 )
            {
                #എങ്കിൽ CONFIG_FREERTOS_SMP
                {
                    taskEXIT_CRITICAL();
                }
                #അല്ലെങ്കിൽ /* CONFIG_FREERTOS_SMP */
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

#എങ്കിൽ ( configUSE_NEWLIB_REENTRANT == 1 )

/**
 * @brief Get reentrancy structure of the current task
 *
 * - This function is required by newlib (when __DYNAMIC_REENT__ is enabled)
 * - It will return a pointer to the current task's reent struct
 * - If FreeRTOS is not running, it will return the global reent struct
 *
 * @return Pointer to a the (current taks's)/(global) reent struct
 */
    ഘടന _reent * __getreent( ശൂന്യം )
    {
        /* No lock needed because if this changes, we won't be running anymore. */
        TCB_t * pxCurTask = ( TCB_t * ) xTaskGetCurrentTaskHandle();
        ഘടന _reent * ret;

        എങ്കിൽ( pxCurTask == ശൂന്യ )
        {
            /* No task running. Return global struct. */
            ret = _GLOBAL_REENT;
        }
        അല്ലെങ്കിൽ
        {
            /* We have a currently executing task. Return its reentrant struct. */
            ret = &pxCurTask->xTLSBlock;
        }

        തിരികെ ret;
    }

#endif /* configUSE_NEWLIB_REENTRANT == 1 */

/* -------------------------------------------------- Task Snapshot ------------------------------------------------- */

/**
 * @brief List of all task lists in FreeRTOS
 *
 * @note There are currently differing number of task list between SMP FreeRTOS and ESP-IDF FreeRTOS
 */
സ്ഥിതം List_t * non_ready_task_lists[] =
{
    #ifdef CONFIG_FREERTOS_SMP
        &xPendingReadyList,
    #അല്ലെങ്കിൽ /* CONFIG_FREERTOS_SMP */
        &xPendingReadyList[ 0 ],
        #ifndef CONFIG_FREERTOS_UNICORE
            &xPendingReadyList[ 1 ],
        #endif /* CONFIG_FREERTOS_UNICORE */
    #endif /* CONFIG_FREERTOS_SMP */
    &xDelayedTaskList1,
    &xDelayedTaskList2,
    #എങ്കിൽ ( INCLUDE_vTaskDelete == 1 )
        &xTasksWaitingTermination,
    #endif
    #എങ്കിൽ ( INCLUDE_vTaskSuspend == 1 )
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
സ്ഥിതം List_t * pxGetTaskListByIndex( UBaseType_t uxListIndex )
{
    List_t * pxTaskList;
    സ്ഥിരം size_t xNonReadyTaskListsCnt = ( വലുപ്പം( non_ready_task_lists ) / വലുപ്പം( List_t * ) );

    എങ്കിൽ( uxListIndex < configMAX_PRIORITIES )
    {
        pxTaskList = &pxReadyTasksLists[ configMAX_PRIORITIES - 1 - uxListIndex ];
    }
    അല്ലെങ്കിൽ എങ്കിൽ( uxListIndex < configMAX_PRIORITIES + xNonReadyTaskListsCnt )
    {
        pxTaskList = non_ready_task_lists[ uxListIndex - configMAX_PRIORITIES ];
    }
    അല്ലെങ്കിൽ
    {
        pxTaskList = ശൂന്യ;
    }

    /* sanity check */
    എങ്കിൽ( pxTaskList )
    {
        എങ്കിൽ( !portVALID_LIST_MEM( pxTaskList ) )
        {
            pxTaskList = ശൂന്യ;
        }
    }

    തിരികെ pxTaskList;
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
സ്ഥിതം വരിയിൽ UBaseType_t px_Get_Task_List_എണ്ണം( ശൂന്യം )
{
    തിരികെ configMAX_PRIORITIES + ( വലുപ്പം( non_ready_task_lists ) / വലുപ്പം( List_t * ) );
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
പൂർണ്ണസംഖ്യ xTaskGetNext( TaskIterator_t * xIterator )
{
    എങ്കിൽ( !xIterator )
    {
        തിരികെ -1;
    }

    ListItem_t * pxNextListItem = xIterator->pxNextListItem;
    UBaseType_t uxCurListIdx = xIterator->uxCurrentListIndex;
    UBaseType_t uxMaxListIdx = px_Get_Task_List_എണ്ണം();

    വേണ്ടി( ; uxCurListIdx < uxMaxListIdx; ++uxCurListIdx )
    {
        List_t * pxCurrentTaskList = pxGetTaskListByIndex( uxCurListIdx );

        എങ്കിൽ( !pxCurrentTaskList || ( listCURRENT_LIST_LENGTH( pxCurrentTaskList ) == 0 ) )
        {
            തുടരുക;
        }

        സ്ഥിരം ListItem_t * pxCurrListItem = listGET_END_MARKER( pxCurrentTaskList );

        എങ്കിൽ( !pxNextListItem )
        {
            /* We are here if the traversal starts from the beginning or when we finish traversing
             * for one of the state lists
             */
            pxNextListItem = listGET_NEXT( pxCurrListItem );
        }

        എങ്കിൽ( !portVALID_LIST_MEM( pxNextListItem ) )
        {
            /* Nothing to do with the corrupted list item. We will skip to the next task state list.
             * pxNextListItem should be NULL at the beginning of each task list.
             */
            pxNextListItem = ശൂന്യ;
            തുടരുക;
        }

        TCB_t * pxTCB = ( TCB_t * ) listGET_LIST_ITEM_OWNER( pxNextListItem );

        എങ്കിൽ( !portVALID_TCB_MEM( pxTCB ) )
        {
            pxTCB = ശൂന്യ;
        }

        xIterator->pxTaskHandle = pxTCB;
        xIterator->uxCurrentListIndex = uxCurListIdx;

        എങ്കിൽ( pxCurrListItem->pxPrevious == pxNextListItem )
        {
            /* If this is the last item of the current state list */
            xIterator->uxCurrentListIndex++;
            xIterator->pxNextListItem = ശൂന്യ;
        }
        അല്ലെങ്കിൽ
        {
            xIterator->pxNextListItem = listGET_NEXT( pxNextListItem );
        }

        തിരികെ uxCurListIdx;
    }

    തിരികെ -1; /* end of the task list */
}
/*----------------------------------------------------------*/

BaseType_t vTaskGetSnapshot( TaskHandle_t pxTask,
                             TaskSnapshot_t * pxTaskSnapshot )
{
    ESP_STATIC_ANALYZER_CHECK(!pxTask, pdFALSE);

    എങ്കിൽ( ( portVALID_TCB_MEM( pxTask ) == false ) || ( pxTaskSnapshot == ശൂന്യ ) )
    {
        തിരികെ pdFALSE;
    }

    TCB_t * pxTCB = ( TCB_t * ) pxTask;
    pxTaskSnapshot->pxTCB = pxTCB;
    pxTaskSnapshot->px_മുകൾ_Of_അട്ടി = ( അട്ടി_Type_t * ) pxTCB->px_മുകൾ_Of_അട്ടി;
    pxTaskSnapshot->px_End_Of_അട്ടി = ( അട്ടി_Type_t * ) pxTCB->px_End_Of_അട്ടി;
    തിരികെ pdTRUE;
}
/*----------------------------------------------------------*/

UBaseType_t uxTaskGetSnapshotAll( TaskSnapshot_t * സ്ഥിരം pxTaskSnapshotArray,
                                  സ്ഥിരം UBaseType_t uxArrayLength,
                                  UBaseType_t * സ്ഥിരം pxTCBSize )
{
    UBaseType_t ux_Array_അക്കം_Filled = 0;

    /* Traverse all of the tasks lists */
    TaskIterator_t xTaskIter = { 0 }; /* Point to the first task list */

    ആകുമ്പോൾ( xTaskGetNext( &xTaskIter ) != -1 && ux_Array_അക്കം_Filled < uxArrayLength )
    {
        vTaskGetSnapshot( xTaskIter.pxTaskHandle, &pxTaskSnapshotArray[ ux_Array_അക്കം_Filled ] );
        ux_Array_അക്കം_Filled++;
    }

    എങ്കിൽ( pxTCBSize != ശൂന്യ )
    {
        *pxTCBSize = വലുപ്പം( TCB_t );
    }

    തിരികെ ux_Array_അക്കം_Filled;
}
/*----------------------------------------------------------*/

/* ----------------------------------------------------- Misc ----------------------------------------------------- */

ശൂന്യം * pvTaskGetCurrentTCBForCore( BaseType_t xCoreID )
{
    ശൂന്യം * pvRet;

    configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

    #എങ്കിൽ CONFIG_FREERTOS_SMP
        /* SMP FreeRTOS defines pxCurrentTCB as a macro function call */
        pvRet = ( ശൂന്യം * ) pxCurrentTCB;
    #അല്ലെങ്കിൽ /* CONFIG_FREERTOS_SMP */
        pvRet = ( ശൂന്യം * ) pxCurrentTCBs[ xCoreID ];
    #endif /* CONFIG_FREERTOS_SMP */
    തിരികെ pvRet;
}

/* ----------------------------------------------------- OpenOCD ---------------------------------------------------- */

#എങ്കിൽ CONFIG_FREERTOS_DEBUG_OCDAWARE

/**
 * Debug param indexes. DO NOT change the order. OpenOCD uses the same indexes
 * Entries in FreeRTOS_openocd_params must match the order of these indexes
 */
    എണ്ണൽ
    {
        ESP_FREERTOS_DEBUG_TABLE_SIZE = 0,
        ESP_FREERTOS_DEBUG_TABLE_VERSION,
        ESP_FREERTOS_DEBUG_KERNEL_VER_MAJOR,
        ESP_FREERTOS_DEBUG_KERNEL_VER_MINOR,
        ESP_FREERTOS_DEBUG_KERNEL_VER_BUILD,
        ESP_FREERTOS_DEBUG_UX_മുകൾ_USED_PIORITY,
        ESP_FREERTOS_DEBUG_PX_മുകൾ_OF_അട്ടി,
        ESP_FREERTOS_DEBUG_PC_TASK_NAME,
        ESP_FREERTOS_DEBUG_LIST_SIZE,
        ESP_FREERTOS_DEBUG_LIST_അക്കം_ഇനങ്ങൾ,
        ESP_FREERTOS_DEBUG_LIST_END,
        ESP_FREERTOS_DEBUG_LIST_END_PREV,
        ESP_FREERTOS_DEBUG_LIST_ITEM_PREV,
        ESP_FREERTOS_DEBUG_LIST_ITEM_OWNER,
        ESP_FREERTOS_DEBUG_TASK_എണ്ണം_WIDTH,
        ESP_FREERTOS_DEBUG_PTR_WIDTH,
        /* New entries must be inserted here */
        ESP_FREERTOS_DEBUG_TABLE_END,
    };

    സ്ഥിരം DRAM_ATTR uint8_t FreeRTOS_openocd_params[ ESP_FREERTOS_DEBUG_TABLE_END ] =
    {
        ESP_FREERTOS_DEBUG_TABLE_END, /* table size */
        1,                            /* table version */
        tskKERNEL_VERSION_MAJOR,
        tskKERNEL_VERSION_MINOR,
        tskKERNEL_VERSION_BUILD,
        configMAX_PRIORITIES - 1,                   /* uxTopUsedPriority */
        offsetof( TCB_t, px_മുകൾ_Of_അട്ടി ),            /* thread_stack_offset; */
        offsetof( TCB_t, pcTaskName ),              /* thread_name_offset; */
        വലുപ്പം( List_t ),                           /* list_width */
        offsetof( List_t, ux_സംഖ്യ_Of_ഇനങ്ങൾ ),        /* list_item_num */
        offsetof( List_t, xListEnd ),               /* list_end_offset */
        offsetof( List_t, xListEnd.pxPrevious ),    /* list_next_offset */
        offsetof( ListItem_t, pxPrevious ),         /* list_elem_next_offset */
        offsetof( ListItem_t, pvOwner ),            /* list_elem_content_offset */
        വലുപ്പം( UBaseType_t ),                      /* task_count_width */
        വലുപ്പം( ശൂന്യം * )                            /* ptr_width */
    };

#endif /* CONFIG_FREERTOS_DEBUG_OCDAWARE */
/*----------------------------------------------------------*/

/* ----------------------------------------------------- PSRAM ---------------------------------------------------- */

#എങ്കിൽ CONFIG_SPIRAM

    #എങ്കിൽ CONFIG_FREERTOS_SMP
        BaseType_t prvTaskCreateDynamicAffinitySetWithCaps( TaskFunction_t pxTaskCode,
                                                            സ്ഥിരം അക്ഷരം * സ്ഥിരം pcName,
                                                            സ്ഥിരം config_അട്ടി_DEPTH_TYPE us_അട്ടി_Depth,
                                                            ശൂന്യം * സ്ഥിരം pvParameters,
                                                            UBaseType_t uxPriority,
                                                            UBaseType_t uxCoreAffinityMask,
                                                            UBaseType_t ux_അട്ടി_Memory_Caps,
                                                            TaskHandle_t * സ്ഥിരം pxCreatedTask )
    #അല്ലെങ്കിൽ /* CONFIG_FREERTOS_SMP */
        BaseType_t prvTaskCreateDynamicPinnedToCoreWithCaps( TaskFunction_t pxTaskCode,
                                                             സ്ഥിരം അക്ഷരം * സ്ഥിരം pcName,
                                                             സ്ഥിരം config_അട്ടി_DEPTH_TYPE us_അട്ടി_Depth,
                                                             ശൂന്യം * സ്ഥിരം pvParameters,
                                                             UBaseType_t uxPriority,
                                                             സ്ഥിരം BaseType_t xCoreID,
                                                             UBaseType_t ux_അട്ടി_Memory_Caps,
                                                             TaskHandle_t * സ്ഥിരം pxCreatedTask )
    #endif /* CONFIG_FREERTOS_SMP */
    {
        TCB_t * pxNewTCB;
        BaseType_t xReturn;

        അട്ടി_Type_t * px_അട്ടി;

        configASSERT( ux_അട്ടി_Memory_Caps & ( MALLOC_CAP_8BIT ) );
        configASSERT( ( ux_അട്ടി_Memory_Caps & MALLOC_CAP_SPIRAM ) ||
                      ( ux_അട്ടി_Memory_Caps & MALLOC_CAP_INTERNAL ) );
        #എങ്കിൽ ( !CONFIG_FREERTOS_SMP )
        {
            configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );
        }
        #endif /* !CONFIG_FREERTOS_SMP */

        /* Allocate space for the stack used by the task being created. */
        px_അട്ടി = heap_caps_malloc( ( ( ( size_t ) us_അട്ടി_Depth ) * വലുപ്പം( അട്ടി_Type_t ) ), ux_അട്ടി_Memory_Caps );

        എങ്കിൽ( px_അട്ടി != ശൂന്യ )
        {
            /* Allocate space for the TCB. */
            pxNewTCB = ( TCB_t * ) pvPortMalloc( വലുപ്പം( TCB_t ) );

            എങ്കിൽ( pxNewTCB != ശൂന്യ )
            {
                memset( ( ശൂന്യം * ) pxNewTCB, 0x00, വലുപ്പം( TCB_t ) );

                /* Store the stack location in the TCB. */
                pxNewTCB->px_അട്ടി = px_അട്ടി;
            }
            അല്ലെങ്കിൽ
            {
                /* The stack cannot be used as the TCB has not been created. Free it. */
                heap_caps_free( px_അട്ടി );
            }
        }
        അല്ലെങ്കിൽ
        {
            pxNewTCB = ശൂന്യ;
        }

        എങ്കിൽ( pxNewTCB != ശൂന്യ )
        {
            #എങ്കിൽ ( tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE != 0 )
            {
                /* Tasks can be created statically or dynamically, so note this
                 * task was created dynamically in case it is later deleted. */
                pxNewTCB->ucStaticallyAllocated = tsk_DYNAMICALLY_ALLOCATED_അട്ടി_AND_TCB;
            }
            #endif /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

            #എങ്കിൽ CONFIG_FREERTOS_SMP
            {
                prvInitialiseNewTask( pxTaskCode, pcName, ( uint32_t ) us_അട്ടി_Depth, pvParameters, uxPriority, pxCreatedTask, pxNewTCB, ശൂന്യ );
                #എങ്കിൽ ( ( config_അക്കം_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
                {
                    /* Set the task's affinity before scheduling it */
                    pxNewTCB->uxCoreAffinityMask = uxCoreAffinityMask;
                }
                #endif /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            }
            #അല്ലെങ്കിൽ /* CONFIG_FREERTOS_SMP */
            {
                prvInitialiseNewTask( pxTaskCode, pcName, ( uint32_t ) us_അട്ടി_Depth, pvParameters, uxPriority, pxCreatedTask, pxNewTCB, ശൂന്യ, xCoreID );
            }
            #endif /* CONFIG_FREERTOS_SMP */

            prvAddNewTaskToReadyList( pxNewTCB );
            xReturn = pdPASS;
        }
        അല്ലെങ്കിൽ
        {
            xReturn = errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY;
        }

        തിരികെ xReturn;
    }

#endif /* CONFIG_SPIRAM */
/*----------------------------------------------------------*/
