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
_Static_assert( offsetof( StaticTask_t, pxDummy6 ) == offsetof( TCB_t, px_پشته ) );
_Static_assert( offsetof( StaticTask_t, pxDummy8 ) == offsetof( TCB_t, px_انتها_از_پشته ) );
#إذا !CONFIG_IDF_هدف_LINUX    // Disabled for linux builds due to differences in types
_Static_assert( tskNO_AFFINITY == ( پایه_Type_t ) CONFIG_FREERTOS_NO_AFFINITY, "CONFIG_FREERTOS_NO_AFFINITY must be the same as tskNO_AFFINITY" );
#پایان_اگر

/* ------------------------------------------------- Kernel Control ------------------------------------------------- */

#إذا ( !CONFIG_FREERTOS_SMP && ( config_رقم_CORES > 1 ) )

/*
 * Wrapper function to take "xKerneLock"
 */
    فراغ prv_Take_Kernel_قفل( فراغ )
    {
        /* We call the tasks.c critical section macro to take xKernelLock */
        taskENTER_CRITICAL( &x_Kernel_قفل );
    }

#پایان_اگر /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#إذا ( !CONFIG_FREERTOS_SMP && ( config_رقم_CORES > 1 ) )

/*
 * Wrapper function to release "xKerneLock"
 */
    فراغ prv_Release_Kernel_قفل( فراغ )
    {
        /* We call the tasks.c critical section macro to release xKernelLock */
        taskEXIT_CRITICAL( &x_Kernel_قفل );
    }

#پایان_اگر /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#إذا ( CONFIG_FREERTOS_SMP && ( config_رقم_CORES > 1 ) )

/*
 * Workaround for non-thread safe multi-core OS startup (see IDF-4524)
 */
    فراغ prv_شروع_Scheduler_Other_Cores( فراغ )
    {
        /* This function is always called with interrupts disabled*/
        xSchedulerRunning = pdTRUE;
    }

#پایان_اگر /* ( CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#إذا ( !CONFIG_FREERTOS_SMP && ( config_رقم_CORES > 1 ) )

    پایه_Type_t xTaskIncrementTickOtherCores( فراغ )
    {
        /* Minor optimization. This function can never switch cores mid
         * execution */
        پایه_Type_t xCoreID = پورت_GET_CORE_ID();
        پایه_Type_t xSwitchRequired = pdFALSE;

        /* This function should never be called by Core 0. */
        configASSERT( xCoreID != 0 );

        /* Called by the portable layer each time a tick interrupt occurs
         * on a core other than core 0. */
        traceTASK_INCREMENT_TICK( x_Tick_شمار );

        إذا( uxSchedulerSuspended[ xCoreID ] == ( UBaseType_t ) 0U )
        {
            /* We need take the kernel lock here as we are about to access
             * kernel data structures. */
            taskENTER_CRITICAL_ISR( &x_Kernel_قفل );

            /* Tasks of equal priority to the currently running task will share
             * processing time (time slice) if preemption is on, and the application
             * writer has not explicitly turned time slicing off. */
            #إذا ( ( configUSE_PREEMPTION == 1 ) && ( config_USE_زمان_SLICING == 1 ) )
            {
                إذا( list_CURRENT_LIST_درازی( &( pxReadyTasksLists[ pxCurrentTCBs[ xCoreID ]->ux_اولویت ] ) ) > ( UBaseType_t ) 1 )
                {
                    xSwitchRequired = pdTRUE;
                }
                وإلا
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            #پایان_اگر /* ( ( configUSE_PREEMPTION == 1 ) && ( configUSE_TIME_SLICING == 1 ) ) */

            /* Release the previously taken kernel lock as we have finished
             * accessing the kernel data structures. */
            taskEXIT_CRITICAL_ISR( &x_Kernel_قفل );

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
            #پایان_اگر /* configUSE_PREEMPTION */
        }

        #إذا ( configUSE_TICK_HOOK == 1 )
        {
            vApplicationTickHook();
        }
        #پایان_اگر

        أرجع xSwitchRequired;
    }

#پایان_اگر /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

/* -------------------------------------------------- Task Creation ------------------------------------------------- */

#إذا ( config_SUPPORT_DYNAMIC_تخصیص == 1 )

    پایه_Type_t x_Task_Create_Pinned_مقصد_Core( TaskFunction_t px_Task_کد,
                                        ثابت حرف * ثابت pcName,
                                        ثابت uint32_t us_پشته_عمق,
                                        فراغ * ثابت pvParameters,
                                        UBaseType_t ux_اولویت,
                                        TaskHandle_t * ثابت pxCreatedTask,
                                        ثابت پایه_Type_t xCoreID )
    {
        پایه_Type_t xReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );

        #إذا CONFIG_FREERTOS_SMP
        {
            /* If using Amazon SMP FreeRTOS. This function is just a wrapper around
             * xTaskCreate() or xTaskCreateAffinitySet(). */
            #إذا ( ( config_رقم_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
            {
                /* Convert xCoreID into an affinity mask */
                UBaseType_t ux_Core_Affinity_ماسک;

                /* Bit shifting << xCoreID is only valid if we have less than
                 * 32 cores. */
                ESP_STATIC_ASSERT( config_رقم_CORES < 32 );

                إذا( xCoreID == tskNO_AFFINITY )
                {
                    ux_Core_Affinity_ماسک = tskNO_AFFINITY;
                }
                وإلا
                {
                    ux_Core_Affinity_ماسک = ( 1 << xCoreID );
                }

                xReturn = x_Task_Create_Affinity_مجموعه( px_Task_کد, pcName, us_پشته_عمق, pvParameters, ux_اولویت, ux_Core_Affinity_ماسک, pxCreatedTask );
            }
            #وإلا /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            {
                xReturn = xTaskCreate( px_Task_کد, pcName, us_پشته_عمق, pvParameters, ux_اولویت, pxCreatedTask );
            }
            #پایان_اگر /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
        }
        #وإلا /* CONFIG_FREERTOS_SMP */
        {
            TCB_t * pxNewTCB;

            /* If the stack grows down then allocate the stack then the TCB so the
             * stack does not grow into the TCB.  Likewise if the stack grows up
             * then allocate the TCB then the stack. */
            #إذا ( پورت_پشته_GROWTH > 0 )
            {
                /* Allocate space for the TCB.  Where the memory comes from depends on
                 * the implementation of the port malloc function and whether or not static
                 * allocation is being used. */
                pxNewTCB = ( TCB_t * ) pv_پورت_Malloc( حجم( TCB_t ) );

                إذا( pxNewTCB != عدم )
                {
                    memset( ( فراغ * ) pxNewTCB, 0x00, حجم( TCB_t ) );

                    /* Allocate space for the stack used by the task being created.
                     * The base of the stack memory stored in the TCB so the task can
                     * be deleted later if required. */
                    pxNewTCB->px_پشته = ( پشته_Type_t * ) pv_پورت_Malloc_پشته( ( ( ( size_t ) us_پشته_عمق ) * حجم( پشته_Type_t ) ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */

                    إذا( pxNewTCB->px_پشته == عدم )
                    {
                        /* Could not allocate the stack.  Delete the allocated TCB. */
                        v_پورت_Free( pxNewTCB );
                        pxNewTCB = عدم;
                    }
                }
            }
            #وإلا /* portSTACK_GROWTH */
            {
                پشته_Type_t * px_پشته;

                /* Allocate space for the stack used by the task being created. */
                px_پشته = pv_پورت_Malloc_پشته( ( ( ( size_t ) us_پشته_عمق ) * حجم( پشته_Type_t ) ) ); /*lint !e9079 All values returned by pvPortMalloc() have at least the alignment required by the MCU's stack and this allocation is the stack. */

                إذا( px_پشته != عدم )
                {
                    /* Allocate space for the TCB. */
                    pxNewTCB = ( TCB_t * ) pv_پورت_Malloc( حجم( TCB_t ) ); /*lint !e9087 !e9079 All values returned by pvPortMalloc() have at least the alignment required by the MCU's stack, and the first member of TCB_t is always a pointer to the task's stack. */

                    إذا( pxNewTCB != عدم )
                    {
                        memset( ( فراغ * ) pxNewTCB, 0x00, حجم( TCB_t ) );

                        /* Store the stack location in the TCB. */
                        pxNewTCB->px_پشته = px_پشته;
                    }
                    وإلا
                    {
                        /* The stack cannot be used as the TCB was not created.  Free
                         * it again. */
                        v_پورت_Free_پشته( px_پشته );
                    }
                }
                وإلا
                {
                    pxNewTCB = عدم;
                }
            }
            #پایان_اگر /* portSTACK_GROWTH */

            إذا( pxNewTCB != عدم )
            {
                #إذا ( tsk_STATIC_AND_DYNAMIC_تخصیص_POSSIBLE != 0 ) /*lint !e9029 !e731 Macro has been consolidated for readability reasons. */
                {
                    /* Tasks can be created statically or dynamically, so note this
                     * task was created dynamically in case it is later deleted. */
                    pxNewTCB->ucStaticallyAllocated = tsk_DYNAMICALLY_ALLOCATED_پشته_AND_TCB;
                }
                #پایان_اگر /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

                prvInitialiseNewTask( px_Task_کد, pcName, ( uint32_t ) us_پشته_عمق, pvParameters, ux_اولویت, pxCreatedTask, pxNewTCB, عدم, xCoreID );
                prv_جمع_کن_New_Task_مقصد_Ready_List( pxNewTCB );
                xReturn = pdPASS;
            }
            وإلا
            {
                xReturn = err_COULD_NOT_ALLOCATE_REQUIRED_حافظه;
            }
        }
        #پایان_اگر /* CONFIG_FREERTOS_SMP */

        أرجع xReturn;
    }

#پایان_اگر /* ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#إذا ( config_SUPPORT_STATIC_تخصیص == 1 )

    TaskHandle_t x_Task_Create_Static_Pinned_مقصد_Core( TaskFunction_t px_Task_کد,
                                                ثابت حرف * ثابت pcName,
                                                ثابت uint32_t ul_پشته_عمق,
                                                فراغ * ثابت pvParameters,
                                                UBaseType_t ux_اولویت,
                                                پشته_Type_t * ثابت pux_پشته_میانگیر,
                                                StaticTask_t * ثابت px_Task_میانگیر,
                                                ثابت پایه_Type_t xCoreID )
    {
        TaskHandle_t xReturn;

        configASSERT( پورت_VALID_پشته_MEM( pux_پشته_میانگیر ) );
        configASSERT( پورت_VALID_TCB_MEM( px_Task_میانگیر ) );
        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );

        #إذا CONFIG_FREERTOS_SMP
        {
            /* If using Amazon SMP FreeRTOS. This function is just a wrapper around
             * xTaskCreateStatic() or xTaskCreateStaticAffinitySet(). */
            #إذا ( ( config_رقم_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
            {
                /* Convert xCoreID into an affinity mask */
                UBaseType_t ux_Core_Affinity_ماسک;

                /* Bit shifting << xCoreID is only valid if we have less than
                 * 32 cores. */
                ESP_STATIC_ASSERT( config_رقم_CORES < 32 );

                إذا( xCoreID == tskNO_AFFINITY )
                {
                    ux_Core_Affinity_ماسک = tskNO_AFFINITY;
                }
                وإلا
                {
                    ux_Core_Affinity_ماسک = ( 1 << xCoreID );
                }

                xReturn = x_Task_Create_Static_Affinity_مجموعه( px_Task_کد, pcName, ul_پشته_عمق, pvParameters, ux_اولویت, pux_پشته_میانگیر, px_Task_میانگیر, ux_Core_Affinity_ماسک );
            }
            #وإلا /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            {
                xReturn = xTaskCreateStatic( px_Task_کد, pcName, ul_پشته_عمق, pvParameters, ux_اولویت, pux_پشته_میانگیر, px_Task_میانگیر );
            }
            #پایان_اگر /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
        }
        #وإلا /* CONFIG_FREERTOS_SMP */
        {
            TCB_t * pxNewTCB;

            #إذا ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticTask_t equals the size of the real task
                 * structure. */
                متطاير size_t x_اندازه = حجم( StaticTask_t );
                configASSERT( x_اندازه == حجم( TCB_t ) );
                ( فراغ ) x_اندازه; /* Prevent lint warning when configASSERT() is not used. */
            }
            #پایان_اگر /* configASSERT_DEFINED */

            إذا( ( px_Task_میانگیر != عدم ) && ( pux_پشته_میانگیر != عدم ) )
            {
                /* The memory used for the task's TCB and stack are passed into this
                 * function - use them. */
                pxNewTCB = ( TCB_t * ) px_Task_میانگیر; /*lint !e740 !e9087 Unusual cast is ok as the structures are designed to have the same alignment, and the size is checked by an assert. */
                memset( ( فراغ * ) pxNewTCB, 0x00, حجم( TCB_t ) );
                pxNewTCB->px_پشته = ( پشته_Type_t * ) pux_پشته_میانگیر;

                #إذا ( tsk_STATIC_AND_DYNAMIC_تخصیص_POSSIBLE != 0 ) /*lint !e731 !e9029 Macro has been consolidated for readability reasons. */
                {
                    /* Tasks can be created statically or dynamically, so note this
                     * task was created statically in case the task is later deleted. */
                    pxNewTCB->ucStaticallyAllocated = tsk_STATICALLY_ALLOCATED_پشته_AND_TCB;
                }
                #پایان_اگر /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

                prvInitialiseNewTask( px_Task_کد, pcName, ul_پشته_عمق, pvParameters, ux_اولویت, &xReturn, pxNewTCB, عدم, xCoreID );
                prv_جمع_کن_New_Task_مقصد_Ready_List( pxNewTCB );
            }
            وإلا
            {
                xReturn = عدم;
            }
        }
        #پایان_اگر /* CONFIG_FREERTOS_SMP */

        أرجع xReturn;
    }

#پایان_اگر /* ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
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
    پایه_Type_t __attribute__( ( weak ) ) xTimerCreateTimerTask( فراغ )
    {
        أرجع pdPASS;
    }

#پایان_اگر /* configUSE_TIMERS */
/*----------------------------------------------------------*/

/* ------------------------------------------------- Task Utilities ------------------------------------------------- */

پایه_Type_t xTaskGetCoreID( TaskHandle_t xTask )
{
    پایه_Type_t xReturn;

    #إذا ( config_رقم_CORES > 1 )
    {
        #إذا CONFIG_FREERTOS_SMP
            UBaseType_t ux_Core_Affinity_ماسک;

            /* Get the core affinity mask and convert it to an ID */
            ux_Core_Affinity_ماسک = vTaskCoreAffinityGet( xTask );

            /* If the task is not pinned to a particular core, treat it as tskNO_AFFINITY */
            إذا( ux_Core_Affinity_ماسک & ( ux_Core_Affinity_ماسک - 1 ) ) /* If more than one bit set */
            {
                xReturn = tskNO_AFFINITY;
            }
            وإلا
            {
                صحيح i_شاخص_Plus_One = __builtin_ffs( ux_Core_Affinity_ماسک );
                أكد( i_شاخص_Plus_One >= 1 );
                xReturn = i_شاخص_Plus_One - 1;
            }
        #وإلا /* CONFIG_FREERTOS_SMP */
            TCB_t * pxTCB;

            pxTCB = prvGetTCBFromHandle( xTask );

            xReturn = pxTCB->xCoreID;
        #پایان_اگر /* CONFIG_FREERTOS_SMP */
    }
    #وإلا /* configNUM_CORES > 1 */
    {
        /* Single-core. Just return a core ID of 0 */
        xReturn = 0;
    }
    #پایان_اگر /* configNUM_CORES > 1 */

    أرجع xReturn;
}
/*----------------------------------------------------------*/

#إذا ( ( !CONFIG_FREERTOS_SMP ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

    TaskHandle_t xTaskGetIdleTaskHandleForCore( پایه_Type_t xCoreID )
    {
        /* If xTaskGetIdleTaskHandle() is called before the scheduler has been
         * started, then xIdleTaskHandle will be NULL. */
        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );
        configASSERT( ( xIdleTaskHandle[ xCoreID ] != عدم ) );
        أرجع xIdleTaskHandle[ xCoreID ];
    }

#پایان_اگر /* ( ( !CONFIG_FREERTOS_SMP ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*----------------------------------------------------------*/

#إذا ( ( !CONFIG_FREERTOS_SMP ) && ( ( INCLUDE_xTaskGetCurrentTaskHandle == 1 ) || ( configUSE_MUTEXES == 1 ) ) )

    TaskHandle_t xTaskGetCurrentTaskHandleForCore( پایه_Type_t xCoreID )
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
        #پایان_اگر /* CONFIG_FREERTOS_SMP */

        أرجع xReturn;
    }

#پایان_اگر /* ( ( !CONFIG_FREERTOS_SMP ) && ( ( INCLUDE_xTaskGetCurrentTaskHandle == 1 ) || ( configUSE_MUTEXES == 1 ) ) ) */
/*----------------------------------------------------------*/

#إذا ( !CONFIG_FREERTOS_SMP && ( config_GENERATE_پیاپی_زمان_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

    config_پیاپی_زمان_COUNTER_TYPE ul_Task_Get_Idle_پیاپی_زمان_Counter_For_Core( پایه_Type_t xCoreID )
    {
        config_پیاپی_زمان_COUNTER_TYPE ul_پیاپی_زمان_Counter;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

        /* For SMP, we need to take the kernel lock here as we are about to
         * access kernel data structures. */
        prvENTER_CRITICAL_SMP_ONLY( &x_Kernel_قفل );
        {
            ul_پیاپی_زمان_Counter = xIdleTaskHandle[ xCoreID ]->ul_پیاپی_زمان_Counter;
        }
        /* Release the previously taken kernel lock. */
        prvEXIT_CRITICAL_SMP_ONLY( &x_Kernel_قفل );

        أرجع ul_پیاپی_زمان_Counter;
    }

#پایان_اگر /* ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*----------------------------------------------------------*/

#إذا ( !CONFIG_FREERTOS_SMP && ( config_GENERATE_پیاپی_زمان_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

    config_پیاپی_زمان_COUNTER_TYPE ul_Task_Get_Idle_پیاپی_زمان_Percent_For_Core( پایه_Type_t xCoreID )
    {
        config_پیاپی_زمان_COUNTER_TYPE ul_مجموع_زمان, ulReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

        #اگر_تعریف_شده پورت_ALT_GET_پیاپی_زمان_COUNTER_مقدار
            پورت_ALT_GET_پیاپی_زمان_COUNTER_مقدار( ul_مجموع_زمان );
        #وإلا
            ul_مجموع_زمان = پورت_GET_پیاپی_زمان_COUNTER_مقدار();
        #پایان_اگر

        /* For percentage calculations. */
        ul_مجموع_زمان /= ( config_پیاپی_زمان_COUNTER_TYPE ) 100;

        /* Avoid divide by zero errors. */
        إذا( ul_مجموع_زمان > ( config_پیاپی_زمان_COUNTER_TYPE ) 0 )
        {
            /* For SMP, we need to take the kernel lock here as we are about
             * to access kernel data structures. */
            prvENTER_CRITICAL_SMP_ONLY( &x_Kernel_قفل );
            {
                ulReturn = xIdleTaskHandle[ xCoreID ]->ul_پیاپی_زمان_Counter / ul_مجموع_زمان;
            }
            /* Release the previously taken kernel lock. */
            prvEXIT_CRITICAL_SMP_ONLY( &x_Kernel_قفل );
        }
        وإلا
        {
            ulReturn = 0;
        }

        أرجع ulReturn;
    }

#پایان_اگر /* ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*-----------------------------------------------------------*/

پشته_Type_t * x_Task_Get_پشته_شروع( TaskHandle_t xTask )
{
    TCB_t * pxTCB;

    pxTCB = prvGetTCBFromHandle( xTask );
    أرجع pxTCB->px_پشته;
}
/*----------------------------------------------------------*/

uint8_t * px_Task_Get_پشته_شروع( TaskHandle_t xTask )
{
    أرجع (uint8_t *)x_Task_Get_پشته_شروع( xTask );
}
/*----------------------------------------------------------*/

#إذا ( INCLUDE_v_Task_اولویت_مجموعه == 1 )

    فراغ prv_Task_اولویت_Raise( prv_Task_Saved_اولویت_t * px_Saved_اولویت,
                               UBaseType_t ux_New_اولویت )
    {
        TCB_t * pxTCB;
        UBaseType_t ux_اولویت_استفاده_شده_On_مدخل;

        configASSERT( ( ux_New_اولویت < configMAX_PRIORITIES ) );

        /* Ensure the new priority is valid. */
        إذا( ux_New_اولویت >= ( UBaseType_t ) configMAX_PRIORITIES )
        {
            ux_New_اولویت = ( UBaseType_t ) configMAX_PRIORITIES - ( UBaseType_t ) 1U;
        }

        #إذا CONFIG_FREERTOS_SMP
            taskENTER_CRITICAL();
        #وإلا
            taskENTER_CRITICAL( &x_Kernel_قفل );
        #پایان_اگر
        {
            pxTCB = prvGetTCBFromHandle( عدم );

            #إذا ( configUSE_MUTEXES == 1 )
            {
                px_Saved_اولویت->ux_اولویت = pxTCB->ux_اولویت;
                px_Saved_اولویت->ux_پایه_اولویت = pxTCB->ux_پایه_اولویت;

                /* If uxNewPriority < uxBasePriority, then there is nothing else to
                 * do, as uxBasePriority is always <= uxPriority. */
                إذا( ux_New_اولویت > pxTCB->ux_پایه_اولویت )
                {
                    pxTCB->ux_پایه_اولویت = ux_New_اولویت;

                    /* Remember the task's current priority before attempting to
                     * change it. If the task's current priority is changed, it must
                     * be done so before moving the task between task lists) in order
                     * for the taskRESET_READY_PRIORITY() macro to function correctly. */
                    ux_اولویت_استفاده_شده_On_مدخل = pxTCB->ux_اولویت;

                    إذا( ux_New_اولویت > pxTCB->ux_اولویت )
                    {
                        pxTCB->ux_اولویت = ux_New_اولویت;

                        /* Only reset the event list item value if the value is not
                         * being used for anything else. */
                        إذا( ( list_GET_LIST_قلم_مقدار( &( pxTCB->x_Event_List_قلم ) ) & task_EVENT_LIST_قلم_مقدار_درون_USE ) == 0UL )
                        {
                            list_مجموعه_LIST_قلم_مقدار( &( pxTCB->x_Event_List_قلم ), ( ( TickType_t ) configMAX_PRIORITIES - ( TickType_t ) ux_New_اولویت ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */
                        }

                        /* If the task is in the blocked or suspended list we need do
                         * nothing more than change its priority variable. However, if
                         * the task is in a ready list it needs to be removed and placed
                         * in the list appropriate to its new priority. */
                        إذا( listIS_CONTAINED_WITHIN( &( pxReadyTasksLists[ ux_اولویت_استفاده_شده_On_مدخل ] ), &( pxTCB->x_State_List_قلم ) ) != pdFALSE )
                        {
                            /* The task is currently in its ready list - remove before
                             * adding it to its new ready list.  As we are in a critical
                             * section we can do this even if the scheduler is suspended. */
                            إذا( uxListRemove( &( pxTCB->x_State_List_قلم ) ) == ( UBaseType_t ) 0 )
                            {
                                /* It is known that the task is in its ready list so
                                 * there is no need to check again and the port level
                                 * reset macro can be called directly. */
                                پورت_RESET_READY_اولویت( ux_اولویت_استفاده_شده_On_مدخل, ux_بالاترین_Ready_اولویت );
                            }

                            prv_جمع_کن_Task_مقصد_Ready_List( pxTCB );
                        }
                    }
                }
            }
            #وإلا /* if ( configUSE_MUTEXES == 1 ) */
            {
                px_Saved_اولویت->ux_اولویت = pxTCB->ux_اولویت;

                إذا( ux_New_اولویت > pxTCB->ux_اولویت )
                {
                    v_Task_اولویت_مجموعه( عدم, ux_New_اولویت );
                }
            }
            #پایان_اگر /* if ( configUSE_MUTEXES == 1 ) */
        }
        #إذا CONFIG_FREERTOS_SMP
            taskEXIT_CRITICAL();
        #وإلا
            taskEXIT_CRITICAL( &x_Kernel_قفل );
        #پایان_اگر
    }

#پایان_اگر /* INCLUDE_vTaskPrioritySet == 1 */
/*----------------------------------------------------------*/

#إذا ( INCLUDE_v_Task_اولویت_مجموعه == 1 )

    فراغ prv_Task_اولویت_Restore( prv_Task_Saved_اولویت_t * px_Saved_اولویت )
    {
        TCB_t * pxTCB;
        UBaseType_t ux_New_اولویت;
        UBaseType_t ux_اولویت_استفاده_شده_On_مدخل;
        UBaseType_t ux_پایه_اولویت_استفاده_شده_On_مدخل;
        پایه_Type_t xYieldRequired = pdFALSE;

        #إذا CONFIG_FREERTOS_SMP
            taskENTER_CRITICAL();
        #وإلا
            taskENTER_CRITICAL( &x_Kernel_قفل );
        #پایان_اگر
        {
            pxTCB = prvGetTCBFromHandle( عدم );

            #إذا ( configUSE_MUTEXES == 1 )
            {
                /* If the saved uxBasePriority == the task's uxBasePriority, it means
                 * that prvTaskPriorityRaise() never raised the task's uxBasePriority.
                 * In that case, there is nothing else to do. */
                إذا( px_Saved_اولویت->ux_پایه_اولویت != pxTCB->ux_پایه_اولویت )
                {
                    ux_پایه_اولویت_استفاده_شده_On_مدخل = pxTCB->ux_پایه_اولویت;
                    pxTCB->ux_پایه_اولویت = px_Saved_اولویت->ux_پایه_اولویت;

                    /* Remember the task's current priority before attempting to
                     * change it. If the task's current priority is changed, it must
                     * be done so before moving the task between task lists in order
                     * for the taskRESET_READY_PRIORITY() macro to function correctly. */
                    ux_اولویت_استفاده_شده_On_مدخل = pxTCB->ux_اولویت;

                    /* Check if the task inherited a priority after prvTaskPriorityRaise().
                     * If this is the case, there is nothing else to do. The priority
                     * will be restored when the task disinherits its priority. */
                    إذا( pxTCB->ux_اولویت == ux_پایه_اولویت_استفاده_شده_On_مدخل )
                    {
                        إذا( pxTCB->uxMutexesHeld == 0 )
                        {
                            /* The task may have inherited a priority before prvTaskPriorityRaise()
                             * then disinherited a priority after prvTaskPriorityRaise().
                             * Thus we need set the uxPriority to the saved base priority
                             * so that the task's priority gets restored to the priority
                             * before any inheritance or raising. */
                            pxTCB->ux_اولویت = px_Saved_اولویت->ux_پایه_اولویت;
                        }
                        وإلا
                        {
                            /* The task may have inherited a priority before prvTaskPriorityRaise()
                             * was called. Thus, we need to restore uxPriority to the
                             * "saved uxPriority" so that the task still retains that
                             * inherited priority. */
                            pxTCB->ux_اولویت = px_Saved_اولویت->ux_اولویت;
                        }

                        ux_New_اولویت = pxTCB->ux_اولویت;

                        إذا( ux_New_اولویت < ux_اولویت_استفاده_شده_On_مدخل )
                        {
                            /* Setting the priority of the running task down means
                             * there may now be another task of higher priority that
                             * is ready to execute. */
                            xYieldRequired = pdTRUE;
                        }

                        /* Only reset the event list item value if the value is not
                         * being used for anything else. */
                        إذا( ( list_GET_LIST_قلم_مقدار( &( pxTCB->x_Event_List_قلم ) ) & task_EVENT_LIST_قلم_مقدار_درون_USE ) == 0UL )
                        {
                            list_مجموعه_LIST_قلم_مقدار( &( pxTCB->x_Event_List_قلم ), ( ( TickType_t ) configMAX_PRIORITIES - ( TickType_t ) ux_New_اولویت ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */
                        }

                        /* If the task is in the blocked or suspended list we need do
                         * nothing more than change its priority variable. However, if
                         * the task is in a ready list it needs to be removed and placed
                         * in the list appropriate to its new priority. */
                        إذا( listIS_CONTAINED_WITHIN( &( pxReadyTasksLists[ ux_اولویت_استفاده_شده_On_مدخل ] ), &( pxTCB->x_State_List_قلم ) ) != pdFALSE )
                        {
                            /* The task is currently in its ready list - remove before
                             * adding it to its new ready list.  As we are in a critical
                             * section we can do this even if the scheduler is suspended. */
                            إذا( uxListRemove( &( pxTCB->x_State_List_قلم ) ) == ( UBaseType_t ) 0 )
                            {
                                /* It is known that the task is in its ready list so
                                 * there is no need to check again and the port level
                                 * reset macro can be called directly. */
                                پورت_RESET_READY_اولویت( ux_اولویت_استفاده_شده_On_مدخل, ux_بالاترین_Ready_اولویت );
                            }

                            prv_جمع_کن_Task_مقصد_Ready_List( pxTCB );
                        }

                        إذا( xYieldRequired != pdFALSE )
                        {
                            #إذا CONFIG_FREERTOS_SMP
                                taskYIELD_TASK_CORE_IF_USING_PREEMPTION( pxTCB );
                            #وإلا
                                taskYIELD_IF_USING_PREEMPTION();
                            #پایان_اگر
                        }
                    }
                }
            }
            #وإلا /* if ( configUSE_MUTEXES == 1 ) */
            {
                v_Task_اولویت_مجموعه( عدم, px_Saved_اولویت->ux_اولویت );
            }
            #پایان_اگر /* if ( configUSE_MUTEXES == 1 ) */
        }
        #إذا CONFIG_FREERTOS_SMP
            taskEXIT_CRITICAL();
        #وإلا
            taskEXIT_CRITICAL( &x_Kernel_قفل );
        #پایان_اگر
    }

#پایان_اگر /* ( INCLUDE_vTaskPrioritySet == 1 ) */
/*----------------------------------------------------------*/

/* --------------------------------------------- TLSP Deletion Callbacks -------------------------------------------- */

#إذا CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS

    فراغ v_Task_مجموعه_Thread_Local_Storage_اشاره_گر_And_Del_Callback( TaskHandle_t x_Task_مقصد_مجموعه,
                                                          پایه_Type_t x_شاخص,
                                                          فراغ * pv_مقدار,
                                                          TlsDeleteCallbackFunction_t pvDelCallback )
    {
        /* If TLSP deletion callbacks are enabled, then configNUM_THREAD_LOCAL_STORAGE_POINTERS
         * is doubled in size so that the latter half of the pvThreadLocalStoragePointers
         * stores the deletion callbacks. */
        إذا( x_شاخص < ( config_رقم_THREAD_LOCAL_STORAGE_اشارهگرها / 2 ) )
        {
            TCB_t * pxTCB;

            #إذا ( config_رقم_CORES > 1 )
            {
                /* For SMP, we need a critical section as another core could also
                 * update this task's TLSP at the same time. */
                #إذا CONFIG_FREERTOS_SMP
                {
                    taskENTER_CRITICAL();
                }
                #وإلا /* CONFIG_FREERTOS_SMP */
                {
                    taskENTER_CRITICAL( &x_Kernel_قفل );
                }
                #پایان_اگر /* CONFIG_FREERTOS_SMP */
            }
            #پایان_اگر /* configNUM_CORES > 1 */

            pxTCB = prvGetTCBFromHandle( x_Task_مقصد_مجموعه );
            /* Store the TLSP by indexing the first half of the array */
            pxTCB->pv_Thread_Local_Storage_اشارهگرها[ x_شاخص ] = pv_مقدار;

            /* Store the TLSP deletion callback by indexing the second half
             * of the array. */
            pxTCB->pv_Thread_Local_Storage_اشارهگرها[ ( x_شاخص + ( config_رقم_THREAD_LOCAL_STORAGE_اشارهگرها / 2 ) ) ] = ( فراغ * ) pvDelCallback;

            #إذا ( config_رقم_CORES > 1 )
            {
                #إذا CONFIG_FREERTOS_SMP
                {
                    taskEXIT_CRITICAL();
                }
                #وإلا /* CONFIG_FREERTOS_SMP */
                {
                    taskEXIT_CRITICAL( &x_Kernel_قفل );
                }
                #پایان_اگر /* CONFIG_FREERTOS_SMP */
            }
            #پایان_اگر /* configNUM_CORES > 1 */
        }
    }

#پایان_اگر /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */
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

#پایان_اگر /* configUSE_NEWLIB_REENTRANT == 1 */

/* -------------------------------------------------- Task Snapshot ------------------------------------------------- */

/**
 * @brief List of all task lists in FreeRTOS
 *
 * @note There are currently differing number of task list between SMP FreeRTOS and ESP-IDF FreeRTOS
 */
ساكن List_t * non_ready_task_lists[] =
{
    #اگر_تعریف_شده CONFIG_FREERTOS_SMP
        &xPendingReadyList,
    #وإلا /* CONFIG_FREERTOS_SMP */
        &xPendingReadyList[ 0 ],
        #اگر_تعریف_نشده CONFIG_FREERTOS_UNICORE
            &xPendingReadyList[ 1 ],
        #پایان_اگر /* CONFIG_FREERTOS_UNICORE */
    #پایان_اگر /* CONFIG_FREERTOS_SMP */
    &xDelayedTaskList1,
    &xDelayedTaskList2,
    #إذا ( INCLUDE_vTaskDelete == 1 )
        &x_Tasks_انتظار_Termination,
    #پایان_اگر
    #إذا ( INCLUDE_vTaskSuspend == 1 )
        &xSuspendedTaskList,
    #پایان_اگر
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
ساكن List_t * px_Get_Task_List_By_شاخص( UBaseType_t ux_List_شاخص )
{
    List_t * pxTaskList;
    ثابت size_t xNonReadyTaskListsCnt = ( حجم( non_ready_task_lists ) / حجم( List_t * ) );

    إذا( ux_List_شاخص < configMAX_PRIORITIES )
    {
        pxTaskList = &pxReadyTasksLists[ configMAX_PRIORITIES - 1 - ux_List_شاخص ];
    }
    وإلا إذا( ux_List_شاخص < configMAX_PRIORITIES + xNonReadyTaskListsCnt )
    {
        pxTaskList = non_ready_task_lists[ ux_List_شاخص - configMAX_PRIORITIES ];
    }
    وإلا
    {
        pxTaskList = عدم;
    }

    /* sanity check */
    إذا( pxTaskList )
    {
        إذا( !پورت_VALID_LIST_MEM( pxTaskList ) )
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
ساكن مضمن UBaseType_t px_Get_Task_List_شمار( فراغ )
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
صحيح x_Task_Get_بعدی( TaskIterator_t * xIterator )
{
    إذا( !xIterator )
    {
        أرجع -1;
    }

    List_قلم_t * px_بعدی_List_قلم = xIterator->px_بعدی_List_قلم;
    UBaseType_t uxCurListIdx = xIterator->ux_Current_List_شاخص;
    UBaseType_t uxMaxListIdx = px_Get_Task_List_شمار();

    لكل( ; uxCurListIdx < uxMaxListIdx; ++uxCurListIdx )
    {
        List_t * pxCurrentTaskList = px_Get_Task_List_By_شاخص( uxCurListIdx );

        إذا( !pxCurrentTaskList || ( list_CURRENT_LIST_درازی( pxCurrentTaskList ) == 0 ) )
        {
            استمر;
        }

        ثابت List_قلم_t * px_Curr_List_قلم = list_GET_انتها_MARKER( pxCurrentTaskList );

        إذا( !px_بعدی_List_قلم )
        {
            /* We are here if the traversal starts from the beginning or when we finish traversing
             * for one of the state lists
             */
            px_بعدی_List_قلم = list_GET_بعدی( px_Curr_List_قلم );
        }

        إذا( !پورت_VALID_LIST_MEM( px_بعدی_List_قلم ) )
        {
            /* Nothing to do with the corrupted list item. We will skip to the next task state list.
             * pxNextListItem should be NULL at the beginning of each task list.
             */
            px_بعدی_List_قلم = عدم;
            استمر;
        }

        TCB_t * pxTCB = ( TCB_t * ) list_GET_LIST_قلم_OWNER( px_بعدی_List_قلم );

        إذا( !پورت_VALID_TCB_MEM( pxTCB ) )
        {
            pxTCB = عدم;
        }

        xIterator->pxTaskHandle = pxTCB;
        xIterator->ux_Current_List_شاخص = uxCurListIdx;

        إذا( px_Curr_List_قلم->px_قبلی == px_بعدی_List_قلم )
        {
            /* If this is the last item of the current state list */
            xIterator->ux_Current_List_شاخص++;
            xIterator->px_بعدی_List_قلم = عدم;
        }
        وإلا
        {
            xIterator->px_بعدی_List_قلم = list_GET_بعدی( px_بعدی_List_قلم );
        }

        أرجع uxCurListIdx;
    }

    أرجع -1; /* end of the task list */
}
/*----------------------------------------------------------*/

پایه_Type_t vTaskGetSnapshot( TaskHandle_t pxTask,
                             TaskSnapshot_t * pxTaskSnapshot )
{
    ESP_STATIC_ANALYZER_CHECK(!pxTask, pdFALSE);

    إذا( ( پورت_VALID_TCB_MEM( pxTask ) == false ) || ( pxTaskSnapshot == عدم ) )
    {
        أرجع pdFALSE;
    }

    TCB_t * pxTCB = ( TCB_t * ) pxTask;
    pxTaskSnapshot->pxTCB = pxTCB;
    pxTaskSnapshot->px_بالاترین_از_پشته = ( پشته_Type_t * ) pxTCB->px_بالاترین_از_پشته;
    pxTaskSnapshot->px_انتها_از_پشته = ( پشته_Type_t * ) pxTCB->px_انتها_از_پشته;
    أرجع pdTRUE;
}
/*----------------------------------------------------------*/

UBaseType_t uxTaskGetSnapshotAll( TaskSnapshot_t * ثابت pxTaskSnapshotArray,
                                  ثابت UBaseType_t ux_Array_درازی,
                                  UBaseType_t * ثابت pxTCBSize )
{
    UBaseType_t ux_Array_رقم_Filled = 0;

    /* Traverse all of the tasks lists */
    TaskIterator_t xTaskIter = { 0 }; /* Point to the first task list */

    طالما( x_Task_Get_بعدی( &xTaskIter ) != -1 && ux_Array_رقم_Filled < ux_Array_درازی )
    {
        vTaskGetSnapshot( xTaskIter.pxTaskHandle, &pxTaskSnapshotArray[ ux_Array_رقم_Filled ] );
        ux_Array_رقم_Filled++;
    }

    إذا( pxTCBSize != عدم )
    {
        *pxTCBSize = حجم( TCB_t );
    }

    أرجع ux_Array_رقم_Filled;
}
/*----------------------------------------------------------*/

/* ----------------------------------------------------- Misc ----------------------------------------------------- */

فراغ * pvTaskGetCurrentTCBForCore( پایه_Type_t xCoreID )
{
    فراغ * pvRet;

    configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

    #إذا CONFIG_FREERTOS_SMP
        /* SMP FreeRTOS defines pxCurrentTCB as a macro function call */
        pvRet = ( فراغ * ) pxCurrentTCB;
    #وإلا /* CONFIG_FREERTOS_SMP */
        pvRet = ( فراغ * ) pxCurrentTCBs[ xCoreID ];
    #پایان_اگر /* CONFIG_FREERTOS_SMP */
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
        ESP_FREERTOS_DEBUG_جدول_اندازه = 0,
        ESP_FREERTOS_DEBUG_جدول_نسخه,
        ESP_FREERTOS_DEBUG_KERNEL_VER_MAJOR,
        ESP_FREERTOS_DEBUG_KERNEL_VER_MINOR,
        ESP_FREERTOS_DEBUG_KERNEL_VER_BUILD,
        ESP_FREERTOS_DEBUG_UX_بالاترین_استفاده_شده_PIORITY,
        ESP_FREERTOS_DEBUG_PX_بالاترین_از_پشته,
        ESP_FREERTOS_DEBUG_PC_TASK_NAME,
        ESP_FREERTOS_DEBUG_LIST_اندازه,
        ESP_FREERTOS_DEBUG_LIST_رقم_اقلام,
        ESP_FREERTOS_DEBUG_LIST_انتها,
        ESP_FREERTOS_DEBUG_LIST_انتها_PREV,
        ESP_FREERTOS_DEBUG_LIST_قلم_PREV,
        ESP_FREERTOS_DEBUG_LIST_قلم_OWNER,
        ESP_FREERTOS_DEBUG_TASK_شمار_عرض,
        ESP_FREERTOS_DEBUG_PTR_عرض,
        /* New entries must be inserted here */
        ESP_FREERTOS_DEBUG_جدول_انتها,
    };

    ثابت DRAM_ATTR uint8_t FreeRTOS_openocd_params[ ESP_FREERTOS_DEBUG_جدول_انتها ] =
    {
        ESP_FREERTOS_DEBUG_جدول_انتها, /* table size */
        1,                            /* table version */
        tsk_KERNEL_نسخه_MAJOR,
        tsk_KERNEL_نسخه_MINOR,
        tsk_KERNEL_نسخه_BUILD,
        configMAX_PRIORITIES - 1,                   /* uxTopUsedPriority */
        offsetof( TCB_t, px_بالاترین_از_پشته ),            /* thread_stack_offset; */
        offsetof( TCB_t, pcTaskName ),              /* thread_name_offset; */
        حجم( List_t ),                           /* list_width */
        offsetof( List_t, ux_عدد_از_اقلام ),        /* list_item_num */
        offsetof( List_t, x_List_انتها ),               /* list_end_offset */
        offsetof( List_t, x_List_انتها.px_قبلی ),    /* list_next_offset */
        offsetof( List_قلم_t, px_قبلی ),         /* list_elem_next_offset */
        offsetof( List_قلم_t, pvOwner ),            /* list_elem_content_offset */
        حجم( UBaseType_t ),                      /* task_count_width */
        حجم( فراغ * )                            /* ptr_width */
    };

#پایان_اگر /* CONFIG_FREERTOS_DEBUG_OCDAWARE */
/*----------------------------------------------------------*/

/* ----------------------------------------------------- PSRAM ---------------------------------------------------- */

#إذا CONFIG_SPIRAM

    #إذا CONFIG_FREERTOS_SMP
        پایه_Type_t prv_Task_Create_Dynamic_Affinity_مجموعه_With_Caps( TaskFunction_t px_Task_کد,
                                                            ثابت حرف * ثابت pcName,
                                                            ثابت config_پشته_عمق_TYPE us_پشته_عمق,
                                                            فراغ * ثابت pvParameters,
                                                            UBaseType_t ux_اولویت,
                                                            UBaseType_t ux_Core_Affinity_ماسک,
                                                            UBaseType_t ux_پشته_حافظه_Caps,
                                                            TaskHandle_t * ثابت pxCreatedTask )
    #وإلا /* CONFIG_FREERTOS_SMP */
        پایه_Type_t prv_Task_Create_Dynamic_Pinned_مقصد_Core_With_Caps( TaskFunction_t px_Task_کد,
                                                             ثابت حرف * ثابت pcName,
                                                             ثابت config_پشته_عمق_TYPE us_پشته_عمق,
                                                             فراغ * ثابت pvParameters,
                                                             UBaseType_t ux_اولویت,
                                                             ثابت پایه_Type_t xCoreID,
                                                             UBaseType_t ux_پشته_حافظه_Caps,
                                                             TaskHandle_t * ثابت pxCreatedTask )
    #پایان_اگر /* CONFIG_FREERTOS_SMP */
    {
        TCB_t * pxNewTCB;
        پایه_Type_t xReturn;

        پشته_Type_t * px_پشته;

        configASSERT( ux_پشته_حافظه_Caps & ( MALLOC_CAP_8_بیت ) );
        configASSERT( ( ux_پشته_حافظه_Caps & MALLOC_CAP_SPIRAM ) ||
                      ( ux_پشته_حافظه_Caps & MALLOC_CAP_داخلی ) );
        #إذا ( !CONFIG_FREERTOS_SMP )
        {
            configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );
        }
        #پایان_اگر /* !CONFIG_FREERTOS_SMP */

        /* Allocate space for the stack used by the task being created. */
        px_پشته = انباشته_caps_malloc( ( ( ( size_t ) us_پشته_عمق ) * حجم( پشته_Type_t ) ), ux_پشته_حافظه_Caps );

        إذا( px_پشته != عدم )
        {
            /* Allocate space for the TCB. */
            pxNewTCB = ( TCB_t * ) pv_پورت_Malloc( حجم( TCB_t ) );

            إذا( pxNewTCB != عدم )
            {
                memset( ( فراغ * ) pxNewTCB, 0x00, حجم( TCB_t ) );

                /* Store the stack location in the TCB. */
                pxNewTCB->px_پشته = px_پشته;
            }
            وإلا
            {
                /* The stack cannot be used as the TCB has not been created. Free it. */
                انباشته_caps_free( px_پشته );
            }
        }
        وإلا
        {
            pxNewTCB = عدم;
        }

        إذا( pxNewTCB != عدم )
        {
            #إذا ( tsk_STATIC_AND_DYNAMIC_تخصیص_POSSIBLE != 0 )
            {
                /* Tasks can be created statically or dynamically, so note this
                 * task was created dynamically in case it is later deleted. */
                pxNewTCB->ucStaticallyAllocated = tsk_DYNAMICALLY_ALLOCATED_پشته_AND_TCB;
            }
            #پایان_اگر /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

            #إذا CONFIG_FREERTOS_SMP
            {
                prvInitialiseNewTask( px_Task_کد, pcName, ( uint32_t ) us_پشته_عمق, pvParameters, ux_اولویت, pxCreatedTask, pxNewTCB, عدم );
                #إذا ( ( config_رقم_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
                {
                    /* Set the task's affinity before scheduling it */
                    pxNewTCB->ux_Core_Affinity_ماسک = ux_Core_Affinity_ماسک;
                }
                #پایان_اگر /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            }
            #وإلا /* CONFIG_FREERTOS_SMP */
            {
                prvInitialiseNewTask( px_Task_کد, pcName, ( uint32_t ) us_پشته_عمق, pvParameters, ux_اولویت, pxCreatedTask, pxNewTCB, عدم, xCoreID );
            }
            #پایان_اگر /* CONFIG_FREERTOS_SMP */

            prv_جمع_کن_New_Task_مقصد_Ready_List( pxNewTCB );
            xReturn = pdPASS;
        }
        وإلا
        {
            xReturn = err_COULD_NOT_ALLOCATE_REQUIRED_حافظه;
        }

        أرجع xReturn;
    }

#پایان_اگر /* CONFIG_SPIRAM */
/*----------------------------------------------------------*/
