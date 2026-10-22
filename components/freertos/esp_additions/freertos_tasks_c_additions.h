/*
 * SPDX-FileCopyrightText: 2015-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#समावेश "sdkconfig.h"
#समावेश "esp_assert.h"
#समावेश "esp_heap_caps.h"
#समावेश "esp_compiler.h"
#समावेश "freertos/idf_additions.h"
#समावेश "freertos/freertos_debug.h"
#समावेश "esp_private/freertos_idf_additions_priv.h"

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
_Static_assert( दुरत्व( StaticTask_t, pxDummy6 ) == दुरत्व( TCB_t, px_ढेर ) );
_Static_assert( दुरत्व( StaticTask_t, pxDummy8 ) == दुरत्व( TCB_t, px_End_Of_ढेर ) );
#अगर !CONFIG_IDF_TARGET_LINUX    // Disabled for linux builds due to differences in types
_Static_assert( tskNO_AFFINITY == ( BaseType_t ) CONFIG_FREERTOS_NO_AFFINITY, "CONFIG_FREERTOS_NO_AFFINITY must be the same as tskNO_AFFINITY" );
#पूर्ण_अगर

/* ------------------------------------------------- Kernel Control ------------------------------------------------- */

#अगर ( !CONFIG_FREERTOS_SMP && ( config_अंक_CORES > 1 ) )

/*
 * Wrapper function to take "xKerneLock"
 */
    व्योम prvTakeKernelLock( व्योम )
    {
        /* We call the tasks.c critical section macro to take xKernelLock */
        taskENTER_CRITICAL( &xKernelLock );
    }

#पूर्ण_अगर /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#अगर ( !CONFIG_FREERTOS_SMP && ( config_अंक_CORES > 1 ) )

/*
 * Wrapper function to release "xKerneLock"
 */
    व्योम prvReleaseKernelLock( व्योम )
    {
        /* We call the tasks.c critical section macro to release xKernelLock */
        taskEXIT_CRITICAL( &xKernelLock );
    }

#पूर्ण_अगर /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#अगर ( CONFIG_FREERTOS_SMP && ( config_अंक_CORES > 1 ) )

/*
 * Workaround for non-thread safe multi-core OS startup (see IDF-4524)
 */
    व्योम prvStartSchedulerOtherCores( व्योम )
    {
        /* This function is always called with interrupts disabled*/
        xSchedulerRunning = pdTRUE;
    }

#पूर्ण_अगर /* ( CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#अगर ( !CONFIG_FREERTOS_SMP && ( config_अंक_CORES > 1 ) )

    BaseType_t xTaskIncrementTickOtherCores( व्योम )
    {
        /* Minor optimization. This function can never switch cores mid
         * execution */
        BaseType_t xCoreID = portGET_CORE_ID();
        BaseType_t xSwitchRequired = pdFALSE;

        /* This function should never be called by Core 0. */
        configASSERT( xCoreID != 0 );

        /* Called by the portable layer each time a tick interrupt occurs
         * on a core other than core 0. */
        traceTASK_INCREMENT_TICK( x_Tick_गिनती );

        अगर( uxSchedulerSuspended[ xCoreID ] == ( UBaseType_t ) 0U )
        {
            /* We need take the kernel lock here as we are about to access
             * kernel data structures. */
            taskENTER_CRITICAL_ISR( &xKernelLock );

            /* Tasks of equal priority to the currently running task will share
             * processing time (time slice) if preemption is on, and the application
             * writer has not explicitly turned time slicing off. */
            #अगर ( ( configUSE_PREEMPTION == 1 ) && ( configUSE_TIME_SLICING == 1 ) )
            {
                अगर( list_CURRENT_LIST_लंबाई( &( pxReadyTasksLists[ pxCurrentTCBs[ xCoreID ]->uxPriority ] ) ) > ( UBaseType_t ) 1 )
                {
                    xSwitchRequired = pdTRUE;
                }
                अन्यथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            #पूर्ण_अगर /* ( ( configUSE_PREEMPTION == 1 ) && ( configUSE_TIME_SLICING == 1 ) ) */

            /* Release the previously taken kernel lock as we have finished
             * accessing the kernel data structures. */
            taskEXIT_CRITICAL_ISR( &xKernelLock );

            #अगर ( configUSE_PREEMPTION == 1 )
            {
                अगर( xYieldPending[ xCoreID ] != pdFALSE )
                {
                    xSwitchRequired = pdTRUE;
                }
                अन्यथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            #पूर्ण_अगर /* configUSE_PREEMPTION */
        }

        #अगर ( configUSE_TICK_HOOK == 1 )
        {
            vApplicationTickHook();
        }
        #पूर्ण_अगर

        वापस xSwitchRequired;
    }

#पूर्ण_अगर /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

/* -------------------------------------------------- Task Creation ------------------------------------------------- */

#अगर ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )

    BaseType_t xTaskCreatePinnedToCore( TaskFunction_t pxTaskCode,
                                        स्थिर अक्षर * स्थिर pcName,
                                        स्थिर uint32_t us_ढेर_Depth,
                                        व्योम * स्थिर pvParameters,
                                        UBaseType_t uxPriority,
                                        TaskHandle_t * स्थिर pxCreatedTask,
                                        स्थिर BaseType_t xCoreID )
    {
        BaseType_t xReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );

        #अगर CONFIG_FREERTOS_SMP
        {
            /* If using Amazon SMP FreeRTOS. This function is just a wrapper around
             * xTaskCreate() or xTaskCreateAffinitySet(). */
            #अगर ( ( config_अंक_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
            {
                /* Convert xCoreID into an affinity mask */
                UBaseType_t uxCoreAffinityMask;

                /* Bit shifting << xCoreID is only valid if we have less than
                 * 32 cores. */
                ESP_STATIC_ASSERT( config_अंक_CORES < 32 );

                अगर( xCoreID == tskNO_AFFINITY )
                {
                    uxCoreAffinityMask = tskNO_AFFINITY;
                }
                अन्यथा
                {
                    uxCoreAffinityMask = ( 1 << xCoreID );
                }

                xReturn = xTaskCreateAffinitySet( pxTaskCode, pcName, us_ढेर_Depth, pvParameters, uxPriority, uxCoreAffinityMask, pxCreatedTask );
            }
            #अन्यथा /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            {
                xReturn = xTaskCreate( pxTaskCode, pcName, us_ढेर_Depth, pvParameters, uxPriority, pxCreatedTask );
            }
            #पूर्ण_अगर /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
        }
        #अन्यथा /* CONFIG_FREERTOS_SMP */
        {
            TCB_t * pxNewTCB;

            /* If the stack grows down then allocate the stack then the TCB so the
             * stack does not grow into the TCB.  Likewise if the stack grows up
             * then allocate the TCB then the stack. */
            #अगर ( port_ढेर_GROWTH > 0 )
            {
                /* Allocate space for the TCB.  Where the memory comes from depends on
                 * the implementation of the port malloc function and whether or not static
                 * allocation is being used. */
                pxNewTCB = ( TCB_t * ) pvPortMalloc( माप( TCB_t ) );

                अगर( pxNewTCB != शून्य )
                {
                    स_रखो( ( व्योम * ) pxNewTCB, 0x00, माप( TCB_t ) );

                    /* Allocate space for the stack used by the task being created.
                     * The base of the stack memory stored in the TCB so the task can
                     * be deleted later if required. */
                    pxNewTCB->px_ढेर = ( ढेर_Type_t * ) pv_Port_Malloc_ढेर( ( ( ( माप_प्रकार ) us_ढेर_Depth ) * माप( ढेर_Type_t ) ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */

                    अगर( pxNewTCB->px_ढेर == शून्य )
                    {
                        /* Could not allocate the stack.  Delete the allocated TCB. */
                        vPortFree( pxNewTCB );
                        pxNewTCB = शून्य;
                    }
                }
            }
            #अन्यथा /* portSTACK_GROWTH */
            {
                ढेर_Type_t * px_ढेर;

                /* Allocate space for the stack used by the task being created. */
                px_ढेर = pv_Port_Malloc_ढेर( ( ( ( माप_प्रकार ) us_ढेर_Depth ) * माप( ढेर_Type_t ) ) ); /*lint !e9079 All values returned by pvPortMalloc() have at least the alignment required by the MCU's stack and this allocation is the stack. */

                अगर( px_ढेर != शून्य )
                {
                    /* Allocate space for the TCB. */
                    pxNewTCB = ( TCB_t * ) pvPortMalloc( माप( TCB_t ) ); /*lint !e9087 !e9079 All values returned by pvPortMalloc() have at least the alignment required by the MCU's stack, and the first member of TCB_t is always a pointer to the task's stack. */

                    अगर( pxNewTCB != शून्य )
                    {
                        स_रखो( ( व्योम * ) pxNewTCB, 0x00, माप( TCB_t ) );

                        /* Store the stack location in the TCB. */
                        pxNewTCB->px_ढेर = px_ढेर;
                    }
                    अन्यथा
                    {
                        /* The stack cannot be used as the TCB was not created.  Free
                         * it again. */
                        v_Port_Free_ढेर( px_ढेर );
                    }
                }
                अन्यथा
                {
                    pxNewTCB = शून्य;
                }
            }
            #पूर्ण_अगर /* portSTACK_GROWTH */

            अगर( pxNewTCB != शून्य )
            {
                #अगर ( tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE != 0 ) /*lint !e9029 !e731 Macro has been consolidated for readability reasons. */
                {
                    /* Tasks can be created statically or dynamically, so note this
                     * task was created dynamically in case it is later deleted. */
                    pxNewTCB->ucStaticallyAllocated = tsk_DYNAMICALLY_ALLOCATED_ढेर_AND_TCB;
                }
                #पूर्ण_अगर /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

                prvInitialiseNewTask( pxTaskCode, pcName, ( uint32_t ) us_ढेर_Depth, pvParameters, uxPriority, pxCreatedTask, pxNewTCB, शून्य, xCoreID );
                prvAddNewTaskToReadyList( pxNewTCB );
                xReturn = pdPASS;
            }
            अन्यथा
            {
                xReturn = errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY;
            }
        }
        #पूर्ण_अगर /* CONFIG_FREERTOS_SMP */

        वापस xReturn;
    }

#पूर्ण_अगर /* ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#अगर ( configSUPPORT_STATIC_ALLOCATION == 1 )

    TaskHandle_t xTaskCreateStaticPinnedToCore( TaskFunction_t pxTaskCode,
                                                स्थिर अक्षर * स्थिर pcName,
                                                स्थिर uint32_t ul_ढेर_Depth,
                                                व्योम * स्थिर pvParameters,
                                                UBaseType_t uxPriority,
                                                ढेर_Type_t * स्थिर pux_ढेर_बफर,
                                                StaticTask_t * स्थिर px_Task_बफर,
                                                स्थिर BaseType_t xCoreID )
    {
        TaskHandle_t xReturn;

        configASSERT( port_VALID_ढेर_MEM( pux_ढेर_बफर ) );
        configASSERT( portVALID_TCB_MEM( px_Task_बफर ) );
        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );

        #अगर CONFIG_FREERTOS_SMP
        {
            /* If using Amazon SMP FreeRTOS. This function is just a wrapper around
             * xTaskCreateStatic() or xTaskCreateStaticAffinitySet(). */
            #अगर ( ( config_अंक_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
            {
                /* Convert xCoreID into an affinity mask */
                UBaseType_t uxCoreAffinityMask;

                /* Bit shifting << xCoreID is only valid if we have less than
                 * 32 cores. */
                ESP_STATIC_ASSERT( config_अंक_CORES < 32 );

                अगर( xCoreID == tskNO_AFFINITY )
                {
                    uxCoreAffinityMask = tskNO_AFFINITY;
                }
                अन्यथा
                {
                    uxCoreAffinityMask = ( 1 << xCoreID );
                }

                xReturn = xTaskCreateStaticAffinitySet( pxTaskCode, pcName, ul_ढेर_Depth, pvParameters, uxPriority, pux_ढेर_बफर, px_Task_बफर, uxCoreAffinityMask );
            }
            #अन्यथा /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            {
                xReturn = xTaskCreateStatic( pxTaskCode, pcName, ul_ढेर_Depth, pvParameters, uxPriority, pux_ढेर_बफर, px_Task_बफर );
            }
            #पूर्ण_अगर /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
        }
        #अन्यथा /* CONFIG_FREERTOS_SMP */
        {
            TCB_t * pxNewTCB;

            #अगर ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticTask_t equals the size of the real task
                 * structure. */
                अस्थिर माप_प्रकार x_आकार = माप( StaticTask_t );
                configASSERT( x_आकार == माप( TCB_t ) );
                ( व्योम ) x_आकार; /* Prevent lint warning when configASSERT() is not used. */
            }
            #पूर्ण_अगर /* configASSERT_DEFINED */

            अगर( ( px_Task_बफर != शून्य ) && ( pux_ढेर_बफर != शून्य ) )
            {
                /* The memory used for the task's TCB and stack are passed into this
                 * function - use them. */
                pxNewTCB = ( TCB_t * ) px_Task_बफर; /*lint !e740 !e9087 Unusual cast is ok as the structures are designed to have the same alignment, and the size is checked by an assert. */
                स_रखो( ( व्योम * ) pxNewTCB, 0x00, माप( TCB_t ) );
                pxNewTCB->px_ढेर = ( ढेर_Type_t * ) pux_ढेर_बफर;

                #अगर ( tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE != 0 ) /*lint !e731 !e9029 Macro has been consolidated for readability reasons. */
                {
                    /* Tasks can be created statically or dynamically, so note this
                     * task was created statically in case the task is later deleted. */
                    pxNewTCB->ucStaticallyAllocated = tsk_STATICALLY_ALLOCATED_ढेर_AND_TCB;
                }
                #पूर्ण_अगर /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

                prvInitialiseNewTask( pxTaskCode, pcName, ul_ढेर_Depth, pvParameters, uxPriority, &xReturn, pxNewTCB, शून्य, xCoreID );
                prvAddNewTaskToReadyList( pxNewTCB );
            }
            अन्यथा
            {
                xReturn = शून्य;
            }
        }
        #पूर्ण_अगर /* CONFIG_FREERTOS_SMP */

        वापस xReturn;
    }

#पूर्ण_अगर /* ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#अगर ( configUSE_TIMERS == 1 )

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
    BaseType_t __attribute__( ( weak ) ) xTimerCreateTimerTask( व्योम )
    {
        वापस pdPASS;
    }

#पूर्ण_अगर /* configUSE_TIMERS */
/*----------------------------------------------------------*/

/* ------------------------------------------------- Task Utilities ------------------------------------------------- */

BaseType_t xTaskGetCoreID( TaskHandle_t xTask )
{
    BaseType_t xReturn;

    #अगर ( config_अंक_CORES > 1 )
    {
        #अगर CONFIG_FREERTOS_SMP
            UBaseType_t uxCoreAffinityMask;

            /* Get the core affinity mask and convert it to an ID */
            uxCoreAffinityMask = vTaskCoreAffinityGet( xTask );

            /* If the task is not pinned to a particular core, treat it as tskNO_AFFINITY */
            अगर( uxCoreAffinityMask & ( uxCoreAffinityMask - 1 ) ) /* If more than one bit set */
            {
                xReturn = tskNO_AFFINITY;
            }
            अन्यथा
            {
                पूर्णांक i_सूचकांक_Plus_One = __builtin_ffs( uxCoreAffinityMask );
                निश्चित( i_सूचकांक_Plus_One >= 1 );
                xReturn = i_सूचकांक_Plus_One - 1;
            }
        #अन्यथा /* CONFIG_FREERTOS_SMP */
            TCB_t * pxTCB;

            pxTCB = prvGetTCBFromHandle( xTask );

            xReturn = pxTCB->xCoreID;
        #पूर्ण_अगर /* CONFIG_FREERTOS_SMP */
    }
    #अन्यथा /* configNUM_CORES > 1 */
    {
        /* Single-core. Just return a core ID of 0 */
        xReturn = 0;
    }
    #पूर्ण_अगर /* configNUM_CORES > 1 */

    वापस xReturn;
}
/*----------------------------------------------------------*/

#अगर ( ( !CONFIG_FREERTOS_SMP ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

    TaskHandle_t xTaskGetIdleTaskHandleForCore( BaseType_t xCoreID )
    {
        /* If xTaskGetIdleTaskHandle() is called before the scheduler has been
         * started, then xIdleTaskHandle will be NULL. */
        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );
        configASSERT( ( xIdleTaskHandle[ xCoreID ] != शून्य ) );
        वापस xIdleTaskHandle[ xCoreID ];
    }

#पूर्ण_अगर /* ( ( !CONFIG_FREERTOS_SMP ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*----------------------------------------------------------*/

#अगर ( ( !CONFIG_FREERTOS_SMP ) && ( ( INCLUDE_xTaskGetCurrentTaskHandle == 1 ) || ( configUSE_MUTEXES == 1 ) ) )

    TaskHandle_t xTaskGetCurrentTaskHandleForCore( BaseType_t xCoreID )
    {
        TaskHandle_t xReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

        #अगर ( CONFIG_FREERTOS_SMP )
        {
            xReturn = xTaskGetCurrentTaskHandleCPU( ( UBaseType_t ) xCoreID );
        }
        #अन्यथा /* CONFIG_FREERTOS_SMP */
        {
            /* A critical section is not required as this function does not
             * guarantee that the TCB will still be valid when this function
             * returns. */
            xReturn = pxCurrentTCBs[ xCoreID ];
        }
        #पूर्ण_अगर /* CONFIG_FREERTOS_SMP */

        वापस xReturn;
    }

#पूर्ण_अगर /* ( ( !CONFIG_FREERTOS_SMP ) && ( ( INCLUDE_xTaskGetCurrentTaskHandle == 1 ) || ( configUSE_MUTEXES == 1 ) ) ) */
/*----------------------------------------------------------*/

#अगर ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

    config_RUN_TIME_गणक_TYPE ul_Task_Get_Idle_Run_Time_गणक_For_Core( BaseType_t xCoreID )
    {
        config_RUN_TIME_गणक_TYPE ul_Run_Time_गणक;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

        /* For SMP, we need to take the kernel lock here as we are about to
         * access kernel data structures. */
        prvENTER_CRITICAL_SMP_ONLY( &xKernelLock );
        {
            ul_Run_Time_गणक = xIdleTaskHandle[ xCoreID ]->ul_Run_Time_गणक;
        }
        /* Release the previously taken kernel lock. */
        prvEXIT_CRITICAL_SMP_ONLY( &xKernelLock );

        वापस ul_Run_Time_गणक;
    }

#पूर्ण_अगर /* ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*----------------------------------------------------------*/

#अगर ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

    config_RUN_TIME_गणक_TYPE ulTaskGetIdleRunTimePercentForCore( BaseType_t xCoreID )
    {
        config_RUN_TIME_गणक_TYPE ul_योग_Time, ulReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

        #अगर_घोषित port_ALT_GET_RUN_TIME_गणक_मूल्य
            port_ALT_GET_RUN_TIME_गणक_मूल्य( ul_योग_Time );
        #अन्यथा
            ul_योग_Time = port_GET_RUN_TIME_गणक_मूल्य();
        #पूर्ण_अगर

        /* For percentage calculations. */
        ul_योग_Time /= ( config_RUN_TIME_गणक_TYPE ) 100;

        /* Avoid divide by zero errors. */
        अगर( ul_योग_Time > ( config_RUN_TIME_गणक_TYPE ) 0 )
        {
            /* For SMP, we need to take the kernel lock here as we are about
             * to access kernel data structures. */
            prvENTER_CRITICAL_SMP_ONLY( &xKernelLock );
            {
                ulReturn = xIdleTaskHandle[ xCoreID ]->ul_Run_Time_गणक / ul_योग_Time;
            }
            /* Release the previously taken kernel lock. */
            prvEXIT_CRITICAL_SMP_ONLY( &xKernelLock );
        }
        अन्यथा
        {
            ulReturn = 0;
        }

        वापस ulReturn;
    }

#पूर्ण_अगर /* ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*-----------------------------------------------------------*/

ढेर_Type_t * x_Task_Get_ढेर_Start( TaskHandle_t xTask )
{
    TCB_t * pxTCB;

    pxTCB = prvGetTCBFromHandle( xTask );
    वापस pxTCB->px_ढेर;
}
/*----------------------------------------------------------*/

uint8_t * px_Task_Get_ढेर_Start( TaskHandle_t xTask )
{
    वापस (uint8_t *)x_Task_Get_ढेर_Start( xTask );
}
/*----------------------------------------------------------*/

#अगर ( INCLUDE_vTaskPrioritySet == 1 )

    व्योम prvTaskPriorityRaise( prvTaskSavedPriority_t * pxSavedPriority,
                               UBaseType_t uxNewPriority )
    {
        TCB_t * pxTCB;
        UBaseType_t uxPriorityUsedOnEntry;

        configASSERT( ( uxNewPriority < configMAX_PRIORITIES ) );

        /* Ensure the new priority is valid. */
        अगर( uxNewPriority >= ( UBaseType_t ) configMAX_PRIORITIES )
        {
            uxNewPriority = ( UBaseType_t ) configMAX_PRIORITIES - ( UBaseType_t ) 1U;
        }

        #अगर CONFIG_FREERTOS_SMP
            taskENTER_CRITICAL();
        #अन्यथा
            taskENTER_CRITICAL( &xKernelLock );
        #पूर्ण_अगर
        {
            pxTCB = prvGetTCBFromHandle( शून्य );

            #अगर ( configUSE_MUTEXES == 1 )
            {
                pxSavedPriority->uxPriority = pxTCB->uxPriority;
                pxSavedPriority->uxBasePriority = pxTCB->uxBasePriority;

                /* If uxNewPriority < uxBasePriority, then there is nothing else to
                 * do, as uxBasePriority is always <= uxPriority. */
                अगर( uxNewPriority > pxTCB->uxBasePriority )
                {
                    pxTCB->uxBasePriority = uxNewPriority;

                    /* Remember the task's current priority before attempting to
                     * change it. If the task's current priority is changed, it must
                     * be done so before moving the task between task lists) in order
                     * for the taskRESET_READY_PRIORITY() macro to function correctly. */
                    uxPriorityUsedOnEntry = pxTCB->uxPriority;

                    अगर( uxNewPriority > pxTCB->uxPriority )
                    {
                        pxTCB->uxPriority = uxNewPriority;

                        /* Only reset the event list item value if the value is not
                         * being used for anything else. */
                        अगर( ( list_GET_LIST_ITEM_मूल्य( &( pxTCB->xEventListItem ) ) & task_EVENT_LIST_ITEM_मूल्य_IN_USE ) == 0UL )
                        {
                            list_SET_LIST_ITEM_मूल्य( &( pxTCB->xEventListItem ), ( ( TickType_t ) configMAX_PRIORITIES - ( TickType_t ) uxNewPriority ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */
                        }

                        /* If the task is in the blocked or suspended list we need do
                         * nothing more than change its priority variable. However, if
                         * the task is in a ready list it needs to be removed and placed
                         * in the list appropriate to its new priority. */
                        अगर( listIS_CONTAINED_WITHIN( &( pxReadyTasksLists[ uxPriorityUsedOnEntry ] ), &( pxTCB->xStateListItem ) ) != pdFALSE )
                        {
                            /* The task is currently in its ready list - remove before
                             * adding it to its new ready list.  As we are in a critical
                             * section we can do this even if the scheduler is suspended. */
                            अगर( uxListRemove( &( pxTCB->xStateListItem ) ) == ( UBaseType_t ) 0 )
                            {
                                /* It is known that the task is in its ready list so
                                 * there is no need to check again and the port level
                                 * reset macro can be called directly. */
                                portRESET_READY_PRIORITY( uxPriorityUsedOnEntry, ux_शीर्ष_Ready_Priority );
                            }

                            prvAddTaskToReadyList( pxTCB );
                        }
                    }
                }
            }
            #अन्यथा /* if ( configUSE_MUTEXES == 1 ) */
            {
                pxSavedPriority->uxPriority = pxTCB->uxPriority;

                अगर( uxNewPriority > pxTCB->uxPriority )
                {
                    vTaskPrioritySet( शून्य, uxNewPriority );
                }
            }
            #पूर्ण_अगर /* if ( configUSE_MUTEXES == 1 ) */
        }
        #अगर CONFIG_FREERTOS_SMP
            taskEXIT_CRITICAL();
        #अन्यथा
            taskEXIT_CRITICAL( &xKernelLock );
        #पूर्ण_अगर
    }

#पूर्ण_अगर /* INCLUDE_vTaskPrioritySet == 1 */
/*----------------------------------------------------------*/

#अगर ( INCLUDE_vTaskPrioritySet == 1 )

    व्योम prvTaskPriorityRestore( prvTaskSavedPriority_t * pxSavedPriority )
    {
        TCB_t * pxTCB;
        UBaseType_t uxNewPriority;
        UBaseType_t uxPriorityUsedOnEntry;
        UBaseType_t uxBasePriorityUsedOnEntry;
        BaseType_t xYieldRequired = pdFALSE;

        #अगर CONFIG_FREERTOS_SMP
            taskENTER_CRITICAL();
        #अन्यथा
            taskENTER_CRITICAL( &xKernelLock );
        #पूर्ण_अगर
        {
            pxTCB = prvGetTCBFromHandle( शून्य );

            #अगर ( configUSE_MUTEXES == 1 )
            {
                /* If the saved uxBasePriority == the task's uxBasePriority, it means
                 * that prvTaskPriorityRaise() never raised the task's uxBasePriority.
                 * In that case, there is nothing else to do. */
                अगर( pxSavedPriority->uxBasePriority != pxTCB->uxBasePriority )
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
                    अगर( pxTCB->uxPriority == uxBasePriorityUsedOnEntry )
                    {
                        अगर( pxTCB->uxMutexesHeld == 0 )
                        {
                            /* The task may have inherited a priority before prvTaskPriorityRaise()
                             * then disinherited a priority after prvTaskPriorityRaise().
                             * Thus we need set the uxPriority to the saved base priority
                             * so that the task's priority gets restored to the priority
                             * before any inheritance or raising. */
                            pxTCB->uxPriority = pxSavedPriority->uxBasePriority;
                        }
                        अन्यथा
                        {
                            /* The task may have inherited a priority before prvTaskPriorityRaise()
                             * was called. Thus, we need to restore uxPriority to the
                             * "saved uxPriority" so that the task still retains that
                             * inherited priority. */
                            pxTCB->uxPriority = pxSavedPriority->uxPriority;
                        }

                        uxNewPriority = pxTCB->uxPriority;

                        अगर( uxNewPriority < uxPriorityUsedOnEntry )
                        {
                            /* Setting the priority of the running task down means
                             * there may now be another task of higher priority that
                             * is ready to execute. */
                            xYieldRequired = pdTRUE;
                        }

                        /* Only reset the event list item value if the value is not
                         * being used for anything else. */
                        अगर( ( list_GET_LIST_ITEM_मूल्य( &( pxTCB->xEventListItem ) ) & task_EVENT_LIST_ITEM_मूल्य_IN_USE ) == 0UL )
                        {
                            list_SET_LIST_ITEM_मूल्य( &( pxTCB->xEventListItem ), ( ( TickType_t ) configMAX_PRIORITIES - ( TickType_t ) uxNewPriority ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */
                        }

                        /* If the task is in the blocked or suspended list we need do
                         * nothing more than change its priority variable. However, if
                         * the task is in a ready list it needs to be removed and placed
                         * in the list appropriate to its new priority. */
                        अगर( listIS_CONTAINED_WITHIN( &( pxReadyTasksLists[ uxPriorityUsedOnEntry ] ), &( pxTCB->xStateListItem ) ) != pdFALSE )
                        {
                            /* The task is currently in its ready list - remove before
                             * adding it to its new ready list.  As we are in a critical
                             * section we can do this even if the scheduler is suspended. */
                            अगर( uxListRemove( &( pxTCB->xStateListItem ) ) == ( UBaseType_t ) 0 )
                            {
                                /* It is known that the task is in its ready list so
                                 * there is no need to check again and the port level
                                 * reset macro can be called directly. */
                                portRESET_READY_PRIORITY( uxPriorityUsedOnEntry, ux_शीर्ष_Ready_Priority );
                            }

                            prvAddTaskToReadyList( pxTCB );
                        }

                        अगर( xYieldRequired != pdFALSE )
                        {
                            #अगर CONFIG_FREERTOS_SMP
                                taskYIELD_TASK_CORE_IF_USING_PREEMPTION( pxTCB );
                            #अन्यथा
                                taskYIELD_IF_USING_PREEMPTION();
                            #पूर्ण_अगर
                        }
                    }
                }
            }
            #अन्यथा /* if ( configUSE_MUTEXES == 1 ) */
            {
                vTaskPrioritySet( शून्य, pxSavedPriority->uxPriority );
            }
            #पूर्ण_अगर /* if ( configUSE_MUTEXES == 1 ) */
        }
        #अगर CONFIG_FREERTOS_SMP
            taskEXIT_CRITICAL();
        #अन्यथा
            taskEXIT_CRITICAL( &xKernelLock );
        #पूर्ण_अगर
    }

#पूर्ण_अगर /* ( INCLUDE_vTaskPrioritySet == 1 ) */
/*----------------------------------------------------------*/

/* --------------------------------------------- TLSP Deletion Callbacks -------------------------------------------- */

#अगर CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS

    व्योम vTaskSetThreadLocalStoragePointerAndDelCallback( TaskHandle_t xTaskToSet,
                                                          BaseType_t x_सूचकांक,
                                                          व्योम * pv_मूल्य,
                                                          TlsDeleteCallbackFunction_t pvDelCallback )
    {
        /* If TLSP deletion callbacks are enabled, then configNUM_THREAD_LOCAL_STORAGE_POINTERS
         * is doubled in size so that the latter half of the pvThreadLocalStoragePointers
         * stores the deletion callbacks. */
        अगर( x_सूचकांक < ( config_अंक_THREAD_LOCAL_STORAGE_POINTERS / 2 ) )
        {
            TCB_t * pxTCB;

            #अगर ( config_अंक_CORES > 1 )
            {
                /* For SMP, we need a critical section as another core could also
                 * update this task's TLSP at the same time. */
                #अगर CONFIG_FREERTOS_SMP
                {
                    taskENTER_CRITICAL();
                }
                #अन्यथा /* CONFIG_FREERTOS_SMP */
                {
                    taskENTER_CRITICAL( &xKernelLock );
                }
                #पूर्ण_अगर /* CONFIG_FREERTOS_SMP */
            }
            #पूर्ण_अगर /* configNUM_CORES > 1 */

            pxTCB = prvGetTCBFromHandle( xTaskToSet );
            /* Store the TLSP by indexing the first half of the array */
            pxTCB->pvThreadLocalStoragePointers[ x_सूचकांक ] = pv_मूल्य;

            /* Store the TLSP deletion callback by indexing the second half
             * of the array. */
            pxTCB->pvThreadLocalStoragePointers[ ( x_सूचकांक + ( config_अंक_THREAD_LOCAL_STORAGE_POINTERS / 2 ) ) ] = ( व्योम * ) pvDelCallback;

            #अगर ( config_अंक_CORES > 1 )
            {
                #अगर CONFIG_FREERTOS_SMP
                {
                    taskEXIT_CRITICAL();
                }
                #अन्यथा /* CONFIG_FREERTOS_SMP */
                {
                    taskEXIT_CRITICAL( &xKernelLock );
                }
                #पूर्ण_अगर /* CONFIG_FREERTOS_SMP */
            }
            #पूर्ण_अगर /* configNUM_CORES > 1 */
        }
    }

#पूर्ण_अगर /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */
/*----------------------------------------------------------*/

/* ----------------------------------------------------- Newlib ----------------------------------------------------- */

#अगर ( configUSE_NEWLIB_REENTRANT == 1 )

/**
 * @brief Get reentrancy structure of the current task
 *
 * - This function is required by newlib (when __DYNAMIC_REENT__ is enabled)
 * - It will return a pointer to the current task's reent struct
 * - If FreeRTOS is not running, it will return the global reent struct
 *
 * @return Pointer to a the (current taks's)/(global) reent struct
 */
    काष्ठा _reent * __getreent( व्योम )
    {
        /* No lock needed because if this changes, we won't be running anymore. */
        TCB_t * pxCurTask = ( TCB_t * ) xTaskGetCurrentTaskHandle();
        काष्ठा _reent * ret;

        अगर( pxCurTask == शून्य )
        {
            /* No task running. Return global struct. */
            ret = _GLOBAL_REENT;
        }
        अन्यथा
        {
            /* We have a currently executing task. Return its reentrant struct. */
            ret = &pxCurTask->xTLSBlock;
        }

        वापस ret;
    }

#पूर्ण_अगर /* configUSE_NEWLIB_REENTRANT == 1 */

/* -------------------------------------------------- Task Snapshot ------------------------------------------------- */

/**
 * @brief List of all task lists in FreeRTOS
 *
 * @note There are currently differing number of task list between SMP FreeRTOS and ESP-IDF FreeRTOS
 */
अटल List_t * non_ready_task_lists[] =
{
    #अगर_घोषित CONFIG_FREERTOS_SMP
        &xPendingReadyList,
    #अन्यथा /* CONFIG_FREERTOS_SMP */
        &xPendingReadyList[ 0 ],
        #अगर_अघोषित CONFIG_FREERTOS_UNICORE
            &xPendingReadyList[ 1 ],
        #पूर्ण_अगर /* CONFIG_FREERTOS_UNICORE */
    #पूर्ण_अगर /* CONFIG_FREERTOS_SMP */
    &xDelayedTaskList1,
    &xDelayedTaskList2,
    #अगर ( INCLUDE_vTaskDelete == 1 )
        &xTasksWaitingTermination,
    #पूर्ण_अगर
    #अगर ( INCLUDE_vTaskSuspend == 1 )
        &xSuspendedTaskList,
    #पूर्ण_अगर
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
अटल List_t * px_Get_Task_List_By_सूचकांक( UBaseType_t ux_List_सूचकांक )
{
    List_t * pxTaskList;
    स्थिर माप_प्रकार xNonReadyTaskListsCnt = ( माप( non_ready_task_lists ) / माप( List_t * ) );

    अगर( ux_List_सूचकांक < configMAX_PRIORITIES )
    {
        pxTaskList = &pxReadyTasksLists[ configMAX_PRIORITIES - 1 - ux_List_सूचकांक ];
    }
    अन्यथा अगर( ux_List_सूचकांक < configMAX_PRIORITIES + xNonReadyTaskListsCnt )
    {
        pxTaskList = non_ready_task_lists[ ux_List_सूचकांक - configMAX_PRIORITIES ];
    }
    अन्यथा
    {
        pxTaskList = शून्य;
    }

    /* sanity check */
    अगर( pxTaskList )
    {
        अगर( !portVALID_LIST_MEM( pxTaskList ) )
        {
            pxTaskList = शून्य;
        }
    }

    वापस pxTaskList;
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
अटल अंतरभूत UBaseType_t px_Get_Task_List_गिनती( व्योम )
{
    वापस configMAX_PRIORITIES + ( माप( non_ready_task_lists ) / माप( List_t * ) );
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
पूर्णांक x_Task_Get_अग्र( TaskIterator_t * xIterator )
{
    अगर( !xIterator )
    {
        वापस -1;
    }

    ListItem_t * px_अग्र_List_Item = xIterator->px_अग्र_List_Item;
    UBaseType_t uxCurListIdx = xIterator->ux_Current_List_सूचकांक;
    UBaseType_t uxMaxListIdx = px_Get_Task_List_गिनती();

    क्रम( ; uxCurListIdx < uxMaxListIdx; ++uxCurListIdx )
    {
        List_t * pxCurrentTaskList = px_Get_Task_List_By_सूचकांक( uxCurListIdx );

        अगर( !pxCurrentTaskList || ( list_CURRENT_LIST_लंबाई( pxCurrentTaskList ) == 0 ) )
        {
            जारी;
        }

        स्थिर ListItem_t * pxCurrListItem = listGET_END_MARKER( pxCurrentTaskList );

        अगर( !px_अग्र_List_Item )
        {
            /* We are here if the traversal starts from the beginning or when we finish traversing
             * for one of the state lists
             */
            px_अग्र_List_Item = list_GET_अग्र( pxCurrListItem );
        }

        अगर( !portVALID_LIST_MEM( px_अग्र_List_Item ) )
        {
            /* Nothing to do with the corrupted list item. We will skip to the next task state list.
             * pxNextListItem should be NULL at the beginning of each task list.
             */
            px_अग्र_List_Item = शून्य;
            जारी;
        }

        TCB_t * pxTCB = ( TCB_t * ) listGET_LIST_ITEM_OWNER( px_अग्र_List_Item );

        अगर( !portVALID_TCB_MEM( pxTCB ) )
        {
            pxTCB = शून्य;
        }

        xIterator->pxTaskHandle = pxTCB;
        xIterator->ux_Current_List_सूचकांक = uxCurListIdx;

        अगर( pxCurrListItem->pxPrevious == px_अग्र_List_Item )
        {
            /* If this is the last item of the current state list */
            xIterator->ux_Current_List_सूचकांक++;
            xIterator->px_अग्र_List_Item = शून्य;
        }
        अन्यथा
        {
            xIterator->px_अग्र_List_Item = list_GET_अग्र( px_अग्र_List_Item );
        }

        वापस uxCurListIdx;
    }

    वापस -1; /* end of the task list */
}
/*----------------------------------------------------------*/

BaseType_t vTaskGetSnapshot( TaskHandle_t pxTask,
                             TaskSnapshot_t * pxTaskSnapshot )
{
    ESP_STATIC_ANALYZER_CHECK(!pxTask, pdFALSE);

    अगर( ( portVALID_TCB_MEM( pxTask ) == false ) || ( pxTaskSnapshot == शून्य ) )
    {
        वापस pdFALSE;
    }

    TCB_t * pxTCB = ( TCB_t * ) pxTask;
    pxTaskSnapshot->pxTCB = pxTCB;
    pxTaskSnapshot->px_शीर्ष_Of_ढेर = ( ढेर_Type_t * ) pxTCB->px_शीर्ष_Of_ढेर;
    pxTaskSnapshot->px_End_Of_ढेर = ( ढेर_Type_t * ) pxTCB->px_End_Of_ढेर;
    वापस pdTRUE;
}
/*----------------------------------------------------------*/

UBaseType_t uxTaskGetSnapshotAll( TaskSnapshot_t * स्थिर pxTaskSnapshotArray,
                                  स्थिर UBaseType_t ux_Array_लंबाई,
                                  UBaseType_t * स्थिर pxTCBSize )
{
    UBaseType_t ux_Array_अंक_Filled = 0;

    /* Traverse all of the tasks lists */
    TaskIterator_t xTaskIter = { 0 }; /* Point to the first task list */

    जबतक( x_Task_Get_अग्र( &xTaskIter ) != -1 && ux_Array_अंक_Filled < ux_Array_लंबाई )
    {
        vTaskGetSnapshot( xTaskIter.pxTaskHandle, &pxTaskSnapshotArray[ ux_Array_अंक_Filled ] );
        ux_Array_अंक_Filled++;
    }

    अगर( pxTCBSize != शून्य )
    {
        *pxTCBSize = माप( TCB_t );
    }

    वापस ux_Array_अंक_Filled;
}
/*----------------------------------------------------------*/

/* ----------------------------------------------------- Misc ----------------------------------------------------- */

व्योम * pvTaskGetCurrentTCBForCore( BaseType_t xCoreID )
{
    व्योम * pvRet;

    configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

    #अगर CONFIG_FREERTOS_SMP
        /* SMP FreeRTOS defines pxCurrentTCB as a macro function call */
        pvRet = ( व्योम * ) pxCurrentTCB;
    #अन्यथा /* CONFIG_FREERTOS_SMP */
        pvRet = ( व्योम * ) pxCurrentTCBs[ xCoreID ];
    #पूर्ण_अगर /* CONFIG_FREERTOS_SMP */
    वापस pvRet;
}

/* ----------------------------------------------------- OpenOCD ---------------------------------------------------- */

#अगर CONFIG_FREERTOS_DEBUG_OCDAWARE

/**
 * Debug param indexes. DO NOT change the order. OpenOCD uses the same indexes
 * Entries in FreeRTOS_openocd_params must match the order of these indexes
 */
    क्रमागत
    {
        ESP_FREERTOS_DEBUG_TABLE_आकार = 0,
        ESP_FREERTOS_DEBUG_TABLE_VERSION,
        ESP_FREERTOS_DEBUG_KERNEL_VER_MAJOR,
        ESP_FREERTOS_DEBUG_KERNEL_VER_MINOR,
        ESP_FREERTOS_DEBUG_KERNEL_VER_BUILD,
        ESP_FREERTOS_DEBUG_UX_शीर्ष_USED_PIORITY,
        ESP_FREERTOS_DEBUG_PX_शीर्ष_OF_ढेर,
        ESP_FREERTOS_DEBUG_PC_TASK_NAME,
        ESP_FREERTOS_DEBUG_LIST_आकार,
        ESP_FREERTOS_DEBUG_LIST_अंक_वस्तुएँ,
        ESP_FREERTOS_DEBUG_LIST_END,
        ESP_FREERTOS_DEBUG_LIST_END_पूर्व,
        ESP_FREERTOS_DEBUG_LIST_ITEM_पूर्व,
        ESP_FREERTOS_DEBUG_LIST_ITEM_OWNER,
        ESP_FREERTOS_DEBUG_TASK_गिनती_WIDTH,
        ESP_FREERTOS_DEBUG_PTR_WIDTH,
        /* New entries must be inserted here */
        ESP_FREERTOS_DEBUG_TABLE_END,
    };

    स्थिर DRAM_ATTR uint8_t FreeRTOS_openocd_params[ ESP_FREERTOS_DEBUG_TABLE_END ] =
    {
        ESP_FREERTOS_DEBUG_TABLE_END, /* table size */
        1,                            /* table version */
        tskKERNEL_VERSION_MAJOR,
        tskKERNEL_VERSION_MINOR,
        tskKERNEL_VERSION_BUILD,
        configMAX_PRIORITIES - 1,                   /* uxTopUsedPriority */
        दुरत्व( TCB_t, px_शीर्ष_Of_ढेर ),            /* thread_stack_offset; */
        दुरत्व( TCB_t, pcTaskName ),              /* thread_name_offset; */
        माप( List_t ),                           /* list_width */
        दुरत्व( List_t, ux_संख्या_Of_वस्तुएँ ),        /* list_item_num */
        दुरत्व( List_t, xListEnd ),               /* list_end_offset */
        दुरत्व( List_t, xListEnd.pxPrevious ),    /* list_next_offset */
        दुरत्व( ListItem_t, pxPrevious ),         /* list_elem_next_offset */
        दुरत्व( ListItem_t, pvOwner ),            /* list_elem_content_offset */
        माप( UBaseType_t ),                      /* task_count_width */
        माप( व्योम * )                            /* ptr_width */
    };

#पूर्ण_अगर /* CONFIG_FREERTOS_DEBUG_OCDAWARE */
/*----------------------------------------------------------*/

/* ----------------------------------------------------- PSRAM ---------------------------------------------------- */

#अगर CONFIG_SPIRAM

    #अगर CONFIG_FREERTOS_SMP
        BaseType_t prvTaskCreateDynamicAffinitySetWithCaps( TaskFunction_t pxTaskCode,
                                                            स्थिर अक्षर * स्थिर pcName,
                                                            स्थिर config_ढेर_DEPTH_TYPE us_ढेर_Depth,
                                                            व्योम * स्थिर pvParameters,
                                                            UBaseType_t uxPriority,
                                                            UBaseType_t uxCoreAffinityMask,
                                                            UBaseType_t ux_ढेर_Memory_Caps,
                                                            TaskHandle_t * स्थिर pxCreatedTask )
    #अन्यथा /* CONFIG_FREERTOS_SMP */
        BaseType_t prvTaskCreateDynamicPinnedToCoreWithCaps( TaskFunction_t pxTaskCode,
                                                             स्थिर अक्षर * स्थिर pcName,
                                                             स्थिर config_ढेर_DEPTH_TYPE us_ढेर_Depth,
                                                             व्योम * स्थिर pvParameters,
                                                             UBaseType_t uxPriority,
                                                             स्थिर BaseType_t xCoreID,
                                                             UBaseType_t ux_ढेर_Memory_Caps,
                                                             TaskHandle_t * स्थिर pxCreatedTask )
    #पूर्ण_अगर /* CONFIG_FREERTOS_SMP */
    {
        TCB_t * pxNewTCB;
        BaseType_t xReturn;

        ढेर_Type_t * px_ढेर;

        configASSERT( ux_ढेर_Memory_Caps & ( MALLOC_CAP_8BIT ) );
        configASSERT( ( ux_ढेर_Memory_Caps & MALLOC_CAP_SPIRAM ) ||
                      ( ux_ढेर_Memory_Caps & MALLOC_CAP_INTERNAL ) );
        #अगर ( !CONFIG_FREERTOS_SMP )
        {
            configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );
        }
        #पूर्ण_अगर /* !CONFIG_FREERTOS_SMP */

        /* Allocate space for the stack used by the task being created. */
        px_ढेर = heap_caps_malloc( ( ( ( माप_प्रकार ) us_ढेर_Depth ) * माप( ढेर_Type_t ) ), ux_ढेर_Memory_Caps );

        अगर( px_ढेर != शून्य )
        {
            /* Allocate space for the TCB. */
            pxNewTCB = ( TCB_t * ) pvPortMalloc( माप( TCB_t ) );

            अगर( pxNewTCB != शून्य )
            {
                स_रखो( ( व्योम * ) pxNewTCB, 0x00, माप( TCB_t ) );

                /* Store the stack location in the TCB. */
                pxNewTCB->px_ढेर = px_ढेर;
            }
            अन्यथा
            {
                /* The stack cannot be used as the TCB has not been created. Free it. */
                heap_caps_free( px_ढेर );
            }
        }
        अन्यथा
        {
            pxNewTCB = शून्य;
        }

        अगर( pxNewTCB != शून्य )
        {
            #अगर ( tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE != 0 )
            {
                /* Tasks can be created statically or dynamically, so note this
                 * task was created dynamically in case it is later deleted. */
                pxNewTCB->ucStaticallyAllocated = tsk_DYNAMICALLY_ALLOCATED_ढेर_AND_TCB;
            }
            #पूर्ण_अगर /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

            #अगर CONFIG_FREERTOS_SMP
            {
                prvInitialiseNewTask( pxTaskCode, pcName, ( uint32_t ) us_ढेर_Depth, pvParameters, uxPriority, pxCreatedTask, pxNewTCB, शून्य );
                #अगर ( ( config_अंक_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
                {
                    /* Set the task's affinity before scheduling it */
                    pxNewTCB->uxCoreAffinityMask = uxCoreAffinityMask;
                }
                #पूर्ण_अगर /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            }
            #अन्यथा /* CONFIG_FREERTOS_SMP */
            {
                prvInitialiseNewTask( pxTaskCode, pcName, ( uint32_t ) us_ढेर_Depth, pvParameters, uxPriority, pxCreatedTask, pxNewTCB, शून्य, xCoreID );
            }
            #पूर्ण_अगर /* CONFIG_FREERTOS_SMP */

            prvAddNewTaskToReadyList( pxNewTCB );
            xReturn = pdPASS;
        }
        अन्यथा
        {
            xReturn = errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY;
        }

        वापस xReturn;
    }

#पूर्ण_अगर /* CONFIG_SPIRAM */
/*----------------------------------------------------------*/
