/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#رهنمود once

/*
 * This file is like "idf_additions.h" but for private API (i.e., only meant to
 * be called internally by other ESP-IDF components.
 */

#ضمن "sdkconfig.h"
#ضمن "freertos/FreeRTOS.h"
#ضمن "freertos/task.h"

/* *INDENT-OFF* */
#اگر_تعریف_شده __cplusplus
    خارجي "C" {
#پایان_اگر
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
#إذا ( !CONFIG_FREERTOS_SMP && ( config_رقم_CORES > 1 ) )

/* Macros that use a critical section when building for SMP */
    #تعریف_کن prvENTER_CRITICAL_OR_SUSPEND_ALL( px_قفل )    taskENTER_CRITICAL( ( px_قفل ) )
    #تعریف_کن prvEXIT_CRITICAL_OR_RESUME_ALL( px_قفل )      ( { taskEXIT_CRITICAL( ( px_قفل ) ); pdFALSE; } )
    #تعریف_کن prv_ENTER_CRITICAL_OR_ماسک_ISR( px_قفل, ux_وضعیت ) \
    {                                                         \
        taskENTER_CRITICAL_ISR( ( px_قفل ) );                 \
        ( فراغ ) ( ux_وضعیت );                                \
    }
    #تعریف_کن prvEXIT_CRITICAL_OR_UNMASK_ISR( px_قفل, ux_وضعیت ) \
    {                                                          \
        taskEXIT_CRITICAL_ISR( ( px_قفل ) );                   \
        ( فراغ ) ( ux_وضعیت );                                 \
    }

/* Macros that enter/exit a critical section only when building for SMP */
    #تعریف_کن prvENTER_CRITICAL_SMP_ONLY( px_قفل )         taskENTER_CRITICAL( px_قفل )
    #تعریف_کن prvEXIT_CRITICAL_SMP_ONLY( px_قفل )          taskEXIT_CRITICAL( px_قفل )
    #تعریف_کن prvENTER_CRITICAL_ISR_SMP_ONLY( px_قفل )     taskENTER_CRITICAL_ISR( px_قفل )
    #تعریف_کن prvEXIT_CRITICAL_ISR_SMP_ONLY( px_قفل )      taskEXIT_CRITICAL_ISR( px_قفل )
    #تعریف_کن prv_ENTER_CRITICAL_امن_SMP_ONLY( px_قفل )    prv_Task_Enter_Critical_امن_SMPOnly( px_قفل )
    #تعریف_کن prv_EXIT_CRITICAL_امن_SMP_ONLY( px_قفل )     prv_Task_Exit_Critical_امن_SMPOnly( px_قفل )

    ساكن مضمن __attribute__( ( always_inline ) )
    فراغ prv_Task_Enter_Critical_امن_SMPOnly( پورت_MUX_TYPE * px_قفل )
    {
        إذا( پورت_CHECK_IF_درون_ISR() == pdFALSE )
        {
            taskENTER_CRITICAL( px_قفل );
        }
        وإلا
        {
            #اگر_تعریف_شده __clang_analyzer__
                /* Teach clang-tidy that ISR version macro can be different */
                configASSERT( 1 );
            #پایان_اگر
            taskENTER_CRITICAL_ISR( px_قفل );
        }
    }

    ساكن مضمن __attribute__( ( always_inline ) )
    فراغ prv_Task_Exit_Critical_امن_SMPOnly( پورت_MUX_TYPE * px_قفل )
    {
        إذا( پورت_CHECK_IF_درون_ISR() == pdFALSE )
        {
            taskEXIT_CRITICAL( px_قفل );
        }
        وإلا
        {
            #اگر_تعریف_شده __clang_analyzer__
                /* Teach clang-tidy that ISR version macro can be different */
                configASSERT( 1 );
            #پایان_اگر
            taskEXIT_CRITICAL_ISR( px_قفل );
        }
    }

/* Macros that enter/exit a critical section only when building for single-core */
    #تعریف_کن prvENTER_CRITICAL_SC_ONLY( px_قفل )
    #تعریف_کن prvEXIT_CRITICAL_SC_ONLY( px_قفل )

/* Macros that enable/disable interrupts only when building for SMP */
    #تعریف_کن prvDISABLE_INTERRUPTS_ISR_SMP_ONLY()             پورت_مجموعه_وقفه_ماسک_FROM_ISR()
    #تعریف_کن prvENABLE_INTERRUPTS_ISR_SMP_ONLY( ux_وضعیت )    پورت_CLEAR_وقفه_ماسک_FROM_ISR( ux_وضعیت )

#وإلاإذا ( !CONFIG_FREERTOS_SMP && ( config_رقم_CORES == 1 ) )

/* Macros that suspend the scheduler or disables interrupts when building for single-core */
    #تعریف_کن prvENTER_CRITICAL_OR_SUSPEND_ALL( px_قفل )    ( { vTaskSuspendAll(); ( فراغ ) ( px_قفل ); } )
    #تعریف_کن prvEXIT_CRITICAL_OR_RESUME_ALL( px_قفل )      xTaskResumeAll()
    #تعریف_کن prv_ENTER_CRITICAL_OR_ماسک_ISR( px_قفل, ux_وضعیت ) \
    {                                                         \
        ( ux_وضعیت ) = پورت_مجموعه_وقفه_ماسک_FROM_ISR();     \
        ( فراغ ) ( px_قفل );                                  \
    }
    #تعریف_کن prvEXIT_CRITICAL_OR_UNMASK_ISR( px_قفل, ux_وضعیت ) \
    {                                                          \
        پورت_CLEAR_وقفه_ماسک_FROM_ISR( ( ux_وضعیت ) );     \
        ( فراغ ) ( px_قفل );                                   \
    }

/* Macros that enter/exit a critical section only when building for SMP */
#إذا !defined prvENTER_CRITICAL_SMP_ONLY
    #تعریف_کن prvENTER_CRITICAL_SMP_ONLY( px_قفل )
#پایان_اگر
#إذا !defined prvEXIT_CRITICAL_SMP_ONLY
    #تعریف_کن prvEXIT_CRITICAL_SMP_ONLY( px_قفل )
#پایان_اگر
#إذا !defined prvENTER_CRITICAL_ISR_SMP_ONLY
    #تعریف_کن prvENTER_CRITICAL_ISR_SMP_ONLY( px_قفل )
#پایان_اگر
#إذا !defined prvEXIT_CRITICAL_ISR_SMP_ONLY
    #تعریف_کن prvEXIT_CRITICAL_ISR_SMP_ONLY( px_قفل )
#پایان_اگر
#إذا !defined prv_ENTER_CRITICAL_امن_SMP_ONLY
    #تعریف_کن prv_ENTER_CRITICAL_امن_SMP_ONLY( px_قفل )
#پایان_اگر
#إذا !defined prv_EXIT_CRITICAL_امن_SMP_ONLY
    #تعریف_کن prv_EXIT_CRITICAL_امن_SMP_ONLY( px_قفل )
#پایان_اگر

/* Macros that enter/exit a critical section only when building for single-core */
    #تعریف_کن prvENTER_CRITICAL_SC_ONLY( px_قفل )              taskENTER_CRITICAL( px_قفل )
    #تعریف_کن prvEXIT_CRITICAL_SC_ONLY( px_قفل )               taskEXIT_CRITICAL( px_قفل )

/* Macros that enable/disable interrupts only when building for SMP */
    #تعریف_کن prvDISABLE_INTERRUPTS_ISR_SMP_ONLY()             ( ( UBaseType_t ) 0 )
    #تعریف_کن prvENABLE_INTERRUPTS_ISR_SMP_ONLY( ux_وضعیت )    ( ( فراغ ) ux_وضعیت )

#پایان_اگر /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES == 1 ) ) */

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
#إذا ( !CONFIG_FREERTOS_SMP && ( config_رقم_CORES > 1 ) )

    فراغ prv_Take_Kernel_قفل( فراغ );
    فراغ prv_Release_Kernel_قفل( فراغ );

#پایان_اگر /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */

#إذا ( CONFIG_FREERTOS_SMP && ( config_رقم_CORES > 1 ) )

/**
 * @brief Workaround for non-thread safe multi-core OS startup (see IDF-4524)
 *
 * This function must be called with interrupts disabled on all cores other than
 * core 0 during startup.
 */
    فراغ prv_شروع_Scheduler_Other_Cores( فراغ );

#پایان_اگر /* ( CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */

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
#إذا ( !CONFIG_FREERTOS_SMP && ( config_رقم_CORES > 1 ) )

    پایه_Type_t xTaskIncrementTickOtherCores( فراغ );

#پایان_اگر /* ( !CONFIG_FREERTOS_SMP && ( configNUM_CORES > 1 ) ) */

#إذا CONFIG_FREERTOS_SYSTICK_استفاده_میکند_SYSTIMER

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
    uint32_t x_پورت_Sys_Tick_Claim_Elapsed_Ticks( صحيح cpu_id );

#پایان_اگر /* CONFIG_FREERTOS_SYSTICK_USES_SYSTIMER */

/*------------------------------------------------------------------------------
 * TASK UTILITIES (PRIVATE)
 *----------------------------------------------------------------------------*/
#إذا ( INCLUDE_v_Task_اولویت_مجموعه == 1 )

/**
 * @brief Structure to save a task's previous priority
 *
 * This structure is meant to be used with prvTaskPriorityRaise() and
 * prvTaskPriorityRestore().
 */
    تعريفنوع بنية
    {
        UBaseType_t ux_اولویت;
        #إذا ( configUSE_MUTEXES == 1 )
            UBaseType_t ux_پایه_اولویت;
        #پایان_اگر
    } prv_Task_Saved_اولویت_t;

#پایان_اگر /* INCLUDE_vTaskPrioritySet == 1 */

#إذا ( INCLUDE_v_Task_اولویت_مجموعه == 1 )

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
    فراغ prv_Task_اولویت_Raise( prv_Task_Saved_اولویت_t * px_Saved_اولویت,
                               UBaseType_t ux_New_اولویت );

#پایان_اگر /* INCLUDE_vTaskPrioritySet == 1 */

#إذا ( INCLUDE_v_Task_اولویت_مجموعه == 1 )

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
    فراغ prv_Task_اولویت_Restore( prv_Task_Saved_اولویت_t * px_Saved_اولویت );

#پایان_اگر /* INCLUDE_vTaskPrioritySet == 1 */

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
        پایه_Type_t prv_Task_Create_Dynamic_Affinity_مجموعه_With_Caps( TaskFunction_t px_Task_کد,
                                                            ثابت حرف * ثابت pcName,
                                                            ثابت config_پشته_عمق_TYPE us_پشته_عمق,
                                                            فراغ * ثابت pvParameters,
                                                            UBaseType_t ux_اولویت,
                                                            UBaseType_t ux_Core_Affinity_ماسک,
                                                            UBaseType_t ux_پشته_حافظه_Caps,
                                                            TaskHandle_t * ثابت pxCreatedTask );
    #وإلا
        پایه_Type_t prv_Task_Create_Dynamic_Pinned_مقصد_Core_With_Caps( TaskFunction_t px_Task_کد,
                                                             ثابت حرف * ثابت pcName,
                                                             ثابت config_پشته_عمق_TYPE us_پشته_عمق,
                                                             فراغ * ثابت pvParameters,
                                                             UBaseType_t ux_اولویت,
                                                             ثابت پایه_Type_t xCoreID,
                                                             UBaseType_t ux_پشته_حافظه_Caps,
                                                             TaskHandle_t * ثابت pxCreatedTask );
    #پایان_اگر // CONFIG_FREERTOS_SMP

#پایان_اگر // CONFIG_SPIRAM

/* *INDENT-OFF* */
#اگر_تعریف_شده __cplusplus
    }
#پایان_اگر
/* *INDENT-ON* */
