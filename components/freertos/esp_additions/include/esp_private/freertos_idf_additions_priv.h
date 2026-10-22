/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ಸೂಚನೆ once

/*
 * This file is like "idf_additions.h" but for private API (i.e., only meant to
 * be called internally by other ESP-IDF components.
 */

#ಅನ್ತರ್ಭಾವ "sdkconfig.h"
#ಅನ್ತರ್ಭಾವ "freertos/FreeRTOS.h"
#ಅನ್ತರ್ಭಾವ "freertos/task.h"

/* *INDENT-OFF* */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
    ಬಾಹ್ಯ "C" {
#ಆದರೆ_ಅಂತ್ಯ
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
#ಆದರೆ ( !CONFIG_FREERTOS_SMP && ( config_ಅಂಕೆ_CORES > 1 ) )

/* Macros that use a critical section when building for SMP */
    #ವ್ಯಾಖ್ಯಾನಿಸು prvENTER_CRITICAL_OR_SUSPEND_ALL( px_ಬೀಗ )    taskENTER_CRITICAL( ( px_ಬೀಗ ) )
    #ವ್ಯಾಖ್ಯಾನಿಸು prvEXIT_CRITICAL_OR_RESUME_ALL( px_ಬೀಗ )      ( { taskEXIT_CRITICAL( ( px_ಬೀಗ ) ); pdFALSE; } )
    #ವ್ಯಾಖ್ಯಾನಿಸು prv_ENTER_CRITICAL_OR_ಮುಖವಾಡ_ISR( px_ಬೀಗ, ux_ಸ್ಥಿತಿ ) \
    {                                                         \
        taskENTER_CRITICAL_ISR( ( px_ಬೀಗ ) );                 \
        ( ಶೂನ್ಯ ) ( ux_ಸ್ಥಿತಿ );                                \
    }
    #ವ್ಯಾಖ್ಯಾನಿಸು prvEXIT_CRITICAL_OR_UNMASK_ISR( px_ಬೀಗ, ux_ಸ್ಥಿತಿ ) \
    {                                                          \
        taskEXIT_CRITICAL_ISR( ( px_ಬೀಗ ) );                   \
        ( ಶೂನ್ಯ ) ( ux_ಸ್ಥಿತಿ );                                 \
    }

/* Macros that enter/exit a critical section only when building for SMP */
    #ವ್ಯಾಖ್ಯಾನಿಸು prvENTER_CRITICAL_SMP_ONLY( px_ಬೀಗ )         taskENTER_CRITICAL( px_ಬೀಗ )
    #ವ್ಯಾಖ್ಯಾನಿಸು prvEXIT_CRITICAL_SMP_ONLY( px_ಬೀಗ )          taskEXIT_CRITICAL( px_ಬೀಗ )
    #ವ್ಯಾಖ್ಯಾನಿಸು prvENTER_CRITICAL_ISR_SMP_ONLY( px_ಬೀಗ )     taskENTER_CRITICAL_ISR( px_ಬೀಗ )
    #ವ್ಯಾಖ್ಯಾನಿಸು prvEXIT_CRITICAL_ISR_SMP_ONLY( px_ಬೀಗ )      taskEXIT_CRITICAL_ISR( px_ಬೀಗ )
    #ವ್ಯಾಖ್ಯಾನಿಸು prv_ENTER_CRITICAL_ಸುರಕ್ಷಿತ_SMP_ONLY( px_ಬೀಗ )    prv_Task_Enter_Critical_ಸುರಕ್ಷಿತ_SMPOnly( px_ಬೀಗ )
    #ವ್ಯಾಖ್ಯಾನಿಸು prv_EXIT_CRITICAL_ಸುರಕ್ಷಿತ_SMP_ONLY( px_ಬೀಗ )     prv_Task_Exit_Critical_ಸುರಕ್ಷಿತ_SMPOnly( px_ಬೀಗ )

    ಸ್ಥಿರ ಸಾಲೊಳಗೆ __attribute__( ( always_inline ) )
    ಶೂನ್ಯ prv_Task_Enter_Critical_ಸುರಕ್ಷಿತ_SMPOnly( ಪೋರ್ಟ್_MUX_TYPE * px_ಬೀಗ )
    {
        ಆದರೆ( ಪೋರ್ಟ್_CHECK_IF_ಒಳ_ISR() == pdFALSE )
        {
            taskENTER_CRITICAL( px_ಬೀಗ );
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __clang_analyzer__
                /* Teach clang-tidy that ISR version macro can be different */
                configASSERT( 1 );
            #ಆದರೆ_ಅಂತ್ಯ
            taskENTER_CRITICAL_ISR( px_ಬೀಗ );
        }
    }

    ಸ್ಥಿರ ಸಾಲೊಳಗೆ __attribute__( ( always_inline ) )
    ಶೂನ್ಯ prv_Task_Exit_Critical_ಸುರಕ್ಷಿತ_SMPOnly( ಪೋರ್ಟ್_MUX_TYPE * px_ಬೀಗ )
    {
        ಆದರೆ( ಪೋರ್ಟ್_CHECK_IF_ಒಳ_ISR() == pdFALSE )
        {
            taskEXIT_CRITICAL( px_ಬೀಗ );
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __clang_analyzer__
                /* Teach clang-tidy that ISR version macro can be different */
                configASSERT( 1 );
            #ಆದರೆ_ಅಂತ್ಯ
            taskEXIT_CRITICAL_ISR( px_ಬೀಗ );
        }
    }

/* Macros that enter/exit a critical section only when building for single-core */
    #ವ್ಯಾಖ್ಯಾನಿಸು prvENTER_CRITICAL_SC_ONLY( px_ಬೀಗ )
    #ವ್ಯಾಖ್ಯಾನಿಸು prvEXIT_CRITICAL_SC_ONLY( px_ಬೀಗ )

/* Macros that enable/disable interrupts only when building for SMP */
    #ವ್ಯಾಖ್ಯಾನಿಸು prvDISABLE_INTERRUPTS_ISR_SMP_ONLY()             ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR()
    #ವ್ಯಾಖ್ಯಾನಿಸು prvENABLE_INTERRUPTS_ISR_SMP_ONLY( ux_ಸ್ಥಿತಿ )    ಪೋರ್ಟ್_CLEAR_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR( ux_ಸ್ಥಿತಿ )

#ಅಥವಾಆದರೆ ( !CONFIG_FREERTOS_SMP && ( config_ಅಂಕೆ_CORES == 1 ) )

/* Macros that suspend the scheduler or disables interrupts when building for single-core */
    #ವ್ಯಾಖ್ಯಾನಿಸು prvENTER_CRITICAL_OR_SUSPEND_ALL( px_ಬೀಗ )    ( { vTaskSuspendAll(); ( ಶೂನ್ಯ ) ( px_ಬೀಗ ); } )
    #ವ್ಯಾಖ್ಯಾನಿಸು prvEXIT_CRITICAL_OR_RESUME_ALL( px_ಬೀಗ )      xTaskResumeAll()
    #ವ್ಯಾಖ್ಯಾನಿಸು prv_ENTER_CRITICAL_OR_ಮುಖವಾಡ_ISR( px_ಬೀಗ, ux_ಸ್ಥಿತಿ ) \
    {                                                         \
        ( ux_ಸ್ಥಿತಿ ) = ಪೋರ್ಟ್_ಗಣ_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR();     \
        ( ಶೂನ್ಯ ) ( px_ಬೀಗ );                                  \
    }
    #ವ್ಯಾಖ್ಯಾನಿಸು prvEXIT_CRITICAL_OR_UNMASK_ISR( px_ಬೀಗ, ux_ಸ್ಥಿತಿ ) \
    {                                                          \
        ಪೋರ್ಟ್_CLEAR_ಅಡಚಣೆ_ಮುಖವಾಡ_FROM_ISR( ( ux_ಸ್ಥಿತಿ ) );     \
        ( ಶೂನ್ಯ ) ( px_ಬೀಗ );                                   \
    }

/* Macros that enter/exit a critical section only when building for SMP */
#ಆದರೆ !defined prvENTER_CRITICAL_SMP_ONLY
    #ವ್ಯಾಖ್ಯಾನಿಸು prvENTER_CRITICAL_SMP_ONLY( px_ಬೀಗ )
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ !defined prvEXIT_CRITICAL_SMP_ONLY
    #ವ್ಯಾಖ್ಯಾನಿಸು prvEXIT_CRITICAL_SMP_ONLY( px_ಬೀಗ )
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ !defined prvENTER_CRITICAL_ISR_SMP_ONLY
    #ವ್ಯಾಖ್ಯಾನಿಸು prvENTER_CRITICAL_ISR_SMP_ONLY( px_ಬೀಗ )
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ !defined prvEXIT_CRITICAL_ISR_SMP_ONLY
    #ವ್ಯಾಖ್ಯಾನಿಸು prvEXIT_CRITICAL_ISR_SMP_ONLY( px_ಬೀಗ )
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ !defined prv_ENTER_CRITICAL_ಸುರಕ್ಷಿತ_SMP_ONLY
    #ವ್ಯಾಖ್ಯಾನಿಸು prv_ENTER_CRITICAL_ಸುರಕ್ಷಿತ_SMP_ONLY( px_ಬೀಗ )
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ !defined prv_EXIT_CRITICAL_ಸುರಕ್ಷಿತ_SMP_ONLY
    #ವ್ಯಾಖ್ಯಾನಿಸು prv_EXIT_CRITICAL_ಸುರಕ್ಷಿತ_SMP_ONLY( px_ಬೀಗ )
#ಆದರೆ_ಅಂತ್ಯ

/* Macros that enter/exit a critical section only when building for single-core */
    #ವ್ಯಾಖ್ಯಾನಿಸು prvENTER_CRITICAL_SC_ONLY( px_ಬೀಗ )              taskENTER_CRITICAL( px_ಬೀಗ )
    #ವ್ಯಾಖ್ಯಾನಿಸು prvEXIT_CRITICAL_SC_ONLY( px_ಬೀಗ )               taskEXIT_CRITICAL( px_ಬೀಗ )

/* Macros that enable/disable interrupts only when building for SMP */
    #ವ್ಯಾಖ್ಯಾನಿಸು prvDISABLE_INTERRUPTS_ISR_SMP_ONLY()             ( ( UBaseType_t ) 0 )
    #ವ್ಯಾಖ್ಯಾನಿಸು prvENABLE_INTERRUPTS_ISR_SMP_ONLY( ux_ಸ್ಥಿತಿ )    ( ( ಶೂನ್ಯ ) ux_ಸ್ಥಿತಿ )

#ಆದರೆ_ಅಂತ್ಯ /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES == 1 ) ) */

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
#ಆದರೆ ( !CONFIG_FREERTOS_SMP && ( config_ಅಂಕೆ_CORES > 1 ) )

    ಶೂನ್ಯ prv_Take_Kernel_ಬೀಗ( ಶೂನ್ಯ );
    ಶೂನ್ಯ prv_Release_Kernel_ಬೀಗ( ಶೂನ್ಯ );

#ಆದರೆ_ಅಂತ್ಯ /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */

#ಆದರೆ ( CONFIG_FREERTOS_SMP && ( config_ಅಂಕೆ_CORES > 1 ) )

/**
 * @brief Workaround for non-thread safe multi-core OS startup (see IDF-4524)
 *
 * This function must be called with interrupts disabled on all cores other than
 * core 0 during startup.
 */
    ಶೂನ್ಯ prv_ಆರಂಭ_Scheduler_Other_Cores( ಶೂನ್ಯ );

#ಆದರೆ_ಅಂತ್ಯ /* ( CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */

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
#ಆದರೆ ( !CONFIG_FREERTOS_SMP && ( config_ಅಂಕೆ_CORES > 1 ) )

    ತಳ_Type_t xTaskIncrementTickOtherCores( ಶೂನ್ಯ );

#ಆದರೆ_ಅಂತ್ಯ /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */

#ಆದರೆ CONFIG_FREERTOS_SYSTICK_ಬಳಸುತ್ತದೆ_SYSTIMER

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
    uint32_t x_ಪೋರ್ಟ್_Sys_Tick_Claim_Elapsed_Ticks( ಪೂರ್ಣಾಂಕ cpu_id );

#ಆದರೆ_ಅಂತ್ಯ /* CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER */

/*------------------------------------------------------------------------------
 * TASK UTILITIES (PRIVATE)
 *----------------------------------------------------------------------------*/
#ಆದರೆ ( INCLUDE_v_Task_ಆದ್ಯತೆ_ಗಣ == 1 )

/**
 * @brief Structure to save a task's previous priority
 *
 * This structure is meant to be used with prvTaskPriorityRaise() and
 * prvTaskPriorityRestore().
 */
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ
    {
        UBaseType_t ux_ಆದ್ಯತೆ;
        #ಆದರೆ ( configUSE_MUTEXES == 1 )
            UBaseType_t ux_ತಳ_ಆದ್ಯತೆ;
        #ಆದರೆ_ಅಂತ್ಯ
    } prv_Task_Saved_ಆದ್ಯತೆ_t;

#ಆದರೆ_ಅಂತ್ಯ /* INCLUDE_vTaskPrioritySet == 1 */

#ಆದರೆ ( INCLUDE_v_Task_ಆದ್ಯತೆ_ಗಣ == 1 )

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
    ಶೂನ್ಯ prv_Task_ಆದ್ಯತೆ_Raise( prv_Task_Saved_ಆದ್ಯತೆ_t * px_Saved_ಆದ್ಯತೆ,
                               UBaseType_t ux_New_ಆದ್ಯತೆ );

#ಆದರೆ_ಅಂತ್ಯ /* INCLUDE_vTaskPrioritySet == 1 */

#ಆದರೆ ( INCLUDE_v_Task_ಆದ್ಯತೆ_ಗಣ == 1 )

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
    ಶೂನ್ಯ prv_Task_ಆದ್ಯತೆ_Restore( prv_Task_Saved_ಆದ್ಯತೆ_t * px_Saved_ಆದ್ಯತೆ );

#ಆದರೆ_ಅಂತ್ಯ /* INCLUDE_vTaskPrioritySet == 1 */

#ಆದರೆ CONFIG_SPIRAM

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
    #ಆದರೆ CONFIG_FREERTOS_SMP
        ತಳ_Type_t prv_Task_Create_Dynamic_Affinity_ಗಣ_With_Caps( TaskFunction_t px_Task_ಸಂಕೇತ,
                                                            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ pcName,
                                                            ಸ್ಥಿರಾಂಕ config_ರಾಶಿ_ಆಳ_TYPE us_ರಾಶಿ_ಆಳ,
                                                            ಶೂನ್ಯ * ಸ್ಥಿರಾಂಕ pvParameters,
                                                            UBaseType_t ux_ಆದ್ಯತೆ,
                                                            UBaseType_t ux_Core_Affinity_ಮುಖವಾಡ,
                                                            UBaseType_t ux_ರಾಶಿ_ಸ್ಮರಣೆ_Caps,
                                                            TaskHandle_t * ಸ್ಥಿರಾಂಕ pxCreatedTask );
    #ಇಲ್ಲದಿದ್ದರೆ
        ತಳ_Type_t prv_Task_Create_Dynamic_Pinned_ಗಮ್ಯ_Core_With_Caps( TaskFunction_t px_Task_ಸಂಕೇತ,
                                                             ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ pcName,
                                                             ಸ್ಥಿರಾಂಕ config_ರಾಶಿ_ಆಳ_TYPE us_ರಾಶಿ_ಆಳ,
                                                             ಶೂನ್ಯ * ಸ್ಥಿರಾಂಕ pvParameters,
                                                             UBaseType_t ux_ಆದ್ಯತೆ,
                                                             ಸ್ಥಿರಾಂಕ ತಳ_Type_t xCoreID,
                                                             UBaseType_t ux_ರಾಶಿ_ಸ್ಮರಣೆ_Caps,
                                                             TaskHandle_t * ಸ್ಥಿರಾಂಕ pxCreatedTask );
    #ಆದರೆ_ಅಂತ್ಯ // CONFIG_FREERTOS_SMP

#ಆದರೆ_ಅಂತ್ಯ // CONFIG_SPIRAM

/* *INDENT-OFF* */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus
    }
#ಆದರೆ_ಅಂತ್ಯ
/* *INDENT-ON* */
