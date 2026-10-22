/*
 * SPDX-FileCopyrightText: 2025-2026 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#समावेश <string.h>
#समावेश <stdlib.h>
#समावेश <stdio.h>
#समावेश <assert.h>
#समावेश <unistd.h>
#समावेश <execinfo.h>
#समावेश <signal.h>
#समावेश <pthread.h>

#समावेश "FreeRTOS.h"
#समावेश "task.h"
#समावेश "esp_log.h"
#समावेश "utils/linux_port_utils.h"

#घोषणा BACKTRACE_PC_ARRAY_आकार 20
#घोषणा FREERTOS_SIM_BACKTRACE_आकार 16
#घोषणा ON_SEGFAULT_MESSAGE "ERROR: Segmentation Fault, here's your backtrace:\n"
#घोषणा ON_ABORT_MESSAGE "ERROR: Aborted\n"

अटल व्योम linux_port_backtrace_handler(पूर्णांक sig)
{
    /* All calls here must be async-signal-safe (no stdio, no malloc).
     * write() and backtrace_symbols_fd() write directly to the fd. */
    व्योम *बफर[FREERTOS_SIM_BACKTRACE_आकार];
    पूर्णांक nptrs = backtrace(बफर, FREERTOS_SIM_BACKTRACE_आकार);

    स्थिर अक्षर *name = pcTaskGetName(शून्य); /* simple pointer dereference, no lock */
    ssize_t ignore __attribute__((unused));
    ignore = ग_लिखो(STDERR_FILENO, "=== Backtrace for task: ", 24);
    अगर (name) {
        माप_प्रकार len = 0;
        जबतक (name[len] != '\0') {
            len++;
        }
        ignore = ग_लिखो(STDERR_FILENO, name, len);
    }
    ignore = ग_लिखो(STDERR_FILENO, " ===\n", 5);
    backtrace_symbols_fd(बफर, nptrs, STDERR_FILENO);
}

व्योम linux_port_setup_backtrace_signal(व्योम)
{
    काष्ठा sigaction sa;
    sa.sa_handler = linux_port_backtrace_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa, शून्य);
}

व्योम linux_port_print_backtrace(व्योम)
{
    pthread_kill(linux_port_get_scheduled_task_pthread(), SIGUSR1);
}

ESP_LOG_ATTR_TAG(LINUX_TAG, "port_idf_linux");
ESP_LOG_ATTR_TAG(MAIN_TAG, "main_task");

#अगर (defined(__APPLE__) && defined(__MACH__))
प्रकार sig_t sighandler_t;
#पूर्ण_अगर

अटल अस्थिर UBaseType_t uxInterruptNesting = 0;
BaseType_t xPortCheckIfInISR(व्योम)
{
    वापस uxInterruptNesting;
}

#अगर CONFIG_COMPILER_OPTIMIZATION_DEBUG
#घोषणा BACKTRACE_PC_ARRAY_आकार_DUMMY 1
अटल व्योम load_libgcc(व्योम)
{
    व्योम *array[BACKTRACE_PC_ARRAY_आकार_DUMMY];
    माप_प्रकार आकार = backtrace(array, BACKTRACE_PC_ARRAY_आकार_DUMMY);
    निश्चित(आकार == 1);
}

अटल व्योम segfault_handler(पूर्णांक sig)
{
    व्योम *array[BACKTRACE_PC_ARRAY_आकार];
    माप_प्रकार आकार = backtrace(array, BACKTRACE_PC_ARRAY_आकार);
    ssize_t ignore __attribute__((unused));
    ignore = ग_लिखो(STDERR_FILENO, ON_SEGFAULT_MESSAGE, माप(ON_SEGFAULT_MESSAGE));
    backtrace_symbols_fd(array, आकार, STDERR_FILENO);
    _निकास(1);
}

अटल व्योम abort_handler(पूर्णांक sig)
{
    // we need a raw file write here because other functions are not async-signal-safe
    ssize_t ignore __attribute__((unused));
    ignore = ग_लिखो(STDERR_FILENO, ON_ABORT_MESSAGE, माप(ON_ABORT_MESSAGE));
    _निकास(1);
}
#पूर्ण_अगर // CONFIG_COMPILER_OPTIMIZATION_DEBUG

/*-----------------------------------------------------------
* Main FreeRTOS task
*-----------------------------------------------------------*/
बाह्य व्योम app_main(व्योम);
अटल व्योम main_task(व्योम *args)
{
    (व्योम)args;
    ESP_LOGI(MAIN_TAG, "Started on CPU%d", (पूर्णांक)xPortGetCoreID());

    ESP_LOGI(MAIN_TAG, "Calling app_main()");

    app_main();

    ESP_LOGI(MAIN_TAG, "Returned from app_main()");
    vTaskDelete(शून्य);
}

व्योम esp_startup_start_app(व्योम)
{
    रखो_भबफ(मानक_निकास, शून्य, _IONBF, 0);

#अगर CONFIG_COMPILER_OPTIMIZATION_DEBUG
    load_libgcc();
    sighandler_t sig_res;

    sig_res = संकेत(संक_अंश, segfault_handler);
    अगर (sig_res == संक_त्रुटि) {
        पात();
    }

    sig_res = संकेत(SIGABRT, abort_handler);
    अगर (sig_res == संक_त्रुटि) {
        पात();
    }
#पूर्ण_अगर

    // Create main_task using FreeRTOS API
    ESP_LOGI(LINUX_TAG, "Starting main task.");
    निश्चित(xTaskCreate(&main_task, "main", ESP_TASK_MAIN_ढेर, शून्य, ESP_TASK_MAIN_PRIO, शून्य) == pdTRUE);

    ESP_LOGI(LINUX_TAG, "Starting scheduler task.");
    vTaskStartScheduler();

    // Should never reach here
    निश्चित(false);
}

/*-----------------------------------------------------------
 * idle and tick hooks
 *-----------------------------------------------------------*/
#अगर (configUSE_IDLE_HOOK > 0)
व्योम vApplicationIdleHook(व्योम)
{
}
#पूर्ण_अगर

#अगर (configUSE_TICK_HOOK > 0)
व्योम vApplicationTickHook(व्योम)
{
    बाह्य व्योम esp_vApplicationTickHook(व्योम);
    esp_vApplicationTickHook();
}
#अन्यथा
#पूर्ण_अगर

/*-----------------------------------------------------------
 * Static allocation support
 *-----------------------------------------------------------*/
#अगर (configSUPPORT_STATIC_ALLOCATION == 1)

व्योम vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   ढेर_Type_t **ppx_Idle_Task_ढेर_बफर,
                                   uint32_t *pul_Idle_Task_ढेर_आकार)
{
    अटल StaticTask_t xIdleTaskTCB;
    अटल ढेर_Type_t ux_Idle_Task_ढेर[config_MINIMAL_ढेर_आकार];

    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppx_Idle_Task_ढेर_बफर = ux_Idle_Task_ढेर;
    *pul_Idle_Task_ढेर_आकार = config_MINIMAL_ढेर_आकार;
}

#अगर (configUSE_TIMERS == 1)
ढेर_Type_t ux_Timer_Task_ढेर[config_TIMER_TASK_ढेर_DEPTH];

व्योम vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    ढेर_Type_t **ppx_Timer_Task_ढेर_बफर,
                                    uint32_t *pul_Timer_Task_ढेर_आकार)
{
    अटल StaticTask_t xTimerTaskTCB;
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppx_Timer_Task_ढेर_बफर = ux_Timer_Task_ढेर;
    *pul_Timer_Task_ढेर_आकार = config_TIMER_TASK_ढेर_DEPTH;
}
#पूर्ण_अगर

#पूर्ण_अगर // configSUPPORT_STATIC_ALLOCATION

/*-----------------------------------------------------------
 * Stack overflow hook
 *-----------------------------------------------------------*/
व्योम __attribute__((weak)) v_Application_ढेर_Overflow_Hook(TaskHandle_t xTask, अक्षर *pcTaskName)
{
#घोषणा ERR_STR1 "***ERROR*** A stack overflow in task "
#घोषणा ERR_STR2 " has been detected."
    स्थिर अक्षर *str[] = {ERR_STR1, pcTaskName, ERR_STR2};

    अक्षर buf[माप(ERR_STR1) + CONFIG_FREERTOS_MAX_TASK_NAME_LEN + माप(ERR_STR2) + 1 /* null char */] = {0};

    अक्षर *dest = buf;
    क्रम (पूर्णांक i = 0; i < माप(str) / माप(str[0]); i++) {
        dest = म_जोड़ो(dest, str[i]);
    }
	म_लिखो("%s\n", buf);
    पात();
}
