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

#define BACKTRACE_PC_ARRAY_SIZE 20
#define FREERTOS_SIM_BACKTRACE_SIZE 16
#define ON_SEGFAULT_MESSAGE "ERROR: Segmentation Fault, here's your backtrace:\n"
#define ON_ABORT_MESSAGE "ERROR: Aborted\n"

स्थिर शून्य linux_port_backtrace_handler(पूर्णाङ्क sig)
{
    /* All calls here must be async-signal-safe (no stdio, no malloc).
     * write() and backtrace_symbols_fd() write directly to the fd. */
    शून्य *buffer[FREERTOS_SIM_BACKTRACE_SIZE];
    पूर्णाङ्क nptrs = backtrace(buffer, FREERTOS_SIM_BACKTRACE_SIZE);

    ध्रुव वर्ण *name = pcTaskGetName(रिक्त); /* simple pointer dereference, no lock */
    ssize_t ignore __attribute__((unused));
    ignore = लिख(STDERR_FILENO, "=== Backtrace for task: ", 24);
    यदि (name) {
        size_t len = 0;
        यावत् (name[len] != '\0') {
            len++;
        }
        ignore = लिख(STDERR_FILENO, name, len);
    }
    ignore = लिख(STDERR_FILENO, " ===\n", 5);
    backtrace_symbols_fd(buffer, nptrs, STDERR_FILENO);
}

शून्य linux_port_setup_backtrace_signal(शून्य)
{
    संरचना sigaction sa;
    sa.sa_handler = linux_port_backtrace_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa, रिक्त);
}

शून्य linux_port_print_backtrace(शून्य)
{
    pthread_kill(linux_port_get_scheduled_task_pthread(), SIGUSR1);
}

ESP_LOG_ATTR_TAG(LINUX_TAG, "port_idf_linux");
ESP_LOG_ATTR_TAG(MAIN_TAG, "main_task");

#यदि (defined(__APPLE__) && defined(__MACH__))
प्रकारनिर्देश sig_t sighandler_t;
#endif

स्थिर चञ्चल UBaseType_t uxInterruptNesting = 0;
BaseType_t xPortCheckIfInISR(शून्य)
{
    प्रत्यागम uxInterruptNesting;
}

#यदि CONFIG_COMPILER_OPTIMIZATION_DEBUG
#define BACKTRACE_PC_ARRAY_SIZE_DUMMY 1
स्थिर शून्य load_libgcc(शून्य)
{
    शून्य *array[BACKTRACE_PC_ARRAY_SIZE_DUMMY];
    size_t size = backtrace(array, BACKTRACE_PC_ARRAY_SIZE_DUMMY);
    प्रतिज्ञा(size == 1);
}

स्थिर शून्य segfault_handler(पूर्णाङ्क sig)
{
    शून्य *array[BACKTRACE_PC_ARRAY_SIZE];
    size_t size = backtrace(array, BACKTRACE_PC_ARRAY_SIZE);
    ssize_t ignore __attribute__((unused));
    ignore = लिख(STDERR_FILENO, ON_SEGFAULT_MESSAGE, परिमाण(ON_SEGFAULT_MESSAGE));
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    _exit(1);
}

स्थिर शून्य abort_handler(पूर्णाङ्क sig)
{
    // we need a raw file write here because other functions are not async-signal-safe
    ssize_t ignore __attribute__((unused));
    ignore = लिख(STDERR_FILENO, ON_ABORT_MESSAGE, परिमाण(ON_ABORT_MESSAGE));
    _exit(1);
}
#endif // CONFIG_COMPILER_OPTIMIZATION_DEBUG

/*-----------------------------------------------------------
* Main FreeRTOS task
*-----------------------------------------------------------*/
बाह्य शून्य app_main(शून्य);
स्थिर शून्य main_task(शून्य *args)
{
    (शून्य)args;
    ESP_LOGI(MAIN_TAG, "Started on CPU%d", (पूर्णाङ्क)xPortGetCoreID());

    ESP_LOGI(MAIN_TAG, "Calling app_main()");

    app_main();

    ESP_LOGI(MAIN_TAG, "Returned from app_main()");
    vTaskDelete(रिक्त);
}

शून्य esp_startup_start_app(शून्य)
{
    setvbuf(stdout, रिक्त, _IONBF, 0);

#यदि CONFIG_COMPILER_OPTIMIZATION_DEBUG
    load_libgcc();
    sighandler_t sig_res;

    sig_res = सङ्केत(SIGSEGV, segfault_handler);
    यदि (sig_res == SIG_ERR) {
        abort();
    }

    sig_res = सङ्केत(SIGABRT, abort_handler);
    यदि (sig_res == SIG_ERR) {
        abort();
    }
#endif

    // Create main_task using FreeRTOS API
    ESP_LOGI(LINUX_TAG, "Starting main task.");
    प्रतिज्ञा(xTaskCreate(&main_task, "main", ESP_TASK_MAIN_चळत, रिक्त, ESP_TASK_MAIN_PRIO, रिक्त) == pdTRUE);

    ESP_LOGI(LINUX_TAG, "Starting scheduler task.");
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
#endif

#यदि (configUSE_TICK_HOOK > 0)
शून्य vApplicationTickHook(शून्य)
{
    बाह्य शून्य esp_vApplicationTickHook(शून्य);
    esp_vApplicationTickHook();
}
#अन्यथा
#endif

/*-----------------------------------------------------------
 * Static allocation support
 *-----------------------------------------------------------*/
#यदि (configSUPPORT_STATIC_ALLOCATION == 1)

शून्य vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   चळत_Type_t **ppx_Idle_Task_चळत_Buffer,
                                   uint32_t *pul_Idle_Task_चळत_Size)
{
    स्थिर StaticTask_t xIdleTaskTCB;
    स्थिर चळत_Type_t ux_Idle_Task_चळत[config_MINIMAL_चळत_SIZE];

    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppx_Idle_Task_चळत_Buffer = ux_Idle_Task_चळत;
    *pul_Idle_Task_चळत_Size = config_MINIMAL_चळत_SIZE;
}

#यदि (configUSE_TIMERS == 1)
चळत_Type_t ux_Timer_Task_चळत[config_TIMER_TASK_चळत_DEPTH];

शून्य vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    चळत_Type_t **ppx_Timer_Task_चळत_Buffer,
                                    uint32_t *pul_Timer_Task_चळत_Size)
{
    स्थिर StaticTask_t xTimerTaskTCB;
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppx_Timer_Task_चळत_Buffer = ux_Timer_Task_चळत;
    *pul_Timer_Task_चळत_Size = config_TIMER_TASK_चळत_DEPTH;
}
#endif

#endif // configSUPPORT_STATIC_ALLOCATION

/*-----------------------------------------------------------
 * Stack overflow hook
 *-----------------------------------------------------------*/
शून्य __attribute__((weak)) v_Application_चळत_Overflow_Hook(TaskHandle_t xTask, वर्ण *pcTaskName)
{
#define ERR_STR1 "***ERROR*** A stack overflow in task "
#define ERR_STR2 " has been detected."
    ध्रुव वर्ण *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    वर्ण buf[परिमाण(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + परिमाण(ERR_STR2) + 1 /* null char */] = {0};

    वर्ण *dest = buf;
    प्रति (पूर्णाङ्क i = 0; i < परिमाण(str) / परिमाण(str[0]); i++) {
        dest = strcat(dest, str[i]);
    }
	printf("%s\n", buf);
    abort();
}
