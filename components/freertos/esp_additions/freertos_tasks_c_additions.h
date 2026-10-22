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
_Static_assert( offsetof( StaticTask_t, pxDummy6 ) == offsetof( TCB_t, px_राशिः ) );
_Static_assert( offsetof( StaticTask_t, pxDummy8 ) == offsetof( TCB_t, px_अन्तः_सम्बन्धि_राशिः ) );
#यदि !CONFIG_IDF_TARGET_LINUX    // Disabled for linux builds due to differences in types
_Static_assert( tskNO_AFFINITY == ( आधारः_Type_t ) CONFIG_FREERTOS_NO_AFFINITY, "CONFIG_FREERTOS_NO_AFFINITY must be the same as tskNO_AFFINITY" );
#यदि_अन्तः

/* ------------------------------------------------- Kernel Control ------------------------------------------------- */

#यदि ( !CONFIG_FREERTOS_SMP && ( config_अङ्कः_CORES > 1 ) )

/*
 * Wrapper function to take "xKerneLock"
 */
    शून्य prv_Take_Kernel_तालकम्( शून्य )
    {
        /* We call the tasks.c critical section macro to take xKernelLock */
        taskENTER_CRITICAL( &x_Kernel_तालकम् );
    }

#यदि_अन्तः /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#यदि ( !CONFIG_FREERTOS_SMP && ( config_अङ्कः_CORES > 1 ) )

/*
 * Wrapper function to release "xKerneLock"
 */
    शून्य prv_Release_Kernel_तालकम्( शून्य )
    {
        /* We call the tasks.c critical section macro to release xKernelLock */
        taskEXIT_CRITICAL( &x_Kernel_तालकम् );
    }

#यदि_अन्तः /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#यदि ( CONFIG_FREERTOS_SMP && ( config_अङ्कः_CORES > 1 ) )

/*
 * Workaround for non-thread safe multi-core OS startup (see IDF-4524)
 */
    शून्य prvStartSchedulerOtherCores( शून्य )
    {
        /* This function is always called with interrupts disabled*/
        xSchedulerRunning = pdTRUE;
    }

#यदि_अन्तः /* ( CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#यदि ( !CONFIG_FREERTOS_SMP && ( config_अङ्कः_CORES > 1 ) )

    आधारः_Type_t xTaskIncrementTickOtherCores( शून्य )
    {
        /* Minor optimization. This function can never switch cores mid
         * execution */
        आधारः_Type_t xCoreID = पत्तनम्_GET_CORE_ID();
        आधारः_Type_t xSwitchRequired = pdFALSE;

        /* This function should never be called by Core 0. */
        configASSERT( xCoreID != 0 );

        /* Called by the portable layer each time a tick interrupt occurs
         * on a core other than core 0. */
        traceTASK_INCREMENT_TICK( x_Tick_गणनम् );

        यदि( uxSchedulerSuspended[ xCoreID ] == ( UBaseType_t ) 0U )
        {
            /* We need take the kernel lock here as we are about to access
             * kernel data structures. */
            taskENTER_CRITICAL_ISR( &x_Kernel_तालकम् );

            /* Tasks of equal priority to the currently running task will share
             * processing time (time slice) if preemption is on, and the application
             * writer has not explicitly turned time slicing off. */
            #यदि ( ( configUSE_PREEMPTION == 1 ) && ( configUSE_TIME_SLICING == 1 ) )
            {
                यदि( list_CURRENT_LIST_दीर्घता( &( pxReadyTasksLists[ pxCurrentTCBs[ xCoreID ]->ux_प्राथमिकता ] ) ) > ( UBaseType_t ) 1 )
                {
                    xSwitchRequired = pdTRUE;
                }
                अन्यथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            #यदि_अन्तः /* ( ( configUSE_PREEMPTION == 1 ) && ( configUSE_TIME_SLICING == 1 ) ) */

            /* Release the previously taken kernel lock as we have finished
             * accessing the kernel data structures. */
            taskEXIT_CRITICAL_ISR( &x_Kernel_तालकम् );

            #यदि ( configUSE_PREEMPTION == 1 )
            {
                यदि( xYieldPending[ xCoreID ] != pdFALSE )
                {
                    xSwitchRequired = pdTRUE;
                }
                अन्यथा
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            #यदि_अन्तः /* configUSE_PREEMPTION */
        }

        #यदि ( configUSE_TICK_HOOK == 1 )
        {
            vApplicationTickHook();
        }
        #यदि_अन्तः

        प्रत्यागम xSwitchRequired;
    }

#यदि_अन्तः /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

/* -------------------------------------------------- Task Creation ------------------------------------------------- */

#यदि ( config_SUPPORT_DYNAMIC_आवण्टनम् == 1 )

    आधारः_Type_t x_Task_Create_Pinned_गन्तव्यम्_Core( TaskFunction_t px_Task_संहिता,
                                        ध्रुव वर्ण * ध्रुव pcName,
                                        ध्रुव uint32_t us_राशिः_गभीरता,
                                        शून्य * ध्रुव pvParameters,
                                        UBaseType_t ux_प्राथमिकता,
                                        TaskHandle_t * ध्रुव pxCreatedTask,
                                        ध्रुव आधारः_Type_t xCoreID )
    {
        आधारः_Type_t xReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );

        #यदि CONFIG_FREERTOS_SMP
        {
            /* If using Amazon SMP FreeRTOS. This function is just a wrapper around
             * xTaskCreate() or xTaskCreateAffinitySet(). */
            #यदि ( ( config_अङ्कः_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
            {
                /* Convert xCoreID into an affinity mask */
                UBaseType_t ux_Core_Affinity_आवरणम्;

                /* Bit shifting << xCoreID is only valid if we have less than
                 * 32 cores. */
                ESP_STATIC_ASSERT( config_अङ्कः_CORES < 32 );

                यदि( xCoreID == tskNO_AFFINITY )
                {
                    ux_Core_Affinity_आवरणम् = tskNO_AFFINITY;
                }
                अन्यथा
                {
                    ux_Core_Affinity_आवरणम् = ( 1 << xCoreID );
                }

                xReturn = x_Task_Create_Affinity_समुच्चयः( px_Task_संहिता, pcName, us_राशिः_गभीरता, pvParameters, ux_प्राथमिकता, ux_Core_Affinity_आवरणम्, pxCreatedTask );
            }
            #अन्यथा /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            {
                xReturn = xTaskCreate( px_Task_संहिता, pcName, us_राशिः_गभीरता, pvParameters, ux_प्राथमिकता, pxCreatedTask );
            }
            #यदि_अन्तः /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
        }
        #अन्यथा /* CONFIG_FREERTOS_SMP */
        {
            TCB_t * pxNewTCB;

            /* If the stack grows down then allocate the stack then the TCB so the
             * stack does not grow into the TCB.  Likewise if the stack grows up
             * then allocate the TCB then the stack. */
            #यदि ( पत्तनम्_राशिः_GROWTH > 0 )
            {
                /* Allocate space for the TCB.  Where the memory comes from depends on
                 * the implementation of the port malloc function and whether or not static
                 * allocation is being used. */
                pxNewTCB = ( TCB_t * ) pv_पत्तनम्_Malloc( परिमाण( TCB_t ) );

                यदि( pxNewTCB != रिक्त )
                {
                    स्मृतिं_पूरय( ( शून्य * ) pxNewTCB, 0x00, परिमाण( TCB_t ) );

                    /* Allocate space for the stack used by the task being created.
                     * The base of the stack memory stored in the TCB so the task can
                     * be deleted later if required. */
                    pxNewTCB->px_राशिः = ( राशिः_Type_t * ) pv_पत्तनम्_Malloc_राशिः( ( ( ( size_t ) us_राशिः_गभीरता ) * परिमाण( राशिः_Type_t ) ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */

                    यदि( pxNewTCB->px_राशिः == रिक्त )
                    {
                        /* Could not allocate the stack.  Delete the allocated TCB. */
                        v_पत्तनम्_Free( pxNewTCB );
                        pxNewTCB = रिक्त;
                    }
                }
            }
            #अन्यथा /* portSTACK_GROWTH */
            {
                राशिः_Type_t * px_राशिः;

                /* Allocate space for the stack used by the task being created. */
                px_राशिः = pv_पत्तनम्_Malloc_राशिः( ( ( ( size_t ) us_राशिः_गभीरता ) * परिमाण( राशिः_Type_t ) ) ); /*lint !e9079 All values returned by pvPortMalloc() have at least the alignment required by the MCU's stack and this allocation is the stack. */

                यदि( px_राशिः != रिक्त )
                {
                    /* Allocate space for the TCB. */
                    pxNewTCB = ( TCB_t * ) pv_पत्तनम्_Malloc( परिमाण( TCB_t ) ); /*lint !e9087 !e9079 All values returned by pvPortMalloc() have at least the alignment required by the MCU's stack, and the first member of TCB_t is always a pointer to the task's stack. */

                    यदि( pxNewTCB != रिक्त )
                    {
                        स्मृतिं_पूरय( ( शून्य * ) pxNewTCB, 0x00, परिमाण( TCB_t ) );

                        /* Store the stack location in the TCB. */
                        pxNewTCB->px_राशिः = px_राशिः;
                    }
                    अन्यथा
                    {
                        /* The stack cannot be used as the TCB was not created.  Free
                         * it again. */
                        v_पत्तनम्_Free_राशिः( px_राशिः );
                    }
                }
                अन्यथा
                {
                    pxNewTCB = रिक्त;
                }
            }
            #यदि_अन्तः /* portSTACK_GROWTH */

            यदि( pxNewTCB != रिक्त )
            {
                #यदि ( tsk_STATIC_AND_DYNAMIC_आवण्टनम्_POSSIBLE != 0 ) /*lint !e9029 !e731 Macro has been consolidated for readability reasons. */
                {
                    /* Tasks can be created statically or dynamically, so note this
                     * task was created dynamically in case it is later deleted. */
                    pxNewTCB->ucStaticallyAllocated = tsk_DYNAMICALLY_ALLOCATED_राशिः_AND_TCB;
                }
                #यदि_अन्तः /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

                prvInitialiseNewTask( px_Task_संहिता, pcName, ( uint32_t ) us_राशिः_गभीरता, pvParameters, ux_प्राथमिकता, pxCreatedTask, pxNewTCB, रिक्त, xCoreID );
                prv_सङ्कलय_New_Task_गन्तव्यम्_Ready_List( pxNewTCB );
                xReturn = pdPASS;
            }
            अन्यथा
            {
                xReturn = err_COULD_NOT_ALLOCATE_REQUIRED_स्मृतिः;
            }
        }
        #यदि_अन्तः /* CONFIG_FREERTOS_SMP */

        प्रत्यागम xReturn;
    }

#यदि_अन्तः /* ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#यदि ( config_SUPPORT_STATIC_आवण्टनम् == 1 )

    TaskHandle_t x_Task_Create_Static_Pinned_गन्तव्यम्_Core( TaskFunction_t px_Task_संहिता,
                                                ध्रुव वर्ण * ध्रुव pcName,
                                                ध्रुव uint32_t ul_राशिः_गभीरता,
                                                शून्य * ध्रुव pvParameters,
                                                UBaseType_t ux_प्राथमिकता,
                                                राशिः_Type_t * ध्रुव pux_राशिः_बफर,
                                                StaticTask_t * ध्रुव px_Task_बफर,
                                                ध्रुव आधारः_Type_t xCoreID )
    {
        TaskHandle_t xReturn;

        configASSERT( पत्तनम्_VALID_राशिः_MEM( pux_राशिः_बफर ) );
        configASSERT( पत्तनम्_VALID_TCB_MEM( px_Task_बफर ) );
        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );

        #यदि CONFIG_FREERTOS_SMP
        {
            /* If using Amazon SMP FreeRTOS. This function is just a wrapper around
             * xTaskCreateStatic() or xTaskCreateStaticAffinitySet(). */
            #यदि ( ( config_अङ्कः_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
            {
                /* Convert xCoreID into an affinity mask */
                UBaseType_t ux_Core_Affinity_आवरणम्;

                /* Bit shifting << xCoreID is only valid if we have less than
                 * 32 cores. */
                ESP_STATIC_ASSERT( config_अङ्कः_CORES < 32 );

                यदि( xCoreID == tskNO_AFFINITY )
                {
                    ux_Core_Affinity_आवरणम् = tskNO_AFFINITY;
                }
                अन्यथा
                {
                    ux_Core_Affinity_आवरणम् = ( 1 << xCoreID );
                }

                xReturn = x_Task_Create_Static_Affinity_समुच्चयः( px_Task_संहिता, pcName, ul_राशिः_गभीरता, pvParameters, ux_प्राथमिकता, pux_राशिः_बफर, px_Task_बफर, ux_Core_Affinity_आवरणम् );
            }
            #अन्यथा /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            {
                xReturn = xTaskCreateStatic( px_Task_संहिता, pcName, ul_राशिः_गभीरता, pvParameters, ux_प्राथमिकता, pux_राशिः_बफर, px_Task_बफर );
            }
            #यदि_अन्तः /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
        }
        #अन्यथा /* CONFIG_FREERTOS_SMP */
        {
            TCB_t * pxNewTCB;

            #यदि ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticTask_t equals the size of the real task
                 * structure. */
                चञ्चल size_t x_आकार = परिमाण( StaticTask_t );
                configASSERT( x_आकार == परिमाण( TCB_t ) );
                ( शून्य ) x_आकार; /* Prevent lint warning when configASSERT() is not used. */
            }
            #यदि_अन्तः /* configASSERT_DEFINED */

            यदि( ( px_Task_बफर != रिक्त ) && ( pux_राशिः_बफर != रिक्त ) )
            {
                /* The memory used for the task's TCB and stack are passed into this
                 * function - use them. */
                pxNewTCB = ( TCB_t * ) px_Task_बफर; /*lint !e740 !e9087 Unusual cast is ok as the structures are designed to have the same alignment, and the size is checked by an assert. */
                स्मृतिं_पूरय( ( शून्य * ) pxNewTCB, 0x00, परिमाण( TCB_t ) );
                pxNewTCB->px_राशिः = ( राशिः_Type_t * ) pux_राशिः_बफर;

                #यदि ( tsk_STATIC_AND_DYNAMIC_आवण्टनम्_POSSIBLE != 0 ) /*lint !e731 !e9029 Macro has been consolidated for readability reasons. */
                {
                    /* Tasks can be created statically or dynamically, so note this
                     * task was created statically in case the task is later deleted. */
                    pxNewTCB->ucStaticallyAllocated = tsk_STATICALLY_ALLOCATED_राशिः_AND_TCB;
                }
                #यदि_अन्तः /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

                prvInitialiseNewTask( px_Task_संहिता, pcName, ul_राशिः_गभीरता, pvParameters, ux_प्राथमिकता, &xReturn, pxNewTCB, रिक्त, xCoreID );
                prv_सङ्कलय_New_Task_गन्तव्यम्_Ready_List( pxNewTCB );
            }
            अन्यथा
            {
                xReturn = रिक्त;
            }
        }
        #यदि_अन्तः /* CONFIG_FREERTOS_SMP */

        प्रत्यागम xReturn;
    }

#यदि_अन्तः /* ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#यदि ( configUSE_TIMERS == 1 )

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
    आधारः_Type_t __attribute__( ( weak ) ) xTimerCreateTimerTask( शून्य )
    {
        प्रत्यागम pdPASS;
    }

#यदि_अन्तः /* configUSE_TIMERS */
/*----------------------------------------------------------*/

/* ------------------------------------------------- Task Utilities ------------------------------------------------- */

आधारः_Type_t xTaskGetCoreID( TaskHandle_t xTask )
{
    आधारः_Type_t xReturn;

    #यदि ( config_अङ्कः_CORES > 1 )
    {
        #यदि CONFIG_FREERTOS_SMP
            UBaseType_t ux_Core_Affinity_आवरणम्;

            /* Get the core affinity mask and convert it to an ID */
            ux_Core_Affinity_आवरणम् = vTaskCoreAffinityGet( xTask );

            /* If the task is not pinned to a particular core, treat it as tskNO_AFFINITY */
            यदि( ux_Core_Affinity_आवरणम् & ( ux_Core_Affinity_आवरणम् - 1 ) ) /* If more than one bit set */
            {
                xReturn = tskNO_AFFINITY;
            }
            अन्यथा
            {
                पूर्णाङ्क i_सूचकांक_Plus_One = __builtin_ffs( ux_Core_Affinity_आवरणम् );
                प्रतिज्ञा( i_सूचकांक_Plus_One >= 1 );
                xReturn = i_सूचकांक_Plus_One - 1;
            }
        #अन्यथा /* CONFIG_FREERTOS_SMP */
            TCB_t * pxTCB;

            pxTCB = prvGetTCBFromHandle( xTask );

            xReturn = pxTCB->xCoreID;
        #यदि_अन्तः /* CONFIG_FREERTOS_SMP */
    }
    #अन्यथा /* configNUM_CORES > 1 */
    {
        /* Single-core. Just return a core ID of 0 */
        xReturn = 0;
    }
    #यदि_अन्तः /* configNUM_CORES > 1 */

    प्रत्यागम xReturn;
}
/*----------------------------------------------------------*/

#यदि ( ( !CONFIG_FREERTOS_SMP ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

    TaskHandle_t xTaskGetIdleTaskHandleForCore( आधारः_Type_t xCoreID )
    {
        /* If xTaskGetIdleTaskHandle() is called before the scheduler has been
         * started, then xIdleTaskHandle will be NULL. */
        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );
        configASSERT( ( xIdleTaskHandle[ xCoreID ] != रिक्त ) );
        प्रत्यागम xIdleTaskHandle[ xCoreID ];
    }

#यदि_अन्तः /* ( ( !CONFIG_FREERTOS_SMP ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*----------------------------------------------------------*/

#यदि ( ( !CONFIG_FREERTOS_SMP ) && ( ( INCLUDE_xTaskGetCurrentTaskHandle == 1 ) || ( configUSE_MUTEXES == 1 ) ) )

    TaskHandle_t xTaskGetCurrentTaskHandleForCore( आधारः_Type_t xCoreID )
    {
        TaskHandle_t xReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

        #यदि ( CONFIG_FREERTOS_SMP )
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
        #यदि_अन्तः /* CONFIG_FREERTOS_SMP */

        प्रत्यागम xReturn;
    }

#यदि_अन्तः /* ( ( !CONFIG_FREERTOS_SMP ) && ( ( INCLUDE_xTaskGetCurrentTaskHandle == 1 ) || ( configUSE_MUTEXES == 1 ) ) ) */
/*----------------------------------------------------------*/

#यदि ( !CONFIG_FREERTOS_SMP && ( config_GENERATE_सततम्_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

    config_सततम्_TIME_गणक_TYPE ul_Task_Get_Idle_सततम्_Time_गणक_For_Core( आधारः_Type_t xCoreID )
    {
        config_सततम्_TIME_गणक_TYPE ul_सततम्_Time_गणक;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

        /* For SMP, we need to take the kernel lock here as we are about to
         * access kernel data structures. */
        prvENTER_CRITICAL_SMP_ONLY( &x_Kernel_तालकम् );
        {
            ul_सततम्_Time_गणक = xIdleTaskHandle[ xCoreID ]->ul_सततम्_Time_गणक;
        }
        /* Release the previously taken kernel lock. */
        prvEXIT_CRITICAL_SMP_ONLY( &x_Kernel_तालकम् );

        प्रत्यागम ul_सततम्_Time_गणक;
    }

#यदि_अन्तः /* ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*----------------------------------------------------------*/

#यदि ( !CONFIG_FREERTOS_SMP && ( config_GENERATE_सततम्_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

    config_सततम्_TIME_गणक_TYPE ul_Task_Get_Idle_सततम्_Time_Percent_For_Core( आधारः_Type_t xCoreID )
    {
        config_सततम्_TIME_गणक_TYPE ul_योग_Time, ulReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

        #यदि_परिभाषितम् पत्तनम्_ALT_GET_सततम्_TIME_गणक_मानम्
            पत्तनम्_ALT_GET_सततम्_TIME_गणक_मानम्( ul_योग_Time );
        #अन्यथा
            ul_योग_Time = पत्तनम्_GET_सततम्_TIME_गणक_मानम्();
        #यदि_अन्तः

        /* For percentage calculations. */
        ul_योग_Time /= ( config_सततम्_TIME_गणक_TYPE ) 100;

        /* Avoid divide by zero errors. */
        यदि( ul_योग_Time > ( config_सततम्_TIME_गणक_TYPE ) 0 )
        {
            /* For SMP, we need to take the kernel lock here as we are about
             * to access kernel data structures. */
            prvENTER_CRITICAL_SMP_ONLY( &x_Kernel_तालकम् );
            {
                ulReturn = xIdleTaskHandle[ xCoreID ]->ul_सततम्_Time_गणक / ul_योग_Time;
            }
            /* Release the previously taken kernel lock. */
            prvEXIT_CRITICAL_SMP_ONLY( &x_Kernel_तालकम् );
        }
        अन्यथा
        {
            ulReturn = 0;
        }

        प्रत्यागम ulReturn;
    }

#यदि_अन्तः /* ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*-----------------------------------------------------------*/

राशिः_Type_t * x_Task_Get_राशिः_Start( TaskHandle_t xTask )
{
    TCB_t * pxTCB;

    pxTCB = prvGetTCBFromHandle( xTask );
    प्रत्यागम pxTCB->px_राशिः;
}
/*----------------------------------------------------------*/

uint8_t * px_Task_Get_राशिः_Start( TaskHandle_t xTask )
{
    प्रत्यागम (uint8_t *)x_Task_Get_राशिः_Start( xTask );
}
/*----------------------------------------------------------*/

#यदि ( INCLUDE_v_Task_प्राथमिकता_समुच्चयः == 1 )

    शून्य prv_Task_प्राथमिकता_Raise( prv_Task_Saved_प्राथमिकता_t * px_Saved_प्राथमिकता,
                               UBaseType_t ux_New_प्राथमिकता )
    {
        TCB_t * pxTCB;
        UBaseType_t ux_प्राथमिकता_प्रयुक्तम्_On_प्रविष्टिः;

        configASSERT( ( ux_New_प्राथमिकता < configMAX_PRIORITIES ) );

        /* Ensure the new priority is valid. */
        यदि( ux_New_प्राथमिकता >= ( UBaseType_t ) configMAX_PRIORITIES )
        {
            ux_New_प्राथमिकता = ( UBaseType_t ) configMAX_PRIORITIES - ( UBaseType_t ) 1U;
        }

        #यदि CONFIG_FREERTOS_SMP
            taskENTER_CRITICAL();
        #अन्यथा
            taskENTER_CRITICAL( &x_Kernel_तालकम् );
        #यदि_अन्तः
        {
            pxTCB = prvGetTCBFromHandle( रिक्त );

            #यदि ( configUSE_MUTEXES == 1 )
            {
                px_Saved_प्राथमिकता->ux_प्राथमिकता = pxTCB->ux_प्राथमिकता;
                px_Saved_प्राथमिकता->ux_आधारः_प्राथमिकता = pxTCB->ux_आधारः_प्राथमिकता;

                /* If uxNewPriority < uxBasePriority, then there is nothing else to
                 * do, as uxBasePriority is always <= uxPriority. */
                यदि( ux_New_प्राथमिकता > pxTCB->ux_आधारः_प्राथमिकता )
                {
                    pxTCB->ux_आधारः_प्राथमिकता = ux_New_प्राथमिकता;

                    /* Remember the task's current priority before attempting to
                     * change it. If the task's current priority is changed, it must
                     * be done so before moving the task between task lists) in order
                     * for the taskRESET_READY_PRIORITY() macro to function correctly. */
                    ux_प्राथमिकता_प्रयुक्तम्_On_प्रविष्टिः = pxTCB->ux_प्राथमिकता;

                    यदि( ux_New_प्राथमिकता > pxTCB->ux_प्राथमिकता )
                    {
                        pxTCB->ux_प्राथमिकता = ux_New_प्राथमिकता;

                        /* Only reset the event list item value if the value is not
                         * being used for anything else. */
                        यदि( ( list_GET_LIST_ITEM_मानम्( &( pxTCB->xEventListItem ) ) & task_EVENT_LIST_ITEM_मानम्_अन्तर्_USE ) == 0UL )
                        {
                            list_समुच्चयः_LIST_ITEM_मानम्( &( pxTCB->xEventListItem ), ( ( TickType_t ) configMAX_PRIORITIES - ( TickType_t ) ux_New_प्राथमिकता ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */
                        }

                        /* If the task is in the blocked or suspended list we need do
                         * nothing more than change its priority variable. However, if
                         * the task is in a ready list it needs to be removed and placed
                         * in the list appropriate to its new priority. */
                        यदि( listIS_CONTAINED_WITHIN( &( pxReadyTasksLists[ ux_प्राथमिकता_प्रयुक्तम्_On_प्रविष्टिः ] ), &( pxTCB->xStateListItem ) ) != pdFALSE )
                        {
                            /* The task is currently in its ready list - remove before
                             * adding it to its new ready list.  As we are in a critical
                             * section we can do this even if the scheduler is suspended. */
                            यदि( uxListRemove( &( pxTCB->xStateListItem ) ) == ( UBaseType_t ) 0 )
                            {
                                /* It is known that the task is in its ready list so
                                 * there is no need to check again and the port level
                                 * reset macro can be called directly. */
                                पत्तनम्_RESET_READY_प्राथमिकता( ux_प्राथमिकता_प्रयुक्तम्_On_प्रविष्टिः, ux_शिखरम्_Ready_प्राथमिकता );
                            }

                            prv_सङ्कलय_Task_गन्तव्यम्_Ready_List( pxTCB );
                        }
                    }
                }
            }
            #अन्यथा /* if ( configUSE_MUTEXES == 1 ) */
            {
                px_Saved_प्राथमिकता->ux_प्राथमिकता = pxTCB->ux_प्राथमिकता;

                यदि( ux_New_प्राथमिकता > pxTCB->ux_प्राथमिकता )
                {
                    v_Task_प्राथमिकता_समुच्चयः( रिक्त, ux_New_प्राथमिकता );
                }
            }
            #यदि_अन्तः /* if ( configUSE_MUTEXES == 1 ) */
        }
        #यदि CONFIG_FREERTOS_SMP
            taskEXIT_CRITICAL();
        #अन्यथा
            taskEXIT_CRITICAL( &x_Kernel_तालकम् );
        #यदि_अन्तः
    }

#यदि_अन्तः /* INCLUDE_vTaskPrioritySet == 1 */
/*----------------------------------------------------------*/

#यदि ( INCLUDE_v_Task_प्राथमिकता_समुच्चयः == 1 )

    शून्य prv_Task_प्राथमिकता_Restore( prv_Task_Saved_प्राथमिकता_t * px_Saved_प्राथमिकता )
    {
        TCB_t * pxTCB;
        UBaseType_t ux_New_प्राथमिकता;
        UBaseType_t ux_प्राथमिकता_प्रयुक्तम्_On_प्रविष्टिः;
        UBaseType_t ux_आधारः_प्राथमिकता_प्रयुक्तम्_On_प्रविष्टिः;
        आधारः_Type_t xYieldRequired = pdFALSE;

        #यदि CONFIG_FREERTOS_SMP
            taskENTER_CRITICAL();
        #अन्यथा
            taskENTER_CRITICAL( &x_Kernel_तालकम् );
        #यदि_अन्तः
        {
            pxTCB = prvGetTCBFromHandle( रिक्त );

            #यदि ( configUSE_MUTEXES == 1 )
            {
                /* If the saved uxBasePriority == the task's uxBasePriority, it means
                 * that prvTaskPriorityRaise() never raised the task's uxBasePriority.
                 * In that case, there is nothing else to do. */
                यदि( px_Saved_प्राथमिकता->ux_आधारः_प्राथमिकता != pxTCB->ux_आधारः_प्राथमिकता )
                {
                    ux_आधारः_प्राथमिकता_प्रयुक्तम्_On_प्रविष्टिः = pxTCB->ux_आधारः_प्राथमिकता;
                    pxTCB->ux_आधारः_प्राथमिकता = px_Saved_प्राथमिकता->ux_आधारः_प्राथमिकता;

                    /* Remember the task's current priority before attempting to
                     * change it. If the task's current priority is changed, it must
                     * be done so before moving the task between task lists in order
                     * for the taskRESET_READY_PRIORITY() macro to function correctly. */
                    ux_प्राथमिकता_प्रयुक्तम्_On_प्रविष्टिः = pxTCB->ux_प्राथमिकता;

                    /* Check if the task inherited a priority after prvTaskPriorityRaise().
                     * If this is the case, there is nothing else to do. The priority
                     * will be restored when the task disinherits its priority. */
                    यदि( pxTCB->ux_प्राथमिकता == ux_आधारः_प्राथमिकता_प्रयुक्तम्_On_प्रविष्टिः )
                    {
                        यदि( pxTCB->uxMutexesHeld == 0 )
                        {
                            /* The task may have inherited a priority before prvTaskPriorityRaise()
                             * then disinherited a priority after prvTaskPriorityRaise().
                             * Thus we need set the uxPriority to the saved base priority
                             * so that the task's priority gets restored to the priority
                             * before any inheritance or raising. */
                            pxTCB->ux_प्राथमिकता = px_Saved_प्राथमिकता->ux_आधारः_प्राथमिकता;
                        }
                        अन्यथा
                        {
                            /* The task may have inherited a priority before prvTaskPriorityRaise()
                             * was called. Thus, we need to restore uxPriority to the
                             * "saved uxPriority" so that the task still retains that
                             * inherited priority. */
                            pxTCB->ux_प्राथमिकता = px_Saved_प्राथमिकता->ux_प्राथमिकता;
                        }

                        ux_New_प्राथमिकता = pxTCB->ux_प्राथमिकता;

                        यदि( ux_New_प्राथमिकता < ux_प्राथमिकता_प्रयुक्तम्_On_प्रविष्टिः )
                        {
                            /* Setting the priority of the running task down means
                             * there may now be another task of higher priority that
                             * is ready to execute. */
                            xYieldRequired = pdTRUE;
                        }

                        /* Only reset the event list item value if the value is not
                         * being used for anything else. */
                        यदि( ( list_GET_LIST_ITEM_मानम्( &( pxTCB->xEventListItem ) ) & task_EVENT_LIST_ITEM_मानम्_अन्तर्_USE ) == 0UL )
                        {
                            list_समुच्चयः_LIST_ITEM_मानम्( &( pxTCB->xEventListItem ), ( ( TickType_t ) configMAX_PRIORITIES - ( TickType_t ) ux_New_प्राथमिकता ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */
                        }

                        /* If the task is in the blocked or suspended list we need do
                         * nothing more than change its priority variable. However, if
                         * the task is in a ready list it needs to be removed and placed
                         * in the list appropriate to its new priority. */
                        यदि( listIS_CONTAINED_WITHIN( &( pxReadyTasksLists[ ux_प्राथमिकता_प्रयुक्तम्_On_प्रविष्टिः ] ), &( pxTCB->xStateListItem ) ) != pdFALSE )
                        {
                            /* The task is currently in its ready list - remove before
                             * adding it to its new ready list.  As we are in a critical
                             * section we can do this even if the scheduler is suspended. */
                            यदि( uxListRemove( &( pxTCB->xStateListItem ) ) == ( UBaseType_t ) 0 )
                            {
                                /* It is known that the task is in its ready list so
                                 * there is no need to check again and the port level
                                 * reset macro can be called directly. */
                                पत्तनम्_RESET_READY_प्राथमिकता( ux_प्राथमिकता_प्रयुक्तम्_On_प्रविष्टिः, ux_शिखरम्_Ready_प्राथमिकता );
                            }

                            prv_सङ्कलय_Task_गन्तव्यम्_Ready_List( pxTCB );
                        }

                        यदि( xYieldRequired != pdFALSE )
                        {
                            #यदि CONFIG_FREERTOS_SMP
                                taskYIELD_TASK_CORE_IF_USING_PREEMPTION( pxTCB );
                            #अन्यथा
                                taskYIELD_IF_USING_PREEMPTION();
                            #यदि_अन्तः
                        }
                    }
                }
            }
            #अन्यथा /* if ( configUSE_MUTEXES == 1 ) */
            {
                v_Task_प्राथमिकता_समुच्चयः( रिक्त, px_Saved_प्राथमिकता->ux_प्राथमिकता );
            }
            #यदि_अन्तः /* if ( configUSE_MUTEXES == 1 ) */
        }
        #यदि CONFIG_FREERTOS_SMP
            taskEXIT_CRITICAL();
        #अन्यथा
            taskEXIT_CRITICAL( &x_Kernel_तालकम् );
        #यदि_अन्तः
    }

#यदि_अन्तः /* ( INCLUDE_vTaskPrioritySet == 1 ) */
/*----------------------------------------------------------*/

/* --------------------------------------------- TLSP Deletion Callbacks -------------------------------------------- */

#यदि CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS

    शून्य v_Task_समुच्चयः_Thread_Local_Storage_Pointer_And_Del_Callback( TaskHandle_t x_Task_गन्तव्यम्_समुच्चयः,
                                                          आधारः_Type_t x_सूचकांक,
                                                          शून्य * pv_मानम्,
                                                          TlsDeleteCallbackFunction_t pvDelCallback )
    {
        /* If TLSP deletion callbacks are enabled, then configNUM_THREAD_LOCAL_STORAGE_POINTERS
         * is doubled in size so that the latter half of the pvThreadLocalStoragePointers
         * stores the deletion callbacks. */
        यदि( x_सूचकांक < ( config_अङ्कः_THREAD_LOCAL_STORAGE_सूचकाः / 2 ) )
        {
            TCB_t * pxTCB;

            #यदि ( config_अङ्कः_CORES > 1 )
            {
                /* For SMP, we need a critical section as another core could also
                 * update this task's TLSP at the same time. */
                #यदि CONFIG_FREERTOS_SMP
                {
                    taskENTER_CRITICAL();
                }
                #अन्यथा /* CONFIG_FREERTOS_SMP */
                {
                    taskENTER_CRITICAL( &x_Kernel_तालकम् );
                }
                #यदि_अन्तः /* CONFIG_FREERTOS_SMP */
            }
            #यदि_अन्तः /* configNUM_CORES > 1 */

            pxTCB = prvGetTCBFromHandle( x_Task_गन्तव्यम्_समुच्चयः );
            /* Store the TLSP by indexing the first half of the array */
            pxTCB->pv_Thread_Local_Storage_सूचकाः[ x_सूचकांक ] = pv_मानम्;

            /* Store the TLSP deletion callback by indexing the second half
             * of the array. */
            pxTCB->pv_Thread_Local_Storage_सूचकाः[ ( x_सूचकांक + ( config_अङ्कः_THREAD_LOCAL_STORAGE_सूचकाः / 2 ) ) ] = ( शून्य * ) pvDelCallback;

            #यदि ( config_अङ्कः_CORES > 1 )
            {
                #यदि CONFIG_FREERTOS_SMP
                {
                    taskEXIT_CRITICAL();
                }
                #अन्यथा /* CONFIG_FREERTOS_SMP */
                {
                    taskEXIT_CRITICAL( &x_Kernel_तालकम् );
                }
                #यदि_अन्तः /* CONFIG_FREERTOS_SMP */
            }
            #यदि_अन्तः /* configNUM_CORES > 1 */
        }
    }

#यदि_अन्तः /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */
/*----------------------------------------------------------*/

/* ----------------------------------------------------- Newlib ----------------------------------------------------- */

#यदि ( configUSE_NEWLIB_REENTRANT == 1 )

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

        यदि( pxCurTask == रिक्त )
        {
            /* No task running. Return global struct. */
            ret = _GLOBAL_REENT;
        }
        अन्यथा
        {
            /* We have a currently executing task. Return its reentrant struct. */
            ret = &pxCurTask->xTLSBlock;
        }

        प्रत्यागम ret;
    }

#यदि_अन्तः /* configUSE_NEWLIB_REENTRANT == 1 */

/* -------------------------------------------------- Task Snapshot ------------------------------------------------- */

/**
 * @brief List of all task lists in FreeRTOS
 *
 * @note There are currently differing number of task list between SMP FreeRTOS and ESP-IDF FreeRTOS
 */
स्थिर List_t * non_ready_task_lists[] =
{
    #यदि_परिभाषितम् CONFIG_FREERTOS_SMP
        &xPendingReadyList,
    #अन्यथा /* CONFIG_FREERTOS_SMP */
        &xPendingReadyList[ 0 ],
        #यदि_अपरिभाषितम् CONFIG_FREERTOS_UNICORE
            &xPendingReadyList[ 1 ],
        #यदि_अन्तः /* CONFIG_FREERTOS_UNICORE */
    #यदि_अन्तः /* CONFIG_FREERTOS_SMP */
    &xDelayedTaskList1,
    &xDelayedTaskList2,
    #यदि ( INCLUDE_vTaskDelete == 1 )
        &x_Tasks_प्रतीक्षा_Termination,
    #यदि_अन्तः
    #यदि ( INCLUDE_vTaskSuspend == 1 )
        &xSuspendedTaskList,
    #यदि_अन्तः
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
स्थिर List_t * px_Get_Task_List_By_सूचकांक( UBaseType_t ux_List_सूचकांक )
{
    List_t * pxTaskList;
    ध्रुव size_t xNonReadyTaskListsCnt = ( परिमाण( non_ready_task_lists ) / परिमाण( List_t * ) );

    यदि( ux_List_सूचकांक < configMAX_PRIORITIES )
    {
        pxTaskList = &pxReadyTasksLists[ configMAX_PRIORITIES - 1 - ux_List_सूचकांक ];
    }
    अन्यथा यदि( ux_List_सूचकांक < configMAX_PRIORITIES + xNonReadyTaskListsCnt )
    {
        pxTaskList = non_ready_task_lists[ ux_List_सूचकांक - configMAX_PRIORITIES ];
    }
    अन्यथा
    {
        pxTaskList = रिक्त;
    }

    /* sanity check */
    यदि( pxTaskList )
    {
        यदि( !पत्तनम्_VALID_LIST_MEM( pxTaskList ) )
        {
            pxTaskList = रिक्त;
        }
    }

    प्रत्यागम pxTaskList;
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
स्थिर अन्तःस्थ UBaseType_t px_Get_Task_List_गणनम्( शून्य )
{
    प्रत्यागम configMAX_PRIORITIES + ( परिमाण( non_ready_task_lists ) / परिमाण( List_t * ) );
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
पूर्णाङ्क x_Task_Get_अग्र( TaskIterator_t * xIterator )
{
    यदि( !xIterator )
    {
        प्रत्यागम -1;
    }

    ListItem_t * px_अग्र_List_Item = xIterator->px_अग्र_List_Item;
    UBaseType_t uxCurListIdx = xIterator->ux_Current_List_सूचकांक;
    UBaseType_t uxMaxListIdx = px_Get_Task_List_गणनम्();

    प्रति( ; uxCurListIdx < uxMaxListIdx; ++uxCurListIdx )
    {
        List_t * pxCurrentTaskList = px_Get_Task_List_By_सूचकांक( uxCurListIdx );

        यदि( !pxCurrentTaskList || ( list_CURRENT_LIST_दीर्घता( pxCurrentTaskList ) == 0 ) )
        {
            अनुवर्त;
        }

        ध्रुव ListItem_t * pxCurrListItem = list_GET_अन्तः_MARKER( pxCurrentTaskList );

        यदि( !px_अग्र_List_Item )
        {
            /* We are here if the traversal starts from the beginning or when we finish traversing
             * for one of the state lists
             */
            px_अग्र_List_Item = list_GET_अग्र( pxCurrListItem );
        }

        यदि( !पत्तनम्_VALID_LIST_MEM( px_अग्र_List_Item ) )
        {
            /* Nothing to do with the corrupted list item. We will skip to the next task state list.
             * pxNextListItem should be NULL at the beginning of each task list.
             */
            px_अग्र_List_Item = रिक्त;
            अनुवर्त;
        }

        TCB_t * pxTCB = ( TCB_t * ) listGET_LIST_ITEM_OWNER( px_अग्र_List_Item );

        यदि( !पत्तनम्_VALID_TCB_MEM( pxTCB ) )
        {
            pxTCB = रिक्त;
        }

        xIterator->pxTaskHandle = pxTCB;
        xIterator->ux_Current_List_सूचकांक = uxCurListIdx;

        यदि( pxCurrListItem->pxPrevious == px_अग्र_List_Item )
        {
            /* If this is the last item of the current state list */
            xIterator->ux_Current_List_सूचकांक++;
            xIterator->px_अग्र_List_Item = रिक्त;
        }
        अन्यथा
        {
            xIterator->px_अग्र_List_Item = list_GET_अग्र( px_अग्र_List_Item );
        }

        प्रत्यागम uxCurListIdx;
    }

    प्रत्यागम -1; /* end of the task list */
}
/*----------------------------------------------------------*/

आधारः_Type_t vTaskGetSnapshot( TaskHandle_t pxTask,
                             TaskSnapshot_t * pxTaskSnapshot )
{
    ESP_STATIC_ANALYZER_CHECK(!pxTask, pdFALSE);

    यदि( ( पत्तनम्_VALID_TCB_MEM( pxTask ) == false ) || ( pxTaskSnapshot == रिक्त ) )
    {
        प्रत्यागम pdFALSE;
    }

    TCB_t * pxTCB = ( TCB_t * ) pxTask;
    pxTaskSnapshot->pxTCB = pxTCB;
    pxTaskSnapshot->px_शिखरम्_सम्बन्धि_राशिः = ( राशिः_Type_t * ) pxTCB->px_शिखरम्_सम्बन्धि_राशिः;
    pxTaskSnapshot->px_अन्तः_सम्बन्धि_राशिः = ( राशिः_Type_t * ) pxTCB->px_अन्तः_सम्बन्धि_राशिः;
    प्रत्यागम pdTRUE;
}
/*----------------------------------------------------------*/

UBaseType_t uxTaskGetSnapshotAll( TaskSnapshot_t * ध्रुव pxTaskSnapshotArray,
                                  ध्रुव UBaseType_t ux_Array_दीर्घता,
                                  UBaseType_t * ध्रुव pxTCBSize )
{
    UBaseType_t ux_Array_अङ्कः_Filled = 0;

    /* Traverse all of the tasks lists */
    TaskIterator_t xTaskIter = { 0 }; /* Point to the first task list */

    यावत्( x_Task_Get_अग्र( &xTaskIter ) != -1 && ux_Array_अङ्कः_Filled < ux_Array_दीर्घता )
    {
        vTaskGetSnapshot( xTaskIter.pxTaskHandle, &pxTaskSnapshotArray[ ux_Array_अङ्कः_Filled ] );
        ux_Array_अङ्कः_Filled++;
    }

    यदि( pxTCBSize != रिक्त )
    {
        *pxTCBSize = परिमाण( TCB_t );
    }

    प्रत्यागम ux_Array_अङ्कः_Filled;
}
/*----------------------------------------------------------*/

/* ----------------------------------------------------- Misc ----------------------------------------------------- */

शून्य * pvTaskGetCurrentTCBForCore( आधारः_Type_t xCoreID )
{
    शून्य * pvRet;

    configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

    #यदि CONFIG_FREERTOS_SMP
        /* SMP FreeRTOS defines pxCurrentTCB as a macro function call */
        pvRet = ( शून्य * ) pxCurrentTCB;
    #अन्यथा /* CONFIG_FREERTOS_SMP */
        pvRet = ( शून्य * ) pxCurrentTCBs[ xCoreID ];
    #यदि_अन्तः /* CONFIG_FREERTOS_SMP */
    प्रत्यागम pvRet;
}

/* ----------------------------------------------------- OpenOCD ---------------------------------------------------- */

#यदि CONFIG_FREERTOS_DEBUG_OCDAWARE

/**
 * Debug param indexes. DO NOT change the order. OpenOCD uses the same indexes
 * Entries in FreeRTOS_openocd_params must match the order of these indexes
 */
    गणना
    {
        ESP_FREERTOS_DEBUG_सारणी_आकार = 0,
        ESP_FREERTOS_DEBUG_सारणी_संस्करणम्,
        ESP_FREERTOS_DEBUG_KERNEL_VER_MAJOR,
        ESP_FREERTOS_DEBUG_KERNEL_VER_MINOR,
        ESP_FREERTOS_DEBUG_KERNEL_VER_BUILD,
        ESP_FREERTOS_DEBUG_UX_शिखरम्_प्रयुक्तम्_PIORITY,
        ESP_FREERTOS_DEBUG_PX_शिखरम्_सम्बन्धि_राशिः,
        ESP_FREERTOS_DEBUG_PC_TASK_NAME,
        ESP_FREERTOS_DEBUG_LIST_आकार,
        ESP_FREERTOS_DEBUG_LIST_अङ्कः_वस्तूनि,
        ESP_FREERTOS_DEBUG_LIST_अन्तः,
        ESP_FREERTOS_DEBUG_LIST_अन्तः_पूर्व,
        ESP_FREERTOS_DEBUG_LIST_ITEM_पूर्व,
        ESP_FREERTOS_DEBUG_LIST_ITEM_OWNER,
        ESP_FREERTOS_DEBUG_TASK_गणनम्_विस्तारः,
        ESP_FREERTOS_DEBUG_PTR_विस्तारः,
        /* New entries must be inserted here */
        ESP_FREERTOS_DEBUG_सारणी_अन्तः,
    };

    ध्रुव DRAM_ATTR uint8_t FreeRTOS_openocd_params[ ESP_FREERTOS_DEBUG_सारणी_अन्तः ] =
    {
        ESP_FREERTOS_DEBUG_सारणी_अन्तः, /* table size */
        1,                            /* table version */
        tsk_KERNEL_संस्करणम्_MAJOR,
        tsk_KERNEL_संस्करणम्_MINOR,
        tsk_KERNEL_संस्करणम्_BUILD,
        configMAX_PRIORITIES - 1,                   /* uxTopUsedPriority */
        offsetof( TCB_t, px_शिखरम्_सम्बन्धि_राशिः ),            /* thread_stack_offset; */
        offsetof( TCB_t, pcTaskName ),              /* thread_name_offset; */
        परिमाण( List_t ),                           /* list_width */
        offsetof( List_t, ux_सङ्ख्या_सम्बन्धि_वस्तूनि ),        /* list_item_num */
        offsetof( List_t, x_List_अन्तः ),               /* list_end_offset */
        offsetof( List_t, x_List_अन्तः.pxPrevious ),    /* list_next_offset */
        offsetof( ListItem_t, pxPrevious ),         /* list_elem_next_offset */
        offsetof( ListItem_t, pvOwner ),            /* list_elem_content_offset */
        परिमाण( UBaseType_t ),                      /* task_count_width */
        परिमाण( शून्य * )                            /* ptr_width */
    };

#यदि_अन्तः /* CONFIG_FREERTOS_DEBUG_OCDAWARE */
/*----------------------------------------------------------*/

/* ----------------------------------------------------- PSRAM ---------------------------------------------------- */

#यदि CONFIG_SPIRAM

    #यदि CONFIG_FREERTOS_SMP
        आधारः_Type_t prv_Task_Create_Dynamic_Affinity_समुच्चयः_With_Caps( TaskFunction_t px_Task_संहिता,
                                                            ध्रुव वर्ण * ध्रुव pcName,
                                                            ध्रुव config_राशिः_गभीरता_TYPE us_राशिः_गभीरता,
                                                            शून्य * ध्रुव pvParameters,
                                                            UBaseType_t ux_प्राथमिकता,
                                                            UBaseType_t ux_Core_Affinity_आवरणम्,
                                                            UBaseType_t ux_राशिः_स्मृतिः_Caps,
                                                            TaskHandle_t * ध्रुव pxCreatedTask )
    #अन्यथा /* CONFIG_FREERTOS_SMP */
        आधारः_Type_t prv_Task_Create_Dynamic_Pinned_गन्तव्यम्_Core_With_Caps( TaskFunction_t px_Task_संहिता,
                                                             ध्रुव वर्ण * ध्रुव pcName,
                                                             ध्रुव config_राशिः_गभीरता_TYPE us_राशिः_गभीरता,
                                                             शून्य * ध्रुव pvParameters,
                                                             UBaseType_t ux_प्राथमिकता,
                                                             ध्रुव आधारः_Type_t xCoreID,
                                                             UBaseType_t ux_राशिः_स्मृतिः_Caps,
                                                             TaskHandle_t * ध्रुव pxCreatedTask )
    #यदि_अन्तः /* CONFIG_FREERTOS_SMP */
    {
        TCB_t * pxNewTCB;
        आधारः_Type_t xReturn;

        राशिः_Type_t * px_राशिः;

        configASSERT( ux_राशिः_स्मृतिः_Caps & ( MALLOC_CAP_8BIT ) );
        configASSERT( ( ux_राशिः_स्मृतिः_Caps & MALLOC_CAP_SPIRAM ) ||
                      ( ux_राशिः_स्मृतिः_Caps & MALLOC_CAP_आन्तरिकम् ) );
        #यदि ( !CONFIG_FREERTOS_SMP )
        {
            configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );
        }
        #यदि_अन्तः /* !CONFIG_FREERTOS_SMP */

        /* Allocate space for the stack used by the task being created. */
        px_राशिः = heap_caps_malloc( ( ( ( size_t ) us_राशिः_गभीरता ) * परिमाण( राशिः_Type_t ) ), ux_राशिः_स्मृतिः_Caps );

        यदि( px_राशिः != रिक्त )
        {
            /* Allocate space for the TCB. */
            pxNewTCB = ( TCB_t * ) pv_पत्तनम्_Malloc( परिमाण( TCB_t ) );

            यदि( pxNewTCB != रिक्त )
            {
                स्मृतिं_पूरय( ( शून्य * ) pxNewTCB, 0x00, परिमाण( TCB_t ) );

                /* Store the stack location in the TCB. */
                pxNewTCB->px_राशिः = px_राशिः;
            }
            अन्यथा
            {
                /* The stack cannot be used as the TCB has not been created. Free it. */
                heap_caps_free( px_राशिः );
            }
        }
        अन्यथा
        {
            pxNewTCB = रिक्त;
        }

        यदि( pxNewTCB != रिक्त )
        {
            #यदि ( tsk_STATIC_AND_DYNAMIC_आवण्टनम्_POSSIBLE != 0 )
            {
                /* Tasks can be created statically or dynamically, so note this
                 * task was created dynamically in case it is later deleted. */
                pxNewTCB->ucStaticallyAllocated = tsk_DYNAMICALLY_ALLOCATED_राशिः_AND_TCB;
            }
            #यदि_अन्तः /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

            #यदि CONFIG_FREERTOS_SMP
            {
                prvInitialiseNewTask( px_Task_संहिता, pcName, ( uint32_t ) us_राशिः_गभीरता, pvParameters, ux_प्राथमिकता, pxCreatedTask, pxNewTCB, रिक्त );
                #यदि ( ( config_अङ्कः_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
                {
                    /* Set the task's affinity before scheduling it */
                    pxNewTCB->ux_Core_Affinity_आवरणम् = ux_Core_Affinity_आवरणम्;
                }
                #यदि_अन्तः /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            }
            #अन्यथा /* CONFIG_FREERTOS_SMP */
            {
                prvInitialiseNewTask( px_Task_संहिता, pcName, ( uint32_t ) us_राशिः_गभीरता, pvParameters, ux_प्राथमिकता, pxCreatedTask, pxNewTCB, रिक्त, xCoreID );
            }
            #यदि_अन्तः /* CONFIG_FREERTOS_SMP */

            prv_सङ्कलय_New_Task_गन्तव्यम्_Ready_List( pxNewTCB );
            xReturn = pdPASS;
        }
        अन्यथा
        {
            xReturn = err_COULD_NOT_ALLOCATE_REQUIRED_स्मृतिः;
        }

        प्रत्यागम xReturn;
    }

#यदि_अन्तः /* CONFIG_SPIRAM */
/*----------------------------------------------------------*/
