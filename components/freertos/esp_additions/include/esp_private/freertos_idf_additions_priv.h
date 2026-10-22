/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#निर्देशः once

/*
 * This file is like "idf_additions.h" but for private API (i.e., only meant to
 * be called internally by other ESP-IDF components.
 */

#अन्तर्भाव "sdkconfig.h"
#अन्तर्भाव "freertos/FreeRTOS.h"
#अन्तर्भाव "freertos/task.h"

/* *INDENT-OFF* */
#यदि_परिभाषितम् __cplusplus
    बाह्य "C" {
#यदि_अन्तः
/* *INDENT-ON* */

/*------------------------------------------------------------------------------
 * KERNEL CONTROL (PRIVATE)
 *----------------------------------------------------------------------------*/

/*
 * The following macros are convenience macros used to account for different
 * thread safety behavior between single-core and SMP in ESP-IDF FreeRTOS.
 *
 * For thread safety...
 *
 * - Single-core will use the following for thread safety (depending on situation)
 *      - `vTaskSuspendAll()`/`xTaskResumeAll()` for non-deterministic operations
 *      - Critical sections or disabling interrupts for deterministic operations
 * - SMP will always use critical sections (determinism is not supported)
 */
#यदि ( !CONFIG_FREERTOS_SMP && ( config_अङ्कः_CORES > 1 ) )

/* Macros that use a critical section when building for SMP */
    #परिभाषय prvENTER_CRITICAL_OR_SUSPEND_ALL( px_तालकम् )    taskENTER_CRITICAL( ( px_तालकम् ) )
    #परिभाषय prvEXIT_CRITICAL_OR_RESUME_ALL( px_तालकम् )      ( { taskEXIT_CRITICAL( ( px_तालकम् ) ); pdFALSE; } )
    #परिभाषय prv_ENTER_CRITICAL_OR_आवरणम्_ISR( px_तालकम्, ux_स्थितिः ) \
    {                                                         \
        taskENTER_CRITICAL_ISR( ( px_तालकम् ) );                 \
        ( शून्य ) ( ux_स्थितिः );                                \
    }
    #परिभाषय prvEXIT_CRITICAL_OR_UNMASK_ISR( px_तालकम्, ux_स्थितिः ) \
    {                                                          \
        taskEXIT_CRITICAL_ISR( ( px_तालकम् ) );                   \
        ( शून्य ) ( ux_स्थितिः );                                 \
    }

/* Macros that enter/exit a critical section only when building for SMP */
    #परिभाषय prvENTER_CRITICAL_SMP_ONLY( px_तालकम् )         taskENTER_CRITICAL( px_तालकम् )
    #परिभाषय prvEXIT_CRITICAL_SMP_ONLY( px_तालकम् )          taskEXIT_CRITICAL( px_तालकम् )
    #परिभाषय prvENTER_CRITICAL_ISR_SMP_ONLY( px_तालकम् )     taskENTER_CRITICAL_ISR( px_तालकम् )
    #परिभाषय prvEXIT_CRITICAL_ISR_SMP_ONLY( px_तालकम् )      taskEXIT_CRITICAL_ISR( px_तालकम् )
    #परिभाषय prvENTER_CRITICAL_SAFE_SMP_ONLY( px_तालकम् )    prvTaskEnterCriticalSafeSMPOnly( px_तालकम् )
    #परिभाषय prvEXIT_CRITICAL_SAFE_SMP_ONLY( px_तालकम् )     prvTaskExitCriticalSafeSMPOnly( px_तालकम् )

    स्थिर अन्तःस्थ __attribute__( ( always_inline ) )
    शून्य prvTaskEnterCriticalSafeSMPOnly( पत्तनम्_MUX_TYPE * px_तालकम् )
    {
        यदि( पत्तनम्_CHECK_IF_अन्तर्_ISR() == pdFALSE )
        {
            taskENTER_CRITICAL( px_तालकम् );
        }
        अन्यथा
        {
            #यदि_परिभाषितम् __clang_analyzer__
                /* Teach clang-tidy that ISR version macro can be different */
                configASSERT( 1 );
            #यदि_अन्तः
            taskENTER_CRITICAL_ISR( px_तालकम् );
        }
    }

    स्थिर अन्तःस्थ __attribute__( ( always_inline ) )
    शून्य prvTaskExitCriticalSafeSMPOnly( पत्तनम्_MUX_TYPE * px_तालकम् )
    {
        यदि( पत्तनम्_CHECK_IF_अन्तर्_ISR() == pdFALSE )
        {
            taskEXIT_CRITICAL( px_तालकम् );
        }
        अन्यथा
        {
            #यदि_परिभाषितम् __clang_analyzer__
                /* Teach clang-tidy that ISR version macro can be different */
                configASSERT( 1 );
            #यदि_अन्तः
            taskEXIT_CRITICAL_ISR( px_तालकम् );
        }
    }

/* Macros that enter/exit a critical section only when building for single-core */
    #परिभाषय prvENTER_CRITICAL_SC_ONLY( px_तालकम् )
    #परिभाषय prvEXIT_CRITICAL_SC_ONLY( px_तालकम् )

/* Macros that enable/disable interrupts only when building for SMP */
    #परिभाषय prvDISABLE_INTERRUPTS_ISR_SMP_ONLY()             पत्तनम्_समुच्चयः_व्यवधानम्_आवरणम्_FROM_ISR()
    #परिभाषय prvENABLE_INTERRUPTS_ISR_SMP_ONLY( ux_स्थितिः )    पत्तनम्_CLEAR_व्यवधानम्_आवरणम्_FROM_ISR( ux_स्थितिः )

#नोचेद्यदि ( !CONFIG_FREERTOS_SMP && ( config_अङ्कः_CORES == 1 ) )

/* Macros that suspend the scheduler or disables interrupts when building for single-core */
    #परिभाषय prvENTER_CRITICAL_OR_SUSPEND_ALL( px_तालकम् )    ( { vTaskSuspendAll(); ( शून्य ) ( px_तालकम् ); } )
    #परिभाषय prvEXIT_CRITICAL_OR_RESUME_ALL( px_तालकम् )      xTaskResumeAll()
    #परिभाषय prv_ENTER_CRITICAL_OR_आवरणम्_ISR( px_तालकम्, ux_स्थितिः ) \
    {                                                         \
        ( ux_स्थितिः ) = पत्तनम्_समुच्चयः_व्यवधानम्_आवरणम्_FROM_ISR();     \
        ( शून्य ) ( px_तालकम् );                                  \
    }
    #परिभाषय prvEXIT_CRITICAL_OR_UNMASK_ISR( px_तालकम्, ux_स्थितिः ) \
    {                                                          \
        पत्तनम्_CLEAR_व्यवधानम्_आवरणम्_FROM_ISR( ( ux_स्थितिः ) );     \
        ( शून्य ) ( px_तालकम् );                                   \
    }

/* Macros that enter/exit a critical section only when building for SMP */
#यदि !defined prvENTER_CRITICAL_SMP_ONLY
    #परिभाषय prvENTER_CRITICAL_SMP_ONLY( px_तालकम् )
#यदि_अन्तः
#यदि !defined prvEXIT_CRITICAL_SMP_ONLY
    #परिभाषय prvEXIT_CRITICAL_SMP_ONLY( px_तालकम् )
#यदि_अन्तः
#यदि !defined prvENTER_CRITICAL_ISR_SMP_ONLY
    #परिभाषय prvENTER_CRITICAL_ISR_SMP_ONLY( px_तालकम् )
#यदि_अन्तः
#यदि !defined prvEXIT_CRITICAL_ISR_SMP_ONLY
    #परिभाषय prvEXIT_CRITICAL_ISR_SMP_ONLY( px_तालकम् )
#यदि_अन्तः
#यदि !defined prvENTER_CRITICAL_SAFE_SMP_ONLY
    #परिभाषय prvENTER_CRITICAL_SAFE_SMP_ONLY( px_तालकम् )
#यदि_अन्तः
#यदि !defined prvEXIT_CRITICAL_SAFE_SMP_ONLY
    #परिभाषय prvEXIT_CRITICAL_SAFE_SMP_ONLY( px_तालकम् )
#यदि_अन्तः

/* Macros that enter/exit a critical section only when building for single-core */
    #परिभाषय prvENTER_CRITICAL_SC_ONLY( px_तालकम् )              taskENTER_CRITICAL( px_तालकम् )
    #परिभाषय prvEXIT_CRITICAL_SC_ONLY( px_तालकम् )               taskEXIT_CRITICAL( px_तालकम् )

/* Macros that enable/disable interrupts only when building for SMP */
    #परिभाषय prvDISABLE_INTERRUPTS_ISR_SMP_ONLY()             ( ( UBaseType_t ) 0 )
    #परिभाषय prvENABLE_INTERRUPTS_ISR_SMP_ONLY( ux_स्थितिः )    ( ( शून्य ) ux_स्थितिः )

#यदि_अन्तः /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES == 1 ) ) */

/*
 * In ESP-IDF FreeRTOS under SMP builds, spinlocks are to protect different
 * groups of data. This function is a wrapper to take the "xKernelLock" spinlock
 * of tasks.c.
 *
 * This lock is taken whenever any of the kernel's data structures are
 * accessed/modified, such as when adding/removing tasks to/from the delayed
 * task list or various event lists.
 *
 * In more cases, kernel data structures are not accessed by functions outside
 * tasks.c. Thus, all accesses of the kernel data structures inside tasks.c will
 * handle the taking/releasing of the "xKerneLock".
 *
 * This functions is meant to be called by xEventGroupSetBits() and
 * vEventGroupDelete() as both those functions will directly access event lists
 * (which are kernel data structures). Thus, a wrapper function must be provided
 * to take/release the "xKernelLock" from outside tasks.c.
 */
#यदि ( !CONFIG_FREERTOS_SMP && ( config_अङ्कः_CORES > 1 ) )

    शून्य prv_Take_Kernel_तालकम्( शून्य );
    शून्य prv_Release_Kernel_तालकम्( शून्य );

#यदि_अन्तः /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */

#यदि ( CONFIG_FREERTOS_SMP && ( config_अङ्कः_CORES > 1 ) )

/**
 * @brief Workaround for non-thread safe multi-core OS startup (see IDF-4524)
 *
 * This function must be called with interrupts disabled on all cores other than
 * core 0 during startup.
 */
    शून्य prvStartSchedulerOtherCores( शून्य );

#यदि_अन्तः /* ( CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */

/*
 * In ESP-IDF FreeRTOS (i.e., multi-core SMP), core 0 manages the the FreeRTOS
 * tick count. Thus only core 0 calls xTaskIncrementTick().
 *
 * However, all other cores also receive a periodic tick interrupt. Thus all
 * other cores should call this function instead.
 *
 * This function will check if the current core requires time slicing, and also
 * call the application tick hook. However, the tick count will remain unchanged.
 */
#यदि ( !CONFIG_FREERTOS_SMP && ( config_अङ्कः_CORES > 1 ) )

    आधारः_Type_t xTaskIncrementTickOtherCores( शून्य );

#यदि_अन्तः /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */

#यदि CONFIG_FREERTOS_SYSTICK_उपयुङ्क्ते_SYSTIMER

/*
 * Reconcile the given core's OS-tick accounting against the free-running counter while its
 * OS-tick interrupt is still masked.
 *
 * Updates s_handled_systicks using the same accounting as SysTickIsrHandler(), so
 * re-enabling the alarm afterwards will not reprocess suppressed ticks. The caller should
 * restore the OS-tick interrupt and then advance the RTOS tick by the returned amount.
 *
 * @param cpu_id Core whose suppressed OS-tick accounting should be reconciled.
 *
 * @return Number of whole OS-tick periods that elapsed while masked.
 */
    uint32_t x_पत्तनम्_Sys_Tick_Claim_Elapsed_Ticks( पूर्णाङ्क cpu_id );

#यदि_अन्तः /* CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER */

/*------------------------------------------------------------------------------
 * TASK UTILITIES (PRIVATE)
 *----------------------------------------------------------------------------*/
#यदि ( INCLUDE_v_Task_प्राथमिकता_समुच्चयः == 1 )

/**
 * @brief Structure to save a task's previous priority
 *
 * This structure is meant to be used with prvTaskPriorityRaise() and
 * prvTaskPriorityRestore().
 */
    प्रकारनिर्देश संरचना
    {
        UBaseType_t ux_प्राथमिकता;
        #यदि ( configUSE_MUTEXES == 1 )
            UBaseType_t ux_आधारः_प्राथमिकता;
        #यदि_अन्तः
    } prv_Task_Saved_प्राथमिकता_t;

#यदि_अन्तः /* INCLUDE_vTaskPrioritySet == 1 */

#यदि ( INCLUDE_v_Task_प्राथमिकता_समुच्चयः == 1 )

/**
 * INCLUDE_vTaskPrioritySet must be defined as 1 for this function to be
 * available. See the configuration section for more information.
 *
 * Saves the current priority and current base priority of a task, then raises
 * the task's current and base priority to uxNewPriority if uxNewPriority is of
 * a higher priority.
 *
 * Once a task's priority has been raised with this function, the priority
 * can be restored by calling prvTaskPriorityRestore()
 *
 * - Note that this function differs from vTaskPrioritySet() as the task's
 *   current priority will be modified even if the task has already
 *   inherited a priority.
 * - This function is intended for special circumstance where a task must be
 *   forced immediately to a higher priority.
 *
 * For configUSE_MUTEXES == 0: A context switch will occur before the
 * function returns if the priority being set is higher than the priority of the
 * currently executing task.
 *
 * @note This functions is private and should only be called internally
 * within various IDF components. Users should never call this function from
 * their application.
 *
 * @note vTaskPrioritySet() should not be called while a task's priority is
 * already raised via this function
 *
 * @param pxSavedPriority returns base and current priorities
 *
 * @param uxNewPriority The priority to which the task's priority will be
 * set.
 */
    शून्य prv_Task_प्राथमिकता_Raise( prv_Task_Saved_प्राथमिकता_t * px_Saved_प्राथमिकता,
                               UBaseType_t ux_New_प्राथमिकता );

#यदि_अन्तः /* INCLUDE_vTaskPrioritySet == 1 */

#यदि ( INCLUDE_v_Task_प्राथमिकता_समुच्चयः == 1 )

/**
 * INCLUDE_vTaskPrioritySet must be defined as 1 for this function to be
 * available. See the configuration section for more information.
 *
 * Restore a task's priority that was previously raised by
 * prvTaskPriorityRaise().
 *
 * For configUSE_MUTEXES == 0: A context switch will occur before the function
 * returns if the priority being set is higher than the priority of the currently
 * executing task.
 *
 * @note This functions is private and should only be called internally within
 * various IDF components. Users should never call this function from their
 * application.
 *
 * @param pxSavedPriority previously saved base and current priorities that need
 * to be restored
 */
    शून्य prv_Task_प्राथमिकता_Restore( prv_Task_Saved_प्राथमिकता_t * px_Saved_प्राथमिकता );

#यदि_अन्तः /* INCLUDE_vTaskPrioritySet == 1 */

#यदि CONFIG_SPIRAM

/**
 * Create a new task with stack having the desired heap capabilities and add it to the
 * list of tasks that are ready to run.
 *
 * @note: This is an internal function and meant for usage by pthread only.
 * @note: Tasks with stacks not having the default heap capabilities for FreeRTOS may
 *        not be able to run while flash cache is disabled.
 *        This is the case if, e.g., any task on the chip is reading or writing
 *        flash memory.
 *
 * @note: The difference between this function and xTaskCreatePinnedToCoreWithCaps()
 *        is the following:
 *        * A FreeRTOS task created by this function is deleted by normal FreeRTOS deletion functions,
 *          i.e., vTaskDelete().
 *        * A FreeRTOS task created by this function can delete itself.
 *
 * This function behaves like xTaskCreateAffinitySet(), except that it allocates
 * the stack from the heap with the desired heap capabilities.
 *
 * @param pxTaskCode Pointer to the task entry function.  Tasks
 * must be implemented to never return (i.e. continuous loop).
 *
 * @param pcName A descriptive name for the task.  This is mainly used to
 * facilitate debugging.  Max length defined by configMAX_TASK_NAME_LEN - default
 * is 16.
 *
 * @param usStackDepth The size of the task stack specified as the number of bytes.
 *
 * @param pvParameters Pointer that will be used as the parameter for the task
 * being created.
 *
 * @param uxPriority The priority at which the task should run.  Systems that
 * include MPU support can optionally create tasks in a privileged (system)
 * mode by setting bit portPRIVILEGE_BIT of the priority parameter.  For
 * example, to create a privileged task at priority 2 the uxPriority parameter
 * should be set to ( 2 | portPRIVILEGE_BIT ).
 *
 * @param xCoreID (only IDF SMP FreeRTOS)
 * The core to which the task is pinned to, or tskNO_AFFINITY if the task has no
 * core affinity.
 *
 * @param uxCoreAffinityMask (only Amazon SMP FreeRTOS)
 * A bitwise value that indicates the cores on which the task can run.
 * Cores are numbered from 0 to configNUM_CORES - 1.
 * For example, to ensure that a task can run on core 0 and core 1, set
 * uxCoreAffinityMask to 0x03. Note that only one of the cores will be used when
 * using the IDF SMP version of FreeRTOS instead of Amazon FreeRTOS SMP.
 *
 * @param uxStackMemoryCaps The heap caps bitfield describing the memory capabilities to
 * be used for stack memory. Currently, the capabilities have to include MALLOC_CAP_8BIT
 * and one of the following: MALLOC_CAP_SPIRAM, MALLOC_CAP_INTERNAL.
 *
 * @param pxCreatedTask Used to pass back a handle by which the created task
 * can be referenced.
 *
 * @return pdPASS if the task was successfully created and added to a ready
 * list, otherwise an error code defined in the file projdefs.h
 */
    #यदि CONFIG_FREERTOS_SMP
        आधारः_Type_t prv_Task_Create_Dynamic_Affinity_समुच्चयः_With_Caps( TaskFunction_t px_Task_संहिता,
                                                            ध्रुव वर्ण * ध्रुव pcName,
                                                            ध्रुव config_राशिः_गभीरता_TYPE us_राशिः_गभीरता,
                                                            शून्य * ध्रुव pvParameters,
                                                            UBaseType_t ux_प्राथमिकता,
                                                            UBaseType_t ux_Core_Affinity_आवरणम्,
                                                            UBaseType_t ux_राशिः_स्मृतिः_Caps,
                                                            TaskHandle_t * ध्रुव pxCreatedTask );
    #अन्यथा
        आधारः_Type_t prv_Task_Create_Dynamic_Pinned_गन्तव्यम्_Core_With_Caps( TaskFunction_t px_Task_संहिता,
                                                             ध्रुव वर्ण * ध्रुव pcName,
                                                             ध्रुव config_राशिः_गभीरता_TYPE us_राशिः_गभीरता,
                                                             शून्य * ध्रुव pvParameters,
                                                             UBaseType_t ux_प्राथमिकता,
                                                             ध्रुव आधारः_Type_t xCoreID,
                                                             UBaseType_t ux_राशिः_स्मृतिः_Caps,
                                                             TaskHandle_t * ध्रुव pxCreatedTask );
    #यदि_अन्तः // CONFIG_FREERTOS_SMP

#यदि_अन्तः // CONFIG_SPIRAM

/* *INDENT-OFF* */
#यदि_परिभाषितम् __cplusplus
    }
#यदि_अन्तः
/* *INDENT-ON* */
