/*
 * SPDX-FileCopyrightText: 2015-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#அந்தர்பாவ "sdkconfig.h"
#அந்தர்பாவ "esp_assert.h"
#அந்தர்பாவ "esp_heap_caps.h"
#அந்தர்பாவ "esp_compiler.h"
#அந்தர்பாவ "freertos/idf_additions.h"
#அந்தர்பாவ "freertos/freertos_debug.h"
#அந்தர்பாவ "esp_private/freertos_idf_additions_priv.h"

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
_Static_assert( offsetof( StaticTask_t, pxDummy6 ) == offsetof( TCB_t, px_அடுக்கு ) );
_Static_assert( offsetof( StaticTask_t, pxDummy8 ) == offsetof( TCB_t, px_End_Of_அடுக்கு ) );
#எனில் !CONFIG_IDF_TARGET_LINUX    // Disabled for linux builds due to differences in types
_Static_assert( tskNO_AFFINITY == ( BaseType_t ) CONFIG_FREERTOS_NO_AFFINITY, "CONFIG_FREERTOS_NO_AFFINITY must be the same as tskNO_AFFINITY" );
#endif

/* ------------------------------------------------- Kernel Control ------------------------------------------------- */

#எனில் ( !CONFIG_FREERTOS_SMP && ( config_இலக்கம்_CORES > 1 ) )

/*
 * Wrapper function to take "xKerneLock"
 */
    வெறுமை prvTakeKernelLock( வெறுமை )
    {
        /* We call the tasks.c critical section macro to take xKernelLock */
        taskENTER_CRITICAL( &xKernelLock );
    }

#endif /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#எனில் ( !CONFIG_FREERTOS_SMP && ( config_இலக்கம்_CORES > 1 ) )

/*
 * Wrapper function to release "xKerneLock"
 */
    வெறுமை prvReleaseKernelLock( வெறுமை )
    {
        /* We call the tasks.c critical section macro to release xKernelLock */
        taskEXIT_CRITICAL( &xKernelLock );
    }

#endif /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#எனில் ( CONFIG_FREERTOS_SMP && ( config_இலக்கம்_CORES > 1 ) )

/*
 * Workaround for non-thread safe multi-core OS startup (see IDF-4524)
 */
    வெறுமை prvStartSchedulerOtherCores( வெறுமை )
    {
        /* This function is always called with interrupts disabled*/
        xSchedulerRunning = pdTRUE;
    }

#endif /* ( CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#எனில் ( !CONFIG_FREERTOS_SMP && ( config_இலக்கம்_CORES > 1 ) )

    BaseType_t xTaskIncrementTickOtherCores( வெறுமை )
    {
        /* Minor optimization. This function can never switch cores mid
         * execution */
        BaseType_t xCoreID = portGET_CORE_ID();
        BaseType_t xSwitchRequired = pdFALSE;

        /* This function should never be called by Core 0. */
        configASSERT( xCoreID != 0 );

        /* Called by the portable layer each time a tick interrupt occurs
         * on a core other than core 0. */
        traceTASK_INCREMENT_TICK( x_Tick_மொத்தம் );

        எனில்( uxSchedulerSuspended[ xCoreID ] == ( UBaseType_t ) 0U )
        {
            /* We need take the kernel lock here as we are about to access
             * kernel data structures. */
            taskENTER_CRITICAL_ISR( &xKernelLock );

            /* Tasks of equal priority to the currently running task will share
             * processing time (time slice) if preemption is on, and the application
             * writer has not explicitly turned time slicing off. */
            #எனில் ( ( configUSE_PREEMPTION == 1 ) && ( configUSE_TIME_SLICING == 1 ) )
            {
                எனில்( listCURRENT_LIST_LENGTH( &( pxReadyTasksLists[ pxCurrentTCBs[ xCoreID ]->uxPriority ] ) ) > ( UBaseType_t ) 1 )
                {
                    xSwitchRequired = pdTRUE;
                }
                இல்லையெனில்
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            #endif /* ( ( configUSE_PREEMPTION == 1 ) && ( configUSE_TIME_SLICING == 1 ) ) */

            /* Release the previously taken kernel lock as we have finished
             * accessing the kernel data structures. */
            taskEXIT_CRITICAL_ISR( &xKernelLock );

            #எனில் ( configUSE_PREEMPTION == 1 )
            {
                எனில்( xYieldPending[ xCoreID ] != pdFALSE )
                {
                    xSwitchRequired = pdTRUE;
                }
                இல்லையெனில்
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            #endif /* configUSE_PREEMPTION */
        }

        #எனில் ( configUSE_TICK_HOOK == 1 )
        {
            vApplicationTickHook();
        }
        #endif

        திரும்பு xSwitchRequired;
    }

#endif /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

/* -------------------------------------------------- Task Creation ------------------------------------------------- */

#எனில் ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )

    BaseType_t xTaskCreatePinnedToCore( TaskFunction_t pxTaskCode,
                                        மாறிலி எழுத்து * மாறிலி pcName,
                                        மாறிலி uint32_t us_அடுக்கு_Depth,
                                        வெறுமை * மாறிலி pvParameters,
                                        UBaseType_t uxPriority,
                                        TaskHandle_t * மாறிலி pxCreatedTask,
                                        மாறிலி BaseType_t xCoreID )
    {
        BaseType_t xReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );

        #எனில் CONFIG_FREERTOS_SMP
        {
            /* If using Amazon SMP FreeRTOS. This function is just a wrapper around
             * xTaskCreate() or xTaskCreateAffinitySet(). */
            #எனில் ( ( config_இலக்கம்_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
            {
                /* Convert xCoreID into an affinity mask */
                UBaseType_t uxCoreAffinityMask;

                /* Bit shifting << xCoreID is only valid if we have less than
                 * 32 cores. */
                ESP_STATIC_ASSERT( config_இலக்கம்_CORES < 32 );

                எனில்( xCoreID == tskNO_AFFINITY )
                {
                    uxCoreAffinityMask = tskNO_AFFINITY;
                }
                இல்லையெனில்
                {
                    uxCoreAffinityMask = ( 1 << xCoreID );
                }

                xReturn = xTaskCreateAffinitySet( pxTaskCode, pcName, us_அடுக்கு_Depth, pvParameters, uxPriority, uxCoreAffinityMask, pxCreatedTask );
            }
            #இல்லையெனில் /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            {
                xReturn = xTaskCreate( pxTaskCode, pcName, us_அடுக்கு_Depth, pvParameters, uxPriority, pxCreatedTask );
            }
            #endif /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
        }
        #இல்லையெனில் /* CONFIG_FREERTOS_SMP */
        {
            TCB_t * pxNewTCB;

            /* If the stack grows down then allocate the stack then the TCB so the
             * stack does not grow into the TCB.  Likewise if the stack grows up
             * then allocate the TCB then the stack. */
            #எனில் ( port_அடுக்கு_GROWTH > 0 )
            {
                /* Allocate space for the TCB.  Where the memory comes from depends on
                 * the implementation of the port malloc function and whether or not static
                 * allocation is being used. */
                pxNewTCB = ( TCB_t * ) pvPortMalloc( அளவு( TCB_t ) );

                எனில்( pxNewTCB != வெற்று )
                {
                    memset( ( வெறுமை * ) pxNewTCB, 0x00, அளவு( TCB_t ) );

                    /* Allocate space for the stack used by the task being created.
                     * The base of the stack memory stored in the TCB so the task can
                     * be deleted later if required. */
                    pxNewTCB->px_அடுக்கு = ( அடுக்கு_Type_t * ) pv_Port_Malloc_அடுக்கு( ( ( ( size_t ) us_அடுக்கு_Depth ) * அளவு( அடுக்கு_Type_t ) ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */

                    எனில்( pxNewTCB->px_அடுக்கு == வெற்று )
                    {
                        /* Could not allocate the stack.  Delete the allocated TCB. */
                        vPortFree( pxNewTCB );
                        pxNewTCB = வெற்று;
                    }
                }
            }
            #இல்லையெனில் /* portSTACK_GROWTH */
            {
                அடுக்கு_Type_t * px_அடுக்கு;

                /* Allocate space for the stack used by the task being created. */
                px_அடுக்கு = pv_Port_Malloc_அடுக்கு( ( ( ( size_t ) us_அடுக்கு_Depth ) * அளவு( அடுக்கு_Type_t ) ) ); /*lint !e9079 All values returned by pvPortMalloc() have at least the alignment required by the MCU's stack and this allocation is the stack. */

                எனில்( px_அடுக்கு != வெற்று )
                {
                    /* Allocate space for the TCB. */
                    pxNewTCB = ( TCB_t * ) pvPortMalloc( அளவு( TCB_t ) ); /*lint !e9087 !e9079 All values returned by pvPortMalloc() have at least the alignment required by the MCU's stack, and the first member of TCB_t is always a pointer to the task's stack. */

                    எனில்( pxNewTCB != வெற்று )
                    {
                        memset( ( வெறுமை * ) pxNewTCB, 0x00, அளவு( TCB_t ) );

                        /* Store the stack location in the TCB. */
                        pxNewTCB->px_அடுக்கு = px_அடுக்கு;
                    }
                    இல்லையெனில்
                    {
                        /* The stack cannot be used as the TCB was not created.  Free
                         * it again. */
                        v_Port_Free_அடுக்கு( px_அடுக்கு );
                    }
                }
                இல்லையெனில்
                {
                    pxNewTCB = வெற்று;
                }
            }
            #endif /* portSTACK_GROWTH */

            எனில்( pxNewTCB != வெற்று )
            {
                #எனில் ( tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE != 0 ) /*lint !e9029 !e731 Macro has been consolidated for readability reasons. */
                {
                    /* Tasks can be created statically or dynamically, so note this
                     * task was created dynamically in case it is later deleted. */
                    pxNewTCB->ucStaticallyAllocated = tsk_DYNAMICALLY_ALLOCATED_அடுக்கு_AND_TCB;
                }
                #endif /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

                prvInitialiseNewTask( pxTaskCode, pcName, ( uint32_t ) us_அடுக்கு_Depth, pvParameters, uxPriority, pxCreatedTask, pxNewTCB, வெற்று, xCoreID );
                prvAddNewTaskToReadyList( pxNewTCB );
                xReturn = pdPASS;
            }
            இல்லையெனில்
            {
                xReturn = errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY;
            }
        }
        #endif /* CONFIG_FREERTOS_SMP */

        திரும்பு xReturn;
    }

#endif /* ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#எனில் ( configSUPPORT_STATIC_ALLOCATION == 1 )

    TaskHandle_t xTaskCreateStaticPinnedToCore( TaskFunction_t pxTaskCode,
                                                மாறிலி எழுத்து * மாறிலி pcName,
                                                மாறிலி uint32_t ul_அடுக்கு_Depth,
                                                வெறுமை * மாறிலி pvParameters,
                                                UBaseType_t uxPriority,
                                                அடுக்கு_Type_t * மாறிலி pux_அடுக்கு_Buffer,
                                                StaticTask_t * மாறிலி pxTaskBuffer,
                                                மாறிலி BaseType_t xCoreID )
    {
        TaskHandle_t xReturn;

        configASSERT( port_VALID_அடுக்கு_MEM( pux_அடுக்கு_Buffer ) );
        configASSERT( portVALID_TCB_MEM( pxTaskBuffer ) );
        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );

        #எனில் CONFIG_FREERTOS_SMP
        {
            /* If using Amazon SMP FreeRTOS. This function is just a wrapper around
             * xTaskCreateStatic() or xTaskCreateStaticAffinitySet(). */
            #எனில் ( ( config_இலக்கம்_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
            {
                /* Convert xCoreID into an affinity mask */
                UBaseType_t uxCoreAffinityMask;

                /* Bit shifting << xCoreID is only valid if we have less than
                 * 32 cores. */
                ESP_STATIC_ASSERT( config_இலக்கம்_CORES < 32 );

                எனில்( xCoreID == tskNO_AFFINITY )
                {
                    uxCoreAffinityMask = tskNO_AFFINITY;
                }
                இல்லையெனில்
                {
                    uxCoreAffinityMask = ( 1 << xCoreID );
                }

                xReturn = xTaskCreateStaticAffinitySet( pxTaskCode, pcName, ul_அடுக்கு_Depth, pvParameters, uxPriority, pux_அடுக்கு_Buffer, pxTaskBuffer, uxCoreAffinityMask );
            }
            #இல்லையெனில் /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            {
                xReturn = xTaskCreateStatic( pxTaskCode, pcName, ul_அடுக்கு_Depth, pvParameters, uxPriority, pux_அடுக்கு_Buffer, pxTaskBuffer );
            }
            #endif /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
        }
        #இல்லையெனில் /* CONFIG_FREERTOS_SMP */
        {
            TCB_t * pxNewTCB;

            #எனில் ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticTask_t equals the size of the real task
                 * structure. */
                நிலையற்ற size_t xSize = அளவு( StaticTask_t );
                configASSERT( xSize == அளவு( TCB_t ) );
                ( வெறுமை ) xSize; /* Prevent lint warning when configASSERT() is not used. */
            }
            #endif /* configASSERT_DEFINED */

            எனில்( ( pxTaskBuffer != வெற்று ) && ( pux_அடுக்கு_Buffer != வெற்று ) )
            {
                /* The memory used for the task's TCB and stack are passed into this
                 * function - use them. */
                pxNewTCB = ( TCB_t * ) pxTaskBuffer; /*lint !e740 !e9087 Unusual cast is ok as the structures are designed to have the same alignment, and the size is checked by an assert. */
                memset( ( வெறுமை * ) pxNewTCB, 0x00, அளவு( TCB_t ) );
                pxNewTCB->px_அடுக்கு = ( அடுக்கு_Type_t * ) pux_அடுக்கு_Buffer;

                #எனில் ( tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE != 0 ) /*lint !e731 !e9029 Macro has been consolidated for readability reasons. */
                {
                    /* Tasks can be created statically or dynamically, so note this
                     * task was created statically in case the task is later deleted. */
                    pxNewTCB->ucStaticallyAllocated = tsk_STATICALLY_ALLOCATED_அடுக்கு_AND_TCB;
                }
                #endif /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

                prvInitialiseNewTask( pxTaskCode, pcName, ul_அடுக்கு_Depth, pvParameters, uxPriority, &xReturn, pxNewTCB, வெற்று, xCoreID );
                prvAddNewTaskToReadyList( pxNewTCB );
            }
            இல்லையெனில்
            {
                xReturn = வெற்று;
            }
        }
        #endif /* CONFIG_FREERTOS_SMP */

        திரும்பு xReturn;
    }

#endif /* ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#எனில் ( configUSE_TIMERS == 1 )

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
    BaseType_t __attribute__( ( weak ) ) xTimerCreateTimerTask( வெறுமை )
    {
        திரும்பு pdPASS;
    }

#endif /* configUSE_TIMERS */
/*----------------------------------------------------------*/

/* ------------------------------------------------- Task Utilities ------------------------------------------------- */

BaseType_t xTaskGetCoreID( TaskHandle_t xTask )
{
    BaseType_t xReturn;

    #எனில் ( config_இலக்கம்_CORES > 1 )
    {
        #எனில் CONFIG_FREERTOS_SMP
            UBaseType_t uxCoreAffinityMask;

            /* Get the core affinity mask and convert it to an ID */
            uxCoreAffinityMask = vTaskCoreAffinityGet( xTask );

            /* If the task is not pinned to a particular core, treat it as tskNO_AFFINITY */
            எனில்( uxCoreAffinityMask & ( uxCoreAffinityMask - 1 ) ) /* If more than one bit set */
            {
                xReturn = tskNO_AFFINITY;
            }
            இல்லையெனில்
            {
                முழு iIndexPlusOne = __builtin_ffs( uxCoreAffinityMask );
                ப்ரதிஜ்ஞா( iIndexPlusOne >= 1 );
                xReturn = iIndexPlusOne - 1;
            }
        #இல்லையெனில் /* CONFIG_FREERTOS_SMP */
            TCB_t * pxTCB;

            pxTCB = prvGetTCBFromHandle( xTask );

            xReturn = pxTCB->xCoreID;
        #endif /* CONFIG_FREERTOS_SMP */
    }
    #இல்லையெனில் /* configNUM_CORES > 1 */
    {
        /* Single-core. Just return a core ID of 0 */
        xReturn = 0;
    }
    #endif /* configNUM_CORES > 1 */

    திரும்பு xReturn;
}
/*----------------------------------------------------------*/

#எனில் ( ( !CONFIG_FREERTOS_SMP ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

    TaskHandle_t xTaskGetIdleTaskHandleForCore( BaseType_t xCoreID )
    {
        /* If xTaskGetIdleTaskHandle() is called before the scheduler has been
         * started, then xIdleTaskHandle will be NULL. */
        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );
        configASSERT( ( xIdleTaskHandle[ xCoreID ] != வெற்று ) );
        திரும்பு xIdleTaskHandle[ xCoreID ];
    }

#endif /* ( ( !CONFIG_FREERTOS_SMP ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*----------------------------------------------------------*/

#எனில் ( ( !CONFIG_FREERTOS_SMP ) && ( ( INCLUDE_xTaskGetCurrentTaskHandle == 1 ) || ( configUSE_MUTEXES == 1 ) ) )

    TaskHandle_t xTaskGetCurrentTaskHandleForCore( BaseType_t xCoreID )
    {
        TaskHandle_t xReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

        #எனில் ( CONFIG_FREERTOS_SMP )
        {
            xReturn = xTaskGetCurrentTaskHandleCPU( ( UBaseType_t ) xCoreID );
        }
        #இல்லையெனில் /* CONFIG_FREERTOS_SMP */
        {
            /* A critical section is not required as this function does not
             * guarantee that the TCB will still be valid when this function
             * returns. */
            xReturn = pxCurrentTCBs[ xCoreID ];
        }
        #endif /* CONFIG_FREERTOS_SMP */

        திரும்பு xReturn;
    }

#endif /* ( ( !CONFIG_FREERTOS_SMP ) && ( ( INCLUDE_xTaskGetCurrentTaskHandle == 1 ) || ( configUSE_MUTEXES == 1 ) ) ) */
/*----------------------------------------------------------*/

#எனில் ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

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

        திரும்பு ulRunTimeCounter;
    }

#endif /* ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*----------------------------------------------------------*/

#எனில் ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

    configRUN_TIME_COUNTER_TYPE ulTaskGetIdleRunTimePercentForCore( BaseType_t xCoreID )
    {
        configRUN_TIME_COUNTER_TYPE ulTotalTime, ulReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

        #ifdef port_ALT_GET_RUN_TIME_COUNTER_மதிப்பு
            port_ALT_GET_RUN_TIME_COUNTER_மதிப்பு( ulTotalTime );
        #இல்லையெனில்
            ulTotalTime = port_GET_RUN_TIME_COUNTER_மதிப்பு();
        #endif

        /* For percentage calculations. */
        ulTotalTime /= ( configRUN_TIME_COUNTER_TYPE ) 100;

        /* Avoid divide by zero errors. */
        எனில்( ulTotalTime > ( configRUN_TIME_COUNTER_TYPE ) 0 )
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
        இல்லையெனில்
        {
            ulReturn = 0;
        }

        திரும்பு ulReturn;
    }

#endif /* ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*-----------------------------------------------------------*/

அடுக்கு_Type_t * x_Task_Get_அடுக்கு_Start( TaskHandle_t xTask )
{
    TCB_t * pxTCB;

    pxTCB = prvGetTCBFromHandle( xTask );
    திரும்பு pxTCB->px_அடுக்கு;
}
/*----------------------------------------------------------*/

uint8_t * px_Task_Get_அடுக்கு_Start( TaskHandle_t xTask )
{
    திரும்பு (uint8_t *)x_Task_Get_அடுக்கு_Start( xTask );
}
/*----------------------------------------------------------*/

#எனில் ( INCLUDE_vTaskPrioritySet == 1 )

    வெறுமை prvTaskPriorityRaise( prvTaskSavedPriority_t * pxSavedPriority,
                               UBaseType_t uxNewPriority )
    {
        TCB_t * pxTCB;
        UBaseType_t uxPriorityUsedOnEntry;

        configASSERT( ( uxNewPriority < configMAX_PRIORITIES ) );

        /* Ensure the new priority is valid. */
        எனில்( uxNewPriority >= ( UBaseType_t ) configMAX_PRIORITIES )
        {
            uxNewPriority = ( UBaseType_t ) configMAX_PRIORITIES - ( UBaseType_t ) 1U;
        }

        #எனில் CONFIG_FREERTOS_SMP
            taskENTER_CRITICAL();
        #இல்லையெனில்
            taskENTER_CRITICAL( &xKernelLock );
        #endif
        {
            pxTCB = prvGetTCBFromHandle( வெற்று );

            #எனில் ( configUSE_MUTEXES == 1 )
            {
                pxSavedPriority->uxPriority = pxTCB->uxPriority;
                pxSavedPriority->uxBasePriority = pxTCB->uxBasePriority;

                /* If uxNewPriority < uxBasePriority, then there is nothing else to
                 * do, as uxBasePriority is always <= uxPriority. */
                எனில்( uxNewPriority > pxTCB->uxBasePriority )
                {
                    pxTCB->uxBasePriority = uxNewPriority;

                    /* Remember the task's current priority before attempting to
                     * change it. If the task's current priority is changed, it must
                     * be done so before moving the task between task lists) in order
                     * for the taskRESET_READY_PRIORITY() macro to function correctly. */
                    uxPriorityUsedOnEntry = pxTCB->uxPriority;

                    எனில்( uxNewPriority > pxTCB->uxPriority )
                    {
                        pxTCB->uxPriority = uxNewPriority;

                        /* Only reset the event list item value if the value is not
                         * being used for anything else. */
                        எனில்( ( list_GET_LIST_ITEM_மதிப்பு( &( pxTCB->xEventListItem ) ) & task_EVENT_LIST_ITEM_மதிப்பு_IN_USE ) == 0UL )
                        {
                            list_SET_LIST_ITEM_மதிப்பு( &( pxTCB->xEventListItem ), ( ( TickType_t ) configMAX_PRIORITIES - ( TickType_t ) uxNewPriority ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */
                        }

                        /* If the task is in the blocked or suspended list we need do
                         * nothing more than change its priority variable. However, if
                         * the task is in a ready list it needs to be removed and placed
                         * in the list appropriate to its new priority. */
                        எனில்( listIS_CONTAINED_WITHIN( &( pxReadyTasksLists[ uxPriorityUsedOnEntry ] ), &( pxTCB->xStateListItem ) ) != pdFALSE )
                        {
                            /* The task is currently in its ready list - remove before
                             * adding it to its new ready list.  As we are in a critical
                             * section we can do this even if the scheduler is suspended. */
                            எனில்( uxListRemove( &( pxTCB->xStateListItem ) ) == ( UBaseType_t ) 0 )
                            {
                                /* It is known that the task is in its ready list so
                                 * there is no need to check again and the port level
                                 * reset macro can be called directly. */
                                portRESET_READY_PRIORITY( uxPriorityUsedOnEntry, ux_உச்சி_Ready_Priority );
                            }

                            prvAddTaskToReadyList( pxTCB );
                        }
                    }
                }
            }
            #இல்லையெனில் /* if ( configUSE_MUTEXES == 1 ) */
            {
                pxSavedPriority->uxPriority = pxTCB->uxPriority;

                எனில்( uxNewPriority > pxTCB->uxPriority )
                {
                    vTaskPrioritySet( வெற்று, uxNewPriority );
                }
            }
            #endif /* if ( configUSE_MUTEXES == 1 ) */
        }
        #எனில் CONFIG_FREERTOS_SMP
            taskEXIT_CRITICAL();
        #இல்லையெனில்
            taskEXIT_CRITICAL( &xKernelLock );
        #endif
    }

#endif /* INCLUDE_vTaskPrioritySet == 1 */
/*----------------------------------------------------------*/

#எனில் ( INCLUDE_vTaskPrioritySet == 1 )

    வெறுமை prvTaskPriorityRestore( prvTaskSavedPriority_t * pxSavedPriority )
    {
        TCB_t * pxTCB;
        UBaseType_t uxNewPriority;
        UBaseType_t uxPriorityUsedOnEntry;
        UBaseType_t uxBasePriorityUsedOnEntry;
        BaseType_t xYieldRequired = pdFALSE;

        #எனில் CONFIG_FREERTOS_SMP
            taskENTER_CRITICAL();
        #இல்லையெனில்
            taskENTER_CRITICAL( &xKernelLock );
        #endif
        {
            pxTCB = prvGetTCBFromHandle( வெற்று );

            #எனில் ( configUSE_MUTEXES == 1 )
            {
                /* If the saved uxBasePriority == the task's uxBasePriority, it means
                 * that prvTaskPriorityRaise() never raised the task's uxBasePriority.
                 * In that case, there is nothing else to do. */
                எனில்( pxSavedPriority->uxBasePriority != pxTCB->uxBasePriority )
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
                    எனில்( pxTCB->uxPriority == uxBasePriorityUsedOnEntry )
                    {
                        எனில்( pxTCB->uxMutexesHeld == 0 )
                        {
                            /* The task may have inherited a priority before prvTaskPriorityRaise()
                             * then disinherited a priority after prvTaskPriorityRaise().
                             * Thus we need set the uxPriority to the saved base priority
                             * so that the task's priority gets restored to the priority
                             * before any inheritance or raising. */
                            pxTCB->uxPriority = pxSavedPriority->uxBasePriority;
                        }
                        இல்லையெனில்
                        {
                            /* The task may have inherited a priority before prvTaskPriorityRaise()
                             * was called. Thus, we need to restore uxPriority to the
                             * "saved uxPriority" so that the task still retains that
                             * inherited priority. */
                            pxTCB->uxPriority = pxSavedPriority->uxPriority;
                        }

                        uxNewPriority = pxTCB->uxPriority;

                        எனில்( uxNewPriority < uxPriorityUsedOnEntry )
                        {
                            /* Setting the priority of the running task down means
                             * there may now be another task of higher priority that
                             * is ready to execute. */
                            xYieldRequired = pdTRUE;
                        }

                        /* Only reset the event list item value if the value is not
                         * being used for anything else. */
                        எனில்( ( list_GET_LIST_ITEM_மதிப்பு( &( pxTCB->xEventListItem ) ) & task_EVENT_LIST_ITEM_மதிப்பு_IN_USE ) == 0UL )
                        {
                            list_SET_LIST_ITEM_மதிப்பு( &( pxTCB->xEventListItem ), ( ( TickType_t ) configMAX_PRIORITIES - ( TickType_t ) uxNewPriority ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */
                        }

                        /* If the task is in the blocked or suspended list we need do
                         * nothing more than change its priority variable. However, if
                         * the task is in a ready list it needs to be removed and placed
                         * in the list appropriate to its new priority. */
                        எனில்( listIS_CONTAINED_WITHIN( &( pxReadyTasksLists[ uxPriorityUsedOnEntry ] ), &( pxTCB->xStateListItem ) ) != pdFALSE )
                        {
                            /* The task is currently in its ready list - remove before
                             * adding it to its new ready list.  As we are in a critical
                             * section we can do this even if the scheduler is suspended. */
                            எனில்( uxListRemove( &( pxTCB->xStateListItem ) ) == ( UBaseType_t ) 0 )
                            {
                                /* It is known that the task is in its ready list so
                                 * there is no need to check again and the port level
                                 * reset macro can be called directly. */
                                portRESET_READY_PRIORITY( uxPriorityUsedOnEntry, ux_உச்சி_Ready_Priority );
                            }

                            prvAddTaskToReadyList( pxTCB );
                        }

                        எனில்( xYieldRequired != pdFALSE )
                        {
                            #எனில் CONFIG_FREERTOS_SMP
                                taskYIELD_TASK_CORE_IF_USING_PREEMPTION( pxTCB );
                            #இல்லையெனில்
                                taskYIELD_IF_USING_PREEMPTION();
                            #endif
                        }
                    }
                }
            }
            #இல்லையெனில் /* if ( configUSE_MUTEXES == 1 ) */
            {
                vTaskPrioritySet( வெற்று, pxSavedPriority->uxPriority );
            }
            #endif /* if ( configUSE_MUTEXES == 1 ) */
        }
        #எனில் CONFIG_FREERTOS_SMP
            taskEXIT_CRITICAL();
        #இல்லையெனில்
            taskEXIT_CRITICAL( &xKernelLock );
        #endif
    }

#endif /* ( INCLUDE_vTaskPrioritySet == 1 ) */
/*----------------------------------------------------------*/

/* --------------------------------------------- TLSP Deletion Callbacks -------------------------------------------- */

#எனில் CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS

    வெறுமை vTaskSetThreadLocalStoragePointerAndDelCallback( TaskHandle_t xTaskToSet,
                                                          BaseType_t xIndex,
                                                          வெறுமை * pv_மதிப்பு,
                                                          TlsDeleteCallbackFunction_t pvDelCallback )
    {
        /* If TLSP deletion callbacks are enabled, then configNUM_THREAD_LOCAL_STORAGE_POINTERS
         * is doubled in size so that the latter half of the pvThreadLocalStoragePointers
         * stores the deletion callbacks. */
        எனில்( xIndex < ( config_இலக்கம்_THREAD_LOCAL_STORAGE_POINTERS / 2 ) )
        {
            TCB_t * pxTCB;

            #எனில் ( config_இலக்கம்_CORES > 1 )
            {
                /* For SMP, we need a critical section as another core could also
                 * update this task's TLSP at the same time. */
                #எனில் CONFIG_FREERTOS_SMP
                {
                    taskENTER_CRITICAL();
                }
                #இல்லையெனில் /* CONFIG_FREERTOS_SMP */
                {
                    taskENTER_CRITICAL( &xKernelLock );
                }
                #endif /* CONFIG_FREERTOS_SMP */
            }
            #endif /* configNUM_CORES > 1 */

            pxTCB = prvGetTCBFromHandle( xTaskToSet );
            /* Store the TLSP by indexing the first half of the array */
            pxTCB->pvThreadLocalStoragePointers[ xIndex ] = pv_மதிப்பு;

            /* Store the TLSP deletion callback by indexing the second half
             * of the array. */
            pxTCB->pvThreadLocalStoragePointers[ ( xIndex + ( config_இலக்கம்_THREAD_LOCAL_STORAGE_POINTERS / 2 ) ) ] = ( வெறுமை * ) pvDelCallback;

            #எனில் ( config_இலக்கம்_CORES > 1 )
            {
                #எனில் CONFIG_FREERTOS_SMP
                {
                    taskEXIT_CRITICAL();
                }
                #இல்லையெனில் /* CONFIG_FREERTOS_SMP */
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

#எனில் ( configUSE_NEWLIB_REENTRANT == 1 )

/**
 * @brief Get reentrancy structure of the current task
 *
 * - This function is required by newlib (when __DYNAMIC_REENT__ is enabled)
 * - It will return a pointer to the current task's reent struct
 * - If FreeRTOS is not running, it will return the global reent struct
 *
 * @return Pointer to a the (current taks's)/(global) reent struct
 */
    கட்டமைப்பு _reent * __getreent( வெறுமை )
    {
        /* No lock needed because if this changes, we won't be running anymore. */
        TCB_t * pxCurTask = ( TCB_t * ) xTaskGetCurrentTaskHandle();
        கட்டமைப்பு _reent * ret;

        எனில்( pxCurTask == வெற்று )
        {
            /* No task running. Return global struct. */
            ret = _GLOBAL_REENT;
        }
        இல்லையெனில்
        {
            /* We have a currently executing task. Return its reentrant struct. */
            ret = &pxCurTask->xTLSBlock;
        }

        திரும்பு ret;
    }

#endif /* configUSE_NEWLIB_REENTRANT == 1 */

/* -------------------------------------------------- Task Snapshot ------------------------------------------------- */

/**
 * @brief List of all task lists in FreeRTOS
 *
 * @note There are currently differing number of task list between SMP FreeRTOS and ESP-IDF FreeRTOS
 */
நிலையான List_t * non_ready_task_lists[] =
{
    #ifdef CONFIG_FREERTOS_SMP
        &xPendingReadyList,
    #இல்லையெனில் /* CONFIG_FREERTOS_SMP */
        &xPendingReadyList[ 0 ],
        #ifndef CONFIG_FREERTOS_UNICORE
            &xPendingReadyList[ 1 ],
        #endif /* CONFIG_FREERTOS_UNICORE */
    #endif /* CONFIG_FREERTOS_SMP */
    &xDelayedTaskList1,
    &xDelayedTaskList2,
    #எனில் ( INCLUDE_vTaskDelete == 1 )
        &xTasksWaitingTermination,
    #endif
    #எனில் ( INCLUDE_vTaskSuspend == 1 )
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
நிலையான List_t * pxGetTaskListByIndex( UBaseType_t uxListIndex )
{
    List_t * pxTaskList;
    மாறிலி size_t xNonReadyTaskListsCnt = ( அளவு( non_ready_task_lists ) / அளவு( List_t * ) );

    எனில்( uxListIndex < configMAX_PRIORITIES )
    {
        pxTaskList = &pxReadyTasksLists[ configMAX_PRIORITIES - 1 - uxListIndex ];
    }
    இல்லையெனில் எனில்( uxListIndex < configMAX_PRIORITIES + xNonReadyTaskListsCnt )
    {
        pxTaskList = non_ready_task_lists[ uxListIndex - configMAX_PRIORITIES ];
    }
    இல்லையெனில்
    {
        pxTaskList = வெற்று;
    }

    /* sanity check */
    எனில்( pxTaskList )
    {
        எனில்( !portVALID_LIST_MEM( pxTaskList ) )
        {
            pxTaskList = வெற்று;
        }
    }

    திரும்பு pxTaskList;
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
நிலையான உள்வரி UBaseType_t px_Get_Task_List_மொத்தம்( வெறுமை )
{
    திரும்பு configMAX_PRIORITIES + ( அளவு( non_ready_task_lists ) / அளவு( List_t * ) );
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
முழு xTaskGetNext( TaskIterator_t * xIterator )
{
    எனில்( !xIterator )
    {
        திரும்பு -1;
    }

    ListItem_t * pxNextListItem = xIterator->pxNextListItem;
    UBaseType_t uxCurListIdx = xIterator->uxCurrentListIndex;
    UBaseType_t uxMaxListIdx = px_Get_Task_List_மொத்தம்();

    க்கு( ; uxCurListIdx < uxMaxListIdx; ++uxCurListIdx )
    {
        List_t * pxCurrentTaskList = pxGetTaskListByIndex( uxCurListIdx );

        எனில்( !pxCurrentTaskList || ( listCURRENT_LIST_LENGTH( pxCurrentTaskList ) == 0 ) )
        {
            தொடர்;
        }

        மாறிலி ListItem_t * pxCurrListItem = listGET_END_MARKER( pxCurrentTaskList );

        எனில்( !pxNextListItem )
        {
            /* We are here if the traversal starts from the beginning or when we finish traversing
             * for one of the state lists
             */
            pxNextListItem = listGET_NEXT( pxCurrListItem );
        }

        எனில்( !portVALID_LIST_MEM( pxNextListItem ) )
        {
            /* Nothing to do with the corrupted list item. We will skip to the next task state list.
             * pxNextListItem should be NULL at the beginning of each task list.
             */
            pxNextListItem = வெற்று;
            தொடர்;
        }

        TCB_t * pxTCB = ( TCB_t * ) listGET_LIST_ITEM_OWNER( pxNextListItem );

        எனில்( !portVALID_TCB_MEM( pxTCB ) )
        {
            pxTCB = வெற்று;
        }

        xIterator->pxTaskHandle = pxTCB;
        xIterator->uxCurrentListIndex = uxCurListIdx;

        எனில்( pxCurrListItem->pxPrevious == pxNextListItem )
        {
            /* If this is the last item of the current state list */
            xIterator->uxCurrentListIndex++;
            xIterator->pxNextListItem = வெற்று;
        }
        இல்லையெனில்
        {
            xIterator->pxNextListItem = listGET_NEXT( pxNextListItem );
        }

        திரும்பு uxCurListIdx;
    }

    திரும்பு -1; /* end of the task list */
}
/*----------------------------------------------------------*/

BaseType_t vTaskGetSnapshot( TaskHandle_t pxTask,
                             TaskSnapshot_t * pxTaskSnapshot )
{
    ESP_STATIC_ANALYZER_CHECK(!pxTask, pdFALSE);

    எனில்( ( portVALID_TCB_MEM( pxTask ) == false ) || ( pxTaskSnapshot == வெற்று ) )
    {
        திரும்பு pdFALSE;
    }

    TCB_t * pxTCB = ( TCB_t * ) pxTask;
    pxTaskSnapshot->pxTCB = pxTCB;
    pxTaskSnapshot->px_உச்சி_Of_அடுக்கு = ( அடுக்கு_Type_t * ) pxTCB->px_உச்சி_Of_அடுக்கு;
    pxTaskSnapshot->px_End_Of_அடுக்கு = ( அடுக்கு_Type_t * ) pxTCB->px_End_Of_அடுக்கு;
    திரும்பு pdTRUE;
}
/*----------------------------------------------------------*/

UBaseType_t uxTaskGetSnapshotAll( TaskSnapshot_t * மாறிலி pxTaskSnapshotArray,
                                  மாறிலி UBaseType_t uxArrayLength,
                                  UBaseType_t * மாறிலி pxTCBSize )
{
    UBaseType_t ux_Array_இலக்கம்_Filled = 0;

    /* Traverse all of the tasks lists */
    TaskIterator_t xTaskIter = { 0 }; /* Point to the first task list */

    போது( xTaskGetNext( &xTaskIter ) != -1 && ux_Array_இலக்கம்_Filled < uxArrayLength )
    {
        vTaskGetSnapshot( xTaskIter.pxTaskHandle, &pxTaskSnapshotArray[ ux_Array_இலக்கம்_Filled ] );
        ux_Array_இலக்கம்_Filled++;
    }

    எனில்( pxTCBSize != வெற்று )
    {
        *pxTCBSize = அளவு( TCB_t );
    }

    திரும்பு ux_Array_இலக்கம்_Filled;
}
/*----------------------------------------------------------*/

/* ----------------------------------------------------- Misc ----------------------------------------------------- */

வெறுமை * pvTaskGetCurrentTCBForCore( BaseType_t xCoreID )
{
    வெறுமை * pvRet;

    configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

    #எனில் CONFIG_FREERTOS_SMP
        /* SMP FreeRTOS defines pxCurrentTCB as a macro function call */
        pvRet = ( வெறுமை * ) pxCurrentTCB;
    #இல்லையெனில் /* CONFIG_FREERTOS_SMP */
        pvRet = ( வெறுமை * ) pxCurrentTCBs[ xCoreID ];
    #endif /* CONFIG_FREERTOS_SMP */
    திரும்பு pvRet;
}

/* ----------------------------------------------------- OpenOCD ---------------------------------------------------- */

#எனில் CONFIG_FREERTOS_DEBUG_OCDAWARE

/**
 * Debug param indexes. DO NOT change the order. OpenOCD uses the same indexes
 * Entries in FreeRTOS_openocd_params must match the order of these indexes
 */
    எண்ணிக்கை
    {
        ESP_FREERTOS_DEBUG_TABLE_SIZE = 0,
        ESP_FREERTOS_DEBUG_TABLE_VERSION,
        ESP_FREERTOS_DEBUG_KERNEL_VER_MAJOR,
        ESP_FREERTOS_DEBUG_KERNEL_VER_MINOR,
        ESP_FREERTOS_DEBUG_KERNEL_VER_BUILD,
        ESP_FREERTOS_DEBUG_UX_உச்சி_USED_PIORITY,
        ESP_FREERTOS_DEBUG_PX_உச்சி_OF_அடுக்கு,
        ESP_FREERTOS_DEBUG_PC_TASK_NAME,
        ESP_FREERTOS_DEBUG_LIST_SIZE,
        ESP_FREERTOS_DEBUG_LIST_இலக்கம்_பொருட்கள்,
        ESP_FREERTOS_DEBUG_LIST_END,
        ESP_FREERTOS_DEBUG_LIST_END_PREV,
        ESP_FREERTOS_DEBUG_LIST_ITEM_PREV,
        ESP_FREERTOS_DEBUG_LIST_ITEM_OWNER,
        ESP_FREERTOS_DEBUG_TASK_மொத்தம்_WIDTH,
        ESP_FREERTOS_DEBUG_PTR_WIDTH,
        /* New entries must be inserted here */
        ESP_FREERTOS_DEBUG_TABLE_END,
    };

    மாறிலி DRAM_ATTR uint8_t FreeRTOS_openocd_params[ ESP_FREERTOS_DEBUG_TABLE_END ] =
    {
        ESP_FREERTOS_DEBUG_TABLE_END, /* table size */
        1,                            /* table version */
        tskKERNEL_VERSION_MAJOR,
        tskKERNEL_VERSION_MINOR,
        tskKERNEL_VERSION_BUILD,
        configMAX_PRIORITIES - 1,                   /* uxTopUsedPriority */
        offsetof( TCB_t, px_உச்சி_Of_அடுக்கு ),            /* thread_stack_offset; */
        offsetof( TCB_t, pcTaskName ),              /* thread_name_offset; */
        அளவு( List_t ),                           /* list_width */
        offsetof( List_t, ux_எண்_Of_பொருட்கள் ),        /* list_item_num */
        offsetof( List_t, xListEnd ),               /* list_end_offset */
        offsetof( List_t, xListEnd.pxPrevious ),    /* list_next_offset */
        offsetof( ListItem_t, pxPrevious ),         /* list_elem_next_offset */
        offsetof( ListItem_t, pvOwner ),            /* list_elem_content_offset */
        அளவு( UBaseType_t ),                      /* task_count_width */
        அளவு( வெறுமை * )                            /* ptr_width */
    };

#endif /* CONFIG_FREERTOS_DEBUG_OCDAWARE */
/*----------------------------------------------------------*/

/* ----------------------------------------------------- PSRAM ---------------------------------------------------- */

#எனில் CONFIG_SPIRAM

    #எனில் CONFIG_FREERTOS_SMP
        BaseType_t prvTaskCreateDynamicAffinitySetWithCaps( TaskFunction_t pxTaskCode,
                                                            மாறிலி எழுத்து * மாறிலி pcName,
                                                            மாறிலி config_அடுக்கு_DEPTH_TYPE us_அடுக்கு_Depth,
                                                            வெறுமை * மாறிலி pvParameters,
                                                            UBaseType_t uxPriority,
                                                            UBaseType_t uxCoreAffinityMask,
                                                            UBaseType_t ux_அடுக்கு_Memory_Caps,
                                                            TaskHandle_t * மாறிலி pxCreatedTask )
    #இல்லையெனில் /* CONFIG_FREERTOS_SMP */
        BaseType_t prvTaskCreateDynamicPinnedToCoreWithCaps( TaskFunction_t pxTaskCode,
                                                             மாறிலி எழுத்து * மாறிலி pcName,
                                                             மாறிலி config_அடுக்கு_DEPTH_TYPE us_அடுக்கு_Depth,
                                                             வெறுமை * மாறிலி pvParameters,
                                                             UBaseType_t uxPriority,
                                                             மாறிலி BaseType_t xCoreID,
                                                             UBaseType_t ux_அடுக்கு_Memory_Caps,
                                                             TaskHandle_t * மாறிலி pxCreatedTask )
    #endif /* CONFIG_FREERTOS_SMP */
    {
        TCB_t * pxNewTCB;
        BaseType_t xReturn;

        அடுக்கு_Type_t * px_அடுக்கு;

        configASSERT( ux_அடுக்கு_Memory_Caps & ( MALLOC_CAP_8BIT ) );
        configASSERT( ( ux_அடுக்கு_Memory_Caps & MALLOC_CAP_SPIRAM ) ||
                      ( ux_அடுக்கு_Memory_Caps & MALLOC_CAP_INTERNAL ) );
        #எனில் ( !CONFIG_FREERTOS_SMP )
        {
            configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );
        }
        #endif /* !CONFIG_FREERTOS_SMP */

        /* Allocate space for the stack used by the task being created. */
        px_அடுக்கு = heap_caps_malloc( ( ( ( size_t ) us_அடுக்கு_Depth ) * அளவு( அடுக்கு_Type_t ) ), ux_அடுக்கு_Memory_Caps );

        எனில்( px_அடுக்கு != வெற்று )
        {
            /* Allocate space for the TCB. */
            pxNewTCB = ( TCB_t * ) pvPortMalloc( அளவு( TCB_t ) );

            எனில்( pxNewTCB != வெற்று )
            {
                memset( ( வெறுமை * ) pxNewTCB, 0x00, அளவு( TCB_t ) );

                /* Store the stack location in the TCB. */
                pxNewTCB->px_அடுக்கு = px_அடுக்கு;
            }
            இல்லையெனில்
            {
                /* The stack cannot be used as the TCB has not been created. Free it. */
                heap_caps_free( px_அடுக்கு );
            }
        }
        இல்லையெனில்
        {
            pxNewTCB = வெற்று;
        }

        எனில்( pxNewTCB != வெற்று )
        {
            #எனில் ( tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE != 0 )
            {
                /* Tasks can be created statically or dynamically, so note this
                 * task was created dynamically in case it is later deleted. */
                pxNewTCB->ucStaticallyAllocated = tsk_DYNAMICALLY_ALLOCATED_அடுக்கு_AND_TCB;
            }
            #endif /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

            #எனில் CONFIG_FREERTOS_SMP
            {
                prvInitialiseNewTask( pxTaskCode, pcName, ( uint32_t ) us_அடுக்கு_Depth, pvParameters, uxPriority, pxCreatedTask, pxNewTCB, வெற்று );
                #எனில் ( ( config_இலக்கம்_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
                {
                    /* Set the task's affinity before scheduling it */
                    pxNewTCB->uxCoreAffinityMask = uxCoreAffinityMask;
                }
                #endif /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            }
            #இல்லையெனில் /* CONFIG_FREERTOS_SMP */
            {
                prvInitialiseNewTask( pxTaskCode, pcName, ( uint32_t ) us_அடுக்கு_Depth, pvParameters, uxPriority, pxCreatedTask, pxNewTCB, வெற்று, xCoreID );
            }
            #endif /* CONFIG_FREERTOS_SMP */

            prvAddNewTaskToReadyList( pxNewTCB );
            xReturn = pdPASS;
        }
        இல்லையெனில்
        {
            xReturn = errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY;
        }

        திரும்பு xReturn;
    }

#endif /* CONFIG_SPIRAM */
/*----------------------------------------------------------*/
