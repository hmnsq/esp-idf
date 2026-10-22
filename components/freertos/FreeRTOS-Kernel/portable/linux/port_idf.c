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

स्थिर शून्य linux_port_backtrace_handler(पूर्णांक sig)
{
    /* All calls here must be async-signal-safe (no stdio, no malloc).
     * write() and backtrace_symbols_fd() write directly to the fd. */
    शून्य *buffer[FREERTOS_SIM_BACKTRACE_SIZE];
    पूर्णांक nptrs = backtrace(buffer, FREERTOS_SIM_BACKTRACE_SIZE);

    स्थिरांक अक्षर *name = pcTaskGetName(रिक्त); /* simple pointer dereference, no lock */
    ssize_t ignore __attribute__((unused));
    ignore = लिख(STDERR_FILENO, "=== Backtrace for task: ", 24);
    जर (name) {
        माप_प्रकार len = 0;
        पर्यंत (name[len] != '\0') {
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

#जर (defined(__APPLE__) && defined(__MACH__))
प्रकारव्याख्या sig_t sighandler_t;
#endif

स्थिर अस्थिर UBaseType_t uxInterruptNesting = 0;
BaseType_t xPortCheckIfInISR(शून्य)
{
    परत uxInterruptNesting;
}

#जर CONFIG_COMPILER_OPTIMIZATION_DEBUG
#define BACKTRACE_PC_ARRAY_SIZE_DUMMY 1
स्थिर शून्य load_libgcc(शून्य)
{
    शून्य *array[BACKTRACE_PC_ARRAY_SIZE_DUMMY];
    माप_प्रकार size = backtrace(array, BACKTRACE_PC_ARRAY_SIZE_DUMMY);
    प्रतिज्ञा(size == 1);
}

स्थिर शून्य segfault_handler(पूर्णांक sig)
{
    शून्य *array[BACKTRACE_PC_ARRAY_SIZE];
    माप_प्रकार size = backtrace(array, BACKTRACE_PC_ARRAY_SIZE);
    ssize_t ignore __attribute__((unused));
    ignore = लिख(STDERR_FILENO, ON_SEGFAULT_MESSAGE, आकार(ON_SEGFAULT_MESSAGE));
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    _exit(1);
}

स्थिर शून्य abort_handler(पूर्णांक sig)
{
    // we need a raw file write here because other functions are not async-signal-safe
    ssize_t ignore __attribute__((unused));
    ignore = लिख(STDERR_FILENO, ON_ABORT_MESSAGE, आकार(ON_ABORT_MESSAGE));
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
    ESP_LOGI(MAIN_TAG, "Started on CPU%d", (पूर्णांक)xPortGetCoreID());

    ESP_LOGI(MAIN_TAG, "Calling app_main()");

    app_main();

    ESP_LOGI(MAIN_TAG, "Returned from app_main()");
    vTaskDelete(रिक्त);
}

शून्य esp_startup_start_app(शून्य)
{
    रखो_भबफ(मानक_निकास, रिक्त, _IONBF, 0);

#जर CONFIG_COMPILER_OPTIMIZATION_DEBUG
    load_libgcc();
    sighandler_t sig_res;

    sig_res = संकेत(संक_अंश, segfault_handler);
    जर (sig_res == संक_त्रुटि) {
        पात();
    }

    sig_res = संकेत(SIGABRT, abort_handler);
    जर (sig_res == संक_त्रुटि) {
        पात();
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
#जर (configUSE_IDLE_HOOK > 0)
शून्य vApplicationIdleHook(शून्य)
{
}
#endif

#जर (configUSE_TICK_HOOK > 0)
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
#जर (configSUPPORT_STATIC_ALLOCATION == 1)

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

#जर (configUSE_TIMERS == 1)
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
शून्य __attribute__((weak)) v_Application_चळत_Overflow_Hook(TaskHandle_t xTask, अक्षर *pcTaskName)
{
#define ERR_STR1 "***ERROR*** A stack overflow in task "
#define ERR_STR2 " has been detected."
    स्थिरांक अक्षर *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    अक्षर buf[आकार(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + आकार(ERR_STR2) + 1 /* null char */] = {0};

    अक्षर *dest = buf;
    साठी (पूर्णांक i = 0; i < आकार(str) / आकार(str[0]); i++) {
        dest = म_जोड़ो(dest, str[i]);
    }
	म_लिखो("%s\n", buf);
    पात();
}
