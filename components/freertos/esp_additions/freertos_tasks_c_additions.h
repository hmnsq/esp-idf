/*
 * SPDX-FileCopyrightText: 2015-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ "esp_assert.h"
#ಅನ್ತರ್ಭಾವ "esp_heap_caps.h"
#ಅನ್ತರ್ಭಾವ "esp_compiler.h"
#ಅನ್ತರ್ಭಾವ "freertos/idf_additions.h"
#ಅನ್ತರ್ಭಾವ "freertos/freertos_debug.h"
#ಅನ್ತರ್ಭಾವ "esp_private/freertos_idf_additions_priv.h"

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
_Static_assert( offsetof( StaticTask_t, pxDummy6 ) == offsetof( TCB_t, px_ರಾಶಿ ) );
_Static_assert( offsetof( StaticTask_t, pxDummy8 ) == offsetof( TCB_t, px_ಕೊನೆ_ದ_ರಾಶಿ ) );
#ಆದರೆ !CONFIG_IDF_ಗುರಿ_LINUX    // Disabled for linux builds due to differences in types
_Static_assert( tskNO_AFFINITY == ( ತಳ_Type_t ) CONFIG_FREERTOS_NO_AFFINITY, "CONFIG_FREERTOS_NO_AFFINITY must be the same as tskNO_AFFINITY" );
#ಆದರೆ_ಅಂತ್ಯ

/* ------------------------------------------------- Kernel Control ------------------------------------------------- */

#ಆದರೆ ( !CONFIG_FREERTOS_SMP && ( config_ಅಂಕೆ_CORES > 1 ) )

/*
 * Wrapper function to take "xKerneLock"
 */
    ಶೂನ್ಯ prv_Take_Kernel_ಬೀಗ( ಶೂನ್ಯ )
    {
        /* We call the tasks.c critical section macro to take xKernelLock */
        taskENTER_CRITICAL( &x_Kernel_ಬೀಗ );
    }

#ಆದರೆ_ಅಂತ್ಯ /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#ಆದರೆ ( !CONFIG_FREERTOS_SMP && ( config_ಅಂಕೆ_CORES > 1 ) )

/*
 * Wrapper function to release "xKerneLock"
 */
    ಶೂನ್ಯ prv_Release_Kernel_ಬೀಗ( ಶೂನ್ಯ )
    {
        /* We call the tasks.c critical section macro to release xKernelLock */
        taskEXIT_CRITICAL( &x_Kernel_ಬೀಗ );
    }

#ಆದರೆ_ಅಂತ್ಯ /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#ಆದರೆ ( CONFIG_FREERTOS_SMP && ( config_ಅಂಕೆ_CORES > 1 ) )

/*
 * Workaround for non-thread safe multi-core OS startup (see IDF-4524)
 */
    ಶೂನ್ಯ prv_ಆರಂಭ_Scheduler_Other_Cores( ಶೂನ್ಯ )
    {
        /* This function is always called with interrupts disabled*/
        xSchedulerRunning = pdTRUE;
    }

#ಆದರೆ_ಅಂತ್ಯ /* ( CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

#ಆದರೆ ( !CONFIG_FREERTOS_SMP && ( config_ಅಂಕೆ_CORES > 1 ) )

    ತಳ_Type_t xTaskIncrementTickOtherCores( ಶೂನ್ಯ )
    {
        /* Minor optimization. This function can never switch cores mid
         * execution */
        ತಳ_Type_t xCoreID = ಪೋರ್ಟ್_GET_CORE_ID();
        ತಳ_Type_t xSwitchRequired = pdFALSE;

        /* This function should never be called by Core 0. */
        configASSERT( xCoreID != 0 );

        /* Called by the portable layer each time a tick interrupt occurs
         * on a core other than core 0. */
        traceTASK_INCREMENT_TICK( x_Tick_ಎಣಿಕೆ );

        ಆದರೆ( uxSchedulerSuspended[ xCoreID ] == ( UBaseType_t ) 0U )
        {
            /* We need take the kernel lock here as we are about to access
             * kernel data structures. */
            taskENTER_CRITICAL_ISR( &x_Kernel_ಬೀಗ );

            /* Tasks of equal priority to the currently running task will share
             * processing time (time slice) if preemption is on, and the application
             * writer has not explicitly turned time slicing off. */
            #ಆದರೆ ( ( configUSE_PREEMPTION == 1 ) && ( config_USE_ಸಮಯ_SLICING == 1 ) )
            {
                ಆದರೆ( list_CURRENT_LIST_ಉದ್ದಳತೆ( &( pxReadyTasksLists[ pxCurrentTCBs[ xCoreID ]->ux_ಆದ್ಯತೆ ] ) ) > ( UBaseType_t ) 1 )
                {
                    xSwitchRequired = pdTRUE;
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            #ಆದರೆ_ಅಂತ್ಯ /* ( ( configUSE_PREEMPTION == 1 ) && ( configUSE_TIME_SLICING == 1 ) ) */

            /* Release the previously taken kernel lock as we have finished
             * accessing the kernel data structures. */
            taskEXIT_CRITICAL_ISR( &x_Kernel_ಬೀಗ );

            #ಆದರೆ ( configUSE_PREEMPTION == 1 )
            {
                ಆದರೆ( xYieldPending[ xCoreID ] != pdFALSE )
                {
                    xSwitchRequired = pdTRUE;
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    mtCOVERAGE_TEST_MARKER();
                }
            }
            #ಆದರೆ_ಅಂತ್ಯ /* configUSE_PREEMPTION */
        }

        #ಆದರೆ ( configUSE_TICK_HOOK == 1 )
        {
            vApplicationTickHook();
        }
        #ಆದರೆ_ಅಂತ್ಯ

        ಹಿಂತಿರುಗಿ xSwitchRequired;
    }

#ಆದರೆ_ಅಂತ್ಯ /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */
/*----------------------------------------------------------*/

/* -------------------------------------------------- Task Creation ------------------------------------------------- */

#ಆದರೆ ( config_SUPPORT_DYNAMIC_ಹಂಚಿಕೆ == 1 )

    ತಳ_Type_t x_Task_Create_Pinned_ಗಮ್ಯ_Core( TaskFunction_t px_Task_ಸಂಕೇತ,
                                        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ pcName,
                                        ಸ್ಥಿರಾಂಕ uint32_t us_ರಾಶಿ_ಆಳ,
                                        ಶೂನ್ಯ * ಸ್ಥಿರಾಂಕ pvParameters,
                                        UBaseType_t ux_ಆದ್ಯತೆ,
                                        TaskHandle_t * ಸ್ಥಿರಾಂಕ pxCreatedTask,
                                        ಸ್ಥಿರಾಂಕ ತಳ_Type_t xCoreID )
    {
        ತಳ_Type_t xReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );

        #ಆದರೆ CONFIG_FREERTOS_SMP
        {
            /* If using Amazon SMP FreeRTOS. This function is just a wrapper around
             * xTaskCreate() or xTaskCreateAffinitySet(). */
            #ಆದರೆ ( ( config_ಅಂಕೆ_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
            {
                /* Convert xCoreID into an affinity mask */
                UBaseType_t ux_Core_Affinity_ಮುಖವಾಡ;

                /* Bit shifting << xCoreID is only valid if we have less than
                 * 32 cores. */
                ESP_STATIC_ASSERT( config_ಅಂಕೆ_CORES < 32 );

                ಆದರೆ( xCoreID == tskNO_AFFINITY )
                {
                    ux_Core_Affinity_ಮುಖವಾಡ = tskNO_AFFINITY;
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    ux_Core_Affinity_ಮುಖವಾಡ = ( 1 << xCoreID );
                }

                xReturn = x_Task_Create_Affinity_ಗಣ( px_Task_ಸಂಕೇತ, pcName, us_ರಾಶಿ_ಆಳ, pvParameters, ux_ಆದ್ಯತೆ, ux_Core_Affinity_ಮುಖವಾಡ, pxCreatedTask );
            }
            #ಇಲ್ಲದಿದ್ದರೆ /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            {
                xReturn = xTaskCreate( px_Task_ಸಂಕೇತ, pcName, us_ರಾಶಿ_ಆಳ, pvParameters, ux_ಆದ್ಯತೆ, pxCreatedTask );
            }
            #ಆದರೆ_ಅಂತ್ಯ /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
        }
        #ಇಲ್ಲದಿದ್ದರೆ /* CONFIG_FREERTOS_SMP */
        {
            TCB_t * pxNewTCB;

            /* If the stack grows down then allocate the stack then the TCB so the
             * stack does not grow into the TCB.  Likewise if the stack grows up
             * then allocate the TCB then the stack. */
            #ಆದರೆ ( ಪೋರ್ಟ್_ರಾಶಿ_GROWTH > 0 )
            {
                /* Allocate space for the TCB.  Where the memory comes from depends on
                 * the implementation of the port malloc function and whether or not static
                 * allocation is being used. */
                pxNewTCB = ( TCB_t * ) pv_ಪೋರ್ಟ್_Malloc( ಗಾತ್ರ( TCB_t ) );

                ಆದರೆ( pxNewTCB != ಖಾಲಿ )
                {
                    memset( ( ಶೂನ್ಯ * ) pxNewTCB, 0x00, ಗಾತ್ರ( TCB_t ) );

                    /* Allocate space for the stack used by the task being created.
                     * The base of the stack memory stored in the TCB so the task can
                     * be deleted later if required. */
                    pxNewTCB->px_ರಾಶಿ = ( ರಾಶಿ_Type_t * ) pv_ಪೋರ್ಟ್_Malloc_ರಾಶಿ( ( ( ( size_t ) us_ರಾಶಿ_ಆಳ ) * ಗಾತ್ರ( ರಾಶಿ_Type_t ) ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */

                    ಆದರೆ( pxNewTCB->px_ರಾಶಿ == ಖಾಲಿ )
                    {
                        /* Could not allocate the stack.  Delete the allocated TCB. */
                        v_ಪೋರ್ಟ್_Free( pxNewTCB );
                        pxNewTCB = ಖಾಲಿ;
                    }
                }
            }
            #ಇಲ್ಲದಿದ್ದರೆ /* portSTACK_GROWTH */
            {
                ರಾಶಿ_Type_t * px_ರಾಶಿ;

                /* Allocate space for the stack used by the task being created. */
                px_ರಾಶಿ = pv_ಪೋರ್ಟ್_Malloc_ರಾಶಿ( ( ( ( size_t ) us_ರಾಶಿ_ಆಳ ) * ಗಾತ್ರ( ರಾಶಿ_Type_t ) ) ); /*lint !e9079 All values returned by pvPortMalloc() have at least the alignment required by the MCU's stack and this allocation is the stack. */

                ಆದರೆ( px_ರಾಶಿ != ಖಾಲಿ )
                {
                    /* Allocate space for the TCB. */
                    pxNewTCB = ( TCB_t * ) pv_ಪೋರ್ಟ್_Malloc( ಗಾತ್ರ( TCB_t ) ); /*lint !e9087 !e9079 All values returned by pvPortMalloc() have at least the alignment required by the MCU's stack, and the first member of TCB_t is always a pointer to the task's stack. */

                    ಆದರೆ( pxNewTCB != ಖಾಲಿ )
                    {
                        memset( ( ಶೂನ್ಯ * ) pxNewTCB, 0x00, ಗಾತ್ರ( TCB_t ) );

                        /* Store the stack location in the TCB. */
                        pxNewTCB->px_ರಾಶಿ = px_ರಾಶಿ;
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                    {
                        /* The stack cannot be used as the TCB was not created.  Free
                         * it again. */
                        v_ಪೋರ್ಟ್_Free_ರಾಶಿ( px_ರಾಶಿ );
                    }
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    pxNewTCB = ಖಾಲಿ;
                }
            }
            #ಆದರೆ_ಅಂತ್ಯ /* portSTACK_GROWTH */

            ಆದರೆ( pxNewTCB != ಖಾಲಿ )
            {
                #ಆದರೆ ( tsk_STATIC_AND_DYNAMIC_ಹಂಚಿಕೆ_POSSIBLE != 0 ) /*lint !e9029 !e731 Macro has been consolidated for readability reasons. */
                {
                    /* Tasks can be created statically or dynamically, so note this
                     * task was created dynamically in case it is later deleted. */
                    pxNewTCB->ucStaticallyAllocated = tsk_DYNAMICALLY_ALLOCATED_ರಾಶಿ_AND_TCB;
                }
                #ಆದರೆ_ಅಂತ್ಯ /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

                prvInitialiseNewTask( px_Task_ಸಂಕೇತ, pcName, ( uint32_t ) us_ರಾಶಿ_ಆಳ, pvParameters, ux_ಆದ್ಯತೆ, pxCreatedTask, pxNewTCB, ಖಾಲಿ, xCoreID );
                prv_ಕೂಡಿಸು_New_Task_ಗಮ್ಯ_Ready_List( pxNewTCB );
                xReturn = pdPASS;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                xReturn = err_COULD_NOT_ALLOCATE_REQUIRED_ಸ್ಮರಣೆ;
            }
        }
        #ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_SMP */

        ಹಿಂತಿರುಗಿ xReturn;
    }

#ಆದರೆ_ಅಂತ್ಯ /* ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#ಆದರೆ ( config_SUPPORT_STATIC_ಹಂಚಿಕೆ == 1 )

    TaskHandle_t x_Task_Create_Static_Pinned_ಗಮ್ಯ_Core( TaskFunction_t px_Task_ಸಂಕೇತ,
                                                ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ pcName,
                                                ಸ್ಥಿರಾಂಕ uint32_t ul_ರಾಶಿ_ಆಳ,
                                                ಶೂನ್ಯ * ಸ್ಥಿರಾಂಕ pvParameters,
                                                UBaseType_t ux_ಆದ್ಯತೆ,
                                                ರಾಶಿ_Type_t * ಸ್ಥಿರಾಂಕ pux_ರಾಶಿ_ಬಫರ್,
                                                StaticTask_t * ಸ್ಥಿರಾಂಕ px_Task_ಬಫರ್,
                                                ಸ್ಥಿರಾಂಕ ತಳ_Type_t xCoreID )
    {
        TaskHandle_t xReturn;

        configASSERT( ಪೋರ್ಟ್_VALID_ರಾಶಿ_MEM( pux_ರಾಶಿ_ಬಫರ್ ) );
        configASSERT( ಪೋರ್ಟ್_VALID_TCB_MEM( px_Task_ಬಫರ್ ) );
        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );

        #ಆದರೆ CONFIG_FREERTOS_SMP
        {
            /* If using Amazon SMP FreeRTOS. This function is just a wrapper around
             * xTaskCreateStatic() or xTaskCreateStaticAffinitySet(). */
            #ಆದರೆ ( ( config_ಅಂಕೆ_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
            {
                /* Convert xCoreID into an affinity mask */
                UBaseType_t ux_Core_Affinity_ಮುಖವಾಡ;

                /* Bit shifting << xCoreID is only valid if we have less than
                 * 32 cores. */
                ESP_STATIC_ASSERT( config_ಅಂಕೆ_CORES < 32 );

                ಆದರೆ( xCoreID == tskNO_AFFINITY )
                {
                    ux_Core_Affinity_ಮುಖವಾಡ = tskNO_AFFINITY;
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    ux_Core_Affinity_ಮುಖವಾಡ = ( 1 << xCoreID );
                }

                xReturn = x_Task_Create_Static_Affinity_ಗಣ( px_Task_ಸಂಕೇತ, pcName, ul_ರಾಶಿ_ಆಳ, pvParameters, ux_ಆದ್ಯತೆ, pux_ರಾಶಿ_ಬಫರ್, px_Task_ಬಫರ್, ux_Core_Affinity_ಮುಖವಾಡ );
            }
            #ಇಲ್ಲದಿದ್ದರೆ /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            {
                xReturn = xTaskCreateStatic( px_Task_ಸಂಕೇತ, pcName, ul_ರಾಶಿ_ಆಳ, pvParameters, ux_ಆದ್ಯತೆ, pux_ರಾಶಿ_ಬಫರ್, px_Task_ಬಫರ್ );
            }
            #ಆದರೆ_ಅಂತ್ಯ /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
        }
        #ಇಲ್ಲದಿದ್ದರೆ /* CONFIG_FREERTOS_SMP */
        {
            TCB_t * pxNewTCB;

            #ಆದರೆ ( configASSERT_DEFINED == 1 )
            {
                /* Sanity check that the size of the structure used to declare a
                 * variable of type StaticTask_t equals the size of the real task
                 * structure. */
                ಅಸ್ಥಿರ size_t x_ಗಾತ್ರ = ಗಾತ್ರ( StaticTask_t );
                configASSERT( x_ಗಾತ್ರ == ಗಾತ್ರ( TCB_t ) );
                ( ಶೂನ್ಯ ) x_ಗಾತ್ರ; /* Prevent lint warning when configASSERT() is not used. */
            }
            #ಆದರೆ_ಅಂತ್ಯ /* configASSERT_DEFINED */

            ಆದರೆ( ( px_Task_ಬಫರ್ != ಖಾಲಿ ) && ( pux_ರಾಶಿ_ಬಫರ್ != ಖಾಲಿ ) )
            {
                /* The memory used for the task's TCB and stack are passed into this
                 * function - use them. */
                pxNewTCB = ( TCB_t * ) px_Task_ಬಫರ್; /*lint !e740 !e9087 Unusual cast is ok as the structures are designed to have the same alignment, and the size is checked by an assert. */
                memset( ( ಶೂನ್ಯ * ) pxNewTCB, 0x00, ಗಾತ್ರ( TCB_t ) );
                pxNewTCB->px_ರಾಶಿ = ( ರಾಶಿ_Type_t * ) pux_ರಾಶಿ_ಬಫರ್;

                #ಆದರೆ ( tsk_STATIC_AND_DYNAMIC_ಹಂಚಿಕೆ_POSSIBLE != 0 ) /*lint !e731 !e9029 Macro has been consolidated for readability reasons. */
                {
                    /* Tasks can be created statically or dynamically, so note this
                     * task was created statically in case the task is later deleted. */
                    pxNewTCB->ucStaticallyAllocated = tsk_STATICALLY_ALLOCATED_ರಾಶಿ_AND_TCB;
                }
                #ಆದರೆ_ಅಂತ್ಯ /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

                prvInitialiseNewTask( px_Task_ಸಂಕೇತ, pcName, ul_ರಾಶಿ_ಆಳ, pvParameters, ux_ಆದ್ಯತೆ, &xReturn, pxNewTCB, ಖಾಲಿ, xCoreID );
                prv_ಕೂಡಿಸು_New_Task_ಗಮ್ಯ_Ready_List( pxNewTCB );
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                xReturn = ಖಾಲಿ;
            }
        }
        #ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_SMP */

        ಹಿಂತಿರುಗಿ xReturn;
    }

#ಆದರೆ_ಅಂತ್ಯ /* ( configSUPPORT_STATIC_ALLOCATION == 1 ) */
/*----------------------------------------------------------*/

#ಆದರೆ ( configUSE_TIMERS == 1 )

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
    ತಳ_Type_t __attribute__( ( weak ) ) xTimerCreateTimerTask( ಶೂನ್ಯ )
    {
        ಹಿಂತಿರುಗಿ pdPASS;
    }

#ಆದರೆ_ಅಂತ್ಯ /* configUSE_TIMERS */
/*----------------------------------------------------------*/

/* ------------------------------------------------- Task Utilities ------------------------------------------------- */

ತಳ_Type_t xTaskGetCoreID( TaskHandle_t xTask )
{
    ತಳ_Type_t xReturn;

    #ಆದರೆ ( config_ಅಂಕೆ_CORES > 1 )
    {
        #ಆದರೆ CONFIG_FREERTOS_SMP
            UBaseType_t ux_Core_Affinity_ಮುಖವಾಡ;

            /* Get the core affinity mask and convert it to an ID */
            ux_Core_Affinity_ಮುಖವಾಡ = vTaskCoreAffinityGet( xTask );

            /* If the task is not pinned to a particular core, treat it as tskNO_AFFINITY */
            ಆದರೆ( ux_Core_Affinity_ಮುಖವಾಡ & ( ux_Core_Affinity_ಮುಖವಾಡ - 1 ) ) /* If more than one bit set */
            {
                xReturn = tskNO_AFFINITY;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ಪೂರ್ಣಾಂಕ i_ಸೂಚ್ಯಂಕ_Plus_One = __builtin_ffs( ux_Core_Affinity_ಮುಖವಾಡ );
                ಪ್ರತಿಜ್ಞಾ( i_ಸೂಚ್ಯಂಕ_Plus_One >= 1 );
                xReturn = i_ಸೂಚ್ಯಂಕ_Plus_One - 1;
            }
        #ಇಲ್ಲದಿದ್ದರೆ /* CONFIG_FREERTOS_SMP */
            TCB_t * pxTCB;

            pxTCB = prvGetTCBFromHandle( xTask );

            xReturn = pxTCB->xCoreID;
        #ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_SMP */
    }
    #ಇಲ್ಲದಿದ್ದರೆ /* configNUM_CORES > 1 */
    {
        /* Single-core. Just return a core ID of 0 */
        xReturn = 0;
    }
    #ಆದರೆ_ಅಂತ್ಯ /* configNUM_CORES > 1 */

    ಹಿಂತಿರುಗಿ xReturn;
}
/*----------------------------------------------------------*/

#ಆದರೆ ( ( !CONFIG_FREERTOS_SMP ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

    TaskHandle_t xTaskGetIdleTaskHandleForCore( ತಳ_Type_t xCoreID )
    {
        /* If xTaskGetIdleTaskHandle() is called before the scheduler has been
         * started, then xIdleTaskHandle will be NULL. */
        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );
        configASSERT( ( xIdleTaskHandle[ xCoreID ] != ಖಾಲಿ ) );
        ಹಿಂತಿರುಗಿ xIdleTaskHandle[ xCoreID ];
    }

#ಆದರೆ_ಅಂತ್ಯ /* ( ( !CONFIG_FREERTOS_SMP ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*----------------------------------------------------------*/

#ಆದರೆ ( ( !CONFIG_FREERTOS_SMP ) && ( ( INCLUDE_xTaskGetCurrentTaskHandle == 1 ) || ( configUSE_MUTEXES == 1 ) ) )

    TaskHandle_t xTaskGetCurrentTaskHandleForCore( ತಳ_Type_t xCoreID )
    {
        TaskHandle_t xReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

        #ಆದರೆ ( CONFIG_FREERTOS_SMP )
        {
            xReturn = xTaskGetCurrentTaskHandleCPU( ( UBaseType_t ) xCoreID );
        }
        #ಇಲ್ಲದಿದ್ದರೆ /* CONFIG_FREERTOS_SMP */
        {
            /* A critical section is not required as this function does not
             * guarantee that the TCB will still be valid when this function
             * returns. */
            xReturn = pxCurrentTCBs[ xCoreID ];
        }
        #ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_SMP */

        ಹಿಂತಿರುಗಿ xReturn;
    }

#ಆದರೆ_ಅಂತ್ಯ /* ( ( !CONFIG_FREERTOS_SMP ) && ( ( INCLUDE_xTaskGetCurrentTaskHandle == 1 ) || ( configUSE_MUTEXES == 1 ) ) ) */
/*----------------------------------------------------------*/

#ಆದರೆ ( !CONFIG_FREERTOS_SMP && ( config_GENERATE_ಸತತ_ಸಮಯ_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

    config_ಸತತ_ಸಮಯ_COUNTER_TYPE ul_Task_Get_Idle_ಸತತ_ಸಮಯ_Counter_For_Core( ತಳ_Type_t xCoreID )
    {
        config_ಸತತ_ಸಮಯ_COUNTER_TYPE ul_ಸತತ_ಸಮಯ_Counter;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

        /* For SMP, we need to take the kernel lock here as we are about to
         * access kernel data structures. */
        prvENTER_CRITICAL_SMP_ONLY( &x_Kernel_ಬೀಗ );
        {
            ul_ಸತತ_ಸಮಯ_Counter = xIdleTaskHandle[ xCoreID ]->ul_ಸತತ_ಸಮಯ_Counter;
        }
        /* Release the previously taken kernel lock. */
        prvEXIT_CRITICAL_SMP_ONLY( &x_Kernel_ಬೀಗ );

        ಹಿಂತಿರುಗಿ ul_ಸತತ_ಸಮಯ_Counter;
    }

#ಆದರೆ_ಅಂತ್ಯ /* ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*----------------------------------------------------------*/

#ಆದರೆ ( !CONFIG_FREERTOS_SMP && ( config_GENERATE_ಸತತ_ಸಮಯ_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

    config_ಸತತ_ಸಮಯ_COUNTER_TYPE ul_Task_Get_Idle_ಸತತ_ಸಮಯ_Percent_For_Core( ತಳ_Type_t xCoreID )
    {
        config_ಸತತ_ಸಮಯ_COUNTER_TYPE ul_ಒಟ್ಟು_ಸಮಯ, ulReturn;

        configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

        #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪೋರ್ಟ್_ALT_GET_ಸತತ_ಸಮಯ_COUNTER_ಮೌಲ್ಯ
            ಪೋರ್ಟ್_ALT_GET_ಸತತ_ಸಮಯ_COUNTER_ಮೌಲ್ಯ( ul_ಒಟ್ಟು_ಸಮಯ );
        #ಇಲ್ಲದಿದ್ದರೆ
            ul_ಒಟ್ಟು_ಸಮಯ = ಪೋರ್ಟ್_GET_ಸತತ_ಸಮಯ_COUNTER_ಮೌಲ್ಯ();
        #ಆದರೆ_ಅಂತ್ಯ

        /* For percentage calculations. */
        ul_ಒಟ್ಟು_ಸಮಯ /= ( config_ಸತತ_ಸಮಯ_COUNTER_TYPE ) 100;

        /* Avoid divide by zero errors. */
        ಆದರೆ( ul_ಒಟ್ಟು_ಸಮಯ > ( config_ಸತತ_ಸಮಯ_COUNTER_TYPE ) 0 )
        {
            /* For SMP, we need to take the kernel lock here as we are about
             * to access kernel data structures. */
            prvENTER_CRITICAL_SMP_ONLY( &x_Kernel_ಬೀಗ );
            {
                ulReturn = xIdleTaskHandle[ xCoreID ]->ul_ಸತತ_ಸಮಯ_Counter / ul_ಒಟ್ಟು_ಸಮಯ;
            }
            /* Release the previously taken kernel lock. */
            prvEXIT_CRITICAL_SMP_ONLY( &x_Kernel_ಬೀಗ );
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            ulReturn = 0;
        }

        ಹಿಂತಿರುಗಿ ulReturn;
    }

#ಆದರೆ_ಅಂತ್ಯ /* ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */
/*-----------------------------------------------------------*/

ರಾಶಿ_Type_t * x_Task_Get_ರಾಶಿ_ಆರಂಭ( TaskHandle_t xTask )
{
    TCB_t * pxTCB;

    pxTCB = prvGetTCBFromHandle( xTask );
    ಹಿಂತಿರುಗಿ pxTCB->px_ರಾಶಿ;
}
/*----------------------------------------------------------*/

uint8_t * px_Task_Get_ರಾಶಿ_ಆರಂಭ( TaskHandle_t xTask )
{
    ಹಿಂತಿರುಗಿ (uint8_t *)x_Task_Get_ರಾಶಿ_ಆರಂಭ( xTask );
}
/*----------------------------------------------------------*/

#ಆದರೆ ( INCLUDE_v_Task_ಆದ್ಯತೆ_ಗಣ == 1 )

    ಶೂನ್ಯ prv_Task_ಆದ್ಯತೆ_Raise( prv_Task_Saved_ಆದ್ಯತೆ_t * px_Saved_ಆದ್ಯತೆ,
                               UBaseType_t ux_New_ಆದ್ಯತೆ )
    {
        TCB_t * pxTCB;
        UBaseType_t ux_ಆದ್ಯತೆ_ಬಳಸಿದ_On_ನಮೂದು;

        configASSERT( ( ux_New_ಆದ್ಯತೆ < configMAX_PRIORITIES ) );

        /* Ensure the new priority is valid. */
        ಆದರೆ( ux_New_ಆದ್ಯತೆ >= ( UBaseType_t ) configMAX_PRIORITIES )
        {
            ux_New_ಆದ್ಯತೆ = ( UBaseType_t ) configMAX_PRIORITIES - ( UBaseType_t ) 1U;
        }

        #ಆದರೆ CONFIG_FREERTOS_SMP
            taskENTER_CRITICAL();
        #ಇಲ್ಲದಿದ್ದರೆ
            taskENTER_CRITICAL( &x_Kernel_ಬೀಗ );
        #ಆದರೆ_ಅಂತ್ಯ
        {
            pxTCB = prvGetTCBFromHandle( ಖಾಲಿ );

            #ಆದರೆ ( configUSE_MUTEXES == 1 )
            {
                px_Saved_ಆದ್ಯತೆ->ux_ಆದ್ಯತೆ = pxTCB->ux_ಆದ್ಯತೆ;
                px_Saved_ಆದ್ಯತೆ->ux_ತಳ_ಆದ್ಯತೆ = pxTCB->ux_ತಳ_ಆದ್ಯತೆ;

                /* If uxNewPriority < uxBasePriority, then there is nothing else to
                 * do, as uxBasePriority is always <= uxPriority. */
                ಆದರೆ( ux_New_ಆದ್ಯತೆ > pxTCB->ux_ತಳ_ಆದ್ಯತೆ )
                {
                    pxTCB->ux_ತಳ_ಆದ್ಯತೆ = ux_New_ಆದ್ಯತೆ;

                    /* Remember the task's current priority before attempting to
                     * change it. If the task's current priority is changed, it must
                     * be done so before moving the task between task lists) in order
                     * for the taskRESET_READY_PRIORITY() macro to function correctly. */
                    ux_ಆದ್ಯತೆ_ಬಳಸಿದ_On_ನಮೂದು = pxTCB->ux_ಆದ್ಯತೆ;

                    ಆದರೆ( ux_New_ಆದ್ಯತೆ > pxTCB->ux_ಆದ್ಯತೆ )
                    {
                        pxTCB->ux_ಆದ್ಯತೆ = ux_New_ಆದ್ಯತೆ;

                        /* Only reset the event list item value if the value is not
                         * being used for anything else. */
                        ಆದರೆ( ( list_GET_LIST_ಅಂಶ_ಮೌಲ್ಯ( &( pxTCB->x_Event_List_ಅಂಶ ) ) & task_EVENT_LIST_ಅಂಶ_ಮೌಲ್ಯ_ಒಳ_USE ) == 0UL )
                        {
                            list_ಗಣ_LIST_ಅಂಶ_ಮೌಲ್ಯ( &( pxTCB->x_Event_List_ಅಂಶ ), ( ( TickType_t ) configMAX_PRIORITIES - ( TickType_t ) ux_New_ಆದ್ಯತೆ ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */
                        }

                        /* If the task is in the blocked or suspended list we need do
                         * nothing more than change its priority variable. However, if
                         * the task is in a ready list it needs to be removed and placed
                         * in the list appropriate to its new priority. */
                        ಆದರೆ( listIS_CONTAINED_WITHIN( &( pxReadyTasksLists[ ux_ಆದ್ಯತೆ_ಬಳಸಿದ_On_ನಮೂದು ] ), &( pxTCB->x_State_List_ಅಂಶ ) ) != pdFALSE )
                        {
                            /* The task is currently in its ready list - remove before
                             * adding it to its new ready list.  As we are in a critical
                             * section we can do this even if the scheduler is suspended. */
                            ಆದರೆ( uxListRemove( &( pxTCB->x_State_List_ಅಂಶ ) ) == ( UBaseType_t ) 0 )
                            {
                                /* It is known that the task is in its ready list so
                                 * there is no need to check again and the port level
                                 * reset macro can be called directly. */
                                ಪೋರ್ಟ್_RESET_READY_ಆದ್ಯತೆ( ux_ಆದ್ಯತೆ_ಬಳಸಿದ_On_ನಮೂದು, ux_ತುದಿ_Ready_ಆದ್ಯತೆ );
                            }

                            prv_ಕೂಡಿಸು_Task_ಗಮ್ಯ_Ready_List( pxTCB );
                        }
                    }
                }
            }
            #ಇಲ್ಲದಿದ್ದರೆ /* if ( configUSE_MUTEXES == 1 ) */
            {
                px_Saved_ಆದ್ಯತೆ->ux_ಆದ್ಯತೆ = pxTCB->ux_ಆದ್ಯತೆ;

                ಆದರೆ( ux_New_ಆದ್ಯತೆ > pxTCB->ux_ಆದ್ಯತೆ )
                {
                    v_Task_ಆದ್ಯತೆ_ಗಣ( ಖಾಲಿ, ux_New_ಆದ್ಯತೆ );
                }
            }
            #ಆದರೆ_ಅಂತ್ಯ /* if ( configUSE_MUTEXES == 1 ) */
        }
        #ಆದರೆ CONFIG_FREERTOS_SMP
            taskEXIT_CRITICAL();
        #ಇಲ್ಲದಿದ್ದರೆ
            taskEXIT_CRITICAL( &x_Kernel_ಬೀಗ );
        #ಆದರೆ_ಅಂತ್ಯ
    }

#ಆದರೆ_ಅಂತ್ಯ /* INCLUDE_vTaskPrioritySet == 1 */
/*----------------------------------------------------------*/

#ಆದರೆ ( INCLUDE_v_Task_ಆದ್ಯತೆ_ಗಣ == 1 )

    ಶೂನ್ಯ prv_Task_ಆದ್ಯತೆ_Restore( prv_Task_Saved_ಆದ್ಯತೆ_t * px_Saved_ಆದ್ಯತೆ )
    {
        TCB_t * pxTCB;
        UBaseType_t ux_New_ಆದ್ಯತೆ;
        UBaseType_t ux_ಆದ್ಯತೆ_ಬಳಸಿದ_On_ನಮೂದು;
        UBaseType_t ux_ತಳ_ಆದ್ಯತೆ_ಬಳಸಿದ_On_ನಮೂದು;
        ತಳ_Type_t xYieldRequired = pdFALSE;

        #ಆದರೆ CONFIG_FREERTOS_SMP
            taskENTER_CRITICAL();
        #ಇಲ್ಲದಿದ್ದರೆ
            taskENTER_CRITICAL( &x_Kernel_ಬೀಗ );
        #ಆದರೆ_ಅಂತ್ಯ
        {
            pxTCB = prvGetTCBFromHandle( ಖಾಲಿ );

            #ಆದರೆ ( configUSE_MUTEXES == 1 )
            {
                /* If the saved uxBasePriority == the task's uxBasePriority, it means
                 * that prvTaskPriorityRaise() never raised the task's uxBasePriority.
                 * In that case, there is nothing else to do. */
                ಆದರೆ( px_Saved_ಆದ್ಯತೆ->ux_ತಳ_ಆದ್ಯತೆ != pxTCB->ux_ತಳ_ಆದ್ಯತೆ )
                {
                    ux_ತಳ_ಆದ್ಯತೆ_ಬಳಸಿದ_On_ನಮೂದು = pxTCB->ux_ತಳ_ಆದ್ಯತೆ;
                    pxTCB->ux_ತಳ_ಆದ್ಯತೆ = px_Saved_ಆದ್ಯತೆ->ux_ತಳ_ಆದ್ಯತೆ;

                    /* Remember the task's current priority before attempting to
                     * change it. If the task's current priority is changed, it must
                     * be done so before moving the task between task lists in order
                     * for the taskRESET_READY_PRIORITY() macro to function correctly. */
                    ux_ಆದ್ಯತೆ_ಬಳಸಿದ_On_ನಮೂದು = pxTCB->ux_ಆದ್ಯತೆ;

                    /* Check if the task inherited a priority after prvTaskPriorityRaise().
                     * If this is the case, there is nothing else to do. The priority
                     * will be restored when the task disinherits its priority. */
                    ಆದರೆ( pxTCB->ux_ಆದ್ಯತೆ == ux_ತಳ_ಆದ್ಯತೆ_ಬಳಸಿದ_On_ನಮೂದು )
                    {
                        ಆದರೆ( pxTCB->uxMutexesHeld == 0 )
                        {
                            /* The task may have inherited a priority before prvTaskPriorityRaise()
                             * then disinherited a priority after prvTaskPriorityRaise().
                             * Thus we need set the uxPriority to the saved base priority
                             * so that the task's priority gets restored to the priority
                             * before any inheritance or raising. */
                            pxTCB->ux_ಆದ್ಯತೆ = px_Saved_ಆದ್ಯತೆ->ux_ತಳ_ಆದ್ಯತೆ;
                        }
                        ಇಲ್ಲದಿದ್ದರೆ
                        {
                            /* The task may have inherited a priority before prvTaskPriorityRaise()
                             * was called. Thus, we need to restore uxPriority to the
                             * "saved uxPriority" so that the task still retains that
                             * inherited priority. */
                            pxTCB->ux_ಆದ್ಯತೆ = px_Saved_ಆದ್ಯತೆ->ux_ಆದ್ಯತೆ;
                        }

                        ux_New_ಆದ್ಯತೆ = pxTCB->ux_ಆದ್ಯತೆ;

                        ಆದರೆ( ux_New_ಆದ್ಯತೆ < ux_ಆದ್ಯತೆ_ಬಳಸಿದ_On_ನಮೂದು )
                        {
                            /* Setting the priority of the running task down means
                             * there may now be another task of higher priority that
                             * is ready to execute. */
                            xYieldRequired = pdTRUE;
                        }

                        /* Only reset the event list item value if the value is not
                         * being used for anything else. */
                        ಆದರೆ( ( list_GET_LIST_ಅಂಶ_ಮೌಲ್ಯ( &( pxTCB->x_Event_List_ಅಂಶ ) ) & task_EVENT_LIST_ಅಂಶ_ಮೌಲ್ಯ_ಒಳ_USE ) == 0UL )
                        {
                            list_ಗಣ_LIST_ಅಂಶ_ಮೌಲ್ಯ( &( pxTCB->x_Event_List_ಅಂಶ ), ( ( TickType_t ) configMAX_PRIORITIES - ( TickType_t ) ux_New_ಆದ್ಯತೆ ) ); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */
                        }

                        /* If the task is in the blocked or suspended list we need do
                         * nothing more than change its priority variable. However, if
                         * the task is in a ready list it needs to be removed and placed
                         * in the list appropriate to its new priority. */
                        ಆದರೆ( listIS_CONTAINED_WITHIN( &( pxReadyTasksLists[ ux_ಆದ್ಯತೆ_ಬಳಸಿದ_On_ನಮೂದು ] ), &( pxTCB->x_State_List_ಅಂಶ ) ) != pdFALSE )
                        {
                            /* The task is currently in its ready list - remove before
                             * adding it to its new ready list.  As we are in a critical
                             * section we can do this even if the scheduler is suspended. */
                            ಆದರೆ( uxListRemove( &( pxTCB->x_State_List_ಅಂಶ ) ) == ( UBaseType_t ) 0 )
                            {
                                /* It is known that the task is in its ready list so
                                 * there is no need to check again and the port level
                                 * reset macro can be called directly. */
                                ಪೋರ್ಟ್_RESET_READY_ಆದ್ಯತೆ( ux_ಆದ್ಯತೆ_ಬಳಸಿದ_On_ನಮೂದು, ux_ತುದಿ_Ready_ಆದ್ಯತೆ );
                            }

                            prv_ಕೂಡಿಸು_Task_ಗಮ್ಯ_Ready_List( pxTCB );
                        }

                        ಆದರೆ( xYieldRequired != pdFALSE )
                        {
                            #ಆದರೆ CONFIG_FREERTOS_SMP
                                taskYIELD_TASK_CORE_IF_USING_PREEMPTION( pxTCB );
                            #ಇಲ್ಲದಿದ್ದರೆ
                                taskYIELD_IF_USING_PREEMPTION();
                            #ಆದರೆ_ಅಂತ್ಯ
                        }
                    }
                }
            }
            #ಇಲ್ಲದಿದ್ದರೆ /* if ( configUSE_MUTEXES == 1 ) */
            {
                v_Task_ಆದ್ಯತೆ_ಗಣ( ಖಾಲಿ, px_Saved_ಆದ್ಯತೆ->ux_ಆದ್ಯತೆ );
            }
            #ಆದರೆ_ಅಂತ್ಯ /* if ( configUSE_MUTEXES == 1 ) */
        }
        #ಆದರೆ CONFIG_FREERTOS_SMP
            taskEXIT_CRITICAL();
        #ಇಲ್ಲದಿದ್ದರೆ
            taskEXIT_CRITICAL( &x_Kernel_ಬೀಗ );
        #ಆದರೆ_ಅಂತ್ಯ
    }

#ಆದರೆ_ಅಂತ್ಯ /* ( INCLUDE_vTaskPrioritySet == 1 ) */
/*----------------------------------------------------------*/

/* --------------------------------------------- TLSP Deletion Callbacks -------------------------------------------- */

#ಆದರೆ CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS

    ಶೂನ್ಯ v_Task_ಗಣ_Thread_Local_Storage_ಸೂಚಕ_And_Del_Callback( TaskHandle_t x_Task_ಗಮ್ಯ_ಗಣ,
                                                          ತಳ_Type_t x_ಸೂಚ್ಯಂಕ,
                                                          ಶೂನ್ಯ * pv_ಮೌಲ್ಯ,
                                                          TlsDeleteCallbackFunction_t pvDelCallback )
    {
        /* If TLSP deletion callbacks are enabled, then configNUM_THREAD_LOCAL_STORAGE_POINTERS
         * is doubled in size so that the latter half of the pvThreadLocalStoragePointers
         * stores the deletion callbacks. */
        ಆದರೆ( x_ಸೂಚ್ಯಂಕ < ( config_ಅಂಕೆ_THREAD_LOCAL_STORAGE_ಸೂಚಕಗಳು / 2 ) )
        {
            TCB_t * pxTCB;

            #ಆದರೆ ( config_ಅಂಕೆ_CORES > 1 )
            {
                /* For SMP, we need a critical section as another core could also
                 * update this task's TLSP at the same time. */
                #ಆದರೆ CONFIG_FREERTOS_SMP
                {
                    taskENTER_CRITICAL();
                }
                #ಇಲ್ಲದಿದ್ದರೆ /* CONFIG_FREERTOS_SMP */
                {
                    taskENTER_CRITICAL( &x_Kernel_ಬೀಗ );
                }
                #ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_SMP */
            }
            #ಆದರೆ_ಅಂತ್ಯ /* configNUM_CORES > 1 */

            pxTCB = prvGetTCBFromHandle( x_Task_ಗಮ್ಯ_ಗಣ );
            /* Store the TLSP by indexing the first half of the array */
            pxTCB->pv_Thread_Local_Storage_ಸೂಚಕಗಳು[ x_ಸೂಚ್ಯಂಕ ] = pv_ಮೌಲ್ಯ;

            /* Store the TLSP deletion callback by indexing the second half
             * of the array. */
            pxTCB->pv_Thread_Local_Storage_ಸೂಚಕಗಳು[ ( x_ಸೂಚ್ಯಂಕ + ( config_ಅಂಕೆ_THREAD_LOCAL_STORAGE_ಸೂಚಕಗಳು / 2 ) ) ] = ( ಶೂನ್ಯ * ) pvDelCallback;

            #ಆದರೆ ( config_ಅಂಕೆ_CORES > 1 )
            {
                #ಆದರೆ CONFIG_FREERTOS_SMP
                {
                    taskEXIT_CRITICAL();
                }
                #ಇಲ್ಲದಿದ್ದರೆ /* CONFIG_FREERTOS_SMP */
                {
                    taskEXIT_CRITICAL( &x_Kernel_ಬೀಗ );
                }
                #ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_SMP */
            }
            #ಆದರೆ_ಅಂತ್ಯ /* configNUM_CORES > 1 */
        }
    }

#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */
/*----------------------------------------------------------*/

/* ----------------------------------------------------- Newlib ----------------------------------------------------- */

#ಆದರೆ ( configUSE_NEWLIB_REENTRANT == 1 )

/**
 * @brief Get reentrancy structure of the current task
 *
 * - This function is required by newlib (when __DYNAMIC_REENT__ is enabled)
 * - It will return a pointer to the current task's reent struct
 * - If FreeRTOS is not running, it will return the global reent struct
 *
 * @return Pointer to a the (current taks's)/(global) reent struct
 */
    ರಚನೆ _reent * __getreent( ಶೂನ್ಯ )
    {
        /* No lock needed because if this changes, we won't be running anymore. */
        TCB_t * pxCurTask = ( TCB_t * ) xTaskGetCurrentTaskHandle();
        ರಚನೆ _reent * ret;

        ಆದರೆ( pxCurTask == ಖಾಲಿ )
        {
            /* No task running. Return global struct. */
            ret = _GLOBAL_REENT;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            /* We have a currently executing task. Return its reentrant struct. */
            ret = &pxCurTask->xTLSBlock;
        }

        ಹಿಂತಿರುಗಿ ret;
    }

#ಆದರೆ_ಅಂತ್ಯ /* configUSE_NEWLIB_REENTRANT == 1 */

/* -------------------------------------------------- Task Snapshot ------------------------------------------------- */

/**
 * @brief List of all task lists in FreeRTOS
 *
 * @note There are currently differing number of task list between SMP FreeRTOS and ESP-IDF FreeRTOS
 */
ಸ್ಥಿರ List_t * non_ready_task_lists[] =
{
    #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CONFIG_FREERTOS_SMP
        &xPendingReadyList,
    #ಇಲ್ಲದಿದ್ದರೆ /* CONFIG_FREERTOS_SMP */
        &xPendingReadyList[ 0 ],
        #ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CONFIG_FREERTOS_UNICORE
            &xPendingReadyList[ 1 ],
        #ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_UNICORE */
    #ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_SMP */
    &xDelayedTaskList1,
    &xDelayedTaskList2,
    #ಆದರೆ ( INCLUDE_vTaskDelete == 1 )
        &x_Tasks_ಕಾಯುವಿಕೆ_Termination,
    #ಆದರೆ_ಅಂತ್ಯ
    #ಆದರೆ ( INCLUDE_vTaskSuspend == 1 )
        &xSuspendedTaskList,
    #ಆದರೆ_ಅಂತ್ಯ
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
ಸ್ಥಿರ List_t * px_Get_Task_List_By_ಸೂಚ್ಯಂಕ( UBaseType_t ux_List_ಸೂಚ್ಯಂಕ )
{
    List_t * pxTaskList;
    ಸ್ಥಿರಾಂಕ size_t xNonReadyTaskListsCnt = ( ಗಾತ್ರ( non_ready_task_lists ) / ಗಾತ್ರ( List_t * ) );

    ಆದರೆ( ux_List_ಸೂಚ್ಯಂಕ < configMAX_PRIORITIES )
    {
        pxTaskList = &pxReadyTasksLists[ configMAX_PRIORITIES - 1 - ux_List_ಸೂಚ್ಯಂಕ ];
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ( ux_List_ಸೂಚ್ಯಂಕ < configMAX_PRIORITIES + xNonReadyTaskListsCnt )
    {
        pxTaskList = non_ready_task_lists[ ux_List_ಸೂಚ್ಯಂಕ - configMAX_PRIORITIES ];
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        pxTaskList = ಖಾಲಿ;
    }

    /* sanity check */
    ಆದರೆ( pxTaskList )
    {
        ಆದರೆ( !ಪೋರ್ಟ್_VALID_LIST_MEM( pxTaskList ) )
        {
            pxTaskList = ಖಾಲಿ;
        }
    }

    ಹಿಂತಿರುಗಿ pxTaskList;
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
ಸ್ಥಿರ ಸಾಲೊಳಗೆ UBaseType_t px_Get_Task_List_ಎಣಿಕೆ( ಶೂನ್ಯ )
{
    ಹಿಂತಿರುಗಿ configMAX_PRIORITIES + ( ಗಾತ್ರ( non_ready_task_lists ) / ಗಾತ್ರ( List_t * ) );
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
ಪೂರ್ಣಾಂಕ x_Task_Get_ಮುಂದಿನ( TaskIterator_t * xIterator )
{
    ಆದರೆ( !xIterator )
    {
        ಹಿಂತಿರುಗಿ -1;
    }

    List_ಅಂಶ_t * px_ಮುಂದಿನ_List_ಅಂಶ = xIterator->px_ಮುಂದಿನ_List_ಅಂಶ;
    UBaseType_t uxCurListIdx = xIterator->ux_Current_List_ಸೂಚ್ಯಂಕ;
    UBaseType_t uxMaxListIdx = px_Get_Task_List_ಎಣಿಕೆ();

    ಗಾಗಿ( ; uxCurListIdx < uxMaxListIdx; ++uxCurListIdx )
    {
        List_t * pxCurrentTaskList = px_Get_Task_List_By_ಸೂಚ್ಯಂಕ( uxCurListIdx );

        ಆದರೆ( !pxCurrentTaskList || ( list_CURRENT_LIST_ಉದ್ದಳತೆ( pxCurrentTaskList ) == 0 ) )
        {
            ಮುಂದುವರಿ;
        }

        ಸ್ಥಿರಾಂಕ List_ಅಂಶ_t * px_Curr_List_ಅಂಶ = list_GET_ಕೊನೆ_MARKER( pxCurrentTaskList );

        ಆದರೆ( !px_ಮುಂದಿನ_List_ಅಂಶ )
        {
            /* We are here if the traversal starts from the beginning or when we finish traversing
             * for one of the state lists
             */
            px_ಮುಂದಿನ_List_ಅಂಶ = list_GET_ಮುಂದಿನ( px_Curr_List_ಅಂಶ );
        }

        ಆದರೆ( !ಪೋರ್ಟ್_VALID_LIST_MEM( px_ಮುಂದಿನ_List_ಅಂಶ ) )
        {
            /* Nothing to do with the corrupted list item. We will skip to the next task state list.
             * pxNextListItem should be NULL at the beginning of each task list.
             */
            px_ಮುಂದಿನ_List_ಅಂಶ = ಖಾಲಿ;
            ಮುಂದುವರಿ;
        }

        TCB_t * pxTCB = ( TCB_t * ) list_GET_LIST_ಅಂಶ_OWNER( px_ಮುಂದಿನ_List_ಅಂಶ );

        ಆದರೆ( !ಪೋರ್ಟ್_VALID_TCB_MEM( pxTCB ) )
        {
            pxTCB = ಖಾಲಿ;
        }

        xIterator->pxTaskHandle = pxTCB;
        xIterator->ux_Current_List_ಸೂಚ್ಯಂಕ = uxCurListIdx;

        ಆದರೆ( px_Curr_List_ಅಂಶ->px_ಹಿಂದಿನ == px_ಮುಂದಿನ_List_ಅಂಶ )
        {
            /* If this is the last item of the current state list */
            xIterator->ux_Current_List_ಸೂಚ್ಯಂಕ++;
            xIterator->px_ಮುಂದಿನ_List_ಅಂಶ = ಖಾಲಿ;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            xIterator->px_ಮುಂದಿನ_List_ಅಂಶ = list_GET_ಮುಂದಿನ( px_ಮುಂದಿನ_List_ಅಂಶ );
        }

        ಹಿಂತಿರುಗಿ uxCurListIdx;
    }

    ಹಿಂತಿರುಗಿ -1; /* end of the task list */
}
/*----------------------------------------------------------*/

ತಳ_Type_t vTaskGetSnapshot( TaskHandle_t pxTask,
                             TaskSnapshot_t * pxTaskSnapshot )
{
    ESP_STATIC_ANALYZER_CHECK(!pxTask, pdFALSE);

    ಆದರೆ( ( ಪೋರ್ಟ್_VALID_TCB_MEM( pxTask ) == false ) || ( pxTaskSnapshot == ಖಾಲಿ ) )
    {
        ಹಿಂತಿರುಗಿ pdFALSE;
    }

    TCB_t * pxTCB = ( TCB_t * ) pxTask;
    pxTaskSnapshot->pxTCB = pxTCB;
    pxTaskSnapshot->px_ತುದಿ_ದ_ರಾಶಿ = ( ರಾಶಿ_Type_t * ) pxTCB->px_ತುದಿ_ದ_ರಾಶಿ;
    pxTaskSnapshot->px_ಕೊನೆ_ದ_ರಾಶಿ = ( ರಾಶಿ_Type_t * ) pxTCB->px_ಕೊನೆ_ದ_ರಾಶಿ;
    ಹಿಂತಿರುಗಿ pdTRUE;
}
/*----------------------------------------------------------*/

UBaseType_t uxTaskGetSnapshotAll( TaskSnapshot_t * ಸ್ಥಿರಾಂಕ pxTaskSnapshotArray,
                                  ಸ್ಥಿರಾಂಕ UBaseType_t ux_Array_ಉದ್ದಳತೆ,
                                  UBaseType_t * ಸ್ಥಿರಾಂಕ pxTCBSize )
{
    UBaseType_t ux_Array_ಅಂಕೆ_Filled = 0;

    /* Traverse all of the tasks lists */
    TaskIterator_t xTaskIter = { 0 }; /* Point to the first task list */

    ಆಗ( x_Task_Get_ಮುಂದಿನ( &xTaskIter ) != -1 && ux_Array_ಅಂಕೆ_Filled < ux_Array_ಉದ್ದಳತೆ )
    {
        vTaskGetSnapshot( xTaskIter.pxTaskHandle, &pxTaskSnapshotArray[ ux_Array_ಅಂಕೆ_Filled ] );
        ux_Array_ಅಂಕೆ_Filled++;
    }

    ಆದರೆ( pxTCBSize != ಖಾಲಿ )
    {
        *pxTCBSize = ಗಾತ್ರ( TCB_t );
    }

    ಹಿಂತಿರುಗಿ ux_Array_ಅಂಕೆ_Filled;
}
/*----------------------------------------------------------*/

/* ----------------------------------------------------- Misc ----------------------------------------------------- */

ಶೂನ್ಯ * pvTaskGetCurrentTCBForCore( ತಳ_Type_t xCoreID )
{
    ಶೂನ್ಯ * pvRet;

    configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE );

    #ಆದರೆ CONFIG_FREERTOS_SMP
        /* SMP FreeRTOS defines pxCurrentTCB as a macro function call */
        pvRet = ( ಶೂನ್ಯ * ) pxCurrentTCB;
    #ಇಲ್ಲದಿದ್ದರೆ /* CONFIG_FREERTOS_SMP */
        pvRet = ( ಶೂನ್ಯ * ) pxCurrentTCBs[ xCoreID ];
    #ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_SMP */
    ಹಿಂತಿರುಗಿ pvRet;
}

/* ----------------------------------------------------- OpenOCD ---------------------------------------------------- */

#ಆದರೆ CONFIG_FREERTOS_DEBUG_OCDAWARE

/**
 * Debug param indexes. DO NOT change the order. OpenOCD uses the same indexes
 * Entries in FreeRTOS_openocd_params must match the order of these indexes
 */
    ಎಣಿಕೆ
    {
        ESP_FREERTOS_DEBUG_ಕೋಷ್ಟಕ_ಗಾತ್ರ = 0,
        ESP_FREERTOS_DEBUG_ಕೋಷ್ಟಕ_ಆವೃತ್ತಿ,
        ESP_FREERTOS_DEBUG_KERNEL_VER_MAJOR,
        ESP_FREERTOS_DEBUG_KERNEL_VER_MINOR,
        ESP_FREERTOS_DEBUG_KERNEL_VER_BUILD,
        ESP_FREERTOS_DEBUG_UX_ತುದಿ_ಬಳಸಿದ_PIORITY,
        ESP_FREERTOS_DEBUG_PX_ತುದಿ_ದ_ರಾಶಿ,
        ESP_FREERTOS_DEBUG_PC_TASK_NAME,
        ESP_FREERTOS_DEBUG_LIST_ಗಾತ್ರ,
        ESP_FREERTOS_DEBUG_LIST_ಅಂಕೆ_ವಸ್ತುಗಳು,
        ESP_FREERTOS_DEBUG_LIST_ಕೊನೆ,
        ESP_FREERTOS_DEBUG_LIST_ಕೊನೆ_PREV,
        ESP_FREERTOS_DEBUG_LIST_ಅಂಶ_PREV,
        ESP_FREERTOS_DEBUG_LIST_ಅಂಶ_OWNER,
        ESP_FREERTOS_DEBUG_TASK_ಎಣಿಕೆ_ಅಗಲ,
        ESP_FREERTOS_DEBUG_PTR_ಅಗಲ,
        /* New entries must be inserted here */
        ESP_FREERTOS_DEBUG_ಕೋಷ್ಟಕ_ಕೊನೆ,
    };

    ಸ್ಥಿರಾಂಕ DRAM_ATTR uint8_t FreeRTOS_openocd_params[ ESP_FREERTOS_DEBUG_ಕೋಷ್ಟಕ_ಕೊನೆ ] =
    {
        ESP_FREERTOS_DEBUG_ಕೋಷ್ಟಕ_ಕೊನೆ, /* table size */
        1,                            /* table version */
        tsk_KERNEL_ಆವೃತ್ತಿ_MAJOR,
        tsk_KERNEL_ಆವೃತ್ತಿ_MINOR,
        tsk_KERNEL_ಆವೃತ್ತಿ_BUILD,
        configMAX_PRIORITIES - 1,                   /* uxTopUsedPriority */
        offsetof( TCB_t, px_ತುದಿ_ದ_ರಾಶಿ ),            /* thread_stack_offset; */
        offsetof( TCB_t, pcTaskName ),              /* thread_name_offset; */
        ಗಾತ್ರ( List_t ),                           /* list_width */
        offsetof( List_t, ux_ಸಂಖ್ಯೆ_ದ_ವಸ್ತುಗಳು ),        /* list_item_num */
        offsetof( List_t, x_List_ಕೊನೆ ),               /* list_end_offset */
        offsetof( List_t, x_List_ಕೊನೆ.px_ಹಿಂದಿನ ),    /* list_next_offset */
        offsetof( List_ಅಂಶ_t, px_ಹಿಂದಿನ ),         /* list_elem_next_offset */
        offsetof( List_ಅಂಶ_t, pvOwner ),            /* list_elem_content_offset */
        ಗಾತ್ರ( UBaseType_t ),                      /* task_count_width */
        ಗಾತ್ರ( ಶೂನ್ಯ * )                            /* ptr_width */
    };

#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_DEBUG_OCDAWARE */
/*----------------------------------------------------------*/

/* ----------------------------------------------------- PSRAM ---------------------------------------------------- */

#ಆದರೆ CONFIG_SPIRAM

    #ಆದರೆ CONFIG_FREERTOS_SMP
        ತಳ_Type_t prv_Task_Create_Dynamic_Affinity_ಗಣ_With_Caps( TaskFunction_t px_Task_ಸಂಕೇತ,
                                                            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ pcName,
                                                            ಸ್ಥಿರಾಂಕ config_ರಾಶಿ_ಆಳ_TYPE us_ರಾಶಿ_ಆಳ,
                                                            ಶೂನ್ಯ * ಸ್ಥಿರಾಂಕ pvParameters,
                                                            UBaseType_t ux_ಆದ್ಯತೆ,
                                                            UBaseType_t ux_Core_Affinity_ಮುಖವಾಡ,
                                                            UBaseType_t ux_ರಾಶಿ_ಸ್ಮರಣೆ_Caps,
                                                            TaskHandle_t * ಸ್ಥಿರಾಂಕ pxCreatedTask )
    #ಇಲ್ಲದಿದ್ದರೆ /* CONFIG_FREERTOS_SMP */
        ತಳ_Type_t prv_Task_Create_Dynamic_Pinned_ಗಮ್ಯ_Core_With_Caps( TaskFunction_t px_Task_ಸಂಕೇತ,
                                                             ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ pcName,
                                                             ಸ್ಥಿರಾಂಕ config_ರಾಶಿ_ಆಳ_TYPE us_ರಾಶಿ_ಆಳ,
                                                             ಶೂನ್ಯ * ಸ್ಥಿರಾಂಕ pvParameters,
                                                             UBaseType_t ux_ಆದ್ಯತೆ,
                                                             ಸ್ಥಿರಾಂಕ ತಳ_Type_t xCoreID,
                                                             UBaseType_t ux_ರಾಶಿ_ಸ್ಮರಣೆ_Caps,
                                                             TaskHandle_t * ಸ್ಥಿರಾಂಕ pxCreatedTask )
    #ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_SMP */
    {
        TCB_t * pxNewTCB;
        ತಳ_Type_t xReturn;

        ರಾಶಿ_Type_t * px_ರಾಶಿ;

        configASSERT( ux_ರಾಶಿ_ಸ್ಮರಣೆ_Caps & ( MALLOC_CAP_8_ಬಿಟ್ ) );
        configASSERT( ( ux_ರಾಶಿ_ಸ್ಮರಣೆ_Caps & MALLOC_CAP_SPIRAM ) ||
                      ( ux_ರಾಶಿ_ಸ್ಮರಣೆ_Caps & MALLOC_CAP_ಆಂತರಿಕ ) );
        #ಆದರೆ ( !CONFIG_FREERTOS_SMP )
        {
            configASSERT( taskVALID_CORE_ID( xCoreID ) == pdTRUE || xCoreID == tskNO_AFFINITY );
        }
        #ಆದರೆ_ಅಂತ್ಯ /* !CONFIG_FREERTOS_SMP */

        /* Allocate space for the stack used by the task being created. */
        px_ರಾಶಿ = ರಾಶಿ_caps_malloc( ( ( ( size_t ) us_ರಾಶಿ_ಆಳ ) * ಗಾತ್ರ( ರಾಶಿ_Type_t ) ), ux_ರಾಶಿ_ಸ್ಮರಣೆ_Caps );

        ಆದರೆ( px_ರಾಶಿ != ಖಾಲಿ )
        {
            /* Allocate space for the TCB. */
            pxNewTCB = ( TCB_t * ) pv_ಪೋರ್ಟ್_Malloc( ಗಾತ್ರ( TCB_t ) );

            ಆದರೆ( pxNewTCB != ಖಾಲಿ )
            {
                memset( ( ಶೂನ್ಯ * ) pxNewTCB, 0x00, ಗಾತ್ರ( TCB_t ) );

                /* Store the stack location in the TCB. */
                pxNewTCB->px_ರಾಶಿ = px_ರಾಶಿ;
            }
            ಇಲ್ಲದಿದ್ದರೆ
            {
                /* The stack cannot be used as the TCB has not been created. Free it. */
                ರಾಶಿ_caps_free( px_ರಾಶಿ );
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            pxNewTCB = ಖಾಲಿ;
        }

        ಆದರೆ( pxNewTCB != ಖಾಲಿ )
        {
            #ಆದರೆ ( tsk_STATIC_AND_DYNAMIC_ಹಂಚಿಕೆ_POSSIBLE != 0 )
            {
                /* Tasks can be created statically or dynamically, so note this
                 * task was created dynamically in case it is later deleted. */
                pxNewTCB->ucStaticallyAllocated = tsk_DYNAMICALLY_ALLOCATED_ರಾಶಿ_AND_TCB;
            }
            #ಆದರೆ_ಅಂತ್ಯ /* tskSTATIC_AND_DYNAMIC_ALLOCATION_POSSIBLE */

            #ಆದರೆ CONFIG_FREERTOS_SMP
            {
                prvInitialiseNewTask( px_Task_ಸಂಕೇತ, pcName, ( uint32_t ) us_ರಾಶಿ_ಆಳ, pvParameters, ux_ಆದ್ಯತೆ, pxCreatedTask, pxNewTCB, ಖಾಲಿ );
                #ಆದರೆ ( ( config_ಅಂಕೆ_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) )
                {
                    /* Set the task's affinity before scheduling it */
                    pxNewTCB->ux_Core_Affinity_ಮುಖವಾಡ = ux_Core_Affinity_ಮುಖವಾಡ;
                }
                #ಆದರೆ_ಅಂತ್ಯ /* ( ( configNUM_CORES > 1 ) && ( configUSE_CORE_AFFINITY == 1 ) ) */
            }
            #ಇಲ್ಲದಿದ್ದರೆ /* CONFIG_FREERTOS_SMP */
            {
                prvInitialiseNewTask( px_Task_ಸಂಕೇತ, pcName, ( uint32_t ) us_ರಾಶಿ_ಆಳ, pvParameters, ux_ಆದ್ಯತೆ, pxCreatedTask, pxNewTCB, ಖಾಲಿ, xCoreID );
            }
            #ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_SMP */

            prv_ಕೂಡಿಸು_New_Task_ಗಮ್ಯ_Ready_List( pxNewTCB );
            xReturn = pdPASS;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            xReturn = err_COULD_NOT_ALLOCATE_REQUIRED_ಸ್ಮರಣೆ;
        }

        ಹಿಂತಿರುಗಿ xReturn;
    }

#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_SPIRAM */
/*----------------------------------------------------------*/
