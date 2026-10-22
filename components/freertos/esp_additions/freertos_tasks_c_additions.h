/*
 * SPDX-FileCopyrightText: 2015-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ "esp_assert.h"
#ಅನ್ತರ್ಭಾವ "esp_heap_caps.h"
#ಅನ್ತರ್ಭಾವ "esp_compiler.h"
#ಅನ್ತರ್ಭಾವ "freertos/idf_additions.h"
#ಅನ್ತರ್ಭಾವ "freertos/freertos_debug.h"
#ಅನ್ತರ್ಭಾವ "esp_private/freertos_idf_additions_priv.h"

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
_Static_assert( offsetof( StaticTask_t, pxDummy6 ) == offsetof( TCB_t, px_ರಾಶಿ ) );
_Static_assert( offsetof( StaticTask_t, pxDummy8 ) == offsetof( TCB_t, px_End_Of_ರಾಶಿ ) );
#ಯದಿ !CONFIG_IDF_TARGET_LINUX    // Disabled for linux builds due to differences in types
_Static_assert( tskNO_AFFINITY == ( BaseType_t ) CONFIG_FREERTOS_NO_AFFINITY, "CONFIG_FREERTOS_NO_AFFINITY must be the same as tskNO_AFFINITY" );
#endif

/* ------------------------------------------------- Kernel Control ------------------------------------------------- */

#ಯದಿ ( !CONFIG_FREERTOS_SMP && ( config_ಅಂಕೆ_CORES > 1 ) )

/*
 * Wrapper function to take "xKerneLock"
 */
    ಶೂನ್ಯ prvTakeKernelLock( ಶೂನ್ಯ )
    {
        /* We call the tasks.c critical section macro to take xKernelLock */
        taskENTER_CRITICAL( &xKernelLock );
    }

#endif /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#ಯದಿ ( !CONFIG_FREERTOS_SMP && ( config_ಅಂಕೆ_CORES > 1 ) )

/*
 * Wrapper function to release "xKerneLock"
 */
    ಶೂನ್ಯ prvReleaseKernelLock( ಶೂನ್ಯ )
    {
        /* We call the tasks.c critical section macro to release xKernelLock */
        taskEXIT_CRITICAL( &xKernelLock );
    }

#endif /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#ಯದಿ ( CONFIG_FREERTOS_SMP && ( config_ಅಂಕೆ_CORES > 1 ) )

/*
 * Workaround for non-thread safe multi-core OS startup (see IDF-4524)
 */
    ಶೂನ್ಯ prvStartSchedulerOtherCores( ಶೂನ್ಯ )
    {
        /* This function is always called with interrupts disabled*/
        xSchedulerRunning = pdTRUE;
    }

#endif /* ( CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#ಯದಿ ( !CONFIG_FREERTOS_SMP && ( config_ಅಂಕೆ_CORES > 1 ) )

    BaseType_t xTaskIncrementTickOtherCores( ಶೂನ್ಯ )
    {
        /* Minor optimization. This function can never switch cores mid
         * execution */
        BaseType_t xCoreID = portGET_CORE_ID();
        BaseType_t xSwitchRequired = pdFALSE;

        /* This function should never be called by Core 0. */
        configASSERT( xCoreID != 0 );

        /* Called by the portable layer each time a tick interrupt occurs
         * on a core other than core 0. */
        traceTASK_INCREMENT_TICK( x_Tick_ಎಣಿಕೆ );

        ಯದಿ( uxSchedulerSuspended[ xCoreID ] == ( UBaseType_t ) 0U )
        {
            /* We need take the kernel lock here as we are about to access
             * kernel data structures. */
            taskENTER_CRITICAL_ISR( &xKernelLock );

            /* Tasks of equal priority to the currently running task will share
             * processing time (time slice) if preemption is on, and the application
             * writer has not explicitly turned time slicing off. */
            #ಯದಿ ( ( configUSE_PREEMPTION == 1 ) && ( configUSE_TIME_SLICING == 1 ) )
            {
                ಯದಿ( listCURRENT_LIST_LENGTH( &( pxReadyTasksLists[ pxCurrentTCBs[ xCoreID ]->uxPriority ] ) ) > ( UBaseType_t ) 1 )
                {
                    xSwitchRequired = pdTRUE;
                }
                ಅನ್ಯಥಾ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            #endif /* ( ( configUSE_PREEMPTION == 1 ) && ( configUSE_TIME_SLICING == 1 ) ) */

            /* Release the previously taken kernel lock as we have finished
             * accessing the kernel data structures. */
            taskEXIT_CRITICAL_ISR( &xKernelLock );

            #ಯದಿ ( configUSE_PREEMPTION == 1 )
            {
                ಯದಿ( xYieldPending[ xCoreID ] != pdFALSE )
                {
                    xSwitchRequired = pdTRUE;
                }
                ಅನ್ಯಥಾ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            #endif /* configUSE_PREEMPTION */
        }

        #ಯದಿ ( configUSE_TICK_HOOK == 1 )
        {
            vApplicationTickHook();
        }
        #endif

        ಪ್ರತ್ಯಾಗಮ xSwitchRequired;
    }

#endif /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

/* -------------------------------------------------- Task Creation ------------------------------------------------- */

#ಯದಿ ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )

    BaseType_t xTaskCreatePinnedToCore( TaskFunction_t pxTaskCode,
                                        ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ pcName,
                                        ಧ್ರುವ uint32_t us_ರಾಶಿ_Depth,
                                        ಶೂನ್ಯ * ಧ್ರುವ pvParameters,
                                        UBaseType_t uxPriority,
                                        TaskHandle_t * ಧ್ರುವ pxCreatedTask,
                                        ಧ್ರುವ BaseType_t xCoreID )
    {
        BaseType_t xReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );

        #ಯದಿ CONFIG_FREERTOS_SMP
        {
            /* If using Amazon SMP FreeRTOS. This function is just a wrapper around
             * xTaskCreate() or xTaskCreateAffinitySet(). */
            #ಯದಿ ( ( config_ಅಂಕೆ_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
            {
                /* Convert xCoreID into an affinity mask */
                UBaseType_t uxCoreAffinityMask;

                /* Bit shifting << xCoreID is only valid if we have less than
                 * 32 cores. */
                ESP_STATIC_ASSERT( config_ಅಂಕೆ_CORES < 32 );

                ಯದಿ( xCoreID == tskNO_AFFINITY )
                {
                    uxCoreAffinityMask = tskNO_AFFINITY;
                }
                ಅನ್ಯಥಾ
                {
                    uxCoreAffinityMask = ( 1 << xCoreID );
                }

                xReturn = xTaskCreateAffinitySet( pxTaskCode, pcName, us_ರಾಶಿ_Depth, pvParameters, uxPriority, uxCoreAffinityMask, pxCreatedTask );
            }
            #ಅನ್ಯಥಾ /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            {
                xReturn = xTaskCreate( pxTaskCode, pcName, us_ರಾಶಿ_Depth, pvParameters, uxPriority, pxCreatedTask );
            }
            #endif /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
        }
        #ಅನ್ಯಥಾ /* CONFIG_FREERTOS_SMP */
        {
            TCB_t * pxNewTCB;

            /* If the stack grows down then allocate the stack then the TCB so the
             * stack does not grow into the TCB.  Likewise if the stack grows up
             * then allocate the TCB then the stack. */
            #ಯದಿ ( port_ರಾಶಿ_GROWTH > 0 )
            {
                /* Allocate space for the TCB.  Where the memory comes from depends on
                 * the implementation of the port malloc function and whether or not static
                 * allocation is being used. */
                pxNewTCB = ( TCB_t * ) pvPortMalloc( ಪರಿಮಾಣ( TCB_t ) );

                ಯದಿ( pxNewTCB != ರಿಕ್ತ )
                {
                    memset( ( ಶೂನ್ಯ * ) pxNewTCB, 0x00, ಪರಿಮಾಣ( TCB_t ) );

                    /* Allocate space for the stack used by the task being created.
                     * The base of the stack memory stored in the TCB so the task can
                     * be deleted later if required. */
                    pxNewTCB->px_ರಾಶಿ = ( ರಾಶಿ_Type_t * ) pv_Port_Malloc_ರಾಶಿ( ( ( ( size_t ) us_ರಾಶಿ_Depth ) * ಪರಿಮಾಣ( ರಾಶಿ_Type_t ) ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */

                    ಯದಿ( pxNewTCB->px_ರಾಶಿ == ರಿಕ್ತ )
                    {
                        /* Could not allocate the stack.  Delete the allocated TCB. */
                        vPortFree( pxNewTCB );
                        pxNewTCB = ರಿಕ್ತ;
                    }
                }
            }
            #ಅನ್ಯಥಾ /* portSTACK_GROWTH */
            {
                ರಾಶಿ_Type_t * px_ರಾಶಿ;

                /* Allocate space for the stack used by the task being created. */
                px_ರಾಶಿ = pv_Port_Malloc_ರಾಶಿ( ( ( ( size_t ) us_ರಾಶಿ_Depth ) * ಪರಿಮಾಣ( ರಾಶಿ_Type_t ) ) ); /*lint !e9079 All values returned by pvPortMalloc() have at least the alignment required by the MCU's stack and this allocation is the stack. */

                ಯದಿ( px_ರಾಶಿ != ರಿಕ್ತ )
                {
                    /* Allocate space for the TCB. */
                    pxNewTCB = ( TCB_t * ) pvPortMalloc( ಪರಿಮಾಣ( TCB_t ) ); /*lint !e9087 !e9079 All values returned by pvPortMalloc() have at least the alignment required by the MCU's stack, and the first member of TCB_t is always a pointer to the task's stack. */

                    ಯದಿ( pxNewTCB != ರಿಕ್ತ )
                    {
                        memset( ( ಶೂನ್ಯ * ) pxNewTCB, 0x00, ಪರಿಮಾಣ( TCB_t ) );

                        /* Store the stack location in the TCB. */
                        pxNewTCB->px_ರಾಶಿ = px_ರಾಶಿ;
                    }
                    ಅನ್ಯಥಾ
                    {
                        /* The stack cannot be used as the TCB was not created.  Free
                         * it again. */
                        v_Port_Free_ರಾಶಿ( px_ರಾಶಿ );
                    }
                }
                ಅನ್ಯಥಾ
                {
                    pxNewTCB = ರಿಕ್ತ;
                }
            }
            #endif /* portSTACK_GROWTH */

            ಯದಿ( pxNewTCB != ರಿಕ್ತ )
            {
                #ಯದಿ ( tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE != 0 ) /*lint !e9029 !e731 Macro has been consolidated for readability reasons. */
                {
                    /* Tasks can be created statically or dynamically, so note this
                     * task was created dynamically in case it is later deleted. */
                    pxNewTCB->ucStaticallyAllocated = tsk_DYNAMICALLY_ALLOCATED_ರಾಶಿ_AND_TCB;
                }
                #endif /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

                prvInitialiseNewTask( pxTaskCode, pcName, ( uint32_t ) us_ರಾಶಿ_Depth, pvParameters, uxPriority, pxCreatedTask, pxNewTCB, ರಿಕ್ತ, xCoreID );
                prvAddNewTaskToReadyList( pxNewTCB );
                xReturn = pdPASS;
            }
            ಅನ್ಯಥಾ
            {
                xReturn = errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY;
            }
        }
        #endif /* CONFIG_FREERTOS_SMP */

        ಪ್ರತ್ಯಾಗಮ xReturn;
    }

#endif /* ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#ಯದಿ ( configSUPPORT_STATIC_ALLOCATION == 1 )

    TaskHandle_t xTaskCreateStaticPinnedToCore( TaskFunction_t pxTaskCode,
                                                ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ pcName,
                                                ಧ್ರುವ uint32_t ul_ರಾಶಿ_Depth,
                                                ಶೂನ್ಯ * ಧ್ರುವ pvParameters,
                                                UBaseType_t uxPriority,
                                                ರಾಶಿ_Type_t * ಧ್ರುವ pux_ರಾಶಿ_Buffer,
                                                StaticTask_t * ಧ್ರುವ pxTaskBuffer,
                                                ಧ್ರುವ BaseType_t xCoreID )
    {
        TaskHandle_t xReturn;

        configASSERT( port_VALID_ರಾಶಿ_MEM( pux_ರಾಶಿ_Buffer ) );
        configASSERT( portVALID_TCB_MEM( pxTaskBuffer ) );
        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );

        #ಯದಿ CONFIG_FREERTOS_SMP
        {
            /* If using Amazon SMP FreeRTOS. This function is just a wrapper around
             * xTaskCreateStatic() or xTaskCreateStaticAffinitySet(). */
            #ಯದಿ ( ( config_ಅಂಕೆ_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
            {
                /* Convert xCoreID into an affinity mask */
                UBaseType_t uxCoreAffinityMask;

                /* Bit shifting << xCoreID is only valid if we have less than
                 * 32 cores. */
                ESP_STATIC_ASSERT( config_ಅಂಕೆ_CORES < 32 );

                ಯದಿ( xCoreID == tskNO_AFFINITY )
                {
                    uxCoreAffinityMask = tskNO_AFFINITY;
                }
                ಅನ್ಯಥಾ
                {
                    uxCoreAffinityMask = ( 1 << xCoreID );
                }

                xReturn = xTaskCreateStaticAffinitySet( pxTaskCode, pcName, ul_ರಾಶಿ_Depth, pvParameters, uxPriority, pux_ರಾಶಿ_Buffer, pxTaskBuffer, uxCoreAffinityMask );
            }
            #ಅನ್ಯಥಾ /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            {
                xReturn = xTaskCreateStatic( pxTaskCode, pcName, ul_ರಾಶಿ_Depth, pvParameters, uxPriority, pux_ರಾಶಿ_Buffer, pxTaskBuffer );
            }
            #endif /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
        }
        #ಅನ್ಯಥಾ /* CONFIG_FREERTOS_SMP */
        {
            TCB_t * pxNewTCB;

            #ಯದಿ ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticTask_t equals the size of the real task
                 * structure. */
                ಚಞ್ಚಲ size_t xSize = ಪರಿಮಾಣ( StaticTask_t );
                configASSERT( xSize == ಪರಿಮಾಣ( TCB_t ) );
                ( ಶೂನ್ಯ ) xSize; /* Prevent lint warning when configASSERT() is not used. */
            }
            #endif /* configASSERT_DEFINED */

            ಯದಿ( ( pxTaskBuffer != ರಿಕ್ತ ) && ( pux_ರಾಶಿ_Buffer != ರಿಕ್ತ ) )
            {
                /* The memory used for the task's TCB and stack are passed into this
                 * function - use them. */
                pxNewTCB = ( TCB_t * ) pxTaskBuffer; /*lint !e740 !e9087 Unusual cast is ok as the structures are designed to have the same alignment, and the size is checked by an assert. */
                memset( ( ಶೂನ್ಯ * ) pxNewTCB, 0x00, ಪರಿಮಾಣ( TCB_t ) );
                pxNewTCB->px_ರಾಶಿ = ( ರಾಶಿ_Type_t * ) pux_ರಾಶಿ_Buffer;

                #ಯದಿ ( tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE != 0 ) /*lint !e731 !e9029 Macro has been consolidated for readability reasons. */
                {
                    /* Tasks can be created statically or dynamically, so note this
                     * task was created statically in case the task is later deleted. */
                    pxNewTCB->ucStaticallyAllocated = tsk_STATICALLY_ALLOCATED_ರಾಶಿ_AND_TCB;
                }
                #endif /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

                prvInitialiseNewTask( pxTaskCode, pcName, ul_ರಾಶಿ_Depth, pvParameters, uxPriority, &xReturn, pxNewTCB, ರಿಕ್ತ, xCoreID );
                prvAddNewTaskToReadyList( pxNewTCB );
            }
            ಅನ್ಯಥಾ
            {
                xReturn = ರಿಕ್ತ;
            }
        }
        #endif /* CONFIG_FREERTOS_SMP */

        ಪ್ರತ್ಯಾಗಮ xReturn;
    }

#endif /* ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#ಯದಿ ( configUSE_TIMERS == 1 )

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
    BaseType_t __attribute__( ( weak ) ) xTimerCreateTimerTask( ಶೂನ್ಯ )
    {
        ಪ್ರತ್ಯಾಗಮ pdPASS;
    }

#endif /* configUSE_TIMERS */
/*----------------------------------------------------------*/

/* ------------------------------------------------- Task Utilities ------------------------------------------------- */

BaseType_t xTaskGetCoreID( TaskHandle_t xTask )
{
    BaseType_t xReturn;

    #ಯದಿ ( config_ಅಂಕೆ_CORES > 1 )
    {
        #ಯದಿ CONFIG_FREERTOS_SMP
            UBaseType_t uxCoreAffinityMask;

            /* Get the core affinity mask and convert it to an ID */
            uxCoreAffinityMask = vTaskCoreAffinityGet( xTask );

            /* If the task is not pinned to a particular core, treat it as tskNO_AFFINITY */
            ಯದಿ( uxCoreAffinityMask & ( uxCoreAffinityMask - 1 ) ) /* If more than one bit set */
            {
                xReturn = tskNO_AFFINITY;
            }
            ಅನ್ಯಥಾ
            {
                ಪೂರ್ಣಾಙ್ಕ iIndexPlusOne = __builtin_ffs( uxCoreAffinityMask );
                ಪ್ರತಿಜ್ಞಾ( iIndexPlusOne >= 1 );
                xReturn = iIndexPlusOne - 1;
            }
        #ಅನ್ಯಥಾ /* CONFIG_FREERTOS_SMP */
            TCB_t * pxTCB;

            pxTCB = prvGetTCBFromHandle( xTask );

            xReturn = pxTCB->xCoreID;
        #endif /* CONFIG_FREERTOS_SMP */
    }
    #ಅನ್ಯಥಾ /* configNUM_CORES > 1 */
    {
        /* Single-core. Just return a core ID of 0 */
        xReturn = 0;
    }
    #endif /* configNUM_CORES > 1 */

    ಪ್ರತ್ಯಾಗಮ xReturn;
}
/*----------------------------------------------------------*/

#ಯದಿ ( ( !CONFIG_FREERTOS_SMP ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

    TaskHandle_t xTaskGetIdleTaskHandleForCore( BaseType_t xCoreID )
    {
        /* If xTaskGetIdleTaskHandle() is called before the scheduler has been
         * started, then xIdleTaskHandle will be NULL. */
        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );
        configASSERT( ( xIdleTaskHandle[ xCoreID ] != ರಿಕ್ತ ) );
        ಪ್ರತ್ಯಾಗಮ xIdleTaskHandle[ xCoreID ];
    }

#endif /* ( ( !CONFIG_FREERTOS_SMP ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*----------------------------------------------------------*/

#ಯದಿ ( ( !CONFIG_FREERTOS_SMP ) && ( ( INCLUDE_xTaskGetCurrentTaskHandle == 1 ) || ( configUSE_MUTEXES == 1 ) ) )

    TaskHandle_t xTaskGetCurrentTaskHandleForCore( BaseType_t xCoreID )
    {
        TaskHandle_t xReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

        #ಯದಿ ( CONFIG_FREERTOS_SMP )
        {
            xReturn = xTaskGetCurrentTaskHandleCPU( ( UBaseType_t ) xCoreID );
        }
        #ಅನ್ಯಥಾ /* CONFIG_FREERTOS_SMP */
        {
            /* A critical section is not required as this function does not
             * guarantee that the TCB will still be valid when this function
             * returns. */
            xReturn = pxCurrentTCBs[ xCoreID ];
        }
        #endif /* CONFIG_FREERTOS_SMP */

        ಪ್ರತ್ಯಾಗಮ xReturn;
    }

#endif /* ( ( !CONFIG_FREERTOS_SMP ) && ( ( INCLUDE_xTaskGetCurrentTaskHandle == 1 ) || ( configUSE_MUTEXES == 1 ) ) ) */
/*----------------------------------------------------------*/

#ಯದಿ ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

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

        ಪ್ರತ್ಯಾಗಮ ulRunTimeCounter;
    }

#endif /* ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*----------------------------------------------------------*/

#ಯದಿ ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

    configRUN_TIME_COUNTER_TYPE ulTaskGetIdleRunTimePercentForCore( BaseType_t xCoreID )
    {
        configRUN_TIME_COUNTER_TYPE ulTotalTime, ulReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

        #ifdef port_ALT_GET_RUN_TIME_COUNTER_ಮೌಲ್ಯ
            port_ALT_GET_RUN_TIME_COUNTER_ಮೌಲ್ಯ( ulTotalTime );
        #ಅನ್ಯಥಾ
            ulTotalTime = port_GET_RUN_TIME_COUNTER_ಮೌಲ್ಯ();
        #endif

        /* For percentage calculations. */
        ulTotalTime /= ( configRUN_TIME_COUNTER_TYPE ) 100;

        /* Avoid divide by zero errors. */
        ಯದಿ( ulTotalTime > ( configRUN_TIME_COUNTER_TYPE ) 0 )
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
        ಅನ್ಯಥಾ
        {
            ulReturn = 0;
        }

        ಪ್ರತ್ಯಾಗಮ ulReturn;
    }

#endif /* ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*-----------------------------------------------------------*/

ರಾಶಿ_Type_t * x_Task_Get_ರಾಶಿ_Start( TaskHandle_t xTask )
{
    TCB_t * pxTCB;

    pxTCB = prvGetTCBFromHandle( xTask );
    ಪ್ರತ್ಯಾಗಮ pxTCB->px_ರಾಶಿ;
}
/*----------------------------------------------------------*/

uint8_t * px_Task_Get_ರಾಶಿ_Start( TaskHandle_t xTask )
{
    ಪ್ರತ್ಯಾಗಮ (uint8_t *)x_Task_Get_ರಾಶಿ_Start( xTask );
}
/*----------------------------------------------------------*/

#ಯದಿ ( INCLUDE_vTaskPrioritySet == 1 )

    ಶೂನ್ಯ prvTaskPriorityRaise( prvTaskSavedPriority_t * pxSavedPriority,
                               UBaseType_t uxNewPriority )
    {
        TCB_t * pxTCB;
        UBaseType_t uxPriorityUsedOnEntry;

        configASSERT( ( uxNewPriority < configMAX_PRIORITIES ) );

        /* Ensure the new priority is valid. */
        ಯದಿ( uxNewPriority >= ( UBaseType_t ) configMAX_PRIORITIES )
        {
            uxNewPriority = ( UBaseType_t ) configMAX_PRIORITIES - ( UBaseType_t ) 1U;
        }

        #ಯದಿ CONFIG_FREERTOS_SMP
            taskENTER_CRITICAL();
        #ಅನ್ಯಥಾ
            taskENTER_CRITICAL( &xKernelLock );
        #endif
        {
            pxTCB = prvGetTCBFromHandle( ರಿಕ್ತ );

            #ಯದಿ ( configUSE_MUTEXES == 1 )
            {
                pxSavedPriority->uxPriority = pxTCB->uxPriority;
                pxSavedPriority->uxBasePriority = pxTCB->uxBasePriority;

                /* If uxNewPriority < uxBasePriority, then there is nothing else to
                 * do, as uxBasePriority is always <= uxPriority. */
                ಯದಿ( uxNewPriority > pxTCB->uxBasePriority )
                {
                    pxTCB->uxBasePriority = uxNewPriority;

                    /* Remember the task's current priority before attempting to
                     * change it. If the task's current priority is changed, it must
                     * be done so before moving the task between task lists) in order
                     * for the taskRESET_READY_PRIORITY() macro to function correctly. */
                    uxPriorityUsedOnEntry = pxTCB->uxPriority;

                    ಯದಿ( uxNewPriority > pxTCB->uxPriority )
                    {
                        pxTCB->uxPriority = uxNewPriority;

                        /* Only reset the event list item value if the value is not
                         * being used for anything else. */
                        ಯದಿ( ( list_GET_LIST_ITEM_ಮೌಲ್ಯ( &( pxTCB->xEventListItem ) ) & task_EVENT_LIST_ITEM_ಮೌಲ್ಯ_IN_USE ) == 0UL )
                        {
                            list_SET_LIST_ITEM_ಮೌಲ್ಯ( &( pxTCB->xEventListItem ), ( ( TickType_t ) configMAX_PRIORITIES - ( TickType_t ) uxNewPriority ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */
                        }

                        /* If the task is in the blocked or suspended list we need do
                         * nothing more than change its priority variable. However, if
                         * the task is in a ready list it needs to be removed and placed
                         * in the list appropriate to its new priority. */
                        ಯದಿ( listIS_CONTAINED_WITHIN( &( pxReadyTasksLists[ uxPriorityUsedOnEntry ] ), &( pxTCB->xStateListItem ) ) != pdFALSE )
                        {
                            /* The task is currently in its ready list - remove before
                             * adding it to its new ready list.  As we are in a critical
                             * section we can do this even if the scheduler is suspended. */
                            ಯದಿ( uxListRemove( &( pxTCB->xStateListItem ) ) == ( UBaseType_t ) 0 )
                            {
                                /* It is known that the task is in its ready list so
                                 * there is no need to check again and the port level
                                 * reset macro can be called directly. */
                                portRESET_READY_PRIORITY( uxPriorityUsedOnEntry, ux_ತುದಿ_Ready_Priority );
                            }

                            prvAddTaskToReadyList( pxTCB );
                        }
                    }
                }
            }
            #ಅನ್ಯಥಾ /* if ( configUSE_MUTEXES == 1 ) */
            {
                pxSavedPriority->uxPriority = pxTCB->uxPriority;

                ಯದಿ( uxNewPriority > pxTCB->uxPriority )
                {
                    vTaskPrioritySet( ರಿಕ್ತ, uxNewPriority );
                }
            }
            #endif /* if ( configUSE_MUTEXES == 1 ) */
        }
        #ಯದಿ CONFIG_FREERTOS_SMP
            taskEXIT_CRITICAL();
        #ಅನ್ಯಥಾ
            taskEXIT_CRITICAL( &xKernelLock );
        #endif
    }

#endif /* INCLUDE_vTaskPrioritySet == 1 */
/*----------------------------------------------------------*/

#ಯದಿ ( INCLUDE_vTaskPrioritySet == 1 )

    ಶೂನ್ಯ prvTaskPriorityRestore( prvTaskSavedPriority_t * pxSavedPriority )
    {
        TCB_t * pxTCB;
        UBaseType_t uxNewPriority;
        UBaseType_t uxPriorityUsedOnEntry;
        UBaseType_t uxBasePriorityUsedOnEntry;
        BaseType_t xYieldRequired = pdFALSE;

        #ಯದಿ CONFIG_FREERTOS_SMP
            taskENTER_CRITICAL();
        #ಅನ್ಯಥಾ
            taskENTER_CRITICAL( &xKernelLock );
        #endif
        {
            pxTCB = prvGetTCBFromHandle( ರಿಕ್ತ );

            #ಯದಿ ( configUSE_MUTEXES == 1 )
            {
                /* If the saved uxBasePriority == the task's uxBasePriority, it means
                 * that prvTaskPriorityRaise() never raised the task's uxBasePriority.
                 * In that case, there is nothing else to do. */
                ಯದಿ( pxSavedPriority->uxBasePriority != pxTCB->uxBasePriority )
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
                    ಯದಿ( pxTCB->uxPriority == uxBasePriorityUsedOnEntry )
                    {
                        ಯದಿ( pxTCB->uxMutexesHeld == 0 )
                        {
                            /* The task may have inherited a priority before prvTaskPriorityRaise()
                             * then disinherited a priority after prvTaskPriorityRaise().
                             * Thus we need set the uxPriority to the saved base priority
                             * so that the task's priority gets restored to the priority
                             * before any inheritance or raising. */
                            pxTCB->uxPriority = pxSavedPriority->uxBasePriority;
                        }
                        ಅನ್ಯಥಾ
                        {
                            /* The task may have inherited a priority before prvTaskPriorityRaise()
                             * was called. Thus, we need to restore uxPriority to the
                             * "saved uxPriority" so that the task still retains that
                             * inherited priority. */
                            pxTCB->uxPriority = pxSavedPriority->uxPriority;
                        }

                        uxNewPriority = pxTCB->uxPriority;

                        ಯದಿ( uxNewPriority < uxPriorityUsedOnEntry )
                        {
                            /* Setting the priority of the running task down means
                             * there may now be another task of higher priority that
                             * is ready to execute. */
                            xYieldRequired = pdTRUE;
                        }

                        /* Only reset the event list item value if the value is not
                         * being used for anything else. */
                        ಯದಿ( ( list_GET_LIST_ITEM_ಮೌಲ್ಯ( &( pxTCB->xEventListItem ) ) & task_EVENT_LIST_ITEM_ಮೌಲ್ಯ_IN_USE ) == 0UL )
                        {
                            list_SET_LIST_ITEM_ಮೌಲ್ಯ( &( pxTCB->xEventListItem ), ( ( TickType_t ) configMAX_PRIORITIES - ( TickType_t ) uxNewPriority ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */
                        }

                        /* If the task is in the blocked or suspended list we need do
                         * nothing more than change its priority variable. However, if
                         * the task is in a ready list it needs to be removed and placed
                         * in the list appropriate to its new priority. */
                        ಯದಿ( listIS_CONTAINED_WITHIN( &( pxReadyTasksLists[ uxPriorityUsedOnEntry ] ), &( pxTCB->xStateListItem ) ) != pdFALSE )
                        {
                            /* The task is currently in its ready list - remove before
                             * adding it to its new ready list.  As we are in a critical
                             * section we can do this even if the scheduler is suspended. */
                            ಯದಿ( uxListRemove( &( pxTCB->xStateListItem ) ) == ( UBaseType_t ) 0 )
                            {
                                /* It is known that the task is in its ready list so
                                 * there is no need to check again and the port level
                                 * reset macro can be called directly. */
                                portRESET_READY_PRIORITY( uxPriorityUsedOnEntry, ux_ತುದಿ_Ready_Priority );
                            }

                            prvAddTaskToReadyList( pxTCB );
                        }

                        ಯದಿ( xYieldRequired != pdFALSE )
                        {
                            #ಯದಿ CONFIG_FREERTOS_SMP
                                taskYIELD_TASK_CORE_IF_USING_PREEMPTION( pxTCB );
                            #ಅನ್ಯಥಾ
                                taskYIELD_IF_USING_PREEMPTION();
                            #endif
                        }
                    }
                }
            }
            #ಅನ್ಯಥಾ /* if ( configUSE_MUTEXES == 1 ) */
            {
                vTaskPrioritySet( ರಿಕ್ತ, pxSavedPriority->uxPriority );
            }
            #endif /* if ( configUSE_MUTEXES == 1 ) */
        }
        #ಯದಿ CONFIG_FREERTOS_SMP
            taskEXIT_CRITICAL();
        #ಅನ್ಯಥಾ
            taskEXIT_CRITICAL( &xKernelLock );
        #endif
    }

#endif /* ( INCLUDE_vTaskPrioritySet == 1 ) */
/*----------------------------------------------------------*/

/* --------------------------------------------- TLSP Deletion Callbacks -------------------------------------------- */

#ಯದಿ CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS

    ಶೂನ್ಯ vTaskSetThreadLocalStoragePointerAndDelCallback( TaskHandle_t xTaskToSet,
                                                          BaseType_t xIndex,
                                                          ಶೂನ್ಯ * pv_ಮೌಲ್ಯ,
                                                          TlsDeleteCallbackFunction_t pvDelCallback )
    {
        /* If TLSP deletion callbacks are enabled, then configNUM_THREAD_LOCAL_STORAGE_POINTERS
         * is doubled in size so that the latter half of the pvThreadLocalStoragePointers
         * stores the deletion callbacks. */
        ಯದಿ( xIndex < ( config_ಅಂಕೆ_THREAD_LOCAL_STORAGE_POINTERS / 2 ) )
        {
            TCB_t * pxTCB;

            #ಯದಿ ( config_ಅಂಕೆ_CORES > 1 )
            {
                /* For SMP, we need a critical section as another core could also
                 * update this task's TLSP at the same time. */
                #ಯದಿ CONFIG_FREERTOS_SMP
                {
                    taskENTER_CRITICAL();
                }
                #ಅನ್ಯಥಾ /* CONFIG_FREERTOS_SMP */
                {
                    taskENTER_CRITICAL( &xKernelLock );
                }
                #endif /* CONFIG_FREERTOS_SMP */
            }
            #endif /* configNUM_CORES > 1 */

            pxTCB = prvGetTCBFromHandle( xTaskToSet );
            /* Store the TLSP by indexing the first half of the array */
            pxTCB->pvThreadLocalStoragePointers[ xIndex ] = pv_ಮೌಲ್ಯ;

            /* Store the TLSP deletion callback by indexing the second half
             * of the array. */
            pxTCB->pvThreadLocalStoragePointers[ ( xIndex + ( config_ಅಂಕೆ_THREAD_LOCAL_STORAGE_POINTERS / 2 ) ) ] = ( ಶೂನ್ಯ * ) pvDelCallback;

            #ಯದಿ ( config_ಅಂಕೆ_CORES > 1 )
            {
                #ಯದಿ CONFIG_FREERTOS_SMP
                {
                    taskEXIT_CRITICAL();
                }
                #ಅನ್ಯಥಾ /* CONFIG_FREERTOS_SMP */
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

#ಯದಿ ( configUSE_NEWLIB_REENTRANT == 1 )

/**
 * @brief Get reentrancy structure of the current task
 *
 * - This function is required by newlib (when __DYNAMIC_REENT__ is enabled)
 * - It will return a pointer to the current task's reent struct
 * - If FreeRTOS is not running, it will return the global reent struct
 *
 * @return Pointer to a the (current taks's)/(global) reent struct
 */
    ಸಂರಚನಾ _reent * __getreent( ಶೂನ್ಯ )
    {
        /* No lock needed because if this changes, we won't be running anymore. */
        TCB_t * pxCurTask = ( TCB_t * ) xTaskGetCurrentTaskHandle();
        ಸಂರಚನಾ _reent * ret;

        ಯದಿ( pxCurTask == ರಿಕ್ತ )
        {
            /* No task running. Return global struct. */
            ret = _GLOBAL_REENT;
        }
        ಅನ್ಯಥಾ
        {
            /* We have a currently executing task. Return its reentrant struct. */
            ret = &pxCurTask->xTLSBlock;
        }

        ಪ್ರತ್ಯಾಗಮ ret;
    }

#endif /* configUSE_NEWLIB_REENTRANT == 1 */

/* -------------------------------------------------- Task Snapshot ------------------------------------------------- */

/**
 * @brief List of all task lists in FreeRTOS
 *
 * @note There are currently differing number of task list between SMP FreeRTOS and ESP-IDF FreeRTOS
 */
ಸ್ಥಿರ List_t * non_ready_task_lists[] =
{
    #ifdef CONFIG_FREERTOS_SMP
        &xPendingReadyList,
    #ಅನ್ಯಥಾ /* CONFIG_FREERTOS_SMP */
        &xPendingReadyList[ 0 ],
        #ifndef CONFIG_FREERTOS_UNICORE
            &xPendingReadyList[ 1 ],
        #endif /* CONFIG_FREERTOS_UNICORE */
    #endif /* CONFIG_FREERTOS_SMP */
    &xDelayedTaskList1,
    &xDelayedTaskList2,
    #ಯದಿ ( INCLUDE_vTaskDelete == 1 )
        &xTasksWaitingTermination,
    #endif
    #ಯದಿ ( INCLUDE_vTaskSuspend == 1 )
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
ಸ್ಥಿರ List_t * pxGetTaskListByIndex( UBaseType_t uxListIndex )
{
    List_t * pxTaskList;
    ಧ್ರುವ size_t xNonReadyTaskListsCnt = ( ಪರಿಮಾಣ( non_ready_task_lists ) / ಪರಿಮಾಣ( List_t * ) );

    ಯದಿ( uxListIndex < configMAX_PRIORITIES )
    {
        pxTaskList = &pxReadyTasksLists[ configMAX_PRIORITIES - 1 - uxListIndex ];
    }
    ಅನ್ಯಥಾ ಯದಿ( uxListIndex < configMAX_PRIORITIES + xNonReadyTaskListsCnt )
    {
        pxTaskList = non_ready_task_lists[ uxListIndex - configMAX_PRIORITIES ];
    }
    ಅನ್ಯಥಾ
    {
        pxTaskList = ರಿಕ್ತ;
    }

    /* sanity check */
    ಯದಿ( pxTaskList )
    {
        ಯದಿ( !portVALID_LIST_MEM( pxTaskList ) )
        {
            pxTaskList = ರಿಕ್ತ;
        }
    }

    ಪ್ರತ್ಯಾಗಮ pxTaskList;
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
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ UBaseType_t px_Get_Task_List_ಎಣಿಕೆ( ಶೂನ್ಯ )
{
    ಪ್ರತ್ಯಾಗಮ configMAX_PRIORITIES + ( ಪರಿಮಾಣ( non_ready_task_lists ) / ಪರಿಮಾಣ( List_t * ) );
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
ಪೂರ್ಣಾಙ್ಕ xTaskGetNext( TaskIterator_t * xIterator )
{
    ಯದಿ( !xIterator )
    {
        ಪ್ರತ್ಯಾಗಮ -1;
    }

    ListItem_t * pxNextListItem = xIterator->pxNextListItem;
    UBaseType_t uxCurListIdx = xIterator->uxCurrentListIndex;
    UBaseType_t uxMaxListIdx = px_Get_Task_List_ಎಣಿಕೆ();

    ಪ್ರತಿ( ; uxCurListIdx < uxMaxListIdx; ++uxCurListIdx )
    {
        List_t * pxCurrentTaskList = pxGetTaskListByIndex( uxCurListIdx );

        ಯದಿ( !pxCurrentTaskList || ( listCURRENT_LIST_LENGTH( pxCurrentTaskList ) == 0 ) )
        {
            ಅನುವರ್ತ;
        }

        ಧ್ರುವ ListItem_t * pxCurrListItem = listGET_END_MARKER( pxCurrentTaskList );

        ಯದಿ( !pxNextListItem )
        {
            /* We are here if the traversal starts from the beginning or when we finish traversing
             * for one of the state lists
             */
            pxNextListItem = listGET_NEXT( pxCurrListItem );
        }

        ಯದಿ( !portVALID_LIST_MEM( pxNextListItem ) )
        {
            /* Nothing to do with the corrupted list item. We will skip to the next task state list.
             * pxNextListItem should be NULL at the beginning of each task list.
             */
            pxNextListItem = ರಿಕ್ತ;
            ಅನುವರ್ತ;
        }

        TCB_t * pxTCB = ( TCB_t * ) listGET_LIST_ITEM_OWNER( pxNextListItem );

        ಯದಿ( !portVALID_TCB_MEM( pxTCB ) )
        {
            pxTCB = ರಿಕ್ತ;
        }

        xIterator->pxTaskHandle = pxTCB;
        xIterator->uxCurrentListIndex = uxCurListIdx;

        ಯದಿ( pxCurrListItem->pxPrevious == pxNextListItem )
        {
            /* If this is the last item of the current state list */
            xIterator->uxCurrentListIndex++;
            xIterator->pxNextListItem = ರಿಕ್ತ;
        }
        ಅನ್ಯಥಾ
        {
            xIterator->pxNextListItem = listGET_NEXT( pxNextListItem );
        }

        ಪ್ರತ್ಯಾಗಮ uxCurListIdx;
    }

    ಪ್ರತ್ಯಾಗಮ -1; /* end of the task list */
}
/*----------------------------------------------------------*/

BaseType_t vTaskGetSnapshot( TaskHandle_t pxTask,
                             TaskSnapshot_t * pxTaskSnapshot )
{
    ESP_STATIC_ANALYZER_CHECK(!pxTask, pdFALSE);

    ಯದಿ( ( portVALID_TCB_MEM( pxTask ) == false ) || ( pxTaskSnapshot == ರಿಕ್ತ ) )
    {
        ಪ್ರತ್ಯಾಗಮ pdFALSE;
    }

    TCB_t * pxTCB = ( TCB_t * ) pxTask;
    pxTaskSnapshot->pxTCB = pxTCB;
    pxTaskSnapshot->px_ತುದಿ_Of_ರಾಶಿ = ( ರಾಶಿ_Type_t * ) pxTCB->px_ತುದಿ_Of_ರಾಶಿ;
    pxTaskSnapshot->px_End_Of_ರಾಶಿ = ( ರಾಶಿ_Type_t * ) pxTCB->px_End_Of_ರಾಶಿ;
    ಪ್ರತ್ಯಾಗಮ pdTRUE;
}
/*----------------------------------------------------------*/

UBaseType_t uxTaskGetSnapshotAll( TaskSnapshot_t * ಧ್ರುವ pxTaskSnapshotArray,
                                  ಧ್ರುವ UBaseType_t uxArrayLength,
                                  UBaseType_t * ಧ್ರುವ pxTCBSize )
{
    UBaseType_t ux_Array_ಅಂಕೆ_Filled = 0;

    /* Traverse all of the tasks lists */
    TaskIterator_t xTaskIter = { 0 }; /* Point to the first task list */

    ಯಾವತ್( xTaskGetNext( &xTaskIter ) != -1 && ux_Array_ಅಂಕೆ_Filled < uxArrayLength )
    {
        vTaskGetSnapshot( xTaskIter.pxTaskHandle, &pxTaskSnapshotArray[ ux_Array_ಅಂಕೆ_Filled ] );
        ux_Array_ಅಂಕೆ_Filled++;
    }

    ಯದಿ( pxTCBSize != ರಿಕ್ತ )
    {
        *pxTCBSize = ಪರಿಮಾಣ( TCB_t );
    }

    ಪ್ರತ್ಯಾಗಮ ux_Array_ಅಂಕೆ_Filled;
}
/*----------------------------------------------------------*/

/* ----------------------------------------------------- Misc ----------------------------------------------------- */

ಶೂನ್ಯ * pvTaskGetCurrentTCBForCore( BaseType_t xCoreID )
{
    ಶೂನ್ಯ * pvRet;

    configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

    #ಯದಿ CONFIG_FREERTOS_SMP
        /* SMP FreeRTOS defines pxCurrentTCB as a macro function call */
        pvRet = ( ಶೂನ್ಯ * ) pxCurrentTCB;
    #ಅನ್ಯಥಾ /* CONFIG_FREERTOS_SMP */
        pvRet = ( ಶೂನ್ಯ * ) pxCurrentTCBs[ xCoreID ];
    #endif /* CONFIG_FREERTOS_SMP */
    ಪ್ರತ್ಯಾಗಮ pvRet;
}

/* ----------------------------------------------------- OpenOCD ---------------------------------------------------- */

#ಯದಿ CONFIG_FREERTOS_DEBUG_OCDAWARE

/**
 * Debug param indexes. DO NOT change the order. OpenOCD uses the same indexes
 * Entries in FreeRTOS_openocd_params must match the order of these indexes
 */
    ಗಣನಾ
    {
        ESP_FREERTOS_DEBUG_TABLE_SIZE = 0,
        ESP_FREERTOS_DEBUG_TABLE_VERSION,
        ESP_FREERTOS_DEBUG_KERNEL_VER_MAJOR,
        ESP_FREERTOS_DEBUG_KERNEL_VER_MINOR,
        ESP_FREERTOS_DEBUG_KERNEL_VER_BUILD,
        ESP_FREERTOS_DEBUG_UX_ತುದಿ_USED_PIORITY,
        ESP_FREERTOS_DEBUG_PX_ತುದಿ_OF_ರಾಶಿ,
        ESP_FREERTOS_DEBUG_PC_TASK_NAME,
        ESP_FREERTOS_DEBUG_LIST_SIZE,
        ESP_FREERTOS_DEBUG_LIST_ಅಂಕೆ_ವಸ್ತುಗಳು,
        ESP_FREERTOS_DEBUG_LIST_END,
        ESP_FREERTOS_DEBUG_LIST_END_PREV,
        ESP_FREERTOS_DEBUG_LIST_ITEM_PREV,
        ESP_FREERTOS_DEBUG_LIST_ITEM_OWNER,
        ESP_FREERTOS_DEBUG_TASK_ಎಣಿಕೆ_WIDTH,
        ESP_FREERTOS_DEBUG_PTR_WIDTH,
        /* New entries must be inserted here */
        ESP_FREERTOS_DEBUG_TABLE_END,
    };

    ಧ್ರುವ DRAM_ATTR uint8_t FreeRTOS_openocd_params[ ESP_FREERTOS_DEBUG_TABLE_END ] =
    {
        ESP_FREERTOS_DEBUG_TABLE_END, /* table size */
        1,                            /* table version */
        tskKERNEL_VERSION_MAJOR,
        tskKERNEL_VERSION_MINOR,
        tskKERNEL_VERSION_BUILD,
        configMAX_PRIORITIES - 1,                   /* uxTopUsedPriority */
        offsetof( TCB_t, px_ತುದಿ_Of_ರಾಶಿ ),            /* thread_stack_offset; */
        offsetof( TCB_t, pcTaskName ),              /* thread_name_offset; */
        ಪರಿಮಾಣ( List_t ),                           /* list_width */
        offsetof( List_t, ux_ಸಂಖ್ಯೆ_Of_ವಸ್ತುಗಳು ),        /* list_item_num */
        offsetof( List_t, xListEnd ),               /* list_end_offset */
        offsetof( List_t, xListEnd.pxPrevious ),    /* list_next_offset */
        offsetof( ListItem_t, pxPrevious ),         /* list_elem_next_offset */
        offsetof( ListItem_t, pvOwner ),            /* list_elem_content_offset */
        ಪರಿಮಾಣ( UBaseType_t ),                      /* task_count_width */
        ಪರಿಮಾಣ( ಶೂನ್ಯ * )                            /* ptr_width */
    };

#endif /* CONFIG_FREERTOS_DEBUG_OCDAWARE */
/*----------------------------------------------------------*/

/* ----------------------------------------------------- PSRAM ---------------------------------------------------- */

#ಯದಿ CONFIG_SPIRAM

    #ಯದಿ CONFIG_FREERTOS_SMP
        BaseType_t prvTaskCreateDynamicAffinitySetWithCaps( TaskFunction_t pxTaskCode,
                                                            ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ pcName,
                                                            ಧ್ರುವ config_ರಾಶಿ_DEPTH_TYPE us_ರಾಶಿ_Depth,
                                                            ಶೂನ್ಯ * ಧ್ರುವ pvParameters,
                                                            UBaseType_t uxPriority,
                                                            UBaseType_t uxCoreAffinityMask,
                                                            UBaseType_t ux_ರಾಶಿ_Memory_Caps,
                                                            TaskHandle_t * ಧ್ರುವ pxCreatedTask )
    #ಅನ್ಯಥಾ /* CONFIG_FREERTOS_SMP */
        BaseType_t prvTaskCreateDynamicPinnedToCoreWithCaps( TaskFunction_t pxTaskCode,
                                                             ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ pcName,
                                                             ಧ್ರುವ config_ರಾಶಿ_DEPTH_TYPE us_ರಾಶಿ_Depth,
                                                             ಶೂನ್ಯ * ಧ್ರುವ pvParameters,
                                                             UBaseType_t uxPriority,
                                                             ಧ್ರುವ BaseType_t xCoreID,
                                                             UBaseType_t ux_ರಾಶಿ_Memory_Caps,
                                                             TaskHandle_t * ಧ್ರುವ pxCreatedTask )
    #endif /* CONFIG_FREERTOS_SMP */
    {
        TCB_t * pxNewTCB;
        BaseType_t xReturn;

        ರಾಶಿ_Type_t * px_ರಾಶಿ;

        configASSERT( ux_ರಾಶಿ_Memory_Caps & ( MALLOC_CAP_8BIT ) );
        configASSERT( ( ux_ರಾಶಿ_Memory_Caps & MALLOC_CAP_SPIRAM ) ||
                      ( ux_ರಾಶಿ_Memory_Caps & MALLOC_CAP_INTERNAL ) );
        #ಯದಿ ( !CONFIG_FREERTOS_SMP )
        {
            configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );
        }
        #endif /* !CONFIG_FREERTOS_SMP */

        /* Allocate space for the stack used by the task being created. */
        px_ರಾಶಿ = heap_caps_malloc( ( ( ( size_t ) us_ರಾಶಿ_Depth ) * ಪರಿಮಾಣ( ರಾಶಿ_Type_t ) ), ux_ರಾಶಿ_Memory_Caps );

        ಯದಿ( px_ರಾಶಿ != ರಿಕ್ತ )
        {
            /* Allocate space for the TCB. */
            pxNewTCB = ( TCB_t * ) pvPortMalloc( ಪರಿಮಾಣ( TCB_t ) );

            ಯದಿ( pxNewTCB != ರಿಕ್ತ )
            {
                memset( ( ಶೂನ್ಯ * ) pxNewTCB, 0x00, ಪರಿಮಾಣ( TCB_t ) );

                /* Store the stack location in the TCB. */
                pxNewTCB->px_ರಾಶಿ = px_ರಾಶಿ;
            }
            ಅನ್ಯಥಾ
            {
                /* The stack cannot be used as the TCB has not been created. Free it. */
                heap_caps_free( px_ರಾಶಿ );
            }
        }
        ಅನ್ಯಥಾ
        {
            pxNewTCB = ರಿಕ್ತ;
        }

        ಯದಿ( pxNewTCB != ರಿಕ್ತ )
        {
            #ಯದಿ ( tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE != 0 )
            {
                /* Tasks can be created statically or dynamically, so note this
                 * task was created dynamically in case it is later deleted. */
                pxNewTCB->ucStaticallyAllocated = tsk_DYNAMICALLY_ALLOCATED_ರಾಶಿ_AND_TCB;
            }
            #endif /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

            #ಯದಿ CONFIG_FREERTOS_SMP
            {
                prvInitialiseNewTask( pxTaskCode, pcName, ( uint32_t ) us_ರಾಶಿ_Depth, pvParameters, uxPriority, pxCreatedTask, pxNewTCB, ರಿಕ್ತ );
                #ಯದಿ ( ( config_ಅಂಕೆ_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
                {
                    /* Set the task's affinity before scheduling it */
                    pxNewTCB->uxCoreAffinityMask = uxCoreAffinityMask;
                }
                #endif /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            }
            #ಅನ್ಯಥಾ /* CONFIG_FREERTOS_SMP */
            {
                prvInitialiseNewTask( pxTaskCode, pcName, ( uint32_t ) us_ರಾಶಿ_Depth, pvParameters, uxPriority, pxCreatedTask, pxNewTCB, ರಿಕ್ತ, xCoreID );
            }
            #endif /* CONFIG_FREERTOS_SMP */

            prvAddNewTaskToReadyList( pxNewTCB );
            xReturn = pdPASS;
        }
        ಅನ್ಯಥಾ
        {
            xReturn = errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY;
        }

        ಪ್ರತ್ಯಾಗಮ xReturn;
    }

#endif /* CONFIG_SPIRAM */
/*----------------------------------------------------------*/
