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
_Static_assert( offsetof( StaticTask_t, pxDummy6 ) == offsetof( TCB_t, px_ڈھیر ) );
_Static_assert( offsetof( StaticTask_t, pxDummy8 ) == offsetof( TCB_t, px_سرا_دا_ڈھیر ) );
#إذا !CONFIG_IDF_نشانہ_LINUX    // Disabled for linux builds due to differences in types
_Static_assert( tskNO_AFFINITY == ( ادھار_Type_t ) CONFIG_FREERTOS_NO_AFFINITY, "CONFIG_FREERTOS_NO_AFFINITY must be the same as tskNO_AFFINITY" );
#جے_انت

/* ------------------------------------------------- Kernel Control ------------------------------------------------- */

#إذا ( !CONFIG_FREERTOS_SMP && ( config_عدد_CORES > 1 ) )

/*
 * Wrapper function to take "xKerneLock"
 */
    فراغ prv_Take_Kernel_جندرا( فراغ )
    {
        /* We call the tasks.c critical section macro to take xKernelLock */
        taskENTER_CRITICAL( &x_Kernel_جندرا );
    }

#جے_انت /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#إذا ( !CONFIG_FREERTOS_SMP && ( config_عدد_CORES > 1 ) )

/*
 * Wrapper function to release "xKerneLock"
 */
    فراغ prv_Release_Kernel_جندرا( فراغ )
    {
        /* We call the tasks.c critical section macro to release xKernelLock */
        taskEXIT_CRITICAL( &x_Kernel_جندرا );
    }

#جے_انت /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#إذا ( CONFIG_FREERTOS_SMP && ( config_عدد_CORES > 1 ) )

/*
 * Workaround for non-thread safe multi-core OS startup (see IDF-4524)
 */
    فراغ prv_شروع_Scheduler_Other_Cores( فراغ )
    {
        /* This function is always called with interrupts disabled*/
        xSchedulerRunning = pdTRUE;
    }

#جے_انت /* ( CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#إذا ( !CONFIG_FREERTOS_SMP && ( config_عدد_CORES > 1 ) )

    ادھار_Type_t xTaskIncrementTickOtherCores( فراغ )
    {
        /* Minor optimization. This function can never switch cores mid
         * execution */
        ادھار_Type_t xCoreID = پورٹ_GET_CORE_ID();
        ادھار_Type_t xSwitchRequired = pdFALSE;

        /* This function should never be called by Core 0. */
        configASSERT( xCoreID != 0 );

        /* Called by the portable layer each time a tick interrupt occurs
         * on a core other than core 0. */
        traceTASK_INCREMENT_TICK( x_Tick_گنتی );

        إذا( uxSchedulerSuspended[ xCoreID ] == ( UBaseType_t ) 0U )
        {
            /* We need take the kernel lock here as we are about to access
             * kernel data structures. */
            taskENTER_CRITICAL_ISR( &x_Kernel_جندرا );

            /* Tasks of equal priority to the currently running task will share
             * processing time (time slice) if preemption is on, and the application
             * writer has not explicitly turned time slicing off. */
            #إذا ( ( configUSE_PREEMPTION == 1 ) && ( config_USE_ویلا_SLICING == 1 ) )
            {
                إذا( list_CURRENT_LIST_لمائی( &( pxReadyTasksLists[ pxCurrentTCBs[ xCoreID ]->ux_ترجیح ] ) ) > ( UBaseType_t ) 1 )
                {
                    xSwitchRequired = pdTRUE;
                }
                وإلا
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            #جے_انت /* ( ( configUSE_PREEMPTION == 1 ) && ( configUSE_TIME_SLICING == 1 ) ) */

            /* Release the previously taken kernel lock as we have finished
             * accessing the kernel data structures. */
            taskEXIT_CRITICAL_ISR( &x_Kernel_جندرا );

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
            #جے_انت /* configUSE_PREEMPTION */
        }

        #إذا ( configUSE_TICK_HOOK == 1 )
        {
            vApplicationTickHook();
        }
        #جے_انت

        أرجع xSwitchRequired;
    }

#جے_انت /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

/* -------------------------------------------------- Task Creation ------------------------------------------------- */

#إذا ( config_SUPPORT_DYNAMIC_ونڈ == 1 )

    ادھار_Type_t x_Task_Create_Pinned_منزل_Core( TaskFunction_t px_Task_کوڈ,
                                        ثابت حرف * ثابت pcName,
                                        ثابت uint32_t us_ڈھیر_ڈونگھائی,
                                        فراغ * ثابت pvParameters,
                                        UBaseType_t ux_ترجیح,
                                        TaskHandle_t * ثابت pxCreatedTask,
                                        ثابت ادھار_Type_t xCoreID )
    {
        ادھار_Type_t xReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );

        #إذا CONFIG_FREERTOS_SMP
        {
            /* If using Amazon SMP FreeRTOS. This function is just a wrapper around
             * xTaskCreate() or xTaskCreateAffinitySet(). */
            #إذا ( ( config_عدد_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
            {
                /* Convert xCoreID into an affinity mask */
                UBaseType_t ux_Core_Affinity_نقاب;

                /* Bit shifting << xCoreID is only valid if we have less than
                 * 32 cores. */
                ESP_STATIC_ASSERT( config_عدد_CORES < 32 );

                إذا( xCoreID == tskNO_AFFINITY )
                {
                    ux_Core_Affinity_نقاب = tskNO_AFFINITY;
                }
                وإلا
                {
                    ux_Core_Affinity_نقاب = ( 1 << xCoreID );
                }

                xReturn = x_Task_Create_Affinity_سیٹ( px_Task_کوڈ, pcName, us_ڈھیر_ڈونگھائی, pvParameters, ux_ترجیح, ux_Core_Affinity_نقاب, pxCreatedTask );
            }
            #وإلا /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            {
                xReturn = xTaskCreate( px_Task_کوڈ, pcName, us_ڈھیر_ڈونگھائی, pvParameters, ux_ترجیح, pxCreatedTask );
            }
            #جے_انت /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
        }
        #وإلا /* CONFIG_FREERTOS_SMP */
        {
            TCB_t * pxNewTCB;

            /* If the stack grows down then allocate the stack then the TCB so the
             * stack does not grow into the TCB.  Likewise if the stack grows up
             * then allocate the TCB then the stack. */
            #إذا ( پورٹ_ڈھیر_GROWTH > 0 )
            {
                /* Allocate space for the TCB.  Where the memory comes from depends on
                 * the implementation of the port malloc function and whether or not static
                 * allocation is being used. */
                pxNewTCB = ( TCB_t * ) pv_پورٹ_Malloc( حجم( TCB_t ) );

                إذا( pxNewTCB != عدم )
                {
                    memset( ( فراغ * ) pxNewTCB, 0x00, حجم( TCB_t ) );

                    /* Allocate space for the stack used by the task being created.
                     * The base of the stack memory stored in the TCB so the task can
                     * be deleted later if required. */
                    pxNewTCB->px_ڈھیر = ( ڈھیر_Type_t * ) pv_پورٹ_Malloc_ڈھیر( ( ( ( size_t ) us_ڈھیر_ڈونگھائی ) * حجم( ڈھیر_Type_t ) ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */

                    إذا( pxNewTCB->px_ڈھیر == عدم )
                    {
                        /* Could not allocate the stack.  Delete the allocated TCB. */
                        v_پورٹ_Free( pxNewTCB );
                        pxNewTCB = عدم;
                    }
                }
            }
            #وإلا /* portSTACK_GROWTH */
            {
                ڈھیر_Type_t * px_ڈھیر;

                /* Allocate space for the stack used by the task being created. */
                px_ڈھیر = pv_پورٹ_Malloc_ڈھیر( ( ( ( size_t ) us_ڈھیر_ڈونگھائی ) * حجم( ڈھیر_Type_t ) ) ); /*lint !e9079 All values returned by pvPortMalloc() have at least the alignment required by the MCU's stack and this allocation is the stack. */

                إذا( px_ڈھیر != عدم )
                {
                    /* Allocate space for the TCB. */
                    pxNewTCB = ( TCB_t * ) pv_پورٹ_Malloc( حجم( TCB_t ) ); /*lint !e9087 !e9079 All values returned by pvPortMalloc() have at least the alignment required by the MCU's stack, and the first member of TCB_t is always a pointer to the task's stack. */

                    إذا( pxNewTCB != عدم )
                    {
                        memset( ( فراغ * ) pxNewTCB, 0x00, حجم( TCB_t ) );

                        /* Store the stack location in the TCB. */
                        pxNewTCB->px_ڈھیر = px_ڈھیر;
                    }
                    وإلا
                    {
                        /* The stack cannot be used as the TCB was not created.  Free
                         * it again. */
                        v_پورٹ_Free_ڈھیر( px_ڈھیر );
                    }
                }
                وإلا
                {
                    pxNewTCB = عدم;
                }
            }
            #جے_انت /* portSTACK_GROWTH */

            إذا( pxNewTCB != عدم )
            {
                #إذا ( tsk_STATIC_AND_DYNAMIC_ونڈ_POSSIBLE != 0 ) /*lint !e9029 !e731 Macro has been consolidated for readability reasons. */
                {
                    /* Tasks can be created statically or dynamically, so note this
                     * task was created dynamically in case it is later deleted. */
                    pxNewTCB->ucStaticallyAllocated = tsk_DYNAMICALLY_ALLOCATED_ڈھیر_AND_TCB;
                }
                #جے_انت /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

                prvInitialiseNewTask( px_Task_کوڈ, pcName, ( uint32_t ) us_ڈھیر_ڈونگھائی, pvParameters, ux_ترجیح, pxCreatedTask, pxNewTCB, عدم, xCoreID );
                prv_جوڑو_New_Task_منزل_Ready_List( pxNewTCB );
                xReturn = pdPASS;
            }
            وإلا
            {
                xReturn = err_COULD_NOT_ALLOCATE_REQUIRED_حافظہ;
            }
        }
        #جے_انت /* CONFIG_FREERTOS_SMP */

        أرجع xReturn;
    }

#جے_انت /* ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#إذا ( config_SUPPORT_STATIC_ونڈ == 1 )

    TaskHandle_t x_Task_Create_Static_Pinned_منزل_Core( TaskFunction_t px_Task_کوڈ,
                                                ثابت حرف * ثابت pcName,
                                                ثابت uint32_t ul_ڈھیر_ڈونگھائی,
                                                فراغ * ثابت pvParameters,
                                                UBaseType_t ux_ترجیح,
                                                ڈھیر_Type_t * ثابت pux_ڈھیر_بفر,
                                                StaticTask_t * ثابت px_Task_بفر,
                                                ثابت ادھار_Type_t xCoreID )
    {
        TaskHandle_t xReturn;

        configASSERT( پورٹ_VALID_ڈھیر_MEM( pux_ڈھیر_بفر ) );
        configASSERT( پورٹ_VALID_TCB_MEM( px_Task_بفر ) );
        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );

        #إذا CONFIG_FREERTOS_SMP
        {
            /* If using Amazon SMP FreeRTOS. This function is just a wrapper around
             * xTaskCreateStatic() or xTaskCreateStaticAffinitySet(). */
            #إذا ( ( config_عدد_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
            {
                /* Convert xCoreID into an affinity mask */
                UBaseType_t ux_Core_Affinity_نقاب;

                /* Bit shifting << xCoreID is only valid if we have less than
                 * 32 cores. */
                ESP_STATIC_ASSERT( config_عدد_CORES < 32 );

                إذا( xCoreID == tskNO_AFFINITY )
                {
                    ux_Core_Affinity_نقاب = tskNO_AFFINITY;
                }
                وإلا
                {
                    ux_Core_Affinity_نقاب = ( 1 << xCoreID );
                }

                xReturn = x_Task_Create_Static_Affinity_سیٹ( px_Task_کوڈ, pcName, ul_ڈھیر_ڈونگھائی, pvParameters, ux_ترجیح, pux_ڈھیر_بفر, px_Task_بفر, ux_Core_Affinity_نقاب );
            }
            #وإلا /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            {
                xReturn = xTaskCreateStatic( px_Task_کوڈ, pcName, ul_ڈھیر_ڈونگھائی, pvParameters, ux_ترجیح, pux_ڈھیر_بفر, px_Task_بفر );
            }
            #جے_انت /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
        }
        #وإلا /* CONFIG_FREERTOS_SMP */
        {
            TCB_t * pxNewTCB;

            #إذا ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticTask_t equals the size of the real task
                 * structure. */
                متطاير size_t x_ناپ = حجم( StaticTask_t );
                configASSERT( x_ناپ == حجم( TCB_t ) );
                ( فراغ ) x_ناپ; /* Prevent lint warning when configASSERT() is not used. */
            }
            #جے_انت /* configASSERT_DEFINED */

            إذا( ( px_Task_بفر != عدم ) && ( pux_ڈھیر_بفر != عدم ) )
            {
                /* The memory used for the task's TCB and stack are passed into this
                 * function - use them. */
                pxNewTCB = ( TCB_t * ) px_Task_بفر; /*lint !e740 !e9087 Unusual cast is ok as the structures are designed to have the same alignment, and the size is checked by an assert. */
                memset( ( فراغ * ) pxNewTCB, 0x00, حجم( TCB_t ) );
                pxNewTCB->px_ڈھیر = ( ڈھیر_Type_t * ) pux_ڈھیر_بفر;

                #إذا ( tsk_STATIC_AND_DYNAMIC_ونڈ_POSSIBLE != 0 ) /*lint !e731 !e9029 Macro has been consolidated for readability reasons. */
                {
                    /* Tasks can be created statically or dynamically, so note this
                     * task was created statically in case the task is later deleted. */
                    pxNewTCB->ucStaticallyAllocated = tsk_STATICALLY_ALLOCATED_ڈھیر_AND_TCB;
                }
                #جے_انت /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

                prvInitialiseNewTask( px_Task_کوڈ, pcName, ul_ڈھیر_ڈونگھائی, pvParameters, ux_ترجیح, &xReturn, pxNewTCB, عدم, xCoreID );
                prv_جوڑو_New_Task_منزل_Ready_List( pxNewTCB );
            }
            وإلا
            {
                xReturn = عدم;
            }
        }
        #جے_انت /* CONFIG_FREERTOS_SMP */

        أرجع xReturn;
    }

#جے_انت /* ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
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
    ادھار_Type_t __attribute__( ( weak ) ) xTimerCreateTimerTask( فراغ )
    {
        أرجع pdPASS;
    }

#جے_انت /* configUSE_TIMERS */
/*----------------------------------------------------------*/

/* ------------------------------------------------- Task Utilities ------------------------------------------------- */

ادھار_Type_t xTaskGetCoreID( TaskHandle_t xTask )
{
    ادھار_Type_t xReturn;

    #إذا ( config_عدد_CORES > 1 )
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
                صحيح i_اشاریہ_Plus_One = __builtin_ffs( ux_Core_Affinity_نقاب );
                أكد( i_اشاریہ_Plus_One >= 1 );
                xReturn = i_اشاریہ_Plus_One - 1;
            }
        #وإلا /* CONFIG_FREERTOS_SMP */
            TCB_t * pxTCB;

            pxTCB = prvGetTCBFromHandle( xTask );

            xReturn = pxTCB->xCoreID;
        #جے_انت /* CONFIG_FREERTOS_SMP */
    }
    #وإلا /* configNUM_CORES > 1 */
    {
        /* Single-core. Just return a core ID of 0 */
        xReturn = 0;
    }
    #جے_انت /* configNUM_CORES > 1 */

    أرجع xReturn;
}
/*----------------------------------------------------------*/

#إذا ( ( !CONFIG_FREERTOS_SMP ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

    TaskHandle_t xTaskGetIdleTaskHandleForCore( ادھار_Type_t xCoreID )
    {
        /* If xTaskGetIdleTaskHandle() is called before the scheduler has been
         * started, then xIdleTaskHandle will be NULL. */
        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );
        configASSERT( ( xIdleTaskHandle[ xCoreID ] != عدم ) );
        أرجع xIdleTaskHandle[ xCoreID ];
    }

#جے_انت /* ( ( !CONFIG_FREERTOS_SMP ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*----------------------------------------------------------*/

#إذا ( ( !CONFIG_FREERTOS_SMP ) && ( ( INCLUDE_xTaskGetCurrentTaskHandle == 1 ) || ( configUSE_MUTEXES == 1 ) ) )

    TaskHandle_t xTaskGetCurrentTaskHandleForCore( ادھار_Type_t xCoreID )
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
        #جے_انت /* CONFIG_FREERTOS_SMP */

        أرجع xReturn;
    }

#جے_انت /* ( ( !CONFIG_FREERTOS_SMP ) && ( ( INCLUDE_xTaskGetCurrentTaskHandle == 1 ) || ( configUSE_MUTEXES == 1 ) ) ) */
/*----------------------------------------------------------*/

#إذا ( !CONFIG_FREERTOS_SMP && ( config_GENERATE_لگاتار_ویلا_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

    config_لگاتار_ویلا_COUNTER_TYPE ul_Task_Get_Idle_لگاتار_ویلا_Counter_For_Core( ادھار_Type_t xCoreID )
    {
        config_لگاتار_ویلا_COUNTER_TYPE ul_لگاتار_ویلا_Counter;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

        /* For SMP, we need to take the kernel lock here as we are about to
         * access kernel data structures. */
        prvENTER_CRITICAL_SMP_ONLY( &x_Kernel_جندرا );
        {
            ul_لگاتار_ویلا_Counter = xIdleTaskHandle[ xCoreID ]->ul_لگاتار_ویلا_Counter;
        }
        /* Release the previously taken kernel lock. */
        prvEXIT_CRITICAL_SMP_ONLY( &x_Kernel_جندرا );

        أرجع ul_لگاتار_ویلا_Counter;
    }

#جے_انت /* ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*----------------------------------------------------------*/

#إذا ( !CONFIG_FREERTOS_SMP && ( config_GENERATE_لگاتار_ویلا_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

    config_لگاتار_ویلا_COUNTER_TYPE ul_Task_Get_Idle_لگاتار_ویلا_Percent_For_Core( ادھار_Type_t xCoreID )
    {
        config_لگاتار_ویلا_COUNTER_TYPE ul_کل_ویلا, ulReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

        #جے_تعریف_شدہ پورٹ_ALT_GET_لگاتار_ویلا_COUNTER_قدر
            پورٹ_ALT_GET_لگاتار_ویلا_COUNTER_قدر( ul_کل_ویلا );
        #وإلا
            ul_کل_ویلا = پورٹ_GET_لگاتار_ویلا_COUNTER_قدر();
        #جے_انت

        /* For percentage calculations. */
        ul_کل_ویلا /= ( config_لگاتار_ویلا_COUNTER_TYPE ) 100;

        /* Avoid divide by zero errors. */
        إذا( ul_کل_ویلا > ( config_لگاتار_ویلا_COUNTER_TYPE ) 0 )
        {
            /* For SMP, we need to take the kernel lock here as we are about
             * to access kernel data structures. */
            prvENTER_CRITICAL_SMP_ONLY( &x_Kernel_جندرا );
            {
                ulReturn = xIdleTaskHandle[ xCoreID ]->ul_لگاتار_ویلا_Counter / ul_کل_ویلا;
            }
            /* Release the previously taken kernel lock. */
            prvEXIT_CRITICAL_SMP_ONLY( &x_Kernel_جندرا );
        }
        وإلا
        {
            ulReturn = 0;
        }

        أرجع ulReturn;
    }

#جے_انت /* ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*-----------------------------------------------------------*/

ڈھیر_Type_t * x_Task_Get_ڈھیر_شروع( TaskHandle_t xTask )
{
    TCB_t * pxTCB;

    pxTCB = prvGetTCBFromHandle( xTask );
    أرجع pxTCB->px_ڈھیر;
}
/*----------------------------------------------------------*/

uint8_t * px_Task_Get_ڈھیر_شروع( TaskHandle_t xTask )
{
    أرجع (uint8_t *)x_Task_Get_ڈھیر_شروع( xTask );
}
/*----------------------------------------------------------*/

#إذا ( INCLUDE_v_Task_ترجیح_سیٹ == 1 )

    فراغ prv_Task_ترجیح_Raise( prv_Task_Saved_ترجیح_t * px_Saved_ترجیح,
                               UBaseType_t ux_New_ترجیح )
    {
        TCB_t * pxTCB;
        UBaseType_t ux_ترجیح_ورتیا_On_اندراج;

        configASSERT( ( ux_New_ترجیح < configMAX_PRIORITIES ) );

        /* Ensure the new priority is valid. */
        إذا( ux_New_ترجیح >= ( UBaseType_t ) configMAX_PRIORITIES )
        {
            ux_New_ترجیح = ( UBaseType_t ) configMAX_PRIORITIES - ( UBaseType_t ) 1U;
        }

        #إذا CONFIG_FREERTOS_SMP
            taskENTER_CRITICAL();
        #وإلا
            taskENTER_CRITICAL( &x_Kernel_جندرا );
        #جے_انت
        {
            pxTCB = prvGetTCBFromHandle( عدم );

            #إذا ( configUSE_MUTEXES == 1 )
            {
                px_Saved_ترجیح->ux_ترجیح = pxTCB->ux_ترجیح;
                px_Saved_ترجیح->ux_ادھار_ترجیح = pxTCB->ux_ادھار_ترجیح;

                /* If uxNewPriority < uxBasePriority, then there is nothing else to
                 * do, as uxBasePriority is always <= uxPriority. */
                إذا( ux_New_ترجیح > pxTCB->ux_ادھار_ترجیح )
                {
                    pxTCB->ux_ادھار_ترجیح = ux_New_ترجیح;

                    /* Remember the task's current priority before attempting to
                     * change it. If the task's current priority is changed, it must
                     * be done so before moving the task between task lists) in order
                     * for the taskRESET_READY_PRIORITY() macro to function correctly. */
                    ux_ترجیح_ورتیا_On_اندراج = pxTCB->ux_ترجیح;

                    إذا( ux_New_ترجیح > pxTCB->ux_ترجیح )
                    {
                        pxTCB->ux_ترجیح = ux_New_ترجیح;

                        /* Only reset the event list item value if the value is not
                         * being used for anything else. */
                        إذا( ( list_GET_LIST_شے_قدر( &( pxTCB->x_Event_List_شے ) ) & task_EVENT_LIST_شے_قدر_وچ_USE ) == 0UL )
                        {
                            list_سیٹ_LIST_شے_قدر( &( pxTCB->x_Event_List_شے ), ( ( TickType_t ) configMAX_PRIORITIES - ( TickType_t ) ux_New_ترجیح ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */
                        }

                        /* If the task is in the blocked or suspended list we need do
                         * nothing more than change its priority variable. However, if
                         * the task is in a ready list it needs to be removed and placed
                         * in the list appropriate to its new priority. */
                        إذا( listIS_CONTAINED_WITHIN( &( pxReadyTasksLists[ ux_ترجیح_ورتیا_On_اندراج ] ), &( pxTCB->x_State_List_شے ) ) != pdFALSE )
                        {
                            /* The task is currently in its ready list - remove before
                             * adding it to its new ready list.  As we are in a critical
                             * section we can do this even if the scheduler is suspended. */
                            إذا( uxListRemove( &( pxTCB->x_State_List_شے ) ) == ( UBaseType_t ) 0 )
                            {
                                /* It is known that the task is in its ready list so
                                 * there is no need to check again and the port level
                                 * reset macro can be called directly. */
                                پورٹ_RESET_READY_ترجیح( ux_ترجیح_ورتیا_On_اندراج, ux_چوٹی_Ready_ترجیح );
                            }

                            prv_جوڑو_Task_منزل_Ready_List( pxTCB );
                        }
                    }
                }
            }
            #وإلا /* if ( configUSE_MUTEXES == 1 ) */
            {
                px_Saved_ترجیح->ux_ترجیح = pxTCB->ux_ترجیح;

                إذا( ux_New_ترجیح > pxTCB->ux_ترجیح )
                {
                    v_Task_ترجیح_سیٹ( عدم, ux_New_ترجیح );
                }
            }
            #جے_انت /* if ( configUSE_MUTEXES == 1 ) */
        }
        #إذا CONFIG_FREERTOS_SMP
            taskEXIT_CRITICAL();
        #وإلا
            taskEXIT_CRITICAL( &x_Kernel_جندرا );
        #جے_انت
    }

#جے_انت /* INCLUDE_vTaskPrioritySet == 1 */
/*----------------------------------------------------------*/

#إذا ( INCLUDE_v_Task_ترجیح_سیٹ == 1 )

    فراغ prv_Task_ترجیح_Restore( prv_Task_Saved_ترجیح_t * px_Saved_ترجیح )
    {
        TCB_t * pxTCB;
        UBaseType_t ux_New_ترجیح;
        UBaseType_t ux_ترجیح_ورتیا_On_اندراج;
        UBaseType_t ux_ادھار_ترجیح_ورتیا_On_اندراج;
        ادھار_Type_t xYieldRequired = pdFALSE;

        #إذا CONFIG_FREERTOS_SMP
            taskENTER_CRITICAL();
        #وإلا
            taskENTER_CRITICAL( &x_Kernel_جندرا );
        #جے_انت
        {
            pxTCB = prvGetTCBFromHandle( عدم );

            #إذا ( configUSE_MUTEXES == 1 )
            {
                /* If the saved uxBasePriority == the task's uxBasePriority, it means
                 * that prvTaskPriorityRaise() never raised the task's uxBasePriority.
                 * In that case, there is nothing else to do. */
                إذا( px_Saved_ترجیح->ux_ادھار_ترجیح != pxTCB->ux_ادھار_ترجیح )
                {
                    ux_ادھار_ترجیح_ورتیا_On_اندراج = pxTCB->ux_ادھار_ترجیح;
                    pxTCB->ux_ادھار_ترجیح = px_Saved_ترجیح->ux_ادھار_ترجیح;

                    /* Remember the task's current priority before attempting to
                     * change it. If the task's current priority is changed, it must
                     * be done so before moving the task between task lists in order
                     * for the taskRESET_READY_PRIORITY() macro to function correctly. */
                    ux_ترجیح_ورتیا_On_اندراج = pxTCB->ux_ترجیح;

                    /* Check if the task inherited a priority after prvTaskPriorityRaise().
                     * If this is the case, there is nothing else to do. The priority
                     * will be restored when the task disinherits its priority. */
                    إذا( pxTCB->ux_ترجیح == ux_ادھار_ترجیح_ورتیا_On_اندراج )
                    {
                        إذا( pxTCB->uxMutexesHeld == 0 )
                        {
                            /* The task may have inherited a priority before prvTaskPriorityRaise()
                             * then disinherited a priority after prvTaskPriorityRaise().
                             * Thus we need set the uxPriority to the saved base priority
                             * so that the task's priority gets restored to the priority
                             * before any inheritance or raising. */
                            pxTCB->ux_ترجیح = px_Saved_ترجیح->ux_ادھار_ترجیح;
                        }
                        وإلا
                        {
                            /* The task may have inherited a priority before prvTaskPriorityRaise()
                             * was called. Thus, we need to restore uxPriority to the
                             * "saved uxPriority" so that the task still retains that
                             * inherited priority. */
                            pxTCB->ux_ترجیح = px_Saved_ترجیح->ux_ترجیح;
                        }

                        ux_New_ترجیح = pxTCB->ux_ترجیح;

                        إذا( ux_New_ترجیح < ux_ترجیح_ورتیا_On_اندراج )
                        {
                            /* Setting the priority of the running task down means
                             * there may now be another task of higher priority that
                             * is ready to execute. */
                            xYieldRequired = pdTRUE;
                        }

                        /* Only reset the event list item value if the value is not
                         * being used for anything else. */
                        إذا( ( list_GET_LIST_شے_قدر( &( pxTCB->x_Event_List_شے ) ) & task_EVENT_LIST_شے_قدر_وچ_USE ) == 0UL )
                        {
                            list_سیٹ_LIST_شے_قدر( &( pxTCB->x_Event_List_شے ), ( ( TickType_t ) configMAX_PRIORITIES - ( TickType_t ) ux_New_ترجیح ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */
                        }

                        /* If the task is in the blocked or suspended list we need do
                         * nothing more than change its priority variable. However, if
                         * the task is in a ready list it needs to be removed and placed
                         * in the list appropriate to its new priority. */
                        إذا( listIS_CONTAINED_WITHIN( &( pxReadyTasksLists[ ux_ترجیح_ورتیا_On_اندراج ] ), &( pxTCB->x_State_List_شے ) ) != pdFALSE )
                        {
                            /* The task is currently in its ready list - remove before
                             * adding it to its new ready list.  As we are in a critical
                             * section we can do this even if the scheduler is suspended. */
                            إذا( uxListRemove( &( pxTCB->x_State_List_شے ) ) == ( UBaseType_t ) 0 )
                            {
                                /* It is known that the task is in its ready list so
                                 * there is no need to check again and the port level
                                 * reset macro can be called directly. */
                                پورٹ_RESET_READY_ترجیح( ux_ترجیح_ورتیا_On_اندراج, ux_چوٹی_Ready_ترجیح );
                            }

                            prv_جوڑو_Task_منزل_Ready_List( pxTCB );
                        }

                        إذا( xYieldRequired != pdFALSE )
                        {
                            #إذا CONFIG_FREERTOS_SMP
                                taskYIELD_TASK_CORE_IF_USING_PREEMPTION( pxTCB );
                            #وإلا
                                taskYIELD_IF_USING_PREEMPTION();
                            #جے_انت
                        }
                    }
                }
            }
            #وإلا /* if ( configUSE_MUTEXES == 1 ) */
            {
                v_Task_ترجیح_سیٹ( عدم, px_Saved_ترجیح->ux_ترجیح );
            }
            #جے_انت /* if ( configUSE_MUTEXES == 1 ) */
        }
        #إذا CONFIG_FREERTOS_SMP
            taskEXIT_CRITICAL();
        #وإلا
            taskEXIT_CRITICAL( &x_Kernel_جندرا );
        #جے_انت
    }

#جے_انت /* ( INCLUDE_vTaskPrioritySet == 1 ) */
/*----------------------------------------------------------*/

/* --------------------------------------------- TLSP Deletion Callbacks -------------------------------------------- */

#إذا CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS

    فراغ v_Task_سیٹ_Thread_Local_Storage_اشارہ_گر_And_Del_Callback( TaskHandle_t x_Task_منزل_سیٹ,
                                                          ادھار_Type_t x_اشاریہ,
                                                          فراغ * pv_قدر,
                                                          TlsDeleteCallbackFunction_t pvDelCallback )
    {
        /* If TLSP deletion callbacks are enabled, then configNUM_THREAD_LOCAL_STORAGE_POINTERS
         * is doubled in size so that the latter half of the pvThreadLocalStoragePointers
         * stores the deletion callbacks. */
        إذا( x_اشاریہ < ( config_عدد_THREAD_LOCAL_STORAGE_سوچکاں / 2 ) )
        {
            TCB_t * pxTCB;

            #إذا ( config_عدد_CORES > 1 )
            {
                /* For SMP, we need a critical section as another core could also
                 * update this task's TLSP at the same time. */
                #إذا CONFIG_FREERTOS_SMP
                {
                    taskENTER_CRITICAL();
                }
                #وإلا /* CONFIG_FREERTOS_SMP */
                {
                    taskENTER_CRITICAL( &x_Kernel_جندرا );
                }
                #جے_انت /* CONFIG_FREERTOS_SMP */
            }
            #جے_انت /* configNUM_CORES > 1 */

            pxTCB = prvGetTCBFromHandle( x_Task_منزل_سیٹ );
            /* Store the TLSP by indexing the first half of the array */
            pxTCB->pv_Thread_Local_Storage_سوچکاں[ x_اشاریہ ] = pv_قدر;

            /* Store the TLSP deletion callback by indexing the second half
             * of the array. */
            pxTCB->pv_Thread_Local_Storage_سوچکاں[ ( x_اشاریہ + ( config_عدد_THREAD_LOCAL_STORAGE_سوچکاں / 2 ) ) ] = ( فراغ * ) pvDelCallback;

            #إذا ( config_عدد_CORES > 1 )
            {
                #إذا CONFIG_FREERTOS_SMP
                {
                    taskEXIT_CRITICAL();
                }
                #وإلا /* CONFIG_FREERTOS_SMP */
                {
                    taskEXIT_CRITICAL( &x_Kernel_جندرا );
                }
                #جے_انت /* CONFIG_FREERTOS_SMP */
            }
            #جے_انت /* configNUM_CORES > 1 */
        }
    }

#جے_انت /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */
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

#جے_انت /* configUSE_NEWLIB_REENTRANT == 1 */

/* -------------------------------------------------- Task Snapshot ------------------------------------------------- */

/**
 * @brief List of all task lists in FreeRTOS
 *
 * @note There are currently differing number of task list between SMP FreeRTOS and ESP-IDF FreeRTOS
 */
ساكن List_t * non_ready_task_lists[] =
{
    #جے_تعریف_شدہ CONFIG_FREERTOS_SMP
        &xPendingReadyList,
    #وإلا /* CONFIG_FREERTOS_SMP */
        &xPendingReadyList[ 0 ],
        #جے_بنا_تعریف CONFIG_FREERTOS_UNICORE
            &xPendingReadyList[ 1 ],
        #جے_انت /* CONFIG_FREERTOS_UNICORE */
    #جے_انت /* CONFIG_FREERTOS_SMP */
    &xDelayedTaskList1,
    &xDelayedTaskList2,
    #إذا ( INCLUDE_vTaskDelete == 1 )
        &x_Tasks_اڈیک_Termination,
    #جے_انت
    #إذا ( INCLUDE_vTaskSuspend == 1 )
        &xSuspendedTaskList,
    #جے_انت
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
ساكن List_t * px_Get_Task_List_By_اشاریہ( UBaseType_t ux_List_اشاریہ )
{
    List_t * pxTaskList;
    ثابت size_t xNonReadyTaskListsCnt = ( حجم( non_ready_task_lists ) / حجم( List_t * ) );

    إذا( ux_List_اشاریہ < configMAX_PRIORITIES )
    {
        pxTaskList = &pxReadyTasksLists[ configMAX_PRIORITIES - 1 - ux_List_اشاریہ ];
    }
    وإلا إذا( ux_List_اشاریہ < configMAX_PRIORITIES + xNonReadyTaskListsCnt )
    {
        pxTaskList = non_ready_task_lists[ ux_List_اشاریہ - configMAX_PRIORITIES ];
    }
    وإلا
    {
        pxTaskList = عدم;
    }

    /* sanity check */
    إذا( pxTaskList )
    {
        إذا( !پورٹ_VALID_LIST_MEM( pxTaskList ) )
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
ساكن مضمن UBaseType_t px_Get_Task_List_گنتی( فراغ )
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
صحيح x_Task_Get_اگلا( TaskIterator_t * xIterator )
{
    إذا( !xIterator )
    {
        أرجع -1;
    }

    List_شے_t * px_اگلا_List_شے = xIterator->px_اگلا_List_شے;
    UBaseType_t uxCurListIdx = xIterator->ux_Current_List_اشاریہ;
    UBaseType_t uxMaxListIdx = px_Get_Task_List_گنتی();

    لكل( ; uxCurListIdx < uxMaxListIdx; ++uxCurListIdx )
    {
        List_t * pxCurrentTaskList = px_Get_Task_List_By_اشاریہ( uxCurListIdx );

        إذا( !pxCurrentTaskList || ( list_CURRENT_LIST_لمائی( pxCurrentTaskList ) == 0 ) )
        {
            استمر;
        }

        ثابت List_شے_t * px_Curr_List_شے = list_GET_سرا_MARKER( pxCurrentTaskList );

        إذا( !px_اگلا_List_شے )
        {
            /* We are here if the traversal starts from the beginning or when we finish traversing
             * for one of the state lists
             */
            px_اگلا_List_شے = list_GET_اگلا( px_Curr_List_شے );
        }

        إذا( !پورٹ_VALID_LIST_MEM( px_اگلا_List_شے ) )
        {
            /* Nothing to do with the corrupted list item. We will skip to the next task state list.
             * pxNextListItem should be NULL at the beginning of each task list.
             */
            px_اگلا_List_شے = عدم;
            استمر;
        }

        TCB_t * pxTCB = ( TCB_t * ) list_GET_LIST_شے_OWNER( px_اگلا_List_شے );

        إذا( !پورٹ_VALID_TCB_MEM( pxTCB ) )
        {
            pxTCB = عدم;
        }

        xIterator->pxTaskHandle = pxTCB;
        xIterator->ux_Current_List_اشاریہ = uxCurListIdx;

        إذا( px_Curr_List_شے->px_پچھلا == px_اگلا_List_شے )
        {
            /* If this is the last item of the current state list */
            xIterator->ux_Current_List_اشاریہ++;
            xIterator->px_اگلا_List_شے = عدم;
        }
        وإلا
        {
            xIterator->px_اگلا_List_شے = list_GET_اگلا( px_اگلا_List_شے );
        }

        أرجع uxCurListIdx;
    }

    أرجع -1; /* end of the task list */
}
/*----------------------------------------------------------*/

ادھار_Type_t vTaskGetSnapshot( TaskHandle_t pxTask,
                             TaskSnapshot_t * pxTaskSnapshot )
{
    ESP_STATIC_ANALYZER_CHECK(!pxTask, pdFALSE);

    إذا( ( پورٹ_VALID_TCB_MEM( pxTask ) == false ) || ( pxTaskSnapshot == عدم ) )
    {
        أرجع pdFALSE;
    }

    TCB_t * pxTCB = ( TCB_t * ) pxTask;
    pxTaskSnapshot->pxTCB = pxTCB;
    pxTaskSnapshot->px_چوٹی_دا_ڈھیر = ( ڈھیر_Type_t * ) pxTCB->px_چوٹی_دا_ڈھیر;
    pxTaskSnapshot->px_سرا_دا_ڈھیر = ( ڈھیر_Type_t * ) pxTCB->px_سرا_دا_ڈھیر;
    أرجع pdTRUE;
}
/*----------------------------------------------------------*/

UBaseType_t uxTaskGetSnapshotAll( TaskSnapshot_t * ثابت pxTaskSnapshotArray,
                                  ثابت UBaseType_t ux_Array_لمائی,
                                  UBaseType_t * ثابت pxTCBSize )
{
    UBaseType_t ux_Array_عدد_Filled = 0;

    /* Traverse all of the tasks lists */
    TaskIterator_t xTaskIter = { 0 }; /* Point to the first task list */

    طالما( x_Task_Get_اگلا( &xTaskIter ) != -1 && ux_Array_عدد_Filled < ux_Array_لمائی )
    {
        vTaskGetSnapshot( xTaskIter.pxTaskHandle, &pxTaskSnapshotArray[ ux_Array_عدد_Filled ] );
        ux_Array_عدد_Filled++;
    }

    إذا( pxTCBSize != عدم )
    {
        *pxTCBSize = حجم( TCB_t );
    }

    أرجع ux_Array_عدد_Filled;
}
/*----------------------------------------------------------*/

/* ----------------------------------------------------- Misc ----------------------------------------------------- */

فراغ * pvTaskGetCurrentTCBForCore( ادھار_Type_t xCoreID )
{
    فراغ * pvRet;

    configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

    #إذا CONFIG_FREERTOS_SMP
        /* SMP FreeRTOS defines pxCurrentTCB as a macro function call */
        pvRet = ( فراغ * ) pxCurrentTCB;
    #وإلا /* CONFIG_FREERTOS_SMP */
        pvRet = ( فراغ * ) pxCurrentTCBs[ xCoreID ];
    #جے_انت /* CONFIG_FREERTOS_SMP */
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
        ESP_FREERTOS_DEBUG_جدول_ناپ = 0,
        ESP_FREERTOS_DEBUG_جدول_نسخہ,
        ESP_FREERTOS_DEBUG_KERNEL_VER_MAJOR,
        ESP_FREERTOS_DEBUG_KERNEL_VER_MINOR,
        ESP_FREERTOS_DEBUG_KERNEL_VER_BUILD,
        ESP_FREERTOS_DEBUG_UX_چوٹی_ورتیا_PIORITY,
        ESP_FREERTOS_DEBUG_PX_چوٹی_دا_ڈھیر,
        ESP_FREERTOS_DEBUG_PC_TASK_NAME,
        ESP_FREERTOS_DEBUG_LIST_ناپ,
        ESP_FREERTOS_DEBUG_LIST_عدد_اشیاء,
        ESP_FREERTOS_DEBUG_LIST_سرا,
        ESP_FREERTOS_DEBUG_LIST_سرا_PREV,
        ESP_FREERTOS_DEBUG_LIST_شے_PREV,
        ESP_FREERTOS_DEBUG_LIST_شے_OWNER,
        ESP_FREERTOS_DEBUG_TASK_گنتی_چوڑائی,
        ESP_FREERTOS_DEBUG_PTR_چوڑائی,
        /* New entries must be inserted here */
        ESP_FREERTOS_DEBUG_جدول_سرا,
    };

    ثابت DRAM_ATTR uint8_t FreeRTOS_openocd_params[ ESP_FREERTOS_DEBUG_جدول_سرا ] =
    {
        ESP_FREERTOS_DEBUG_جدول_سرا, /* table size */
        1,                            /* table version */
        tsk_KERNEL_نسخہ_MAJOR,
        tsk_KERNEL_نسخہ_MINOR,
        tsk_KERNEL_نسخہ_BUILD,
        configMAX_PRIORITIES - 1,                   /* uxTopUsedPriority */
        offsetof( TCB_t, px_چوٹی_دا_ڈھیر ),            /* thread_stack_offset; */
        offsetof( TCB_t, pcTaskName ),              /* thread_name_offset; */
        حجم( List_t ),                           /* list_width */
        offsetof( List_t, ux_نمبر_دا_اشیاء ),        /* list_item_num */
        offsetof( List_t, x_List_سرا ),               /* list_end_offset */
        offsetof( List_t, x_List_سرا.px_پچھلا ),    /* list_next_offset */
        offsetof( List_شے_t, px_پچھلا ),         /* list_elem_next_offset */
        offsetof( List_شے_t, pvOwner ),            /* list_elem_content_offset */
        حجم( UBaseType_t ),                      /* task_count_width */
        حجم( فراغ * )                            /* ptr_width */
    };

#جے_انت /* CONFIG_FREERTOS_DEBUG_OCDAWARE */
/*----------------------------------------------------------*/

/* ----------------------------------------------------- PSRAM ---------------------------------------------------- */

#إذا CONFIG_SPIRAM

    #إذا CONFIG_FREERTOS_SMP
        ادھار_Type_t prv_Task_Create_Dynamic_Affinity_سیٹ_With_Caps( TaskFunction_t px_Task_کوڈ,
                                                            ثابت حرف * ثابت pcName,
                                                            ثابت config_ڈھیر_ڈونگھائی_TYPE us_ڈھیر_ڈونگھائی,
                                                            فراغ * ثابت pvParameters,
                                                            UBaseType_t ux_ترجیح,
                                                            UBaseType_t ux_Core_Affinity_نقاب,
                                                            UBaseType_t ux_ڈھیر_حافظہ_Caps,
                                                            TaskHandle_t * ثابت pxCreatedTask )
    #وإلا /* CONFIG_FREERTOS_SMP */
        ادھار_Type_t prv_Task_Create_Dynamic_Pinned_منزل_Core_With_Caps( TaskFunction_t px_Task_کوڈ,
                                                             ثابت حرف * ثابت pcName,
                                                             ثابت config_ڈھیر_ڈونگھائی_TYPE us_ڈھیر_ڈونگھائی,
                                                             فراغ * ثابت pvParameters,
                                                             UBaseType_t ux_ترجیح,
                                                             ثابت ادھار_Type_t xCoreID,
                                                             UBaseType_t ux_ڈھیر_حافظہ_Caps,
                                                             TaskHandle_t * ثابت pxCreatedTask )
    #جے_انت /* CONFIG_FREERTOS_SMP */
    {
        TCB_t * pxNewTCB;
        ادھار_Type_t xReturn;

        ڈھیر_Type_t * px_ڈھیر;

        configASSERT( ux_ڈھیر_حافظہ_Caps & ( MALLOC_CAP_8_ثنائی_ہندسہ ) );
        configASSERT( ( ux_ڈھیر_حافظہ_Caps & MALLOC_CAP_SPIRAM ) ||
                      ( ux_ڈھیر_حافظہ_Caps & MALLOC_CAP_اندرونی ) );
        #إذا ( !CONFIG_FREERTOS_SMP )
        {
            configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );
        }
        #جے_انت /* !CONFIG_FREERTOS_SMP */

        /* Allocate space for the stack used by the task being created. */
        px_ڈھیر = ڈھیر_caps_malloc( ( ( ( size_t ) us_ڈھیر_ڈونگھائی ) * حجم( ڈھیر_Type_t ) ), ux_ڈھیر_حافظہ_Caps );

        إذا( px_ڈھیر != عدم )
        {
            /* Allocate space for the TCB. */
            pxNewTCB = ( TCB_t * ) pv_پورٹ_Malloc( حجم( TCB_t ) );

            إذا( pxNewTCB != عدم )
            {
                memset( ( فراغ * ) pxNewTCB, 0x00, حجم( TCB_t ) );

                /* Store the stack location in the TCB. */
                pxNewTCB->px_ڈھیر = px_ڈھیر;
            }
            وإلا
            {
                /* The stack cannot be used as the TCB has not been created. Free it. */
                ڈھیر_caps_free( px_ڈھیر );
            }
        }
        وإلا
        {
            pxNewTCB = عدم;
        }

        إذا( pxNewTCB != عدم )
        {
            #إذا ( tsk_STATIC_AND_DYNAMIC_ونڈ_POSSIBLE != 0 )
            {
                /* Tasks can be created statically or dynamically, so note this
                 * task was created dynamically in case it is later deleted. */
                pxNewTCB->ucStaticallyAllocated = tsk_DYNAMICALLY_ALLOCATED_ڈھیر_AND_TCB;
            }
            #جے_انت /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

            #إذا CONFIG_FREERTOS_SMP
            {
                prvInitialiseNewTask( px_Task_کوڈ, pcName, ( uint32_t ) us_ڈھیر_ڈونگھائی, pvParameters, ux_ترجیح, pxCreatedTask, pxNewTCB, عدم );
                #إذا ( ( config_عدد_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
                {
                    /* Set the task's affinity before scheduling it */
                    pxNewTCB->ux_Core_Affinity_نقاب = ux_Core_Affinity_نقاب;
                }
                #جے_انت /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            }
            #وإلا /* CONFIG_FREERTOS_SMP */
            {
                prvInitialiseNewTask( px_Task_کوڈ, pcName, ( uint32_t ) us_ڈھیر_ڈونگھائی, pvParameters, ux_ترجیح, pxCreatedTask, pxNewTCB, عدم, xCoreID );
            }
            #جے_انت /* CONFIG_FREERTOS_SMP */

            prv_جوڑو_New_Task_منزل_Ready_List( pxNewTCB );
            xReturn = pdPASS;
        }
        وإلا
        {
            xReturn = err_COULD_NOT_ALLOCATE_REQUIRED_حافظہ;
        }

        أرجع xReturn;
    }

#جے_انت /* CONFIG_SPIRAM */
/*----------------------------------------------------------*/
