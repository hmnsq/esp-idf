/*
 * SPDX-FileCopyrightText: 2015-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#अन्तर्भाव "sdkconfig.h"
#अन्तर्भाव "esp_assert.h"
#अन्तर्भाव "esp_heap_caps.h"
#अन्तर्भाव "esp_compiler.h"
#अन्तर्भाव "freertos/idf_additions.h"
#अन्तर्भाव "freertos/freertos_debug.h"
#अन्तर्भाव "esp_private/freertos_idf_additions_priv.h"

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
_Static_assert( दुरत्व( StaticTask_t, pxDummy6 ) == दुरत्व( TCB_t, px_चळत ) );
_Static_assert( दुरत्व( StaticTask_t, pxDummy8 ) == दुरत्व( TCB_t, px_End_Of_चळत ) );
#जर !CONFIG_IDF_TARGET_LINUX    // Disabled for linux builds due to differences in types
_Static_assert( tskNO_AFFINITY == ( BaseType_t ) CONFIG_FREERTOS_NO_AFFINITY, "CONFIG_FREERTOS_NO_AFFINITY must be the same as tskNO_AFFINITY" );
#endif

/* ------------------------------------------------- Kernel Control ------------------------------------------------- */

#जर ( !CONFIG_FREERTOS_SMP && ( config_अंक_CORES > 1 ) )

/*
 * Wrapper function to take "xKerneLock"
 */
    शून्य prvTakeKernelLock( शून्य )
    {
        /* We call the tasks.c critical section macro to take xKernelLock */
        taskENTER_CRITICAL( &xKernelLock );
    }

#endif /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#जर ( !CONFIG_FREERTOS_SMP && ( config_अंक_CORES > 1 ) )

/*
 * Wrapper function to release "xKerneLock"
 */
    शून्य prvReleaseKernelLock( शून्य )
    {
        /* We call the tasks.c critical section macro to release xKernelLock */
        taskEXIT_CRITICAL( &xKernelLock );
    }

#endif /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#जर ( CONFIG_FREERTOS_SMP && ( config_अंक_CORES > 1 ) )

/*
 * Workaround for non-thread safe multi-core OS startup (see IDF-4524)
 */
    शून्य prvStartSchedulerOtherCores( शून्य )
    {
        /* This function is always called with interrupts disabled*/
        xSchedulerRunning = pdTRUE;
    }

#endif /* ( CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#जर ( !CONFIG_FREERTOS_SMP && ( config_अंक_CORES > 1 ) )

    BaseType_t xTaskIncrementTickOtherCores( शून्य )
    {
        /* Minor optimization. This function can never switch cores mid
         * execution */
        BaseType_t xCoreID = portGET_CORE_ID();
        BaseType_t xSwitchRequired = pdFALSE;

        /* This function should never be called by Core 0. */
        configASSERT( xCoreID != 0 );

        /* Called by the portable layer each time a tick interrupt occurs
         * on a core other than core 0. */
        traceTASK_INCREMENT_TICK( x_Tick_मोजणी );

        जर( uxSchedulerSuspended[ xCoreID ] == ( UBaseType_t ) 0U )
        {
            /* We need take the kernel lock here as we are about to access
             * kernel data structures. */
            taskENTER_CRITICAL_ISR( &xKernelLock );

            /* Tasks of equal priority to the currently running task will share
             * processing time (time slice) if preemption is on, and the application
             * writer has not explicitly turned time slicing off. */
            #जर ( ( configUSE_PREEMPTION == 1 ) && ( configUSE_TIME_SLICING == 1 ) )
            {
                जर( listCURRENT_LIST_LENGTH( &( pxReadyTasksLists[ pxCurrentTCBs[ xCoreID ]->uxPriority ] ) ) > ( UBaseType_t ) 1 )
                {
                    xSwitchRequired = pdTRUE;
                }
                अन्यथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            #endif /* ( ( configUSE_PREEMPTION == 1 ) && ( configUSE_TIME_SLICING == 1 ) ) */

            /* Release the previously taken kernel lock as we have finished
             * accessing the kernel data structures. */
            taskEXIT_CRITICAL_ISR( &xKernelLock );

            #जर ( configUSE_PREEMPTION == 1 )
            {
                जर( xYieldPending[ xCoreID ] != pdFALSE )
                {
                    xSwitchRequired = pdTRUE;
                }
                अन्यथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            #endif /* configUSE_PREEMPTION */
        }

        #जर ( configUSE_TICK_HOOK == 1 )
        {
            vApplicationTickHook();
        }
        #endif

        परत xSwitchRequired;
    }

#endif /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

/* -------------------------------------------------- Task Creation ------------------------------------------------- */

#जर ( configSUPPORT_DYNAMIC_ALLOCATION == 1 )

    BaseType_t xTaskCreatePinnedToCore( TaskFunction_t pxTaskCode,
                                        स्थिरांक अक्षर * स्थिरांक pcName,
                                        स्थिरांक uint32_t us_चळत_Depth,
                                        शून्य * स्थिरांक pvParameters,
                                        UBaseType_t uxPriority,
                                        TaskHandle_t * स्थिरांक pxCreatedTask,
                                        स्थिरांक BaseType_t xCoreID )
    {
        BaseType_t xReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );

        #जर CONFIG_FREERTOS_SMP
        {
            /* If using Amazon SMP FreeRTOS. This function is just a wrapper around
             * xTaskCreate() or xTaskCreateAffinitySet(). */
            #जर ( ( config_अंक_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
            {
                /* Convert xCoreID into an affinity mask */
                UBaseType_t uxCoreAffinityMask;

                /* Bit shifting << xCoreID is only valid if we have less than
                 * 32 cores. */
                ESP_STATIC_ASSERT( config_अंक_CORES < 32 );

                जर( xCoreID == tskNO_AFFINITY )
                {
                    uxCoreAffinityMask = tskNO_AFFINITY;
                }
                अन्यथा
                {
                    uxCoreAffinityMask = ( 1 << xCoreID );
                }

                xReturn = xTaskCreateAffinitySet( pxTaskCode, pcName, us_चळत_Depth, pvParameters, uxPriority, uxCoreAffinityMask, pxCreatedTask );
            }
            #अन्यथा /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            {
                xReturn = xTaskCreate( pxTaskCode, pcName, us_चळत_Depth, pvParameters, uxPriority, pxCreatedTask );
            }
            #endif /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
        }
        #अन्यथा /* CONFIG_FREERTOS_SMP */
        {
            TCB_t * pxNewTCB;

            /* If the stack grows down then allocate the stack then the TCB so the
             * stack does not grow into the TCB.  Likewise if the stack grows up
             * then allocate the TCB then the stack. */
            #जर ( port_चळत_GROWTH > 0 )
            {
                /* Allocate space for the TCB.  Where the memory comes from depends on
                 * the implementation of the port malloc function and whether or not static
                 * allocation is being used. */
                pxNewTCB = ( TCB_t * ) pvPortMalloc( आकार( TCB_t ) );

                जर( pxNewTCB != रिक्त )
                {
                    स_रखो( ( शून्य * ) pxNewTCB, 0x00, आकार( TCB_t ) );

                    /* Allocate space for the stack used by the task being created.
                     * The base of the stack memory stored in the TCB so the task can
                     * be deleted later if required. */
                    pxNewTCB->px_चळत = ( चळत_Type_t * ) pv_Port_Malloc_चळत( ( ( ( माप_प्रकार ) us_चळत_Depth ) * आकार( चळत_Type_t ) ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */

                    जर( pxNewTCB->px_चळत == रिक्त )
                    {
                        /* Could not allocate the stack.  Delete the allocated TCB. */
                        vPortFree( pxNewTCB );
                        pxNewTCB = रिक्त;
                    }
                }
            }
            #अन्यथा /* portSTACK_GROWTH */
            {
                चळत_Type_t * px_चळत;

                /* Allocate space for the stack used by the task being created. */
                px_चळत = pv_Port_Malloc_चळत( ( ( ( माप_प्रकार ) us_चळत_Depth ) * आकार( चळत_Type_t ) ) ); /*lint !e9079 All values returned by pvPortMalloc() have at least the alignment required by the MCU's stack and this allocation is the stack. */

                जर( px_चळत != रिक्त )
                {
                    /* Allocate space for the TCB. */
                    pxNewTCB = ( TCB_t * ) pvPortMalloc( आकार( TCB_t ) ); /*lint !e9087 !e9079 All values returned by pvPortMalloc() have at least the alignment required by the MCU's stack, and the first member of TCB_t is always a pointer to the task's stack. */

                    जर( pxNewTCB != रिक्त )
                    {
                        स_रखो( ( शून्य * ) pxNewTCB, 0x00, आकार( TCB_t ) );

                        /* Store the stack location in the TCB. */
                        pxNewTCB->px_चळत = px_चळत;
                    }
                    अन्यथा
                    {
                        /* The stack cannot be used as the TCB was not created.  Free
                         * it again. */
                        v_Port_Free_चळत( px_चळत );
                    }
                }
                अन्यथा
                {
                    pxNewTCB = रिक्त;
                }
            }
            #endif /* portSTACK_GROWTH */

            जर( pxNewTCB != रिक्त )
            {
                #जर ( tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE != 0 ) /*lint !e9029 !e731 Macro has been consolidated for readability reasons. */
                {
                    /* Tasks can be created statically or dynamically, so note this
                     * task was created dynamically in case it is later deleted. */
                    pxNewTCB->ucStaticallyAllocated = tsk_DYNAMICALLY_ALLOCATED_चळत_AND_TCB;
                }
                #endif /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

                prvInitialiseNewTask( pxTaskCode, pcName, ( uint32_t ) us_चळत_Depth, pvParameters, uxPriority, pxCreatedTask, pxNewTCB, रिक्त, xCoreID );
                prvAddNewTaskToReadyList( pxNewTCB );
                xReturn = pdPASS;
            }
            अन्यथा
            {
                xReturn = errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY;
            }
        }
        #endif /* CONFIG_FREERTOS_SMP */

        परत xReturn;
    }

#endif /* ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#जर ( configSUPPORT_STATIC_ALLOCATION == 1 )

    TaskHandle_t xTaskCreateStaticPinnedToCore( TaskFunction_t pxTaskCode,
                                                स्थिरांक अक्षर * स्थिरांक pcName,
                                                स्थिरांक uint32_t ul_चळत_Depth,
                                                शून्य * स्थिरांक pvParameters,
                                                UBaseType_t uxPriority,
                                                चळत_Type_t * स्थिरांक pux_चळत_Buffer,
                                                StaticTask_t * स्थिरांक pxTaskBuffer,
                                                स्थिरांक BaseType_t xCoreID )
    {
        TaskHandle_t xReturn;

        configASSERT( port_VALID_चळत_MEM( pux_चळत_Buffer ) );
        configASSERT( portVALID_TCB_MEM( pxTaskBuffer ) );
        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );

        #जर CONFIG_FREERTOS_SMP
        {
            /* If using Amazon SMP FreeRTOS. This function is just a wrapper around
             * xTaskCreateStatic() or xTaskCreateStaticAffinitySet(). */
            #जर ( ( config_अंक_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
            {
                /* Convert xCoreID into an affinity mask */
                UBaseType_t uxCoreAffinityMask;

                /* Bit shifting << xCoreID is only valid if we have less than
                 * 32 cores. */
                ESP_STATIC_ASSERT( config_अंक_CORES < 32 );

                जर( xCoreID == tskNO_AFFINITY )
                {
                    uxCoreAffinityMask = tskNO_AFFINITY;
                }
                अन्यथा
                {
                    uxCoreAffinityMask = ( 1 << xCoreID );
                }

                xReturn = xTaskCreateStaticAffinitySet( pxTaskCode, pcName, ul_चळत_Depth, pvParameters, uxPriority, pux_चळत_Buffer, pxTaskBuffer, uxCoreAffinityMask );
            }
            #अन्यथा /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            {
                xReturn = xTaskCreateStatic( pxTaskCode, pcName, ul_चळत_Depth, pvParameters, uxPriority, pux_चळत_Buffer, pxTaskBuffer );
            }
            #endif /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
        }
        #अन्यथा /* CONFIG_FREERTOS_SMP */
        {
            TCB_t * pxNewTCB;

            #जर ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticTask_t equals the size of the real task
                 * structure. */
                अस्थिर माप_प्रकार xSize = आकार( StaticTask_t );
                configASSERT( xSize == आकार( TCB_t ) );
                ( शून्य ) xSize; /* Prevent lint warning when configASSERT() is not used. */
            }
            #endif /* configASSERT_DEFINED */

            जर( ( pxTaskBuffer != रिक्त ) && ( pux_चळत_Buffer != रिक्त ) )
            {
                /* The memory used for the task's TCB and stack are passed into this
                 * function - use them. */
                pxNewTCB = ( TCB_t * ) pxTaskBuffer; /*lint !e740 !e9087 Unusual cast is ok as the structures are designed to have the same alignment, and the size is checked by an assert. */
                स_रखो( ( शून्य * ) pxNewTCB, 0x00, आकार( TCB_t ) );
                pxNewTCB->px_चळत = ( चळत_Type_t * ) pux_चळत_Buffer;

                #जर ( tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE != 0 ) /*lint !e731 !e9029 Macro has been consolidated for readability reasons. */
                {
                    /* Tasks can be created statically or dynamically, so note this
                     * task was created statically in case the task is later deleted. */
                    pxNewTCB->ucStaticallyAllocated = tsk_STATICALLY_ALLOCATED_चळत_AND_TCB;
                }
                #endif /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

                prvInitialiseNewTask( pxTaskCode, pcName, ul_चळत_Depth, pvParameters, uxPriority, &xReturn, pxNewTCB, रिक्त, xCoreID );
                prvAddNewTaskToReadyList( pxNewTCB );
            }
            अन्यथा
            {
                xReturn = रिक्त;
            }
        }
        #endif /* CONFIG_FREERTOS_SMP */

        परत xReturn;
    }

#endif /* ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#जर ( configUSE_TIMERS == 1 )

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
    BaseType_t __attribute__( ( weak ) ) xTimerCreateTimerTask( शून्य )
    {
        परत pdPASS;
    }

#endif /* configUSE_TIMERS */
/*----------------------------------------------------------*/

/* ------------------------------------------------- Task Utilities ------------------------------------------------- */

BaseType_t xTaskGetCoreID( TaskHandle_t xTask )
{
    BaseType_t xReturn;

    #जर ( config_अंक_CORES > 1 )
    {
        #जर CONFIG_FREERTOS_SMP
            UBaseType_t uxCoreAffinityMask;

            /* Get the core affinity mask and convert it to an ID */
            uxCoreAffinityMask = vTaskCoreAffinityGet( xTask );

            /* If the task is not pinned to a particular core, treat it as tskNO_AFFINITY */
            जर( uxCoreAffinityMask & ( uxCoreAffinityMask - 1 ) ) /* If more than one bit set */
            {
                xReturn = tskNO_AFFINITY;
            }
            अन्यथा
            {
                पूर्णांक iIndexPlusOne = __builtin_ffs( uxCoreAffinityMask );
                प्रतिज्ञा( iIndexPlusOne >= 1 );
                xReturn = iIndexPlusOne - 1;
            }
        #अन्यथा /* CONFIG_FREERTOS_SMP */
            TCB_t * pxTCB;

            pxTCB = prvGetTCBFromHandle( xTask );

            xReturn = pxTCB->xCoreID;
        #endif /* CONFIG_FREERTOS_SMP */
    }
    #अन्यथा /* configNUM_CORES > 1 */
    {
        /* Single-core. Just return a core ID of 0 */
        xReturn = 0;
    }
    #endif /* configNUM_CORES > 1 */

    परत xReturn;
}
/*----------------------------------------------------------*/

#जर ( ( !CONFIG_FREERTOS_SMP ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

    TaskHandle_t xTaskGetIdleTaskHandleForCore( BaseType_t xCoreID )
    {
        /* If xTaskGetIdleTaskHandle() is called before the scheduler has been
         * started, then xIdleTaskHandle will be NULL. */
        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );
        configASSERT( ( xIdleTaskHandle[ xCoreID ] != रिक्त ) );
        परत xIdleTaskHandle[ xCoreID ];
    }

#endif /* ( ( !CONFIG_FREERTOS_SMP ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*----------------------------------------------------------*/

#जर ( ( !CONFIG_FREERTOS_SMP ) && ( ( INCLUDE_xTaskGetCurrentTaskHandle == 1 ) || ( configUSE_MUTEXES == 1 ) ) )

    TaskHandle_t xTaskGetCurrentTaskHandleForCore( BaseType_t xCoreID )
    {
        TaskHandle_t xReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

        #जर ( CONFIG_FREERTOS_SMP )
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
        #endif /* CONFIG_FREERTOS_SMP */

        परत xReturn;
    }

#endif /* ( ( !CONFIG_FREERTOS_SMP ) && ( ( INCLUDE_xTaskGetCurrentTaskHandle == 1 ) || ( configUSE_MUTEXES == 1 ) ) ) */
/*----------------------------------------------------------*/

#जर ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

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

        परत ulRunTimeCounter;
    }

#endif /* ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*----------------------------------------------------------*/

#जर ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

    configRUN_TIME_COUNTER_TYPE ulTaskGetIdleRunTimePercentForCore( BaseType_t xCoreID )
    {
        configRUN_TIME_COUNTER_TYPE ulTotalTime, ulReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

        #ifdef port_ALT_GET_RUN_TIME_COUNTER_मूल्य
            port_ALT_GET_RUN_TIME_COUNTER_मूल्य( ulTotalTime );
        #अन्यथा
            ulTotalTime = port_GET_RUN_TIME_COUNTER_मूल्य();
        #endif

        /* For percentage calculations. */
        ulTotalTime /= ( configRUN_TIME_COUNTER_TYPE ) 100;

        /* Avoid divide by zero errors. */
        जर( ulTotalTime > ( configRUN_TIME_COUNTER_TYPE ) 0 )
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
        अन्यथा
        {
            ulReturn = 0;
        }

        परत ulReturn;
    }

#endif /* ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*-----------------------------------------------------------*/

चळत_Type_t * x_Task_Get_चळत_Start( TaskHandle_t xTask )
{
    TCB_t * pxTCB;

    pxTCB = prvGetTCBFromHandle( xTask );
    परत pxTCB->px_चळत;
}
/*----------------------------------------------------------*/

uint8_t * px_Task_Get_चळत_Start( TaskHandle_t xTask )
{
    परत (uint8_t *)x_Task_Get_चळत_Start( xTask );
}
/*----------------------------------------------------------*/

#जर ( INCLUDE_vTaskPrioritySet == 1 )

    शून्य prvTaskPriorityRaise( prvTaskSavedPriority_t * pxSavedPriority,
                               UBaseType_t uxNewPriority )
    {
        TCB_t * pxTCB;
        UBaseType_t uxPriorityUsedOnEntry;

        configASSERT( ( uxNewPriority < configMAX_PRIORITIES ) );

        /* Ensure the new priority is valid. */
        जर( uxNewPriority >= ( UBaseType_t ) configMAX_PRIORITIES )
        {
            uxNewPriority = ( UBaseType_t ) configMAX_PRIORITIES - ( UBaseType_t ) 1U;
        }

        #जर CONFIG_FREERTOS_SMP
            taskENTER_CRITICAL();
        #अन्यथा
            taskENTER_CRITICAL( &xKernelLock );
        #endif
        {
            pxTCB = prvGetTCBFromHandle( रिक्त );

            #जर ( configUSE_MUTEXES == 1 )
            {
                pxSavedPriority->uxPriority = pxTCB->uxPriority;
                pxSavedPriority->uxBasePriority = pxTCB->uxBasePriority;

                /* If uxNewPriority < uxBasePriority, then there is nothing else to
                 * do, as uxBasePriority is always <= uxPriority. */
                जर( uxNewPriority > pxTCB->uxBasePriority )
                {
                    pxTCB->uxBasePriority = uxNewPriority;

                    /* Remember the task's current priority before attempting to
                     * change it. If the task's current priority is changed, it must
                     * be done so before moving the task between task lists) in order
                     * for the taskRESET_READY_PRIORITY() macro to function correctly. */
                    uxPriorityUsedOnEntry = pxTCB->uxPriority;

                    जर( uxNewPriority > pxTCB->uxPriority )
                    {
                        pxTCB->uxPriority = uxNewPriority;

                        /* Only reset the event list item value if the value is not
                         * being used for anything else. */
                        जर( ( list_GET_LIST_ITEM_मूल्य( &( pxTCB->xEventListItem ) ) & task_EVENT_LIST_ITEM_मूल्य_IN_USE ) == 0UL )
                        {
                            list_SET_LIST_ITEM_मूल्य( &( pxTCB->xEventListItem ), ( ( TickType_t ) configMAX_PRIORITIES - ( TickType_t ) uxNewPriority ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */
                        }

                        /* If the task is in the blocked or suspended list we need do
                         * nothing more than change its priority variable. However, if
                         * the task is in a ready list it needs to be removed and placed
                         * in the list appropriate to its new priority. */
                        जर( listIS_CONTAINED_WITHIN( &( pxReadyTasksLists[ uxPriorityUsedOnEntry ] ), &( pxTCB->xStateListItem ) ) != pdFALSE )
                        {
                            /* The task is currently in its ready list - remove before
                             * adding it to its new ready list.  As we are in a critical
                             * section we can do this even if the scheduler is suspended. */
                            जर( uxListRemove( &( pxTCB->xStateListItem ) ) == ( UBaseType_t ) 0 )
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

                जर( uxNewPriority > pxTCB->uxPriority )
                {
                    vTaskPrioritySet( रिक्त, uxNewPriority );
                }
            }
            #endif /* if ( configUSE_MUTEXES == 1 ) */
        }
        #जर CONFIG_FREERTOS_SMP
            taskEXIT_CRITICAL();
        #अन्यथा
            taskEXIT_CRITICAL( &xKernelLock );
        #endif
    }

#endif /* INCLUDE_vTaskPrioritySet == 1 */
/*----------------------------------------------------------*/

#जर ( INCLUDE_vTaskPrioritySet == 1 )

    शून्य prvTaskPriorityRestore( prvTaskSavedPriority_t * pxSavedPriority )
    {
        TCB_t * pxTCB;
        UBaseType_t uxNewPriority;
        UBaseType_t uxPriorityUsedOnEntry;
        UBaseType_t uxBasePriorityUsedOnEntry;
        BaseType_t xYieldRequired = pdFALSE;

        #जर CONFIG_FREERTOS_SMP
            taskENTER_CRITICAL();
        #अन्यथा
            taskENTER_CRITICAL( &xKernelLock );
        #endif
        {
            pxTCB = prvGetTCBFromHandle( रिक्त );

            #जर ( configUSE_MUTEXES == 1 )
            {
                /* If the saved uxBasePriority == the task's uxBasePriority, it means
                 * that prvTaskPriorityRaise() never raised the task's uxBasePriority.
                 * In that case, there is nothing else to do. */
                जर( pxSavedPriority->uxBasePriority != pxTCB->uxBasePriority )
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
                    जर( pxTCB->uxPriority == uxBasePriorityUsedOnEntry )
                    {
                        जर( pxTCB->uxMutexesHeld == 0 )
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

                        जर( uxNewPriority < uxPriorityUsedOnEntry )
                        {
                            /* Setting the priority of the running task down means
                             * there may now be another task of higher priority that
                             * is ready to execute. */
                            xYieldRequired = pdTRUE;
                        }

                        /* Only reset the event list item value if the value is not
                         * being used for anything else. */
                        जर( ( list_GET_LIST_ITEM_मूल्य( &( pxTCB->xEventListItem ) ) & task_EVENT_LIST_ITEM_मूल्य_IN_USE ) == 0UL )
                        {
                            list_SET_LIST_ITEM_मूल्य( &( pxTCB->xEventListItem ), ( ( TickType_t ) configMAX_PRIORITIES - ( TickType_t ) uxNewPriority ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */
                        }

                        /* If the task is in the blocked or suspended list we need do
                         * nothing more than change its priority variable. However, if
                         * the task is in a ready list it needs to be removed and placed
                         * in the list appropriate to its new priority. */
                        जर( listIS_CONTAINED_WITHIN( &( pxReadyTasksLists[ uxPriorityUsedOnEntry ] ), &( pxTCB->xStateListItem ) ) != pdFALSE )
                        {
                            /* The task is currently in its ready list - remove before
                             * adding it to its new ready list.  As we are in a critical
                             * section we can do this even if the scheduler is suspended. */
                            जर( uxListRemove( &( pxTCB->xStateListItem ) ) == ( UBaseType_t ) 0 )
                            {
                                /* It is known that the task is in its ready list so
                                 * there is no need to check again and the port level
                                 * reset macro can be called directly. */
                                portRESET_READY_PRIORITY( uxPriorityUsedOnEntry, ux_शीर्ष_Ready_Priority );
                            }

                            prvAddTaskToReadyList( pxTCB );
                        }

                        जर( xYieldRequired != pdFALSE )
                        {
                            #जर CONFIG_FREERTOS_SMP
                                taskYIELD_TASK_CORE_IF_USING_PREEMPTION( pxTCB );
                            #अन्यथा
                                taskYIELD_IF_USING_PREEMPTION();
                            #endif
                        }
                    }
                }
            }
            #अन्यथा /* if ( configUSE_MUTEXES == 1 ) */
            {
                vTaskPrioritySet( रिक्त, pxSavedPriority->uxPriority );
            }
            #endif /* if ( configUSE_MUTEXES == 1 ) */
        }
        #जर CONFIG_FREERTOS_SMP
            taskEXIT_CRITICAL();
        #अन्यथा
            taskEXIT_CRITICAL( &xKernelLock );
        #endif
    }

#endif /* ( INCLUDE_vTaskPrioritySet == 1 ) */
/*----------------------------------------------------------*/

/* --------------------------------------------- TLSP Deletion Callbacks -------------------------------------------- */

#जर CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS

    शून्य vTaskSetThreadLocalStoragePointerAndDelCallback( TaskHandle_t xTaskToSet,
                                                          BaseType_t xIndex,
                                                          शून्य * pv_मूल्य,
                                                          TlsDeleteCallbackFunction_t pvDelCallback )
    {
        /* If TLSP deletion callbacks are enabled, then configNUM_THREAD_LOCAL_STORAGE_POINTERS
         * is doubled in size so that the latter half of the pvThreadLocalStoragePointers
         * stores the deletion callbacks. */
        जर( xIndex < ( config_अंक_THREAD_LOCAL_STORAGE_POINTERS / 2 ) )
        {
            TCB_t * pxTCB;

            #जर ( config_अंक_CORES > 1 )
            {
                /* For SMP, we need a critical section as another core could also
                 * update this task's TLSP at the same time. */
                #जर CONFIG_FREERTOS_SMP
                {
                    taskENTER_CRITICAL();
                }
                #अन्यथा /* CONFIG_FREERTOS_SMP */
                {
                    taskENTER_CRITICAL( &xKernelLock );
                }
                #endif /* CONFIG_FREERTOS_SMP */
            }
            #endif /* configNUM_CORES > 1 */

            pxTCB = prvGetTCBFromHandle( xTaskToSet );
            /* Store the TLSP by indexing the first half of the array */
            pxTCB->pvThreadLocalStoragePointers[ xIndex ] = pv_मूल्य;

            /* Store the TLSP deletion callback by indexing the second half
             * of the array. */
            pxTCB->pvThreadLocalStoragePointers[ ( xIndex + ( config_अंक_THREAD_LOCAL_STORAGE_POINTERS / 2 ) ) ] = ( शून्य * ) pvDelCallback;

            #जर ( config_अंक_CORES > 1 )
            {
                #जर CONFIG_FREERTOS_SMP
                {
                    taskEXIT_CRITICAL();
                }
                #अन्यथा /* CONFIG_FREERTOS_SMP */
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

#जर ( configUSE_NEWLIB_REENTRANT == 1 )

/**
 * @brief Get reentrancy structure of the current task
 *
 * - This function is required by newlib (when __DYNAMIC_REENT__ is enabled)
 * - It will return a pointer to the current task's reent struct
 * - If FreeRTOS is not running, it will return the global reent struct
 *
 * @return Pointer to a the (current taks's)/(global) reent struct
 */
    संरचना _reent * __getreent( शून्य )
    {
        /* No lock needed because if this changes, we won't be running anymore. */
        TCB_t * pxCurTask = ( TCB_t * ) xTaskGetCurrentTaskHandle();
        संरचना _reent * ret;

        जर( pxCurTask == रिक्त )
        {
            /* No task running. Return global struct. */
            ret = _GLOBAL_REENT;
        }
        अन्यथा
        {
            /* We have a currently executing task. Return its reentrant struct. */
            ret = &pxCurTask->xTLSBlock;
        }

        परत ret;
    }

#endif /* configUSE_NEWLIB_REENTRANT == 1 */

/* -------------------------------------------------- Task Snapshot ------------------------------------------------- */

/**
 * @brief List of all task lists in FreeRTOS
 *
 * @note There are currently differing number of task list between SMP FreeRTOS and ESP-IDF FreeRTOS
 */
स्थिर List_t * non_ready_task_lists[] =
{
    #ifdef CONFIG_FREERTOS_SMP
        &xPendingReadyList,
    #अन्यथा /* CONFIG_FREERTOS_SMP */
        &xPendingReadyList[ 0 ],
        #ifndef CONFIG_FREERTOS_UNICORE
            &xPendingReadyList[ 1 ],
        #endif /* CONFIG_FREERTOS_UNICORE */
    #endif /* CONFIG_FREERTOS_SMP */
    &xDelayedTaskList1,
    &xDelayedTaskList2,
    #जर ( INCLUDE_vTaskDelete == 1 )
        &xTasksWaitingTermination,
    #endif
    #जर ( INCLUDE_vTaskSuspend == 1 )
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
स्थिर List_t * pxGetTaskListByIndex( UBaseType_t uxListIndex )
{
    List_t * pxTaskList;
    स्थिरांक माप_प्रकार xNonReadyTaskListsCnt = ( आकार( non_ready_task_lists ) / आकार( List_t * ) );

    जर( uxListIndex < configMAX_PRIORITIES )
    {
        pxTaskList = &pxReadyTasksLists[ configMAX_PRIORITIES - 1 - uxListIndex ];
    }
    अन्यथा जर( uxListIndex < configMAX_PRIORITIES + xNonReadyTaskListsCnt )
    {
        pxTaskList = non_ready_task_lists[ uxListIndex - configMAX_PRIORITIES ];
    }
    अन्यथा
    {
        pxTaskList = रिक्त;
    }

    /* sanity check */
    जर( pxTaskList )
    {
        जर( !portVALID_LIST_MEM( pxTaskList ) )
        {
            pxTaskList = रिक्त;
        }
    }

    परत pxTaskList;
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
स्थिर ओळीत UBaseType_t px_Get_Task_List_मोजणी( शून्य )
{
    परत configMAX_PRIORITIES + ( आकार( non_ready_task_lists ) / आकार( List_t * ) );
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
पूर्णांक xTaskGetNext( TaskIterator_t * xIterator )
{
    जर( !xIterator )
    {
        परत -1;
    }

    ListItem_t * pxNextListItem = xIterator->pxNextListItem;
    UBaseType_t uxCurListIdx = xIterator->uxCurrentListIndex;
    UBaseType_t uxMaxListIdx = px_Get_Task_List_मोजणी();

    साठी( ; uxCurListIdx < uxMaxListIdx; ++uxCurListIdx )
    {
        List_t * pxCurrentTaskList = pxGetTaskListByIndex( uxCurListIdx );

        जर( !pxCurrentTaskList || ( listCURRENT_LIST_LENGTH( pxCurrentTaskList ) == 0 ) )
        {
            सुरूठेवा;
        }

        स्थिरांक ListItem_t * pxCurrListItem = listGET_END_MARKER( pxCurrentTaskList );

        जर( !pxNextListItem )
        {
            /* We are here if the traversal starts from the beginning or when we finish traversing
             * for one of the state lists
             */
            pxNextListItem = listGET_NEXT( pxCurrListItem );
        }

        जर( !portVALID_LIST_MEM( pxNextListItem ) )
        {
            /* Nothing to do with the corrupted list item. We will skip to the next task state list.
             * pxNextListItem should be NULL at the beginning of each task list.
             */
            pxNextListItem = रिक्त;
            सुरूठेवा;
        }

        TCB_t * pxTCB = ( TCB_t * ) listGET_LIST_ITEM_OWNER( pxNextListItem );

        जर( !portVALID_TCB_MEM( pxTCB ) )
        {
            pxTCB = रिक्त;
        }

        xIterator->pxTaskHandle = pxTCB;
        xIterator->uxCurrentListIndex = uxCurListIdx;

        जर( pxCurrListItem->pxPrevious == pxNextListItem )
        {
            /* If this is the last item of the current state list */
            xIterator->uxCurrentListIndex++;
            xIterator->pxNextListItem = रिक्त;
        }
        अन्यथा
        {
            xIterator->pxNextListItem = listGET_NEXT( pxNextListItem );
        }

        परत uxCurListIdx;
    }

    परत -1; /* end of the task list */
}
/*----------------------------------------------------------*/

BaseType_t vTaskGetSnapshot( TaskHandle_t pxTask,
                             TaskSnapshot_t * pxTaskSnapshot )
{
    ESP_STATIC_ANALYZER_CHECK(!pxTask, pdFALSE);

    जर( ( portVALID_TCB_MEM( pxTask ) == false ) || ( pxTaskSnapshot == रिक्त ) )
    {
        परत pdFALSE;
    }

    TCB_t * pxTCB = ( TCB_t * ) pxTask;
    pxTaskSnapshot->pxTCB = pxTCB;
    pxTaskSnapshot->px_शीर्ष_Of_चळत = ( चळत_Type_t * ) pxTCB->px_शीर्ष_Of_चळत;
    pxTaskSnapshot->px_End_Of_चळत = ( चळत_Type_t * ) pxTCB->px_End_Of_चळत;
    परत pdTRUE;
}
/*----------------------------------------------------------*/

UBaseType_t uxTaskGetSnapshotAll( TaskSnapshot_t * स्थिरांक pxTaskSnapshotArray,
                                  स्थिरांक UBaseType_t uxArrayLength,
                                  UBaseType_t * स्थिरांक pxTCBSize )
{
    UBaseType_t ux_Array_अंक_Filled = 0;

    /* Traverse all of the tasks lists */
    TaskIterator_t xTaskIter = { 0 }; /* Point to the first task list */

    पर्यंत( xTaskGetNext( &xTaskIter ) != -1 && ux_Array_अंक_Filled < uxArrayLength )
    {
        vTaskGetSnapshot( xTaskIter.pxTaskHandle, &pxTaskSnapshotArray[ ux_Array_अंक_Filled ] );
        ux_Array_अंक_Filled++;
    }

    जर( pxTCBSize != रिक्त )
    {
        *pxTCBSize = आकार( TCB_t );
    }

    परत ux_Array_अंक_Filled;
}
/*----------------------------------------------------------*/

/* ----------------------------------------------------- Misc ----------------------------------------------------- */

शून्य * pvTaskGetCurrentTCBForCore( BaseType_t xCoreID )
{
    शून्य * pvRet;

    configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

    #जर CONFIG_FREERTOS_SMP
        /* SMP FreeRTOS defines pxCurrentTCB as a macro function call */
        pvRet = ( शून्य * ) pxCurrentTCB;
    #अन्यथा /* CONFIG_FREERTOS_SMP */
        pvRet = ( शून्य * ) pxCurrentTCBs[ xCoreID ];
    #endif /* CONFIG_FREERTOS_SMP */
    परत pvRet;
}

/* ----------------------------------------------------- OpenOCD ---------------------------------------------------- */

#जर CONFIG_FREERTOS_DEBUG_OCDAWARE

/**
 * Debug param indexes. DO NOT change the order. OpenOCD uses the same indexes
 * Entries in FreeRTOS_openocd_params must match the order of these indexes
 */
    गणना
    {
        ESP_FREERTOS_DEBUG_TABLE_SIZE = 0,
        ESP_FREERTOS_DEBUG_TABLE_VERSION,
        ESP_FREERTOS_DEBUG_KERNEL_VER_MAJOR,
        ESP_FREERTOS_DEBUG_KERNEL_VER_MINOR,
        ESP_FREERTOS_DEBUG_KERNEL_VER_BUILD,
        ESP_FREERTOS_DEBUG_UX_शीर्ष_USED_PIORITY,
        ESP_FREERTOS_DEBUG_PX_शीर्ष_OF_चळत,
        ESP_FREERTOS_DEBUG_PC_TASK_NAME,
        ESP_FREERTOS_DEBUG_LIST_SIZE,
        ESP_FREERTOS_DEBUG_LIST_अंक_वस्तुसंच,
        ESP_FREERTOS_DEBUG_LIST_END,
        ESP_FREERTOS_DEBUG_LIST_END_PREV,
        ESP_FREERTOS_DEBUG_LIST_ITEM_PREV,
        ESP_FREERTOS_DEBUG_LIST_ITEM_OWNER,
        ESP_FREERTOS_DEBUG_TASK_मोजणी_WIDTH,
        ESP_FREERTOS_DEBUG_PTR_WIDTH,
        /* New entries must be inserted here */
        ESP_FREERTOS_DEBUG_TABLE_END,
    };

    स्थिरांक DRAM_ATTR uint8_t FreeRTOS_openocd_params[ ESP_FREERTOS_DEBUG_TABLE_END ] =
    {
        ESP_FREERTOS_DEBUG_TABLE_END, /* table size */
        1,                            /* table version */
        tskKERNEL_VERSION_MAJOR,
        tskKERNEL_VERSION_MINOR,
        tskKERNEL_VERSION_BUILD,
        configMAX_PRIORITIES - 1,                   /* uxTopUsedPriority */
        दुरत्व( TCB_t, px_शीर्ष_Of_चळत ),            /* thread_stack_offset; */
        दुरत्व( TCB_t, pcTaskName ),              /* thread_name_offset; */
        आकार( List_t ),                           /* list_width */
        दुरत्व( List_t, ux_संख्या_Of_वस्तुसंच ),        /* list_item_num */
        दुरत्व( List_t, xListEnd ),               /* list_end_offset */
        दुरत्व( List_t, xListEnd.pxPrevious ),    /* list_next_offset */
        दुरत्व( ListItem_t, pxPrevious ),         /* list_elem_next_offset */
        दुरत्व( ListItem_t, pvOwner ),            /* list_elem_content_offset */
        आकार( UBaseType_t ),                      /* task_count_width */
        आकार( शून्य * )                            /* ptr_width */
    };

#endif /* CONFIG_FREERTOS_DEBUG_OCDAWARE */
/*----------------------------------------------------------*/

/* ----------------------------------------------------- PSRAM ---------------------------------------------------- */

#जर CONFIG_SPIRAM

    #जर CONFIG_FREERTOS_SMP
        BaseType_t prvTaskCreateDynamicAffinitySetWithCaps( TaskFunction_t pxTaskCode,
                                                            स्थिरांक अक्षर * स्थिरांक pcName,
                                                            स्थिरांक config_चळत_DEPTH_TYPE us_चळत_Depth,
                                                            शून्य * स्थिरांक pvParameters,
                                                            UBaseType_t uxPriority,
                                                            UBaseType_t uxCoreAffinityMask,
                                                            UBaseType_t ux_चळत_Memory_Caps,
                                                            TaskHandle_t * स्थिरांक pxCreatedTask )
    #अन्यथा /* CONFIG_FREERTOS_SMP */
        BaseType_t prvTaskCreateDynamicPinnedToCoreWithCaps( TaskFunction_t pxTaskCode,
                                                             स्थिरांक अक्षर * स्थिरांक pcName,
                                                             स्थिरांक config_चळत_DEPTH_TYPE us_चळत_Depth,
                                                             शून्य * स्थिरांक pvParameters,
                                                             UBaseType_t uxPriority,
                                                             स्थिरांक BaseType_t xCoreID,
                                                             UBaseType_t ux_चळत_Memory_Caps,
                                                             TaskHandle_t * स्थिरांक pxCreatedTask )
    #endif /* CONFIG_FREERTOS_SMP */
    {
        TCB_t * pxNewTCB;
        BaseType_t xReturn;

        चळत_Type_t * px_चळत;

        configASSERT( ux_चळत_Memory_Caps & ( MALLOC_CAP_8BIT ) );
        configASSERT( ( ux_चळत_Memory_Caps & MALLOC_CAP_SPIRAM ) ||
                      ( ux_चळत_Memory_Caps & MALLOC_CAP_INTERNAL ) );
        #जर ( !CONFIG_FREERTOS_SMP )
        {
            configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );
        }
        #endif /* !CONFIG_FREERTOS_SMP */

        /* Allocate space for the stack used by the task being created. */
        px_चळत = heap_caps_malloc( ( ( ( माप_प्रकार ) us_चळत_Depth ) * आकार( चळत_Type_t ) ), ux_चळत_Memory_Caps );

        जर( px_चळत != रिक्त )
        {
            /* Allocate space for the TCB. */
            pxNewTCB = ( TCB_t * ) pvPortMalloc( आकार( TCB_t ) );

            जर( pxNewTCB != रिक्त )
            {
                स_रखो( ( शून्य * ) pxNewTCB, 0x00, आकार( TCB_t ) );

                /* Store the stack location in the TCB. */
                pxNewTCB->px_चळत = px_चळत;
            }
            अन्यथा
            {
                /* The stack cannot be used as the TCB has not been created. Free it. */
                heap_caps_free( px_चळत );
            }
        }
        अन्यथा
        {
            pxNewTCB = रिक्त;
        }

        जर( pxNewTCB != रिक्त )
        {
            #जर ( tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE != 0 )
            {
                /* Tasks can be created statically or dynamically, so note this
                 * task was created dynamically in case it is later deleted. */
                pxNewTCB->ucStaticallyAllocated = tsk_DYNAMICALLY_ALLOCATED_चळत_AND_TCB;
            }
            #endif /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

            #जर CONFIG_FREERTOS_SMP
            {
                prvInitialiseNewTask( pxTaskCode, pcName, ( uint32_t ) us_चळत_Depth, pvParameters, uxPriority, pxCreatedTask, pxNewTCB, रिक्त );
                #जर ( ( config_अंक_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
                {
                    /* Set the task's affinity before scheduling it */
                    pxNewTCB->uxCoreAffinityMask = uxCoreAffinityMask;
                }
                #endif /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            }
            #अन्यथा /* CONFIG_FREERTOS_SMP */
            {
                prvInitialiseNewTask( pxTaskCode, pcName, ( uint32_t ) us_चळत_Depth, pvParameters, uxPriority, pxCreatedTask, pxNewTCB, रिक्त, xCoreID );
            }
            #endif /* CONFIG_FREERTOS_SMP */

            prvAddNewTaskToReadyList( pxNewTCB );
            xReturn = pdPASS;
        }
        अन्यथा
        {
            xReturn = errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY;
        }

        परत xReturn;
    }

#endif /* CONFIG_SPIRAM */
/*----------------------------------------------------------*/
