/*
 * SPDX-FileCopyrightText: 2015-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ "esp_assert.h"
#అన్తర్భావ "esp_heap_caps.h"
#అన్తర్భావ "esp_compiler.h"
#అన్తర్భావ "freertos/idf_additions.h"
#అన్తర్భావ "freertos/freertos_debug.h"
#అన్తర్భావ "esp_private/freertos_idf_additions_priv.h"

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
_Static_assert( offsetof( StaticTask_t, pxDummy6 ) == offsetof( TCB_t, px_దొంతర ) );
_Static_assert( offsetof( StaticTask_t, pxDummy8 ) == offsetof( TCB_t, px_చివర_యొక్క_దొంతర ) );
#ఒకవేళ !CONFIG_IDF_లక్ష్యం_LINUX    // Disabled for linux builds due to differences in types
_Static_assert( tskNO_AFFINITY == ( పునాది_Type_t ) CONFIG_FREERTOS_NO_AFFINITY, "CONFIG_FREERTOS_NO_AFFINITY must be the same as tskNO_AFFINITY" );
#అయితే_ముగింపు

/* ------------------------------------------------- Kernel Control ------------------------------------------------- */

#ఒకవేళ ( !CONFIG_FREERTOS_SMP && ( config_అంకె_CORES > 1 ) )

/*
 * Wrapper function to take "xKerneLock"
 */
    శూన్య prv_Take_Kernel_తాళం( శూన్య )
    {
        /* We call the tasks.c critical section macro to take xKernelLock */
        taskENTER_CRITICAL( &x_Kernel_తాళం );
    }

#అయితే_ముగింపు /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#ఒకవేళ ( !CONFIG_FREERTOS_SMP && ( config_అంకె_CORES > 1 ) )

/*
 * Wrapper function to release "xKerneLock"
 */
    శూన్య prv_Release_Kernel_తాళం( శూన్య )
    {
        /* We call the tasks.c critical section macro to release xKernelLock */
        taskEXIT_CRITICAL( &x_Kernel_తాళం );
    }

#అయితే_ముగింపు /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#ఒకవేళ ( CONFIG_FREERTOS_SMP && ( config_అంకె_CORES > 1 ) )

/*
 * Workaround for non-thread safe multi-core OS startup (see IDF-4524)
 */
    శూన్య prv_ప్రారంభం_Scheduler_Other_Cores( శూన్య )
    {
        /* This function is always called with interrupts disabled*/
        xSchedulerRunning = pdTRUE;
    }

#అయితే_ముగింపు /* ( CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#ఒకవేళ ( !CONFIG_FREERTOS_SMP && ( config_అంకె_CORES > 1 ) )

    పునాది_Type_t xTaskIncrementTickOtherCores( శూన్య )
    {
        /* Minor optimization. This function can never switch cores mid
         * execution */
        పునాది_Type_t xCoreID = పోర్టు_GET_CORE_ID();
        పునాది_Type_t xSwitchRequired = pdFALSE;

        /* This function should never be called by Core 0. */
        configASSERT( xCoreID != 0 );

        /* Called by the portable layer each time a tick interrupt occurs
         * on a core other than core 0. */
        traceTASK_INCREMENT_TICK( x_Tick_లెక్క );

        ఒకవేళ( uxSchedulerSuspended[ xCoreID ] == ( UBaseType_t ) 0U )
        {
            /* We need take the kernel lock here as we are about to access
             * kernel data structures. */
            taskENTER_CRITICAL_ISR( &x_Kernel_తాళం );

            /* Tasks of equal priority to the currently running task will share
             * processing time (time slice) if preemption is on, and the application
             * writer has not explicitly turned time slicing off. */
            #ఒకవేళ ( ( configUSE_PREEMPTION == 1 ) && ( config_USE_సమయం_SLICING == 1 ) )
            {
                ఒకవేళ( list_CURRENT_LIST_నిడివి( &( pxReadyTasksLists[ pxCurrentTCBs[ xCoreID ]->ux_ప్రాధాన్యత ] ) ) > ( UBaseType_t ) 1 )
                {
                    xSwitchRequired = pdTRUE;
                }
                లేకపోతే
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            #అయితే_ముగింపు /* ( ( configUSE_PREEMPTION == 1 ) && ( configUSE_TIME_SLICING == 1 ) ) */

            /* Release the previously taken kernel lock as we have finished
             * accessing the kernel data structures. */
            taskEXIT_CRITICAL_ISR( &x_Kernel_తాళం );

            #ఒకవేళ ( configUSE_PREEMPTION == 1 )
            {
                ఒకవేళ( xYieldPending[ xCoreID ] != pdFALSE )
                {
                    xSwitchRequired = pdTRUE;
                }
                లేకపోతే
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            #అయితే_ముగింపు /* configUSE_PREEMPTION */
        }

        #ఒకవేళ ( configUSE_TICK_HOOK == 1 )
        {
            vApplicationTickHook();
        }
        #అయితే_ముగింపు

        తిరిగి xSwitchRequired;
    }

#అయితే_ముగింపు /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

/* -------------------------------------------------- Task Creation ------------------------------------------------- */

#ఒకవేళ ( config_SUPPORT_DYNAMIC_కేటాయింపు == 1 )

    పునాది_Type_t x_Task_Create_Pinned_గమ్యం_Core( TaskFunction_t px_Task_సంకేతం,
                                        స్థిరరాశి అక్షరం * స్థిరరాశి pcName,
                                        స్థిరరాశి uint32_t us_దొంతర_లోతు,
                                        శూన్య * స్థిరరాశి pvParameters,
                                        UBaseType_t ux_ప్రాధాన్యత,
                                        TaskHandle_t * స్థిరరాశి pxCreatedTask,
                                        స్థిరరాశి పునాది_Type_t xCoreID )
    {
        పునాది_Type_t xReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );

        #ఒకవేళ CONFIG_FREERTOS_SMP
        {
            /* If using Amazon SMP FreeRTOS. This function is just a wrapper around
             * xTaskCreate() or xTaskCreateAffinitySet(). */
            #ఒకవేళ ( ( config_అంకె_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
            {
                /* Convert xCoreID into an affinity mask */
                UBaseType_t ux_Core_Affinity_ముసుగు;

                /* Bit shifting << xCoreID is only valid if we have less than
                 * 32 cores. */
                ESP_STATIC_ASSERT( config_అంకె_CORES < 32 );

                ఒకవేళ( xCoreID == tskNO_AFFINITY )
                {
                    ux_Core_Affinity_ముసుగు = tskNO_AFFINITY;
                }
                లేకపోతే
                {
                    ux_Core_Affinity_ముసుగు = ( 1 << xCoreID );
                }

                xReturn = x_Task_Create_Affinity_సమితి( px_Task_సంకేతం, pcName, us_దొంతర_లోతు, pvParameters, ux_ప్రాధాన్యత, ux_Core_Affinity_ముసుగు, pxCreatedTask );
            }
            #లేకపోతే /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            {
                xReturn = xTaskCreate( px_Task_సంకేతం, pcName, us_దొంతర_లోతు, pvParameters, ux_ప్రాధాన్యత, pxCreatedTask );
            }
            #అయితే_ముగింపు /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
        }
        #లేకపోతే /* CONFIG_FREERTOS_SMP */
        {
            TCB_t * pxNewTCB;

            /* If the stack grows down then allocate the stack then the TCB so the
             * stack does not grow into the TCB.  Likewise if the stack grows up
             * then allocate the TCB then the stack. */
            #ఒకవేళ ( పోర్టు_దొంతర_GROWTH > 0 )
            {
                /* Allocate space for the TCB.  Where the memory comes from depends on
                 * the implementation of the port malloc function and whether or not static
                 * allocation is being used. */
                pxNewTCB = ( TCB_t * ) pv_పోర్టు_Malloc( పరిమాణం( TCB_t ) );

                ఒకవేళ( pxNewTCB != ఖాళీ )
                {
                    memset( ( శూన్య * ) pxNewTCB, 0x00, పరిమాణం( TCB_t ) );

                    /* Allocate space for the stack used by the task being created.
                     * The base of the stack memory stored in the TCB so the task can
                     * be deleted later if required. */
                    pxNewTCB->px_దొంతర = ( దొంతర_Type_t * ) pv_పోర్టు_Malloc_దొంతర( ( ( ( size_t ) us_దొంతర_లోతు ) * పరిమాణం( దొంతర_Type_t ) ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */

                    ఒకవేళ( pxNewTCB->px_దొంతర == ఖాళీ )
                    {
                        /* Could not allocate the stack.  Delete the allocated TCB. */
                        v_పోర్టు_Free( pxNewTCB );
                        pxNewTCB = ఖాళీ;
                    }
                }
            }
            #లేకపోతే /* portSTACK_GROWTH */
            {
                దొంతర_Type_t * px_దొంతర;

                /* Allocate space for the stack used by the task being created. */
                px_దొంతర = pv_పోర్టు_Malloc_దొంతర( ( ( ( size_t ) us_దొంతర_లోతు ) * పరిమాణం( దొంతర_Type_t ) ) ); /*lint !e9079 All values returned by pvPortMalloc() have at least the alignment required by the MCU's stack and this allocation is the stack. */

                ఒకవేళ( px_దొంతర != ఖాళీ )
                {
                    /* Allocate space for the TCB. */
                    pxNewTCB = ( TCB_t * ) pv_పోర్టు_Malloc( పరిమాణం( TCB_t ) ); /*lint !e9087 !e9079 All values returned by pvPortMalloc() have at least the alignment required by the MCU's stack, and the first member of TCB_t is always a pointer to the task's stack. */

                    ఒకవేళ( pxNewTCB != ఖాళీ )
                    {
                        memset( ( శూన్య * ) pxNewTCB, 0x00, పరిమాణం( TCB_t ) );

                        /* Store the stack location in the TCB. */
                        pxNewTCB->px_దొంతర = px_దొంతర;
                    }
                    లేకపోతే
                    {
                        /* The stack cannot be used as the TCB was not created.  Free
                         * it again. */
                        v_పోర్టు_Free_దొంతర( px_దొంతర );
                    }
                }
                లేకపోతే
                {
                    pxNewTCB = ఖాళీ;
                }
            }
            #అయితే_ముగింపు /* portSTACK_GROWTH */

            ఒకవేళ( pxNewTCB != ఖాళీ )
            {
                #ఒకవేళ ( tsk_STATIC_AND_DYNAMIC_కేటాయింపు_POSSIBLE != 0 ) /*lint !e9029 !e731 Macro has been consolidated for readability reasons. */
                {
                    /* Tasks can be created statically or dynamically, so note this
                     * task was created dynamically in case it is later deleted. */
                    pxNewTCB->ucStaticallyAllocated = tsk_DYNAMICALLY_ALLOCATED_దొంతర_AND_TCB;
                }
                #అయితే_ముగింపు /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

                prvInitialiseNewTask( px_Task_సంకేతం, pcName, ( uint32_t ) us_దొంతర_లోతు, pvParameters, ux_ప్రాధాన్యత, pxCreatedTask, pxNewTCB, ఖాళీ, xCoreID );
                prv_కూడు_New_Task_గమ్యం_Ready_List( pxNewTCB );
                xReturn = pdPASS;
            }
            లేకపోతే
            {
                xReturn = err_COULD_NOT_ALLOCATE_REQUIRED_జ్ఞాపకం;
            }
        }
        #అయితే_ముగింపు /* CONFIG_FREERTOS_SMP */

        తిరిగి xReturn;
    }

#అయితే_ముగింపు /* ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )

    TaskHandle_t x_Task_Create_Static_Pinned_గమ్యం_Core( TaskFunction_t px_Task_సంకేతం,
                                                స్థిరరాశి అక్షరం * స్థిరరాశి pcName,
                                                స్థిరరాశి uint32_t ul_దొంతర_లోతు,
                                                శూన్య * స్థిరరాశి pvParameters,
                                                UBaseType_t ux_ప్రాధాన్యత,
                                                దొంతర_Type_t * స్థిరరాశి pux_దొంతర_బఫరు,
                                                StaticTask_t * స్థిరరాశి px_Task_బఫరు,
                                                స్థిరరాశి పునాది_Type_t xCoreID )
    {
        TaskHandle_t xReturn;

        configASSERT( పోర్టు_VALID_దొంతర_MEM( pux_దొంతర_బఫరు ) );
        configASSERT( పోర్టు_VALID_TCB_MEM( px_Task_బఫరు ) );
        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );

        #ఒకవేళ CONFIG_FREERTOS_SMP
        {
            /* If using Amazon SMP FreeRTOS. This function is just a wrapper around
             * xTaskCreateStatic() or xTaskCreateStaticAffinitySet(). */
            #ఒకవేళ ( ( config_అంకె_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
            {
                /* Convert xCoreID into an affinity mask */
                UBaseType_t ux_Core_Affinity_ముసుగు;

                /* Bit shifting << xCoreID is only valid if we have less than
                 * 32 cores. */
                ESP_STATIC_ASSERT( config_అంకె_CORES < 32 );

                ఒకవేళ( xCoreID == tskNO_AFFINITY )
                {
                    ux_Core_Affinity_ముసుగు = tskNO_AFFINITY;
                }
                లేకపోతే
                {
                    ux_Core_Affinity_ముసుగు = ( 1 << xCoreID );
                }

                xReturn = x_Task_Create_Static_Affinity_సమితి( px_Task_సంకేతం, pcName, ul_దొంతర_లోతు, pvParameters, ux_ప్రాధాన్యత, pux_దొంతర_బఫరు, px_Task_బఫరు, ux_Core_Affinity_ముసుగు );
            }
            #లేకపోతే /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            {
                xReturn = xTaskCreateStatic( px_Task_సంకేతం, pcName, ul_దొంతర_లోతు, pvParameters, ux_ప్రాధాన్యత, pux_దొంతర_బఫరు, px_Task_బఫరు );
            }
            #అయితే_ముగింపు /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
        }
        #లేకపోతే /* CONFIG_FREERTOS_SMP */
        {
            TCB_t * pxNewTCB;

            #ఒకవేళ ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticTask_t equals the size of the real task
                 * structure. */
                అస్థిర size_t x_కొలత = పరిమాణం( StaticTask_t );
                configASSERT( x_కొలత == పరిమాణం( TCB_t ) );
                ( శూన్య ) x_కొలత; /* Prevent lint warning when configASSERT() is not used. */
            }
            #అయితే_ముగింపు /* configASSERT_DEFINED */

            ఒకవేళ( ( px_Task_బఫరు != ఖాళీ ) && ( pux_దొంతర_బఫరు != ఖాళీ ) )
            {
                /* The memory used for the task's TCB and stack are passed into this
                 * function - use them. */
                pxNewTCB = ( TCB_t * ) px_Task_బఫరు; /*lint !e740 !e9087 Unusual cast is ok as the structures are designed to have the same alignment, and the size is checked by an assert. */
                memset( ( శూన్య * ) pxNewTCB, 0x00, పరిమాణం( TCB_t ) );
                pxNewTCB->px_దొంతర = ( దొంతర_Type_t * ) pux_దొంతర_బఫరు;

                #ఒకవేళ ( tsk_STATIC_AND_DYNAMIC_కేటాయింపు_POSSIBLE != 0 ) /*lint !e731 !e9029 Macro has been consolidated for readability reasons. */
                {
                    /* Tasks can be created statically or dynamically, so note this
                     * task was created statically in case the task is later deleted. */
                    pxNewTCB->ucStaticallyAllocated = tsk_STATICALLY_ALLOCATED_దొంతర_AND_TCB;
                }
                #అయితే_ముగింపు /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

                prvInitialiseNewTask( px_Task_సంకేతం, pcName, ul_దొంతర_లోతు, pvParameters, ux_ప్రాధాన్యత, &xReturn, pxNewTCB, ఖాళీ, xCoreID );
                prv_కూడు_New_Task_గమ్యం_Ready_List( pxNewTCB );
            }
            లేకపోతే
            {
                xReturn = ఖాళీ;
            }
        }
        #అయితే_ముగింపు /* CONFIG_FREERTOS_SMP */

        తిరిగి xReturn;
    }

#అయితే_ముగింపు /* ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#ఒకవేళ ( configUSE_TIMERS == 1 )

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
    పునాది_Type_t __attribute__( ( weak ) ) xTimerCreateTimerTask( శూన్య )
    {
        తిరిగి pdPASS;
    }

#అయితే_ముగింపు /* configUSE_TIMERS */
/*----------------------------------------------------------*/

/* ------------------------------------------------- Task Utilities ------------------------------------------------- */

పునాది_Type_t xTaskGetCoreID( TaskHandle_t xTask )
{
    పునాది_Type_t xReturn;

    #ఒకవేళ ( config_అంకె_CORES > 1 )
    {
        #ఒకవేళ CONFIG_FREERTOS_SMP
            UBaseType_t ux_Core_Affinity_ముసుగు;

            /* Get the core affinity mask and convert it to an ID */
            ux_Core_Affinity_ముసుగు = vTaskCoreAffinityGet( xTask );

            /* If the task is not pinned to a particular core, treat it as tskNO_AFFINITY */
            ఒకవేళ( ux_Core_Affinity_ముసుగు & ( ux_Core_Affinity_ముసుగు - 1 ) ) /* If more than one bit set */
            {
                xReturn = tskNO_AFFINITY;
            }
            లేకపోతే
            {
                పూర్ణాంకం i_సూచీ_Plus_One = __builtin_ffs( ux_Core_Affinity_ముసుగు );
                ప్రతిజ్ఞా( i_సూచీ_Plus_One >= 1 );
                xReturn = i_సూచీ_Plus_One - 1;
            }
        #లేకపోతే /* CONFIG_FREERTOS_SMP */
            TCB_t * pxTCB;

            pxTCB = prvGetTCBFromHandle( xTask );

            xReturn = pxTCB->xCoreID;
        #అయితే_ముగింపు /* CONFIG_FREERTOS_SMP */
    }
    #లేకపోతే /* configNUM_CORES > 1 */
    {
        /* Single-core. Just return a core ID of 0 */
        xReturn = 0;
    }
    #అయితే_ముగింపు /* configNUM_CORES > 1 */

    తిరిగి xReturn;
}
/*----------------------------------------------------------*/

#ఒకవేళ ( ( !CONFIG_FREERTOS_SMP ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

    TaskHandle_t xTaskGetIdleTaskHandleForCore( పునాది_Type_t xCoreID )
    {
        /* If xTaskGetIdleTaskHandle() is called before the scheduler has been
         * started, then xIdleTaskHandle will be NULL. */
        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );
        configASSERT( ( xIdleTaskHandle[ xCoreID ] != ఖాళీ ) );
        తిరిగి xIdleTaskHandle[ xCoreID ];
    }

#అయితే_ముగింపు /* ( ( !CONFIG_FREERTOS_SMP ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*----------------------------------------------------------*/

#ఒకవేళ ( ( !CONFIG_FREERTOS_SMP ) && ( ( INCLUDE_xTaskGetCurrentTaskHandle == 1 ) || ( configUSE_MUTEXES == 1 ) ) )

    TaskHandle_t xTaskGetCurrentTaskHandleForCore( పునాది_Type_t xCoreID )
    {
        TaskHandle_t xReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

        #ఒకవేళ ( CONFIG_FREERTOS_SMP )
        {
            xReturn = xTaskGetCurrentTaskHandleCPU( ( UBaseType_t ) xCoreID );
        }
        #లేకపోతే /* CONFIG_FREERTOS_SMP */
        {
            /* A critical section is not required as this function does not
             * guarantee that the TCB will still be valid when this function
             * returns. */
            xReturn = pxCurrentTCBs[ xCoreID ];
        }
        #అయితే_ముగింపు /* CONFIG_FREERTOS_SMP */

        తిరిగి xReturn;
    }

#అయితే_ముగింపు /* ( ( !CONFIG_FREERTOS_SMP ) && ( ( INCLUDE_xTaskGetCurrentTaskHandle == 1 ) || ( configUSE_MUTEXES == 1 ) ) ) */
/*----------------------------------------------------------*/

#ఒకవేళ ( !CONFIG_FREERTOS_SMP && ( config_GENERATE_నిరంతరం_సమయం_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

    config_నిరంతరం_సమయం_COUNTER_TYPE ul_Task_Get_Idle_నిరంతరం_సమయం_Counter_For_Core( పునాది_Type_t xCoreID )
    {
        config_నిరంతరం_సమయం_COUNTER_TYPE ul_నిరంతరం_సమయం_Counter;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

        /* For SMP, we need to take the kernel lock here as we are about to
         * access kernel data structures. */
        prvENTER_CRITICAL_SMP_ONLY( &x_Kernel_తాళం );
        {
            ul_నిరంతరం_సమయం_Counter = xIdleTaskHandle[ xCoreID ]->ul_నిరంతరం_సమయం_Counter;
        }
        /* Release the previously taken kernel lock. */
        prvEXIT_CRITICAL_SMP_ONLY( &x_Kernel_తాళం );

        తిరిగి ul_నిరంతరం_సమయం_Counter;
    }

#అయితే_ముగింపు /* ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*----------------------------------------------------------*/

#ఒకవేళ ( !CONFIG_FREERTOS_SMP && ( config_GENERATE_నిరంతరం_సమయం_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

    config_నిరంతరం_సమయం_COUNTER_TYPE ul_Task_Get_Idle_నిరంతరం_సమయం_Percent_For_Core( పునాది_Type_t xCoreID )
    {
        config_నిరంతరం_సమయం_COUNTER_TYPE ul_మొత్తం_సమయం, ulReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

        #నిర్వచితమైతే పోర్టు_ALT_GET_నిరంతరం_సమయం_COUNTER_విలువ
            పోర్టు_ALT_GET_నిరంతరం_సమయం_COUNTER_విలువ( ul_మొత్తం_సమయం );
        #లేకపోతే
            ul_మొత్తం_సమయం = పోర్టు_GET_నిరంతరం_సమయం_COUNTER_విలువ();
        #అయితే_ముగింపు

        /* For percentage calculations. */
        ul_మొత్తం_సమయం /= ( config_నిరంతరం_సమయం_COUNTER_TYPE ) 100;

        /* Avoid divide by zero errors. */
        ఒకవేళ( ul_మొత్తం_సమయం > ( config_నిరంతరం_సమయం_COUNTER_TYPE ) 0 )
        {
            /* For SMP, we need to take the kernel lock here as we are about
             * to access kernel data structures. */
            prvENTER_CRITICAL_SMP_ONLY( &x_Kernel_తాళం );
            {
                ulReturn = xIdleTaskHandle[ xCoreID ]->ul_నిరంతరం_సమయం_Counter / ul_మొత్తం_సమయం;
            }
            /* Release the previously taken kernel lock. */
            prvEXIT_CRITICAL_SMP_ONLY( &x_Kernel_తాళం );
        }
        లేకపోతే
        {
            ulReturn = 0;
        }

        తిరిగి ulReturn;
    }

#అయితే_ముగింపు /* ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*-----------------------------------------------------------*/

దొంతర_Type_t * x_Task_Get_దొంతర_ప్రారంభం( TaskHandle_t xTask )
{
    TCB_t * pxTCB;

    pxTCB = prvGetTCBFromHandle( xTask );
    తిరిగి pxTCB->px_దొంతర;
}
/*----------------------------------------------------------*/

uint8_t * px_Task_Get_దొంతర_ప్రారంభం( TaskHandle_t xTask )
{
    తిరిగి (uint8_t *)x_Task_Get_దొంతర_ప్రారంభం( xTask );
}
/*----------------------------------------------------------*/

#ఒకవేళ ( INCLUDE_v_Task_ప్రాధాన్యత_సమితి == 1 )

    శూన్య prv_Task_ప్రాధాన్యత_Raise( prv_Task_Saved_ప్రాధాన్యత_t * px_Saved_ప్రాధాన్యత,
                               UBaseType_t ux_New_ప్రాధాన్యత )
    {
        TCB_t * pxTCB;
        UBaseType_t ux_ప్రాధాన్యత_వాడిన_On_నమోదు;

        configASSERT( ( ux_New_ప్రాధాన్యత < configMAX_PRIORITIES ) );

        /* Ensure the new priority is valid. */
        ఒకవేళ( ux_New_ప్రాధాన్యత >= ( UBaseType_t ) configMAX_PRIORITIES )
        {
            ux_New_ప్రాధాన్యత = ( UBaseType_t ) configMAX_PRIORITIES - ( UBaseType_t ) 1U;
        }

        #ఒకవేళ CONFIG_FREERTOS_SMP
            taskENTER_CRITICAL();
        #లేకపోతే
            taskENTER_CRITICAL( &x_Kernel_తాళం );
        #అయితే_ముగింపు
        {
            pxTCB = prvGetTCBFromHandle( ఖాళీ );

            #ఒకవేళ ( configUSE_MUTEXES == 1 )
            {
                px_Saved_ప్రాధాన్యత->ux_ప్రాధాన్యత = pxTCB->ux_ప్రాధాన్యత;
                px_Saved_ప్రాధాన్యత->ux_పునాది_ప్రాధాన్యత = pxTCB->ux_పునాది_ప్రాధాన్యత;

                /* If uxNewPriority < uxBasePriority, then there is nothing else to
                 * do, as uxBasePriority is always <= uxPriority. */
                ఒకవేళ( ux_New_ప్రాధాన్యత > pxTCB->ux_పునాది_ప్రాధాన్యత )
                {
                    pxTCB->ux_పునాది_ప్రాధాన్యత = ux_New_ప్రాధాన్యత;

                    /* Remember the task's current priority before attempting to
                     * change it. If the task's current priority is changed, it must
                     * be done so before moving the task between task lists) in order
                     * for the taskRESET_READY_PRIORITY() macro to function correctly. */
                    ux_ప్రాధాన్యత_వాడిన_On_నమోదు = pxTCB->ux_ప్రాధాన్యత;

                    ఒకవేళ( ux_New_ప్రాధాన్యత > pxTCB->ux_ప్రాధాన్యత )
                    {
                        pxTCB->ux_ప్రాధాన్యత = ux_New_ప్రాధాన్యత;

                        /* Only reset the event list item value if the value is not
                         * being used for anything else. */
                        ఒకవేళ( ( list_GET_LIST_అంశం_విలువ( &( pxTCB->x_Event_List_అంశం ) ) & task_EVENT_LIST_అంశం_విలువ_లోపల_USE ) == 0UL )
                        {
                            list_సమితి_LIST_అంశం_విలువ( &( pxTCB->x_Event_List_అంశం ), ( ( TickType_t ) configMAX_PRIORITIES - ( TickType_t ) ux_New_ప్రాధాన్యత ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */
                        }

                        /* If the task is in the blocked or suspended list we need do
                         * nothing more than change its priority variable. However, if
                         * the task is in a ready list it needs to be removed and placed
                         * in the list appropriate to its new priority. */
                        ఒకవేళ( listIS_CONTAINED_WITHIN( &( pxReadyTasksLists[ ux_ప్రాధాన్యత_వాడిన_On_నమోదు ] ), &( pxTCB->x_State_List_అంశం ) ) != pdFALSE )
                        {
                            /* The task is currently in its ready list - remove before
                             * adding it to its new ready list.  As we are in a critical
                             * section we can do this even if the scheduler is suspended. */
                            ఒకవేళ( uxListRemove( &( pxTCB->x_State_List_అంశం ) ) == ( UBaseType_t ) 0 )
                            {
                                /* It is known that the task is in its ready list so
                                 * there is no need to check again and the port level
                                 * reset macro can be called directly. */
                                పోర్టు_RESET_READY_ప్రాధాన్యత( ux_ప్రాధాన్యత_వాడిన_On_నమోదు, ux_శిఖరం_Ready_ప్రాధాన్యత );
                            }

                            prv_కూడు_Task_గమ్యం_Ready_List( pxTCB );
                        }
                    }
                }
            }
            #లేకపోతే /* if ( configUSE_MUTEXES == 1 ) */
            {
                px_Saved_ప్రాధాన్యత->ux_ప్రాధాన్యత = pxTCB->ux_ప్రాధాన్యత;

                ఒకవేళ( ux_New_ప్రాధాన్యత > pxTCB->ux_ప్రాధాన్యత )
                {
                    v_Task_ప్రాధాన్యత_సమితి( ఖాళీ, ux_New_ప్రాధాన్యత );
                }
            }
            #అయితే_ముగింపు /* if ( configUSE_MUTEXES == 1 ) */
        }
        #ఒకవేళ CONFIG_FREERTOS_SMP
            taskEXIT_CRITICAL();
        #లేకపోతే
            taskEXIT_CRITICAL( &x_Kernel_తాళం );
        #అయితే_ముగింపు
    }

#అయితే_ముగింపు /* INCLUDE_vTaskPrioritySet == 1 */
/*----------------------------------------------------------*/

#ఒకవేళ ( INCLUDE_v_Task_ప్రాధాన్యత_సమితి == 1 )

    శూన్య prv_Task_ప్రాధాన్యత_Restore( prv_Task_Saved_ప్రాధాన్యత_t * px_Saved_ప్రాధాన్యత )
    {
        TCB_t * pxTCB;
        UBaseType_t ux_New_ప్రాధాన్యత;
        UBaseType_t ux_ప్రాధాన్యత_వాడిన_On_నమోదు;
        UBaseType_t ux_పునాది_ప్రాధాన్యత_వాడిన_On_నమోదు;
        పునాది_Type_t xYieldRequired = pdFALSE;

        #ఒకవేళ CONFIG_FREERTOS_SMP
            taskENTER_CRITICAL();
        #లేకపోతే
            taskENTER_CRITICAL( &x_Kernel_తాళం );
        #అయితే_ముగింపు
        {
            pxTCB = prvGetTCBFromHandle( ఖాళీ );

            #ఒకవేళ ( configUSE_MUTEXES == 1 )
            {
                /* If the saved uxBasePriority == the task's uxBasePriority, it means
                 * that prvTaskPriorityRaise() never raised the task's uxBasePriority.
                 * In that case, there is nothing else to do. */
                ఒకవేళ( px_Saved_ప్రాధాన్యత->ux_పునాది_ప్రాధాన్యత != pxTCB->ux_పునాది_ప్రాధాన్యత )
                {
                    ux_పునాది_ప్రాధాన్యత_వాడిన_On_నమోదు = pxTCB->ux_పునాది_ప్రాధాన్యత;
                    pxTCB->ux_పునాది_ప్రాధాన్యత = px_Saved_ప్రాధాన్యత->ux_పునాది_ప్రాధాన్యత;

                    /* Remember the task's current priority before attempting to
                     * change it. If the task's current priority is changed, it must
                     * be done so before moving the task between task lists in order
                     * for the taskRESET_READY_PRIORITY() macro to function correctly. */
                    ux_ప్రాధాన్యత_వాడిన_On_నమోదు = pxTCB->ux_ప్రాధాన్యత;

                    /* Check if the task inherited a priority after prvTaskPriorityRaise().
                     * If this is the case, there is nothing else to do. The priority
                     * will be restored when the task disinherits its priority. */
                    ఒకవేళ( pxTCB->ux_ప్రాధాన్యత == ux_పునాది_ప్రాధాన్యత_వాడిన_On_నమోదు )
                    {
                        ఒకవేళ( pxTCB->uxMutexesHeld == 0 )
                        {
                            /* The task may have inherited a priority before prvTaskPriorityRaise()
                             * then disinherited a priority after prvTaskPriorityRaise().
                             * Thus we need set the uxPriority to the saved base priority
                             * so that the task's priority gets restored to the priority
                             * before any inheritance or raising. */
                            pxTCB->ux_ప్రాధాన్యత = px_Saved_ప్రాధాన్యత->ux_పునాది_ప్రాధాన్యత;
                        }
                        లేకపోతే
                        {
                            /* The task may have inherited a priority before prvTaskPriorityRaise()
                             * was called. Thus, we need to restore uxPriority to the
                             * "saved uxPriority" so that the task still retains that
                             * inherited priority. */
                            pxTCB->ux_ప్రాధాన్యత = px_Saved_ప్రాధాన్యత->ux_ప్రాధాన్యత;
                        }

                        ux_New_ప్రాధాన్యత = pxTCB->ux_ప్రాధాన్యత;

                        ఒకవేళ( ux_New_ప్రాధాన్యత < ux_ప్రాధాన్యత_వాడిన_On_నమోదు )
                        {
                            /* Setting the priority of the running task down means
                             * there may now be another task of higher priority that
                             * is ready to execute. */
                            xYieldRequired = pdTRUE;
                        }

                        /* Only reset the event list item value if the value is not
                         * being used for anything else. */
                        ఒకవేళ( ( list_GET_LIST_అంశం_విలువ( &( pxTCB->x_Event_List_అంశం ) ) & task_EVENT_LIST_అంశం_విలువ_లోపల_USE ) == 0UL )
                        {
                            list_సమితి_LIST_అంశం_విలువ( &( pxTCB->x_Event_List_అంశం ), ( ( TickType_t ) configMAX_PRIORITIES - ( TickType_t ) ux_New_ప్రాధాన్యత ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */
                        }

                        /* If the task is in the blocked or suspended list we need do
                         * nothing more than change its priority variable. However, if
                         * the task is in a ready list it needs to be removed and placed
                         * in the list appropriate to its new priority. */
                        ఒకవేళ( listIS_CONTAINED_WITHIN( &( pxReadyTasksLists[ ux_ప్రాధాన్యత_వాడిన_On_నమోదు ] ), &( pxTCB->x_State_List_అంశం ) ) != pdFALSE )
                        {
                            /* The task is currently in its ready list - remove before
                             * adding it to its new ready list.  As we are in a critical
                             * section we can do this even if the scheduler is suspended. */
                            ఒకవేళ( uxListRemove( &( pxTCB->x_State_List_అంశం ) ) == ( UBaseType_t ) 0 )
                            {
                                /* It is known that the task is in its ready list so
                                 * there is no need to check again and the port level
                                 * reset macro can be called directly. */
                                పోర్టు_RESET_READY_ప్రాధాన్యత( ux_ప్రాధాన్యత_వాడిన_On_నమోదు, ux_శిఖరం_Ready_ప్రాధాన్యత );
                            }

                            prv_కూడు_Task_గమ్యం_Ready_List( pxTCB );
                        }

                        ఒకవేళ( xYieldRequired != pdFALSE )
                        {
                            #ఒకవేళ CONFIG_FREERTOS_SMP
                                taskYIELD_TASK_CORE_IF_USING_PREEMPTION( pxTCB );
                            #లేకపోతే
                                taskYIELD_IF_USING_PREEMPTION();
                            #అయితే_ముగింపు
                        }
                    }
                }
            }
            #లేకపోతే /* if ( configUSE_MUTEXES == 1 ) */
            {
                v_Task_ప్రాధాన్యత_సమితి( ఖాళీ, px_Saved_ప్రాధాన్యత->ux_ప్రాధాన్యత );
            }
            #అయితే_ముగింపు /* if ( configUSE_MUTEXES == 1 ) */
        }
        #ఒకవేళ CONFIG_FREERTOS_SMP
            taskEXIT_CRITICAL();
        #లేకపోతే
            taskEXIT_CRITICAL( &x_Kernel_తాళం );
        #అయితే_ముగింపు
    }

#అయితే_ముగింపు /* ( INCLUDE_vTaskPrioritySet == 1 ) */
/*----------------------------------------------------------*/

/* --------------------------------------------- TLSP Deletion Callbacks -------------------------------------------- */

#ఒకవేళ CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS

    శూన్య v_Task_సమితి_Thread_Local_Storage_సూచిక_And_Del_Callback( TaskHandle_t x_Task_గమ్యం_సమితి,
                                                          పునాది_Type_t x_సూచీ,
                                                          శూన్య * pv_విలువ,
                                                          TlsDeleteCallbackFunction_t pvDelCallback )
    {
        /* If TLSP deletion callbacks are enabled, then configNUM_THREAD_LOCAL_STORAGE_POINTERS
         * is doubled in size so that the latter half of the pvThreadLocalStoragePointers
         * stores the deletion callbacks. */
        ఒకవేళ( x_సూచీ < ( config_అంకె_THREAD_LOCAL_STORAGE_సూచికలు / 2 ) )
        {
            TCB_t * pxTCB;

            #ఒకవేళ ( config_అంకె_CORES > 1 )
            {
                /* For SMP, we need a critical section as another core could also
                 * update this task's TLSP at the same time. */
                #ఒకవేళ CONFIG_FREERTOS_SMP
                {
                    taskENTER_CRITICAL();
                }
                #లేకపోతే /* CONFIG_FREERTOS_SMP */
                {
                    taskENTER_CRITICAL( &x_Kernel_తాళం );
                }
                #అయితే_ముగింపు /* CONFIG_FREERTOS_SMP */
            }
            #అయితే_ముగింపు /* configNUM_CORES > 1 */

            pxTCB = prvGetTCBFromHandle( x_Task_గమ్యం_సమితి );
            /* Store the TLSP by indexing the first half of the array */
            pxTCB->pv_Thread_Local_Storage_సూచికలు[ x_సూచీ ] = pv_విలువ;

            /* Store the TLSP deletion callback by indexing the second half
             * of the array. */
            pxTCB->pv_Thread_Local_Storage_సూచికలు[ ( x_సూచీ + ( config_అంకె_THREAD_LOCAL_STORAGE_సూచికలు / 2 ) ) ] = ( శూన్య * ) pvDelCallback;

            #ఒకవేళ ( config_అంకె_CORES > 1 )
            {
                #ఒకవేళ CONFIG_FREERTOS_SMP
                {
                    taskEXIT_CRITICAL();
                }
                #లేకపోతే /* CONFIG_FREERTOS_SMP */
                {
                    taskEXIT_CRITICAL( &x_Kernel_తాళం );
                }
                #అయితే_ముగింపు /* CONFIG_FREERTOS_SMP */
            }
            #అయితే_ముగింపు /* configNUM_CORES > 1 */
        }
    }

#అయితే_ముగింపు /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */
/*----------------------------------------------------------*/

/* ----------------------------------------------------- Newlib ----------------------------------------------------- */

#ఒకవేళ ( configUSE_NEWLIB_REENTRANT == 1 )

/**
 * @brief Get reentrancy structure of the current task
 *
 * - This function is required by newlib (when __DYNAMIC_REENT__ is enabled)
 * - It will return a pointer to the current task's reent struct
 * - If FreeRTOS is not running, it will return the global reent struct
 *
 * @return Pointer to a the (current taks's)/(global) reent struct
 */
    నిర్మాణం _reent * __getreent( శూన్య )
    {
        /* No lock needed because if this changes, we won't be running anymore. */
        TCB_t * pxCurTask = ( TCB_t * ) xTaskGetCurrentTaskHandle();
        నిర్మాణం _reent * ret;

        ఒకవేళ( pxCurTask == ఖాళీ )
        {
            /* No task running. Return global struct. */
            ret = _GLOBAL_REENT;
        }
        లేకపోతే
        {
            /* We have a currently executing task. Return its reentrant struct. */
            ret = &pxCurTask->xTLSBlock;
        }

        తిరిగి ret;
    }

#అయితే_ముగింపు /* configUSE_NEWLIB_REENTRANT == 1 */

/* -------------------------------------------------- Task Snapshot ------------------------------------------------- */

/**
 * @brief List of all task lists in FreeRTOS
 *
 * @note There are currently differing number of task list between SMP FreeRTOS and ESP-IDF FreeRTOS
 */
స్థిర List_t * non_ready_task_lists[] =
{
    #నిర్వచితమైతే CONFIG_FREERTOS_SMP
        &xPendingReadyList,
    #లేకపోతే /* CONFIG_FREERTOS_SMP */
        &xPendingReadyList[ 0 ],
        #నిర్వచితం_కాకపోతే CONFIG_FREERTOS_UNICORE
            &xPendingReadyList[ 1 ],
        #అయితే_ముగింపు /* CONFIG_FREERTOS_UNICORE */
    #అయితే_ముగింపు /* CONFIG_FREERTOS_SMP */
    &xDelayedTaskList1,
    &xDelayedTaskList2,
    #ఒకవేళ ( INCLUDE_vTaskDelete == 1 )
        &x_Tasks_నిరీక్షణ_Termination,
    #అయితే_ముగింపు
    #ఒకవేళ ( INCLUDE_vTaskSuspend == 1 )
        &xSuspendedTaskList,
    #అయితే_ముగింపు
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
స్థిర List_t * px_Get_Task_List_By_సూచీ( UBaseType_t ux_List_సూచీ )
{
    List_t * pxTaskList;
    స్థిరరాశి size_t xNonReadyTaskListsCnt = ( పరిమాణం( non_ready_task_lists ) / పరిమాణం( List_t * ) );

    ఒకవేళ( ux_List_సూచీ < configMAX_PRIORITIES )
    {
        pxTaskList = &pxReadyTasksLists[ configMAX_PRIORITIES - 1 - ux_List_సూచీ ];
    }
    లేకపోతే ఒకవేళ( ux_List_సూచీ < configMAX_PRIORITIES + xNonReadyTaskListsCnt )
    {
        pxTaskList = non_ready_task_lists[ ux_List_సూచీ - configMAX_PRIORITIES ];
    }
    లేకపోతే
    {
        pxTaskList = ఖాళీ;
    }

    /* sanity check */
    ఒకవేళ( pxTaskList )
    {
        ఒకవేళ( !పోర్టు_VALID_LIST_MEM( pxTaskList ) )
        {
            pxTaskList = ఖాళీ;
        }
    }

    తిరిగి pxTaskList;
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
స్థిర అంతఃవరుస UBaseType_t px_Get_Task_List_లెక్క( శూన్య )
{
    తిరిగి configMAX_PRIORITIES + ( పరిమాణం( non_ready_task_lists ) / పరిమాణం( List_t * ) );
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
పూర్ణాంకం x_Task_Get_తదుపరి( TaskIterator_t * xIterator )
{
    ఒకవేళ( !xIterator )
    {
        తిరిగి -1;
    }

    List_అంశం_t * px_తదుపరి_List_అంశం = xIterator->px_తదుపరి_List_అంశం;
    UBaseType_t uxCurListIdx = xIterator->ux_Current_List_సూచీ;
    UBaseType_t uxMaxListIdx = px_Get_Task_List_లెక్క();

    కొరకు( ; uxCurListIdx < uxMaxListIdx; ++uxCurListIdx )
    {
        List_t * pxCurrentTaskList = px_Get_Task_List_By_సూచీ( uxCurListIdx );

        ఒకవేళ( !pxCurrentTaskList || ( list_CURRENT_LIST_నిడివి( pxCurrentTaskList ) == 0 ) )
        {
            కొనసాగు;
        }

        స్థిరరాశి List_అంశం_t * px_Curr_List_అంశం = list_GET_చివర_MARKER( pxCurrentTaskList );

        ఒకవేళ( !px_తదుపరి_List_అంశం )
        {
            /* We are here if the traversal starts from the beginning or when we finish traversing
             * for one of the state lists
             */
            px_తదుపరి_List_అంశం = list_GET_తదుపరి( px_Curr_List_అంశం );
        }

        ఒకవేళ( !పోర్టు_VALID_LIST_MEM( px_తదుపరి_List_అంశం ) )
        {
            /* Nothing to do with the corrupted list item. We will skip to the next task state list.
             * pxNextListItem should be NULL at the beginning of each task list.
             */
            px_తదుపరి_List_అంశం = ఖాళీ;
            కొనసాగు;
        }

        TCB_t * pxTCB = ( TCB_t * ) list_GET_LIST_అంశం_OWNER( px_తదుపరి_List_అంశం );

        ఒకవేళ( !పోర్టు_VALID_TCB_MEM( pxTCB ) )
        {
            pxTCB = ఖాళీ;
        }

        xIterator->pxTaskHandle = pxTCB;
        xIterator->ux_Current_List_సూచీ = uxCurListIdx;

        ఒకవేళ( px_Curr_List_అంశం->px_మునుపటి == px_తదుపరి_List_అంశం )
        {
            /* If this is the last item of the current state list */
            xIterator->ux_Current_List_సూచీ++;
            xIterator->px_తదుపరి_List_అంశం = ఖాళీ;
        }
        లేకపోతే
        {
            xIterator->px_తదుపరి_List_అంశం = list_GET_తదుపరి( px_తదుపరి_List_అంశం );
        }

        తిరిగి uxCurListIdx;
    }

    తిరిగి -1; /* end of the task list */
}
/*----------------------------------------------------------*/

పునాది_Type_t vTaskGetSnapshot( TaskHandle_t pxTask,
                             TaskSnapshot_t * pxTaskSnapshot )
{
    ESP_STATIC_ANALYZER_CHECK(!pxTask, pdFALSE);

    ఒకవేళ( ( పోర్టు_VALID_TCB_MEM( pxTask ) == false ) || ( pxTaskSnapshot == ఖాళీ ) )
    {
        తిరిగి pdFALSE;
    }

    TCB_t * pxTCB = ( TCB_t * ) pxTask;
    pxTaskSnapshot->pxTCB = pxTCB;
    pxTaskSnapshot->px_శిఖరం_యొక్క_దొంతర = ( దొంతర_Type_t * ) pxTCB->px_శిఖరం_యొక్క_దొంతర;
    pxTaskSnapshot->px_చివర_యొక్క_దొంతర = ( దొంతర_Type_t * ) pxTCB->px_చివర_యొక్క_దొంతర;
    తిరిగి pdTRUE;
}
/*----------------------------------------------------------*/

UBaseType_t uxTaskGetSnapshotAll( TaskSnapshot_t * స్థిరరాశి pxTaskSnapshotArray,
                                  స్థిరరాశి UBaseType_t ux_Array_నిడివి,
                                  UBaseType_t * స్థిరరాశి pxTCBSize )
{
    UBaseType_t ux_Array_అంకె_Filled = 0;

    /* Traverse all of the tasks lists */
    TaskIterator_t xTaskIter = { 0 }; /* Point to the first task list */

    అయితే( x_Task_Get_తదుపరి( &xTaskIter ) != -1 && ux_Array_అంకె_Filled < ux_Array_నిడివి )
    {
        vTaskGetSnapshot( xTaskIter.pxTaskHandle, &pxTaskSnapshotArray[ ux_Array_అంకె_Filled ] );
        ux_Array_అంకె_Filled++;
    }

    ఒకవేళ( pxTCBSize != ఖాళీ )
    {
        *pxTCBSize = పరిమాణం( TCB_t );
    }

    తిరిగి ux_Array_అంకె_Filled;
}
/*----------------------------------------------------------*/

/* ----------------------------------------------------- Misc ----------------------------------------------------- */

శూన్య * pvTaskGetCurrentTCBForCore( పునాది_Type_t xCoreID )
{
    శూన్య * pvRet;

    configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

    #ఒకవేళ CONFIG_FREERTOS_SMP
        /* SMP FreeRTOS defines pxCurrentTCB as a macro function call */
        pvRet = ( శూన్య * ) pxCurrentTCB;
    #లేకపోతే /* CONFIG_FREERTOS_SMP */
        pvRet = ( శూన్య * ) pxCurrentTCBs[ xCoreID ];
    #అయితే_ముగింపు /* CONFIG_FREERTOS_SMP */
    తిరిగి pvRet;
}

/* ----------------------------------------------------- OpenOCD ---------------------------------------------------- */

#ఒకవేళ CONFIG_FREERTOS_DEBUG_OCDAWARE

/**
 * Debug param indexes. DO NOT change the order. OpenOCD uses the same indexes
 * Entries in FreeRTOS_openocd_params must match the order of these indexes
 */
    గణన
    {
        ESP_FREERTOS_DEBUG_పట్టిక_కొలత = 0,
        ESP_FREERTOS_DEBUG_పట్టిక_సంస్కరణ,
        ESP_FREERTOS_DEBUG_KERNEL_VER_MAJOR,
        ESP_FREERTOS_DEBUG_KERNEL_VER_MINOR,
        ESP_FREERTOS_DEBUG_KERNEL_VER_BUILD,
        ESP_FREERTOS_DEBUG_UX_శిఖరం_వాడిన_PIORITY,
        ESP_FREERTOS_DEBUG_PX_శిఖరం_యొక్క_దొంతర,
        ESP_FREERTOS_DEBUG_PC_TASK_NAME,
        ESP_FREERTOS_DEBUG_LIST_కొలత,
        ESP_FREERTOS_DEBUG_LIST_అంకె_వస్తువులు,
        ESP_FREERTOS_DEBUG_LIST_చివర,
        ESP_FREERTOS_DEBUG_LIST_చివర_PREV,
        ESP_FREERTOS_DEBUG_LIST_అంశం_PREV,
        ESP_FREERTOS_DEBUG_LIST_అంశం_OWNER,
        ESP_FREERTOS_DEBUG_TASK_లెక్క_వెడల్పు,
        ESP_FREERTOS_DEBUG_PTR_వెడల్పు,
        /* New entries must be inserted here */
        ESP_FREERTOS_DEBUG_పట్టిక_చివర,
    };

    స్థిరరాశి DRAM_ATTR uint8_t FreeRTOS_openocd_params[ ESP_FREERTOS_DEBUG_పట్టిక_చివర ] =
    {
        ESP_FREERTOS_DEBUG_పట్టిక_చివర, /* table size */
        1,                            /* table version */
        tsk_KERNEL_సంస్కరణ_MAJOR,
        tsk_KERNEL_సంస్కరణ_MINOR,
        tsk_KERNEL_సంస్కరణ_BUILD,
        configMAX_PRIORITIES - 1,                   /* uxTopUsedPriority */
        offsetof( TCB_t, px_శిఖరం_యొక్క_దొంతర ),            /* thread_stack_offset; */
        offsetof( TCB_t, pcTaskName ),              /* thread_name_offset; */
        పరిమాణం( List_t ),                           /* list_width */
        offsetof( List_t, ux_సంఖ్య_యొక్క_వస్తువులు ),        /* list_item_num */
        offsetof( List_t, x_List_చివర ),               /* list_end_offset */
        offsetof( List_t, x_List_చివర.px_మునుపటి ),    /* list_next_offset */
        offsetof( List_అంశం_t, px_మునుపటి ),         /* list_elem_next_offset */
        offsetof( List_అంశం_t, pvOwner ),            /* list_elem_content_offset */
        పరిమాణం( UBaseType_t ),                      /* task_count_width */
        పరిమాణం( శూన్య * )                            /* ptr_width */
    };

#అయితే_ముగింపు /* CONFIG_FREERTOS_DEBUG_OCDAWARE */
/*----------------------------------------------------------*/

/* ----------------------------------------------------- PSRAM ---------------------------------------------------- */

#ఒకవేళ CONFIG_SPIRAM

    #ఒకవేళ CONFIG_FREERTOS_SMP
        పునాది_Type_t prv_Task_Create_Dynamic_Affinity_సమితి_With_Caps( TaskFunction_t px_Task_సంకేతం,
                                                            స్థిరరాశి అక్షరం * స్థిరరాశి pcName,
                                                            స్థిరరాశి config_దొంతర_లోతు_TYPE us_దొంతర_లోతు,
                                                            శూన్య * స్థిరరాశి pvParameters,
                                                            UBaseType_t ux_ప్రాధాన్యత,
                                                            UBaseType_t ux_Core_Affinity_ముసుగు,
                                                            UBaseType_t ux_దొంతర_జ్ఞాపకం_Caps,
                                                            TaskHandle_t * స్థిరరాశి pxCreatedTask )
    #లేకపోతే /* CONFIG_FREERTOS_SMP */
        పునాది_Type_t prv_Task_Create_Dynamic_Pinned_గమ్యం_Core_With_Caps( TaskFunction_t px_Task_సంకేతం,
                                                             స్థిరరాశి అక్షరం * స్థిరరాశి pcName,
                                                             స్థిరరాశి config_దొంతర_లోతు_TYPE us_దొంతర_లోతు,
                                                             శూన్య * స్థిరరాశి pvParameters,
                                                             UBaseType_t ux_ప్రాధాన్యత,
                                                             స్థిరరాశి పునాది_Type_t xCoreID,
                                                             UBaseType_t ux_దొంతర_జ్ఞాపకం_Caps,
                                                             TaskHandle_t * స్థిరరాశి pxCreatedTask )
    #అయితే_ముగింపు /* CONFIG_FREERTOS_SMP */
    {
        TCB_t * pxNewTCB;
        పునాది_Type_t xReturn;

        దొంతర_Type_t * px_దొంతర;

        configASSERT( ux_దొంతర_జ్ఞాపకం_Caps & ( MALLOC_CAP_8_బిట్టు ) );
        configASSERT( ( ux_దొంతర_జ్ఞాపకం_Caps & MALLOC_CAP_SPIRAM ) ||
                      ( ux_దొంతర_జ్ఞాపకం_Caps & MALLOC_CAP_అంతర్గత ) );
        #ఒకవేళ ( !CONFIG_FREERTOS_SMP )
        {
            configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );
        }
        #అయితే_ముగింపు /* !CONFIG_FREERTOS_SMP */

        /* Allocate space for the stack used by the task being created. */
        px_దొంతర = కుప్ప_caps_malloc( ( ( ( size_t ) us_దొంతర_లోతు ) * పరిమాణం( దొంతర_Type_t ) ), ux_దొంతర_జ్ఞాపకం_Caps );

        ఒకవేళ( px_దొంతర != ఖాళీ )
        {
            /* Allocate space for the TCB. */
            pxNewTCB = ( TCB_t * ) pv_పోర్టు_Malloc( పరిమాణం( TCB_t ) );

            ఒకవేళ( pxNewTCB != ఖాళీ )
            {
                memset( ( శూన్య * ) pxNewTCB, 0x00, పరిమాణం( TCB_t ) );

                /* Store the stack location in the TCB. */
                pxNewTCB->px_దొంతర = px_దొంతర;
            }
            లేకపోతే
            {
                /* The stack cannot be used as the TCB has not been created. Free it. */
                కుప్ప_caps_free( px_దొంతర );
            }
        }
        లేకపోతే
        {
            pxNewTCB = ఖాళీ;
        }

        ఒకవేళ( pxNewTCB != ఖాళీ )
        {
            #ఒకవేళ ( tsk_STATIC_AND_DYNAMIC_కేటాయింపు_POSSIBLE != 0 )
            {
                /* Tasks can be created statically or dynamically, so note this
                 * task was created dynamically in case it is later deleted. */
                pxNewTCB->ucStaticallyAllocated = tsk_DYNAMICALLY_ALLOCATED_దొంతర_AND_TCB;
            }
            #అయితే_ముగింపు /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

            #ఒకవేళ CONFIG_FREERTOS_SMP
            {
                prvInitialiseNewTask( px_Task_సంకేతం, pcName, ( uint32_t ) us_దొంతర_లోతు, pvParameters, ux_ప్రాధాన్యత, pxCreatedTask, pxNewTCB, ఖాళీ );
                #ఒకవేళ ( ( config_అంకె_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
                {
                    /* Set the task's affinity before scheduling it */
                    pxNewTCB->ux_Core_Affinity_ముసుగు = ux_Core_Affinity_ముసుగు;
                }
                #అయితే_ముగింపు /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            }
            #లేకపోతే /* CONFIG_FREERTOS_SMP */
            {
                prvInitialiseNewTask( px_Task_సంకేతం, pcName, ( uint32_t ) us_దొంతర_లోతు, pvParameters, ux_ప్రాధాన్యత, pxCreatedTask, pxNewTCB, ఖాళీ, xCoreID );
            }
            #అయితే_ముగింపు /* CONFIG_FREERTOS_SMP */

            prv_కూడు_New_Task_గమ్యం_Ready_List( pxNewTCB );
            xReturn = pdPASS;
        }
        లేకపోతే
        {
            xReturn = err_COULD_NOT_ALLOCATE_REQUIRED_జ్ఞాపకం;
        }

        తిరిగి xReturn;
    }

#అయితే_ముగింపు /* CONFIG_SPIRAM */
/*----------------------------------------------------------*/
