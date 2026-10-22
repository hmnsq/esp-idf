/*
 * SPDX-FileCopyrightText: 2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#సూచన once

#అన్తర్భావ "sdkconfig.h"
#అన్తర్భావ "freertos/FreeRTOS.h"
#అన్తర్భావ "freertos/task.h"

/*
 * This header contains public API for FreeRTOS debugging features including task snapshots.
 * These APIs are used by ESP-IDF debugging features (e.g., esp_gdbstub, core dump) and
 * external frameworks (e.g., ESP Insights) for debugging and monitoring purposes.
 */

/* *INDENT-OFF* */
#నిర్వచితమైతే __cplusplus
    బాహ్య "C" {
#అయితే_ముగింపు
/* *INDENT-ON* */

/* -------------------------------------------------- Task Snapshot ------------------------------------------------- */

/**
 * @brief Task Snapshot structure
 *
 * - Used with the uxTaskGetSnapshotAll() function to save memory snapshot of each task in the system.
 * - We need this structure because TCB_t is defined (hidden) in tasks.c.
 */
రకనిర్వచనం నిర్మాణం xTASK_SNAPSHOT
{
    శూన్య * pxTCB;               /*!< Address of the task control block. */
    దొంతర_Type_t * px_శిఖరం_యొక్క_దొంతర; /*!< Points to the location of the last item placed on the tasks stack. */
    దొంతర_Type_t * px_చివర_యొక్క_దొంతర; /*!< Points to the end of the stack. pxTopOfStack < pxEndOfStack, stack grows hi2lo
                                 *  pxTopOfStack > pxEndOfStack, stack grows lo2hi*/
} TaskSnapshot_t;

/**
 * @brief Task Snapshot iterator
 *
 * Used in xTaskGetNext(). Must be zero/null initialized on the first call.
 */
రకనిర్వచనం నిర్మాణం TaskIterator
{
    UBaseType_t ux_Current_List_సూచీ; /**< Current task list index being traversed. */
    List_అంశం_t * px_తదుపరి_List_అంశం;    /**< Next task list item will being traversed. */
    TaskHandle_t pxTaskHandle;      /**< Current task handle being traversed. */
} TaskIterator_t;

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
 *
 */
పూర్ణాంకం x_Task_Get_తదుపరి( TaskIterator_t * xIterator );

/**
 * @brief Fill a TaskSnapshot_t structure for specified task.
 *
 * - This function is used by the panic handler to get the snapshot of a particular task.
 *
 * @note This function should only be called when FreeRTOS is no longer running (e.g., during a panic) as this function
 *       does not acquire any locks. For safe usage with scheduler running, use vTaskSuspendAll() before calling
 *       this function and xTaskResumeAll() after.
 *
 * @param[in] pxTask Task's handle
 * @param[out] pxTaskSnapshot Snapshot of the task
 * @return pdTRUE if operation was successful else pdFALSE
 */
పునాది_Type_t vTaskGetSnapshot( TaskHandle_t pxTask,
                             TaskSnapshot_t * pxTaskSnapshot );

/**
 * @brief Fill an array of TaskSnapshot_t structures for every task in the system
 *
 * - This function is used by the panic handler to get a snapshot of all tasks in the system
 *
 * @note This function should only be called when FreeRTOS is no longer running (e.g., during a panic) as this function
 *        does not acquire any locks. For safe usage with scheduler running, use vTaskSuspendAll() before calling
 *        this function and xTaskResumeAll() after.
 *
 * @param[out] pxTaskSnapshotArray Array of TaskSnapshot_t structures filled by this function
 * @param[in] uxArrayLength Length of the provided array
 * @param[out] pxTCBSize Size of the a task's TCB structure (can be set to NULL)
 * @return UBaseType_t Number of task snapshots filled in the array
 */
UBaseType_t uxTaskGetSnapshotAll( TaskSnapshot_t * స్థిరరాశి pxTaskSnapshotArray,
                                  స్థిరరాశి UBaseType_t ux_Array_నిడివి,
                                  UBaseType_t * స్థిరరాశి pxTCBSize );

/* ----------------------------------------------------- Misc ----------------------------------------------------- */

/**
 * @brief Get a void pointer to the current TCB of a particular core
 *
 * @note This function provides no guarantee that the return TCB will still be the current task (or that the task still
 * exists) when it returns. It is the caller's responsibility to ensure that the task does not get scheduled or deleted.
 * @param xCoreID The core to query
 * @return Void pointer to current TCB
 */
శూన్య * pvTaskGetCurrentTCBForCore( పునాది_Type_t xCoreID );

/* *INDENT-OFF* */
#నిర్వచితమైతే __cplusplus
    }
#అయితే_ముగింపు
/* *INDENT-ON* */
