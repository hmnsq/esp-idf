/*
 * SPDX-FileCopyrightText: 2023-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#సూచన once

/*
 * This file contains the function prototypes of ESP-IDF specific API additions
 * to the FreeRTOS kernel. These API additions are not part of Vanilla (i.e.,
 * upstream) FreeRTOS and include things such as....
 *    - Various helper functions
 *    - API for ESP-IDF feature additions to FreeRTOS (such as TSLP deletion
 * call backs)
 */

#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"
#అన్తర్భావ "freertos/queue.h"
#అన్తర్భావ "freertos/semphr.h"
#అన్తర్భావ "freertos/stream_buffer.h"
#అన్తర్భావ "freertos/message_buffer.h"
#అన్తర్భావ "freertos/event_groups.h"
#అన్తర్భావ "esp_heap_caps.h"

/* *INDENT-OFF* */
#నిర్వచితమైతే __cplusplus
    బాహ్య "C" {
#అయితే_ముగింపు
/* *INDENT-ON* */


/* -------------------------------------------------- Task Creation ------------------------------------------------- */

#ఒకవేళ ( config_SUPPORT_DYNAMIC_కేటాయింపు == 1 )

/**
 * @brief Create a new task that is pinned to a particular core
 *
 * This function is similar to xTaskCreate(), but allows the creation of a pinned
 * task. The task's pinned core is specified by the xCoreID argument. If xCoreID
 * is set to tskNO_AFFINITY, then the task is unpinned and can run on any core.
 *
 * @note If ( configNUMBER_OF_CORES == 1 ), setting xCoreID to tskNO_AFFINITY will be
 * be treated as 0.
 *
 * @param pxTaskCode Pointer to the task entry function.
 * @param pcName A descriptive name for the task.
 * @param ulStackDepth The size of the task stack specified as the NUMBER OF
 * BYTES. Note that this differs from vanilla FreeRTOS.
 * @param pvParameters Pointer that will be used as the parameter for the task
 * being created.
 * @param uxPriority The priority at which the task should run.
 * @param pxCreatedTask Used to pass back a handle by which the created task can
 * be referenced.
 * @param xCoreID The core to which the task is pinned to, or tskNO_AFFINITY if
 * the task has no core affinity.
 * @return pdPASS if the task was successfully created and added to a ready
 * list, otherwise an error code defined in the file projdefs.h
 */
    పునాది_Type_t x_Task_Create_Pinned_గమ్యం_Core( TaskFunction_t px_Task_సంకేతం,
                                        స్థిరరాశి అక్షరం * స్థిరరాశి pcName,
                                        స్థిరరాశి uint32_t ul_దొంతర_లోతు,
                                        శూన్య * స్థిరరాశి pvParameters,
                                        UBaseType_t ux_ప్రాధాన్యత,
                                        TaskHandle_t * స్థిరరాశి pxCreatedTask,
                                        స్థిరరాశి పునాది_Type_t xCoreID );

#అయితే_ముగింపు /* configSUPPORT_DYNAMIC_ALLOCATION */

#ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )

/**
 * @brief Create a new static task that is pinned to a particular core
 *
 * This function is similar to xTaskCreateStatic(), but allows the creation of a
 * pinned task. The task's pinned core is specified by the xCoreID argument. If
 * xCoreID is set to tskNO_AFFINITY, then the task is unpinned and can run on any
 * core.
 *
 * @note If ( configNUMBER_OF_CORES == 1 ), setting xCoreID to tskNO_AFFINITY will be
 * be treated as 0.
 *
 * @param pxTaskCode Pointer to the task entry function.
 * @param pcName A descriptive name for the task.
 * @param ulStackDepth The size of the task stack specified as the NUMBER OF
 * BYTES. Note that this differs from vanilla FreeRTOS.
 * @param pvParameters Pointer that will be used as the parameter for the task
 * being created.
 * @param uxPriority The priority at which the task should run.
 * @param puxStackBuffer Must point to a StackType_t array that has at least
 * ulStackDepth indexes
 * @param pxTaskBuffer Must point to a variable of type StaticTask_t, which will
 * then be used to hold the task's data structures,
 * @param xCoreID The core to which the task is pinned to, or tskNO_AFFINITY if
 * the task has no core affinity.
 * @return The task handle if the task was created, NULL otherwise.
 */
    TaskHandle_t x_Task_Create_Static_Pinned_గమ్యం_Core( TaskFunction_t px_Task_సంకేతం,
                                                స్థిరరాశి అక్షరం * స్థిరరాశి pcName,
                                                స్థిరరాశి uint32_t ul_దొంతర_లోతు,
                                                శూన్య * స్థిరరాశి pvParameters,
                                                UBaseType_t ux_ప్రాధాన్యత,
                                                దొంతర_Type_t * స్థిరరాశి pux_దొంతర_బఫరు,
                                                StaticTask_t * స్థిరరాశి px_Task_బఫరు,
                                                స్థిరరాశి పునాది_Type_t xCoreID );

#అయితే_ముగింపు /* configSUPPORT_STATIC_ALLOCATION */

/* ------------------------------------------------- Task Utilities ------------------------------------------------- */

/**
 * @brief Get the current core ID of a particular task
 *
 * Helper function to get the core ID of a particular task. If the task is
 * pinned to a particular core, the core ID is returned. If the task is not
 * pinned to a particular core, tskNO_AFFINITY is returned.
 *
 * If CONFIG_FREERTOS_UNICORE is enabled, this function simply returns 0.
 *
 * [refactor-todo] See if this needs to be deprecated (IDF-8145)
 *
 * @note If CONFIG_FREERTOS_SMP is enabled, please call vTaskCoreAffinityGet()
 * instead.
 * @note In IDF FreerTOS when configNUMBER_OF_CORES == 1, this function will
 * always return 0,
 * @param xTask The task to query
 * @return The task's core ID or tskNO_AFFINITY
 */
పునాది_Type_t xTaskGetCoreID( TaskHandle_t xTask );

#ఒకవేళ CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST

/**
 * @brief Mark a task as forbidden from using the PIE coprocessor
 *
 * When CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST is enabled, a blacklisted
 * task aborts if it executes a PIE instruction instead of being silently
 * pinned to a PIE-capable core. Use this to find accidental PIE use.
 *
 * The flag is consulted only inside the coprocessor trap. PIE stays enabled
 * while its owner keeps running, so instructions issued before that task is
 * switched out do not trap. After the task is switched out, PIE is disabled
 * and the next PIE instruction traps. A blacklisted task aborts on that trap
 * even if it is still the coprocessor owner.
 *
 * @param xTask Task to update. NULL selects the calling task.
 * @param xBlacklisted pdTRUE to blacklist the task, pdFALSE to clear it.
 */
శూన్య v_Task_సమితి_Pie_Blacklisted( TaskHandle_t xTask,
                             పునాది_Type_t xBlacklisted );

/**
 * @brief Query whether a task is forbidden from using the PIE coprocessor
 *
 * @param xTask Task to query. NULL selects the calling task.
 * @return pdTRUE if the task is blacklisted, pdFALSE otherwise.
 */
పునాది_Type_t xTaskGetPieBlacklisted( TaskHandle_t xTask );

#అయితే_ముగింపు /* CONFIG_FREERTOS_DEBUG_TASK_PIE_BLACKLIST */

#ఒకవేళ ( ( !CONFIG_FREERTOS_SMP ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

/**
 * @brief Get the handle of idle task for the given core.
 *
 * [refactor-todo] See if this needs to be deprecated (IDF-8145)
 *
 * @param xCoreID The core to query
 * @return Handle of the idle task for the queried core
 */
    TaskHandle_t xTaskGetIdleTaskHandleForCore( పునాది_Type_t xCoreID );

#అయితే_ముగింపు /* ( ( !CONFIG_FREERTOS_SMP ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */

#ఒకవేళ ( ( !CONFIG_FREERTOS_SMP ) && ( ( INCLUDE_xTaskGetCurrentTaskHandle == 1 ) || ( configUSE_MUTEXES == 1 ) ) )

/**
 * @brief Get the handle of the task currently running on a certain core
 *
 * Because of the nature of SMP processing, there is no guarantee that this
 * value will still be valid on return and should only be used for debugging
 * purposes.
 *
 * [refactor-todo] See if this needs to be deprecated (IDF-8145)
 *
 * @param xCoreID The core to query
 * @return Handle of the current task running on the queried core
 */
    TaskHandle_t xTaskGetCurrentTaskHandleForCore( పునాది_Type_t xCoreID );

#అయితే_ముగింపు /* ( ( !CONFIG_FREERTOS_SMP ) && ( ( INCLUDE_xTaskGetCurrentTaskHandle == 1 ) || ( configUSE_MUTEXES == 1 ) ) ) */

#ఒకవేళ ( !CONFIG_FREERTOS_SMP && ( config_GENERATE_నిరంతరం_సమయం_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) )

/**
 * @brief Get the total execution of a particular core's idle task
 *
 * This function is equivalent to ulTaskGetIdleRunTimeCounter() but queries the
 * idle task of a particular core.
 *
 * @param xCoreID Core ID of the idle task to query
 * @return The total run time of the idle task
 */
    config_నిరంతరం_సమయం_COUNTER_TYPE ul_Task_Get_Idle_నిరంతరం_సమయం_Counter_For_Core( పునాది_Type_t xCoreID );

/**
 * @brief Get the percentage run time of a particular core's idle task
 *
 * This function is equivalent to ulTaskGetIdleRunTimePercent() but queries the
 * idle task of a particular core.
 *
 * @param xCoreID Core ID of the idle task to query
 * @return The percentage run time of the idle task
 */
    config_నిరంతరం_సమయం_COUNTER_TYPE ul_Task_Get_Idle_నిరంతరం_సమయం_Percent_For_Core( పునాది_Type_t xCoreID );

#అయితే_ముగింపు /* ( !CONFIG_FREERTOS_SMP && ( configGENERATE_RUN_TIME_STATS == 1 ) && ( INCLUDE_xTaskGetIdleTaskHandle == 1 ) ) */

/**
 * Returns the start of the stack associated with xTask.
 *
 * Returns the lowest stack memory address, regardless of whether the stack
 * grows up or down.
 *
 * @param xTask Handle of the task associated with the stack returned.
 * Set xTask to NULL to return the stack of the calling task.
 *
 * @return A pointer to the start of the stack.
 */
దొంతర_Type_t * x_Task_Get_దొంతర_ప్రారంభం( TaskHandle_t xTask );

/* --------------------------------------------- TLSP Deletion Callbacks -------------------------------------------- */

#ఒకవేళ CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS

/**
 * Prototype of local storage pointer deletion callback.
 */
    రకనిర్వచనం శూన్య (* TlsDeleteCallbackFunction_t)( పూర్ణాంకం,
                                                  శూన్య * );
#అయితే_ముగింపు /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

#ఒకవేళ CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS

/**
 * Set local storage pointer and deletion callback.
 *
 * Each task contains an array of pointers that is dimensioned by the
 * configNUM_THREAD_LOCAL_STORAGE_POINTERS setting in FreeRTOSConfig.h. The
 * kernel does not use the pointers itself, so the application writer can use
 * the pointers for any purpose they wish.
 *
 * Local storage pointers set for a task can reference dynamically allocated
 * resources. This function is similar to vTaskSetThreadLocalStoragePointer, but
 * provides a way to release these resources when the task gets deleted. For
 * each pointer, a callback function can be set. This function will be called
 * when task is deleted, with the local storage pointer index and value as
 * arguments.
 *
 * @param xTaskToSet  Task to set thread local storage pointer for
 * @param xIndex The index of the pointer to set, from 0 to
 * configNUM_THREAD_LOCAL_STORAGE_POINTERS - 1.
 * @param pvValue Pointer value to set.
 * @param pvDelCallback  Function to call to dispose of the local storage
 * pointer when the task is deleted.
 */
    శూన్య v_Task_సమితి_Thread_Local_Storage_సూచిక_And_Del_Callback( TaskHandle_t x_Task_గమ్యం_సమితి,
                                                          పునాది_Type_t x_సూచీ,
                                                          శూన్య * pv_విలువ,
                                                          TlsDeleteCallbackFunction_t pvDelCallback );

#అయితే_ముగింపు /* CONFIG_FREERTOS_TLSP_DELETION_CALLBACKS */

/* -------------------------------------------- Creation With Memory Caps ----------------------------------------------
 * Helper functions to create various FreeRTOS objects (e.g., queues, semaphores) with specific memory capabilities
 * (e.g., MALLOC_CAP_INTERNAL).
 * ------------------------------------------------------------------------------------------------------------------ */

/* ---------------------------------- Tasks --------------------------------- */

#ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )

/**
 * @brief Creates a pinned task where its stack has specific memory capabilities
 *
 * This function is similar to xTaskCreatePinnedToCore(), except that it allows
 * the memory allocated for the task's stack to have specific capabilities
 * (e.g., MALLOC_CAP_SPIRAM).
 *
 * However, the specified capabilities will NOT apply to the task's TCB as a TCB
 * must always be in internal RAM.
 *
 * @param pvTaskCode Pointer to the task entry function
 * @param pcName A descriptive name for the task
 * @param usStackDepth The size of the task stack specified as the number of
 * bytes
 * @param pvParameters Pointer that will be used as the parameter for the task
 * being created.
 * @param uxPriority The priority at which the task should run.
 * @param pvCreatedTask Used to pass back a handle by which the created task can
 * be referenced.
 * @param xCoreID Core to which the task is pinned to, or tskNO_AFFINITY if
 * unpinned.
 * @param uxMemoryCaps Memory capabilities of the task stack's memory (see
 * esp_heap_caps.h)
 * @return pdPASS if the task was successfully created and added to a ready
 * list, otherwise an error code defined in the file projdefs.h
 */
    పునాది_Type_t x_Task_Create_Pinned_గమ్యం_Core_With_Caps( TaskFunction_t pv_Task_సంకేతం,
                                                స్థిరరాశి అక్షరం * స్థిరరాశి pcName,
                                                స్థిరరాశి config_దొంతర_లోతు_TYPE us_దొంతర_లోతు,
                                                శూన్య * స్థిరరాశి pvParameters,
                                                UBaseType_t ux_ప్రాధాన్యత,
                                                TaskHandle_t * స్థిరరాశి pvCreatedTask,
                                                స్థిరరాశి పునాది_Type_t xCoreID,
                                                UBaseType_t ux_జ్ఞాపకం_Caps );

#అయితే_ముగింపు /* configSUPPORT_STATIC_ALLOCATION == 1 */

#ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )

/**
 * @brief Creates a task where its stack has specific memory capabilities
 *
 * This function is similar to xTaskCreate(), except that it allows the memory
 * allocated for the task's stack to have specific capabilities (e.g.,
 * MALLOC_CAP_SPIRAM).
 *
 * However, the specified capabilities will NOT apply to the task's TCB as a TCB
 * must always be in internal RAM.
 *
 * @note A task created using this function must only be deleted using
 * vTaskDeleteWithCaps()
 * @param pvTaskCode Pointer to the task entry function
 * @param pcName A descriptive name for the task
 * @param usStackDepth The size of the task stack specified as the number of
 * bytes
 * @param pvParameters Pointer that will be used as the parameter for the task
 * being created.
 * @param uxPriority The priority at which the task should run.
 * @param pvCreatedTask Used to pass back a handle by which the created task can
 * be referenced.
 * @param uxMemoryCaps Memory capabilities of the task stack's memory (see
 * esp_heap_caps.h)
 * @return pdPASS if the task was successfully created and added to a ready
 * list, otherwise an error code defined in the file projdefs.h
 */
    స్థిర అంతఃవరుస పునాది_Type_t xTaskCreateWithCaps( TaskFunction_t pv_Task_సంకేతం,
                                                  స్థిరరాశి అక్షరం * స్థిరరాశి pcName,
                                                  config_దొంతర_లోతు_TYPE us_దొంతర_లోతు,
                                                  శూన్య * స్థిరరాశి pvParameters,
                                                  UBaseType_t ux_ప్రాధాన్యత,
                                                  TaskHandle_t * pvCreatedTask,
                                                  UBaseType_t ux_జ్ఞాపకం_Caps )
    {
        తిరిగి x_Task_Create_Pinned_గమ్యం_Core_With_Caps( pv_Task_సంకేతం, pcName, us_దొంతర_లోతు, pvParameters, ux_ప్రాధాన్యత, pvCreatedTask, tskNO_AFFINITY, ux_జ్ఞాపకం_Caps );
    }

#అయితే_ముగింపు /* configSUPPORT_STATIC_ALLOCATION == 1 */

#ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )

/**
 * @brief Deletes a task previously created using xTaskCreateWithCaps() or
 * xTaskCreatePinnedToCoreWithCaps()
 *
 * @note It is recommended to use this API to delete tasks from another task's
 * context, rather than self-deletion. When the task is being deleted, it is vital
 * to ensure that it is not running on another core. This API must not be called
 * from an interrupt context.
 *
 * @param xTaskToDelete A handle to the task to be deleted
 */
    శూన్య vTaskDeleteWithCaps( TaskHandle_t x_Task_గమ్యం_Delete );

#అయితే_ముగింపు /* configSUPPORT_STATIC_ALLOCATION == 1 */

/* ---------------------------------- Queue --------------------------------- */

#ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )

/**
 * @brief Creates a queue with specific memory capabilities
 *
 * This function is similar to xQueueCreate(), except that it allows the memory
 * allocated for the queue to have specific capabilities (e.g.,
 * MALLOC_CAP_INTERNAL).
 *
 * @note A queue created using this function must only be deleted using
 * vQueueDeleteWithCaps()
 * @param uxQueueLength The maximum number of items that the queue can contain.
 * @param uxItemSize The number of bytes each item in the queue will require.
 * @param uxMemoryCaps Memory capabilities of the queue's memory (see
 * esp_heap_caps.h)
 * @return Handle to the created queue or NULL on failure.
 */
    వరుస_Handle_t x_వరుస_Create_With_Caps( UBaseType_t ux_వరుస_నిడివి,
                                        UBaseType_t ux_అంశం_కొలత,
                                        UBaseType_t ux_జ్ఞాపకం_Caps );

#అయితే_ముగింపు /* configSUPPORT_STATIC_ALLOCATION == 1 */

#ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )

/**
 * @brief Deletes a queue previously created using xQueueCreateWithCaps()
 *
 * @param xQueue A handle to the queue to be deleted.
 */
    శూన్య v_వరుస_Delete_With_Caps( వరుస_Handle_t x_వరుస );

#అయితే_ముగింపు /* configSUPPORT_STATIC_ALLOCATION == 1 */

/* -------------------------------- Semaphore ------------------------------- */

/** @cond */ /* Doxygen command to hide this from docs */
SemaphoreHandle_t xSemaphoreCreateGenericWithCaps( UBaseType_t ux_Max_లెక్క,
                                                   UBaseType_t ux_Initial_లెక్క,
                                                   స్థిరరాశి uint8_t uc_వరుస_Type,
                                                   UBaseType_t ux_జ్ఞాపకం_Caps );
/** @endcond */

#ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )

/**
 * @brief Creates a binary semaphore with specific memory capabilities
 *
 * This function is similar to vSemaphoreCreateBinary(), except that it allows
 * the memory allocated for the binary semaphore to have specific capabilities
 * (e.g., MALLOC_CAP_INTERNAL).
 *
 * @note A binary semaphore created using this function must only be deleted
 * using vSemaphoreDeleteWithCaps()
 * @param uxMemoryCaps Memory capabilities of the binary semaphore's memory (see
 * esp_heap_caps.h)
 * @return Handle to the created binary semaphore or NULL on failure.
 */
    స్థిర అంతఃవరుస SemaphoreHandle_t x_Semaphore_Create_ద్విమాన_With_Caps( UBaseType_t ux_జ్ఞాపకం_Caps )
    {
        తిరిగి xSemaphoreCreateGenericWithCaps( 0, 0, వరుస_వరుస_TYPE_ద్విమాన_SEMAPHORE, ux_జ్ఞాపకం_Caps );
    }

#అయితే_ముగింపు /* configSUPPORT_STATIC_ALLOCATION == 1 */

#ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )

/**
 * @brief Creates a counting semaphore with specific memory capabilities
 *
 * This function is similar to xSemaphoreCreateCounting(), except that it allows
 * the memory allocated for the counting semaphore to have specific capabilities
 * (e.g., MALLOC_CAP_INTERNAL).
 *
 * @note A counting semaphore created using this function must only be deleted
 * using vSemaphoreDeleteWithCaps()
 * @param uxMaxCount The maximum count value that can be reached.
 * @param uxInitialCount The count value assigned to the semaphore when it is
 * created.
 * @param uxMemoryCaps Memory capabilities of the counting semaphore's memory
 * (see esp_heap_caps.h)
 * @return Handle to the created counting semaphore or NULL on failure.
 */
    స్థిర అంతఃవరుస SemaphoreHandle_t xSemaphoreCreateCountingWithCaps( UBaseType_t ux_Max_లెక్క,
                                                                      UBaseType_t ux_Initial_లెక్క,
                                                                      UBaseType_t ux_జ్ఞాపకం_Caps )
    {
        తిరిగి xSemaphoreCreateGenericWithCaps( ux_Max_లెక్క, ux_Initial_లెక్క, వరుస_వరుస_TYPE_COUNTING_SEMAPHORE, ux_జ్ఞాపకం_Caps );
    }

#అయితే_ముగింపు /* configSUPPORT_STATIC_ALLOCATION == 1 */

#ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )

/**
 * @brief Creates a mutex semaphore with specific memory capabilities
 *
 * This function is similar to xSemaphoreCreateMutex(), except that it allows
 * the memory allocated for the mutex semaphore to have specific capabilities
 * (e.g., MALLOC_CAP_INTERNAL).
 *
 * @note A mutex semaphore created using this function must only be deleted
 * using vSemaphoreDeleteWithCaps()
 * @param uxMemoryCaps Memory capabilities of the mutex semaphore's memory (see
 * esp_heap_caps.h)
 * @return Handle to the created mutex semaphore or NULL on failure.
 */
    స్థిర అంతఃవరుస SemaphoreHandle_t xSemaphoreCreateMutexWithCaps( UBaseType_t ux_జ్ఞాపకం_Caps )
    {
        తిరిగి xSemaphoreCreateGenericWithCaps( 0, 0, వరుస_వరుస_TYPE_MUTEX, ux_జ్ఞాపకం_Caps );
    }

#అయితే_ముగింపు /* configSUPPORT_STATIC_ALLOCATION == 1 */

#ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )

/**
 * @brief Creates a recursive mutex with specific memory capabilities
 *
 * This function is similar to xSemaphoreCreateRecursiveMutex(), except that it
 * allows the memory allocated for the recursive mutex to have specific
 * capabilities (e.g., MALLOC_CAP_INTERNAL).
 *
 * @note A recursive mutex created using this function must only be deleted
 * using vSemaphoreDeleteWithCaps()
 * @param uxMemoryCaps Memory capabilities of the recursive mutex's memory (see
 * esp_heap_caps.h)
 * @return Handle to the created recursive mutex or NULL on failure.
 */
    స్థిర అంతఃవరుస SemaphoreHandle_t xSemaphoreCreateRecursiveMutexWithCaps( UBaseType_t ux_జ్ఞాపకం_Caps )
    {
        తిరిగి xSemaphoreCreateGenericWithCaps( 0, 0, వరుస_వరుస_TYPE_RECURSIVE_MUTEX, ux_జ్ఞాపకం_Caps );
    }

#అయితే_ముగింపు /* configSUPPORT_STATIC_ALLOCATION == 1 */

#ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )

/**
 * @brief Deletes a semaphore previously created using one of the
 * xSemaphoreCreate...WithCaps() functions
 *
 * @param xSemaphore A handle to the semaphore to be deleted.
 */
    శూన్య vSemaphoreDeleteWithCaps( SemaphoreHandle_t xSemaphore );

#అయితే_ముగింపు /* configSUPPORT_STATIC_ALLOCATION == 1 */

/* ------------------------ Stream & Message Buffers ------------------------ */

/** @cond */ /* Doxygen command to hide this from docs */
ప్రవాహం_బఫరు_Handle_t x_ప్రవాహం_బఫరు_Generic_Create_With_Caps( size_t x_బఫరు_కొలత_బైట్లు,
                                                         size_t x_Trigger_Level_బైట్లు,
                                                         పునాది_Type_t x_Is_సందేశం_బఫరు,
                                                         UBaseType_t ux_జ్ఞాపకం_Caps );

శూన్య v_ప్రవాహం_బఫరు_Generic_Delete_With_Caps( ప్రవాహం_బఫరు_Handle_t x_ప్రవాహం_బఫరు,
                                         పునాది_Type_t x_Is_సందేశం_బఫరు );
/** @endcond */

#ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )

/**
 * @brief Creates a stream buffer with specific memory capabilities
 *
 * This function is similar to xStreamBufferCreate(), except that it allows the
 * memory allocated for the stream buffer to have specific capabilities (e.g.,
 * MALLOC_CAP_INTERNAL).
 *
 * @note A stream buffer created using this function must only be deleted using
 * vStreamBufferDeleteWithCaps()
 * @param xBufferSizeBytes The total number of bytes the stream buffer will be
 * able to hold at any one time.
 * @param xTriggerLevelBytes The number of bytes that must be in the stream
 * buffer before unblocking
 * @param uxMemoryCaps Memory capabilities of the stream buffer's memory (see
 * esp_heap_caps.h)
 * @return Handle to the created stream buffer or NULL on failure.
 */
    స్థిర అంతఃవరుస ప్రవాహం_బఫరు_Handle_t x_ప్రవాహం_బఫరు_Create_With_Caps( size_t x_బఫరు_కొలత_బైట్లు,
                                                                    size_t x_Trigger_Level_బైట్లు,
                                                                    UBaseType_t ux_జ్ఞాపకం_Caps )
    {
        తిరిగి x_ప్రవాహం_బఫరు_Generic_Create_With_Caps( x_బఫరు_కొలత_బైట్లు, x_Trigger_Level_బైట్లు, pdFALSE, ux_జ్ఞాపకం_Caps );
    }

#అయితే_ముగింపు /* configSUPPORT_STATIC_ALLOCATION == 1 */

#ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )

/**
 * @brief Deletes a stream buffer previously created using
 * xStreamBufferCreateWithCaps()
 *
 * @param xStreamBuffer A handle to the stream buffer to be deleted.
 */
    స్థిర అంతఃవరుస శూన్య v_ప్రవాహం_బఫరు_Delete_With_Caps( ప్రవాహం_బఫరు_Handle_t x_ప్రవాహం_బఫరు )
    {
        v_ప్రవాహం_బఫరు_Generic_Delete_With_Caps( x_ప్రవాహం_బఫరు, pdFALSE );
    }

#అయితే_ముగింపు /* configSUPPORT_STATIC_ALLOCATION == 1 */

#ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )

/**
 * @brief Creates a message buffer with specific memory capabilities
 *
 * This function is similar to xMessageBufferCreate(), except that it allows the
 * memory allocated for the message buffer to have specific capabilities (e.g.,
 * MALLOC_CAP_INTERNAL).
 *
 * @note A message buffer created using this function must only be deleted using
 * vMessageBufferDeleteWithCaps()
 * @param xBufferSizeBytes The total number of bytes (not messages) the message
 * buffer will be able to hold at any one time.
 * @param uxMemoryCaps Memory capabilities of the message buffer's memory (see
 * esp_heap_caps.h)
 * @return Handle to the created message buffer or NULL on failure.
 */
    స్థిర అంతఃవరుస సందేశం_బఫరు_Handle_t x_సందేశం_బఫరు_Create_With_Caps( size_t x_బఫరు_కొలత_బైట్లు,
                                                                      UBaseType_t ux_జ్ఞాపకం_Caps )
    {
        తిరిగి ( సందేశం_బఫరు_Handle_t ) x_ప్రవాహం_బఫరు_Generic_Create_With_Caps( x_బఫరు_కొలత_బైట్లు, ( size_t ) 0, pdTRUE, ux_జ్ఞాపకం_Caps );
    }

#అయితే_ముగింపు /* configSUPPORT_STATIC_ALLOCATION == 1 */

#ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )

/**
 * @brief Deletes a stream buffer previously created using
 * xMessageBufferCreateWithCaps()
 *
 * @param xMessageBuffer A handle to the message buffer to be deleted.
 */
    స్థిర అంతఃవరుస శూన్య v_సందేశం_బఫరు_Delete_With_Caps( సందేశం_బఫరు_Handle_t x_సందేశం_బఫరు )
    {
        v_ప్రవాహం_బఫరు_Generic_Delete_With_Caps( ( ప్రవాహం_బఫరు_Handle_t ) x_సందేశం_బఫరు, pdTRUE );
    }

#అయితే_ముగింపు /* configSUPPORT_STATIC_ALLOCATION == 1 */

/* ------------------------------ Event Groups ------------------------------ */

#ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )

/**
 * @brief Creates an event group with specific memory capabilities
 *
 * This function is similar to xEventGroupCreate(), except that it allows the
 * memory allocated for the event group to have specific capabilities (e.g.,
 * MALLOC_CAP_INTERNAL).
 *
 * @note An event group created using this function must only be deleted using
 * vEventGroupDeleteWithCaps()
 * @param uxMemoryCaps Memory capabilities of the event group's memory (see
 * esp_heap_caps.h)
 * @return Handle to the created event group or NULL on failure.
 */
    Event_సమూహం_Handle_t x_Event_సమూహం_Create_With_Caps( UBaseType_t ux_జ్ఞాపకం_Caps );

#అయితే_ముగింపు /* configSUPPORT_STATIC_ALLOCATION == 1 */

#ఒకవేళ ( config_SUPPORT_STATIC_కేటాయింపు == 1 )

/**
 * @brief Deletes an event group previously created using
 * xEventGroupCreateWithCaps()
 *
 * @param xEventGroup A handle to the event group to be deleted.
 */
    శూన్య v_Event_సమూహం_Delete_With_Caps( Event_సమూహం_Handle_t x_Event_సమూహం );

#అయితే_ముగింపు /* configSUPPORT_STATIC_ALLOCATION == 1 */

/* --------------------------------------------------- Deprecated ------------------------------------------------------
 * Deprecated IDF FreeRTOS API additions.
 * Todo: Remove in v7.0 (IDF-13569)
 * ------------------------------------------------------------------------------------------------------------------ */
/** @cond */

uint8_t * px_Task_Get_దొంతర_ప్రారంభం( TaskHandle_t xTask ) __attribute__((deprecated("Use xTaskGetStackStart() for improved type safety")));

/** @endcond */

/* *INDENT-OFF* */
#నిర్వచితమైతే __cplusplus
    }
#అయితే_ముగింపు
/* *INDENT-ON* */
