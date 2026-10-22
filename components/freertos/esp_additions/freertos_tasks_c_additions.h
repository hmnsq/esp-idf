/*
 * SPDX-FileCopyrightText: 2015-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ضمن "sdkconfig.h"
#ضمن "esp_assert.h"
#ضمن "esp_heap_caps.h"
#ضمن "esp_compiler.h"
#ضمن "freertos/idf_additions.h"
#ضمن "freertos/freertos_debug.h"
#ضمن "esp_private/freertos_idf_additions_priv.h"

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
_Static_assert( offsetof( StaticTask_t, pxDummy6 ) == offsetof( TCB_t, px_ڍير ) );
_Static_assert( offsetof( StaticTask_t, pxDummy8 ) == offsetof( TCB_t, px_پڇاڙي_جو_ڍير ) );
#إذا !CONFIG_IDF_هدف_LINUX    // Disabled for linux builds due to differences in types
_Static_assert( tskNO_AFFINITY == ( بنياد_Type_t ) CONFIG_FREERTOS_NO_AFFINITY, "CONFIG_FREERTOS_NO_AFFINITY must be the same as tskNO_AFFINITY" );
#جيڪڏهن_پورو

/* ------------------------------------------------- Kernel Control ------------------------------------------------- */

#إذا ( !CONFIG_FREERTOS_SMP && ( config_انگ_CORES > 1 ) )

/*
 * Wrapper function to take "xKerneLock"
 */
    فراغ prv_Take_Kernel_تالو( فراغ )
    {
        /* We call the tasks.c critical section macro to take xKernelLock */
        taskENTER_CRITICAL( &x_Kernel_تالو );
    }

#جيڪڏهن_پورو /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#إذا ( !CONFIG_FREERTOS_SMP && ( config_انگ_CORES > 1 ) )

/*
 * Wrapper function to release "xKerneLock"
 */
    فراغ prv_Release_Kernel_تالو( فراغ )
    {
        /* We call the tasks.c critical section macro to release xKernelLock */
        taskEXIT_CRITICAL( &x_Kernel_تالو );
    }

#جيڪڏهن_پورو /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#إذا ( CONFIG_FREERTOS_SMP && ( config_انگ_CORES > 1 ) )

/*
 * Workaround for non-thread safe multi-core OS startup (see IDF-4524)
 */
    فراغ prv_شروعات_Scheduler_Other_Cores( فراغ )
    {
        /* This function is always called with interrupts disabled*/
        xSchedulerRunning = pdTRUE;
    }

#جيڪڏهن_پورو /* ( CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#إذا ( !CONFIG_FREERTOS_SMP && ( config_انگ_CORES > 1 ) )

    بنياد_Type_t xTaskIncrementTickOtherCores( فراغ )
    {
        /* Minor optimization. This function can never switch cores mid
         * execution */
        بنياد_Type_t xCoreID = پورٽ_GET_CORE_ID();
        بنياد_Type_t xSwitchRequired = pdFALSE;

        /* This function should never be called by Core 0. */
        configASSERT( xCoreID != 0 );

        /* Called by the portable layer each time a tick interrupt occurs
         * on a core other than core 0. */
        traceTASK_INCREMENT_TICK( x_Tick_ڳڻپ );

        إذا( uxSchedulerSuspended[ xCoreID ] == ( UBaseType_t ) 0U )
        {
            /* We need take the kernel lock here as we are about to access
             * kernel data structures. */
            taskENTER_CRITICAL_ISR( &x_Kernel_تالو );

            /* Tasks of equal priority to the currently running task will share
             * processing time (time slice) if preemption is on, and the application
             * writer has not explicitly turned time slicing off. */
            #إذا ( ( configUSE_PREEMPTION == 1 ) && ( config_USE_وقت_SLICING == 1 ) )
            {
                إذا( list_CURRENT_LIST_ڊيگهائي( &( pxReadyTasksLists[ pxCurrentTCBs[ xCoreID ]->ux_ترجيح ] ) ) > ( UBaseType_t ) 1 )
                {
                    xSwitchRequired = pdTRUE;
                }
                وإلا
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            #جيڪڏهن_پورو /* ( ( configUSE_PREEMPTION == 1 ) && ( configUSE_TIME_SLICING == 1 ) ) */

            /* Release the previously taken kernel lock as we have finished
             * accessing the kernel data structures. */
            taskEXIT_CRITICAL_ISR( &x_Kernel_تالو );

            #إذا ( configUSE_PREEMPTION == 1 )
            {
                إذا( xYieldPending[ xCoreID ] != pdFALSE )
                {
                    xSwitchRequired = pdTRUE;
                }
                وإلا
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            #جيڪڏهن_پورو /* configUSE_PREEMPTION */
        }

        #إذا ( configUSE_TICK_HOOK == 1 )
        {
            vApplicationTickHook();
        }
        #جيڪڏهن_پورو

        أرجع xSwitchRequired;
    }

#جيڪڏهن_پورو /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

/* -------------------------------------------------- Task Creation ------------------------------------------------- */

#إذا ( config_SUPPORT_DYNAMIC_ورهاست == 1 )

    بنياد_Type_t x_Task_Create_Pinned_منزل_Core( TaskFunction_t px_Task_ڪوڊ,
                                        ثابت حرف * ثابت pcName,
                                        ثابت uint32_t us_ڍير_اونهائي,
                                        فراغ * ثابت pvParameters,
                                        UBaseType_t ux_ترجيح,
                                        TaskHandle_t * ثابت pxCreatedTask,
                                        ثابت بنياد_Type_t xCoreID )
    {
        بنياد_Type_t xReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );

        #إذا CONFIG_FREERTOS_SMP
        {
            /* If using Amazon SMP FreeRTOS. This function is just a wrapper around
             * xTaskCreate() or xTaskCreateAffinitySet(). */
            #إذا ( ( config_انگ_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
            {
                /* Convert xCoreID into an affinity mask */
                UBaseType_t ux_Core_Affinity_نقاب;

                /* Bit shifting << xCoreID is only valid if we have less than
                 * 32 cores. */
                ESP_STATIC_ASSERT( config_انگ_CORES < 32 );

                إذا( xCoreID == tskNO_AFFINITY )
                {
                    ux_Core_Affinity_نقاب = tskNO_AFFINITY;
                }
                وإلا
                {
                    ux_Core_Affinity_نقاب = ( 1 << xCoreID );
                }

                xReturn = x_Task_Create_Affinity_سيٽ( px_Task_ڪوڊ, pcName, us_ڍير_اونهائي, pvParameters, ux_ترجيح, ux_Core_Affinity_نقاب, pxCreatedTask );
            }
            #وإلا /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            {
                xReturn = xTaskCreate( px_Task_ڪوڊ, pcName, us_ڍير_اونهائي, pvParameters, ux_ترجيح, pxCreatedTask );
            }
            #جيڪڏهن_پورو /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
        }
        #وإلا /* CONFIG_FREERTOS_SMP */
        {
            TCB_t * pxNewTCB;

            /* If the stack grows down then allocate the stack then the TCB so the
             * stack does not grow into the TCB.  Likewise if the stack grows up
             * then allocate the TCB then the stack. */
            #إذا ( پورٽ_ڍير_GROWTH > 0 )
            {
                /* Allocate space for the TCB.  Where the memory comes from depends on
                 * the implementation of the port malloc function and whether or not static
                 * allocation is being used. */
                pxNewTCB = ( TCB_t * ) pv_پورٽ_Malloc( حجم( TCB_t ) );

                إذا( pxNewTCB != عدم )
                {
                    memset( ( فراغ * ) pxNewTCB, 0x00, حجم( TCB_t ) );

                    /* Allocate space for the stack used by the task being created.
                     * The base of the stack memory stored in the TCB so the task can
                     * be deleted later if required. */
                    pxNewTCB->px_ڍير = ( ڍير_Type_t * ) pv_پورٽ_Malloc_ڍير( ( ( ( size_t ) us_ڍير_اونهائي ) * حجم( ڍير_Type_t ) ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */

                    إذا( pxNewTCB->px_ڍير == عدم )
                    {
                        /* Could not allocate the stack.  Delete the allocated TCB. */
                        v_پورٽ_Free( pxNewTCB );
                        pxNewTCB = عدم;
                    }
                }
            }
            #وإلا /* portSTACK_GROWTH */
            {
                ڍير_Type_t * px_ڍير;

                /* Allocate space for the stack used by the task being created. */
                px_ڍير = pv_پورٽ_Malloc_ڍير( ( ( ( size_t ) us_ڍير_اونهائي ) * حجم( ڍير_Type_t ) ) ); /*lint !e9079 All values returned by pvPortMalloc() have at least the alignment required by the MCU's stack and this allocation is the stack. */

                إذا( px_ڍير != عدم )
                {
                    /* Allocate space for the TCB. */
                    pxNewTCB = ( TCB_t * ) pv_پورٽ_Malloc( حجم( TCB_t ) ); /*lint !e9087 !e9079 All values returned by pvPortMalloc() have at least the alignment required by the MCU's stack, and the first member of TCB_t is always a pointer to the task's stack. */

                    إذا( pxNewTCB != عدم )
                    {
                        memset( ( فراغ * ) pxNewTCB, 0x00, حجم( TCB_t ) );

                        /* Store the stack location in the TCB. */
                        pxNewTCB->px_ڍير = px_ڍير;
                    }
                    وإلا
                    {
                        /* The stack cannot be used as the TCB was not created.  Free
                         * it again. */
                        v_پورٽ_Free_ڍير( px_ڍير );
                    }
                }
                وإلا
                {
                    pxNewTCB = عدم;
                }
            }
            #جيڪڏهن_پورو /* portSTACK_GROWTH */

            إذا( pxNewTCB != عدم )
            {
                #إذا ( tsk_STATIC_AND_DYNAMIC_ورهاست_POSSIBLE != 0 ) /*lint !e9029 !e731 Macro has been consolidated for readability reasons. */
                {
                    /* Tasks can be created statically or dynamically, so note this
                     * task was created dynamically in case it is later deleted. */
                    pxNewTCB->ucStaticallyAllocated = tsk_DYNAMICALLY_ALLOCATED_ڍير_AND_TCB;
                }
                #جيڪڏهن_پورو /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

                prvInitialiseNewTask( px_Task_ڪوڊ, pcName, ( uint32_t ) us_ڍير_اونهائي, pvParameters, ux_ترجيح, pxCreatedTask, pxNewTCB, عدم, xCoreID );
                prv_جوڙيو_New_Task_منزل_Ready_List( pxNewTCB );
                xReturn = pdPASS;
            }
            وإلا
            {
                xReturn = err_COULD_NOT_ALLOCATE_REQUIRED_يادگيري;
            }
        }
        #جيڪڏهن_پورو /* CONFIG_FREERTOS_SMP */

        أرجع xReturn;
    }

#جيڪڏهن_پورو /* ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#إذا ( config_SUPPORT_STATIC_ورهاست == 1 )

    TaskHandle_t x_Task_Create_Static_Pinned_منزل_Core( TaskFunction_t px_Task_ڪوڊ,
                                                ثابت حرف * ثابت pcName,
                                                ثابت uint32_t ul_ڍير_اونهائي,
                                                فراغ * ثابت pvParameters,
                                                UBaseType_t ux_ترجيح,
                                                ڍير_Type_t * ثابت pux_ڍير_بفر,
                                                StaticTask_t * ثابت px_Task_بفر,
                                                ثابت بنياد_Type_t xCoreID )
    {
        TaskHandle_t xReturn;

        configASSERT( پورٽ_VALID_ڍير_MEM( pux_ڍير_بفر ) );
        configASSERT( پورٽ_VALID_TCB_MEM( px_Task_بفر ) );
        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );

        #إذا CONFIG_FREERTOS_SMP
        {
            /* If using Amazon SMP FreeRTOS. This function is just a wrapper around
             * xTaskCreateStatic() or xTaskCreateStaticAffinitySet(). */
            #إذا ( ( config_انگ_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
            {
                /* Convert xCoreID into an affinity mask */
                UBaseType_t ux_Core_Affinity_نقاب;

                /* Bit shifting << xCoreID is only valid if we have less than
                 * 32 cores. */
                ESP_STATIC_ASSERT( config_انگ_CORES < 32 );

                إذا( xCoreID == tskNO_AFFINITY )
                {
                    ux_Core_Affinity_نقاب = tskNO_AFFINITY;
                }
                وإلا
                {
                    ux_Core_Affinity_نقاب = ( 1 << xCoreID );
                }

                xReturn = x_Task_Create_Static_Affinity_سيٽ( px_Task_ڪوڊ, pcName, ul_ڍير_اونهائي, pvParameters, ux_ترجيح, pux_ڍير_بفر, px_Task_بفر, ux_Core_Affinity_نقاب );
            }
            #وإلا /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            {
                xReturn = xTaskCreateStatic( px_Task_ڪوڊ, pcName, ul_ڍير_اونهائي, pvParameters, ux_ترجيح, pux_ڍير_بفر, px_Task_بفر );
            }
            #جيڪڏهن_پورو /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
        }
        #وإلا /* CONFIG_FREERTOS_SMP */
        {
            TCB_t * pxNewTCB;

            #إذا ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticTask_t equals the size of the real task
                 * structure. */
                متطاير size_t x_سائيز = حجم( StaticTask_t );
                configASSERT( x_سائيز == حجم( TCB_t ) );
                ( فراغ ) x_سائيز; /* Prevent lint warning when configASSERT() is not used. */
            }
            #جيڪڏهن_پورو /* configASSERT_DEFINED */

            إذا( ( px_Task_بفر != عدم ) && ( pux_ڍير_بفر != عدم ) )
            {
                /* The memory used for the task's TCB and stack are passed into this
                 * function - use them. */
                pxNewTCB = ( TCB_t * ) px_Task_بفر; /*lint !e740 !e9087 Unusual cast is ok as the structures are designed to have the same alignment, and the size is checked by an assert. */
                memset( ( فراغ * ) pxNewTCB, 0x00, حجم( TCB_t ) );
                pxNewTCB->px_ڍير = ( ڍير_Type_t * ) pux_ڍير_بفر;

                #إذا ( tsk_STATIC_AND_DYNAMIC_ورهاست_POSSIBLE != 0 ) /*lint !e731 !e9029 Macro has been consolidated for readability reasons. */
                {
                    /* Tasks can be created statically or dynamically, so note this
                     * task was created statically in case the task is later deleted. */
                    pxNewTCB->ucStaticallyAllocated = tsk_STATICALLY_ALLOCATED_ڍير_AND_TCB;
                }
                #جيڪڏهن_پورو /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

                prvInitialiseNewTask( px_Task_ڪوڊ, pcName, ul_ڍير_اونهائي, pvParameters, ux_ترجيح, &xReturn, pxNewTCB, عدم, xCoreID );
                prv_جوڙيو_New_Task_منزل_Ready_List( pxNewTCB );
            }
            وإلا
            {
                xReturn = عدم;
            }
        }
        #جيڪڏهن_پورو /* CONFIG_FREERTOS_SMP */

        أرجع xReturn;
    }

#جيڪڏهن_پورو /* ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#إذا ( configUSE_TIMERS == 1 )

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
    بنياد_Type_t __attribute__( ( weak ) ) xTimerCreateTimerTask( فراغ )
    {
        أرجع pdPASS;
    }

#جيڪڏهن_پورو /* configUSE_TIMERS */
/*----------------------------------------------------------*/

/* ------------------------------------------------- Task Utilities ------------------------------------------------- */

بنياد_Type_t xTaskGetCoreID( TaskHandle_t xTask )
{
    بنياد_Type_t xReturn;

    #إذا ( config_انگ_CORES > 1 )
    {
        #إذا CONFIG_FREERTOS_SMP
            UBaseType_t ux_Core_Affinity_نقاب;

            /* Get the core affinity mask and convert it to an ID */
            ux_Core_Affinity_نقاب = vTaskCoreAffinityGet( xTask );

            /* If the task is not pinned to a particular core, treat it as tskNO_AFFINITY */
            إذا( ux_Core_Affinity_نقاب & ( ux_Core_Affinity_نقاب - 1 ) ) /* If more than one bit set */
            {
                xReturn = tskNO_AFFINITY;
            }
            وإلا
            {
                صحيح i_اشاريو_Plus_One = __builtin_ffs( ux_Core_Affinity_نقاب );
                أكد( i_اشاريو_Plus_One >= 1 );
                xReturn = i_اشاريو_Plus_One - 1;
            }
        #وإلا /* CONFIG_FREERTOS_SMP */
            TCB_t * pxTCB;

            pxTCB = prvGetTCBFromHandle( xTask );

            xReturn = pxTCB->xCoreID;
        #جيڪڏهن_پورو /* CONFIG_FREERTOS_SMP */
    }
    #وإلا /* configNUM_CORES > 1 */
    {
        /* Single-core. Just return a core ID of 0 */
        xReturn = 0;
    }
    #جيڪڏهن_پورو /* configNUM_CORES > 1 */

    أرجع xReturn;
}
/*----------------------------------------------------------*/

#إذا ( ( !CONFIG_FREERTOS_SMP ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

    TaskHandle_t xTaskGetIdleTaskHandleForCore( بنياد_Type_t xCoreID )
    {
        /* If xTaskGetIdleTaskHandle() is called before the scheduler has been
         * started, then xIdleTaskHandle will be NULL. */
        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );
        configASSERT( ( xIdleTaskHandle[ xCoreID ] != عدم ) );
        أرجع xIdleTaskHandle[ xCoreID ];
    }

#جيڪڏهن_پورو /* ( ( !CONFIG_FREERTOS_SMP ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*----------------------------------------------------------*/

#إذا ( ( !CONFIG_FREERTOS_SMP ) && ( ( INCLUDE_xTaskGetCurrentTaskHandle == 1 ) || ( configUSE_MUTEXES == 1 ) ) )

    TaskHandle_t xTaskGetCurrentTaskHandleForCore( بنياد_Type_t xCoreID )
    {
        TaskHandle_t xReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

        #إذا ( CONFIG_FREERTOS_SMP )
        {
            xReturn = xTaskGetCurrentTaskHandleCPU( ( UBaseType_t ) xCoreID );
        }
        #وإلا /* CONFIG_FREERTOS_SMP */
        {
            /* A critical section is not required as this function does not
             * guarantee that the TCB will still be valid when this function
             * returns. */
            xReturn = pxCurrentTCBs[ xCoreID ];
        }
        #جيڪڏهن_پورو /* CONFIG_FREERTOS_SMP */

        أرجع xReturn;
    }

#جيڪڏهن_پورو /* ( ( !CONFIG_FREERTOS_SMP ) && ( ( INCLUDE_xTaskGetCurrentTaskHandle == 1 ) || ( configUSE_MUTEXES == 1 ) ) ) */
/*----------------------------------------------------------*/

#إذا ( !CONFIG_FREERTOS_SMP && ( config_GENERATE_لاڳيتو_وقت_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

    config_لاڳيتو_وقت_COUNTER_TYPE ul_Task_Get_Idle_لاڳيتو_وقت_Counter_For_Core( بنياد_Type_t xCoreID )
    {
        config_لاڳيتو_وقت_COUNTER_TYPE ul_لاڳيتو_وقت_Counter;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

        /* For SMP, we need to take the kernel lock here as we are about to
         * access kernel data structures. */
        prvENTER_CRITICAL_SMP_ONLY( &x_Kernel_تالو );
        {
            ul_لاڳيتو_وقت_Counter = xIdleTaskHandle[ xCoreID ]->ul_لاڳيتو_وقت_Counter;
        }
        /* Release the previously taken kernel lock. */
        prvEXIT_CRITICAL_SMP_ONLY( &x_Kernel_تالو );

        أرجع ul_لاڳيتو_وقت_Counter;
    }

#جيڪڏهن_پورو /* ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*----------------------------------------------------------*/

#إذا ( !CONFIG_FREERTOS_SMP && ( config_GENERATE_لاڳيتو_وقت_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

    config_لاڳيتو_وقت_COUNTER_TYPE ul_Task_Get_Idle_لاڳيتو_وقت_Percent_For_Core( بنياد_Type_t xCoreID )
    {
        config_لاڳيتو_وقت_COUNTER_TYPE ul_ڪل_وقت, ulReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

        #جيڪڏهن_وصف_ٿيل پورٽ_ALT_GET_لاڳيتو_وقت_COUNTER_قدر
            پورٽ_ALT_GET_لاڳيتو_وقت_COUNTER_قدر( ul_ڪل_وقت );
        #وإلا
            ul_ڪل_وقت = پورٽ_GET_لاڳيتو_وقت_COUNTER_قدر();
        #جيڪڏهن_پورو

        /* For percentage calculations. */
        ul_ڪل_وقت /= ( config_لاڳيتو_وقت_COUNTER_TYPE ) 100;

        /* Avoid divide by zero errors. */
        إذا( ul_ڪل_وقت > ( config_لاڳيتو_وقت_COUNTER_TYPE ) 0 )
        {
            /* For SMP, we need to take the kernel lock here as we are about
             * to access kernel data structures. */
            prvENTER_CRITICAL_SMP_ONLY( &x_Kernel_تالو );
            {
                ulReturn = xIdleTaskHandle[ xCoreID ]->ul_لاڳيتو_وقت_Counter / ul_ڪل_وقت;
            }
            /* Release the previously taken kernel lock. */
            prvEXIT_CRITICAL_SMP_ONLY( &x_Kernel_تالو );
        }
        وإلا
        {
            ulReturn = 0;
        }

        أرجع ulReturn;
    }

#جيڪڏهن_پورو /* ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*-----------------------------------------------------------*/

ڍير_Type_t * x_Task_Get_ڍير_شروعات( TaskHandle_t xTask )
{
    TCB_t * pxTCB;

    pxTCB = prvGetTCBFromHandle( xTask );
    أرجع pxTCB->px_ڍير;
}
/*----------------------------------------------------------*/

uint8_t * px_Task_Get_ڍير_شروعات( TaskHandle_t xTask )
{
    أرجع (uint8_t *)x_Task_Get_ڍير_شروعات( xTask );
}
/*----------------------------------------------------------*/

#إذا ( INCLUDE_v_Task_ترجيح_سيٽ == 1 )

    فراغ prv_Task_ترجيح_Raise( prv_Task_Saved_ترجيح_t * px_Saved_ترجيح,
                               UBaseType_t ux_New_ترجيح )
    {
        TCB_t * pxTCB;
        UBaseType_t ux_ترجيح_استعمال_ٿيل_On_داخلا;

        configASSERT( ( ux_New_ترجيح < configMAX_PRIORITIES ) );

        /* Ensure the new priority is valid. */
        إذا( ux_New_ترجيح >= ( UBaseType_t ) configMAX_PRIORITIES )
        {
            ux_New_ترجيح = ( UBaseType_t ) configMAX_PRIORITIES - ( UBaseType_t ) 1U;
        }

        #إذا CONFIG_FREERTOS_SMP
            taskENTER_CRITICAL();
        #وإلا
            taskENTER_CRITICAL( &x_Kernel_تالو );
        #جيڪڏهن_پورو
        {
            pxTCB = prvGetTCBFromHandle( عدم );

            #إذا ( configUSE_MUTEXES == 1 )
            {
                px_Saved_ترجيح->ux_ترجيح = pxTCB->ux_ترجيح;
                px_Saved_ترجيح->ux_بنياد_ترجيح = pxTCB->ux_بنياد_ترجيح;

                /* If uxNewPriority < uxBasePriority, then there is nothing else to
                 * do, as uxBasePriority is always <= uxPriority. */
                إذا( ux_New_ترجيح > pxTCB->ux_بنياد_ترجيح )
                {
                    pxTCB->ux_بنياد_ترجيح = ux_New_ترجيح;

                    /* Remember the task's current priority before attempting to
                     * change it. If the task's current priority is changed, it must
                     * be done so before moving the task between task lists) in order
                     * for the taskRESET_READY_PRIORITY() macro to function correctly. */
                    ux_ترجيح_استعمال_ٿيل_On_داخلا = pxTCB->ux_ترجيح;

                    إذا( ux_New_ترجيح > pxTCB->ux_ترجيح )
                    {
                        pxTCB->ux_ترجيح = ux_New_ترجيح;

                        /* Only reset the event list item value if the value is not
                         * being used for anything else. */
                        إذا( ( list_GET_LIST_شي_قدر( &( pxTCB->x_Event_List_شي ) ) & task_EVENT_LIST_شي_قدر_اندريون_USE ) == 0UL )
                        {
                            list_سيٽ_LIST_شي_قدر( &( pxTCB->x_Event_List_شي ), ( ( TickType_t ) configMAX_PRIORITIES - ( TickType_t ) ux_New_ترجيح ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */
                        }

                        /* If the task is in the blocked or suspended list we need do
                         * nothing more than change its priority variable. However, if
                         * the task is in a ready list it needs to be removed and placed
                         * in the list appropriate to its new priority. */
                        إذا( listIS_CONTAINED_WITHIN( &( pxReadyTasksLists[ ux_ترجيح_استعمال_ٿيل_On_داخلا ] ), &( pxTCB->x_State_List_شي ) ) != pdFALSE )
                        {
                            /* The task is currently in its ready list - remove before
                             * adding it to its new ready list.  As we are in a critical
                             * section we can do this even if the scheduler is suspended. */
                            إذا( uxListRemove( &( pxTCB->x_State_List_شي ) ) == ( UBaseType_t ) 0 )
                            {
                                /* It is known that the task is in its ready list so
                                 * there is no need to check again and the port level
                                 * reset macro can be called directly. */
                                پورٽ_RESET_READY_ترجيح( ux_ترجيح_استعمال_ٿيل_On_داخلا, ux_چوٽي_Ready_ترجيح );
                            }

                            prv_جوڙيو_Task_منزل_Ready_List( pxTCB );
                        }
                    }
                }
            }
            #وإلا /* if ( configUSE_MUTEXES == 1 ) */
            {
                px_Saved_ترجيح->ux_ترجيح = pxTCB->ux_ترجيح;

                إذا( ux_New_ترجيح > pxTCB->ux_ترجيح )
                {
                    v_Task_ترجيح_سيٽ( عدم, ux_New_ترجيح );
                }
            }
            #جيڪڏهن_پورو /* if ( configUSE_MUTEXES == 1 ) */
        }
        #إذا CONFIG_FREERTOS_SMP
            taskEXIT_CRITICAL();
        #وإلا
            taskEXIT_CRITICAL( &x_Kernel_تالو );
        #جيڪڏهن_پورو
    }

#جيڪڏهن_پورو /* INCLUDE_vTaskPrioritySet == 1 */
/*----------------------------------------------------------*/

#إذا ( INCLUDE_v_Task_ترجيح_سيٽ == 1 )

    فراغ prv_Task_ترجيح_Restore( prv_Task_Saved_ترجيح_t * px_Saved_ترجيح )
    {
        TCB_t * pxTCB;
        UBaseType_t ux_New_ترجيح;
        UBaseType_t ux_ترجيح_استعمال_ٿيل_On_داخلا;
        UBaseType_t ux_بنياد_ترجيح_استعمال_ٿيل_On_داخلا;
        بنياد_Type_t xYieldRequired = pdFALSE;

        #إذا CONFIG_FREERTOS_SMP
            taskENTER_CRITICAL();
        #وإلا
            taskENTER_CRITICAL( &x_Kernel_تالو );
        #جيڪڏهن_پورو
        {
            pxTCB = prvGetTCBFromHandle( عدم );

            #إذا ( configUSE_MUTEXES == 1 )
            {
                /* If the saved uxBasePriority == the task's uxBasePriority, it means
                 * that prvTaskPriorityRaise() never raised the task's uxBasePriority.
                 * In that case, there is nothing else to do. */
                إذا( px_Saved_ترجيح->ux_بنياد_ترجيح != pxTCB->ux_بنياد_ترجيح )
                {
                    ux_بنياد_ترجيح_استعمال_ٿيل_On_داخلا = pxTCB->ux_بنياد_ترجيح;
                    pxTCB->ux_بنياد_ترجيح = px_Saved_ترجيح->ux_بنياد_ترجيح;

                    /* Remember the task's current priority before attempting to
                     * change it. If the task's current priority is changed, it must
                     * be done so before moving the task between task lists in order
                     * for the taskRESET_READY_PRIORITY() macro to function correctly. */
                    ux_ترجيح_استعمال_ٿيل_On_داخلا = pxTCB->ux_ترجيح;

                    /* Check if the task inherited a priority after prvTaskPriorityRaise().
                     * If this is the case, there is nothing else to do. The priority
                     * will be restored when the task disinherits its priority. */
                    إذا( pxTCB->ux_ترجيح == ux_بنياد_ترجيح_استعمال_ٿيل_On_داخلا )
                    {
                        إذا( pxTCB->uxMutexesHeld == 0 )
                        {
                            /* The task may have inherited a priority before prvTaskPriorityRaise()
                             * then disinherited a priority after prvTaskPriorityRaise().
                             * Thus we need set the uxPriority to the saved base priority
                             * so that the task's priority gets restored to the priority
                             * before any inheritance or raising. */
                            pxTCB->ux_ترجيح = px_Saved_ترجيح->ux_بنياد_ترجيح;
                        }
                        وإلا
                        {
                            /* The task may have inherited a priority before prvTaskPriorityRaise()
                             * was called. Thus, we need to restore uxPriority to the
                             * "saved uxPriority" so that the task still retains that
                             * inherited priority. */
                            pxTCB->ux_ترجيح = px_Saved_ترجيح->ux_ترجيح;
                        }

                        ux_New_ترجيح = pxTCB->ux_ترجيح;

                        إذا( ux_New_ترجيح < ux_ترجيح_استعمال_ٿيل_On_داخلا )
                        {
                            /* Setting the priority of the running task down means
                             * there may now be another task of higher priority that
                             * is ready to execute. */
                            xYieldRequired = pdTRUE;
                        }

                        /* Only reset the event list item value if the value is not
                         * being used for anything else. */
                        إذا( ( list_GET_LIST_شي_قدر( &( pxTCB->x_Event_List_شي ) ) & task_EVENT_LIST_شي_قدر_اندريون_USE ) == 0UL )
                        {
                            list_سيٽ_LIST_شي_قدر( &( pxTCB->x_Event_List_شي ), ( ( TickType_t ) configMAX_PRIORITIES - ( TickType_t ) ux_New_ترجيح ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */
                        }

                        /* If the task is in the blocked or suspended list we need do
                         * nothing more than change its priority variable. However, if
                         * the task is in a ready list it needs to be removed and placed
                         * in the list appropriate to its new priority. */
                        إذا( listIS_CONTAINED_WITHIN( &( pxReadyTasksLists[ ux_ترجيح_استعمال_ٿيل_On_داخلا ] ), &( pxTCB->x_State_List_شي ) ) != pdFALSE )
                        {
                            /* The task is currently in its ready list - remove before
                             * adding it to its new ready list.  As we are in a critical
                             * section we can do this even if the scheduler is suspended. */
                            إذا( uxListRemove( &( pxTCB->x_State_List_شي ) ) == ( UBaseType_t ) 0 )
                            {
                                /* It is known that the task is in its ready list so
                                 * there is no need to check again and the port level
                                 * reset macro can be called directly. */
                                پورٽ_RESET_READY_ترجيح( ux_ترجيح_استعمال_ٿيل_On_داخلا, ux_چوٽي_Ready_ترجيح );
                            }

                            prv_جوڙيو_Task_منزل_Ready_List( pxTCB );
                        }

                        إذا( xYieldRequired != pdFALSE )
                        {
                            #إذا CONFIG_FREERTOS_SMP
                                taskYIELD_TASK_CORE_IF_USING_PREEMPTION( pxTCB );
                            #وإلا
                                taskYIELD_IF_USING_PREEMPTION();
                            #جيڪڏهن_پورو
                        }
                    }
                }
            }
            #وإلا /* if ( configUSE_MUTEXES == 1 ) */
            {
                v_Task_ترجيح_سيٽ( عدم, px_Saved_ترجيح->ux_ترجيح );
            }
            #جيڪڏهن_پورو /* if ( configUSE_MUTEXES == 1 ) */
        }
        #إذا CONFIG_FREERTOS_SMP
            taskEXIT_CRITICAL();
        #وإلا
            taskEXIT_CRITICAL( &x_Kernel_تالو );
        #جيڪڏهن_پورو
    }

#جيڪڏهن_پورو /* ( INCLUDE_vTaskPrioritySet == 1 ) */
/*----------------------------------------------------------*/

/* --------------------------------------------- TLSP Deletion Callbacks -------------------------------------------- */

#إذا CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS

    فراغ v_Task_سيٽ_Thread_Local_Storage_اشارو_And_Del_Callback( TaskHandle_t x_Task_منزل_سيٽ,
                                                          بنياد_Type_t x_اشاريو,
                                                          فراغ * pv_قدر,
                                                          TlsDeleteCallbackFunction_t pvDelCallback )
    {
        /* If TLSP deletion callbacks are enabled, then configNUM_THREAD_LOCAL_STORAGE_POINTERS
         * is doubled in size so that the latter half of the pvThreadLocalStoragePointers
         * stores the deletion callbacks. */
        إذا( x_اشاريو < ( config_انگ_THREAD_LOCAL_STORAGE_اشارا / 2 ) )
        {
            TCB_t * pxTCB;

            #إذا ( config_انگ_CORES > 1 )
            {
                /* For SMP, we need a critical section as another core could also
                 * update this task's TLSP at the same time. */
                #إذا CONFIG_FREERTOS_SMP
                {
                    taskENTER_CRITICAL();
                }
                #وإلا /* CONFIG_FREERTOS_SMP */
                {
                    taskENTER_CRITICAL( &x_Kernel_تالو );
                }
                #جيڪڏهن_پورو /* CONFIG_FREERTOS_SMP */
            }
            #جيڪڏهن_پورو /* configNUM_CORES > 1 */

            pxTCB = prvGetTCBFromHandle( x_Task_منزل_سيٽ );
            /* Store the TLSP by indexing the first half of the array */
            pxTCB->pv_Thread_Local_Storage_اشارا[ x_اشاريو ] = pv_قدر;

            /* Store the TLSP deletion callback by indexing the second half
             * of the array. */
            pxTCB->pv_Thread_Local_Storage_اشارا[ ( x_اشاريو + ( config_انگ_THREAD_LOCAL_STORAGE_اشارا / 2 ) ) ] = ( فراغ * ) pvDelCallback;

            #إذا ( config_انگ_CORES > 1 )
            {
                #إذا CONFIG_FREERTOS_SMP
                {
                    taskEXIT_CRITICAL();
                }
                #وإلا /* CONFIG_FREERTOS_SMP */
                {
                    taskEXIT_CRITICAL( &x_Kernel_تالو );
                }
                #جيڪڏهن_پورو /* CONFIG_FREERTOS_SMP */
            }
            #جيڪڏهن_پورو /* configNUM_CORES > 1 */
        }
    }

#جيڪڏهن_پورو /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */
/*----------------------------------------------------------*/

/* ----------------------------------------------------- Newlib ----------------------------------------------------- */

#إذا ( configUSE_NEWLIB_REENTRANT == 1 )

/**
 * @brief Get reentrancy structure of the current task
 *
 * - This function is required by newlib (when __DYNAMIC_REENT__ is enabled)
 * - It will return a pointer to the current task's reent struct
 * - If FreeRTOS is not running, it will return the global reent struct
 *
 * @return Pointer to a the (current taks's)/(global) reent struct
 */
    بنية _reent * __getreent( فراغ )
    {
        /* No lock needed because if this changes, we won't be running anymore. */
        TCB_t * pxCurTask = ( TCB_t * ) xTaskGetCurrentTaskHandle();
        بنية _reent * ret;

        إذا( pxCurTask == عدم )
        {
            /* No task running. Return global struct. */
            ret = _GLOBAL_REENT;
        }
        وإلا
        {
            /* We have a currently executing task. Return its reentrant struct. */
            ret = &pxCurTask->xTLSBlock;
        }

        أرجع ret;
    }

#جيڪڏهن_پورو /* configUSE_NEWLIB_REENTRANT == 1 */

/* -------------------------------------------------- Task Snapshot ------------------------------------------------- */

/**
 * @brief List of all task lists in FreeRTOS
 *
 * @note There are currently differing number of task list between SMP FreeRTOS and ESP-IDF FreeRTOS
 */
ساكن List_t * non_ready_task_lists[] =
{
    #جيڪڏهن_وصف_ٿيل CONFIG_FREERTOS_SMP
        &xPendingReadyList,
    #وإلا /* CONFIG_FREERTOS_SMP */
        &xPendingReadyList[ 0 ],
        #جيڪڏهن_وصف_نه_ٿيل CONFIG_FREERTOS_UNICORE
            &xPendingReadyList[ 1 ],
        #جيڪڏهن_پورو /* CONFIG_FREERTOS_UNICORE */
    #جيڪڏهن_پورو /* CONFIG_FREERTOS_SMP */
    &xDelayedTaskList1,
    &xDelayedTaskList2,
    #إذا ( INCLUDE_vTaskDelete == 1 )
        &x_Tasks_انتظار_Termination,
    #جيڪڏهن_پورو
    #إذا ( INCLUDE_vTaskSuspend == 1 )
        &xSuspendedTaskList,
    #جيڪڏهن_پورو
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
ساكن List_t * px_Get_Task_List_By_اشاريو( UBaseType_t ux_List_اشاريو )
{
    List_t * pxTaskList;
    ثابت size_t xNonReadyTaskListsCnt = ( حجم( non_ready_task_lists ) / حجم( List_t * ) );

    إذا( ux_List_اشاريو < configMAX_PRIORITIES )
    {
        pxTaskList = &pxReadyTasksLists[ configMAX_PRIORITIES - 1 - ux_List_اشاريو ];
    }
    وإلا إذا( ux_List_اشاريو < configMAX_PRIORITIES + xNonReadyTaskListsCnt )
    {
        pxTaskList = non_ready_task_lists[ ux_List_اشاريو - configMAX_PRIORITIES ];
    }
    وإلا
    {
        pxTaskList = عدم;
    }

    /* sanity check */
    إذا( pxTaskList )
    {
        إذا( !پورٽ_VALID_LIST_MEM( pxTaskList ) )
        {
            pxTaskList = عدم;
        }
    }

    أرجع pxTaskList;
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
ساكن مضمن UBaseType_t px_Get_Task_List_ڳڻپ( فراغ )
{
    أرجع configMAX_PRIORITIES + ( حجم( non_ready_task_lists ) / حجم( List_t * ) );
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
صحيح x_Task_Get_ايندڙ( TaskIterator_t * xIterator )
{
    إذا( !xIterator )
    {
        أرجع -1;
    }

    List_شي_t * px_ايندڙ_List_شي = xIterator->px_ايندڙ_List_شي;
    UBaseType_t uxCurListIdx = xIterator->ux_Current_List_اشاريو;
    UBaseType_t uxMaxListIdx = px_Get_Task_List_ڳڻپ();

    لكل( ; uxCurListIdx < uxMaxListIdx; ++uxCurListIdx )
    {
        List_t * pxCurrentTaskList = px_Get_Task_List_By_اشاريو( uxCurListIdx );

        إذا( !pxCurrentTaskList || ( list_CURRENT_LIST_ڊيگهائي( pxCurrentTaskList ) == 0 ) )
        {
            استمر;
        }

        ثابت List_شي_t * px_Curr_List_شي = list_GET_پڇاڙي_MARKER( pxCurrentTaskList );

        إذا( !px_ايندڙ_List_شي )
        {
            /* We are here if the traversal starts from the beginning or when we finish traversing
             * for one of the state lists
             */
            px_ايندڙ_List_شي = list_GET_ايندڙ( px_Curr_List_شي );
        }

        إذا( !پورٽ_VALID_LIST_MEM( px_ايندڙ_List_شي ) )
        {
            /* Nothing to do with the corrupted list item. We will skip to the next task state list.
             * pxNextListItem should be NULL at the beginning of each task list.
             */
            px_ايندڙ_List_شي = عدم;
            استمر;
        }

        TCB_t * pxTCB = ( TCB_t * ) list_GET_LIST_شي_OWNER( px_ايندڙ_List_شي );

        إذا( !پورٽ_VALID_TCB_MEM( pxTCB ) )
        {
            pxTCB = عدم;
        }

        xIterator->pxTaskHandle = pxTCB;
        xIterator->ux_Current_List_اشاريو = uxCurListIdx;

        إذا( px_Curr_List_شي->px_اڳيون == px_ايندڙ_List_شي )
        {
            /* If this is the last item of the current state list */
            xIterator->ux_Current_List_اشاريو++;
            xIterator->px_ايندڙ_List_شي = عدم;
        }
        وإلا
        {
            xIterator->px_ايندڙ_List_شي = list_GET_ايندڙ( px_ايندڙ_List_شي );
        }

        أرجع uxCurListIdx;
    }

    أرجع -1; /* end of the task list */
}
/*----------------------------------------------------------*/

بنياد_Type_t vTaskGetSnapshot( TaskHandle_t pxTask,
                             TaskSnapshot_t * pxTaskSnapshot )
{
    ESP_STATIC_ANALYZER_CHECK(!pxTask, pdFALSE);

    إذا( ( پورٽ_VALID_TCB_MEM( pxTask ) == false ) || ( pxTaskSnapshot == عدم ) )
    {
        أرجع pdFALSE;
    }

    TCB_t * pxTCB = ( TCB_t * ) pxTask;
    pxTaskSnapshot->pxTCB = pxTCB;
    pxTaskSnapshot->px_چوٽي_جو_ڍير = ( ڍير_Type_t * ) pxTCB->px_چوٽي_جو_ڍير;
    pxTaskSnapshot->px_پڇاڙي_جو_ڍير = ( ڍير_Type_t * ) pxTCB->px_پڇاڙي_جو_ڍير;
    أرجع pdTRUE;
}
/*----------------------------------------------------------*/

UBaseType_t uxTaskGetSnapshotAll( TaskSnapshot_t * ثابت pxTaskSnapshotArray,
                                  ثابت UBaseType_t ux_Array_ڊيگهائي,
                                  UBaseType_t * ثابت pxTCBSize )
{
    UBaseType_t ux_Array_انگ_Filled = 0;

    /* Traverse all of the tasks lists */
    TaskIterator_t xTaskIter = { 0 }; /* Point to the first task list */

    طالما( x_Task_Get_ايندڙ( &xTaskIter ) != -1 && ux_Array_انگ_Filled < ux_Array_ڊيگهائي )
    {
        vTaskGetSnapshot( xTaskIter.pxTaskHandle, &pxTaskSnapshotArray[ ux_Array_انگ_Filled ] );
        ux_Array_انگ_Filled++;
    }

    إذا( pxTCBSize != عدم )
    {
        *pxTCBSize = حجم( TCB_t );
    }

    أرجع ux_Array_انگ_Filled;
}
/*----------------------------------------------------------*/

/* ----------------------------------------------------- Misc ----------------------------------------------------- */

فراغ * pvTaskGetCurrentTCBForCore( بنياد_Type_t xCoreID )
{
    فراغ * pvRet;

    configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

    #إذا CONFIG_FREERTOS_SMP
        /* SMP FreeRTOS defines pxCurrentTCB as a macro function call */
        pvRet = ( فراغ * ) pxCurrentTCB;
    #وإلا /* CONFIG_FREERTOS_SMP */
        pvRet = ( فراغ * ) pxCurrentTCBs[ xCoreID ];
    #جيڪڏهن_پورو /* CONFIG_FREERTOS_SMP */
    أرجع pvRet;
}

/* ----------------------------------------------------- OpenOCD ---------------------------------------------------- */

#إذا CONFIG_FREERTOS_DEBUG_OCDAWARE

/**
 * Debug param indexes. DO NOT change the order. OpenOCD uses the same indexes
 * Entries in FreeRTOS_openocd_params must match the order of these indexes
 */
    تعداد
    {
        ESP_FREERTOS_DEBUG_جدول_سائيز = 0,
        ESP_FREERTOS_DEBUG_جدول_نسخو,
        ESP_FREERTOS_DEBUG_KERNEL_VER_MAJOR,
        ESP_FREERTOS_DEBUG_KERNEL_VER_MINOR,
        ESP_FREERTOS_DEBUG_KERNEL_VER_BUILD,
        ESP_FREERTOS_DEBUG_UX_چوٽي_استعمال_ٿيل_PIORITY,
        ESP_FREERTOS_DEBUG_PX_چوٽي_جو_ڍير,
        ESP_FREERTOS_DEBUG_PC_TASK_NAME,
        ESP_FREERTOS_DEBUG_LIST_سائيز,
        ESP_FREERTOS_DEBUG_LIST_انگ_شيون,
        ESP_FREERTOS_DEBUG_LIST_پڇاڙي,
        ESP_FREERTOS_DEBUG_LIST_پڇاڙي_PREV,
        ESP_FREERTOS_DEBUG_LIST_شي_PREV,
        ESP_FREERTOS_DEBUG_LIST_شي_OWNER,
        ESP_FREERTOS_DEBUG_TASK_ڳڻپ_ويڪر,
        ESP_FREERTOS_DEBUG_PTR_ويڪر,
        /* New entries must be inserted here */
        ESP_FREERTOS_DEBUG_جدول_پڇاڙي,
    };

    ثابت DRAM_ATTR uint8_t FreeRTOS_openocd_params[ ESP_FREERTOS_DEBUG_جدول_پڇاڙي ] =
    {
        ESP_FREERTOS_DEBUG_جدول_پڇاڙي, /* table size */
        1,                            /* table version */
        tsk_KERNEL_نسخو_MAJOR,
        tsk_KERNEL_نسخو_MINOR,
        tsk_KERNEL_نسخو_BUILD,
        configMAX_PRIORITIES - 1,                   /* uxTopUsedPriority */
        offsetof( TCB_t, px_چوٽي_جو_ڍير ),            /* thread_stack_offset; */
        offsetof( TCB_t, pcTaskName ),              /* thread_name_offset; */
        حجم( List_t ),                           /* list_width */
        offsetof( List_t, ux_عدد_جو_شيون ),        /* list_item_num */
        offsetof( List_t, x_List_پڇاڙي ),               /* list_end_offset */
        offsetof( List_t, x_List_پڇاڙي.px_اڳيون ),    /* list_next_offset */
        offsetof( List_شي_t, px_اڳيون ),         /* list_elem_next_offset */
        offsetof( List_شي_t, pvOwner ),            /* list_elem_content_offset */
        حجم( UBaseType_t ),                      /* task_count_width */
        حجم( فراغ * )                            /* ptr_width */
    };

#جيڪڏهن_پورو /* CONFIG_FREERTOS_DEBUG_OCDAWARE */
/*----------------------------------------------------------*/

/* ----------------------------------------------------- PSRAM ---------------------------------------------------- */

#إذا CONFIG_SPIRAM

    #إذا CONFIG_FREERTOS_SMP
        بنياد_Type_t prv_Task_Create_Dynamic_Affinity_سيٽ_With_Caps( TaskFunction_t px_Task_ڪوڊ,
                                                            ثابت حرف * ثابت pcName,
                                                            ثابت config_ڍير_اونهائي_TYPE us_ڍير_اونهائي,
                                                            فراغ * ثابت pvParameters,
                                                            UBaseType_t ux_ترجيح,
                                                            UBaseType_t ux_Core_Affinity_نقاب,
                                                            UBaseType_t ux_ڍير_يادگيري_Caps,
                                                            TaskHandle_t * ثابت pxCreatedTask )
    #وإلا /* CONFIG_FREERTOS_SMP */
        بنياد_Type_t prv_Task_Create_Dynamic_Pinned_منزل_Core_With_Caps( TaskFunction_t px_Task_ڪوڊ,
                                                             ثابت حرف * ثابت pcName,
                                                             ثابت config_ڍير_اونهائي_TYPE us_ڍير_اونهائي,
                                                             فراغ * ثابت pvParameters,
                                                             UBaseType_t ux_ترجيح,
                                                             ثابت بنياد_Type_t xCoreID,
                                                             UBaseType_t ux_ڍير_يادگيري_Caps,
                                                             TaskHandle_t * ثابت pxCreatedTask )
    #جيڪڏهن_پورو /* CONFIG_FREERTOS_SMP */
    {
        TCB_t * pxNewTCB;
        بنياد_Type_t xReturn;

        ڍير_Type_t * px_ڍير;

        configASSERT( ux_ڍير_يادگيري_Caps & ( MALLOC_CAP_8_ٻٽو_انگ ) );
        configASSERT( ( ux_ڍير_يادگيري_Caps & MALLOC_CAP_SPIRAM ) ||
                      ( ux_ڍير_يادگيري_Caps & MALLOC_CAP_اندروني ) );
        #إذا ( !CONFIG_FREERTOS_SMP )
        {
            configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );
        }
        #جيڪڏهن_پورو /* !CONFIG_FREERTOS_SMP */

        /* Allocate space for the stack used by the task being created. */
        px_ڍير = ڍير_caps_malloc( ( ( ( size_t ) us_ڍير_اونهائي ) * حجم( ڍير_Type_t ) ), ux_ڍير_يادگيري_Caps );

        إذا( px_ڍير != عدم )
        {
            /* Allocate space for the TCB. */
            pxNewTCB = ( TCB_t * ) pv_پورٽ_Malloc( حجم( TCB_t ) );

            إذا( pxNewTCB != عدم )
            {
                memset( ( فراغ * ) pxNewTCB, 0x00, حجم( TCB_t ) );

                /* Store the stack location in the TCB. */
                pxNewTCB->px_ڍير = px_ڍير;
            }
            وإلا
            {
                /* The stack cannot be used as the TCB has not been created. Free it. */
                ڍير_caps_free( px_ڍير );
            }
        }
        وإلا
        {
            pxNewTCB = عدم;
        }

        إذا( pxNewTCB != عدم )
        {
            #إذا ( tsk_STATIC_AND_DYNAMIC_ورهاست_POSSIBLE != 0 )
            {
                /* Tasks can be created statically or dynamically, so note this
                 * task was created dynamically in case it is later deleted. */
                pxNewTCB->ucStaticallyAllocated = tsk_DYNAMICALLY_ALLOCATED_ڍير_AND_TCB;
            }
            #جيڪڏهن_پورو /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

            #إذا CONFIG_FREERTOS_SMP
            {
                prvInitialiseNewTask( px_Task_ڪوڊ, pcName, ( uint32_t ) us_ڍير_اونهائي, pvParameters, ux_ترجيح, pxCreatedTask, pxNewTCB, عدم );
                #إذا ( ( config_انگ_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
                {
                    /* Set the task's affinity before scheduling it */
                    pxNewTCB->ux_Core_Affinity_نقاب = ux_Core_Affinity_نقاب;
                }
                #جيڪڏهن_پورو /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            }
            #وإلا /* CONFIG_FREERTOS_SMP */
            {
                prvInitialiseNewTask( px_Task_ڪوڊ, pcName, ( uint32_t ) us_ڍير_اونهائي, pvParameters, ux_ترجيح, pxCreatedTask, pxNewTCB, عدم, xCoreID );
            }
            #جيڪڏهن_پورو /* CONFIG_FREERTOS_SMP */

            prv_جوڙيو_New_Task_منزل_Ready_List( pxNewTCB );
            xReturn = pdPASS;
        }
        وإلا
        {
            xReturn = err_COULD_NOT_ALLOCATE_REQUIRED_يادگيري;
        }

        أرجع xReturn;
    }

#جيڪڏهن_پورو /* CONFIG_SPIRAM */
/*----------------------------------------------------------*/
