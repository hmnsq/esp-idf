/*
 * SPDX-FileCopyrightText: 2025-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#अन्तर्भाव <string.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <assert.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <execinfo.h>
#अन्तर्भाव <signal.h>
#अन्तर्भाव <pthread.h>

#अन्तर्भाव "FreeRTOS.h"
#अन्तर्भाव "task.h"
#अन्तर्भाव "esp_log.h"
#अन्तर्भाव "utils/linux_port_utils.h"

#परिभाषय BACKTRACE_PC_ARRAY_आकार 20
#परिभाषय FREERTOS_SIM_BACKTRACE_आकार 16
#परिभाषय ON_SEGFAULT_सन्देशः "ERROR: Segmentation Fault, here's your backtrace:\n"
#परिभाषय ON_ABORT_सन्देशः "ERROR: Aborted\n"

स्थिर शून्य linux_पत्तनम्_backtrace_handler(पूर्णाङ्क sig)
{
    /* All calls here must be async-signal-safe (no stdio, no malloc).
     * write() and backtrace_symbols_fd() write directly to the fd. */
    शून्य *बफर[FREERTOS_SIM_BACKTRACE_आकार];
    पूर्णाङ्क nptrs = backtrace(बफर, FREERTOS_SIM_BACKTRACE_आकार);

    ध्रुव वर्ण *name = pcTaskGetName(रिक्त); /* simple pointer dereference, no lock */
    ssize_t ignore __attribute__((अप्रयुक्तम्));
    ignore = लिख(STDERR_FILENO, "=== Backtrace for task: ", 24);
    यदि (name) {
        size_t len = 0;
        यावत् (name[len] != '\0') {
            len++;
        }
        ignore = लिख(STDERR_FILENO, name, len);
    }
    ignore = लिख(STDERR_FILENO, " ===\n", 5);
    backtrace_symbols_fd(बफर, nptrs, STDERR_FILENO);
}

शून्य linux_पत्तनम्_सज्जता_backtrace_signal(शून्य)
{
    संरचना sigaction sa;
    sa.sa_handler = linux_पत्तनम्_backtrace_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa, रिक्त);
}

शून्य linux_पत्तनम्_print_backtrace(शून्य)
{
    pthread_kill(linux_पत्तनम्_get_scheduled_task_pthread(), SIGUSR1);
}

ESP_LOG_ATTR_चिह्नकम्(LINUX_चिह्नकम्, "port_idf_linux");
ESP_LOG_ATTR_चिह्नकम्(MAIN_चिह्नकम्, "main_task");

#यदि (defined(__APPLE__) && defined(__MACH__))
प्रकारनिर्देश sig_t sighandler_t;
#यदि_अन्तः

स्थिर चञ्चल UBaseType_t ux_व्यवधानम्_Nesting = 0;
आधारः_Type_t x_पत्तनम्_Check_If_अन्तर्_ISR(शून्य)
{
    प्रत्यागम ux_व्यवधानम्_Nesting;
}

#यदि CONFIG_COMPILER_OPTIMIZATION_DEBUG
#परिभाषय BACKTRACE_PC_ARRAY_आकार_DUMMY 1
स्थिर शून्य आरोपय_libgcc(शून्य)
{
    शून्य *array[BACKTRACE_PC_ARRAY_आकार_DUMMY];
    size_t आकार = backtrace(array, BACKTRACE_PC_ARRAY_आकार_DUMMY);
    प्रतिज्ञा(आकार == 1);
}

स्थिर शून्य segfault_handler(पूर्णाङ्क sig)
{
    शून्य *array[BACKTRACE_PC_ARRAY_आकार];
    size_t आकार = backtrace(array, BACKTRACE_PC_ARRAY_आकार);
    ssize_t ignore __attribute__((अप्रयुक्तम्));
    ignore = लिख(STDERR_FILENO, ON_SEGFAULT_सन्देशः, परिमाण(ON_SEGFAULT_सन्देशः));
    backtrace_symbols_fd(array, आकार, STDERR_FILENO);
    _exit(1);
}

स्थिर शून्य abort_handler(पूर्णाङ्क sig)
{
    // we need a raw file write here because other functions are not async-signal-safe
    ssize_t ignore __attribute__((अप्रयुक्तम्));
    ignore = लिख(STDERR_FILENO, ON_ABORT_सन्देशः, परिमाण(ON_ABORT_सन्देशः));
    _exit(1);
}
#यदि_अन्तः // CONFIG_COMPILER_OPTIMIZATION_DEBUG

/*-----------------------------------------------------------
* Main FreeRTOS task
*-----------------------------------------------------------*/
बाह्य शून्य app_main(शून्य);
स्थिर शून्य main_task(शून्य *तर्काः)
{
    (शून्य)तर्काः;
    ESP_LOGI(MAIN_चिह्नकम्, "Started on CPU%d", (पूर्णाङ्क)x_पत्तनम्_Get_Core_ID());

    ESP_LOGI(MAIN_चिह्नकम्, "Calling app_main()");

    app_main();

    ESP_LOGI(MAIN_चिह्नकम्, "Returned from app_main()");
    vTaskDelete(रिक्त);
}

शून्य esp_startup_start_app(शून्य)
{
    setvbuf(stdout, रिक्त, _IONBF, 0);

#यदि CONFIG_COMPILER_OPTIMIZATION_DEBUG
    आरोपय_libgcc();
    sighandler_t sig_res;

    sig_res = सङ्केत(SIGSEGV, segfault_handler);
    यदि (sig_res == SIG_ERR) {
        abort();
    }

    sig_res = सङ्केत(SIGABRT, abort_handler);
    यदि (sig_res == SIG_ERR) {
        abort();
    }
#यदि_अन्तः

    // Create main_task using FreeRTOS API
    ESP_LOGI(LINUX_चिह्नकम्, "Starting main task.");
    प्रतिज्ञा(xTaskCreate(&main_task, "main", ESP_TASK_MAIN_राशिः, रिक्त, ESP_TASK_MAIN_PRIO, रिक्त) == pdTRUE);

    ESP_LOGI(LINUX_चिह्नकम्, "Starting scheduler task.");
    vTaskStartScheduler();

    // Should never reach here
    प्रतिज्ञा(false);
}

/*-----------------------------------------------------------
 * idle and tick hooks
 *-----------------------------------------------------------*/
#यदि (configUSE_IDLE_HOOK > 0)
शून्य vApplicationIdleHook(शून्य)
{
}
#यदि_अन्तः

#यदि (configUSE_TICK_HOOK > 0)
शून्य vApplicationTickHook(शून्य)
{
    बाह्य शून्य esp_vApplicationTickHook(शून्य);
    esp_vApplicationTickHook();
}
#अन्यथा
#यदि_अन्तः

/*-----------------------------------------------------------
 * Static allocation support
 *-----------------------------------------------------------*/
#यदि (config_SUPPORT_STATIC_आवण्टनम् == 1)

शून्य v_Application_Get_Idle_Task_स्मृतिः(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   राशिः_Type_t **ppx_Idle_Task_राशिः_बफर,
                                   uint32_t *pul_Idle_Task_राशिः_आकार)
{
    स्थिर StaticTask_t xIdleTaskTCB;
    स्थिर राशिः_Type_t ux_Idle_Task_राशिः[config_अल्पिष्ठम्_राशिः_आकार];

    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppx_Idle_Task_राशिः_बफर = ux_Idle_Task_राशिः;
    *pul_Idle_Task_राशिः_आकार = config_अल्पिष्ठम्_राशिः_आकार;
}

#यदि (configUSE_TIMERS == 1)
राशिः_Type_t ux_Timer_Task_राशिः[config_TIMER_TASK_राशिः_गभीरता];

शून्य v_Application_Get_Timer_Task_स्मृतिः(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    राशिः_Type_t **ppx_Timer_Task_राशिः_बफर,
                                    uint32_t *pul_Timer_Task_राशिः_आकार)
{
    स्थिर StaticTask_t xTimerTaskTCB;
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppx_Timer_Task_राशिः_बफर = ux_Timer_Task_राशिः;
    *pul_Timer_Task_राशिः_आकार = config_TIMER_TASK_राशिः_गभीरता;
}
#यदि_अन्तः

#यदि_अन्तः // configSUPPORT_STATIC_ALLOCATION

/*-----------------------------------------------------------
 * Stack overflow hook
 *-----------------------------------------------------------*/
शून्य __attribute__((weak)) v_Application_राशिः_अतिप्रवाहः_Hook(TaskHandle_t xTask, वर्ण *pcTaskName)
{
#परिभाषय ERR_STR1 "***ERROR*** A stack overflow in task "
#परिभाषय ERR_STR2 " has been detected."
    ध्रुव वर्ण *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    वर्ण buf[परिमाण(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + परिमाण(ERR_STR2) + 1 /* null char */] = {0};

    वर्ण *dest = buf;
    प्रति (पूर्णाङ्क i = 0; i < परिमाण(str) / परिमाण(str[0]); i++) {
        dest = वाक्यं_योजय(dest, str[i]);
    }
	प्रारूप_लिख("%s\n", buf);
    abort();
}
