/*
 * SPDX-FileCopyrightText: 2015-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#شامل "sdkconfig.h"
#شامل "esp_assert.h"
#شامل "esp_heap_caps.h"
#شامل "esp_compiler.h"
#شامل "freertos/idf_additions.h"
#شامل "freertos/freertos_debug.h"
#شامل "esp_private/freertos_idf_additions_priv.h"

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
_Static_assert( offsetof( StaticTask_t, pxDummy8 ) == offsetof( TCB_t, px_انتہا_کا_ڈھیر ) );
#اگر !CONFIG_IDF_ہدف_LINUX    // Disabled for linux builds due to differences in types
_Static_assert( tskNO_AFFINITY == ( بنیاد_Type_t ) CONFIG_FREERTOS_NO_AFFINITY, "CONFIG_FREERTOS_NO_AFFINITY must be the same as tskNO_AFFINITY" );
#اگر_ختم

/* ------------------------------------------------- Kernel Control ------------------------------------------------- */

#اگر ( !CONFIG_FREERTOS_SMP && ( config_عدد_CORES > 1 ) )

/*
 * Wrapper function to take "xKerneLock"
 */
    خالی prv_Take_Kernel_تالا( خالی )
    {
        /* We call the tasks.c critical section macro to take xKernelLock */
        taskENTER_CRITICAL( &x_Kernel_تالا );
    }

#اگر_ختم /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#اگر ( !CONFIG_FREERTOS_SMP && ( config_عدد_CORES > 1 ) )

/*
 * Wrapper function to release "xKerneLock"
 */
    خالی prv_Release_Kernel_تالا( خالی )
    {
        /* We call the tasks.c critical section macro to release xKernelLock */
        taskEXIT_CRITICAL( &x_Kernel_تالا );
    }

#اگر_ختم /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#اگر ( CONFIG_FREERTOS_SMP && ( config_عدد_CORES > 1 ) )

/*
 * Workaround for non-thread safe multi-core OS startup (see IDF-4524)
 */
    خالی prv_آغاز_Scheduler_Other_Cores( خالی )
    {
        /* This function is always called with interrupts disabled*/
        xSchedulerRunning = pdTRUE;
    }

#اگر_ختم /* ( CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#اگر ( !CONFIG_FREERTOS_SMP && ( config_عدد_CORES > 1 ) )

    بنیاد_Type_t xTaskIncrementTickOtherCores( خالی )
    {
        /* Minor optimization. This function can never switch cores mid
         * execution */
        بنیاد_Type_t xCoreID = پورٹ_GET_CORE_ID();
        بنیاد_Type_t xSwitchRequired = pdFALSE;

        /* This function should never be called by Core 0. */
        configASSERT( xCoreID != 0 );

        /* Called by the portable layer each time a tick interrupt occurs
         * on a core other than core 0. */
        traceTASK_INCREMENT_TICK( x_Tick_گنتی );

        اگر( uxSchedulerSuspended[ xCoreID ] == ( UBaseType_t ) 0U )
        {
            /* We need take the kernel lock here as we are about to access
             * kernel data structures. */
            taskENTER_CRITICAL_ISR( &x_Kernel_تالا );

            /* Tasks of equal priority to the currently running task will share
             * processing time (time slice) if preemption is on, and the application
             * writer has not explicitly turned time slicing off. */
            #اگر ( ( configUSE_PREEMPTION == 1 ) && ( config_USE_وقت_SLICING == 1 ) )
            {
                اگر( list_CURRENT_LIST_لمبائی( &( pxReadyTasksLists[ pxCurrentTCBs[ xCoreID ]->ux_ترجیح ] ) ) > ( UBaseType_t ) 1 )
                {
                    xSwitchRequired = pdTRUE;
                }
                ورنہ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            #اگر_ختم /* ( ( configUSE_PREEMPTION == 1 ) && ( configUSE_TIME_SLICING == 1 ) ) */

            /* Release the previously taken kernel lock as we have finished
             * accessing the kernel data structures. */
            taskEXIT_CRITICAL_ISR( &x_Kernel_تالا );

            #اگر ( configUSE_PREEMPTION == 1 )
            {
                اگر( xYieldPending[ xCoreID ] != pdFALSE )
                {
                    xSwitchRequired = pdTRUE;
                }
                ورنہ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            #اگر_ختم /* configUSE_PREEMPTION */
        }

        #اگر ( configUSE_TICK_HOOK == 1 )
        {
            vApplicationTickHook();
        }
        #اگر_ختم

        واپس xSwitchRequired;
    }

#اگر_ختم /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

/* -------------------------------------------------- Task Creation ------------------------------------------------- */

#اگر ( config_SUPPORT_DYNAMIC_تخصیص == 1 )

    بنیاد_Type_t x_Task_Create_Pinned_مقصد_Core( TaskFunction_t px_Task_رمز,
                                        ثابت حرف * ثابت pcName,
                                        ثابت uint32_t us_ڈھیر_گہرائی,
                                        خالی * ثابت pvParameters,
                                        UBaseType_t ux_ترجیح,
                                        TaskHandle_t * ثابت pxCreatedTask,
                                        ثابت بنیاد_Type_t xCoreID )
    {
        بنیاد_Type_t xReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );

        #اگر CONFIG_FREERTOS_SMP
        {
            /* If using Amazon SMP FreeRTOS. This function is just a wrapper around
             * xTaskCreate() or xTaskCreateAffinitySet(). */
            #اگر ( ( config_عدد_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
            {
                /* Convert xCoreID into an affinity mask */
                UBaseType_t ux_Core_Affinity_نقاب;

                /* Bit shifting << xCoreID is only valid if we have less than
                 * 32 cores. */
                ESP_STATIC_ASSERT( config_عدد_CORES < 32 );

                اگر( xCoreID == tskNO_AFFINITY )
                {
                    ux_Core_Affinity_نقاب = tskNO_AFFINITY;
                }
                ورنہ
                {
                    ux_Core_Affinity_نقاب = ( 1 << xCoreID );
                }

                xReturn = x_Task_Create_Affinity_سیٹ( px_Task_رمز, pcName, us_ڈھیر_گہرائی, pvParameters, ux_ترجیح, ux_Core_Affinity_نقاب, pxCreatedTask );
            }
            #ورنہ /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            {
                xReturn = xTaskCreate( px_Task_رمز, pcName, us_ڈھیر_گہرائی, pvParameters, ux_ترجیح, pxCreatedTask );
            }
            #اگر_ختم /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
        }
        #ورنہ /* CONFIG_FREERTOS_SMP */
        {
            TCB_t * pxNewTCB;

            /* If the stack grows down then allocate the stack then the TCB so the
             * stack does not grow into the TCB.  Likewise if the stack grows up
             * then allocate the TCB then the stack. */
            #اگر ( پورٹ_ڈھیر_GROWTH > 0 )
            {
                /* Allocate space for the TCB.  Where the memory comes from depends on
                 * the implementation of the port malloc function and whether or not static
                 * allocation is being used. */
                pxNewTCB = ( TCB_t * ) pv_پورٹ_Malloc( حجم( TCB_t ) );

                اگر( pxNewTCB != تہی )
                {
                    memset( ( خالی * ) pxNewTCB, 0x00, حجم( TCB_t ) );

                    /* Allocate space for the stack used by the task being created.
                     * The base of the stack memory stored in the TCB so the task can
                     * be deleted later if required. */
                    pxNewTCB->px_ڈھیر = ( ڈھیر_Type_t * ) pv_پورٹ_Malloc_ڈھیر( ( ( ( size_t ) us_ڈھیر_گہرائی ) * حجم( ڈھیر_Type_t ) ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */

                    اگر( pxNewTCB->px_ڈھیر == تہی )
                    {
                        /* Could not allocate the stack.  Delete the allocated TCB. */
                        v_پورٹ_Free( pxNewTCB );
                        pxNewTCB = تہی;
                    }
                }
            }
            #ورنہ /* portSTACK_GROWTH */
            {
                ڈھیر_Type_t * px_ڈھیر;

                /* Allocate space for the stack used by the task being created. */
                px_ڈھیر = pv_پورٹ_Malloc_ڈھیر( ( ( ( size_t ) us_ڈھیر_گہرائی ) * حجم( ڈھیر_Type_t ) ) ); /*lint !e9079 All values returned by pvPortMalloc() have at least the alignment required by the MCU's stack and this allocation is the stack. */

                اگر( px_ڈھیر != تہی )
                {
                    /* Allocate space for the TCB. */
                    pxNewTCB = ( TCB_t * ) pv_پورٹ_Malloc( حجم( TCB_t ) ); /*lint !e9087 !e9079 All values returned by pvPortMalloc() have at least the alignment required by the MCU's stack, and the first member of TCB_t is always a pointer to the task's stack. */

                    اگر( pxNewTCB != تہی )
                    {
                        memset( ( خالی * ) pxNewTCB, 0x00, حجم( TCB_t ) );

                        /* Store the stack location in the TCB. */
                        pxNewTCB->px_ڈھیر = px_ڈھیر;
                    }
                    ورنہ
                    {
                        /* The stack cannot be used as the TCB was not created.  Free
                         * it again. */
                        v_پورٹ_Free_ڈھیر( px_ڈھیر );
                    }
                }
                ورنہ
                {
                    pxNewTCB = تہی;
                }
            }
            #اگر_ختم /* portSTACK_GROWTH */

            اگر( pxNewTCB != تہی )
            {
                #اگر ( tsk_STATIC_AND_DYNAMIC_تخصیص_POSSIBLE != 0 ) /*lint !e9029 !e731 Macro has been consolidated for readability reasons. */
                {
                    /* Tasks can be created statically or dynamically, so note this
                     * task was created dynamically in case it is later deleted. */
                    pxNewTCB->ucStaticallyAllocated = tsk_DYNAMICALLY_ALLOCATED_ڈھیر_AND_TCB;
                }
                #اگر_ختم /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

                prvInitialiseNewTask( px_Task_رمز, pcName, ( uint32_t ) us_ڈھیر_گہرائی, pvParameters, ux_ترجیح, pxCreatedTask, pxNewTCB, تہی, xCoreID );
                prv_جوڑو_New_Task_مقصد_Ready_List( pxNewTCB );
                xReturn = pdPASS;
            }
            ورنہ
            {
                xReturn = err_COULD_NOT_ALLOCATE_REQUIRED_حافظہ;
            }
        }
        #اگر_ختم /* CONFIG_FREERTOS_SMP */

        واپس xReturn;
    }

#اگر_ختم /* ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#اگر ( config_SUPPORT_STATIC_تخصیص == 1 )

    TaskHandle_t x_Task_Create_Static_Pinned_مقصد_Core( TaskFunction_t px_Task_رمز,
                                                ثابت حرف * ثابت pcName,
                                                ثابت uint32_t ul_ڈھیر_گہرائی,
                                                خالی * ثابت pvParameters,
                                                UBaseType_t ux_ترجیح,
                                                ڈھیر_Type_t * ثابت pux_ڈھیر_بفر,
                                                StaticTask_t * ثابت px_Task_بفر,
                                                ثابت بنیاد_Type_t xCoreID )
    {
        TaskHandle_t xReturn;

        configASSERT( پورٹ_VALID_ڈھیر_MEM( pux_ڈھیر_بفر ) );
        configASSERT( پورٹ_VALID_TCB_MEM( px_Task_بفر ) );
        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );

        #اگر CONFIG_FREERTOS_SMP
        {
            /* If using Amazon SMP FreeRTOS. This function is just a wrapper around
             * xTaskCreateStatic() or xTaskCreateStaticAffinitySet(). */
            #اگر ( ( config_عدد_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
            {
                /* Convert xCoreID into an affinity mask */
                UBaseType_t ux_Core_Affinity_نقاب;

                /* Bit shifting << xCoreID is only valid if we have less than
                 * 32 cores. */
                ESP_STATIC_ASSERT( config_عدد_CORES < 32 );

                اگر( xCoreID == tskNO_AFFINITY )
                {
                    ux_Core_Affinity_نقاب = tskNO_AFFINITY;
                }
                ورنہ
                {
                    ux_Core_Affinity_نقاب = ( 1 << xCoreID );
                }

                xReturn = x_Task_Create_Static_Affinity_سیٹ( px_Task_رمز, pcName, ul_ڈھیر_گہرائی, pvParameters, ux_ترجیح, pux_ڈھیر_بفر, px_Task_بفر, ux_Core_Affinity_نقاب );
            }
            #ورنہ /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            {
                xReturn = xTaskCreateStatic( px_Task_رمز, pcName, ul_ڈھیر_گہرائی, pvParameters, ux_ترجیح, pux_ڈھیر_بفر, px_Task_بفر );
            }
            #اگر_ختم /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
        }
        #ورنہ /* CONFIG_FREERTOS_SMP */
        {
            TCB_t * pxNewTCB;

            #اگر ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticTask_t equals the size of the real task
                 * structure. */
                متلون size_t x_سائز = حجم( StaticTask_t );
                configASSERT( x_سائز == حجم( TCB_t ) );
                ( خالی ) x_سائز; /* Prevent lint warning when configASSERT() is not used. */
            }
            #اگر_ختم /* configASSERT_DEFINED */

            اگر( ( px_Task_بفر != تہی ) && ( pux_ڈھیر_بفر != تہی ) )
            {
                /* The memory used for the task's TCB and stack are passed into this
                 * function - use them. */
                pxNewTCB = ( TCB_t * ) px_Task_بفر; /*lint !e740 !e9087 Unusual cast is ok as the structures are designed to have the same alignment, and the size is checked by an assert. */
                memset( ( خالی * ) pxNewTCB, 0x00, حجم( TCB_t ) );
                pxNewTCB->px_ڈھیر = ( ڈھیر_Type_t * ) pux_ڈھیر_بفر;

                #اگر ( tsk_STATIC_AND_DYNAMIC_تخصیص_POSSIBLE != 0 ) /*lint !e731 !e9029 Macro has been consolidated for readability reasons. */
                {
                    /* Tasks can be created statically or dynamically, so note this
                     * task was created statically in case the task is later deleted. */
                    pxNewTCB->ucStaticallyAllocated = tsk_STATICALLY_ALLOCATED_ڈھیر_AND_TCB;
                }
                #اگر_ختم /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

                prvInitialiseNewTask( px_Task_رمز, pcName, ul_ڈھیر_گہرائی, pvParameters, ux_ترجیح, &xReturn, pxNewTCB, تہی, xCoreID );
                prv_جوڑو_New_Task_مقصد_Ready_List( pxNewTCB );
            }
            ورنہ
            {
                xReturn = تہی;
            }
        }
        #اگر_ختم /* CONFIG_FREERTOS_SMP */

        واپس xReturn;
    }

#اگر_ختم /* ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#اگر ( configUSE_TIMERS == 1 )

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
    بنیاد_Type_t __attribute__( ( weak ) ) xTimerCreateTimerTask( خالی )
    {
        واپس pdPASS;
    }

#اگر_ختم /* configUSE_TIMERS */
/*----------------------------------------------------------*/

/* ------------------------------------------------- Task Utilities ------------------------------------------------- */

بنیاد_Type_t xTaskGetCoreID( TaskHandle_t xTask )
{
    بنیاد_Type_t xReturn;

    #اگر ( config_عدد_CORES > 1 )
    {
        #اگر CONFIG_FREERTOS_SMP
            UBaseType_t ux_Core_Affinity_نقاب;

            /* Get the core affinity mask and convert it to an ID */
            ux_Core_Affinity_نقاب = vTaskCoreAffinityGet( xTask );

            /* If the task is not pinned to a particular core, treat it as tskNO_AFFINITY */
            اگر( ux_Core_Affinity_نقاب & ( ux_Core_Affinity_نقاب - 1 ) ) /* If more than one bit set */
            {
                xReturn = tskNO_AFFINITY;
            }
            ورنہ
            {
                صحیح i_اشاریہ_Plus_One = __builtin_ffs( ux_Core_Affinity_نقاب );
                أكد( i_اشاریہ_Plus_One >= 1 );
                xReturn = i_اشاریہ_Plus_One - 1;
            }
        #ورنہ /* CONFIG_FREERTOS_SMP */
            TCB_t * pxTCB;

            pxTCB = prvGetTCBFromHandle( xTask );

            xReturn = pxTCB->xCoreID;
        #اگر_ختم /* CONFIG_FREERTOS_SMP */
    }
    #ورنہ /* configNUM_CORES > 1 */
    {
        /* Single-core. Just return a core ID of 0 */
        xReturn = 0;
    }
    #اگر_ختم /* configNUM_CORES > 1 */

    واپس xReturn;
}
/*----------------------------------------------------------*/

#اگر ( ( !CONFIG_FREERTOS_SMP ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

    TaskHandle_t xTaskGetIdleTaskHandleForCore( بنیاد_Type_t xCoreID )
    {
        /* If xTaskGetIdleTaskHandle() is called before the scheduler has been
         * started, then xIdleTaskHandle will be NULL. */
        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );
        configASSERT( ( xIdleTaskHandle[ xCoreID ] != تہی ) );
        واپس xIdleTaskHandle[ xCoreID ];
    }

#اگر_ختم /* ( ( !CONFIG_FREERTOS_SMP ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*----------------------------------------------------------*/

#اگر ( ( !CONFIG_FREERTOS_SMP ) && ( ( INCLUDE_xTaskGetCurrentTaskHandle == 1 ) || ( configUSE_MUTEXES == 1 ) ) )

    TaskHandle_t xTaskGetCurrentTaskHandleForCore( بنیاد_Type_t xCoreID )
    {
        TaskHandle_t xReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

        #اگر ( CONFIG_FREERTOS_SMP )
        {
            xReturn = xTaskGetCurrentTaskHandleCPU( ( UBaseType_t ) xCoreID );
        }
        #ورنہ /* CONFIG_FREERTOS_SMP */
        {
            /* A critical section is not required as this function does not
             * guarantee that the TCB will still be valid when this function
             * returns. */
            xReturn = pxCurrentTCBs[ xCoreID ];
        }
        #اگر_ختم /* CONFIG_FREERTOS_SMP */

        واپس xReturn;
    }

#اگر_ختم /* ( ( !CONFIG_FREERTOS_SMP ) && ( ( INCLUDE_xTaskGetCurrentTaskHandle == 1 ) || ( configUSE_MUTEXES == 1 ) ) ) */
/*----------------------------------------------------------*/

#اگر ( !CONFIG_FREERTOS_SMP && ( config_GENERATE_تسلسل_وقت_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

    config_تسلسل_وقت_شمار_TYPE ul_Task_Get_Idle_تسلسل_وقت_شمار_For_Core( بنیاد_Type_t xCoreID )
    {
        config_تسلسل_وقت_شمار_TYPE ul_تسلسل_وقت_شمار;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

        /* For SMP, we need to take the kernel lock here as we are about to
         * access kernel data structures. */
        prvENTER_CRITICAL_SMP_ONLY( &x_Kernel_تالا );
        {
            ul_تسلسل_وقت_شمار = xIdleTaskHandle[ xCoreID ]->ul_تسلسل_وقت_شمار;
        }
        /* Release the previously taken kernel lock. */
        prvEXIT_CRITICAL_SMP_ONLY( &x_Kernel_تالا );

        واپس ul_تسلسل_وقت_شمار;
    }

#اگر_ختم /* ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*----------------------------------------------------------*/

#اگر ( !CONFIG_FREERTOS_SMP && ( config_GENERATE_تسلسل_وقت_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

    config_تسلسل_وقت_شمار_TYPE ul_Task_Get_Idle_تسلسل_وقت_Percent_For_Core( بنیاد_Type_t xCoreID )
    {
        config_تسلسل_وقت_شمار_TYPE ul_کل_وقت, ulReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

        #اگر_تعریف_شدہ پورٹ_ALT_GET_تسلسل_وقت_شمار_قدر
            پورٹ_ALT_GET_تسلسل_وقت_شمار_قدر( ul_کل_وقت );
        #ورنہ
            ul_کل_وقت = پورٹ_GET_تسلسل_وقت_شمار_قدر();
        #اگر_ختم

        /* For percentage calculations. */
        ul_کل_وقت /= ( config_تسلسل_وقت_شمار_TYPE ) 100;

        /* Avoid divide by zero errors. */
        اگر( ul_کل_وقت > ( config_تسلسل_وقت_شمار_TYPE ) 0 )
        {
            /* For SMP, we need to take the kernel lock here as we are about
             * to access kernel data structures. */
            prvENTER_CRITICAL_SMP_ONLY( &x_Kernel_تالا );
            {
                ulReturn = xIdleTaskHandle[ xCoreID ]->ul_تسلسل_وقت_شمار / ul_کل_وقت;
            }
            /* Release the previously taken kernel lock. */
            prvEXIT_CRITICAL_SMP_ONLY( &x_Kernel_تالا );
        }
        ورنہ
        {
            ulReturn = 0;
        }

        واپس ulReturn;
    }

#اگر_ختم /* ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*-----------------------------------------------------------*/

ڈھیر_Type_t * x_Task_Get_ڈھیر_آغاز( TaskHandle_t xTask )
{
    TCB_t * pxTCB;

    pxTCB = prvGetTCBFromHandle( xTask );
    واپس pxTCB->px_ڈھیر;
}
/*----------------------------------------------------------*/

uint8_t * px_Task_Get_ڈھیر_آغاز( TaskHandle_t xTask )
{
    واپس (uint8_t *)x_Task_Get_ڈھیر_آغاز( xTask );
}
/*----------------------------------------------------------*/

#اگر ( INCLUDE_v_Task_ترجیح_سیٹ == 1 )

    خالی prv_Task_ترجیح_Raise( prv_Task_Saved_ترجیح_t * px_Saved_ترجیح,
                               UBaseType_t ux_New_ترجیح )
    {
        TCB_t * pxTCB;
        UBaseType_t ux_ترجیح_مستعمل_On_اندراج;

        configASSERT( ( ux_New_ترجیح < configMAX_PRIORITIES ) );

        /* Ensure the new priority is valid. */
        اگر( ux_New_ترجیح >= ( UBaseType_t ) configMAX_PRIORITIES )
        {
            ux_New_ترجیح = ( UBaseType_t ) configMAX_PRIORITIES - ( UBaseType_t ) 1U;
        }

        #اگر CONFIG_FREERTOS_SMP
            taskENTER_CRITICAL();
        #ورنہ
            taskENTER_CRITICAL( &x_Kernel_تالا );
        #اگر_ختم
        {
            pxTCB = prvGetTCBFromHandle( تہی );

            #اگر ( configUSE_MUTEXES == 1 )
            {
                px_Saved_ترجیح->ux_ترجیح = pxTCB->ux_ترجیح;
                px_Saved_ترجیح->ux_بنیاد_ترجیح = pxTCB->ux_بنیاد_ترجیح;

                /* If uxNewPriority < uxBasePriority, then there is nothing else to
                 * do, as uxBasePriority is always <= uxPriority. */
                اگر( ux_New_ترجیح > pxTCB->ux_بنیاد_ترجیح )
                {
                    pxTCB->ux_بنیاد_ترجیح = ux_New_ترجیح;

                    /* Remember the task's current priority before attempting to
                     * change it. If the task's current priority is changed, it must
                     * be done so before moving the task between task lists) in order
                     * for the taskRESET_READY_PRIORITY() macro to function correctly. */
                    ux_ترجیح_مستعمل_On_اندراج = pxTCB->ux_ترجیح;

                    اگر( ux_New_ترجیح > pxTCB->ux_ترجیح )
                    {
                        pxTCB->ux_ترجیح = ux_New_ترجیح;

                        /* Only reset the event list item value if the value is not
                         * being used for anything else. */
                        اگر( ( list_GET_LIST_شے_قدر( &( pxTCB->x_Event_List_شے ) ) & task_EVENT_LIST_شے_قدر_اندر_USE ) == 0UL )
                        {
                            list_سیٹ_LIST_شے_قدر( &( pxTCB->x_Event_List_شے ), ( ( TickType_t ) configMAX_PRIORITIES - ( TickType_t ) ux_New_ترجیح ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */
                        }

                        /* If the task is in the blocked or suspended list we need do
                         * nothing more than change its priority variable. However, if
                         * the task is in a ready list it needs to be removed and placed
                         * in the list appropriate to its new priority. */
                        اگر( listIS_CONTAINED_WITHIN( &( pxReadyTasksLists[ ux_ترجیح_مستعمل_On_اندراج ] ), &( pxTCB->x_State_List_شے ) ) != pdFALSE )
                        {
                            /* The task is currently in its ready list - remove before
                             * adding it to its new ready list.  As we are in a critical
                             * section we can do this even if the scheduler is suspended. */
                            اگر( uxListRemove( &( pxTCB->x_State_List_شے ) ) == ( UBaseType_t ) 0 )
                            {
                                /* It is known that the task is in its ready list so
                                 * there is no need to check again and the port level
                                 * reset macro can be called directly. */
                                پورٹ_RESET_READY_ترجیح( ux_ترجیح_مستعمل_On_اندراج, ux_چوٹی_Ready_ترجیح );
                            }

                            prv_جوڑو_Task_مقصد_Ready_List( pxTCB );
                        }
                    }
                }
            }
            #ورنہ /* if ( configUSE_MUTEXES == 1 ) */
            {
                px_Saved_ترجیح->ux_ترجیح = pxTCB->ux_ترجیح;

                اگر( ux_New_ترجیح > pxTCB->ux_ترجیح )
                {
                    v_Task_ترجیح_سیٹ( تہی, ux_New_ترجیح );
                }
            }
            #اگر_ختم /* if ( configUSE_MUTEXES == 1 ) */
        }
        #اگر CONFIG_FREERTOS_SMP
            taskEXIT_CRITICAL();
        #ورنہ
            taskEXIT_CRITICAL( &x_Kernel_تالا );
        #اگر_ختم
    }

#اگر_ختم /* INCLUDE_vTaskPrioritySet == 1 */
/*----------------------------------------------------------*/

#اگر ( INCLUDE_v_Task_ترجیح_سیٹ == 1 )

    خالی prv_Task_ترجیح_Restore( prv_Task_Saved_ترجیح_t * px_Saved_ترجیح )
    {
        TCB_t * pxTCB;
        UBaseType_t ux_New_ترجیح;
        UBaseType_t ux_ترجیح_مستعمل_On_اندراج;
        UBaseType_t ux_بنیاد_ترجیح_مستعمل_On_اندراج;
        بنیاد_Type_t xYieldRequired = pdFALSE;

        #اگر CONFIG_FREERTOS_SMP
            taskENTER_CRITICAL();
        #ورنہ
            taskENTER_CRITICAL( &x_Kernel_تالا );
        #اگر_ختم
        {
            pxTCB = prvGetTCBFromHandle( تہی );

            #اگر ( configUSE_MUTEXES == 1 )
            {
                /* If the saved uxBasePriority == the task's uxBasePriority, it means
                 * that prvTaskPriorityRaise() never raised the task's uxBasePriority.
                 * In that case, there is nothing else to do. */
                اگر( px_Saved_ترجیح->ux_بنیاد_ترجیح != pxTCB->ux_بنیاد_ترجیح )
                {
                    ux_بنیاد_ترجیح_مستعمل_On_اندراج = pxTCB->ux_بنیاد_ترجیح;
                    pxTCB->ux_بنیاد_ترجیح = px_Saved_ترجیح->ux_بنیاد_ترجیح;

                    /* Remember the task's current priority before attempting to
                     * change it. If the task's current priority is changed, it must
                     * be done so before moving the task between task lists in order
                     * for the taskRESET_READY_PRIORITY() macro to function correctly. */
                    ux_ترجیح_مستعمل_On_اندراج = pxTCB->ux_ترجیح;

                    /* Check if the task inherited a priority after prvTaskPriorityRaise().
                     * If this is the case, there is nothing else to do. The priority
                     * will be restored when the task disinherits its priority. */
                    اگر( pxTCB->ux_ترجیح == ux_بنیاد_ترجیح_مستعمل_On_اندراج )
                    {
                        اگر( pxTCB->uxMutexesHeld == 0 )
                        {
                            /* The task may have inherited a priority before prvTaskPriorityRaise()
                             * then disinherited a priority after prvTaskPriorityRaise().
                             * Thus we need set the uxPriority to the saved base priority
                             * so that the task's priority gets restored to the priority
                             * before any inheritance or raising. */
                            pxTCB->ux_ترجیح = px_Saved_ترجیح->ux_بنیاد_ترجیح;
                        }
                        ورنہ
                        {
                            /* The task may have inherited a priority before prvTaskPriorityRaise()
                             * was called. Thus, we need to restore uxPriority to the
                             * "saved uxPriority" so that the task still retains that
                             * inherited priority. */
                            pxTCB->ux_ترجیح = px_Saved_ترجیح->ux_ترجیح;
                        }

                        ux_New_ترجیح = pxTCB->ux_ترجیح;

                        اگر( ux_New_ترجیح < ux_ترجیح_مستعمل_On_اندراج )
                        {
                            /* Setting the priority of the running task down means
                             * there may now be another task of higher priority that
                             * is ready to execute. */
                            xYieldRequired = pdTRUE;
                        }

                        /* Only reset the event list item value if the value is not
                         * being used for anything else. */
                        اگر( ( list_GET_LIST_شے_قدر( &( pxTCB->x_Event_List_شے ) ) & task_EVENT_LIST_شے_قدر_اندر_USE ) == 0UL )
                        {
                            list_سیٹ_LIST_شے_قدر( &( pxTCB->x_Event_List_شے ), ( ( TickType_t ) configMAX_PRIORITIES - ( TickType_t ) ux_New_ترجیح ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */
                        }

                        /* If the task is in the blocked or suspended list we need do
                         * nothing more than change its priority variable. However, if
                         * the task is in a ready list it needs to be removed and placed
                         * in the list appropriate to its new priority. */
                        اگر( listIS_CONTAINED_WITHIN( &( pxReadyTasksLists[ ux_ترجیح_مستعمل_On_اندراج ] ), &( pxTCB->x_State_List_شے ) ) != pdFALSE )
                        {
                            /* The task is currently in its ready list - remove before
                             * adding it to its new ready list.  As we are in a critical
                             * section we can do this even if the scheduler is suspended. */
                            اگر( uxListRemove( &( pxTCB->x_State_List_شے ) ) == ( UBaseType_t ) 0 )
                            {
                                /* It is known that the task is in its ready list so
                                 * there is no need to check again and the port level
                                 * reset macro can be called directly. */
                                پورٹ_RESET_READY_ترجیح( ux_ترجیح_مستعمل_On_اندراج, ux_چوٹی_Ready_ترجیح );
                            }

                            prv_جوڑو_Task_مقصد_Ready_List( pxTCB );
                        }

                        اگر( xYieldRequired != pdFALSE )
                        {
                            #اگر CONFIG_FREERTOS_SMP
                                taskYIELD_TASK_CORE_IF_USING_PREEMPTION( pxTCB );
                            #ورنہ
                                taskYIELD_IF_USING_PREEMPTION();
                            #اگر_ختم
                        }
                    }
                }
            }
            #ورنہ /* if ( configUSE_MUTEXES == 1 ) */
            {
                v_Task_ترجیح_سیٹ( تہی, px_Saved_ترجیح->ux_ترجیح );
            }
            #اگر_ختم /* if ( configUSE_MUTEXES == 1 ) */
        }
        #اگر CONFIG_FREERTOS_SMP
            taskEXIT_CRITICAL();
        #ورنہ
            taskEXIT_CRITICAL( &x_Kernel_تالا );
        #اگر_ختم
    }

#اگر_ختم /* ( INCLUDE_vTaskPrioritySet == 1 ) */
/*----------------------------------------------------------*/

/* --------------------------------------------- TLSP Deletion Callbacks -------------------------------------------- */

#اگر CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS

    خالی v_Task_سیٹ_Thread_Local_Storage_اشارہ_گر_And_Del_Callback( TaskHandle_t x_Task_مقصد_سیٹ,
                                                          بنیاد_Type_t x_اشاریہ,
                                                          خالی * pv_قدر,
                                                          TlsDeleteCallbackFunction_t pvDelCallback )
    {
        /* If TLSP deletion callbacks are enabled, then configNUM_THREAD_LOCAL_STORAGE_POINTERS
         * is doubled in size so that the latter half of the pvThreadLocalStoragePointers
         * stores the deletion callbacks. */
        اگر( x_اشاریہ < ( config_عدد_THREAD_LOCAL_STORAGE_اشارے / 2 ) )
        {
            TCB_t * pxTCB;

            #اگر ( config_عدد_CORES > 1 )
            {
                /* For SMP, we need a critical section as another core could also
                 * update this task's TLSP at the same time. */
                #اگر CONFIG_FREERTOS_SMP
                {
                    taskENTER_CRITICAL();
                }
                #ورنہ /* CONFIG_FREERTOS_SMP */
                {
                    taskENTER_CRITICAL( &x_Kernel_تالا );
                }
                #اگر_ختم /* CONFIG_FREERTOS_SMP */
            }
            #اگر_ختم /* configNUM_CORES > 1 */

            pxTCB = prvGetTCBFromHandle( x_Task_مقصد_سیٹ );
            /* Store the TLSP by indexing the first half of the array */
            pxTCB->pv_Thread_Local_Storage_اشارے[ x_اشاریہ ] = pv_قدر;

            /* Store the TLSP deletion callback by indexing the second half
             * of the array. */
            pxTCB->pv_Thread_Local_Storage_اشارے[ ( x_اشاریہ + ( config_عدد_THREAD_LOCAL_STORAGE_اشارے / 2 ) ) ] = ( خالی * ) pvDelCallback;

            #اگر ( config_عدد_CORES > 1 )
            {
                #اگر CONFIG_FREERTOS_SMP
                {
                    taskEXIT_CRITICAL();
                }
                #ورنہ /* CONFIG_FREERTOS_SMP */
                {
                    taskEXIT_CRITICAL( &x_Kernel_تالا );
                }
                #اگر_ختم /* CONFIG_FREERTOS_SMP */
            }
            #اگر_ختم /* configNUM_CORES > 1 */
        }
    }

#اگر_ختم /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */
/*----------------------------------------------------------*/

/* ----------------------------------------------------- Newlib ----------------------------------------------------- */

#اگر ( configUSE_NEWLIB_REENTRANT == 1 )

/**
 * @brief Get reentrancy structure of the current task
 *
 * - This function is required by newlib (when __DYNAMIC_REENT__ is enabled)
 * - It will return a pointer to the current task's reent struct
 * - If FreeRTOS is not running, it will return the global reent struct
 *
 * @return Pointer to a the (current taks's)/(global) reent struct
 */
    ساخت _reent * __getreent( خالی )
    {
        /* No lock needed because if this changes, we won't be running anymore. */
        TCB_t * pxCurTask = ( TCB_t * ) xTaskGetCurrentTaskHandle();
        ساخت _reent * ret;

        اگر( pxCurTask == تہی )
        {
            /* No task running. Return global struct. */
            ret = _GLOBAL_REENT;
        }
        ورنہ
        {
            /* We have a currently executing task. Return its reentrant struct. */
            ret = &pxCurTask->xTLSBlock;
        }

        واپس ret;
    }

#اگر_ختم /* configUSE_NEWLIB_REENTRANT == 1 */

/* -------------------------------------------------- Task Snapshot ------------------------------------------------- */

/**
 * @brief List of all task lists in FreeRTOS
 *
 * @note There are currently differing number of task list between SMP FreeRTOS and ESP-IDF FreeRTOS
 */
ساکن List_t * non_ready_task_lists[] =
{
    #اگر_تعریف_شدہ CONFIG_FREERTOS_SMP
        &xPendingReadyList,
    #ورنہ /* CONFIG_FREERTOS_SMP */
        &xPendingReadyList[ 0 ],
        #اگر_غیر_تعریف_شدہ CONFIG_FREERTOS_UNICORE
            &xPendingReadyList[ 1 ],
        #اگر_ختم /* CONFIG_FREERTOS_UNICORE */
    #اگر_ختم /* CONFIG_FREERTOS_SMP */
    &xDelayedTaskList1,
    &xDelayedTaskList2,
    #اگر ( INCLUDE_vTaskDelete == 1 )
        &x_Tasks_انتظار_Termination,
    #اگر_ختم
    #اگر ( INCLUDE_vTaskSuspend == 1 )
        &xSuspendedTaskList,
    #اگر_ختم
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
ساکن List_t * px_Get_Task_List_By_اشاریہ( UBaseType_t ux_List_اشاریہ )
{
    List_t * pxTaskList;
    ثابت size_t xNonReadyTaskListsCnt = ( حجم( non_ready_task_lists ) / حجم( List_t * ) );

    اگر( ux_List_اشاریہ < configMAX_PRIORITIES )
    {
        pxTaskList = &pxReadyTasksLists[ configMAX_PRIORITIES - 1 - ux_List_اشاریہ ];
    }
    ورنہ اگر( ux_List_اشاریہ < configMAX_PRIORITIES + xNonReadyTaskListsCnt )
    {
        pxTaskList = non_ready_task_lists[ ux_List_اشاریہ - configMAX_PRIORITIES ];
    }
    ورنہ
    {
        pxTaskList = تہی;
    }

    /* sanity check */
    اگر( pxTaskList )
    {
        اگر( !پورٹ_VALID_LIST_MEM( pxTaskList ) )
        {
            pxTaskList = تہی;
        }
    }

    واپس pxTaskList;
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
ساکن اندرونی UBaseType_t px_Get_Task_List_گنتی( خالی )
{
    واپس configMAX_PRIORITIES + ( حجم( non_ready_task_lists ) / حجم( List_t * ) );
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
صحیح x_Task_Get_اگلا( TaskIterator_t * xIterator )
{
    اگر( !xIterator )
    {
        واپس -1;
    }

    List_شے_t * px_اگلا_List_شے = xIterator->px_اگلا_List_شے;
    UBaseType_t uxCurListIdx = xIterator->ux_Current_List_اشاریہ;
    UBaseType_t uxMaxListIdx = px_Get_Task_List_گنتی();

    برائے( ; uxCurListIdx < uxMaxListIdx; ++uxCurListIdx )
    {
        List_t * pxCurrentTaskList = px_Get_Task_List_By_اشاریہ( uxCurListIdx );

        اگر( !pxCurrentTaskList || ( list_CURRENT_LIST_لمبائی( pxCurrentTaskList ) == 0 ) )
        {
            جاری;
        }

        ثابت List_شے_t * px_Curr_List_شے = list_GET_انتہا_MARKER( pxCurrentTaskList );

        اگر( !px_اگلا_List_شے )
        {
            /* We are here if the traversal starts from the beginning or when we finish traversing
             * for one of the state lists
             */
            px_اگلا_List_شے = list_GET_اگلا( px_Curr_List_شے );
        }

        اگر( !پورٹ_VALID_LIST_MEM( px_اگلا_List_شے ) )
        {
            /* Nothing to do with the corrupted list item. We will skip to the next task state list.
             * pxNextListItem should be NULL at the beginning of each task list.
             */
            px_اگلا_List_شے = تہی;
            جاری;
        }

        TCB_t * pxTCB = ( TCB_t * ) list_GET_LIST_شے_OWNER( px_اگلا_List_شے );

        اگر( !پورٹ_VALID_TCB_MEM( pxTCB ) )
        {
            pxTCB = تہی;
        }

        xIterator->pxTaskHandle = pxTCB;
        xIterator->ux_Current_List_اشاریہ = uxCurListIdx;

        اگر( px_Curr_List_شے->px_پچھلا == px_اگلا_List_شے )
        {
            /* If this is the last item of the current state list */
            xIterator->ux_Current_List_اشاریہ++;
            xIterator->px_اگلا_List_شے = تہی;
        }
        ورنہ
        {
            xIterator->px_اگلا_List_شے = list_GET_اگلا( px_اگلا_List_شے );
        }

        واپس uxCurListIdx;
    }

    واپس -1; /* end of the task list */
}
/*----------------------------------------------------------*/

بنیاد_Type_t vTaskGetSnapshot( TaskHandle_t pxTask,
                             TaskSnapshot_t * pxTaskSnapshot )
{
    ESP_STATIC_ANALYZER_CHECK(!pxTask, pdFALSE);

    اگر( ( پورٹ_VALID_TCB_MEM( pxTask ) == false ) || ( pxTaskSnapshot == تہی ) )
    {
        واپس pdFALSE;
    }

    TCB_t * pxTCB = ( TCB_t * ) pxTask;
    pxTaskSnapshot->pxTCB = pxTCB;
    pxTaskSnapshot->px_چوٹی_کا_ڈھیر = ( ڈھیر_Type_t * ) pxTCB->px_چوٹی_کا_ڈھیر;
    pxTaskSnapshot->px_انتہا_کا_ڈھیر = ( ڈھیر_Type_t * ) pxTCB->px_انتہا_کا_ڈھیر;
    واپس pdTRUE;
}
/*----------------------------------------------------------*/

UBaseType_t uxTaskGetSnapshotAll( TaskSnapshot_t * ثابت pxTaskSnapshotArray,
                                  ثابت UBaseType_t ux_Array_لمبائی,
                                  UBaseType_t * ثابت pxTCBSize )
{
    UBaseType_t ux_Array_عدد_Filled = 0;

    /* Traverse all of the tasks lists */
    TaskIterator_t xTaskIter = { 0 }; /* Point to the first task list */

    جب( x_Task_Get_اگلا( &xTaskIter ) != -1 && ux_Array_عدد_Filled < ux_Array_لمبائی )
    {
        vTaskGetSnapshot( xTaskIter.pxTaskHandle, &pxTaskSnapshotArray[ ux_Array_عدد_Filled ] );
        ux_Array_عدد_Filled++;
    }

    اگر( pxTCBSize != تہی )
    {
        *pxTCBSize = حجم( TCB_t );
    }

    واپس ux_Array_عدد_Filled;
}
/*----------------------------------------------------------*/

/* ----------------------------------------------------- Misc ----------------------------------------------------- */

خالی * pvTaskGetCurrentTCBForCore( بنیاد_Type_t xCoreID )
{
    خالی * pvRet;

    configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

    #اگر CONFIG_FREERTOS_SMP
        /* SMP FreeRTOS defines pxCurrentTCB as a macro function call */
        pvRet = ( خالی * ) pxCurrentTCB;
    #ورنہ /* CONFIG_FREERTOS_SMP */
        pvRet = ( خالی * ) pxCurrentTCBs[ xCoreID ];
    #اگر_ختم /* CONFIG_FREERTOS_SMP */
    واپس pvRet;
}

/* ----------------------------------------------------- OpenOCD ---------------------------------------------------- */

#اگر CONFIG_FREERTOS_DEBUG_OCDAWARE

/**
 * Debug param indexes. DO NOT change the order. OpenOCD uses the same indexes
 * Entries in FreeRTOS_openocd_params must match the order of these indexes
 */
    شمار
    {
        ESP_FREERTOS_DEBUG_جدول_سائز = 0,
        ESP_FREERTOS_DEBUG_جدول_نسخہ,
        ESP_FREERTOS_DEBUG_KERNEL_VER_MAJOR,
        ESP_FREERTOS_DEBUG_KERNEL_VER_MINOR,
        ESP_FREERTOS_DEBUG_KERNEL_VER_BUILD,
        ESP_FREERTOS_DEBUG_UX_چوٹی_مستعمل_PIORITY,
        ESP_FREERTOS_DEBUG_PX_چوٹی_کا_ڈھیر,
        ESP_FREERTOS_DEBUG_PC_TASK_NAME,
        ESP_FREERTOS_DEBUG_LIST_سائز,
        ESP_FREERTOS_DEBUG_LIST_عدد_اشیاء,
        ESP_FREERTOS_DEBUG_LIST_انتہا,
        ESP_FREERTOS_DEBUG_LIST_انتہا_پچھلا,
        ESP_FREERTOS_DEBUG_LIST_شے_پچھلا,
        ESP_FREERTOS_DEBUG_LIST_شے_OWNER,
        ESP_FREERTOS_DEBUG_TASK_گنتی_چوڑائی,
        ESP_FREERTOS_DEBUG_PTR_چوڑائی,
        /* New entries must be inserted here */
        ESP_FREERTOS_DEBUG_جدول_انتہا,
    };

    ثابت DRAM_ATTR uint8_t FreeRTOS_openocd_params[ ESP_FREERTOS_DEBUG_جدول_انتہا ] =
    {
        ESP_FREERTOS_DEBUG_جدول_انتہا, /* table size */
        1,                            /* table version */
        tsk_KERNEL_نسخہ_MAJOR,
        tsk_KERNEL_نسخہ_MINOR,
        tsk_KERNEL_نسخہ_BUILD,
        configMAX_PRIORITIES - 1,                   /* uxTopUsedPriority */
        offsetof( TCB_t, px_چوٹی_کا_ڈھیر ),            /* thread_stack_offset; */
        offsetof( TCB_t, pcTaskName ),              /* thread_name_offset; */
        حجم( List_t ),                           /* list_width */
        offsetof( List_t, ux_نمبر_کا_اشیاء ),        /* list_item_num */
        offsetof( List_t, x_List_انتہا ),               /* list_end_offset */
        offsetof( List_t, x_List_انتہا.px_پچھلا ),    /* list_next_offset */
        offsetof( List_شے_t, px_پچھلا ),         /* list_elem_next_offset */
        offsetof( List_شے_t, pvOwner ),            /* list_elem_content_offset */
        حجم( UBaseType_t ),                      /* task_count_width */
        حجم( خالی * )                            /* ptr_width */
    };

#اگر_ختم /* CONFIG_FREERTOS_DEBUG_OCDAWARE */
/*----------------------------------------------------------*/

/* ----------------------------------------------------- PSRAM ---------------------------------------------------- */

#اگر CONFIG_SPIRAM

    #اگر CONFIG_FREERTOS_SMP
        بنیاد_Type_t prv_Task_Create_Dynamic_Affinity_سیٹ_With_Caps( TaskFunction_t px_Task_رمز,
                                                            ثابت حرف * ثابت pcName,
                                                            ثابت config_ڈھیر_گہرائی_TYPE us_ڈھیر_گہرائی,
                                                            خالی * ثابت pvParameters,
                                                            UBaseType_t ux_ترجیح,
                                                            UBaseType_t ux_Core_Affinity_نقاب,
                                                            UBaseType_t ux_ڈھیر_حافظہ_Caps,
                                                            TaskHandle_t * ثابت pxCreatedTask )
    #ورنہ /* CONFIG_FREERTOS_SMP */
        بنیاد_Type_t prv_Task_Create_Dynamic_Pinned_مقصد_Core_With_Caps( TaskFunction_t px_Task_رمز,
                                                             ثابت حرف * ثابت pcName,
                                                             ثابت config_ڈھیر_گہرائی_TYPE us_ڈھیر_گہرائی,
                                                             خالی * ثابت pvParameters,
                                                             UBaseType_t ux_ترجیح,
                                                             ثابت بنیاد_Type_t xCoreID,
                                                             UBaseType_t ux_ڈھیر_حافظہ_Caps,
                                                             TaskHandle_t * ثابت pxCreatedTask )
    #اگر_ختم /* CONFIG_FREERTOS_SMP */
    {
        TCB_t * pxNewTCB;
        بنیاد_Type_t xReturn;

        ڈھیر_Type_t * px_ڈھیر;

        configASSERT( ux_ڈھیر_حافظہ_Caps & ( MALLOC_CAP_8_ثنائی_ہندسہ ) );
        configASSERT( ( ux_ڈھیر_حافظہ_Caps & MALLOC_CAP_SPIRAM ) ||
                      ( ux_ڈھیر_حافظہ_Caps & MALLOC_CAP_اندرونی ) );
        #اگر ( !CONFIG_FREERTOS_SMP )
        {
            configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );
        }
        #اگر_ختم /* !CONFIG_FREERTOS_SMP */

        /* Allocate space for the stack used by the task being created. */
        px_ڈھیر = ڈھیر_caps_malloc( ( ( ( size_t ) us_ڈھیر_گہرائی ) * حجم( ڈھیر_Type_t ) ), ux_ڈھیر_حافظہ_Caps );

        اگر( px_ڈھیر != تہی )
        {
            /* Allocate space for the TCB. */
            pxNewTCB = ( TCB_t * ) pv_پورٹ_Malloc( حجم( TCB_t ) );

            اگر( pxNewTCB != تہی )
            {
                memset( ( خالی * ) pxNewTCB, 0x00, حجم( TCB_t ) );

                /* Store the stack location in the TCB. */
                pxNewTCB->px_ڈھیر = px_ڈھیر;
            }
            ورنہ
            {
                /* The stack cannot be used as the TCB has not been created. Free it. */
                ڈھیر_caps_free( px_ڈھیر );
            }
        }
        ورنہ
        {
            pxNewTCB = تہی;
        }

        اگر( pxNewTCB != تہی )
        {
            #اگر ( tsk_STATIC_AND_DYNAMIC_تخصیص_POSSIBLE != 0 )
            {
                /* Tasks can be created statically or dynamically, so note this
                 * task was created dynamically in case it is later deleted. */
                pxNewTCB->ucStaticallyAllocated = tsk_DYNAMICALLY_ALLOCATED_ڈھیر_AND_TCB;
            }
            #اگر_ختم /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

            #اگر CONFIG_FREERTOS_SMP
            {
                prvInitialiseNewTask( px_Task_رمز, pcName, ( uint32_t ) us_ڈھیر_گہرائی, pvParameters, ux_ترجیح, pxCreatedTask, pxNewTCB, تہی );
                #اگر ( ( config_عدد_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
                {
                    /* Set the task's affinity before scheduling it */
                    pxNewTCB->ux_Core_Affinity_نقاب = ux_Core_Affinity_نقاب;
                }
                #اگر_ختم /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            }
            #ورنہ /* CONFIG_FREERTOS_SMP */
            {
                prvInitialiseNewTask( px_Task_رمز, pcName, ( uint32_t ) us_ڈھیر_گہرائی, pvParameters, ux_ترجیح, pxCreatedTask, pxNewTCB, تہی, xCoreID );
            }
            #اگر_ختم /* CONFIG_FREERTOS_SMP */

            prv_جوڑو_New_Task_مقصد_Ready_List( pxNewTCB );
            xReturn = pdPASS;
        }
        ورنہ
        {
            xReturn = err_COULD_NOT_ALLOCATE_REQUIRED_حافظہ;
        }

        واپس xReturn;
    }

#اگر_ختم /* CONFIG_SPIRAM */
/*----------------------------------------------------------*/
