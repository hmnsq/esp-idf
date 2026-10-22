/*
 * SPDX-FileCopyrightText: 2025-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#అన్తర్భావ <string.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <assert.h>
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <execinfo.h>
#అన్తర్భావ <signal.h>
#అన్తర్భావ <pthread.h>

#అన్తర్భావ "FreeRTOS.h"
#అన్తర్భావ "task.h"
#అన్తర్భావ "esp_log.h"
#అన్తర్భావ "utils/linux_port_utils.h"

#నిర్వచించు BACKTRACE_PC_ARRAY_కొలత 20
#నిర్వచించు FREERTOS_SIM_BACKTRACE_కొలత 16
#నిర్వచించు ON_SEGFAULT_సందేశం "ERROR: Segmentation Fault, here's your backtrace:\n"
#నిర్వచించు ON_ABORT_సందేశం "ERROR: Aborted\n"

స్థిర శూన్య linux_పోర్టు_backtrace_handler(పూర్ణాంకం sig)
{
    /* All calls here must be async-signal-safe (no stdio, no malloc).
     * write() and backtrace_symbols_fd() write directly to the fd. */
    శూన్య *బఫరు[FREERTOS_SIM_BACKTRACE_కొలత];
    పూర్ణాంకం nptrs = backtrace(బఫరు, FREERTOS_SIM_BACKTRACE_కొలత);

    స్థిరరాశి అక్షరం *name = pcTaskGetName(ఖాళీ); /* simple pointer dereference, no lock */
    ssize_t ignore __attribute__((వాడని));
    ignore = లిఖ(STDERR_FILENO, "=== Backtrace for task: ", 24);
    ఒకవేళ (name) {
        size_t len = 0;
        అయితే (name[len] != '\0') {
            len++;
        }
        ignore = లిఖ(STDERR_FILENO, name, len);
    }
    ignore = లిఖ(STDERR_FILENO, " ===\n", 5);
    backtrace_symbols_fd(బఫరు, nptrs, STDERR_FILENO);
}

శూన్య linux_పోర్టు_అమరిక_backtrace_signal(శూన్య)
{
    నిర్మాణం sigaction sa;
    sa.sa_handler = linux_పోర్టు_backtrace_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa, ఖాళీ);
}

శూన్య linux_పోర్టు_print_backtrace(శూన్య)
{
    pthread_kill(linux_పోర్టు_get_scheduled_task_pthread(), SIGUSR1);
}

ESP_LOG_ATTR_ట్యాగ్(LINUX_ట్యాగ్, "port_idf_linux");
ESP_LOG_ATTR_ట్యాగ్(MAIN_ట్యాగ్, "main_task");

#ఒకవేళ (defined(__APPLE__) && defined(__MACH__))
రకనిర్వచనం sig_t sighandler_t;
#అయితే_ముగింపు

స్థిర అస్థిర UBaseType_t ux_అంతరాయం_Nesting = 0;
పునాది_Type_t x_పోర్టు_Check_If_లోపల_ISR(శూన్య)
{
    తిరిగి ux_అంతరాయం_Nesting;
}

#ఒకవేళ CONFIG_COMPILER_OPTIMIZATION_DEBUG
#నిర్వచించు BACKTRACE_PC_ARRAY_కొలత_DUMMY 1
స్థిర శూన్య ఎక్కించు_libgcc(శూన్య)
{
    శూన్య *array[BACKTRACE_PC_ARRAY_కొలత_DUMMY];
    size_t కొలత = backtrace(array, BACKTRACE_PC_ARRAY_కొలత_DUMMY);
    ప్రతిజ్ఞా(కొలత == 1);
}

స్థిర శూన్య segfault_handler(పూర్ణాంకం sig)
{
    శూన్య *array[BACKTRACE_PC_ARRAY_కొలత];
    size_t కొలత = backtrace(array, BACKTRACE_PC_ARRAY_కొలత);
    ssize_t ignore __attribute__((వాడని));
    ignore = లిఖ(STDERR_FILENO, ON_SEGFAULT_సందేశం, పరిమాణం(ON_SEGFAULT_సందేశం));
    backtrace_symbols_fd(array, కొలత, STDERR_FILENO);
    _exit(1);
}

స్థిర శూన్య abort_handler(పూర్ణాంకం sig)
{
    // we need a raw file write here because other functions are not async-signal-safe
    ssize_t ignore __attribute__((వాడని));
    ignore = లిఖ(STDERR_FILENO, ON_ABORT_సందేశం, పరిమాణం(ON_ABORT_సందేశం));
    _exit(1);
}
#అయితే_ముగింపు // CONFIG_COMPILER_OPTIMIZATION_DEBUG

/*-----------------------------------------------------------
* Main FreeRTOS task
*-----------------------------------------------------------*/
బాహ్య శూన్య app_main(శూన్య);
స్థిర శూన్య main_task(శూన్య *వాదనలు)
{
    (శూన్య)వాదనలు;
    ESP_LOGI(MAIN_ట్యాగ్, "Started on CPU%d", (పూర్ణాంకం)x_పోర్టు_Get_Core_ID());

    ESP_LOGI(MAIN_ట్యాగ్, "Calling app_main()");

    app_main();

    ESP_LOGI(MAIN_ట్యాగ్, "Returned from app_main()");
    vTaskDelete(ఖాళీ);
}

శూన్య esp_startup_ప్రారంభం_app(శూన్య)
{
    setvbuf(stdout, ఖాళీ, _IONBF, 0);

#ఒకవేళ CONFIG_COMPILER_OPTIMIZATION_DEBUG
    ఎక్కించు_libgcc();
    sighandler_t sig_res;

    sig_res = సంకేతం(SIGSEGV, segfault_handler);
    ఒకవేళ (sig_res == SIG_ERR) {
        abort();
    }

    sig_res = సంకేతం(SIGABRT, abort_handler);
    ఒకవేళ (sig_res == SIG_ERR) {
        abort();
    }
#అయితే_ముగింపు

    // Create main_task using FreeRTOS API
    ESP_LOGI(LINUX_ట్యాగ్, "Starting main task.");
    ప్రతిజ్ఞా(xTaskCreate(&main_task, "main", ESP_TASK_MAIN_దొంతర, ఖాళీ, ESP_TASK_MAIN_PRIO, ఖాళీ) == pdTRUE);

    ESP_LOGI(LINUX_ట్యాగ్, "Starting scheduler task.");
    v_Task_ప్రారంభం_Scheduler();

    // Should never reach here
    ప్రతిజ్ఞా(false);
}

/*-----------------------------------------------------------
 * idle and tick hooks
 *-----------------------------------------------------------*/
#ఒకవేళ (configUSE_IDLE_HOOK > 0)
శూన్య vApplicationIdleHook(శూన్య)
{
}
#అయితే_ముగింపు

#ఒకవేళ (configUSE_TICK_HOOK > 0)
శూన్య vApplicationTickHook(శూన్య)
{
    బాహ్య శూన్య esp_vApplicationTickHook(శూన్య);
    esp_vApplicationTickHook();
}
#లేకపోతే
#అయితే_ముగింపు

/*-----------------------------------------------------------
 * Static allocation support
 *-----------------------------------------------------------*/
#ఒకవేళ (config_SUPPORT_STATIC_కేటాయింపు == 1)

శూన్య v_Application_Get_Idle_Task_జ్ఞాపకం(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   దొంతర_Type_t **ppx_Idle_Task_దొంతర_బఫరు,
                                   uint32_t *pul_Idle_Task_దొంతర_కొలత)
{
    స్థిర StaticTask_t xIdleTaskTCB;
    స్థిర దొంతర_Type_t ux_Idle_Task_దొంతర[config_కనీస_దొంతర_కొలత];

    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppx_Idle_Task_దొంతర_బఫరు = ux_Idle_Task_దొంతర;
    *pul_Idle_Task_దొంతర_కొలత = config_కనీస_దొంతర_కొలత;
}

#ఒకవేళ (configUSE_TIMERS == 1)
దొంతర_Type_t ux_Timer_Task_దొంతర[config_TIMER_TASK_దొంతర_లోతు];

శూన్య v_Application_Get_Timer_Task_జ్ఞాపకం(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    దొంతర_Type_t **ppx_Timer_Task_దొంతర_బఫరు,
                                    uint32_t *pul_Timer_Task_దొంతర_కొలత)
{
    స్థిర StaticTask_t xTimerTaskTCB;
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppx_Timer_Task_దొంతర_బఫరు = ux_Timer_Task_దొంతర;
    *pul_Timer_Task_దొంతర_కొలత = config_TIMER_TASK_దొంతర_లోతు;
}
#అయితే_ముగింపు

#అయితే_ముగింపు // configSUPPORT_STATIC_ALLOCATION

/*-----------------------------------------------------------
 * Stack overflow hook
 *-----------------------------------------------------------*/
శూన్య __attribute__((weak)) v_Application_దొంతర_అధికప్రవాహం_Hook(TaskHandle_t xTask, అక్షరం *pcTaskName)
{
#నిర్వచించు ERR_STR1 "***ERROR*** A stack overflow in task "
#నిర్వచించు ERR_STR2 " has been detected."
    స్థిరరాశి అక్షరం *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    అక్షరం buf[పరిమాణం(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + పరిమాణం(ERR_STR2) + 1 /* null char */] = {0};

    అక్షరం *dest = buf;
    కొరకు (పూర్ణాంకం i = 0; i < పరిమాణం(str) / పరిమాణం(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
	రాయి("%s\n", buf);
    abort();
}
