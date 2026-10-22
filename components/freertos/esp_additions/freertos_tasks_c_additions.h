/*
 * SPDX-FileCopyrightText: 2015-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ਅਨ੍ਤਰ੍ਭਾਵ "sdkconfig.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_assert.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_heap_caps.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_compiler.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/idf_additions.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "freertos/freertos_debug.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "esp_private/freertos_idf_additions_priv.h"

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
_Static_assert( offsetof( StaticTask_t, pxDummy6 ) == offsetof( TCB_t, px_ਢੇਰ ) );
_Static_assert( offsetof( StaticTask_t, pxDummy8 ) == offsetof( TCB_t, px_ਸਿਰਾ_ਦਾ_ਢੇਰ ) );
#ਜੇ !CONFIG_IDF_ਨਿਸ਼ਾਨਾ_LINUX    // Disabled for linux builds due to differences in types
_Static_assert( tskNO_AFFINITY == ( ਅਧਾਰ_Type_t ) CONFIG_FREERTOS_NO_AFFINITY, "CONFIG_FREERTOS_NO_AFFINITY must be the same as tskNO_AFFINITY" );
#ਜੇ_ਅੰਤ

/* ------------------------------------------------- Kernel Control ------------------------------------------------- */

#ਜੇ ( !CONFIG_FREERTOS_SMP && ( config_ਅੰਕ_CORES > 1 ) )

/*
 * Wrapper function to take "xKerneLock"
 */
    ਖਾਲੀ prv_Take_Kernel_ਤਾਲਾ( ਖਾਲੀ )
    {
        /* We call the tasks.c critical section macro to take xKernelLock */
        taskENTER_CRITICAL( &x_Kernel_ਤਾਲਾ );
    }

#ਜੇ_ਅੰਤ /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#ਜੇ ( !CONFIG_FREERTOS_SMP && ( config_ਅੰਕ_CORES > 1 ) )

/*
 * Wrapper function to release "xKerneLock"
 */
    ਖਾਲੀ prv_Release_Kernel_ਤਾਲਾ( ਖਾਲੀ )
    {
        /* We call the tasks.c critical section macro to release xKernelLock */
        taskEXIT_CRITICAL( &x_Kernel_ਤਾਲਾ );
    }

#ਜੇ_ਅੰਤ /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#ਜੇ ( CONFIG_FREERTOS_SMP && ( config_ਅੰਕ_CORES > 1 ) )

/*
 * Workaround for non-thread safe multi-core OS startup (see IDF-4524)
 */
    ਖਾਲੀ prv_ਸ਼ੁਰੂ_Scheduler_Other_Cores( ਖਾਲੀ )
    {
        /* This function is always called with interrupts disabled*/
        xSchedulerRunning = pdTRUE;
    }

#ਜੇ_ਅੰਤ /* ( CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#ਜੇ ( !CONFIG_FREERTOS_SMP && ( config_ਅੰਕ_CORES > 1 ) )

    ਅਧਾਰ_Type_t xTaskIncrementTickOtherCores( ਖਾਲੀ )
    {
        /* Minor optimization. This function can never switch cores mid
         * execution */
        ਅਧਾਰ_Type_t xCoreID = ਪੋਰਟ_GET_CORE_ID();
        ਅਧਾਰ_Type_t xSwitchRequired = pdFALSE;

        /* This function should never be called by Core 0. */
        configASSERT( xCoreID != 0 );

        /* Called by the portable layer each time a tick interrupt occurs
         * on a core other than core 0. */
        traceTASK_INCREMENT_TICK( x_Tick_ਗਿਣਤੀ );

        ਜੇ( uxSchedulerSuspended[ xCoreID ] == ( UBaseType_t ) 0U )
        {
            /* We need take the kernel lock here as we are about to access
             * kernel data structures. */
            taskENTER_CRITICAL_ISR( &x_Kernel_ਤਾਲਾ );

            /* Tasks of equal priority to the currently running task will share
             * processing time (time slice) if preemption is on, and the application
             * writer has not explicitly turned time slicing off. */
            #ਜੇ ( ( configUSE_PREEMPTION == 1 ) && ( config_USE_ਸਮਾਂ_SLICING == 1 ) )
            {
                ਜੇ( list_CURRENT_LIST_ਲੰਮਾਈ( &( pxReadyTasksLists[ pxCurrentTCBs[ xCoreID ]->ux_ਤਰਜੀਹ ] ) ) > ( UBaseType_t ) 1 )
                {
                    xSwitchRequired = pdTRUE;
                }
                ਨਹੀਂਤਾਂ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            #ਜੇ_ਅੰਤ /* ( ( configUSE_PREEMPTION == 1 ) && ( configUSE_TIME_SLICING == 1 ) ) */

            /* Release the previously taken kernel lock as we have finished
             * accessing the kernel data structures. */
            taskEXIT_CRITICAL_ISR( &x_Kernel_ਤਾਲਾ );

            #ਜੇ ( configUSE_PREEMPTION == 1 )
            {
                ਜੇ( xYieldPending[ xCoreID ] != pdFALSE )
                {
                    xSwitchRequired = pdTRUE;
                }
                ਨਹੀਂਤਾਂ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            #ਜੇ_ਅੰਤ /* configUSE_PREEMPTION */
        }

        #ਜੇ ( configUSE_TICK_HOOK == 1 )
        {
            vApplicationTickHook();
        }
        #ਜੇ_ਅੰਤ

        ਵਾਪਸ xSwitchRequired;
    }

#ਜੇ_ਅੰਤ /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

/* -------------------------------------------------- Task Creation ------------------------------------------------- */

#ਜੇ ( config_SUPPORT_DYNAMIC_ਵੰਡ == 1 )

    ਅਧਾਰ_Type_t x_Task_Create_Pinned_ਮੰਜ਼ਿਲ_Core( TaskFunction_t px_Task_ਕੋਡ,
                                        ਅਚਲ ਅੱਖਰ * ਅਚਲ pcName,
                                        ਅਚਲ uint32_t us_ਢੇਰ_ਡੂੰਘਾਈ,
                                        ਖਾਲੀ * ਅਚਲ pvParameters,
                                        UBaseType_t ux_ਤਰਜੀਹ,
                                        TaskHandle_t * ਅਚਲ pxCreatedTask,
                                        ਅਚਲ ਅਧਾਰ_Type_t xCoreID )
    {
        ਅਧਾਰ_Type_t xReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );

        #ਜੇ CONFIG_FREERTOS_SMP
        {
            /* If using Amazon SMP FreeRTOS. This function is just a wrapper around
             * xTaskCreate() or xTaskCreateAffinitySet(). */
            #ਜੇ ( ( config_ਅੰਕ_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
            {
                /* Convert xCoreID into an affinity mask */
                UBaseType_t ux_Core_Affinity_ਨਕਾਬ;

                /* Bit shifting << xCoreID is only valid if we have less than
                 * 32 cores. */
                ESP_STATIC_ASSERT( config_ਅੰਕ_CORES < 32 );

                ਜੇ( xCoreID == tskNO_AFFINITY )
                {
                    ux_Core_Affinity_ਨਕਾਬ = tskNO_AFFINITY;
                }
                ਨਹੀਂਤਾਂ
                {
                    ux_Core_Affinity_ਨਕਾਬ = ( 1 << xCoreID );
                }

                xReturn = x_Task_Create_Affinity_ਸੈੱਟ( px_Task_ਕੋਡ, pcName, us_ਢੇਰ_ਡੂੰਘਾਈ, pvParameters, ux_ਤਰਜੀਹ, ux_Core_Affinity_ਨਕਾਬ, pxCreatedTask );
            }
            #ਨਹੀਂਤਾਂ /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            {
                xReturn = xTaskCreate( px_Task_ਕੋਡ, pcName, us_ਢੇਰ_ਡੂੰਘਾਈ, pvParameters, ux_ਤਰਜੀਹ, pxCreatedTask );
            }
            #ਜੇ_ਅੰਤ /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
        }
        #ਨਹੀਂਤਾਂ /* CONFIG_FREERTOS_SMP */
        {
            TCB_t * pxNewTCB;

            /* If the stack grows down then allocate the stack then the TCB so the
             * stack does not grow into the TCB.  Likewise if the stack grows up
             * then allocate the TCB then the stack. */
            #ਜੇ ( ਪੋਰਟ_ਢੇਰ_GROWTH > 0 )
            {
                /* Allocate space for the TCB.  Where the memory comes from depends on
                 * the implementation of the port malloc function and whether or not static
                 * allocation is being used. */
                pxNewTCB = ( TCB_t * ) pv_ਪੋਰਟ_Malloc( ਆਕਾਰ( TCB_t ) );

                ਜੇ( pxNewTCB != ਨਿਰਾ )
                {
                    memset( ( ਖਾਲੀ * ) pxNewTCB, 0x00, ਆਕਾਰ( TCB_t ) );

                    /* Allocate space for the stack used by the task being created.
                     * The base of the stack memory stored in the TCB so the task can
                     * be deleted later if required. */
                    pxNewTCB->px_ਢੇਰ = ( ਢੇਰ_Type_t * ) pv_ਪੋਰਟ_Malloc_ਢੇਰ( ( ( ( size_t ) us_ਢੇਰ_ਡੂੰਘਾਈ ) * ਆਕਾਰ( ਢੇਰ_Type_t ) ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */

                    ਜੇ( pxNewTCB->px_ਢੇਰ == ਨਿਰਾ )
                    {
                        /* Could not allocate the stack.  Delete the allocated TCB. */
                        v_ਪੋਰਟ_Free( pxNewTCB );
                        pxNewTCB = ਨਿਰਾ;
                    }
                }
            }
            #ਨਹੀਂਤਾਂ /* portSTACK_GROWTH */
            {
                ਢੇਰ_Type_t * px_ਢੇਰ;

                /* Allocate space for the stack used by the task being created. */
                px_ਢੇਰ = pv_ਪੋਰਟ_Malloc_ਢੇਰ( ( ( ( size_t ) us_ਢੇਰ_ਡੂੰਘਾਈ ) * ਆਕਾਰ( ਢੇਰ_Type_t ) ) ); /*lint !e9079 All values returned by pvPortMalloc() have at least the alignment required by the MCU's stack and this allocation is the stack. */

                ਜੇ( px_ਢੇਰ != ਨਿਰਾ )
                {
                    /* Allocate space for the TCB. */
                    pxNewTCB = ( TCB_t * ) pv_ਪੋਰਟ_Malloc( ਆਕਾਰ( TCB_t ) ); /*lint !e9087 !e9079 All values returned by pvPortMalloc() have at least the alignment required by the MCU's stack, and the first member of TCB_t is always a pointer to the task's stack. */

                    ਜੇ( pxNewTCB != ਨਿਰਾ )
                    {
                        memset( ( ਖਾਲੀ * ) pxNewTCB, 0x00, ਆਕਾਰ( TCB_t ) );

                        /* Store the stack location in the TCB. */
                        pxNewTCB->px_ਢੇਰ = px_ਢੇਰ;
                    }
                    ਨਹੀਂਤਾਂ
                    {
                        /* The stack cannot be used as the TCB was not created.  Free
                         * it again. */
                        v_ਪੋਰਟ_Free_ਢੇਰ( px_ਢੇਰ );
                    }
                }
                ਨਹੀਂਤਾਂ
                {
                    pxNewTCB = ਨਿਰਾ;
                }
            }
            #ਜੇ_ਅੰਤ /* portSTACK_GROWTH */

            ਜੇ( pxNewTCB != ਨਿਰਾ )
            {
                #ਜੇ ( tsk_STATIC_AND_DYNAMIC_ਵੰਡ_POSSIBLE != 0 ) /*lint !e9029 !e731 Macro has been consolidated for readability reasons. */
                {
                    /* Tasks can be created statically or dynamically, so note this
                     * task was created dynamically in case it is later deleted. */
                    pxNewTCB->ucStaticallyAllocated = tsk_DYNAMICALLY_ALLOCATED_ਢੇਰ_AND_TCB;
                }
                #ਜੇ_ਅੰਤ /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

                prvInitialiseNewTask( px_Task_ਕੋਡ, pcName, ( uint32_t ) us_ਢੇਰ_ਡੂੰਘਾਈ, pvParameters, ux_ਤਰਜੀਹ, pxCreatedTask, pxNewTCB, ਨਿਰਾ, xCoreID );
                prv_ਜੋੜੋ_New_Task_ਮੰਜ਼ਿਲ_Ready_List( pxNewTCB );
                xReturn = pdPASS;
            }
            ਨਹੀਂਤਾਂ
            {
                xReturn = err_COULD_NOT_ALLOCATE_REQUIRED_ਸਮ੍ਰਿਤੀ;
            }
        }
        #ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_SMP */

        ਵਾਪਸ xReturn;
    }

#ਜੇ_ਅੰਤ /* ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#ਜੇ ( config_SUPPORT_STATIC_ਵੰਡ == 1 )

    TaskHandle_t x_Task_Create_Static_Pinned_ਮੰਜ਼ਿਲ_Core( TaskFunction_t px_Task_ਕੋਡ,
                                                ਅਚਲ ਅੱਖਰ * ਅਚਲ pcName,
                                                ਅਚਲ uint32_t ul_ਢੇਰ_ਡੂੰਘਾਈ,
                                                ਖਾਲੀ * ਅਚਲ pvParameters,
                                                UBaseType_t ux_ਤਰਜੀਹ,
                                                ਢੇਰ_Type_t * ਅਚਲ pux_ਢੇਰ_ਬਫਰ,
                                                StaticTask_t * ਅਚਲ px_Task_ਬਫਰ,
                                                ਅਚਲ ਅਧਾਰ_Type_t xCoreID )
    {
        TaskHandle_t xReturn;

        configASSERT( ਪੋਰਟ_VALID_ਢੇਰ_MEM( pux_ਢੇਰ_ਬਫਰ ) );
        configASSERT( ਪੋਰਟ_VALID_TCB_MEM( px_Task_ਬਫਰ ) );
        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );

        #ਜੇ CONFIG_FREERTOS_SMP
        {
            /* If using Amazon SMP FreeRTOS. This function is just a wrapper around
             * xTaskCreateStatic() or xTaskCreateStaticAffinitySet(). */
            #ਜੇ ( ( config_ਅੰਕ_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
            {
                /* Convert xCoreID into an affinity mask */
                UBaseType_t ux_Core_Affinity_ਨਕਾਬ;

                /* Bit shifting << xCoreID is only valid if we have less than
                 * 32 cores. */
                ESP_STATIC_ASSERT( config_ਅੰਕ_CORES < 32 );

                ਜੇ( xCoreID == tskNO_AFFINITY )
                {
                    ux_Core_Affinity_ਨਕਾਬ = tskNO_AFFINITY;
                }
                ਨਹੀਂਤਾਂ
                {
                    ux_Core_Affinity_ਨਕਾਬ = ( 1 << xCoreID );
                }

                xReturn = x_Task_Create_Static_Affinity_ਸੈੱਟ( px_Task_ਕੋਡ, pcName, ul_ਢੇਰ_ਡੂੰਘਾਈ, pvParameters, ux_ਤਰਜੀਹ, pux_ਢੇਰ_ਬਫਰ, px_Task_ਬਫਰ, ux_Core_Affinity_ਨਕਾਬ );
            }
            #ਨਹੀਂਤਾਂ /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            {
                xReturn = xTaskCreateStatic( px_Task_ਕੋਡ, pcName, ul_ਢੇਰ_ਡੂੰਘਾਈ, pvParameters, ux_ਤਰਜੀਹ, pux_ਢੇਰ_ਬਫਰ, px_Task_ਬਫਰ );
            }
            #ਜੇ_ਅੰਤ /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
        }
        #ਨਹੀਂਤਾਂ /* CONFIG_FREERTOS_SMP */
        {
            TCB_t * pxNewTCB;

            #ਜੇ ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticTask_t equals the size of the real task
                 * structure. */
                ਅਸਥਿਰ size_t x_ਮਾਪ = ਆਕਾਰ( StaticTask_t );
                configASSERT( x_ਮਾਪ == ਆਕਾਰ( TCB_t ) );
                ( ਖਾਲੀ ) x_ਮਾਪ; /* Prevent lint warning when configASSERT() is not used. */
            }
            #ਜੇ_ਅੰਤ /* configASSERT_DEFINED */

            ਜੇ( ( px_Task_ਬਫਰ != ਨਿਰਾ ) && ( pux_ਢੇਰ_ਬਫਰ != ਨਿਰਾ ) )
            {
                /* The memory used for the task's TCB and stack are passed into this
                 * function - use them. */
                pxNewTCB = ( TCB_t * ) px_Task_ਬਫਰ; /*lint !e740 !e9087 Unusual cast is ok as the structures are designed to have the same alignment, and the size is checked by an assert. */
                memset( ( ਖਾਲੀ * ) pxNewTCB, 0x00, ਆਕਾਰ( TCB_t ) );
                pxNewTCB->px_ਢੇਰ = ( ਢੇਰ_Type_t * ) pux_ਢੇਰ_ਬਫਰ;

                #ਜੇ ( tsk_STATIC_AND_DYNAMIC_ਵੰਡ_POSSIBLE != 0 ) /*lint !e731 !e9029 Macro has been consolidated for readability reasons. */
                {
                    /* Tasks can be created statically or dynamically, so note this
                     * task was created statically in case the task is later deleted. */
                    pxNewTCB->ucStaticallyAllocated = tsk_STATICALLY_ALLOCATED_ਢੇਰ_AND_TCB;
                }
                #ਜੇ_ਅੰਤ /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

                prvInitialiseNewTask( px_Task_ਕੋਡ, pcName, ul_ਢੇਰ_ਡੂੰਘਾਈ, pvParameters, ux_ਤਰਜੀਹ, &xReturn, pxNewTCB, ਨਿਰਾ, xCoreID );
                prv_ਜੋੜੋ_New_Task_ਮੰਜ਼ਿਲ_Ready_List( pxNewTCB );
            }
            ਨਹੀਂਤਾਂ
            {
                xReturn = ਨਿਰਾ;
            }
        }
        #ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_SMP */

        ਵਾਪਸ xReturn;
    }

#ਜੇ_ਅੰਤ /* ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#ਜੇ ( configUSE_TIMERS == 1 )

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
    ਅਧਾਰ_Type_t __attribute__( ( weak ) ) xTimerCreateTimerTask( ਖਾਲੀ )
    {
        ਵਾਪਸ pdPASS;
    }

#ਜੇ_ਅੰਤ /* configUSE_TIMERS */
/*----------------------------------------------------------*/

/* ------------------------------------------------- Task Utilities ------------------------------------------------- */

ਅਧਾਰ_Type_t xTaskGetCoreID( TaskHandle_t xTask )
{
    ਅਧਾਰ_Type_t xReturn;

    #ਜੇ ( config_ਅੰਕ_CORES > 1 )
    {
        #ਜੇ CONFIG_FREERTOS_SMP
            UBaseType_t ux_Core_Affinity_ਨਕਾਬ;

            /* Get the core affinity mask and convert it to an ID */
            ux_Core_Affinity_ਨਕਾਬ = vTaskCoreAffinityGet( xTask );

            /* If the task is not pinned to a particular core, treat it as tskNO_AFFINITY */
            ਜੇ( ux_Core_Affinity_ਨਕਾਬ & ( ux_Core_Affinity_ਨਕਾਬ - 1 ) ) /* If more than one bit set */
            {
                xReturn = tskNO_AFFINITY;
            }
            ਨਹੀਂਤਾਂ
            {
                ਪੂਰਨ i_ਸੂਚਕਾਂਕ_Plus_One = __builtin_ffs( ux_Core_Affinity_ਨਕਾਬ );
                ਪ੍ਰਤਿਜ੍ਞਾ( i_ਸੂਚਕਾਂਕ_Plus_One >= 1 );
                xReturn = i_ਸੂਚਕਾਂਕ_Plus_One - 1;
            }
        #ਨਹੀਂਤਾਂ /* CONFIG_FREERTOS_SMP */
            TCB_t * pxTCB;

            pxTCB = prvGetTCBFromHandle( xTask );

            xReturn = pxTCB->xCoreID;
        #ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_SMP */
    }
    #ਨਹੀਂਤਾਂ /* configNUM_CORES > 1 */
    {
        /* Single-core. Just return a core ID of 0 */
        xReturn = 0;
    }
    #ਜੇ_ਅੰਤ /* configNUM_CORES > 1 */

    ਵਾਪਸ xReturn;
}
/*----------------------------------------------------------*/

#ਜੇ ( ( !CONFIG_FREERTOS_SMP ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

    TaskHandle_t xTaskGetIdleTaskHandleForCore( ਅਧਾਰ_Type_t xCoreID )
    {
        /* If xTaskGetIdleTaskHandle() is called before the scheduler has been
         * started, then xIdleTaskHandle will be NULL. */
        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );
        configASSERT( ( xIdleTaskHandle[ xCoreID ] != ਨਿਰਾ ) );
        ਵਾਪਸ xIdleTaskHandle[ xCoreID ];
    }

#ਜੇ_ਅੰਤ /* ( ( !CONFIG_FREERTOS_SMP ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*----------------------------------------------------------*/

#ਜੇ ( ( !CONFIG_FREERTOS_SMP ) && ( ( INCLUDE_xTaskGetCurrentTaskHandle == 1 ) || ( configUSE_MUTEXES == 1 ) ) )

    TaskHandle_t xTaskGetCurrentTaskHandleForCore( ਅਧਾਰ_Type_t xCoreID )
    {
        TaskHandle_t xReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

        #ਜੇ ( CONFIG_FREERTOS_SMP )
        {
            xReturn = xTaskGetCurrentTaskHandleCPU( ( UBaseType_t ) xCoreID );
        }
        #ਨਹੀਂਤਾਂ /* CONFIG_FREERTOS_SMP */
        {
            /* A critical section is not required as this function does not
             * guarantee that the TCB will still be valid when this function
             * returns. */
            xReturn = pxCurrentTCBs[ xCoreID ];
        }
        #ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_SMP */

        ਵਾਪਸ xReturn;
    }

#ਜੇ_ਅੰਤ /* ( ( !CONFIG_FREERTOS_SMP ) && ( ( INCLUDE_xTaskGetCurrentTaskHandle == 1 ) || ( configUSE_MUTEXES == 1 ) ) ) */
/*----------------------------------------------------------*/

#ਜੇ ( !CONFIG_FREERTOS_SMP && ( config_GENERATE_ਲੜੀਵਾਰ_ਸਮਾਂ_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

    config_ਲੜੀਵਾਰ_ਸਮਾਂ_COUNTER_TYPE ul_Task_Get_Idle_ਲੜੀਵਾਰ_ਸਮਾਂ_Counter_For_Core( ਅਧਾਰ_Type_t xCoreID )
    {
        config_ਲੜੀਵਾਰ_ਸਮਾਂ_COUNTER_TYPE ul_ਲੜੀਵਾਰ_ਸਮਾਂ_Counter;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

        /* For SMP, we need to take the kernel lock here as we are about to
         * access kernel data structures. */
        prvENTER_CRITICAL_SMP_ONLY( &x_Kernel_ਤਾਲਾ );
        {
            ul_ਲੜੀਵਾਰ_ਸਮਾਂ_Counter = xIdleTaskHandle[ xCoreID ]->ul_ਲੜੀਵਾਰ_ਸਮਾਂ_Counter;
        }
        /* Release the previously taken kernel lock. */
        prvEXIT_CRITICAL_SMP_ONLY( &x_Kernel_ਤਾਲਾ );

        ਵਾਪਸ ul_ਲੜੀਵਾਰ_ਸਮਾਂ_Counter;
    }

#ਜੇ_ਅੰਤ /* ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*----------------------------------------------------------*/

#ਜੇ ( !CONFIG_FREERTOS_SMP && ( config_GENERATE_ਲੜੀਵਾਰ_ਸਮਾਂ_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

    config_ਲੜੀਵਾਰ_ਸਮਾਂ_COUNTER_TYPE ul_Task_Get_Idle_ਲੜੀਵਾਰ_ਸਮਾਂ_Percent_For_Core( ਅਧਾਰ_Type_t xCoreID )
    {
        config_ਲੜੀਵਾਰ_ਸਮਾਂ_COUNTER_TYPE ul_ਕੁੱਲ_ਸਮਾਂ, ulReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

        #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਪੋਰਟ_ALT_GET_ਲੜੀਵਾਰ_ਸਮਾਂ_COUNTER_ਮੁੱਲ
            ਪੋਰਟ_ALT_GET_ਲੜੀਵਾਰ_ਸਮਾਂ_COUNTER_ਮੁੱਲ( ul_ਕੁੱਲ_ਸਮਾਂ );
        #ਨਹੀਂਤਾਂ
            ul_ਕੁੱਲ_ਸਮਾਂ = ਪੋਰਟ_GET_ਲੜੀਵਾਰ_ਸਮਾਂ_COUNTER_ਮੁੱਲ();
        #ਜੇ_ਅੰਤ

        /* For percentage calculations. */
        ul_ਕੁੱਲ_ਸਮਾਂ /= ( config_ਲੜੀਵਾਰ_ਸਮਾਂ_COUNTER_TYPE ) 100;

        /* Avoid divide by zero errors. */
        ਜੇ( ul_ਕੁੱਲ_ਸਮਾਂ > ( config_ਲੜੀਵਾਰ_ਸਮਾਂ_COUNTER_TYPE ) 0 )
        {
            /* For SMP, we need to take the kernel lock here as we are about
             * to access kernel data structures. */
            prvENTER_CRITICAL_SMP_ONLY( &x_Kernel_ਤਾਲਾ );
            {
                ulReturn = xIdleTaskHandle[ xCoreID ]->ul_ਲੜੀਵਾਰ_ਸਮਾਂ_Counter / ul_ਕੁੱਲ_ਸਮਾਂ;
            }
            /* Release the previously taken kernel lock. */
            prvEXIT_CRITICAL_SMP_ONLY( &x_Kernel_ਤਾਲਾ );
        }
        ਨਹੀਂਤਾਂ
        {
            ulReturn = 0;
        }

        ਵਾਪਸ ulReturn;
    }

#ਜੇ_ਅੰਤ /* ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*-----------------------------------------------------------*/

ਢੇਰ_Type_t * x_Task_Get_ਢੇਰ_ਸ਼ੁਰੂ( TaskHandle_t xTask )
{
    TCB_t * pxTCB;

    pxTCB = prvGetTCBFromHandle( xTask );
    ਵਾਪਸ pxTCB->px_ਢੇਰ;
}
/*----------------------------------------------------------*/

uint8_t * px_Task_Get_ਢੇਰ_ਸ਼ੁਰੂ( TaskHandle_t xTask )
{
    ਵਾਪਸ (uint8_t *)x_Task_Get_ਢੇਰ_ਸ਼ੁਰੂ( xTask );
}
/*----------------------------------------------------------*/

#ਜੇ ( INCLUDE_v_Task_ਤਰਜੀਹ_ਸੈੱਟ == 1 )

    ਖਾਲੀ prv_Task_ਤਰਜੀਹ_Raise( prv_Task_Saved_ਤਰਜੀਹ_t * px_Saved_ਤਰਜੀਹ,
                               UBaseType_t ux_New_ਤਰਜੀਹ )
    {
        TCB_t * pxTCB;
        UBaseType_t ux_ਤਰਜੀਹ_ਵਰਤਿਆ_On_ਇੰਦਰਾਜ;

        configASSERT( ( ux_New_ਤਰਜੀਹ < configMAX_PRIORITIES ) );

        /* Ensure the new priority is valid. */
        ਜੇ( ux_New_ਤਰਜੀਹ >= ( UBaseType_t ) configMAX_PRIORITIES )
        {
            ux_New_ਤਰਜੀਹ = ( UBaseType_t ) configMAX_PRIORITIES - ( UBaseType_t ) 1U;
        }

        #ਜੇ CONFIG_FREERTOS_SMP
            taskENTER_CRITICAL();
        #ਨਹੀਂਤਾਂ
            taskENTER_CRITICAL( &x_Kernel_ਤਾਲਾ );
        #ਜੇ_ਅੰਤ
        {
            pxTCB = prvGetTCBFromHandle( ਨਿਰਾ );

            #ਜੇ ( configUSE_MUTEXES == 1 )
            {
                px_Saved_ਤਰਜੀਹ->ux_ਤਰਜੀਹ = pxTCB->ux_ਤਰਜੀਹ;
                px_Saved_ਤਰਜੀਹ->ux_ਅਧਾਰ_ਤਰਜੀਹ = pxTCB->ux_ਅਧਾਰ_ਤਰਜੀਹ;

                /* If uxNewPriority < uxBasePriority, then there is nothing else to
                 * do, as uxBasePriority is always <= uxPriority. */
                ਜੇ( ux_New_ਤਰਜੀਹ > pxTCB->ux_ਅਧਾਰ_ਤਰਜੀਹ )
                {
                    pxTCB->ux_ਅਧਾਰ_ਤਰਜੀਹ = ux_New_ਤਰਜੀਹ;

                    /* Remember the task's current priority before attempting to
                     * change it. If the task's current priority is changed, it must
                     * be done so before moving the task between task lists) in order
                     * for the taskRESET_READY_PRIORITY() macro to function correctly. */
                    ux_ਤਰਜੀਹ_ਵਰਤਿਆ_On_ਇੰਦਰਾਜ = pxTCB->ux_ਤਰਜੀਹ;

                    ਜੇ( ux_New_ਤਰਜੀਹ > pxTCB->ux_ਤਰਜੀਹ )
                    {
                        pxTCB->ux_ਤਰਜੀਹ = ux_New_ਤਰਜੀਹ;

                        /* Only reset the event list item value if the value is not
                         * being used for anything else. */
                        ਜੇ( ( list_GET_LIST_ਵਸਤੂ_ਮੁੱਲ( &( pxTCB->x_Event_List_ਵਸਤੂ ) ) & task_EVENT_LIST_ਵਸਤੂ_ਮੁੱਲ_ਵਿੱਚ_USE ) == 0UL )
                        {
                            list_ਸੈੱਟ_LIST_ਵਸਤੂ_ਮੁੱਲ( &( pxTCB->x_Event_List_ਵਸਤੂ ), ( ( TickType_t ) configMAX_PRIORITIES - ( TickType_t ) ux_New_ਤਰਜੀਹ ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */
                        }

                        /* If the task is in the blocked or suspended list we need do
                         * nothing more than change its priority variable. However, if
                         * the task is in a ready list it needs to be removed and placed
                         * in the list appropriate to its new priority. */
                        ਜੇ( listIS_CONTAINED_WITHIN( &( pxReadyTasksLists[ ux_ਤਰਜੀਹ_ਵਰਤਿਆ_On_ਇੰਦਰਾਜ ] ), &( pxTCB->x_State_List_ਵਸਤੂ ) ) != pdFALSE )
                        {
                            /* The task is currently in its ready list - remove before
                             * adding it to its new ready list.  As we are in a critical
                             * section we can do this even if the scheduler is suspended. */
                            ਜੇ( uxListRemove( &( pxTCB->x_State_List_ਵਸਤੂ ) ) == ( UBaseType_t ) 0 )
                            {
                                /* It is known that the task is in its ready list so
                                 * there is no need to check again and the port level
                                 * reset macro can be called directly. */
                                ਪੋਰਟ_RESET_READY_ਤਰਜੀਹ( ux_ਤਰਜੀਹ_ਵਰਤਿਆ_On_ਇੰਦਰਾਜ, ux_ਸਿਖਰ_Ready_ਤਰਜੀਹ );
                            }

                            prv_ਜੋੜੋ_Task_ਮੰਜ਼ਿਲ_Ready_List( pxTCB );
                        }
                    }
                }
            }
            #ਨਹੀਂਤਾਂ /* if ( configUSE_MUTEXES == 1 ) */
            {
                px_Saved_ਤਰਜੀਹ->ux_ਤਰਜੀਹ = pxTCB->ux_ਤਰਜੀਹ;

                ਜੇ( ux_New_ਤਰਜੀਹ > pxTCB->ux_ਤਰਜੀਹ )
                {
                    v_Task_ਤਰਜੀਹ_ਸੈੱਟ( ਨਿਰਾ, ux_New_ਤਰਜੀਹ );
                }
            }
            #ਜੇ_ਅੰਤ /* if ( configUSE_MUTEXES == 1 ) */
        }
        #ਜੇ CONFIG_FREERTOS_SMP
            taskEXIT_CRITICAL();
        #ਨਹੀਂਤਾਂ
            taskEXIT_CRITICAL( &x_Kernel_ਤਾਲਾ );
        #ਜੇ_ਅੰਤ
    }

#ਜੇ_ਅੰਤ /* INCLUDE_vTaskPrioritySet == 1 */
/*----------------------------------------------------------*/

#ਜੇ ( INCLUDE_v_Task_ਤਰਜੀਹ_ਸੈੱਟ == 1 )

    ਖਾਲੀ prv_Task_ਤਰਜੀਹ_Restore( prv_Task_Saved_ਤਰਜੀਹ_t * px_Saved_ਤਰਜੀਹ )
    {
        TCB_t * pxTCB;
        UBaseType_t ux_New_ਤਰਜੀਹ;
        UBaseType_t ux_ਤਰਜੀਹ_ਵਰਤਿਆ_On_ਇੰਦਰਾਜ;
        UBaseType_t ux_ਅਧਾਰ_ਤਰਜੀਹ_ਵਰਤਿਆ_On_ਇੰਦਰਾਜ;
        ਅਧਾਰ_Type_t xYieldRequired = pdFALSE;

        #ਜੇ CONFIG_FREERTOS_SMP
            taskENTER_CRITICAL();
        #ਨਹੀਂਤਾਂ
            taskENTER_CRITICAL( &x_Kernel_ਤਾਲਾ );
        #ਜੇ_ਅੰਤ
        {
            pxTCB = prvGetTCBFromHandle( ਨਿਰਾ );

            #ਜੇ ( configUSE_MUTEXES == 1 )
            {
                /* If the saved uxBasePriority == the task's uxBasePriority, it means
                 * that prvTaskPriorityRaise() never raised the task's uxBasePriority.
                 * In that case, there is nothing else to do. */
                ਜੇ( px_Saved_ਤਰਜੀਹ->ux_ਅਧਾਰ_ਤਰਜੀਹ != pxTCB->ux_ਅਧਾਰ_ਤਰਜੀਹ )
                {
                    ux_ਅਧਾਰ_ਤਰਜੀਹ_ਵਰਤਿਆ_On_ਇੰਦਰਾਜ = pxTCB->ux_ਅਧਾਰ_ਤਰਜੀਹ;
                    pxTCB->ux_ਅਧਾਰ_ਤਰਜੀਹ = px_Saved_ਤਰਜੀਹ->ux_ਅਧਾਰ_ਤਰਜੀਹ;

                    /* Remember the task's current priority before attempting to
                     * change it. If the task's current priority is changed, it must
                     * be done so before moving the task between task lists in order
                     * for the taskRESET_READY_PRIORITY() macro to function correctly. */
                    ux_ਤਰਜੀਹ_ਵਰਤਿਆ_On_ਇੰਦਰਾਜ = pxTCB->ux_ਤਰਜੀਹ;

                    /* Check if the task inherited a priority after prvTaskPriorityRaise().
                     * If this is the case, there is nothing else to do. The priority
                     * will be restored when the task disinherits its priority. */
                    ਜੇ( pxTCB->ux_ਤਰਜੀਹ == ux_ਅਧਾਰ_ਤਰਜੀਹ_ਵਰਤਿਆ_On_ਇੰਦਰਾਜ )
                    {
                        ਜੇ( pxTCB->uxMutexesHeld == 0 )
                        {
                            /* The task may have inherited a priority before prvTaskPriorityRaise()
                             * then disinherited a priority after prvTaskPriorityRaise().
                             * Thus we need set the uxPriority to the saved base priority
                             * so that the task's priority gets restored to the priority
                             * before any inheritance or raising. */
                            pxTCB->ux_ਤਰਜੀਹ = px_Saved_ਤਰਜੀਹ->ux_ਅਧਾਰ_ਤਰਜੀਹ;
                        }
                        ਨਹੀਂਤਾਂ
                        {
                            /* The task may have inherited a priority before prvTaskPriorityRaise()
                             * was called. Thus, we need to restore uxPriority to the
                             * "saved uxPriority" so that the task still retains that
                             * inherited priority. */
                            pxTCB->ux_ਤਰਜੀਹ = px_Saved_ਤਰਜੀਹ->ux_ਤਰਜੀਹ;
                        }

                        ux_New_ਤਰਜੀਹ = pxTCB->ux_ਤਰਜੀਹ;

                        ਜੇ( ux_New_ਤਰਜੀਹ < ux_ਤਰਜੀਹ_ਵਰਤਿਆ_On_ਇੰਦਰਾਜ )
                        {
                            /* Setting the priority of the running task down means
                             * there may now be another task of higher priority that
                             * is ready to execute. */
                            xYieldRequired = pdTRUE;
                        }

                        /* Only reset the event list item value if the value is not
                         * being used for anything else. */
                        ਜੇ( ( list_GET_LIST_ਵਸਤੂ_ਮੁੱਲ( &( pxTCB->x_Event_List_ਵਸਤੂ ) ) & task_EVENT_LIST_ਵਸਤੂ_ਮੁੱਲ_ਵਿੱਚ_USE ) == 0UL )
                        {
                            list_ਸੈੱਟ_LIST_ਵਸਤੂ_ਮੁੱਲ( &( pxTCB->x_Event_List_ਵਸਤੂ ), ( ( TickType_t ) configMAX_PRIORITIES - ( TickType_t ) ux_New_ਤਰਜੀਹ ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */
                        }

                        /* If the task is in the blocked or suspended list we need do
                         * nothing more than change its priority variable. However, if
                         * the task is in a ready list it needs to be removed and placed
                         * in the list appropriate to its new priority. */
                        ਜੇ( listIS_CONTAINED_WITHIN( &( pxReadyTasksLists[ ux_ਤਰਜੀਹ_ਵਰਤਿਆ_On_ਇੰਦਰਾਜ ] ), &( pxTCB->x_State_List_ਵਸਤੂ ) ) != pdFALSE )
                        {
                            /* The task is currently in its ready list - remove before
                             * adding it to its new ready list.  As we are in a critical
                             * section we can do this even if the scheduler is suspended. */
                            ਜੇ( uxListRemove( &( pxTCB->x_State_List_ਵਸਤੂ ) ) == ( UBaseType_t ) 0 )
                            {
                                /* It is known that the task is in its ready list so
                                 * there is no need to check again and the port level
                                 * reset macro can be called directly. */
                                ਪੋਰਟ_RESET_READY_ਤਰਜੀਹ( ux_ਤਰਜੀਹ_ਵਰਤਿਆ_On_ਇੰਦਰਾਜ, ux_ਸਿਖਰ_Ready_ਤਰਜੀਹ );
                            }

                            prv_ਜੋੜੋ_Task_ਮੰਜ਼ਿਲ_Ready_List( pxTCB );
                        }

                        ਜੇ( xYieldRequired != pdFALSE )
                        {
                            #ਜੇ CONFIG_FREERTOS_SMP
                                taskYIELD_TASK_CORE_IF_USING_PREEMPTION( pxTCB );
                            #ਨਹੀਂਤਾਂ
                                taskYIELD_IF_USING_PREEMPTION();
                            #ਜੇ_ਅੰਤ
                        }
                    }
                }
            }
            #ਨਹੀਂਤਾਂ /* if ( configUSE_MUTEXES == 1 ) */
            {
                v_Task_ਤਰਜੀਹ_ਸੈੱਟ( ਨਿਰਾ, px_Saved_ਤਰਜੀਹ->ux_ਤਰਜੀਹ );
            }
            #ਜੇ_ਅੰਤ /* if ( configUSE_MUTEXES == 1 ) */
        }
        #ਜੇ CONFIG_FREERTOS_SMP
            taskEXIT_CRITICAL();
        #ਨਹੀਂਤਾਂ
            taskEXIT_CRITICAL( &x_Kernel_ਤਾਲਾ );
        #ਜੇ_ਅੰਤ
    }

#ਜੇ_ਅੰਤ /* ( INCLUDE_vTaskPrioritySet == 1 ) */
/*----------------------------------------------------------*/

/* --------------------------------------------- TLSP Deletion Callbacks -------------------------------------------- */

#ਜੇ CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS

    ਖਾਲੀ v_Task_ਸੈੱਟ_Thread_Local_Storage_ਸੂਚਕ_And_Del_Callback( TaskHandle_t x_Task_ਮੰਜ਼ਿਲ_ਸੈੱਟ,
                                                          ਅਧਾਰ_Type_t x_ਸੂਚਕਾਂਕ,
                                                          ਖਾਲੀ * pv_ਮੁੱਲ,
                                                          TlsDeleteCallbackFunction_t pvDelCallback )
    {
        /* If TLSP deletion callbacks are enabled, then configNUM_THREAD_LOCAL_STORAGE_POINTERS
         * is doubled in size so that the latter half of the pvThreadLocalStoragePointers
         * stores the deletion callbacks. */
        ਜੇ( x_ਸੂਚਕਾਂਕ < ( config_ਅੰਕ_THREAD_LOCAL_STORAGE_ਸੂਚਕ_ਸਾਰੇ / 2 ) )
        {
            TCB_t * pxTCB;

            #ਜੇ ( config_ਅੰਕ_CORES > 1 )
            {
                /* For SMP, we need a critical section as another core could also
                 * update this task's TLSP at the same time. */
                #ਜੇ CONFIG_FREERTOS_SMP
                {
                    taskENTER_CRITICAL();
                }
                #ਨਹੀਂਤਾਂ /* CONFIG_FREERTOS_SMP */
                {
                    taskENTER_CRITICAL( &x_Kernel_ਤਾਲਾ );
                }
                #ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_SMP */
            }
            #ਜੇ_ਅੰਤ /* configNUM_CORES > 1 */

            pxTCB = prvGetTCBFromHandle( x_Task_ਮੰਜ਼ਿਲ_ਸੈੱਟ );
            /* Store the TLSP by indexing the first half of the array */
            pxTCB->pv_Thread_Local_Storage_ਸੂਚਕ_ਸਾਰੇ[ x_ਸੂਚਕਾਂਕ ] = pv_ਮੁੱਲ;

            /* Store the TLSP deletion callback by indexing the second half
             * of the array. */
            pxTCB->pv_Thread_Local_Storage_ਸੂਚਕ_ਸਾਰੇ[ ( x_ਸੂਚਕਾਂਕ + ( config_ਅੰਕ_THREAD_LOCAL_STORAGE_ਸੂਚਕ_ਸਾਰੇ / 2 ) ) ] = ( ਖਾਲੀ * ) pvDelCallback;

            #ਜੇ ( config_ਅੰਕ_CORES > 1 )
            {
                #ਜੇ CONFIG_FREERTOS_SMP
                {
                    taskEXIT_CRITICAL();
                }
                #ਨਹੀਂਤਾਂ /* CONFIG_FREERTOS_SMP */
                {
                    taskEXIT_CRITICAL( &x_Kernel_ਤਾਲਾ );
                }
                #ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_SMP */
            }
            #ਜੇ_ਅੰਤ /* configNUM_CORES > 1 */
        }
    }

#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */
/*----------------------------------------------------------*/

/* ----------------------------------------------------- Newlib ----------------------------------------------------- */

#ਜੇ ( configUSE_NEWLIB_REENTRANT == 1 )

/**
 * @brief Get reentrancy structure of the current task
 *
 * - This function is required by newlib (when __DYNAMIC_REENT__ is enabled)
 * - It will return a pointer to the current task's reent struct
 * - If FreeRTOS is not running, it will return the global reent struct
 *
 * @return Pointer to a the (current taks's)/(global) reent struct
 */
    ਬਣਤਰ _reent * __getreent( ਖਾਲੀ )
    {
        /* No lock needed because if this changes, we won't be running anymore. */
        TCB_t * pxCurTask = ( TCB_t * ) xTaskGetCurrentTaskHandle();
        ਬਣਤਰ _reent * ret;

        ਜੇ( pxCurTask == ਨਿਰਾ )
        {
            /* No task running. Return global struct. */
            ret = _GLOBAL_REENT;
        }
        ਨਹੀਂਤਾਂ
        {
            /* We have a currently executing task. Return its reentrant struct. */
            ret = &pxCurTask->xTLSBlock;
        }

        ਵਾਪਸ ret;
    }

#ਜੇ_ਅੰਤ /* configUSE_NEWLIB_REENTRANT == 1 */

/* -------------------------------------------------- Task Snapshot ------------------------------------------------- */

/**
 * @brief List of all task lists in FreeRTOS
 *
 * @note There are currently differing number of task list between SMP FreeRTOS and ESP-IDF FreeRTOS
 */
ਸਥਿਰ List_t * non_ready_task_lists[] =
{
    #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CONFIG_FREERTOS_SMP
        &xPendingReadyList,
    #ਨਹੀਂਤਾਂ /* CONFIG_FREERTOS_SMP */
        &xPendingReadyList[ 0 ],
        #ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CONFIG_FREERTOS_UNICORE
            &xPendingReadyList[ 1 ],
        #ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_UNICORE */
    #ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_SMP */
    &xDelayedTaskList1,
    &xDelayedTaskList2,
    #ਜੇ ( INCLUDE_vTaskDelete == 1 )
        &x_Tasks_ਉਡੀਕ_Termination,
    #ਜੇ_ਅੰਤ
    #ਜੇ ( INCLUDE_vTaskSuspend == 1 )
        &xSuspendedTaskList,
    #ਜੇ_ਅੰਤ
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
ਸਥਿਰ List_t * px_Get_Task_List_By_ਸੂਚਕਾਂਕ( UBaseType_t ux_List_ਸੂਚਕਾਂਕ )
{
    List_t * pxTaskList;
    ਅਚਲ size_t xNonReadyTaskListsCnt = ( ਆਕਾਰ( non_ready_task_lists ) / ਆਕਾਰ( List_t * ) );

    ਜੇ( ux_List_ਸੂਚਕਾਂਕ < configMAX_PRIORITIES )
    {
        pxTaskList = &pxReadyTasksLists[ configMAX_PRIORITIES - 1 - ux_List_ਸੂਚਕਾਂਕ ];
    }
    ਨਹੀਂਤਾਂ ਜੇ( ux_List_ਸੂਚਕਾਂਕ < configMAX_PRIORITIES + xNonReadyTaskListsCnt )
    {
        pxTaskList = non_ready_task_lists[ ux_List_ਸੂਚਕਾਂਕ - configMAX_PRIORITIES ];
    }
    ਨਹੀਂਤਾਂ
    {
        pxTaskList = ਨਿਰਾ;
    }

    /* sanity check */
    ਜੇ( pxTaskList )
    {
        ਜੇ( !ਪੋਰਟ_VALID_LIST_MEM( pxTaskList ) )
        {
            pxTaskList = ਨਿਰਾ;
        }
    }

    ਵਾਪਸ pxTaskList;
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
ਸਥਿਰ ਸਤਰਅੰਦਰ UBaseType_t px_Get_Task_List_ਗਿਣਤੀ( ਖਾਲੀ )
{
    ਵਾਪਸ configMAX_PRIORITIES + ( ਆਕਾਰ( non_ready_task_lists ) / ਆਕਾਰ( List_t * ) );
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
ਪੂਰਨ x_Task_Get_ਅਗਲਾ( TaskIterator_t * xIterator )
{
    ਜੇ( !xIterator )
    {
        ਵਾਪਸ -1;
    }

    List_ਵਸਤੂ_t * px_ਅਗਲਾ_List_ਵਸਤੂ = xIterator->px_ਅਗਲਾ_List_ਵਸਤੂ;
    UBaseType_t uxCurListIdx = xIterator->ux_Current_List_ਸੂਚਕਾਂਕ;
    UBaseType_t uxMaxListIdx = px_Get_Task_List_ਗਿਣਤੀ();

    ਲਈ( ; uxCurListIdx < uxMaxListIdx; ++uxCurListIdx )
    {
        List_t * pxCurrentTaskList = px_Get_Task_List_By_ਸੂਚਕਾਂਕ( uxCurListIdx );

        ਜੇ( !pxCurrentTaskList || ( list_CURRENT_LIST_ਲੰਮਾਈ( pxCurrentTaskList ) == 0 ) )
        {
            ਜਾਰੀ;
        }

        ਅਚਲ List_ਵਸਤੂ_t * px_Curr_List_ਵਸਤੂ = list_GET_ਸਿਰਾ_MARKER( pxCurrentTaskList );

        ਜੇ( !px_ਅਗਲਾ_List_ਵਸਤੂ )
        {
            /* We are here if the traversal starts from the beginning or when we finish traversing
             * for one of the state lists
             */
            px_ਅਗਲਾ_List_ਵਸਤੂ = list_GET_ਅਗਲਾ( px_Curr_List_ਵਸਤੂ );
        }

        ਜੇ( !ਪੋਰਟ_VALID_LIST_MEM( px_ਅਗਲਾ_List_ਵਸਤੂ ) )
        {
            /* Nothing to do with the corrupted list item. We will skip to the next task state list.
             * pxNextListItem should be NULL at the beginning of each task list.
             */
            px_ਅਗਲਾ_List_ਵਸਤੂ = ਨਿਰਾ;
            ਜਾਰੀ;
        }

        TCB_t * pxTCB = ( TCB_t * ) list_GET_LIST_ਵਸਤੂ_OWNER( px_ਅਗਲਾ_List_ਵਸਤੂ );

        ਜੇ( !ਪੋਰਟ_VALID_TCB_MEM( pxTCB ) )
        {
            pxTCB = ਨਿਰਾ;
        }

        xIterator->pxTaskHandle = pxTCB;
        xIterator->ux_Current_List_ਸੂਚਕਾਂਕ = uxCurListIdx;

        ਜੇ( px_Curr_List_ਵਸਤੂ->px_ਪਿਛਲਾ == px_ਅਗਲਾ_List_ਵਸਤੂ )
        {
            /* If this is the last item of the current state list */
            xIterator->ux_Current_List_ਸੂਚਕਾਂਕ++;
            xIterator->px_ਅਗਲਾ_List_ਵਸਤੂ = ਨਿਰਾ;
        }
        ਨਹੀਂਤਾਂ
        {
            xIterator->px_ਅਗਲਾ_List_ਵਸਤੂ = list_GET_ਅਗਲਾ( px_ਅਗਲਾ_List_ਵਸਤੂ );
        }

        ਵਾਪਸ uxCurListIdx;
    }

    ਵਾਪਸ -1; /* end of the task list */
}
/*----------------------------------------------------------*/

ਅਧਾਰ_Type_t vTaskGetSnapshot( TaskHandle_t pxTask,
                             TaskSnapshot_t * pxTaskSnapshot )
{
    ESP_STATIC_ANALYZER_CHECK(!pxTask, pdFALSE);

    ਜੇ( ( ਪੋਰਟ_VALID_TCB_MEM( pxTask ) == false ) || ( pxTaskSnapshot == ਨਿਰਾ ) )
    {
        ਵਾਪਸ pdFALSE;
    }

    TCB_t * pxTCB = ( TCB_t * ) pxTask;
    pxTaskSnapshot->pxTCB = pxTCB;
    pxTaskSnapshot->px_ਸਿਖਰ_ਦਾ_ਢੇਰ = ( ਢੇਰ_Type_t * ) pxTCB->px_ਸਿਖਰ_ਦਾ_ਢੇਰ;
    pxTaskSnapshot->px_ਸਿਰਾ_ਦਾ_ਢੇਰ = ( ਢੇਰ_Type_t * ) pxTCB->px_ਸਿਰਾ_ਦਾ_ਢੇਰ;
    ਵਾਪਸ pdTRUE;
}
/*----------------------------------------------------------*/

UBaseType_t uxTaskGetSnapshotAll( TaskSnapshot_t * ਅਚਲ pxTaskSnapshotArray,
                                  ਅਚਲ UBaseType_t ux_Array_ਲੰਮਾਈ,
                                  UBaseType_t * ਅਚਲ pxTCBSize )
{
    UBaseType_t ux_Array_ਅੰਕ_Filled = 0;

    /* Traverse all of the tasks lists */
    TaskIterator_t xTaskIter = { 0 }; /* Point to the first task list */

    ਜਦੋਂ( x_Task_Get_ਅਗਲਾ( &xTaskIter ) != -1 && ux_Array_ਅੰਕ_Filled < ux_Array_ਲੰਮਾਈ )
    {
        vTaskGetSnapshot( xTaskIter.pxTaskHandle, &pxTaskSnapshotArray[ ux_Array_ਅੰਕ_Filled ] );
        ux_Array_ਅੰਕ_Filled++;
    }

    ਜੇ( pxTCBSize != ਨਿਰਾ )
    {
        *pxTCBSize = ਆਕਾਰ( TCB_t );
    }

    ਵਾਪਸ ux_Array_ਅੰਕ_Filled;
}
/*----------------------------------------------------------*/

/* ----------------------------------------------------- Misc ----------------------------------------------------- */

ਖਾਲੀ * pvTaskGetCurrentTCBForCore( ਅਧਾਰ_Type_t xCoreID )
{
    ਖਾਲੀ * pvRet;

    configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

    #ਜੇ CONFIG_FREERTOS_SMP
        /* SMP FreeRTOS defines pxCurrentTCB as a macro function call */
        pvRet = ( ਖਾਲੀ * ) pxCurrentTCB;
    #ਨਹੀਂਤਾਂ /* CONFIG_FREERTOS_SMP */
        pvRet = ( ਖਾਲੀ * ) pxCurrentTCBs[ xCoreID ];
    #ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_SMP */
    ਵਾਪਸ pvRet;
}

/* ----------------------------------------------------- OpenOCD ---------------------------------------------------- */

#ਜੇ CONFIG_FREERTOS_DEBUG_OCDAWARE

/**
 * Debug param indexes. DO NOT change the order. OpenOCD uses the same indexes
 * Entries in FreeRTOS_openocd_params must match the order of these indexes
 */
    ਗਿਣਤੀ
    {
        ESP_FREERTOS_DEBUG_ਸਾਰਣੀ_ਮਾਪ = 0,
        ESP_FREERTOS_DEBUG_ਸਾਰਣੀ_ਸੰਸਕਰਣ,
        ESP_FREERTOS_DEBUG_KERNEL_VER_MAJOR,
        ESP_FREERTOS_DEBUG_KERNEL_VER_MINOR,
        ESP_FREERTOS_DEBUG_KERNEL_VER_BUILD,
        ESP_FREERTOS_DEBUG_UX_ਸਿਖਰ_ਵਰਤਿਆ_PIORITY,
        ESP_FREERTOS_DEBUG_PX_ਸਿਖਰ_ਦਾ_ਢੇਰ,
        ESP_FREERTOS_DEBUG_PC_TASK_NAME,
        ESP_FREERTOS_DEBUG_LIST_ਮਾਪ,
        ESP_FREERTOS_DEBUG_LIST_ਅੰਕ_ਚੀਜ਼ਾਂ,
        ESP_FREERTOS_DEBUG_LIST_ਸਿਰਾ,
        ESP_FREERTOS_DEBUG_LIST_ਸਿਰਾ_PREV,
        ESP_FREERTOS_DEBUG_LIST_ਵਸਤੂ_PREV,
        ESP_FREERTOS_DEBUG_LIST_ਵਸਤੂ_OWNER,
        ESP_FREERTOS_DEBUG_TASK_ਗਿਣਤੀ_ਚੌੜਾਈ,
        ESP_FREERTOS_DEBUG_PTR_ਚੌੜਾਈ,
        /* New entries must be inserted here */
        ESP_FREERTOS_DEBUG_ਸਾਰਣੀ_ਸਿਰਾ,
    };

    ਅਚਲ DRAM_ATTR uint8_t FreeRTOS_openocd_params[ ESP_FREERTOS_DEBUG_ਸਾਰਣੀ_ਸਿਰਾ ] =
    {
        ESP_FREERTOS_DEBUG_ਸਾਰਣੀ_ਸਿਰਾ, /* table size */
        1,                            /* table version */
        tsk_KERNEL_ਸੰਸਕਰਣ_MAJOR,
        tsk_KERNEL_ਸੰਸਕਰਣ_MINOR,
        tsk_KERNEL_ਸੰਸਕਰਣ_BUILD,
        configMAX_PRIORITIES - 1,                   /* uxTopUsedPriority */
        offsetof( TCB_t, px_ਸਿਖਰ_ਦਾ_ਢੇਰ ),            /* thread_stack_offset; */
        offsetof( TCB_t, pcTaskName ),              /* thread_name_offset; */
        ਆਕਾਰ( List_t ),                           /* list_width */
        offsetof( List_t, ux_ਸੰਖਿਆ_ਦਾ_ਚੀਜ਼ਾਂ ),        /* list_item_num */
        offsetof( List_t, x_List_ਸਿਰਾ ),               /* list_end_offset */
        offsetof( List_t, x_List_ਸਿਰਾ.px_ਪਿਛਲਾ ),    /* list_next_offset */
        offsetof( List_ਵਸਤੂ_t, px_ਪਿਛਲਾ ),         /* list_elem_next_offset */
        offsetof( List_ਵਸਤੂ_t, pvOwner ),            /* list_elem_content_offset */
        ਆਕਾਰ( UBaseType_t ),                      /* task_count_width */
        ਆਕਾਰ( ਖਾਲੀ * )                            /* ptr_width */
    };

#ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_DEBUG_OCDAWARE */
/*----------------------------------------------------------*/

/* ----------------------------------------------------- PSRAM ---------------------------------------------------- */

#ਜੇ CONFIG_SPIRAM

    #ਜੇ CONFIG_FREERTOS_SMP
        ਅਧਾਰ_Type_t prv_Task_Create_Dynamic_Affinity_ਸੈੱਟ_With_Caps( TaskFunction_t px_Task_ਕੋਡ,
                                                            ਅਚਲ ਅੱਖਰ * ਅਚਲ pcName,
                                                            ਅਚਲ config_ਢੇਰ_ਡੂੰਘਾਈ_TYPE us_ਢੇਰ_ਡੂੰਘਾਈ,
                                                            ਖਾਲੀ * ਅਚਲ pvParameters,
                                                            UBaseType_t ux_ਤਰਜੀਹ,
                                                            UBaseType_t ux_Core_Affinity_ਨਕਾਬ,
                                                            UBaseType_t ux_ਢੇਰ_ਸਮ੍ਰਿਤੀ_Caps,
                                                            TaskHandle_t * ਅਚਲ pxCreatedTask )
    #ਨਹੀਂਤਾਂ /* CONFIG_FREERTOS_SMP */
        ਅਧਾਰ_Type_t prv_Task_Create_Dynamic_Pinned_ਮੰਜ਼ਿਲ_Core_With_Caps( TaskFunction_t px_Task_ਕੋਡ,
                                                             ਅਚਲ ਅੱਖਰ * ਅਚਲ pcName,
                                                             ਅਚਲ config_ਢੇਰ_ਡੂੰਘਾਈ_TYPE us_ਢੇਰ_ਡੂੰਘਾਈ,
                                                             ਖਾਲੀ * ਅਚਲ pvParameters,
                                                             UBaseType_t ux_ਤਰਜੀਹ,
                                                             ਅਚਲ ਅਧਾਰ_Type_t xCoreID,
                                                             UBaseType_t ux_ਢੇਰ_ਸਮ੍ਰਿਤੀ_Caps,
                                                             TaskHandle_t * ਅਚਲ pxCreatedTask )
    #ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_SMP */
    {
        TCB_t * pxNewTCB;
        ਅਧਾਰ_Type_t xReturn;

        ਢੇਰ_Type_t * px_ਢੇਰ;

        configASSERT( ux_ਢੇਰ_ਸਮ੍ਰਿਤੀ_Caps & ( MALLOC_CAP_8_ਦੋਆਧਾਰੀ_ਅੰਕ ) );
        configASSERT( ( ux_ਢੇਰ_ਸਮ੍ਰਿਤੀ_Caps & MALLOC_CAP_SPIRAM ) ||
                      ( ux_ਢੇਰ_ਸਮ੍ਰਿਤੀ_Caps & MALLOC_CAP_ਅੰਦਰੂਨੀ ) );
        #ਜੇ ( !CONFIG_FREERTOS_SMP )
        {
            configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );
        }
        #ਜੇ_ਅੰਤ /* !CONFIG_FREERTOS_SMP */

        /* Allocate space for the stack used by the task being created. */
        px_ਢੇਰ = ਢੇਰ_caps_malloc( ( ( ( size_t ) us_ਢੇਰ_ਡੂੰਘਾਈ ) * ਆਕਾਰ( ਢੇਰ_Type_t ) ), ux_ਢੇਰ_ਸਮ੍ਰਿਤੀ_Caps );

        ਜੇ( px_ਢੇਰ != ਨਿਰਾ )
        {
            /* Allocate space for the TCB. */
            pxNewTCB = ( TCB_t * ) pv_ਪੋਰਟ_Malloc( ਆਕਾਰ( TCB_t ) );

            ਜੇ( pxNewTCB != ਨਿਰਾ )
            {
                memset( ( ਖਾਲੀ * ) pxNewTCB, 0x00, ਆਕਾਰ( TCB_t ) );

                /* Store the stack location in the TCB. */
                pxNewTCB->px_ਢੇਰ = px_ਢੇਰ;
            }
            ਨਹੀਂਤਾਂ
            {
                /* The stack cannot be used as the TCB has not been created. Free it. */
                ਢੇਰ_caps_free( px_ਢੇਰ );
            }
        }
        ਨਹੀਂਤਾਂ
        {
            pxNewTCB = ਨਿਰਾ;
        }

        ਜੇ( pxNewTCB != ਨਿਰਾ )
        {
            #ਜੇ ( tsk_STATIC_AND_DYNAMIC_ਵੰਡ_POSSIBLE != 0 )
            {
                /* Tasks can be created statically or dynamically, so note this
                 * task was created dynamically in case it is later deleted. */
                pxNewTCB->ucStaticallyAllocated = tsk_DYNAMICALLY_ALLOCATED_ਢੇਰ_AND_TCB;
            }
            #ਜੇ_ਅੰਤ /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

            #ਜੇ CONFIG_FREERTOS_SMP
            {
                prvInitialiseNewTask( px_Task_ਕੋਡ, pcName, ( uint32_t ) us_ਢੇਰ_ਡੂੰਘਾਈ, pvParameters, ux_ਤਰਜੀਹ, pxCreatedTask, pxNewTCB, ਨਿਰਾ );
                #ਜੇ ( ( config_ਅੰਕ_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
                {
                    /* Set the task's affinity before scheduling it */
                    pxNewTCB->ux_Core_Affinity_ਨਕਾਬ = ux_Core_Affinity_ਨਕਾਬ;
                }
                #ਜੇ_ਅੰਤ /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            }
            #ਨਹੀਂਤਾਂ /* CONFIG_FREERTOS_SMP */
            {
                prvInitialiseNewTask( px_Task_ਕੋਡ, pcName, ( uint32_t ) us_ਢੇਰ_ਡੂੰਘਾਈ, pvParameters, ux_ਤਰਜੀਹ, pxCreatedTask, pxNewTCB, ਨਿਰਾ, xCoreID );
            }
            #ਜੇ_ਅੰਤ /* CONFIG_FREERTOS_SMP */

            prv_ਜੋੜੋ_New_Task_ਮੰਜ਼ਿਲ_Ready_List( pxNewTCB );
            xReturn = pdPASS;
        }
        ਨਹੀਂਤਾਂ
        {
            xReturn = err_COULD_NOT_ALLOCATE_REQUIRED_ਸਮ੍ਰਿਤੀ;
        }

        ਵਾਪਸ xReturn;
    }

#ਜੇ_ਅੰਤ /* CONFIG_SPIRAM */
/*----------------------------------------------------------*/
