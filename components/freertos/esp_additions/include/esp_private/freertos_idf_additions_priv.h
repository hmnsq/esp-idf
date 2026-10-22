/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#لارښوونه once

/*
 * This file is like "idf_additions.h" but for private API (i.e., only meant to
 * be called internally by other ESP-IDF components.
 */

#ضمن "sdkconfig.h"
#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"

/* *INDENT-OFF* */
#که_تعریف_شوی __cplusplus
    خارجي "C" {
#د_که_پای
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
#إذا ( !CONFIG_FREERTOS_SMP && ( config_عدد_CORES > 1 ) )

/* Macros that use a critical section when building for SMP */
    #تعریف_کړه prvENTER_CRITICAL_OR_SUSPEND_ALL( px_قلف )    taskENTER_CRITICAL( ( px_قلف ) )
    #تعریف_کړه prvEXIT_CRITICAL_OR_RESUME_ALL( px_قلف )      ( { taskEXIT_CRITICAL( ( px_قلف ) ); pdFALSE; } )
    #تعریف_کړه prv_ENTER_CRITICAL_OR_ماسک_ISR( px_قلف, ux_حالت ) \
    {                                                         \
        taskENTER_CRITICAL_ISR( ( px_قلف ) );                 \
        ( فراغ ) ( ux_حالت );                                \
    }
    #تعریف_کړه prvEXIT_CRITICAL_OR_UNMASK_ISR( px_قلف, ux_حالت ) \
    {                                                          \
        taskEXIT_CRITICAL_ISR( ( px_قلف ) );                   \
        ( فراغ ) ( ux_حالت );                                 \
    }

/* Macros that enter/exit a critical section only when building for SMP */
    #تعریف_کړه prvENTER_CRITICAL_SMP_ONLY( px_قلف )         taskENTER_CRITICAL( px_قلف )
    #تعریف_کړه prvEXIT_CRITICAL_SMP_ONLY( px_قلف )          taskEXIT_CRITICAL( px_قلف )
    #تعریف_کړه prvENTER_CRITICAL_ISR_SMP_ONLY( px_قلف )     taskENTER_CRITICAL_ISR( px_قلف )
    #تعریف_کړه prvEXIT_CRITICAL_ISR_SMP_ONLY( px_قلف )      taskEXIT_CRITICAL_ISR( px_قلف )
    #تعریف_کړه prv_ENTER_CRITICAL_خوندي_SMP_ONLY( px_قلف )    prv_Task_Enter_Critical_خوندي_SMPOnly( px_قلف )
    #تعریف_کړه prv_EXIT_CRITICAL_خوندي_SMP_ONLY( px_قلف )     prv_Task_Exit_Critical_خوندي_SMPOnly( px_قلف )

    ساكن مضمن __attribute__( ( always_inline ) )
    فراغ prv_Task_Enter_Critical_خوندي_SMPOnly( پورټ_MUX_TYPE * px_قلف )
    {
        إذا( پورټ_CHECK_IF_دننه_ISR() == pdFALSE )
        {
            taskENTER_CRITICAL( px_قلف );
        }
        وإلا
        {
            #که_تعریف_شوی __clang_analyzer__
                /* Teach clang-tidy that ISR version macro can be different */
                configASSERT( 1 );
            #د_که_پای
            taskENTER_CRITICAL_ISR( px_قلف );
        }
    }

    ساكن مضمن __attribute__( ( always_inline ) )
    فراغ prv_Task_Exit_Critical_خوندي_SMPOnly( پورټ_MUX_TYPE * px_قلف )
    {
        إذا( پورټ_CHECK_IF_دننه_ISR() == pdFALSE )
        {
            taskEXIT_CRITICAL( px_قلف );
        }
        وإلا
        {
            #که_تعریف_شوی __clang_analyzer__
                /* Teach clang-tidy that ISR version macro can be different */
                configASSERT( 1 );
            #د_که_پای
            taskEXIT_CRITICAL_ISR( px_قلف );
        }
    }

/* Macros that enter/exit a critical section only when building for single-core */
    #تعریف_کړه prvENTER_CRITICAL_SC_ONLY( px_قلف )
    #تعریف_کړه prvEXIT_CRITICAL_SC_ONLY( px_قلف )

/* Macros that enable/disable interrupts only when building for SMP */
    #تعریف_کړه prvDISABLE_INTERRUPTS_ISR_SMP_ONLY()             پورټ_سېټ_مداخله_ماسک_FROM_ISR()
    #تعریف_کړه prvENABLE_INTERRUPTS_ISR_SMP_ONLY( ux_حالت )    پورټ_CLEAR_مداخله_ماسک_FROM_ISR( ux_حالت )

#وإلاإذا ( !CONFIG_FREERTOS_SMP && ( config_عدد_CORES == 1 ) )

/* Macros that suspend the scheduler or disables interrupts when building for single-core */
    #تعریف_کړه prvENTER_CRITICAL_OR_SUSPEND_ALL( px_قلف )    ( { vTaskSuspendAll(); ( فراغ ) ( px_قلف ); } )
    #تعریف_کړه prvEXIT_CRITICAL_OR_RESUME_ALL( px_قلف )      xTaskResumeAll()
    #تعریف_کړه prv_ENTER_CRITICAL_OR_ماسک_ISR( px_قلف, ux_حالت ) \
    {                                                         \
        ( ux_حالت ) = پورټ_سېټ_مداخله_ماسک_FROM_ISR();     \
        ( فراغ ) ( px_قلف );                                  \
    }
    #تعریف_کړه prvEXIT_CRITICAL_OR_UNMASK_ISR( px_قلف, ux_حالت ) \
    {                                                          \
        پورټ_CLEAR_مداخله_ماسک_FROM_ISR( ( ux_حالت ) );     \
        ( فراغ ) ( px_قلف );                                   \
    }

/* Macros that enter/exit a critical section only when building for SMP */
#إذا !defined prvENTER_CRITICAL_SMP_ONLY
    #تعریف_کړه prvENTER_CRITICAL_SMP_ONLY( px_قلف )
#د_که_پای
#إذا !defined prvEXIT_CRITICAL_SMP_ONLY
    #تعریف_کړه prvEXIT_CRITICAL_SMP_ONLY( px_قلف )
#د_که_پای
#إذا !defined prvENTER_CRITICAL_ISR_SMP_ONLY
    #تعریف_کړه prvENTER_CRITICAL_ISR_SMP_ONLY( px_قلف )
#د_که_پای
#إذا !defined prvEXIT_CRITICAL_ISR_SMP_ONLY
    #تعریف_کړه prvEXIT_CRITICAL_ISR_SMP_ONLY( px_قلف )
#د_که_پای
#إذا !defined prv_ENTER_CRITICAL_خوندي_SMP_ONLY
    #تعریف_کړه prv_ENTER_CRITICAL_خوندي_SMP_ONLY( px_قلف )
#د_که_پای
#إذا !defined prv_EXIT_CRITICAL_خوندي_SMP_ONLY
    #تعریف_کړه prv_EXIT_CRITICAL_خوندي_SMP_ONLY( px_قلف )
#د_که_پای

/* Macros that enter/exit a critical section only when building for single-core */
    #تعریف_کړه prvENTER_CRITICAL_SC_ONLY( px_قلف )              taskENTER_CRITICAL( px_قلف )
    #تعریف_کړه prvEXIT_CRITICAL_SC_ONLY( px_قلف )               taskEXIT_CRITICAL( px_قلف )

/* Macros that enable/disable interrupts only when building for SMP */
    #تعریف_کړه prvDISABLE_INTERRUPTS_ISR_SMP_ONLY()             ( ( UBaseType_t ) 0 )
    #تعریف_کړه prvENABLE_INTERRUPTS_ISR_SMP_ONLY( ux_حالت )    ( ( فراغ ) ux_حالت )

#د_که_پای /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES == 1 ) ) */

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
#إذا ( !CONFIG_FREERTOS_SMP && ( config_عدد_CORES > 1 ) )

    فراغ prv_Take_Kernel_قلف( فراغ );
    فراغ prv_Release_Kernel_قلف( فراغ );

#د_که_پای /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */

#إذا ( CONFIG_FREERTOS_SMP && ( config_عدد_CORES > 1 ) )

/**
 * @brief Workaround for non-thread safe multi-core OS startup (see IDF-4524)
 *
 * This function must be called with interrupts disabled on all cores other than
 * core 0 during startup.
 */
    فراغ prv_پیل_Scheduler_Other_Cores( فراغ );

#د_که_پای /* ( CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */

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
#إذا ( !CONFIG_FREERTOS_SMP && ( config_عدد_CORES > 1 ) )

    بنسټ_Type_t xTaskIncrementTickOtherCores( فراغ );

#د_که_پای /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */

#إذا CONFIG_FREERTOS_SYSTICK_کاروي_SYSTIMER

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
    uint32_t x_پورټ_Sys_Tick_Claim_Elapsed_Ticks( صحيح cpu_id );

#د_که_پای /* CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER */

/*------------------------------------------------------------------------------
 * TASK UTILITIES (PRIVATE)
 *----------------------------------------------------------------------------*/
#إذا ( INCLUDE_v_Task_لومړیتوب_سېټ == 1 )

/**
 * @brief Structure to save a task's previous priority
 *
 * This structure is meant to be used with prvTaskPriorityRaise() and
 * prvTaskPriorityRestore().
 */
    تعريفنوع بنية
    {
        UBaseType_t ux_لومړیتوب;
        #إذا ( configUSE_MUTEXES == 1 )
            UBaseType_t ux_بنسټ_لومړیتوب;
        #د_که_پای
    } prv_Task_Saved_لومړیتوب_t;

#د_که_پای /* INCLUDE_vTaskPrioritySet == 1 */

#إذا ( INCLUDE_v_Task_لومړیتوب_سېټ == 1 )

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
    فراغ prv_Task_لومړیتوب_Raise( prv_Task_Saved_لومړیتوب_t * px_Saved_لومړیتوب,
                               UBaseType_t ux_New_لومړیتوب );

#د_که_پای /* INCLUDE_vTaskPrioritySet == 1 */

#إذا ( INCLUDE_v_Task_لومړیتوب_سېټ == 1 )

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
    فراغ prv_Task_لومړیتوب_Restore( prv_Task_Saved_لومړیتوب_t * px_Saved_لومړیتوب );

#د_که_پای /* INCLUDE_vTaskPrioritySet == 1 */

#إذا CONFIG_SPIRAM

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
    #إذا CONFIG_FREERTOS_SMP
        بنسټ_Type_t prv_Task_Create_Dynamic_Affinity_سېټ_With_Caps( TaskFunction_t px_Task_کوډ,
                                                            ثابت حرف * ثابت pcName,
                                                            ثابت config_ډېری_ژوروالی_TYPE us_ډېری_ژوروالی,
                                                            فراغ * ثابت pvParameters,
                                                            UBaseType_t ux_لومړیتوب,
                                                            UBaseType_t ux_Core_Affinity_ماسک,
                                                            UBaseType_t ux_ډېری_حافظه_Caps,
                                                            TaskHandle_t * ثابت pxCreatedTask );
    #وإلا
        بنسټ_Type_t prv_Task_Create_Dynamic_Pinned_منزل_Core_With_Caps( TaskFunction_t px_Task_کوډ,
                                                             ثابت حرف * ثابت pcName,
                                                             ثابت config_ډېری_ژوروالی_TYPE us_ډېری_ژوروالی,
                                                             فراغ * ثابت pvParameters,
                                                             UBaseType_t ux_لومړیتوب,
                                                             ثابت بنسټ_Type_t xCoreID,
                                                             UBaseType_t ux_ډېری_حافظه_Caps,
                                                             TaskHandle_t * ثابت pxCreatedTask );
    #د_که_پای // CONFIG_FREERTOS_SMP

#د_که_پای // CONFIG_SPIRAM

/* *INDENT-OFF* */
#که_تعریف_شوی __cplusplus
    }
#د_که_پای
/* *INDENT-ON* */
